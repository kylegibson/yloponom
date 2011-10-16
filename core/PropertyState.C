#include "PropertyState.h"

	using namespace net;

	PropertyState::PropertyState(Property_t id, Player_t owner, PropertyLevel_t level)
	: _id(id), _owner(owner), _level(level)
	{ }

	PropertyState::PropertyState(const PropertyState & rhs)
	: _id(rhs._id), _owner(rhs._owner), _level(rhs._level)
	{ }

	bool PropertyState::operator == (Property_t id) const
	{
		return get_id() == id;
	}

	bool PropertyState::operator != (Property_t id) const
	{
		return get_id() != id;
	}

	Property_t PropertyState::get_id() const
	{
		return _id;
	}

	Player_t PropertyState::get_owner() const
	{
		return _owner;
	}

	PropertyLevel_t PropertyState::get_level() const
	{
		return _level;
	}

	bool PropertyState::has_owner() const
	{
		return get_owner() != NOTOWNED;
	}

	// These functions replace the previous values
	void PropertyState::set_id(Property_t id)
	{
		_id = id;
	}

	void PropertyState::set_owner(Player_t owner)
	{
		_owner = owner;
	}

	void PropertyState::set_level(PropertyLevel_t level)
	{
		_level = level;
	}

	void PropertyState::set_unowned()
	{
		set_owner(NOTOWNED);
	}

	// Zeros the owner and level
	void PropertyState::reset()
	{
		set_owner(0);
		set_level(0);
	}


