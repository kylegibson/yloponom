/////////////////////////////////////////////////////////////////////////////
// PlayerState.h
//
//
/////////////////////////////////////////////////////////////////////////////

#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

#include "network/types.h"
#include <string>

enum PSE_Jail { kFree, kFreshman, kSophie, kJunior, kSenior };

class PlayerState
{
private:
	net::Player_t _id;
	net::Name_t _name;
	net::Money_t _money;
	net::Position_t _pos;
	net::Jail_t _jail;
	net::PropertyList_t _owned;

	bool _dead;

public:
	PlayerState(net::Player_t id, net::Name_t name, net::Money_t money, net::Position_t pos);
	PlayerState(net::Player_t id, net::Name_t name, net::Money_t money, net::Position_t pos, net::PropertyList_t props);
	PlayerState(const PlayerState & rhs);

	bool operator == (const net::Name_t & name) const;
	bool operator != (const net::Name_t & name) const;
	bool operator == (const net::Player_t & id) const;
	bool operator != (const net::Player_t & id) const;

	net::Player_t get_id() const;
	net::Name_t get_name() const;
	net::Money_t get_money() const;
	net::Position_t get_position() const;
	net::Jail_t get_jail_status() const;
	const net::PropertyList_t & get_owned_properties() const;

	// These functions replace the previous values
	void set_id(net::Player_t id);
	void set_name(const net::Name_t & name);
	void set_money(net::Money_t money);
	void set_position(net::Position_t pos);
	void set_jail_status(net::Jail_t stat);
	void set_owned_properties(const net::PropertyList_t & props);

	// Returns FALSE if the property exists
	bool add_property(net::Property_t p);

	// Returns TRUE if property was removed
	bool del_property(net::Property_t p);

	// These functions add onto the previous values
	void add_money(net::Money_t money);
	void move_forward(net::Position_t pos);

	// These functions substract from the previous values
	void sub_money(net::Money_t money);
	void move_back(net::Position_t pos);

	bool has_property(net::Position_t pos) const;

	void kill();
	bool alive() const;
};

#endif

