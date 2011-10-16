#include "network/Server.h"
#include "network/GameState.h"
#include "core/GameLogicUnit.h"
#include "core/Game_Base.h"

#include <stdexcept>
#include <iostream>

#include <csignal>
#include <cassert>

const char *DEFAULT_PORT = "2210";
const char *DEFAULT_ADDR = "0.0.0.0";

const int TIME_TO_DIE = 0xDEADBEEF;
volatile sig_atomic_t loop = ! TIME_TO_DIE;
void signal_handler(int sig) {
	if(SIGINT == sig) {
		loop = TIME_TO_DIE;
	}
}

int main(int argc, char*argv[])
{
	signal(SIGINT, signal_handler);

	const char * port = NULL;
	const char * addr = NULL;

	if(argc > 1) {
		port = argv[1];
	} else {
		port = DEFAULT_PORT;
	}

	if(argc > 2) {
		addr = argv[2];
	} else {
		addr = DEFAULT_ADDR;
	}

	

	try {
		Game_Base base ("data.dat");

		assert(NULL != base.name(0));

		net::GameState state (&base);
		GameLogicUnit unit (&state);

		Server server(&unit, addr, port);
		return server.exec(&loop, TIME_TO_DIE);

	} catch( const std::exception & e ) {
		std::cout << e.what() << std::endl;
	}

	return 1;
}
