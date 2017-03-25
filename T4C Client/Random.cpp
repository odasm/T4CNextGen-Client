// Random.cpp: implementation of the Random class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <windows.h>
#include "Random.h"
#include "Global.h"
#include "MemoryManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Random::~Random()
{

}

Random::Random(unsigned int NewMinValue, unsigned int NewMaxValue,
					unsigned int InitSeed, unsigned int seedNumber) {
// Constructor
	MinValue = NewMinValue;
	MaxValue = NewMaxValue + 1;
	Seed = InitSeed;
	SeedID = seedNumber;

	CreateTable();
}

void Random::SetSeed(unsigned int NewSeed) {
// Change the Seed Value.
	Seed = NewSeed;

	CreateTable();
}

void Random::SetMinValue(unsigned int NewMinValue) {
// Change the Minimum Value;
	MinValue = NewMinValue;

	CreateTable();
}

void Random::SetMaxValue(unsigned int NewMaxValue) {
// Change the Maximum Value;
	MaxValue = NewMaxValue + 1;

	CreateTable();
}

void Random::SetMinMaxValue(unsigned int NewMinValue,
									 unsigned int NewMaxValue) {
// Change the Minimum and Maximum Value;
	MinValue = NewMinValue;
	MaxValue = NewMaxValue + 1;

	CreateTable();
}

Random::operator int (void) {
// Return the Random Number.
	return Randomize();
}

inline unsigned int Random::Randomize(void) {
// Generate a Random Number
	switch (SeedID) {
	case 0:
		Seed = Seed * 8203597 + 5 * 3;
		break;
	case 99999:
		Seed = Seed * 7563921 + 1;
		break;
	}
	
	if (MaxValue==MinValue) 
      return MinValue;
   
   return (Seed % (MaxValue-MinValue)) + MinValue;
}

unsigned int Random::operator () (unsigned int NewSeed) {
	// Return the Random Number using a New Seed.
	Seed = NewSeed;

	return Randomize();
}

unsigned int Random::operator () (unsigned int NewMinValue,
														 unsigned int NewMaxValue) {
// Return the Random Number using a New Minimun and Maximum Value.
	MinValue = NewMinValue;
	MaxValue = NewMaxValue + 1;

	CreateTable();
	return Randomize();
}

unsigned int Random::operator () (unsigned int NewSeed,
											 unsigned int NewMinValue,
											 unsigned int NewMaxValue) {
// Return the Random Number using a New Seed, New Minimun and Maximum Value.
	Seed = NewSeed;
	MinValue = NewMinValue;
	MaxValue = NewMaxValue + 1;

	CreateTable();
	return Randomize();
}