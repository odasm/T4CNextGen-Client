// ************************************************************************
// ***                                                                  ***
//      File Name: IDirectXImage.Cpp
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
#include "pch.h"
#include <windows.h>
#include <string.h>

#include "IDirectXDraw.H"
#include "IDirectXImage.H"
#include "PowerDebug.h"
#include "App.h"
#include "Global.h"
#include "MemoryManager.h"

extern Global g_Global;


IDirectXImage::IDirectXImage(void) {
/* Image Constructor.
 */
	lpImageSurface = 0;
   lpPal = NULL;
}

HRESULT IDirectXImage::Create(int dwWidth,int dwHeight) {
/* Image Creation
 */
	DDSURFACEDESC image;
	HRESULT hresult;
	
   ZeroMemory(&image, sizeof(image));
	// Create Image Surface.
   image.dwSize            = sizeof(image);
   image.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
   image.ddsCaps.dwCaps    = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	image.dwWidth           = dwWidth;//g_Global.GetDisplayW();
   image.dwHeight          = dwHeight;//g_Global.GetDisplayH();
	


   if ((hresult = lpDXDirectDraw->lpdd->CreateSurface(&image, 
		&lpImageSurface, NULL)) != DD_OK)																
   // Can't create the Image Surface.
      return hresult;

	lpPal          = new BYTE [768];

   return DD_OK;
}

IDirectXImage::~IDirectXImage(void) {
/* Destructor
 */
	Release();
}

void IDirectXImage::Release(void) {
/* Release all memory used.
 */
	if (lpImageSurface && !g_boQuitApp) {
	// Image Surface already deleted ?
		lpImageSurface->Release();
		lpImageSurface = 0;
	}

   if (lpPal) {
      delete lpPal;
      lpPal = NULL;
   }
}

HRESULT IDirectXImage::LoadImage(LPCSTR filename,int dwWidth,int dwHeight) {
/* Load the Image
 */
	if (!lpImageSurface)
		Create(dwWidth,dwHeight);
	
	LPSTR extension;

	extension = (LPSTR) strchr(filename, '.'); // DaP

	if (!_stricmp(extension, ".PCX")) 
	// If it's a PCX file.				
		return LoadPcxImage(filename,dwWidth,dwHeight);
	
	return NULL;
}

HRESULT IDirectXImage::SetHeader(void *header) {
/* Set the Header of the Image (not very usefull for now)
 */
	if (!(lpHeader = new char [sizeof(header)]))
	// Not enought memory
		return DDERR_OUTOFMEMORY;
	memcpy(lpHeader, header, sizeof(header));
	
	delete lpHeader;
	return TRUE;
}

LPDIRECTDRAWSURFACE IDirectXImage::ImageSurface(void) {
/* Return the Image Surface Pointer.
 */
	return lpImageSurface;
}

IDirectXImage::operator LPDIRECTDRAWSURFACE (void) {
/* Return the Image Surface Pointer.
 */
	return lpImageSurface;
}

LPBYTE IDirectXImage::Palette(void) {
/* Return the Palette Entry Pointer.
 */
	return lpPal;
}