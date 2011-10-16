/////////////////////////////////////////////////////////////////////////////////////////////////
//
// YLOPONOM
//
// CS3141 - Spring 2006
// Group 7
// Michael Hyde, Nick Quaderer, Will Pratt, Kyle Gibson
//
// GUI - This file contains the code for structuring the GUI for Yloponom
/////////////////////////////////////////////////////////////////////////////////////////////////

#define DEBUG(x) std::cerr << x << std::endl;
#ifdef DEBUG
#include <iostream>
#else
#define DEBUG(x) 
#endif

#include "ServerConnect.h"
#include "network/Client.h"
#include "network/packet.h"
#include "network/GameState.h"
#include "network/action_packet.h"

#include "core/Random_Generator.h"

#include <qapplication.h> // Use the quit SLOT of qApp
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qmessagebox.h>
#include <qlineedit.h>
#include <qtimer.h>

#include <stdexcept>

#include <vector>

#define REFRATE 500

/////////////////////////////////////////////////////////////////////////////////////////////////
// ServerConnect class to do initial connection to server
/////////////////////////////////////////////////////////////////////////////////////////////////
ServerConnect::ServerConnect( QWidget *parent, const char *name )
: QWidget( parent, name ), _client(NULL)
{
	setFixedSize(200, 200);

	QGridLayout * grid = new QGridLayout(this, 6, 3, 5);

	QLabel * title = new QLabel("<b>Connect to Server</b>", this, "title" );
	title->setFixedSize(180, 55);
	title->setAlignment( Qt::AlignCenter );
	grid->addMultiCellWidget(title, 0, 0, 0, 2, Qt::AlignCenter);

	// Add and connect the buttons
	QPushButton * btnconnect = new QPushButton("Connect", this, "connect");
	btnconnect->setDefault(true);
	grid->addWidget(btnconnect, 5, 0);

	QPushButton * btncancel = new QPushButton("Quit", this, "cancel");
	grid->addWidget(btncancel, 5, 2);

	connect(btncancel, SIGNAL(clicked()), qApp, SLOT(quit()));
	connect(btnconnect, SIGNAL(clicked()), this, SLOT(connect_to_server()));

	// create labels
	QLabel * lblserver = new QLabel( "Server:" , this, "lblserver");
	QLabel * lblport = new QLabel( "Port:" , this, "lblserver");
	QLabel * lblnick = new QLabel( "Nickname:" , this, "lblserver");

	grid->addWidget(lblserver, 1, 0);
	grid->addWidget(lblport, 2, 0);
	grid->addWidget(lblnick, 3, 0);

	// create the input fields
	_addressField = new QLineEdit( "localhost", this);
	_portField = new QLineEdit( "2210", this);

	Random_Generator rand;

	_nickField = new QLineEdit( QString("player-") + QString::number( rand.range(100, 999) ) , this);
	_nickField->setMaxLength(9);

	grid->addWidget(_addressField, 1, 2, Qt::AlignRight);
	grid->addWidget(_portField, 2, 2, Qt::AlignRight);
	grid->addWidget(_nickField, 3, 2, Qt::AlignRight);


}

ServerConnect::~ServerConnect()
{
	kill();
}

void ServerConnect::kill() 
{
	delete _client;
	_client = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Method to bring up beginning window to connect to server
/////////////////////////////////////////////////////////////////////////////////////////////////
void ServerConnect::connect_to_server()
{
	QString host = _addressField->text();
	QString port = _portField->text();
	QString name = _nickField->text();

	if(host.isEmpty() || port.isEmpty() || name.isEmpty()) {
		warn( "Input fields may not be blank" );
		return;
	}

	bool alloc = false;
	int ret = -1;
	try {
		if(_client == NULL) {
			// if the constructor of Client throws and exception
			// the data allocated WILL be freed for me :)
			_client = new net::Client(host, port);
		}
		alloc = true;
		// but if initialize fails, client still needs to be cleaned up
		ret = _client->initialize(name);

	} catch ( const std::exception & err ) {
		warn(err.what());

		// for the alloc case, we delete it
		if(alloc) {
			delete _client;
		}

		// for both cases, we reset the pointer
		_client = NULL;
		return;
	}

	switch(ret) {
	case -1: // fail
		warn("Name already in use. Pick a new one. Now.");
		return;

	case 0: // good
		emit connected(&name);
		break;

	case 1: // first
		emit connected(&name);
		emit i_am_first_player();
		break;
	}

	// create a refresh timer
	_timer = new QTimer(this, "timer");
	_timer->start(REFRATE);
	connect(_timer, SIGNAL(timeout()), this, SLOT(query_network()));

	return;
}


void ServerConnect::query_network()
{
	if(_client == NULL) { return; }

	_timer->stop();

	try {
		if(_client->has_data()) {
			std::vector<int> args(10, 0);
			
			int act = _client->handle_packet(&args[0]);

			switch(act) {

			case kHPBad: // this won would never actually come through
				break;

			case kHPFirstPlayer: // first
				emit i_am_first_player();
				break;

			case kHPStartGame: // game start
				emit game_has_started();
				break;

			case kHPGameState: // game update
				emit update_game_state( reinterpret_cast<net::GameState *> ( args[0] ) );
				break;

			case kHPRolledDice: // dice response
				_handle_dice_response(args[0], args[1], args[2]);
				break;

			case kHPDead:
				emit commit_suicide();
				break;

			case kHPBuyProp:
				emit buy_property();
				break;

			case kHPCard:
				emit show_card(args[0]);
				break;

			case kHPCasino:
				emit show_casino();
				break;
			
			case kHPFreeParking:
				emit free_parking();
				break;

			case kHPTrade:
				emit trade_message( reinterpret_cast<net::Trade *> ( args[0] ) );
				break;

			case kHPJail:
				emit you_are_in_jail();
				break;

			case kHPAction:
				emit action_packet( reinterpret_cast<net::ActionPacket *> ( args[0] ) );
				break;			

			}

		} else {
			_client->request_update();
		}

	} catch ( const std::exception & err ) {
		error(err.what());
	}

	_timer->start(REFRATE);
}


/*
void ServerConnect::_handle_action_packet(net::ActionPacket * act)
{
	switch(act->get_state()) {
		case kAPTurnStart: {
				std::string name;
				
				DEBUG("");
			} break;
		case kAPDead:
			break;
	}
}
*/


void ServerConnect::_handle_dice_response(unsigned state, unsigned d1, unsigned d2)
{
	if(state == 0) {
		warn("It wasn't your turn to roll. Now stop that.");
	} else if ( state == 2 ) {

		DEBUG("rolled_dice(" << d1 << "," << d2 << ")");
		emit update_dice(d1, d2);

		warn("You are now in Jail for speeding! Watch your rolls, next time.");

	} else {
		DEBUG("rolled_dice(" << d1 << "," << d2 << ")");
		emit update_dice(d1, d2);
	}
}

void ServerConnect::send_the_start_game_packet()
{
	if(_client == NULL) { return; }

	_timer->stop();

	try {
		_client->start_game();
	} catch ( const std::exception & err ) {
		error(err.what());
	}

	DEBUG("Send start game");
	_timer->start(REFRATE);
}

void ServerConnect::warn(const char * err)
{
	QMessageBox::warning(NULL, "Yloponom - It's fun for everyone", err );
}

void ServerConnect::error(const char * err)
{
	warn(err);
	abort();
}

void ServerConnect::send_the_roll_dice_packet()
{
	if(_client == NULL) { return; }

	_timer->stop();

	try {
		_client->roll_dice();
	} catch ( const std::exception & err ) {
		error(err.what());
	}

	DEBUG("Rolling the dice.");

	_timer->start(REFRATE);
}

void ServerConnect::respond(bool resp)
{
	if(_client == NULL) { return; }

	_timer->stop();

	try {
		if(resp) {
			_client->confirm();
		} else {
			_client->decline();
		}
	} catch ( const std::exception & err ) {
		error(err.what());
	}

	_timer->start(REFRATE);

}

// type = kCCraps, kCSeven, kCEleven, kCDuece, kCAorB
void ServerConnect::casino(unsigned type, unsigned wager)
{
	if(_client == NULL) { return; }

	_timer->stop();

	try {
		_client->casino( type, wager );

	} catch ( const std::exception & err ) {
		error(err.what());
	}

	_timer->start(REFRATE);
}

void ServerConnect::improve(unsigned id, unsigned level)
{
	if(_client == NULL) { return; }

	_timer->stop();

	try {
		_client->improve( id, level );

	} catch ( const std::exception & err ) {
		error(err.what());
	}

	_timer->start(REFRATE);
}

void ServerConnect::trade(net::Trade * trade)
{
	if(_client == NULL) { return; }

	_timer->stop();

	try {
		_client->trade( trade );

	} catch ( const std::exception & err ) {
		error(err.what());
	}

	_timer->start(REFRATE);
}

void ServerConnect::jail(unsigned option)
{
	if(_client == NULL) { return; }

	_timer->stop();

	try {
		_client->jail( option );

	} catch ( const std::exception & err ) {
		error(err.what());
	}

	_timer->start(REFRATE);
}


// vim600: noet sw=4 ts=4 fdm=marker
