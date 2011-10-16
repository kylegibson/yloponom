/////////////////////////////////////////////////////////////////////////////
// Game_Base.C
//
// Contains the class implementation for the Game_Base class which provides
// static space and card data to all concerned parties.
/////////////////////////////////////////////////////////////////////////////

#include "Game_Base.h"

Game_Base::Game_Base() : fSpaces(), fCards(), fRand(), fCSplit(0)
{
}

Game_Base::Game_Base(const char* filename) : fSpaces(), fCards(), fRand(),
											 fCSplit(0)
{
	load(filename);
}

Game_Base::~Game_Base()
{
	unload();
}

bool Game_Base::load(const char* filename)
{
	ifstream fin;		// input filestream object
	string	 st_tmp;
	char	 ch_tmp;	// temporary character
	int		 state = 0; // processing state
	bool (Game_Base::*reader[])(ifstream&, Game_Base&)
		 = {&Game_Base::pRead_Space,
			&Game_Base::pRead_ChCard,
			&Game_Base::pRead_CCCard};	// reader func. array because I can
	
	// attempt to open the specified file
	fin.open(filename);
	if (fin.fail()) return false;

	// priming read looking for comment or section delimiter
	ch_tmp = fin.peek();

	// reading loop
	while (fin.good())
	{
		switch (ch_tmp)
		{
			case ' ':
			case '\n':
			case '#':	// comment line
				fin.ignore(0x7fffffff, '\n');		// ignore line
				break;
			case '$':	// section delimiter line
				fin.ignore(0x7fffffff, '\n');		// ignore line
				++state;							// advance state
				break;
			default:	// data section, send it to appropriate reader
				if (!(this->*reader[state])(fin, (*this))) return false;
		}

		ch_tmp = fin.peek();
	}

	return true;
}

void Game_Base::unload()
{
	// clean up allocated memory
	for (unsigned i = 0; i < fSpaces.size(); ++i)		
		if (fSpaces[i].rent != NULL) delete[] fSpaces[i].rent;

	// we're done because the vector destructor will take care of the rest
	return;
}

unsigned Game_Base::group(unsigned index) const
{
	if (index >= fSpaces.size()) return 0;		// stupidity check
	if (kUtil > fSpaces[index].type) return 0;	// only kRRoad, kUtil and kProp
	return fSpaces[index].group;				// ^ have a group id
}

unsigned Game_Base::group_members(unsigned index, vector<unsigned>& members) const
{
	signed l, r, i = 0;

	if (index >= fSpaces.size()) return 0;		// stupidity check
	if (kUtil > fSpaces[index].type) return 0;	// only kRRoad, kUtil and kProp

	members.clear();			// clear the reference vector
	members.push_back(index);	// put the starting index in the return array
	++i;

	l = index + 1;	// left search point setup
	r = index - 1;	// right search point setup

	for (; l != r; ++l, --r)	// comparison works due to board geom.
	{
		l %= fSpaces.size();				// left search adjustment
		if (0 > r) r = fSpaces.size() - 1;	// right search adjustment

		// check both l and r for membership
		if (fSpaces[l].group == fSpaces[index].group)
		{
			members.push_back(l);
			++i;
		}
		
		if (fSpaces[r].group == fSpaces[index].group)
		{
			members.push_back(r);
			++i;
		}
	}

	// final check of the overlapping position
	if (fSpaces[l].group == fSpaces[index].group)
	{
		members.push_back(l);
		++i;
	}
	
	return i;
}

unsigned Game_Base::rep_member(unsigned gid) const
{
	unsigned size = fSpaces.size();

	for (unsigned i = 0; i < size; ++i)
		if (fSpaces[i].group == gid) return i;

	return 0;
}

const string* Game_Base::name(unsigned index) const
{
	if (index >= fSpaces.size()) return NULL;	// stupidity check
	return &(fSpaces[index].name);
}

signed Game_Base::price(unsigned index) const
{
	if (index >= fSpaces.size()) return 0; // stupidity check
	return fSpaces[index].price;		  // return the price (universal)
}

unsigned Game_Base::rent(unsigned index, unsigned level) const
{
	if (index >= fSpaces.size()) return 0;	// stupidity check

	switch (fSpaces[index].type)			// filter by type
	{
		case kRRoad:						// kRRoad only has one level
			if (7 < level) return 0;
			return fSpaces[index].rent[0] * (1 << level);
		case kProp:							// kProp has six levels
			if (6 < level) return 0;
			if (5 < level) return 3 * fSpaces[index].rent[5] / 2; //1.5*lvl5
			return fSpaces[index].rent[level];
		default:							// everything else has no rent
			return 0;
	}
}

unsigned Game_Base::type(unsigned index) const
{
	if (index >= fSpaces.size()) return 0;	// stupidity check
	return fSpaces[index].type;
}

unsigned Game_Base::upgrade(unsigned index, unsigned level) const
{
	if (index >= fSpaces.size()) return 0;		// stupidity check
	if (kProp != fSpaces[index].type) return 0; // only kProp is upgradeable
	if (5 < level) return 0;					// no further upgrades possible

	// standard case from levels 0 - 5
	if (5 > level) return fSpaces[index].rent[0] * 10;

	// special case from 5 -> 6
	return fSpaces[index].rent[0] * 15;
}

unsigned Game_Base::num_spaces() const
{
	return fSpaces.size();
}

const Game_Base::GB_Card* Game_Base::draw_card(unsigned deck, unsigned* num) const
{
	unsigned rand;

	if (deck > 1) return NULL;	// stupidity check

	// draw the card from the right deck
	if (deck) rand = fRand.range(fCSplit, fCards.size() - 1);
	else rand = fRand.range(0, fCSplit - 1);

	// return the card number and data
	(*num) = rand;
	return &(fCards[rand]);
}

const Game_Base::GB_Card* Game_Base::get_card(unsigned num) const
{
	if (num >= fCards.size()) return NULL;

	return &(fCards[num]);
}
	

bool Game_Base::pRead_Space(ifstream& fin, Game_Base& base)
{
	GB_Space temp;

	getline(fin, temp.name);		// get the name
	fin >> temp.type;				// get the type
	fin.ignore(0x7fffffff, '\n');
	fin >> temp.price;				// get the price
	fin.ignore(0x7fffffff, '\n');
	
	switch (temp.type)
	{
		case kUtil:		// utils have a group id
			fin >> temp.group;
			temp.rent = NULL;
			break;
		case kRRoad:	// RRoads have group and rent[0]
			fin >> temp.group;
			fin.ignore(0x7fffffff, '\n');
			temp.rent = new unsigned[1];
			fin >> temp.rent[0];
			break;
		case kProp:		// props have group and rent[0,5]
			fin >> temp.group;
			fin.ignore(0x7fffffff, '\n');
			temp.rent = new unsigned[6];
			for (int i = 0; i < 6; ++i) fin >> temp.rent[i];
			break;
		default:		// for everything else group = rent = 0
			temp.rent = NULL;
			temp.group = 0;
	}

	fin.ignore(0x7fffffff, '\n');

	// push the constructed entry into the data array
	base.fSpaces.push_back(temp);

	return true;
}

bool Game_Base::pRead_ChCard(ifstream& fin, Game_Base& base)
{
	GB_Card temp;

	temp.deck = false;

	getline(fin, temp.message);		// get the message
	fin >> temp.type;				// get the type
	fin.ignore(0x7fffffff, '\n');
	fin >> temp.value;				// get the value
	fin.ignore(0x7fffffff, '\n');

	// push the constructed entry into the data array
	base.fCards.push_back(temp);


	// increment the split indicator
	base.fCSplit += 1;

	return true;
}

bool Game_Base::pRead_CCCard(ifstream& fin, Game_Base& base)
{
	GB_Card temp;

	temp.deck = true;

	getline(fin, temp.message);		// get the message
	fin >> temp.type;				// get the type
	fin.ignore(0x7fffffff, '\n');
	fin >> temp.value;				// get the value
	fin.ignore(0x7fffffff, '\n');

	// push the constructed entry into the data array
	base.fCards.push_back(temp);

	return true;
}
