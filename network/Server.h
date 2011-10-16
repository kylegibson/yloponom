/////////////////////////////////////////////////////////////////////////////
// Server.h
//
// Server class definition for Yloponom
/////////////////////////////////////////////////////////////////////////////

#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <list>
#include <csignal> 		// sig_atomic_t

// Server class is for hosting a Yloponom Server
class GameLogicUnit;
class ServerClient;
class Server
{
public:
	typedef int Socket_t;
	typedef std::list< ServerClient* > ClientList;

private:
	Socket_t _server;
	ClientList _clients;
	GameLogicUnit* _logic;

	void add_client(Socket_t s);
	ClientList::iterator del_client(ClientList::iterator iter);
	ClientList::iterator del_client(Socket_t s);

	void init_sets(fd_set & rfds, fd_set & wfds);
	bool poll_sets(fd_set & rfds, fd_set & wfds);
	void process_sets(fd_set & rfds, fd_set & wfds);

	bool handle_read(ServerClient* client);
	bool handle_send(ServerClient* client);

	void check_incoming(fd_set & rfds);
	
public:
	// Constructs a Server
	Server(GameLogicUnit*, const std::string & host, const std::string & port);
	virtual ~Server();

	int exec(volatile sig_atomic_t * loop, int condition);
};

#endif
