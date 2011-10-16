/////////////////////////////////////////////////////////////////////////////
// packet.h
//
// Contains basic packet structure information
/////////////////////////////////////////////////////////////////////////////

#ifndef PACKET_H
#define PACKET_H

#include "types.h"

#include <string>

namespace net {

// Packet action
enum Action {
	A_NONE,
	A_TRADE,			// 
	A_PROPERTY,			// 
	A_JAIL,				// 

	A_START_GAME,		// 
	A_SEND_UPDATE,      // 
	A_GAME_STATE,		// 

	A_REGISTER,			// Register packet, consists of a name
	A_REG_FAIL,			// Register failure, name is in use
	A_REG_GOOD,			// Register successful, name added
	A_REG_FIRST,		// Register successful, may send A_START_GAME

	A_ROLL_DICE, 		// try to roll the dice

	A_YOU_DIED,			// player died

	A_BUY_PROP,			// asks client whether to buy the property
	A_FREE_PARKING, 

	A_CONFIRM,			// sends a simple YES back to the server
	A_DECLINE,			// a simple NO

	A_CARD,

	A_CASINO,

	A_ACTION,

};

class Decoder;
class Packet {
protected:
	Action_t _action;	// The packet type, or action to perform

public:
	Packet(Action_t action);
	/** 
	 * @brief	Constructs a simple packet
	 * @param1	Packet type or action to perform
	 */

	Packet(Decoder * decoder);
	/** 
	 * @brief	Constructs from encoded network data 
	 * @param1	Decoder object encapsulating encoded data
	 * @errors
	 * 	(1) std::runtime_error => param1 is null
	 */

	virtual ~Packet() { }

	Packet& operator=(const Packet &);

	Action_t get_action() const;
	// Accessor method

	virtual std::string encode() const;
	/** 
	 * @brief	Transforms packet into network-safe binary data 
	 * @see		Encoder
	 * @return	string containing binary data
	 */

};

} // End of namespace

#endif /* inclusion protection */

