/////////////////////////////////////////////////////////////////////////////
// casino_packet.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef CASINOPACKET_H
#define CASINOPACKET_H

#include "types.h"
#include "packet.h"
#include <string>

namespace net {

enum BetType {

};

class Decoder;

//
//
//
class Casino : public Packet {
private:
	Byte_t	_bet;
	Money_t _wager;

public:
	Casino(Byte_t bet, Money_t wager);
	Casino(Decoder * decoder);
	Casino(const Casino & rhs);

	virtual ~Casino() { }

	Byte_t get_bet_type() const;
	Money_t get_wager() const;

	virtual std::string encode() const;
};


} // End of namespace

#endif /* inclusion protection */

