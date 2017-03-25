// ************************************************************************
// ***                                                                  ***
//      File Name: Main.cpp
//      Project:   The 4th Coming
//      Creation:  25 Mars, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***
//#include "pch.h"

#pragma warning( disable : 4786 )
#pragma warning( disable : 4291 )

// Number used to enable Debug
#define MAGIC_DEBUG_NUMBER   8030


// Include Files
#include <windows.h>
#include <shlobj.h>
#include "App.h"
#include "Bitmap.h"
#include "V2Database.h"
#include "DirectSocket.h"
#include "DirectXInput.h"
#include "Disp.h"
#include "Ddraw.h"
//#include "Effect.h"
#include <eh.h>
#include "ExpFltr.h"
#include "Fading.h"
#include "NewFont.h"
#include "Franck.h"
#include "Icon3D.h"
#include "IDirectXDraw.h"
#include "IDirectXFont.h"
#include "IDirectXImage.h"
#include "LocalString.h"
#include "GUILocalString.h"
#include "MacroHandler.h"
#include "Mouse.h"
#include <new.h>
#include "NTime.h"
#include "MemoryManager.h"
#include "ObjectListing.h"
#include "old.h"
#include "Packet.h"
#include "PowerDebug.h"
#include <process.h> 
#include "Random.h"
#include <ras.h>
#include "Resource.h"
//#include "Sprite.h"
#include <stdio.h>
#include "System.h"
#include "SystemAction.h"
#include "SystemInfo.h"
#include "TFCFlag.h"
#include "TFCObject.h"
#include "TFCPlayer.h"
#include "TFCSocket.h"
#include "TileSet.h"
#include "VisualObjectList.h"
#include "WarningBox.h"
#include <wininet.h>
#include "Global.h"
#include "C2.h"
#include "FileLogger.h"
#include "ProcStat.h"
#include "Logger.h"
#include "version.h"
#include "CombatCursor.h"
#include "Encrypter.h"
#include "SaveGame.h"
#include "VideoCapture.h"
#include "NMVideoCapture.h"
#include "Zlib/zlib.h"


#include "NewInterface/RootBoxUI.h"
#include "NewInterface/ChatterUI.h"
#include "NewInterface/InventoryUI.h"
#include "NewInterface/CharacterUI.h"
#include "NewInterface/GroupPlayUI.h"
#include "NewInterface/GuildUI.h"
#include "NewInterface/SpellUI.h"
#include "NewInterface/MacroUI.h"
#include "NewInterface/OptionsUI.h"
#include "NewInterface/EffectStatusUI.h"
#include "NewInterface/TradeUI.h"

#include "..\T4CLauncher\T4CLauncher.h"

#include "XpStat.h"
#include "PvpRanking.h"

#import "webpatch.exe"

// Scotch: Server history vars
std::vector<std::string>vServerHistory;

// Static Global
// Alt Tab
static BOOL inGameTab = FALSE; // Verify si le Code de ALT-TAB est utiliser pour un 
// ATL-TAB ou pour entrer dans le jeux.
static BOOL bGetID = FALSE;	 // Stock de ALT-TAB que j'vas modifer 
static DWORD dwGetID = 0;		 // Stock de ALT-TAB que j'vas modifier
//int Tabbing = 1;					 // Le Tabbing c le nb de fois que t'a fait ALT-TAB //BLBLB 26 mars 2009 désactivé ce truc outdated.
// Thread
HANDLE hMaintenanceThread = NULL; // Ca sert a-peu-pres juste pour le alt-tab.
extern DWORD TargetID;

// Global                      
// Exception Filter
// Unsorted
extern unsigned char key;
static MMRESULT timerid;
extern HANDLE hDrawThread;
extern HANDLE hMouseThread;
extern HANDLE hCDThread;
extern bool MenuThreadFinished;
int CTRL_State = 0;
bool SHIFT_State = false;
HWND g_hwnd;
Random rnd(10, 20);
static Random rnd2(0, 5);
extern BACK Backpack[24];
WarningBox WARNINGBOX;
Bitmap Grid;
Bitmap GridID;
Bitmap GridPriority;
Bitmap GridBlocking;
T3VSBSound SoundFX[250];
T3VSBSound ConnectSoundFX;
TFCSocket     Remote;
unsigned char msglist = 0;
volatile unsigned long Round = 0;
int Mx, My;
double    CriticalSection::OldPriority = 9999999;
char      CriticalSection::OldName[100];
char      CriticalSection::OldID[100];
char ServerIP[100];
char ServerResol[100];
char *      ServerPort;
sockaddr_in ServerIPaddr;
bool Register = false;
char TFC_State = TFC_CONNECT;
static char Try = 0;
extern bool AskForMenu;
Font *fFont = NULL;
Font *fSystemFont = NULL;
Font *fSkillFont  = NULL;
Font *fSkillFont2 = NULL;
Font *fNewGuiFont = NULL;
Font *fNewGuiBtnFont = NULL;
Font *fNewGuiBtnFontBig = NULL;
Font *fNewGuiBtnFont2 = NULL;
Font *fNewGuiBtnFont3 = NULL;
Font *fNewGuiInfoFont = NULL;
Font *fNewGuiInfoFont2 = NULL;
Font *fNewGuiSpellFont = NULL;
Font *fMenuFont   = NULL;
Font *fMenuLittleFont = NULL;
Font *fMenuDescFont = NULL;
Font *BucephalusFont = NULL;
bool ScreenDisplayThreadFinished = false;
bool MessageThreadFinished = false;
extern DirectXInput KeyBoard;
extern LPDIRECTINPUTDEVICE lpDIMouseDevice; // Direct Input Device
BOOL QuitFirstLoop = FALSE;
//extern char Text[1000];
char LastText[1000];
extern BOOL INGAME;
extern unsigned char Direction;
extern short TalkToOffset;
HWND GlobalHwnd;
char RegistrationCode[80];
BOOL MESSAGE_THREAD = TRUE;
BOOL MENU_THREAD = TRUE;
bool NoKey;
extern BOOL boKeyProcess;
extern int MEMORY1;
extern int MEMORY2;
extern HANDLE InterPacketEvent;
extern unsigned char *Packet;
char *MsgDay = NULL;
char  g_strGeneralMessage[100];
extern QWORD qwPacketUseCounter[150];
extern QWORD qwPacketUseTime[150];
extern QWORD qwPacketUsePeek[150];
DWORD ThreadITimeStamp = 0;
CProcStat g_TimeStat;
bool g_ForceDecompression = false;
char szCopyCmdLine[256];
static bool boMusicDecompressionCancelled = false;
extern int TalkToX, TalkToY;
extern DWORD TalkToID;

extern Global g_Global;
extern CSaveGame g_SaveGame;
extern NMVideoCapture g_NMVideoCapture;

extern unsigned char *g_ListData;
extern int            g_ListSize;

/// TEMP
extern bool g_DisplayLetter;
extern bool g_boWaitForShutdown;
// END TEMP

//New loading One Shoot Thread...
BOOL    g_bFirstLoadComplete = FALSE;
void    DrawFisrtLoadingText();
void    FirstInitObject(LPVOID pParam);

// Function
bool    WndClassInit   ( void ); // Initialisation de la Class Window.
bool    CreateWnd      ( void ); // Creation du Window.
void    InitializeTFCData(void); // Initialize all T4C Data
int     ReadConfigLanguage(char *);


// Unsorted
QWORD FileTime2QWord(FILETIME *f);
BOOL DecriptVSB(BOOL, int);
DWORD GetThreadTime(HANDLE);
void ParseText(char *pText);

void WebPatchUpdate(char *, char *, char *, char *, DWORD);

BOOL Save = FALSE;
BOOL Only = FALSE;
BOOL Graph = FALSE;
BOOL Windowed = FALSE;

BYTE DlgState;

char *MsgErr = NULL;
char MsgUrl[100];

bool boolGOA = false;

char TryNb = 0;

BOOL InSocket = FALSE;
int SocketState = 0;

UINT SocketThreadID = NULL;
HANDLE SocketThreadHandle = NULL;

int StLine = 0;
int EndLine = 0;
int NbLine = 0;

extern DWORD SpellID;


//NightMare -- Add 2005-02-15
HCURSOR m_hMouseTransCursor; //Contain Window Empty (transparent) Mouse Cursor

static BOOL    g_bExitComplete = FALSE;
void    ExitThreadGame(LPVOID pParam);

static BOOL    g_bRunCommand = FALSE;
static char    g_szFileName[512];
void    RunCommandThread(LPVOID pParam);

//BLBLBL 29 mars 2009 tentative de sauvegarde du T4C.DAT en WM_CLOSE
extern ClientConfig m_ClientConfig;
extern bool boInterruptLogoff;//BLBLBL antiplug variable
extern bool boForceLogoff;//BLBLBL antiplug variable



//by Asteryth Pvp Ranking
PvpRanking pvpranking;

//SHAINON XPSTAT
XpStat xpstat;


// Code
UINT WINAPI SocketFunction(LPVOID pParam);

UINT WINAPI MaintenanceThread(LPVOID pParam) 
{
	Remote.MainThread();// Thread de Maintenance.
	_endthreadex(1);
	return 1;
}

// Used to lock some macro keys.
static unsigned int EmptyCallback( void *null )
{
    return 0;
}

static UINT StartCapture( void *null )
{
	g_NMVideoCapture.StartCapture();
	ChatterUI::GetInstance()->AddBackscrollItem( "", "Starting VIDEO Capture...", SYSTEM_COLOR, true, false );
	return 0;
}

static UINT EndCapture( void *null )
{
    g_NMVideoCapture.StopCapture();
    ChatterUI::GetInstance()->AddBackscrollItem( "", "END of VIDEO Capture", SYSTEM_COLOR, true, false );
    return 0;
}
static UINT CommitCapture( void *null )
{
    return 0;
}
static UINT ScreenShot( void *null )
{
	if(Custom.TakeScreenShot)
		return 0;
	Custom.TakeScreenShot = TRUE;
	/*
	VideoCapture *pCap = new VideoCapture;
	pCap->TakeDesktopSnapshot();
	
	  ClientConfig *pConfig = g_Global.GetClientConfig();
	  char strScreenShotPath[1024];
	  sprintf(strScreenShotPath,"%s\\ScreenShot",pConfig->strSavePath);
	  CreateDirectory( strScreenShotPath, NULL );
	  pCap->SaveToFile(strScreenShotPath);
	  delete pCap;
	*/
	
	/*
	VideoCapture::GetInstance()->TakeDesktopSnapshot();
	CreateDirectory( "ScreenShot", NULL );
	VideoCapture::GetInstance()->SaveToFile("ScreenShot");
	*/
	
	ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 558 ], SYSTEM_COLOR, true, false );
	return 0;
}

static UINT IdentifyAllObject( void *null )
{
	Objects.IdentifyAll();
	return 0;
}

static UINT PasteClipboard( void *null )
{
	if (OpenClipboard(NULL)) 
	{
		if (::IsClipboardFormatAvailable(CF_TEXT) || ::IsClipboardFormatAvailable(CF_OEMTEXT))
		{
			// Retrieve the Clipboard data (specifying that 
			// we want ANSI text (via the CF_TEXT value).
			HANDLE hClipboardData = GetClipboardData(CF_TEXT);
			
			// Call GlobalLock so that to retrieve a pointer
			// to the data associated with the handle returned
			// from GetClipboardData.
			char *pchData = (char*)GlobalLock(hClipboardData);
			for(UINT i=0;i<strlen(pchData);i++)
				ChatterUI::GetInstance()->TextInput( pchData[i] );
			GlobalUnlock(hClipboardData);
		}
		CloseClipboard();
	}
	return 0;
}


void RestartT4C()
{
	char thisFile[ MAX_PATH ];
	GetModuleFileName( NULL, thisFile, MAX_PATH );
	
	string params( szCopyCmdLine );
	
	char appDir[ MAX_PATH ];
	GetCurrentDirectory( MAX_PATH, appDir );
	
	SHELLEXECUTEINFO shellInfo;
	shellInfo.cbSize = sizeof( SHELLEXECUTEINFO );
	shellInfo.fMask  = SEE_MASK_NOCLOSEPROCESS;
	shellInfo.hwnd   = NULL;
	shellInfo.lpVerb = "open";
	shellInfo.lpFile = thisFile; 
	shellInfo.lpParameters = params.c_str();
	shellInfo.lpDirectory  = appDir; 
	shellInfo.nShow = SW_SHOW;
	
	// If the shell executed correctly.
	ShellExecuteEx( &shellInfo );
}

void COMMCallBack(COMM_INTR_PROTOTYPE) 
{
	// Function callback lors du recoit d'un packet.
	TFCPacket *Msg = new TFCPacket; 					// Packet (ou Message)
	
	//   OutputDebugString("PAK RECU, traitement..");
	
	// Creation du Message a prtir du Buffer.
	Msg->SetBuffer(lpbBuffer, nBufferSize);
	
	/* Its no longer used. Replaced by new crypt.
	// Decription du Message.
	if ( !Msg->DecryptPacket(0) )
	{
	if ( !Msg->DecryptPacket(99999) ) 
	{
	return;
	}
	switch ( Msg->GetPacketID() ) 
	{ // only allow old-crypt key for registration proccess, then use the new one *only*.
	case 66:
	case 91:
	case 14:
	break;
	default:
	return;
	}
	}
	*/
	
	
	if (COMM.State == 1) 
	{
		// COMM Center est en Mode Syncronious. Les packets sont recut avec une loop while(receive())
		// pour du backward compatibility. On mets donc les packets dans une liste qui donnera le packet
		// a notre function receive().
		COMM.Lock();
		COMM.AddPacket(Msg);
		//TFCPacket *Msg2 = new TFCPacket; 					// Packet (ou Message)
		//memcpy(Msg2, Msg, sizeof(TFCPacket));
		short Type;
		Msg->Get((short *)&Type);
		Msg->Seek(-2/*-sizeof( short )*/, 1 );
		LOG << "CHECK PACKET TYPE [" << Type << "] [" << timeGetTime() << "]\r\n";
		COMM.Unlock();

	} 
	else 
	{
		//g_App.LockPacketThread();
		HandlePacket(Msg);// Handle Packet.
		delete Msg;// Delete Message.
		//g_App.UnlockPacketThread();//BLBL 08/12/2010 déplacé après le delete Msg
	} 
};


UINT WINAPI Decryption(LPVOID pParam) 
{
	if (!DecriptVSB(FALSE, TRUE)) 
	{
		SetDlgItemText(GlobalHwnd, IDC_MESSAGE, g_LocalString[189]);
		while (TRUE) 
			Sleep(15);
		return 0;
	}
	
	if (DecriptVSB(FALSE, FALSE)) 
	{
		EndDialog(GlobalHwnd, 0);
		return 0;
	} 
	else 
	{
		LOG << "Not enough space to play the game\r\n";
		char Temp[1000];
		DWORD Sector;
		DWORD Cluster;
		DWORD Byte;
		DWORD TotalCluster;
		GetDiskFreeSpace(NULL, &Sector, &Byte, &Cluster, &TotalCluster);
		sprintf(Temp, g_LocalString[182], (Byte*Sector*Cluster) / 1000000);
		SetDlgItemText(GlobalHwnd, IDC_MESSAGE, Temp );
		//EndDialog(GlobalHwnd, 0);
		while (TRUE) 
			Sleep(15);
		return 0;
	}
}

BOOL WINAPI Dlg_Procd2 (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	
	if (uMsg == WM_COMMAND) 
	{
		if ((wParam & 0xFFFF) == IDCANCEL) 
		{
			boMusicDecompressionCancelled = true;
			EndDialog(hwnd, 0);
		}
	}
	
	if (uMsg == WM_PAINT) 
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
	}
	
	if (uMsg == WM_INITDIALOG) 
	{
		GlobalHwnd = hwnd;
		InvalidateRect(hwnd, NULL, FALSE);
		UpdateWindow(hwnd);
		UINT ID;
		SetDlgItemText(hwnd, IDC_MESSAGE, g_LocalString[175] );
		SetDlgItemText(hwnd, IDC_BOX, g_LocalString[176] );
		SetDlgItemText(hwnd, IDCANCEL, g_LocalString[160] );
		SetWindowText(hwnd, g_LocalString[9]);
		_beginthreadex(NULL, 0, Decryption, NULL, NULL, &ID);
	}
	
	return FALSE;
}

void CorrectText(char *Name)          {
	unsigned int k = 0;
	for (unsigned int j = 0; j < strlen(Name); j++) {
		if (Name[j] != 10 && Name[j] != 13) {
			Name[j] = Name[k];
			k++;
		}
	}
	Name[k] = 0;
}

char RealName[200][500];
char Name[200][500];
char PatchV[200][500];
char Desc[200][500];
char Button[200][500];
char Web[200][500];
char LastOne[500];
char Login[500];
char Login2[500];
//char ProxyPatch[500];

int iGok = 0;

bool bSearchIntoVector(std::string str, std::vector<std::string>vStr)
{
	bool bRet = false;
	for(std::vector<std::string>::iterator i = vStr.begin(); i != vStr.end(); i++)
	{
		if( *i == str ) { bRet = true; }
	}
	return bRet;
}

WORD bColor = 0;

static long FAR PASCAL WindowMessageInput( 
                                          HWND hWnd, 
                                          UINT message, 
                                          WPARAM wParam, 
                                          LPARAM lParam 
                                          ) 
{
	//LOG.Lock();
	//LOG << "\r\nMainPump received message " << message << ", wParam=" << wParam << ", lParam="<<lParam << ".!";
	//LOG.Unlock();
	
	ThreadITimeStamp = timeGetTime();
	
	switch ( message ) 
	{
		
	case WM_SIZE :
		{
			if(Windowed)
			{
			/*
			if(g_SaveGame.GetOptionsParam()->bLockResize)
			{
			
			  LPRECT pRect  = (LPRECT)lParam;
			  return TRUE;
              
				break;
				}
				*/
			}
		}
		break;
		// NightMare -- Add 2005-02-17
		// When resizing in Windowed Mode Keep Aspect ratio
		// Valide All king of resizing...
		// 1.2756 == ratio factor Width / Height  648 / 508
		// 648 == 480 + 8pixels of border
		// 508 == 480 + 8pixels of border +20 pixels tilebars
	case WM_SIZING :
		{
            if(Windowed)
            {
				if(g_SaveGame.GetOptionsParam()->bLockResize)
				{
					RECT rc;
					GetWindowRect(siGethWnd(),&rc);
					LPRECT pRect  = (LPRECT)lParam;
					
					pRect->left   = rc.left;
					pRect->right  = rc.right;
					pRect->top    = rc.top;
					pRect->bottom = rc.bottom;
					break;
				}
				
				UINT   fwSide = wParam;
				LPRECT pRect  = (LPRECT)lParam;
				long lScrWidth  = GetSystemMetrics(SM_CXSCREEN);
				long lScrHeight = GetSystemMetrics(SM_CYSCREEN);
				switch(fwSide)
				{
				case WMSZ_BOTTOM :
					pRect->right = (int)(pRect->left +((pRect->bottom-pRect->top)*1.2756));
					if(pRect->right > lScrWidth)
					{
                        pRect->right  = lScrWidth;
                        pRect->bottom = (int)(pRect->top+((pRect->right-pRect->left)/1.2756));
					}
					break;
				case WMSZ_RIGHT :
					pRect->bottom = (int)(pRect->top+((pRect->right-pRect->left)/1.2756));
					if(pRect->bottom > lScrHeight)
					{
                        pRect->bottom  = lScrHeight;
                        pRect->right = (int)(pRect->left +((pRect->bottom-pRect->top) *1.2756));
					}
					break;
				case WMSZ_BOTTOMRIGHT :
					pRect->right = (int)(pRect->left +((pRect->bottom-pRect->top) *1.2756));
					if(pRect->right > lScrWidth)
					{
                        pRect->right  = lScrWidth;
                        pRect->bottom = (int)(pRect->top +((pRect->right-pRect->left)/1.2756));
					}
					pRect->bottom = (int)(pRect->top +((pRect->right-pRect->left)/1.2756));
					if(pRect->bottom > lScrHeight)
					{
                        pRect->bottom  = lScrHeight;
                        pRect->right = (int)(pRect->left +((pRect->bottom-pRect->top) *1.2756));
					}
					break;
				case WMSZ_TOP :
					pRect->left = (int)(pRect->right -((pRect->bottom-pRect->top) *1.2756));
					if(pRect->left < 0)
					{
                        pRect->left   = 0;
                        pRect->top = (int)(pRect->bottom -((pRect->right-pRect->left)/1.2756));
					}
					break;
				case WMSZ_LEFT :
					pRect->top = (int)(pRect->bottom -((pRect->right-pRect->left)/1.2756));
					if(pRect->top < 0)
					{
                        pRect->top  = 0;
                        pRect->left = (int)(pRect->right -((pRect->bottom-pRect->top) *1.2756));
					}
					break;
				case WMSZ_TOPLEFT :
					pRect->left = (int)(pRect->right -((pRect->bottom-pRect->top)*1.2756));
					if(pRect->left < 0)
					{
                        pRect->left   = 0;
                        pRect->top = (int)(pRect->bottom -((pRect->right-pRect->left)/1.2756));
					}
					pRect->top = (int)(pRect->bottom -((pRect->right-pRect->left)/1.2756));
					if(pRect->top < 0)
					{
                        pRect->top  = 0;
                        pRect->left = (int)(pRect->right -((pRect->bottom-pRect->top) *1.2756));
					}
					break;
				case WMSZ_BOTTOMLEFT :
					pRect->left = (int)(pRect->right -((pRect->bottom-pRect->top) *1.2756));
					if(pRect->left < 0)
					{
                        pRect->left   = 0;
                        pRect->bottom = (int)(pRect->top +((pRect->right-pRect->left)/1.2756));
					}
					pRect->bottom = (int)(pRect->top +((pRect->right-pRect->left)/1.2756));
					if(pRect->bottom > lScrHeight)
					{
                        pRect->bottom  = lScrHeight;
                        pRect->left = (int)(pRect->right -((pRect->bottom-pRect->top) *1.2756));
					}  
					break;
				case WMSZ_TOPRIGHT :
					pRect->right = (int)(pRect->left +((pRect->bottom-pRect->top)*1.2756));
					if(pRect->right > lScrWidth)
					{
                        pRect->right  = lScrWidth;
                        pRect->top = (int)(pRect->bottom -((pRect->right-pRect->left)/1.2756));
					}
					pRect->top = (int)(pRect->bottom 
                        -((pRect->right-pRect->left)/1.2756));
					if(pRect->top < 0)
					{
                        pRect->top  = 0;
                        pRect->right = (int)(pRect->left +((pRect->bottom-pRect->top) *1.2756));
					}
					break;
				}
            }
         }
         break;
		 // END NightMare
		 //////////////////////////////
		 
		 // NightMare -- Add 2005-02-15
		 // Verify windows Mouse change message and if Message 
		 // not on Client we Set Transparent Cursor and return TRUE;
		 // return TRUE not apply cursor modification...
		 // NightMare -- Update 2005-02-17
		 // remove variable bWindowedCursorStatus and add Custom.WindowedSizing
		 // to keep sizing status to block player when resizing
      case WM_SETCURSOR:
		  // Only on Windowed Mode...
		  if(Windowed)
		  {
			  // keep the last stat of cursor 
			  // do not change cursor if already transparent...
			  int dwHitTestCode = LOWORD(lParam); 
			  if(g_SaveGame.GetOptionsParam()->bLockResize && (dwHitTestCode >=10 && dwHitTestCode <=17))
				  return FALSE;
			  //int yPos = HIWORD(lParam); //BLBLBL 29 mars 2009 suppr. lignes inutilisée
			  if(dwHitTestCode == HTCLIENT  )
			  {
				  if(Custom.gWindowedSizing)
				  {
					  SetCursor(m_hMouseTransCursor);
					  Custom.gWindowedSizing = false;
				  }
				  return TRUE;
			  }
			  else
			  {
				  // Here Cursor can change... suppose to be on title bar or Border
				  Custom.gWindowedSizing = true;
			  }
		  }
		  break;
		  // END NightMare
		  //////////////////////////
		  
      case WM_DESTROY:
      case WM_QUIT:
		  {
			  g_App.Close("DESTROY");
		  } break;
		  
		  /*case MM_MCINOTIFY: 
		  {
		  if (wParam == MCI_NOTIFY_SUCCESSFUL)
		  PlayCDAudio(GlobalCDTrack, TRUE);
		  
			OutputDebugString("Receive NOTIFY\r\n");
      } break;*/
		  
		  
      /*2-2
      1
      
        2-2
        3
        2-1
		  */
		  
	  case WM_KILLFOCUS: 
		  {
			  LOG << "\r\nWM_KILLFOCUS";
			  if (inGameTab) {			
				  //    g_App.SetError(1, g_LocalString[402]);
				  //    g_App.Close(g_LocalString[402]);
				  // KILL FOCUS BLOCK
				  /*LOG << "* ALT = 1\r\n";           //BLBL: 26 mars tentative de réactivation du bloc
				  if (!wParam && !g_boQuitApp) {	    //la réactivation est une catastrophe.
				  QuitFirstLoop = TRUE;
				  if (hMaintenanceThread) {
                  if (hDrawThread) {
				  SuspendThread(hDrawThread);
                  }
				  } 
			  } */
			  } 
			  // END KILL FOCUS BLOCK
		  } break;
		  
	  case WM_SYSCOMMAND:
		  {
			  if(wParam == SC_MINIMIZE)
			  {
				  //            if(g_Var.inGame)//BLBL même si on est pas encore INGAME !
				  //{
				  g_Var.minimizeState = false;//BLBLBL test à false tout le temps.
				  g_Var.tWaitFPSTime  = 1000/Custom.NMFPSValue;//BLBLBL on garde le même taux de FSP tout le temps
				  //}
			  }
			  else// if(wParam == SC_MINIMIZE)
			  {
				  g_Var.minimizeState = false;
				  g_Var.tWaitFPSTime = 1000/Custom.NMFPSValue;
			  }
		  }
		  break;
		  
	  case WM_ACTIVATE:
		  {
			  LOG << "\r\nWM_ACTIVATE";
			  
			  if (inGameTab)
			  {
				  SetCursor(m_hMouseTransCursor);
				  Custom.gWindowedSizing = false;
				  if (bGetID) 
				  {
					  dwGetID = lParam;
					  bGetID = FALSE;
				  }
				  if ((!lParam || lParam == (long)dwGetID) && !g_boQuitApp) 
				  {
					  
					  BOOL fActive = wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE;
					  if (fActive) 
					  {
						  //if( !g_App.isFocus() )//BLBL Freeze TEST
						  //{
						  LOG << "\r\nENTERING FROM ALT-TAB";
						  if(!Custom.gWindowed)
						  {
							  DXDSetView();
							  DXDRestoreSurfaceF();
							  DXDRestoreSurface();
							  DXDSetView2();
						  }
						  // SET FOCUS ACTIVE
						  LOG << "* ALT = 2-1\r\n";
						  if (KeyBoard.IsCreated()) 
						  {
							  if (lpDIMouseDevice) 
							  {
							  /*if (siGetSystemOS() == SI_WIN_NT_40)  //BLBLB 26 mars 2009 désactivé ce truc outdated.
							  {
                              Tabbing+=2;
							  }*/
								  lpDIMouseDevice->Acquire();
							  }
							  LOG << "* DBG = Keyboard Acquire 1\r\n";
							  KeyBoard.Acquire();
						  }
						  g_App.SetFocus(true);
						  //}//BLBL Freeze TEST
						  // END SET FOCUS ACTIVE
					  } 
					  else 
					  {
						  LOG << "\r\nEXITING BY ALT-TAB";
						  // KILL FOCUS ACTIVE
						  LOG << "* ALT = 2-2\r\n";
						  //if (g_App.isFocus()) {//BLBL Freeze test
						  LOG << "* ALT = 2-3\r\n";                             
						  if (hMaintenanceThread && !g_boQuitApp)
						  {                         
							  //g_App.SetFocus(false);//BLBL Freeze test
							  if (lpDIMouseDevice) 
							  {
								  LOG << "* ALT = 2-4\r\n";
								  lpDIMouseDevice->Unacquire();
							  }
							  LOG << "* DBG = Keyboard Unacquire 1\r\n";
							  KeyBoard.Unacquire();
						  }
						  LOG << "* ALT = 2-5\r\n";
						  //}//BLBL Freeze test
						  // END KILL FOCUS ACTIVE
					  }
				  }
			  } 
		  } break;
		  
	  case WM_SETFOCUS:
		  {
			  LOG << "\r\nWM_SETFOCUS";
			  
			  if (inGameTab) {
				  // SET FOCUS BLOCK
				  if ((!wParam || wParam == dwGetID) && !g_boQuitApp) {
					  QuitFirstLoop = FALSE;
					  if (hMaintenanceThread) {
						  CTRL_State = 0; //indique que la touche control n'est pas enfoncée.
					  }
				  }
				  // END SET FOCUS BLOCK
			  }
		  } break;
		  
	  case WM_KEYUP:
		  {
			  LOG << "KEY UP: [" << (DWORD)wParam << "]\r\n";
			  // ESCAPE depressed
			  if (wParam == 17) {
				  CTRL_State = 0;
				  
			  }
			  // SHIFT depressed.
			  if( wParam == VK_SHIFT ){
				  SHIFT_State = false;
			  }         
		  } break;
		  
	  case WM_SYSKEYDOWN:
		  LOG << "\r\nWM_SYSKEYDOWN";
		  
		  /*
		  if (wParam == VK_F10) 
		  {
		  PostMessage(hWnd, WM_KEYDOWN, VK_F10, NULL);
		  PostMessage(hWnd, WM_KEYUP, VK_F10, NULL);
		  }
		  */
		  
		  if (wParam == 17) 
		  {
			  CTRL_State = 1;
			  
		  }
		  break; 
		  
	  case WM_SYSKEYUP:
		  if (wParam == 17) {
			  CTRL_State = 0;
		  }
		  break;
		  
	  case WM_KEYDOWN:
		  {
			  // Disable TAB key.
			  /*if (wParam == 9) 
			  {
			  return DefWindowProc(hWnd, message, wParam, lParam);            
		  }*/
			  
			  if (wParam == 17) {
				  CTRL_State = 1;
				  
			  }
			  if( wParam == VK_SHIFT )
			  {
				  SHIFT_State = true;
			  }
			  
			  NoKey = false;
			  switch (TFC_State) 
			  {
			  case TFC_MENU:                  case TFC_INTRODUCTION:  case TFC_CHOOSE_RACE: 
			  case TFC_MAKE_REROLL:           case TFC_CHOOSE_PLAYER: case TFC_WARNING: 
			  case TFC_OPTION:        case TFC_VIEW_CREDITS: 
			  case TFC_CONFIRM_DELETE:        case TFC_LOGO:          case TFC_INTRO_LINE: 
			  case TFC_ASK_CREATE_CHARACTER:  case TFC_CHOOSE_SEXX:   
			  case TFC_WARNING_2: 
				  {
					  if (wParam == VK_DOWN)
						  key = 200;
					  if (wParam == VK_UP)
						  key = 201;
					  if (wParam == VK_F10) 
						  return 1; // disable f10 key
				  } break;
				  
			  case TFC_PLAY:
				  {
					  if( 1)
					  {
						  if (g_DisplayLetter) 
						  {
							  g_DisplayLetter = false;
						  } 
						  else 
						  {
							  VKey vKey;
							  vKey.SetKey( wParam, CTRL_State != 0, SHIFT_State );
							  // If that's a valid vkey.
							  if( vKey.GetKey() != 0 )
							  {
								  if( !Custom.gMacro.CallMacro( vKey ) && !MacroUI::GetInstance()->CallMacro( vKey ) )
								  {
									  RootBoxUI::GetInstance()->VKeyInput( vKey );
								  }
								  else
									  g_Global.ResetItemInfo();
							  }
						  }
					  }
					  
					  switch (wParam) 
					  {
						  //case VK_NUMPAD1: case VK_NUMPAD2: case VK_NUMPAD3: case VK_NUMPAD4: case VK_NUMPAD5: 
						  //case VK_NUMPAD6: case VK_NUMPAD7: case VK_NUMPAD8: case VK_NUMPAD9: 
						  //case VK_END: case VK_NEXT: case VK_HOME:  case VK_PRIOR:   
						  
					  case VK_UP: case VK_DOWN: case VK_LEFT:    case VK_RIGHT: case VK_ESCAPE:
						  {
							  
							  NoKey = true;
							  
							  switch( wParam )
							  {
							  case VK_ESCAPE:
								  //RootBoxUI::GetInstance()->KbHit( wParam );
								  break;
							  default:
								  //boKeyProcess = !RootBoxUI::GetInstance()->KbHit( 0, wParam );
								  break;
							  }
						  } 
						  break;
						  
					  case VK_F1: 
						  {
							  
						  } break; 
						  
					  case VK_F2: 
						  {
						  /*
						  for (int i = 10001; i < 10800; i++) 
						  {
						  char Text[1000];
						  sprintf(Text, "Give Spell %d to Frost", i);
						  TFCPacket Send; 
						  
							Send << (short)RQ_IndirectTalk;
							Send << (long)Player.ID;
							Send << (char)0;
							Send << (long)Custom.gTextColor;
							Send << (short)strlen(Text);
							Send << (char *)Text;
							SEND_PACKET(Send); 
							} 
							  */
						  } 
						  break;
					  case VK_F3: 
						  break;
					  case VK_F4: 
						  break;
					  case VK_F5: 
						  break; 
					  case VK_F6: 
						  break;
					  case VK_F7: 
						  break;
					  case VK_F8: 
						  {
						  /*
						  if( Custom.NMDebug )
						  {
						  Objects.Lock(0);
						  Objects.DeleteAll();
						  Objects.Unlock(0);
						  }
							  */
						  } 
						  break;
					  case VK_F9: 
						  break;
					  case VK_F10: 
						  return 1; // Disable F10 Key
						  break;
						  
					  }
             } break;
        }
        } break;
        
        case 0x0109:
			char Temp[100];//BLBLBL affichage en mode debug des lettres tapées pour chercher le pb avec le coréen
			sprintf(Temp, "WM_UNICHAR : %u\r\n", wParam);//BLBLBL affichage en mode debug des lettres tapées pour chercher le pb avec le coréen
			OutputDebugString( Temp );//BLBL affichage en fenetre log de ce qui est recu
			break;
		case WM_CHAR:
			{
				// Disable TAB key.
				if (wParam == 9)
					return DefWindowProc(hWnd, message, wParam, lParam);
				
				switch (TFC_State) {
					
				case TFC_PLAY: 
					{
						if (NoKey) 
							break;
						if ( !CTRL_State || ( isprint( wParam ) || ( wParam > 128 && wParam < 256 ) ) ) //BLBL info : saisie des caractères dans T4C, filtrage sur le paramètre d'entrée
						{
							if (INGAME) 
							{
								
								char Temp[100];//BLBLBL affichage en mode debug des lettres tapées pour chercher le pb avec le coréen
								//	sprintf(Temp, "WM_CHAR : %u\r\n", wParam);//BLBLBL affichage en mode debug des lettres tapées pour chercher le pb avec le coréen
								//	OutputDebugString( Temp );//BLBL affichage en fenetre log de ce qui est recu
								
								if( RootBoxUI::GetInstance()->TextInput( wParam ) )
								{
									NoKey = true;
									break;
								}
								
								ChatterUI::GetInstance()->TextInput( wParam );
								NoKey = true;
							} 
							
							/*if ( !CTRL_State ) //BLBL bidouille à chaotik pour lire les lettres sur 2 bytes :
							{
							if (INGAME) 
							{
							if ((wParam & 0xFF00)==0) //standard char
							{
							if( RootBoxUI::GetInstance()->TextInput( wParam ) )
							{
							NoKey = true;
							break;
							}
							
							  ChatterUI::GetInstance()->TextInput( wParam );
							  
								} else //multi byte
								{
								
								  if( RootBoxUI::GetInstance()->TextInput( wParam>>8 ) )
								  {
								  if ( RootBoxUI::GetInstance()->TextInput( wParam & 0xFF ))
								  {
								  
									NoKey = true;
									break;
									}
									}
									
									  ChatterUI::GetInstance()->TextInput( wParam>>8);
									  ChatterUI::GetInstance()->TextInput( wParam & 0xff );
									  }
									  
										
										  NoKey = true;
						}*/
							
						/*                  #ifdef GRBX_CHAR
						BoxHandler.PostBoxMessage(GWIN_MSG_CHAR, wParam, NULL, NULL); break;
#endif  */
						}
					} break;
					
				case TFC_MENU: case TFC_INTRODUCTION: case TFC_CHOOSE_RACE: 
				case TFC_MAKE_REROLL: case TFC_CHOOSE_PLAYER: case TFC_WARNING:
				case TFC_OPTION: case TFC_VIEW_CREDITS: case TFC_CONFIRM_DELETE:
				case TFC_LOGO: case TFC_INTRO_LINE: case TFC_ASK_CREATE_CHARACTER:
				case TFC_WARNING_2: case TFC_CHOOSE_SEXX: 
					{
						if (!CTRL_State || !isalpha(wParam))
							key = tolower(wParam);
					} break;
					
				case TFC_CHOOSE_NAME:
					{
						key = wParam;
					} break;
				} 
				
			} break;
			
		case WM_CLOSE:
			{
				if(g_Var.inGame)
				{
					//int caca = 1;//BLBLBL 29 mars 2009 suppr. lignes inutilisée
					//int dwVal  = lParam;//BLBLBL 29 mars 2009 suppr. lignes inutilisée
					
					if(lParam == 0x6666/* || lParam == 0x1111*/)
					{
						
						if(!g_bExitComplete)
						{
							g_bExitComplete = TRUE;
							_beginthread( ExitThreadGame, 0, NULL );
						}
						OptionsUI::GetInstance()->HideQuitWindows();
						OptionsUI::GetInstance()->Hide();
						return FALSE;
					}
					else if(lParam == 0x6969)
					{
						//Save SaveGameCOnfig
						g_SaveGame.bSave();
						
						//BLBLBL 29 mars 2009 placement de la sauvegarde du T4C.DAT dans la WM_CLOSE : 
						g_Global.WriteClientConfig();
						
						
						/*				  TFCPacket Send;//BLBLBL reprise du pak de déco finale
						Send << (short)20;//qui apparement n'est jamais appellé dans app.cpp*/
						
						//Fermeture d l'application :
						g_App.Close(g_LocalString[403]);
					}
					else
					{
						g_Global.ResetItemInfo();
						OptionsUI::Display(NULL);
						OptionsUI::GetInstance()->ShowQuitWindows();
						return FALSE;
					}
					
				}
				else
				{
					g_App.Close(g_LocalString[403]);
					Sleep(1000);
					
				}
			} break;
			
			//      case WM_KILLFOCUS:
			//         exit(1);
			//      break;
   }
   
   ThreadITimeStamp = 0;
   
   return DefWindowProc(hWnd, message, wParam, lParam);
}

void CALLBACK TimerFunc( UINT nID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD d2);

using namespace WEBPATCHLib;
bool g_WebPatchSuccess = false;


// ******************************************************************************************* //
int WINAPI WinMain 
// ******************************************************************************************* //
// La Procedure Principale.
// 
(
 HINSTANCE hInstance,      // The Application Instance handle.
 HINSTANCE hPrevInstance,  // Instance of the same Application currently running.
 LPSTR     lpCmdLine,      // The Command line.
 int       nShowCmd        // Specified how the application should be shown.
 )
{
	char Path[256];
	GetModuleFileName(NULL, Path, 256);
	int len = strlen(Path) - 1;
	while (len && Path[len] != '\\')
		len--;
	Path[len] = 0;
	SetCurrentDirectory(Path); 
	
	DeleteFile( "Debug.log" );
	DeleteFile( "Client.gp" );
	
	
	ClientConfig *pConfig = g_Global.ReadClientConfig();
	
	Custom.Debug          = pConfig->dwDebug;
	Custom.NMDebug        = pConfig->dwNMDebug;
	Custom.NMShowPosition = pConfig->dwNMShowPOS;
	Custom.NMShowFPS      = pConfig->dwNMShowFPS;
	Custom.NMFPSValue     = pConfig->dwNMFPSValue;
	if(Custom.NMFPSValue <1 || Custom.NMFPSValue > 25)
		Custom.NMFPSValue = 16;
	//Custom.NMFPSValue = 32;
	g_Var.tWaitFPSTime     = 1000/Custom.NMFPSValue;
	Custom.TakeScreenShot  = 0;
	
	
	Custom.bEnableGameSnow   = false;
	Custom.bEnableGameFog   = false;
	Custom.bEnableGameRain  = false; 
	Custom.bEnableCArrow    = FALSE;
	
	//Custom.Debug = true; //BLBLBL 26 mars : mode débug activé.
	
#ifdef _DEBUG
	Custom.gWebPatchEnabled = false;//pConfig->WebpatchEnable;        
#else	//Disable webpatching for public builds @PoPo
	Custom.gWebPatchEnabled = false;//pConfig->WebpatchEnable;        
#endif
	
	//Custom.gWebPatchEnabled = false;
	
	
	// Name the current thread for the home-made Profiler 
	g_TimeStat.SetThreadIDName(GetCurrentThreadId(), "Message Thread");
	
	// TRY to Update DLL patchw32.DLL
	//CopyFile("NEWDLL.DLL", "PATCHW32.DLL", FALSE);//BLBLBL ?? sert à quoi ça !?
	
	// Set the Exception handler.
	CExpFltr &ExpFltr = CExpFltr::GetInstance();
	char Temp[1000];
	sprintf(Temp, "The 4th Coming Client %s", Version::GetVersionText() );
	ExpFltr.SetAppTitle(Temp);
	ExpFltr.SetReportFile("Client.GP");
	ExpFltr.InitFilter(NULL, 1);
	
	// Boost our process priority
	//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	
	
	// Variables.
	MSG msg;
	
	//g_LocalString   .Release();
	//g_LocalString   .Create();
	//g_GUILocalString.Release();
	//g_GUILocalString.Create();
	
	// System Information Initialisation.
	siCreate();
	siSethInstance(hInstance);
	siSetSystemOS();
	
	// Activate Memory Leak Logging.
	g_Var.bMemory = true;
	
	WndClassInit();      // Register the Window Class.
	CreateWnd();         // Create the Window.
	
	if( Custom.Debug)
		MessageBox(siGethWnd(), "Warning, this is an internal debug version. This version WILL run slower than your original T4Cversion.", "Warning!", MB_OK);
	
	LOG << "BUILD NUMBER " << *(Version::GetBuild()) << "\r\n";
	OutputDebugString("Init!\r\n");
	bGetID = TRUE;
	
	if (!g_boQuitApp)
		InitializeTFCData();
	if(g_boQuitApp)
		return true;
	
	// - Correction of small bug on LoadCursor Parameters...
	m_hMouseTransCursor = LoadCursor(siGethInstance(), MAKEINTRESOURCE(IDC_EMPTY_CURSOR)); 
	SetCursor(m_hMouseTransCursor);
	
	while ( !g_boQuitApp ) 
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
		{
			if ( !GetMessage( &msg, NULL, 0, 0 ) )
			{
				return msg.wParam;
			} 
			
			if( msg.message == WM_ACTIVATEAPP )
			{
				SetCursor(m_hMouseTransCursor);
			}
			
			//if(msg.hwnd)//XPSTAT :
			//{
            if (!TranslateAccelerator(msg.hwnd, NULL, &msg)) 
            {
				if(msg.message == WM_SYSKEYDOWN && msg.wParam == VK_F10)
				{
					PostMessage(msg.hwnd, WM_KEYDOWN, VK_F10, NULL);
					PostMessage(msg.hwnd, WM_KEYUP, VK_F10, NULL);
				}
				else
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
            }
			//}
		} 
		else 
		{
			WaitMessage();
		}
	}
	
	
	//Save SaveGameCOnfig
	//g_SaveGame.bSave();
	
	LOG << "\r\nQuit App!";   
	OutputDebugString( "\r\nWaiting for shutdown.\n\n" );
	int dwQuitCnt = 0;
	while( g_boWaitForShutdown && dwQuitCnt <50)
	{
		Sleep( 600 );
		dwQuitCnt++;
	}
	if(dwQuitCnt>=50)
	{
		OutputDebugString( "\r\n***********Pas eu le temps de fermer.....\n\n\n" );
		RemoveFontResource("Game Files\\t4cbeaulieux.ttf"); // steph Game Files au lieu de Fonts
		exit(5678);
		
	}
	g_boWaitForShutdown = 0;
	OutputDebugString( "\r\nClosing app.\n\n\n" );
	RemoveFontResource("Game Files\\t4cbeaulieux.ttf"); // steph Game Files au lieu de Fonts
	Sleep(500);
	siRelease();
	Sleep(500);
	//exit(0);
	return true;
}

// ******************************************************************************************* //
// Initialisation de la Class Window.
// Return:
//	  true   Initialisation Succefull.
//    false  Error with Initialisation.
//
// ******************************************************************************************* //
bool WndClassInit ( void )

{
	WNDCLASS wcex; // Window Class
	
	//	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_VREDRAW; // No Close, Redraw on Change.
	wcex.lpfnWndProc   = WindowMessageInput; // Callback function.
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = siGethInstance(); // Window Instance
	wcex.hIcon         = LoadIcon( siGethInstance(), MAKEINTRESOURCE(APP_ICON) ); // Application Icon //NightMare COrrect The name of Icons
	wcex.hCursor       = LoadCursor( NULL, IDC_ARROW ); // Application Arrow
	wcex.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH ); 
	wcex.lpszMenuName  = NULL;
	wcex.lpszClassName = "AppClass"; // Class Name
	//	wcex.hIconSm       = NULL; //LoadIcon( ug_siApp->GethInstance(), MAKEINTRESOURCE(IDI_APP_ICON) ); // Application Small Icon
	
	if ( !RegisterClass( &wcex ) ) 
	{
		GetLastError();
	}
	// Success.
	return true;
}


// ******************************************************************************************* //
// Creation du Window.
// Return:
//		true   Creation Succefull.
//    false  Error with Creation.
//
// ******************************************************************************************* //
bool CreateWnd ( void )
{
	HWND hWnd; // Window Handle
	
	hWnd = CreateWindow ( 
		"AppClass", // Window Class Name
		"The 4th Coming: Next Generation 1.68V2", // Application Name // steph T4C Rebirth 1.68V2 (16fps) au lieu de The 4th Coming
		WS_POPUP,// | WS_MAXIMIZE, // Style
		(GetSystemMetrics(SM_CXSCREEN)-g_Global.GetScreenW())/2, // left
		(GetSystemMetrics(SM_CYSCREEN)-g_Global.GetScreenH())/2, // top
		0, // left
		0, // top
		//		GetSystemMetrics(SM_CXSCREEN), // right
		//		GetSystemMetrics(SM_CYSCREEN), // bottom
		NULL,
		NULL,
		siGethInstance(), // Handle Instance
		NULL 
		);
	//ShowWindow(hWnd, SW_SHOW);
	
	if ( !hWnd ) 
	{
		// Cannot create the Window.
		GetLastError();
	}
	siSethWnd(hWnd);
	return true;
}


// True cursors!
CV2Sprite MouseCursor1;
//CombatCursorVSF MouseCursor2;
//CombatCursorVSF MouseCursor3;
AnimV2Sprite MouseCursor2;
AnimV2Sprite MouseCursor3;
CV2Sprite MouseCursor4;
CV2Sprite MouseCursor5;
CV2Sprite MouseCursor6;
CV2Sprite MouseCursor7;
CV2Sprite MouseCursor8;
CV2Sprite MouseCursor9;
CV2Sprite MouseCursor10;
CV2Sprite MouseCursor11;
CV2Sprite MouseCursor12;
CV2Sprite MouseCursor13;
CV2Sprite MouseCursor14;
CV2Sprite MouseCursor15;
AnimV2Sprite MouseCursor16;
AnimV2Sprite MouseCursor17;
AnimV2Sprite MouseCursor18;

CombatCursorVSF AttackCursorIcon;


//++NMNMNM --[NEW GUI CV2Sprite Sprite]
CV2Sprite    Intro[NM_CONNECT_SCREEN_NBR_BACK_ANIM];
CV2Sprite    Connect_D_Anim[36];
//CV2Sprite    Connect_Title;
CV2Sprite    Connect_Title2;
CV2Sprite    Connect_D;
CV2Sprite    Connect_V;
CV2Sprite    Connect_Main_Back;
CV2Sprite    Connect_Main_BtnN;
CV2Sprite    Connect_Main_BtnD;
CV2Sprite    Connect_Main_BtnH;
CV2Sprite    PS_Back;
CV2Sprite    PS_BackDown;
CV2Sprite    PS_Btn_N;
CV2Sprite    PS_Btn_H;
CV2Sprite    PS_SmallBtn_N;
CV2Sprite    PS_SmallBtn_H;
CV2Sprite    PS_Over;
CV2Sprite    PS_SBtnUp_N;
CV2Sprite    PS_SBtnUp_H;
CV2Sprite    PS_SBtnDn_N;
CV2Sprite    PS_SBtnDn_H;
CV2Sprite    Q_Back;
CV2Sprite    Q_BackSelect;
CV2Sprite    J_Back;
CV2Sprite    E_Back;


//CV2Sprite SplashP;
CV2Sprite    Splash;
CV2Sprite P, M;

// Webpatch
DWORD g_dwVersion;
char g_lpszIP[256];
char g_lpszUser[256];
char g_lpszPath[256];
char g_lpszPassword[256];


UINT WINAPI SocketFunction(LPVOID pParam) 
{
	if (InSocket) 
		return 0;
	InSocket = TRUE;
	OutputDebugString("Init Socket Func\r\n");
	DWORD LastTry = 0;
	TFCPacket *Msg;
	BOOL NotRegister = TRUE;
	WORD Type;
	DWORD Round = 1;//BLBLBL annulation modif, on sait pas si ça change qq chose ou pas//BLBLBL 29 mars 2009 suppr. lignes inutilisée
	BOOL boMsgDay = FALSE;
	
	COMM.State = 1;
	TryNb = 0;
	
	if (SocketState == 1) 
	{
		TFCPacket Send;
		Send.SetPacketSeedID(99999);
		Send << (short)66;
		SEND_PACKET(Send);
	}
	while (!boMsgDay && SocketState == 1) 
	{
		if (DlgState == 7) 
		{
			InSocket = FALSE;
			return 1;
		}
		
		if (timeGetTime() - LastTry > 1000 && NotRegister) 
		{
			LastTry = timeGetTime();
			TryNb++;
			if (TryNb == 10) //BLBLBL 30=>10
			{
				DlgState = 4;
				MessageBox(NULL, g_LocalString[430], g_LocalString[410], MB_OK);
			}
		}
		
		Sleep(5); //BLBLBL 25 mars 2009 de 15 à 5 pour voir.
		COMM.Lock();
		int Len = COMM.Receive(Msg);
		COMM.Unlock();
		if(Len > 0) 
		{		
			OutputDebugString("Receive Packet\r\n");
			// Packet is OK.
			Msg->Get((short *)&Type);
			
			LPBYTE lpbBuffer2;
			int nBufferSize2;
			
			Msg->GetBuffer(lpbBuffer2, nBufferSize2);
			
			if(Custom.NMDebug)
			{
				OutputDebugString("TEXT : \r\n");
				for (int i = 0; i < nBufferSize2; i++) 
				{
					char Temp[10];
					sprintf(Temp, "%c",lpbBuffer2[i]);
					OutputDebugString(Temp);
				}
				OutputDebugString("\r\n");
				OutputDebugString("HEXA : \r\n");
				for (int i = 0; i < nBufferSize2; i++) 
				{
					char Temp[100];
					sprintf(Temp, "[0x%02X]", lpbBuffer2[i]);
					OutputDebugString(Temp);
				}
				OutputDebugString("\r\n");
			}
			
			switch(Type) 
			{
				// Type of Packet
            case 66: 
				{
					OutputDebugString(" ** Receive MOTD!\r\n");
					sprintf(g_strGeneralMessage,g_GUILocalString[131]);
					short Len;
					char Lettre;
					int j = 0;
					boMsgDay = TRUE;
					Msg->Get((short *)&Len);
					MsgDay = new char [Len+500];
					for (int i = 0; i < Len; i++) 
					{
						Msg->Get((char *)&Lettre);
						if (Lettre == 10) 
						{
							
						} 
						else if (Lettre == 13) 
						{
							MsgDay[j] = ' ';
							j++;
							MsgDay[j] = '<';
							j++;
							MsgDay[j] = '>';
							j++;
						} 
						else 
						{
							MsgDay[j] = Lettre;
							j++;
						}
					}
					MsgDay[j] = 0;
					DlgState = 3;
				} break;
			};	
			LPBYTE lpbBuffer;
			int nBufferSize;
			Msg->GetBuffer(lpbBuffer, nBufferSize);
			delete Msg;
		}
	}
	
	TryNb = 0;
	
	while (NotRegister && SocketState == 2) 
	{
		if (timeGetTime() - LastTry > 1000 && NotRegister) 
		{
			LastTry = timeGetTime();
			TryNb++;
			if (TryNb == 5)//réduction du nombre d'essais à 5 (5 secondes) ça permet de rééssayer plus rapidement. 
			{
				DlgState = 4;
			}
			
			if (TryNb <= 1)  //BLBLBL un truc louche à regarder ici ! cette condition me semble foireuse vis à vis de la boucle
			{
				OutputDebugString("Send Registration\r\n");
				sprintf(g_strGeneralMessage,g_GUILocalString[170]);
				LastTry = timeGetTime();
				TFCPacket Send;
				Send.SetPacketSeedID(99999);
				Send << (short)91;//RQ_Register
				SEND_PACKET(Send);
			}
		}
		
		COMM.Lock();
		int Len = COMM.Receive(Msg);
		COMM.Unlock();
		if(Len > 0) 
		{		
			Msg->Get((short *)&Type);
			
			switch(Type) 
			{
				// Type of Packet
            case 91:
				{
					CoInitialize( NULL );
					Player.Version = (unsigned short)Version::GetVersion();
					
					char Temp[256];
					WORD wLen;
					WORD wLanguage;
					
					Msg->Get((long *)&g_dwVersion);
					
					Msg->Get((short *)&wLen);
					for (int i = 0; i < wLen; i++)
						Msg->Get((char *)&Temp[i]);
					Temp[wLen] = 0;
					strcpy(g_lpszIP, Temp);
					
					Msg->Get((short *)&wLen);
					for (int i = 0; i < wLen; i++)
						Msg->Get((char *)&Temp[i]);
					Temp[wLen] = 0;
					strcpy(g_lpszPath, Temp);
					
					Msg->Get((short *)&wLen);
					for (int i = 0; i < wLen; i++)
						Msg->Get((char *)&Temp[i]);
					Temp[wLen] = 0;
					strcpy(g_lpszUser, Temp);
					
					Msg->Get((short *)&wLen);
					for (int i = 0; i < wLen; i++)
						Msg->Get((char *)&Temp[i]);
					Temp[wLen] = 0;
					strcpy(g_lpszPassword, Temp);
					
					Msg->Get((short *)&wLanguage);
					
					
					/*
					
					  LOG << "* LNG = " << Player.szLanguage << "\r\n";
					  
						g_LocalString   .Release();
						g_LocalString   .Create();
						g_GUILocalString.Release();
						g_GUILocalString.Create();
					*/
					
					try 
					{      
						IWebPatchPtr lpWebPatch(__uuidof(WebPatch));
					} 
					catch (_com_error e) 
					{
						
						char szFullPath[256];
						char szDownloadDir[256];
						char szAppDir[256];
						char szFrom[256];
						char szTo[256];
						char szAppName[256];
						
						GetModuleFileName(NULL, szFullPath, 256);      
						strcpy(szAppDir, szFullPath);
						for(int i = strlen(szAppDir)-1; i > 0; i--) 
						{
							if (szAppDir[i] == '\\') 
							{
								szAppDir[i] = NULL;
								strcpy(szAppName, szAppDir+i+1);
								i = 0;
							}
						}
						strcpy(szDownloadDir, szAppDir);
						strcat(szDownloadDir, "\\Download");
						
						strcpy(szFrom, szDownloadDir);
						strcat(szFrom, "\\Webpatch.Exe");
						
						strcpy(szTo, szAppDir);
						strcat(szTo, "\\Webpatch.Exe");
						
						int i = 0;
						OutputDebugString("\r\n\nCOPY WEB PATCH: ");
						sprintf(g_strGeneralMessage,g_GUILocalString[132]);
						
						OutputDebugString(szFrom);
						OutputDebugString(", ");
						OutputDebugString(szTo);
						OutputDebugString("\r\n\n");
						while (!CopyFile(szFrom, szTo, FALSE) && i < 80 ) {
							Sleep(250);//BLBLBL huh ? 250 ?
							i++;
						}
						ShellExecute(siGethWnd(), "open", "webpatch.exe", "-regserver", "",SW_HIDE);
						Sleep(500);
					}
					if (g_boQuitApp) 
					{
						DlgState = 6;
						return 1;
					}
					
					TFCPacket Send;
					Send.SetPacketSeedID(99999);
					
					Send << (short)14;//RQ_RegisterAccount
					Send << (char)strlen(Player.Account);
					Send << (char *)Player.Account;
					Send << (char)strlen(Player.Password);
					Send << (char *)Player.Password;
					Send << (short)Player.Version;
					Send << (short)0;
					
					SEND_PACKET(Send);
					
            } break;
			
            case TFCRegisterAccount: 
				{
					OutputDebugString("Receive Registration\r\n");
					sprintf(g_strGeneralMessage,g_GUILocalString[133]);
					unsigned char Validation;
					Msg->Get((char *)&Validation);
					switch (Validation) 
					{
					case TFCRegisterAccount_Registred: 
						{ // Account registred
							if ( Custom.gWebPatchEnabled ) 
							{
								WebPatchUpdate(g_lpszPath, g_lpszIP, g_lpszUser, g_lpszPassword, g_dwVersion);
							}
							if (g_boQuitApp) 
							{
								DlgState = 6;
								return 1; 
							}
							LOG << "Register\r\n";
							OutputDebugString("Registering\r\n");
							
							TFCPacket Send;                     
							Send << (RQ_SIZE)RQ_AuthenticateServerVersion;
							Send << (long)Player.Version;
							SEND_PACKET(Send);
						} 
						break;
						
					case TFCRegisterAccount_WrongPassword: 
						{ // Account refused, wrong pwd
							LOG << "Wrong Password\r\n";
							NotRegister = FALSE;
							EnableWindow(GetDlgItem(GlobalHwnd, IDOK), TRUE);
							OutputDebugString("Receive Wrong Password\r\n");
							sprintf(g_strGeneralMessage,g_GUILocalString[134]);
							short Len;
							char Lettre;
							int j = 0;
							Msg->Get((short *)&Len);
							MsgErr = new char [Len+1];
							for (int i = 0; i < Len; i++) 
							{
								Msg->Get((char *)&Lettre);
								if (Lettre == 10) 
								{
								} 
								else if (Lettre == 13) 
								{
									MsgErr[j] = ' ';
									j++;
									MsgErr[j] = '<';
									j++;
									MsgErr[j] = '>';
									j++;
								} 
								else 
								{
									MsgErr[j] = Lettre;
									j++;
								}
							}
							MsgErr[j] = 0;
							DlgState = 2;
						} 
						break;
						
					case 2: 
						{
							LOG << "Other, Resent!\r\n";
							Sleep(1000);
							TFCPacket Send;
							Send.SetPacketSeedID(99999);
							
							Send << (short)14;//RQ_RegisterAccount
							Send << (char)strlen(Player.Account);
							Send << (char *)Player.Account;
							Send << (char)strlen(Player.Password);
							Send << (char *)Player.Password;
							Send << (short)Player.Version;
							Send << (short)0;
							
							SEND_PACKET(Send);
						} 
						break;
					} 
				} 
				break;
            case RQ_AuthenticateServerVersion:
				{
					OutputDebugString("Received server version authentication\r\n");
					sprintf(g_strGeneralMessage,g_GUILocalString[135]);
					unsigned long Valid = 0;
					Msg->Get((long*)&Valid);
					
					if( Valid == 1 )
					{
						// Set state to 6, enter game.
						DlgState = 6;
						NotRegister = FALSE;
					}
					else
					{
						NotRegister = FALSE;
						EnableWindow(GetDlgItem(GlobalHwnd, IDOK), TRUE);
						OutputDebugString("Receive Wrong Version\r\n");
						sprintf(g_strGeneralMessage,g_GUILocalString[136]);
						MsgErr = new char [ strlen( g_LocalString[ 542 ] ) + 1 ];
						strcpy( MsgErr, g_LocalString[ 542 ] );
						DlgState = 2;
					}
				} 
				break;
            case TFCStillConnected: 
				{
					TFCPacket Send;
					LOG << "\r\nSending code 10.";
					
					Send << (short)10;//RQ_Ack
					
					SEND_PACKET(Send);
				} 
				break;
				
            default: 
				{
					LOG << "Unknow Packet\r\n";
					char Temp[100];
					OutputDebugString("Unknow packet type [");
					OutputDebugString(itoa(Type, Temp, 10));
					OutputDebugString("]\r\n");
				} 
				break;
				
         };	
         LPBYTE lpbBuffer;
         int nBufferSize;
         Msg->GetBuffer(lpbBuffer, nBufferSize);
         delete Msg;
      }
   };
   NotRegister = TRUE;
   InSocket = FALSE;
   SocketThreadHandle = NULL;
   return 1;
}

void InitializeTFCData(void) 
{
	
	Custom.gFirstConnect = TRUE;
	LOG << "1, ";
	/////////////////////////////////
	/* //NMDELNM
	if (!RegistryKey.Open(HKEY_CURRENT_USER, "Software\\Dialsoft\\T4C_V2")) 
	{
	LOG << "2, ";
	OutputDebugString("**** CREATING REGISTRY ****\r\n");
	RegistryKey.Create(HKEY_CURRENT_USER, "Software\\Dialsoft\\T4C_V2");
	}
	*/
	
	
	LOG << "3, ";
	
	char Path[256];
	GetModuleFileName(NULL, Path, 256);
	int len = strlen(Path) - 1;
	while (len && Path[len] != '\\')
		len--;
	Path[len] = 0;
	
	ClientConfig *pConfig = g_Global.GetClientConfig();
	
	
	int dwLanguage = ReadConfigLanguage(pConfig->strSavePath);
	
	g_LocalString   .Release();
	g_LocalString   .Create(dwLanguage);
	g_GUILocalString.Release();
	g_GUILocalString.Create(dwLanguage);
	
	
	LOG << "8, ";
	
	Custom.gfirstTimeAddOn = false;
	if(pConfig->FirstTimeAddon == 1)
		Custom.gfirstTimeAddOn = true;
	pConfig->FirstTimeAddon = 0;
	
	LOG << "10, ";
	GlobalCDHandle = siGethWnd();
	//OpenCDAudio();
	
	LOG << "12, ";
	/*   if (FALSE && QueryCDAudio()) 
	{
	LOG << "13, ";
}*/
	
	LOG << "14, ";
	
	/*if (g_SaveGame.GetOptionsParam()->bUseCD) 
	{
	PlayCDAudio(7, TRUE); // bye default play track 7
	}
	LOG << "15, ";*/
	
	//unsigned char CodeC[5] = { 120, 87, 123, 98, 8 };//BLBLBL 29 mars 2009 suppr. lignes inutilisée
	
	LOG << "16, ";
	
	
	LOG << "17, ";
	char Temp[100];
	UINT i;
	for (i = 0; i < strlen(ServerIP); i++) {
        Temp[i] = ServerIP[i];
	}
	Temp[i] = 0;
	
	LOG << "18, ";
	
	ug_T3SoundManager = new T3SoundManager;   
	
	LOG << "19, ";
	int iMusicRetry = 0;
	do 
	{
        LOG << "20, ";
        std::string errMsg;                    
        if (!ug_T3SoundManager->Create(siGethWnd(), 44100, 16, 2, errMsg)) 
        {
			LOG << "\r\nT3SoundManager2::Create() failed! Error string: " << (char*)errMsg.c_str() << ".";
			if (siGetSystemOS() == SI_WIN_NT_40) 
			{
				LOG << "NT BOX\r\n";
				iMusicRetry = MessageBox(siGethWnd(), g_LocalString[543], g_LocalString[504], MB_OKCANCEL);
			} 
			else 
			{
				LOG << "WIN 9X BOX\r\n";
				iMusicRetry = MessageBox(siGethWnd(), g_LocalString[503], g_LocalString[504], MB_RETRYCANCEL);
			}
        }
	} while (iMusicRetry == IDRETRY);
	if (iMusicRetry == IDCANCEL && siGetSystemOS() == SI_WIN_NT_40) 
	{
        g_App.Close("User abort on NT4 (Music not initiazed).");
        exit(0);
	}
	
	LOG << "21, ";
	
	if (g_ForceDecompression) 
	{
        LOG << "22, ";
        DialogBox(siGethInstance(), MAKEINTRESOURCE(IDD_DIALOG2), siGethWnd(), Dlg_Procd2);
        
        if( !boMusicDecompressionCancelled ){
			if (g_ForceDecompression)
				exit(777);
			
			MessageBox(siGethWnd(), g_LocalString[438], g_LocalString[437], MB_OK);
			RestartT4C();
        }
        SendMessage(siGethWnd(), WM_CLOSE, 0, 0x6969);
        return;
	}		
	
	LOG << "23, ";
	
	if (g_ForceDecompression)
        exit(777);
	
	
	LOG << "24, ";
	
	char *lpszDialogTitle = new char[ strlen(g_LocalString[9]) + strlen(Version::GetVersionText()) + 5];
	sprintf( lpszDialogTitle, "%s :: %s", g_LocalString[9], Version::GetVersionText() );
	sprintf(g_strGeneralMessage,g_GUILocalString[130]);
	CT4CLauncher *pLauncher = new CT4CLauncher();
	pLauncher->CreateNMDialog(lpszDialogTitle,(char*)Version::GetVersionText(),pConfig->strPath,pConfig->strSavePath,pConfig->strAccountName,pConfig->strAccountIP);
	UINT dwAction = 0;
	DlgState = 7;
	int DlgStateTmp = DlgState;
	while(pLauncher->IsLuncherAvailable() && dwAction != 1)
	{
        //Process les option du Launcher ICI...
        pLauncher->ProcessMessage();
        dwAction = pLauncher->GetLastAction();
		
        
        switch(dwAction)
        {
		case LAST_CONNECT_ACTION_EXIT: //Action Quitter on fais rien :S
			g_boQuitApp = TRUE;
            break;
		case LAST_CONNECT_ACTION_CONENCT: //case Connection...
			if (DlgState == 7) 
			{
				// Dois recup les info de connection...
				// et les config du jeux...
				sprintf(ServerIP,"%s",pLauncher->GetIP());
				g_Global.SetDisplaySize(pLauncher->GetWidth(),pLauncher->GetHeight());
				//g_Global.SetDisplaySize(640,480);
				ServerPort = strtok(ServerIP, ":");
				ServerPort = strtok(NULL, ":"); 
				COMM.Close();
				COMM.SetIPAddr(ServerIP);
				if (ServerPort)
					COMM.SetAddrPort(atoi(ServerPort));
				else
					COMM.SetAddrPort(11677);
				if (!COMM.Create(COMMCallBack)) 
				{
					DlgState    = 5;
					DlgStateTmp = 8;
					FormatText HelpText2;
					HelpText2.SetText(g_LocalString[84]);
					HelpText2.Format(60);
					for(int i=0;i<HelpText2.GetnbLine();i++)
                        pLauncher->AddConnectMessage(HelpText2.GetText(i),i);
				} 
				else 
				{
					DlgState    = 8;
					DlgStateTmp = 8;
					FormatText HelpText2;
					HelpText2.SetText(g_LocalString[248]);
					HelpText2.Format(60);
					for(int i=0;i<HelpText2.GetnbLine();i++)
                        pLauncher->AddConnectMessage(HelpText2.GetText(i),i);
					
					SocketState = 1;
					SocketThreadHandle = (HANDLE) _beginthreadex(NULL, 0, SocketFunction, NULL, NULL, &SocketThreadID);
				}
			}
			else if(DlgState == 2 || DlgState == 3)
			{
				Windowed  = pLauncher->GetWindowed();
				sprintf(Player.Account  ,"%s",pLauncher->GetLogin());
				sprintf(Player.Password ,"%s",pLauncher->GetPassword());
				
				switch (pLauncher->GetLanguage()) 
				{
				case 0:  strcpy(Player.szLanguage, "English"); break; 
				case 1:  strcpy(Player.szLanguage, "French"); break; 
				case 2:  strcpy(Player.szLanguage, "Italian"); break; 
				case 3:  strcpy(Player.szLanguage, "Portugal"); break; 
				case 4:  strcpy(Player.szLanguage, "Spanish"); break; 
				case 5:  strcpy(Player.szLanguage, "German"); break; 
				case 6:  strcpy(Player.szLanguage, "Korean"); break; 
				default: strcpy(Player.szLanguage, "English"); break; 
				}
				
				if (DlgState == 2)
					DlgState = 1;
				pLauncher->ForceConnectState(FALSE);
				SocketState = 2;
				SocketThreadHandle = (HANDLE) _beginthreadex(NULL, 0, SocketFunction, NULL, NULL, &SocketThreadID);
			}else{//BLBLBL ajout pour voir si on peut redemande la connexion en recliquant le bouton
				
				DlgState = 7;
				//sprintf(g_strGeneralMessage,g_LocalString[98]);
				pLauncher->AddGeneralMessage("Connection attempt canceled.");
				pLauncher->AddConnectMessage("Please try again",0);
				
			}
            break;
				case LAST_CONNECT_ACTION_RESET:
					DlgState = 7;
					sprintf(g_strGeneralMessage,g_GUILocalString[130]);
					pLauncher->AddGeneralMessage(g_strGeneralMessage);
					pLauncher->AddConnectMessage("",0);
					break;
				case LAST_CHANGE_LANGUAGE:
					g_LocalString   .Release();
					g_LocalString   .Create(pLauncher->GetLanguage());
					g_GUILocalString.Release();
					g_GUILocalString.Create(pLauncher->GetLanguage());
					
					break;
				default :
					if(DlgStateTmp != DlgState)
					{
						DlgStateTmp = DlgState;
						
						FormatText HelpText2;
						int i;
						switch(DlgState)
						{
						case 2: 
							pLauncher->ForceConnectState(TRUE);
							HelpText2.SetText(MsgErr);
							HelpText2.Format(60);
							for(int i=0;i<HelpText2.GetnbLine();i++)
								pLauncher->AddConnectMessage(HelpText2.GetText(i),i);
							break;
						case 3:
							if (MsgDay) 
							{
								pLauncher->AddConnectMessage("",0);
								pLauncher->ForceConnectState(TRUE);
								HelpText2.SetText(MsgDay);
								HelpText2.Format(60);
								for(int i=0;i<HelpText2.GetnbLine();i++)
									pLauncher->AddConnectMessage(HelpText2.GetText(i),i,TRUE);
							} 
							else 
							{
								DlgState    = 1;
								DlgStateTmp = 1;
								
								SocketState = 2;
								SocketThreadHandle = (HANDLE) _beginthreadex(NULL, 0, SocketFunction, NULL, NULL, &SocketThreadID);
							} 
							break;
						case 4:
							HelpText2.SetText(g_LocalString[83]);
							HelpText2.Format(60);
							for(int i=0;i<HelpText2.GetnbLine();i++)
								pLauncher->AddConnectMessage(HelpText2.GetText(i),i);
							break;
						case 6: 
							dwAction = 1;// On quit l'application
							break;
						}
					}
					break;
        }
        pLauncher->AddGeneralMessage(g_strGeneralMessage);
        Sleep(50);
     }
     
     pLauncher->FreeNMDialog();
     sprintf(pConfig->strAccountName,"%s",pLauncher->GetAccountLogin());
     sprintf(pConfig->strAccountIP  ,"%s",pLauncher->GetAccountIP());
     delete pLauncher;
     delete lpszDialogTitle;
	 
     //g_boQuitApp = 1;
     LOG << "25, ";
     if (g_boQuitApp) 
     { 
		 LOG << "On quit le app\r\n";
		 if (ug_T3SoundManager)
			 delete ug_T3SoundManager;
		 ug_T3SoundManager = NULL;
		 g_LocalString   .Release();
		 g_GUILocalString.Release();
		 exit(0);
     }
	 
     
     
     LOG << "26, ";
     FILE *index = fopen("Game Files\\MD.MD", "rb");
     if (index  || g_ForceDecompression) 
     {
		 if (index)
			 fclose(index);
		 DialogBox(siGethInstance(), MAKEINTRESOURCE(IDD_DIALOG2), siGethWnd(), Dlg_Procd2);
		 
		 if( !boMusicDecompressionCancelled )
		 {
			 if (g_ForceDecompression)
				 exit(777);
			 MessageBox(siGethWnd(), g_LocalString[438], g_LocalString[437], MB_OK);
			 RestartT4C();
		 }
		 SendMessage(siGethWnd(), WM_CLOSE, 0, 0x6969);
		 return;
     } 
     else
     {
		 LOG << "27, ";
		 FILE *hFile;
		 hFile = fopen( "Game Files\\T4CGameFile.VSB", "rb");
		 FILE *hFile2 = fopen( "Game Files\\VSBInfo.Txt", "rb");
		 int fileSize = 0;
		 BYTE bBuffer[256];
		 
		 if (hFile != NULL && hFile2 != NULL) 
		 {
			 LOG << "Verify VSB Integrity.\r\n";
			 
			 fgets((char *)bBuffer, 256, hFile2);
			 
			 int iValue = atoi((char *)bBuffer);
			 if (iValue) 
			 {
				 
				 fseek(hFile, 0, SEEK_END);
				 fileSize = ftell(hFile);
				 
				 fclose(hFile);
				 fclose(hFile2);
				 
				 if (fileSize != iValue) 
				 {                  
					 DialogBox(siGethInstance(), MAKEINTRESOURCE(IDD_DIALOG2), siGethWnd(), Dlg_Procd2);
					 if( !boMusicDecompressionCancelled )
					 {
						 if (g_ForceDecompression)
							 exit(777);
						 
						 MessageBox(siGethWnd(), g_LocalString[438], g_LocalString[437], MB_OK);
						 RestartT4C();                    
					 }
					 SendMessage(siGethWnd(), WM_CLOSE, 0, 0x6969);
					 return;
				 } 
			 } 
			 else 
			 {
				 fclose(hFile);
				 fclose(hFile2);
			 }
		 } 
		 else 
		 {
			 LOG << "VSB Need to be decompressed.\r\n";
			 if (hFile != NULL)
				 fclose(hFile);
			 if (hFile2 != NULL)
				 fclose(hFile2);
			 
			 DialogBox(siGethInstance(), MAKEINTRESOURCE(IDD_DIALOG2), siGethWnd(), Dlg_Procd2);
			 if( !boMusicDecompressionCancelled )
			 {
				 if (g_ForceDecompression)
					 exit(777);
				 MessageBox(siGethWnd(), g_LocalString[438], g_LocalString[437], MB_OK);
				 RestartT4C();
			 }            
			 SendMessage(siGethWnd(), WM_CLOSE, 0, 0x6969);
			 return;
		 }
		 
		 LOG << "29, ";
		 LPBYTE MemTest = new BYTE [32000000];
		 if (!MemTest) 
		 {
			 DialogBox(siGethInstance(), MAKEINTRESOURCE(IDD_DIALOG2), siGethWnd(), Dlg_Procd2);
			 if (g_boQuitApp)
				 return;
		 }
		 delete MemTest;
		 if (g_boQuitApp) 
		 {
			 SendMessage(siGethWnd(), WM_CLOSE, 0, 0x6969);
			 return;
		 }
     }	
     
     /*if (Custom.NMDebug) 
     {
	 //NMNMNM
	 time_t maintenant;
	 struct tm *m;
	 time(&maintenant);
	 m= localtime(&maintenant);
	 
	   if(((m->tm_year+1900)  > 2006 || (m->tm_mon+1) >10 ) || ((m->tm_mon+1) ==10 && m->tm_mday > 15))
	   {
	   MessageBox(siGethWnd(), "Your release is Expired !!!", "Error !", MB_OK);
	   exit(0);
	   } 
     }*/
     
     LOG << "30, ";
     if (g_ForceDecompression)
		 exit(777);
     
     LOG << "31, ";
     if (MsgDay) 
     {
		 delete MsgDay;
		 MsgDay = NULL;
     }
     
     
     LOG << "32, ";
     
     // NightMare -- Add 2005-02-17
     // When resizing in Windowed Mode Keep Aspect ratio
     Custom.gWindowedSizing = false; //default init
     // END NightMare
     ///////////////////////////
	 
     Custom.gTextColor       = RGB(190, 190, 0);
     if (Windowed)
     {
		 Custom.gWindowed = true;
		 SetWindowLong( siGethWnd(), GWL_STYLE, WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU ); // Set titlebar, min/max/restore buttons and sysmenu // steph WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU au lieu de WS_OVERLAPPEDWINDOW
     }
     else
     {
		 Custom.gWindowed = false;
     }
     
     LOG << "* LOG = " << (char *)Player.Account << "\r\n";
     LOG << "* LOG = " << (int)Player.Version << "\r\n";
     
     memset(&ServerIPaddr, 0, sizeof(sockaddr));	
     ServerIPaddr.sin_family = AF_INET;
     ServerIPaddr.sin_addr.s_addr = inet_addr((LPCTSTR)ServerIP);
     ServerIPaddr.sin_port = htons(11677);
	 
	 
     //Load le launcher Config pour loader les macro voulu...
     char strConfig[512];
     sprintf(strConfig,"%s\\V2Launcher.dat",pConfig->strSavePath);
     
     FILE *pf1 = fopen(strConfig,"rb");
     if(!pf1)
     {
		 Custom.gMacro.AddNewMacro( VKey( 'I', true ), InventoryUI::Display, NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'S', true ), CharacterUI::Display, NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'C', true),  AttackMode, NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'L', true ), ChatterUI::Display,   NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'G', true ), GroupPlayUI::Display, NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'P', true ), SpellUI::Display,     NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'M', true ), MacroUI::Display,     NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'O', true ), OptionsUI::Display,   NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'W', true ), ShowWorldMap,		 NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'T', true ), TradeUI::Display,	 NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'H', true ), ScreenShot,   NULL );//VK_SNAPSHOT
		 Custom.gMacro.AddNewMacro( VKey( 'A', true ), ChatterUI::ToggleChatSize,		 NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'V', true ), IdentifyAllObject,		 NULL );
		 //Custom.gMacro.AddNewMacro( VKey( 'N', true ), PasteClipboard   , NULL );
		 
		 Custom.g_dwNbrCCColor = 8;
		 Custom.g_pNewCCColor = new int[Custom.g_dwNbrCCColor];
		 Custom.g_pNewCCColor[0] = RGB(17,181,205);
		 Custom.g_pNewCCColor[1] = RGB(255,33,75);
		 Custom.g_pNewCCColor[2] = RGB(90,119,216);
		 Custom.g_pNewCCColor[3] = RGB(220,202,6);
		 Custom.g_pNewCCColor[4] = RGB(5,197,37);
		 Custom.g_pNewCCColor[5] = RGB(199,10,204);
		 Custom.g_pNewCCColor[6] = RGB(192,192,192);
		 Custom.g_pNewCCColor[7] = RGB(224,100,10);
     }
     else
     {
		 
		 ULONG dwTailleOri;
		 ULONG dwTailleComp;
		 BYTE *pFileDataTmp = NULL;
		 BYTE *pFileDataTmpC = NULL;
		 
		 fread(&dwTailleOri,4,1,pf1);
		 fread(&dwTailleComp,4,1,pf1);
		 pFileDataTmp = new BYTE[dwTailleOri];
		 pFileDataTmpC= new BYTE[dwTailleComp];
		 fread(pFileDataTmpC,1,dwTailleComp,pf1);
		 fclose(pf1);
		 
		 if(uncompress((BYTE*)pFileDataTmp,&dwTailleOri,(BYTE*)pFileDataTmpC,dwTailleComp) != Z_OK)
			 MessageBox(NULL,"Erreur UnCompress Connection","",MB_OK);
		 
		 unsigned char *pchBuff = pFileDataTmp;
		 
		 BOOL bMacro[12];
		 BYTE chMacroKey[12];
		 
		 pchBuff+=(20+22);
		 memcpy(&bMacro          ,pchBuff,4*12); pchBuff+= (4*12);
		 memcpy(&chMacroKey      ,pchBuff,12  ); pchBuff+= 12;
		 memcpy(&Custom.g_dwNbrCCColor,pchBuff,4); pchBuff+=4;
		 if(Custom.g_dwNbrCCColor)
		 {
			 Custom.g_pNewCCColor = new int[Custom.g_dwNbrCCColor];
			 for(int i=0;i<Custom.g_dwNbrCCColor;i++)
			 {
				 int dwColor;
				 memcpy(&dwColor,pchBuff,sizeof(int)); pchBuff+=sizeof(int);
				 Custom.g_pNewCCColor[i] = dwColor;
			 }
		 }
		 
		 
		 delete []pFileDataTmp;
		 delete []pFileDataTmpC;
		 
		 if(bMacro[0])
			 Custom.gMacro.AddNewMacro( VKey( chMacroKey[0], true ), InventoryUI::Display, NULL );
		 if(bMacro[1])
			 Custom.gMacro.AddNewMacro( VKey( chMacroKey[1], true ), CharacterUI::Display, NULL );
		 if(bMacro[2])
			 Custom.gMacro.AddNewMacro( VKey( chMacroKey[2], true ), ChatterUI::Display, NULL );
		 if(bMacro[3])
			 Custom.gMacro.AddNewMacro( VKey( chMacroKey[3], true ), GroupPlayUI::Display, NULL );
		 if(bMacro[4])
			 Custom.gMacro.AddNewMacro( VKey( chMacroKey[4], true ), SpellUI::Display, NULL );
		 if(bMacro[5])
			 Custom.gMacro.AddNewMacro( VKey( chMacroKey[5], true ), MacroUI::Display, NULL );
		 if(bMacro[6])
			 Custom.gMacro.AddNewMacro( VKey( chMacroKey[6], true ), TradeUI::Display, NULL );
		 if(bMacro[7])
			 Custom.gMacro.AddNewMacro( VKey( chMacroKey[7], true ), OptionsUI::Display, NULL );
		 if(bMacro[8])
			 Custom.gMacro.AddNewMacro( VKey( chMacroKey[8], true ), ScreenShot, NULL );
		 if(bMacro[9])
			 Custom.gMacro.AddNewMacro( VKey( chMacroKey[9], true ), ChatterUI::ToggleChatSize,NULL );
		 
		 Custom.gMacro.AddNewMacro( VKey( 'C', true),  AttackMode       , NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'W', true ), ShowWorldMap     , NULL );
		 Custom.gMacro.AddNewMacro( VKey( 'V', true ), IdentifyAllObject, NULL );
		 //Custom.gMacro.AddNewMacro( VKey( 'N', true ), PasteClipboard   , NULL );
     }
     
	 
     ///////////////////////////////////////////////////////////////
	 
     ///Custom.gMacro.AddNewMacro( VKey( 'I', true ), InventoryUI::Display, NULL );
     ///Custom.gMacro.AddNewMacro( VKey( 'S', true ), CharacterUI::Display, NULL );
     ///Custom.gMacro.AddNewMacro( VKey( 'L', true ), ChatterUI::Display,   NULL );
     ///Custom.gMacro.AddNewMacro( VKey( 'G', true ), GroupPlayUI::Display, NULL );
     ///Custom.gMacro.AddNewMacro( VKey( 'P', true ), SpellUI::Display,     NULL );
     ///Custom.gMacro.AddNewMacro( VKey( 'M', true ), MacroUI::Display,     NULL );
     ///Custom.gMacro.AddNewMacro( VKey( 'T', true ), TradeUI::Display, 	 NULL );
     ///Custom.gMacro.AddNewMacro( VKey( 'O', true ), OptionsUI::Display,   NULL );
     ///Custom.gMacro.AddNewMacro( VKey( 'H', true ), ScreenShot,   NULL );//VK_SNAPSHOT
     
     ///Custom.gMacro.AddNewMacro( VKey( 'C', true),  AttackMode, NULL );
     ///Custom.gMacro.AddNewMacro( VKey( 'W', true ), ShowWorldMap,		 NULL );
     //Custom.gMacro.AddNewMacro( VKey( 'A', true ), ShowActualMap,		 NULL );
	 
     
     {
		 //Custom.gMacro.AddNewMacro( VKey( 'B', true ), StartCapture, NULL );
		 //Custom.gMacro.AddNewMacro( VKey( 'N', true ), EndCapture, NULL );
     }
     
     
     //DC
     
     LOG.Lock();
     LOG << "\r\nStatusUI Display address=" << (DWORD)CharacterUI::Display << ";";
     LOG.Unlock();
     
     //   g_hwnd = m_hWnd;
     time_t CurrentTime;
     time(&CurrentTime);
     LOG << "BUILD NUMBER " << *(Version::GetBuild()) << "\r\n";
     LOG << "* LOG = " << ctime(&CurrentTime) << "\r\n";
     
     // Change the registry key under which our settings are stored.
     // You should modify this string to be something appropriate
     // such as the name of your company or organization.
     
     
     //	IDirectXWave  Wave;
     
     // NightMare -- Add 2005-02-15
     // Make validation, if not on Windowed Mode, Hide Mouse Cursor
     // and If on Windowed Mode, hot Hide cursor, a transparent Cursor
     // is use now...
     if(!Custom.gWindowed)
     {
		 int h = ShowCursor(false);
		 while (h > 0)
			 h = ShowCursor(false);
     }
     // END NightMare
     //////////////////////////////
     
     lpDXDirectDraw = new IDirectXDraw; // Init DirectDraw ptr.
     // Init DirectX Draw
     if (DXDCreate(siGethWnd(), 0) != DD_OK) {
		 g_App.SetError(1, g_LocalString[154]);
		 g_App.Close("Error in initialization");
		 return;
     }
     
     
     // MARCHE PAS SIBOIRE
     
     FontHandler *fh = new FontHandler;
     UniqueFontHandler = fh;
     fh->Initialize();
	 
     FontManager::GetInstance()->Initialize();
     
     
     char FontName[100];
     double FontSize;
     
     ZeroMemory(FontName, 100);
     FontSize = 1;
     
     strcpy(FontName, "T4C Beaulieux");
	 
	 
     fFont				 = fh->GetFont(FontName);
     fSkillFont		 = fh->GetFont(FontName);
     fSkillFont2      = fh->GetFont(FontName);
     BucephalusFont  = fh->GetFont(FontName);
     fMenuFont		 = fh->GetFont(FontName);
     fMenuLittleFont = fh->GetFont(FontName);
     fMenuDescFont   = fh->GetFont(FontName);
     fSystemFont     = fh->GetFont("Tahoma");//BLBL System=>Tahoma 
     fNewGuiFont     = fh->GetFont("Arial");
     fNewGuiBtnFont  = fh->GetFont(FontName);
     fNewGuiBtnFontBig  = fh->GetFont(FontName);
     fNewGuiBtnFont2  = fh->GetFont(FontName);
     fNewGuiBtnFont3  = fh->GetFont(FontName);
     fNewGuiSpellFont= fh->GetFont(FontName);
     fNewGuiInfoFont = fh->GetFont("Verdana");
     fNewGuiInfoFont2 = fh->GetFont(FontName);
	 
	 
     
     Objects.Lock(0);
     Objects.lmPlayerLight     .Create("TorchPlayer.rgb"     , g_Global.GetMainTorchW(), g_Global.GetMainTorchH());
     Objects.lmOtherPlayerLight.Create("TorchOtherPlayer.rgb", 160, 120);
     Objects.lmTorchLight      .Create("Torch.rgb", 160, 120);
     Objects.lmTorchLanterne1  .Create("TorchLanterne1.rgb", 160, 130);
     Objects.lmTorchPoutre     .Create("TorchPoutre.rgb", 80, 90);
     Objects.lmTorchChandelle2 .Create("TorchChandelle2.rgb", 80, 90);
     Objects.Unlock(0);
     
     double TempFontHeight;
     HDC hdc;
     BucephalusFont->Create();
     DXDGetDC(&hdc, 44);
     TempFontHeight = (double)BucephalusFont->GetHeight(hdc);
     DXDReleaseDC(hdc, 44);
     fMenuFont->SetSize((32.0*FontSize)/TempFontHeight);
     fMenuFont->Create();
     fMenuLittleFont->SetSize((24.0*FontSize)/TempFontHeight);
     fMenuLittleFont->Create();
     fMenuDescFont->SetSize((22.0*FontSize)/TempFontHeight);	
     fMenuDescFont->Create();
     fFont->SetSize((22.0*FontSize)/TempFontHeight);
     fFont->Create();
     fSkillFont->SetSize((14.0*FontSize)/TempFontHeight);
     fSkillFont->Create();
     fSkillFont2->SetSize((16.0*FontSize)/TempFontHeight);
     fSkillFont2->Create();
     fSystemFont->SetSize((8.0*FontSize)/TempFontHeight);
     fSystemFont->Create();
     
     fNewGuiBtnFont->SetSize(5,23,600);
     fNewGuiBtnFont->Create();
	 
     fNewGuiBtnFontBig->SetSize(6,20,600);
     fNewGuiBtnFontBig->Create();
	 
     fNewGuiBtnFont3->SetSize(6,18,600);
     fNewGuiBtnFont3->Create();
	 
     fNewGuiBtnFont2->SetSize(6,26,800);
     fNewGuiBtnFont2->Create();
	 
     fNewGuiInfoFont->SetSize(6,14);
     fNewGuiInfoFont->Create();
	 
     fNewGuiInfoFont2->SetSize(6,16);
     fNewGuiInfoFont2->Create();
	 
     fNewGuiSpellFont->SetSize(6,16,800);
     fNewGuiSpellFont->Create();
	 
     fNewGuiFont->SetSize(8,12);
     fNewGuiFont->Create();
     
     IDirectXImage diLoading;
     HRESULT hrLoading;
     try 
     {
		 //Isp.PCX for 800 or 1024...
		 char strISPName[20];
		 sprintf(strISPName,"ISP%d.PCX",g_Global.GetScreenW());
		 hrLoading = diLoading.LoadImage(strISPName,g_Global.GetScreenW(),g_Global.GetScreenH());
		 if (hrLoading) 
		 {
			 DXDBlt(diLoading);
			 LOG << "* END = 0-2-d\r\n";
			 //diLoading.Release();
		 }
     } catch (...) 
     {
		 LOG << "* WRN = ISP.PCX\r\n";
     };
	 
     g_bFirstLoadComplete = FALSE;
     _beginthread( FirstInitObject, 0, NULL );
	 
     while(!g_bFirstLoadComplete)
     {        
		 if(hrLoading)
			 DXDBlt(diLoading);
		 DrawFisrtLoadingText();
		 DXDFlip();
		 Sleep(100);
     }
	 
	 Sleep(2000);//on laisse le message did you know 2 secondes de plus à l'écran
	 
     if(hrLoading != NULL)//BLBLBL 29 mars 2009 vérification pointeur initialisé
		 diLoading.Release();
     
     // Loading Game Font.
     
     // Set World Information.
     World.SetTileDimension   (32,   16  );
     World.SetViewDimension   (g_Global.GetNbrTileX(),   g_Global.GetNbrTileY());
     Grid.SetBitmap           (g_Global.GetNbrTileX()+1, g_Global.GetNbrTileY()+1  );
     GridID.SetBitmap         (g_Global.GetNbrTileX()+1, g_Global.GetNbrTileY()+1  );
     GridPriority.SetBitmap	 (g_Global.GetNbrTileX()+1, g_Global.GetNbrTileY()+1  );
     GridBlocking.SetBitmap   (g_Global.GetNbrTileX()+1, g_Global.GetNbrTileY()+1  );
     World.SetVirtualDimension(256,  256);
     World.SetWorldDimension  (3072, 3072);
     World.SetNumberofWorld   (4);	//Was set to 4 // steph 4 au lieu de 8
     World.SetWorldFileName   ("Game Files\\V2_WorldMap.Map"); // steph désactivation
     World.SetWorldFileName   ("Game Files\\V2_DungeonMap.Map");
     World.SetWorldFileName   ("Game Files\\V2_CavernMap.Map");
     World.SetWorldFileName   ("Game Files\\V2_Underworld.Map");
     //World.SetWorldFileName   ("Game Files\\V2_LeoWorld.Map");	//New map by Leo for Dialsoft
     //World.SetWorldFileName   ("Game Files\\V2_Extension01.Map");	
     //World.SetWorldFileName   ("Game Files\\V2_Extension02.Map");	
     //World.SetWorldFileName   ("Game Files\\V2_Extension03.Map");	
     World.CreateView();
	 
     AttackCursorIcon.LoadSprite("StaticAttackCursor");
     
     MouseCursor1.LoadSprite("64kInterfaceCursor");
	 
	 
     MouseCursor2.CreateSprite("AttackCursor00");
     MouseCursor2.AddFrame( "AttackCursor01" );
     MouseCursor2.AddFrame( "AttackCursor02" );
     MouseCursor2.AddFrame( "AttackCursor03" );
     MouseCursor2.AddFrame( "AttackCursor04" );
     MouseCursor2.AddFrame( "AttackCursor05" );
     MouseCursor2.AddFrame( "AttackCursor06" );
     MouseCursor2.AddFrame( "AttackCursor07" );
     MouseCursor2.AddFrame( "AttackCursor08" );
	 
     MouseCursor3.CreateSprite("64kCursorBow-a");
     MouseCursor3.AddFrame( "64kCursorBow-b" );
     MouseCursor3.AddFrame( "64kCursorBow-c" );
     MouseCursor3.AddFrame( "64kCursorBow-d" );
     MouseCursor3.AddFrame( "64kCursorBow-e" );
     MouseCursor3.AddFrame( "64kCursorBow-f" );
     MouseCursor3.AddFrame( "64kCursorBow-g" );
     MouseCursor3.AddFrame( "64kCursorBow-h" );
     MouseCursor3.AddFrame( "64kCursorBow-i" );
     MouseCursor3.AddFrame( "64kCursorBow-j" );
     MouseCursor3.AddFrame( "64kCursorBow-k" );
	 
	 
	 
     
     MouseCursor4.LoadSprite("64kInterfaceCursor"); // Talk
     MouseCursor14.LoadSprite("64kInterfaceCursor"); // Use
     MouseCursor15.LoadSprite("64kInterfaceCursor");  // Get
     MouseCursor5.LoadSprite("North Cursor");
     MouseCursor6.LoadSprite("North-East Cursor");
     MouseCursor7.LoadSprite("East Cursor");
     MouseCursor8.LoadSprite("South-East Cursor");
     MouseCursor9.LoadSprite("South Cursor");
     MouseCursor10.LoadSprite("South-West Cursor");
     MouseCursor11.LoadSprite("West Cursor");
     MouseCursor12.LoadSprite("North-West Cursor");
     
     MouseCursor13.LoadSprite("64kInterfaceCursor");// set a 64k cursor instead of 8bit
     //MouseCursor13.LoadSprite("_Intro Finger");   // this is the old cursor no more used
	 
     MouseCursor16.CreateSprite("TakeCursor00");
     MouseCursor16.AddFrame( "TakeCursor01" );
     MouseCursor16.AddFrame( "TakeCursor02" );
     MouseCursor16.AddFrame( "TakeCursor03" );
     MouseCursor16.AddFrame( "TakeCursor04" );
     MouseCursor16.AddFrame( "TakeCursor05" );
     MouseCursor16.AddFrame( "TakeCursor06" );
     MouseCursor16.AddFrame( "TakeCursor07" );
     MouseCursor16.AddFrame( "TakeCursor08" );
     MouseCursor16.AddFrame( "TakeCursor09" );
     MouseCursor16.AddFrame( "TakeCursor10" );
	 
     MouseCursor17.CreateSprite("TalkCursor00");
     MouseCursor17.AddFrame( "TalkCursor00" );
     MouseCursor17.AddFrame( "TalkCursor00" );
     MouseCursor17.AddFrame( "TalkCursor00" );
     MouseCursor17.AddFrame( "TalkCursor01" );
     MouseCursor17.AddFrame( "TalkCursor01" );
     MouseCursor17.AddFrame( "TalkCursor01" );
     MouseCursor17.AddFrame( "TalkCursor01" );
     MouseCursor17.AddFrame( "TalkCursor02" );
     MouseCursor17.AddFrame( "TalkCursor02" );
     MouseCursor17.AddFrame( "TalkCursor02" );
     MouseCursor17.AddFrame( "TalkCursor02" );
     MouseCursor17.AddFrame( "TalkCursor03" );
     MouseCursor17.AddFrame( "TalkCursor03" );
     MouseCursor17.AddFrame( "TalkCursor03" );
     MouseCursor17.AddFrame( "TalkCursor03" );
	 
     MouseCursor18.CreateSprite("DoorCursor00");
     MouseCursor18.AddFrame( "DoorCursor01" );
     MouseCursor18.AddFrame( "DoorCursor02" );
     MouseCursor18.AddFrame( "DoorCursor03" );
     MouseCursor18.AddFrame( "DoorCursor04" );
     MouseCursor18.AddFrame( "DoorCursor05" );
     MouseCursor18.AddFrame( "DoorCursor06" );
     MouseCursor18.AddFrame( "DoorCursor07" );
     MouseCursor18.AddFrame( "DoorCursor08" );
     MouseCursor18.AddFrame( "DoorCursor09" );
     MouseCursor18.AddFrame( "DoorCursor10" );
     MouseCursor18.AddFrame( "DoorCursor11" );
	 
     
     
     g_Var.vsfDisk     .CreateSprite("DiskAccess");
     g_Var.vsfMap      .CreateSprite("MapLoading");
     g_Var.vsfNetwork  .CreateSprite("NetworkAccess");
     g_Var.vsfProgressB.CreateSprite("EnterProgressBar");
     g_Var.vsfProgressT.CreateSprite("EnterProgressTick");
     g_Var.vsfMapZone  .CreateSprite("MapZoneDisplay");
     g_Var.vsfCode[0]  .CreateSprite("Marc");
     g_Var.vsfCode[1]  .CreateSprite("Lemming");
     g_Var.vsfCode[2]  .CreateSprite("Destiny");
     g_Var.vsfCode[3]  .CreateSprite("crush");
     g_Var.vsfCode[4]  .CreateSprite("Heschal");
     g_Var.wDisplayDisk    = 0;
     g_Var.wDisplayMap     = 0;
     g_Var.wProgressCnt    = 0;
     g_Var.wDisplayMapZone = 0;
     g_Var.wProgressInc = 1;
     
	 
     /*
     SplashP.LoadPalette("T4COptionP");
     Splash.lpRefPal = SplashP.lpSpritePal;
     Splash.LoadSprite("T4COption");
     */
	 
     char strNameTmp2[128];
     //char strNameTmpP[128];
     sprintf(strNameTmp2,"T4COption%d",g_Global.GetScreenW());
     //sprintf(strNameTmpP,"T4COption%dP",g_Global.GetScreenW());
     //SplashP.LoadPalette(strNameTmpP);
     //Splash.m_pRefDataPal = SplashP.lpSpritePal;
     Splash.LoadSprite(strNameTmp2);
	 
	 
     
     //++NMNMNM --[NEW GUI CV2Sprite Sprite]
     char strNameTmp[128];
     for(int aa=0;aa<NM_CONNECT_SCREEN_NBR_BACK_ANIM;aa++)
     {
		 sprintf(strNameTmp,"",aa+1,g_Global.GetScreenW()); // steph désactivation Back%02d_%d
		 Intro [aa].LoadSprite(strNameTmp);
     }
	 
     for(int aa=0;aa<36;aa++)
     {
		 sprintf(strNameTmp,"",aa); // steph désactivation logo36000%02d
		 Connect_D_Anim[aa].LoadSprite(strNameTmp);
     }
	 
     Connect_Title2    .LoadSprite("Connect_Title2");
     Connect_D         .LoadSprite(""); // steph désactivation Connect2_Dialsoft
	 
     Connect_V         .LoadSprite(""); // steph désactivation Connect2_Vircom
     Connect_Main_Back .LoadSprite("Connect_Main_Back");
     Connect_Main_BtnN .LoadSprite("Connect_Main_BtnN");
     Connect_Main_BtnD .LoadSprite("Connect_Main_BtnD");
     Connect_Main_BtnH .LoadSprite("Connect_Main_BtnH");
     PS_Back           .LoadSprite("PS_Back");
     PS_BackDown       .LoadSprite("PS_BackDown");
     PS_Btn_N          .LoadSprite("PS_BtnN");
     PS_Btn_H          .LoadSprite("PS_BtnH");
     PS_SmallBtn_N     .LoadSprite("PS_SmallBtnN");
     PS_SmallBtn_H     .LoadSprite("PS_SmallBtnH");
     PS_Over           .LoadSprite("PS_Over");
     PS_SBtnUp_N       .LoadSprite("PS_SBtnUpN");
     PS_SBtnUp_H       .LoadSprite("PS_SBtnUpH");
     PS_SBtnDn_N       .LoadSprite("PS_SBtnDnN");
     PS_SBtnDn_H       .LoadSprite("PS_SBtnDnH");
     Q_Back            .LoadSprite("Q_Back");
     Q_BackSelect      .LoadSprite("Q_BackSelect");
     J_Back            .LoadSprite("J_Back");
     E_Back            .LoadSprite("E_Back");
     
     
	 
     
	 //     P.LoadSprite("P");
	 //     M.LoadSprite("M");
     
     
     TIMECAPS caps;
     timeGetDevCaps( &caps, sizeof(caps) );
     timeBeginPeriod( caps.wPeriodMin );
     
     
     ZeroMemory(qwPacketUseCounter, 150*sizeof(QWORD));
     ZeroMemory(qwPacketUseTime, 150*sizeof(QWORD));
     
     inGameTab = TRUE;
     
     // Start le thread de GetMessage.
     unsigned int iID;
     hMaintenanceThread = (HANDLE) _beginthreadex(NULL, 0, MaintenanceThread, NULL, NULL, &iID);
}

/***********/ // Fonctions de calcul XPSTAT :

extern __int64 XPStatValue,XPStatLastValue;
__int64 XpStatHourly[60];

DWORD XPTimerId=0;
DWORD XpStatIndex=0; 



float GetLastHourlyXpMean(void)
{
	float XpAccu=0.0f;
	for(int i=0;i<60;i++)
	{
		XpAccu+=XpStatHourly[i];
	}
	return XpAccu/60.0f;
};

float GetHourlyXpEstimate(void)
{
	int XpIndex=XpStatIndex;
	if (XpIndex>4)
		XpIndex-=5;
	else
	{
		XpIndex=60-(5-XpIndex);
	}
	float XpAccu=0;
	for(int i=0;i<5;i++)
	{
		XpAccu+=XpStatHourly[XpIndex];
		XpIndex=(XpIndex+1) % 60;
	}
	return XpAccu*12.0;
};

unsigned long GetSecondXpToNextLevel(void)
{
	float Hourly=GetHourlyXpEstimate();
	if (Hourly<=0)
		Hourly=1;
	return (unsigned long)(((float)Player.Exp2Go)/GetHourlyXpEstimate())*3600;
};


void __stdcall XpTimerCallback(HWND hwnd,unsigned int uMsg,unsigned int idEvent,DWORD dwTime)
{
	
	XpStatHourly[XpStatIndex]=XPStatValue;
	XPStatValue=0;
	XpStatIndex=(XpStatIndex+1) % 60;
};

void InitXpStat(void)
{
	XPStatValue=0;
	XPStatLastValue=Player.Exp;
	
	for(int i=0;i<60;i++)
	{
		XpStatHourly[i]=0;
	}
	if (XPTimerId==0)
	{
		XPTimerId=SetTimer(NULL,0,60000,XpTimerCallback);
	}
};
/***********/


void ParseText(char *pText) 
{
	Objects.Lock(28);
	BOOL boNPCisThere = Objects.FoundID(TalkToID);
	Objects.Unlock(28);
	TFCPacket Send;
	
	ClientConfig *pConfig = g_Global.GetClientConfig();
	
	char strText[1000];
	int dwlength = strlen(pText);
	if(dwlength >999)
		dwlength = 999;
	
	memcpy(strText,pText,dwlength+1);
	
	
	strcpy(LastText, strText);
	
	char *pRet = strstr(strText,"$mousepos");
	if(pRet != NULL)
	{
		int total = strlen(strText);
		int Last  = strlen(pRet);
		
		strText[total-Last] = 0x00;
		char strPos[100];
		sprintf(strPos,"%d,%d,%d",g_Global.GetLastMouseWorldX(),g_Global.GetLastMouseWorldY(),Player.World);
		strcat(strText,strPos);
		
	}
	
	if (strText[0] == '!') 
	{ 
		char *szCmd = strtok(strText, " ");
		
		
		//commande xpstat :
		if (!_stricmp(szCmd, "!XPSTAT")) { //Shainon
			xpstat.CalculateAndShowXpStat(Player.Exp,Player.Exp2Go);// TODO DaP
				

		}
		else if(!_stricmp(szCmd, "!XPRESET") || !_stricmp(szCmd, "!XPSTART")){ //Shainon
			xpstat.Init(Player.Exp);// TODO DaP
		}
		// by Asteryth PVP RANKING
		else if (!_stricmp(szCmd, "!Pvp"))
		{
			char *fct = strtok(NULL, " ");
			if(fct)
			{
				if (!_stricmp(fct, "stat"))
				{ // TODO DaP
					pvpranking.myPvpStat(
						Player.TotalDeathNumber,
						Player.TotalKillNumber,
						Player.CurrentDeathNumber,
						Player.CurrentKillNumber,
						Player.BestSerieMurder,
						Player.CurrentSerieMurder,
						Player.CurrentPvpPoint);
				}
			}
			else
			{
				ChatterUI::GetInstance()->AddBackscrollItem("", "PVP RANKING use methods:", RGB(174,238,238), true);
				ChatterUI::GetInstance()->AddBackscrollItem("", "!Pvp [stat]"/*|off|view|message]"*/, RGB(174,238,238), true);
				ChatterUI::GetInstance()->AddBackscrollItem("", "stat     : shows your pvp stats", RGB(174,238,238), true);
			}
		}
		else if (!stricmp(szCmd, "!RUN") && Player.CanRunScripts && g_bRunCommand == FALSE) 
		{
			g_bRunCommand = TRUE;
			sprintf(g_szFileName,"%s",strtok(NULL, " "));
			_beginthread(RunCommandThread, 0, NULL );
			/*
			char *szFileName = strtok(NULL, " ");
			
			  FILE *hFile = fopen(szFileName, "rt");
			  if (!hFile) 
			  {
			  char szTemp[256];
			  sprintf(szTemp, "Error: File %s cannot be opened.", szFileName);
			  
				ChatterUI::GetInstance()->AddBackscrollItem(
				"",
				szTemp,
				RGB(255,255,255),
				true
				);
				} 
				else 
				{
				char szTemp[256];
				while (fgets(szTemp, 255, hFile)) 
				{
				if (szTemp[strlen(szTemp)-1] == 10 || szTemp[strlen(szTemp)-1] == 13)
				szTemp[strlen(szTemp)-1] = NULL;
				ParseText(szTemp);
				}
				fclose(hFile);
				}
			*/
		} 
		else if (!stricmp(szCmd, "!SLEEP")) 
		{
			char *szTime = strtok(NULL, " ");
			if (szTime)
				Sleep(atoi(szTime));
		}
		else if(!stricmp(szCmd, "!GUILD"))
		{
			GuildUI::Display( NULL );
		}
		
		else if (!stricmp(szCmd, "!SPELL"))// && Player.CanRunScripts && Custom.gWebPatchEnabled == FALSE)
		{
			char *spellId = strtok(NULL, " ");
			/*
			char *posX	= strtok(NULL, " "),
			*posY	= strtok(NULL, " "),
			*spellID = strtok(NULL, " ");
			if (spellID == NULL && posX != NULL) {
			spellID = posX;
			posX = posY = "0";
			} else if (posY == NULL) { return; }
        
		  TFCPacket spellPacket;
		  spellPacket << (RQ_SIZE)RQ_SpellEffect;
		  spellPacket << (short)atoi(spellID);
		  spellPacket << (long)Player.ID;
		  spellPacket << (long)0;
		  spellPacket << (short)(Player.xPos+atoi(posX));
		  spellPacket << (short)(Player.yPos+atoi(posY));
		  spellPacket << (short)(Player.xPos);
		  spellPacket << (short)(Player.yPos);
		  spellPacket << (long)1;
		  spellPacket << (long)0;
		  HandlePacket(&spellPacket);
		  */
			TFCPacket sending;
        
        sending << (RQ_SIZE)RQ_CastSpell;
		sending << (short) atoi(spellId);//spell.Id;
		sending << (short)0;
		sending << (short)0;
		sending << (long)0;

        SEND_PACKET( sending );
		  }
		
		
		else if (!stricmp(szCmd, "!AFK")) 
		{
			
			char *szAFKCmd = strtok(NULL, "");
			char szTemp[256];
			
			if(szAFKCmd)
			{
				
				if (!stricmp(szAFKCmd, "VIEW"))
				{
					char szAFK[5];
					if(pConfig->dwAfkStatus)
						sprintf(szAFK,"on");
					else
						sprintf(szAFK,"off");
					
					sprintf(szTemp, "AFK Mode is %s. Your current afk message is : %s", szAFK, pConfig->strAfkMessage);
					ChatterUI::GetInstance()->AddBackscrollItem("", szTemp, RGB(255, 0, 0), true);
				}
				else if (!stricmp(szAFKCmd, "ON"))
				{
					pConfig->dwAfkStatus = 1;
					sprintf(szTemp, "AFK Message enabled.");
					ChatterUI::GetInstance()->AddBackscrollItem("", szTemp, RGB(255, 0, 0), true);
				} 
				else if (!stricmp(szAFKCmd, "OFF"))
				{
					pConfig->dwAfkStatus = 0;               
					sprintf(szTemp, "AFK Message disabled.");
					ChatterUI::GetInstance()->AddBackscrollItem("", szTemp, RGB(255, 0, 0), true);
				} 
				else
				{
					sprintf(pConfig->strAfkMessage,"%s",szAFKCmd);
					sprintf(szTemp, "AFK Message set to %s.", szAFKCmd);
					ChatterUI::GetInstance()->AddBackscrollItem("", szTemp, RGB(255, 0, 0), true);
				}
			}
			else
			{
				ChatterUI::GetInstance()->AddBackscrollItem("", "AFK Mode using method:", RGB(255, 0, 0), true);
				ChatterUI::GetInstance()->AddBackscrollItem("", "!AFK [on|off|view|message]", RGB(255, 0, 0), true);
				ChatterUI::GetInstance()->AddBackscrollItem("", "on     : turns the afk mode on", RGB(255, 0, 0), true);
				ChatterUI::GetInstance()->AddBackscrollItem("", "off    : turns the afk mode off.", RGB(255, 0, 0), true);
				ChatterUI::GetInstance()->AddBackscrollItem("", "view   : shows the current afk message and mode", RGB(255, 0, 0), true);
				ChatterUI::GetInstance()->AddBackscrollItem("", "message: modify the current afk message.", RGB(255, 0, 0), true);
			}
			
		}
		else if (!stricmp(szCmd, "!POS") /*&& Player.CanRunScripts*/) 
		{
			if(Custom.NMShowPosition == 0)
			{
				Custom.NMShowPosition = 1;
				ChatterUI::GetInstance()->AddBackscrollItem("", "Show MAP position (X,Y,W).", RGB(255, 0, 0), true);
			}
			else
			{
				Custom.NMShowPosition = 0;
				ChatterUI::GetInstance()->AddBackscrollItem("", "Hide MAP position.", RGB(255, 0, 0), true);
			}
			pConfig->dwNMShowPOS = Custom.NMShowPosition; 
		}
		else if (!stricmp(szCmd, "!Clear") /*&& Player.CanRunScripts*/) 
		{
            ChatterUI::GetInstance()->ClearBackScroll();
		}
		else if (!stricmp(szCmd, "!FPS") /*&& Player.CanRunScripts*/) 
		{
			//char *szFPSTime = strtok(NULL, " ");//BLBLBL 29 mars 2009 suppr. lignes inutilisée
			
			
			/*if(szFPSTime)
			{
            Custom.NMFPSValue = atoi(szFPSTime);
            if(Custom.NMFPSValue <1 || Custom.NMFPSValue >50)
			Custom.NMFPSValue = 32;
            g_Var.tWaitFPSTime     = 1000/Custom.NMFPSValue;
            pConfig->dwNMFPSValue = Custom.NMFPSValue; 
			}
			else
			{*/
            if(Custom.NMShowFPS == 0)
            {
				Custom.NMShowFPS = 1;
				ChatterUI::GetInstance()->AddBackscrollItem("", "Show FPS.", RGB(255, 0, 0), true);
            }
            else
            {
				Custom.NMShowFPS = 0;
				ChatterUI::GetInstance()->AddBackscrollItem("", "Hide FPS.", RGB(255, 0, 0), true);
            }
            pConfig->dwNMShowFPS = Custom.NMShowFPS; 
			//}
		}
		
		//the strlen>2 avoids smileys :), :p to be displayed in main channel as ) or p
		//exception is for the word : k often used as Okay
     } else if ( strText[0] == ':' && ( (strlen(strText)>2)||(strText[1]=='k')||(strText[1]=='K') ) ) { 
		 
		 Send << (short)RQ_Shout;
		 Send << (short)strlen(Player.Name);
		 Send << (char *)Player.Name;
		 Send << (long)Custom.gTextColor;
		 Send << (short)(strlen(strText)-1);
		 Send << (char *)&strText[1];
		 LOG << "* INT = 1, " << strlen(Player.Name) << "\r\n";
		 SEND_PACKET(Send);
		 
		 
		 
		 //								} else if (strText[0] == ';') {
		 //									u_Chatter.ParseCommand(&strText[1]);
     } 
     else if (strText[0] == '/') 
     {
		 OptionParam *pOption = g_SaveGame.GetOptionsParam();
		 if (pOption->bPageEnable) 
		 {
			 BOOL MultiName = FALSE;
			 Send << (short)RQ_Page;
			 char Token[2500];
			 for (UINT i = 1; i < strlen(strText); i++) {
				 if (strText[i] == '\"' && i == 1) {
					 MultiName = TRUE;
				 } else if (strText[i] == ' ' && !MultiName) {
					 Token[i-(MultiName ? 2 : 1)] = 0;
					 i = strlen(strText);
				 } else if (strText[i] == '\"' && MultiName) {
					 Token[i-(MultiName ? 2 : 1)] = 0;
					 i = strlen(strText);
				 } else {
					 Token[i-(MultiName ? 2 : 1)] = strText[i];
				 }
			 }         
			 LOG << "PAGING: [" << Token << "] [" << &strText[strlen(Token)+1+(MultiName ? 2 : 0)] << "]\r\n";
			 
			 //char buf[ 1024 ];
			 //sprintf( buf, g_LocalString[88], Token );
			 //string str( buf );
			 
			 Send << (short)(strlen(Token));
			 Send << (char *)Token;
			 if (strlen(&strText[strlen(Token)+1+(MultiName ? 2 : 0)])) 
			 {
				 //short shVal = (short)strlen(&strText[strlen(Token)+1+(MultiName ? 2 : 0)]);
				 //char strtoto[1000];
				 //sprintf(strtoto,"%s",(char *)&strText[strlen(Token)+1+(MultiName ? 2 : 0)]);
				 
				 
				 Send << (short)strlen(&strText[strlen(Token)+1+(MultiName ? 2 : 0)]);            
				 Send << (char *)&strText[strlen(Token)+1+(MultiName ? 2 : 0)];
				 //str += &strText[strlen(Token)+1+(MultiName ? 2 : 0)];
			 } else {
				 Send << (short)1;
				 Send << (char *)" ";
				 //str += " ";
			 }
			 // To avoid paging while AFK.
			 if(!pConfig->dwAfkStatus)
			 {
				 if (!ChatterUI::GetInstance()->IsUserIgnored( Token )) {
					 SEND_PACKET(Send);
				 } else {
					 ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[517], RGB(155,155,155), true, false );
				 }
			 } else {
				 ChatterUI::GetInstance()->AddBackscrollItem( "", "You cannot page while AFK!", RGB(155, 155, 155), true, false);
			 }
			 
			 /*ChatterUI::GetInstance()->AddBackscrollItem(
			 "",
			 str.c_str(),
			 SYSTEM_COLOR,
			 false
			 );*/
			 
		 } else {
			 ChatterUI::GetInstance()->AddBackscrollItem(
				 "",
				 g_LocalString[100],
				 RGB(255,255,255),
				 true
				 );
		 }
		 //the strlen>2 avoids smileys :), :p to be displayed in main channel as ) or p
		 //exception is for the word : k often used as Okay
     }else if ( strText[0] == ';'  && ( (strlen(strText)>2)||(strText[1]=='k')||(strText[1]=='K') ) ) { 
		 ChatterUI::GetInstance()->SendMessageToCurrentChannel(
			 &strText[1]
			 );
     } else if (!stricmp(strText, g_LocalString[557])) {
		 g_EventHelp.Lock();
		 g_EventHelp.OnOff = false;
		 g_EventHelp.Unlock();
     } else if (!stricmp(strText, g_LocalString[556])) {
		 g_EventHelp.Lock();
		 g_EventHelp.OnOff = true;
		 g_EventHelp.Unlock();
     }else if ((TalkToX || TalkToY) && boNPCisThere) {
		 if (strText[0]) {
			 Send << (short)RQ_DirectedTalk;
			 Send << (short)(TalkToX);
			 Send << (short)(TalkToY);
			 TalkToOffset = 0;
			 Send << (long)TalkToID;
			 Direction = 1;
			 int a = ((TalkToX-Player.xPos)+11)*3;
			 int b = ((TalkToY-Player.yPos)+16)*2;
			 if	(b > 30) {
				 if (a > 30) {
					 a -= 30;
					 b -= 30;
					 if (a > b*2) {
						 Direction = 6;
					 } else if (b > a*2) {
						 Direction = 2;
					 } else {
						 Direction = 3;
					 }
				 } else {
					 b -= 30;
					 a = 30 - a;
					 if (a > b*2) {
						 Direction = 4;
					 } else if (b > a*2) {
						 Direction = 2;
					 } else {
						 Direction = 1;
					 }
				 }
			 } else {
				 if (a > 30) {
					 a -= 30;
					 b = 30 - b;
					 if (a > b*2) {
						 Direction = 6;
					 } else if (b > a*2) {
						 Direction = 8;
					 } else {
						 Direction = 9;
					 }	
				 } else {
					 a = 30 - a;
					 b = 30 - b;
					 if (a > b*2) {
						 Direction = 4;
					 } else if (b > a*2) {
						 Direction = 8;
					 } else {
						 Direction = 7;
					 }
				 }			
			 }
			 Objects.Lock(28);
			 LOG << "NPC : " << timeGetTime() << " " << Objects.GetName(TalkToID) << ": " << strText << ".\r\n";
			 Objects.Unlock(28);
			 Send << (char)Direction;
			 Send << (long)Custom.gTextColor;
			 Send << (short)strlen(strText);
			 Send << (char *)strText;
			 SEND_PACKET(Send);
		 } else {
			 Send << (short)RQ_BroadcastTextChange;
			 Send << (long)TalkToID;
			 Send << (short)TalkToOffset;
			 LOG << "* INT 2, " << TalkToID << ", " << TalkToOffset << "\r\n";
			 SEND_PACKET(Send);
		 }
     } else { 
		 Send << (short)RQ_IndirectTalk;
		 Send << (long)Player.ID;
		 Send << (char)0;
		 Send << (long)Custom.gTextColor;
		 Send << (short)strlen(strText);
		 if (strText[0])
			 Send << (char *)strText;
		 SEND_PACKET(Send); 
     } 
     strText[0] = 0;
}

//Fonction de webpatch, qui merde sous Vista :
void WebPatchUpdate(char *lpszPath, char *lpszIP, char *lpszUser, char *lpszPassword,
	DWORD dwVersion) {

	try {
		// Webpatch Update.

		BOOL bWebPatchUpdate = FALSE;

		BSTR lpszFileList;
		long lProcID;

		// Webpatch Setup
		char szFullPath[MAX_PATH];
		char szAppDir[MAX_PATH];
		char szAppName[MAX_PATH];
		char szDownloadDir[256];

		wchar_t szPatchDir[256];

		DWORD dwExitCode;

		// Get the Full Module Path
		GetModuleFileName(NULL, szFullPath, MAX_PATH);

		// Get the Application Directory
		strcpy(szAppDir, szFullPath);
		for (int i = strlen(szAppDir) - 1; i > 0; i--) {
			if (szAppDir[i] == '\\') {
				szAppDir[i] = NULL;
				strcpy(szAppName, szAppDir + i + 1);
				i = 0;
			}
		}

		// Get the Download Directory
		strcpy(szDownloadDir, szAppDir);
		strcat(szDownloadDir, "/Download");

		{
			// Setup WebPatch
			IWebPatchPtr lpWebPatch(__uuidof(WebPatch));
			wchar_t Temp[MAX_PATH];

			ZeroMemory(Temp, MAX_PATH * 2);
			vir::AnsiToUnicode()(szAppName, Temp);
			lpWebPatch->appID = Temp;

			ZeroMemory(Temp, MAX_PATH * 2);
			vir::AnsiToUnicode()(szAppDir, Temp);
			lpWebPatch->appDirectory = Temp;

			ZeroMemory(Temp, MAX_PATH * 2);
			vir::AnsiToUnicode()(szDownloadDir, Temp);
			lpWebPatch->downloadDirectory = Temp;

			ZeroMemory(Temp, MAX_PATH * 2);
			vir::AnsiToUnicode()(lpszPath, Temp);
			lpWebPatch->imagePath = Temp;
			LOG << "* WEB = " << lpszPath << "\r\n";

			ZeroMemory(Temp, MAX_PATH * 2);
			vir::AnsiToUnicode()(ServerIP, Temp);
			lpWebPatch->referer = Temp;

			ZeroMemory(Temp, MAX_PATH * 2);
			vir::AnsiToUnicode()(lpszIP, Temp);
			lpWebPatch->url = Temp;
			LOG << "* WEB = " << lpszIP << "\r\n";

			ZeroMemory(Temp, MAX_PATH * 2);
			vir::AnsiToUnicode()(lpszUser, Temp);
			lpWebPatch->userName = Temp;

			ZeroMemory(Temp, MAX_PATH * 2);
			vir::AnsiToUnicode()(lpszPassword, Temp);
			lpWebPatch->userPassword = Temp;

			if (Custom.Debug) {
				vir::AnsiToUnicode()("WebPatchDebug.Log", Temp);
				lpWebPatch->logFile = Temp;
			}

			wcscpy(szPatchDir, lpWebPatch->webPatchDirectory);
			lProcID = lpWebPatch->processID;

			if (lpWebPatch->NewWebPatchNeeded(&lpszFileList)) {
				EnableWindow(GetDlgItem(GlobalHwnd, IDC_COMBO1), FALSE);
				EnableWindow(GetDlgItem(GlobalHwnd, ACCOUNT), FALSE);
				EnableWindow(GetDlgItem(GlobalHwnd, PASSWORD), FALSE);
				EnableWindow(GetDlgItem(GlobalHwnd, IDC_SAVE), FALSE);
				EnableWindow(GetDlgItem(GlobalHwnd, IDC_WINDOWED), FALSE);
				EnableWindow(GetDlgItem(GlobalHwnd, IDC_GRAPH), FALSE);
				EnableWindow(GetDlgItem(GlobalHwnd, IDC_INFO), FALSE);
				EnableWindow(GetDlgItem(GlobalHwnd, IDCANCEL), FALSE);
				InvalidateRect(GlobalHwnd, NULL, FALSE);
				UpdateWindow(GlobalHwnd);
				bWebPatchUpdate = TRUE;
			}
			else {

				// If a patch is needed
				if (lpWebPatch->PatchNeeded(Player.Version, dwVersion))
				{
					// Send exit packet.                
					TFCPacket sending;
					sending << (RQ_SIZE)RQ_ExitGame;

					SEND_PACKET(sending);
					Sleep(500);

					if (!g_WebPatchSuccess)
					{
						char szRestartApp[256];
						strcpy(szRestartApp, szAppDir);
						strcat(szRestartApp, "\\T4C.EXE");
						//                  lpWebPatch->StartPatch( GetCurrentProcessId(), szFullPath, szAppDir, "WebPatch Success", "WebPatch Cancel", Player.Version, dwVersion );
						char szTemp[1000];
						sprintf(szTemp, "WebPatch Success %s", szCopyCmdLine);
						lpWebPatch->StartPatch(GetCurrentProcessId(), szRestartApp, szAppDir, szTemp, "WebPatch Cancel", Player.Version, dwVersion);
						g_App.Close(g_LocalString[408]);
					}
					else
					{
						lpWebPatch->WriteErrorReport("WebPatch.Log");
						MessageBox(siGethWnd(), g_LocalString[409], g_LocalString[410], MB_OK);
						g_App.Close(g_LocalString[409]);
					}

				}

				// Flush the download directory

				///////////////////////////////
				// !OPTIONAL!
				//BSTR moduleList;
				//lpWebPatch->GetModuleList( &moduleList );

				//SysFreeString( moduleList )

				//lpWebPatch->DownloadModule(L"Module Name", true)
			}

			lProcID = lpWebPatch->processID;
		}

		// SHUTDOWN WEBPATCH
		int iErrorCode = 0;//BLBL 29 mars 2009 initialisation de la variable à 0
		int i = 0;
		if (bWebPatchUpdate) {
			HANDLE hP = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, lProcID);
			GetExitCodeProcess(hP, &dwExitCode);
			while (dwExitCode == STILL_ACTIVE && i < 80) {
				iErrorCode = GetLastError();
				TerminateProcess(hP, 0);
				Sleep(250);
				GetExitCodeProcess(hP, &dwExitCode);
				i++;
			}
		}

		if (i >= 80) {
			char Temp[1000];
			LPVOID lpMsgBuf;

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				iErrorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR)&lpMsgBuf,
				0,
				NULL
			);

			sprintf(Temp, g_LocalString[411], (LPCTSTR)lpMsgBuf);
			MessageBox(siGethWnd(), Temp, g_LocalString[410], MB_OK);

			LocalFree(lpMsgBuf);
			g_App.Close(Temp);
		}

		if (bWebPatchUpdate) {
			// Copy files in the Download directory to the directory
			// specified by lpWebPatch->webPatchDirectory

			wchar_t szFrom[256];
			wchar_t szTo[256];
			wchar_t *szFileName;
			wchar_t *szTemp;

			szTemp = wcstok(lpszFileList, L"|");
			while (szTemp) {

				wcscpy(szFrom, szTemp);

				for (int i = wcslen(szTemp); i > 0; i--) {
					if (szTemp[i] == L'\\') {
						szFileName = szTemp + i + 1;
						i = 0;
					}
				}

				wcscpy(szTo, szPatchDir);
				wcscat(szTo, szFileName);

				char szFromA[1000];
				char szToA[1000];

				ZeroMemory(szFromA, 1000);
				ZeroMemory(szToA, 1000);
				vir::UnicodeToAnsi()(szFrom, szFromA);
				vir::UnicodeToAnsi()(szTo, szToA);

				i = 0;
				while (!CopyFile(szFromA, szToA, FALSE) && i < 80) {
					iErrorCode = GetLastError();
					Sleep(250);
					i++;
				}
				Sleep(1000);
				DeleteFileW(szFrom);

				if (i >= 80) {
					char Temp[1000];
					LPVOID lpMsgBuf;

					ZeroMemory(Temp, 1000);

					FormatMessage(
						FORMAT_MESSAGE_ALLOCATE_BUFFER |
						FORMAT_MESSAGE_FROM_SYSTEM |
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						iErrorCode,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR)&lpMsgBuf,
						0,
						NULL
					);

					sprintf(Temp, g_LocalString[412], szFromA, szToA, (LPCTSTR)lpMsgBuf);
					MessageBox(siGethWnd(), Temp, g_LocalString[410], MB_OK);

					LocalFree(lpMsgBuf);
					g_App.Close(Temp);
				}

				szTemp = wcstok(NULL, L"|");
			}

			ShellExecute(siGethWnd(), "open", "webpatch.exe", "-regserver", "", SW_HIDE);
			Sleep(500);

			if (!g_boQuitApp) {
				EnableWindow(GetDlgItem(GlobalHwnd, IDC_COMBO1), TRUE);
				EnableWindow(GetDlgItem(GlobalHwnd, ACCOUNT), TRUE);
				EnableWindow(GetDlgItem(GlobalHwnd, PASSWORD), TRUE);
				EnableWindow(GetDlgItem(GlobalHwnd, IDC_SAVE), TRUE);
				EnableWindow(GetDlgItem(GlobalHwnd, IDC_GRAPH), TRUE);
				EnableWindow(GetDlgItem(GlobalHwnd, IDC_WINDOWED), TRUE);
				EnableWindow(GetDlgItem(GlobalHwnd, IDC_INFO), TRUE);
				EnableWindow(GetDlgItem(GlobalHwnd, IDCANCEL), TRUE);
				InvalidateRect(GlobalHwnd, NULL, FALSE);
				UpdateWindow(GlobalHwnd);
				WebPatchUpdate(lpszPath, lpszIP, lpszUser, lpszPassword, dwVersion);
			}
		}
	}
	catch (_com_error e) {
		char Temp[1000];
		sprintf(Temp, g_LocalString[413], e.ErrorMessage());
		MessageBox(siGethWnd(), Temp, "Webpatch error", MB_OK);
		MessageBox(siGethWnd(), "Please run T4C in Administrator mode to solve this issue.", "Webpatch error", MB_OK);
		g_App.Close(Temp);
	}
}

//BLBL message Please wait while loading The Fouth coming
void DrawFisrtLoadingText()
{
	//static int dwCntFLT = 2;
	static int tipOfTheDay =0;
	if (tipOfTheDay==0) {
		Random rndTip(589, 599);
		tipOfTheDay=rndTip;
	}
	
	HDC hdc;
	DXDGetDC(&hdc, 14);
	
	int dwX = (g_Global.GetScreenW()-fMenuFont->GetLen(hdc,g_LocalString[1]))/2;
	int dwY = g_Global.GetScreenH()-125;
	fMenuFont->DrawFont(hdc, dwX,dwY , RGB(255, 255, 255), 0, g_LocalString[1]);
	
	char *Text  = new char [100];
	char *Text2 = new char [100];
	//char *Text3 = new char [100];
	
	strcpy(Text, g_LocalString[9]);
	strcat(Text, " ");
	strcat(Text, g_LocalString[10]);
	
	//int dwCnt = 0;//BLBLBL 29 mars 2009 suppr. lignes inutilisée
	/*for(int i=0;i<dwCntFLT;i+=2)
	{
	Text3[i+0] = '.';
	Text3[i+1] = ' ';
	}
	Text3[i] = 0x00;
	dwCntFLT++;
	if(dwCntFLT >= 60)
	dwCntFLT = 2;*/
	
	dwX = (g_Global.GetScreenW()-fMenuFont->GetLen(hdc,Text))/2;
	dwY = dwY+10+fMenuFont->GetHeight(hdc);
	fMenuFont->DrawFont(hdc, dwX, dwY, RGB(255, 255, 255), 0, Text);
	
	/*dwX = (g_Global.GetScreenW()-fSystemFont->GetLen(hdc,Text3))/2;
	fSystemFont->DrawFont(hdc, dwX, dwY+30 +fSystemFont->GetHeight(hdc), RGB(255, 255, 255), 0, Text3);*/
	
	
	char *TextDidYouKnow = new char [100];//BLBLBL 09/12/2010
	//BLBLBL : textes pris au pif dans english.elng
	strcpy(TextDidYouKnow, g_LocalString[tipOfTheDay]);
		  
	int dwXdidYouKnow = (g_Global.GetScreenW()-fMenuLittleFont->GetLen(hdc,TextDidYouKnow))/2;
	int dwYdidYouKnow = dwY+10+fMenuFont->GetHeight(hdc);		   
	fMenuLittleFont->DrawFont(hdc, dwXdidYouKnow, dwYdidYouKnow, RGB(255, 255, 255), 0, TextDidYouKnow);
	
	
	delete Text;
	delete Text2;
	//   delete Text3;
	
	DXDReleaseDC(hdc, 14);
}

void FirstInitObject(LPVOID pParam)
{
	SoundFX[6].Create("Open Box", TS_MEMORY);
	SoundFX[110].Create("Equip", TS_MEMORY);
	SoundFX[44].Create("Vampire Dying", TS_MEMORY);
	
	Objects.Lock(0);
	Objects.Create(); // Create the Game Object.
	Objects.Unlock(0);
	g_bFirstLoadComplete = TRUE;
}

int ReadConfigLanguage(char *strPath)
{
	char strConfigFile[512];
	sprintf(strConfigFile,"%s\\V2Launcher.dat",strPath);
	
	FILE *pf1 = fopen(strConfigFile,"rb");
	if(!pf1)
	{
		return 0; // english by default
	}
	else
	{
		ULONG dwTailleOri;
		ULONG dwTailleComp;
		BYTE *pFileDataTmp = NULL;
		BYTE *pFileDataTmpC = NULL;
		
		fread(&dwTailleOri,4,1,pf1);
		fread(&dwTailleComp,4,1,pf1);
		pFileDataTmp = new BYTE[dwTailleOri];
		pFileDataTmpC= new BYTE[dwTailleComp];
		fread(pFileDataTmpC,1,dwTailleComp,pf1);
		fclose(pf1);
		
		if(uncompress((BYTE*)pFileDataTmp,&dwTailleOri,(BYTE*)pFileDataTmpC,dwTailleComp) != Z_OK)
			OutputDebugString("Erreur UnCompress Connection");
		
		unsigned char *pchBuff = pFileDataTmp;
		
		int dwBid = 0;
		int dwLanguage;
		pchBuff+=22;//header
		memcpy(&dwBid,pchBuff,4); pchBuff+=4;
		memcpy(&dwBid,pchBuff,4); pchBuff+=4;
		memcpy(&dwBid,pchBuff,4); pchBuff+=4;
		memcpy(&dwLanguage,pchBuff,4); pchBuff+=4;
		delete []pFileDataTmp;
		delete []pFileDataTmpC;
		
		return dwLanguage;
	}
}

void ExitThreadGame(LPVOID pParam)
{
	
	
	boInterruptLogoff=FALSE;//par défaut on n'est pas interrompu quand on lance la déco, att ces variables doivent être en début de fonction car modifiée par un thread déclenché après l'envoie du pak.
	boForceLogoff=FALSE;//par défaut on n'est pas interrompu quand on lance la déco
	
	//BLBLBL : on envoie la notification comme quoi le client commence le décompte
	TFCPacket Send;
	LOG << "\r\nSending code 123 (safeplug).";
	Send << (short)RQ_SafePlug;
	Send << (char)0;
	SEND_PACKET(Send);
	
	//BLBLBL message d'information :
	ChatterUI::GetInstance()->AddBackscrollItem("", g_GUILocalString[220], RGB(255,180,0), true);
	
	char strMessage[128];
	
	for(int i=15;i>0;i--) //NADNADNAD
	{
		if (boInterruptLogoff||boForceLogoff) {
			break;//si on est interrompu on sort du décompte
		}
		sprintf( strMessage, g_GUILocalString[169], i );
		ChatterUI::GetInstance()->AddBackscrollItem("",strMessage, RGB(255,180,0), true);
		Sleep(1000);
	}
	
	if (boInterruptLogoff){
		
		g_bExitComplete = false;//on indique au GUI client que la sortie du jeu a été annulée
		ChatterUI::GetInstance()->AddBackscrollItem("", g_GUILocalString[221], RGB(255,0,0), true);
		
	}else if (boForceLogoff){
		
		ChatterUI::GetInstance()->AddBackscrollItem("", g_GUILocalString[222], SYSTEM_COLOR, true);
		Sleep(500);
		PostMessage(siGethWnd(), WM_CLOSE, NULL, 0x6969);//cette fois le paramettre est 0x6969
		
	}else{
		
		ChatterUI::GetInstance()->AddBackscrollItem("", g_GUILocalString[219], SYSTEM_COLOR, true);  
		Sleep(500);
		PostMessage(siGethWnd(), WM_CLOSE, NULL, 0x6969);//cette fois le paramettre est 0x6969
		
	}
}  

void RunCommandThread(LPVOID pParam)
{
	FILE *hFile = fopen(g_szFileName, "rt");
	if (!hFile) 
	{
		char szTemp[1000];
		sprintf(szTemp, "Error: File %s cannot be opened.", g_szFileName);
		
		ChatterUI::GetInstance()->AddBackscrollItem("",szTemp,RGB(255,255,255),true);
	} 
	else 
	{
		char szTemp[1000];
		while (fgets(szTemp, 255, hFile)) 
		{
			if (szTemp[strlen(szTemp)-1] == 10 || szTemp[strlen(szTemp)-1] == 13)
				szTemp[strlen(szTemp)-1] = NULL;
			OutputDebugString(szTemp);
			OutputDebugString("\n");
			
			ParseText(szTemp);
			Sleep(50);
		}
		fclose(hFile);
	}
	g_bRunCommand = FALSE;
} 

