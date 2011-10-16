#include "packet_encoder.h"

#include <vector>
#include <netinet/in.h>
#include <cstring>

namespace net {

	Length_t Encoder::get_length() const
	{
		Length_t len = 0;
		memcpy(&len, &_data[0], sizeof(Length_t) );

		// return  length in host-byte order
		return ntohs(len);
	}

	void Encoder::set_length(Length_t len)
	{
		// the data is stored in network-byte order
		len = htons(len);
		memcpy(&_data[0], &len, sizeof(Length_t) );
	}

	Length_t Encoder::addData(const Type * data)
	{
		_data.append(data->data(), data->size());
		return data->size();
	}

	Length_t Encoder::addNumber(const Number& num)
	{
		return addData(&num);
	}

	Length_t Encoder::addSize(Length_t num)
	{
		return addNumber(num);
	}

	Length_t Encoder::addString(const String& str) 
	{
		Length_t len = 0;
		len += addSize( str.size() );
		len += addData(&str);
		return len;
	}

	Length_t Encoder::addSizedNumber(const Number& num)
	{
		Length_t len = 0;
		len += addSize(num.size());
		len += addNumber(num);
		return len;
	}

	Encoder::Encoder(const Packet & p)
	{
		// The length starts at zero, and we'll change it as data is added
		addSize( 0 );
		addNumber( p.get_action() );
	}

	Encoder::Encoder(const Encoder & rhs) 
	: _data(rhs._data)
	{ }
	
	const std::string& Encoder::get_data() const
	{
		return _data;
	}

}

