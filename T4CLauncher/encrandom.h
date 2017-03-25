// Random.h: interface for the Random class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANDOM_H__LAUNCHER__INCLUDED_)
#define AFX_RANDOM_H__LAUNCHER__INCLUDED_


#define MAXINT 0xFFFFFFFF

namespace t4csvrL{

typedef unsigned __int64 uhyper;
//typedef __int64          hyper;

class dice{
public:
    dice( unsigned int nNewDiceCount, unsigned int nNewDiceValue, int nNewPlus = 0 ) :
        nDiceCount( nNewDiceCount ), nDiceValue( nNewDiceValue ), nPlus( nNewPlus ){};

    inline unsigned int Value( void ){ return nDiceValue; };
    inline unsigned int Count( void ){ return nDiceCount; };
    inline int Plus ( void ){ return nPlus;      };
private:
    unsigned int nDiceCount;
    unsigned int nDiceValue;
    int nPlus;
};

class __declspec(dllexport) Random  
{
	public:
        Random( void ){};

		static void SetSeed(uhyper NewSeed = 0/*(unsigned long)time(NULL)*/);
				
		static int testvs( int nAttacker, int nDefender );

		signed int operator () ( uhyper NewSeed, int NewMinValue, int NewMaxValue );
		signed int operator () ( int NewMinValue, int NewMaxValue);
        signed int operator () ( dice &cDice );

        inline signed int roll( dice &cDice ){
            return (*this)( cDice );
        }
        
        static uhyper GetSeed( void );
	private:
        // Prohibit assignements and copies.
        Random( Random &cRand ){};
        Random &operator = ( Random &cRand ){ return cRand; };

		static int    LastValue;
        static uhyper Seed;

		static __int64 Randomize( int nMin, int nMax, BOOL boSameTwice = FALSE );
};

}
#endif // !defined(AFX_RANDOM_H__LAUNCHER__INCLUDED_)
