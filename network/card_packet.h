/////////////////////////////////////////////////////////////////////////////
// card_packet.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef CARDPACKET_H
#define CARDPACKET_H

#include "types.h"
#include "packet.h"
#include <string>

namespace net {

class Decoder;

class Card : public Packet {
private:
	Card_t _card;

public:
	Card(Card_t card);
	Card(Decoder * decoder);
	Card(const Card & rhs);
	virtual ~Card() { }

	Card_t get_card() const;
	virtual std::string encode() const;
};

} // End of namespace

#endif /* inclusion protection */

