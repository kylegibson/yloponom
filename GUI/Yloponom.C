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
// vim600: noet sw=4 ts=4 fdm=marker

#define DEBUG(x) std::cerr << x << std::endl;
#ifdef DEBUG
#include <iostream>
#else
#define DEBUG(x) 
#endif

#include "Yloponom.h"

#include "BoardView.h"
#include "DiceView.h"
#include "InfoView.h"
#include "ServerConnect.h"
#include "Limbo.h"
#include "PlayerStats.h"
#include "ImproveDialog.h"
#include "CasinoDialog.h"
#include "TradeDialog.h"
#include "TradeRequestDialog.h"
#include "CasinoDialog.h"

#include "core/Random_Generator.h"
#include "core/PlayerState.h"
#include "core/Game_Base.h"

#include "network/GameState.h"
#include "network/trade_packet.h"
#include "network/action_packet.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qapplication.h>
#include <qtimer.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qtextedit.h>

#include <sstream>

#include <vector>

/////////////////////////////////////////////////////////////////////////////////////////////////
// Yloponom class
/////////////////////////////////////////////////////////////////////////////////////////////////
Yloponom::Yloponom( QWidget *parent, const char *name ) 
: QWidget( parent, name ), 
_serverConn(NULL), _limbo(NULL), _main(NULL), 
_lbl_tcount(NULL), _lbl_turn(NULL), _lbl_elapsed(NULL),
_boardview(NULL), _infoview(NULL), _diceview(NULL), _playerlist(NULL), _gamestate(NULL),
_action_log(NULL)
{ // {{{

	// The initial display of the Yloponom interface is a connection window
	// Then, when that window emits a switch signal, we switch to the limbo mode window
	// then to the

	this->setCaption("Yloponom - Better than HDTV");

	// create server connect
	_create_server_connect_window();

	// center ourself
	_center_widget_to_desktop(this);

	// construct the base yloponom window
	_create_main_window();

	// Connect signals to slots
	_create_initial_connections();

	// ?
	_tcount = -1;

	// starting at 0 elapsed time
	_elapsed_time = 0;

	_game_started = false;
} // }}}

void Yloponom::_create_server_connect_window()
{
	_serverConn = new ServerConnect( this, "_serverConn" );
	setFixedSize( _serverConn->size() );
}

void Yloponom::_create_main_window()
{
	// this is a holder widget for all the elements of yloponom
	_main = new QWidget( this, "main");
	_main->setFixedSize(870, 749);
	_main->setHidden(true);
	_main->setPaletteBackgroundColor(darkRed);

	// create a layout for the widget
	QGridLayout * grid = new QGridLayout(_main, 17, 6, -1, -1, "grid");

	_create_board_view(grid);
	_create_info_view(grid);
	_create_dice_view(grid);
	_create_player_list(grid);
	_create_turn_status_view(grid);
	_create_action_buttons(grid);
}

void Yloponom::_create_board_view(QGridLayout * g)
{
	// the boardview is the board... yes
	if(NULL == _main) return;

  	_boardview = new BoardView(_main, "_boardview");
  	_boardview->setFixedSize(672, 672);
  	g->addMultiCellWidget(_boardview, 0, 13, 1, 5, 0);
}

void Yloponom::_create_info_view(QGridLayout * g)
{
	if(NULL == _main) return;

	_infoview = new InfoView(_main, "_infoview");
	_infoview->setFixedSize(200,260);
	_infoview->setPaletteBackgroundColor(darkRed);
	g->addWidget(_infoview, 0, 0, 0);
}

void Yloponom::_create_dice_view(QGridLayout * g)
{
	if(NULL == _main) return;

	_diceview = new DiceView(_main, "_diceview");
	_diceview->setPaletteBackgroundColor(green);
	_diceview->setFixedSize(134, 76);
	g->addMultiCellWidget(_diceview, 15, 16, 5, 5, 0);
}

void Yloponom::_create_player_list(QGridLayout * g)
{
	if(NULL == _main) return;

	_playerlist = new PlayerStats(_main, "_playerlist");
	_playerlist->setPaletteBackgroundColor(darkBlue);
	g->addWidget(_playerlist, 2, 0, Qt::AlignCenter);

	_action_log = new QTextEdit(QString::null, QString::null, _main, "_action_log");
	_action_log->setFrameStyle(QFrame::Box);
	_action_log->setLineWidth(1);
	_action_log->setMaxLogLines(100);
	_action_log->setReadOnly(true);
	_action_log->setFocusPolicy(QWidget::NoFocus);
	_action_log->setTextFormat(Qt::RichText);
	_action_log->setFixedSize(190, 170);
	_action_log->setWordWrap(QTextEdit::WidgetWidth);
	_action_log->setHScrollBarMode(QScrollView::AlwaysOff);

	_action_log->setPaletteForegroundColor(lightGray);
	_action_log->setPaletteBackgroundColor(darkRed);

	_action_log->append("Welcome to Yloponom, where all your dreams come true!");
	
	g->addItem( new QSpacerItem(10, 25), 3, 0);
	g->addWidget(_action_log, 4, 0, Qt::AlignCenter);
}

void Yloponom::_create_turn_status_view(QGridLayout * g)
{
	if(NULL == _main) return;

	_lbl_turn = new QLabel("", _main, "_lbl_turn");
	_lbl_turn->setFixedHeight(15);
	_lbl_turn->setAlignment(AlignHCenter);
	_lbl_turn->setPaletteBackgroundColor(darkRed);
	_lbl_turn->setPaletteForegroundColor(white);
	g->addMultiCellWidget(_lbl_turn, 14, 14, 1, 5, 0);

	QLabel * turns_played = new QLabel("Turns played:", _main, "turns_played");
	g->addWidget(turns_played, 16, 1, 0);

	_lbl_tcount = new QLabel("", _main, "_lbl_tcount");
	g->addWidget(_lbl_tcount, 16, 2, 0);

	QLabel * time = new QLabel("Seconds Elapsed:", _main, "time");
	g->addWidget(time, 16, 3, 0);

	_lbl_elapsed = new QLabel("", _main, "_lbl_elapsed");
	g->addWidget(_lbl_elapsed, 16, 4, 0);

}

void Yloponom::_create_action_buttons(QGridLayout * g)
{
	if(NULL == _main) return;

	QPushButton * concede = new QPushButton("CONCEDE", _main, "concede");
	concede->setMinimumHeight(25);
	concede->setPaletteForegroundColor(red);
	connect(concede, SIGNAL(clicked()), qApp, SLOT(quit()));
	g->addWidget(concede, 15, 1, 0);

	tradeButton = new QPushButton("Offer Trade", _main, "trade");
	tradeButton->setMinimumHeight(25);
	connect(tradeButton, SIGNAL(clicked()), this, SLOT(tradeWindow()));
	g->addWidget(tradeButton, 15, 2, 0);

	improveButton = new QPushButton("Build Improvements", _main, "improve");
	improveButton->setMinimumHeight(25);
	connect(improveButton, SIGNAL(clicked()), this, SLOT(improveWindow()));
	g->addWidget(improveButton, 15, 3, 0);

	rollButton = new QPushButton("Roll Dice", _main, "roll");
	rollButton->setMinimumHeight(25);
	g->addWidget(rollButton, 15, 4, 0);

	// disable all the buttons, initially.
	rollButton->setEnabled(false);
	tradeButton->setEnabled(false);
	improveButton->setEnabled(false);
}

void Yloponom::_create_initial_connections()
{
	// this specifies when the connection is made to the server... successfully
	if(_serverConn != NULL) {
		connect(_serverConn, SIGNAL(connected(QString*)), this, SLOT(connected_to_server(QString*)));
		// when the game has started, we need to be ready!
		connect(_serverConn, SIGNAL(game_has_started()), this, SLOT(start_game()) );
	}

	// when the board is clicked, the property is sent to display the deed
	if(_boardview != NULL) {
		if(_infoview != NULL) {
			connect(_boardview, SIGNAL(board_clicked(int)), _infoview, SLOT(display_deed(int)));
		}
	}
}

void Yloponom::action(net::ActionPacket * act)
{
	if(_gamestate == NULL) {
		return;
	}

	int id = act->get_player();
	const PlayerState * me = _gamestate->get_player_ptr(_my_name);

	int my_id = -1;
	if(me != NULL) {
		my_id = me->get_id();
	}

	const PlayerState * player = _gamestate->get_player_ptr(id);

	if(player == NULL) {
		DEBUG("Why am I null? id="<< id);
		return;
	}

	const Game_Base & base = _gamestate->get_base();

	std::vector<unsigned> iparams;
	std::vector<std::string> sparams;

	act->fill_param_int(iparams, 10);
	act->fill_param_str(sparams, 10);

	std::stringstream ss;

	bool format = false;
	bool dead = false;
	bool jail = false;
	bool buy_prop = false;

	switch(act->get_state()) {
		case kAPTurnStart: // {{{
			ss << "* ";
			if(my_id != id) {
				ss << player->get_name() << "'s turn now.";

				rollButton->setEnabled(false);
				tradeButton->setEnabled(false);
				improveButton->setEnabled(false);

			} else {
				format = true;
				ss << "Your turn";

				rollButton->setEnabled(true);
				tradeButton->setEnabled(true);
				improveButton->setEnabled(true);

			}
			_update_turn_count();
			break; // }}}

		case kAPDead: // {{{
			ss << "* ";
			if(my_id != id) {
				ss << player->get_name() << " has quit (" << sparams[0] << ")";
			} else {
				format = dead = true;
				ss << "You have quit (" << sparams[0] << ")";
			}
			break; // }}}

		case kAPJail: // {{{ 

			_diceview->change_dice(iparams[1], iparams[2]);

			ss << "* ";

			switch(iparams[0]) {

			case kJAJustThere: // {{{
				if(my_id != id) { // different player
					ss << player->get_name() << " is in jail.";
				} else { // me
					format = jail = true;
					ss << "You are in jail.";
				} 
				break; // }}}

			case kJARoll: // {{{
				if(my_id != id) { // different player 
					ss << player->get_name() << " ";
				} else { // me
					format = true;
					ss << "You ";
				}
				if(iparams[1] == iparams[2]) 
					ss << "escaped from jail ";
				else 
					ss << "failed to escape from jail ";

				ss << "by rolling (" << iparams[1] << "," << iparams[2] << ")";

				break; // }}}

			case kJAPaidCash: // {{{
				if(my_id != id) { // different player
					ss << player->get_name() << " ";
				} else { // me
					format = true;
					ss << "You ";
				}
				ss << "posted bond and rolled (" << iparams[1] << "," << iparams[2] << ")";
				break; // }}}

			case kJAUsedCard: // {{{
				if(my_id != id) { // different player
					ss << player->get_name() << " ";
				} else { // me
					format = true;
					ss << "You ";
				}
				ss << "escaped from jail using a card and rolled (" << iparams[1] << "," << iparams[2] << ")";
				break; // }}}

			case kJADoubles: // {{{
				if(my_id != id) { // different player
					ss << player->get_name() << " was sent to jail ";
				} else { // me
					format = true;
					ss << "You have been sent to jail ";
				}
				ss << "for chaining doubles (" << iparams[1] << "," << iparams[2] << ")";
				break; // }}}

			}
			break; // }}}

		case kAPMoved: // {{{

			_diceview->change_dice(iparams[1], iparams[2]);

			ss << "* ";
			if(my_id != id) {
				ss << player->get_name() << " ";

			} else {
				_infoview->display_deed( iparams[0] );

				format = true;
				ss << "You ";
			}
			ss << " rolled (" << iparams[1] << "," << iparams[2] << ") ";
			if(NULL != base.name(iparams[0])) {
				ss << " and moved to " << *base.name(iparams[0]);
				if( base.type( iparams[0] ) == Game_Base::kJail && player->get_jail_status() == kFree ) {
					ss << " (Just visiting)";
				}
			}
			
			break; // }}}

		case kAPPaidDebt: // {{{
			ss << "* ";
			if(my_id != id) {
				ss << player->get_name() << " ";

			} else {
				format = true;
				ss << "You ";
			}
			if(NULL != base.name(iparams[0])) {
				ss << " paid $" << iparams[1] << " on " << *base.name(iparams[0]) << ".";
			}
			break; // }}}

		case kAPBuyProp: // {{{
/*
			ss << "* ";
			if(my_id != id) {
				ss << player->get_name() << " ";

			} else {
				format = true;
				ss << "You ";
				
			}
*/
			if(my_id == id && iparams[1] == 0) {
				buy_prop = true;
			}

/*
			switch(iparams[1]) {
			case 0: // asked
				
			case 1: // accept
			case 2: // decline
			}
*/

			break; // }}}

		default:
			ss << "unhandled action " << act->get_state() << ".";
			break;
	}

	std::string data = ss.str();
	if(!data.empty()) {
		_action_log->append( QString(format ? "<b><font color=darkBlue>" : "") + data + QString( format ? "</font></b>" : "") );
	}

	_action_log->scrollToBottom();

	if(dead) {
		you_lose();
	}

	if(jail) {
		display_jail();
	}

	if(buy_prop) {
		buy_message( iparams[0] );
	}
}

void Yloponom::_start_game_connections() // {{{
{
	if(_serverConn != NULL) {
		//connect(_serverConn, SIGNAL( update_game_state(net::GameState *) ), this, SLOT( game_state_updated(net::GameState *) ) );
		connect(_serverConn, SIGNAL( commit_suicide() ), this, SLOT( you_lose() ) );
		//connect(_serverConn, SIGNAL( buy_property() ), this, SLOT( buy_message() ) );
		connect(_serverConn, SIGNAL( show_card(int) ), this, SLOT( display_card(int) ) );
		connect(_serverConn, SIGNAL( show_casino() ), this, SLOT( displayCasino() ) );
		connect(_serverConn, SIGNAL( free_parking() ), this, SLOT( displayFreeTransport() ) );
		connect(_serverConn, SIGNAL( you_are_in_jail() ), this, SLOT( display_jail() ) );
		connect(_serverConn, SIGNAL( trade_message(net::Trade *) ), this, SLOT( trade_message(net::Trade *) ) );

		connect(_serverConn, SIGNAL( action_packet(net::ActionPacket *)), this, SLOT( action(net::ActionPacket *) ) );
		//connect(_serverConn, SIGNAL( update_dice(int,int) ), this, SLOT( dice_rolled(int,int) ));
	}

	if(_serverConn != NULL && _diceview != NULL) {
		connect(_serverConn, SIGNAL(update_dice(int,int)), _diceview, SLOT(change_dice(int,int)));
	}

	if(rollButton != NULL && _serverConn != NULL) {
		connect(rollButton, SIGNAL(clicked()), _serverConn, SLOT(send_the_roll_dice_packet()) );
	}

	connect(rollButton, SIGNAL(clicked()), this, SLOT(roll_button_pressed()) );


} // }}}

void Yloponom::roll_button_pressed()
{
	rollButton->setEnabled(false);
	tradeButton->setEnabled(false);
	improveButton->setEnabled(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Method to bring up improvement dialog box when button is clicked
/////////////////////////////////////////////////////////////////////////////////////////////////
void Yloponom::improveWindow()
{
	if(_gamestate->get_base().name(0) == NULL) {
		DEBUG("Base is not properly filled.");
		return;
	}

	const PlayerState * me = NULL;
	if(NULL == (me = _gamestate->get_player_ptr( _my_name ) ) ) {
		DEBUG("improveWindow - you do not exist");
		return;
	}

	unsigned count = me->get_owned_properties().size();
	if(count == 0) {
		QMessageBox::warning(NULL, "Yloponom - It's fun for everyone.", "You don't have any properties on which to build improvements");
		return;
	}

	ImproveDialog k(_gamestate, me, this);
	if(QDialog::Accepted == k.exec()) {
		// check for a valid property AND that a different level has been selected
		if(k.get_property() > 0 || _gamestate->get_property( k.get_property() ).get_level() != k.get_level() ) {
			DEBUG("Accepted. (" << k.get_level() << "," << k.get_property() << ")" );
			_serverConn->improve(k.get_property(), k.get_level());
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Method to bring up trade dialog box when button is clicked
/////////////////////////////////////////////////////////////////////////////////////////////////
void Yloponom::tradeWindow() // {{{
{
	if(_gamestate == NULL || _playerlist == NULL) return;

	QString * clicked_player = _playerlist->get_selected_player();

	const PlayerState * me = NULL, * other = NULL;

	if(NULL == clicked_player) {
		QMessageBox::warning(NULL, "Yloponom - It's fun for everyone.", "You must first select a player in the player list.");
	} else if(*clicked_player == _my_name) {
		QMessageBox::warning(NULL, "Yloponom - It's fun for everyone.", "You can't trade with yourself..");
	} else if( NULL == (me = _gamestate->get_player_ptr(_my_name)) ) {
		QMessageBox::warning(NULL, "Yloponom - It's fun for everyone.", "You can't trade, you don't exist (lol)");
	} else if( NULL == (other = _gamestate->get_player_ptr(*clicked_player)) ) {
		QMessageBox::warning(NULL, "Yloponom - It's fun for everyone.", "The selected player doesn't actually exist now");
	} else {
		int my_id = me->get_id();

		int o_id = other->get_id();

		DEBUG("1Requesting trade(me=" << my_id << "," << o_id << ")");
		TradeDialog k (_gamestate, me, other, this);
		DEBUG("2Requesting trade(me=" << my_id << "," << o_id << ")");


		if(QDialog::Accepted == k.exec()) {

			unsigned offmoney = k.get_offer_money();
			unsigned reqmoney = k.get_request_money();

			DEBUG("Trade(" << offmoney << "," << reqmoney << ")");

			std::vector<unsigned> offprops;
			std::vector<unsigned> reqprops;

			k.get_offer_props(offprops);
			k.get_request_props(reqprops);

			net::Trade trade(my_id, o_id, net::TS_INIT, offmoney, reqmoney);


			// add offer and requested properties
			std::vector<unsigned>::const_iterator it;
			for(it = offprops.begin(); it != offprops.end(); ++it) {
				trade.offer_property( *it );
			}

			for(it = reqprops.begin(); it != reqprops.end(); ++it) {
				trade.request_property( *it );
			}

			DEBUG("3Requesting trade(me=" << my_id << "," << o_id << ")");

			_serverConn->trade(&trade);

			DEBUG("4Requesting trade(me=" << my_id << "," << o_id << ")");
		}

	}
} // }}}

void Yloponom::displayCasino() // {{{
{
	DEBUG("Casino.");

	const PlayerState * me = NULL;

	if(NULL == (me = _gamestate->get_player_ptr( _my_name ) ) ) {
		DEBUG("casino - you don't exist");
		you_lose();
	}

	// They MUST accept the casino. if they cancel it somehow,
	// we just redisplay the window.
	CasinoDialog c (me, this, "c");

	do {
		// loop until the client accepts the window
	} while(QDialog::Rejected == c.exec() || c.get_bet_type() == -1 );


	unsigned int money = c.get_wager();
	unsigned int bet = c.get_bet_type();

	DEBUG("Money(" << money << ")");
	DEBUG("Bet-type(" << bet << ")");

	if(_serverConn != NULL) {
		// type = kCCraps, kCSeven, kCEleven, kCDuece, kCAorB
		_serverConn->casino(bet, money);
	}

} // }}}

void Yloponom::displayFreeTransport() // {{{
{
	DEBUG("Free transport.");

	switch( QMessageBox::question( this, "Yloponom - The terrorists have not won",
		"Hey there! You've landed on the special <b>FREE TRANSPORT!!</b> "
		"That's right, you can move to the other ring, right now! "
		"Best of all, it is <b>free of charge</b>. Do you want to move?",
		"Move me", "Don't move me", QString::null, 
		1, 1 ) ) {
	case 0: // The user clicked 'Move me'
		DEBUG("Yes, move me.");
		_serverConn->respond(true);
		break;
	case 1: // The user clicked 'Don't move me'
		DEBUG("Don't move, clicked.");
		_serverConn->respond(false);
		break;
	}

} // }}}

void Yloponom::updateTime()
{
	++_elapsed_time;
	_lbl_elapsed->setText(QString::number(_elapsed_time));
	_lbl_elapsed->repaint();
}

// slot that changes the current display
// from the Connect to the Limbo mode
void Yloponom::connected_to_server(QString* name)
{
	// update our name.
	_my_name = *name;

	DEBUG("connected(" << _my_name << ")");

	_serverConn->setHidden(true);
	_serverConn->setEnabled(false);
	
	_limbo = new Limbo(this, "_limbo");
	_limbo->setHidden(false);

	// _serverConn will notify us when we are the first player,
	// this should be sent to the limbo window
	connect(_serverConn, SIGNAL(i_am_first_player()), _limbo, SLOT(enable_first_player()) );

	// we also need to send a packet too ... 
	connect(_limbo, SIGNAL(start_the_game_now()), _serverConn, SLOT(send_the_start_game_packet()) );

	// This will update the game state
	connect(_serverConn, SIGNAL(update_game_state(net::GameState *)), _limbo, SLOT(game_state_updated(net::GameState *)));
	connect(_serverConn, SIGNAL(update_game_state(net::GameState *) ), this, SLOT( game_state_updated(net::GameState *) ) );
	connect(_serverConn, SIGNAL(update_game_state(net::GameState *)), _infoview, SLOT(game_state_updated(net::GameState *)));
	connect(_serverConn, SIGNAL(update_game_state(net::GameState *)), _boardview, SLOT(game_state_updated(net::GameState *)));
	connect(_serverConn, SIGNAL(update_game_state(net::GameState *)), _playerlist, SLOT(game_state_updated(net::GameState *)));


	setFixedSize( _limbo->size() );
}

void Yloponom::start_game() 
{
	// make some connections
	_start_game_connections();

	// hide and disable the limbo
	_limbo->setHidden(true);
	_limbo->setEnabled(false);

	// adjust
	setFixedSize( _main->size() );

	// center ourself
	_center_widget_to_desktop(this);

	// reveal main window
	_main->setHidden(false);

	setCaption( caption() + QString(" - ") + _my_name );

	// setup timer
	QTimer* timer = new QTimer(this, "timer");
	timer->start(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));

	DEBUG("Start game.");

	_game_started = true;
}


void Yloponom::_update_turn_count()
{
	++_tcount;
	_lbl_tcount->setText(QString::number(_tcount));
	_lbl_tcount->repaint();	
}

void Yloponom::dice_rolled(int d1, int d2)
{
}

void Yloponom::game_state_updated(net::GameState * newstate) { // {{{

	if(newstate == NULL) {
		DEBUG("State is null.");
		return;
	}

	// update to the new gamestate
	_gamestate = newstate;

	_main->repaint();

	// do not process the win/lose conditions
	// if the game hasn't begun
	if(!_game_started) return;


	const PlayerState * me = NULL;
	if(NULL == (me = _gamestate->get_player_ptr(_my_name))) {
		DEBUG("I do not exist!!!");
		you_lose();
	}

	if(_gamestate->get_player_count() == 1) {
		you_win();
	}

} // }}}

void Yloponom::display_card(int i)
{
	const Game_Base &base = _gamestate->get_base();
	const Game_Base::GB_Card* card = base.get_card(i);

	if(card==NULL) return;

	QMessageBox mb( card->deck ? "Community Chest" : "Chance" , card->message,
        QMessageBox::NoIcon,
        QMessageBox::Ok | QMessageBox::Default,
        QMessageBox::NoButton,
        QMessageBox::NoButton );

	if(card->deck){
		mb.setIconPixmap( QPixmap("bin/cc.png") );
		mb.setPaletteBackgroundColor(QColor(246,244,97));
	}
	else{
		mb.setIconPixmap( QPixmap("bin/chance.png") );
		mb.setPaletteBackgroundColor(QColor(246,171,65));
	}

	mb.exec();
}

void Yloponom::buy_message(int prop) { // {{{
	QString sprice;

	const PlayerState * me = NULL;
	if(NULL == (me = _gamestate->get_player_ptr(_my_name))) {
		DEBUG("buy_message() - I do not exist!");
		_serverConn->respond(false);
		return;
	}

	const Game_Base & base = _gamestate->get_base();
	const std::string * name = base.name( prop );
	if(name == NULL) {
		DEBUG("buy_message() - name for position " << me->get_position() << " not in base.");
		_serverConn->respond(false);
		return;
	}

	unsigned iprice = base.price(prop);
	sprice = QString::number(iprice);

	// This player doesn't have enough to buy this property. Don't let them try.
	if(me->get_money() < iprice) {
		DEBUG("buy_message() - Player does not have enough money to buy this property. Auto-decline.");
		_serverConn->respond(false);
		return;
	}
	
	QString message = QString("Would you like to buy ") + *name + QString(" for $") + sprice;

	QMessageBox buy( "Buy Property", message,
        QMessageBox::Question,
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No | QMessageBox::Escape,
        QMessageBox::NoButton );

	if(buy.exec() == QMessageBox::Yes) {
		_serverConn->respond(true);
	} else{
		_serverConn->respond(false);
	}

} // }}}

void Yloponom::you_win()
{
	_serverConn->disconnect();

	QMessageBox::information(NULL, "You Win!!!!111oneoneeleventy2", "Congradufication, you have won at Yloponom. Please play again." );

	qApp->quit();
}


void Yloponom::you_lose()
{
	_serverConn->disconnect();
	//_serverConn->kill();

	QMessageBox::warning(NULL, "You lose!", "You lose! Get out of here!!" );
	qApp->quit();
}

void Yloponom::trade_message(net::Trade * trade)
{
	DEBUG("trade_message.");

	switch(trade->get_trade_state()) {
	case net::TS_INIT:
		// display window asking player to accept/decline
		display_trade_request(trade);
		break;
	case net::TS_ACCEPT:
		// tell current player that the trade was accepted, and applied
		QMessageBox::information(NULL, "Trade Transaction", "The trade has been accepted, and applied." );
		break;
	case net::TS_DECLINE:
		// tell current player that the trade was not accepted and not applied.
		QMessageBox::information(NULL, "Trade Transaction", "Sorry, the trade was declined. No items were exchanged." );
		break;
	}
}

void Yloponom::display_trade_request(net::Trade* trade)
{
	unsigned src = trade->get_source_player();
	unsigned offmoney = trade->get_offered_money();
	DEBUG("TradeRequest(src="<<src<<",offmoney="<<offmoney<<")");

	TradeRequestDialog c (_gamestate, trade, this, "trade request dialog");

	switch(c.exec()) {
	case QDialog::Accepted:
		trade->set_trade_state(net::TS_ACCEPT);
		break;
	case QDialog::Rejected:
		trade->set_trade_state(net::TS_DECLINE);
		break;
	}

	_serverConn->trade(trade);

}

void Yloponom::_center_widget_to_desktop(QWidget * widget)
{
    QDesktopWidget *d = QApplication::desktop();
	
	int x = (d->width() - widget->width()) / 2;
	int y = (d->height() - widget->height()) / 2;

	widget->move(x, y);
}

void Yloponom::display_jail()
{
	if(_gamestate == NULL) { 
		DEBUG("jail() - null gamestate");
		return;
	}

	const PlayerState * me = NULL;
	if(NULL == (me = _gamestate->get_player_ptr(_my_name))) {
		DEBUG("jail() - I do not exist!");
		return;
	}

	bool has_card = me->has_property(255) || me->has_property(254);

	int result = 0;

	if(has_card) {
		result = QMessageBox::question ( NULL, "Welcome to Jail!", "You are in Jail. What would you like to do?", 
			"Roll the dice", "Pay $50", "Use a card", 0, 1 );
	} else {
		result = QMessageBox::question ( NULL, "Welcome to Jail!", "You are in Jail. What would you like to do?", 
			"Roll the dice", "Pay $50", QString::null, 0, 1 );
	}

	_serverConn->jail(result);
}

// vim600: noet sw=4 ts=4 fdm=marker
