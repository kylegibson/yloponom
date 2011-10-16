#include "card_packet.h"

#include "packet_decoder.h"
#include "packet_encoder.h"

namespace net {

	Card::Card(Card_t card) 
	: Packet(A_CARD), _card(card) 
	{ }

	Card::Card(Decoder * decoder)
	: Packet(decoder)
	{
		_card = decoder->getNumber<Card_t>();
	}

	Card::Card(const Card & rhs) 
	: Packet(rhs), _card(rhs._card)
	{ }

	Card_t Card::get_card() const 
	{ 
		return _card;
	}

	std::string Card::encode() const
	{
		Encoder e(*this);

		Length_t len = 0;
		len += e.addNumber( get_card() );
		e.set_length(len);

		return e.get_data();
	}

}

