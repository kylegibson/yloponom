/////////////////////////////////////////////////////////////////////////////
// ServerClient.h
//
// ServerClient class definition for Yloponom
/////////////////////////////////////////////////////////////////////////////

#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include <string>
#include "Server.h"

class ServerClient
{
public:
	int sock;
	std::string rbuf;
	std::string wbuf;

	ServerClient(int s) ;
	ServerClient(const ServerClient & rhs);
	virtual ~ServerClient();
};

#endif

