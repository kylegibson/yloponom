/////////////////////////////////////////////////////////////////////////////
// packet_encoder.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef PACKETENCODER_H
#define PACKETENCODER_H

#include "types.h"
#include "packet.h"
#include "trade_packet.h"
#include "register_packet.h"
#include "property_packet.h"
#include "jail_packet.h"

#include "serialize.h"

#include <string>

namespace net {

//
//
//
class Encoder {
public:

private:
	std::string _data;

public:
	Encoder(const Packet & p);
	Encoder(const Encoder & rhs);

	Length_t addData(const Type * data);
	Length_t addNumber(const Number& num);
	Length_t addSizedNumber(const Number& num);
	Length_t addString(const String& str);
	Length_t addSize(Length_t num);

	Length_t get_length() const;
	void set_length(Length_t len);

	const std::string& get_data() const;
};

} // End of namespace

#endif /* inclusion protection */

