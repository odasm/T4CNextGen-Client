/******************************************
	Statistics of a Process/Threads
	Copyright 1999 Vircom Inc.
	Benoit Martin
*******************************************/

#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include "ProcStat.h"

CProcStat::CProcStat()
{
	// initialize the arrays.. 
	memset(&m_adwThreadIDBank, 0, sizeof(m_adwThreadIDBank));
	m_bEnabled = FALSE;
	QueryPerformanceFrequency(&m_liFreq);
}

CProcStat::~CProcStat()
{
}


void CProcStat::
EnableEngine()
// Start processing tick counts for each registered blocks..
{
	m_dwStartTest = ::timeGetTime();
   m_bEnabled = TRUE;
	return;
}


void CProcStat::
DisableEngine()
// Stop processing tick counts for each registered blocks..
{
	m_dwEndTest = ::timeGetTime();
	m_bEnabled = FALSE;
	return;
}


BOOL CProcStat::
StartBlockCount(DWORD dwThreadID, DWORD dwBlockID)
// Timestamps the given BlockID with the current tick count..
{
	if (m_bEnabled)
	{
		// verify if BlockID too big.. 
		if (dwBlockID >= MAX_BLOCK_ID_VALUE) return FALSE;

		// verify first if dwThreadID has been mapped... 
		short nID = getArrayID(dwThreadID);
      if (nID == -1) {
      // Set a Default Thread name
         SetThreadIDName(dwThreadID, "Undefined");
         nID = getArrayID(dwThreadID);
      }

		// update structure with start information..
		STAT_BLOCK *ptrBlock = &(m_aStat[nID].aStatBlock[dwBlockID]);
		QueryPerformanceCounter(&ptrBlock->liStartCounter);
      ptrBlock->iCount++;
	}
	return TRUE;
}


BOOL CProcStat::
StopBlockCount(DWORD dwThreadID, DWORD dwBlockID)
// Calculate total amount of ticks spent and update total..
{
	if (m_bEnabled)
	{
		// verify if BlockID too big.. 
		if (dwBlockID >= MAX_BLOCK_ID_VALUE) return FALSE;

		// verify first if dwThreadID has been mapped... 
		short nID = getArrayID(dwThreadID);
		if (nID == -1) return FALSE;

		// update structure with stop information..
		STAT_BLOCK *ptrBlock = &(m_aStat[nID].aStatBlock[dwBlockID]);
		LARGE_INTEGER liStop;

      if (ptrBlock->liStartCounter.LowPart) {
      // Counter was started.
         QueryPerformanceCounter(&liStop);
		   ptrBlock->fTotalTime += (float) (liStop.LowPart - ptrBlock->liStartCounter.LowPart) / (float) m_liFreq.LowPart;
         ptrBlock->liStartCounter.LowPart = 0;
      }
	}
	return TRUE;
}


short CProcStat::
GenerateInfo(char *szBuffer, short nLenBuffer)
// Create report and return it in string buffer..
//
// returns	-1 if an error occured
//			0 if successfull
//			size X, if buffer not big enough..
{
	// report format is like this
	// date - title.. 
	// Section on Each Threads/Name
		// SubSection on Each BlockID/Name: 
	// Total of All Threads
	// Statistics: TotalTime, % of Thread Usage, % of OverAll Usage.

	// we need a certain size for LenBuffer.. 
	if (nLenBuffer <= 0) nLenBuffer = 1;

	short nRequiredSize = 0;
	char szLine[256];

	char *szTmpBuffer = new char[nLenBuffer];
	if (!szTmpBuffer) return -1;
	memset(szTmpBuffer, 0, nLenBuffer);

	float fOverallTime = getTotalTime(0);

	STAT_THREAD *ptrThread;
	STAT_BLOCK *ptrBlock;

	// TimeStamp
	SYSTEMTIME sysTime; 
	GetLocalTime(&sysTime);
	sprintf(szLine, "%s %d %d %d:%02d:%02d (Log Time %0.4f Seconds)  -  Thread Time Statistics\r\n",
		getMonth(sysTime.wMonth), sysTime.wDay, sysTime.wYear, sysTime.wHour, sysTime.wMinute,
		sysTime.wSecond, (float)(m_dwEndTest-m_dwStartTest)/1000.0);
	STRNCAT(szTmpBuffer, szLine, nLenBuffer);
	nRequiredSize += strlen(szLine);

	// go through each threads..
	for (short i=0; i<MAX_THREADS; i++)
	{
		if (m_adwThreadIDBank[i]) 
		{
			float fThreadTime = getTotalTime(m_adwThreadIDBank[i]);
			ptrThread = &m_aStat[i];

			// header of thread
			// ThreadName:ThreadID \tabTotal: %lu time
			sprintf(szLine, "\r\n%-32.32s [ID:%lu]\tTotal: %0.4f seconds\r\n\r\n", 
				ptrThread->szName, m_adwThreadIDBank[i], fThreadTime);

			// STRNCAT will prevent we overflow the buffer.. 
			STRNCAT(szTmpBuffer, szLine, nLenBuffer);
			nRequiredSize += strlen(szLine);

			// go through each blockIDs..
			for (short j=0; j<MAX_BLOCK_ID_VALUE; j++)
			{
				ptrBlock = &(ptrThread->aStatBlock[j]);
				if (ptrBlock->fTotalTime > 0)
				{
					float fBlock = (float) ((ptrBlock->fTotalTime / fThreadTime) * 100.0);
					float fTotal = (float) ((ptrBlock->fTotalTime / fOverallTime) * 100.0);

					// print out to Buffer..
					sprintf(szLine, "\t%-32.32s [ID:%lu]\t%0.4f seconds [%2.2f%% <> %2.2f%%, %I64u -> %0.10f]\r\n", 
						ptrBlock->szName, j, ptrBlock->fTotalTime, fBlock, fTotal, ptrBlock->iCount, ptrBlock->fTotalTime / (float)ptrBlock->iCount);

					STRNCAT(szTmpBuffer, szLine, nLenBuffer);
					nRequiredSize += strlen(szLine);
				}
			}
		}
	}

	// now verify if enough space was included..
	if (nRequiredSize >= nLenBuffer)
	{
		delete[] szTmpBuffer;
		return nRequiredSize+10;
	}

	// copy our buffer into the returned buffer..
	STRNCPY(szBuffer, szTmpBuffer, nLenBuffer);

	delete[] szTmpBuffer;
	return 0;
}


BOOL CProcStat::
SetThreadIDName(DWORD dwThreadID, char *szName)
// Each Threads *must* be mapped through this function before they can be used.. 
// (actually maps the threadID in our structured array)
{
	if (!szName) return FALSE;

	for (short i=0; i<MAX_THREADS; i++)
	{
		if (!m_adwThreadIDBank[i]) 
		{
			m_adwThreadIDBank[i] = dwThreadID;
			STAT_THREAD *ptrThread = &m_aStat[i];
			STRNCPY(ptrThread->szName, szName, sizeof(ptrThread->szName));
			return TRUE;
		}
	}
	return FALSE;
}


BOOL CProcStat::
SetBlockIDName(DWORD dwThreadID, DWORD dwBlockID, char *szName)
// Optional function: Blocks are not required to be named.. 
{
	if ((dwBlockID >= MAX_BLOCK_ID_VALUE) || !szName) return FALSE;

	short nID = getArrayID(dwThreadID);
	if (nID == -1) return FALSE;

	STAT_BLOCK *ptrBlock = &m_aStat[nID].aStatBlock[dwBlockID];
	STRNCPY(ptrBlock->szName, szName, sizeof(ptrBlock->szName));
	return TRUE;
}


// Private methods
short CProcStat::
getArrayID(DWORD dwID)
// seek in our ThreadIDBank, the given ID, and return its array position..
// returns -1 if not found.
{
	for (short i=0; i<MAX_THREADS; i++)
	{
		if (m_adwThreadIDBank[i] == dwID) return i;
	}
	return -1;
}

float CProcStat::
getTotalTime(DWORD dwThreadID)
// returns the total amount of ticks assigned to blocks in a ThreadID, 
// or overall, if the threadID is 0..
{
	float fRetVal=0;
	STAT_THREAD *ptrThread;
	STAT_BLOCK *ptrBlock;

	// go through each threads..
	for (short i=0; i<MAX_THREADS; i++)
	{
		if (m_adwThreadIDBank[i] && (!dwThreadID || (dwThreadID == m_adwThreadIDBank[i])))
		{
			// go through each blockIDs..
			ptrThread = &m_aStat[i];

			for (short j=0; j<MAX_BLOCK_ID_VALUE; j++)
			{
				ptrBlock = &(ptrThread->aStatBlock[j]);
				fRetVal += ptrBlock->fTotalTime;
			}
		}
	}
	return fRetVal;
}

void CProcStat::
STRNCPY(char *dest,const char *src, WORD length) 
// Copy src to dest, appending a null byte and respecting the maximum length
// of dest.
{
	strncpy(dest, src, length);
	dest[length-1]=0;
	return;
}

void CProcStat::
STRNCAT(char *dest,const char *src, WORD length) 
// Concatenate src to dest, appending a null byte and respecting the maximum
// length of dest.
{
	WORD i = strlen(dest);
    if (i>=length) {
        return;
    }
    dest += i;
    while (*src && i<length-1) {
        *dest++ = *src++;
        i++;
    }
    *dest = '\0';
	return;
}

// utilities...
char * CProcStat::
getMonth(WORD wMonth)
{
	switch (wMonth)
	{
	case 1: return "January";
	case 2: return "February";
	case 3: return "March";
	case 4: return "April";
	case 5: return "May";
	case 6: return "June";
	case 7: return "July";
	case 8: return "August";
	case 9: return "September";
	case 10: return "October";
	case 11: return "November";
	case 12: return "December";
	}
	return NULL;
}
