#include "jail_packet.h"

#include "packet_decoder.h"
#include "packet_encoder.h"

namespace net {

	Jail::Jail(State_t state) 
	: Packet(A_JAIL), _state(state) 
	{ }

	Jail::Jail(Decoder * decoder)
	: Packet(decoder)
	{
		_state = decoder->getNumber<State_t>();
	}

	Jail::Jail(const Jail & rhs) 
	: Packet(rhs), _state(rhs._state)
	{ }

	State_t Jail::get_jail_state() const 
	{ 
		return _state;
	}

	std::string Jail::encode() const
	{
		Encoder e(*this);

		Length_t len = 0;
		len += e.addNumber( get_jail_state() );
		e.set_length(len);

		return e.get_data();
	}

}

