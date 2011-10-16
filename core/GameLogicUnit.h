/////////////////////////////////////////////////////////////////////////////
// GameLogicUnit.h
//
// Provides a network interface the server uses to interact with the case
// specific methods for modifying the GameState. The GameLogicUnit maintains
// a ruleset which performs actions based on network events
/////////////////////////////////////////////////////////////////////////////

#ifndef GAME_LOGIC_UNIT_H
#define GAME_LOGIC_UNIT_H

/*** LOCAL INCS ***/
	

/*** SYSTEM INCS ***/
	#include <string>
	#include <list>

/*** FORWARD DECLS ***/
	class GameRules;
	class PlayerState;
	class ServerClient;
	namespace net {
		class Packet;
		class GameState;
		class Decoder;
	}

/*** CLASS DEFN ***/
	class GameLogicUnit
	{
	public:
	/*** ENUMERATION TYPES ***/
	enum EGLU_apply { kASuccess, kAFail, kANotDone, kAInvalArg };
	
	/*** ADMIN ***/
		GameLogicUnit(net::GameState*);
		// Default constructor	- creates a logic unit
		// @param1	game state pointer

		virtual ~GameLogicUnit();
		// Default destructor - destroys the unit

	/*** INTERFACE ***/
		unsigned apply_to(ServerClient*);
		/**
		 * @brief	Add client to logic unit and process logic events
		 * @param1	pointer to server client stream
		 * @return 	kASuccess	= the logic operation succeeded
		 * 			kAFail		= the logic operation failed
		 *			kANotDone	= more data is needed from client
		 *			kAInvalArg	= @param1 is null, or otherwise not valid
		 */

		void remove(ServerClient*);
		/**
		 * @brief	Deletes and cleans up any information associated with
		 *			the client stream
		 * @param1	pointer to client stream to remove
		 * @post	the logic unit will operate with one less player
		 */

	private: 
	/*** PRIVATE STRUCTS ***/
		struct GLU_Player
		{
			ServerClient* client;
			PlayerState* state;
			unsigned flags;

			void set_flag(unsigned flag) { flags |= flag; }
			void clear_flag(unsigned flag) { flags &= ~flag; }
			bool has_flag(unsigned flag) const { return 0 != (flags & flag); }
		};

	/*** PRIVATE VARS ***/
		typedef std::list<GLU_Player> GLU_Player_List;
		GLU_Player_List _players;		// list of players
		GameRules*		_rules;			// ruleset
		net::GameState*	_state;			// game-data manipulated by rules
		unsigned		_status;		// status of the logic unit
		ServerClient*	_cturn;			// Current turn

	
	/*** PRIVATE METHODS ***/

		void _dump_game_state() const;


		bool _next_turn();
		/**
		 * @brief	Moves the turn to the next valid player
		 * @return	FALSE if there are no players left
		 */

		bool _set_turn(ServerClient*);
		bool _set_turn(GLU_Player_List::iterator);
		// returns false if client is invalid

		GLU_Player_List::iterator _get_player_by_client(ServerClient*);
		/**
		 * @brief	Searches for a player structure with a client stream
		 * @param1	pointer to a server client stream
		 * @return	pointer to GLU_Player structure or _bad_iter() if @param1
		 *			is invalid, or the player was not found
		 * @warn	iterator will become invalid if list is changed
		 */

		GLU_Player_List::iterator _get_player_by_flag(unsigned);
		/**
		 * @brief	
		 * @param1	single, or multiple, flags bit-wise OR'd together
		 * @return	pointer to GLU_Player structure or _bad_iter() if @param1
		 *			is invalid, or the player was not found
		 * @warn	iterator will become invalid if list is changed
		 */

		GLU_Player_List::iterator _get_player_by_id(unsigned);
		/**
		 * @brief	
		 * @param1	player-state id
		 * @return	pointer to GLU_Player structure or _bad_iter() if @param1
		 *			is invalid, or the player was not found
		 * @warn	iterator will become invalid if list is changed
		 */
		
		bool _is_bad_iter(GLU_Player_List::const_iterator) const;
		// returns True if the supplied iterator is 'bad'

		GLU_Player_List::const_iterator _bad_iter() const;
		GLU_Player_List::iterator _bad_iter();
		// returns a bad iterator

		bool _handle_packet(net::Decoder* decoder, ServerClient* client);
		/**
		 * @brief	process incoming packet based on the current logic state
		 * @param1	pointer to decoder for incoming packet
		 * @param2	pointer to client stream that sent the packet
		 * @return	TRUE if packet can be handled and is context valid
		 * @pre		decoder and client may not be NULL
		 * @post	client will have been added to logic unit if registering
		 */

		GLU_Player* _add_player(const std::string & name, ServerClient* client);
		/** 
		 * @brief	registers the player stream for game play as name
		 * @param1	pointer to decoder for incoming packet
		 * @param2	pointer to client stream that sent the packet
		 * @return	NULL if the player was not be added, else a pointer to the player
		 * @pre		decoder and client may not be NULL
		 * @post	adds the player to the unit
		 */

		bool _register_player(net::Decoder* decoder, ServerClient* client);
		/** 
		 * @brief	registers the player stream for game play
		 * @param1	pointer to decoder for incoming packet
		 * @param2	pointer to client stream that sent the packet
		 * @return	TRUE if packet is a register packet and client is added
		 * @pre		decoder and client may not be NULL
		 * @post	adds the player to the unit
		 */

		bool _limbo_mode(net::Decoder* decoder, ServerClient*);
		// TODO

		void _start_game();
		// TODO

		bool _game_mode(net::Decoder*, ServerClient*);
		// TODO

		void _send_game_state_to(ServerClient*) const;
		// TODO

		void _send_card_to(unsigned, ServerClient*) const;
		// TODO

		void _send_dice_to(unsigned, unsigned, unsigned, ServerClient*) const;
		// TODO

		bool _handle_roll_dice(ServerClient* client);
		// TODO

		bool _handle_casino(net::Decoder*, ServerClient*);
		// TODO

		bool _handle_improve(net::Decoder*, ServerClient*);
		// TODO

		bool _handle_trade(net::Decoder*, ServerClient*);
		// TODO

		bool _handle_jail(net::Decoder*, ServerClient*);
		// TODO

		bool _handle_move(GLU_Player_List::iterator);
		// TODO

		void _scan_for_dead_players();

		void _change_first_player(GLU_Player_List::iterator);
		/**
		 * @brief	sets the first player to the next player in list
		 * @param1	valid player iterator
		 * @pre		iterator must not be bad
		 * @post	next player in list will become first player if the list
		 *			has any other players. this new first player will be
		 * 			notified -only if- the current logic status is kFLimbo
		 */

		void _change_turn(GLU_Player_List::iterator);
		/**
		 * @brief	forwards to the next turn after this player
		 * @param1	valid player iterator
		 * @pre		iterator must not be bad
		 * @post	turn will have advanced to the next player
		 */

		void _respond_simple(unsigned, ServerClient*) const;
		// TODO

		void _respond(const std::string &, ServerClient*) const;
		/** 
		 * @brief	appends @param1 to the out buffer of the client
		 * @param1	encoded packet data
		 * @param2	pointer to client stream to send data to
		 * @pre		data may not be empty and client may not be NULL
		 */

		void _respond_to_all(const std::string &) const;
		/** 
		 * @brief	appends @param1 to all client's out buffers
		 * @param1	encoded packet data
		 * @pre		data may not be empty
		 */

		void _respond(net::Packet *, ServerClient*) const;

		void _respond_to_all(net::Packet *) const;


		//void _broadcast_simple_action(unsigned action, unsigned playerID) const;

		void _broadcast(unsigned action, unsigned playerID, ...) const;

		//void _broadcast_in_jail(unsigned playerID, unsigned status) const;
		//void _broadcast_death(unsigned playerID, const char * reason) const;

	}; // GameLogicUnit

#endif
