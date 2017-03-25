// VideoCapture.h: interface for the VideoCapture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIDEOCAPTURE_H__3020E7D3_2B3F_11D4_B59E_00E0296B9056__INCLUDED_)
#define AFX_VIDEOCAPTURE_H__3020E7D3_2B3F_11D4_B59E_00E0296B9056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include <string>
#include "Lock.h"

class VideoCapture  
{
public:	
   VideoCapture();
   virtual ~VideoCapture();
   
   void Start();
   void Stop();
   void Capture();
   
   void CommitFrames( std::string directory );
   
   static VideoCapture *GetInstance();
   
   BITMAPFILEHEADER m_BitmapFileHeader;	// Données de la structure BITMAPFILEHEADER du Bitmap
   PBITMAPINFOHEADER m_pBitmapInfoHeader;	// Pointeur sur la structure BITMAPINFOHEADER du Bitmap
   PBITMAPINFO m_pBitmapInfo;	// Pointeur sur la structure BITMAPINFO du Bitmap
   LPBYTE m_pBitmapData;	// Pointeur sur les Data du Bitmap
   
   HDC m_hDcBitmap;	// Handle sur le hDc du Bitmap en memoire
   HBITMAP m_hBitmap;	// Handle sur le Bitmap
   
   bool TakeDesktopSnapshot(void);		// Prend un ScreenShot du bureau, return false si échec
   bool SaveToFile(const char* Path);	// Save le fichier dans un répertoire, return false si échec
   
   int GetWidth();
   int GetHeight();
   
private:
   
   class Frame{
   public:
      unsigned short *buffer;
      unsigned long width, height;
      DWORD rBitMask;
      DWORD gBitMask;
      DWORD bBitMask;
   };
   
   bool capture;
   std::list< Frame > frameList;
   
   CLock lock;
};

#endif // !defined(AFX_VIDEOCAPTURE_H__3020E7D3_2B3F_11D4_B59E_00E0296B9056__INCLUDED_)
