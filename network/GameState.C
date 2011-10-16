#include "GameState.h"

#include "core/Game_Base.h"

#include "packet_decoder.h"
#include "packet_encoder.h"


#include <stdexcept>
#include <cassert>

#include <vector>

namespace net {

	const char * DEFAULT_BASE = "data.dat";

	GameState::GameState()
	: Packet(A_GAME_STATE)
	{
		_base = new Game_Base(DEFAULT_BASE);
	}

	GameState::GameState(Game_Base * base)
	: Packet(A_GAME_STATE), _base(base), _clean_base(false)
	{
		assert(base != NULL);
	}

	GameState::GameState(Decoder * decoder, Game_Base * base)
	: Packet(decoder), _base(base), _clean_base(base == NULL)
	{
		std::string current = decoder->getString();

		// player state list
		Length_t playerSize = decoder->getSize();
		while(playerSize-- > 0) {
			Player_t id = decoder->getNumber<Player_t>();
			Name_t name = decoder->getString();
			Money_t money = decoder->getNumber<Money_t>();
			Position_t pos = decoder->getNumber<Position_t>();
			Jail_t jail = decoder->getNumber<Jail_t>();

			PropertyList_t props;

			// parse owned properties
			Length_t propsz = decoder->getSize();
			while(propsz-- > 0 ) {
				props.push_back( decoder->getNumber<Property_t>() );
			}

			PlayerState player(id, name, money, pos, props);
			player.set_jail_status(jail);

			add_player( player ); // THIS FUNCTION CHANGES THE CURRENT TURN...
			// [UPDATE] it doesn't change the current turn anymore
		}

		// property state list
		Length_t propSize = decoder->getSize();
		while(propSize-- > 0) {
			Property_t id = decoder->getNumber<Property_t>();
			Player_t owner = decoder->getNumber<Player_t>();
			PropertyLevel_t level = decoder->getNumber<PropertyLevel_t>();
			add_property( PropertyState(id, owner, level) );
		}

		if(_clean_base)
			_base = new Game_Base(DEFAULT_BASE);

		// UPDATE THE TURN AT THE END SO EVERYTHING WORKS
		_current_turn = current;
	}

	GameState::GameState(const GameState & rhs)
	: Packet(rhs), _current_turn(rhs._current_turn), _players(rhs._players),
		_props(rhs._props), _base(rhs._base), _clean_base(rhs._clean_base)
	{
		// we only want to construct a new game base if the state we are copying from
		// plans on deleting his copy
		if(_clean_base)
			_base = new Game_Base(DEFAULT_BASE);
	}

	GameState::~GameState()
	{
		if(_clean_base)
			delete _base;
	}

	GameState & GameState::operator=(const GameState & rhs)
	{
		Packet::operator=(rhs);
		_current_turn = rhs._current_turn;
		_players = rhs._players;
		_props = rhs._props;
		_base = rhs._base;
		_clean_base = rhs._clean_base;

		if(_clean_base) 
			_base = new Game_Base(DEFAULT_BASE);

		return *this;		
	}


	PlayerState* GameState::get_player_ptr(const Name_t& name) 
	{
		std::list < PlayerState >::iterator f = std::find( _players.begin(), _players.end(), name);
		if(f == _players.end())		return NULL;
		return &(*f);
	}

	PlayerState* GameState::get_player_ptr(const Player_t& id) 
	{
		std::list < PlayerState >::iterator f = std::find( _players.begin(), _players.end(), id);
		if(f == _players.end())		return NULL;
		return &(*f);
	}

	const PlayerState* GameState::get_player_ptr(const Name_t& name) const
	{
		std::list < PlayerState >::const_iterator f = std::find( _players.begin(), _players.end(), name);
		if(f == _players.end())		return NULL;
		return &(*f);
	}

	const PlayerState* GameState::get_player_ptr(const Player_t& id) const
	{
		std::list < PlayerState >::const_iterator f = std::find( _players.begin(), _players.end(), id);
		if(f == _players.end())		return NULL;
		return &(*f);
	}

	PlayerState* GameState::current_player()
	{
		return get_player_ptr(get_current_turn());
	}

	const PlayerState* GameState::current_player() const
	{
		return get_player_ptr(get_current_turn());
	}

	const Name_t& GameState::get_current_turn() const
	{
		return _current_turn;
	}

	PlayerState * GameState::get_newest_player()
	{
		if(_players.empty()) {
			return NULL;
		}

		return &_players.back();
	}

	PlayerState & GameState::get_player(const Name_t& name)
	{
		PlayerState * p = get_player_ptr(name);
		if(NULL == p) throw std::runtime_error("No such player");
		return *p;
	}

	const PlayerState & GameState::get_player(const Name_t& name) const
	{
		const PlayerState * p = get_player_ptr(name);
		if(NULL == p) throw std::runtime_error("No such player");
		return *p;
	}

	PlayerState & GameState::get_player(const Player_t& id)
	{
		PlayerState * p = get_player_ptr(id);
		if(NULL == p) throw std::runtime_error("No such player");
		return *p;
	}

	const PlayerState & GameState::get_player(const Player_t& id) const
	{
		const PlayerState * p = get_player_ptr(id);
		if(NULL == p) throw std::runtime_error("No such player");
		return *p;
	}

	size_t GameState::get_player_count() const
	{
		return _players.size();
	}

	size_t GameState::get_property_count() const
	{
		return _props.size();
	}

	void GameState::set_current_turn(const Name_t& name)
	{
		_current_turn = name;
	}

	bool GameState::property_exists(Property_t id) const
	{
		std::list < PropertyState >::const_iterator f = std::find( _props.begin(), _props.end(), id);
		return f != _props.end();
	}

	const PropertyState & GameState::get_property(Property_t id) const
	{
		std::list < PropertyState >::const_iterator f = std::find( _props.begin(), _props.end(), id);
		if(f == _props.end()) {
			throw std::runtime_error("No such property");
		}
		return *f;
	}

	PropertyState & GameState::get_property(Property_t id)
	{
		std::list < PropertyState >::iterator f = std::find( _props.begin(), _props.end(), id);
		if(f == _props.end()) {
			throw std::runtime_error("No such property");
		}
		return *f;
	}

	bool GameState::add_player(const PlayerState& play)
	{
		if(NULL != get_player_ptr(play.get_name())) {
			return false;
		}

		_players.push_back ( play );

		return true;
	}

	bool GameState::add_property(const PropertyState& prop)
	{
		if(property_exists(prop.get_id())) {
			return false;
		}
		_props.push_back( prop );
		return true;
	}

	bool GameState::del_player(const Name_t& name)
	{
		std::list < PlayerState >::iterator f = std::find( _players.begin(), _players.end(), name);
		if(f == _players.end()) { // player does not exist
			return false;
		}
		_players.erase(f);
		return true;
	}

	bool GameState::del_property(Property_t id)
	{
		std::list < PropertyState >::iterator f = std::find( _props.begin(), _props.end(), id);
		if(f == _props.end()) { // property does not exist
			return false;
		}
		_props.erase(f);
		return true;
	}

	std::string GameState::encode() const
	{
		Encoder e(*this);

		Length_t len = 0;
		len += e.addString( get_current_turn() );

		// add player states
		len += e.addSize( _players.size() );

		std::list < PlayerState >::const_iterator playerI = _players.begin();
		const std::list < PlayerState >::const_iterator playerE = _players.end();

		while(playerI != playerE) {
			const PlayerState& player = *(playerI++);
			// add this player data
			len += e.addNumber(player.get_id());
			len += e.addString(player.get_name());
			len += e.addNumber(player.get_money());
			len += e.addNumber(player.get_position());
			len += e.addNumber(player.get_jail_status());
			// now add the properties this player owns
			const PropertyList_t & playerProps = player.get_owned_properties();
			len += e.addSize(playerProps.size());

			PropertyList_t::const_iterator pindex = playerProps.begin();
			const PropertyList_t::const_iterator pend = playerProps.end();
			while (pindex != pend)
			{
				len += e.addNumber( *(pindex++) );
			}
		}

		// add property 
		len += e.addSize( _props.size() );

		std::list < PropertyState >::const_iterator propI = _props.begin();
		const std::list < PropertyState >::const_iterator propE = _props.end();

		while(propI != propE) {
			const PropertyState& prop = *(propI++);
			len += e.addNumber( prop.get_id() );
			len += e.addNumber( prop.get_owner() );
			len += e.addNumber( prop.get_level() );
		}

		e.set_length(len);

		return e.get_data();
	}

	Player_t GameState::find_unused_id(int max) const
	{
		std::list < PlayerState >::const_iterator i;
		std::list < PlayerState >::const_iterator e;

		for(int k = 0; k < max; ++k) {
			bool unused = true;
			i = _players.begin();
			e = _players.end();
			while(i != e) {
				if(i->get_id() == k) {
					unused = false;
					break;
				}
				++i;
			}
			// unused will be true if we can iterate
			// over all of the players and not find a match	
			if(unused) {
				return k; // this means this ID is not being used
				// and can be used now :)
			}
		}
		return max;
	}

	const Game_Base & GameState::get_base() const
	{
		return *_base;
	}

	const GameState::PlayerStateList & GameState::get_player_list() const
	{
		return _players;
	}

	const GameState::PropertyStateList & GameState::get_property_list() const
	{
		return _props;
	}

} // end of namespace

