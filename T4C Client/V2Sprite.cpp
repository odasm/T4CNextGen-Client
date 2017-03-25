// Include 
#include "pch.h"
#include <windows.h>
#pragma warning( disable : 4786 )
#include "V2Sprite.h"
#include "WarningBox.h"
#include "IDirectXDraw.h"
#include "V2Database.h"
#include "PowerDebug.h"
#include "Random.h"
#include <stdio.h>
#include "global.h"
#include "MemoryManager.h"
#include "App.h"
#include "Sprite.h"
#include "V2PalManager.h"
#include "MacroHandler.h"
#include "VisualObjectList.h"

// No Color Key Decompression 
#define V2_NCK_BREAK     0
#define V2_NCK_SAME_LINE 1
#define V2_NCK_NEXT_LINE 2

#define V2_NCK_NORMAL 0
#define V2_NCK_SHADOW 1 


#define _RGB16BIT555_T(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))
#define _RGB16BIT565_T(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))
#define _RGB32BIT_T(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))


extern BOOL OVERLAP;
extern WORD bColor;

/*********************************************************************************
* Constructor
*
*********************************************************************************/
CV2Sprite::CV2Sprite( void )
{
   m_pRefDataPal     = NULL;
   m_lpV2SpriteInfo  = NULL;
   m_pTempSprite     = NULL;
}


/*********************************************************************************
* Destructor
*
*********************************************************************************/
CV2Sprite::~CV2Sprite( void )
{
   Release();
}



/*********************************************************************************
* Release the Database Object
*
*********************************************************************************/
void CV2Sprite::Release( void )
{
   if (!m_strName.empty() && !g_boQuitApp) 
   {
      g_V2Database.Release(m_strName.c_str());
      m_pRefDataPal = NULL;
      m_strName = "";
   }
}

/**********************************************************************************
 * Load a Sprite from the database.
 * 
 * @PARAM dwRelativeIndex The Index to start the search in the Database. If it's
 * 0, the search will start from the last index and the search will go backward,
 * if it's less than 0, the search will start from the specified index and the
 * search will go forward. This is to optimized multiple sprite loading.
 *
 *   LPCTSTR lpszID,                // ID of the Sprite
 *   DWORD dwRelativeIndex,         // The Index to start the search in the Database.
 *   int iPalID                     // ID nb of the Pal
 *
 * @REMARK This is a wrapper method around CreateSprite. It was introduce to be
 * backward compatble with the old sprite class.
 * 
 * @RETURN Return the Index of the current Sprite ID (usefull to make a 
 * relative index search).
 *
 *********************************************************************************/
DWORD CV2Sprite::LoadSprite(LPCTSTR lpszID,DWORD dwRelativeIndex,int iPalID)
{
   return CreateSprite(lpszID, dwRelativeIndex, iPalID);
}

void CV2Sprite::LoadSpriteData(BYTE* pData, BYTE* pPal, int dwWidth, int dwHeight)
{
    if(m_lpV2SpriteInfo )
       delete m_lpV2SpriteInfo;
    m_lpV2SpriteInfo = NULL;


    m_lpV2SpriteInfo = new V2SPRITEINFO;
    m_lpV2SpriteInfo->Header.dwCompType      = COMP_DD;
    m_lpV2SpriteInfo->Header.flag1           = 1;
    m_lpV2SpriteInfo->Header.dwWidth         = dwWidth;
    m_lpV2SpriteInfo->Header.dwHeight        = dwHeight;
    m_lpV2SpriteInfo->Header.shOffX1         = 0;
    m_lpV2SpriteInfo->Header.shOffY1         = 0;
    m_lpV2SpriteInfo->Header.shOffX2         = 0;
    m_lpV2SpriteInfo->Header.shOffY2         = 0;
    m_lpV2SpriteInfo->Header.ushTransparency = 3;
    m_lpV2SpriteInfo->Header.ushTransColor   = 255;
    m_lpV2SpriteInfo->Header.dwDataUnpack    = NULL;
    m_lpV2SpriteInfo->Header.dwDataPack      = NULL;


    WORD wWidth  = m_lpV2SpriteInfo->Header.dwWidth;
    WORD wHeight = m_lpV2SpriteInfo->Header.dwHeight;

    {
       DDSURFACEDESC Surface; 
       ZeroMemory(&Surface, sizeof(Surface)); 
       Surface.dwSize          = sizeof(Surface); 
       Surface.dwFlags         = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH; 
       Surface.ddsCaps.dwCaps  = DDSCAPS_SYSTEMMEMORY; 
       Surface.dwWidth         = wWidth; 
       Surface.dwHeight        = wHeight; 
       DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Surface, &m_lpV2SpriteInfo->Data.lpDDSurface, NULL), "Cheap Ass"); 
    }

    DDSURFACEDESC Surface;
    memset(&Surface, 0, sizeof(Surface));
    Surface.dwSize = sizeof(Surface);
    
    WORD wPal, wRed, wGreen, wBlue, wColor;
    
    LPBYTE lpPal = pPal;
    wPal = m_lpV2SpriteInfo->Header.ushTransColor*3;
    
    wRed   = (lpPal[wPal  ])>>lpDXDirectDraw->bRDown;
    wGreen = (lpPal[wPal+1])>>lpDXDirectDraw->bGDown;
    wBlue  = (lpPal[wPal+2])>>lpDXDirectDraw->bBDown;
    
    
    DDCOLORKEY ColorKey;
    ColorKey.dwColorSpaceLowValue  =_RGB16BIT565_T(wRed,wGreen,wBlue);
    ColorKey.dwColorSpaceHighValue =_RGB16BIT565_T(wRed,wGreen,wBlue);
    m_lpV2SpriteInfo->Data.lpDDSurface->SetColorKey(DDCKEY_SRCBLT, &ColorKey); 
    
    
    m_lpV2SpriteInfo->Data.lpDDSurface->Lock(NULL, &Surface,DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
    
    LPBYTE lpbDest = (LPBYTE)Surface.lpSurface;
    LPBYTE lpbSource = pData;
    long   lPitch = Surface.lPitch;
    
    //WORD wPal, wRed, wGreen, wBlue, wColor;
    
    
    for (int i = 0; i < wHeight; i++) 
    {
       for (int j = 0; j < wWidth; j++) 
       {
          wPal = (*(BYTE *)(lpbSource+j))*3;
          wRed = (lpPal[wPal])>>lpDXDirectDraw->bRDown;
          wGreen = (lpPal[wPal+1])>>lpDXDirectDraw->bGDown;
          wBlue = (lpPal[wPal+2])>>lpDXDirectDraw->bBDown;
          wColor = (wRed<<lpDXDirectDraw->bRUp)+(wGreen<<lpDXDirectDraw->bGUp)+(wBlue<<lpDXDirectDraw->bBUp);
          
          lpbDest[(j<<1)+1] = (BYTE)(wColor >> 8);
          lpbDest[(j<<1)] = (BYTE)(wColor & 0xFF);
       }
       lpbDest += lPitch;
       lpbSource += wWidth;
    }
    m_lpV2SpriteInfo->Data.lpDDSurface->Unlock(Surface.lpSurface);
}

/**********************************************************************************
 * Load a Sprite from the database.
 * 
 * @PARAM dwRelativeIndex The Index to start the search in the Database. If it's
 * 0, the search will start from the last index and the search will go backward,
 * if it's less than 0, the search will start from the specified index and the
 * search will go forward. This is to optimized multiple sprite loading.
 *
 *   LPCTSTR lpszID,                // ID of the Sprite
 *   DWORD dwRelativeIndex,         // The Index to start the search in the Database.
 *   int iPalID                     // ID nb of the Pal
 *
 *
 * @RETURN Return the Index of the current Sprite ID (usefull to make a 
 * relative index search).
 *********************************************************************************/
DWORD CV2Sprite::CreateSprite(LPCTSTR lpszID,DWORD dwRelativeIndex,int iPalID)
{
    DWORD dwIdxPos = 0;
	if (lpszID != "") { //BLBL on essaye même pas de charger un sprite sans nom..

	   if (!m_pRefDataPal) 
	   {
		   if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
			   CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
		  m_pRefDataPal = CV2PalManager::GetInstance()->GetPal(lpszID, iPalID);
	   }

	   
	   if (m_strName.empty()) 
	   {
			if(iPalID == 1)
			 dwIdxPos = g_V2Database.LoadSprite(lpszID, &m_lpV2SpriteInfo, dwRelativeIndex,m_pRefDataPal,iPalID);
			else
			 dwIdxPos = g_V2Database.LoadSprite(lpszID, &m_lpV2SpriteInfo, dwRelativeIndex,m_pRefDataPal,0);
      
			m_strName = lpszID;
      
		  // If the sprite doesn't exist.
		  if (!dwIdxPos) 
		  {
			  if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
				  CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
			 // Load a Default Sprite
			 m_pRefDataPal = CV2PalManager::GetInstance()->GetPal("", iPalID);
			 dwIdxPos = g_V2Database.LoadSprite("Black Tile", &m_lpV2SpriteInfo, dwRelativeIndex,m_pRefDataPal,1);
			 m_strName = "Black Tile";
         
			 char strTmp[100];
			 sprintf(strTmp,"SPRITE ERROR1 [%s]\n",lpszID);
			 OutputDebugString(strTmp);
		  }
	   }
   }else{//BLBL pour les sprites vides on crée des black tile :

		if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
			CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
			 // Load a Default Sprite
			 m_pRefDataPal = CV2PalManager::GetInstance()->GetPal("", iPalID);
			 dwIdxPos = g_V2Database.LoadSprite("Black Tile", &m_lpV2SpriteInfo, dwRelativeIndex,m_pRefDataPal,1);
			 m_strName = "Black Tile";
         
	
   }
   return dwIdxPos;
}

/**********************************************************************************
 * Load a Sprite from the database.
 * 
 * @PARAM dwRelativeIndex The Index to start the search in the Database. If it's
 * 0, the search will start from the last index and the search will go backward,
 * if it's less than 0, the search will start from the specified index and the
 * search will go forward. This is to optimized multiple sprite loading.
 *
 *   LPCTSTR lpszID,                // ID of the Sprite
 *   DWORD dwRelativeIndex,         // The Index to start the search in the Database.
 *   LPBYTE iPalID,                 // ID nb of the Pal
 *   int iPalIndex
 *
 *
 * @RETURN Return the Index of the current Sprite ID (usefull to make a 
 * relative index search).
 *********************************************************************************/
DWORD CV2Sprite::CreateSpriteByPal(LPCTSTR lpszID,DWORD dwRelativeIndex,LPBYTE iPalID,int iPalIndex)
{
	m_pRefDataPal   = iPalID;

   DWORD dwIdxPos = 0;
   if (m_strName.empty()) 
   {
      dwIdxPos = g_V2Database.LoadSprite(lpszID, &m_lpV2SpriteInfo, dwRelativeIndex,m_pRefDataPal,0);
      m_strName = lpszID;
      
      // If the sprite doesn't exist.
      if (!dwIdxPos) 
      {
		  if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
			  CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
         // Load a Default Sprite
         m_pRefDataPal = CV2PalManager::GetInstance()->GetPal("", 1);
         dwIdxPos = g_V2Database.LoadSprite("Black Tile", &m_lpV2SpriteInfo, dwRelativeIndex,m_pRefDataPal,1);
         m_strName = "Black Tile";
         
         char strTmp[100];
         sprintf(strTmp,"SPRITE ERROR2 [%s]\n",lpszID);
         OutputDebugString(strTmp);
      }
   }
   
   return dwIdxPos;
}


/*********************************************************************************
 * Get the Transparent Color of the Sprite.
 *********************************************************************************/
BYTE CV2Sprite::GetTransColor( )
{
   return (BYTE)m_lpV2SpriteInfo->Header.ushTransColor;
}

/*********************************************************************************
 * Get the Transparent Flag of the Sprite.
 *********************************************************************************/
BOOL CV2Sprite::GetfTransparent( )
{
    if (m_lpV2SpriteInfo != NULL) return m_lpV2SpriteInfo->Header.ushTransparency;
	else return 0;
}

/*********************************************************************************
 * Get the X Translation value of the Sprite.
 *********************************************************************************/
short CV2Sprite::GetCX( )
{
   return m_lpV2SpriteInfo->Header.shOffX1;
}

/*********************************************************************************
 * Get the Y Translation value of the Sprite.
 *********************************************************************************/
short CV2Sprite::GetCY( )
{
   return m_lpV2SpriteInfo->Header.shOffY1;
}

/*********************************************************************************
 * Get the  Flip X Translation value of the Sprite.
 *********************************************************************************/
short CV2Sprite::GetFCX( )
{
   return m_lpV2SpriteInfo->Header.shOffX2;
}

/*********************************************************************************
 * Get the  Flip Y Translation value of the Sprite.
 *********************************************************************************/
short CV2Sprite::GetFCY( )
{
   return m_lpV2SpriteInfo->Header.shOffY2;
}

/*********************************************************************************
 * Get the Width of the Sprite in pixel.
 *********************************************************************************/
WORD CV2Sprite::GetWidth( )
{
	if (m_lpV2SpriteInfo == nullptr)
		return NULL;
    return m_lpV2SpriteInfo->Header.dwWidth;
}

/*********************************************************************************
 * Get the Height of the Sprite in pixel.
 *********************************************************************************/
WORD CV2Sprite::GetHeight( )
{
    return m_lpV2SpriteInfo->Header.dwHeight;
	
}

/**********************************************************************************
 * Get the Direct Draw Surface Memory of the Sprite.
 * 
 * @REMARK This method will only work if the Sprite is a V2SPRITE_DD type (Direct
 * Draw). It is used to make dynamic smoothing.
 * 
 * @WARNING You need to call ReleaseSurfaceMemory when you no longer need the
 * surface memory ptr.
 *
 * @RETURN The surface memory ptr.
 *********************************************************************************/
LPWORD CV2Sprite::GetSurfaceMemory()
{
   DDSURFACEDESC ddsd;
   ZeroMemory(&ddsd, sizeof(ddsd));
   ddsd.dwSize = sizeof(ddsd);

   if (m_lpV2SpriteInfo->Header.dwCompType == COMP_DD) 
   {
      m_lpV2SpriteInfo->Data.lpDDSurface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
   } 
   else 
   {
      m_pTempSprite = new Sprite; 
      m_pTempSprite->Create(GetWidth(), GetHeight(), FALSE, 0, 0, 0, 0, 0); 
      V2SPRITEFX spriteFX;
      ZeroMemory(&spriteFX, sizeof(V2SPRITEFX));
      spriteFX.lpDirectDrawSurface = m_pTempSprite->GetSurface();
      DrawSpriteN(0, 0, &spriteFX);
      ddsd.lpSurface = m_pTempSprite->GetSurfaceMemory();
   }
   return (LPWORD)(ddsd.lpSurface);
}


/**********************************************************************************
 * Release a Surface Memory ptr.
 *********************************************************************************/
void CV2Sprite::ReleaseSurfaceMemory(LPWORD lpb)
{
   if (m_lpV2SpriteInfo->Header.dwCompType == COMP_DD) 
   {
      m_lpV2SpriteInfo->Data.lpDDSurface->Unlock((LPBYTE)lpb);
   } 
   else 
   {
      m_pTempSprite->ReleaseSurfaceMemory(lpb);
      m_pTempSprite->Release();
      delete m_pTempSprite;
      m_pTempSprite = NULL;
   }
}
/**********************************************************************************
 * This function get the type of the current Sprite.
 *********************************************************************************/
BYTE CV2Sprite::GetType( )
{
   return (BYTE)m_lpV2SpriteInfo->Header.dwCompType;
}

/**********************************************************************************
 * Draw a Sprite to a Surface Memory. 
 *
 * @PARAM xPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM yPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM Clip This is an exclusive clipping area (everything outside the clipping 
 * area will not be displayed). 
 *
 * @PARAM Wrap This is the wrap area. It is like the clipping area but everything 
 * rendered outside the wrap area is rendered at the opposite side of the surface.
 * This is essential for the algorithm of the graphic engine.
 *
 * @PARAM ToBlit This is the Target Surface to render the sprite on. If it's not 
 * specifed, the default value is the Backbuffer Surface (called the Complex Surface).
 *
 * @PARAM ParamDither This is the transpancy Value, it is TRUE or FALSE. If this value
 * is true, the sprite renderer will use dithering to render the sprite. The dither 
 * patern is as follow. 1 pixel visible, 1 pixel invisible. This simulate a 50%
 * alpha rendering.
 *
 * @REMARK This is the old format of the DrawSprite and it is there to keep a 
 * compatibility with the old code.
 *
 * @WARNING You should NOT use this method. Instead, use the other version of the 
 * DrawSprite method: DrawSprite(int xPos, int yPos, LPV2SPRITEFX FX)
 *********************************************************************************/
void CV2Sprite::DrawSprite(int xPos,int yPos,RECT *Clip,RECT *OutBound,RECT *Wrap,
                     LPDIRECTDRAWSURFACE ToBlit,char FX,signed char Format,BOOL ParamDither)
{
   V2SPRITEFX spritefx;
   ZeroMemory(&spritefx, sizeof(V2SPRITEFX));
   
   spritefx.lpClipRect = Clip;
   spritefx.lpWrapRect = Wrap;
   spritefx.lpDirectDrawSurface = ToBlit;
   if (FX)
      spritefx.dwFX |= FX_LEFTRIGHTMIRROR;
   if (ParamDither)
      spritefx.dwFX  |= FX_DITHER;
   
   DrawSpriteN(xPos, yPos, &spritefx); 
}

/**********************************************************************************
* Draw a Sprite to a Surface Memory. 
 *
 * @PARAM xPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM yPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM FX This specifed any special behavior when rendering the sprite. Please 
 * refer to V2SPRITEFX for more information about this parameters.
 *
 * @RETURN Return value, a non zero value means an error have occured. Return 
 * value will be V2SPRITE_OK if everything worked fine, or ERR_FORMATNOTHANDLE if the 
 * V2SPRITE format is not handled.
 *********************************************************************************/
HRESULT CV2Sprite::DrawSpriteN(LONG xPos,LONG yPos,LPV2SPRITEFX FX)
{
	LPRECT lpRect;
   BOOL   boLocalFX = FALSE;
	HRESULT hResult;
   
   if (!FX) 
   {
		CreateComponent(FX, V2SPRITEFX);
      CreateComponent(lpRect, RECT);
      SetRectangle(lpRect, 0, 0, g_Global.GetScreenW(), g_Global.GetScreenH());
      FX->lpClipRect = lpRect;
      boLocalFX = TRUE;
   }
   if(!m_lpV2SpriteInfo)
	   return 0;//:CR_NMNMNM

   switch (m_lpV2SpriteInfo->Header.dwCompType) 
   {
      case COMP_DD: // RAW
			hResult = DrawDDSprite(xPos, yPos, FX,FALSE,255);
         if (boLocalFX) 
         {
            if (lpRect!=NULL) DeleteComponent(lpRect);//BLBLBL 29 mars 2009 ajout d'un test non NULL
            if (FX != NULL) DeleteComponent(FX);//BLBLBL 29 mars 2009 ajout d'un test non NULL
         }
         return hResult; 
 		
      case COMP_NCK:  // No Color Key;
         if (FX->dwFX & FX_LEFTRIGHTMIRROR) 
         { 
            hResult = DrawNCKMirrorSprite(xPos, yPos, FX);
            if (boLocalFX) 
            {
               DeleteComponent(lpRect);
               DeleteComponent(FX);
            }
            return hResult; 
         } 
         else 
         { 
            hResult = DrawNCKNormalSprite(xPos, yPos, FX); 
            if (boLocalFX) 
            {
               DeleteComponent(lpRect);
               DeleteComponent(FX);
            }
            return hResult;
         }  
      break;
      case COMP_ZIP:
      break;
      case COMP_NULL:
		break;

      default:
      break;
	}

	if (boLocalFX) 
   {
		DeleteComponent(lpRect);
      DeleteComponent(FX);
   }

   return ERR_FORMATNOTHANDLE;
}

/**********************************************************************************
 * Draw a Sprite to a Surface Memory. 
 *
 * @PARAM xPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM yPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM FX This specifed any special behavior when rendering the sprite. Please 
 * refer to V2SPRITEFX for more information about this parameters.
 *
 * @RETURN Return value, a non zero value means an error have occured. Return 
 * value will be V2SPRITE_OK if everything worked fine, or ERR_FORMATNOTHANDLE if the 
 * V2SPRITE format is not handled.
 *
/*********************************************************************************/
HRESULT CV2Sprite::DrawSpriteNSemiTrans(LONG xPos,LONG yPos,LPV2SPRITEFX FX,BOOL bSemiTrans,BYTE chAlphaLevel)
{
   LPRECT lpRect;
   BOOL   boLocalFX = FALSE;
	HRESULT hResult;
   
   if (!FX) 
   {
		CreateComponent(FX, V2SPRITEFX);
      CreateComponent(lpRect, RECT);
      SetRectangle(lpRect, 0, 0, g_Global.GetScreenW(), g_Global.GetScreenH());
      FX->lpClipRect = lpRect;
      boLocalFX = TRUE;
   }

   if(!m_lpV2SpriteInfo)
	   return 0;//:CR_NMNMNM

   switch (m_lpV2SpriteInfo->Header.dwCompType) 
   {
      case COMP_DD: // RAW
			hResult = DrawDDSprite(xPos, yPos, FX,bSemiTrans,chAlphaLevel);
         if (boLocalFX) 
         {
            if (lpRect != NULL) DeleteComponent(lpRect);//BLBLBL 29 mars 2009 ajout d'un test non NULL
            if (FX != NULL) DeleteComponent(FX);//BLBLBL 29 mars 2009 ajout d'un test non NULL
         }
         return hResult; 
      break;
      case COMP_ZIP:
      break;
      case COMP_NULL:
		break;

      default:
      break;
	}

	if (boLocalFX) 
   {
		DeleteComponent(lpRect);
      DeleteComponent(FX);
   }

   return ERR_FORMATNOTHANDLE;
}

/**********************************************************************************
 * Draw a RAW Sprite to a Surface Memory. 
 *
 * @PARAM xPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM yPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM FX This specifed any special behavior when rendering the sprite. Please 
 * refer to V2SPRITEFX for more information about this parameters.
 *
 * @REMARK The RAW format is the old format and all graphics using this format
 * will be removed from the game eventually. We suggest you use the NCK sprite
 * format.
 *
 * @RETURN Return value, a non zero value means an error have occured. Return 
 * value will be V2SPRITE_OK if everything worked fine, or ERR_FORMATNOTHANDLE if the 
 * V2SPRITE format is not handled.
 *
 *
 * LONG xPos,                     // Horizontal position of the destination.
 * LONG yPos,                     // Vertical position of the destination.
 * LPV2SPRITEFX FX,                    // Specify any special behavior when rendering.
 * BOOL bSemiTrans,
 * BYTE chAlphaLevel
 *
 //*********************************************************************************/
HRESULT CV2Sprite::DrawDDSprite(LONG xPos,LONG yPos,LPV2SPRITEFX FX,BOOL bSemiTrans,BYTE chAlphaLevel)
{
   RECT sourceRectOri, sourceRect,destRect;
   BOOL FullScreen = FALSE;
   WORD wTransOption = 0;
   
   wTransOption = m_lpV2SpriteInfo->Header.ushTransparency;
   if(wTransOption ==4)
      int toto = 1;
   
   
   // Applying Corection
   if (FX) 
   {
      if (!(FX->dwFX & FX_NOCORRECTION)) 
      {
         if (FX->dwFX & FX_LEFTRIGHTMIRROR) 
         {
            xPos += m_lpV2SpriteInfo->Header.shOffX2;
            yPos += m_lpV2SpriteInfo->Header.shOffY2;
         } 
         else 
         {
            xPos += m_lpV2SpriteInfo->Header.shOffX1;
            yPos += m_lpV2SpriteInfo->Header.shOffY1;
         }
      } 
   } 
   else 
   {
      xPos += m_lpV2SpriteInfo->Header.shOffX1;
      yPos += m_lpV2SpriteInfo->Header.shOffY1;
   }
   
   BOOL boOutline = FALSE;
   if (FX->dwFX & FX_OUTLINE)
      boOutline = TRUE;
   
   
   sourceRect.left = 0;
   sourceRect.right = m_lpV2SpriteInfo->Header.dwWidth;
   sourceRect.top = 0;
   sourceRect.bottom = m_lpV2SpriteInfo->Header.dwHeight;
   sourceRectOri = sourceRect;
   destRect.left = xPos;
   destRect.right = xPos + sourceRect.right;
   destRect.top = yPos;
   destRect.bottom = yPos + sourceRect.bottom;
   
   RECT *Clip = FX->lpClipRect;
   RECT *Wrap = FX->lpWrapRect;
   BOOL bDither = FX->dwFX & FX_DITHER;
   FullScreen = FX->dwFX & FX_FIT2SCREEN;
   RECT Source[4];
   RECT Dest[4];
   
   int LeftBorder, RightBorder, TopBorder, BottomBorder;
   int FX2 = 0;
   if (FX->dwFX & FX_LEFTRIGHTMIRROR)
      FX2 = 1;
   
   LPDIRECTDRAWSURFACE lpSpriteSurface = m_lpV2SpriteInfo->Data.lpDDSurface;
   int Format = 0;
   LPDIRECTDRAWSURFACE ToBlit = FX->lpDirectDrawSurface;

   if (!ToBlit)
      ToBlit = lpDXDirectDraw->GetBackBufferDirect();
   
   if (Clip) 
   {
      // Clip Border ?
      LeftBorder   = Clip->left;
      RightBorder  = Clip->right;
      TopBorder    = Clip->top;
      BottomBorder = Clip->bottom;
      
      if (destRect.left > RightBorder) 
         return V2SPRITE_OK;
      if (destRect.right > RightBorder) 
      {
         sourceRect.right  = (m_lpV2SpriteInfo->Header.dwWidth) - (destRect.right   - RightBorder);
         destRect  .right  = RightBorder;
      }
      if (destRect.top > BottomBorder) 
         return V2SPRITE_OK;
      if (destRect.bottom > BottomBorder) 
      {
         sourceRect.bottom = (m_lpV2SpriteInfo->Header.dwHeight) - (destRect.bottom - BottomBorder);
         destRect  .bottom = BottomBorder;
      }
      if (destRect.right < LeftBorder) 
         return V2SPRITE_OK;
      if (destRect.left < LeftBorder) 
      {
         sourceRect.left  = (LeftBorder - destRect.left);
         destRect  .left  = LeftBorder;
      }
      if (destRect.bottom < TopBorder) 
         return V2SPRITE_OK;
      if (destRect.top < TopBorder) 
      {
         sourceRect.top  = (TopBorder - destRect.top);
         destRect  .top  = TopBorder;
      }
   }
   
   if (Wrap) 
   {
      // Wrap Sprite around the Screen ?
      LeftBorder   = Wrap->left;
      RightBorder  = Wrap->right;
      TopBorder    = Wrap->top;
      BottomBorder = Wrap->bottom;
      
      memset(Dest,   0, sizeof(RECT)*4);
      memset(Source, 0, sizeof(RECT)*4);
      
      BOOL Adjust;
      // Out of Bound ?
      do 
      {
         Adjust = FALSE;
         if (destRect.left >= RightBorder) 
         {
            destRect.left  -= RightBorder;
            destRect.right -= RightBorder;
            Adjust = TRUE;
         }
         if (destRect.right < LeftBorder) 
         {
            destRect.left  += RightBorder;
            destRect.right += RightBorder;
            Adjust = TRUE;
         }
         
         if (destRect.top >= BottomBorder) 
         {
            destRect.top    -= BottomBorder;
            destRect.bottom -= BottomBorder;
            Adjust = TRUE;
         }
         if (destRect.bottom < TopBorder) 
         {
            destRect.top    += BottomBorder;
            destRect.bottom += BottomBorder;
            Adjust = TRUE;
         }
      } while (Adjust);
      
      if (destRect.left < LeftBorder) 
      {
         Dest[0].left  = Dest[2].left  = RightBorder + destRect.left;
         Dest[0].right = Dest[2].right = RightBorder;
         Dest[1].left  = Dest[3].left  = LeftBorder;
         Dest[1].right = Dest[3].right = destRect.right;
         
         Source[0].left  = Source[2].left  = sourceRect.left;
         Source[0].right = Source[2].right = sourceRect.left - destRect.left;
         Source[1].left  = Source[3].left  = Source[0].right;
         Source[1].right = Source[3].right = sourceRect.right;
      } 
      else if (destRect.right > RightBorder) 
      {
         Dest[0].left  = Dest[2].left  = destRect.left;
         Dest[0].right = Dest[2].right = RightBorder;
         Dest[1].left  = Dest[3].left  = LeftBorder;
         Dest[1].right = Dest[3].right = destRect.right - RightBorder;
         
         Source[0].left  = Source[2].left  = sourceRect.left;
         Source[0].right = Source[2].right = sourceRect.left+RightBorder-destRect.left;
         Source[1].left  = Source[3].left  = Source[0].right;
         Source[1].right = Source[3].right = sourceRect.right;
      } 
      else 
      {
         Dest[0].left  = destRect.left;
         Dest[0].right = destRect.right;
         Dest[2].left  = destRect.left;
         Dest[2].right = destRect.right;
         
         Source[0].left  = sourceRect.left;
         Source[0].right = sourceRect.right;
         Source[2].left  = sourceRect.left;
         Source[2].right = sourceRect.right;
      }
      
      if (destRect.top < TopBorder) 
      {
         Dest[0].top    = Dest[1].top    = BottomBorder + destRect.top;
         Dest[0].bottom = Dest[1].bottom = BottomBorder;
         Dest[2].top    = Dest[3].top    = TopBorder;
         Dest[2].bottom = Dest[3].bottom = destRect.bottom;
         
         Source[0].top    = Source[1].top    = sourceRect.top;
         Source[0].bottom = Source[1].bottom = sourceRect.top - destRect.top;
         Source[2].top    = Source[3].top    = Source[0].bottom;
         Source[2].bottom = Source[3].bottom = sourceRect.bottom;
      } 
      else if (destRect.bottom > BottomBorder) 
      {
         Dest[0].top    = Dest[1].top    = destRect.top;
         Dest[0].bottom = Dest[1].bottom = BottomBorder;
         Dest[2].top    = Dest[3].top    = TopBorder;
         Dest[2].bottom = Dest[3].bottom = destRect.bottom - BottomBorder;
         
         Source[0].top    = Source[1].top    = sourceRect.top;
         Source[0].bottom = Source[1].bottom = sourceRect.top+BottomBorder - destRect.top;
         Source[2].top    = Source[3].top    = Source[0].bottom;
         Source[2].bottom = Source[3].bottom = sourceRect.bottom;
      } 
      else 
      {
         Dest[0].top    = destRect.top;
         Dest[0].bottom = destRect.bottom;
         Dest[1].top    = destRect.top;
         Dest[1].bottom = destRect.bottom;
         
         Source[0].top    = sourceRect.top;
         Source[0].bottom = sourceRect.bottom;
         Source[1].top    = sourceRect.top;
         Source[1].bottom = sourceRect.bottom;
      }
      if (FX2 == 1) 
      {
         LPDIRECTDRAWSURFACE lpNewSurface;
         DDSURFACEDESC Sprite;
         LPDDCOLORKEY lpddColorKey;
         DDBLTFX DDBltFx;
         
         ZeroMemory(&Sprite, sizeof(Sprite));
         Sprite.dwSize            = sizeof(Sprite);
         Sprite.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
         Sprite.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
         Sprite.dwHeight          = m_lpV2SpriteInfo->Header.dwHeight;
         Sprite.dwWidth           = m_lpV2SpriteInfo->Header.dwWidth;
         
         DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite, &lpNewSurface, NULL), "Draw Sprite");
         
         lpddColorKey = new DDCOLORKEY;
         lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, lpddColorKey); 
         DXDErrorHandle(lpNewSurface->SetColorKey(DDCKEY_SRCBLT, lpddColorKey), "Draw Sprite 2");
         
         memset(&DDBltFx, 0, sizeof(DDBltFx));
         DDBltFx.dwSize = sizeof(DDBltFx);
         DDBltFx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT;
         
         lpNewSurface->Blt(NULL, lpSpriteSurface, NULL, DDBLT_WAIT | DDBLT_DDFX, &DDBltFx);
         
         delete lpddColorKey;
         
         for (int i = 0; i < 4; i++) 
         {
            if (Format < 0) 
            {
               int Left   = Dest[i].left;
               int Right  = Dest[i].right;
               int Top    = Dest[i].top;
               int Bottom = Dest[i].bottom;
               Dest[i].left   = Left + ((Right-Left)*(100+Format)/200); // ((100 - format) / 2) / 100
               Dest[i].right  = Left + ((Right-Left)*(100+Format)*3/200); // (((100 - format) / 2) * 3) / 100
               Dest[i].top    = Top  + ((Bottom-Top)*(100+Format)/200); 
               Dest[i].bottom = Top  + ((Bottom-Top)*(100+Format)*3/200);
            }
            ToBlit->Blt(&Dest[i], lpNewSurface, &Source[i], DDBLT_WAIT | (m_lpV2SpriteInfo->Header.ushTransparency ? DDBLT_KEYSRC : NULL), NULL);
         }
         
         lpNewSurface->Release();
      } 
      else 
      {
         for (int i = 0; i < 4; i++) 
         {
            if (Format < 0) 
            {
               int Left   = Dest[i].left;
               int Right  = Dest[i].right;
               int Top    = Dest[i].top;
               int Bottom = Dest[i].bottom;
               Dest[i].left   = Left + ((Right-Left)*(100+Format)/200); // ((100 - format) / 2) / 100
               Dest[i].right  = Left + ((Right-Left)*(100+Format)*3/200); // (((100 - format) / 2) * 3) / 100
               Dest[i].top    = Top  + ((Bottom-Top)*(100+Format)/200); 
               Dest[i].bottom = Top  + ((Bottom-Top)*(100+Format)*3/200);
            }
            
            ToBlit->Blt(&Dest[i], lpSpriteSurface, &Source[i], DDBLT_WAIT | (m_lpV2SpriteInfo->Header.ushTransparency ? DDBLT_KEYSRC : NULL), NULL);
         } 
      }
      
   } 
   else 
   {
      if (FX2 == 1) 
      {
         LPDIRECTDRAWSURFACE lpNewSurface;
         DDSURFACEDESC Sprite;
         LPDDCOLORKEY lpddColorKey;
         DDBLTFX DDBltFx;
         
         ZeroMemory(&Sprite, sizeof(Sprite));
         Sprite.dwSize            = sizeof(Sprite);
         Sprite.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
         Sprite.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
         Sprite.dwHeight          = m_lpV2SpriteInfo->Header.dwHeight;
         Sprite.dwWidth           = m_lpV2SpriteInfo->Header.dwWidth;
         
         DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite, &lpNewSurface, NULL), "Draw Sprite 3");
         
         lpddColorKey = new DDCOLORKEY;
         lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, lpddColorKey); 
         
         DXDErrorHandle(lpNewSurface->SetColorKey(DDCKEY_SRCBLT, lpddColorKey), "Draw Sprite 4");
         
         memset(&DDBltFx, 0, sizeof(DDBltFx));
         DDBltFx.dwSize = sizeof(DDBltFx);
         DDBltFx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT;
         
         lpNewSurface->Blt(NULL, lpSpriteSurface, NULL, DDBLT_WAIT | DDBLT_DDFX, &DDBltFx);
         
         delete lpddColorKey;
         
         if (Format < 0) 
         {
            int Left   = destRect.left;
            int Right  = destRect.right;
            int Top    = destRect.top;
            int Bottom = destRect.bottom;
            destRect.left   = Left + ((Right-Left)*(100+Format)/200); // ((100 - format) / 2) / 100
            destRect.right  = Left + ((Right-Left)*(100+Format)*3/200); // (((100 - format) / 2) * 3) / 100
            destRect.top    = Top  + ((Bottom-Top)*(100+Format)/200); 
            destRect.bottom = Top  + ((Bottom-Top)*(100+Format)*3/200);
         }
         if (Format < 0)
         {
            ToBlit->Blt(&destRect, lpNewSurface, &sourceRect, DDBLT_WAIT | (m_lpV2SpriteInfo->Header.ushTransparency ? DDBLT_KEYSRC : NULL), NULL);
         }
         else if(wTransOption == 5)
         {
            LPDIRECTDRAWSURFACE lpNewSurface;
            DDSURFACEDESC Sprite;
            
            //Construit un sprite temporaire, on blit la zone du back dans ce sprite...
            ZeroMemory(&Sprite, sizeof(Sprite));
            Sprite.dwSize            = sizeof(Sprite);
            Sprite.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
            Sprite.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
            Sprite.dwHeight          = destRect.bottom-destRect.top;
            Sprite.dwWidth           = destRect.right -destRect.left;
            if(Sprite.dwWidth >0 && Sprite.dwHeight>0)
            {
               
               DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite, &lpNewSurface, NULL), "Draw Sprite 3");
               
               
               RECT destRectTmp;
               destRectTmp.left   = 0;
               destRectTmp.top    = 0;
               destRectTmp.right  = destRect.right -destRect.left;
               destRectTmp.bottom = destRect.bottom-destRect.top;
               //lpNewSurface->BltFast(0,0, ToBlit, &destRect, DDBLTFAST_WAIT);
               lpNewSurface->Blt(NULL, ToBlit,&destRect, DDBLT_WAIT | DDBLT_DDFX, &DDBltFx);
               // COntient la zone du back en dessous du sprite a blitter en alpha...
               
               DDCOLORKEY ddColorKey;
               lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 
               
               // si on clip la surface a dessiner...
               if(!m_lpV2SpriteInfo->Header.ushTransparency)
                  ddColorKey.dwColorSpaceLowValue = 65535;
               
               TransAlphaGlow(m_lpV2SpriteInfo->Data.lpDDSurface,lpNewSurface,0,0,0,0, sourceRect, ddColorKey.dwColorSpaceLowValue,16,0);
               //ToBlit->BltFast(destRect.left, destRect.top, lpNewSurface, &destRectTmp, DDBLTFAST_WAIT);
               ToBlit->Blt(&destRect, lpNewSurface, &destRectTmp, DDBLT_WAIT | DDBLT_DDFX, &DDBltFx);
               lpNewSurface->Release();
            }
         }
         else if(wTransOption == 6)
         {
            LPDIRECTDRAWSURFACE lpNewSurface2;
            DDSURFACEDESC Sprite2;
            
            //Construit un sprite temporaire, on blit la zone du back dans ce sprite...
            ZeroMemory(&Sprite2, sizeof(Sprite2));
            Sprite2.dwSize            = sizeof(Sprite2);
            Sprite2.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
            Sprite2.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
            Sprite2.dwHeight          = destRect.bottom-destRect.top;
            Sprite2.dwWidth           = destRect.right -destRect.left;

            if(Sprite2.dwWidth >0 && Sprite2.dwHeight>0)
            {
               DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite2, &lpNewSurface2, NULL), "Draw Sprite 5");
               RECT destRectTmp;
               destRectTmp.left   = 0;
               destRectTmp.top    = 0;
               destRectTmp.right  = destRect.right -destRect.left;
               destRectTmp.bottom = destRect.bottom-destRect.top;
               lpNewSurface2->BltFast(0,0, ToBlit, &destRect, DDBLTFAST_WAIT);
               // COntient la zone du back en dessous du sprite a blitter en alpha...
            
               DDCOLORKEY ddColorKey2;
               lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey2); 
            
               TransAlphaGlow(lpNewSurface,lpNewSurface2,0,0,0,0, sourceRect, ddColorKey2.dwColorSpaceLowValue,16,2);
               ToBlit->BltFast(destRect.left, destRect.top, lpNewSurface2, &destRectTmp, DDBLTFAST_WAIT);

               //////////////////////////
               if(boOutline)
               {
                  DDSURFACEDESC ddsd;
                  DDSURFACEDESC ddsd2;
                  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
                  ddsd.dwSize = sizeof(DDSURFACEDESC);
                  memset(&ddsd2, 0, sizeof(DDSURFACEDESC));
                  ddsd2.dwSize = sizeof(DDSURFACEDESC);
               
                  DDCOLORKEY ddColorKey;
                  lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 
               
                  if( ToBlit->IsLost() == DD_OK )
                  {
                     lpNewSurface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
                     ToBlit->Lock(NULL, &ddsd2, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
                  
                     USHORT *p = (USHORT *)ddsd.lpSurface  + sourceRect.top * (ddsd.lPitch>>1);
                     USHORT *q = (USHORT *)ddsd2.lpSurface + (destRect.top   * (ddsd2.lPitch>>1))+destRect.left;
                     int i, j, d;
                     for (j = sourceRect.top; j < sourceRect.bottom; j++) 
                     {
                        int dwStat = 0;
                        for (i = sourceRect.left,d=0 ; i < sourceRect.right; i++,d++) 
                        {
                           if(i < sourceRect.right-2)
                           {
                              if(p[i+1] !=ddColorKey.dwColorSpaceLowValue  && p[i+2] !=ddColorKey.dwColorSpaceLowValue && dwStat == 0)
                              {
                                 q[d] = (WORD)FX->dwOutlineColor;
                                 dwStat = 1;
                              }
                              else if(p[i] ==ddColorKey.dwColorSpaceLowValue && dwStat == 1)
                              {
                                 q[d] = (WORD)FX->dwOutlineColor;
                                 dwStat = 0;
                              }
                           }
                           else if(dwStat==1)
                           {
                              q[d+1] = (WORD)FX->dwOutlineColor;
                              dwStat = 0;
                           }
                        
                        }
                        p += ddsd.lPitch>>1;
                        q += ddsd2.lPitch>>1; 
                     }
                  
                     lpSpriteSurface->Unlock(ddsd.lpSurface);
                     ToBlit->Unlock(ddsd2.lpSurface);
                  }
               }
               //////////////////////////
               lpNewSurface2->Release();
            }
         }
         else
         {
            if(boOutline)
            {
               DDSURFACEDESC ddsd;
               DDSURFACEDESC ddsd2;
               memset(&ddsd, 0, sizeof(DDSURFACEDESC));
               ddsd.dwSize = sizeof(DDSURFACEDESC);
               memset(&ddsd2, 0, sizeof(DDSURFACEDESC));
               ddsd2.dwSize = sizeof(DDSURFACEDESC);
            
               DDCOLORKEY ddColorKey;
               lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 

               if( ToBlit->IsLost() == DD_OK )
               {
                  lpNewSurface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
                  ToBlit->Lock(NULL, &ddsd2, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
               
                  USHORT *p = (USHORT *)ddsd.lpSurface  + sourceRect.top * (ddsd.lPitch>>1);
                  USHORT *q = (USHORT *)ddsd2.lpSurface + (destRect.top   * (ddsd2.lPitch>>1))+destRect.left;
                  int i, j, d;
                  for (j = sourceRect.top; j < sourceRect.bottom; j++) 
                  {
                     int dwStat = 0;
                     for (i = sourceRect.left,d=0 ; i < sourceRect.right; i++,d++) 
                     {
                        if(p[i] != ddColorKey.dwColorSpaceLowValue)
                        {
                           q[d] = p[i];
                        }
                        if(i < sourceRect.right-2)
                        {
                           if(p[i+1] !=ddColorKey.dwColorSpaceLowValue  && p[i+2] !=ddColorKey.dwColorSpaceLowValue && dwStat == 0)
                           {
                              q[d] = (WORD)FX->dwOutlineColor;
                              dwStat = 1;
                           }
                           else if(p[i] ==ddColorKey.dwColorSpaceLowValue && dwStat == 1)
                           {
                              q[d] = (WORD)FX->dwOutlineColor;
                              dwStat = 0;
                           }
                        }
                        else if(dwStat==1)
                        {
                           q[d+1] = (WORD)FX->dwOutlineColor;
                           dwStat = 0;
                        }

                     }
                     p += ddsd.lPitch>>1;
                     q += ddsd2.lPitch>>1; 
                  }
               
                  lpSpriteSurface->Unlock(ddsd.lpSurface);
                  ToBlit->Unlock(ddsd2.lpSurface);
               }
            }
            else
               ToBlit->BltFast(destRect.left, destRect.top, lpNewSurface, &sourceRect, DDBLTFAST_WAIT | (m_lpV2SpriteInfo->Header.ushTransparency ? DDBLTFAST_SRCCOLORKEY : NULL));
         }
         lpNewSurface->Release();
      } 
      else  
      {
         if (FullScreen) 
         {
            ToBlit->Blt(NULL, lpSpriteSurface, &sourceRect, DDBLT_WAIT | (m_lpV2SpriteInfo->Header.ushTransparency ? DDBLT_KEYSRC : NULL), NULL);
         } 
         else 
         {
            
            if (Format < 0) 
            {
               int Left   = destRect.left;
               int Right  = destRect.right;
               int Top    = destRect.top;
               int Bottom = destRect.bottom;
               destRect.left   = Left + ((Right-Left)*(100+Format)/200); // ((100 - format) / 2) / 100
               destRect.right  = Left + ((Right-Left)*(100+Format)*3/200); // (((100 - format) / 2) * 3) / 100
               destRect.top    = Top  + ((Bottom-Top)*(100+Format)/200); 
               destRect.bottom = Top  + ((Bottom-Top)*(100+Format)*3/200);
            }
            if (Format < 0) 
            {
               ToBlit->Blt(&destRect, lpSpriteSurface, &sourceRect, DDBLT_WAIT | (m_lpV2SpriteInfo->Header.ushTransparency ? DDBLT_KEYSRC : NULL), NULL);
            } 
            else 
            {
               if (bDither) 
               {
                  if((destRect.right -destRect.left) >1 && (destRect.bottom -destRect.top) >0)
                  {
                     LPDIRECTDRAWSURFACE lpNewSurface;
                     DDSURFACEDESC Sprite;
         
                     //Construit un sprite temporaire, on blit la zone du back dans ce sprite...
                     ZeroMemory(&Sprite, sizeof(Sprite));
                     Sprite.dwSize            = sizeof(Sprite);
                     Sprite.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
                     Sprite.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
                     Sprite.dwHeight          = destRect.bottom-destRect.top;
                     Sprite.dwWidth           = destRect.right -destRect.left;

                     if(Sprite.dwWidth >0 && Sprite.dwHeight>0)
                     {
                        DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite, &lpNewSurface, NULL), "Draw Sprite 3");
         

                        RECT destRectTmp;
                        destRectTmp.left   = 0;
                        destRectTmp.top    = 0;
                        destRectTmp.right  = destRect.right -destRect.left;
                        destRectTmp.bottom = destRect.bottom-destRect.top;
 
 
                        lpNewSurface->BltFast(0,0, ToBlit, &destRect, DDBLTFAST_WAIT);

                        // COntient la zone du back en dessous du sprite a blitter en alpha...

                        DDCOLORKEY ddColorKey;
                        lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 

                        // si on clip la surface a dessiner...
                        if(!m_lpV2SpriteInfo->Header.ushTransparency)
                           ddColorKey.dwColorSpaceLowValue = 65535;

                        TransAlphaImproved(m_lpV2SpriteInfo->Data.lpDDSurface,lpNewSurface,0,0,0,0, sourceRect, 140, ddColorKey.dwColorSpaceLowValue,16);
                        ToBlit->BltFast(destRect.left, destRect.top, lpNewSurface, &destRectTmp, DDBLTFAST_WAIT);

                        lpNewSurface->Release();
                     }
                  }
               }
               else if(wTransOption == 3 && chAlphaLevel <255 && bSemiTrans)
               {
                  LPDIRECTDRAWSURFACE lpNewSurface;
                  DDSURFACEDESC Sprite;
         
                  //Construit un sprite temporaire, on blit la zone du back dans ce sprite...
                  ZeroMemory(&Sprite, sizeof(Sprite));
                  Sprite.dwSize            = sizeof(Sprite);
                  Sprite.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
                  Sprite.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
                  Sprite.dwHeight          = destRect.bottom-destRect.top;
                  Sprite.dwWidth           = destRect.right -destRect.left;
                  if(Sprite.dwWidth >0 && Sprite.dwHeight>0)
                  {
                           
                     DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite, &lpNewSurface, NULL), "Draw Sprite 3");
         

                     RECT destRectTmp;
                     destRectTmp.left   = 0;
                     destRectTmp.top    = 0;
                     destRectTmp.right  = destRect.right -destRect.left;
                     destRectTmp.bottom = destRect.bottom-destRect.top;
 
 
                     lpNewSurface->BltFast(0,0, ToBlit, &destRect, DDBLTFAST_WAIT);

                     // COntient la zone du back en dessous du sprite a blitter en alpha...

                     DDCOLORKEY ddColorKey;
                     lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 

                     // si on clip la surface a dessiner...
                     if(!m_lpV2SpriteInfo->Header.ushTransparency)
                        ddColorKey.dwColorSpaceLowValue = 65535;

                     TransAlphaImproved(m_lpV2SpriteInfo->Data.lpDDSurface,lpNewSurface,0,0,0,0, sourceRect, chAlphaLevel, ddColorKey.dwColorSpaceLowValue,16);
                     ToBlit->BltFast(destRect.left, destRect.top, lpNewSurface, &destRectTmp, DDBLTFAST_WAIT);

                     lpNewSurface->Release();
                  }
               }
               else if(wTransOption == 4)
               {
                  LPDIRECTDRAWSURFACE lpNewSurface;
                  DDSURFACEDESC Sprite;
         
                  //Construit un sprite temporaire, on blit la zone du back dans ce sprite...
                  ZeroMemory(&Sprite, sizeof(Sprite));
                  Sprite.dwSize            = sizeof(Sprite);
                  Sprite.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
                  Sprite.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
                  Sprite.dwHeight          = destRect.bottom-destRect.top;
                  Sprite.dwWidth           = destRect.right -destRect.left;
                  if(Sprite.dwWidth >0 && Sprite.dwHeight>0)
                  {
         
                     DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite, &lpNewSurface, NULL), "Draw Sprite 3");
         

                     RECT destRectTmp;
                     destRectTmp.left   = 0;
                     destRectTmp.top    = 0;
                     destRectTmp.right  = destRect.right -destRect.left;
                     destRectTmp.bottom = destRect.bottom-destRect.top;
                     lpNewSurface->BltFast(0,0, ToBlit, &destRect, DDBLTFAST_WAIT);
                     // COntient la zone du back en dessous du sprite a blitter en alpha...

                     DDCOLORKEY ddColorKey;
                     lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 

                     // si on clip la surface a dessiner...
                     if(!m_lpV2SpriteInfo->Header.ushTransparency)
                        ddColorKey.dwColorSpaceLowValue = 65535;

                     TransAlphaGlow(m_lpV2SpriteInfo->Data.lpDDSurface,lpNewSurface,0,0,0,0, sourceRect, ddColorKey.dwColorSpaceLowValue,16,0);
                     ToBlit->BltFast(destRect.left, destRect.top, lpNewSurface, &destRectTmp, DDBLTFAST_WAIT);
                     lpNewSurface->Release();
                  }
               }
               else if(wTransOption == 5)
               {
                  LPDIRECTDRAWSURFACE lpNewSurface;
                  DDSURFACEDESC Sprite;
         
                  //Construit un sprite temporaire, on blit la zone du back dans ce sprite...
                  ZeroMemory(&Sprite, sizeof(Sprite));
                  Sprite.dwSize            = sizeof(Sprite);
                  Sprite.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
                  Sprite.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
                  Sprite.dwHeight          = destRect.bottom-destRect.top;
                  Sprite.dwWidth           = destRect.right -destRect.left;
                  if(Sprite.dwWidth >0 && Sprite.dwHeight>0)
                  {
         
                     DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite, &lpNewSurface, NULL), "Draw Sprite 3");
         

                     RECT destRectTmp;
                     destRectTmp.left   = 0;
                     destRectTmp.top    = 0;
                     destRectTmp.right  = destRect.right -destRect.left;
                     destRectTmp.bottom = destRect.bottom-destRect.top;
                     lpNewSurface->BltFast(0,0, ToBlit, &destRect, DDBLTFAST_WAIT);
                     // COntient la zone du back en dessous du sprite a blitter en alpha...

                     DDCOLORKEY ddColorKey;
                     lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 

                     // si on clip la surface a dessiner...
                     if(!m_lpV2SpriteInfo->Header.ushTransparency)
                        ddColorKey.dwColorSpaceLowValue = 65535;

                     TransAlphaGlow(m_lpV2SpriteInfo->Data.lpDDSurface,lpNewSurface,0,0,0,0, sourceRect, ddColorKey.dwColorSpaceLowValue,16,0);
                     ToBlit->BltFast(destRect.left, destRect.top, lpNewSurface, &destRectTmp, DDBLTFAST_WAIT);
                     lpNewSurface->Release();
                  }
               }
               else if(wTransOption == 6)
               {
                  LPDIRECTDRAWSURFACE lpNewSurface;
                  DDSURFACEDESC Sprite;
         
                  //Construit un sprite temporaire, on blit la zone du back dans ce sprite...
                  ZeroMemory(&Sprite, sizeof(Sprite));
                  Sprite.dwSize            = sizeof(Sprite);
                  Sprite.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
                  Sprite.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
                  Sprite.dwHeight          = destRect.bottom-destRect.top;
                  Sprite.dwWidth           = destRect.right -destRect.left;

                  if(Sprite.dwWidth >0 && Sprite.dwHeight>0)
                  {
                     DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite, &lpNewSurface, NULL), "Draw Sprite 3");

                     RECT destRectTmp;
                     destRectTmp.left   = 0;
                     destRectTmp.top    = 0;
                     destRectTmp.right  = destRect.right -destRect.left;
                     destRectTmp.bottom = destRect.bottom-destRect.top;
                     lpNewSurface->BltFast(0,0, ToBlit, &destRect, DDBLTFAST_WAIT);
                     // COntient la zone du back en dessous du sprite a blitter en alpha...

                     DDCOLORKEY ddColorKey;
                     lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 

                     // si on clip la surface a dessiner...
                     if(!m_lpV2SpriteInfo->Header.ushTransparency)
                        ddColorKey.dwColorSpaceLowValue = 65535;
                     TransAlphaGlow(m_lpV2SpriteInfo->Data.lpDDSurface,lpNewSurface,0,0,0,0, sourceRect, ddColorKey.dwColorSpaceLowValue,16,2);
                     ToBlit->BltFast(destRect.left, destRect.top, lpNewSurface, &destRectTmp, DDBLTFAST_WAIT);

                     if(boOutline)
                     {
                        DDSURFACEDESC ddsd;
                        DDSURFACEDESC ddsd2;
                        memset(&ddsd, 0, sizeof(DDSURFACEDESC));
                        ddsd.dwSize = sizeof(DDSURFACEDESC);
                        memset(&ddsd2, 0, sizeof(DDSURFACEDESC));
                        ddsd2.dwSize = sizeof(DDSURFACEDESC);
                  
                        DDCOLORKEY ddColorKey;
                        lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 

                        if( ToBlit->IsLost() == DD_OK )
                        {
                           lpSpriteSurface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
                           ToBlit->Lock(NULL, &ddsd2, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
                     
                           USHORT *p = (USHORT *)ddsd.lpSurface  + sourceRect.top * (ddsd.lPitch>>1);
                           USHORT *q = (USHORT *)ddsd2.lpSurface + (destRect.top   * (ddsd2.lPitch>>1))+destRect.left;
                           int i, j, d;
                           for (j = sourceRect.top; j < sourceRect.bottom; j++) 
                           {
                              int dwStat = 0;
                              for (i = sourceRect.left,d=0 ; i < sourceRect.right; i++,d++) 
                              {
                                 if(i < sourceRect.right-2)
                                 {
                                    if(p[i+1] !=ddColorKey.dwColorSpaceLowValue  && p[i+2] !=ddColorKey.dwColorSpaceLowValue && dwStat == 0)
                                    {
                                       q[d] = (WORD)FX->dwOutlineColor;
                                       dwStat = 1;
                                    }
                                    else if(p[i] ==ddColorKey.dwColorSpaceLowValue && dwStat == 1)
                                    {
                                       q[d] = (WORD)FX->dwOutlineColor;
                                       dwStat = 0;
                                    }
                                 }
                                 else if(dwStat==1)
                                 {
                                    q[d+1] = (WORD)FX->dwOutlineColor;
                                    dwStat = 0;
                                 }

                              }
                              p += ddsd.lPitch>>1;
                              q += ddsd2.lPitch>>1; 
                           }
                     
                           lpSpriteSurface->Unlock(ddsd.lpSurface);
                           ToBlit->Unlock(ddsd2.lpSurface);
                        }
                     }
                     lpNewSurface->Release();
                  }
               }
               else if(wTransOption == 7)
               {
                  LPDIRECTDRAWSURFACE lpNewSurface;
                  DDSURFACEDESC Sprite;
         
                  //Construit un sprite temporaire, on blit la zone du back dans ce sprite...
                  ZeroMemory(&Sprite, sizeof(Sprite));
                  Sprite.dwSize            = sizeof(Sprite);
                  Sprite.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
                  Sprite.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
                  Sprite.dwHeight          = destRect.bottom-destRect.top;
                  Sprite.dwWidth           = destRect.right -destRect.left;

                  if(Sprite.dwWidth >0 && Sprite.dwHeight>0)
                  {
                     DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite, &lpNewSurface, NULL), "Draw Sprite 3");

                     RECT destRectTmp;
                     destRectTmp.left   = 0;
                     destRectTmp.top    = 0;
                     destRectTmp.right  = destRect.right -destRect.left;
                     destRectTmp.bottom = destRect.bottom-destRect.top;
                     lpNewSurface->BltFast(0,0, ToBlit, &destRect, DDBLTFAST_WAIT);
                     // COntient la zone du back en dessous du sprite a blitter en alpha...

                     DDCOLORKEY ddColorKey;
                     lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 
                     DDCOLORKEY ddColorKey2;
                     ddColorKey2.dwColorSpaceLowValue = 0;
                     
                     // si on clip la surface a dessiner...
                     if(!m_lpV2SpriteInfo->Header.ushTransparency)
                        ddColorKey.dwColorSpaceLowValue = 65535;
                     
                     TransAlphaGlow(m_lpV2SpriteInfo->Data.lpDDSurface,lpNewSurface,0,0,0,0, sourceRect, ddColorKey.dwColorSpaceLowValue,16,2,OVERLAP);
                     ToBlit->BltFast(destRect.left, destRect.top, lpNewSurface, &destRectTmp, DDBLTFAST_WAIT);

                     lpNewSurface->Release();
                  }
               }
               else if(wTransOption == 8)
               {
                  DDSURFACEDESC ddsd;
                  DDSURFACEDESC ddsd2;
                  memset(&ddsd, 0, sizeof(DDSURFACEDESC));
                  ddsd.dwSize = sizeof(DDSURFACEDESC);
                  memset(&ddsd2, 0, sizeof(DDSURFACEDESC));
                  ddsd2.dwSize = sizeof(DDSURFACEDESC);
                  
                  DDCOLORKEY ddColorKey;
                  lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 
                  
                  if( ToBlit->IsLost() == DD_OK )
                  {
                     lpSpriteSurface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
                     ToBlit->Lock(NULL, &ddsd2, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
                     
                     USHORT *p = (USHORT *)ddsd.lpSurface  + sourceRect.top * (ddsd.lPitch>>1);
                     USHORT *q = (USHORT *)ddsd2.lpSurface + (destRect.top   * (ddsd2.lPitch>>1))+destRect.left;
                     int i, j, d;
                     for (j = sourceRect.top; j < sourceRect.bottom; j++) 
                     {
                        for (i = sourceRect.left,d=0 ; i < sourceRect.right; i++,d++) 
                        {
                           if(p[i] == ddColorKey.dwColorSpaceLowValue)
                           {
                              q[d] = 0x0000;
                           }
                        }
                        p += ddsd.lPitch>>1;
                        q += ddsd2.lPitch>>1; 
                     }
                     
                     lpSpriteSurface->Unlock(ddsd.lpSurface);
                     ToBlit->Unlock(ddsd2.lpSurface);
                  }
               }
               else  
               {
                  if(boOutline)
                  {
                     DDSURFACEDESC ddsd;
                     DDSURFACEDESC ddsd2;
                     memset(&ddsd, 0, sizeof(DDSURFACEDESC));
                     ddsd.dwSize = sizeof(DDSURFACEDESC);
                     memset(&ddsd2, 0, sizeof(DDSURFACEDESC));
                     ddsd2.dwSize = sizeof(DDSURFACEDESC);
                  
                     DDCOLORKEY ddColorKey;
                     lpSpriteSurface->GetColorKey(DDCKEY_SRCBLT, &ddColorKey); 

                     if( ToBlit->IsLost() == DD_OK )
                     {
                        lpSpriteSurface->Lock(NULL, &ddsd, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
                        ToBlit->Lock(NULL, &ddsd2, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
                     
                        USHORT *p = (USHORT *)ddsd.lpSurface  + sourceRect.top * (ddsd.lPitch>>1);
                        USHORT *q = (USHORT *)ddsd2.lpSurface + (destRect.top   * (ddsd2.lPitch>>1))+destRect.left;
                        int i, j, d;
                        for (j = sourceRect.top; j < sourceRect.bottom; j++) 
                        {
                           int dwStat = 0;
                           for (i = sourceRect.left,d=0 ; i < sourceRect.right; i++,d++) 
                           {
                              if(p[i] != ddColorKey.dwColorSpaceLowValue)
                              {
                                 q[d] = p[i];
                              }
                              if(i < sourceRect.right-2)
                              {
                                 if(p[i+1] !=ddColorKey.dwColorSpaceLowValue  && p[i+2] !=ddColorKey.dwColorSpaceLowValue && dwStat == 0)
                                 {
                                    q[d] = (WORD)FX->dwOutlineColor;
                                    dwStat = 1;
                                 }
                                 else if(p[i] ==ddColorKey.dwColorSpaceLowValue && dwStat == 1)
                                 {
                                    q[d] = (WORD)FX->dwOutlineColor;
                                    dwStat = 0;
                                 }
                              }
                              else if(dwStat==1)
                              {
                                 q[d+1] = (WORD)FX->dwOutlineColor;
                                 dwStat = 0;
                              }

                           }
                           p += ddsd.lPitch>>1;
                           q += ddsd2.lPitch>>1; 
                        }
                     
                        lpSpriteSurface->Unlock(ddsd.lpSurface);
                        ToBlit->Unlock(ddsd2.lpSurface);
                     }
                  }
                  else
                     ToBlit->BltFast(destRect.left, destRect.top, lpSpriteSurface, &sourceRect, DDBLTFAST_WAIT | (m_lpV2SpriteInfo->Header.ushTransparency ? DDBLTFAST_SRCCOLORKEY : NULL));
               }
            } 
         }
      }	
   }
   return V2SPRITE_OK;
}

/**********************************************************************************
 * Draw a NO COLOR KEY Sprite to a Surface Memory. 
 *
 * @PARAM xPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM yPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM FX This specifed any special behavior when rendering the sprite. Please 
 * refer to V2SPRITEFX for more information about this parameters.
 *
 * @REMARK Here is how to decompress the NCK sprite format. The NCK format is a 
 * palettize format, and it is a chunck base graphic model. Each chucnk have the 
 * current format and there must be at least 1 chunck per sprite. 
 * The philosophy behind the NCK format is to jump over the transparency specified 
 * by the color key. 
 * 
 * <b>WORD</b>     Position offset, absolute value from the starting horizontal 
 * position of the sprite. 
 * <b>BYTE</b>     Number of DWORD blocks in the data chunck. 
 * <b>BYTE</b>     Remaining byte in non-aligned data chunck. 
 * <b>BYTE</b>     This is flag specifing the type of the chunck. There is currently
 * two types of chuncks. The first one is a NCK_DISPLAY chucnk and the other is a 
 * NCK_SHADOW chunck. Each chunck as a dirrent behavior and format. 
 * 
 * Here is the following chunck format for a NCK_DISPLAY chunck. Its function is to 
 * display graphic to the screen. Information in the buffer will be parse thru a palette
 * before being rendered to the Surface. 
 * <b>DATA</b>     Buffer of size ( 'Number of DWORD' * sizeof( DWORD ) + 
 * 'Remaining bytes' ) BYTEs. 
 * <b>BYTE</b>     This is the type of the next block. It may be a NCK_BREAK, 
 * meaning that this was the last chunck in the sprite. A NCK_SAME_LINE, meaning
 * that the next chunck information is on the same vertical line or a NCK_NEXT_LINE
 * meaning that the information is on the next vertical line. 
 *
 * @RETURN Return value, a non zero value means an error have occured. Return 
 * value will be V2SPRITE_OK if everything worked fine, or ERR_FORMATNOTHANDLE if the 
 * V2SPRITE format is not handled.
 //*********************************************************************************/
HRESULT CV2Sprite::DrawNCKNormalSprite(LONG xPos,LONG yPos,LPV2SPRITEFX FX)
{
   /*if( !g_App.isFocus() && !Custom.gWindowed)//BLBL Freeze test
   {
      return V2SPRITE_OK;
   }*/
   
   // Variable
   DDSURFACEDESC Surface;    // Dest Surface Desc.
   SizeMemory(&Surface);
   // Result.
   HRESULT hr; 
   BOOL    boExit = FALSE;
   // Clipping Variable
   long          wClipXl = 0;     // X left   Clipping Value.
   long          wClipXr = 65536;     // X right  Clipping Value.
   long          wClipYt = 0;     // Y top    Clipping Value.
   long          wClipYb = 65536;    // Y bottom Clipping Value.
   // NonBackgroundSurface
   LPDIRECTDRAWSURFACE lpDirectDrawSurface = lpDXDirectDraw->GetBackBufferDirect();
   // General FX
   BOOL boOutline = FALSE;
   BOOL boInvisible = FALSE;
   
   RECT sourceRect, destRect;
   
   // Applying Corection
   if (FX) 
   {
      if (!(FX->dwFX & FX_NOCORRECTION)) 
      {
         xPos += m_lpV2SpriteInfo->Header.shOffX1;
         yPos += m_lpV2SpriteInfo->Header.shOffY1;
      }
   } 
   else 
   {
      xPos += m_lpV2SpriteInfo->Header.shOffX1;
      yPos += m_lpV2SpriteInfo->Header.shOffY1;
   }
   
   sourceRect.left = 0;
   sourceRect.right = m_lpV2SpriteInfo->Header.dwWidth;
   sourceRect.top = 0;
   sourceRect.bottom = m_lpV2SpriteInfo->Header.dwHeight;
   
   destRect.left = xPos;
   destRect.right = xPos + sourceRect.right;
   destRect.top = yPos;
   destRect.bottom = yPos + sourceRect.bottom;
   
   // Do FX
   if (FX) 
   {
      if (FX->lpClipRect) 
      {
         // Clipping FX ?
         long lLeftBorder   = FX->lpClipRect->left;
         long lRightBorder  = FX->lpClipRect->right;
         long lTopBorder    = FX->lpClipRect->top;
         long lBottomBorder = FX->lpClipRect->bottom;
         
         WORD wWidth  = m_lpV2SpriteInfo->Header.dwWidth;
         WORD wHeight = m_lpV2SpriteInfo->Header.dwHeight;
         
         // Out of the Clipping Border.
         if (xPos > lRightBorder)         
            return V2SPRITE_OK;
         if (yPos > lBottomBorder)        
            return V2SPRITE_OK;
         if (xPos + wWidth < lLeftBorder) 
            return V2SPRITE_OK;
         if (yPos + wHeight < lTopBorder) 
            return V2SPRITE_OK;
         
         
         if (xPos < lLeftBorder)  // Clipping ?
         {
            // Do we clip on the left ?
            wClipXl = lLeftBorder - xPos;
            xPos    = lLeftBorder;
            sourceRect.left  = (lLeftBorder - destRect.left);
            destRect  .left  = lLeftBorder;
         }
         
         if (xPos + wWidth > lRightBorder) 
         {
            // Do we clip on the right ?
            wClipXr = lRightBorder-xPos+wClipXl;
            sourceRect.right  = (m_lpV2SpriteInfo->Header.dwWidth) - (destRect.right   - lRightBorder);
            destRect  .right  = lRightBorder;
         }
         
         if (yPos < lTopBorder) 
         {
            // Do we clip on the top ?
            wClipYt = lTopBorder;
            sourceRect.top  = (lTopBorder - destRect.top);
            destRect  .top  = lTopBorder;
         }
         
         if (yPos + wHeight > lBottomBorder) 
         {
            // Do we clip on the bottom ?
            wClipYb = lBottomBorder;
            sourceRect.bottom = (m_lpV2SpriteInfo->Header.dwHeight) - (destRect.bottom - lBottomBorder);
            destRect  .bottom = lBottomBorder;
         }
      }
      
      if (FX->lpDirectDrawSurface) 
      {
         // Non Background Surface ?
         lpDirectDrawSurface = FX->lpDirectDrawSurface;
      }
      
      if (FX->dwFX & FX_OUTLINE)
         boOutline = TRUE;
      
      if (FX->lpWrapRect) 
      {
         // Wrap Sprite around the Screen ?
         long LeftBorder   = FX->lpWrapRect->left;
         long RightBorder  = FX->lpWrapRect->right;
         long TopBorder    = FX->lpWrapRect->top;
         long BottomBorder = FX->lpWrapRect->bottom;
         
         WORD wWidth  = m_lpV2SpriteInfo->Header.dwWidth;
         WORD wHeight = m_lpV2SpriteInfo->Header.dwHeight;
         
         RECT Dest[4];
         RECT Source[4];
         
         memset(Dest,   0, sizeof(RECT)*4);
         memset(Source, 0, sizeof(RECT)*4);
         
         BOOL Adjust;
         // Out of Bound ?
         do 
         {
            Adjust = FALSE;
            if (destRect.left >= RightBorder) 
            {
               destRect.left  -= RightBorder;
               destRect.right -= RightBorder;
               Adjust = TRUE;
            }
            if (destRect.left < LeftBorder) 
            {
               destRect.left  += RightBorder;
               destRect.right += RightBorder;
               Adjust = TRUE;
            }
            if (destRect.top >= BottomBorder) 
            {
               destRect.top    -= BottomBorder;
               destRect.bottom -= BottomBorder;
               Adjust = TRUE;
            }
            if (destRect.top < TopBorder) 
            {
               destRect.top    += BottomBorder;
               destRect.bottom += BottomBorder;
               Adjust = TRUE;
            }
         } while (Adjust);
         
         if (destRect.left < LeftBorder) 
         {
            Dest[0].left  = Dest[2].left  = RightBorder + destRect.left;
            Dest[0].right = Dest[2].right = RightBorder;
            Dest[1].left  = Dest[3].left  = LeftBorder;
            Dest[1].right = Dest[3].right = destRect.right;
            
            Source[0].left  = Source[2].left  = sourceRect.left;
            Source[0].right = Source[2].right = sourceRect.left - destRect.left;
            Source[1].left  = Source[3].left  = Source[0].right;
            Source[1].right = Source[3].right = sourceRect.right;
         } 
         else if (destRect.right > RightBorder) 
         {
            Dest[0].left  = Dest[2].left  = destRect.left;
            Dest[0].right = Dest[2].right = RightBorder;
            Dest[1].left  = Dest[3].left  = LeftBorder;
            Dest[1].right = Dest[3].right = destRect.right - RightBorder;
            
            Source[0].left  = Source[2].left  = sourceRect.left;
            Source[0].right = Source[2].right = sourceRect.left+RightBorder-destRect.left;
            Source[1].left  = Source[3].left  = Source[0].right;
            Source[1].right = Source[3].right = sourceRect.right;
         } 
         else 
         {
            Dest[0].left  = destRect.left;
            Dest[0].right = destRect.right;
            Dest[2].left  = destRect.left;
            Dest[2].right = destRect.right;
            
            Source[0].left  = sourceRect.left;
            Source[0].right = sourceRect.right;
            Source[2].left  = sourceRect.left;
            Source[2].right = sourceRect.right;
         }
         
         if (destRect.top < TopBorder) 
         {
            Dest[0].top    = Dest[1].top    = BottomBorder + destRect.top;
            Dest[0].bottom = Dest[1].bottom = BottomBorder;
            Dest[2].top    = Dest[3].top    = TopBorder;
            Dest[2].bottom = Dest[3].bottom = destRect.bottom;
            
            Source[0].top    = Source[1].top    = sourceRect.top;
            Source[0].bottom = Source[1].bottom = sourceRect.top - destRect.top;
            Source[2].top    = Source[3].top    = Source[0].bottom;
            Source[2].bottom = Source[3].bottom = sourceRect.bottom;
         } 
         else if (destRect.bottom > BottomBorder) 
         {
            Dest[0].top    = Dest[1].top    = destRect.top;
            Dest[0].bottom = Dest[1].bottom = BottomBorder;
            Dest[2].top    = Dest[3].top    = TopBorder;
            Dest[2].bottom = Dest[3].bottom = destRect.bottom - BottomBorder;
            
            Source[0].top    = Source[1].top    = sourceRect.top;
            Source[0].bottom = Source[1].bottom = sourceRect.top+BottomBorder - destRect.top;
            Source[2].top    = Source[3].top    = Source[0].bottom;
            Source[2].bottom = Source[3].bottom = sourceRect.bottom;
         } 
         else 
         {
            Dest[0].top    = destRect.top;
            Dest[0].bottom = destRect.bottom;
            Dest[1].top    = destRect.top;
            Dest[1].bottom = destRect.bottom;
            
            Source[0].top    = sourceRect.top;
            Source[0].bottom = sourceRect.bottom;
            Source[1].top    = sourceRect.top;
            Source[1].bottom = sourceRect.bottom;
         }
         
         V2SPRITEFX SpriteFX;
         ZeroMemory(&SpriteFX, sizeof(V2SPRITEFX));
         SpriteFX.lpDirectDrawSurface = FX->lpDirectDrawSurface;
         SpriteFX.dwFX = FX_NOCORRECTION;
         
         SpriteFX.lpClipRect = &Dest[0];
         DrawNCKNormalSprite(Dest[0].left - Source[0].left, Dest[0].top - Source[0].top, &SpriteFX);
         SpriteFX.lpClipRect = &Dest[1];
         DrawNCKNormalSprite(Dest[1].left - Source[1].left, Dest[1].top - Source[1].top, &SpriteFX);
         SpriteFX.lpClipRect = &Dest[2];
         DrawNCKNormalSprite(Dest[2].left - Source[2].left, Dest[2].top - Source[2].top, &SpriteFX);
         SpriteFX.lpClipRect = &Dest[3];
         DrawNCKNormalSprite(Dest[3].left - Source[3].left, Dest[3].top - Source[3].top, &SpriteFX);
         
         return V2SPRITE_OK;
      }
      
      if (FX->dwFX & FX_NODRAW) {
         
         boInvisible = TRUE;
      }
      
   } // End FX
   
   
   // Get the Pointer of the Back Buffer. (Lock the Back Buffer)
   const int MaxTries = 5;
   int tries = 0;   
   do 
   {
      hr = lpDirectDrawSurface->Lock(NULL, &Surface, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
      if (hr == DDERR_SURFACELOST) 
      {
         Sleep( 5 );
         /*if( !g_App.isFocus() )//BLBL Freeze test
            return V2SPRITE_OK;*/
         
         lpDirectDrawSurface->Restore();
      } 
      else 
         break;
      if( tries++ > MaxTries )
         return V2SPRITE_OK;          
   } while (1);
   
   LPBYTE lpbSource = m_lpV2SpriteInfo->Data.lpbChunck;				   // The Source.
   long   lPitch = Surface.lPitch;		                              // Pitch of the Back Buffer.
   LPBYTE lpbDestStart = (LPBYTE)Surface.lpSurface + (yPos * lPitch) + (xPos<<1);  // Destination Start.
   LPBYTE lpbDestCurr = 0;
   long   wByte;						// Number of Byte to be Blit.
   long   wByteTemp;
   long   wAdv;     // Number of Pixel to advance before drawing sprite.
   long   wClipXlTemp;
   WORD   wColor = 0;
   WORD   wMixColor = 0;
   WORD   wRed;
   WORD   wGreen;
   WORD   wBlue;
   WORD   wPal;
   BYTE   bRDown = lpDXDirectDraw->bRDown;
   BYTE   bGDown = lpDXDirectDraw->bGDown;
   BYTE   bBDown = lpDXDirectDraw->bBDown;
   BYTE   bRUp = lpDXDirectDraw->bRUp;
   BYTE   bGUp = lpDXDirectDraw->bGUp;
   BYTE   bBUp = lpDXDirectDraw->bBUp;
   WORD   wRMask = lpDXDirectDraw->wRMask;
   WORD   wGMask = lpDXDirectDraw->wGMask;
   WORD   wBMask = lpDXDirectDraw->wBMask;
   
   int i;
   
   // This will decompress the NCK Folowing the Definition above.
   int STPos = yPos;
InnerLoop:
   if (yPos >= wClipYb) 
   {
      // Do we end because of Clipping ?
      goto EndLoop;
   }
   
   wClipXlTemp = wClipXl;
   // We calculate the offset of the Destination (Dest Start + Pos Offset)	
   wAdv = *(LPWORD)lpbSource;
   lpbSource++;
   lpbSource++;
   
   if (wClipXlTemp) 
   {
      // Do we cut in the "transparent" pixel ?
      if (wAdv > wClipXlTemp) {
         wAdv -= wClipXlTemp;
         wClipXlTemp = 0;
      } else {
         wClipXlTemp -= wAdv;
         wAdv = 0;
      }
   }
   
   // We then Compute the number of byte to be blitted
   // Number of DWORD Block * 4 + Number of remaining BYTE Block
   // This has be devided in DWORD and BYTE block for futur optimization.
   wByte  = ((*(LPBYTE)(lpbSource)) << 2);
   lpbSource++;
   wByte  += *(LPBYTE)(lpbSource);
   lpbSource++;
   wByteTemp = wByte;
   
   if (wAdv < wClipXr - wClipXl) {
      
      lpbDestCurr = lpbDestStart + (wAdv << 1);
      
      wByte = (signed(((wClipXr - (wClipXl-wClipXlTemp)) - wAdv) < wByte ? (wClipXr - (wClipXl-wClipXlTemp)) - wAdv : wByte));
      
      // Copy the memory.
      if (wClipYt <= yPos) 
      { 
         if (*(LPBYTE)(lpbSource) == V2_NCK_SHADOW) 
         {
            lpbSource++;
            
            if (wByte > wClipXlTemp)  
            {
               wByte -= wClipXlTemp;
               
               if (wByte > 1) 
               {
                  if (!(yPos % 2)) 
                  {
                     if (((int)lpbDestCurr % 4)) 
                     {
                        wByte--;
                        lpbDestCurr++;
                        lpbDestCurr++;
                     }
                  } 
                  else if (!((int)lpbDestCurr % 4)) 
                  {
                     lpbDestCurr++;
                     lpbDestCurr++;
                     wByte--;
                  } 
               }
               
               if (!boInvisible) 
               {
                  while (wByte > 0 && !OVERLAP) 
                  {
                     wByte -= 2;
                     *lpbDestCurr = 0;
                     lpbDestCurr++;
                     *lpbDestCurr = 0;
                     lpbDestCurr++;
                     lpbDestCurr++;
                     lpbDestCurr++;
                  } 
               }
            }
            wByteTemp = 0; 
         } 
         else 
         {
            lpbSource++;
            if (wByte > wClipXlTemp) 
            {
               if (!boInvisible) 
               {
                  LPWORD lpwTmp = (LPWORD)lpbDestCurr;
                  
                  for (i = 0; i < (wByte-wClipXlTemp); i++) 
                  {
                     wPal = (*(BYTE *)(lpbSource+wClipXlTemp+i))*3;
                     
                     // Normal 
                     wRed   = (m_pRefDataPal[wPal])>>bRDown;
                     wGreen = (m_pRefDataPal[wPal+1])>>bGDown;
                     wBlue  = (m_pRefDataPal[wPal+2])>>bBDown;
                     
                     wColor = (wRed<<bRUp)+(wGreen<<bGUp)+(wBlue<<bBUp);
                     *lpwTmp++ = wColor;
                  }
               }
               if (boOutline) 
               {  
                  if (wByte == wByteTemp) 
                  {
                     ((LPWORD)(lpbDestCurr))[0] = (WORD)FX->dwOutlineColor;
                  }
                  ((LPWORD)(lpbDestCurr))[wByte-1-wClipXlTemp] = (WORD)FX->dwOutlineColor;
               }
            }
         }
      } 
      else 
      {
         if (*(LPBYTE)(lpbSource) == V2_NCK_SHADOW) 
         {
            wByteTemp = 0;   
         }
         lpbSource++;
      }
      // What's the Next Step (Default is on the same line)
   } 
   else 
   {
 		   if (*(LPBYTE)(lpbSource) == V2_NCK_SHADOW) 
         {
            wByteTemp = 0;   
         }
         lpbSource++;
   }
   lpbSource += wByteTemp;
   
   switch (*(LPBYTE)(lpbSource)) 
   {
      // Next Line.
			case V2_NCK_NEXT_LINE: 
            lpbDestStart += lPitch; yPos++; 
            break;
            // End of Decompression.
         case V2_NCK_BREAK: 
            goto EndLoop; break;
            
   }
   lpbSource++;
   
   // Get the Next Chunck.
   goto InnerLoop;
   EndLoop: ;
         
   // Unlock the Back Buffer.
   lpDirectDrawSurface->Unlock(Surface.lpSurface);
   return V2SPRITE_OK;
}

/**********************************************************************************
 * Draw a NO COLOR KEY Sprite to a Surface Memory but reversed. 
 *
 * @PARAM yPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM xPos This is the position where the sprite will be rendered in the
 * Target Surface. This is compatible with basic graphic programming.
 *
 * @PARAM FX This specifed any special behavior when rendering the sprite. Please 
 * refer to V2SPRITEFX for more information about this parameters.
 *
 * @REMARK Here is how to decompress the NCK sprite format. The NCK format is a
 * palettize format, and it is a chunck base graphic model. Each chucnk have the
 * current format and there must be at least 1 chunck per sprite.
 * The philosophy behind the NCK format is to jump over the transparency specified
 * by the color key.
 * <b>WORD</b>     Position offset, absolute value from the starting horizontal
 * position of the sprite.
 * <b>BYTE</b>     Number of DWORD blocks in the data chunck.
 * <b>BYTE</b>     Remaining byte in non-aligned data chunck.
 * <b>BYTE</b>     This is flag specifing the type of the chunck. There is currently
 * two types of chuncks. The first one is a NCK_DISPLAY chucnk and the other is a
 * NCK_SHADOW chunck. Each chunck as a dirrent behavior and format.
 *
 * Here is the following chunck format for a NCK_DISPLAY chunck. Its function is to
 * display graphic to the screen. Information in the buffer will be parse thru a palette
 * before being rendered to the Surface.
 * <b>DATA</b>     Buffer of size ( 'Number of DWORD' * sizeof( DWORD ) +
 * 'Remaining bytes' ) BYTEs.
 * <b>BYTE</b>     This is the type of the next block. It may be a NCK_BREAK,
 * meaning that this was the last chunck in the sprite. A NCK_SAME_LINE, meaning
 * that the next chunck information is on the same vertical line or a NCK_NEXT_LINE
 * meaning that the information is on the next vertical line.
 *
 * Here is the following chunck format for a NCK_SHADOW chunck. Its function is to
 * display a dithered black line of a variable lenght. This is used to make Shadow in
 * T4C. The size is specifed by the following formula:
 * 'Number of DWORD' * sizeof( DWORD ) +  Remaining bytes'
 * <b>BYTE</b>     This is the type of the next block. It may be a NCK_BREAK,
 * meaning that this was the last chunck in the sprite. A NCK_SAME_LINE, meaning
 * that the next chunck information is on the same vertical line or a NCK_NEXT_LINE
 * meaning that the information is on the next vertical line.
 *
 * @WARNING NCK_SHADOW is not supported in the Reverse version of NCK. This wasn't
 * implemented because reverse shadow in not needed in T4C. To keep a consistency
 * shadow all are in the same direction and reversing a shadow would break that
 * consistency.
 *
 * @RETURN Return value, a non zero value means an error have occured. Return 
 * value will be V2SPRITE_OK if everything worked fine, or ERR_FORMATNOTHANDLE if the 
 * V2SPRITE format is not handled.
 //*********************************************************************************/
HRESULT CV2Sprite::DrawNCKMirrorSprite(LONG xPos,LONG yPos,LPV2SPRITEFX FX)
{
   /*if( !g_App.isFocus() && !Custom.gWindowed)//BLBL Freeze test
   {
      return V2SPRITE_OK;
   }*/
   
   DDSURFACEDESC Surface;    // Dest Surface Desc.
   SizeMemory(&Surface);
   HRESULT hr; 
   BOOL    boExit = FALSE;
   long          wClipXl = 65536;     // X left   Clipping Value.
   long          wClipXr = 0;         // X right  Clipping Value.
   long          wClipYt = 0;         // Y top    Clipping Value.
   long          wClipYb = 65536;     // Y bottom Clipping Value.
   LPDIRECTDRAWSURFACE lpDirectDrawSurface = lpDXDirectDraw->GetBackBufferDirect();
   BOOL boOutline   = FALSE;
   BOOL boInvisible = FALSE;
   
   RECT sourceRect, destRect;

   // Applying Corection
   if (FX) 
   {
      if (!(FX->dwFX & FX_NOCORRECTION)) 
      {
         xPos += m_lpV2SpriteInfo->Header.shOffX2;
         yPos += m_lpV2SpriteInfo->Header.shOffY2;
      }
   } 
   else 
   {
      xPos += m_lpV2SpriteInfo->Header.shOffX2;
      yPos += m_lpV2SpriteInfo->Header.shOffY2;
   }

   sourceRect.left = 0;
   sourceRect.right = m_lpV2SpriteInfo->Header.dwWidth;
   sourceRect.top = 0;
   sourceRect.bottom = m_lpV2SpriteInfo->Header.dwHeight;

   destRect.left = xPos;
   destRect.right = xPos + sourceRect.right;
   destRect.top = yPos;
   destRect.bottom = yPos + sourceRect.bottom;

   // Do FX
   if (FX) 
   {
      if (FX->lpClipRect) 
      {
   		long lLeftBorder   = FX->lpClipRect->left;
		   long lRightBorder  = FX->lpClipRect->right;
		   long lTopBorder    = FX->lpClipRect->top;
		   long lBottomBorder = FX->lpClipRect->bottom;

         WORD wWidth  = m_lpV2SpriteInfo->Header.dwWidth;
         WORD wHeight = m_lpV2SpriteInfo->Header.dwHeight;

         // Out of the Clipping Border.
         xPos += wWidth;
		   if (xPos - wWidth > lRightBorder)         return V2SPRITE_OK;
		   if (yPos > lBottomBorder)        return V2SPRITE_OK;
		   if (xPos <= lLeftBorder) return V2SPRITE_OK;
         if (yPos + wHeight < lTopBorder) return V2SPRITE_OK;

         // Clipping ?
         if (xPos > lRightBorder) {
         // Do we clip on the right ?
            wClipXr = xPos - lRightBorder;
            xPos = lRightBorder;
   			sourceRect.right  = (m_lpV2SpriteInfo->Header.dwWidth) - (destRect.right - lRightBorder);
	   		destRect  .right  = lRightBorder;
         } 
         
         if (xPos - wWidth < lLeftBorder) {
         // Do we clip on the left ?
   			wClipXl = xPos - lLeftBorder+wClipXr;
	   		sourceRect.left  = (lLeftBorder - destRect.left);
   			destRect  .left  = lLeftBorder;
		   }

         if (yPos < lTopBorder) {
         // Do we clip on the top ?
			   wClipYt = lTopBorder;
	   		sourceRect.top  = (lTopBorder - destRect.top);
   			destRect  .top  = lTopBorder;
		   }

         if (yPos + wHeight > lBottomBorder) 
         {
         // Do we clip on the bottom ?
            wClipYb = lBottomBorder;
   			sourceRect.bottom = (m_lpV2SpriteInfo->Header.dwHeight) - (destRect.bottom - lBottomBorder);
	   		destRect  .bottom = lBottomBorder;
         }
      } 
      else 
      {
         xPos = xPos + m_lpV2SpriteInfo->Header.dwWidth;
      }

      if (FX->lpDirectDrawSurface) 
      {
         lpDirectDrawSurface = FX->lpDirectDrawSurface;
      } 
      else 
      {
         lpDirectDrawSurface = lpDXDirectDraw->GetBackBufferDirect();
      }

      if (FX->dwFX & FX_OUTLINE)
         boOutline = TRUE;

      if (FX->lpWrapRect) {
	   // Wrap Sprite around the Screen ?
         long LeftBorder   = FX->lpWrapRect->left;
	   	long RightBorder  = FX->lpWrapRect->right;
   		long TopBorder    = FX->lpWrapRect->top;
		   long BottomBorder = FX->lpWrapRect->bottom;

         WORD wWidth  = m_lpV2SpriteInfo->Header.dwWidth;
         WORD wHeight = m_lpV2SpriteInfo->Header.dwHeight;

         RECT Dest[4];
		   RECT Source[4];

      memset(Dest,   0, sizeof(RECT)*4);
		memset(Source, 0, sizeof(RECT)*4);

   	BOOL Adjust;
      // Out of Bound ?
      do {
         Adjust = FALSE;
         if (destRect.left >= RightBorder) {
   			destRect.left  -= RightBorder;
			   destRect.right -= RightBorder;
            Adjust = TRUE;
		   }
		   if (destRect.left < LeftBorder) {
   			destRect.left  += RightBorder;
			   destRect.right += RightBorder;
            Adjust = TRUE;
		   }

   		if (destRect.top >= BottomBorder) {
			   destRect.top    -= BottomBorder;
   			destRect.bottom -= BottomBorder;
            Adjust = TRUE;
		   }
   		if (destRect.top < TopBorder) {
			   destRect.top    += BottomBorder;
   			destRect.bottom += BottomBorder;
            Adjust = TRUE;
		   }
      } while (Adjust);

      if (destRect.left < LeftBorder) {
			Dest[0].left  = Dest[2].left  = RightBorder + destRect.left;
			Dest[0].right = Dest[2].right = RightBorder;
			Dest[1].left  = Dest[3].left  = LeftBorder;
			Dest[1].right = Dest[3].right = destRect.right;

			Source[0].left  = Source[2].left  = sourceRect.left;
			Source[0].right = Source[2].right = sourceRect.left - destRect.left;
			Source[1].left  = Source[3].left  = Source[0].right;
			Source[1].right = Source[3].right = sourceRect.right;
		} else if (destRect.right > RightBorder) {
			Dest[0].left  = Dest[2].left  = destRect.left;
			Dest[0].right = Dest[2].right = RightBorder;
			Dest[1].left  = Dest[3].left  = LeftBorder;
			Dest[1].right = Dest[3].right = destRect.right - RightBorder;

			Source[0].left  = Source[2].left  = sourceRect.left;
			Source[0].right = Source[2].right = sourceRect.left+RightBorder-destRect.left;
			Source[1].left  = Source[3].left  = Source[0].right;
			Source[1].right = Source[3].right = sourceRect.right;
		} else {
			Dest[0].left  = destRect.left;
			Dest[0].right = destRect.right;
			Dest[2].left  = destRect.left;
			Dest[2].right = destRect.right;

			Source[0].left  = sourceRect.left;
			Source[0].right = sourceRect.right;
			Source[2].left  = sourceRect.left;
			Source[2].right = sourceRect.right;
		}

		if (destRect.top < TopBorder) {
			Dest[0].top    = Dest[1].top    = BottomBorder + destRect.top;
			Dest[0].bottom = Dest[1].bottom = BottomBorder;
			Dest[2].top    = Dest[3].top    = TopBorder;
			Dest[2].bottom = Dest[3].bottom = destRect.bottom;

			Source[0].top    = Source[1].top    = sourceRect.top;
			Source[0].bottom = Source[1].bottom = sourceRect.top - destRect.top;
			Source[2].top    = Source[3].top    = Source[0].bottom;
			Source[2].bottom = Source[3].bottom = sourceRect.bottom;
		} else if (destRect.bottom > BottomBorder) {
			Dest[0].top    = Dest[1].top    = destRect.top;
			Dest[0].bottom = Dest[1].bottom = BottomBorder;
			Dest[2].top    = Dest[3].top    = TopBorder;
			Dest[2].bottom = Dest[3].bottom = destRect.bottom - BottomBorder;

			Source[0].top    = Source[1].top    = sourceRect.top;
			Source[0].bottom = Source[1].bottom = sourceRect.top+BottomBorder - destRect.top;
			Source[2].top    = Source[3].top    = Source[0].bottom;
			Source[2].bottom = Source[3].bottom = sourceRect.bottom;
		} else {
			Dest[0].top    = destRect.top;
			Dest[0].bottom = destRect.bottom;
			Dest[1].top    = destRect.top;
			Dest[1].bottom = destRect.bottom;

			Source[0].top    = sourceRect.top;
			Source[0].bottom = sourceRect.bottom;
			Source[1].top    = sourceRect.top;
			Source[1].bottom = sourceRect.bottom;
		}

         V2SPRITEFX spritefx;
         ZeroMemory(&spritefx, sizeof(V2SPRITEFX));
         spritefx.lpDirectDrawSurface = FX->lpDirectDrawSurface;
         spritefx.dwFX = FX_NOCORRECTION;

         spritefx.lpClipRect = &Dest[0];
         DrawNCKMirrorSprite(Dest[0].left - Source[0].left, Dest[0].top - Source[0].top, &spritefx);
         spritefx.lpClipRect = &Dest[1];
         DrawNCKMirrorSprite(Dest[1].left - Source[1].left, Dest[1].top - Source[1].top, &spritefx);
         spritefx.lpClipRect = &Dest[2];
         DrawNCKMirrorSprite(Dest[2].left - Source[2].left, Dest[2].top - Source[2].top, &spritefx);
         spritefx.lpClipRect = &Dest[3];
         DrawNCKMirrorSprite(Dest[3].left - Source[3].left, Dest[3].top - Source[3].top, &spritefx);
		
         return V2SPRITE_OK;
      }

      if (FX->dwFX & FX_NODRAW) {
         boInvisible = TRUE;
      }
   }
	
   // Get the Pointer of the Back Buffer. (Lock the Back Buffer)
   const int MaxTries = 5;
   int tries = 0;
   do {
      hr = lpDirectDrawSurface->Lock(NULL, &Surface, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
      
      if (hr == DDERR_SURFACELOST) {
         Sleep( 5 );
         /*if( !g_App.isFocus() ){//BLBL Freeze test
             return V2SPRITE_OK;
         }*/
         lpDirectDrawSurface->Restore();
      } else {
         break;
      }
      if( tries++ > 5 ){
        return V2SPRITE_OK;
      }
   } while (1);

	LPBYTE lpbSource = m_lpV2SpriteInfo->Data.lpbChunck;				   // The Source.
	long   lPitch = Surface.lPitch;		// Pitch of the Back Buffer.
	LPBYTE lpbDestStart = (LPBYTE)Surface.lpSurface +  
                         ((yPos * lPitch) + (xPos<<1));  // Destination Start.
	LPBYTE lpbDestCurr = 0;
  
	long   wByte;						// Number of Byte to be Blit.
   long   wByteTemp;
   long   wByteTemp2;

   long   wAdv;     // Number of Pixel to advance before drawing sprite.
   long   wAdvTemp;

   long   wClipXrTemp;
   WORD   wColor = 0;
   WORD   wRed;
   WORD   wGreen;
   WORD   wBlue;
   WORD   wPal;
   BYTE   bRDown = lpDXDirectDraw->bRDown;
   BYTE   bGDown = lpDXDirectDraw->bGDown;
   BYTE   bBDown = lpDXDirectDraw->bBDown;
   BYTE   bRUp = lpDXDirectDraw->bRUp;
   BYTE   bGUp = lpDXDirectDraw->bGUp;
   BYTE   bBUp = lpDXDirectDraw->bBUp;
   int    i;
   
   // This will decompress the NCK Folowing the Definition above.
	InnerLoop:
      if (yPos >= wClipYb) {
      // Do we end because of Clipping ?
         goto EndLoop;
      }

      wClipXrTemp = wClipXr;

      // We calculate the offset of the Destination (Dest Start + Pos Offset)	
      wAdvTemp = wAdv = *(LPWORD)lpbSource;
      lpbSource++;
      lpbSource++;
      
      if (wClipXrTemp) {
      // Do we cut in the "transparent" pixel ?
         if (wAdv > wClipXrTemp) {
            wAdv -= wClipXrTemp;
            wClipXrTemp = 0;
         } else {
            wClipXrTemp -= wAdv;
            wAdv = 0;
         }
      }

	   // We then Compute the number of byte to be blitted
	   // Number of DWORD Block * 4 + Number of remaining BYTE Block
      // This has be devided in DWORD and BYTE block for futur optimization.
      wByte  = ((*(LPBYTE)(lpbSource)) << 2);
      lpbSource++;
      wByte  += *(LPBYTE)(lpbSource);
      lpbSource++;
      wByteTemp = wByte;

      if (wAdv < wClipXl - wClipXr) {
      
         lpbDestCurr = lpbDestStart - (wAdv<<1);
		
         wByte = signed(((wClipXl - (wClipXr - wClipXrTemp)) - wAdv) < wByte ? (wClipXl - (wClipXr - wClipXrTemp)) - wAdv : wByte);
		
   		// Copy the memory.
         if (wClipYt <= yPos) 
         {
            if (*(LPBYTE)(lpbSource) == V2_NCK_SHADOW) 
            {
               lpbSource++;
               wByteTemp = 0;
            } else 
            {
               lpbSource++;
               if (wByte > wClipXrTemp) 
               {
                  wByteTemp2 = wByte - wClipXrTemp;

                  lpbDestCurr--;
                  lpbDestCurr--;
                  if (!boInvisible) 
                  {
                     for (i = 0; i < wByteTemp2; i++) 
                     {
                        wPal = (*(BYTE *)(lpbSource+wClipXrTemp+i))*3;
                        
                        wRed   = (m_pRefDataPal[wPal])>>bRDown;
                        wGreen = (m_pRefDataPal[wPal+1])>>bGDown;
                        wBlue  = (m_pRefDataPal[wPal+2])>>bBDown;
                        
                        wColor = (wRed<<bRUp)+(wGreen<<bGUp)+(wBlue<<bBUp);
                        
                        ((LPWORD)(lpbDestCurr))[0] = wColor;
                        lpbDestCurr--;
                        lpbDestCurr--;
                     }
                  }
                  if (boOutline) {  
                     lpbDestCurr = lpbDestStart - (wAdv<<1);
                     lpbDestCurr--;
                     lpbDestCurr--;
                     if (wByte == wByteTemp) {
                        ((LPWORD)(lpbDestCurr))[0] = (WORD)FX->dwOutlineColor;
                     }
                     if (!wClipXrTemp) {
                        *(LPWORD)(lpbDestCurr-(wByte<<1)) = (WORD)FX->dwOutlineColor;
                     }
                  }
               }
            }
         } else {
   		   if (*(LPBYTE)(lpbSource) == V2_NCK_SHADOW) {
               wByteTemp = 0;   
            }
            lpbSource++;
         }
   		
         // What's the Next Step (Default is on the same line)
      } else {
 		   if (*(LPBYTE)(lpbSource) == V2_NCK_SHADOW) {
           wByteTemp = 0;   
         }
         lpbSource++;
      }
         
      lpbSource += wByteTemp;
	
      switch (*(LPBYTE)(lpbSource)) {
			// Next Line.
			case V2_NCK_NEXT_LINE: lpbDestStart += lPitch; yPos++; break;
			// End of Decompression.
			case V2_NCK_BREAK: 
         goto EndLoop; break;
		}
      lpbSource++;

      // Get the Next Chunck.
		goto InnerLoop;
	EndLoop:

   // Unlock the Back Buffer.
	lpDirectDrawSurface->Unlock(Surface.lpSurface);

	return V2SPRITE_OK;
}


HRESULT CV2Sprite::DrawNMSemiTrans(LONG xPos, LONG yPos, LPV2SPRITEFX FX)
{
   RECT sourceRect;
   sourceRect.left = 0;
   sourceRect.right = m_lpV2SpriteInfo->Header.dwWidth;
   sourceRect.top = 0;
   sourceRect.bottom = m_lpV2SpriteInfo->Header.dwHeight;


   LPDIRECTDRAWSURFACE ToBlit = FX->lpDirectDrawSurface;
   if (!ToBlit)
      ToBlit = lpDXDirectDraw->GetBackBufferDirect();

   // Normal 
   BYTE   bRDown = lpDXDirectDraw->bRDown;
   BYTE   bGDown = lpDXDirectDraw->bGDown;
   BYTE   bBDown = lpDXDirectDraw->bBDown;
   BYTE   bRUp = lpDXDirectDraw->bRUp;
   BYTE   bGUp = lpDXDirectDraw->bGUp;
   BYTE   bBUp = lpDXDirectDraw->bBUp;

   WORD wPal = m_lpV2SpriteInfo->Header.ushTransColor*3;
   WORD wRed   = (m_pRefDataPal[wPal  ])>>bRDown;
   WORD wGreen = (m_pRefDataPal[wPal+1])>>bGDown;
   WORD wBlue  = (m_pRefDataPal[wPal+2])>>bBDown;
   
   DWORD dwColor = (wRed<<bRUp)+(wGreen<<bGUp)+(wBlue<<bBUp);

   if (FX) 
   {
      if (!(FX->dwFX & FX_NOCORRECTION)) 
      {
         xPos += m_lpV2SpriteInfo->Header.shOffX1;
         yPos += m_lpV2SpriteInfo->Header.shOffY1;
      }
   } 
   else 
   {
      xPos += m_lpV2SpriteInfo->Header.shOffX1;
      yPos += m_lpV2SpriteInfo->Header.shOffY1;
   }

   
   return V2SPRITE_OK;
}



//////////////////////////////////////////////////////////////////////////////////
// TransAlphaImproved - 
//////////////////////////////////////////////////////////////////////////////////
HRESULT CV2Sprite::TransAlphaImproved(LPDIRECTDRAWSURFACE src, LPDIRECTDRAWSURFACE dest,LONG lDestX, LONG lDestY,LONG lDestW,LONG lDestH, RECT srcRect, WORD ALPHA, DWORD ColorKey, WORD BPP)
{
   int register i,j;
   int height,width;
   BYTE* lpSprite;
   BYTE* lpDest;
   WORD dPitch, SpritePitch;
   DWORD sColorKey;
   DWORD sTemp,dTemp;
   DWORD sr,sg,sb,dr,dg,db;
   WORD sbuf,dbuf;
   DWORD Result;
   BOOL oddWidth = FALSE;
   DDSURFACEDESC srcDDSD, destDDSD;
   DWORD REDC,GREENC, BLUEC;
   DWORD PLUS64;
   DWORD ALPHABY4;
   DWORD doubleColorKey;

   // Check the ALPHA value
   if (ALPHA < 0)
      ALPHA = 0;
   else if (ALPHA > 256)
      ALPHA = 256;
   
   // Set height and width of SPRITE
   height = srcRect.bottom - srcRect.top;
   width = srcRect.right - srcRect.left; 
   
   // Lock down both surfaces for read and write
   ZeroMemory(&srcDDSD, sizeof(srcDDSD));
   srcDDSD.dwSize = sizeof(srcDDSD);
	src->Lock(NULL, &srcDDSD, DDLOCK_WAIT, NULL);
    
	ZeroMemory(&destDDSD, sizeof(destDDSD));
   destDDSD.dwSize = sizeof(destDDSD);
   dest->Lock(NULL, &destDDSD, DDLOCK_WAIT, NULL);
   
   // Get the color key for sprite surface
   sColorKey = ColorKey;
   
   // Set the pitch for both surfaces
   SpritePitch = (WORD)srcDDSD.lPitch;
   dPitch      = (WORD)destDDSD.lPitch;
   
   // Initialize the pointers to the upper left hand corner of surface
   lpSprite = (BYTE*)srcDDSD.lpSurface;
   lpDest   = (BYTE*)destDDSD.lpSurface;

   if(!lpSprite || !lpDest)
      return DD_OK;
   
   // Do some work outside of the loop
   PLUS64         = 64 | (64 << 16);
   ALPHABY4      = (ALPHA / 4) | ((ALPHA / 4) << 16);
   doubleColorKey = ColorKey | (ColorKey << 16);
   
   switch(BPP)
   {
      case 16:
      
         // Initialize the pointers to the first pixel in the rectangle
         lpSprite += (srcRect.top * SpritePitch) + (srcRect.left * 2);
         lpDest   += (lDestY * dPitch) + (lDestX * 2);
      
         // Set the horizontal padding
         sbuf = (WORD)(SpritePitch - (2 * width));
         dbuf = (WORD)(dPitch - (2 * width));
      
         // Is the Sprite width odd or even?
         if (width % 2 == 1)
         {
            oddWidth = TRUE;
            width = (width - 1) / 2; //div by 2, processing 2 pixels at a time.
         }
         else
            width = width / 2;  //div by 2, processing 2 pixels at a time.
      
      
         i = height;
         do
         {
            if (oddWidth)
            {
               sTemp = *((WORD*)lpSprite);
            
               if (sTemp != ColorKey)
               {
                  dTemp = *((WORD*)lpDest);
                  sb = sTemp & 0x1f;
                  db = dTemp & 0x1f;
                  sg = (sTemp >> 5) & 0x3f;
                  dg = (dTemp >> 5) & 0x3f;
                  sr = (sTemp >> 11) & 0x1f;
                  dr = (dTemp >> 11) & 0x1f;
               
                  *((WORD*)lpDest) = (WORD)((ALPHA * (sb - db) >> 8) + db |
                     ((ALPHA * (sg - dg) >> 8) + dg) << 5 |
                     ((ALPHA * (sr - dr) >> 8) + dr) << 11);
               }
            
               lpDest   += 2;
               lpSprite += 2;
            }
            j = width;
            do
            {
               sTemp = *((DWORD*)lpSprite);
            
               if ( sTemp != doubleColorKey )
               {
                  dTemp = *((DWORD*)lpDest);
               
                  sb = sTemp & 0x001F001F;
                  db = dTemp & 0x001F001F;
                  sg = (sTemp >> 5)  & 0x003F003F;
                  dg = (dTemp >> 5)  & 0x003F003F;
                  sr = (sTemp >> 11) & 0x001F001F;
                  dr = (dTemp >> 11) & 0x001F001F;
               
                  BLUEC  = ((((ALPHA * ((sb + PLUS64) - db)) >> 8) + db) - ALPHABY4) & 0x001F001F;
                  GREENC = (((((ALPHA * ((sg + PLUS64) - dg)) >> 8) + dg) - ALPHABY4) & 0x003F003F) << 5;
                  REDC   = (((((ALPHA * ((sr + PLUS64) - dr)) >> 8) + dr) - ALPHABY4) & 0x001F001F) << 11;
               
                  Result = BLUEC | GREENC | REDC;
               
                  if ( (sTemp >> 16) == ColorKey )
                     Result = (Result & 0xFFFF) | (dTemp & 0xFFFF0000);
                  else if ( (sTemp & 0xFFFF) == ColorKey )
                     Result = (Result & 0xFFFF0000) | (dTemp & 0xFFFF);
               
                  *((DWORD*)lpDest) = Result;
               }
               lpDest    += 4;
               lpSprite  += 4;
            
            }while (--j > 0);
         
            lpDest   += dbuf;
            lpSprite += sbuf;
         
         }while (--i > 0);
      break;

	} // End RGB Format switch statement
	src->Unlock(NULL);
	dest->Unlock(NULL);

	return DD_OK;
}

HRESULT CV2Sprite::TransAlphaGlow(LPDIRECTDRAWSURFACE src, LPDIRECTDRAWSURFACE dest,LONG lDestX, LONG lDestY,LONG lDestW,LONG lDestH, RECT srcRect, DWORD ColorKey, WORD BPP,int dwDiv,DWORD ColorKey2)
{
   int register i,j;
   int height,width;
   BYTE* lpSprite;
   WORD* lpSpriteBase;
   BYTE* lpDest;
   WORD dPitch, SpritePitch;
   DWORD sColorKey;
   DWORD sTemp,dTemp;
   DWORD sr,sg,sb,dr,dg,db;
   WORD sbuf,dbuf;
   WORD   wRed,wGreen,wBlue;
   DDSURFACEDESC srcDDSD, destDDSD;
   BYTE   bRDown = lpDXDirectDraw->bRDown;
   BYTE   bGDown = lpDXDirectDraw->bGDown;
   BYTE   bBDown = lpDXDirectDraw->bBDown;
   BYTE   bRUp = lpDXDirectDraw->bRUp;
   BYTE   bGUp = lpDXDirectDraw->bGUp;
   BYTE   bBUp = lpDXDirectDraw->bBUp;

   WORD   bR,bG,bB;

    
   // Set height and width of SPRITE
   height = srcRect.bottom - srcRect.top;
   width = srcRect.right - srcRect.left; 
   
   // Lock down both surfaces for read and write
   ZeroMemory(&srcDDSD, sizeof(srcDDSD));
   srcDDSD.dwSize = sizeof(srcDDSD);
	src->Lock(NULL, &srcDDSD, DDLOCK_WAIT, NULL);
    
	ZeroMemory(&destDDSD, sizeof(destDDSD));
   destDDSD.dwSize = sizeof(destDDSD);
   dest->Lock(NULL, &destDDSD, DDLOCK_WAIT, NULL);
   
   // Get the color key for sprite surface
   sColorKey = ColorKey;
   
   // Set the pitch for both surfaces
   SpritePitch = (WORD)srcDDSD.lPitch;
   dPitch      = (WORD)destDDSD.lPitch;
   
   // Initialize the pointers to the upper left hand corner of surface
   lpSprite = (BYTE*)srcDDSD.lpSurface;
   lpDest   = (BYTE*)destDDSD.lpSurface;

   lpSpriteBase = (WORD*)srcDDSD.lpSurface;
   
      
   switch(BPP)
   {
      case 16:
      
         // Initialize the pointers to the first pixel in the rectangle
         lpSprite += (srcRect.top * SpritePitch) + (srcRect.left * 2);
         lpDest   += (lDestY * dPitch) + (lDestX * 2);
      
         // Set the horizontal padding
         sbuf = (WORD)(SpritePitch - (2 * width));
         dbuf = (WORD)(dPitch - (2 * width));

         DWORD RBM, GBM, BBM;
         RBM = lpDXDirectDraw->ddsd.ddpfPixelFormat.dwRBitMask;
         GBM = lpDXDirectDraw->ddsd.ddpfPixelFormat.dwGBitMask;
         BBM = lpDXDirectDraw->ddsd.ddpfPixelFormat.dwBBitMask;
      
         if(dwDiv >0)
         {
            int xx = srcRect.left;
            int yy = srcRect.top;
            int dwStat = 0;
            bool bBlend = false;

            i = height;
            do
            {
               j = width;
               xx = srcRect.left;
               do
               {
                  sTemp = *((WORD*)lpSprite);
                  if (sTemp != ColorKey)
                  {
                     if(xx >1 && (DWORD)xx<srcDDSD.dwWidth-2 && yy>1 && (DWORD)yy<srcDDSD.dwHeight-2)
                     {
                        bBlend = isNeedGlow(lpSpriteBase,srcDDSD.dwWidth,srcDDSD.dwHeight,xx,yy,(USHORT)ColorKey,dwDiv);
                     }
                     else
                        bBlend = true; // on ets sur le bord de l<images...

                     dTemp = *((WORD*)lpDest);

                     sr =((sTemp & RBM) * 255) / RBM;
                     sg =((sTemp & GBM) * 255) / GBM;
                     sb =((sTemp & BBM) * 255) / BBM;

                     dr =((dTemp & RBM) * 255) / RBM;
                     dg =((dTemp & GBM) * 255) / GBM;
                     db =((dTemp & BBM) * 255) / BBM;

                     
                     if( bBlend)
                     {
                        if(ColorKey2)
                        {
                           bR = (WORD)sr;
                           bG = (WORD)sg;
                           bB = (WORD)sb;
                        }
                        else
                        {
                           bR = (WORD)(dr+(sr/3));
                           bG = (WORD)(dg+(sg/3));
                           bB = (WORD)(db+(sb/3));
                           if(bR >255)
                              bR = 255;
                           if(bG >255)
                              bG = 255;
                           if(bB >255)
                              bB = 255;
                        }

                     }
                     else
                     {
                         bR = (WORD)sr;
                         bG = (WORD)sg;
                         bB = (WORD)sb;
                     }

                     wRed   = bR>>bRDown;
                     wGreen = bG>>bGDown;
                     wBlue  = bB>>bBDown;
                     *((WORD*)lpDest) = (wRed<<bRUp)+(wGreen<<bGUp)+(wBlue<<bBUp);

                  }
                  lpDest    += 2;
                  lpSprite  += 2;
                  xx++;
               }while (--j > 0);
               lpDest   += dbuf;
               lpSprite += sbuf;
               yy++;
            }while (--i > 0);
         }
         else
         {
            i = height;
            do
            {
               j = width;
               do
               {
                  sTemp = *((WORD*)lpSprite);
                  if (sTemp != ColorKey)
                  {
                     dTemp = *((WORD*)lpDest);

                     sr =((sTemp & RBM) * 255) / RBM;
                     sg =((sTemp & GBM) * 255) / GBM;
                     sb =((sTemp & BBM) * 255) / BBM;

                     dr =((dTemp & RBM) * 255) / RBM;
                     dg =((dTemp & GBM) * 255) / GBM;
                     db =((dTemp & BBM) * 255) / BBM;

                     {           
                        if(sr > dr)
                           bR = (WORD)sr;
                        else
                           bR = (WORD)dr;
                        
                        if(sg > dg)
                           bG = (WORD)sg;
                        else
                           bG = (WORD)dg;
                        
                        if(sb > db)
                           bB = (WORD)sb;
                        else
                           bB = (WORD)db;
                     }

                     wRed   = bR>>bRDown;
                     wGreen = bG>>bGDown;
                     wBlue  = bB>>bBDown;
                     *((WORD*)lpDest) = (wRed<<bRUp)+(wGreen<<bGUp)+(wBlue<<bBUp);
                  }
                  lpDest    += 2;
                  lpSprite  += 2;
               }while (--j > 0);
               
               lpDest   += dbuf;
               lpSprite += sbuf;
            }while (--i > 0);
         }
      break;

	} // End RGB Format switch statement
	src->Unlock(NULL);
	dest->Unlock(NULL);

	return DD_OK;
}


bool CV2Sprite::isNeedGlow(WORD *pchSource,int dwW,int dwH,int dwX,int dwY,WORD wColorKey,int Range)
{
   if(dwX < 2 || dwX>=dwW-2 || dwY <2 || dwY >=dwH-2)
      return true;

   WORD *pWTmp;
   for(int y=0;y<Range*2+1;y++)
   {
      pWTmp = pchSource + ((((dwY-Range)+y)*dwW)+(dwX-Range));
      for(int x=0;x<Range*2+1;x++)
      {
         if(y != Range && x != Range)
         {
            if(pWTmp[x] == wColorKey)
               return true;
         }
      }
   }
   return false;
}





//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
AnimV2Sprite::AnimV2Sprite( void ) 
{
   curFrame = 0;
   running  = true;
}

// Destructor ////////////////////////////////////////////////////////////////////////////
AnimV2Sprite::~AnimV2Sprite( void )
{
   // Delete each frame.
   for(UINT i = 0; i < frames.size(); i++ )
   {
      delete frames[ i ];
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Animates the sprite.
//  Draw the Sprite to Screen. 
// 
//////////////////////////////////////////////////////////////////////////////////////////
// Return: HRESULT, 
//////////////////////////////////////////////////////////////////////////////////////////
HRESULT AnimV2Sprite::DrawSpriteN(LONG xPos,LONG yPos,LPV2SPRITEFX FX)
{
    if( curFrame > frames.size() )
        curFrame = 0;

    // Frame 0 is this sprite
    if( curFrame == 0 )
    {
        curFrame++;
        return CV2Sprite::DrawSpriteN( xPos, yPos, FX );
    }

    HRESULT r = frames[ curFrame - 1 ]->DrawSpriteN( xPos, yPos, FX );
    
    if( running )
        curFrame++;

    return r;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Animates the sprite.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void AnimV2Sprite::DrawSprite(int xPos,int yPos,RECT *Clip,RECT *OutBound,RECT *Wrap,LPDIRECTDRAWSURFACE ToBlit,
                              char FX,signed char Format,BOOL ParamDither)
{
    if( curFrame > frames.size() )
        curFrame = 0;

    // Frame 0 is this sprite.
    if( curFrame == 0 )
    {
        curFrame++;
        CV2Sprite::DrawSprite( xPos, yPos, Clip, OutBound, Wrap, ToBlit, FX, Format, ParamDither );
    }

    frames[ curFrame - 1 ]->DrawSprite( xPos, yPos, Clip, OutBound, Wrap, ToBlit, FX, Format, ParamDither );
    
    if( running )
        curFrame++;
}
    
//////////////////////////////////////////////////////////////////////////////////////////
//  Adds a frame to the animation.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void AnimV2Sprite::AddFrame(const char *frameId)
{
    // Create the frame
    CV2Sprite *sprite = new CV2Sprite;
    sprite->CreateSprite( frameId ); 

    // Add it to the frames.
    frames.push_back( sprite );

}

//////////////////////////////////////////////////////////////////////////////////////////
// Runs the animation
// 
//////////////////////////////////////////////////////////////////////////////////////////
void AnimV2Sprite::Play( void )
{
    running = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Stops the animation and reset the current frame.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void AnimV2Sprite::Stop( void )
{
    running = false;
    curFrame = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Stops the animation at the current frame.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void AnimV2Sprite::Pause( void )
{
    running = false;
}
