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

#include "TradeDialog.h"

#include "network/GameState.h"
#include "core/Game_Base.h"


#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qhbox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qlistbox.h>


/////////////////////////////////////////////////////////////////////////////////////////////////
// Trade dialog class
/////////////////////////////////////////////////////////////////////////////////////////////////
TradeDialog::TradeDialog(net::GameState *gamestate, const PlayerState* me, const PlayerState* other, QWidget *parent, const char * name) : QDialog(parent, name, TRUE)
{
	setCaption("Make a trade!");

	QGridLayout *grid = new QGridLayout(this, 4, 2, 5, 10, "grid");

	const Game_Base & base = gamestate->get_base();

	QLabel * lbl = NULL;

	// titles

	lbl = new QLabel("Your inventory", this);
	grid->addWidget( lbl, 0, 0, Qt::AlignCenter );

	lbl = new QLabel( other->get_name() + QString("'s inventory") , this);
	grid->addWidget( lbl , 0, 1, Qt::AlignCenter );

	// buttons

	QPushButton *okbtn = new QPushButton ("Request Trade", this);
	connect( okbtn, SIGNAL(clicked()), SLOT(accept()));
	grid->addWidget( okbtn, 3, 0, Qt::AlignCenter );

	QPushButton *cancelbtn = new QPushButton( "Cancel", this);
	connect(cancelbtn, SIGNAL(clicked()), SLOT(reject()));
	grid->addWidget( cancelbtn, 3, 1, Qt::AlignCenter );

	// money spinboxes

	QHBox * lhbox = new QHBox(this);
	lhbox->setSpacing(5);
	new QLabel("Money:", lhbox);
	_my_spin = new QSpinBox( 0, me->get_money(), 1, lhbox);
	grid->addWidget(lhbox, 2, 0, Qt::AlignCenter );
	
	QHBox * rhbox = new QHBox(this);
	rhbox->setSpacing(5);
	new QLabel("Money:", rhbox);
	_their_spin = new QSpinBox( 0, other->get_money(), 1, rhbox);
	grid->addWidget(rhbox, 2, 1, Qt::AlignCenter );


	net::PropertyList_t::const_iterator it;



	// current player's inventory

	QListBox* llbox = _my_list = new QListBox(this);
	llbox->setSelectionMode( QListBox::Multi );
	llbox->setMinimumHeight(100);

	const net::PropertyList_t & props = me->get_owned_properties();
	for(it = props.begin(); it != props.end(); ++it) {
		net::Property_t id = *it;
		if(id == 255 || id == 254) {
			_my_trans.push_back(id);
			llbox->insertItem( QString("Get out of Jail card - ") + QString::number(id - 253) );
		} else {
			const std::string* name = base.name(id);
			if(name != NULL) {
				_my_trans.push_back(id);
				llbox->insertItem( *name );
			}
		}
	}

	grid->addWidget(llbox, 1, 0, Qt::AlignHCenter | Qt::AlignTop );


	QListBox* rlbox = _their_list = new QListBox(this);
	rlbox->setSelectionMode( QListBox::Multi );
	rlbox->setMinimumHeight(100);

	const net::PropertyList_t tprops = other->get_owned_properties();
	for(it = tprops.begin(); it != tprops.end(); ++it) {
		net::Property_t id = *it;
		if(id == 255 || id == 254) {
			_their_trans.push_back(id);
			rlbox->insertItem( QString("Get out of Jail card - ") + QString::number(id - 253) );
		} else {
			const std::string* name = base.name(id);
			if(name != NULL) {
				_their_trans.push_back(id);
				rlbox->insertItem( *name );
			}
		}
	}


	grid->addWidget(rlbox, 1, 1, Qt::AlignHCenter | Qt::AlignTop );

}

unsigned TradeDialog::get_offer_money()
{
	return _my_spin->text().toUInt();
}

unsigned TradeDialog::get_request_money()
{
	return _their_spin->text().toUInt();
}

unsigned TradeDialog::get_offer_props(std::vector<unsigned> & list)
{
	for(unsigned k = 0; k < _my_list->count(); ++k ) {
		if(_my_list->isSelected(k)) {
			//std::cout << _my_trans[k] << std::endl;
			list.push_back( _my_trans[k] );
		}
	}
	//std::cout << "---" << std::endl;
	return list.size();
}

unsigned TradeDialog::get_request_props(std::vector<unsigned> & list)
{
	for(unsigned k = 0; k < _their_list->count(); ++k ) {
		if(_their_list->isSelected(k)) {
			//std::cout << _their_trans[k] << std::endl;
			list.push_back( _their_trans[k] );
		}
	}
	//std::cout << "---" << std::endl;
	return list.size();
}

