// Random.cpp: implementation of the Random class.
//
//////////////////////////////////////////////////////////////////////

//#include "pch.h"
//#include <windows.h>
#include "stdafx.h"
#include <time.h>
#include "EncRandom.h"
#include <algorithm>
 
 
namespace t4csvrL{
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

uhyper Random::Seed = time(NULL);
int Random::LastValue = 0;  // This value is not thread safe,
                            // but this is irrelevant for a randomizer.

hyper Random::Randomize( int nMin, int nMax, BOOL boSameTwice ) {
// Generate a Random Number
	Seed = Seed * 725472321UL + 1UL;

    // If minimum is higher
    if( nMin > nMax ){
        // Exchange values.
        std::swap( nMax, nMin );
    }

	// May happen that MaxValue = MinValue, then simply return one of the value.
    if( nMin != nMax ){
        int nValue = (int)(((Seed >> 16UL) % ( nMax-nMin)) + nMin);
        if( LastValue == nValue && !boSameTwice ){
            // Re-randomize but this time allow same value twice to avoid deadlocks.
            return Randomize( nMin, nMax, TRUE );
        }
        LastValue = nValue;
		return nValue;
	}else{        
		return LastValue = (int)( nMin );
	}
}

int Random::operator () ( int nMin, int nMax ) {
// Return the Random Number using a New Minimun and Maximum Value.
	return (int)Randomize( nMin, nMax + 1 );
}

int Random::operator () (uhyper NewSeed, int NewMinValue, int NewMaxValue) {
// Return the Random Number using a New Seed, New Minimun and Maximum Value.
	Seed = NewSeed;

	return (int)Randomize( NewMinValue, NewMaxValue );
}
/*int Random::roll(DWORD dice){
	WORD i;
	int value = 0;

	int nMax = LOWORD(dice) + 1;

	for(i = 0; i < HIWORD(dice); i++){
		value += (int)Randomize( 1, nMax, TRUE );
	}
	
return value;
}*/

uhyper Random::GetSeed( void ){ 
    return Seed; 
};
void Random::SetSeed( uhyper NewSeed ){
    Seed = NewSeed;
}

signed int Random::operator () ( dice &cDice ){
    signed int nReturn = cDice.Plus();

    UINT i;
    for( i = 0; i < cDice.Count(); i++ ){
        nReturn += (int)Randomize( 1, cDice.Value() + 1, TRUE );
    }
    return nReturn;
}

int Random::testvs( int nAttacker, int nDefender ){
	int nDefenderDice = 0;
	int nAttackerDice = 0;
	
    nAttackerDice = (int)Randomize( 1, nAttacker );
	nDefenderDice = (int)Randomize( 1, nDefender );
	
	return nAttackerDice - nDefenderDice;
}
}
