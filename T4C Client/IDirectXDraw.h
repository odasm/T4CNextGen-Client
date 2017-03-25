// ************************************************************************
// ***                                                                  ***
//      File Name: IDirectXDraw.h
//      Project:   The Fourth Coming
//      Creation:  25 Mars, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         03-25-1997      1.0     TH          Initial development
//
//      Description
//         Provide Simplified class for DirectDraw, DirectSurface and 
//         DirectPalette.
// ***                                                                  ***
//	************************************************************************
// *** Copyright (c) 1997-1998 Vircom. All rights reserved.             ***

#ifndef __DIRECTXDRAW
#define __DIRECTXDRAW

#include <ddraw.h>
#include "SystemInfo.h"
#include "Global.h"
                       
extern Global g_Global;   

#define DXDCreate(x, y)  lpDXDirectDraw->CreateDirectX(x, g_Global.GetScreenW(), g_Global.GetScreenH(), 16, y)
#define DXDRelease()     lpDXDirectDraw->Release()

extern bool primaryDrawBuffer;
																																																																																				
#define DXDGetDC lpDXDirectDraw->GetDC
#define DXDReleaseDC lpDXDirectDraw->ReleaseDC
#define DXDGetFDC lpDXDirectDraw->GetFDC
#define DXDReleaseFDC lpDXDirectDraw->ReleaseFDC
#define DXDFlip lpDXDirectDraw->Flip
#define DXDBlt lpDXDirectDraw->Blt
#define DXDBlt2 lpDXDirectDraw->Blt2
#define DXDRestoreView lpDXDirectDraw->RestoreView
#define DXDRestoreSurface lpDXDirectDraw->RestoreSurface
#define DXDRestoreSurfaceF lpDXDirectDraw->RestoreSurfaceF


inline bool IsPrimaryDrawBufferOn(){
    return primaryDrawBuffer;
}
																                  


// DirectDraw, DirectSurface and DirectPallette simplified class.
class IDirectXDraw {
	private:
	public:
	   LPDIRECTDRAWSURFACE lpFrontBuffer;	  // Pointer to the Primary Surface
	   LPDIRECTDRAWSURFACE lpBackBuffer;	  // Pointer to the Back Surface

      int OldScreenX;
      int OldScreenY;
      int OldScreenBPP;
	  int NewScreenX;
	  int NewScreenY;
	  int NewScreenBPP;

		BOOL DXCompleteInit;

		LPDIRECTDRAWCLIPPER lpClipper;

														   
	public:
	   DDSURFACEDESC       ddsd;				  // Surface Description
		LPDIRECTDRAW        lpdd;				  // DirectDraw
	   LPDIRECTDRAWPALETTE lpddpal;		     // Complex Palette
		LPDIRECTDRAWPALETTE lpddfrontpal;     // Primary Palette
		BOOL					fEnabled;		// DirectDraw enabled

      WORD                wRMask;
      WORD                wGMask;
      WORD                wBMask;
      BYTE                bRUp;
      BYTE                bGUp;
      BYTE                bBUp;
      BYTE                bRDown;
      BYTE                bGDown;
      BYTE                bBDown;

		IDirectXDraw(void);				        // Class Constructor
	   ~IDirectXDraw(void);			           // Class Destructor

		HRESULT CreateDirectX(HWND, int ScreenX = g_Global.GetScreenW(), int ScreenY = g_Global.GetScreenH(), 
			            int ScreenBPP= 16, int forceVideoMode = 0);    // Class Initialisation 
		void Release(void);                   // Class Closing
		void RestoreDisplayMode(void);

        //LPDIRECTDRAWSURFACE PicklockFrontBuffer( LPRECT lpRect, LPDDSURFACEDESC sur, DWORD flags, HANDLE hEvent );
        LPDIRECTDRAWSURFACE PicklockBackBuffer( LPRECT lpRect, LPDDSURFACEDESC sur, DWORD flags, HANDLE hEvent );
        LPDIRECTDRAWSURFACE GetBackBufferDirect();
        LPDIRECTDRAWSURFACE GetFrontBufferDirect();
		LPDIRECTDRAWSURFACE	lpDDSSurface_Effects;

        void GetDC( HDC *x, int a );
        void ReleaseDC( HDC x, int a );
        void GetFDC( HDC *x);
        void ReleaseFDC( HDC x);
        void Flip();
        void Blt( LPDIRECTDRAWSURFACE x );
        void Blt2( LPDIRECTDRAWSURFACE x ,RECT *rcSource,RECT *rcDest);
        void RestoreView();
        void RestoreSurface();
        void RestoreSurfaceF();
};

typedef class IDirectXDraw FAR *LPDIRECTXDRAW;

void DXDErrorHandle(HRESULT, char *);
void DXDSetView(void);
void DXDSetView2(void);
void DXDFadeEntry(LPPALETTEENTRY, LPPALETTEENTRY, int MaxRed = 100, int MaxGreen = 100, int MaxBlue = 100, int Spare = 6);
HRESULT DXDClear(void);

// DirectX Draw Global Unique Identifier (GUID)
extern LPDIRECTXDRAW        lpDXDirectDraw;

//#define DXDPicklockFrontBuffer lpDXDirectDraw->PicklockFrontBuffer
//#define DXDPicklockBackBuffer  lpDXDirectDraw->PicklockBackBuffer


static LPDIRECTDRAWSURFACE	lpBackBuffer = NULL;
static LPDIRECTDRAWSURFACE	lpDXPrimarySurface = NULL;


#endif