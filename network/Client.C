/////////////////////////////////////////////////////////////////////////////
// Client.C
//
/////////////////////////////////////////////////////////////////////////////

#include "Client.h"

#include "packet.h"
#include "packet_decoder.h"
#include "jail_packet.h"
#include "dice_packet.h"
#include "card_packet.h"
#include "casino_packet.h"
//#include "register_packet.h"
#include "property_packet.h"
#include "trade_packet.h"
#include "action_packet.h"

#include "GameState.h"
#include "core/Game_Base.h"

#include <stdexcept>
#include <cstring>
#include <cassert>
#include <cerrno>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>

#define DEBUG(x) std::cerr << x << std::endl;
#ifdef DEBUG
#include <iostream>
#else
#define DEBUG(x) 
#endif

namespace net {

Client::Client(const std::string & host, const std::string & port)
: _client(-1)
{
	// Make sure we are given good connect info
	if(host.empty() || port.empty()) {
		throw std::runtime_error("Host or port is empty");
	}

	struct addrinfo* result;

	struct addrinfo hints;
	hints.ai_flags          = 0;
	hints.ai_family         = PF_UNSPEC;
	hints.ai_socktype       = SOCK_STREAM;
	hints.ai_protocol       = IPPROTO_TCP;
	hints.ai_addrlen        = 0;
	hints.ai_addr           = NULL;
	hints.ai_canonname      = NULL;
	hints.ai_next           = NULL;

	int error = -1;
	// Generate a usable address structure
	if(0 != (error = getaddrinfo(host.c_str(), port.c_str(), &hints, &result) ) ) {
		switch(error) {
		case EAI_SERVICE: throw std::runtime_error("Please type in a valid, numeric port number");
		default: throw std::runtime_error(gai_strerror(error));
		}
	}

	assert(result != 0);

	// Create socket
	errno = 0;
	if(-1 == (_client = socket(result->ai_family, result->ai_socktype, result->ai_protocol) ) ) {
		freeaddrinfo(result);
		throw std::runtime_error( strerror(errno) );
	}

	// Connect the socket
	errno = 0;
	if(-1 == connect(_client, result->ai_addr, result->ai_addrlen) ) {
		freeaddrinfo(result);
		throw std::runtime_error( strerror(errno) );
	}

	freeaddrinfo(result);

	_base = new Game_Base("data.dat");
	_gstate = new GameState(_base);
	_trade = new Trade(0, 0, 0, 0, 0);
	_actpack = new ActionPacket(0,0);
}

Client::~Client()
{
	delete _actpack;
	delete _trade;
	delete _gstate;
	::close(_client);
}

void Client::sendall(std::string data) const
{
	while(!data.empty()) {
		ssize_t bytes = send(_client, &data[0], data.size(), MSG_NOSIGNAL);
		if(-1 == bytes) {
			throw std::runtime_error("Failed to send data to server.");
		}
		data.erase(0, bytes);
	}
}

Decoder Client::getpacket() 
{
	std::string buff = _data;
	char buf[BUFSIZ];

	while( true ) {

		try {
			// try and decode, this will throw an exception
			// if the data is not completed
			Decoder dec(buff);

			size_t tsize = dec.get_packet_length() + HEADER_SIZE;

			// deal with the case where several packets are sent in succession
			if(buff.size() == tsize) {
				//DEBUG("same size as packet");
				_data.clear();

			} else if( buff.size() > tsize ) {
				//DEBUG("more data than packet.");
				buff.erase(0, tsize);
				_data = buff;

			} else {
				throw std::runtime_error( "Buffer size is... less than the actual packet size?" );
			}

			return dec;

		} catch ( const IncompletePacketException & e ) { }

		errno = 0;
		ssize_t bytes = recv(_client, &buf, BUFSIZ, MSG_NOSIGNAL);
		switch(bytes) {
		case 0:		throw std::runtime_error( "Server has closed the connection. You have been disconnected. Goodbye :(" );
		case -1:	throw std::runtime_error( strerror(errno) );
		default:	buff.append(buf, bytes);
		}

	}

	throw std::runtime_error("Unknown error");
}

int Client::initialize(const std::string & name)
{
	// initialize sends a register packet to the server
	// and gets a response back. the server will either send:
	// 1) a simple packet with 'A_START_GAME' action
	// 2) a duplicate of the register packet you sent

	// if #1 is true, then this client is 'first' client,
	// and will subsequently control the game start

	// if #2 is true, then this client is not the first,
	// but has successfully registered.

	// any other response would indicate an error, such as
	// 1) too many players.
	// 2) game has already started
	// 3) nick name is being used

	assert(!name.empty());

	ActionPacket reg(kAPRegister);
	reg.add_sparam(name);
	sendpacket(&reg);
	
	// may throw an exception (should be handled in caller)
	Decoder dec = getpacket();

	switch( dec.get_packet_action() ) {
	case A_REG_FIRST: 
		return 1;

	case A_REG_GOOD:
		return 0;

	case A_REG_FAIL:
		return -1;
		
	default:
		throw std::runtime_error("Server responded with invalid packet");
		return -1;
	}
 	
	return -1;
}

void Client::sendpacket(Packet* p) const
{
	std::string data = p->encode();
	sendall(data);
}

void Client::send_simple(int action)
{
	Packet packet(action);
	sendpacket(&packet);
}

void Client::request_update()
{ 
	send_simple(A_SEND_UPDATE);
}

void Client::start_game()
{
	send_simple(A_START_GAME);
}

bool Client::has_data() const
{
	struct timeval timeout = {0, 100000};
	fd_set set;
	FD_ZERO(&set);
	FD_SET(_client, &set);

	int r = -1;
 
	if(-1 == (r = select(_client + 1, &set, NULL, NULL, &timeout) ) ) {
		throw std::runtime_error("The network is having issues.");
	}

	bool timedout = (r == 0);

	bool ready = FD_ISSET(_client, &set);

	return (!timedout && ready) || !_data.empty();
}


int Client::handle_packet(int * params)
{
	Decoder dec = getpacket();

	switch(dec.get_packet_action()) {
	case A_REG_FIRST:
		return kHPFirstPlayer;

	case A_START_GAME:
		return kHPStartGame;

	case A_GAME_STATE:
		_handle_game_state(dec, params);
		return kHPGameState;

	case A_ROLL_DICE:
		_handle_roll_dice(dec, params);
		return kHPRolledDice;

	case A_YOU_DIED:
		return kHPDead;

	case A_BUY_PROP:
		return kHPBuyProp;

	case A_CARD:
		_handle_card(dec, params);
		return kHPCard;

	case A_CASINO:
		return kHPCasino;

	case A_FREE_PARKING:
		return kHPFreeParking;

	case A_TRADE:
		_handle_trade(dec, params);
		return kHPTrade;

	case A_JAIL:
		return kHPJail;

	case A_ACTION:
		_handle_action(dec, params);
		return kHPAction;

	default:
		throw std::runtime_error("Unknown packet.");
		return kHPBad;
	}
}

void Client::_handle_game_state(Decoder & dec, int * params)
{
	delete _gstate;
	_gstate = new GameState(&dec, _base);
	*params = reinterpret_cast<int> (_gstate);
}

void Client::_handle_roll_dice(Decoder & dec, int * params)
{
	Dice dice(&dec);
	*(params++) = dice.get_state();
	*(params++) = dice.get_first_dice();
	*(params++) = dice.get_second_dice();
}

void Client::_handle_card(Decoder & dec, int * params)
{
	Card card(&dec);
	*params = card.get_card();
}

void Client::_handle_trade(Decoder & dec, int * params)
{
	delete _trade;
	_trade = new Trade(&dec);
	*params = reinterpret_cast<int>( _trade );
}

void Client::_handle_action(Decoder & dec, int * params)
{
	delete _actpack;
	_actpack = new ActionPacket(&dec);
	*params = reinterpret_cast<int>( _actpack );
}


void Client::roll_dice() 
{
	send_simple(A_ROLL_DICE);
}

void Client::confirm()
{
	send_simple(A_CONFIRM);
}

void Client::decline()
{
	send_simple(A_DECLINE);
}

// type = kCCraps, kCSeven, kCEleven, kCDuece, kCAorB
void Client::casino(unsigned type, unsigned wager)
{
	net::Casino k(type, wager);
	sendpacket(&k);
}


void Client::improve(unsigned id, unsigned level)
{
	net::Property k(id, level);
	sendpacket(&k);
}


void Client::trade(net::Trade * trade)
{
	assert(trade != NULL);
	sendpacket(trade);
}

void Client::jail(unsigned option)
{
	net::Jail k(option);
	sendpacket(&k);
}


} // end of namespace

// vim600: noet sw=4 ts=4 fdm=marker
