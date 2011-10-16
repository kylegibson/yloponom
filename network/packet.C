#include "packet.h"

#include <stdexcept>

#include "packet_decoder.h"
#include "packet_encoder.h"

namespace net {

	Packet::Packet(Decoder * decoder)
	{
		if(NULL == decoder) {
			throw std::runtime_error("Invalid argument");
		}
		_action = decoder->get_packet_action();
	}

	Packet::Packet(Action_t action)
	: _action(action)
	{ }

	Packet& Packet::operator=(const Packet & rhs)
	{
		_action = rhs.get_action();
		return *this;
	}

	Action_t Packet::get_action() const 
	{
		return _action;
	}

	std::string Packet::encode() const
	{
		Encoder e(*this);
		return e.get_data();
	}

}

