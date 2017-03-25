// ************************************************************************
// ***                                                                  ***
//      File Name: IDirectXSprite.Cpp
//      Project:   The Fourth Coming
//      Creation:  24 April, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         04-24-1997      1.0     TH          Initial development
//
//      Description
//         Provide fonction to load a Sprite into Image.
// ***                                                                  ***
//	************************************************************************
// *** Copyright (c) 1997-1998 Vircom. All rights reserved.             ***
#ifndef __DIRECTXSPRITE_H
#define __DIRECTXSPRITE_H     
/*
#include "IDirectXDraw.h"

#define S_FLIP 1000

class IDirectXSprite {
// DirectSprite Class.
	private:
	public:
		int					   x;               // X position.
		int					   y;					  // Y position.
		int					   Height;			  // Height.
		int					   Width;			  // Width.
		int					   nSprites;		  // Number of Sprites.	
		RECT					   SpriteRect;		  // Sprite Rectangle Spec.
		BOOL                 fTransparent;	  // Transparency flag.
		int                  TransColor;      // Transparency color.
		LPDIRECTDRAWSURFACE *lpSpriteSurface; // Sprite Surface.
		LPDIRECTDRAWSURFACE  lpTransferSurface;
	
	public:
		static int LeftBorder, TopBorder, RightBorder, BottomBorder;

		IDirectXSprite(void);					 // Class Constructor Default
		~IDirectXSprite(void);					 // Class Destructor	

      HRESULT Create(LPDIRECTDRAWSURFACE, int, int, BOOL, 
			            int, int, int, int);  // Class Initialisation Default
		HRESULT Add(LPDIRECTDRAWSURFACE, int, int, int);
		void Release(void);						 // Class Closing
		
		HRESULT Draw(int, int, int, int Angle = 0, LPDIRECTDRAWSURFACE ToBlit = lpDXComplexSurface, BOOL Clip = FALSE, BOOL Wrapper = FALSE, int TextColor = 255);	 // Draw Sprite Function		
};

typedef IDirectXSprite FAR *LPDIRECTXSPRITE;
*/
#endif
