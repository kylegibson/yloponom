/////////////////////////////////////////////////////////////////////////////
// trade.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef TRADEPACKET_H
#define TRADEPACKET_H

#include "types.h"
#include "packet.h"
#include "serialize.h"

#include <string>

namespace net {

// Trade state action
enum TradeState {
	TS_INIT,			// 
	TS_ACCEPT,			//
	TS_DECLINE			//
};

class Decoder;

class Trade : public Packet {

private:
	State_t _state;		// State of the packet, see TradeState

	Player_t _src; 		// Source of offer (player number)
	Player_t _dst; 		// Destination of offer (player number)
	Money_t _offMoney; 	// Money being offered to @dst
	Money_t _reqMoney;  	// Money that @src wants

	PropertyList_t _offProps;
	PropertyList_t _reqProps;

public:
	Trade(Player_t srcPlayerNum, Player_t dstPlayerNum, 
		  State_t state, Money_t offeredMoney, Money_t requestedMoney);
	Trade(Decoder * decoder);
	Trade(const Trade & rhs);
	
	virtual ~Trade() { }

	void set_trade_state(State_t state);

	State_t get_trade_state() const;
	Money_t get_offered_money() const;
	Money_t get_request_money() const;
	Player_t get_source_player() const;
	Player_t get_destin_player() const;

	const PropertyList_t & get_offered_properties() const;
	const PropertyList_t & get_requested_properties() const;

	void offer_property(Property_t propNum);
	void request_property(Property_t propNum);

	virtual std::string encode() const;

};


} // End of namespace

#endif /* inclusion protection */

