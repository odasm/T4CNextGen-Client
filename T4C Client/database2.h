#ifndef __DATA2
#define __DATA2

#include "MemMapFile.h"

typedef struct _VSBINFO {
   WORD wHertz;
   BYTE bBits;
   DWORD dwSize;
} VSBINFO, *LPVSBINFO;

struct VSBINDEXINFO {
   LPTSTR lpszID;
   DWORD dwOffset;
   DWORD dwSize;
   WORD wHertz;
   BYTE bBits;
   DWORD dwCount;
   LPVSBINFO lpvsbInfo;
};

class VSBDataBase {
   public:
      BOOL boIndexLoad;
      MemMapFile lpbDataBuffer;
      VSBINDEXINFO *IndexInfo;
      DWORD dwNbIndex;
      QWORD qwOffset;
      CRITICAL_SECTION csThreadLock;
   
      VSBDataBase(void);
      ~VSBDataBase(void);
      void LoadIndex(void);
      DWORD LoadWave(LPTSTR, LPVSBINFO *);
      DWORD LoadChunck(DWORD, LPVSBINFO *, DWORD, DWORD, LPBYTE);
      void Release(int);
};
// Unique Global
extern VSBDataBase ug_Database2_44444;

#define GetFromBuffer3(a, b) \
       lpbDataBuffer.CpyMemory(a, b, qwOffset); \
		 qwOffset += b;

#define GetFromBuffer4(a, b) \
      ug_Database2_44444.lpbDataBuffer.CpyMemory(a, b, ug_Database2_44444.qwOffset); \
      ug_Database2_44444.qwOffset += b;

#define DatabaseLoadVSB(a, b) \
   ug_Database2_44444.LoadWave(a, b);

#define DatabaseLoadVSBChunck(a, b, c, d, e) \
   ug_Database2_44444.LoadChunck(a, b, c, d, e);

#define DatabaseReleaseVSB(a) \
   ug_Database2_44444.Release(a);

#endif