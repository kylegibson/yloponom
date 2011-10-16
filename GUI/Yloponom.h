#ifndef YLOPONOM_H
#define YLOPONOM_H

#include <qwidget.h>
#include <qstring.h>

class ServerConnect;
class Limbo;
class BoardView;
class InfoView;
class Tokens;
class DiceView;
class PlayerStats;
class PlayerState;

class QLabel;
class QPixmap;
class QPushButton;
class QGridLayout;
class QListBox;
class QTextEdit;

namespace net{
	class GameState;
	class Trade;
	class ActionPacket;
}

class Yloponom : public QWidget {
Q_OBJECT

private:
	ServerConnect* 	_serverConn;	// Server Connection dialog
	Limbo* 			_limbo;			// Pre-game (waiting) dialog
	QWidget* 		_main;			// Widget to contain all main stuff

	QLabel*			_lbl_tcount;	// Displays how many turns have occurred
	int				_tcount;		// Manages how many turns have occurred

	QLabel*			_lbl_turn;		// Displays the current turn
	QLabel*			_lbl_elapsed;	// Displays how many seconds have elapsed
	int				_elapsed_time;	// Manages how many seconds have elapsed

	BoardView*		_boardview;
	InfoView*		_infoview;
	DiceView*		_diceview;
	PlayerStats*	_playerlist;
	net::GameState*	_gamestate;

	QTextEdit* _action_log;

	bool _game_started;

	QPushButton *tradeButton;
	QPushButton *improveButton;
	QPushButton *rollButton;

	QString _my_name;

	void _center_widget_to_desktop(QWidget *);
	// Centers the widget to the desktop

	void _create_server_connect_window();
	void _create_main_window();
	void _create_board_view(QGridLayout *);
	void _create_info_view(QGridLayout *);
	void _create_dice_view(QGridLayout *);
	void _create_player_list(QGridLayout *);
	void _create_turn_status_view(QGridLayout *);
	void _create_action_buttons(QGridLayout *);
	void _create_initial_connections();

	void _update_turn_count();
	// increments the turn count by 1 and updates label

	void _start_game_connections();
	// called to connect signals and slots when the game starts

	void display_trade_request(net::Trade*);

public:
	Yloponom( QWidget *parent=0, const char *name=0);

public slots:
	// Received when a connection to the server is made
	// @param1 = registered name
	void connected_to_server(QString*);

	// received when the game has started
	void start_game();

	// received every second to update the time
	void updateTime();

	// received when a new game structure has been sent
	void game_state_updated(net::GameState *);

	// received when the player lands on the casino
	void displayCasino();

	// received when the player lands on a free parking transport
	void displayFreeTransport();

	// received when the player loses.
	void you_lose();

	// received when the player is the last player remaining
	void you_win();

	// received when a player is given a card
	void display_card(int);

	// received when a player lands on an unowned spot, and can buy it
	void buy_message(int);

	// received when a trade is incoming, or a response is coming
	void trade_message(net::Trade *);

	//
	void action(net::ActionPacket *);

	// 
	void display_jail();

	void tradeWindow();
	void improveWindow();

	// 
	void dice_rolled(int,int);


	void roll_button_pressed();	
};

#endif

// vim600: noet sw=4 ts=4 fdm=marker
