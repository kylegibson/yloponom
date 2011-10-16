/////////////////////////////////////////////////////////////////////////////
// GameRules.C
//
// Contains the implementation for the GameRules class which provides
// case-specific methods for modifying the current GameState based on the
// Yloponom ruleset
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <unistd.h>
#include "GameRules.h"

GameRules::GameRules(net::GameState* state) : fState(state),
											  fBase(state->get_base()),
											  fLastCard(0), fDoubleCnt(0),
											  fPurchaseProp(0), fMoveTo(0),
											  fRoll(0)
{
}

GameRules::~GameRules()
{
}

GameRules::EGR_result GameRules::turn_start()
{
	const PlayerState& cur_plr = fState->get_player(fState->get_current_turn());

	// zero out turn state data
	fRoll = fMoveTo = fDoubleCnt = fPurchaseProp = 0;

	// if the player is in jail
	if (kFree < cur_plr.get_jail_status()) return kInJail;

	// if the player has no assets whatsoever, force them to concede
	if (0 == cur_plr.get_money())
	{
		const net::PropertyList_t & props = cur_plr.get_owned_properties();
		
		// if the player has no money or properties they have no assets
		if (props.empty()) return kConcede;		
	}

	return kContinue;
}

bool GameRules::improve(int id, int level)
{
	vector<unsigned> vec;
	unsigned num = fBase.group_members(id, vec);
	unsigned cost = 0;

	// get a ref to the current player
	PlayerState& cur_plr = fState->get_player(fState->get_current_turn());

	// if id isn't from a group, return 0
	if (!num) return false;

	// check if this is a sell-off
	if (level < fState->get_property(id).get_level())
	{
		// calculate the returns
		for (unsigned i = 0; i < num; ++i)
		{
			// calculate the returns from a single prop
			for (int j = level; j < fState->get_property(vec[i]).get_level(); ++j)
				cost += (fBase.upgrade(vec[i], j) / 2);
			
			// apply the changes to gamestate
			fState->get_property(vec[i]).set_level(level);
		}

		// add the returns to the player's account
		cur_plr.add_money(cost);

		return true;
	}

	// calculate the upgrade cost for the group
	for (unsigned i = 0; i < num; ++i)
		for (int j = fState->get_property(id).get_level(); j < level; ++j)
			cost += fBase.upgrade(vec[i], j);

	// abort if insufficient funds
	if (cur_plr.get_money() < cost) return false;

	// apply the upgrades to the GameState
	for (unsigned i = 0; i < num; ++i)
		fState->get_property(vec[i]).set_level(level);

	// deduct from the player's account
	cur_plr.sub_money(cost);

	return true;
}

int GameRules::trade(net::Trade* trade)
{
	// ids of the two players in the transaction
	unsigned n_src = trade->get_source_player();
	unsigned n_des = trade->get_destin_player();

	// refs of the two players in the transaction
	PlayerState& p_src = fState->get_player(n_src);
	PlayerState& p_des = fState->get_player(n_des);

	// refs to the lists of properties being traded
	const net::PropertyList_t& l_src = trade->get_requested_properties();
	const net::PropertyList_t& l_des = trade->get_offered_properties();

	// iterator for the prop lists
	net::PropertyList_t::const_iterator it;

	// adjust the source player's money
	p_src.add_money(trade->get_request_money());
	p_src.sub_money(trade->get_offered_money());

	// adjust the dest player's money
	p_des.add_money(trade->get_offered_money());
	p_des.sub_money(trade->get_request_money());

	// iterate through the requested list
	for (it = l_src.begin(); it != l_src.end(); ++it)
	{
		// change the owner in gamestate
		fState->get_property(*it).set_owner(n_src);
		
		// add the property to the source player's list
		p_src.add_property(*it);

		// remove the property from the dest player's list
		p_des.del_property(*it);
	}

	// iterate through the offered list
	for (it = l_des.begin(); it != l_des.end(); ++it)
	{
		// change the owner in gamestate
		fState->get_property(*it).set_owner(n_des);

		// add the property to the dest player's list
		p_des.add_property(*it);

		// remove the property from the source player's list
		p_src.del_property(*it);
	}

	return 0;
}

GameRules::EGR_result GameRules::roll(int& d1, int& d2)
{
	PlayerState& plr = fState->get_player(fState->get_current_turn());
	
	unsigned die1 = fRand.roll_d6();
	usleep(500);
	unsigned die2 = fRand.roll_d6();

	// check for doubles and going to jail from too many
	if (die1 == die2) ++fDoubleCnt;
	if (2 < fDoubleCnt)
	{
		// reset fDoubleCnt
		fDoubleCnt = 0;

		// move the player to jail
		plr.set_position(10);

		// set player's jail status to kFreshman
		plr.set_jail_status(kFreshman);

		return kToJail;
	}

	// schedule the movement
	fRoll = die1 + die2;
	fMoveTo = bind_move(plr.get_position() + fRoll);

	// return
	d1 = die1;
	d2 = die2;
	return kContinue;
}

GameRules::EGR_move GameRules::move()
{
	PlayerState& plr = fState->get_player(fState->get_current_turn());
	unsigned o_pos = plr.get_position();

	// move the player
	plr.set_position(fMoveTo);

	// check for passing either GO
	if (o_pos > fMoveTo)					// outter go
		plr.add_money(fBase.price(0));
	
	if (40 > o_pos && 39 < fMoveTo)			// inner go, generally
		plr.add_money(fBase.price(40));
	
	if (40 < o_pos && 39 < fMoveTo && o_pos > fMoveTo)	// inner go, weird
		plr.add_money(fBase.price(40));

	if (40 > o_pos && 40 > fMoveTo && o_pos > fMoveTo) // inner go, weird 2
		plr.add_money(fBase.price(40));

	// check property type for simple cases and respond accordingly
	switch (fBase.type(fMoveTo))
	{
		case Game_Base::kGo:
		case Game_Base::kJail:
			return kNone;
		case Game_Base::kITax:
		case Game_Base::kLTax:
			return kPay;
		case Game_Base::kChance:
		case Game_Base::kCChest:
			return kCrd;
		case Game_Base::kFParking:
			return kFP;
		case Game_Base::kGoJail:
			return kGTJ;
		case Game_Base::kCasino:
			return kCas;
	}

	// at this point the space is either an owned or unowned property
	
	// if it is a railroad the player must transfer
	if (Game_Base::kRRoad == fBase.type(fMoveTo))
		plr.set_position(transfer(fMoveTo));
	

	// if it is owned, schedule the player to pay rent if player is not the owner
	if (fState->get_property(fMoveTo).has_owner())
		if (fState->get_property(fMoveTo).get_owner() == plr.get_id())
			return kNone;
		else return kPay;

	// otherwise, schedule the prop for purchase
	fPurchaseProp = fMoveTo;
	return kBuy;
}

GameRules::EGR_result GameRules::pay(int& amt)
{
	PlayerState& plr = fState->get_player(fState->get_current_turn());
	PlayerState* onr = NULL; 
	unsigned pay = 0;

	// check to see if we are dealing with an ownable property
	if (fState->get_property(fMoveTo).has_owner())
		onr = &(fState->get_player(fState->get_property(fMoveTo).get_owner()));

	// figure out how much the player owes
	switch (fBase.type(fMoveTo))
	{
		case Game_Base::kITax:		// percentage tax
			pay = plr.get_money() * fBase.price(fMoveTo) / 100;
			break;
		case Game_Base::kLTax:		// flat tax
			pay = fBase.price(fMoveTo);
			break;
		case Game_Base::kUtil:		// utility
			pay = fRoll * count_mems(*onr, fMoveTo) * 4;
			break;
		case Game_Base::kRRoad:	// rail road
			pay = fBase.rent(fMoveTo, count_mems(*onr, fMoveTo) - 1);
			break;
		case Game_Base::kProp:		// property
			pay = fBase.rent(fMoveTo, fState->get_property(fMoveTo).get_level());
			if (fState->get_property(fMoveTo).get_level() == 0 &&
				owns_group((unsigned)onr->get_id(), fBase.group(fMoveTo)))
				pay *= 2;

			break;
	}

	// return the amount to be paid
	amt = pay;

	// check for insufficient assets
	if (plr.get_money() < pay)
		if (!free(plr.get_id(), pay))	// if still insufficient
		{
			if (Game_Base::kUtil <= fBase.type(fMoveTo))
				onr->add_money(plr.get_money());		// tranfer whatever they have
			return kConcede;						// and send concede signal
		}

	// if they have enough, transfer funds to owner
	plr.sub_money(pay);
	if (Game_Base::kUtil <= fBase.type(fMoveTo)) onr->add_money(pay);

	return kDone;
}

GameRules::EGR_result GameRules::card(unsigned& id, int& amt)
{
	PlayerState& plr = fState->get_player(fState->get_current_turn());
	const net::GameState::PlayerStateList& plr_lst = fState->get_player_list();
	net::GameState::PlayerStateList::const_iterator it;
	unsigned deck = (Game_Base::kChance == fBase.type(fMoveTo)) ? 0 : 1;
	const Game_Base::GB_Card* card = fBase.draw_card(deck, &fLastCard);
	unsigned pay = 0;

	// zero the amount
	amt = 0;

	// debug message
	if (!card)
	{
		std::cout << "\nGameRules::card(...) - " << fLastCard << " is an invalid card. redrawing...\n";
		while (!card) card = fBase.draw_card(deck, &fLastCard);
	}

	// make sure the GOoJF card isn't already in play, drawing again as necessary
	if (Game_Base::kOutJail == card->type && fState->get_property(255 - deck).has_owner())
		while (Game_Base::kOutJail) card = fBase.draw_card(deck, &fLastCard);

	// return the card id
	id = fLastCard;
	
	// handle the simple cases first
	switch (card->type)
	{
		case Game_Base::kGet:
			plr.add_money(card->value);	// simply add value to player account
			return kDone;
		
		case Game_Base::kLose:
			if (plr.get_money() < (unsigned)card->value) // check funds
				if (!free(plr.get_id(), card->value)) return kConcede;
			plr.sub_money(card->value);					// subtract from account
			pay = card->value * -1;
			return kDone;
		
		case Game_Base::kMove:
			fMoveTo = bind_move(card->value);	// queue the move
			return kContinue;
		
		case Game_Base::kMoveRel:
			fMoveTo = bind_move(plr.get_position() + card->value);
			return kContinue;
		
		case Game_Base::kOthersPaid:
			pay = card->value * (fState->get_player_count() - 1);// calc total cost
			amt = pay * -1;
			if (plr.get_money() < pay)	// check funds
				if (!free(plr.get_id(), pay))
				{
					pay = plr.get_money() / (fState->get_player_count() - 1);
					for (it = plr_lst.begin(); it != plr_lst.end(); ++it)
						if (plr.get_id() != it->get_id())
							//TODO only necessary because get_player_list() is declared const
							fState->get_player(it->get_id()).add_money(card->value);
					return kConcede;
				}
			for (it = plr_lst.begin(); it != plr_lst.end(); ++it)
				if (plr.get_id() != it->get_id())
					//TODO only necessary because get_player_list() is declared const
					fState->get_player(it->get_id()).add_money(card->value);
			plr.sub_money(pay);
			return kDone;
		
		case Game_Base::kOthersGet:
			for (it = plr_lst.begin(); it != plr_lst.end(); ++it)
				if (plr.get_id() != it->get_id())
					//TODO only necessary because get_player_list() is declared const
					fState->get_player(it->get_id()).add_money(card->value);
			return kDone;
		
		case Game_Base::kAllGet:
			for (it = plr_lst.begin(); it != plr_lst.end(); ++it)
				// TODO this is necessary because get_player_list() is declared const
				fState->get_player(it->get_id()).add_money(card->value);
			return kDone;
		
		case Game_Base::kOutJail:
			fState->get_property(255 - deck).set_owner(plr.get_id()); // take ownership in state
			plr.add_property(255 - deck);							  // add to player state
			return kDone;
		
		case Game_Base::kInJail:
			fDoubleCnt = 0;						// reset double count
			plr.set_position(10);				// move to jail
			plr.set_jail_status(kFreshman);		// set jail status
			return kDone;

		case Game_Base::kOthersLose:
			for (it = plr_lst.begin(); it != plr_lst.end(); ++it)
			{
				if (plr.get_id() != it->get_id())
				{
					if (it->get_money() < (unsigned)card->value)
						if (!free(it->get_id(), card->value))
							fState->get_player(it->get_id()).kill();
					fState->get_player(it->get_id()).sub_money(card->value);
				}
			}
			return kDone;

		case Game_Base::kOthersPay:
			for (it = plr_lst.begin(), amt = 0; it != plr_lst.end(); ++it)
			{
				if (plr.get_id() != it->get_id())
				{
					if (it->get_money() < (unsigned)card->value)
					{
						if (!free(it->get_id(), card->value))
						{
							plr.add_money(it->get_money());
							fState->get_player(it->get_id()).kill();
						}
					}
					plr.add_money(card->value);
					amt += card->value;
					fState->get_player(it->get_id()).sub_money(card->value);
				}
			}
			return kDone;

		case Game_Base::kAllLose:
			for (it = plr_lst.begin(); it != plr_lst.end(); ++it)
			{
				if (it->get_money() < (unsigned)card->value)
					if (!free(it->get_id(), card->value))
					{
						if (it->get_id() == plr.get_id()) fDoubleCnt = 0;

						fState->get_player(it->get_id()).kill();
					}
				fState->get_player(it->get_id()).sub_money(card->value);
			}
			amt = card->value * -1;
			return kDone;
	}

	// handle the tougher ones: kPerProp
	if (Game_Base::kPerProp == card->type)
	{
		const net::PropertyList_t& prp_lst = plr.get_owned_properties();
		net::PropertyList_t::const_iterator it;

		for (it = prp_lst.begin(); it != prp_lst.end(); ++it)
		{
			unsigned level = fState->get_property(*it).get_level();

			if (5 > level) pay += level * card->value;
			else pay += level * card->value * 3;
		}

		amt = pay * -1;

		if (plr.get_money() < pay)
			if (!free(plr.get_id(), pay)) return kConcede;

		plr.sub_money(pay);
		return kDone;
	}
	
	return kDone;
}

GameRules::EGR_result GameRules::jail(GameRules::EGR_jail act, int& d1, int& d2)
{
	PlayerState& plr = fState->get_player(fState->get_current_turn());
	unsigned card = 0xdeadbeef; // :)

	// TODO do we need something here to increase randomness?
	unsigned die1 = d1 = fRand.roll_d6();
	usleep(500);
	unsigned die2 = d2 =fRand.roll_d6();

	// figure out the move for if we need it
	fRoll = die1 + die2;
	fMoveTo = bind_move(plr.get_position() + fRoll);

	if (kJCard == act)
	{
		if (fState->get_property(254).get_owner() == plr.get_id()) card = 1;
		else if (fState->get_property(255).get_owner() == plr.get_id()) card = 0;

		if (0xdeadbeef != card)
		{
			plr.set_jail_status(kFree);						// release player
			fState->get_property(255 - card).set_unowned();	// release the card from state
			plr.del_property(255 - card);					// delete the card from the player
			return kContinue;								// indicate move(...)
		}
		else act = kJRoll;
	}

	if (kJunior > plr.get_jail_status())
	{
		if (kJPay == act)
		{
			if (plr.get_money() < (unsigned)fBase.price(plr.get_position()))// not enough cash
				act = kJRoll;										// queue a roll
			else
			{
				plr.set_jail_status(kFree);							// release player
				plr.sub_money(fBase.price(plr.get_position()));		// subtract fee
				return kContinue;									// indicate move(...)
			}
		}
		
		if (die1 == die2)
		{
			plr.set_jail_status(kFree);							// release player
			return kContinue;									// indicate move(...)
		}
		else
		{
			plr.set_jail_status(plr.get_jail_status() + 1);		// advance jail status
			return kDone;										// player's turn is over
		}
	}
	
	if (die1 == die2)
	{
		plr.set_jail_status(kFree);								// release player
		return kContinue;										// indicate move(...)
	}
	else
	{
		if (plr.get_money() < (unsigned)fBase.price(plr.get_position()))
			if (!free(plr.get_id(), fBase.price(plr.get_position())))
				return kConcede;								// player can't pay fee

		plr.set_jail_status(kFree);								// release player
		plr.sub_money(fBase.price(plr.get_position()));			// subtract fee
		return kContinue;										// indicate move(...)
	}
}

signed GameRules::casino(int wager, GameRules::EGR_casino type, int& d1, int& d2)
{
	int	flag = wager;
	
	unsigned die1 = d1 = fRand.roll_d6();
	usleep(500);							// to increase randomness
	unsigned die2 = d2 = fRand.roll_d6();

	unsigned roll = die1 + die2;
	PlayerState& plr = fState->get_player(fState->get_current_turn());

	// check the player's cash level and adjust wager accordingly
	if ((unsigned)flag > plr.get_money()) flag = plr.get_money();

	// check for a winner based on the bet type
	switch (type)
	{
		case kCCraps:	// win if 2, 3 or 12
			if (2 == roll || 3 == roll || 12 == roll) flag *= 8;
			break;
		case kCSeven:	// win if 7
			if (7 == roll) flag *= 5; 
			break;
		case kCEleven:	// win if 11
			if (11 == roll) flag *= 16;
			break;
		case kCDuece:	// win if 3
			if (3 == roll) flag *= 16;
			break;
		case kCAorB:	// win if 2 or 12
			if (2 == roll || 12 == roll) flag *= 32;
			break;
	}

	if (flag == wager)	// player lost, subtract flag from money
	{
		plr.sub_money(flag);
		flag *= -1;
	}
	else				// player won, add flag to money
		plr.add_money(flag);

	// return the amount of money won or lost
	return flag;
}

bool GameRules::doubled()
{
	if (fDoubleCnt) return true;
	return false;
}

unsigned GameRules::get_last_card()
{
	return fLastCard;
}

unsigned GameRules::get_purch_prop()
{
	return fPurchaseProp;
}

unsigned GameRules::get_move_to()
{
	return fMoveTo;
}

bool GameRules::confirmed()
{
	PlayerState& plr = fState->get_player(fState->get_current_turn());

	// if the player is on Free Parking move them to the other FP
	// TODO: this is hardcoded to switch between 20 and 52
	if (Game_Base::kFParking == fBase.type(fMoveTo))
	{
		if (20 == fMoveTo) plr.set_position(52);
		else plr.set_position(20);
		return true;
	}

	// if the player has the opportunity to purchase a prop, attempt to buy
	if (fPurchaseProp)
	{	
		// check for insufficient funds
		if (fState->get_player(fState->get_current_turn()).get_money() < 
			(unsigned)(fBase.price(fPurchaseProp)))
			return false;

		// add the property to the player state of the current player
		plr.add_property(fPurchaseProp);

		// adjust the owner of the property in GameState
		fState->get_property(fPurchaseProp).set_owner(plr.get_id());

		// subtract the price from the player's money
		plr.sub_money(fBase.price(fPurchaseProp));

		fPurchaseProp = 0;

		return true;
	}

	// if the player is on a Go To Jail space, move to jail
	if (Game_Base::kGoJail == fBase.type(fMoveTo))
	{
		// reset fDoubleCnt
		fDoubleCnt = 0;

		// move the player to jail
		plr.set_position(10);

		// set player's jail status to kFreshman
		plr.set_jail_status(kFreshman);

		return true;
	}

	return false;
}

bool GameRules::release_assets(const PlayerState& plr)
{
	const net::PropertyList_t& p_list = plr.get_owned_properties();
	net::PropertyList_t::const_iterator it;

	// iterate through the list freeing assets
	for (it = p_list.begin(); it != p_list.end(); ++it)
	{
		// set the property to unowned
		fState->get_property(*it).set_unowned();
		
		// set the property level to zero
		// TODO is this necessary?
		fState->get_property(*it).set_level(0);
	}

	return true;
}

unsigned GameRules::owns_group(unsigned player, vector<unsigned>& groups)
{
	// reference to the specified player
	const PlayerState& cur_plr = fState->get_player(player);

	// temporary list of specified players props
	net::PropertyList_t props = cur_plr.get_owned_properties();

	// iterators into the list of props
	net::PropertyList_t::iterator it1, it2;

	std::vector<unsigned>	vec;	// temporary vector
	bool					flag;	// boolean search flag	
	unsigned				cnt = 0;// hit counter

	// erase the dest vector
	groups.clear();

	// while the list of props is non-empty
	while (!props.empty())
	{
		it1 = props.begin();	// set the key iter to the start
		flag = true;			// set the flag to true
		
		// get the indecies of the group members of the key iter
		unsigned num = fBase.group_members(*it1, vec);

		// look for each group member in the list of props
		for (unsigned i = 1; i < num; ++i)
		{
			it2 = find(props.begin(), props.end(), vec[i]);
			if (it2 == props.end()) flag = false;	// if not found flag = false
			else props.erase(it2);					// otherwise erase the entry
		}

		// if flag is still true
		if (flag)
		{
			// push the key id on the return vec and increment hit count
			groups.push_back(*it1);
			++cnt;
		}

		// erase the key iter from the list
		props.erase(it1);
	}

	return cnt;
}

bool GameRules::owns_group(unsigned pid, unsigned id)
{
	std::vector<unsigned> vec;
	unsigned num = owns_group(pid, vec);

	for (unsigned i = 0; i < num; ++i)
		if (vec[i] == id) return true;

	return false;
}

bool GameRules::free(unsigned pid, unsigned target)
{
	// temporary vector
	vector<unsigned> vec_g, vec_p;
	vector<unsigned>::iterator v_it;

	// reference to the current player
	PlayerState& cur_plr = fState->get_player(pid);

	// const reference to the player's property list
	const net::PropertyList_t& props = cur_plr.get_owned_properties();

	// temporary iterator into the list
	net::PropertyList_t::const_iterator it;

	// the number of groups owned by the player
	unsigned grps = owns_group(cur_plr.get_id(), vec_g);

	// look for upgrades to sell first then start selling props
	if (grps)
	{
		while (cur_plr.get_money() < target)
		{
			if (vec_g.empty()) break;	// no more groups to check

			// set the ref iter to the first id
			v_it = vec_g.begin();

			// check to see if the property has upgrades
			if (fState->get_property(*v_it).get_level())
			{
				// get the property's group members
				unsigned mems = fBase.group_members(*v_it, vec_p);

				// remove a level from the members and add to money
				for (unsigned i = 0; i < mems; ++i)
				{
					// get a ref to the property
					PropertyState& prop = fState->get_property(vec_p[i]);

					// reduce the level
					prop.set_level(prop.get_level() - 1);

					// add half the upgrade price to the player's money
					cur_plr.add_money(fBase.upgrade(vec_p[i], prop.get_level()) / 2);
				}
			}
			else
			{
				// remove the ref prop from the group list
				vec_g.erase(v_it);
			}
		}
	}

	// if no groups or upgrades start selling props
	while (cur_plr.get_money() < target)
	{
		// check to see if the player has any more props to sell
		if (cur_plr.get_owned_properties().empty()) return false;

		// set the ref iter to the first prop
		it = props.begin();
		
		// add the mortgage value for the property to money
		cur_plr.add_money(fBase.price(*it) / 2);

		// change the owner in GameState to unowned
		fState->get_property(*it).set_unowned();

		// delete the prop from the player's list
		cur_plr.del_property(*it);
	}

	return true;
}

unsigned GameRules::transfer(unsigned pos)
{
	switch (pos)
	{
		case 5: return 43;
		case 15: return 49;
		case 25: return 55;
		case 35: return 61;
		case 43: return 5;
		case 49: return 15;
		case 55: return 25;
		case 61: return 35;
		default: return pos;
				 
	}
}

unsigned GameRules::bind_move(unsigned pos)
{
	return pos % fBase.num_spaces();
}

unsigned GameRules::count_mems(const PlayerState& plr, unsigned id)
{
	vector<unsigned> vec;
	unsigned mems = fBase.group_members(id, vec);
	unsigned cnt = 0;
	const net::PropertyList_t props = plr.get_owned_properties();
	net::PropertyList_t::const_iterator it;

	for (unsigned i = 0; i < mems; ++i)
	{
		for (it = props.begin(); it != props.end(); ++it)
			if (vec[i] == *it)
			{
				++cnt;
				break;
			}
	}

	return cnt;
}
