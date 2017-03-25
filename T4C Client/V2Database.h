// *******************************************************************************************
#ifndef __V2_Database_H__
#define __V2_Database_H__

// Include File.
#include "V2Sprite.h"
#pragma warning( disable : 4786 )
#include <map>
#include <functional>

#define NB_FILES 20

// DATABASE ERR
#define V2DB_OK			   0 // Okay
#define V2DBERR_IDNOTFOUND   1 // ID Not Found

// Database Type
#define V2DB_NONE    0
#define V2DB_SPRITE  1
#define V2DB_PALETTE 2


#define NAME_LENGTH 64
#define PATH_LENGTH 256

typedef struct _sHeaderFile
{
   char         strNom[NAME_LENGTH];
   char         strPath[PATH_LENGTH];
   unsigned int dwFileOffset;
   unsigned int dwDataFileIndex;
   unsigned int dwThisPosIndex;
} sHeaderFile;

typedef struct _sDataFile
{
   unsigned short	dwCompType;	 // 1=unpack 2=pack 9 =Zip
   unsigned short	flag1;		 
   unsigned short	dwWidth;		 
   unsigned short	dwHeight; 	 
   short			   shOffX1;		 
   short			   shOffY1;		 
   short			   shOffX2;		 
   short			   shOffY2;		 
   unsigned short	ushTransparency;
   unsigned short	ushTransColor;	// Index de la couleur transparente
   unsigned long	dwDataUnpack;
   unsigned long	dwDataPack;
   unsigned char	*pstrData;
} sDataFile,LPsDataFile;

typedef struct _sHeaderFileMem
{
   unsigned char   chNbrNom;
   char            *pstrNom;
   unsigned int    dwFileOffset;
   unsigned int    dwCount;
   unsigned int    dwDataFileIndex;
   LPV2SPRITEINFO  lpSpriteInfo;
} DATAHEADER,*LPDATAHEADER;


class MemMapFile;

// ******************************************************************************************* 
typedef class CV2Database 
{
// ******************************************************************************************* 
// The CV2Database for the V2SpriteFile.
//
   public:
      CV2Database();			// CV2Database Constructor.
		~CV2Database();		// CV2Database Destructor.
      void Release(void);
      void Release( LPCTSTR lpszID);
      void LoadIndex(void);

      DWORD LoadSprite    (LPCTSTR lpszID, LPV2SPRITEINFO *lplpSpriteInfo, DWORD dwRelativeIndex,LPBYTE lpPal, int iPalIndex); // Load a Sprite Type.
      void  LoadSprite_NCK(LPV2SPRITEINFO *, LPDATAHEADER ii,LPCTSTR lpszID,LPBYTE lpPal, int iPalIndex, int dwDataCompIndex);	// Load a g_V2Database.LoadSprite No Color Keying.
      void  LoadSprite_ZIP(LPV2SPRITEINFO *, LPDATAHEADER ii,LPCTSTR lpszID,LPBYTE lpPal, int iPalIndex, int dwDataCompIndex);	// Load a g_V2Database.LoadSprite No Color Keying.
      void  LoadSprite_DD (LPV2SPRITEINFO *, LPDATAHEADER ii, int dwDataCompIndex);	// Load a g_V2Database.LoadSprite RAW,

      void  LoadSpriteUnpack_DD(LPV2SPRITEINFO *,LPDATAHEADER ii,BYTE *pchUnpackData,LPCTSTR lpszID,LPBYTE lpPal);
      void  GetOffsetAndDB(LPSTR, long &dwOffset, char &bDB );

   public:
      MemMapFile *m_pDataBuffer;
      bool       m_bDataBufferLoaded[NB_FILES];
      int        m_dwLastFileindexLoaded;


   protected:
      unsigned char  CalcChecksumComp2(unsigned char	*pData, unsigned long dwNbrData);


   protected:
      BOOL m_bIndexLoaded;



	private:
		CRITICAL_SECTION csThreadLock; 

	public:
      
} V2DATABASE, *LPV2DATABASE;

// Unique Global
extern V2DATABASE g_V2Database;

#endif
  
