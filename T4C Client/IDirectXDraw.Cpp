// ************************************************************************
// ***                                                                  ***
//      File Name: IDirectXDraw.Cpp
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
// *** Copyright (c) 1997-1998 Vircom.             All rights reserved. ***

#include "pch.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "IDirectXDraw.h"
#include "PowerDebug.h"
#include "LocalString.h"
#include "App.h"
#include "Global.h"
#include "MemoryManager.h"
#include "MacroHandler.h"

extern Global g_Global;


#ifdef _DEBUG
int DebugDC = 0;
#endif

LPDIRECTDRAW        lpdd;

/*RECT	Rectangle_Src,Rectangle_Dest;	//pour l'affichage des sprites

void Zoom(LPDIRECTDRAWSURFACE lpImageSurface, int Left1, int Top1, int Right1, int Bottom1, int Left2, int Top2, int Right2, int Bottom2, int Iteration, int Nombre_Iterations);
*/
//DirectX Draw Global Unique Identifier (GUID)
//static LPDIRECTDRAWSURFACE  lpDXPrimarySurface = NULL;
static LPDIRECTDRAWSURFACE  lpDXComplexSurface = NULL;
static LPDIRECTDRAWSURFACE  lpDXDummySurface = NULL;
LPDIRECTXDRAW        lpDXDirectDraw = NULL;
bool primaryDrawBuffer = true;

IDirectXDraw::IDirectXDraw(void) {
/* Initialize the DirectX Environment.
   */                      
   lpddpal = 0;
   lpdd = 0;
   lpFrontBuffer = 0;
   lpBackBuffer = 0;
   DXCompleteInit = FALSE;
}


IDirectXDraw::~IDirectXDraw(void) {
/* Close the DirectX Draw Environment.
   */                      
   Release();
}


LONG CALLBACK HookMessageCallback( int message, WPARAM wParam, LPARAM lParam );

HRESULT IDirectXDraw::CreateDirectX(HWND hwnd, int ScreenX, int ScreenY, int ScreenBPP, int forceVideoMode) 
{
   HRESULT hresult;

   RestoreDisplayMode();
   Release();
   

   // Initialize the DirectX Draw Environment.
   LOG << "* DRW = 1- DirectDrawCreate\r\n";
   try 
   { 
      hresult = DirectDrawCreate(NULL, &lpdd, NULL);
   } catch (...) 
   {
      g_App.SetError(1, g_LocalString[154]);
      g_App.Close(g_LocalString[154]);
   }

   // Unknow random initialization error, this may help the system to stabilize.
   //Sleep(500);
      
   LOG << "* DRW = 2- DXDErrorHandle\r\n";
   DXDErrorHandle(hresult, "DirectDraw Init");
   if (hresult != DD_OK) {
      g_App.SetError(1, g_LocalString[396]);
      g_App.Close(g_LocalString[396]);
   }
   
   
   LOG << "* DRW = 3- SetCooperativeLevel\r\n";
   ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
   ddsd.dwSize            = sizeof(ddsd);
   lpdd->GetDisplayMode(&ddsd);
   OldScreenX   = ddsd.dwWidth;
   OldScreenY   = ddsd.dwHeight;
   OldScreenBPP = ddsd.ddpfPixelFormat.dwRGBBitCount;//afk 2 min k
   if (Custom.gWindowed)
   {
      if ((hresult = lpdd->SetCooperativeLevel(hwnd, DDSCL_NORMAL )) != DD_OK) 
         return hresult;// Can't set the Cooperative Level.
      
      if (OldScreenBPP != 16){
         lpdd->SetDisplayMode(NULL,NULL,16);
	  }
   }
   else 
   {
      if ((hresult = lpdd->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)) != DD_OK) 
         return hresult;// Can't set the Cooperative Level.
   }   
   
   SetWindowPos(hwnd,NULL,0,0,g_Global.GetDisplayW(),g_Global.GetDisplayH(),SWP_DRAWFRAME);
   ShowWindow(hwnd,SW_SHOW);
   RECT ClientRect; 
   GetClientRect(hwnd,&ClientRect);
   SetWindowPos(hwnd,NULL,0,0,2*g_Global.GetDisplayW()-ClientRect.right,2*g_Global.GetDisplayH()-ClientRect.bottom,SWP_NOMOVE);

   if (Custom.gWindowed)
   {
      LOG << "* DRW = 4- Start Windowed\r\n";
      
      memset( &ddsd, 0, sizeof(ddsd) );
      ddsd.dwSize            = sizeof(ddsd);
      ddsd.dwFlags           = DDSD_CAPS;
      ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE;
      
      if ((hresult = lpdd->CreateSurface(&ddsd, &lpFrontBuffer, NULL)) != DD_OK)
         return hresult; // Can't create the Surface.
      
      
      lpDXPrimarySurface = lpFrontBuffer;
      
      LOG << "* DRW = 4- CreateClipper\r\n";
      if (hresult = lpdd->CreateClipper( 0, &lpClipper, NULL )!= DD_OK)
      {
         lpFrontBuffer->Release();
         lpdd->Release();
         return hresult;
      }
      
      LOG << "* DRW = 4- SetHWnd\r\n";
      if (hresult = lpClipper->SetHWnd( 0, hwnd )!= DD_OK)
      {
         lpClipper-> Release();
         lpFrontBuffer->Release();
         lpdd->Release();
         return hresult;
      }
      
      LOG << "* DRW = 4- SetClipper\r\n";
      if (hresult = lpDXPrimarySurface->SetClipper( lpClipper )!= DD_OK)
      {
         lpClipper-> Release();
         lpFrontBuffer->Release();
         lpdd->Release();
         return hresult;
      }

		memset( &ddsd, 0, sizeof(ddsd) );
		ddsd.dwSize = sizeof( ddsd );
		ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_CKSRCBLT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth = g_Global.GetDisplayW();
		ddsd.dwHeight = g_Global.GetDisplayH();

      LOG << "* DRW = 4- CreateSurface(backBuffer)\r\n";
      if (hresult = lpdd->CreateSurface( &ddsd, &lpBackBuffer, NULL )!= DD_OK)
      {
         lpClipper-> Release();
         lpFrontBuffer->Release();
         lpdd->Release();
         return hresult;
      }
      
      lpDXComplexSurface = lpBackBuffer;
      
      lpdd->GetDisplayMode(&ddsd);
      wRMask = (WORD)ddsd.ddpfPixelFormat.dwRBitMask;
      wGMask = (WORD)ddsd.ddpfPixelFormat.dwGBitMask;
      wBMask = (WORD)ddsd.ddpfPixelFormat.dwBBitMask;

      switch (forceVideoMode) 
      {
         case 5:
   			wRMask = 5;
            wGMask = 6;
            wBMask = 5;
         break;
         case 6:
            wRMask = 5;
            wGMask = 6;
            wBMask = 5;
         break;
         default:
         break;
      }
	  
      int i;
      for (i = 0; i < 16; i++) 
      {
         if (wRMask & (1<<i)) 
         {
            bRUp = i;
            i = 16;
         }
      }
      for (i = 0; i < 16; i++) 
      {
         if (wGMask & (1<<i)) 
         {
            bGUp = i;
            i = 16;
         }
      }
      for (i = 0; i < 16; i++) 
      {
         if (wBMask & (1<<i)) 
         {
            bBUp = i;
            i = 16;
         }
      }
      for (i = 15; i >= 0; i--) 
      {
         if (wRMask & (1 << i)) 
         {
            bRDown = 8-(i-bRUp+1);
            i = -1;
         }
      }
      for (i = 15; i >= 0; i--) 
      {
         if (wGMask & (1 << i)) 
         {
            bGDown = 8-(i-bGUp+1);
            i = -1;
         }
      }
      for (i = 15; i >= 0; i--) 
      {
         if (wBMask & (1 << i)) 
         {
            bBDown = 8-(i-bBUp+1);
            i = -1;
         }
      }
      DDBLTFX ddbltfx;
      ddbltfx.dwSize = sizeof(ddbltfx);
      ddbltfx.dwFillColor = 0;
      
      LOG << "* DRW = 4- lpDXPrimarySurface->Blt\r\n";
      lpDXPrimarySurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
      lpDXComplexSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT,&ddbltfx);
      LOG << "* DRW = 5- Fin Windowed...\r\n";
   }
   else 
   {
      if ((hresult = lpdd->SetDisplayMode(g_Global.GetDisplayW(), g_Global.GetDisplayH(), 16)) != DD_OK)
         // Can't set the display mode.
         return hresult;
      
      if ((hresult = lpdd->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN)) != DD_OK)
         // Can't set the Cooperative Level.
         return hresult; 
      
      LOG << "* DRW = 1-5\r\n";
      if ((hresult = lpdd->SetDisplayMode(g_Global.GetDisplayW(), g_Global.GetDisplayH(), 16)) != DD_OK)
         // Can't set the display mode.
         return hresult;
      
      LOG << "* DRW = 1-6\r\n";
      // Create a PrimarySurface

      ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
      ddsd.dwSize            = sizeof(ddsd);
      ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
      ddsd.dwBackBufferCount = 1;
      ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX |
                               DDSCAPS_VIDEOMEMORY;
      
      if ((hresult = lpdd->CreateSurface(&ddsd, &lpFrontBuffer, NULL)) != DD_OK) {
         // Can't create the Surface.
         ZeroMemory(&ddsd, sizeof(DDSURFACEDESC));
         ddsd.dwSize            = sizeof(ddsd);
         ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
         ddsd.dwBackBufferCount = 1;
         ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
         
         if ((hresult = lpdd->CreateSurface(&ddsd, &lpFrontBuffer, NULL)) != DD_OK) {
            // Can't create the Surface.
            return hresult;
         }
      }
      {
            DDSURFACEDESC Sprite;  
            memset( &Sprite, 0, sizeof( DDSURFACEDESC ) );
            Sprite.dwSize            = sizeof(Sprite);
            Sprite.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
            Sprite.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
            Sprite.dwHeight          = g_Global.GetDisplayH();
            Sprite.dwWidth           = g_Global.GetDisplayW();

            DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Sprite, &lpDXDummySurface, NULL), "Create DummySurface");
      }


      lpdd->GetDisplayMode(&ddsd);
      wRMask = (WORD)ddsd.ddpfPixelFormat.dwRBitMask;
      wGMask = (WORD)ddsd.ddpfPixelFormat.dwGBitMask;
      wBMask = (WORD)ddsd.ddpfPixelFormat.dwBBitMask;

      switch (forceVideoMode) {
         case 5:
            wRMask = 31744;
            wGMask = 992;
            wBMask = 31;
         break;
         case 6:
            /*wRMask = 63488;
            wGMask = 2016;
            wBMask = 31;*/
			wRMask = 5;
            wGMask = 6;
            wBMask = 5;
         break;
         default:
         break;
      };

      /*char Temp[100];
      sprintf(Temp, "BIT %d\r\n", wGMask);
      OutputDebugString(Temp);*/

	  /* allocated memory reduced */
	  char *Temp; 
	  Temp = (char*)malloc(100);
	  sprintf(Temp, "BIT %d\r\n", wGMask);
	  OutputDebugString(Temp);
	  free(Temp);

      int i;
      for (i = 0; i < 16; i++) {
         if (wRMask & (1<<i)) {
            bRUp = i;
            i = 16;
         }
      }
      for (i = 0; i < 16; i++) {
         if (wGMask & (1<<i)) {
            bGUp = i;
            i = 16;
         }
      }
      for (i = 0; i < 16; i++) {
         if (wBMask & (1<<i)) {
            bBUp = i;
            i = 16;
         }
      }
      for (i = 15; i >= 0; i--) {
         if (wRMask & (1 << i)) {
            bRDown = 8-(i-bRUp+1);
            i = -1;
         }
      }
      for (i = 15; i >= 0; i--) {
         if (wGMask & (1 << i)) {
            bGDown = 8-(i-bGUp+1);
            i = -1;
         }
      }
      for (i = 15; i >= 0; i--) {
         if (wBMask & (1 << i)) {
            bBDown = 8-(i-bBUp+1);
            i = -1;
         }
      }

      
      LOG << "* DRW = 1-7\r\n";
      // Create the ComplexSurface
      DDSCAPS ddscaps;
      ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
      
      if ((hresult = lpFrontBuffer->GetAttachedSurface(&ddscaps, &lpBackBuffer)) 
         != DD_OK)
         // Can't create the Surface
         return hresult;
      
      LOG << "* DRW = 1-8\r\n";
      
      lpDXPrimarySurface = lpFrontBuffer;
      lpDXComplexSurface = lpBackBuffer;
      
      // Clear both PrimarySuface and ComplexSurface.
      DDBLTFX ddbltfx;
      ddbltfx.dwSize = sizeof(ddbltfx);
      ddbltfx.dwFillColor = 0;
      
      LOG << "* DRW = 1-10\r\n";
      lpDXPrimarySurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT,
         &ddbltfx);
      lpDXComplexSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT,
         &ddbltfx);
      LOG << "* DRW = 1-11\r\n";
   }
   DXCompleteInit = TRUE;


   return DD_OK;
}

void IDirectXDraw::RestoreDisplayMode(void) {
	if (DXCompleteInit)
		lpdd->RestoreDisplayMode();
}

#define CLOSELOG( str ) if( Custom.Debug ){ FILE *f = fopen( "exit.log", "ab" ); fprintf( f, "\r\n%s", str ); fclose( f ); OutputDebugString( str ); };
extern AppManagement g_App;

void IDirectXDraw::Release(void) {
/* Close the DirectX Draw Environment.
 */                      
	if (lpBackBuffer/* && g_App.isFocus() */) { //BLBL Freeze test
   // Chk si le lpBackBuffer n'a pas deja ete deleter.
      CLOSELOG( "Releasing lpBackBuffer" );
        
      if( lpBackBuffer->IsLost() == DD_OK ){
          lpBackBuffer->Release();
      }else{
          CLOSELOG( "BackBuffer lost!" );
      }
      lpBackBuffer = 0;
      lpDXComplexSurface = 0;
   }else{
        CLOSELOG( "lpBackBuffer not released." );
   }

   if (lpFrontBuffer /*&& g_App.isFocus() */) {//BLBL Freeze test 
       CLOSELOG( "Releasing lpFrontBuffer" );
   // Chk si le lpFrontBuffer n'a pas deja ete deleter.
       if( lpFrontBuffer->IsLost() == DD_OK ){
            lpFrontBuffer->Release();
       }else{
           CLOSELOG( "FrontBuffer lost!" );
       }
		lpFrontBuffer = 0;
      lpDXPrimarySurface = 0;
   }else{
       CLOSELOG( "lpFrontBuffer not released." );
   }
             
   if (lpdd) {
        CLOSELOG( "Restoring display mode" );

   // Verifi si le lpdd n'a pas deja ete deleter.
      lpdd->RestoreDisplayMode();

      CLOSELOG( "Releasing lpdd" );
      lpdd->Release();
		lpdd = 0;   
   }
}

void DXDErrorHandle(HRESULT hresult, char *Method) {
/* Handle Errors.
 */
	switch (hresult) {
	case DD_OK: 
	// Everything is fine.
		break;
	
	case DDERR_ALREADYINITIALIZED: 
	// The object has already been initialized.
		LOG << "Error Number " << 1;
		break;

	case DDERR_BLTFASTCANTCLIP:														
	// A DirectDrawClipper object is attached to a source surface that has 
	// passed into a call to the IDirectDrawSurface2::BltFast method. 
		LOG << "Error Number " << 2;
		break;

	case DDERR_CANNOTATTACHSURFACE:
	// A surface cannot be attached to another requested surface. 
		LOG << "Error Number " << 3;
		break;

	case DDERR_CANNOTDETACHSURFACE:
	// A surface cannot be detached from another requested surface.
		LOG << "Error Number " << 4;
		break;

	case DDERR_CANTCREATEDC:
	// Windows cannot create any more device contexts (DCs).
		LOG << "Error Number " << 5;
		break;

	case DDERR_CANTDUPLICATE:
	// Primary and 3D surfaces, or surfaces that are implicitly created, 
	// cannot be duplicated.
		LOG << "Error Number " << 6;
		break;

	case DDERR_CANTLOCKSURFACE:															
	// Access to this surface is refused because an attempt was made to lock
	// the primary surface without DCI support. 
		LOG << "Error Number " << 7;
		break;

	case DDERR_CANTPAGELOCK:																
	// An attempt to page lock a surface failed. Page lock will not work on a 
	// display-memory surface or an emulated primary surface.
		LOG << "Error Number " << 8;
		break;

	case DDERR_CANTPAGEUNLOCK:																
	// An attempt to page unlock a surface failed. Page unlock will not work
	// on a display-memory surface or an emulated primary surface.
		LOG << "Error Number " << 9;
		break;

	case DDERR_CLIPPERISUSINGHWND:														
	// An attempt was made to set a clip list for a DirectDrawClipper object
	// that is already monitoring a window handle.
		LOG << "Error Number " << 10;
		break;

	case DDERR_COLORKEYNOTSET:
	// No source color key is specified for this operation.
		LOG << "Error Number " << 11;
		break;

	case DDERR_CURRENTLYNOTAVAIL:
	// No support is currently available. 
		LOG << "Error Number " << 12;
		break;

	case DDERR_DCALREADYCREATED:														
	// A device context (DC) has already been returned for this surface. Only
	// one DC can be retrieved for each surface. 
		LOG << "Error Number " << 13;
		break;

	case DDERR_DIRECTDRAWALREADYCREATED:												
	// A DirectDraw object representing this driver has already been created
	// for this process. 
		LOG << "Error Number " << 14;
		break;

	case DDERR_EXCEPTION:																	
	// An exception was encountered while performing the requested operation.
		LOG << "Error Number " << 15;
		break;

	case DDERR_EXCLUSIVEMODEALREADYSET:													
	// An attempt was made to set the cooperative level when it was already 
	// set to exclusive. 
		LOG << "Error Number " << 16;
		break;

	case DDERR_GENERIC:
	// There is an undefined error condition. 
		LOG << "Error Number " << 17;
		break;

	case DDERR_HEIGHTALIGN:																	
	// The height of the provided rectangle is not a multiple of the required
	// alignment. 
		LOG << "Error Number " << 18;
		break;

	case DDERR_HWNDALREADYSET:																
	// The DirectDraw cooperative level window handle has already been set. 
	// It cannot be reset while the process has surfaces or palettes created. 
		LOG << "Error Number " << 19;
		break;

	case DDERR_HWNDSUBCLASSED:																
	// DirectDraw is prevented from restoring state because the DirectDraw 
	// cooperative level window handle has been subclassed. 
		LOG << "Error Number " << 20;
		break;

	case DDERR_IMPLICITLYCREATED:														
	// The surface cannot be restored because it is an implicitly created 
	// surface.
		LOG << "Error Number " << 21;
		break;

	case DDERR_INCOMPATIBLEPRIMARY:														
	// The primary surface creation request does not match with the existing 
	// primary surface. 
		LOG << "Error Number " << 22;
		break;

	case DDERR_INVALIDCAPS:
	// One or more of the capability bits passed to the callback function are
	// incorrect. 
		LOG << "Error Number " << 23;
		break;

	case DDERR_INVALIDCLIPLIST:
	// DirectDraw does not support the provided clip list.
		LOG << "Error Number " << 24;
		break;

	case DDERR_INVALIDDIRECTDRAWGUID:													
	// The globally unique identifier (GUID) passed to the DirectDrawCreate 
	// function is not a valid DirectDraw driver identifier.
		LOG << "Error Number " << 25;
		break;

	case DDERR_INVALIDMODE:
	// DirectDraw does not support the requested mode. 
		LOG << "Error Number " << 26;
		break;

	case DDERR_INVALIDOBJECT:
	// DirectDraw received a pointer that was an invalid DirectDraw object.
		LOG << "Error Number " << 27;
		break;

	case DDERR_INVALIDPARAMS:
	// One or more of the parameters passed to the method are incorrect. 
		LOG << "Error Number " << 28;
		break;

	case DDERR_INVALIDPIXELFORMAT:
	// The pixel format was invalid as specified. 
		LOG << "Error Number " << 29;
		break;

	case DDERR_INVALIDPOSITION:
	// The position of the overlay on the destination is no longer legal. 
		LOG << "Error Number " << 30;
		break;
	
	case DDERR_INVALIDRECT:
	// The provided rectangle was invalid. 
		LOG << "Error Number " << 31;
	break;


	case DDERR_INVALIDSURFACETYPE:
	// The requested operation could not be performed because the surface was of the wrong type. 
		LOG << "Error Number " << 32;
	break;

	case DDERR_LOCKEDSURFACES:
	// One or more surfaces are locked, causing the failure of the requested operation. 
		LOG << "Error Number " << 33;
	break;

	case DDERR_NO3D:
	// No 3D hardware or emulation is present. 
		LOG << "Error Number " << 33;
	break;

	case DDERR_NOALPHAHW:
	// No alpha acceleration hardware is present or available, causing the failure of the requested operation. 
		LOG << "Error Number " << 34;
	break;
	

	case DDERR_NOBLTHW:
	// No blitter hardware is present. 
		LOG << "Error Number " << 35;
	break;

	case DDERR_NOCLIPLIST:
	// No clip list is available. 
		LOG << "Error Number " << 36;
	break;

	case DDERR_NOCLIPPERATTACHED:
	// No DirectDrawClipper object is attached to the surface object. 
		LOG << "Error Number " << 37;
	break;

	case DDERR_NOCOLORCONVHW:
	// The operation cannot be carried out because no color-conversion hardware is present or available. 
		LOG << "Error Number " << 38;
	break;

	case DDERR_NOCOLORKEY:
	// The surface does not currently have a color key. 
		LOG << "Error Number " << 39;
	break;

	case DDERR_NOCOLORKEYHW:
	// The operation cannot be carried out because there is no hardware support for the destination color key. 
		LOG << "Error Number " << 40;
	break;

	case DDERR_NOCOOPERATIVELEVELSET:
	// A create function is called without the IDirectDraw2::SetCooperativeLevel method being called. 
		LOG << "Error Number " << 41;
		break;

	case DDERR_NODC:
	// No DC has ever been created for this surface. 
		LOG << "Error Number " << 42;
	break;

	case DDERR_NODDROPSHW:
	// No DirectDraw raster operation (ROP) hardware is available. 
		LOG << "Error Number " << 43;
	break;

	case DDERR_NODIRECTDRAWHW:
	// Hardware-only DirectDraw object creation is not possible; the driver does not support any hardware. 
		LOG << "Error Number " << 44;
	break;

	case DDERR_NODIRECTDRAWSUPPORT:
	// DirectDraw support is not possible with the current display driver. 
		LOG << "Error Number " << 45;
	break;

	case DDERR_NOEMULATION:
	// Software emulation is not available. 
		LOG << "Error Number " << 46;
	break;

	case DDERR_NOEXCLUSIVEMODE:
	// The operation requires the application to have exclusive mode, but the application does not have exclusive mode. 
		LOG << "Error Number " << 47;
	break;

	case DDERR_NOFLIPHW:
	// Flipping visible surfaces is not supported. 
		LOG << "Error Number " << 48;
	break;

	case DDERR_NOGDI:
	// No GDI is present. 
		LOG << "Error Number " << 49;
	break;

	case DDERR_NOHWND:
	// Clipper notification requires a window handle, or no window handle has been previously set as the cooperative level window handle. 
		LOG << "Error Number " << 50;
	break;

	case DDERR_NOMIPMAPHW:
	// The operation cannot be carried out because no mipmap texture mapping hardware is present or available. 
		LOG << "Error Number " << 51;
	break;

	case DDERR_NOMIRRORHW:
	// The operation cannot be carried out because no mirroring hardware is present or available. 
		LOG << "Error Number " << 52;
	break;

	case DDERR_NOOVERLAYDEST:
	// The IDirectDrawSurface2::GetOverlayPosition method is called on an overlay that the IDirectDrawSurface2::UpdateOverlay method has not been called on to establish a destination. 
		LOG << "Error Number " << 53;
	break;

	case DDERR_NOOVERLAYHW:
	// The operation cannot be carried out because no overlay hardware is present or available. 
		LOG << "Error Number " << 54;
	break;

	case DDERR_NOPALETTEATTACHED:
	// No palette object is attached to this surface. 
		LOG << "Error Number " << 55;
	break;

	case DDERR_NOPALETTEHW:
	// There is no hardware support for 16- or 256-color palettes. 
		LOG << "Error Number " << 56;
	break;

	case DDERR_NORASTEROPHW:
	// The operation cannot be carried out because no appropriate raster operation hardware is present or available. 
		LOG << "Error Number " << 57;
	break;

	case DDERR_NOROTATIONHW:
	// The operation cannot be carried out because no rotation hardware is present or available. 
		LOG << "Error Number " << 58;
	break;

	case DDERR_NOSTRETCHHW:
	// The operation cannot be carried out because there is no hardware support for stretching. 
		LOG << "Error Number " << 59;
	break;

	case DDERR_NOT4BITCOLOR:
	// The DirectDrawSurface object is not using a 4-bit color palette and the requested operation requires a 4-bit color palette. 
		LOG << "Error Number " << 60;
	break;

	case DDERR_NOT4BITCOLORINDEX:
	// The DirectDrawSurface object is not using a 4-bit color index palette and the requested operation requires a 4-bit color index palette. 
		LOG << "Error Number " << 61;
	break;

	case DDERR_NOT8BITCOLOR:
	// The DirectDrawSurface object is not using an 8-bit color palette and the requested operation requires an 8-bit color palette. 
		LOG << "Error Number " << 62;
	break;

	case DDERR_NOTAOVERLAYSURFACE:
	// An overlay component is called for a non-overlay surface. 
		LOG << "Error Number " << 63;
	break;

	case DDERR_NOTEXTUREHW:
	// The operation cannot be carried out because no texture-mapping hardware is present or available. 
		LOG << "Error Number " << 64;
	break;

	case DDERR_NOTFLIPPABLE:
	// An attempt has been made to flip a surface that cannot be flipped. 
		LOG << "Error Number " << 65;
	break;

	case DDERR_NOTFOUND:
	// The requested item was not found. 
		LOG << "Error Number " << 66;
	break;

	case DDERR_NOTINITIALIZED:
	// An attempt was made to call an interface method of a DirectDraw object created by CoCreateInstance before the object was initialized. 
		LOG << "Error Number " << 67;
	break;

	case DDERR_NOTLOCKED:
	// An attempt is made to unlock a surface that was not locked. 
		LOG << "Error Number " << 68;
	break;

	case DDERR_NOTPAGELOCKED:
	// An attempt is made to page unlock a surface with no outstanding page locks. 
		LOG << "Error Number " << 69;
	break;

	case DDERR_NOTPALETTIZED:
	// The surface being used is not a palette-based surface. 
		LOG << "Error Number " << 70;
	break;

	case DDERR_NOVSYNCHW:
	// The operation cannot be carried out because there is no hardware support for vertical blank synchronized operations. 
		LOG << "Error Number " << 71;
	break;

	case DDERR_NOZBUFFERHW:
	// The operation to create a z-buffer in display memory or to perform a blit using a z-buffer cannot be carried out because there is no hardware support for z-buffers. 
		LOG << "Error Number " << 72;
	break;

	case DDERR_NOZOVERLAYHW:
	// The overlay surfaces cannot be z-layered based on the z-order because the hardware does not support z-ordering of overlays. 
		LOG << "Error Number " << 73;
	break;

	case DDERR_OUTOFCAPS:
	// The hardware needed for the requested operation has already been allocated. 
		LOG << "Error Number " << 74;
	break;

	case DDERR_OUTOFMEMORY:
	// DirectDraw does not have enough memory to perform the operation. 
		LOG << "Error Number " << 75;
	break;

	case DDERR_OUTOFVIDEOMEMORY:
	// DirectDraw does not have enough display memory to perform the operation. 
		LOG << "Error Number " << 76;
	break;

   case DDERR_OVERLAYCANTCLIP:
   // The hardware does not support clipped overlays. 
		LOG << "Error Number " << 77;
	break;

   case DDERR_OVERLAYCOLORKEYONLYONEACTIVE:
   // An attempt was made to have more than one color key active on an overlay. 
		LOG << "Error Number " << 78;
	break;

   case DDERR_OVERLAYNOTVISIBLE :
   // The IDirectDrawSurface2::GetOverlayPosition method is called on a hidden overlay. 
		LOG << "Error Number " << 79;
	break;

   case DDERR_PALETTEBUSY :
   // Access to this palette is refused because the palette is locked by another thread. 
		LOG << "Error Number " << 80;
	break;

   case DDERR_PRIMARYSURFACEALREADYEXISTS:
	// This process has already created a primary surface. 
		LOG << "Error Number " << 81;
	break;

   case DDERR_REGIONTOOSMALL :
   // The region passed to the IDirectDrawClipper::GetClipList method is too small. 
		LOG << "Error Number " << 82;
	break;

   case DDERR_SURFACEALREADYATTACHED :
   // An attempt was made to attach a surface to another surface to which it is already attached. 
		LOG << "Error Number " << 83;
	break;

   case DDERR_SURFACEALREADYDEPENDENT :
   // An attempt was made to make a surface a dependency of another surface to which it is already dependent. 
		LOG << "Error Number " << 84;
	break;

   case DDERR_SURFACEBUSY :
   // Access to the surface is refused because the surface is locked by another thread. 
		LOG << "Error Number " << 85;
	break;

   case DDERR_SURFACEISOBSCURED :
   // Access to the surface is refused because the surface is obscured. 
		LOG << "Error Number " << 86;
	break;

   case DDERR_SURFACELOST :
   // Access to the surface is refused because the surface memory is gone. The DirectDrawSurface object representing this surface should have the IDirectDrawSurface2::Restore method called on it. 
		LOG << "Error Number " << 87;
	break;

   case DDERR_SURFACENOTATTACHED :
   // The requested surface is not attached. 
		LOG << "Error Number " << 88;
	break;

   case DDERR_TOOBIGHEIGHT :
   // The height requested by DirectDraw is too large. 
		LOG << "Error Number " << 89;
	break;

   case DDERR_TOOBIGSIZE :
   // The size requested by DirectDraw is too large. However, the individual height and width are OK. 
		LOG << "Error Number " << 90;
	break;

   case DDERR_TOOBIGWIDTH :
   // The width requested by DirectDraw is too large. 
		LOG << "Error Number " << 91;
	break;

	case DDERR_UNSUPPORTED:
   // The operation is not supported. 
		LOG << "Error Number " << 92;
	break;

   case DDERR_UNSUPPORTEDFORMAT :
   // The FourCC format requested is not supported by DirectDraw. 
		LOG << "Error Number " << 93;
	break;

   case DDERR_UNSUPPORTEDMASK :
   // The bitmask in the pixel format requested is not supported by DirectDraw. 
		LOG << "Error Number " << 94;
	break;

   case DDERR_UNSUPPORTEDMODE :
   // The display is currently in an unsupported mode. 
		LOG << "Error Number " << 95;
	break;

   case DDERR_VERTICALBLANKINPROGRESS :
   // A vertical blank is in progress. 
		LOG << "Error Number " << 96;
	break;

   case DDERR_WASSTILLDRAWING :
   // The previous blit operation that is transferring information to or from this surface is incomplete. 
		LOG << "Error Number " << 97;
	break;

   case DDERR_WRONGMODE :
   // This surface cannot be restored because it was created in a different mode. 
		LOG << "Error Number " << 98;
	break;

   case DDERR_XALIGN :
   // The provided rectangle was not horizontally aligned on a required boundary.
		LOG << "Error Number " << 99;
	break;
	
	default:
	// Unknown error
		LOG << "Error Number " << 100;
		break;
	}

   if (hresult != DD_OK) {
      LOG << " In Method [" << Method << "]\r\n";

		//g_App.SetError(1, g_LocalString[154]);
		//g_App.Close();
      throw "DxError";
	}
}

/*void DXDFadeEntry(LPPALETTEENTRY Dest, LPPALETTEENTRY Source, int MaxRed, int MaxGreen, int MaxBlue, int Spare) {
   for (int i=0; i < DEFAULT_COLORS; i++) {
		Dest[i].peRed   = Source[i].peRed   * MaxRed / 100;
		Dest[i].peGreen = Source[i].peGreen * MaxGreen / 100;
		Dest[i].peBlue  = Source[i].peBlue  * MaxBlue / 100;
	    Dest[i].peFlags = PC_NOCOLLAPSE;
	}

   for (i = DEFAULT_COLORS-Spare; i < DEFAULT_COLORS; i++) {
		Dest[i].peRed   = Source[i].peRed;
		Dest[i].peGreen = Source[i].peGreen;
		Dest[i].peBlue  = Source[i].peBlue;
	    Dest[i].peFlags = PC_NOCOLLAPSE;
   }
}*/

void DXDSetView(void) {
	if (Custom.gWindowed)
	{
	   DXDErrorHandle(lpDXDirectDraw->lpdd->SetCooperativeLevel(siGethWnd(), DDSCL_NORMAL), "SetCooperative"); 
	   DXDErrorHandle(lpDXDirectDraw->lpdd->RestoreDisplayMode(), "Restore View"); 
	}
	else
	{
	   DXDErrorHandle(lpDXDirectDraw->lpdd->SetCooperativeLevel(siGethWnd(), DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN), "SetCooperative"); 
	   DXDErrorHandle(lpDXDirectDraw->lpdd->RestoreDisplayMode(), "Restore View"); 
	   DXDErrorHandle(lpDXDirectDraw->lpdd->SetDisplayMode(g_Global.GetDisplayW(), g_Global.GetDisplayH(), 16), "Set View"); 
	}
}

void DXDSetView2(void) {
   //DXDErrorHandle(lpDXComplexSurface->SetPalette(lpDXDirectDraw->lpddfrontpal), "Set Palette"); 
   //DXDErrorHandle(lpDXPrimarySurface->SetPalette(lpDXDirectDraw->lpddfrontpal), "Set Palette");
/*   LPPALETTEENTRY lpPal;
   lpPal = new PALETTEENTRY [DEFAULT_COLORS];
   memset(lpPal, 0, sizeof(PALETTEENTRY) << DEFAULT_BPP);
   for (int i=0; i < DEFAULT_COLORS; i++) 
      lpPal[i].p eFlags = PC_NOCOLLAPSE;
   lpPal[255].peRed = lpPal[255].peGreen = lpPal[255].peBlue = 255;
   lpDXDirectDraw->lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | 
		               DDPCAPS_INITIALIZE, lpPal, &lpDXDirectDraw->lpddfrontpal, 
							NULL);
*/}

HRESULT DXDClear(void) {
	DDBLTFX ddbltfx;
    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = 0;
      
    return lpDXComplexSurface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}

//////////////////////////////////////////////////////////////////////////////////////////
//LPDIRECTDRAWSURFACE IDirectXDraw::PicklockFrontBuffer //BLBL 26 mars 2009 désactivé complètement cette fonction.
//////////////////////////////////////////////////////////////////////////////////////////
// Gets a lock on the front buffer. If the surface as lost, the fonction
// will try to restore it only if the application currently has the focus. 
// 
/*(
 LPRECT lpRect,
 LPDDSURFACEDESC sur,
 DWORD flags,
 HANDLE hEvent
)*/
// Return: LPDIRECTDRAWSURFACE, A valid surface or NULL if a lock couldn't be aquired.
//////////////////////////////////////////////////////////////////////////////////////////
//{
   /*if( !g_App.isFocus() ){ 
       Sleep(0);
       return NULL;
   }*/

   /*const MaxTries = 5;
   int tries = 0;

   do {
       while( lpFrontBuffer->IsLost() != DD_OK ){
           lpFrontBuffer->Restore();
           Sleep(5);
           if( tries++ >= 5 ){
              return NULL;
           }
       }

      HRESULT hr = lpFrontBuffer->Lock(
          lpRect, sur, flags, hEvent
      );
      
      if( hr == DDERR_SURFACELOST ){
         Sleep(5);
         if( !g_App.isFocus() ){
             return NULL;
         }
         lpFrontBuffer->Restore();
      }else{
         break;
      }
      if( tries++ > MaxTries ){
          return NULL;
      }
   } while (1);
   return lpFrontBuffer;
}*/

//////////////////////////////////////////////////////////////////////////////////////////
LPDIRECTDRAWSURFACE IDirectXDraw::PicklockBackBuffer
//////////////////////////////////////////////////////////////////////////////////////////
// Gets a lock on the back buffer. If the surface as lost, the fonction
// will try to restore it only if the application currently has the focus. 
// 
(
 LPRECT lpRect,
 LPDDSURFACEDESC sur,
 DWORD flags,
 HANDLE hEvent
)
// Return: LPDIRECTDRAWSURFACE, A valid surface or NULL if a lock couldn't be aquired.
//////////////////////////////////////////////////////////////////////////////////////////
{
    //LOG << "\r\nPicklock1";
    /*if( !g_App.isFocus() ){
        LOG << "\r\nPicklock2";
        Sleep(0);
        return NULL;
    }*/

	
   const int MaxTries = 5;
   int tries = 0;

   do {
      while( lpBackBuffer->IsLost() != DD_OK ){
//          LOG << "\r\nPicklock 2.1";
          lpFrontBuffer->Restore();
          Sleep(5) ;
          if( tries++ >= 5 ){
             return NULL;
          }          
      }

//      LOG << "\r\nPicklock3";
      HRESULT hr = lpBackBuffer->Lock(
          lpRect, sur, flags, hEvent
      );
      
//      LOG << "\r\nPicklock4";
      if( hr == DDERR_SURFACELOST ){
//         LOG << "\r\nPicklock5";
          Sleep(5);
         /*if( !g_App.isFocus() ){//BLBL Freeze test
//             LOG << "\r\nPicklock6";
             return NULL;
         }*/
         lpBackBuffer->Restore(); 
//         LOG << "\r\nPicklock8";
      }else{
         break;
      }
      if( tries++ > MaxTries ){
          return NULL;
      }

   } while (1);
//   LOG << "\r\nPicklock9";
   return lpBackBuffer;
}


//////////////////////////////////////////////////////////////////////////////////////////
LPDIRECTDRAWSURFACE IDirectXDraw::GetBackBufferDirect( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the backbuffer
//////////////////////////////////////////////////////////////////////////////////////////
{
	/*if (!Custom.gWindowed)//BLBL Freeze test
    if( !g_App.isFocus() ){
        return lpDXDummySurface;
    }*/

    return lpBackBuffer;
}

//////////////////////////////////////////////////////////////////////////////////////////
LPDIRECTDRAWSURFACE IDirectXDraw::GetFrontBufferDirect( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the front buffer
//////////////////////////////////////////////////////////////////////////////////////////
{
	/*if (!Custom.gWindowed)//BLBL Freeze test
    if( !g_App.isFocus() ){
        return lpDXDummySurface;
    }*/
   // return lpFrontBuffer;
	return lpBackBuffer;

}

// Accessor functions
void IDirectXDraw::GetDC( HDC *x, int a ){
    if( (/*!g_App.isFocus() || */lpDXComplexSurface->IsLost())&& !Custom.gWindowed){//BLBL Freeze test
	//if( !g_App.isFocus() || lpDXComplexSurface->IsLost()){
        lpDXDummySurface->GetDC( x );
    }else{
        lpDXComplexSurface->GetDC(x);
    }
}
void IDirectXDraw::ReleaseDC( HDC x, int a ){
	if( (/*!g_App.isFocus() || */lpDXComplexSurface->IsLost())&& !Custom.gWindowed){//BLBL Freeze test
	//if( !g_App.isFocus() || lpDXComplexSurface->IsLost()){
		lpDXDummySurface->ReleaseDC( x );
    }else{
        lpDXComplexSurface->ReleaseDC(x);
    }
}
void IDirectXDraw::GetFDC( HDC *x ){
    if( (/*!g_App.isFocus() || */lpDXComplexSurface->IsLost())&& !Custom.gWindowed){//BLBL Freeze test
	//if( !g_App.isFocus() || lpDXComplexSurface->IsLost()){
		lpDXDummySurface->GetDC( x );
    }else{
        lpDXPrimarySurface->GetDC(x);
    }
}
void IDirectXDraw::ReleaseFDC( HDC x ){
	if( (/*!g_App.isFocus() || */lpDXComplexSurface->IsLost())&& !Custom.gWindowed){//BLBL Freeze test
	//if( !g_App.isFocus() || lpDXComplexSurface->IsLost()){
		lpDXDummySurface->ReleaseDC( x );
    }else{
        lpDXPrimarySurface->ReleaseDC(x);
    }
}
void IDirectXDraw::Flip()
{
   if (Custom.gWindowed)
   {
      RECT rcRectDest;
      POINT p;
      p.x = 0; p.y = 0;
      ClientToScreen(siGethWnd(), &p);
      GetClientRect(siGethWnd(), &rcRectDest);
      OffsetRect(&rcRectDest, p.x, p.y);

      lpDXPrimarySurface->SetClipper( lpClipper );
      lpDXPrimarySurface->Blt(&rcRectDest, lpDXComplexSurface, NULL, DDBLT_WAIT ,NULL);
   }
   else if( /*g_App.isFocus() && */!Custom.gWindowed )//BLBL Freeze test
   {
      primaryDrawBuffer = !primaryDrawBuffer; 
      lpDXPrimarySurface->Flip(NULL, DDFLIP_WAIT); 
   }
   else
   {
      Sleep(0);
   }
}
void IDirectXDraw::Blt( LPDIRECTDRAWSURFACE x ){
    /*if( !g_App.isFocus() && !Custom.gWindowed){//BLBL Freeze test
        Sleep(0);
        return;
    }*/
    if( lpDXComplexSurface->IsLost() != DD_OK ){
        lpDXComplexSurface->Restore();
    }
    lpDXComplexSurface->Blt(NULL, x, NULL, DDBLT_WAIT, NULL);

}

void IDirectXDraw::Blt2( LPDIRECTDRAWSURFACE x ,RECT *rcSource,RECT *rcDest)
{
    /*if( !g_App.isFocus() && !Custom.gWindowed){//BLBL Freeze test
        Sleep(0);
        return;
    }*/
    if( lpDXComplexSurface->IsLost() != DD_OK ){
        lpDXComplexSurface->Restore();
    }
    lpDXComplexSurface->Blt(rcDest, x, rcSource, DDBLT_WAIT, NULL);

}
void IDirectXDraw::RestoreView(){
    lpDXDirectDraw->RestoreDisplayMode();
}

void IDirectXDraw::RestoreSurfaceF(){
    DXDErrorHandle(lpDXPrimarySurface->Restore(), "Primary Get Focus");
}
void IDirectXDraw::RestoreSurface(){
    DXDErrorHandle(lpDXComplexSurface->Restore(), "Complex Get Focus");
}

