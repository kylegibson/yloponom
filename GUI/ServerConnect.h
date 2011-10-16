#ifndef SERVERCONNECT_H
#define SERVERCONNECT_H

#include <qwidget.h>

namespace net {
	class Client;
	class GameState;
	class Trade;
	class ActionPacket;
}

class QTimer;
class QLineEdit;
class QString;

class ServerConnect : public QWidget {
Q_OBJECT
private:
	net::Client* _client;		// Interfaces with the network and emits signals
	QLineEdit* _addressField;	//
	QLineEdit* _portField;		//
	QLineEdit* _nickField;		//
	QTimer* _timer;				//

	void warn(const char *);
	void error(const char *);

	void _handle_dice_response(unsigned, unsigned, unsigned);


public:
	ServerConnect(QWidget *parent=0, const char *name=0);
	virtual ~ServerConnect();

	// closes the client
	void kill();

signals:
	void connected(QString*);
	void game_has_started();
	void i_am_first_player();
	void update_game_state(net::GameState *);
	
	void update_dice(int, int);


	void commit_suicide();

	void buy_property();

	void show_card(int);

	void show_casino();

	void free_parking();

	void trade_message(net::Trade *);

	void you_are_in_jail();

	void action_packet(net::ActionPacket *);

public slots:
	void connect_to_server();
	void query_network();
	void send_the_start_game_packet();
	void send_the_roll_dice_packet();

	void respond(bool resp);

	// type = kCCraps, kCSeven, kCEleven, kCDuece, kCAorB
				// type		wager
	void casino(unsigned, unsigned);

				// id		level
	void improve(unsigned, unsigned);

	void trade(net::Trade *);

	void jail(unsigned);

};

#endif
