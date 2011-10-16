#ifndef LIMBO_H
#define LIMBO_H

#include <qwidget.h>

class QPushButton;
class QLabel;

namespace net {
	class GameState;
}

class Limbo : public QWidget {
	Q_OBJECT
private:
	QPushButton * _start;
	QLabel * _msg;

signals:
	void start_the_game_now();

public:
	Limbo (QWidget *paren=0, const char *name=0);

public slots:
	void enable_first_player();
	void start_pushed();

	void game_state_updated(net::GameState *);
};

#endif
