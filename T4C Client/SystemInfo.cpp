//
//
//



// Include Files.
#include "pch.h"
#include <windows.h>
#include "SystemInfo.h"
#include "Global.h"
#include "MemoryManager.h"

SystemInfo  *ug_siApp_22486 = NULL;

//********************************************************//
SystemInfo::SystemInfo( void )
//********************************************************//
// Default Constructor.
// 
{
	hInstance = NULL;
    hMainWndHook = 0;
}

//********************************************************//
SystemInfo::~SystemInfo( void )
//********************************************************//
// Destructor
// 
{
}

//********************************************************//
void SystemInfo::SethInstance
//********************************************************//
// Set le Windows Instance.
// 
(
 HINSTANCE hI // Windows Instance
) 
{
	hInstance = hI;
}

//********************************************************//
HINSTANCE SystemInfo::GethInstance( void )
//********************************************************//
// Get le Windows Instance.
// Return:
//		Windows hInstance.
//
{
	return hInstance;
}

//********************************************************//
void SystemInfo::SethWnd
//********************************************************//
// Set le Window Handle
// 
(
 HWND hw // Window Handle
) 
{
	hWnd = hw;
}

//********************************************************//
void SystemInfo::SetSystemOS ( void )
//********************************************************//
// Set le Window Handle
// 
{

   LPOSVERSIONINFO osInfo = new OSVERSIONINFO;

   osInfo->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   
   GetVersionEx(osInfo);

   if (osInfo->dwPlatformId == VER_PLATFORM_WIN32_NT) {
      OpSystem = SI_WIN_NT_40;
   } else {
      OpSystem = SI_WIN_95;
   }

	delete osInfo;
}

//********************************************************//
BYTE SystemInfo::GetSystemOS ( void )
//********************************************************//
// Set le Window Handle
// 
{
   return OpSystem;
}


//********************************************************//
HWND SystemInfo::GethWnd( void )
//********************************************************//
// Get le Window Handle.
// Return:
//    Window Handle.
//
{
	return hWnd;
}


//////////////////////////////////////////////////////////////////////////////////////////
void SystemInfo::SetMainWndThreadId
//////////////////////////////////////////////////////////////////////////////////////////
// Sets the main windows' thread id.
// 
(
 DWORD dw // The id.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    mainWndThreadId = dw;
}
DWORD SystemInfo::GetMainWndThreadId(){ 
    return mainWndThreadId;
}

void SystemInfo::SetMainWndHook( HHOOK hHook ){
    hMainWndHook = hHook;
}
HHOOK SystemInfo::GetMainWndHook(){
    return hMainWndHook;
}
