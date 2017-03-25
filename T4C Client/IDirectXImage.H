// ************************************************************************
// ***                                                                  ***
//      File Name: IDirectXImage.h
//      Project:   The Fourth Coming
//      Creation:  31 Mars, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         03-31-1997      1.0     TH          Initial development
//
//      Description
//         Provide fonction to load a image file.
// ***                                                                  ***
//	************************************************************************
// *** Copyright (c) 1997-1998 Vircom. All rights reserved.             ***   

#ifndef __DIRECTXIMAGE_H
#define __DIRECTXIMAGE_H

#include <ddraw.h>

#include "IDirectXPcx.H"

class IDirectXImage 
#ifdef __DIRECTXPCX_H
				  : public IDirectXPcx
#endif	
{
	private:
   public:
		HRESULT SetHeader(void *header);
		
		LPVOID              lpHeader;       // A pointer to the picture header
		LPDIRECTDRAWSURFACE lpImageSurface; // A pointer holding the picture
      LPBYTE              lpPal;          // A pointer holding the palette
																										
	public:
		IDirectXImage(void);          // Consturctor
		~IDirectXImage(void);         // Destructor

		HRESULT Create(int dwWidth,int dwHeight);         // Initialize
		void Release(void);				// Closing		
		HRESULT LoadImage(LPCSTR,int dwWidth,int dwHeight);    // Load the Image

		operator LPDIRECTDRAWSURFACE (void);
		LPDIRECTDRAWSURFACE ImageSurface(void);
		LPBYTE Palette(void);

};

typedef IDirectXImage FAR *LPDIRECTXIMAGE;

#endif