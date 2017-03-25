// ************************************************************************
// ***                                                                  ***
//      File Name: DirectXInput.h
//      Project:   TFC Client
//      Creation:  15 July, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         07-15-1997      1.0     TH          Initial development
//
//      Description
//         Provide an Interface to interract with the mouse.
// ***                                                                  ***
//	************************************************************************
// *** Copyright (c) 1997-1998 Vircom. All rights reserved.             ***

#ifndef __Vircom_DirectXInput_080233071597
#define __Vircom_DirectXInput_080233071597

#include "dinput.h"
#include "IDirectXDraw.h"
#include "Bitmap.h"

enum DeviceType { NODEVICE, UNKNOWN, MOUSE, KEYBOARD };

class DirectXInput {
	private:
		DIMOUSESTATE g_Status;
      BOOL DeviceWasRelease;
   public:
		// Device and Interface Variable
		LPDIRECTINPUT lpdi;				  // Direct Input Interface
		LPDIRECTINPUTDEVICE lpDIDevice; // Direct Input Device
		DeviceType Device;              // Device Type
	
		// Mouse variable.
		RECT Rect;                              // Cursor Dimensions
		int Width, Height;					       // Width and Height of ???
		int xPos, yPos;								 // (x,y) position of the Cursor
		LPDIRECTDRAWSURFACE BackgroundSurface;  // The Surface behind the Cursor
		LPDIRECTDRAWSURFACE BackgroundSurface2;  // The Surface behind the Cursor
		//LPDIRECTDRAWSURFACE *lplpCursorSurface; // Ptr to all the Cursor
	   Bitmap *VirtualGrid;							 // 
		int nbCursor;									 // Number of Cursor	
		int ScreenLeft, ScreenRight,            // Screen Left and Right Border
			 ScreenTop, ScreenBottom,			    // Screen Top and Bottom Border
			 ScreenWidth, ScreenHeight;			 // Screen Width and Height
		int yGridPixel, xGridPixel;			    //

		void Exception(HRESULT hresult, char *DebugInformation, int DebugValue = 0);

		char LastKey;
		char Status[256];
	
	public:
		// Constructor and Desctrutor
		DirectXInput();
		virtual ~DirectXInput();
	
		void Create(void);
		void CreateDevice(REFGUID GUID_Device);
		void ReleaseDevice(void);
		void Release(void);

		void SetMouseBorder(int, int, int, int);
		void GetBackground(void);
		char GetStatus(int *x, int *y, int *button);
		void SetVirtualGrid(Bitmap *);
		char KeyHit(void);
		void KeyBuffer(char *);
		void GetRelease(int *);
		void GetXYPos(int *, int *);
      BOOL IsCreated(void);
      BOOL WasRelease(void);
      void Unacquire(void);
      void Acquire(void);
};

#endif