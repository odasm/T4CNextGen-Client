#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include "c2.h"
#include "Database2.h"
#include "PowerDebug.h"
#include "App.h"
#include "Global.h"
#include "MemoryManager.h"
#include "LocalString.h"
#include "MacroHandler.h"

extern UserCustom Custom;

VSBDataBase ug_Database2_44444;

VSBDataBase::VSBDataBase(void) {
   boIndexLoad = FALSE;
   qwOffset = 0;
   dwNbIndex = 0;
	InitializeCriticalSection( &csThreadLock );
}

VSBDataBase::~VSBDataBase(void) {
   DeleteCriticalSection( &csThreadLock );

	lpbDataBuffer.Release();

   if (IndexInfo) {
      for (UINT i = 0; i < dwNbIndex; i++) {
         if (IndexInfo[i].lpszID) 
         {
            delete IndexInfo[i].lpszID;
            IndexInfo[i].lpszID = NULL;
         }
         if (IndexInfo[i].lpvsbInfo)
         {
            delete IndexInfo[i].lpvsbInfo;
            IndexInfo[i].lpvsbInfo = NULL;
         }
      }
      delete [] IndexInfo;
      IndexInfo = NULL;
   }
}

void VSBDataBase::Release(int dwIndexPos) 
{
   if (IndexInfo[dwIndexPos].dwCount)
      IndexInfo[dwIndexPos].dwCount--;

   if (!IndexInfo[dwIndexPos].dwCount && IndexInfo[dwIndexPos].lpvsbInfo)  
   {
      if (IndexInfo[dwIndexPos].lpvsbInfo) 
      {
         delete IndexInfo[dwIndexPos].lpvsbInfo;
         IndexInfo[dwIndexPos].lpvsbInfo = 0;
      }
   } 
}

void VSBDataBase::LoadIndex(void) {
	boIndexLoad = TRUE;
   lpbDataBuffer.Create("Game Files\\T4CGameFile.vsb");
	GetFromBuffer3(&dwNbIndex, 4);

	IndexInfo = new VSBINDEXINFO [dwNbIndex];

	BYTE wIDLenght;

	for (UINT i = 0; i < dwNbIndex; i++) {
      GetFromBuffer3(&wIDLenght, 1);
		IndexInfo[i].lpszID = new TCHAR [wIDLenght + 1];
		IndexInfo[i].lpszID[wIDLenght] = 0;
		GetFromBuffer3(IndexInfo[i].lpszID, wIDLenght);
		GetFromBuffer3(&IndexInfo[i].dwOffset, 4);
		GetFromBuffer3(&IndexInfo[i].dwSize, 4);
		GetFromBuffer3(&IndexInfo[i].wHertz, 2);
		GetFromBuffer3(&IndexInfo[i].bBits, 1);
		IndexInfo[i].dwCount = 0;
		IndexInfo[i].lpvsbInfo = 0;
	}
}

DWORD VSBDataBase::LoadWave( 
 LPTSTR    lpszID,	  // ID of the Object
 LPVSBINFO *lplpvsbInfo  // Ptr of a CV2Sprite Type
) {
	EnterCriticalSection( &csThreadLock );
	
	if (!boIndexLoad) LoadIndex();
	
	signed int Offset = -1; // Offset of the ID.
	DWORD      dwIdxPos = 0;				  // Index Position.
	
	for (DWORD Index = 0; Index < dwNbIndex; Index++) {
	// Search the Offset of the Sprite
      if (!stricmp(IndexInfo[Index].lpszID, lpszID)) {
		// Sprite ID si found.
			Offset = IndexInfo[Index].dwOffset;
			dwIdxPos = Index;
			Index= dwNbIndex;
		}
	}

   if (Offset == -1) {
	// Cannot find the Sprite ID
      LeaveCriticalSection( &csThreadLock );
		LOG << "Error With Sound ID : " << lpszID << ".\r\nDebug Info ---\r\n NbIndex [" << dwNbIndex << "] \r\n";
   	for (DWORD Index = 0; Index < dwNbIndex; Index++) {
         LOG << IndexInfo[Index].lpszID << "\r\n";
		}
      return 0xFFFFFFFF;
	}

   // Increment Usage Count
	IndexInfo[dwIdxPos].dwCount++;
	
	if (IndexInfo[dwIdxPos].lpvsbInfo) {
	// This Graphics is already loaded in memory.
		*lplpvsbInfo = IndexInfo[dwIdxPos].lpvsbInfo;
		LOG << "Get OLD Sound\r\n";
	} else {
      *lplpvsbInfo = new VSBINFO;

      qwOffset = Offset;

      (*lplpvsbInfo)->wHertz = IndexInfo[dwIdxPos].wHertz;
      (*lplpvsbInfo)->bBits = IndexInfo[dwIdxPos].bBits;
      (*lplpvsbInfo)->dwSize = IndexInfo[dwIdxPos].dwSize;
		IndexInfo[dwIdxPos].lpvsbInfo = *lplpvsbInfo;
   } 
	LeaveCriticalSection( &csThreadLock );

   return dwIdxPos;
}

DWORD VSBDataBase::LoadChunck( 
 DWORD dwIdxPos,
 LPVSBINFO *lplpvsbInfo,  // Ptr of a CV2Sprite Type
 DWORD dwOffLoad,
 DWORD dwMaxLoad,
 LPBYTE lpBuffer
) {
   EnterCriticalSection( &csThreadLock );
	
   qwOffset = IndexInfo[dwIdxPos].dwOffset + dwOffLoad;

   GetFromBuffer3(lpBuffer, dwMaxLoad);

	LeaveCriticalSection( &csThreadLock );

   return dwIdxPos;
}

      
      

