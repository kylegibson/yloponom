#include "trade_packet.h"

#include "packet_decoder.h"
#include "packet_encoder.h"


namespace net {

	Trade::Trade(Player_t srcPlayerNum, Player_t dstPlayerNum, 
		  State_t state, Money_t offeredMoney, Money_t requestedMoney)
	: Packet(A_TRADE), 
		_state(state), _src(srcPlayerNum), _dst(dstPlayerNum), _offMoney(offeredMoney),
		_reqMoney(requestedMoney)
	{ }

	Trade::Trade(Decoder * decoder)
	: Packet(decoder)
	{
		_state = decoder->getNumber<State_t>();
		_offMoney = decoder->getNumber<Money_t>();
		_reqMoney = decoder->getNumber<Money_t>();
		_src = decoder->getNumber<Player_t>();
		_dst = decoder->getNumber<Player_t>();

		// offer props list
		Length_t offsize = decoder->getSize();
		while(offsize-- > 0) {
			Property_t n = decoder->getNumber<Property_t>();
			this->offer_property(n);
		}

		// request props list
		Length_t reqsize = decoder->getSize();
		while(reqsize-- > 0) {
			Property_t n = decoder->getNumber<Property_t>();
			this->request_property(n);
		}

	}

	Trade::Trade(const Trade & rhs)
	: Packet(rhs), _state(rhs._state), _src(rhs._src), 
		_dst(rhs._dst), _offMoney(rhs._offMoney), _reqMoney(rhs._reqMoney), 
		_offProps(rhs._offProps), _reqProps(rhs._reqProps)
	{ }

	void Trade::set_trade_state(State_t state)
	{
		_state = state;
	}


	State_t Trade::get_trade_state() const
	{
		return _state;
	}

	Money_t Trade::get_offered_money() const 
	{
		return _offMoney;
	}

	Money_t Trade::get_request_money() const 
	{
		return _reqMoney;
	}

	Player_t Trade::get_source_player() const 
	{
		return _src;
	}

	Player_t Trade::get_destin_player() const 
	{
		return _dst;
	}

	void Trade::offer_property(uint8_t propNum)
	{
		_offProps.push_back(propNum);
	}

	const PropertyList_t & Trade::get_offered_properties() const
	{
		return _offProps;
	}

	const PropertyList_t & Trade::get_requested_properties() const
	{
		return _reqProps;
	}

	void Trade::request_property(Property_t propNum)
	{
		_reqProps.push_back(propNum);
	}

	std::string Trade::encode() const
	{
		Encoder e(*this);

		Length_t len = 0;

		len += e.addNumber( get_trade_state() );
		len += e.addNumber( get_offered_money() );
		len += e.addNumber( get_request_money() );
		len += e.addNumber( get_source_player() );
		len += e.addNumber( get_destin_player() );

		// add the offer props list
		len += e.addSize(_offProps.size());

		PropertyList_t::const_iterator i = _offProps.begin();
		PropertyList_t::const_iterator ed = _offProps.end();

		while(i != ed) {
			len += e.addNumber( *(i++) );
		}

		// add the request props list
		len += e.addSize(_reqProps.size());

		i = _reqProps.begin();
		ed = _reqProps.end();

		while(i != ed) {
			len += e.addNumber( *(i++) );
		}

		e.set_length(len);

		return e.get_data();
	}

}

