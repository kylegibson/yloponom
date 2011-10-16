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

#include "TradeRequestDialog.h"
#include "network/trade_packet.h"
#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include "network/GameState.h"
#include "core/Game_Base.h"
#include <qscrollview.h>
#include <qvbox.h>
#include <qcheckbox.h>
#include <qspinbox.h>


/////////////////////////////////////////////////////////////////////////////////////////////////
// Trade dialog class
/////////////////////////////////////////////////////////////////////////////////////////////////
TradeRequestDialog::TradeRequestDialog(net::GameState *gamestate, net::Trade *trade,QWidget *parent, const char *name) : QDialog(parent, name, TRUE)
{
	const PlayerState & them = gamestate->get_player(trade->get_destin_player());

	setCaption(QString("Trade Request from ") + them.get_name());

//-----------------------------------------------------------------------------------------------
// Set up acceptance and rejection buttons
//-----------------------------------------------------------------------------------------------
		QPushButton *ok, *cancel;

		ok = new QPushButton ("Accept", this);
		ok->setGeometry( 0, 260, 208, 30);
		connect( ok, SIGNAL(clicked()), SLOT(accept()));

		cancel = new QPushButton( "Reject", this);
		cancel->setGeometry( 218, 260, 208, 30);
		connect(cancel, SIGNAL(clicked()), SLOT(reject()));

//-----------------------------------------------------------------------------------------------
// Set up frames for trading players
//-----------------------------------------------------------------------------------------------
		QLabel *p1frameLbl = new QLabel( "Stuff offered (to you)", this);
		p1frameLbl->setGeometry(0, 0, 208, 15);
		p1frameLbl->setAlignment(AlignHCenter);

		QLabel *frame = new QLabel( "", this);
		frame->setFrameStyle( QFrame::Box | QFrame::Raised );
		frame->setLineWidth(2);
		frame->setGeometry(0, 20, 208, 235);

		QScrollView * scrolling = new QScrollView(frame, "scrolling");
		scrolling->setGeometry(4, 4, 200, 190);
    QVBox * big_box = new QVBox(scrolling->viewport());
    scrolling->addChild(big_box);

		QLabel *p2frameLbl = new QLabel( "Stuff they want (from you)", this);
		p2frameLbl->setGeometry(218, 0, 208, 15);
		p2frameLbl->setAlignment(AlignHCenter);

		QLabel *frame2 = new QLabel( "", this);
		frame2->setFrameStyle( QFrame::Box | QFrame::Raised );
		frame2->setLineWidth(2);
		frame2->setGeometry(218, 20, 208, 235);

		QScrollView * scrolling2 = new QScrollView(frame2, "scrolling2");
		scrolling2->setGeometry(4, 4, 200, 190);
    QVBox * big_box2 = new QVBox(scrolling2->viewport());
    scrolling2->addChild(big_box2);

//-----------------------------------------------------------------------------------------------
// Set up property check boxes for trading players
//-----------------------------------------------------------------------------------------------


		const net::PropertyList_t & plist = trade->get_offered_properties();
		net::PropertyList_t::const_iterator start = plist.begin();
		net::PropertyList_t::const_iterator end = plist.end();

		const Game_Base & base = gamestate->get_base();

		int i = 0;
		while(start != end) {
			net::Property_t id = *(start++);
			// id is an integer identification for the property
			// query gamebase to find out the name, etc. for this property

			if(id == 255 || id == 254) {
				properties[i] = new QLabel( QString("Get out of Jail card - ") + QString::number(id - 253) , big_box);
				properties[i]->setFixedWidth(180);
				++i;
			} else {
				const std::string* name = base.name(id);
				if(name != NULL) {
					properties[i] = new QLabel( *name , big_box);
					properties[i]->setFixedWidth(180);
					++i;
				}
			}
		}

		const net::PropertyList_t & tlist = trade->get_requested_properties();
		start = tlist.begin();
		end = tlist.end();

		i = 0;
		while(start != end) {
			net::Property_t id = *(start++);
			// id is an integer identification for the property
			// query gamebase to find out the name, etc. for this property

			if(id == 255 || id == 254) {
				theirProps[i] = new QLabel( QString("Get out of Jail card - ") + QString::number(id - 253) , big_box2);
				theirProps[i]->setFixedWidth(180);
				++i;
			} else {
				const std::string* name = base.name(id);
				if(name != NULL) {
					theirProps[i] = new QLabel( *name , big_box2);
					theirProps[i]->setFixedWidth(180);
					++i;
				}
			}


		}

//-----------------------------------------------------------------------------------------------
// Set up out-of-jail-free card check boxes and money field for first player
//-----------------------------------------------------------------------------------------------
		
		QLabel *p1avail$;
		QLabel *p1$;
		

	
		p1$ = new QLabel ( "Money:", frame);
		p1$->setGeometry(30, 200, 65, 30);
		p1avail$ = new QLabel (QString::number(trade->get_offered_money()), frame);
		p1avail$->setGeometry(130, 200, 65, 30);

		

//-----------------------------------------------------------------------------------------------
// Set up out-of-jail-free card check boxes and money field for second player
//-----------------------------------------------------------------------------------------------
		
		QLabel *p2avail$;
		QLabel *p2$;
		

		

		p2$ = new QLabel ( "Money:", frame2);
		p2$->setGeometry(30, 200, 65, 30);
		p2avail$ = new QLabel (QString::number(trade->get_request_money()), frame2);
		p2avail$->setGeometry(130, 200, 65, 30);

		
}
