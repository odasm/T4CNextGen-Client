// Random.h: interface for the Random class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __RANDOM_H
#define __RANDOM_H

#define MAXSHRT 65535

#include <time.h>

class Random  
{
public:
	virtual ~Random();

	protected:
   public:
		unsigned int Seed;
		unsigned int MinValue;
		unsigned int MaxValue;
		unsigned int SeedID;

		virtual unsigned int Randomize(void);
		virtual void CreateTable(void) {};

	public:
		Random(unsigned int NewMinValue = 0,
				 unsigned int NewMaxValue = MAXSHRT,
				 unsigned int InitSeed = (unsigned int)time(NULL),
				 unsigned int seedNumber = 0);

		void SetSeed(unsigned int NewSeed = 0/*(unsigned long)time(NULL)*/);
		void SetMinValue(unsigned int NewMinValue = 0);
		void SetMaxValue(unsigned int NewMaxValue = MAXSHRT);
		void SetMinMaxValue(unsigned int NewMinValue = 0,
								  unsigned int NewMaxValue = MAXSHRT);
		operator int  (void);

		unsigned int operator () (unsigned int NewSeed);
		unsigned int operator () (unsigned int NewMinValue,
										  unsigned int NewMaxValue);
		unsigned int operator () (unsigned int NewSeed,
										  unsigned int NewMinValue,
										  unsigned int NewMaxValue);
};

template <int Number>
class RandomTable// : public Random
{
	protected:
		unsigned int Seed;
		unsigned int MinValue;
		unsigned int MaxValue;
		unsigned int Multiplier;

	private:
		unsigned short Values[Number];
		
		unsigned int Randomize(void);
		void CreateTable(void);
	
	public:
		RandomTable(unsigned int NewMinValue = 0,
						unsigned int NewMaxValue = MAXSHRT,
						unsigned int InitSeed = (unsigned int)time(NULL));

		void CreateRandom(unsigned int NewMinValue = 0,
						      unsigned int NewMaxValue = MAXSHRT,
						      unsigned int InitSeed = (unsigned int)time(NULL),
							  unsigned int NewMultiplier = 0);

		unsigned int operator () (unsigned int NewSeed);
};

template <int Number>
unsigned int RandomTable<Number>::operator () (unsigned int NewSeed) {
	// Return the Random Number using a New Seed.
	Seed = NewSeed;

	return Randomize();
}

template <int Number>
void RandomTable<Number>::CreateRandom(unsigned int NewMinValue,
											 unsigned int NewMaxValue,
											 unsigned int InitSeed,
											 unsigned int NewMultiplier) {
// Create Table
	MinValue = NewMinValue;
	MaxValue = NewMaxValue;
	Seed = InitSeed;
	if (NewMultiplier) Multiplier = NewMultiplier;

	CreateTable();
}

template <int Number>
void RandomTable<Number>::CreateTable(void) {
	for (int Index = 0; Index < Number; Index++) {
		Seed = Seed * Multiplier + 1;
		//Seed = Seed * 8203597 + 1; // new Key
		Values[Index] = (Seed % (MaxValue-MinValue)) + MinValue;
	}
}

template <int Number>
inline unsigned int RandomTable<Number>::Randomize(void) {
// Generate a Random Number
		return Values[Seed % Number];
}

template <int Number>
RandomTable<Number>::RandomTable(unsigned int NewMinValue,
											 unsigned int NewMaxValue,
											 unsigned int InitSeed) {
// Constructor
	MinValue = NewMinValue;
	MaxValue = NewMaxValue;
	Seed = InitSeed;
	Multiplier = 7563921;

	CreateTable();
}

#endif 
