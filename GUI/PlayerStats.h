#ifndef PLAYERSTATS_H
#define PLAYERSTATS_H

#include <qwidget.h>
#include <qstring.h>

#include "Tokens.h"

class QLabel;
class QMouseEvent;
class QColor;
class QString;
class QListView;
class QListViewItem;

namespace net {
	class GameState;
}

class PlayerStats : public QWidget {
Q_OBJECT
private:
	net::GameState * _gamestate;
	QListView* _list;
	Tokens _tokens;

public:
    PlayerStats ( QWidget *parent=0, const char *name=0 );
	QString * get_selected_player();

public slots:
	void game_state_updated(net::GameState *);

};

#endif

