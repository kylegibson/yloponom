#include "dice_packet.h"
#include "packet_encoder.h"
#include "packet_decoder.h"

namespace net {

	Dice::Dice(State_t state, Dice_t one, Dice_t two)
	: Packet(A_ROLL_DICE), _state(state), _one(one), _two(two) 
	{ }

	Dice::Dice(Decoder * decoder)
	: Packet(decoder)
	{
		_state = decoder->getNumber<State_t>();
		_one = decoder->getNumber<Dice_t>();
		_two = decoder->getNumber<Dice_t>();
	}

	Dice::Dice(const Dice & rhs) 
	: Packet(rhs), _state(rhs._state), _one(rhs._one), _two(rhs._two)
	{ }
	
	State_t Dice::get_state() const
	{
		return _state;
	}

	Dice_t Dice::get_first_dice() const
	{
		return _one;
	}

	Dice_t Dice::get_second_dice() const
	{
		return _two;
	}

	std::string Dice::encode() const
	{
		Encoder e(*this);

		Length_t len = 0;

		len += e.addNumber( get_state() );
		len += e.addNumber( get_first_dice() );
		len += e.addNumber( get_second_dice() );

		e.set_length(len);

		return e.get_data();
	}

}

