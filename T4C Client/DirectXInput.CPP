// ************************************************************************
// ***                                                                  ***
//      File Name: DirectXInput.Cpp
//      Project:   TFC Client
//      Creation:  15 July, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         07-15-1997      1.0     TH          Initial development
//
//      Description
//         Provide an Interface to interract with the mouse.
// ***                                                                  ***
//	************************************************************************
// *** Copyright (c) 1997-1998 Vircom. All rights reserved.             ***

// QUESTION:
// Est-ce possible que quand je call DEVICECREATE qu'il me retourne ok
// et que le device soit pas attacher ?

// UPDATE INFORMATION:
// Work with IDirectXDraw Version 1.0

#include "pch.h"
#include <windows.h>
#include "DirectXInput.h"
#include "PowerDebug.h"
#include "WarningBox.h"
#include "Bitmap.h"
#include "SystemInfo.h"
#include "IDirectXFont.h"
#include "Global.h"
#include "MemoryManager.h"
#include "MacroHandler.h"
#include "LocalString.h"
#include "App.h"

extern int NBCURSOR;
extern Global g_Global;

void Clip(RECT *R, RECT *S = NULL) {
	if (R->right > g_Global.GetScreenW())  {
		if (S) 
			S->right = S->right - (R->right - g_Global.GetScreenW());
		R->right = g_Global.GetScreenW();
	}
	if (R->bottom > g_Global.GetScreenH()) {
		if (S)
			S->bottom = S->bottom - (R->bottom - g_Global.GetScreenH());
		R->bottom = g_Global.GetScreenH();
	}
}

void Clip2(RECT *R, RECT *S = NULL) {
	S->right = (R->right - R->left)+S->left;
	S->bottom = (R->bottom - R->top)+S->top;

}

/////////////////////////////////////////////////////////////////////////////
DirectXInput::DirectXInput(void) {
// Constructor
	lpdi              = NULL;
	lpDIDevice        = NULL;
//   lplpCursor = NULL;
	BackgroundSurface = NULL;
   VirtualGrid       = NULL;
	Device            = NODEVICE;
	nbCursor          = 0;
   ScreenLeft        = 0;
   ScreenTop         = 0;
   ScreenRight       = 0;
   ScreenBottom      = 0;
	Rect.left         = 0;
	Rect.top          = 0;
	Rect.right        = 0;
	Rect.bottom       = 0;
	Width             = 0;
	Height            = 0;
	ZeroMemory(&g_Status, sizeof(g_Status));
   DeviceWasRelease = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
DirectXInput::~DirectXInput(void) {
// Destructor
//	delete lplpCursor;
//	ReleaseDevice();
//	Release();
}

void DirectXInput::GetRelease(int *b) {
	int x, y, bu;
	
	GetStatus(&x, &y, &bu);
	if (*b)
		if (*b != bu)
			*b = 0;
}

BOOL DirectXInput::IsCreated(void) {
   return (lpdi != NULL);
}

/////////////////////////////////////////////////////////////////////////////
void DirectXInput::Create(void) {
// Initialize the DirectX Input Environment.
   DeviceWasRelease = FALSE;

   HRESULT hresult; // Result

/*#ifdef _DEBUG
	// Verify that Direct Input Interface was previously created.
	if (lpdi) {
	// Direct Input Interface was already created
		TRACE("*** DirectXInput Report:\r\n");
		TRACE("    Create was call twice.\r\n");
		TRACE("    PowerDebug -> Reject the second Create call.\r\n");
		return;
	}
#endif*/

	// Create Direct Input Interface.
	hresult = DirectInputCreate(siGethInstance(), DIRECTINPUT_VERSION, &lpdi, NULL);
	if (hresult != DI_OK)
		Exception(hresult, "Create => DirectInputCreate\r\nDirect Input Version", DIRECTINPUT_VERSION);
}

/////////////////////////////////////////////////////////////////////////////
void DirectXInput::CreateDevice(REFGUID GUID_Device) {
// Initialize a Direct Input Device
	HRESULT hresult; // Result

#ifdef _DEBUG
	// Verify that Direct Input Interface was previously created.
	if (!lpdi) {
	// No Direct Input Interface
		// WARNINGBOX
/*		TRACE("*** DirectXInput Report:\r\n");
		TRACE("    CreateDevice was call before Create.\r\n");
		TRACE("    Cannot attach a Device to a non-created interface.\r\n");
		TRACE("    PowerDebug -> Call Create before calling CreateDevice.\r\n");
*/		Create();
	}
#endif

	// Create a Device
	hresult = lpdi->CreateDevice(GUID_Device, &lpDIDevice, NULL);
	if (hresult != DI_OK)
		Exception(hresult, "CreateDevice -> CreateDevice");

	// Get the Device Status
	hresult = lpdi->GetDeviceStatus(GUID_Device);
	if (hresult != DI_OK)
		Exception(hresult, "CreateDevice -> GetDeviceStatus");

	// Set Cooperative Level, will work with both Win95 an WinNT
	if (GUID_Device == GUID_SysMouse) {
		hresult = lpDIDevice->SetCooperativeLevel(siGethWnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	} else if (GUID_Device == GUID_SysKeyboard) {
		hresult = lpDIDevice->SetCooperativeLevel(siGethWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	} else
		hresult = lpDIDevice->SetCooperativeLevel(siGethWnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	if (hresult != DI_OK)
		Exception(hresult, "CreateDevice -> SetCooperativeLevel");

	// Set the Data Format.
	if (GUID_Device == GUID_SysMouse) {
	// Device is a Mouse.
		hresult = lpDIDevice->SetDataFormat(&c_dfDIMouse);
	} else if (GUID_Device == GUID_SysKeyboard)  {
	// Device is a Keyboard.
		hresult = lpDIDevice->SetDataFormat(&c_dfDIKeyboard);
	// } else  {
	// Device is something else.
	//	hresult = lpDIDevice->SetDataFormat(LPCDIDATAFORMAT lpdf);
	}

	if (hresult != DI_OK)
		Exception(hresult, "Create Device-> SetDataFormat");

   if (GUID_Device == GUID_SysMouse)
		Device = MOUSE;
	else if (GUID_Device == GUID_SysKeyboard)
		Device = KEYBOARD;
	else 
		Device = UNKNOWN;
}

void DirectXInput::Unacquire(void) {
	if (lpDIDevice)
   	lpDIDevice->Unacquire();
}

void DirectXInput::Acquire(void) 
{
   if (lpDIDevice)
     lpDIDevice->Acquire();
}


/////////////////////////////////////////////////////////////////////////////
void DirectXInput::ReleaseDevice(void) {
// Close a Direct Input Device

	HRESULT hresult; // Result
   
	if (lpDIDevice) {
	// If Device wasn't released
		hresult = lpDIDevice->Unacquire();
		hresult = DI_OK;
		if (hresult != DI_OK)
			Exception(hresult, "ReleaseDevice -> Unacquire", -1);
		lpDIDevice->Release();
		lpDIDevice = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
void DirectXInput::Release(void) {
// Close the Direct Input Interface
	if (lpdi) {
		lpdi->Release();
		lpdi = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////////
void DirectXInput::GetBackground(void) {
// Get the Cursor Background.

	if (Device == MOUSE) {
	// Device is a Mouse.
		BackgroundSurface->Blt(NULL, lpDXDirectDraw->GetFrontBufferDirect(), &Rect, DDBLT_WAIT, NULL);
	}
/*#ifdef _DEBUG
	  else if (Device != NODEVICE) {
		TRACE("*** DirectXInput Report:\r\n");
		TRACE("    GetBackground was call with a Non-Mouse Interface.\r\n");
		TRACE("    Cannot get the Background of a Mouse with a Non-Mouse Interface.\r\n");
		TRACE("    PowerDebug -> Call of GetBackground was rejeted.\r\n");
	}
#endif*/
}

//////////////////////////////////////////////////////////////////////////////
void DirectXInput::SetMouseBorder(int left, int top, int right, int bottom) {
// Set the mouse Screen Border

	if (Device == MOUSE) {
	// Device is a Mouse.
	   ScreenLeft   = left;
	   ScreenTop    = top;
	   ScreenRight  = right;
	   ScreenBottom = bottom;
		ScreenWidth  = ScreenRight-ScreenLeft;
		ScreenHeight = ScreenBottom-ScreenTop;
	}
/*#ifdef _DEBUG
	  else if (Device != NODEVICE) {
		TRACE("*** DirectXInput Report:\r\n");
		TRACE("    SetMouseBorder was call with a Non-Mouse Interface.\r\n");
		TRACE("    Cannot Set the Mouse Border with a Non-Mouse Interface.\r\n");
		TRACE("    PowerDebug -> Call of SetMouseBorder was rejeted.\r\n");
	}
#endif*/
}

void DirectXInput::GetXYPos(int *x, int *y) {
	if (Device == MOUSE) {
	DIMOUSESTATE Status;

	lpDIDevice->GetDeviceState(sizeof(Status), &Status);
	g_Status.lX += Status.lX;
	g_Status.lY += Status.lY;

	for (int i = 0; i < 4; i++) 
		g_Status.rgbButtons[i] |= Status.rgbButtons[i];

	*x = xPos + Status.lX;
	*y = yPos + Status.lY;
	}
}

char DirectXInput::GetStatus(int *x, int *y, int *button) {
	if (Device == MOUSE) {
	DIMOUSESTATE Status;

	lpDIDevice->GetDeviceState(sizeof(Status), &Status);
	g_Status.lX += Status.lX;
	g_Status.lY += Status.lY;

	for (int i = 0; i < 4; i++) 
		g_Status.rgbButtons[i] |= Status.rgbButtons[i];

	for (int i = 0; i < 4; i++) {
		if (Status.rgbButtons[i]) {
			*x = ((xPos + Status.lX+48) / xGridPixel);
			*y = ((yPos + Status.lY-8) / yGridPixel);
			*button = i+1;
		}
	}

	if ((*VirtualGrid)(((xPos+Status.lX+48)/xGridPixel),(yPos+Status.lY-8)/yGridPixel)) {
		*x = ((xPos + Status.lX+48) / xGridPixel);
		*y = ((yPos + Status.lY-8) / yGridPixel);
	} else {
		*x = 0;
		*y = 0;
	}}
	
	return 0;
}	

//////////////////////////////////////////////////////////////////////////////
void DirectXInput::SetVirtualGrid(Bitmap *Grid) {
// Set the Virtual Mouse Cursor Grid
	if (Device == MOUSE) {
	// Device is a Mouse.
	   VirtualGrid = Grid;
		xGridPixel = ScreenWidth/(VirtualGrid->xMap-1);
		yGridPixel = ScreenHeight/(VirtualGrid->yMap-1);
	}
/*#ifdef _DEBUG
	  else if (Device != NODEVICE) {
		TRACE("*** DirectXInput Report:\r\n");
		TRACE("    SetVirtualGrid was call with a Non-Mouse Interface.\r\n");
		TRACE("    Cannot Set the Virtual Grid with a Non-Mouse Interface.\r\n");
		TRACE("    PowerDebug -> Call of SetVirtualGrid was rejeted.\r\n");
	}
#endif*/
}








/*
void DirectXInput::GetData(void) {
	// Get all the Data from the last call.
	HRESULT       hresult;
	unsigned long dwItems = INFINITE;

	DIDEVICEOBJECTDATA TEST[1000];

	POWERTRACE("DirectXInput: Get Data Length : ");
	hresult = lpDIDevice->GetDeviceData(sizeof(TEST), NULL, &dwItems, 0);
	if (hresult != DI_OK)
		Exception(hresult, "GetData -> Get Device Data 1", 0);

	POWERTRACE("%lu Items.\r\n", dwItems);
	hresult = lpDIDevice->GetDeviceData(sizeof(TEST), NULL, &dwItems, 0);
	if (hresult != DI_OK)
		Exception(hresult, "GetData -> Get Device Data 1", 0);

	POWERTRACE("%lu Items.\r\n", dwItems);
	hresult = lpDIDevice->GetDeviceData(sizeof(TEST), NULL, &dwItems, 0);
	if (hresult != DI_OK) 
		Exception(hresult, "GetData -> Get Device Data 1", 0);

	POWERTRACE("%lu Items.\r\n", dwItems);
	//dwItems = number of elements in buffer
//	DIDEVICEOBJECTDATA *rgdod = new DIDEVICEOBJECTDATA [dwItems];

//	POWERTRACE("DirectXInput: Get Data.\r\n");
//	lpDIDevice->GetDeviceData(sizeof(rgdod), rgdod, &dwItems, NULL);
//	if (hresult != DI_OK)
//		Exception(hresult, "GetData -> Get Device Data 2", 0);

//	for (unsigned int i = 0; i < dwItems; i++) {
//		TRACE("Chk moi ca man! : %lu\r\n", rgdod[i].dwData);
//	}
}
*/

/*
void DirectXInput::TEMP(HINSTANCE hInstance) {


// NOT USE COMMAND
// lpdi->EnumDevices ==> FORGET IT
//	lpDIMouse->EnumObjects ==> FORGET IT



	DIDEVCAPS DIDevCaps;
	DIDevCaps.dwSize = sizeof(DIDevCaps);

	lpDIMouse->GetCapabilities(&DIDevCaps);
// RETURN CODE
// DI_OK
// DIERR_INVALIDPARAM

	// Analyze DIDevCaps
	// DIDevCaps.dwDevType = 
	// DIDEVTYPE_MOUSE
	//   DIDEVTYPEMOUSE_UNKNOWN
	//   DIDEVTYPEMOUSE_TRADITIONAL
	//   DIDEVTYPEMOUSE_FINGERSTICK
	//   DIDEVTYPEMOUSE_TOUCHPAD
	//   DIDEVTYPEMOUSE_TRACKBALL
	// DIDEVTYPE_KEYBOARD
	//   DIDEVTYPEKEYBOARD_PCXT // 83 keys
	//   DIDEVTYPEKEYBOARD_OLIVETTI // 102 keys
	//   DIDEVTYPEKEYBOARD_PCAT // 84 keys
	//   DIDEVTYPEKEYBOARD_PCENH // 101/102 keys
	//   DIDEVTYPEKEYBOARD_NOKIA1050
	//   DIDEVTYPEKEYBOARD_NOKIA9140
	//   DIDEVTYPEKEYBOARD_NEC98
	//   DIDEVTYPEKEYBOARD_NEC98LAPTOP
	//   DIDEVTYPEKEYBOARD_NEC98106
	//   DIDEVTYPEKEYBOARD_JAPAN106
	//   DIDEVTYPEKEYBOARD_JAPANAX
	//   DIDEVTYPEKEYBOARD_J3100

	DIDEVICEINSTANCE pdidi;

	pdidi.dwsize = sizeof(pdidi);
	
	lpDIMouse->GetDeviceInfo(&pdidi);
// RETURN CODE
// DI_OK
// DIERR_INVALIDPARAM

	// Analyse pdidi
	// ??????

	DIDEVICEOBJECTINSTANCE pdidoi;
	pdidoi.dwSize = sizeof(pdidoi);

//	lpDIMouse->GetObjectInfo(&pdidoi, ==> FORGET IT
// RETURN CODE
// DI_OK
// DIERR_INVALIDPARAM
// DIERR_OBJECTNOTFOUND

	// See Examples
//	lpDIMouse->GetProperty(); ==> FORGET IT
// RETURN CODE
// DI_OK
// DIERR_INVALIDPARAM
// DIERR_OBJECTNOTFOUND
// DIERR_UNSUPPORTED

	// See Example
//	lpDIMouse->SetEventNotification( FORGET IT
// RETURN CODE
// DI_OK
// DIERR_ACQUIRED
// DIERR_HANDLEEXISTS
// DIERR_INVALIDPARAM

//	lpDIMouse->SetProperty( FORGET IT
// RETURN CODE
// DI_PROPNOEFFECT
// DIERR_INVALIDPARAM
// DIERR_OBJECTNOTFOUND
// DIERR_UNSUPPORTED
}
*/

/////////////////////////////////////////////////////////////////////////////
void DirectXInput::Exception(HRESULT hresult, char *DebugInformation, int DebugValue) {
// Direct Input Exception Handling
//	TRACE("\r\nDirectXInput: An Exception as occured.\r\n");
	switch (hresult) {
		case DIERR_BETADIRECTINPUTVERSION:
		// The application was written for an unsupported prerelease version 
		// of DirectInput. 		
			LOG << "FATAL ERROR:\r\nThe application was written for an unsupported prerelease version of DirecInput.\r\n";
		break;

		case DIERR_INVALIDPARAM:
		// An invalid parameter was passed to the returning function, or the
		// object was not in a state that permitted the function to be called.
		// This value is equal to the E_INVALIDARG standard COM return value. 
			LOG << "Invalid Parameters.\r\n";
			LOG << "PowerDebug Information: \r\n";
			LOG << DebugInformation << "\r\n";
		break;

		case DIERR_OLDDIRECTINPUTVERSION:
		// The application requires a newer version of DirectInput.
			LOG << "You need a newer version of DirectInput to run this program.\r\n";
		break;

		case DIERR_OUTOFMEMORY:
		// The DirectInput subsystem couldn't allocate sufficient memory to
		// complete the caller's request. This value is equal to the
		// E_OUTOFMEMORY standard COM return value.			
			LOG << "Out of memory!";
		break;

		case DIERR_DEVICENOTREG:														
		// The device or device instance is not registered with DirectInput.
		// This value is equal to the REGDB_E_CLASSNOTREG standard COM return
		// value.
			LOG << "PROGRAMMING ERROR: Device in not registered with DirectInput.\r\n";
		break;

		case DIERR_NOINTERFACE:
		// The specified interface is not supported by the object. This value 
		// is equal to the E_NOINTERFACE standard COM return value.
			LOG << "The specified interface is not supported by the object.\r\n";
		break;

		case DI_NOTATTACHED:
		// The device exists but is not currently attached. This value is equal
		// to the S_FALSE standard COM return value.
			if (DebugValue == -1) {
			// Unaquare call.
/*				TRACE("*** DirectXInput Report:\r\n");
				TRACE("    Unaquire was call on a device that was already Unaquire.\r\n");
				TRACE("    PowerDebug -> Program continue, but this could leed to a crash.\r\n");*/
			}
			LOG << "*** DirectXInput Error Report:\r\n";
			LOG << "    Device is not attached.\r\n\n";
			LOG << "    PowerDebug Information:\r\n";
			LOG << "   " << DebugInformation << "\r\n";
		break;

		case DIERR_GENERIC:
 		// An undetermined error occurred inside the DirectInput subsystem. 
		// This value is equal to the E_FAIL standard COM return value
			LOG << "Undetermined error in DirectInput.\r\n";
			LOG << "   " << DebugInformation << "\r\n";
		break;

		case DIERR_ACQUIRED:
		// The operation cannot be performed while the device is acquired.
			LOG << "The operation cannot be performed while the device is acquired.\r\n";
		break;

		case DIERR_INPUTLOST:
		// Access to the input device has been lost. It must be re-acquired.
			LOG << "Access to the input device has been lost.\r\n";
		break;

		case DIERR_NOTACQUIRED:
			LOG << "NOTACQUIRED.\r\n";
		break;

		case E_PENDING:
			LOG << "E_PENDING.\r\n";
		break;

		default:
		// Unknown Error...
			LOG << "An Unknow Error as occured...: \r\n";
//			OutputDebugString(hresult);
			LOG << "PowerDebug Information: \r\n";
			LOG << DebugInformation;
		break;
	}

   char Temp[100];
   sprintf(Temp, g_LocalString[155], 134, hresult);
   g_App.SetError(1, Temp);
	g_App.Close(Temp);
}

BOOL DirectXInput::WasRelease(void) {
   return DeviceWasRelease;
}

void DirectXInput::KeyBuffer(char *C) {
	if (Device == KEYBOARD) {
	// Device is a Keyboard
		memset(C, 0, 256);
		lpDIDevice->GetDeviceState(256, C);
	}
}

char DirectXInput::KeyHit(void) {
	if (Device == KEYBOARD) {
	// Device is a Keyboard
		memset(Status, 0, 256);
		lpDIDevice->GetDeviceState(256, Status);

		if (LastKey) {
			if (LastKey == 'q' && !Status[DIK_Q]) { LastKey = 0; return 'q'; }
			if (LastKey == 'w' && !Status[DIK_W]) { LastKey = 0; return 'w'; }
			if (LastKey == 'e' && !Status[DIK_E]) { LastKey = 0; return 'e'; }
			if (LastKey == 'r' && !Status[DIK_R]) { LastKey = 0; return 'r'; }
			if (LastKey == 't' && !Status[DIK_T]) { LastKey = 0; return 't'; }
			if (LastKey == 'y' && !Status[DIK_Y]) { LastKey = 0; return 'y'; }
			if (LastKey == 'u' && !Status[DIK_U]) { LastKey = 0; return 'u'; }
			if (LastKey == 'i' && !Status[DIK_I]) { LastKey = 0; return 'i'; }
			if (LastKey == 'o' && !Status[DIK_O]) { LastKey = 0; return 'o'; }
			if	(LastKey == 'p' && !Status[DIK_P]) { LastKey = 0; return 'p'; }
			if (LastKey == 'a' && !Status[DIK_A]) { LastKey = 0; return 'a'; }
			if (LastKey == 's' && !Status[DIK_S]) { LastKey = 0; return 's'; }
			if (LastKey == 'd' && !Status[DIK_D]) { LastKey = 0; return 'd'; }
			if (LastKey == 'f' && !Status[DIK_F]) { LastKey = 0; return 'f'; }
			if (LastKey == 'g' && !Status[DIK_G]) { LastKey = 0; return 'g'; }
			if (LastKey == 'h' && !Status[DIK_H]) { LastKey = 0; return 'h'; }
			if (LastKey == 'j' && !Status[DIK_J]) { LastKey = 0; return 'j'; }
			if (LastKey == 'k' && !Status[DIK_K]) { LastKey = 0; return 'k'; }
			if (LastKey == 'l' && !Status[DIK_L]) { LastKey = 0; return 'l'; }
			if (LastKey == 'z' && !Status[DIK_Z]) { LastKey = 0; return 'z'; }
			if (LastKey == 'x' && !Status[DIK_X]) { LastKey = 0; return 'x'; }
			if (LastKey == 'c' && !Status[DIK_C]) { LastKey = 0; return 'c'; }
			if (LastKey == 'v' && !Status[DIK_V]) { LastKey = 0; return 'v'; }
			if (LastKey == 'b' && !Status[DIK_B]) { LastKey = 0; return 'b'; }
			if	(LastKey == 'n' && !Status[DIK_N]) { LastKey = 0; return 'n'; }
			if (LastKey == 'm' && !Status[DIK_M]) { LastKey = 0; return 'm'; }
			if (LastKey == 'E' && !Status[DIK_RETURN]) { LastKey = 0; return 'E'; }
			if (LastKey == '+' && !Status[DIK_ADD]) { LastKey = 0; return '+'; }
			if	(LastKey == '-' && !Status[DIK_SUBTRACT]) { LastKey = 0; return '-'; }
			if (LastKey == 'B' && !Status[DIK_BACK]) { LastKey = 0; return 'B'; }
			if (LastKey == 'S' && !Status[DIK_ESCAPE]) { LastKey = 0; return 'S'; }
			if (LastKey == ' ' && !Status[DIK_SPACE]) { LastKey = 0; return ' '; } 
			if (LastKey == '1' && !Status[DIK_1]) { LastKey = 0; return '1'; }
			if (LastKey == '2' && !Status[DIK_2]) { LastKey = 0; return '2'; }
			if (LastKey == '3' && !Status[DIK_3]) { LastKey = 0; return '3'; }
			if (LastKey == '4' && !Status[DIK_4]) { LastKey = 0; return '4'; }
			if (LastKey == '5' && !Status[DIK_5]) { LastKey = 0; return '5'; }
			if (LastKey == '6' && !Status[DIK_6]) { LastKey = 0; return '6'; }
			if (LastKey == '7' && !Status[DIK_7]) { LastKey = 0; return '7'; }
			if (LastKey == '8' && !Status[DIK_8]) { LastKey = 0; return '8'; }
			if (LastKey == '9' && !Status[DIK_9]) { LastKey = 0; return '9'; }
			if (LastKey == '0' && !Status[DIK_0]) { LastKey = 0; return '0'; }
			if (LastKey == '.' && !Status[DIK_PERIOD]) { LastKey = 0; return '.'; }
			if (LastKey == ',' && !Status[DIK_COMMA]) { LastKey = 0; return ','; }
		} else {
			if (Status[DIK_SPACE]) LastKey = ' ';
			if (Status[DIK_BACK]) LastKey = 'B';
			if (Status[DIK_ESCAPE]) LastKey = 'S';
			if (Status[DIK_ADD]) LastKey = '+';
			if (Status[DIK_SUBTRACT]) LastKey = '-';
			if (Status[DIK_RETURN]) LastKey = 'E';
			if (Status[DIK_Q]) LastKey = 'q';
			if (Status[DIK_W]) LastKey = 'w';
			if (Status[DIK_E]) LastKey = 'e';
			if (Status[DIK_R]) LastKey = 'r';
			if (Status[DIK_T]) LastKey = 't';
			if (Status[DIK_Y]) LastKey = 'y';
			if (Status[DIK_U]) LastKey = 'u';
			if (Status[DIK_I]) LastKey = 'i';
			if (Status[DIK_O]) LastKey = 'o';
			if (Status[DIK_P]) LastKey = 'p';
			if (Status[DIK_A]) LastKey = 'a';
			if (Status[DIK_S]) LastKey = 's';
			if (Status[DIK_D]) LastKey = 'd';
			if (Status[DIK_F]) LastKey = 'f';
			if	(Status[DIK_G]) LastKey = 'g';
			if (Status[DIK_H]) LastKey = 'h';
			if (Status[DIK_J]) LastKey = 'j';
			if (Status[DIK_K]) LastKey = 'k';
			if (Status[DIK_L]) LastKey = 'l';
			if (Status[DIK_Z]) LastKey = 'z';
			if (Status[DIK_X]) LastKey = 'x';
			if (Status[DIK_C]) LastKey = 'c';
			if (Status[DIK_V]) LastKey = 'v';
			if (Status[DIK_B]) LastKey = 'b';
			if (Status[DIK_N]) LastKey = 'n';
			if (Status[DIK_M]) LastKey = 'm';
			if (Status[DIK_1]) LastKey = '1';
			if (Status[DIK_2]) LastKey = '2';
			if (Status[DIK_3]) LastKey = '3';
			if (Status[DIK_4]) LastKey = '4';
			if (Status[DIK_5]) LastKey = '5';
			if (Status[DIK_6]) LastKey = '6';
			if (Status[DIK_7]) LastKey = '7';
			if (Status[DIK_8]) LastKey = '8';
			if (Status[DIK_9]) LastKey = '9';
			if (Status[DIK_0]) LastKey = '0';
			if (Status[DIK_PERIOD]) LastKey = '.'; 
			if (Status[DIK_COMMA])  LastKey = ','; 
		}
	}
	
	return 0;
}



