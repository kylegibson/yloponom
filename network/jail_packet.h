/////////////////////////////////////////////////////////////////////////////
// jail.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef JAILPACKET_H
#define JAILPACKET_H

#include "types.h"
#include "packet.h"
#include <string>

namespace net {

enum JailState {
	JS_ROLL,			//
	JS_PAY,				//
	JS_CARD				//
};

class Decoder;

class Jail : public Packet {
private:
	State_t _state;

public:
	Jail(State_t state);
	Jail(Decoder * decoder);
	Jail(const Jail & rhs);

	virtual ~Jail() { }

	State_t get_jail_state() const;
	std::string encode() const;
};

} // End of namespace

#endif /* inclusion protection */

