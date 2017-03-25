// NMVideoCapture.cpp: implementation of the Global class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <malloc.h>
#include <stdio.h>

#include "NMVideoCapture.h"
#include "IDirectXDraw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NMVideoCapture g_NMVideoCapture;

NMVideoCapture::NMVideoCapture()
{
   //return;
   m_hDcBitmap   = NULL;
	m_hBitmap     = NULL;
   m_pBitmapInfo = NULL;
   m_pBitmapData = NULL;

   m_dwNbrCnt     = 0;
   m_dwVideoCount = 0;

   m_bCaptureInprogress = FALSE;
}

NMVideoCapture::~NMVideoCapture()
{
   //return;
   if(m_pBitmapData)
      delete []m_pBitmapData;
   m_pBitmapData = NULL;

   if(m_pBitmapInfo)
      delete []m_pBitmapInfo;
   m_pBitmapInfo = NULL;

   if(m_hDcBitmap)
	   DeleteDC(m_hDcBitmap);
   m_hDcBitmap = NULL;
   if(m_hBitmap)
	   DeleteObject(m_hBitmap);
   m_hBitmap = NULL;
}

void NMVideoCapture::StartCapture()
{
   //return;
   if(m_bCaptureInprogress)
      return;//capture already in progress...

   m_dwNbrCnt = 0;

   char strTmp[100];
   sprintf(strTmp,"Video%03d",m_dwVideoCount);
   m_dwVideoCount++;
   CreateDirectory( strTmp, NULL );
   m_bCaptureInprogress = TRUE;
}

void NMVideoCapture::StopCapture()
{
   //return;
   if(!m_bCaptureInprogress)
      return; //no capture in progress

   
   m_bCaptureInprogress = FALSE;
}

void NMVideoCapture::CaptureFrame()
{
   //return;
   if(!m_bCaptureInprogress)
      return;
   CAutoLock autoLock( &lock );
   
   
   try 
   { 
      //recupere un frame...
      char strFileName[100];
      HDC hDcWindow;
      HANDLE hFichier;
      DWORD dwTmp;
      BITMAPFILEHEADER BitmapInfoHeader;
      DXDGetDC(&hDcWindow, 1000);
      
      // Création d'un contexte mémoire
      if(m_hDcBitmap ==NULL)
         m_hDcBitmap = CreateCompatibleDC(hDcWindow);
      if(!m_hDcBitmap)
      {
         DXDReleaseDC(hDcWindow, 1000);
         return ;
      }
      
      if(m_hBitmap == NULL)
      {
         HBITMAP hBitmap = CreateCompatibleBitmap(hDcWindow, g_Global.GetScreenW(), g_Global.GetScreenH());
         m_hBitmap = hBitmap;
         if(!hBitmap) 
         {
            DXDReleaseDC(hDcWindow, 1000);
            return ;
         }
      }
      
      //Blit le frame dans le Bitmap temporaire
      // On sélectionne ce bitmap dans le contexte mémoire
      if(!SelectObject(m_hDcBitmap, m_hBitmap)) 
      {
         DXDReleaseDC(hDcWindow, 1000);
         return ;
      }
      
      // On effectue une copie du contexte écran vers le contexte mémoire
      if(!BitBlt(m_hDcBitmap, 0, 0, g_Global.GetScreenW(), g_Global.GetScreenH(), hDcWindow, 0, 0, SRCCOPY)) 
      {
         DXDReleaseDC(hDcWindow, 1000);
         return ;
      }
      
      // Maintenant on crée la structure BITMAPINFO
      BITMAP Bitmap;  
      WORD    Couleur; 
      
      // Récupération de la structure BITMAP
      if (!GetObject(m_hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap)) 
      {
         DXDReleaseDC(hDcWindow, 1000);
         return ;
      }
      
      // Convertie le format de couleur 
      Couleur = (WORD)(Bitmap.bmPlanes * Bitmap.bmBitsPixel); 
      if (Couleur == 1) 
         Couleur = 1; 
      else if (Couleur <= 4) 
         Couleur = 4; 
      else if (Couleur <= 8) 
         Couleur = 8; 
      else if (Couleur <= 16) 
         Couleur = 16; 
      else if (Couleur <= 24) 
         Couleur = 24; 
      else 
         Couleur = 32; 
      
      
      // Alloue de la memoire pour la structure BITMAPINFO (Cette structure 
      // contient une structure BITMAPINFOHEADER et un tableau de RGBQUAD
      if (Couleur != 24) 
         m_pBitmapInfo = (PBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< Couleur)]; 
      else  // Il n'y a pas de tableau de RGBQUAD pour le format 24 bit
         m_pBitmapInfo = (PBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
      if(!m_pBitmapInfo) 
      {
         DXDReleaseDC(hDcWindow, 1000);
         return ;
      }
      
      // Initialise la structure BITMAPINFO
      
      m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
      m_pBitmapInfo->bmiHeader.biWidth = Bitmap.bmWidth; 
      m_pBitmapInfo->bmiHeader.biHeight = Bitmap.bmHeight; 
      m_pBitmapInfo->bmiHeader.biPlanes = Bitmap.bmPlanes; 
      m_pBitmapInfo->bmiHeader.biBitCount = Bitmap.bmBitsPixel; 
      if (Couleur < 24) 
         m_pBitmapInfo->bmiHeader.biClrUsed = (1<<Couleur); 
      
      // If the bitmap is not compressed, set the BI_RGB flag. 
      m_pBitmapInfo->bmiHeader.biCompression = BI_RGB; 
      m_pBitmapInfo->bmiHeader.biSizeImage = ((m_pBitmapInfo->bmiHeader.biWidth * Couleur +31) & ~31) /8 * m_pBitmapInfo->bmiHeader.biHeight; 
      
      // Set biClrImportant to 0, indicating that all of the 
      // device colors are important. 
      m_pBitmapInfo->bmiHeader.biClrImportant = 0;
      
      if(m_pBitmapData==NULL)
         m_pBitmapData = new BYTE[m_pBitmapInfo->bmiHeader.biSizeImage];
      if (!m_pBitmapData) 
      {
         DXDReleaseDC(hDcWindow, 1000);
         return ;
      }
      
      if (!GetDIBits(m_hDcBitmap, m_hBitmap, 0, (WORD) m_pBitmapInfo->bmiHeader.biHeight, m_pBitmapData, m_pBitmapInfo, DIB_RGB_COLORS)) 
      {
         DXDReleaseDC(hDcWindow, 1000);
         return ;
      }


      
      BitmapInfoHeader.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
       
      // Compute the size of the entire file. 
      BitmapInfoHeader.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) +  m_pBitmapInfo->bmiHeader.biSize +  m_pBitmapInfo->bmiHeader.biClrUsed * sizeof(RGBQUAD) +  m_pBitmapInfo->bmiHeader.biSizeImage); 
      BitmapInfoHeader.bfReserved1 = 0; 
      BitmapInfoHeader.bfReserved2 = 0; 
      
      // Compute the offset to the array of color indices. 
      BitmapInfoHeader.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +  m_pBitmapInfo->bmiHeader.biSize +  m_pBitmapInfo->bmiHeader.biClrUsed * sizeof (RGBQUAD); 

      //ecrit le fichier sur disque...
      
      // Search for a valid file name.
      sprintf(strFileName,"Video%03d\\img%05d.bmp",m_dwVideoCount-1,m_dwNbrCnt++);
         
      // Create the .BMP file. 
      hFichier = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
      
      // Copy the BITMAPFILEHEADER into the .BMP file. 
      if (!WriteFile(hFichier, &BitmapInfoHeader, sizeof(BITMAPFILEHEADER), &dwTmp,  NULL))
         return ;
      
      // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
      if (!WriteFile(hFichier, (PBITMAPINFOHEADER) m_pBitmapInfo, sizeof(BITMAPINFOHEADER) + m_pBitmapInfo->bmiHeader.biClrUsed * sizeof (RGBQUAD), &dwTmp, NULL)) 
         return ; 
      
      // Copy the array of color indices into the .BMP file. 
      if (!WriteFile(hFichier, (LPSTR) m_pBitmapData, (int) m_pBitmapInfo->bmiHeader.biSizeImage, &dwTmp, NULL)) 
         return ; 
      
      // Close the .BMP file. 
      if (!CloseHandle(hFichier)) 
         return ; 


      
      DXDReleaseDC(hDcWindow, 1000);
   }catch (...) {}
}