#ifndef BOARDVIEW_H
#define BOARDVIEW_H

#include <qwidget.h>
#include <qpixmap.h>
	#include <vector>

#include "Tokens.h"

class Tokens;
class QPaintEvent;
class QMouseEvent;
class QImage;
class QTimer;

namespace net {
	class GameState;
}

struct token_animate {
	int tk_id;		// token id to draw
	int cpos;		// current pos
	int mv_cnt;		// move counter
	bool anim;		// whether to draw at cpos, or the gamestate's pos
};

class BoardView : public QWidget {
Q_OBJECT
private:
	net::GameState*	_gamestate;
	QPixmap 		_background;
	Tokens 			tokens;
	QTimer*			_timer;
	std::vector<token_animate> _anim;

	void init_bg();
	void draw_prop_own_overlay(QPainter &, int, QColor *);

public:
	QImage *House;
	QImage *Hotel;
	QImage *HouseS;
	QImage *HotelS;
	BoardView ( QWidget *parent=0, const char *name=0 );
	void paintEvent(QPaintEvent *pe);
	void mousePressEvent(QMouseEvent *m);
	int propID(int , int);

	void draw_upgrades(QPainter &,int,int);

public slots:
	void token_animate_timeout();
	void game_state_updated(net::GameState *);

	// param1 = id, param2 = # spaces
	void moved_token(int, int);
	
signals:
	void board_clicked(int);

};

#endif
