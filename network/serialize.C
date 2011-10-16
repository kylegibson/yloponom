#include "serialize.h"

#include <netinet/in.h>

#define TO_NETBYTE_ORDER(x, y) (const char*) ((x=y) ? &x : &x)

namespace net {

	Type::Type(const std::string& data)
	: _data(data)
	{ }

	Type::Type(const char* ptr, size_t count)
	: _data(ptr, count)
	{ }

	Type::Type(const Type& rhs)
	: _data(rhs._data)
	{ }

	Type::~Type()
	{ }

	Length_t Type::size() const
	{
		return _data.size();
	}

	const char* Type::data() const
	{
		return _data.c_str();
	}

	Number::Number(uint32_t val)
	: Type( TO_NETBYTE_ORDER(val, htonl(val) ) , sizeof(uint32_t))
	{ 
	}

	Number::Number(uint16_t val)
	: Type( TO_NETBYTE_ORDER(val, htons(val) ), sizeof(uint16_t))
	{ }

	Number::Number(uint8_t val)
	: Type((const char*)&val, sizeof(uint8_t))
	{ }

	Number::Number(int32_t val)
	: Type( TO_NETBYTE_ORDER(val, htonl(val) ), sizeof(int32_t))
	{ }

	Number::Number(int16_t val)
	: Type( TO_NETBYTE_ORDER(val, htons(val) ), sizeof(int16_t))
	{ }

	Number::Number(int8_t val)
	: Type((const char*)&val, sizeof(int8_t))
	{ }
	
	// ----

	String::String(const std::string& str)
	: Type(str)
	{ }
	
}

