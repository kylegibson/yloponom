#ifndef TRADEREQUESTDIALOG_H
#define TRADEREQUESTDIALOG_H

#include <qdialog.h>

class QWidget;
class PlayerState;
namespace net{
class GameState;
class Trade;
}
class QString;
class QLabel;

class TradeRequestDialog : public QDialog {
	Q_OBJECT
	public:
		QLabel *properties[100];
		QLabel *theirProps[100];
		TradeRequestDialog(net::GameState *, net::Trade *, QWidget *, const char *name=0);
	private:
		net::GameState *gamestate;
		PlayerState *playerProp;
};

#endif
