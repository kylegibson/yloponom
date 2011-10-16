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

#include "PlayerStats.h"

#include "core/Game_Base.h"
#include "network/GameState.h"
#include "core/PlayerState.h"

#include <qlabel.h>
#include <qevent.h>
#include <qlistview.h>
#include <qpixmap.h>
#include <qpalette.h>

#include <iostream>

/////////////////////////////////////////////////////////////////////////////////////////////////
// PlayerStats class
/////////////////////////////////////////////////////////////////////////////////////////////////
PlayerStats::PlayerStats( QWidget *parent, const char *name ) 
: QWidget( parent, name ), _gamestate(NULL)
{
	setFixedSize(190, 200);

	_list = new QListView(this, "_list");
	_list->setFixedSize( size() );
	_list->setSorting(-1);
	_list->setShowToolTips(true);
	_list->setSelectionMode(QListView::Single);
	_list->addColumn("Name");
	_list->addColumn("$$$", 40);
	_list->addColumn("Position");

	_list->setPaletteForegroundColor(black);
	_list->setPaletteBackgroundColor(lightGray);
	_list->setAllColumnsShowFocus(true);

	QPalette pal = _list->palette();
	pal.setColor( QColorGroup::Highlight, darkGray );
	pal.setColor( QColorGroup::HighlightedText, black );
	_list->setPalette(pal);

	_list->setFocusPolicy(QWidget::NoFocus);

}

void PlayerStats::game_state_updated(net::GameState * state)
{
	_gamestate = state;

	QListViewItem * selected = _list->selectedItem();

	QString cur = (selected != NULL) ? selected->text(0) : "";

	_list->clear();

	// basically we will just loop through 8 ids and try to set them
	net::GameState::PlayerStateList players = _gamestate->get_player_list();
	net::GameState::PlayerStateList::const_iterator it;

	for(it = players.begin(); it != players.end(); ++it) {
		const PlayerState & player = *it;
		QString money = QString::number(player.get_money());

		QString pos;
		const std::string * pos_name = _gamestate->get_base().name( player.get_position() );
		if(pos_name != NULL) {
			pos = *pos_name;

			// 
			if(Game_Base::kJail == _gamestate->get_base().type( player.get_position() ) ) {
				if(player.get_jail_status() == kFree) {
					pos += " (Just visiting)";
				}
			}
		}

		QListViewItem *v = new QListViewItem(_list, player.get_name(), money, pos );
		if(cur == player.get_name()) { 
			selected = v;
		}

		QImage * itoken = _tokens.get_token(player.get_id());
		if(itoken != NULL) {
			QPixmap token = *itoken;
			v->setPixmap(0, token);
		}

	}

	_list->setSelected( selected, true );
	_list->triggerUpdate();
	
	repaint();
}

QString * PlayerStats::get_selected_player()
{
	QListViewItem * selected = _list->selectedItem();
	if(NULL == selected) return NULL;

	static QString name;

	name = selected->text(0);
	return &name;
}



