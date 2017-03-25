// SideMenu.cpp: implementation of the SideMenu class.
//
//////////////////////////////////////////////////////////////////////
#include "../pch.h"

#include "SideMenu.h"
#include "RootBoxUI.h"

#include "CharacterUI.h"
#include "ChatterUI.h"
#include "..\Zlib/zlib.h"

#include "..\Bitmap.h"
#include "..\SaveGame.h"

extern CSaveGame g_SaveGame;
extern Bitmap Grid;

#define NBR_TMI_MAP 8

#define GPS_MAP_X   3072
#define GPS_MAP_Y   3072
#define GPS_VIEW_X  3072//512
#define GPS_VIEW_Y  3072//512
#define GPS_VIEW_X2 (GPS_VIEW_X/2)
#define GPS_VIEW_Y2 (GPS_VIEW_Y/2)



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SideMenu::SideMenu()
{
   // Setup the chatter channel
   int dwDrawOffset = 150;
   SetPos( FramePos(0, g_Global.GetScreenH()-dwDrawOffset-180, 180, g_Global.GetScreenH()-dwDrawOffset ) );
   RootBoxUI::GetInstance()->AddChild( this );

   //Load TMIWorld
   m_pTMIData        = new WORD[116*113]; // size of TMI mask...

   ClientConfig *pConfig = g_Global.GetClientConfig();
   char strPath[512];
   sprintf(strPath,"%s",pConfig->strPath);
   sprintf(m_TMIFile,"%s%s",strPath,"\\Game Files\\TMI_Map.dat");

   m_pTMIMap = new BYTE[GPS_VIEW_X*GPS_VIEW_Y];
   m_pTMIPal = new BYTE[256*3];
   memset(m_pTMIMap,0x00,GPS_VIEW_X*GPS_VIEW_Y);
   memset(m_pTMIPal,0x00,256*3);

   m_MainTMI      .SetSpriteId( "GUI_Back_TMI" );
   m_MainTMI      .SetPos( FramePos(0, g_Global.GetScreenH()-dwDrawOffset-180, 180, g_Global.GetScreenH()-dwDrawOffset ) );
   m_MainTMI      .DrawAlpha(true);
   m_MainTMIMask  .SetSpriteId( "GUI_back_TMI_Mask" );
   m_MainTMIMask  .SetPos( FramePos(33, g_Global.GetScreenH()-dwDrawOffset-180+35, 33+116, g_Global.GetScreenH()-dwDrawOffset-180+35+113 ) );
   m_MainTMIPPos  .SetSpriteId( "GUI_back_TMI_PPos" );
   m_MainTMIPPos  .SetPos( FramePos(33, g_Global.GetScreenH()-dwDrawOffset-180+35, 33+116, g_Global.GetScreenH()-dwDrawOffset-180+35+113 ) );

   AddChild( &m_MainTMI);
   AddChild( &m_MainTMIMask);
   AddChild( &m_MainTMIPPos);

   m_dwCurrentLoadedWorld = -1;

   //m_dwLoadZoneXS = -1;
   //m_dwLoadZoneYS = -1;

   SetupViewObject();

   Hide();
}

SideMenu::~SideMenu()
{
   if(m_pTMIMap)
      delete m_pTMIMap;
   if(m_pTMIPal)
      delete m_pTMIPal;
   if(m_pTMIData)
      delete []m_pTMIData;

   m_pTMIMap = NULL;
   m_pTMIPal = NULL;
   m_pTMIData = NULL;
}

void SideMenu::LoadTmiWorld(int iWorld,int dwX,int dwY)
{
   if(iWorld <0 ||  iWorld >NBR_TMI_MAP-1)
      iWorld = 0;

   if(iWorld == m_dwCurrentLoadedWorld)
      return;
   m_dwCurrentLoadedWorld = iWorld;
   OutputDebugString(" *****LOAD TMI WORLD\n");

   
   FILE *pf1 = fopen(m_TMIFile,"rb");
   if(!pf1)
   {
      memset(m_pTMIMap,0x00,GPS_VIEW_X*GPS_VIEW_Y);
      memset(m_pTMIPal,0x00,256*3);
   }
   else
   {

      ULONG dwTailleOri;
      ULONG dwTailleComp[NBR_TMI_MAP];
      ULONG dwFilePos[NBR_TMI_MAP];

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
         memset(m_pTMIMap,0x00,GPS_MAP_X*GPS_MAP_Y);
         memset(m_pTMIPal,0x00,256*3);
      }
      else
      {
         BYTE *WPosMap = pFileDataTmp;
         BYTE *WPosPal = pFileDataTmp+(GPS_MAP_X*GPS_MAP_Y);
         memcpy(m_pTMIMap,WPosMap,GPS_MAP_X*GPS_MAP_Y);
         memcpy(m_pTMIPal,WPosPal,256*3);
      }

      //char strTmp[512];
      //sprintf(strTmp,"c:\\!!__GPSWorld%d.bmp",iWorld);
      //SaveCurrentImage8(m_pTMIMap,m_pTMIPal,GPS_MAP_X, GPS_MAP_Y,strTmp,false);

      delete []pFileDataTmp;
      pFileDataTmp = NULL;
      delete []pFileDataTmpC;
      pFileDataTmpC = NULL;

      /*

      BYTE *pRTMap = new BYTE[GPS_MAP_X*GPS_MAP_Y];
      
      if(uncompress((BYTE*)pFileDataTmp,&dwTailleOri,(BYTE*)pFileDataTmpC,dwTailleComp[iWorld]) != Z_OK)
      {
         memset(pRTMap,0x00,GPS_MAP_X*GPS_MAP_Y);
         memset(m_pTMIPal,0x00,256*3);

      }

      BYTE *WPosMap = pFileDataTmp;
      BYTE *WPosPal = pFileDataTmp+(GPS_MAP_X*GPS_MAP_Y);
      memcpy(pRTMap,WPosMap,GPS_MAP_X*GPS_MAP_Y);
      memcpy(m_pTMIPal,WPosPal,256*3);

      // maintenant on doit extraire la zone qui nous interresse...
      for(int y=0;y<GPS_VIEW_Y;y++)
      {
         if((y+m_dwLoadZoneYS) < GPS_MAP_Y)
         {
            int dwOffset = ((y+m_dwLoadZoneYS)*GPS_MAP_X);
            dwOffset+=m_dwLoadZoneXS;
            BYTE *pS = pRTMap  +dwOffset;
            BYTE *pD = m_pTMIMap +(y*GPS_VIEW_X);

            memcpy(pD,pS,GPS_VIEW_X);
         }
         else
         {
            BYTE *pD = m_pTMIMap +(y*GPS_VIEW_X);
            memset(pD,0x00,GPS_VIEW_X);
         }
      }

      //SaveCurrentImage8(m_pTMIMap,GPS_VIEW_X, GPS_VIEW_Y,"C:\\!!!OHOH.bmp",FALSE);

      delete []pRTMap;
      pRTMap = NULL;
      delete []pFileDataTmp;
      pFileDataTmp = NULL;
      delete []pFileDataTmpC;
      pFileDataTmpC = NULL;
      */
   }
   
}


//////////////////////////////////////////////////////////////////////////////////////////
SideMenu *SideMenu::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the side menu instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static SideMenu instance;
    return &instance;
}

//////////////////////////////////////////////////////////////////////////////////////////
void SideMenu::Draw(V2SPRITEFX *vsfFX)
{

   if(ChatterUI::GetInstance()->IsMinimizedHight() && g_SaveGame.GetOptionsParam()->bHyperChat)
      return; //dont draw TMI in hyper chat mode...


   int dwDrawOffset = 0;
   if(ChatterUI::GetInstance()->IsShown())
   {
      if(ChatterUI::GetInstance()->IsMinimizedHight() && !g_SaveGame.GetOptionsParam()->bHyperChat)
         dwDrawOffset = 300;
      else
         dwDrawOffset = 150;
   }

   SetPos( FramePos(0, g_Global.GetScreenH()-dwDrawOffset-180, 180, g_Global.GetScreenH()-dwDrawOffset ) );
   m_MainTMI      .SetPos( FramePos(0, g_Global.GetScreenH()-dwDrawOffset-180, 180, g_Global.GetScreenH()-dwDrawOffset ) );
   m_MainTMIMask  .SetPos( FramePos(33, g_Global.GetScreenH()-dwDrawOffset-180+35, 33+116, g_Global.GetScreenH()-dwDrawOffset-180+35+113 ) );
   m_MainTMIPPos  .SetPos( FramePos(33, g_Global.GetScreenH()-dwDrawOffset-180+35, 33+116, g_Global.GetScreenH()-dwDrawOffset-180+35+113 ) );

   
   DrawTMI();
   GameUI::Draw(vsfFX);
}

int SideMenu::GetStartOffsetX()
{
   FramePos thisPos = GetPos();
   int dwOffset = 0;
   //if(!IsShown())
   //   dwOffset = 100;

   return ((thisPos.y+dwOffset-60)/42);
} 

void SideMenu::DrawTMI( void )
{
   CV2Sprite *pVSFInfo = m_MainTMIMask.GetVSF();
   LPDIRECTDRAWSURFACE lpSpriteSurface = pVSFInfo->m_lpV2SpriteInfo->Data.lpDDSurface;
   if(lpSpriteSurface && CreateTMIMap(m_pTMIData,Player.xPos,Player.yPos,Player.World))
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
         pTmiMap+=116;
      }
      lpSpriteSurface->Unlock(ddsd.lpSurface);

      
      Player.xPos;
      Player.yPos;
      Player.World;
      

   }
}

BOOL SideMenu::CreateTMIMap(WORD* pMap,USHORT xPos,USHORT yPos,USHORT World)
{
   memset(pMap,0x00,116*113*2);

   int xSd = 0;
   int xEd = 116;
   int ySd = 0;
   int yEd = 113;

   int xSs    = xPos-57;
   int ySs    = yPos-57;
   int ySsOri = ySs;
   
   if(xPos -57 < 0)
   {
      xSd = 57-xPos;
      xSs += xSd;
   }
   if(yPos -57 < 0)
   {
      ySd = 57-yPos;
      ySs += ySd;
   }
   
   if(xPos +56 > GPS_MAP_X)
      xEd = GPS_MAP_X-xPos+56;
   if(yPos +56 > GPS_MAP_Y)
      yEd = GPS_MAP_Y-yPos+56;
   
   LoadTmiWorld(World,xSs,ySs);
   
   BYTE *pTMI  = m_pTMIMap;
   BYTE *pTMIP = m_pTMIPal;
   
   
   if(!pTMI)
      return TRUE; // ok, but draw no draw map...


   WORD   wColor = 0;
   WORD   wRed=0,wGreen=0,wBlue=0,wPal=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
   BYTE   bRDown = lpDXDirectDraw->bRDown;
   BYTE   bGDown = lpDXDirectDraw->bGDown;
   BYTE   bBDown = lpDXDirectDraw->bBDown;
   BYTE   bRUp = lpDXDirectDraw->bRUp;
   BYTE   bGUp = lpDXDirectDraw->bGUp;
   BYTE   bBUp = lpDXDirectDraw->bBUp;

   CV2Sprite *pVSFInfo = m_MainTMIMask.GetVSF();
   LPDIRECTDRAWSURFACE lpSpriteSurface = pVSFInfo->m_lpV2SpriteInfo->Data.lpDDSurface;
   DDCOLORKEY ddColorKey;
   lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 

   int dwOffset = 0;

   for(int y=ySd ; y<yEd ; y++,ySs++)
   {
      BYTE *pS = pTMI + ((ySs*GPS_VIEW_X)+xSs);
      WORD *pD = pMap + y*116;
      int dwCnt = xSs;

      dwOffset = xSd;

      for(int x=xSd ; x<xEd ; x++)
      {
         wPal = pS[x-dwOffset]*3;
         wRed   = (pTMIP[wPal])  >>bRDown;
         wGreen = (pTMIP[wPal+1])>>bGDown;
         wBlue  = (pTMIP[wPal+2])>>bBDown;

         wColor = (wRed<<bRUp)+(wGreen<<bGUp)+(wBlue<<bBUp);


         if(ddColorKey.dwColorSpaceLowValue == wColor)
            wColor = 0;
         pD[x] = wColor;
      }
   }

   /*
   for(int y=ySd ; y<yEd ; y++,ySs++)
   {
      BYTE *pS = pTMI + (((ySs-m_dwLoadZoneYS)*GPS_VIEW_X)+(xSs-m_dwLoadZoneXS));
      WORD *pD = pMap + y*116;
      int dwCnt = xSs-m_dwLoadZoneXS;

      int dwOffset = 0;

      for(int x=xSd ; x<xEd ; x++)
      {
         wPal = pS[x-dwOffset]*3;
         wRed   = (pTMIP[wPal])  >>bRDown;
         wGreen = (pTMIP[wPal+1])>>bGDown;
         wBlue  = (pTMIP[wPal+2])>>bBDown;

         wColor = (wRed<<bRUp)+(wGreen<<bGUp)+(wBlue<<bBUp);


         if(ddColorKey.dwColorSpaceLowValue == wColor)
            wColor = 0;
         pD[x] = wColor;
      }
   }
   */


   //Object->Friendly == 1  // Autre Joueurs...
   //Object->Friendly == 2  // PNJ
   //Object->Friendly == 3  // Monstres
   //Object->Friendly == 4  // portes / Chest... 
   //Object->Friendly == 5  // Items au sol...
   //Object->Friendly == 6  // Pancarte...

   // Scann la liste des object pour en afficher sur la map TMI...
   Objects.Lock(2266);

   Objects.ToHead();
   TFCObject *Object;
   while(Objects.QueryNext()) 
   {
      Object = Objects.GetObject();

     
      if(Object->AbsX >  xSs && Object->AbsX < xSs+114 && 
         Object->AbsY >  ySsOri && Object->AbsY < ySsOri+111     )
      {
         int y = (Object->AbsY-ySsOri);
         int x = (Object->AbsX-xSs);

         x+=dwOffset;


         WORD *pD = pMap + y*116;

         
         BOOL bDraw = FALSE;
         if(Object->Type <10000)
         {
            if(Object->Type <__OBJGROUP_LASTGROUP && m_bShowThisObjType[Object->Type])
            {
               bDraw = TRUE;
               wRed   = (0)  >>bRDown;
               wGreen = (255)>>bGDown;
               wBlue  = (0)>>bBDown;
            }
         }
        /* else if(Object->Type >10000 && Object->Friendly == 1) //Friendly==1 : les joueurs
         {
            bDraw = FALSE;//BLBLBL on cache les points rouges
            wRed   = (255)  >>bRDown;
            wGreen = (0)>>bGDown;
            wBlue  = (0)>>bBDown;
         }*/
         else if(Object->Type >10000 && Object->Friendly == 2) //PNJs
         {
            bDraw = TRUE; 
            wRed   = (0)   >>bRDown;
            wGreen = (0)   >>bGDown;
            wBlue  = (255) >>bBDown;
         }

         if(bDraw)
         {
            wColor = (wRed<<bRUp)+(wGreen<<bGUp)+(wBlue<<bBUp);
            pD[x]   = wColor;
            pD[x+1] = wColor;
            pD[x+2] = wColor;
            y++;
            pD = pMap + y*116;
            pD[x]   = wColor;
            pD[x+1] = wColor;
            pD[x+2] = wColor;
            y++;
            pD = pMap + y*116;
            pD[x]   = wColor;
            pD[x+1] = wColor;
            pD[x+2] = wColor;
         }
      }
   }
   Objects.Unlock(2266);

   return TRUE;
}

void SideMenu::SetupViewObject()
{
   for(int i=0;i<__OBJGROUP_LASTGROUP;i++)
      m_bShowThisObjType[i] = FALSE;

   //Armor and items...
   for(int i=1;i<=14;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=18;i<=103;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=107;i<=129;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=141;i<=142;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=163;i<=189;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=200;i<=213;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=235;i<=289;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=294;i<=329;i++)
      m_bShowThisObjType[i] = TRUE;

   for(int i=421;i<=455;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=458;i<=644;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=646;i<=768;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=782;i<=790;i++)
      m_bShowThisObjType[i] = TRUE;
   for(int i=869;i<=872;i++)
      m_bShowThisObjType[i] = TRUE;
   

   

   // ajoute les objets pouvant etre afficher...
}

int SideMenu::SaveCurrentImage8(BYTE *pshImage,BYTE *pPal,int dwWidth, int dwHeight,char *pstrName,BOOL bFlipV)
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