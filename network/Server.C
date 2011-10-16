/////////////////////////////////////////////////////////////////////////////
// Server.C
//
// Consists of the Server class implementation which allows for the
// hosting of a Yloponom game.
/////////////////////////////////////////////////////////////////////////////

#include "Server.h"

#include <stdexcept> 	// Exceptions

#include <cstring>
#include <cerrno>
#include <cassert>

#include <sys/socket.h> // socket
#include <netinet/in.h> // htons
#include <arpa/inet.h>	// inet_aton

#include "ServerClient.h"

#include "core/GameLogicUnit.h"

//#define DEBUG(x) (std::cout << x << std::endl);

#ifdef DEBUG
#include <iostream>
#else
#define DEBUG(x) 
#endif

Server::Server(GameLogicUnit* unit, const std::string & host, const std::string & port)
: _server(-1), _logic(unit)
{
	assert(unit != NULL);

	// We're going to work with IPV4 ;)
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;

	// Convert dotted-quad string to usable address
	if(0 == ::inet_aton(host.c_str(), &addr.sin_addr) ) {
		throw std::runtime_error("Host address is not in dotted-quad IP format");
	}

	int iport = atoi(port.c_str());

	if(iport <= 0 || iport > 0xFFFF ) {
		throw std::runtime_error("Supplied port is not valid: must be greater than 0 and less than 65536.");
	}

	// Convert the port to integer, then to net-order.
	addr.sin_port = htons( iport );

	// Create a socket
	if(-1 == (_server = ::socket(addr.sin_family, SOCK_STREAM, 0) ) ) {
		throw std::runtime_error(strerror(errno));
	}

	int opt = 1;
	// Reuse the address (since we're serving it wont change)
	if(-1 == ::setsockopt(_server, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(int))) {
		throw std::runtime_error(strerror(errno));
	}
	
	// Bind to our host address
	if(-1 == ::bind(_server, (struct sockaddr*)&addr, sizeof( sockaddr_in ) ) ) {
		switch(errno) {
		case EADDRNOTAVAIL:
			throw std::runtime_error("The I.P. address you have supplied is not available for use on this machine.");
		case EACCES:
			throw std::runtime_error("You don't have the necessary permissions to bind to this address/port");
		case EADDRINUSE:
			throw std::runtime_error("Pick a different address/port, the one you are trying is being used.");
		default:
			throw std::runtime_error(strerror(errno));
		}
	}

	// Listen for connections
	if(-1 == ::listen(_server, 10) ) {
		throw std::runtime_error(strerror(errno));
	}
}

Server::~Server()
{
	DEBUG("~Server()");
	// Clean up existing connections which haven't been lost
	ClientList::iterator start = _clients.begin();
	ClientList::const_iterator end = _clients.end();

	while(start != end) {
		start = del_client(start);
	}
	DEBUG("+~Server()");
	::close(_server);
}

void Server::add_client(Socket_t s)
{
	_clients.push_back ( new ServerClient(s) );
}

Server::ClientList::iterator Server::del_client(ClientList::iterator iter)
{
	ServerClient* c = *iter;

	_logic->remove(c);

	DEBUG("Deleting " << c->sock);
	delete c;
	return _clients.erase(iter);
}

Server::ClientList::iterator Server::del_client(Socket_t s)
{
	ClientList::iterator start = _clients.begin();
	ClientList::const_iterator end = _clients.end();
	while(start != end) {
		ServerClient* c = *start;
		if(c->sock == s) {
			return del_client(start);
		}
		++start;
	}
	return start;
}

void Server::init_sets(fd_set & rfds, fd_set & wfds)
{
	// initialize them to nothing
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);

	// Add each client to the arrays
	ClientList::iterator start = _clients.begin();
	ClientList::const_iterator end = _clients.end();

	// monitor for incoming connections
	FD_SET(_server, &rfds);

	while(start != end) {
		ServerClient * s = *start++;
		FD_SET(s->sock, &rfds);
		FD_SET(s->sock, &wfds);
	}
}

bool Server::poll_sets(fd_set & rfds, fd_set & wfds)
{
	if(-1 == ::select(FD_SETSIZE, &rfds, &wfds, 0, 0) ) {
		// Don't stop on interruptions
		if(errno == EINTR) {
			return false;
		}

		throw std::runtime_error(strerror(errno));
	}
	return true;
}

bool Server::handle_read(ServerClient* client)
{
	std::string buffer(BUFSIZ, 0);
	ssize_t bytes = -1;

	if(-1 == (bytes = ::read(client->sock, &buffer[0], buffer.size() ) ) ) {
		return false;
	}

	if(0 == bytes) {
		DEBUG("EOF ON " << client->sock);
		return false;
	}

	// append the received data to the client's read buffer
	client->rbuf.append(buffer.begin(), buffer.begin() + bytes);

	// Now let the magic begin

	switch(_logic->apply_to(client)) {
		case GameLogicUnit::kASuccess:
		case GameLogicUnit::kANotDone:	return true;
		case GameLogicUnit::kAInvalArg:DEBUG("client is invalid");
		case GameLogicUnit::kAFail:	return false;
	}	

	return true;
}

bool Server::handle_send(ServerClient* client)
{
	if(!client->wbuf.empty()) {
		ssize_t bytes = -1;
		if(-1 == (bytes = ::write(client->sock, &client->wbuf[0], client->wbuf.size() ) ) ) {
			return false;
		}
		client->wbuf.erase( client->wbuf.begin(), client->wbuf.begin() + bytes );
	}
	return true;
}

void Server::process_sets(fd_set & rfds, fd_set & wfds)
{
	// Add each client to the arrays
	ClientList::iterator start = _clients.begin();
	ClientList::const_iterator end = _clients.end();

	while(start != end) {
		ServerClient * s = *start;	

		// ready to read
		if(FD_ISSET(s->sock, &rfds)) {
			if(!handle_read(s)) {
				start = del_client(start);
				continue;
			}
		}

		// ready to write
		if(FD_ISSET(s->sock, &wfds)) {
			if(!handle_send(s)) {
				start = del_client(start);
				continue;
			}
		}

		++start;
	}
}

void Server::check_incoming(fd_set & rfds)
{
	// if the server is "readable", then a connection is ready.
	if(FD_ISSET(_server, &rfds)) {
		int client = -1;
		if(-1 == (client = ::accept(_server, 0, 0) ) ) {
			throw std::runtime_error(strerror(errno));
		}
		DEBUG("Accepted " << client);
		add_client(client);
	}
}

int Server::exec(volatile sig_atomic_t * loop, int condition)
{
	assert(loop != 0);

	while(*loop != condition) {
		// Create file descriptor arrays
		fd_set readfds;
		fd_set writefds;

		// initialize
		init_sets(readfds, writefds);

		// see which ones are active
		if(!poll_sets(readfds, writefds)) {
			continue; // interrupted
		}

		// handle IO on sockets
		process_sets(readfds, writefds);

		// see if there is a new connection
		check_incoming(readfds);

		// dont be a CPU hog
		usleep(100000);
	}

	DEBUG("Done");

	return 0;
}

