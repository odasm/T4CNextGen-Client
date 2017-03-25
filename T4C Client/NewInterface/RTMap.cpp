// RTMap.cpp: implementation of the RTMap class.
//
//////////////////////////////////////////////////////////////////////
#include "../pch.h"

#include "RTMap.h"
#include "RootBoxUI.h"

#include "CharacterUI.h"
#include "ChatterUI.h"
#include "..\SaveGame.h"
#include "..\Zlib/zlib.h"

extern CSaveGame g_SaveGame;

#define NBR_RT_MAP     8


#define RT_VIEW_X  640
#define RT_VIEW_Y  448
//#define RT_VIEW_2X (3072*2)//(RT_VIEW_X*2)
//#define RT_VIEW_2Y (3072)//(RT_VIEW_Y*2)
#define RT_VIEW_X2 (RT_VIEW_X/2)
#define RT_VIEW_Y2 (RT_VIEW_Y/2)
#define RT_VIEW_X4 (RT_VIEW_X/4)
#define RT_VIEW_Y4 (RT_VIEW_Y/4)

#define RTMAP_X    (3072*2)
#define RTMAP_Y    (3072)

#define RTMAP_X2    (RTMAP_X/2)
#define RTMAP_Y2    (RTMAP_Y/2)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RTMap::RTMap()
{
   // Setup the chatter channel
   int dwX = (g_Global.GetScreenW()-RT_VIEW_X)/2;
   int dwY = (g_Global.GetScreenH()-150-RT_VIEW_Y)/2;
   if(dwY <0)
       dwY =0;

   SetPos( FramePos(dwX, dwY, dwX+RT_VIEW_X, dwY+RT_VIEW_Y) );
   RootBoxUI::GetInstance()->AddChild( this );

   m_pTMIData        = new WORD[RT_VIEW_X*RT_VIEW_Y]; // size of TMI mask...
   m_pTMIDataTmp     = new WORD[RT_VIEW_X2*RT_VIEW_Y2]; // size of TMI mask...

   ClientConfig *pConfig = g_Global.GetClientConfig();
   char strPath[512];
   sprintf(strPath,"%s",pConfig->strPath);
   sprintf(m_RTFile,"%s%s",strPath,"\\Game Files\\RT_Map.dat");

   //m_pRTMapS  = new BYTE[RT_VIEW_2X*RT_VIEW_2Y];


   //m_pRTMapS  = new BYTE[RTMAP_X*RTMAP_Y];
   m_pRTMapS  = NULL;
   m_pRTPal   = new BYTE[256*3];

   m_pMapImage  = new BYTE[RT_VIEW_X2*RT_VIEW_Y2];
   m_pMapImageM = new BYTE[((11*32)+16)*((15*16)+16)];
   

   m_MainMap      .SetSpriteId( "GUI_RTMapBack" );
   m_MainMap      .SetPos( FramePos(dwX, dwY, dwX+RT_VIEW_X, dwY+RT_VIEW_Y) );
   m_MainMap      .DrawAlpha(true);
   m_MainMapMask  .SetSpriteId( "GUI_RTMapMask" );
   m_MainMapMask  .SetPos( FramePos(dwX, dwY, dwX+RT_VIEW_X, dwY+RT_VIEW_Y) );
   m_rtMask       .SetSpriteId( "RT_MAP_Mask");

   m_MainMapPPos  .SetSpriteId( "GUI_back_TMI_PPos" );
   m_MainMapPPos  .SetPos( FramePos(dwX+268,dwY+169, dwX+268+116,dwY+169+113 ) );

   AddChild( &m_MainMap);
   AddChild( &m_MainMapMask);
   AddChild( &m_MainMapPPos);

   m_dwLoadX = -1;
   m_dwLoadY = -1;
   m_dwLoadW = -1;

   //m_dwLoadZoneXS = -1;
   //m_dwLoadZoneYS = -1;

   m_dwCurrentLoadedWorld = -1;

   Hide();
}

RTMap::~RTMap()
{
   if(m_pRTMapS)
      delete []m_pRTMapS;
   if(m_pRTPal)
      delete []m_pRTPal;
   m_pRTMapS = NULL;
   m_pRTPal  = NULL;
   
   if(m_pMapImage)
      delete []m_pMapImage;
   m_pMapImage = NULL;
   if(m_pMapImageM)
      delete []m_pMapImageM;
   m_pMapImageM = NULL;

   if(m_pTMIData)
      delete []m_pTMIData;
   m_pTMIData = NULL;
   if(m_pTMIDataTmp)
      delete []m_pTMIDataTmp;
   m_pTMIDataTmp = NULL;
}

void RTMap::LoadRTWorld(int iWorld,int dwX,int dwY)
{
   if(iWorld <0 ||  iWorld >NBR_RT_MAP-1)
      iWorld = 0;

   if(iWorld == m_dwCurrentLoadedWorld)
      return;
   m_dwCurrentLoadedWorld = iWorld;


   
   FILE *pf1 = fopen(m_RTFile,"rb");
   if(!pf1)
   {
      //memset(m_pRTMapS,0x00,RT_VIEW_2X*RT_VIEW_2Y);
      memset(m_pRTMapS,0x00,RTMAP_X*RTMAP_Y);
      memset(m_pRTPal,0x00,256*3);
   }
   else
   {

      ULONG dwTailleOri;
      ULONG dwTailleComp[NBR_RT_MAP];
      ULONG dwFilePos[NBR_RT_MAP];

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

      fseek( pf1, dwFilePos[iWorld], SEEK_SET);

      BYTE *pFileDataTmpC = NULL;
      pFileDataTmpC= new BYTE[dwTailleComp[iWorld]];
      fread(pFileDataTmpC,1,dwTailleComp[iWorld],pf1);
      fclose(pf1);

      BYTE *pFileDataTmp = NULL;
      pFileDataTmp = new BYTE[dwTailleOri];


	  if(uncompress((BYTE*)pFileDataTmp,&dwTailleOri,(BYTE*)pFileDataTmpC,dwTailleComp[iWorld]) != Z_OK)
	  {
		 memset(m_pRTMapS ,0x00,RTMAP_X*RTMAP_Y);
		 memset(m_pRTPal,0x00,256*3);
	  }
	  else
	  {
		 BYTE *WPosMap = pFileDataTmp;
		 BYTE *WPosPal = pFileDataTmp+(RTMAP_X*RTMAP_Y);
		 memcpy(m_pRTMapS,WPosMap,RTMAP_X*RTMAP_Y);
		 memcpy(m_pRTPal,WPosPal,256*3);
	  }
	  

	  //char strTmp[512];
	  //sprintf(strTmp,"c:\\!!__RTWorld%d.bmp",iWorld);
	  //SaveCurrentImage8(m_pRTMapS,m_pRTPal,RTMAP_X, RTMAP_Y,strTmp,false);
  
	  delete []pFileDataTmp;
	  pFileDataTmp = NULL;
	  delete []pFileDataTmpC;
	  pFileDataTmpC = NULL;

	  
      /*
      BYTE *pRTMap = new BYTE[RTMAP_X*RTMAP_Y];
      
      if(uncompress((BYTE*)pFileDataTmp,&dwTailleOri,(BYTE*)pFileDataTmpC,dwTailleComp[iWorld]) != Z_OK)
      {
         memset(pRTMap ,0x00,RTMAP_X*RTMAP_Y);
         memset(m_pRTPal,0x00,256*3);
      }

      BYTE *WPosMap = pFileDataTmp;
      BYTE *WPosPal = pFileDataTmp+(RTMAP_X*RTMAP_Y);
      memcpy(pRTMap ,WPosMap,RTMAP_X*RTMAP_Y);
      memcpy(m_pRTPal,WPosPal,256*3);

      // maintenant on doit extraire la zone qui nous interresse...
      for(int y=0;y<RT_VIEW_2Y;y++)
      {
         if((y+m_dwLoadZoneYS) < 3072)
         {
            int dwOffset = ((y+m_dwLoadZoneYS)*RTMAP_X);
            dwOffset+=m_dwLoadZoneXS;
            BYTE *pS = pRTMap  +dwOffset;
            BYTE *pD = m_pRTMapS +(y*RT_VIEW_2X);

            memcpy(pD,pS,RT_VIEW_2X);
         }
         else
         {
            BYTE *pD = m_pRTMapS +(y*RT_VIEW_2X);
            memset(pD,0x00,RT_VIEW_2X);
         }
      }

      delete []pRTMap ;
      pRTMap  = NULL;
      delete []pFileDataTmp;
      pFileDataTmp = NULL;
      delete []pFileDataTmpC;
      pFileDataTmpC = NULL;
      */
   }
   
}


//////////////////////////////////////////////////////////////////////////////////////////
RTMap *RTMap::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the side menu instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static RTMap instance;
    return &instance;
}

//////////////////////////////////////////////////////////////////////////////////////////
void RTMap::Draw(V2SPRITEFX *vsfFX)
{
  
   DrawMap();
   GameUI::Draw(vsfFX);
}

void RTMap::DrawMap( void )
{
   static BOOL bDraw = FALSE;
   if(bDraw)
      return;

   bDraw = TRUE;
   CV2Sprite *pVSFInfo = m_MainMapMask.GetVSF();
   LPDIRECTDRAWSURFACE lpSpriteSurface = pVSFInfo->m_lpV2SpriteInfo->Data.lpDDSurface;
   if(lpSpriteSurface && CreateRTMap(m_pTMIData,Player.xPos,Player.yPos,Player.World))
   {
      DDCOLORKEY ddColorKey;
      lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 
      DDSURFACEDESC ddsd;
      memset(&ddsd, 0, sizeof(DDSURFACEDESC));
      ddsd.dwSize = sizeof(DDSURFACEDESC);
      lpSpriteSurface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
      USHORT *pSource = (USHORT *)ddsd.lpSurface;
      WORD   *pTmiMap = m_pTMIData;
      int x, y;
      for (y = 0; y < pVSFInfo->m_lpV2SpriteInfo->Header.dwHeight; y++) 
      {
         for (x = 0; x < pVSFInfo->m_lpV2SpriteInfo->Header.dwWidth; x++) 
         {
            if (pSource[x] != ddColorKey.dwColorSpaceLowValue)
            {
               pSource[x] = pTmiMap[x];
            }
         }
         pSource += ddsd.lPitch>>1;
         pTmiMap+=RT_VIEW_X;
      }
      lpSpriteSurface->Unlock(ddsd.lpSurface);
   }
   bDraw = FALSE;
}

BOOL RTMap::CreateRTMap(WORD* pMap,USHORT xPos,USHORT yPos,USHORT World)
{
   if(m_dwLoadX == (xPos*2) && m_dwLoadY == yPos/* && m_dwLoadW == World*/)
   {
      // dois copuer les item de la liste...
      return TRUE;
   }
      

   memset(pMap,0x00,RT_VIEW_X*RT_VIEW_Y*2);
   memset(m_pTMIDataTmp,0x00,RT_VIEW_X2*RT_VIEW_Y2*2);

   if(World >NBR_RT_MAP-1)
      return TRUE;

   //Convertie les pos du player en pos des map...
   //yPos *=2;
   int xOri = xPos-RT_VIEW_Y4;
   xPos *=2;

   int xSd = 0;
   int xEd = RT_VIEW_X2;
   int ySd = 0;
   int yEd = RT_VIEW_Y2;

   int xSs    = xPos-RT_VIEW_X4;
   int ySs    = yPos-RT_VIEW_Y4;
   int ySsOri = ySs;
   
   if(xPos -RT_VIEW_X4 < 0)
   {
      xSs = 0;
   }
   if(yPos -RT_VIEW_Y4 < 0)
   {
      ySs = 0;
   }
   
   if(xPos +RT_VIEW_X4 > RTMAP_X)
      xEd = RTMAP_X-xPos+RT_VIEW_X4;
   if(yPos +RT_VIEW_Y4 > RTMAP_Y)
      yEd = RTMAP_Y-yPos+RT_VIEW_Y4;


   if(m_pRTMapS == NULL)
      m_pRTMapS = new BYTE[RTMAP_X*RTMAP_Y];
   LoadRTWorld(World,xSs,ySs);
   LoadMapZone(m_pRTMapS,m_pMapImage,xSs,ySs,RT_VIEW_X2,RT_VIEW_Y2);

 
   BYTE *pTMI  = m_pMapImage;
   BYTE *pTMIM = m_pMapImageM;
   BYTE *pTMIP = m_pRTPal;
   
   if(!pTMI)
      return TRUE; // ok, but draw no draw map...

   WORD   wColor = 0;
   WORD   wRed,wGreen,wBlue,wPal;
   BYTE   bRDown = lpDXDirectDraw->bRDown;
   BYTE   bGDown = lpDXDirectDraw->bGDown;
   BYTE   bBDown = lpDXDirectDraw->bBDown;
   BYTE   bRUp = lpDXDirectDraw->bRUp;
   BYTE   bGUp = lpDXDirectDraw->bGUp;
   BYTE   bBUp = lpDXDirectDraw->bBUp;

   int dwXOff = ((xSs/2)%TR_MAP_XDIV);

   int tMapVisible[15][11];
   for(int yyy=0;yyy<15;yyy++)
      for(int xxx=0;xxx<11;xxx++)
         tMapVisible[yyy][xxx] = 0;


   
   int dwFirstX,dwFirstY;
   g_SaveGame.GetRTMapVal(((xSs+xSd)/2),ySs,Player.World,&dwFirstX,&dwFirstY);

   int dwOffX = ((xSs)) - (dwFirstX*32);
   int dwOffY = (ySs) - (dwFirstY*16);


   int dwYSsT = ySs;
   // Trouve les case affecter...
   for(int y=ySd ; y<yEd ; y++,dwYSsT++)
   {
      for(int x=xSd ; x<xEd ; x++)
      {
         int dwCaseX,dwCaseY;
         if(g_SaveGame.GetRTMapVal(((xSs+x)/2),dwYSsT,Player.World,&dwCaseX,&dwCaseY))
            tMapVisible[dwCaseY-dwFirstY][dwCaseX-dwFirstX] = 1;
         else
            tMapVisible[dwCaseY-dwFirstY][dwCaseX-dwFirstX] = 0;

      }
   }

   CreateMapMask(m_pMapImageM,tMapVisible,((11*32)+16),((15*16)+16));


   int y;
   for(y=ySd ; y<yEd ; y++,ySs++)
   {
      BYTE *pS = pTMI          + y*RT_VIEW_X2;
      BYTE *pM = pTMIM         + (y+dwOffY+8)*((11*32)+16);
      WORD *pD = m_pTMIDataTmp + y*RT_VIEW_X2;
      for(int x=xSd ; x<xEd ; x++)
      {
         if(pM[x+8+dwOffX])
         {
            wPal = pS[x]*3;
            wRed   = (pTMIP[wPal])  >>bRDown;
            wGreen = (pTMIP[wPal+1])>>bGDown;
            wBlue  = (pTMIP[wPal+2])>>bBDown;

            wColor = (wRed<<bRUp)+(wGreen<<bGUp)+(wBlue<<bBUp);

            pD[x] = wColor;
         }
      }
   }


   xSd = 0;
   ySd = 0;
   if(xPos -RT_VIEW_X4 < 0)
      xSd = RT_VIEW_X4-xPos;
   if(yPos -RT_VIEW_Y4 < 0)
      ySd = RT_VIEW_Y4-yPos;

   // on resize la petite image dans la grande case...
   int yy = (ySd*2);
   int xx = 0;
   for(int y=0 ; y<RT_VIEW_Y2-ySd ; y++,yy+=2)
   {
      xx = (xSd*2);
      for(int x=0;x<RT_VIEW_X2-xSd;x++,xx+=2)
      {
         pMap[((yy+0)*RT_VIEW_X)+(xx+0)] = m_pTMIDataTmp[(y*RT_VIEW_X2)+x];
         pMap[((yy+0)*RT_VIEW_X)+(xx+1)] = m_pTMIDataTmp[(y*RT_VIEW_X2)+x];
         pMap[((yy+1)*RT_VIEW_X)+(xx+0)] = m_pTMIDataTmp[(y*RT_VIEW_X2)+x];
         pMap[((yy+1)*RT_VIEW_X)+(xx+1)] = m_pTMIDataTmp[(y*RT_VIEW_X2)+x];
      }
   }

   m_dwLoadX = xPos;
   m_dwLoadY = yPos;
   m_dwLoadW = World;

   return TRUE;
}



void RTMap::LoadMapZone(BYTE *pMap,BYTE *pData,int dwX, int dwY,int dwW,int dwH)
{
   memset(pData,0x00,dwW*dwH);

   for(int y=dwY,yy=0;y<(dwY+dwH);y++,yy++)
   {
      BYTE *pByteLine = pData+(yy*dwW);
      if(y<RTMAP_Y)
      {
         BYTE *pSource   = pMap+((y*RTMAP_X)+dwX);
         memcpy(pByteLine,pSource,dwW);
         //memset(pByteLine,0x00,dwW);
      }
      else
      {
         memset(pByteLine,0x00,dwW);
      }
   }

   
/*
   // on dois se positionner a la ligne voulu et ire cette ligne...
   int yy = 0;
   for(int y=dwY;y<(dwY+dwH);y++)
   {
      if(((dwX-m_dwLoadZoneXS)+dwW) > RT_VIEW_2X)
         int caca = 1;


      BYTE *pByteLine = pData+(yy*dwW);
      //BYTE *pSource   = pMap+((y*RTMAP_X)+dwX);
      if(y<RTMAP_Y)
      {
         BYTE *pSource   = pMap+(((y-m_dwLoadZoneYS)*RT_VIEW_2X)+(dwX-m_dwLoadZoneXS));
         memcpy(pByteLine,pSource,dwW);
      }
      else
      {
         memset(pByteLine,0x00,dwW);
      }
      yy++;
   }
   */
}

HRESULT RTMap::CreateMapMask (BYTE *pData,int ptMap[15][11],int dwW,int dwH)
{
   memset(pData,0x00,dwW*dwH);
   // on doit scanner toute la grille et affecter si oui ou non on dessine...

   CV2Sprite *pVSFInfo = m_rtMask.GetVSF();
   LPDIRECTDRAWSURFACE lpSpriteSurface = pVSFInfo->m_lpV2SpriteInfo->Data.lpDDSurface;
   if(lpSpriteSurface)
   {
      DDCOLORKEY ddColorKey;
      lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 
      DDSURFACEDESC ddsd;
      memset(&ddsd, 0, sizeof(DDSURFACEDESC));
      ddsd.dwSize = sizeof(DDSURFACEDESC);
      lpSpriteSurface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
      USHORT *pSource = (USHORT *)ddsd.lpSurface;

      // on dois boucler pour toute les zone de la minimaps
      // et appliquer le mask si cette zone est visible...

      for(int y=0;y<15;y++)
      {
         for(int x=0;x<11;x++)
         {
            if(ptMap[y][x])
            {
               // on dois appliquer le mask...
               USHORT *pSource = (USHORT *)ddsd.lpSurface;
               
               int mx = (x*32);
               int my = (y*16);
               for (int yy = 0; yy < pVSFInfo->m_lpV2SpriteInfo->Header.dwHeight; yy++) 
               {
                  int dwMaskPos = ((my+yy)*dwW)+mx;
                  for (int xx = 0; xx < pVSFInfo->m_lpV2SpriteInfo->Header.dwWidth; xx++) 
                  {
                     //pData[dwMaskPos+xx] = 0xFF;
                     if (pSource[xx] != ddColorKey.dwColorSpaceLowValue)
                     {
                        pData[dwMaskPos+xx] = 0xFF;
                     }
                  }
                  pSource += ddsd.lPitch>>1;
               }
            }
         }
      }
      lpSpriteSurface->Unlock(ddsd.lpSurface);
   }
   return TRUE;
}

void RTMap::Hide()
{
   BoxUI::Hide();

   if(m_pRTMapS)
      delete []m_pRTMapS;
   m_pRTMapS = NULL;
   m_dwLoadX = -1;
   m_dwLoadY = -1;
   m_dwLoadW = -1;
   m_dwCurrentLoadedWorld = -1;
}

int RTMap::SaveCurrentImage8(BYTE *pshImage,BYTE *pPal,int dwWidth, int dwHeight,char *pstrName,BOOL bFlipV)
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

