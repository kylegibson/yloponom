#include "../core/Game_Base.h"

const unsigned tar = 35;

int main()
{
	Game_Base base("../data.dat");
	const Game_Base::GB_Card* temp;
	vector<unsigned> vec;
	unsigned cnt, spcs;

	spcs = base.num_spaces();

	for (int j = 0; j < spcs; ++j)
	{

		cout << (*base.name(j)) << ' ';

		int k = 54 - base.name(j)->size();
		
		for (int i = 0; i < k; ++i)
			cout << '-';
			
		cout << "\n  Type:    ";

		switch (base.type(j))
		{
			case Game_Base::kGo:
				cout << "GO";
				break;
			case Game_Base::kJail:
				cout << "Jail";
				break;
			case Game_Base::kFParking:
				cout << "Free Parking";
				break;
			case Game_Base::kGoJail:
				cout << "Go to Jail";
				break;
			case Game_Base::kCasino:
				cout << "Casino";
				break;
			case Game_Base::kITax:
				cout << "Income Tax";
				break;
			case Game_Base::kLTax:
				cout << "Luxury Tax";
				break;
			case Game_Base::kChance:
				cout << "Chance";
				break;
			case Game_Base::kCChest:
				cout << "Community Chest";
				break;
			case Game_Base::kUtil:
				cout << "Utility";
				break;
			case Game_Base::kRRoad:
				cout << "Railroad";
				break;
			case Game_Base::kProp:
				cout << "Property";
				break;
		}
		//0=kGo, 1=kJail, 2=kFParking, 3=kGoJail, 4=kCasino, 5=kITax, 6=kLTax, 7=kChance, 8=kCChest, 9=kUtil, 10=kRRoad, 11=kProp
		
		if (base.price(j)) cout << "\n  Price:   $" << base.price(j);
		
		if (base.group(j))
		{
			cout << "\n  Group #: " << base.group(j) << "\n  Members: ";
			cnt = base.group_members(j, vec);

			for (int i = 1; i < cnt; ++i)
			{
				if (i > 1) cout << "           ";
				cout << (*base.name(vec[i])) << endl;
			}
		}

		switch (base.type(j))
		{
			case Game_Base::kRRoad:
				cout << "  Rent:    ";
				for (int i = 0; i < 8; ++i)
					cout << '$' << base.rent(j, i) << ' ';
				break;
			case Game_Base::kProp:
				cout << "  Rent:    ";
				for (int i = 0; i < 7; ++i)
					cout << '$' << base.rent(j, i) << ' ';
		}

		cout << "\n-------------------------------------------------------\n\n";
	}

	

	cout << "Radomly drawn Chance Cards\n-------------------------------------------------------\n";
	for (int i = 0; i < 10; ++i)
	{
		unsigned num;
		temp = base.draw_card(0, &num);
		cout << "- " << temp->message << " - " << num << endl;
		temp = base.get_card(num);
		cout << "- " << temp->message << " - " << num << "\n\n";

	}

	cout << "\n\nRandomly drawn Community Chest Cards\n-------------------------------------------------------\n";

	for (int i = 0; i < 10; ++i)
	{
		unsigned num;
		temp = base.draw_card(1, &num);
		cout << "- " << temp->message << " - " << num << endl;
		temp = base.get_card(num);
		cout << "- " << temp->message << " - " << num << "\n\n";
	}

	/*{
		//unsigned num;
		//temp = base.draw_card(1, &num);
		temp = base.get_card(42);
		cout << "- " << temp->message << " - " << "42" << endl;
		temp = base.get_card(42);
		cout << "- " << temp->message << " - " << "42" << "\n\n";
	}*/

	return 0;
}
