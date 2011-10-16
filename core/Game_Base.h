/////////////////////////////////////////////////////////////////////////////
// Game_Base.h
//
// Contains the class defininition for the Game_Base class which provides
// static space and card data to all concerned parties
/////////////////////////////////////////////////////////////////////////////

#ifndef GAME_BASE_H
#define GAME_BASE_H

// includes required by Game_Base
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Random_Generator.h"
using namespace std;	// so there aren't tons of std's in the code

class Game_Base
{
public:
	/*** ENUMERATION TYPES ***/
	enum EGB_Space_type {kGo, kJail, kFParking, kGoJail, kCasino, kITax, kLTax,
						 kChance, kCChest, kUtil, kRRoad, kProp};

	enum EGB_Card_type {kGet, kLose, kMove, kMoveRel, kOthersPaid, kOthersPay,
						kOthersGet, kOthersLose, kAllGet, kAllLose, kOutJail,
						kPerProp, kInJail};

	/*** PUBLIC DATA STRUCTURES ***/
	struct GB_Card
	{
		unsigned type;		// card type
		bool	 deck;		// card deck (0 = chance, 1 = CChest)
		signed   value;		// card value (type specific meaning)
		string   message;	// card text message
	};

	/*** LIFECYCLE ***/
		Game_Base();
		// Default contructor - propagates basic instatiation to class members

		Game_Base(const char *);
		/* Parametered Constructor - populates game base
		 *
		 * ARG1: filename of structured data file to use for base population
		 * POST: base populated with file data if file exists, left in empty
		 *		 state if file is invalid.
		 */

		~Game_Base();
		// default destructor - deletes base data and frees associated memory
	
	/*** INITIALIZATION ***/
		bool load(const char *);
		/* populates game base with data from a structured data file
		 *
		 * ARG1: filename of structured data file to use for base population
		 * POST: current contents of Game_Base are deleted and associated mem
		 *		 is freed then base is repopulated from the specified file. If
		 *		 file is invalid, base is left unchanged.
		 * RETN: true if file is valid (base changed), false otherwise
		 */
		
		void unload();
		// deletes contents of current base and frees associated memory
	
	/*** PROPERTY DATA INTERFACE ***/
		unsigned group(unsigned) const;
		/* returns the group identification of the indicated property
		 *
		 * ARG1: property index
		 * RETN: 0 if property is not part of a group, otherwise returns the
		 *		 group ID for the property.
		 */

		unsigned group_members(unsigned, vector<unsigned>&) const;
		/* returns the indecies of the specified property's group members
		 * including that of the specified property itself.
		 *
		 * ARG1: property index
		 * ARG2: vector to hold the group member indecies [by reference return]
		 * RETN: number of group members found, 0 if the property is singular
		 *		 or otherwise not part of a group
		 * WARN: the contents of the vector referenced by ARG2 will be deleted
		 */

		unsigned rep_member(unsigned) const;
		/* returns the index of a representative member of a group from a
		 * group identifier
		 *
		 * ARG1: group id
		 * RETN: id of the representative member
		 */

		const string* name(unsigned) const;
		/* returns a pointer to the immutable name string of the indicated space
		 *
		 * ARG1: property index
		 * RETN: write protected pointer to the name string of the space. Null
		 *		 if invalid index is passed.
		 */

		signed price(unsigned) const;
		/* returns the purchase price of the indicated space
		 *
		 * ARG1: property index
		 * RETN: the purchase price of the space or some other type-specific
		 *		 value information.
		 * WARN: Game_Base is not responsible for interpreting the meaning of
		 *		 the price value, that is up to the calling object.
		 */

		unsigned rent(unsigned, unsigned) const;
		/* returns the rent of the specified level for the indicated property
		 *
		 * ARG1: property index
		 * ARG2: desired rent level to retrieve
		 * RETN: 0 if specified index is not of type kProp or if ARG2 > the max
		 *		 rent level for the specified property, otherwise returns rent
		 * WARN: ARG2 must be in [0,6] for type kProp and equal to [0,7] for
		 *		 type kRRoad, any other values will result in a RETN of zero.
		 */

		unsigned type(unsigned) const;
		/* returns the type of the indicated property
		 *
		 * ARG1: property index
		 * RETN: property type
		 */

		unsigned upgrade(unsigned, unsigned) const;
		/* returns the upgrade cost of the indicated property at the specifed
		 * level
		 *
		 * ARG1: property index
		 * ARG2: current upgrade level
		 * RETN: price to upgrade to next level or zero if no upgrade possible
		 */

		unsigned num_spaces() const;
		// returns the number of spaces in the spaces array

	/*** CARD INTERFACE ***/
		const GB_Card* draw_card(unsigned, unsigned* = NULL) const;
		/* returns an immutable pointer to a card data structure in specified
		 * deck
		 *
		 * ARG1: deck index (0 = chance, 1 = community chest)
		 * ARG2: address of an unsigned to receive # of the card drawn. Passing
		 *		 NULL indicates you have no interest in the number
		 * RETN: immutable pointer to a randomly chosen card struct, NULL if
		 *		 improper deck identifier is passed
		 * WARN: Game_Base does not enforce any rules, it is the task of the
		 *		 calling object to enforce specific rules (e.g. only one "Get
		 *		 Out of Jail Free" card per deck in play at a time)
		 */

		const GB_Card* get_card(unsigned) const;
		/* returns an immutable pointer to the specified card
		 * 
		 * ARG1: number of the card to get (spans decks)
		 * RETN: immutable pointer to the desired card, NULL if arg1 invalid
		 */

private:
	struct GB_Space
	{
		unsigned  type;		// space type
		unsigned  group;	// space group id
		signed    price;	// purchase price or type-specific value
		unsigned* rent;		// property rent array for levels up to 5
		string    name;		// space name character string
	};

	vector<GB_Space> fSpaces;	// spaces data array
	vector<GB_Card>	 fCards;	// card data array
	Random_Generator fRand;		// random generator class for card drawing
	unsigned		 fCSplit;	// index of the first CChest card

	/*** PRIVATE METHODS ***/
		bool pRead_Space(ifstream&, Game_Base&);	// space data reader
		bool pRead_ChCard(ifstream&, Game_Base&);	// chance card data reader
		bool pRead_CCCard(ifstream&, Game_Base&);	// CChest card data reader
};

#endif

