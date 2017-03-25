/*
#ifndef __MEMORY_MANAGER_H
#define __MEMORY_MANAGER_H

#include <windows.h>

#define DEBUG_NEW	 new    (__FILE__, __LINE__)

#define MEMTAB_SIZE			1024

void *operator new(size_t size, char *ptr, int i);
void *operator new(size_t size);
void operator delete( void * );

class MemManager {
	friend void *operator new(size_t, char *, int);
	friend void *operator new(size_t);
	friend void operator delete( void * );

	public:
		static MemManager *GetInstance(void); // Get Instance.
      virtual ~MemManager(void);	// Destructor.
      DWORD GetAllocMem(void);
      void Dump(void);

	protected:
		class MemWalk {
			public:
				void    *ptr;			// Pointer allocated
				size_t  Size;			// Its size
				char    *lpszSource;	// Source file
				int		iLine;			// Source line
				MemWalk *Next;			// Next in list
		};

		class MemTab {
			public:
				MemWalk *memTop[MEMTAB_SIZE]; // low 16-bits >> 6 table
		};

		DWORD dwTotalAlloc;
      
      CRITICAL_SECTION cs;
      bool Init;
		
		MemWalk *memTop;
		MemWalk *memWalk;
		MemWalk *memPtr;
		MemWalk *memOld;

		MemTab  *memTab;
		
		int nItems;
	
		MemManager(void);			// Constructor.

      DWORD takeofs(void *ptr);	// Return offset.
      void Initialize(void);    // Init.
      void Release(void);        // Release.

		void *Create( int size, char *, int );				  // New
		bool Change( void *oldptr, void *newptr, int size );  // Realloc
		void Free(void *ptr);								  // Delete
	
};

#define new			DEBUG_NEW
#endif

*/