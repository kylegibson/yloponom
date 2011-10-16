/////////////////////////////////////////////////////////////////////////////
// register.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef REGISTERPACKET_H
#define REGISTERPACKET_H

#include "types.h"
#include "packet.h"

#include <string>

namespace net {

class Decoder;

//
//
//
class Register : public Packet {
protected:
	Name_t _name;

public:
	Register(const Name_t& name);
	Register(Decoder * decoder);
	Register(const Register & rhs);

	virtual ~Register() { }

	const Name_t& get_name() const;

	virtual std::string encode() const;
};

} // End of namespace

#endif /* inclusion protection */

