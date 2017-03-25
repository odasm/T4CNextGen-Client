//#include "pch.h"
#include <windows.h>
#include "MemMapFile.h"
#include "App.h"
/*#include "c2.h"
#include "PowerDebug.h"

#include "Global.h"
#include "MemoryManager.h"

*/

MemMapFile::MemMapFile(void) 
{
   m_pfFile = NULL;
   m_bXOR   = true;
   XOR.CreateRandom(0, 255, 666666);
}

MemMapFile::~MemMapFile(void) 
{
	Release();
}

bool MemMapFile::Create(LPTSTR FileName,bool bXOR) 
{
   m_dwRelativeOffset = 0;
   m_bXOR = bXOR;
   if(!m_bXOR)
      m_dwRelativeOffset = 4;
	m_pfFile = fopen(FileName, "rb");
	if (!m_pfFile) 
   {
		return false;
	}
   fseek(m_pfFile, 0, SEEK_END);
   m_qwFileSize = ftell(m_pfFile);
   fseek(m_pfFile, 0, SEEK_SET);
   m_OldOffset = 0;
   return true;
}

void MemMapFile::Release(void) 
{
   if (m_pfFile)
      fclose(m_pfFile);
   m_pfFile = NULL;
}

void MemMapFile::CpyMemory(void *ptr, DWORD dwSize, QWORD qwFileOffset) 
{

   qwFileOffset+=m_dwRelativeOffset;
   
	if (qwFileOffset+(unsigned __int64)dwSize > m_qwFileSize) 
   {
		DebugBreak();
		g_App.SetError(1, "Error in Database Index.");
		g_App.Close("Error in Database Index.");
		return;
	}
	
	UINT i = 0;

   if (m_qwFileSize >= (qwFileOffset+(QWORD)dwSize)) 
   {
      if (m_OldOffset != qwFileOffset) 
      {
         fseek(m_pfFile, qwFileOffset, SEEK_SET);
      }
      fread((LPBYTE)ptr, dwSize, 1, m_pfFile);
      if(m_bXOR)
      {
         for (i = 0; i < dwSize; i++) 
         {
            ((LPBYTE)ptr)[i] = ((LPBYTE)ptr)[i] ^ XOR((qwFileOffset+i)/4096);
         }
      }
      m_OldOffset = qwFileOffset+dwSize;
   } 
   else 
   {
      DebugBreak();
      g_App.SetError(1, "Error in Database Index.");
      g_App.Close("Error in Database Index.");
   }
}
