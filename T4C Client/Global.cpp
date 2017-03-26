// Global.cpp: implementation of the Global class.
//
//////////////////////////////////////////////////////////////////////
#include "Global.h"
#include <stdio.h>
#include <shlobj.h>
#include "TileSet.h"
#include "..\Zlib/zlib.h"

#include "SaveGame.h"
extern CSaveGame g_SaveGame; 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define RGBNM(r,g,b)          ((unsigned long)(((unsigned char)(r)|((unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))
 

#define ZONE_MAP_X   3072
#define ZONE_MAP_Y   3072
#define ZONE_VIEW_X  3072//512
#define ZONE_VIEW_Y  3072//512
#define ZONE_VIEW_X2 (ZONE_VIEW_X/2)
#define ZONE_VIEW_Y2 (ZONE_VIEW_Y/2)


Global g_Global;

Global::Global()
{
   SetDisplaySize(1280,720); // Default Resolution 1280x720
   m_dwMainTorchW   = 320;
   m_dwMainTorchH   = 240;

   m_crBtnEText = RGBNM(223, 157, 0);
   m_crBtnDText = RGBNM(150, 90, 0);

   m_dwAlphaValue       = 200;
   m_dwLastScrollStatus = 0;
   m_dwZoomStatus       = 0;
   m_dwAnimWaterCount   = 0;
   m_dwAnimLavaCount   = 0;

   //sprintf(m_ZoneMapInfo[0].strMapName,"ZWorldmap.bmp");
   m_ZoneMapInfo[0].bEnable = true;
   m_ZoneMapInfo[1].bEnable = false;
   m_ZoneMapInfo[2].bEnable = false;
   m_ZoneMapInfo[3].bEnable = false;
   m_ZoneMapInfo[4].bEnable = false;
   m_ZoneMapInfo[5].bEnable = false;
   m_ZoneMapInfo[6].bEnable = false;
   m_ZoneMapInfo[7].bEnable = false;
   m_dwCurMapsZone = -1;
   m_dwCurPosZone  = -1;
   m_dwChangeX     = -1;
   m_dwChangeY     = -1;
   //m_dwLoadZoneXS  = -1;
   //m_dwLoadZoneYS  = -1;
   m_bCanDisplayChange  = true;
   m_bDisplayChangeZone = false;
   m_bForceDisplayZone  = false;

   for(int n=0;n<NBR_ZONE_MAP;n++)
   {
      for(int i=0;i<256;i++)
      {
         sprintf(m_ZoneInfo[n][i].strZoneName,"");
         m_ZoneInfo[n][i].bEnable = false;
      }
   }

   sprintf(m_ZoneInfo[0][0].strZoneName,"LightHaven");      //2932,1067,0
   m_ZoneInfo[0][0].bEnable = true;
   sprintf(m_ZoneInfo[0][1].strZoneName,"Windhowl");        //1778,1317,0
   m_ZoneInfo[0][1].bEnable = true;
   sprintf(m_ZoneInfo[0][2].strZoneName,"Silversky");       //1486,2425,0
   m_ZoneInfo[0][2].bEnable = true;
   sprintf(m_ZoneInfo[0][3].strZoneName,"Stonecrest");      //225,732,0
   m_ZoneInfo[0][3].bEnable = true;
   sprintf(m_ZoneInfo[0][4].strZoneName,"Goblin Camp");     //2455,775,0
   m_ZoneInfo[0][4].bEnable = true;
   sprintf(m_ZoneInfo[0][5].strZoneName,"Jarko Mountain");  //2144,240,0
   m_ZoneInfo[0][5].bEnable = true;
   sprintf(m_ZoneInfo[0][6].strZoneName,"Tarantulas Lake"); //790,2060,0
   m_ZoneInfo[0][6].bEnable = true;

   m_pchZoneMaps = new unsigned char[ZONE_MAP_X*ZONE_MAP_Y];

   m_bConfigRead = false;

   ReadClientConfig();
   ClientConfig *pConfig = g_Global.GetClientConfig();
   sprintf(m_strPath,"%s",pConfig->strPath);
   sprintf(m_ZoneFile,"%s%s",m_strPath,"\\Game Files\\Zone_Map.dat");


   m_pItemInfo = new sItemInfo;
   m_pItemInfo->Name       = NULL;
   m_pItemInfo->shNbrBoust = 0;
   m_pItemInfo->pBoust     = NULL;
   m_pItemInfo->bSet       = false;

   m_dwLastWorldX = 0;
   m_dwLastWorldY = 0;

   InitializeCriticalSection(&csInternal);
}

Global::~Global()
{
   if(m_pchZoneMaps)
      delete []m_pchZoneMaps;
   m_pchZoneMaps = NULL;

   if(m_pItemInfo->Name)
      delete []m_pItemInfo->Name;
   m_pItemInfo->Name = NULL;
   if(m_pItemInfo->pBoust)
      delete []m_pItemInfo->pBoust;
   m_pItemInfo->pBoust = NULL;
   m_pItemInfo->shNbrBoust = 0;
   m_pItemInfo->bSet = false;

   delete m_pItemInfo;
   //WriteClientConfig();
   DeleteCriticalSection(&csInternal);
}

void Global::SetDisplaySize(int dwW, int dwH)
{
   m_dwDisplayW     = dwW;
   m_dwDisplayH     = dwH;
   m_dwDisplayWDiv2 = m_dwDisplayW/2;
   m_dwDisplayHDiv2 = m_dwDisplayH/2;

   m_dwNbrTileX     = m_dwDisplayW/32;
   m_dwNbrTileY     = m_dwDisplayH/16;

   if(m_dwNbrTileY%2)
   {
      m_dwNbrTileXDiv2 = m_dwNbrTileX/2+1;
      m_dwNbrTileYDiv2 = m_dwNbrTileY/2+1;
   }
   else
   {
      m_dwNbrTileXDiv2 = m_dwNbrTileX/2;
      m_dwNbrTileYDiv2 = m_dwNbrTileY/2;
   }

   m_dwScreenW      = m_dwNbrTileX*32;
   m_dwScreenH      = m_dwNbrTileY*16;
   m_dwScreenWDiv2  = m_dwScreenW/2;
   m_dwScreenHDiv2  = m_dwScreenH/2;
}

void Global::SetZoomStatus(int dwIncVal)
{
   int dwZoomTmp = m_dwZoomStatus += dwIncVal;
   if(dwZoomTmp < 0)
      dwZoomTmp = 0;
   else if(dwZoomTmp > 14)
      dwZoomTmp = 14;

   if(!g_SaveGame.GetOptionsParam()->bEnableZoom)
      dwZoomTmp = 0;
   m_dwZoomStatus = dwZoomTmp;
}

void Global::StepAnimWaterFrame()
{
   m_dwAnimWaterCount++;
   if(++m_dwAnimWaterCount >=T_AWATER01_N)
      m_dwAnimWaterCount = 0;
}

/*
void Global::StepAnimLavaFrame()
{
   m_dwAnimLavaCount++;
   if(++m_dwAnimLavaCount >=T_ALAVA01_N)
      m_dwAnimLavaCount = 0;
}
*/

void Global::LoadZoneMapWorld(int dwWorld, int dwX, int dwY)
{
   if(dwWorld <0 ||  dwWorld >NBR_ZONE_MAP-1)
      dwWorld = 0;

   if(dwWorld == m_dwCurMapsZone)
      return;
   m_dwCurMapsZone = dwWorld;
   
   /*
   int dwX1 = dwX;
   int dwY1 = dwY;

   if(dwX1 >= ZONE_MAP_X)
      dwX1 = ZONE_MAP_X-1;

   if(dwY1 >= ZONE_MAP_Y)
      dwY1 = ZONE_MAP_Y-1;

   if( (dwWorld == m_dwCurMapsZone) && 
       (dwX >= m_dwLoadZoneXS && dwX1 <m_dwLoadZoneXS+ZONE_VIEW_X && dwY >= m_dwLoadZoneYS && dwY1 <m_dwLoadZoneYS+ZONE_VIEW_Y ))
      return;
   
   //if(dwWorld == m_dwCurMapsZone)
   //   return;

   m_dwCurMapsZone = dwWorld;
   m_dwLoadZoneXS  = dwX-ZONE_VIEW_X2;
   m_dwLoadZoneYS  = dwY-ZONE_VIEW_Y2;

   if(m_dwLoadZoneXS <0)
      m_dwLoadZoneXS = 0;
   else if(m_dwLoadZoneXS > ZONE_MAP_X-ZONE_VIEW_X)
      m_dwLoadZoneXS = ZONE_MAP_X-ZONE_VIEW_X;

   if(m_dwLoadZoneYS <0)
      m_dwLoadZoneYS = 0;
   else if(m_dwLoadZoneYS > ZONE_MAP_Y-ZONE_VIEW_Y)
      m_dwLoadZoneYS = ZONE_MAP_Y-ZONE_VIEW_Y;
   */

   FILE *pf1 = fopen(m_ZoneFile,"rb");
   if(!pf1)
   {
      memset(m_pchZoneMaps,0xFF,ZONE_VIEW_X*ZONE_VIEW_Y);
   }
   else
   {

      ULONG dwTailleOri;
      ULONG dwTailleComp[NBR_ZONE_MAP];
      ULONG dwFilePos[NBR_ZONE_MAP];

      fread(&dwTailleOri,4,1,pf1);
      fread(&dwTailleComp[0],4,1,pf1);
      fread(&dwTailleComp[1],4,1,pf1);
      fread(&dwTailleComp[2],4,1,pf1);
      fread(&dwTailleComp[3],4,1,pf1);
      fread(&dwTailleComp[4],4,1,pf1);
      fread(&dwTailleComp[5],4,1,pf1);
      fread(&dwTailleComp[6],4,1,pf1);
      fread(&dwTailleComp[7],4,1,pf1);
      fread(&dwFilePos[0],4,1,pf1);
      fread(&dwFilePos[1],4,1,pf1);
      fread(&dwFilePos[2],4,1,pf1);
      fread(&dwFilePos[3],4,1,pf1);
      fread(&dwFilePos[4],4,1,pf1);
      fread(&dwFilePos[5],4,1,pf1);
      fread(&dwFilePos[6],4,1,pf1);
      fread(&dwFilePos[7],4,1,pf1);

      fseek( pf1, dwFilePos[dwWorld], SEEK_SET);

      BYTE *pFileDataTmpC = NULL;
      pFileDataTmpC= new BYTE[dwTailleComp[dwWorld]];
      fread(pFileDataTmpC,1,dwTailleComp[dwWorld],pf1);
      fclose(pf1);

      BYTE *pFileDataTmp = NULL;
      pFileDataTmp = new BYTE[dwTailleOri];

      //BYTE *pTMIPal = new BYTE[256*3];
      if(uncompress((BYTE*)pFileDataTmp,&dwTailleOri,(BYTE*)pFileDataTmpC,dwTailleComp[dwWorld]) != Z_OK)
      {
         memset(m_pchZoneMaps,0xFF,ZONE_MAP_X*ZONE_MAP_Y);
      }
      else
      {
         BYTE *WPosPal = pFileDataTmp+(ZONE_MAP_X*ZONE_MAP_Y);

         memcpy(m_pchZoneMaps,pFileDataTmp,ZONE_MAP_X*ZONE_MAP_Y);
         //memcpy(pTMIPal,WPosPal,256*3);
      }

      //char strTmp[512];
      //sprintf(strTmp,"c:\\!!__NAMEWorld%d.bmp",dwWorld);
      //SaveCurrentImage8(m_pchZoneMaps,pTMIPal,ZONE_MAP_X, ZONE_MAP_Y,strTmp,false);

      //delete []pTMIPal;
      //pTMIPal = NULL;
      delete []pFileDataTmp;
      pFileDataTmp = NULL;
      delete []pFileDataTmpC;
      pFileDataTmpC = NULL;

      /*
      BYTE *pFileDataTmp = NULL;
      pFileDataTmp = new BYTE[dwTailleOri];

      BYTE *pRTMap = new BYTE[ZONE_MAP_X*ZONE_MAP_Y];
      
      if(uncompress((BYTE*)pFileDataTmp,&dwTailleOri,(BYTE*)pFileDataTmpC,dwTailleComp[dwWorld]) != Z_OK)
      {
         memset(pRTMap,0x00,ZONE_MAP_X*ZONE_MAP_Y);
      }

      BYTE *WPosMap = pFileDataTmp;
      memcpy(pRTMap,WPosMap,ZONE_MAP_X*ZONE_MAP_Y);

      // maintenant on doit extraire la zone qui nous interresse...
      for(int y=0;y<ZONE_VIEW_Y;y++)
      {
         if((y+m_dwLoadZoneYS) < ZONE_MAP_Y)
         {
            int dwOffset = ((y+m_dwLoadZoneYS)*ZONE_MAP_X);
            dwOffset+=m_dwLoadZoneXS;
            BYTE *pS = pRTMap  +dwOffset;
            BYTE *pD = m_pchZoneMaps +(y*ZONE_VIEW_X);

            memcpy(pD,pS,ZONE_VIEW_X);
         }
         else
         {
            BYTE *pD = m_pchZoneMaps +(y*ZONE_VIEW_X);
            memset(pD,0x00,ZONE_VIEW_X);
         }
      }

      //SaveCurrentImage8(m_pchZoneMaps,ZONE_VIEW_X, ZONE_VIEW_Y,"C:\\!!!OHOH.bmp",FALSE);

      delete []pRTMap;
      pRTMap = NULL;
      delete []pFileDataTmp;
      pFileDataTmp = NULL;
      delete []pFileDataTmpC;
      pFileDataTmpC = NULL;
      */
   }


   /*
   if(m_ZoneMapInfo[m_dwCurMapsZone].bEnable)
   {
      char strZoneMap[512];
      sprintf(strZoneMap,"%s%s%s",m_strPath,"\\Game Files\\",m_ZoneMapInfo[m_dwCurMapsZone].strMapName);
      LoadImageDataToBmpFile(strZoneMap,m_pchZoneMaps);
   }
   */


}

int Global::GetDisplayZone()
{
   if(m_bDisplayChangeZone || m_bForceDisplayZone)
   {
      m_bDisplayChangeZone = false;
      m_bForceDisplayZone  = false;
      return m_dwCurPosZone;
   }
   else
      return -1;
}
char* Global::GetDisplayZoneName()
{
   return m_ZoneInfo[m_dwCurMapsZone][m_dwCurPosZone].strZoneName;
}

void Global::ValidMapZonePosition(int dwPosX, int dwPosY)
{
   // non actif ou changement deja en cours...
   if(!m_ZoneMapInfo[m_dwCurMapsZone].bEnable || m_bDisplayChangeZone)
      return;

   // on valide si ya eu un deplacement de aumoins 5 case en x et en y, si la personne reste
   // a 5 case de la zone... ben on affiche pas elle est pas aller assez loin...
   if(m_dwChangeX != -1 && m_dwChangeY != -1 && !m_bCanDisplayChange)
   {
      if((abs(dwPosX -m_dwChangeX) >5) && (abs(dwPosY -m_dwChangeY) >5))
         m_bCanDisplayChange = true;
   }

   // on affiche pas changement don pas la peine aller plus loin...
   if(!m_bCanDisplayChange)
      return;

  
   // 
   if(m_pchZoneMaps[(dwPosY*3072)+dwPosX] != m_dwCurPosZone)
   {
      m_dwCurPosZone = m_pchZoneMaps[(dwPosY*3072)+dwPosX];
      m_dwChangeX    = dwPosX;
      m_dwChangeY    = dwPosY;
      if(m_ZoneInfo[m_dwCurMapsZone][m_dwCurPosZone].bEnable)
      {
         m_bCanDisplayChange  = false;
         m_bDisplayChangeZone = true;
      }
   }

}

void Global::ForceDisplayZone(int dwPosX, int dwPosY)
{
   if(!m_ZoneMapInfo[m_dwCurMapsZone].bEnable)
      return;
   m_dwCurPosZone = m_pchZoneMaps[(dwPosY*3072)+dwPosX];
   //m_dwCurPosZone = m_pchZoneMaps[((dwPosY-m_dwLoadZoneYS)*ZONE_VIEW_X)+(dwPosX-m_dwLoadZoneXS)];
   m_dwChangeX    = dwPosX;
   m_dwChangeY    = dwPosY;
   if(m_ZoneInfo[m_dwCurMapsZone][m_dwCurPosZone].bEnable)
   {
      
      m_bCanDisplayChange  = false;
      m_bDisplayChangeZone = true;
   }
}


int Global::LoadImageDataToBmpFile(char * lpszFileName,unsigned char *pData) 
{
   void   *lpBits; 
   HANDLE hSection = NULL;
   unsigned long  dwOffset = 0;
   
   FILE *file;
   file = fopen( lpszFileName, "rb");
   if( file ==NULL )
      return NULL;
   
   BITMAPFILEHEADER bmfHeader;
   long nFileLen;
   
	fseek(file,0,SEEK_END);
	nFileLen = ftell(file);
	fseek(file,0,SEEK_SET);
   
   // Read file header
   if (fread((LPSTR)&bmfHeader,1, sizeof(bmfHeader),file) != sizeof(bmfHeader))
      return NULL;
   
   // File type should be 'BM'
   if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
   {
      fclose(file);
      return NULL;
   }
   
   BITMAPINFO *pbmInfo;
   pbmInfo = (BITMAPINFO *)::GlobalAlloc(GMEM_FIXED, 
      sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256 );
   if (pbmInfo == NULL)
   {
      fclose(file);
      return NULL;
   }
   
   // Read the BITMAPINFO
   fread( pbmInfo, 1,sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256,file );
   
   BITMAPINFO &bmInfo = *pbmInfo ;
   
   HBITMAP hBmp = CreateDIBSection( NULL, pbmInfo, DIB_RGB_COLORS, &lpBits, 
						hSection, dwOffset );
   
   LPBYTE  lpDIBBits;              // Pointer to DIB bits
   int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
   1 << bmInfo.bmiHeader.biBitCount;
   
   if( bmInfo.bmiHeader.biBitCount > 8 )
      lpDIBBits = (LPBYTE)((LPDWORD)(bmInfo.bmiColors + 
      bmInfo.bmiHeader.biClrUsed) + 
      ((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
   else
      lpDIBBits = (LPBYTE)(bmInfo.bmiColors + nColors);
   
   if(bmInfo.bmiHeader.biWidth != 3072 || bmInfo.bmiHeader.biHeight != 3072 || bmInfo.bmiHeader.biBitCount != 8)
   {
      ::GlobalFree(pbmInfo);
      //delete []pMapTmp;
      DeleteObject(hBmp);
      fclose(file);
      return 0;
   }
   
   int nOffset = sizeof(BITMAPFILEHEADER) + (lpDIBBits - (LPBYTE)pbmInfo);
   fseek(file,nOffset,SEEK_SET);
   fread((LPSTR)lpBits, 1,nFileLen - nOffset,file);
   
 
   memcpy(pData,lpBits,nFileLen - nOffset);
   ::GlobalFree(pbmInfo);
   //delete []pMapTmp;
   DeleteObject(hBmp);
   fclose(file);
   return 0;
} 

void Global::Lock(void) 
{
	EnterCriticalSection(&csInternal);
}

void Global::Unlock(void) 
{
	LeaveCriticalSection(&csInternal);
}

void Global::ResetItemInfo()
{
   EnterCriticalSection(&csInternal);
   if(m_pItemInfo->Name)
      delete []m_pItemInfo->Name;
   m_pItemInfo->Name = NULL;
   if(m_pItemInfo->pBoust)
      delete []m_pItemInfo->pBoust;
   m_pItemInfo->pBoust = NULL;
   m_pItemInfo->shNbrBoust = 0;
   m_pItemInfo->bSet = false;
   LeaveCriticalSection(&csInternal);
}

void Global::SetItemInfoID(ULONG ulID)
{
   EnterCriticalSection(&csInternal);
   m_pItemInfo->ulID = ulID;
   LeaveCriticalSection(&csInternal);
}
void Global::SetItemInfoInventory(bool bInventory)
{
   EnterCriticalSection(&csInternal);
   m_pItemInfo->bInventory = bInventory;
   LeaveCriticalSection(&csInternal);
}

int Global::SaveCurrentImage8(BYTE *pshImage,BYTE *pPal,int dwWidth, int dwHeight,char *pstrName,BOOL bFlipV)
{
   int dwBufSize = dwWidth*dwHeight;

   BYTE *pchImageByte = new BYTE[dwWidth*dwHeight];

   if(bFlipV)
   {
      int dwPosDest = 0;

      for(int y=dwHeight-1;y>=0;y--)
      {
         for(int x=0;x<dwWidth;x++)
         {
            pchImageByte[(y*dwWidth)+x] = (BYTE) pshImage[(dwPosDest*dwWidth)+x];
         }
         dwPosDest++;
      }
   }
   else
      memcpy(pchImageByte,pshImage,dwWidth*dwHeight);


   HANDLE hf = CreateFile(pstrName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,NULL,NULL);
   if(hf != INVALID_HANDLE_VALUE)
   {
      BITMAPFILEHEADER bfh;
      memset(&bfh,0,sizeof(bfh));
      bfh.bfType    = 'MB';
      bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER) +(sizeof(RGBQUAD)*256);
      bfh.bfSize    = bfh.bfOffBits + (1*dwWidth*dwHeight);

      DWORD dwWritten = 0;
      WriteFile(hf,&bfh,sizeof(bfh),&dwWritten,NULL);
      
      BITMAPINFOHEADER  bih;
      
      memset(&bih,0 , sizeof(bih));
      bih.biSize          = sizeof(bih);
      bih.biWidth         = dwWidth;
      bih.biHeight        = dwHeight;
      bih.biPlanes        = 1;
      bih.biCompression   = BI_RGB;
      bih.biSizeImage     = dwBufSize;
      bih.biBitCount      = 8;
      bih.biClrUsed       = 0;
      bih.biClrImportant  = 0;

      dwWritten = 0;
      WriteFile(hf,&bih,sizeof(bih),&dwWritten,NULL);

      ////////////////////////////////////////////
      // ecriture palette                
      unsigned char PalTmp[1024];
      int cntTmp = 0;
      int dwCnt2 = 0;
      for(int j=0;j<1024;j+=4)
      { 
         PalTmp[j+2] = pPal[dwCnt2++];
         PalTmp[j+1] = pPal[dwCnt2++];
         PalTmp[j+0] = pPal[dwCnt2++];
         PalTmp[j+3] = 0;
         /*
         PalTmp[j]   = (unsigned char)cntTmp;
         PalTmp[j+1] = (unsigned char)cntTmp;
         PalTmp[j+2] = (unsigned char)cntTmp;
         PalTmp[j+3] = (unsigned char)0;
         cntTmp++;
         */
      }
      
      dwWritten = 0;
      WriteFile(hf,&PalTmp,sizeof(PalTmp),&dwWritten,NULL);
      ////////////////////////////////////////////
      
      dwWritten = 0;
      WriteFile(hf,pchImageByte,dwBufSize,&dwWritten,NULL);
      
      CloseHandle(hf);
   }
   if(pchImageByte)
   {
      delete []pchImageByte;
      pchImageByte = NULL;
   }

   return 0;
}

ClientConfig *Global::GetClientConfig()
{
   ReadClientConfig();
   return &m_ClientConfig;
}

ClientConfig *Global::ReadClientConfig()
{
   if(m_bConfigRead)
      return &m_ClientConfig;

   //set le path de l<application
   char Path[MAX_PATH];
   GetModuleFileName(NULL, Path, MAX_PATH);
   int len = strlen(Path) - 1;
   while (len && Path[len] != '\\')
      len--;
   Path[len] = 0;
   sprintf(m_ClientConfig.strPath       ,"%s",Path);
   
   //set le path de save...
   if(SHGetSpecialFolderPath(NULL,m_ClientConfig.strSavePath,CSIDL_PERSONAL   ,FALSE) != 1)
      GetTempPath(MAX_PATH,m_ClientConfig.strSavePath);
   strcat(m_ClientConfig.strSavePath,"\\Dialsoft");
   _mkdir(m_ClientConfig.strSavePath);
   strcat(m_ClientConfig.strSavePath,"\\T4CV2");
   _mkdir(m_ClientConfig.strSavePath);

   char strConfig[1024];
   sprintf(strConfig,"%s\\T4C.dat",m_ClientConfig.strSavePath);

   FILE *pf1 = fopen(strConfig,"rb");
   if(!pf1)
   {
      sprintf(m_ClientConfig.strAccountName,"");
      sprintf(m_ClientConfig.strAccountIP  ,"");
      m_ClientConfig.dwAfkStatus = 0;
      sprintf(m_ClientConfig.strAfkMessage ,"");
      m_ClientConfig.dwDebug          = 0;
      m_ClientConfig.dwNMDebug        = 0;
      m_ClientConfig.dwNMShowFPS      = 0;
      m_ClientConfig.dwNMShowPOS      = 0;
      m_ClientConfig.dwNMFPSValue     = 16;
      m_ClientConfig.FirstTimeAddon   = 1;
      m_ClientConfig.WebpatchEnable   = 0;
      
   }
   else
   {
      char chHeader[23];
      fread(chHeader                       ,1,22,pf1);
      chHeader[22] = 0x00;
      if(strcmp(chHeader,"NM_GENERALT4C_CFG_V002") != 0)
      {
         sprintf(m_ClientConfig.strAccountName,"");
         sprintf(m_ClientConfig.strAccountIP  ,"");
         m_ClientConfig.dwAfkStatus = 0;
         sprintf(m_ClientConfig.strAfkMessage ,"");
         m_ClientConfig.dwDebug          = 0;
         m_ClientConfig.dwNMDebug        = 0;
         m_ClientConfig.dwNMShowFPS      = 0;
         m_ClientConfig.dwNMShowPOS      = 0;
         m_ClientConfig.dwNMFPSValue     = 16;
         m_ClientConfig.FirstTimeAddon   = 1;
         m_ClientConfig.WebpatchEnable   = 0;

         MessageBox(NULL,"Invalid T4C Configuration File...\nDefault Configuration Loaded !","Error !",MB_OK+MB_ICONSTOP);
      }
      else
      {
         fread(m_ClientConfig.strAccountName   ,1,50,pf1);
         fread(m_ClientConfig.strAccountIP     ,1,50,pf1);
         fread(&m_ClientConfig.dwAfkStatus     ,4,1,pf1);
         fread(m_ClientConfig.strAfkMessage    ,1,2048,pf1);
         fread(&m_ClientConfig.dwDebug         ,4,1,pf1);
         fread(&m_ClientConfig.dwNMDebug       ,4,1,pf1);
         fread(&m_ClientConfig.dwNMShowFPS     ,4,1,pf1);
         fread(&m_ClientConfig.dwNMShowPOS     ,4,1,pf1);
         fread(&m_ClientConfig.dwNMFPSValue    ,4,1,pf1);
         fread(&m_ClientConfig.FirstTimeAddon  ,4,1,pf1);
         fread(&m_ClientConfig.WebpatchEnable  ,4,1,pf1);
      }
      fclose(pf1);
   }
   
   m_bConfigRead = true;
   return &m_ClientConfig;
}

void Global::WriteClientConfig()
{
   char strConfig[1024];//BLBLBL 29 mars 2009, tentative déplacement de la save T4C.DAT
   sprintf(strConfig,"%s\\T4C.dat",m_ClientConfig.strSavePath);

   FILE *pf1 = fopen(strConfig,"wb+");
   if(pf1)
   {
      char chHeader[23];
      sprintf(chHeader,"NM_GENERALT4C_CFG_V002");
      fwrite(&chHeader                       ,1,strlen(chHeader),pf1);
      fwrite(m_ClientConfig.strAccountName   ,1,50,pf1);
      fwrite(m_ClientConfig.strAccountIP     ,1,50,pf1);
      fwrite(&m_ClientConfig.dwAfkStatus     ,4,1,pf1);
      fwrite(m_ClientConfig.strAfkMessage    ,1,2048,pf1);
      fwrite(&m_ClientConfig.dwDebug         ,4,1,pf1);
      fwrite(&m_ClientConfig.dwNMDebug       ,4,1,pf1);
      fwrite(&m_ClientConfig.dwNMShowFPS     ,4,1,pf1);
      fwrite(&m_ClientConfig.dwNMShowPOS     ,4,1,pf1);
      fwrite(&m_ClientConfig.dwNMFPSValue    ,4,1,pf1);
      fwrite(&m_ClientConfig.FirstTimeAddon  ,4,1,pf1);
      fwrite(&m_ClientConfig.WebpatchEnable  ,4,1,pf1);
      fclose(pf1);
   }
   /*else{
      MessageBox(NULL,"Cannot save Client informations T4C.dat !","Error !",MB_OK+MB_ICONSTOP);
   }*/
}