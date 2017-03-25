// ************************************************************************
// ***                                                                  ***
//      File Name: App.cpp
//      Project:   The 4th Coming
//      Creation:  25 Mars, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***
#include "pch.h"
// Include 
#include "App.h"
#include "TFCPacket.h" // vector.
#include "COMM.h"
#include "IDirectXDraw.h"
#include "PowerDebug.h"
#include "Fading.h"
#include "GameMusic.h"
#include "V2Database.h"
#include "Disp.h"
#include "LocalString.h"
#include "GUILocalString.h"   
#include "NewInterface/RootBoxUI.h"
#include "MemoryManager.h"
#include "NewFont.h"
#include "Lock.h"

#include "Global.h"
#include "SaveGame.h"

// Extern Global.
extern Font *fFont;
extern Font *fSystemFont;
extern Font *fSkillFont;
extern Font *fSkillFont2;
extern Font *fNewGuiFont;
extern Font *fNewGuiBtnFont;
extern Font *fNewGuiBtnFontBig;
extern Font *fNewGuiBtnFont2;
extern Font *fNewGuiBtnFont3;
extern Font *fNewGuiInfoFont;
extern Font *fNewGuiInfoFont2;
extern Font *fNewGuiSpellFont;
extern Font *fMenuFont;
extern Font *fMenuLittleFont;
extern Font *fMenuDescFont;
extern Font *BucephalusFont;
extern CV2Sprite **lplpCursor;
extern HANDLE InterPacketEvent;
extern char *MsgDay;
extern HANDLE hMaintenanceThread;
extern HANDLE hDrawThread;
extern HANDLE hMouseActionThread;
extern CSaveGame g_SaveGame;

// Global Var.
AppManagement g_App;
AppVariable   g_Var;
bool          g_boQuitApp = false;
bool          g_UiInit = false;
bool          g_boWaitForShutdown = true;
EVENTHELP	  g_EventHelp;

unsigned char *g_ListData  = NULL;
int            g_ListSize  = 0;

static CLock  drawLock;
static CLock  mainLock;
static CLock  mouseLock;
static CLock  packetLock;

static bool loading = false;

//*********************************************************************************
AppManagement::AppManagement( void )
/**********************************************************************************
 * AppManager Constructor
 */
{
   // Constructor
   SMT_Exit = true;
   SCT_Exit = true;
   DT_Exit  = true;
   MT_Exit  = true;
   MS_Exit  = true;
   CD_Exit  = true;
   
   boFocus   = true;
   AddFontResource("Game Files\\t4cbeaulieux.ttf"); // steph Game Files au lieu de Fonts
}

//*********************************************************************************
AppManagement::~AppManagement( void )
/**********************************************************************************
 * App Manager Desctructor
 */
{

   
}

#define CLOSELOG( str ) if( Custom.Debug ){ FILE *f = fopen( "exit.log", "ab" ); fprintf( f, "\r\n%s", str ); fclose( f ); OutputDebugString( str ); };

//////////////////////////////////////////////////////////////////////////////////////////
UINT AppManagement::AsyncClose
//////////////////////////////////////////////////////////////////////////////////////////
//  Asynchronous closing function
// 
(
 LPVOID theApp // Unused
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    AppManagement *app = static_cast< AppManagement * >( theApp );

    CLOSELOG( "OUT-1" );
    // Are we in quitting mode ? No then...
    int i; // Loop value
  
  
    // Set the flag that we are currenlty closing the application.    

    try
    {
       Sleep(250);
      // Sending the Close packet (20) to the Server.
      TFCPacket Send;
      Send << (short)20;//BLBLBL déconnexion finale
  
      if (COMM.Ctr) {
         // Was socket created ?
         SEND_PACKET(Send);
         Sleep(50);
      }

      if( g_UiInit ){
        // Notify all UI of a shutdown.
        RootBoxUI::GetInstance()->ClientShutdown();
      }

      CLOSELOG( "OUT1" );
   
      if( siGetMainWndHook() != 0 ){
          UnhookWindowsHookEx( siGetMainWndHook() );
      }
  
      CLOSELOG( "OUT2" );

	  // Allow time for the close packet to be sent.
	  Sleep( 1000 );
	  CLOSELOG( "OUT2.1" );
      // Close the COMM Center
      COMM.Close();

      // Delete all Fonts.
      FontManager::GetInstance()->Release();

      CLOSELOG( "OUT4" );
  
      // Closing Thread Stuff.
      if ( 1/*app->boFocus*/ ) {
         // Are we in focus ?
         // Sound Manager Thread. Set the Event to make the loop exit.
         for (i = 0; i < 200; i++) {
            if (app->SMT_Exit)
               i = 200;
            Sleep(15);
         }
     
         CLOSELOG( "OUT4.1" );
		 // Sound Control Thread.
         for (i = 0; i < 200; i++) {
            if (app->SCT_Exit)
               i = 200;
            Sleep(15);
         }
     
         CLOSELOG( "OUT4.2" );
		 // Drawing Thread.
         for (i = 0; i < 200; i++) {
            if (app->DT_Exit)
               i = 200;
			
            Sleep(15);
         }
		if( WaitForSingleObject( hDrawThread, 0 ) == WAIT_TIMEOUT ){
            CLOSELOG( "\r\nSuspending draw thread." );
            SuspendThread( hDrawThread );
		}

     
         CLOSELOG( "OUT4.3" );
		 // Maintenance Thread.
         for (i = 0; i < 200; i++) {
            if (app->MT_Exit)
               i = 200;
			Sleep(15);
         }

		 CLOSELOG( "OUT4.4" );
		 if( WaitForSingleObject( hMaintenanceThread, 0 ) == WAIT_TIMEOUT ){
			 CLOSELOG( "\r\nSuspending maintenance thread." );
             Sleep( 5000 );
			 //SuspendThread( hMaintenanceThread );
		 }
     
         CLOSELOG( "OUT4.5" );
		 // Mouse Thread.
         for (i = 0; i < 200; i++) {
            if (app->MS_Exit)
               i = 200;
			Sleep(15);
         }

		 if( WaitForSingleObject( hMouseActionThread, 0 ) == WAIT_TIMEOUT ){
			 CLOSELOG( "\r\nSuspending mouse thread." );
			 SuspendThread( hMouseActionThread );
		 }

     
         CLOSELOG( "OUT4.6" );
		 // CD Thread.
         for (i = 0; i < 200; i++) {
            if (app->CD_Exit)
               i = 200;
			Sleep(15);
         }
      }

 
      CLOSELOG( "OUT6" );
  
      try{
      // Music Section. We are currently Stopping and closing the CDAudio
      g_GameMusic.Stop();
      //CloseCDAudio();
      if (ug_T3SoundManager) {
         if (ug_T3SoundManager->hEvent)
            SetEvent(ug_T3SoundManager->hEvent);
         Sleep(50);
         if (ug_T3SoundManager)
            delete ug_T3SoundManager;
         ug_T3SoundManager = NULL;
         
      }
      }catch(...){
          LOG << "\r\nError closing sound.";
      }
	
	  CLOSELOG( "OUT6.1" );

	  // Closing DirectDraw Stuff.
      if (lpDXDirectDraw) {
         // Was directdraw initialized ?
         if (lpDXDirectDraw->lpdd && lpDXDirectDraw->DXCompleteInit) {
			 CLOSELOG( "OUT6.2" );
            // Was directdraw fully initialized ?
            //if (app->boFocus) {//BLBL Freeze test
				CLOSELOG( "OUT6.3" );
               // Are we in focus ? If so fade the screen before closing.
               g_Pal.FadeToBlack();
           
               CLOSELOG( "OUT6.4" );
			   if (DXDClear() == DD_OK) {
				   CLOSELOG( "OUT6.5" );
               // This prevent some error of Surface Lost (or other....)

                  DXDFlip();
                  DXDClear();
               }
            //}//BLBL Freeze test
         }
     
		 CLOSELOG( "OUT6.6" );
         // Release everyting.
         lpDXDirectDraw->Release();
		 CLOSELOG( "OUT6.7" );
         delete lpDXDirectDraw;
      }

      // Delete Mouse Cursor
      delete [] lplpCursor;
  
      // Restore Mouse Cursor
      int h = ShowCursor(true);
      while (!h) {
         h = ShowCursor(true);
      }

      CLOSELOG( "OUT7" );
  
      // Delete InterpretPacket Event.
      if (InterPacketEvent) {
         CloseHandle(InterPacketEvent);
         InterPacketEvent = NULL;
      }
  
      CLOSELOG( "OUT8" );
       

      // Delete font handler and all fonts
      if (UniqueFontHandler)
         delete UniqueFontHandler;

      CLOSELOG( "OUT8.1" );
      if (fFont)
         delete fFont;
      CLOSELOG( "OUT8.2" );
      if (fSkillFont)
         delete fSkillFont;
      if (fSkillFont2)
         delete fSkillFont2;
      CLOSELOG( "OUT8.3" );
      if (BucephalusFont)
         delete BucephalusFont;
      CLOSELOG( "OUT8.4" );
      if (fMenuFont)
         delete fMenuFont;
      CLOSELOG( "OUT8.5" );
      if (fMenuLittleFont)
         delete fMenuLittleFont;
      CLOSELOG( "OUT8.6" );
      if (fMenuDescFont)
         delete fMenuDescFont;
      CLOSELOG( "OUT8.7" );
      if (fSystemFont)
         delete fSystemFont;
      CLOSELOG( "OUT8.8" );
      if (fNewGuiFont)
         delete fNewGuiFont;
      CLOSELOG( "OUT8.9" );
      if (fNewGuiBtnFont)
         delete fNewGuiBtnFont;
      if (fNewGuiBtnFontBig)
         delete fNewGuiBtnFontBig;
      if (fNewGuiBtnFont2)
         delete fNewGuiBtnFont2;
      if (fNewGuiBtnFont3)
         delete fNewGuiBtnFont3;
      CLOSELOG( "OUT8.10" );
      if (fNewGuiSpellFont)
         delete fNewGuiSpellFont;
      CLOSELOG( "OUT8.11" );
      if (fNewGuiInfoFont)
         delete fNewGuiInfoFont;
      if (fNewGuiInfoFont2)
         delete fNewGuiInfoFont2;



      CLOSELOG( "OUT9" );
      
	  // If there is a Message of the Day, delete it!
      if (MsgDay)
         delete MsgDay;
    }catch( ... ){
        CLOSELOG( "ERROR QUITTING APP." );
    }

    try{
		CLOSELOG( "OUT9.2" );
		//ShowWindow( siGethWnd(), SW_HIDE );        

        CLOSELOG( "OUT10" );
        // Display the Custom Error Message
        switch (app->dwErrNb) {
		case 1: {
         MessageBox(NULL, app->lpszErrStr, g_LocalString[410], MB_OK | MB_TOPMOST );
              } break;
        }

    }catch(...){
    }

    CLOSELOG( "OUT11" );

    // Delete all local String.
    g_LocalString   .Release();
    g_GUILocalString.Release();

    if(g_ListData)
       delete []g_ListData;
    g_ListData = NULL;
    

    // Close every unclose file handle.
    fcloseall();
   
    CLOSELOG( "OUT12" );

	// New delete
    g_V2Database.Release();

    g_boWaitForShutdown = false;	
	Sleep( 5000 );

	CLOSELOG( "OUTFAILED" );

    exit( -1236 );

    return 0;
}

//*********************************************************************************
void AppManagement::Close( char *szReason )
/**********************************************************************************
 * Close the application.
 *
 * @REMARK This function should always be call in order to terminate the 
 * application and make the proper clean up.
 */
{
    // Close Application       
    if (!g_boQuitApp) {
		CLOSELOG( "----------------" );
		CLOSELOG( szReason );

		LOG << "* CLS = " << szReason << "\r\n";
        g_boQuitApp = true;
        unsigned int nil;
        HANDLE hCloseThread = (HANDLE)_beginthreadex( NULL, 0, AsyncClose, this, 0, &nil );

		if( hCloseThread == NULL ){
			exit( -1234 );
		}else{
			//SetThreadPriority( hCloseThread, THREAD_PRIORITY_TIME_CRITICAL );
		}
		// Wait 30 seconds for the object to close.
		/*if( WaitForSingleObject( hCloseThread, 30000 ) == WAIT_TIMEOUT ){
			exit( -1235 );
		}*/
    }
}

//*********************************************************************************
void AppManagement::SetError
/**********************************************************************************
 * This function Set andError Number and an Error String to be displayed by the 
 * Close Method.
 */
(
 DWORD dwErr,    // Error Number
 char *lpszErr   // Error String
)
//*********************************************************************************
{
   if (!g_boQuitApp) {
		// Set Error # and Message.
		dwErrNb = dwErr;
		lpszErrStr = new char [strlen(lpszErr)+1];
		strcpy(lpszErrStr, lpszErr);
   }
}

//*********************************************************************************
void AppManagement::ExitThread
/**********************************************************************************
 * This method notify the Application Managr than a Core Thread has terminated. 
 * It is not currently supported to add custom thread to this method.
 */
(
 DWORD dwThread  // Internal Thread ID (Not the Window Thread ID)
)
//*********************************************************************************
{
   // Set the Flag to Exit Thread
   switch (dwThread) {
   case APP_MOUSE_THREAD:			MS_Exit  = true;	break;
   case APP_CD_THREAD:				CD_Exit  = true;	break;
   case APP_DRAWING_THREAD:		DT_Exit  = true;	break;
   case APP_MAINTENANCE_THREAD:	MT_Exit  = true;	break;
   case APP_SOUND_THREAD:			SMT_Exit = true;	break;
   case APP_VOLUME_THREAD:			SCT_Exit = true;	break;
      
   default:					  
      SetError(1, g_LocalString[393]); 
      Close(g_LocalString[393]); 
      break;
   }
}

//*********************************************************************************
void AppManagement::StartThread
/**********************************************************************************
 * This method notify the Application manager that a Core Thread has started. 
 * It is not currently supported to add custom thread to this method.
 */
(
 DWORD dwThread  // Internal Thread ID (Not the Window Thread ID)
)
//*********************************************************************************
{
   // Set the Flag to Start Thread
   switch (dwThread) {
   case APP_MOUSE_THREAD:			MS_Exit  = false;	break;
   case APP_CD_THREAD:				CD_Exit  = false;	break;
   case APP_DRAWING_THREAD:		DT_Exit  = false;	break;
   case APP_MAINTENANCE_THREAD:	MT_Exit  = false;	break;
   case APP_SOUND_THREAD:			SMT_Exit = false;	break;
   case APP_VOLUME_THREAD:			SCT_Exit = false;	break;
      
   default:					  
      SetError(1, g_LocalString[394]); 
      Close(g_LocalString[394]); 
      break;
   }
}

//*********************************************************************************
bool AppManagement::isFocus( void )
/**********************************************************************************
 * This method return the application focus state (set by the SetFocus method). 
 *
 * @RETURN <b>true</b> if the application has the focus, <b>false</b> if the 
 * application hasn't the focus.
 */
{
   // is the App on Focus ?
   return boFocus;
}

//*********************************************************************************
void AppManagement::SetFocus
/**********************************************************************************
 * This method tell the application manager if the application has the focus.
 */
(
 bool b          // Focus State
)
//*********************************************************************************
{
    
	return ;

    if( !loading ){//BLBLBL 26 mars 2009 tentative de toujours lock/unlock les threads quand on arrive en focus
        // Lock all principal threads.
        LOG << "\r\nLocking DrawThread.";
        LockDrawThread();    
        LOG << "\r\nLocking MainThread.";
        LockMainThread();    
        LOG << "\r\nLocking MouseThread.";
        LockMouseThread();
        LOG << "\r\nLocking PacketThread.";
        LockPacketThread();
    }else{
        LOG << "\r\nloading is TRUE";
    }

    if( b ){
        LOG << "\r\nSetting focus to true.";
    }else{
        LOG << "\r\nSetting focus to false.";
    }
  
    
    // Set the app Focus.
   boFocus = b; //BLBLBL freeze test
//      boFocus= true;

    if( !loading ){
        LOG << "\r\nUnLocking All.";
        // Release all principal threads.
        UnlockDrawThread();
        UnlockMainThread();
        UnlockMouseThread();
        UnlockPacketThread();
    }
	else if( b ){//BLBLBL  : 26 mars 2009 tentative de virer ça
      // If focus was set, reload the load.pcx image.
        IDirectXImage diLoading;

        try {
            // Load.PCX for 800 and 1024
            char strLOADName[20];
            sprintf(strLOADName,"LOAD%d.PCX",g_Global.GetScreenW());
            if (diLoading.LoadImage(strLOADName,g_Global.GetScreenW(),g_Global.GetScreenH())) 
            {
                DXDClear();
                DXDBlt(diLoading);
                DXDFlip();
                LOG << "* END = 0-2-a\r\n";
            }
        } catch (...) {
            LOG << "* WRN = LOAD.PCX\r\n";
        };
    }
}

//*********************************************************************************
AppVariable::AppVariable( void )
/**********************************************************************************
 * Constructor. Initialize global application variable to default state.
 */
{
   inGame           = false; 
   InvalideSaveGame = false;
   bMemory          = false; 
   minimizeState    = false;
   tWaitFPSTime     = 1000/17;
}

void AppManagement::LockDrawThread(){     drawLock.Lock(); }
void AppManagement::UnlockDrawThread(){   drawLock.Unlock(); }
void AppManagement::LockMainThread(){     mainLock.Lock(); }
void AppManagement::UnlockMainThread(){   mainLock.Unlock(); }
void AppManagement::LockMouseThread(){    mouseLock.Lock(); }
void AppManagement::UnlockMouseThread(){  mouseLock.Unlock(); }
void AppManagement::LockPacketThread(){   packetLock.Lock(); }
void AppManagement::UnlockPacketThread(){ packetLock.Unlock(); }

//////////////////////////////////////////////////////////////////////////////////////////
void AppManagement::SetLoading
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
(
 bool loadValue
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( loadValue ){
        LOG << "\r\nSetting loading value to true.";
    }else{
        LOG << "\r\nSetting loading value to false.";
    }
    loading = loadValue;
}

bool AppManagement::IsLoading(){ return loading; };