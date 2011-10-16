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

#include "CasinoDialog.h"
#include "core/PlayerState.h"
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qradiobutton.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qbuttongroup.h>

/////////////////////////////////////////////////////////////////////////////////////////////////
// Trade dialog class
/////////////////////////////////////////////////////////////////////////////////////////////////
CasinoDialog::CasinoDialog(const PlayerState* player, QWidget *parent, const char * name)
: QDialog(parent, name, true)
{

	unsigned int money = player->get_money();

	setCaption("Welcome to the Casino.");

//-----------------------------------------------------------------------------------------------
// Set up acceptance and rejection buttons
//-----------------------------------------------------------------------------------------------
		
		QPushButton *ok = new QPushButton ("Place your bet", this);
		ok->setGeometry( 110, 300, 215, 30);
		connect( ok, SIGNAL(clicked()), SLOT(accept()));

		
//-----------------------------------------------------------------------------------------------
// Set up frames for trading players
//-----------------------------------------------------------------------------------------------
		QLabel *avail = new QLabel ( QString("Available Money:  $") + QString::number(money), this);
		avail->setGeometry( 130, 170, 205, 30);
		
		QLabel *betLabel = new QLabel ( "Place Your Bet:", this);
		betLabel->setGeometry( 130, 200, 205, 30);

		_spin = new QSpinBox( (money < 100 ? money : 100) , money, 100, this, "_spin");
		_spin->setGeometry(230, 205, 50, 20);

//-----------------------------------------------------------------------------------------------
// Set up property check boxes for first player
//-----------------------------------------------------------------------------------------------
		QRadioButton *Prop1, *Prop2, *Prop3, *Prop4, *Prop5;

		Prop1 = new QRadioButton( "Any Craps: wins on a throw of 2, 3 or 12 with a payoff of 8:1", this);
		Prop1->setGeometry( 5, 0, 400, 30);

		Prop2 = new QRadioButton( "Any Seven: wins on a throw of 7 with a payoff of 5:1", this);
		Prop2->setGeometry( 5, 25, 400, 30);

		Prop3 = new QRadioButton( "Eleven: wins on a throw of 11 with a payoff of 16:1", this);
		Prop3->setGeometry( 5, 50, 400, 30);

		Prop4 = new QRadioButton( "Ace Duece: wins on a throw of 3 with a payoff of 16:1", this);
		Prop4->setGeometry( 5, 75, 400, 30);

		Prop5 = new QRadioButton( "Aces or Boxcars: wins on a throw of 2 or 12 with a payoff of 30:1", this);
		Prop5->setGeometry( 5, 100, 410, 30);

		_group = new QButtonGroup(this);
		_group->setGeometry(0, 0, 0, 0);
		
		// type = kCCraps, kCSeven, kCEleven, kCDuece, kCAorB
		_group->insert(Prop1,0); // any craps
		_group->insert(Prop2,1); // any seven
		_group->insert(Prop3,2); // eleven
		_group->insert(Prop4,3); // ace duece
		_group->insert(Prop5,4); // aces or boxcars

		setFixedSize(420,335);
}


int CasinoDialog::get_bet_type()
{
	return _group->selectedId();
}

int CasinoDialog::get_wager()
{
	QString t = _spin->text();
	return t.toUInt();
}



