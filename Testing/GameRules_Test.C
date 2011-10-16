#include <iostream>
#include "core/GameRules.h"

int main()
{
	PlayerState		p_test(0,"bob",100000000,0);
	Game_Base		b_test("../data.dat");
	net::GameState	s_test(&b_test);
	GameRules		r_test(&s_test);
	vector<unsigned> vec;
	int		 d1, d2;
	signed			 win;


	p_test.add_property(5);
	p_test.add_property(15);
	p_test.add_property(25);
	p_test.add_property(35);

	s_test.add_player(p_test);
	s_test.set_current_turn(p_test.get_name());

	cout << endl << r_test.count_mems(s_test.get_player(0), 5) << "  $" << b_test.rent(25,r_test.count_mems(s_test.get_player(0), 5) - 1) << endl;


	/*cout << "Craps\tRoll\t\tSeven\tRoll\n\n";
	for (int i = 0; i < 100; ++i)
	{
		cout << r_test.casino(100, GameRules::kCCraps, d1, d2) << '\t' << d1 + d2 << "\t\t"
			 << r_test.casino(100, GameRules::kCSeven, d1, d2) << '\t' << d1 + d2 << endl;
	}*/
	
	return 0;
}
