/////////////////////////////////////////////////////////////////////////////
// action_packet.h
//
// 
/////////////////////////////////////////////////////////////////////////////

#ifndef ACTION_PACKET_H
#define ACTION_PACKET_H

#include "types.h"
#include "packet.h"
#include <string>
#include <vector>

const unsigned ap_fmt_count = 20;

extern const char * ap_fmt_list[ap_fmt_count];

enum EAP_actions 
	{
		/* the player_id specifies to which player the action occurs */
		// these messages should come from server
		kAPTurnStart,	// player_id's turn begins
		kAPDead,		// player_id is dead [sparam = reason]
		kAPJail,		// player's jail status [iparam1 = (0=> still in jail, 1=> paid money, 2=> used card), iparam2 = dice1, iparam3 = dice2]
		kAPResult,		// action result [iparam1 = good(1)/bad(0), sparam1 = reason]
		kAPCasino,		// casino result [iparam1 = win(1)/lose(0), iparam2 = net earnings, iparam3 = dice1, iparam4 = dice2]
		kAPPaidDebt,	// paid rent [iparam1 = propertyID]
		kAPMoved,		// moved on board [iparam1 = new location, iparam2 = dice1, iparam3 = dice2]
		kAPImprove,		// player improved property [iparam1 = rep. member id of group, iparam2 = level]
		kAPBuyProp,		// player purchased property [iparam1 = propID]
		kAPCard,		// player picked a card [iparam1 = cardID]

		// these messages originate from client
		kAPRegister,	// register player [sparam = nick name]

	};

/* in-jail status codes */
enum EJA_status 
	{
		kJARoll,
		kJAPaidCash,
		kJAUsedCard,
		kJAJustThere,
		kJADoubles,
	};

namespace net {

class Decoder;
class ActionPacket : public Packet {
private:
	State_t _state;
	Player_t _player;

	std::vector<unsigned> _iparams;
	std::vector<std::string> _sparams;

public:
	ActionPacket(State_t state, Player_t player = 0);
	ActionPacket(Decoder * decoder);
	ActionPacket(const ActionPacket & rhs);

	virtual ~ActionPacket() { }

	int get_state() const;
	int get_player() const;

	unsigned fill_param_int(std::vector<unsigned> &, unsigned) const;
	unsigned fill_param_str(std::vector<std::string> &, unsigned) const;

	// returns FALSE if the param is 0 or "", respectively (or not even there)
	bool get_iparam(int &, unsigned i) const;
	bool get_sparam(std::string &, unsigned i) const;

	void add_iparam(unsigned);
	void add_sparam(const std::string &);

	virtual std::string encode() const;
};


} // End of namespace

#endif /* inclusion protection */

