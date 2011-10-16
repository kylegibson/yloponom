/////////////////////////////////////////////////////////////////////////////
// Client.h
//
// Contains class definition used by the Yloponom GUI to interface with the
// network layer, and communicate with the server.
/////////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "packet_decoder.h"

class Game_Base;

enum HPE_result { kHPBad, kHPFirstPlayer, kHPStartGame, 
	kHPGameState, kHPRolledDice, kHPDead, kHPBuyProp, kHPCard, 
	kHPCasino, kHPFreeParking, kHPTrade, kHPJail, kHPAction };

namespace net {

class Trade;
class GameState;
class ActionPacket;
class Client
{
private: 
	int _client;	// Socket descriptor
	std::string _data;

	Game_Base * _base;
	GameState * _gstate;
	Trade *	_trade;
	ActionPacket * _actpack;

	void sendall(std::string data) const;
	void sendpacket(Packet*) const;

	Decoder getpacket();

	void send_simple(int action);

	void _handle_game_state(Decoder & dec, int *);
	void _handle_roll_dice(Decoder & dec, int *);
	void _handle_card(Decoder & dec, int *);
	void _handle_trade(Decoder & dec, int *);
	void _handle_action(Decoder & dec, int *);

public:
	Client(const std::string &, const std::string &);
	/** 
	 * @brief	Connects to a Yloponom server 
	 * @param1	host name, or Internet protocol address
	 * @param2	numeric port number in range [0 - 65535]
	 * @errors
	 * 	(1) std::runtime_error => param1 OR param2 are empty
	 * 	(2) std::runtime_error => param1 OR param2 are not valid
	 *	(3) std::runtime_error => socket creation failure
	 *	(4) std::runtime_error => connection failure
	 */

	virtual ~Client();
	// Default destructor - deletes client data and closes the socket

	int initialize(const std::string &);
	/**
	 * @brief 	Registers with the connected-to server
	 * @param1	name to register with
	 * @return	-1:	Failed, name is in use
	 *			 0:	Success, registered with server
	 *			 1:	Success, registered and first player
	 */

	bool has_data() const;
	// determines whether the client has data available in buffer or to be read

	int handle_packet(int *);
	// handles an incoming packet

	void start_game();
	void request_update();
	void roll_dice();

	void confirm();
	void decline();

	// kCCraps, kCSeven, kCEleven, kCDuece, kCAorB
	//			type		wager
	void casino(unsigned, unsigned);

	//				id		level
	void improve(unsigned, unsigned);

	void trade(net::Trade *);

	void jail(unsigned);
};


}

#endif

// vim600: noet sw=4 ts=4 fdm=marker
