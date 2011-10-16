#include "PlayerState.h"

	using namespace net;

	PlayerState::PlayerState(Player_t id, Name_t name, Money_t money, Position_t pos)
	: _id(id), _name(name), _money(money), _pos(pos), _jail(kFree), _dead(false)
	{ }

	PlayerState::PlayerState(Player_t id, Name_t name, Money_t money, Position_t pos, PropertyList_t props)
	: _id(id), _name(name), _money(money), _pos(pos), _jail(kFree), _owned(props), _dead(false)
	{ }

	PlayerState::PlayerState(const PlayerState & rhs)
	: _id(rhs._id), _name(rhs._name), _money(rhs._money), _pos(rhs._pos), 
	_jail(rhs._jail), _owned(rhs._owned), _dead(rhs._dead)
	{ }

	bool PlayerState::operator == (const Name_t & name) const
	{
		return get_name() == name;
	}

	bool PlayerState::operator != (const Name_t & name) const
	{
		return get_name() != name;
	}

	bool PlayerState::operator == (const net::Player_t & id) const
	{
		return get_id() == id;
	}

	bool PlayerState::operator != (const net::Player_t & id) const
	{
		return get_id() != id;
	}

	Player_t PlayerState::get_id() const
	{
		return _id;
	}

	Name_t PlayerState::get_name() const
	{
		return _name;
	}

	Money_t PlayerState::get_money() const
	{
		return _money;
	}

	Position_t PlayerState::get_position() const
	{
		return _pos;
	}

	net::Jail_t PlayerState::get_jail_status() const
	{
		return _jail;
	}

	const PropertyList_t & PlayerState::get_owned_properties() const
	{
		return _owned;
	}

	void PlayerState::set_id(net::Player_t id)
	{
		_id = id;
	}

	// These functions replace the previous values
	void PlayerState::set_name(const Name_t & name)
	{
		_name = name;
	}

	void PlayerState::set_money(Money_t money)
	{
		_money = money;
	}

	void PlayerState::set_position(Position_t pos)
	{
		_pos = pos;
	}

	void PlayerState::set_jail_status(net::Jail_t stat)
	{
		_jail = stat;
	}

	void PlayerState::set_owned_properties(const PropertyList_t & props)
	{
		_owned = props;
	}

	// Returns FALSE if the property exists
	bool PlayerState::add_property(Property_t p)
	{
		if(has_property(p)) {
			return false;
		}
		_owned.push_back(p);
		return true;
	}

	// Returns TRUE if property was removed
	bool PlayerState::del_property(Property_t p)
	{
		PropertyList_t::iterator f = std::find(_owned.begin(), _owned.end(), p);
		if(f != _owned.end()) {
			_owned.erase(f);
		}
		return (f != _owned.end());
	}

	// These functions add onto the previous values
	void PlayerState::add_money(Money_t money)
	{
		_money += money;
	}

	void PlayerState::move_forward(Position_t pos)
	{
		_pos += pos;
	}

	// These functions substract from the previous values
	void PlayerState::sub_money(Money_t money)
	{
		_money -= money;
	}

	void PlayerState::move_back(Position_t pos)
	{
		_pos -= pos;
	}

	void PlayerState::kill()
	{
		_dead = true;
	}

	bool PlayerState::alive() const
	{
		return !_dead;
	}

	bool PlayerState::has_property(net::Position_t pos) const
	{
		PropertyList_t::const_iterator f = std::find(_owned.begin(), _owned.end(), pos);
		return f != _owned.end();
	}


