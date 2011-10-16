/////////////////////////////////////////////////////////////////////////////
// property.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef PROPERTYPACKET_H
#define PROPERTYPACKET_H

#include "types.h"
#include "packet.h"
#include <string>

namespace net {

// Property Levels for Property packet
enum PropertyLevel {
	PL_NULL=-1,			//
	PL_EMPTY,			//
	PL_1HOUSE,			//
	PL_2HOUSE,			//
	PL_3HOUSE,			//
	PL_4HOUSE,			//
	PL_1HOTEL,			//
};

class Decoder;

//
//
//
class Property : public Packet {
private:
	Property_t		_prop;		// Property to manage (number)
	PropertyLevel_t	_level;		// Upgrade level, see PropertyLevel

public:
	Property(Property_t prop, PropertyLevel_t level);
	Property(Decoder * decoder);
	Property(const Property & rhs);

	virtual ~Property() { }

	Property_t get_number() const;
	PropertyLevel_t get_level() const;

	virtual std::string encode() const;
};


} // End of namespace

#endif /* inclusion protection */

