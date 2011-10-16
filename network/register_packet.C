#include "register_packet.h"
#include "packet_decoder.h"
#include "packet_encoder.h"

#include <iostream>

namespace net {

	Register::Register(const Name_t& name)
	: Packet(A_REGISTER), _name(name)
	{ }

	Register::Register(Decoder * decoder)
	: Packet(decoder), _name(decoder->getString())
	{
	}

	Register::Register(const Register & rhs) 
	: Packet(rhs), _name(rhs._name) { }

	const Name_t& Register::get_name() const
	{
		return _name;
	}

	std::string Register::encode() const
	{
		Encoder e(*this);

		Length_t len = 0;
		len += e.addString( get_name() );
		e.set_length(len);

		return e.get_data();
	}


}

