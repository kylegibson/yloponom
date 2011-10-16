#include "action_packet.h"
#include "packet_encoder.h"
#include "packet_decoder.h"

#include <iostream>

const char * ap_fmt_list[ap_fmt_count]  = {
	0,			// kAPTurnStart
	"s",		// kAPDead(reason)
	"ddd",		// kAPJail(status,d1,d2)
	"ds",		// kAPResult(status,msg)
	"dddd",		// kAPCasino(status,earnings,d1,d2)
	"dd",		// kAPPaidDebt(property,amount)
	"ddd",		// kAPMoved(location,d1,d2)
	"dd",		// kAPImprove(property,level)
	"dd",		// kAPBuyProp(property,status(0=asked,1=bought,2=decline))
	"d",		// kAPCard(card)
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

namespace net {

	ActionPacket::ActionPacket(State_t state, Player_t player)
	: Packet(A_ACTION), _state(state), _player(player)
	{ }

	ActionPacket::ActionPacket(Decoder * decoder)
	: Packet(decoder)
	{
		_state = decoder->getNumber<State_t>();
		_player = decoder->getNumber<Player_t>();

		Length_t iparam_sz = decoder->getSize();
		while(iparam_sz-- > 0) {
			unsigned data = decoder->getNumber<unsigned>();
			_iparams.push_back(data);
		}

		Length_t sparam_sz = decoder->getSize();
		while(sparam_sz-- > 0) {
			std::string data = decoder->getString();
			_sparams.push_back(data);
		}

	}

	ActionPacket::ActionPacket(const ActionPacket & rhs) 
	: Packet(rhs), _state(rhs._state), _player(rhs._player), 
		_iparams(rhs._iparams), _sparams(rhs._sparams)
	{ }
	
	int ActionPacket::get_state() const
	{
		return _state;
	}

	int ActionPacket::get_player() const
	{
		return _player;
	}

	unsigned ActionPacket::fill_param_int(std::vector<unsigned> & arr, unsigned c) const
	{
		arr.clear();
		arr.reserve(c);

		std::vector<unsigned>::const_iterator it;
		for(it = _iparams.begin(); it != _iparams.end(); ++it, --c) {
			arr.push_back( *it );
		}

		while(c-- > 0) {
			arr.push_back( 0 );
		}
		
		return arr.size();
	}

	unsigned ActionPacket::fill_param_str(std::vector<std::string> & arr, unsigned c) const
	{
		arr.clear();
		arr.reserve(c);

		std::vector<std::string>::const_iterator it;
		for(it = _sparams.begin(); it != _sparams.end(); ++it, --c) {
			arr.push_back( *it );
		}

		while(c-- > 0) {
			arr.push_back( "" );
		}

		return arr.size();
	}

	bool ActionPacket::get_iparam(int & data, unsigned i) const
	{
		if( i >= _iparams.size() ) return false;
		data = _iparams[i];
		return 0 == data;
	}

	bool ActionPacket::get_sparam(std::string & data, unsigned i) const
	{
		if( i >= _sparams.size() ) return false;
		data = _sparams[i];
		return !data.empty();
	}

	void ActionPacket::add_iparam(unsigned data)
	{
		_iparams.push_back(data);
	}

	void ActionPacket::add_sparam(const std::string & data)
	{
		_sparams.push_back(data);
	}

	std::string ActionPacket::encode() const
	{
		Encoder e(*this);

		Length_t len = 0;

		len += e.addNumber( _state );
		len += e.addNumber( _player );

		// add the integer parameters
		len += e.addSize( _iparams.size() );
		std::vector<unsigned>::const_iterator iit;
		for(iit = _iparams.begin(); iit != _iparams.end(); ++iit) {
			len += e.addNumber( *iit );
		}

		// add the string parameters
		len += e.addSize( _sparams.size() );
		std::vector<std::string>::const_iterator sit;
		for(sit = _sparams.begin(); sit != _sparams.end(); ++sit) {
			len += e.addString( *sit );
		}
		e.set_length(len);

		return e.get_data();
	}

}

