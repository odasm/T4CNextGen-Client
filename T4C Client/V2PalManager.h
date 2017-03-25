// *******************************************************************************************
#ifndef __V2_Palette_H__
#define __V2_Palette_H__

#include <windows.h>
#include "Sprite.h"




class CV2PalManager  
{
public:
   typedef struct _PalInfo 
   {
      char lpszID[64];
      char lpSpritePal[256*3];
   }PalInfo ;

   
	CV2PalManager();
	virtual ~CV2PalManager();

   LPBYTE GetPal(const char *, int);
   LPBYTE GetPalIndex(const char *, int) ;
   static CV2PalManager *GetInstance(void);
   
   Sprite m_plRefPal;
   PalInfo *m_pV2Palette;
   UINT     m_dwNbrPalette;

   

protected:
   unsigned char CalcChecksumComp2(unsigned char	*pData, unsigned long dwNbrData);
};

#endif
