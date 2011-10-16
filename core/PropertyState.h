/////////////////////////////////////////////////////////////////////////////
// PropertyState.h
//
//
/////////////////////////////////////////////////////////////////////////////

#ifndef PROPERTYSTATE_H
#define PROPERTYSTATE_H

#include "network/types.h"
#include <string>

class PropertyState
{
private:
	net::Property_t _id;
	net::Player_t _owner;
	net::PropertyLevel_t _level;

public:

	static const net::Player_t NOTOWNED = static_cast<net::Player_t>(-1);

	PropertyState(net::Property_t id, net::Player_t owner=NOTOWNED, net::PropertyLevel_t level=0);
	PropertyState(const PropertyState & rhs);

	bool operator == (net::Property_t id) const;
	bool operator != (net::Property_t id) const;

	net::Property_t get_id() const;
	net::Player_t get_owner() const;
	net::PropertyLevel_t get_level() const;
	bool has_owner() const;

	// These functions replace the previous values
	void set_id(net::Property_t id);
	void set_owner(net::Player_t owner);
	void set_unowned();
	void set_level(net::PropertyLevel_t level);

	// Zeros the owner and level
	void reset();
};

#endif

