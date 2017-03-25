// ************************************************************************
// ***                                                                  ***
//      File Name: IDirectXPcx.h
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

#ifndef __DIRECTXPCX_H
#define __DIRECTXPCX_H

#include <ddraw.h>

struct PcxHeader {
// PCX Header	
	private:
      /*
		char manufacturer;		// the manufacturer of the file
		char version;				// the file format version
		char encoding;				// type of compression
		char bits_per_pixel;		// number of bits per pixel
		int  x, y;					// starting location of image
		int  width, height;		// size of image
		int  horz_res;				// resolution in DPI (dots per inch)
		int  horz_ver;			   //
		char ega_palette[48];	// the old EGA palette
		char reserved;				// don't care
		char num_color_planes;	// number of color planes
		int  bytes_per_line;		// number of bytes per line of the image
		int  palette_type;		// 1 for color, 2 for grey scale palette
		char padding[58];			// extra bytes
      */
      public:
         BYTE	manufacturer;      /* PCX Id Number (Always 0x0A) */
         BYTE	version;           /* Version Number */
         BYTE	encoding;          /* Encoding Format */
         BYTE	bits_per_pixel;    /* Bits per Pixel */
         WORD	x;                 /* Left of image */
         WORD	y;                 /* Top of Image */
         WORD	width;            /* Right of Image*/
         WORD	height;            /* Bottom of image */
         WORD	horz_res;           /* Horizontal Resolution */
         WORD	horz_ver;           /* Vertical Resolution */
         BYTE	ega_palette[48];       /* 16-Color EGA Palette */
         BYTE	reserved;         /* Reserved (Always 0) */
         BYTE	num_color_planes;      /* Number of Bit Planes */
         WORD	bytes_per_line;      /* Bytes per Scan-line */
         WORD	palette_type;       /* Palette Type */
         WORD	HorzScreenSize;    /* Horizontal Screen Size */
         WORD	VertScreenSize;    /* Vertical Screen Size */
         BYTE	padding[54];     /* Reserved (Always 0) */
};

typedef PcxHeader FAR *LPPCXHEADER;

class IDirectXPcx {
// PCX Class
	private:
		virtual HRESULT SetHeader(void *header) = 0;         // Image Header
		virtual LPDIRECTDRAWSURFACE ImageSurface(void) = 0;  // Image Surface
		virtual LPBYTE Palette(void) = 0;				       // Image Palette
 
		HRESULT LoadHeader(HFILE &);  // Load the Header
		HRESULT LoadImage(HFILE &,int dwWidth,int dwHeight);   // Load the Image
		HRESULT LoadPalette(HFILE &); // Load the Palette
			
	public:
		HRESULT LoadPcxImage(LPCSTR,int,int); // Load the PCX Image

};

typedef IDirectXPcx FAR *LPDIRECTXPCX;

#endif