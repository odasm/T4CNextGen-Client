#pragma warning( disable : 4291 )

#include "pch.h"
#include <windows.h>
#include "PowerDebug.h"
#include "Mouse.h"
#include "IDirectXDraw.h"
#include "mmsystem.h"
#include "SystemInfo.h"
#include <process.h>
#include "DirectXInput.h"
#include "old.h"
#include <eh.h>
#include "ExpFltr.h"
#include <stdio.h>
#include "Comm.h"
#include "LocalString.h"
#include "App.h"
#include "Global.h"
#include "NewInterface/RootBoxUI.h"
#include "UIMouseEvent.h"
#include "MemoryManager.h"

extern Global g_Global;


inline int Abs(int x) {
	return ((x < 0) ? (-x) : (x));
}

QWORD FileTime2QWord(FILETIME *f);

extern int NBCURSOR;
extern CV2Sprite **lplpCursor;
extern Bitmap Grid;
extern char Force;

DWORD ThreadCTimeStamp = 0;

HANDLE hMouseThread = NULL;

UINT MouseDevice::QueryMouseEventID = NULL;
int MouseDevice::DeviceCount = 0;
int MouseDevice::xPos = 0;
int MouseDevice::yPos = 0;
UINT MouseDevice::Buttons[4];
UINT MouseDevice::ButtonsState[4];
DWORD MouseDevice::Click[4];
int MouseDevice::xClick[4];
int MouseDevice::yClick[4];
DWORD MouseDevice::ButtonDown[4];
DWORD MouseDevice::LockEvent[4] = { DM_NONE, DM_NONE, DM_NONE, DM_NONE };
LPVOID MouseDevice::LockCustomPtr[4];
UINT (* MouseDevice::LockEventCallBack[4])(LPVOID);


//**************************************************************************************//
MouseDevice::MouseDevice( void )
//**************************************************************************************//
// Default Constructor
// 
{
   if (!DeviceCount) {
      hMouseThread = (HANDLE) _beginthreadex(NULL, 0, QueryMouseEventThread, NULL, NULL, &QueryMouseEventID);
   }
	DeviceCount=1;
}

//*** Destructor ***********************************************************************//
MouseDevice::~MouseDevice( void )
//**************************************************************************************//
// Default Destructor
// 
{
//	DeviceCount--;
//	if (!DeviceCount) {}
}

void MouseDevice::SetPosition(int x, int y) {
   xPos = x;
	yPos = y;
}

bool MouseDevice::LockNextEvent(DWORD dwEvent, LPVOID lpCustomPtr, UINT (* CallBack)(LPVOID), int i) {
    // Refuse mouse event if the mouse is already locked.
    if( LockEvent[i] != DM_NONE ){
        return false;
    }
    
    LockEventCallBack[i] = CallBack;
    LockEvent[i] = dwEvent;
    LockCustomPtr[i] = lpCustomPtr;

    return true;
}

//**************************************************************************************//
void MouseDevice::GetPosition
//**************************************************************************************//
// Get the Mouse Cursor Position
// 
(
 int *x, // x Position of the Mouse Cursor
 int *y  // y Position of the Mouse Cursor
)
//**************************************************************************************//
{
   *x = xPos;
	*y = yPos;
}

//**************************************************************************************//
void MouseDevice::GetDrag
//**************************************************************************************//
// Get the Mouse Cursor Position
// 
(
 int *x, // x Position of the Mouse Cursor
 int *y, // y Position of the Mouse Cursor
 int Button
)
//**************************************************************************************//
{
	*x = xClick[Button];
	*y = yClick[Button];
}

//**************************************************************************************//
void MouseDevice::GetStatus
//**************************************************************************************//
// Get the Status of the Mouse
// 
(
 int *x, // x Position of the Mouse Cursor
 int *y, // y Position of the Mouse Cursor
 int *button // Button Status
)
//**************************************************************************************//
{
	*x = xPos;
	*y = yPos;
	for (int i = 0; i < 4; i++) {
		if (ButtonDown[i] && ButtonsState[i] != DM_NONE && ButtonsState[i] != DM_DRAG) {
         if (timeGetTime() - ButtonDown[i] > 250) {
				ButtonsState[i] = DM_DRAG;
         }
		}
		button[i] = ButtonsState[i];
		if (ButtonsState[i] != DM_DOWN && ButtonsState[i] != DM_DRAG)
			ButtonsState[i] = DM_NONE;
	
      if (LockEvent[i] == (unsigned)button[i] && LockEvent[i]) {
         LockEventCallBack[i](LockCustomPtr[i]);
         LockEvent[i] = DM_NONE;
         button[i] = DM_WAIT;
      }
   }

	if (GetSystemMetrics(SM_SWAPBUTTON)) {
		int Temp = button[0];
		button[0] = button[1];
		button[1] = Temp;
	}
}

//extern int Tabbing;//BLBLB 26 mars 2009 désactivé ce truc outdated.

extern DirectXInput KeyBoard;
LPDIRECTINPUTDEVICE lpDIMouseDevice = NULL; // Direct Input Device

extern BOOL inAttack;

UINT WINAPI QueryMouseEventThread(LPVOID pParam) {
	g_App.StartThread(APP_MOUSE_THREAD);
	
	HRESULT hresult; // Result
   LPDIRECTINPUT lpdi;				  // Direct Input Interface

   bool inverseMouse = false; 
   if (GetSystemMetrics(SM_SWAPBUTTON)) {
       inverseMouse = true;
   }

	// Create Direct Input Interface.
   if (!KeyBoard.IsCreated()) {
      LOG << "* DBG = Keyboard Create 1\r\n";
      KeyBoard.Create();
	}
   lpdi = KeyBoard.lpdi;

   // Create a Device
	hresult = lpdi->CreateDevice(GUID_SysMouse, &lpDIMouseDevice, NULL);
	if (hresult != DI_OK) {
		char Temp[100];
      sprintf(Temp, g_LocalString[155], 2, hresult);
		g_App.SetError(1, Temp);
		g_App.Close(Temp);
	}
   Sleep(15);

	// Get the Device Status
	hresult = lpdi->GetDeviceStatus(GUID_SysMouse);
	if (hresult != DI_OK) {
		char Temp[100];
      sprintf(Temp, g_LocalString[155], 3, hresult);
		g_App.SetError(1, Temp);
		g_App.Close(Temp);
	}
   Sleep(15);

	hresult = lpDIMouseDevice->SetDataFormat(&c_dfDIMouse);
	if (hresult != DI_OK) {
		char Temp[100];
      sprintf(Temp, g_LocalString[155], 5, hresult);
		g_App.SetError(1, Temp);
		g_App.Close(Temp);
	}
   Sleep(15);

   // Set Cooperative Level, will work with both Win95 an WinNT
	hresult = lpDIMouseDevice->SetCooperativeLevel(siGethWnd(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE); // FOREGROUND
	if (hresult != DI_OK) {
		char Temp[100];
      sprintf(Temp, g_LocalString[155], 4, hresult);
		g_App.SetError(1, Temp);
		g_App.Close(Temp);
	}
//	LOG << "Mouse Info " 
//		 << -2147467263 
//	    << ", " << DIERR_ACQUIRED
//	    << ", " << DIERR_ALREADYINITIALIZED  
//	    << ", " << DIERR_BADDRIVERVER  
//	    << ", " << DIERR_BETADIRECTINPUTVERSION  
//		 << ", " << DIERR_DEVICEFULL  
//	    << ", " << DIERR_DEVICENOTREG  
//	    << ", " << DIERR_EFFECTPLAYING 
//	    << ", " << DIERR_HASEFFECTS 
//	    << ", " << DIERR_GENERIC  
//	    << ", " << DIERR_HANDLEEXISTS  
//	    << ", " << DIERR_INCOMPLETEEFFECT 
//	    << ", " << DIERR_INPUTLOST  
//	    << ", " << DIERR_INVALIDPARAM  
//	    << ", " << DIERR_MOREDATA 
//	    << ", " << DIERR_NOAGGREGATION  
//	    << ", " << DIERR_NOINTERFACE  
//	    << ", " << DIERR_NOTACQUIRED  
//	    << ", " << DIERR_NOTBUFFERED 
//	    << ", " << DIERR_NOTDOWNLOADED 
//	    << ", " << DIERR_NOTEXCLUSIVEACQUIRED  
//	    << ", " << DIERR_NOTFOUND  
//	    << ", " << DIERR_NOTINITIALIZED  
//	    << ", " << DIERR_OBJECTNOTFOUND  
//	    << ", " << DIERR_OLDDIRECTINPUTVERSION  
//	    << ", " << DIERR_OTHERAPPHASPRIO  
//	    << ", " << DIERR_OUTOFMEMORY  
//	    << ", " << DIERR_READONLY  
//	    << ", " << DIERR_REPORTFULL  
//	    << ", " << DIERR_UNPLUGGED 
//	    << ", " << DIERR_UNSUPPORTED  
//	    << "\r\n";

   Sleep(15);
	
	// Acquire the Input Device.

	DIMOUSESTATE Status;
   ZeroMemory(&Status, sizeof(DIMOUSESTATE));
	
	hresult = lpDIMouseDevice->Acquire();
	hresult = DI_OK;
	if (hresult != DI_OK) {
		char Temp[100];
      sprintf(Temp, g_LocalString[155], 6, hresult);
		g_App.SetError(1, Temp);
		g_App.Close(Temp);
	}
   Sleep(15);

 	 BYTE buttonStatus[ 4 ] = { 0, 0, 0, 0 };
    lpDIMouseDevice->GetDeviceState(sizeof(Status), &Status);
    long lastZButton = Status.lZ;

    DWORD lastDragTime = 0;

	do {
      ThreadCTimeStamp = 0;
         
      Sleep(5);
        	
      ThreadCTimeStamp = timeGetTime();
	  POINT pos;
	  RECT poswin;
	  GetWindowRect(siGethWnd(),&poswin);
	  GetCursorPos(&pos);

	  bool InScreen = true;

	  if (Custom.gWindowed)
	  {
		  // NightMare -- Add 2005-02-17
		  // When resizing Block All Screen Event
		  if (Custom.gWindowedSizing)
		  {
			  InScreen=false;
		  }
		  else
		  {
		     if( pos.x > poswin.left && pos.x < poswin.right &&
			     pos.y > poswin.top && pos.y < poswin.bottom     )
			    InScreen=true;
		     else 
			    InScreen=false;
		  }
		  // END NightMare
		  ///////////////////////////
	  }
	  /*else if (!g_App.isFocus())//BLBL Freeze test
		  InScreen=false;*/
	  

		if (InScreen) {
		    // DIMOUSESTATE
            lpDIMouseDevice->GetDeviceState(sizeof(Status), &Status);        
                               
            int i;
			POINT p={0,0};
			ClientToScreen(siGethWnd(), &p);
			RECT clientRect;
			GetClientRect(siGethWnd(),&clientRect);
			

			MouseDevice::xPos=(int)((float)(pos.x-p.x) * (float)((float)g_Global.GetScreenW()/(float)clientRect.right));//att jreflechi sur le calcule a faire
			MouseDevice::yPos=(int)((float)(pos.y-p.y) * (float)((float)g_Global.GetScreenH()/(float)clientRect.bottom));// test et dit moi si ca marche , sinon quels sont les changement

            if (MouseDevice::xPos /*+ Status.lX*/ > g_Global.GetScreenW())
			    MouseDevice::xPos = g_Global.GetScreenW();
		    else if (MouseDevice::xPos /*+ Status.lX*/ < 0)
			    MouseDevice::xPos = 0;
		   /* else
			    MouseDevice::xPos += (Status.lX / Tabbing);*/

		    if (MouseDevice::yPos /*+ Status.lY*/ > g_Global.GetScreenH())
			    MouseDevice::yPos = g_Global.GetScreenH();
		    else if (MouseDevice::yPos /*+ Status.lY*/ < 0)
			    MouseDevice::yPos = 0;
		    /*else
			    MouseDevice::yPos += (Status.lY / Tabbing);*/
            
          
            //RootBoxUI *rootBox = RootBoxUI::GetInstance(); // TODO DaP non mais c'est une blague ????

            UIMouseEvent *mouse = UIMouseEvent::GetInstance();

            // If the state of the left button changed.
            if( buttonStatus[ 0 ] != Status.rgbButtons[ 0 ] ){
                // Mouse was pushed down.
                if( Status.rgbButtons[ 0 ] != 0 ){
                    LOG << "Left mouse down at " << MouseDevice::xPos << ", " << MouseDevice::yPos;
                    //boUIMouseControl = rootBox->LeftMouseDown( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    if( inverseMouse ){
                        mouse->QueueRightMouseDown( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    }else{
                        mouse->QueueLeftMouseDown( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    }
                }
                // Mouse was lifted up.
                else{
                    LOG << "Left mouse up at " << MouseDevice::xPos << ", " << MouseDevice::yPos;
                    //boUIMouseControl = rootBox->LeftMouseUp( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    if( inverseMouse ){
                        mouse->QueueRightMouseUp( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    }else{
                        mouse->QueueLeftMouseUp( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    }
                }

                buttonStatus[ 0 ] = Status.rgbButtons[ 0 ];
            }else

            // If the state of the left button changed.
            if( buttonStatus[ 1 ] != Status.rgbButtons[ 1 ] ){
                // Mouse was pushed down.
                if( Status.rgbButtons[ 1 ] != 0 ){
                    LOG << "Right mouse down at " << MouseDevice::xPos << ", " << MouseDevice::yPos;
                    //boUIMouseControl = rootBox->RightMouseDown( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    if( inverseMouse ){
                        mouse->QueueLeftMouseDown( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    }else{
                        mouse->QueueRightMouseDown( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    }
                }
                // Mouse was lifted up.
                else{
                    LOG << "Right mouse up at " << MouseDevice::xPos << ", " << MouseDevice::yPos;
                    //boUIMouseControl = rootBox->RightMouseUp( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    if( inverseMouse ){
                        mouse->QueueLeftMouseUp( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    }else{
                        mouse->QueueRightMouseUp( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    }
                }

                buttonStatus[ 1 ] = Status.rgbButtons[ 1 ];
            }else

            // If the state of the Z axis changed (Wheel change).
            if( lastZButton != Status.lZ ){

                LOG.Lock();
                LOG << "\r\nMouse wheel movement, pos: " << (int)Status.lZ << " prior pos: " << (int)lastZButton;
                LOG.Unlock();

                int delta = -Status.lZ / 120;//( lastZButton - Status.lZ ) / 120;
                
                // If wheel went up.
                if( delta > 0 ){
                    LOG.Lock();
                    LOG << "\r\nWheel up at " << MouseDevice::xPos << ", " << MouseDevice::yPos << " Delta = " << (int)delta << "  ";
                    LOG.Unlock();
                                       
                    long z;
                    for( z = 0; z < abs( delta ); z++ ){
                        //boUIMouseControl = rootBox->WheelUp( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                        mouse->QueueWheelUp( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    }
                }else{
                    LOG.Lock();
                    LOG << "\r\nWheel down at " << MouseDevice::xPos << ", " << MouseDevice::yPos << " Delta = " << (int)delta << "  ";
                    LOG.Unlock();
                    long z;
                    for( z = 0; z < abs( delta ); z++ ){
                        //boUIMouseControl = rootBox->WheelDown( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                        mouse->QueueWheelDown( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    }
                }
                // Reset last Z button.
                lastZButton = Status.lZ;
                if(lastZButton > 0 )
                   g_Global.SetlastScrollStatus(1);
                else if(lastZButton < 0 )
                   g_Global.SetlastScrollStatus(-1);
            }else if( MouseDevice::ButtonsState[ 0 ] == DM_DRAG ){
                if( GetTickCount() >= lastDragTime ){
                    mouse->QueueDrag( MousePos( MouseDevice::xPos, MouseDevice::yPos ) );
                    lastDragTime = GetTickCount() + 50; // ms
                }
            }

            for ( i = 0; i < 4; i++) {
			    if (Status.rgbButtons[i]) {
                if (MouseDevice::Buttons[i] == DM_UP) {
                   MouseDevice::xClick[i] = MouseDevice::xPos;
                   MouseDevice::yClick[i] = MouseDevice::yPos;
                   MouseDevice::Buttons[i] = DM_DOWN;
                   if (!MouseDevice::LockEvent[i] || (MouseDevice::LockEvent[i] == DM_DOWN)) {
                      MouseDevice::ButtonsState[i] = DM_DOWN;
                   }
				       MouseDevice::ButtonDown[i] = timeGetTime();
                } else if (MouseDevice::Buttons[i] != DM_WAIT || MouseDevice::ButtonsState[i] != DM_DRAG) {
                   if (Abs(MouseDevice::xClick[i] - MouseDevice::xPos) > 25 || Abs(MouseDevice::yClick[i] - MouseDevice::yPos) > 25)
                      if (!MouseDevice::LockEvent[i] || (MouseDevice::LockEvent[i] == DM_DRAG)) {
                         MouseDevice::ButtonsState[i] = DM_DRAG;
                      }
                }
			    } else {
                if (MouseDevice::Buttons[i] == DM_DOWN || MouseDevice::ButtonsState[i] == DM_DRAG) {
					    if (timeGetTime() - MouseDevice::Click[i] < 250) {
						    MouseDevice::ButtonsState[i] = DM_DOUBLE_CLICK;
						    MouseDevice::Click[i] = 0;
					    } else if (MouseDevice::ButtonsState[i] == DM_DRAG) {
						    MouseDevice::ButtonsState[i] = DM_DROP;
						    MouseDevice::Click[i] = timeGetTime();
					    } else {
                      MouseDevice::ButtonsState[i] = DM_CLICK;
						    MouseDevice::Click[i] = timeGetTime();
					    }
				    }
				    MouseDevice::ButtonDown[i] = 0;
				    MouseDevice::Buttons[i] = DM_UP;
				    if (!i)
					    inAttack = FALSE;
			    }
		    }
		}
   } while (!g_boQuitApp);

   lpDIMouseDevice->Unacquire();
   lpDIMouseDevice->Release();
   lpdi->Release();

	g_App.ExitThread(APP_MOUSE_THREAD);
	_endthreadex(1);
   return 1;
}

CMouseCursor::CMouseCursor(void) {
   InitializeCriticalSection(&csInternal);
   vsfCursor = NULL;
   iXCor = 0;
   iYCor = 0;
}

CMouseCursor::~CMouseCursor(void) {
   DeleteCriticalSection(&csInternal);
}

CMouseCursor *CMouseCursor::GetInstance(void) {
   static CMouseCursor mcDevice;
   return &mcDevice;
}

void CMouseCursor::SetCustomCursor(CV2Sprite *cc, int x, int y, bool corr ) {
   EnterCriticalSection(&csInternal);
   vsfCursor = cc;
   iXCor = x;
   iYCor = y; 
   correction = corr;
   LeaveCriticalSection(&csInternal);
}

void CMouseCursor::DrawCursor(int x, int y) {
    EnterCriticalSection(&csInternal);
    int xPos = x;
    int yPos = y;
    int CursorWidth = 40;
    int CursorHeight = 47;
    int xGridPixel = 32;
    int yGridPixel = 16;

    V2SPRITEFX fx;
    ZeroMemory(&fx, sizeof(V2SPRITEFX));
    RECT r = {0, 0, g_Global.GetScreenW(), g_Global.GetScreenH()};
    fx.lpClipRect = &r;
    if( !correction )
    {
        //fx.dwFX |= FX_NOCORRECTION;
    }

    // If a forced cursor or a custom cursor was specified.
    if (Force || vsfCursor) 
    {
        int dwMouseDistance = g_Global.GetScreenWDiv2()*3;    
        // Determine the exact forced cursor to use.
        if (Force != 9+NBCURSOR && !vsfCursor) 
        {
            int a = xPos*3;
            int b = yPos*4;
            if (b > dwMouseDistance) 
            {
                if (a > dwMouseDistance) 
                {
                    a -= dwMouseDistance;
                    b -= dwMouseDistance;
                    if (a > b*2)
                        Force = 3+NBCURSOR;
                    else if (b > a*2)
                        Force = 5+NBCURSOR;
                    else
                        Force = 4+NBCURSOR;	
                } 
                else 
                {
                    b -= dwMouseDistance;
                    a = dwMouseDistance - a;
                    if (a > b*2)
                        Force = 7+NBCURSOR;
                    else if (b > a*2)
                        Force = 5+NBCURSOR;
                    else
                        Force = 6+NBCURSOR;
                }
            } 
            else 
            {
                if (a > dwMouseDistance) 
                {
                    a -= dwMouseDistance;
                    b = dwMouseDistance - b;
                    if (a > b*2)
                        Force = 3+NBCURSOR;
                    else if (b > a*2)
                        Force = 1+NBCURSOR;
                    else
                        Force = 2+NBCURSOR;
                } else 
                {
                    a = dwMouseDistance - a;
                    b = dwMouseDistance - b;
                    if (a > b*2)
                        Force = 7+NBCURSOR;
                    else if (b > a*2)
                        Force = 1+NBCURSOR;
                    else
                        Force = 8+NBCURSOR;
                }			
            }
        }
        
        
        // Display either the custom vsf cursor or the forced cursor.
        if (vsfCursor) 
        {
            vsfCursor->DrawSpriteN(x+iXCor, y+iYCor, &fx);
        } 
        else 
        {
            lplpCursor[Force]->DrawSpriteN( x + iXCor, y+iYCor, &fx );
        }
        LeaveCriticalSection(&csInternal);
        return;
    } 
    else 
    {
        // Release the cursor looking for the new UI ownership.
        LeaveCriticalSection(&csInternal);
		bool mouseOwned  = RootBoxUI::GetInstance()->IsMouseOwned(MousePos(x, y)); // TODO DaP
        EnterCriticalSection(&csInternal);

        if( mouseOwned )
        {
            // Only draw the hand icon when in the new interface
            lplpCursor[0]->DrawSpriteN( x + iXCor, y+iYCor, &fx );            
        }
        else
        {
           //++NMNMNM --[ZOOM mode Correction]
           int dwZoomStatus = g_Global.GetZoomStatus();
           if(dwZoomStatus)
           {
              int dwNewWidth  =   g_Global.GetScreenW()- (g_Global.GetScreenW()*(5*dwZoomStatus)/100);
              int dwNewHeight =   g_Global.GetScreenH()- (g_Global.GetScreenH()*(5*dwZoomStatus)/100);
              int dwNX = (xPos * dwNewWidth ) / g_Global.GetScreenW();
              int dwNY = (yPos * dwNewHeight) / g_Global.GetScreenH();
              xPos = dwNX + ((g_Global.GetScreenW()-dwNewWidth)/2);
              yPos = dwNY + ((g_Global.GetScreenH()-dwNewHeight)/2);
           }

            // Otherwise check what type of unit is at the mouse position
            // (grid coordinates) and show the associated cursor.
            if (Grid(((xPos+48-16)/xGridPixel),(yPos-8)/yGridPixel)) 
            {
               int dwType = Grid(((xPos+48-16)/xGridPixel),(yPos-8)/yGridPixel); 
               if(dwType == 2 && !Player.rangedAttack)
                  dwType--;
               else if(dwType ==3) //item
                  dwType = 17;     // Bulle pour parler...
               else if(dwType ==4) //Door / coffre
                  dwType = 16;     // Door Cursor
               else if(dwType ==5) //item
                  dwType = 16;     // take item cursor...
               lplpCursor[dwType]->DrawSpriteN( x + iXCor, y+iYCor, &fx );
               /*
               char strtoto[1000];
               sprintf(strtoto,"[33]Cursor Index == [%d][dwType]\n",Grid(((xPos+48-16)/xGridPixel),(yPos-8)/yGridPixel));
               OutputDebugString(strtoto);
               */
            } 
            else 
            {
               /*
                char strtoto[1000];
                sprintf(strtoto,"[44]Cursor Index == [%d]\n",Grid(((xPos+48-16)/xGridPixel),(yPos-8)/yGridPixel));
                OutputDebugString(strtoto);
                */
                lplpCursor[Grid(((xPos+48-16)/xGridPixel),(yPos-8)/yGridPixel)]->DrawSpriteN( x + iXCor, y+iYCor, &fx );
            }
        }
    }

    LeaveCriticalSection(&csInternal);
   return;
}
