// *******************************************************************************************
#ifndef __V2_Sprite_H__
#define __V2_Sprite_H__

// Include
#include "IDirectXDraw.h"          
#include "c2.h"
#include <vector>
#include <string>
#include "sprite.h"

// Constant                                        
#define COMP_DD 	   1
#define COMP_NCK	   2
#define COMP_NULL     3
#define COMP_ZIP      9

// Error Constant
#define V2SPRITE_OK	             0
#define ERR_FORMATNOTHANDLE       1

// FX Constant
#define FX_LEFTRIGHTMIRROR     (1 << 0)
#define FX_NOCORRECTION        (1 << 1)
#define FX_OUTLINE			    (1 << 3)
#define FX_DITHER					 (1 << 4)
#define FX_FIT2SCREEN          (1 << 5)
#define FX_NODRAW              (1 << 6)

// FX Internal Constant
#define FX_CLIPPING            (1 << 2)

//CR:NMNMNM

// ******************************************************************************************* 
typedef struct _V2SPRITEDATA 
{
   LPBYTE              lpbChunck;   // Chunck of data to be decompresed.
	LPDIRECTDRAWSURFACE lpDDSurface; // A DirectDraw Surface.
} V2SPRITEDATA, *LPV2SPRITEDATA;

typedef struct _V2SPRITEHEADER 
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
} V2SPRITEHEADER, *LPV2SPRITEHEADER;

typedef struct _V2SPRITEINFO 
{
	V2SPRITEHEADER Header; // V2SPRITE Header
	V2SPRITEDATA   Data;   // V2SPRITE Data
} V2SPRITEINFO, *LPV2SPRITEINFO;



// ******************************************************************************************* 
typedef struct _V2SPRITEFX {
// ******************************************************************************************* 
// The V2Sprite FX Structure.
//
	LPRECT              lpClipRect;
   LPRECT              lpWrapRect;
   LPDIRECTDRAWSURFACE lpDirectDrawSurface;
   DWORD               dwFX;
   DWORD               dwOutlineColor;
   DWORD               dwConstantValue;
} V2SPRITEFX, *LPV2SPRITEFX;



class CV2Sprite 
{
public:
   CV2Sprite();
   virtual ~CV2Sprite();
   virtual void  Release();

   virtual void  LoadSpriteData(BYTE* pData, BYTE* pPal, int dwWidth, int dwHeight);   // Create the Sprite from the Byte* and palette...
   virtual DWORD LoadSprite(LPCTSTR, DWORD = 0, int = 1);	                           // Create the Sprite from the ID
   virtual DWORD CreateSprite(LPCTSTR, DWORD = 0, int = 1);	                           // Create the Sprite from the ID
   virtual DWORD CreateSpriteByPal(LPCTSTR, DWORD = 0, LPBYTE = 0,int iPalIndex = 1);	// Create the Sprite from the ID & the pallet
   
   virtual BYTE  GetTransColor();
   virtual BOOL  GetfTransparent();
   virtual short GetCX();
   virtual short GetCY();
   virtual short GetFCX();
   virtual short GetFCY();
   virtual WORD  GetWidth();
   virtual WORD  GetHeight();

   virtual LPWORD GetSurfaceMemory();
   virtual void   ReleaseSurfaceMemory(LPWORD);
   virtual BYTE   GetType();

   virtual void DrawSprite(int xPos, int yPos, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE ToBlit = lpDXDirectDraw->GetBackBufferDirect(), char FX = 0, signed char Format = 0, BOOL ParamDither = FALSE);  // Draw a Sprite
   virtual HRESULT DrawSpriteN(LONG, LONG, LPV2SPRITEFX = NULL);               // Draw the Sprite to Screen.
   virtual HRESULT DrawSpriteNSemiTrans(LONG, LONG, LPV2SPRITEFX = NULL, BOOL bSemiTrans = FALSE,BYTE chAlphaLevel = 255);


   HRESULT DrawDDSprite(LONG, LONG, LPV2SPRITEFX,BOOL bSemiTrans,BYTE chAlphaLevel);	// Draw RAW Sprite Format.
   HRESULT DrawNCKNormalSprite(LONG, LONG, LPV2SPRITEFX);	// Draw No Color Key Sprite Format.
   HRESULT DrawNCKMirrorSprite(LONG, LONG, LPV2SPRITEFX);	// Draw No Color Key Sprite Format.

   HRESULT DrawNMSemiTrans(LONG, LONG, LPV2SPRITEFX);
   HRESULT TransAlphaImproved(LPDIRECTDRAWSURFACE src, LPDIRECTDRAWSURFACE dest,LONG lDestX, LONG lDestY,LONG lDestW,LONG lDestH, RECT srcRect, WORD ALPHA, DWORD ColorKey, WORD BPP);

   HRESULT TransAlphaGlow    (LPDIRECTDRAWSURFACE src, LPDIRECTDRAWSURFACE dest,LONG lDestX, LONG lDestY,LONG lDestW,LONG lDestH, RECT srcRect, DWORD ColorKey, WORD BPP,int dwDiv,DWORD ColorKey2 = 0);
   bool    isNeedGlow(WORD *pchSource,int dwW,int dwH,int dwX,int dwY,WORD wColorKey,int Range);

   

   
public:
   std::string    m_strName;
   LPV2SPRITEINFO m_lpV2SpriteInfo;
   LPBYTE         m_pRefDataPal;

   Sprite        *m_pTempSprite;
   
};

class AnimV2Sprite : public CV2Sprite{
public:
    AnimV2Sprite();
    ~AnimV2Sprite();

    // Draw the Sprite to Screen.
    virtual HRESULT DrawSpriteN(LONG, LONG, LPV2SPRITEFX = NULL);
    // Draw a Sprite
    virtual void DrawSprite(int xPos, int yPos, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE ToBlit = lpDXDirectDraw->GetBackBufferDirect(), char FX = 0, signed char Format = 0, BOOL ParamDither = FALSE);

    
    void AddFrame( const char *frameId );
    void Play();
    void Stop();
    void Pause();

private:
    DWORD curFrame;
    bool  running;

    std::vector< CV2Sprite * > frames;
};

typedef CV2Sprite* LPV2Sprite;

#endif