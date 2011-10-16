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

#include "ImproveDialog.h"
#include "network/GameState.h"
#include "core/Game_Base.h"
#include "core/PlayerState.h"
#include "core/PropertyState.h"
#include <qstring.h>

#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <qlayout.h>

#include <iostream>
#include <vector>
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////////////////////////
// Improvements dialog box
/////////////////////////////////////////////////////////////////////////////////////////////////
ImproveDialog::ImproveDialog(net::GameState *state, const PlayerState *current, QWidget *parent, const char * name) 
: QDialog(parent, name, true), _gamestate(state)
{
	setCaption("Building Improvements");

	_grid = new QGridLayout(this, 4, 2, 5, 5, "grid");

	QPushButton *okbtn = new QPushButton("Improve!", this);
	_grid->addWidget(okbtn, 3, 0, Qt::AlignLeft );
	connect( okbtn, SIGNAL(clicked()), this, SLOT(accept()));

	QPushButton *cancelbtn = new QPushButton("Cancel", this);
	_grid->addWidget(cancelbtn, 3, 1, Qt::AlignRight );
	connect( cancelbtn, SIGNAL(clicked()), this, SLOT(reject()));

	QLabel * levellbl = new QLabel("Upgrade level:", this);
	_grid->addWidget(levellbl, 1, 0, Qt::AlignLeft );

	_levelList = new QComboBox(this, "_levelList");
	_grid->addWidget(_levelList, 1, 1, Qt::AlignCenter );

	_populate_group_list(current);

	item_selected(0);
}

void ImproveDialog::_populate_group_list(const PlayerState * player)
{
	const Game_Base & base = _gamestate->get_base();

	// used to store properties in a group
	std::vector<unsigned> props;

	// list of properties owned by the player
	const net::PropertyList_t & pown = player->get_owned_properties();

	_groupList = new QComboBox(this, "_propList");

	connect( _groupList, SIGNAL(activated(int)), this, SLOT(item_selected(int)));

	_grid->addMultiCellWidget(_groupList, 0, 0, 0, 1, Qt::AlignCenter );

	for(unsigned i = 0; i < base.num_spaces(); ++i) {

		// skip lots that aren't properties
		if(Game_Base::kProp != base.type(i)) continue;

		// skip lots that aren't in a group, and extract the group members
		if(!base.group_members(i, props)) continue;

		// props will now contain the list of properties for the group
		// whose representative (first) member is `i'

		// see if the player owns each of them
		std::vector<unsigned>::const_iterator it;
		bool owns = true;
		for(it = props.begin(); it != props.end(); ++it) {

			// look for the property in the player's list of properties
			if(pown.end() == std::find(pown.begin(), pown.end(), *it)) {
				owns = false;
				break;
			}
		}

		if(owns) {
			const std::string * name = base.name(i);
			if(name == NULL) continue;

			// display the name
			_groupList->insertItem(*name);

			// `i' is still the representative property id
			_translate.push_back(i);
		}

		// now set `i' equal to  the last property id
		i = *max_element(props.begin(), props.end());
	}
}


void ImproveDialog::item_selected(int iid)
{
	static QString list_table[6] = {
		"Empty lot",
		"1 house",
		"2 houses",
		"3 houses",
		"4 houses",
		"1 hotel",
	};

	unsigned id = (unsigned)iid;
	if(id >= _translate.size()) return;

	_levelList->clear();

	unsigned prop = _translate[id];

	const Game_Base & base = _gamestate->get_base();
	unsigned clvl = _gamestate->get_property(prop).get_level();

	std::vector<unsigned> vec;
	unsigned pcount = base.group_members(prop, vec);

	for(unsigned k = 0; k < 6; ++k ) {
		QString label = list_table[k];

		if(clvl > k) {
			unsigned profit = 0;

			for(unsigned i = 0; i < pcount; ++i) {
				prop = vec[i];

				// we must do this for each property in the proplist
				for(unsigned j = clvl; j > k; --j) {
					profit += (base.upgrade(prop, j-1) / 2);
				}
			}

			label += " (earn) $";
			label += QString::number(profit);

			//label += 
		} else if( clvl < k ) {
			unsigned cost = 0;

			for(unsigned i = 0; i < pcount; ++i) {
				std::cout << "2accessing at pcount="<< pcount << std::endl;
				prop = vec[i];
				for(unsigned j = clvl; j < k; ++j) {
					cost += base.upgrade(prop, j);
				}
			}
			label += " (cost) $";
			label += QString::number(cost);

		} else {
			label += "";
		}

		_levelList->insertItem( label );
	}
}


unsigned ImproveDialog::get_level() const
{
	int c = _levelList->currentItem();
	return c;
}

unsigned ImproveDialog::get_property() const
{
	unsigned c = _groupList->currentItem();
	if(c < 0 || c >= _translate.size()) return 0;

	return _translate[c];
}


