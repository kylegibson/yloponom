#include "casino_packet.h"
#include "packet_encoder.h"
#include "packet_decoder.h"

namespace net {

	Casino::Casino(Byte_t bet, Money_t wager)
	: Packet(A_CASINO), _bet(bet), _wager(wager)
	{ }

	Casino::Casino(Decoder * decoder) 
	: Packet(decoder)
	{
		_bet = decoder->getNumber<Byte_t>();
		_wager = decoder->getNumber<Money_t>();
	}

	Casino::Casino(const Casino & rhs) 
	: Packet(rhs), _bet(rhs._bet), _wager(rhs._wager)
	{ }
	
	Byte_t Casino::get_bet_type() const 
	{
		return _bet;
	}

	Money_t Casino::get_wager() const
	{
		return _wager;
	}

	std::string Casino::encode() const
	{
		Encoder e(*this);

		Length_t len = 0;

		len += e.addNumber( get_bet_type() );
		len += e.addNumber( get_wager() );

		e.set_length(len);

		return e.get_data();
	}

}

