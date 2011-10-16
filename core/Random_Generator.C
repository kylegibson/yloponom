/////////////////////////////////////////////////////////////////////////////
// Random_Generator.C
//
// Contains the class implementation for the Random_Generator class which
// provides ranged integer random number generation capability.
/////////////////////////////////////////////////////////////////////////////

#include "Random_Generator.h"

Random_Generator::Random_Generator()
{
	srand(time(0));
}

unsigned int Random_Generator::roll_d6() const
{
	return range(1, 6);
}

unsigned int Random_Generator::range(unsigned int min, unsigned int max) const
{
	//double d_rand;
	//unsigned int u_rand;

	// get the range in proper order
	if (min > max) 
	{
		unsigned int temp = min;
		min = max;
		max = temp;
	}

	// a simpler way because we don't really care and I can't find my more 31337 code
	return rand() % (max - min + 1) + min;

	// get a double precision fp random in [0, max - min + 1]
//	d_rand = rand()/(double)RAND_MAX * (max - min + 1);

	// floor to the nearest integer and add min
//	u_rand = (unsigned int)floor(d_rand - 0.000000000001) + min;

//	return u_rand;
}
