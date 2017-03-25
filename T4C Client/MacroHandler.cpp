// Include Files.
#include "pch.h"
#include <windows.h>
#include "MacroHandler.h"
#include "PowerDebug.h"
#include "Global.h"
#include "MemoryManager.h"


UserCustom Custom;

static BOOL boMacroEnabled = TRUE;
extern BOOL DoNotMove;

//*******************************************************************************************//
MacroHandler::MacroHandler( void )
//*******************************************************************************************//
// Default Constructor.
// 
{
   Custom.g_pNewCCColor = NULL;
}

//*******************************************************************************************//
MacroHandler::~MacroHandler( void )
//*******************************************************************************************//
// Destructor
// 
{
	HandlerDesc *hd;

	ToHead();
	while(QueryNext()) {
		hd = GetObject();
      RemoveObject();
      delete hd;
	}
   if(Custom.g_pNewCCColor)
      delete []Custom.g_pNewCCColor;
   Custom.g_pNewCCColor = NULL;
}


void MacroHandler::DeleteMacro( VKey vKey ){
	HandlerDesc *hd;

    Lock(0);
    ToHead();
    while( QueryNext() ){
        hd = GetObject();
        if (hd->vKey == vKey) {
            DeleteObject();
			SetQueryState(STOP_QUERY);
		}
    }
    Unlock(0);
}

void MacroHandler::AddNewMacro( VKey vKey, UINT (* CallBack)(LPVOID), LPVOID lpParam) {
    // Delete any previous key binded to the new macro call
    DeleteMacro( vKey );

    // Create a new handler desc for this macro.
    HandlerDesc *hd = new HandlerDesc;
    
    // Put the Key.
	hd->vKey = vKey;
	// Call back
	hd->CallBack = CallBack;
	// Param Pointer
	hd->lpParam = lpParam;

	Lock(0);
    AddToTail(hd);
    Unlock(0);
}

BOOL MacroHandler::MacroExists( VKey vKey ){
    BOOL boFound = FALSE;

    HandlerDesc *hd;

    Lock(0);
    ToHead();
    while( QueryNext() ){
        hd = GetObject();
        if (hd->vKey == vKey) {
            boFound = TRUE;
			SetQueryState(STOP_QUERY);
		}
    }
    Unlock(0);
    return boFound;
}

//*******************************************************************************************//
BOOL MacroHandler::CallMacro
//*******************************************************************************************//
// Set CallBack Function
// 
(
  VKey vKey
)
//*******************************************************************************************//
{
    if( !boMacroEnabled ){
        LOG << "\r\nMacro call disabled!!";
        return FALSE;
    }

	HandlerDesc *hd;

    BOOL boFound = FALSE;

	Lock(0);

    if( GetNbObjects() == 0 ){
        LOG << "\r\nThere are no available macros!";
    }

    ToHead();
	while(QueryNext()) {
		hd = GetObject();
        if (hd->vKey == vKey && !DoNotMove) {
            LOG.Lock();  
            LOG << "\r\nMacro called! Address=" << (DWORD)hd->CallBack << ";";
            LOG.Unlock();

			hd->CallBack(hd->lpParam); 

            boFound = TRUE;
            break;
		}
	}
    Unlock(0);

    return boFound;
}

void MacroHandler::EnableMacroCall(){
    boMacroEnabled = TRUE;
}

void MacroHandler::DisableMacroCall(){
    boMacroEnabled = FALSE;
}