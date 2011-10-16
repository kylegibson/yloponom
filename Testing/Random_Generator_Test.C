#include <iostream>
#include "core/Random_Generator.h"
using namespace std;

int main(void)
{
	Random_Generator rand_gen;
	
	for (int i = 0; i < 40; ++i)
		cout << rand_gen.roll_d6() << '\t' << rand_gen.roll_d6() << '\t'
			 << rand_gen.roll_d6() << '\t' << rand_gen.roll_d6() << '\t'
			 << rand_gen.roll_d6() << endl;

	cout << "\n\nTesting range(0, 1000): " << rand_gen.range(0, 1000);
	cout << "\nTesting range(1000, 0): " << rand_gen.range(1000, 0) << endl;

	return 0;
}
