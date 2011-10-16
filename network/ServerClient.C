#include "ServerClient.h"
#include <unistd.h>		// close

ServerClient::ServerClient(int s) 
: sock(s), rbuf(BUFSIZ, 0), wbuf(BUFSIZ, 0)
{
	rbuf.clear();
	wbuf.clear();
}

ServerClient::ServerClient(const ServerClient & rhs) 
: sock(rhs.sock), rbuf(rhs.rbuf), wbuf(rhs.wbuf)
{ }

ServerClient::~ServerClient() 
{ 
	::close(sock);
}

