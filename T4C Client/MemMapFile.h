#ifndef __MEMMAPFILE
#define __MEMMAPFILE

#include "C2.h"
#include "Random.H"
#include <stdio.h>

class MemMapFile 
{
	private:
	public:
		RandomTable<4096> XOR;
      FILE *m_pfFile;
		QWORD m_qwFileSize;
      int m_OldOffset;
      bool m_bXOR;
      int  m_dwRelativeOffset;

	public:
		MemMapFile(void);
		~MemMapFile(void);
		bool Create(LPTSTR,bool bXOR = true);
		void Release(void);
      void CpyMemory(void *ptr, DWORD dwSize, QWORD qwFileOffset);
};

#endif