/////////////////////////////////////////////////////////////////////////////
// GameLogicUnit.C
//
// Provides a network interface the server uses to interact with the case
// specific methods for modifying the GameState. The GameLogicUnit maintains
// a ruleset which performs actions based on network events
/////////////////////////////////////////////////////////////////////////////

// includes and defns {{{

#include "GameLogicUnit.h"

#include "Random_Generator.h"
#include "GameRules.h"
#include "PlayerState.h"
#include "PropertyState.h"
#include "network/packet.h"
#include "network/ServerClient.h"
#include "network/GameState.h"
#include "network/packet_decoder.h"
#include "network/property_packet.h"
//#include "network/register_packet.h"
#include "network/dice_packet.h"
#include "network/card_packet.h"
#include "network/casino_packet.h"
#include "network/trade_packet.h"
#include "network/jail_packet.h"

#include "network/action_packet.h"

#include <cassert>
#include <stdint.h>
#include <unistd.h>

#include <cstdarg>

#define DEBUG(x) std::cerr << x << std::endl;
#ifdef DEBUG
#include <iostream>
#else
#define DEBUG(x) 
#endif

const unsigned MAX_PLAYERS 		= 8;
const unsigned INITIAL_MONEY	= 1500;
const unsigned INITIAL_POS 		= 0;

enum EGLU_status {kSNone, kSLimbo, kSPlay};
enum EGLU_pflags {kPFNone, kPFFirst};

// }}}

/*** ADMIN  {{{ ***/

GameLogicUnit::GameLogicUnit(net::GameState* state)
: _rules(NULL), _state(state), _status(kSNone), _cturn(NULL)
{
	_rules = new GameRules(_state);

	// add the default properties
	for(unsigned i = 0; i < _state->get_base().num_spaces(); ++i) {
		_state->add_property( PropertyState(i, PropertyState::NOTOWNED, 0) );
		//DEBUG("Property " << *_state->get_base().name(i) << " was added, owned by " << (i%4) << ".");
	}

	// add the Get Out of Jail Free Cards
	_state->add_property( PropertyState(254, PropertyState::NOTOWNED, 0) );
	_state->add_property( PropertyState(255, PropertyState::NOTOWNED, 0) );


	//DEBUG("Property size=" << _state->get_property_count() << ")");
}

GameLogicUnit::~GameLogicUnit()
{
	delete _rules;
}

// }}}

/*** INTERFACE ***/

unsigned GameLogicUnit::apply_to(ServerClient* client) // {{{
{
	if(NULL == client) return kAInvalArg;

	while(!client->rbuf.empty()) {

		try {
			// this may throw an exception if the data is not finished.
			net::Decoder dec (client->rbuf);

			// calculate the size of the packet so we can erase that amount from
			// the client's rbuf. if we don't erase it, we'll end up processing
			// it for a second time.
			unsigned int packsz = dec.get_packet_length() + net::HEADER_SIZE;
			client->rbuf.erase(0, packsz);

			if( false == _handle_packet(&dec, client) ) {
				return kAFail;
			}
			
		} catch( const net::IncompletePacketException & e ) {
			// this exception tells us that the packet isn't finished yet
			return kANotDone;
		} catch ( const std::exception & e ) {
			DEBUG("Caught exception " << e.what() << " in apply_to" );
		} catch ( ... ) {
			DEBUG("Unknown except caught" );
			abort();
		}

	}

	return kASuccess;
} // }}}

void GameLogicUnit::remove(ServerClient* client) // {{{
{
	if(NULL == client) return;

	// locate the struct
	GLU_Player_List::iterator player = _get_player_by_client(client);
	if(true == _is_bad_iter(player)) return;				// checking return value

	DEBUG("remove(" << player->state->get_name() << ")");

	if(kSPlay == _status) {
		if(player->state->alive()) {
			_broadcast(kAPDead, player->state->get_id(), "conceded");
		}
	}

	// is this the first player?
	_change_first_player(player);

	// now the player *after* this one is the first player
	// and will have been notified if the status is correct

	// is it this player's turn?
	_change_turn(player);

	// rules needs to release info for the player
	// GameRules
	_rules->release_assets( *player->state );

	// remove from gamestate
	_state->del_player(player->state->get_name());

	DEBUG("GameState.player_count=(" << _state->get_player_count() << ")" );

	// remove from player streams
	_players.erase(player);

	// After removing the player

	// If the unit is empty, return to normal
	if(_players.empty()) {
		DEBUG("Initial state.");
		_status = kSNone;
		_cturn = NULL;
		_state->set_current_turn("");
	}

} // }}}

/*** PRIVATE METHODS ***/

GameLogicUnit::GLU_Player_List::iterator GameLogicUnit::_get_player_by_client(ServerClient* client) // {{{
{
	if(NULL == client) return _bad_iter();

	GLU_Player_List::iterator i = this->_players.begin();

	while (!_is_bad_iter(i)) {
		if(i->client == client) {
			return i;
		}
		++i;
	}
	return _bad_iter();
} // }}}

GameLogicUnit::GLU_Player_List::iterator GameLogicUnit::_get_player_by_flag(unsigned flag) // {{{
{
	GLU_Player_List::iterator i = this->_players.begin();

	while (!_is_bad_iter(i)) {
		if(i->has_flag(flag)) {
			return i;
		}
		++i;
	}
	return _bad_iter();
} // }}}

GameLogicUnit::GLU_Player_List::iterator GameLogicUnit::_get_player_by_id(unsigned id) // {{{
{
	GLU_Player_List::iterator i = this->_players.begin();

	while (!_is_bad_iter(i)) {
		if(i->state->get_id() == id) {
			return i;
		}
		++i;
	}
	return _bad_iter();
} // }}}

bool GameLogicUnit::_is_bad_iter(GLU_Player_List::const_iterator iter) const // {{{
{
	return iter == _bad_iter();
}

GameLogicUnit::GLU_Player_List::const_iterator GameLogicUnit::_bad_iter() const
{
	return _players.end();
}

GameLogicUnit::GLU_Player_List::iterator GameLogicUnit::_bad_iter() 
{
	return _players.end();
} // }}}

bool GameLogicUnit::_handle_packet(net::Decoder* decoder, ServerClient* client) // {{{
{
	assert(NULL != decoder);
	assert(NULL != client);

	switch (_status) {
	case kSNone: 	// No players connected
		// In this state, the only valid packet is Register.
		if(!_register_player(decoder, client) ) {
			return false;
		} else {
			_status = kSLimbo;
		}
		break;
	case kSLimbo:	// Players have connected, game hasn't started
		return _limbo_mode(decoder, client);
		break;
	case kSPlay:	// No more players can connect, game is started
		return _game_mode(decoder, client);
		break;
	default:
		DEBUG("Unknown game status.");
		return false;
	};

	return true;
} // }}}

bool GameLogicUnit::_limbo_mode(net::Decoder* decoder, ServerClient* client) // {{{
{
	// in limbo mode, there are two valid types of packets:
	// 1) A_REGISTER,
	// 2) A_START_GAME

	switch(decoder->get_packet_action()) {
	case net::A_ACTION:
		if(!_register_player(decoder, client) ) {
			return false;
		}

		if(_players.size() == MAX_PLAYERS) {
			_start_game();
			_status = kSPlay;
		}

		break;

	case net::A_START_GAME:
		_start_game();
		_status = kSPlay;
		break;

	case net::A_SEND_UPDATE:
		_send_game_state_to(client);
		break;

	default:
		DEBUG("ERR invalid limbo-mode packet(" << decoder->get_packet_action() << ")");
		return false;
	}

	return true;
} // }}}

void GameLogicUnit::_start_game() // {{{
{
	// setup the initial turn stuff
	if(!_next_turn()) {
		DEBUG("_start_game() - this is bad.");
	}

	// Notify everyone that the game has begun!
	net::Packet simple(net::A_START_GAME);
	_respond_to_all(&simple);

	// this is necessary so everyone knows the first turn
	GLU_Player_List::iterator player = _get_player_by_flag(kPFFirst);
	_broadcast(kAPTurnStart, player->state->get_id());

} // }}}

bool GameLogicUnit::_game_mode(net::Decoder* decoder, ServerClient* client) // {{{
{
	bool yes = false;

	switch(decoder->get_packet_action()) {
	case net::A_ROLL_DICE:
		return _handle_roll_dice(client);
		break;


	// confirm applies to FREE PARKING and Buy property
	case net::A_CONFIRM:
		DEBUG("player says yes.");

		// tell rules that the player has confirmed the action
		if(!_rules->confirmed()) {
			// TODO: respond_simple(FAILED_ACTION)
		}

		yes = true;

		// this fall-through is intentional
	
	case net::A_DECLINE:
		if(!yes) DEBUG("player says no :(");

		// TODO: ignore doubles for now
		_next_turn();

		break;

	case net::A_CASINO:
		return _handle_casino(decoder, client);
		break;

	case net::A_SEND_UPDATE:
		_send_game_state_to(client);
		break;

	case net::A_PROPERTY:
		return _handle_improve(decoder, client);
		break;

	case net::A_TRADE:
		return _handle_trade(decoder, client);
		break;

	case net::A_JAIL:
		return _handle_jail(decoder, client);

	default:
		DEBUG("ERR invalid game-mode packet(" << decoder->get_packet_action() << ")");
		return false;
	}

	return true;
} // }}}

bool GameLogicUnit::_handle_casino(net::Decoder* decoder, ServerClient* client) // {{{
{
	net::Casino k(decoder);
	DEBUG("Casino(type=" << (int)k.get_bet_type() << ",wager=" << k.get_wager() << ")");

	int d1, d2 = 0;

	/*signed result = */_rules->casino((int)k.get_wager(), (GameRules::EGR_casino)k.get_bet_type(), d1, d2);

	_send_dice_to(net::kDGood, d1, d2, client);

	// TODO: tell client how much he won/lost

	// TODO: doubles
	_next_turn();

	return true;
} // }}}

bool GameLogicUnit::_handle_improve(net::Decoder* decoder, ServerClient* client) // {{{
{
	net::Property k(decoder);
	DEBUG("Property(number=" << (int)k.get_number() << ",level=" << (int)k.get_level() << ")");

	try {

	if(_rules->improve((int)k.get_number(), (int)k.get_level())) {
		// TODO: tell player
		DEBUG(" Improve returned true");
	} else {
		// TODO: tell player
		DEBUG(" Improve returned FALSE DIE DIE. :/");
	}

	} catch ( ... ) {
		DEBUG("Unhandled exception in improve.");
	}

	return true;
} // }}}

bool GameLogicUnit::_handle_trade(net::Decoder* decoder, ServerClient* client) // {{{
{
	net::Trade k(decoder);

	DEBUG("Trade(src=" << (int)k.get_source_player() << ",dest=" << (int)k.get_destin_player()
		<< ",offmoney=" << (int)k.get_offered_money() << ",reqmoney=" << (int)k.get_request_money() << ")" );

	GLU_Player_List::const_iterator src = _get_player_by_id( k.get_source_player() );
	GLU_Player_List::const_iterator dst = _get_player_by_id( k.get_destin_player() );
	GLU_Player_List::const_iterator sender = _get_player_by_client( client );

	// if any of the iterators are invalid, the transaction shouldn't happen.
	if(_is_bad_iter(src) || _is_bad_iter(dst) ||  _is_bad_iter(sender) ) {
		DEBUG("Bad iterators.");
		return true;
	}

	if(k.get_trade_state() == net::TS_INIT) {

		DEBUG("Trade init.");

		// sanity check - the client that sent us this data should
		// have the same 'id' as the 'source' field in the packet
		// if this is false, then the client is playing tricks on us
		// and should be taught a lesson
		if( sender->state->get_id() != k.get_source_player() ) { 
			DEBUG("sanity check 1 in handle_trade failed.");
			DEBUG("(sender_id=" << (int)sender->state->get_id() << ",source=" << (int)k.get_source_player() << ",dest=" << (int)k.get_destin_player() << ")");
			_dump_game_state();
			return false;
		}

		_respond(decoder->get_packet_data(), dst->client);

	} else if ( k.get_trade_state() == net::TS_ACCEPT ) {

		DEBUG("Trade accept.");

		if( sender->state->get_id() != k.get_destin_player() ) { 
			DEBUG("sanity check 2 in handle_trade failed.");
			DEBUG("(sender_id=" << (int)sender->state->get_id() << ",source=" << (int)k.get_source_player() << ",dest=" << (int)k.get_destin_player() << ")");
			_dump_game_state();
			return false;
		}

		// destination accepted trade, apply result and tell source
		// that it was good.

		_rules->trade(&k);

		// tell the source that everything was traded
		_respond(decoder->get_packet_data(), src->client);


	} else if ( k.get_trade_state() == net::TS_DECLINE ) {
		DEBUG("Trade decline.");

		if( sender->state->get_id() != k.get_destin_player() ) { 
			DEBUG("sanity check 3 in handle_trade failed.");
			DEBUG("(sender_id=" << (int)sender->state->get_id() << ",source=" << (int)k.get_source_player() << ",dest=" << (int)k.get_destin_player() << ")");
			_dump_game_state();
			return false;
		}

		// tell the source that nothing was traded.
		_respond(decoder->get_packet_data(), src->client);

	} else {
		DEBUG("Trade other???.");
		DEBUG("(sender_id=" << sender->state->get_id() << ",source=" << k.get_source_player() << ",dest=" << (int)k.get_destin_player() << ")");
		_dump_game_state();
		return false;
	}


	return true;
} // }}}

void GameLogicUnit::_dump_game_state() const // {{{
{
	DEBUG("+SERVER DUMP ==========");
	GLU_Player_List::const_iterator it;
	for(it = _players.begin(); it != _players.end(); ++it) {
		DEBUG("Player(client="<<it->client << ",state=" << it->state << ",flags=" << it->flags << ")");
		if(it->state) {
			DEBUG("PlayerState(id="<<(int)it->state->get_id()<<",name="<<it->state->get_name()<<",pos="<<(int)it->state->get_position() << ")");
		}
	}
	DEBUG("-SERVER DUMP ==========");
} // }}}

bool GameLogicUnit::_handle_jail(net::Decoder* decoder, ServerClient* client) // {{{
{
	net::Jail k(decoder);
	DEBUG("Jail(option=" << (int)k.get_jail_state() << ")");

	int d1 = 0, d2 = 0;

	GameRules::EGR_result jres = _rules->jail( (GameRules::EGR_jail) k.get_jail_state(), d1, d2 );

	GLU_Player_List::iterator player = _get_player_by_client(client);

	switch(jres) {
	case GameRules::kDone: // player tried to roll and failed
		DEBUG(" Player has dropped the soap (in jail)");

		// notify everyone that the player tried to roll, and failed
		_broadcast(kAPJail, player->state->get_id(), kJARoll, d1, d2);

		_next_turn();

		break;
	case GameRules::kConcede:
		DEBUG(" Player has died in Jail.");

		// for whatever reason, the player has no money now
		_broadcast(kAPDead, player->state->get_id(), "died in jail");

		_next_turn();

		break;
	case GameRules::kContinue:
	
		// the player's action was successful, and will be moved	
		_broadcast(kAPJail, player->state->get_id(), k.get_jail_state(), d1, d2);

		return _handle_move(player);
		break;

	default:
		DEBUG("Unknown jail return response.");
		break;
	}
	
	return true;
} // }}}

bool GameLogicUnit::_handle_move(GLU_Player_List::iterator player) // {{{
{
	unsigned card = 0;
	GameRules::EGR_result card_res;

	bool go_to_next_turn = false;
	bool move = false;

	unsigned player_id = player->state->get_id();

	int paid = 0;

	do {
		move = false;

		// move the player
		GameRules::EGR_move move_res = _rules->move();

		// update their game window screen
		_send_game_state_to(player->client);

		// now apply any 'post-move' effects
		switch(move_res) {
			case GameRules::kNone: // no additional actions
				// TODO: handle DOUBLES
				go_to_next_turn = true; /* = ! _rules->doubled() */ 

				break;

			case GameRules::kBuy:  // property is buyable, call confirm() to buy
				DEBUG("	Player may purchase this property");

				_broadcast(kAPBuyProp, player_id, _rules->get_purch_prop(), 0);

				//_respond_simple(net::A_BUY_PROP, player->client);

				break;

			case GameRules::kPay:  // player owes rent/tax, call pay()
				DEBUG("	Player must make a payment");

				switch(_rules->pay(paid)) {
					case GameRules::kDone: // no further action needed
						DEBUG("	 Player has made a payment");

						if(paid > 0) {
							_broadcast(kAPPaidDebt, player_id, _rules->get_move_to() , paid );
						}

						// TODO: notify player of rent charge
						// TODO: handle DOUBLES
						go_to_next_turn = true; 

						break;
					case GameRules::kConcede: // dead player
						DEBUG("	 Player does not have sufficient funds to pay his debts");

						_broadcast(kAPDead, player_id, "insufficient funds");
						player->state->kill();
						//_respond_simple(net::A_YOU_DIED, client);

						go_to_next_turn = true; 

						break;
					default:
						DEBUG("  Unexpected pay return value.");
						break;

				}
				break;

			case GameRules::kCrd:  // player must draw a card, call card()
				DEBUG("	Player must take a card");

				card_res = _rules->card(card, paid);

				DEBUG("	 Player has received card " << card <<" and is done.");

				_send_card_to(card, player->client);

				DEBUG("	 Sent card to player");


				switch(card_res) {
					case GameRules::kContinue: // need to move again
						DEBUG("   The card drawn requires the player to move again.");
						move = true;
						continue;
						break;

					case GameRules::kDone: // no further action needed
						DEBUG("	  The card has been applied, and the player has survived... for now.");

						_scan_for_dead_players();

						// TODO: scan the player list for dead players, and send them the death letter
						// TODO: handle DOUBLES
						go_to_next_turn = true; 

						break;

					case GameRules::kConcede: // dead player
						DEBUG("	  The card has been applied, and the player has died!");
						// TODO: tell the player why he died
						_respond_simple(net::A_YOU_DIED, player->client);

						go_to_next_turn = true; 

						break;

					default:
						DEBUG("  Unexpected card return value.");
						break;
				}
				break;

			case GameRules::kFP:   // player is on Free Parking, call confirm() to transfer
				DEBUG("	Player landed on free parking");
				_respond_simple(net::A_FREE_PARKING, player->client);

				break;

			case GameRules::kGTJ:  // player is on Go To Jail, call confirm()
				DEBUG("Go to jail, do not pass go, do not collect your salary");

				// TODO: notify player of jail occupation
				// TODO: why call confirmed() ?

				// confirmed appears to just move the player to this spot
				// i think i can just call it here.
				_rules->confirmed();

				go_to_next_turn = true;

				break;

			case GameRules::kCas:  // player is on casino, call casino()
				DEBUG("	Player is on the casino");
				_respond_simple(net::A_CASINO, player->client);
				break;

			default:
				DEBUG("Unexpected move return value.");
				break;
		}

	} while( move );

	if(go_to_next_turn) {
		_next_turn();
	}

	return true;
} // }}}

void GameLogicUnit::_scan_for_dead_players() // {{{
{
	GLU_Player_List::iterator it;
	for(it = _players.begin(); it != _players.end(); ++it ) {
		if(!it->state->alive()) {
			_respond_simple(net::A_YOU_DIED, it->client);
		}
	}
} // }}}

bool GameLogicUnit::_register_player(net::Decoder* decoder, ServerClient* client) // {{{
{
	assert(NULL != decoder);
	assert(NULL != client);

	// has client been added already?
	if(_get_player_by_client(client) != _bad_iter()) {
		DEBUG("ERR Client has already been added.");
		return false;
	}

	// only process an action packet
	if(decoder->get_packet_action() != net::A_ACTION) {
		DEBUG("ERR non-action packet(" << decoder->get_packet_action() << ")");
		return false;
	}

	std::string name;

	try {
		net::ActionPacket act(decoder);

		DEBUG("Received action packet = " << act.get_action() << "." );

		if(act.get_state() != kAPRegister) {
			DEBUG("ERR non-register packet");
			return false;
		}

		if(!act.get_sparam(name, 0)) {
			DEBUG("Invalid register name");
			return false;
		}
	} catch (...) {
		DEBUG("ERR Decoding reg action packet threw up.");
		return false;
	}

	bool first_player = _players.empty();
	int action = net::A_NONE;
	GLU_Player* player = NULL;

	// add the player to the state
	if(NULL == (player = _add_player( name, client ) ) ) {
		// failure
		DEBUG("ERR Could not register with name " << name );

		action = net::A_REG_FAIL;

	} else {
		// success

		if(first_player) {

			DEBUG("first player(" << name << ")");

			// set the first flag
			player->set_flag(kPFFirst);

			// tell them that they are first
			action = net::A_REG_FIRST;

		} else {
			DEBUG("Not first player");
			action = net::A_REG_GOOD;
		}
	}

	// construct simple response packet
	net::Packet simple(action);
	_respond(&simple, client);

	// success
	return true;
} // }}}

bool GameLogicUnit::_handle_roll_dice(ServerClient* client) // {{{
{
	assert(NULL != client);

	DEBUG("Handling the roll.");

	GLU_Player_List::iterator player = _get_player_by_client(client);

	// sanity check
	if(_is_bad_iter(player)) {
		DEBUG("ERR bad player found.");
		return false;
	}

	// Only process rolls from the current turn
	if(_cturn != client) return true;

	int d1 = 0, d2 = 0;

	DEBUG("Rollin' the dice");

	switch(_rules->roll(d1, d2)) {
		case GameRules::kContinue:
			DEBUG("Roll was good.");
			break;

		case GameRules::kToJail:
			DEBUG("Doubles chain, go to jail!");

			_broadcast(kAPJail, player->state->get_id(), kJADoubles, d1, d2);

			_next_turn();
			return true;
			break;

		default:
			DEBUG(" Unexpected roll return value.");
			return true;
			break;
	}

	

	// notify the player the result of the dice roll.
	//_send_dice_to(net::kDGood, d1, d2, client);

	_broadcast(kAPMoved, player->state->get_id(), _rules->get_move_to(), d1, d2);

	// the player's dice have been rolled, time to move
	return _handle_move(player);

} // }}}

void GameLogicUnit::_respond_simple(unsigned act, ServerClient* client) const // {{{
{
	net::Packet simple(act);
	_respond(&simple, client);
}

void GameLogicUnit::_respond(const std::string & data, ServerClient* client) const
{
	assert(NULL != client);
	client->wbuf += data;
}

void GameLogicUnit::_respond_to_all(const std::string & data) const
{
	GLU_Player_List::const_iterator i = _players.begin();
	while(!_is_bad_iter(i)) {
		ServerClient* client = (i++)->client;
		_respond(data, client);
	}
} // }}}

void GameLogicUnit::_send_game_state_to(ServerClient* client) const // {{{
{
	assert(NULL != client);
	std::string data = _state->encode();
	_respond(data, client);
} // }}}

void GameLogicUnit::_send_card_to(unsigned num, ServerClient* client) const // {{{
{
	net::Card card(num);
	std::string data = card.encode();
	_respond(data, client);
} // }}}

void GameLogicUnit::_send_dice_to(unsigned act, unsigned d1, unsigned d2, ServerClient* client) const // {{{
{
	net::Dice dice(act, d1, d2);
	std::string data = dice.encode();
	_respond(data, client);

} // }}}

GameLogicUnit::GLU_Player* GameLogicUnit::_add_player(const std::string & name, ServerClient* client) // {{{
{
	assert( name.empty() == false );
	assert( client != NULL );

	net::Player_t id = _state->find_unused_id( static_cast<net::Player_t>(-1) );
	PlayerState player( id, name, INITIAL_MONEY, INITIAL_POS );

	DEBUG("adding player(" << (int)id << "," << name << ")");

	if(!_state->add_player( player ) ) {
		DEBUG("ERR player exists.");
		return NULL;
	}

	GLU_Player p;
	p.client = client;
	p.state = _state->get_newest_player();
	p.flags = 0;


	_players.push_back( p );

	return &_players.back();
} // }}}

void GameLogicUnit::_change_first_player(GLU_Player_List::iterator player) // {{{
{
	// changing the first player is only relevent in the limbo mode
	if(kSLimbo != _status) {
		return;
	}

	// bad iterators are bad for your health
	if(_is_bad_iter(player)) {
		return;
	}

	// this player isn't even the first player
	if(!player->has_flag(kPFFirst)) {
		return;
	}

	// well, they were, now they are not
	player->clear_flag(kPFFirst);
	
	// forward to next player
	++player;

	// so this was the only player
	if(_is_bad_iter(player)) {
		return;
	}

	// guess there are more players
	player->set_flag(kPFFirst);

	// notify the new first player about his promotion.
	net::Packet simple(net::A_REG_FIRST);
	_respond(&simple, player->client);


} // }}}

void GameLogicUnit::_change_turn(GLU_Player_List::iterator player) // {{{
{
	if(_is_bad_iter(player)) {
		return;
	}

	// change turn will only work if 'player' is actually the current turn
	if(player->client == _cturn) {
		if(_players.size() > 1) {
			_next_turn(); // next_turn does all the work
		} else {
			_cturn = NULL;
		}
	}

} // }}}

bool GameLogicUnit::_next_turn() // {{{
{
	if(_players.empty()) {
		DEBUG("Why are you empty now?");
		return false;
	}

	/* TODO
	if(_rules->doubled()) {
		// send doubles notification to client?
		return;
	}
	*/

	GLU_Player_List::iterator start = _players.begin();

	// players can exist, but have been killed off.
	// we do not want to include "dead to me" players in the turn sequence
	while(!start->state->alive()) {
		++start;

		// make sure we don't go beyond the list,
		// and if we do, this just means that all of
		// the players are dead
		if(_is_bad_iter(start)) {
			DEBUG("All players are dead now?");
			return false;
		}
	}

	// start represents the first "non-dead" player.

	if(_cturn == NULL) {
		DEBUG("Null _cturn, sending start iterator.");
		// turn has not been set yet
		// the 'first player' in the list becomes the next turn
		_set_turn(start);

	} else {

		GLU_Player_List::iterator player = _get_player_by_client(_cturn);
		if(_is_bad_iter(player)) {
			DEBUG("iterator for player is bad... setting to start");

			// for some reason, the current turn is bad.
			// this is a panic case, so we just give priority to the first non-dead player
			_set_turn(start);

		} else {

			// loop to find the first non-dead next player
			do {

				// try to move to the next player
				++player;

				if(_is_bad_iter(player)) {
					// this means the player was at the end, we need to cycle back now
					player = start;
					break; // we assume start will be alive, since we verified this in the first loop
				}

				// player isn't at the end, but it remains to be seen whether
				// this player is alive

			// make sure the selected player is alive
			} while(!player->state->alive());

			// in any event, the result of the loop executes here
			_set_turn(player);
		}

	}
	return true;
} // }}}

bool GameLogicUnit::_set_turn(ServerClient* client) // {{{
{
	assert(NULL != client);

	GLU_Player_List::iterator player = _get_player_by_client(client);

	return _set_turn(player);
} // }}}

bool GameLogicUnit::_set_turn(GLU_Player_List::iterator player) // {{{
{
	if(_is_bad_iter(player)) {
		DEBUG("Player is bad... naughty player ;)>-<");
		return false;
	}

	DEBUG("Setting turn to " << player->state->get_name() << ".");

	_state->set_current_turn(player->state->get_name());
	_cturn = player->client;

	_broadcast(kAPTurnStart, player->state->get_id());

	switch( _rules->turn_start() ) {
		case GameRules::kContinue:
			DEBUG(" Player is OKAY - turn may commence");
			break;

		case GameRules::kInJail: // player is in jail 
			DEBUG(" Player is in Jail.");

			// tell everyone that the current player is jailed
			// this also tells the current player that he needs to decide 
			// whether to pay up, roll, or use a card (if he has it)
			_broadcast(kAPJail, player->state->get_id(), kJAJustThere, 0, 0);

			break;

		case GameRules::kConcede:
			DEBUG(" Player is dead... :(");

			// TODO: SEND DEATH LETTER
			_broadcast(kAPDead, player->state->get_id(), "no assets");

			player->state->kill();

			//_respond_simple(net::A_YOU_DIED, player->client);

			// since this player is dead, he/she cannot possibly
			// have a turn, therefore, we move to the next player
			++player;

			return _set_turn(player);

			break;
		default:
			DEBUG("Unhandled turn start return value.");
			break;
	}

	return true;
} // }}}

void GameLogicUnit::_respond(net::Packet * p, ServerClient* c) const
{
	std::string data = p->encode();
	_respond(data, c);
}

void GameLogicUnit::_respond_to_all(net::Packet * p) const
{
	std::string data = p->encode();
	_respond_to_all(data);
}

void GameLogicUnit::_broadcast(unsigned action, unsigned playerID, ...) const // {{{
{
	va_list ap;
	int d;
	char *s = NULL;

	assert(action < ap_fmt_count);

	net::ActionPacket act(action, playerID);

	const char * fmt = ap_fmt_list[action];

	if(fmt != NULL) {
		va_start(ap, playerID);
		while (*fmt) {
			switch(*fmt++) {
			case 's': // string
				s = va_arg(ap, char*);
				act.add_sparam(s);
				break;
			case 'd': // integer
				d = va_arg(ap, int);
				act.add_iparam(d);
				break;
			default: // ignore anything else
				break;
			}
		}
		va_end(ap);
	}

	_respond_to_all(&act);
} // }}}

// vim600: noet sw=4 ts=4 fdm=marker
