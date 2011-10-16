/////////////////////////////////////////////////////////////////////////////
// types.h
//
// Universal types file
/////////////////////////////////////////////////////////////////////////////

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <list>

// We will assume a word has a size of 4 bytes 
typedef uint32_t FullWord_t;
typedef uint16_t HalfWord_t;
typedef uint8_t Byte_t;
typedef std::string String_t;

namespace net {

typedef HalfWord_t Length_t;
typedef HalfWord_t Action_t;

// State refers to the status of the packet, a specific condition of the packet
typedef Byte_t State_t;

// Property refers to the identification of a property
typedef Byte_t Property_t;

// Position is a player's position on the board
typedef Property_t Position_t;

// A list of properties
typedef std::list<Property_t> PropertyList_t;

// A property has several upgrade levels
typedef Byte_t PropertyLevel_t;

// A player's name is a string
typedef String_t Name_t;

// A player has a numerical identification too
typedef Byte_t Player_t;

// Money should be a big field
typedef FullWord_t Money_t;

typedef Byte_t Jail_t;

typedef Byte_t Dice_t;

typedef Byte_t Card_t;

}

#endif /* inclusion protection */

