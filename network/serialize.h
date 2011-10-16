/////////////////////////////////////////////////////////////////////////////
// serialize.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <string>
#include <vector>
#include <stdint.h>

#include "types.h"

namespace net {

	class Type {
	private:
		std::string _data;
	public:
		Type(const std::string& data);
		Type(const char* ptr, size_t count);
		Type(const Type& rhs);
		virtual ~Type();
		Length_t size() const;
		const char* data() const;
	};

	class Number : public Type {
	public:
		Number(uint32_t val);
		Number(uint16_t val);
		Number(uint8_t val);
		Number(int32_t val);
		Number(int16_t val);
		Number(int8_t val);
		Number(const Number& rhs);
	};

	class String : public Type {
	public:
		String(const std::string& str);
	};


}

#endif /* inclusion protection */

