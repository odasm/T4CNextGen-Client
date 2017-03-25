// PAS BON
// ************************************************************************
// ***                                                                  ***
//      File Name: Sprite.cpp
//      Project:   TFC Client
//      Creation:  29 June, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         06-29-1997      1.0     TH          Initial development
//
//      Description
//         Provide fonction to load a Sprite from a Image or an ID
// ***                                                                  ***
//	************************************************************************
// *** Copyright (c) 1997-1998 Vircom. All rights reserved.             ***

#ifndef __Vircom_Sprite_083612062797
#define __Vircom_Sprite_083612062797

#include "IDirectXDraw.h"

class Sprite {
	private:
		LPDIRECTDRAWSURFACE  lpSpriteSurface; // SpriteSurface.
		LPPALETTEENTRY       lpSpriteEntry;   // SpriteColorEntry.
		LPDIRECTDRAWPALETTE  lpSpritePalette; // SpritePalette.

		int LeftBorder;
		int RightBorder;
		int TopBorder;
		int BottomBorder;
		RECT destRect;
		RECT sourceRect;
		RECT Dest[4];
		RECT Source[4];

	protected:
	public:
		unsigned	short		   Height;			  // Height.
		unsigned	short		   Width;			  // Width.
		signed   short       xCor;            // X Coordinate Correction
		signed   short       yCor;            // Y Coordinate Correction
		signed   short       FlipxCor;        // X Coordinate Correction
		signed   short       FlipyCor;        // Y Coordinate Correction
		BOOL                 fTransparent;	  // Transparency flag.
		unsigned	short       TransColor;      // Transparency color.
		RECT					   SpriteRect;		  // Sprite Rectangle Spec.
      BYTE *               lpSpritePal;     // Pal of the Sprite.
      BYTE *               lpRefPal;

	public:
		BOOL Dither;
      
      Sprite(void);					           // Class Constructor Default
		~Sprite(void);					           // Class Destructor

		void LoadSprite(char *SpriteID);   // Load a Sprite from is ID
		void LoadPalette(char *PaletteID); // Load a Palette from is ID
		void LoadSprite(LPDIRECTDRAWSURFACE lpImageSurface, int xPos, int yPos, short width, short height,
			             bool ftransparent = FALSE, int transcolor = 0); // Load a Sprite from an Image
		void SaveSprite(char *SpriteID);   // Save a Sprite
		void DrawSprite(int xPos, int yPos, RECT *Clip = NULL, RECT *OutBound = NULL, RECT *Wrap = NULL, LPDIRECTDRAWSURFACE ToBlit = lpDXDirectDraw->GetBackBufferDirect(), char FX = 0, signed char Format = 0, BOOL ParamDither = FALSE);  // Draw a Sprite
		void SetPalette(void); // Set the Palette;

		void Release(void);      // Release the memory // Ligne succeptible de causer de serieux bug
		void Create(int W, int H, BOOL f, int Color, int Xc = 0, int Yc = 0, int XFc = 0, int YFc = 0);

      unsigned short      GetWidth(void);
      unsigned short      GetHeight(void);
      unsigned short       GetTransColor(void);
      LPDIRECTDRAWSURFACE GetSurface(void);
		LPPALETTEENTRY      GetPalette(void);
		RECT *				  GetRect(void);
      LPWORD              GetSurfaceMemory(VOID);
      void                GetSpriteDC(HDC *);
      void                ReleaseSpriteDC(HDC);
      VOID                ReleaseSurfaceMemory(LPWORD);

		bool Mark;
   protected:
      HRESULT TransAlphaImproved(LPDIRECTDRAWSURFACE src, LPDIRECTDRAWSURFACE dest,LONG lDestX, LONG lDestY,LONG lDestW,LONG lDestH, RECT srcRect, WORD ALPHA, DWORD ColorKey, WORD BPP);
};

#endif
