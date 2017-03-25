// NMVideoCapture.h: interface for the Global class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMVideoCapture_H__BE9CC3E6_9953_49D5_86F2_D3E2D40E7912__INCLUDED_)
#define AFX_NMVideoCapture_H__BE9CC3E6_9953_49D5_86F2_D3E2D40E7912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "Lock.h"
//#include "..\AviFileWrite\AviFileWrite.h"

class NMVideoCapture  
{
public:
	NMVideoCapture();
	virtual ~NMVideoCapture();

   void CaptureFrame();
   void StartCapture();
   void StopCapture();


protected:
   HDC         m_hDcBitmap;  // Handle sur le hDc du Bitmap en memoire
	HBITMAP     m_hBitmap;  	 // Handle sur le Bitmap
   PBITMAPINFO m_pBitmapInfo;	// Pointeur sur la structure BITMAPINFO du Bitmap
   LPBYTE      m_pBitmapData;	// Pointeur sur les Data du Bitmap

private :
   CLock lock;
   BOOL m_bCaptureInprogress;
   int m_dwNbrCnt;
   int m_dwVideoCount;


   //CAviFileWrite *m_pAviFileWrite;


};

#endif // !defined(AFX_NMVideoCapture_H__BE9CC3E6_9953_49D5_86F2_D3E2D40E7912__INCLUDED_)
