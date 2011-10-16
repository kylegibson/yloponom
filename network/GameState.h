/////////////////////////////////////////////////////////////////////////////
// GameState.h
//
// Contains the dynamic board and player states, manages property
// ownership and player finances.
/////////////////////////////////////////////////////////////////////////////

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "packet.h"
#include "types.h"

#include "core/PlayerState.h"
#include "core/PropertyState.h"

#include <list>

class Game_Base;

namespace net {



class Decoder;
class GameState : public Packet
{
public: 
	typedef std::list < PlayerState > PlayerStateList;
	typedef std::list < PropertyState > PropertyStateList;


private:

	Name_t _current_turn;
	PlayerStateList _players;
	PropertyStateList _props;

	Game_Base * _base;
	bool _clean_base;

public:
	GameState();
	// Default constructor that constructs an empty state

	GameState(Game_Base *);
	// Default construct from a game base

	// Constructs by decoding data from the network
	GameState(Decoder *, Game_Base * base = NULL);
	/** 
	 * @brief	Constructs from encoded network data 
	 * @param1	Decoder object encapsulating encoded data
	 * @see		Packet
	 */

	GameState(const GameState & rhs);
	// Constructs from another object

	virtual ~GameState();
	// Clean up

	GameState & operator=(const GameState &);

	const Name_t& get_current_turn() const;
	// Accessor to the name of the player who has the current turn

	//bool player_exists(const Name_t& name) const;
	//bool player_exists(const Player_t& id) const;
	/** 
	 * @brief	Determines if player exists
	 * @param1	name of the player
	 * @return	'true' if player is in the GameState
	 */

	// These functions return NULL if player does not exist
	// otherwise they return a pointer to a player state
	PlayerState* get_player_ptr(const Player_t& id);
	PlayerState* get_player_ptr(const Name_t& name);
	const PlayerState* get_player_ptr(const Player_t& id) const;
	const PlayerState* get_player_ptr(const Name_t& name) const;

	// the current turn player
	PlayerState* current_player();
	const PlayerState* current_player() const;


	PlayerState * get_newest_player();


/** DEPRECATED **/
	// Access mutable player object who has 'name'
	PlayerState & get_player(const Name_t& name);
/** DEPRECATED **/
	// Access immutable player object who has 'name'
	const PlayerState & get_player(const Name_t& name) const;
/** DEPRECATED **/
	// Access mutable player object who has 'id'
	PlayerState & get_player(const Player_t& id);
/** DEPRECATED **/
	// Access immutable player object who has 'id'
	const PlayerState & get_player(const Player_t& id) const;

	// Access the number of players in the state
	size_t get_player_count() const;

	// access the number of properties in the state
	size_t get_property_count() const;

	// Manually set the turn to a particularly player
	void set_current_turn(const Name_t& name);

	// Returns TRUE if the property with 'id' is in the GameState
	bool property_exists(Property_t id) const;

	// Access mutable property object with 'id'
	PropertyState & get_property(Property_t id);

	// Access immutable property object with 'id'
	const PropertyState & get_property(Property_t id) const;

	// Returns TRUE if player is added to the State
	// FALSE is returned if the player exists
	// this function does NOT set the current player
	bool add_player(const PlayerState& play);

	// Same as above, but reverse
	// if the player being deleted is the current player,
	// the caller is responsible for updating the next player. 
	// consider calling "next_turn" before deleting the player
	bool del_player(const Name_t& name); 

	// Returns TRUE if property is added to the State
	// FALSE is returned if the property exists
	bool add_property(const PropertyState& prop);
	// Save as above, but reverse
	bool del_property(Property_t id);

	// Encodes the State into a network-safe binary data string
	std::string encode() const;

	// looks for an unused player id up to max
	// if all ids are used, max is returned
	Player_t find_unused_id(int max) const;

	const Game_Base & get_base() const;

	const PlayerStateList & get_player_list() const;

	const PropertyStateList & get_property_list() const;

}; // end of class

} // end of namespace

#endif

