/******************************************
	Statistics of a Process/Threads
	Copyright 1999 Vircom Inc.
	Benoit Martin
*******************************************/

#ifndef __PROCSTAT_H_
#define __PROCSTAT_H_

// Maximum of 10 threads with up to 100 Code Blocks each to evaluate..
// The ThreadID/BlockID values must not exceed these numbers
// (static array used to store the information based on IDs for faster access)
#define MAX_THREADS			10
#define MAX_BLOCK_ID_VALUE	200

class CProcStat
{
public:
	CProcStat();
	~CProcStat();

	void EnableEngine();
	void DisableEngine();

	BOOL StartBlockCount(DWORD dwThreadID, DWORD dwBlockID);
	BOOL StopBlockCount(DWORD dwThreadID, DWORD dwBlockID);

	// If the engine is enabled, will disable it then return results...
	short GenerateInfo(char *szBuffer, short nLenBuffer);

	// Will not work if engine enabled..(must be called prior to EnableEngine())
	BOOL SetThreadIDName(DWORD dwThreadID, char *szName);
	BOOL SetBlockIDName(DWORD dwThreadID, DWORD dwBlockID, char *szName);

private:

	// maps threadIDs to array values..
	DWORD m_adwThreadIDBank[MAX_THREADS];

	struct STAT_BLOCK {
		LARGE_INTEGER liStartCounter;
		float fTotalTime;
		char szName[32];
      __int64  iCount;
	};
	
	struct STAT_THREAD {
		LARGE_INTEGER liStartCounter;
		float fTotalTime;
		char szName[32];
		STAT_BLOCK aStatBlock[MAX_BLOCK_ID_VALUE];
	};

	STAT_THREAD m_aStat [MAX_THREADS];
	BOOL  m_bEnabled;
   DWORD m_dwStartTest;
   DWORD m_dwEndTest;
	LARGE_INTEGER m_liFreq;

	// private methods
	short getArrayID(DWORD dwID);
	float getTotalTime(DWORD dwThreadID);

	void STRNCPY(char *dest,const char *src, WORD length);
	void STRNCAT(char *dest,const char *src, WORD length);
	char * getMonth(WORD wMonth);
};

#endif