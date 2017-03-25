// ************************************************************************
// ***                                                                  ***
//      File Name: IDirectXPcx.Cpp
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
//         Provide fonction to load a PCX file.
// ***                                                                  ***
//	************************************************************************
// *** Copyright (c) 1997-1998 Vircom. All rights reserved.             ***

#include "pch.h"
#include <windows.h>
#include "IDirectXDraw.H"
#include "IDirectXPcx.H"
#include "Global.h"
#include "MemoryManager.h"
#include "PowerDebug.h"

extern Global g_Global;

HRESULT IDirectXPcx::LoadPcxImage(LPCSTR filename,int dwWidth,int dwHeight) {
/* This function loads a PCX file into the image structure. The function
 * has three main parts: 1. load the PCX header, 2. load the image data and
 * decompress it and 3. load the palette data.
 */
	HFILE          hfile;		 // File pointer for the PCX file
	HRESULT			hresult;

	if ((hfile = _lopen(filename, OF_READ)) == HFILE_ERROR) {
	// Can't open the file.
		//return DDERR_NOTFOUND;
		LOG << "* DBG = Cannot open file " << (char *)filename << "\r\n";
		hfile = NULL;
      return NULL;
	}
		
	if (!(hresult = LoadPalette(hfile)))
	// Can't load the palette.
		return NULL;

	if (!(hresult = LoadHeader(hfile)))
	// Can't load the header.
		return NULL;
	
	if (!(hresult = LoadImage(hfile,dwWidth,dwHeight)))
	// Can't load the image.
		return NULL;

	_lclose(hfile);
	Sleep(100);

	return TRUE;
}

HRESULT IDirectXPcx::LoadHeader(HFILE &hfile) {
/* Load the PCX Header.
 */
	if (!hfile)
		return NULL;
	PcxHeader     header;      // PCX Header

	_llseek(hfile, 0, FILE_BEGIN);
   _lread(hfile, &header, 128);
	return SetHeader(&header);
}

HRESULT IDirectXPcx::LoadImage(HFILE &hfile,int dwWidth,int dwHeight) {
/* Decompress the PCX file.
 */
	WORD *ImageBuffer;         // Image Buffer
	int            Index       = 0;     // Number of bytes decompressed
   int            IndexX      = 0;     // Number of bytes decompressed
   int            IndexY      = 0;     // Number of bytes decompressed
   int            dwCenterOffset = 0;
	int			   LoadCounter = 0;     // Number of bytes before reloading
	int            nBlock      = 1000000; // Number of bytes in a Block
	unsigned char  Pixel       = 0;     // Pixel data
   WORD           wColor = 0;
	unsigned char  nBytes;              // Number of Bytes in current RLE run
	unsigned char *BlockBuffer;         // Block Buffer
														
	if (!(BlockBuffer = new unsigned char [nBlock]))
	// Not enough memory
		return DDERR_OUTOFMEMORY;

   PcxHeader     header;      // PCX Header

	_llseek(hfile, 0, FILE_BEGIN);
   _lread(hfile, &header, 128);


	// Lock the Image Surface
	DDSURFACEDESC desc;
	memset(&desc, 0, sizeof(desc));
	desc.dwSize = sizeof(desc);

	int x = ImageSurface()->Lock(NULL, &desc, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,
			                 NULL);

	ImageBuffer = (WORD *)desc.lpSurface;
	long lPitch      = desc.lPitch;
   /*int toto = sizeof(ImageBuffer);*///BLBLBL 29 mars 2009 suppr. lignes inutilisée
	
	if (!hfile) 
   {
      memset(ImageBuffer, 0, dwWidth*dwHeight*2/*g_Global.GetDisplayW()*g_Global.GetDisplayH()*2*/);
	} 
   else 
   {
      int dwOffSetYTmp = 0;
             
		// Loop and decompress the PCX.
		while ((Index < dwWidth*dwHeight/*g_Global.GetDisplayW()*g_Global.GetDisplayH()*/) && IndexY <= header.height)
      {
			if (!(LoadCounter % nBlock))
			// Need to load ?
				_lread(hfile, BlockBuffer, nBlock);
			Pixel = BlockBuffer[LoadCounter % nBlock];
         LoadCounter++;
		
			if (Pixel > 192) 
         {
			// RLE compression.
				nBytes = Pixel - 192;
				if (!(LoadCounter % nBlock))
				// Need to load ?
					_lread(hfile, BlockBuffer, nBlock);
				Pixel = BlockBuffer[LoadCounter % nBlock];
            LoadCounter++;
			
				while (nBytes) 
            {
				// Replicate data in buffer num_bytes times.
					nBytes--;
               if (Index < dwWidth*dwHeight/*g_Global.GetDisplayW()*g_Global.GetDisplayH()*/) 
               {
                  wColor = ((Palette()[(Pixel*3)] >> lpDXDirectDraw->bRDown) << lpDXDirectDraw->bRUp) + ((Palette()[(Pixel*3)+1] >> lpDXDirectDraw->bGDown) << lpDXDirectDraw->bGUp) + ((Palette()[(Pixel*3)+2] >> lpDXDirectDraw->bBDown) << lpDXDirectDraw->bBUp);
                  ImageBuffer[Index] = wColor;
               }
               Index++;
               IndexX++;
               if(IndexX >=header.bytes_per_line)
               {
                  IndexX = 0;
                  IndexY ++;
                  Index = (IndexY+dwOffSetYTmp)*desc.dwWidth;
                  Index +=dwCenterOffset;
               }
				}
			}
			else 
         {
			// Not a RLE compression
            if (Index < dwWidth*dwHeight/*g_Global.GetDisplayW()*g_Global.GetDisplayH()*/) 
            {
               wColor = ((Palette()[(Pixel*3)] >> lpDXDirectDraw->bRDown) << lpDXDirectDraw->bRUp) + ((Palette()[(Pixel*3)+1] >> lpDXDirectDraw->bGDown) << lpDXDirectDraw->bGUp) + ((Palette()[(Pixel*3)+2] >> lpDXDirectDraw->bBDown) << lpDXDirectDraw->bBUp);
   				ImageBuffer[Index] = wColor;
            }
            Index++;
            IndexX++;
            if(IndexX >=header.bytes_per_line)
            {
               IndexX = 0;
               IndexY ++;
               Index = (IndexY+dwOffSetYTmp)*desc.dwWidth;
               Index +=dwCenterOffset;
            }
			}
      }
	}


	delete BlockBuffer;
	ImageSurface()->Unlock(desc.lpSurface);

	return TRUE;
}

HRESULT IDirectXPcx::LoadPalette(HFILE &hfile) {
/* Load the Palette
 */
	_llseek(hfile, -768l, FILE_END);
   _lread(hfile, Palette(), 768);

	return TRUE;
}