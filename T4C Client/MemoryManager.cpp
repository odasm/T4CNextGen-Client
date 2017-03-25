#include "pch.h"
/*
#pragma warning( disable : 4786 )

#include <windows.h>
#include <malloc.h>
#include "stdio.h"
#include "MemoryManager.h"
#include "Autolock.h"
#include "PowerDebug.h"
#include "App.h"
#include "MacroHandler.h"

#undef new
#include "FileLogger.h"

void *operator new(size_t size, char *lpszSource, int iLine)
{		
   MemManager *Mem = MemManager::GetInstance();
   
   if (!Mem->Init || !g_Var.bMemory)
      return malloc(size);

   Autolock AL(&Mem->cs);
   return Mem->Create(size, lpszSource, iLine);
}

void *operator new(size_t size)
{		
   MemManager *Mem = MemManager::GetInstance();
   
   if (!Mem->Init || !g_Var.bMemory)
      return malloc(size);

   Autolock AL(&Mem->cs);
   return Mem->Create(size, "Undefined", 0);
}

void operator delete( void *ptr )
{
   MemManager *Mem = MemManager::GetInstance();

   if (!Mem->Init) {
      free(ptr);
      return;
   }
   
   Autolock AL(&Mem->cs);
   Mem->Free(ptr);
}

MemManager *MemManager::GetInstance(void) {
   static MemManager X;
   return &X;
}

MemManager::MemManager(void) {
// Constructor.
   Initialize();
}	

void MemManager::Initialize(void) {
// Constructor.
	nItems  = 0;
	memTop  = NULL;
	memWalk = NULL;
	memPtr  = NULL;
	memOld  = NULL;
   dwTotalAlloc = 0;

	// Creation de la tab.
	memTab = (MemTab *) malloc (sizeof(MemTab));
	ZeroMemory(memTab, sizeof(MemTab));

	InitializeCriticalSection(&cs);
   Init = true;
}

MemManager::~MemManager(void)
// Destructor.
{
   if (Init) {
      Init = false;
      Release();
   }
}

void MemManager::Release(void) {
	Init = false;

   int i;
   int j = 0;

   vir::FileLogger Log;
   Log.SetLogName("Memory.Log");
   Log.SetLogLevel(vir::LL_ALL);
   DWORD dwTotalLeak = 0;
   for (i=0; i<MEMTAB_SIZE; i++) {
	
		memPtr = memTab->memTop[i];
													
		while(memPtr) {
			memWalk = memPtr->Next;
			if (memPtr->ptr) {
				char Temp[1000];
				sprintf(Temp, "[%5d] Memory Leak in %s, Line %d, of size %d\r\n", 
						++j, memPtr->lpszSource, memPtr->iLine, memPtr->Size);
				OutputDebugString(Temp);
            Log.Log(vir::LL_ERROR, Temp);
            dwTotalLeak += memPtr->Size;
				//free(memPtr->ptr);
				//free(memPtr->lpszSource);
			}
			
			//free(memPtr);
			memPtr = memWalk;
		}

		//memTab->memTop[i] = NULL;
	}
   
   char Temp[1000];
   sprintf(Temp, "Total leak of %ld\r\n", dwTotalLeak);
   OutputDebugString(Temp);
   Log.Log(vir::LL_ERROR, Temp);

	//memPtr = memWalk = memTop = NULL;	
	
	//free(memTab);
	//memTab = NULL;

	//DeleteCriticalSection(&cs);
}

DWORD
MemManager::takeofs(void *ptr)
{
	DWORD ofs = ((DWORD)ptr & 0xffff) >> 6;
	return ofs;
}

void *
MemManager::Create(int size, char *lpszSource, int iLine )
// Create a new entry.
{
	if (!memTab)
		return NULL;
  
   void *ptr = malloc(size);

	DWORD addr = takeofs(ptr);

	if (memTab->memTop[ addr ]) {
		memWalk = memTab->memTop[ addr ];
		while(memWalk->Next)
			memWalk = memWalk->Next;
		memWalk->Next = (MemWalk *)malloc(sizeof(MemWalk));
		ZeroMemory(memWalk->Next, sizeof(MemWalk));
		memPtr = memWalk->Next;
	}
	else {
		memTab->memTop[ addr ] = (MemWalk *)malloc(sizeof(MemWalk));
		ZeroMemory(memTab->memTop[ addr ], sizeof(MemWalk));
		memWalk = memTab->memTop[ addr ];
		memPtr = memWalk;
	}
  
	memPtr->ptr = ptr;
  	dwTotalAlloc += size;
	memPtr->Size = size;
	memPtr->iLine = iLine;
	memPtr->lpszSource = (char *)malloc(strlen(lpszSource)+1);
	strcpy(memPtr->lpszSource, lpszSource);
	memPtr->Next = NULL;
	nItems ++;
	
	return ptr;
}

bool
MemManager::Change(void *oldptr, void *newptr, int size)
//
// Modify an entry (set new pointer & size)
//
// Return 1 on success
//		  0 on fail
//
{	
	DWORD addr = takeofs(oldptr);
	DWORD newaddr = takeofs(newptr);

	if (!memTab)
		return false;

	memOld = NULL;
	memPtr = memTab->memTop [ addr ];
	
	if (!memPtr)
		return false;

	while(memPtr->Next || memPtr->ptr == oldptr) {
		if (memPtr->ptr == oldptr) {
			if (addr != newaddr) {
				// Move bank
				memWalk = memTab->memTop [ newaddr ];
				if (!memWalk) {
					// Create entry
					memTab->memTop [ newaddr ] = (MemWalk *)malloc(sizeof(MemWalk));
					ZeroMemory(memTab->memTop [ newaddr ], sizeof(MemWalk));
					memWalk = memTab->memTop [ newaddr ];
				}
				else {
					// Create simple node
					memWalk = memTab->memTop [ newaddr ];
					while(memWalk->Next)
						memWalk = memWalk->Next;
					memWalk->Next = (MemWalk *)malloc(sizeof(MemWalk));
					memWalk = memWalk->Next;
					ZeroMemory(memWalk, sizeof(MemWalk));
				}
				if (memOld == NULL) {
					// First entry, kill it and set to Next
					memOld = memPtr->Next;
					free(memPtr);
					memTab->memTop[ addr ] = memOld;					
				}
				else {
					// Simply chop old entry
					memOld->Next = memPtr->Next;
					free(memPtr);
				}
				memWalk->ptr = newptr;
				memWalk->Size = size;
				memWalk->Next = NULL;
				return true;
			}
			else {
				memPtr->ptr = newptr;
				memPtr->Size = size;
				return true;
			}
		}
		memOld = memPtr;
		memPtr = memPtr->Next;
	}
	return false;
}

void
MemManager::Free(void *ptr)
// 
// Free an allocated pointer entry
//
{
	MemWalk *memPrev = NULL, *memNext = NULL;
	DWORD addr = takeofs(ptr);
	int found=0;

   if (!memTab) {
   	free(ptr);	
		return;
   }
	
	memPtr = memTab->memTop[ addr ];
   if (!memPtr) {
   	free(ptr);	
		return;
   }

	while(memPtr->Next || memPtr->ptr == ptr) {
		if (memPtr->ptr == ptr) {
			memPtr->ptr = NULL;
      	dwTotalAlloc -= memPtr->Size;
			memPtr->Size = 0;
			memNext = memPtr->Next;
			if (memPrev) 
				memPrev->Next = memNext;
			else {
				memTab->memTop[ addr ] = memNext;
			}
			free(memPtr->lpszSource);
			free(memPtr);
			found = 1;
			nItems --;
			break;
		}
		memPrev = memPtr;
		memPtr = memPtr->Next;
	}

	free(ptr);	
}

DWORD MemManager::GetAllocMem(void) {
   return dwTotalAlloc;
}

void MemManager::Dump(void) {
   Release();
}

*/