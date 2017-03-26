#pragma warning( disable : 4291 )
#pragma warning( disable : 4786 )

#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include "MMsystem.h"
#include "VisualObjectList.h"
#include "PowerDebug.h"
#include "TFCPlayer.h"
#include "ObjectListing.h"
#include "Bitmap.h"
#include "TileSet.h"
#include "TemplateQueues.h"
#include "TFCPacket.h"
#include "TFCSocket.h"
#include "Apparence.h"
#include "IDirectXFont.h"
#include "MacroHandler.h"
#include "C2.h"
#include "TFCLandDef.h"
#include "Mouse.h"
#include "Puppet.h"
#include "NTime.h"
#include <math.h>
#include "Spell.h"
#include "COMM.h"
#include "LocalString.h"
#include "Global.h"
#include "App.h"
#include "Packet.h"
#include "MemoryManager.h"
#include "ProcStat.h"
#include "FileLogger.h"
#include "GameIcons.h"
#include <math.h>
#include "NewInterface/ChatterUI.h"
#include "NewInterface/RootBoxUI.h"
#include "SaveGame.h"
extern CSaveGame g_SaveGame;
extern Font *fMenuFont;
extern CProcStat g_TimeStat;
extern bool Move2;
bool seraphDisplay = true;
bool goodSeraph = true;

#define VOL_PLAYER 1
#define VOL_NPC 2
#define VOL_MONSTER 3
#define VOL_OBJECT 4

#define VOL_CANNOTTALK 0x0
#define VOL_CANTALK 0x1
#define VOL_ISPLAYER 0x2

extern Font *fFont; 

extern DWORD SpellID;

extern int NBCURSOR;
extern T3VSBSound SoundFX[250];
extern T3VSBSound ConnectSoundFX;
extern unsigned long TalkToID;
extern short TalkToOffset;
extern HWND g_hwnd;
extern volatile unsigned long Round;
extern int ResizeCorrection;
extern TFCSocket Remote;
extern sockaddr_in ServerIPaddr;

VisualObjectList Objects;
extern Bitmap Grid;
extern Bitmap GridID;
extern Bitmap GridPriority;
extern Bitmap GridBlocking;
DWORD TargetID;
extern DWORD FreezeID;

extern float FramePerSecond;

inline int Abs(int x) {
    return ((x < 0) ? (-x) : (x));
}

signed char staticCharZero = 0;
short staticShortZero = 0;

#define StrMake(x, y) { x = new char [strlen(y)+1]; strcpy(x, y); }

#define isMovX ((abs((int)Object->MovX) ? (int)Object->MovX/abs((int)Object->MovX) : 0) != (abs((int)Object->SpeedX) ? (int)Object->SpeedX/abs((int)Object->SpeedX) : 0))
#define isMovY ((abs((int)Object->MovY) ? (int)Object->MovY/abs((int)Object->MovY) : 0) != (abs((int)Object->SpeedY) ? (int)Object->SpeedY/abs((int)Object->SpeedY) : 0))

#define KEEPALIVE_NM	if (timeGetTime() - dwLoadTime > 1500) {\
		TFCPacket Send; \
		dwLoadTime = timeGetTime();\
		Send << (short)10;\
		SEND_PACKET(Send);	\
		COMM.KeepAlive();\
	}
//		Sleep(50);\

// Bitmap fonctions
//////////////////////////////////////////////////////////////////////////////////////////
unsigned long &Bitmap::operator()
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns a reference to the value at the given position.
// 
(
 int x, // The 'x' screen position.
 int y  // The 'y' screen position
 )
 // Return: unsigned long, 
 //////////////////////////////////////////////////////////////////////////////////////////
{
    Temp = 0;
    
    if (x < 0){ 
        return Temp;
    }else if (x >= xMap){
        return Temp;
    }
    
    if (y < 0){
        return Temp;
    }else if (y >= yMap){
        return Temp;
    }
    
    return BitmapData[x+y*xMap];
}

void SetMouseCursor(TFCObject *Object) {
    if (Object->Type < 10000) {
        
        unsigned short tType;
        switch (Object->Type) {
        case __OBJGROUP_CHEST_I: tType = __OBJGROUP_CHEST; break;
        case __OBJGROUP_CHEST_OPEN_I: tType = __OBJGROUP_CHEST_OPEN; break;
        case __OBJGROUP_CHEST2_I: tType = __OBJGROUP_CHEST2; break;
        case __OBJGROUP_WOODEN_DOOR_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR_CLOSED; break;
        case __OBJGROUP_WOODEN_DOOR_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR_OPENED; break;
        case __OBJGROUP_WOODEN_CHAIR_I: tType = __OBJGROUP_WOODEN_CHAIR; break;
        case __OBJGROUP_WOODEN_CHAIR_2_I: tType = __OBJGROUP_WOODEN_CHAIR_2; break;
        case __OBJGROUP_WOODEN_ROUND_CHAIR_2_I: tType = __OBJGROUP_WOODEN_ROUND_CHAIR_2; break;
        case __OBJGROUP_2_WOODEN_CHAIR_I: tType = __OBJGROUP_2_WOODEN_CHAIR; break;
        case __OBJGROUP_2_WOODEN_CHAIR_2_I: tType = __OBJGROUP_2_WOODEN_CHAIR_2; break;
        case __OBJGROUP_2_WOODEN_ROUND_CHAIR_2_I: tType = __OBJGROUP_2_WOODEN_ROUND_CHAIR_2; break;
        case __OBJGROUP_VAULT_TALK_I: tType = __OBJGROUP_VAULT_TALK; break;
        case __OBJGROUP_VAULT_I: tType = __OBJGROUP_VAULT; break;
        case __OBJGROUP_DOOR_TALK_I: tType = __OBJGROUP_DOOR_TALK; break;
        case __OBJGROUP_SKAVEN_CORPSE_I1: tType = __OBJGROUP_SKAVEN_CORPSE1; break;
        case __OBJGROUP_SKAVEN_CORPSE_I2: tType = __OBJGROUP_SKAVEN_CORPSE2; break;
        case __OBJGROUP_SKAVEN_CORPSE_I3: tType = __OBJGROUP_SKAVEN_CORPSE3; break;
        case __OBJGROUP_SKAVEN_CORPSE_I4: tType = __OBJGROUP_SKAVEN_CORPSE4; break;
        case __OBJGROUP_WOODEN_DOOR2_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR2_CLOSED; break;
        case __OBJGROUP_WOODEN_DOOR2_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR2_OPENED; break;
        case __OBJGROUP_WOODEN_DOOR3_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR3_CLOSED; break;
        case __OBJGROUP_WOODEN_DOOR3_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR3_OPENED; break;
        case __OBJGROUP_WOODEN_DOOR4_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR4_CLOSED; break;
        case __OBJGROUP_WOODEN_DOOR4_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR4_OPENED; break;
        case __OBJGROUP_WOODEN_DOOR5_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR5_CLOSED; break;
        case __OBJGROUP_WOODEN_DOOR5_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR5_OPENED; break;
        case __OBJGROUP_WOODEN_DOOR6_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR6_CLOSED; break;
        case __OBJGROUP_WOODEN_DOOR6_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR6_OPENED; break;
        case __OBJGROUP_WOODEN_DOOR7_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR7_CLOSED; break;
        case __OBJGROUP_WOODEN_DOOR7_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR7_OPENED; break;
        case __OBJGROUP_WOODEN_DOOR8_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR8_CLOSED; break;
        case __OBJGROUP_WOODEN_DOOR8_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR8_OPENED; break;
        default: tType = Object->Type; break;
        }
        
        Object->MouseCursor = DefaultMouseCursor[tType];
    }
}


bool IsRangedObject( TFCObject *obj ){
    switch( obj->PuppetInfo[8] ){
    case PUPEQ_SMALL_BOW:
    case PUPEQ_LARGE_BOW:
    case PUPEQ_NEW_BOW01:
    case PUPEQ_V2_BOW01 :
    case PUPEQ_V2_BOW02 :
    case PUPEQ_V2_BOW03 :
    case PUPEQ_V2_BOW04 :
    case PUPEQ_V2_BOW05 :
    case PUPEQ_V2_BOW05_ICE:
    case PUPEQ_V2_BOW06 :
    case PUPEQ_V2_BOW07 :
    case PUPEQ_V2_BOW08 :
    case PUPEQ_V2_BOW09 :
    case PUPEQ_V2_BOW10 :
        return true;
    }
    return false;
}


//#undef POWERTRACE
//#define POWERTRACE POWERLOG
/////////////////////////////////////////////////////////////////////////////
VisualObjectList::VisualObjectList() {
    // Constructor
    LastAttempt      = 0;
    LastAttemptIDAll = 0;
    DoUnlock = false;
    nbUnit = 0;
    MainObject.Standing = ST_STANDING;
    MainObject.Name = 0;
    MainObject.Type = 0;
    MainObject.nbSprite4Move = 0;
    MainObject.MouseCursor = 0;
    csLock.SetCriticalSection("VisualObjectList", 90);
    NoMove = FALSE;
    
    GridPriority.Fill(INFINITE);
    
    for (int i = 0; i < NBBASICSPRITE; i++) {
        VObject3D[i] = NULL;
    }
    
    ZeroMemory(Object3DSound, 100*11*2);

    m_bAskForSkinChange  = FALSE;
    m_dwNewSkin          = 10011;
    



    SDClear();
    SDAnimClear();
   SDAddObject( 291,   24, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 311,   26, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 236,   29, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 286,   29, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 731,   29, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 256,   31, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 321,   36, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 746,   36, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 266,   41, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 726,   44, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 331,   46, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 756,   46, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 721,   49, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 706,   54, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 341,   56, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 766,   56, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 701,   59, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 721,   59, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 241,   64, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 756,   66, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 206,   69, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 236,   69, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 721,   69, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 201,   74, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 686,   74, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 726,   74, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 746,   74, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 766,   76, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 786,   76, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 296,   81, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1051,   84, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 301,   86, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 716,   86, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 726,   86, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 796,   86, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1101,   86, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 671,   89, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1066,   89, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 246,   91, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 731,   91, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1041,   94, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1081,   94, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 726,   96, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 806,   96, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 196,   99, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 711,   99, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 751,  101, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 171,  104, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 291,  104, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 786,  104, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1031,  104, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 271,  106, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 816,  106, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1121,  106, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 166,  109, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 721,  111, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 646,  114, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 806,  114, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 826,  116, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 771,  119, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1016,  119, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 691,  121, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 711,  121, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1046,  121, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 636,  124, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 776,  124, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1006,  129, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 841,  131, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 451,  134, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 786,  134, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 806,  134, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 811,  139, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 486,  141, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 851,  141, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1056,  141, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 696,  144, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 716,  144, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 736,  144, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1081,  144, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 706,  146, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 436,  149, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 611,  149, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1076,  149, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 751,  151, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1066,  151, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1106,  151, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1166,  151, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 201,  154, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 991,  154, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 501,  156, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 726,  156, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 846,  156, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 691,  159, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1016,  161, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1046,  161, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 191,  164, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 461,  164, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 836,  164, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 736,  166, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1141,  166, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 496,  169, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 691,  169, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 851,  169, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 516,  171, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 731,  171, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 816,  174, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 981,  174, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 676,  176, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 746,  176, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1061,  176, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  41,  179, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 731,  179, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 991,  179, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1046,  179, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  61,  181, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 741,  181, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 801,  181, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 441,  184, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 461,  184, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 756,  184, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 451,  186, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 826,  186, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  31,  189, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 396,  189, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 691,  189, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 721,  189, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 731,  189, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1026,  189, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1036,  189, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 536,  191, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 781,  191, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 161,  194, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 431,  194, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 471,  194, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 706,  196, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 796,  196, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 651,  199, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 721,  199, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 811,  199, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1016,  199, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 151,  204, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 381,  204, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 646,  204, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 696,  204, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 766,  204, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 971,  204, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 501,  206, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 551,  206, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 631,  209, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 781,  209, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 196,  211, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 711,  211, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 531,  214, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 676,  214, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 746,  216, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 796,  216, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1021,  216, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 621,  219, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1136,  219, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 101,  221, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 516,  221, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 771,  221, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 441,  224, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1131,  224, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 551,  226, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 766,  226, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 786,  226, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 986,  229, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1076,  229, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 701,  231, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 781,  231, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 551,  234, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 561,  236, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1066,  239, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 131,  241, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 541,  244, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 746,  244, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1001,  244, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1041,  246, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  71,  249, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  66,  254, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 511,  254, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 136,  256, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  41,  259, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 101,  261, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 541,  264, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 551,  266, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 746,  266, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1076,  271, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 511,  274, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 531,  274, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 321,  276, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 296,  279, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1036,  279, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 196,  284, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 331,  286, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 511,  286, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  21,  289, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 191,  289, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 286,  289, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 151,  291, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  16,  294, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 126,  294, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 186,  294, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 521,  296, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 496,  311, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 106,  314, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 126,  316, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 201,  321, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  96,  324, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 136,  326, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 331,  326, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  11,  329, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  41,  329, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 346,  329, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 211,  331, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  66,  336, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 176,  344, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  76,  346, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 136,  346, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 171,  349, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 911,  351, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  21,  359, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 171,  361, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  86,  374, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 296,  379, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 326,  379, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 406,  379, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 386,  381, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 401,  384, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 431,  386, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 421,  391, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 971,  391, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 221,  419, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 211,  429, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 251,  431, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 156,  434, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  31,  444, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 146,  444, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  41,  446, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  21,  454, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  51,  456, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  11,  464, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 206,  466, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  56,  471, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  61,  486, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 226,  486, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  66,  491, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  81,  494, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  91,  496, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  96,  501, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 176,  514, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 221,  529, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  96,  591, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 111,  606, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 111,  626, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 301,  629, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 291,  639, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 111,  646, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 281,  649, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 116,  651, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 131,  654, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 141,  656, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 271,  659, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 146,  661, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 261,  669, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 311,  669, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 411,  671, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 336,  676, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 251,  679, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 421,  681, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 296,  684, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 241,  689, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 431,  691, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 231,  699, 1, 100, __OBJGROUP_TORCHE_I); 
   SDAddObject( 441,  701, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 221,  709, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 451,  711, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 211,  719, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 461,  721, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 201,  729, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 471,  731, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 191,  739, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 381,  739, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 481,  741, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 316,  744, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 181,  749, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 311,  749, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 171,  759, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 291,  759, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 201,  761, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 451,  761, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 456,  766, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 161,  769, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 181,  769, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 246,  774, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 416,  774, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 151,  779, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 201,  779, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 271,  779, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 141,  789, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 171,  791, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 416,  794, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 411,  799, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 341,  801, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 406,  804, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 121,  809, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 171,  809, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 401,  809, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 351,  811, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 326,  814, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 491,  824, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 341,  829, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 371,  829, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 221,  841, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 196,  846, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 231,  851, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 241,  861, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 396,  864, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 406,  869, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 386,  874, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 411,  874, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 396,  879, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 201,  881, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 376,  884, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 401,  884, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 206,  886, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 341,  899, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1496,  899, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 271,  901, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 336,  904, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1491,  904, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 276,  906, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 366,  914, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1546,  919, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 276,  924, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 271,  929, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 321,  929, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1536,  929, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1481,  934, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 311,  939, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1526,  939, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1586,  941, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 401,  944, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 301,  949, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1596,  951, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1646,  961, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1391,  964, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1461,  964, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1481,  966, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1386,  969, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1656,  971, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1451,  974, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1666,  981, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1441,  984, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1636,  986, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 331,  989, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1591,  996, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1616, 1006, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1466, 1009, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 336, 1014, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1441, 1016, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1546, 1019, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1666, 1019, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1616, 1021, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1686, 1021, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1451, 1026, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1536, 1029, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1656, 1029, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1696, 1031, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1526, 1039, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1476, 1051, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 241, 1059, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1416, 1059, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 261, 1061, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1486, 1061, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1586, 1061, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1531, 1066, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1711, 1066, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 271, 1071, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1596, 1071, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 306, 1076, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1721, 1076, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1606, 1081, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 316, 1086, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1731, 1086, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 126, 1094, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1646, 1099, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1546, 1101, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 116, 1104, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1641, 1104, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 141, 1109, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 261, 1111, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1521, 1116, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 251, 1129, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 191, 1131, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 246, 1134, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 226, 1136, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1691, 1136, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1556, 1149, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1616, 1159, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1656, 1159, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1611, 1164, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 191, 1189, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 181, 1199, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 201, 1221, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 241, 1231, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 251, 1241, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 196, 1254, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 356, 1299, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 416, 1299, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 436, 1301, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 346, 1309, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 476, 1311, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 401, 1334, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 336, 1339, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 296, 1351, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 431, 1366, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 166, 1371, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 141, 1374, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 441, 1376, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 396, 1379, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 176, 1381, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 131, 1384, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  76, 1389, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 276, 1389, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  96, 1391, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 186, 1391, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 121, 1394, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  66, 1399, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 136, 1399, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 241, 1404, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 231, 1414, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 161, 1424, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 221, 1424, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 401, 1424, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 431, 1456, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 441, 1466, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 316, 1481, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 431, 1484, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 346, 1491, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 401, 1494, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 381, 1496, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 191, 1501, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 156, 1504, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 146, 1514, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 206, 1516, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 141, 1519, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 131, 1529, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 196, 1534, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 181, 1549, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 286, 1554, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 876, 1556, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 306, 1558, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 166, 1574, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 901, 1579, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 891, 1589, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 911, 1601, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 236, 1604, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 951, 1649, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 961, 1651, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 441, 1699, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 461, 1701, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 431, 1709, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 471, 1711, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 506, 1714, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 831, 1714, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 901, 1716, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 421, 1719, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 906, 1721, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 911, 1726, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 551, 1731, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 916, 1731, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 181, 1734, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 201, 1736, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 921, 1736, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 561, 1741, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 601, 1741, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 406, 1744, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 236, 1751, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 611, 1751, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 396, 1754, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 161, 1764, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 386, 1764, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 151, 1774, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 901, 1784, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 571, 1789, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 266, 1791, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 361, 1791, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 566, 1794, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 931, 1794, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 426, 1796, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 921, 1796, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 941, 1796, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 206, 1799, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 561, 1799, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 371, 1801, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 946, 1801, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 556, 1804, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 951, 1806, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 516, 1814, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 911, 1814, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 226, 1821, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 451, 1821, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 131, 1824, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 506, 1824, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 576, 1826, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 121, 1834, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 496, 1834, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 586, 1836, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 486, 1844, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 536, 1844, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 596, 1846, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 461, 1851, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 376, 1854, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 331, 1859, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 431, 1861, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 321, 1869, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 441, 1871, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 256, 1879, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 591, 1899, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 351, 1901, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 231, 1904, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 356, 1906, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 321, 1909, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 221, 1914, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 376, 1914, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 371, 1919, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 366, 1924, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 361, 1929, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 541, 1929, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 191, 1944, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 181, 1954, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 191, 2024, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 591, 2024, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 156, 2029, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 621, 2046, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 626, 2051, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 481, 2054, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 531, 2054, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 651, 2054, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 211, 2056, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 551, 2056, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 631, 2056, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 671, 2056, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 526, 2059, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 556, 2061, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 676, 2061, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 471, 2064, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 521, 2064, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 591, 2064, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 461, 2074, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 561, 2084, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 426, 2089, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 556, 2089, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 626, 2089, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 551, 2094, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 616, 2099, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 461, 2106, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 606, 2109, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 466, 2111, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 536, 2111, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 201, 2116, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 496, 2119, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 596, 2119, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 511, 2126, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 631, 2126, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 586, 2129, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 516, 2131, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 251, 2134, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 601, 2134, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 521, 2136, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 561, 2136, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 246, 2139, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 411, 2144, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 176, 2149, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 491, 2156, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 556, 2159, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1271, 2181, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 591, 2186, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1281, 2191, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 221, 2194, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 141, 2196, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 541, 2204, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1426, 2206, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 106, 2209, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 536, 2209, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 531, 2214, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 836, 2214, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1436, 2216, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 526, 2219, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 166, 2221, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 246, 2221, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1446, 2226, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 881, 2236, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1456, 2236, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 156, 2239, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1466, 2246, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1211, 2259, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1141, 2261, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1151, 2271, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1161, 2281, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1186, 2284, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1451, 2289, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1291, 2299, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1441, 2299, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1021, 2301, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 571, 2304, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 506, 2309, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1431, 2309, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 526, 2311, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 596, 2311, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1031, 2311, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 166, 2314, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 501, 2314, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 561, 2314, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 966, 2314, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 606, 2321, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 551, 2324, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1261, 2329, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 646, 2331, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 486, 2339, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 586, 2341, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 656, 2341, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 481, 2344, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 936, 2344, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1126, 2344, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 591, 2346, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 416, 2349, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 476, 2349, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 471, 2354, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 521, 2354, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 466, 2359, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 516, 2359, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 461, 2364, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 616, 2369, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1101, 2369, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 556, 2371, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 491, 2374, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1366, 2374, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 561, 2376, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 691, 2376, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 606, 2379, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 481, 2384, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 541, 2386, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 596, 2389, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 471, 2394, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1076, 2394, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 271, 2399, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 261, 2401, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 281, 2401, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 461, 2404, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1336, 2404, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1251, 2409, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 486, 2411, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 451, 2414, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 711, 2414, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 731, 2416, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 566, 2419, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 706, 2419, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 441, 2424, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 561, 2424, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 416, 2439, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 476, 2441, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 706, 2441, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 521, 2446, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1441, 2449, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1286, 2454, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1011, 2459, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 191, 2464, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1106, 2464, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1276, 2464, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 576, 2471, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 181, 2474, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 491, 2474, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1096, 2474, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 171, 2484, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 986, 2484, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1406, 2484, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 471, 2494, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1266, 2494, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 251, 2496, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1066, 2504, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 581, 2506, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 961, 2509, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 266, 2511, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 591, 2516, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 886, 2516, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1236, 2524, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 281, 2526, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 896, 2526, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1041, 2529, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 111, 2534, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 906, 2536, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1826, 2536, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 106, 2539, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 296, 2541, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 201, 2544, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 196, 2549, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 526, 2551, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1841, 2551, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1016, 2554, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1056, 2564, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 951, 2581, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 961, 2591, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 281, 2596, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  46, 2599, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1121, 2601, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 816, 2604, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  36, 2609, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 811, 2609, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1151, 2609, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1131, 2611, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1046, 2614, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 246, 2619, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1241, 2619, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 266, 2621, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1036, 2624, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  96, 2629, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 236, 2629, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  91, 2634, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1026, 2634, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1126, 2634, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1006, 2636, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 156, 2641, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1111, 2649, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 206, 2659, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1001, 2659, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1031, 2661, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1076, 2664, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  41, 2666, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 121, 2666, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 196, 2669, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 991, 2669, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 221, 2674, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 131, 2676, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 981, 2679, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  66, 2691, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  76, 2701, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1031, 2709, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1016, 2724, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 121, 2741, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 126, 2746, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 906, 2751, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 916, 2761, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 966, 2774, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 981, 2781, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(2046, 2781, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(2096, 2781, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject( 956, 2784, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 986, 2786, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(2111, 2796, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(2126, 2811, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(2486, 2811, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(2491, 2816, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(2171, 2826, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(1996, 2839, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2026, 2839, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1976, 2859, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 116, 2869, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 111, 2874, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 106, 2879, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 101, 2884, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(  96, 2889, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 116, 2889, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2866, 2891, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(  91, 2894, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject( 111, 2894, 1, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2101, 2896, 1, 100, __OBJGROUP_TORCHE);
   SDAddObject(2876, 2901, 1, 100, __OBJGROUP_TORCHE);


   //NMNMNM --> Ajout de torche...
   // [Chateau Etage 1]
   SDAddObject(1430, 2373, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1419, 2362, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1415, 2358, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1411, 2354, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1407, 2350, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1421, 2307, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1350, 2368, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1364, 2381, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1378, 2395, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1393, 2410, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1409, 2427, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1474, 2360, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1446, 2344, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(1434, 2332, 0, 100, __OBJGROUP_TORCHE);

   SDAddObject(1386, 2347, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1382, 2351, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1401, 2305, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1401, 2323, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1385, 2339, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1369, 2355, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1353, 2371, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1335, 2371, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1395, 2410, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1402, 2403, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1434, 2367, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1442, 2359, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1398, 2422, 0, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(1457, 2363, 0, 100, __OBJGROUP_TORCHE_I);


   // [Chateau Etage 2]
   SDAddObject(2872, 155, 4, 100, __OBJGROUP_TORCHE);
   SDAddObject(2831, 244, 4, 100, __OBJGROUP_TORCHE);
   SDAddObject(2841, 254, 4, 100, __OBJGROUP_TORCHE);
   SDAddObject(2878, 233, 4, 100, __OBJGROUP_TORCHE);
   SDAddObject(2850, 205, 4, 100, __OBJGROUP_TORCHE);
   SDAddObject(2850, 205, 4, 100, __OBJGROUP_TORCHE);
   SDAddObject(2900, 195, 4, 100, __OBJGROUP_TORCHE);
   SDAddObject(2891, 186, 4, 100, __OBJGROUP_TORCHE);
   

   SDAddObject(2855, 158, 4, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2857, 174, 4, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2837, 194, 4, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2832, 199, 4, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2808, 223, 4, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2850, 259, 4, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2852, 275, 4, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2867, 242, 4, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2879, 230, 4, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2892, 217, 4, 100, __OBJGROUP_TORCHE_I);
   SDAddObject(2911, 216, 4, 100, __OBJGROUP_TORCHE_I);

   SDAddObject(2589, 2373, 0, 100, __OBJGROUP_TORCHE);
   SDAddObject(2594, 2378, 0, 100, __OBJGROUP_TORCHE);
   
    // SDEnd();
}

/////////////////////////////////////////////////////////////////////////////
VisualObjectList::~VisualObjectList() {
    // Destructor
    Release();
}	

CV2Sprite*  VisualObjectList::GetVSFObject(int dwIndex)
{
   //Lock(6666);
   if(dwIndex <0 || dwIndex >= (__OBJGROUP_LASTGROUP+__OBJANIM_LASTGROUP))
   {
      OutputDebugString("*********************** BUG BUG BUG *****************************\n");
      return &VObject[4]; // MorningStar
   }
   return &VObject[dwIndex];
   //Unlock(6666);
}

void VisualObjectList::Release(void) {
    DeleteAll();
    ReleaseAll();
    if (Object3DLoad[Set]) {
        Object3DLoad[Set] = false;
        VObject3D[Set]->Release();
        delete VObject3D[Set];
        VObject3D[Set] = NULL;
        for (int j = 0; j < 11; j++) {
            if (Object3DSound[Object3DSound[Set][j]]) {
                SoundFX[Object3DSound[Set][j]].Release(TRUE);
            } else {
                break;
            }
        }
    }
}

void VisualObjectList::LoadObject(int x) 
{
    if (!Object3DLoad[x]) 
    {
        if (g_Var.inGame) 
        {
           g_Var.wDisplayDisk = 1;
        }
        if (Custom.Debug) 
        {
            char Temp[1000];
            sprintf(Temp, "Loading Drawing #%d", x);
            ChatterUI::GetInstance()->AddBackscrollItem(
                "",
                Temp,
                RGB(255,255,255),
                true
                );            
        }
        switch (x) {
        case 0:
            VObject3D[0] = new Sprite3D;
            VObject3D[0]->LoadSprite3D(7, 8, 0, 0, 11, "Beholder", 1, 0, 0);
            SoundFX[45].Create("Beholder Dying", TS_MEMORY);
            SoundFX[46].Create("Beholder Attack", TS_MEMORY);
            SoundFX[47].Create("Beholder Hit", TS_MEMORY);
            Object3DSound[0][0] = 45;
            Object3DSound[0][1] = 46;
            Object3DSound[0][2] = 47;
            break;
        case 1: 
            VObject3D[1] = new Sprite3D;
            VObject3D[1]->LoadSprite3D(12, 11, 0, 0, 1, "Wizard", 0, 0, 0);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[13].Create("Whooshm 1", TS_MEMORY);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[1][0] = 19;
            Object3DSound[1][1] = 13;
            Object3DSound[1][2] = 98;
            Object3DSound[1][3] = 99;
            Object3DSound[1][4] = 96;
            Object3DSound[1][5] = 97;
            break;
        case 2:
            VObject3D[2] = new Sprite3D;
            VObject3D[2]->LoadSprite3D(12, 9, 0, 0, 15, "Goblin", 0, 0, 0); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[2][0] = 11;
            Object3DSound[2][1] = 18;
            Object3DSound[2][2] = 33;
            Object3DSound[2][3] = 34;
            break;
        case 3:
            VObject3D[3] = new Sprite3D;
            VObject3D[3]->LoadSprite3D(9, 10, 0, 0, 1, "Mummy", 0, 0, 0); 
            SoundFX[66].Create("Mummy Dying", TS_MEMORY);
            SoundFX[67].Create("Mummy Attack", TS_MEMORY);
            SoundFX[68].Create("Mummy Hit", TS_MEMORY);
            Object3DSound[3][0] = 66;
            Object3DSound[3][1] = 67;
            Object3DSound[3][2] = 68;
            break;
        case 4:
            VObject3D[4] = new Sprite3D;
            VObject3D[4]->LoadSprite3D(9, 9, 0, 0, 11, "Demon", 0, 0, 0); 
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[4][0] = 27;
            Object3DSound[4][1] = 28;
            Object3DSound[4][2] = 29;
            break;
        case 5:
            VObject3D[5] = new Sprite3D;
            VObject3D[5]->LoadSprite3D(6, 9, 0, 0, 13, "Minotaur", 0, 0, 0); 
            SoundFX[107].Create("Minotaur Dying", TS_MEMORY);
            SoundFX[108].Create("Minotaur Attack", TS_MEMORY);
            SoundFX[109].Create("Minotaur Hit", TS_MEMORY);
            Object3DSound[5][0] = 107;
            Object3DSound[5][1] = 108;
            Object3DSound[5][2] = 109;
            break;
        case 6:
            VObject3D[6] = new Sprite3D;
            VObject3D[6]->LoadSprite3D(13, 9, 0, 0, 12, "Thief", 0, 0, 0);
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[14].Create("Whooshm 2", TS_MEMORY);
            SoundFX[15].Create("Whooshm 3", TS_MEMORY);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[6][0] = 4;
            Object3DSound[6][1] = 14;
            Object3DSound[6][2] = 15;
            Object3DSound[6][3] = 98;
            Object3DSound[6][4] = 99;
            Object3DSound[6][5] = 96;
            Object3DSound[6][6] = 97;
            break;
        case 7:
            VObject3D[7] = new Sprite3D;
            SoundFX[35].Create("Rat Dying", TS_MEMORY);
            SoundFX[36].Create("Rat Attack", TS_MEMORY);
            SoundFX[37].Create("Rat Hit", TS_MEMORY);
            VObject3D[7]->LoadSprite3D(6, 9, 0, 0, 10, "Rat", 0, 0, 0);
            Object3DSound[7][0] = 35;
            Object3DSound[7][1] = 36;
            Object3DSound[7][2] = 37;
            break;
        case 8:
            VObject3D[8] = new Sprite3D;
            VObject3D[8]->LoadSprite3D(8, 9, 0, 0, 12, "Bat", 2, 0, 0);
            SoundFX[25].Create("Bat Attack", TS_MEMORY);
            SoundFX[24].Create("Bat Dying", TS_MEMORY);
            SoundFX[26].Create("Bat Hit", TS_MEMORY);
            Object3DSound[8][0] = 25;
            Object3DSound[8][1] = 24;
            Object3DSound[8][2] = 26;
            break;
        case 9:
            VObject3D[9] = new Sprite3D;
            VObject3D[9]->LoadSprite3D(6, 6, 0, 0, 13, "Spider", 3, 0, 0); 
            SoundFX[72].Create("Spider Dying", TS_MEMORY);
            SoundFX[73].Create("Spider Attack", TS_MEMORY);
            SoundFX[74].Create("Spider Hit", TS_MEMORY);
            Object3DSound[9][0] = 72;
            Object3DSound[9][1] = 73;
            Object3DSound[9][2] = 74;
            break;
        case 10:
            VObject3D[10] = new Sprite3D;
            VObject3D[10]->LoadSprite3D(7, 9, 0, 0, 11, "Skeleton", 0, 0, 0); 
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[38].Create("Skeleton Dying", TS_MEMORY);
            SoundFX[39].Create("Skeleton Hit", TS_MEMORY);
            Object3DSound[10][0] = 4;
            Object3DSound[10][1] = 12;
            Object3DSound[10][2] = 18;
            Object3DSound[10][4] = 38;
            Object3DSound[10][5] = 39;
            break;
        case 11:
            VObject3D[11] = new Sprite3D;
            VObject3D[11]->LoadSprite3D(13, 12, 0, 0, 11, "BlackWarrior", 0, 0, 0);
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[11][0] = 4;
            Object3DSound[11][1] = 11;
            Object3DSound[11][2] = 12;
            Object3DSound[11][3] = 98;
            Object3DSound[11][4] = 99;
            Object3DSound[11][5] = 96;
            Object3DSound[11][6] = 97;
            break;
        case 12:
            VObject3D[12] = new Sprite3D;
            VObject3D[12]->LoadSprite3D(12, 10, 0, 0, 15, "Priest", 0, 0, 0);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[12][0] = 19;
            Object3DSound[12][1] = 11;
            Object3DSound[12][2] = 98;
            Object3DSound[12][3] = 99;
            Object3DSound[12][4] = 96;
            Object3DSound[12][5] = 97;
            break;
        case 13:
            VObject3D[13] = new Sprite3D;
            VObject3D[13]->LoadSprite3D(6, 0, 1, "PaysanModel1", 0, 0);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[13][0] = 98;
            Object3DSound[13][1] = 99;
            Object3DSound[13][2] = 96;
            Object3DSound[13][3] = 97;
            break;
        case 14:
            VObject3D[14] = new Sprite3D;
            VObject3D[14]->LoadSprite3D(8, 8, 0, 0, 1, "GuardModel1", 0, 0, 0);
            SoundFX[16].Create("Whooshm 4", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[14][0] = 16;
            Object3DSound[14][1] = 18;
            Object3DSound[14][2] = 98;
            Object3DSound[14][3] = 99;
            Object3DSound[14][4] = 96;
            Object3DSound[14][5] = 97;
            break;
        case 15:
            VObject3D[15] = new Sprite3D;
            VObject3D[15]->LoadSprite3D(8, 0, 1, "Mage", 0, 0);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[13].Create("Whooshm 1", TS_MEMORY);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[15][0] = 19;
            Object3DSound[15][1] = 13;
            Object3DSound[15][2] = 98;
            Object3DSound[15][3] = 99;
            Object3DSound[15][4] = 96;
            Object3DSound[15][5] = 97;
            break;
        case 16:
            VObject3D[16] = new Sprite3D;
            VObject3D[16]->LoadSprite3D(12, 0, 1, "PaysanneModel1", 0, 0);
            SoundFX[53].Create("Female Hit 1", TS_MEMORY);
            SoundFX[54].Create("Female Hit 2", TS_MEMORY);
            SoundFX[51].Create("Female Dying 1", TS_MEMORY);
            SoundFX[52].Create("Female Dying 2", TS_MEMORY);
            Object3DSound[16][0] = 53;
            Object3DSound[16][1] = 54;
            Object3DSound[16][2] = 51;
            Object3DSound[16][3] = 52;
            break;
        case 17:
            VObject3D[17] = new Sprite3D;
            VObject3D[17]->LoadSprite3D(8, 0, 1, "Cleric", 0, 0);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[17][0] = 98;
            Object3DSound[17][1] = 99;
            Object3DSound[17][2] = 96;
            Object3DSound[17][3] = 97;
            break;
        case 18:
            VObject3D[18] = new Sprite3D;
            VObject3D[18]->LoadSprite3D(12, 7, 0, 0, 1, "Kobold", 0, 0, 0); 
            SoundFX[15].Create("Whooshm 3", TS_MEMORY);
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[5].Create("Kobold Hit", TS_MEMORY);
            SoundFX[3].Create("Kobold Dying", TS_MEMORY);
            Object3DSound[18][0] = 15;
            Object3DSound[18][1] = 11;
            Object3DSound[18][2] = 4;
            Object3DSound[18][3] = 5;
            Object3DSound[18][4] = 3;
            break;
        case 19:
            VObject3D[19] = new Sprite3D;
            VObject3D[19]->LoadSpriteC(23, 35, 26, "Slime");
            SoundFX[30].Create("Ooze Dying", TS_MEMORY);
            SoundFX[31].Create("Ooze Attack", TS_MEMORY);
            SoundFX[32].Create("Ooze Hit", TS_MEMORY);
            Object3DSound[19][0] = 30;
            Object3DSound[19][1] = 31;
            Object3DSound[19][2] = 32;
            break;
        case 20:
            VObject3D[20] = new Sprite3D;
            VObject3D[20]->LoadSprite3D(7, 9, 0, 0, 11, "Orc", 0, 0, 0);
            SoundFX[103].Create("Whooshm 8", TS_MEMORY);
            SoundFX[105].Create("Orc Dying", TS_MEMORY);
            SoundFX[106].Create("Orc Hit", TS_MEMORY);
            Object3DSound[20][0] = 103;;
            Object3DSound[20][1] = 105;
            Object3DSound[20][2] = 106;
            break;
        case 21:
            VObject3D[21] = new Sprite3D;
            VObject3D[21]->LoadSprite3D(8, 7, 0, 0, 10, "Zombie", 0, 0, 0);
            SoundFX[67].Create("Mummy Attack", TS_MEMORY);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[83].Create("Zombie Dying", TS_MEMORY);
            SoundFX[84].Create("Zombie Hit", TS_MEMORY);
            Object3DSound[21][0] = 67;
            Object3DSound[21][1] = 19;
            Object3DSound[21][2] = 83;
            Object3DSound[21][3] = 84;
            break;
        case 22:
            VObject3D[22] = new Sprite3D;
            VObject3D[22]->LoadSprite3D(6, 7, 0, 0, 11, "GreenTroll", 0, 0, 0);
            SoundFX[21].Create("Troll Dying", TS_MEMORY);
            SoundFX[22].Create("Troll Attack", TS_MEMORY);
            SoundFX[23].Create("Troll Hit", TS_MEMORY);
            Object3DSound[22][0] = 21;
            Object3DSound[22][1] = 22;
            Object3DSound[22][2] = 23;
            break;
        case 23:
            VObject3D[23] = new Sprite3D;
            VObject3D[23]->LoadSprite3D(12, 12, 0, 0, 1, "Warrio", 0, 0, 0);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[15].Create("Whooshm 3", TS_MEMORY);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[23][0] = 12;
            Object3DSound[23][1] = 15;
            Object3DSound[23][2] = 98;
            Object3DSound[23][3] = 99;
            Object3DSound[23][4] = 96;
            Object3DSound[23][5] = 97;
            break;
        case 24:
            VObject3D[24] = new Sprite3D;
            VObject3D[24]->LoadSprite3D(12, 12, 0, 0, 1, "Warrio", 0, 0, 0); // Brigand
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[15].Create("Whooshm 3", TS_MEMORY);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[24][0] = 4;
            Object3DSound[24][1] = 19;
            Object3DSound[24][2] = 15;
            Object3DSound[24][3] = 98;
            Object3DSound[24][4] = 99;
            Object3DSound[24][5] = 96;
            Object3DSound[24][6] = 97;
            break;
        case 25:
            VObject3D[25] = new Sprite3D;
            VObject3D[25]->LoadSprite3D(13, 11, 0, 0, 11, "SmallWorm", 0, 0, 0);
            SoundFX[40].Create("Worm Dying", TS_MEMORY);
            SoundFX[41].Create("Worm Attack", TS_MEMORY);
            SoundFX[42].Create("Worm Hit", TS_MEMORY);
            Object3DSound[25][0] = 40;
            Object3DSound[25][1] = 41;
            Object3DSound[25][2] = 42;
            break;
        case 26:
            VObject3D[26] = new Sprite3D;
            VObject3D[26]->LoadSprite3D(8, 11, 0, 0, 13, "BigWorm", 0, 0, 0);
            SoundFX[40].Create("Worm Dying", TS_MEMORY);
            SoundFX[41].Create("Worm Attack", TS_MEMORY);
            SoundFX[42].Create("Worm Hit", TS_MEMORY);
            Object3DSound[26][0] = 40;
            Object3DSound[26][1] = 41;
            Object3DSound[26][2] = 42;
            break;
        case 27:
            VObject3D[27] = new Sprite3D;
            VObject3D[27]->LoadSprite3D(9, 9, 0, 0, 10, "TreeEnt", 0, 0, 0);
            SoundFX[75].Create("Tree Ent Dying", TS_MEMORY);
            SoundFX[76].Create("Electrik", TS_MEMORY);
            SoundFX[90].Create("AxeWood", TS_MEMORY);
            Object3DSound[27][0] = 75;
            Object3DSound[27][1] = 76;
            Object3DSound[27][2] = 90;
            break;
        case 28:
            VObject3D[28] = new Sprite3D;
            VObject3D[28]->LoadSprite3D(8, 7, 0, 0, 13, "Snake", 0, 0, 0);
            SoundFX[69].Create("Snake Dying", TS_MEMORY);
            SoundFX[70].Create("Snake Attack", TS_MEMORY);
            SoundFX[71].Create("Snake Hit", TS_MEMORY);
            Object3DSound[28][0] = 69;
            Object3DSound[28][1] = 70;
            Object3DSound[28][2] = 71;
            break;
        case 29:
            VObject3D[29] = new Sprite3D;
            VObject3D[29]->LoadSprite3D(8, 8, 0, 0, 13, "Unicorn", 0, 0, 0);
            SoundFX[100].Create("Pegase Dying", TS_MEMORY);
            SoundFX[101].Create("Pegase Attack", TS_MEMORY);
            SoundFX[102].Create("Pegase Hit", TS_MEMORY);
            Object3DSound[29][0] = 100;
            Object3DSound[29][1] = 101;
            Object3DSound[29][2] = 102;
            break;
        case 30:
            VObject3D[30] = new Sprite3D;
            VObject3D[30]->LoadSprite3D(8, 8, 0, 0, 15, "Centaur", 0, 0, 0);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[94].Create("Centaur Dying", TS_MEMORY);
            SoundFX[95].Create("Centaur Hit", TS_MEMORY);
            SoundFX[104].Create("Whooshm 9", TS_MEMORY);
            Object3DSound[30][0] = 19;
            Object3DSound[30][1] = 94;
            Object3DSound[30][2] = 95;
            Object3DSound[30][3] = 104;
            break;
        case 31:
            VObject3D[31] = new Sprite3D;
            VObject3D[31]->LoadSprite3D(8, 0, 12, "Horse", 0, 0);
            SoundFX[100].Create("Pegase Dying", TS_MEMORY);
            SoundFX[101].Create("Pegase Attack", TS_MEMORY);
            SoundFX[102].Create("Pegase Hit", TS_MEMORY);
            Object3DSound[31][0] = 100;
            Object3DSound[31][1] = 101;
            Object3DSound[31][2] = 102;
            break;
        case 32:
            VObject3D[32] = new Sprite3D;
            VObject3D[32]->LoadSprite3D(8, 8, 0, 0, 13, "Pegasus", 0, 0, 0);
            SoundFX[100].Create("Pegase Dying", TS_MEMORY);
            SoundFX[101].Create("Pegase Attack", TS_MEMORY);
            SoundFX[102].Create("Pegase Hit", TS_MEMORY);
            Object3DSound[32][0] = 100;
            Object3DSound[32][1] = 101;
            Object3DSound[32][2] = 102;
            break;
        case 33:
            VObject3D[33] = new Sprite3D;
            VObject3D[33]->LoadSprite3D(8, 7, 0, 0, 12, "Scorpion", 0, 0, 0);
            SoundFX[58].Create("Elemear Attack", TS_MEMORY);
            SoundFX[85].Create("Scorpion Dying", TS_MEMORY);
            SoundFX[86].Create("Scorpion Hit", TS_MEMORY);
            Object3DSound[33][0] = 58;
            Object3DSound[33][1] = 85;
            Object3DSound[33][2] = 86;
            break;
        case 34:
            VObject3D[34] = new Sprite3D;
            VObject3D[34]->LoadSprite3D(8, 8, 0, 0, 12, "Kraanian", 0, 0, 0);
            SoundFX[48].Create("Atrocity Dying", TS_MEMORY);
            SoundFX[49].Create("Atrocity Attack", TS_MEMORY);
            SoundFX[50].Create("Atrocity Hit", TS_MEMORY);
            Object3DSound[34][0] = 48;
            Object3DSound[34][1] = 49;
            Object3DSound[34][2] = 50;
            break;
        case 35:
            VObject3D[35] = new Sprite3D;
            VObject3D[35]->LoadSprite3D(8, 8, 0, 0, 11, "Atrocity", 0, 0, 0);
            SoundFX[91].Create("Beast Dying", TS_MEMORY);
            SoundFX[92].Create("Beast Attack", TS_MEMORY);
            SoundFX[93].Create("Beast Hit", TS_MEMORY);
            Object3DSound[35][0] = 91;
            Object3DSound[35][1] = 92;
            Object3DSound[35][2] = 93;
            break;
        case 36:
            VObject3D[36] = new Sprite3D;
            VObject3D[36]->LoadSprite3D(9, 9, 0, 0, 10, "TreeEnt", 0, 0, 0); // Nightmare
            SoundFX[100].Create("Pegase Dying", TS_MEMORY);
            SoundFX[101].Create("Pegase Attack", TS_MEMORY);
            SoundFX[102].Create("Pegase Hit", TS_MEMORY);
            Object3DSound[36][0] = 100;
            Object3DSound[36][1] = 101;
            Object3DSound[36][2] = 102;
            break;
        case 37:
            VObject3D[37] = new Sprite3D;
            VObject3D[37]->LoadSprite3D(1, 15, 1, "Dragon", 0, -1);
            SoundFX[87].Create("Wasp Dying", TS_MEMORY);
            SoundFX[88].Create("Wasp Attack", TS_MEMORY);
            SoundFX[89].Create("Wasp Hit", TS_MEMORY);
            Object3DSound[37][0] = 87;
            Object3DSound[37][1] = 88;
            Object3DSound[37][2] = 89;
            break;
        case 38:
            VObject3D[38] = new Sprite3D;
            VObject3D[38]->LoadSprite3D(8, 8, 0, 0, 11, "GiantWasp", 2, 0, 0);
            SoundFX[87].Create("Wasp Dying", TS_MEMORY);
            SoundFX[88].Create("Wasp Attack", TS_MEMORY);
            SoundFX[89].Create("Wasp Hit", TS_MEMORY);
            Object3DSound[38][0] = 87;
            Object3DSound[38][1] = 88;
            Object3DSound[38][2] = 89;
            break;
        case 39:
            VObject3D[39] = new Spell;
            VObject3D[39]->LoadSpell(16, "HealingSpell-");
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[39][0] = 112;
            break;
        case 40:
            VObject3D[40] = new Sprite3D;
            VObject3D[40]->LoadSprite3D(6, "FireBall1");
            SoundFX[117].Create("FireBall 2", TS_MEMORY);
            Object3DSound[40][0] = 117;
            break;
        case 41:
            VObject3D[41] = new Spell;
            VObject3D[41]->LoadSpell(22, "GreenWipe-");
            SoundFX[113].Create("Mind Shield", TS_MEMORY);
            Object3DSound[41][0] = 113;
            break;
        case 42:
            VObject3D[42] = new Spell;
            VObject3D[42]->LoadSpell(19, "BlueWipe-");
            SoundFX[113].Create("Mind Shield", TS_MEMORY);
            Object3DSound[42][0] = 113;
            break;
        case 43:
            VObject3D[43] = new Spell;
            VObject3D[43]->LoadSpell(30, "FireWipe-");
            SoundFX[113].Create("Mind Shield", TS_MEMORY);
            Object3DSound[43][0] = 113;
            break;
        case 44:
            VObject3D[44] = new Spell;
            VObject3D[44]->LoadSpell(50, "Pentacle-");
            SoundFX[44].Create("Vampire Dying", TS_MEMORY);
            Object3DSound[44][0] = 44;
            break;
        case 45:
            VObject3D[45] = new Spell;
            VObject3D[45]->LoadSpell(26, "RedWipe-");
            SoundFX[113].Create("Mind Shield", TS_MEMORY);
            Object3DSound[45][0] = 113;
            break;
        case 46:
            VObject3D[46] = new Spell;
            VObject3D[46]->LoadSpell(26, "ShockWave-");
            SoundFX[115].Create("Stun Wave", TS_MEMORY);
            Object3DSound[46][0] = 115;
            break;
        case 47:
            VObject3D[47] = new Spell;
            VObject3D[47]->LoadSpell(11, "SmallExplosion-");
            SoundFX[114].Create("Explosion", TS_MEMORY);
            Object3DSound[47][0] = 114;
            break;
        case 48:
            VObject3D[48] = new Spell;
            VObject3D[48]->LoadSpell(33, "GreatExplosion-");
            SoundFX[114].Create("Explosion", TS_MEMORY);
            Object3DSound[48][0] = 114;
            break;
        case 49:
            VObject3D[49] = new Spell;
            VObject3D[49]->LoadSpell(26, "Flak2-");
            SoundFX[114].Create("Explosion", TS_MEMORY);
            Object3DSound[49][0] = 114;
            break;
        case 50:
            VObject3D[50] = new Spell;
            VObject3D[50]->LoadSpell(18, "Flak1-");
            SoundFX[114].Create("Explosion", TS_MEMORY);
            Object3DSound[50][0] = 114;
            break;
        case 51:
            VObject3D[51] = new Spell;
            VObject3D[51]->LoadSpell(29, "GreatBolt-");
            SoundFX[116].Create("Spark", TS_MEMORY);
            Object3DSound[51][0] = 116;
            break;
        case 52:
            VObject3D[52] = new Sprite3D;
            VObject3D[52]->LoadSprite3D(6, "FireBolt");
            SoundFX[65].Create("Small Projectile", TS_MEMORY);
            Object3DSound[52][0] = 65;
            break;
        case 53:
            VObject3D[53] = new Spell;
            VObject3D[53]->LoadSpell(21, "Curse-");
            SoundFX[2].Create("Curse", TS_MEMORY);
            Object3DSound[53][0] = 2;
            break;
        case 54:
            VObject3D[54] = new Spell;
            VObject3D[54]->LoadSpell(6, "ElectricShield-");
            SoundFX[7].Create("Electric Shield", TS_MEMORY);
            Object3DSound[54][0] = 7;
            break;
        case 55:
            VObject3D[55] = new Sprite3D;
            VObject3D[55]->LoadSprite3D(11, "Lightning");
            SoundFX[60].Create("Lightning", TS_MEMORY);
            Object3DSound[55][0] = 60;
            break;
        case 56:
            VObject3D[56] = new Sprite3D;
            VObject3D[56]->LoadSprite3D(6, "PoisonArrow");
            SoundFX[65].Create("Small Projectile", TS_MEMORY);
            Object3DSound[56][0] = 65;
            break;
        case 57:
            VObject3D[57] = new Sprite3D;
            VObject3D[57]->LoadSprite3D(7, "IceShard");
            SoundFX[65].Create("Small Projectile", TS_MEMORY);
            Object3DSound[57][0] = 65;
            break;
        case 58:
            VObject3D[58] = new Sprite3D;
            VObject3D[58]->LoadSprite3D(4, "StoneShard");
            SoundFX[65].Create("Small Projectile", TS_MEMORY);
            Object3DSound[58][0] = 65;
            break;
        case 59:
            VObject3D[59] = new Spell;
            VObject3D[59]->LoadSpell(46, "HealSerious-");
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[59][0] = 112;
            break;
        case 60:
            VObject3D[60] = new Spell;
            VObject3D[60]->LoadSpell(17, "SemiBigExplosion-");
            SoundFX[64].Create("SemiBig Explosion", TS_MEMORY);
            Object3DSound[60][0] = 64;
            break;
        case 61:
            VObject3D[61] = new Spell;
            VObject3D[61]->LoadSpell(30, "Freeze-");
            SoundFX[43].Create("Freeze", TS_MEMORY);
            Object3DSound[61][0] = 43;
            break;
        case 62:
            VObject3D[62] = new Spell;
            VObject3D[62]->LoadSpell(15, "RockyFly-");
            SoundFX[63].Create("Rocks Fly", TS_MEMORY);
            Object3DSound[62][0] = 63;
            break;
        case 63:
            VObject3D[63] = new Spell;
            VObject3D[63]->LoadSpell(17, "IceCloud-");
            SoundFX[59].Create("Ice Cloud", TS_MEMORY);
            Object3DSound[63][0] = 59;
            break;
        case 64:
            VObject3D[64] = new Spell;
            VObject3D[64]->LoadSpell(13, "SmallPoisonCloud-");
            SoundFX[59].Create("Ice Cloud", TS_MEMORY);
            Object3DSound[64][0] = 59;
            break;
        case 65:
            VObject3D[65] = new Sprite3D;
            VObject3D[65]->LoadSprite3D(8, 8, 0, 0, 12, "KraanianFlying", 2, 0, 0); 
            SoundFX[55].Create("Kraanian Dying", TS_MEMORY);
            SoundFX[56].Create("Kraanian Attack", TS_MEMORY);
            SoundFX[57].Create("Kraanian Hit", TS_MEMORY);
            Object3DSound[65][0] = 55;
            Object3DSound[65][1] = 56;
            Object3DSound[65][2] = 57;
            break;
        case 66:
            VObject3D[66] = new Sprite3D;
            VObject3D[66]->LoadSprite3D(9, 8, 0, 0, 12, "KraanianMilipede", 0, 0, 0); 
            SoundFX[48].Create("Atrocity Dying", TS_MEMORY);
            SoundFX[49].Create("Atrocity Attack", TS_MEMORY);
            SoundFX[50].Create("Atrocity Hit", TS_MEMORY);
            Object3DSound[66][0] = 48;
            Object3DSound[66][1] = 49;
            Object3DSound[66][2] = 50;
            break;
        case 67:
            VObject3D[67] = new Sprite3D;
            VObject3D[67]->LoadSprite3D(16, 0, 12, "Dromadary", 0, 0);
            SoundFX[66].Create("Mummy Dying", TS_MEMORY);
            SoundFX[67].Create("Mummy Attack", TS_MEMORY);
            SoundFX[68].Create("Mummy Hit", TS_MEMORY);
            Object3DSound[67][0] = 66;
            Object3DSound[67][1] = 67;
            Object3DSound[67][2] = 68;
            break;
        case 68:
            VObject3D[68] = new Sprite3D;
            VObject3D[68]->LoadSprite3D(9, 0, 26, "Pig", 0, 0);
            SoundFX[61].Create("Pig Dying", TS_MEMORY);
            SoundFX[62].Create("Pig Hit", TS_MEMORY);
            Object3DSound[68][0] = 61;
            Object3DSound[68][1] = 62;
            break;
        case 69:
            VObject3D[69] = new Sprite3D;
            VObject3D[69]->LoadSprite3D(8, 0, 12, "Ori", 0, 0);
            SoundFX[66].Create("Mummy Dying", TS_MEMORY);
            SoundFX[67].Create("Mummy Attack", TS_MEMORY);
            SoundFX[68].Create("Mummy Hit", TS_MEMORY);
            Object3DSound[69][0] = 66;
            Object3DSound[69][1] = 67;
            Object3DSound[69][2] = 68;
            break;
        case 70:
            VObject3D[70] = new Sprite3D;
            VObject3D[70]->LoadSprite3D(13, 13, 0, 0, 14, "Tarantula", 3, 0, 0); 
            SoundFX[72].Create("Spider Dying", TS_MEMORY);
            SoundFX[73].Create("Spider Attack", TS_MEMORY);
            SoundFX[74].Create("Spider Hit", TS_MEMORY);
            Object3DSound[70][0] = 72;
            Object3DSound[70][1] = 73;
            Object3DSound[70][2] = 74;
            break;
        case 71:
            VObject3D[71] = new Puppet;
            VObject3D[71]->LoadBodyPart();
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[20].Create("Bow Attack", TS_MEMORY);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            SoundFX[53].Create("Female Hit 1", TS_MEMORY);
            SoundFX[54].Create("Female Hit 2", TS_MEMORY);
            SoundFX[51].Create("Female Dying 1", TS_MEMORY);
            SoundFX[52].Create("Female Dying 2", TS_MEMORY);
            Object3DSound[71][0] = 4;
            Object3DSound[71][1] = 11;
            Object3DSound[71][2] = 12;
            Object3DSound[71][3] = 98;
            Object3DSound[71][4] = 99;
            Object3DSound[71][5] = 96;
            Object3DSound[71][6] = 97;
            Object3DSound[71][7] = 53;
            Object3DSound[71][8] = 54;
            Object3DSound[71][9] = 51;
            Object3DSound[71][10] = 52;
            break;
        case 72:
            VObject3D[72] = new Sprite3D;
            VObject3D[72]->LoadSprite3D(8, 8, 0, 0, 16, "Agmorkian", 0, 0, 0); 
            SoundFX[55].Create("Kraanian Dying", TS_MEMORY);
            SoundFX[56].Create("Kraanian Attack", TS_MEMORY);
            SoundFX[57].Create("Kraanian Hit", TS_MEMORY);
            Object3DSound[72][0] = 55;
            Object3DSound[72][1] = 56;
            Object3DSound[72][2] = 57;
            break;     
        case 73:
            VObject3D[73] = new Sprite3D;
            VObject3D[73]->LoadSprite3D(8, 8, 0, 0, 14, "Tank", 0, 0, 0); 
            SoundFX[48].Create("Atrocity Dying", TS_MEMORY);
            SoundFX[49].Create("Atrocity Attack", TS_MEMORY);
            SoundFX[50].Create("Atrocity Hit", TS_MEMORY);
            Object3DSound[73][0] = 48;
            Object3DSound[73][1] = 49;
            Object3DSound[73][2] = 50;
            break;
        case 74:
            VObject3D[74] = new Sprite3D;
            VObject3D[74]->LoadSprite3D(8, 8, 0, 0, 13, "Taunting", 0, 0, 0); 
            SoundFX[77].Create("Taunting Dying", TS_MEMORY);
            SoundFX[78].Create("Taunting Attack", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            Object3DSound[74][0] = 77;
            Object3DSound[74][1] = 78;
            Object3DSound[74][2] = 79;
            break;
        case 75:
            VObject3D[75] = new Sprite3D;
            VObject3D[75]->LoadSprite3D(8, 8, 0, 0, 11, "AtrocityBoss", 0, 0, 0);
            SoundFX[91].Create("Beast Dying", TS_MEMORY);
            SoundFX[92].Create("Beast Attack", TS_MEMORY);
            SoundFX[93].Create("Beast Hit", TS_MEMORY);
            Object3DSound[75][0] = 91;
            Object3DSound[75][1] = 92;
            Object3DSound[75][2] = 93;
            break;
        case 76:
            VObject3D[76] = new Sprite3D;
            VObject3D[76]->LoadSprite3D(12, 9, 0, 0, 12, "GoblinBoss", 0, 0, 0); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[76][0] = 11;
            Object3DSound[76][1] = 18;
            Object3DSound[76][2] = 33;
            Object3DSound[76][0] = 34;
            break;
        case 77:
            VObject3D[77] = new Sprite3D;
            VObject3D[77]->LoadSprite3D(9, 9, 0, 0, 14, "Wolf", 0, 0, 0);
            SoundFX[8].Create("Wolf Dying", TS_MEMORY);
            SoundFX[9].Create("Wolf Attack", TS_MEMORY);
            SoundFX[10].Create("Wolf Hit", TS_MEMORY);
            Object3DSound[77][0] = 8;
            Object3DSound[77][1] = 9;
            Object3DSound[77][2] = 10;
            break;
        case 78:
            VObject3D[78] = new Sprite3D;
            VObject3D[78]->LoadSprite3D(1, "64kArrowNormal");
            SoundFX[65].Create("Small Projectile", TS_MEMORY);
            Object3DSound[78][0] = 65; 
            break;
        case 79:
            VObject3D[79] = new Spell;
            VObject3D[79]->LoadSpell(36, "64kSpellBless-");
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[79][0] = 112;
            break;
        case 80:
            VObject3D[80] = new Spell;
            VObject3D[80]->LoadSpell(21, "64kSpellEntangle-");
            SoundFX[123].Create("Entangle", TS_MEMORY);
            Object3DSound[80][0] = 20;
            break;
        case 81:
            VObject3D[81] = new Spell;
            VObject3D[81]->LoadSpell(18, "64kSpellGlacier-");
            SoundFX[80].Create("Glacier", TS_MEMORY);
            Object3DSound[81][0] = 80;
            break;
        case 82:
            VObject3D[82] = new Spell;
            VObject3D[82]->LoadSpell(21, "64kSpellBoulders-");
            SoundFX[81].Create("Boulders", TS_MEMORY);
            Object3DSound[82][0] = 81;
            LoadObject(85);
            break;
        case 83:
            VObject3D[83] = new Spell;
            VObject3D[83]->LoadSpell(30, "64kSpellMeteor-");
            SoundFX[82].Create("Meteor", TS_MEMORY);
            Object3DSound[83][0] = 82;
            break;
        case 84:
            VObject3D[84] = new Spell;
            VObject3D[84]->LoadSpell(15, "64kSpellGateWay-");
            SoundFX[111].Create("Gateway", TS_MEMORY);
            Object3DSound[84][0] = 111;
            break;
        case 85:
            VObject3D[85] = new Spell;
            VObject3D[85]->LoadSpell(21, "64kSpellBouldersShadow-");
            SoundFX[81].Create("Boulders", TS_MEMORY);
            Object3DSound[85][0] = 81;
            break;
        case 86:
            VObject3D[86] = new Sprite3D;
            VObject3D[86]->LoadSprite3D(9, 9, 0, 0, 14, "Wolf", 0, 0, 0, 2);
            SoundFX[8].Create("Wolf Dying", TS_MEMORY);
            SoundFX[9].Create("Wolf Attack", TS_MEMORY);
            SoundFX[10].Create("Wolf Hit", TS_MEMORY);
            Object3DSound[86][0] = 8;
            Object3DSound[86][1] = 9;
            Object3DSound[86][2] = 10;
            break;
        case 87:
            VObject3D[87] = new Sprite3D;
            VObject3D[87]->LoadSprite3D(8, 0, 0, 0, 0, "64kArrowNormalBreak", 0, 0, 0, 0);
            SoundFX[64].Create("SemiBig Explosion", TS_MEMORY);
            Object3DSound[87][0] = 64;
            VObject3D[87]->Type = 3; // Consider this monster as an explosion spell.
            break;
        case 88:
            VObject3D[88] = new Spell;
            VObject3D[88]->LoadSpell(10, "64kSpellEnergyBall-");
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[88][0] = 112;
            break;
        case 89:
            VObject3D[89] = new Spell;
            VObject3D[89]->LoadSpell(10, "64kSpellEnergyBall-", 2);
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[89][0] = 112;
            break;
        case 90:
            VObject3D[90] = new Spell;
            VObject3D[90]->LoadSpell(10, "64kSpellEnergyBall-", 3);
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[90][0] = 112;
            break;
        case 91:
            VObject3D[91] = new Spell;
            VObject3D[91]->LoadSpell(10, "64kSpellEnergyBallGreen-");
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[91][0] = 112;
            break;
        case 92:
            VObject3D[92] = new Spell;
            VObject3D[92]->LoadSpell(10, "64kSpellEnergyBallWhite-");
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[92][0] = 112;
            break;
        case 93:
            VObject3D[93] = new Spell;
            VObject3D[93]->LoadSpell(10, "64kSpellEnergyBallWhite-", 2);
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[93][0] = 112;
            break;
        case 94:
            VObject3D[94] = new Spell;
            VObject3D[94]->LoadSpell(10, "64kSpellEnergyBall-", 4);
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[94][0] = 112;
            break;
        case 95:
            VObject3D[95] = new Spell;
            VObject3D[95]->LoadSpell(21, "64kSpellFireCircle-");
            SoundFX[122].Create("Fire circle", TS_MEMORY);
            Object3DSound[95][0] = 122;
            break;
        case 96:
            VObject3D[96] = new Sprite3D;
            VObject3D[96]->LoadSprite3D(6, "64kSpellFireBall");
            SoundFX[117].Create("FireBall 2", TS_MEMORY);
            Object3DSound[96][0] = 117;
            break;
        case 97:
            VObject3D[97] = new Sprite3D;
            VObject3D[97]->LoadSprite3D(9, 10, 0, 0, 20, "64kSkavenPeon", 0, 0, 0); 
            SoundFX[118].Create("Skraug attack", TS_MEMORY);
            SoundFX[119].Create("Skraug die", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            Object3DSound[97][0] = 118;
            Object3DSound[97][1] = 119;
            Object3DSound[97][2] = 79;
            break;
        case 98:
            VObject3D[98] = new Sprite3D;
            VObject3D[98]->LoadSprite3D(10, 10, 0, 0, 19, "64kSkavenShaman", 0, 0, 0); 
            SoundFX[118].Create("Skraug attack", TS_MEMORY);
            SoundFX[119].Create("Skraug die", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            Object3DSound[98][0] = 118;
            Object3DSound[98][1] = 119;
            Object3DSound[98][2] = 79;
            break;
        case 99:
            VObject3D[99] = new Sprite3D;
            VObject3D[99]->LoadSprite3D(9, 9, 0, 0, 19, "64kSkavenSkavenger", 0, 0, 0); 
            SoundFX[118].Create("Skraug attack", TS_MEMORY);
            SoundFX[119].Create("Skraug die", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            Object3DSound[99][0] = 118;
            Object3DSound[99][1] = 119;
            Object3DSound[99][2] = 79;
            break;
        case 100:
            VObject3D[100] = new Sprite3D;
            VObject3D[100]->LoadSprite3D(9, 11, 0, 0, 19, "64kSkavenWarrior", 0, 0, 0); 
            SoundFX[118].Create("Skraug attack", TS_MEMORY);
            SoundFX[119].Create("Skraug die", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            Object3DSound[100][0] = 118;
            Object3DSound[100][1] = 119;
            Object3DSound[100][2] = 79;
            break;
        case 101:
            VObject3D[101] = new Sprite3D;
            VObject3D[101]->LoadSprite3D(9, 9, 0, 0, 14, "64kCentaurWarrior", 0, 0, 0);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            SoundFX[95].Create("Centaur Hit", TS_MEMORY);
            Object3DSound[101][0] = 19;
            Object3DSound[101][1] = 79;
            Object3DSound[101][2] = 95;
            break;
        case 102:
            VObject3D[102] = new Sprite3D;
            VObject3D[102]->LoadSprite3D(9, 0, 1, "64kCentaurArcher", 0, 0);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            SoundFX[95].Create("Centaur Hit", TS_MEMORY);
            Object3DSound[102][0] = 19;
            Object3DSound[102][1] = 79;
            Object3DSound[102][2] = 95;
            break;
        case 103:
            VObject3D[103] = new Sprite3D;
            VObject3D[103]->LoadSprite3D(9, 0, 1, "64kCentaurShaman", 0, 0);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            SoundFX[95].Create("Centaur Hit", TS_MEMORY);
            Object3DSound[103][0] = 19;
            Object3DSound[103][1] = 79;
            Object3DSound[103][2] = 95;
            break;
        case 104:
            VObject3D[104] = new Sprite3D;
            VObject3D[104]->LoadSprite3D(9, 9, 0, 0, 14, "64kCentaurKing", 0, 0, 0);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            SoundFX[95].Create("Centaur Hit", TS_MEMORY);
            Object3DSound[104][0] = 19;
            Object3DSound[104][1] = 79;
            Object3DSound[104][2] = 95;
            break;
        case 105:
            VObject3D[105] = new Sprite3D;
            VObject3D[105]->LoadSprite3D(13, 13, 0, 0, 16, "64kSkeletonServant1", 0, 0, 0); 
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[38].Create("Skeleton Dying", TS_MEMORY);
            SoundFX[39].Create("Skeleton Hit", TS_MEMORY);
            Object3DSound[105][0] = 4;
            Object3DSound[105][1] = 12;
            Object3DSound[105][2] = 18;
            Object3DSound[105][4] = 38;
            Object3DSound[105][5] = 39;
            break;
        case 106:
            VObject3D[106] = new Sprite3D;
            VObject3D[106]->LoadSprite3D(13, 11, 0, 0, 16, "64kSkeletonServant2", 0, 0, 0); 
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[38].Create("Skeleton Dying", TS_MEMORY);
            SoundFX[39].Create("Skeleton Hit", TS_MEMORY);
            Object3DSound[106][0] = 4;
            Object3DSound[106][1] = 12;
            Object3DSound[106][2] = 18;
            Object3DSound[106][4] = 38;
            Object3DSound[106][5] = 39;
            break;
        case 107:
            VObject3D[107] = new Sprite3D;
            VObject3D[107]->LoadSprite3D(13, 11, 0, 0, 16, "64kSkeletonKing", 0, 0, 0); 
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[38].Create("Skeleton Dying", TS_MEMORY);
            SoundFX[39].Create("Skeleton Hit", TS_MEMORY);
            Object3DSound[107][0] = 4;
            Object3DSound[107][1] = 12;
            Object3DSound[107][2] = 18;
            Object3DSound[107][4] = 38;
            Object3DSound[107][5] = 39;
            break;
        case 108:
            VObject3D[108] = new Sprite3D;
            VObject3D[108]->LoadSprite3D(12, 10, 0, 0, 23, "64kLich", 0, 0, 0); 
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[44].Create("Vampire Dying", TS_MEMORY);
            SoundFX[120].Create("Vampire Hit", TS_MEMORY);
            Object3DSound[108][0] = 12;
            Object3DSound[108][1] = 44;
            Object3DSound[108][2] = 120;
            break;
        case 109:
            VObject3D[109] = new Spell;
            VObject3D[109]->LoadSpell(91, "64kSpellSeraphArrival-");
            SoundFX[121].Create("Seraph", TS_MEMORY);
            Object3DSound[109][0] = 121;
            break;
        case 110:
            VObject3D[110] = new Sprite3D;
            VObject3D[110]->LoadSprite3D(9, 10, 0, 0, 20, "64kSkavenPeon", 0, 0, 0, 2); 
            SoundFX[118].Create("Skraug attack", TS_MEMORY);
            SoundFX[119].Create("Skraug die", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            Object3DSound[110][0] = 118;
            Object3DSound[110][1] = 119;
            Object3DSound[110][2] = 79;
            break;
        case 111:
            VObject3D[111] = new Sprite3D;
            VObject3D[111]->LoadSprite3D(10, 10, 0, 0, 19, "64kSkavenShaman", 0, 0, 0, 2); 
            SoundFX[118].Create("Skraug attack", TS_MEMORY);
            SoundFX[119].Create("Skraug die", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            Object3DSound[111][0] = 118;
            Object3DSound[111][1] = 119;
            Object3DSound[111][2] = 79;
            break;
        case 112:
            VObject3D[112] = new Sprite3D;
            VObject3D[112]->LoadSprite3D(9, 9, 0, 0, 19, "64kSkavenSkavenger", 0, 0, 0, 2); 
            SoundFX[118].Create("Skraug attack", TS_MEMORY);
            SoundFX[119].Create("Skraug die", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            Object3DSound[112][0] = 118;
            Object3DSound[112][1] = 119;
            Object3DSound[112][2] = 79;
            break;
        case 113:
            VObject3D[113] = new Sprite3D;
            VObject3D[113]->LoadSprite3D(9, 11, 0, 0, 19, "64kSkavenWarrior", 0, 0, 0, 2); 
            SoundFX[118].Create("Skraug attack", TS_MEMORY);
            SoundFX[119].Create("Skraug die", TS_MEMORY);
            SoundFX[79].Create("Taunting Hit", TS_MEMORY);
            Object3DSound[113][0] = 118;
            Object3DSound[113][1] = 119;
            Object3DSound[113][2] = 79;
            break;
        case 114:
            VObject3D[114] = new Sprite3D;
            VObject3D[114]->LoadSprite3D(9, 9, 0, 0, 14, "64kCentaurSkeleton", 0, 0, 0); 
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[38].Create("Skeleton Dying", TS_MEMORY);
            SoundFX[39].Create("Skeleton Hit", TS_MEMORY);
            Object3DSound[114][0] = 4;
            Object3DSound[114][1] = 12;
            Object3DSound[114][2] = 18;
            Object3DSound[114][4] = 38;
            Object3DSound[114][5] = 39;
            break;
        case 115:
            VObject3D[115] = new Spell;
            VObject3D[115]->LoadSpell(93, "64kSeraphArivalBlack-");
            SoundFX[124].Create("Evil Seraph", TS_MEMORY);
            Object3DSound[115][0] = 124;
            break;
        case 116:
            VObject3D[116] = new Sprite3D;
            VObject3D[116]->LoadSprite3D2(12, 0, 0, 0, 0, "PupSanta", 0, 0, 0);
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[98].Create("Male Hit 1", TS_MEMORY);
            SoundFX[99].Create("Male Hit 2", TS_MEMORY);
            SoundFX[96].Create("Male Dying 1", TS_MEMORY);
            SoundFX[97].Create("Male Dying 2", TS_MEMORY);
            Object3DSound[11][0] = 4;
            Object3DSound[11][1] = 11;
            Object3DSound[11][2] = 12;
            Object3DSound[11][3] = 98;
            Object3DSound[11][4] = 99;
            Object3DSound[11][5] = 96;
            Object3DSound[11][6] = 97;
            break;             
        case 117:
            VObject3D[117] = new Sprite3D;
            VObject3D[117]->LoadSprite3D2(11, 11, 0, 0, 12, "MonsDraconianPlate", 0, 0, 0); 
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);		
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[117][0] = 12;
            Object3DSound[117][1] = 19;
            Object3DSound[117][2] = 37;
            Object3DSound[117][3] = 39;
            break;
        case 118:
            VObject3D[118] = new Sprite3D;
            VObject3D[118]->LoadSprite3D2(11, 11, 0, 0, 12, "MonsDraconianRobe", 0, 0, 0); 
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[118][0] = 12;
            Object3DSound[118][1] = 19;
            Object3DSound[118][2] = 37;
            Object3DSound[118][3] = 39;
            break;
        case 119:
            VObject3D[119] = new Sprite3D;
            VObject3D[119]->LoadSprite3D2(11, 11, 0, 0, 12, "MonsDraconianLeather", 0, 0, 0); 
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[19].Create("Whooshm 7", TS_MEMORY);
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[119][0] = 12;
            Object3DSound[119][1] = 19;
            Object3DSound[119][2] = 37;
            Object3DSound[119][3] = 39;
            break;

		// New added by Dialsoft - BEGIN
		case 120:
            VObject3D[120] = new Sprite3D;
            VObject3D[120]->LoadSprite3D2(12, 9, 0, 0, 11, "DG", 0, 0, 0); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[120][0] = 11;
            Object3DSound[120][1] = 18;
            Object3DSound[120][2] = 33;
            Object3DSound[120][3] = 34;
            break;

		case 121:
            VObject3D[121] = new Spell;
            VObject3D[121]->LoadSpell(26, "SpellMediumHealing-");
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[121][0] = 112;
            break;

		case 122:
            VObject3D[122] = new Spell;
            VObject3D[122]->LoadSpell(20, "iceTree-");
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[122][0] = 112;
            break;
		case 123:
            VObject3D[123] = new Sprite3D;
            VObject3D[123]->LoadSprite3D(9, 9, 0, 0, 14, "BT", 0, 0, 0); 
            SoundFX[75].Create("Tree Ent Dying", TS_MEMORY);
            SoundFX[76].Create("Electrik", TS_MEMORY);
            SoundFX[90].Create("AxeWood", TS_MEMORY);
            Object3DSound[123][0] = 75;
            Object3DSound[123][1] = 76;
            Object3DSound[123][2] = 90;
            Object3DSound[123][3] = 76;
            break;
		case 124:
            VObject3D[124] = new Sprite3D;
            VObject3D[124]->LoadSprite3D(9, 9, 0, 0, 10, "GroundMummy", 0, 0, 0); 
            SoundFX[66].Create("Mummy Dying", TS_MEMORY);
            SoundFX[67].Create("Mummy Attack", TS_MEMORY);
            SoundFX[68].Create("Mummy Hit", TS_MEMORY);
            Object3DSound[124][0] = 66;
            Object3DSound[124][1] = 67;
            Object3DSound[124][2] = 68;
            break;
		case 125:
            VObject3D[125] = new Sprite3D;
            VObject3D[125]->LoadSprite3D(6, 9, 0, 0, 9, "BossRat", 0, 0, 0); 
	        SoundFX[35].Create("Rat Dying", TS_MEMORY);
            SoundFX[36].Create("Rat Attack", TS_MEMORY);
            SoundFX[37].Create("Rat Hit", TS_MEMORY);
            Object3DSound[125][0] = 35;
            Object3DSound[125][1] = 36;
            Object3DSound[125][2] = 37;
			break;
		case 126:
            VObject3D[126] = new Sprite3D;
            VObject3D[126]->LoadSprite3D(13, 13, 0, 0, 9, "BigSkel", 0, 0, 0); 
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[38].Create("Skeleton Dying", TS_MEMORY);
            SoundFX[39].Create("Skeleton Hit", TS_MEMORY);
            Object3DSound[126][0] = 4;
            Object3DSound[126][1] = 12;
            Object3DSound[126][2] = 18;
            Object3DSound[126][4] = 38;
            Object3DSound[126][5] = 39;
            break;
		case 127:
			VObject3D[127] = new Spell;
            VObject3D[127]->LoadSpell(58, "SupraHeal-");
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[127][0] = 112;
            break;
		case 128:
            VObject3D[128] = new Sprite3D;
            VObject3D[128]->LoadSprite3D(9, 9, 0, 0, 11, "Demon", 0, 0, 0, 2); 
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[128][0] = 27;
            Object3DSound[128][1] = 28;
            Object3DSound[128][2] = 29;
            break;
		case 129:
            VObject3D[129] = new Sprite3D;
            VObject3D[129]->LoadSprite3D(9, 9, 0, 0, 11, "Demon", 0, 0, 0, 3); 
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[129][0] = 27;
            Object3DSound[129][1] = 28;
            Object3DSound[129][2] = 29;
            break;
		case 130:
            VObject3D[130] = new Sprite3D;
            VObject3D[130]->LoadSprite3D(9, 9, 0, 0, 11, "Demon", 0, 0, 0, 4); 
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[130][0] = 27;
            Object3DSound[130][1] = 28;
            Object3DSound[130][2] = 29;
            break;
		case 131:
            VObject3D[131] = new Sprite3D;
            VObject3D[131]->LoadSprite3D(9, 9, 0, 0, 11, "Demon", 0, 0, 0, 5); 
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[131][0] = 27;
            Object3DSound[131][1] = 28;
            Object3DSound[131][2] = 29;
            break;
		case 132:
            VObject3D[132] = new Sprite3D;
            VObject3D[132]->LoadSprite3D(9, 9, 0, 0, 11, "Demon", 0, 0, 0, 6); 
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[132][0] = 27;
            Object3DSound[132][1] = 28;
            Object3DSound[132][2] = 29;
            break;
		case 133:
            VObject3D[133] = new Sprite3D;
            VObject3D[133]->LoadSprite3D(9, 9, 0, 0, 11, "Demon", 0, 0, 0, 7); 
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[133][0] = 27;
            Object3DSound[133][1] = 28;
            Object3DSound[133][2] = 29;
            break;
		case 134:
            VObject3D[134] = new Sprite3D;
            VObject3D[134]->LoadSprite3D(9, 9, 0, 0, 11, "Demon", 0, 0, 0, 8); 
            SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[134][0] = 27;
            Object3DSound[134][1] = 28;
            Object3DSound[134][2] = 29;
            break;
		case 135:
            VObject3D[135] = new Sprite3D;
            VObject3D[135]->LoadSprite3D(12, 9, 0, 0, 15, "Goblin", 0, 0, 0, 2); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[135][0] = 11;
            Object3DSound[135][1] = 18;
            Object3DSound[135][2] = 33;
            Object3DSound[135][3] = 34;
            break;
		case 136:
            VObject3D[136] = new Sprite3D;
            VObject3D[136]->LoadSprite3D(12, 9, 0, 0, 15, "Goblin", 0, 0, 0, 3); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[136][0] = 11;
            Object3DSound[136][1] = 18;
            Object3DSound[136][2] = 33;
            Object3DSound[136][3] = 34;
            break;
		case 137:
            VObject3D[137] = new Sprite3D;
            VObject3D[137]->LoadSprite3D(12, 9, 0, 0, 15, "Goblin", 0, 0, 0, 4); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[137][0] = 11;
            Object3DSound[137][1] = 18;
            Object3DSound[137][2] = 33;
            Object3DSound[137][3] = 34;
            break;
		case 138:
            VObject3D[138] = new Sprite3D;
            VObject3D[138]->LoadSprite3D(12, 9, 0, 0, 15, "Goblin", 0, 0, 0, 5); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[138][0] = 11;
            Object3DSound[138][1] = 18;
            Object3DSound[138][2] = 33;
            Object3DSound[138][3] = 34;
            break;
		case 139:
            VObject3D[139] = new Sprite3D;
            VObject3D[139]->LoadSprite3D(12, 9, 0, 0, 15, "Goblin", 0, 0, 0, 6); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[139][0] = 11;
            Object3DSound[139][1] = 18;
            Object3DSound[139][2] = 33;
            Object3DSound[139][3] = 34;
            break;
		case 140:
            VObject3D[140] = new Sprite3D;
            VObject3D[140]->LoadSprite3D(12, 9, 0, 0, 15, "Goblin", 0, 0, 0, 7); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[140][0] = 11;
            Object3DSound[140][1] = 18;
            Object3DSound[140][2] = 33;
            Object3DSound[140][3] = 34;
            break;
		case 141:
			VObject3D[141] = new Sprite3D;
            VObject3D[141]->LoadSprite3D(12, 9, 0, 0, 15, "Goblin", 0, 0, 0, 8); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[141][0] = 11;
            Object3DSound[141][1] = 18;
            Object3DSound[141][2] = 33;
            Object3DSound[141][3] = 34;
            break;
		case 142:
            VObject3D[142] = new Sprite3D;
            VObject3D[142]->LoadSpriteC(23, 35, 26, "Slime", 2);
            SoundFX[30].Create("Ooze Dying", TS_MEMORY);
            SoundFX[31].Create("Ooze Attack", TS_MEMORY);
            SoundFX[32].Create("Ooze Hit", TS_MEMORY);
            Object3DSound[142][0] = 30;
            Object3DSound[142][1] = 31;
            Object3DSound[142][2] = 32;
            break;
		case 143:
            VObject3D[143] = new Sprite3D;
            VObject3D[143]->LoadSpriteC(23, 35, 26, "Slime", 3);
            SoundFX[30].Create("Ooze Dying", TS_MEMORY);
            SoundFX[31].Create("Ooze Attack", TS_MEMORY);
            SoundFX[32].Create("Ooze Hit", TS_MEMORY);
            Object3DSound[143][0] = 30;
            Object3DSound[143][1] = 31;
            Object3DSound[143][2] = 32;
            break;
		case 144:
            VObject3D[144] = new Sprite3D;
            VObject3D[144]->LoadSpriteC(23, 35, 26, "Slime", 4);
            SoundFX[30].Create("Ooze Dying", TS_MEMORY);
            SoundFX[31].Create("Ooze Attack", TS_MEMORY);
            SoundFX[32].Create("Ooze Hit", TS_MEMORY);
            Object3DSound[144][0] = 30;
            Object3DSound[144][1] = 31;
            Object3DSound[144][2] = 32;
            break;
		case 145:
            VObject3D[145] = new Sprite3D;
            VObject3D[145]->LoadSpriteC(23, 35, 26, "Slime", 5);
            SoundFX[30].Create("Ooze Dying", TS_MEMORY);
            SoundFX[31].Create("Ooze Attack", TS_MEMORY);
            SoundFX[32].Create("Ooze Hit", TS_MEMORY);
            Object3DSound[145][0] = 30;
            Object3DSound[145][1] = 31;
            Object3DSound[145][2] = 32;
            break;
		case 146:
            VObject3D[146] = new Sprite3D;
            VObject3D[146]->LoadSpriteC(23, 35, 26, "Slime", 6);
            SoundFX[30].Create("Ooze Dying", TS_MEMORY);
            SoundFX[31].Create("Ooze Attack", TS_MEMORY);
            SoundFX[32].Create("Ooze Hit", TS_MEMORY);
            Object3DSound[146][0] = 30;
            Object3DSound[146][1] = 31;
            Object3DSound[146][2] = 32;
            break;
		case 147:
            VObject3D[147] = new Sprite3D;
            VObject3D[147]->LoadSpriteC(23, 35, 26, "Slime", 7);
            SoundFX[30].Create("Ooze Dying", TS_MEMORY);
            SoundFX[31].Create("Ooze Attack", TS_MEMORY);
            SoundFX[32].Create("Ooze Hit", TS_MEMORY);
            Object3DSound[147][0] = 30;
            Object3DSound[147][1] = 31;
            Object3DSound[147][2] = 32;
            break;
		case 148:
            VObject3D[148] = new Sprite3D;
            VObject3D[148]->LoadSpriteC(23, 35, 26, "Slime", 8);
            SoundFX[30].Create("Ooze Dying", TS_MEMORY);
            SoundFX[31].Create("Ooze Attack", TS_MEMORY);
            SoundFX[32].Create("Ooze Hit", TS_MEMORY);
            Object3DSound[148][0] = 30;
            Object3DSound[148][1] = 31;
            Object3DSound[148][2] = 32;
            break;
		case 149:
            VObject3D[149] = new Sprite3D;
			VObject3D[149]->LoadSprite3D(5, 5, 0, 0, 11, "Whitecat", 0, 0, 0); 
            SoundFX[8].Create("Wolf Dying", TS_MEMORY);
            SoundFX[9].Create("Wolf Attack", TS_MEMORY);
            SoundFX[10].Create("Wolf Hit", TS_MEMORY);
            Object3DSound[149][0] = 8;
            Object3DSound[149][1] = 9;
            Object3DSound[149][2] = 10;
            break;
		case 150:
            VObject3D[150] = new Sprite3D;
            VObject3D[150]->LoadSprite3D(10, 8, 0, 0, 9, "dragonDarkGreen", 0, 0, 0); 
			   SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[150][0] = 27;
            Object3DSound[150][1] = 28;
            Object3DSound[150][2] = 29;            
			break;
		case 151:
            VObject3D[151] = new Sprite3D;
            VObject3D[151]->LoadSprite3D(13, 9, 0, 0, 0, "fgamemaster", 0, 0, 0); 
			break;
		//New added by Dialsoft - END
         // BEGIN DIALSOFT SPELLS NEW
      case 152:
         VObject3D[152] = new Sprite3D;
         VObject3D[152]->LoadSprite3D(6, "64kSpellFireBall", 2);
         SoundFX[117].Create("FireBall 2", TS_MEMORY);
         Object3DSound[152][0] = 117;
         break;
      case 153:
         VObject3D[153] = new Sprite3D;
         VObject3D[153]->LoadSprite3D(6, "64kSpellFireBall", 3);
         SoundFX[117].Create("FireBall 2", TS_MEMORY);
         Object3DSound[153][0] = 117;
         break;
      case 154:
         VObject3D[154] = new Sprite3D;
         VObject3D[154]->LoadSprite3D(6, "64kSpellFireBall", 4);
         SoundFX[117].Create("FireBall 2", TS_MEMORY);
         Object3DSound[154][0] = 117;
         break;
      case 155:
         VObject3D[155] = new Sprite3D;
         VObject3D[155]->LoadSprite3D(6, "64kSpellFireBall", 5);
         SoundFX[117].Create("FireBall 2", TS_MEMORY);
         Object3DSound[155][0] = 117;
         break;
      case 156:
         VObject3D[156] = new Sprite3D;
         VObject3D[156]->LoadSprite3D(6, "64kSpellFireBall", 6);
         SoundFX[117].Create("FireBall 2", TS_MEMORY);
         Object3DSound[156][0] = 117;
         break;
      case 157:
         VObject3D[157] = new Sprite3D;
         VObject3D[157]->LoadSprite3D(6, "64kSpellFireBall", 7);
         SoundFX[117].Create("FireBall 2", TS_MEMORY);
         Object3DSound[157][0] = 117;
         break;
      case 158:
         VObject3D[158] = new Sprite3D;
         VObject3D[158]->LoadSprite3D(6, "FireBolt", 2);
         SoundFX[65].Create("Small Projectile", TS_MEMORY);
         Object3DSound[158][0] = 65;
         break;
      case 159:
         VObject3D[159] = new Sprite3D;
         VObject3D[159]->LoadSprite3D(6, "FireBolt", 3);
         SoundFX[65].Create("Small Projectile", TS_MEMORY);
         Object3DSound[159][0] = 65;
         break;
      case 160:
         VObject3D[160] = new Sprite3D;
         VObject3D[160]->LoadSprite3D(6, "FireBolt", 4);
         SoundFX[65].Create("Small Projectile", TS_MEMORY);
         Object3DSound[160][0] = 65;
         break;
      case 161:
         VObject3D[161] = new Sprite3D;
         VObject3D[161]->LoadSprite3D(6, "FireBolt", 5);
         SoundFX[65].Create("Small Projectile", TS_MEMORY);
         Object3DSound[161][0] = 65;
         break;
      case 162:
         VObject3D[162] = new Sprite3D;
         VObject3D[162]->LoadSprite3D(6, "FireBolt", 6);
         SoundFX[65].Create("Small Projectile", TS_MEMORY);
         Object3DSound[162][0] = 65;
         break;
      case 163:
         VObject3D[163] = new Sprite3D;
         VObject3D[163]->LoadSprite3D(6, "FireBolt", 7);
         SoundFX[65].Create("Small Projectile", TS_MEMORY);
         Object3DSound[163][0] = 65;
         break;
      case 164:
         VObject3D[164] = new Spell;
         VObject3D[164]->LoadSpell(30, "64kSpellMeteor-", 2);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[164][0] = 82;
         break;
      case 165:
         VObject3D[165] = new Spell;
         VObject3D[165]->LoadSpell(30, "64kSpellMeteor-", 3);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[165][0] = 82;
         break;
      case 166:
         VObject3D[166] = new Spell;
         VObject3D[166]->LoadSpell(30, "64kSpellMeteor-", 4);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[166][0] = 82;
         break;
      case 167:
         VObject3D[167] = new Spell;
         VObject3D[167]->LoadSpell(30, "64kSpellMeteor-", 5);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[167][0] = 82;
         break;
      case 168:
         VObject3D[168] = new Spell;
         VObject3D[168]->LoadSpell(30, "64kSpellMeteor-", 6);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[168][0] = 82;
         break;
      case 169:
         VObject3D[169] = new Spell;
         VObject3D[169]->LoadSpell(30, "64kSpellMeteor-", 7);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[169][0] = 82;
         break;
      case 170:
         VObject3D[170] = new Spell;
         VObject3D[170]->LoadSpell(30, "64kSpellMeteor-", 8);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[170][0] = 82;
         break;
      case 171:
         VObject3D[171] = new Spell;
         VObject3D[171]->LoadSpell(30, "64kSpellMeteor-", 9);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[171][0] = 82;
         break;
      case 172:
         VObject3D[172] = new Spell;
         VObject3D[172]->LoadSpell(30, "64kSpellMeteor-", 10);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[172][0] = 82;
         break;
      case 173:
         VObject3D[173] = new Spell;
         VObject3D[173]->LoadSpell(30, "64kSpellMeteor-", 11);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[173][0] = 82;
         break;
      case 174:
         VObject3D[174] = new Spell;
         VObject3D[174]->LoadSpell(30, "64kSpellMeteor-", 12);
         SoundFX[82].Create("Meteor", TS_MEMORY);
         Object3DSound[174][0] = 82;
         break;
      case 175:
         VObject3D[175] = new Spell;
         VObject3D[175]->LoadSpell(29, "GreatBolt-", 2);
         SoundFX[116].Create("Spark", TS_MEMORY);
         Object3DSound[175][0] = 116;
         break;
      case 176:
         VObject3D[176] = new Spell;
         VObject3D[176]->LoadSpell(29, "GreatBolt-", 3);
         SoundFX[116].Create("Spark", TS_MEMORY);
         Object3DSound[176][0] = 116;
         break;
      case 177:
         VObject3D[177] = new Spell;
         VObject3D[177]->LoadSpell(29, "GreatBolt-", 4);
         SoundFX[116].Create("Spark", TS_MEMORY);
         Object3DSound[177][0] = 116;
         break;
      case 178:
         VObject3D[178] = new Spell;
         VObject3D[178]->LoadSpell(29, "GreatBolt-", 5);
         SoundFX[116].Create("Spark", TS_MEMORY);
         Object3DSound[178][0] = 116;
         break;
      case 179:
         VObject3D[179] = new Spell;
         VObject3D[179]->LoadSpell(29, "GreatBolt-", 6);
         SoundFX[116].Create("Spark", TS_MEMORY);
         Object3DSound[179][0] = 116;
         break;
      case 180:
         VObject3D[180] = new Spell;
         VObject3D[180]->LoadSpell(29, "GreatBolt-", 7);
         SoundFX[116].Create("Spark", TS_MEMORY);
         Object3DSound[180][0] = 116;
         break;
      case 181:
         VObject3D[181] = new Spell;
         VObject3D[181]->LoadSpell(29, "GreatBolt-", 8);
         SoundFX[116].Create("Spark", TS_MEMORY);
         Object3DSound[181][0] = 116;
         break;
      case 182:
         VObject3D[182] = new Spell;
         VObject3D[182]->LoadSpell(29, "GreatBolt-", 9);
         SoundFX[116].Create("Spark", TS_MEMORY);
         Object3DSound[182][0] = 116;
         break;
         // END DIALSOFT SPELLS NEW
      case 183:  
            VObject3D[183] = new Sprite3D;
            VObject3D[183]->LoadSprite3D(10, 10, 0, 0, 11, "Yeti", 0, 0, 0); 
			   SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[183][0] = 27;
            Object3DSound[183][1] = 28;
            Object3DSound[183][2] = 29;            
			break;
      case 184:  
            VObject3D[184] = new Sprite3D;
            VObject3D[184]->LoadSprite3D(10, 10, 0, 0, 11, "sYeti", 0, 0, 0); 
			   SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[184][0] = 27;
            Object3DSound[184][1] = 28;
            Object3DSound[184][2] = 29;            
			break;
      case 185:
         VObject3D[185] = new Spell;
         VObject3D[185]->LoadSpell(17, "SkillSpell-", 1);
         SoundFX[59].Create("Ice Cloud", TS_MEMORY);
         Object3DSound[185][0] = 59;
         break;
         
      case 186:
         VObject3D[186] = new Spell;
         VObject3D[186]->LoadSpell(41, "Levelup-", 1);
         SoundFX[112].Create("Healing", TS_MEMORY);
         Object3DSound[186][0] = 112;
         break;
         
      case 187:  
         VObject3D[187] = new Sprite3D;
         VObject3D[187]->LoadSprite3D(10, 10, 0, 0, 16, "Blob", 0, 0, 0); 
         SoundFX[30].Create("Ooze Dying", TS_MEMORY);
         SoundFX[31].Create("Ooze Attack", TS_MEMORY);
         SoundFX[32].Create("Ooze Hit", TS_MEMORY);
         Object3DSound[187][0] = 30;
         Object3DSound[187][1] = 31;
         Object3DSound[187][2] = 32;          
         break;
      case 188:  
         VObject3D[188] = new Sprite3D;
         VObject3D[188]->LoadSprite3D(10, 10, 0, 0, 16, "Blob", 0, 0, 0,2); 
         SoundFX[30].Create("Ooze Dying", TS_MEMORY);
         SoundFX[31].Create("Ooze Attack", TS_MEMORY);
         SoundFX[32].Create("Ooze Hit", TS_MEMORY);
         Object3DSound[188][0] = 30;
         Object3DSound[188][1] = 31;
         Object3DSound[188][2] = 32;          
         break;
      case 189:
            VObject3D[189] = new Sprite3D;
            VObject3D[189]->LoadSprite3D(13, 13, 0, 0, 14, "Tarantula", 3, 0, 0,2); 
            SoundFX[72].Create("Spider Dying", TS_MEMORY);
            SoundFX[73].Create("Spider Attack", TS_MEMORY);
            SoundFX[74].Create("Spider Hit", TS_MEMORY);
            Object3DSound[189][0] = 72;
            Object3DSound[189][1] = 73;
            Object3DSound[189][2] = 74;
            break;
      case 190:
            VObject3D[190] = new Sprite3D;
            VObject3D[190]->LoadSprite3D2(12, 9, 0, 0, 11, "DG2", 0, 0, 0); 
            SoundFX[11].Create("Whooshh 2", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[33].Create("Goblin Dying", TS_MEMORY);
            SoundFX[34].Create("Goblin Hit", TS_MEMORY);
            Object3DSound[190][0] = 11;
            Object3DSound[190][1] = 18;
            Object3DSound[190][2] = 33;
            Object3DSound[190][3] = 34;
            break;
      case 191:
            VObject3D[191] = new Sprite3D;
            VObject3D[191]->LoadSprite3D(8, 8, 0, 0, 12, "Kraanian", 0, 0, 0,2);
            SoundFX[48].Create("Atrocity Dying", TS_MEMORY);
            SoundFX[49].Create("Atrocity Attack", TS_MEMORY);
            SoundFX[50].Create("Atrocity Hit", TS_MEMORY);
            Object3DSound[191][0] = 48;
            Object3DSound[191][1] = 49;
            Object3DSound[191][2] = 50;
            break;
      case 192:
            VObject3D[192] = new Sprite3D;
            VObject3D[192]->LoadSprite3D(8, 8, 0, 0, 12, "KraanianFlying", 2, 0, 0,2); 
            SoundFX[55].Create("Kraanian Dying", TS_MEMORY);
            SoundFX[56].Create("Kraanian Attack", TS_MEMORY);
            SoundFX[57].Create("Kraanian Hit", TS_MEMORY);
            Object3DSound[192][0] = 55;
            Object3DSound[192][1] = 56;
            Object3DSound[192][2] = 57;
            break;
      case 193:
            VObject3D[193] = new Sprite3D;
            VObject3D[193]->LoadSprite3D(8, 8, 0, 0, 14, "Tank", 0, 0, 0,2); 
            SoundFX[48].Create("Atrocity Dying", TS_MEMORY);
            SoundFX[49].Create("Atrocity Attack", TS_MEMORY);
            SoundFX[50].Create("Atrocity Hit", TS_MEMORY);
            Object3DSound[193][0] = 48;
            Object3DSound[193][1] = 49;
            Object3DSound[193][2] = 50;
            break;
      case 194:
            VObject3D[194] = new Sprite3D;
            VObject3D[194]->LoadSprite3D(7, 9, 0, 0, 11, "Orc", 0, 0, 0,2);
            SoundFX[103].Create("Whooshm 8", TS_MEMORY);
            SoundFX[105].Create("Orc Dying", TS_MEMORY);
            SoundFX[106].Create("Orc Hit", TS_MEMORY);
            Object3DSound[194][0] = 103;
            Object3DSound[194][1] = 105;
            Object3DSound[194][2] = 106;
            break;
      case 195:
            VObject3D[195] = new Sprite3D;
            VObject3D[195]->LoadSprite3D(1, 1, 0, 0, 8, "IceKraanianPod", 0, 0, 0); 
            SoundFX[48].Create("Atrocity Dying", TS_MEMORY);
            SoundFX[49].Create("Atrocity Attack", TS_MEMORY);
            SoundFX[50].Create("Atrocity Hit", TS_MEMORY);
            Object3DSound[195][0] = 48;
            Object3DSound[195][1] = 49;
            Object3DSound[195][2] = 50;
            break;

      case 196:
         VObject3D[196] = new Sprite3D;
         VObject3D[196]->LoadSprite3D(9, 10, 0, 0, 20, "64kSkavenPeon", 0, 0, 0, 3); 
         SoundFX[118].Create("Skraug attack", TS_MEMORY);
         SoundFX[119].Create("Skraug die", TS_MEMORY);
         SoundFX[79].Create("Taunting Hit", TS_MEMORY);
         Object3DSound[196][0] = 118;
         Object3DSound[196][1] = 119;
         Object3DSound[196][2] = 79;
         break;
      case 197:
         VObject3D[197] = new Sprite3D;
         VObject3D[197]->LoadSprite3D(10, 10, 0, 0, 19, "64kSkavenShaman", 0, 0, 0, 3); 
         SoundFX[118].Create("Skraug attack", TS_MEMORY);
         SoundFX[119].Create("Skraug die", TS_MEMORY);
         SoundFX[79].Create("Taunting Hit", TS_MEMORY);
         Object3DSound[197][0] = 118;
         Object3DSound[197][1] = 119;
         Object3DSound[197][2] = 79;
         break;
      case 198:
         VObject3D[198] = new Sprite3D;
         VObject3D[198]->LoadSprite3D(9, 9, 0, 0, 19, "64kSkavenSkavenger", 0, 0, 0, 3); 
         SoundFX[118].Create("Skraug attack", TS_MEMORY);
         SoundFX[119].Create("Skraug die", TS_MEMORY);
         SoundFX[79].Create("Taunting Hit", TS_MEMORY);
         Object3DSound[198][0] = 118;
         Object3DSound[198][1] = 119;
         Object3DSound[198][2] = 79;
         break;
      case 199:
         VObject3D[199] = new Sprite3D;
         VObject3D[199]->LoadSprite3D(9, 11, 0, 0, 19, "64kSkavenWarrior", 0, 0, 0, 3); 
         SoundFX[118].Create("Skraug attack", TS_MEMORY);
         SoundFX[119].Create("Skraug die", TS_MEMORY);
         SoundFX[79].Create("Taunting Hit", TS_MEMORY);
         Object3DSound[199][0] = 118;
         Object3DSound[199][1] = 119;
         Object3DSound[199][2] = 79;
         break;
      case 200:
         VObject3D[200] = new Sprite3D;
         VObject3D[200]->LoadSprite3D(10, 8, 0, 0, 9, "SdragonDarkGreen", 0, 0, 0); 
         SoundFX[118].Create("Skraug attack", TS_MEMORY);
         SoundFX[119].Create("Skraug die", TS_MEMORY);
         SoundFX[79].Create("Taunting Hit", TS_MEMORY);
         Object3DSound[200][0] = 118;
         Object3DSound[200][1] = 119;
         Object3DSound[200][2] = 79;
         break;
      case 201:
         VObject3D[201] = new Sprite3D;
         VObject3D[201]->LoadSprite3D(10, 8, 0, 0, 9, "SdragonDarkGreen", 0, 0, 0,2); 
         SoundFX[118].Create("Skraug attack", TS_MEMORY);
         SoundFX[119].Create("Skraug die", TS_MEMORY);
         SoundFX[79].Create("Taunting Hit", TS_MEMORY);
         Object3DSound[201][0] = 118;
         Object3DSound[201][1] = 119;
         Object3DSound[201][2] = 79;
         break;
      case 202:
         VObject3D[202] = new Sprite3D;
         VObject3D[202]->LoadSprite3D(10, 8, 0, 0, 9, "SSdragonDarkGreen", 0, 0, 0); 
         SoundFX[118].Create("Skraug attack", TS_MEMORY);
         SoundFX[119].Create("Skraug die", TS_MEMORY);
         SoundFX[79].Create("Taunting Hit", TS_MEMORY);
         Object3DSound[202][0] = 118;
         Object3DSound[202][1] = 119;
         Object3DSound[202][2] = 79;
         break;
      case 203:
         VObject3D[203] = new Sprite3D;
         VObject3D[203]->LoadSprite3D(10, 8, 0, 0, 9, "SSdragonDarkGreen", 0, 0, 0,2); 
         SoundFX[118].Create("Skraug attack", TS_MEMORY);
         SoundFX[119].Create("Skraug die", TS_MEMORY);
         SoundFX[79].Create("Taunting Hit", TS_MEMORY);
         Object3DSound[203][0] = 118;
         Object3DSound[203][1] = 119;
         Object3DSound[203][2] = 79;
         break;
      case 204:
         VObject3D[204] = new Sprite3D;
         VObject3D[204]->LoadSprite3D(10, 8, 0, 0, 9, "SSdragonDarkGreen", 0, 0, 0,3); 
         SoundFX[118].Create("Skraug attack", TS_MEMORY);
         SoundFX[119].Create("Skraug die", TS_MEMORY);
         SoundFX[79].Create("Taunting Hit", TS_MEMORY);
         Object3DSound[204][0] = 118;
         Object3DSound[204][1] = 119;
         Object3DSound[204][2] = 79;
         break;
      case 205:
         VObject3D[205] = new Sprite3D;
         VObject3D[205]->LoadSprite3D(13, 13, 0, 0, 14, "BigTarantula", 3, 0, 0); 
         SoundFX[72].Create("Spider Dying", TS_MEMORY);
         SoundFX[73].Create("Spider Attack", TS_MEMORY);
         SoundFX[74].Create("Spider Hit", TS_MEMORY);
         Object3DSound[205][0] = 72;
         Object3DSound[205][1] = 73;
         Object3DSound[205][2] = 74;
         break;
      case 206:
         VObject3D[206] = new Sprite3D;
         VObject3D[206]->LoadSprite3D(13, 13, 0, 0, 14, "BigTarantula", 3, 0, 0,2); 
         SoundFX[72].Create("Spider Dying", TS_MEMORY);
         SoundFX[73].Create("Spider Attack", TS_MEMORY);
         SoundFX[74].Create("Spider Hit", TS_MEMORY);
         Object3DSound[206][0] = 72;
         Object3DSound[206][1] = 73;
         Object3DSound[206][2] = 74;
         break;
      case 207:
         VObject3D[207] = new Spell;
         VObject3D[207]->LoadSpell(113, "NM_DechuArival-");
         Object3DSound[207][0] = NULL;
         break;
      case 208:
            VObject3D[208] = new Sprite3D;
            VObject3D[208]->LoadSprite3D(6, "NM_Fireball");
            SoundFX[117].Create("FireBall 2", TS_MEMORY);
            Object3DSound[208][0] = 117;
            break;
       case 209:
            VObject3D[209] = new Sprite3D;
            VObject3D[209]->LoadSprite3D(9, 9, 0, 0, 14, "BigWolf", 0, 0, 0);
            SoundFX[8].Create("Wolf Dying", TS_MEMORY);
            SoundFX[9].Create("Wolf Attack", TS_MEMORY);
            SoundFX[10].Create("Wolf Hit", TS_MEMORY);
            Object3DSound[209][0] = 8;
            Object3DSound[209][1] = 9;
            Object3DSound[209][2] = 10;
            break;
      case 210:
            VObject3D[210] = new Sprite3D;
            VObject3D[210]->LoadSprite3D(9, 9, 0, 0, 14, "BigWolf", 0, 0, 0,2);
            SoundFX[8].Create("Wolf Dying", TS_MEMORY);
            SoundFX[9].Create("Wolf Attack", TS_MEMORY);
            SoundFX[10].Create("Wolf Hit", TS_MEMORY);
            Object3DSound[210][0] = 8;
            Object3DSound[210][1] = 9;
            Object3DSound[210][2] = 10;
            break;
      case 211:
            VObject3D[211] = new Sprite3D;
            VObject3D[211]->LoadSprite3D(13, 13, 0, 0, 9, "BigBigSkel", 0, 0, 0); 
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[38].Create("Skeleton Dying", TS_MEMORY);
            SoundFX[39].Create("Skeleton Hit", TS_MEMORY);
            Object3DSound[211][0] = 4;
            Object3DSound[211][1] = 12;
            Object3DSound[211][2] = 18;
            Object3DSound[211][4] = 38;
            Object3DSound[211][5] = 39;
            break;
      case 212:
            VObject3D[212] = new Sprite3D;
            VObject3D[212]->LoadSprite3D(9, 9, 0, 0, 14, "BigBT", 0, 0, 0); 
            SoundFX[75].Create("Tree Ent Dying", TS_MEMORY);
            SoundFX[76].Create("Electrik", TS_MEMORY);
            SoundFX[90].Create("AxeWood", TS_MEMORY);
            Object3DSound[212][0] = 75;
            Object3DSound[212][1] = 76;
            Object3DSound[212][2] = 90;
            Object3DSound[212][3] = 76;
            break;
      case 213:
            VObject3D[213] = new Sprite3D;
            VObject3D[213]->LoadSprite3D(8, 8, 0, 0, 11, "BigGiantWasp", 2, 0, 0);
            SoundFX[87].Create("Wasp Dying", TS_MEMORY);
            SoundFX[88].Create("Wasp Attack", TS_MEMORY);
            SoundFX[89].Create("Wasp Hit", TS_MEMORY);
            Object3DSound[213][0] = 87;
            Object3DSound[213][1] = 88;
            Object3DSound[213][2] = 89;
            break;
      case 214:
            VObject3D[214] = new Sprite3D;
            VObject3D[214]->LoadSprite3D(9, 9, 0, 0, 6, "DemonSkull", 0, 0, 0); 
            SoundFX[4].Create("Whooshh 1", TS_MEMORY);
            SoundFX[12].Create("Whooshh 3", TS_MEMORY);
            SoundFX[18].Create("Whooshm 6", TS_MEMORY);
            SoundFX[38].Create("Skeleton Dying", TS_MEMORY);
            SoundFX[39].Create("Skeleton Hit", TS_MEMORY);
            Object3DSound[214][0] = 4;
            Object3DSound[214][1] = 12;
            Object3DSound[214][2] = 18;
            Object3DSound[214][4] = 38;
            Object3DSound[214][5] = 39;
            break;
       case 215:
            VObject3D[215] = new Sprite3D;
            SoundFX[35].Create("Rat Dying", TS_MEMORY); 
            SoundFX[36].Create("Rat Attack", TS_MEMORY);
            SoundFX[37].Create("Rat Hit", TS_MEMORY);
            VObject3D[215]->LoadSprite3D(9, 9, 0, 0, 9, "Twiglee", 0, 0, 0);
            Object3DSound[215][0] = 35;
            Object3DSound[215][1] = 36;
            Object3DSound[215][2] = 37;
            break;
       case 216:
            VObject3D[216] = new Sprite3D;
            VObject3D[216]->LoadSprite3D(9, 9, 0, 0, 15, "Ogre", 0, 0, 0);
            SoundFX[103].Create("Whooshm 8", TS_MEMORY);
            SoundFX[105].Create("Orc Dying", TS_MEMORY);
            SoundFX[106].Create("Orc Hit", TS_MEMORY);
            Object3DSound[216][0] = 103;
            Object3DSound[216][1] = 105;
            Object3DSound[216][2] = 106;
            break;

        case 217:
            VObject3D[217] = new Sprite3D;
            VObject3D[217]->LoadSprite3D(18, 15, 0, 0, 18, "Golem", 0, 0, 0);
            SoundFX[103].Create("Whooshm 8", TS_MEMORY);
            SoundFX[105].Create("Orc Dying", TS_MEMORY);
            SoundFX[106].Create("Orc Hit", TS_MEMORY);
            Object3DSound[217][0] = 103;
            Object3DSound[217][1] = 105;
            Object3DSound[217][2] = 106;
            break;

         case 218:
            VObject3D[218] = new Sprite3D;
            VObject3D[218]->LoadSprite3D(18, 15, 0, 0, 18, "Golem", 0, 0, 0,2);
            SoundFX[103].Create("Whooshm 8", TS_MEMORY);
            SoundFX[105].Create("Orc Dying", TS_MEMORY);
            SoundFX[106].Create("Orc Hit", TS_MEMORY);
            Object3DSound[218][0] = 103;
            Object3DSound[218][1] = 105;
            Object3DSound[218][2] = 106;
            break;

         case 219:
            VObject3D[219] = new Sprite3D;
            VObject3D[219]->LoadSprite3D(18, 15, 0, 0, 18, "Golem", 0, 0, 0,3);
            SoundFX[103].Create("Whooshm 8", TS_MEMORY);
            SoundFX[105].Create("Orc Dying", TS_MEMORY);
            SoundFX[106].Create("Orc Hit", TS_MEMORY);
            Object3DSound[219][0] = 103;
            Object3DSound[219][1] = 105;
            Object3DSound[219][2] = 106;
            break;

         case 220:
            VObject3D[220] = new Sprite3D;
            VObject3D[220]->LoadSprite3D(10, 8, 0, 0, 9, "dragonDarkGreen", 0, 0, 0,2); 
			   SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[220][0] = 27;
            Object3DSound[220][1] = 28;
            Object3DSound[220][2] = 29;            
			break;
         case 221:
            VObject3D[221] = new Sprite3D;
            VObject3D[221]->LoadSprite3D(10, 8, 0, 0, 9, "dragonDarkGreen", 0, 0, 0,3); 
			   SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[221][0] = 27;
            Object3DSound[221][1] = 28;
            Object3DSound[221][2] = 29;            
			break;
         case 222:
            VObject3D[222] = new Sprite3D;
            VObject3D[222]->LoadSprite3D(10, 8, 0, 0, 9, "dragonDarkGreen", 0, 0, 0,4); 
			   SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[222][0] = 27;
            Object3DSound[222][1] = 28;
            Object3DSound[222][2] = 29;            
			break;
         case 223:
            VObject3D[223] = new Sprite3D;
            VObject3D[223]->LoadSprite3D(10, 8, 0, 0, 9, "dragonDarkGreen", 0, 0, 0,5); 
			   SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[223][0] = 27;
            Object3DSound[223][1] = 28;
            Object3DSound[223][2] = 29;            
			break;
         case 224:
            VObject3D[224] = new Sprite3D;
            VObject3D[224]->LoadSprite3D(10, 8, 0, 0, 9, "dragonDarkGreen", 0, 0, 0,6); 
			   SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[224][0] = 27;
            Object3DSound[224][1] = 28;
            Object3DSound[224][2] = 29;            
			break;
         case 225:
            VObject3D[225] = new Sprite3D;
            VObject3D[225]->LoadSprite3D(10, 8, 0, 0, 9, "dragonDarkGreen", 0, 0, 0,7); 
			   SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[225][0] = 27;
            Object3DSound[225][1] = 28;
            Object3DSound[225][2] = 29;            
			break;
         case 226:
            VObject3D[226] = new Sprite3D;
            VObject3D[226]->LoadSprite3D(10, 8, 0, 0, 9, "dragonDarkGreen", 0, 0, 0,8); 
			   SoundFX[27].Create("Demon Dying", TS_MEMORY);
            SoundFX[28].Create("Demon Attack", TS_MEMORY);
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[226][0] = 27;
            Object3DSound[226][1] = 28;
            Object3DSound[226][2] = 29;            
			break;
         case 227:
            VObject3D[227] = new Sprite3D;
            VObject3D[227]->LoadSprite3D(1, "Sword01_Spell");
            SoundFX[65].Create("Small Projectile", TS_MEMORY);
            Object3DSound[227][0] = 65;          
			break;
         case 228:
            VObject3D[228] = new Spell;
            VObject3D[228]->LoadSpell(33, "NM_Lightning000-");
            SoundFX[125].Create("FireBall 2", TS_MEMORY);
            Object3DSound[228][0] = 125;
         break;
         case 229:
            VObject3D[229] = new Spell;
            VObject3D[229]->LoadSpell(41, "NM_Fire000-");
            SoundFX[125].Create("FireBall 2", TS_MEMORY);
            Object3DSound[229][0] = 125;
         break;
         case 230:
            VObject3D[230] = new Spell;
            VObject3D[230]->LoadSpell(41, "NM_Poison000-");
            SoundFX[59].Create("Ice Cloud", TS_MEMORY);
            Object3DSound[230][0] = 59;
         break;
		 
		 //BLBL test d'ajout de sorts dans les DDA ::
         case 231:
            VObject3D[231] = new Spell;
            VObject3D[231]->LoadSpell(41, "NMS_SupraHeal-");
            SoundFX[112].Create("Healing", TS_MEMORY);
            Object3DSound[231][0] = 112;
         break;
         case 232:
            VObject3D[232] = new Spell;
            //VObject3D[232]->LoadSpell(41, "NEW_Boulder-");//cette animation est trop grosse ! 41 images en 640x480!!
			VObject3D[232]->LoadSpell(21, "64kSpellBoulders-");//je charge le boulders normal à la place.
            SoundFX[29].Create("Demon Hit", TS_MEMORY);
            Object3DSound[232][0] = 29;
         break;


         
         
         
         //++NMNMNM --[NEW TROWING Weopen Load]
         /*
         case 200:
            VObject3D[200] = new Sprite3D;
            VObject3D[200]->LoadSprite3D(6, "Throwing01Star");
            SoundFX[65].Create("Small Projectile", TS_MEMORY);
            Object3DSound[200][0] = 65;
            break;
         case 201:
            VObject3D[201] = new Sprite3D;
            VObject3D[201]->LoadSprite3D(1, "Throwing01Dagger");
            SoundFX[65].Create("Small Projectile", TS_MEMORY);
            Object3DSound[201][0] = 65;
            break;
         */
         //++NMNMNM --[NEW TEST SPELL Load]
            /*
         case 190:
            VObject3D[190] = new Spell;
            VObject3D[190]->LoadSpell(31, "BoulderFire-");
         break;
         case 191:
            VObject3D[191] = new Spell;
            VObject3D[191]->LoadSpell(31, "BoulderFireA-");
         break;
         case 192:
            VObject3D[192] = new Spell;
            VObject3D[192]->LoadSpell(36, "NecroA01-");
         break;
         case 193:
            VObject3D[193] = new Spell;
            VObject3D[193]->LoadSpell(36, "NecroA01A-");
         break;
         case 194:
            VObject3D[194] = new Spell;
            VObject3D[194]->LoadSpell(36, "PoisonA01-");
         break;
         case 195:
            VObject3D[195] = new Spell;
            VObject3D[195]->LoadSpell(36, "PoisonA01A-");
         break;
         case 196:
            VObject3D[196] = new Spell;
            VObject3D[196]->LoadSpell(41, "FireGlow01-");
         break;
         case 197:
            VObject3D[197] = new Spell;
            VObject3D[197]->LoadSpell(41, "FireGlow01a-");
         break;
         case 198:
            VObject3D[198] = new Spell;
            VObject3D[198]->LoadSpell(27, "ProtectA01-");
         break;
         case 199:
            VObject3D[199] = new Spell;
            VObject3D[199]->LoadSpell(27, "ProtectA01a-");
         break;
         case 200:
            VObject3D[200] = new Spell;
            VObject3D[200]->LoadSpell(31, "HealA01-");
         break;
         case 201:
            VObject3D[201] = new Spell;
            VObject3D[201]->LoadSpell(31, "HealA01a-");
         break;
         case 202:
            VObject3D[202] = new Spell;
            VObject3D[202]->LoadSpell(36, "FireGlow02-");
         break;
         case 203:
            VObject3D[203] = new Spell;
            VObject3D[203]->LoadSpell(36, "FireGlow02a-");
         break;

         case 230:
            VObject3D[230] = new Spell;
            VObject3D[230]->LoadSpell(51, "thunderstorm-");
         break;
         */
         
      
      }
      Object3DLoad[x] = true;
   }
}



/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::Create(void) 
{
    DWORD dwLoadTime = timeGetTime();
    // Load all Object and Object3D sprite into memory.
    DefaultMouseCursor[__OBJGROUP_DAGGER]						   = GET;
    DefaultMouseCursor[__OBJGROUP_SHORT_SWORD]				   = GET;
    DefaultMouseCursor[__OBJGROUP_LONG_SWORD] 					= GET;
    DefaultMouseCursor[__OBJGROUP_FLAIL]		   				= GET;
    DefaultMouseCursor[__OBJGROUP_MORNING_STAR]	   			= GET;
    DefaultMouseCursor[__OBJGROUP_WARHAMMER]			   		= GET;
    DefaultMouseCursor[__OBJGROUP_SPEAR]					   	= GET;
    DefaultMouseCursor[__OBJGROUP_AXE]							   = GET;
    DefaultMouseCursor[__OBJGROUP_LEATHER_ARMOR] 				= GET;   	
    DefaultMouseCursor[__OBJGROUP_WELL_TALK]		   			= TALK;
    //	DefaultMouseCursor[__OBJGROUP_RINGMAIL]			   		= GET;
    DefaultMouseCursor[__OBJGROUP_LEATHER_HELM]			   	= GET;
    DefaultMouseCursor[__OBJGROUP_MAGE_SPELLBOOK]			   = GET;
    DefaultMouseCursor[__OBJGROUP_PRIEST_ANKH]   			   = GET;
    DefaultMouseCursor[__OBJGROUP_SCROLL]			   	      = GET;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR_CLOSED]  	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR_OPENED]	   	= USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_CHAIR]  	         = NONE;
    DefaultMouseCursor[__OBJGROUP_LONGUE_VUE]		   	      = USE;
    DefaultMouseCursor[__OBJGROUP_TORCH]				   		= GET;
    DefaultMouseCursor[__OBJGROUP_IRON_KEY]				   	= GET;
    DefaultMouseCursor[__OBJGROUP_BLUE_KEY]					   = GET;
    DefaultMouseCursor[__OBJGROUP_WEIRD_KEY]  					= GET;
    DefaultMouseCursor[__OBJGROUP_GOLD_KEY]	   				= GET;
    DefaultMouseCursor[__OBJGROUP_KEY_HOLD]		   			= GET;
    DefaultMouseCursor[__OBJGROUP_EMPTY_MUG]			   		= GET;
    DefaultMouseCursor[__OBJGROUP_BEER_MUG]				   	= USE;
    DefaultMouseCursor[__OBJGROUP_BEER_MUG_TOO_FULL]		   = USE;
    DefaultMouseCursor[__OBJGROUP_EMPTY_GLASS]   				= GET;
    DefaultMouseCursor[__OBJGROUP_BEVERAGE]		   			= USE;
    DefaultMouseCursor[__OBJGROUP_CUP_FULL]			   		= GET;
    DefaultMouseCursor[__OBJGROUP_WOODEN_CUP]				   	= GET;
    DefaultMouseCursor[__OBJGROUP_CHEST]						   = USE;
    DefaultMouseCursor[__OBJGROUP_CHEST_OPEN] 					= USE;
    DefaultMouseCursor[__OBJGROUP_BASKET_CLOSE]  				= USE;
    DefaultMouseCursor[__OBJGROUP_BASKET_OPEN]	   			= USE;
    DefaultMouseCursor[__OBJGROUP_RED_BOOK]			   		= GET;
    DefaultMouseCursor[__OBJGROUP_GREY_BOOK]				   	= NONE;
    DefaultMouseCursor[__OBJGROUP_BROWN_BOOK]					   = NONE;
    DefaultMouseCursor[__OBJGROUP_BACKPACK]   					= GET;
    DefaultMouseCursor[__OBJGROUP_BANDAGES_PACK] 				= GET;
    DefaultMouseCursor[__OBJGROUP_BANDAGE_1]		   			= GET;
    DefaultMouseCursor[__OBJGROUP_BANDAGE_2]			   		= GET;
    DefaultMouseCursor[__OBJGROUP_WOODEN_BOWL]			   	= GET;
    DefaultMouseCursor[__OBJGROUP_WOODEN_VASE]			   	= GET;
    DefaultMouseCursor[__OBJGROUP_GOLD_BAR]   					= GET;
    DefaultMouseCursor[__OBJGROUP_GOLD_BAR_PACK] 				= GET;
    DefaultMouseCursor[__OBJGROUP_BRONZE_BAR]		   			= GET;
    DefaultMouseCursor[__OBJGROUP_BRONZE_BAR_PACK]	   		= GET;
    DefaultMouseCursor[__OBJGROUP_IRON_BAR]				   	= GET;
    DefaultMouseCursor[__OBJGROUP_IRON_BAR_PACK] 				= GET;
    DefaultMouseCursor[__OBJGROUP_BROOM_ON_WALL]	   			= GET;
    DefaultMouseCursor[__OBJGROUP_PIOCHE]				   		= GET;
    DefaultMouseCursor[__OBJGROUP_BROOM_ON_FLOOR]   			= GET;
    DefaultMouseCursor[__OBJGROUP_HAMMER]  						= GET;
    DefaultMouseCursor[__OBJGROUP_PINCES]	   					= GET;
    DefaultMouseCursor[__OBJGROUP_BALANCE]		   				= GET;
    DefaultMouseCursor[__OBJGROUP_HOURGLASS]		   			= GET;
    DefaultMouseCursor[__OBJGROUP_EPROUVETTE_6_PACKS]  		= GET;
    DefaultMouseCursor[__OBJGROUP_EPROUVETTE_3_PACKS]	   	= GET;
    DefaultMouseCursor[__OBJGROUP_EPROUVETTE_ALONE] 			= GET;
    DefaultMouseCursor[__OBJGROUP_BUCHE_DE_BOIS]		   		= GET;
    DefaultMouseCursor[__OBJGROUP_WOOD_PACK_CLASSED]	   	= GET;
    DefaultMouseCursor[__OBJGROUP_WOOD_PACK_MIXED]			   = GET;
    DefaultMouseCursor[__OBJGROUP_JAMBON]  						= GET;
    DefaultMouseCursor[__OBJGROUP_LONG_SAUSAGE]  				= GET;
    DefaultMouseCursor[__OBJGROUP_SAUSAGE_PACK]	   			= GET;
    DefaultMouseCursor[__OBJGROUP_FAT_JAMBON]			   		= GET;
    DefaultMouseCursor[__OBJGROUP_CHIKEN]					   	= GET;
    DefaultMouseCursor[__OBJGROUP_BREAD]						   = GET;
    DefaultMouseCursor[__OBJGROUP_PACK_OF_PASTRIES] 			= GET;
    DefaultMouseCursor[__OBJGROUP_WATERBASKET]		   		= GET;
    DefaultMouseCursor[__OBJGROUP_FISHING_POLE]			   	= GET;
    DefaultMouseCursor[__OBJGROUP_DEAD_FISHES]				   = GET;
    DefaultMouseCursor[__OBJGROUP_EMPTY_BASKET]  				= GET;
    DefaultMouseCursor[__OBJGROUP_ARROW]			   			= GET;
    DefaultMouseCursor[__OBJGROUP_ARROW_PACK]			   		= GET;
    DefaultMouseCursor[__OBJGROUP_BOW]						   	= GET;
    DefaultMouseCursor[__OBJGROUP_CROSSBOW]					   = GET;
    DefaultMouseCursor[__OBJGROUP_BANJO]   						= GET;
    DefaultMouseCursor[__OBJGROUP_TAMBOUR]	   					= GET;
    DefaultMouseCursor[__OBJGROUP_MAP]			   				= GET;
    DefaultMouseCursor[__OBJGROUP_SCIE]				   			= GET;
    DefaultMouseCursor[__OBJGROUP_LONG_WHOOL_BALL]	   		= GET;
    DefaultMouseCursor[__OBJGROUP_WHOOL_BALL]				   	= GET;
    DefaultMouseCursor[__OBJGROUP_GREEN_WHOOL_BALL]			   = GET;
    DefaultMouseCursor[__OBJGROUP_BLUE_WHOOL_BALL]  			= GET;
    DefaultMouseCursor[__OBJGROUP_SHOVEL]  						= GET;
    DefaultMouseCursor[__OBJGROUP_BLUE_POTION]   				= GET;
    DefaultMouseCursor[__OBJGROUP_YELLOW_POTION]	   			= GET;
    DefaultMouseCursor[__OBJGROUP_BLACK_POTION]		   		= GET;
    DefaultMouseCursor[__OBJGROUP_EMPTY_POTION]			   	= GET;
    DefaultMouseCursor[__OBJGROUP_PURPLE_POTION]				   = GET;
    DefaultMouseCursor[__OBJGROUP_BASKET_FULL_WATER]   		= GET;
    DefaultMouseCursor[__OBJGROUP_BASKET_FULL_FRUIT]	   	= GET;
    DefaultMouseCursor[__OBJGROUP_WOODEN_ROUND_CHAIR]		   = NONE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_CHAIR_2]   			= NONE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_ROUND_CHAIR_2]   	= NONE;
    DefaultMouseCursor[__OBJGROUP_CRATES]  						= NONE;
    DefaultMouseCursor[__OBJGROUP_RECTANGLE_CRATES] 			= NONE;
    DefaultMouseCursor[__OBJGROUP_CUBIQ_CRATES]		   		= GET;
    DefaultMouseCursor[__OBJGROUP_KITCHEN_BOWL]			   	= GET;
    DefaultMouseCursor[__OBJGROUP_KITCHEN_SPOON]				   = GET;
    DefaultMouseCursor[__OBJGROUP_KITCHEN_FORK]  				= GET;
    DefaultMouseCursor[__OBJGROUP_KITCHEN_KNIFE]	   			= GET;
    DefaultMouseCursor[__OBJGROUP_ROULEAU_A_PATRE]	   		= GET;
    DefaultMouseCursor[__OBJGROUP_CHANDELLE]				      = GET;
    DefaultMouseCursor[__OBJGROUP_CHANDELLE_SUR_PIED]		   = GET;
    DefaultMouseCursor[__OBJGROUP_MIRROR]  						= GET;
    DefaultMouseCursor[__OBJGROUP_STAFF1]	   					= GET;
    DefaultMouseCursor[__OBJGROUP_MACE]	            			= GET;
    DefaultMouseCursor[__OBJGROUP_SPIKE_MACE]		   			= GET;
    DefaultMouseCursor[__OBJGROUP_MAUL]					   		= GET;
    DefaultMouseCursor[__OBJGROUP_DOUBLE_AXE]				   	= GET;
    DefaultMouseCursor[__OBJGROUP_SINGLE_AXE]					   = GET;
    DefaultMouseCursor[__OBJGROUP_BASTARD_SWORD] 				= GET;
    DefaultMouseCursor[__OBJGROUP_SPEAR_AXE]		   			= GET;
    DefaultMouseCursor[__OBJGROUP_SCROLL_OUVERT]		   		= GET;
    DefaultMouseCursor[__OBJGROUP_SCROLL_BLUE]			   	= GET;
    DefaultMouseCursor[__OBJGROUP_LIVRE_OUVERT]				   = GET;
    DefaultMouseCursor[__OBJGROUP_LIVRE_OUVERT_EPAIS]  		= GET;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR_CLOSED_I]      = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR_OPENED_I]      = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_CHAIR_I]			   = NONE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_CHAIR_2_I]			   = NONE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_ROUND_CHAIR_2_I]    = NONE;
    DefaultMouseCursor[__OBJGROUP_2_WOODEN_CHAIR]			   = NONE;
    DefaultMouseCursor[__OBJGROUP_2_WOODEN_CHAIR_2]			   = NONE;
    DefaultMouseCursor[__OBJGROUP_2_WOODEN_ROUND_CHAIR_2]    = NONE;
    DefaultMouseCursor[__OBJGROUP_2_WOODEN_CHAIR_I]			   = NONE;
    DefaultMouseCursor[__OBJGROUP_2_WOODEN_CHAIR_2_I]		   = NONE;
    DefaultMouseCursor[__OBJGROUP_2_WOODEN_ROUND_CHAIR_2_I]  = NONE;
    DefaultMouseCursor[__OBJGROUP_POUCH]                     = GET;
    DefaultMouseCursor[__OBJGROUP_ROPE]                      = GET;
    DefaultMouseCursor[__OBJGROUP_STUFFED_BASKET_1]          = GET;
    DefaultMouseCursor[__OBJGROUP_STUFFED_BASKET_2]          = GET;
    DefaultMouseCursor[__OBJGROUP_STUFFED_BASKET_3]          = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_1]                   = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_2]                   = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_3]                   = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_4]                   = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_5]                   = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_6]                   = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_7]                   = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_8]                   = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_9]                   = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_10]                  = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_11]                  = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_12]                  = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_13]                  = GET;
    DefaultMouseCursor[__OBJGROUP_FRUIT_14]                  = GET;
    DefaultMouseCursor[__OBJGROUP_COINS]                     = GET;
    DefaultMouseCursor[__OBJGROUP_COINS_PILE]                = GET;
    DefaultMouseCursor[__OBJGROUP_COINS_BIG_PILE]            = GET;
    DefaultMouseCursor[__OBJGROUP_BATWINGS]                  = GET;
    DefaultMouseCursor[__OBJGROUP_GEMS_PURPLE]               = GET;
    DefaultMouseCursor[__OBJGROUP_GEMS_YELLOW]               = GET;
    DefaultMouseCursor[__OBJGROUP_GEMS_BLUE]                 = GET;
    DefaultMouseCursor[__OBJGROUP_GEMS_GREEN]                = GET;
    DefaultMouseCursor[__OBJGROUP_GEMS_PACK_1]               = GET;
    DefaultMouseCursor[__OBJGROUP_GEMS_PACK_2]               = GET;
    DefaultMouseCursor[__OBJGROUP_GEMS_PACK_3]               = GET;
    DefaultMouseCursor[__OBJGROUP_KOBOLDHAIR]                = GET;      
    DefaultMouseCursor[__OBJGROUP_NECKLACE_1]                = GET;
    DefaultMouseCursor[__OBJGROUP_NECKLACE_2]                = GET;
    DefaultMouseCursor[__OBJGROUP_NECKLACE_3]                = GET;
    DefaultMouseCursor[__OBJGROUP_ORCFEET]                   = GET;
    DefaultMouseCursor[__OBJGROUP_RINGS_1]                   = GET;
    DefaultMouseCursor[__OBJGROUP_RINGS_2]                   = GET;
    DefaultMouseCursor[__OBJGROUP_RINGS_3]                   = GET;
    DefaultMouseCursor[__OBJGROUP_RINGS_4]                   = GET;
    DefaultMouseCursor[__OBJGROUP_RINGS_5]                   = GET;
    DefaultMouseCursor[__OBJGROUP_SKEL_BONE]                 = GET;
    DefaultMouseCursor[__OBJGROUP_SPIDER_EYES]               = GET;
    DefaultMouseCursor[__OBJGROUP_PADDED_GLOVE]              = GET;
    DefaultMouseCursor[__OBJGROUP_PADDED_HELM]               = GET;
    DefaultMouseCursor[__OBJGROUP_PADDED_ARMOR]              = GET;
    DefaultMouseCursor[__OBJGROUP_PADDED_SLEEVES]            = GET;
    DefaultMouseCursor[__OBJGROUP_PADDED_LEGGINGS]           = GET;
    DefaultMouseCursor[__OBJGROUP_SCALE_ARMOR]               = GET;
    DefaultMouseCursor[__OBJGROUP_LEVIER]                    = USE;
    DefaultMouseCursor[__OBJGROUP_FIRECAMP]                  = USE;
    DefaultMouseCursor[__OBJGROUP_CAMPBURNT]                 = USE;
    DefaultMouseCursor[__OBJGROUP_GLINTING_SWORD]            = GET;
    DefaultMouseCursor[__OBJGROUP_DESTINY_GEM]               = GET;
    DefaultMouseCursor[__OBJGROUP_VOLCANO_ROCK]              = GET;
    DefaultMouseCursor[__OBJGROUP_RING_HELM]                 = GET;     
    DefaultMouseCursor[__OBJGROUP_RING_ARMOR]                = GET;     
    DefaultMouseCursor[__OBJGROUP_RING_ARMOR_SLEEVES]        = GET;
    DefaultMouseCursor[__OBJGROUP_RING_LEGGINGS]             = GET;    
    DefaultMouseCursor[__OBJGROUP_FEATHER]                   = GET;
    DefaultMouseCursor[__OBJGROUP_ICESHARD]                  = GET;
    DefaultMouseCursor[__OBJGROUP_LIFEGEM]                   = GET;
    DefaultMouseCursor[__OBJGROUP_OAKTREELEAF]               = GET;
    DefaultMouseCursor[__OBJGROUP_PADDED_BOOTS]              = GET;
    DefaultMouseCursor[__OBJGROUP_TORCHE]                    = USE;
    DefaultMouseCursor[__OBJGROUP_TORCHE_I]                  = USE;
    DefaultMouseCursor[__OBJGROUP_BELT]                      = GET;
    DefaultMouseCursor[__OBJGROUP_BLUEFLASK]                 = GET;
    DefaultMouseCursor[__OBJGROUP_BRACELET]                  = GET;
    DefaultMouseCursor[__OBJGROUP_CHEST2]                    = USE;
    DefaultMouseCursor[__OBJGROUP_PINKLEAF]                  = GET;
    DefaultMouseCursor[__OBJGROUP_POT_GREEN]                 = GET;
    DefaultMouseCursor[__OBJGROUP_POT_RED]                   = GET;
    DefaultMouseCursor[__OBJGROUP_SHIELD]                    = GET;
    DefaultMouseCursor[__OBJGROUP_STONELIFE]                 = GET;
    DefaultMouseCursor[__OBJGROUP_TORCH2]                    = GET;
    DefaultMouseCursor[__OBJGROUP_TOOTH]                     = GET;
    DefaultMouseCursor[__OBJGROUP_GODPOT_BLUE]               = GET;
    DefaultMouseCursor[__OBJGROUP_GODPOT_GREEN]              = GET;
    DefaultMouseCursor[__OBJGROUP_GODPOT_RED]                = GET;
    DefaultMouseCursor[__OBJGROUP_GODPOT_YELLOW]             = GET;
    DefaultMouseCursor[__OBJGROUP_LARGEPOT_GREEN]            = GET;
    DefaultMouseCursor[__OBJGROUP_LARGEPOT_RED]              = GET;
    DefaultMouseCursor[__OBJGROUP_POT_BLUE]                  = GET;
    DefaultMouseCursor[__OBJGROUP_POT_ORANGE]                = GET;
    DefaultMouseCursor[__OBJGROUP_POT_TURQUOISE]             = GET;
    DefaultMouseCursor[__OBJGROUP_POT_VIOLET]                = GET;
    DefaultMouseCursor[__OBJGROUP_POT_YELLOW]                = GET;
    DefaultMouseCursor[__OBJGROUP_GREENFLASK]                = GET;
    DefaultMouseCursor[__OBJGROUP_REDFLASK]                  = GET;
    DefaultMouseCursor[__OBJGROUP_LEATHER_GLOVE]             = GET;
    DefaultMouseCursor[__OBJGROUP_LEATHER_BOOTS]             = GET;
    DefaultMouseCursor[__OBJGROUP_LEATHER_PANTS]             = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_GLOVE]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_W_SLEEVE]      = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_BOOT]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_LEGS]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_HELM]                = GET;
    DefaultMouseCursor[__OBJGROUP_CHAIN_LEGS]                = GET;
    DefaultMouseCursor[__OBJGROUP_CHAIN_BODY]                = GET;
    DefaultMouseCursor[__OBJGROUP_CHAIN_COIF]                = GET;
    DefaultMouseCursor[__OBJGROUP_DARK_SWORD]                = GET;
    DefaultMouseCursor[__OBJGROUP_ROMAN_SHIELD]              = GET;
    DefaultMouseCursor[__OBJGROUP_BAROSSA_SHIELD]            = GET;
    DefaultMouseCursor[__OBJGROUP_REAL_DARKSWORD]            = GET;
    DefaultMouseCursor[__OBJGROUP_HORNED_HELMET]             = GET;
    DefaultMouseCursor[__OBJGROUP_BATTLE_SWORD]              = GET;
    DefaultMouseCursor[__OBJGROUP_NECROMANROBE]              = GET;
    DefaultMouseCursor[__OBJGROUP_GOLDENCROWN]               = GET;
    DefaultMouseCursor[__OBJGROUP_GOLDEN_STAR]               = GET;
    DefaultMouseCursor[__OBJGROUP_ELVEN_HAT]                 = GET;
    DefaultMouseCursor[__OBJGROUP_ORC_SHIELD]                = GET;
    DefaultMouseCursor[__OBJGROUP_STUDDEDARMOR]              = GET;
    DefaultMouseCursor[__OBJGROUP_LEG_CLOTH1]                = GET;
    DefaultMouseCursor[__OBJGROUP_BODY_CLOTH1]               = GET;
    DefaultMouseCursor[__OBJGROUP_STUDDEDLEG]                = GET;
    DefaultMouseCursor[__OBJGROUP_REDCAPE]                   = GET;
    DefaultMouseCursor[__OBJGROUP_BLACKLEATHER_BOOT]         = GET;
    DefaultMouseCursor[__OBJGROUP_WHITELEATHER_BOOT]         = GET;
    DefaultMouseCursor[__OBJGROUP_HEAD]                      = GET;
    DefaultMouseCursor[__OBJGROUP_CHEST_TALK]                = TALK;
    DefaultMouseCursor[__OBJGROUP_SUNDIAL_TALK]              = TALK;
    DefaultMouseCursor[__OBJGROUP_SIGN1_TALK]                = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SIGN2_TALK]                = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SIGN3_TALK]                = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_STAFF2]                    = GET;
    DefaultMouseCursor[__OBJGROUP_STAFF3]                    = GET;
    DefaultMouseCursor[__OBJGROUP_STAFF4]                    = GET;
    DefaultMouseCursor[__OBJGROUP_PORTAL]                    = TALK;
    DefaultMouseCursor[__OBJGROUP_VAULT_TALK]                = TALK;
    DefaultMouseCursor[__OBJGROUP_FOUGERE]                   = USE;
    DefaultMouseCursor[__OBJGROUP_KRAANIAN_EGG]              = GET;
    DefaultMouseCursor[__OBJGROUP_WOLF_PELT]                 = GET;
    DefaultMouseCursor[__OBJGROUP_VAULT_TALK_I]              = TALK;
    DefaultMouseCursor[__OBJGROUP_HUMAN_FOOT]                = GET;
    DefaultMouseCursor[__OBJGROUP_TROLLBADGE]                = GET;
    DefaultMouseCursor[__OBJGROUP_SHOP_INN]                  = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SHOP_POTION]               = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SHOP_PAWN]                 = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SHOP_ARMOR]                = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SHOP_WEAPON]               = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SHOP_INN_I]                = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SHOP_POTION_I]             = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SHOP_PAWN_I]               = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SHOP_ARMOR_I]              = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_SHOP_WEAPON_I]             = USE_ONSITE;
    DefaultMouseCursor[__OBJGROUP_LARGE_BOW]                 = GET;
    DefaultMouseCursor[__OBJGROUP_QUIVER]                    = GET;
    DefaultMouseCursor[__OBJGROUP_REDROBE]                   = GET;
    DefaultMouseCursor[__OBJGROUP_MAGEROBE]                  = GET;
    DefaultMouseCursor[__OBJGROUP_WHITEROBE]                 = GET;
    DefaultMouseCursor[__OBJGROUP_CHEST_TROLL]				   = USE;
    DefaultMouseCursor[__OBJGROUP_WHIRLPOOL]                 = NONE;
    DefaultMouseCursor[__OBJGROUP_OGRECLUB]			   		= GET;
    DefaultMouseCursor[__OBJGROUP_LARGE_BOW2]	       	   	= GET;
    DefaultMouseCursor[__OBJGROUP_FANCY_SHORT_BOW]	   		= GET;
    DefaultMouseCursor[__OBJGROUP_FANCY_LONG_BOW]	   		= GET;
    DefaultMouseCursor[__OBJGROUP_BLUE_QUIVER]	   		   = GET;
    DefaultMouseCursor[__OBJGROUP_YELLOW_QUIVER]	   		   = GET;
    DefaultMouseCursor[__OBJGROUP_GREEN_QUIVER]  	   		= GET;
    DefaultMouseCursor[__OBJGROUP_RED_QUIVER]	         		= GET;
    DefaultMouseCursor[__OBJGROUP_BLACK_QUIVER]	      		= GET;
    DefaultMouseCursor[__OBJGROUP_DOOR_TALK]                 = TALK;
    DefaultMouseCursor[__OBJGROUP_DOOR_TALK_I]               = TALK;
    DefaultMouseCursor[__OBJGROUP_VAULT]                     = USE;
    DefaultMouseCursor[__OBJGROUP_VAULT_I]                   = USE;
    DefaultMouseCursor[__OBJGROUP_CENTAUR_SHIELD1]           = GET;
    DefaultMouseCursor[__OBJGROUP_CENTAUR_SHIELD2]           = GET;
    DefaultMouseCursor[__OBJGROUP_SHAMAN_HELM]               = GET;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_CLUB]               = GET;             
    DefaultMouseCursor[__OBJGROUP_SKAVEN_KNIFE]              = GET;              
    DefaultMouseCursor[__OBJGROUP_SKAVEN_SHIELD1]            = GET;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_SHIELD2]            = GET;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_SHIELD3]            = GET;
    DefaultMouseCursor[__OBJGROUP_SKELETON_AXE]              = GET;
    DefaultMouseCursor[__OBJGROUP_SKELETON_HELM]             = GET;
    DefaultMouseCursor[__OBJGROUP_SKELETON_SHIELD]           = GET;
    DefaultMouseCursor[__OBJGROUP_ORC_SHIELD2]               = GET;
    DefaultMouseCursor[__OBJGROUP_SK_SHIELD]                 = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SHIELD01]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SHIELD02]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_ICESHIELD]               = GET;
    DefaultMouseCursor[__OBJGROUP_T4CP_SHIELD]               = GET;
    DefaultMouseCursor[__OBJGROUP_SKELETON_SWORD]            = GET;
    DefaultMouseCursor[__OBJGROUP_SERAPH_BLACK_WINGS]        = GET;              
    DefaultMouseCursor[__OBJGROUP_SERAPH_WHITE_WINGS]        = GET;
    DefaultMouseCursor[__OBJGROUP_SERAPH_DECHU_WINGS]        = GET;
    DefaultMouseCursor[__OBJGROUP_SERAPH_DECHU2_WINGS]       = GET;
    DefaultMouseCursor[__OBJGROUP_DARK_GEM]                  = GET;
    DefaultMouseCursor[__OBJGROUP_PURPLE_GEM]                = GET;
    DefaultMouseCursor[__OBJGROUP_RED_GEM]                   = GET;
    DefaultMouseCursor[__OBJGROUP_WHIRLPOOL2]		   			= NONE;
    DefaultMouseCursor[__OBJGROUP_COFFIN_TALK]		   		= TALK;
    DefaultMouseCursor[__OBJGROUP_HALBERD]                   = GET;
    DefaultMouseCursor[__OBJGROUP_SMALL_PURPLE_GEM]          = GET;
    DefaultMouseCursor[__OBJGROUP_SMALL_RED_GEM]             = GET;
    DefaultMouseCursor[__OBJGROUP_CAULDRON]                  = NONE;
    DefaultMouseCursor[__OBJGROUP_CAULDRON_GROUND]           = NONE;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_CORPSE1]            = USE;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_CORPSE2]            = USE;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_CORPSE3]            = USE;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_CORPSE4]            = USE;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_CORPSE_I1]          = USE;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_CORPSE_I2]          = USE;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_CORPSE_I3]          = USE;
    DefaultMouseCursor[__OBJGROUP_SKAVEN_CORPSE_I4]          = USE;
    DefaultMouseCursor[__OBJGROUP_RIBCAGE]		   			= TALK;
    DefaultMouseCursor[__OBJGROUP_HEART]                = GET;
	//New added by Dialsoft - BEGIN
	DefaultMouseCursor[__OBJGROUP_BRACELET_RUBIS]			= GET;
	DefaultMouseCursor[__OBJGROUP_BELT_JEWELS_BROWN]		= GET;
	DefaultMouseCursor[__OBJGROUP_BELT_JEWELS_PURPLE]		= GET;
	DefaultMouseCursor[__OBJGROUP_BELT_RED]					= GET;
	DefaultMouseCursor[__OBJGROUP_BRACELET_AZURE]			= GET;
	DefaultMouseCursor[__OBJGROUP_BRACELET_SQUARE_WHITE]	= GET;
	DefaultMouseCursor[__OBJGROUP_GEM_AZURE]				= GET;
	DefaultMouseCursor[__OBJGROUP_GEM_ORANGE]				= GET;
	DefaultMouseCursor[__OBJGROUP_GEM_PINK]					= GET;
	DefaultMouseCursor[__OBJGROUP_RING_3RUBIS]				= GET;
	DefaultMouseCursor[__OBJGROUP_RING_RED]					= GET;
	DefaultMouseCursor[__OBJGROUP_RING_STARGOLD]			= GET;
	DefaultMouseCursor[__OBJGROUP_MASQUE_GOBLIN]			= GET;
	DefaultMouseCursor[__OBJGROUP_RING_CUT_GREEN]			= GET;
	DefaultMouseCursor[__OBJGROUP_RING_CUT_RED]				= GET;
	DefaultMouseCursor[__OBJGROUP_RUBIS_FOCUS]				= GET;
	DefaultMouseCursor[__OBJGROUP_DIAMOND_FOCUS]			= GET;
	DefaultMouseCursor[__OBJGROUP_EMERALD_FOCUS]			= GET;
	DefaultMouseCursor[__OBJGROUP_SAPHIRE_FOCUS]			= GET;
	DefaultMouseCursor[__OBJGROUP_EYE_OF_TIGER]				= GET;
	DefaultMouseCursor[__OBJGROUP_MANA_STONE]				= GET;
	DefaultMouseCursor[__OBJGROUP_BLUE_STONE]				= GET;
	DefaultMouseCursor[__OBJGROUP_REDFEATHER]     			= GET;
	DefaultMouseCursor[__OBJGROUP_FIREFLAIL]     			= GET;
	DefaultMouseCursor[__OBJGROUP_SPIKEDLEATHER]   			= GET;
	DefaultMouseCursor[__OBJGROUP_DARKWINGS]                = GET;
    DefaultMouseCursor[__OBJGROUP_SERAPH_NEUTRAL_WINGS]     = GET; 
	DefaultMouseCursor[__OBJGROUP_SERAPH_RED_WINGS]		    = GET; 
	DefaultMouseCursor[__OBJGROUP_SERAPH_GOLD_WINGS]	    = GET; 
	DefaultMouseCursor[__OBJGROUP_SERAPH_DARKRED_WINGS]	    = GET; 
	DefaultMouseCursor[__OBJGROUP_SERAPH_DARKGOLD_WINGS]    = GET; 
	DefaultMouseCursor[__OBJGROUP_SERAPH_LIGHTBLUE_WINGS]   = GET; 
	DefaultMouseCursor[__OBJGROUP_SERAPH_ORANGE_WINGS]	    = GET; 
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_BLUE]          = GET;
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_GREEN]         = GET;
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_DARKBLUE]      = GET;
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_VIOLET]        = GET;
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_PURPLE]        = GET;
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_YELLOW]        = GET;
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_GRAY]          = GET;
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_MAUVE]         = GET;
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_ORANGE]        = GET;
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_BRUN]          = GET;
	DefaultMouseCursor[__OBJGROUP_FWHITEROBE_DARK]          = GET;
	// Added By Tiamat
	DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_GOLD]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_GLOVE_GOLD]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_GOLD]      = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_BOOT_GOLD]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_LEGS_GOLD]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_HELM_GOLD]                = GET;
	DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_SILVER]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_GLOVE_SILVER]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_SILVER]      = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_BOOT_SILVER]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_LEGS_SILVER]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_HELM_SILVER]                = GET;
	DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_BLACK]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_GLOVE_BLACK]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLACK]      = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_BOOT_BLACK]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_LEGS_BLACK]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_HELM_BLACK]                = GET;
	DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_BLUE]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_GLOVE_BLUE]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLUE]      = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_BOOT_BLUE]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_LEGS_BLUE]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_HELM_BLUE]                = GET;
	DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_GREEN]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_GLOVE_GREEN]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_GREEN]      = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_BOOT_GREEN]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_LEGS_GREEN]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_HELM_GREEN]                = GET;
	DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_RED]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_GLOVE_RED]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_RED]      = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_BOOT_RED]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_LEGS_RED]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_HELM_RED]                = GET;
	DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_PURPLE]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_GLOVE_PURPLE]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_PURPLE]      = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_BOOT_PURPLE]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_LEGS_PURPLE]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_HELM_PURPLE]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_EMERALD]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_GLOVE_EMERALD]               = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_EMERALD]      = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_BOOT_EMERALD]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_LEGS_EMERALD]                = GET;
    DefaultMouseCursor[__OBJGROUP_PLATE_HELM_EMERALD]                = GET;
    DefaultMouseCursor[__OBJGROUP_ORGUE]							 = NONE;
    DefaultMouseCursor[__OBJGROUP_TIGERAXE]							 = GET;
    DefaultMouseCursor[__OBJGROUP_ANCIENTAXE]						 = GET;
    DefaultMouseCursor[__OBJGROUP_FLOWERS]							 = GET;
    DefaultMouseCursor[__OBJGROUP_NECROSTAFF]						 = GET;
        // Lucifer
    DefaultMouseCursor[__OBJGROUP_TREE1]							= NONE;
    DefaultMouseCursor[__OBJGROUP_GREENCAPE]               = GET;
    DefaultMouseCursor[__OBJGROUP_YELLOWCAPE]               = GET;
    DefaultMouseCursor[__OBJGROUP_SILVERCAPE]               = GET;
    DefaultMouseCursor[__OBJGROUP_PURPLECAPE]               = GET;
    DefaultMouseCursor[__OBJGROUP_BLACKCAPE]               = GET;
    DefaultMouseCursor[__OBJGROUP_AQUACAPE]               = GET;
    DefaultMouseCursor[__OBJGROUP_BLUECAPE]               = GET;

    
    DefaultMouseCursor[__OBJGROUP_WITCH_HAT1]             = GET;
    DefaultMouseCursor[__OBJGROUP_WITCH_HAT2]             = GET;
    DefaultMouseCursor[__OBJGROUP_WITCH_HAT3]             = GET;
    DefaultMouseCursor[__OBJGROUP_WITCH_HAT4]             = GET;
    DefaultMouseCursor[__OBJGROUP_WITCH_HAT5]             = GET;
    DefaultMouseCursor[__OBJGROUP_NOEL_HAT]               = GET;

    DefaultMouseCursor[__OBJGROUP_NEW_BOW01]              = GET;
    DefaultMouseCursor[__OBJGROUP_DWARF_HAMMER]           = GET;
    DefaultMouseCursor[__OBJGROUP_SWORD_ANGEL]            = GET;
    DefaultMouseCursor[__OBJGROUP_CLAY2BLADES]            = GET;
    DefaultMouseCursor[__OBJGROUP_SWORD_HERO]             = GET;
    DefaultMouseCursor[__OBJGROUP_AXE_DESTRUCTION]        = GET;
    DefaultMouseCursor[__OBJGROUP_BUTTERFLY_WING1]        = GET;
    DefaultMouseCursor[__OBJGROUP_BUTTERFLY_WING2]        = GET;
    DefaultMouseCursor[__OBJGROUP_BUTTERFLY_WING3]        = GET;
    DefaultMouseCursor[__OBJGROUP_BUTTERFLY_WING4]        = GET;
    DefaultMouseCursor[__OBJGROUP_HAMM]                   = GET;
    DefaultMouseCursor[__OBJGROUP_ARCHWINGS_WHITE]        = GET;
    DefaultMouseCursor[__OBJGROUP_LICHROBE]               = GET;
    DefaultMouseCursor[__OBJGROUP_ARMORED_ROBE]           = GET;
    
    DefaultMouseCursor[__OBJGROUP_V2_HAUME01]             = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAUME02]             = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAUME03]             = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAUME04]             = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAUME04_BL]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAT01]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_MASK01]              = GET;

    DefaultMouseCursor[__OBJGROUP_V2_DAGGER_VIP]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_DAGGER01  ]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_DAGGER02  ]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_DAGGER03  ]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_DAGGER04  ]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_DAGGER05  ]          = GET;
    
    //NM_TAG_ADD_SWORD
    DefaultMouseCursor[__OBJGROUP_V2_BUSSWORD01   ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BUSSWORD02   ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BUSSWORD03   ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_CLAYSWORD01  ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_CLAYSWORD02  ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_GLAIVESWORD01]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_GLAIVESWORD02]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SWORD01      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SWORD02      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SWORD03      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SWORD04      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SWORD05      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SWORD06      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SWORD07      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SWORD08      ]       = GET;

    DefaultMouseCursor[__OBJGROUP_V2_2BUSSWORD01   ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2BUSSWORD02   ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2BUSSWORD03   ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2CLAYSWORD01  ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2CLAYSWORD02  ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2GLAIVESWORD01]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2GLAIVESWORD02]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SWORD01      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SWORD02      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SWORD03      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SWORD04      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SWORD05      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SWORD06      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SWORD07      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SWORD08      ]      = GET;

    DefaultMouseCursor[__OBJGROUP_V2_HACHE01       ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HACHE02       ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HACHE03       ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HACHE04       ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HACHE05       ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HACHE06       ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HACHE07       ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HACHE08       ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HACHE09       ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HACHE10       ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HACHE11       ]      = GET;

    DefaultMouseCursor[__OBJGROUP_V2_2HACHE01      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2HACHE02      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2HACHE03      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2HACHE04      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2HACHE05      ]      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2HACHE06      ]      = GET;

    DefaultMouseCursor[__OBJGROUP_V2_2DAGGER01  ]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2DAGGER02  ]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2DAGGER03  ]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2DAGGER04  ]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2DAGGER05  ]          = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2DAGGER06  ]          = GET;
    
    DefaultMouseCursor[__OBJGROUP_V2_BOW01]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BOW02]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BOW03]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BOW04]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BOW05]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BOW06]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BOW07]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BOW08]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BOW09]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_BOW10]               = GET;

    DefaultMouseCursor[__OBJGROUP_V2_SCEPTRE01]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SCEPTRE02]               = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SCEPTRE03]               = GET;

    DefaultMouseCursor[__OBJGROUP_V2_SP01      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SP02      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SP03      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SP04      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SP05      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SP06      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_SP07      ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SP01     ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SP02     ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SP03     ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SP04     ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SP05     ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2SP06     ]       = GET;

    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR2_CLOSED]  	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR2_OPENED]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR2_CLOSED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR2_OPENED_I]	   = USE;

    DefaultMouseCursor[__OBJGROUP_V2_HAMMER01  ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAMMER02  ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAMMER03  ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAMMER04  ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAMMER05  ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAMMER06  ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_HAMMER07  ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2HAMMER01 ]       = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2HAMMER02 ]       = GET;

    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR3_CLOSED]  	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR3_OPENED]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR3_CLOSED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR3_OPENED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR4_CLOSED]  	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR4_OPENED]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR4_CLOSED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR4_OPENED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR5_CLOSED]  	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR5_OPENED]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR5_CLOSED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR5_OPENED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR6_CLOSED]  	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR6_OPENED]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR6_CLOSED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR6_OPENED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR7_CLOSED]  	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR7_OPENED]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR7_CLOSED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR7_OPENED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR8_CLOSED]  	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR8_OPENED]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR8_CLOSED_I]	   = USE;
    DefaultMouseCursor[__OBJGROUP_WOODEN_DOOR8_OPENED_I]	   = USE;

    DefaultMouseCursor[__OBJGROUP_V2_MASQUE_GOBLIN_ICE]		= GET;
    DefaultMouseCursor[__OBJGROUP_V2_BOW05_ICE]			      = GET;
    DefaultMouseCursor[__OBJGROUP_V2_2GLAIVESWORD02_ICE]		= GET;
    DefaultMouseCursor[__OBJGROUP_V2_GLAIVESWORD02_ICE]  	= GET;

    DefaultMouseCursor[__OBJGROUP_MAN_ARMOR01_BODY_GR] 		= GET;
    DefaultMouseCursor[__OBJGROUP_MAN_ARMOR01_BODY_BL] 		= GET;
    DefaultMouseCursor[__OBJGROUP_MAN_ARMOR01_LEGS_GR] 		= GET;
    DefaultMouseCursor[__OBJGROUP_MAN_ARMOR01_LEGS_BL] 		= GET;
    DefaultMouseCursor[__OBJGROUP_MAN_ARMOR01_HAND_GR] 		= GET;
    DefaultMouseCursor[__OBJGROUP_MAN_ARMOR01_HAND_BL] 		= GET;
    DefaultMouseCursor[__OBJGROUP_MAN_ARMOR01_FOOT_GR] 		= GET;
    DefaultMouseCursor[__OBJGROUP_MAN_ARMOR01_FOOT_BL] 		= GET;

    DefaultMouseCursor[__OBJGROUP_T4CP_CASQUE]              = GET;
    DefaultMouseCursor[__OBJGROUP_T4CP_CASQUETTE]           = GET;

    DefaultMouseCursor[__OBJGROUP_T4CP_MATRAQUE]          	= GET;
    DefaultMouseCursor[__OBJGROUP_T4CP_BAT]               	= GET;
    DefaultMouseCursor[__OBJGROUP_T4CP_12]               	= GET;

    DefaultMouseCursor[__OBJGROUP_CAULDRON_CONTAINER]       = TALK;
   

    


   

     //static summable items :
    KEEPALIVE_NM;

    for(int dwCnt = 0;dwCnt<__OBJANIM_LASTGROUP;dwCnt++)
       DefaultMouseCursor[__OBJANIM_FONTAIN_02+dwCnt] = NONE;

    DefaultMouseCursor[__OBJLIGHT_LANTERNE1]  = NONE;
    DefaultMouseCursor[__OBJLIGHT_CHANDELIER] = NONE;
    DefaultMouseCursor[__OBJLIGHT_POUTRE]     = NONE;
    DefaultMouseCursor[__OBJLIGHT_CHANDELLE2] = NONE;

	 // New added by Dialsoft - END
    GetVSFObject(  1)->CreateSprite("ShortSword");
    GetVSFObject(  2)->CreateSprite("NormalSword");
    GetVSFObject(  3)->CreateSprite("Flail");
    GetVSFObject(  4)->CreateSprite("MorningStar");
    GetVSFObject(  5)->CreateSprite("Warhammer");
    GetVSFObject(  6)->CreateSprite("Weapons 7");
    GetVSFObject(  7)->CreateSprite("BattleAxe");
    GetVSFObject(  8)->CreateSprite("LeatherArmorBody");
    GetVSFObject(  9)->CreateSprite("DungeonWell");
    GetVSFObject( 10)->CreateSprite("Chest");
    GetVSFObject( 11)->CreateSprite("LeatherArmorHelm");
    GetVSFObject( 12)->CreateSprite("Misc1 4");
    GetVSFObject( 13)->CreateSprite("Misc1 4"); // Priest Ank
    GetVSFObject( 14)->CreateSprite("Misc1 3");
    GetVSFObject( 15)->CreateSprite("RockDoor1");
    GetVSFObject( 16)->CreateSprite("RockDoor11");
    GetVSFObject( 17)->CreateSprite("Chairs 3");
    GetVSFObject( 18)->CreateSprite("RockDoor2"); 
    GetVSFObject( 19)->CreateSprite("RockDoor3");
    GetVSFObject( 20)->CreateSprite("RockDoor4");
    GetVSFObject( 21)->CreateSprite("RockDoor5");
    GetVSFObject( 22)->CreateSprite("RockDoor6");
    GetVSFObject( 23)->CreateSprite("RockDoor7");
    GetVSFObject( 24)->CreateSprite("RockDoor8");
    GetVSFObject( 25)->CreateSprite("RockDoor9");
    GetVSFObject( 26)->CreateSprite("RockDoor10");
    VObject[27].CreateSprite("Misc 1 - Part 1 1");
    VObject[28].CreateSprite("Misc 1 - Part 1 2");
    VObject[29].CreateSprite("Misc 1 - Part 1 3");
    VObject[30].CreateSprite("Misc 1 - Part 1 4");
    VObject[31].CreateSprite("Misc 1 - Part 1 5");
    VObject[32].CreateSprite("Misc 1 - Part 1 6");
    VObject[33].CreateSprite("Misc 1 - Part 1 7");
    VObject[34].CreateSprite("Misc 1 - Part 2 1");
    VObject[35].CreateSprite("Misc 1 - Part 2 2");
    VObject[36].CreateSprite("Misc 1 - Part 2 3");
    VObject[37].CreateSprite("Misc 1 - Part 2 4");
    VObject[38].CreateSprite("Misc 1 - Part 2 5");
    VObject[39].CreateSprite("Misc 1 - Part 2 6");
    VObject[40].CreateSprite("Misc 1 - Part 2 7");
    VObject[41].CreateSprite("Misc 2 - All 1");
    VObject[42].CreateSprite("Misc 2 - All 2");
    VObject[43].CreateSprite("Misc 2 - All 3");
    VObject[44].CreateSprite("Misc 2 - All 4");
    VObject[45].CreateSprite("Misc 3 - Part 1 1");
    VObject[46].CreateSprite("Misc 3 - Part 1 2");
    VObject[47].CreateSprite("Misc 3 - Part 1 3");
    VObject[48].CreateSprite("Misc 3 - Part 1 4");
    VObject[49].CreateSprite("Misc 3 - Part 1 5");
    VObject[50].CreateSprite("Misc 3 - Part 1 6");
    VObject[51].CreateSprite("Misc 3 - Part 1 7");
    VObject[52].CreateSprite("Misc 3 - Part 1 8");
    VObject[53].CreateSprite("Misc 3 - Part 1 9");
    VObject[54].CreateSprite("Misc 3 - Part 2 1");
    VObject[55].CreateSprite("Misc 3 - Part 2 2");
    VObject[56].CreateSprite("Misc 3 - Part 2 3");
    VObject[57].CreateSprite("Misc 3 - Part 2 4");
    VObject[58].CreateSprite("Misc 3 - Part 2 5");
    VObject[59].CreateSprite("Misc 3 - Part 2 6");
    VObject[60].CreateSprite("Misc 4 - Part 1 1");
    VObject[61].CreateSprite("Misc 4 - Part 1 2");
    VObject[62].CreateSprite("Misc 4 - Part 1 3");
    VObject[63].CreateSprite("Misc 4 - Part 1 4");
    VObject[64].CreateSprite("Misc 4 - Part 1 5");
    VObject[65].CreateSprite("Misc 4 - Part 1 6");
    VObject[66].CreateSprite("Misc 4 - Part 1 7");
    VObject[67].CreateSprite("Misc 4 - Part 1 8");
    VObject[68].CreateSprite("Misc 4 - Part 1 9");
    VObject[69].CreateSprite("Misc 4 - Part 1 10");
    VObject[70].CreateSprite("Misc 4 - Part 2 1");
    VObject[71].CreateSprite("Misc 4 - Part 2 3");
    VObject[72].CreateSprite("Misc 4 - Part 2 4");
    VObject[73].CreateSprite("Misc 6 - All 1");
    VObject[74].CreateSprite("Misc 6 - All 2");
    VObject[75].CreateSprite("Misc 6 - All 3");
    VObject[76].CreateSprite("Misc 6 - All 4");
    VObject[77].CreateSprite("Misc 6 - All 5");
    VObject[78].CreateSprite("Misc 6 - All 6");
    VObject[79].CreateSprite("Misc 6 - All 7");
    VObject[80].CreateSprite("Misc 6 - All 8");
    VObject[81].CreateSprite("Misc 7 - All 1");
    KEEPALIVE_NM;
    VObject[82].CreateSprite("Misc 7 - All 2");
    VObject[83].CreateSprite("Misc 7 - All 3");
    VObject[84].CreateSprite("Misc 7 - All 4");
    VObject[85].CreateSprite("Misc 7 - All 5");
    VObject[86].CreateSprite("64kItemGrShortBow");
    VObject[87].CreateSprite("Misc 7 - All 7");
    VObject[88].CreateSprite("Misc 7 - All 9");
    VObject[89].CreateSprite("Misc 7 - All 10");
    VObject[90].CreateSprite("Misc 8 - All 1");
    VObject[91].CreateSprite("Misc 8 - All 2");
    VObject[92].CreateSprite("Misc 8 - All 5");
    VObject[93].CreateSprite("Misc 8 - All 6");
    VObject[94].CreateSprite("Misc 8 - All 7");
    VObject[95].CreateSprite("Misc 8 - All 8");
    VObject[96].CreateSprite("Misc 9 - All 1");
    VObject[97].CreateSprite("Misc 9 - All 2");
    VObject[98].CreateSprite("Misc 9 - All 3");
    VObject[99].CreateSprite("Misc 9 - All 4");
    VObject[100].CreateSprite("Misc 9 - All 5");
    VObject[101].CreateSprite("Misc 9 - All 6");
    VObject[102].CreateSprite("Misc 9 - All 7");
    VObject[103].CreateSprite("Misc 9 - All 8");
    VObject[104].CreateSprite("Chairs 2");
    VObject[105].CreateSprite("Chairs 4");
    VObject[106].CreateSprite("Chairs 5");
    VObject[107].CreateSprite("Crates 1");
    VObject[108].CreateSprite("Crates 2");
    VObject[109].CreateSprite("Crates 3");
    VObject[110].CreateSprite("Kitchen 2");
    VObject[111].CreateSprite("Kitchen 3");
    VObject[112].CreateSprite("Kitchen 4");
    VObject[113].CreateSprite("Kitchen 5");
    VObject[114].CreateSprite("Kitchen 6");
    VObject[115].CreateSprite("Lights 1");
    VObject[116].CreateSprite("Lights 2");
    VObject[117].CreateSprite("Mirrors 1");
    VObject[118].CreateSprite("WoodenStaff");
    VObject[119].CreateSprite("Mace");
    VObject[120].CreateSprite("Spiked Mace");
    VObject[121].CreateSprite("Maul");
    VObject[122].CreateSprite("Double Axe");
    VObject[123].CreateSprite("Single Axe");     
    VObject[124].CreateSprite("BattleSword");
    VObject[125].CreateSprite("Spear Axe");
    VObject[126].CreateSprite("Misc1 1");
    VObject[127].CreateSprite("Misc1 2");
    VObject[128].CreateSprite("Misc1 5");
    VObject[129].CreateSprite("Misc1 6");
    VObject[130].CreateSprite("BrickFrame2");
    //	VObject[131].CreateSprite("Wall7tm 2 1"); 
    //	VObject[132].CreateSprite("Wall7tm 2 1");
    //	VObject[131].CreateSprite("StuccoMainWallDoor 1");
    //	VObject[132].CreateSprite("StuccoDarkWallDoor 1");
    VObject[135].CreateSprite("Shito 3");
    VObject[136].CreateSprite("Shito 4");
    VObject[137].CreateSprite("Shito 5");
    VObject[141].CreateSprite("Pouch");
    VObject[142].CreateSprite("Rope");
    VObject[143].CreateSprite("StuffedBaskets 1");
    VObject[144].CreateSprite("StuffedBaskets 2");
    VObject[145].CreateSprite("StuffedBaskets 3");
    VObject[146].CreateSprite("Fruits 1");
    VObject[147].CreateSprite("Fruits 2");
    VObject[148].CreateSprite("Fruits 3");
    VObject[149].CreateSprite("Fruits 4");
    VObject[150].CreateSprite("Fruits 5");
    VObject[151].CreateSprite("Fruits 6");
    VObject[152].CreateSprite("Fruits 7");
    VObject[153].CreateSprite("Fruits 8");
    VObject[154].CreateSprite("Fruits 9");
    VObject[155].CreateSprite("Fruits 10");
    VObject[156].CreateSprite("Fruits 11");
    VObject[157].CreateSprite("Fruits 12");
    VObject[158].CreateSprite("Fruits 13");
    VObject[159].CreateSprite("Fruits 14");
    VObject[160].CreateSprite("Coins 1");
    VObject[161].CreateSprite("Coins 2");
    VObject[162].CreateSprite("Coins 3");
    VObject[163].CreateSprite("DecayingBatWings");
    VObject[164].CreateSprite("Gems 1");
    VObject[165].CreateSprite("Gems 2");
    VObject[166].CreateSprite("Gems 3");
    VObject[167].CreateSprite("Gems 4");
    VObject[168].CreateSprite("Gems 5");
    VObject[169].CreateSprite("Gems 6");
    VObject[170].CreateSprite("Gems 7");
    VObject[171].CreateSprite("KoboldHairs");
    VObject[172].CreateSprite("Necklace 1");
    VObject[173].CreateSprite("Necklace 2");
    VObject[174].CreateSprite("Necklace 3");
    VObject[175].CreateSprite("OrcFeet");
    VObject[176].CreateSprite("Rings 1");
    VObject[177].CreateSprite("Rings 2");
    VObject[178].CreateSprite("Rings 3");
    VObject[179].CreateSprite("Rings 4");
    VObject[180].CreateSprite("Rings 5");
    VObject[181].CreateSprite("SkeletonBone");
    VObject[182].CreateSprite("SpiderEyes");
    VObject[183].CreateSprite("Padded 1");
    VObject[184].CreateSprite("Padded 2");
    VObject[185].CreateSprite("Padded 3");
    VObject[186].CreateSprite("Padded 4");
    VObject[187].CreateSprite("Padded 5");
    VObject[188].CreateSprite("Scale");
    VObject[189].CreateSprite("Lever");
    VObject[190].CreateSprite("Camp1");
    VObject[191].CreateSprite("Camp2");
    VObject[192].CreateSprite("Camp3");
    VObject[193].CreateSprite("Camp4");
    VObject[194].CreateSprite("Camp5");
    VObject[195].CreateSprite("Camp6");
    VObject[196].CreateSprite("Camp7");
    VObject[197].CreateSprite("Camp8");
    VObject[198].CreateSprite("Camp9");
    VObject[199].CreateSprite("Camp10");
    VObject[200].CreateSprite("Camp11");

    KEEPALIVE_NM;

    
    VObject[201].CreateSprite("CampFireBurnt");
    VObject[202].CreateSprite("Glinting Sword");
    VObject[203].CreateSprite("DestinyGem");
    VObject[204].CreateSprite("VolcanoRock");
    VObject[205].CreateSprite("RingMail 1");
    VObject[206].CreateSprite("RingMail 4");
    VObject[207].CreateSprite("RingMail 2");
    VObject[208].CreateSprite("RingMail 3");
    VObject[209].CreateSprite("Feather");
    VObject[210].CreateSprite("IceShard");
    VObject[211].CreateSprite("LifeGem");
    VObject[212].CreateSprite("OakTreeLeaf");
    VObject[213].CreateSprite("Padded 6");
    VObject[214].CreateSprite("DungeonTorch-a");
    VObject[215].CreateSprite("DungeonTorch-b");
    VObject[216].CreateSprite("DungeonTorch-c");
    VObject[217].CreateSprite("DungeonTorch-d");
    VObject[218].CreateSprite("DungeonTorch-e");
    VObject[219].CreateSprite("DungeonTorch-f");
    VObject[220].CreateSprite("DungeonTorch-g");
    VObject[221].CreateSprite("DungeonTorch-h");
    VObject[222].CreateSprite("DungeonTorch-i");
    VObject[223].CreateSprite("DungeonTorch-j");
    VObject[224].CreateSprite("DungeonTorch-k");
    VObject[225].CreateSprite("DungeonTorch-l");
    VObject[226].CreateSprite("DungeonTorch-m");
    VObject[227].CreateSprite("DungeonTorch-n");
    VObject[228].CreateSprite("DungeonTorch-o");
    VObject[229].CreateSprite("DungeonTorch-p");
    VObject[230].CreateSprite("DungeonTorch-q");
    VObject[231].CreateSprite("DungeonTorch-r");
    VObject[232].CreateSprite("DungeonTorch-s");
    VObject[233].CreateSprite("DungeonTorch-t");
    VObject[234].CreateSprite("DungeonTorch-u");
    VObject[235].CreateSprite("Belt");
    VObject[236].CreateSprite("BlueFlask");
    VObject[237].CreateSprite("Bracelet");
    VObject[238].CreateSprite("Chest");
    VObject[239].CreateSprite("PinkLeaf");
    VObject[240].CreateSprite("Potion 1");    //(rouge)
    VObject[241].CreateSprite("Potion 2");    //(verte)
    VObject[242].CreateSprite("Shield");
    VObject[243].CreateSprite("StoneLife");
    VObject[244].CreateSprite("Torch");
    VObject[245].CreateSprite("Tooth");
    VObject[246].CreateSprite("GodlyPotion 1");
    VObject[247].CreateSprite("GodlyPotion 2");
    VObject[248].CreateSprite("GodlyPotion 3");
    VObject[249].CreateSprite("GodlyPotion 4");
    VObject[250].CreateSprite("Potions 8");
    VObject[251].CreateSprite("Potions 9");
    VObject[252].CreateSprite("Potions 2");
    VObject[253].CreateSprite("Potions 3");
    VObject[254].CreateSprite("Potions 4");
    VObject[255].CreateSprite("Potions 5");
    VObject[256].CreateSprite("Potions 1");
    VObject[257].CreateSprite("Potions 7");
    VObject[258].CreateSprite("Potions 6");
    VObject[259].CreateSprite("LeatherArmorGloves");
    VObject[260].CreateSprite("LeatherArmorBoots");
    VObject[261].CreateSprite("LeatherArmorLegs");
    VObject[262].CreateSprite("PlateArmorBody");
    VObject[263].CreateSprite("PlateGlove");
    VObject[264].CreateSprite("PlateArmorSleeves");
    VObject[265].CreateSprite("PlateArmorFeet");
    VObject[266].CreateSprite("PlateArmorLegs");
    VObject[267].CreateSprite("PlateArmorHelm");
    VObject[268].CreateSprite("ChainMailLegs");
    VObject[269].CreateSprite("ChainMailBody");
    VObject[270].CreateSprite("ChainMailHelm");
    VObject[271].CreateSprite("DarkSword");
    VObject[272].CreateSprite("RomanShield");
    KEEPALIVE_NM;
    VObject[273].CreateSprite("BarossaShield");
    VObject[274].CreateSprite("BattleDagger");
    VObject[275].CreateSprite("RealDarkSword");
    VObject[276].CreateSprite("HornedHelmet");
    VObject[277].CreateSprite("BattleSword");
    VObject[278].CreateSprite("NecromanRobe");
    VObject[279].CreateSprite("GoldenCrown");
    VObject[280].CreateSprite("GoldenMorningStar");
    VObject[281].CreateSprite("ElvenHat");
    VObject[282].CreateSprite("OrcShield");
    VObject[283].CreateSprite("StuddedLeatherBody");
    VObject[284].CreateSprite("ClothSet1Legs");
    VObject[285].CreateSprite("ClothSet1Body");
    VObject[286].CreateSprite("StuddedLeatherLegs");
    VObject[287].CreateSprite("RedCape");
    VObject[288].CreateSprite("BlackLeatherBoots");
    VObject[289].CreateSprite("Head");
    VObject[290].CreateSprite("Horloge Solaire");
    VObject[291].CreateSprite("Sign1");
    VObject[292].CreateSprite("Sign2");
    VObject[293].CreateSprite("Sign3");
    VObject[294].CreateSprite("LichStaff");
    VObject[295].CreateSprite("GemStaff");
    VObject[296].CreateSprite("SimpleStaff");
    VObject[297].CreateSprite("SimplePortal-a");
    VObject[298].CreateSprite("SimplePortal-b");
    VObject[299].CreateSprite("SimplePortal-c");
    VObject[300].CreateSprite("SimplePortal-d");
    VObject[301].CreateSprite("SimplePortal-e");
    VObject[302].CreateSprite("SimplePortal-f");
    VObject[303].CreateSprite("SimplePortal-g");
    VObject[304].CreateSprite("SimplePortal-h");
    VObject[305].CreateSprite("SimplePortal-i");
    VObject[306].CreateSprite("SimplePortal-j");
    VObject[307].CreateSprite("SimplePortal-k");
    VObject[308].CreateSprite("SimplePortal-l");
    VObject[309].CreateSprite("SimplePortal-m");
    VObject[310].CreateSprite("SimplePortal-n");
    VObject[311].CreateSprite("SimplePortal-o");
    VObject[312].CreateSprite("SimplePortal-p");
    VObject[313].CreateSprite("SimplePortal-q");
    VObject[314].CreateSprite("SimplePortal-r");
    VObject[315].CreateSprite("SimplePortal-s");
    VObject[316].CreateSprite("SimplePortal-t");
    VObject[317].CreateSprite("SimplePortal-u");
    VObject[318].CreateSprite("SimplePortal-v");
    VObject[319].CreateSprite("SimplePortal-w");
    VObject[320].CreateSprite("SimplePortal-x");
    VObject[321].CreateSprite("SimplePortal-y");
    VObject[322].CreateSprite("SimplePortal-z");
    VObject[323].CreateSprite("Vault");
    VObject[324].CreateSprite("SmallTree1");
    VObject[325].CreateSprite("KraanianEgg");
    VObject[326].CreateSprite("WolfPelt");
    VObject[328].CreateSprite("HumanFoot");
    VObject[329].CreateSprite("TrollBadge");
    VObject[330].CreateSprite("64kInnShopSign-a");
    VObject[331].CreateSprite("64kInnShopSign-b");
    VObject[332].CreateSprite("64kInnShopSign-c");
    VObject[333].CreateSprite("64kInnShopSign-d");
    VObject[334].CreateSprite("64kInnShopSign-e");
    VObject[335].CreateSprite("64kInnShopSign-f");
    VObject[336].CreateSprite("64kInnShopSign-g");
    VObject[337].CreateSprite("64kInnShopSign-h");
    VObject[338].CreateSprite("64kInnShopSign-i");
    VObject[339].CreateSprite("64kInnShopSign-j");
    VObject[340].CreateSprite("64kInnShopSign-k");
    VObject[341].CreateSprite("64kInnShopSign-l");
    VObject[342].CreateSprite("64kInnShopSign-m");
    VObject[343].CreateSprite("64kInnShopSign-n");
    VObject[344].CreateSprite("64kInnShopSign-o");
    VObject[345].CreateSprite("64kInnShopSign-p");
    VObject[346].CreateSprite("64kInnShopSign-q");
    VObject[347].CreateSprite("64kPotionShopSign-a");
    VObject[348].CreateSprite("64kPotionShopSign-b");
    VObject[349].CreateSprite("64kPotionShopSign-c");
    VObject[350].CreateSprite("64kPotionShopSign-d");
    VObject[351].CreateSprite("64kPotionShopSign-e");
    VObject[352].CreateSprite("64kPotionShopSign-f");
    VObject[353].CreateSprite("64kPotionShopSign-g");
    VObject[354].CreateSprite("64kPotionShopSign-h");
    VObject[355].CreateSprite("64kPotionShopSign-i");
    VObject[356].CreateSprite("64kPotionShopSign-j");
    VObject[357].CreateSprite("64kPotionShopSign-k");
    VObject[358].CreateSprite("64kPotionShopSign-l");
    VObject[359].CreateSprite("64kPotionShopSign-m");
    VObject[360].CreateSprite("64kPawnShopSign-a");
    VObject[361].CreateSprite("64kPawnShopSign-b");
    VObject[362].CreateSprite("64kPawnShopSign-c");
    VObject[363].CreateSprite("64kPawnShopSign-d");
    VObject[364].CreateSprite("64kPawnShopSign-e");
    VObject[365].CreateSprite("64kPawnShopSign-f");
    VObject[366].CreateSprite("64kPawnShopSign-g");
    VObject[367].CreateSprite("64kPawnShopSign-h");
    VObject[368].CreateSprite("64kPawnShopSign-i");
    VObject[369].CreateSprite("64kPawnShopSign-j");
    VObject[370].CreateSprite("64kPawnShopSign-k");
    VObject[371].CreateSprite("64kPawnShopSign-l");
    VObject[372].CreateSprite("64kPawnShopSign-m");
    VObject[373].CreateSprite("64kArmorShopSign-a");
    VObject[374].CreateSprite("64kArmorShopSign-b");
    VObject[375].CreateSprite("64kArmorShopSign-c");
    VObject[376].CreateSprite("64kArmorShopSign-d");
    VObject[377].CreateSprite("64kArmorShopSign-e");
    VObject[378].CreateSprite("64kArmorShopSign-f");
    VObject[379].CreateSprite("64kArmorShopSign-g");
    VObject[380].CreateSprite("64kArmorShopSign-h");
    VObject[381].CreateSprite("64kArmorShopSign-i");
    VObject[382].CreateSprite("64kArmorShopSign-j");
    VObject[383].CreateSprite("64kArmorShopSign-k");
    VObject[384].CreateSprite("64kArmorShopSign-l");
    VObject[385].CreateSprite("64kArmorShopSign-m");
    VObject[386].CreateSprite("64kArmorShopSign-n");
    VObject[387].CreateSprite("64kArmorShopSign-o");
    VObject[388].CreateSprite("64kArmorShopSign-p");
    VObject[389].CreateSprite("64kArmorShopSign-q");
    VObject[390].CreateSprite("64kWeaponShopSign-a");
    VObject[391].CreateSprite("64kWeaponShopSign-b");
    VObject[392].CreateSprite("64kWeaponShopSign-c");
    VObject[393].CreateSprite("64kWeaponShopSign-d");
    VObject[394].CreateSprite("64kWeaponShopSign-e");
    VObject[395].CreateSprite("64kWeaponShopSign-f");
    VObject[396].CreateSprite("64kWeaponShopSign-g");
    VObject[397].CreateSprite("64kWeaponShopSign-h");
    VObject[398].CreateSprite("64kWeaponShopSign-i");
    VObject[399].CreateSprite("64kWeaponShopSign-j");
    VObject[400].CreateSprite("64kWeaponShopSign-k");

    KEEPALIVE_NM;

    
    VObject[401].CreateSprite("64kWeaponShopSign-l");
    VObject[402].CreateSprite("64kWeaponShopSign-m");
    VObject[403].CreateSprite("64kWeaponShopSign-n");
    VObject[404].CreateSprite("64kWeaponShopSign-o");
    VObject[405].CreateSprite("64kWeaponShopSign-p");
    VObject[406].CreateSprite("64kWeaponShopSign-q");
    VObject[407].CreateSprite("64kWeaponShopSign-r");
    VObject[408].CreateSprite("64kWeaponShopSign-s");
    VObject[409].CreateSprite("64kWeaponShopSign-t");
    VObject[410].CreateSprite("64kWeaponShopSign-u");
    VObject[411].CreateSprite("64kWeaponShopSign-v");
    VObject[412].CreateSprite("64kWeaponShopSign-w");
    VObject[413].CreateSprite("64kWeaponShopSign-x");
    VObject[414].CreateSprite("64kWeaponShopSign-y");
    VObject[415].CreateSprite("64kWeaponShopSign-z");
    VObject[416].CreateSprite("64kWeaponShopSign-2a");
    VObject[417].CreateSprite("64kWeaponShopSign-2b");
    VObject[418].CreateSprite("64kWeaponShopSign-2c");
    VObject[419].CreateSprite("64kWeaponShopSign-2d");
    VObject[420].CreateSprite("64kWeaponShopSign-2e");
    VObject[__OBJGROUP_LARGE_BOW].CreateSprite("64kItemGrLongBow");
    VObject[__OBJGROUP_QUIVER].CreateSprite("64kItemGrQuiver");
    VObject[__OBJGROUP_REDROBE].CreateSprite("64kItemGrRedRobe");
    VObject[__OBJGROUP_MAGEROBE].CreateSprite("64kItemGrMageRobe");
    VObject[__OBJGROUP_WHITEROBE].CreateSprite("64kItemGrWhiteRobe");
    VObject[__OBJGROUP_CHEST_TROLL].CreateSprite("GreenTrollC-k");
    VObject[__OBJGROUP_WHIRLPOOL].CreateSprite("64kSpellGateGround-a");
    VObject[__OBJGROUP_WHIRLPOOL+1].CreateSprite("64kSpellGateGround-b");
    VObject[__OBJGROUP_WHIRLPOOL+2].CreateSprite("64kSpellGateGround-c");
    VObject[__OBJGROUP_WHIRLPOOL+3].CreateSprite("64kSpellGateGround-d");
    VObject[__OBJGROUP_WHIRLPOOL+4].CreateSprite("64kSpellGateGround-e");
    VObject[__OBJGROUP_WHIRLPOOL+5].CreateSprite("64kSpellGateGround-f");
    VObject[__OBJGROUP_WHIRLPOOL+6].CreateSprite("64kSpellGateGround-g");
    VObject[__OBJGROUP_WHIRLPOOL+7].CreateSprite("64kSpellGateGround-h");
    VObject[__OBJGROUP_WHIRLPOOL+8].CreateSprite("64kSpellGateGround-i");
    VObject[__OBJGROUP_WHIRLPOOL+9].CreateSprite("64kSpellGateGround-j");
    VObject[__OBJGROUP_WHIRLPOOL+10].CreateSprite("64kSpellGateGround-k");
    VObject[__OBJGROUP_WHIRLPOOL+11].CreateSprite("64kSpellGateGround-l");
    VObject[__OBJGROUP_WHIRLPOOL+12].CreateSprite("64kSpellGateGround-m");
    VObject[__OBJGROUP_WHIRLPOOL+13].CreateSprite("64kSpellGateGround-n");
    VObject[__OBJGROUP_WHIRLPOOL+14].CreateSprite("64kSpellGateGround-o");
    VObject[__OBJGROUP_WHIRLPOOL+15].CreateSprite("64kSpellGateGround-p");
    VObject[__OBJGROUP_WHIRLPOOL+16].CreateSprite("64kSpellGateGround-q");
    VObject[__OBJGROUP_WHIRLPOOL+17].CreateSprite("64kSpellGateGround-r");
    VObject[__OBJGROUP_WHIRLPOOL+18].CreateSprite("64kSpellGateGround-s");
    VObject[__OBJGROUP_WHIRLPOOL+19].CreateSprite("64kSpellGateGround-t");
    VObject[__OBJGROUP_OGRECLUB].CreateSprite("OgreClub");
    VObject[__OBJGROUP_LARGE_BOW2].CreateSprite("64kItemGrLongBow");
    VObject[__OBJGROUP_FANCY_SHORT_BOW].CreateSprite("64kItemGrShortBow");
    VObject[__OBJGROUP_FANCY_LONG_BOW].CreateSprite("64kItemGrLongBow");
    VObject[__OBJGROUP_BLUE_QUIVER].CreateSprite("64kItemGrQuiver",0,2);
    VObject[__OBJGROUP_YELLOW_QUIVER].CreateSprite("64kItemGrQuiver",0,3);
    VObject[__OBJGROUP_GREEN_QUIVER].CreateSprite("64kItemGrQuiver",0,4);
    VObject[__OBJGROUP_RED_QUIVER].CreateSprite("64kItemGrQuiver",0,5 );
    VObject[__OBJGROUP_BLACK_QUIVER].CreateSprite("64kItemGrQuiver",0,6 );
    VObject[__OBJGROUP_DOOR_TALK].CreateSprite("RockDoor1");
    VObject[__OBJGROUP_DOOR_TALK_I].CreateSprite("RockDoor1");
    VObject[__OBJGROUP_VAULT].CreateSprite("Vault");
    VObject[__OBJGROUP_VAULT_I].CreateSprite("Vault");
    VObject[__OBJGROUP_CENTAUR_SHIELD1].CreateSprite("64kItemGrCentaurShield1");
    VObject[__OBJGROUP_CENTAUR_SHIELD2].CreateSprite("64kItemGrCentaurShield2");
    VObject[__OBJGROUP_SHAMAN_HELM].CreateSprite("64kItemGrShamanHelm");
    VObject[__OBJGROUP_SKAVEN_CLUB].CreateSprite("64kItemGrSkavenClub");
    VObject[__OBJGROUP_SKAVEN_KNIFE].CreateSprite("64kItemGrSkavenKnife");
    VObject[__OBJGROUP_SKAVEN_SHIELD1].CreateSprite("64kItemGrSkavenShield1");
    VObject[__OBJGROUP_SKAVEN_SHIELD2].CreateSprite("64kItemGrSkavenShield2");
    VObject[__OBJGROUP_SKAVEN_SHIELD3].CreateSprite("64kItemGrSkavenShield3");
    VObject[__OBJGROUP_SKELETON_AXE].CreateSprite("64kItemGrSkeletonAxe");
    VObject[__OBJGROUP_SKELETON_HELM].CreateSprite("64kItemGrSkeletonHelm");
    VObject[__OBJGROUP_SKELETON_SHIELD].CreateSprite("64kItemGrSkeletonShield");
    VObject[__OBJGROUP_ORC_SHIELD2].CreateSprite("Ground_OrcShield");
    VObject[__OBJGROUP_SK_SHIELD].CreateSprite("Ground_SkShield");
    VObject[__OBJGROUP_SKELETON_SWORD].CreateSprite("64kItemGrSkeletonSword");
    VObject[__OBJGROUP_SERAPH_BLACK_WINGS].CreateSprite("64kItemGrSeraphBlackWings");
    VObject[__OBJGROUP_SERAPH_WHITE_WINGS].CreateSprite("64kItemGrSeraphWhiteWings");
    VObject[__OBJGROUP_SERAPH_DECHU_WINGS].CreateSprite("Ground_NM2DechuWings");
    VObject[__OBJGROUP_SERAPH_DECHU2_WINGS].CreateSprite("Ground_NM2DechuWings");
    VObject[__OBJGROUP_DARK_GEM].CreateSprite("64kItemGrDarkGem");
    VObject[__OBJGROUP_PURPLE_GEM].CreateSprite("64kItemGrPurpleGem");
    VObject[__OBJGROUP_RED_GEM].CreateSprite("64kItemGrRedGem");
    VObject[__OBJGROUP_WHIRLPOOL2].CreateSprite("64kSpellGroundGate2-a");
    VObject[__OBJGROUP_WHIRLPOOL2+1].CreateSprite("64kSpellGroundGate2-b");
    VObject[__OBJGROUP_WHIRLPOOL2+2].CreateSprite("64kSpellGroundGate2-c");
    VObject[__OBJGROUP_WHIRLPOOL2+3].CreateSprite("64kSpellGroundGate2-d");
    VObject[__OBJGROUP_WHIRLPOOL2+4].CreateSprite("64kSpellGroundGate2-e");
    VObject[__OBJGROUP_WHIRLPOOL2+5].CreateSprite("64kSpellGroundGate2-f");
    VObject[__OBJGROUP_WHIRLPOOL2+6].CreateSprite("64kSpellGroundGate2-g");
    VObject[__OBJGROUP_WHIRLPOOL2+7].CreateSprite("64kSpellGroundGate2-h");
    VObject[__OBJGROUP_WHIRLPOOL2+8].CreateSprite("64kSpellGroundGate2-i");
    VObject[__OBJGROUP_WHIRLPOOL2+9].CreateSprite("64kSpellGroundGate2-j");
    VObject[__OBJGROUP_WHIRLPOOL2+10].CreateSprite("64kSpellGroundGate2-k");
    VObject[__OBJGROUP_WHIRLPOOL2+11].CreateSprite("64kSpellGroundGate2-l");
    VObject[__OBJGROUP_WHIRLPOOL2+12].CreateSprite("64kSpellGroundGate2-m");
    VObject[__OBJGROUP_WHIRLPOOL2+13].CreateSprite("64kSpellGroundGate2-n");
    VObject[__OBJGROUP_WHIRLPOOL2+14].CreateSprite("64kSpellGroundGate2-o");
    VObject[__OBJGROUP_WHIRLPOOL2+15].CreateSprite("64kSpellGroundGate2-p");
    VObject[__OBJGROUP_WHIRLPOOL2+16].CreateSprite("64kSpellGroundGate2-q");
    VObject[__OBJGROUP_WHIRLPOOL2+17].CreateSprite("64kSpellGroundGate2-r");
    VObject[__OBJGROUP_WHIRLPOOL2+18].CreateSprite("64kSpellGroundGate2-s");
    VObject[__OBJGROUP_WHIRLPOOL2+19].CreateSprite("64kSpellGroundGate2-t");
    VObject[__OBJGROUP_COFFIN_TALK].CreateSprite("DungeonTomb1");
    VObject[__OBJGROUP_HALBERD].CreateSprite("64kItemGrHalberd");
    VObject[__OBJGROUP_SMALL_PURPLE_GEM].CreateSprite("64kItemGrPurpleGem");
    VObject[__OBJGROUP_SMALL_RED_GEM].CreateSprite("64kItemGrRedGem");
    VObject[__OBJGROUP_CAULDRON].CreateSprite("64kTrollCauldron-a");
    VObject[__OBJGROUP_CAULDRON+1].CreateSprite("64kTrollCauldron-b");
    VObject[__OBJGROUP_CAULDRON+2].CreateSprite("64kTrollCauldron-c");
    VObject[__OBJGROUP_CAULDRON+3].CreateSprite("64kTrollCauldron-d");
    VObject[__OBJGROUP_CAULDRON+4].CreateSprite("64kTrollCauldron-e");
    VObject[__OBJGROUP_CAULDRON+5].CreateSprite("64kTrollCauldron-f");
    VObject[__OBJGROUP_CAULDRON+6].CreateSprite("64kTrollCauldron-g");
    VObject[__OBJGROUP_CAULDRON+7].CreateSprite("64kTrollCauldron-h");
    VObject[__OBJGROUP_CAULDRON+8].CreateSprite("64kTrollCauldron-i");
    VObject[__OBJGROUP_CAULDRON+9].CreateSprite("64kTrollCauldron-j");
    VObject[__OBJGROUP_CAULDRON+10].CreateSprite("64kTrollCauldron-k");
    VObject[__OBJGROUP_CAULDRON+11].CreateSprite("64kTrollCauldron-l");
    VObject[__OBJGROUP_CAULDRON+12].CreateSprite("64kTrollCauldron-m");
    VObject[__OBJGROUP_CAULDRON+13].CreateSprite("64kTrollCauldron-n");
    VObject[__OBJGROUP_CAULDRON+14].CreateSprite("64kTrollCauldron-o");
    VObject[__OBJGROUP_CAULDRON+15].CreateSprite("64kTrollCauldron-p");
    VObject[__OBJGROUP_CAULDRON+16].CreateSprite("64kTrollCauldron-q");
    VObject[__OBJGROUP_CAULDRON+17].CreateSprite("64kTrollCauldron-r");
    VObject[__OBJGROUP_CAULDRON+18].CreateSprite("64kTrollCauldron-s");
    VObject[__OBJGROUP_CAULDRON+19].CreateSprite("64kTrollCauldron-t");
    VObject[__OBJGROUP_CAULDRON+20].CreateSprite("64kTrollCauldron-u");
    //VObject[__OBJGROUP_CAULDRON_GROUND].CreateSprite("64kTrollCauldronGround-a");
    //VObject[__OBJGROUP_CAULDRON_GROUND+1].CreateSprite("64kTrollCauldronGround-b");
    KEEPALIVE_NM;
    //VObject[__OBJGROUP_CAULDRON_GROUND+2].CreateSprite("64kTrollCauldronGround-c");
    //VObject[__OBJGROUP_CAULDRON_GROUND+3].CreateSprite("64kTrollCauldronGround-d");
    //VObject[__OBJGROUP_CAULDRON_GROUND+4].CreateSprite("64kTrollCauldronGround-e");
    //VObject[__OBJGROUP_CAULDRON_GROUND+5].CreateSprite("64kTrollCauldronGround-f");
    //VObject[__OBJGROUP_CAULDRON_GROUND+6].CreateSprite("64kTrollCauldronGround-g");
    //VObject[__OBJGROUP_CAULDRON_GROUND+7].CreateSprite("64kTrollCauldronGround-h");
    //VObject[__OBJGROUP_CAULDRON_GROUND+8].CreateSprite("64kTrollCauldronGround-i");
    //VObject[__OBJGROUP_CAULDRON_GROUND+9].CreateSprite("64kTrollCauldronGround-j");
    //VObject[__OBJGROUP_CAULDRON_GROUND+10].CreateSprite("64kTrollCauldronGround-k");
    //VObject[__OBJGROUP_CAULDRON_GROUND+11].CreateSprite("64kTrollCauldronGround-l");
    //VObject[__OBJGROUP_CAULDRON_GROUND+12].CreateSprite("64kTrollCauldronGround-m");
    //VObject[__OBJGROUP_CAULDRON_GROUND+13].CreateSprite("64kTrollCauldronGround-n");
    //VObject[__OBJGROUP_CAULDRON_GROUND+14].CreateSprite("64kTrollCauldronGround-o");
    //VObject[__OBJGROUP_CAULDRON_GROUND+15].CreateSprite("64kTrollCauldronGround-p");
    //VObject[__OBJGROUP_CAULDRON_GROUND+16].CreateSprite("64kTrollCauldronGround-q");
    //VObject[__OBJGROUP_CAULDRON_GROUND+17].CreateSprite("64kTrollCauldronGround-r");
    //VObject[__OBJGROUP_CAULDRON_GROUND+18].CreateSprite("64kTrollCauldronGround-s");
    //VObject[__OBJGROUP_CAULDRON_GROUND+19].CreateSprite("64kTrollCauldronGround-t");
    //VObject[__OBJGROUP_CAULDRON_GROUND+20].CreateSprite("64kTrollCauldronGround-u");
    VObject[__OBJGROUP_SKAVEN_CORPSE1].CreateSprite("64kSkavenPeonC-t");
    VObject[__OBJGROUP_SKAVEN_CORPSE2].CreateSprite("64kSkavenShamanC-s");
    VObject[__OBJGROUP_SKAVEN_CORPSE3].CreateSprite("64kSkavenSkavengerC-s");
    VObject[__OBJGROUP_SKAVEN_CORPSE4].CreateSprite("64kSkavenWarriorC-s");
    VObject[__OBJGROUP_SKAVEN_CORPSE_I1].CreateSprite("64kSkavenPeonC-t");
    VObject[__OBJGROUP_SKAVEN_CORPSE_I2].CreateSprite("64kSkavenShamanC-s");
    VObject[__OBJGROUP_SKAVEN_CORPSE_I3].CreateSprite("64kSkavenSkavengerC-s");
    VObject[__OBJGROUP_SKAVEN_CORPSE_I4].CreateSprite("64kSkavenWarriorC-s");
    VObject[__OBJGROUP_RIBCAGE].CreateSprite("Object_Ribcage");
    VObject[__OBJGROUP_HEART].CreateSprite("Object_Ground_Heart");
    // New added by Dialsoft - BEGIN
    VObject[__OBJGROUP_BRACELET_RUBIS].CreateSprite("Bracelet");
    VObject[__OBJGROUP_BELT_JEWELS_BROWN].CreateSprite("GroundBeltBrownJewels");
    VObject[__OBJGROUP_BELT_JEWELS_PURPLE].CreateSprite("GroundBeltVioletJewels");
    VObject[__OBJGROUP_BELT_RED].CreateSprite("GroundBeltRed");
    VObject[__OBJGROUP_BRACELET_AZURE].CreateSprite("Bracelet");
    VObject[__OBJGROUP_BRACELET_SQUARE_WHITE].CreateSprite("GroundBraceletSquareWhite");
    VObject[__OBJGROUP_GEM_AZURE].CreateSprite("GroundGemAzure");
    VObject[__OBJGROUP_GEM_ORANGE].CreateSprite("GroundGemOrange");
    VObject[__OBJGROUP_GEM_PINK].CreateSprite("GroundGemPink");
    VObject[__OBJGROUP_RING_3RUBIS].CreateSprite("GroundRingMulti");
    VObject[__OBJGROUP_RING_RED].CreateSprite("Rings 1");
    VObject[__OBJGROUP_RING_STARGOLD].CreateSprite("Rings 2");
    VObject[__OBJGROUP_MASQUE_GOBLIN].CreateSprite("GroundGobMask");
    VObject[__OBJGROUP_V2_MASQUE_GOBLIN_ICE].CreateSprite("GroundGobMask", 0, 2);
    VObject[__OBJGROUP_RING_CUT_GREEN].CreateSprite("GroundRingCutGreen");
    VObject[__OBJGROUP_RING_CUT_RED].CreateSprite("GroundRingCutRed");
    VObject[__OBJGROUP_RUBIS_FOCUS].CreateSprite("GroundRubisFocus");
    VObject[__OBJGROUP_EMERALD_FOCUS].CreateSprite("GroundEmeraldFocus");
    VObject[__OBJGROUP_DIAMOND_FOCUS].CreateSprite("GroundDiamondFocus");
    VObject[__OBJGROUP_SAPHIRE_FOCUS].CreateSprite("GroundSaphireFocus");
    VObject[__OBJGROUP_EYE_OF_TIGER].CreateSprite("GroundEyeOfTiger");
    VObject[__OBJGROUP_MANA_STONE].CreateSprite("GroundManaStone");
    VObject[__OBJGROUP_BLUE_STONE].CreateSprite("GroundBlueStone");
    VObject[__OBJGROUP_REDFEATHER].CreateSprite("GroundRedFeHelmet");
    VObject[__OBJGROUP_FIREFLAIL].CreateSprite("GroundFireFlail");
    VObject[__OBJGROUP_SPIKEDLEATHER].CreateSprite("LeatherArmorBody"); // ^^^^ spiked leather dont have ground pic yet
    VObject[__OBJGROUP_DARKWINGS].CreateSprite("64kItemGrSeraphBlackWings");
    VObject[__OBJGROUP_SERAPH_NEUTRAL_WINGS].CreateSprite("64kItemGrSeraphBlackWings");
    VObject[__OBJGROUP_SERAPH_RED_WINGS].CreateSprite("64kItemGrSeraphBlackWings");
    VObject[__OBJGROUP_SERAPH_GOLD_WINGS].CreateSprite("64kItemGrSeraphBlackWings");
    VObject[__OBJGROUP_SERAPH_DARKRED_WINGS].CreateSprite("64kItemGrSeraphBlackWings");
    VObject[__OBJGROUP_SERAPH_DARKGOLD_WINGS].CreateSprite("64kItemGrSeraphBlackWings");
    VObject[__OBJGROUP_SERAPH_LIGHTBLUE_WINGS].CreateSprite("64kItemGrSeraphBlackWings");
    VObject[__OBJGROUP_SERAPH_ORANGE_WINGS].CreateSprite("64kItemGrSeraphBlackWings");
    VObject[__OBJGROUP_FWHITEROBE_BLUE].CreateSprite("64kItemGrWhiteRobe", 0, 2);
    VObject[__OBJGROUP_FWHITEROBE_GREEN].CreateSprite("64kItemGrWhiteRobe", 0, 3);
    VObject[__OBJGROUP_FWHITEROBE_DARKBLUE].CreateSprite("64kItemGrWhiteRobe", 0, 4);
    VObject[__OBJGROUP_FWHITEROBE_VIOLET].CreateSprite("64kItemGrWhiteRobe", 0, 5);
    VObject[__OBJGROUP_FWHITEROBE_PURPLE].CreateSprite("64kItemGrWhiteRobe", 0, 6);
    VObject[__OBJGROUP_FWHITEROBE_YELLOW].CreateSprite("64kItemGrWhiteRobe", 0, 7);
    VObject[__OBJGROUP_FWHITEROBE_GRAY].CreateSprite("64kItemGrWhiteRobe", 0, 8);
    VObject[__OBJGROUP_FWHITEROBE_MAUVE].CreateSprite("64kItemGrWhiteRobe", 0, 9);
    VObject[__OBJGROUP_FWHITEROBE_ORANGE].CreateSprite("64kItemGrWhiteRobe", 0, 10);
    VObject[__OBJGROUP_FWHITEROBE_BRUN].CreateSprite("64kItemGrWhiteRobe", 0, 11);
    VObject[__OBJGROUP_FWHITEROBE_DARK].CreateSprite("64kItemGrWhiteRobe", 0, 12);
    //Added by tiamat
    VObject[__OBJGROUP_PLATE_ARMOR_GOLD].CreateSprite("PlateArmorBody", 0, 2);
    VObject[__OBJGROUP_PLATE_GLOVE_GOLD].CreateSprite("PlateGlove", 0, 2);
    VObject[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_GOLD].CreateSprite("PlateArmorSleeves", 0, 2);
    VObject[__OBJGROUP_PLATE_BOOT_GOLD].CreateSprite("PlateArmorFeet", 0, 2);
    VObject[__OBJGROUP_PLATE_LEGS_GOLD].CreateSprite("PlateArmorLegs", 0, 2);
    VObject[__OBJGROUP_PLATE_HELM_GOLD].CreateSprite("PlateArmorHelm", 0, 2);
    VObject[__OBJGROUP_PLATE_ARMOR_SILVER].CreateSprite("PlateArmorBody", 0, 3);
    VObject[__OBJGROUP_PLATE_GLOVE_SILVER].CreateSprite("PlateGlove", 0, 3);
    VObject[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_SILVER].CreateSprite("PlateArmorSleeves", 0, 3);
    VObject[__OBJGROUP_PLATE_BOOT_SILVER].CreateSprite("PlateArmorFeet", 0, 3);
    VObject[__OBJGROUP_PLATE_LEGS_SILVER].CreateSprite("PlateArmorLegs", 0, 3);
    VObject[__OBJGROUP_PLATE_HELM_SILVER].CreateSprite("PlateArmorHelm", 0, 3);
    VObject[__OBJGROUP_PLATE_ARMOR_BLACK].CreateSprite("PlateArmorBody", 0, 4);
    VObject[__OBJGROUP_PLATE_GLOVE_BLACK].CreateSprite("PlateGlove", 0, 4);
    VObject[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLACK].CreateSprite("PlateArmorSleeves", 0, 4);
    VObject[__OBJGROUP_PLATE_BOOT_BLACK].CreateSprite("PlateArmorFeet", 0, 4);
    VObject[__OBJGROUP_PLATE_LEGS_BLACK].CreateSprite("PlateArmorLegs", 0, 4);
    VObject[__OBJGROUP_PLATE_HELM_BLACK].CreateSprite("PlateArmorHelm", 0, 4);
    VObject[__OBJGROUP_PLATE_ARMOR_BLUE].CreateSprite("PlateArmorBody", 0, 5);
    VObject[__OBJGROUP_PLATE_GLOVE_BLUE].CreateSprite("PlateGlove", 0, 5);
    VObject[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLUE].CreateSprite("PlateArmorSleeves", 0, 5);
    VObject[__OBJGROUP_PLATE_BOOT_BLUE].CreateSprite("PlateArmorFeet", 0, 5);
    VObject[__OBJGROUP_PLATE_LEGS_BLUE].CreateSprite("PlateArmorLegs", 0, 5);
    VObject[__OBJGROUP_PLATE_HELM_BLUE].CreateSprite("PlateArmorHelm", 0, 5);
    VObject[__OBJGROUP_PLATE_ARMOR_GREEN].CreateSprite("PlateArmorBody", 0, 6);
    VObject[__OBJGROUP_PLATE_GLOVE_GREEN].CreateSprite("PlateGlove", 0, 6);
    VObject[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_GREEN].CreateSprite("PlateArmorSleeves", 0, 6);
    VObject[__OBJGROUP_PLATE_BOOT_GREEN].CreateSprite("PlateArmorFeet", 0, 6);
    VObject[__OBJGROUP_PLATE_LEGS_GREEN].CreateSprite("PlateArmorLegs", 0, 6);
    VObject[__OBJGROUP_PLATE_HELM_GREEN].CreateSprite("PlateArmorHelm", 0, 6);
    VObject[__OBJGROUP_PLATE_ARMOR_RED].CreateSprite("PlateArmorBody", 0, 7);
    VObject[__OBJGROUP_PLATE_GLOVE_RED].CreateSprite("PlateGlove", 0, 7);
    VObject[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_RED].CreateSprite("PlateArmorSleeves", 0, 7);
    KEEPALIVE_NM;
    VObject[__OBJGROUP_PLATE_BOOT_RED].CreateSprite("PlateArmorFeet", 0, 7);
    VObject[__OBJGROUP_PLATE_LEGS_RED].CreateSprite("PlateArmorLegs", 0, 7);
    VObject[__OBJGROUP_PLATE_HELM_RED].CreateSprite("PlateArmorHelm", 0, 7);
    VObject[__OBJGROUP_PLATE_ARMOR_PURPLE].CreateSprite("PlateArmorBody", 0, 8);
    VObject[__OBJGROUP_PLATE_GLOVE_PURPLE].CreateSprite("PlateGlove", 0, 8);
    VObject[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_PURPLE].CreateSprite("PlateArmorSleeves", 0, 8);
    VObject[__OBJGROUP_PLATE_BOOT_PURPLE].CreateSprite("PlateArmorFeet", 0, 8);
    VObject[__OBJGROUP_PLATE_LEGS_PURPLE].CreateSprite("PlateArmorLegs", 0, 8);
    VObject[__OBJGROUP_PLATE_HELM_PURPLE].CreateSprite("PlateArmorHelm", 0, 8);
    VObject[__OBJGROUP_PLATE_ARMOR_EMERALD].CreateSprite("PlateArmorBody", 0, 9);
    VObject[__OBJGROUP_PLATE_GLOVE_EMERALD].CreateSprite("PlateGlove", 0, 9);
    VObject[__OBJGROUP_PLATE_ARMOR_W_SLEEVE_EMERALD].CreateSprite("PlateArmorSleeves", 0, 9);
    VObject[__OBJGROUP_PLATE_BOOT_EMERALD].CreateSprite("PlateArmorFeet", 0, 9);
    VObject[__OBJGROUP_PLATE_LEGS_EMERALD].CreateSprite("PlateArmorLegs", 0, 9);
    VObject[__OBJGROUP_PLATE_HELM_EMERALD].CreateSprite("PlateArmorHelm", 0, 9);
    VObject[__OBJGROUP_ORGUE].CreateSprite("64kTrollBed");
    VObject[__OBJGROUP_TIGERAXE].CreateSprite("64kItemGrSkeletonAxe");
    VObject[__OBJGROUP_ANCIENTAXE].CreateSprite("64kItemGrSkeletonAxe");
    VObject[__OBJGROUP_FLOWERS].CreateSprite("GroundFireFlail");
    VObject[__OBJGROUP_NECROSTAFF].CreateSprite("LichStaff");
    	//Added by Lucifer
    VObject[__OBJGROUP_GREENCAPE].CreateSprite("RedCape",0,2);
    VObject[__OBJGROUP_YELLOWCAPE].CreateSprite("RedCape",0,3);
    VObject[__OBJGROUP_SILVERCAPE].CreateSprite("RedCape",0,4);
    VObject[__OBJGROUP_PURPLECAPE].CreateSprite("RedCape",0,5);
    VObject[__OBJGROUP_BLACKCAPE].CreateSprite("RedCape",0,6);
    VObject[__OBJGROUP_AQUACAPE].CreateSprite("RedCape",0,7);
    VObject[__OBJGROUP_BLUECAPE].CreateSprite("RedCape",0,8);

    VObject[__OBJGROUP_WITCH_HAT1].CreateSprite("Ground_WitchHat1");
    VObject[__OBJGROUP_WITCH_HAT2].CreateSprite("Ground_WitchHat2");
    VObject[__OBJGROUP_WITCH_HAT3].CreateSprite("Ground_WitchHat3");
    VObject[__OBJGROUP_WITCH_HAT4].CreateSprite("Ground_WitchHat4");
    VObject[__OBJGROUP_WITCH_HAT5].CreateSprite("Ground_WitchHat5");
    VObject[__OBJGROUP_NOEL_HAT]  .CreateSprite("Ground_NoelHat");
    VObject[__OBJGROUP_T4CP_CASQUE]      .CreateSprite("Ground_T4CP_Casque");
    VObject[__OBJGROUP_T4CP_CASQUETTE]   .CreateSprite("Ground_T4CP_Casquette");
    

    VObject[__OBJGROUP_NEW_BOW01   ]   .CreateSprite("Ground_NewBow01");
    VObject[__OBJGROUP_DWARF_HAMMER]   .CreateSprite("Ground_DwarfHammer");
    VObject[__OBJGROUP_AXE_DESTRUCTION].CreateSprite("Ground_AxeDestruction");
    VObject[__OBJGROUP_SWORD_ANGEL ]   .CreateSprite("Ground_SwordAngel");
    VObject[__OBJGROUP_CLAY2BLADES]    .CreateSprite("Ground_Clay2Blades");
    VObject[__OBJGROUP_SWORD_HERO]     .CreateSprite("Ground_SwordAngel");

    VObject[__OBJGROUP_BUTTERFLY_WING1] .CreateSprite("Ground_ButterFlyWing");
    VObject[__OBJGROUP_BUTTERFLY_WING2] .CreateSprite("Ground_ButterFlyWing",0,2);
    VObject[__OBJGROUP_BUTTERFLY_WING3] .CreateSprite("Ground_ButterFlyWing",0,3);
    VObject[__OBJGROUP_BUTTERFLY_WING4] .CreateSprite("Ground_ButterFlyWing",0,4);

    VObject[__OBJGROUP_HAMM]            .CreateSprite("Ground_Hamm");
    VObject[__OBJGROUP_ARCHWINGS_WHITE] .CreateSprite("Ground_ArchWingsW");
    VObject[__OBJGROUP_LICHROBE]        .CreateSprite("Ground_LichRobeOri");//BLBLBL 25 mars 2009 : ajouté un sprite pour l'item au sol, on utilise la robe blanche teinte bleutée
    VObject[__OBJGROUP_ARMORED_ROBE]    .CreateSprite("Ground_ArmoredRobe");

    VObject[__OBJGROUP_V2_HAUME01].CreateSprite("Ground_V2_Haume01");
    VObject[__OBJGROUP_V2_HAUME02].CreateSprite("Ground_V2_Haume02");
    VObject[__OBJGROUP_V2_HAUME03].CreateSprite("Ground_V2_Haume03");
    VObject[__OBJGROUP_V2_HAUME04].CreateSprite("Ground_V2_Haume04");
    VObject[__OBJGROUP_V2_HAUME04_BL].CreateSprite("Ground_V2_Haume04",0,2);
    VObject[__OBJGROUP_V2_HAT01]  .CreateSprite("Ground_V2_Hat01");
    VObject[__OBJGROUP_V2_MASK01] .CreateSprite("Ground_V2_Mask01");

    VObject[__OBJGROUP_V2_DAGGER_VIP].CreateSprite("Ground_V2_Viperine");
    VObject[__OBJGROUP_V2_DAGGER01  ].CreateSprite("Ground_V2_dague01");
    VObject[__OBJGROUP_V2_DAGGER02  ].CreateSprite("Ground_V2_dague02");
    VObject[__OBJGROUP_V2_DAGGER03  ].CreateSprite("Ground_V2_dague03");
    VObject[__OBJGROUP_V2_DAGGER04  ].CreateSprite("Ground_V2_dague04");
    VObject[__OBJGROUP_V2_DAGGER05  ].CreateSprite("Ground_V2_dague05");

    //NM_TAG_ADD_SWORD
    VObject[__OBJGROUP_V2_BUSSWORD01     ].CreateSprite("Ground_V2_BusterSlayer01");
    VObject[__OBJGROUP_V2_BUSSWORD02     ].CreateSprite("Ground_V2_BusterSlayer02");
    VObject[__OBJGROUP_V2_BUSSWORD03     ].CreateSprite("Ground_V2_BusterSlayer03");
    VObject[__OBJGROUP_V2_CLAYSWORD01    ].CreateSprite("Ground_V2_Claymore01");
    VObject[__OBJGROUP_V2_CLAYSWORD02    ].CreateSprite("Ground_V2_Claymore02");
    
    VObject[__OBJGROUP_V2_GLAIVESWORD01    ].CreateSprite("Ground_V2_GlaiveSlayer01");
    VObject[__OBJGROUP_V2_GLAIVESWORD02    ].CreateSprite("Ground_V2_GlaiveSlayer02");
    VObject[__OBJGROUP_V2_GLAIVESWORD02_ICE].CreateSprite("Ground_V2_IceSword01");

    VObject[__OBJGROUP_V2_SWORD01        ].CreateSprite("Ground_V2_Sword01");
    VObject[__OBJGROUP_V2_SWORD02        ].CreateSprite("Ground_V2_Sword02");
    VObject[__OBJGROUP_V2_SWORD03        ].CreateSprite("Ground_V2_Sword03");
    VObject[__OBJGROUP_V2_SWORD04        ].CreateSprite("Ground_V2_Sword04");
    VObject[__OBJGROUP_V2_SWORD05        ].CreateSprite("Ground_V2_Sword05");
    VObject[__OBJGROUP_V2_SWORD06        ].CreateSprite("Ground_V2_Sword06");
    VObject[__OBJGROUP_V2_SWORD07        ].CreateSprite("Ground_V2_Sword07");
    VObject[__OBJGROUP_V2_SWORD08        ].CreateSprite("Ground_V2_Sword08");
  
    VObject[__OBJGROUP_V2_2BUSSWORD01     ].CreateSprite("Ground_V2_BusterSlayer01");
    VObject[__OBJGROUP_V2_2BUSSWORD02     ].CreateSprite("Ground_V2_BusterSlayer02");
    VObject[__OBJGROUP_V2_2BUSSWORD03     ].CreateSprite("Ground_V2_BusterSlayer03");
    VObject[__OBJGROUP_V2_2CLAYSWORD01    ].CreateSprite("Ground_V2_Claymore01");
    VObject[__OBJGROUP_V2_2CLAYSWORD02    ].CreateSprite("Ground_V2_Claymore02");

    VObject[__OBJGROUP_V2_2GLAIVESWORD01    ].CreateSprite("Ground_V2_GlaiveSlayer01");
    VObject[__OBJGROUP_V2_2GLAIVESWORD02    ].CreateSprite("Ground_V2_GlaiveSlayer02");
    VObject[__OBJGROUP_V2_2GLAIVESWORD02_ICE].CreateSprite("Ground_V2_IceSword01");

    VObject[__OBJGROUP_V2_2SWORD01        ].CreateSprite("Ground_V2_Sword01");
    VObject[__OBJGROUP_V2_2SWORD02        ].CreateSprite("Ground_V2_Sword02");
    VObject[__OBJGROUP_V2_2SWORD03        ].CreateSprite("Ground_V2_Sword03");
    VObject[__OBJGROUP_V2_2SWORD04        ].CreateSprite("Ground_V2_Sword04");
    VObject[__OBJGROUP_V2_2SWORD05        ].CreateSprite("Ground_V2_Sword05");
    VObject[__OBJGROUP_V2_2SWORD06        ].CreateSprite("Ground_V2_Sword06");
    VObject[__OBJGROUP_V2_2SWORD07        ].CreateSprite("Ground_V2_Sword07");
    VObject[__OBJGROUP_V2_2SWORD08        ].CreateSprite("Ground_V2_Sword08");

    VObject[__OBJGROUP_V2_HACHE01       ].CreateSprite("Ground_V2_Hache01");
    VObject[__OBJGROUP_V2_HACHE02       ].CreateSprite("Ground_V2_Hache02");
    VObject[__OBJGROUP_V2_HACHE03       ].CreateSprite("Ground_V2_Hache03");
    VObject[__OBJGROUP_V2_HACHE04       ].CreateSprite("Ground_V2_Hache04");
    VObject[__OBJGROUP_V2_HACHE05       ].CreateSprite("Ground_V2_Hache05");
    VObject[__OBJGROUP_V2_HACHE06       ].CreateSprite("Ground_V2_Hache06");
    VObject[__OBJGROUP_V2_HACHE07       ].CreateSprite("Ground_V2_Hache07");
    VObject[__OBJGROUP_V2_HACHE08       ].CreateSprite("Ground_V2_Hache08");
    VObject[__OBJGROUP_V2_HACHE09       ].CreateSprite("Ground_V2_Hache09");
    VObject[__OBJGROUP_V2_HACHE10       ].CreateSprite("Ground_V2_Hache10");
    VObject[__OBJGROUP_V2_HACHE11       ].CreateSprite("Ground_V2_Hache11");

    VObject[__OBJGROUP_V2_2HACHE01      ].CreateSprite("Ground_V2_Hache08");
    VObject[__OBJGROUP_V2_2HACHE02      ].CreateSprite("Ground_V2_Hache02");
    VObject[__OBJGROUP_V2_2HACHE03      ].CreateSprite("Ground_V2_Hache04");
    VObject[__OBJGROUP_V2_2HACHE04      ].CreateSprite("Ground_V2_Hache07");
    VObject[__OBJGROUP_V2_2HACHE05      ].CreateSprite("Ground_V2_Hache06");
    VObject[__OBJGROUP_V2_2HACHE06      ].CreateSprite("Ground_V2_Hache09");


    VObject[__OBJGROUP_V2_2DAGGER01  ].CreateSprite("Ground_V2_dague01");
    VObject[__OBJGROUP_V2_2DAGGER02  ].CreateSprite("Ground_V2_dague02");
    VObject[__OBJGROUP_V2_2DAGGER03  ].CreateSprite("Ground_V2_dague03");
    VObject[__OBJGROUP_V2_2DAGGER04  ].CreateSprite("Ground_V2_dague04");
    VObject[__OBJGROUP_V2_2DAGGER05  ].CreateSprite("Ground_V2_dague05");
    VObject[__OBJGROUP_V2_2DAGGER06  ].CreateSprite("Ground_V2_Viperine");

    VObject[__OBJGROUP_V2_BOW01        ].CreateSprite("Ground_V2_Bow01");
    VObject[__OBJGROUP_V2_BOW02        ].CreateSprite("Ground_V2_Bow02");
    VObject[__OBJGROUP_V2_BOW03        ].CreateSprite("Ground_V2_Bow03");
    VObject[__OBJGROUP_V2_BOW04        ].CreateSprite("Ground_V2_Bow04");
    VObject[__OBJGROUP_V2_BOW05        ].CreateSprite("Ground_V2_Bow05");
    VObject[__OBJGROUP_V2_BOW05_ICE    ].CreateSprite("Ground_V2_IceBow01");
    VObject[__OBJGROUP_V2_BOW06        ].CreateSprite("Ground_V2_Bow06");
    VObject[__OBJGROUP_V2_BOW07        ].CreateSprite("Ground_V2_Bow07");
    VObject[__OBJGROUP_V2_BOW08        ].CreateSprite("Ground_V2_Bow08");
    VObject[__OBJGROUP_V2_BOW09        ].CreateSprite("Ground_V2_Bow09");
    VObject[__OBJGROUP_V2_BOW10        ].CreateSprite("Ground_V2_Bow10");
    KEEPALIVE_NM;
    VObject[__OBJGROUP_V2_SCEPTRE01        ].CreateSprite("Ground_V2_Sceptre01");
    VObject[__OBJGROUP_V2_SCEPTRE02        ].CreateSprite("Ground_V2_Sceptre02");
    VObject[__OBJGROUP_V2_SCEPTRE03        ].CreateSprite("Ground_V2_Sceptre03");

    VObject[__OBJGROUP_V2_SP01      ].CreateSprite("Ground_V2_Sp01");
    VObject[__OBJGROUP_V2_SP02      ].CreateSprite("Ground_V2_Sp02");
    VObject[__OBJGROUP_V2_SP03      ].CreateSprite("Ground_V2_Sp03");
    VObject[__OBJGROUP_V2_SP04      ].CreateSprite("Ground_V2_Sp04");
    VObject[__OBJGROUP_V2_SP05      ].CreateSprite("Ground_V2_Sp05");
    VObject[__OBJGROUP_V2_SP06      ].CreateSprite("Ground_V2_Sp06");
    VObject[__OBJGROUP_V2_SP07      ].CreateSprite("Ground_V2_Sp07");

    VObject[__OBJGROUP_V2_2SP01      ].CreateSprite("Ground_V2_Sp01");
    VObject[__OBJGROUP_V2_2SP02      ].CreateSprite("Ground_V2_Sp02");
    VObject[__OBJGROUP_V2_2SP03      ].CreateSprite("Ground_V2_Sp04");
    VObject[__OBJGROUP_V2_2SP04      ].CreateSprite("Ground_V2_Sp05");
    VObject[__OBJGROUP_V2_2SP05      ].CreateSprite("Ground_V2_Sp06");
    VObject[__OBJGROUP_V2_2SP06      ].CreateSprite("Ground_V2_Sp07");


    VObject[__OBJGROUP_WOODEN_DOOR2_CLOSED].CreateSprite("Rock2Door1"); 
    VObject[__OBJGROUP_WOODEN_DOOR2_OPENED].CreateSprite("Rock2Door11"); 
    VObject[__OBJGROUP_WOODEN_DOOR2_ANIM02].CreateSprite("Rock2Door2");
    VObject[__OBJGROUP_WOODEN_DOOR2_ANIM03].CreateSprite("Rock2Door3"); 
    VObject[__OBJGROUP_WOODEN_DOOR2_ANIM04].CreateSprite("Rock2Door4"); 
    VObject[__OBJGROUP_WOODEN_DOOR2_ANIM05].CreateSprite("Rock2Door5"); 
    VObject[__OBJGROUP_WOODEN_DOOR2_ANIM06].CreateSprite("Rock2Door6"); 
    VObject[__OBJGROUP_WOODEN_DOOR2_ANIM07].CreateSprite("Rock2Door7"); 
    VObject[__OBJGROUP_WOODEN_DOOR2_ANIM08].CreateSprite("Rock2Door8"); 
    VObject[__OBJGROUP_WOODEN_DOOR2_ANIM09].CreateSprite("Rock2Door9"); 
    VObject[__OBJGROUP_WOODEN_DOOR2_ANIM10].CreateSprite("Rock2Door10"); 

    VObject[__OBJGROUP_V2_HAMMER01      ].CreateSprite("Ground_V2_Hammer01");
    VObject[__OBJGROUP_V2_HAMMER02      ].CreateSprite("Ground_V2_Hammer02");
    VObject[__OBJGROUP_V2_HAMMER03      ].CreateSprite("Ground_V2_Hammer03");
    VObject[__OBJGROUP_V2_HAMMER04      ].CreateSprite("Ground_V2_Hammer04");
    VObject[__OBJGROUP_V2_HAMMER05      ].CreateSprite("Ground_V2_Hammer05");
    VObject[__OBJGROUP_V2_HAMMER06      ].CreateSprite("Ground_V2_Hammer06");
    VObject[__OBJGROUP_V2_HAMMER07      ].CreateSprite("Ground_V2_Hammer07");
    VObject[__OBJGROUP_V2_2HAMMER01     ].CreateSprite("Ground_V2_Hammer04");
    VObject[__OBJGROUP_V2_2HAMMER02     ].CreateSprite("Ground_V2_Hammer05");

    VObject[__OBJGROUP_WOODEN_DOOR3_CLOSED].CreateSprite("Rock3Door1"); 
    VObject[__OBJGROUP_WOODEN_DOOR3_OPENED].CreateSprite("Rock3Door11"); 
    VObject[__OBJGROUP_WOODEN_DOOR3_ANIM02].CreateSprite("Rock3Door2");
    VObject[__OBJGROUP_WOODEN_DOOR3_ANIM03].CreateSprite("Rock3Door3"); 
    VObject[__OBJGROUP_WOODEN_DOOR3_ANIM04].CreateSprite("Rock3Door4"); 
    VObject[__OBJGROUP_WOODEN_DOOR3_ANIM05].CreateSprite("Rock3Door5"); 
    VObject[__OBJGROUP_WOODEN_DOOR3_ANIM06].CreateSprite("Rock3Door6"); 
    VObject[__OBJGROUP_WOODEN_DOOR3_ANIM07].CreateSprite("Rock3Door7"); 
    VObject[__OBJGROUP_WOODEN_DOOR3_ANIM08].CreateSprite("Rock3Door8"); 
    VObject[__OBJGROUP_WOODEN_DOOR3_ANIM09].CreateSprite("Rock3Door9"); 
    VObject[__OBJGROUP_WOODEN_DOOR3_ANIM10].CreateSprite("Rock3Door10"); 

    VObject[__OBJGROUP_WOODEN_DOOR4_CLOSED].CreateSprite("Rock4Door1"); 
    VObject[__OBJGROUP_WOODEN_DOOR4_OPENED].CreateSprite("Rock4Door11"); 
    VObject[__OBJGROUP_WOODEN_DOOR4_ANIM02].CreateSprite("Rock4Door2");
    VObject[__OBJGROUP_WOODEN_DOOR4_ANIM03].CreateSprite("Rock4Door3"); 
    VObject[__OBJGROUP_WOODEN_DOOR4_ANIM04].CreateSprite("Rock4Door4"); 
    VObject[__OBJGROUP_WOODEN_DOOR4_ANIM05].CreateSprite("Rock4Door5"); 
    VObject[__OBJGROUP_WOODEN_DOOR4_ANIM06].CreateSprite("Rock4Door6"); 
    VObject[__OBJGROUP_WOODEN_DOOR4_ANIM07].CreateSprite("Rock4Door7"); 
    VObject[__OBJGROUP_WOODEN_DOOR4_ANIM08].CreateSprite("Rock4Door8"); 
    VObject[__OBJGROUP_WOODEN_DOOR4_ANIM09].CreateSprite("Rock4Door9"); 
    VObject[__OBJGROUP_WOODEN_DOOR4_ANIM10].CreateSprite("Rock4Door10"); 

    VObject[__OBJGROUP_WOODEN_DOOR5_CLOSED].CreateSprite("Rock5Door1"); 
    VObject[__OBJGROUP_WOODEN_DOOR5_OPENED].CreateSprite("Rock5Door11"); 
    VObject[__OBJGROUP_WOODEN_DOOR5_ANIM02].CreateSprite("Rock5Door2");
    VObject[__OBJGROUP_WOODEN_DOOR5_ANIM03].CreateSprite("Rock5Door3"); 
    VObject[__OBJGROUP_WOODEN_DOOR5_ANIM04].CreateSprite("Rock5Door4"); 
    VObject[__OBJGROUP_WOODEN_DOOR5_ANIM05].CreateSprite("Rock5Door5"); 
    VObject[__OBJGROUP_WOODEN_DOOR5_ANIM06].CreateSprite("Rock5Door6"); 
    VObject[__OBJGROUP_WOODEN_DOOR5_ANIM07].CreateSprite("Rock5Door7"); 
    VObject[__OBJGROUP_WOODEN_DOOR5_ANIM08].CreateSprite("Rock5Door8"); 
    VObject[__OBJGROUP_WOODEN_DOOR5_ANIM09].CreateSprite("Rock5Door9"); 
    VObject[__OBJGROUP_WOODEN_DOOR5_ANIM10].CreateSprite("Rock5Door10"); 

    VObject[__OBJGROUP_WOODEN_DOOR6_CLOSED].CreateSprite("Rock6Door1"); 
    VObject[__OBJGROUP_WOODEN_DOOR6_OPENED].CreateSprite("Rock6Door11"); 
    VObject[__OBJGROUP_WOODEN_DOOR6_ANIM02].CreateSprite("Rock6Door2");
    VObject[__OBJGROUP_WOODEN_DOOR6_ANIM03].CreateSprite("Rock6Door3"); 
    VObject[__OBJGROUP_WOODEN_DOOR6_ANIM04].CreateSprite("Rock6Door4"); 
    VObject[__OBJGROUP_WOODEN_DOOR6_ANIM05].CreateSprite("Rock6Door5"); 
    VObject[__OBJGROUP_WOODEN_DOOR6_ANIM06].CreateSprite("Rock6Door6"); 
    VObject[__OBJGROUP_WOODEN_DOOR6_ANIM07].CreateSprite("Rock6Door7"); 
    VObject[__OBJGROUP_WOODEN_DOOR6_ANIM08].CreateSprite("Rock6Door8"); 
    VObject[__OBJGROUP_WOODEN_DOOR6_ANIM09].CreateSprite("Rock6Door9"); 
    VObject[__OBJGROUP_WOODEN_DOOR6_ANIM10].CreateSprite("Rock6Door10"); 

    VObject[__OBJGROUP_WOODEN_DOOR7_CLOSED].CreateSprite("Rock7Door1"); 
    VObject[__OBJGROUP_WOODEN_DOOR7_OPENED].CreateSprite("Rock7Door11"); 
    VObject[__OBJGROUP_WOODEN_DOOR7_ANIM02].CreateSprite("Rock7Door2");
    VObject[__OBJGROUP_WOODEN_DOOR7_ANIM03].CreateSprite("Rock7Door3"); 
    VObject[__OBJGROUP_WOODEN_DOOR7_ANIM04].CreateSprite("Rock7Door4"); 
    VObject[__OBJGROUP_WOODEN_DOOR7_ANIM05].CreateSprite("Rock7Door5"); 
    VObject[__OBJGROUP_WOODEN_DOOR7_ANIM06].CreateSprite("Rock7Door6"); 
    VObject[__OBJGROUP_WOODEN_DOOR7_ANIM07].CreateSprite("Rock7Door7"); 
    VObject[__OBJGROUP_WOODEN_DOOR7_ANIM08].CreateSprite("Rock7Door8"); 
    VObject[__OBJGROUP_WOODEN_DOOR7_ANIM09].CreateSprite("Rock7Door9"); 
    VObject[__OBJGROUP_WOODEN_DOOR7_ANIM10].CreateSprite("Rock7Door10"); 

    VObject[__OBJGROUP_WOODEN_DOOR8_CLOSED].CreateSprite("Rock8Door1"); 
    VObject[__OBJGROUP_WOODEN_DOOR8_OPENED].CreateSprite("Rock8Door11"); 
    VObject[__OBJGROUP_WOODEN_DOOR8_ANIM02].CreateSprite("Rock8Door2");
    VObject[__OBJGROUP_WOODEN_DOOR8_ANIM03].CreateSprite("Rock8Door3"); 
    VObject[__OBJGROUP_WOODEN_DOOR8_ANIM04].CreateSprite("Rock8Door4"); 
    VObject[__OBJGROUP_WOODEN_DOOR8_ANIM05].CreateSprite("Rock8Door5"); 
    VObject[__OBJGROUP_WOODEN_DOOR8_ANIM06].CreateSprite("Rock8Door6"); 
    VObject[__OBJGROUP_WOODEN_DOOR8_ANIM07].CreateSprite("Rock8Door7"); 
    VObject[__OBJGROUP_WOODEN_DOOR8_ANIM08].CreateSprite("Rock8Door8"); 
    VObject[__OBJGROUP_WOODEN_DOOR8_ANIM09].CreateSprite("Rock8Door9"); 
    VObject[__OBJGROUP_WOODEN_DOOR8_ANIM10].CreateSprite("Rock8Door10");

    VObject[__OBJGROUP_V2_SHIELD01].CreateSprite("Ground_V2_Shield01");
    VObject[__OBJGROUP_V2_SHIELD02].CreateSprite("Ground_V2_Shield02");
    VObject[__OBJGROUP_V2_ICESHIELD].CreateSprite("Ground_V2_iceShield01");
    

    VObject[__OBJGROUP_MAN_ARMOR01_BODY_GR].CreateSprite("Ground_ManArmor01Body" , 0);
    VObject[__OBJGROUP_MAN_ARMOR01_BODY_BL].CreateSprite("Ground_ManArmor01Body" , 0, 2);
    VObject[__OBJGROUP_MAN_ARMOR01_LEGS_GR].CreateSprite("Ground_ManArmor01Legs" , 0);
    VObject[__OBJGROUP_MAN_ARMOR01_LEGS_BL].CreateSprite("Ground_ManArmor01Legs" , 0, 2);
    VObject[__OBJGROUP_MAN_ARMOR01_HAND_GR].CreateSprite("Ground_ManArmor01Glove", 0);
    VObject[__OBJGROUP_MAN_ARMOR01_HAND_BL].CreateSprite("Ground_ManArmor01Glove", 0, 2);
    VObject[__OBJGROUP_MAN_ARMOR01_FOOT_GR].CreateSprite("Ground_ManArmor01Boots", 0);
    VObject[__OBJGROUP_MAN_ARMOR01_FOOT_BL].CreateSprite("Ground_ManArmor01Boots", 0, 2);

    VObject[__OBJGROUP_T4CP_SHIELD].CreateSprite("Ground_T4CP_Shield");
    VObject[__OBJGROUP_T4CP_MATRAQUE].CreateSprite("Ground_T4CP_Matraque");
    VObject[__OBJGROUP_T4CP_BAT]     .CreateSprite("Ground_TankBat");
    VObject[__OBJGROUP_T4CP_12 ]     .CreateSprite("Ground_T4CP_12");
    VObject[__OBJGROUP_WHITELEATHER_BOOT].CreateSprite("BlackLeatherBoots",0,2);
    

    VObject[__OBJGROUP_CAULDRON_CONTAINER].CreateSprite("64kTrollCauldron-a");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+1].CreateSprite("64kTrollCauldron-b");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+2].CreateSprite("64kTrollCauldron-c");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+3].CreateSprite("64kTrollCauldron-d");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+4].CreateSprite("64kTrollCauldron-e");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+5].CreateSprite("64kTrollCauldron-f");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+6].CreateSprite("64kTrollCauldron-g");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+7].CreateSprite("64kTrollCauldron-h");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+8].CreateSprite("64kTrollCauldron-i");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+9].CreateSprite("64kTrollCauldron-j");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+10].CreateSprite("64kTrollCauldron-k");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+11].CreateSprite("64kTrollCauldron-l");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+12].CreateSprite("64kTrollCauldron-m");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+13].CreateSprite("64kTrollCauldron-n");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+14].CreateSprite("64kTrollCauldron-o");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+15].CreateSprite("64kTrollCauldron-p");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+16].CreateSprite("64kTrollCauldron-q");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+17].CreateSprite("64kTrollCauldron-r");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+18].CreateSprite("64kTrollCauldron-s");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+19].CreateSprite("64kTrollCauldron-t");
    VObject[__OBJGROUP_CAULDRON_CONTAINER+20].CreateSprite("64kTrollCauldron-u");
    
       
    

    //Static sprites summonable added (like cauldron, adding some trees and tables..)
    VObject[__OBJGROUP_TREE1].CreateSprite("Oaktree2");//BLBLBL test Oaktree2=>MissingSprite (BarBar funny)
    // New added by Dialsoft - END

    KEEPALIVE_NM;
    
    char strNom[100];
    for(int nmI=0;nmI<__OBJANIM_FONTAIN_02_CNT;nmI++)
    {
       sprintf(strNom,"Fountaine2%04d",nmI);
       VObject[__OBJANIM_FONTAIN_02+ nmI].CreateSprite(strNom);
    }
    VObject[__OBJLIGHT_LANTERNE1 ].CreateSprite(""); //cre un sprite carree noire
    VObject[__OBJLIGHT_CHANDELIER].CreateSprite(""); //cre un sprite carree noire
    VObject[__OBJLIGHT_POUTRE    ].CreateSprite(""); //cre un sprite carree noire
    VObject[__OBJLIGHT_CHANDELLE2].CreateSprite(""); //cre un sprite carree noire

    int Image = 0;
    for(int nmI=0;nmI<__OBJANIM_MOULIND_CNT;nmI++)
    {
       sprintf(strNom,"MoulinD-%04d",Image);
       Image+=2;
       VObject[__OBJANIM_MOULIND+ nmI].CreateSprite(strNom);
    }

    Image = 0;
    for(int nmI=0;nmI<__OBJANIM_MOULING_CNT;nmI++)
    {
       sprintf(strNom,"MoulinG-%04d",Image);
       Image+=2;
       VObject[__OBJANIM_MOULING+ nmI].CreateSprite(strNom);
    }


    //:CR_NMNMNM
    for(int vv=0;vv<51;vv++)
    {
       char strName[30];
       sprintf(strName,"CercleVie-S%02d",vv);
       m_v2SpriteLifeS[vv].CreateSprite(strName);
       char strNameB[30];
       sprintf(strNameB,"BigCercleVie-S%02d",vv);
       m_v2SpriteLifeSB[vv].CreateSprite(strNameB);
    }
    m_v2SpritePShadow.CreateSprite("PlayerShadow");
    
    
#define IARMOR     "64kIconArmor"
#define IAXE       "64kIconAxe"
#define IBELT      "64kIconBelt"
#define IBOOTS     "64kIconBoots"
#define IBOW       "64kIconBow"
#define IDAGGER    "64kIconDagger"
#define IFLAIL     "64kIconFlail"
#define IWARHAMMER "64kIconWarHammer"
#define IGLOVES    "64kIconGloves"
#define IHELMET    "64kIconHelmet"
#define INECKLACE  "64kIconNecklace"
#define IPOTIONGRN "64kIconPotion"
#define IPOTIONBLU "64kIconPotionBlue"
#define IPOTIONRED "64kIconPotionRed"
#define IPOUCH     "64kIconPouch"
#define IQUIVER    "64kIconQuiver"
#define IRING      "64kIconRing"
#define ISCROLL    "64kIconScroll"
#define ISHIELD    "64kIconShield"
#define ISTAFF     "64kIconStaff"
#define ISWORD     "64kIconSword"
#define ILEGGINGS  "64kIconArmorLegging"
#define ICAPE      "64kIconCape"
#define ITORCH     "64kIconTorch"
#define ISPEAR     "64kIconSpear"
#define IGEM	    "64kIconGem"
#define IUNK       "??"
    
    // Set all the icons.
    ItemIcons.BindSprite( ISWORD, __OBJGROUP_SHORT_SWORD );
    ItemIcons.BindSprite( ISWORD, __OBJGROUP_LONG_SWORD );
    ItemIcons.BindSprite( IFLAIL, __OBJGROUP_FLAIL );
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_MORNING_STAR );
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_WARHAMMER );
    ItemIcons.BindSprite( ISPEAR, __OBJGROUP_SPEAR );
    ItemIcons.BindSprite( IAXE,   __OBJGROUP_AXE );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_LEATHER_ARMOR );
    //ItemIcons.BindSprite( __OBJGROUP_WELL_TALK );
    //ItemIcons.BindSprite( __OBJGROUP_CHEST_TALK );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_LEATHER_HELM );
    ItemIcons.BindSprite( ISCROLL, __OBJGROUP_MAGE_SPELLBOOK );
    //ItemIcons.BindSprite( __OBJGROUP_PRIEST_ANKH );
    ItemIcons.BindSprite( ISCROLL, __OBJGROUP_SCROLL );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_DOOR_CLOSED );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_DOOR_OPENED );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_CHAIR );
    //ItemIcons.BindSprite( __OBJGROUP_CHEST_I );
    //ItemIcons.BindSprite( __OBJGROUP_CHEST_OPEN_I );
    //ItemIcons.BindSprite( __OBJGROUP_CHEST2_I );
    //ItemIcons.BindSprite( __OBJGROUP_LONGUE_VUE );
    ItemIcons.BindSprite( ISTAFF, __OBJGROUP_TORCH );
    //ItemIcons.BindSprite( __OBJGROUP_IRON_KEY );
    //ItemIcons.BindSprite( __OBJGROUP_BLUE_KEY );
    //ItemIcons.BindSprite( __OBJGROUP_WEIRD_KEY );
    //ItemIcons.BindSprite( __OBJGROUP_GOLD_KEY );
    //ItemIcons.BindSprite( __OBJGROUP_KEY_HOLD );
    //ItemIcons.BindSprite( __OBJGROUP_EMPTY_MUG );
    //ItemIcons.BindSprite( __OBJGROUP_BEER_MUG );
    //ItemIcons.BindSprite( __OBJGROUP_BEER_MUG_TOO_FULL );
    //ItemIcons.BindSprite( __OBJGROUP_EMPTY_GLASS );
    //ItemIcons.BindSprite( __OBJGROUP_BEVERAGE );
    //ItemIcons.BindSprite( __OBJGROUP_CUP_FULL );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_CUP );
    //ItemIcons.BindSprite( __OBJGROUP_CHEST );
    //ItemIcons.BindSprite( __OBJGROUP_CHEST_OPEN );
    //ItemIcons.BindSprite( __OBJGROUP_BASKET_CLOSE );
    //ItemIcons.BindSprite( __OBJGROUP_BASKET_OPEN );
    ItemIcons.BindSprite( ISCROLL, __OBJGROUP_RED_BOOK );
    ItemIcons.BindSprite( ISCROLL, __OBJGROUP_GREY_BOOK );
    ItemIcons.BindSprite( ISCROLL, __OBJGROUP_BROWN_BOOK );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_BACKPACK );
    //ItemIcons.BindSprite( __OBJGROUP_BANDAGES_PACK );
    //ItemIcons.BindSprite( __OBJGROUP_BANDAGE_1 );
    //ItemIcons.BindSprite( __OBJGROUP_BANDAGE_2 );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_BOWL );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_VASE );
    //ItemIcons.BindSprite( __OBJGROUP_GOLD_BAR );
    //ItemIcons.BindSprite( __OBJGROUP_GOLD_BAR_PACK );
    //ItemIcons.BindSprite( __OBJGROUP_BRONZE_BAR );
    //ItemIcons.BindSprite( __OBJGROUP_BRONZE_BAR_PACK );
    //ItemIcons.BindSprite( __OBJGROUP_IRON_BAR );
    //ItemIcons.BindSprite( __OBJGROUP_IRON_BAR_PACK );
    //ItemIcons.BindSprite( __OBJGROUP_BROOM_ON_WALL );
    //ItemIcons.BindSprite( __OBJGROUP_PIOCHE );
    //ItemIcons.BindSprite( __OBJGROUP_BROOM_ON_FLOOR );
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_HAMMER );
    //ItemIcons.BindSprite( __OBJGROUP_PINCES );
    //ItemIcons.BindSprite( __OBJGROUP_BALANCE );
    //ItemIcons.BindSprite( __OBJGROUP_HOURGLASS );
    //ItemIcons.BindSprite( __OBJGROUP_EPROUVETTE_6_PACKS );
    //ItemIcons.BindSprite( __OBJGROUP_EPROUVETTE_3_PACKS );
    //ItemIcons.BindSprite( __OBJGROUP_EPROUVETTE_ALONE );
    //ItemIcons.BindSprite( __OBJGROUP_BUCHE_DE_BOIS );
    //ItemIcons.BindSprite( __OBJGROUP_WOOD_PACK_CLASSED );
    //ItemIcons.BindSprite( __OBJGROUP_WOOD_PACK_MIXED );
    //ItemIcons.BindSprite( __OBJGROUP_JAMBON );
    //ItemIcons.BindSprite( __OBJGROUP_LONG_SAUSAGE );
    //ItemIcons.BindSprite( __OBJGROUP_SAUSAGE_PACK );
    //ItemIcons.BindSprite( __OBJGROUP_FAT_JAMBON );
    //ItemIcons.BindSprite( __OBJGROUP_CHIKEN );
    //ItemIcons.BindSprite( __OBJGROUP_BREAD );
    //ItemIcons.BindSprite( __OBJGROUP_PACK_OF_PASTRIES );
    //ItemIcons.BindSprite( __OBJGROUP_WATERBASKET );
    //ItemIcons.BindSprite( __OBJGROUP_FISHING_POLE );
    //ItemIcons.BindSprite( __OBJGROUP_DEAD_FISHES );
    //ItemIcons.BindSprite( __OBJGROUP_EMPTY_BASKET );
    ItemIcons.BindSprite( IQUIVER, __OBJGROUP_ARROW );
    ItemIcons.BindSprite( IQUIVER, __OBJGROUP_ARROW_PACK );
    ItemIcons.BindSprite( IBOW, __OBJGROUP_BOW );
    ItemIcons.BindSprite( IBOW, __OBJGROUP_CROSSBOW );
    //ItemIcons.BindSprite( __OBJGROUP_BANJO );
    //ItemIcons.BindSprite( __OBJGROUP_TAMBOUR );
    //ItemIcons.BindSprite( __OBJGROUP_MAP );
    //ItemIcons.BindSprite( __OBJGROUP_SCIE );
    //ItemIcons.BindSprite( __OBJGROUP_LONG_WHOOL_BALL );
    //ItemIcons.BindSprite( __OBJGROUP_WHOOL_BALL );
    //ItemIcons.BindSprite( __OBJGROUP_GREEN_WHOOL_BALL );
    //ItemIcons.BindSprite( __OBJGROUP_BLUE_WHOOL_BALL );
    //ItemIcons.BindSprite( __OBJGROUP_SHOVEL );
    ItemIcons.BindSprite( IPOTIONBLU, __OBJGROUP_BLUE_POTION );
    ItemIcons.BindSprite( IPOTIONRED, __OBJGROUP_YELLOW_POTION );
    ItemIcons.BindSprite( IPOTIONGRN, __OBJGROUP_BLACK_POTION );
    ItemIcons.BindSprite( IPOTIONBLU, __OBJGROUP_EMPTY_POTION );
    ItemIcons.BindSprite( IPOTIONBLU, __OBJGROUP_PURPLE_POTION );
    //ItemIcons.BindSprite( __OBJGROUP_BASKET_FULL_WATER );
    //ItemIcons.BindSprite( __OBJGROUP_BASKET_FULL_FRUIT );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_ROUND_CHAIR );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_CHAIR_2 );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_ROUND_CHAIR_2 );
    //ItemIcons.BindSprite( __OBJGROUP_CRATES );
    //ItemIcons.BindSprite( __OBJGROUP_RECTANGLE_CRATES );
    //ItemIcons.BindSprite( __OBJGROUP_CUBIQ_CRATES );
    //ItemIcons.BindSprite( __OBJGROUP_KITCHEN_BOWL );
    //ItemIcons.BindSprite( __OBJGROUP_KITCHEN_SPOON );
    //ItemIcons.BindSprite( __OBJGROUP_KITCHEN_FORK );
    //ItemIcons.BindSprite( __OBJGROUP_KITCHEN_KNIFE );
    //ItemIcons.BindSprite( __OBJGROUP_ROULEAU_A_PATRE );
    //ItemIcons.BindSprite( __OBJGROUP_CHANDELLE );
    //ItemIcons.BindSprite( __OBJGROUP_CHANDELLE_SUR_PIED );
    //ItemIcons.BindSprite( __OBJGROUP_MIRROR );
    ItemIcons.BindSprite( ISTAFF, __OBJGROUP_STAFF1 );
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_MACE );
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_SPIKE_MACE );
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_MAUL );
    ItemIcons.BindSprite( IAXE, __OBJGROUP_DOUBLE_AXE );
    ItemIcons.BindSprite( IAXE, __OBJGROUP_SINGLE_AXE );
    ItemIcons.BindSprite( ISWORD, __OBJGROUP_BASTARD_SWORD );
    ItemIcons.BindSprite( IAXE, __OBJGROUP_SPEAR_AXE );
    ItemIcons.BindSprite( ISCROLL, __OBJGROUP_SCROLL_OUVERT );
    ItemIcons.BindSprite( ISCROLL, __OBJGROUP_SCROLL_BLUE );
    ItemIcons.BindSprite( ISCROLL, __OBJGROUP_LIVRE_OUVERT );
    ItemIcons.BindSprite( ISCROLL, __OBJGROUP_LIVRE_OUVERT_EPAIS );
    KEEPALIVE_NM;
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_DOOR_CLOSED_I );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_DOOR_OPENED_I );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_CHAIR_I );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_CHAIR_2_I );
    //ItemIcons.BindSprite( __OBJGROUP_WOODEN_ROUND_CHAIR_2_I );
    //ItemIcons.BindSprite( __OBJGROUP_2_WOODEN_CHAIR );
    //ItemIcons.BindSprite( __OBJGROUP_2_WOODEN_CHAIR_2 );
    //ItemIcons.BindSprite( __OBJGROUP_2_WOODEN_ROUND_CHAIR_2 );
    //ItemIcons.BindSprite( __OBJGROUP_2_WOODEN_CHAIR_I );
    //ItemIcons.BindSprite( __OBJGROUP_2_WOODEN_CHAIR_2_I );
    //ItemIcons.BindSprite( __OBJGROUP_2_WOODEN_ROUND_CHAIR_2_I );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_POUCH );
    //ItemIcons.BindSprite( __OBJGROUP_ROPE );
    //ItemIcons.BindSprite( __OBJGROUP_STUFFED_BASKET_1 );
    //ItemIcons.BindSprite( __OBJGROUP_STUFFED_BASKET_2 );
    //ItemIcons.BindSprite( __OBJGROUP_STUFFED_BASKET_3 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_1 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_2 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_3 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_4 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_5 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_6 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_7 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_8 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_9 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_10 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_11 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_12 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_13 );
    ItemIcons.BindSprite( IPOUCH, __OBJGROUP_FRUIT_14 );
    //ItemIcons.BindSprite( __OBJGROUP_COINS                      = 160;
    //ItemIcons.BindSprite( __OBJGROUP_COINS_PILE                 = 161;
    //ItemIcons.BindSprite( __OBJGROUP_COINS_BIG_PILE             = 162;
    //ItemIcons.BindSprite( __OBJGROUP_BATWINGS                   = 163;
    ItemIcons.BindSprite( IGEM, __OBJGROUP_GEMS_PURPLE );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_GEMS_YELLOW );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_GEMS_BLUE );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_GEMS_GREEN );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_GEMS_PACK_1 );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_GEMS_PACK_2 );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_GEMS_PACK_3 );
    //ItemIcons.BindSprite( __OBJGROUP_KOBOLDHAIR                 = 171;      
    ItemIcons.BindSprite( INECKLACE, __OBJGROUP_NECKLACE_1 );
    ItemIcons.BindSprite( INECKLACE, __OBJGROUP_NECKLACE_2 );
    ItemIcons.BindSprite( INECKLACE, __OBJGROUP_NECKLACE_3 );
    //ItemIcons.BindSprite( __OBJGROUP_ORCFEET                    = 175;
    ItemIcons.BindSprite( IRING, __OBJGROUP_RINGS_1 );
    ItemIcons.BindSprite( IRING, __OBJGROUP_RINGS_2 );
    ItemIcons.BindSprite( IRING, __OBJGROUP_RINGS_3 );
    ItemIcons.BindSprite( IRING, __OBJGROUP_RINGS_4 );
    ItemIcons.BindSprite( IRING, __OBJGROUP_RINGS_5 );
    //ItemIcons.BindSprite( __OBJGROUP_SKEL_BONE );
    //ItemIcons.BindSprite( __OBJGROUP_SPIDER_EYES );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_PADDED_GLOVE );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_PADDED_HELM );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PADDED_ARMOR );
    //ItemIcons.BindSprite( __OBJGROUP_PADDED_SLEEVES );
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_PADDED_LEGGINGS );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_SCALE_ARMOR );
    //ItemIcons.BindSprite( __OBJGROUP_LEVIER );
    ItemIcons.BindSprite( ITORCH, __OBJGROUP_FIRECAMP ); //BLBLBL : 25 mars 2009 ajouté une icone inventaire aux feux de camp
    //ItemIcons.BindSprite( __OBJGROUP_CAMPBURNT );
    ItemIcons.BindSprite( ISWORD, __OBJGROUP_GLINTING_SWORD );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_DESTINY_GEM );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_VOLCANO_ROCK );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_RING_HELM );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_RING_ARMOR );
    //ItemIcons.BindSprite( __OBJGROUP_RING_ARMOR_SLEEVES );
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_RING_LEGGINGS );
    //ItemIcons.BindSprite( __OBJGROUP_FEATHER );
    //ItemIcons.BindSprite( IUNK, __OBJGROUP_ICESHARD );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_LIFEGEM );
    //ItemIcons.BindSprite( __OBJGROUP_OAKTREELEAF );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_PADDED_BOOTS );
    ItemIcons.BindSprite( ITORCH, __OBJGROUP_TORCHE );
    ItemIcons.BindSprite( ITORCH, __OBJGROUP_TORCHE_I );
    
    
    ItemIcons.BindSprite( IBELT, __OBJGROUP_BELT );
    ItemIcons.BindSprite( IPOTIONBLU, __OBJGROUP_BLUEFLASK );
    ItemIcons.BindSprite( IRING, __OBJGROUP_BRACELET );
    //ItemIcons.BindSprite( __OBJGROUP_CHEST2 );
    //ItemIcons.BindSprite( __OBJGROUP_PINKLEAF );
    ItemIcons.BindSprite( IPOTIONGRN, __OBJGROUP_POT_GREEN );
    ItemIcons.BindSprite( IPOTIONRED, __OBJGROUP_POT_RED );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_SHIELD );
    ItemIcons.BindSprite( IRING, __OBJGROUP_STONELIFE );
    ItemIcons.BindSprite( ITORCH, __OBJGROUP_TORCH2 );
    //ItemIcons.BindSprite( __OBJGROUP_TOOTH );
    ItemIcons.BindSprite( IPOTIONBLU, __OBJGROUP_GODPOT_BLUE );
    ItemIcons.BindSprite( IPOTIONGRN, __OBJGROUP_GODPOT_GREEN );
    ItemIcons.BindSprite( IPOTIONRED, __OBJGROUP_GODPOT_RED );
    ItemIcons.BindSprite( IPOTIONRED, __OBJGROUP_GODPOT_YELLOW );
    ItemIcons.BindSprite( IPOTIONGRN, __OBJGROUP_LARGEPOT_GREEN );
    ItemIcons.BindSprite( IPOTIONRED, __OBJGROUP_LARGEPOT_RED );
    ItemIcons.BindSprite( IPOTIONBLU, __OBJGROUP_POT_BLUE );
    ItemIcons.BindSprite( IPOTIONRED, __OBJGROUP_POT_ORANGE );
    ItemIcons.BindSprite( IPOTIONBLU, __OBJGROUP_POT_TURQUOISE );
    ItemIcons.BindSprite( IPOTIONBLU, __OBJGROUP_POT_VIOLET );
    ItemIcons.BindSprite( IPOTIONRED, __OBJGROUP_POT_YELLOW );
    ItemIcons.BindSprite( IPOTIONGRN, __OBJGROUP_GREENFLASK );
    ItemIcons.BindSprite( IPOTIONRED, __OBJGROUP_REDFLASK );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_LEATHER_GLOVE );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_LEATHER_BOOTS );
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_LEATHER_PANTS );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_PLATE_GLOVE );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_W_SLEEVE );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_PLATE_BOOT	);
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_PLATE_LEGS );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_PLATE_HELM );
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_CHAIN_LEGS );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_CHAIN_BODY );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_CHAIN_COIF );
    ItemIcons.BindSprite( ISWORD, __OBJGROUP_DARK_SWORD );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_ROMAN_SHIELD );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_BAROSSA_SHIELD );
    ItemIcons.BindSprite( IDAGGER, __OBJGROUP_DAGGER );
    ItemIcons.BindSprite( ISWORD, __OBJGROUP_REAL_DARKSWORD );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_HORNED_HELMET );
    ItemIcons.BindSprite( ISWORD, __OBJGROUP_BATTLE_SWORD );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_NECROMANROBE );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_REDROBE );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_MAGEROBE );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_WHITEROBE );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_GOLDENCROWN );
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_GOLDEN_STAR );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_ELVEN_HAT );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_ORC_SHIELD );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_STUDDEDARMOR );
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_LEG_CLOTH1 );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_BODY_CLOTH1 );
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_STUDDEDLEG );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_REDCAPE );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_BLACKLEATHER_BOOT );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_WHITELEATHER_BOOT );
    //ItemIcons.BindSprite( __OBJGROUP_HEAD );
    //ItemIcons.BindSprite( __OBJGROUP_SUNDIAL_TALK );
    //ItemIcons.BindSprite( __OBJGROUP_SIGN1_TALK );
    //ItemIcons.BindSprite( __OBJGROUP_SIGN2_TALK );
    //ItemIcons.BindSprite( __OBJGROUP_SIGN3_TALK );
    ItemIcons.BindSprite( ISTAFF, __OBJGROUP_STAFF2 );
    ItemIcons.BindSprite( ISTAFF, __OBJGROUP_STAFF3 );
    ItemIcons.BindSprite( ISTAFF, __OBJGROUP_STAFF4 );
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_OGRECLUB );
    ItemIcons.BindSprite( IQUIVER, __OBJGROUP_QUIVER );
    ItemIcons.BindSprite( IBOW, __OBJGROUP_BOW );
    ItemIcons.BindSprite( IBOW, __OBJGROUP_LARGE_BOW );
    ItemIcons.BindSprite( IBOW, __OBJGROUP_LARGE_BOW2 );
    ItemIcons.BindSprite( IBOW, __OBJGROUP_FANCY_SHORT_BOW );
    ItemIcons.BindSprite( IBOW, __OBJGROUP_FANCY_LONG_BOW );
    ItemIcons.BindSprite( IQUIVER, __OBJGROUP_BLUE_QUIVER );
    ItemIcons.BindSprite( IQUIVER, __OBJGROUP_YELLOW_QUIVER );
    ItemIcons.BindSprite( IQUIVER, __OBJGROUP_GREEN_QUIVER );
    ItemIcons.BindSprite( IQUIVER, __OBJGROUP_RED_QUIVER );
    ItemIcons.BindSprite( IQUIVER, __OBJGROUP_BLACK_QUIVER );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_CENTAUR_SHIELD1 );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_CENTAUR_SHIELD2 );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_SHAMAN_HELM );
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_SKAVEN_CLUB );
    ItemIcons.BindSprite( IDAGGER, __OBJGROUP_SKAVEN_KNIFE );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_SKAVEN_SHIELD1 );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_SKAVEN_SHIELD2 );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_SKAVEN_SHIELD3 );
    ItemIcons.BindSprite( IAXE, __OBJGROUP_SKELETON_AXE );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_SKELETON_HELM );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_SKELETON_SHIELD );
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_ORC_SHIELD2);
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_SK_SHIELD);
    ItemIcons.BindSprite( ISWORD, __OBJGROUP_SKELETON_SWORD );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_BLACK_WINGS );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_WHITE_WINGS );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_DECHU_WINGS );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_DECHU2_WINGS );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_DARK_GEM );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_PURPLE_GEM );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_RED_GEM );
    ItemIcons.BindSprite( IAXE, __OBJGROUP_HALBERD );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_SMALL_PURPLE_GEM );
    ItemIcons.BindSprite( IGEM, __OBJGROUP_SMALL_RED_GEM );

	// New added by Dialsoft - BEGIN
	ItemIcons.BindSprite( IRING, __OBJGROUP_BRACELET_RUBIS );
	ItemIcons.BindSprite( IRING, __OBJGROUP_BRACELET_AZURE );
	ItemIcons.BindSprite( IRING, __OBJGROUP_BRACELET_SQUARE_WHITE );
	ItemIcons.BindSprite( IBELT, __OBJGROUP_BELT_JEWELS_BROWN );
	ItemIcons.BindSprite( IBELT, __OBJGROUP_BELT_JEWELS_PURPLE );
	ItemIcons.BindSprite( IBELT, __OBJGROUP_BELT_RED );
	ItemIcons.BindSprite( IGEM, __OBJGROUP_GEM_AZURE );
	ItemIcons.BindSprite( IGEM, __OBJGROUP_GEM_ORANGE );
	ItemIcons.BindSprite( IGEM, __OBJGROUP_GEM_PINK );
	ItemIcons.BindSprite( IRING, __OBJGROUP_RING_3RUBIS );
	ItemIcons.BindSprite( IRING, __OBJGROUP_RING_RED );
	ItemIcons.BindSprite( IRING, __OBJGROUP_RING_STARGOLD );
	ItemIcons.BindSprite( IRING, __OBJGROUP_RING_CUT_GREEN );
	ItemIcons.BindSprite( IRING, __OBJGROUP_RING_CUT_RED );
	ItemIcons.BindSprite( IGEM, __OBJGROUP_RUBIS_FOCUS );
	ItemIcons.BindSprite( IGEM, __OBJGROUP_EMERALD_FOCUS );
	ItemIcons.BindSprite( IGEM, __OBJGROUP_DIAMOND_FOCUS );
	ItemIcons.BindSprite( IGEM, __OBJGROUP_SAPHIRE_FOCUS );
	ItemIcons.BindSprite( IGEM, __OBJGROUP_EYE_OF_TIGER );
	ItemIcons.BindSprite( IGEM, __OBJGROUP_MANA_STONE );
	ItemIcons.BindSprite( IGEM, __OBJGROUP_BLUE_STONE );
	ItemIcons.BindSprite( IHELMET, __OBJGROUP_REDFEATHER );
	ItemIcons.BindSprite( IFLAIL, __OBJGROUP_FIREFLAIL );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_SPIKEDLEATHER );
	ItemIcons.BindSprite( ICAPE, __OBJGROUP_DARKWINGS );
	ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_NEUTRAL_WINGS );
	ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_RED_WINGS );
	ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_GOLD_WINGS );
	ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_DARKRED_WINGS );
	ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_DARKGOLD_WINGS );
	ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_LIGHTBLUE_WINGS );
	ItemIcons.BindSprite( ICAPE, __OBJGROUP_SERAPH_ORANGE_WINGS );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_FWHITEROBE_BLUE );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_FWHITEROBE_GREEN );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_FWHITEROBE_DARKBLUE );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_FWHITEROBE_VIOLET );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_FWHITEROBE_PURPLE );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_FWHITEROBE_YELLOW );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_FWHITEROBE_GRAY );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_FWHITEROBE_MAUVE );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_FWHITEROBE_ORANGE );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_FWHITEROBE_DARK );
   KEEPALIVE_NM;
	//Added by Tiamat
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_GOLD );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_PLATE_GLOVE_GOLD );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_GOLD );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_PLATE_BOOT_GOLD	);
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_PLATE_LEGS_GOLD );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_PLATE_HELM_GOLD );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_SILVER );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_PLATE_GLOVE_SILVER );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_SILVER );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_PLATE_BOOT_SILVER	);
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_PLATE_LEGS_SILVER );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_PLATE_HELM_SILVER );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_BLACK );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_PLATE_GLOVE_BLACK );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLACK );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_PLATE_BOOT_BLACK	);
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_PLATE_LEGS_BLACK );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_PLATE_HELM_BLACK );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_BLUE );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_PLATE_GLOVE_BLUE );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLUE );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_PLATE_BOOT_BLUE	);
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_PLATE_LEGS_BLUE );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_PLATE_HELM_BLUE );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_GREEN );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_PLATE_GLOVE_GREEN );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_GREEN );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_PLATE_BOOT_GREEN	);
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_PLATE_LEGS_GREEN );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_PLATE_HELM_GREEN );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_RED );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_PLATE_GLOVE_RED );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_RED );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_PLATE_BOOT_RED	);
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_PLATE_LEGS_RED );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_PLATE_HELM_RED );
	ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_PURPLE );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_PLATE_GLOVE_PURPLE );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_PURPLE );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_PLATE_BOOT_PURPLE	);
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_PLATE_LEGS_PURPLE );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_PLATE_HELM_PURPLE );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_EMERALD );
    ItemIcons.BindSprite( IGLOVES, __OBJGROUP_PLATE_GLOVE_EMERALD );
    ItemIcons.BindSprite( IARMOR, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_EMERALD );
    ItemIcons.BindSprite( IBOOTS, __OBJGROUP_PLATE_BOOT_EMERALD	);
    ItemIcons.BindSprite( ILEGGINGS, __OBJGROUP_PLATE_LEGS_EMERALD );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_PLATE_HELM_EMERALD );
    ItemIcons.BindSprite( IAXE, __OBJGROUP_TIGERAXE );
    ItemIcons.BindSprite( IAXE, __OBJGROUP_ANCIENTAXE );
    ItemIcons.BindSprite( IFLAIL, __OBJGROUP_FLOWERS );
    ItemIcons.BindSprite( ISTAFF, __OBJGROUP_NECROSTAFF );

   	//ADDED BY LUCIFER
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_BLUECAPE );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_AQUACAPE );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_BLACKCAPE );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_PURPLECAPE );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_SILVERCAPE );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_YELLOWCAPE );
    ItemIcons.BindSprite( ICAPE, __OBJGROUP_GREENCAPE );

    ItemIcons.BindSprite( IHELMET, __OBJGROUP_WITCH_HAT1 );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_WITCH_HAT2 );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_WITCH_HAT3 );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_WITCH_HAT4 );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_WITCH_HAT5 );
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_NOEL_HAT);
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_T4CP_CASQUE);
    ItemIcons.BindSprite( IHELMET, __OBJGROUP_T4CP_CASQUETTE);

    ItemIcons.BindSprite( IBOW       , __OBJGROUP_NEW_BOW01);
    ItemIcons.BindSprite( IWARHAMMER , __OBJGROUP_DWARF_HAMMER);
    ItemIcons.BindSprite( IAXE       ,__OBJGROUP_AXE_DESTRUCTION);
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_SWORD_ANGEL);
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_CLAY2BLADES);
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_SWORD_HERO);
    

    ItemIcons.BindSprite( ICAPE     , __OBJGROUP_BUTTERFLY_WING1);
    ItemIcons.BindSprite( ICAPE     , __OBJGROUP_BUTTERFLY_WING2);
    ItemIcons.BindSprite( ICAPE     , __OBJGROUP_BUTTERFLY_WING3);
    ItemIcons.BindSprite( ICAPE     , __OBJGROUP_BUTTERFLY_WING4);

    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_HAMM);

    ItemIcons.BindSprite( ICAPE     , __OBJGROUP_ARCHWINGS_WHITE);
    ItemIcons.BindSprite( IARMOR    , __OBJGROUP_LICHROBE);//NAD
    ItemIcons.BindSprite( IHELMET   , __OBJGROUP_MASQUE_GOBLIN );
    ItemIcons.BindSprite( IARMOR    , __OBJGROUP_ARMORED_ROBE);

    ItemIcons.BindSprite( IHELMET    , __OBJGROUP_V2_HAUME01);
    ItemIcons.BindSprite( IHELMET    , __OBJGROUP_V2_HAUME02);
    ItemIcons.BindSprite( IHELMET    , __OBJGROUP_V2_HAUME03);
    ItemIcons.BindSprite( IHELMET    , __OBJGROUP_V2_HAUME04);
    ItemIcons.BindSprite( IHELMET    , __OBJGROUP_V2_HAUME04_BL);
    ItemIcons.BindSprite( IHELMET    , __OBJGROUP_V2_HAT01  );
    ItemIcons.BindSprite( IHELMET    , __OBJGROUP_V2_MASK01 );

    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_DAGGER_VIP);
    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_DAGGER01  );
    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_DAGGER02  );
    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_DAGGER03  );
    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_DAGGER04  );
    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_DAGGER05  );

    //NM_TAG_ADD_SWORD
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_BUSSWORD01   );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_BUSSWORD02   );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_BUSSWORD03   );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_CLAYSWORD01  );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_CLAYSWORD02  );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_GLAIVESWORD01);
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_GLAIVESWORD02);
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SWORD01      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SWORD02      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SWORD03      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SWORD04      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SWORD05      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SWORD06      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SWORD07      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SWORD08      );

    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2BUSSWORD01   );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2BUSSWORD02   );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2BUSSWORD03   );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2CLAYSWORD01  );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2CLAYSWORD02  );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2GLAIVESWORD01);
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2GLAIVESWORD02);
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SWORD01      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SWORD02      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SWORD03      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SWORD04      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SWORD05      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SWORD06      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SWORD07      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SWORD08      );

    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE01);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE02);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE03);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE04);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE05);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE06);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE07);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE08);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE09);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE10);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_HACHE11);

    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_2HACHE01);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_2HACHE02);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_2HACHE03);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_2HACHE04);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_2HACHE05);
    ItemIcons.BindSprite( IAXE     , __OBJGROUP_V2_2HACHE06);

    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_2DAGGER01  );
    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_2DAGGER02  );
    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_2DAGGER03  );
    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_2DAGGER04  );
    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_2DAGGER05  );
    ItemIcons.BindSprite( IDAGGER    , __OBJGROUP_V2_2DAGGER06  );

 	 ItemIcons.BindSprite( IBOW       , __OBJGROUP_V2_BOW01);
    ItemIcons.BindSprite( IBOW       , __OBJGROUP_V2_BOW02);
    ItemIcons.BindSprite( IBOW       , __OBJGROUP_V2_BOW03);
    ItemIcons.BindSprite( IBOW       , __OBJGROUP_V2_BOW04);
    ItemIcons.BindSprite( IBOW       , __OBJGROUP_V2_BOW05);
    ItemIcons.BindSprite( IBOW       , __OBJGROUP_V2_BOW06);
    ItemIcons.BindSprite( IBOW       , __OBJGROUP_V2_BOW07);
    ItemIcons.BindSprite( IBOW       , __OBJGROUP_V2_BOW08);
    ItemIcons.BindSprite( IBOW       , __OBJGROUP_V2_BOW09);
    ItemIcons.BindSprite( IBOW       , __OBJGROUP_V2_BOW10);

    ItemIcons.BindSprite( ISTAFF, __OBJGROUP_V2_SCEPTRE01 );
    ItemIcons.BindSprite( ISTAFF, __OBJGROUP_V2_SCEPTRE02 );
    ItemIcons.BindSprite( ISTAFF, __OBJGROUP_V2_SCEPTRE03 );

    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SP01      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SP02      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SP03      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SP04      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SP05      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SP06      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_SP07      );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SP01     );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SP02     );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SP03     );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SP04     );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SP05     );
    ItemIcons.BindSprite( ISWORD     , __OBJGROUP_V2_2SP06     );

    ItemIcons.BindSprite( IWARHAMMER     ,__OBJGROUP_V2_HAMMER01   );
    ItemIcons.BindSprite( IWARHAMMER     ,__OBJGROUP_V2_HAMMER02   );
    ItemIcons.BindSprite( IWARHAMMER     ,__OBJGROUP_V2_HAMMER03   );
    ItemIcons.BindSprite( IWARHAMMER     ,__OBJGROUP_V2_HAMMER04   );
    ItemIcons.BindSprite( IWARHAMMER     ,__OBJGROUP_V2_HAMMER05   );
    ItemIcons.BindSprite( IWARHAMMER     ,__OBJGROUP_V2_HAMMER06   );
    ItemIcons.BindSprite( IWARHAMMER     ,__OBJGROUP_V2_HAMMER07   );
    ItemIcons.BindSprite( IWARHAMMER     ,__OBJGROUP_V2_2HAMMER01  );
    ItemIcons.BindSprite( IWARHAMMER     ,__OBJGROUP_V2_2HAMMER02  );

    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_V2_SHIELD01);
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_V2_SHIELD02);
    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_V2_ICESHIELD);
    

    ItemIcons.BindSprite( IHELMET   , __OBJGROUP_V2_MASQUE_GOBLIN_ICE);
    ItemIcons.BindSprite( IBOW      , __OBJGROUP_V2_BOW05_ICE);
    ItemIcons.BindSprite( ISWORD    , __OBJGROUP_V2_2GLAIVESWORD02_ICE);
    ItemIcons.BindSprite( ISWORD    , __OBJGROUP_V2_GLAIVESWORD02_ICE);

    ItemIcons.BindSprite( IARMOR   ,__OBJGROUP_MAN_ARMOR01_BODY_GR);
    ItemIcons.BindSprite( IARMOR   ,__OBJGROUP_MAN_ARMOR01_BODY_BL);
    ItemIcons.BindSprite( ILEGGINGS,__OBJGROUP_MAN_ARMOR01_LEGS_GR);
    ItemIcons.BindSprite( ILEGGINGS,__OBJGROUP_MAN_ARMOR01_LEGS_BL);
    ItemIcons.BindSprite( IGLOVES  ,__OBJGROUP_MAN_ARMOR01_HAND_GR);
    ItemIcons.BindSprite( IGLOVES  ,__OBJGROUP_MAN_ARMOR01_HAND_BL);
    ItemIcons.BindSprite( IBOOTS   ,__OBJGROUP_MAN_ARMOR01_FOOT_GR);
    ItemIcons.BindSprite( IBOOTS   ,__OBJGROUP_MAN_ARMOR01_FOOT_BL);

    ItemIcons.BindSprite( ISHIELD, __OBJGROUP_T4CP_SHIELD);
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_T4CP_MATRAQUE);
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_T4CP_BAT);
    ItemIcons.BindSprite( IWARHAMMER, __OBJGROUP_T4CP_12);
    



    KEEPALIVE_NM;
    

	// New added by Dialsoft - END
    
    
    //ItemIcons.BindSprite( __OBJGROUP_PORTAL );
    //ItemIcons.BindSprite( __OBJGROUP_VAULT_TALK );
    //ItemIcons.BindSprite( __OBJGROUP_FOUGERE );
    //ItemIcons.BindSprite( __OBJGROUP_KRAANIAN_EGG );
    //ItemIcons.BindSprite( __OBJGROUP_WOLF_PELT                  = 326;
    //ItemIcons.BindSprite( __OBJGROUP_VAULT_TALK_I               = 327;
    //ItemIcons.BindSprite( __OBJGROUP_HUMAN_FOOT                 = 328;
    //ItemIcons.BindSprite( __OBJGROUP_TROLLBADGE                 = 329;
    //ItemIcons.BindSprite( __OBJGROUP_SHOP_INN   		      		= 330;
    //ItemIcons.BindSprite( __OBJGROUP_SHOP_POTION 	      		= 347;
    //ItemIcons.BindSprite( __OBJGROUP_SHOP_PAWN   	      		= 360;
    //ItemIcons.BindSprite( __OBJGROUP_SHOP_ARMOR 	         		= 373;
    //ItemIcons.BindSprite( __OBJGROUP_SHOP_WEAPON 	      		= 390;
    
#define BIND_INV( i, s ) InvItemIcons.BindSprite( s, i );
#define BIND_INV_PAL(i, s, p) InvItemIcons.BindSprite(s, i, p);
    
    BIND_INV( 1, "64kInvShortSword");
    BIND_INV( 2, "64kInvNormalSword");
    BIND_INV( 3, "64kInvFlail");
    BIND_INV( 4, "64kInvMorningStar");
    BIND_INV( 5, "64kInvWarhammer");
    //BIND_INV( 6, "64kInvWeapons 7");
    BIND_INV( 7, "64kInvBattleAxe");
    BIND_INV( 8, "64kInvLeatherArmorBody");
    //BIND_INV( 9, "64kInvDungeonWell");
    BIND_INV( 10, "64kInvChest");
    BIND_INV( 11, "64kInvLeatherArmorHelm");
    BIND_INV( 12, "64kInvMisc1 4");
    BIND_INV( 13, "64kInvMisc1 4"); // Priest Ank
    BIND_INV( 14, "64kInvMisc1 3");
    //BIND_INV( 15, "64kInvRockDoor1");
    //BIND_INV( 16, "64kInvRockDoor11");
    //BIND_INV( 17, "64kInvChairs 3");
    //BIND_INV( 18, "64kInvRockDoor2"); 
    //BIND_INV( 19, "64kInvRockDoor3");
    //BIND_INV( 20, "64kInvRockDoor4");
    //BIND_INV( 21, "64kInvRockDoor5");
    //BIND_INV( 22, "64kInvRockDoor6");
    //BIND_INV( 23, "64kInvRockDoor7");
    //BIND_INV( 24, "64kInvRockDoor8");
    //BIND_INV( 25, "64kInvRockDoor9");
    //BIND_INV( 26, "64kInvRockDoor10");
    BIND_INV( 27, "64kInvMisc 1 - Part 1 1");
    BIND_INV( 28, "64kInvMisc 1 - Part 1 2");
    BIND_INV( 29, "64kInvMisc 1 - Part 1 3");
    BIND_INV( 30, "64kInvMisc 1 - Part 1 4");
    BIND_INV( 31, "64kInvMisc 1 - Part 1 5");
    BIND_INV( 32, "64kInvMisc 1 - Part 1 6");
    BIND_INV( 33, "64kInvMisc 1 - Part 1 7");
    BIND_INV( 34, "64kInvMisc 1 - Part 2 1");
    BIND_INV( 35, "64kInvMisc 1 - Part 2 2");
    BIND_INV( 36, "64kInvMisc 1 - Part 2 3");
    BIND_INV( 37, "64kInvMisc 1 - Part 2 4");
    BIND_INV( 38, "64kInvMisc 1 - Part 2 5");
    BIND_INV( 39, "64kInvMisc 1 - Part 2 6");
    BIND_INV( 40, "64kInvMisc 1 - Part 2 7");
    BIND_INV( 41, "64kInvMisc 2 - All 1");
    BIND_INV( 42, "64kInvMisc 2 - All 2");
    BIND_INV( 43, "64kInvMisc 2 - All 3");
    BIND_INV( 44, "64kInvMisc 2 - All 4");
    BIND_INV( 45, "64kInvMisc 3 - Part 1 1");
    BIND_INV( 46, "64kInvMisc 3 - Part 1 2");
    BIND_INV( 47, "64kInvMisc 3 - Part 1 3");
    BIND_INV( 48, "64kInvMisc 3 - Part 1 4");
    BIND_INV( 49, "64kInvMisc 3 - Part 1 5");
    BIND_INV( 50, "64kInvMisc 3 - Part 1 6");
    BIND_INV( 51, "64kInvMisc 3 - Part 1 7");
    BIND_INV( 52, "64kInvMisc 3 - Part 1 8");
    BIND_INV( 53, "64kInvMisc 3 - Part 1 9");
    BIND_INV( 54, "64kInvMisc 3 - Part 2 1");
    BIND_INV( 55, "64kInvMisc 3 - Part 2 2");
    BIND_INV( 56, "64kInvMisc 3 - Part 2 3");
    BIND_INV( 57, "64kInvMisc 3 - Part 2 4");
    BIND_INV( 58, "64kInvMisc 3 - Part 2 5");
    BIND_INV( 59, "64kInvMisc 3 - Part 2 6");
    BIND_INV( 60, "64kInvMisc 4 - Part 1 1");
    BIND_INV( 61, "64kInvMisc 4 - Part 1 2");
    BIND_INV( 62, "64kInvMisc 4 - Part 1 3");
    BIND_INV( 63, "64kInvMisc 4 - Part 1 4");
    BIND_INV( 64, "64kInvMisc 4 - Part 1 5");
    BIND_INV( 65, "64kInvMisc 4 - Part 1 6");
    BIND_INV( 66, "64kInvMisc 4 - Part 1 7");
    BIND_INV( 67, "64kInvMisc 4 - Part 1 8");
    BIND_INV( 68, "64kInvMisc 4 - Part 1 9");
    BIND_INV( 69, "64kInvMisc 4 - Part 1 10");
    BIND_INV( 70, "64kInvMisc 4 - Part 2 1");
    BIND_INV( 71, "64kInvMisc 4 - Part 2 3");
    BIND_INV( 72, "64kInvMisc 4 - Part 2 4");
    BIND_INV( 73, "64kInvMisc 6 - All 1");
    BIND_INV( 74, "64kInvMisc 6 - All 2");
    BIND_INV( 75, "64kInvMisc 6 - All 3");
    BIND_INV( 76, "64kInvMisc 6 - All 4");
    BIND_INV( 77, "64kInvMisc 6 - All 5");
    BIND_INV( 78, "64kInvMisc 6 - All 6");
    BIND_INV( 79, "64kInvMisc 6 - All 7");
    BIND_INV( 80, "64kInvMisc 6 - All 8");
    BIND_INV( 81, "64kInvMisc 7 - All 1");
    BIND_INV( 82, "64kInvMisc 7 - All 2");
    BIND_INV( 83, "64kInvMisc 7 - All 3");
    BIND_INV( 84, "64kInvMisc 7 - All 4");
    BIND_INV( 85, "64kInvMisc 7 - All 5");
    BIND_INV( 86, "64kInvShortBow" );
    BIND_INV( 87, "64kInvMisc 7 - All 7");
    BIND_INV( 88, "64kInvMisc 7 - All 9");
    BIND_INV( 89, "64kInvMisc 7 - All 10");
    BIND_INV( 90, "64kInvMisc 8 - All 1");
    BIND_INV( 91, "64kInvMisc 8 - All 2");
    BIND_INV( 92, "64kInvMisc 8 - All 5");
    BIND_INV( 93, "64kInvMisc 8 - All 6");
    BIND_INV( 94, "64kInvMisc 8 - All 7");
    BIND_INV( 95, "64kInvMisc 8 - All 8");
    BIND_INV( 96, "64kInvMisc 9 - All 1");
    BIND_INV( 97, "64kInvMisc 9 - All 2");
    BIND_INV( 98, "64kInvMisc 9 - All 3");
    BIND_INV( 99, "64kInvMisc 9 - All 4");
    BIND_INV( 100, "64kInvMisc 9 - All 5");
    BIND_INV( 101, "64kInvMisc 9 - All 6");
    BIND_INV( 102, "64kInvMisc 9 - All 7");
    BIND_INV( 103, "64kInvMisc 9 - All 8");
    KEEPALIVE_NM;
    //BIND_INV( 104, "64kInvChairs 2");
    //BIND_INV( 105, "64kInvChairs 4");
    //BIND_INV( 106, "64kInvChairs 5");
    BIND_INV( 107, "64kInvCrates 1");
    BIND_INV( 108, "64kInvCrates 2");
    BIND_INV( 109, "64kInvCrates 3");
    BIND_INV( 110, "64kInvKitchen 2");
    BIND_INV( 111, "64kInvKitchen 3");
    BIND_INV( 112, "64kInvKitchen 4");
    BIND_INV( 113, "64kInvKitchen 5");
    BIND_INV( 114, "64kInvKitchen 6");
    BIND_INV( 115, "64kInvLights 1");
    BIND_INV( 116, "64kInvLights 2");
    BIND_INV( 117, "64kInvMirrors 1");
    BIND_INV( 118, "64kInvWoodenStaff");
    BIND_INV( 119, "64kInvMace");
    BIND_INV( 120, "64kInvSpiked Mace");
    BIND_INV( 121, "64kInvMaul");
    BIND_INV( 122, "64kInvDouble Axe");
    BIND_INV( 123, "64kInvSingle Axe");     
    BIND_INV( 124, "64kInvWeapon 15");
    BIND_INV( 125, "64kInvSpear Axe");
    BIND_INV( 126, "64kInvMisc1 1");
    BIND_INV( 127, "64kInvMisc1 2");
    BIND_INV( 128, "64kInvMisc1 5");
    BIND_INV( 129, "64kInvMisc1 6");
    //BIND_INV( 130, "64kInvBrickFrame2");
    //	BIND_INV( 131, "64kInvWall7tm 2 1"); 
    //	BIND_INV( 132, "64kInvWall7tm 2 1");
    //	BIND_INV( 131, "64kInvStuccoMainWallDoor 1");
    //	BIND_INV( 132, "64kInvStuccoDarkWallDoor 1");
    //BIND_INV( 135, "64kInvShito 3");
    //BIND_INV( 136, "64kInvShito 4");
    //BIND_INV( 137, "64kInvShito 5");
    BIND_INV( 141, "64kInvPouch");
    BIND_INV( 142, "64kInvRope");
    BIND_INV( 143, "64kInvStuffedBaskets 1");
    BIND_INV( 144, "64kInvStuffedBaskets 2");
    BIND_INV( 145, "64kInvStuffedBaskets 3");
    BIND_INV( 146, "64kInvFruits 1");
    //BIND_INV( 147, "64kInvFruits 2");
    //BIND_INV( 148, "64kInvFruits 3");
    //BIND_INV( 149, "64kInvFruits 4");
    //BIND_INV( 150, "64kInvFruits 5");
    //BIND_INV( 151, "64kInvFruits 6");
    //BIND_INV( 152, "64kInvFruits 7");
    //BIND_INV( 153, "64kInvFruits 8");
    //BIND_INV( 154, "64kInvFruits 9");
    //BIND_INV( 155, "64kInvFruits 10");
    //BIND_INV( 156, "64kInvFruits 11");
    //BIND_INV( 157, "64kInvFruits 12");
    //BIND_INV( 158, "64kInvFruits 13");
    //BIND_INV( 159, "64kInvFruits 14");
    BIND_INV( 160, "64kInvCoins 1");
    BIND_INV( 161, "64kInvCoins 2");
    //BIND_INV( 162, "64kInvCoins 3");
    BIND_INV( 163, "64kInvDecayingBatWings");
    BIND_INV( 164, "64kInvGems 1");
    BIND_INV( 165, "64kInvGems 2");
    BIND_INV( 166, "64kInvGems 3");
    BIND_INV( 167, "64kInvGems 4");
    //BIND_INV( 168, "64kInvGems 5");
    //BIND_INV( 169, "64kInvGems 6");
    //BIND_INV( 170, "64kInvGems 7");
    BIND_INV( 171, "64kInvKoboldHairs");
    BIND_INV( 172, "64kInvNecklace 1");
    BIND_INV( 173, "64kInvNecklace 2");
    BIND_INV( 174, "64kInvNecklace 3");
    BIND_INV( 175, "64kInvOrcFeet");
    BIND_INV( 176, "64kInvRings 1");
    BIND_INV( 177, "64kInvRings 2");
    BIND_INV( 178, "64kInvRings 3");
    BIND_INV( 179, "64kInvRings 4");
    BIND_INV( 180, "64kInvRings 5");
    BIND_INV( 181, "64kInvSkeletonBone");
    BIND_INV( 182, "64kInvSpiderEyes");
    //BIND_INV( 183, "64kInvPadded 1");
    //BIND_INV( 184, "64kInvPadded 2");
    //BIND_INV( 185, "64kInvPadded 3");
    //BIND_INV( 186, "64kInvPadded 4");
    //BIND_INV( 187, "64kInvPadded 5");
    BIND_INV( 188, "64kInvScale");
    //BIND_INV( 189, "64kInvLever");
    BIND_INV( 190, "64kInvTorch"); //Ajout d'une icone dans le sac pour le feu de camp
    //BIND_INV( 191, "64kInvCamp2");
    //BIND_INV( 192, "64kInvCamp3");
    //BIND_INV( 193, "64kInvCamp4");
    //BIND_INV( 194, "64kInvCamp5");
    //BIND_INV( 195, "64kInvCamp6");
    //BIND_INV( 196, "64kInvCamp7");
    //BIND_INV( 197, "64kInvCamp8");
    //BIND_INV( 198, "64kInvCamp9");
    //BIND_INV( 199, "64kInvCamp10");
    //BIND_INV( 200, "64kInvCamp11");
    //BIND_INV( 201, "64kInvCampFireBurnt");
    BIND_INV( 202, "64kInvGlinting Sword");
    BIND_INV( 203, "64kInvDestinyGem");
    BIND_INV( 204, "64kInvVolcanoRock");
    BIND_INV( 205, "64kInvRingMail 1");
    BIND_INV( 206, "64kInvRingMail 4");
    BIND_INV( 207, "64kInvRingMail 2");
    BIND_INV( 208, "64kInvRingMail 3");
    BIND_INV( 209, "64kInvFeather");
    BIND_INV( 210, "64kInvIceShard");
    BIND_INV( 211, "64kInvLifeGem");
    BIND_INV( 212, "64kInvOakTreeLeaf");
    /*
    BIND_INV( 213, "64kInvPadded 6");
    BIND_INV( 214, "64kInvDungeonTorch-a");
    BIND_INV( 215, "64kInvDungeonTorch-b");
    BIND_INV( 216, "64kInvDungeonTorch-c");
    BIND_INV( 217, "64kInvDungeonTorch-d");
    BIND_INV( 218, "64kInvDungeonTorch-e");
    BIND_INV( 219, "64kInvDungeonTorch-f");
    BIND_INV( 220, "64kInvDungeonTorch-g");
    BIND_INV( 221, "64kInvDungeonTorch-h");
    BIND_INV( 222, "64kInvDungeonTorch-i");
    BIND_INV( 223, "64kInvDungeonTorch-j");
    BIND_INV( 224, "64kInvDungeonTorch-k");
    BIND_INV( 225, "64kInvDungeonTorch-l");
    BIND_INV( 226, "64kInvDungeonTorch-m");
    BIND_INV( 227, "64kInvDungeonTorch-n");
    BIND_INV( 228, "64kInvDungeonTorch-o");
    BIND_INV( 229, "64kInvDungeonTorch-p");
    BIND_INV( 230, "64kInvDungeonTorch-q");
    BIND_INV( 231, "64kInvDungeonTorch-r");
    BIND_INV( 232, "64kInvDungeonTorch-s");
    BIND_INV( 233, "64kInvDungeonTorch-t");
    BIND_INV( 234, "64kInvDungeonTorch-u");
    */
    BIND_INV( 235, "64kInvBelt");
    BIND_INV( 236, "64kInvBlueFlask");
    BIND_INV( 237, "64kInvBracelet");
    BIND_INV( 238, "64kInvChest");
    BIND_INV( 239, "64kInvPinkLeaf");
    BIND_INV( 240, "64kInvPotion 1");    //(rouge)
    BIND_INV( 241, "64kInvPotion 2");    //(verte)
    BIND_INV( 242, "64kInvShield");
    BIND_INV( 243, "64kInvStoneLife");
    BIND_INV( 244, "64kInvTorch");
    BIND_INV( 245, "64kInvTooth");
    BIND_INV( 246, "64kInvGodlyPotion 1");
    BIND_INV( 247, "64kInvGodlyPotion 2");
    BIND_INV( 248, "64kInvGodlyPotion 3");
    BIND_INV( 249, "64kInvGodlyPotion 4");
    BIND_INV( 250, "64kInvPotions 8");
    BIND_INV( 251, "64kInvPotions 9");
    BIND_INV( 252, "64kInvPotions 2");
    BIND_INV( 253, "64kInvPotions 3");
    BIND_INV( 254, "64kInvPotions 4");
    BIND_INV( 255, "64kInvPotions 5");
    BIND_INV( 256, "64kInvPotions 1");
    BIND_INV( 257, "64kInvPotions 7");
    BIND_INV( 258, "64kInvPotions 6");
    BIND_INV( 259, "64kInvLeatherArmorGloves");
    BIND_INV( 260, "64kInvLeatherArmorBoots");
    BIND_INV( 261, "64kInvLeatherArmorLegs");
    BIND_INV( 262, "64kInvPlateArmorSleeves");//"64kInvPlateArmorBody");//NAD
    BIND_INV( 263, "64kInvPlateGlove");
    BIND_INV( 264, "64kInvPlateArmorSleeves");
    BIND_INV( 265, "64kInvPlateArmorFeet");
    BIND_INV( 266, "64kInvPlateArmorLegs");
    BIND_INV( 267, "64kInvPlateArmorHelm");
    BIND_INV( 268, "64kInvChainMailLegs");
    BIND_INV( 269, "64kInvChainMailBody");
    BIND_INV( 270, "64kInvChainMailHelm");
    BIND_INV( 271, "64kInvDarkSword");
    BIND_INV( 272, "64kInvRomanShield");
    BIND_INV( 273, "64kInvBarossaShield");
    BIND_INV( 274, "64kInvBattleDagger");
    BIND_INV( 275, "64kInvRealDarkSword");
    BIND_INV( 276, "64kInvHornedHelmet");
    BIND_INV( 277, "64kInvBattleSword");
    BIND_INV( 278, "64kInvNecromanRobe");
    BIND_INV( 279, "64kInvGoldenCrown");
    BIND_INV( 280, "64kInvGoldenMorningStar");
    BIND_INV( 281, "64kInvElvenHat");
    BIND_INV( 282, "64kInvOrcShield");
    BIND_INV( 283, "64kInvStuddedLeatherBody");
    BIND_INV( 284, "64kInvClothSet1Legs");
    BIND_INV( 285, "64kInvClothSet1Body");
    BIND_INV( 286, "64kInvStuddedLeatherLegs");
    BIND_INV( 287, "64kInvRedCape");
    BIND_INV( 288, "64kInvBlackLeatherBoots");
    BIND_INV( 289, "64kInvHead");
    //BIND_INV( 290, "64kInvHorloge Solaire");
    //BIND_INV( 291, "64kInvSign1");
    //BIND_INV( 292, "64kInvSign2");
    //BIND_INV( 293, "64kInvSign3");
    BIND_INV( 294, "64kInvLichStaff");
    BIND_INV( 295, "64kInvGemStaff");
    BIND_INV( 296, "64kInvSimpleStaff");
    /*
    BIND_INV( 297, "64kInvSimplePortal-a");
    BIND_INV( 298, "64kInvSimplePortal-b");
    BIND_INV( 299, "64kInvSimplePortal-c");
    BIND_INV( 300, "64kInvSimplePortal-d");
    BIND_INV( 301, "64kInvSimplePortal-e");
    BIND_INV( 302, "64kInvSimplePortal-f");
    BIND_INV( 303, "64kInvSimplePortal-g");
    BIND_INV( 304, "64kInvSimplePortal-h");
    BIND_INV( 305, "64kInvSimplePortal-i");
    BIND_INV( 306, "64kInvSimplePortal-j");
    BIND_INV( 307, "64kInvSimplePortal-k");
    BIND_INV( 308, "64kInvSimplePortal-l");
    BIND_INV( 309, "64kInvSimplePortal-m");
    BIND_INV( 310, "64kInvSimplePortal-n");
    BIND_INV( 311, "64kInvSimplePortal-o");
    BIND_INV( 312, "64kInvSimplePortal-p");
    BIND_INV( 313, "64kInvSimplePortal-q");
    BIND_INV( 314, "64kInvSimplePortal-r");
    BIND_INV( 315, "64kInvSimplePortal-s");
    BIND_INV( 316, "64kInvSimplePortal-t");
    BIND_INV( 317, "64kInvSimplePortal-u");
    BIND_INV( 318, "64kInvSimplePortal-v");
    BIND_INV( 319, "64kInvSimplePortal-w");
    BIND_INV( 320, "64kInvSimplePortal-x");
    BIND_INV( 321, "64kInvSimplePortal-y");
    BIND_INV( 322, "64kInvSimplePortal-z");
    */
    BIND_INV( 323, "64kInvVault");
    BIND_INV( 324, "64kInvSmallTree1");
    BIND_INV( 325, "64kInvKraanianEgg");
    BIND_INV( 326, "64kInvWolfPelt");
    BIND_INV( 328, "64kInvHumanFoot");
    BIND_INV( 329, "64kInvTrollBadge");
    BIND_INV( __OBJGROUP_QUIVER, "64kInvQuiver" );
    BIND_INV( __OBJGROUP_LARGE_BOW, "64kInvLongBow" );
    BIND_INV( __OBJGROUP_REDROBE, "64kInvRedRobe");
    BIND_INV( __OBJGROUP_OGRECLUB, "64kInvOgreClub");
    BIND_INV( __OBJGROUP_LARGE_BOW2, "64kInvLongBow2" );
    BIND_INV( __OBJGROUP_FANCY_SHORT_BOW, "64kInvShortBowFancy" );
    BIND_INV( __OBJGROUP_FANCY_LONG_BOW, "64kInvLongBowFancy" );
    InvItemIcons.BindSprite( "64kInvQuiver", __OBJGROUP_BLUE_QUIVER, 2 );
    InvItemIcons.BindSprite( "64kInvQuiver", __OBJGROUP_YELLOW_QUIVER, 3 );
    InvItemIcons.BindSprite( "64kInvQuiver", __OBJGROUP_GREEN_QUIVER, 4 );
    InvItemIcons.BindSprite( "64kInvQuiver", __OBJGROUP_RED_QUIVER, 5 );
    InvItemIcons.BindSprite( "64kInvQuiver", __OBJGROUP_BLACK_QUIVER, 6 );
    BIND_INV( __OBJGROUP_MAGEROBE, "64kInvMageRobe" );
    BIND_INV( __OBJGROUP_WHITEROBE, "64kInvWhiteRobe" );
    BIND_INV( __OBJGROUP_CENTAUR_SHIELD1, "64kInvCentaurShield1" );
    BIND_INV( __OBJGROUP_CENTAUR_SHIELD2, "64kInvCentaurShield2" );
    BIND_INV( __OBJGROUP_SHAMAN_HELM, "64kInvShamanHelm" );
    BIND_INV( __OBJGROUP_SKAVEN_CLUB, "64kInvSkavenClub" );
    BIND_INV( __OBJGROUP_SKAVEN_KNIFE, "64kInvSkavenKnife" );
    BIND_INV( __OBJGROUP_SKAVEN_SHIELD1, "64kInvSkavenShield1" );
    BIND_INV( __OBJGROUP_SKAVEN_SHIELD2, "64kInvSkavenShield2" );
    BIND_INV( __OBJGROUP_SKAVEN_SHIELD3, "64kInvSkavenShield3" );
    BIND_INV( __OBJGROUP_SKELETON_AXE, "64kInvSkeletonAxe" );
    BIND_INV( __OBJGROUP_SKELETON_HELM, "64kInvSkeletonHelm" );
    BIND_INV( __OBJGROUP_SKELETON_SHIELD, "64kInvSkeletonShield" );
    BIND_INV( __OBJGROUP_ORC_SHIELD2,   "Inv_OrcShield" );
    BIND_INV( __OBJGROUP_SK_SHIELD,   "Inv_SkShield" );
    BIND_INV( __OBJGROUP_SKELETON_SWORD, "64kInvSkeletonSword" );
    BIND_INV( __OBJGROUP_SERAPH_BLACK_WINGS, "64kInvSeraphBlackWings" );
    BIND_INV( __OBJGROUP_SERAPH_WHITE_WINGS, "64kInvSeraphWhiteWings" );
    BIND_INV( __OBJGROUP_SERAPH_DECHU_WINGS, "Inv_NMDechuWings" );
    BIND_INV( __OBJGROUP_SERAPH_DECHU2_WINGS, "Inv_NM2DechuWings" );
    
    BIND_INV( __OBJGROUP_DARK_GEM, "64kInvDarkGem" );
    BIND_INV( __OBJGROUP_PURPLE_GEM, "64kInvPurpleGem" );
    BIND_INV( __OBJGROUP_RED_GEM, "64kInvRedGem" );
    BIND_INV( __OBJGROUP_HALBERD, "64kInvHalberd" );
    BIND_INV( __OBJGROUP_SMALL_PURPLE_GEM, "64kInvSmallGemPurple" );
    BIND_INV( __OBJGROUP_SMALL_RED_GEM, "64kInvSmallGemRed" );
    BIND_INV( __OBJGROUP_HEART, "Object_Inventory_Heart");
	//New added by Dialsoft - BEGIN
	BIND_INV( __OBJGROUP_BRACELET_RUBIS, "InvBraceletRubis");
	BIND_INV( __OBJGROUP_BELT_JEWELS_BROWN, "InvBeltBrownJewels");
	BIND_INV( __OBJGROUP_BELT_JEWELS_PURPLE, "InvBeltVioletJewels");
	BIND_INV( __OBJGROUP_BELT_RED, "InvBeltRed");
	BIND_INV( __OBJGROUP_BRACELET_AZURE, "InvBraceletAzure");
	BIND_INV( __OBJGROUP_BRACELET_SQUARE_WHITE, "InvBraceletSquareWhite");
	BIND_INV( __OBJGROUP_GEM_AZURE, "InvGemAzure");
	BIND_INV( __OBJGROUP_GEM_ORANGE, "InvGemOrange");
	BIND_INV( __OBJGROUP_GEM_PINK, "InvGemPink");
	BIND_INV( __OBJGROUP_RING_3RUBIS, "InvRingMulti");
	BIND_INV( __OBJGROUP_RING_RED, "InvRingRed");
	BIND_INV( __OBJGROUP_RING_STARGOLD, "InvRingStarGold");
	BIND_INV( __OBJGROUP_MASQUE_GOBLIN, "InvGobMask");
   BIND_INV_PAL( __OBJGROUP_V2_MASQUE_GOBLIN_ICE, "InvGobMask",2);
	BIND_INV( __OBJGROUP_RING_CUT_GREEN, "InvRingCutGreen");
	BIND_INV( __OBJGROUP_RING_CUT_RED, "InvRingCutRed");
	BIND_INV( __OBJGROUP_RUBIS_FOCUS, "InvRubisFocus");
	BIND_INV( __OBJGROUP_EMERALD_FOCUS, "InvEmeraldFocus");
	BIND_INV( __OBJGROUP_DIAMOND_FOCUS, "InvDiamondFocus");
	BIND_INV( __OBJGROUP_SAPHIRE_FOCUS, "InvSaphireFocus");
	BIND_INV( __OBJGROUP_EYE_OF_TIGER, "InvEyeOfTiger");
	BIND_INV( __OBJGROUP_MANA_STONE, "InvManaStone");
	BIND_INV( __OBJGROUP_BLUE_STONE, "InvBlueStone");
	BIND_INV( __OBJGROUP_REDFEATHER, "InvRedFeHelmet");
	BIND_INV( __OBJGROUP_FIREFLAIL, "InvFireFlail");
	BIND_INV( __OBJGROUP_SPIKEDLEATHER, "InvSpikedLeatherBody");
	BIND_INV( __OBJGROUP_DARKWINGS, "64kInvSeraphBlackWings" );


	BIND_INV_PAL( __OBJGROUP_SERAPH_RED_WINGS, "64kInvSeraphWhiteWings", 2 );
	BIND_INV_PAL( __OBJGROUP_SERAPH_NEUTRAL_WINGS, "64kInvSeraphWhiteWings", 3 );
	BIND_INV_PAL( __OBJGROUP_SERAPH_GOLD_WINGS, "64kInvSeraphWhiteWings", 4 );
	BIND_INV_PAL( __OBJGROUP_SERAPH_DARKRED_WINGS, "64kInvSeraphWhiteWings", 5 );
	BIND_INV_PAL( __OBJGROUP_SERAPH_DARKGOLD_WINGS, "64kInvSeraphWhiteWings", 6 );
	BIND_INV_PAL( __OBJGROUP_SERAPH_LIGHTBLUE_WINGS, "64kInvSeraphWhiteWings", 7 );
	BIND_INV_PAL( __OBJGROUP_SERAPH_ORANGE_WINGS, "64kInvSeraphWhiteWings", 8 );
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_BLUE, "64kInvWhiteRobe", 2 );
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_GREEN, "64kInvWhiteRobe", 3 );
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_DARKBLUE, "64kInvWhiteRobe", 4 );
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_VIOLET, "64kInvWhiteRobe", 5 );
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_PURPLE, "64kInvWhiteRobe", 6 );
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_YELLOW, "64kInvWhiteRobe", 7 );
   KEEPALIVE_NM;
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_GRAY, "64kInvWhiteRobe", 8 );
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_MAUVE, "64kInvWhiteRobe", 9 );
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_ORANGE, "64kInvWhiteRobe", 10 );
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_BRUN, "64kInvWhiteRobe", 11 );
	BIND_INV_PAL( __OBJGROUP_FWHITEROBE_DARK, "64kInvWhiteRobe", 12 );
	//Added By Tiamat
	BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_GOLD, "64kInvPlateArmorSleeves", 2);
    BIND_INV_PAL( __OBJGROUP_PLATE_GLOVE_GOLD, "64kInvPlateGlove", 2);
    BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_W_SLEEVE_GOLD, "64kInvPlateArmorSleeves", 2);
    BIND_INV_PAL( __OBJGROUP_PLATE_BOOT_GOLD, "64kInvPlateArmorFeet", 2);
    BIND_INV_PAL( __OBJGROUP_PLATE_LEGS_GOLD, "64kInvPlateArmorLegs", 2);
    BIND_INV_PAL( __OBJGROUP_PLATE_HELM_GOLD, "64kInvPlateArmorHelm", 2);
	BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_SILVER, "64kInvPlateArmorSleeves", 3);
    BIND_INV_PAL( __OBJGROUP_PLATE_GLOVE_SILVER, "64kInvPlateGlove", 3);
    BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_W_SLEEVE_SILVER, "64kInvPlateArmorSleeves", 3);
    BIND_INV_PAL( __OBJGROUP_PLATE_BOOT_SILVER, "64kInvPlateArmorFeet", 3);
    BIND_INV_PAL( __OBJGROUP_PLATE_LEGS_SILVER, "64kInvPlateArmorLegs", 3);
    BIND_INV_PAL( __OBJGROUP_PLATE_HELM_SILVER, "64kInvPlateArmorHelm", 3);
	BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_BLACK, "64kInvPlateArmorSleeves", 4);
    BIND_INV_PAL( __OBJGROUP_PLATE_GLOVE_BLACK, "64kInvPlateGlove", 4);
    BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLACK, "64kInvPlateArmorSleeves", 4);
    BIND_INV_PAL( __OBJGROUP_PLATE_BOOT_BLACK, "64kInvPlateArmorFeet", 4);
    BIND_INV_PAL( __OBJGROUP_PLATE_LEGS_BLACK, "64kInvPlateArmorLegs", 4);
    BIND_INV_PAL( __OBJGROUP_PLATE_HELM_BLACK, "64kInvPlateArmorHelm", 4);
	BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_BLUE, "64kInvPlateArmorSleeves", 5);
    BIND_INV_PAL( __OBJGROUP_PLATE_GLOVE_BLUE, "64kInvPlateGlove", 5);
    BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLUE, "64kInvPlateArmorSleeves", 5);
    BIND_INV_PAL( __OBJGROUP_PLATE_BOOT_BLUE, "64kInvPlateArmorFeet", 5);
    BIND_INV_PAL( __OBJGROUP_PLATE_LEGS_BLUE, "64kInvPlateArmorLegs", 5);
    BIND_INV_PAL( __OBJGROUP_PLATE_HELM_BLUE, "64kInvPlateArmorHelm", 5);
	BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_GREEN, "64kInvPlateArmorSleeves", 6);
    BIND_INV_PAL( __OBJGROUP_PLATE_GLOVE_GREEN, "64kInvPlateGlove", 6);
    BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_W_SLEEVE_GREEN, "64kInvPlateArmorSleeves", 6);
    BIND_INV_PAL( __OBJGROUP_PLATE_BOOT_GREEN, "64kInvPlateArmorFeet", 6);
    BIND_INV_PAL( __OBJGROUP_PLATE_LEGS_GREEN, "64kInvPlateArmorLegs", 6);
    BIND_INV_PAL( __OBJGROUP_PLATE_HELM_GREEN, "64kInvPlateArmorHelm", 6);
	BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_RED, "64kInvPlateArmorSleeves", 7);
    BIND_INV_PAL( __OBJGROUP_PLATE_GLOVE_RED, "64kInvPlateGlove", 7);
    BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_W_SLEEVE_RED, "64kInvPlateArmorSleeves", 7);
    BIND_INV_PAL( __OBJGROUP_PLATE_BOOT_RED, "64kInvPlateArmorFeet", 7);
    BIND_INV_PAL( __OBJGROUP_PLATE_LEGS_RED, "64kInvPlateArmorLegs", 7);
    BIND_INV_PAL( __OBJGROUP_PLATE_HELM_RED, "64kInvPlateArmorHelm", 7);
    BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_PURPLE, "64kInvPlateArmorSleeves", 8);
    BIND_INV_PAL( __OBJGROUP_PLATE_GLOVE_PURPLE, "64kInvPlateGlove", 8);
    BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_W_SLEEVE_PURPLE, "64kInvPlateArmorSleeves", 8);
    BIND_INV_PAL( __OBJGROUP_PLATE_BOOT_PURPLE, "64kInvPlateArmorFeet", 8);
    BIND_INV_PAL( __OBJGROUP_PLATE_LEGS_PURPLE, "64kInvPlateArmorLegs", 8);
    BIND_INV_PAL( __OBJGROUP_PLATE_HELM_PURPLE, "64kInvPlateArmorHelm", 8);
    BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_EMERALD, "64kInvPlateArmorSleeves", 9);
    BIND_INV_PAL( __OBJGROUP_PLATE_GLOVE_EMERALD, "64kInvPlateGlove", 9);
    BIND_INV_PAL( __OBJGROUP_PLATE_ARMOR_W_SLEEVE_EMERALD, "64kInvPlateArmorSleeves", 9);
    BIND_INV_PAL( __OBJGROUP_PLATE_BOOT_EMERALD, "64kInvPlateArmorFeet", 9);
    BIND_INV_PAL( __OBJGROUP_PLATE_LEGS_EMERALD, "64kInvPlateArmorLegs", 9);
    BIND_INV_PAL( __OBJGROUP_PLATE_HELM_EMERALD, "64kInvPlateArmorHelm", 9);
    BIND_INV( __OBJGROUP_TIGERAXE, "64kInvSkeletonAxe");
    BIND_INV( __OBJGROUP_ANCIENTAXE, "64kInvSkeletonAxe");
    BIND_INV( __OBJGROUP_FLOWERS, "InvFireFlail");
    BIND_INV( __OBJGROUP_NECROSTAFF, "64kInvLichStaff");
   	// ADDED BY LUCIFER
    BIND_INV_PAL( __OBJGROUP_GREENCAPE, "64kInvRedCape", 2);
    BIND_INV_PAL( __OBJGROUP_YELLOWCAPE, "64kInvRedCape", 3);
    BIND_INV_PAL( __OBJGROUP_SILVERCAPE, "64kInvRedCape", 4);
    BIND_INV_PAL( __OBJGROUP_PURPLECAPE, "64kInvRedCape", 5);
    BIND_INV_PAL( __OBJGROUP_BLACKCAPE, "64kInvRedCape", 6);
    BIND_INV_PAL( __OBJGROUP_AQUACAPE, "64kInvRedCape", 7);
    BIND_INV_PAL( __OBJGROUP_BLUECAPE, "64kInvRedCape", 8);
    BIND_INV( __OBJGROUP_WITCH_HAT1, "inv_WitchHat1");
    BIND_INV( __OBJGROUP_WITCH_HAT2, "inv_WitchHat2");
    BIND_INV( __OBJGROUP_WITCH_HAT3, "inv_WitchHat3");
    BIND_INV( __OBJGROUP_WITCH_HAT4, "inv_WitchHat4");
    BIND_INV( __OBJGROUP_WITCH_HAT5, "inv_WitchHat5");
    BIND_INV( __OBJGROUP_NOEL_HAT, "inv_NoelHat");
    BIND_INV( __OBJGROUP_T4CP_CASQUE, "Inv_T4CP_Casque");
    BIND_INV( __OBJGROUP_T4CP_CASQUETTE, "Inv_T4CP_Casquette");
    
       
    

    BIND_INV( __OBJGROUP_NEW_BOW01   ,   "Inv_NewBow01" );
	
    BIND_INV( __OBJGROUP_DWARF_HAMMER,   "Inv_DwarfHammer");
    BIND_INV( __OBJGROUP_SWORD_ANGEL,    "Inv_SwordAngel");
    BIND_INV( __OBJGROUP_CLAY2BLADES,    "Inv_Clay2Blades");
    BIND_INV( __OBJGROUP_SWORD_HERO,     "Inv_SwordAngel");
    BIND_INV( __OBJGROUP_AXE_DESTRUCTION,"Inv_AxeDestruction");
    

    BIND_INV( __OBJGROUP_BUTTERFLY_WING1,"Inv_ButterFlyWing");
    BIND_INV_PAL( __OBJGROUP_BUTTERFLY_WING2,"Inv_ButterFlyWing",2);
    BIND_INV_PAL( __OBJGROUP_BUTTERFLY_WING3,"Inv_ButterFlyWing",3);
    BIND_INV_PAL( __OBJGROUP_BUTTERFLY_WING4,"Inv_ButterFlyWing",4);

    BIND_INV( __OBJGROUP_HAMM,    "Inv_Hamm");

    BIND_INV( __OBJGROUP_ARCHWINGS_WHITE,"Inv_ArchWingsW");
    BIND_INV( __OBJGROUP_LICHROBE,  "Inv_LichRobeOri"); //BLBLBL : 25 mars 2009 : ajouté une image d'inventaire pour la lich robe, teinte bleue
    BIND_INV( __OBJGROUP_ARMORED_ROBE , "Inv_ArmoredRobe");

    BIND_INV( __OBJGROUP_V2_HAUME01 , "Inv_V2_Haume01");
    BIND_INV( __OBJGROUP_V2_HAUME02 , "Inv_V2_Haume02");
    BIND_INV( __OBJGROUP_V2_HAUME03 , "Inv_V2_Haume03");
    BIND_INV( __OBJGROUP_V2_HAUME04 , "Inv_V2_Haume04");
    BIND_INV_PAL( __OBJGROUP_V2_HAUME04_BL , "Inv_V2_Haume04",2);
    BIND_INV( __OBJGROUP_V2_HAT01   , "Inv_V2_Hat01");
    BIND_INV( __OBJGROUP_V2_MASK01  , "Inv_V2_Mask01");

    BIND_INV( __OBJGROUP_V2_DAGGER_VIP, "Inv_V2_Viperine");
    BIND_INV( __OBJGROUP_V2_DAGGER01  , "Inv_V2_dague01");
    BIND_INV( __OBJGROUP_V2_DAGGER02  , "Inv_V2_dague02");
    BIND_INV( __OBJGROUP_V2_DAGGER03  , "Inv_V2_dague03");
    BIND_INV( __OBJGROUP_V2_DAGGER04  , "Inv_V2_dague04");
    BIND_INV( __OBJGROUP_V2_DAGGER05  , "Inv_V2_dague05");

    //NM_TAG_ADD_SWORD
    BIND_INV( __OBJGROUP_V2_BUSSWORD01   , "Inv_V2_BusterSlayer01");
    BIND_INV( __OBJGROUP_V2_BUSSWORD02   , "Inv_V2_BusterSlayer02");
    BIND_INV( __OBJGROUP_V2_BUSSWORD03   , "Inv_V2_BusterSlayer03");
    BIND_INV( __OBJGROUP_V2_CLAYSWORD01  , "Inv_V2_Claymore01");
    BIND_INV( __OBJGROUP_V2_CLAYSWORD02  , "Inv_V2_Claymore02");
    
    BIND_INV( __OBJGROUP_V2_GLAIVESWORD01, "Inv_V2_GlaiveSlayer01");
    BIND_INV( __OBJGROUP_V2_GLAIVESWORD02, "Inv_V2_GlaiveSlayer02");
    BIND_INV( __OBJGROUP_V2_GLAIVESWORD02_ICE, "Inv_V2_IceSword01");

    BIND_INV( __OBJGROUP_T4CP_MATRAQUE, "Inv_T4CP_Matraque");
    BIND_INV( __OBJGROUP_T4CP_BAT     , "Inv_TankBat");
    BIND_INV( __OBJGROUP_T4CP_12      , "Inv_T4CP_12");
    

    BIND_INV( __OBJGROUP_V2_SWORD01      , "Inv_V2_Sword01");
    BIND_INV( __OBJGROUP_V2_SWORD02      , "Inv_V2_Sword02");
    BIND_INV( __OBJGROUP_V2_SWORD03      , "Inv_V2_Sword03");
    BIND_INV( __OBJGROUP_V2_SWORD04      , "Inv_V2_Sword04");
    BIND_INV( __OBJGROUP_V2_SWORD05      , "Inv_V2_Sword05");
    BIND_INV( __OBJGROUP_V2_SWORD06      , "Inv_V2_Sword06");
    BIND_INV( __OBJGROUP_V2_SWORD07      , "Inv_V2_Sword07");
    BIND_INV( __OBJGROUP_V2_SWORD08      , "Inv_V2_Sword08");

    BIND_INV( __OBJGROUP_V2_2BUSSWORD01   , "Inv_V2_BusterSlayer01");
    BIND_INV( __OBJGROUP_V2_2BUSSWORD02   , "Inv_V2_BusterSlayer02");
    BIND_INV( __OBJGROUP_V2_2BUSSWORD03   , "Inv_V2_BusterSlayer03");
    BIND_INV( __OBJGROUP_V2_2CLAYSWORD01  , "Inv_V2_Claymore01");
    BIND_INV( __OBJGROUP_V2_2CLAYSWORD02  , "Inv_V2_Claymore02");

    BIND_INV( __OBJGROUP_V2_2GLAIVESWORD01, "Inv_V2_GlaiveSlayer01");
    BIND_INV( __OBJGROUP_V2_2GLAIVESWORD02, "Inv_V2_GlaiveSlayer02");
    BIND_INV( __OBJGROUP_V2_2GLAIVESWORD02_ICE, "Inv_V2_IceSword01");

    BIND_INV( __OBJGROUP_V2_2SWORD01      , "Inv_V2_Sword01");
    BIND_INV( __OBJGROUP_V2_2SWORD02      , "Inv_V2_Sword02");
    BIND_INV( __OBJGROUP_V2_2SWORD03      , "Inv_V2_Sword03");
    BIND_INV( __OBJGROUP_V2_2SWORD04      , "Inv_V2_Sword04");
    BIND_INV( __OBJGROUP_V2_2SWORD05      , "Inv_V2_Sword05");
    BIND_INV( __OBJGROUP_V2_2SWORD06      , "Inv_V2_Sword06");
    BIND_INV( __OBJGROUP_V2_2SWORD07      , "Inv_V2_Sword07");
    BIND_INV( __OBJGROUP_V2_2SWORD08      , "Inv_V2_Sword08");

    BIND_INV( __OBJGROUP_V2_HACHE01       , "Inv_V2_Hache01");
    BIND_INV( __OBJGROUP_V2_HACHE02       , "Inv_V2_Hache02");
    BIND_INV( __OBJGROUP_V2_HACHE03       , "Inv_V2_Hache03");
    BIND_INV( __OBJGROUP_V2_HACHE04       , "Inv_V2_Hache04");
    BIND_INV( __OBJGROUP_V2_HACHE05       , "Inv_V2_Hache05");
    BIND_INV( __OBJGROUP_V2_HACHE06       , "Inv_V2_Hache06");
    BIND_INV( __OBJGROUP_V2_HACHE07       , "Inv_V2_Hache07");
    BIND_INV( __OBJGROUP_V2_HACHE08       , "Inv_V2_Hache08");
    BIND_INV( __OBJGROUP_V2_HACHE09       , "Inv_V2_Hache09");
    BIND_INV( __OBJGROUP_V2_HACHE10       , "Inv_V2_Hache10");
    BIND_INV( __OBJGROUP_V2_HACHE11       , "Inv_V2_Hache11");

    BIND_INV( __OBJGROUP_V2_2HACHE01      , "Inv_V2_Hache08");
    BIND_INV( __OBJGROUP_V2_2HACHE02      , "Inv_V2_Hache02");
    BIND_INV( __OBJGROUP_V2_2HACHE03      , "Inv_V2_Hache04");
    BIND_INV( __OBJGROUP_V2_2HACHE04      , "Inv_V2_Hache07");
    BIND_INV( __OBJGROUP_V2_2HACHE05      , "Inv_V2_Hache06");
    BIND_INV( __OBJGROUP_V2_2HACHE06      , "Inv_V2_Hache09");

    BIND_INV( __OBJGROUP_V2_2DAGGER01  , "Inv_V2_dague01");
    BIND_INV( __OBJGROUP_V2_2DAGGER02  , "Inv_V2_dague02");
    BIND_INV( __OBJGROUP_V2_2DAGGER03  , "Inv_V2_dague03");
    BIND_INV( __OBJGROUP_V2_2DAGGER04  , "Inv_V2_dague04");
    BIND_INV( __OBJGROUP_V2_2DAGGER05  , "Inv_V2_dague05");
    BIND_INV( __OBJGROUP_V2_2DAGGER06  , "Inv_V2_Viperine");
	
	 BIND_INV( __OBJGROUP_V2_BOW01    ,   "Inv_V2_Bow01" );
    BIND_INV( __OBJGROUP_V2_BOW02    ,   "Inv_V2_Bow02" );
    BIND_INV( __OBJGROUP_V2_BOW03    ,   "Inv_V2_Bow03" );
    BIND_INV( __OBJGROUP_V2_BOW04    ,   "Inv_V2_Bow04" );
    BIND_INV( __OBJGROUP_V2_BOW05    ,   "Inv_V2_Bow05" );
    BIND_INV( __OBJGROUP_V2_BOW05_ICE,   "Inv_V2_IceBow01");
    BIND_INV( __OBJGROUP_V2_BOW06    ,   "Inv_V2_Bow06" );
    BIND_INV( __OBJGROUP_V2_BOW07    ,   "Inv_V2_Bow07" );
    BIND_INV( __OBJGROUP_V2_BOW08    ,   "Inv_V2_Bow08" );
    BIND_INV( __OBJGROUP_V2_BOW09    ,   "Inv_V2_Bow09" );
    BIND_INV( __OBJGROUP_V2_BOW10    ,   "Inv_V2_Bow10" );

    BIND_INV( __OBJGROUP_V2_SCEPTRE01    ,   "Inv_V2_Sceptre01" );
    BIND_INV( __OBJGROUP_V2_SCEPTRE02    ,   "Inv_V2_Sceptre02" );
    BIND_INV( __OBJGROUP_V2_SCEPTRE03    ,   "Inv_V2_Sceptre03" );

    BIND_INV( __OBJGROUP_V2_SP01      , "Inv_V2_Sp01");
    BIND_INV( __OBJGROUP_V2_SP02      , "Inv_V2_Sp02");
    BIND_INV( __OBJGROUP_V2_SP03      , "Inv_V2_Sp03");
    BIND_INV( __OBJGROUP_V2_SP04      , "Inv_V2_Sp04");
    BIND_INV( __OBJGROUP_V2_SP05      , "Inv_V2_Sp05");
    BIND_INV( __OBJGROUP_V2_SP06      , "Inv_V2_Sp06");
    BIND_INV( __OBJGROUP_V2_SP07      , "Inv_V2_Sp07");

    BIND_INV( __OBJGROUP_V2_2SP01      , "Inv_V2_Sp01");
    BIND_INV( __OBJGROUP_V2_2SP02      , "Inv_V2_Sp02");
    BIND_INV( __OBJGROUP_V2_2SP03      , "Inv_V2_Sp04");
    BIND_INV( __OBJGROUP_V2_2SP04      , "Inv_V2_Sp05");
    BIND_INV( __OBJGROUP_V2_2SP05      , "Inv_V2_Sp06");
    BIND_INV( __OBJGROUP_V2_2SP06      , "Inv_V2_Sp07");

    BIND_INV( __OBJGROUP_V2_HAMMER01      , "Inv_V2_Hammer01");
    BIND_INV( __OBJGROUP_V2_HAMMER02      , "Inv_V2_Hammer02");
    BIND_INV( __OBJGROUP_V2_HAMMER03      , "Inv_V2_Hammer03");
    BIND_INV( __OBJGROUP_V2_HAMMER04      , "Inv_V2_Hammer04");
    BIND_INV( __OBJGROUP_V2_HAMMER05      , "Inv_V2_Hammer05");
    BIND_INV( __OBJGROUP_V2_HAMMER06      , "Inv_V2_Hammer06");
    BIND_INV( __OBJGROUP_V2_HAMMER07      , "Inv_V2_Hammer07");
    BIND_INV( __OBJGROUP_V2_2HAMMER01     , "Inv_V2_Hammer04");
    BIND_INV( __OBJGROUP_V2_2HAMMER02     , "Inv_V2_Hammer05");

    BIND_INV( __OBJGROUP_V2_SHIELD01,   "Inv_V2_Shield01" );
    BIND_INV( __OBJGROUP_V2_SHIELD02,   "Inv_V2_Shield02" );
    BIND_INV( __OBJGROUP_V2_ICESHIELD,   "Inv_V2_iceShield01" );

    BIND_INV( __OBJGROUP_MAN_ARMOR01_BODY_GR,"Inv_ManArmor01Body" );
    BIND_INV_PAL( __OBJGROUP_MAN_ARMOR01_BODY_BL,"Inv_ManArmor01Body" ,2);
    BIND_INV( __OBJGROUP_MAN_ARMOR01_LEGS_GR,"Inv_ManArmor01Legs" );
    BIND_INV_PAL( __OBJGROUP_MAN_ARMOR01_LEGS_BL,"Inv_ManArmor01Legs" ,2);
    BIND_INV( __OBJGROUP_MAN_ARMOR01_HAND_GR,"Inv_ManArmor01Glove");
    BIND_INV_PAL( __OBJGROUP_MAN_ARMOR01_HAND_BL,"Inv_ManArmor01Glove",2);
    BIND_INV( __OBJGROUP_MAN_ARMOR01_FOOT_GR,"Inv_ManArmor01Boots");
    BIND_INV_PAL( __OBJGROUP_MAN_ARMOR01_FOOT_BL,"Inv_ManArmor01Boots",2);

    BIND_INV( __OBJGROUP_T4CP_SHIELD,   "Inv_T4CP_Shield" );
    BIND_INV_PAL( __OBJGROUP_WHITELEATHER_BOOT, "64kInvBlackLeatherBoots",2);

    

    KEEPALIVE_NM;
    

	//New added by Dialsoft - END
    
    SkillIcons.BindSprite( "64kIconStunBlow", 1 );
    SkillIcons.BindSprite( "64kIconPowerBlow", 2 );
    //#define __SKILL_CRITICAL_STRIKE				3		// done
    SkillIcons.BindSprite( "64kIconFirstAid", 4 );
    SkillIcons.BindSprite( "64kIconParry",    8 );
    SkillIcons.BindSprite( "64kIconMeditate", 9 );
    SkillIcons.BindSprite( "64kIconShield", 11 ); // Dodge
    SkillIcons.BindSprite( "64kIconSword", 12 );  // Attack	
    SkillIcons.BindSprite( "64kIconHide", 14 );
    SkillIcons.BindSprite( "64kIconRob", 15 );
    SkillIcons.BindSprite( "64kIconSneak", 16 );
    SkillIcons.BindSprite( "64kIconSearch", 17 );
    SkillIcons.BindSprite( "64kIconPicklock", 26 );
    SkillIcons.BindSprite( "64kIconArmorPierce", 27 );
    SkillIcons.BindSprite( "64kIconPeek", 28 );
    SkillIcons.BindSprite( "64kIconRapidHealing", 29 );
    SkillIcons.BindSprite( "64kIconBow", 35 );
    SkillIcons.BindSprite( "64kIconDualSword", 36 );

    SpellIcons.BindSprite( "64kSpellIconAirAttackArea", 1 );
    SpellIcons.BindSprite( "64kSpellIconAirAttackSingle", 2 );
    SpellIcons.BindSprite( "64kSpellIconAirBoost", 3 );
    SpellIcons.BindSprite( "64kSpellIconAirDefense", 4 );
    SpellIcons.BindSprite( "64kSpellIconDarkAttack Area", 5 );
    SpellIcons.BindSprite( "64kSpellIconDarkAttackSingle", 6 );
    SpellIcons.BindSprite( "64kSpellIconDarkDefense", 7 );
    SpellIcons.BindSprite( "64kSpellIconDarkBoost", 8 );
    SpellIcons.BindSprite( "64kSpellIconDarkDrainArea", 9 );
    SpellIcons.BindSprite( "64kSpellIconDarkDrainSingle", 10 );
    SpellIcons.BindSprite( "64kSpellIconEarthAttackArea", 11 );
    SpellIcons.BindSprite( "64kSpellIconEarthAttackSingle", 12 );
    SpellIcons.BindSprite( "64kSpellIconEarthBoost", 13 );
    SpellIcons.BindSprite( "64kSpellIconEarthDefense", 14 );
    SpellIcons.BindSprite( "64kSpellIconFireAttackArea", 15 );
    SpellIcons.BindSprite( "64kSpellIconFireAttackSingle", 16 );
    SpellIcons.BindSprite( "64kSpellIconFireBoost", 17 );
    SpellIcons.BindSprite( "64kSpellIconFireDefense", 18 );
    SpellIcons.BindSprite( "64kSpellIconLightAttackArea", 19 );
    SpellIcons.BindSprite( "64kSpellIconLightAttackSingle", 20 );
    SpellIcons.BindSprite( "64kSpellIconLightBoost", 21 );
    SpellIcons.BindSprite( "64kSpellIconLightDefense", 22 );
    SpellIcons.BindSprite( "64kSpellIconLightHealArea", 23 );
    SpellIcons.BindSprite( "64kSpellIconLightHealSingle", 24 );
    SpellIcons.BindSprite( "64kSpellIconNoneAttackArea", 25 );
    SpellIcons.BindSprite( "64kSpellIconNoneAttackSingle", 26 );
    SpellIcons.BindSprite( "64kSpellIconNoneBoost", 27 );
    SpellIcons.BindSprite( "64kSpellIconNoneDefense", 28 );
    SpellIcons.BindSprite( "64kSpellIconWaterAttackArea", 29 );
    SpellIcons.BindSprite( "64kSpellIconWaterAttackSingle", 30 );
    SpellIcons.BindSprite( "64kSpellIconWaterBoost", 31 );
    SpellIcons.BindSprite( "64kSpellIconWaterDefense", 32 );
    SpellIcons.BindSprite( "64kIconArmor", 33 );
    SpellIcons.BindSprite( "64kIconAxe", 34 );
    SpellIcons.BindSprite( "64kIconBelt", 35 );
    SpellIcons.BindSprite( "64kIconBoots", 36 );
    SpellIcons.BindSprite( "64kIconBow", 37 );
    SpellIcons.BindSprite( "64kIconDagger", 38 );
    SpellIcons.BindSprite( "64kIconFlail", 39 );
    SpellIcons.BindSprite( "64kIconWarHammer", 40 );
    SpellIcons.BindSprite( "64kIconGloves", 41 );
    SpellIcons.BindSprite( "64kIconHelmet", 42 );
    SpellIcons.BindSprite( "64kIconNecklace", 43 );
    SpellIcons.BindSprite( "64kIconPotion", 44 );
    SpellIcons.BindSprite( "64kIconPouch", 45 );
    SpellIcons.BindSprite( "64kIconQuiver", 46 );
    SpellIcons.BindSprite( "64kIconRing", 47 );
    SpellIcons.BindSprite( "64kIconScroll", 48 );
    SpellIcons.BindSprite( "64kIconShield", 49 );
    SpellIcons.BindSprite( "64kIconStaff", 50 );
    SpellIcons.BindSprite( "64kIconSword", 51 );
    SpellIcons.BindSprite( "64kIconArmorLegging", 52 );
    SpellIcons.BindSprite( "64kIconCape", 53 );
    SpellIcons.BindSprite( "64kIconTorch", 54 );
    SpellIcons.BindSprite( "64kSpellIconAirMain", 55 );
    SpellIcons.BindSprite( "64kSpellIconDarkMain", 56 );
    SpellIcons.BindSprite( "64kSpellIconEarthMain", 57 );
    SpellIcons.BindSprite( "64kSpellIconFireMain", 58 );
    SpellIcons.BindSprite( "64kSpellIconLightMain", 59 );
    SpellIcons.BindSprite( "64kSpellIconNoneMain", 60 );
    SpellIcons.BindSprite( "64kSpellIconWaterMain", 61 );
    
#define SCLOTH "Cloth"
#define SCHAIN "Chainmail"
#define SSCALE "Scalemail"
#define SPLATE "PLateMail"
#define SSWORD "Swords"
#define SAXE   "Axes"
#define SCHAINWEAPON "Chain weapons"
#define SWOODWEAPON "Wooden Weapons"
#define SPOTION "Potions"
#define SDEF    "Generic Pickup Item"
#define SGOLD   "Gold sound"    
    
    // Set all the icons.
    ItemDragSounds.BindSound( SSWORD, __OBJGROUP_SHORT_SWORD );
    ItemDragSounds.BindSound( SSWORD, __OBJGROUP_LONG_SWORD );
    ItemDragSounds.BindSound( SCHAINWEAPON, __OBJGROUP_FLAIL );
    ItemDragSounds.BindSound( SCHAINWEAPON, __OBJGROUP_MORNING_STAR );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_WARHAMMER );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_SPEAR );
    ItemDragSounds.BindSound( SAXE,   __OBJGROUP_AXE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_LEATHER_ARMOR );
    //ItemDragSounds.BindSound( __OBJGROUP_WELL_TALK );
    //ItemDragSounds.BindSound( __OBJGROUP_CHEST_TALK );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_LEATHER_HELM );
    //ItemDragSounds.BindSound( __OBJGROUP_MAGE_SPELLBOOK );
    //ItemDragSounds.BindSound( __OBJGROUP_PRIEST_ANKH );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SCROLL );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_DOOR_CLOSED );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_DOOR_OPENED );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_CHAIR );
    //ItemDragSounds.BindSound( __OBJGROUP_CHEST_I );
    //ItemDragSounds.BindSound( __OBJGROUP_CHEST_OPEN_I );
    //ItemDragSounds.BindSound( __OBJGROUP_CHEST2_I );
    //ItemDragSounds.BindSound( __OBJGROUP_LONGUE_VUE );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_TORCH );
    //ItemDragSounds.BindSound( __OBJGROUP_IRON_KEY );
    //ItemDragSounds.BindSound( __OBJGROUP_BLUE_KEY );
    //ItemDragSounds.BindSound( __OBJGROUP_WEIRD_KEY );
    //ItemDragSounds.BindSound( __OBJGROUP_GOLD_KEY );
    //ItemDragSounds.BindSound( __OBJGROUP_KEY_HOLD );
    //ItemDragSounds.BindSound( __OBJGROUP_EMPTY_MUG );
    //ItemDragSounds.BindSound( __OBJGROUP_BEER_MUG );
    //ItemDragSounds.BindSound( __OBJGROUP_BEER_MUG_TOO_FULL );
    //ItemDragSounds.BindSound( __OBJGROUP_EMPTY_GLASS );
    //ItemDragSounds.BindSound( __OBJGROUP_BEVERAGE );
    //ItemDragSounds.BindSound( __OBJGROUP_CUP_FULL );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_CUP );
    //ItemDragSounds.BindSound( __OBJGROUP_CHEST );
    //ItemDragSounds.BindSound( __OBJGROUP_CHEST_OPEN );
    //ItemDragSounds.BindSound( __OBJGROUP_BASKET_CLOSE );
    //ItemDragSounds.BindSound( __OBJGROUP_BASKET_OPEN );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_RED_BOOK );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_GREY_BOOK );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_BROWN_BOOK );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_BACKPACK );
    //ItemDragSounds.BindSound( __OBJGROUP_BANDAGES_PACK );
    //ItemDragSounds.BindSound( __OBJGROUP_BANDAGE_1 );
    //ItemDragSounds.BindSound( __OBJGROUP_BANDAGE_2 );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_BOWL );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_VASE );
    ItemDragSounds.BindSound( SGOLD, __OBJGROUP_GOLD_BAR );
    ItemDragSounds.BindSound( SGOLD, __OBJGROUP_GOLD_BAR_PACK );
    //ItemDragSounds.BindSound( __OBJGROUP_BRONZE_BAR );
    //ItemDragSounds.BindSound( __OBJGROUP_BRONZE_BAR_PACK );
    //ItemDragSounds.BindSound( __OBJGROUP_IRON_BAR );
    //ItemDragSounds.BindSound( __OBJGROUP_IRON_BAR_PACK );
    //ItemDragSounds.BindSound( __OBJGROUP_BROOM_ON_WALL );
    //ItemDragSounds.BindSound( __OBJGROUP_PIOCHE );
    //ItemDragSounds.BindSound( __OBJGROUP_BROOM_ON_FLOOR );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_HAMMER );
    //ItemDragSounds.BindSound( __OBJGROUP_PINCES );
    //ItemDragSounds.BindSound( __OBJGROUP_BALANCE );
    //ItemDragSounds.BindSound( __OBJGROUP_HOURGLASS );
    //ItemDragSounds.BindSound( __OBJGROUP_EPROUVETTE_6_PACKS );
    //ItemDragSounds.BindSound( __OBJGROUP_EPROUVETTE_3_PACKS );
    //ItemDragSounds.BindSound( __OBJGROUP_EPROUVETTE_ALONE );
    //ItemDragSounds.BindSound( __OBJGROUP_BUCHE_DE_BOIS );
    //ItemDragSounds.BindSound( __OBJGROUP_WOOD_PACK_CLASSED );
    //ItemDragSounds.BindSound( __OBJGROUP_WOOD_PACK_MIXED );
    //ItemDragSounds.BindSound( __OBJGROUP_JAMBON );
    //ItemDragSounds.BindSound( __OBJGROUP_LONG_SAUSAGE );
    //ItemDragSounds.BindSound( __OBJGROUP_SAUSAGE_PACK );
    //ItemDragSounds.BindSound( __OBJGROUP_FAT_JAMBON );
    //ItemDragSounds.BindSound( __OBJGROUP_CHIKEN );
    //ItemDragSounds.BindSound( __OBJGROUP_BREAD );
    //ItemDragSounds.BindSound( __OBJGROUP_PACK_OF_PASTRIES );
    //ItemDragSounds.BindSound( __OBJGROUP_WATERBASKET );
    //ItemDragSounds.BindSound( __OBJGROUP_FISHING_POLE );
    //ItemDragSounds.BindSound( __OBJGROUP_DEAD_FISHES );
    //ItemDragSounds.BindSound( __OBJGROUP_EMPTY_BASKET );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_ARROW );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_ARROW_PACK );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_BOW );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_CROSSBOW );
    //ItemDragSounds.BindSound( __OBJGROUP_BANJO );
    //ItemDragSounds.BindSound( __OBJGROUP_TAMBOUR );
    //ItemDragSounds.BindSound( __OBJGROUP_MAP );
    //ItemDragSounds.BindSound( __OBJGROUP_SCIE );
    //ItemDragSounds.BindSound( __OBJGROUP_LONG_WHOOL_BALL );
    //ItemDragSounds.BindSound( __OBJGROUP_WHOOL_BALL );
    //ItemDragSounds.BindSound( __OBJGROUP_GREEN_WHOOL_BALL );
    //ItemDragSounds.BindSound( __OBJGROUP_BLUE_WHOOL_BALL );
    //ItemDragSounds.BindSound( __OBJGROUP_SHOVEL );
    KEEPALIVE_NM;
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_BLUE_POTION );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_YELLOW_POTION );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_BLACK_POTION );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_EMPTY_POTION );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_PURPLE_POTION );
    //ItemDragSounds.BindSound( __OBJGROUP_BASKET_FULL_WATER );
    //ItemDragSounds.BindSound( __OBJGROUP_BASKET_FULL_FRUIT );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_ROUND_CHAIR );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_CHAIR_2 );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_ROUND_CHAIR_2 );
    //ItemDragSounds.BindSound( __OBJGROUP_CRATES );
    //ItemDragSounds.BindSound( __OBJGROUP_RECTANGLE_CRATES );
    //ItemDragSounds.BindSound( __OBJGROUP_CUBIQ_CRATES );
    //ItemDragSounds.BindSound( __OBJGROUP_KITCHEN_BOWL );
    //ItemDragSounds.BindSound( __OBJGROUP_KITCHEN_SPOON );
    //ItemDragSounds.BindSound( __OBJGROUP_KITCHEN_FORK );
    //ItemDragSounds.BindSound( __OBJGROUP_KITCHEN_KNIFE );
    //ItemDragSounds.BindSound( __OBJGROUP_ROULEAU_A_PATRE );
    //ItemDragSounds.BindSound( __OBJGROUP_CHANDELLE );
    //ItemDragSounds.BindSound( __OBJGROUP_CHANDELLE_SUR_PIED );
    //ItemDragSounds.BindSound( __OBJGROUP_MIRROR );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_STAFF1 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_MACE );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_SPIKE_MACE );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_MAUL );
    ItemDragSounds.BindSound( SAXE, __OBJGROUP_DOUBLE_AXE );
    ItemDragSounds.BindSound( SAXE, __OBJGROUP_SINGLE_AXE );
    ItemDragSounds.BindSound( SSWORD, __OBJGROUP_BASTARD_SWORD );
    ItemDragSounds.BindSound( SAXE, __OBJGROUP_SPEAR_AXE );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SCROLL_OUVERT );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SCROLL_BLUE );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_LIVRE_OUVERT );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_LIVRE_OUVERT_EPAIS );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_DOOR_CLOSED_I );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_DOOR_OPENED_I );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_CHAIR_I );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_CHAIR_2_I );
    //ItemDragSounds.BindSound( __OBJGROUP_WOODEN_ROUND_CHAIR_2_I );
    //ItemDragSounds.BindSound( __OBJGROUP_2_WOODEN_CHAIR );
    //ItemDragSounds.BindSound( __OBJGROUP_2_WOODEN_CHAIR_2 );
    //ItemDragSounds.BindSound( __OBJGROUP_2_WOODEN_ROUND_CHAIR_2 );
    //ItemDragSounds.BindSound( __OBJGROUP_2_WOODEN_CHAIR_I );
    //ItemDragSounds.BindSound( __OBJGROUP_2_WOODEN_CHAIR_2_I );
    //ItemDragSounds.BindSound( __OBJGROUP_2_WOODEN_ROUND_CHAIR_2_I );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_POUCH );
    //ItemDragSounds.BindSound( __OBJGROUP_ROPE );
    //ItemDragSounds.BindSound( __OBJGROUP_STUFFED_BASKET_1 );
    //ItemDragSounds.BindSound( __OBJGROUP_STUFFED_BASKET_2 );
    //ItemDragSounds.BindSound( __OBJGROUP_STUFFED_BASKET_3 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_1 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_2 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_3 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_4 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_5 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_6 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_7 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_8 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_9 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_10 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_11 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_12 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_13 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FRUIT_14 );
    ItemDragSounds.BindSound( SGOLD, __OBJGROUP_COINS );
    ItemDragSounds.BindSound( SGOLD, __OBJGROUP_COINS_PILE );
    ItemDragSounds.BindSound( SGOLD, __OBJGROUP_COINS_BIG_PILE );
    //ItemDragSounds.BindSound( __OBJGROUP_BATWINGS                   = 163;
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_GEMS_PURPLE );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_GEMS_YELLOW );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_GEMS_BLUE );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_GEMS_GREEN );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_GEMS_PACK_1 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_GEMS_PACK_2 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_GEMS_PACK_3 );
    //ItemDragSounds.BindSound( __OBJGROUP_KOBOLDHAIR                 = 171;      
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_NECKLACE_1 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_NECKLACE_2 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_NECKLACE_3 );
    //ItemDragSounds.BindSound( __OBJGROUP_ORCFEET                    = 175;
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_RINGS_1 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_RINGS_2 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_RINGS_3 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_RINGS_4 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_RINGS_5 );
    //ItemDragSounds.BindSound( __OBJGROUP_SKEL_BONE );
    //ItemDragSounds.BindSound( __OBJGROUP_SPIDER_EYES );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_PADDED_GLOVE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_PADDED_HELM );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_PADDED_ARMOR );
    //ItemDragSounds.BindSound( __OBJGROUP_PADDED_SLEEVES );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_PADDED_LEGGINGS );
    ItemDragSounds.BindSound( SSCALE, __OBJGROUP_SCALE_ARMOR );
    //ItemDragSounds.BindSound( __OBJGROUP_LEVIER );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FIRECAMP ); //BLBLBL : 25 mars 200ç ajouté un son au feu de camp
    //ItemDragSounds.BindSound( __OBJGROUP_CAMPBURNT );
    ItemDragSounds.BindSound( SSWORD, __OBJGROUP_GLINTING_SWORD );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_DESTINY_GEM );
    //ItemDragSounds.BindSound( __OBJGROUP_VOLCANO_ROCK );
    ItemDragSounds.BindSound( SSCALE, __OBJGROUP_RING_HELM );
    ItemDragSounds.BindSound( SSCALE, __OBJGROUP_RING_ARMOR );
    //ItemDragSounds.BindSound( __OBJGROUP_RING_ARMOR_SLEEVES );
    ItemDragSounds.BindSound( SSCALE, __OBJGROUP_RING_LEGGINGS );
    //ItemDragSounds.BindSound( __OBJGROUP_FEATHER );
    //ItemDragSounds.BindSound( IUNK, __OBJGROUP_ICESHARD );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_LIFEGEM );
    //ItemDragSounds.BindSound( __OBJGROUP_OAKTREELEAF );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_PADDED_BOOTS );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_TORCHE );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_TORCHE_I );
    
    //ItemDragSounds.BindSound( SPOTION, __OBJANIM_FONTAIN_02);

    ItemDragSounds.BindSound( SDEF, __OBJGROUP_BELT );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_BLUEFLASK );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_BRACELET );
    //ItemDragSounds.BindSound( __OBJGROUP_CHEST2 );
    //ItemDragSounds.BindSound( __OBJGROUP_PINKLEAF );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_POT_GREEN );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_POT_RED );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SHIELD );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_STONELIFE );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_TORCH2 );
    //ItemDragSounds.BindSound( __OBJGROUP_TOOTH );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_GODPOT_BLUE );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_GODPOT_GREEN );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_GODPOT_RED );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_GODPOT_YELLOW );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_LARGEPOT_GREEN );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_LARGEPOT_RED );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_POT_BLUE );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_POT_ORANGE );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_POT_TURQUOISE );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_POT_VIOLET );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_POT_YELLOW );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_GREENFLASK );
    ItemDragSounds.BindSound( SPOTION, __OBJGROUP_REDFLASK );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_LEATHER_GLOVE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_LEATHER_BOOTS );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_LEATHER_PANTS );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_GLOVE );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_W_SLEEVE );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_BOOT	);
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_LEGS );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_HELM );
    ItemDragSounds.BindSound( SCHAIN, __OBJGROUP_CHAIN_LEGS );
    ItemDragSounds.BindSound( SCHAIN, __OBJGROUP_CHAIN_BODY );
    ItemDragSounds.BindSound( SCHAIN, __OBJGROUP_CHAIN_COIF );
    ItemDragSounds.BindSound( SSWORD, __OBJGROUP_DARK_SWORD );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_ROMAN_SHIELD );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_BAROSSA_SHIELD );
    ItemDragSounds.BindSound( SSWORD, __OBJGROUP_DAGGER );
    ItemDragSounds.BindSound( SSWORD, __OBJGROUP_REAL_DARKSWORD );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_HORNED_HELMET );
    ItemDragSounds.BindSound( SSWORD, __OBJGROUP_BATTLE_SWORD );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_NECROMANROBE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_REDROBE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_MAGEROBE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_WHITEROBE );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_GOLDENCROWN );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_GOLDEN_STAR );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_ELVEN_HAT );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_ORC_SHIELD );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_STUDDEDARMOR );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_LEG_CLOTH1 );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_BODY_CLOTH1 );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_STUDDEDLEG );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_REDCAPE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_BLACKLEATHER_BOOT );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_WHITELEATHER_BOOT );
    //ItemDragSounds.BindSound( __OBJGROUP_HEAD );
    //ItemDragSounds.BindSound( __OBJGROUP_SUNDIAL_TALK );
    //ItemDragSounds.BindSound( __OBJGROUP_SIGN1_TALK );
    //ItemDragSounds.BindSound( __OBJGROUP_SIGN2_TALK );
    //ItemDragSounds.BindSound( __OBJGROUP_SIGN3_TALK );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_STAFF2 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_STAFF3 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_STAFF4 );
    //ItemDragSounds.BindSound( __OBJGROUP_PORTAL );
    //ItemDragSounds.BindSound( __OBJGROUP_VAULT_TALK );
    //ItemDragSounds.BindSound( __OBJGROUP_FOUGERE );
    //ItemDragSounds.BindSound( __OBJGROUP_KRAANIAN_EGG );
    //ItemDragSounds.BindSound( __OBJGROUP_WOLF_PELT                  = 326;
    //ItemDragSounds.BindSound( __OBJGROUP_VAULT_TALK_I               = 327;
    //ItemDragSounds.BindSound( __OBJGROUP_HUMAN_FOOT                 = 328;
    //ItemDragSounds.BindSound( __OBJGROUP_TROLLBADGE                 = 329;
    //ItemDragSounds.BindSound( __OBJGROUP_SHOP_INN   		      		= 330;
    //ItemDragSounds.BindSound( __OBJGROUP_SHOP_POTION 	      		= 347;
    //ItemDragSounds.BindSound( __OBJGROUP_SHOP_PAWN   	      		= 360;
    //ItemDragSounds.BindSound( __OBJGROUP_SHOP_ARMOR 	         		= 373;
    //ItemDragSounds.BindSound( __OBJGROUP_SHOP_WEAPON 	      		= 390;
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_OGRECLUB );
    
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_BOW );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_LARGE_BOW );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_LARGE_BOW2 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_FANCY_SHORT_BOW );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_FANCY_LONG_BOW );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_BLUE_QUIVER );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_YELLOW_QUIVER );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_GREEN_QUIVER );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_RED_QUIVER );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_BLACK_QUIVER );    
    
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_CENTAUR_SHIELD1 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_CENTAUR_SHIELD2 );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_SHAMAN_HELM );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_SKAVEN_CLUB );
    ItemDragSounds.BindSound( SSWORD, __OBJGROUP_SKAVEN_KNIFE );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SKAVEN_SHIELD1 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SKAVEN_SHIELD2 );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SKAVEN_SHIELD3 );
    ItemDragSounds.BindSound( SAXE, __OBJGROUP_SKELETON_AXE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_SKELETON_HELM );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SKELETON_SHIELD );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_ORC_SHIELD2);
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SK_SHIELD);
    
    ItemDragSounds.BindSound( SSWORD, __OBJGROUP_SKELETON_SWORD );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_BLACK_WINGS );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_WHITE_WINGS );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_DECHU_WINGS );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_DECHU2_WINGS );
    
    ItemDragSounds.BindSound( SAXE, __OBJGROUP_HALBERD );
    
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_DARK_GEM );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_PURPLE_GEM );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_RED_GEM );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SMALL_PURPLE_GEM );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SMALL_RED_GEM );
	//New added by Dialsoft - BEGIN
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_BRACELET_RUBIS );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_BELT_JEWELS_BROWN );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_BELT_JEWELS_PURPLE );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_BELT_RED );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_BRACELET_AZURE );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_BRACELET_SQUARE_WHITE );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_GEM_AZURE );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_GEM_ORANGE );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_GEM_PINK );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_RING_3RUBIS );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_RING_RED );    
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_RING_STARGOLD );   
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_RING_CUT_GREEN );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_RING_CUT_RED );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_RUBIS_FOCUS );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_EMERALD_FOCUS );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_DIAMOND_FOCUS );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_SAPHIRE_FOCUS );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_EYE_OF_TIGER );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_MANA_STONE );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_BLUE_STONE );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_REDFEATHER );
	ItemDragSounds.BindSound( SCHAINWEAPON, __OBJGROUP_FIREFLAIL );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_SPIKEDLEATHER );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_DARKWINGS );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_NEUTRAL_WINGS );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_RED_WINGS );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_GOLD_WINGS );
   KEEPALIVE_NM;
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_DARKRED_WINGS );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_DARKGOLD_WINGS );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_LIGHTBLUE_WINGS );
	ItemDragSounds.BindSound( SDEF, __OBJGROUP_SERAPH_ORANGE_WINGS );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_BLUE );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_GREEN );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_DARKBLUE );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_VIOLET );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_PURPLE );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_YELLOW );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_GRAY );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_MAUVE );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_ORANGE );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_BRUN );
	ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_FWHITEROBE_DARK );
	//Added by Tiamat
	ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_GOLD );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_GLOVE_GOLD );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_GOLD );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_BOOT_GOLD	);
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_LEGS_GOLD );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_HELM_GOLD );
	ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_SILVER );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_GLOVE_SILVER );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_SILVER );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_BOOT_SILVER	);
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_LEGS_SILVER );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_HELM_SILVER );
	ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_BLACK );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_GLOVE_BLACK );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLACK );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_BOOT_BLACK	);
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_LEGS_BLACK );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_HELM_BLACK );
	ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_BLUE );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_GLOVE_BLUE );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLUE );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_BOOT_BLUE	);
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_LEGS_BLUE );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_HELM_BLUE );
	ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_GREEN );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_GLOVE_GREEN );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_GREEN );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_BOOT_GREEN	);
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_LEGS_GREEN );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_HELM_GREEN );
	ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_RED );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_GLOVE_RED );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_RED );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_BOOT_RED	);
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_LEGS_RED );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_HELM_RED );
	ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_PURPLE );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_GLOVE_PURPLE );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_PURPLE );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_BOOT_PURPLE	);
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_LEGS_PURPLE );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_HELM_PURPLE );
	ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_EMERALD );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_GLOVE_EMERALD );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_ARMOR_W_SLEEVE_EMERALD );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_BOOT_EMERALD	);
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_LEGS_EMERALD );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_PLATE_HELM_EMERALD );
    ItemDragSounds.BindSound( SCHAINWEAPON, __OBJGROUP_TIGERAXE );
    ItemDragSounds.BindSound( SCHAINWEAPON, __OBJGROUP_ANCIENTAXE );
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_FLOWERS );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_NECROSTAFF );
   	// ADDED BY LUCIFER
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_GREENCAPE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_YELLOWCAPE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_SILVERCAPE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_PURPLECAPE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_BLACKCAPE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_AQUACAPE );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_BLUECAPE );

    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_WITCH_HAT1 );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_WITCH_HAT2 );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_WITCH_HAT3 );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_WITCH_HAT4 );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_WITCH_HAT5 );
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_NOEL_HAT);
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_T4CP_CASQUE);
    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_T4CP_CASQUETTE);
    

    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_NEW_BOW01 );
	
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_DWARF_HAMMER);
    ItemDragSounds.BindSound( SAXE       , __OBJGROUP_AXE_DESTRUCTION);
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_SWORD_ANGEL);
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_CLAY2BLADES);
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_SWORD_HERO);
    

    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_BUTTERFLY_WING1);
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_BUTTERFLY_WING2);
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_BUTTERFLY_WING3);
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_BUTTERFLY_WING4);

    ItemDragSounds.BindSound( SCLOTH, __OBJGROUP_HAMM );

    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_ARCHWINGS_WHITE);
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_LICHROBE);
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_MASQUE_GOBLIN );
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_ARMORED_ROBE);

    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_V2_HAUME01 );
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_V2_HAUME02 );
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_V2_HAUME03 );
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_V2_HAUME04 );
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_V2_HAUME04_BL );
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_V2_HAT01   );
    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_V2_MASK01  );

    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_DAGGER_VIP);
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_DAGGER01  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_DAGGER02  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_DAGGER03  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_DAGGER04  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_DAGGER05  );

    //NM_TAG_ADD_SWORD
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_BUSSWORD01   );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_BUSSWORD02   );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_BUSSWORD03   );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_CLAYSWORD01  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_CLAYSWORD02  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_GLAIVESWORD01);
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_GLAIVESWORD02);
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SWORD01      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SWORD02      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SWORD03      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SWORD04      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SWORD05      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SWORD06      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SWORD07      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SWORD08      );

    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2BUSSWORD01   );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2BUSSWORD02   );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2BUSSWORD03   );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2CLAYSWORD01  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2CLAYSWORD02  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2GLAIVESWORD01);
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2GLAIVESWORD02);
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SWORD01      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SWORD02      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SWORD03      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SWORD04      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SWORD05      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SWORD06      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SWORD07      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SWORD08      );


    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE01 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE02 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE03 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE04 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE05 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE06 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE07 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE08 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE09 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE10 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_HACHE11 );

    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_2HACHE01 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_2HACHE02 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_2HACHE03 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_2HACHE04 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_2HACHE05 );
    ItemDragSounds.BindSound( SAXE     , __OBJGROUP_V2_2HACHE06 );

    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2DAGGER01  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2DAGGER02  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2DAGGER03  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2DAGGER04  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2DAGGER05  );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2DAGGER06  );
    KEEPALIVE_NM;
	 ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW01 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW02 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW03 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW04 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW05 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW06 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW07 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW08 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW09 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW10 );

    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_SCEPTRE01 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_SCEPTRE02 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_SCEPTRE03 );

    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SP01      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SP02      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SP03      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SP04      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SP05      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SP06      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_SP07      );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SP01     );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SP02     );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SP03     );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SP04     );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SP05     );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2SP06     );

    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_HAMMER01  );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_HAMMER02  );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_HAMMER03  );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_HAMMER04  );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_HAMMER05  );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_HAMMER06  );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_HAMMER07  );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_2HAMMER01 );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_2HAMMER02 );

    ItemDragSounds.BindSound( SDEF, __OBJGROUP_V2_SHIELD01);
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_V2_SHIELD02);
    ItemDragSounds.BindSound( SDEF, __OBJGROUP_V2_ICESHIELD);
    

    ItemDragSounds.BindSound( SCLOTH     , __OBJGROUP_V2_MASQUE_GOBLIN_ICE );
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_V2_BOW05_ICE );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_2GLAIVESWORD02_ICE );
    ItemDragSounds.BindSound( SSWORD     , __OBJGROUP_V2_GLAIVESWORD02_ICE );

    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_MAN_ARMOR01_BODY_GR );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_MAN_ARMOR01_BODY_BL );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_MAN_ARMOR01_LEGS_GR );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_MAN_ARMOR01_LEGS_BL );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_MAN_ARMOR01_HAND_GR );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_MAN_ARMOR01_HAND_BL );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_MAN_ARMOR01_FOOT_GR );
    ItemDragSounds.BindSound( SPLATE, __OBJGROUP_MAN_ARMOR01_FOOT_BL );

    ItemDragSounds.BindSound( SDEF, __OBJGROUP_T4CP_SHIELD);
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_T4CP_MATRAQUE);
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_T4CP_BAT);
    ItemDragSounds.BindSound( SWOODWEAPON, __OBJGROUP_T4CP_12);

    KEEPALIVE_NM;


	//New added by Dialsoft - END
    
    SoundFX[0].Create("Open Wooden Door", TS_MEMORY);
    SoundFX[1].Create("Close Wooden Door", TS_MEMORY);
    
    for (int i = 0; i < 30; i++)
        Object3DLoad[i] = false;
    
    MainObject.SpriteNumber = 1;
    MainObject.SpriteMove = 1;
    MainObject.Speed = 0;
    MainObject.AttFrame = 0;
    MainObject.Direction = 1;
    MainObject.Chose = 0;
    MainObject.DrawOverlap = TRUE;
    
    MainObject.nbSprite4Move = 1*FRAMING32; // 32 FPS @PoPo
    MainObject.nbSprite4Att = 1*FRAMING32; // 32 FPS
    
    //**NMNMNM --[Change visible Area range] Old (15,20)
    //SetRange(21, 29); //21,//29
    //SetRange(22, 30); //21,//29
    SetRange(26, 39); //21,//29
    
    
    //	LoadObject(0);
    //	LoadObject(1);
    //	LoadObject(2);
    //	LoadObject(3);
    //	LoadObject(4);
    //	LoadObject(5);
    //	LoadObject(6);
    //	LoadObject(7);
    //	LoadObject(8);
    //	LoadObject(9);
    //	LoadObject(10);
    //	LoadObject(11);
    //	LoadObject(12);
}

void VisualObjectList::SavePointer(void) {
    SavePtr = Current;
    SaveQuery = Querying;
}

void VisualObjectList::RestorePointer(void) {
    Current = SavePtr;
    Querying = SaveQuery;
}

T3VSBSound SoundEffects[10];
int soundNumber = 0;

void VisualObjectList::Add(unsigned long ID, unsigned short Type, unsigned short AbsX, unsigned short AbsY, signed char Light, unsigned char Friend, unsigned long AttachID, char pHp, char defaultDirection, bool allowOutOfBound, long spellEffectID, long spellChildID ) {
    AttachID = 0;
    
    if (Type >= 40000) {
        switch (Type) {
        case __SOUND_ICE_CLOUD:
            SoundFX[59].Create("Ice Cloud", TS_MEMORY);
            SoundEffects[soundNumber % 10].CopyWave(&SoundFX[59]);
            SoundEffects[soundNumber % 10].Play(FALSE);
            soundNumber++;
            break;
        }
    }
    
    // Add an object to the list.
    if (Type >= (__OBJGROUP_LASTGROUP+__OBJANIM_LASTGROUP) && Type <= 10000) 
       return;
    bool AlreadyHere = false;
    int a, b;
    int Height, Width;
    
    ToHead();
    
    while (QueryNext()) {
        if (GetObject()->ID == ID) {
            // Object is already in the list.
            AlreadyHere = true;
            SetQueryState(STOP_QUERY);
        }
    }
    
    if (AlreadyHere) 
    {
        LOG << "Object: " << Type << " rejected.\r\n";
        return;
    }
    
    if (!AlreadyHere) {
        LOG << "Creating Object: ID [" << ID << "] Type [" << Type << "]\r\n";
        
        // Object is not in the list.
        TFCObject *Object = new TFCObject;
        Object->OX    = (signed char)(AbsX - Player.xPos);
        Object->OY    = (signed char)(AbsY - Player.yPos);
        Object->OC    = 0;
        Object->allowOutOfBound = allowOutOfBound;
        a = b = 0;
        if (World.Done) {
            b = -World.MovY;
            a = -World.MovX;
        }
        World.GetHeight(Object->OX+ViewWidth, Object->OY+ViewHeight, &Width, &Height, &Object->OC);
        Object->OY    += Height;
        Object->OX -= a;
        Object->OY -= b;
        Object->AbsX = AbsX;
        Object->AbsY = AbsY;
        Object->bInvisible = FALSE;
        if ((Type >= 21000 && Type < 25000) || (Type >= 11000 && Type < 15000)) {
            Object->bInvisible = TRUE;
            Type -= 1000;
        }
        Object->Type  = Type;
        if (Type == __SPELL_SERAPH_MALE      || Type == __SPELL_SERAPH_FEMALE      || 
            Type == __SPELL_EVIL_SERAPH_MALE || Type == __SPELL_EVIL_SERAPH_FEMALE || 
            Type == __SPELL_DECHU_MALE       || Type == __SPELL_DECHU_FEMALE         ) 
        {
            Object->ID = SpellID;
            SpellID--;
            Object->NewID    = ID;
        } 
        else 
        {
            Object->ID = ID;
        }
        Object->spellEffectID = spellEffectID;
        Object->spellChildID = spellChildID;
        Object->SpriteNumber = 1;
        Object->SpriteMove = 1;
        Object->MovX = 0;
        Object->MovY = 0;
        Object->fMovX = 0;
        Object->fMovY = 0;
        Object->SpeedX = 0;
        Object->SpeedY = 0;
        Object->Speed = 0;
        Object->Direction = defaultDirection;
        Object->Movement = -1;
        Object->Chose = 0;
        Object->MaxyCor = 0;
        Object->MouseCursor = 0;
        Object->Faces = 0;
        Object->FacesStM = 0;
        Object->Anim = 0;
        Object->Extended = false;
        Object->Temp = timeGetTime();
        Object->IdentifyTemp = timeGetTime();
        Object->Slow = 2;
        Object->nbSprite4Move = 1*FRAMING32; // 32 FPS Spells @PoPo
        Object->nbSprite4Att = 1*FRAMING32; // 32 FPS Spells
        Object->Name = 0;
        Object->Up = 0;
        Object->Down = 0;
        Object->Left = 1;
        Object->Right = 1;
        Object->NoMove = FALSE;
        Object->TextCorrection = -20;
        Object->onAttack[0].CopyWave(&SoundFX[11]);
        Object->onAttack[1].CopyWave(&SoundFX[11]);
        Object->onAttack[2].CopyWave(&SoundFX[11]);
        Object->onAttacked[0].CopyWave(&SoundFX[11]);
        Object->onAttacked[1].CopyWave(&SoundFX[11]);
        Object->onAttacked[2].CopyWave(&SoundFX[11]);
        Object->onUse.CopyWave(&SoundFX[11]);
        Object->onKilled[0].CopyWave(&SoundFX[11]);
        Object->onKilled[1].CopyWave(&SoundFX[11]);
        Object->Killed = false;
        Object->Format = 0; 
        Object->Flush = 0;
        Object->DrawOverlap = TRUE;
        Object->Dynamic = true; // false
        Object->Standing = ST_STANDING;
        Object->Light = Light;
        Object->Type3D = -1;
        Object->KillType = 0;
        Object->KnownPuppet = FALSE;
        if ((Type > 15000 && Type < 20000) || (Type > 25000 && Type < 30000)) {
            Object->KillType = 1;
            Object->KillTimer = timeGetTime();
        }
        Object->pHp = pHp;
        Object->CurrentCorpseFrame = -1;
        Object->DeleteMe = FALSE;
        Object->SummonID = 0;
        Object->Parry = NULL;
        Object->AttachID = AttachID;
        Object->Count = 0;
        Object->pOX = &staticCharZero;
        Object->pOY = &staticCharZero;
        Object->pMovX = &staticShortZero;
        Object->pMovY = &staticShortZero;
        Object->pOverlap = NULL;
        
        Object->VisiblePart = 0;
        memset(Object->PuppetInfo, 0, 32);
        memset(Object->OldPuppetInfo, 255, 32);
        if (Friend == VOL_ISPLAYER) {
            Object->Friendly = VOL_PLAYER;
        } else if (Friend == VOL_CANTALK) {
            Object->Friendly = VOL_NPC;
        } else if (Friend == VOL_CANNOTTALK) {
            Object->Friendly = VOL_MONSTER;
        } else {
            Object->Friendly = VOL_OBJECT;
        }
        if (Type == __MONSTER_PIG)
            Object->Friendly = VOL_MONSTER;
        
        if (Object->Type == __OBJGROUP_WELL_TALK || Object->Type == __OBJGROUP_CHEST_TALK || Object->Type == __OBJGROUP_SUNDIAL_TALK ||
            Object->Type == __OBJGROUP_SIGN1_TALK || Object->Type == __OBJGROUP_SIGN2_TALK || Object->Type == __OBJGROUP_SIGN3_TALK ||
            Object->Type == __OBJGROUP_VAULT_TALK || Object->Type == __OBJGROUP_VAULT_TALK_I || Object->Type == __OBJGROUP_DOOR_TALK ||
            Object->Type == __OBJGROUP_DOOR_TALK_I || Object->Type == __OBJGROUP_COFFIN_TALK || Object->Type == __OBJGROUP_RIBCAGE)  {
            Object->Friendly = VOL_NPC;
        }
        
        if (Type < 10000) {
            
            unsigned short tType;
            switch (Type) {
            case __OBJGROUP_CHEST_I: tType = __OBJGROUP_CHEST; break;
            case __OBJGROUP_CHEST_OPEN_I: tType = __OBJGROUP_CHEST_OPEN; break;
            case __OBJGROUP_CHEST2_I: tType = __OBJGROUP_CHEST2; break;
            case __OBJGROUP_WOODEN_DOOR_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR_CLOSED; break;
            case __OBJGROUP_WOODEN_DOOR_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR_OPENED; break;
            case __OBJGROUP_WOODEN_CHAIR_I: tType = __OBJGROUP_WOODEN_CHAIR; break;
            case __OBJGROUP_WOODEN_CHAIR_2_I: tType = __OBJGROUP_WOODEN_CHAIR_2; break;
            case __OBJGROUP_WOODEN_ROUND_CHAIR_2_I: tType = __OBJGROUP_WOODEN_ROUND_CHAIR_2; break;
            case __OBJGROUP_2_WOODEN_CHAIR_I: tType = __OBJGROUP_2_WOODEN_CHAIR; break;
            case __OBJGROUP_2_WOODEN_CHAIR_2_I: tType = __OBJGROUP_2_WOODEN_CHAIR_2; break;
            case __OBJGROUP_2_WOODEN_ROUND_CHAIR_2_I: tType = __OBJGROUP_2_WOODEN_ROUND_CHAIR_2; break;
            case __OBJGROUP_VAULT_TALK_I: tType = __OBJGROUP_VAULT_TALK; break;
            case __OBJGROUP_VAULT_I: tType = __OBJGROUP_VAULT; break;
            case __OBJGROUP_DOOR_TALK: tType = __OBJGROUP_DOOR_TALK_I; break;
            case __OBJGROUP_SKAVEN_CORPSE_I1: tType = __OBJGROUP_SKAVEN_CORPSE1; break;
            case __OBJGROUP_SKAVEN_CORPSE_I2: tType = __OBJGROUP_SKAVEN_CORPSE2; break;
            case __OBJGROUP_SKAVEN_CORPSE_I3: tType = __OBJGROUP_SKAVEN_CORPSE3; break;
            case __OBJGROUP_SKAVEN_CORPSE_I4: tType = __OBJGROUP_SKAVEN_CORPSE4; break;
            case __OBJGROUP_WOODEN_DOOR2_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR2_CLOSED; break;
            case __OBJGROUP_WOODEN_DOOR2_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR2_OPENED; break;
            case __OBJGROUP_WOODEN_DOOR3_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR3_CLOSED; break;
            case __OBJGROUP_WOODEN_DOOR3_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR3_OPENED; break;
            case __OBJGROUP_WOODEN_DOOR4_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR4_CLOSED; break;
            case __OBJGROUP_WOODEN_DOOR4_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR4_OPENED; break;
            case __OBJGROUP_WOODEN_DOOR5_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR5_CLOSED; break;
            case __OBJGROUP_WOODEN_DOOR5_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR5_OPENED; break;
            case __OBJGROUP_WOODEN_DOOR6_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR6_CLOSED; break;
            case __OBJGROUP_WOODEN_DOOR6_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR6_OPENED; break;
            case __OBJGROUP_WOODEN_DOOR7_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR7_CLOSED; break;
            case __OBJGROUP_WOODEN_DOOR7_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR7_OPENED; break;
            case __OBJGROUP_WOODEN_DOOR8_CLOSED_I: tType = __OBJGROUP_WOODEN_DOOR8_CLOSED; break;
            case __OBJGROUP_WOODEN_DOOR8_OPENED_I: tType = __OBJGROUP_WOODEN_DOOR8_OPENED; break;
            default: tType = Type; break;
            }
            
			if (tType){//BLBLBL ajout d'un check sur le tType.

				Object->MouseCursor = DefaultMouseCursor[tType];
				Object->MaxWidth = GetVSFObject(tType)->GetWidth();
				Object->MaxHeight = GetVSFObject(tType)->GetHeight();
				Object->fTransparent = GetVSFObject(tType)->GetfTransparent();
				Object->TransColor = GetVSFObject(tType)->GetTransColor();
				Object->MaxxCor = GetVSFObject(tType)->GetCX();
				if (Object->MaxxCor > GetVSFObject(tType)->GetFCX()) Object->MaxxCor = GetVSFObject(tType)->GetFCX();
				Object->MaxyCor = GetVSFObject(tType)->GetCY();
				if (Object->MaxyCor > GetVSFObject(tType)->GetFCY()) Object->MaxyCor = GetVSFObject(tType)->GetFCY();
				//:CR_NMNMNM  Overlap Size
				Object->Overlap.Create(Object->MaxWidth, Object->MaxHeight, TRUE, lpDXDirectDraw->wGMask, Object->MaxxCor, Object->MaxyCor);
				Object->Overlap.Dither = g_SaveGame.GetOptionsParam()->bDitherGraph;
				Object->Up = 0;
				Object->Down = 1;
				Object->Left = 1;
				Object->Right = 1;
            
				a = b = 0;
				if (World.Done) {
					if (World.MovY == -1)
						b = 16;
					if (World.MovY == 1)
						b = -16;
					if (World.MovX == -1)
						a = 32;
					if (World.MovX == 1)
						a = -32;
				}
				if (!Object->OC) {
					INFO *Return = new INFO;
					Return->Width = GetVSFObject(tType)->GetWidth();
					Return->Height = GetVSFObject(tType)->GetHeight();
					Return->CX = GetVSFObject(tType)->GetCX();
					Return->CY = GetVSFObject(tType)->GetCY();
					ClearAnimID(Object->ID);
					/**/World.DrawOverlap(Object->OX+ViewWidth, Object->OY+ViewHeight+Object->OC, 0, 0, (Object->OX+ViewWidth)*32+Object->Overlap.xCor-48+a, (Object->OY+Object->OC+ViewHeight)*16+Object->Overlap.yCor+13+b, &Object->Overlap, 2, Return,Object->ID);
					Object->DrawOverlap = Return->DrawOverlap;
					delete Return;
				}
			}
        } 
        
        switch (Type) {
        case __OBJGROUP_WOODEN_CHAIR_I: 
        case __OBJGROUP_2_WOODEN_CHAIR_I: 
        case __OBJGROUP_2_WOODEN_CHAIR_2_I: 
        case __OBJGROUP_2_WOODEN_ROUND_CHAIR_2_I: 
        case __OBJGROUP_2_WOODEN_CHAIR: 
        case __OBJGROUP_2_WOODEN_CHAIR_2: 
        case __OBJGROUP_2_WOODEN_ROUND_CHAIR_2: 
            Object->Up = 3;
            Object->Down = 1;
            Object->Left	= 1;
            Object->Right = -1;
            break;
            
        case __OBJGROUP_WOODEN_CHAIR: 
            Object->Up = 4;
            Object->Down = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_WOODEN_ROUND_CHAIR:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 3;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_VAULT_TALK_I:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            Object->Down = 3;
            break;
        case __OBJGROUP_VAULT_I:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            Object->Down = 3;
            break;
            
        case __OBJGROUP_SUNDIAL_TALK:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 5;
            Object->Down = 1;
            Object->TextCorrection = -80;
            break;
            
        case __OBJGROUP_DOOR_TALK:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 5;
            Object->Down = 1;
            Object->TextCorrection = -90;
            break;
            
        case __OBJGROUP_DOOR_TALK_I:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 5;
            Object->Down = 1;
            Object->TextCorrection = -90;
            break;
            
        case __OBJGROUP_HUMAN_FOOT:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_HEART:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_WELL_TALK:
            Object->Right = 1;
            Object->Left = 2;
            Object->Up = 7;
            Object->Down = 2;
            Object->TextCorrection = -80;
            break;
            
        case __OBJGROUP_RIBCAGE:
            Object->Right = 0;
            Object->Left = 2;
            Object->Up = 3;
            Object->Down = 2;
            Object->TextCorrection = -40;
            break;
            
        case __OBJGROUP_COFFIN_TALK:
            Object->Right = 1;
            Object->Left = 2;
            Object->Up = 3;
            Object->Down = 3;
            Object->TextCorrection = -80;
            break;
            
        case __OBJGROUP_BASKET_FULL_FRUIT:
            Object->Right = -1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_WOODEN_CHAIR_2: 
        case __OBJGROUP_WOODEN_CHAIR_2_I: 
            Object->Up = 4;
            Object->Down = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_WOODEN_ROUND_CHAIR_2_I: 
        case __OBJGROUP_WOODEN_ROUND_CHAIR_2: 
            Object->Up = 5;
            Object->Down = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_MAGE_SPELLBOOK:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_STUDDEDLEG:
            Object->Left	= 1;
            Object->Up = 1;
            Object->Right = 0;
            break;
        // LUCIFER CHANGES START
        case __OBJGROUP_GREENCAPE:
        case __OBJGROUP_YELLOWCAPE:
        case __OBJGROUP_SILVERCAPE:
        case __OBJGROUP_PURPLECAPE:
        case __OBJGROUP_BLACKCAPE:
        case __OBJGROUP_AQUACAPE:
        case __OBJGROUP_BLUECAPE:
        case __OBJGROUP_REDCAPE:
            Object->Up = 2;
            Object->Down = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_BASKET_FULL_WATER:
            Object->Right = -1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_REDFLASK:
        case __OBJGROUP_GREENFLASK:
        case __OBJGROUP_BLUEFLASK:
        case __OBJGROUP_POT_YELLOW:
        case __OBJGROUP_POT_BLUE:
        case __OBJGROUP_POT_ORANGE:
        case __OBJGROUP_POT_TURQUOISE:
        case __OBJGROUP_POT_VIOLET:
        case __OBJGROUP_LARGEPOT_RED:
        case __OBJGROUP_LARGEPOT_GREEN:
            Object->Up = 1;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = -1;
            break;
            
        case __OBJGROUP_EMPTY_POTION:
        case __OBJGROUP_BLUE_POTION:
        case __OBJGROUP_BLACK_POTION:
        case __OBJGROUP_YELLOW_POTION:
        case __OBJGROUP_PURPLE_POTION:
            Object->Right = -1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_SHOVEL:
            Object->Right = 0;
            Object->Up = 2;
            break;
            
        case __OBJGROUP_WHOOL_BALL:
        case __OBJGROUP_GREEN_WHOOL_BALL:
        case __OBJGROUP_BLUE_WHOOL_BALL:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_LONG_WHOOL_BALL:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_SCIE:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_MAP:
            Object->Up = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_BANJO:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_TAMBOUR:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_CROSSBOW:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_BOW:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_ARROW_PACK:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_ARROW:
            Object->Left = 1;
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_EMPTY_BASKET:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_DEAD_FISHES:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_FISHING_POLE:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_WATERBASKET:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_PACK_OF_PASTRIES:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_BREAD:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_CHIKEN:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_FAT_JAMBON:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_SAUSAGE_PACK:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_LONG_SAUSAGE:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_JAMBON:
            Object->Right = -1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_WOOD_PACK_CLASSED:
            Object->Right = 0;
            Object->Up = 2;
            break;
            
        case __OBJGROUP_WOOD_PACK_MIXED:
            Object->Right = 0;
            Object->Up = 2;
            break;
            
        case __OBJGROUP_BUCHE_DE_BOIS:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_EPROUVETTE_ALONE:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_PINCES:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_GOLD_BAR:
        case __OBJGROUP_BRONZE_BAR:
        case __OBJGROUP_IRON_BAR:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_WOODEN_BOWL:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_EPROUVETTE_3_PACKS:
        case __OBJGROUP_EPROUVETTE_6_PACKS:
            Object->Right = 0;
            Object->Up = 1;
            Object->Down = 2;
            break;
            
        case __OBJGROUP_BALANCE:
            Object->Right = 0;
            Object->Up = 1;
            Object->Down = 2;
            break;
            
        case __OBJGROUP_HAMMER:
            Object->Right = 0;
            break;
            
        case __OBJGROUP_WOODEN_VASE:
            Object->Right = -1;
            Object->Down = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_HOURGLASS:
            Object->Right = -1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_BROOM_ON_FLOOR:
            Object->Right = 0;
            Object->Up = 3;
            break;
            
        case __OBJGROUP_PIOCHE:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_BROOM_ON_WALL:
            Object->Right = -1;
            Object->Up = 4;
            break;
            
        case __OBJGROUP_IRON_BAR_PACK:
        case __OBJGROUP_BRONZE_BAR_PACK:
        case __OBJGROUP_GOLD_BAR_PACK:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_MORNING_STAR:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_BANDAGE_2:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_BANDAGE_1:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_RED_BOOK:
        case __OBJGROUP_GREY_BOOK:
        case __OBJGROUP_BROWN_BOOK:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_GOLD_KEY:
        case __OBJGROUP_WEIRD_KEY:
        case __OBJGROUP_BLUE_KEY:
        case __OBJGROUP_IRON_KEY:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_KEY_HOLD:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_TORCH:
            Object->Up = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_TORCHE:
            Object->Anim = 21;
            //				Object->Right = 0;
            //				Object->Left = 2;
            //				Object->Up = 1;
            Object->Down = 0;
            break;

        case __OBJANIM_FONTAIN_02:
            Object->Anim = __OBJANIM_FONTAIN_02_CNT;
            Object->Down = 0;
            break;
        case __OBJANIM_MOULIND:
            Object->Anim = __OBJANIM_MOULIND_CNT;
            Object->Down = 0;
            break;
        case __OBJANIM_MOULING:
            Object->Anim = __OBJANIM_MOULING_CNT;
            Object->Down = 0;
            break;
        
		case __OBJGROUP_PORTAL:
            Object->Anim = 26;
            Object->Right = 0;
            Object->Left = 2;
            Object->Up = 8;
            Object->Down = 1;
            Object->TextCorrection = -120;
            break;
    	case __OBJGROUP_CAULDRON_CONTAINER:
            Object->Anim = 21;
            Object->Right = 0;
            Object->Left = 2;
            Object->Up = 8;
            Object->Down = 1;
            Object->TextCorrection = -120;
            break;
            
        case __OBJGROUP_SHOP_INN:
            Object->TextCorrection = -99;
            Object->Anim = 17;
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 6;
            Object->Down = -2;
            break;
            
        case __OBJGROUP_SHOP_POTION:
            Object->TextCorrection = -99;
            Object->Anim = 13;
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 7;
            Object->Down = -1;
            break;
            
        case __OBJGROUP_SHOP_PAWN:
            Object->TextCorrection = -51;
            Object->Anim = 13;
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 4;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_SHOP_ARMOR:
            Object->TextCorrection = -99;
            Object->Anim = 17;
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 6;
            Object->Down = -2;
            break;
            
        case __OBJGROUP_SHOP_WEAPON:
            Object->TextCorrection = -99;
            Object->Anim = 31;
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 6;
            Object->Down = -2;
            break;
            
        case __OBJGROUP_BANDAGES_PACK:
            Object->Right = 0;
            Object->Up = 1;
            break;
        case __OBJGROUP_SHOP_INN_I:
            Object->TextCorrection = -99;
            Object->Anim = 17;
            Object->Right = -1;
            Object->Left = 2;
            Object->Up = 6;
            Object->Down = -2;
            break;
            
        case __OBJGROUP_SHOP_POTION_I:
            Object->TextCorrection = -99;
            Object->Anim = 13;
            Object->Right = -1;
            Object->Left = 2;
            Object->Up = 6;
            Object->Down = -1;
            break;
            
        case __OBJGROUP_SHOP_PAWN_I:
            Object->TextCorrection = -51;
            Object->Anim = 13;
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 4;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_SHOP_ARMOR_I:
            Object->TextCorrection = -99;
            Object->Anim = 17;
            Object->Right = -1;
            Object->Left = 2;
            Object->Up = 6;
            Object->Down = -2;
            break;
            
        case __OBJGROUP_SHOP_WEAPON_I:
            Object->TextCorrection = -99;
            Object->Anim = 31;
            Object->Right = -1;
            Object->Left = 2;
            Object->Up = 6;
            Object->Down = -2;
            break;
            
        case __OBJGROUP_SIGN1_TALK:
            Object->TextCorrection = -90;
            Object->Right = 1;
            Object->Left = 1;
            Object->Up = 6;
            Object->Down = 1;
            break;
        case __OBJGROUP_SIGN2_TALK:
            Object->TextCorrection = -90;
            Object->Right = 1;
            Object->Left = 1;
            Object->Up = 6;
            Object->Down = 1;
            break;
        case __OBJGROUP_SIGN3_TALK:
            Object->TextCorrection = -90;
            Object->Right = 1;
            Object->Left = 1;
            Object->Up = 6;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_BACKPACK:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_BASKET_OPEN:
        case __OBJGROUP_BASKET_CLOSE:
            Object->Up = 2;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_BEVERAGE:
        case __OBJGROUP_EMPTY_GLASS:
        case __OBJGROUP_BEER_MUG_TOO_FULL:
        case __OBJGROUP_BEER_MUG:
        case __OBJGROUP_EMPTY_MUG:
        case __OBJGROUP_CUP_FULL:
        case __OBJGROUP_WOODEN_CUP:
            Object->Right = -1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_LONGUE_VUE:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_LEATHER_PANTS:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_NECKLACE_1:
        case __OBJGROUP_NECKLACE_2:
        case __OBJGROUP_NECKLACE_3:
            Object->Up = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_BATWINGS:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_COINS_BIG_PILE:
            Object->Left = 2;
            Object->Up = 3;
            Object->Down = 2;
            break;
            
        case __OBJGROUP_COINS:
            Object->Right = 0;
            Object->Left = 2;
            Object->Up = 1;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_GEMS_PURPLE:
        case __OBJGROUP_GEMS_YELLOW:
        case __OBJGROUP_GEMS_BLUE:
        case __OBJGROUP_GEMS_GREEN:
		case __OBJGROUP_GEM_AZURE:
		case __OBJGROUP_GEM_ORANGE:
		case __OBJGROUP_GEM_PINK:
		case __OBJGROUP_RUBIS_FOCUS:
		case __OBJGROUP_EMERALD_FOCUS:
		case __OBJGROUP_DIAMOND_FOCUS:
		case __OBJGROUP_SAPHIRE_FOCUS:
		case __OBJGROUP_EYE_OF_TIGER:
		case __OBJGROUP_MANA_STONE:
		case __OBJGROUP_BLUE_STONE:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_DARK_GEM:
            Object->Up = 2;
            Object->Down = 0;
            Object->Left = 1;
            Object->Right = -1;
            break;
            
            
        case __OBJGROUP_GEMS_PACK_1:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_GEMS_PACK_2:
            Object->Left = 2;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_GEMS_PACK_3:
            break;
            
        case __OBJGROUP_STUFFED_BASKET_1:
        case __OBJGROUP_STUFFED_BASKET_2:
        case __OBJGROUP_STUFFED_BASKET_3:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_ROPE:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_POUCH:
            Object->Right = -1;
            Object->Up = 1;
            Object->Left = 1;
            break;
            
        case __OBJGROUP_SCROLL_BLUE:
        case __OBJGROUP_SCROLL_OUVERT:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up =1;
            break;
            
        case __OBJGROUP_LIVRE_OUVERT:
        case __OBJGROUP_LIVRE_OUVERT_EPAIS:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up =1;
            break;
            
        case __OBJGROUP_BASTARD_SWORD:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
            
        //Dialsoft added
		case __OBJGROUP_FIREFLAIL:
		//Dialsoft end
		case __OBJGROUP_FLAIL:
            Object->Up = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        //Dialsoft added
		case __OBJGROUP_SPIKEDLEATHER:
		//dialsoft end
		case __OBJGROUP_LEATHER_ARMOR:
            Object->Up = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_PADDED_LEGGINGS:
            Object->Up = 1;
            Object->Left	= 1;
            break;
            
        case __OBJGROUP_PADDED_ARMOR:
            Object->Up = 2;
            Object->Left	= 1;
            break;
            
        case __OBJGROUP_PADDED_GLOVE:
            Object->Up = 1;
            Object->Left	= 1;
            break;
            
        case __OBJGROUP_BELT:
		case __OBJGROUP_BELT_JEWELS_BROWN:
		case __OBJGROUP_BELT_JEWELS_PURPLE:
		case __OBJGROUP_BELT_RED:
            Object->Up =2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_LEG_CLOTH1:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_BODY_CLOTH1:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_STUDDEDARMOR:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_ORC_SHIELD:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_WITCH_HAT1:
        case __OBJGROUP_WITCH_HAT2:
        case __OBJGROUP_WITCH_HAT3:
        case __OBJGROUP_WITCH_HAT4:
        case __OBJGROUP_WITCH_HAT5:
        case __OBJGROUP_ELVEN_HAT:
        case __OBJGROUP_NOEL_HAT:
        case __OBJGROUP_V2_HAT01:
        case __OBJGROUP_T4CP_CASQUE:
        case __OBJGROUP_T4CP_CASQUETTE:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_GOLDEN_STAR:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_GOLDENCROWN:
        case __OBJGROUP_MASQUE_GOBLIN:
        case __OBJGROUP_V2_MASK01:
        case __OBJGROUP_V2_MASQUE_GOBLIN_ICE:
            Object->Right = -1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_LEATHER_BOOTS:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = -1;
            break;
            
        case __OBJGROUP_BLACKLEATHER_BOOT:
        case __OBJGROUP_WHITELEATHER_BOOT:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = -1;
            break;
            
        case __OBJGROUP_LEATHER_GLOVE:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_TOOTH:
            Object->Right = -1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_STONELIFE:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_SHIELD:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_PINKLEAF:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_BRACELET:
		case __OBJGROUP_BRACELET_RUBIS:
		case __OBJGROUP_BRACELET_AZURE:
		case __OBJGROUP_BRACELET_SQUARE_WHITE:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_PADDED_BOOTS:
            Object->Right = 0;
            Object->Up = 1;
            Object->Down = 2;
            break;
            
        case __OBJGROUP_OAKTREELEAF:
            Object->Up = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_LIFEGEM:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_ICESHARD:
            Object->Right = 0;
            Object->Down = 2;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_FEATHER:
            Object->Right = 0;
            Object->Down = 2;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_VOLCANO_ROCK:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_DESTINY_GEM:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_RING_HELM:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_GLINTING_SWORD:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_CAMPBURNT:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 2;
            break;
            
        case __OBJGROUP_LEVIER:
            Object->Right = 1;
            Object->Down = 3;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_STAFF1:
            Object->Up = 1;
            Object->Right = 1;
            Object->Down = 2;
            Object->Left	= 1;
            break;
            
        case __OBJGROUP_STAFF2:
		case __OBJGROUP_NECROSTAFF:
      case __OBJGROUP_V2_SCEPTRE01:
      case __OBJGROUP_V2_SCEPTRE02:
      case __OBJGROUP_V2_SCEPTRE03:
            Object->Up = 1;
            Object->Right = 1;
            Object->Down = 2;
            Object->Left	= 1;
            break;
            
        case __OBJGROUP_STAFF3:
            Object->Up = 1;
            Object->Right = 1;
            Object->Down = 2;
            Object->Left	= 1;
            break;
            
        case __OBJGROUP_STAFF4:
            Object->Up = 1;
            Object->Right = 1;
            Object->Down = 2;
            Object->Left	= 1;
            break;
            
        case __OBJGROUP_MIRROR:
            Object->Right = 0;
            Object->Up = 3;
            Object->Left = 1;
            break;
            
        case __OBJGROUP_CHANDELLE_SUR_PIED:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 3;
            break;
            
        case __OBJGROUP_CHANDELLE:
            Object->Right = -1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_ROULEAU_A_PATRE:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_KITCHEN_KNIFE:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_LEATHER_HELM:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_LONG_SWORD:
        case __OBJGROUP_SHORT_SWORD:
        case __OBJGROUP_SWORD_ANGEL:
        case __OBJGROUP_CLAY2BLADES:
        case __OBJGROUP_SWORD_HERO:
        case __OBJGROUP_HAMM:
        //NM_TAG_ADD_SWORD
        case __OBJGROUP_V2_BUSSWORD01:
        case __OBJGROUP_V2_BUSSWORD02:
        case __OBJGROUP_V2_BUSSWORD03:
        case __OBJGROUP_V2_CLAYSWORD01:
        case __OBJGROUP_V2_CLAYSWORD02:
        case __OBJGROUP_V2_GLAIVESWORD02_ICE:
        case __OBJGROUP_V2_GLAIVESWORD01:
        case __OBJGROUP_V2_GLAIVESWORD02:
        case __OBJGROUP_V2_SWORD01:
        case __OBJGROUP_V2_SWORD02:
        case __OBJGROUP_V2_SWORD03:
        case __OBJGROUP_V2_SWORD04:
        case __OBJGROUP_V2_SWORD05:
        case __OBJGROUP_V2_SWORD06:
        case __OBJGROUP_V2_SWORD07:
        case __OBJGROUP_V2_SWORD08:
        case __OBJGROUP_V2_2BUSSWORD01:
        case __OBJGROUP_V2_2BUSSWORD02:
        case __OBJGROUP_V2_2BUSSWORD03:
        case __OBJGROUP_V2_2CLAYSWORD01:
        case __OBJGROUP_V2_2CLAYSWORD02:
        case __OBJGROUP_V2_2GLAIVESWORD01:
        case __OBJGROUP_V2_2GLAIVESWORD02:
        case __OBJGROUP_V2_2GLAIVESWORD02_ICE:
        case __OBJGROUP_V2_2SWORD01:
        case __OBJGROUP_V2_2SWORD02:
        case __OBJGROUP_V2_2SWORD03:
        case __OBJGROUP_V2_2SWORD04:
        case __OBJGROUP_V2_2SWORD05:
        case __OBJGROUP_V2_2SWORD06:
        case __OBJGROUP_V2_2SWORD07:
        case __OBJGROUP_V2_2SWORD08:
        case __OBJGROUP_V2_SP01:
        case __OBJGROUP_V2_SP02:
        case __OBJGROUP_V2_SP03:
        case __OBJGROUP_V2_SP04:
        case __OBJGROUP_V2_SP05:
        case __OBJGROUP_V2_SP06:
        case __OBJGROUP_V2_SP07:
        case __OBJGROUP_V2_2SP01:
        case __OBJGROUP_V2_2SP02:
        case __OBJGROUP_V2_2SP03:
        case __OBJGROUP_V2_2SP04:
        case __OBJGROUP_V2_2SP05:
        case __OBJGROUP_V2_2SP06:
        case __OBJGROUP_V2_HAMMER01 :
        case __OBJGROUP_V2_HAMMER02 :
        case __OBJGROUP_V2_HAMMER03 :
        case __OBJGROUP_V2_HAMMER04 :
        case __OBJGROUP_V2_HAMMER05 :
        case __OBJGROUP_V2_HAMMER06 :
        case __OBJGROUP_V2_HAMMER07 :
        case __OBJGROUP_V2_2HAMMER01:
        case __OBJGROUP_V2_2HAMMER02:
        case __OBJGROUP_T4CP_MATRAQUE:
        case __OBJGROUP_T4CP_BAT:
        case __OBJGROUP_T4CP_12:
            Object->Up = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_SPEAR_AXE:
            Object->Right = 1;
            Object->Down = 2;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_SPEAR:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_MAUL:
            Object->Up = 1;
            Object->Down = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_OGRECLUB:
		case __OBJGROUP_FLOWERS:
            Object->Up = 1;
            Object->Down = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
      case __OBJGROUP_SINGLE_AXE:
      case __OBJGROUP_DOUBLE_AXE:
      case __OBJGROUP_TIGERAXE:
      case __OBJGROUP_ANCIENTAXE:
      case __OBJGROUP_AXE_DESTRUCTION:
      case __OBJGROUP_V2_HACHE01:
      case __OBJGROUP_V2_HACHE02:
      case __OBJGROUP_V2_HACHE03:
      case __OBJGROUP_V2_HACHE04:
      case __OBJGROUP_V2_HACHE05:
      case __OBJGROUP_V2_HACHE06:
      case __OBJGROUP_V2_HACHE07:
      case __OBJGROUP_V2_HACHE08:
      case __OBJGROUP_V2_HACHE09:
      case __OBJGROUP_V2_HACHE10:
      case __OBJGROUP_V2_HACHE11:
      case __OBJGROUP_V2_2HACHE01:
      case __OBJGROUP_V2_2HACHE02:
      case __OBJGROUP_V2_2HACHE03:
      case __OBJGROUP_V2_2HACHE04:
      case __OBJGROUP_V2_2HACHE05:
      case __OBJGROUP_V2_2HACHE06:
         Object->Down = 2;
         Object->Up = 1;
         break;
            
        case __OBJGROUP_MACE:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_SPIKE_MACE:
            Object->Up = 1;
            Object->Down = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_RINGS_1:
        case __OBJGROUP_RINGS_2:
        case __OBJGROUP_RINGS_3:
        case __OBJGROUP_RINGS_4:
        case __OBJGROUP_RINGS_5:
		case __OBJGROUP_RING_3RUBIS:
		case __OBJGROUP_RING_RED:
		case __OBJGROUP_RING_STARGOLD:
		case __OBJGROUP_RING_CUT_GREEN:
		case __OBJGROUP_RING_CUT_RED:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = -1;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_DAGGER:
        case __OBJGROUP_V2_DAGGER_VIP:
        case __OBJGROUP_V2_DAGGER01:
        case __OBJGROUP_V2_DAGGER02:
        case __OBJGROUP_V2_DAGGER03:
        case __OBJGROUP_V2_DAGGER04:
        case __OBJGROUP_V2_DAGGER05:
        case __OBJGROUP_V2_2DAGGER01:
        case __OBJGROUP_V2_2DAGGER02:
        case __OBJGROUP_V2_2DAGGER03:
        case __OBJGROUP_V2_2DAGGER04:
        case __OBJGROUP_V2_2DAGGER05:
        case __OBJGROUP_V2_2DAGGER06:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = -1;
            break;
            
        case __OBJGROUP_BAROSSA_SHIELD:
            Object->Right = 0;
            Object->Up = 2;
            Object->Left	= 1;
            break;
            
        case __OBJGROUP_FRUIT_1:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_CHAIN_COIF:
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_SCALE_ARMOR:
            Object->Up = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_KITCHEN_SPOON:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_KITCHEN_FORK:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_KITCHEN_BOWL:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_RECTANGLE_CRATES:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 2;
            break;
            
        case __OBJGROUP_CUBIQ_CRATES:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 3;
            break;
            
        case __OBJGROUP_SPIDER_EYES:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_SKEL_BONE:
            Object->Right = 0;
            Object->Left = 2;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_AXE:
            Object->Up = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_CHAIN_BODY:
            Object->Up = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_CHAIN_LEGS:
            Object->Up = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
		//Dialsoft new helm
		case __OBJGROUP_REDFEATHER: 
		//Dialsoft END
		//Tiamat Changes Start
		case __OBJGROUP_PLATE_HELM_GOLD:
		case __OBJGROUP_PLATE_HELM_SILVER:
		case __OBJGROUP_PLATE_HELM_BLACK:
		case __OBJGROUP_PLATE_HELM_BLUE:
		case __OBJGROUP_PLATE_HELM_GREEN:
		case __OBJGROUP_PLATE_HELM_RED:
		case __OBJGROUP_PLATE_HELM_PURPLE:
		case __OBJGROUP_PLATE_HELM_EMERALD:
      case __OBJGROUP_PLATE_HELM:
      case __OBJGROUP_V2_HAUME01:
      case __OBJGROUP_V2_HAUME02:
      case __OBJGROUP_V2_HAUME03:
      case __OBJGROUP_V2_HAUME04:
      case __OBJGROUP_V2_HAUME04_BL:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;

        case __OBJGROUP_PLATE_LEGS_GOLD:
		case __OBJGROUP_PLATE_LEGS_SILVER:
		case __OBJGROUP_PLATE_LEGS_BLACK:
		case __OBJGROUP_PLATE_LEGS_BLUE:
		case __OBJGROUP_PLATE_LEGS_GREEN:
		case __OBJGROUP_PLATE_LEGS_RED:
		case __OBJGROUP_PLATE_LEGS_PURPLE:
		case __OBJGROUP_PLATE_LEGS_EMERALD:    
      case __OBJGROUP_PLATE_LEGS:
      case __OBJGROUP_MAN_ARMOR01_LEGS_GR:
      case __OBJGROUP_MAN_ARMOR01_LEGS_BL:
            Object->Right = 0;
            Object->Up = 1;
            break;
        
		case __OBJGROUP_PLATE_BOOT_GOLD:
		case __OBJGROUP_PLATE_BOOT_SILVER:
		case __OBJGROUP_PLATE_BOOT_BLACK:
		case __OBJGROUP_PLATE_BOOT_BLUE:
		case __OBJGROUP_PLATE_BOOT_GREEN:
		case __OBJGROUP_PLATE_BOOT_RED:
		case __OBJGROUP_PLATE_BOOT_PURPLE:
		case __OBJGROUP_PLATE_BOOT_EMERALD:
      case __OBJGROUP_PLATE_BOOT:
      case __OBJGROUP_MAN_ARMOR01_FOOT_GR:
      case __OBJGROUP_MAN_ARMOR01_FOOT_BL:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
        
		case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_GOLD:
		case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_SILVER:
		case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLACK:
		case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLUE:
		case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_GREEN:
		case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_RED:
		case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_PURPLE:
		case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_EMERALD:
        case __OBJGROUP_PLATE_ARMOR_W_SLEEVE:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
        
		case __OBJGROUP_PLATE_GLOVE_GOLD:
		case __OBJGROUP_PLATE_GLOVE_SILVER:
		case __OBJGROUP_PLATE_GLOVE_BLACK:
		case __OBJGROUP_PLATE_GLOVE_BLUE:
		case __OBJGROUP_PLATE_GLOVE_GREEN:
		case __OBJGROUP_PLATE_GLOVE_RED:
		case __OBJGROUP_PLATE_GLOVE_PURPLE:
		case __OBJGROUP_PLATE_GLOVE_EMERALD:
      case __OBJGROUP_PLATE_GLOVE:
      case __OBJGROUP_MAN_ARMOR01_HAND_GR:
      case __OBJGROUP_MAN_ARMOR01_HAND_BL:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
        
		case __OBJGROUP_PLATE_ARMOR_GOLD:
		case __OBJGROUP_PLATE_ARMOR_SILVER:
		case __OBJGROUP_PLATE_ARMOR_BLACK:
		case __OBJGROUP_PLATE_ARMOR_BLUE:
		case __OBJGROUP_PLATE_ARMOR_GREEN:
		case __OBJGROUP_PLATE_ARMOR_RED:
		case __OBJGROUP_PLATE_ARMOR_PURPLE:
		case __OBJGROUP_PLATE_ARMOR_EMERALD:
      case __OBJGROUP_PLATE_ARMOR:
      case __OBJGROUP_MAN_ARMOR01_BODY_GR:
      case __OBJGROUP_MAN_ARMOR01_BODY_BL:

            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
          
		//Tiamat Changes End
        case __OBJGROUP_ROMAN_SHIELD:
            Object->Up = 2;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_DARK_SWORD:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_COINS_PILE:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_NECROMANROBE:
            Object->Up = 2;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_WHITEROBE:
		case __OBJGROUP_FWHITEROBE_BLUE:
		case __OBJGROUP_FWHITEROBE_GREEN:
		case __OBJGROUP_FWHITEROBE_DARKBLUE:
		case __OBJGROUP_FWHITEROBE_VIOLET:
		case __OBJGROUP_FWHITEROBE_PURPLE:
		case __OBJGROUP_FWHITEROBE_YELLOW:
		case __OBJGROUP_FWHITEROBE_GRAY:
		case __OBJGROUP_FWHITEROBE_MAUVE:
		case __OBJGROUP_FWHITEROBE_ORANGE:
		case __OBJGROUP_FWHITEROBE_BRUN:
		case __OBJGROUP_FWHITEROBE_DARK:
            Object->Up = 1;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 1;
            break;
        case __OBJGROUP_LICHROBE:
        case __OBJGROUP_ARMORED_ROBE:
        case __OBJGROUP_MAGEROBE:
            Object->Up = 1;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 1;
            break;
            
        case __OBJGROUP_BATTLE_SWORD:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_HORNED_HELMET:
            Object->Right = -1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_REAL_DARKSWORD:
            Object->Right = 0;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_TORCH2:
            Object->Down = 2;
            Object->Up = 1;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_TORCHE_I:
            Object->Anim = 21;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_CRATES:
            Object->Up = 4;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_FIRECAMP:
            Object->Anim = 11;
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 2;
            break;
            
        case __OBJGROUP_WHIRLPOOL:
            Object->Anim = 20; 
            Object->Right = 0;
            Object->Left = 0;
            Object->Up = 0;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_CAULDRON:
			Object->TextCorrection = -120;
            Object->Anim = 21; 
            Object->Right = 0;
            Object->Left = 0;
            Object->Up = 0;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_SKAVEN_CORPSE1:
            Object->Right = 1;
            Object->Left = 1;
            Object->Up = 3;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_SKAVEN_CORPSE2:
            Object->Right = 1;
            Object->Left = 1;
            Object->Up = 4;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_SKAVEN_CORPSE3:
            Object->Right = 0;
            Object->Left = 1;
            Object->Up = 4;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_SKAVEN_CORPSE4:
            Object->Right = 1;
            Object->Left = 1;
            Object->Up = 4;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_SKAVEN_CORPSE_I1:
            Object->Right = -1;
            Object->Left = 2;
            Object->Up = 4;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_SKAVEN_CORPSE_I2:
            Object->Right = 0;
            Object->Left = 2;
            Object->Up = 4;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_SKAVEN_CORPSE_I3:
            Object->Right = 1;
            Object->Left = 0;
            Object->Up = 4;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_SKAVEN_CORPSE_I4:
            Object->Right = 0;
            Object->Left = 2;
            Object->Up = 4;
            Object->Down = 1;
            break;
            
        case __OBJGROUP_WHIRLPOOL2:
            Object->Anim = 20; 
            Object->Right = 0;
            Object->Left = 0;
            Object->Up = 0;
            Object->Down = 0;
            break;
            
        case __OBJGROUP_POT_RED:
        case __OBJGROUP_POT_GREEN:
            Object->Up = 2;
            Object->Down  = 2;
            Object->Left	= 1;
            Object->Right = -1;
            break;
            
        case __OBJGROUP_WARHAMMER:
        case __OBJGROUP_DWARF_HAMMER:
            Object->Up = 2;
            Object->Left	= 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_WOODEN_DOOR_OPENED_I:
        case __OBJGROUP_WOODEN_DOOR2_OPENED_I:
        case __OBJGROUP_WOODEN_DOOR3_OPENED_I:
        case __OBJGROUP_WOODEN_DOOR4_OPENED_I:
        case __OBJGROUP_WOODEN_DOOR5_OPENED_I:
        case __OBJGROUP_WOODEN_DOOR6_OPENED_I:
        case __OBJGROUP_WOODEN_DOOR7_OPENED_I:
        case __OBJGROUP_WOODEN_DOOR8_OPENED_I:
            Object->Overlap.Dither = FALSE;
            Object->onUse.CopyWave(&SoundFX[0]);
            Object->TextCorrection = -96;
            Object->Up = 8;
            Object->Down  = -1;
            Object->Left	= 1;
            break;
            
        case __OBJGROUP_GODPOT_BLUE:
        case __OBJGROUP_GODPOT_GREEN:
        case __OBJGROUP_GODPOT_RED:
        case __OBJGROUP_GODPOT_YELLOW:
            Object->TextCorrection = -26;
            Object->Up = 1;
            Object->Left	= 1;
            Object->Down = 4;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_CHEST2_I:
            //				Object->onUse.CopyWave(&SoundFX[111]);
            Object->TextCorrection = -26;
            Object->Up = 1;
            Object->Down =  2;
            Object->Left = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_CHEST2:
            //				Object->onUse.CopyWave(&SoundFX[111]);
            Object->TextCorrection = -26;
            Object->Up = 1;
            Object->Down =  2;
            Object->Left = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_CHEST_OPEN_I:
            //				Object->onUse.CopyWave(&SoundFX[111]);
            Object->TextCorrection = -26;
            Object->Up = 1;
            Object->Down =  2;
            Object->Left = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_CHEST_OPEN:
            //				Object->onUse.CopyWave(&SoundFX[111]);
            Object->TextCorrection = -26;
            Object->Up = 1;
            Object->Down =  2;
            Object->Left = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_CHEST_I:
            //				Object->onUse.CopyWave(&SoundFX[111]);
            Object->TextCorrection = -26;
            Object->Up = 1;
            Object->Down =  2;
            Object->Left = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_CHEST:
            //				Object->onUse.CopyWave(&SoundFX[111]);
            Object->TextCorrection = -26;
            Object->Up = 2;
            Object->Down =	1;
            Object->Left = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_CHEST_TROLL:
            Object->TextCorrection = -26;
            Object->Up = 1;
            Object->Down =	2;
            Object->Left = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_WOODEN_DOOR_CLOSED_I:
        case __OBJGROUP_WOODEN_DOOR2_CLOSED_I:
        case __OBJGROUP_WOODEN_DOOR3_CLOSED_I:
        case __OBJGROUP_WOODEN_DOOR4_CLOSED_I:
        case __OBJGROUP_WOODEN_DOOR5_CLOSED_I:
        case __OBJGROUP_WOODEN_DOOR6_CLOSED_I:
        case __OBJGROUP_WOODEN_DOOR7_CLOSED_I:
        case __OBJGROUP_WOODEN_DOOR8_CLOSED_I:
            Object->Overlap.Dither = FALSE;
            Object->onUse.CopyWave(&SoundFX[1]);
            Object->TextCorrection = -96;
            Object->Up = 7;
            Object->Down  = 0;
            Object->Left = 2;
            Object->Right = 1;
            break;
        case __OBJGROUP_WOODEN_DOOR_OPENED:
        case __OBJGROUP_WOODEN_DOOR2_OPENED:
        case __OBJGROUP_WOODEN_DOOR3_OPENED:
        case __OBJGROUP_WOODEN_DOOR4_OPENED:
        case __OBJGROUP_WOODEN_DOOR5_OPENED:
        case __OBJGROUP_WOODEN_DOOR6_OPENED:
        case __OBJGROUP_WOODEN_DOOR7_OPENED:
        case __OBJGROUP_WOODEN_DOOR8_OPENED:
            Object->Overlap.Dither = FALSE;
            Object->onUse.CopyWave(&SoundFX[0]);
            Object->TextCorrection = -96;
            Object->Up = 8;
            Object->Down  = -1;
            Object->Left = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_SCROLL:
            Object->Right = -1;
            Object->Left = 1;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_PRIEST_ANKH:
            Object->Right = 0;
            Object->Left = 2;
            Object->Up = 1;
            break;
            
        case __OBJGROUP_WOODEN_DOOR_CLOSED:
        case __OBJGROUP_WOODEN_DOOR2_CLOSED:
        case __OBJGROUP_WOODEN_DOOR3_CLOSED:
        case __OBJGROUP_WOODEN_DOOR4_CLOSED:
        case __OBJGROUP_WOODEN_DOOR5_CLOSED:
        case __OBJGROUP_WOODEN_DOOR6_CLOSED:
        case __OBJGROUP_WOODEN_DOOR7_CLOSED:
        case __OBJGROUP_WOODEN_DOOR8_CLOSED:
            Object->Overlap.Dither = FALSE;
            Object->onUse.CopyWave(&SoundFX[1]);
            Object->TextCorrection = -96;
            Object->Up = 7;
            Object->Down  = 0;
            Object->Left = 1;
            Object->Right = 0;
            break;
            
        case __OBJGROUP_FANCY_SHORT_BOW:
            Object->Right = 0;         
            Object->Up = 1;
            break;
            
        case __OBJGROUP_FANCY_LONG_BOW:
        case __OBJGROUP_LARGE_BOW:
        case __OBJGROUP_LARGE_BOW2:
        case __OBJGROUP_NEW_BOW01:
		  case __OBJGROUP_V2_BOW01:
        case __OBJGROUP_V2_BOW02:
        case __OBJGROUP_V2_BOW03:
        case __OBJGROUP_V2_BOW04:
        case __OBJGROUP_V2_BOW05:
        case __OBJGROUP_V2_BOW05_ICE:
        case __OBJGROUP_V2_BOW06:
        case __OBJGROUP_V2_BOW07:
        case __OBJGROUP_V2_BOW08:
        case __OBJGROUP_V2_BOW09:
        case __OBJGROUP_V2_BOW10:
            
            Object->Right = 0;         
            Object->Up = 1;
            
            break;
        case __OBJGROUP_QUIVER:
        case __OBJGROUP_BLUE_QUIVER:
        case __OBJGROUP_YELLOW_QUIVER:
        case __OBJGROUP_GREEN_QUIVER:
        case __OBJGROUP_RED_QUIVER:
        case __OBJGROUP_BLACK_QUIVER:
            
            Object->Up = 1;
            Object->Right = 0;
            
            break;
        case __OBJGROUP_RED_GEM:
        case __OBJGROUP_PURPLE_GEM:
        case __OBJGROUP_SMALL_RED_GEM:
        case __OBJGROUP_SMALL_PURPLE_GEM:
            Object->Right = -1;
            Object->Up = 1;
            break;

		
            
		case __OBJGROUP_ORGUE:
			Object->Up = 0;
			Object->Left = 0;
			Object->Right = 0;
			Object->Down = 0;
			break;

        case __SPELL_LIGHTNINGBOLT:
            Object->Type3D = 55;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_ICESHARD:
            Object->Type3D = 57;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_STONESHARD:
            Object->Type3D = 58;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
        case __SPELL_ARROW:
            Object->Type3D = 78;
            SetMonsterStats(Type, Object);
            //Object->onAttack[0].Play(FALSE);
            
            break;
        case __SPELL_POISONARROW:
            Object->Type3D = 56;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_FIREBALL:
            Object->Type3D = 40;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_FIREBOLT:
            Object->Type3D = 52;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_HEALING:
            Object->Type3D = 39;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_SERAPH_MALE:
            Object->Type3D = 109;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            Object->SummonID = __PLAYER_HUMAN_PUPPET;
            Object->OC = 1;
            Object->Light = 100;
            break;
            
        case __SPELL_SERAPH:
            Move2 = false;
            seraphDisplay = false;
            goodSeraph = true;
            Object->Type3D = 109;
            SetMonsterStats(Type, Object);
            //Object->onAttack[0].Play(FALSE);//
            Object->OC = 1;
            Object->Light = 100;
            MainObject.Direction = 1;
            Direction2 = 1;
            break;
            
        case __SPELL_SERAPH_FEMALE:
            Object->Type3D = 109;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            Object->SummonID = __PLAYER_HUMAN_FEMALE;
            Object->OC = 1;
            Object->Light = 100;
            break;
            
        case __SPELL_EVIL_SERAPH_MALE:
            Object->Type3D = 115;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            Object->SummonID = __PLAYER_HUMAN_PUPPET;
            Object->OC = 1;
            break;
            
        case __SPELL_EVIL_SERAPH:
            Move2 = false;
            seraphDisplay = false;
            goodSeraph = false;
            Object->Type3D = 115;
            SetMonsterStats(Type, Object);
            //Object->onAttack[0].Play(FALSE);//
            Object->OC = 1;
            MainObject.Direction = 2;
            Direction2 = 2;
            break;
            
        case __SPELL_EVIL_SERAPH_FEMALE:
            Object->Type3D = 115;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            Object->SummonID = __PLAYER_HUMAN_FEMALE;
            Object->OC = 1;
            break;
            
        case __SPELL_BLESS:
            Object->Type3D = 79;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_ENTANGLE:
            Object->Type3D = 80;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_GLACIER:
            Object->Type3D = 81;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_BOULDERS:
            Object->Type3D = 82;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_METEORS:
            Object->Type3D = 83;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_GATEWAY_1:
            Object->Type3D = 84;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_HEALSERIOUS:
            Object->Type3D = 59;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_POISONCLOUD:
            Object->Type3D = 64;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_ICECLOUD:
            Object->Type3D = 63;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_ROCKFLY:
            Object->Type3D = 62;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_FREEZE:
            Object->Type3D = 61;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_SEMIBIGEXPLOSION:
            Object->Type3D = 60;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_BROKEN_ARROW:
            Object->Type3D = 87;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_CURSE:
            Object->Type3D = 53;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_ELECTRICSHIELD:
            Object->Type3D = 54;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_GREENWIPE:
            Object->Type3D = 41;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_BLUEWIPE:
            Object->Type3D = 42;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_FIREWIPE:
            Object->Type3D = 43;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_PENTACLE:
            Object->Type3D = 44;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_REDWIPE:
            Object->Type3D = 45;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_SHOCKWAVE:
            Object->Type3D = 46;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_SMALLEXPLOSION:
            Object->Type3D = 47;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_GREATEXPLOSION:
            Object->Type3D = 48;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_FLAK2:
            Object->Type3D = 49;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_FLAK:
            Object->Type3D = 50;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_GREATBOLT:
            Object->Type3D = 51;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_RED_ENERGY_BALL:
        case __SPELL_CURSE_WITH_RED_BALL:
        case __SPELL_FIREWIPE_WITH_RED_BALL:
        case __SPELL_FLAK_WITH_RED_BALL:
        case __SPELL_FLAK2_WITH_RED_BALL:
        case __SPELL_GATEWAY_1_WITH_RED_BALL:
        case __SPELL_GREATEXPLOSION_WITH_RED_BALL:
        case __SPELL_METEOR_WITH_RED_BALL:
        case __SPELL_PENTACLE_WITH_RED_BALL:
        case __SPELL_REDWIPE_WITH_RED_BALL:
        case __SPELL_SEMIBIGEXPLOSION_WITH_RED_BALL:
        case __SPELL_SMALLEXPLOSION_WITH_RED_BALL:
        case __SPELL_FIRECIRCLE_WITH_RED_BALL:
        // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_BLACK_WITH_RED_BALL:
        case __SPELL_METEOR_WHITE_WITH_RED_BALL:
        case __SPELL_METEOR_PURPLE_WITH_RED_BALL:
        case __SPELL_METEOR_COBOLT_WITH_RED_BALL:
        case __SPELL_METEOR_GREEN_WITH_RED_BALL:
        case __SPELL_METEOR_CANDY_WITH_RED_BALL:
        case __SPELL_METEOR_GOLD_WITH_RED_BALL:
        case __SPELL_METEOR_BROWN_WITH_RED_BALL:
        case __SPELL_METEOR_DARK_WITH_RED_BALL:
        case __SPELL_METEOR_BLUE_WITH_RED_BALL:
        case __SPELL_METEOR_VIOLET_WITH_RED_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_RED_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_RED_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_RED_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_RED_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_RED_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_RED_BALL:
        case __SPELL_GREATBOLT_RED_WITH_RED_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_RED_BALL:
        // END DIALSOFT NEW SPELLS

            Object->Type3D = 88;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_BLUE_ENERGY_BALL:
        case __SPELL_BLUEWIPE_WITH_BLUE_BALL:
        case __SPELL_CURSE_WITH_BLUE_BALL:
        case __SPELL_ELECTRICSHIELD_WITH_BLUE_BALL:
        case __SPELL_FREEZE_WITH_BLUE_BALL:
        case __SPELL_GLACIER_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_WITH_BLUE_BALL:
        case __SPELL_GREENWIPE_WITH_BLUE_BALL:
        case __SPELL_HEALING_WITH_BLUE_BALL:
        case __SPELL_HEALSERIOUS_WITH_BLUE_BALL:
        case __SPELL_ICECLOUD_WITH_BLUE_BALL:
        case __SPELL_POISONCLOUD_WITH_BLUE_BALL:
        // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_WITH_BLUE_BALL:
        case __SPELL_METEOR_BLACK_WITH_BLUE_BALL:
        case __SPELL_METEOR_WHITE_WITH_BLUE_BALL:
        case __SPELL_METEOR_PURPLE_WITH_BLUE_BALL:
        case __SPELL_METEOR_COBOLT_WITH_BLUE_BALL:
        case __SPELL_METEOR_GREEN_WITH_BLUE_BALL:
        case __SPELL_METEOR_CANDY_WITH_BLUE_BALL:
        case __SPELL_METEOR_GOLD_WITH_BLUE_BALL:
        case __SPELL_METEOR_BROWN_WITH_BLUE_BALL:
        case __SPELL_METEOR_DARK_WITH_BLUE_BALL:
        case __SPELL_METEOR_BLUE_WITH_BLUE_BALL:
        case __SPELL_METEOR_VIOLET_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_RED_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_BLUE_BALL:
           // END DIALSOFT NEW SPELLS

            Object->Type3D = 89;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_YELLOW_ENERGY_BALL:
        case __SPELL_BLESS_WITH_YELLOW_BALL:
        case __SPELL_BLUEWIPE_WITH_YELLOW_BALL:
        case __SPELL_CURSE_WITH_YELLOW_BALL:
        case __SPELL_ELECTRICSHIELD_WITH_YELLOW_BALL:
        case __SPELL_FREEZE_WITH_YELLOW_BALL:
        case __SPELL_GATEWAY_1_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_WITH_YELLOW_BALL:
        case __SPELL_ICECLOUD_WITH_YELLOW_BALL:
        // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_WITH_YELLOW_BALL:
        case __SPELL_METEOR_BLACK_WITH_YELLOW_BALL:
        case __SPELL_METEOR_WHITE_WITH_YELLOW_BALL:
        case __SPELL_METEOR_PURPLE_WITH_YELLOW_BALL:
        case __SPELL_METEOR_COBOLT_WITH_YELLOW_BALL:
        case __SPELL_METEOR_GREEN_WITH_YELLOW_BALL:
        case __SPELL_METEOR_CANDY_WITH_YELLOW_BALL:
        case __SPELL_METEOR_GOLD_WITH_YELLOW_BALL:
        case __SPELL_METEOR_BROWN_WITH_YELLOW_BALL:
        case __SPELL_METEOR_DARK_WITH_YELLOW_BALL:
        case __SPELL_METEOR_BLUE_WITH_YELLOW_BALL:
        case __SPELL_METEOR_VIOLET_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_RED_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_YELLOW_BALL:
           // END DIALSOFT NEW SPELLS

            Object->Type3D = 90;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_GREEN_ENERGY_BALL:
        case __SPELL_BOULDERS_WITH_GREEN_BALL:
        case __SPELL_CURSE_WITH_GREEN_BALL:
        case __SPELL_ENTANGLE_WITH_GREEN_BALL:
        case __SPELL_FLAK_WITH_GREEN_BALL:
        case __SPELL_FLAK2_WITH_GREEN_BALL:
        case __SPELL_GREENWIPE_WITH_GREEN_BALL:
        case __SPELL_PENTACLE_WITH_GREEN_BALL:
        case __SPELL_POISONCLOUD_WITH_GREEN_BALL:
        case __SPELL_ROCKFLY_WITH_GREEN_BALL:
        // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_WITH_GREEN_BALL:
        case __SPELL_METEOR_BLACK_WITH_GREEN_BALL:
        case __SPELL_METEOR_WHITE_WITH_GREEN_BALL:
        case __SPELL_METEOR_PURPLE_WITH_GREEN_BALL:
        case __SPELL_METEOR_COBOLT_WITH_GREEN_BALL:
        case __SPELL_METEOR_GREEN_WITH_GREEN_BALL:
        case __SPELL_METEOR_CANDY_WITH_GREEN_BALL:
        case __SPELL_METEOR_GOLD_WITH_GREEN_BALL:
        case __SPELL_METEOR_BROWN_WITH_GREEN_BALL:
        case __SPELL_METEOR_DARK_WITH_GREEN_BALL:
        case __SPELL_METEOR_BLUE_WITH_GREEN_BALL:
        case __SPELL_METEOR_VIOLET_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_RED_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_GREEN_BALL:
           // END DIALSOFT NEW SPELLS

            Object->Type3D = 91;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_WHITE_ENERGY_BALL:
        case __SPELL_BLESS_WITH_WHITE_BALL:
        case __SPELL_BLUEWIPE_WITH_WHITE_BALL:
        case __SPELL_CURSE_WITH_WHITE_BALL:
        case __SPELL_ELECTRICSHIELD_WITH_WHITE_BALL:
        case __SPELL_FLAK_WITH_WHITE_BALL:
        case __SPELL_FLAK2_WITH_WHITE_BALL:
        case __SPELL_HEALING_WITH_WHITE_BALL:
        case __SPELL_HEALSERIOUS_WITH_WHITE_BALL:
        case __SPELL_ICECLOUD_WITH_WHITE_BALL:
        // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_WITH_WHITE_BALL:
        case __SPELL_METEOR_BLACK_WITH_WHITE_BALL:
        case __SPELL_METEOR_WHITE_WITH_WHITE_BALL:
        case __SPELL_METEOR_PURPLE_WITH_WHITE_BALL:
        case __SPELL_METEOR_COBOLT_WITH_WHITE_BALL:
        case __SPELL_METEOR_GREEN_WITH_WHITE_BALL:
        case __SPELL_METEOR_CANDY_WITH_WHITE_BALL:
        case __SPELL_METEOR_GOLD_WITH_WHITE_BALL:
        case __SPELL_METEOR_BROWN_WITH_WHITE_BALL:
        case __SPELL_METEOR_DARK_WITH_WHITE_BALL:
        case __SPELL_METEOR_BLUE_WITH_WHITE_BALL:
        case __SPELL_METEOR_VIOLET_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_RED_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_WHITE_BALL:
           // END DIALSOFT NEW SPELLS

            Object->Type3D = 92;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_BLACK_ENERGY_BALL:
        case __SPELL_CURSE_WITH_BLACK_BALL:
        case __SPELL_ENTANGLE_WITH_BLACK_BALL:
        case __SPELL_FIREWIPE_WITH_BLACK_BALL:
        case __SPELL_GREATEXPLOSION_WITH_BLACK_BALL:
        case __SPELL_GREENWIPE_WITH_BLACK_BALL:
        case __SPELL_METEOR_WITH_BLACK_BALL:
        case __SPELL_REDWIPE_WITH_BLACK_BALL:
        case __SPELL_SEMIBIGEXPLOSION_WITH_BLACK_BALL:
        case __SPELL_SMALLEXPLOSION_WITH_BLACK_BALL:
        case __SPELL_FIRECIRCLE_WITH_BLACK_BALL:
        // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_BLACK_WITH_BLACK_BALL:
        case __SPELL_METEOR_WHITE_WITH_BLACK_BALL:
        case __SPELL_METEOR_PURPLE_WITH_BLACK_BALL:
        case __SPELL_METEOR_COBOLT_WITH_BLACK_BALL:
        case __SPELL_METEOR_GREEN_WITH_BLACK_BALL:
        case __SPELL_METEOR_CANDY_WITH_BLACK_BALL:
        case __SPELL_METEOR_GOLD_WITH_BLACK_BALL:
        case __SPELL_METEOR_BROWN_WITH_BLACK_BALL:
        case __SPELL_METEOR_DARK_WITH_BLACK_BALL:
        case __SPELL_METEOR_BLUE_WITH_BLACK_BALL:
        case __SPELL_METEOR_VIOLET_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_RED_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_BLACK_BALL:
           // END DIALSOFT NEW SPELLS
            Object->Type3D = 93;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_PURPLE_ENERGY_BALL:
        case __SPELL_BLUEWIPE_WITH_PURPLE_BALL:
        case __SPELL_CURSE_WITH_PURPLE_BALL:
        case __SPELL_ELECTRICSHIELD_WITH_PURPLE_BALL:
        case __SPELL_FLAK_WITH_PURPLE_BALL:
        case __SPELL_FLAK2_WITH_PURPLE_BALL:
        case __SPELL_GATEWAY_1_WITH_PURPLE_BALL:
        case __SPELL_GREATEXPLOSION_WITH_PURPLE_BALL:
        case __SPELL_GREENWIPE_WITH_PURPLE_BALL:
        case __SPELL_METEOR_WITH_PURPLE_BALL:
        case __SPELL_PENTACLE_WITH_PURPLE_BALL:
        case __SPELL_POISONCLOUD_WITH_PURPLE_BALL:
        case __SPELL_SEMIBIGEXPLOSION_WITH_PURPLE_BALL:
        case __SPELL_SMALLEXPLOSION_WITH_PURPLE_BALL:
        case __SPELL_FIRECIRCLE_WITH_PURPLE_BALL:
           // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_BLACK_WITH_PURPLE_BALL:
        case __SPELL_METEOR_WHITE_WITH_PURPLE_BALL:
        case __SPELL_METEOR_PURPLE_WITH_PURPLE_BALL:
        case __SPELL_METEOR_COBOLT_WITH_PURPLE_BALL:
        case __SPELL_METEOR_GREEN_WITH_PURPLE_BALL:
        case __SPELL_METEOR_CANDY_WITH_PURPLE_BALL:
        case __SPELL_METEOR_GOLD_WITH_PURPLE_BALL:
        case __SPELL_METEOR_BROWN_WITH_PURPLE_BALL:
        case __SPELL_METEOR_DARK_WITH_PURPLE_BALL:
        case __SPELL_METEOR_BLUE_WITH_PURPLE_BALL:
        case __SPELL_METEOR_VIOLET_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_RED_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_PURPLE_BALL:
           // END DIALSOFT NEW SPELLS

            Object->Type3D = 94;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_FIRECIRCLE:
            Object->Type3D = 95;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
        case __SPELL_NEWFIREBALL:
            Object->Type3D = 96;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
        
		// New added by Dialsoft - BEGIN
		case __SPELL_MEDIUM_HEALING:
            Object->Type3D = 121;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
		case __SPELL_ICE_TREE:
            Object->Type3D = 122;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
		case __SPELL_SUPRAHEAL:
            Object->Type3D = 127;
            SetMonsterStats(Type, Object);
            Object->onAttack[0].Play(FALSE);
            break;
            
      // BEGIN DIALSOFT NEW SPELLS
      // FIREBALLS
      case __SPELL_NEWFIREBALL_BLACK:
         Object->Type3D = 152;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_NEWFIREBALL_WHITE:
         Object->Type3D = 153;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_NEWFIREBALL_AZURE:
         Object->Type3D = 154;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_NEWFIREBALL_MAGENTA:
         Object->Type3D = 155;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_NEWFIREBALL_YELLOW:
         Object->Type3D = 156;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_NEWFIREBALL_PURPLE:
         Object->Type3D = 157;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
         // FIREBOLTS
      case __SPELL_FIREBOLT_BLACK:
         Object->Type3D = 158;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_FIREBOLT_WHITE:
         Object->Type3D = 159;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_FIREBOLT_YELLOW:
         Object->Type3D = 160;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_FIREBOLT_GREEN:
         Object->Type3D = 161;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_FIREBOLT_BLUE:
         Object->Type3D = 162;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_FIREBOLT_PURPLE:
         Object->Type3D = 163;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
         // METEORS
      case __SPELL_METEOR_BLACK:
         Object->Type3D = 164;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_METEOR_WHITE:
         Object->Type3D = 165;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_METEOR_PURPLE:
         Object->Type3D = 166;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_METEOR_COBOLT:
         Object->Type3D = 167;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_METEOR_GREEN:
         Object->Type3D = 168;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_METEOR_CANDY:
         Object->Type3D = 169;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_METEOR_GOLD:
         Object->Type3D = 170;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_METEOR_BROWN:
         Object->Type3D = 171;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_METEOR_DARK:
         Object->Type3D = 172;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_METEOR_BLUE:
         Object->Type3D = 173;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_METEOR_VIOLET:
         Object->Type3D = 174;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
         // CALL LIGHTNINGS
      case __SPELL_GREATBOLT_AZURE:
         Object->Type3D = 175;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_GREATBOLT_CRIMSON:
         Object->Type3D = 176;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_GREATBOLT_LIME:
         Object->Type3D = 177;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_GREATBOLT_EMERALD:
         Object->Type3D = 178;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_GREATBOLT_GREEN:
         Object->Type3D = 179;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_GREATBOLT_CANDY:
         Object->Type3D = 180;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_GREATBOLT_RED:
         Object->Type3D = 181;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_GREATBOLT_BLACK:
         Object->Type3D = 182;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;

      case __SPELL_SKILLS_EFFECT:
         Object->Type3D = 185;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;

      case __SPELL_LEVELUP:
         Object->Type3D = 186;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_DECHU_MALE:
         Object->Type3D = 207;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         Object->SummonID = __PLAYER_HUMAN_PUPPET;
         Object->OC = 1;
         break;
      case __SPELL_DECHU_FEMALE:
         Object->Type3D = 207;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         Object->SummonID = __PLAYER_HUMAN_FEMALE;
         Object->OC = 1;
         break;
         
      case __SPELL_DECHU:
         Move2 = false;
         seraphDisplay = false;
         goodSeraph = false;
         Object->Type3D = 207;
         SetMonsterStats(Type, Object);
         //Object->onAttack[0].Play(FALSE);//
         Object->OC = 1;
         MainObject.Direction = 2;
         Direction2 = 2;
         break;
      case __SPELL_NEWFIREBALL2:
         Object->Type3D = 208;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_SWORD01:
         Object->Type3D = 227;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;

      case __SPELL_NEW_LIGHTNING1:
         Object->Type3D = 228;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_NEW_FIRE1:
         Object->Type3D = 229;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_NEW_POISON1:
         Object->Type3D = 230;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
	   case __SPELL_NEW_SUPRAHEAL://BLBL tentative d'ajouter deux nouveaux sorts depuis les DDA
         Object->Type3D = 231;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
	   case	__SPELL_NEW_BOULDERS://BLBL tentative d'ajouter deux nouveaux sorts depuis les DDA
         Object->Type3D = 232;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;


         
         
         // END DIALSOFT NEW SPELLS


		// New added by Dialsoft - END
      //++NMNMNM --[NEW TEST SPELL Link]
      /*
      case __SPELL_A_BOULDER_FIRE:
         Object->Type3D = 210;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_BOULDER_FIREA:
         Object->Type3D = 191;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_NECRO_A01:
         Object->Type3D = 192;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_NECRO_A01A:
         Object->Type3D = 193;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_POISON_A01:
         Object->Type3D = 194;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_POISON_A01A:
         Object->Type3D = 195;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_FIRE_A01:
         Object->Type3D = 196;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_FIRE_A01A:
         Object->Type3D = 197;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_PROTECT_A01:
         Object->Type3D = 198;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_PROTECT_A01A:
         Object->Type3D = 199;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_HEAL_A01:
         Object->Type3D = 200;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_HEAL_A01A:
         Object->Type3D = 201;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_FIRE_A02:
         Object->Type3D = 202;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_A_FIRE_A02A:
         Object->Type3D = 203;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;

      case __SPELL_A_THUNDERSTROM_A:
         Object->Type3D = 230;
         SetMonsterStats(Type, Object);
         Object->onAttack[0].Play(FALSE);
         break;
      */
      
         

            // Drake2
        case __MONSTER_HUMAN_SWORDMAN: case __MONSTER_HUMAN_SWORDMAN_CORPSE:
            Object->Type3D = 11;
            SetMonsterStats(Type, Object);
            break;
            
        case __PLAYER_HUMAN_PUPPET: case __PLAYER_HUMAN_PUPPET_CORPSE:
            Object->Type3D = 71;
            SetMonsterStats(Type, Object);
            break;
            
        case __PLAYER_HUMAN_FEMALE: case __PLAYER_HUMAN_FEMALE_CORPSE:
            Object->Type3D = 71;
            SetMonsterStats(Type, Object);
            break;
            
        case __NPC_HUMAN_GUARD: case __NPC_HUMAN_GUARD_CORPSE:
            Object->Type3D = 14;
            SetMonsterStats(Type, Object);
            break;
            
        case __NPC_HUMAN_WARRIOR: case __NPC_HUMAN_WARRIOR_CORPSE:
            Object->Type3D = 23;
            SetMonsterStats(Type, Object);
            break;
            
        case __NPC_HUMAN_RED_MAGE: case __NPC_HUMAN_RED_MAGE_CORPSE:
            Object->Type3D = 15;
            SetMonsterStats(Type, Object);
            break;
            
        case __NPC_HUMAN_PEASANT: case __NPC_HUMAN_PEASANT_CORPSE:
            Object->Type3D = 13;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_DROMADARY: case __MONSTER_DROMADARY_CORPSE:
            Object->Type3D = 67;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_PIG: case __MONSTER_PIG_CORPSE:
            Object->Type3D = 68;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_ORI: case __MONSTER_ORI_CORPSE:
            Object->Type3D = 69;
            SetMonsterStats(Type, Object);
            break;
            
        case __NPC_HUMAN_PRIEST: case __NPC_HUMAN_PRIEST_CORPSE:
            Object->Type3D = 17;
            SetMonsterStats(Type, Object);
            break;
            
        case __NPC_HUMAN_PAYSANNE: case __NPC_HUMAN_PAYSANNE_CORPSE:
            Object->Type3D = 16;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_HUMAN_MAGE: case __MONSTER_HUMAN_MAGE_CORPSE:
            Object->Type3D = 1;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_HUMAN_PRIEST: case __MONSTER_HUMAN_PRIEST_CORPSE:
            Object->Type3D = 12;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_HUMAN_THIEF: case __MONSTER_HUMAN_THIEF_CORPSE:
            Object->Type3D = 6;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_KOBOLD: case __MONSTER_KOBOLD_CORPSE:
            Object->Type3D = 18;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_OOZE: case __MONSTER_OOZE_CORPSE:
            Object->Type3D = 19;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_ORC: case __MONSTER_ORC_CORPSE:
            Object->Type3D = 20;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_ZOMBIE: case __MONSTER_ZOMBIE_CORPSE:
            Object->Type3D = 21;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_WOLF: case __MONSTER_WOLF_CORPSE:
            Object->Type3D = 77;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_DARKWOLF: case __MONSTER_DARKWOLF_CORPSE:
            Object->Type3D = 86;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_BRIGAND: case __MONSTER_BRIGAND_CORPSE:
            Object->Type3D = 24;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_RED_GOBLIN: case __MONSTER_RED_GOBLIN_CORPSE:
            Object->Type3D = 2;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_DRACONIAN_PLATE: case __MONSTER_DRACONIAN_PLATE_CORPSE:
            Object->Type3D = 117;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_DRACONIAN_ROBE: case __MONSTER_DRACONIAN_ROBE_CORPSE:
            Object->Type3D = 118;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_DRACONIAN_LEATHER: case __MONSTER_DRACONIAN_LEATHER_CORPSE:
            Object->Type3D = 119;
            SetMonsterStats(Type, Object);
            break;
            
            
        case __MONSTER_SKELETON_CENTAUR: case __MONSTER_SKELETON_CENTAUR_CORPSE:
            Object->Type3D = 114;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKELETON_SERVANT_1: case __MONSTER_SKELETON_SERVANT_1_CORPSE:
            Object->Type3D = 105;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKELETON_SERVANT_2: case __MONSTER_SKELETON_SERVANT_2_CORPSE:
            Object->Type3D = 106;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKELETON_KING: case __MONSTER_SKELETON_KING_CORPSE:
            Object->Type3D = 107;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_LICH: case __MONSTER_LICH_CORPSE:
            Object->Type3D = 108;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_CENTAUR_WARRIOR: case __MONSTER_CENTAUR_WARRIOR_CORPSE:
            Object->Type3D = 101;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_CENTAUR_ARCHER: case __MONSTER_CENTAUR_ARCHER_CORPSE:
            Object->Type3D = 102;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_CENTAUR_SHAMAN: case __MONSTER_CENTAUR_SHAMAN_CORPSE:
            Object->Type3D = 103;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_CENTAUR_KING: case __MONSTER_CENTAUR_KING_CORPSE:
            Object->Type3D = 104;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKAVEN_PEON: case __MONSTER_SKAVEN_PEON_CORPSE:
            Object->Type3D = 97;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKAVEN_PEON2: case __MONSTER_SKAVEN_PEON2_CORPSE:
            Object->Type3D = 110;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKAVEN_SHAMAN: case __MONSTER_SKAVEN_SHAMAN_CORPSE:
            Object->Type3D = 98;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKAVEN_SHAMAN2: case __MONSTER_SKAVEN_SHAMAN2_CORPSE:
            Object->Type3D = 111;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKAVEN_SKAVENGER: case __MONSTER_SKAVEN_SKAVENGER_CORPSE:
            Object->Type3D = 99;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKAVEN_SKAVENGER2: case __MONSTER_SKAVEN_SKAVENGER2_CORPSE:
            Object->Type3D = 112;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKAVEN_WARRIOR: case __MONSTER_SKAVEN_WARRIOR_CORPSE:
            Object->Type3D = 100;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKAVEN_WARRIOR2: case __MONSTER_SKAVEN_WARRIOR2_CORPSE:
            Object->Type3D = 113;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_RED_GOBLINBOSS: case __MONSTER_RED_GOBLINBOSS_CORPSE:
            Object->Type3D = 76;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_UNICORN: case __MONSTER_UNICORN_CORPSE:
            Object->Type3D = 29;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_WASP: case __MONSTER_WASP_CORPSE:
            Object->Type3D = 38;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_DRAGON: case __MONSTER_DRAGON_CORPSE:
            Object->Type3D = 37;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SCORPION: case __MONSTER_SCORPION_CORPSE:
            Object->Type3D = 33;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_KRAANIAN: case __MONSTER_KRAANIAN_CORPSE:
            Object->Type3D = 34;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_NIGHTMARE: case __MONSTER_NIGHTMARE_CORPSE:
            Object->Type3D = 36;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_ATROCITY: case __MONSTER_ATROCITY_CORPSE:
            Object->Type3D = 35;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_ATROCITYBOSS: case __MONSTER_ATROCITYBOSS_CORPSE:
            Object->Type3D = 75;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_PEGASUS: case __MONSTER_PEGASUS_CORPSE:
            Object->Type3D = 32;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_HORSE: case __MONSTER_HORSE_CORPSE: 
            Object->Type3D = 31;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_CENTAUR: case __MONSTER_CENTAUR_CORPSE:
            Object->Type3D = 30;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SNAKE: case __MONSTER_SNAKE_CORPSE:
            Object->Type3D = 28;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_TREE_ENT: case __MONSTER_TREE_ENT_CORPSE:
            Object->Type3D = 27;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SMALL_WORM: case __MONSTER_SMALL_WORM_CORPSE:
            Object->Type3D = 25;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_BIG_WORM: case __MONSTER_BIG_WORM_CORPSE:
            Object->Type3D = 26;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_GREEN_TROLL: case __MONSTER_GREEN_TROLL_CORPSE:
            Object->Type3D = 22;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_BEHOLDER: case __MONSTER_BEHOLDER_CORPSE:
            Object->Type3D = 0;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_MUMMY: case __MONSTER_MUMMY_CORPSE:
            Object->Type3D = 3;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_KRAANIANFLYING: case __MONSTER_KRAANIANFLYING_CORPSE:
            Object->Type3D = 65;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_TARANTULA: case __MONSTER_TARANTULA_CORPSE:
            Object->Type3D = 70;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_AGMORKIAN: case __MONSTER_AGMORKIAN_CORPSE:
            Object->Type3D = 72;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_KRAANIANTANK: case __MONSTER_KRAANIANTANK_CORPSE:
            Object->Type3D = 73;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_TAUNTING: case __MONSTER_TAUNTING_CORPSE:
            Object->Type3D = 74;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_KRAANIANMILIPEDE: case __MONSTER_KRAANIANMILIPEDE_CORPSE:
            Object->Type3D = 66;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_DEMON: case __MONSTER_DEMON_CORPSE:
            Object->Type3D = 4;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_MINOTAUR: case __MONSTER_MINOTAUR_CORPSE:
            Object->Type3D = 5;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_RAT: case __MONSTER_RAT_CORPSE:
            Object->Type3D = 7;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_BAT: case __MONSTER_BAT_CORPSE:
            Object->Type3D = 8;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SPIDER: case __MONSTER_SPIDER_CORPSE:
            Object->Type3D = 9;
            SetMonsterStats(Type, Object);
            break;
            
        case __MONSTER_SKELETON: case __MONSTER_SKELETON_CORPSE:
            Object->Type3D = 10;
            SetMonsterStats(Type, Object);
            break;

        case __MONSTER_SANTA_CLAUS:
            Object->Type3D = 116;
            SetMonsterStats(Type, Object);
            break;  
//Dialsoft new code - BEGIN
		case __MONSTER_DARKGOBLIN: case __MONSTER_DARKGOBLIN_CORPSE:
            Object->Type3D = 120;
            SetMonsterStats(Type, Object);
            break; 
		case __MONSTER_BURNTREE: case __MONSTER_BURNTREE_CORPSE:
            Object->Type3D = 123;
            SetMonsterStats(Type, Object);
            break; 
		case __MONSTER_GROUNDMUMMY: case __MONSTER_GROUNDMUMMY_CORPSE:
            Object->Type3D = 124;
            SetMonsterStats(Type, Object);
            break; 
		case __MONSTER_BOSSRAT: case __MONSTER_BOSSRAT_CORPSE:
            Object->Type3D = 125;
            SetMonsterStats(Type, Object);
            break; 
		case __MONSTER_BIGSKEL: case __MONSTER_BIGSKEL_CORPSE:
            Object->Type3D = 126;
            SetMonsterStats(Type, Object);
            break; 
		case __MONSTER_DEMON_VIOLET: case __MONSTER_DEMON_VIOLET_CORPSE:
            Object->Type3D = 128;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_DEMON_BLUE: case __MONSTER_DEMON_BLUE_CORPSE:
            Object->Type3D = 129;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_DEMON_GREEN: case __MONSTER_DEMON_GREEN_CORPSE:
            Object->Type3D = 130;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_DEMON_LIGHTBLUE: case __MONSTER_DEMON_LIGHTBLUE_CORPSE:
            Object->Type3D = 131;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_DEMON_BLACK: case __MONSTER_DEMON_BLACK_CORPSE:
            Object->Type3D = 132;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_DEMON_YELLOW: case __MONSTER_DEMON_YELLOW_CORPSE:
            Object->Type3D = 133;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_DEMON_WHITE: case __MONSTER_DEMON_WHITE_CORPSE:
            Object->Type3D = 134;
            SetMonsterStats(Type, Object);
            break;
        case __MONSTER_FRED_GOBLIN_VIOLET: case __MONSTER_FRED_GOBLIN_VIOLET_CORPSE:
            Object->Type3D = 135;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FRED_GOBLIN_BLUE: case __MONSTER_FRED_GOBLIN_BLUE_CORPSE:
            Object->Type3D = 136;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FRED_GOBLIN_GREEN: case __MONSTER_FRED_GOBLIN_GREEN_CORPSE:
            Object->Type3D = 137;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FRED_GOBLIN_LIGHTBLUE: case __MONSTER_FRED_GOBLIN_LIGHTBLUE_CORPSE:
            Object->Type3D = 138;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FRED_GOBLIN_GRAY: case __MONSTER_FRED_GOBLIN_GRAY_CORPSE:
            Object->Type3D = 139;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FRED_GOBLIN_YELLOW: case __MONSTER_FRED_GOBLIN_YELLOW_CORPSE:
            Object->Type3D = 140;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FRED_GOBLIN_WHITE: case __MONSTER_FRED_GOBLIN_WHITE_CORPSE:
            Object->Type3D = 141;
            SetMonsterStats(Type, Object);
            break;
	    case __MONSTER_FOOZE_DARKGREEN: case __MONSTER_FOOZE_DARKGREEN_CORPSE:
            Object->Type3D = 142;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FOOZE_DARKYELLOW: case __MONSTER_FOOZE_DARKYELLOW_CORPSE:
            Object->Type3D = 143;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FOOZE_GREEN: case __MONSTER_FOOZE_GREEN_CORPSE:
            Object->Type3D = 144;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FOOZE_RED: case __MONSTER_FOOZE_RED_CORPSE:
            Object->Type3D = 145;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FOOZE_BLACK: case __MONSTER_FOOZE_BLACK_CORPSE:
            Object->Type3D = 146;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FOOZE_YELLOW: case __MONSTER_FOOZE_YELLOW_CORPSE:
            Object->Type3D = 147;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FOOZE_WHITE: case __MONSTER_FOOZE_WHITE_CORPSE:
            Object->Type3D = 148;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_WHITECAT: case __MONSTER_WHITECAT_CORPSE:
            Object->Type3D = 149;
            SetMonsterStats(Type, Object);
            break;
  		case __MONSTER_DRAGONDARKGREEN: case __MONSTER_DRAGONDARKGREEN_CORPSE:
            Object->Type3D = 150;
            SetMonsterStats(Type, Object);
            break;
		case __MONSTER_FGAMEMASTER: case __MONSTER_FGAMEMASTER_CORPSE:
            Object->Type3D = 151;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_YETI: case __MONSTER_YETI_CORPSE:
            Object->Type3D = 183;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_SMALL_YETI: case __MONSTER_SMALL_YETI_CORPSE:
            Object->Type3D = 184;
            SetMonsterStats(Type, Object);
            break;

      case __MONSTER_BLOB_R: case __MONSTER_BLOB_R_CORPSE:
            Object->Type3D = 187;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_BLOB_G: case __MONSTER_BLOB_G_CORPSE:
            Object->Type3D = 188;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_TARANTULA2: case __MONSTER_TARANTULA2_CORPSE:
            Object->Type3D = 189;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_DARKGOBLIN_ICE: case __MONSTER_DARKGOBLIN_ICE_CORPSE:
            Object->Type3D = 190;
            SetMonsterStats(Type, Object);
            break; 
      case __MONSTER_KRAANIAN_ICE: case __MONSTER_KRAANIAN_ICE_CORPSE:
            Object->Type3D = 191;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_KRAANIANFLYING_ICE: case __MONSTER_KRAANIANFLYING_ICE_CORPSE:
            Object->Type3D = 192;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_KRAANIANTANK_ICE: case __MONSTER_KRAANIANTANK_ICE_CORPSE:
            Object->Type3D = 193;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_ORC_ICE: case __MONSTER_ORC_ICE_CORPSE:
            Object->Type3D = 194;
            SetMonsterStats(Type, Object);
            break;   
      case __MONSTER_KRAANIAN_POD_ICE: case __MONSTER_KRAANIAN_POD_ICE_CORPSE:
            Object->Type3D = 195;
            SetMonsterStats(Type, Object);
            break; 
      case __MONSTER_SKAVEN_PEON3: case __MONSTER_SKAVEN_PEON3_CORPSE:
            Object->Type3D = 196;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_SKAVEN_SHAMAN3: case __MONSTER_SKAVEN_SHAMAN3_CORPSE:
            Object->Type3D = 197;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_SKAVEN_SKAVENGER3: case __MONSTER_SKAVEN_SKAVENGER3_CORPSE:
            Object->Type3D = 198;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_SKAVEN_WARRIOR3: case __MONSTER_SKAVEN_WARRIOR3_CORPSE:
            Object->Type3D = 199;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_SDRAGONDARKGREEN: case __MONSTER_SDRAGONDARKGREEN_CORPSE:
            Object->Type3D = 200;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_SDRAGONDARKRED: case __MONSTER_SDRAGONDARKRED_CORPSE:
            Object->Type3D = 201;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_SSDRAGONDARKGREEN: case __MONSTER_SSDRAGONDARKGREEN_CORPSE:
            Object->Type3D = 202;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_SSDRAGONDARKRED: case __MONSTER_SSDRAGONDARKRED_CORPSE:
            Object->Type3D = 203;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_SSDRAGONDARKORANGE: case __MONSTER_SSDRAGONDARKORANGE_CORPSE:
            Object->Type3D = 204;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_BIGTARANTULA1: case __MONSTER_BIGTARANTULA1_CORPSE:
            Object->Type3D = 205;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_BIGTARANTULA2: case __MONSTER_BIGTARANTULA2_CORPSE:
            Object->Type3D = 206;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_BIGWOLF1: case __MONSTER_BIGWOLF1_CORPSE:
            Object->Type3D = 209;//77;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_BIGWOLF2: case __MONSTER_BIGWOLF2_CORPSE:
            Object->Type3D = 210;//77;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_BIGBIGSKEL: case __MONSTER_BIGBIGSKEL_CORPSE:
            Object->Type3D = 211;//126;
            SetMonsterStats(Type, Object);
            break; 
      case __MONSTER_BIGBURNTREE: case __MONSTER_BIGBURNTREE_CORPSE:
            Object->Type3D = 212;//123;
            SetMonsterStats(Type, Object);
            break; 
      case __MONSTER_BIGWASP: case __MONSTER_BIGWASP_CORPSE:
            Object->Type3D = 213;//38;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_DEMONSKULL: case __MONSTER_DEMONSKULL_CORPSE:
            Object->Type3D = 214;//141;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_TWIGLEE: case __MONSTER_TWIGLEE_CORPSE:
            Object->Type3D = 215;//141;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_OGRE: case __MONSTER_OGRE_CORPSE:
            Object->Type3D = 216;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_GOLEM1: case __MONSTER_GOLEM1_CORPSE: 
         Object->Type3D = 217;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_GOLEM2: case __MONSTER_GOLEM2_CORPSE: 
         Object->Type3D = 218;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_GOLEM3: case __MONSTER_GOLEM3_CORPSE: 
         Object->Type3D = 219;
            SetMonsterStats(Type, Object);
            break;

      case __MONSTER_DRAGON_RED: case __MONSTER_DRAGON_RED_CORPSE: 
         Object->Type3D = 220;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_DRAGON_ORANGE: case __MONSTER_DRAGON_ORANGE_CORPSE: 
         Object->Type3D = 221;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_DRAGON_JAUNE: case __MONSTER_DRAGON_JAUNE_CORPSE: 
         Object->Type3D = 222;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_DRAGON_BLEU: case __MONSTER_DRAGON_BLEU_CORPSE: 
         Object->Type3D = 223;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_DRAGON_GRIS: case __MONSTER_DRAGON_GRIS_CORPSE: 
         Object->Type3D = 224;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_DRAGON_BLANC: case __MONSTER_DRAGON_BLANC_CORPSE: 
         Object->Type3D = 225;
            SetMonsterStats(Type, Object);
            break;
      case __MONSTER_DRAGON_NOIR: case __MONSTER_DRAGON_NOIR_CORPSE: 
         Object->Type3D = 226;
            SetMonsterStats(Type, Object);
            break;
    

     

		//Dialsoft new code - END
      /*
      //++NMNMNM --[NEW TROWING Weopen Link]
      case __SPELL_TROWING_STAR1:
         Object->Type3D = 200;
         SetMonsterStats(Type, Object);
         //Object->onAttack[0].Play(FALSE);
         break;
      case __SPELL_TROWING_DAGGER1:
         Object->Type3D = 201;
         SetMonsterStats(Type, Object);
         //Object->onAttack[0].Play(FALSE);
         break;
         */
      }
      
    
      
      
      long LightFactor;   
      
      LightFactor = 20-(sqrt(pow(abs(Object->OX), 2)+pow(abs(Object->OY), 2)));
      if (LightFactor < 0)
          LightFactor = 0;
      if (LightFactor > 16)
          LightFactor = 16;
      
      if (Object->Light * LightFactor / 16 > g_TimeStructure.CurrentLight) g_TimeStructure.CurrentLight = Object->Light * LightFactor / 16;
      if (Object->Light * LightFactor / 16 < g_TimeStructure.CurrentLow) g_TimeStructure.CurrentLow = Object->Light * LightFactor / 16;
      
      BOOL IDFound = FALSE;
      TFCObject *Object2 = Object;
      if (AttachID && AttachID != Player.ID) {
          ToHead();
          while (QueryNext()) {
              Object = GetObject();
              if (Object->ID == AttachID) {
                  IDFound = TRUE;
                  Object->Count++;
                  Object2->pOX = &Object->OX;
                  Object2->pOY = &Object->OY;
                  Object2->pMovX = &Object->MovX;
                  Object2->pMovY = &Object->MovY;
                  Object2->pOverlap = &Object->Overlap;
                  SetQueryState(STOP_QUERY);
              }
          }
      } 
      
      bool deletedObject;
      if(IDFound || !AttachID || AttachID == Player.ID) {
          AddToTail(Object2);
          nbUnit++;
          SetGrid(Object2);
          LOG << "Adding Object, param: " << IDFound << ", "  << AttachID << ", " << Player.ID << "\r\n";
          
          deletedObject = false;
      } else if (!IDFound && AttachID && AttachID != Player.yPos) {
          AddToTail(Object2);
          nbUnit++;
          SetGrid(Object2);
          LOG << "Adding Object, param: " << IDFound << ", "  << AttachID << ", " << Player.ID << "\r\n";
          
          deletedObject = false;
      } else {
          LOG << "Delete Object, reason: " << IDFound << ", "  << AttachID << ", " << Player.ID << "\r\n";
          delete Object2;
          
          deletedObject = true;
      }
      
      if ((Object->Type == __PLAYER_HUMAN_PUPPET || Object->Type == __PLAYER_HUMAN_FEMALE) && !Object->KnownPuppet) {
          TFCPacket Send; 
          Send << (short)68;//RQ_QueryPuppetInfo
          Send << (long)Object->ID;
          Send << (short)Object->AbsX;
          Send << (short)Object->AbsY;
          LOG << "INT = 68, " << Object->AbsX << ", " << Object->AbsY << "\r\n";
          SEND_PACKET(Send);
      }
      
      if( !deletedObject ){
          AttackMode(true);
      }
   }
}

void VisualObjectList::SetTotalGrid(void) {
    ToHead();
    
    BOOL NeverDraw = TRUE;
    Grid.Fill(0);
    GridID.Fill(0);
    GridPriority.Fill(INFINITE);
    GridBlocking.Fill(0);
    
    TFCObject *Object;
    while (QueryNext()) {
        Object = GetObject();
        if (Object->OY >= 0 && !(
                                  (Object->OY == 0 || Object->OY == 1 || Object->OY == 2) && 
                                  (Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED  || Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED_I  || Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED  || Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED_I  ||
                                   Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED_I ||
                                   Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED_I ||
                                   Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED_I ||
                                   Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED_I ||
                                   Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED_I ||
                                   Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED_I ||
                                   Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED_I
                                  )
                                ) 
           && NeverDraw) 
        {
            SetGrid(NULL);
            NeverDraw = FALSE;
        }
        
        if (abs(Object->OX) < ViewWidth || abs(Object->OY) < ViewHeight) {
            SetGrid(Object);
            Object->Extended = false;
        }
    }
    if (NeverDraw)
        SetGrid(NULL);
}

/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::MovePlayer(signed char X, signed char Y) {
    // Move all object in the inverse direction of the player.
    long LightFactor;
    BOOL NeverDraw = TRUE;
    
    ToHead();
    
    Grid.Fill(0);
    GridID.Fill(0);
    GridPriority.Fill(INFINITE);
    GridBlocking.Fill(0);
    TFCObject *Object;
    g_TimeStructure.CurrentLight = Player.Light;
    g_TimeStructure.CurrentLow = Player.Light;
    while (QueryNext()) {
        Object = GetObject();
        Object->OX -= X;
        Object->OY -= Y;
        
        if (Object->OY >= 0 && !(
                                 (Object->OY == 0 || Object->OY == 1 || Object->OY == 2) && 
                                 (Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED  || Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED_I  || Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED  || Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED_I  ||
                                  Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED_I ||
                                  Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED_I ||
                                  Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED_I ||
                                  Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED_I ||
                                  Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED_I ||
                                  Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED_I ||
                                  Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED_I
                                  )
                                ) 
                            && NeverDraw) 
        {
            SetGrid(NULL);
            NeverDraw = FALSE;
        }
        
        if (abs(Object->OX) < ViewWidth || abs(Object->OY) < ViewHeight) {
            Object->Extended = false;
            SetGrid(Object);
        }
        
        if ((((abs(Object->OX) > RangeWidth || abs(Object->OY) > RangeHeight) && Object->Type < 30000) && !Object->allowOutOfBound ) && !Object->Count) {
            //			Object->Overlap.Release();
            if (Object->AttachID && Object->AttachID != Player.ID) {
                SavePointer();
                ToHead();
                TFCObject *Object2;
                while(QueryNext()) {
                    Object2 = GetObject();
                    if (Object2->ID == Object->AttachID) {
                        Object2->Count--;
                        SetQueryState(STOP_QUERY);
                    }
                }
                RestorePointer();
            } 
            KillObject(1);
        } else {
            LightFactor = 20-(sqrt(pow(abs(Object->OX), 2)+pow(abs(Object->OY), 2)));
            if (LightFactor < 0)
                LightFactor = 0;
            if (LightFactor > 16)
                LightFactor = 16;
            
            if (Object->Light * LightFactor / 16 > g_TimeStructure.CurrentLight) g_TimeStructure.CurrentLight = Object->Light * LightFactor / 16;
            if (Object->Light * LightFactor / 16 < g_TimeStructure.CurrentLow) g_TimeStructure.CurrentLow = Object->Light * LightFactor / 16;
        }
    } 
    
    int a = 0, b = 0;
    if (World.Done) {
        b = World.MovY;
        a = World.MovX;
    }
    
    for (int i = 0; i < SDObjNb; i++) 
    {
        if (Player.World == SDObjW[i]) 
        {
            if (abs(SDObjX[i] - Player.xPos) == (RangeWidth-1)) 
            {
                if (abs(SDObjY[i] - Player.yPos) < RangeHeight) 
                {
                    Add(SDObjID[i], SDObjType[i], SDObjX[i]-a, SDObjY[i]-b, SDObjLight[i], 0, NULL, 100);
                }
            } 
            else if (abs(SDObjY[i] - Player.yPos) == (RangeHeight-1)) 
            {
                if (abs(SDObjX[i] - Player.xPos) < RangeWidth) 
                {
                    Add(SDObjID[i], SDObjType[i], SDObjX[i]-a, SDObjY[i]-b, SDObjLight[i], 0, NULL, 100);
                }
            }
        }
    }

    for (int i = 0; i < m_animObjNbr; i++) 
    {
        if (Player.World == m_sAnimObj[i].SDObjW) 
        {
            if (abs(m_sAnimObj[i].SDObjX - Player.xPos) == (RangeWidth-1)) 
            {
                if (abs(m_sAnimObj[i].SDObjY - Player.yPos) < RangeHeight) 
                {
                    Add(m_sAnimObj[i].SDObjID, m_sAnimObj[i].SDObjType, m_sAnimObj[i].SDObjX-a, m_sAnimObj[i].SDObjY-b, m_sAnimObj[i].SDObjLight, 0, NULL, 100);
                }
            } 
            else if (abs(m_sAnimObj[i].SDObjY - Player.yPos) == (RangeHeight-1)) 
            {
                if (abs(m_sAnimObj[i].SDObjX - Player.xPos) < RangeWidth) 
                {
                    Add(m_sAnimObj[i].SDObjID, m_sAnimObj[i].SDObjType, m_sAnimObj[i].SDObjX-a, m_sAnimObj[i].SDObjY-b, m_sAnimObj[i].SDObjLight, 0, NULL, 100);
                }
            }
        }
    }
   
    if (MainObject.SpriteNumber > 0) 
    {
        ClearAnimID(99999999);
        INFO *Return = VObject3D[Set]->GetCurrentVSF(MainObject.Direction, MainObject.SpriteNumber-1, MainObject.Standing);
        /**/World.DrawOverlap(ViewWidth, ViewHeight, 0, 0, (ViewWidth*32)+Overlap.xCor-48, (ViewHeight*16)+Overlap.yCor-24+37, &Overlap, 28, Return,99999999);
        MainObject.DrawOverlap = Return->DrawOverlap;
        delete Return;
    }
    
    if (NeverDraw) {
        SetGrid(NULL);
    }
}

void VisualObjectList::UpdateLight(BOOL ForceLight, int FadeLevel) {
    long LightFactor;
    
    ToHead();
    
    g_TimeStructure.CurrentLight = Player.Light;
    g_TimeStructure.CurrentLow = Player.Light;
    
    TFCObject *Object;
    while (QueryNext()) {
        Object = GetObject();
        LightFactor = 20-(sqrt(pow(abs(Object->OX), 2)+pow(abs(Object->OY), 2)));
        if (LightFactor < 0)
            LightFactor = 0;
        if (LightFactor > 16)
            LightFactor = 16;
        
        if (Object->Light * LightFactor / 16 > g_TimeStructure.CurrentLight) g_TimeStructure.CurrentLight = Object->Light * LightFactor / 16;
        if (Object->Light * LightFactor / 16 < g_TimeStructure.CurrentLow) g_TimeStructure.CurrentLow = Object->Light * LightFactor / 16;
    }
}

void VisualObjectList::AddSDObject(void) 
{
    for (int i = 0; i < SDObjNb; i++) 
    {
        if (Player.World == SDObjW[i]) 
        {
            if (abs(SDObjX[i] - Player.xPos) < RangeWidth && abs(SDObjY[i] - Player.yPos) < RangeHeight) 
            {
                Add(SDObjID[i], SDObjType[i], SDObjX[i], SDObjY[i], SDObjLight[i], 0, NULL, 100);
            }
        }
    }
}

void VisualObjectList::AddSDAnimObject(void) 
{
    for (int i = 0; i < m_animObjNbr; i++) 
    {
        if (Player.World == m_sAnimObj[i].SDObjW) 
        {
            if (abs(m_sAnimObj[i].SDObjX - Player.xPos) < RangeWidth && abs(m_sAnimObj[i].SDObjY - Player.yPos) < RangeHeight) 
            {
                Add(m_sAnimObj[i].SDObjID, m_sAnimObj[i].SDObjType, m_sAnimObj[i].SDObjX, m_sAnimObj[i].SDObjY, m_sAnimObj[i].SDObjLight, 0, NULL, 100);
            }
        }
    }
}

BOOL VisualObjectList::CheckDelete(unsigned long ID) {
    ToHead();
    TFCObject *Object;
    
    while (QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
            if (Object->DeleteMe)
                return TRUE;
            else
                return FALSE;
        }
    }
    
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::Delete(unsigned long ID) 
{
    // Delete an object from is ID
    ToHead();
    
    TFCObject *Object;
    while (QueryNext()) 
    {
        Object = GetObject();
        if (Object->ID == ID) 
        {
            if (!Object->Count)//If no items of that ID in the object list : 
            {
                if (Object->AttachID && Object->AttachID != Player.ID) 
                {
                    SavePointer();
                    ToHead();
                    TFCObject *Object2;
                    while(QueryNext()) 
                    {
                        Object2 = GetObject();
                        if (Object2->ID == Object->AttachID) 
                        {
                            Object2->Count--;
                            SetQueryState(STOP_QUERY);
                        }
                    }
                    RestorePointer();
                } 
                KillObject(2);
            } 
            else //else set this object for deletion :
            {
                Object->DeleteMe = TRUE;
            }
            SetQueryState(STOP_QUERY);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::DeleteType(unsigned long ID) 
{
    // Delete an object from is ID
    ToHead();
    
    TFCObject *Object;
    while (QueryNext()) 
    {
        Object = GetObject();
        if (Object->Type == ID) 
        {
            KillObject(8);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::DeleteAll(void) {
    // Delete an object from is ID
    ToHead();
    
    while (QueryNext()) {
        KillObject(3);
    }
    nbUnit = 0;
}

int VisualObjectList::ChangePlayerType(unsigned short Type) 
{
    if(m_bAskForSkinChange)
       return 1; //already new skin progress
    m_bAskForSkinChange  = TRUE;
    m_dwNewSkin          = Type;

    /*
    MainObject.Type = Type;
    SetSet(FALSE);   
    */
    return 1;
}

void VisualObjectList::IsNeedChangeSkin()
{
   if(m_bAskForSkinChange)
   {
      m_bAskForSkinChange = FALSE;
      MainObject.Type = m_dwNewSkin;
      SetSet(FALSE); 
   }
}

/////////////////////////////////////////////////////////////////////////////
int VisualObjectList::ChangeType(unsigned long ID, unsigned short Type) {
    // Change the type of an object.
    ToHead();
    
    TFCObject *Object;
    while (QueryNext()) 
    {
        Object = GetObject();
        if (Object->ID == ID) 
        {
            SetQueryState(STOP_QUERY);
            
            if (Type == 15011 || Type == 15012) 
            {
                Add(SpellID, __SPELL_FLAK, Object->OX+Player.xPos, Object->OY+Player.yPos, 100, 0, ID, 100);
                SpellID--;
                Delete(ID);
                SetTotalGrid();
                return 1;
            } else if ((Type > 15000 && Type < 20000) || (Type > 25000 && Type < 30000)) {
                if ((Type >= 26000 && Type < 30000) || (Type >= 16000 && Type < 20000)) {
                    Type -= 1000;
                }
                Object->Killed = true;
                Object->KillType = Type;
                Object->KillTimer = timeGetTime();
                Object->Chose = 0;
            } 
            else 
            {
                Object->Type = Type;	
            }
            
            
            switch (Type) {
            case __OBJGROUP_WOODEN_DOOR_CLOSED:
            case __OBJGROUP_WOODEN_DOOR2_CLOSED:
            case __OBJGROUP_WOODEN_DOOR3_CLOSED:
            case __OBJGROUP_WOODEN_DOOR4_CLOSED:
            case __OBJGROUP_WOODEN_DOOR5_CLOSED:
            case __OBJGROUP_WOODEN_DOOR6_CLOSED:
            case __OBJGROUP_WOODEN_DOOR7_CLOSED:
            case __OBJGROUP_WOODEN_DOOR8_CLOSED:
                Object->onUse.CopyWave(&SoundFX[1]);
                Object->onUse.Play(FALSE);
                Object->Faces = 8;
                return 9;
                break;	
                
            case __OBJGROUP_WOODEN_DOOR_OPENED:
            case __OBJGROUP_WOODEN_DOOR2_OPENED:
            case __OBJGROUP_WOODEN_DOOR3_OPENED:
            case __OBJGROUP_WOODEN_DOOR4_OPENED:
            case __OBJGROUP_WOODEN_DOOR5_OPENED:
            case __OBJGROUP_WOODEN_DOOR6_OPENED:
            case __OBJGROUP_WOODEN_DOOR7_OPENED:
            case __OBJGROUP_WOODEN_DOOR8_OPENED:
                Object->onUse.CopyWave(&SoundFX[0]);
                Object->onUse.Play(FALSE);
                Object->Faces = 8;
                return 9;
                break;
                
            case __OBJGROUP_WOODEN_DOOR_CLOSED_I:
            case __OBJGROUP_WOODEN_DOOR2_CLOSED_I:
            case __OBJGROUP_WOODEN_DOOR3_CLOSED_I:
            case __OBJGROUP_WOODEN_DOOR4_CLOSED_I:
            case __OBJGROUP_WOODEN_DOOR5_CLOSED_I:
            case __OBJGROUP_WOODEN_DOOR6_CLOSED_I:
            case __OBJGROUP_WOODEN_DOOR7_CLOSED_I:
            case __OBJGROUP_WOODEN_DOOR8_CLOSED_I:
                Object->onUse.CopyWave(&SoundFX[1]);
                Object->onUse.Play(FALSE);
                Object->Faces = 8;
                return 9;
                break;	
                
            case __OBJGROUP_WOODEN_DOOR_OPENED_I:
            case __OBJGROUP_WOODEN_DOOR2_OPENED_I:
            case __OBJGROUP_WOODEN_DOOR3_OPENED_I:
            case __OBJGROUP_WOODEN_DOOR4_OPENED_I:
            case __OBJGROUP_WOODEN_DOOR5_OPENED_I:
            case __OBJGROUP_WOODEN_DOOR6_OPENED_I:
            case __OBJGROUP_WOODEN_DOOR7_OPENED_I:
            case __OBJGROUP_WOODEN_DOOR8_OPENED_I:
                Object->onUse.CopyWave(&SoundFX[0]);
                Object->onUse.Play(FALSE);
                Object->Faces = 8;
                return 9;
                break;
            }
            SetTotalGrid();
            return 1;
        }
    }
    
    return 1;
}

/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::MoveObject(unsigned long ID, signed long NewX, signed long NewY, BOOL DeleteMe, 
                                  unsigned long SummonID, char pHp, unsigned short Type, signed char Light, 
                                  unsigned char Status, bool blowUp) {
    // Move an Object.
    BOOL Found = FALSE;
    Deplacement *Moving = new Deplacement;
    ZeroMemory(Moving, sizeof(Deplacement));
    Moving->ID = ID;
    if (NewX == -1) {
        Moving->Type = 2;
    } else {
        Moving->Type = 0;
        Moving->NewX = NewX;
        Moving->NewY = NewY;
    }
    
    TFCObject *Object;
    ToHead();
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
            if ((Object->Type == __PLAYER_HUMAN_PUPPET || Object->Type == __PLAYER_HUMAN_FEMALE) && !Object->KnownPuppet) {
                TFCPacket Send; 
                Send << (short)68;//RQ_QueryPuppetInfo
                Send << (long)Object->ID;
                Send << (short)Object->AbsX;
                Send << (short)Object->AbsY;
                LOG << "INT = 68, " << Object->AbsX << ", " << Object->AbsY << "\r\n";
                SEND_PACKET(Send);
            }
            if (NewX != Object->AbsX || NewY != Object->AbsY) {
                Found = TRUE;
                Object->MovingQueue.AddToQueue(Moving);
            }
            Object->DeleteMe = DeleteMe;
            Object->SummonID = SummonID;
            Object->Flush = 0;
            if (pHp > 0)
                Object->pHp = pHp;
            //         Object->Dynamic = true;
            //         World.ChangeStatic();
            SetQueryState(STOP_QUERY);
        }
    }
    
    if (!Found && Type <= 30000 && Type) {
        Add(ID, Type, NewX, NewY, Light, Status, NULL, pHp);
    }
    
    if (!Found) {
        if (Moving->Victim)
            delete Moving->Victim;
        if (Moving->Parry)
            delete Moving->Parry;
        delete Moving;
    }
}

extern int DrawLastMoving;

void VisualObjectList::MoveObject2(Deplacement *D, BOOL Fast) {
    TFCObject *Object = NULL;
    int Debug = 0;
    int a, b;
    int Mul;
    int Speed = 1;
    Debug = 1;
    Object = GetObject();
    if (Object->MovingQueue.NbObjects() > 1)
        Speed = 2;
    if (Object->MovingQueue.NbObjects() > 2)
        Speed = 4;
    if (Speed > 1 && D->Type != 0 && (!isMovX && !isMovY) )
        return;
    
    Debug = 2;
    
    if (Object->Type > 30000) 
        Fast = TRUE;
    else
        Fast = FALSE;
    
    Debug = 3;
    if (D->Type == 0 && (Object->OX != (D->NewX-Player.xPos) || Object->OY != (D->NewY-Player.yPos))) {
        Debug = 4;
        Object->Standing = ST_MOVING;
        Object->AbsX = D->NewX;
        Object->AbsY = D->NewY;
        a = b = 0;
        if (World.Done) {
            b = -World.MovY;
            a = -World.MovX;
        }
        Object->MovX   = (signed short)(Object->OX-(D->NewX-Player.xPos))*32;
        Object->MovY   = (signed short)(Object->OY-(D->NewY-Player.yPos))*16;
        Object->fMovX = Object->MovX;
        Object->fMovY = Object->MovY;
        float RatioX = 0, RatioY = 0;
        float fX = abs(Object->MovX);
        float fY = abs(Object->MovY*2);
        Debug = 5;
        if (fX > fY) {
            RatioY = fY/fX;
            RatioX = 1;
        } else if (fY > fX) {
            RatioX = fX/fY;
            RatioY = 1;
        } else {
            if (fX) {
                RatioX = 1;
                RatioY = 1;
            }
        }
        
        if (Fast) {
            //Object->SpeedX = -Object->MovX/30;
            if (abs(Object->MovX))
                Object->SpeedX = -(Object->MovX/abs(Object->MovX))*RatioX*28;
            //Object->SpeedX = (signed char) (Object->OX-(D->NewX-Player.xPos))*-MOVX/Object->Slow;
        } else if (Object->OX-(D->NewX-Player.xPos) > 0) {
            Object->SpeedX = (signed char)(-MOVX/Object->Slow)*Speed;
        } else if (Object->OX-(D->NewX-Player.xPos) < 0) {
            Object->SpeedX = (signed char)(MOVX/Object->Slow)*Speed;
        }
        Object->OX     = (signed char) (D->NewX-Player.xPos);
        if (Fast) {
            //Object->SpeedY = -Object->MovY/30;
            if (abs(Object->MovY))
                Object->SpeedY = -(Object->MovY/abs(Object->MovY))*RatioY*14;
            //Object->SpeedY = (signed char) (Object->OY-(D->NewY-Player.yPos))*-MOVY/Object->Slow;
        }
        else if (Object->OY-(D->NewY-Player.yPos) > 0) Object->SpeedY = (signed char)(-MOVY/Object->Slow)*Speed;
        else if (Object->OY-(D->NewY-Player.yPos) < 0) Object->SpeedY = (signed char) (MOVY/Object->Slow)*Speed;
        Object->OY     = (signed char) (D->NewY-Player.yPos);
        
        //		Object->OX -= a;
        //		Object->OY -= b;
        
        //		if (Object->MovX)
        //			Object->MovX += Object->SpeedX;
        //		if (Object->MovY)
        //			Object->MovY += Object->SpeedY;
        Object->Direction = 1;
        a = (Object->MovX+15);
        b = (Object->MovY+15);
        if (b > 15) {
            if (a > 15) {
                a -= 15;
                b -= 15;
                if (a > b*2)
                    Object->Direction = 4;
                else if (b > a*2)
                    Object->Direction = 8;
                else
                    Object->Direction = 7;
            } else {
                b -= 15;
                a = 15 - a;
                if (a > b*2)
                    Object->Direction = 6;
                else if (b > a*2)
                    Object->Direction = 8;
                else
                    Object->Direction = 9;
            }
        } else {
            if (a > 15) {
                a -= 15;
                b = 15 - b;
                if (a > b*2)
                    Object->Direction = 4;
                else if (b > a*2)
                    Object->Direction = 2;
                else
                    Object->Direction = 1;
            } else {
                a = 15 - a;
                b = 15 - b;
                if (a > b*2)
                    Object->Direction = 6;
                else if (b > a*2)
                    Object->Direction = 2;
                else
                    Object->Direction = 3;
            }			
        }
        
        Debug = 6;
        a = b = 0;
        if (World.Done) {
            if (World.MovY == -1)
                b = 16;
            if (World.MovY == 1)
                b = -16;
            if (World.MovX == -1)
                a = 32;
            if (World.MovX == 1)
                a = -32;
        }
        
        Debug = 7;
        if (Object->Type3D > -1) {
            Debug = 8;
            if (VObject3D[Object->Type3D]) 
            {
                ClearAnimID(Object->ID);
                INFO *Return = VObject3D[Object->Type3D]->GetCurrentVSF(Object->Direction, 0, Object->Standing);
                /**/World.DrawOverlap(Object->OX+ViewWidth, Object->OY+ViewHeight, 0, 0, ((Object->OX+ViewWidth)<<5)+Object->Overlap.xCor-48+a, ((Object->OY+ViewHeight)<<4)+Object->Overlap.yCor+13+b, &Object->Overlap, 30, Return,Object->ID);
                Object->DrawOverlap = Return->DrawOverlap;
                delete Return;
            }
            Debug = 9;
        } else {
           
            Debug = 10;
            INFO Return;
            Return.Width = GetVSFObject(Object->Type)->GetWidth();
            Return.Height = GetVSFObject(Object->Type)->GetHeight();
            Return.CX = GetVSFObject(Object->Type)->GetCX();
            Return.CY = GetVSFObject(Object->Type)->GetCY();
            ClearAnimID(Object->ID);
            /**/World.DrawOverlap(Object->OX+ViewWidth, Object->OY+ViewHeight, 0, 0, ((Object->OX+ViewWidth)<<5)+Object->Overlap.xCor-48+a, ((Object->OY+ViewHeight)<<4)+Object->Overlap.yCor+13+b, &Object->Overlap, 30, &Return,Object->ID);
            Object->DrawOverlap = Return.DrawOverlap;
            Debug = 11;
        }
        
        Debug = 12;
        Mul = Object->MovX/32;
        if (Object->MovY/16 > Mul) Mul = Object->MovY/16;
        Debug = 13;
   }
   if (D->Type == 1) {
       //		Anim(D->ID, D->AnimType);
       Player.LastAttack = timeGetTime();
       Object->Standing = ST_ATTACK;
       if (D->Direction)
           Object->Direction = D->Direction;
       
       Object->Chose = 1; // Type
       Object->AttFrame = 0;
       Object->AttSpeed = 0;
       if (IsRangedObject(Object)) {
           Object->onAttack[3].Play(FALSE);
       } else {
           int RandomSound = (rand() % 3);
           Object->onAttack[RandomSound].Play(FALSE);
       }
       if (!D->Miss && D->Victim) {
           FXType *FXObject = new FXType;
           FXObject->Debug = FALSE;
           FXObject->Time = timeGetTime();
           FXObject->Sound.CopyWave(D->Victim);
           delete D->Victim;
           D->Victim = NULL;
           SoundFXList.Lock("SFX 1");
           SoundFXList.AddToTail(FXObject);
           SoundFXList.Unlock("SFX 1");
       } else if (D->Parry && !(rand() % 10)) {
           FXType *FXObject = new FXType;
           FXObject->Debug = FALSE;
           FXObject->Time = timeGetTime();
           FXObject->Sound.CopyWave(D->Parry);
           delete D->Parry;
           D->Parry = NULL;
           SoundFXList.Lock("SFX 2");
           SoundFXList.AddToTail(FXObject);
           SoundFXList.Unlock("SFX 2");
       }
   }
   
   if (D->Type == 4) {
       Object->Standing = ST_STANDING;
       Object->Direction = D->Direction;
   }
   if (D->Type == 3) {
       Object->Standing = ST_ATTACK;
       Object->Direction = D->Direction;
       Object->Chose = 1; // Type
       Object->AttFrame = 0;
       Object->AttSpeed = 0;
       if (IsRangedObject(Object)) {
           Object->onAttack[3].Play(FALSE);
       } else {
           int RandomSound = (rand() % 3);
           Object->onAttack[RandomSound].Play(FALSE);
       }
   }
   if (D->Type == 2) {
       Object->Standing = ST_STANDING;
       MainObject.Direction = 1;
       int a = (Object->OX+11)*3;
       int b = (Object->OY+16)*2;
       if (b > 30) {
           if (a > 30) {
               a -= 30;
               b -= 30;
               if (a > b*2) {
                   MainObject.Direction = 6;
                   Object->Direction = 4;
               } else if (b > a*2) {
                   MainObject.Direction = 2;
                   Object->Direction = 8;
               } else {
                   MainObject.Direction = 3;
                   Object->Direction = 7;	
               }
           } else {
               b -= 30;
               a = 30 - a;
               if (a > b*2) {
                   MainObject.Direction = 4;
                   Object->Direction = 6;
               } else if (b > a*2) {
                   MainObject.Direction = 2;
                   Object->Direction = 8;
               } else {
                   MainObject.Direction = 1;
                   Object->Direction = 9;
               }
           }
       } else {
           if (a > 30) {
               a -= 30;
               b = 30 - b;
               if (a > b*2) {
                   MainObject.Direction = 6;
                   Object->Direction = 4;
               } else if (b > a*2) {
                   MainObject.Direction = 8;
                   Object->Direction = 2;
               } else {
                   MainObject.Direction = 9;
                   Object->Direction = 1;
               }
           } else {
               a = 30 - a;
               b = 30 - b;
               if (a > b*2) {
                   MainObject.Direction = 4;
                   Object->Direction = 6;
               } else if (b > a*2) {
                   MainObject.Direction = 8;
                   Object->Direction = 2;
               } else {
                   MainObject.Direction = 7;
                   Object->Direction = 3;
               }
           }			
       }
   }
}

void VisualObjectList::Sort(void) 
{
   if (!nbUnit) 
      return;


   TemplateObject<class TFCObject> **ptr = 
      new TemplateObject<class TFCObject> * [nbUnit];
   signed char *value = new signed char [nbUnit];
   signed char *value2 = new signed char [nbUnit];
   DWORD *value3 = new DWORD [nbUnit];
   signed char *spec  = new signed char [nbUnit];
   TemplateObject<class TFCObject> *TempPtr;
   signed  TempValue;
   DWORD TempValue2;
   BOOL Change = FALSE;
   int i = 0, j = 0;	
   bool foundSeraph = false;
   bool childFound = false;
   
   TFCObject *Object;
   
   ToHead();
   while(QueryNext()) 
   {
      Object = GetObject();

      childFound = false;
      
      if (Object->spellChildID) 
      {
         SavePointer();
         if (FoundChild(Object->spellChildID)) 
         {
            childFound = true;
         }
         RestorePointer();
      } 
      
      
      if (Object->Type == __SPELL_SERAPH_MALE || Object->Type == __SPELL_SERAPH_FEMALE) 
      {
         if (Object->SummonID && Object->SpriteNumber >= 25) 
         {
            SavePointer();
            Add(Object->NewID, Object->SummonID, Object->AbsX, Object->AbsY, Object->Light, VOL_ISPLAYER, Object->AttachID, 100, 1);
            RestorePointer();
            Object->SummonID = NULL;
            return;
         }
      }
      
      if (Object->Type == __SPELL_EVIL_SERAPH_MALE || Object->Type == __SPELL_EVIL_SERAPH_FEMALE) {
         if (Object->SummonID && Object->SpriteNumber >= 25) {
            SavePointer();
            Add(Object->NewID, Object->SummonID, Object->AbsX, Object->AbsY, Object->Light, VOL_ISPLAYER, Object->AttachID, 100, 2);
            RestorePointer();
            Object->SummonID = NULL;
            return;
         }
      }

      if (Object->Type == __SPELL_DECHU_MALE || Object->Type == __SPELL_DECHU_FEMALE) 
      {
         if (Object->SummonID && Object->SpriteNumber >= 90) 
         {
            SavePointer();
            Add(Object->NewID, Object->SummonID, Object->AbsX, Object->AbsY, Object->Light, VOL_ISPLAYER, Object->AttachID, 100, 1);
            RestorePointer();
            Object->SummonID = NULL;
            return;
         }
      }

      if (Object->Type == __SPELL_SERAPH || Object->Type == __SPELL_EVIL_SERAPH || Object->Type == __SPELL_DECHU) 
      {
         foundSeraph = true;
      }
      
      if ((Object->Type == __SPELL_SERAPH || Object->Type == __SPELL_EVIL_SERAPH) && Object->SpriteNumber >= 89) 
      {
         Move2 = true;
      }
      if (Object->Type == __SPELL_DECHU  && Object->SpriteNumber >= 105) 
      {
         Move2 = true;
      }
      
      if (Object->Type == __SPELL_SERAPH && Object->SpriteNumber >= 25) 
      {
         seraphDisplay = true;
      }
      
      if (Object->Type == __SPELL_EVIL_SERAPH && Object->SpriteNumber >= 78) 
      {
         seraphDisplay = true;
      }

      if (Object->Type == __SPELL_DECHU && Object->SpriteNumber >= 90) 
      {
         seraphDisplay = true;
      }

      //char strTmp[100];
      //sprintf(strTmp,"X[%d > %d]  Y[%d > %d]  Type == %d  Out OK [%d]\n",abs(Object->OX),RangeWidth,abs(Object->OY),RangeHeight,Object->Type,Object->allowOutOfBound);
      //OutputDebugString(strTmp);
      if ( //out of Bound
            ((( abs(Object->OX) > RangeWidth || abs(Object->OY) > RangeHeight ) && Object->Type < 30000 )&& !Object->allowOutOfBound )
         || 
           // If object is to be deleted.
            ( Object->DeleteMe && !isMovX && !isMovY && !Object->MovingQueue.NbObjects() ) && !Object->Count && !childFound) 
      {
         j++;
         //			Object->Overlap.Release();
         if (Object->Light == g_TimeStructure.CurrentLight || Object->Light == g_TimeStructure.CurrentLow) 
         {
            Object->Light = 0;
            //          Change = TRUE;
            if (Object->SummonID) 
            {
               SavePointer();
               Add(SpellID, Object->SummonID, Object->AbsX, Object->AbsY, Object->Light, 0, Object->AttachID, 100, Object->Direction);
               RestorePointer();
               SpellID--;
            }
            if (Object->AttachID && Object->AttachID != Player.ID) 
            {
               SavePointer();
               ToHead();
               TFCObject *Object2;
               while(QueryNext()) 
               {
                  Object2 = GetObject();
                  if (Object2->ID == Object->AttachID) 
                  { 
                     Object2->Count--;
                     SetQueryState(STOP_QUERY);
                  }
               }
               RestorePointer();
            }
            LOG << "* KILL 4\r\n";
            KillObject(4);
            ToHead();
            i = 0;
         } 
         else 
         {
            if (Object->SummonID) 
            {
               SavePointer();
               Add(SpellID, Object->SummonID, Object->AbsX, Object->AbsY, Object->Light, 0, Object->AttachID, 100, Object->Direction);
               RestorePointer();
               SpellID--;
            }
            if (Object->AttachID && Object->AttachID != Player.ID) {
               SavePointer();
               ToHead();
               TFCObject *Object2;
               while(QueryNext()) 
               {
                  Object2 = GetObject();
                  if (Object2->ID == Object->AttachID) 
                  {
                     Object2->Count--;
                     SetQueryState(STOP_QUERY);
                  }
               }
               RestorePointer();
            } 
            LOG << "* KILL 5\r\n";
            KillObject(5);
            ToHead();
            i = 0;
         }
      }
      /*
      // Patch pour detruire les ghost de fleche d<archer...
      else if(Object->Type == 30027 && Object->DeleteMe ==0)
      {
         KillObject(6);
         ToHead();
         i = 0;
      }
      */
      else 
      { 
         ptr[i] = Current;
         value[i] = Object->OY+Object->OC;
         spec[i] = Object->OC;
         value2[i] = Object->OX;
         if (Object->AttachID)
            value3[i] = Object->AttachID;
         else
            value3[i] = 1;
         i++;
      } 
   }
   
   if (!foundSeraph) 
   {
      Move2 = true;
      seraphDisplay = true;
   }
   
   bool Sorting;//BLBLBL correction de tri à bulle.

   for (i = 0; i < nbUnit; i++) 
   {
      Sorting=false;//BLBLBL correction de tri à bulle
      for (j = i+1; j < nbUnit; j++) 
      {
         if (value[j] < value[i]) 
         {
            Sorting=true;//BLBLBL correction de tri à bulle
            TempValue = value[i];
            value[i] = value[j];
            value[j] = TempValue;
            TempValue = value2[i];
            value2[i] = value2[j];
            value2[j] = TempValue;
            TempValue2 = value3[i];
            value3[i] = value3[j];
            value3[j] = TempValue2;
            TempValue = spec[i];
            spec[i] = spec[j];
            spec[j] = TempValue;
            TempPtr = ptr[i];
            ptr[i] = ptr[j];
            ptr[j] = TempPtr;
         }
      }
	  if (Sorting==false)//BLBLBL correction de tri à bulle
			break;

   }
   
   for (i = 0; i < nbUnit; i++) 
   {
	  Sorting=false;//BLBLBL correction de tri à bulle
      for (j = i+1; j < nbUnit; j++) 
      {
         if (value[j] == value[i]) 
         {
            if (spec[j] < spec[i]) 
            {
			   Sorting=true;//BLBLBL correction de tri à bulle
               TempValue = value[i];
               value[i] = value[j];
               value[j] = TempValue;
               TempValue = value2[i];
               value2[i] = value2[j];
               value2[j] = TempValue;
               TempValue2 = value3[i];
               value3[i] = value3[j];
               value3[j] = TempValue2;
               TempValue = spec[i];
               spec[i] = spec[j];
               spec[j] = TempValue;
               TempPtr = ptr[i];
               ptr[i] = ptr[j];
               ptr[j] = TempPtr;
            }
         }
      }
	  if (Sorting == false)//BLBLBL correction de tri à bulle
		  break;
   }
   
   for (i = 0; i < nbUnit; i++) 
   {
	  Sorting=false;//BLBLBL correction de tri à bulle
      for (j = i+1; j < nbUnit; j++) 
      {
         if (value[j] == value[i]) 
         {
            if (value2[j] < value2[i]) 
            {
			   Sorting=true;//BLBLBL correction de tri à bulle
               TempValue = value[i];
               value[i] = value[j];
               value[j] = TempValue;
               TempValue = value2[i];
               value2[i] = value2[j];
               value2[j] = TempValue;
               TempValue2 = value3[i];
               value3[i] = value3[j];
               value3[j] = TempValue2;
               TempValue = spec[i];
               spec[i] = spec[j];
               spec[j] = TempValue;
               TempPtr = ptr[i];
               ptr[i] = ptr[j];
               ptr[j] = TempPtr;
            }
         }
      }
	  if (Sorting==false)//BLBLBL correction de tri à bulle
		  break;
   }
   
   for (i = 0; i < nbUnit; i++) 
   {
	  Sorting=false;
      for (j = i+1; j < nbUnit; j++) 
      {
         if (value[j] == value[i]) 
         {
            if (value3[j] < value3[i]) 
            {
			   Sorting=true;
               TempValue = value[i];
               value[i] = value[j];
               value[j] = TempValue;
               TempValue = value2[i];
               value2[i] = value2[j];
               value2[j] = TempValue;
               TempValue2 = value3[i];
               value3[i] = value3[j];
               value3[j] = TempValue2;
               TempValue = spec[i];
               spec[i] = spec[j];
               spec[j] = TempValue;
               TempPtr = ptr[i];
               ptr[i] = ptr[j];
               ptr[j] = TempPtr;
            }
         }
      }
	  if(Sorting==false)//BLBLBL correction de tri à bulle
		  break;
   }
   
   if (nbUnit) 
   {
      Head = Current = ptr[0];
      Tail = ptr[nbUnit-1];
      ptr[0]->Lesser = NULL;
      if (nbUnit == 1) 
      {
         ptr[0]->Greater = NULL;
      } 
      else 
      {
         ptr[0]->Greater = ptr[1];
      }
      
      ptr[nbUnit-1]->Greater = NULL;
      if (nbUnit == 1) 
      {
         ptr[nbUnit-1]->Lesser = NULL;
      } 
      else 
      {
         ptr[nbUnit-1]->Lesser = ptr[nbUnit-2];
      }
      
      for (i = 1; i < nbUnit-1; i++) 
      {
         ptr[i]->Lesser = ptr[i-1];
         ptr[i]->Greater = ptr[i+1];
      }
   } 
   else 
   {
      Head = Tail = Current = NULL;
   }
   
   if (Change) 
   {
      UpdateLight();
   }
   
   delete ptr;
   delete value;
   delete value2;
   delete value3;
   delete spec;
}

int DOOR = 0;

/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::DrawOverlap(int XCor, int YCor, bool Draw, LPDIRECTDRAWSURFACE lpBlitSurface, bool Flag, LPRECT Clip) 
{
    // Draw all the object to Screen.
}

void VisualObjectList::DrawObjectSelect(int XCor, int YCor, bool Draw, LPDIRECTDRAWSURFACE lpBlitSurface, bool Flag, bool bDrawText, bool bNewInt) 
{
   if(!g_SaveGame.GetOptionsParam()->bShowNewLife)
      return; // ancienne methode on return tout simplement

   // Draw all the object to Screen.
   Lock(6668);
   TFCObject *Object = NULL;
   bool NeverDraw = true;
   LPRECT Clip = new RECT;
   LPV2SPRITEFX VsfFX;
   bool childFound = false;
   
   XCor -= 48;
   YCor += - 24 + 37;
   if (!bDrawText) 
   {
      if (!Flag) 
      {
         Clip->left = 0;
         Clip->right = g_Global.GetScreenW()+64;  // 704 Avant with * 2 tiles de 32 de large
         Clip->top = 0;
         Clip->bottom = g_Global.GetScreenH()+32; // 512 Avant height * 2 tiles de 16 de haut;
      } 
      else 
      {
         Clip->left = 0;
         Clip->right = g_Global.GetScreenW();
         Clip->top = 0;
         Clip->bottom = g_Global.GetScreenH();
      }
      
      Grid.Fill(0);		
      GridID.Fill(0);
      GridPriority.Fill(INFINITE);
      GridBlocking.Fill(0);
      CreateComponent(VsfFX, V2SPRITEFX);
      
      for (int MultiPass = 0; MultiPass < 2; MultiPass++) 
      {
         ToHead();
         while(QueryNext()) 
         {
            Object = GetObject();
            childFound = false;
            if (!((Object->Type == __PLAYER_HUMAN_PUPPET || Object->Type == __PLAYER_HUMAN_FEMALE) && !Object->KnownPuppet)) 
            {
               
               if (MultiPass == 1) 
               {
                  if (Object->OY >= 0 && !((Object->OY == 0 || Object->OY == 1 || Object->OY == 2) && 
                                           (Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED  || Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED_I  || Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED  || Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED_I  ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED_I
                                           )) && NeverDraw && seraphDisplay) 
                  {
                     NeverDraw = false;
                     if (Player.Dead) 
                     {
                     } 
                     else 
                     {
                        if (Object3DLoad[Set]) 
                        {
                           if (MainObject.Chose == 1) 
                           {
                              if (MainObject.Chose == 1 && Flag == true && !bNewInt) 
                              {
                                 BOOL boOutline = FALSE;
                                 DWORD OutColor=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                                 DWORD dwCalcPV=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                                 
                                 if (TargetID == Player.ID) 
                                 {
                                    boOutline = TRUE;
                                    OutColor = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 100 / Player.MaxHp));
                                    dwCalcPV = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 50 / Player.MaxHp));
                                    if(dwCalcPV > 50)
                                       dwCalcPV = 50;
                                 }
                                 if(boOutline)
                                 {
                                    VsfFX->lpClipRect = Clip;
                                    VsfFX->lpDirectDrawSurface = lpBlitSurface;
                                    VsfFX->dwFX = NULL;
                                    
                                    m_v2SpriteLifeS[dwCalcPV].DrawSpriteNSemiTrans((ViewWidth*32)-48, (ViewHeight*16)-24+37, VsfFX,TRUE,128);
                                    boOutline = FALSE;
                                 }
                              }
                           } 
                           if (MainObject.Chose != 1 && Flag == true) 
                           {
                              BOOL boOutline = FALSE;
                              DWORD OutColor=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                              DWORD dwCalcPV=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                              
                              if (TargetID == Player.ID) 
                              {
                                 boOutline = TRUE;
                                 OutColor = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 100 / Player.MaxHp));
                                 dwCalcPV = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 50 / Player.MaxHp));
                                 if(dwCalcPV > 50)
                                    dwCalcPV = 50;
                              }
                              if (!bNewInt) 
                              {
                                 if(boOutline)
                                 {
                                    VsfFX->lpClipRect = Clip;
                                    VsfFX->lpDirectDrawSurface = lpBlitSurface;
                                    VsfFX->dwFX = NULL;
                                    
                                    m_v2SpriteLifeS[dwCalcPV].DrawSpriteNSemiTrans((ViewWidth*32)-48, (ViewHeight*16)-24+37, VsfFX,TRUE,128);
                                    boOutline = FALSE;
                                 }
                              }
                           }
                        }
                     }
                     SetGrid(NULL);
                  }
               }
               
               if (Object->Dynamic == Flag) 
               {
                  BOOL boOutline = FALSE;
                  DWORD OutColor=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                  if (Object->ID == TargetID || Object->ID == FreezeID) 
                  {
                     boOutline = TRUE;
                     OutColor = Object->pHp;
                  }
                  
                  if (Object->Chose == 1 && MultiPass == 1 && !bNewInt && !childFound) 
                  {
                     switch (Object->Type) 
                     {
                        case __PLAYER_HUMAN_PUPPET:
                        case __PLAYER_HUMAN_FEMALE:
                        case __NPC_HUMAN_GUARD:
                        case __NPC_HUMAN_WARRIOR:
                        case __MONSTER_HUMAN_SWORDMAN:
                        case __MONSTER_HUMAN_MAGE:
                        case __MONSTER_HUMAN_PRIEST:
                        case __MONSTER_HUMAN_THIEF:
                        case __MONSTER_KOBOLD:
                        case __MONSTER_OOZE:
                        case __MONSTER_ORC:
                        case __MONSTER_ORC_ICE:
                        case __MONSTER_KRAANIAN_POD_ICE:
                        case __MONSTER_ZOMBIE:
                        case __MONSTER_WOLF:
                        case __MONSTER_BIGWOLF1:
                        case __MONSTER_BIGWOLF2:
                        case __MONSTER_DARKWOLF:
                        case __MONSTER_RED_GOBLIN:
                        case __MONSTER_DRACONIAN_PLATE:
                        case __MONSTER_DRACONIAN_ROBE:
                        case __MONSTER_DRACONIAN_LEATHER:
                        case __MONSTER_SKELETON_CENTAUR:
                        case __MONSTER_SKELETON_SERVANT_1:
                        case __MONSTER_SKELETON_SERVANT_2:
                        case __MONSTER_SKELETON_KING:
                        case __MONSTER_OGRE:
                        case __MONSTER_GOLEM1:
                        case __MONSTER_GOLEM2:
                        case __MONSTER_GOLEM3:
                        case __MONSTER_LICH:
                        case __MONSTER_CENTAUR_WARRIOR:
                        case __MONSTER_CENTAUR_ARCHER:
                        case __MONSTER_CENTAUR_SHAMAN:
                        case __MONSTER_CENTAUR_KING:
                        case __MONSTER_SKAVEN_PEON:
                        case __MONSTER_SKAVEN_SHAMAN:
                        case __MONSTER_SKAVEN_SKAVENGER:
                        case __MONSTER_SKAVEN_WARRIOR:
                        case __MONSTER_SKAVEN_PEON2:
                        case __MONSTER_SKAVEN_SHAMAN2:
                        case __MONSTER_SKAVEN_SKAVENGER2:
                        case __MONSTER_SKAVEN_WARRIOR2:
                        case __MONSTER_SKAVEN_PEON3:
                        case __MONSTER_SKAVEN_SHAMAN3:
                        case __MONSTER_SKAVEN_SKAVENGER3:
                        case __MONSTER_SKAVEN_WARRIOR3:
                        case __MONSTER_RED_GOBLINBOSS:
                        case __MONSTER_UNICORN:
                        case __MONSTER_DRAGON:
                        case __MONSTER_SCORPION:
                        case __MONSTER_KRAANIAN:
                        case __MONSTER_KRAANIAN_ICE:
                        case __MONSTER_NIGHTMARE:
                        case __MONSTER_ATROCITY:
                        case __MONSTER_ATROCITYBOSS:
                        case __MONSTER_PEGASUS:
                        case __MONSTER_HORSE:
                        case __MONSTER_CENTAUR:
                        case __MONSTER_SNAKE:
                        case __MONSTER_TREE_ENT:
                        case __MONSTER_SMALL_WORM:
                        case __MONSTER_BIG_WORM:
                        case __MONSTER_BRIGAND:
                        case __MONSTER_GREEN_TROLL:
                        case __MONSTER_BEHOLDER:
                        case __MONSTER_MUMMY:
                        case __MONSTER_TARANTULA:
                        case __MONSTER_TARANTULA2:
                        case __MONSTER_AGMORKIAN:
                        case __MONSTER_KRAANIANTANK:
                        case __MONSTER_KRAANIANTANK_ICE:
                        case __MONSTER_TAUNTING:
                        case __MONSTER_KRAANIANMILIPEDE:
                        case __MONSTER_DEMON:
                        case __MONSTER_MINOTAUR:
                        case __MONSTER_RAT:
                        case __MONSTER_SPIDER:
                        case __MONSTER_SKELETON:
                        case __MONSTER_SANTA_CLAUS:
                        
                           //Dialsoft new code - BEGIN
                        case __MONSTER_DARKGOBLIN:
                        case __MONSTER_DARKGOBLIN_ICE:
                        case __MONSTER_BURNTREE:
                        case __MONSTER_GROUNDMUMMY:
                        case __MONSTER_BOSSRAT:
                        case __MONSTER_BIGSKEL:
                        case __MONSTER_BIGBIGSKEL:
                        case __MONSTER_DEMON_VIOLET:
                        case __MONSTER_DEMON_BLUE:
                        case __MONSTER_DEMON_GREEN:
                        case __MONSTER_DEMON_LIGHTBLUE:
                        case __MONSTER_DEMON_BLACK:
                        case __MONSTER_DEMON_YELLOW:
                        case __MONSTER_DEMON_WHITE:
                        case __MONSTER_FRED_GOBLIN_VIOLET:
                        case __MONSTER_FRED_GOBLIN_BLUE:
                        case __MONSTER_FRED_GOBLIN_GREEN:
                        case __MONSTER_FRED_GOBLIN_LIGHTBLUE:
                        case __MONSTER_FRED_GOBLIN_GRAY:
                        case __MONSTER_FRED_GOBLIN_YELLOW:
                        case __MONSTER_FRED_GOBLIN_WHITE:
                        case __MONSTER_FOOZE_DARKGREEN:
                        case __MONSTER_FOOZE_DARKYELLOW:
                        case __MONSTER_FOOZE_GREEN:
                        case __MONSTER_FOOZE_RED:
                        case __MONSTER_FOOZE_BLACK:
                        case __MONSTER_FOOZE_YELLOW:
                        case __MONSTER_FOOZE_WHITE:
                        case __MONSTER_WHITECAT:
                        case __MONSTER_SDRAGONDARKGREEN:
                        case __MONSTER_SDRAGONDARKRED:
                        case __MONSTER_SSDRAGONDARKGREEN:
                        case __MONSTER_SSDRAGONDARKRED:
                        case __MONSTER_SSDRAGONDARKORANGE:
                        case __MONSTER_FGAMEMASTER:
                        case __MONSTER_YETI:
                        case __MONSTER_SMALL_YETI:
                        case __MONSTER_BLOB_R:
                        case __MONSTER_BLOB_G:
                        case __MONSTER_DEMONSKULL:
                        case __MONSTER_TWIGLEE:
                        //Dialsoft new code -END
                        {
                           if(boOutline)
                           {
                              DWORD dwCalcPV = OutColor *50/100;
                              if(dwCalcPV > 50)
                                 dwCalcPV = 50;

                              VsfFX->lpClipRect = Clip;
                              VsfFX->lpDirectDrawSurface = lpBlitSurface;
                              VsfFX->dwFX = NULL;
                              
                              m_v2SpriteLifeS[dwCalcPV].DrawSpriteNSemiTrans(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX,TRUE,128);
                              boOutline = FALSE;
                           }
                        } break;

                     case __MONSTER_BIGBURNTREE:
                     case __MONSTER_DRAGONDARKGREEN:
                     case __MONSTER_DRAGON_RED:
                     case __MONSTER_DRAGON_ORANGE:
                     case __MONSTER_DRAGON_JAUNE:
                     case __MONSTER_DRAGON_BLEU:
                     case __MONSTER_DRAGON_GRIS:
                     case __MONSTER_DRAGON_BLANC:
                     case __MONSTER_DRAGON_NOIR:
                     case __MONSTER_BIGTARANTULA1:
                     case __MONSTER_BIGTARANTULA2:
                     {
                           if(boOutline)
                           {
                              DWORD dwCalcPV = OutColor *50/100;
                              if(dwCalcPV > 50)
                                 dwCalcPV = 50;

                              VsfFX->lpClipRect = Clip;
                              VsfFX->lpDirectDrawSurface = lpBlitSurface;
                              VsfFX->dwFX = NULL;
                              
                              m_v2SpriteLifeSB[dwCalcPV].DrawSpriteNSemiTrans(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX,TRUE,128);
                              boOutline = FALSE;
                           }
                     } 
                     break;
                        
                     case __MONSTER_BAT:
                     case __MONSTER_KRAANIANFLYING:
                     case __MONSTER_KRAANIANFLYING_ICE:
                     case __MONSTER_WASP:
                     case __MONSTER_BIGWASP:
                        if(boOutline)
                        {
                           DWORD dwCalcPV = OutColor *50/100;
                           if(dwCalcPV > 50)
                              dwCalcPV = 50;
                           
                           VsfFX->lpClipRect = Clip;
                           VsfFX->lpDirectDrawSurface = lpBlitSurface;
                           VsfFX->dwFX = NULL;
                           
                           m_v2SpriteLifeS[dwCalcPV].DrawSpriteNSemiTrans(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX,TRUE,128);
                           boOutline = FALSE;
                        }
                        break;
                      } 
                  } 
                  else if (!bNewInt && !childFound) 
                  {
                     VsfFX->lpClipRect = Clip;
                     VsfFX->lpDirectDrawSurface = lpBlitSurface;
                     if (boOutline) 
                     {
                        VsfFX->dwFX = FX_OUTLINE;
                        VsfFX->dwOutlineColor = RGB(255, 255, 255);
                     } 
                     else
                        VsfFX->dwFX = NULL;
                     
                     {
                        switch (Object->Type) 
                        {
                           case __MONSTER_HUMAN_MAGE:
                           case __MONSTER_HUMAN_PRIEST:
                           case __MONSTER_HUMAN_THIEF:
                           case __MONSTER_KOBOLD:
                           case __MONSTER_OOZE:
                           case __MONSTER_ORC:
                           case __MONSTER_ORC_ICE:
                           case __MONSTER_KRAANIAN_POD_ICE:
                           case __MONSTER_ZOMBIE:
                           case __MONSTER_WOLF:
                           case __MONSTER_BIGWOLF1:
                           case __MONSTER_BIGWOLF2:
                           case __MONSTER_DARKWOLF:
                           case __MONSTER_RED_GOBLIN:
                           case __MONSTER_DRACONIAN_PLATE:
                           case __MONSTER_DRACONIAN_ROBE:
                           case __MONSTER_DRACONIAN_LEATHER:
                           case __MONSTER_SKELETON_CENTAUR:
                           case __MONSTER_SKELETON_SERVANT_1:
                           case __MONSTER_SKELETON_SERVANT_2:
                           case __MONSTER_SKELETON_KING:
                           case __MONSTER_OGRE:
                           case __MONSTER_GOLEM1:
                           case __MONSTER_GOLEM2:
                           case __MONSTER_GOLEM3:
                           case __MONSTER_LICH:
                           case __MONSTER_CENTAUR_WARRIOR:
                           case __MONSTER_CENTAUR_ARCHER:
                           case __MONSTER_CENTAUR_SHAMAN:
                           case __MONSTER_CENTAUR_KING:
                           case __MONSTER_SKAVEN_PEON:
                           case __MONSTER_SKAVEN_SHAMAN:
                           case __MONSTER_SKAVEN_SKAVENGER:
                           case __MONSTER_SKAVEN_WARRIOR:
                           case __MONSTER_SKAVEN_PEON2:
                           case __MONSTER_SKAVEN_SHAMAN2:
                           case __MONSTER_SKAVEN_SKAVENGER2:
                           case __MONSTER_SKAVEN_WARRIOR2:
                           case __MONSTER_SKAVEN_PEON3:
                           case __MONSTER_SKAVEN_SHAMAN3:
                           case __MONSTER_SKAVEN_SKAVENGER3:
                           case __MONSTER_SKAVEN_WARRIOR3:
                           case __MONSTER_RED_GOBLINBOSS:
                           case __MONSTER_UNICORN:
                           case __MONSTER_DRAGON:
                           case __NPC_HUMAN_PEASANT:
                           case __MONSTER_DROMADARY:
                           case __MONSTER_PIG:
                           case __MONSTER_ORI:
                           case __NPC_HUMAN_PRIEST:
                           case __NPC_HUMAN_PAYSANNE:
                           case __NPC_HUMAN_RED_MAGE:
                           case __NPC_HUMAN_GUARD:
                           case __NPC_HUMAN_WARRIOR:
                           case __MONSTER_HUMAN_SWORDMAN:
                           case __PLAYER_HUMAN_PUPPET:
                           case __PLAYER_HUMAN_FEMALE:
                           case __MONSTER_SCORPION:
                           case __MONSTER_KRAANIAN:
                           case __MONSTER_KRAANIAN_ICE:
                           case __MONSTER_NIGHTMARE:
                           case __MONSTER_ATROCITY:
                           case __MONSTER_ATROCITYBOSS:
                           case __MONSTER_PEGASUS:
                           case __MONSTER_HORSE:
                           case __MONSTER_CENTAUR:
                           case __MONSTER_SNAKE:
                           case __MONSTER_TREE_ENT:
                           case __MONSTER_SMALL_WORM:
                           case __MONSTER_BIG_WORM:
                           case __MONSTER_SPIDER:
                           case __MONSTER_SKELETON:
                           case __MONSTER_SANTA_CLAUS:
                           case __MONSTER_BRIGAND:
                           case __MONSTER_GREEN_TROLL:
                           case __MONSTER_BEHOLDER:
                           case __MONSTER_MUMMY:
                           case __MONSTER_TARANTULA:
                           case __MONSTER_TARANTULA2:
                           case __MONSTER_AGMORKIAN:
                           case __MONSTER_KRAANIANTANK:
                           case __MONSTER_KRAANIANTANK_ICE:
                           case __MONSTER_TAUNTING:
                           case __MONSTER_KRAANIANMILIPEDE:
                           case __MONSTER_DEMON:
                           case __MONSTER_MINOTAUR:
                           case __MONSTER_RAT:
                              //Dialsoft new code - BEGIN
                           case __MONSTER_DARKGOBLIN:
                           case __MONSTER_DARKGOBLIN_ICE:
                           case __MONSTER_BURNTREE:
                           case __MONSTER_GROUNDMUMMY:
                           case __MONSTER_BOSSRAT:
                           case __MONSTER_BIGSKEL: 
                           case __MONSTER_BIGBIGSKEL:
                           case __MONSTER_DEMON_VIOLET:
                           case __MONSTER_DEMON_BLUE:
                           case __MONSTER_DEMON_GREEN:
                           case __MONSTER_DEMON_LIGHTBLUE:
                           case __MONSTER_DEMON_BLACK:
                           case __MONSTER_DEMON_YELLOW:
                           case __MONSTER_DEMON_WHITE:
                           case __MONSTER_FRED_GOBLIN_VIOLET:
                           case __MONSTER_FRED_GOBLIN_BLUE:
                           case __MONSTER_FRED_GOBLIN_GREEN:
                           case __MONSTER_FRED_GOBLIN_LIGHTBLUE:
                           case __MONSTER_FRED_GOBLIN_GRAY:
                           case __MONSTER_FRED_GOBLIN_YELLOW:
                           case __MONSTER_FRED_GOBLIN_WHITE:
                           case __MONSTER_FOOZE_DARKGREEN:
                           case __MONSTER_FOOZE_DARKYELLOW:
                           case __MONSTER_FOOZE_GREEN:
                           case __MONSTER_FOOZE_RED:
                           case __MONSTER_FOOZE_BLACK:
                           case __MONSTER_FOOZE_YELLOW:
                           case __MONSTER_FOOZE_WHITE:
                           case __MONSTER_WHITECAT:
                           case __MONSTER_SDRAGONDARKGREEN:
                           case __MONSTER_SDRAGONDARKRED:
                           case __MONSTER_SSDRAGONDARKGREEN:
                           case __MONSTER_SSDRAGONDARKRED:
                           case __MONSTER_SSDRAGONDARKORANGE:
                           case __MONSTER_FGAMEMASTER:
                           case __MONSTER_YETI:
                           case __MONSTER_SMALL_YETI:
                           case __MONSTER_BLOB_R:
                           case __MONSTER_BLOB_G:
                           case __MONSTER_DEMONSKULL:
                           case __MONSTER_TWIGLEE:
                              if (MultiPass == 1) 
                              {
                                 if(boOutline)
                                 {
                                    DWORD dwCalcPV = OutColor *50/100;
                                    if(dwCalcPV > 50)
                                       dwCalcPV = 50;
                                    
                                    VsfFX->lpClipRect = Clip;
                                    VsfFX->lpDirectDrawSurface = lpBlitSurface;
                                    VsfFX->dwFX = NULL;
                                    
                                    m_v2SpriteLifeS[dwCalcPV].DrawSpriteNSemiTrans(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX,TRUE,128);
                                    boOutline = FALSE;
                                 }
                              }
                           break;
                           case __MONSTER_BIGBURNTREE:
                           case __MONSTER_DRAGONDARKGREEN:
                           case __MONSTER_DRAGON_RED:
                           case __MONSTER_DRAGON_ORANGE:
                           case __MONSTER_DRAGON_JAUNE:
                           case __MONSTER_DRAGON_BLEU:
                           case __MONSTER_DRAGON_GRIS:
                           case __MONSTER_DRAGON_BLANC:
                           case __MONSTER_DRAGON_NOIR:
                           case __MONSTER_BIGTARANTULA1:
                           case __MONSTER_BIGTARANTULA2:
                              if (MultiPass == 1) 
                              {
                                 if(boOutline)
                                 {
                                    DWORD dwCalcPV = OutColor *50/100;
                                    if(dwCalcPV > 50)
                                       dwCalcPV = 50;
                                    
                                    VsfFX->lpClipRect = Clip;
                                    VsfFX->lpDirectDrawSurface = lpBlitSurface;
                                    VsfFX->dwFX = NULL;
                                    
                                    m_v2SpriteLifeSB[dwCalcPV].DrawSpriteNSemiTrans(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX,TRUE,128);
                                    boOutline = FALSE;
                                 }
                              }
                           break;

                           case __MONSTER_BAT:
                           case __MONSTER_KRAANIANFLYING:
                           case __MONSTER_KRAANIANFLYING_ICE:
                           case __MONSTER_WASP:
                           case __MONSTER_BIGWASP:
                              if (MultiPass == 1) 
                              {
                                 if(boOutline)
                                 {
                                    DWORD dwCalcPV = OutColor *50/100;
                                    if(dwCalcPV > 50)
                                       dwCalcPV = 50;
                                    
                                    VsfFX->lpClipRect = Clip;
                                    VsfFX->lpDirectDrawSurface = lpBlitSurface;
                                    VsfFX->dwFX = NULL;
                                    
                                    m_v2SpriteLifeS[dwCalcPV].DrawSpriteNSemiTrans(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX,TRUE,128);
                                    boOutline = FALSE;
                                 }
                              }
                           break;
                        }
                     }
                  }
               } 
               SetGrid(Object);
            }
 
         }
      }
   
      
      if (NeverDraw && seraphDisplay) 
      {
         NeverDraw = false;
         if (Player.Dead) 
         {
         } 
         else 
         {
            if (Object3DLoad[Set]) 
            {
               if (MainObject.Chose == 1) 
               {
                  if (MainObject.Chose == 1 && Flag == true && !bNewInt) 
                  {
                     BOOL boOutline = FALSE;
                     DWORD OutColor=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                     DWORD dwCalcPV=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                  
                     if (TargetID == Player.ID) 
                     {
                        boOutline = TRUE;
                        OutColor = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 100 / Player.MaxHp));
                        dwCalcPV = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 50 / Player.MaxHp));
                        if(dwCalcPV > 50)
                           dwCalcPV = 50;

                        if(boOutline)
                        {
                           VsfFX->lpClipRect = Clip;
                           VsfFX->lpDirectDrawSurface = lpBlitSurface;
                           VsfFX->dwFX = NULL;
                           
                           m_v2SpriteLifeS[dwCalcPV].DrawSpriteNSemiTrans((ViewWidth*32)-48, (ViewHeight*16)-24+37, VsfFX,TRUE,128);
                           boOutline = FALSE;
                        }
                     }

                  }
               } 
               else	
               {
                  if (MainObject.Chose != 1 && Flag == true) 
                  {
                     if (!bNewInt) 
                     {
                        BOOL boOutline = FALSE;
                        DWORD OutColor=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                        DWORD dwCalcPV=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                        
                        if (TargetID == Player.ID) 
                        {
                           boOutline = TRUE;
                           OutColor = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 100 / Player.MaxHp));
                           dwCalcPV = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 50 / Player.MaxHp));
                           if(dwCalcPV > 50)
                               dwCalcPV = 50;
                           if(boOutline)
                           {
                              VsfFX->lpClipRect = Clip;
                              VsfFX->lpDirectDrawSurface = lpBlitSurface;
                              VsfFX->dwFX = NULL;
                           
                              m_v2SpriteLifeS[dwCalcPV].DrawSpriteNSemiTrans((ViewWidth*32)-48, (ViewHeight*16)-24+37, VsfFX,TRUE,128);
                              boOutline = FALSE;
                           }
                        }
                        
                     }
                  }
               }
            }
         }
         SetGrid(NULL);
      } 
      DeleteComponent(VsfFX); // GELAGE 1
   }
   
   delete Clip;
   Unlock(6668);
}


void VisualObjectList::DrawObjectShadow(int XCor, int YCor, bool Draw, LPDIRECTDRAWSURFACE lpBlitSurface, bool Flag, bool bDrawText, bool bNewInt) 
{
   if(!g_SaveGame.GetOptionsParam()->bShowNewOmbrage)
      return; // ancienne methode on return tout simplement

   int dwNiveauTrans = 160;
   // Draw all the object to Screen.
   Lock(6669);
   TFCObject *Object = NULL;
   bool NeverDraw = true;
   LPRECT Clip = new RECT;
   LPV2SPRITEFX VsfFX;
   bool childFound = false;
   
   XCor -= 48;
   YCor += - 24 + 37;
   if (!bDrawText) 
   {
      if (!Flag) 
      {
         Clip->left = 0;
         Clip->right = g_Global.GetScreenW()+64;  // 704 Avant with * 2 tiles de 32 de large
         Clip->top = 0;
         Clip->bottom = g_Global.GetScreenH()+32; // 512 Avant height * 2 tiles de 16 de haut;
      } 
      else 
      {
         Clip->left = 0;
         Clip->right = g_Global.GetScreenW();
         Clip->top = 0;
         Clip->bottom = g_Global.GetScreenH();
      }
      
      Grid.Fill(0);		
      GridID.Fill(0);
      GridPriority.Fill(INFINITE);
      GridBlocking.Fill(0);
      CreateComponent(VsfFX, V2SPRITEFX);
      
      for (int MultiPass = 0; MultiPass < 2; MultiPass++) 
      {
         ToHead();
         while(QueryNext()) 
         {
            Object = GetObject();
            childFound = false;
            if (!((Object->Type == __PLAYER_HUMAN_PUPPET || Object->Type == __PLAYER_HUMAN_FEMALE) && !Object->KnownPuppet)) 
            {
               
               if (MultiPass == 1) 
               {
                  if (Object->OY >= 0 && !((Object->OY == 0 || Object->OY == 1 || Object->OY == 2) && 
                                           (Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED  || Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED_I  || Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED  || Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED_I  ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED_I
                                           )) && NeverDraw && seraphDisplay) 
                  {
                     NeverDraw = false;
                     if (Player.Dead) 
                     {
                     } 
                     else 
                     {
                        if (Object3DLoad[Set]) 
                        {
                           if (MainObject.Chose == 1) 
                           {
                              if (MainObject.Chose == 1 && Flag == true && !bNewInt) 
                              {
                                    VsfFX->lpClipRect = Clip;
                                    VsfFX->lpDirectDrawSurface = lpBlitSurface;
                                    VsfFX->dwFX = NULL;

                                    m_v2SpritePShadow.DrawSpriteNSemiTrans((ViewWidth*32)-48, (ViewHeight*16)-24+37, VsfFX,TRUE,dwNiveauTrans);
                              }
                           } 
                           if (MainObject.Chose != 1 && Flag == true) 
                           {
                              if (!bNewInt) 
                              {

                                 VsfFX->lpClipRect = Clip;
                                 VsfFX->lpDirectDrawSurface = lpBlitSurface;
                                 VsfFX->dwFX = NULL;
                                 m_v2SpritePShadow.DrawSpriteNSemiTrans((ViewWidth*32)-48, (ViewHeight*16)-24+37, VsfFX,TRUE,dwNiveauTrans);
                              }
                           }
                        }
                     }
                     SetGrid(NULL);
                  }
               }
               
               if (Object->Dynamic == Flag) 
               {
                  if (Object->Chose == 1 && MultiPass == 1 && !bNewInt && !childFound) 
                  {
                     switch (Object->Type) 
                     {
                        case __PLAYER_HUMAN_PUPPET:
                        case __PLAYER_HUMAN_FEMALE:
                        case __NPC_HUMAN_GUARD:
                        case __NPC_HUMAN_WARRIOR:
                        case __MONSTER_HUMAN_SWORDMAN:
                        case __MONSTER_HUMAN_MAGE:
                        case __MONSTER_HUMAN_PRIEST:
                        case __MONSTER_HUMAN_THIEF:
                        case __MONSTER_KOBOLD:
                        case __MONSTER_OOZE:
                        case __MONSTER_ORC:
                        case __MONSTER_ORC_ICE:
                        case __MONSTER_KRAANIAN_POD_ICE:
                        case __MONSTER_ZOMBIE:
                        case __MONSTER_WOLF:
                        case __MONSTER_BIGWOLF1:
                        case __MONSTER_BIGWOLF2:
                        case __MONSTER_DARKWOLF:
                        case __MONSTER_RED_GOBLIN:
                        case __MONSTER_DRACONIAN_PLATE:
                        case __MONSTER_DRACONIAN_ROBE:
                        case __MONSTER_DRACONIAN_LEATHER:
                        case __MONSTER_SKELETON_CENTAUR:
                        case __MONSTER_SKELETON_SERVANT_1:
                        case __MONSTER_SKELETON_SERVANT_2:
                        case __MONSTER_SKELETON_KING:
                        case __MONSTER_OGRE:
                        case __MONSTER_GOLEM1:
                        case __MONSTER_GOLEM2:
                        case __MONSTER_GOLEM3:
                        case __MONSTER_LICH:
                        case __MONSTER_CENTAUR_WARRIOR:
                        case __MONSTER_CENTAUR_ARCHER:
                        case __MONSTER_CENTAUR_SHAMAN:
                        case __MONSTER_CENTAUR_KING:
                        case __MONSTER_SKAVEN_PEON:
                        case __MONSTER_SKAVEN_SHAMAN:
                        case __MONSTER_SKAVEN_SKAVENGER:
                        case __MONSTER_SKAVEN_WARRIOR:
                        case __MONSTER_SKAVEN_PEON2:
                        case __MONSTER_SKAVEN_SHAMAN2:
                        case __MONSTER_SKAVEN_SKAVENGER2:
                        case __MONSTER_SKAVEN_WARRIOR2:
                        case __MONSTER_SKAVEN_PEON3:
                        case __MONSTER_SKAVEN_SHAMAN3:
                        case __MONSTER_SKAVEN_SKAVENGER3:
                        case __MONSTER_SKAVEN_WARRIOR3:
                        case __MONSTER_RED_GOBLINBOSS:
                        case __MONSTER_UNICORN:
                        case __MONSTER_DRAGON:
                        case __MONSTER_SCORPION:
                        case __MONSTER_KRAANIAN:
                        case __MONSTER_KRAANIAN_ICE:
                        case __MONSTER_NIGHTMARE:
                        case __MONSTER_ATROCITY:
                        case __MONSTER_ATROCITYBOSS:
                        case __MONSTER_PEGASUS:
                        case __MONSTER_HORSE:
                        case __MONSTER_CENTAUR:
                        case __MONSTER_SNAKE:
                        case __MONSTER_TREE_ENT:
                        case __MONSTER_SMALL_WORM:
                        case __MONSTER_BIG_WORM:
                        case __MONSTER_BRIGAND:
                        case __MONSTER_GREEN_TROLL:
                        case __MONSTER_BEHOLDER:
                        case __MONSTER_MUMMY:
                        case __MONSTER_TARANTULA:
                        case __MONSTER_TARANTULA2:
                        case __MONSTER_BIGTARANTULA1:
                        case __MONSTER_BIGTARANTULA2:
                        case __MONSTER_AGMORKIAN:
                        case __MONSTER_KRAANIANTANK:
                        case __MONSTER_KRAANIANTANK_ICE:
                        case __MONSTER_TAUNTING:
                        case __MONSTER_KRAANIANMILIPEDE:
                        case __MONSTER_DEMON:
                        case __MONSTER_MINOTAUR:
                        case __MONSTER_RAT:
                        case __MONSTER_SPIDER:
                        case __MONSTER_SKELETON:
                        case __MONSTER_SANTA_CLAUS:
                        
                           //Dialsoft new code - BEGIN
                        case __MONSTER_DARKGOBLIN:
                        case __MONSTER_DARKGOBLIN_ICE:
                        case __MONSTER_BURNTREE:
                        case __MONSTER_BIGBURNTREE:
                        case __MONSTER_GROUNDMUMMY:
                        case __MONSTER_BOSSRAT:
                        case __MONSTER_BIGSKEL:
                        case __MONSTER_BIGBIGSKEL:
                        case __MONSTER_DEMON_VIOLET:
                        case __MONSTER_DEMON_BLUE:
                        case __MONSTER_DEMON_GREEN:
                        case __MONSTER_DEMON_LIGHTBLUE:
                        case __MONSTER_DEMON_BLACK:
                        case __MONSTER_DEMON_YELLOW:
                        case __MONSTER_DEMON_WHITE:
                        case __MONSTER_FRED_GOBLIN_VIOLET:
                        case __MONSTER_FRED_GOBLIN_BLUE:
                        case __MONSTER_FRED_GOBLIN_GREEN:
                        case __MONSTER_FRED_GOBLIN_LIGHTBLUE:
                        case __MONSTER_FRED_GOBLIN_GRAY:
                        case __MONSTER_FRED_GOBLIN_YELLOW:
                        case __MONSTER_FRED_GOBLIN_WHITE:
                        case __MONSTER_FOOZE_DARKGREEN:
                        case __MONSTER_FOOZE_DARKYELLOW:
                        case __MONSTER_FOOZE_GREEN:
                        case __MONSTER_FOOZE_RED:
                        case __MONSTER_FOOZE_BLACK:
                        case __MONSTER_FOOZE_YELLOW:
                        case __MONSTER_FOOZE_WHITE:
                        case __MONSTER_WHITECAT:
      					   case __MONSTER_DRAGONDARKGREEN:
                        case __MONSTER_DRAGON_RED:
                        case __MONSTER_DRAGON_ORANGE:
                        case __MONSTER_DRAGON_JAUNE:
                        case __MONSTER_DRAGON_BLEU:
                        case __MONSTER_DRAGON_GRIS:
                        case __MONSTER_DRAGON_BLANC:
                        case __MONSTER_DRAGON_NOIR:
                        case __MONSTER_SDRAGONDARKGREEN:
                        case __MONSTER_SDRAGONDARKRED:
                        case __MONSTER_SSDRAGONDARKGREEN:
                        case __MONSTER_SSDRAGONDARKRED:
                        case __MONSTER_SSDRAGONDARKORANGE:
                        case __MONSTER_FGAMEMASTER:
                        case __MONSTER_YETI:
                        case __MONSTER_SMALL_YETI:
                        case __MONSTER_BLOB_R:
                        case __MONSTER_BLOB_G:
                        case __MONSTER_DEMONSKULL:
                        case __MONSTER_TWIGLEE:
                        //Dialsoft new code -END
                        {
                           VsfFX->lpClipRect = Clip;
                           VsfFX->lpDirectDrawSurface = lpBlitSurface;
                           VsfFX->dwFX = NULL;
                           m_v2SpritePShadow.DrawSpriteNSemiTrans(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX,TRUE,dwNiveauTrans);
                              
                        } 
                        break;
 
                     case __MONSTER_BAT:
                     case __MONSTER_KRAANIANFLYING:
                     case __MONSTER_KRAANIANFLYING_ICE:
                     case __MONSTER_WASP:
                     case __MONSTER_BIGWASP:
                       
                        VsfFX->lpClipRect = Clip;
                        VsfFX->lpDirectDrawSurface = lpBlitSurface;
                        VsfFX->dwFX = NULL;
                        m_v2SpritePShadow.DrawSpriteNSemiTrans(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX,TRUE,dwNiveauTrans);
                        break;
                      } 
                  } 
                  else if (!bNewInt && !childFound) 
                  {
                        switch (Object->Type) 
                        {
                           case __MONSTER_HUMAN_MAGE:
                           case __MONSTER_HUMAN_PRIEST:
                           case __MONSTER_HUMAN_THIEF:
                           case __MONSTER_KOBOLD:
                           case __MONSTER_OOZE:
                           case __MONSTER_ORC:
                           case __MONSTER_ORC_ICE:
                           case __MONSTER_KRAANIAN_POD_ICE:
                           case __MONSTER_ZOMBIE:
                           case __MONSTER_WOLF:
                           case __MONSTER_BIGWOLF1:
                           case __MONSTER_BIGWOLF2:
                           case __MONSTER_DARKWOLF:
                           case __MONSTER_RED_GOBLIN:
                           case __MONSTER_DRACONIAN_PLATE:
                           case __MONSTER_DRACONIAN_ROBE:
                           case __MONSTER_DRACONIAN_LEATHER:
                           case __MONSTER_SKELETON_CENTAUR:
                           case __MONSTER_SKELETON_SERVANT_1:
                           case __MONSTER_SKELETON_SERVANT_2:
                           case __MONSTER_SKELETON_KING:
                           case __MONSTER_OGRE:
                           case __MONSTER_GOLEM1:
                           case __MONSTER_GOLEM2:
                           case __MONSTER_GOLEM3:
                           case __MONSTER_LICH:
                           case __MONSTER_CENTAUR_WARRIOR:
                           case __MONSTER_CENTAUR_ARCHER:
                           case __MONSTER_CENTAUR_SHAMAN:
                           case __MONSTER_CENTAUR_KING:
                           case __MONSTER_SKAVEN_PEON:
                           case __MONSTER_SKAVEN_SHAMAN:
                           case __MONSTER_SKAVEN_SKAVENGER:
                           case __MONSTER_SKAVEN_WARRIOR:
                           case __MONSTER_SKAVEN_PEON2:
                           case __MONSTER_SKAVEN_SHAMAN2:
                           case __MONSTER_SKAVEN_SKAVENGER2:
                           case __MONSTER_SKAVEN_WARRIOR2:
                           case __MONSTER_SKAVEN_PEON3:
                           case __MONSTER_SKAVEN_SHAMAN3:
                           case __MONSTER_SKAVEN_SKAVENGER3:
                           case __MONSTER_SKAVEN_WARRIOR3:
                           case __MONSTER_RED_GOBLINBOSS:
                           case __MONSTER_UNICORN:
                           case __MONSTER_DRAGON:
                           case __NPC_HUMAN_PEASANT:
                           case __MONSTER_DROMADARY:
                           case __MONSTER_PIG:
                           case __MONSTER_ORI:
                           case __NPC_HUMAN_PRIEST:
                           case __NPC_HUMAN_PAYSANNE:
                           case __NPC_HUMAN_RED_MAGE:
                           case __NPC_HUMAN_GUARD:
                           case __NPC_HUMAN_WARRIOR:
                           case __MONSTER_HUMAN_SWORDMAN:
                           case __PLAYER_HUMAN_PUPPET:
                           case __PLAYER_HUMAN_FEMALE:
                           case __MONSTER_SCORPION:
                           case __MONSTER_KRAANIAN:
                           case __MONSTER_KRAANIAN_ICE:
                           case __MONSTER_NIGHTMARE:
                           case __MONSTER_ATROCITY:
                           case __MONSTER_ATROCITYBOSS:
                           case __MONSTER_PEGASUS:
                           case __MONSTER_HORSE:
                           case __MONSTER_CENTAUR:
                           case __MONSTER_SNAKE:
                           case __MONSTER_TREE_ENT:
                           case __MONSTER_SMALL_WORM:
                           case __MONSTER_BIG_WORM:
                           case __MONSTER_SPIDER:
                           case __MONSTER_SKELETON:
                           case __MONSTER_SANTA_CLAUS:
                           case __MONSTER_BRIGAND:
                           case __MONSTER_GREEN_TROLL:
                           case __MONSTER_BEHOLDER:
                           case __MONSTER_MUMMY:
                           case __MONSTER_TARANTULA:
                           case __MONSTER_TARANTULA2:
                           case __MONSTER_BIGTARANTULA1:
                           case __MONSTER_BIGTARANTULA2:
                           case __MONSTER_AGMORKIAN:
                           case __MONSTER_KRAANIANTANK:
                           case __MONSTER_KRAANIANTANK_ICE:
                           case __MONSTER_TAUNTING:
                           case __MONSTER_KRAANIANMILIPEDE:
                           case __MONSTER_DEMON:
                           case __MONSTER_MINOTAUR:
                           case __MONSTER_RAT:
                              //Dialsoft new code - BEGIN
                           case __MONSTER_DARKGOBLIN:
                           case __MONSTER_DARKGOBLIN_ICE:
                           case __MONSTER_BURNTREE:
                           case __MONSTER_BIGBURNTREE:
                           case __MONSTER_GROUNDMUMMY:
                           case __MONSTER_BOSSRAT:
                           case __MONSTER_BIGSKEL:
                           case __MONSTER_BIGBIGSKEL:
                           case __MONSTER_DEMON_VIOLET:
                           case __MONSTER_DEMON_BLUE:
                           case __MONSTER_DEMON_GREEN:
                           case __MONSTER_DEMON_LIGHTBLUE:
                           case __MONSTER_DEMON_BLACK:
                           case __MONSTER_DEMON_YELLOW:
                           case __MONSTER_DEMON_WHITE:
                           case __MONSTER_FRED_GOBLIN_VIOLET:
                           case __MONSTER_FRED_GOBLIN_BLUE:
                           case __MONSTER_FRED_GOBLIN_GREEN:
                           case __MONSTER_FRED_GOBLIN_LIGHTBLUE:
                           case __MONSTER_FRED_GOBLIN_GRAY:
                           case __MONSTER_FRED_GOBLIN_YELLOW:
                           case __MONSTER_FRED_GOBLIN_WHITE:
                           case __MONSTER_FOOZE_DARKGREEN:
                           case __MONSTER_FOOZE_DARKYELLOW:
                           case __MONSTER_FOOZE_GREEN:
                           case __MONSTER_FOOZE_RED:
                           case __MONSTER_FOOZE_BLACK:
                           case __MONSTER_FOOZE_YELLOW:
                           case __MONSTER_FOOZE_WHITE:
                           case __MONSTER_WHITECAT:
      					      case __MONSTER_DRAGONDARKGREEN:
                           case __MONSTER_DRAGON_RED:
                           case __MONSTER_DRAGON_ORANGE:
                           case __MONSTER_DRAGON_JAUNE:
                           case __MONSTER_DRAGON_BLEU:
                           case __MONSTER_DRAGON_GRIS:
                           case __MONSTER_DRAGON_BLANC:
                           case __MONSTER_DRAGON_NOIR:
                           case __MONSTER_SDRAGONDARKGREEN:
                           case __MONSTER_SDRAGONDARKRED:
                           case __MONSTER_SSDRAGONDARKGREEN:
                           case __MONSTER_SSDRAGONDARKRED:
                           case __MONSTER_SSDRAGONDARKORANGE:
                           case __MONSTER_FGAMEMASTER:
                           case __MONSTER_YETI:
                           case __MONSTER_SMALL_YETI:
                           case __MONSTER_BLOB_R:
                           case __MONSTER_BLOB_G:
                           case __MONSTER_DEMONSKULL:
                           case __MONSTER_TWIGLEE:
                              if (MultiPass == 1) 
                              {
                                    VsfFX->lpClipRect = Clip;
                                    VsfFX->lpDirectDrawSurface = lpBlitSurface;
                                    VsfFX->dwFX = NULL;

                                    m_v2SpritePShadow.DrawSpriteNSemiTrans(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX,TRUE,dwNiveauTrans);
                              }
                           break;
                           case __MONSTER_BAT:
                           case __MONSTER_KRAANIANFLYING:
                           case __MONSTER_KRAANIANFLYING_ICE:
                           case __MONSTER_WASP:
                           case __MONSTER_BIGWASP:
                              if (MultiPass == 1) 
                              {
                                    VsfFX->lpClipRect = Clip;
                                    VsfFX->lpDirectDrawSurface = lpBlitSurface;
                                    VsfFX->dwFX = NULL;
                                    m_v2SpritePShadow.DrawSpriteNSemiTrans(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX,TRUE,dwNiveauTrans);
                              }
                           break;
                        }
                  }
               } 
               SetGrid(Object);
            }
 
         }
      }
   
      
      if (NeverDraw && seraphDisplay) 
      {
         NeverDraw = false;
         if (Player.Dead) 
         {
         } 
         else 
         {
            if (Object3DLoad[Set]) 
            {
               if (MainObject.Chose == 1) 
               {
                  if (MainObject.Chose == 1 && Flag == true && !bNewInt) 
                  {

                     VsfFX->lpClipRect = Clip;
                     VsfFX->lpDirectDrawSurface = lpBlitSurface;
                     VsfFX->dwFX = NULL;

                     m_v2SpritePShadow.DrawSpriteNSemiTrans((ViewWidth*32)-48, (ViewHeight*16)-24+37, VsfFX,TRUE,dwNiveauTrans);
                  }
               } 
               else	
               {
                  if (MainObject.Chose != 1 && Flag == true) 
                  {
                     if (!bNewInt) 
                     {

                        VsfFX->lpClipRect = Clip;
                        VsfFX->lpDirectDrawSurface = lpBlitSurface;
                        VsfFX->dwFX = NULL;
                        m_v2SpritePShadow.DrawSpriteNSemiTrans((ViewWidth*32)-48, (ViewHeight*16)-24+37, VsfFX,TRUE,dwNiveauTrans);
                     }
                  }
               }
            }
         }
         SetGrid(NULL);
      } 
      DeleteComponent(VsfFX); // GELAGE 1
   }
   
   delete Clip;
   Unlock(6669);
}


/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::DrawObject(int XCor, int YCor, bool Draw, LPDIRECTDRAWSURFACE lpBlitSurface, bool Flag, bool bDrawText, bool bNewInt) 
{
   // Draw all the object to Screen.
   Lock(1668);
   TFCObject *Object = NULL;
   bool NeverDraw = true;
   LPRECT Clip = new RECT;
   LPV2SPRITEFX VsfFX;
   bool childFound = false;
   
   int x, y;
   g_TimeStructure.SetLight();
   

   if (Player.Light) 
   {
      
      if (g_SaveGame.GetOptionsParam()->bLightHightGraph) 
      {
         lmPlayerLight.MergeLightMap(0,0);
      }
      else
      {
         int dwOffsetX = (g_Global.GetScreenWDiv2()-320)/2;
         int dwOffsetY = (g_Global.GetScreenHDiv2()-240)/2;
         lmPlayerLight.MergeLightMap(dwOffsetX,dwOffsetY);
      }
   }
  
   XCor -= 48;
   YCor += - 24 + 37;
   if (!bDrawText) 
   {
      if (!Flag) {
         Clip->left = 0;
         Clip->right = g_Global.GetScreenW()+64;  // 704 Avant with * 2 tiles de 32 de large
         Clip->top = 0;
         Clip->bottom = g_Global.GetScreenH()+32; // 512 Avant height * 2 tiles de 16 de haut;
      } else {
         Clip->left = 0;
         Clip->right = g_Global.GetScreenW();
         Clip->top = 0;
         Clip->bottom = g_Global.GetScreenH();
      }
      
      Grid.Fill(0);		
      GridID.Fill(0);
      GridPriority.Fill(INFINITE);
      GridBlocking.Fill(0);
      CreateComponent(VsfFX, V2SPRITEFX);
      
      for (int MultiPass = 0; MultiPass < 2; MultiPass++) {
         ToHead();
         while(QueryNext()) {
            Object = GetObject();
            childFound = false;
            if (!((Object->Type == __PLAYER_HUMAN_PUPPET || Object->Type == __PLAYER_HUMAN_FEMALE) && !Object->KnownPuppet)) 
            {
               
               if (MultiPass == 1) {
                  if (Object->OY >= 0 && !((Object->OY == 0 || Object->OY == 1 || Object->OY == 2) && 
                                           (Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED  || Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED_I  || Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED  || Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED_I  ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED_I ||
                                            Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED || Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED_I || Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED || Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED_I
                                           )) && NeverDraw && seraphDisplay) 
                  {
                     NeverDraw = false;
                     if (Player.Dead) {
                        //				VObject3D[Set].DrawCorpse(ViewWidth*32-48, ViewHeight*16-24+37, NULL, NULL, NULL, lpBlitSurface);
                        //				Overlap.DrawSprite(", ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                     } else {
                        if (Object3DLoad[Set]) {
                           if (MainObject.Chose == 1) {
                              MainObject.Standing = ST_ATTACK;
                              if (!(MainObject.AttSpeed % MainObject.nbSprite4Att))
                                 MainObject.AttFrame++;
                              if (MainObject.AttFrame == VObject3D[Set]->FacesA) {
                                 MainObject.Chose = 0;
                                 MainObject.AttFrame = 0;
                                 MainObject.SpriteNumber = 1;
                              }
                              if (MainObject.Chose == 1 && Flag == true && !bNewInt) {
                                 BOOL boOutline = FALSE;
                                 DWORD OutColor=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                                 
                                 if (TargetID == Player.ID) 
                                 {
                                    boOutline = TRUE;
                                    OutColor = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 100 / Player.MaxHp));
                                 }
                                 if(boOutline && g_SaveGame.GetOptionsParam()->bShowNewLife)
                                 {
                                    boOutline = FALSE;
                                 }
                                 
                                 //:CR_NMNMNM :Draw Player Attack Ranged and normnal
                                 if( IsRangedObject( &MainObject ) )
                                 {
                                    
                                    VObject3D[Set]->DrawSprite3DR(MainObject.Direction, MainObject.AttFrame, ViewWidth*32-48, ViewHeight*16-24+37, NULL, boOutline, OutColor, lpBlitSurface, 0, &MainObject);                                              
                                 }
                                 else
                                 {
                                    //:CR_NMNMNM :Draw Player Attack Epee
                                    VObject3D[Set]->DrawSprite3DA(MainObject.Direction, MainObject.AttFrame, ViewWidth*32-48, ViewHeight*16-24+37, NULL, boOutline, OutColor, lpBlitSurface, 0, &MainObject);
                                 }
                                 if (MainObject.DrawOverlap)
                                    Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                              }
                              MainObject.AttSpeed++;
                              if (NoMove) {
                                 MainObject.SpriteNumber = 1;
                                 NoMove = FALSE;                       
                              }
                           } else {
                              if (Draw) {
                                 if (VObject3D[Set]->Type == 1) {
                                    if (!(MainObject.Speed % MainObject.nbSprite4Move)) {
                                       if (MainObject.SpriteNumber == 1)
                                          MainObject.SpriteMove = 1;
                                       else if (MainObject.SpriteNumber == VObject3D[Set]->Faces)
                                          MainObject.SpriteMove = -1;
                                       
                                       MainObject.SpriteNumber += MainObject.SpriteMove;
                                    }
                                    MainObject.Speed++;
                                 } else if (VObject3D[Set]->Type == 0) {
                                    if (!(MainObject.Speed % MainObject.nbSprite4Move)) {
                                       if (MainObject.SpriteNumber == VObject3D[Set]->Faces)
                                          MainObject.SpriteNumber = 0;
                                       
                                       MainObject.SpriteNumber += MainObject.SpriteMove;
                                    }
                                    MainObject.Speed++;
                                 }
                                 MainObject.Standing = ST_MOVING;
                                 if (NoMove) {
                                    MainObject.SpriteNumber = 1;
                                    NoMove = FALSE;
                                 }
                              } else {
                                 if (MainObject.Standing == ST_ATTACK || MainObject.Standing == ST_ATTACK_STANDING) {
                                    MainObject.Standing = ST_ATTACK_STANDING;
                                    if (VObject3D[Set]->FacesStA) {
                                       if (!NoMove) {
                                          MainObject.SpriteNumber = 1;
                                          NoMove = TRUE;
                                       }
                                       if (!(MainObject.Speed % MainObject.nbSprite4Move)) {
                                          if (MainObject.SpriteNumber == VObject3D[Set]->FacesStA)
                                             MainObject.SpriteNumber = 0;
                                          
                                          MainObject.SpriteNumber += MainObject.SpriteMove;
                                       }
                                       MainObject.Speed++;
                                       if ((MainObject.Speed % (VObject3D[Set]->FacesStA*3)) >= VObject3D[Set]->FacesStA)
                                          MainObject.SpriteNumber = 1;
                                    }
                                 } else {
                                    MainObject.Standing = ST_STANDING;      
                                    if (VObject3D[Set]->FacesStM) {
                                       if (!NoMove) {
                                          MainObject.SpriteNumber = 1;
                                          NoMove = TRUE;
                                       }
                                       if (!(MainObject.Speed % MainObject.nbSprite4Move)) {
                                          if (MainObject.SpriteNumber == VObject3D[Set]->FacesStM)
                                             MainObject.SpriteNumber = 0;
                                          
                                          MainObject.SpriteNumber += MainObject.SpriteMove;
                                       }
                                       MainObject.Speed++;
                                       if ((MainObject.Speed % (VObject3D[Set]->FacesStM*3)) >= VObject3D[Set]->FacesStM)
                                          MainObject.SpriteNumber = 1;
                                    }
                                 }
                              }
                              
                           }     
                           if (MainObject.Chose != 1 && Flag == true) {
                              BOOL boOutline = FALSE;
                              DWORD OutColor=0;//BLBLBL 29 mars 2009 initialisation à 0
                              
                              if (TargetID == Player.ID) 
                              {
                                 boOutline = TRUE;
                                 OutColor = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 100 / Player.MaxHp));
                              }
                              if (!bNewInt) 
                              {
                                 if(boOutline && g_SaveGame.GetOptionsParam()->bShowNewLife)
                                 {
                                    boOutline = FALSE;
                                 }

                                 //:CR_NMNMNM :Draw Player Normal
                                 VObject3D[Set]->DrawSprite3D(MainObject.Direction, MainObject.SpriteNumber-1, (ViewWidth*32)-48, (ViewHeight*16)-24+37, MainObject.Standing, NULL, boOutline, OutColor, lpBlitSurface, 0, &MainObject);
                                 if (MainObject.DrawOverlap)
                                    Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                              }
                           }
                        }
                     }
                     SetGrid(NULL);
                  }
               }
               
               if (Object->Dynamic == Flag) {
                  
                  if (MultiPass == 1) {
                     if (Object->spellChildID) {
                        SavePointer();
                        if (FoundChild(Object->spellChildID)) {
                           childFound = true;
                        }
                        RestorePointer();
                     } 
                     if (!childFound) {
                        if (Object->Chose == 0 || (Object->Chose == 1 && (isMovX || isMovY) )) {
                           int DebugValue = 0;
                           World.Save();
                           
                           DebugValue = 1;
                           
                           Deplacement *D;
                           
                           D = Object->MovingQueue.Retreive(FALSE);
                           
                           if (Object->Chose == 0 && (!isMovX && !isMovY) || (D && D->Type != 0)
                              || Object->MovingQueue.NbObjects() > 3
                              ) {
                              DebugValue = 2;
                              
                              BOOL Fast = FALSE;
                              
                              if(D = Object->MovingQueue.Retreive()) {
                                 DebugValue = 3;
                                 MoveObject2(D, Fast);
                                 if (D->Victim)
                                    delete D->Victim;
                                 if (D->Parry)
                                    delete D->Parry;
                                 delete D;
                                 if (Object->NoMove) {
                                    Object->SpriteNumber = 1;
                                    Object->NoMove = FALSE;
                                 }
                              } else {
                                 DebugValue = 4;
                                 //               if (Object->Dynamic) {
                                 //                  Object->Dynamic = false;
                                 //                  World.ChangeStatic();			 
                                 //               }
                                 if (!isMovX && !isMovY)
                                    if (Object->Standing == ST_ATTACK || Object->Standing == ST_ATTACK_STANDING) {
                                       DebugValue = 5;
                                       Object->Standing = ST_ATTACK_STANDING;
                                       if (Object->FacesStA) {
                                          if (!Object->NoMove) {
                                             Object->SpriteNumber = 1;
                                             Object->NoMove = TRUE;
                                          }
                                          if (!(Object->Speed % MainObject.nbSprite4Move)) {
                                             DebugValue = 6;
                                             if (Object->SpriteNumber == Object->FacesStA)
                                                Object->SpriteNumber = 0;
                                             
                                             Object->SpriteNumber += Object->SpriteMove;
                                          }
                                          Object->Speed++;
                                          if ((Object->Speed % (Object->FacesStA*3)) >= Object->FacesStA)
                                             Object->SpriteNumber = 1;
                                       }
                                    } else {
                                       DebugValue = 7;
                                       Object->Standing = ST_STANDING;
                                       if (Object->FacesStM) {
                                          if (!Object->NoMove) {
                                             Object->SpriteNumber = 1;
                                             Object->NoMove = TRUE;
                                          }
                                          if (!(Object->Speed % MainObject.nbSprite4Move)) {
                                             DebugValue = 8;
                                             if (Object->SpriteNumber == Object->FacesStM)
                                                Object->SpriteNumber = 0;
                                             
                                             Object->SpriteNumber += Object->SpriteMove;
                                          }
                                          Object->Speed++;
                                          if ((Object->Speed % (Object->FacesStM*3)) >= Object->FacesStM)
                                             Object->SpriteNumber = 1;
                                       }
                                    } 
                              }
                           }
                           DebugValue = 9;
                           
                           if (Object->Movement == 3) {
                              DebugValue = 10;
                              if (!(Object->Speed % Object->nbSprite4Move)) {
                                 if (Object->SpriteNumber == Object->Faces - 1) {
                                    Object->DeleteMe = TRUE;
                                 }
                                 if (Object->SpriteNumber == Object->Faces) {
                                    Object->SpriteNumber = 0;
                                 }
                                 Object->SpriteNumber += Object->SpriteMove;
                              }
                              Object->Speed++;
                           }
                           
                           if (Object->Movement == 2) {
                              DebugValue = 11;
                              if (!(Object->Speed % Object->nbSprite4Move)) {
                                 if (Object->SpriteNumber == Object->Faces)
                                    Object->SpriteNumber = 0;
                                 
                                 Object->SpriteNumber += Object->SpriteMove;
                              }
                              Object->Speed++;
                           } 
                           
                           if (isMovX || isMovY) {
                              DebugValue = 12;
                              if (isMovX) {
                                 Object->fMovX += Object->SpeedX;
                                 Object->MovX = Object->fMovX;
                              }
                              if (isMovY) {
                                 Object->fMovY += Object->SpeedY;
                                 Object->MovY = Object->fMovY;
                              }
                              if (Object->Movement == 1) {
                                 if (!(Object->Speed % Object->nbSprite4Move)) {
                                    if (Object->SpriteNumber == 1)
                                       Object->SpriteMove = 1;
                                    else if (Object->SpriteNumber == Object->Faces) 
                                       Object->SpriteMove = -1;
                                    
                                    Object->SpriteNumber += Object->SpriteMove;
                                 }
                                 Object->Speed++;
                              } else if (Object->Movement == 0) {
                                 if (!(Object->Speed % Object->nbSprite4Move)) {
                                    if (Object->SpriteNumber == Object->Faces)
                                       Object->SpriteNumber = 0;
                                    
                                    Object->SpriteNumber += Object->SpriteMove;
                                 }
                                 Object->Speed++;
                              } 
                           } 
                        } 
                        if (Object->Chose == 1) {
                           if (!(Object->AttSpeed % Object->nbSprite4Att))
                              Object->AttFrame++;
                           if (Object->AttFrame == Object->AttFaces) {
                              Object->AttFrame = 0;
                              Object->Chose = 0;
                              Object->SpriteNumber = 1;
                           }
                           Object->AttSpeed++;
                        }
                     }
                  }
                  
                  if (MultiPass == 0) {
                     if ((Object->Type >= 15000 && Object->Type < 20000) || (Object->Type >= 25000 && Object->Type < 30000)) {
                        if (Object->CurrentCorpseFrame < Object->CorpseFaces-1)
                           Object->CurrentCorpseFrame++;
                        Object->Chose = 0;
                     }
                  }
                  
                  BOOL boOutline = FALSE;
                  DWORD OutColor=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                  if (Object->ID == TargetID || Object->ID == FreezeID) {
                     boOutline = TRUE;
                     OutColor = Object->pHp;
                  }
                  if (FreezeID != TargetID && Object->ID == FreezeID) {
                     OutColor = 150;
                  }
                  
                  if (Object->Chose == 1 && MultiPass == 1 && !bNewInt && !childFound) {
                     switch (Object->Type) {
                     case __PLAYER_HUMAN_PUPPET:
                     case __PLAYER_HUMAN_FEMALE:
                     case __NPC_HUMAN_GUARD:
                     case __NPC_HUMAN_WARRIOR:
                     case __MONSTER_HUMAN_SWORDMAN:
                     case __MONSTER_HUMAN_MAGE:
                     case __MONSTER_HUMAN_PRIEST:
                     case __MONSTER_HUMAN_THIEF:
                     case __MONSTER_KOBOLD:
                     case __MONSTER_OOZE:
                     case __MONSTER_ORC:
                     case __MONSTER_ORC_ICE:
                     case __MONSTER_KRAANIAN_POD_ICE:
                     case __MONSTER_ZOMBIE:
                     case __MONSTER_WOLF:
                     case __MONSTER_BIGWOLF1:
                     case __MONSTER_BIGWOLF2:
                     case __MONSTER_DARKWOLF:
                     case __MONSTER_RED_GOBLIN:
                     case __MONSTER_DRACONIAN_PLATE:
                     case __MONSTER_DRACONIAN_ROBE:
                     case __MONSTER_DRACONIAN_LEATHER:
                     case __MONSTER_SKELETON_CENTAUR:
                     case __MONSTER_SKELETON_SERVANT_1:
                     case __MONSTER_SKELETON_SERVANT_2:
                     case __MONSTER_SKELETON_KING:
                     case __MONSTER_OGRE:
                     case __MONSTER_GOLEM1:
                     case __MONSTER_GOLEM2:
                     case __MONSTER_GOLEM3:
                     case __MONSTER_LICH:
                     case __MONSTER_CENTAUR_WARRIOR:
                     case __MONSTER_CENTAUR_ARCHER:
                     case __MONSTER_CENTAUR_SHAMAN:
                     case __MONSTER_CENTAUR_KING:
                     case __MONSTER_SKAVEN_PEON:
                     case __MONSTER_SKAVEN_SHAMAN:
                     case __MONSTER_SKAVEN_SKAVENGER:
                     case __MONSTER_SKAVEN_WARRIOR:
                     case __MONSTER_SKAVEN_PEON2:
                     case __MONSTER_SKAVEN_SHAMAN2:
                     case __MONSTER_SKAVEN_SKAVENGER2:
                     case __MONSTER_SKAVEN_WARRIOR2:
                     case __MONSTER_SKAVEN_PEON3:
                     case __MONSTER_SKAVEN_SHAMAN3:
                     case __MONSTER_SKAVEN_SKAVENGER3:
                     case __MONSTER_SKAVEN_WARRIOR3:
                     case __MONSTER_RED_GOBLINBOSS:
                     case __MONSTER_UNICORN:
                     case __MONSTER_DRAGON:
                     case __MONSTER_SCORPION:
                     case __MONSTER_KRAANIAN:
                     case __MONSTER_KRAANIAN_ICE:
                     case __MONSTER_NIGHTMARE:
                     case __MONSTER_ATROCITY:
                     case __MONSTER_ATROCITYBOSS:
                     case __MONSTER_PEGASUS:
                     case __MONSTER_HORSE:
                     case __MONSTER_CENTAUR:
                     case __MONSTER_SNAKE:
                     case __MONSTER_TREE_ENT:
                     case __MONSTER_SMALL_WORM:
                     case __MONSTER_BIG_WORM:
                     case __MONSTER_BRIGAND:
                     case __MONSTER_GREEN_TROLL:
                     case __MONSTER_BEHOLDER:
                     case __MONSTER_MUMMY:
                     case __MONSTER_TARANTULA:
                     case __MONSTER_TARANTULA2:
                     case __MONSTER_BIGTARANTULA1:
                     case __MONSTER_BIGTARANTULA2:
                     case __MONSTER_AGMORKIAN:
                     case __MONSTER_KRAANIANTANK:
                     case __MONSTER_KRAANIANTANK_ICE:
                     case __MONSTER_TAUNTING:
                     case __MONSTER_KRAANIANMILIPEDE:
                     case __MONSTER_DEMON:
                     case __MONSTER_MINOTAUR:
                     case __MONSTER_RAT:
                     case __MONSTER_SPIDER:
                     case __MONSTER_SKELETON:
                     case __MONSTER_SANTA_CLAUS:
                        
                        //Dialsoft new code - BEGIN
                     case __MONSTER_DARKGOBLIN:
                     case __MONSTER_DARKGOBLIN_ICE:
                     case __MONSTER_BURNTREE:
                     case __MONSTER_BIGBURNTREE:
                     case __MONSTER_GROUNDMUMMY:
                     case __MONSTER_BOSSRAT:
                     case __MONSTER_BIGSKEL:
                     case __MONSTER_BIGBIGSKEL:
                     case __MONSTER_DEMON_VIOLET:
                     case __MONSTER_DEMON_BLUE:
                     case __MONSTER_DEMON_GREEN:
                     case __MONSTER_DEMON_LIGHTBLUE:
                     case __MONSTER_DEMON_BLACK:
                     case __MONSTER_DEMON_YELLOW:
                     case __MONSTER_DEMON_WHITE:
                     case __MONSTER_FRED_GOBLIN_VIOLET:
                     case __MONSTER_FRED_GOBLIN_BLUE:
                     case __MONSTER_FRED_GOBLIN_GREEN:
                     case __MONSTER_FRED_GOBLIN_LIGHTBLUE:
                     case __MONSTER_FRED_GOBLIN_GRAY:
                     case __MONSTER_FRED_GOBLIN_YELLOW:
                     case __MONSTER_FRED_GOBLIN_WHITE:
                     case __MONSTER_FOOZE_DARKGREEN:
                     case __MONSTER_FOOZE_DARKYELLOW:
                     case __MONSTER_FOOZE_GREEN:
                     case __MONSTER_FOOZE_RED:
                     case __MONSTER_FOOZE_BLACK:
                     case __MONSTER_FOOZE_YELLOW:
                     case __MONSTER_FOOZE_WHITE:
                     case __MONSTER_WHITECAT:
      					case __MONSTER_DRAGONDARKGREEN:
                     case __MONSTER_DRAGON_RED:
                     case __MONSTER_DRAGON_ORANGE:
                     case __MONSTER_DRAGON_JAUNE:
                     case __MONSTER_DRAGON_BLEU:
                     case __MONSTER_DRAGON_GRIS:
                     case __MONSTER_DRAGON_BLANC:
                     case __MONSTER_DRAGON_NOIR:
                     case __MONSTER_SDRAGONDARKGREEN:
                     case __MONSTER_SDRAGONDARKRED:
                     case __MONSTER_SSDRAGONDARKGREEN:
                     case __MONSTER_SSDRAGONDARKRED:
                     case __MONSTER_SSDRAGONDARKORANGE:
                     case __MONSTER_FGAMEMASTER:
                     case __MONSTER_YETI:
                     case __MONSTER_SMALL_YETI:
                     case __MONSTER_BLOB_R:
                     case __MONSTER_BLOB_G:
                     case __MONSTER_DEMONSKULL:
                     case __MONSTER_TWIGLEE:
                        
                        //Dialsoft new code -END
                        {
                           if(!Object3DLoad[Object->Type3D]) {
                              LoadObject(Object->Type3D);
                           }
                           x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                           y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;


                           if(boOutline && g_SaveGame.GetOptionsParam()->bShowNewLife)
                           {
                              boOutline = FALSE;
                           }

                           if( IsRangedObject( Object ) )
                           {
                              VObject3D[Object->Type3D]->DrawSprite3DR(Object->Direction, Object->AttFrame, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                           }
                           else
                           {
                              VObject3D[Object->Type3D]->DrawSprite3DA(Object->Direction, Object->AttFrame, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                           }
                           INFO *Return = VObject3D[Object->Type3D]->GetCurrentVSF(Object->Direction, Object->SpriteNumber-1, Object->Standing);
                           x += Return->CX;
                           y += Return->CY;
                           delete Return;
                           if (Object->Light == 100)
                              lmOtherPlayerLight.MergeLightMap((x-42+16-30-80)>>1, (y-35+8-55-60)>>1);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        } break;
                        
                     case __MONSTER_BAT:
                     case __MONSTER_KRAANIANFLYING:
                     case __MONSTER_KRAANIANFLYING_ICE:
                     case __MONSTER_WASP:
                     case __MONSTER_BIGWASP:
                        if(!Object3DLoad[Object->Type3D]) 
                        {
                           LoadObject(Object->Type3D);
                        }
                        if(boOutline && g_SaveGame.GetOptionsParam()->bShowNewLife)
                        {
                           boOutline = FALSE;
                        }
                        VObject3D[Object->Type3D]->DrawSprite3DA(Object->Direction, Object->AttFrame, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                        break;
                      } 
                  } else if (!bNewInt && !childFound) {
                     VsfFX->lpClipRect = Clip;
                     VsfFX->lpDirectDrawSurface = lpBlitSurface;
                     if (boOutline) {
                        VsfFX->dwFX = FX_OUTLINE;
                        VsfFX->dwOutlineColor = RGB(255, 255, 255);
                     } else
                        VsfFX->dwFX = NULL;
                     
                     if (Object->Type == __OBJGROUP_2_WOODEN_ROUND_CHAIR_2_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_2_WOODEN_ROUND_CHAIR_2].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_2_WOODEN_CHAIR_2_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_2_WOODEN_CHAIR_2].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_2_WOODEN_CHAIR_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_2_WOODEN_CHAIR].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_WOODEN_ROUND_CHAIR_2_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_WOODEN_ROUND_CHAIR_2].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)							
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_WOODEN_CHAIR_2_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_WOODEN_CHAIR_2].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_WOODEN_CHAIR_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_WOODEN_CHAIR].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_CHEST_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_CHEST].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_CHEST_OPEN_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_CHEST_OPEN].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_CHEST2_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_CHEST2].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_VAULT_TALK_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_VAULT_TALK].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_DOOR_TALK_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_DOOR_TALK].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_VAULT_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        VObject[__OBJGROUP_VAULT].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC)
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } 

                     ///////////////////////////////
                     // DOOR TYPE 1
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject(Object->Faces + 18)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject((8-Object->Faces) + 18)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR_CLOSED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject(Object->Faces + 18)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR_OPENED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject((8-Object->Faces) + 18)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     } 

                     ///////////////////////////////
                     // DOOR TYPE 2
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR2_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR2_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR2_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR2_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR2_CLOSED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR2_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR2_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR2_OPENED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR2_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR2_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     }


                     ///////////////////////////////
                     // DOOR TYPE 3
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR3_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR3_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR3_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR3_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR3_CLOSED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR3_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR3_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR3_OPENED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR3_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR3_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     }

                     ///////////////////////////////
                     // DOOR TYPE 4
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR4_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR4_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR4_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR4_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR4_CLOSED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR4_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR4_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR4_OPENED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR4_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR4_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     }

                     ///////////////////////////////
                     // DOOR TYPE 5
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR5_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR5_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR5_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR5_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR5_CLOSED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR5_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR5_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR5_OPENED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR5_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR5_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     }

                     ///////////////////////////////
                     // DOOR TYPE 6
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR6_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR6_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR6_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR6_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR6_CLOSED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR6_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR6_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR6_OPENED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR6_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR6_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     }

                     ///////////////////////////////
                     // DOOR TYPE 7
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR7_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR7_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR7_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR7_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR7_CLOSED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR7_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR7_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR7_OPENED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR7_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR7_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     }

                     ///////////////////////////////
                     // DOOR TYPE 8
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR8_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR8_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED_I && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR8_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                           VObject[__OBJGROUP_WOODEN_DOOR8_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR8_CLOSED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject(Object->Faces + __OBJGROUP_WOODEN_DOOR8_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else 
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR8_CLOSED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     } 
                     else if (Object->Type == __OBJGROUP_WOODEN_DOOR8_OPENED && MultiPass == 1) 
                     {
                        if (Object->Faces) 
                        {
                           GetVSFObject((8-Object->Faces) + __OBJGROUP_WOODEN_DOOR8_ANIM02)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                           Object->Faces--;
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           World.Restore();
                        } 
                        else  
                        {
                           VObject[__OBJGROUP_WOODEN_DOOR8_OPENED].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           if (Object->DrawOverlap)
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        }
                        VsfFX->dwFX = NULL;
                     }



                     
                     
                     
                     else if (Object->Type == __OBJGROUP_FIRECAMP && MultiPass == 1) {
                        //32 FPS @PoPo
                        GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + __OBJGROUP_FIRECAMP)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        Object->Faces++;
                        if (Object->DrawOverlap)
                           Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        //               Object->Dynamic = true;
                        x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;
                        //x -= GetVSFObject((Object->Faces/2 % Object->Anim) + Object->Type)->GetCX();
                        //y -= GetVSFObject((Object->Faces/2 % Object->Anim) + Object->Type)->GetCY();
                        lmTorchLight.MergeLightMap((x-80-65)>>1, (y-40-80)>>1);
                        World.Restore();
                     } 
                     else if (Object->Type == __OBJGROUP_TORCHE || Object->Type == __OBJGROUP_PORTAL || Object->Type == __OBJGROUP_CAULDRON_CONTAINER &&MultiPass == 1) 
                     {//32 FPS @PoPo
                        GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + Object->Type)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX-10, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;
                        x += GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + Object->Type)->GetCX();
                        y += GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + Object->Type)->GetCY();
                        lmTorchLight.MergeLightMap((x-80-65)>>1, (y-40)>>1);
                        Object->Faces++;
                        if (Object->Type == __OBJGROUP_PORTAL || Object->Type == __OBJGROUP_CAULDRON_CONTAINER)
                           Object->Faces++;
                        //               Object->Dynamic = true;
                        World.Restore();
                     } 
  
                     else if (Object->Type == __OBJANIM_FONTAIN_02 && MultiPass == 1) 
                     {
                        //On Fais Rien...
                     }  
                     else if (Object->Type == __OBJANIM_MOULIND && MultiPass == 1) 
                     {
                        //On Fais Rien...
                     }  
                     else if (Object->Type == __OBJANIM_MOULING && MultiPass == 1) 
                     {
                        //On Fais Rien...
                     }  
                     else if (Object->Type == __OBJLIGHT_LANTERNE1  && MultiPass == 1) 
                     {
                        x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;
                        lmTorchLanterne1.MergeLightMap((x-80-65)>>1, (y-165)>>1);                        
                     }
                     else if (Object->Type == __OBJLIGHT_CHANDELIER  && MultiPass == 1) 
                     {
                        x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;
                        lmPlayerLight.MergeLightMap((x-300)>>1, (y-240)>>1);                        
                     }
                     else if (Object->Type == __OBJLIGHT_POUTRE  && MultiPass == 1) 
                     {
                        x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;
                        lmTorchPoutre.MergeLightMap((x-65)>>1, (y-125)>>1);                        
                     }
                     else if (Object->Type == __OBJLIGHT_CHANDELLE2  && MultiPass == 1) 
                     {
                        x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;
                        lmTorchChandelle2.MergeLightMap((x-65)>>1, (y-125)>>1);                        
                     }
                     //32 FPS @PoPo
                     else if (Object->Type == __OBJGROUP_CAULDRON /*|| Object->Type == __OBJGROUP_CAULDRON_CONTAINER*/&& MultiPass == 1) {
                        GetVSFObject((Object->Faces % Object->Anim) + Object->Type)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;
                        x += GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + Object->Type)->GetCX();
                        y += GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + Object->Type)->GetCY();
                        lmTorchLight.MergeLightMap((x-80-65)>>1, (y-40)>>1);
                        Object->Faces++;
                        //               Object->Dynamic = true;
                        if (Object->DrawOverlap)
                           Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        World.Restore();
                     } else if (Object->Type == __OBJGROUP_CAULDRON /*|| Object->Type == __OBJGROUP_CAULDRON_CONTAINER*/&& MultiPass == 0) {
                     } else if (Object->Type >= __OBJGROUP_SKAVEN_CORPSE1 && Object->Type <= __OBJGROUP_SKAVEN_CORPSE4 && MultiPass == 1) {
                     } else if (Object->Type >= __OBJGROUP_SKAVEN_CORPSE1 && Object->Type <= __OBJGROUP_SKAVEN_CORPSE4 && MultiPass == 0) {
                        GetVSFObject(Object->Type)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        Object->Faces++;
                        //               Object->Dynamic = true;
                        if (Object->DrawOverlap)
                           Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        World.Restore();
                     } else if (Object->Type >= __OBJGROUP_SKAVEN_CORPSE_I1 && Object->Type <= __OBJGROUP_SKAVEN_CORPSE_I4 && MultiPass == 1) {
                     } else if (Object->Type >= __OBJGROUP_SKAVEN_CORPSE_I1 && Object->Type <= __OBJGROUP_SKAVEN_CORPSE_I4 && MultiPass == 0) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        GetVSFObject(Object->Type)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        Object->Faces++;
                        //               Object->Dynamic = true;
                        if (Object->DrawOverlap)
                           Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        World.Restore();
                     } else if (Object->Type == __OBJGROUP_WHIRLPOOL && MultiPass == 1) {
                     } else if (Object->Type == __OBJGROUP_WHIRLPOOL && MultiPass == 0) {
                        GetVSFObject((Object->Faces % Object->Anim) + Object->Type)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        Object->Faces++;
                        //               Object->Dynamic = true;
                        if (Object->DrawOverlap)
                           Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        World.Restore();
                     } else if (Object->Type == __OBJGROUP_WHIRLPOOL2 && MultiPass == 1) {
                     } else if (Object->Type == __OBJGROUP_WHIRLPOOL2 && MultiPass == 0) {
                        GetVSFObject((Object->Faces % Object->Anim) + Object->Type)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        Object->Faces++;
                        //               Object->Dynamic = true;
                        if (Object->DrawOverlap)
                           Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        World.Restore();
                     } else if (Object->Type == __OBJGROUP_ORGUE && MultiPass == 1) {
                     } else if (Object->Type == __OBJGROUP_ORGUE && MultiPass == 0) {
                        GetVSFObject(Object->Type)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        Object->Faces++;
                        //               Object->Dynamic = true;
                        if (Object->DrawOverlap)
                           Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                     } else if (Object->Type == __OBJGROUP_SHOP_INN || 
                        Object->Type == __OBJGROUP_SHOP_POTION ||
                        Object->Type == __OBJGROUP_SHOP_PAWN || 
                        Object->Type == __OBJGROUP_SHOP_ARMOR ||
                        Object->Type == __OBJGROUP_SHOP_WEAPON && 
                        MultiPass == 1) {
                        int n = (Object->Faces/(2*FRAMING32)); //32 FPS @PoPo
                        if (n >= Object->Anim) {
                           n = 0;
                           if (!(rand() % 1000)) {
                              Object->Faces = 0;
                           }
                        }
                        // 32 FPS @PoPo
                        GetVSFObject(n + Object->Type)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX-10, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;
                        x += GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + Object->Type)->GetCX();
                        y += GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + Object->Type)->GetCY();
                        Object->Faces++;
                        if (Object->DrawOverlap)
                           Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        World.Restore();
                     } else if (Object->Type == __OBJGROUP_SHOP_INN_I || 
                        Object->Type == __OBJGROUP_SHOP_POTION_I ||
                        Object->Type == __OBJGROUP_SHOP_PAWN_I || 
                        Object->Type == __OBJGROUP_SHOP_ARMOR_I ||
                        Object->Type == __OBJGROUP_SHOP_WEAPON_I && 
                        MultiPass == 1) {
                        
                        
                        int n = (Object->Faces/(2*FRAMING32));
                        if (n >= Object->Anim) {
                           n = 0;
                           if (!(rand() % 1000)) {
                              Object->Faces = 0;
                           }
                        }//32 FPS @PoPo
                        // Flipped image, use a mirrored CV2Sprite sprite and type -1.
                        int Type = Object->Type - 1;
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        GetVSFObject(n + Type)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX-10, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;
                        x += GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + Type)->GetCX();
                        y += GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + Type)->GetCY();
                        Object->Faces++;
                        if (Object->DrawOverlap)
                           Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                        World.Restore();
                     } else if (Object->Type == __OBJGROUP_TORCHE_I && MultiPass == 1) {
                        VsfFX->dwFX |= FX_LEFTRIGHTMIRROR;
                        GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + __OBJGROUP_TORCHE)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+10, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;
                        x += GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + __OBJGROUP_TORCHE)->GetCX();
                        y += GetVSFObject((Object->Faces/(2*FRAMING32) % Object->Anim) + __OBJGROUP_TORCHE)->GetCY();
                        lmTorchLight.MergeLightMap((x-80-65)>>1, (y-40)>>1);
                        Object->Faces++;
                        //               Object->Dynamic = true;
                        World.Restore();
                     } else if (Object->Type < (__OBJGROUP_LASTGROUP+__OBJANIM_LASTGROUP) && MultiPass == 1) {
                        GetVSFObject(Object->Type)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);
                        if (!Object->OC) {
                           if (Object->DrawOverlap) {
                              Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           }
                        }
                     } else{
                        switch (Object->Type) {
                        case __SPELL_BOULDERS: 
                           {
                              int XXX = 0, YYY = -28;
                              if (MultiPass == 0) {
                                 if(!Object3DLoad[85]) {
                                    LoadObject(85);
                                 }
                                 if (Object->AttachID == Player.ID) {
                                    VObject3D[85]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ViewWidth*32-48+XXX, ViewHeight*16-24+37+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                 } else if (Object->AttachID) {
                                    SavePointer();
                                    if (FoundID(Object->AttachID, true)) {
                                       VObject3D[85]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((*Object->pOX+ViewWidth)<<5)+XCor+*Object->pMovX+XXX, ((*Object->pOY+ViewHeight)<<4)+YCor+*Object->pMovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    }
                                    RestorePointer();
                                 } else  {
                                    VObject3D[85]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+XXX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                 }
                              }
                           }
                           
                           {
                              int XXX = 0, YYY = -28;
                              
                              if (MultiPass == 1) {
                                 if(!Object3DLoad[Object->Type3D]) {
                                    LoadObject(Object->Type3D);
                                 }
                                 if (Object->AttachID == Player.ID) {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ViewWidth*32-48+XXX, ViewHeight*16-24+37+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                                 } else if (Object->AttachID) {
                                    SavePointer();
                                    if (FoundID(Object->AttachID, true)) {
                                       VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((*Object->pOX+ViewWidth)<<5)+XCor+*Object->pMovX+XXX, ((*Object->pOY+ViewHeight)<<4)+YCor+*Object->pMovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                       if (Object->pOverlap)
                                          Object->pOverlap->DrawSprite(((*Object->pOX+ViewWidth)<<5)+XCor, ((*Object->pOY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                    }
                                    RestorePointer();
                                 } else  {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+XXX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                 }
                              }
                           } break;
                        case __SPELL_BLESS:
                           {
                              int XXX = 10, YYY = -5;
                              
                              if (MultiPass == 1) {
                                 if(!Object3DLoad[Object->Type3D]) {
                                    LoadObject(Object->Type3D);
                                 }
                                 if (Object->AttachID == Player.ID) {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ViewWidth*32-48+XXX, ViewHeight*16-24+37+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                                 } else if (Object->AttachID) {
                                    SavePointer();
                                    if (FoundID(Object->AttachID, true)) {
                                       VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((*Object->pOX+ViewWidth)<<5)+XCor+*Object->pMovX+XXX, ((*Object->pOY+ViewHeight)<<4)+YCor+*Object->pMovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                       if (Object->pOverlap)
                                          Object->pOverlap->DrawSprite(((*Object->pOX+ViewWidth)<<5)+XCor, ((*Object->pOY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                    }
                                    RestorePointer();
                                 } else  {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+XXX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                 }
                              }
                           } break;
                        case __SPELL_ENTANGLE:
                           {
                              int XXX = 0, YYY = 0;
                              
                              if (MultiPass == 1) {
                                 if(!Object3DLoad[Object->Type3D]) {
                                    LoadObject(Object->Type3D);
                                 }
                                 if (Object->AttachID == Player.ID) {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ViewWidth*32-48+XXX, ViewHeight*16-24+37+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                                 } else if (Object->AttachID) {
                                    SavePointer();
                                    if (FoundID(Object->AttachID, true)) {
                                       VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((*Object->pOX+ViewWidth)<<5)+XCor+*Object->pMovX+XXX, ((*Object->pOY+ViewHeight)<<4)+YCor+*Object->pMovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                       if (Object->pOverlap)
                                          Object->pOverlap->DrawSprite(((*Object->pOX+ViewWidth)<<5)+XCor, ((*Object->pOY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                    }
                                    RestorePointer();
                                 } else  {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+XXX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                 }
                              }
                           } break;
                        case __SPELL_GLACIER:
                           {
                              int XXX = 15, YYY = -35;
                              
                              if (MultiPass == 1) {
                                 if(!Object3DLoad[Object->Type3D]) {
                                    LoadObject(Object->Type3D);
                                 }
                                 if (Object->AttachID == Player.ID) {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ViewWidth*32-48+XXX, ViewHeight*16-24+37+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                                 } else if (Object->AttachID) {
                                    SavePointer();
                                    if (FoundID(Object->AttachID, true)) {
                                       VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((*Object->pOX+ViewWidth)<<5)+XCor+*Object->pMovX+XXX, ((*Object->pOY+ViewHeight)<<4)+YCor+*Object->pMovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                       if (Object->pOverlap)
                                          Object->pOverlap->DrawSprite(((*Object->pOX+ViewWidth)<<5)+XCor, ((*Object->pOY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                    }
                                    RestorePointer();
                                 } else  {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+XXX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                 }
                              }
                           } break;
                        case __SPELL_GATEWAY_1:
                           {
                              int XXX = 15, YYY = -35;
                              
                              if (MultiPass == 1) {
                                 if(!Object3DLoad[Object->Type3D]) {
                                    LoadObject(Object->Type3D);
                                 }
                                 if (Object->AttachID == Player.ID) {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ViewWidth*32-48+XXX, ViewHeight*16-24+37+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                                 } else if (Object->AttachID) {
                                    SavePointer();
                                    if (FoundID(Object->AttachID, true)) {
                                       VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((*Object->pOX+ViewWidth)<<5)+XCor+*Object->pMovX+XXX, ((*Object->pOY+ViewHeight)<<4)+YCor+*Object->pMovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                       if (Object->pOverlap)
                                          Object->pOverlap->DrawSprite(((*Object->pOX+ViewWidth)<<5)+XCor, ((*Object->pOY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                    }
                                    RestorePointer();
                                 } else  {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+XXX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                 }
                              }
                           } break;
                           // BEGIN DIALSOFT NEW SPELLS
                        case __SPELL_METEOR_BLACK:
                        case __SPELL_METEOR_WHITE:
                        case __SPELL_METEOR_PURPLE:
                        case __SPELL_METEOR_COBOLT:
                        case __SPELL_METEOR_GREEN:
                        case __SPELL_METEOR_CANDY:
                        case __SPELL_METEOR_GOLD:
                        case __SPELL_METEOR_BROWN:
                        case __SPELL_METEOR_DARK:
                        case __SPELL_METEOR_BLUE:
                        case __SPELL_METEOR_VIOLET:
                           // END DIALSOFT NEW SPELLS
                        case __SPELL_METEORS:
                           {
                              int XXX = 0, YYY = 0;
                              
                              if (MultiPass == 1) {
                                 if(!Object3DLoad[Object->Type3D]) {
                                    LoadObject(Object->Type3D);
                                 }
                                 if (Object->AttachID == Player.ID) {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ViewWidth*32-48+XXX, ViewHeight*16-24+37+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                                 } else if (Object->AttachID) {
                                    SavePointer();
                                    if (FoundID(Object->AttachID, true)) {
                                       VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((*Object->pOX+ViewWidth)<<5)+XCor+*Object->pMovX+XXX, ((*Object->pOY+ViewHeight)<<4)+YCor+*Object->pMovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                       if (Object->pOverlap)
                                          Object->pOverlap->DrawSprite(((*Object->pOX+ViewWidth)<<5)+XCor, ((*Object->pOY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                    }
                                    RestorePointer();
                                 } else  {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+XXX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY+YYY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->DrawOverlap)
                                       Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                 }
                              }
                           } break;
                        case __SPELL_HEALING:
                        case __SPELL_SERAPH_MALE:
                        case __SPELL_SERAPH_FEMALE:
                        case __SPELL_SERAPH:
                        case __SPELL_EVIL_SERAPH_MALE:
                        case __SPELL_EVIL_SERAPH_FEMALE:
                        case __SPELL_EVIL_SERAPH:
                        case __SPELL_DECHU_MALE:
                        case __SPELL_DECHU_FEMALE:
                        case __SPELL_DECHU:
                        case __SPELL_HEALSERIOUS:
                        case __SPELL_POISONCLOUD:
                        case __SPELL_ICECLOUD:
                        case __SPELL_ROCKFLY:
                        case __SPELL_FREEZE:
                        case __SPELL_SEMIBIGEXPLOSION:
                        case __SPELL_CURSE:
                        case __SPELL_ELECTRICSHIELD:
                        case __SPELL_GREENWIPE:
                        case __SPELL_BLUEWIPE:
                        case __SPELL_FIREWIPE:
                        case __SPELL_PENTACLE:
                        case __SPELL_REDWIPE:
                        case __SPELL_SHOCKWAVE:
                        case __SPELL_SMALLEXPLOSION:
                        case __SPELL_GREATEXPLOSION:
                        case __SPELL_FLAK2:
                        case __SPELL_FLAK:
                        case __SPELL_GREATBOLT:
                        case __SPELL_FIRECIRCLE:
                           // BEGIN DIALSOFT NEW SPELLS
                        case __SPELL_GREATBOLT_AZURE:
                        case __SPELL_GREATBOLT_CRIMSON:
                        case __SPELL_GREATBOLT_LIME:
                        case __SPELL_GREATBOLT_EMERALD:
                        case __SPELL_GREATBOLT_GREEN:
                        case __SPELL_GREATBOLT_CANDY:
                        case __SPELL_GREATBOLT_RED:
                        case __SPELL_GREATBOLT_BLACK:
                           // END DIALSOFT NEW SPELLS

                           //Dialsoft added code
                        case __SPELL_MEDIUM_HEALING:
                        case __SPELL_ICE_TREE:
                        case __SPELL_SUPRAHEAL:
                        case __SPELL_SKILLS_EFFECT:
                        case __SPELL_LEVELUP:
                        case __SPELL_NEW_LIGHTNING1:
                        case __SPELL_NEW_FIRE1:
                        case __SPELL_NEW_POISON1:
                        case __SPELL_NEW_SUPRAHEAL://BLBL tentative d'ajouter 2 nouveau sorts des DDA
                        case __SPELL_NEW_BOULDERS://BLBL tentative d'ajouter 2 nouveau sorts des DDA
                           //dialsoft END
                           
                              
                        //++NMNMNM --[NEW TEST SPELL Draw Sprite]
                        /*
                        case __SPELL_A_THUNDERSTROM_A:
                           MultiPass = !MultiPass;
                           MultiPass = !MultiPass;
                        case __SPELL_A_BOULDER_FIREA:
                        case __SPELL_A_NECRO_A01A:
                        case __SPELL_A_POISON_A01A:
                        case __SPELL_A_FIRE_A01A:
                        case __SPELL_A_PROTECT_A01A:
                        case __SPELL_A_HEAL_A01A:
                        case __SPELL_A_FIRE_A02A:
                        */
                        
                           if (MultiPass == 1) 
                           {
                              if(!Object3DLoad[Object->Type3D]) 
                              {
                                 LoadObject(Object->Type3D);
                              }
                              if (Object->AttachID == Player.ID) 
                              {
                                 VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ViewWidth*32-48, ViewHeight*16-24+37, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                 if (Object->DrawOverlap)
                                    Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                              } 
                              else if (Object->AttachID) 
                              {
                                 SavePointer();
                                 if (FoundID(Object->AttachID, true)) 
                                 {
                                    VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((*Object->pOX+ViewWidth)<<5)+XCor+*Object->pMovX, ((*Object->pOY+ViewHeight)<<4)+YCor+*Object->pMovY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->pOverlap)
                                       Object->pOverlap->DrawSprite(((*Object->pOX+ViewWidth)<<5)+XCor, ((*Object->pOY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                 }
                                 RestorePointer();
                              } 
                              else  
                              {
                                 VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                 if (Object->DrawOverlap)
                                    Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                              }
                           }
                           break;

                         //++NMNMNM --[NEW TEST SPELL double spell... dessine tj le glow apres le spell ]
                           /*
                        case __SPELL_A_BOULDER_FIRE: //case __SPELL_A_BOULDER_FIREA:
                        case __SPELL_A_NECRO_A01   : //case __SPELL_A_NECRO_A01A:
                        case __SPELL_A_POISON_A01  : //case __SPELL_A_POISON_A01A:
                        case __SPELL_A_FIRE_A01    : //case __SPELL_A_FIRE_A01A:
                        case __SPELL_A_PROTECT_A01 : //case __SPELL_A_PROTECT_A01A:
                        case __SPELL_A_HEAL_A01    : //case __SPELL_A_HEAL_A01A:
                        case __SPELL_A_FIRE_A02    : //case __SPELL_A_FIRE_A02A:
                        */
                           /*if (MultiPass == 1) //BLBLBL 29 mars 2009 retrait des lignes inutilisées
                           {
                              if(!Object3DLoad[Object->Type3D+1]) 
                              {
                                 LoadObject(Object->Type3D+1);
                              }
                              if(!Object3DLoad[Object->Type3D]) 
                              {
                                 LoadObject(Object->Type3D);
                              }

                              if (Object->AttachID == Player.ID) 
                              {
                                 VObject3D[Object->Type3D+1]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ViewWidth*32-48, ViewHeight*16-24+37, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                 VObject3D[Object->Type3D+0]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ViewWidth*32-48, ViewHeight*16-24+37, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                 if (Object->DrawOverlap)
                                    Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                              } 
                              else if (Object->AttachID) 
                              {
                                 SavePointer();
                                 if (FoundID(Object->AttachID, true)) 
                                 {
                                    VObject3D[Object->Type3D+1]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((*Object->pOX+ViewWidth)<<5)+XCor+*Object->pMovX, ((*Object->pOY+ViewHeight)<<4)+YCor+*Object->pMovY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    VObject3D[Object->Type3D+0]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((*Object->pOX+ViewWidth)<<5)+XCor+*Object->pMovX, ((*Object->pOY+ViewHeight)<<4)+YCor+*Object->pMovY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                    if (Object->pOverlap)
                                       Object->pOverlap->DrawSprite(((*Object->pOX+ViewWidth)<<5)+XCor, ((*Object->pOY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                                 }
                                 RestorePointer();
                              } 
                              else  
                              {
                                 VObject3D[Object->Type3D+1]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                 VObject3D[Object->Type3D+0]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                                 if (Object->DrawOverlap)
                                    Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                              }
                           }
                           break;*/
                           
                        case __SPELL_LIGHTNINGBOLT:
                        case __SPELL_ARROW:
                        case __SPELL_POISONARROW:
                        case __SPELL_STONESHARD:
                        case __SPELL_SWORD01:
                        case __SPELL_ICESHARD:
                        case __SPELL_FIREBALL:
                        case __SPELL_NEWFIREBALL:
                        case __SPELL_NEWFIREBALL2:
                           // BEGIN DIALSOFT NEW SPELLS
                           // FIREBALL
                        case __SPELL_NEWFIREBALL_BLACK:
                        case __SPELL_NEWFIREBALL_WHITE:
                        case __SPELL_NEWFIREBALL_AZURE:
                        case __SPELL_NEWFIREBALL_MAGENTA:
                        case __SPELL_NEWFIREBALL_YELLOW:
                        case __SPELL_NEWFIREBALL_PURPLE:
                           // END DIALSOFT NEW SPELLS

                           // Drake3
                        case __SPELL_FIREBOLT:
                           // BEGIN DIALSOFT NEW SPELLS
                           // FIREBOLT
                        case __SPELL_FIREBOLT_BLACK:
                        case __SPELL_FIREBOLT_WHITE:
                        case __SPELL_FIREBOLT_YELLOW:
                        case __SPELL_FIREBOLT_GREEN:
                        case __SPELL_FIREBOLT_BLUE:
                        case __SPELL_FIREBOLT_PURPLE:
                           // END DIALSOFT NEW SPELLS
                           

                        //++NMNMNM --[NEW TROWING Weopen Draw Sprite]
                        /*
                        case __SPELL_TROWING_STAR1:
                        case __SPELL_TROWING_DAGGER1:
                        */
                           if (MultiPass == 1) {
                              if(!Object3DLoad[Object->Type3D]) {
                                 LoadObject(Object->Type3D);
                              }
                              VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, Object->Format, Object);
                              if (Object->DrawOverlap)
                                 Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           }
                           break;
                           
                        case __SPELL_RED_ENERGY_BALL:
                        case __SPELL_BLUE_ENERGY_BALL:
                        case __SPELL_YELLOW_ENERGY_BALL:
                        case __SPELL_GREEN_ENERGY_BALL:
                        case __SPELL_WHITE_ENERGY_BALL:
                        case __SPELL_BLACK_ENERGY_BALL:
                        case __SPELL_PURPLE_ENERGY_BALL:
                        case __SPELL_CURSE_WITH_RED_BALL:
                        case __SPELL_FIREWIPE_WITH_RED_BALL:
                        case __SPELL_FLAK_WITH_RED_BALL:
                        case __SPELL_FLAK2_WITH_RED_BALL:
                        case __SPELL_GATEWAY_1_WITH_RED_BALL:
                        case __SPELL_GREATEXPLOSION_WITH_RED_BALL:
                        case __SPELL_METEOR_WITH_RED_BALL:
                        case __SPELL_PENTACLE_WITH_RED_BALL:
                        case __SPELL_REDWIPE_WITH_RED_BALL:
                        case __SPELL_SEMIBIGEXPLOSION_WITH_RED_BALL:
                        case __SPELL_SMALLEXPLOSION_WITH_RED_BALL:
                        case __SPELL_BLUEWIPE_WITH_BLUE_BALL:
                        case __SPELL_CURSE_WITH_BLUE_BALL:
                        case __SPELL_ELECTRICSHIELD_WITH_BLUE_BALL:
                        case __SPELL_FREEZE_WITH_BLUE_BALL:
                        case __SPELL_GLACIER_WITH_BLUE_BALL:
                        case __SPELL_GREATBOLT_WITH_BLUE_BALL:
                        case __SPELL_GREENWIPE_WITH_BLUE_BALL:
                        case __SPELL_HEALING_WITH_BLUE_BALL:
                        case __SPELL_HEALSERIOUS_WITH_BLUE_BALL:
                        case __SPELL_ICECLOUD_WITH_BLUE_BALL:
                        case __SPELL_POISONCLOUD_WITH_BLUE_BALL:
                        case __SPELL_BLESS_WITH_YELLOW_BALL:
                        case __SPELL_BLUEWIPE_WITH_YELLOW_BALL:
                        case __SPELL_CURSE_WITH_YELLOW_BALL:
                        case __SPELL_ELECTRICSHIELD_WITH_YELLOW_BALL:
                        case __SPELL_FREEZE_WITH_YELLOW_BALL:
                        case __SPELL_GATEWAY_1_WITH_YELLOW_BALL:
                        case __SPELL_GREATBOLT_WITH_YELLOW_BALL:
                        case __SPELL_ICECLOUD_WITH_YELLOW_BALL:
                        case __SPELL_BOULDERS_WITH_GREEN_BALL:
                        case __SPELL_CURSE_WITH_GREEN_BALL:
                        case __SPELL_ENTANGLE_WITH_GREEN_BALL:
                        case __SPELL_FLAK_WITH_GREEN_BALL:
                        case __SPELL_FLAK2_WITH_GREEN_BALL:
                        case __SPELL_GREENWIPE_WITH_GREEN_BALL:
                        case __SPELL_PENTACLE_WITH_GREEN_BALL:
                        case __SPELL_POISONCLOUD_WITH_GREEN_BALL:
                        case __SPELL_ROCKFLY_WITH_GREEN_BALL:
                        case __SPELL_BLESS_WITH_WHITE_BALL:
                        case __SPELL_BLUEWIPE_WITH_WHITE_BALL:
                        case __SPELL_CURSE_WITH_WHITE_BALL:
                        case __SPELL_ELECTRICSHIELD_WITH_WHITE_BALL:
                        case __SPELL_FLAK_WITH_WHITE_BALL:
                        case __SPELL_FLAK2_WITH_WHITE_BALL:
                        case __SPELL_HEALING_WITH_WHITE_BALL:
                        case __SPELL_HEALSERIOUS_WITH_WHITE_BALL:
                        case __SPELL_ICECLOUD_WITH_WHITE_BALL:
                        case __SPELL_CURSE_WITH_BLACK_BALL:
                        case __SPELL_ENTANGLE_WITH_BLACK_BALL:
                        case __SPELL_FIREWIPE_WITH_BLACK_BALL:
                        case __SPELL_GREATEXPLOSION_WITH_BLACK_BALL:
                        case __SPELL_GREENWIPE_WITH_BLACK_BALL:
                        case __SPELL_METEOR_WITH_BLACK_BALL:
                        case __SPELL_REDWIPE_WITH_BLACK_BALL:
                        case __SPELL_SEMIBIGEXPLOSION_WITH_BLACK_BALL:
                        case __SPELL_SMALLEXPLOSION_WITH_BLACK_BALL:
                        case __SPELL_BLUEWIPE_WITH_PURPLE_BALL:
                        case __SPELL_CURSE_WITH_PURPLE_BALL:
                        case __SPELL_ELECTRICSHIELD_WITH_PURPLE_BALL:
                        case __SPELL_FLAK_WITH_PURPLE_BALL:
                        case __SPELL_FLAK2_WITH_PURPLE_BALL:
                        case __SPELL_GATEWAY_1_WITH_PURPLE_BALL:
                        case __SPELL_GREATEXPLOSION_WITH_PURPLE_BALL:
                        case __SPELL_GREENWIPE_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_WITH_PURPLE_BALL:
                        case __SPELL_PENTACLE_WITH_PURPLE_BALL:
                        case __SPELL_POISONCLOUD_WITH_PURPLE_BALL:
                        case __SPELL_SEMIBIGEXPLOSION_WITH_PURPLE_BALL:
                        case __SPELL_SMALLEXPLOSION_WITH_PURPLE_BALL:
                        case __SPELL_FIRECIRCLE_WITH_RED_BALL:
                        case __SPELL_FIRECIRCLE_WITH_BLACK_BALL:
                        case __SPELL_FIRECIRCLE_WITH_PURPLE_BALL:
                           // BEGIN DIALSOFT NEW SPELLS
                           // METEORS
                        case __SPELL_METEOR_BLACK_WITH_RED_BALL:
                        case __SPELL_METEOR_WHITE_WITH_RED_BALL:
                        case __SPELL_METEOR_PURPLE_WITH_RED_BALL:
                        case __SPELL_METEOR_COBOLT_WITH_RED_BALL:
                        case __SPELL_METEOR_GREEN_WITH_RED_BALL:
                        case __SPELL_METEOR_CANDY_WITH_RED_BALL:
                        case __SPELL_METEOR_GOLD_WITH_RED_BALL:
                        case __SPELL_METEOR_BROWN_WITH_RED_BALL:
                        case __SPELL_METEOR_DARK_WITH_RED_BALL:
                        case __SPELL_METEOR_BLUE_WITH_RED_BALL:
                        case __SPELL_METEOR_VIOLET_WITH_RED_BALL:
                        case __SPELL_METEOR_WITH_BLUE_BALL:
                        case __SPELL_METEOR_BLACK_WITH_BLUE_BALL:
                        case __SPELL_METEOR_WHITE_WITH_BLUE_BALL:
                        case __SPELL_METEOR_PURPLE_WITH_BLUE_BALL:
                        case __SPELL_METEOR_COBOLT_WITH_BLUE_BALL:
                        case __SPELL_METEOR_GREEN_WITH_BLUE_BALL:
                        case __SPELL_METEOR_CANDY_WITH_BLUE_BALL:
                        case __SPELL_METEOR_GOLD_WITH_BLUE_BALL:
                        case __SPELL_METEOR_BROWN_WITH_BLUE_BALL:
                        case __SPELL_METEOR_DARK_WITH_BLUE_BALL:
                        case __SPELL_METEOR_BLUE_WITH_BLUE_BALL:
                        case __SPELL_METEOR_VIOLET_WITH_BLUE_BALL:
                        case __SPELL_METEOR_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_BLACK_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_WHITE_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_PURPLE_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_COBOLT_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_GREEN_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_CANDY_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_GOLD_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_BROWN_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_DARK_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_BLUE_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_VIOLET_WITH_YELLOW_BALL:
                        case __SPELL_METEOR_WITH_GREEN_BALL:
                        case __SPELL_METEOR_BLACK_WITH_GREEN_BALL:
                        case __SPELL_METEOR_WHITE_WITH_GREEN_BALL:
                        case __SPELL_METEOR_PURPLE_WITH_GREEN_BALL:
                        case __SPELL_METEOR_COBOLT_WITH_GREEN_BALL:
                        case __SPELL_METEOR_GREEN_WITH_GREEN_BALL:
                        case __SPELL_METEOR_CANDY_WITH_GREEN_BALL:
                        case __SPELL_METEOR_GOLD_WITH_GREEN_BALL:
                        case __SPELL_METEOR_BROWN_WITH_GREEN_BALL:
                        case __SPELL_METEOR_DARK_WITH_GREEN_BALL:
                        case __SPELL_METEOR_BLUE_WITH_GREEN_BALL:
                        case __SPELL_METEOR_VIOLET_WITH_GREEN_BALL:
                        case __SPELL_METEOR_WITH_WHITE_BALL:
                        case __SPELL_METEOR_BLACK_WITH_WHITE_BALL:
                        case __SPELL_METEOR_WHITE_WITH_WHITE_BALL:
                        case __SPELL_METEOR_PURPLE_WITH_WHITE_BALL:
                        case __SPELL_METEOR_COBOLT_WITH_WHITE_BALL:
                        case __SPELL_METEOR_GREEN_WITH_WHITE_BALL:
                        case __SPELL_METEOR_CANDY_WITH_WHITE_BALL:
                        case __SPELL_METEOR_GOLD_WITH_WHITE_BALL:
                        case __SPELL_METEOR_BROWN_WITH_WHITE_BALL:
                        case __SPELL_METEOR_DARK_WITH_WHITE_BALL:
                        case __SPELL_METEOR_BLUE_WITH_WHITE_BALL:
                        case __SPELL_METEOR_VIOLET_WITH_WHITE_BALL:
                        case __SPELL_METEOR_BLACK_WITH_BLACK_BALL:
                        case __SPELL_METEOR_WHITE_WITH_BLACK_BALL:
                        case __SPELL_METEOR_PURPLE_WITH_BLACK_BALL:
                        case __SPELL_METEOR_COBOLT_WITH_BLACK_BALL:
                        case __SPELL_METEOR_GREEN_WITH_BLACK_BALL:
                        case __SPELL_METEOR_CANDY_WITH_BLACK_BALL:
                        case __SPELL_METEOR_GOLD_WITH_BLACK_BALL:
                        case __SPELL_METEOR_BROWN_WITH_BLACK_BALL:
                        case __SPELL_METEOR_DARK_WITH_BLACK_BALL:
                        case __SPELL_METEOR_BLUE_WITH_BLACK_BALL:
                        case __SPELL_METEOR_VIOLET_WITH_BLACK_BALL:
                        case __SPELL_METEOR_BLACK_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_WHITE_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_PURPLE_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_COBOLT_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_GREEN_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_CANDY_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_GOLD_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_BROWN_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_DARK_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_BLUE_WITH_PURPLE_BALL:
                        case __SPELL_METEOR_VIOLET_WITH_PURPLE_BALL:
                           // CALL LIGHTNINGS
                        case __SPELL_GREATBOLT_AZURE_WITH_RED_BALL:
                        case __SPELL_GREATBOLT_CRIMSON_WITH_RED_BALL:
                        case __SPELL_GREATBOLT_LIME_WITH_RED_BALL:
                        case __SPELL_GREATBOLT_EMERALD_WITH_RED_BALL:
                        case __SPELL_GREATBOLT_GREEN_WITH_RED_BALL:
                        case __SPELL_GREATBOLT_CANDY_WITH_RED_BALL:
                        case __SPELL_GREATBOLT_RED_WITH_RED_BALL:
                        case __SPELL_GREATBOLT_BLACK_WITH_RED_BALL:
                        case __SPELL_GREATBOLT_AZURE_WITH_BLUE_BALL:
                        case __SPELL_GREATBOLT_CRIMSON_WITH_BLUE_BALL:
                        case __SPELL_GREATBOLT_LIME_WITH_BLUE_BALL:
                        case __SPELL_GREATBOLT_EMERALD_WITH_BLUE_BALL:
                        case __SPELL_GREATBOLT_GREEN_WITH_BLUE_BALL:
                        case __SPELL_GREATBOLT_CANDY_WITH_BLUE_BALL:
                        case __SPELL_GREATBOLT_RED_WITH_BLUE_BALL:
                        case __SPELL_GREATBOLT_BLACK_WITH_BLUE_BALL:
                        case __SPELL_GREATBOLT_AZURE_WITH_YELLOW_BALL:
                        case __SPELL_GREATBOLT_CRIMSON_WITH_YELLOW_BALL:
                        case __SPELL_GREATBOLT_LIME_WITH_YELLOW_BALL:
                        case __SPELL_GREATBOLT_EMERALD_WITH_YELLOW_BALL:
                        case __SPELL_GREATBOLT_GREEN_WITH_YELLOW_BALL:
                        case __SPELL_GREATBOLT_CANDY_WITH_YELLOW_BALL:
                        case __SPELL_GREATBOLT_RED_WITH_YELLOW_BALL:
                        case __SPELL_GREATBOLT_BLACK_WITH_YELLOW_BALL:
                        case __SPELL_GREATBOLT_AZURE_WITH_GREEN_BALL:
                        case __SPELL_GREATBOLT_CRIMSON_WITH_GREEN_BALL:
                        case __SPELL_GREATBOLT_LIME_WITH_GREEN_BALL:
                        case __SPELL_GREATBOLT_EMERALD_WITH_GREEN_BALL:
                        case __SPELL_GREATBOLT_GREEN_WITH_GREEN_BALL:
                        case __SPELL_GREATBOLT_CANDY_WITH_GREEN_BALL:
                        case __SPELL_GREATBOLT_RED_WITH_GREEN_BALL:
                        case __SPELL_GREATBOLT_BLACK_WITH_GREEN_BALL:
                        case __SPELL_GREATBOLT_AZURE_WITH_WHITE_BALL:
                        case __SPELL_GREATBOLT_CRIMSON_WITH_WHITE_BALL:
                        case __SPELL_GREATBOLT_LIME_WITH_WHITE_BALL:
                        case __SPELL_GREATBOLT_EMERALD_WITH_WHITE_BALL:
                        case __SPELL_GREATBOLT_GREEN_WITH_WHITE_BALL:
                        case __SPELL_GREATBOLT_CANDY_WITH_WHITE_BALL:
                        case __SPELL_GREATBOLT_RED_WITH_WHITE_BALL:
                        case __SPELL_GREATBOLT_BLACK_WITH_WHITE_BALL:
                        case __SPELL_GREATBOLT_AZURE_WITH_BLACK_BALL:
                        case __SPELL_GREATBOLT_CRIMSON_WITH_BLACK_BALL:
                        case __SPELL_GREATBOLT_LIME_WITH_BLACK_BALL:
                        case __SPELL_GREATBOLT_EMERALD_WITH_BLACK_BALL:
                        case __SPELL_GREATBOLT_GREEN_WITH_BLACK_BALL:
                        case __SPELL_GREATBOLT_CANDY_WITH_BLACK_BALL:
                        case __SPELL_GREATBOLT_RED_WITH_BLACK_BALL:
                        case __SPELL_GREATBOLT_BLACK_WITH_BLACK_BALL:
                        case __SPELL_GREATBOLT_AZURE_WITH_PURPLE_BALL:
                        case __SPELL_GREATBOLT_CRIMSON_WITH_PURPLE_BALL:
                        case __SPELL_GREATBOLT_LIME_WITH_PURPLE_BALL:
                        case __SPELL_GREATBOLT_EMERALD_WITH_PURPLE_BALL:
                        case __SPELL_GREATBOLT_GREEN_WITH_PURPLE_BALL:
                        case __SPELL_GREATBOLT_CANDY_WITH_PURPLE_BALL:
                        case __SPELL_GREATBOLT_RED_WITH_PURPLE_BALL:
                        case __SPELL_GREATBOLT_BLACK_WITH_PURPLE_BALL:
                           // END DIALSOFT NEW SPELLS

                           
                           if (MultiPass == 1) {
                              if(!Object3DLoad[Object->Type3D]) {
                                 LoadObject(Object->Type3D);
                              }
                              VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY-35, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, Object->Format, Object);
                              if (Object->DrawOverlap)
                                 Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           }
                           break;
                           
                           
                           
                        case __SPELL_BROKEN_ARROW:
                           if (MultiPass == 1) 
                           {
                              if(!Object3DLoad[Object->Type3D]) 
                              {
                                 LoadObject(Object->Type3D);
                              }
                              VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, ST_MOVING, Clip, boOutline, OutColor, lpBlitSurface, Object->Format, Object);
                              if (Object->DrawOverlap)
                                 Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           }
                           break;
                           
                        case __MONSTER_HUMAN_MAGE:
                        case __MONSTER_HUMAN_PRIEST:
                        case __MONSTER_HUMAN_THIEF:
                        case __MONSTER_KOBOLD:
                        case __MONSTER_OOZE:
                        case __MONSTER_ORC:
                        case __MONSTER_ORC_ICE:
                        case __MONSTER_KRAANIAN_POD_ICE:
                        case __MONSTER_ZOMBIE:
                        case __MONSTER_WOLF:
                        case __MONSTER_BIGWOLF1:
                        case __MONSTER_BIGWOLF2:
                        case __MONSTER_DARKWOLF:
                        case __MONSTER_RED_GOBLIN:
                        case __MONSTER_DRACONIAN_PLATE:
                        case __MONSTER_DRACONIAN_ROBE:
                        case __MONSTER_DRACONIAN_LEATHER:
                        case __MONSTER_SKELETON_CENTAUR:
                        case __MONSTER_SKELETON_SERVANT_1:
                        case __MONSTER_SKELETON_SERVANT_2:
                        case __MONSTER_SKELETON_KING:
                        case __MONSTER_OGRE:
                        case __MONSTER_GOLEM1:
                        case __MONSTER_GOLEM2:
                        case __MONSTER_GOLEM3:
                        case __MONSTER_LICH:
                        case __MONSTER_CENTAUR_WARRIOR:
                        case __MONSTER_CENTAUR_ARCHER:
                        case __MONSTER_CENTAUR_SHAMAN:
                        case __MONSTER_CENTAUR_KING:
                        case __MONSTER_SKAVEN_PEON:
                        case __MONSTER_SKAVEN_SHAMAN:
                        case __MONSTER_SKAVEN_SKAVENGER:
                        case __MONSTER_SKAVEN_WARRIOR:
                        case __MONSTER_SKAVEN_PEON2:
                        case __MONSTER_SKAVEN_SHAMAN2:
                        case __MONSTER_SKAVEN_SKAVENGER2:
                        case __MONSTER_SKAVEN_WARRIOR2:
                        case __MONSTER_SKAVEN_PEON3:
                        case __MONSTER_SKAVEN_SHAMAN3:
                        case __MONSTER_SKAVEN_SKAVENGER3:
                        case __MONSTER_SKAVEN_WARRIOR3:
                        case __MONSTER_RED_GOBLINBOSS:
                        case __MONSTER_UNICORN:
                        case __MONSTER_DRAGON:
                        case __NPC_HUMAN_PEASANT:
                        case __MONSTER_DROMADARY:
                        case __MONSTER_PIG:
                        case __MONSTER_ORI:
                        case __NPC_HUMAN_PRIEST:
                        case __NPC_HUMAN_PAYSANNE:
                        case __NPC_HUMAN_RED_MAGE:
                        case __NPC_HUMAN_GUARD:
                        case __NPC_HUMAN_WARRIOR:
                        case __MONSTER_HUMAN_SWORDMAN:
                        case __PLAYER_HUMAN_PUPPET:
                        case __PLAYER_HUMAN_FEMALE:
                        case __MONSTER_SCORPION:
                        case __MONSTER_KRAANIAN:
                        case __MONSTER_KRAANIAN_ICE:
                        case __MONSTER_NIGHTMARE:
                        case __MONSTER_ATROCITY:
                        case __MONSTER_ATROCITYBOSS:
                        case __MONSTER_PEGASUS:
                        case __MONSTER_HORSE:
                        case __MONSTER_CENTAUR:
                        case __MONSTER_SNAKE:
                        case __MONSTER_TREE_ENT:
                        case __MONSTER_SMALL_WORM:
                        case __MONSTER_BIG_WORM:
                        case __MONSTER_SPIDER:
                        case __MONSTER_SKELETON:
                        case __MONSTER_SANTA_CLAUS:
                        case __MONSTER_BRIGAND:
                        case __MONSTER_GREEN_TROLL:
                        case __MONSTER_BEHOLDER:
                        case __MONSTER_MUMMY:
                        case __MONSTER_TARANTULA:
                        case __MONSTER_TARANTULA2:
                        case __MONSTER_BIGTARANTULA1:
                        case __MONSTER_BIGTARANTULA2:
                        case __MONSTER_AGMORKIAN:
                        case __MONSTER_KRAANIANTANK:
                        case __MONSTER_KRAANIANTANK_ICE:
                        case __MONSTER_TAUNTING:
                        case __MONSTER_KRAANIANMILIPEDE:
                        case __MONSTER_DEMON:
                        case __MONSTER_MINOTAUR:
                        case __MONSTER_RAT:
                           //Dialsoft new code - BEGIN
                        case __MONSTER_DARKGOBLIN:
                        case __MONSTER_DARKGOBLIN_ICE:
                        case __MONSTER_BURNTREE:
                        case __MONSTER_BIGBURNTREE:
                        case __MONSTER_GROUNDMUMMY:
                        case __MONSTER_BOSSRAT:
                        case __MONSTER_BIGSKEL: 
                        case __MONSTER_BIGBIGSKEL:
                        case __MONSTER_DEMON_VIOLET:
                        case __MONSTER_DEMON_BLUE:
                        case __MONSTER_DEMON_GREEN:
                        case __MONSTER_DEMON_LIGHTBLUE:
                        case __MONSTER_DEMON_BLACK:
                        case __MONSTER_DEMON_YELLOW:
                        case __MONSTER_DEMON_WHITE:
                        case __MONSTER_FRED_GOBLIN_VIOLET:
                        case __MONSTER_FRED_GOBLIN_BLUE:
                        case __MONSTER_FRED_GOBLIN_GREEN:
                        case __MONSTER_FRED_GOBLIN_LIGHTBLUE:
                        case __MONSTER_FRED_GOBLIN_GRAY:
                        case __MONSTER_FRED_GOBLIN_YELLOW:
                        case __MONSTER_FRED_GOBLIN_WHITE:
                        case __MONSTER_FOOZE_DARKGREEN:
                        case __MONSTER_FOOZE_DARKYELLOW:
                        case __MONSTER_FOOZE_GREEN:
                        case __MONSTER_FOOZE_RED:
                        case __MONSTER_FOOZE_BLACK:
                        case __MONSTER_FOOZE_YELLOW:
                        case __MONSTER_FOOZE_WHITE:
                        case __MONSTER_WHITECAT:
      					   case __MONSTER_DRAGONDARKGREEN:
                        case __MONSTER_DRAGON_RED:
                        case __MONSTER_DRAGON_ORANGE:
                        case __MONSTER_DRAGON_JAUNE:
                        case __MONSTER_DRAGON_BLEU:
                        case __MONSTER_DRAGON_GRIS:
                        case __MONSTER_DRAGON_BLANC:
                        case __MONSTER_DRAGON_NOIR:
                        case __MONSTER_SDRAGONDARKGREEN:
                        case __MONSTER_SDRAGONDARKRED:
                        case __MONSTER_SSDRAGONDARKGREEN:
                        case __MONSTER_SSDRAGONDARKRED:
                        case __MONSTER_SSDRAGONDARKORANGE:
                        case __MONSTER_FGAMEMASTER:
                        case __MONSTER_YETI:
                        case __MONSTER_SMALL_YETI:
                        case __MONSTER_BLOB_R:
                        case __MONSTER_BLOB_G:
                        case __MONSTER_DEMONSKULL:
                        case __MONSTER_TWIGLEE:

                          
                           //Dialsoft new code - END
                           if (MultiPass == 1) {
                              if(!Object3DLoad[Object->Type3D]) 
                              {
                                 LoadObject(Object->Type3D);
                              }
                              x = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                              y = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY;

                              if(boOutline && g_SaveGame.GetOptionsParam()->bShowNewLife)
                              {
                                 boOutline = FALSE;
                              }

                              VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, Object->Standing, Clip, boOutline, OutColor, lpBlitSurface, Object->Format, Object);
                              INFO *Return = VObject3D[Object->Type3D]->GetCurrentVSF(Object->Direction, Object->SpriteNumber-1, Object->Standing);
                              x += Return->CX;
                              y += Return->CY;
                              delete Return;
                              if (Object->Light == 100)
                                 lmOtherPlayerLight.MergeLightMap((x-42+16-30-80)>>1, (y-35+8-55-60)>>1);
                              if (Object->DrawOverlap)
                                 Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           }
                           break;
                           
                        case __MONSTER_BAT:
                        case __MONSTER_KRAANIANFLYING:
                        case __MONSTER_KRAANIANFLYING_ICE:
                        case __MONSTER_WASP:
                        case __MONSTER_BIGWASP:
                           if (MultiPass == 1) {
                              if(!Object3DLoad[Object->Type3D]) {
                                 LoadObject(Object->Type3D);
                              }
                              if(boOutline && g_SaveGame.GetOptionsParam()->bShowNewLife)
                              {
                                 boOutline = FALSE;
                              }
                              VObject3D[Object->Type3D]->DrawSprite3D(Object->Direction, Object->SpriteNumber-1, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, ST_MOVING, Clip, boOutline, OutColor, lpBlitSurface, 0, Object);
                           }
                           break;
                           
                        case __NPC_HUMAN_PEASANT_CORPSE:
                        case __MONSTER_DROMADARY_CORPSE:
                        case __MONSTER_PIG_CORPSE:
                        case __MONSTER_ORI_CORPSE:
                        case __NPC_HUMAN_PRIEST_CORPSE:
                        case __MONSTER_HUMAN_THIEF_CORPSE:
                        case __MONSTER_HUMAN_PRIEST_CORPSE:
                        case __MONSTER_HUMAN_MAGE_CORPSE:
                        case __NPC_HUMAN_PAYSANNE_CORPSE:
                        case __NPC_HUMAN_RED_MAGE_CORPSE:
                        case __NPC_HUMAN_GUARD_CORPSE:
                        case __NPC_HUMAN_WARRIOR_CORPSE:
                        case __MONSTER_HUMAN_SWORDMAN_CORPSE:
                        case __PLAYER_HUMAN_PUPPET_CORPSE:
                        case __PLAYER_HUMAN_FEMALE_CORPSE:
                        case __MONSTER_KOBOLD_CORPSE:
                        case __MONSTER_OOZE_CORPSE:
                        case __MONSTER_ORC_CORPSE:
                        case __MONSTER_ORC_ICE_CORPSE:
                        case __MONSTER_KRAANIAN_POD_ICE_CORPSE:
                        case __MONSTER_ZOMBIE_CORPSE:
                        case __MONSTER_WOLF_CORPSE:
                        case __MONSTER_BIGWOLF1_CORPSE:
                        case __MONSTER_BIGWOLF2_CORPSE:
                        case __MONSTER_DARKWOLF_CORPSE:
                        case __MONSTER_BRIGAND_CORPSE:
                        case __MONSTER_RED_GOBLIN_CORPSE:
                        case __MONSTER_DRACONIAN_PLATE_CORPSE:
                        case __MONSTER_DRACONIAN_ROBE_CORPSE:
                        case __MONSTER_DRACONIAN_LEATHER_CORPSE:
                        case __MONSTER_SKELETON_CENTAUR_CORPSE:
                        case __MONSTER_SKELETON_SERVANT_1_CORPSE:
                        case __MONSTER_SKELETON_SERVANT_2_CORPSE:
                        case __MONSTER_SKELETON_KING_CORPSE:
                        case __MONSTER_OGRE_CORPSE:
                        case __MONSTER_GOLEM1_CORPSE:
                        case __MONSTER_GOLEM2_CORPSE:
                        case __MONSTER_GOLEM3_CORPSE:
                        case __MONSTER_LICH_CORPSE:
                        case __MONSTER_CENTAUR_WARRIOR_CORPSE:
                        case __MONSTER_CENTAUR_ARCHER_CORPSE:
                        case __MONSTER_CENTAUR_SHAMAN_CORPSE:
                        case __MONSTER_CENTAUR_KING_CORPSE:
                        case __MONSTER_SKAVEN_PEON_CORPSE:
                        case __MONSTER_SKAVEN_SHAMAN_CORPSE:
                        case __MONSTER_SKAVEN_SKAVENGER_CORPSE:
                        case __MONSTER_SKAVEN_WARRIOR_CORPSE:
                        case __MONSTER_SKAVEN_PEON2_CORPSE:
                        case __MONSTER_SKAVEN_SHAMAN2_CORPSE:
                        case __MONSTER_SKAVEN_SKAVENGER2_CORPSE:
                        case __MONSTER_SKAVEN_WARRIOR2_CORPSE:
                        case __MONSTER_SKAVEN_PEON3_CORPSE:
                        case __MONSTER_SKAVEN_SHAMAN3_CORPSE:
                        case __MONSTER_SKAVEN_SKAVENGER3_CORPSE:
                        case __MONSTER_SKAVEN_WARRIOR3_CORPSE:
                        case __MONSTER_RED_GOBLINBOSS_CORPSE:
                        case __MONSTER_UNICORN_CORPSE:
                        case __MONSTER_WASP_CORPSE:
                        case __MONSTER_BIGWASP_CORPSE:
                        case __MONSTER_DRAGON_CORPSE:
                        case __MONSTER_SCORPION_CORPSE:
                        case __MONSTER_KRAANIAN_CORPSE:
                        case __MONSTER_KRAANIAN_ICE_CORPSE:
                        case __MONSTER_NIGHTMARE_CORPSE:
                        case __MONSTER_ATROCITY_CORPSE:
                        case __MONSTER_ATROCITYBOSS_CORPSE:
                        case __MONSTER_PEGASUS_CORPSE:
                        case __MONSTER_HORSE_CORPSE:
                        case __MONSTER_CENTAUR_CORPSE:
                        case __MONSTER_SNAKE_CORPSE:
                        case __MONSTER_TREE_ENT_CORPSE:
                        case __MONSTER_SMALL_WORM_CORPSE:
                        case __MONSTER_BIG_WORM_CORPSE:
                        case __MONSTER_BAT_CORPSE:
                        case __MONSTER_RAT_CORPSE:
                        case __MONSTER_SPIDER_CORPSE:
                        case __MONSTER_MUMMY_CORPSE:
                        case __MONSTER_KRAANIANFLYING_CORPSE:
                        case __MONSTER_KRAANIANFLYING_ICE_CORPSE:
                        case __MONSTER_TARANTULA_CORPSE:
                        case __MONSTER_TARANTULA2_CORPSE:
                        case __MONSTER_BIGTARANTULA1_CORPSE:
                        case __MONSTER_BIGTARANTULA2_CORPSE:
                        case __MONSTER_AGMORKIAN_CORPSE:
                        case __MONSTER_KRAANIANTANK_CORPSE:
                        case __MONSTER_KRAANIANTANK_ICE_CORPSE:
                        case __MONSTER_TAUNTING_CORPSE:
                        case __MONSTER_KRAANIANMILIPEDE_CORPSE:
                        case __MONSTER_DEMON_CORPSE:
                        case __MONSTER_MINOTAUR_CORPSE:
                        case __MONSTER_BEHOLDER_CORPSE:
                        case __MONSTER_GREEN_TROLL_CORPSE:
                        case __MONSTER_SKELETON_CORPSE:
                           //Dialsoft new code - BEGIN
                        case __MONSTER_DARKGOBLIN_CORPSE:
                        case __MONSTER_DARKGOBLIN_ICE_CORPSE:
                        case __MONSTER_BURNTREE_CORPSE:
                        case __MONSTER_BIGBURNTREE_CORPSE:
                        case __MONSTER_GROUNDMUMMY_CORPSE:
                        case __MONSTER_BOSSRAT_CORPSE:
                        case __MONSTER_BIGSKEL_CORPSE:
                        case __MONSTER_BIGBIGSKEL_CORPSE:
                        case __MONSTER_DEMON_VIOLET_CORPSE:
                        case __MONSTER_DEMON_BLUE_CORPSE:
                        case __MONSTER_DEMON_GREEN_CORPSE:
                        case __MONSTER_DEMON_LIGHTBLUE_CORPSE:
                        case __MONSTER_DEMON_BLACK_CORPSE:
                        case __MONSTER_DEMON_YELLOW_CORPSE:
                        case __MONSTER_DEMON_WHITE_CORPSE:
                        case __MONSTER_FRED_GOBLIN_VIOLET_CORPSE:
                        case __MONSTER_FRED_GOBLIN_BLUE_CORPSE:
                        case __MONSTER_FRED_GOBLIN_GREEN_CORPSE:
                        case __MONSTER_FRED_GOBLIN_LIGHTBLUE_CORPSE:
                        case __MONSTER_FRED_GOBLIN_GRAY_CORPSE:
                        case __MONSTER_FRED_GOBLIN_YELLOW_CORPSE:
                        case __MONSTER_FRED_GOBLIN_WHITE_CORPSE:
                        case __MONSTER_FOOZE_DARKGREEN_CORPSE:
                        case __MONSTER_FOOZE_DARKYELLOW_CORPSE:
                        case __MONSTER_FOOZE_GREEN_CORPSE:
                        case __MONSTER_FOOZE_RED_CORPSE:
                        case __MONSTER_FOOZE_BLACK_CORPSE:
                        case __MONSTER_FOOZE_YELLOW_CORPSE:
                        case __MONSTER_FOOZE_WHITE_CORPSE:
                        case __MONSTER_WHITECAT_CORPSE:
                		   case __MONSTER_DRAGONDARKGREEN_CORPSE:
                        case __MONSTER_DRAGON_RED_CORPSE:
                        case __MONSTER_DRAGON_ORANGE_CORPSE:
                        case __MONSTER_DRAGON_JAUNE_CORPSE:
                        case __MONSTER_DRAGON_BLEU_CORPSE:
                        case __MONSTER_DRAGON_GRIS_CORPSE:
                        case __MONSTER_DRAGON_BLANC_CORPSE:
                        case __MONSTER_DRAGON_NOIR_CORPSE:
                        case __MONSTER_SDRAGONDARKGREEN_CORPSE:
                        case __MONSTER_SDRAGONDARKRED_CORPSE:
                        case __MONSTER_SSDRAGONDARKGREEN_CORPSE:
                        case __MONSTER_SSDRAGONDARKRED_CORPSE:
                        case __MONSTER_SSDRAGONDARKORANGE_CORPSE:
                        case __MONSTER_FGAMEMASTER_CORPSE:
                        case __MONSTER_YETI_CORPSE:
                        case __MONSTER_SMALL_YETI_CORPSE:
                        case __MONSTER_BLOB_R_CORPSE:
                        case __MONSTER_BLOB_G_CORPSE:
                        case __MONSTER_DEMONSKULL_CORPSE:
                        case __MONSTER_TWIGLEE_CORPSE:
                           //Dialsoft new code - END
                           if ((MultiPass == 0 && Object->CurrentCorpseFrame == Object->CorpseFaces-1) || (MultiPass == 1 && Object->CurrentCorpseFrame != Object->CorpseFaces-1)) {
                              if(!Object3DLoad[Object->Type3D]) {
                                 LoadObject(Object->Type3D);
                              }
                              VObject3D[Object->Type3D]->DrawCorpse(Object->CurrentCorpseFrame, ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, Clip, NULL, NULL, lpBlitSurface, 0, Object->AbsX, Object->AbsY, Object);
                              if (Object->DrawOverlap)
                                 Object->Overlap.DrawSprite(((Object->OX+ViewWidth)<<5)+XCor, ((Object->OY+ViewHeight)<<4)+YCor, Clip, NULL, NULL, lpBlitSurface);
                           }
                           break;
                           
                        default:
                           if (MultiPass == 1) {
                              VObject[__OBJGROUP_SCROLL].DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY, VsfFX);			
                           }
                           break;
            }
         }
      }
      } 
      SetGrid(Object);
      }
      if(MultiPass == 1)
      {
         //char str[300];
         //sprintf(str,"--Search ID[%d] TYPE [%d]\n",Object->ID,Object->ID);
         //OutputDebugString(str);
         //if(Object->DrawOverlap)
         {
            int dwFound = FoundAnimListID(Object->ID);
            if(dwFound>=0)
            {
               //char str[300];
               //sprintf(str,"--Found ID[%d] TYPE [%d]\n",Object->ID,Object->ID);
               //OutputDebugString(str);
               DrawObjectAnimOverLapID(dwFound,false,XCor,YCor,lpBlitSurface);
            }
         }
      }   
   }
   }
   
   DeleteComponent(VsfFX); // GELAGE 1
   if (NeverDraw && seraphDisplay) {
      NeverDraw = false;
      if (Player.Dead) {
         //			VObject3D[Set].DrawCorpse(ViewWidth*32-48, ViewHeight*16-24+37, NULL, NULL, NULL, lpBlitSurface);
         //			Overlap.DrawSprite(", ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
      } else {
         if (Object3DLoad[Set]) {
            if (MainObject.Chose == 1) {
               MainObject.Standing = ST_ATTACK;
               if (!(MainObject.AttSpeed % MainObject.nbSprite4Att))
                  MainObject.AttFrame++;
               if (MainObject.AttFrame == VObject3D[Set]->FacesA) {
                  MainObject.Chose = 0;
                  MainObject.AttFrame = 0;
               }
               if (MainObject.Chose == 1 && Flag == true && !bNewInt) {
                  BOOL boOutline = FALSE;
                  DWORD OutColor=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                  
                  if (TargetID == Player.ID) 
                  {
                     boOutline = TRUE;
                     OutColor = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 100 / Player.MaxHp));
                  }
                  if(boOutline && g_SaveGame.GetOptionsParam()->bShowNewLife)
                  {
                     boOutline = FALSE;
                  }
                  if( IsRangedObject( &MainObject ) )
                  {
                     VObject3D[Set]->DrawSprite3DR(MainObject.Direction, MainObject.AttFrame, ViewWidth*32-48, ViewHeight*16-24+37, NULL, boOutline, OutColor, lpBlitSurface, 0, &MainObject);
                  }
                  else
                  {
                     VObject3D[Set]->DrawSprite3DA(MainObject.Direction, MainObject.AttFrame, ViewWidth*32-48, ViewHeight*16-24+37, NULL, boOutline, OutColor, lpBlitSurface, 0, &MainObject);
                  }
                  
                  
                  RECT r;
                  r.left = 0;
                  r.right = g_Global.GetScreenWDiv2();
                  r.top = 0;
                  r.bottom = g_Global.GetScreenH();
                  if (MainObject.DrawOverlap)
                     Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, &r, NULL, NULL, lpBlitSurface);
               }
               MainObject.AttSpeed++;
            } else																					
               if (Draw) {
                  if (VObject3D[Set]->Type == 1) {
                     if (!(MainObject.Speed % MainObject.nbSprite4Move)) {
                        if (MainObject.SpriteNumber == 1)
                           MainObject.SpriteMove = 1;
                        else if (MainObject.SpriteNumber == VObject3D[Set]->Faces)
                           MainObject.SpriteMove = -1;
                        
                        MainObject.SpriteNumber += MainObject.SpriteMove;
                     }
                     MainObject.Speed++;
                  } else if (VObject3D[Set]->Type == 0) {
                     if (!(MainObject.Speed % MainObject.nbSprite4Move)) {
                        if (MainObject.SpriteNumber == VObject3D[Set]->Faces)
                           MainObject.SpriteNumber = 0;
                        
                        MainObject.SpriteNumber += MainObject.SpriteMove;
                     }
                     MainObject.Speed++;
                  }
                  MainObject.Standing = ST_MOVING;
               } else {
                  if (MainObject.Standing == ST_ATTACK || MainObject.Standing == ST_ATTACK_STANDING) 
                     MainObject.Standing = ST_ATTACK_STANDING;
                  else 
                     MainObject.Standing = ST_STANDING;
               }
               if (MainObject.Chose != 1 && Flag == true) {
                  if (!bNewInt) {
                     BOOL boOutline = FALSE;
                     DWORD OutColor=0;//BLBLBL 29 mars 2009 ajout d'une initialisation à 0
                     if (TargetID == Player.ID) 
                     {
                        boOutline = TRUE;
                        OutColor = ((Player.MaxHp == 0) ? 0 : (Player.Hp * 100 / Player.MaxHp));
                     }
                     if(boOutline && g_SaveGame.GetOptionsParam()->bShowNewLife)
                     {
                        boOutline = FALSE;
                     }
                     VObject3D[Set]->DrawSprite3D(MainObject.Direction, MainObject.SpriteNumber-1, (ViewWidth*32)-48, (ViewHeight*16)-24+37, MainObject.Standing, NULL, boOutline, OutColor, lpBlitSurface, 0, &MainObject);
                     if (MainObject.DrawOverlap)
                        Overlap.DrawSprite(ViewWidth*32+XCor, ViewHeight*16+YCor, NULL, NULL, NULL, lpBlitSurface);
                  }
               }
         }
      }
      SetGrid(NULL);
   } 
   // BLOCK 1 START
   }
   
   // Draw the overhead texts.
   ToHead();
   while(QueryNext() && bDrawText) 
   {
      Object = GetObject();

      int dwOffsetZoomX = 0;
      int dwOffsetZoomY = 0;
      int dwZoomStatus = g_Global.GetZoomStatus();
      if(dwZoomStatus > 0 && (Object->IsTalkTextDrawn() || Object->IsNameDrawn())) 
      {
         /**/

         int xp = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+16;
         int yp = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY+Object->TextCorrection;

         int dwReelX1 =   g_Global.GetScreenW()- (g_Global.GetScreenW()*(5*dwZoomStatus)/100);
         int dwReelY1 =   g_Global.GetScreenH()- (g_Global.GetScreenH()*(5*dwZoomStatus)/100);

		   int dwOffsetXZoom = (g_Global.GetScreenW()-dwReelX1)/2;
		   int dwOffsetYZoom = (g_Global.GetScreenH()-dwReelY1)/2;

         
         if(xp < g_Global.GetScreenWDiv2())
         {
            int dwPtsTmp = xp - dwOffsetXZoom;
            xp = (dwPtsTmp *g_Global.GetScreenWDiv2())/(dwReelX1/2);
         }
         else
         {
            int dwPtsTmp = xp - g_Global.GetScreenWDiv2();
            dwPtsTmp = (dwPtsTmp *g_Global.GetScreenWDiv2())/(dwReelX1/2);
            xp = dwPtsTmp +(g_Global.GetScreenWDiv2());
         }
         
         if(yp < g_Global.GetScreenHDiv2())
         {
            int dwPtsTmp = yp - dwOffsetYZoom;
            dwPtsTmp = (dwPtsTmp *g_Global.GetScreenHDiv2())/(dwReelY1/2);
            yp = dwPtsTmp;
         }
         else
         {
            int dwPtsTmp = yp - g_Global.GetScreenHDiv2();
            dwPtsTmp = (dwPtsTmp *g_Global.GetScreenHDiv2())/(dwReelY1/2);
            yp = dwPtsTmp +g_Global.GetScreenHDiv2();
         }

         if( Object->IsTalkTextDrawn() )
            Object->DrawTalkText(xp,yp);           
         else if(Object->IsNameDrawn())
            Object->DrawName(xp,yp);
         
      }
      else
      {
         if( Object->IsTalkTextDrawn() )
            Object->DrawTalkText( ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+16,((Object->OY+ViewHeight)<<4)+YCor+Object->MovY+Object->TextCorrection);           
         else if(Object->IsNameDrawn())
            Object->DrawName(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX+16,((Object->OY+ViewHeight)<<4)+YCor+Object->MovY+Object->TextCorrection);
      }
   }
   
   delete Clip;
   // BLOCK 1 END
   Unlock(1668);
}

      

/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::DrawObjectAnim(int XCor, int YCor, bool Draw, LPDIRECTDRAWSURFACE lpBlitSurface, bool Flag, bool bDrawText, bool bNewInt) 
{

   if(!g_SaveGame.GetOptionsParam()->bShowAnimDecorsLight)
      return;
   
   // Draw all the object to Screen.
   TFCObject *Object = NULL;
   bool NeverDraw = true;
   LPRECT Clip = new RECT;
   LPV2SPRITEFX VsfFX;
   bool childFound = false;
   
   int x, y;
   
   XCor -= 48;
   YCor += - 24 + 37;
   
   if (!bDrawText) 
   {
      //Sort();
      if (!Flag) 
      {
         Clip->left = 0;
         Clip->right = g_Global.GetScreenW()+64;  // 704 Avant with * 2 tiles de 32 de large
         Clip->top = 0;
         Clip->bottom = g_Global.GetScreenH()+32; // 512 Avant height * 2 tiles de 16 de haut;
      } 
      else 
      {
         Clip->left = 0;
         Clip->right = g_Global.GetScreenW();
         Clip->top = 0;
         Clip->bottom = g_Global.GetScreenH();
      }
      
      Grid.Fill(0);		
      GridID.Fill(0);
      GridPriority.Fill(INFINITE);
      GridBlocking.Fill(0);
      CreateComponent(VsfFX, V2SPRITEFX);

      for (int MultiPass = 0; MultiPass < 2; MultiPass++) 
      {
         ToHead();
         while(QueryNext()) 
         {
            Object = GetObject();
            childFound = false;
            if (!((Object->Type == __PLAYER_HUMAN_PUPPET || Object->Type == __PLAYER_HUMAN_FEMALE) && !Object->KnownPuppet)) 
            {
               if (Object->Dynamic == Flag) 
               {
                  BOOL boOutline = FALSE;
                  
                  if (!bNewInt && !childFound) 
                  {
                     VsfFX->lpClipRect = Clip;
                     VsfFX->lpDirectDrawSurface = lpBlitSurface;
                     //VsfFX->dwFX = NULL;
                     
                     if ((Object->Type == __OBJANIM_FONTAIN_02 ||
                         Object->Type == __OBJANIM_MOULIND    ||
                         Object->Type == __OBJANIM_MOULING)    && MultiPass == 1) 
                     {
                        //On dessine la fontaine...
                        int dwLoopFrame;
                        //if(Object->Type == __OBJANIM_FONTAIN_02)
                           dwLoopFrame = (Object->Faces/(2*FRAMING32) % Object->Anim) + Object->Type;
                        //else
                        //   dwLoopFrame = (Object->Faces % Object->Anim) + Object->Type;

                        

                        //int dwLoopFrame = Object->Type;
                        GetVSFObject(dwLoopFrame)->DrawSpriteN(((Object->OX+ViewWidth)<<5)+XCor+Object->MovX, ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY-21, VsfFX);

                        
                        int dwW  = GetVSFObject(dwLoopFrame)->GetWidth();
                        int dwH  = GetVSFObject(dwLoopFrame)->GetHeight();
                        int dwOX = GetVSFObject(dwLoopFrame)->GetCX();
                        int dwOY = GetVSFObject(dwLoopFrame)->GetCY();
                        int dwposX = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX;
                        int dwposY = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY-21;

                        RECT rc;
                        rc.left    = dwposX + dwOX;
                        rc.right   = rc.left+ dwW;
                        rc.top     = dwposY + dwOY;
                        rc.bottom  = rc.top + dwH;

                        
                        Object->Faces++;

                        
                        int a1 = 0,b1 = 0;
                        if (World.Done) 
                        {
                           if (World.MovY == -1)
                              b1 = 16;
                           if (World.MovY == 1)
                              b1 = -16;
                           if (World.MovX == -1)
                              a1 = 32;
                           if (World.MovX == 1)
                              a1 = -32;
                        } 
                        
                        int dwY2 =  ((Object->OY+Object->OC+ViewHeight)*16)+b1;
                        int dwX2 =  ((Object->OX+ViewWidth)*32)+a1;
                        
                        World.DecorDrawOverlap(Object->OX+ViewWidth,Object->OY+ViewHeight+Object->OC, XCor, YCor, dwX2,dwY2, lpBlitSurface, 2, NULL,Object->ID,&rc);
                        //Object->DrawOverlap = FALSE;
                        
                        //World.Restore();
                     } 
                     
                  }
               }
            }
         } 
         SetGrid(Object);
      }
   }
   DeleteComponent(VsfFX); // GELAGE 1
   delete Clip;
   // BLOCK 1 END
}


void VisualObjectList::DrawObjectAnimOverLap(int XCor, int YCor, LPDIRECTDRAWSURFACE lpBlitSurface) 
{
   // Draw all the object to Screen.
   Lock(1669);
   TFCObject *Object = NULL;
   bool NeverDraw = true;
   
   int x, y, i;
   
   XCor -= 48;
   YCor += - 24 + 37;

   for(i=0;i<m_sAnimOverlap.size();i++)
   {
      if(m_sAnimOverlap[i].uiIDSprite == __OBJANIM_FONTAIN_02 ||
         m_sAnimOverlap[i].uiIDSprite == __OBJANIM_MOULIND    ||
         m_sAnimOverlap[i].uiIDSprite == __OBJANIM_MOULING       )
      {
         BOOL bFound = FALSE;
         ToHead();
         while(!bFound && QueryNext()) 
         {
            Object = GetObject();
            if (Object->Type == __OBJANIM_FONTAIN_02 ||
                Object->Type == __OBJANIM_MOULIND    ||
                Object->Type == __OBJANIM_MOULING       )
            {
               bFound = TRUE;

               int dwX = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX     ;//+ m_sAnimOverlap[i].ObjX;
               int dwY = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY-21 ;//+ m_sAnimOverlap[i].ObjY;

               RECT rc;
               rc.left    = m_sAnimOverlap[i].rcClip.left   +dwX-m_sAnimOverlap[i].ObjX;
               rc.right   = m_sAnimOverlap[i].rcClip.right  +dwX-m_sAnimOverlap[i].ObjX;
               rc.top     = m_sAnimOverlap[i].rcClip.top    +dwY-m_sAnimOverlap[i].ObjY;
               rc.bottom  = m_sAnimOverlap[i].rcClip.bottom +dwY-m_sAnimOverlap[i].ObjY;

               GetVSFObject(((Object->Faces-1)/2 % Object->Anim) + Object->Type)->DrawSprite(dwX,dwY, &rc,NULL,NULL, lpBlitSurface,0,0,FALSE);
            } 
         }
      }
   }
   Unlock(1669);
}

void VisualObjectList::DrawObjectAnimOverLapID(int ID,bool bOffset, int XCor, int YCor,LPDIRECTDRAWSURFACE lpBlitSurface)
{
   // Draw all the object to Screen.
   Lock(1669);
   TFCObject *Object = NULL;
  
   int x, y;
   
   if(bOffset)
   {
      XCor -= 48;
      YCor += - 24 + 37;
   }

   switch(m_sAnimOverlap[ID].uiIDSprite)
   {
      case __OBJANIM_FONTAIN_02:
      case __OBJANIM_MOULIND:
      case __OBJANIM_MOULING:
      {
         BOOL bFound = FALSE;
         SavePointer();
         ToHead();
         while(!bFound && QueryNext()) 
         {
            Object = GetObject();
            if (Object->Type == __OBJANIM_FONTAIN_02 ||
                Object->Type == __OBJANIM_MOULIND    ||
                Object->Type == __OBJANIM_MOULING      )

            {
               bFound = TRUE;

               int dwX = ((Object->OX+ViewWidth)<<5)+XCor+Object->MovX     ;//+ m_sAnimOverlap[i].ObjX;
               int dwY = ((Object->OY+ViewHeight)<<4)+YCor+Object->MovY-21 ;//+ m_sAnimOverlap[i].ObjY;

               RECT rc;
               rc.left    = m_sAnimOverlap[ID].rcClip.left   +dwX-m_sAnimOverlap[ID].ObjX;
               rc.right   = m_sAnimOverlap[ID].rcClip.right  +dwX-m_sAnimOverlap[ID].ObjX;
               rc.top     = m_sAnimOverlap[ID].rcClip.top    +dwY-m_sAnimOverlap[ID].ObjY;
               rc.bottom  = m_sAnimOverlap[ID].rcClip.bottom +dwY-m_sAnimOverlap[ID].ObjY;

               
               GetVSFObject(((Object->Faces-1)/2 % Object->Anim) + Object->Type)->DrawSprite(dwX,dwY, &rc,NULL,NULL, lpBlitSurface,0,0,TRUE);

               int a1 = 0,b1 = 0;
               if (World.Done) 
               {
                  if (World.MovY == -1)
                     b1 = 16;
                  if (World.MovY == 1)
                     b1 = -16;
                  if (World.MovX == -1)
                     a1 = 32;
                  if (World.MovX == 1)
                     a1 = -32;
               }
               
               int dwY2 =  ((Object->OY+Object->OC+ViewHeight)*16)+b1;
               int dwX2 =  ((Object->OX+ViewWidth)*32)+a1;
               
               
               World.DecorDrawOverlapID(Object->OX+ViewWidth,Object->OY+ViewHeight+Object->OC, XCor, YCor, dwX2,dwY2, lpBlitSurface, 2, NULL,Object->ID,&rc);
               //Object->DrawOverlap = FALSE;
               //World.Restore();

               
            } 
         }
         RestorePointer();
      }

      break;
   }
   Unlock(1669);
}



/////////////////////////////////////////////////////////////////////////////
unsigned short VisualObjectList::Found(signed char X, signed char Y) {
    // Return a type from a position
    ToTail();
    
    TFCObject *Object;
    while(QueryPrevious()) {
        Object = GetObject();
        if (Object->OX == X && Object->OY == Y) { 
            SetQueryState(STOP_QUERY);
            return Object->Type;
        }
    }
    
    return 0;
}

BOOL VisualObjectList::CheckDead(DWORD ID) {
    ToHead();
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) { 
            if ((Object->Type >= 15000 && Object->Type <= 20000) || (Object->Type >= 25000 && Object->Type <= 30000) || Object->Killed)
                return FALSE;
            else 
                return TRUE;
        }
    }
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
BOOL VisualObjectList::FoundID(unsigned long ID, bool includeDead) {
    ToHead();
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) { 
            if ((Object->Type > 15000 && Object->Type < 20000 || Object->Type > 25000 && Object->Type < 30000) && !includeDead) {
                SetQueryState(STOP_QUERY);
                return FALSE;
            } else {
                SetQueryState(STOP_QUERY);
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

BOOL VisualObjectList::FoundChild(unsigned long ID) {
    ToHead();
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->spellEffectID == ID) { 
            return TRUE;
        }
    }
    
    return FALSE;
}

void VisualObjectList::Dither(void) {
    ToHead();
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->Type != __OBJGROUP_WOODEN_DOOR_CLOSED  && Object->Type != __OBJGROUP_WOODEN_DOOR_OPENED  && Object->Type != __OBJGROUP_WOODEN_DOOR_CLOSED_I  && Object->Type != __OBJGROUP_WOODEN_DOOR_OPENED_I   &&
            Object->Type != __OBJGROUP_WOODEN_DOOR2_CLOSED && Object->Type != __OBJGROUP_WOODEN_DOOR2_OPENED && Object->Type != __OBJGROUP_WOODEN_DOOR2_CLOSED_I && Object->Type != __OBJGROUP_WOODEN_DOOR2_OPENED_I  &&
            Object->Type != __OBJGROUP_WOODEN_DOOR3_CLOSED && Object->Type != __OBJGROUP_WOODEN_DOOR3_OPENED && Object->Type != __OBJGROUP_WOODEN_DOOR3_CLOSED_I && Object->Type != __OBJGROUP_WOODEN_DOOR3_OPENED_I  &&
            Object->Type != __OBJGROUP_WOODEN_DOOR4_CLOSED && Object->Type != __OBJGROUP_WOODEN_DOOR4_OPENED && Object->Type != __OBJGROUP_WOODEN_DOOR4_CLOSED_I && Object->Type != __OBJGROUP_WOODEN_DOOR4_OPENED_I  &&
            Object->Type != __OBJGROUP_WOODEN_DOOR5_CLOSED && Object->Type != __OBJGROUP_WOODEN_DOOR5_OPENED && Object->Type != __OBJGROUP_WOODEN_DOOR5_CLOSED_I && Object->Type != __OBJGROUP_WOODEN_DOOR5_OPENED_I  &&
            Object->Type != __OBJGROUP_WOODEN_DOOR6_CLOSED && Object->Type != __OBJGROUP_WOODEN_DOOR6_OPENED && Object->Type != __OBJGROUP_WOODEN_DOOR6_CLOSED_I && Object->Type != __OBJGROUP_WOODEN_DOOR6_OPENED_I  &&
            Object->Type != __OBJGROUP_WOODEN_DOOR7_CLOSED && Object->Type != __OBJGROUP_WOODEN_DOOR7_OPENED && Object->Type != __OBJGROUP_WOODEN_DOOR7_CLOSED_I && Object->Type != __OBJGROUP_WOODEN_DOOR7_OPENED_I  &&
            Object->Type != __OBJGROUP_WOODEN_DOOR8_CLOSED && Object->Type != __OBJGROUP_WOODEN_DOOR8_OPENED && Object->Type != __OBJGROUP_WOODEN_DOOR8_CLOSED_I && Object->Type != __OBJGROUP_WOODEN_DOOR8_OPENED_I  
            ) 
        {
            Object->Overlap.Dither = g_SaveGame.GetOptionsParam()->bDitherGraph;
        }
    }
    Overlap.Dither = g_SaveGame.GetOptionsParam()->bDitherGraph;
}
unsigned long VisualObjectList::FoundType(short Type) {
    ToHead();
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->Type == Type) { 
            SetQueryState(STOP_QUERY);
            return Object->ID;
        }
    }
    
    return 0;
}

unsigned long VisualObjectList::FoundID2(signed char X, signed char Y) {
    // return an ID from a position.
    ToTail();
    
    TFCObject *Object;
    while(QueryPrevious()) {
        Object = GetObject();
        if (Object->OX == X && Object->OY == Y && Object->Type <= 30000) { 
            SetQueryState(STOP_QUERY);
            return Object->ID;
        }
    }
    
    return 0;
}

void VisualObjectList::SetExtended(signed char X, signed char Y) {
    // return an ID from a position.
    ToTail();
    
    TFCObject *Object;
    while(QueryPrevious()) {
        Object = GetObject();
        if (Object->OX == X && Object->OY == Y) { 
            SetQueryState(STOP_QUERY);
            
            if (Object->Extended)
                Object->Extended = false;
            else
                Object->Extended = true;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::SetView(int Width, int Height) {
    // Set the View Width and Height
    ViewWidth = Width/2+1;
    ViewHeight = Height/2+1;
}

/////////////////////////////////////////////////////////////////////////////
void VisualObjectList::SetRange(int Width, int Height) {
    RangeWidth = Width;
    RangeHeight = Height;
}

/////////////////////////////////////////////////////////////////////////////
int VisualObjectList::Modify(signed char X, signed char Y) {
    return 1;
}	

void VisualObjectList::Text(unsigned long ID, char *TalkingText, unsigned int Color, unsigned char Direction) {
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
            //delete Object->TalkingText;
            //Object->TalkingText = new char [strlen(TalkingText)+1];
            //strcpy(Object->TalkingText, TalkingText);
            Object->SetTalkText( TalkingText, Color );
            //Object->Offset = 0;
            Object->Temp = timeGetTime();
            if (Direction) {
                Deplacement *Moving = new Deplacement;
                ZeroMemory(Moving, sizeof(Deplacement));
                Moving->ID = Object->ID;
                Moving->Type = 4;
                Moving->Direction = Direction;
                Object->MovingQueue.AddToQueue(Moving);
                //            Object->Dynamic = true;
                //            World.ChangeStatic();
            }
            return;
        }
    }
}

void VisualObjectList::NoIdentify(unsigned long ID) {
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
            Object->StopNameDisplay();
            //delete Object->IdentifyText;
            //Object->IdentifyText = new char [1];
            //Object->IdentifyText[0] = 0;
            return;
        }
    }
}

void VisualObjectList::Identify(unsigned long ID) 
{
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) 
    {
        Object = GetObject();
        if (Object->ID == ID) 
        {
           /*
           char str[300];
           sprintf(str,"friendly == [%d]\n",Object->Friendly);
           OutputDebugString(str);
           */
            if (Object->Name)//Si on a le nom, on l'affiche...
            {
                Object->StopTalkText();//BLBL bougé ici, c'est plus logique de masquer le texte avant d'en mettre un autre
				Object->DisplayName();
                //delete Object->IdentifyText;
                //Object->IdentifyText = new char [strlen(Object->Name)+1];
                //strcpy(Object->IdentifyText, Object->Name);
                Object->IdentifyTemp = timeGetTime();//initialisation du compteur, on affiche le nom 3.5 Secondes...
                //Object->StopTalkText();//BLBLBL déplacé cette ligne ^^^^
            } 
            else //Autrement on envoie un paquet de demande d'identification :
            {
                if (timeGetTime() - LastAttempt > 1000) 
                {
                    LastAttempt = timeGetTime();

				//	ChatterUI::GetInstance()->AddBackscrollItem("","Asking server what is that item you can see...",RGB(0,100,255),true);//BLBLBL un peu de traces
                    
                    TFCPacket Send; 
                    
                    Send << (short)35;//RQ_GetUnitName
                    Send << (long)Object->ID;
                    Send << (short)(Object->OX+Player.xPos);
                    Send << (short)(Object->OY+Player.yPos);
                    
                    SEND_PACKET(Send);					
                }
            }
        }
    }
}

void VisualObjectList::IdentifyAll() 
{
   if (timeGetTime() - LastAttemptIDAll > 2000)  //jamais au plus de 2 sec...
   {
      LastAttemptIDAll = timeGetTime();
      Lock(0);
      
      ToHead();
      //g_uiNBRList = 0;
      TFCObject *Object;
      while(QueryNext()) 
      {
         Object = GetObject();
         if (Object->Name) 
         {
            Object->DisplayName();
            Object->IdentifyTemp = timeGetTime();
            Object->StopTalkText();
         } 
         else 
         {
            
//			ChatterUI::GetInstance()->AddBackscrollItem("","Asking server what is that item you can see...",RGB(0,100,255),true);//BLBLBL un peu de traces
			  
			TFCPacket Send; 
            
            Send << (short)35;//RQ_GetUnitName
            Send << (long)Object->ID;
            Send << (short)(Object->OX+Player.xPos);
            Send << (short)(Object->OY+Player.yPos);
            SEND_PACKET(Send);
			
         }
      }      
      Unlock(0);
   }
}



void VisualObjectList::Anim(unsigned long ID, char Type) {
    ToHead();
    
    if (Type) {
        TFCObject *Object;
        while(QueryNext()) {
            Object = GetObject();
            if (Object->ID == ID) {
                Object->Chose = 1; // Type
                Object->Speed = 0; // @PoPo
                Object->AttFrame = 0;
                Object->AttSpeed = 0;
            }
        }
    }
}

int VisualObjectList::RealPos(signed char X, signed char Y, signed char *NX, signed char *NY) {
    if (GridID(X+11, Y+16) == 0)
        return 0;
    
    Found(NX, NY, GridID(X+11, Y+16));
    return 1;
}

void VisualObjectList::SetGrid(TFCObject *Object) {
    int i, j;
    
    int Priority = 0;
    int MiddleX;
    int MiddleY;
    
    MousePos screenOffset = RootBoxUI::GetInstance()->GetRootScreenOffset();
    screenOffset.x = abs( screenOffset.x ) / 64;
    screenOffset.y = -abs( screenOffset.y ) / 32;
    if (Object) {
        
        
        MiddleX = -Object->Left + ((Object->Right + Object->Left) / 2) + (((Object->Right + Object->Left) & 1) ? 1 : 0);
        MiddleY = -Object->Up + ((Object->Down + Object->Up) / 2);
        for (j = -Object->Left; j < Object->Right + 1; j++) {
            for (i = -Object->Up; i < Object->Down; i++) {
                
                
                Priority = abs(j-MiddleX) + abs((i-MiddleY)/(Object->Type > 10000 ? 5 : 2)); 
                if (GridPriority(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight+i+1) >= Priority) {
                    // Better priority
                    if (((Object->MouseCursor != TALK && Object->MouseCursor) || Player.SetGridMode) && !(Object->MouseCursor == 2 && Player.SetGridMode == 2)) {
                        // If it is a Player or if the SetGridMode == 1
                        Grid(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight+i+1) = Object->MouseCursor;
                        GridID(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight+i+1) = Object->ID;
                        GridPriority(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight+i+1) = Priority;
                    } else if (Grid(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight + i+1) != ATTACK && Player.SetGridMode != 2) {
                        // Test to see if the old cussor was an attack one or if the SetGridMode == 2
                        Grid(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight+i+1) = Object->MouseCursor;
                        GridID(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight+i+1) = Object->ID;
                        GridPriority(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight+i+1) = Priority;
                    } else if (GridID(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight + i+1) == 0) {
                        Grid(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight+i+1) = Object->MouseCursor;
                        GridID(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight+i+1) = Object->ID;
                        GridPriority(screenOffset.x + Object->OX + j + ViewWidth, screenOffset.y + Object->OY + ViewHeight+i+1) = Priority;
                    }
                }
            }
        }
        
        
        if ((Object->Type > 10000 && Object->Type < 15000) || (Object->Type > 20000 && Object->Type < 25000)) {
            switch (Object->Type) {
            /*case KRAANIAN: {
            GridBlocking(Object->OX + ViewWidth-1, Object->OY + ViewHeight-1) = 1;
            } break; */
                
            case __PLAYER_HUMAN_PUPPET: break;
            case __PLAYER_HUMAN_FEMALE: break;
                
            default: 
                GridBlocking(Object->OX + ViewWidth-1, Object->OY + ViewHeight-1) = 1;
            }
            /*} else if (Object->Type == __OBJGROUP_WELL_TALK) {
            GridBlocking(Object->OX + ViewWidth-1 - 1, Object->OY + ViewHeight-1 - 2) = 1;
            GridBlocking(Object->OX + ViewWidth-1,     Object->OY + ViewHeight-1 - 2) = 1;
            GridBlocking(Object->OX + ViewWidth-1 + 1, Object->OY + ViewHeight-1 - 2) = 1;
            GridBlocking(Object->OX + ViewWidth-1 + 2, Object->OY + ViewHeight-1 - 2) = 1;
            GridBlocking(Object->OX + ViewWidth-1 - 2, Object->OY + ViewHeight-1 - 1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 - 1, Object->OY + ViewHeight-1 - 1) = 1;
            GridBlocking(Object->OX + ViewWidth-1,     Object->OY + ViewHeight-1 - 1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 + 1, Object->OY + ViewHeight-1 - 1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 + 2, Object->OY + ViewHeight-1 - 1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 - 2, Object->OY + ViewHeight-1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 - 1, Object->OY + ViewHeight-1) = 1;
            GridBlocking(Object->OX + ViewWidth-1,     Object->OY + ViewHeight-1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 + 1, Object->OY + ViewHeight-1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 + 2, Object->OY + ViewHeight-1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 - 2, Object->OY + ViewHeight-1 + 1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 - 1, Object->OY + ViewHeight-1 + 1) = 1;
            GridBlocking(Object->OX + ViewWidth-1,     Object->OY + ViewHeight-1 + 1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 + 1, Object->OY + ViewHeight-1 + 1) = 1;
            GridBlocking(Object->OX + ViewWidth-1 + 2, Object->OY + ViewHeight-1 + 1) = 1; */
        }
    } else {
        // Player Set Grid.
        MiddleX = 0;
        MiddleY = -3;
        
        for (j = -1; j < 1; j++) {
            for (i = -7; i < 2; i++) {
                Priority = abs(j) + abs((i+3)/5); 
                if (GridPriority(j + ViewWidth, ViewHeight+i+1) >= Priority) {
                    // Better priority
                    if (((MainObject.MouseCursor != TALK && MainObject.MouseCursor) || Player.SetGridMode) && !(MainObject.MouseCursor == 2 && Player.SetGridMode == 2)) {
                        // If it is a Player or if the SetGridMode == 1
                        Grid(screenOffset.x + j + ViewWidth, screenOffset.y + ViewHeight+i+1) = MainObject.MouseCursor;
                        GridID(screenOffset.x + j + ViewWidth, screenOffset.y + ViewHeight+i+1) = Player.ID;
                        GridPriority(screenOffset.x + ViewWidth, screenOffset.y + ViewHeight+i+1) = Priority;
                    } else if (Grid(screenOffset.x + j + ViewWidth, screenOffset.y + ViewHeight + i+1) != ATTACK && Player.SetGridMode != 2) {
                        // Test to see if the old cussor was an attack one or if the SetGridMode == 2
                        Grid(screenOffset.x + j + ViewWidth, screenOffset.y + ViewHeight+i+1) = MainObject.MouseCursor;
                        GridID(screenOffset.x + j + ViewWidth, screenOffset.y + ViewHeight+i+1) = Player.ID;
                        GridPriority(screenOffset.x + j + ViewWidth, screenOffset.y + ViewHeight+i+1) = Priority;
                    } else if (GridID(screenOffset.x + j + ViewWidth, screenOffset.y + ViewHeight + i+1) == 0) {
                        Grid(screenOffset.x + j + ViewWidth, screenOffset.y + ViewHeight+i+1) = MainObject.MouseCursor;
                        GridID(screenOffset.x + j + ViewWidth, screenOffset.y + ViewHeight+i+1) = Player.ID;
                        GridPriority(screenOffset.x + j + ViewWidth,  screenOffset.y + ViewHeight+i+1) = Priority;
                    }
                }
            }
        }
    }
}

struct BLOCK {
    int nWidth;
    int nHeight;
    int nCenterX;
    int nCenterY;
    char *lpbData;
};

class TOTALBLOCK {
public:
    BLOCK Angle[10];
    
    void Create(int x0, int y0, char *d0, int x1, int y1, char *d1, int x2, int y2, char *d2, 
        int x3, int y3, char *d3, int x4, int y4, char *d4) {
        MakeAngle(0, x0, y0, d0, 0);
        MakeAngle(1, x1, y1, d1, 0);
        MakeAngle(2, x2, y2, d2, 0);
        MakeAngle(3, x3, y3, d3, 0);
        MakeAngle(4, x4, y4, d4, 0);
        MakeAngle(5, x1, y1, d1, 1);
        MakeAngle(6, x2, y2, d2, 1);
        MakeAngle(7, x3, y3, d3, 1);
        
    }
    
    void MakeAngle(int a, int x, int y, char *d, int i) {
        Angle[a].nWidth  = x ;
        Angle[a].nHeight = y;
        
        for (int j = 0; j < y; j++) {
            for (int i = 0; i < x; i++) {
                if (d[i+j*x] == 'o') {
                    if (i) {
                        Angle[a].nCenterX = y-i;
                        Angle[a].nCenterY = x-j;
                    } else {
                        Angle[a].nCenterX = i;
                        Angle[a].nCenterY = j;
                    }
                    i = x;
                    j = y;
                }
            }
        }
        
        if (i) {
            Angle[a].lpbData = d;
        } else {
            Angle[a].lpbData = d;
        }
    }
};


void Test(void) {
    char Kraanian0[] = {
        'x',
            'o'
    };
    char Kraanian45[] = {
        'x', 'x', ' ',
            'x', 'o', 'x'
    };
    char Kraanian90[] = {
        'x', 'x', 'x',
            'x', 'o', 'x'
    };
    char Kraanian135[] = {
        'x', 'x', ' ',
            ' ', 'o', 'x'
    };
    char Kraanian180[] = { 
        'x',
            'o',
    };
    
    TOTALBLOCK KraanianBlocking;
    
    KraanianBlocking.Create(1, 2, Kraanian0,
        3, 2, Kraanian45,
        3, 2, Kraanian90,
        3, 2, Kraanian135,
        1, 2, Kraanian180);
    
    
    
    
    
}

void VisualObjectList::SetSet(BOOL kP) {
    
    Overlap.Dither = g_SaveGame.GetOptionsParam()->bDitherGraph;
    switch (MainObject.Type) 
    {
        
    case __MONSTER_HUMAN_SWORDMAN:    Set = 11; break;
    case __PLAYER_HUMAN_PUPPET:      Set = 71; break;
    case __PLAYER_HUMAN_FEMALE:      Set = 71; break;
    case __MONSTER_HUMAN_MAGE:        Set = 1;  break;
    case __MONSTER_HUMAN_PRIEST:      Set = 12; break;
    case __MONSTER_HUMAN_THIEF:       Set = 6;  break;
    case __NPC_HUMAN_PEASANT:        Set = 13; break;
    case __NPC_HUMAN_GUARD:          Set = 14; break;
    case __NPC_HUMAN_RED_MAGE:       Set = 15; break;
    case __NPC_HUMAN_PAYSANNE:       Set = 16; break;
    case __NPC_HUMAN_PRIEST:         Set = 17; break;
    case __NPC_HUMAN_WARRIOR:        Set = 23; break;
    case __MONSTER_RED_GOBLIN:       Set = 2;  break;
    case __MONSTER_DRACONIAN_PLATE:  Set = 117;  break;
    case __MONSTER_DRACONIAN_ROBE:  Set = 118;  break;
    case __MONSTER_DRACONIAN_LEATHER:  Set = 119;  break;
    case __MONSTER_SKELETON_CENTAUR: Set = 114;  break;
    case __MONSTER_SKELETON_SERVANT_1: Set = 105;  break;
    case __MONSTER_SKELETON_SERVANT_2: Set = 106;  break;
    case __MONSTER_SKELETON_KING: Set = 107;  break;
    case __MONSTER_LICH: Set = 108;  break;
    case __MONSTER_CENTAUR_WARRIOR:  Set = 101;  break;
    case __MONSTER_CENTAUR_ARCHER:   Set = 102;  break;
    case __MONSTER_CENTAUR_SHAMAN:   Set = 103;  break;
    case __MONSTER_CENTAUR_KING:     Set = 104;  break;
    case __MONSTER_SKAVEN_PEON:      Set = 97;  break;
    case __MONSTER_SKAVEN_SHAMAN:    Set = 98;  break;
    case __MONSTER_SKAVEN_SKAVENGER: Set = 99;  break;
    case __MONSTER_SKAVEN_WARRIOR:   Set = 100;  break;
    case __MONSTER_SKAVEN_PEON2:     Set = 110;  break;
    case __MONSTER_SKAVEN_SHAMAN2:   Set = 111;  break;
    case __MONSTER_SKAVEN_SKAVENGER2:Set = 112;  break;
    case __MONSTER_SKAVEN_WARRIOR2:  Set = 113;  break;
    case __MONSTER_RED_GOBLINBOSS:   Set = 76;  break;
    case __MONSTER_BAT:              Set = 8;  break;
    case __MONSTER_RAT:              Set = 7;  break;
    case __MONSTER_KOBOLD:           Set = 18; break;
    case __MONSTER_OOZE:             Set = 19; break;
    case __MONSTER_BRIGAND:          Set = 24; break;
    case __MONSTER_SPIDER:           Set = 9;  break;
    case __MONSTER_ORC:              Set = 20; break;
    case __MONSTER_ZOMBIE:           Set = 21; break;
    case __MONSTER_WOLF:             Set = 77; break;
    case __MONSTER_DARKWOLF:         Set = 86; break;
    case __MONSTER_GREEN_TROLL:      Set = 22; break;
    case __MONSTER_MUMMY:            Set = 3;  break;
    case __MONSTER_KRAANIANFLYING:   Set = 65; break;
    case __MONSTER_TARANTULA:        Set = 70; break;
    case __MONSTER_AGMORKIAN:        Set = 72; break;
    case __MONSTER_KRAANIANTANK:     Set = 73; break;
    case __MONSTER_TAUNTING:         Set = 74; break;
    case __MONSTER_KRAANIANMILIPEDE: Set = 66; break;
    case __MONSTER_SKELETON:         Set = 10; break;
    case __MONSTER_DEMON:            Set = 4;  break;
    case __MONSTER_MINOTAUR:         Set = 5;  break;
    case __MONSTER_BEHOLDER:         Set = 71; break;
    case __MONSTER_SMALL_WORM:       Set = 25; break;
    case __MONSTER_BIG_WORM:         Set = 26; break;
    case __MONSTER_TREE_ENT:         Set = 27; break;
    case __MONSTER_SNAKE:            Set = 28; break;
    case __MONSTER_UNICORN:          Set = 29; break;
    case __MONSTER_CENTAUR:          Set = 30; break;
    case __MONSTER_HORSE:            Set = 31; break;
    case __MONSTER_PEGASUS:          Set = 32; break;
    case __MONSTER_SCORPION:         Set = 33; break;
    case __MONSTER_KRAANIAN:         Set = 34; break;
    case __MONSTER_ATROCITY:         Set = 35; break;
    case __MONSTER_ATROCITYBOSS:     Set = 75; break;
    case __MONSTER_NIGHTMARE:        Set = 36; break;
    case __MONSTER_DRAGON:           Set = 37; break;
    case __MONSTER_WASP:             Set = 38; break;
    case __MONSTER_DROMADARY:        Set = 67; break;
    case __MONSTER_PIG:              Set = 68; break;
    case __MONSTER_ORI:              Set = 69; break;
    case __MONSTER_SANTA_CLAUS:      Set = 116; break;
	//Dialsoft new code BEGIN
	case __MONSTER_DARKGOBLIN:		 Set = 120; break;
	case __MONSTER_BURNTREE:		 Set = 123; break;
	case __MONSTER_GROUNDMUMMY:		 Set = 124; break;
	case __MONSTER_BOSSRAT:	     	 Set = 125; break;
	case __MONSTER_BIGSKEL:	     	 Set = 126; break;
    case __MONSTER_DEMON_VIOLET:     Set = 128;  break;
	case __MONSTER_DEMON_BLUE:       Set = 129;  break;
	case __MONSTER_DEMON_GREEN:      Set = 130;  break;
	case __MONSTER_DEMON_LIGHTBLUE:  Set = 131;  break;
	case __MONSTER_DEMON_BLACK:      Set = 132;  break;
	case __MONSTER_DEMON_YELLOW:     Set = 133;  break;
	case __MONSTER_DEMON_WHITE:      Set = 134;  break;
	case __MONSTER_FRED_GOBLIN_VIOLET:     Set = 135;  break;
	case __MONSTER_FRED_GOBLIN_BLUE:       Set = 136;  break;
	case __MONSTER_FRED_GOBLIN_GREEN:      Set = 137;  break;
	case __MONSTER_FRED_GOBLIN_LIGHTBLUE:  Set = 138;  break;
	case __MONSTER_FRED_GOBLIN_GRAY:       Set = 139;  break;
	case __MONSTER_FRED_GOBLIN_YELLOW:     Set = 140;  break;
	case __MONSTER_FRED_GOBLIN_WHITE:      Set = 141;  break;
	case __MONSTER_FOOZE_DARKGREEN:        Set = 142;  break;
	case __MONSTER_FOOZE_DARKYELLOW:       Set = 143;  break;
	case __MONSTER_FOOZE_GREEN:            Set = 144;  break;
	case __MONSTER_FOOZE_RED:              Set = 145;  break;
	case __MONSTER_FOOZE_BLACK:            Set = 146;  break;
	case __MONSTER_FOOZE_YELLOW:           Set = 147;  break;
	case __MONSTER_FOOZE_WHITE:            Set = 148;  break;
	case __MONSTER_WHITECAT:               Set = 149;  break;
   case __MONSTER_DRAGONDARKGREEN:        Set = 150;  break;
	case __MONSTER_FGAMEMASTER:            Set = 151;  break;
   case __MONSTER_YETI:                   Set = 183;  break;
   case __MONSTER_SMALL_YETI:             Set = 184;  break;
   case __MONSTER_BLOB_R:                 Set = 187;  break;
   case __MONSTER_BLOB_G:                 Set = 188;  break;
   case __MONSTER_TARANTULA2:             Set = 189;  break;
   case __MONSTER_DARKGOBLIN_ICE:     	   Set = 190;  break;
   case __MONSTER_KRAANIAN_ICE:           Set = 191;  break;
   case __MONSTER_KRAANIANFLYING_ICE:     Set = 192;  break;
   case __MONSTER_KRAANIANTANK_ICE:       Set = 193;  break;
   case __MONSTER_ORC_ICE:                Set = 194;  break;
   case __MONSTER_KRAANIAN_POD_ICE:       Set = 195;  break;
   case __MONSTER_SKAVEN_PEON3:           Set = 196;  break;
   case __MONSTER_SKAVEN_SHAMAN3:         Set = 197;  break;
   case __MONSTER_SKAVEN_SKAVENGER3:      Set = 198;  break;
   case __MONSTER_SKAVEN_WARRIOR3:        Set = 199;  break;
   case __MONSTER_SDRAGONDARKGREEN:       Set = 200;  break;
   case __MONSTER_SDRAGONDARKRED  :       Set = 201;  break;
   case __MONSTER_SSDRAGONDARKGREEN:      Set = 202;  break;
   case __MONSTER_SSDRAGONDARKRED  :      Set = 203;  break;
   case __MONSTER_SSDRAGONDARKORANGE :    Set = 204;  break;
   case __MONSTER_BIGTARANTULA1:          Set = 205;  break;
   case __MONSTER_BIGTARANTULA2:          Set = 206;  break;
   case __MONSTER_BIGWOLF1:               Set = 209;  break;
   case __MONSTER_BIGWOLF2:               Set = 210;  break;
   case __MONSTER_BIGBIGSKEL:             Set = 211;  break;
   case __MONSTER_BIGBURNTREE:		      Set = 212;  break;
   case __MONSTER_BIGWASP:                Set = 213;  break;
   case __MONSTER_DEMONSKULL:             Set = 214;  break;
   case __MONSTER_TWIGLEE:                Set = 215;  break;
   case __MONSTER_OGRE:                   Set = 216;  break;
   case __MONSTER_GOLEM1:                 Set = 217;  break;
   case __MONSTER_GOLEM2:                 Set = 218;  break;
   case __MONSTER_GOLEM3:                 Set = 219;  break;

   case __MONSTER_DRAGON_RED:             Set = 220;  break;
   case __MONSTER_DRAGON_ORANGE:          Set = 221;  break;
   case __MONSTER_DRAGON_JAUNE:           Set = 222;  break;
   case __MONSTER_DRAGON_BLEU:            Set = 223;  break;
   case __MONSTER_DRAGON_GRIS:            Set = 224;  break;
   case __MONSTER_DRAGON_BLANC:           Set = 225;  break;
   case __MONSTER_DRAGON_NOIR:            Set = 226;  break;


		//Dialsoft new code - END
    }
    LoadObject(Set);
    //	if (!kP) {
    //		memset(MainObject.PuppetInfo, 0, 32);
    //		memset(MainObject.OldPuppetInfo, 255, 32);
    //	   MainObject.VisiblePart = 0;
    //	}
    if (Set == 71)
        VObject3D[Set]->SetPuppet(&MainObject);
    MainObject.SpriteNumber = 1;
    MainObject.SpriteMove = 1; //@PoPo
    
    Overlap.Release(); 
    //:CR_NMNMNM  Player overlap Size...
    Overlap.Create(VObject3D[Set]->MaxWidth+32, VObject3D[Set]->MaxHeight+16, TRUE, lpDXDirectDraw->wGMask, VObject3D[Set]->MaxxCor, VObject3D[Set]->MaxyCor);
    
}

void VisualObjectList::SetPuppet(int x) {
    memset(MainObject.PuppetInfo, x, 32);
    if (Set == 71)
        VObject3D[Set]->SetPuppet(&MainObject);
}

void Puppetize( TFCObject *Object, WORD BODY, WORD FEET, WORD GLOVES, WORD HELM, WORD LEGS, WORD W_RIGHT,
               WORD W_LEFT, WORD CAPE );

void VisualObjectList::SetPuppet(DWORD ID, WORD BODY, WORD FEET, WORD GLOVES, WORD HELM, 
                                 WORD LEGS, WORD W_RIGHT, WORD W_LEFT, WORD CAPE) {
    
    if (ID == Player.ID) 
    {
        memset(MainObject.PuppetInfo, 0, 32);
        Puppetize(&MainObject, BODY, FEET, GLOVES, HELM, LEGS, W_RIGHT, W_LEFT, CAPE);

        if(MainObject.Type == 10011 || MainObject.Type == 10012)
           VObject3D[71]->SetPuppet(&MainObject);
        SetQueryState(STOP_QUERY);
		if (MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_BLACKWINGS || 
			 MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_WHITEWINGS ||
			 MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_DARKWINGS  ||
			 MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_NEUTRALWINGS ||
			 MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_REDWINGS ||
			 MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_GOLDWINGS ||
			 MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_DARKREDWINGS ||
			 MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_DARKGOLDWINGS ||
			 MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_LIGHTBLUEWINGS ||
			 MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_ORANGEWINGS    ||
          
          //MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_BUTTERFLYWINGS1 ||  
          //MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_BUTTERFLYWINGS2 ||  
          //MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_BUTTERFLYWINGS3 ||  
          //MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_BUTTERFLYWINGS4 ||  
          MainObject.PuppetInfo[PUP_CAPE] == PUPEQ_ARCHWINGSW    
          ) 
      {
			 Player.isSeraph = true;
		}
    } else {
        ToHead();
        TFCObject *Object;
        while(QueryNext()) 
        {
            Object = GetObject();
            if (Object->ID == ID) 
            {
                Object->KnownPuppet = TRUE;
                memset(Object->PuppetInfo, 0, 32);
                Puppetize(Object, BODY, FEET, GLOVES, HELM, LEGS, W_RIGHT, W_LEFT, CAPE);
                if(Object->Type == 10011 || Object->Type == 10012)
                  VObject3D[71]->SetPuppet(Object);
                SetQueryState(STOP_QUERY);
            }
        }
    }
}

void Puppetize( TFCObject *Object, WORD BODY, WORD FEET, WORD GLOVES, WORD HELM, WORD LEGS, WORD W_RIGHT,
               WORD W_LEFT, WORD CAPE ) {
   switch (CAPE) 
   {
      case __OBJGROUP_GREENCAPE:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_GREENCAPE;
         break;
      case __OBJGROUP_YELLOWCAPE:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_YELLOWCAPE;
         break;
      case __OBJGROUP_SILVERCAPE:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_SILVERCAPE;
         break;
      case __OBJGROUP_PURPLECAPE:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_PURPLECAPE;
         break;
      case __OBJGROUP_BLACKCAPE:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_BLACKCAPE;
         break;
      case __OBJGROUP_AQUACAPE:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_AQUACAPE;
         break;
      case __OBJGROUP_BLUECAPE:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_BLUECAPE;
         break;
      
      case __OBJGROUP_REDCAPE:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_REDCAPE;
         break;
      case __OBJGROUP_SERAPH_BLACK_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_BLACKWINGS;
         break;
      case __OBJGROUP_SERAPH_WHITE_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_WHITEWINGS;
         break;
      case __OBJGROUP_SERAPH_DECHU_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_NM_DECHUWINGS;
         break;
      case __OBJGROUP_SERAPH_DECHU2_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_NM2_DECHUWINGS;
         break;
      case __OBJGROUP_DARKWINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_DARKWINGS;
         break;
      case __OBJGROUP_SERAPH_NEUTRAL_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_NEUTRALWINGS;
         break;
      case __OBJGROUP_SERAPH_RED_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_REDWINGS;
         break;
      case __OBJGROUP_SERAPH_GOLD_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_GOLDWINGS;
         break;
      case __OBJGROUP_SERAPH_DARKRED_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_DARKREDWINGS;
         break;
      case __OBJGROUP_SERAPH_DARKGOLD_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_DARKGOLDWINGS;
         break;
      case __OBJGROUP_SERAPH_LIGHTBLUE_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_LIGHTBLUEWINGS;
         break;
      case __OBJGROUP_SERAPH_ORANGE_WINGS:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_ORANGEWINGS;
         break;
      case __OBJGROUP_BUTTERFLY_WING1:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_BUTTERFLYWINGS1;
         break;
      case __OBJGROUP_BUTTERFLY_WING2:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_BUTTERFLYWINGS2;
         break;
      case __OBJGROUP_BUTTERFLY_WING3:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_BUTTERFLYWINGS3;
         break;
      case __OBJGROUP_BUTTERFLY_WING4:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_BUTTERFLYWINGS4;
         break;
      case __OBJGROUP_ARCHWINGS_WHITE:
         Object->PuppetInfo[PUP_CAPE] = PUPEQ_ARCHWINGSW;
         break;

         
   }
    
    switch (W_RIGHT) {
    case __OBJGROUP_DAGGER:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_BATTLEDAGGER;
        break;
    case __OBJGROUP_SHORT_SWORD:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_NORMALSWORD;
        break;
    case __OBJGROUP_LONG_SWORD:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_NORMALSWORD;
        break;
    case __OBJGROUP_FLAIL:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_FLAIL;
        break;
    case __OBJGROUP_MORNING_STAR:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_MORNINGSTAR;
        break;
    case __OBJGROUP_WARHAMMER:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_WARHAMMER;
        break;
    case __OBJGROUP_SPEAR:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_NORMALSWORD;
        break;
    case __OBJGROUP_AXE:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_AXE;
        break;
    case __OBJGROUP_STAFF1:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_STAFF1;
        break;
    case __OBJGROUP_STAFF2:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_STAFF2;
        break;
    case __OBJGROUP_STAFF3:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_STAFF3;
        break;
    case __OBJGROUP_STAFF4:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_STAFF4;
        break;
    case __OBJGROUP_MACE:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_MACE;
        break;
    case __OBJGROUP_SPIKE_MACE:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_MACE;
        break;
    case __OBJGROUP_MAUL:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_NORMALSWORD;
        break;
    case __OBJGROUP_OGRECLUB:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_OGRECLUB;
        break;
    case __OBJGROUP_DOUBLE_AXE:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_AXE;
        break;
    case __OBJGROUP_SINGLE_AXE:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_AXE;
        break;
    case __OBJGROUP_SPEAR_AXE:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_AXE;
        break;
    case __OBJGROUP_GLINTING_SWORD:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_NORMALSWORD;
        break;
    case __OBJGROUP_DARK_SWORD:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_NORMALSWORD;
        break;
    case __OBJGROUP_REAL_DARKSWORD:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_REALDARK;
        break;
    case __OBJGROUP_BATTLE_SWORD:
    case __OBJGROUP_BASTARD_SWORD:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_BATTLESWORD;
        break;
    case __OBJGROUP_GOLDEN_STAR:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_GOLDENSTAR;
        break;
    case __OBJGROUP_FANCY_SHORT_BOW:
    case __OBJGROUP_BOW:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_SMALL_BOW;
        break; 
    case __OBJGROUP_FANCY_LONG_BOW:
    case __OBJGROUP_LARGE_BOW2:
    case __OBJGROUP_LARGE_BOW:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_LARGE_BOW;
        break;
    case __OBJGROUP_SKAVEN_CLUB:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_SKAVENCLUB;
        break;
    case __OBJGROUP_SKAVEN_KNIFE:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_SKAVENKNIFE;
        break;
    case __OBJGROUP_SKELETON_AXE:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_SKELAXE;
        break;
    case __OBJGROUP_HALBERD:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_HALBERD;
        break;
    case __OBJGROUP_SKELETON_SWORD:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_SKELSWORD;
        break;
//Dialsoft new weapon
	case __OBJGROUP_FIREFLAIL:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_FIREFLAIL;
        break;
	case __OBJGROUP_TIGERAXE:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_TIGERAXE;
		break;
	case __OBJGROUP_ANCIENTAXE:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_ANCIENTAXE;
		break;
	case __OBJGROUP_FLOWERS:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_FLOWERS;
		break;
	case __OBJGROUP_NECROSTAFF:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_NECROSTAFF;
		break;
   case __OBJGROUP_NEW_BOW01:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_NEW_BOW01;
      break;
   case __OBJGROUP_DWARF_HAMMER :
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_DWARF_HAMMER;
      break;
   case __OBJGROUP_AXE_DESTRUCTION:
        Object->PuppetInfo[PUP_WEAPON] = PUPEQ_AXE_DESTRUCTION;
      break;
   case __OBJGROUP_SWORD_ANGEL:
         Object->PuppetInfo[PUP_WEAPON] = PUPEQ_SWORD_ANGEL;
      break;
   case __OBJGROUP_CLAY2BLADES:
         Object->PuppetInfo[PUP_WEAPON] = PUPEQ_CLAY_2BLADES;
      break;
   case __OBJGROUP_SWORD_HERO:
         Object->PuppetInfo[PUP_WEAPON] = PUPEQ_SWORD_HERO;
      break;
   case __OBJGROUP_HAMM:
         Object->PuppetInfo[PUP_WEAPON] = PUPEQ_HAMM;
      break;

   case __OBJGROUP_V2_DAGGER_VIP:
         Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_DAGGER_VIP;
      break;
   case __OBJGROUP_V2_DAGGER01:
         Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_DAGGER_01;
      break;
   case __OBJGROUP_V2_DAGGER02:
         Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_DAGGER_02;
      break;
   case __OBJGROUP_V2_DAGGER03:
         Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_DAGGER_03;
      break;
   case __OBJGROUP_V2_DAGGER04:
         Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_DAGGER_04;
      break;
   case __OBJGROUP_V2_DAGGER05:
         Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_DAGGER_05;
      break;

   //NM_TAG_ADD_SWORD
   case __OBJGROUP_V2_BUSSWORD01:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BUSSWORD01;
      break;
   case __OBJGROUP_V2_BUSSWORD02:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BUSSWORD02;
      break;
   case __OBJGROUP_V2_BUSSWORD03:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BUSSWORD03;
      break;
   case __OBJGROUP_V2_CLAYSWORD01:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_CLAYSWORD01;
      break;
   case __OBJGROUP_V2_CLAYSWORD02:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_CLAYSWORD02;
      break;
   case __OBJGROUP_V2_GLAIVESWORD01:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_GLAIVESWORD01;
      break;
   case __OBJGROUP_V2_GLAIVESWORD02:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_GLAIVESWORD02;
      break;
   case __OBJGROUP_V2_GLAIVESWORD02_ICE:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_GLAIVESWORD02_ICE;
      break;
   case __OBJGROUP_V2_SWORD01:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SWORD01;
      break;
   case __OBJGROUP_V2_SWORD02:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SWORD02;
      break;
   case __OBJGROUP_V2_SWORD03:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SWORD03;
      break;
   case __OBJGROUP_V2_SWORD04:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SWORD04;
      break;
   case __OBJGROUP_V2_SWORD05:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SWORD05;
      break;
   case __OBJGROUP_V2_SWORD06:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SWORD06;
      break;
   case __OBJGROUP_V2_SWORD07:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SWORD07;
      break;
   case __OBJGROUP_V2_SWORD08:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SWORD08;
      break;
      
   case __OBJGROUP_V2_HACHE01:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE01;
      break;
   case __OBJGROUP_V2_HACHE02:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE02;
      break;
   case __OBJGROUP_V2_HACHE03:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE03;
      break;
   case __OBJGROUP_V2_HACHE04:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE04;
      break;
   case __OBJGROUP_V2_HACHE05:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE05;
      break;
   case __OBJGROUP_V2_HACHE06:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE06;
      break;
   case __OBJGROUP_V2_HACHE07:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE07;
      break;
   case __OBJGROUP_V2_HACHE08:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE08;
      break;
   case __OBJGROUP_V2_HACHE09:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE09;
      break;
   case __OBJGROUP_V2_HACHE10:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE10;
      break;
   case __OBJGROUP_V2_HACHE11:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HACHE11;
      break;

   //new BOW
	case __OBJGROUP_V2_BOW01:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW01;
	break;
   case __OBJGROUP_V2_BOW02:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW02;
	break;
   case __OBJGROUP_V2_BOW03:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW03;
	break;
   case __OBJGROUP_V2_BOW04:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW04;
	break;
   case __OBJGROUP_V2_BOW05:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW05;
	break;
   case __OBJGROUP_V2_BOW05_ICE:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW05_ICE;
   break;
   case __OBJGROUP_V2_BOW06:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW06;
	break;
   case __OBJGROUP_V2_BOW07:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW07;
	break;
   case __OBJGROUP_V2_BOW08:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW08;
	break;
   case __OBJGROUP_V2_BOW09:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW09;
	break;
   case __OBJGROUP_V2_BOW10:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_BOW10;
	break;

   case __OBJGROUP_V2_SCEPTRE01:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SCEPTRE01;
	break;
   case __OBJGROUP_V2_SCEPTRE02:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SCEPTRE02;
	break;
   case __OBJGROUP_V2_SCEPTRE03:
		Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SCEPTRE03;
	break;

   case __OBJGROUP_V2_SP01:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SP01;
      break;
   case __OBJGROUP_V2_SP02:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SP02;
      break;
   case __OBJGROUP_V2_SP03:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SP03;
      break;
   case __OBJGROUP_V2_SP04:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SP04;
      break;
   case __OBJGROUP_V2_SP05:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SP05;
      break;
   case __OBJGROUP_V2_SP06:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SP06;
      break;
   case __OBJGROUP_V2_SP07:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_SP07;
      break;

   case __OBJGROUP_V2_HAMMER01:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HAMMER01;
      break;
   case __OBJGROUP_V2_HAMMER02:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HAMMER02;
      break;
   case __OBJGROUP_V2_HAMMER03:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HAMMER03;
      break;
   case __OBJGROUP_V2_HAMMER04:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HAMMER04;
      break;
   case __OBJGROUP_V2_HAMMER05:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HAMMER05;
      break;
   case __OBJGROUP_V2_HAMMER06:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HAMMER06;
      break;
   case __OBJGROUP_V2_HAMMER07:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_V2_HAMMER07;
      break;
   case __OBJGROUP_T4CP_MATRAQUE:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_T4CP_MATRAQUE;
      break;
   case __OBJGROUP_T4CP_BAT:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_T4CP_BAT;
      break;
   case __OBJGROUP_T4CP_12:
      Object->PuppetInfo[PUP_WEAPON] = PUPEQ_T4CP_12;
      break;
   


      
  

     

      
//Dialsoft end
    }
    
    switch (HELM) {
    case __OBJGROUP_LEATHER_HELM:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_LEATHER;
        break;
    case __OBJGROUP_PADDED_HELM:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_LEATHER;
        break;
    case __OBJGROUP_RING_HELM:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_CHAIN;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_CHAIN;
        break;
    case __OBJGROUP_CHAIN_COIF:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_CHAIN;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_CHAIN;
        break;
		//Tiamat Changes Start
    case __OBJGROUP_PLATE_HELM:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_PLATE;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_PLATE;
        break;
	case __OBJGROUP_PLATE_HELM_GOLD:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_PLATE_GOLD;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_PLATE_GOLD;
        break;
	case __OBJGROUP_PLATE_HELM_SILVER:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_PLATE_SILVER;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_PLATE_SILVER;
        break;
	case __OBJGROUP_PLATE_HELM_BLACK:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_PLATE_BLACK;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_PLATE_BLACK;
        break;
	case __OBJGROUP_PLATE_HELM_BLUE:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_PLATE_BLUE;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_PLATE_BLUE;
        break;
	case __OBJGROUP_PLATE_HELM_GREEN:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_PLATE_GREEN;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_PLATE_GREEN;
        break;
	case __OBJGROUP_PLATE_HELM_RED:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_PLATE_RED;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_PLATE_RED;
        break;
	case __OBJGROUP_PLATE_HELM_PURPLE:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_PLATE_PURPLE;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_PLATE_PURPLE;
        break;
	case __OBJGROUP_PLATE_HELM_EMERALD:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_PLATE_EMERALD;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_PLATE_EMERALD;
        break;
		//Tiamat Changes End
    case __OBJGROUP_HORNED_HELMET:
        Object->PuppetInfo[PUP_HEAD] = PUPEQ_HORNED;
        Object->PuppetInfo[PUP_HAT] = PUPEQ_HORNED;
        break;
    case __OBJGROUP_GOLDENCROWN:
        Object->PuppetInfo[PUP_HAT] = PUPEQ_GOLDENCROWN;
        break;
    case __OBJGROUP_ELVEN_HAT:
        Object->PuppetInfo[PUP_HAT] = PUPEQ_ELVEN;
        break;
    case __OBJGROUP_SHAMAN_HELM: 
        Object->PuppetInfo[PUP_HAT] = PUPEQ_SHAMAN;
        break;
    case __OBJGROUP_SKELETON_HELM:
        Object->PuppetInfo[PUP_HAT] = PUPEQ_SKEL;
        break;
 //Dialsoft new helmet:
    case __OBJGROUP_REDFEATHER:
        Object->PuppetInfo[PUP_HAT] = PUPEQ_REDFEATHER;
        break;
   case __OBJGROUP_WITCH_HAT1:
        Object->PuppetInfo[PUP_HAT]  = PUPEQ_WITCHHAT1;
        break;
   case __OBJGROUP_WITCH_HAT2:
        Object->PuppetInfo[PUP_HAT]  = PUPEQ_WITCHHAT2;
        break;
   case __OBJGROUP_WITCH_HAT3:
        Object->PuppetInfo[PUP_HAT]  = PUPEQ_WITCHHAT3;
        break;
   case __OBJGROUP_WITCH_HAT4:
        Object->PuppetInfo[PUP_HAT]  = PUPEQ_WITCHHAT4;
        break;
   case __OBJGROUP_WITCH_HAT5:
        Object->PuppetInfo[PUP_HAT]  = PUPEQ_WITCHHAT5;
        break;
   case __OBJGROUP_MASQUE_GOBLIN:
		   Object->PuppetInfo[PUP_MASK] = PUPEQ_GOBMASK;
		break;
   case __OBJGROUP_V2_MASQUE_GOBLIN_ICE:
         Object->PuppetInfo[PUP_MASK] = PUPEQ_V2_GOBMASK_ICE;
      break;
   case __OBJGROUP_NOEL_HAT:
        Object->PuppetInfo[PUP_HAT]  = PUPEQ_NOEL_HAT;
        break;
   case __OBJGROUP_V2_HAUME01:
		   Object->PuppetInfo[PUP_HAT] = PUPEQ_V2_HAUME01;
		break;
   case __OBJGROUP_V2_HAUME02:
		   Object->PuppetInfo[PUP_HAT] = PUPEQ_V2_HAUME02;
		break;
   case __OBJGROUP_V2_HAUME03:
		   Object->PuppetInfo[PUP_HAT] = PUPEQ_V2_HAUME03;
		break;
   case __OBJGROUP_V2_HAUME04:
		   Object->PuppetInfo[PUP_HAT] = PUPEQ_V2_HAUME04;
		break;
   case __OBJGROUP_V2_HAT01:
		   Object->PuppetInfo[PUP_HAT] = PUPEQ_V2_HAT01;
		break;
   case __OBJGROUP_V2_MASK01:
		   Object->PuppetInfo[PUP_MASK] = PUPEQ_V2_MASK01;
		break;
   case __OBJGROUP_V2_HAUME04_BL:
		   Object->PuppetInfo[PUP_HAT] = PUPEQ_V2_HAUME04_BL;
		break;
   case __OBJGROUP_T4CP_CASQUE:
        Object->PuppetInfo[PUP_HAT]  = PUPEQ_T4CP_CASQUE;
        break;
   case __OBJGROUP_T4CP_CASQUETTE:
        Object->PuppetInfo[PUP_HAT]  = PUPEQ_T4CP_CASQUETTE;
        break;
      
    }
//Dialsoft end
    switch (GLOVES) {
    case __OBJGROUP_PADDED_GLOVE:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_LEATHER;
        break;
    case __OBJGROUP_LEATHER_GLOVE:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_LEATHER;
        break;
	//Tiamat changes start
    case __OBJGROUP_PLATE_GLOVE:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_PLATE;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_PLATE;
        break;
	case __OBJGROUP_PLATE_GLOVE_GOLD:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_PLATE_GOLD;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_PLATE_GOLD;
        break;
	case __OBJGROUP_PLATE_GLOVE_SILVER:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_PLATE_SILVER;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_PLATE_SILVER;
        break;
	case __OBJGROUP_PLATE_GLOVE_BLACK:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_PLATE_BLACK;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_PLATE_BLACK;
        break;
	case __OBJGROUP_PLATE_GLOVE_BLUE:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_PLATE_BLUE;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_PLATE_BLUE;
        break;
	case __OBJGROUP_PLATE_GLOVE_GREEN:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_PLATE_GREEN;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_PLATE_GREEN;
        break;
	case __OBJGROUP_PLATE_GLOVE_RED:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_PLATE_RED;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_PLATE_RED;
        break;
	case __OBJGROUP_PLATE_GLOVE_PURPLE:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_PLATE_PURPLE;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_PLATE_PURPLE;
        break;
	case __OBJGROUP_PLATE_GLOVE_EMERALD:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_PLATE_EMERALD;
        Object->PuppetInfo[PUP_HAND_LEFT] = PUPEQ_PLATE_EMERALD;
        break;
   case __OBJGROUP_MAN_ARMOR01_HAND_GR:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_V2_MAN_ARMOR01GR;
        Object->PuppetInfo[PUP_HAND_LEFT ] = PUPEQ_V2_MAN_ARMOR01GR;
        break;
    case __OBJGROUP_MAN_ARMOR01_HAND_BL:
        Object->PuppetInfo[PUP_HAND_RIGHT] = PUPEQ_V2_MAN_ARMOR01BL;
        Object->PuppetInfo[PUP_HAND_LEFT ] = PUPEQ_V2_MAN_ARMOR01BL;
        break;
 
    }
	//Tiamat changes end
    
    switch (LEGS) {
    case __OBJGROUP_PADDED_LEGGINGS:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_LEATHER;
        break;
    case __OBJGROUP_RING_LEGGINGS:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_CHAIN;
        break;
    case __OBJGROUP_LEATHER_PANTS:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_LEATHER;
        break;
	//Tiamat changes start
    case __OBJGROUP_PLATE_LEGS:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_PLATE;
        break;
	case __OBJGROUP_PLATE_LEGS_GOLD:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_PLATE_GOLD;
        break;
	case __OBJGROUP_PLATE_LEGS_SILVER:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_PLATE_SILVER;
        break;
	case __OBJGROUP_PLATE_LEGS_BLACK:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_PLATE_BLACK;
        break;
	case __OBJGROUP_PLATE_LEGS_BLUE:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_PLATE_BLUE;
        break;
	case __OBJGROUP_PLATE_LEGS_GREEN:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_PLATE_GREEN;
        break;
	case __OBJGROUP_PLATE_LEGS_RED:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_PLATE_RED;
        break;
	case __OBJGROUP_PLATE_LEGS_PURPLE:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_PLATE_PURPLE;
        break;
	case __OBJGROUP_PLATE_LEGS_EMERALD:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_PLATE_EMERALD;
        break;
	//Tiamat changes end
    case __OBJGROUP_CHAIN_LEGS:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_CHAIN;
        break;
    case __OBJGROUP_LEG_CLOTH1:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_SET1;
        Object->PuppetInfo[PUP_ROBELEGS] = PUPEQ_SET1;
        break;
    case __OBJGROUP_STUDDEDLEG:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_STUDDED;
        break;
    case __OBJGROUP_MAN_ARMOR01_LEGS_GR:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_V2_MAN_ARMOR01GR;
        break;
    case __OBJGROUP_MAN_ARMOR01_LEGS_BL:
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_V2_MAN_ARMOR01BL;
        break;
    }
    
    switch (FEET) {
    case __OBJGROUP_PADDED_BOOTS:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_LEATHER;
        break;
    case __OBJGROUP_LEATHER_BOOTS:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_LEATHER;
        break;
	//Tiamat changes start
    case __OBJGROUP_PLATE_BOOT:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_PLATE;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_PLATE;
        break;
	case __OBJGROUP_PLATE_BOOT_GOLD:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_PLATE_GOLD;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_PLATE_GOLD;
        break;
	case __OBJGROUP_PLATE_BOOT_SILVER:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_PLATE_SILVER;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_PLATE_SILVER;
        break;
	case __OBJGROUP_PLATE_BOOT_BLACK:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_PLATE_BLACK;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_PLATE_BLACK;
        break;
	case __OBJGROUP_PLATE_BOOT_BLUE:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_PLATE_BLUE;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_PLATE_BLUE;
        break;
	case __OBJGROUP_PLATE_BOOT_GREEN:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_PLATE_GREEN;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_PLATE_GREEN;
        break;
	case __OBJGROUP_PLATE_BOOT_RED:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_PLATE_RED;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_PLATE_RED;
        break;
	case __OBJGROUP_PLATE_BOOT_PURPLE:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_PLATE_PURPLE;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_PLATE_PURPLE;
        break;
	case __OBJGROUP_PLATE_BOOT_EMERALD:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_PLATE_EMERALD;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_PLATE_EMERALD;
        break;
	//Tiamat changes end
    case __OBJGROUP_BLACKLEATHER_BOOT:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_BLACKLEATHER;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_BLACKLEATHER;
        break;
    case __OBJGROUP_MAN_ARMOR01_FOOT_GR:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_V2_MAN_ARMOR01GR;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_V2_MAN_ARMOR01GR;
        break;
    case __OBJGROUP_MAN_ARMOR01_FOOT_BL:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_V2_MAN_ARMOR01BL;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_V2_MAN_ARMOR01BL;
        break;
    case __OBJGROUP_WHITELEATHER_BOOT:
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_WHITELEATHER;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_WHITELEATHER;
        break;
        
    }

    
    switch (W_LEFT) {
    case __OBJGROUP_SHIELD:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_BAROSSASHIELD;
        break;
    case __OBJGROUP_ROMAN_SHIELD:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_ROMANSHIELD;
        break;
    case __OBJGROUP_BAROSSA_SHIELD:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_BAROSSASHIELD;
        break;
    case __OBJGROUP_ORC_SHIELD:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_ORCSHIELD;
        break;
    case __OBJGROUP_CENTAUR_SHIELD1:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_CENTAURSHIELD1;
        break;
    case __OBJGROUP_CENTAUR_SHIELD2:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_CENTAURSHIELD2;
        break;
    case __OBJGROUP_SKAVEN_SHIELD1:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_SKAVENSHIELD1;
        break;
    case __OBJGROUP_SKAVEN_SHIELD2:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_SKAVENSHIELD2;
        break;
    case __OBJGROUP_SKAVEN_SHIELD3:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_SKAVENSHIELD3;
        break;
    case __OBJGROUP_SKELETON_SHIELD:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_SKELSHIELD;
        break;
    case __OBJGROUP_ORC_SHIELD2:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_ORCSHIELD2;
        break;
    case __OBJGROUP_SK_SHIELD:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_SKSHIELD;
        break;
    case __OBJGROUP_V2_SHIELD01:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_V2_SHIELD01;
        break;
    case __OBJGROUP_V2_SHIELD02:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_V2_SHIELD02;
        break;
    case __OBJGROUP_V2_ICESHIELD:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_V2_ICESHIELD01;
        break;
    
    //NM_TAG_ADD_SWORD Hand 2
    case __OBJGROUP_V2_2BUSSWORD01:
      Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2BUSSWORD01;
      break;
    case __OBJGROUP_V2_2BUSSWORD02:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2BUSSWORD02;
       break;
    case __OBJGROUP_V2_2BUSSWORD03:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2BUSSWORD03;
       break;
    case __OBJGROUP_V2_2CLAYSWORD01:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2CLAYSWORD01;
       break;
    case __OBJGROUP_V2_2CLAYSWORD02:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2CLAYSWORD02;
       break;
    case __OBJGROUP_V2_2GLAIVESWORD01:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2GLAIVESWORD01;
       break;
    case __OBJGROUP_V2_2GLAIVESWORD02:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2GLAIVESWORD02;
       break;
    case __OBJGROUP_V2_2GLAIVESWORD02_ICE: 
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2GLAIVESWORD02_ICE;
       break;
    case __OBJGROUP_V2_2SWORD01:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SWORD01;
       break;
    case __OBJGROUP_V2_2SWORD02:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SWORD02;
       break;
    case __OBJGROUP_V2_2SWORD03:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SWORD03;
       break;
    case __OBJGROUP_V2_2SWORD04:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SWORD04;
       break;
    case __OBJGROUP_V2_2SWORD05:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SWORD05;
       break;
    case __OBJGROUP_V2_2SWORD06:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SWORD06;
       break;
    case __OBJGROUP_V2_2SWORD07:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SWORD07;
       break;
    case __OBJGROUP_V2_2SWORD08:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SWORD08;
       break;

    case __OBJGROUP_V2_2HACHE01:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2HACHE01;
       break;
    case __OBJGROUP_V2_2HACHE02:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2HACHE02;
       break;
    case __OBJGROUP_V2_2HACHE03:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2HACHE03;
       break;
    case __OBJGROUP_V2_2HACHE04:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2HACHE04;
       break;
    case __OBJGROUP_V2_2HACHE05:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2HACHE05;
       break;
    case __OBJGROUP_V2_2HACHE06:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2HACHE06;
       break;

    case __OBJGROUP_V2_2DAGGER01:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2DAGUE01;
       break;
    case __OBJGROUP_V2_2DAGGER02:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2DAGUE02;
       break;
    case __OBJGROUP_V2_2DAGGER03:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2DAGUE03;
       break;
    case __OBJGROUP_V2_2DAGGER04:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2DAGUE04;
       break;
    case __OBJGROUP_V2_2DAGGER05:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2DAGUE05;
       break;
    case __OBJGROUP_V2_2DAGGER06:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2DAGUE06;
       break;

    case __OBJGROUP_V2_2SP01:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SP01;
       break;
    case __OBJGROUP_V2_2SP02:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SP02;
       break;
    case __OBJGROUP_V2_2SP03:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SP03;
       break;
    case __OBJGROUP_V2_2SP04:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SP04;
       break;
    case __OBJGROUP_V2_2SP05:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SP05;
       break;
    case __OBJGROUP_V2_2SP06:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2SP06;
       break;
       
    case __OBJGROUP_V2_2HAMMER01:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2HAMMER01;
       break;
    case __OBJGROUP_V2_2HAMMER02:
       Object->PuppetInfo[PUP_WEAPON2] = PUPEQ_V2_2HAMMER02;
       break;
    case __OBJGROUP_T4CP_SHIELD:
        Object->PuppetInfo[PUP_SHIELD] = PUPEQ_T4CP_SHIELD;
        break;
    }


     
    switch (BODY) {
    case __OBJGROUP_LEATHER_ARMOR: 
        Object->PuppetInfo[PUP_BODY] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_BACKBODY] = PUPEQ_LEATHER;  
        break;
    case __OBJGROUP_PADDED_ARMOR:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_LEATHER;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_LEATHER;
        break;
    case __OBJGROUP_SCALE_ARMOR:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_CHAIN;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_CHAIN;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_CHAIN;
        break;
    case __OBJGROUP_RING_ARMOR:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_CHAIN;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_CHAIN;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_CHAIN;
        break;
    case __OBJGROUP_CHAIN_BODY:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_CHAIN;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_CHAIN;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_CHAIN;
        break;
	//Tiamat changes start
    case __OBJGROUP_PLATE_ARMOR:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE;
        break;
	case __OBJGROUP_PLATE_ARMOR_GOLD:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_GOLD;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_GOLD;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_GOLD;
        break;
	case __OBJGROUP_PLATE_ARMOR_SILVER:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_SILVER;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_SILVER;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_SILVER;
        break;
	case __OBJGROUP_PLATE_ARMOR_BLACK:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_BLACK;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_BLACK;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_BLACK;
        break;
	case __OBJGROUP_PLATE_ARMOR_BLUE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_BLUE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_BLUE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_BLUE;
        break;
	case __OBJGROUP_PLATE_ARMOR_GREEN:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_GREEN;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_GREEN;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_GREEN;
        break;
	case __OBJGROUP_PLATE_ARMOR_RED:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_RED;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_RED;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_RED;
        break;
	case __OBJGROUP_PLATE_ARMOR_PURPLE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_PURPLE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_PURPLE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_PURPLE;
        break;
	case __OBJGROUP_PLATE_ARMOR_EMERALD:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_EMERALD;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_EMERALD;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_EMERALD;
        break;
    case __OBJGROUP_PLATE_ARMOR_W_SLEEVE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE;
        break;
	case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_GOLD:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_GOLD;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_GOLD;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_GOLD;
        break;
	case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_SILVER:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_SILVER;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_SILVER;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_SILVER;
        break;
	case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLACK:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_BLACK;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_BLACK;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_BLACK;
        break;
	case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_BLUE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_BLUE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_BLUE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_BLUE;
        break;
	case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_GREEN:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_GREEN;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_GREEN;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_GREEN;
        break;
	case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_RED:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_RED;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_RED;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_RED;
        break;
	case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_PURPLE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_PURPLE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_PURPLE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_PURPLE;
        break;
	case __OBJGROUP_PLATE_ARMOR_W_SLEEVE_EMERALD:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_PLATE_EMERALD;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_PLATE_EMERALD;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_PLATE_EMERALD;
        break;
	//Tiamat changes end
    case __OBJGROUP_NECROMANROBE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_NECROROBE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_NECROROBE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_NECROROBE;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_NECROROBE; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_NECROROBE; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_NECROROBE; 
        break;
    case __OBJGROUP_WHITEROBE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_WHITEROBE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_WHITEROBE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_WHITEROBE;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_WHITEROBE; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_WHITEROBE; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_WHITEROBE; 
        break;
    case __OBJGROUP_MAGEROBE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_MAGEROBE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_MAGEROBE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_MAGEROBE;
        Object->PuppetInfo[PUP_BACKBODY] = PUPEQ_MAGEROBE;
        if (Object->Type == 10011) {
            Object->PuppetInfo[PUP_LEGS] = PUPEQ_MAGEROBE;
            Object->PuppetInfo[PUP_FOOT] = PUPEQ_MAGEROBE;
            Object->PuppetInfo[PUP_BOOT] = PUPEQ_MAGEROBE;
            Object->PuppetInfo[PUP_CAPE_2] = PUPEQ_MAGEROBE;
        }
        break;
    case __OBJGROUP_LICHROBE:
        /*Object->PuppetInfo[PUP_BODY] = PUPEQ_LICHROBE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_LICHROBE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_LICHROBE;
        Object->PuppetInfo[PUP_BACKBODY] = PUPEQ_LICHROBE;
        Object->PuppetInfo[PUP_ROBELEGS] = PUPEQ_LICHROBE;
        if (Object->Type == 10011) //NMNMNM
        {
            Object->PuppetInfo[PUP_LEGS] = PUPEQ_LICHROBE; 
            Object->PuppetInfo[PUP_FOOT] = PUPEQ_LICHROBE; 
            Object->PuppetInfo[PUP_BOOT] = PUPEQ_LICHROBE; 
        }*/
        Object->PuppetInfo[PUP_BODY] = PUPEQ_LICHROBE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_LICHROBE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_LICHROBE;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_LICHROBE;
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_LICHROBE;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_LICHROBE;


    break;
    case __OBJGROUP_ARMORED_ROBE:
        Object->PuppetInfo[PUP_BODY]      = PUPEQ_ARMORED_ROBE;
        Object->PuppetInfo[PUP_ARM_LEFT]  = PUPEQ_ARMORED_ROBE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_ARMORED_ROBE;
        Object->PuppetInfo[PUP_BACKBODY]  = PUPEQ_ARMORED_ROBE;
        Object->PuppetInfo[PUP_ROBELEGS]  = PUPEQ_ARMORED_ROBE;
    break;
    case __OBJGROUP_REDROBE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_REDROBE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_REDROBE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_REDROBE;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_REDROBE;
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_REDROBE;
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_REDROBE;
        break;
    case __OBJGROUP_STUDDEDARMOR:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_STUDDED;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_STUDDED;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_STUDDED;
        break;
    case __OBJGROUP_BODY_CLOTH1:
        Object->PuppetInfo[PUP_BODY]      = PUPEQ_SET1;
        Object->PuppetInfo[PUP_ARM_LEFT]  = PUPEQ_SET1;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_SET1;
        break;
	//Dialsoft added
	case __OBJGROUP_SPIKEDLEATHER: 
        Object->PuppetInfo[PUP_BODY]      = PUPEQ_SPIKEDLEATHER;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_SPIKEDLEATHER;
        Object->PuppetInfo[PUP_ARM_LEFT]  = PUPEQ_SPIKEDLEATHER;
        Object->PuppetInfo[PUP_BACKBODY]  = PUPEQ_SPIKEDLEATHER;   //fixed here, no backbody spiked leather pic.
        break;
	case __OBJGROUP_FWHITEROBE_BLUE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_BLUE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_BLUE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_BLUE;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_BLUE; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_BLUE; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_BLUE; 
        break;
	case __OBJGROUP_FWHITEROBE_GREEN:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_GREEN;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_GREEN;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_GREEN;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_GREEN; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_GREEN; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_GREEN; 
        break;
	case __OBJGROUP_FWHITEROBE_DARKBLUE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_DARKBLUE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_DARKBLUE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_DARKBLUE;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_DARKBLUE; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_DARKBLUE; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_DARKBLUE; 
        break;
	case __OBJGROUP_FWHITEROBE_VIOLET:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_VIOLET;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_VIOLET;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_VIOLET;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_VIOLET; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_VIOLET; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_VIOLET; 
        break;
	case __OBJGROUP_FWHITEROBE_PURPLE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_PURPLE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_PURPLE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_PURPLE;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_PURPLE; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_PURPLE; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_PURPLE; 
        break;
	case __OBJGROUP_FWHITEROBE_YELLOW:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_YELLOW;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_YELLOW;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_YELLOW;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_YELLOW; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_YELLOW; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_YELLOW; 
        break;
	case __OBJGROUP_FWHITEROBE_GRAY:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_GRAY;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_GRAY;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_GRAY;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_GRAY; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_GRAY; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_GRAY; 
        break;
	case __OBJGROUP_FWHITEROBE_MAUVE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_MAUVE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_MAUVE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_MAUVE;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_MAUVE; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_MAUVE; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_MAUVE; 
        break;
	case __OBJGROUP_FWHITEROBE_ORANGE:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_ORANGE;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_ORANGE;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_ORANGE;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_ORANGE; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_ORANGE; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_ORANGE; 
        break;
	case __OBJGROUP_FWHITEROBE_BRUN:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_BRUN;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_BRUN;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_BRUN;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_BRUN; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_BRUN; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_BRUN; 
        break;
	case __OBJGROUP_FWHITEROBE_DARK:
        Object->PuppetInfo[PUP_BODY] = PUPEQ_FWHITEROBE_DARK;
        Object->PuppetInfo[PUP_ARM_LEFT] = PUPEQ_FWHITEROBE_DARK;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_FWHITEROBE_DARK;
        Object->PuppetInfo[PUP_LEGS] = PUPEQ_FWHITEROBE_DARK; 
        Object->PuppetInfo[PUP_FOOT] = PUPEQ_FWHITEROBE_DARK; 
        Object->PuppetInfo[PUP_BOOT] = PUPEQ_FWHITEROBE_DARK; 
        break;
   case __OBJGROUP_MAN_ARMOR01_BODY_GR:
        Object->PuppetInfo[PUP_BODY]      = PUPEQ_V2_MAN_ARMOR01GR;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_V2_MAN_ARMOR01GR;
        Object->PuppetInfo[PUP_ARM_LEFT]  = PUPEQ_V2_MAN_ARMOR01GR;
        break;
   case __OBJGROUP_MAN_ARMOR01_BODY_BL:
        Object->PuppetInfo[PUP_BODY]      = PUPEQ_V2_MAN_ARMOR01BL;
        Object->PuppetInfo[PUP_ARM_RIGHT] = PUPEQ_V2_MAN_ARMOR01BL;
        Object->PuppetInfo[PUP_ARM_LEFT]  = PUPEQ_V2_MAN_ARMOR01BL;
        break;
	//Dialsoft end
    }	
}


BOOL VisualObjectList::Found(signed char *xDif, signed char *yDif, unsigned long FollowID) {
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == FollowID) {
            *xDif = Object->OX;
            *yDif = Object->OY;
            SetQueryState(STOP_QUERY);
            return TRUE;
        }
    }
    
    return FALSE;
}

BOOL VisualObjectList::FoundValidID(signed char *xDif, signed char *yDif, unsigned long FollowID) {
    if( Player.CanSlayUsers ){
        return Found( xDif, yDif, FollowID );
    }
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == FollowID) {
            if (Object->Type < 10000) {
                SetQueryState(STOP_QUERY);
                return FALSE;
            }
            *xDif = Object->OX;
            *yDif = Object->OY;
            SetQueryState(STOP_QUERY);
            return TRUE;
        }
    }
    
    return FALSE;
}

WORD VisualObjectList::GetType(DWORD ID) {
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
            SetQueryState(STOP_QUERY);
            return Object->Type;
        }
    }
    
    return 0;
}

char * VisualObjectList::GetName(DWORD ID) {
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
            SetQueryState(STOP_QUERY);
            if (Object->Name)
                return Object->Name;
        }
    }
    
    return g_LocalString[204];
}


BOOL VisualObjectList::SetMissing(DWORD ID, BOOL bReturn) {
    ToHead();
    int Flush = 0;
    
    TFCObject *Object;
    while (QueryNext()) 
    {
        Object = GetObject();
        if (Object->ID == ID) 
        {
            Object->Flush++;
            Flush = Object->Flush;
            SetQueryState(STOP_QUERY);
        }
    }
    
    if (Flush > 1) 
    {
       if (bReturn)
           return TRUE;
        Add(SpellID, __SPELL_ICECLOUD, Player.xPos, Player.yPos, 100, 0, ID, 100);
        SpellID--;
        Delete(ID);
    }
    
    return FALSE;
}

int VisualObjectList::SetDirection(unsigned long ID, unsigned long ID2, bool bSetNow) {
    signed char X = 0, Y = 0;
    int Direction = 2;
    
    TFCObject *Object;
    if (ID2 == Player.ID) {
        X = 0;
        Y = 0;
    } else {
        ToHead();
        while(QueryNext()) {
            Object = GetObject();
            if (Object->ID == ID2) {
                X = Object->OX;
                Y = Object->OY;
                SetQueryState(STOP_QUERY);
            }
        }
    }
    
    ToHead();
    while(QueryNext()) 
    {
        Object = GetObject();
        if (Object->ID == ID) 
        {
            if (Object->OX > X && Object->OY == Y)
                Direction = 4;
            else if (Object->OX < X && Object->OY == Y)
                Direction = 6;
            else if (Object->OX == X && Object->OY < Y)
                Direction = 2;
            else if (Object->OX == X && Object->OY > Y)
                Direction = 8;
            else if (Object->OX > X && Object->OY < Y)
                Direction = 1;
            else if (Object->OX > X && Object->OY > Y)
                Direction = 7;
            else if (Object->OX < X && Object->OY < Y)
                Direction = 3;
            else if (Object->OX < X && Object->OY > Y)
                Direction = 9;
            else
               Direction = 1;

            
            SetQueryState(STOP_QUERY);
        }
    }
    
    if (bSetNow)
        Object->Direction = Direction;
    
    return Direction;
}			

int VisualObjectList::ChkText(void) {
    if (MainObject.IsTalkTextDrawn()){
        if ((timeGetTime() - MainObject.Temp) > 10000) {
            MainObject.StopTalkText();
            /*delete MainObject.TalkingText;
            MainObject.TalkingText = new char [1];
            MainObject.TalkingText[0] = 0;
            MainObject.Offset = 0;*/
        }
    }
    
    TFCObject *Object = NULL; 
    int returnthis = 0;
    ToHead();
    
    bool KillOne = false;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->IsTalkTextDrawn()){
            if ((timeGetTime() - Object->Temp) > 25000) {
                Object->StopTalkText();
                /*delete Object->TalkingText;
                Object->TalkingText = new char [1];
                Object->TalkingText[0] = 0;
                Object->Offset = 0;*/
                returnthis = 1;
            }
        }
        if (Object->IsNameDrawn()){
            if ((timeGetTime() - Object->IdentifyTemp) > 3500) { 
                Object->StopNameDisplay();
                //delete Object->IdentifyText;
                //Object->IdentifyText = new char [1];
                //Object->IdentifyText[0] = 0;
                returnthis = 1;
            }
        }
        if (Object->Killed) {
            if ((timeGetTime() - Object->KillTimer) > 500 && !Object->MovingQueue.NbObjects() ) {
                int RandomSound = (rand() % 2);
                Object->onKilled[RandomSound].Play(FALSE);
                Object->Up = 0;
                Object->Down = 0;
                Object->Right = 0;
                Object->Left = 0;
                Object->Type = Object->KillType;
                Object->Killed = false;
                Object->Chose = 0;
                while (isMovX) {
                    Object->fMovX += Object->SpeedX;
                    Object->MovX = Object->fMovX;
                }
                while (isMovY) {
                    Object->fMovY += Object->SpeedY;
                    Object->MovY = Object->fMovY;
                }
            }
        }
        
        if (Object->KillType && timeGetTime() - Object->KillTimer > 5000) {
            Object->DeleteMe = TRUE;
        }
    }
    
    FXType *FXObject;
    SoundFXList.Lock("SFX 3");
    SoundFXList.ToHead();
    while (SoundFXList.QueryNext()) {
        FXObject = SoundFXList.GetObject();
        if (!FXObject->Debug && timeGetTime() - FXObject->Time > 150) {
            FXObject->Sound.Play(FALSE);
            FXObject->Debug = TRUE;
        }
        if (FXObject->Debug && timeGetTime() - FXObject->Time > 2500) {
            SoundFXList.RemoveObject();
            delete FXObject;
        }
    }
    SoundFXList.Unlock("SFX 3");
    
    for (int i = 0; i < NBBASICSPRITE; i++) {
        if (Object3DLoad[i] && i != Set) {
            if (VObject3D[i]->Release(3600000)) {
                Object3DLoad[i] = false;
                VObject3D[i]->Release();
                delete VObject3D[i];
                VObject3D[i] = NULL;
                for (int j = 0; j < 11; j++) {
                    if (Object3DSound[i][j]) {
                        SoundFX[Object3DSound[i][j]].Release(FALSE);
                    } else {
                        break;
                    }
                }
            }
        }
    }
    
    return returnthis;
    //return 0;//BLBLBL 29 mars 2009 retrait du code inutilisé
}


void VisualObjectList::CreatePlayer(void) {
    
    SetSet(FALSE);
    
}

void VisualObjectList::SetEvil(unsigned long IDAttack) {
    ToHead();
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == IDAttack) {
            if (Object->Type > 10000) {
                if (Object->Friendly != VOL_MONSTER) {
                    Object->Friendly = VOL_MONSTER;
                    Object->MouseCursor = 2;
                }
            } else {
                SetMouseCursor(Object);
            }
            SetGrid(Object);
            SetQueryState(STOP_QUERY);
        }
    }                                          
}

void VisualObjectList::SetAttack(unsigned long IDAttack, unsigned long IDDefend, char TypeAttack, char TypeDefend, char pHp, DWORD forcedDirection) {
    Deplacement *Moving = new Deplacement;
    ZeroMemory(Moving, sizeof(Deplacement));
    Moving->ID = IDAttack;
    if (IDDefend)
        Moving->Type = 1;
    else
        Moving->Type = 3;
    
    Moving->AnimType = TypeAttack;
    if( IDDefend ){
        Moving->Direction = SetDirection(IDAttack, IDDefend);
    }else{
        if( forcedDirection != 0 ){
            Moving->Direction = forcedDirection;
        }
    }
    if (TypeDefend) {
        Moving->Miss = false;
    } else {
        Moving->Miss = true;
    }
    
    Moving->Victim = new T3VSBSound;
    Moving->Victim->CopyWave(&SoundFX[99]);
    ToHead();
    if (IDDefend == Player.ID && MainObject.Type == __PLAYER_HUMAN_PUPPET) {
        switch (rand() % 3) {
        case 0: Moving->Victim->CopyWave(&SoundFX[98]); break;
        case 1: Moving->Victim->CopyWave(&SoundFX[99]); break;
        case 2: Moving->Victim->CopyWave(&SoundFX[99]); break;
        }
        
        Moving->Parry = NULL;
    } else if (IDDefend == Player.ID && MainObject.Type == __PLAYER_HUMAN_FEMALE) {
        switch (rand() % 3) {
        case 0: Moving->Victim->CopyWave(&SoundFX[53]); break;
        case 1: Moving->Victim->CopyWave(&SoundFX[54]); break;             
        case 2: Moving->Victim->CopyWave(&SoundFX[54]); break;
        }
        
        Moving->Parry = NULL;
    }else {
        TFCObject *Object;
        while(QueryNext()) {
            Object = GetObject();
            if (Object->ID == IDDefend) {
                Moving->Victim->CopyWave(&Object->onAttacked[rand() % 3]);
                if (Object->Parry) {
                    Moving->Parry = new T3VSBSound;
                    Moving->Parry->CopyWave(&Object->onParry);
                }
                else
                    Moving->Parry = NULL;
                if (pHp > 0)
                    Object->pHp = pHp;
                
                // If this a ranged weapon attack
                if( IsRangedObject( Object ) && Object->AttFrame > 2 ){
                    // Reset the attack frame to 2 if the player wasn't standing.
                    Objects.MainObject.AttFrame = 2;
                }
            }
        }
    }
    
    BOOL Found = FALSE;
    ToHead();
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == IDAttack) {
            if (Object->Friendly != VOL_MONSTER && IDDefend == Player.ID) {
                if (Object->Type > 10000) {
                    Object->Friendly = VOL_MONSTER;
                    Object->MouseCursor = 2;
                } else {
                    SetMouseCursor(Object);
                }
                SetGrid(Object);
            }
            
            // If no direction was specified.
            if( Moving->Direction == 0 ){
                // Use the direction of the attacker.
                Moving->Direction = Object->Direction;
            }
            
            Found = TRUE;
            Object->MovingQueue.AddToQueue(Moving);
            //			 Object->Dynamic = true;
            //	        World.ChangeStatic();
            SetQueryState(STOP_QUERY);
        }
    }
    
    if (!Found) {
        if (Moving->Victim)
            delete Moving->Victim;
        if (Moving->Parry)
            delete Moving->Parry;
        delete Moving;
    }
}	

void VisualObjectList::SetOffset(unsigned long ID, short Offset) {
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
            Object->SetTalkOffset( Offset );
            /*if (Object->TalkingText[0] && strlen(Object->TalkingText) > Offset)
                Object->Offset = Offset;
            else
                Object->Offset = Offset = 0;
            
            Object->Temp = timeGetTime();
            if (!Offset) {
                delete Object->TalkingText;
                Object->TalkingText = new char [1];
                Object->TalkingText[0] = 0;
                Object->Offset = 0;
            }*/

            // TODO !!!!
            SetQueryState(STOP_QUERY);
        }
    }
}

void VisualObjectList::SetGuildName(unsigned long ID, char *GuildName, DWORD color) {//BLBLBL
	ToHead();
	TFCObject *Object;
	while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
			// Destiny: avoid memory leak
			if( Object->GuildName != NULL )
				delete Object->GuildName;

			Object->GuildName = new char [strlen(GuildName)+1];
			strcpy(Object->GuildName, GuildName);
			Object->GuildColor = color;

            SetQueryState(STOP_QUERY); 			
		}
	}
}

void VisualObjectList::SetName(unsigned long ID, char *Name, BOOL DisplayNow, DWORD color ) {
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
			// Destiny: avoid memory leak
			if( Object->Name != NULL )
				delete Object->Name;

            Object->Name = new char [strlen(Name)+1];
            strcpy(Object->Name, Name);
            SetQueryState(STOP_QUERY); 

			Object->NameColor = color;
            Object->DisplayName();
            //delete Object->IdentifyText;
            //Object->IdentifyText = new char [strlen(Object->Name)+1];
            //strcpy(Object->IdentifyText, Object->Name);
            
            if( DisplayNow ){
                Object->IdentifyTemp = timeGetTime();
            }
        }
    }
}

T3VSBSound *VisualObjectList::GetSound(unsigned long ID) {
    ToHead();
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
            SetQueryState(STOP_QUERY);
            return &Object->onAttacked[rand() % 3];
        }
    }
    
    return NULL;
}

void VisualObjectList::SetUnit(unsigned long ID, short Type, char Light, char Status, char pHp) {
    BOOL ChangeLight = FALSE;
    
    if (ID == Player.ID) {
        if (Light != Player.Light)
            ChangeLight = TRUE;
        Player.Light = Light;
    } else {
        ToHead();
        TFCObject *Object;
        while(QueryNext()) {
            Object = GetObject();
            if (Object->ID == ID) {
                Object->pHp = pHp;
                if (Light != Object->Light)
                    ChangeLight = TRUE;
                Object->Light = Light;
                SetQueryState(STOP_QUERY);
            }
        }
    }
    
    if (ChangeLight)
        UpdateLight();
}

void VisualObjectList::SetHP(unsigned long ID, char pHp) {
    if (ID != Player.ID) {
        ToHead();
        TFCObject *Object;
        while(QueryNext()) {
            Object = GetObject();
            if (Object->ID == ID) {
                Object->pHp = pHp;
                SetQueryState(STOP_QUERY);
            }
        }
    }
}

void VisualObjectList::OrderObject(void) {
    int Width, Height, a, b;
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        Object->OX    = (signed char)(Object->AbsX - Player.xPos);
        Object->OY    = (signed char)(Object->AbsY - Player.yPos);
        Object->OC    = 0;
        a = b = 0;
        World.GetHeight(Object->OX+ViewWidth, Object->OY+ViewHeight, &Width, &Height, &Object->OC);
        Object->OY    += Height;
    }
}

void VisualObjectList::AttackMode(bool currObject) {
    TFCObject *Object;
    if (!currObject) {
        Player.AttackMode++;
        ToHead();
        
        while(QueryNext()) {
            Object = GetObject();
            if ( ( Player.AttackMode & 1 ) && Object->Type > 10000) {
                Object->MouseCursor = 2;
            } else {         
                if( Object->Type > 10000 ){
                    if (Object->Friendly == VOL_PLAYER) {
                        Object->MouseCursor = 0;
                    }
                    if (Object->Friendly == VOL_NPC) {
                        Object->MouseCursor = 3;
                    }
                    if (Object->Friendly == VOL_MONSTER) {
                        if (Object->Type > 10000) {
                            Object->MouseCursor = 2;
                        } else {
                            SetMouseCursor(Object);
                        }
                    }
                }
            }      
        }
        
        SetTotalGrid();
    } else {
        Object = GetObject();
        LOG << "OBJECT: " << (int)Object << "\r\n";
        if ( ( Player.AttackMode & 1 ) && Object->Type > 10000) {
            Object->MouseCursor = 2;
        } else {         
            if( Object->Type > 10000 ){
                if (Object->Friendly == VOL_PLAYER) {
                    Object->MouseCursor = 0;
                }
                if (Object->Friendly == VOL_NPC) {
                    Object->MouseCursor = 3;
                }
                if (Object->Friendly == VOL_MONSTER) {
                    if (Object->Type > 10000) {
                        Object->MouseCursor = 2;
                    } else {
                        SetMouseCursor(Object);
                    }
                }
            }
        }      
    }
}

void VisualObjectList::SetGridMode(int Type) {
    if (Type == 0) {
        // (Monster Prioritize)
        Player.SetGridMode = 0;
    } else if (Type == 1) {
        // Both Equal
        Player.SetGridMode = 1;
    } else if (Type == 2) {
        // Player Prioritize
        Player.SetGridMode = 2;
    }
    
    ToHead();
    
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Player.AttackMode & 1) {
            if (Object->Type > 10000) {
                Object->MouseCursor = 2;
            }
        } else {
            if (Object->Friendly == VOL_PLAYER) {
                Object->MouseCursor = 0;
            }
            if (Object->Friendly == VOL_NPC) {
                Object->MouseCursor = 3;
            }
            if (Object->Friendly == VOL_MONSTER) {
                if (Object->Type > 10000) {
                    Object->MouseCursor = 2;
                } else {
                    SetMouseCursor(Object);
                }
            }
        }
    }
    
    SetTotalGrid();
}

void VisualObjectList::PlAttack(DWORD ID, char pHp) {
    Player.LastAttack = timeGetTime();
    
    if (!IsRangedObject(&MainObject)) {
        switch (rand() % 3) {
        case 0: SoundFX[4].Play(FALSE); break;
        case 1: SoundFX[11].Play(FALSE); break;
        case 2: SoundFX[12].Play(FALSE); break;
        }
    } else {
        SoundFX[20].Play(FALSE);
    }
    
    if (ID == NULL)
        return;
    
    ToHead();
    TFCObject *Object;
    while(QueryNext()) {
        Object = GetObject();
        if (Object->ID == ID) {
            FXType *FXObject = new FXType;
            FXObject->Time = timeGetTime();
            FXObject->Debug = FALSE;
            FXObject->Sound.CopyWave(&Object->onAttacked[rand() % 3]);
            SoundFXList.Lock("SFX 4");
            SoundFXList.AddToTail(FXObject);
            SoundFXList.Unlock("SFX 4");
            if (pHp > 0)
                Object->pHp = pHp;
            SetQueryState(STOP_QUERY);
        }
    }
}

void VisualObjectList::Lock(int i) {
    csLock.Lock("");
    
    //if (i)
    //   g_TimeStat.StartBlockCount(GetCurrentThreadId(), i);
}

void VisualObjectList::Unlock(int i) {
    //if (i)
    //  g_TimeStat.StopBlockCount(GetCurrentThreadId(), i);
    csLock.Unlock("");
}

void VisualObjectList::SetMonsterStats(int Type, TFCObject *Object) {
    int a, b;
    LoadObject(Object->Type3D);
    if ((Type >= 15000 && Type < 20000) || (Type >= 25000 && Type < 30000)) {
        Object->Up = 0;
        Object->Down = 0;
        Object->Left = 0;
        Object->Right = 0;
        Object->CurrentCorpseFrame = VObject3D[Object->Type3D]->FacesC-1;
    } else {
        Object->onAttack[0].Release(FALSE);
        Object->onAttack[1].Release(FALSE);
        Object->onAttack[2].Release(FALSE);
        Object->onAttacked[0].Release(FALSE);
        Object->onAttacked[1].Release(FALSE);
        Object->onAttacked[2].Release(FALSE);
        Object->onKilled[0].Release(FALSE);
        Object->onKilled[1].Release(FALSE); 
        switch (Type) {
        case __SPELL_POISONARROW:
            Object->onAttack[0].CopyWave(&SoundFX[65]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_STONESHARD:
        case __SPELL_SWORD01:
            Object->onAttack[0].CopyWave(&SoundFX[65]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
        
        //++NMNMNM --[NEW TROWING Weopen Sound]
        /*
        case __SPELL_TROWING_STAR1:
        case __SPELL_TROWING_DAGGER1:
        */
        case __SPELL_ARROW:
           
            Object->onAttack[0].CopyWave(&SoundFX[65]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_ICESHARD:
            Object->onAttack[0].CopyWave(&SoundFX[65]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_LIGHTNINGBOLT:
            Object->onAttack[0].CopyWave(&SoundFX[60]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_FIREBALL:
            Object->onAttack[0].CopyWave(&SoundFX[117]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
            // BEGIN DIALSOFT SPELLS NEW
            // FIREBOLT
        case __SPELL_FIREBOLT_BLACK:
        case __SPELL_FIREBOLT_WHITE:
        case __SPELL_FIREBOLT_YELLOW:
        case __SPELL_FIREBOLT_GREEN:
        case __SPELL_FIREBOLT_BLUE:
        case __SPELL_FIREBOLT_PURPLE:
           // END DIALSOFT SPELLS NEW
        case __SPELL_FIREBOLT:
            Object->onAttack[0].CopyWave(&SoundFX[65]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_HEALING:
            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_SERAPH_MALE:
            Object->onAttack[0].CopyWave(&SoundFX[121]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_SERAPH:
            Object->onAttack[0].CopyWave(&SoundFX[121]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_SERAPH_FEMALE:
            Object->onAttack[0].CopyWave(&SoundFX[121]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_EVIL_SERAPH_MALE:
            Object->onAttack[0].CopyWave(&SoundFX[124]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_EVIL_SERAPH:
            Object->onAttack[0].CopyWave(&SoundFX[124]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_EVIL_SERAPH_FEMALE:
            Object->onAttack[0].CopyWave(&SoundFX[124]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;

        case __SPELL_DECHU_MALE:
        case __SPELL_DECHU_FEMALE:
        case __SPELL_DECHU:
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
        break;
            
        case __SPELL_BLESS:
            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_ENTANGLE:
            Object->onAttack[0].CopyWave(&SoundFX[123]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_GLACIER:
            Object->onAttack[0].CopyWave(&SoundFX[80]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_BOULDERS:
            Object->onAttack[0].CopyWave(&SoundFX[81]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_BLACK:
        case __SPELL_METEOR_WHITE:
        case __SPELL_METEOR_PURPLE:
        case __SPELL_METEOR_COBOLT:
        case __SPELL_METEOR_GREEN:
        case __SPELL_METEOR_CANDY:
        case __SPELL_METEOR_GOLD:
        case __SPELL_METEOR_BROWN:
        case __SPELL_METEOR_DARK:
        case __SPELL_METEOR_BLUE:
        case __SPELL_METEOR_VIOLET:
           // END DIALSOFT NEW SPELLS
        case __SPELL_METEORS:
            Object->onAttack[0].CopyWave(&SoundFX[82]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_GATEWAY_1:
            Object->onAttack[0].CopyWave(&SoundFX[111]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_HEALSERIOUS:
            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_POISONCLOUD:
            Object->onAttack[0].CopyWave(&SoundFX[59]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_ICECLOUD:
            Object->onAttack[0].CopyWave(&SoundFX[59]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_ROCKFLY:
            Object->onAttack[0].CopyWave(&SoundFX[63]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_FREEZE:
            Object->onAttack[0].CopyWave(&SoundFX[43]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_SEMIBIGEXPLOSION:
            Object->onAttack[0].CopyWave(&SoundFX[64]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_BROKEN_ARROW:
            Object->onAttack[0].CopyWave(&SoundFX[64]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_CURSE:
            Object->onAttack[0].CopyWave(&SoundFX[2]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_ELECTRICSHIELD:
            Object->onAttack[0].CopyWave(&SoundFX[7]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_GREENWIPE:
            Object->onAttack[0].CopyWave(&SoundFX[113]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_BLUEWIPE:
            Object->onAttack[0].CopyWave(&SoundFX[113]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_FIREWIPE:
            Object->onAttack[0].CopyWave(&SoundFX[113]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_PENTACLE:
            Object->onAttack[0].CopyWave(&SoundFX[44]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_REDWIPE:
            Object->onAttack[0].CopyWave(&SoundFX[113]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_SHOCKWAVE:
            Object->onAttack[0].CopyWave(&SoundFX[115]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_SMALLEXPLOSION:
            Object->onAttack[0].CopyWave(&SoundFX[114]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_GREATEXPLOSION:
            Object->onAttack[0].CopyWave(&SoundFX[114]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_FLAK2:
            Object->onAttack[0].CopyWave(&SoundFX[114]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_FLAK:
            Object->onAttack[0].CopyWave(&SoundFX[114]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
        
            // BEGIN DIALSOFT NEW SPELLS
            // CALL LIGHTNING
        case __SPELL_GREATBOLT_AZURE:
        case __SPELL_GREATBOLT_CRIMSON:
        case __SPELL_GREATBOLT_LIME:
        case __SPELL_GREATBOLT_EMERALD:
        case __SPELL_GREATBOLT_GREEN:
        case __SPELL_GREATBOLT_CANDY:
        case __SPELL_GREATBOLT_RED:
        case __SPELL_GREATBOLT_BLACK:
           // END DIALSOFT NEW SPELLS
        case __SPELL_GREATBOLT:
            Object->onAttack[0].CopyWave(&SoundFX[116]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
            
        case __SPELL_FIRECIRCLE:
            Object->onAttack[0].CopyWave(&SoundFX[122]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            //Dialsoft added code
            // BEGIN DIALSOFT SPELLS NEW
            // FIREBALL
        case __SPELL_NEWFIREBALL_BLACK:
        case __SPELL_NEWFIREBALL_WHITE:
        case __SPELL_NEWFIREBALL_AZURE:
        case __SPELL_NEWFIREBALL_MAGENTA:
        case __SPELL_NEWFIREBALL_YELLOW:
        case __SPELL_NEWFIREBALL_PURPLE:
           // END DIALSOFT SPELLS NEW
        case __SPELL_NEWFIREBALL:
        case __SPELL_NEWFIREBALL2:
            Object->onAttack[0].CopyWave(&SoundFX[117]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
//Dialsoft added code
		case __SPELL_MEDIUM_HEALING:
		case __SPELL_ICE_TREE:
		case __SPELL_SUPRAHEAL:
            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
//Dialsoft END
        case __SPELL_RED_ENERGY_BALL:
        case __SPELL_CURSE_WITH_RED_BALL:
        case __SPELL_FIREWIPE_WITH_RED_BALL:
        case __SPELL_FLAK_WITH_RED_BALL:
        case __SPELL_FLAK2_WITH_RED_BALL:
        case __SPELL_GATEWAY_1_WITH_RED_BALL:
        case __SPELL_GREATEXPLOSION_WITH_RED_BALL:
        case __SPELL_METEOR_WITH_RED_BALL:
        case __SPELL_PENTACLE_WITH_RED_BALL:
        case __SPELL_REDWIPE_WITH_RED_BALL:
        case __SPELL_SEMIBIGEXPLOSION_WITH_RED_BALL:
        case __SPELL_SMALLEXPLOSION_WITH_RED_BALL:
        case __SPELL_FIRECIRCLE_WITH_RED_BALL:
           // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_BLACK_WITH_RED_BALL:
        case __SPELL_METEOR_WHITE_WITH_RED_BALL:
        case __SPELL_METEOR_PURPLE_WITH_RED_BALL:
        case __SPELL_METEOR_COBOLT_WITH_RED_BALL:
        case __SPELL_METEOR_GREEN_WITH_RED_BALL:
        case __SPELL_METEOR_CANDY_WITH_RED_BALL:
        case __SPELL_METEOR_GOLD_WITH_RED_BALL:
        case __SPELL_METEOR_BROWN_WITH_RED_BALL:
        case __SPELL_METEOR_DARK_WITH_RED_BALL:
        case __SPELL_METEOR_BLUE_WITH_RED_BALL:
        case __SPELL_METEOR_VIOLET_WITH_RED_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_RED_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_RED_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_RED_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_RED_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_RED_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_RED_BALL:
        case __SPELL_GREATBOLT_RED_WITH_RED_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_RED_BALL:
           // END DIALSOFT NEW SPELLS

            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_BLUE_ENERGY_BALL:
        case __SPELL_BLUEWIPE_WITH_BLUE_BALL:
        case __SPELL_CURSE_WITH_BLUE_BALL:
        case __SPELL_ELECTRICSHIELD_WITH_BLUE_BALL:
        case __SPELL_FREEZE_WITH_BLUE_BALL:
        case __SPELL_GLACIER_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_WITH_BLUE_BALL:
        case __SPELL_GREENWIPE_WITH_BLUE_BALL:
        case __SPELL_HEALING_WITH_BLUE_BALL:
        case __SPELL_HEALSERIOUS_WITH_BLUE_BALL:
        case __SPELL_ICECLOUD_WITH_BLUE_BALL:
        case __SPELL_POISONCLOUD_WITH_BLUE_BALL:
           // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_WITH_BLUE_BALL:
        case __SPELL_METEOR_BLACK_WITH_BLUE_BALL:
        case __SPELL_METEOR_WHITE_WITH_BLUE_BALL:
        case __SPELL_METEOR_PURPLE_WITH_BLUE_BALL:
        case __SPELL_METEOR_COBOLT_WITH_BLUE_BALL:
        case __SPELL_METEOR_GREEN_WITH_BLUE_BALL:
        case __SPELL_METEOR_CANDY_WITH_BLUE_BALL:
        case __SPELL_METEOR_GOLD_WITH_BLUE_BALL:
        case __SPELL_METEOR_BROWN_WITH_BLUE_BALL:
        case __SPELL_METEOR_DARK_WITH_BLUE_BALL:
        case __SPELL_METEOR_BLUE_WITH_BLUE_BALL:
        case __SPELL_METEOR_VIOLET_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_RED_WITH_BLUE_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_BLUE_BALL:
           // END DIALSOFT NEW SPELLS
           
           Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_YELLOW_ENERGY_BALL:
        case __SPELL_BLESS_WITH_YELLOW_BALL:
        case __SPELL_BLUEWIPE_WITH_YELLOW_BALL:
        case __SPELL_CURSE_WITH_YELLOW_BALL:
        case __SPELL_ELECTRICSHIELD_WITH_YELLOW_BALL:
        case __SPELL_FREEZE_WITH_YELLOW_BALL:
        case __SPELL_GATEWAY_1_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_WITH_YELLOW_BALL:
        case __SPELL_ICECLOUD_WITH_YELLOW_BALL:
           // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_WITH_YELLOW_BALL:
        case __SPELL_METEOR_BLACK_WITH_YELLOW_BALL:
        case __SPELL_METEOR_WHITE_WITH_YELLOW_BALL:
        case __SPELL_METEOR_PURPLE_WITH_YELLOW_BALL:
        case __SPELL_METEOR_COBOLT_WITH_YELLOW_BALL:
        case __SPELL_METEOR_GREEN_WITH_YELLOW_BALL:
        case __SPELL_METEOR_CANDY_WITH_YELLOW_BALL:
        case __SPELL_METEOR_GOLD_WITH_YELLOW_BALL:
        case __SPELL_METEOR_BROWN_WITH_YELLOW_BALL:
        case __SPELL_METEOR_DARK_WITH_YELLOW_BALL:
        case __SPELL_METEOR_BLUE_WITH_YELLOW_BALL:
        case __SPELL_METEOR_VIOLET_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_RED_WITH_YELLOW_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_YELLOW_BALL:
           // END DIALSOFT NEW SPELLS

            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_GREEN_ENERGY_BALL:
        case __SPELL_BOULDERS_WITH_GREEN_BALL:
        case __SPELL_CURSE_WITH_GREEN_BALL:
        case __SPELL_ENTANGLE_WITH_GREEN_BALL:
        case __SPELL_FLAK_WITH_GREEN_BALL:
        case __SPELL_FLAK2_WITH_GREEN_BALL:
        case __SPELL_GREENWIPE_WITH_GREEN_BALL:
        case __SPELL_PENTACLE_WITH_GREEN_BALL:
        case __SPELL_POISONCLOUD_WITH_GREEN_BALL:
        case __SPELL_ROCKFLY_WITH_GREEN_BALL:
           // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_WITH_GREEN_BALL:
        case __SPELL_METEOR_BLACK_WITH_GREEN_BALL:
        case __SPELL_METEOR_WHITE_WITH_GREEN_BALL:
        case __SPELL_METEOR_PURPLE_WITH_GREEN_BALL:
        case __SPELL_METEOR_COBOLT_WITH_GREEN_BALL:
        case __SPELL_METEOR_GREEN_WITH_GREEN_BALL:
        case __SPELL_METEOR_CANDY_WITH_GREEN_BALL:
        case __SPELL_METEOR_GOLD_WITH_GREEN_BALL:
        case __SPELL_METEOR_BROWN_WITH_GREEN_BALL:
        case __SPELL_METEOR_DARK_WITH_GREEN_BALL:
        case __SPELL_METEOR_BLUE_WITH_GREEN_BALL:
        case __SPELL_METEOR_VIOLET_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_RED_WITH_GREEN_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_GREEN_BALL:
           // END DIALSOFT NEW SPELLS

            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_WHITE_ENERGY_BALL:
        case __SPELL_BLESS_WITH_WHITE_BALL:
        case __SPELL_BLUEWIPE_WITH_WHITE_BALL:
        case __SPELL_CURSE_WITH_WHITE_BALL:
        case __SPELL_ELECTRICSHIELD_WITH_WHITE_BALL:
        case __SPELL_FLAK_WITH_WHITE_BALL:
        case __SPELL_FLAK2_WITH_WHITE_BALL:
        case __SPELL_HEALING_WITH_WHITE_BALL:
        case __SPELL_HEALSERIOUS_WITH_WHITE_BALL:
        case __SPELL_ICECLOUD_WITH_WHITE_BALL:
           // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_WITH_WHITE_BALL:
        case __SPELL_METEOR_BLACK_WITH_WHITE_BALL:
        case __SPELL_METEOR_WHITE_WITH_WHITE_BALL:
        case __SPELL_METEOR_PURPLE_WITH_WHITE_BALL:
        case __SPELL_METEOR_COBOLT_WITH_WHITE_BALL:
        case __SPELL_METEOR_GREEN_WITH_WHITE_BALL:
        case __SPELL_METEOR_CANDY_WITH_WHITE_BALL:
        case __SPELL_METEOR_GOLD_WITH_WHITE_BALL:
        case __SPELL_METEOR_BROWN_WITH_WHITE_BALL:
        case __SPELL_METEOR_DARK_WITH_WHITE_BALL:
        case __SPELL_METEOR_BLUE_WITH_WHITE_BALL:
        case __SPELL_METEOR_VIOLET_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_RED_WITH_WHITE_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_WHITE_BALL:
           // END DIALSOFT NEW SPELLS

            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_BLACK_ENERGY_BALL:
        case __SPELL_CURSE_WITH_BLACK_BALL:
        case __SPELL_ENTANGLE_WITH_BLACK_BALL:
        case __SPELL_FIREWIPE_WITH_BLACK_BALL:
        case __SPELL_GREATEXPLOSION_WITH_BLACK_BALL:
        case __SPELL_GREENWIPE_WITH_BLACK_BALL:
        case __SPELL_METEOR_WITH_BLACK_BALL:
        case __SPELL_REDWIPE_WITH_BLACK_BALL:
        case __SPELL_SEMIBIGEXPLOSION_WITH_BLACK_BALL:
        case __SPELL_SMALLEXPLOSION_WITH_BLACK_BALL:
        case __SPELL_FIRECIRCLE_WITH_BLACK_BALL:
           // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_BLACK_WITH_BLACK_BALL:
        case __SPELL_METEOR_WHITE_WITH_BLACK_BALL:
        case __SPELL_METEOR_PURPLE_WITH_BLACK_BALL:
        case __SPELL_METEOR_COBOLT_WITH_BLACK_BALL:
        case __SPELL_METEOR_GREEN_WITH_BLACK_BALL:
        case __SPELL_METEOR_CANDY_WITH_BLACK_BALL:
        case __SPELL_METEOR_GOLD_WITH_BLACK_BALL:
        case __SPELL_METEOR_BROWN_WITH_BLACK_BALL:
        case __SPELL_METEOR_DARK_WITH_BLACK_BALL:
        case __SPELL_METEOR_BLUE_WITH_BLACK_BALL:
        case __SPELL_METEOR_VIOLET_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_RED_WITH_BLACK_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_BLACK_BALL:
           // END DIALSOFT NEW SPELLS

            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;
            
        case __SPELL_PURPLE_ENERGY_BALL:
        case __SPELL_BLUEWIPE_WITH_PURPLE_BALL:
        case __SPELL_CURSE_WITH_PURPLE_BALL:
        case __SPELL_ELECTRICSHIELD_WITH_PURPLE_BALL:
        case __SPELL_FLAK_WITH_PURPLE_BALL:
        case __SPELL_FLAK2_WITH_PURPLE_BALL:
        case __SPELL_GATEWAY_1_WITH_PURPLE_BALL:
        case __SPELL_GREATEXPLOSION_WITH_PURPLE_BALL:
        case __SPELL_GREENWIPE_WITH_PURPLE_BALL:
        case __SPELL_METEOR_WITH_PURPLE_BALL:
        case __SPELL_PENTACLE_WITH_PURPLE_BALL:
        case __SPELL_POISONCLOUD_WITH_PURPLE_BALL:
        case __SPELL_SEMIBIGEXPLOSION_WITH_PURPLE_BALL:
        case __SPELL_SMALLEXPLOSION_WITH_PURPLE_BALL:
        case __SPELL_FIRECIRCLE_WITH_PURPLE_BALL:
           // BEGIN DIALSOFT NEW SPELLS
        case __SPELL_METEOR_BLACK_WITH_PURPLE_BALL:
        case __SPELL_METEOR_WHITE_WITH_PURPLE_BALL:
        case __SPELL_METEOR_PURPLE_WITH_PURPLE_BALL:
        case __SPELL_METEOR_COBOLT_WITH_PURPLE_BALL:
        case __SPELL_METEOR_GREEN_WITH_PURPLE_BALL:
        case __SPELL_METEOR_CANDY_WITH_PURPLE_BALL:
        case __SPELL_METEOR_GOLD_WITH_PURPLE_BALL:
        case __SPELL_METEOR_BROWN_WITH_PURPLE_BALL:
        case __SPELL_METEOR_DARK_WITH_PURPLE_BALL:
        case __SPELL_METEOR_BLUE_WITH_PURPLE_BALL:
        case __SPELL_METEOR_VIOLET_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_AZURE_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_CRIMSON_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_LIME_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_EMERALD_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_GREEN_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_CANDY_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_RED_WITH_PURPLE_BALL:
        case __SPELL_GREATBOLT_BLACK_WITH_PURPLE_BALL:
           // END DIALSOFT NEW SPELLS

            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
            break;

        case __SPELL_SKILLS_EFFECT:
            Object->onAttack[0].CopyWave(&SoundFX[59]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
         break;
         case __SPELL_LEVELUP:
            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
         break;
         case __SPELL_NEW_LIGHTNING1:
            Object->onAttack[0].CopyWave(&SoundFX[125]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
         break;
         case __SPELL_NEW_FIRE1:
            Object->onAttack[0].CopyWave(&SoundFX[125]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
         break;
         case __SPELL_NEW_POISON1:
            Object->onAttack[0].CopyWave(&SoundFX[117]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
         break;
         case __SPELL_NEW_SUPRAHEAL://BLBL tentative d'ajouter 2 nouveau sorts des DDA
            Object->onAttack[0].CopyWave(&SoundFX[112]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
         break;
         case __SPELL_NEW_BOULDERS://BLBL tentative d'ajouter 2 nouveau sorts des DDA
            Object->onAttack[0].CopyWave(&SoundFX[81]);
            Object->Up = 0;
            Object->Down = 0;
            Object->Left = 0;
            Object->Right = 0;
         break;

         
            
            // Drake4
        case __MONSTER_HUMAN_SWORDMAN:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[11]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[99]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 1;
            Object->Slow = 1;
            Object->TextCorrection = -75;
            break;
            
        case __PLAYER_HUMAN_PUPPET:
            VObject3D[71]->SetPuppet(Object);
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[11]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttack[3].CopyWave(&SoundFX[20]);
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[99]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 0;
            Object->Slow = 1;
            Object->TextCorrection = -75;
            break;
            
        case __PLAYER_HUMAN_FEMALE:
            VObject3D[71]->SetPuppet(Object);
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[11]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttack[3].CopyWave(&SoundFX[20]);
            Object->onAttacked[0].CopyWave(&SoundFX[53]);
            Object->onAttacked[1].CopyWave(&SoundFX[54]);
            Object->onAttacked[2].CopyWave(&SoundFX[54]);
            Object->onKilled[0].CopyWave(&SoundFX[51]);
            Object->onKilled[1].CopyWave(&SoundFX[52]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 0;
            Object->Slow = 1;
            Object->TextCorrection = -75;
            break;
            
        case __NPC_HUMAN_GUARD:
            Object->onAttack[0].CopyWave(&SoundFX[16]);
            Object->onAttack[1].CopyWave(&SoundFX[16]);
            Object->onAttack[2].CopyWave(&SoundFX[18]);
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[99]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __NPC_HUMAN_WARRIOR:
            Object->onAttack[0].CopyWave(&SoundFX[12]);
            Object->onAttack[1].CopyWave(&SoundFX[12]);
            Object->onAttack[2].CopyWave(&SoundFX[15]);
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[99]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __NPC_HUMAN_RED_MAGE:
            Object->onAttack[0].CopyWave(&SoundFX[19]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[13]);
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[99]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __NPC_HUMAN_PEASANT:
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[99]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __NPC_HUMAN_PRIEST:
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[99]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __NPC_HUMAN_PAYSANNE:
            Object->onAttacked[0].CopyWave(&SoundFX[53]);
            Object->onAttacked[1].CopyWave(&SoundFX[54]);
            Object->onAttacked[2].CopyWave(&SoundFX[54]);
            Object->onKilled[0].CopyWave(&SoundFX[51]);
            Object->onKilled[1].CopyWave(&SoundFX[52]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_HUMAN_MAGE:
            Object->onAttack[0].CopyWave(&SoundFX[19]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[13]);
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[99]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 1;
            Object->Slow = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_HUMAN_PRIEST:
            Object->onAttack[0].CopyWave(&SoundFX[11]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[11]);
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[99]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 1;
            Object->Slow = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_HUMAN_THIEF:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[14]);
            Object->onAttack[2].CopyWave(&SoundFX[15]);
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[99]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 1;
            Object->Slow = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_KOBOLD:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[11]);
            Object->onAttack[2].CopyWave(&SoundFX[15]);
            Object->onAttacked[0].CopyWave(&SoundFX[5]);
            Object->onAttacked[1].CopyWave(&SoundFX[5]);
            Object->onAttacked[2].CopyWave(&SoundFX[5]);
            Object->onKilled[0].CopyWave(&SoundFX[3]);
            Object->onKilled[1].CopyWave(&SoundFX[3]);
            Object->Up = 5;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -55;
            break;
            
        case __MONSTER_OOZE:
		case __MONSTER_FOOZE_DARKGREEN:
		case __MONSTER_FOOZE_DARKYELLOW:
		case __MONSTER_FOOZE_GREEN:
		case __MONSTER_FOOZE_RED:
		case __MONSTER_FOOZE_BLACK:
		case __MONSTER_FOOZE_YELLOW:
		case __MONSTER_FOOZE_WHITE:
      case __MONSTER_BLOB_R:
      case __MONSTER_BLOB_G:
            Object->onAttack[0].CopyWave(&SoundFX[31]);
            Object->onAttack[1].CopyWave(&SoundFX[31]);
            Object->onAttack[2].CopyWave(&SoundFX[31]);
            Object->onAttacked[0].CopyWave(&SoundFX[32]);
            Object->onAttacked[1].CopyWave(&SoundFX[32]);
            Object->onAttacked[2].CopyWave(&SoundFX[32]);
            Object->onKilled[0].CopyWave(&SoundFX[30]);
            Object->onKilled[1].CopyWave(&SoundFX[30]);
            Object->Up = 3;
            Object->Down = 3;
            Object->Left = 3;
            Object->Right = 2;
            Object->TextCorrection = -20;
            break;
            
        case __MONSTER_ORC:
            Object->onAttack[0].CopyWave(&SoundFX[103]);
            Object->onAttack[1].CopyWave(&SoundFX[103]);
            Object->onAttack[2].CopyWave(&SoundFX[103]);
            Object->onAttacked[0].CopyWave(&SoundFX[106]);
            Object->onAttacked[1].CopyWave(&SoundFX[106]);
            Object->onAttacked[2].CopyWave(&SoundFX[106]);
            Object->onKilled[0].CopyWave(&SoundFX[105]);
            Object->onKilled[1].CopyWave(&SoundFX[105]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 2;
            Object->TextCorrection = -80;
            break;
            
        case __MONSTER_ZOMBIE:
            Object->onAttack[0].CopyWave(&SoundFX[67]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[19]);
            Object->onAttacked[0].CopyWave(&SoundFX[84]);
            Object->onAttacked[1].CopyWave(&SoundFX[84]);
            Object->onAttacked[2].CopyWave(&SoundFX[84]);
            Object->onKilled[0].CopyWave(&SoundFX[83]);
            Object->onKilled[1].CopyWave(&SoundFX[83]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_WOLF:
        case __MONSTER_WHITECAT://dialsoft white cat
            Object->onAttack[0].CopyWave(&SoundFX[9]);
            Object->onAttack[1].CopyWave(&SoundFX[9]);
            Object->onAttack[2].CopyWave(&SoundFX[9]);
            Object->onAttacked[0].CopyWave(&SoundFX[10]);
            Object->onAttacked[1].CopyWave(&SoundFX[10]);
            Object->onAttacked[2].CopyWave(&SoundFX[10]);
            Object->onKilled[0].CopyWave(&SoundFX[8]);
            Object->onKilled[1].CopyWave(&SoundFX[8]);
            Object->Up = 2;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -40;
            break;
            
        case __MONSTER_DARKWOLF:
        case __MONSTER_BIGWOLF1:
        case __MONSTER_BIGWOLF2:
            Object->onAttack[0].CopyWave(&SoundFX[9]);
            Object->onAttack[1].CopyWave(&SoundFX[9]);
            Object->onAttack[2].CopyWave(&SoundFX[9]);
            Object->onAttacked[0].CopyWave(&SoundFX[10]);
            Object->onAttacked[1].CopyWave(&SoundFX[10]);
            Object->onAttacked[2].CopyWave(&SoundFX[10]);
            Object->onKilled[0].CopyWave(&SoundFX[8]);
            Object->onKilled[1].CopyWave(&SoundFX[8]);
            Object->Up = 4;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_BRIGAND:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[14]);
            Object->onAttack[2].CopyWave(&SoundFX[15]);
            Object->onAttacked[0].CopyWave(&SoundFX[98]);
            Object->onAttacked[1].CopyWave(&SoundFX[98]);
            Object->onAttacked[2].CopyWave(&SoundFX[99]);
            Object->onKilled[0].CopyWave(&SoundFX[96]);
            Object->onKilled[1].CopyWave(&SoundFX[97]);
            Object->Up = 5;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_RED_GOBLIN:
		case __MONSTER_FRED_GOBLIN_BLUE:
		case __MONSTER_FRED_GOBLIN_GREEN:
		case __MONSTER_FRED_GOBLIN_LIGHTBLUE:
		case __MONSTER_FRED_GOBLIN_GRAY:
		case __MONSTER_FRED_GOBLIN_YELLOW:
		case __MONSTER_FRED_GOBLIN_WHITE:
		case __MONSTER_FRED_GOBLIN_VIOLET:
            Object->onAttack[0].CopyWave(&SoundFX[11]);
            Object->onAttack[1].CopyWave(&SoundFX[11]);
            Object->onAttack[2].CopyWave(&SoundFX[18]);
            Object->onAttacked[0].CopyWave(&SoundFX[34]);
            Object->onAttacked[1].CopyWave(&SoundFX[34]);
            Object->onAttacked[2].CopyWave(&SoundFX[34]);
            Object->onKilled[0].CopyWave(&SoundFX[33]);
            Object->onKilled[1].CopyWave(&SoundFX[33]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_DRACONIAN_PLATE:
            Object->onAttack[0].CopyWave(&SoundFX[12]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[19]);
            Object->onAttacked[0].CopyWave(&SoundFX[29]);
            Object->onAttacked[1].CopyWave(&SoundFX[29]);
            Object->onAttacked[2].CopyWave(&SoundFX[29]);
            Object->onKilled[0].CopyWave(&SoundFX[27]);
            Object->onKilled[1].CopyWave(&SoundFX[27]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_DRACONIAN_ROBE:
            Object->onAttack[0].CopyWave(&SoundFX[12]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[19]);
            Object->onAttacked[0].CopyWave(&SoundFX[29]);
            Object->onAttacked[1].CopyWave(&SoundFX[29]);
            Object->onAttacked[2].CopyWave(&SoundFX[29]);
            Object->onKilled[0].CopyWave(&SoundFX[27]);
            Object->onKilled[1].CopyWave(&SoundFX[27]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_DRACONIAN_LEATHER:
            Object->onAttack[0].CopyWave(&SoundFX[12]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[19]);
            Object->onAttacked[0].CopyWave(&SoundFX[29]);
            Object->onAttacked[1].CopyWave(&SoundFX[29]);
            Object->onAttacked[2].CopyWave(&SoundFX[29]);
            Object->onKilled[0].CopyWave(&SoundFX[27]);
            Object->onKilled[1].CopyWave(&SoundFX[27]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_SKELETON_CENTAUR:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[18]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttacked[0].CopyWave(&SoundFX[39]);
            Object->onAttacked[1].CopyWave(&SoundFX[39]);
            Object->onAttacked[2].CopyWave(&SoundFX[39]);
            Object->onKilled[0].CopyWave(&SoundFX[38]);
            Object->onKilled[1].CopyWave(&SoundFX[38]);
            Object->Up = 6;
            Object->Down = 1;
            Object->Left = 2;
            Object->Right = 0;
            Object->TextCorrection = -100;
            break;
            
        case __MONSTER_SKELETON_SERVANT_1:
        case __MONSTER_DEMONSKULL:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[18]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttacked[0].CopyWave(&SoundFX[39]);
            Object->onAttacked[1].CopyWave(&SoundFX[39]);
            Object->onAttacked[2].CopyWave(&SoundFX[39]);
            Object->onKilled[0].CopyWave(&SoundFX[38]);
            Object->onKilled[1].CopyWave(&SoundFX[38]);
            Object->Up = 6;
            Object->Down = 1;
            Object->Left = 2;
            Object->Right = 0;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_SKELETON_SERVANT_2:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[18]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttacked[0].CopyWave(&SoundFX[39]);
            Object->onAttacked[1].CopyWave(&SoundFX[39]);
            Object->onAttacked[2].CopyWave(&SoundFX[39]);
            Object->onKilled[0].CopyWave(&SoundFX[38]);
            Object->onKilled[1].CopyWave(&SoundFX[38]);
            Object->Up = 6;
            Object->Down = 1;
            Object->Left = 2;
            Object->Right = 0;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_SKELETON_KING:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[18]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttacked[0].CopyWave(&SoundFX[39]);
            Object->onAttacked[1].CopyWave(&SoundFX[39]);
            Object->onAttacked[2].CopyWave(&SoundFX[39]);
            Object->onKilled[0].CopyWave(&SoundFX[38]);
            Object->onKilled[1].CopyWave(&SoundFX[38]);
            Object->Up = 7;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -96;
            break;
            
        case __MONSTER_LICH:
            Object->onAttack[0].CopyWave(&SoundFX[12]);
            Object->onAttack[1].CopyWave(&SoundFX[12]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttacked[0].CopyWave(&SoundFX[120]);
            Object->onAttacked[1].CopyWave(&SoundFX[120]);
            Object->onAttacked[2].CopyWave(&SoundFX[120]);
            Object->onKilled[0].CopyWave(&SoundFX[44]);
            Object->onKilled[1].CopyWave(&SoundFX[44]);
            Object->Up = 8 ;
            Object->Down = 1;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_CENTAUR_WARRIOR:
            Object->onAttack[0].CopyWave(&SoundFX[19]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[19]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[95]);
            Object->onKilled[1].CopyWave(&SoundFX[95]);
            Object->Up = 6;
            Object->Down = 1;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -96;
            break;
            
        case __MONSTER_CENTAUR_ARCHER:
            Object->onAttack[0].CopyWave(&SoundFX[19]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[19]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[95]);
            Object->onKilled[1].CopyWave(&SoundFX[95]);
            Object->Up = 6;
            Object->Down = 1;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -96;
            break;
            
        case __MONSTER_CENTAUR_SHAMAN:
            Object->onAttack[0].CopyWave(&SoundFX[19]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[19]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[95]);
            Object->onKilled[1].CopyWave(&SoundFX[95]);
            Object->Up = 6;
            Object->Down = 1;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -96;
            break;
            
        case __MONSTER_CENTAUR_KING:
            Object->onAttack[0].CopyWave(&SoundFX[19]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[19]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[95]);
            Object->onKilled[1].CopyWave(&SoundFX[95]);
            Object->Up = 6;
            Object->Down = 1;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -96;
            break;
            
        case __MONSTER_SKAVEN_PEON:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_SKAVEN_PEON2:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;

         case __MONSTER_SKAVEN_PEON3:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_SKAVEN_SHAMAN:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_SKAVEN_SHAMAN2:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;

          case __MONSTER_SKAVEN_SHAMAN3:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_SKAVEN_SKAVENGER:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_SKAVEN_SKAVENGER2:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;

         case __MONSTER_SKAVEN_SKAVENGER3:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_SKAVEN_WARRIOR:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_SKAVEN_WARRIOR2:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;

         case __MONSTER_SKAVEN_WARRIOR3:
            Object->onAttack[0].CopyWave(&SoundFX[118]);
            Object->onAttack[1].CopyWave(&SoundFX[118]);
            Object->onAttack[2].CopyWave(&SoundFX[118]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[119]);
            Object->onKilled[1].CopyWave(&SoundFX[119]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_RED_GOBLINBOSS:
            Object->onAttack[0].CopyWave(&SoundFX[11]);
            Object->onAttack[1].CopyWave(&SoundFX[18]);
            Object->onAttack[2].CopyWave(&SoundFX[18]);
            Object->onAttacked[0].CopyWave(&SoundFX[34]);
            Object->onAttacked[1].CopyWave(&SoundFX[34]);
            Object->onAttacked[2].CopyWave(&SoundFX[34]);
            Object->onKilled[0].CopyWave(&SoundFX[33]);
            Object->onKilled[1].CopyWave(&SoundFX[33]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -60;
            break;
            
        case __MONSTER_UNICORN:
            Object->onAttack[0].CopyWave(&SoundFX[101]);
            Object->onAttack[1].CopyWave(&SoundFX[101]);
            Object->onAttack[2].CopyWave(&SoundFX[101]);
            Object->onAttacked[0].CopyWave(&SoundFX[102]);
            Object->onAttacked[1].CopyWave(&SoundFX[102]);
            Object->onAttacked[2].CopyWave(&SoundFX[102]);
            Object->onKilled[0].CopyWave(&SoundFX[100]);
            Object->onKilled[1].CopyWave(&SoundFX[100]);
            Object->Up = 0;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -100;
            break;
            
        case __MONSTER_WASP:
            Object->onAttack[0].CopyWave(&SoundFX[88]);
            Object->onAttack[1].CopyWave(&SoundFX[88]);
            Object->onAttack[2].CopyWave(&SoundFX[88]);
            Object->onAttacked[0].CopyWave(&SoundFX[89]);
            Object->onAttacked[1].CopyWave(&SoundFX[89]);
            Object->onAttacked[2].CopyWave(&SoundFX[89]);
            Object->onKilled[0].CopyWave(&SoundFX[87]);
            Object->onKilled[1].CopyWave(&SoundFX[87]);
            Object->Up = 8;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
         case __MONSTER_BIGWASP:
            Object->onAttack[0].CopyWave(&SoundFX[88]);
            Object->onAttack[1].CopyWave(&SoundFX[88]);
            Object->onAttack[2].CopyWave(&SoundFX[88]);
            Object->onAttacked[0].CopyWave(&SoundFX[89]);
            Object->onAttacked[1].CopyWave(&SoundFX[89]);
            Object->onAttacked[2].CopyWave(&SoundFX[89]);
            Object->onKilled[0].CopyWave(&SoundFX[87]);
            Object->onKilled[1].CopyWave(&SoundFX[87]);
            Object->Up = 8;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -110;
            break;
            
        case __MONSTER_DRAGON:
            Object->onAttack[0].CopyWave(&SoundFX[88]);
            Object->onAttack[1].CopyWave(&SoundFX[88]);
            Object->onAttack[2].CopyWave(&SoundFX[88]);
            Object->onAttacked[0].CopyWave(&SoundFX[89]);
            Object->onAttacked[1].CopyWave(&SoundFX[89]);
            Object->onAttacked[2].CopyWave(&SoundFX[89]);
            Object->onKilled[0].CopyWave(&SoundFX[87]);
            Object->onKilled[1].CopyWave(&SoundFX[87]);
            Object->Up = 15;
            Object->Down = 5;
            Object->Left = 6;
            Object->Right = 6;
            Object->TextCorrection = -130;
            break;
            
        case __MONSTER_SCORPION:
            Object->onAttack[0].CopyWave(&SoundFX[58]);
            Object->onAttack[1].CopyWave(&SoundFX[58]);
            Object->onAttack[2].CopyWave(&SoundFX[58]);
            Object->onAttacked[0].CopyWave(&SoundFX[86]);
            Object->onAttacked[1].CopyWave(&SoundFX[86]);
            Object->onAttacked[2].CopyWave(&SoundFX[86]);
            Object->onKilled[0].CopyWave(&SoundFX[85]);
            Object->onKilled[1].CopyWave(&SoundFX[85]);
            Object->Up = 2;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -40;
            break;
            
        case __MONSTER_KRAANIAN:
            Object->onAttack[0].CopyWave(&SoundFX[49]);
            Object->onAttack[1].CopyWave(&SoundFX[49]);
            Object->onAttack[2].CopyWave(&SoundFX[49]);
            Object->onAttacked[0].CopyWave(&SoundFX[50]);
            Object->onAttacked[1].CopyWave(&SoundFX[50]);
            Object->onAttacked[2].CopyWave(&SoundFX[50]);
            Object->onKilled[0].CopyWave(&SoundFX[48]);
            Object->onKilled[1].CopyWave(&SoundFX[48]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -50;
            break;
            
        case __MONSTER_NIGHTMARE:
            Object->onAttack[0].CopyWave(&SoundFX[101]);
            Object->onAttack[1].CopyWave(&SoundFX[101]);
            Object->onAttack[2].CopyWave(&SoundFX[101]);
            Object->onAttacked[0].CopyWave(&SoundFX[102]);
            Object->onAttacked[1].CopyWave(&SoundFX[102]);
            Object->onAttacked[2].CopyWave(&SoundFX[102]);
            Object->onKilled[0].CopyWave(&SoundFX[100]);
            Object->onKilled[1].CopyWave(&SoundFX[100]);
            Object->Up = 0;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 1;
            break;
            
        case __MONSTER_ATROCITY:
            Object->onAttack[0].CopyWave(&SoundFX[92]);
            Object->onAttack[1].CopyWave(&SoundFX[92]);
            Object->onAttack[2].CopyWave(&SoundFX[92]);
            Object->onAttacked[0].CopyWave(&SoundFX[93]);
            Object->onAttacked[1].CopyWave(&SoundFX[93]);
            Object->onAttacked[2].CopyWave(&SoundFX[93]);
            Object->onKilled[0].CopyWave(&SoundFX[91]);
            Object->onKilled[1].CopyWave(&SoundFX[91]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -70;
            break;
            
        case __MONSTER_ATROCITYBOSS:
            Object->onAttack[0].CopyWave(&SoundFX[92]);
            Object->onAttack[1].CopyWave(&SoundFX[92]);
            Object->onAttack[2].CopyWave(&SoundFX[92]);
            Object->onAttacked[0].CopyWave(&SoundFX[93]);
            Object->onAttacked[1].CopyWave(&SoundFX[93]);
            Object->onAttacked[2].CopyWave(&SoundFX[93]);
            Object->onKilled[0].CopyWave(&SoundFX[91]);
            Object->onKilled[1].CopyWave(&SoundFX[91]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -90;
            break;
            
        case __MONSTER_PEGASUS:
            Object->onAttack[0].CopyWave(&SoundFX[101]);
            Object->onAttack[1].CopyWave(&SoundFX[101]);
            Object->onAttack[2].CopyWave(&SoundFX[101]);
            Object->onAttacked[0].CopyWave(&SoundFX[102]);
            Object->onAttacked[1].CopyWave(&SoundFX[102]);
            Object->onAttacked[2].CopyWave(&SoundFX[102]);
            Object->onKilled[0].CopyWave(&SoundFX[100]);
            Object->onKilled[1].CopyWave(&SoundFX[100]);
            Object->onParry.CopyWave(&SoundFX[7]);
            Object->Up = 0;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 1;
            break;
            
        case __MONSTER_HORSE:
            Object->onAttack[0].CopyWave(&SoundFX[101]);
            Object->onAttack[1].CopyWave(&SoundFX[101]);
            Object->onAttack[2].CopyWave(&SoundFX[101]);
            Object->onAttacked[0].CopyWave(&SoundFX[102]);
            Object->onAttacked[1].CopyWave(&SoundFX[102]);
            Object->onAttacked[2].CopyWave(&SoundFX[102]);
            Object->onKilled[0].CopyWave(&SoundFX[100]);
            Object->onKilled[1].CopyWave(&SoundFX[100]);
            Object->Up = 9;
            Object->Down = 1;
            Object->Left = 3;
            Object->Right = 3;
            Object->TextCorrection = -100;
            break;
            
        case __MONSTER_CENTAUR:
            Object->onAttack[0].CopyWave(&SoundFX[19]);
            Object->onAttack[1].CopyWave(&SoundFX[19]);
            Object->onAttack[2].CopyWave(&SoundFX[104]);
            Object->onAttacked[0].CopyWave(&SoundFX[94]);
            Object->onAttacked[1].CopyWave(&SoundFX[94]);
            Object->onAttacked[2].CopyWave(&SoundFX[94]);
            Object->onKilled[0].CopyWave(&SoundFX[95]);
            Object->onKilled[1].CopyWave(&SoundFX[95]);
            Object->Up = 8;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 3;
            break;
            
        case __MONSTER_SNAKE:
            Object->onAttack[0].CopyWave(&SoundFX[70]);
            Object->onAttack[1].CopyWave(&SoundFX[70]);
            Object->onAttack[2].CopyWave(&SoundFX[70]);
            Object->onAttacked[0].CopyWave(&SoundFX[71]);
            Object->onAttacked[1].CopyWave(&SoundFX[71]);
            Object->onAttacked[2].CopyWave(&SoundFX[71]);
            Object->onKilled[0].CopyWave(&SoundFX[69]);
            Object->onKilled[1].CopyWave(&SoundFX[69]);
            Object->Up = 8;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -50;
            break;
            
        case __MONSTER_TREE_ENT:
            Object->onAttack[0].CopyWave(&SoundFX[76]);
            Object->onAttack[1].CopyWave(&SoundFX[76]);
            Object->onAttack[2].CopyWave(&SoundFX[76]);
            Object->onAttacked[0].CopyWave(&SoundFX[90]);
            Object->onAttacked[1].CopyWave(&SoundFX[90]);
            Object->onAttacked[2].CopyWave(&SoundFX[90]);
            Object->onKilled[0].CopyWave(&SoundFX[75]);
            Object->onKilled[1].CopyWave(&SoundFX[75]);
            Object->Up = 13;
            Object->Down = 3;
            Object->Left = 4;
            Object->Right = 4;
            Object->TextCorrection = -100;
            break;
            
        case __MONSTER_SMALL_WORM:
            Object->onAttack[0].CopyWave(&SoundFX[41]);
            Object->onAttack[1].CopyWave(&SoundFX[41]);
            Object->onAttack[2].CopyWave(&SoundFX[41]);
            Object->onAttacked[0].CopyWave(&SoundFX[42]);
            Object->onAttacked[1].CopyWave(&SoundFX[42]);
            Object->onAttacked[2].CopyWave(&SoundFX[42]);
            Object->onKilled[0].CopyWave(&SoundFX[40]);
            Object->onKilled[1].CopyWave(&SoundFX[40]);
            Object->Up = 3;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -25;
            break;
            
        case __MONSTER_BIG_WORM:
            Object->onAttack[0].CopyWave(&SoundFX[41]);
            Object->onAttack[1].CopyWave(&SoundFX[41]);
            Object->onAttack[2].CopyWave(&SoundFX[41]);
            Object->onAttacked[0].CopyWave(&SoundFX[42]);
            Object->onAttacked[1].CopyWave(&SoundFX[42]);
            Object->onAttacked[2].CopyWave(&SoundFX[42]);
            Object->onKilled[0].CopyWave(&SoundFX[40]);
            Object->onKilled[1].CopyWave(&SoundFX[40]);
            Object->Up = 5;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 2;
            Object->TextCorrection = -50;
            break;
            
        case __MONSTER_GREEN_TROLL:
            Object->onAttack[0].CopyWave(&SoundFX[22]);
            Object->onAttack[1].CopyWave(&SoundFX[22]);
            Object->onAttack[2].CopyWave(&SoundFX[22]);
            Object->onAttacked[0].CopyWave(&SoundFX[23]);
            Object->onAttacked[1].CopyWave(&SoundFX[23]);
            Object->onAttacked[2].CopyWave(&SoundFX[23]);
            Object->onKilled[0].CopyWave(&SoundFX[21]);
            Object->onKilled[1].CopyWave(&SoundFX[21]);
            Object->Up = 8;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 2;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_BEHOLDER:
            Object->onAttack[0].CopyWave(&SoundFX[46]);
            Object->onAttack[1].CopyWave(&SoundFX[46]);
            Object->onAttack[2].CopyWave(&SoundFX[46]);
            Object->onAttacked[0].CopyWave(&SoundFX[47]);
            Object->onAttacked[1].CopyWave(&SoundFX[47]);
            Object->onAttacked[2].CopyWave(&SoundFX[47]);
            Object->onKilled[0].CopyWave(&SoundFX[45]);
            Object->onKilled[1].CopyWave(&SoundFX[45]);
            Object->Up = 8;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 2;
            break;
            
        case __MONSTER_MUMMY:
            Object->onAttack[0].CopyWave(&SoundFX[67]);
            Object->onAttack[1].CopyWave(&SoundFX[67]);
            Object->onAttack[2].CopyWave(&SoundFX[67]);
            Object->onAttacked[0].CopyWave(&SoundFX[68]);
            Object->onAttacked[1].CopyWave(&SoundFX[68]);
            Object->onAttacked[2].CopyWave(&SoundFX[68]);
            Object->onKilled[0].CopyWave(&SoundFX[66]);
            Object->onKilled[1].CopyWave(&SoundFX[66]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_KRAANIANFLYING:
            Object->onAttack[0].CopyWave(&SoundFX[56]);
            Object->onAttack[1].CopyWave(&SoundFX[56]);
            Object->onAttack[2].CopyWave(&SoundFX[56]);
            Object->onAttacked[0].CopyWave(&SoundFX[57]);
            Object->onAttacked[1].CopyWave(&SoundFX[57]);
            Object->onAttacked[2].CopyWave(&SoundFX[57]);
            Object->onKilled[0].CopyWave(&SoundFX[55]);
            Object->onKilled[1].CopyWave(&SoundFX[55]);
            Object->Up = 8;
            Object->Down = 1;
            Object->Left = 3;
            Object->Right = 2;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_TARANTULA:
        case __MONSTER_TARANTULA2:
            Object->onAttack[0].CopyWave(&SoundFX[73]);
            Object->onAttack[1].CopyWave(&SoundFX[73]);
            Object->onAttack[2].CopyWave(&SoundFX[73]);
            Object->onAttacked[0].CopyWave(&SoundFX[74]);
            Object->onAttacked[1].CopyWave(&SoundFX[74]);
            Object->onAttacked[2].CopyWave(&SoundFX[74]);
            Object->onKilled[0].CopyWave(&SoundFX[72]);
            Object->onKilled[1].CopyWave(&SoundFX[72]);
            Object->Up = 6;
            Object->Down = 4;
            Object->Left = 3;
            Object->Right = 3;
            Object->TextCorrection = -75;
            break;
        case __MONSTER_BIGTARANTULA1:
        case __MONSTER_BIGTARANTULA2:
            Object->onAttack[0].CopyWave(&SoundFX[73]);
            Object->onAttack[1].CopyWave(&SoundFX[73]);
            Object->onAttack[2].CopyWave(&SoundFX[73]);
            Object->onAttacked[0].CopyWave(&SoundFX[74]);
            Object->onAttacked[1].CopyWave(&SoundFX[74]);
            Object->onAttacked[2].CopyWave(&SoundFX[74]);
            Object->onKilled[0].CopyWave(&SoundFX[72]);
            Object->onKilled[1].CopyWave(&SoundFX[72]);
            Object->Up = 6;
            Object->Down = 4;
            Object->Left = 3;
            Object->Right = 3;
            Object->TextCorrection = -85;
            break;
            
        case __MONSTER_AGMORKIAN:
            Object->onAttack[0].CopyWave(&SoundFX[56]);
            Object->onAttack[1].CopyWave(&SoundFX[56]);
            Object->onAttack[2].CopyWave(&SoundFX[56]);
            Object->onAttacked[0].CopyWave(&SoundFX[57]);
            Object->onAttacked[1].CopyWave(&SoundFX[57]);
            Object->onAttacked[2].CopyWave(&SoundFX[57]);
            Object->onKilled[0].CopyWave(&SoundFX[55]);
            Object->onKilled[1].CopyWave(&SoundFX[55]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 1;
            Object->Right = 0;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_KRAANIANTANK:
            Object->onAttack[0].CopyWave(&SoundFX[49]);
            Object->onAttack[1].CopyWave(&SoundFX[49]);
            Object->onAttack[2].CopyWave(&SoundFX[49]);
            Object->onAttacked[0].CopyWave(&SoundFX[50]);
            Object->onAttacked[1].CopyWave(&SoundFX[50]);
            Object->onAttacked[2].CopyWave(&SoundFX[50]);
            Object->onKilled[0].CopyWave(&SoundFX[48]);
            Object->onKilled[1].CopyWave(&SoundFX[48]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_TAUNTING:
            Object->onAttack[0].CopyWave(&SoundFX[78]);
            Object->onAttack[1].CopyWave(&SoundFX[78]);
            Object->onAttack[2].CopyWave(&SoundFX[78]);
            Object->onAttacked[0].CopyWave(&SoundFX[79]);
            Object->onAttacked[1].CopyWave(&SoundFX[79]);
            Object->onAttacked[2].CopyWave(&SoundFX[79]);
            Object->onKilled[0].CopyWave(&SoundFX[77]);
            Object->onKilled[1].CopyWave(&SoundFX[77]);
            Object->Up = 8;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 3;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_KRAANIANMILIPEDE:
            Object->onAttack[0].CopyWave(&SoundFX[49]);
            Object->onAttack[1].CopyWave(&SoundFX[49]);
            Object->onAttack[2].CopyWave(&SoundFX[49]);
            Object->onAttacked[0].CopyWave(&SoundFX[50]);
            Object->onAttacked[1].CopyWave(&SoundFX[50]);
            Object->onAttacked[2].CopyWave(&SoundFX[50]);
            Object->onKilled[0].CopyWave(&SoundFX[49]);
            Object->onKilled[1].CopyWave(&SoundFX[49]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_ORI:
            Object->onAttack[0].CopyWave(&SoundFX[67]);
            Object->onAttack[1].CopyWave(&SoundFX[67]);
            Object->onAttack[2].CopyWave(&SoundFX[67]);
            Object->onAttacked[0].CopyWave(&SoundFX[68]);
            Object->onAttacked[1].CopyWave(&SoundFX[68]);
            Object->onAttacked[2].CopyWave(&SoundFX[68]);
            Object->onKilled[0].CopyWave(&SoundFX[66]);
            Object->onKilled[1].CopyWave(&SoundFX[66]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_PIG:
            Object->onAttack[0].CopyWave(&SoundFX[62]);
            Object->onAttack[1].CopyWave(&SoundFX[62]);
            Object->onAttack[2].CopyWave(&SoundFX[62]);
            Object->onAttacked[0].CopyWave(&SoundFX[62]);
            Object->onAttacked[1].CopyWave(&SoundFX[62]);
            Object->onAttacked[2].CopyWave(&SoundFX[62]);
            Object->onKilled[0].CopyWave(&SoundFX[61]);
            Object->onKilled[1].CopyWave(&SoundFX[61]);
            Object->Up = 5;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_BAT:
            Object->onAttack[0].CopyWave(&SoundFX[25]);
            Object->onAttack[1].CopyWave(&SoundFX[25]);
            Object->onAttack[2].CopyWave(&SoundFX[25]);
            Object->onAttacked[0].CopyWave(&SoundFX[26]);
            Object->onAttacked[1].CopyWave(&SoundFX[26]);
            Object->onAttacked[2].CopyWave(&SoundFX[26]);
            Object->onKilled[0].CopyWave(&SoundFX[24]);
            Object->onKilled[1].CopyWave(&SoundFX[24]);
            Object->Up = 6;
            Object->Down = 1;
            Object->Left = 3;
            Object->Right = 1;
            break;
            
        case __MONSTER_RAT:
        case __MONSTER_TWIGLEE:      
            Object->onAttack[0].CopyWave(&SoundFX[36]);
            Object->onAttack[1].CopyWave(&SoundFX[36]);
            Object->onAttack[2].CopyWave(&SoundFX[36]);
            Object->onAttacked[0].CopyWave(&SoundFX[37]);
            Object->onAttacked[1].CopyWave(&SoundFX[37]);
            Object->onAttacked[2].CopyWave(&SoundFX[37]);
            Object->onKilled[0].CopyWave(&SoundFX[35]);
            Object->onKilled[1].CopyWave(&SoundFX[35]);
            Object->Up = 3;
            Object->Down = 1;
            Object->Left = 2;
            Object->Right = 1;
            break;
            
        case __MONSTER_SPIDER:
            Object->onAttack[0].CopyWave(&SoundFX[73]);
            Object->onAttack[1].CopyWave(&SoundFX[73]);
            Object->onAttack[2].CopyWave(&SoundFX[73]);
            Object->onAttacked[0].CopyWave(&SoundFX[74]);
            Object->onAttacked[1].CopyWave(&SoundFX[74]);
            Object->onAttacked[2].CopyWave(&SoundFX[74]);
            Object->onKilled[0].CopyWave(&SoundFX[72]);
            Object->onKilled[1].CopyWave(&SoundFX[72]);
            Object->Up = 4;
            Object->Down = 3;
            Object->Left = 3;
            Object->Right = 2;
            break;
            
        case __MONSTER_SKELETON:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[18]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttacked[0].CopyWave(&SoundFX[39]);
            Object->onAttacked[1].CopyWave(&SoundFX[39]);
            Object->onAttacked[2].CopyWave(&SoundFX[39]);
            Object->onKilled[0].CopyWave(&SoundFX[38]);
            Object->onKilled[1].CopyWave(&SoundFX[38]);
            Object->onParry.CopyWave(&SoundFX[7]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
            
        case __MONSTER_DEMON:
		case __MONSTER_DEMON_BLUE:
		case __MONSTER_DEMON_GREEN:
		case __MONSTER_DEMON_LIGHTBLUE:
		case __MONSTER_DEMON_BLACK:
		case __MONSTER_DEMON_YELLOW:
		case __MONSTER_DEMON_WHITE:
		case __MONSTER_DRAGONDARKGREEN:
      case __MONSTER_DRAGON_RED:  
      case __MONSTER_DRAGON_ORANGE:
      case __MONSTER_DRAGON_JAUNE:
      case __MONSTER_DRAGON_BLEU: 
      case __MONSTER_DRAGON_GRIS: 
      case __MONSTER_DRAGON_BLANC:
      case __MONSTER_DRAGON_NOIR: 
      case __MONSTER_SMALL_YETI:
      case __MONSTER_SDRAGONDARKGREEN:
      case __MONSTER_SDRAGONDARKRED  :
            Object->onAttack[0].CopyWave(&SoundFX[28]);
            Object->onAttack[1].CopyWave(&SoundFX[28]);
            Object->onAttack[2].CopyWave(&SoundFX[28]);
            Object->onAttacked[0].CopyWave(&SoundFX[29]);
            Object->onAttacked[1].CopyWave(&SoundFX[29]);
            Object->onAttacked[2].CopyWave(&SoundFX[29]);
            Object->onKilled[0].CopyWave(&SoundFX[27]);
            Object->onKilled[1].CopyWave(&SoundFX[27]);
            Object->Up = 10;
            Object->Down = 2;
            Object->Left = 4; 
            Object->Right = 3;
            Object->TextCorrection = -95;
            break;
      case __MONSTER_SSDRAGONDARKGREEN:
      case __MONSTER_SSDRAGONDARKRED  :
      case __MONSTER_SSDRAGONDARKORANGE  :
            Object->onAttack[0].CopyWave(&SoundFX[28]);
            Object->onAttack[1].CopyWave(&SoundFX[28]);
            Object->onAttack[2].CopyWave(&SoundFX[28]);
            Object->onAttacked[0].CopyWave(&SoundFX[29]);
            Object->onAttacked[1].CopyWave(&SoundFX[29]);
            Object->onAttacked[2].CopyWave(&SoundFX[29]);
            Object->onKilled[0].CopyWave(&SoundFX[27]);
            Object->onKilled[1].CopyWave(&SoundFX[27]);
            Object->Up = 10;
            Object->Down = 2;
            Object->Left = 4;
            Object->Right = 3;
            Object->TextCorrection = -75;
            break;
      case __MONSTER_YETI:
         Object->onAttack[0].CopyWave(&SoundFX[28]);
            Object->onAttack[1].CopyWave(&SoundFX[28]);
            Object->onAttack[2].CopyWave(&SoundFX[28]);
            Object->onAttacked[0].CopyWave(&SoundFX[29]);
            Object->onAttacked[1].CopyWave(&SoundFX[29]);
            Object->onAttacked[2].CopyWave(&SoundFX[29]);
            Object->onKilled[0].CopyWave(&SoundFX[27]);
            Object->onKilled[1].CopyWave(&SoundFX[27]);
            Object->Up = 10;
            Object->Down = 2;
            Object->Left = 4;
            Object->Right = 3;
            Object->TextCorrection = -150;
      break;

        case __MONSTER_SANTA_CLAUS:
		case __MONSTER_FGAMEMASTER:
            Object->Up = 10;
            Object->Down = 2;
            Object->Left = 4;
            Object->Right = 3;
            Object->TextCorrection = -75;
            break;
                       
        case __MONSTER_MINOTAUR:
            Object->onAttack[0].CopyWave(&SoundFX[108]);
            Object->onAttack[1].CopyWave(&SoundFX[108]);
            Object->onAttack[2].CopyWave(&SoundFX[108]);
            Object->onAttacked[0].CopyWave(&SoundFX[109]);
            Object->onAttacked[1].CopyWave(&SoundFX[109]);
            Object->onAttacked[2].CopyWave(&SoundFX[109]);
            Object->onKilled[0].CopyWave(&SoundFX[107]);
            Object->onKilled[1].CopyWave(&SoundFX[107]);
            Object->onParry.CopyWave(&SoundFX[7]);
            Object->Up = 8;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 2;
            break;
		//Dialsoft new code BEGIN
		case __MONSTER_DARKGOBLIN:
            Object->onAttack[0].CopyWave(&SoundFX[11]);
            Object->onAttack[1].CopyWave(&SoundFX[11]);
            Object->onAttack[2].CopyWave(&SoundFX[18]);
            Object->onAttacked[0].CopyWave(&SoundFX[34]);
            Object->onAttacked[1].CopyWave(&SoundFX[34]);
            Object->onAttacked[2].CopyWave(&SoundFX[34]);
            Object->onKilled[0].CopyWave(&SoundFX[33]);
            Object->onKilled[1].CopyWave(&SoundFX[33]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -70;
            break;
            
		case __MONSTER_BURNTREE:
            Object->onAttack[0].CopyWave(&SoundFX[76]);
            Object->onAttack[1].CopyWave(&SoundFX[76]);
            Object->onAttack[2].CopyWave(&SoundFX[76]);
            Object->onAttacked[0].CopyWave(&SoundFX[90]);
            Object->onAttacked[1].CopyWave(&SoundFX[90]);
            Object->onAttacked[2].CopyWave(&SoundFX[90]);
            Object->onKilled[0].CopyWave(&SoundFX[75]);
            Object->onKilled[1].CopyWave(&SoundFX[75]);
            Object->Up = 13;
            Object->Down = 3;
            Object->Left = 4;
            Object->Right = 4;
            Object->TextCorrection = -100;
            break;
      case __MONSTER_BIGBURNTREE:
            Object->onAttack[0].CopyWave(&SoundFX[76]);
            Object->onAttack[1].CopyWave(&SoundFX[76]);
            Object->onAttack[2].CopyWave(&SoundFX[76]);
            Object->onAttacked[0].CopyWave(&SoundFX[90]);
            Object->onAttacked[1].CopyWave(&SoundFX[90]);
            Object->onAttacked[2].CopyWave(&SoundFX[90]);
            Object->onKilled[0].CopyWave(&SoundFX[75]);
            Object->onKilled[1].CopyWave(&SoundFX[75]);
            Object->Up = 13;
            Object->Down = 3;
            Object->Left = 4;
            Object->Right = 4;
            Object->TextCorrection = -180;
            break;
		
		case __MONSTER_GROUNDMUMMY:
            Object->onAttack[0].CopyWave(&SoundFX[67]);
            Object->onAttack[1].CopyWave(&SoundFX[67]);
            Object->onAttack[2].CopyWave(&SoundFX[67]);
            Object->onAttacked[0].CopyWave(&SoundFX[68]);
            Object->onAttacked[1].CopyWave(&SoundFX[68]);
            Object->onAttacked[2].CopyWave(&SoundFX[68]);
            Object->onKilled[0].CopyWave(&SoundFX[66]);
            Object->onKilled[1].CopyWave(&SoundFX[66]);
            Object->Up = 2;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -55;
            break;
            
		case __MONSTER_BOSSRAT:
            Object->onAttack[0].CopyWave(&SoundFX[36]);
            Object->onAttack[1].CopyWave(&SoundFX[36]);
            Object->onAttack[2].CopyWave(&SoundFX[36]);
            Object->onAttacked[0].CopyWave(&SoundFX[37]);
            Object->onAttacked[1].CopyWave(&SoundFX[37]);
            Object->onAttacked[2].CopyWave(&SoundFX[37]);
            Object->onKilled[0].CopyWave(&SoundFX[35]);
            Object->onKilled[1].CopyWave(&SoundFX[35]);
            Object->Up = 3;
            Object->Down = 1;
            Object->Left = 2;
            Object->Right = 1;
			Object->TextCorrection = -55;
            break;          
		
		case __MONSTER_BIGSKEL:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[18]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttacked[0].CopyWave(&SoundFX[39]);
            Object->onAttacked[1].CopyWave(&SoundFX[39]);
            Object->onAttacked[2].CopyWave(&SoundFX[39]);
            Object->onKilled[0].CopyWave(&SoundFX[38]);
            Object->onKilled[1].CopyWave(&SoundFX[38]);
            Object->onParry.CopyWave(&SoundFX[7]);
            Object->Up = 8;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -90;

            break;	
       case __MONSTER_BIGBIGSKEL:
            Object->onAttack[0].CopyWave(&SoundFX[4]);
            Object->onAttack[1].CopyWave(&SoundFX[18]);
            Object->onAttack[2].CopyWave(&SoundFX[12]);
            Object->onAttacked[0].CopyWave(&SoundFX[39]);
            Object->onAttacked[1].CopyWave(&SoundFX[39]);
            Object->onAttacked[2].CopyWave(&SoundFX[39]);
            Object->onKilled[0].CopyWave(&SoundFX[38]);
            Object->onKilled[1].CopyWave(&SoundFX[38]);
            Object->onParry.CopyWave(&SoundFX[7]);
            Object->Up = 8;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -180;

            break;	

		case __MONSTER_DEMON_VIOLET:
			Object->onAttack[0].CopyWave(&SoundFX[28]);
            Object->onAttack[1].CopyWave(&SoundFX[28]);
            Object->onAttack[2].CopyWave(&SoundFX[28]);
            Object->onAttacked[0].CopyWave(&SoundFX[29]);
            Object->onAttacked[1].CopyWave(&SoundFX[29]);
            Object->onAttacked[2].CopyWave(&SoundFX[29]);
            Object->onKilled[0].CopyWave(&SoundFX[27]);
            Object->onKilled[1].CopyWave(&SoundFX[27]);
            Object->Up = 10;
            Object->Down = 2;
            Object->Left = 4;
            Object->Right = 3;
            Object->TextCorrection = -95;
            break;
      case __MONSTER_DARKGOBLIN_ICE:
            Object->onAttack[0].CopyWave(&SoundFX[11]);
            Object->onAttack[1].CopyWave(&SoundFX[11]);
            Object->onAttack[2].CopyWave(&SoundFX[18]);
            Object->onAttacked[0].CopyWave(&SoundFX[34]);
            Object->onAttacked[1].CopyWave(&SoundFX[34]);
            Object->onAttacked[2].CopyWave(&SoundFX[34]);
            Object->onKilled[0].CopyWave(&SoundFX[33]);
            Object->onKilled[1].CopyWave(&SoundFX[33]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 1;
            Object->TextCorrection = -70;
            break;
      case __MONSTER_KRAANIAN_ICE:
            Object->onAttack[0].CopyWave(&SoundFX[49]);
            Object->onAttack[1].CopyWave(&SoundFX[49]);
            Object->onAttack[2].CopyWave(&SoundFX[49]);
            Object->onAttacked[0].CopyWave(&SoundFX[50]);
            Object->onAttacked[1].CopyWave(&SoundFX[50]);
            Object->onAttacked[2].CopyWave(&SoundFX[50]);
            Object->onKilled[0].CopyWave(&SoundFX[48]);
            Object->onKilled[1].CopyWave(&SoundFX[48]);
            Object->Up = 6;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -50;
            break;
      case __MONSTER_KRAANIANFLYING_ICE:
            Object->onAttack[0].CopyWave(&SoundFX[56]);
            Object->onAttack[1].CopyWave(&SoundFX[56]);
            Object->onAttack[2].CopyWave(&SoundFX[56]);
            Object->onAttacked[0].CopyWave(&SoundFX[57]);
            Object->onAttacked[1].CopyWave(&SoundFX[57]);
            Object->onAttacked[2].CopyWave(&SoundFX[57]);
            Object->onKilled[0].CopyWave(&SoundFX[55]);
            Object->onKilled[1].CopyWave(&SoundFX[55]);
            Object->Up = 8;
            Object->Down = 1;
            Object->Left = 3;
            Object->Right = 2;
            Object->TextCorrection = -75;
            break;
       case __MONSTER_KRAANIANTANK_ICE:
            Object->onAttack[0].CopyWave(&SoundFX[49]);
            Object->onAttack[1].CopyWave(&SoundFX[49]);
            Object->onAttack[2].CopyWave(&SoundFX[49]);
            Object->onAttacked[0].CopyWave(&SoundFX[50]);
            Object->onAttacked[1].CopyWave(&SoundFX[50]);
            Object->onAttacked[2].CopyWave(&SoundFX[50]);
            Object->onKilled[0].CopyWave(&SoundFX[48]);
            Object->onKilled[1].CopyWave(&SoundFX[48]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
      case __MONSTER_ORC_ICE:
            Object->onAttack[0].CopyWave(&SoundFX[103]);
            Object->onAttack[1].CopyWave(&SoundFX[103]);
            Object->onAttack[2].CopyWave(&SoundFX[103]);
            Object->onAttacked[0].CopyWave(&SoundFX[106]);
            Object->onAttacked[1].CopyWave(&SoundFX[106]);
            Object->onAttacked[2].CopyWave(&SoundFX[106]);
            Object->onKilled[0].CopyWave(&SoundFX[105]);
            Object->onKilled[1].CopyWave(&SoundFX[105]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 3;
            Object->Right = 2;
            Object->TextCorrection = -80;
            break;
      case __MONSTER_KRAANIAN_POD_ICE:
            Object->onAttack[0].CopyWave(&SoundFX[49]);
            Object->onAttack[1].CopyWave(&SoundFX[49]);
            Object->onAttack[2].CopyWave(&SoundFX[49]);
            Object->onAttacked[0].CopyWave(&SoundFX[50]);
            Object->onAttacked[1].CopyWave(&SoundFX[50]);
            Object->onAttacked[2].CopyWave(&SoundFX[50]);
            Object->onKilled[0].CopyWave(&SoundFX[48]);
            Object->onKilled[1].CopyWave(&SoundFX[48]);
            Object->Up = 7;
            Object->Down = 2;
            Object->Left = 2;
            Object->Right = 1;
            Object->TextCorrection = -75;
            break;
       case __MONSTER_OGRE:
       case __MONSTER_GOLEM1:
       case __MONSTER_GOLEM2:
       case __MONSTER_GOLEM3:
      
            Object->onAttack[0].CopyWave(&SoundFX[103]);
            Object->onAttack[1].CopyWave(&SoundFX[103]);
            Object->onAttack[2].CopyWave(&SoundFX[103]);
            Object->onAttacked[0].CopyWave(&SoundFX[106]);
            Object->onAttacked[1].CopyWave(&SoundFX[106]);
            Object->onAttacked[2].CopyWave(&SoundFX[106]);
            Object->onKilled[0].CopyWave(&SoundFX[105]);
            Object->onKilled[1].CopyWave(&SoundFX[105]);
            Object->Up = 7;
            Object->Down = 1;
            Object->Left = 1;
            Object->Right = 1;
            Object->TextCorrection = -96;
            break;
		//Dialsoft new code END            
      };
   }
   Object->MaxWidth = VObject3D[Object->Type3D]->MaxWidth+32;
   Object->MaxHeight = VObject3D[Object->Type3D]->MaxHeight+16;
   Object->MaxxCor = VObject3D[Object->Type3D]->MaxxCor;
   Object->MaxyCor = VObject3D[Object->Type3D]->MaxyCor;
   Object->fTransparent = VObject3D[Object->Type3D]->fTransparent;
   Object->TransColor = VObject3D[Object->Type3D]->TransColor;
   Object->Overlap.Release();
   //:CR_NMNMNM  Overlap Size
   Object->Overlap.Create(Object->MaxWidth, Object->MaxHeight, TRUE, lpDXDirectDraw->wGMask, Object->MaxxCor, Object->MaxyCor);
   Object->Overlap.Dither = g_SaveGame.GetOptionsParam()->bDitherGraph;
   Object->Movement = VObject3D[Object->Type3D]->Type;
   Object->Faces = VObject3D[Object->Type3D]->Faces;
   a = b = 0;
   if (World.Done) {
       if (World.MovY == -1)
           b = 16;
       if (World.MovY == 1)
           b = -16;
       if (World.MovX == -1)
           a = 32;
       if (World.MovX == 1)
           a = -32;
   }
   INFO *Return = VObject3D[Object->Type3D]->GetCurrentVSF(Object->Direction, 0, Object->Standing);
   /**/World.DrawOverlap(Object->OX+ViewWidth, Object->OY+ViewHeight, 0, 0, (Object->OX+ViewWidth)*32+Object->Overlap.xCor-48+a, (Object->OY+ViewHeight)*16+Object->Overlap.yCor+13+b, &Object->Overlap, 18, Return,Object->ID);
   Object->DrawOverlap = Return->DrawOverlap;
   delete Return;
   Object->AttFaces = VObject3D[Object->Type3D]->FacesA;
   Object->FacesStA = VObject3D[Object->Type3D]->FacesStA;
   Object->CorpseFaces = VObject3D[Object->Type3D]->FacesC;
   Object->FacesStM = VObject3D[Object->Type3D]->FacesStM;
   if (Object->Friendly == VOL_PLAYER) {
       Object->MouseCursor = 0;
   } else if (Object->Friendly == VOL_NPC) {
       Object->MouseCursor = 3;
   } else if (Object->Friendly == VOL_MONSTER) {
       Object->MouseCursor = 2;
   }
}

void VisualObjectList::ReleaseAll(void) {
    for (int i = 0; i < NBBASICSPRITE; i++) 
    {
        if (Object3DLoad[i] && i != Set) 
        {
            Object3DLoad[i] = false;
            VObject3D[i]->Release();
            delete VObject3D[i];
            VObject3D[i] = NULL;
            for (int j = 0; j < 11; j++) 
            {
                if (Object3DSound[i][j]) 
                {
                    SoundFX[Object3DSound[i][j]].Release(TRUE);
                } 
                else 
                {
                    break;
                }
            }
        }
    }
    ConnectSoundFX.Release(TRUE);
}

void VisualObjectList::SDClear(void) {
    SDObjNb = 0;
    SDID = 0;
}

void VisualObjectList::SDAddObject(short x, short y, short w, char l, short t) {
    SDID--; 
    SDObjID[SDObjNb]    = SDID; 
    SDObjX[SDObjNb]     = x; 
    SDObjY[SDObjNb]     = y; 
    SDObjW[SDObjNb]     = w; 
    SDObjLight[SDObjNb] = l; 
    SDObjType[SDObjNb]  = t; 
    SDObjNb++;
}

void VisualObjectList::SDAnimClear(void) 
{
    m_animObjNbr = 0;
    m_animObjID = 0;
}

void VisualObjectList::SDAnimAddObject(short x, short y, short w, char l, short t) 
{
    //Find if exist if not exist add new Item...
    for(int i=0;i<m_animObjNbr;i++)
    {
       if(m_sAnimObj[i].SDObjX == x && m_sAnimObj[i].SDObjY == y &&
          m_sAnimObj[i].SDObjW == w && m_sAnimObj[i].SDObjType == t)
       {
          // already in list on return tout simplement
          return;
       }
    }
    if(m_animObjNbr > 999)
       return;
    m_animObjID--; 
    m_sAnimObj[m_animObjNbr].SDObjID    = m_animObjID; 
    m_sAnimObj[m_animObjNbr].SDObjX     = x; 
    m_sAnimObj[m_animObjNbr].SDObjY     = y; 
    m_sAnimObj[m_animObjNbr].SDObjW     = w; 
    m_sAnimObj[m_animObjNbr].SDObjLight = l; 
    m_sAnimObj[m_animObjNbr].SDObjType  = t; 
    m_animObjNbr++;
}

void VisualObjectList::AddOverlapAnim(UINT id,UINT dIDS, short x,short y,RECT rcClip)
{
    for(int i=0;i<m_sAnimOverlap.size();i++)
    {
       if(m_sAnimOverlap[i].uiID == id)
       {

          m_sAnimOverlap[i].ObjX       = x; 
          m_sAnimOverlap[i].ObjY       = y; 
          m_sAnimOverlap[i].rcClip     = rcClip; 
          return;
       }
       
    }
    sAnimOverlap sAnimOverlap;
    sAnimOverlap.uiID       = id; 
    sAnimOverlap.uiIDSprite = dIDS; 
    sAnimOverlap.ObjX       = x; 
    sAnimOverlap.ObjY       = y; 
    sAnimOverlap.rcClip     = rcClip; 

    m_sAnimOverlap.push_back(sAnimOverlap);

    //char str[300];
    //sprintf(str,"--ADD ID[%d] TYPE [%d]\n",id,dIDS);
    //OutputDebugString(str);
}

void VisualObjectList::ClearAnimID(UINT id)
{
   vector< sAnimOverlap >::iterator i;
   for( i = m_sAnimOverlap.begin(); i != m_sAnimOverlap.end(); i++ )
   {					
      if(i->uiID == id)
      {
         m_sAnimOverlap.erase(i);
         return;
      }
   }
}

int  VisualObjectList::FoundAnimListID(UINT dwID)
{
   if(!g_SaveGame.GetOptionsParam()->bShowAnimDecorsLight)
      return -1;
   int dwIndex = 0;
   vector< sAnimOverlap >::iterator i;
   for( i = m_sAnimOverlap.begin(); i != m_sAnimOverlap.end(); i++,dwIndex++ )
   {					
      if(i->uiID == dwID)
      {
         return dwIndex;
      }
   }
   return -1;
}


void VisualObjectList::KillObject(DWORD dwCall) 
{
    try 
    {
        TFCObject *Object = GetObject();
        RemoveObject();
        LOG << "Deleting Object ID: " << Object->ID << " in call " << dwCall << ". Info OXY(" << Object->OX << ", " << Object->OY << ") Del: " << Object->DeleteMe << "\r\n";
        delete Object;
        nbUnit--;
    } 
    catch (...) 
    {
        char Temp[1000];
        
        vir::FileLogger f;
        f.SetLogName("Error.Log");
        f.SetLogLevel(vir::LL_ALL);
        sprintf(Temp, "KillObject");
        f.Log(vir::LL_ERROR, Temp);
        throw;
    }
    LOG << "END KILL OBJECT\r\n";
}

void VisualObjectList::DrawPuppet(int xPos, int yPos,int dwDirection) 
{
    Lock(1014);
    VObject3D[Set]->DrawSprite3D(dwDirection, 0, xPos, yPos, ST_STANDING, NULL, false, 0, NULL, 0, &MainObject);
    Unlock(1014);
}

TFCObject *VisualObjectList::GetMainObject()
{
    return &MainObject;
}
