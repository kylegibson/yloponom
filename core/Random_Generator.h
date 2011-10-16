/////////////////////////////////////////////////////////////////////////////
// Random_Generator.h
//
// Contains the class defininition for the Random_Generator class which
// provides ranged integer random number generation capability.
/////////////////////////////////////////////////////////////////////////////

#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

// includes required by Random_Generator
#include <cstdlib>
#include <ctime>
#include <cmath>

class Random_Generator
{
public:

	// default constructor - seeds C pseudorandom number generator
	Random_Generator();

	// roll_d6 - equivalent to range(1, 6)
	unsigned int roll_d6() const;

	// range - returns an unsigned int in range [arg1, arg2]
	unsigned int range(unsigned int, unsigned int) const;
};

#endif
