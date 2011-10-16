/////////////////////////////////////////////////////////////////////////////
// GameRules.h
//
// Contains the class defininition for the GameRules class which provides
// case-specific methods for modifying the current GameState based on the
// Yloponom ruleset
/////////////////////////////////////////////////////////////////////////////

#ifndef GAME_RULES_H
#define GAME_RULES_H

// includes required by Game_Base
#include <list>
#include "network/GameState.h"
#include "Game_Base.h"
#include "Random_Generator.h"
#include "network/trade_packet.h"

class GameRules
{
public:
	/*** ENUMERATION TYPES ***/
	enum EGR_jail	{kJRoll, kJPay, kJCard};
	enum EGR_casino	{kCCraps, kCSeven, kCEleven, kCDuece, kCAorB};
	enum EGR_roll	{kGood, kLose, kUnowned, kCard, kCardDeath, kFParking, kCasino,
					 kUnownedCard};
	enum EGR_result {kContinue, kDone, kInJail, kToJail, kConcede};
	enum EGR_move	{kNone, kBuy, kPay, kCrd, kFP, kGTJ, kCas};
	
	/*** LIFECYCLE ***/
		GameRules(net::GameState*);
		// Default contructor - creates the rule class and sets the state pointer

		~GameRules();
		// default destructor - deletes any internal objects and frees memory
	
	/*** INTERFACE ***/
		GameRules::EGR_result turn_start();
		/* Initializes turn specific data and handles zero assets condition
		 *
		 * RETN: will be either kContinue, kLose or kInJail
		 */

		bool improve(int, int);
		/* Takes a representative property id from the group to upgrade and the
		 * level to upgrade the group to and applies the secified upgrade if
		 * possible. Also handles downgrading (selling) despite the name
		 *
		 * ARG1: representative property id
		 * ARG2: desired final upgrade level
		 * RETN: true - upgrade applied successfully
		 *		 false - upgrade not applied
		 */

		int trade(net::Trade*);
		/* Accepts a trade packet and applies the contents to the game state
		 *
		 * ARG1: pointer to a trade packet to parse
		 * RETN: currently undefined
		 */

		GameRules::EGR_result roll(int&, int&);
		/* Rolls the dice, checking for doubles and go to jail conditions
		 *
		 * ARG1: by reference return of the 1st die value
		 * ARG2: by reference return of the 2nd die value
		 * RETN: will be either kContinue or kToJail
		 * NOTE: receiving kToJail implies that the player's turn is over
		 */

		GameRules::EGR_move move();
		/* Move the current player to the index specified in the internal data,
		 * checking for passing any of the GO spaces. The return value is used
		 * to coordinate any addtional actions required for the move
		 *
		 * RETN: kNone - no additional actions
		 *		 kBuy - property is buyable, call confirm() to buy
		 *		 kPay - player owes rent/tax, call pay()
		 *		 kCrd - player must draw a card, call card()
		 *		 kFP - player is on Free Parking, call confirm() to transfer
		 *		 kGTJ - player is on Go To Jail, call confirm()
		 *		 kCas - player is on casino, call casino()
		 */

		GameRules::EGR_result pay(int&);
		/* Assesses rent or tax against the current player
		 *
		 * ARG1: by reference return of raw amount to be paid by the player
		 * RETN: either kDone or kConcede
		 */

		GameRules::EGR_result card(unsigned&, int&);
		/* Draws and processes a card, return value coordinates followup
		 * actions
		 *
		 * ARG1: by reference return of the card index
		 * ARG2: by reference return of the amount won (+) or lost (-)
		 * RETN: kContinue - need to move and proc again
		 *		 kDone - card applied, no further action necessary
		 *		 kConcede - current player forced to concede
		 */

		GameRules::EGR_result jail(GameRules::EGR_jail, int&, int&);
		/* handles the condition of a player in jail. Return coordinates followup
		 * actions
		 *
		 * ARG1: enum specifying whether the player intends to roll, pay or use a card
		 * ARG2: by reference return of the value of the first die
		 * ARG3: by reference return of the value of the second die
		 * RETN: kContinue - need to move and process
		 *		 kDone - player's turn is over
		 *		 kConcede - player has been forced to concede
		 * NOTE: the player's 3rd turn in jail implicitly starts with an attempted
		 *		 roll and the player is only forced to pay if the roll is unsuccessful
		 * NOTE: if ARG1 = kPay and the player has insufficient funds, a roll will
		 *		 automatically be entered on his behalf
		 */

		signed casino(int, GameRules::EGR_casino, int&, int&);
		/* Takes a wager amount and type, rolls the dice and applies the result
		 *
		 * ARG1: the wager amount
		 * ARG2: the wager type
		 * ARG3: by reference return of the first die value
		 * ARG4: by reference return of the second die value
		 * RETN: the amount of money won or lost by the player
		 * NOTE: It is not possible to lose on the casino, if the player does
		 *	     not have the funds necessary to meet the minimum wager, all of
		 *	     their available cash will go to the wager
		 */

		bool doubled();
		// Returns true if the player's last roll was a chaining double
		
		unsigned get_last_card();
		// Returns the index of the last card drawn
		
		unsigned get_purch_prop();
		// returns the index of the property availible for the player to purchase
		// only applicable after a call to roll_dice() returns kUnowned or kCardUnowned

		unsigned get_move_to();
		// returns the property index the player has been moved to

		bool confirmed();
		/* Applies context-specific confirmation to the game state. Currently
		 * for FParking, prop purchase and Go To Jail spaces
		 *
		 * RETN: true if successful, false otherwise
		 */

		bool release_assets(const PlayerState&);
		/* Releases the assets of the player from the gamestate
		 *
		 * ARG1: PlayerState to query for assets
		 * RETN: currently undefined
		 */

private:
	net::GameState*		fState;			// pointer to the server's GameState
	const Game_Base&	fBase;			// game base for use by GameRules
	Random_Generator	fRand;			// rand. gen. for use by GameRules
	unsigned			fLastCard;		// index of the last card drawn
	unsigned			fDoubleCnt;		// number of consecutive doubles
	unsigned			fPurchaseProp;	// index of prop purchasible by player
	unsigned			fMoveTo;		// index player is scheduled to move to
	unsigned			fRoll;			// the value of the player's last roll
	
	/*** PRIVATE METHODS ***/
		unsigned owns_group(unsigned, vector<unsigned>&);
		/* finds the groups owned by the specified player and returns a vector
		 * of representative ids from each group by reference
		 *
		 * ARG1: player id
		 * ARG2: vec reference for representative id return
		 * RETN: number of groups owned (elements in vec)
		 */

		bool owns_group(unsigned, unsigned);
		/* returns true if the player indicated in ARG1 owns the entire group
		 * represented by the group id in ARG2 */

		bool free(unsigned, unsigned);
		/* attempts to sell player assets to meet the specified monetary target
		 *
		 * ARG1: player id
		 * ARG2: target amount
		 * RETN: true if target met, false otherwise
		 */

		unsigned transfer(unsigned);
		/* calculates the destination of a transport transfer
		 * 
		 * ARG1: current location
		 */

		unsigned bind_move(unsigned);
		/* takes an unbound board index and binds it within the valid play field
		 *
		 * ARG1: unbounded index
		 * RETN: bound index
		 */

public:

		unsigned count_mems(const PlayerState&, unsigned);
		/* counts the number of members of a particular group owned by a player
		 *
		 * ARG1: const ref to the owning player
		 * ARG2: representative member of group
		 * RETN: number members of group owned by player
		 */
};

#endif
