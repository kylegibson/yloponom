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
 
#include "Limbo.h"

#include "network/GameState.h"

#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <iostream>
	
Limbo::Limbo(QWidget *parent, const char *name )
: QWidget( parent, name )
{
	setFixedSize(200,200);
	QVBoxLayout * layout = new QVBoxLayout(this, 5, 5);

	_msg = new QLabel("Waiting for players to join...", this);
	_msg->setAlignment(Qt::AlignCenter);
	layout->addWidget(_msg);

	_start = new QPushButton("Start Game Now!", this);
	_start->setHidden(true);
	_start->setEnabled(false);
	_start->setFixedWidth(150);
	layout->addWidget(_start, 0, Qt::AlignHCenter);

	layout->addSpacing(50);
	
}

void Limbo::enable_first_player()
{
	// unhide the start button :)
	_start->setHidden(false);
	//_start->setEnabled(true);

	// setup connection
	connect(_start, SIGNAL(clicked()), this, SLOT(start_pushed()));
}

void Limbo::start_pushed()
{
	emit start_the_game_now();
}

void Limbo::game_state_updated(net::GameState * state)
{
	if(state->get_player_count() == 0) {
		_msg->setText("Waiting for players to join...");
	} else {
		if( state->get_player_count() == 1 ) {
			_start->setEnabled(false);
		} else {
			_start->setEnabled(true);
		}
		_msg->setText(QString("There are ") + QString::number(state->get_player_count()) + QString(" players connected.") );
	}
}

