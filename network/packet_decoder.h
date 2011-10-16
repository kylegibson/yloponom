/////////////////////////////////////////////////////////////////////////////
// packet_decoder.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef PACKETDECODER_H
#define PACKETDECODER_H

#include "types.h"
#include "packet.h"

#include <string>
#include <stdexcept>
#include <cstring>

namespace net {

static const size_t HEADER_SIZE	= ( sizeof(Length_t) + sizeof(Action_t) );

//
//
//
class IncompletePacketException : public std::runtime_error {
public:
	IncompletePacketException() : std::runtime_error("Exception occured") { }
	IncompletePacketException(const std::string & error) : std::runtime_error(error) { }
};

//
//
//
class Decoder {
private:
	std::string _data;
	size_t _pos;
	Length_t _plen;
	Action_t _pact;

public:
	Decoder(const std::string& data);
	Decoder(const Decoder & rhs);

	long get_long();
	short get_short();
	char get_char();
	void* get_data(Length_t size, void * buf);

	template <typename T>
	T getNumber(Length_t size = 0)
	{
		size = (size == 0) ? sizeof(T) : size;

		switch(size) {
		case sizeof(char):
			return get_char();
		case sizeof(short):
			return get_short();
		case sizeof(long):
			return get_long();
		}

		return 0xDEADBEEF;
	}

	Length_t getSize();
	std::string getString();
	uint32_t getSizedNumber();

	const std::string & get_packet_data() const;
	Length_t get_packet_length() const;
	Action_t get_packet_action() const;
};

} // End of namespace

#endif /* inclusion protection */

