/////////////////////////////////////////////////////////////////////////////
// dice_packet.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef DICE_H
#define DICE_H

#include "types.h"
#include "packet.h"
#include <string>

namespace net {

enum ED_state { kDFail, kDGood, kDJail };

class Decoder;
class Dice : public Packet {
private:
	State_t _state;
	Dice_t _one;
	Dice_t _two;

public:
	Dice(State_t state, Dice_t one, Dice_t two);
	Dice(Decoder * decoder);
	Dice(const Dice & rhs);

	virtual ~Dice() { }

	State_t get_state() const;
	Dice_t get_first_dice() const;
	Dice_t get_second_dice() const;

	virtual std::string encode() const;
};


} // End of namespace

#endif /* inclusion protection */

