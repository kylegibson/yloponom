/////////////////////////////////////////////////////////////////////////////////////////////////
//
// YLOPONOM
//
// CS3141 - Spring 2006
// Group 7
// Michael Hyde, Nick Quaderer, Will Pratt, Kyle Gibson
//
// GUI - This file contains the code for structuring the GUI for Yloponom
////////////////////////////////////////////////////////////////////////////////////////////////
// vim600: noet sw=4 ts=4 fdm=marker

#include "BoardView.h"
#include "Tokens.h"
#include "network/GameState.h"
#include "core/Game_Base.h"
#include "core/PlayerState.h"
#include "core/PropertyState.h"

#include <qevent.h>
#include <qpainter.h>
#include <qimage.h>
#include <qtimer.h>
#include <iostream>

#define ANIMATE_TIME 300 // milliseconds

/////////////////////////////////////////////////////////////////////////////////////////////////
// BoardView
/////////////////////////////////////////////////////////////////////////////////////////////////
BoardView::BoardView( QWidget *parent, const char *name)
 : QWidget( parent, name ), _gamestate(NULL), _background(672,672)
{
	setFixedSize(672, 672);

	init_bg();

	for(int i = 0; i < 8; ++i ) {
		token_animate ta = { i, 0, 0, false };
		_anim.push_back(ta);
	}

	_timer = new QTimer(this, "_timer");
	connect(_timer,SIGNAL(timeout()), this, SLOT(token_animate_timeout()));
}

void BoardView::moved_token(int id, int spaces)
{
}

void BoardView::token_animate_timeout()
{
}

void BoardView::init_bg() { // {{{
	QPainter p(&_background);
	_background.fill(lightGray);

	p.fillRect(_background.rect(), QBrush(darkGray, DiagCrossPattern) );

	p.setPen( QPen(black, 4) );
	p.drawRect( _background.rect() );


	QImage BottomRail("bin/RRb.png");
	QImage LeftRail("bin/RRl.png");
	QImage TopRail("bin/RRt.png");
	QImage RightRail("bin/RRr.png");
	QImage BottomChance("bin/chanceb.png");
	QImage LeftChance("bin/chancel.png");
	QImage TopChance("bin/chancet.png");
	QImage RightChance("bin/chancer.png");
	QImage BottomCommunity("bin/ccb.png");
	QImage LeftCommunity("bin/ccl.png");
	QImage TopCommunity("bin/cct.png");
	QImage RightCommunity("bin/ccr.png");
	QImage Heat("bin/heat.png");
	QImage Water("bin/ww.png");
	QImage Elec("bin/elec.png");
	QImage Income("bin/income.png");
	QImage Luxury("bin/lux.png");
	QImage Sales("bin/sales.png");
	QImage GO("bin/go.png");
	QImage Free("bin/free.png");
	QImage GoToJail("bin/go_to_jail.png");
	QImage GoToJail2("bin/go_to_jail2.png");
	QImage Jail("bin/jail.png");
	QImage Casino("bin/casino.png");

	House = new QImage ("bin/house.png");
	Hotel = new QImage ("bin/hotel.png");
	HouseS = new QImage ("bin/house-s.png");
	HotelS = new QImage ("bin/hotel-s.png");



//-----------------------------------------------------------------------------------------------
// Set up board
//-----------------------------------------------------------------------------------------------

	p.setBrush(white);
	p.setPen(black);
	//-------------------------------------------------------------------------------------------
	// Create top of outer ring
	//-------------------------------------------------------------------------------------------
	p.drawImage(27, 27, Free);
	p.drawRect(111, 27, 50, 84);
	p.drawImage(161, 27, TopChance);
	p.drawRect(211, 27, 50, 84);
	p.drawRect(261, 27, 50, 84);
	p.drawImage(311, 27, TopRail);
	p.drawRect(361, 27, 50, 84);
	p.drawRect(411, 27, 50, 84);
	p.drawImage(461, 27, Water);
	p.drawRect(511, 27, 50, 84);
	p.drawImage(561, 27, GoToJail);

	//-------------------------------------------------------------------------------------------
	// Create bottom of outer ring
	//-------------------------------------------------------------------------------------------
	p.drawImage(27, 561, Jail);
	p.drawRect(111, 561, 50, 84);
	p.drawRect(161, 561, 50, 84);
	p.drawImage(211, 561, BottomChance);
	p.drawRect(261, 561, 50, 84);
	p.drawImage(311, 561, BottomRail);
	p.drawImage(361, 561, Income);
	p.drawRect(411, 561, 50, 84);
	p.drawImage(461, 561, BottomCommunity);
	p.drawRect(511, 561, 50, 84);
	p.drawImage(561, 561, GO);

	//-------------------------------------------------------------------------------------------
	// Create left side of outer ring
	//-------------------------------------------------------------------------------------------
	p.drawRect(27, 111, 84, 50);
	p.drawRect(27, 161, 84, 50);
	p.drawImage(27, 211, LeftCommunity);
	p.drawRect(27, 261, 84, 50);
	p.drawImage(27, 311, LeftRail);
	p.drawRect(27, 361, 84, 50);
	p.drawRect(27, 411, 84, 50);
	p.drawImage(27, 461, Elec);
	p.drawRect(27, 511, 84, 50);

	//-------------------------------------------------------------------------------------------
	// Create right side of outer ring
	//-------------------------------------------------------------------------------------------
	p.drawRect(561, 111, 84, 50);
	p.drawRect(561, 161, 84, 50);
	p.drawImage(561, 211, RightCommunity);
	p.drawRect(561, 261, 84, 50);
	p.drawImage(561, 311, RightRail);
	p.drawImage(561, 361, RightChance);
	p.drawRect(561, 411, 84, 50);
	p.drawImage(561, 461, Luxury);
	p.drawRect(561, 511, 84, 50);

	p.setBrush(white);
	//-------------------------------------------------------------------------------------------
	// Create top of inner ring
	//-------------------------------------------------------------------------------------------
	p.drawImage(127, 127, Free);
	p.drawImage(211, 127, Sales);
	p.drawRect(261, 127, 50, 84);
	p.drawImage(311, 127, TopRail);
	p.drawRect(361, 127, 50, 84);
	p.drawImage(411, 127, TopCommunity);
	p.drawImage(461, 127, Casino);

	//-------------------------------------------------------------------------------------------
	// Create bottom of inner ring
	//-------------------------------------------------------------------------------------------
	p.drawImage(127, 461, GoToJail2);
	p.drawRect(211, 461, 50, 84);
	p.drawRect(261, 461, 50, 84);
	p.drawImage(311, 461, BottomRail);
	p.drawRect(361, 461, 50, 84);
	p.drawImage(411, 461, Heat);
	p.drawImage(461, 461, GO);

	//-------------------------------------------------------------------------------------------
	// Create left side of inner ring
	//-------------------------------------------------------------------------------------------
	p.drawRect(127, 211, 84, 50);
	p.drawRect(127, 261, 84, 50);
	p.drawImage(127, 311, LeftRail);
	p.drawRect(127, 361, 84, 50);
	p.drawImage(127, 411, LeftChance);

	//-------------------------------------------------------------------------------------------
	// Create right side of inner ring
	//-------------------------------------------------------------------------------------------
	p.drawRect(461, 211, 84, 50);
	p.drawRect(461, 261, 84, 50);
	p.drawImage(461, 311, RightRail);
	p.drawRect(461, 361, 84, 50);
	p.drawRect(461, 411, 84, 50);

	//-------------------------------------------------------------------------------------------
	// Create railways between rings
	//-------------------------------------------------------------------------------------------
	p.setPen(black);
	p.drawLine(111, 331, 127, 331);
	p.drawLine(111, 341, 127, 341);
	p.drawLine(331, 111, 331, 127);
	p.drawLine(341, 111, 341, 127);
	p.drawLine(545, 331, 561, 331);
	p.drawLine(545, 341, 561, 341);
	p.drawLine(331, 545, 331, 561);
	p.drawLine(341, 545, 341, 561);
	p.drawLine(111, 116, 127, 132);
	p.drawLine(116, 111, 132, 127);
	p.drawLine(545, 540, 561, 556);
	p.drawLine(540, 545, 556, 561);

	p.setPen(black);
	//-------------------------------------------------------------------------------------------
	// Create property set colorings
	//-------------------------------------------------------------------------------------------
	p.setBrush(darkMagenta);
	p.drawRect(511, 561, 50, 21);
	p.drawRect(411, 561, 50, 21);

	p.setBrush(cyan);
	p.drawRect(111, 561, 50, 21);
	p.drawRect(161, 561, 50, 21);
	p.drawRect(261, 561, 50, 21);

	p.setBrush(magenta);
	p.drawRect(90, 511, 21, 50);
	p.drawRect(90, 361, 21, 50);
	p.drawRect(90, 411, 21, 50);

	p.setBrush(QColor(255, 153, 0));
	p.drawRect(90, 111, 21, 50);
	p.drawRect(90, 161, 21, 50);
	p.drawRect(90, 261, 21, 50);

	p.setBrush(red);
	p.drawRect(111, 90, 50, 21);
	p.drawRect(211, 90, 50, 21);
	p.drawRect(261, 90, 50, 21);

	p.setBrush(yellow);
	p.drawRect(361, 90, 50, 21);
	p.drawRect(411, 90, 50, 21);
	p.drawRect(511, 90, 50, 21);

	p.setBrush(darkGreen);
	p.drawRect(561, 111, 21, 50);
	p.drawRect(561, 161, 21, 50);
	p.drawRect(561, 261, 21, 50);

	p.setBrush(blue);
	p.drawRect(561, 411, 21, 50);
	p.drawRect(561, 511, 21, 50);

	p.setBrush(darkRed);
	p.drawRect(211, 461, 50, 21);
	p.drawRect(261, 461, 50, 21);
	p.drawRect(361, 461, 50, 21);

	p.setBrush(green);
	p.drawRect(190, 211, 21, 50);
	p.drawRect(190, 261, 21, 50);
	p.drawRect(190, 361, 21, 50);

	p.setBrush(darkBlue);
	p.drawRect(261, 190, 50, 21);
	p.drawRect(361, 190, 50, 21);

	p.setBrush(QColor(255, 0 ,102));
	p.drawRect(461, 211, 21, 50);
	p.drawRect(461, 261, 21, 50);

	p.setBrush(black);
	p.drawRect(461, 361, 21, 50);
	p.drawRect(461, 411, 21, 50);
} // }}}

void BoardView::paintEvent(QPaintEvent *) // {{{
{
	// double buffer
	QPixmap buffer( width(), height() );
	//buffer.fill ( lightGray );

	// draw onto second buffer
	QPainter p(&buffer);
	p.drawPixmap(0,0,_background);

	if(_gamestate != NULL) {

		// draw_upgrades(..., group id, upgrade level)
		const Game_Base & base = _gamestate->get_base();
		
		// iterate until the group id given by 'j' has
		// and "invalid" representative number
		for(int j = 1; ; ++j) {

			// this gives us a representative member for this particular group
			unsigned int rep = base.rep_member(j);
			if(rep == 0) break;

			// now we need to query gamestate to see what level that property is at
			const PropertyState & prop = _gamestate->get_property(rep);

			if( prop.has_owner() ) {
				draw_upgrades(p, j, prop.get_level() );
			}
		}


		// draw the ownership overlay
		{ 
			const net::GameState::PropertyStateList props = _gamestate->get_property_list();
			net::GameState::PropertyStateList::const_iterator it;

			for(it = props.begin(); it != props.end(); ++it) {
				// only deal with properties that are actually owned
				if( it->has_owner() ) {
					QColor color = tokens.get_color_of(it->get_owner());
					if(color.isValid()) {
						draw_prop_own_overlay(p, it->get_id(), &color);
					} else {
						std::cout << "color not valid for " << (int)it->get_owner() << std::endl;
					}
				}
			}
		}



		// draw the tokens
		{
			std::vector<token_animate>::const_iterator it;
			for(it = _anim.begin(); it != _anim.end(); ++it) {

				PlayerState * player = NULL;
				if(NULL == (player = _gamestate->get_player_ptr(it->tk_id))) continue;
				
				//const PlayerState & player = _gamestate->get_player(it->tk_id);
				bool jail = player->get_jail_status() != kFree;

				// animate the token?
				if(!it->anim) {
					tokens.draw_token(p, player->get_id(), player->get_position(), jail);
				} else {
					// else we animate, and draw up the cpos
					tokens.draw_token(p, player->get_id(), it->cpos, jail);
				}
				
			}
		}


/*
		{
			const net::GameState::PlayerStateList players = _gamestate->get_player_list();
			net::GameState::PlayerStateList::const_iterator it;

			for(it = players.begin(); it != players.end(); ++it) {
				bool jail = it->get_jail_status() != kFree;

				// okay so we have a player... draw the token?
				tokens.draw_token(p, it->get_id() + 1, it->get_position() + 1, jail);
			}

		}
*/

	}

		
	// Done drawing to the temporary buffer
	p.end();

	// Draw to the window
	p.begin(this);
	p.drawPixmap( 0, 0, buffer);
	p.end();
} // }}}

//-----------------------------------------------------------------------------------------------
// Listen for mouse clicks
//-----------------------------------------------------------------------------------------------
void BoardView::mousePressEvent(QMouseEvent *e)
{
	if(NULL == _gamestate) return;

	int prop = propID(e->x(), e->y());
	emit board_clicked(prop);
}

void BoardView::game_state_updated(net::GameState * state)
{
	if(state == NULL) return;

	_gamestate = state;

	if(!_timer->isActive()) {
		DEBUG("BoardView - Starting timer.");
		_timer->start(ANIMATE_TIME);
	}

	repaint(false);
}

void BoardView::draw_prop_own_overlay(QPainter &p , int prop, QColor * color) // {{{
{
	if(color == NULL || !color->isValid()) 	return;

	QBrush brush(color->light(), Qt::Dense5Pattern);
	p.setBrush(brush);
	

	//-------------------------------------------------------------------------------------------
	// Create top of outer ring
	//-------------------------------------------------------------------------------------------
	//p.drawImage(27, 27, Free);
	if(prop==21){
		p.drawRect(111, 27, 50, 63);
	}
	//p.drawImage(161, 27, TopChance);
	if(prop==23){
		p.drawRect(211, 27, 50, 63);
	}
	if(prop==24){
		p.drawRect(261, 27, 50, 63);
	}
	if(prop==25){
		p.drawRect(311, 27, 50, 84);
	}
	if(prop==26){
		p.drawRect(361, 27, 50, 63);
	}
	if(prop==27){
		p.drawRect(411, 27, 50, 63);
	}
	if(prop==28){
		p.drawRect(461, 27, 50, 84);
	}
	if(prop==29){
		p.drawRect(511, 27, 50, 63);
	}
	//p.drawImage(561, 27, GoToJail);

	//-------------------------------------------------------------------------------------------
	// Create bottom of outer ring
	//-------------------------------------------------------------------------------------------
	//p.drawImage(27, 561, Jail);
	if(prop==9){
		p.drawRect(111, 582, 50, 63);
	}
	if(prop==8){
		p.drawRect(161, 582, 50, 63);
	}
	//p.drawImage(211, 561, BottomChance);
	if(prop==6){
		p.drawRect(261, 582, 50, 63);
	}
	if(prop==5){
		p.drawRect(311, 561, 50, 84);
	}
	//p.drawImage(361, 561, Income);
	if(prop==3){
		p.drawRect(411, 582, 50, 63);
	}
	//p.drawImage(461, 561, BottomCommunity);
	if(prop==1){
		p.drawRect(511, 582, 50, 63);
	}
	//p.drawImage(561, 561, GO);

	//-------------------------------------------------------------------------------------------
	// Create left side of outer ring
	//-------------------------------------------------------------------------------------------
	if(prop==19){
		p.drawRect(27, 111, 63, 50);
	}
	if(prop==18){
		p.drawRect(27, 161, 63, 50);
	}
	//p.drawImage(27, 211, LeftCommunity);
	if(prop==16){
		p.drawRect(27, 261, 63, 50);
	}
	if(prop==15){
		p.drawRect(27, 311, 84, 50);
	}
	if(prop==14){
		p.drawRect(27, 361, 63, 50);
	}
	if(prop==13){
		p.drawRect(27, 411, 63, 50);
	}
	if(prop==12){
		p.drawRect(27, 461, 84, 50);
	}
	if(prop==11){
		p.drawRect(27, 511, 63, 50);
	}

	//-------------------------------------------------------------------------------------------
	// Create right side of outer ring
	//-------------------------------------------------------------------------------------------
	
	if(prop==31){
		p.drawRect(582, 111, 63, 50);
	}
	if(prop==32){
		p.drawRect(582, 161, 63, 50);
	}
	//p.drawImage(561, 211, RightCommunity);
	if(prop==34){
		p.drawRect(582, 261, 63, 50);
	}
	if(prop==35){
		p.drawRect(561, 311, 84, 50);
	}
	//p.drawImage(561, 361, RightChance);
	if(prop==37){
		p.drawRect(582, 411, 63, 50);
	}
	//p.drawImage(561, 461, Luxury);
	if(prop==39){
		p.drawRect(582, 511, 63, 50);
	}

	
	//-------------------------------------------------------------------------------------------
	// Create top of inner ring
	//-------------------------------------------------------------------------------------------
	//p.drawImage(127, 127, Free);
	//p.drawImage(211, 127, Sales);
	if(prop==54){
		p.drawRect(261, 127, 50, 63);
	}
	if(prop==55){
		p.drawRect(311, 127, 50, 84);
	}
	if(prop==56){
		p.drawRect(361, 127, 50, 63);
	}
	//p.drawImage(411, 127, TopCommunity);
	//p.drawImage(461, 127, Casino);

	//-------------------------------------------------------------------------------------------
	// Create bottom of inner ring
	//-------------------------------------------------------------------------------------------
	//p.drawImage(127, 461, GoToJail2);
	if(prop==45){
		p.drawRect(211, 482, 50, 63);
	}
	if(prop==44){
		p.drawRect(261, 482, 50, 63);
	}
	if(prop==43){
		p.drawRect(311, 461, 50, 84);
	}
	if(prop==42){
		p.drawRect(361, 482, 50, 63);
	}
	if(prop==41){
		p.drawRect(411, 461, 50, 84);
	}
	//p.drawImage(461, 461, GO);

	//-------------------------------------------------------------------------------------------
	// Create left side of inner ring
	//-------------------------------------------------------------------------------------------
	if(prop==51){
		p.drawRect(127, 211, 63, 50);
	}
	if(prop==50){
		p.drawRect(127, 261, 63, 50);
	}
	if(prop==49){
		p.drawRect(127, 311, 84, 50);
	}
	if(prop==48){
		p.drawRect(127, 361, 63, 50);
	}
	//p.drawImage(127, 411, LeftChance);

	//-------------------------------------------------------------------------------------------
	// Create right side of inner ring
	//-------------------------------------------------------------------------------------------
	if(prop==59){
		p.drawRect(482, 211, 63, 50);
	}
	if(prop==60){
		p.drawRect(482, 261, 63, 50);
	}
	if(prop==61){
		p.drawRect(461, 311, 84, 50);
	}
	if(prop==62){
		p.drawRect(482, 361, 63, 50);
	}
	if(prop==63){
		p.drawRect(482, 411, 63, 50);
	}

} // }}}

//-----------------------------------------------------------------------------------------------
// Returns value of space to put specified image into upper left corner of game window
//-----------------------------------------------------------------------------------------------
int BoardView::propID(int x, int y) // {{{
{
	if ((x >= 27 and x < 111) and (y >= 27 and y < 111)) {
			return 20;
	}
	if ((x >= 27 and x < 111) and (y >= 111 and y < 161)) {
			return 19;
	}
	if ((x >= 27 and x < 111) and (y >= 161 and y < 211)) {
			return 18;
	}
	if ((x >= 27 and x < 111) and (y >= 211 and y < 261)) {
			return 17;
	}
	if ((x >= 27 and x < 111) and (y >= 261 and y < 311)) {
			return 16;
	}
	if ((x >= 27 and x < 111) and (y >= 311 and y < 361)) {
			return 15;
	}
	if ((x >= 27 and x < 111) and (y >= 361 and y < 411)) {
			return 14;
	}
	if ((x >= 27 and x < 111) and (y >= 411 and y < 461)) {
			return 13;
	}
	if ((x >= 27 and x < 111) and (y >= 461 and y < 511)) {
			return 12;
	}
	if ((x >= 27 and x < 111) and (y >= 511 and y < 561)) {
			return 11;
	}
	if ((x >= 27 and x < 111) and (y >= 561 and y < 645)) {
			return 10;
	}
	if ((y >= 27 and y < 111) and (x >= 111 and x < 161)) {
			return 21;
	}
	if ((y >= 27 and y < 111) and (x >= 161 and x < 211)) {
			return 22;
	}
	if ((x >= 127 and x < 211) and (y >= 127 and y < 211)) {
			return 52;
	}
	if ((x >= 127 and x < 211) and (y >= 211 and y < 261)) {
			return 51;
	}
	if ((x >= 127 and x < 211) and (y >= 261 and y < 311)) {
			return 50;
	}
	if ((x >= 127 and x < 211) and (y >= 311 and y < 361)) {
			return 49;
	}
	if ((x >= 127 and x < 211) and (y >= 361 and y < 411)) {
			return 48;
	}
	if ((x >= 127 and x < 211) and (y >= 411 and y < 461)) {
			return 47;
	}
	if ((x >= 127 and x < 211) and (y >= 461 and y < 545)) {
			return 46;
	}
	if ((y >= 561 and y < 645) and (x >= 111 and x < 161)) {
			return 9;
	}
	if ((y >= 561 and y < 645) and (x >= 161 and x < 211)) {
			return 8;
	}
	if ((x >= 211 and x < 261) and (y >= 27 and y < 111)) {
			return 23;
	}
	if ((x >= 211 and x < 261) and (y >= 127 and y < 211)) {
			return 53;
	}
	if ((x >= 211 and x < 261) and (y >= 461 and y < 545)) {
			return 45;
	}
	if ((x >= 211 and x < 261) and (y >= 561 and y < 645)) {
			return 7;
	}
	if ((x >= 261 and x < 311) and (y >= 27 and y < 111)) {
			return 24;
	}
	if ((x >= 261 and x < 311) and (y >= 127 and y < 211)) {
			return 54;
	}
	if ((x >= 261 and x < 311) and (y >= 461 and y < 545)) {
			return 44;
	}
	if ((x >= 261 and x < 311) and (y >= 561 and y < 645)) {
			return 6;
	}
	if ((x >= 311 and x < 361) and (y >= 27 and y < 111)) {
			return 25;
	}
	if ((x >= 311 and x < 361) and (y >= 127 and y < 211)) {
			return 55;
	}
	if ((x >= 311 and x < 361) and (y >= 461 and y < 545)) {
			return 43;
	}
	if ((x >= 311 and x < 361) and (y >= 561 and y < 645)) {
			return 5;
	}
	if ((x >= 361 and x < 411) and (y >= 27 and y < 111)) {
			return 26;
	}
	if ((x >= 361 and x < 411) and (y >= 127 and y < 211)) {
			return 56;
	}
	if ((x >= 361 and x < 411) and (y >= 461 and y < 545)) {
			return 42;
	}
	if ((x >= 361 and x < 411) and (y >= 561 and y < 645)) {
			return 4;
	}
	if ((x >= 411 and x < 461) and (y >= 27 and y < 111)) {
			return 27;
	}
	if ((x >= 411 and x < 461) and (y >= 127 and y < 211)) {
			return 57;
	}
	if ((x >= 411 and x < 461) and (y >= 461 and y < 545)) {
			return 41;
	}
	if ((x >= 411 and x < 461) and (y >= 561 and y < 645)) {
			return 3;
	}
	if ((y >= 27 and y < 111) and (x >= 461 and x < 511)) {
			return 28;
	}
	if ((y >= 27 and y < 111) and (x >= 511 and x < 561)) {
			return 29;
	}
	if ((x >= 461 and x < 545) and (y >= 127 and y < 211)) {
			return 58;
	}
	if ((x >= 461 and x < 545) and (y >= 211 and y < 261)) {
			return 59;
	}
	if ((x >= 461 and x < 545) and (y >= 261 and y < 311)) {
			return 60;
	}
	if ((x >= 461 and x < 545) and (y >= 311 and y < 361)) {
			return 61;
	}
	if ((x >= 461 and x < 545) and (y >= 361 and y < 411)) {
			return 62;
	}
	if ((x >= 461 and x < 545) and (y >= 411 and y < 461)) {
			return 63;
	}
	if ((x >= 461 and x < 545) and (y >= 461 and y < 545)) {
			return 40;
	}
	if ((y >= 561 and y < 645) and (x >= 461 and x < 511)) {
			return 2;
	}
	if ((y >= 561 and y < 645) and (x >= 511 and x < 561)) {
			return 1;
	}
	if ((x >= 561 and x < 645) and (y >= 27 and y < 111)) {
			return 30;
	}
	if ((x >= 561 and x < 645) and (y >= 111 and y < 161)) {
			return 31;
	}
	if ((x >= 561 and x < 645) and (y >= 161 and y < 211)) {
			return 32;
	}
	if ((x >= 561 and x < 645) and (y >= 211 and y < 261)) {
			return 33;
	}
	if ((x >= 561 and x < 645) and (y >= 261 and y < 311)) {
			return 34;
	}
	if ((x >= 561 and x < 645) and (y >= 311 and y < 361)) {
			return 35;
	}
	if ((x >= 561 and x < 645) and (y >= 361 and y < 411)) {
			return 36;
	}
	if ((x >= 561 and x < 645) and (y >= 411 and y < 461)) {
			return 37;
	}
	if ((x >= 561 and x < 645) and (y >= 461 and y < 511)) {
			return 38;
	}
	if ((x >= 561 and x < 645) and (y >= 511 and y < 561)) {
			return 39;
	}
	if ((x >= 561 and x < 645) and (y >= 561 and y < 645)) {
			return 0;
	}
	return 99;
} // }}}

void BoardView::draw_upgrades(QPainter &u, int group, int level) // {{{
{
	if(group == 0 || level == 0) return;

	if(group==1){

		if(level>=1 && level < 5){
			u.drawImage(513, 563,*House);
			u.drawImage(413, 563,*House);
		}
		if(level>=2 && level < 5){
			u.drawImage(525, 563,*House);
			u.drawImage(425, 563,*House);
		}
		if(level>=3 && level < 5){
			u.drawImage(537, 563,*House);
			u.drawImage(437, 563,*House);
		}
		if(level==4){
			u.drawImage(549, 563,*House);
			u.drawImage(449, 563,*House);
		}
		if(level>=5){
			u.drawImage(513, 563,*Hotel);
			u.drawImage(413, 563,*Hotel);
		}
		if(level==6){
			u.drawImage(537, 563,*Hotel);
			u.drawImage(437, 563,*Hotel);
		}
		return;
	}

	//group=2
	if(group==3){

		if(level>=1 && level < 5){
			u.drawImage(113, 563,*House);
			u.drawImage(163, 563,*House);
			u.drawImage(263, 563,*House);
		}
		if(level>=2 && level < 5){
			u.drawImage(125, 563,*House);
			u.drawImage(175, 563,*House);
			u.drawImage(275, 563,*House);
		}
		if(level>=3 && level < 5){
			u.drawImage(137, 563,*House);
			u.drawImage(187, 563,*House);
			u.drawImage(287, 563,*House);
		}
		if(level==4){
			u.drawImage(149, 563,*House);
			u.drawImage(199, 563,*House);
			u.drawImage(299, 563,*House);
		}
		if(level>=5){
			u.drawImage(113, 563,*Hotel);
			u.drawImage(163, 563,*Hotel);
			u.drawImage(263, 563,*Hotel);
		}
		if(level==6){
			u.drawImage(137, 563,*Hotel);
			u.drawImage(187, 563,*Hotel);
			u.drawImage(287, 563,*Hotel);
		}
		return;
	}


	if(group==4){

		if(level>=1 && level < 5){
			u.drawImage(92, 513,*HouseS);
			u.drawImage(92, 413,*HouseS);
			u.drawImage(92, 363,*HouseS);
		}
		if(level>=2 && level < 5){
			u.drawImage(92, 525,*HouseS);
			u.drawImage(92, 425,*HouseS);
			u.drawImage(92, 375,*HouseS);
		}
		if(level>=3 && level < 5){
			u.drawImage(92, 537,*HouseS);
			u.drawImage(92, 437,*HouseS);
			u.drawImage(92, 387,*HouseS);
		}
		if(level==4){
			u.drawImage(92, 549,*HouseS);
			u.drawImage(92, 449,*HouseS);
			u.drawImage(92, 399,*HouseS);
		}
		if(level>=5){
			u.drawImage(92, 513,*HotelS);
			u.drawImage(92, 413,*HotelS);
			u.drawImage(92, 363,*HotelS);
		}
		if(level==6){
			u.drawImage(92, 537,*HotelS);
			u.drawImage(92, 437,*HotelS);
			u.drawImage(92, 387,*HotelS);
		}
		return;
	}


	//group=5
	if(group==6){

		if(level>=1 && level < 5){
			u.drawImage(92, 113,*HouseS);
			u.drawImage(92, 163,*HouseS);
			u.drawImage(92, 263,*HouseS);
		}
		if(level>=2 && level < 5){
			u.drawImage(92, 125,*HouseS);
			u.drawImage(92, 175,*HouseS);
			u.drawImage(92, 275,*HouseS);
		}
		if(level>=3 && level < 5){
			u.drawImage(92, 137,*HouseS);
			u.drawImage(92, 187,*HouseS);
			u.drawImage(92, 287,*HouseS);
		}
		if(level==4){
			u.drawImage(92, 149,*HouseS);
			u.drawImage(92, 199,*HouseS);
			u.drawImage(92, 299,*HouseS);
		}
		if(level>=5){
			u.drawImage(92, 113,*HotelS);
			u.drawImage(92, 163,*HotelS);
			u.drawImage(92, 263,*HotelS);
		}
		if(level==6){
			u.drawImage(92, 137,*HotelS);
			u.drawImage(92, 187,*HotelS);
			u.drawImage(92, 287,*HotelS);
		}
		return;
	}


	if(group==7){

		if(level>=1 && level < 5){
			u.drawImage(113, 92,*House);
			u.drawImage(213, 92,*House);
			u.drawImage(263, 92,*House);
		}
		if(level>=2 && level < 5){
			u.drawImage(125, 92,*House);
			u.drawImage(225, 92,*House);
			u.drawImage(275, 92,*House);
		}
		if(level>=3 && level < 5){
			u.drawImage(137, 92,*House);
			u.drawImage(237, 92,*House);
			u.drawImage(287, 92,*House);
		}
		if(level==4){
			u.drawImage(149, 92,*House);
			u.drawImage(249, 92,*House);
			u.drawImage(299, 92,*House);
		}
		if(level>=5){
			u.drawImage(113, 92,*Hotel);
			u.drawImage(213, 92,*Hotel);
			u.drawImage(263, 92,*Hotel);
		}
		if(level==6){
			u.drawImage(137, 92,*Hotel);
			u.drawImage(237, 92,*Hotel);
			u.drawImage(287, 92,*Hotel);
		}
		return;
	}


	if(group==8){

		if(level>=1 && level < 5){
			u.drawImage(363, 92,*House);
			u.drawImage(413, 92,*House);
			u.drawImage(513, 92,*House);
		}
		if(level>=2 && level < 5){
			u.drawImage(375, 92,*House);
			u.drawImage(425, 92,*House);
			u.drawImage(525, 92,*House);
		}
		if(level>=3 && level < 5){
			u.drawImage(387, 92,*House);
			u.drawImage(437, 92,*House);
			u.drawImage(537, 92,*House);
		}
		if(level==4){
			u.drawImage(399, 92,*House);
			u.drawImage(449, 92,*House);
			u.drawImage(549, 92,*House);
		}
		if(level>=5){
			u.drawImage(363, 92,*Hotel);
			u.drawImage(413, 92,*Hotel);
			u.drawImage(513, 92,*Hotel);
		}
		if(level==6){
			u.drawImage(387, 92,*Hotel);
			u.drawImage(437, 92,*Hotel);
			u.drawImage(537, 92,*Hotel);
		}
		return;
	}


	if(group==9){

		if(level>=1 && level < 5){
			u.drawImage(563, 113,*HouseS);
			u.drawImage(563, 163,*HouseS);
			u.drawImage(563, 263,*HouseS);
		}
		if(level>=2 && level < 5){
			u.drawImage(563, 125,*HouseS);
			u.drawImage(563, 175,*HouseS);
			u.drawImage(563, 275,*HouseS);
		}
		if(level>=3 && level < 5){
			u.drawImage(563, 137,*HouseS);
			u.drawImage(563, 187,*HouseS);
			u.drawImage(563, 287,*HouseS);
		}
		if(level==4){
			u.drawImage(563, 149,*HouseS);
			u.drawImage(563, 199,*HouseS);
			u.drawImage(563, 299,*HouseS);
		}
		if(level>=5){
			u.drawImage(563, 113,*HotelS);
			u.drawImage(563, 163,*HotelS);
			u.drawImage(563, 263,*HotelS);
		}
		if(level==6){
			u.drawImage(563, 137,*HotelS);
			u.drawImage(563, 187,*HotelS);
			u.drawImage(563, 287,*HotelS);
		}
		return;
	}


	if(group==10){

		if(level>=1 && level < 5){
			u.drawImage(563, 413,*HouseS);
			u.drawImage(563, 513,*HouseS);
		}
		if(level>=2 && level < 5){
			u.drawImage(563, 425,*HouseS);
			u.drawImage(563, 525,*HouseS);
		}
		if(level>=3 && level < 5){
			u.drawImage(563, 437,*HouseS);
			u.drawImage(563, 537,*HouseS);
		}
		if(level==4){
			u.drawImage(563, 449,*HouseS);
			u.drawImage(563, 549,*HouseS);
		}
		if(level>=5){
			u.drawImage(563, 413,*HotelS);
			u.drawImage(563, 513,*HotelS);
		}
		if(level==6){
			u.drawImage(563, 437,*HotelS);
			u.drawImage(563, 537,*HotelS);
		}
		return;
	}


	if(group==11){

		if(level>=1 && level < 5){
			u.drawImage(213, 463,*House);
			u.drawImage(263, 463,*House);
			u.drawImage(363, 463,*House);
		}
		if(level>=2 && level < 5){
			u.drawImage(225, 463,*House);
			u.drawImage(275, 463,*House);
			u.drawImage(375, 463,*House);
		}
		if(level>=3 && level < 5){
			u.drawImage(237, 463,*House);
			u.drawImage(287, 463,*House);
			u.drawImage(387, 463,*House);
		}
		if(level==4){
			u.drawImage(249, 463,*House);
			u.drawImage(299, 463,*House);
			u.drawImage(399, 463,*House);
		}
		if(level>=5){
			u.drawImage(213, 463,*Hotel);
			u.drawImage(263, 463,*Hotel);
			u.drawImage(363, 463,*Hotel);
		}
		if(level==6){
			u.drawImage(237, 463,*Hotel);
			u.drawImage(287, 463,*Hotel);
			u.drawImage(387, 463,*Hotel);
		}
		return;
	}


	if(group==12){

		if(level>=1 && level < 5){
			u.drawImage(192, 213,*HouseS);
			u.drawImage(192, 263,*HouseS);
			u.drawImage(192, 363,*HouseS);
		}
		if(level>=2 && level < 5){
			u.drawImage(192, 225,*HouseS);
			u.drawImage(192, 275,*HouseS);
			u.drawImage(192, 375,*HouseS);
		}
		if(level>=3 && level < 5){
			u.drawImage(192, 237,*HouseS);
			u.drawImage(192, 287,*HouseS);
			u.drawImage(192, 387,*HouseS);
		}
		if(level==4){
			u.drawImage(192, 249,*HouseS);
			u.drawImage(192, 299,*HouseS);
			u.drawImage(192, 399,*HouseS);
		}
		if(level>=5){
			u.drawImage(192, 213,*HotelS);
			u.drawImage(192, 263,*HotelS);
			u.drawImage(192, 363,*HotelS);
		}
		if(level==6){
			u.drawImage(192, 237,*HotelS);
			u.drawImage(192, 287,*HotelS);
			u.drawImage(192, 387,*HotelS);
		}
		return;
	}


	if(group==13){

		if(level>=1 && level < 5){
			u.drawImage(263, 192,*House);
			u.drawImage(363, 192,*House);
		}
		if(level>=2 && level < 5){
			u.drawImage(275, 192,*House);
			u.drawImage(375, 192,*House);
		}
		if(level>=3 && level < 5){
			u.drawImage(287, 192,*House);
			u.drawImage(387, 192,*House);
		}
		if(level==4){
			u.drawImage(299, 192,*House);
			u.drawImage(399, 192,*House);
		}
		if(level>=5){
			u.drawImage(263, 192,*Hotel);
			u.drawImage(363, 192,*Hotel);
		}
		if(level==6){
			u.drawImage(287, 192,*Hotel);
			u.drawImage(387, 192,*Hotel);
		}
		return;
	}


	if(group==14){

		if(level>=1 && level < 5){
			u.drawImage(463, 213,*HouseS);
			u.drawImage(463, 263,*HouseS);
		}
		if(level>=2 && level < 5){
			u.drawImage(463, 225,*HouseS);
			u.drawImage(463, 275,*HouseS);
		}
		if(level>=3 && level < 5){
			u.drawImage(463, 237,*HouseS);
			u.drawImage(463, 287,*HouseS);
		}
		if(level==4){
			u.drawImage(463, 249,*HouseS);
			u.drawImage(463, 299,*HouseS);
		}
		if(level>=5){
			u.drawImage(463, 213,*HotelS);
			u.drawImage(463, 263,*HotelS);
		}
		if(level==6){
			u.drawImage(463, 237,*HotelS);
			u.drawImage(463, 287,*HotelS);
		}
		return;
	}


	if(group==15){

		if(level>=1 && level < 5){
			u.drawImage(463, 363,*HouseS);
			u.drawImage(463, 413,*HouseS);
		}
		if(level>=2 && level < 5){
			u.drawImage(463, 375,*HouseS);
			u.drawImage(463, 425,*HouseS);
		}
		if(level>=3 && level < 5){
			u.drawImage(463, 387,*HouseS);
			u.drawImage(463, 437,*HouseS);
		}
		if(level==4){
			u.drawImage(463, 399,*HouseS);
			u.drawImage(463, 449,*HouseS);
		}
		if(level>=5){
			u.drawImage(463, 363,*HotelS);
			u.drawImage(463, 413,*HotelS);
		}
		if(level==6){
			u.drawImage(463, 387,*HotelS);
			u.drawImage(463, 437,*HotelS);
		}
		return;
	}
		
} // }}}
