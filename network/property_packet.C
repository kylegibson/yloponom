#include "property_packet.h"
#include "packet_encoder.h"
#include "packet_decoder.h"

namespace net {

	Property::Property(Property_t prop, PropertyLevel_t level)
	: Packet(A_PROPERTY), _prop(prop), _level(level)
	{ }

	Property::Property(Decoder * decoder) 
	: Packet(decoder)
	{
		_prop = decoder->getNumber<Property_t>();
		_level = decoder->getNumber<PropertyLevel_t>();
	}

	Property::Property(const Property & rhs) 
	: Packet(rhs), _prop(rhs._prop), _level(rhs._level) 
	{ }
	
	Property_t Property::get_number() const 
	{
		return _prop;
	}

	PropertyLevel_t Property::get_level() const
	{
		return _level;
	}

	std::string Property::encode() const
	{
		Encoder e(*this);

		Length_t len = 0;

		len += e.addNumber( get_number() );
		len += e.addNumber( get_level() );

		e.set_length(len);

		return e.get_data();
	}

}

