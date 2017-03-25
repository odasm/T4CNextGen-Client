// VideoCapture.cpp: implementation of the VideoCapture class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <windows.h>
#include "VideoCapture.h"
#include "IDirectXDraw.h"
#include "PowerDebug.h"
#include "MacroHandler.h"

//DC #include <Stdio.h>
//DC #include <Stdio.h>
//DC #include <Objbase.h>
//DC #include <Gdiplus.h>
//DC using namespace Gdiplus;

//DC // Helper functions
//DC int GetCodecClsid(const WCHAR*, CLSID*);


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
typedef struct BITMAPFILEHEADER
{
   UINT16    type;
   UINT32    size;
   INT16     xHotspot;
   INT16     yHotspot;
   UINT32    offsetToBits;
}  BITMAPFILEHEADER;
Figure 2: BITMAPARRAYHEADER structure.
typedef struct BITMAPARRAYHEADER
{
   UINT16    type;
   UINT32    size;
   UINT32    next;
   UINT16    screenWidth;
   UINT16    screenHeight;
} BITMAPARRAYHEADER;
Figure 3: BITMAPHEADER structure.
typedef struct BITMAPHEADER
{
    UINT32 size;
    INT32  width;
    INT32  height;
    UINT16 numBitPlanes;
    UINT16 numBitsPerPlane;
    UINT32 compressionScheme;
    UINT32 sizeOfImageData;
    UINT32 xResolution;
    UINT32 yResolution;
    UINT32 numColorsUsed;
    UINT32 numImportantColors;
    UINT16 resolutionUnits;
    UINT16 padding;
    UINT16 origin;
    UINT16 halftoning;
    UINT32 halftoningParam1;
    UINT32 halftoningParam2;
    UINT32 colorEncoding;
    UINT32 identifier;
} BITMAPHEADER;
Figure 4: RGB structure.
typedef struct RGB
{
  UINT8 blue;
  UINT8 green;
  UINT8 red;
} RGB;
*/

VideoCapture::VideoCapture()
{
	capture = false;

	// Initialisation des données
	m_pBitmapInfoHeader = NULL;
	m_pBitmapInfo = NULL;
	m_pBitmapData = NULL;

	m_hDcBitmap = NULL;
	m_hBitmap = NULL;
}

VideoCapture::~VideoCapture()
{
   if(m_pBitmapInfo)
	   delete[] m_pBitmapInfo;
   m_pBitmapInfo = NULL;

   if(m_pBitmapData)
	   delete[] m_pBitmapData;
   m_pBitmapData = NULL;

	// Free memory.
	DeleteDC(m_hDcBitmap);
	DeleteObject(m_hBitmap);
}

bool VideoCapture::TakeDesktopSnapshot(void)
{
	// Récupération du HWND et du HDC de la fenêtre actuel
	
	HDC hDcWindow;
	lpDXDirectDraw->GetDC(&hDcWindow, NULL);

	// Création d'un contexte mémoire
	m_hDcBitmap = CreateCompatibleDC(hDcWindow);
	if(!m_hDcBitmap) return 0;

	// On crée un bitmap en mémoire ayant les dimensions du bureau
	HBITMAP hBitmap = CreateCompatibleBitmap(hDcWindow, g_Global.GetScreenW(), g_Global.GetScreenH());
	if(!hBitmap) return 0;

	m_hBitmap = hBitmap;

	// On sélectionne ce bitmap dans le contexte mémoire
	if(!SelectObject(m_hDcBitmap, hBitmap)) return 0;

	// On effectue une copie du contexte écran vers le contexte mémoire
	if(!BitBlt(m_hDcBitmap, 0, 0, g_Global.GetScreenW(), g_Global.GetScreenH(), hDcWindow, 0, 0, SRCCOPY)) return 0;

	// Maintenant on crée la structure BITMAPINFO
	BITMAP Bitmap;  
    WORD    Couleur; 

    // Récupération de la structure BITMAP
    if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap)) return 0;

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
    else Couleur = 32; 

	// Alloue de la memoire pour la structure BITMAPINFO (Cette structure 
	// contient une structure BITMAPINFOHEADER et un tableau de RGBQUAD
    if (Couleur != 24) 
		m_pBitmapInfo = (PBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< Couleur)]; 

    // Il n'y a pas de tableau de RGBQUAD pour le format 24 bit

    else 
		m_pBitmapInfo = (PBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];

	if(!m_pBitmapInfo) return 0;

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

	//initialise le reste des structures... 

	m_pBitmapInfoHeader = (PBITMAPINFOHEADER) m_pBitmapInfo; 

	m_pBitmapData = new BYTE[m_pBitmapInfoHeader->biSizeImage];
    if (!m_pBitmapData) return 0; 

    // Retrieve the color table (RGBQUAD array) and the bits 
    // (array of palette indices) from the DIB. 
    if (!GetDIBits(m_hDcBitmap, hBitmap, 0, (WORD) m_pBitmapInfoHeader->biHeight, m_pBitmapData, m_pBitmapInfo, DIB_RGB_COLORS)) return 0;

	m_BitmapFileHeader.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 

    // Compute the size of the entire file. 
    m_BitmapFileHeader.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + m_pBitmapInfoHeader->biSize + m_pBitmapInfoHeader->biClrUsed * sizeof(RGBQUAD) + m_pBitmapInfoHeader->biSizeImage); 
    m_BitmapFileHeader.bfReserved1 = 0; 
    m_BitmapFileHeader.bfReserved2 = 0; 

    // Compute the offset to the array of color indices. 
    m_BitmapFileHeader.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + m_pBitmapInfoHeader->biSize + m_pBitmapInfoHeader->biClrUsed * sizeof (RGBQUAD); 

	lpDXDirectDraw->ReleaseDC(hDcWindow, NULL);
	return 1;
}

bool VideoCapture::SaveToFile(const char* Path)
{

	HANDLE hFichier;
    DWORD dwTmp, screenId = 1; 
	char fileName[ 1024 ];
	
	HANDLE hFile;
    // Search for a valid file name.
    do{
        sprintf( fileName, "%s\\img%03u.bmp", Path, screenId );

        hFile = CreateFile(
            fileName,
            0,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );
        if( hFile != INVALID_HANDLE_VALUE ){
            CloseHandle( hFile );
        }
        screenId++;
    }while( hFile != INVALID_HANDLE_VALUE );

    // Create the .BMP file. 
	hFichier = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

    // Copy the BITMAPFILEHEADER into the .BMP file. 
    if (!WriteFile(hFichier, &m_BitmapFileHeader, sizeof(BITMAPFILEHEADER), &dwTmp,  NULL)) return 0;

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
    if (!WriteFile(hFichier, m_pBitmapInfoHeader, sizeof(BITMAPINFOHEADER) + m_pBitmapInfoHeader->biClrUsed * sizeof (RGBQUAD), &dwTmp, NULL)) return 0; 

    // Copy the array of color indices into the .BMP file. 
    if (!WriteFile(hFichier, (LPSTR) m_pBitmapData, (int) m_pBitmapInfoHeader->biSizeImage, &dwTmp, NULL)) return 0; 

    // Close the .BMP file. 
    if (!CloseHandle(hFichier)) 
       return 0; 

	return 1;

}

int VideoCapture::GetWidth()
{
	return m_pBitmapInfo->bmiHeader.biWidth;
}

int VideoCapture::GetHeight()
{
	return m_pBitmapInfo->bmiHeader.biHeight;
}


//////////////////////////////////////////////////////////////////////////////////////////
void VideoCapture::Start( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &lock );
    capture = true;
}
//////////////////////////////////////////////////////////////////////////////////////////
void VideoCapture::Stop( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &lock );
    capture = false;
}
//////////////////////////////////////////////////////////////////////////////////////////
void VideoCapture::Capture( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Captures the screen.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &lock );

    if( !capture ){
        return;
    }
	try {
		DDSURFACEDESC surfaceDesc;
      ZeroMemory(&surfaceDesc, sizeof(surfaceDesc)); 
		surfaceDesc.dwSize = sizeof( DDSURFACEDESC );
		LPDIRECTDRAWSURFACE backBuffer = lpDXDirectDraw->PicklockBackBuffer( NULL, &surfaceDesc, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL );
		if( backBuffer == NULL ){
			return;
		}

		Frame frame;
		frame.buffer = new WORD[ surfaceDesc.dwWidth * surfaceDesc.dwHeight ];
		frame.width = surfaceDesc.dwWidth;
		frame.height = surfaceDesc.dwHeight;
		frame.rBitMask = surfaceDesc.ddpfPixelFormat.dwRBitMask;
		frame.gBitMask = surfaceDesc.ddpfPixelFormat.dwGBitMask;
		frame.bBitMask = surfaceDesc.ddpfPixelFormat.dwBBitMask;

		// Copy each line of the buffer.
		DWORD y;
		for( y = 0; y < surfaceDesc.dwHeight; y++ ){
			memcpy( 
				(LPBYTE)(frame.buffer) + ( ( surfaceDesc.dwHeight - y ) * surfaceDesc.dwWidth * 2 ),
				(LPBYTE)surfaceDesc.lpSurface + (y * surfaceDesc.lPitch ), 
				surfaceDesc.dwWidth * 2 
			);
		}
    

		backBuffer->Unlock( surfaceDesc.lpSurface );
    
		// Add the frame to the frame list.
		frameList.push_back( frame );
	} catch (...) {}
}

struct color{
    union{
      WORD r : 5,
           g : 6,
           b : 5;
      WORD word;
    } u;
};


WORD GetNumberOfBits( DWORD dwMask )
{
    WORD wBits = 0;
    while( dwMask )
    {
        dwMask = dwMask & ( dwMask - 1 );  
        wBits++;
    }
    return wBits;
}


DWORD rBits;
DWORD gBits;
DWORD bBits;

WORD Inverse( WORD d ){
    DWORD n;
    ((BYTE *)( &n ))[ 0 ] = ((BYTE *)( &d ))[ 1 ];
    ((BYTE *)( &n ))[ 1 ] = ((BYTE *)( &d ))[ 0 ];
    return (WORD)n;
}

WORD FromFrameTo555( DWORD rMask, DWORD gMask, DWORD bMask, DWORD w ){
    WORD r = (WORD)(w & rMask); // b
    WORD g = (WORD)(w & gMask); // r
    WORD b = (WORD)(w & bMask); // g
    
    if( gBits == 6 ){
        r = r >> 1;

        g = g >> 5;
        g = g * ( 1 << 5 ) / ( 1 << gBits );
        g = g << 5;    
    }

    return( r | g | b );
}

//////////////////////////////////////////////////////////////////////////////////////////
void VideoCapture::CommitFrames
//////////////////////////////////////////////////////////////////////////////////////////
// Commites the frames on disk.
// 
(
 std::string directory  // Directory to store the frames to.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &lock );
    
    DWORD screenId = 1;

    char fileName[ 1024 ];

    std::list< Frame >::iterator i;
    for( i = frameList.begin(); i != frameList.end(); i++ ){
        Frame &thisFrame = *i;

        HANDLE hFile;
        // Search for a valid file name.
        do{
            sprintf( fileName, "%s\\img%03u.bmp", directory.c_str(), screenId );

            hFile = CreateFile(
                fileName,
                0,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                0,
                NULL
            );
            if( hFile != INVALID_HANDLE_VALUE ){
                CloseHandle( hFile );
            }
            screenId++;
        }while( hFile != INVALID_HANDLE_VALUE );

        rBits = GetNumberOfBits( thisFrame.rBitMask );
        gBits = GetNumberOfBits( thisFrame.gBitMask );
        bBits = GetNumberOfBits( thisFrame.bBitMask );

        LOG.Lock();
        LOG << "\r\nCAPTURE SAVE: R=" << rBits << " G=" << gBits << " B=" << bBits << "..";
        LOG << "\r\nCAPTURE SAVE: Rm=" << thisFrame.rBitMask << " Gm=" << thisFrame.gBitMask << " Bm=" << thisFrame.bBitMask << "..";
        LOG.Unlock();


        BITMAPFILEHEADER bitMapHeader;        
        bitMapHeader.bfReserved1 = 0;
        bitMapHeader.bfReserved2 = 0;
        bitMapHeader.bfSize = sizeof( BITMAPFILEHEADER ) + 
            sizeof( BITMAPINFOHEADER ) + thisFrame.width * thisFrame.height * 2;
        bitMapHeader.bfType = 0x4D42;
        bitMapHeader.bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + 4;

        BITMAPINFOHEADER bitMapInfo;
        bitMapInfo.biSize = sizeof( BITMAPINFOHEADER );
        bitMapInfo.biWidth = thisFrame.width;
        bitMapInfo.biHeight = thisFrame.height;
        bitMapInfo.biPlanes = 1;
        bitMapInfo.biBitCount = 16;
        bitMapInfo.biCompression = BI_RGB;
        bitMapInfo.biSizeImage = thisFrame.width * thisFrame.height * 2;
        bitMapInfo.biXPelsPerMeter = thisFrame.width;
        bitMapInfo.biYPelsPerMeter = thisFrame.height;
        bitMapInfo.biClrUsed = 1;
        bitMapInfo.biClrImportant = 1;

        hFile = CreateFile( fileName,
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            NULL,
            CREATE_NEW,
            0,
            NULL
        );
        if( hFile == INVALID_HANDLE_VALUE ){
            continue;
        }

        DWORD i;
        for( i = 0; i < thisFrame.width * thisFrame.height; i ++ ){
            thisFrame.buffer[ i ] = FromFrameTo555( thisFrame.rBitMask, thisFrame.gBitMask, thisFrame.bBitMask, thisFrame.buffer[ i ] );
        }

        DWORD dummy = 0;
        WriteFile( hFile, &bitMapHeader, sizeof( BITMAPFILEHEADER ), &dummy, NULL );
        WriteFile( hFile, &bitMapInfo, sizeof( bitMapInfo ), &dummy, NULL );
        WriteFile( hFile, thisFrame.buffer, thisFrame.width * thisFrame.height * 2, &dummy, NULL );
//DC

//DC
        CloseHandle( hFile );

        delete thisFrame.buffer;
    }
    frameList.clear();
}


VideoCapture *VideoCapture::GetInstance(){
    static VideoCapture instance;
    return &instance;
}