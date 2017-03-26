// **************************************************************************
// ***                                                                    ***
//      File Name: TFCSocket.Cpp
//      Project:   The Fourth Coming
//      Creation:  May 30th, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                    ***
// **************************************************************************
// ***                                                                    ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         06-27-1997      1.0     TH          Initial development.
//
//		  Fonctions:
//			  Provide Socket Receive Function
//
// ***                                                                    ***
// **************************************************************************
// *** Copyright (c) 1996-2003 Vircom inc.         All rights reserved.   ***
#include "pch.h"
#pragma warning( disable : 4291 )

#define uMinus(a, b) (b > a ? (0) : (a - b))

#pragma warning (disable:4786)

#include <windows.h>
#include "App.H"
#include "Bitmap.h"
#include "cs.h"
#include "DirectXInput.h"
#include "Disp.h"
#include <eh.h>
#include "ExpFltr.h"
#include "FormatText.h"
#include "MacroHandler.h"
#include "Mouse.h"
#include "MouseAction.h"
#include "NewFont.h"
#include "NTime.h"
#include "ObjectListing.h"
#include "old.h"
#include "Packet.h"
#include "pf.h"
#include "PowerDebug.h"
#include <process.h>
#include "Random.h"
#include <stdio.h>
#include "TFCFlag.h"
#include "TFCLandDef.h"
#include "TFCPacket.h"
#include "TFCPlayer.h"
#include "TFCSocket.h"
#include "TileSet.h"
#include "VisualObjectList.h"
#include "WarningBox.h"
#include "LocalString.h"
#include "Fading.h"
#include "NewInterface/RootBoxUI.h"
#include "NewInterface/CharacterUI.h"
#include "NewInterface/ChatterUI.h"
#include "NewInterface/SideMenu.h"
#include "NewInterface/ChestUI.h"
#include "NewInterface/TradeUI.h"
#include "MemoryManager.h"
#include "ProcStat.h"
#include "CombatCursor.h"
#include "SysMsg.h"
#include "SaveGame.h"
#include "NMVideoCapture.h"
#include "VideoCapture.h"

extern CSaveGame g_SaveGame;
extern NMVideoCapture g_NMVideoCapture;
extern Font *fNewGuiBtnFont2;

extern unsigned char *g_ListData;
extern int            g_ListSize;
extern char           ServerIP[100];

#define MOUSE_CLICK(a, b, c, d) (x >= a && x <= a+c && y >= b && y <= b+d && Chose == 1)

extern CProcStat g_TimeStat;

BOOL ReAsk = FALSE;

BOOL DBL = FALSE;

DWORD ThreadATimeStamp = 0;
DWORD ThreadBTimeStamp = 0;

DWORD LastCall;
BOOL CreateFlag = FALSE;

extern BOOL QuitFirstLoop;

DWORD CreateBoost = 1;

DWORD TOTALOUT = 0;
DWORD OUTREALPEEK = 0;
DWORD OUTREALLOW = -1;
DWORD OUTPEEK = 0;
DWORD TIMESTART = 0;
DWORD TOTALIN = 0;
DWORD INREALPEEK = 0;
DWORD INREALLOW = -1;
DWORD INPEEK = 0;
DWORD OUTLASTGET = 0;
DWORD INLASTGET = 0;

DWORD LAG = 0;
DWORD LAG2 = 0;

int SlowMeDown = 0;

extern HANDLE hDrawThread;
extern HANDLE hMaintenanceThread;
extern HANDLE hMouseActionThread;
extern BOOL DoNotMove;

extern BOOL MESSAGE_THREAD;
extern BOOL MENU_THREAD;

int ResizeMax = 624;

extern BOOL INGAME;
char MaxCharactersPerAccount = 3;
unsigned char key;

unsigned char Direction;

extern LPDIRECTINPUTDEVICE lpDIMouseDevice; // Direct Input Device

char MenuName[100];

extern void LoadMusic(void);
extern void ReleaseMusic(void);

DWORD GetThreadTime(HANDLE h) {
    FILETIME Dumb;
    FILETIME Time;
    FILETIME Kernel;
    GetThreadTimes(h, &Dumb, &Dumb, &Kernel, &Time);
    DWORD dwT = (Time.dwHighDateTime << 19) + (Time.dwLowDateTime >> 13);
    dwT += (Kernel.dwHighDateTime << 19) + (Kernel.dwLowDateTime >> 13);
    return dwT;
}

inline int Abs(int x) {
    return ((x < 0) ? (-x) : (x));
}
bool Retrace = false;


extern T3VSBSound SoundFX[250];
T3VSBSound *Victim = NULL;

DWORD LastID;

extern bool INFINITELOOP;
int Chose = 0;

extern HANDLE InterPacketEvent;

class InterPacket : public TemplateList <unsigned char *> {
public:
    void Set(void) {
        SetEvent(InterPacketEvent);
    }
};

extern InterPacket ug_InterPacket;

extern Font *fFont;
extern Font *fMenuFont;
extern Font *fSystemFont;
extern Font *fSkillFont;
extern Font *fMenuLittleFont;
extern Font *fMenuDescFont;
extern Font *BucephalusFont;
extern Font *fNewGuiBtnFont;
extern Font *fNewGuiBtnFontBig;
extern Font *fNewGuiBtnFont3;

int GeX = 0, GeY = 0;

extern bool WannaPos;
extern int SkillX;
extern int SkillY;

bool Shit = false;
bool LevelUp = false;
extern HWND g_hwnd;

DWORD DoubleClickTemp2 = -10000;
bool DoubleClick = false;

char Force = 0;

bool            Draw;
//char Text[1000];

bool Follow = false;
int FollowID = 0;
bool Err = true;
bool DrawFinish = false;

// TODO : TFC CLIENT JUNK
bool MenuThreadFinished = false;

QWORD FileTime2QWord(FILETIME *);

//++NMNMNM --[NEW GUI CV2Sprite Sprite]
//extern Sprite IntroP[NM_CONNECT_SCREEN_NBR_BACK_ANIM];
extern CV2Sprite    Intro[NM_CONNECT_SCREEN_NBR_BACK_ANIM];
extern CV2Sprite    Connect_D_Anim[36];
extern CV2Sprite    Connect_Title2;
extern CV2Sprite    Connect_D;
extern CV2Sprite    Connect_V;
extern CV2Sprite    Connect_Main_Back;
extern CV2Sprite    Connect_Main_BtnN;
extern CV2Sprite    Connect_Main_BtnD;
extern CV2Sprite    Connect_Main_BtnH;
extern CV2Sprite    PS_Back;
extern CV2Sprite    PS_BackDown;
extern CV2Sprite    PS_Btn_N;
extern CV2Sprite    PS_Btn_H;
extern CV2Sprite    PS_SmallBtn_N;
extern CV2Sprite    PS_SmallBtn_H;
extern CV2Sprite    PS_Over;
extern CV2Sprite    PS_SBtnUp_N;
extern CV2Sprite    PS_SBtnUp_H;
extern CV2Sprite    PS_SBtnDn_N;
extern CV2Sprite    PS_SBtnDn_H;
extern CV2Sprite    Q_Back;
extern CV2Sprite    Q_BackSelect;
extern CV2Sprite    J_Back;
extern CV2Sprite    E_Back;


extern bool MessageThreadFinished;
extern Random rnd;
//extern Sprite SplashP;
extern CV2Sprite    Splash;
extern CV2Sprite P, M;
extern volatile unsigned long Round;
extern char TFC_State;
char LastState = 0;
//extern CString ServerIP;
//extern unsigned int ServerPort;
extern sockaddr_in ServerIPaddr;
extern bool Register;
bool Move = true;
bool Move2 = true;
bool NMAutoMove = false;
int  NMAutoDir  = 0;
DWORD dwTimeAsk = 0;

int xResize = 20;
int yResize = 30;
int ResizeCorrection = 0;

extern bool MOUSEINUSE;
extern VisualObjectList Objects;

bool SocketUsed = false;

extern Bitmap Grid;
extern Bitmap GridID;
extern Bitmap GridBlocking;

extern char TFC_State;

bool MOUSEINUSE = false;
DirectXInput KeyBoard;

extern CV2Sprite MouseCursor1;
//extern CombatCursorVSF MouseCursor2;
//extern CombatCursorVSF MouseCursor3;
extern AnimV2Sprite MouseCursor2;
extern AnimV2Sprite MouseCursor3;
extern CV2Sprite MouseCursor4;
extern CV2Sprite MouseCursor5;
extern CV2Sprite MouseCursor6;
extern CV2Sprite MouseCursor7;
extern CV2Sprite MouseCursor8;
extern CV2Sprite MouseCursor9;
extern CV2Sprite MouseCursor10;
extern CV2Sprite MouseCursor11;
extern CV2Sprite MouseCursor12;
extern CV2Sprite MouseCursor13;
extern CV2Sprite MouseCursor14;
extern CV2Sprite MouseCursor15;
extern AnimV2Sprite MouseCursor16;
extern AnimV2Sprite MouseCursor17;
extern AnimV2Sprite MouseCursor18;
extern int Tick;

int DrawLastMoving = 0;
bool NeedRedraw = false;
bool WantGame = false;
bool WantPreGame = false;

extern bool ScreenDisplayThreadFinished;

extern int NBCURSOR;

Sprite GamaPal[10];
Sprite LoadingGama;

//DIRECTDRAWSURFACE *lplpCursorSurface = NULL;
CV2Sprite **lplpCursor = NULL;

LPDIRECTDRAWSURFACE lpSafe;
BOOL bSafe = FALSE;


//New loading One Shoot Thread...
static BOOL    g_bEnterGamePart1Complete = FALSE;
void    EntergamePart1(LPVOID pParam);

TFCSocket::TFCSocket() {
    // Constructeur
    InitializeCriticalSection(&csTFCSocket);
}

TFCSocket::~TFCSocket() {
    // Destructeur
    DeleteCriticalSection(&csTFCSocket);
}

const int MAXMOV = 20;

bool Attack = false;
bool Get = false;
bool Use = false;
int XAttack;
int YAttack;
int XGet;
int YGet;
int XUse;
int YUse;

bool Patente = false;
sockaddr_in ReceiveIPaddr;
extern TFCSocket Remote;

BACK Backpack[24];

bool stShow = false;
int x = 0, y = 0, button[4];
//	int newbutton = 0; 
DWORD monSleep = 0;

bool NoDRAW = false;
bool bDisplayList = false;
char DisplayList[25][100];
int  DisplayInt[25];
int  nbList = 0;

DWORD dwGetMoveTime;

DWORD Try = 0;

TemplateList <SysMsg> tlSystemMessage;

void ManageAutoMove(char* strKeyInput);


//##NMNMNM --[Main Draw Thread ZONE....]
UINT WINAPI DrawThread(LPVOID pParam) 
{
   g_App.StartThread(APP_DRAWING_THREAD);
   g_TimeStat.SetThreadIDName(GetCurrentThreadId(), "Drawing Thread");
   LOG << "* DRW = Init\r\n";
   
   g_App.SetLoading( false );
   
   string lastText;
   string cachedText;
   MouseDevice Mouse2;
   DWORD tLastFrame = 0;
   DWORD tTimeFrame = 0;
   //DWORD tWaitFPSTime = 1000/16;
   DWORD dwTextOnFullScreenCnt = 0;

   DWORD tLastTick = 0;
   float FramePerSecond=0;//BLBLBL initialisation à 0
   
   //Disabled; seems to cause conflicts w/ 25+ fps. @PoPo
   /*while (!g_Var.inGame)
   {
      Sleep(50);
   }*/

   int dwCode = 0;
   int dwCnt  = 0;
   int RndChk = (rand() % 3000)+2000;
   int RndCrh = (rand() % 1000)+1000;

#if 0
   //sprintf(Buf2, "%s","NightMare"); 
   //sprintf(Buf2, "%s","Marc");     //1-3   (3)
   //sprintf(Buf2, "%s","Lemming");  //2-4   (2)
   //sprintf(Buf2, "%s","Destiny");  //1-4   (1)
   //sprintf(Buf2, "%s","crush");    //1-2-3 (3)
   //sprintf(Buf2, "%s","Heschal");  //3-4   (1)
#endif
   int dwCode1 = 0; //0 to block code
   int dwCode2 = 0; //0 to block code
   char strCode[20];
   switch(dwCode1)
   {
      case 1: sprintf(strCode,"%s","Marc"); break;
      case 2: sprintf(strCode,"%s","Lemming"); break;
      case 3: sprintf(strCode,"%s","Destiny"); break;
      case 4: sprintf(strCode,"%s","Crush"); break;
      case 5: sprintf(strCode,"%s","Heschal"); break;
   } 
   
   while (!g_boQuitApp) 
   {
      g_App.LockDrawThread();

      
      //if ((timeGetTime() - tLastFrame) > (g_Var.tWaitFPSTime)) 
      //{
      //tLastFrame = timeGetTime(); 

      if (NeedRedraw && !DrawLastMoving && !NoDRAW) 
      {
         Move = true;
         DrawLastMoving = DONE;
         World.Lock(115);
         Objects.Lock(1009);
         Objects.GetMainObject()->Direction = Objects.Direction2;
         Objects.MovePlayer(World.MovX, World.MovY);
         World.Switch();
         Objects.Unlock(1009);
         World.Unlock(115);
         
         
         Draw = true;
         
         if (Follow) 
         {
            signed char xDif = 0;                 
            signed char yDif = 0;
            Objects.Lock(117);
            Objects.Found(&xDif, &yDif, FollowID);
            Objects.Unlock(117);
            
            pfSetPosition(xDif+Player.xPos, yDif+Player.yPos);
            if (Attack) 
            {
               XAttack = xDif+Player.xPos;
               YAttack = yDif+Player.yPos;
            }
            pfNearPosition();
            if (Use) 
            {
               pfNearPosition();
            }
         }
         
         if (!pfIsAtPosition()) 
         {
            Try = timeGetTime();
            Move = false;
            dwGetMoveTime = timeGetTime();
            TFCPacket Send;
            
            long NewID = 0;
            short Menrage = (short)pfGetNextMovement();
            Objects.Lock(118);
            switch (Menrage) 
            {
            case 1: NewID = Objects.FoundID2( 0, -1);  break;
            case 2: NewID = Objects.FoundID2( 1, -1);  break;
            case 3: NewID = Objects.FoundID2( 1,  0);  break;
            case 4: NewID = Objects.FoundID2( 1,  1);  break;
            case 5: NewID = Objects.FoundID2( 0,  1);  break;
            case 6: NewID = Objects.FoundID2( -1,  1); break;
            case 7: NewID = Objects.FoundID2( -1,  0); break;
            case 8: NewID = Objects.FoundID2( -1, -1); break;
            }; 
            Objects.Unlock(118);
            
            if (Menrage && (!Attack || !NewID)) 
            {
               Send << Menrage;
               
               SEND_PACKET(Send);
            }
         } 
         else if (Attack) 
         {
            pfStopMovement("Move 4");
            Err = false;
            Follow = false;
            
            TFCPacket Send; 
            
            Send << (short)24;
            Send << (short)XAttack;
            Send << (short)YAttack;
            Send << (long)FollowID;
            
            SEND_PACKET(Send);
            Attack = false;
         } 
         else if (Use) 
         {
            Err = false;
            Follow = false;
            
            TFCPacket Send; 
            
            Send << (short)23;
            Send << (short)XUse;
            Send << (short)YUse;
            Send << (long)FollowID;
            
            SEND_PACKET(Send);
            Use = false;
         } 
         else if (Get) 
         {
            Err = false;
            Follow = false;
            
            TFCPacket Send; 
            
            Send << (short)11;
            Send << (short)XGet;
            Send << (short)YGet;
            Send << (long)FollowID;
            
            SEND_PACKET(Send);
            Get = false;
         }
         
         if (Retrace) 
         {
            Retrace = false;
            Move = false;
            dwGetMoveTime = timeGetTime();
            TFCPacket Send; 
            TFCPacket Send2;
            
            Send << (short)TFCQueryPlayerPosition;
            Send2 << (short)16;
            Send2 << (char)Objects.Direction2;
            Send2 << (short)Player.xPos;
            Send2 << (short)Player.yPos;
            
            SEND_PACKET(Send);
            SEND_PACKET(Send2);
         }
         NeedRedraw = false;
      }


	  //Main Thread @PoPo
      if ((timeGetTime() - tLastFrame) > g_Var.tWaitFPSTime ) 
      {
         tLastFrame = timeGetTime(); 
         MousePos &mp = RootBoxUI::GetInstance()->GetRootScreenOffset();
         bool isMaximized = RootBoxUI::GetInstance()->IsMaximized();
         
         static int toto = 0;
         World.Lock(119);
         Objects.Lock(1010);
         Objects.IsNeedChangeSkin();
         World.UpdateView(Draw, false, mp.x, mp.y); // Draw2
         Objects.Unlock(1010);
         World.Unlock(119);


         
         if (1) 
         {
            // Draw main GUI
            CharacterUI::GetInstance()->DrawStatus();  
            // Draw the UIs
            RootBoxUI::GetInstance()->Draw( NULL );

            //Objects.Lock(1011);
            //Objects.GetMainObject()->DrawTalkText(Objects.ViewWidth*32-32+(mp.x/2),Objects.ViewHeight*16-67+(mp.y/2));
            //Objects.Unlock(1011);

            // Draw Text si le ChatBar ets a OFF
            if(!ChatterUI::GetInstance()->IsShown() && dwTextOnFullScreenCnt >16)
            {
               //Draw le Input texte...
               string txt = ChatterUI::GetInstance()->GetInputText();
               
               if ( !txt.empty() && !mp.y) 
               {
                  HDC hdc;
                  DXDGetDC(&hdc, 27);
                  // If the text has changed since last loop
                  if( txt != lastText )
                  {
                     lastText = txt;
                     cachedText = txt;
                     // Remove characters until the string fits.
                     if( fFont->GetLen( hdc, const_cast< char * >( cachedText.c_str() ) ) > 635 )
                     {
                        while( fFont->GetLen( hdc, const_cast< char * >( cachedText.c_str() ) ) > 635 )
                        {
                           cachedText = cachedText.substr( 1, cachedText.size() - 1 );
                        }
                     }                   
                  }
                  
                  SetTextAlign(hdc, TA_LEFT |TA_TOP | TA_NOUPDATECP);
                  if(!SideMenu::GetInstance()->IsShown())
                  {
                     fFont->DrawFont(hdc, 21-1, g_Global.GetScreenH()-30-ResizeCorrection-1, 0, 0, const_cast< char * >( cachedText.c_str() ) );
                     fFont->DrawFont(hdc, 21+1, g_Global.GetScreenH()-30-ResizeCorrection+1, 0, 0, const_cast< char * >( cachedText.c_str() ) );
                     fFont->DrawFont(hdc, 21,   g_Global.GetScreenH()-30-ResizeCorrection, Custom.gTextColor, 0, const_cast< char * >( cachedText.c_str() ) );
                  }
                  else
                  {
                     fFont->DrawFont(hdc, 201-1, g_Global.GetScreenH()-30-ResizeCorrection-1, 0, 0, const_cast< char * >( cachedText.c_str() ) );
                     fFont->DrawFont(hdc, 201+1, g_Global.GetScreenH()-30-ResizeCorrection+1, 0, 0, const_cast< char * >( cachedText.c_str() ) );
                     fFont->DrawFont(hdc, 201,   g_Global.GetScreenH()-30-ResizeCorrection, Custom.gTextColor, 0, const_cast< char * >( cachedText.c_str() ) );
                  }
                  DXDReleaseDC(hdc, 27);
               }

               // draw tous les texte....
               tlSystemMessage.Lock("Draw");

               if (tlSystemMessage.GetNbObjects() && !mp.y) 
               {
                  tlSystemMessage.ToTail();
                  int Row = 0;
                  int dwCnt = 0;
                  while(tlSystemMessage.QueryPrevious() && dwCnt < 5) 
                  {
                     SysMsg *Msg = tlSystemMessage.GetObject();
                  
                     if(!SideMenu::GetInstance()->IsShown())
                        Msg->Draw( 20, g_Global.GetScreenH()-45 - Row - Msg->GetHeight() );
                     else
                        Msg->Draw( 200, g_Global.GetScreenH()-45 - Row - Msg->GetHeight() );

                     Row += Msg->GetHeight();
                     if (timeGetTime() > Msg->GetTime() && timeGetTime() - Msg->GetTime() > 4000) 
                     {
                        delete Msg;
                        tlSystemMessage.RemoveObject();
                     }
                     dwCnt++;
                  };
               }
               tlSystemMessage.Unlock("Draw"); 
               
            }
            else if(!ChatterUI::GetInstance()->IsShown())
            {
               dwTextOnFullScreenCnt++;
            } 
            else
               dwTextOnFullScreenCnt = 0;
            

            /*
            // Draw main GUI
            CharacterUI::GetInstance()->DrawStatus();  
            // Draw the UIs
            RootBoxUI::GetInstance()->Draw( NULL );
            */

            
            // Draw the helps on top of all UIs.
            RootBoxUI::GetInstance()->DrawHelp( NULL );

            g_SaveGame.SetRTMapVal(Player.xPos,Player.yPos,Player.World);

         }

         if(Custom.NMShowPosition /*&& Player.CanRunScripts*/)
         {
            HDC hdc;
            DXDGetDC(&hdc, 42);
            char Buf[100];

            sprintf(Buf, "%d, %d, %d",Player.xPos, Player.yPos, Player.World); 
            fMenuDescFont->DrawFont(hdc, 5, 5, /*RGB(0 , 101, 255)*/RGB(255,0,0), 0, Buf);
            DXDReleaseDC(hdc, 42);
         }

         if(Custom.NMShowFPS /*&& Player.CanRunScripts*/)
         {
            HDC hdc;
            DXDGetDC(&hdc, 42);
            char Buf[100];

            sprintf(Buf, "fps %d.%d",(int)FramePerSecond,tTimeFrame); 
            fMenuDescFont->DrawFont(hdc, 130, 5, /*RGB(0 , 101, 255)*/RGB(255,0,0), 0, Buf);
            DXDReleaseDC(hdc, 42);
         }
 
         if (Custom.NMDebug) 
         {
            HDC hdc;
            DXDGetDC(&hdc, 41);
            char Buf[100];

            //sprintf(Buf, "XYW( %d, %d, %d )",Player.xPos, Player.yPos, Player.World); 
            //fMenuFont->DrawFont(hdc, 70, 10, RGB(255, 0, 0), 0, Buf);
            //sprintf(Buf, "FPS= %4.02f", FramePerSecond); 
            //fMenuFont->DrawFont(hdc, 70, 35, RGB(255, 0, 0), 0, Buf);
            //sprintf(Buf, "Time= %d", tTimeFrame); 
            //fMenuFont->DrawFont(hdc, 70, 60, RGB(255, 0, 0), 0, Buf);
 
            

            sprintf(Buf, "v. 1.61 beta13  (V2 Team ONLY...)"); 
            int dwXpos = (g_Global.GetScreenW()-264)/2; 
            fMenuFont->DrawFont(hdc, dwXpos, 30, RGB(0 , 101, 255), 0, Buf);
            DXDReleaseDC(hdc, 41);
         }


         if(dwCode1 > 0)
         {
            HDC hdc;
            DXDGetDC(&hdc, 42);
            fMenuDescFont->DrawFont(hdc, 60, 50, RGB(255,0,0), 0, strCode);
            DXDReleaseDC(hdc, 42);
         }

         if(g_Var.wDisplayDisk >0)
         {
            V2SPRITEFX VsfFX;
            ZeroMemory(&VsfFX, sizeof(V2SPRITEFX));
            VsfFX.lpClipRect = new RECT;
            VsfFX.lpClipRect->left = 0;
            VsfFX.lpClipRect->top = 0;
            VsfFX.lpClipRect->right = g_Global.GetScreenW();
            VsfFX.lpClipRect->bottom = g_Global.GetScreenH();
            g_Var.vsfDisk.DrawSpriteN(g_Global.GetScreenW()-g_Var.vsfDisk.GetWidth(),g_Global.GetScreenH()-g_Var.vsfDisk.GetHeight(), &VsfFX);
            delete VsfFX.lpClipRect;
            if(++g_Var.wDisplayDisk >5)
               g_Var.wDisplayDisk = 0;
         }

         if(g_Var.wDisplayMap >0)
         {
            V2SPRITEFX VsfFX;
            ZeroMemory(&VsfFX, sizeof(V2SPRITEFX));
            VsfFX.lpClipRect = new RECT;
            VsfFX.lpClipRect->left = 0;
            VsfFX.lpClipRect->top = 0;
            VsfFX.lpClipRect->right = g_Global.GetScreenW();
            VsfFX.lpClipRect->bottom = g_Global.GetScreenH();
            g_Var.vsfMap.DrawSpriteN(g_Global.GetScreenW()-g_Var.vsfMap.GetWidth(),g_Global.GetScreenH()-g_Var.vsfMap.GetHeight(), &VsfFX);
            delete VsfFX.lpClipRect;
            if(++g_Var.wDisplayMap >5)
               g_Var.wDisplayMap = 0;
         }
            
         if (!COMM.isHalf()) 
         {
            V2SPRITEFX VsfFX;
            ZeroMemory(&VsfFX, sizeof(V2SPRITEFX));
            VsfFX.lpClipRect = new RECT;
            VsfFX.lpClipRect->left = 0;
            VsfFX.lpClipRect->top = 0;
            VsfFX.lpClipRect->right = g_Global.GetScreenW();
            VsfFX.lpClipRect->bottom = g_Global.GetScreenH();
            g_Var.vsfNetwork.DrawSpriteN(g_Global.GetScreenW()-g_Var.vsfNetwork.GetWidth(),g_Global.GetScreenH()-g_Var.vsfNetwork.GetHeight(), &VsfFX);
            delete VsfFX.lpClipRect;
         }

         static int g_chAlphaZone = 0;
         if(g_Global.GetDisplayZone() >=0)
         {
            g_Var.vsfMapZone  .Release();
            g_Var.vsfMapZone  .CreateSprite("MapZoneDisplay");
            g_Var.wDisplayMapZone = 1;
            sprintf(g_Var.strMapZoneName,"%s",g_Global.GetDisplayZoneName());

            HDC hdc;
            g_Var.vsfMapZone.m_lpV2SpriteInfo->Data.lpDDSurface->GetDC(&hdc);

            int dwXPos = (g_Global.GetScreenW()-g_Var.vsfMapZone.GetWidth())/2;
            int dwYPos = (g_Global.GetScreenW()-g_Var.vsfMapZone.GetWidth())/2;
            int dwOffsetX = (182-(fNewGuiBtnFont2->GetLen(hdc,g_Var.strMapZoneName)))/2;
            
            fNewGuiBtnFont2->DrawFont(hdc, 0+dwOffsetX+ 95+1, 0+63+1 , RGB(0, 0, 0)        , 0, g_Var.strMapZoneName);
            fNewGuiBtnFont2->DrawFont(hdc, 0+dwOffsetX+ 95  , 0+63   , g_Global.GetBtnTCE(), 0, g_Var.strMapZoneName);


            g_Var.vsfMapZone.m_lpV2SpriteInfo->Data.lpDDSurface->ReleaseDC(hdc);
            g_chAlphaZone = 0;
         }
         
         if(g_Var.wDisplayMapZone >0)
         {
            V2SPRITEFX VsfFX;
            ZeroMemory(&VsfFX, sizeof(V2SPRITEFX));
            VsfFX.lpClipRect = new RECT;
            VsfFX.lpClipRect->left = (g_Global.GetScreenW()-g_Var.vsfMapZone.GetWidth())/2;
            VsfFX.lpClipRect->top  = 60;
            VsfFX.lpClipRect->right = VsfFX.lpClipRect->left+g_Var.vsfMapZone.GetWidth();
            VsfFX.lpClipRect->bottom = VsfFX.lpClipRect->top+g_Var.vsfMapZone.GetHeight();
            if(g_Var.wDisplayMapZone >0 && g_Var.wDisplayMapZone < 30)
            {
               g_Var.vsfMapZone.DrawSpriteNSemiTrans(VsfFX.lpClipRect->left,VsfFX.lpClipRect->top, &VsfFX,TRUE,g_chAlphaZone);
               g_chAlphaZone+=10;
               if(g_chAlphaZone > 220)
                  g_chAlphaZone = 220;
            }
            else if(g_Var.wDisplayMapZone >29 && g_Var.wDisplayMapZone < 70)
            {
               g_Var.vsfMapZone.DrawSpriteNSemiTrans(VsfFX.lpClipRect->left,VsfFX.lpClipRect->top, &VsfFX,TRUE,220);
            }
            else
            {
               g_Var.vsfMapZone.DrawSpriteNSemiTrans(VsfFX.lpClipRect->left,VsfFX.lpClipRect->top, &VsfFX,TRUE,g_chAlphaZone);
               g_chAlphaZone-=10;
               if(g_chAlphaZone < 10)
                  g_chAlphaZone = 10;
            }
            
            delete VsfFX.lpClipRect;

            if(++g_Var.wDisplayMapZone >100)
               g_Var.wDisplayMapZone = 0;

            //OutputDebugString(g_Var.strMapZoneName);
            //OutputDebugString("\n");
         } 

         //NMNMNM ++Code
         if(dwCode2 > 0)
         {
            V2SPRITEFX VsfFX;
            ZeroMemory(&VsfFX, sizeof(V2SPRITEFX));
            VsfFX.lpClipRect = new RECT;
            VsfFX.lpClipRect->left = 0;
            VsfFX.lpClipRect->top = 0;
            VsfFX.lpClipRect->right = g_Global.GetScreenW();
            VsfFX.lpClipRect->bottom = g_Global.GetScreenH();
            g_Var.vsfCode[dwCode2-1].DrawSpriteN(0,g_Global.GetScreenH()-g_Var.vsfCode[dwCode2-1].GetHeight(), &VsfFX);
            delete VsfFX.lpClipRect;
         }
         
         Mouse2.GetPosition(&x, &y);
         CMouseCursor::GetInstance()->DrawCursor(x, y); // 1   
         x = (x+48-16)/32;
         y = (y-8)/16;
         

         // ici on pourrais enregistrer le backbuffer... si on veux faire un avi...
         //**NMNMNM --[Video record...]
         //g_NMVideoCapture.CaptureFrame();

         if(Custom.TakeScreenShot)
         {
            VideoCapture *pCap = new VideoCapture;
            pCap->TakeDesktopSnapshot();
         
            ClientConfig *pConfig = g_Global.GetClientConfig();
            char strScreenShotPath[1024];
            sprintf(strScreenShotPath,"%s\\ScreenShot",pConfig->strSavePath);
            CreateDirectory( strScreenShotPath, NULL );
            pCap->SaveToFile(strScreenShotPath);
            delete pCap;
            Custom.TakeScreenShot = FALSE;
         }

         DXDFlip();
         
         if (stShow) 
         {
            Objects.Lock(121);
            Objects.SetTotalGrid();
            Objects.Unlock(121);
            stShow = false;
         }
         
         if (DrawLastMoving)
            DrawLastMoving--;
         else
            Draw = false;
         
         if (NoDRAW && !World.Done2)
            DrawFinish = true;

         FramePerSecond = (float)1000/((float)timeGetTime() - (float)tLastTick);
         tLastTick = timeGetTime();
         tTimeFrame = tLastTick- tLastFrame; 
         /*
         if(g_Var.minimizeState)
         {
            Sleep(100);
         }
         else
            Sleep(10);
            */
      }
      else
      {
         if(g_Var.minimizeState)
         {
            Sleep(5);//BLBLBL changé 100 à 5 pour que le jeu aille même vitesse quand réduit
         }
         else
         {
            Sleep(5);
         }
         
      }
      
      g_App.UnlockDrawThread();
   }
   g_App.ExitThread(APP_DRAWING_THREAD);
   _endthreadex(1);
   return 1;
}


int Looping = 0;
int TalkToX = 0, TalkToY = 0;
short TalkToOffset = 0;
unsigned long TalkToID = 0;
int NBCURSOR = 6;
DWORD LastMacro = 0;
DWORD tLastTime = 0;
int   tLastMinute = 0;
int   tLastHour = 0;

bool AskForMenu = false;

DWORD LastAsk3 = 0;

bool ForceSpell = false;

BOOL boKeyProcess = FALSE;

unsigned char *Packet = NULL;

void TFCSocket::MainThread(void) {
    g_App.StartThread(APP_MAINTENANCE_THREAD);
    g_TimeStat.SetThreadIDName(GetCurrentThreadId(), "Maintenace Thread");
    InterPacketEvent = CreateEvent(NULL, false, false, "Socket Event");
    
    //   LPBYTE lpszCommandString = new BYTE [100];
    //   BYTE lpszReturnString[100];
    //            mciSendString("open cdaudio", (char *)lpszReturnString,
    //               sizeof(lpszReturnString), NULL);
    //            mciSendString("set cdaudio time format tmsf", (char *)lpszReturnString, 
    //               sizeof(lpszReturnString), NULL);
    //   GlobalCDTrack = 1;
    //   wsprintf((char *)lpszCommandString, "play cdaudio from %d to %d notify", GlobalCDTrack, GlobalCDTrack+1);
    //   mciSendString((char *)lpszCommandString, (char *)lpszReturnString, 
    //      sizeof(lpszReturnString), siGethWnd());
    //   
    if (TFC_State == TFC_CONNECT) 
    {
        //TFC_State = TFC_LOGO;
		TFC_State = TFC_MENU; //BLBBL on avance plus vite ++
        LastState = 0;
        COMM.LongLive();
        MenuThread();
		KeyBoard.Acquire();
    }
    
    //	sockaddr_in ServerIPaddr;
    
    //CString IP;
    //unsigned int Port;
    time_t Old = 0;
    time_t Old2 = 0;
    //strcpy(Text, "");
    Objects.Direction2 = 1;
    Backpack[0].nbObjects = 0;
    
    TIMESTART = timeGetTime();
    
    COMM.State = 2;
    
    
    
    
    // Variable Initialisation.
    int LastType = 0;
    
    BOOL bSendPreGame = FALSE;
    
    char *KBuffer = new char [256];
    memset(KBuffer, 0, 256);
    //	KeyBoard.Release();
    MouseDevice Mouse2;
    
    while (!g_boQuitApp) {
        g_App.LockMainThread();
        
        
        //Sleep(20); // Seems unnecessary @PoPo
        // While Main Thread
        if (!COMM.isAlive()) {
            LOG << "Ask Close 14\r\n";
            
            ChatterUI::GetInstance()->AddBackscrollItem(
                "",
                g_LocalString[98], //Your connection with the server has been lost.
                RGB(255,255,255), 
                true
                );          
            
            Sleep(2500);
            char Temp[100];
            sprintf(Temp, g_LocalString[98], 24, 0);
            g_App.SetError(1, Temp);
            LOG << "* CLS = 4\r\n";
            PostMessage(siGethWnd(), WM_CLOSE, NULL, 0x6969);
            Sleep(250);
            continue;
        } 
        
        if ( WantPreGame && bSendPreGame ) {//BLBLBL 26 mars test de renvoi de demande de pregame si on recoit vraiment rien
			Sleep (4000); //on attends au moins 4 secondes pour laisser le temps au serveur de répondre

            //on reprépare le paquet et on le renvoie :
			TFCPacket Send;
            Send << (short)13; //PutPlayerIngame
            Send << (char)strlen(MenuName);
            Send << (char *)MenuName;
            SEND_PACKET(Send);

			LOG << "* PAK 13 Ask once more to PutPlayerIngame \r\n";
		}

		//OutputDebugString("ici ici cii \n");
        if (WantPreGame && !bSendPreGame) 
        {
            TFCPacket Send;
            bSendPreGame = TRUE;

            Send << (short)13; //PutPlayerIngame
            Send << (char)strlen(MenuName);
            Send << (char *)MenuName;
            strcpy(Player.Name, MenuName);

            //++NMNMNM --[ Load Savegame Settings]
            g_SaveGame.bLoad(Player.Account,Player.Name);			
            
            SEND_PACKET(Send);
			LOG << "* PAK 13 Ask to PutPlayerIngame \r\n";

            Sleep(250);

            GamaPal[0].LoadPalette("Bright1");
            GamaPal[1].LoadPalette("Bright2");
            GamaPal[2].LoadPalette("Bright3");
            GamaPal[3].LoadPalette("Bright4");
            GamaPal[4].LoadPalette("Bright5");
            GamaPal[5].LoadPalette("Bright6");
            GamaPal[6].LoadPalette("Bright7");
            GamaPal[7].LoadPalette("Bright8");
            GamaPal[8].LoadPalette("Bright9");
            GamaPal[9].LoadPalette("Bright10");

            //Change le loading ICI...
            g_GameMusic.Stop();
            DXDClear();
            DXDFlip();   
            /*DXDClear();*/ //BLBLBL 23 mars 09 : viré ces trucs en doublon

            g_Pal.SetCurrentPal(GamaPal[g_SaveGame.GetOptionsParam()->dwBrightness-1].GetPalette());
            g_Pal.SetVisiblePal(GamaPal[g_SaveGame.GetOptionsParam()->dwBrightness-1].GetPalette());

	 
            IDirectXImage diLoading;
            try 
            {
               // Load.PCX for 800 and 1024
               char strLOADName[20];
               sprintf(strLOADName,"LOAD%d.PCX",g_Global.GetScreenW());
               if (diLoading.LoadImage(strLOADName,g_Global.GetScreenW(),g_Global.GetScreenH())) 
               {
                  DXDClear();
                  DXDBlt(diLoading);
                  DXDFlip();
                  //DXDSetPalette(diLoading);
                  diLoading.Release();
               }
            } 
            catch (...) 
            {
               LOG << "* WRN = LOAD.PCX\r\n";
            };
 
            /*
            FILE *pf = fopen("C:\\!!!Nm_toto.txt","a+");
            fprintf(pf,"StartLoading1\n");
            fclose(pf);
            Objects.Lock(136);
            //Objects.CreatePlayer(); 
            Objects.Unlock(136);
            pf = fopen("C:\\!!!Nm_toto.txt","a+");
            fprintf(pf,"StopLoading1\n");
            fclose(pf);
            */
            
			/*

            g_bEnterGamePart1Complete = FALSE;
            //::MessageBox(NULL,"Start Loadingpart1","",MB_OK);
            _beginthread( EntergamePart1, 0, NULL );


            // CV2Sprite for Progressbar
            int dwX = (g_Global.GetScreenW()-g_Var.vsfProgressB.GetWidth())/2;
            V2SPRITEFX VsfFX;
            ZeroMemory(&VsfFX, sizeof(V2SPRITEFX));
            VsfFX.lpClipRect = new RECT; 
            VsfFX.lpClipRect->left   = dwX;
            VsfFX.lpClipRect->top    = g_Global.GetScreenH()-60;
            VsfFX.lpClipRect->right  = VsfFX.lpClipRect->left + g_Var.vsfProgressB.GetWidth();
            VsfFX.lpClipRect->bottom = VsfFX.lpClipRect->top  + g_Var.vsfProgressB.GetHeight();
            
            g_Var.wProgressCnt = 0;
            g_Var.wProgressInc = 1;
            while(!g_bEnterGamePart1Complete)
            {

               Sleep(100);
               if(diLoading)
                  DXDBlt(diLoading);

               g_Var.vsfProgressB.DrawSpriteNSemiTrans(dwX,g_Global.GetScreenH()-60, &VsfFX,TRUE,200);
               int dwXT =  (dwX+53)+(g_Var.wProgressCnt*7);
               g_Var.vsfProgressT.DrawSpriteNSemiTrans(dwXT,g_Global.GetScreenH()-40, &VsfFX,TRUE,200);
               DXDFlip();
               g_Var.wProgressCnt += g_Var.wProgressInc;
               if(g_Var.wProgressCnt >=39 || g_Var.wProgressCnt <1)
               {
                  g_Var.wProgressInc*=-1;
               }
            }
            if(diLoading)
               diLoading.Release();

            delete VsfFX.lpClipRect;
            //::MessageBox(NULL,"END Loadingpart1","",MB_OK);
            */
        }
        
        
        if (LevelUp) {
            if (timeGetTime() - LastAsk3 > 5000) {
                LevelUp = false;
            }
        }
        
        if (timeGetTime() - tLastTime > 50) 
        {
            tLastTime = timeGetTime();
            g_TimeStructure.AddSeconde();
            /*
            // HELP ADD-ON
            bool findPotion = false;
            bool findTorch = false;
            Player.tlBackpack.Lock("Lock backpack");
            Player.tlBackpack.ToHead();
            while (Player.tlBackpack.QueryNext()) {
                BAG_ITEM *obj = Player.tlBackpack.GetObject();
                if (obj->wBaseID == 40623) {
                    findPotion = true;
                }
                if (obj->wBaseID == 40015) {
                    findTorch = true;
                }
            }
            Player.tlBackpack.Unlock("Lock backpack");
            
            g_EventHelp.Lock();
            if (!findPotion && Player.Level == 1 && Player.Gold < 150 && Player.Hp < Player.MaxHp/2) {
                g_EventHelp.NoMoney = true;
                g_EventHelp.BuyPotion = false;
                g_EventHelp.HP = false;
            } else if (!findPotion && Player.Level == 1 && Player.Hp < Player.MaxHp/2) {
                g_EventHelp.BuyPotion = true;
                g_EventHelp.HP = false;
                g_EventHelp.NoMoney = false;
            } else if (Player.Hp < Player.MaxHp/2 && Player.Level == 1) {
                g_EventHelp.HP = true;
                g_EventHelp.BuyPotion = false;
                g_EventHelp.NoMoney = false;
            } else {
                g_EventHelp.BuyPotion = false;
                g_EventHelp.NoMoney = false;
                g_EventHelp.HP = false;
            }
            
            if (!findTorch && Player.Level == 1) {
                g_EventHelp.BuyTorch = true;
            } else {
                g_EventHelp.BuyTorch = false;
            }
            
            if (Player.StatsPts && Player.Level == 2) {
                g_EventHelp.Stats = true;
            } else {
                g_EventHelp.Stats = false;
            }
            
            if (Player.SkillPts && Player.Level == 2) {
                g_EventHelp.Skills = true;
            } else {
                g_EventHelp.Skills = false;
            }
            g_EventHelp.Unlock();
            
            if (Abs(g_TimeStructure.Minute - tLastMinute) > 5 && g_Var.inGame) {
                tLastMinute = g_TimeStructure.Minute;
                Objects.Lock(138);
                Objects.UpdateLight();
                Objects.Unlock(138);
            }
            if (g_TimeStructure.Hour != tLastHour) {
                
                tLastHour = g_TimeStructure.Hour;
                TFCPacket Send, Send2;
                
                Send << (short)45;
                Send2 << (short)60;
                
                if (g_Var.inGame) {
                    SEND_PACKET(Send);
                    if (!(g_TimeStructure.Hour % 1))
                        SEND_PACKET(Send2);
                }
            }
            */
        }
        Objects.Lock(123);
        Objects.ChkText();
        Objects.Unlock(123); 
        
        
        if (!WantPreGame) 
        {
            if ((timeGetTime() - Try) > 250 && !Move && !NeedRedraw) 
            {
                LOG << "* TST = TimeOut\r\n";
                Move = true;
            }
            
            if (!QuitFirstLoop) { //boucle principale du jeu ?
                
                KeyBoard.KeyBuffer(KBuffer);
                
                ForceSpell = false;
                if (KBuffer[DIK_LCONTROL] || KBuffer[DIK_RCONTROL]) {
                    ForceSpell = true;
                } else {
                }

				
				CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
				ChatterUI *chatterUI = ChatterUI::GetInstance();

				BOOL boIsAnyInterfaceUIBlockingMove = FALSE;
				{	
               if (
						  (chatterUI->IsShown() && chatterUI->IsMinimized() == FALSE)
						  || (ChestUI::GetInstance()->IsShown())
						  || (TradeUI::GetInstance()->IsShown())
						)
					{
						boIsAnyInterfaceUIBlockingMove = TRUE;
					}

				}
               
                
                //ManageAutoMove(KBuffer);
                //Movement function
                
				if (!boKeyProcess && !DoNotMove && Move2 && boIsAnyInterfaceUIBlockingMove == FALSE && (!(KBuffer[DIK_LCONTROL] || KBuffer[DIK_RCONTROL])) && (!(KBuffer[DIK_LSHIFT] || KBuffer[DIK_RSHIFT])))
				{
					short shNLState = GetKeyState(VK_NUMLOCK);
					shNLState = 1 - shNLState;

					if ((KBuffer[DIK_DOWN] || (shNLState && KBuffer[DIK_NUMPAD2])) &&
						(KBuffer[DIK_LEFT] || (shNLState && KBuffer[DIK_NUMPAD4]))) {
						pfStopMovement("Move 5");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(139);
						if (!GridBlocking(9, 16)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)6;//RQ_MoveSouthWest

								SEND_PACKET(Send);
							}
						}
						Objects.Unlock(139);
					}

					if ((KBuffer[DIK_DOWN] || (shNLState && KBuffer[DIK_NUMPAD2])) &&
						(KBuffer[DIK_RIGHT] || (shNLState && KBuffer[DIK_NUMPAD6]))) {
						pfStopMovement("Move 6");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(140);
						if (!GridBlocking(11, 16)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)4;//RQ_MoveSouthEast

								SEND_PACKET(Send);
							}
						}
						Objects.Unlock(140);
					}

					if ((KBuffer[DIK_UP] || (shNLState && KBuffer[DIK_NUMPAD8])) &&
						(KBuffer[DIK_LEFT] || (shNLState && KBuffer[DIK_NUMPAD4]))) {
						pfStopMovement("Move 7");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(141);
						if (!GridBlocking(9, 14)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)8;//RQ_MoveNorthWest

								SEND_PACKET(Send);

							}
						}
						Objects.Unlock(141);
					}

					if ((KBuffer[DIK_UP] || (shNLState && KBuffer[DIK_NUMPAD8])) &&
						(KBuffer[DIK_RIGHT] || (shNLState && KBuffer[DIK_NUMPAD6]))) {
						pfStopMovement("Move 8");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(142);
						if (!GridBlocking(11, 14)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)2;//RQ_MoveNorthWest

								SEND_PACKET(Send);

							}
						}
						Objects.Unlock(142);
					}

					if (KBuffer[DIK_DOWN] || (shNLState && KBuffer[DIK_NUMPAD2])) {
						// Send Mouvement
						pfStopMovement("Move 9");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(143);
						if (!GridBlocking(10, 16)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)5;//RQ_MoveSouth

								SEND_PACKET(Send);

							}
						}
						Objects.Unlock(143);
					}

					if (KBuffer[DIK_UP] || (shNLState && KBuffer[DIK_NUMPAD8])) {
						pfStopMovement("Move 10");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(144);
						if (!GridBlocking(10, 14)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)1;//RQ_MoveNorth

								SEND_PACKET(Send);

							}
						}
						Objects.Unlock(144);
					}

					if (KBuffer[DIK_LEFT] || (shNLState && KBuffer[DIK_NUMPAD4])) {
						pfStopMovement("Move 11");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(145);
						if (!GridBlocking(9, 15)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)7;//RQ_MoveWest

								SEND_PACKET(Send);

							}
						}
						Objects.Unlock(145);
					}

					if (KBuffer[DIK_RIGHT] || (shNLState && KBuffer[DIK_NUMPAD6])) {
						pfStopMovement("Move 12");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(146);
						if (!GridBlocking(11, 15)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)3;//RQ_MoveEast

								SEND_PACKET(Send);

							}
						}
						Objects.Unlock(146);
					}

					if ((shNLState && KBuffer[DIK_HOME]) || (shNLState && KBuffer[DIK_NUMPAD7])) {
						pfStopMovement("Move 13");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(147);
						if (!GridBlocking(9, 14)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)8;//RQ_MoveNorthWest

								SEND_PACKET(Send);
							}
						}
						Objects.Unlock(147);
					}

					if ((shNLState && KBuffer[DIK_END]) || (shNLState && KBuffer[DIK_NUMPAD1])) {
						pfStopMovement("Move 14");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(148);
						if (!GridBlocking(9, 16)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)6;//RQ_MoveSouthWest

								SEND_PACKET(Send);
							}
						}
						Objects.Unlock(148);
					}

					if ((shNLState && KBuffer[DIK_PRIOR]) || (shNLState && KBuffer[DIK_NUMPAD9])) {
						pfStopMovement("Move 15");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(149);
						if (!GridBlocking(11, 14)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)2;//RQ_MoveNorthEast

								SEND_PACKET(Send);
							}
						}
						Objects.Unlock(149);
					}

					if ((shNLState && KBuffer[DIK_NEXT]) || (shNLState && KBuffer[DIK_NUMPAD3])) {
						pfStopMovement("Move 16");
						Follow = false;
						Attack = false;
						Use = false;
						Get = false;
						//Move = true;
						Objects.Lock(150);
						if (!GridBlocking(11, 16)) {
							if (Move) {
								Try = timeGetTime();
								Move = false;
								dwGetMoveTime = timeGetTime();
								dwTimeAsk = timeGetTime();
								TFCPacket Send;
								TFCPacket Info;

								Send << (short)4;//RQ_MoveSouthEast

								SEND_PACKET(Send);

							}
						}
                        Objects.Unlock(150);
      }}
      
      } 
      
      
      
      //Mouse.GetRelease(&button);
   } 
   
   g_App.UnlockMainThread();
   } //	while (!MainThreadFinished) 
   delete KBuffer;
   fFont->Release();
   
   g_App.ExitThread(APP_MAINTENANCE_THREAD);
   _endthreadex(1);
}



void EntergamePart1(LPVOID pParam)
{
   Objects.Lock(136);
   Objects.CreatePlayer(); 
   Objects.Unlock(136);
   g_bEnterGamePart1Complete = TRUE;
}                                

UINT WINAPI MouseActionThread(LPVOID pParam){
    while( !g_boQuitApp ){
        if( !WantPreGame ){
            MouseAction();
        }
        Sleep( 50 );
    }
    return 0;
}



int QN[8];
int RN[8][5];
int QuestionNumber = 0;
BYTE QuestionAnswer[5];

VOID Shuffle(VOID) {
    
    Random Rnd8(0, 7, timeGetTime());
    Random Rnd5(0, 4, timeGetTime());
    
    int i, j, k;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 5; j++)
            RN[i][j] = 5;
    }
    
    for (i = 0; i < 8; i++)
        QN[i] = 6;
    
    for (i = 0; i < 8; i++) {
        QN[i] = Rnd8;
        
        BOOL Okay = TRUE;					
        
        for (j = 0; j < i; j++) {
            if (QN[i] == QN[j])
                Okay = FALSE;
        }
        
        if (!Okay)
            i--;
    }
    
    for (k = 0; k < 8; k++) {		
        for (i = 0; i < 5; i++) {
            RN[QN[k]][i] = Rnd5;
            
            BOOL Okay = TRUE;					
            
            for (j = 0; j < i; j++) {
                if (RN[QN[k]][i] == RN[QN[k]][j])
                    Okay = FALSE;
            }
            
            if (!Okay)
                i--;
        }
    }
}


BOOL IsOnButton(int Check,int x,int y,CV2Sprite *pVsf,int xS, int yS)
{
   if(!Check)
      return FALSE;

   if(x >= xS && x <= xS+pVsf->GetWidth() && y >= yS && y <= yS+pVsf->GetHeight())
      return TRUE;
   return FALSE;
}


namespace{

	T3VSBSound *GUI_BtnDOwn = NULL;

    void InitSound()
	{
        if( !GUI_BtnDOwn )
		{
			GUI_BtnDOwn = new T3VSBSound;
			GUI_BtnDOwn->Create( "Generic Drop Item", TS_MEMORY );
        }
    }
	void FreeSound()
	{
		if(GUI_BtnDOwn)
			delete GUI_BtnDOwn;
		GUI_BtnDOwn = NULL;
	}
};


//##NMNMNM --[Menu thread ZONE... Tous le loading....]
void TFCSocket::MenuThread(void) 
{

   /*IDirectXImage diLoading;
   //Isp.PCX for 800 or 1024...
   char strISPName[20];
   sprintf(strISPName,"ISP%d.PCX",g_Global.GetScreenW());
   diLoading.LoadImage(strISPName,g_Global.GetScreenW(),g_Global.GetScreenH());
   //DXDClear();
   DXDBlt(diLoading);
   DXDFlip();*///BLBLBL test de désactivation ISP.PCX
   
    
   unsigned short  Type;
   TFCPacket      *Msg;
   int x = 0, y = 0, button[4];
   ZeroMemory(button, 4*sizeof(int));
   
   //	CString IP;
   //	unsigned int Port;
   int Chose = 0;
   time_t Old = 0;
   time_t Old2 = 0;
   
   MouseDevice Mouse2;
   if (!AskForMenu) 
   {
      if (!KeyBoard.IsCreated()) 
      {
         LOG << "* DBG = Keyboard Create 2\r\n";
         KeyBoard.Create();
      }
      LOG << "* DBG = Keyboard Create Device 1\r\n";
      KeyBoard.CreateDevice(GUID_SysKeyboard);
      LOG << "* DBG = Keyboard Acquire 2\r\n";
      KeyBoard.Acquire();
      lplpCursor = new CV2Sprite *[19];
      lplpCursor[0] = &MouseCursor1;
      lplpCursor[1] = &MouseCursor2;
      lplpCursor[2] = &MouseCursor3;
      lplpCursor[3] = &MouseCursor4;
      lplpCursor[4] = &MouseCursor14;
      lplpCursor[5] = &MouseCursor15;
      lplpCursor[6] = &MouseCursor14;
      lplpCursor[7] = &MouseCursor5;
      lplpCursor[8] = &MouseCursor6;
      lplpCursor[9] = &MouseCursor7;
      lplpCursor[10] = &MouseCursor8;
      lplpCursor[11] = &MouseCursor9;
      lplpCursor[12] = &MouseCursor10;
      lplpCursor[13] = &MouseCursor11;
      lplpCursor[14] = &MouseCursor12;
      lplpCursor[15] = &MouseCursor13;
      lplpCursor[16] = &MouseCursor16;
      lplpCursor[17] = &MouseCursor17;
      lplpCursor[18] = &MouseCursor18;
      DDSURFACEDESC ddSurfaceDesc;
      ZeroMemory(&ddSurfaceDesc, sizeof(DDSURFACEDESC));
      
      ddSurfaceDesc.dwSize            = sizeof(ddSurfaceDesc);
      ddSurfaceDesc.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
      ddSurfaceDesc.ddsCaps.dwCaps    = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
      ddSurfaceDesc.dwHeight          = 40;
      ddSurfaceDesc.dwWidth           = 47;
      
      DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&ddSurfaceDesc, &lpSafe, NULL), "Add Mouse Cursor 543");
      
      MOUSEINUSE = true;
   }
   else 
   {
      //g_Pal.SetCurrentPal(IntroP[0].GetPalette());
      //g_Pal.SetVisiblePal(IntroP[0].GetPalette());
      AskForMenu = false;
   }
    
   int Frame = 0;
   Force = 9+NBCURSOR;
   
   //LPPALETTEENTRY Dest;
   
   //Dest = new PALETTEENTRY [DEFAULT_COLORS];
   int N = 0;
   
   char Name[50][100];
   short Race[50];
   short Level[50];
   short IsDead[50];
   int DeleteNumber = 0;
   
   for (int i_zeroMemoryCounter=0; i_zeroMemoryCounter<50; i_zeroMemoryCounter++) 
      ZeroMemory(Name[i_zeroMemoryCounter], 100);
   
   ZeroMemory(Level, 100);
   
   DWORD WaitTime;
   int ScrollX = g_Global.GetScreenH()*2;
   
   FormatText Introduction; 
   FormatText Credits;
   FormatText Question[8];
   FormatText Reponse[8][5];
   
   for (int i = 0; i < 8; i++) 
   {
      Question[i].SetText(g_LocalString[106+i*6]);
      
      Question[i].Format(500, fNewGuiBtnFontBig);
      
      Reponse[i][0].SetText(g_LocalString[107+i*6]);
      Reponse[i][1].SetText(g_LocalString[108+i*6]);
      Reponse[i][2].SetText(g_LocalString[109+i*6]);
      Reponse[i][3].SetText(g_LocalString[110+i*6]);
      Reponse[i][4].SetText(g_LocalString[111+i*6]);
      
      Reponse[i][0].Format(500, fNewGuiBtnFont3);
      Reponse[i][1].Format(500, fNewGuiBtnFont3);
      Reponse[i][2].Format(500, fNewGuiBtnFont3);
      Reponse[i][3].Format(500, fNewGuiBtnFont3);
      Reponse[i][4].Format(500, fNewGuiBtnFont3);
   }
   int Selected = 0;
   
   Introduction.SetText(g_LocalString[45]);
   Introduction.SetText("<>");
   Introduction.SetText("<>");
   
   Introduction.SetText(g_LocalString[46]);
   Introduction.SetText("<>");
   Introduction.SetText("<>");
   
   Introduction.SetText(g_LocalString[47]);
   Introduction.SetText("<>");
   Introduction.SetText("<>");
   
   Introduction.SetText(g_LocalString[92]);
   Introduction.SetText("<>");
   Introduction.SetText("<>");
   
   Introduction.SetText(g_LocalString[93]);
   Introduction.SetText("<>");
   Introduction.SetText("<>"); 
     
   Introduction.Format(600, fMenuLittleFont);



   Credits.SetText(">==T4C V2.0 Project Team==<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.6 Coordination Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Marc Frega, Dialsoft Foundator && CEO");
   Credits.SetText("<>");
   Credits.SetText("Loïc [Black Lemming] Jean-Fulcrand, Project Leader");
   Credits.SetText("<>");
   Credits.SetText("Carl [Nightmare] Vachon, Programmer Leader");
   Credits.SetText("<>");
   Credits.SetText("Kitten, Writing Unit Leader");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.6 Coding Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Carl [Nightmare] Vachon, programmer Leader");
   Credits.SetText("<>");
   Credits.SetText("Sébastien [The Destiny] Mériot, Editor's Coder");
   Credits.SetText("<>");
   Credits.SetText("Carlos [FPGA] Lima, Begin Network layer");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.6 Graphics Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Jonathan [Heschal] Pelletier");
   Credits.SetText("<>");
   Credits.SetText("Nadine [Lune] Pelletier");
   Credits.SetText("<>");
   Credits.SetText("Loïc [Black Lemming] Jean-Fulcrand");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.6 World Design Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Nadine [Lune] Pelletier");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.6 Sounds & Musics--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Loïc [Black Lemming] Jean-Fulcrand");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.6 Writers Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Kitten");
   Credits.SetText("<>");
   Credits.SetText("Mouse");
   Credits.SetText("<>");
   Credits.SetText("Kujo");
   Credits.SetText("<>");
   Credits.SetText("Scarlett");
   Credits.SetText("<>");
   Credits.SetText("Redskull");
   Credits.SetText("<>");
   Credits.SetText("Kenko");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("-----------------------------------------------------------------------------");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");



   Credits.SetText(">--1.5 Coordination Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Marc Frega, Dialsoft Foundator && CEO");
   Credits.SetText("<>");
   Credits.SetText("Loïc [Black Lemming] Jean-Fulcrand, Project Leader");
   Credits.SetText("<>");
   Credits.SetText("Carlos [FPGA] Lima, Coders Leader");
   Credits.SetText("<>");
   Credits.SetText("Kitten, Writing Unit Leader");
   Credits.SetText("<>");
   Credits.SetText("Romain [Leo] Wagner, World Design Unit Leader");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.5 Coding Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Carlos [FPGA] Lima");
   Credits.SetText("<>");
   Credits.SetText("Ophise");
   Credits.SetText("<>");
   Credits.SetText("Tiamat");
   Credits.SetText("<>");
   Credits.SetText("Scotch");
   Credits.SetText("<>");
   Credits.SetText("Sébastien [The Destiny] Mériot");
   Credits.SetText("<>");
   Credits.SetText("Desboys");
   Credits.SetText("<>");
   Credits.SetText("Nara");
   Credits.SetText("<>");
   Credits.SetText("Tyrion");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.5 Graphics Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Jonathan [Heschal] Pelletier");
   Credits.SetText("<>");
   Credits.SetText("Loïc [Black Lemming] Jean-Fulcrand");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.5 World Design Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Romain [Leo] Wagner");
   Credits.SetText("<>");
   Credits.SetText("Anadroow");
   Credits.SetText("<>");
   Credits.SetText("Clariss");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.5 Sounds & Musics--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Loïc [Black Lemming] Jean-Fulcrand");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.5 Writers Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Kitten");
   Credits.SetText("<>");
   Credits.SetText("Blade");
   Credits.SetText("<>");
   Credits.SetText("Ostian");
   Credits.SetText("<>");
   Credits.SetText("Reorx");
   Credits.SetText("<>");
   Credits.SetText("Redskull");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("-----------------------------------------------------------------------------");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");


   Credits.SetText(">--1.4 Coordination Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Marc Frega, Dialsoft Foundator && CEO");
   Credits.SetText("<>");
   Credits.SetText("Loïc [Black Lemming] Jean-Fulcrand, Project Leader");
   Credits.SetText("<>");
   Credits.SetText("Carlos [FPGA] Lima, Coders Leader");
   Credits.SetText("<>");
   Credits.SetText("Blade, Writing Leader");
   Credits.SetText("<>");
   Credits.SetText("Romain [Leo] Wagner, World Design Unit Leader");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.4 Coding Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Carlos [FPGA] Lima");
   Credits.SetText("<>");
   Credits.SetText("Ophise");
   Credits.SetText("<>");
   Credits.SetText("Tiamat");
   Credits.SetText("<>");
   Credits.SetText("Scotch");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.4 Graphics Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Loïc [Black Lemming] Jean-Fulcrand");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.4 World Design Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Romain [Leo] Wagner");
   Credits.SetText("<>");
   Credits.SetText("Anadroow");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.4 Writers Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Blade");
   Credits.SetText("<>");
   Credits.SetText("Ostian");
   Credits.SetText("<>");
   Credits.SetText("Reorx");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("-----------------------------------------------------------------------------");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");



   Credits.SetText(">--1.3 Coordination Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Marc Frega, Dialsoft Foundator && CEO");
   Credits.SetText("<>");
   Credits.SetText("Kilivan, Project Leader");
   Credits.SetText("<>");
   Credits.SetText("Carlos [FPGA] Lima, Coders Leader");
   Credits.SetText("<>");
   Credits.SetText("Blade, Writing Leader");
   Credits.SetText("<>");
   Credits.SetText("Romain [Leo] Wagner, World Design Unit Leader");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.3 Coding Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Carlos [FPGA] Lima");
   Credits.SetText("<>");
   Credits.SetText("Scotch");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.3 Graphics Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Loïc [Black Lemming] Jean-Fulcrand");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText(">--1.3 World Design Unit--<");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Romain [Leo] Wagner");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("-----------------------------------------------------------------------------");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("<>");

   Credits.SetText("<>");
   Credits.SetText("<>");
   Credits.SetText("Thanks to all the Sysops, HGM and GM who shared their");
   Credits.SetText("<>");
   Credits.SetText("time to help us hunting the bugs !");
   Credits.SetText("<>");
   Credits.SetText("Thanks to all players, thanks for your patience");
   Credits.SetText("<>");
   Credits.SetText("suggestions and support ! Thanks to Marc, who made this");
   Credits.SetText("<>");
   Credits.SetText("project possible.");
   Credits.SetText("<>");
   Credits.SetText("Thanks to all people we forgot and who helped us");
   Credits.SetText("<>");
   Credits.SetText("at one time or an other !");
   Credits.SetText("<>");
   Credits.SetText("Thanks to Darkfang for giving gold to newbies..");
   Credits.SetText("<>");
   Credits.SetText("Thanks Mestoph & Chaotikmind for your help debugging !");
   Credits.SetText("<>");
   Credits.Format(600, fMenuLittleFont);

  
   
   
   V2SPRITEFX vsffx;
   ZeroMemory(&vsffx, sizeof(V2SPRITEFX));
   RECT rect;
   rect.left = 0;
   rect.top = 0;
   rect.right = g_Global.GetScreenW();
   rect.bottom = g_Global.GetScreenH();
   vsffx.lpClipRect = &rect;
   vsffx.dwFX |= FX_FIT2SCREEN;

   V2SPRITEFX vsffx2;
   ZeroMemory(&vsffx2, sizeof(V2SPRITEFX));
   vsffx2.lpClipRect = &rect;

  
   int RegisterCount = 0;
   
   Register = true;
   
   BYTE QuestionChoice = 1;
   char NbOfPlayer = 0;
   
   BYTE nameIsNotOk = 0;
   DWORD dwLastUse = timeGetTime();

   g_GameMusic.Start();

   int NM_FPVisible = 0;
   int NM_MPVisible = 4;

   char strErrRaisonL1[500];
   char strErrRaisonL2[500];

   InitSound();

  
   WaitTime = timeGetTime();
   while (!MenuThreadFinished && !g_boQuitApp) 
   {
      g_App.LockMainThread();
      Sleep(25);
      
      // While Main Thread
      if (timeGetTime() - WaitTime > 1000) //Smoothen the improved FPS 2000>>1000
      {
         WaitTime = timeGetTime();
         
         if (timeGetTime() - dwLastUse > 90000) 
         {
            TFC_State = TFC_WARNING_2;
            strcpy(strErrRaisonL1,  g_LocalString[ 497 ] );
            strcpy(strErrRaisonL2, g_LocalString[ 498 ] );
         }
         
         if (RegisterCount == 1) 
         {
            TFCPacket Send;
            TFCPacket Info;
            
            if (AskForMenu) 
            {
               Send << (short)38;//RQ_ReturnToMenu
            } 
            else 
            {
               Send << (short)TFCRegisterAccount;
               Send << (char)strlen(Player.Account);
               Send << (char *)Player.Account;
               Send << (char)strlen(Player.Password);
               Send << (char *)Player.Password;
               Send << (short)Player.Version;
               Send << (short)0;
            }
            SEND_PACKET(Send);
         }
      }
      
      ScrollX-=2;


      // display du back de toute les page apres le splash et le LOGO !!!!!!!
      int dwOffset = 20;
      if(g_Global.GetScreenW() == 720) // for 1280x720
         dwOffset = 40;
      else if(g_Global.GetScreenW() == 900) // for 1600x900
         dwOffset = 60;

      int dwTitemEndPos = dwOffset+Connect_Title2.GetHeight();
      static int dwDisplayIndex  = 0;
      static int dwFrameRepeat   = 0;

      static int dwDisplayIndexLA  = 0;
      static int dwFrameRepeatLA   = 0;


      if (TFC_State >= TFC_INTRODUCTION ) 
      {
         DXDClear();
         Intro[dwDisplayIndex].DrawSpriteN(0, 0, &vsffx2);
         
         if(TFC_State != TFC_INTRODUCTION && TFC_State != TFC_VIEW_CREDITS )
         {
            


            Connect_Title2.DrawSpriteN((g_Global.GetScreenW()-Connect_Title2.GetWidth())/2, dwOffset, &vsffx2);
            //Connect_D    .DrawSpriteN(10                           , g_Global.GetScreenH()-85, &vsffx2);

            Connect_D_Anim[dwDisplayIndexLA].DrawSpriteN(15 , g_Global.GetScreenH()-Connect_D_Anim[dwDisplayIndexLA].GetHeight()-15, &vsffx2);

            dwFrameRepeatLA++;
            if( dwFrameRepeatLA > 1)
            {
               dwFrameRepeatLA = 0;
               dwDisplayIndexLA++;
               if(dwDisplayIndexLA>=36)
               {
                  dwDisplayIndexLA = 0;
               }
            }
         }
         
         
         dwFrameRepeat++;
         int dwChangeCondition = 4;
         if(dwDisplayIndex == 0)
            dwChangeCondition = 4;
         if( dwFrameRepeat > dwChangeCondition)
         {
            dwFrameRepeat = 0;
            dwDisplayIndex++;
            if(dwDisplayIndex>=NM_CONNECT_SCREEN_NBR_BACK_ANIM)
            {
               dwDisplayIndex = 0;
            }
         }
      }

      // reagrde l<etat de la souris...
    
      // DRAW the screen for good state...
      
      switch (TFC_State) 
      {
         case TFC_LOGO://OKOKOK
     //       DXDBlt(diLoading);
     //       DXDFlip();
            Frame++;
            N = 100;
            if (Frame > 50) 
            {
               TFC_State = TFC_SPLASH;
               Frame = 0;
            }
         break;
         case TFC_SPLASH://OKOKOK
            DXDClear();
            Splash.DrawSpriteN(0, 0, &vsffx);
            Frame++;
            if (Frame > 50) //BLBLB on avance plus vite que 100
            {
               TFC_State = TFC_MENU;
            }
         break;
         case TFC_INTRODUCTION: 
         {  
            OutputDebugString("TFC_INTRODUCTION\n");
            HDC hdc;
            DXDGetDC(&hdc, 33);
            
            for (int i = 0; i < Introduction.GetnbLine(); i++) 
            {
               fMenuLittleFont->DrawFont(hdc, (g_Global.GetScreenW()-fMenuLittleFont->GetLen(hdc, Introduction.GetText(i)))/2, (ScrollX/2)+i*fMenuLittleFont->GetHeight(hdc), RGB(255, 255, 255), 0, Introduction.GetText(i));
            }
            DXDReleaseDC(hdc, 33);
         } 
         break;
         case TFC_VIEW_CREDITS: 
         {
            HDC hdc;
            DXDGetDC(&hdc, 34);
            
            for (int i = 0; i < Credits.GetnbLine(); i++) {
               fMenuLittleFont->DrawFont(hdc, (g_Global.GetScreenW()-fMenuLittleFont->GetLen(hdc, Credits.GetText(i)))/2, (ScrollX/2)+i*fMenuLittleFont->GetHeight(hdc), RGB(255, 255, 255), 0, Credits.GetText(i));
            }
            DXDReleaseDC(hdc, 34);
         } 
         break;
         case TFC_MENU: //OKOKOK
         {
            int dwXBox =  (g_Global.GetScreenW()-Connect_Main_Back.GetWidth())/2;
            int dwYBox =  (g_Global.GetScreenH()-Connect_Main_Back.GetHeight())/2+30;
            Connect_Main_Back .DrawSpriteNSemiTrans(dwXBox,dwYBox,&vsffx2,TRUE,180);
            
            int dwXBtn = dwXBox+14;
            int dwYBtn = dwYBox+38;
            
            int x = 0;
            int y = 0;
            int BtnClick = 0;
            Mouse2.GetPosition(&x, &y);
            
            for(int ib=0;ib<4;ib++)
            {
               if(x >= dwXBtn && x <= dwXBtn+Connect_Main_BtnN.GetWidth() && y >= dwYBtn && y <= dwYBtn+Connect_Main_BtnN.GetHeight()   )
                   Connect_Main_BtnH .DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
               else
                  Connect_Main_BtnN .DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
               dwYBtn+=32;
            }
            
            dwXBtn = dwXBox+14;
            dwYBtn = dwYBox+38;
            HDC hdc;
            DXDGetDC(&hdc, 36);
            int dwTPos = 0;
            for(int ib=0;ib<4;ib++)
            {
               if(ib == 0)
                  dwTPos = 2;
               else if(ib == 1)
                  dwTPos = 3;
               else if(ib == 2)
                  dwTPos = 5;
               else if(ib == 3)
                  dwTPos = 6;

               char Temp2[2];
               Temp2[0] = g_LocalString[dwTPos][0];
               Temp2[1] = NULL;
               int dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[dwTPos]);
               dwVal = (192-dwVal)/2;
               
               fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal+1,  dwYBtn+2+1, RGB(190, 180, 140), 0, Temp2);
               fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal  ,  dwYBtn+2  , RGB(0,0,0)        , 0, g_LocalString[dwTPos]);
               dwYBtn+=32;
            }
            DXDReleaseDC(hdc, 36);
         } 
         break;
         case TFC_CONFIRM_DELETE:
         case TFC_CHOOSE_NAME: 
         case TFC_CHOOSE_PLAYER: //OKOKOK
         case TFC_ASK_CREATE_CHARACTER:
         case TFC_CHOOSE_SEXX: 
         {
            int dwXBox =  (g_Global.GetScreenW()-PS_Back.GetWidth())/2;
            int dwYBox =  (g_Global.GetScreenH()-PS_Back.GetHeight())/2+30;
            PS_Back.DrawSpriteNSemiTrans(dwXBox,dwYBox,&vsffx2,TRUE,180);
            if (TFC_State == TFC_CONFIRM_DELETE || TFC_State == TFC_CHOOSE_NAME || TFC_State == TFC_CHOOSE_SEXX)
            {
               PS_BackDown.DrawSpriteNSemiTrans(dwXBox,dwYBox+PS_Back.GetHeight(),&vsffx2,TRUE,180);
            }
            if(TFC_State == TFC_CHOOSE_NAME && nameIsNotOk != 0)
            {

               PS_BackDown.DrawSpriteNSemiTrans(dwXBox,dwYBox+PS_Back.GetHeight()+PS_BackDown.GetHeight(),&vsffx2,TRUE,180);
            }
            
            int dwXBtn = dwXBox+318;
            int dwYBtn = dwYBox+67;
            
            int x = 0;
            int y = 0;
            int BtnClick = 0;
            Mouse2.GetPosition(&x, &y);

            if (TFC_State == TFC_CHOOSE_PLAYER)
            {
               for(int ib=0;ib<3;ib++)
               {
                  if(ib == 1 && NbOfPlayer >= MaxCharactersPerAccount) 
                  {
                     //PS_Btn_D.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
                  }
                  else
                  {
                     if(x >= dwXBtn && x <= dwXBtn+PS_Btn_H.GetWidth() && y >= dwYBtn && y <= dwYBtn+PS_Btn_H.GetHeight()   )
                        PS_Btn_H.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
                     else
                        PS_Btn_N.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
                  }
                  dwYBtn+=32;
               }

               dwYBtn = dwYBox+177;
               if(x >= dwXBtn && x <= dwXBtn+PS_Btn_H.GetWidth() && y >= dwYBtn && y <= dwYBtn+PS_Btn_H.GetHeight()   )
                  PS_Btn_H.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
               else
                  PS_Btn_N.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);

               //if(NbOfPlayer >NM_MPVisible)
               {
                  //UP
                  dwXBtn = dwXBox+278;
                  dwYBtn = dwYBox+76;
                  if(x >= dwXBtn && x <= dwXBtn+PS_SBtnUp_H.GetWidth() && y >= dwYBtn && y <= dwYBtn+PS_SBtnUp_H.GetHeight()   )
                     PS_SBtnUp_H.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
                  else
                     PS_SBtnUp_N.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);

                  //Down
                  dwXBtn = dwXBox+278;
                  dwYBtn = dwYBox+170;
                  if(x >= dwXBtn && x <= dwXBtn+PS_SBtnDn_H.GetWidth() && y >= dwYBtn && y <= dwYBtn+PS_SBtnDn_H.GetHeight()   )
                     PS_SBtnDn_H.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
                  else
                     PS_SBtnDn_N.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
               }
            }
            else if (TFC_State == TFC_CONFIRM_DELETE || TFC_State == TFC_CHOOSE_SEXX)
            {
               int dwXBtn = dwXBox+289;
               int dwYBtn = dwYBox+243;
               for(int ib=0;ib<2;ib++)
               {
                  if(x >= dwXBtn && x <= dwXBtn+PS_SmallBtn_H.GetWidth() && y >= dwYBtn && y <= dwYBtn+PS_SmallBtn_H.GetHeight()   )
                     PS_SmallBtn_H.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
                  else
                     PS_SmallBtn_N.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);

                  dwXBtn+=78;
               }
            }


            int dwXSel = dwXBox+16;
            int dwYSel = dwYBox+76;
            //dessine la liste des players....
            for (int i = NM_FPVisible,j=0; i < NM_MPVisible+NM_MPVisible; i++,j++) 
            {
               if(i < NbOfPlayer)
               {
                  if (Selected == i)
                  {
                     PS_Over.DrawSpriteN(dwXSel,dwYSel,&vsffx2);
                  }
                  dwYSel+=31;
               }
            }

            HDC hdc;
            DXDGetDC(&hdc, 36);
			int ib;
            if (TFC_State == TFC_CHOOSE_PLAYER)
            {
               dwXBtn = dwXBox+318;
               dwYBtn = dwYBox+67;
               
               int dwTPos = 0;
               for(ib=0;ib<3;ib++)
               {
                  if(ib == 0)
                     dwTPos = 105;    //entrer
                  else if(ib == 1)
                     dwTPos = 4;    //Create
                  else if(ib == 2)
                     dwTPos = 7;    //delete

                  if(ib == 1 && NbOfPlayer >= MaxCharactersPerAccount) 
                  {
                  }
                  else
                  {
                     char Temp2[2];
                     Temp2[0] = g_LocalString[dwTPos][0];
                     Temp2[1] = NULL;
                     int dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[dwTPos]);
                     dwVal = (116-dwVal)/2;
                     fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal+1,  dwYBtn+2+1, RGB(190, 180, 140), 0, Temp2);
                     fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal  ,  dwYBtn+2  , RGB(0,0,0)        , 0, g_LocalString[dwTPos]);
                  }
                  dwYBtn+=32;
               }

               dwYBtn = dwYBox+177;
               if(ib == 3)
                  dwTPos = 21;  //retour
               char Temp2[2];
               Temp2[0] = g_LocalString[dwTPos][0];
               Temp2[1] = NULL;
               int dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[dwTPos]);
               dwVal = (116-dwVal)/2;
               fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal+1,  dwYBtn+2+1, RGB(190, 180, 140), 0, Temp2);
               fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal  ,  dwYBtn+2  , RGB(0,0,0)        , 0, g_LocalString[dwTPos]);

            }
            else if (TFC_State == TFC_CONFIRM_DELETE)
            {
               int dwXBtn = dwXBox+289;
               int dwYBtn = dwYBox+243;
               int dwTPos = 0;
               for(int ib=0;ib<2;ib++) 
               {
                  if(ib == 0)
                     dwTPos = 25;    //yes
                  else if(ib == 1)
                     dwTPos = 26;    //No

                  char Temp2[2];
                  Temp2[0] = g_LocalString[dwTPos][0];
                  Temp2[1] = NULL;
                  int dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[dwTPos]);
                  dwVal = (72-dwVal)/2;
                  
                  fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal+1,  dwYBtn+2+1, RGB(190, 180, 140), 0, Temp2);
                  fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal  ,  dwYBtn+2  , RGB(0,0,0)        , 0, g_LocalString[dwTPos]);
                  dwXBtn+=78;
               }
            }
            else if(TFC_State == TFC_CHOOSE_SEXX)
            {
               int dwXBtn = dwXBox+289;
               int dwYBtn = dwYBox+243;
               int dwTPos = 0;
               for(int ib=0;ib<2;ib++) 
               {
                  if(ib == 0)
                     dwTPos = 178;    //
                  else if(ib == 1)
                     dwTPos = 179;    //
                  
                  char Temp2[2];
                  Temp2[0] = g_LocalString[dwTPos][0];
                  Temp2[1] = NULL;
                  int dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[dwTPos]);
                  dwVal = (72-dwVal)/2;
                  
                  fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal+1,  dwYBtn+2+1, RGB(190, 180, 140), 0, Temp2);
                  fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal  ,  dwYBtn+2  , RGB(0,0,0)        , 0, g_LocalString[dwTPos]);
                  dwXBtn+=78;
               }
            }

            char LevelTemp1[20];
            char LevelTemp2[20];

            //l<entete. des collone
            int dwXName = dwXBox+16;
            int dwYName = dwYBox+44;
            
            int dwVal = (191-fNewGuiBtnFont->GetLen(hdc, g_LocalString[576]))/2;
            fNewGuiBtnFont->DrawFont(hdc, dwXName+dwVal+1,  dwYName+1, RGB(222, 158, 0), 0, g_LocalString[576]);

            dwXName = dwXBox+215;
            dwVal = (55-fNewGuiBtnFont->GetLen(hdc, g_LocalString[39]))/2;
            fNewGuiBtnFont->DrawFont(hdc, dwXName+dwVal+1,  dwYName+1, RGB(222, 158, 0), 0, g_LocalString[39]);


 
            dwXName = dwXBox+16;
            dwYName = dwYBox+76;
            //dessine la liste des players....
			int i;
            for (i = NM_FPVisible; i < NM_FPVisible+NM_MPVisible; i++) 
            { 
               if(i < NbOfPlayer)
               {
                  sprintf(LevelTemp1, " %s",itoa(Level[i], LevelTemp2, 10));
                  sprintf(LevelTemp2, "%u. %s",i+1,Name[i]);
                  if(IsDead[i])
                  {
                     fMenuLittleFont->DrawFont(hdc, dwXName+10 , dwYName, RGB(225, 50, 50), 0, LevelTemp2);
                     fMenuLittleFont->DrawFont(hdc, dwXName+205, dwYName, RGB(225, 50, 50), 0, LevelTemp1);
                  }
                  else
                  {
                     fMenuLittleFont->DrawFont(hdc, dwXName+10 , dwYName, RGB(255, 255, 255), 0, LevelTemp2);
                     fMenuLittleFont->DrawFont(hdc, dwXName+205, dwYName, RGB(255, 255, 255), 0, LevelTemp1);
                  }

                  dwYName+=30;
               }
            }
            if(TFC_State == TFC_CONFIRM_DELETE)
            {
               dwXName = dwXBox+16;
               dwYName = dwYBox+244;
               fNewGuiBtnFont->DrawFont(hdc, dwXName+dwVal+1,  dwYName+1, RGB(222, 158, 0), 0, g_LocalString[577]);
            }
            else if(TFC_State == TFC_CHOOSE_NAME)
            {
               char Temp[100];
               strcpy(Temp, Player.Name);
               strcat(Temp, "_");//DC
               
               dwXName = dwXBox+16;
               dwYName = dwYBox+244;
               fNewGuiBtnFont->DrawFont(hdc, dwXName+dwVal+1  ,  dwYName+1, RGB(222, 158,  0), 0, g_LocalString[19]);
               fNewGuiBtnFont->DrawFont(hdc, dwXName+dwVal+285,  dwYName+1, RGB(255, 255,255), 0, Temp);
               
               dwXName = dwXBox+6;
               dwYName = dwYBox+309;
               if (nameIsNotOk == 1) 
               {
                  int dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[499]);
                  dwVal = (441-dwVal)/2;
                  fNewGuiBtnFont->DrawFont(hdc, dwXName+dwVal, dwYName, RGB(160,20,20), 0, g_LocalString[ 499 ] );
               } 
               else if (nameIsNotOk == 2) 
               {
                  int dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[521]);
                  dwVal = (441-dwVal)/2;
                  fNewGuiBtnFont->DrawFont(hdc, dwXName+dwVal, dwYName, RGB(160,20,20), 0, g_LocalString[ 521 ] );
               }
            }
            else if(TFC_State == TFC_CHOOSE_SEXX)
            {
               dwXName = dwXBox+16;
               dwYName = dwYBox+244;
               char strTmp[100];
               sprintf(strTmp,"%s (%s)",g_LocalString[578],Player.Name);
               fNewGuiBtnFont->DrawFont(hdc, dwXName+dwVal+1,  dwYName+1, RGB(222, 158, 0), 0, strTmp);
            }

            
            DXDReleaseDC(hdc, 36);

         }
         break;
         case TFC_CHOOSE_RACE: 
         {
            int dwXBox =  (g_Global.GetScreenW()-Q_Back.GetWidth())/2;
            int dwYBox =  dwTitemEndPos+20;
            if(dwYBox+Q_Back.GetHeight() > g_Global.GetScreenH())
               dwYBox =g_Global.GetScreenH()-Q_Back.GetHeight();
            
            Q_Back.DrawSpriteNSemiTrans(dwXBox,dwYBox,&vsffx2,TRUE,180);
            Q_BackSelect.DrawSpriteN(dwXBox+17,(dwYBox+106)+((QuestionChoice-1)*48),&vsffx2);

            int dwXBtn = dwXBox+537;
            int dwYBtn = dwYBox+265;
            for(int ib=0;ib<2;ib++)
            {
               if(x >= dwXBtn && x <= dwXBtn+PS_SmallBtn_H.GetWidth() && y >= dwYBtn && y <= dwYBtn+PS_SmallBtn_H.GetHeight()   )
                  PS_SmallBtn_H.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
               else
                  PS_SmallBtn_N.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
               
               dwYBtn+=48;
            }
           
            HDC hdc;
            DXDGetDC(&hdc, 39);
            if (QuestionNumber < 4) 
            {
               int i;
               
               //Question
               for (i = 0; i < Question[QN[QuestionNumber]].GetnbLine(); i++)
                  fNewGuiBtnFontBig->DrawFont(hdc, dwXBox+18, dwYBox+34+i*fNewGuiBtnFontBig->GetHeight(hdc), RGB(222, 158,  0), 0, Question[QN[QuestionNumber]].GetText(i));

               //reponse 1
               for (i = 0; i < Reponse[QN[QuestionNumber]][RN[QuestionNumber][0]].GetnbLine(); i++)
                  fNewGuiBtnFont3->DrawFont(hdc, dwXBox+18, (dwYBox+106)+(i*fNewGuiBtnFont3->GetHeight(hdc)), RGB(255, 255, 255), 0, Reponse[QN[QuestionNumber]][RN[QuestionNumber][0]].GetText(i));

               //reponse 2
               for (i = 0; i < Reponse[QN[QuestionNumber]][RN[QuestionNumber][1]].GetnbLine(); i++)
                  fNewGuiBtnFont3->DrawFont(hdc, dwXBox+18, (dwYBox+154)+(i*fNewGuiBtnFont3->GetHeight(hdc)), RGB(255, 255, 255), 0, Reponse[QN[QuestionNumber]][RN[QuestionNumber][1]].GetText(i));

               //reponse 3
               for (i = 0; i < Reponse[QN[QuestionNumber]][RN[QuestionNumber][2]].GetnbLine(); i++)
                  fNewGuiBtnFont3->DrawFont(hdc, dwXBox+18, (dwYBox+202)+(i*fNewGuiBtnFont3->GetHeight(hdc)), RGB(255, 255, 255), 0, Reponse[QN[QuestionNumber]][RN[QuestionNumber][2]].GetText(i));

               //reponse 4
               for (i = 0; i < Reponse[QN[QuestionNumber]][RN[QuestionNumber][3]].GetnbLine(); i++)
                  fNewGuiBtnFont3->DrawFont(hdc, dwXBox+18, (dwYBox+250)+(i*fNewGuiBtnFont3->GetHeight(hdc)), RGB(255, 255, 255), 0, Reponse[QN[QuestionNumber]][RN[QuestionNumber][3]].GetText(i));

               //reponse 5
               for (i = 0; i < Reponse[QN[QuestionNumber]][RN[QuestionNumber][4]].GetnbLine(); i++)
                  fNewGuiBtnFont3->DrawFont(hdc, dwXBox+18, (dwYBox+298)+(i*fNewGuiBtnFont3->GetHeight(hdc)), RGB(255, 255, 255), 0, Reponse[QN[QuestionNumber]][RN[QuestionNumber][4]].GetText(i));

               dwXBtn = dwXBox+537;
               dwYBtn = dwYBox+265;
               int dwTPos = 0;
               for(int ib=0;ib<2;ib++) 
               {
                  if(ib == 0)
                     dwTPos = 20;    //
                  else if(ib == 1)
                     dwTPos = 21;    //
                  
                  char Temp2[2];
                  Temp2[0] = g_LocalString[dwTPos][0];
                  Temp2[1] = NULL;
                  int dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[dwTPos]);
                  dwVal = (72-dwVal)/2;
                  
                  fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal+1,  dwYBtn+2+1, RGB(190, 180, 140), 0, Temp2);
                  fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal  ,  dwYBtn+2  , RGB(0,0,0)        , 0, g_LocalString[dwTPos]);
                  dwYBtn+=48;
               }
            }
            DXDReleaseDC(hdc, 39);
         } 
         break;
         
         case TFC_MAKE_REROLL: 
            {

               int dwXBox =  (g_Global.GetScreenW()-J_Back.GetWidth())/2;
               int dwYBox =  (g_Global.GetScreenH()-J_Back.GetHeight())/2+30;
               J_Back.DrawSpriteNSemiTrans(dwXBox,dwYBox,&vsffx2,TRUE,180);
            
               int dwXBtn = dwXBox+306;
               int dwYBtn = dwYBox+74;
            
               int x = 0;
               int y = 0;
               int BtnClick = 0;
               Mouse2.GetPosition(&x, &y);

               for(int ib=0;ib<2;ib++)
               {
                  if(x >= dwXBtn && x <= dwXBtn+PS_Btn_H.GetWidth() && y >= dwYBtn && y <= dwYBtn+PS_Btn_H.GetHeight()   )
                     PS_Btn_H.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
                  else
                     PS_Btn_N.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
                  dwYBtn+=32;
               }

               dwYBtn = dwYBox+178;
               if(x >= dwXBtn && x <= dwXBtn+PS_Btn_H.GetWidth() && y >= dwYBtn && y <= dwYBtn+PS_Btn_H.GetHeight()   )
                  PS_Btn_H.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);
               else
                  PS_Btn_N.DrawSpriteN(dwXBtn,dwYBtn,&vsffx2);

               HDC hdc;
               DXDGetDC(&hdc, 22);

               //dessine le titre...
               int dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[180]);
               dwVal = (254-dwVal)/2;


               fMenuDescFont->DrawFont(hdc, dwXBox+dwVal, dwYBox+44, RGB(222, 158,  0), 0, g_LocalString[180]);
               
               fMenuDescFont->DrawFont(hdc, dwXBox+22, dwYBox+78+0  , RGB(128, 128, 128), 0, g_LocalString[31]);
               fMenuDescFont->DrawFont(hdc, dwXBox+22, dwYBox+78+20 , RGB(128, 128, 128), 0, g_LocalString[32]);
               fMenuDescFont->DrawFont(hdc, dwXBox+22, dwYBox+78+40 , RGB(128, 128, 128), 0, g_LocalString[33]);
               fMenuDescFont->DrawFont(hdc, dwXBox+22, dwYBox+78+60 , RGB(128, 128, 128), 0, g_LocalString[35]);
               fMenuDescFont->DrawFont(hdc, dwXBox+22, dwYBox+78+80 , RGB(128, 128, 128), 0, g_LocalString[36]);
			   //fMenuDescFont->DrawFont(hdc, dwXBox+22, dwYBox+78+100, RGB(128, 128, 128), 0, g_LocalString[37]);
               fMenuDescFont->DrawFont(hdc, dwXBox+22, dwYBox+78+100, RGB(128, 128, 128), 0, g_LocalString[38]);
               
               char Chiffre[10];
               fMenuDescFont->DrawFont(hdc, dwXBox+175, dwYBox+78+0  , RGB(255, 255, 255), 0, itoa(Player.Str,  Chiffre, 10));
               fMenuDescFont->DrawFont(hdc, dwXBox+175, dwYBox+78+20 , RGB(255, 255, 255), 0, itoa(Player.End,  Chiffre, 10));
               fMenuDescFont->DrawFont(hdc, dwXBox+175, dwYBox+78+40 , RGB(255, 255, 255), 0, itoa(Player.Agi,  Chiffre, 10));
               fMenuDescFont->DrawFont(hdc, dwXBox+175, dwYBox+78+60 , RGB(255, 255, 255), 0, itoa(Player.Wis,  Chiffre, 10));
               fMenuDescFont->DrawFont(hdc, dwXBox+175, dwYBox+78+80 , RGB(255, 255, 255), 0, itoa(Player.Int,  Chiffre, 10));
			   //fMenuDescFont->DrawFont(hdc, dwXBox+175, dwYBox+78+100, RGB(255, 255, 255), 0, itoa(Player.Lck,  Chiffre, 10));
               fMenuDescFont->DrawFont(hdc, dwXBox+175, dwYBox+78+100, RGB(255, 255, 255), 0, itoa(Player.Hp,   Chiffre, 10));


               //texte sur les bouton...
               dwXBtn = dwXBox+306;
               dwYBtn = dwYBox+74;
               int dwTPos = 0;
			   int ib;
               for(ib=0;ib<2;ib++)
               {
                  if(ib == 0)
                     dwTPos = 22;    //
                  else if(ib == 1)
                     dwTPos = 23;    //
                  
                  char Temp2[2];
                  Temp2[0] = g_LocalString[dwTPos][0];
                  Temp2[1] = NULL;
                  int dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[dwTPos]);
                  dwVal = (116-dwVal)/2;
                  
                  fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal+1,  dwYBtn+2+1, RGB(190, 180, 140), 0, Temp2);
                  fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal  ,  dwYBtn+2  , RGB(0,0,0)        , 0, g_LocalString[dwTPos]);
                  dwYBtn+=32;
               }
               dwTPos = 21;    //
               dwYBtn = dwYBox+178;
               char Temp2[2];
               Temp2[0] = g_LocalString[dwTPos][0];
               Temp2[1] = NULL;
               dwVal = fNewGuiBtnFont->GetLen(hdc, g_LocalString[dwTPos]);
               dwVal = (116-dwVal)/2;
               
               fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal+1,  dwYBtn+2+1, RGB(190, 180, 140), 0, Temp2);
               fNewGuiBtnFont->DrawFont(hdc, dwXBtn+dwVal  ,  dwYBtn+2  , RGB(0,0,0)        , 0, g_LocalString[dwTPos]);
               



               DXDReleaseDC(hdc, 22);
            } 
         break;
            
         case TFC_WARNING_2:
         case TFC_WARNING:
            {
               int dwXBox =  (g_Global.GetScreenW()-E_Back.GetWidth())/2;
               int dwYBox =  (g_Global.GetScreenH()-E_Back.GetHeight())/2+30;
               E_Back.DrawSpriteNSemiTrans(dwXBox,dwYBox,&vsffx2,TRUE,180);
               
               int dwXBtn = dwXBox+14;
               int dwYBtn = dwYBox+38;

               HDC hdc;
               DXDGetDC(&hdc, 6);
            
               fNewGuiBtnFont->DrawFont(hdc, dwXBox+115, dwYBox+55 , RGB(160, 20, 20), 0, strErrRaisonL1);
               fNewGuiBtnFont->DrawFont(hdc, dwXBox+115, dwYBox+115, RGB(160, 20, 20), 0, strErrRaisonL2);
            
               DXDReleaseDC(hdc, 6);
            }
         break;
      }
      
      x = y = 0; 
      ZeroMemory(button, 4*sizeof(int));
      if (TFC_State >= TFC_INTRODUCTION)
      {
          if (MOUSEINUSE) 
          {
              Mouse2.GetPosition(&x, &y);
              CMouseCursor::GetInstance()->DrawCursor(x, y); // 1
          }
      } 
      else 
      {
          Mouse2.GetStatus(&x, &y, button);
      }
      
     
      Chose = 0;
      if (TFC_State > TFC_INTRODUCTION && N >= 100) 
      {
          if (MOUSEINUSE) 
          {
              Mouse2.GetPosition(&x, &y);
              CMouseCursor::GetInstance()->DrawCursor(x, y); // 1
              x = (x+48-16)/32;
              y = (y-8)/16;
          }
      } 
      else 
      {
          Mouse2.GetStatus(&x, &y, button);
          x = (x+48-16)/32;
          y = (y-8)/16;
          if (button[0] == DM_CLICK) 
          {
              Chose = 1;
          }
      }
      
      Sleep(10);
      
      if (!COMM.isAlive()) 
      {
          TFC_State = TFC_WARNING_2;
          strcpy(strErrRaisonL1, g_LocalString[ 497 ] );//Connection Error:
          strcpy(strErrRaisonL2, g_LocalString[ 498 ] );//Connection Lost.
      }
      
      DXDFlip();
      
      bool quit = false;
      
	  /////////////////////////////////////////////////////
	  // GAME LOGIC LOOP : (why is that in TFCSocket ?!!)
	  ////////////////////////////////////////////////////
	  
	  
	  while (!quit) 
      {
          Sleep(0);
          COMM.Lock();
          int Len = COMM.Receive(Msg);
          COMM.Unlock();
          if(Len > 0) 
          {
              Msg->Get((short *)&Type);
              LOG << "INTERPRET PACKET [" << Type << "] [" << timeGetTime() << "]\r\n";
              dwLastUse = timeGetTime();
              
              switch(Type) 
              {
                  // Type of Packet
			         case RQ_MaxCharactersPerAccountInfo:
                  {
				         Msg->Get((char *)&MaxCharactersPerAccount);
			         } break;
                  case 20:
                     g_App.SetError(1, g_LocalString[500]);
                     g_App.Close(g_LocalString[500]);
                  break;
                  case 18: 
                  {
                      // View Backpack
                      LOG << "* PAK = 18\r\n";
                      unsigned short nbObjects = 0;
                      
                      Msg->Get((short *)&nbObjects);
                      
                      if (nbObjects > 24) nbObjects = 24;
                      
                      Backpack[0].nbObjects = nbObjects;
                      LOG << "Nb of Objects(" << nbObjects << ")";
                      for (int i = 0; i < nbObjects; i++) {
                          Msg->Get((short *)&Backpack[i].Type);
                          Msg->Get((long *)&Backpack[i].ID);
                          Backpack[i].Def = 0;
                          //Backpack[i].Time = -1;
                          LOG << " [Objects(" << i+1 << ") Type(" << Backpack[i].Type << ") ID (" << Backpack[i].ID << ")]";
                      }
                  } break;
                  case TFCStillConnected: 
                  {
                      LOG << "* PAK = 10\r\n";
                      TFCPacket Send; 
                      
                      Send << (short)10;//RQ_Ack
                      
                      SEND_PACKET(Send);
                  } break; 
                  
                  case RQ_DeletePlayer: 
                  {

                  } break;
                  
                  case TFCGetPlayerList: 
                  {
                      LOG << "'RECEIVE PACKET AT [" << timeGetTime() << "]\r\n";
                      LOG << "* PAK = 26\r\n";
                      char Len;
                      
                      if (CreateFlag) 
                      {
                          MenuThreadFinished = true;
                          WantPreGame = true;
                          
                          strcpy(MenuName, Player.Name);
                         
                          for(int i=0;i<NM_CONNECT_SCREEN_NBR_BACK_ANIM;i++)
                             Intro[i].Release();

                          for(int i=0;i<36;i++)
                             Connect_D_Anim[i].Release();


                          //Connect_Title.Release();
                          Connect_Title2.Release();
                          Connect_D.Release();
                          Connect_V.Release();
                          Connect_Main_Back.Release();
                          Connect_Main_BtnN.Release();
                          Connect_Main_BtnD.Release();
                          Connect_Main_BtnH.Release();
                          PS_Back.Release();
                          PS_BackDown.Release();
                          PS_Btn_N.Release();
                          PS_Btn_H.Release();
                          PS_SmallBtn_N.Release();
                          PS_SmallBtn_H.Release();
                          PS_Over.Release();
                          PS_SBtnUp_N.Release();
                          PS_SBtnUp_H.Release();
                          PS_SBtnDn_N.Release();
                          PS_SBtnDn_H.Release();
                          Q_Back.Release();
                          Q_BackSelect.Release();
                          J_Back.Release();
                          E_Back.Release();


                          P.Release();
                          M.Release();
                          
                          break;
                      }
                      
                      Msg->Get((char*)&NbOfPlayer);
                      
                      if (!NbOfPlayer && TFC_State == TFC_CHOOSE_PLAYER) 
                      {
                          TFC_State = TFC_CHOOSE_NAME;
                          Objects.GetMainObject()->Type = __PLAYER_HUMAN_PUPPET;
                          Player.Name[0] = 0;
                          QuestionNumber = 0;
                          QuestionAnswer[0] = 0;
                          QuestionAnswer[1] = 0;
                          QuestionAnswer[2] = 0;
                          QuestionAnswer[3] = 0;
                          QuestionAnswer[4] = 0;
                          Shuffle();
                      }
                      else if (!NbOfPlayer) 
                      {
                          TFC_State = TFC_WARNING;
                          strcpy(strErrRaisonL1, g_LocalString[103]);
                          strcpy(strErrRaisonL2, "");
                      }
                      
                      if (NbOfPlayer < MaxCharactersPerAccount)
                          CreateBoost = 1;
                      else
                          CreateBoost = 0;
                      
                      for(int kk=0;kk<50;kk++)
                      {
                         ZeroMemory(Name[kk], 100);
                         Race[kk]   = 0;
                         Level[kk]  = 0;
                         IsDead[kk] = 0;
                      }
                      
                      for (int i = 0; i < NbOfPlayer; i++) 
                      {
                          Msg->Get((char *)&Len);
                          
                          for (int j = 0; j < Len; j++) 
                              Msg->Get((char *)&Name[i][j]);
                          
                          Msg->Get((short *)&Race[i]);
                          Msg->Get((short *)&Level[i]);
                          //Msg->Get((short *)&IsDead[i]);
                          /*if(IsDead[i])
                              OutputDebugString("Dead***\n");*/
                      }
                      
                      BYTE TempName[100];
                      DWORD TempRace;
                      DWORD TempLevel;
                      DWORD TempDead;
                      
                      /*for (i = 0; i < NbOfPlayer; i++) 
                      {
                          for (int j = i+1; j < NbOfPlayer; j++) 
                          {
                              if (strcmp(Name[i], Name[j]) > 0) 
                              {
                                  strcpy((char *)TempName, Name[i]);
                                  strcpy(Name[i], Name[j]);
                                  strcpy(Name[j], (char *)TempName);
                                  TempRace = Race[i];
                                  Race[i] = Race[j];
                                  Race[j] = (short)TempRace;
                                  TempLevel = Level[i];
                                  Level[i] = Level[j];
                                  Level[j] = (short)TempLevel;
                                  TempDead  = IsDead[i];
                                  IsDead[i] = IsDead[j];
                                  IsDead[j] = (short)TempDead;
                              }
                          }
                      } */
                      
                      //							if (Nb == 2)
                      //								Name[2][0] = 0;
                      
                } break;
            
                case 90: 
                {
                    Msg->Get((char *)&nameIsNotOk);
                    
                    if (!nameIsNotOk) {
                        TFC_State++;
                        Selected = 1;

                        if (!Player.Sexx) 
                        {
                            Objects.GetMainObject()->Type = __PLAYER_HUMAN_PUPPET;
                        } 
                        else 
                        {
                            Objects.GetMainObject()->Type = __PLAYER_HUMAN_FEMALE;
                        }
                    }
                } break;
                
                case TFCReroll: 
                {
                    LOG << "* PAK = 31\r\n";
                    Msg->Get((char *)&Player.Agi);
                    Msg->Get((char *)&Player.End);
                    Msg->Get((char *)&Player.Int);
                    Msg->Get((char *)&Player.Lck);
                    Msg->Get((char *)&Player.Str);
                    Msg->Get((char *)&Player.Wil);
                    Msg->Get((char *)&Player.Wis);
                    Msg->Get((long *)&Player.MaxHp);
                    Msg->Get((long *)&Player.Hp);
                } break;
                
                case TFCCreateNewPlayer: 
                {
                    LOG << "* PAK = 25\r\n";
                    unsigned char ERR;
                    Msg->Get((char *)&ERR);
                    
                    switch (ERR) {
                    case TFCCreateNewPlayer_CreatedPlayer: 
                        {
                            if (TFC_State == TFC_CHOOSE_RACE)
                                TFC_State++;
                            
                            Msg->Get((char *)&Player.Agi);
                            Msg->Get((char *)&Player.End);
                            Msg->Get((char *)&Player.Int);
                            Msg->Get((char *)&Player.Lck);
                            Msg->Get((char *)&Player.Str);
                            Msg->Get((char *)&Player.Wil);
                            Msg->Get((char *)&Player.Wis);
                            Msg->Get((long *)&Player.MaxHp);
                            Msg->Get((long *)&Player.Hp);
                            Msg->Get((short *)&Player.MaxMana);
                            Msg->Get((short *)&Player.Mana);
                        } break;
                        
                    case TFCCreateNewPlayer_AccountInGame: 
                        TFC_State = TFC_WARNING;
                        strcpy(strErrRaisonL1, g_LocalString[ 429 ] );
                        strcpy(strErrRaisonL2, "");
                        break;
                        
                    case TFCCreateNewPlayer_NoCredits:
                        TFC_State = TFC_WARNING;
                        strcpy(strErrRaisonL1, g_LocalString[ 488 ] );
                        strcpy(strErrRaisonL2, "");
                        break;
                        
                    case TFCCreateNewPlayer_TooManyAccounts:
                        TFC_State = TFC_WARNING;
                        strcpy(strErrRaisonL1, g_LocalString[ 489 ] );
                        strcpy(strErrRaisonL2, g_LocalString[ 490 ] );
                        break;
                        
                    case TFCCreateNewPlayer_PlayerAlreadyExists:
                        TFC_State = TFC_WARNING;
                        strcpy(strErrRaisonL1, g_LocalString[ 491 ] );
                        strcpy(strErrRaisonL2, g_LocalString[ 492 ] );
                        break;
                        
                    case TFCCreateNewPlayer_PlayerDontExists:
                        TFC_State = TFC_WARNING;
                        strcpy(strErrRaisonL1, g_LocalString[ 493 ] );
                        strcpy(strErrRaisonL2, "");
                        break;
                        
                    case TFCCreateNewPlayer_PlayerAlreadyGame:
                        TFC_State = TFC_WARNING;
                        strcpy(strErrRaisonL1, g_LocalString[ 494 ] );
                        strcpy(strErrRaisonL2, "");
                        break;
                        
                    case TFCCreateNewPlayer_InvalidNameSpec:
                        TFC_State = TFC_WARNING;
                        strcpy(strErrRaisonL1,  g_LocalString[ 495 ] );
                        strcpy(strErrRaisonL2, g_LocalString[ 496 ] );
                        break;
                    }
                } break;
               } // End Switch
            LPBYTE lpbBuffer;
            int nBufferSize;
            Msg->GetBuffer(lpbBuffer, nBufferSize);
            delete Msg;
         } 
         else 
         {
          //LOG << "NO PACKETS\r\n";
          quit = true;
         }
      }
      
      int xp, yp;
      Mouse2.GetStatus(&xp, &yp, button);
      x = xp;
      y = yp;
      if (button[0] == DM_CLICK) 
      {
          Chose = 1;
          DBL = FALSE;
      }
      if (button[0] == DM_DOUBLE_CLICK) 
      {
          Chose = 1;
          DBL = TRUE;
      }
      
      ///////// BLBLBL : useles ?

	  switch (TFC_State) 
      {
         case TFC_VIEW_CREDITS: 
         {
            if (key || Chose == 1) 
            {
               TFC_State = TFC_MENU;
               LastState = 0;
               key = 0;
            }
         } 
         break;
         case TFC_INTRODUCTION: 
         {
            if (key || Chose == 1) {
               TFC_State = TFC_MENU;
               LastState = 0;
               key = 0;
            }
         } 
         break;
          
         case TFC_LOGO: 
            {
               if (key || Chose == 1) 
               {
                  Chose = 2;
                  TFC_State = TFC_SPLASH;
                  key = 0;
                  Frame = 0;
               }
            } 
         break;
         case TFC_SPLASH: 
            {
               if (key || Chose == 1) 
               {
                  Chose = 2;
                  key = 0;
                  TFC_State = TFC_MENU;
               }
            } 
         break;
         case TFC_MENU: //OKOKOK
            {
               int dwXBox =  (g_Global.GetScreenW()-Connect_Main_Back.GetWidth())/2;
               int dwYBox =  (g_Global.GetScreenH()-Connect_Main_Back.GetHeight())/2+30;
            
               int dwXBtn = dwXBox+14;
               int dwYBtn = dwYBox+38;
               int dwBtnMouseClick = -1;
               if(Chose == 1)
               {
                  for(int ib=0;ib<4;ib++)
                  {
                     if(x >= dwXBtn && x <= dwXBtn+Connect_Main_BtnN.GetWidth() && y >= dwYBtn && y <= dwYBtn+Connect_Main_BtnN.GetHeight()   )
					 {
                        dwBtnMouseClick = ib;
						GUI_BtnDOwn->Play(2);
					 }
                     dwYBtn+=32;
                  }
               }
               if (key == tolower(g_LocalString.GetHotKey(2)) || key == 13 || dwBtnMouseClick == 0) //Enter the realm
               {
                  key = 0;
                  RegisterCount = 0;
                  Chose = 2;
                  TFC_State = TFC_CHOOSE_PLAYER;
                  TFCPacket Send;
                  Send << (RQ_SIZE)26;
                  LOG << "SEND PACKET AT [" << timeGetTime() << "]\r\n";

                  for(int kk=0;kk<50;kk++)
                  {
                     ZeroMemory(Name[kk], 100);
                     Race[kk]   = 0;
                     Level[kk]  = 0;
                     IsDead[kk] = 0;
                  }

                  SEND_PACKET(Send);
                  key = 0;
               }
               else if (key == tolower(g_LocalString.GetHotKey(3)) || dwBtnMouseClick==1) 
               {
                  Chose = 2;
                  TFC_State = TFC_INTRODUCTION;
                  ScrollX = g_Global.GetScreenH()*2;
                  key = 0;
               }
               else if (key == tolower(g_LocalString.GetHotKey(5)) || dwBtnMouseClick==2) 
               {
                  Chose = 2;
                  TFC_State = TFC_VIEW_CREDITS;
                  ScrollX = g_Global.GetScreenH()*2;
                  key = 0;
               }
               if (key == 27 || key == tolower(g_LocalString.GetHotKey(6)) || dwBtnMouseClick==3) 
               {
                  Chose = 2;
                  key = 0;
                  delete Packet;
                  LOG << "Ask Close 9\r\n";
                  LOG << "* CLS = 5\r\n";
                  PostMessage(siGethWnd(), WM_CLOSE, NULL, 0x1111);
                  return;
               }
            } 
         break;
         case TFC_CHOOSE_PLAYER:  //OKOKOK
            {
               int dwXBox =  (g_Global.GetScreenW()-PS_Back.GetWidth())/2;
               int dwYBox =  (g_Global.GetScreenH()-PS_Back.GetHeight())/2+30;
               int dwXBtn = dwXBox+278;
               int dwYBtn = dwYBox+170;
               
               //Down
               if (key == 200 || IsOnButton(Chose,x,y,&PS_SBtnDn_N,dwXBtn,dwYBtn)) 
               {
				  if(key != 200)
					  GUI_BtnDOwn->Play(2);
                  if(Selected <NbOfPlayer-1)
                  {
                     Selected++;
                     if(Selected >= NM_FPVisible+NM_MPVisible)
                     {
                        NM_FPVisible++;
                     }
                  }
                  key = 0;
               }
               //Up
               dwYBtn = dwYBox+76;
               if (key == 201 || IsOnButton(Chose,x,y,&PS_SBtnUp_N,dwXBtn,dwYBtn)) 
               {
				  if(key != 201)
					  GUI_BtnDOwn->Play(2);
                  if(Selected >0)
                  {
                     Selected--;
                     if(Selected < NM_FPVisible)
                     {
                        NM_FPVisible--;
                     }
                  }
                  key = 0;
               }
               
               int Drop = 0;
               if (CreateBoost)
                  Drop = 380; // Now the 'New Player' option stays always at the same location (380)

               dwXBtn = dwXBox+318;
               dwYBtn = dwYBox+67;
               int dwBtnMouseClick = -1;
			   int ib;
               for(ib=0;ib<3;ib++)
               {
                  if(ib == 1 && NbOfPlayer >= MaxCharactersPerAccount) 
                  {
                     
                  }
                  else
                  {
                     if(IsOnButton(Chose,x,y,&PS_Btn_N,dwXBtn,dwYBtn))
					 {
                        dwBtnMouseClick = ib;
						GUI_BtnDOwn->Play(2);
					 }
                  }
                  dwYBtn+=32;
               }
               
               dwYBtn = dwYBox+177;
               if(IsOnButton(Chose,x,y,&PS_Btn_N,dwXBtn,dwYBtn))
			   {
                  dwBtnMouseClick = ib;
				  GUI_BtnDOwn->Play(2);
			   }

               // return to Menu
               if (key == 27 || key == tolower(g_LocalString.GetHotKey(21)) || dwBtnMouseClick == 3)
               {	
                  TFC_State = TFC_MENU;
                  LastState = 0;
                  key = 0;
               } 
               // Create New player
               else if ((NbOfPlayer < MaxCharactersPerAccount) && ((key == tolower(g_LocalString.GetHotKey(4)) || dwBtnMouseClick ==1) && Drop))
               {
                  Chose = 2;
                  TFC_State = TFC_ASK_CREATE_CHARACTER;
                  key = 0;
                  QuestionNumber = 0;
                  QuestionAnswer[0] = 0;
                  QuestionAnswer[1] = 0;
                  QuestionAnswer[2] = 0;
                  QuestionAnswer[3] = 0;
                  QuestionAnswer[4] = 0;
                  Shuffle();
               } 
               //Delete player
               else if (key == tolower(g_LocalString.GetHotKey(7)) || dwBtnMouseClick ==2) 
               {
                  Chose = 2;
                  TFC_State = TFC_CONFIRM_DELETE;
                  key = 0;
               } 
               else 
               {
                  int dwXSel = dwXBox+16;
                  int dwYSel = dwYBox+76;
                  int dwSelectedZone = -1;
                  //dessine la liste des players....
                  for (int i = NM_FPVisible,j=0; i < NM_MPVisible+NM_MPVisible; i++,j++) 
                  {
                     if(IsOnButton(Chose,x,y,&PS_Over,dwXSel,dwYSel))
                        dwSelectedZone = j;
                     dwYSel+=31;
                  }
                  for (int i_KeybHotkeys = 0; i_KeybHotkeys < 9; i_KeybHotkeys++) 
                  {
                     if ((key == '1'+i_KeybHotkeys) || (dwSelectedZone == i_KeybHotkeys && i_KeybHotkeys <NbOfPlayer)) 
                     {
                        
                        //if (Selected == i_KeybHotkeys || DBL == TRUE) 
                        //{
                        //   key = 13;
                        //}
                        
                        Selected = i_KeybHotkeys;
                     }
                  }
                  
                  if (key == tolower(g_LocalString.GetHotKey(105)) || key == 13 || dwBtnMouseClick ==0) 
                  {
                     if (Race[0]) 
                     {
                        
                        if(IsDead[Selected])
                        {
                           TFC_State = TFC_WARNING;
                           sprintf(strErrRaisonL1, "%s est mort(e)...",Name[Selected]);
                           strcpy(strErrRaisonL2, "Vous devez choisir un autre personnage.");
                           key = 0;
                        }
                        else
                        {
                           Objects.GetMainObject()->Type = Race[Selected];
                           TFC_State = TFC_PLAY;
                           Chose = 2;
                           MenuThreadFinished = true;
                        
                           WantPreGame = true;
                           strcpy(MenuName, Name[Selected]);
                        
                        
                           for(int i=0;i<NM_CONNECT_SCREEN_NBR_BACK_ANIM;i++)
                              Intro[i].Release();

                           for(int i=0;i<36;i++)
                                Connect_D_Anim[i].Release();
                        
                           //Connect_Title.Release();
                           Connect_Title2.Release();
                           Connect_D.Release();
                           Connect_V.Release();
                           Connect_Main_Back.Release();
                           Connect_Main_BtnN.Release();
                           Connect_Main_BtnD.Release();
                           Connect_Main_BtnH.Release();
                           PS_Back.Release();
                           PS_BackDown.Release();
                           PS_Btn_N.Release();
                           PS_Btn_H.Release();
                           PS_SmallBtn_N.Release();
                           PS_SmallBtn_H.Release();
                           PS_Over.Release();
                           PS_SBtnUp_N.Release();
                           PS_SBtnUp_H.Release();
                           PS_SBtnDn_N.Release();
                           PS_SBtnDn_H.Release();
                           Q_Back.Release();
                           Q_BackSelect.Release();
                           J_Back.Release();
                           E_Back.Release();
                           P.Release();
                           M.Release();
                        
                           key = 0;
                        }
                     }
                  }
               }
            } 
         break; 
         case TFC_ASK_CREATE_CHARACTER: 
            {
                 TFC_State = TFC_CHOOSE_NAME;
                 Objects.GetMainObject()->Type = __PLAYER_HUMAN_PUPPET;
                 Player.Name[0] = 0;
            } 
         break;
         case TFC_ASK_CHOOSE_PLAYER: 
            {
               TFC_State = TFC_CHOOSE_PLAYER;
               TFCPacket Send;
               Send << (RQ_SIZE)26;
               SEND_PACKET(Send);
               LOG << "SEND PACKET AT [" << timeGetTime() << "]\r\n";
               for(int kk=0;kk<50;kk++)
               {
                  ZeroMemory(Name[kk], 100);
                  Race[kk]   = 0;
                  Level[kk]  = 0;
                  IsDead[kk] = 0;
               }
            } 
         break;
         case TFC_CONFIRM_DELETE: 
            {
               int dwXBox =  (g_Global.GetScreenW()-PS_Back.GetWidth())/2;
               int dwYBox =  (g_Global.GetScreenH()-PS_Back.GetHeight())/2+30;
               int dwXSel = dwXBox+289;
               int dwYSel = dwYBox+243;
               int dwSelectedZone = -1;
               //dessine la liste des players....
               for (int i = 0; i < 2; i++) 
               {
                  if(IsOnButton(Chose,x,y,&PS_SmallBtn_N,dwXSel,dwYSel))
				  {
                     dwSelectedZone = i;
					 GUI_BtnDOwn->Play(2);
				  }
                  dwXSel+=78;
               }
               
               if (key == 27 || key == tolower(g_LocalString.GetHotKey(26))  || dwSelectedZone == 1) 
               {
                  Chose = 2;
                  TFC_State = TFC_CHOOSE_PLAYER;
                  LastState = 0;
                  key = 0;
               } 
               else 
               {
                  if (key == tolower(g_LocalString.GetHotKey(25)) || key == 13 || dwSelectedZone == 0) 
                  {
                     Chose = 2;
                     key = 0;
                     TFC_State = TFC_CHOOSE_PLAYER;
                     
                     TFCPacket Send2;
                     
                     Send2 << (RQ_SIZE)15;
                     Send2 << (char)strlen(Name[Selected]);
                     Send2 << (char *)Name[Selected];
                     
                     LOG << "Delete User " << Name[Selected] << "\r\n";
                     
                     SEND_PACKET(Send2);
                     Sleep(100);
                     
                     
                     TFCPacket Send;
                     
                     Send << (RQ_SIZE)26;

                     for(int kk=0;kk<50;kk++)
                     {
                        ZeroMemory(Name[kk], 100);
                        Race[kk]   = 0;
                        Level[kk]  = 0;
                        IsDead[kk] = 0;
                     }
                     
                     SEND_PACKET(Send);
                     LOG << "SEND PACKET AT [" << timeGetTime() << "]\r\n";
                     
                     Selected = 0;
                     NM_FPVisible = 0;
                  }
               }
            } 
         break;
         case TFC_CHOOSE_NAME: 
            {
               if (key == 13) 
               {
                  nameIsNotOk = 0;
                  if (strlen(Player.Name) != 0) 
                  {
                     TFCPacket Send;
                     
                     Send << (short)90;//RQ_ChooseName
                     Send << (short)strlen(Player.Name);
                     Send << (char *)Player.Name;
                     
                     SEND_PACKET(Send);
                  }
                  key = 0;
               } 
               else if (key == 8) 
               {
                  nameIsNotOk = 0;
                  if (strlen(Player.Name) > 0)
                     Player.Name[strlen(Player.Name)-1] = 0;
                  key = 0;
               } 
               else if (key == 27) 
               {
                  nameIsNotOk = 0;
                  TFC_State = TFC_CHOOSE_PLAYER;
                  LastState = 0;
                  key = 0;
               } 
               else 
               {
                  if (key) 
                  {
                     nameIsNotOk = 0;
                     if (strlen(Player.Name) == 0 && key ==' ') 
                     {
                        key = 0;
                     }
                     if (Player.Name[strlen(Player.Name)-1] == ' ' && key ==' ') 
                     {
                        key = 0;
                     }
                     if (strlen(Player.Name) <= 19)//BLBLBL 
                     {
                        char Temp[2];
                        Temp[0] = key;
                        Temp[1] = 0;
                        strcat(Player.Name, Temp);
                     }
                     key = 0;
                  }
               }
            } 
         break;
         case TFC_CHOOSE_SEXX: 
            {
               int dwXBox =  (g_Global.GetScreenW()-PS_Back.GetWidth())/2;
               int dwYBox =  (g_Global.GetScreenH()-PS_Back.GetHeight())/2+30;
               int dwXSel = dwXBox+289;
               int dwYSel = dwYBox+243;
               int dwSelectedZone = -1;
               //dessine la liste des players....
               for (int i = 0; i < 2; i++) 
               {
                  if(IsOnButton(Chose,x,y,&PS_SmallBtn_N,dwXSel,dwYSel))
				  {
                     dwSelectedZone = i;
					 GUI_BtnDOwn->Play(2);
				  }
                  dwXSel+=78;
               }

               if (Player.Name[strlen(Player.Name)-1] == ' ') 
               {
                  Player.Name[strlen(Player.Name)-1] = 0;
               }
               
               if (key == 200) 
               {
                  Selected = (Selected % 2);
                  Selected++;
                  key = 0;
               }
               if (key == 201) 
               {
                  Selected--;
                  if (!Selected) Selected = 2;
                  key = 0;
               }
               
               if (key == tolower(g_LocalString[178][0]) || dwSelectedZone == 0) 
               {
                  Selected = 1;
                  key = 13;
               }
               
               if (key == tolower(g_LocalString[179][0]) || dwSelectedZone == 1) 
               {
                  Selected = 2;
                  key = 13;
               }
               
               if (key == 13 || key == tolower(g_LocalString.GetHotKey(20))) 
               {	
                  TFC_State = TFC_CHOOSE_RACE;
                  key = 0;
                  Player.Sexx = Selected-1;
               }
               
               if (key == 27 || key == tolower(g_LocalString.GetHotKey(21))) 
               {
                  Chose = 2;
                  TFC_State = TFC_MENU;
                  LastState = 0;
                  key = 0;
               }
            } 
         break;
         case TFC_CHOOSE_RACE: 
            {
               int dwXBox =  (g_Global.GetScreenW()-Q_Back.GetWidth())/2;
               int dwYBox =  dwTitemEndPos+20;
               if(dwYBox+Q_Back.GetHeight() > g_Global.GetScreenH())
               dwYBox =g_Global.GetScreenH()-Q_Back.GetHeight();
               int dwSelectedZone = -1;

               int dwXSel = dwXBox+17;
               int dwYSel = dwYBox+106;
               dwSelectedZone = -1;
               //dessine la liste des players....
               for (int i = 0; i < 5; i++) 
               {
                  if(IsOnButton(Chose,x,y,&Q_BackSelect,dwXSel,dwYSel))
				  {
                     dwSelectedZone = i;
				  }
                  dwYSel+=48;
               }
               
               if (key == 200) 
               {
                  key = 0;
                  QuestionChoice++;
                  QuestionChoice = ((QuestionChoice-1) % 5)+1;
               }
               
               if (key == 201) 
               {
                  key = 0;
                  QuestionChoice--;
                  if (QuestionChoice < 1) QuestionChoice += 5;
               }
               
               if (key == '1' || dwSelectedZone == 0) 
               {
                  Chose = 2;
                  key = 0;
                  QuestionChoice = 1;
               }
               
               if (key == '2' || dwSelectedZone == 1) 
               {
                  Chose = 2;
                  key = 0;
                  QuestionChoice = 2;
               }
               
               if (key == '3' || dwSelectedZone == 2) 
               {
                  Chose = 2;
                  key = 0;
                  QuestionChoice = 3;
               }
               
               if (key == '4' || dwSelectedZone == 3) 
               {
                  Chose = 2;
                  key = 0;
                  QuestionChoice = 4;
               }
               
               if (key == '5' || dwSelectedZone == 4) 
               {
                  Chose = 2;
                  key = 0;
                  QuestionChoice = 5;
               }

               int dwXBtn = dwXBox+537;
               int dwYBtn = dwYBox+265;
               dwSelectedZone = -1;

               //dessine la liste des players....
               for (int i = 0; i < 2; i++) 
               {
                  if(IsOnButton(Chose,x,y,&PS_SmallBtn_N,dwXBtn,dwYBtn))
				  {
                     dwSelectedZone = i;
					 GUI_BtnDOwn->Play(2);
				  }

                  dwYBtn+=48;
               }

               
               if (key == 13 || key == tolower(g_LocalString.GetHotKey(20)) || dwSelectedZone == 0) 
               {	
                  Chose = 2;
                  key = 0;
                  
                  QuestionAnswer[RN[QuestionNumber][QuestionChoice-1]]++; 
                  
                  QuestionNumber++;
                  
                  if (QuestionNumber == 4) 
                  {
                     //Sound[1].Play(false);
                     TFCPacket Send;
                     
                     Send << (short)25;//RQ_CreatePlayer
                     Send << (char)QuestionAnswer[3];	
                     Send << (char)QuestionAnswer[2];	
                     Send << (char)QuestionAnswer[0]; 
                     Send << (char)QuestionAnswer[1];	
                     Send << (char)QuestionAnswer[4];
                     Send << (char)Player.Sexx;
                     Send << (char)strlen(Player.Name);
                     Send << (char *)Player.Name;
                     
                     if (!Player.Sexx) 
                     {
                        Objects.GetMainObject()->Type = __PLAYER_HUMAN_PUPPET;
                     } else 
                     {
                        Objects.GetMainObject()->Type = __PLAYER_HUMAN_FEMALE;
                     }
                     
                     SEND_PACKET(Send);
                  } 
               }
               
               if (key == 27 || key == tolower(g_LocalString.GetHotKey(21)) || dwSelectedZone == 1) 
               {
                  Chose = 2;
                  TFC_State = TFC_MENU;
                  LastState = 0;
                  key = 0;
               }
            } 
         break;
         
         case TFC_MAKE_REROLL: 
            {
               int dwXBox =  (g_Global.GetScreenW()-J_Back.GetWidth())/2;
               int dwYBox =  (g_Global.GetScreenH()-J_Back.GetHeight())/2+30;
               int dwXBtn = dwXBox+306;
               int dwYBtn = dwYBox+74;
               int dwSelectedZone = -1;
			   int ib;
               for(ib=0;ib<2;ib++)
               {
                  if(IsOnButton(Chose,x,y,&PS_Btn_N,dwXBtn,dwYBtn))
				  {
                     dwSelectedZone = ib;
					 GUI_BtnDOwn->Play(2);
				  }
                  dwYBtn+=32;
               }

               dwYBtn = dwYBox+178;
               if(IsOnButton(Chose,x,y,&PS_Btn_N,dwXBtn,dwYBtn))
			   {
                  dwSelectedZone = ib;
				  GUI_BtnDOwn->Play(2);
			   }
               

               if (key == 200 || key == tolower(g_LocalString.GetHotKey(23)) || key == 201  || dwSelectedZone == 1) 
               {
                  Chose = 2;
                  TFCPacket Send;
                  
                  Send << (short)31;//RQ_Reroll
                  
                  SEND_PACKET(Send);
                  key = 0;
               }
               if (key == 27 || key == tolower(g_LocalString.GetHotKey(21)) || dwSelectedZone == 2) 
               {
                  Chose = 2;
                  TFCPacket Send2;
                  
                  Send2 << (RQ_SIZE)15;
                  Send2 << (char)strlen(Player.Name);
                  Send2 << (char *)Player.Name;
                  
                  LOG << "Delete User " << Player.Name << "\r\n";
                  
                  SEND_PACKET(Send2);
                  
                  TFC_State = TFC_MENU;
                  LastState = 0;
                  key = 0;
               }
               if (key == 13 || key == tolower(g_LocalString.GetHotKey(22)) || dwSelectedZone == 0) 
               {
                  TFC_State = TFC_PLAY;
                  Chose = 2;
                  
                  TFCPacket Send;
                  
                  CreateFlag = TRUE;
                  
                  Send << (RQ_SIZE)26;
                  SEND_PACKET(Send);
                  LOG << "SEND PACKET AT [" << timeGetTime() << "]\r\n";
                  
                  key = 0;
               } 
            } 
         break;
         case TFC_WARNING:
            {
               if (key || Chose == 1) 
               {
                  Chose = 2;
                  TFC_State = TFC_MENU;
                  LastState = 0;
                  key = 0;
               }
            } break;
            
         case TFC_WARNING_2:
            {
               if (key || Chose == 1) 
               {
                  Chose = 2;
                  
                  g_Pal.FadeToBlack();
                  
                  LOG << "Ask Close 10\r\n";
                  LOG << "* CLS = 6\r\n";
                  PostMessage(siGethWnd(), WM_CLOSE, NULL, 0x6969);
                  return;
                  
                  key = 0;
               }
            } break;
      } 
      g_App.UnlockMainThread();
   } //	while (!MainThreadFinished) 
   LOG << "* END = 15\r\n";
   
   //delete [] Dest;
   delete Packet;
   FreeSound();
   
   MENU_THREAD = FALSE;
   
}


void ManageAutoMove(char* strKeyInput)
{
   if(strKeyInput[DIK_LCONTROL] && strKeyInput[DIK_LSHIFT] && !NMAutoMove)
   {
      NMAutoDir = 0;
      if(strKeyInput[DIK_DOWN] && strKeyInput[DIK_LEFT])
         NMAutoDir = 5;
      else if(strKeyInput[DIK_DOWN] && strKeyInput[DIK_RIGHT])
         NMAutoDir = 6;
      else if(strKeyInput[DIK_UP] && strKeyInput[DIK_LEFT])
         NMAutoDir = 7;
      else if(strKeyInput[DIK_UP] && strKeyInput[DIK_RIGHT])
         NMAutoDir = 8;
      else if(strKeyInput[DIK_UP])
         NMAutoDir = 1;
      else if(strKeyInput[DIK_RIGHT])
         NMAutoDir = 2;
      else if(strKeyInput[DIK_DOWN])
         NMAutoDir = 3;
      else if(strKeyInput[DIK_LEFT])
         NMAutoDir = 4;

      if(NMAutoDir > 0)
      {
         NMAutoMove = true;
         ChatterUI::GetInstance()->AddBackscrollItem("","Starting AutoMove...",RGB(255,255,255),true);     
         
      }
   }
   else if(NMAutoMove)
   {
      //verifie si on dois desactiver le automove...
      if(/*strKeyInput[DIK_LCONTROL] ||*/ strKeyInput[DIK_NUMPAD5] ||
         strKeyInput[DIK_F1 ] || strKeyInput[DIK_F2 ] || strKeyInput[DIK_F3 ] || strKeyInput[DIK_F4 ] ||
         strKeyInput[DIK_F5 ] || strKeyInput[DIK_F6 ] || strKeyInput[DIK_F7 ] || strKeyInput[DIK_F8 ] ||
         strKeyInput[DIK_F9 ] || strKeyInput[DIK_F10] || strKeyInput[DIK_F11] || strKeyInput[DIK_F12]    )
      {
         NMAutoMove = false;
         NMAutoDir  = 0;
         ChatterUI::GetInstance()->AddBackscrollItem("","Stopping AutoMove...",RGB(255,255,255),true);     
      }
      else
      {
         if(strKeyInput[DIK_DOWN] && strKeyInput[DIK_LEFT])
            NMAutoDir = 5;
         else if(strKeyInput[DIK_DOWN] && strKeyInput[DIK_RIGHT])
            NMAutoDir = 6;
         else if(strKeyInput[DIK_UP] && strKeyInput[DIK_LEFT])
            NMAutoDir = 7;
         else if(strKeyInput[DIK_UP] && strKeyInput[DIK_RIGHT])
            NMAutoDir = 8;
         else if(strKeyInput[DIK_UP])
            NMAutoDir = 1;
         else if(strKeyInput[DIK_RIGHT])
            NMAutoDir = 2;
         else if(strKeyInput[DIK_DOWN])
            NMAutoDir = 3;
         else if(strKeyInput[DIK_LEFT])
            NMAutoDir = 4;

         if(NMAutoDir == 5)
         {
            strKeyInput[DIK_DOWN] = 1;
            strKeyInput[DIK_LEFT] = 1;
         }
         else if(NMAutoDir == 6)
         {
            strKeyInput[DIK_DOWN] = 1;
            strKeyInput[DIK_RIGHT] = 1;
         }
         else if(NMAutoDir == 7)
         {
            strKeyInput[DIK_UP] = 1;
            strKeyInput[DIK_LEFT] = 1;
         }
         else if(NMAutoDir == 8)
         {
            strKeyInput[DIK_UP] = 1;
            strKeyInput[DIK_RIGHT] = 1;
         }
         else if(NMAutoDir == 1)
         {
            strKeyInput[DIK_UP] = 1;
         }
         else if(NMAutoDir == 2)
         {
            strKeyInput[DIK_RIGHT] = 1;
         }
         else if(NMAutoDir == 3)
         {
            strKeyInput[DIK_DOWN] = 1;
         }
         else if(NMAutoDir == 4)
         {
            strKeyInput[DIK_LEFT] = 1;
         }

      }
   }
}