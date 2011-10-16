#include "packet_decoder.h"

#include <netinet/in.h>

namespace net {

	Decoder::Decoder(const std::string& data)
	: _data(data), _pos(0), _plen(0), _pact(0)
	{
		if(sizeof(Length_t) > data.size()) {
			throw IncompletePacketException("Packet is incomplete.");
		}

		// extract the data length
		_plen = getNumber<Length_t>();

		// the amount of data in 'data' should exceed, or equal
		// the size of the packet header, plus the size of the packet 
		if(data.size() < (HEADER_SIZE + _plen) ) {
			throw IncompletePacketException("Packet is incomplete.");
		}

		// extract the packet action
		_pact = getNumber<Action_t>();
	}

	Decoder::Decoder(const Decoder & rhs) 
	: _data(rhs._data), _pos(rhs._pos), _plen(rhs._plen), _pact(rhs._pact)
	{ }

	long Decoder::get_long()
	{
		long val = 0;
		get_data(sizeof(long), &val);
		return ntohl(val);
	}

	short Decoder::get_short()
	{
		short val = 0;
		get_data(sizeof(short), &val);
		return ntohs(val);
	}

	char Decoder::get_char()
	{
		char val = 0;
		get_data(sizeof(char), &val);
		return val;
	}

	void* Decoder::get_data(Length_t size, void * buf)
	{
		memcpy(buf, &_data[_pos], size);
		_pos += size;
		return buf;
	}

	Length_t Decoder::getSize() 
	{
		return getNumber<Length_t>();
	}

	std::string Decoder::getString()
	{
		// the size of the string appears before it
		Length_t size = getSize();

		// directly copy it
		std::string str(&_data[_pos], size);
		_pos += size;

		return str;
	}

	uint32_t Decoder::getSizedNumber()
	{
		Length_t size = getSize();
		return getNumber<uint32_t>(size);
	}

	Length_t Decoder::get_packet_length() const
	{
		return _plen;
	}

	Action_t Decoder::get_packet_action() const
	{
		return _pact;
	}

	const std::string& Decoder::get_packet_data() const
	{
		return _data;
	}
}

