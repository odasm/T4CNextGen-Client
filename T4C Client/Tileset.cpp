// **************************************************************************
// ***                                                                    ***
//      File Name: TileSet.Cpp
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
//         05-30-1997      1.0     TH          Initial development.
//
//		  Fonctions:
//			  Provide TileSet Basic.
//
// ***                                                                    ***
// **************************************************************************
// *** Copyright (c) 1996-2003 Vircom inc. All rights reserved.           ***

#pragma warning(disable : 4786)
#pragma warning( disable : 4291 )

#include "pch.h" 
#include <windows.h>           

#include <stdio.h>
#include "TileSet.h"
 
#include "IDirectXImage.h"

#include "TFCLandDef.h"
#include "TFCPlayer.h"
#include "VisualObjectList.h"

#include "IDirectXFont.h"
#include "Random.h"
#include "PowerDebug.h"

#include "Objectlisting.h"
#include "Apparence.h"

#include "Disp.h"
#include "NTime.h"

#include "Icon3D.h"

#include "Math.h"
#include "MacroHandler.h"

#include "TFCPacket.h"
#include "COMM.h"

#include "LocalString.h"
#include "Fading.h"

#include "Global.h"
#include "FileLogger.h"

#include "App.h"
#include "MemoryManager.h"

#include "ProcStat.h"

#include "LightMap.h"
#include "FormatText.h"
#include "CombatCursor.h"
#include <stdlib.h>
#include "Weather.h"
#include "NewInterface/RootBoxUI.h"
#include "NewInterface/ChatterUI.h"


#include "SaveGame.h"
extern CSaveGame g_SaveGame;
extern bool LevelUp;

extern DWORD SpellID;
extern CProcStat g_TimeStat;
DWORD g_QueueSize;


/// TEMP
///FormatText g_ftStory;
extern Font *fMenuDescFont;
extern bool g_DisplayLetter;
/// END TEMP

#define LoadInit()\
   int Load3i, Load3j; \
   char Load3Sprite[100]; \
   char Load3Temp[10]; 

#define LoadIsoSquare(Var, Name, x, y)\
   for (Load3j = 0; Load3j < y; Load3j++) {\
      for (Load3i = 0; Load3i < x; Load3i++) {\
         strcpy(Load3Sprite, Name);\
         strcat(Load3Sprite, " (");\
         strcat(Load3Sprite, itoa(Load3i+1, Load3Temp, 10));\
         strcat(Load3Sprite, ", ");\
         strcat(Load3Sprite, itoa(Load3j+1, Load3Temp, 10));\
         strcat(Load3Sprite, ")");\
         Var[Load3j][Load3i].LoadSprite(Load3Sprite);\
      }\
   }


void WaterSmooth(CV2Sprite &A, CV2Sprite &B, CV2Sprite &C, int xScreen, int yScreen, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE lpSurface) { 
   LPWORD s1 = A.GetSurfaceMemory(); 
   LPWORD d = C.GetSurfaceMemory(); 
   Sprite Temp; 
   Temp.Create(32, 16, FALSE, 0, 0, 0, 0, 0); 
   LPWORD t = Temp.GetSurfaceMemory(); 
   for (int j = 0; j < 16; j++) { 
      for (int i = 0; i < 32; i++) { 
         if (d[j*32+i] != 0) { 
            t[j*32+i] = s1[j*32+i]; 
         } 
      }
   } 
   A.ReleaseSurfaceMemory(s1); 
   C.ReleaseSurfaceMemory(d); 
   Temp.ReleaseSurfaceMemory(t); 
   Temp.DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 0, 0, TRUE); 
}

void Smootage(CV2Sprite &A, CV2Sprite &B, CV2Sprite &C, int xScreen, int yScreen, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE lpSurface) {
   LPWORD s1 = A.GetSurfaceMemory(); 
   LPWORD s2 = B.GetSurfaceMemory(); 
   LPWORD d = C.GetSurfaceMemory(); 
   Sprite Temp; 
   Temp.Create(32, 16, FALSE, 0, 0, 0, 0, 0); 
   LPWORD t = Temp.GetSurfaceMemory(); 
   WORD wMask = lpDXDirectDraw->wRMask + lpDXDirectDraw->wGMask + lpDXDirectDraw->wBMask;
   for (int j = 0; j < 16; j++) { 
      for (int i = 0; i < 32; i++) {
         if (d[j*32+i] != wMask) { 
            t[j*32+i] = s1[j*32+i]; 
         } else { 
            t[j*32+i] = s2[j*32+i]; 
         } 
      }
   } 
   B.ReleaseSurfaceMemory(s2); 
   A.ReleaseSurfaceMemory(s1); 
   C.ReleaseSurfaceMemory(d); 
   Temp.ReleaseSurfaceMemory(t); 
   Temp.DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
}

extern bool FadeOut;
extern DWORD GetThreadTime(HANDLE h);
extern HANDLE hDrawThread;
extern HANDLE hMaintenanceThread;

void Smootage2(CV2Sprite &A, CV2Sprite &B, CV2Sprite &C, int xScreen, int yScreen, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE lpSurface) { 
   LPWORD s1 = A.GetSurfaceMemory(); 
   LPWORD s2 = B.GetSurfaceMemory(); 
   LPWORD d = C.GetSurfaceMemory(); 
   Sprite Temp; 
   Temp.Create(32, 16, FALSE, 0, 0, 0, 0, 0); 
   LPWORD t = Temp.GetSurfaceMemory(); 
   for (int j = 0; j < 16; j++) { 
      for (int i = 0; i < 32; i++) { 
         if (d[j*32+i] == 0x7FFF) { 
            t[j*32+i] = s2[j*32+i];
         } else { 
            t[j*32+i] = d[j*32+i];
         } 
      }
   } 
   B.ReleaseSurfaceMemory(s2); 
   A.ReleaseSurfaceMemory(s1); 
   C.ReleaseSurfaceMemory(d); 
   Temp.ReleaseSurfaceMemory(t); 
   Temp.DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
}

void Smootage3(CV2Sprite &A, CV2Sprite &B, CV2Sprite &C, int xScreen, int yScreen, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE lpSurface) {
   LPWORD s1 = A.GetSurfaceMemory(); 
   LPWORD s2 = B.GetSurfaceMemory(); 
   LPWORD d = C.GetSurfaceMemory(); 
   Sprite Temp; 
   Temp.Create(32, 16, FALSE, 0, 0, 0, 0, 0); 
   LPWORD t = Temp.GetSurfaceMemory(); 
   WORD wMask = lpDXDirectDraw->wRMask + lpDXDirectDraw->wGMask + lpDXDirectDraw->wBMask;
   for (int j = 0; j < 16; j++) { 
      for (int i = 0; i < 32; i++) {
         if (d[j*32+i] != wMask) { 
            t[j*32+i] = s1[j*32+i]; 
         } else { 
            t[j*32+i] = s2[j*32+i]; 
         } 
      }
   } 
   B.ReleaseSurfaceMemory(s2); 
   A.ReleaseSurfaceMemory(s1); 
   C.ReleaseSurfaceMemory(d); 
   Temp.ReleaseSurfaceMemory(t); 
   Temp.DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
}

#define AllSmoothLoad(A, B) \
	flpLoadSprite[N_##A] = B; \
	flpLoadSprite[S_##A] = B; \
	flpLoadSprite[E_##A] = B; \
	flpLoadSprite[W_##A] = B; \
	flpLoadSprite[NE_##A] = B; \
	flpLoadSprite[NW_##A] = B; \
	flpLoadSprite[SE_##A] = B; \
	flpLoadSprite[SW_##A] = B; \
	flpLoadSprite[XNE_##A] = B; \
	flpLoadSprite[XNW_##A] = B; \
	flpLoadSprite[XSE_##A] = B; \
	flpLoadSprite[XSW_##A] = B; \
	flpLoadSprite[X2NE_##A] = B; \
	flpLoadSprite[X2NW_##A] = B; \
	flpLoadSprite[X2SE_##A] = B; \
	flpLoadSprite[X2SW_##A] = B; \
	flpLoadSprite[X3NE_##A] = B; \
	flpLoadSprite[X3NW_##A] = B; \
	flpLoadSprite[X3SE_##A] = B; \
	flpLoadSprite[X3SW_##A] = B; \
	flpLoadSprite[X4NE_##A] = B; \
	flpLoadSprite[X4NW_##A] = B; \
	flpLoadSprite[X4SE_##A] = B; \
	flpLoadSprite[X4SW_##A] = B; \
	flpLoadSprite[X5NE_##A] = B; \
	flpLoadSprite[X5NW_##A] = B; \
	flpLoadSprite[X5SE_##A] = B; \
	flpLoadSprite[X5SW_##A] = B; \
	flpLoadSprite[DNE_##A] = B; \
	flpLoadSprite[DNW_##A] = B; \
	flpLoadSprite[DSE_##A] = B; \
	flpLoadSprite[DSW_##A] = B; 

#define AllSmoothLoad3(A, B) \
	flpLoadSprite[NE_##A] = B; \
	flpLoadSprite[NW_##A] = B; \
	flpLoadSprite[SE_##A] = B; \
	flpLoadSprite[SW_##A] = B;

#define AllSmooth(A, B, C) {  \
   case N_##A:    Smootage(B, C, Smoothing[1], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case S_##A:	 Smootage(B, C, Smoothing[0], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case E_##A:	 Smootage(B, C, Smoothing[3], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case W_##A:	 Smootage(B, C, Smoothing[2], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case NE_##A:   Smootage(B, C, Smoothing[6], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
  	case NW_##A:   Smootage(B, C, Smoothing[7], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case SE_##A:   Smootage(B, C, Smoothing[4], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case SW_##A:   Smootage(B, C, Smoothing[5], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case XNE_##A:  Smootage(B, C, Smoothing[10], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case XNW_##A:  Smootage(B, C, Smoothing[11], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case XSE_##A:  Smootage(B, C, Smoothing[8], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;  \
   case XSW_##A:  Smootage(B, C, Smoothing[9], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;  \
   case X2NE_##A: Smootage(B, C, Smoothing[14], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X2NW_##A: Smootage(B, C, Smoothing[15], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X2SE_##A: Smootage(B, C, Smoothing[12], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X2SW_##A: Smootage(B, C, Smoothing[13], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X3NE_##A: Smootage(B, C, Smoothing[18], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X3NW_##A: Smootage(B, C, Smoothing[19], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X3SE_##A: Smootage(B, C, Smoothing[16], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X3SW_##A: Smootage(B, C, Smoothing[17], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X4NE_##A: Smootage(B, C, Smoothing[22], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X4NW_##A: Smootage(B, C, Smoothing[23], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X4SE_##A: Smootage(B, C, Smoothing[20], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X4SW_##A: Smootage(B, C, Smoothing[21], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case DNE_##A:  Smootage(B, C, Smoothing[27], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case DNW_##A:  Smootage(B, C, Smoothing[26], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case DSE_##A:  Smootage(B, C, Smoothing[24], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case DSW_##A:  Smootage(B, C, Smoothing[25], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X5NE_##A: Smootage(B, C, Smoothing[28], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X5NW_##A: Smootage(B, C, Smoothing[31], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X5SE_##A: Smootage(B, C, Smoothing[29], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X5SW_##A: Smootage(B, C, Smoothing[30], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; }

#define WatSmooth(A, B, C) {  \
   case N_##A:    WaterSmooth(B, C, Smoothing[1], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case S_##A:	 WaterSmooth(B, C, Smoothing[0], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case E_##A:	 WaterSmooth(B, C, Smoothing[3], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case W_##A:	 WaterSmooth(B, C, Smoothing[2], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case NE_##A:   WaterSmooth(B, C, Smoothing[6], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
  	case NW_##A:   WaterSmooth(B, C, Smoothing[7], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case SE_##A:   WaterSmooth(B, C, Smoothing[4], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case SW_##A:   WaterSmooth(B, C, Smoothing[5], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case XNE_##A:  WaterSmooth(B, C, Smoothing[10], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case XNW_##A:  WaterSmooth(B, C, Smoothing[11], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case XSE_##A:  WaterSmooth(B, C, Smoothing[8], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;  \
   case XSW_##A:  WaterSmooth(B, C, Smoothing[9], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;  \
   case X2NE_##A: WaterSmooth(B, C, Smoothing[14], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X2NW_##A: WaterSmooth(B, C, Smoothing[15], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X2SE_##A: WaterSmooth(B, C, Smoothing[12], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X2SW_##A: WaterSmooth(B, C, Smoothing[13], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X3NE_##A: WaterSmooth(B, C, Smoothing[18], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X3NW_##A: WaterSmooth(B, C, Smoothing[19], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X3SE_##A: WaterSmooth(B, C, Smoothing[16], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X3SW_##A: WaterSmooth(B, C, Smoothing[17], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X4NE_##A: WaterSmooth(B, C, Smoothing[22], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X4NW_##A: WaterSmooth(B, C, Smoothing[23], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X4SE_##A: WaterSmooth(B, C, Smoothing[20], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X4SW_##A: WaterSmooth(B, C, Smoothing[21], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case DNE_##A:  WaterSmooth(B, C, Smoothing[27], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case DNW_##A:  WaterSmooth(B, C, Smoothing[26], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case DSE_##A:  WaterSmooth(B, C, Smoothing[24], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case DSW_##A:  WaterSmooth(B, C, Smoothing[25], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X5NE_##A: WaterSmooth(B, C, Smoothing[28], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X5NW_##A: WaterSmooth(B, C, Smoothing[31], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X5SE_##A: WaterSmooth(B, C, Smoothing[29], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X5SW_##A: WaterSmooth(B, C, Smoothing[30], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; }


#define AllSmooth2(A, B, C) {  \
   case N_##A:    Smootage(B, C, Smoothing2[1], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case S_##A:	 Smootage(B, C, Smoothing2[0], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case E_##A:	 Smootage(B, C, Smoothing2[3], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case W_##A:	 Smootage(B, C, Smoothing2[2], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case NE_##A:   Smootage(B, C, Smoothing2[6], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case NW_##A:   Smootage(B, C, Smoothing2[7], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case SE_##A:   Smootage(B, C, Smoothing2[4], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case SW_##A:   Smootage(B, C, Smoothing2[5], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case XNE_##A:  Smootage(B, C, Smoothing2[10], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case XNW_##A:  Smootage(B, C, Smoothing2[11], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case XSE_##A:  Smootage(B, C, Smoothing2[8], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case XSW_##A:  Smootage(B, C, Smoothing2[9], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X2NE_##A: Smootage(B, C, Smoothing2[14], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X2NW_##A: Smootage(B, C, Smoothing2[15], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X2SE_##A: Smootage(B, C, Smoothing2[12], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X2SW_##A: Smootage(B, C, Smoothing2[13], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X3NE_##A: Smootage(B, C, Smoothing2[18], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X3NW_##A: Smootage(B, C, Smoothing2[19], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X3SE_##A: Smootage(B, C, Smoothing2[16], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X3SW_##A: Smootage(B, C, Smoothing2[17], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X4NE_##A: Smootage(B, C, Smoothing2[22], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X4NW_##A: Smootage(B, C, Smoothing2[23], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X4SE_##A: Smootage(B, C, Smoothing2[20], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X4SW_##A: Smootage(B, C, Smoothing2[21], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case DNE_##A:  Smootage(B, C, Smoothing2[27], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case DNW_##A:  Smootage(B, C, Smoothing2[26], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case DSE_##A:  Smootage(B, C, Smoothing2[24], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case DSW_##A:  Smootage(B, C, Smoothing2[25], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X5NE_##A: Smootage(B, C, Smoothing2[28], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X5NW_##A: Smootage(B, C, Smoothing2[31], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X5SE_##A: Smootage(B, C, Smoothing2[29], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X5SW_##A: Smootage(B, C, Smoothing2[30], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; }

#define AllWatSmooth(A, B, C) { \
   case N_##A:    Smootage2(B, C, WatSmooth[1], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case S_##A:	 Smootage2(B, C, WatSmooth[0], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case E_##A:	 Smootage2(B, C, WatSmooth[3], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case W_##A:	 Smootage2(B, C, WatSmooth[2], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case NE_##A:   Smootage2(B, C, WatSmooth[6], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
  	case NW_##A:   Smootage2(B, C, WatSmooth[7], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
  	case SE_##A:   Smootage2(B, C, WatSmooth[4], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
  	case SW_##A:   Smootage2(B, C, WatSmooth[5], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
  	case XNE_##A:  Smootage2(B, C, WatSmooth[10], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case XNW_##A:  Smootage2(B, C, WatSmooth[11], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case XSE_##A:  Smootage2(B, C, WatSmooth[8], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case XSW_##A:  Smootage2(B, C, WatSmooth[9], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case X2NE_##A: Smootage2(B, C, WatSmooth[14], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X2NW_##A: Smootage2(B, C, WatSmooth[15], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X2SE_##A: Smootage2(B, C, WatSmooth[12], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X2SW_##A: Smootage2(B, C, WatSmooth[13], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X3NE_##A: Smootage2(B, C, WatSmooth[18], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X3NW_##A: Smootage2(B, C, WatSmooth[19], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X3SE_##A: Smootage2(B, C, WatSmooth[16], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X3SW_##A: Smootage2(B, C, WatSmooth[17], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X4NE_##A: Smootage2(B, C, WatSmooth[22], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X4NW_##A: Smootage2(B, C, WatSmooth[23], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X4SE_##A: Smootage2(B, C, WatSmooth[20], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X4SW_##A: Smootage2(B, C, WatSmooth[21], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case DNE_##A:  Smootage2(B, C, WatSmooth[27], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case DNW_##A:  Smootage2(B, C, WatSmooth[26], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case DSE_##A:  Smootage2(B, C, WatSmooth[24], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case DSW_##A:  Smootage2(B, C, WatSmooth[25], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X5NE_##A: Smootage2(B, C, WatSmooth[28], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X5NW_##A: Smootage2(B, C, WatSmooth[31], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X5SE_##A: Smootage2(B, C, WatSmooth[29], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
   case X5SW_##A: Smootage2(B, C, WatSmooth[30], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; }

#define AllSmooth3(A, B, C) {  \
   case NE_##A:   Smootage3(B, C, Smoothing3[0], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;\
  	case NW_##A:   Smootage3(B, C, Smoothing3[1], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case SE_##A:   Smootage3(B, C, Smoothing3[2], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; \
   case SW_##A:   Smootage3(B, C, Smoothing3[3], xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break; }

extern Font *fMenuFont;

extern NTime g_TimeStructure;

inline int Abs(int x) {
	return ((x < 0) ? (-x) : (x));
}

extern DWORD ThreadATimeStamp;
extern DWORD ThreadBTimeStamp;
extern DWORD ThreadCTimeStamp;
extern DWORD ThreadDTimeStamp;
extern DWORD ThreadETimeStamp;
extern DWORD ThreadFTimeStamp;
extern DWORD ThreadGTimeStamp;
extern DWORD ThreadHTimeStamp;
extern DWORD ThreadITimeStamp;
extern DWORD ThreadJTimeStamp;

extern BACK Backpack[24];

// GUID
TileSet World;
extern TFCPlayer Player;

extern Font *fSkillFont;

//extern IDirectXFont DigitFont;
//extern IDirectXFont EquipFont;

const int Square = 36;//25;

extern CombatCursorVSF AttackCursorIcon;

//#undef POWERTRACE
//#define POWERTRACE POWERLOG

template <class type>
DWORD RLE_Decompress(type *lpCompress, type *lpDecompress, DWORD dwLen, DWORD dwMax, DWORD dwLimit) {
	DWORD nCountD = 0;
   long nCountC = 0;
   type dwData = 0;
   long nBytes;
   
   while (nCountD < dwLen) {
      dwData = lpCompress[nCountC];
      nCountC++;
      if (dwData > dwLimit) {
         nBytes = dwData - dwLimit;
         dwData = lpCompress[nCountC];
         nCountC++;
         for (int i = nBytes; i > 0; i--) {
            lpDecompress[nCountD] = dwData;
            nCountD++;
         }
      } else {
         lpDecompress[nCountD] = dwData;
         nCountD++;
      }
   }

   return nCountD;
}

template <class type>
DWORD RLE_Compress(type *lpCompress, type*lpDecompress, DWORD dwLen, DWORD dwMax, DWORD dwLimit) {
   long nCountD = 0;
   long nCountC = 0;
   type dwData = 0;
   long nBytes = 0;

   while (nCountD < dwLen) {
      dwData = lpDecompress[nCountD];
      nCountD++;
      if (dwData == lpDecompress[nCountD] && (nBytes < dwMax-dwLimit) && nCountD < dwLen) {
         nBytes++;
      } else {
         if (nBytes || dwData >= dwLimit) {
            nBytes++;
            lpCompress[nCountC] = nBytes+dwLimit;
            nCountC++;
            lpCompress[nCountC] = dwData;
            nCountC++;
            nBytes = 0;
         } else {
            lpCompress[nCountC] = dwData;
            nCountC++;
         }
      }
   }

   return nCountC;
}

TileSetException::TileSetException(TileSetError Error) {
// Error in TileSet Class
	switch (Error) {
		case NOT_DEFINED_VIEW:
//			TRACE("TileSetException: View and/or VirtualView Width and/or isn't defined. No View Created.\r\n");
			throw;
		break;

		case FILE_NOT_FOUND:
//			TRACE("TileSetException: Virtual Map Files Not Found.\r\n");
			throw;
		break;                                                                                  
	} 
}

LPDIRECTDRAWSURFACE TileSet::lpFractalSurface = NULL;

TileSet::TileSet(void) {
// TileSet Default Constructor.
//	CompiledView = new unsigned short;
	VirtualView  = new unsigned short;
	WorldFileName = new char *[1];

	iNbGSPtr = 0;
   iCustomCorner    = 0;
   iCustomWall      = 0;
   iCustomDarkWall  = 0;
   iCustomExcp1     = 0;
   iCustomDarkExcp1 = 0;
   iCustomExcp2     = 0;
   iCustomDarkExcp2 = 0;
   iCustomExcp3     = 0;
   iCustomDarkExcp3 = 0;
   iCustomExcp4     = 0;
   iCustomDarkExcp4 = 0;
   iCustomExcp5     = 0;
   iCustomDarkExcp5 = 0;
   iCustomBorder    = 0;
   
   TileWidth     = 0;
	TileHeight    = 0;
	ViewWidth     = 0;
	ViewHeight    = 0;
	VirtualWidth  = 0;
	VirtualHeight = 0;
	xPos          = 0;
	yPos          = 0;
   SepX = SepX2  = 0;
   SepY = SepY2  = 0;
	Rnd.CreateRandom(1, 12, 65498478);
   StaticState = false;
   FadeLevel = 0;
	RealFading = false;
	SetFading = false;

   csLock.SetCriticalSection("TileSet", 100);

   m_dwFogX[0 ] = -1024;
   m_dwFogX[1 ] = -512;
   m_dwFogX[2 ] = 0;
   m_dwFogX[3 ] = 512;
   m_dwFogX[4 ] = 1024;
   m_dwFogX[5 ] = -512;
   m_dwFogX[6 ] = 0;
   m_dwFogX[7 ] = 512;
   m_dwFogX[8 ] = 1024;
   m_dwFogX[9 ] = 1536;
   m_dwFogX[10] = 0;
   m_dwFogX[11] = 512;
   m_dwFogX[12] = 1024;
   m_dwFogX[13] = 1536;
   m_dwFogX[14] = 2048;

   m_dwFogY[0 ] = 0;
   m_dwFogY[1 ] = 0;
   m_dwFogY[2 ] = 0;
   m_dwFogY[3 ] = 0;
   m_dwFogY[4 ] = 0;
   m_dwFogY[5 ] = 384;
   m_dwFogY[6 ] = 384;
   m_dwFogY[7 ] = 384;
   m_dwFogY[8 ] = 384;
   m_dwFogY[9 ] = 384;
   m_dwFogY[10] = 768;
   m_dwFogY[11] = 768;
   m_dwFogY[12] = 768;
   m_dwFogY[13] = 768;
   m_dwFogY[14] = 768;

   for(int i=0;i<15;i++)
      m_fFoyY[i] = (float)m_dwFogY[i];

   m_wFogStatus     = 0;
   m_wFogAlphaVal   = 0;
   m_dwFogCntGlobal = 0;
   
   m_wRainStatus     = 0;
   m_wRainAlphaVal   = 0;
   m_dwRainCntGlobal = 0;

   m_wSnowStatus     = 0;
   m_wSnowAlphaVal   = 0;
   m_dwSnowCntGlobal = 0;

/*   unsigned short *Comp = new unsigned short [100];
   unsigned short *Decomp = new unsigned short [100];

   memset(Comp, 'l', 200);
   memset(Decomp, 'l', 200);

   Decomp[0] = 'l';
   Decomp[1] = 'l';
   Decomp[2] = 'l';
   Decomp[3] = 'l';
   Decomp[4] = 60000;

   LOG << "Comp  [" << RLE_Compress(Comp, Decomp, 5, 16, 4) << "]\r\n";
   LOG << "Decmp [" << RLE_Decompress(Comp, Decomp, 5, 16, 4) << "]\r\n";
	LOG.SaveLogFile("PowerLog.Log");
*/
};

TileSet::~TileSet(void) {
// TileSet Destructor.
   for (int i = 0; i < CurrentWorld; i++)
      delete WorldFileName[i];
   delete [] WorldFileName;
//	delete CompiledView;
	delete VirtualView;
	delete XPString;
	delete MPString;
	delete HPString;	
	int i;
    for (i = 0; i < iNbGSPtr; i++) {
		delete flpDeletePtr[i]->Name;
		delete flpDeletePtr[i];
	}
	for (i = 0; i < iCustomCorner; i++) {
		delete [] CustomCorner[i];
	}
	for (i = 0; i < iCustomWall; i++) {
		delete [] CustomWall[i];
	}
	for (i = 0; i < iCustomDarkWall; i++) {
		delete [] CustomDarkWall[i];
	}
	for (i = 0; i < iCustomBorder; i++) {
		delete [] CustomBorder[i];
	}
	for (i = 0; i < iCustomExcp1; i++) {
		delete [] CustomExcp1[i];
	}
	for (i = 0; i < iCustomExcp2; i++) {
		delete [] CustomExcp2[i];
	}
	for (i = 0; i < iCustomExcp3; i++) {
		delete [] CustomExcp3[i];
	}
	for (i = 0; i < iCustomExcp4; i++) {
		delete [] CustomExcp4[i];
	}
	for (i = 0; i < iCustomExcp5; i++) {
		delete [] CustomExcp5[i];
	}
	for (i = 0; i < iCustomDarkExcp1; i++) {
		delete [] CustomDarkExcp1[i];
	}
	for (i = 0; i < iCustomDarkExcp2; i++) {
		delete [] CustomDarkExcp2[i];
	}
	for (i = 0; i < iCustomDarkExcp3; i++) {
		delete [] CustomDarkExcp3[i];
	}
	for (i = 0; i < iCustomDarkExcp4; i++) {
		delete [] CustomDarkExcp4[i];
	}
	for (i = 0; i < iCustomDarkExcp5; i++) {
		delete [] CustomDarkExcp5[i];
	}
}
	
void Load1(void) {
   World.Mountain[0].LoadSprite("Mount1");
	World.Mountain[1].LoadSprite("Mount2");
	World.Mountain[2].LoadSprite("Mount2");
	World.Mountain[3].LoadSprite("Mount2");
   World.Mountain[4].LoadSprite("Mount2");
	World.Mountain[5].LoadSprite("Cent");
}

void Load2(void) {
	LoadInit();
	LoadIsoSquare(World.MountainRock, "HardRock", T_MOUNT_X, T_MOUNT_Y);
}

void Load3(void) {
      World.Object[126].LoadSprite("Trunk1");
      World.Object[127].LoadSprite("Trunk2");
      World.Object[128].LoadSprite("Trunk3");
}

void Load4(void) {
      World.Object[152].LoadSprite("Display");
}

void Load5(void) {
      World.Object[153].LoadSprite("Display2");
}

void Load6(void) {
      World.Object[154].LoadSprite("Display3");                                                   
}

void Load7(void) {
      World.Object[139].LoadSprite("WoodPile");
}

void Load8(void) {
      World.Object[151].LoadSprite("Knight");
}

void Load9(void) {
      World.Object[129].LoadSprite("Web1");
}

void Load10(void) {
      World.Object[130].LoadSprite("Web2");
}

void Load11(void) {
      World.Object[131].LoadSprite("Web3");
}

void Load12(void) {
      World.Object[132].LoadSprite("Web4");
}

void Load13(void) {
      World.Object[133].LoadSprite("Web5");
}

void Load14(void) 
{
   LoadInit();
	LoadIsoSquare(World.Desert, "Desert", T_SAND_X, T_SAND_Y);
}

void Load15(void) {
      World.Object[134].LoadSprite("Web6");

}

void Load16(void) {
      World.Object[135].LoadSprite("Web7");
}

void Load17(void) {
      World.Object[136].LoadSprite("Web8");
}

void Load18(void) {
      World.Object[137].LoadSprite("Web9");
}

void Load19(void) {
		World.Ice[0].LoadSprite("Ice 1");
		World.Ice[1].LoadSprite("Ice 2");
		World.Ice[2].LoadSprite("Ice 3");
		World.Ice[3].LoadSprite("Ice 4");
		World.Ice[4].LoadSprite("Ice 5");
		World.Ice[5].LoadSprite("Ice 6");
		World.Ice[6].LoadSprite("Ice 7");
		World.Ice[7].LoadSprite("Ice 8");
		World.Ice[8].LoadSprite("Ice 9");
		World.Ice[9].LoadSprite("Ice 10");
}

void Load20(void) {
   World.Bug[0].LoadSprite("Black Tile");	 // Grid
}

void Load21(void) {
	LoadInit();
	LoadIsoSquare(World.DGrass,		  "DGrass", T_DGRASS_X, T_DGRASS_Y);
}

void Load22(void) {
	LoadInit();
   LoadIsoSquare(World.Forest,       "Grass", T_FOREST_X, T_FOREST_Y);
}

void Load23(void) {
	LoadInit()
   LoadIsoSquare(World.Earth,        "EarthTile", T_EARTH_X, T_EARTH_Y);
}

void Load24(void) {
   LoadInit();
	LoadIsoSquare(World.OldRock,		  "TownRoadDale", T_OLDROCK_X, T_OLDROCK_Y);
}

void Load25(void) {                                            
      World.Object[138].LoadSprite("Web10");
}

void Load26(void) {
   LoadInit();
   LoadIsoSquare(World.Rock2, "RockFloor", T_ROCK_X, T_ROCK_Y);
}

void Load27(void) {
	LoadInit();
	LoadIsoSquare(World.Grass,        "NormalGrass", T_NGRASS_X, T_NGRASS_Y);
   //LoadIsoSquare(World.GrassBreak1,  "64kNormalGrassBreaker1", 8, 9);
   //LoadIsoSquare(World.GrassBreak2,  "64kNormalGrassBreaker2", 5, 7);
}
		
void Load28(void) {
      World.DungeonFloor[0][0].LoadSprite("Dtm4 1");
      World.DungeonFloor[0][1].LoadSprite("Dtm4 2");
      World.DungeonFloor[0][2].LoadSprite("Dtm4 3");
      World.DungeonFloor[0][3].LoadSprite("Dtm4 4");
      World.DungeonFloor[0][4].LoadSprite("Dtm4 5");
      World.DungeonFloor[1][0].LoadSprite("Dtm5 1");
      World.DungeonFloor[1][1].LoadSprite("Dtm5 2");
      World.DungeonFloor[1][2].LoadSprite("Dtm5 3");
      World.DungeonFloor[1][3].LoadSprite("Dtm5 4");
      World.DungeonFloor[1][4].LoadSprite("Dtm5 5");
      World.DungeonFloor[2][0].LoadSprite("Dtm1 1");
      World.DungeonFloor[2][1].LoadSprite("Dtm1 2");
      World.DungeonFloor[2][2].LoadSprite("Dtm1 3");
      World.DungeonFloor[2][3].LoadSprite("Dtm1 4");
      World.DungeonFloor[2][4].LoadSprite("Dtm1 5");
      World.DungeonFloor[3][0].LoadSprite("Dtm2 1");
      World.DungeonFloor[3][1].LoadSprite("Dtm2 2");
      World.DungeonFloor[3][2].LoadSprite("Dtm2 3");
      World.DungeonFloor[3][3].LoadSprite("Dtm2 4");
      World.DungeonFloor[3][4].LoadSprite("Dtm2 5");
      World.DungeonFloor[4][0].LoadSprite("Dtm3 1");
      World.DungeonFloor[4][1].LoadSprite("Dtm3 2");
      World.DungeonFloor[4][2].LoadSprite("Dtm3 3");
      World.DungeonFloor[4][3].LoadSprite("Dtm3 4");
      World.DungeonFloor[4][4].LoadSprite("Dtm3 5");
}

void Load29(void) {
      World.DungeonLight[0][0].LoadSprite("DungeonFloorTorch1 1");
      World.DungeonLight[0][1].LoadSprite("Dtm1 2");
      World.DungeonLight[0][2].LoadSprite("Dtm1 3");
      World.DungeonLight[0][3].LoadSprite("Dtm1 4");
      World.DungeonLight[0][4].LoadSprite("Dtm1 5");

      World.DungeonLight[1][0].LoadSprite("DungeonFloorTorch2 1");
      World.DungeonLight[1][1].LoadSprite("DungeonFloorTorch2 2");
      World.DungeonLight[1][2].LoadSprite("Dtm2 3");
      World.DungeonLight[1][3].LoadSprite("Dtm2 4");
      World.DungeonLight[1][4].LoadSprite("Dtm2 5");

      World.DungeonLight[2][0].LoadSprite("DungeonFloorTorch3 1");
      World.DungeonLight[2][1].LoadSprite("DungeonFloorTorch3 2");
      World.DungeonLight[2][2].LoadSprite("DungeonFloorTorch3 3");
      World.DungeonLight[2][3].LoadSprite("Dtm3 4");
      World.DungeonLight[2][4].LoadSprite("Dtm3 5");

      World.DungeonLight[3][0].LoadSprite("DungeonFloorTorch4 1");
      World.DungeonLight[3][1].LoadSprite("DungeonFloorTorch4 2");
      World.DungeonLight[3][2].LoadSprite("DungeonFloorTorch4 3");
      World.DungeonLight[3][3].LoadSprite("DungeonFloorTorch4 4");
      World.DungeonLight[3][4].LoadSprite("Dtm4 5");

      World.DungeonLight[4][0].LoadSprite("DungeonFloorTorch5 1");
      World.DungeonLight[4][1].LoadSprite("DungeonFloorTorch5 2");
      World.DungeonLight[4][2].LoadSprite("DungeonFloorTorch5 3");
      World.DungeonLight[4][3].LoadSprite("DungeonFloorTorch5 4");
      World.DungeonLight[4][4].LoadSprite("DungeonFloorTorch5 5");
}

void Load30(void) {
      World.FlipDungeonLight[0][0].LoadSprite("Dtm1 1");
      World.FlipDungeonLight[0][1].LoadSprite("Dtm1 2");
      World.FlipDungeonLight[0][2].LoadSprite("Dtm1 3");
      World.FlipDungeonLight[0][3].LoadSprite("Dtm1 4");
      World.FlipDungeonLight[0][4].LoadSprite("DungeonFloorTorchLeft (5, 1)");

      World.FlipDungeonLight[1][0].LoadSprite("Dtm2 1");
      World.FlipDungeonLight[1][1].LoadSprite("Dtm2 2");
      World.FlipDungeonLight[1][2].LoadSprite("Dtm2 3");
      World.FlipDungeonLight[1][3].LoadSprite("DungeonFloorTorchLeft (4, 2)");
      World.FlipDungeonLight[1][4].LoadSprite("DungeonFloorTorchLeft (5, 2)");

      World.FlipDungeonLight[2][0].LoadSprite("Dtm3 1");
      World.FlipDungeonLight[2][1].LoadSprite("Dtm3 2");
      World.FlipDungeonLight[2][2].LoadSprite("DungeonFloorTorchLeft (3, 3)");
      World.FlipDungeonLight[2][3].LoadSprite("DungeonFloorTorchLeft (4, 3)");
      World.FlipDungeonLight[2][4].LoadSprite("DungeonFloorTorchLeft (5, 3)");

      World.FlipDungeonLight[3][0].LoadSprite("Dtm4 1");
      World.FlipDungeonLight[3][1].LoadSprite("DungeonFloorTorchLeft (2, 4)");
      World.FlipDungeonLight[3][2].LoadSprite("DungeonFloorTorchLeft (3, 4)");
      World.FlipDungeonLight[3][3].LoadSprite("DungeonFloorTorchLeft (4, 4)");
      World.FlipDungeonLight[3][4].LoadSprite("DungeonFloorTorchLeft (5, 4)");

      World.FlipDungeonLight[4][0].LoadSprite("DungeonFloorTorchLeft (1, 5)");
      World.FlipDungeonLight[4][1].LoadSprite("DungeonFloorTorchLeft (2, 5)");
      World.FlipDungeonLight[4][2].LoadSprite("DungeonFloorTorchLeft (3, 5)");
      World.FlipDungeonLight[4][3].LoadSprite("DungeonFloorTorchLeft (4, 5)");
      World.FlipDungeonLight[4][4].LoadSprite("DungeonFloorTorchLeft (5, 5)");
}

void Load31(void) 
{
	LoadInit();
	LoadIsoSquare(World.Water1,        "Water3", T_WATER1_X, T_WATER1_Y);
	LoadIsoSquare(World.Water2,        "Water4", T_WATER2_X, T_WATER2_Y);
}

void Load32(void) {
	LoadInit();
	LoadIsoSquare(World.Cavern,       "CavernFloor", 14, 14);
}

void Load33(void) {
		World.Floor[0].LoadSprite("Floor: Wooden 1");
		World.Floor[1].LoadSprite("Floor: Wooden 2");
		World.Floor[2].LoadSprite("Floor: Wooden 3");
		World.Floor[3].LoadSprite("Floor: Wooden 4");
		World.Floor[4].LoadSprite("Floor: Wooden 5");
		World.Floor[5].LoadSprite("Floor: Wooden Separation");
}

void Load34(void) {
	LoadInit();
	LoadIsoSquare(World.DungeonOver1, "DungeonFloorOverHeadLight1", 10, 10);
}

void Load35(void) {
	LoadInit();
	LoadIsoSquare(World.DungeonOver2, "DungeonFloorOverHead2", 15, 15);
}

void Load36(void) {
   Load32();
   World.CavernWall[0].LoadSprite("CavernCornerN");
   World.CavernWall[1].LoadSprite("CavernTopWall 1");
   World.CavernWall[2].LoadSprite("CavernTopWall 2");
   World.CavernWall[3].LoadSprite("CavernTopWall 3");
   World.CavernWall[4].LoadSprite("CavernTopWall 4");
   World.CavernWall[5].LoadSprite("CavernTopWall 5");
   World.CavernWall[6].LoadSprite("CavernTopWall 6");
   World.CavernWall[7].LoadSprite("CavernTopWall 7");
   World.CavernWall[8].LoadSprite("CavernTopWall 8");
   World.CavernWall[9].LoadSprite("CavernTopWall 9");
   World.CavernWall[10].LoadSprite("CavernWall 1");
   World.CavernWall[11].LoadSprite("CavernWall 2");
   World.CavernWall[12].LoadSprite("CavernWall 3");
   World.CavernWall[13].LoadSprite("CavernWall 4");
   World.CavernWall[14].LoadSprite("CavernWall 5");
   World.CavernWall[15].LoadSprite("CavernWall 6");
   World.CavernWall[16].LoadSprite("CavernWall 7");
   World.CavernWall[17].LoadSprite("CavernWall 8");
   World.CavernWall[18].LoadSprite("CavernWall 9");
   World.CavernWall[19].LoadSprite("CavernLowWall 1");
   World.CavernWall[20].LoadSprite("CavernLowWall 2");
   World.CavernWall[21].LoadSprite("CavernLowWall 3");
   World.CavernWall[22].LoadSprite("CavernLowWall 4");
   World.CavernWall[23].LoadSprite("CavernLowWall 5");
   World.CavernWall[24].LoadSprite("CavernLowWall 6");
   World.CavernWall[25].LoadSprite("CavernLowWall 7");
   World.CavernWall[26].LoadSprite("CavernLowWall 8");
   World.CavernWall[27].LoadSprite("CavernLowWall 9");
   World.CavernWall[28].LoadSprite("CavernRightWall 1");
   World.CavernWall[29].LoadSprite("CavernRightWall 2");
   World.CavernWall[30].LoadSprite("CavernRightWall 3");
   World.CavernWall[31].LoadSprite("CavernRightWall 4");
   World.CavernWall[32].LoadSprite("CavernRightWall 5");
   World.CavernWall[33].LoadSprite("CavernRightWall 6"); 
   World.CavernWall[34].LoadSprite("CavernRightWall 7");
   World.CavernWall[35].LoadSprite("CavernRightWall 8");
   World.CavernWall[36].LoadSprite("CavernRightWall 9");
   World.CavernWall[37].LoadSprite("CavernCornerV");
   World.CavernWall[38].LoadSprite("CavernCorner");
   World.CavernWall[39].LoadSprite("CavernCorner2");
   World.CavernWall[40].LoadSprite("InnerCornerN");
   World.CavernWall[41].LoadSprite("InnerCornerV");
   World.CavernWall[42].LoadSprite("InnerCorner");
   World.CavernWall[43].LoadSprite("InnerCorner2");
   World.Object[161].LoadSprite("CavernInternEntrance");
}  

void Load37(void) {
      World.Object[122].LoadSprite("Stairs");
}

void Load38(void) {
      World.Object[124].LoadSprite("Stair2");
}

void Load39(void) {
      World.Object[123].LoadSprite("StairUp");
}

void Load40(void) {
      World.Object[125].LoadSprite("Stair2Up");
}

void Load41(void) {
      World.Object[118].LoadSprite("Rock1");
}

void Load42(void) {
      World.Object[119].LoadSprite("Rock2");
}

void Load43(void) {
      World.Object[120].LoadSprite("Rock3");
}

void Load44(void) {
      World.Object[121].LoadSprite("Rock4");
}

void Load266(void);
void Load267(void);

void Load45(void) {
	Load27();   // GRASS
	Load266();   // Smooth3
	Load24(); // Pavement
   //Load267(); // Old Smoothing.
}

void Load46(void) {
		Load27();
		World.GraRoc2[0].LoadSprite("Townrsmo 4");
		World.GraRoc2[1].LoadSprite("Townrsmo 1");
		World.GraRoc2[2].LoadSprite("Townrsmo 2");
		World.GraRoc2[3].LoadSprite("Townrsmo 3");
}

void Load47(void) {
		World.SRockFloor[0].LoadSprite("srf 1");
		World.SRockFloor[1].LoadSprite("srf 2");
		World.SRockFloor[2].LoadSprite("srf 3");
		World.SRockFloor[3].LoadSprite("srf 4");
}

void Load48(void) {
		World.Smoothing[0].LoadSprite("tmpl1 1");
      World.Smoothing[1].LoadSprite("tmpl1 2");
      World.Smoothing[2].LoadSprite("tmpl1 3");
      World.Smoothing[3].LoadSprite("tmpl1 4");
      World.Smoothing[4].LoadSprite("tmpl1 5");
      World.Smoothing[5].LoadSprite("tmpl1 6");
      World.Smoothing[6].LoadSprite("tmpl1 7");
      World.Smoothing[7].LoadSprite("tmpl1 8");
      World.Smoothing[8].LoadSprite("tmpl1 9");
      World.Smoothing[9].LoadSprite("tmpl1 10");
      World.Smoothing[10].LoadSprite("tmpl1 11");
      World.Smoothing[11].LoadSprite("tmpl1 12");
      World.Smoothing[12].LoadSprite("tmpl1 13");
      World.Smoothing[13].LoadSprite("tmpl1 14");
      World.Smoothing[14].LoadSprite("tmpl1 15");
      World.Smoothing[15].LoadSprite("tmpl1 16");
      World.Smoothing[16].LoadSprite("tmpl1 17");
      World.Smoothing[17].LoadSprite("tmpl1 18");
      World.Smoothing[18].LoadSprite("tmpl1 19");
      World.Smoothing[19].LoadSprite("tmpl1 20");
      World.Smoothing[20].LoadSprite("tmpl1 21");
      World.Smoothing[21].LoadSprite("tmpl1 22");
      World.Smoothing[22].LoadSprite("tmpl1 23");
      World.Smoothing[23].LoadSprite("tmpl1 24");
      World.Smoothing[24].LoadSprite("tmpl1 25");
      World.Smoothing[25].LoadSprite("tmpl1 26");
      World.Smoothing[26].LoadSprite("tmpl1 27");
      World.Smoothing[27].LoadSprite("tmpl1 28");
      World.Smoothing[28].LoadSprite("tmpl1 29");
      World.Smoothing[29].LoadSprite("tmpl1 30");
      World.Smoothing[30].LoadSprite("tmpl1 31");
      World.Smoothing[31].LoadSprite("tmpl1 32");
}

void Load49(void) {
      World.Smoothing2[0].LoadSprite("tmpl3 1");
      World.Smoothing2[1].LoadSprite("tmpl3 2");
      World.Smoothing2[2].LoadSprite("tmpl3 3");
      World.Smoothing2[3].LoadSprite("tmpl3 4");
      World.Smoothing2[4].LoadSprite("tmpl3 5");
      World.Smoothing2[5].LoadSprite("tmpl3 6");
      World.Smoothing2[6].LoadSprite("tmpl3 7");
      World.Smoothing2[7].LoadSprite("tmpl3 8");
      World.Smoothing2[8].LoadSprite("tmpl3 9");
      World.Smoothing2[9].LoadSprite("tmpl3 10");
      World.Smoothing2[10].LoadSprite("tmpl3 11");
      World.Smoothing2[11].LoadSprite("tmpl3 12");
      World.Smoothing2[12].LoadSprite("tmpl3 13");
      World.Smoothing2[13].LoadSprite("tmpl3 14");
      World.Smoothing2[14].LoadSprite("tmpl3 15");
      World.Smoothing2[15].LoadSprite("tmpl3 16");
      World.Smoothing2[16].LoadSprite("tmpl3 17");
      World.Smoothing2[17].LoadSprite("tmpl3 18");
      World.Smoothing2[18].LoadSprite("tmpl3 19");
      World.Smoothing2[19].LoadSprite("tmpl3 20");
      World.Smoothing2[20].LoadSprite("tmpl3 21");
      World.Smoothing2[21].LoadSprite("tmpl3 22");
      World.Smoothing2[22].LoadSprite("tmpl3 23");
      World.Smoothing2[23].LoadSprite("tmpl3 24");
      World.Smoothing2[24].LoadSprite("tmpl3 25");
      World.Smoothing2[25].LoadSprite("tmpl3 26");
      World.Smoothing2[26].LoadSprite("tmpl3 27");
      World.Smoothing2[27].LoadSprite("tmpl3 28");
      World.Smoothing2[28].LoadSprite("tmpl3 29");
      World.Smoothing2[29].LoadSprite("tmpl3 30");
      World.Smoothing2[30].LoadSprite("tmpl3 31");
      World.Smoothing2[31].LoadSprite("tmpl3 32");
}

void Load50(void) {
		World.RivWat[0].LoadSprite("ShaWat 1");
		World.RivWat[1].LoadSprite("ShaWat 2");
		World.RivWat[2].LoadSprite("ShaWat 3");
		World.RivWat[3].LoadSprite("ShaWat 4");
		World.RivWat[4].LoadSprite("ShaWat 5");
		World.RivWat[5].LoadSprite("ShaWat 6");
}

void Load51(void) {
		World.Object[27].LoadSprite("Bed Simple");
}

void Load52(void) {
		World.Object[26].LoadSprite("Bed Double Type 1");
}

void Load53(void) {
		World.Object[28].LoadSprite("Bed Double Type 2");
}

void Load54(void) {
		World.Object[0] .LoadSprite("Wooden Armoir SW");
}

void Load55(void) {
		World.Object[1] .LoadSprite("Etagere SW");
}

void Load56(void) {
	World.Object[55].LoadSprite("Shito 2");
}

void Load57(void) {
		World.Object[54].LoadSprite("Shito 1");
}

void Load58(void) {
		World.Object[2] .LoadSprite("Bureau SW");
}

void Load59(void) {
		World.Object[3] .LoadSprite("Red-Wooden Armoir SW");
}

void Load60(void) {
		World.Object[4] .LoadSprite("Colonne Cercle Bas");
}

void Load61(void) {
		World.Object[5] .LoadSprite("Colonne Cercle Bas-Haut");
}

void Load62(void) {
		World.Object[6] .LoadSprite("Colonne Carrer Bas-Haut");
}

void Load63(void) {
		World.Object[7] .LoadSprite("Colonne Greque");
}

void Load64(void) {
      World.Object[87].LoadSprite("Painting1");
      World.Object[88].LoadSprite("Painting2");
      World.Object[89].LoadSprite("Painting3");
}

void Load65(void) {
      World.Object[111].LoadSprite("HeadPile");
}

void Load66(void) {
      World.Object[115].LoadSprite("SkeletonCorpse2");
      World.Object[116].LoadSprite("SkeletonCorpsePile1");
}

void Load67(void) {
      World.Object[113].LoadSprite("Manacle 1");
      World.Object[114].LoadSprite("Manacle 2");
}

void Load68(void) {
      World.Object[155].LoadSprite("SmallTreeInBowl");
}

void Load69(void) {
      World.Object[117].LoadSprite("DungeonTomb1");
}

void Load70(void) {
      World.Object[101].LoadSprite("Blood1");
      World.Object[102].LoadSprite("Blood2");
}

void Load71(void) {
      World.Object[76].LoadSprite("Sword");
      World.Object[77].LoadSprite("AxeCross");
}

void Load72(void) {
      World.Object[108].LoadSprite("DungeonWell");
}

void Load73(void) {
      World.Object[109].LoadSprite("GiantWell");
}

void Load74(void) {
      World.Object[110].LoadSprite("Head");
      World.Object[112].LoadSprite("Heads");
}

void Load75(void) {
      World.Object[104].LoadSprite("DebrisPile2");
}

void Load76(void) {
      World.Object[105].LoadSprite("DebrisPile3");
}

void Load77(void) {
      World.Object[106].LoadSprite("DebrisPile4");
}

void Load78(void) {
      World.Object[107].LoadSprite("DebrisPile5");
}

void Load79(void) {
      World.Object[75].LoadSprite("Anvil");
}

void Load80(void) {
      World.Object[103].LoadSprite("DebrisPile1");
}

void Load81(void) {
      World.Object[99].LoadSprite("FirePlace1");
}

void Load82(void) {
      World.Object[100].LoadSprite("Oven");
}

void Load83(void) {
      World.Object[74].LoadSprite("RuinedBed1");
}

void Load84(void) {
      World.Object[79].LoadSprite("Desk");
}

void Load85(void) {
      World.Object[80].LoadSprite("Desk2");
}

void Load86(void) {
      World.Object[81].LoadSprite("Cerebus");
}

void Load87(void) {
      World.Object[82].LoadSprite("MonkStudy");
}

void Load88(void) {
      World.Object[83].LoadSprite("Lutrin");
}

void Load89(void) {
      World.Object[84].LoadSprite("Throne");
}

void Load90(void) {
      World.Object[86].LoadSprite("Crypt");
}

void Load91(void) {
      World.Object[98].LoadSprite("Bonfire");
}

void Load92(void) {
      World.Object[97].LoadSprite("Banquet");
}

void Load93(void) {
      World.Object[90].LoadSprite("Tapisall 1");
}

void Load94(void) {
      World.Object[156].LoadSprite("Stone2H");
}

void Load95(void) {
      World.Object[164].LoadSprite("Stone3");
}

void Load96(void) {
      World.Object[91].LoadSprite("Stone1H");
}

void Load97(void) {
      World.Object[94].LoadSprite("TapisRouge 2");
}

void Load98(void) {
      World.Object[95].LoadSprite("TapisRouge 1");
}

void Load99(void) {
      World.Object[96].LoadSprite("TapisRouge 3");
}

void Load100(void) {
      World.Object[163].LoadSprite("Boulder");
}

void Load101(void) {
      World.Object[165].LoadSprite("PrisonBed");
}

void Load102(void) {
      World.Object[73].LoadSprite("RuinedMirror1");
}

void Load103(void) {
      World.Object[85].LoadSprite("BigVase");
}

void Load104(void) {
      World.Object[72].LoadSprite("RuinedTable1");
}

void Load105(void) {
      World.Object[70].LoadSprite("WeaponRacks 1");
}

void Load106(void) {
      World.Object[71].LoadSprite("WeaponRacks 2");
}

void Load107(void) {
      World.Object[68].LoadSprite("Dummy");
      World.Object[69].LoadSprite("Dummy2");
}

void Load108(void) {
      World.Object[67].LoadSprite("ArmorOnARack");
}

void Load109(void) {
      World.Object[66].LoadSprite("Archer");
}

void Load110(void) {
      World.Object[65].LoadSprite("BlackSmithForgery");
}

void Load111(void) {
		World.Object[8] .LoadSprite("Tomb Carrer");
		World.Object[9] .LoadSprite("Tomb Hemisphere");
		World.Object[10].LoadSprite("Tomb Croix");
		World.Object[11].LoadSprite("Tomb Plaque");	
}

void Load112(void) {
		World.Object[12].LoadSprite("Golden Mirror");
}

void Load113(void) {
		World.Object[13].LoadSprite("Horloge Grand Pere");
}

void Load115(void) {
		World.Object[15].LoadSprite("Roche 1");
}

void Load116(void) {
		World.Object[32].LoadSprite("Medium Rock");
}

void Load117(void) {
		World.Object[33].LoadSprite("Small Rock");
}

void Load118(void) {
	//OutputDebugString("Color 26 should not be in .MAP\r\n");
}

void Load119(void) {
	//OutputDebugString("Color 27 should not be in .MAP\r\n");
}

void Load120(void) {
	//OutputDebugString("Color 28 should not be in .MAP\r\n");
}

void Load121(void) {
		World.Object[37].LoadSprite("Shop Sign");
}

void Load122(void) {
		World.Object[38].LoadSprite("Plaque Wood");
}

void Load123(void) {
		World.Object[39].LoadSprite("Plaque Marble");
}

void Load124(void) {
		World.Object[40].LoadSprite("Small Wood Sign");
}

void Load125(void) {
		World.Object[41].LoadSprite("Small Gold Sign");
}

void Load126(void) {
		World.Object[42].LoadSprite("Small Marble Sign");
}

void Load127(void) {
		World.Object[57].LoadSprite("Signs2 1");
}

void Load128(void) {
		World.Object[43].LoadSprite("Wood Sign 1");
}

void Load129(void) {
		World.Object[44].LoadSprite("Wood Sign 2");
}

void Load130(void) {
      World.Object[92].LoadSprite("Shelf");
}

void Load131(void) {
		World.Object[30].LoadSprite("Misc 4 - Part 2 2");
}

void Load132(void) {
		World.Object[50].LoadSprite("Kitchen 1");
}

void Load133(void) 
{
		World.Object[51].LoadSprite("Lights 3");
      World.Object[242].LoadSprite("nLights 3");
      World.Object[243].LoadSprite("mLights 3");
}

void Load134(void) {
      World.Object[157].LoadSprite("Bridge1");
}

void Load135(void) {
      World.Object[158].LoadSprite("Bridge2");
}

void Load136(void) {
      World.Object[162].LoadSprite("Bridge3");
}

void Load137(void) {
      World.Object[159].LoadSprite("Border1");
}

void Load138(void) {
      World.Object[160].LoadSprite("Border2");
}

void Load139(void) {
		World.Object[45].LoadSprite("Misc 7 - All 8");
}

void Load140(void) {
		World.Object[48].LoadSprite("Chairs 1");
}

void Load141(void) {
		World.Object[49].LoadSprite("Shito 6"); // Chairs 6
}

void Load142(void) {
		World.Object[52].LoadSprite("Lights 4");
}

void Load143(void) {
		World.Object[53].LoadSprite("Lights 5");
}

void Load144(void) {
		World.Object[46].LoadSprite("Misc 8 - All 3");
}

void Load145(void) {
		World.Object[47].LoadSprite("Misc 8 - All 4");
}

void Load146(void) {
		World.Object[29].LoadSprite("Fontaine");
}

void Load147(void) {
      World.Object[93].LoadSprite("Tapisall 2");
}

void Load148(void) {
		World.Object[31].LoadSprite("Motte Terre");
}

void Load149(void) {
		World.Object[56].LoadSprite("TortureChair");
}

void Load150(void) {
		World.Object[62].LoadSprite("Ble");
}

void Load151(void) 
{
	   World.Object[61].LoadSprite("Fountaine20000");
}

void Load329(void) 
{
	   World.Moulin[0].LoadSprite("MoulinD-0024");
}

void Load330(void) 
{
	   World.Moulin[1].LoadSprite("MoulinG-0024");
}



void Load152(void) {
		World.Object[63].LoadSprite("BarCounter");
}

void Load153(void) {
		World.Object[64].LoadSprite("ShopTable1");
}

void Load154(void) {
		World.Object[59].LoadSprite("Well");
}

void Load155(void) {
		World.Object[60].LoadSprite("TownBoard 1");
}

void Load156(void) {
		World.Object[58].LoadSprite("Autel");
}

void Load157(void) {
		World.Object[23].LoadSprite("Obelisk 1");
}

void Load158(void) {
      World.Object[141].LoadSprite("Stal1");
}

void Load159(void) {
      World.Object[142].LoadSprite("Stal2");
}

void Load160(void) {
      World.Object[143].LoadSprite("Stal3");
}

void Load161(void) {
      World.Object[144].LoadSprite("Stal4");
}

void Load162(void) {
      World.Object[145].LoadSprite("Stal5");
}

void Load163(void) {
      World.Object[146].LoadSprite("Stal6");
}

void Load164(void) {
      World.Object[147].LoadSprite("Stal7");
}

void Load165(void) {
      World.Object[148].LoadSprite("LargeC1");
}

void Load166(void) {
      World.Object[149].LoadSprite("LargeC2");
}

void Load167(void) {
      World.Object[150].LoadSprite("LargeC3");
}

void Load168(void) {
		World.Object[16].LoadSprite("Menhir de Roche");
}

void Load169(void) {
		World.Object[17].LoadSprite("Menhir Noir");
}

void Load170(void) {
		World.Object[18].LoadSprite("Moyenne Table Ronde Bois");
}

void Load171(void) {
		World.Object[19].LoadSprite("Petite Table Bois Rectangle");
}

void Load172(void) {
		World.Object[20].LoadSprite("Petite Table Ronde Bois");
}

void Load173(void) {
		World.Object[21].LoadSprite("Table Carre Moyenne Bois");
}

void Load174(void) {
		World.Object[22].LoadSprite("Grande Table Rectangle Bois");
		World.Object[25].LoadSprite("LongTable2");
      World.Object[78].LoadSprite("Table2");
}

void Load175(void) {
		World.Tree[0].LoadSprite("SmallTree4");
		World.Tree[1].LoadSprite("SmallTree3");
		World.Tree[2].LoadSprite("SmallTree2");
		World.Tree[3].LoadSprite("SmallTree1");
}

void Load176(void) {
		World.Tree[16].LoadSprite("HighTree2");
}

void Load177(void) {
		World.Tree[12].LoadSprite("LeafLessTree2");
		World.Tree[13].LoadSprite("LeafLessTree1");
}

void Load178(void) {
		World.Tree[7].LoadSprite("OakTree2");
		World.Tree[8].LoadSprite("OakTree1");
		World.Tree[15].LoadSprite("LargeBasedLowLeafedTree1");
}

void Load179(void) {
		World.Tree[9].LoadSprite("NearlyLeafLessTree1");
		World.Tree[10].LoadSprite("MultiTrunkTree2");
}

void Load180(void) {
		World.Tree[5].LoadSprite("PalmTree2");
		World.Tree[6].LoadSprite("PalmTree1");
}

void Load181(void) {
		World.Tree[14].LoadSprite("LargePine1");
		World.Tree[28].LoadSprite("HighPine1");
}

void Load182(void) {
		World.Tree[19].LoadSprite("CityTree2");
		World.Tree[20].LoadSprite("CityTree1");
}

void Load183(void) {
		World.Tree[23].LoadSprite("AppleTree1");
}

void Load184(void) {
		World.Tree[12].LoadSprite("LeafLessTree2");
		World.Tree[13].LoadSprite("LeafLessTree1");
		World.Tree[18].LoadSprite("DeadTree1");
}

void Load185(void) {
		World.Tree[4].LoadSprite("PinkTree1");
}

void Load186(void) {
		World.Tree[11].LoadSprite("MultiTrunkTree1");
		World.Tree[21].LoadSprite("Bush2");
		World.Tree[22].LoadSprite("Bush1");
}

void Load187(void) {
		World.Tree[24].LoadSprite("Fougere1");
		World.Tree[25].LoadSprite("Fougere2");
		World.Tree[26].LoadSprite("PlantModel1");
		World.Tree[27].LoadSprite("SmallRockPack1");
}

void Load188(void) {
		World.Tree[29].LoadSprite("Cactus1");
		World.Tree[30].LoadSprite("Cactus2");
		World.Tree[31].LoadSprite("HighCactus1");
		World.Tree[32].LoadSprite("HighCactus2");
}

void Load189(void) {
		World.Tree[17].LoadSprite("HighTree1");
}

void Load190(void) {
      World.Tree[33].LoadSprite("DarkTree1");
      World.Tree[34].LoadSprite("DarkTree2");
      World.Tree[35].LoadSprite("DarkTree3");
      World.Tree[36].LoadSprite("DarkTree4");
      World.Tree[37].LoadSprite("DarkTree5");
      World.Tree[38].LoadSprite("DarkTree6");
}

void Load191(void) {
      World.DungeonWall[22].LoadSprite("DungeonMainArcEnd1");
      World.DungeonWall[23].LoadSprite("DungeonMainArcEnd2 1");
      World.DungeonWall[24].LoadSprite("DungeonMainArcEnd2 2");
      World.DungeonWall[25].LoadSprite("DungeonDarkArcEnd1");
      World.DungeonWall[26].LoadSprite("DungeonDarkArcEnd2 1");
      World.DungeonWall[27].LoadSprite("DungeonDarkArcEnd2 2");

      World.DungeonWall[14].LoadSprite("DungeonEnd1");
      World.DungeonWall[15].LoadSprite("DungeonEnd2");
      World.DungeonWall[16].LoadSprite("DungeonEnd3");
      World.DungeonWall[17].LoadSprite("DungeonEnd4");
      World.DungeonWall[18].LoadSprite("DungeonDoorSide1");
      World.DungeonWall[19].LoadSprite("DungeonDoorSide2");
      World.DungeonWall[20].LoadSprite("DungeonDoorSide3");
      World.DungeonWall[21].LoadSprite("DungeonDoorSide4");
      World.DungeonWallSpec[0].LoadSprite("DungeonMainTorch1 1");
      World.DungeonWallSpec[1].LoadSprite("DungeonMainTorch1 2");
      World.DungeonWallSpec[2].LoadSprite("DungeonMainTorch1 3");
      World.DungeonWallSpec[3].LoadSprite("DungeonMainTorch1 4");
      World.DungeonWallSpec[4].LoadSprite("DungeonMainTorch1 5");
      World.FlipDungeonWallSpec[0].LoadSprite("DungeonDarkTorch1 1");
      World.FlipDungeonWallSpec[1].LoadSprite("DungeonDarkTorch1 2");
      World.FlipDungeonWallSpec[2].LoadSprite("DungeonDarkTorch1 3");
      World.FlipDungeonWallSpec[3].LoadSprite("DungeonDarkTorch1 4");
      World.FlipDungeonWallSpec[4].LoadSprite("DungeonDarkTorch1 5");
      World.DungeonWallSpec2[0].LoadSprite("DungeonMainArc 1");
      World.DungeonWallSpec2[1].LoadSprite("DungeonMainArc 2");
      World.DungeonWallSpec2[2].LoadSprite("DungeonMainArc 3");
      World.DungeonWallSpec2[3].LoadSprite("DungeonMainArc 4");
      World.DungeonWallSpec2[4].LoadSprite("DungeonMainArc 5");
      World.FlipDungeonWallSpec2[0].LoadSprite("DungeonDarkArc 1");
      World.FlipDungeonWallSpec2[1].LoadSprite("DungeonDarkArc 2");
      World.FlipDungeonWallSpec2[2].LoadSprite("DungeonDarkArc 3");
      World.FlipDungeonWallSpec2[3].LoadSprite("DungeonDarkArc 4");
      World.FlipDungeonWallSpec2[4].LoadSprite("DungeonDarkArc 5");
      World.DungeonWallSpec3[0].LoadSprite("DungeonMainArcTorch 1");
      World.DungeonWallSpec3[1].LoadSprite("DungeonMainArcTorch 2");
      World.DungeonWallSpec3[2].LoadSprite("DungeonMainArcTorch 3");
      World.DungeonWallSpec3[3].LoadSprite("DungeonMainArcTorch 4");
      World.DungeonWallSpec3[4].LoadSprite("DungeonMainArcTorch 5");
      World.FlipDungeonWallSpec3[0].LoadSprite("DungeonDarkArcTorch 1");
      World.FlipDungeonWallSpec3[1].LoadSprite("DungeonDarkArcTorch 2");
      World.FlipDungeonWallSpec3[2].LoadSprite("DungeonDarkArcTorch 3");
      World.FlipDungeonWallSpec3[3].LoadSprite("DungeonDarkArcTorch 4");
      World.FlipDungeonWallSpec3[4].LoadSprite("DungeonDarkArcTorch 5");
		World.DungeonWall[0] .LoadSprite("DungeonMainWall 1");
		World.DungeonWall[1] .LoadSprite("DungeonMainWall 2");
		World.DungeonWall[2] .LoadSprite("DungeonMainWall 3");
		World.DungeonWall[3] .LoadSprite("DungeonMainWall 4");
		World.DungeonWall[4] .LoadSprite("DungeonMainWall 5");
		World.DungeonWall[5] .LoadSprite("DungeonMainCornerV");
		World.DungeonWall[6] .LoadSprite("DungeonMainCornerN");
		World.DungeonWall[7] .LoadSprite("DungeonMainCorner");
      World.DungeonWall[8] .LoadSprite("DungeonMainCornerX");
      World.DungeonWall[9] .LoadSprite("DungeonMainCorner2"); 
      World.DungeonWall[10].LoadSprite("DTNE");
      World.DungeonWall[11].LoadSprite("DTNW");
      World.DungeonWall[12].LoadSprite("DTSE");
      World.DungeonWall[13].LoadSprite("DTSW");
      World.FlipDungeonWall[0] .LoadSprite("DungeonDarkWall 1");
      World.FlipDungeonWall[1] .LoadSprite("DungeonDarkWall 2");
      World.FlipDungeonWall[2] .LoadSprite("DungeonDarkWall 3");
      World.FlipDungeonWall[3] .LoadSprite("DungeonDarkWall 4");
      World.FlipDungeonWall[4] .LoadSprite("DungeonDarkWall 5");
}

void Load192(void) {
      World.Rampart[0].LoadSprite("Rampart 2");
      World.Rampart[1].LoadSprite("Tour");
      World.Rampart[4].LoadSprite("Rampart 5");
      World.Rampart[5].LoadSprite("Rampart 4");
      World.Rampart[6].LoadSprite("Rampart 3");
}

void Load193(void) {
		World.Grave[0] .LoadSprite("Cemetery Gates /");
		World.Grave[1] .LoadSprite("Cemetery Gates -");
		World.Grave[2] .LoadSprite("Cemetery Gates .|");
		World.Grave[3] .LoadSprite("Cemetery Gates ^");
		World.Grave[4] .LoadSprite("Cemetery Gates >");
		World.Grave[5] .LoadSprite("Cemetery Gates v");
		World.Grave[6] .LoadSprite("Cemetery Gates \\v");
		World.Grave[7] .LoadSprite("Cemetery Gates /^");
		World.Grave[8] .LoadSprite("Cemetery Gates X");
		World.Grave[9] .LoadSprite("Gate");
}

void Load194(void) {
        World.WhiteWall[13].LoadSprite("StuccoMainSmallWallB");
		World.WhiteWall[0] .LoadSprite("StuccoDarkSmallWallB");
		World.WhiteWall[3] .LoadSprite("StuccoCornerN");
		World.WhiteWall[4] .LoadSprite("StuccoCorner");
		World.WhiteWall[16].LoadSprite("StuccoCorner2");
		World.WhiteWall[5] .LoadSprite("StuccoCornerV");
		World.WhiteWall[6] .LoadSprite("StuccoCornerTNE");
		World.WhiteWall[7] .LoadSprite("StuccoCornerTSE");
		World.WhiteWall[1] .LoadSprite("DungeonDarkArcTorch 5");
		World.WhiteWall[2] .LoadSprite("DungeonDarkArcTorch 5");
		World.WhiteWall[8] .LoadSprite("StuccoCornerX");
		World.WhiteWall[9] .LoadSprite("StuccoMainWindowA");
		World.WhiteWall[10].LoadSprite("StuccoDarkSmallWallA");
		World.WhiteWall[11].LoadSprite("StuccoDarkWall 2");
		World.WhiteWall[12].LoadSprite("StuccoDarkWall 3");
		World.WhiteWall[14].LoadSprite("StuccoMainWall 2");
		World.WhiteWall[22].LoadSprite("StuccoDarkWindowA");
		World.WhiteWall[15].LoadSprite("StuccoMainWall 1");
		World.WhiteWall[17].LoadSprite("StuccoMainSmallWallA");
		World.WhiteWall[23].LoadSprite("StuccoMainWindowB");
		World.WhiteWall[25].LoadSprite("StuccoMainWindowC");
		World.WhiteWall[22].LoadSprite("StuccoDarkWindowA");
		World.WhiteWall[24].LoadSprite("StuccoDarkWindowB");
		World.WhiteWall[26].LoadSprite("StuccoDarkWindowC");
      World.WhiteWall[18].LoadSprite("StuccoCornerTNW");
      World.WhiteWall[19].LoadSprite("StuccoCornerTSW");
      World.WhiteWall[27].LoadSprite("StuccoDarkWallDoor 1");
      World.WhiteWall[28].LoadSprite("StuccoDarkWallDoor 2");
      World.WhiteWall[29].LoadSprite("StuccoMainWallDoor 2");
      World.WhiteWall[30].LoadSprite("StuccoMainWallDoor 1");
      World.WhiteWall[20].LoadSprite("StuccoMainSmallWallC");
      World.WhiteWall[21].LoadSprite("StuccoDarkSmallWallC");
}

void Load195(void) {
		World.BrickWall[0] .LoadSprite("BrickDarkWall 1");
		World.BrickWall[1] .LoadSprite("DungeonDarkArcTorch 5");
		World.BrickWall[2] .LoadSprite("DungeonDarkArcTorch 5");
		World.BrickWall[3] .LoadSprite("BrickWallCornerN");
		World.BrickWall[4] .LoadSprite("BrickWallCorner");
		World.BrickWall[5] .LoadSprite("BrickWallCornerV");
		World.BrickWall[6] .LoadSprite("BrickWallCornerTNE");
		World.BrickWall[7] .LoadSprite("BrickWallCornerTSE");
		World.BrickWall[8] .LoadSprite("BrickWallCornerX");
		World.BrickWall[9] .LoadSprite("BrickWallWindow 2");
		World.BrickWall[10].LoadSprite("BrickDarkWall 2"); 
		World.BrickWall[11].LoadSprite("BrickDarkWall 2"); // Enlever
		World.BrickWall[12].LoadSprite("BrickDarkWall 3"); // Enlever
      World.BrickWall[13].LoadSprite("BrickWall 1");
		World.BrickWall[14].LoadSprite("BrickWall 2"); // Enlever
		World.BrickWall[16].LoadSprite("BrickWallCorner2");
		World.BrickWall[17].LoadSprite("BrickWall 2");
      World.BrickWall[18].LoadSprite("BrickWallCornerTNW");
      World.BrickWall[19].LoadSprite("BrickWallCornerTSW");
      World.BrickWall[20].LoadSprite("BrickWall 3"); 
      World.BrickWall[21].LoadSprite("BrickDarkWall 3");
		World.BrickWall[22].LoadSprite("BrickDarkWallWindow 2");
		World.BrickWall[23].LoadSprite("BrickWallWindow 1");
		World.BrickWall[24].LoadSprite("BrickDarkWallWindow 3");
		World.BrickWall[25].LoadSprite("BrickWallWindow 3");
		World.BrickWall[26].LoadSprite("BrickDarkWallWindow 1");
      World.BrickWall[27].LoadSprite("BrickDarkWallDoor 1");
      World.BrickWall[28].LoadSprite("BrickDarkWallDoor 2");
      World.BrickWall[29].LoadSprite("BrickWallDoor 2");
      World.BrickWall[30].LoadSprite("BrickWallDoor 1");
		World.BrickWall[15].LoadSprite("BrickWall 1"); // Enlever
}

void Load196(void) {
		World.Haie[0] .LoadSprite("DungeonMainWall 1");
		World.Haie[1] .LoadSprite("DungeonMainWall 1");
		World.Haie[2] .LoadSprite("DungeonMainWall 1");
		World.Haie[3] .LoadSprite("DungeonMainWall 1");
		World.Haie[4] .LoadSprite("DungeonMainWall 1");
		World.Haie[5] .LoadSprite("DungeonMainWall 1");
		World.Haie[6] .LoadSprite("DungeonMainWall 1");
}

void Load197(void) {
		World.Lava[0].LoadSprite("Lava 1");
		World.Lava[1].LoadSprite("Lava 2");
		World.Lava[2].LoadSprite("Lava 3");
		World.Lava[3].LoadSprite("Lava 4");
		World.Lava[4].LoadSprite("Lava 5");
		World.Lava[5].LoadSprite("Lava 6");
		World.Lava[6].LoadSprite("Lava 7");
		World.Lava[7].LoadSprite("Lava 8");
		World.Lava[8].LoadSprite("Lava 9");           
		World.Lava[9].LoadSprite("Lava 10");
}

void Load198(void) {
	//OutputDebugString("Color mountain 1 should not be in .MAP\r\n");
}

void Load199(void) {
	Load22();
	Load23();
	Load48();
}

void Load200(void) {
	Load27();
	Load23();
	Load48();
}

void Load201(void) {
	Load27();
	Load22();
	Load48();
}

void Load202(void) {
	Load31();
	Load14();
	Load49();
}

void Load203(void) {
	Load31();
	Load22();
	Load49();
}

void Load204(void) {
	Load31();
	Load27();
	Load49();
}

void Load205(void) {
	Load27();
	Load14();
	Load49();
}

void Load206(void) {
	Load23();
	Load14();
	Load49();
}

void Load207(void) {
	Load22();
	Load14();
	Load49();
}

void Load208(void) {
	Load31();
	Load32();
	Load49();
}

void Load209(void) {
	Load2();
	Load14();
	Load48();
}

void Load210(void) {
	Load2();
	Load22();
	Load48();
}

void Load211(void) {
	Load2();
	Load23();
	Load48();
}

void Load212(void) {
	Load2();
	Load27();
	Load48();
}

void Load213(void) {
	Load31();
	Load23();
	Load49();
}

void Load214(void) {
	Load31();
	Load2();
	Load49();
}

void Load215(void) {
	//OutputDebugString("Color BED_RICHE should not be in .MAP\r\n");
}

void Load216(void) {
	//OutputDebugString("Color ROCK_WALL should not be in .MAP\r\n");
}

void Load217(void) {
	//OutputDebugString("Color WHITE_WALL should not be in .MAP\r\n");
}

void Load218(void) {
	//OutputDebugString("Color GRAVEYARD should not be in .MAP\r\n");
}

void Load219(void) {
	//OutputDebugString("Color RAMPART should not be in .MAP\r\n");
}

void Load220(void) {
	//OutputDebugString("Color CAVERNWALL_2 should not be in .MAP\r\n");
}

void Load221(void) {
	//OutputDebugString("Color DUNGEON_WALL should not be in .MAP\r\n");
}

void Load222(void) {
	//OutputDebugString("Color CAVERN_WALL should not be in .MAP\r\n");
}

void Load223(void) {
	//OutputDebugString("Color WALLSPEC1 should not be in .MAP\r\n");
}

void Load224(void) {
	//OutputDebugString("Color WALLSPEC2 should not be in .MAP\r\n");
}

void Load225(void) {
	//OutputDebugString("Color WALLSPEC3 should not be in .MAP\r\n");
}

void Load226(void) {
	//OutputDebugString("Color WALLSPEC4 should not be in .MAP\r\n");
}

void Load227(void) {
	//OutputDebugString("Color WALLSPEC5 should not be in .MAP\r\n");
}

void Load228(void) {
	//OutputDebugString("Color BED_PAUVRE should not be in .MAP\r\n");
}

void Load229(void) {
	//OutputDebugString("Color CHAUDRON should not be in .MAP\r\n");
}

void Load230(void) {
	//OutputDebugString("Color AUTEL should not be in .MAP\r\n");
}

void Load231(void) {
	//OutputDebugString("Color 156 should not be in .MAP\r\n");
}

void Load232(void) {
	//OutputDebugString("Color DUNGEON_WELL should not be in .MAP\r\n");
}

void Load233(void) {
	//OutputDebugString("Color 283 should not be in .MAP\r\n");
}

void Load234(void) {
	//OutputDebugString("Color 2560 should not be in .MAP\r\n");
}

void Load235(void) {
	//OutputDebugString("Color 4005 should not be in .MAP\r\n");
}

void Load236(void) {
	//OutputDebugString("Color BIG_DUNGEON_WELL should not be in .MAP\r\n");
}

void Load237(void) {
	//OutputDebugString("Color 255 should not be in .MAP\r\n");
}

void Load238(void) {
	//OutputDebugString("Color DUNGEON_TOP should not be in .MAP\r\n");
}

void Load239(void) {
	//OutputDebugString("Color DUNGEONROCK_WALL should not be in .MAP\r\n");
}

void Load240(void) {
	//OutputDebugString("Color CAVERN_ENTRANCE should not be in .MAP\r\n");
}

void Load241(void) {
      World.DungeonWallSpec2[0].LoadSprite("DungeonMainArc 1");
      World.DungeonWallSpec2[1].LoadSprite("DungeonMainArc 2");
      World.DungeonWallSpec2[2].LoadSprite("DungeonMainArc 3");
      World.DungeonWallSpec2[3].LoadSprite("DungeonMainArc 4");
      World.DungeonWallSpec2[4].LoadSprite("DungeonMainArc 5");
      World.FlipDungeonWallSpec2[0].LoadSprite("DungeonDarkArc 1");
      World.FlipDungeonWallSpec2[1].LoadSprite("DungeonDarkArc 2");
      World.FlipDungeonWallSpec2[2].LoadSprite("DungeonDarkArc 3");
      World.FlipDungeonWallSpec2[3].LoadSprite("DungeonDarkArc 4");
      World.FlipDungeonWallSpec2[4].LoadSprite("DungeonDarkArc 5");
      World.DungeonWallSpec3[0].LoadSprite("DungeonMainArcTorch 1");
      World.DungeonWallSpec3[1].LoadSprite("DungeonMainArcTorch 2");
      World.DungeonWallSpec3[2].LoadSprite("DungeonMainArcTorch 3");
      World.DungeonWallSpec3[3].LoadSprite("DungeonMainArcTorch 4");
      World.DungeonWallSpec3[4].LoadSprite("DungeonMainArcTorch 5");
      World.FlipDungeonWallSpec3[0].LoadSprite("DungeonDarkArcTorch 1");
      World.FlipDungeonWallSpec3[1].LoadSprite("DungeonDarkArcTorch 2");
      World.FlipDungeonWallSpec3[2].LoadSprite("DungeonDarkArcTorch 3");
      World.FlipDungeonWallSpec3[3].LoadSprite("DungeonDarkArcTorch 4");
      World.FlipDungeonWallSpec3[4].LoadSprite("DungeonDarkArcTorch 5");
		
      World.DungeonRockWall[14].LoadSprite("64kDungeonRockArcEnd1 5");
      World.DungeonRockWall[15].LoadSprite("64kDungeonRockArcEnd1 4");
      World.DungeonRockWall[16].LoadSprite("64kDungeonRockArcEnd1 3");
      World.DungeonRockWall[17].LoadSprite("64kDungeonRockArcEnd1 2");
      World.DungeonRockWall[18].LoadSprite("64kDungeonRockArcEnd1 1");
      World.DungeonRockWall[19].LoadSprite("64kDungeonRockArcEnd2 1");
      World.DungeonRockWall[20].LoadSprite("64kDungeonRockArcEnd2 2");
      World.DungeonRockWall[21].LoadSprite("64kDungeonRockArcEnd2 3");
      World.DungeonRockWall[22].LoadSprite("64kDungeonRockArcEnd2 4");
      World.DungeonRockWall[23].LoadSprite("64kDungeonRockArcEnd2 5");
      World.DungeonRockWall[24].LoadSprite("64kDungeonRockArcDarkEnd1 1");
      World.DungeonRockWall[25].LoadSprite("64kDungeonRockArcDarkEnd1 2");
      World.DungeonRockWall[26].LoadSprite("64kDungeonRockArcDarkEnd1 3");
      World.DungeonRockWall[27].LoadSprite("64kDungeonRockArcDarkEnd1 4");
      World.DungeonRockWall[28].LoadSprite("64kDungeonRockArcDarkEnd1 5");
      World.DungeonRockWall[29].LoadSprite("64kDungeonRockArcDarkEnd2 5");
      World.DungeonRockWall[30].LoadSprite("64kDungeonRockArcDarkEnd2 4");
      World.DungeonRockWall[31].LoadSprite("64kDungeonRockArcDarkEnd2 3");
      World.DungeonRockWall[32].LoadSprite("64kDungeonRockArcDarkEnd2 2");
      World.DungeonRockWall[33].LoadSprite("64kDungeonRockArcDarkEnd2 1");
      World.DungeonRockWall[34].LoadSprite("64kDungeonRockWallEnd1");
      World.DungeonRockWall[35].LoadSprite("64kDungeonRockWallEnd2");
      World.DungeonRockWall[36].LoadSprite("64kDungeonRockWallEnd3");
      World.DungeonRockWall[37].LoadSprite("64kDungeonRockWallEnd4");
      World.DungeonRockWall[38].LoadSprite("64kDungeonRockWallEnd1");
      World.DungeonRockWall[39].LoadSprite("64kDungeonRockWallEnd2");
      World.DungeonRockWall[40].LoadSprite("64kDungeonRockWallEnd3");
      World.DungeonRockWall[41].LoadSprite("64kDungeonRockWallEnd4");

      World.DungeonRockWallSpec[0].LoadSprite("64kDungeonRockArcDark 2");
      World.DungeonRockWallSpec[1].LoadSprite("64kDungeonRockArcDark 3");
      World.DungeonRockWallSpec[2].LoadSprite("64kDungeonRockArcDark 4");
      World.DungeonRockWallSpec[3].LoadSprite("64kDungeonRockArcDark 5");
      World.DungeonRockWallSpec[4].LoadSprite("64kDungeonRockArcDark 1");
      World.FlipDungeonRockWallSpec[0].LoadSprite("64kDungeonRockArc 2");
      World.FlipDungeonRockWallSpec[1].LoadSprite("64kDungeonRockArc 3");
      World.FlipDungeonRockWallSpec[2].LoadSprite("64kDungeonRockArc 4");
      World.FlipDungeonRockWallSpec[3].LoadSprite("64kDungeonRockArc 5");
      World.FlipDungeonRockWallSpec[4].LoadSprite("64kDungeonRockArc 1");

		World.DungeonRockWall[0] .LoadSprite("64kDungeonDarkRockWall 2");
		World.DungeonRockWall[1] .LoadSprite("64kDungeonDarkRockWall 3");
		World.DungeonRockWall[2] .LoadSprite("64kDungeonDarkRockWall 4");
		World.DungeonRockWall[3] .LoadSprite("64kDungeonDarkRockWall 5");
		World.DungeonRockWall[4] .LoadSprite("64kDungeonDarkRockWall 1");
		World.DungeonRockWall[5] .LoadSprite("64kDungeonRockCornerV");
		World.DungeonRockWall[6] .LoadSprite("64kDungeonRockCornerN");
		World.DungeonRockWall[7] .LoadSprite("64kDungeonRockCorner");
      World.DungeonRockWall[8] .LoadSprite("64kDungeonRockCornerX");
      World.DungeonRockWall[9] .LoadSprite("64kDungeonRockCorner2"); 
      World.DungeonRockWall[10].LoadSprite("64kDungeonRockCornerNE");
      World.DungeonRockWall[11].LoadSprite("64kDungeonRockCornerNW");
      World.DungeonRockWall[12].LoadSprite("64kDungeonRockCornerSE");
      World.DungeonRockWall[13].LoadSprite("64kDungeonRockCornerSW");

      World.FlipDungeonRockWall[0].LoadSprite("64kDungeonRockWall 2");
      World.FlipDungeonRockWall[1].LoadSprite("64kDungeonRockWall 3");
      World.FlipDungeonRockWall[2].LoadSprite("64kDungeonRockWall 4");
      World.FlipDungeonRockWall[3].LoadSprite("64kDungeonRockWall 5");
      World.FlipDungeonRockWall[4].LoadSprite("64kDungeonRockWall 1");
}

void Load242(void) {
	LoadInit();
	LoadIsoSquare(World.DungeonRockFloor,		  "DungeonRockFloor", 7, 6);
}

void Load243(void) {
	//OutputDebugString("Color 284 should not be in .MAP\r\n");
}

void Load244(void) {
	World.CastleWall[0].LoadSprite("CastleWall 1");
   World.CastleWall[1].LoadSprite("CastleWall 2");
   World.CastleWall[2].LoadSprite("CastleWall 3");
   World.CastleWall[3].LoadSprite("CastleWall 4");
   World.CastleWall[4].LoadSprite("CastleWall 5");
   World.CastleWall[5].LoadSprite("CastleWallCornerV");
	World.CastleWall[6].LoadSprite("CastleWallCornerN");
	World.CastleWall[7].LoadSprite("CastleWallCorner");
   World.CastleWall[8].LoadSprite("CastleWallCornerX");
   World.CastleWall[9].LoadSprite("CastleWallCorner2"); 
   World.CastleWall[10].LoadSprite("CastleWallCornerNE");
   World.CastleWall[11].LoadSprite("CastleWallCornerNW");
   World.CastleWall[12].LoadSprite("CastleWallCornerSE");
   World.CastleWall[13].LoadSprite("CastleWallCornerSW");
   World.CastleWall[14].LoadSprite("CastleEnd1");
   World.CastleWall[15].LoadSprite("CastleEnd2");
   World.CastleWall[16].LoadSprite("CastleEnd3");
   World.CastleWall[17].LoadSprite("CastleEnd4");
   World.FlipCastleWall[0].LoadSprite("CastleDarkWall 1");
   World.FlipCastleWall[1].LoadSprite("CastleDarkWall 2");
   World.FlipCastleWall[2].LoadSprite("CastleDarkWall 3");
   World.FlipCastleWall[3].LoadSprite("CastleDarkWall 4");
   World.FlipCastleWall[4].LoadSprite("CastleDarkWall 5");
}	

void Load245(void) {
	//OutputDebugString("Color 153 should not be in .MAP\r\n");
}

void Load246(void) {
	//OutputDebugString("Color CASTLEWALL should not be in .MAP\r\n");
}

void Load247(void) {
	//OutputDebugString("Color 66 should not be in .MAP\r\n");
}

void Load248(void) {
	//OutputDebugString("Color 206 should not be in .MAP\r\n");
}

void Load249(void) {
	//OutputDebugString("Color 575 should not be in .MAP\r\n");
}

void Load250(void) {
	//OutputDebugString("Color 64 should not be in .MAP\r\n");
}

void Load251(void) {
	Load27();
	Load33(); // WOODEN FLOOR
	Load49();
}

void Load252(void) {
	Load22();
	Load33(); // WOODEN FLOOR
	Load49();
}

void Load253(void) {
	Load23();
	Load33(); // WOODEN FLOOR
	Load49();
}

void Load254(void) {
	Load14();
	Load33(); // WOODEN FLOOR
	Load49();
}

void Load255(void) {
	//OutputDebugString("Color 191 should not be in .MAP\r\n");
}

void Load256(void) {
	//OutputDebugString("Color 74 should not be in .MAP\r\n");
}

void Load257(void) {
	//OutputDebugString("Color ponts pas bon should not be in .MAP\r\n");
}

void Load258(void) {
	Load27();   // GRASS
	Load242();  // DUNGEONROCK_FLOOR
	Load48();   // Smooth1
}

void Load259(void) {
      World.Tree[39].LoadSprite("64kDeadForestTree1");
      World.Tree[40].LoadSprite("64kDeadForestTree2");
      World.Tree[41].LoadSprite("64kDeadForestTree3");
      World.Tree[42].LoadSprite("64kDeadForestTree4");
}

void Load260(void) {
      World.DungeonWallSpec2[0].LoadSprite("DungeonMainArc 1");
      World.DungeonWallSpec2[1].LoadSprite("DungeonMainArc 2");
      World.DungeonWallSpec2[2].LoadSprite("DungeonMainArc 3");
      World.DungeonWallSpec2[3].LoadSprite("DungeonMainArc 4");
      World.DungeonWallSpec2[4].LoadSprite("DungeonMainArc 5");
      World.FlipDungeonWallSpec2[0].LoadSprite("DungeonDarkArc 1");
      World.FlipDungeonWallSpec2[1].LoadSprite("DungeonDarkArc 2");
      World.FlipDungeonWallSpec2[2].LoadSprite("DungeonDarkArc 3");
      World.FlipDungeonWallSpec2[3].LoadSprite("DungeonDarkArc 4");
      World.FlipDungeonWallSpec2[4].LoadSprite("DungeonDarkArc 5");
      World.DungeonWallSpec3[0].LoadSprite("DungeonMainArcTorch 1");
      World.DungeonWallSpec3[1].LoadSprite("DungeonMainArcTorch 2");
      World.DungeonWallSpec3[2].LoadSprite("DungeonMainArcTorch 3");
      World.DungeonWallSpec3[3].LoadSprite("DungeonMainArcTorch 4");
      World.DungeonWallSpec3[4].LoadSprite("DungeonMainArcTorch 5");
      World.FlipDungeonWallSpec3[0].LoadSprite("DungeonDarkArcTorch 1");
      World.FlipDungeonWallSpec3[1].LoadSprite("DungeonDarkArcTorch 2");
      World.FlipDungeonWallSpec3[2].LoadSprite("DungeonDarkArcTorch 3");
      World.FlipDungeonWallSpec3[3].LoadSprite("DungeonDarkArcTorch 4");
      World.FlipDungeonWallSpec3[4].LoadSprite("DungeonDarkArcTorch 5");
		
      World.DungeonRockWall2[14].LoadSprite("64kDungeonRockArcEnd1 5", 0, 2);
      World.DungeonRockWall2[15].LoadSprite("64kDungeonRockArcEnd1 4", 0, 2);
      World.DungeonRockWall2[16].LoadSprite("64kDungeonRockArcEnd1 3", 0, 2);
      World.DungeonRockWall2[17].LoadSprite("64kDungeonRockArcEnd1 2", 0, 2);
      World.DungeonRockWall2[18].LoadSprite("64kDungeonRockArcEnd1 1", 0, 2);
      World.DungeonRockWall2[19].LoadSprite("64kDungeonRockArcEnd2 1", 0, 2);
      World.DungeonRockWall2[20].LoadSprite("64kDungeonRockArcEnd2 2", 0, 2);
      World.DungeonRockWall2[21].LoadSprite("64kDungeonRockArcEnd2 3", 0, 2);
      World.DungeonRockWall2[22].LoadSprite("64kDungeonRockArcEnd2 4", 0, 2);
      World.DungeonRockWall2[23].LoadSprite("64kDungeonRockArcEnd2 5", 0, 2);
      World.DungeonRockWall2[24].LoadSprite("64kDungeonRockArcDarkEnd1 1", 0, 2);
      World.DungeonRockWall2[25].LoadSprite("64kDungeonRockArcDarkEnd1 2", 0, 2);
      World.DungeonRockWall2[26].LoadSprite("64kDungeonRockArcDarkEnd1 3", 0, 2);
      World.DungeonRockWall2[27].LoadSprite("64kDungeonRockArcDarkEnd1 4", 0, 2);
      World.DungeonRockWall2[28].LoadSprite("64kDungeonRockArcDarkEnd1 5", 0, 2);
      World.DungeonRockWall2[29].LoadSprite("64kDungeonRockArcDarkEnd2 5", 0, 2);
      World.DungeonRockWall2[30].LoadSprite("64kDungeonRockArcDarkEnd2 4", 0, 2);
      World.DungeonRockWall2[31].LoadSprite("64kDungeonRockArcDarkEnd2 3", 0, 2);
      World.DungeonRockWall2[32].LoadSprite("64kDungeonRockArcDarkEnd2 2", 0, 2);
      World.DungeonRockWall2[33].LoadSprite("64kDungeonRockArcDarkEnd2 1", 0, 2);
      World.DungeonRockWall2[34].LoadSprite("64kDungeonRockWallEnd1", 0, 2);
      World.DungeonRockWall2[35].LoadSprite("64kDungeonRockWallEnd2", 0, 2);
      World.DungeonRockWall2[36].LoadSprite("64kDungeonRockWallEnd3", 0, 2);
      World.DungeonRockWall2[37].LoadSprite("64kDungeonRockWallEnd4", 0, 2);
      World.DungeonRockWall2[38].LoadSprite("64kDungeonRockWallEnd1", 0, 2);
      World.DungeonRockWall2[39].LoadSprite("64kDungeonRockWallEnd2", 0, 2);
      World.DungeonRockWall2[40].LoadSprite("64kDungeonRockWallEnd3", 0, 2);
      World.DungeonRockWall2[41].LoadSprite("64kDungeonRockWallEnd4", 0, 2);

      World.DungeonRockWall2Spec[0].LoadSprite("64kDungeonRockArcDark 2", 0, 2);
      World.DungeonRockWall2Spec[1].LoadSprite("64kDungeonRockArcDark 3", 0, 2);
      World.DungeonRockWall2Spec[2].LoadSprite("64kDungeonRockArcDark 4", 0, 2);
      World.DungeonRockWall2Spec[3].LoadSprite("64kDungeonRockArcDark 5", 0, 2);
      World.DungeonRockWall2Spec[4].LoadSprite("64kDungeonRockArcDark 1", 0, 2);
      World.FlipDungeonRockWall2Spec[0].LoadSprite("64kDungeonRockArc 2", 0, 2);
      World.FlipDungeonRockWall2Spec[1].LoadSprite("64kDungeonRockArc 3", 0, 2);
      World.FlipDungeonRockWall2Spec[2].LoadSprite("64kDungeonRockArc 4", 0, 2);
      World.FlipDungeonRockWall2Spec[3].LoadSprite("64kDungeonRockArc 5", 0, 2);
      World.FlipDungeonRockWall2Spec[4].LoadSprite("64kDungeonRockArc 1", 0, 2);

		World.DungeonRockWall2[0] .LoadSprite("64kDungeonDarkRockWall 2", 0, 2);
		World.DungeonRockWall2[1] .LoadSprite("64kDungeonDarkRockWall 3", 0, 2);
		World.DungeonRockWall2[2] .LoadSprite("64kDungeonDarkRockWall 4", 0, 2);
		World.DungeonRockWall2[3] .LoadSprite("64kDungeonDarkRockWall 5", 0, 2);
		World.DungeonRockWall2[4] .LoadSprite("64kDungeonDarkRockWall 1", 0, 2);
		World.DungeonRockWall2[5] .LoadSprite("64kDungeonRockCornerV", 0, 2);
		World.DungeonRockWall2[6] .LoadSprite("64kDungeonRockCornerN", 0, 2);
		World.DungeonRockWall2[7] .LoadSprite("64kDungeonRockCorner", 0, 2);
      World.DungeonRockWall2[8] .LoadSprite("64kDungeonRockCornerX", 0, 2);
      World.DungeonRockWall2[9] .LoadSprite("64kDungeonRockCorner2", 0, 2); 
      World.DungeonRockWall2[10].LoadSprite("64kDungeonRockCornerNE", 0, 2);
      World.DungeonRockWall2[11].LoadSprite("64kDungeonRockCornerNW", 0, 2);
      World.DungeonRockWall2[12].LoadSprite("64kDungeonRockCornerSE", 0, 2);
      World.DungeonRockWall2[13].LoadSprite("64kDungeonRockCornerSW", 0, 2);

      World.FlipDungeonRockWall2[0].LoadSprite("64kDungeonRockWall 2", 0, 2);
      World.FlipDungeonRockWall2[1].LoadSprite("64kDungeonRockWall 3", 0, 2);
      World.FlipDungeonRockWall2[2].LoadSprite("64kDungeonRockWall 4", 0, 2);
      World.FlipDungeonRockWall2[3].LoadSprite("64kDungeonRockWall 5", 0, 2);
      World.FlipDungeonRockWall2[4].LoadSprite("64kDungeonRockWall 1", 0, 2);
}

void Load261(void) {
      World.DungeonWallSpec2[0].LoadSprite("DungeonMainArc 1");
      World.DungeonWallSpec2[1].LoadSprite("DungeonMainArc 2");
      World.DungeonWallSpec2[2].LoadSprite("DungeonMainArc 3");
      World.DungeonWallSpec2[3].LoadSprite("DungeonMainArc 4");
      World.DungeonWallSpec2[4].LoadSprite("DungeonMainArc 5");
      World.FlipDungeonWallSpec2[0].LoadSprite("DungeonDarkArc 1");
      World.FlipDungeonWallSpec2[1].LoadSprite("DungeonDarkArc 2");
      World.FlipDungeonWallSpec2[2].LoadSprite("DungeonDarkArc 3");
      World.FlipDungeonWallSpec2[3].LoadSprite("DungeonDarkArc 4");
      World.FlipDungeonWallSpec2[4].LoadSprite("DungeonDarkArc 5");
      World.DungeonWallSpec3[0].LoadSprite("DungeonMainArcTorch 1");
      World.DungeonWallSpec3[1].LoadSprite("DungeonMainArcTorch 2");
      World.DungeonWallSpec3[2].LoadSprite("DungeonMainArcTorch 3");
      World.DungeonWallSpec3[3].LoadSprite("DungeonMainArcTorch 4");
      World.DungeonWallSpec3[4].LoadSprite("DungeonMainArcTorch 5");
      World.FlipDungeonWallSpec3[0].LoadSprite("DungeonDarkArcTorch 1");
      World.FlipDungeonWallSpec3[1].LoadSprite("DungeonDarkArcTorch 2");
      World.FlipDungeonWallSpec3[2].LoadSprite("DungeonDarkArcTorch 3");
      World.FlipDungeonWallSpec3[3].LoadSprite("DungeonDarkArcTorch 4");
      World.FlipDungeonWallSpec3[4].LoadSprite("DungeonDarkArcTorch 5");
		
      World.DungeonRockWall3[14].LoadSprite("64kDungeonRockArcEnd1 5", 0, 3);
      World.DungeonRockWall3[15].LoadSprite("64kDungeonRockArcEnd1 4", 0, 3);
      World.DungeonRockWall3[16].LoadSprite("64kDungeonRockArcEnd1 3", 0, 3);
      World.DungeonRockWall3[17].LoadSprite("64kDungeonRockArcEnd1 2", 0, 3);
      World.DungeonRockWall3[18].LoadSprite("64kDungeonRockArcEnd1 1", 0, 3);
      World.DungeonRockWall3[19].LoadSprite("64kDungeonRockArcEnd2 1", 0, 3);
      World.DungeonRockWall3[20].LoadSprite("64kDungeonRockArcEnd2 2", 0, 3);
      World.DungeonRockWall3[21].LoadSprite("64kDungeonRockArcEnd2 3", 0, 3);
      World.DungeonRockWall3[22].LoadSprite("64kDungeonRockArcEnd2 4", 0, 3);
      World.DungeonRockWall3[23].LoadSprite("64kDungeonRockArcEnd2 5", 0, 3);
      World.DungeonRockWall3[24].LoadSprite("64kDungeonRockArcDarkEnd1 1", 0, 3);
      World.DungeonRockWall3[25].LoadSprite("64kDungeonRockArcDarkEnd1 2", 0, 3);
      World.DungeonRockWall3[26].LoadSprite("64kDungeonRockArcDarkEnd1 3", 0, 3);
      World.DungeonRockWall3[27].LoadSprite("64kDungeonRockArcDarkEnd1 4", 0, 3);
      World.DungeonRockWall3[28].LoadSprite("64kDungeonRockArcDarkEnd1 5", 0, 3);
      World.DungeonRockWall3[29].LoadSprite("64kDungeonRockArcDarkEnd2 5", 0, 3);
      World.DungeonRockWall3[30].LoadSprite("64kDungeonRockArcDarkEnd2 4", 0, 3);
      World.DungeonRockWall3[31].LoadSprite("64kDungeonRockArcDarkEnd2 3", 0, 3);
      World.DungeonRockWall3[32].LoadSprite("64kDungeonRockArcDarkEnd2 2", 0, 3);
      World.DungeonRockWall3[33].LoadSprite("64kDungeonRockArcDarkEnd2 1", 0, 3);
      World.DungeonRockWall3[34].LoadSprite("64kDungeonRockWallEnd1", 0, 3);
      World.DungeonRockWall3[35].LoadSprite("64kDungeonRockWallEnd2", 0, 3);
      World.DungeonRockWall3[36].LoadSprite("64kDungeonRockWallEnd3", 0, 3);
      World.DungeonRockWall3[37].LoadSprite("64kDungeonRockWallEnd4", 0, 3);
      World.DungeonRockWall3[38].LoadSprite("64kDungeonRockWallEnd1", 0, 3);
      World.DungeonRockWall3[39].LoadSprite("64kDungeonRockWallEnd2", 0, 3);
      World.DungeonRockWall3[40].LoadSprite("64kDungeonRockWallEnd3", 0, 3);
      World.DungeonRockWall3[41].LoadSprite("64kDungeonRockWallEnd4", 0, 3);

      World.DungeonRockWall3Spec[0].LoadSprite("64kDungeonRockArcDark 2", 0, 3);
      World.DungeonRockWall3Spec[1].LoadSprite("64kDungeonRockArcDark 3", 0, 3);
      World.DungeonRockWall3Spec[2].LoadSprite("64kDungeonRockArcDark 4", 0, 3);
      World.DungeonRockWall3Spec[3].LoadSprite("64kDungeonRockArcDark 5", 0, 3);
      World.DungeonRockWall3Spec[4].LoadSprite("64kDungeonRockArcDark 1", 0, 3);
      World.FlipDungeonRockWall3Spec[0].LoadSprite("64kDungeonRockArc 2", 0, 3);
      World.FlipDungeonRockWall3Spec[1].LoadSprite("64kDungeonRockArc 3", 0, 3);
      World.FlipDungeonRockWall3Spec[2].LoadSprite("64kDungeonRockArc 4", 0, 3);
      World.FlipDungeonRockWall3Spec[3].LoadSprite("64kDungeonRockArc 5", 0, 3);
      World.FlipDungeonRockWall3Spec[4].LoadSprite("64kDungeonRockArc 1", 0, 3);

		World.DungeonRockWall3[0] .LoadSprite("64kDungeonDarkRockWall 2", 0, 3);
		World.DungeonRockWall3[1] .LoadSprite("64kDungeonDarkRockWall 3", 0, 3);
		World.DungeonRockWall3[2] .LoadSprite("64kDungeonDarkRockWall 4", 0, 3);
		World.DungeonRockWall3[3] .LoadSprite("64kDungeonDarkRockWall 5", 0, 3);
		World.DungeonRockWall3[4] .LoadSprite("64kDungeonDarkRockWall 1", 0, 3);
		World.DungeonRockWall3[5] .LoadSprite("64kDungeonRockCornerV", 0, 3);
		World.DungeonRockWall3[6] .LoadSprite("64kDungeonRockCornerN", 0, 3);
		World.DungeonRockWall3[7] .LoadSprite("64kDungeonRockCorner", 0, 3);
      World.DungeonRockWall3[8] .LoadSprite("64kDungeonRockCornerX", 0, 3);
      World.DungeonRockWall3[9] .LoadSprite("64kDungeonRockCorner2", 0, 3); 
      World.DungeonRockWall3[10].LoadSprite("64kDungeonRockCornerNE", 0, 3);
      World.DungeonRockWall3[11].LoadSprite("64kDungeonRockCornerNW", 0, 3);
      World.DungeonRockWall3[12].LoadSprite("64kDungeonRockCornerSE", 0, 3);
      World.DungeonRockWall3[13].LoadSprite("64kDungeonRockCornerSW", 0, 3);

      World.FlipDungeonRockWall3[0].LoadSprite("64kDungeonRockWall 2", 0, 3);
      World.FlipDungeonRockWall3[1].LoadSprite("64kDungeonRockWall 3", 0, 3);
      World.FlipDungeonRockWall3[2].LoadSprite("64kDungeonRockWall 4", 0, 3);
      World.FlipDungeonRockWall3[3].LoadSprite("64kDungeonRockWall 5", 0, 3);
      World.FlipDungeonRockWall3[4].LoadSprite("64kDungeonRockWall 1", 0, 3);
}

void Load262(void) {
      World.DungeonWallSpec2[0].LoadSprite("DungeonMainArc 1");
      World.DungeonWallSpec2[1].LoadSprite("DungeonMainArc 2");
      World.DungeonWallSpec2[2].LoadSprite("DungeonMainArc 3");
      World.DungeonWallSpec2[3].LoadSprite("DungeonMainArc 4");
      World.DungeonWallSpec2[4].LoadSprite("DungeonMainArc 5");
      World.FlipDungeonWallSpec2[0].LoadSprite("DungeonDarkArc 1");
      World.FlipDungeonWallSpec2[1].LoadSprite("DungeonDarkArc 2");
      World.FlipDungeonWallSpec2[2].LoadSprite("DungeonDarkArc 3");
      World.FlipDungeonWallSpec2[3].LoadSprite("DungeonDarkArc 4");
      World.FlipDungeonWallSpec2[4].LoadSprite("DungeonDarkArc 5");
      World.DungeonWallSpec3[0].LoadSprite("DungeonMainArcTorch 1");
      World.DungeonWallSpec3[1].LoadSprite("DungeonMainArcTorch 2");
      World.DungeonWallSpec3[2].LoadSprite("DungeonMainArcTorch 3");
      World.DungeonWallSpec3[3].LoadSprite("DungeonMainArcTorch 4");
      World.DungeonWallSpec3[4].LoadSprite("DungeonMainArcTorch 5");
      World.FlipDungeonWallSpec3[0].LoadSprite("DungeonDarkArcTorch 1");
      World.FlipDungeonWallSpec3[1].LoadSprite("DungeonDarkArcTorch 2");
      World.FlipDungeonWallSpec3[2].LoadSprite("DungeonDarkArcTorch 3");
      World.FlipDungeonWallSpec3[3].LoadSprite("DungeonDarkArcTorch 4");
      World.FlipDungeonWallSpec3[4].LoadSprite("DungeonDarkArcTorch 5");
		
      World.DungeonRockWall4[14].LoadSprite("64kDungeonRockArcEnd1 5", 0, 4);
      World.DungeonRockWall4[15].LoadSprite("64kDungeonRockArcEnd1 4", 0, 4);
      World.DungeonRockWall4[16].LoadSprite("64kDungeonRockArcEnd1 3", 0, 4);
      World.DungeonRockWall4[17].LoadSprite("64kDungeonRockArcEnd1 2", 0, 4);
      World.DungeonRockWall4[18].LoadSprite("64kDungeonRockArcEnd1 1", 0, 4);
      World.DungeonRockWall4[19].LoadSprite("64kDungeonRockArcEnd2 1", 0, 4);
      World.DungeonRockWall4[20].LoadSprite("64kDungeonRockArcEnd2 2", 0, 4);
      World.DungeonRockWall4[21].LoadSprite("64kDungeonRockArcEnd2 3", 0, 4);
      World.DungeonRockWall4[22].LoadSprite("64kDungeonRockArcEnd2 4", 0, 4);
      World.DungeonRockWall4[23].LoadSprite("64kDungeonRockArcEnd2 5", 0, 4);
      World.DungeonRockWall4[24].LoadSprite("64kDungeonRockArcDarkEnd1 1", 0, 4);
      World.DungeonRockWall4[25].LoadSprite("64kDungeonRockArcDarkEnd1 2", 0, 4);
      World.DungeonRockWall4[26].LoadSprite("64kDungeonRockArcDarkEnd1 3", 0, 4);
      World.DungeonRockWall4[27].LoadSprite("64kDungeonRockArcDarkEnd1 4", 0, 4);
      World.DungeonRockWall4[28].LoadSprite("64kDungeonRockArcDarkEnd1 5", 0, 4);
      World.DungeonRockWall4[29].LoadSprite("64kDungeonRockArcDarkEnd2 5", 0, 4);
      World.DungeonRockWall4[30].LoadSprite("64kDungeonRockArcDarkEnd2 4", 0, 4);
      World.DungeonRockWall4[31].LoadSprite("64kDungeonRockArcDarkEnd2 3", 0, 4);
      World.DungeonRockWall4[32].LoadSprite("64kDungeonRockArcDarkEnd2 2", 0, 4);
      World.DungeonRockWall4[33].LoadSprite("64kDungeonRockArcDarkEnd2 1", 0, 4);
      World.DungeonRockWall4[34].LoadSprite("64kDungeonRockWallEnd1", 0, 4);
      World.DungeonRockWall4[35].LoadSprite("64kDungeonRockWallEnd2", 0, 4);
      World.DungeonRockWall4[36].LoadSprite("64kDungeonRockWallEnd3", 0, 4);
      World.DungeonRockWall4[37].LoadSprite("64kDungeonRockWallEnd4", 0, 4);
      World.DungeonRockWall4[38].LoadSprite("64kDungeonRockWallEnd1", 0, 4);
      World.DungeonRockWall4[39].LoadSprite("64kDungeonRockWallEnd2", 0, 4);
      World.DungeonRockWall4[40].LoadSprite("64kDungeonRockWallEnd3", 0, 4);
      World.DungeonRockWall4[41].LoadSprite("64kDungeonRockWallEnd4", 0, 4);

      World.DungeonRockWall4Spec[0].LoadSprite("64kDungeonRockArcDark 2", 0, 4);
      World.DungeonRockWall4Spec[1].LoadSprite("64kDungeonRockArcDark 3", 0, 4);
      World.DungeonRockWall4Spec[2].LoadSprite("64kDungeonRockArcDark 4", 0, 4);
      World.DungeonRockWall4Spec[3].LoadSprite("64kDungeonRockArcDark 5", 0, 4);
      World.DungeonRockWall4Spec[4].LoadSprite("64kDungeonRockArcDark 1", 0, 4);
      World.FlipDungeonRockWall4Spec[0].LoadSprite("64kDungeonRockArc 2", 0, 4);
      World.FlipDungeonRockWall4Spec[1].LoadSprite("64kDungeonRockArc 3", 0, 4);
      World.FlipDungeonRockWall4Spec[2].LoadSprite("64kDungeonRockArc 4", 0, 4);
      World.FlipDungeonRockWall4Spec[3].LoadSprite("64kDungeonRockArc 5", 0, 4);
      World.FlipDungeonRockWall4Spec[4].LoadSprite("64kDungeonRockArc 1", 0, 4);

		World.DungeonRockWall4[0] .LoadSprite("64kDungeonDarkRockWall 2", 0, 4);
		World.DungeonRockWall4[1] .LoadSprite("64kDungeonDarkRockWall 3", 0, 4);
		World.DungeonRockWall4[2] .LoadSprite("64kDungeonDarkRockWall 4", 0, 4);
		World.DungeonRockWall4[3] .LoadSprite("64kDungeonDarkRockWall 5", 0, 4);
		World.DungeonRockWall4[4] .LoadSprite("64kDungeonDarkRockWall 1", 0, 4);
		World.DungeonRockWall4[5] .LoadSprite("64kDungeonRockCornerV", 0, 4);
		World.DungeonRockWall4[6] .LoadSprite("64kDungeonRockCornerN", 0, 4);
		World.DungeonRockWall4[7] .LoadSprite("64kDungeonRockCorner", 0, 4);
      World.DungeonRockWall4[8] .LoadSprite("64kDungeonRockCornerX", 0, 4);
      World.DungeonRockWall4[9] .LoadSprite("64kDungeonRockCorner2", 0, 4); 
      World.DungeonRockWall4[10].LoadSprite("64kDungeonRockCornerNE", 0, 4);
      World.DungeonRockWall4[11].LoadSprite("64kDungeonRockCornerNW", 0, 4);
      World.DungeonRockWall4[12].LoadSprite("64kDungeonRockCornerSE", 0, 4);
      World.DungeonRockWall4[13].LoadSprite("64kDungeonRockCornerSW", 0, 4);

      World.FlipDungeonRockWall4[0].LoadSprite("64kDungeonRockWall 2", 0, 4);
      World.FlipDungeonRockWall4[1].LoadSprite("64kDungeonRockWall 3", 0, 4);
      World.FlipDungeonRockWall4[2].LoadSprite("64kDungeonRockWall 4", 0, 4);
      World.FlipDungeonRockWall4[3].LoadSprite("64kDungeonRockWall 5", 0, 4);
      World.FlipDungeonRockWall4[4].LoadSprite("64kDungeonRockWall 1", 0, 4);
}

void Load263(void) {
	//OutputDebugString("Color 175 should not be in .MAP\r\n");
}

void Load264(void) {
	Load27();   // GRASS
	Load266();   // Smooth3
		Load24(); // Pavement
}

void Load265(void) {
	Load2();   // Mountain
	Load266();   // Smooth3
		Load24(); // Pavement
}

void Load266(void) {
		World.Smoothing3[0].LoadSprite("tmpl4 2");
      World.Smoothing3[1].LoadSprite("tmpl4 1");
      World.Smoothing3[2].LoadSprite("tmpl4 4");
      World.Smoothing3[3].LoadSprite("tmpl4 3"); // ok
}

void Load267(void) {
		World.GraRoc[0].LoadSprite("Town Road 4");
		World.GraRoc[1].LoadSprite("Town Road 2");
		World.GraRoc[2].LoadSprite("Town Road 1");
		World.GraRoc[3].LoadSprite("Town Road 3");
}

void Load269(void) {
   World.Troll[0].LoadSprite("64kTrollBed");
   World.Troll[1].LoadSprite("64kTrollRoundTable");
   World.Troll[2].LoadSprite("64kTrollTable");
   World.Troll[3].LoadSprite("64kTrollChair1");
   World.Troll[4].LoadSprite("64kTrollChair2");
}

void Load270(void) {
	Load27();   // GRASS
	Load266();   // Smooth3
	Load26(); // Rock2
}

void Load271(void) {
	Load27();   // GRASS
	Load32();  // CAVERN
	Load48();   // Smooth1
}

void Load272(void) {
   Load21(); // Swamp
	Load31(); // water
	Load49(); // Smooth2 (pour water)
}

void Load273(void) {
   Load21(); // Swamp
	Load2(); // mountain
	Load48(); // Smooth 1 (pour autre)
}

void Load274(void) {
   Load21(); // Swamp
	Load23(); // chemin
	Load48(); // Smooth 1 (pour autre)
}

void Load275(void) {
   Load21(); // Swamp
	Load27(); // grass
	Load48(); // Smooth 1 (pour autre)
}

void Load276(void) {
   Load21(); // Swamp
	Load22(); // forest
	Load48(); // Smooth 1 (pour autre)
}

void Load277(void) {
   Load21(); // Swamp
	Load14(); // desert
	Load48(); // Smooth 1 (pour autre)
}

void Load278(void) {
		World.RuinedWhiteWall[0] .LoadSprite("StuccoDarkSmallWallB");
		World.RuinedWhiteWall[1] .LoadSprite("DungeonDarkArcTorch 5");
		World.RuinedWhiteWall[2] .LoadSprite("DungeonDarkArcTorch 5");
		World.RuinedWhiteWall[3] .LoadSprite("StuccoCornerN");
		World.RuinedWhiteWall[4] .LoadSprite("StuccoCorner");
		World.RuinedWhiteWall[5] .LoadSprite("StuccoCornerV");
		World.RuinedWhiteWall[6] .LoadSprite("StuccoCornerTNE");
		World.RuinedWhiteWall[7] .LoadSprite("StuccoCornerTSE");
		World.RuinedWhiteWall[8] .LoadSprite("StuccoCornerX");
		World.RuinedWhiteWall[9] .LoadSprite("StuccoMainWindowA");
		World.RuinedWhiteWall[10].LoadSprite("StuccoDarkSmallWallA");
		World.RuinedWhiteWall[11].LoadSprite("StuccoDarkWall 2");
		World.RuinedWhiteWall[12].LoadSprite("StuccoDarkWall 3");
        World.RuinedWhiteWall[13].LoadSprite("StuccoMainSmallWallB");
		World.RuinedWhiteWall[14].LoadSprite("StuccoMainWall 2");
		World.RuinedWhiteWall[15].LoadSprite("StuccoMainWall 1");
		World.RuinedWhiteWall[16].LoadSprite("StuccoCorner2");
		World.RuinedWhiteWall[17].LoadSprite("StuccoMainSmallWallA");
        World.RuinedWhiteWall[18].LoadSprite("StuccoCornerTNW");
        World.RuinedWhiteWall[19].LoadSprite("StuccoCornerTSW");
        World.RuinedWhiteWall[20].LoadSprite("StuccoMainSmallWallC");
        World.RuinedWhiteWall[21].LoadSprite("StuccoDarkSmallWallC");
		World.RuinedWhiteWall[22].LoadSprite("StuccoDarkWindowA");
		World.RuinedWhiteWall[23].LoadSprite("StuccoMainWindowB");
		World.RuinedWhiteWall[24].LoadSprite("StuccoDarkWindowB");
		World.RuinedWhiteWall[25].LoadSprite("StuccoMainWindowC");
		World.RuinedWhiteWall[26].LoadSprite("StuccoDarkWindowC");
        World.RuinedWhiteWall[27].LoadSprite("StuccoDarkWallDoor 1");
        World.RuinedWhiteWall[28].LoadSprite("StuccoDarkWallDoor 2");
        World.RuinedWhiteWall[29].LoadSprite("StuccoMainWallDoor 2");
        World.RuinedWhiteWall[30].LoadSprite("StuccoMainWallDoor 1");
		World.RuinedWhiteWall[31].LoadSprite("Ruined_StuccoMainWall 1");
		World.RuinedWhiteWall[32].LoadSprite("Ruined_StuccoMainWall 2");
		World.RuinedWhiteWall[33].LoadSprite("Ruined_StuccoMainWall 3");
		World.RuinedWhiteWall[34].LoadSprite("Ruined_StuccoDarkWall 1");
		World.RuinedWhiteWall[35].LoadSprite("Ruined_StuccoDarkWall 2");
		World.RuinedWhiteWall[36].LoadSprite("Ruined_StuccoDarkWall 3");
		World.RuinedWhiteWall[37].LoadSprite("Ruined_StuccoMainWindow 1");
		World.RuinedWhiteWall[38].LoadSprite("Ruined_StuccoMainWindow 2");
		World.RuinedWhiteWall[39].LoadSprite("Ruined_StuccoMainWindow 3");
		World.RuinedWhiteWall[40].LoadSprite("Ruined_StuccoDarkWindow 1");
		World.RuinedWhiteWall[41].LoadSprite("Ruined_StuccoDarkWindow 2");
		World.RuinedWhiteWall[42].LoadSprite("Ruined_StuccoDarkWindow 3");
}

void Load279(void) {
		World.RuinedBrickWall[0] .LoadSprite("BrickDarkWall 1");
		World.RuinedBrickWall[1] .LoadSprite("Ruined_BrickDarkWall 1");
		World.RuinedBrickWall[2] .LoadSprite("Ruined_BrickDarkWall 2");
		World.RuinedBrickWall[3] .LoadSprite("BrickWallCornerN");
		World.RuinedBrickWall[4] .LoadSprite("BrickWallCorner");
		World.RuinedBrickWall[5] .LoadSprite("BrickWallCornerV");
		World.RuinedBrickWall[6] .LoadSprite("BrickWallCornerTNE");
		World.RuinedBrickWall[7] .LoadSprite("BrickWallCornerTSE");
		World.RuinedBrickWall[8] .LoadSprite("BrickWallCornerX");
		World.RuinedBrickWall[9] .LoadSprite("Ruined_BrickDarkWall 3");
		World.RuinedBrickWall[10].LoadSprite("BrickDarkWall 2");  
		World.RuinedBrickWall[11].LoadSprite("Ruined_BrickDarkWallWindow 1");
		World.RuinedBrickWall[12].LoadSprite("Ruined_BrickDarkWallWindow 2");
      World.RuinedBrickWall[13].LoadSprite("BrickWall 1");
		World.RuinedBrickWall[14].LoadSprite("Ruined_BrickDarkWallWindow 3");
		World.RuinedBrickWall[16].LoadSprite("BrickWallCorner2");
		World.RuinedBrickWall[17].LoadSprite("BrickWall 2");
      World.RuinedBrickWall[18].LoadSprite("BrickWallCornerTNW");
      World.RuinedBrickWall[19].LoadSprite("BrickWallCornerTSW");
      World.RuinedBrickWall[20].LoadSprite("BrickWall 3"); 
      World.RuinedBrickWall[21].LoadSprite("BrickDarkWall 3");
		World.RuinedBrickWall[22].LoadSprite("Ruined_BrickWall 1");
		World.RuinedBrickWall[23].LoadSprite("Ruined_BrickWall 2");
		World.RuinedBrickWall[24].LoadSprite("Ruined_BrickWall 3");
		World.RuinedBrickWall[25].LoadSprite("Ruined_BrickWallWindow 1");
		World.RuinedBrickWall[26].LoadSprite("Ruined_BrickWallWindow 2");
      World.RuinedBrickWall[27].LoadSprite("BrickDarkWallDoor 1");
      World.RuinedBrickWall[28].LoadSprite("BrickDarkWallDoor 2");
      World.RuinedBrickWall[29].LoadSprite("BrickWallDoor 2");
      World.RuinedBrickWall[30].LoadSprite("BrickWallDoor 1");
		World.RuinedBrickWall[15].LoadSprite("Ruined_BrickWallWindow 3");
}

void Load280( void ) {
	World.Object[166].LoadSprite( "Object_LightHaven_Crypt" );
}

void Load281( void ) 
{
	World.Object[167].LoadSprite( "Object_LightHaven_Fountain" );
}

void Load282( void ) {
	World.Object[168].LoadSprite( "Object_CatapultBoulder_01" );
}

void Load283( void ) {
	World.Object[169].LoadSprite( "Object_CatapultBoulder_02" );
}

void Load284( void ) {
	World.Object[170].LoadSprite( "DungeonGreen_Column" );
}

void Load285( void ) {
	World.Object[171].LoadSprite( "DungeonBlue_Column" );
}

void Load286( void ) {
	World.Object[172].LoadSprite( "DungeonBrown_Column" );
}

void Load287( void ) {
	World.Object[173].LoadSprite( "Balista" );
}

void Load288( void ) {
	World.Object[174].LoadSprite( "BalistaF" );
}

void Load289( void ) {
	World.Object[175].LoadSprite( "Catapult" );
}

void Load290( void ) {
	World.Object[176].LoadSprite( "Trebuchet" );
}

void Load291( void ) {
	World.Object[177].LoadSprite( "Trebuchet2" );
}

void Load292 ( ) 
{
	LoadInit();
	LoadIsoSquare(World.FloorLava, "Lava", 4, 4);
}

void Load293 ( )
{
	World.Object[178].LoadSprite( "WoodPrison" );
}

void Load294 ( )
{
	World.Object[179].LoadSprite( "CleanPentacle" );
}

void Load295 ( )
{
	World.Object[180].LoadSprite( "DarkPentacle" );
}

void Load296 ( )
{
	World.Object[181].LoadSprite( "OldPentacle" );
}

void Load297 ( )
{
	World.Object[182].LoadSprite( "TombOfDarkness" );
}

void Load298 ( )
{
	LoadInit();
	LoadIsoSquare(World.FloorIce, "Ice", 10, 9);
}

void Load299 ( ) 
{  
	World.Object[183].LoadSprite( "Shito 6" ); 
}
void Load300 ( ) 
{  
	World.Object[184].LoadSprite( "Crypt2" ); 
}
void Load301 ( ) 
{  
	World.Object[185].LoadSprite( "Forge1" ); 
}
void Load302 ( ) 
{  
	World.Object[186].LoadSprite( "HorseWater" ); 
}
void Load303 ( ) 
{  
	World.Object[187].LoadSprite( "HuteGob1" ); 
}
void Load304 ( ) 
{  
	World.Object[188].LoadSprite( "BarBar" ); 
}

void Load305 ( ) 
{  
	World.Object[189].LoadSprite( "Orgue_Small" ); 
}
void Load306 ( ) 
{  
	World.Object[190].LoadSprite( "TableForge1" ); 
}
void Load307 ( ) 
{  
	World.Object[191].LoadSprite( "TableTemple1" ); 
}
void Load308 ( ) 
{  
	World.Object[192].LoadSprite( "Torture1" ); 
}
void Load309 ( ) 
{  
	World.Object[193].LoadSprite( "Torture2" ); 
}
void Load310 ( ) 
{  
	World.Object[194].LoadSprite( "Torture3" ); 
}
void Load311 ( ) 
{  
	World.Object[195].LoadSprite( "Torture4" ); 
}
void Load312 ( ) 
{  
	World.Object[196].LoadSprite( "Torture5" ); 
}
void Load313 ( ) 
{  
	World.Object[197].LoadSprite( "Torture6" ); 
}
void Load314 ( ) 
{  
	World.Object[198].LoadSprite( "Trone1King" ); 
}
void Load315 ( ) 
{  
	World.Object[199].LoadSprite( "Trone2King" ); 
}
void Load316 ( ) 
{  
	World.Object[200].LoadSprite( "Blockfleches" ); 
}
void Load317 ( ) 
{  
	World.Object[201].LoadSprite( "Entreemine" ); 
}
void Load318 ( ) 
{  
	World.Object[202].LoadSprite( "Pontdepierre" ); 
}
void Load319 ( ) 
{  
	World.Object[203].LoadSprite( "Pontdepierretop" ); 
}
void Load320 ( ) 
{  
	World.Object[204].LoadSprite( "Tente1" ); 
}
void Load321 ( ) 
{  
	World.Object[205].LoadSprite( "Tente2" ); 
}
void Load322 ( ) 
{  
	World.Object[206].LoadSprite( "Tente3" ); 
}
void Load323(void)
{
   World.Object[207].LoadSprite("LanterneSimple");
}
void Load324(void)
{
   World.Object[208].LoadSprite("LH_Bridge");
}

void Load325(void)
{
   World.Object[209].LoadSprite("LH_BridgeB");
}

void Load326(void)
{
   World.Object[237].LoadSprite("FBridgeCastleTop");
   World.Object[238].LoadSprite("BridgeCastleTop1");
   World.Object[241].LoadSprite("BridgeCastleTop2");
}

void Load327(void)
{
   World.EscalierObject[0].LoadSprite("Esc_Escalier");
   World.EscalierObject[1].LoadSprite("Esc_CtrEscalier");
   World.EscalierObject[2].LoadSprite("Esc_Tile1");
   World.EscalierObject[3].LoadSprite("Esc_Tile2");
   World.EscalierObject[4].LoadSprite("Esc_Tile3");
   World.EscalierObject[5].LoadSprite("Esc_Tile4");
}

void Load328(void)
{
   World.Object[240].LoadSprite("TiPontLH");
}


void Load400(void)
{ 
		World.WoodWall[0] .LoadSprite("WoodWall_SE");
		World.WoodWall[1] .LoadSprite("WoodWall_NE");
		World.WoodWall[2] .LoadSprite("WoodWall_SE_Window");
		World.WoodWall[3] .LoadSprite("WoodWall_NE_Window");
		World.WoodWall[4] .LoadSprite("WoodWall_NE_DoorB");
		World.WoodWall[5] .LoadSprite("WoodWall_NE_DoorT");
		World.WoodWall[6] .LoadSprite("WoodWall_SE_DoorB");
		World.WoodWall[7] .LoadSprite("WoodWall_SE_DoorT");
		World.WoodWall[8] .LoadSprite("WoodWall_CornerR");
		World.WoodWall[9] .LoadSprite("WoodWall_CornerT");
		World.WoodWall[10].LoadSprite("WoodWall_CornerL"); 
		World.WoodWall[11].LoadSprite("WoodWall_CornerB");
		World.WoodWall[12].LoadSprite("WoodWall_CornerX");
      World.WoodWall[13].LoadSprite("WoodWall_TNE");
		World.WoodWall[14].LoadSprite("WoodWall_TNW");
		World.WoodWall[15].LoadSprite("WoodWall_TSE");
		World.WoodWall[16].LoadSprite("WoodWall_TSW");
      World.WoodWall[17].LoadSprite("MidWoodWall_NE_Window");
		World.WoodWall[18].LoadSprite("MidWoodWall_SE_Window");
}

void Load401(void)
{ 
		World.NBrickWall[0] .LoadSprite("NBrickWall_SE");
		World.NBrickWall[1] .LoadSprite("NBrickWall_NE");
		World.NBrickWall[2] .LoadSprite("NBrickWall_NE_Window");
		World.NBrickWall[3] .LoadSprite("NBrickWall_SE_Window");
		World.NBrickWall[4] .LoadSprite("NBrickWall_NE_DoorB");
		World.NBrickWall[5] .LoadSprite("NBrickWall_NE_DoorT");
		World.NBrickWall[6] .LoadSprite("NBrickWall_SE_DoorB");
		World.NBrickWall[7] .LoadSprite("NBrickWall_SE_DoorT");
		World.NBrickWall[8] .LoadSprite("NBrickWall_CoinE");
		World.NBrickWall[9] .LoadSprite("NBrickWall_CoinN");
		World.NBrickWall[10].LoadSprite("NBrickWall_CoinW"); 
		World.NBrickWall[11].LoadSprite("NBrickWall_CoinS");
		World.NBrickWall[12].LoadSprite("NBrickWall_CoinX");
      World.NBrickWall[13].LoadSprite("NBrickWall_TNE");
		World.NBrickWall[14].LoadSprite("NBrickWall_TNW");
		World.NBrickWall[15].LoadSprite("NBrickWall_TSE");
		World.NBrickWall[16].LoadSprite("NBrickWall_TSW");
}

void Load402(void)
{ 
		World.NBrickWallS[0] .LoadSprite("NBrickWallS_SE");
		World.NBrickWallS[1] .LoadSprite("NBrickWallS_NE");
		World.NBrickWallS[2] .LoadSprite("NBrickWallS_SE_Window");
		World.NBrickWallS[3] .LoadSprite("NBrickWallS_NE_Window");
		World.NBrickWallS[4] .LoadSprite("NBrickWallS_NE_DoorB");
		World.NBrickWallS[5] .LoadSprite("NBrickWallS_NE_DoorT");
		World.NBrickWallS[6] .LoadSprite("NBrickWallS_SE_DoorB");
		World.NBrickWallS[7] .LoadSprite("NBrickWallS_SE_DoorT");
		World.NBrickWallS[8] .LoadSprite("NBrickWallS_CoinE");
		World.NBrickWallS[9] .LoadSprite("NBrickWallS_CoinN");
		World.NBrickWallS[10].LoadSprite("NBrickWallS_CoinW"); 
		World.NBrickWallS[11].LoadSprite("NBrickWallS_CoinS");
		World.NBrickWallS[12].LoadSprite("NBrickWallS_CoinX");
      World.NBrickWallS[13].LoadSprite("NBrickWallS_TNE");
		World.NBrickWallS[14].LoadSprite("NBrickWallS_TNW");
		World.NBrickWallS[15].LoadSprite("NBrickWallS_TSE");
		World.NBrickWallS[16].LoadSprite("NBrickWallS_TSW");
      World.NBrickWallS[17].LoadSprite("NBrickWallS_SE_S");
		World.NBrickWallS[18].LoadSprite("NBrickWallS_SE_E");
		World.NBrickWallS[19].LoadSprite("NBrickWallS_NE_S");
		World.NBrickWallS[20].LoadSprite("NBrickWallS_NE_E");
}

void Load403(void)
{ 
		World.NBrickWallS2[0] .LoadSprite("NBrickWallS2_SE");
		World.NBrickWallS2[1] .LoadSprite("NBrickWallS2_NE");
		World.NBrickWallS2[2] .LoadSprite("NBrickWallS2_SE_Window");
		World.NBrickWallS2[3] .LoadSprite("NBrickWallS2_NE_Window");
		World.NBrickWallS2[4] .LoadSprite("NBrickWallS2_NE_DoorB");
		World.NBrickWallS2[5] .LoadSprite("NBrickWallS2_NE_DoorT");
		World.NBrickWallS2[6] .LoadSprite("NBrickWallS2_SE_DoorB");
		World.NBrickWallS2[7] .LoadSprite("NBrickWallS2_SE_DoorT");
		World.NBrickWallS2[8] .LoadSprite("NBrickWallS2_CoinE");
		World.NBrickWallS2[9] .LoadSprite("NBrickWallS2_CoinN");
		World.NBrickWallS2[10].LoadSprite("NBrickWallS2_CoinW"); 
		World.NBrickWallS2[11].LoadSprite("NBrickWallS2_CoinS");
		World.NBrickWallS2[12].LoadSprite("NBrickWallS2_CoinX");
      World.NBrickWallS2[13].LoadSprite("NBrickWallS2_TNE");
		World.NBrickWallS2[14].LoadSprite("NBrickWallS2_TNW");
		World.NBrickWallS2[15].LoadSprite("NBrickWallS2_TSE");
		World.NBrickWallS2[16].LoadSprite("NBrickWallS2_TSW");
}


void Load6002(void) 
{
	LoadInit();
   LoadIsoSquare(World.CavernG,       "CavernG", T_CAVERNG_X, T_CAVERNG_Y);
}

void Load404(void) {
   Load6002();
   World.GCavernWall[0].LoadSprite("GCavernCornerN");
   World.GCavernWall[1].LoadSprite("GCavernTopWall 1");
   World.GCavernWall[2].LoadSprite("GCavernTopWall 2");
   World.GCavernWall[3].LoadSprite("GCavernTopWall 3");
   World.GCavernWall[4].LoadSprite("GCavernTopWall 4");
   World.GCavernWall[5].LoadSprite("GCavernTopWall 5");
   World.GCavernWall[6].LoadSprite("GCavernTopWall 6");
   World.GCavernWall[7].LoadSprite("GCavernTopWall 7");
   World.GCavernWall[8].LoadSprite("GCavernTopWall 8");
   World.GCavernWall[9].LoadSprite("GCavernTopWall 9");
   World.GCavernWall[10].LoadSprite("GCavernWall 1");
   World.GCavernWall[11].LoadSprite("GCavernWall 2");
   World.GCavernWall[12].LoadSprite("GCavernWall 3");
   World.GCavernWall[13].LoadSprite("GCavernWall 4");
   World.GCavernWall[14].LoadSprite("GCavernWall 5");
   World.GCavernWall[15].LoadSprite("GCavernWall 6");
   World.GCavernWall[16].LoadSprite("GCavernWall 7");
   World.GCavernWall[17].LoadSprite("GCavernWall 8");
   World.GCavernWall[18].LoadSprite("GCavernWall 9");
   World.GCavernWall[19].LoadSprite("GCavernLowWall 1");
   World.GCavernWall[20].LoadSprite("GCavernLowWall 2");
   World.GCavernWall[21].LoadSprite("GCavernLowWall 3");
   World.GCavernWall[22].LoadSprite("GCavernLowWall 4");
   World.GCavernWall[23].LoadSprite("GCavernLowWall 5");
   World.GCavernWall[24].LoadSprite("GCavernLowWall 6");
   World.GCavernWall[25].LoadSprite("GCavernLowWall 7");
   World.GCavernWall[26].LoadSprite("GCavernLowWall 8");
   World.GCavernWall[27].LoadSprite("GCavernLowWall 9");
   World.GCavernWall[28].LoadSprite("GCavernRightWall 1");
   World.GCavernWall[29].LoadSprite("GCavernRightWall 2");
   World.GCavernWall[30].LoadSprite("GCavernRightWall 3");
   World.GCavernWall[31].LoadSprite("GCavernRightWall 4");
   World.GCavernWall[32].LoadSprite("GCavernRightWall 5");
   World.GCavernWall[33].LoadSprite("GCavernRightWall 6"); 
   World.GCavernWall[34].LoadSprite("GCavernRightWall 7");
   World.GCavernWall[35].LoadSprite("GCavernRightWall 8");
   World.GCavernWall[36].LoadSprite("GCavernRightWall 9");
   World.GCavernWall[37].LoadSprite("GCavernCornerV");
   World.GCavernWall[38].LoadSprite("GCavernCorner");
   World.GCavernWall[39].LoadSprite("GCavernCorner2");
   World.GCavernWall[40].LoadSprite("GInnerCornerN");
   World.GCavernWall[41].LoadSprite("GInnerCornerV");
   World.GCavernWall[42].LoadSprite("GInnerCorner");
   World.GCavernWall[43].LoadSprite("GInnerCorner2");
   World.Object[220].LoadSprite("GCavernInternEntrance");
} 

void Load405(void) {
      World.Object[210].LoadSprite("GStal1");
}

void Load406(void) {
      World.Object[211].LoadSprite("GStal2");
}

void Load407(void) {
      World.Object[212].LoadSprite("GStal3");
}

void Load408(void) {
      World.Object[213].LoadSprite("GStal4");
}

void Load409(void) {
      World.Object[214].LoadSprite("GStal5");
}

void Load410(void) {
      World.Object[215].LoadSprite("GStal6");
}

void Load411(void) {
      World.Object[216].LoadSprite("GStal7");
}

void Load412(void) {
      World.Object[217].LoadSprite("GLargeC1");
}

void Load413(void) {
      World.Object[218].LoadSprite("GLargeC2");
}

void Load414(void) {
      World.Object[219].LoadSprite("GLargeC3");
} 

void Load415(void)
{
   World.Object[221].LoadSprite("FontaineNeige1");
}

void Load416(void)
{
   World.Object[222].LoadSprite("HuteGobG1Ffoor");
}

void Load417(void)
{
   World.Object[223].LoadSprite("HuteGobGlace1");
}

void Load418(void)
{
   World.Object[224].LoadSprite("EntreeMontG");
}

void Load419(void)
{
   World.Object[225].LoadSprite("MontG");
}

void Load420(void)
{
   World.Object[226].LoadSprite("Mont2G");
}

void Load421(void)
{ 
	World.NBrickWallS3[0] .LoadSprite("3NBrickWallS3_SE");
	World.NBrickWallS3[1] .LoadSprite("3NBrickWallS3_NE");
	World.NBrickWallS3[2] .LoadSprite("3NBrickWallS3_SE_Window");
	World.NBrickWallS3[3] .LoadSprite("3NBrickWallS3_NE_Window");
	World.NBrickWallS3[4] .LoadSprite("3NBrickWallS3_NE_DoorB");
	World.NBrickWallS3[5] .LoadSprite("3NBrickWallS3_NE_DoorT");
	World.NBrickWallS3[6] .LoadSprite("3NBrickWallS3_SE_DoorB");
	World.NBrickWallS3[7] .LoadSprite("3NBrickWallS3_SE_DoorT");
	World.NBrickWallS3[8] .LoadSprite("3NBrickWallS3_CoinE");
	World.NBrickWallS3[9] .LoadSprite("3NBrickWallS3_CoinN");
	World.NBrickWallS3[10].LoadSprite("3NBrickWallS3_CoinW"); 
	World.NBrickWallS3[11].LoadSprite("3NBrickWallS3_CoinS");
	World.NBrickWallS3[12].LoadSprite("3NBrickWallS3_CoinX");
   World.NBrickWallS3[13].LoadSprite("3NBrickWallS3_TNE");
	World.NBrickWallS3[14].LoadSprite("3NBrickWallS3_TNW");
	World.NBrickWallS3[15].LoadSprite("3NBrickWallS3_TSE");
	World.NBrickWallS3[16].LoadSprite("3NBrickWallS3_TSW");
}

void Load422(void)
{ 
	World.NBrickWallS4[0] .LoadSprite("4NBrickWallS4_SE");
	World.NBrickWallS4[1] .LoadSprite("4NBrickWallS4_NE");
	World.NBrickWallS4[2] .LoadSprite("4NBrickWallS4_SE_Window");
	World.NBrickWallS4[3] .LoadSprite("4NBrickWallS4_NE_Window");
	World.NBrickWallS4[4] .LoadSprite("4NBrickWallS4_NE_DoorB");
	World.NBrickWallS4[5] .LoadSprite("4NBrickWallS4_NE_DoorT");
	World.NBrickWallS4[6] .LoadSprite("4NBrickWallS4_SE_DoorB");
	World.NBrickWallS4[7] .LoadSprite("4NBrickWallS4_SE_DoorT");
	World.NBrickWallS4[8] .LoadSprite("4NBrickWallS4_CoinE");
	World.NBrickWallS4[9] .LoadSprite("4NBrickWallS4_CoinN");
	World.NBrickWallS4[10].LoadSprite("4NBrickWallS4_CoinW"); 
	World.NBrickWallS4[11].LoadSprite("4NBrickWallS4_CoinS");
	World.NBrickWallS4[12].LoadSprite("4NBrickWallS4_CoinX");
   World.NBrickWallS4[13].LoadSprite("4NBrickWallS4_TNE");
	World.NBrickWallS4[14].LoadSprite("4NBrickWallS4_TNW");
	World.NBrickWallS4[15].LoadSprite("4NBrickWallS4_TSE");
	World.NBrickWallS4[16].LoadSprite("4NBrickWallS4_TSW");
}

void Load423(void)
{ 
	World.NBrickWallS5[0] .LoadSprite("5NBrickWallS5_SE");
	World.NBrickWallS5[1] .LoadSprite("5NBrickWallS5_NE");
	World.NBrickWallS5[2] .LoadSprite("5NBrickWallS5_SE_Window");
	World.NBrickWallS5[3] .LoadSprite("5NBrickWallS5_NE_Window");
	World.NBrickWallS5[4] .LoadSprite("5NBrickWallS5_NE_DoorB");
	World.NBrickWallS5[5] .LoadSprite("5NBrickWallS5_NE_DoorT");
	World.NBrickWallS5[6] .LoadSprite("5NBrickWallS5_SE_DoorB");
	World.NBrickWallS5[7] .LoadSprite("5NBrickWallS5_SE_DoorT");
	World.NBrickWallS5[8] .LoadSprite("5NBrickWallS5_CoinE");
	World.NBrickWallS5[9] .LoadSprite("5NBrickWallS5_CoinN");
	World.NBrickWallS5[10].LoadSprite("5NBrickWallS5_CoinW"); 
	World.NBrickWallS5[11].LoadSprite("5NBrickWallS5_CoinS");
	World.NBrickWallS5[12].LoadSprite("5NBrickWallS5_CoinX");
   World.NBrickWallS5[13].LoadSprite("5NBrickWallS5_TNE");
	World.NBrickWallS5[14].LoadSprite("5NBrickWallS5_TNW");
	World.NBrickWallS5[15].LoadSprite("5NBrickWallS5_TSE");
	World.NBrickWallS5[16].LoadSprite("5NBrickWallS5_TSW");
}

void Load424(void)
{ 
	World.NBrickWallS6[0] .LoadSprite("6NBrickWallS6_SE");
	World.NBrickWallS6[1] .LoadSprite("6NBrickWallS6_NE");
	World.NBrickWallS6[2] .LoadSprite("6NBrickWallS6_SE_Window");
	World.NBrickWallS6[3] .LoadSprite("6NBrickWallS6_NE_Window");
	World.NBrickWallS6[4] .LoadSprite("6NBrickWallS6_NE_DoorB");
	World.NBrickWallS6[5] .LoadSprite("6NBrickWallS6_NE_DoorT");
	World.NBrickWallS6[6] .LoadSprite("6NBrickWallS6_SE_DoorB");
	World.NBrickWallS6[7] .LoadSprite("6NBrickWallS6_SE_DoorT");
	World.NBrickWallS6[8] .LoadSprite("6NBrickWallS6_CoinE");
	World.NBrickWallS6[9] .LoadSprite("6NBrickWallS6_CoinN");
	World.NBrickWallS6[10].LoadSprite("6NBrickWallS6_CoinW"); 
	World.NBrickWallS6[11].LoadSprite("6NBrickWallS6_CoinS");
	World.NBrickWallS6[12].LoadSprite("6NBrickWallS6_CoinX");
   World.NBrickWallS6[13].LoadSprite("6NBrickWallS6_TNE");
	World.NBrickWallS6[14].LoadSprite("6NBrickWallS6_TNW");
	World.NBrickWallS6[15].LoadSprite("6NBrickWallS6_TSE");
	World.NBrickWallS6[16].LoadSprite("6NBrickWallS6_TSW");
}

void Load425(void)
{ 
	World.NBrickWallRS3[0] .LoadSprite("3Ruined_CoinS");
	World.NBrickWallRS3[1] .LoadSprite("3Ruined_SE_Wall");
	World.NBrickWallRS3[2] .LoadSprite("3Ruined_NE_Wall");
	World.NBrickWallRS3[3] .LoadSprite("3Ruined_SE_Window");
	World.NBrickWallRS3[4] .LoadSprite("3Ruined_NE_Window");
}

void Load426(void)
{ 
	World.NBrickWallRS4[0] .LoadSprite("4Ruined_CoinS");
	World.NBrickWallRS4[1] .LoadSprite("4Ruined_SE_Wall");
	World.NBrickWallRS4[2] .LoadSprite("4Ruined_NE_Wall");
	World.NBrickWallRS4[3] .LoadSprite("4Ruined_SE_Window");
	World.NBrickWallRS4[4] .LoadSprite("4Ruined_NE_Window");
}

void Load427(void)
{ 
	World.NBrickWallRS5[0] .LoadSprite("5Ruined_CoinS");
	World.NBrickWallRS5[1] .LoadSprite("5Ruined_SE_Wall");
	World.NBrickWallRS5[2] .LoadSprite("5Ruined_NE_Wall");
	World.NBrickWallRS5[3] .LoadSprite("5Ruined_SE_Window");
	World.NBrickWallRS5[4] .LoadSprite("5Ruined_NE_Window");
}

void Load428(void)
{ 
	World.NBrickWallRS6[0] .LoadSprite("6Ruined_CoinS");
	World.NBrickWallRS6[1] .LoadSprite("6Ruined_SE_Wall");
	World.NBrickWallRS6[2] .LoadSprite("6Ruined_NE_Wall");
	World.NBrickWallRS6[3] .LoadSprite("6Ruined_SE_Window");
	World.NBrickWallRS6[4] .LoadSprite("6Ruined_NE_Window");
}

void Load429(void)
{ 
	World.Object[227].LoadSprite("BarilPied");
}

void Load430(void)
{ 
	World.Object[228].LoadSprite("FTourFront");
}

void Load431(void)
{ 
	World.Object[229].LoadSprite("BTourBack");
}

void Load432(void)
{
   World.Object[230].LoadSprite("V2_Flower01");
   World.Object[231].LoadSprite("V2_Flower02");
   World.Object[232].LoadSprite("V2_Flower03");
   World.Object[233].LoadSprite("V2_Flower04");
   World.Object[234].LoadSprite("V2_Flower05");
   World.Object[235].LoadSprite("V2_Flower06");
   World.Object[236].LoadSprite("V2_Flower07");
}

void Load433(void)
{
   World.V2CastleWall[ 0].LoadSprite("Castle_SE_1");
   World.V2CastleWall[ 1].LoadSprite("Castle_SE_2");
   World.V2CastleWall[ 2].LoadSprite("Castle_SE_3");
   World.V2CastleWall[ 3].LoadSprite("Castle_SE_4");
   World.V2CastleWall[ 4].LoadSprite("Castle_NE_1");
   World.V2CastleWall[ 5].LoadSprite("Castle_NE_2");
   World.V2CastleWall[ 6].LoadSprite("Castle_NE_3");
   World.V2CastleWall[ 7].LoadSprite("Castle_NE_4");
   World.V2CastleWall[ 8].LoadSprite("Castle_NE_DoorB");
   World.V2CastleWall[ 9].LoadSprite("Castle_NE_DoorT");
   World.V2CastleWall[10].LoadSprite("Castle_SE_DoorB");
   World.V2CastleWall[11].LoadSprite("Castle_SE_DoorT");
   World.V2CastleWall[12].LoadSprite("Castle_CornerR");
   World.V2CastleWall[13].LoadSprite("Castle_CornerT");
   World.V2CastleWall[14].LoadSprite("Castle_CornerL");
   World.V2CastleWall[15].LoadSprite("Castle_CornerB");
   World.V2CastleWall[16].LoadSprite("Castle_CornerX");
   World.V2CastleWall[17].LoadSprite("Castle_TNE");
   World.V2CastleWall[18].LoadSprite("Castle_TNW");
   World.V2CastleWall[19].LoadSprite("Castle_TSE");
   World.V2CastleWall[20].LoadSprite("Castle_TSW");
   World.V2CastleWall[21].LoadSprite("Caltle_Poutre");
   World.V2CastleWall[22].LoadSprite("Caltle_Poutre2");
   World.V2CastleWall[23].LoadSprite("Caltle_Poutre3");
   World.V2CastleWall[24].LoadSprite("Castle_NE_DDoorB");
   World.V2CastleWall[25].LoadSprite("Castle_NE_DDoorT");
   World.V2CastleWall[26].LoadSprite("Castle_SE_DDoorB");
   World.V2CastleWall[27].LoadSprite("Castle_SE_DDoorT");
}

void Load434(void)
{
   World.V2CastleRampart[ 0].LoadSprite("V2_Rampart_SE");
   World.V2CastleRampart[ 1].LoadSprite("V2_Rampart_NE");
   World.V2CastleRampart[ 2].LoadSprite("V2_Rampart_SETour");
   World.V2CastleRampart[ 3].LoadSprite("V2_Rampart_NETour");
   World.V2CastleRampart[ 4].LoadSprite("V2_Rampart_CoinL");
   World.V2CastleRampart[ 5].LoadSprite("V2_Rampart_CoinT");
   World.V2CastleRampart[ 6].LoadSprite("V2_Rampart_CoinR");
   World.V2CastleRampart[ 7].LoadSprite("V2_Rampart_CoinB");
   World.V2CastleRampart[ 8].LoadSprite("V2_Rampart_D");
   World.V2CastleRampart[ 9].LoadSprite("V2_Rampart_F");
   World.V2CastleRampart[10].LoadSprite("Castle_Collonne");
}

void Load435(void)
{
   World.Object[239].LoadSprite("Cedre_Nain");
}

void Load436(void)
{
   World.V2CastleWallS[ 0].LoadSprite("SCastle_SE_1");
   World.V2CastleWallS[ 1].LoadSprite("SCastle_SE_2");
   World.V2CastleWallS[ 2].LoadSprite("SCastle_SE_3");
   World.V2CastleWallS[ 3].LoadSprite("SCastle_SE_4");
   World.V2CastleWallS[ 4].LoadSprite("SCastle_NE_1");
   World.V2CastleWallS[ 5].LoadSprite("SCastle_NE_2");
   World.V2CastleWallS[ 6].LoadSprite("SCastle_NE_3");
   World.V2CastleWallS[ 7].LoadSprite("SCastle_NE_4");
   World.V2CastleWallS[ 8].LoadSprite("SCastle_Merge1");
   World.V2CastleWallS[ 9].LoadSprite("SCastle_Merge2");
   World.V2CastleWallS[10].LoadSprite("SCastle_Merge3");
   World.V2CastleWallS[11].LoadSprite("SCastle_Merge4");
   World.V2CastleWallS[12].LoadSprite("SCastle_CornerR");
   World.V2CastleWallS[13].LoadSprite("SCastle_CornerT");
   World.V2CastleWallS[14].LoadSprite("SCastle_CornerL");
   World.V2CastleWallS[15].LoadSprite("SCastle_CornerB");
   World.V2CastleWallS[16].LoadSprite("SCastle_CornerX");
   World.V2CastleWallS[17].LoadSprite("SCastle_TNE");
   World.V2CastleWallS[18].LoadSprite("SCastle_TNW");
   World.V2CastleWallS[19].LoadSprite("SCastle_TSE");
   World.V2CastleWallS[20].LoadSprite("SCastle_TSW");
}

void Load4269(void)
{
   World.NStucoWall[ 0].LoadSprite("StucoNew_SE1");
   World.NStucoWall[ 1].LoadSprite("StucoNew_SE2");
   World.NStucoWall[ 2].LoadSprite("StucoNew_SE3");
   World.NStucoWall[ 3].LoadSprite("StucoNew_SE4");
   World.NStucoWall[ 4].LoadSprite("StucoNew_NE1");
   World.NStucoWall[ 5].LoadSprite("StucoNew_NE2");
   World.NStucoWall[ 6].LoadSprite("StucoNew_NE3");
   World.NStucoWall[ 7].LoadSprite("StucoNew_NE4");
   World.NStucoWall[ 8].LoadSprite("StucoNew_NE_WindowO1");
   World.NStucoWall[ 9].LoadSprite("StucoNew_NE_WindowO2");
   World.NStucoWall[10].LoadSprite("StucoNew_NE_WindowO3");
   World.NStucoWall[11].LoadSprite("StucoNew_NE_WindowO4");
   World.NStucoWall[12].LoadSprite("StucoNew_SE_WindowO1");
   World.NStucoWall[13].LoadSprite("StucoNew_SE_WindowO2");
   World.NStucoWall[14].LoadSprite("StucoNew_SE_WindowO3");
   World.NStucoWall[15].LoadSprite("StucoNew_SE_WindowO4");
   World.NStucoWall[16].LoadSprite("StucoNew_NE_WindowE1");
   World.NStucoWall[17].LoadSprite("StucoNew_NE_WindowE2");
   World.NStucoWall[18].LoadSprite("StucoNew_NE_WindowE3");
   World.NStucoWall[19].LoadSprite("StucoNew_NE_WindowE4");
   World.NStucoWall[20].LoadSprite("StucoNew_SE_WindowE1");
   World.NStucoWall[21].LoadSprite("StucoNew_SE_WindowE2");
   World.NStucoWall[22].LoadSprite("StucoNew_SE_WindowE3");
   World.NStucoWall[23].LoadSprite("StucoNew_SE_WindowE4");
   World.NStucoWall[24].LoadSprite("StucoNew_CoinE");
   World.NStucoWall[25].LoadSprite("StucoNew_CoinN");
   World.NStucoWall[26].LoadSprite("StucoNew_CoinS");
   World.NStucoWall[27].LoadSprite("StucoNew_CoinW");
   World.NStucoWall[28].LoadSprite("StucoNew_CoinX");
   World.NStucoWall[29].LoadSprite("StucoNew_TNE");
   World.NStucoWall[30].LoadSprite("StucoNew_TNW");
   World.NStucoWall[31].LoadSprite("StucoNew_TSE");
   World.NStucoWall[32].LoadSprite("StucoNew_TSW");
   World.NStucoWall[33].LoadSprite("StucoNew_NE_DoorB");
   World.NStucoWall[34].LoadSprite("StucoNew_NE_DoorT");
   World.NStucoWall[35].LoadSprite("StucoNew_SE_DoorB");
   World.NStucoWall[36].LoadSprite("StucoNew_SE_DoorT");
   World.NStucoWall[37].LoadSprite("StucoNewP_SE1");
   World.NStucoWall[38].LoadSprite("StucoNewP_SE2");
   World.NStucoWall[39].LoadSprite("StucoNewP_SE3");
   World.NStucoWall[40].LoadSprite("StucoNewP_SE4");
   World.NStucoWall[41].LoadSprite("StucoNewP_NE1");
   World.NStucoWall[42].LoadSprite("StucoNewP_NE2");
   World.NStucoWall[43].LoadSprite("StucoNewP_NE3");
   World.NStucoWall[44].LoadSprite("StucoNewP_NE4");
}

void Load4290(void)
{
   World.NMurPoutre[0].LoadSprite("Poutre01");
   World.NMurPoutre[1].LoadSprite("Poutre02");
   World.NMurPoutre[2].LoadSprite("Poutre03");
   World.NMurPoutre[3].LoadSprite("Poutre04");
   World.NMurPoutre[4].LoadSprite("Poutre05");
   World.NMurPoutre[5].LoadSprite("Poutre06");
   World.NMurPoutre[6].LoadSprite("Poutre07");
}

void Load4297(void)
{
   World.NTourGarde[0].LoadSprite("TourGardeHG");
   World.NTourGarde[1].LoadSprite("TourGardeHD");
   World.NTourGarde[2].LoadSprite("TourGardeBG");
   World.NTourGarde[3].LoadSprite("TourGardeBD");
}

void Load6000(void) 
{
	LoadInit();
   LoadIsoSquare(World.Snow,       "Snow", T_SNOW_X, T_SNOW_Y);
}

void Load6001(void) 
{
	LoadInit();
   LoadIsoSquare(World.EarthI,       "EarthI", T_EARTHI_X, T_EARTHI_Y);
}

void Load6003(void) 
{
	LoadInit();
   LoadIsoSquare(World.IceFloor,       "IceFloor", T_ICE_X, T_ICE_Y);
}

void Load6004(void) 
{
	LoadInit();
   LoadIsoSquare(World.TuileTmps1,       "TuileTmpl1", T_TUILE1_X, T_TUILE1_Y);
}

void Load6005(void) 
{
	LoadInit();
   LoadIsoSquare(World.TuileTmps2,       "TuileTmpl2", T_TUILE2_X, T_TUILE2_Y);
}

void Load6006(void) 
{
	LoadInit();
   LoadIsoSquare(World.RockFloor2,       "Rock2Floor", T_ROCK2_X, T_ROCK2_Y);
}

void Load6007(void) 
{
	LoadInit();
   LoadIsoSquare(World.STuileTmps1,       "STuileTmpl1", T_STUILE1_X, T_STUILE1_Y);
}

void Load6008(void) 
{
	LoadInit();
   LoadIsoSquare(World.STuileTmps2,       "STuileTmpl2", T_STUILE2_X, T_STUILE2_Y);
}

void Load6009(void) 
{
	LoadInit();
   for(int i=0;i<T_AWATER01_N;i++)
   {
      char strName[100];
      sprintf(strName,"%02d_AnimWater01",i);
      LoadIsoSquare(World.AnimWater01[i],strName, T_AWATER01_X, T_AWATER01_Y);
   }
}

void Load6011(void)
{
   LoadInit();
   LoadIsoSquare(World.NewLava,       "NewLava", T_NLAVA_X, T_NLAVA_Y);
   /*
   for(int i=0;i<T_ALAVA01_N;i++)
   {
      char strName[100];
      sprintf(strName,"%02d_AnimLava01",i);
      LoadIsoSquare(World.AnimLava01[i],strName, T_ALAVA01_X, T_ALAVA01_Y);
   }
   */
}

void Load6013(void) 
{
	LoadInit();
   LoadIsoSquare(World.Cendre,       "Cendre", T_CENDRE_X, T_CENDRE_Y);
}

void Load1291(void) 
{
   World.DrakeSkeleton[0].LoadSprite("Drake Skeleton01");
}
void Load1292(void) 
{
   World.DrakeSkeleton[1].LoadSprite("Drake Skeleton02");
}
void Load1293(void) 
{
   World.DrakeSkeleton[2].LoadSprite("Drake Skeleton03");
}
void Load1294(void) 
{
   World.DrakeSkeleton[3].LoadSprite("Drake Skeleton04");
}

void Load4251(void) 
{
   World.Object[244].LoadSprite("BancTempleNoir");
}

void Load4253(void) 
{
   World.Object[245].LoadSprite("MummyBox1");
   World.Object[246].LoadSprite("MummyBox2");
}

void Load4257(void) 
{
   World.Object[247].LoadSprite("SquareTower1");
   World.Object[248].LoadSprite("SquareTower2");
}

void Load4261(void) 
{
   World.Object[249].LoadSprite("StatueModel1_1");
   World.Object[250].LoadSprite("StatueModel1_2");
   World.Object[251].LoadSprite("StatueModel1_3");
   World.Object[252].LoadSprite("StatueModel1_4");
   World.Object[253].LoadSprite("StatueModel2_1");
   World.Object[254].LoadSprite("StatueModel2_2");
   World.Object[255].LoadSprite("StatueModel2_3");
   World.Object[256].LoadSprite("StatueModel2_4");
}



void Load6500(void) 
{
	Load22();
	Load6000();
	Load48();
}

void Load6532(void) 
{
	Load23();
	Load6000();
	Load48();
}

void Load6564(void) 
{
	Load6000();
	Load6001();
	Load48();
}
void Load6596(void) 
{
	Load22();
	Load6001();
	Load48();
}
void Load6628(void) 
{
	Load23();
	Load6001();
	Load48();
}

void Load6660(void) 
{
	Load6000();
	Load6002();
	Load48();
}

void Load6692(void) 
{
	Load6001();
	Load6002();
	Load48();
}

void Load6724(void) 
{
	Load6003();
	Load31();
	Load48();
}

void Load6756(void) 
{
	Load6003();
	Load6000();
	Load48();
}

void Load6788(void) 
{
	Load27();    // GRASS
	Load266();   // Smooth3
	Load6004();  // Templ1
}

void Load6792(void) 
{
	Load26();    // Rock
	Load266();   // Smooth3
	Load6004();  // Templ1
}

void Load6816(void)
{
   Load6000();  // SNOW
	Load266();   // Smooth3
	Load6004();  // Templ1
}

void Load6796(void) 
{
	Load27();    // GRASS
	Load266();   // Smooth3
	Load6005();  // Templ1
}

void Load6800(void) 
{
	Load26();    // Rock
	Load266();   // Smooth3
	Load6005();  // Templ1
}

void Load6820(void)
{
   Load6000();  // SNOW
	Load266();   // Smooth3
	Load6005();  // Templ1
}

void Load7000(void) 
{
   LoadInit();
   LoadIsoSquare(World.Floor2,       "2Wooden", T_2WOODEN_X, T_2WOODEN_Y);
}

void Load7001(void) 
{
   LoadInit();
   LoadIsoSquare(World.Floor3,       "3Wooden", T_3WOODEN_X, T_3WOODEN_Y);
}

void Load6804(void) 
{
	Load7000();  
	Load266();   // Smooth3
	Load6005();  // Templ1
}


void Load6808(void) 
{
	Load24();    // Rock
	Load266();   // Smooth3
	Load6005();  // Templ1
}


void Load6812(void) 
{
	Load6004();
	Load266(); 
	Load6005();
}






void Load7003(void) 
{
	Load27();
	Load7000(); // WOODEN FLOOR
	Load49();
}

void Load7035(void) 
{
	Load22();
	Load7000(); // WOODEN FLOOR
	Load49();
}

void Load7057(void) 
{
	Load23();
	Load7000(); // WOODEN FLOOR
	Load49();
}

void Load7089(void) 
{
	Load14();
	Load7000(); // WOODEN FLOOR
	Load49();
}

void Load7122(void) 
{
	Load27();
	Load7001(); // WOODEN FLOOR
	Load49();
}

void Load7154(void) 
{
	Load22();
	Load7001(); // WOODEN FLOOR
	Load49();
}

void Load7176(void) 
{
	Load23();
	Load7001(); // WOODEN FLOOR
	Load49();
}

void Load7209(void) 
{
	Load14();
	Load7001(); // WOODEN FLOOR
	Load49();
}

void Load7261(void) 
{
		World.SRockFloor2[0].LoadSprite("2srf1");
		World.SRockFloor2[1].LoadSprite("2srf2");
		World.SRockFloor2[2].LoadSprite("2srf3");
		World.SRockFloor2[3].LoadSprite("2srf4");
}

void Load7265(void) {
		World.SRockFloor3[0].LoadSprite("3srf1");
		World.SRockFloor3[1].LoadSprite("3srf2");
		World.SRockFloor3[2].LoadSprite("3srf3");
		World.SRockFloor3[3].LoadSprite("3srf4");
}

void Load7269(void) {
		World.SRockFloor4[0].LoadSprite("4srf1");
		World.SRockFloor4[1].LoadSprite("4srf2");
		World.SRockFloor4[2].LoadSprite("4srf3");
		World.SRockFloor4[3].LoadSprite("4srf4");
}

void Load7273(void) {
		World.SRockFloor5[0].LoadSprite("5srf1");
		World.SRockFloor5[1].LoadSprite("5srf2");
		World.SRockFloor5[2].LoadSprite("5srf3");
		World.SRockFloor5[3].LoadSprite("5srf4");
}

void Load7277(void) {
		World.SRockFloor6[0].LoadSprite("6srf1");
		World.SRockFloor6[1].LoadSprite("6srf2");
		World.SRockFloor6[2].LoadSprite("6srf3");
		World.SRockFloor6[3].LoadSprite("6srf4");
}

void Load7251(void) 
{
   Load26();    // Rock
	Load49();   // Smooth
	Load6000();  // Templ1
}

void Load7313(void)
{
   Load33();    // Wood1
	Load266();   // Smooth3
	Load6000();  // Snow
}

void Load7317(void)
{
   Load7000();  // Wood2
	Load266();   // Smooth3
	Load6000();  // Snow
}

void Load7321(void)
{
   Load7001();  // Wood3
	Load266();   // Smooth3
	Load6000();  // Snow
}

void Load7325(void)
{
   Load6006();  // Rock2
	Load266();   // Smooth3
	Load6004();  // Templ1
}


void Load7329(void)
{
   Load6006();  // Rock2
	Load266();   // Smooth3
	Load6007();  // Templ1
}

void Load7333(void)
{
   Load6006();  // Rock2
	Load266();   // Smooth3
	Load6008();  // Templ2
}

void Load7337(void) 
{
	Load27();    // GRASS
	Load266();   // Smooth3
	Load6007();  // Templ1
}

void Load7341(void) 
{
	Load27();    // GRASS
	Load266();   // Smooth3
	Load6008();  // Templ2
}

void Load7345(void) 
{
	Load26();    // Rock
	Load266();   // Smooth3
	Load6007();  // Templ1
}

void Load7349(void) 
{
	Load26();    // Rock
	Load266();   // Smooth3
	Load6008();  // Templ2
}

void Load7353(void)
{
   Load6000();  // SNOW
	Load266();   // Smooth3
	Load6007();  // Templ1
}

void Load7357(void)
{
   Load6000();  // SNOW
	Load266();   // Smooth3
	Load6008();  // Templ2
}

void Load7361(void) 
{
	Load6007();
	Load266(); 
	Load6008();
}


void Load7365(void)
{
   Load33();    // Wood1
	Load266();   // Smooth3
	Load6007();  // Templ1
}

void Load7369(void)
{
   Load33();    // Wood1
	Load266();   // Smooth3
	Load6008();  // Templ2
}

void Load7373(void)
{
   Load7000();  // Wood2
	Load266();   // Smooth3
	Load6007();  // Templ1
}

void Load7377(void)
{
   Load7000();  // Wood2
	Load266();   // Smooth3
	Load6008();  // Templ2
}

void Load7381(void)
{
   Load7001();  // Wood3
	Load266();   // Smooth3
	Load6007();  // Templ1
}

void Load7385(void)
{
   Load7001();  // Wood3
	Load266();   // Smooth3
	Load6008();  // Templ2
}

void Load7389(void)
{
   Load6004();  // Temp1Big
	Load266();   // Smooth3
	Load6007();  // Templ1
}

void Load7393(void)
{
   Load6005();  // Temp2Big
	Load266();   // Smooth3
	Load6008();  // Templ2
}

void Load7397(void) 
{
   Load26();  // Rock
	Load49();  // Smooth
	Load31();  // Water 3
}

void Load7429(void)
{
	Load49();  // Smooth
	Load31();  // Water 3
}

void Load7461(void) {
	Load27();   // GRASS
	Load49();   // Smooth
	Load26(); // Rock2
}

void Load7493(void) {
	Load6013();   // cendre
	Load48();   // Smooth
	Load2(); // Rock
}

void Load7525(void) {
	Load6013();  // cendre
	Load48();    // Smooth
	Load32();    // Cav
}

void Load7557(void) {
	Load6013();   // cendre
	Load49();     // Smooth
	Load31();     // Water1
}



void Load7850(void) {
	Load6009();
   Load23();
	Load49();
}

void Load7882(void) {
	Load6009();
   Load2();
	Load49();
}

void Load7914(void) {
	Load6009();
   Load26();
	Load49();
}

void Load7946(void) {
	Load6009();
   Load14();
	Load49();
}

void Load7978(void) {
	Load6009();
   Load27();
	Load49();
}

void Load8010(void) {
	Load6009();
   Load22();
	Load49();
}

void Load8042(void) {
	Load6011();
   Load6013();
	Load49();
}



void TileSet::SetTileDimension(int Width, int Height) {
	
	
	// Set the Tile Dimension.
	TileWidth  = Width;
	TileHeight = Height;
}

void TileSet::SetViewDimension(int Width, int Height) {
// Set the View Dimension;
	ViewWidth  = Width+1;
	ViewHeight = Height+1;
	SizeWidth  = Width+1;
	SizeHeight = Height+1;
	Objects.SetView(ViewWidth, ViewHeight);
}

void TileSet::SetVirtualDimension(int Width, int Height) {
// Set the Virtual Map Dimension
	VirtualWidth  = Width;
	VirtualHeight = Height;
}

void TileSet::SetWorldDimension(int Width, int Height) {

   // Set the World Dimension
	WorldWidth  = Width;
	WorldHeight = Height;
}

void TileSet::SetNumberofWorld(char nb) {
	NumberofWorld = nb;

	delete [] WorldFileName;
	WorldFileName = new char *[NumberofWorld];

	CurrentWorld = 0;
}

void TileSet::SetWorldFileName(char *s) {
// Set the File Name of the World.
	WorldFileName[CurrentWorld] = new char [strlen(s)+1];

	strcpy(WorldFileName[CurrentWorld], s);
	CurrentWorld++;
}

BOOL TileSet::GetWallType(int x, int y, int t) {
   for (int i = -2; i < 3; i++) {
      for (int j = -2; j < 3; j++) {
         if (VirtualView[(xPos+x+i)+(yPos+y+j)*VirtualWidth] == t) {
            return TRUE;
			}
		}
	}
   return FALSE;
}

void TileSet::LoadVirtualMap(int xLoad, int yLoad, int World) {

 // Load the Virtual Map into memory.
	HFILE LoadMap;	                       // Map File Handle.

	int i, j, k;
   int DX, DY, DEX, DEY;
   int DX2, DY2, DEX2, DEY2;
   int XM, YM;
	long FilePtr = 0;                     // File position.
	unsigned char *Buffer;
	unsigned char *Buffer2;
	Buffer  = new unsigned char	[VirtualWidth*VirtualHeight/2]; // Buffer pour la Map.
	Buffer2 = new unsigned char	[VirtualWidth*VirtualHeight/2]; // Buffer pour la Map.
   ZeroMemory(Buffer,  VirtualWidth*VirtualHeight/2);
   ZeroMemory(Buffer2, VirtualWidth*VirtualHeight/2);
   int xMap, yMap;
   DWORD Index;
   int Temp;
	
   if (g_Var.inGame) 
   {
      g_Var.wDisplayMap = 1;
   }

   if ((LoadMap = _lopen(WorldFileName[World], OF_READ)) == HFILE_ERROR)
	// File doesn't exists.
		throw;

	// Ajuster la position du player.
   if (xLoad % (VirtualWidth/2) && xLoad > 0) {
      if ((xLoad % (VirtualWidth/2)) < VirtualWidth/4) {
         Temp   = xLoad - (xLoad % (VirtualWidth/2));
         xPos  += (xLoad % (VirtualWidth/2));
         xLoad  = Temp;
      } else {
         Temp  = xLoad + (VirtualWidth/2) - (xLoad % (VirtualWidth/2));
         xPos  -= (VirtualWidth/2) - (xLoad % (VirtualWidth/2));
         xLoad = Temp;
      }
   }

   if (yLoad % (VirtualHeight/2) && yLoad > 0) {
      if ((yLoad % (VirtualHeight/2)) < VirtualHeight/4) {
         Temp   = yLoad - (yLoad % (VirtualHeight/2));
         yPos  += (yLoad % (VirtualHeight/2));
         yLoad  = Temp;
      } else {
         Temp  = yLoad + (VirtualHeight/2) - (yLoad % (VirtualHeight/2));
         yPos  -= (VirtualHeight/2) - (yLoad % (VirtualHeight/2));
         yLoad = Temp;
      }
   }

	XM = YM = 1;
	if (xLoad < 0) {
      DX = -xLoad;
      xLoad = 0;
		if (DX & 1) {
			DX--;
			xPos--;
		}
   } else {
      DX = 0;
   }

   if (DX > VirtualWidth/2) {
      DX2 = DX-VirtualWidth/2;
   } else {
      DX2 = 0;
   }

   if (yLoad < 0) {
      DY = -yLoad;
      yLoad = 0;
   } else {
      DY = 0;
   }

   if (DY > VirtualHeight/2) {
      DY2 = DY-VirtualHeight/2;
   } else {
      DY2 = 0;
   }

   if (xLoad+VirtualWidth > WorldWidth) {
      DEX = (xLoad+VirtualWidth) - WorldWidth;
		XM = 0;
   } else {
      DEX = 0;
   }
   
   if (DEX > VirtualWidth/2) {
      DEX2 = DEX-VirtualWidth/2;
   } else {
      DEX2 = 0;
   }

   if (yLoad+VirtualHeight > WorldHeight) {
      DEY = (yLoad+VirtualHeight) - (WorldHeight+1);
		YM = 0;
   } else {
      DEY = 0;
   }

   if (DEY > VirtualHeight/2) {
      DEY2 = DEY-VirtualHeight/2;
   } else {
      DEY2 = 0;
   }

   xMap = (xLoad/(VirtualWidth/2)); 
   yMap = (yLoad/(VirtualHeight/2));

	LOG << "* DRW = " << VirtualWidth << ", " << VirtualHeight << ", " << xLoad << ", " << yLoad << ", " << DX << ", " << DY << ", " << DX2 << ", " << DY2 << ", " << DEX << ", " << DEY << ", " << DEX2 << ", " << DEY2 << ", " << xMap << ", " << yMap << ", " << XM << ", " << YM << "\r\n";

   memset(VirtualView, 20, VirtualWidth*VirtualHeight*2);
   // Loader la map
  
   // Loading Map (1, 1)
   
   _llseek(LoadMap, (xMap+yMap*(WorldWidth/(VirtualWidth/2)))*sizeof(DWORD), FILE_BEGIN);
   _lread(LoadMap, &Index, sizeof(DWORD));

   _llseek(LoadMap, Index, FILE_BEGIN);    // Change the position.
	_lread(LoadMap, Buffer2, VirtualWidth*VirtualHeight/2);

   RLE_Decompress((unsigned short *)Buffer2, (unsigned short *)Buffer, 128*128, 65536, LASTONE);

   for (j = DY; j < (VirtualHeight/2)-DEY2+DY; j++) {
	// Loop thru all y components.
		// Compute file position.
      k = (j-DY)*256;
      for (i = DX; i < (VirtualWidth/2)-DEX2+DX; i+=2) {
		// Loop trhu the x components of the map.
			VirtualView[i+j*VirtualWidth]   = Buffer[k] + (Buffer[k+1] << 8);
			VirtualView[(i+1)+j*VirtualWidth] = Buffer[k+2] + (Buffer[k+3] << 8);
         k+=4;
		}
	}

   // Loading Map (2, 1)
   
   _llseek(LoadMap, ((xMap+XM)+yMap*(WorldWidth/(VirtualWidth/2)))*sizeof(DWORD), FILE_BEGIN);
   _lread(LoadMap, &Index, sizeof(DWORD));

	_llseek(LoadMap, Index, FILE_BEGIN);    // Change the position.
	_lread(LoadMap, Buffer2, VirtualWidth*VirtualHeight/2);

   RLE_Decompress((unsigned short *)Buffer2, (unsigned short *)Buffer, 128*128, 65536, LASTONE);

   for (j = DY; j < (VirtualHeight/2)-DEY2+DY; j++) {
	// Loop thru all y components.
		// Compute file position.
      k = (j-DY)*256;
		for (i = (VirtualWidth/2)+DX2+DX; i < VirtualWidth-DEX; i+=2) {
		// Loop trhu the x components of the map.

			VirtualView[i+j*VirtualWidth]   = Buffer[k] + (Buffer[k+1] << 8);
			VirtualView[(i+1)+j*VirtualWidth] = Buffer[k+2] + (Buffer[k+3] << 8);
			k+=4;
		}
   }
  
   // Loading Map (1, 2)
   
   _llseek(LoadMap, (xMap+(yMap+YM)*(WorldWidth/(VirtualWidth/2)))*sizeof(DWORD), FILE_BEGIN);
   _lread(LoadMap, &Index, sizeof(DWORD));

	_llseek(LoadMap, Index, FILE_BEGIN);    // Change the position.
	_lread(LoadMap, Buffer2, VirtualWidth*VirtualHeight/2);

   RLE_Decompress((unsigned short *)Buffer2, (unsigned short *)Buffer, 128*128, 65536, LASTONE);
   
   for (j = (VirtualHeight/2)+DY2+DY; j < VirtualHeight-DEY; j++) {
	// Loop thru all y components.
		// Compute file position.
      k = (j-((VirtualHeight/2)+DY2+DY))*256;
      for (i = DX; i < (VirtualWidth/2)-DEX2+DX; i+=2) {
		// Loop trhu the x components of the map.

			VirtualView[i+j*VirtualWidth]   = Buffer[k] + (Buffer[k+1] << 8);
			VirtualView[(i+1)+j*VirtualWidth] = Buffer[k+2] + (Buffer[k+3] << 8);
			k+=4;
		}
	}

   // Loading Map (2, 2)
   
   _llseek(LoadMap, ((xMap+XM)+(yMap+YM)*(WorldWidth/(VirtualWidth/2)))*sizeof(DWORD), FILE_BEGIN);
   _lread(LoadMap, &Index, sizeof(DWORD));

   _llseek(LoadMap, Index, FILE_BEGIN);    // Change the position.
	_lread(LoadMap, Buffer2, VirtualWidth*VirtualHeight/2);

   RLE_Decompress((unsigned short *)Buffer2, (unsigned short *)Buffer, 128*128, 65536, LASTONE);
   
	for (j = (VirtualHeight/2)+DY2+DY; j < VirtualHeight-DEY; j++) {
	// Loop thru all y components.
		// Compute file position.
      k = (j-((VirtualHeight/2)+DY2+DY))*256;
		for (i = (VirtualWidth/2)+DX2+DX; i < VirtualWidth-DEX; i+=2) {
		// Loop trhu the x components of the map.

			VirtualView[i+j*VirtualWidth]   = Buffer[k] + (Buffer[k+1] << 8);
			VirtualView[(i+1)+j*VirtualWidth] = Buffer[k+2] + (Buffer[k+3] << 8);
			k+=4;
		}
	}
	

	char Temp2[100];
	sprintf(Temp2, "Game Files\\Map%d.Dat", World+1);
	
	HFILE f = _lopen(Temp2, OF_READ);
	unsigned short LoadMe = 0;
	unsigned char Buff = 0;
	
	COMM.LongLive();
	DWORD dwSquareLoad = 0;
	LOG << "* MAP = Load Start, ";
	DWORD LastSend = timeGetTime();

	do 
   {
		if (!_lread(f, &Buff, 1))
			break;
		
		LoadMe = (Buff << 8);
		_lread(f, &Buff, 1);
		LoadMe += Buff;
		LOG << (int)LoadMe << ", ";
      if (flpLoadSprite[LoadMe]) 
      {
         flpLoadSprite[LoadMe]();
      } 
      else 
      {
         if (flpLoadSprite2[LoadMe]) 
         {
            flpLoadSprite2[LoadMe]->Load();
         }
      }

		if (timeGetTime() - LastSend > 1000) {
			LastSend = timeGetTime();
			TFCPacket Send; 
			Send << (short)10;//RQ_Ack
			SEND_PACKET(Send);
			//Sleep(0);
		}

		dwSquareLoad++;

	} while (TRUE);
	LOG << "\r\n* MAP = Load End\r\n";

	_lclose(f);
  
   _lclose(LoadMap);
	//Sleep(100);

   // Loop pour trouver les decors animee...
   // on dois ajouter ses decors a la liste des items static
   // dans Visual object...
   
   
   int dwCnt = 0;
   if(g_SaveGame.GetOptionsParam()->bShowAnimDecorsLight)
   {
      for(int y=0;y<VirtualHeight;y++)
      {
         for(int x=0;x<VirtualWidth;x++)
         {
            if(VirtualView[dwCnt] == BIG_FONTAINE_1)
            {
               Objects.SDAnimAddObject( x+xLoad-DX,   y+yLoad-DY, World, 100, __OBJANIM_FONTAIN_02);
            }
            else if(VirtualView[dwCnt] == BIG_MOULIND)
            {
               Objects.SDAnimAddObject( x+xLoad-DX,   y+yLoad-DY, World, 100, __OBJANIM_MOULIND);
            }
            else if(VirtualView[dwCnt] == BIG_MOULING)
            {
               Objects.SDAnimAddObject( x+xLoad-DX,   y+yLoad-DY, World, 100, __OBJANIM_MOULING);
            }
            else if(VirtualView[dwCnt] == SPEC_LANTERNE1)
               Objects.SDAnimAddObject( x+xLoad-DX,   y+yLoad-DY, World, 100, __OBJLIGHT_LANTERNE1);
            else if(VirtualView[dwCnt] == SPEC_BIG_CHANDELLE || VirtualView[dwCnt] == SPEC_BIG_CHANDELLE2 || VirtualView[dwCnt] == SPEC_BIG_CHANDELLE3)
               Objects.SDAnimAddObject( x+xLoad-DX,   y+yLoad-DY, World, 100, __OBJLIGHT_CHANDELIER);
            else if(VirtualView[dwCnt] == SPEC_CHANDELLE_2 || VirtualView[dwCnt] == SPEC_CHANDELLE_2_I)
               Objects.SDAnimAddObject( x+xLoad-DX,   y+yLoad-DY, World, 100, __OBJLIGHT_CHANDELLE2);
            dwCnt++;
         }
      }
      Objects.AddSDAnimObject();
   }
   else
   {
      //Objects.Lock(0);
      //Objects.SDAnimClear();
      //Objects.Unlock(0);
   }
   


   delete []Buffer;
   delete []Buffer2;
}

void TileSet::ChangeStatic(void) {
   StaticState = true;
}

void TileSet::Save(void) {
   StaticSave = StaticState;
}

void TileSet::Restore(void) {
   StaticState = StaticSave;
}

void TileSet::DrawOverlap(int x, int y, int XC, int YC, int xPos, int yPos, Sprite *Over, int ID, INFO *Sp,UINT uiID=0) 
{
   int RealWidth;
   int RealHeight;
   int RealXCor;
   int RealYCor;
   
   int X = xPos;
   int Y = yPos;
   
   y+=1;
   x-=1;
   
   xPos += 16;
	yPos += 8;
	
	Sp->DrawOverlap = FALSE;

	int i, j, i2, j2, i3, j3;
	int lMo = 0, rMo = 0;
	int aB, bB;
	int RANGE = 15;
	int YVALUE = 2;
	int d = 0;
	int Chose = 0;
	
//	i  = Square+x+1;
//	j  = Square+y;
//	i2 = Square+x;
//	j2 = Square+y+1;
	i  = Square+x+1;
	j  = Square+y;
	i2 = Square+x;
	j2 = Square+y+1;
	i3 = Square+x-1;
	j3 = Square+y+2;
   if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1))
   if (i2 > 0 && i2 < (ViewWidth+Square+Square-1) && j2 > 0 && j2 < (ViewHeight+Square+Square-1))
   if (i3 > 0 && i3 < (ViewWidth+Square+Square-1) && j3 > 0 && j3 < (ViewHeight+Square+Square-1))
	if (((CompiledView[i+j*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i+j*(ViewWidth+Square+Square)] <= OBJECT_DO_OVERLAP_E)||
		  (CompiledView[i3+j3*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i3+j3*(ViewWidth+Square+Square)] <= OBJECT_DO_OVERLAP_E)) &&
	     (CompiledView[i2+j2*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i2+j2*(ViewWidth+Square+Square)] <= OBJECT_DO_OVERLAP_E))
		 rMo = 1;

	i  = Square+x+1;
	j  = Square+y;
	i2 = Square+x+2;
	j2 = Square+y+1;
   if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1))
   if (i2 > 0 && i2 < (ViewWidth+Square+Square-1) && j2 > 0 && j2 < (ViewHeight+Square+Square-1))
	if ((CompiledView[i+j*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i+j*(ViewWidth+Square+Square)] <= OBJECT_DO_OVERLAP_E) &&
	    (CompiledView[i2+j2*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i2+j2*(ViewWidth+Square+Square)] <= OBJECT_DO_OVERLAP_E))
		 lMo = 1;

	i  = Square+x+1;
	j  = Square+y+2;
   if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1))
	if (CompiledView[i+j*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i+j*(ViewWidth+Square+Square)] <= OBJECT_DO_OVERLAP_E)
		 Chose = 1;

	i  = Square+x+1;
	j  = Square+y+3;
   if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1))
	if (CompiledView[i+j*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i+j*(ViewWidth+Square+Square)] <= OBJECT_DO_OVERLAP_E)
		 Chose = 1;

	i  = Square+x+1;
	j  = Square+y+4;
   if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1))
	if (CompiledView[i+j*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i+j*(ViewWidth+Square+Square)] <= OBJECT_DO_OVERLAP_E)
		 Chose = 1;

	Chose = 0;

	RECT Clip;

	Clip.left = 0;
	Clip.top = 0;
	Clip.right = Over->Width;
	Clip.bottom = Over->Height;

	int xScreen, yScreen;
	
	DDSURFACEDESC Sprite;
	memset(&Sprite, 0, sizeof(Sprite));
	Sprite.dwSize = sizeof(Sprite);	
   DXDErrorHandle(Over->GetSurface()->Lock(NULL, &Sprite, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL), "Draw Overlap");

   for (i = 0; i < Sprite.lPitch*Over->Height/2; i++)
      ((LPWORD)Sprite.lpSurface)[i] = (WORD)Over->TransColor;
   LPWORD p = (LPWORD)Sprite.lpSurface;
   
   RealWidth = Sp->Width;
   RealHeight = Sp->Height;
   RealXCor = Sp->CX;
   RealYCor = Sp->CY;
   
   if (Over->Width < RealWidth)
      RealWidth = Over->Width;
   if (Over->Height < RealHeight)
      RealHeight = Over->Height;
   if (Over->xCor > RealXCor)
      RealXCor = Over->xCor;
   if (Over->yCor > RealYCor)
      RealYCor = Over->yCor;

   if (RealXCor - Over->xCor + RealWidth > Over->Width)
      RealXCor = Over->Width - RealWidth + Over->xCor;
   if (RealYCor - Over->yCor + RealHeight > Over->Height)
      RealYCor = Over->Height - RealHeight + Over->yCor;


   /*
   PosXY(50, 50)
   BigS(100, 100);
   BigC(-25, -25);
   RelS(50, 50);
   RelC(-15, -15);
   */
   
/*   for (j = 0; j < Over->Height; j++) {
      for (i = 0; i < Over->Width; i++) {
        p[i+j*Sprite.lPitch] = 2;
      }
   }
*/  
/*   for (j = 0; j < RealHeight; j++) {
      for (i = 0; i < RealWidth; i++) {
        p[i+RealXCor-Over->xCor+((RealYCor-Over->yCor+j)*Sprite.lPitch)] = 0;
      }
   } */

	Over->GetSurface()->Unlock(Sprite.lpSurface);
	
	y+=Square;
	i  = Square+x+1;
	j  = y+2;
	xScreen = (ViewWidth+Square-1)*TileWidth+XC-(((ViewWidth+Square*2)-(x+Square+1))*TileWidth);//+(TileWidth>>1); // Init starting x sprite position
	xScreen -= (ViewWidth-SizeWidth)*16;
	yScreen = -(Square*TileHeight)+YC+((y+1)*TileHeight);//+(TileHeight>>1); // Init starting y sprite position.
	yScreen -= (ViewHeight-SizeHeight)*8;

   if (Chose) {

	//if (j == 1+y && lMo == 1)
	//	aB = x+1+Square;
	//else
		aB = x+Square+1+RANGE;

	//if (j == 1+y && rMo == 1)
	//	bB = Square+x;
	//else
		bB = x+Square+1-RANGE-1;
		
	yScreen = -(Square*TileHeight)+YC+((y-RANGE)*TileHeight);//+(TileHeight>>1); // Init starting y sprite position.
	yScreen -= (ViewHeight-SizeHeight)*8;

   for (j = 1+y-RANGE; j < 1+y+Square+YVALUE; j++) {
		xScreen = (ViewWidth+Square-1)*TileWidth+XC-(((ViewWidth+Square*2)-(aB))*TileWidth);//+(TileWidth>>1); // Init starting x sprite position
		xScreen -= (ViewWidth-SizeWidth)*16;

		for (i = aB; i > bB; i--) {
		// Loop thru all the x component.
			if ((CompiledView[i+j*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i+j*(ViewWidth+Square+Square)] <= OBJECT_DO_OVERLAP_E) &&
				(i >= (aB+1)-j+1+y-RANGE || i <= (bB)+j-1-y+RANGE)) {

				a = i;
				b = j;
				DrawTileSet(xScreen-xPos, yScreen-yPos, CompiledView[i+j*(ViewWidth+Square+Square)], &Clip, NULL, NULL, false, Over->GetSurface(), true, FALSE,uiID);
				Sp->DrawOverlap = TRUE;
			}

			xScreen -= TileWidth;
		}
		yScreen += TileHeight;
	}
   } else {


   yScreen = -(Square*TileHeight)+YC+((y)*TileHeight);//+(TileHeight>>1); // Init starting y sprite position.
	yScreen -= (ViewHeight-SizeHeight)*8;

   for (j = 1+y; j < 1+y+Square+YVALUE; j++) {
	// Loop thru all the y component.
      
		if (j == 1+y && lMo == 1)
			aB = x+1+Square;
		else
			aB = x+Square+1+RANGE;

		if (j == 1+y && rMo == 1)
			bB = Square+x;
		else
			bB = x+Square+1-RANGE-1;

		xScreen = (ViewWidth+Square-1)*TileWidth+XC-(((ViewWidth+Square*2)-(aB))*TileWidth);//+(TileWidth>>1); // Init starting x sprite position
		xScreen -= (ViewWidth-SizeWidth)*16;
      for (i = aB; i > bB; i--) 
      {
		// Loop thru all the x component.
		   a = i;
			b = j;
         if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1))
			   if (CompiledView[i+j*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i+j*(ViewWidth+Square+Square)] <= OBJECT_DO_OVERLAP_E) 
            {
				   if (
               
                  // le coin en haut a gauche c'est (0, 0)
                  // le [j] est la position variable en y dans le carrer visible de l'ecran
                  // le [i] est la position variable en x dans le carrer visible de l'ecran
                  // le [y+1] est la position absolue de la unit dans le carrer de l'ecran en y 
                  // le [x+square+1] est la position absolue de la unit dans le carrer de l'ecran en x.
               
                  // Ici on coupe l'overlapping si le dessins d'overlapping [j] est audessus de la case sur 
                  // les pieds de la unit [y+1]
                  j <= 1+y  ||
                  (-(i-(Square+1+x)) > (j-y-1) && CompiledView[i+j*(ViewWidth+Square+Square)] > SPECIAL_NE && 
                     CompiledView[i+j*(ViewWidth+Square+Square)] < SPECIAL_NW) ||
					   ((i-(Square+1+x)) > (j-y-1) && CompiledView[i+j*(ViewWidth+Square+Square)] > SPECIAL_NW && 
                     CompiledView[i+j*(ViewWidth+Square+Square)] < OBJECT_NORMAL) ||
		      	   (Abs((signed)(i-(Square+x+1))) <= 2 && i-(Square+x+1) != 0 &&
					      CompiledView[i+j*(ViewWidth+Square+Square)] == COLONNE_GREQUE)
                  )
				   {
            
               }
				   else 
               {
					   DrawTileSet(xScreen-xPos, yScreen-yPos, CompiledView[i+j*(ViewWidth+Square+Square)], &Clip, NULL, NULL, false, Over->GetSurface(), true, FALSE,uiID);
					   Sp->DrawOverlap = TRUE;
				   }

			   }

			xScreen -= TileWidth;
		}
      yScreen += TileHeight;
	}
   }
}

void TileSet::DecorDrawOverlap(int x, int y, int XC, int YC, int xPos, int yPos, LPDIRECTDRAWSURFACE lpBlitSurface, int ID, INFO *Sp,UINT uiID,RECT *pRc) 
{
   int X = x;
   int Y = y;
   
   y+=1;
   x-=1;
   
   xPos -= 32;
	yPos += 21;

   
	
	int i, j;
	int lMo = 0, rMo = 0;
	int aB, bB;
	int RANGE = 8;
	int YVALUE = 2;
	int d = 0;
	int Chose = 0;
	
	Chose = 0;

	RECT Clip;

   /*
	Clip.left = 0;
	Clip.top = 0;
	Clip.right = g_Global.GetScreenW();
	Clip.bottom = g_Global.GetScreenH();
   */
   
   Clip.left   = pRc->left;
	Clip.top    = pRc->top;
	Clip.right  = pRc->right;
	Clip.bottom = pRc->bottom;

   if(Clip.right > g_Global.GetScreenW())
      Clip.right = g_Global.GetScreenW()-1;
   if(Clip.left  < 0)
      Clip.left = 0;

   if(Clip.bottom > g_Global.GetScreenH())
      Clip.bottom = g_Global.GetScreenH()-1;
   if(Clip.top  < 0)
      Clip.top = 0;

   if(Clip.bottom <  0                     || Clip.right < 0                    || 
      Clip.top    >= g_Global.GetScreenH() || Clip.left  > g_Global.GetScreenW()  )
      return ;


	int xScreen, yScreen;
 	
	y  +=Square;
	i  = Square+x+1;
	j  = y+2;


   static int YCC = -1;

   int ViewMaxSize = (ViewWidth+Square+Square) * (ViewHeight+Square+Square);
   

   {
      yScreen = (-(Square*TileHeight))+((y+Y)*TileHeight);//+(TileHeight>>1); // Init starting y sprite position.
      yScreen -= (ViewHeight-SizeHeight)*8;
     
      int yLimit = 1+y+Square+YVALUE;
      //if(yLimit >99)
      //   yLimit = 99;

      int dwCnt = 0;
      for (j = 1+y; j < yLimit; j++,dwCnt++) 
      {
         // Loop thru all the y component.
         
         if (j == 1+y && lMo == 1)
            aB = x+1+Square;
         else
            aB = x+Square+1+RANGE;
         
         if (j == 1+y && rMo == 1)
            bB = Square+x;
         else
            bB = x+Square+1-RANGE-1;
         
         xScreen = (ViewWidth+Square-1)*TileWidth+0-(((ViewWidth+Square*2)-(aB+X))*TileWidth);//+(TileWidth>>1); // Init starting x sprite position
         xScreen -= (ViewWidth-SizeWidth)*16;

         int DrawMe = 0;
         for (i = aB; i > bB; i--) 
         {
            // Loop thru all the x component.
            a = i;
            b = j;
            if (i+j*(ViewWidth+Square+Square) < ViewMaxSize)
            {
               if(CompiledView[i+j*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i+j*(ViewWidth+Square+Square)] < OBJECT_DO_OVERLAP_E)// ROUTE_ROCHE)
               {
				      DrawMe = CompiledView[i+j*(ViewWidth+Square+Square)];//WATER;
                  //OutputDebugString("DecorDrawOverlap\n");
                  DrawTileSet(xScreen-xPos+XC, yScreen-yPos+YC, DrawMe,&Clip, NULL, NULL, false, lpBlitSurface, false, FALSE);
               }
            }
            xScreen -= TileWidth;
         }
         yScreen += TileHeight;
      }
   }
}

void TileSet::DecorDrawOverlapID(int x, int y, int XC, int YC, int xPos, int yPos, LPDIRECTDRAWSURFACE lpBlitSurface, int ID, INFO *Sp,UINT uiID,RECT *pRc)
{
   int X = x;
   int Y = y;
   
   y+=1;
   x-=1;
   
   xPos -= 32;
	yPos += 21;

   
	
	int i, j;
	int lMo = 0, rMo = 0;
	int aB, bB;
	int RANGE = 8;
	int YVALUE = 2;
	int d = 0;
	int Chose = 0;
	
	Chose = 0;

	RECT Clip;

	Clip.left   = pRc->left;
	Clip.top    = pRc->top;
	Clip.right  = pRc->right;
	Clip.bottom = pRc->bottom;

   if(Clip.right > g_Global.GetScreenW())
      Clip.right = g_Global.GetScreenW()-1;
   if(Clip.left  < 0)
      Clip.left = 0;

   if(Clip.bottom > g_Global.GetScreenH())
      Clip.bottom = g_Global.GetScreenH()-1;
   if(Clip.top  < 0)
      Clip.top = 0;

   if(Clip.bottom <  0                     || Clip.right < 0                    || 
      Clip.top    >= g_Global.GetScreenH() || Clip.left  > g_Global.GetScreenW()  )
      return ;

	int xScreen, yScreen;
	
	
 	
	y  +=Square;
	i  = Square+x+1;
	j  = y+2;


   static int YCC = -1;
   int ViewMaxSize = (ViewWidth+Square+Square) * (ViewHeight+Square+Square);
   

   {
      yScreen = (-(Square*TileHeight))+((y+Y)*TileHeight);//+(TileHeight>>1); // Init starting y sprite position.
      yScreen -= (ViewHeight-SizeHeight)*8;
     
      int yLimit = 1+y+Square+YVALUE;
      //if(yLimit >99)
      //   yLimit = 99;

      int dwCnt = 0;
      for (j = 1+y; j < yLimit; j++,dwCnt++) 
      {
         // Loop thru all the y component.
         
         if (j == 1+y && lMo == 1)
            aB = x+1+Square;
         else
            aB = x+Square+1+RANGE;
         
         if (j == 1+y && rMo == 1)
            bB = Square+x;
         else
            bB = x+Square+1-RANGE-1;
         
         xScreen = (ViewWidth+Square-1)*TileWidth+0-(((ViewWidth+Square*2)-(aB+X))*TileWidth);//+(TileWidth>>1); // Init starting x sprite position
         xScreen -= (ViewWidth-SizeWidth)*16;

         int DrawMe = 0;
         for (i = aB; i > bB; i--) 
         {
            // Loop thru all the x component.
            a = i;
            b = j;
            if (i+j*(ViewWidth+Square+Square) < ViewMaxSize)
            {
               if(CompiledView[i+j*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i+j*(ViewWidth+Square+Square)] < OBJECT_DO_OVERLAP_E)// ROUTE_ROCHE)
               {
				      DrawMe = CompiledView[i+j*(ViewWidth+Square+Square)];//WATER;
                  DrawTileSet(xScreen-xPos+XC, yScreen-yPos+YC, DrawMe,&Clip, NULL, NULL, false, lpBlitSurface, false, FALSE);
               }
            }
            xScreen -= TileWidth;
         }
         yScreen += TileHeight;
      }
   }
}

void TileSet::Lock(int i) {
	csLock.Lock("");
   g_TimeStat.StartBlockCount(GetCurrentThreadId(), i);
}

void TileSet::Unlock(int i) {
   g_TimeStat.StopBlockCount(GetCurrentThreadId(), i);
	csLock.Unlock("");
}

void TileSet::SetPosition(int NewX, int NewY, int NewWorld, BOOL F,BOOL ForceName) {
// Set the new x,y position.
	int i, j;
	int xScreen, yScreen;
	RECT Clip;

   SepX = SepX2  = 0;
   SepY = SepY2  = 0;
   Middle = 0;
   MovX = MovY = Done = 0;

	xPos = (VirtualWidth  >> 1);
	yPos = (VirtualHeight >> 1);
	wPos = NewWorld;

   if(ForceName)
   {
      // on commence par loader la map du new world...
      g_Global.LoadZoneMapWorld(wPos,NewX,NewY);
      g_Global.ForceDisplayZone(NewX,NewY);
   }

	LoadVirtualMap(NewX - xPos, NewY - yPos, NewWorld); 

	// Initialise 
	for (i = 0; i < ViewWidth+Square+Square; i++) {
		for (j = 0; j < ViewHeight+Square+Square; j++) {
			CompiledView[i+j*(ViewWidth+Square+Square)] = VirtualView[i-1-(ViewWidth>>1)-Square+xPos+(j-2-(ViewHeight>>1)-Square+yPos)*VirtualWidth];
		}
	}

	Clip.left = 0;
	Clip.top = 0;
	Clip.right = g_Global.GetScreenW()+96; //TimeWith*3
	Clip.bottom = g_Global.GetScreenH()+48;//TileHeight*3
	
	yScreen = -(Square*TileHeight);//+(TileHeight>>1); // Init starting y sprite position.
	for (j = 0; j < ViewHeight+Square+Square; j++) {
	// Loop thru all the y component.
		
		xScreen = (ViewWidth+Square-1)*TileWidth;//+(TileWidth>>1); // Init starting x sprite position
		for (i = ViewWidth+Square+Square-1; i > -1; i--) {
		// Loop thru all the x component.
			a = i;
			b = j;
			if (CompiledView[i+(j-Middle)*(ViewWidth+Square+Square)] < SPECIAL_S || CompiledView[i+(j-Middle)*(ViewWidth+Square+Square)] >SPECIAL_E) 
				DrawTileSet(xScreen, yScreen, CompiledView[i+j*(ViewWidth+Square+Square)], &Clip, NULL, NULL, false, lpFractalSurface, false, FALSE);
         else if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1)) {
            if (DrawTileSet(xScreen, yScreen, CompiledView[(i)+(j+1)*(ViewWidth+Square+Square)], &Clip, NULL, NULL, true, lpFractalSurface, false , FALSE)) {} 
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i-1)+(j+1)*(ViewWidth+Square+Square)], &Clip, NULL, NULL, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i+1)+(j+1)*(ViewWidth+Square+Square)], &Clip, NULL, NULL, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i-1)+(j)*(ViewWidth+Square+Square)], &Clip, NULL, NULL, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i-1)+(j-1)*(ViewWidth+Square+Square)], &Clip, NULL, NULL, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i)+(j-1)*(ViewWidth+Square+Square)], &Clip, NULL, NULL, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i+1)+(j-1)*(ViewWidth+Square+Square)], &Clip, NULL, NULL, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i+1)+(j)*(ViewWidth+Square+Square)], &Clip, NULL, NULL, true, lpFractalSurface, false, FALSE)) {}
         }

			xScreen -= TileWidth;
		}
		yScreen += TileHeight;
	}

	yScreen = -(Square*TileHeight);//+(TileHeight>>1); // Init starting y sprite position.
	for (j = 0; j < ViewHeight+Square+Square; j++) {
	// Loop thru all the y component.
		
		xScreen = (ViewWidth+Square-1)*TileWidth;//+(TileWidth>>1); // Init starting x sprite position
		for (i = ViewWidth+Square+Square-1; i > -1; i--) {
		// Loop thru all the x component.
			a = i;
			b = j;
			if (CompiledView[i+(j-Middle)*(ViewWidth+Square+Square)] > SPECIAL_S && CompiledView[i+(j-Middle)*(ViewWidth+Square+Square)] <= SPECIAL_E) {
				DrawTileSet(xScreen, yScreen, CompiledView[i+j*(ViewWidth+Square+Square)], &Clip, NULL, NULL, false, lpFractalSurface, false, FALSE);
			}

			xScreen -= TileWidth;
		}
		yScreen += TileHeight;
	}

	ScreenDest.left         = 0;
	ScreenDest.top          = 0;
	ScreenDest.right        = (ViewWidth-1) *TileWidth;
	ScreenDest.bottom       = (ViewHeight-1)*TileHeight;
	
	SmoothSource.left       = TileWidth+TileWidth/2;
	SmoothSource.top        = TileHeight+TileHeight/2;
	SmoothSource.right      = TileWidth *ViewWidth +TileWidth-TileWidth/2;
	SmoothSource.bottom     = TileHeight*ViewHeight+TileHeight-TileHeight/2;

	SurfaceDest.left        = 0;
	SurfaceDest.top         = 0;
	SurfaceDest.right       = TileWidth *ViewWidth +TileWidth *2;
	SurfaceDest.bottom      = TileHeight*ViewHeight+TileHeight*2;

	SurfaceSource.left      = 0;
	SurfaceSource.top       = 0;
	SurfaceSource.right     = TileWidth *ViewWidth +TileWidth *2;
	SurfaceSource.bottom    = TileHeight*ViewHeight+TileHeight*2;

	FractalSource[0].left   = TileWidth;
	FractalSource[0].top    = TileHeight;
	FractalSource[0].right  = TileWidth *ViewWidth +TileWidth*2;
	FractalSource[0].bottom = TileHeight*ViewHeight+TileHeight*2;

   SepX = SepX2 = 0;
   SepY = SepY2 = 0;

		DestRect[0].left     = 0;
		DestRect[0].top      = 0;
		DestRect[0].right    = ViewWidth *TileWidth +TileWidth *2-SepX;
		DestRect[0].bottom   = ViewHeight*TileHeight+TileHeight*2-SepY;

		DestRect[1].left     = 0;
		DestRect[1].top      = ViewHeight*TileHeight+TileHeight*2-SepY;
		DestRect[1].right    = ViewWidth *TileWidth +TileWidth *2-SepX;
		DestRect[1].bottom   = ViewHeight*TileHeight+TileHeight*2;

		DestRect[2].left     = ViewWidth *TileWidth +TileWidth *2-SepX;
		DestRect[2].top      = 0;
		DestRect[2].right    = ViewWidth *TileWidth +TileWidth *2;
		DestRect[2].bottom   = ViewHeight*TileHeight+TileHeight*2-SepY;

		DestRect[3].left     = ViewWidth *TileWidth +TileWidth *2-SepX;
		DestRect[3].top      = ViewHeight*TileHeight+TileHeight*2-SepY;
		DestRect[3].right    = ViewWidth *TileWidth +TileWidth *2;
		DestRect[3].bottom   = ViewHeight*TileHeight+TileHeight*2;

		SourceRect[0].left   = SepX;
		SourceRect[0].top    = SepY;
		SourceRect[0].right  = TileWidth *ViewWidth +TileWidth *2;
		SourceRect[0].bottom = TileHeight*ViewHeight+TileHeight*2;

		SourceRect[1].left   = SepX;
		SourceRect[1].top    = 0;
		SourceRect[1].right  = TileWidth*ViewWidth+TileWidth*2;
		SourceRect[1].bottom = SepY;

		SourceRect[2].left   = 0;
		SourceRect[2].top    = SepY;
		SourceRect[2].right  = SepX;
		SourceRect[2].bottom = TileHeight*ViewHeight+TileHeight*2;

		SourceRect[3].left   = 0; 
		SourceRect[3].top    = 0;
		SourceRect[3].right  = SepX;
		SourceRect[3].bottom = SepY;
	lpSmoothSurface2->Blt(&SurfaceDest, lpFractalSurface, &SurfaceSource, DDBLT_WAIT, NULL);

	Objects.Lock(1011);
	INFO *Return = Objects.VObject3D[Objects.Set]->GetCurrentVSF(Objects.GetMainObject()->Direction, 0, Objects.GetMainObject()->Standing);
   /**/DrawOverlap(Objects.ViewWidth, Objects.ViewHeight, 0, 0, (Objects.ViewWidth*32)+Objects.Overlap.xCor-48, (Objects.ViewHeight*16)+Objects.Overlap.yCor-24+37, &Objects.Overlap, 1, Return,99999998);
   delete Return;
	Objects.Unlock(1011);

   if (F) {
      FadeLevel = 0;
		SetFading = true;
   }
}


void TileSet::CreateView(void) {
// Allocate the View Memory.
//#ifdef _DEBUG
   if (ViewWidth && ViewHeight && VirtualWidth && VirtualHeight) {
//#endif
	// If View is Specified.
//		delete CompiledView;
		delete VirtualView;

//		CompiledView = new unsigned short [(ViewWidth+Square+Square) * (ViewHeight+Square+Square)];
      CompiledView.Alloc((ViewWidth+Square+Square) * (ViewHeight+Square+Square));
		VirtualView  = new unsigned short [VirtualWidth  * VirtualHeight];
	
		DDSURFACEDESC LandType;
	
		// Create LandType Surface.
  	   ZeroMemory(&LandType, sizeof(LandType));
  	   LandType.dwSize            = sizeof(LandType);
		LandType.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		LandType.ddsCaps.dwCaps    = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		LandType.dwWidth           = (ViewWidth+2)  * TileWidth;
		LandType.dwHeight          = (ViewHeight+2) * TileHeight;

      lpDXDirectDraw->lpdd->CreateSurface(&LandType, &lpSmoothSurface2, NULL);
	   lpDXDirectDraw->lpdd->CreateSurface(&LandType, &lpFractalSurface, NULL);

		LandType.dwWidth           = g_Global.GetScreenW();
		LandType.dwHeight          = g_Global.GetScreenH();
      lpDXDirectDraw->lpdd->CreateSurface(&LandType, &lpPreLightSurface, NULL);

		
		// OBJECT QUI SONT LOADER ET QUI NE SERVENT PRESENTEMENT A RIEN
		// ShinglesTop 1-4
		// ShinglesSide 1-4
		// ShinglesUnseenSide 1-4
		// ShinglesBorders*.*
		// Ambiguiter GRASS/CLEANGRASS
		// Grass Rock <=== FAIRE ATTENTION
		// Grass Earth 
		// Denivelation*.*
		// Water Denivelation

		// OBJECT QUI SONT LOADER ET QUI N'ON PAS DE REFERENCE DANS LA MAP
		// Ambiguiter GRASS/CLEANGRASS
	
	 	ZeroMemory(flpLoadSprite, sizeof(flpLoadSprite));
		Load20();
		flpLoadSprite[MOUNTAIN_NEW_12]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_13]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_14]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_21]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_23]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_24]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_31]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_32]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_34]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_41]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_42]				= Load1;
		flpLoadSprite[MOUNTAIN_NEW_43]				= Load1;
		flpLoadSprite[MOUNTAIN_2]						= Load1;
		flpLoadSprite[MOUNTAIN_2_I]					= Load1;
		flpLoadSprite[MOUNTAIN_NE_1]					= Load1;
		flpLoadSprite[MOUNTAIN_NE_2]					= Load1;
		flpLoadSprite[MOUNTAIN_NE_3]					= Load1;
		flpLoadSprite[MOUNTAIN_NE_4]					= Load1;
		flpLoadSprite[MOUNTAIN_NW_1]					= Load1;
		flpLoadSprite[MOUNTAIN_NW_2]					= Load1;
		flpLoadSprite[MOUNTAIN_NW_3]					= Load1;
		flpLoadSprite[MOUNTAIN_NW_4]					= Load1;
		flpLoadSprite[MOUNTAIN_LEFT]					= Load1;
		flpLoadSprite[MOUNTAIN_RIGHT]					= Load1;
		flpLoadSprite[CAVERN_ENTRANCE]				= Load240;
		flpLoadSprite[CAVERN_ENTRANCE_NE]			= Load1;
		flpLoadSprite[CAVERN_ENTRANCE_NW] 			= Load1;
		flpLoadSprite[GRASS]								= Load27;
		flpLoadSprite[GRASS_BREAK1]					= Load27;
		flpLoadSprite[GRASS_BREAK2]					= Load27;
		flpLoadSprite[DUNGEON_FLOOR]					= Load28;
		flpLoadSprite[DUNGEON_LIGHT]					= Load29;
		flpLoadSprite[DUNGEON_LIGHT2]					= Load30;
		flpLoadSprite[RIVER]								= Load31;
		flpLoadSprite[CAVERN]							= Load32;
		flpLoadSprite[WOODEN_FLOOR]					= Load33;
      flpLoadSprite[WOODEN_FLOOR2]  				= Load7000;
      flpLoadSprite[WOODEN_FLOOR3]					= Load7001;
		flpLoadSprite[DUNGEONOVERHEAD]				= Load34;
		flpLoadSprite[DUNGEONOVERHEAD2]				= Load35;
		flpLoadSprite[DESERT]							= Load14;
		flpLoadSprite[WATER]	    						= Load31;
		flpLoadSprite[ICE]	    						= Load19;
		flpLoadSprite[SUPER_BLACK]						= Load20;
		flpLoadSprite[DGRASS]							= Load21;
		flpLoadSprite[FOREST]							= Load22;
		flpLoadSprite[CHEMIN]							= Load23;
		flpLoadSprite[EARTH]								= Load23;
		flpLoadSprite[ROUTE_DALLER]					= Load24;
		flpLoadSprite[ROUTE_ROCHE]						= Load26;
		flpLoadSprite[MOUNTAIN_ROAD]					= Load2;
		flpLoadSprite[SPEC_NEWTRUNK]					= Load3;
		flpLoadSprite[SPEC_DISPLAY_1]					= Load4;
		flpLoadSprite[SPEC_DISPLAY_2]					= Load5;
		flpLoadSprite[SPEC_DISPLAY_3]					= Load6;
		flpLoadSprite[SPEC_DISPLAY_2_I]				= Load5;
		flpLoadSprite[SPEC_DISPLAY_3_I]				= Load6;
		flpLoadSprite[SPEC_WOODPILE]					= Load7;
		flpLoadSprite[SPEC_KNIGHT]						= Load8;
		flpLoadSprite[SPEC_WEB_1]						= Load9;
		flpLoadSprite[SPEC_WEB_2]						= Load10;
		flpLoadSprite[SPEC_WEB_3]						= Load11;
		flpLoadSprite[SPEC_WEB_4]						= Load12;
		flpLoadSprite[SPEC_WEB_5]						= Load13;
		flpLoadSprite[SPEC_WEB_6]						= Load15;
		flpLoadSprite[SPEC_WEB_7]						= Load16;
		flpLoadSprite[SPEC_WEB_8]						= Load17;
		flpLoadSprite[SPEC_WEB_9]						= Load18;
		flpLoadSprite[SPEC_WEB_10]						= Load25;
		flpLoadSprite[SPEC_WEB_1_I]					= Load9;
		flpLoadSprite[SPEC_WEB_2_I]					= Load10;
		flpLoadSprite[SPEC_WEB_3_I]					= Load11;
		flpLoadSprite[SPEC_WEB_4_I]					= Load12;
		flpLoadSprite[SPEC_WEB_5_I]					= Load13;
		flpLoadSprite[SPEC_WEB_6_I]					= Load15;
		flpLoadSprite[SPEC_WEB_7_I]					= Load16;
		flpLoadSprite[SPEC_WEB_8_I]					= Load17;
		flpLoadSprite[SPEC_WEB_9_I]					= Load18;
		flpLoadSprite[SPEC_WEB_10_I]					= Load25;
		flpLoadSprite[SPEC_CAVERN_I_ENTRANCE]		= Load36;
		flpLoadSprite[SPEC_CAVERN_I_ENTRANCE_I]	= Load36;
		flpLoadSprite[STAIRS_1]							= Load37;
		flpLoadSprite[STAIRS_1_I]						= Load37;
		flpLoadSprite[STAIRS_2]							= Load38;
		flpLoadSprite[STAIRS_2_I]						= Load38;
		flpLoadSprite[STAIRSUP_1]						= Load39;
		flpLoadSprite[STAIRSUP_1_I]					= Load39;
		flpLoadSprite[STAIRSUP_2]						= Load40;
		flpLoadSprite[STAIRSUP_2_I]					= Load40;
		flpLoadSprite[NEWROCK_1]						= Load41;
		flpLoadSprite[NEWROCK_2]						= Load42;
		flpLoadSprite[NEWROCK_3]						= Load43;
		flpLoadSprite[NEWROCK_4]						= Load44;
		flpLoadSprite[GRAROC2_N]						= Load46;
		flpLoadSprite[GRAROC2_S]						= Load46;
		flpLoadSprite[GRAROC2_E]						= Load46;
		flpLoadSprite[GRAROC2_W]						= Load46;
		flpLoadSprite[GRAROC2_NE]						= Load46;
		flpLoadSprite[GRAROC2_SE]						= Load46;
		flpLoadSprite[GRAROC2_SW]						= Load46;
		flpLoadSprite[GRAROC2_NW]						= Load46;
		flpLoadSprite[ROCFLO_1]							= Load47;
		flpLoadSprite[ROCFLO_2]							= Load47;
		flpLoadSprite[ROCFLO_3]							= Load47;
		flpLoadSprite[ROCFLO_4]							= Load47;
		flpLoadSprite[RIVWAT_1]							= Load50;
		flpLoadSprite[RIVWAT_2]							= Load50;
		flpLoadSprite[RIVWAT_3]							= Load50;
		flpLoadSprite[RIVWAT_4]							= Load50;
		flpLoadSprite[RIVWAT_5]							= Load50;
		flpLoadSprite[RIVWAT_6]							= Load50;
		flpLoadSprite[SIMPLE_BED_1_SE]				= Load51;
		flpLoadSprite[SIMPLE_BED_1_NE]				= Load51;
		flpLoadSprite[DOUBLE_BED_1_SE]				= Load52;
		flpLoadSprite[DOUBLE_BED_1_NE]				= Load52;
		flpLoadSprite[DOUBLE_BED_2_SE]				= Load53;
		flpLoadSprite[DOUBLE_BED_2_NE]				= Load53;
		flpLoadSprite[WOODEN_ARMOIR_SE]				= Load54;
		flpLoadSprite[WOODEN_ARMOIR_SW]				= Load54;
		flpLoadSprite[WOODEN_ARMOIR_NE]				= Load54;
		flpLoadSprite[WOODEN_ARMOIR_NW]				= Load54;
		flpLoadSprite[ETAGERE_SE]						= Load55;
		flpLoadSprite[ETAGERE_SW]						= Load55;
		flpLoadSprite[ETAGERE_NE]						= Load55;
		flpLoadSprite[ETAGERE_NW]						= Load55;
		flpLoadSprite[SPEC_BIBLIOTHEQUE]				= Load56;
		flpLoadSprite[SPEC_BIBLIOTHEQUE_I]			= Load56;
		flpLoadSprite[SPEC_LITTLE_BIBLIOTHEQUE]	= Load57;
		flpLoadSprite[SPEC_LITTLE_BIBLIOTHEQUE_I]	= Load57;
		flpLoadSprite[BUREAU_SE]						= Load58;
		flpLoadSprite[BUREAU_SW]						= Load58;
		flpLoadSprite[BUREAU_NE]						= Load58;
		flpLoadSprite[BUREAU_NW]						= Load58;
		flpLoadSprite[REDWOODEN_ARMOIR_SE]			= Load59;
		flpLoadSprite[REDWOODEN_ARMOIR_SW]			= Load59;
		flpLoadSprite[REDWOODEN_ARMOIR_NE]			= Load59;
		flpLoadSprite[REDWOODEN_ARMOIR_NW]			= Load59;
		flpLoadSprite[COLONNE_CE_B]					= Load60;
		flpLoadSprite[COLONNE_CE_BH]					= Load61;
		flpLoadSprite[COLONNE_CA_BH]					= Load62;
		flpLoadSprite[COLONNE_GREQUE]					= Load63;
		flpLoadSprite[SPEC_TABLEAU]					= Load64;
		flpLoadSprite[SPEC_TABLEAU_I]					= Load64;
		flpLoadSprite[SPEC_HEADPILE]					= Load65;
		flpLoadSprite[SPEC_HEADPILE_I]				= Load65;
		flpLoadSprite[SPEC_SKELETON_CORPSE]			= Load66;
		flpLoadSprite[SPEC_MANACLE1]					= Load67;
		flpLoadSprite[SPEC_MANACLE1_I]				= Load67;
		flpLoadSprite[SPEC_LITTLE_TREE]				= Load68;
		flpLoadSprite[SPEC_DUNGEON_TOMB]				= Load69;
		flpLoadSprite[SPEC_DUNGEON_TOMB_I]			= Load69;
		flpLoadSprite[SPEC_BLOOD]						= Load70;
		flpLoadSprite[SPEC_SWORD]						= Load71;
		flpLoadSprite[SPEC_SWORD_I]					= Load71;
		flpLoadSprite[SPEC_DUNGEON_WELL]				= Load72;
		flpLoadSprite[SPEC_BIG_DUNGEON_WELL]		= Load73;
		flpLoadSprite[SPEC_SKEL_HEAD]					= Load74;
		flpLoadSprite[SPEC_DEBRIS_1]					= Load75;
		flpLoadSprite[SPEC_DEBRIS_1_I]				= Load75;
		flpLoadSprite[SPEC_DEBRIS_2]					= Load76;
		flpLoadSprite[SPEC_DEBRIS_2_I]				= Load76;
		flpLoadSprite[SPEC_DEBRIS_3]					= Load77;
		flpLoadSprite[SPEC_DEBRIS_3_I]				= Load77;
		flpLoadSprite[SPEC_DEBRIS_4]					= Load78;
		flpLoadSprite[SPEC_DEBRIS_4_I]				= Load78;
		flpLoadSprite[SPEC_ANVIL]						= Load79;
		flpLoadSprite[SPEC_ANVIL_I]					= Load79;
		flpLoadSprite[SPEC_DEBRIS_WALL]				= Load80;
		flpLoadSprite[SPEC_DEBRIS_WALL_I]			= Load80;
		flpLoadSprite[SPEC_FIREPLACE]					= Load81;
		flpLoadSprite[SPEC_FIREPLACE_I]				= Load81;
		flpLoadSprite[SPEC_OVEN]						= Load82;
		flpLoadSprite[SPEC_OVEN_I]						= Load82;
		flpLoadSprite[SPEC_RUINED_BED]				= Load83;
		flpLoadSprite[SPEC_RUINED_BED_I]				= Load83;
		flpLoadSprite[SPEC_DESK_1]						= Load84;
		flpLoadSprite[SPEC_DESK_1_I]					= Load84;
		flpLoadSprite[SPEC_DESK_2]						= Load85;
		flpLoadSprite[SPEC_DESK_2_I]					= Load85;
		flpLoadSprite[SPEC_CEREBUS]					= Load86;
		flpLoadSprite[SPEC_CEREBUS_I]					= Load86;
		flpLoadSprite[SPEC_MONK]						= Load87;
		flpLoadSprite[SPEC_MONK_I]						= Load87;
		flpLoadSprite[SPEC_LUTRIN]						= Load88;
		flpLoadSprite[SPEC_LUTRIN_I]					= Load88;
		flpLoadSprite[SPEC_THRONE]						= Load89;
		flpLoadSprite[SPEC_THRONE_I]					= Load89;
		flpLoadSprite[SPEC_CRYPT]						= Load90;
		flpLoadSprite[SPEC_CRYPT_I]					= Load90;
		flpLoadSprite[SPEC_BONFIRE]					= Load91;
		flpLoadSprite[SPEC_BONFIRE_I]					= Load91;
		flpLoadSprite[SPEC_BANQUET]					= Load92;
		flpLoadSprite[SPEC_BANQUET_I]					= Load92;
		flpLoadSprite[SPEC_TAPIS]						= Load93;
		flpLoadSprite[SPEC_TAPIS_I]					= Load93;
		flpLoadSprite[SPEC_MONOLITH_2]				= Load94;
		flpLoadSprite[SPEC_MONOLITH]					= Load95;
		flpLoadSprite[SPEC_MONOLITH_I]				= Load96;
		flpLoadSprite[SPEC_TAPIS_ROUGE_1]			= Load97;
		flpLoadSprite[SPEC_TAPIS_ROUGE_1_I]			= Load97;
		flpLoadSprite[SPEC_TAPIS_ROUGE_2]			= Load98;
		flpLoadSprite[SPEC_TAPIS_ROUGE_2_I]			= Load98;
		flpLoadSprite[SPEC_TAPIS_ROUGE_3]			= Load99;
		flpLoadSprite[SPEC_TAPIS_ROUGE_3_I]			= Load99;
		flpLoadSprite[SPEC_BOULDER]					= Load100;
		flpLoadSprite[SPEC_BOULDER_I]					= Load100;
		flpLoadSprite[SPEC_PRISON_BED]				= Load101;
		flpLoadSprite[SPEC_PRISON_BED_I]				= Load101;
		flpLoadSprite[SPEC_RUINED_MIRROR]			= Load102;
		flpLoadSprite[SPEC_RUINED_MIRROR_I]			= Load102;
		flpLoadSprite[SPEC_BIG_VASE]					= Load103;
		flpLoadSprite[SPEC_RUINED_TABLE]				= Load104;
		flpLoadSprite[SPEC_WEAPONS_RACK_2]			= Load105;
		flpLoadSprite[SPEC_WEAPONS_RACK_2_I]		= Load105;
		flpLoadSprite[SPEC_WEAPONS_RACK_1]			= Load106;
		flpLoadSprite[SPEC_WEAPONS_RACK_1_I]		= Load106;
		flpLoadSprite[SPEC_DUMMY]						= Load107;
		flpLoadSprite[SPEC_DUMMY_I]					= Load107;
		flpLoadSprite[SPEC_ARMOR_ON_A_RACK]			= Load108;
		flpLoadSprite[SPEC_ARMOR_ON_A_RACK_I]		= Load108;
		flpLoadSprite[SPEC_ARCHER]						= Load109;
		flpLoadSprite[SPEC_ARCHER_I]					= Load109;
		flpLoadSprite[SPEC_BLACK_SMITH_FORGERY]	= Load110;
		flpLoadSprite[TOMB_SW]							= Load111;
		flpLoadSprite[TOMB_SE]							= Load111;
		flpLoadSprite[GOLDEN_MIRROR_NE]				= Load112;
		flpLoadSprite[GOLDEN_MIRROR_NW]				= Load112;
		flpLoadSprite[HORLOGE_GP_SE]					= Load113;
		flpLoadSprite[HORLOGE_GP_SW]					= Load113;
		flpLoadSprite[HORLOGE_GP_NE]					= Load113;
		flpLoadSprite[HORLOGE_GP_NW]					= Load113;
		flpLoadSprite[ROCHE_1]							= Load115;
		flpLoadSprite[ROCHE_3]							= Load116;
		flpLoadSprite[ROCHE_2]							= Load117;
      flpLoadSprite[26]                         = Load118;
      flpLoadSprite[27]                         = Load119;
      flpLoadSprite[28]                         = Load120;
		flpLoadSprite[SHOP_SIGN_1]						= Load121;
		flpLoadSprite[PLAQUE_1]							= Load122;
		flpLoadSprite[PLAQUE_2]							= Load123;
		flpLoadSprite[PLAQUE_3]							= Load124;
		flpLoadSprite[PLAQUE_4]							= Load125;
		flpLoadSprite[PLAQUE_5]							= Load126;
		flpLoadSprite[SHOP_SIGN_1_I]					= Load121;
		flpLoadSprite[SHOP_SIGN_2]						= Load127;
		flpLoadSprite[SHOP_SIGN_2_I]					= Load127;
		flpLoadSprite[PLAQUE_1_I]						= Load122;
		flpLoadSprite[PLAQUE_2_I]						= Load123;
		flpLoadSprite[PLAQUE_3_I]						= Load124;
		flpLoadSprite[PLAQUE_4_I]						= Load125;
		flpLoadSprite[PLAQUE_5_I]						= Load126;
		flpLoadSprite[WOOD_SIGN_1]						= Load128;
		flpLoadSprite[WOOD_SIGN_2]						= Load129;
		flpLoadSprite[WOOD_SIGN_2_I]					= Load129;
		flpLoadSprite[SPEC_SHELF]						= Load130;
		flpLoadSprite[SPEC_SHELF_I]					= Load130;
		flpLoadSprite[SPEC_TRUNK]						= Load131;
		flpLoadSprite[SPEC_CHAUDRON]					= Load132;
		flpLoadSprite[SPEC_BIG_CHANDELLE]			= Load133;
      flpLoadSprite[SPEC_BIG_CHANDELLE2]			= Load133;
      flpLoadSprite[SPEC_BIG_CHANDELLE3]			= Load133;
		flpLoadSprite[SPEC_BRIDGE1]					= Load134;
		flpLoadSprite[SPEC_BRIDGE1_I]					= Load134;
		flpLoadSprite[SPEC_BRIDGE2]					= Load135;
		flpLoadSprite[SPEC_BRIDGE2_I]					= Load135;
		flpLoadSprite[SPEC_BRIDGE3]					= Load136;
		flpLoadSprite[SPEC_BRIDGE3_I]					= Load136;
		flpLoadSprite[SPEC_BORDER_BRIDGE1]			= Load137;
		flpLoadSprite[SPEC_BORDER_BRIDGE1_I]		= Load137;
		flpLoadSprite[SPEC_BORDER_BRIDGE1B]			= Load137;
		flpLoadSprite[SPEC_BORDER_BRIDGE1_IB]		= Load137;
		flpLoadSprite[SPEC_BORDER_BRIDGE2]			= Load138;
		flpLoadSprite[SPEC_BORDER_BRIDGE2_I]		= Load138;
		flpLoadSprite[SPEC_BORDER_BRIDGE2B]			= Load138;
		flpLoadSprite[SPEC_BORDER_BRIDGE2_IB]		= Load138;
		flpLoadSprite[SPEC_HARP]						= Load139;
		flpLoadSprite[SPEC_HARP_I]						= Load139;
		flpLoadSprite[SPEC_MARBLE_BANC]				= Load140;
		flpLoadSprite[SPEC_MARBLE_BANC_I]			= Load140;
		flpLoadSprite[SPEC_WOODEN_BANC]				= Load141;
		flpLoadSprite[SPEC_WOODEN_BANC_I]			= Load141;
		flpLoadSprite[SPEC_CHANDELLE]					= Load142;
		flpLoadSprite[SPEC_CHANDELLE_I]				= Load142;
		flpLoadSprite[SPEC_CHANDELLE_2]				= Load143;
		flpLoadSprite[SPEC_CHANDELLE_2_I]			= Load143;
		flpLoadSprite[SPEC_SPINNING_WHEEL]			= Load144;
		flpLoadSprite[SPEC_SPINNING_WHEEL_I]		= Load144;
		flpLoadSprite[SPEC_BUCHE]						= Load145;
		flpLoadSprite[SPEC_BUCHE_I]					= Load145;
		flpLoadSprite[FONTAINE_EAU]					= Load146;
		flpLoadSprite[SPEC_TAPIS_R]					= Load147;
		flpLoadSprite[MOTTE_TERRE]						= Load148;
		flpLoadSprite[TORTURE_1]						= Load149;
		flpLoadSprite[CHAMP]								= Load150;
		flpLoadSprite[BIG_FONTAINE_1]					= Load151;
		flpLoadSprite[BAR_1]								= Load152;
		flpLoadSprite[BAR_2]								= Load152;
		flpLoadSprite[SHOP_COUNTER_1]					= Load153;
		flpLoadSprite[SHOP_COUNTER_2]					= Load153;
		flpLoadSprite[WHELL_1]							= Load154;
		flpLoadSprite[TOWN_BOARD_1]					= Load155;
		flpLoadSprite[AUTEL_1]							= Load156;
		flpLoadSprite[AUTEL_1_I]						= Load156;
		flpLoadSprite[OBELISK_1]						= Load157;
		flpLoadSprite[SPEC_STALAGTIC_1]				= Load158;
		flpLoadSprite[SPEC_STALAGTIC_2]				= Load159;
		flpLoadSprite[SPEC_STALAGTIC_3]				= Load160;
		flpLoadSprite[SPEC_STALAGTIC_4]				= Load161;
		flpLoadSprite[SPEC_STALAGTIC_5]				= Load162;
		flpLoadSprite[SPEC_STALAGTIC_6]				= Load163;
		flpLoadSprite[SPEC_STALAGTIC_7]				= Load164;
		flpLoadSprite[SPEC_GROSSE_ROCHE_1]			= Load165;
		flpLoadSprite[SPEC_GROSSE_ROCHE_2]			= Load166;
		flpLoadSprite[SPEC_GROSSE_ROCHE_3]			= Load167;
		flpLoadSprite[MENIR_ROCHE]						= Load168;
		flpLoadSprite[MENIR_NOIR]						= Load169;
		flpLoadSprite[ROUND_WOODEN_TABLE]			= Load170;
		flpLoadSprite[LITTLE_WOODEN_TABLE_SE]		= Load171;
		flpLoadSprite[LITTLE_WOODEN_TABLE_SW]		= Load171;
		flpLoadSprite[LITTLE_ROUND_WOODEN_TABLE]  = Load172;
		flpLoadSprite[SQUARE_WOODEN_TABLE]			= Load173;
		flpLoadSprite[LARGE_WOODEN_TABLE_SE]		= Load174;
		flpLoadSprite[LARGE_WOODEN_TABLE_SW]		= Load174;
		flpLoadSprite[SPEC_TREE_SET_1]				= Load175;
		flpLoadSprite[SPEC_TREE_SET_2]				= Load176;
		flpLoadSprite[SPEC_TREE_SET_3]				= Load177;
		flpLoadSprite[SPEC_TREE_SET_4]				= Load178;
		flpLoadSprite[SPEC_TREE_SET_5]				= Load179;
		flpLoadSprite[SPEC_TREE_SET_6]				= Load180;
		flpLoadSprite[SPEC_TREE_SET_7]				= Load181;
		flpLoadSprite[SPEC_TREE_SET_8]				= Load182;
		flpLoadSprite[SPEC_TREE_SET_9]				= Load183;
		flpLoadSprite[SPEC_TREE_SET_10]				= Load184;
		flpLoadSprite[SPEC_TREE_SET_11]				= Load185;
		flpLoadSprite[SPEC_TREE_SET_12]				= Load186;
		flpLoadSprite[SPEC_TREE_SET_13]				= Load187;
		flpLoadSprite[SPEC_TREE_SET_14]				= Load188;
		flpLoadSprite[SPEC_TREE_SET_15]				= Load189;
		flpLoadSprite[SPEC_TREE_SET_16]				= Load190;
		flpLoadSprite[SPEC_TREE_SET_17]				= Load259;
		flpLoadSprite[SPEC_TREE_SET_18]				= Load259;
		flpLoadSprite[DUNGEON_SPEC_2_BORDER_NW_SE_1_1] = Load191;
		flpLoadSprite[DUNGEON_SPEC_2_BORDER_NW_SE_1_2] = Load191;
		flpLoadSprite[DUNGEON_SPEC_2_BORDER_NW_SE_2] = Load191;
		flpLoadSprite[DUNGEON_SPEC_2_BORDER_NE_SW_1_1] = Load191;
		flpLoadSprite[DUNGEON_SPEC_2_BORDER_NE_SW_1_2] = Load191;
		flpLoadSprite[DUNGEON_SPEC_2_BORDER_NE_SW_2] = Load191;
		flpLoadSprite[DUNGEON_BORDER_2_1]			= Load191;
		flpLoadSprite[DUNGEON_BORDER_2_2]			= Load191;
		flpLoadSprite[DUNGEON_BORDER_2_3]			= Load191;
		flpLoadSprite[DUNGEON_BORDER_2_4]			= Load191;
		flpLoadSprite[DUNGEON_BORDER_1]				= Load191;
		flpLoadSprite[DUNGEON_BORDER_2]				= Load191;
		flpLoadSprite[DUNGEON_BORDER_3]				= Load191;
		flpLoadSprite[DUNGEON_BORDER_4]				= Load191;
		flpLoadSprite[DUNGEON_SPEC_1_NW_SE]			= Load191;
		flpLoadSprite[DUNGEON_SPEC_1_NE_SW]			= Load191;
		flpLoadSprite[DUNGEON_SPEC_2_NW_SE]			= Load191;
		flpLoadSprite[DUNGEON_SPEC_2_NE_SW]			= Load191;
		flpLoadSprite[DUNGEON_SPEC_3_NW_SE]			= Load191;
		flpLoadSprite[DUNGEON_SPEC_3_NE_SW]			= Load191;
		flpLoadSprite[DUNGEON_B_NE_SW]				= Load191;
		flpLoadSprite[DUNGEON_NE_SW]					= Load191;
		flpLoadSprite[DUNGEON_B_NW_SE]				= Load191;
		flpLoadSprite[DUNGEON_NW_SE]					= Load191;
		flpLoadSprite[DUNGEON_WSE]						= Load191;
		flpLoadSprite[DUNGEON_WNE]						= Load191;
		flpLoadSprite[DUNGEON_NES]						= Load191;
		flpLoadSprite[DUNGEON_NWS]						= Load191;
		flpLoadSprite[DUNGEON_X]						= Load191;
		flpLoadSprite[DUNGEON_T_NSW]					= Load191;
		flpLoadSprite[DUNGEON_T_NEW]					= Load191;
		flpLoadSprite[DUNGEON_T_NES]					= Load191;
		flpLoadSprite[DUNGEON_T_ESW]					= Load191;
		flpLoadSprite[CAVERN_BORDER_1]				= Load36;
		flpLoadSprite[CAVERN_BORDER_2]				= Load36;
		flpLoadSprite[CAVERN_BORDER_3]				= Load36;
		flpLoadSprite[CAVERN_BORDER_4]				= Load36;
		flpLoadSprite[CAVERN_SPEC_1_NW_SE]			= Load36;
		flpLoadSprite[CAVERN_SPEC_1_NE_SW]			= Load36;
		flpLoadSprite[CAVERN_SPEC_2_NW_SE]			= Load36;
		flpLoadSprite[CAVERN_SPEC_2_NE_SW]			= Load36;
		flpLoadSprite[CAVERN_SPEC_3_NW_SE]			= Load36;
		flpLoadSprite[CAVERN_SPEC_3_NE_SW]			= Load36;
		flpLoadSprite[CAVERN_B_NE_SW]					= Load36;
		flpLoadSprite[CAVERN_EW]						= Load36;
		flpLoadSprite[CAVERN_NE_SW]					= Load36;
		flpLoadSprite[CAVERN_B_NW_SE]					= Load36;
		flpLoadSprite[CAVERN_NW_SE]					= Load36;
		flpLoadSprite[CAVERN_WSE]						= Load36;
		flpLoadSprite[CAVERN_WNE]						= Load36;
		flpLoadSprite[CAVERN_NES]						= Load36;
		flpLoadSprite[CAVERN_NWS]						= Load36;
		flpLoadSprite[CAVERN_WSE_2]					= Load36;
		flpLoadSprite[CAVERN_WNE_2]					= Load36;
		flpLoadSprite[CAVERN_NES_2]					= Load36;
		flpLoadSprite[CAVERN_NWS_2]					= Load36;
		flpLoadSprite[CAVERN_X]							= Load36;
		flpLoadSprite[CAVERN_T_NSW]					= Load36;
		flpLoadSprite[CAVERN_T_NEW]					= Load36;
		flpLoadSprite[CAVERN_T_NES]					= Load36;
		flpLoadSprite[CAVERN_T_ESW]					= Load36;
		flpLoadSprite[CAVERN_SKIP_1]					= Load36;
		flpLoadSprite[CAVERN_SKIP_2]					= Load36;
		flpLoadSprite[CAVERN_SKIP_3]					= Load36;
		flpLoadSprite[CAVERN_SKIP_4]					= Load36;
		flpLoadSprite[CAVERN_SKIP_5]					= Load36;
		flpLoadSprite[CAVERN_SKIP_6]					= Load36;
		flpLoadSprite[CAVERN_SKIP_7]					= Load36;	
		flpLoadSprite[CAVERN_SKIP_8]					= Load36;
		flpLoadSprite[CAVERN_SKIP_5_2]				= Load36;
		flpLoadSprite[CAVERN_SKIP_6_2]				= Load36;
		flpLoadSprite[CAVERN_SKIP_7_2]				= Load36;	
		flpLoadSprite[CAVERN_SKIP_8_2]				= Load36;
		flpLoadSprite[RAMPART_B_NE_SW]				= Load192;
		flpLoadSprite[RAMPART_NE_SW]					= Load192;
		flpLoadSprite[RAMPART_B_NW_SE]				= Load192;
		flpLoadSprite[RAMPART_NW_SE]					= Load192;
		flpLoadSprite[RAMPART_TOWER_1]				= Load192;
		flpLoadSprite[RAMPART_TOWER_2]				= Load192;
		flpLoadSprite[GRAVEYARD_B_NW_SE]				= Load193;
		flpLoadSprite[GRAVEYARD_NW_SE]				= Load193;
		flpLoadSprite[GRAVEYARD_B_NE_SW]				= Load193;
		flpLoadSprite[GRAVEYARD_NE_SW]				= Load193;
		flpLoadSprite[GRAVEYARD_WNE]					= Load193;
		flpLoadSprite[GRAVEYARD_NWS]					= Load193;
		flpLoadSprite[GRAVEYARD_NES]					= Load193;
		flpLoadSprite[GRAVEYARD_WSE]					= Load193;
		flpLoadSprite[GRAVEYARD_T_NES]				= Load193;
		flpLoadSprite[GRAVEYARD_T_ESW]				= Load193;
		flpLoadSprite[GRAVEYARD_T_NSW]				= Load193;
		flpLoadSprite[GRAVEYARD_T_NEW]				= Load193;
		flpLoadSprite[GRAVEYARD_X]						= Load193;
		flpLoadSprite[GRAVEYARD_C_WSE]				= Load193;
		flpLoadSprite[GRAVEYARD_NE]					= Load193;
		flpLoadSprite[GRAVEYARD_NW]					= Load193;
		flpLoadSprite[GRAVEYARD_EW]					= Load193;
		flpLoadSprite[GRAVEYARD_ENS]					= Load193;
		flpLoadSprite[GRAVEYARD_WNS]					= Load193;
		flpLoadSprite[GRAVEYARD_C_NWS]				= Load193;
		flpLoadSprite[GRAVEYARD_SE]					= Load193;
		flpLoadSprite[GRAVEYARD_C_NES]				= Load193;
		flpLoadSprite[GRAVEYARD_SW]					= Load193;
		flpLoadSprite[GRAVEYARD_C_WNE]				= Load193;
		flpLoadSprite[GRAVEYARD_PLUS]					= Load193;
		flpLoadSprite[GRAVEYARD_1_NW_SE]				= Load193;
		flpLoadSprite[GRAVEYARD_1_NE_SW]				= Load193;
		flpLoadSprite[GRAVEYARD_BS_NW_SE]			= Load193;
		flpLoadSprite[GRAVEYARD_BS_NE_SW]			= Load193;
		flpLoadSprite[WHITEWALL_BORDER_WG_I]		= Load194;
		flpLoadSprite[WHITEWALL_BORDER_WD]			= Load194;
		flpLoadSprite[WHITEWALL_BORDER_WD_I]		= Load194;
		flpLoadSprite[WHITEWALL_BORDER_WG]			= Load194;
		flpLoadSprite[WHITEWALL_X_NW_SE]				= Load194;
		flpLoadSprite[WHITEWALL_X_NE_SW]				= Load194;
		flpLoadSprite[WHITEWALL_BORDER_NW_SE]		= Load194;
		flpLoadSprite[WHITEWALL_BORDER_NE_SW]		= Load194;
		flpLoadSprite[WHITEWALL_TOP_NW_SE]			= Load194;
		flpLoadSprite[WHITEWALL_TOP_NE_SW]			= Load194;
		flpLoadSprite[WHITEWALL_B_NW_SE]				= Load194;
		flpLoadSprite[WHITEWALL_NW_SE]				= Load194;
		flpLoadSprite[WHITEWALL_B_NE_SW]				= Load194;
		flpLoadSprite[WHITEWALL_NE_SW]				= Load194;
		flpLoadSprite[WHITEWALL_WNE]					= Load194;
		flpLoadSprite[WHITEWALL_NWS]					= Load194;
		flpLoadSprite[WHITEWALL_NES]					= Load194;
		flpLoadSprite[WHITEWALL_WSE]					= Load194;
		flpLoadSprite[WHITEWALL_T_NES]				= Load194;
		flpLoadSprite[WHITEWALL_T_ESW]				= Load194;
		flpLoadSprite[WHITEWALL_T_NSW]				= Load194;
		flpLoadSprite[WHITEWALL_T_NEW]				= Load194;
		flpLoadSprite[WHITEWALL_B_NW_SE]				= Load194;
		flpLoadSprite[WHITEWALL_X]						= Load194;
		flpLoadSprite[WHITEWALL_C_WSE]				= Load194;
		flpLoadSprite[WHITEWALL_NE]					= Load194;
		flpLoadSprite[WHITEWALL_NW]					= Load194;
		flpLoadSprite[WHITEWALL_EW]					= Load194;
		flpLoadSprite[WHITEWALL_ENS]					= Load194;
		flpLoadSprite[WHITEWALL_WNS]					= Load194;
		flpLoadSprite[WHITEWALL_C_NWS]				= Load194;
		flpLoadSprite[WHITEWALL_SE]					= Load194;
		flpLoadSprite[WHITEWALL_C_NES]				= Load194;
		flpLoadSprite[WHITEWALL_SW]					= Load194;
		flpLoadSprite[WHITEWALL_C_WNE]				= Load194;
		flpLoadSprite[WHITEWALL_PLUS]					= Load194;
		flpLoadSprite[WHITEWALL_W_NW_SE]				= Load194;
		flpLoadSprite[WHITEWALL_W_NE_SW]				= Load194;
		flpLoadSprite[WHITEWALL_P_NW_SE]				= Load194;
		flpLoadSprite[WHITEWALL_P_NE_SW]				= Load194;
		flpLoadSprite[WHITEWALL_H1_NW_SE]			= Load194;
		flpLoadSprite[WHITEWALL_H1_NE_SW]			= Load194;
		flpLoadSprite[WHITEWALL_H2_NW_SE]			= Load194;
		flpLoadSprite[WHITEWALL_H2_NE_SW]			= Load194;
		flpLoadSprite[WHITEWALL_BS_NW_SE]			= Load194;
		flpLoadSprite[WHITEWALL_BS_NE_SW]			= Load194;
		flpLoadSprite[BRICKWALL_BORDER_WG_I]		= Load195;
		flpLoadSprite[BRICKWALL_BORDER_WG]			= Load195;
		flpLoadSprite[BRICKWALL_BORDER_WD_I]		= Load195;
		flpLoadSprite[BRICKWALL_BORDER_WD]			= Load195;
		flpLoadSprite[BRICKWALL_BORDER_NW_SE]		= Load195;
		flpLoadSprite[BRICKWALL_BORDER_NE_SW]		= Load195;
		flpLoadSprite[BRICKWALL_TOP_NW_SE]			= Load195;
		flpLoadSprite[BRICKWALL_TOP_NE_SW]			= Load195;
		flpLoadSprite[BRICKWALL_B_NW_SE]				= Load195;
		flpLoadSprite[BRICKWALL_B_NE_SW]				= Load195;
		flpLoadSprite[BRICKWALL_NW_SE]				= Load195;
		flpLoadSprite[BRICKWALL_NE_SW]				= Load195;
		flpLoadSprite[BRICKWALL_WNE]					= Load195;
		flpLoadSprite[BRICKWALL_NWS]					= Load195;
		flpLoadSprite[BRICKWALL_NES]					= Load195;
		flpLoadSprite[BRICKWALL_WSE]					= Load195;
		flpLoadSprite[BRICKWALL_T_NES]				= Load195;
		flpLoadSprite[BRICKWALL_T_ESW]				= Load195;
		flpLoadSprite[BRICKWALL_T_NSW]				= Load195;
		flpLoadSprite[BRICKWALL_T_NEW]				= Load195;
		flpLoadSprite[BRICKWALL_X]						= Load195;
		flpLoadSprite[BRICKWALL_C_WSE]				= Load195;
		flpLoadSprite[BRICKWALL_NE]					= Load195;
		flpLoadSprite[BRICKWALL_NW]					= Load195;
		flpLoadSprite[BRICKWALL_EW]					= Load195;
		flpLoadSprite[BRICKWALL_ENS]					= Load195;
		flpLoadSprite[BRICKWALL_WNS]					= Load195;
		flpLoadSprite[BRICKWALL_C_NWS]				= Load195;
		flpLoadSprite[BRICKWALL_SE]					= Load195;
		flpLoadSprite[BRICKWALL_C_NES]				= Load195;
		flpLoadSprite[BRICKWALL_SW]					= Load195;
		flpLoadSprite[BRICKWALL_C_WNE]				= Load195;
		flpLoadSprite[BRICKWALL_PLUS]					= Load195;
		flpLoadSprite[BRICKWALL_W_NW_SE]				= Load195;
		flpLoadSprite[BRICKWALL_W_NE_SW]				= Load195;
		flpLoadSprite[BRICKWALL_P_NW_SE]				= Load195;
		flpLoadSprite[BRICKWALL_P_NE_SW]				= Load195;
		flpLoadSprite[BRICKWALL_H1_NW_SE]			= Load195;
		flpLoadSprite[BRICKWALL_H1_NE_SW]			= Load195;
		flpLoadSprite[BRICKWALL_H2_NW_SE]			= Load195;
		flpLoadSprite[BRICKWALL_H2_NE_SW]			= Load195;
		flpLoadSprite[BRICKWALL_BS_NW_SE]			= Load195;
		flpLoadSprite[BRICKWALL_BS_NE_SW]			= Load195;
		flpLoadSprite[HAIE_B_NW_SE]					= Load196;
		flpLoadSprite[HAIE_NW_SE]						= Load196;
		flpLoadSprite[HAIE_B_NE_SW]					= Load196;
		flpLoadSprite[HAIE_NE_SW]						= Load196;
		flpLoadSprite[HAIE_WNE]							= Load196;
		flpLoadSprite[HAIE_NWS]							= Load196;
		flpLoadSprite[HAIE_NES]							= Load196;
		flpLoadSprite[HAIE_WSE]							= Load196;
		flpLoadSprite[HAIE_T_NES]						= Load196;
		flpLoadSprite[HAIE_T_ESW]						= Load196;
		flpLoadSprite[HAIE_T_NSW]						= Load196;
		flpLoadSprite[HAIE_T_NEW]						= Load196;
		flpLoadSprite[HAIE_X]							= Load196;
		flpLoadSprite[LAVA]								= Load197;
		flpLoadSprite[MOUNTAIN_1]						= Load198;
		flpLoadSprite[RUINED_BRICKWALL_NW_SE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_NE_SW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_W_NW_SE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_W_NE_SW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_PLUS]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_WNE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_NWS]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_NES]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_WSE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_T_NES]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_T_ESW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_T_NSW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_T_NEW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_X]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_EW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_WNS]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_NE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_NW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_SE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_SW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_C_NES]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_C_NWS]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_C_WNE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_C_WSE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_B_NW_SE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_B_NE_SW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_BS_NW_SE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_BS_NE_SW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_ENS]					= Load279;
		flpLoadSprite[RUINED_BRICKWALL_BORDER_WD]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_BORDER_WG]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_BORDER_WD_I]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_BORDER_WG_I]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_P_NW_SE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_P_NE_SW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_H1_NW_SE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_H1_NE_SW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_H2_NW_SE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_H2_NE_SW]			= Load279;
    	flpLoadSprite[RUINED_BRICKWALL_X_NW_SE]			= Load279;
    	flpLoadSprite[RUINED_BRICKWALL_X_NE_SW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_TOP_NE_SW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_TOP_NW_SE]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_BORDER_NE_SW]			= Load279;
		flpLoadSprite[RUINED_BRICKWALL_BORDER_NW_SE]			= Load279;
		flpLoadSprite[RUINED_WHITEWALL_BORDER_WG_I]		= Load278;
		flpLoadSprite[RUINED_WHITEWALL_BORDER_WD]			= Load278;
		flpLoadSprite[RUINED_WHITEWALL_BORDER_WD_I]		= Load278;
		flpLoadSprite[RUINED_WHITEWALL_BORDER_WG]			= Load278;
		flpLoadSprite[RUINED_WHITEWALL_X_NW_SE]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_X_NE_SW]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_BORDER_NW_SE]		= Load278;
		flpLoadSprite[RUINED_WHITEWALL_BORDER_NE_SW]		= Load278;
		flpLoadSprite[RUINED_WHITEWALL_TOP_NW_SE]			= Load278;
		flpLoadSprite[RUINED_WHITEWALL_TOP_NE_SW]			= Load278;
		flpLoadSprite[RUINED_WHITEWALL_B_NW_SE]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_NW_SE]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_B_NE_SW]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_NE_SW]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_WNE]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_NWS]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_NES]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_WSE]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_T_NES]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_T_ESW]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_T_NSW]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_T_NEW]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_B_NW_SE]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_X]						= Load278;
		flpLoadSprite[RUINED_WHITEWALL_C_WSE]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_NE]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_NW]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_EW]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_ENS]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_WNS]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_C_NWS]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_SE]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_C_NES]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_SW]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_C_WNE]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_PLUS]					= Load278;
		flpLoadSprite[RUINED_WHITEWALL_W_NW_SE]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_W_NE_SW]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_P_NW_SE]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_P_NE_SW]				= Load278;
		flpLoadSprite[RUINED_WHITEWALL_H1_NW_SE]			= Load278;
		flpLoadSprite[RUINED_WHITEWALL_H1_NE_SW]			= Load278;
		flpLoadSprite[RUINED_WHITEWALL_H2_NW_SE]			= Load278;
		flpLoadSprite[RUINED_WHITEWALL_H2_NE_SW]			= Load278;
		flpLoadSprite[RUINED_WHITEWALL_BS_NW_SE]			= Load278;
		flpLoadSprite[RUINED_WHITEWALL_BS_NE_SW]			= Load194;
		AllSmoothLoad(FORTER, Load199);
		AllSmoothLoad(GRATER, Load200);
		AllSmoothLoad(GRAFOR, Load201); //
		AllSmoothLoad(WATDES, Load202);
		AllSmoothLoad(WATFOR, Load203);
		AllSmoothLoad(WATGRA, Load204);
		AllSmoothLoad(GRADES, Load205);
		AllSmoothLoad(TERDES, Load206);
		AllSmoothLoad(FORDES, Load207);
		AllSmoothLoad(WATCAV, Load208);
		AllSmoothLoad(MTNDES, Load209);
		AllSmoothLoad(MTNFOR, Load210);
		AllSmoothLoad(MTNTER, Load211);
		AllSmoothLoad(MTNGRA, Load212);
		AllSmoothLoad(WATTER, Load213);
		AllSmoothLoad(WATMTN, Load214);
		AllSmoothLoad(FLOGRA, Load251);
		AllSmoothLoad(FLOFOR, Load252);
		AllSmoothLoad(FLOTER, Load253);
		AllSmoothLoad(FLODES, Load254);
		AllSmoothLoad(GRADUN, Load258);
		AllSmoothLoad(GRACAV, Load271);
      AllSmoothLoad3(GRAROC, Load45 );
      AllSmoothLoad3(GRARC2, Load270 );
      AllSmoothLoad3(ROCROC, Load264);
      AllSmoothLoad3(MTNROC, Load265);

		AllSmoothLoad(SWPWAT, Load272);
		AllSmoothLoad(SWPMNT, Load273);
		AllSmoothLoad(SWPTER, Load274);
		AllSmoothLoad(SWPGRA, Load275);
		AllSmoothLoad(SWPFOR, Load276);
		AllSmoothLoad(SWPDES, Load277);


		flpLoadSprite[BED_RICHE]						= Load215;
		flpLoadSprite[ROCK_WALL]						= Load216;
		flpLoadSprite[WHITEWALL]						= Load217;
		flpLoadSprite[GRAVEYARD_FENCE]				= Load218;
		flpLoadSprite[RAMPART]							= Load219;
		flpLoadSprite[CAVERNWALL_2]					= Load220;
		flpLoadSprite[DUNGEON_WALL]					= Load221;
		flpLoadSprite[CAVERNWALL]						= Load222;
		flpLoadSprite[WALL_SPEC1]						= Load223;
		flpLoadSprite[WALL_SPEC2]						= Load224;
		flpLoadSprite[WALL_SPEC3]						= Load225;
		flpLoadSprite[WALL_SPEC4]						= Load226;
		flpLoadSprite[WALL_SPEC5]						= Load227;
		flpLoadSprite[BED_PAUVRE]						= Load228;
		flpLoadSprite[CHAUDRON]							= Load229;
		flpLoadSprite[AUTEL]								= Load230;
		flpLoadSprite[156]								= Load231;
		flpLoadSprite[DUNGEON_WELL]					= Load232;
		flpLoadSprite[283]								= Load233;
		flpLoadSprite[2560]								= Load234;
		flpLoadSprite[4005]								= Load235;
		flpLoadSprite[BIG_DUNGEON_WELL]				= Load236;
		flpLoadSprite[255]								= Load237;
		flpLoadSprite[DUNGEON_TOP]						= Load238;
		flpLoadSprite[DUNGEON_TOP_2]					= Load238;
		flpLoadSprite[DUNGEON_TOP_3]					= Load238;
		flpLoadSprite[DUNGEON_TOP_4]					= Load238;
		flpLoadSprite[DUNGEON_TOP_4_I]				= Load238;
		flpLoadSprite[DUNGEON_TOP_5]					= Load238;
		flpLoadSprite[DUNGEON_TOP_6]					= Load238;
		flpLoadSprite[DUNGEON_TOP_7]					= Load238;
		flpLoadSprite[DUNGEON_TOP_8]					= Load238;
		flpLoadSprite[DUNGEON_TOP_9]					= Load238;
		flpLoadSprite[DUNGEON_TOP_10]					= Load238;
		flpLoadSprite[DUNGEON_TOP_11]					= Load238;
		flpLoadSprite[DUNGEON_TOP_12]					= Load238;
		flpLoadSprite[DUNGEON_TOP_13]					= Load238;
		flpLoadSprite[DUNGEON_TOP_14]					= Load238;
		flpLoadSprite[DUNGEON_TOP_15]					= Load238;
		flpLoadSprite[DUNGEON_TOP_16]					= Load238;
		flpLoadSprite[DUNGEONROCK_WALL]					= Load239;
		flpLoadSprite[DUNGEONROCK_BORDER_2_1]			= Load241;
		flpLoadSprite[DUNGEONROCK_BORDER_2_2]			= Load241;
		flpLoadSprite[DUNGEONROCK_BORDER_2_3]			= Load241;
		flpLoadSprite[DUNGEONROCK_BORDER_2_4]			= Load241;
		flpLoadSprite[DUNGEONROCK_BORDER_1]				= Load241;
		flpLoadSprite[DUNGEONROCK_BORDER_2]				= Load241;
		flpLoadSprite[DUNGEONROCK_BORDER_3]				= Load241;
		flpLoadSprite[DUNGEONROCK_BORDER_4]				= Load241;
		flpLoadSprite[DUNGEONROCK_SPEC_1_NW_SE]			= Load241;
		flpLoadSprite[DUNGEONROCK_SPEC_1_NE_SW]			= Load241;
		flpLoadSprite[DUNGEONROCK_SPEC_2_NW_SE]			= Load241;
		flpLoadSprite[DUNGEONROCK_SPEC_2_NE_SW]			= Load241;
		flpLoadSprite[DUNGEONROCK_SPEC_3_NW_SE]			= Load241;
		flpLoadSprite[DUNGEONROCK_SPEC_3_NE_SW]			= Load241;
		flpLoadSprite[DUNGEONROCK_B_NE_SW]				= Load241;
		flpLoadSprite[DUNGEONROCK_NE_SW]					= Load241;
		flpLoadSprite[DUNGEONROCK_B_NW_SE]				= Load241;
		flpLoadSprite[DUNGEONROCK_NW_SE]					= Load241;
		flpLoadSprite[DUNGEONROCK_WSE]						= Load241;
		flpLoadSprite[DUNGEONROCK_WNE]						= Load241;
		flpLoadSprite[DUNGEONROCK_NES]						= Load241;
		flpLoadSprite[DUNGEONROCK_NWS]						= Load241;
		flpLoadSprite[DUNGEONROCK_X]						= Load241;
		flpLoadSprite[DUNGEONROCK_T_NSW]					= Load241;
		flpLoadSprite[DUNGEONROCK_T_NEW]					= Load241;
		flpLoadSprite[DUNGEONROCK_T_NES]					= Load241;
		flpLoadSprite[DUNGEONROCK_T_ESW]					= Load241;
		flpLoadSprite[DUNGEONROCK_TOP]						= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_2]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_3]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_4]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_4_I]				= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_5]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_6]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_7]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_8]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_9]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_10]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_11]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_12]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_13]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_14]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_15]					= Load238;
		flpLoadSprite[DUNGEONROCK_TOP_16]					= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NW_SE_1_1]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NW_SE_1_2]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NW_SE_1_3]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NW_SE_1_4]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NW_SE_1_5]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NW_SE_2_1]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NW_SE_2_2]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NW_SE_2_3]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NW_SE_2_4]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NW_SE_2_5]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NE_SW_1_1]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NE_SW_1_2]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NE_SW_1_3]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NE_SW_1_4]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NE_SW_1_5]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NE_SW_2_1]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NE_SW_2_2]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NE_SW_2_3]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NE_SW_2_4]	= Load238;
		flpLoadSprite[DUNGEONROCK_SPEC_1_BORDER_NE_SW_2_5]	= Load238;
		flpLoadSprite[DUNGEONROCK_FLOOR]							= Load242;
		flpLoadSprite[284]											= Load243;
		flpLoadSprite[CASTLEWALL]				 			= Load246;
		flpLoadSprite[CASTLEWALL_W1_NW_SE]				= Load244;
		flpLoadSprite[CASTLEWALL_W1_NE_SW]				= Load244;
		flpLoadSprite[CASTLEWALL_BORDER_2_1]			= Load244;
		flpLoadSprite[CASTLEWALL_BORDER_2_2]			= Load244;
		flpLoadSprite[CASTLEWALL_BORDER_2_3]			= Load244;
		flpLoadSprite[CASTLEWALL_BORDER_2_4]			= Load244;
		flpLoadSprite[CASTLEWALL_BORDER_1]				= Load244;
		flpLoadSprite[CASTLEWALL_BORDER_2]				= Load244;
		flpLoadSprite[CASTLEWALL_BORDER_3]				= Load244;
		flpLoadSprite[CASTLEWALL_BORDER_4]				= Load244;
		flpLoadSprite[CASTLEWALL_PLUS]			      = Load244,
		flpLoadSprite[CASTLEWALL_SPEC_1_NW_SE]			= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_NE_SW]			= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_2_NW_SE]			= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_2_NE_SW]			= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_3_NW_SE]			= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_3_NE_SW]			= Load244;
		flpLoadSprite[CASTLEWALL_B_NE_SW]				= Load244;
		flpLoadSprite[CASTLEWALL_NE_SW]					= Load244;
		flpLoadSprite[CASTLEWALL_B_NW_SE]				= Load244;
		flpLoadSprite[CASTLEWALL_NW_SE]					= Load244;
		flpLoadSprite[CASTLEWALL_WSE]						= Load244;
		flpLoadSprite[CASTLEWALL_WNE]						= Load244;
		flpLoadSprite[CASTLEWALL_NES]						= Load244;
		flpLoadSprite[CASTLEWALL_NWS]						= Load244;
		flpLoadSprite[CASTLEWALL_X]						= Load244;
		flpLoadSprite[CASTLEWALL_T_NSW]					= Load244;
		flpLoadSprite[CASTLEWALL_T_NEW]					= Load244;
		flpLoadSprite[CASTLEWALL_T_NES]					= Load244;
		flpLoadSprite[CASTLEWALL_T_ESW]					= Load244;
		flpLoadSprite[CASTLEWALL_SE]						= Load244;
		flpLoadSprite[CASTLEWALL_EW]						= Load244;
		flpLoadSprite[CASTLEWALL_WNS]						= Load244;
		flpLoadSprite[CASTLEWALL_NE]						= Load244;
		flpLoadSprite[CASTLEWALL_NW]						= Load244;
		flpLoadSprite[CASTLEWALL_SE]						= Load244;
		flpLoadSprite[CASTLEWALL_SW]						= Load244;
		flpLoadSprite[CASTLEWALL_C_NES]					= Load244;
		flpLoadSprite[CASTLEWALL_C_NWS]					= Load244;
		flpLoadSprite[CASTLEWALL_C_WNE]					= Load244;
		flpLoadSprite[CASTLEWALL_C_WSE]					= Load244;
		flpLoadSprite[CASTLEWALL_B_NW_SE]				= Load244;
		flpLoadSprite[CASTLEWALL_B_NE_SW]				= Load244;
		flpLoadSprite[CASTLEWALL_BS_NW_SE]				= Load244;
		flpLoadSprite[CASTLEWALL_BS_NE_SW]				= Load244;
		flpLoadSprite[CASTLEWALL_ENS]						= Load244;
		flpLoadSprite[CASTLEWALL_TOP]						= Load244;
		flpLoadSprite[CASTLEWALL_TOP_2]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_3]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_4]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_4_I]				= Load244;
		flpLoadSprite[CASTLEWALL_TOP_5]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_6]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_7]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_8]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_9]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_10]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_11]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_12]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_13]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_14]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_15]					= Load244;
		flpLoadSprite[CASTLEWALL_TOP_16]					= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NW_SE_1_1]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NW_SE_1_2]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NW_SE_1_3]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NW_SE_1_4]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NW_SE_1_5]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NW_SE_2_1]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NW_SE_2_2]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NW_SE_2_3]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NW_SE_2_4]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NW_SE_2_5]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NE_SW_1_1]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NE_SW_1_2]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NE_SW_1_3]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NE_SW_1_4]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NE_SW_1_5]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NE_SW_2_1]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NE_SW_2_2]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NE_SW_2_3]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NE_SW_2_4]	= Load244;
		flpLoadSprite[CASTLEWALL_SPEC_1_BORDER_NE_SW_2_5]	= Load244;
		flpLoadSprite[153]											= Load245;
		flpLoadSprite[66]												= Load247;
		flpLoadSprite[206]											= Load248;
		flpLoadSprite[575]											= Load249;
		flpLoadSprite[64]												= Load250;
		flpLoadSprite[191]											= Load255;
      flpLoadSprite[74]                                  = Load256;
      flpLoadSprite[23]                                  = Load257;
      flpLoadSprite[24]                                  = Load257;
      flpLoadSprite[25]                                  = Load257;
      Mathieu();
		flpLoadSprite[DUNGEONROCK_WALL2] 					= Load239;
		flpLoadSprite[DUNGEONROCK_2_BORDER_2_1]			= Load260;
		flpLoadSprite[DUNGEONROCK_2_BORDER_2_2]			= Load260;
		flpLoadSprite[DUNGEONROCK_2_BORDER_2_3]			= Load260;
		flpLoadSprite[DUNGEONROCK_2_BORDER_2_4]			= Load260;
		flpLoadSprite[DUNGEONROCK_2_BORDER_1]				= Load260;
		flpLoadSprite[DUNGEONROCK_2_BORDER_2]				= Load260;
		flpLoadSprite[DUNGEONROCK_2_BORDER_3]				= Load260;
		flpLoadSprite[DUNGEONROCK_2_BORDER_4]				= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_NW_SE]			= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_NE_SW]			= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_2_NW_SE]			= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_2_NE_SW]			= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_3_NW_SE]			= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_3_NE_SW]			= Load260;
		flpLoadSprite[DUNGEONROCK_2_B_NE_SW]				= Load260;
		flpLoadSprite[DUNGEONROCK_2_NE_SW]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_B_NW_SE]				= Load260;
		flpLoadSprite[DUNGEONROCK_2_NW_SE]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_WSE]						= Load260;
		flpLoadSprite[DUNGEONROCK_2_WNE]						= Load260;
		flpLoadSprite[DUNGEONROCK_2_NES]						= Load260;
		flpLoadSprite[DUNGEONROCK_2_NWS]						= Load260;
		flpLoadSprite[DUNGEONROCK_2_X]						= Load260;
		flpLoadSprite[DUNGEONROCK_2_T_NSW]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_T_NEW]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_T_NES]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_T_ESW]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP]						= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_2]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_3]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_4]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_4_I]				= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_5]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_6]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_7]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_8]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_9]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_10]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_11]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_12]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_13]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_14]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_15]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_TOP_16]					= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_1_1]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_1_2]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_1_3]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_1_4]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_1_5]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_2_1]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_2_2]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_2_3]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_2_4]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_2_5]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_1_1]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_1_2]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_1_3]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_1_4]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_1_5]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_2_1]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_2_2]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_2_3]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_2_4]	= Load260;
		flpLoadSprite[DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_2_5]	= Load260;
		flpLoadSprite[DUNGEONROCK_WALL3] 					= Load239;
		flpLoadSprite[DUNGEONROCK_3_BORDER_2_1]			= Load261;
		flpLoadSprite[DUNGEONROCK_3_BORDER_2_2]			= Load261;
		flpLoadSprite[DUNGEONROCK_3_BORDER_2_3]			= Load261;
		flpLoadSprite[DUNGEONROCK_3_BORDER_2_4]			= Load261;
		flpLoadSprite[DUNGEONROCK_3_BORDER_1]				= Load261;
		flpLoadSprite[DUNGEONROCK_3_BORDER_2]				= Load261;
		flpLoadSprite[DUNGEONROCK_3_BORDER_3]				= Load261;
		flpLoadSprite[DUNGEONROCK_3_BORDER_4]				= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_NW_SE]			= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_NE_SW]			= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_2_NW_SE]			= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_2_NE_SW]			= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_3_NW_SE]			= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_3_NE_SW]			= Load261;
		flpLoadSprite[DUNGEONROCK_3_B_NE_SW]				= Load261;
		flpLoadSprite[DUNGEONROCK_3_NE_SW]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_B_NW_SE]				= Load261;
		flpLoadSprite[DUNGEONROCK_3_NW_SE]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_WSE]						= Load261;
		flpLoadSprite[DUNGEONROCK_3_WNE]						= Load261;
		flpLoadSprite[DUNGEONROCK_3_NES]						= Load261;
		flpLoadSprite[DUNGEONROCK_3_NWS]						= Load261;
		flpLoadSprite[DUNGEONROCK_3_X]						= Load261;
		flpLoadSprite[DUNGEONROCK_3_T_NSW]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_T_NEW]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_T_NES]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_T_ESW]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP]						= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_2]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_3]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_4]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_4_I]				= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_5]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_6]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_7]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_8]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_9]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_10]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_11]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_12]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_13]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_14]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_15]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_TOP_16]					= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_1_1]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_1_2]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_1_3]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_1_4]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_1_5]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_2_1]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_2_2]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_2_3]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_2_4]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_2_5]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_1_1]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_1_2]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_1_3]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_1_4]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_1_5]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_2_1]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_2_2]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_2_3]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_2_4]	= Load261;
		flpLoadSprite[DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_2_5]	= Load261;
		flpLoadSprite[DUNGEONROCK_WALL4]	   				= Load239;
		flpLoadSprite[DUNGEONROCK_4_BORDER_2_1]			= Load262;
		flpLoadSprite[DUNGEONROCK_4_BORDER_2_2]			= Load262;
		flpLoadSprite[DUNGEONROCK_4_BORDER_2_3]			= Load262;
		flpLoadSprite[DUNGEONROCK_4_BORDER_2_4]			= Load262;
		flpLoadSprite[DUNGEONROCK_4_BORDER_1]				= Load262;
		flpLoadSprite[DUNGEONROCK_4_BORDER_2]				= Load262;
		flpLoadSprite[DUNGEONROCK_4_BORDER_3]				= Load262;
		flpLoadSprite[DUNGEONROCK_4_BORDER_4]				= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_NW_SE]			= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_NE_SW]			= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_2_NW_SE]			= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_2_NE_SW]			= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_3_NW_SE]			= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_3_NE_SW]			= Load262;
		flpLoadSprite[DUNGEONROCK_4_B_NE_SW]				= Load262;
		flpLoadSprite[DUNGEONROCK_4_NE_SW]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_B_NW_SE]				= Load262;
		flpLoadSprite[DUNGEONROCK_4_NW_SE]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_WSE]						= Load262;
		flpLoadSprite[DUNGEONROCK_4_WNE]						= Load262;
		flpLoadSprite[DUNGEONROCK_4_NES]						= Load262;
		flpLoadSprite[DUNGEONROCK_4_NWS]						= Load262;
		flpLoadSprite[DUNGEONROCK_4_X]						= Load262;
		flpLoadSprite[DUNGEONROCK_4_T_NSW]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_T_NEW]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_T_NES]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_T_ESW]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP]						= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_2]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_3]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_4]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_4_I]				= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_5]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_6]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_7]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_8]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_9]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_10]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_11]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_12]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_13]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_14]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_15]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_TOP_16]					= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_1_1]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_1_2]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_1_3]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_1_4]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_1_5]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_2_1]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_2_2]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_2_3]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_2_4]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_2_5]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_1_1]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_1_2]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_1_3]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_1_4]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_1_5]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_2_1]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_2_2]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_2_3]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_2_4]	= Load262;
		flpLoadSprite[DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_2_5]	= Load262;
      flpLoadSprite[SPIDER_2] = Load263;
      flpLoadSprite[SPEC_TROLL_BED]             = Load269;
      flpLoadSprite[SPEC_TROLL_BED_I]           = Load269;
      flpLoadSprite[SPEC_TROLL_ROUND_TABLE]     = Load269;
      flpLoadSprite[SPEC_TROLL_TABLE]           = Load269;
      flpLoadSprite[SPEC_TROLL_TABLE_I]         = Load269;
      flpLoadSprite[SPEC_TROLL_CHAIR_1]         = Load269;
      flpLoadSprite[SPEC_TROLL_CHAIR_1_I]       = Load269;
      flpLoadSprite[SPEC_TROLL_CHAIR_2]         = Load269;
		flpLoadSprite[SPEC_BROKEN_CRYPT]				= Load280;
		flpLoadSprite[SPEC_BROKEN_CRYPT_I]				= Load280;
		flpLoadSprite[SPEC_BROKEN_FOUNTAIN]				= Load281;
		flpLoadSprite[SPEC_BROKEN_FOUNTAIN_I]				= Load281;
		flpLoadSprite[SPEC_CATAPULT_BOULDER1]				= Load282;
		flpLoadSprite[SPEC_CATAPULT_BOULDER2]				= Load283;
		flpLoadSprite[SPEC_GREEN_COLUMN]				= Load284;
		flpLoadSprite[SPEC_BLUE_COLUMN]				= Load285;
		flpLoadSprite[SPEC_BROWN_COLUMN]				= Load286;
		flpLoadSprite[SPEC_BALISTA]				= Load287;
		flpLoadSprite[SPEC_BALISTA_I]				= Load287;
		flpLoadSprite[SPEC_BALISTA2]				= Load288;
		flpLoadSprite[SPEC_BALISTA2_I]				= Load288;
		flpLoadSprite[SPEC_CATAPULT]				= Load289;
		flpLoadSprite[SPEC_CATAPULT_I]				= Load289;
		flpLoadSprite[SPEC_TREBUCHET]				= Load290;
		flpLoadSprite[SPEC_TREBUCHET_I]				= Load290;
		flpLoadSprite[SPEC_TREBUCHET2]				= Load291;
		flpLoadSprite[SPEC_TREBUCHET2_I]			= Load291;
		flpLoadSprite[FLOOR_LAVA]					= Load292;
		flpLoadSprite[SPEC_WOOD_PRISON]				= Load293;
		flpLoadSprite[SPEC_PENTACLE]				= Load294;
		flpLoadSprite[SPEC_DARK_PENTACLE]			= Load295;
		flpLoadSprite[SPEC_OLD_PENTACLE]			= Load296;
		flpLoadSprite[SPEC_TOMB_OF_DARKNESS]		= Load297;
		flpLoadSprite[FLOOR_ICE_B]					= Load298; 
		flpLoadSprite[SPEC_BANC_TEMPLE]                  = Load299;
		flpLoadSprite[SPEC_CRYPT2]                       = Load300;
		flpLoadSprite[SPEC_FORGE1]                       = Load301;
		flpLoadSprite[SPEC_FORGE2]                       = Load301;
      flpLoadSprite[SPEC_HORSE_WATER]                  = Load302;
      flpLoadSprite[SPEC_HORSE_WATER_I]                = Load302;
		flpLoadSprite[SPEC_HUTE_GOB1]                    = Load303;
		flpLoadSprite[SPEC_HUTE_GOB2]                    = Load303;
      flpLoadSprite[SPEC_BAR]                          = Load304;
      flpLoadSprite[SPEC_BAR_I]                        = Load304;
		flpLoadSprite[SPEC_ORGUE]                        = Load305;
      flpLoadSprite[SPEC_ORGUE_I]                      = Load305;
		flpLoadSprite[SPEC_TABLE_FORGE1]                 = Load306;
      flpLoadSprite[SPEC_TABLE_FORGE1_I]               = Load306;
		flpLoadSprite[SPEC_TABLE_TEMPLE1]                = Load307;
      flpLoadSprite[SPEC_TABLE_TEMPLE1_I]              = Load307;
		flpLoadSprite[SPEC_TORTUR1]                      = Load308;
      flpLoadSprite[SPEC_TORTUR1_I]                    = Load308;
		flpLoadSprite[SPEC_TORTUR2]                      = Load309;
		flpLoadSprite[SPEC_TORTUR3]                      = Load310;
		flpLoadSprite[SPEC_TORTUR4]                      = Load311;
		flpLoadSprite[SPEC_TORTUR5]                      = Load312;
		flpLoadSprite[SPEC_TORTUR6]                      = Load313;
      flpLoadSprite[SPEC_TORTUR6_I]                    = Load313;
		flpLoadSprite[SPEC_TRONE_KING1]                  = Load314;
		flpLoadSprite[SPEC_TRONE_KING2]                  = Load315;
		flpLoadSprite[SPEC_BLOCKFLECHES]                 = Load316;
		flpLoadSprite[SPEC_ENTREEMINE]          	        = Load317;
		flpLoadSprite[SPEC_PONTPIERRE]          	        = Load318;
		flpLoadSprite[SPEC_PONTPIERRETOP]                = Load319;
		flpLoadSprite[SPEC_TENTE1]          		        = Load320;
		flpLoadSprite[SPEC_TENTE2]          		        = Load321;
		flpLoadSprite[SPEC_TENTE3]          		        = Load322;
      flpLoadSprite[SPEC_LANTERNE1]              = Load323;
      flpLoadSprite[SPEC_LH_BRIDGE]              = Load324;
      flpLoadSprite[SPEC_LH_BRIDGE_B]            = Load325;
      flpLoadSprite[SPEC_CASTLE_BRIDGE]          = Load326;
      flpLoadSprite[SPEC_CASTLE_BRIDGE_B1]       = Load326;
      flpLoadSprite[SPEC_CASTLE_BRIDGE_B2]       = Load326;
      flpLoadSprite[SPEC_CASTLE_BRIDGE_I]        = Load326;
      flpLoadSprite[SPEC_CASTLE_BRIDGE_B1_I]     = Load326;
      flpLoadSprite[SPEC_CASTLE_BRIDGE_B2_I]     = Load326;

      flpLoadSprite[SPEC_ESC_ESCALIER]           = Load327;
      flpLoadSprite[SPEC_ESC_ESCALIER_I]         = Load327;
      flpLoadSprite[SPEC_ESC_ESCALIER_C]         = Load327;
      flpLoadSprite[SPEC_ESC_TUILE1]             = Load327;
      flpLoadSprite[SPEC_ESC_TUILE2]             = Load327;
      flpLoadSprite[SPEC_ESC_TUILE3]             = Load327;
      flpLoadSprite[SPEC_ESC_TUILE4]             = Load327;

      flpLoadSprite[SPEC_SMALL_BRIDGE]           = Load328;
      flpLoadSprite[SPEC_SMALL_BRIDGE_I]         = Load328;

      flpLoadSprite[BIG_MOULIND]			    	 	 = Load329;
      flpLoadSprite[BIG_MOULING]			    	 	 = Load330;



      

      flpLoadSprite[WOODWALL_SE]                  = Load400;
      flpLoadSprite[WOODWALL_NE]                  = Load400;
      flpLoadSprite[WOODWALL_NE_WINDOW]           = Load400;
      flpLoadSprite[WOODWALL_SE_WINDOW]           = Load400;
      flpLoadSprite[WOODWALL_NE_DOORB]            = Load400;
      flpLoadSprite[WOODWALL_NE_DOORT]            = Load400;
      flpLoadSprite[WOODWALL_SE_DOORB]            = Load400;
      flpLoadSprite[WOODWALL_SE_DOORT]            = Load400;
      flpLoadSprite[WOODWALL_CORNER_R]            = Load400;
      flpLoadSprite[WOODWALL_CORNER_T]            = Load400;
      flpLoadSprite[WOODWALL_CORNER_L]            = Load400;
      flpLoadSprite[WOODWALL_CORNER_B]            = Load400; 
      flpLoadSprite[WOODWALL_CORNER_X]            = Load400;
      flpLoadSprite[WOODWALL_TNE]                 = Load400;
      flpLoadSprite[WOODWALL_TNW]                 = Load400;
      flpLoadSprite[WOODWALL_TSE]                 = Load400;
      flpLoadSprite[WOODWALL_TSW]                 = Load400;
      flpLoadSprite[WOODWALL_NE_MID]              = Load400;
	   flpLoadSprite[WOODWALL_SE_MID]              = Load400;

      flpLoadSprite[NBRICKWALL_SE]                  = Load401;
      flpLoadSprite[NBRICKWALL_NE]                  = Load401;
      flpLoadSprite[NBRICKWALL_NE_WINDOW]           = Load401;
      flpLoadSprite[NBRICKWALL_SE_WINDOW]           = Load401;
      flpLoadSprite[NBRICKWALL_NE_DOORB]            = Load401;
      flpLoadSprite[NBRICKWALL_NE_DOORT]            = Load401;
      flpLoadSprite[NBRICKWALL_SE_DOORB]            = Load401;
      flpLoadSprite[NBRICKWALL_SE_DOORT]            = Load401;
      flpLoadSprite[NBRICKWALL_CORNER_R]            = Load401;
      flpLoadSprite[NBRICKWALL_CORNER_T]            = Load401;
      flpLoadSprite[NBRICKWALL_CORNER_L]            = Load401;
      flpLoadSprite[NBRICKWALL_CORNER_B]            = Load401; 
      flpLoadSprite[NBRICKWALL_CORNER_X]            = Load401;
      flpLoadSprite[NBRICKWALL_TNE]                 = Load401;
      flpLoadSprite[NBRICKWALL_TNW]                 = Load401;
      flpLoadSprite[NBRICKWALL_TSE]                 = Load401;
      flpLoadSprite[NBRICKWALL_TSW]                 = Load401;

      flpLoadSprite[NBRICKWALLS_SE]                  = Load402;
      flpLoadSprite[NBRICKWALLS_NE]                  = Load402;
      flpLoadSprite[NBRICKWALLS_SE_WINDOW]           = Load402;
      flpLoadSprite[NBRICKWALLS_NE_WINDOW]           = Load402;
      flpLoadSprite[NBRICKWALLS_NE_DOORB]            = Load402;
      flpLoadSprite[NBRICKWALLS_NE_DOORT]            = Load402;
      flpLoadSprite[NBRICKWALLS_SE_DOORB]            = Load402;
      flpLoadSprite[NBRICKWALLS_SE_DOORT]            = Load402;
      flpLoadSprite[NBRICKWALLS_CORNER_R]            = Load402;
      flpLoadSprite[NBRICKWALLS_CORNER_T]            = Load402;
      flpLoadSprite[NBRICKWALLS_CORNER_L]            = Load402;
      flpLoadSprite[NBRICKWALLS_CORNER_B]            = Load402; 
      flpLoadSprite[NBRICKWALLS_CORNER_X]            = Load402;
      flpLoadSprite[NBRICKWALLS_TNE]                 = Load402;
      flpLoadSprite[NBRICKWALLS_TNW]                 = Load402;
      flpLoadSprite[NBRICKWALLS_TSE]                 = Load402;
      flpLoadSprite[NBRICKWALLS_TSW]                 = Load402;
      flpLoadSprite[NBRICKWALLS_SE_S]                = Load402;
      flpLoadSprite[NBRICKWALLS_SE_E]                = Load402;
      flpLoadSprite[NBRICKWALLS_NE_S]                = Load402;
      flpLoadSprite[NBRICKWALLS_NE_E]                = Load402;

      flpLoadSprite[NBRICKWALLS2_SE]                  = Load403;
      flpLoadSprite[NBRICKWALLS2_NE]                  = Load403;
      flpLoadSprite[NBRICKWALLS2_SE_WINDOW]           = Load403;
      flpLoadSprite[NBRICKWALLS2_NE_WINDOW]           = Load403;
      flpLoadSprite[NBRICKWALLS2_NE_DOORB]            = Load403;
      flpLoadSprite[NBRICKWALLS2_NE_DOORT]            = Load403;
      flpLoadSprite[NBRICKWALLS2_SE_DOORB]            = Load403;
      flpLoadSprite[NBRICKWALLS2_SE_DOORT]            = Load403;
      flpLoadSprite[NBRICKWALLS2_CORNER_R]            = Load403;
      flpLoadSprite[NBRICKWALLS2_CORNER_T]            = Load403;
      flpLoadSprite[NBRICKWALLS2_CORNER_L]            = Load403;
      flpLoadSprite[NBRICKWALLS2_CORNER_B]            = Load403; 
      flpLoadSprite[NBRICKWALLS2_CORNER_X]            = Load403;
      flpLoadSprite[NBRICKWALLS2_TNE]                 = Load403;
      flpLoadSprite[NBRICKWALLS2_TNW]                 = Load403;
      flpLoadSprite[NBRICKWALLS2_TSE]                 = Load403;
      flpLoadSprite[NBRICKWALLS2_TSW]                 = Load403;

      flpLoadSprite[GSPEC_CAVERN_I_ENTRANCE]		= Load404;
		flpLoadSprite[GSPEC_CAVERN_I_ENTRANCE_I]	= Load404;
		flpLoadSprite[GCAVERN_SPEC_1_NW_SE]			= Load404;
		flpLoadSprite[GCAVERN_SPEC_1_NE_SW]			= Load404;
		flpLoadSprite[GCAVERN_B_NE_SW]			   = Load404;
		flpLoadSprite[GCAVERN_NE_SW]					= Load404;
		flpLoadSprite[GCAVERN_B_NW_SE]				= Load404;
		flpLoadSprite[GCAVERN_NW_SE]					= Load404;
		flpLoadSprite[GCAVERN_WSE]						= Load404;
		flpLoadSprite[GCAVERN_WNE]						= Load404;
		flpLoadSprite[GCAVERN_NES]						= Load404;
		flpLoadSprite[GCAVERN_NWS]						= Load404;
		flpLoadSprite[GCAVERN_WSE_2]					= Load404;
		flpLoadSprite[GCAVERN_WNE_2]					= Load404;
		flpLoadSprite[GCAVERN_NES_2]					= Load404;
		flpLoadSprite[GCAVERN_NWS_2]					= Load404;
		flpLoadSprite[GCAVERN_SKIP_1]					= Load404;
		flpLoadSprite[GCAVERN_SKIP_2]					= Load404;
		flpLoadSprite[GCAVERN_SKIP_3]					= Load404;
		flpLoadSprite[GCAVERN_SKIP_4]					= Load404;

      flpLoadSprite[SPEC_GSTALAGTIC_1]				= Load405;
		flpLoadSprite[SPEC_GSTALAGTIC_2]				= Load406;
		flpLoadSprite[SPEC_GSTALAGTIC_3]				= Load407;
		flpLoadSprite[SPEC_GSTALAGTIC_4]				= Load408;
		flpLoadSprite[SPEC_GSTALAGTIC_5]				= Load409;
		flpLoadSprite[SPEC_GSTALAGTIC_6]				= Load410;
		flpLoadSprite[SPEC_GSTALAGTIC_7]				= Load411;
		flpLoadSprite[SPEC_GGROSSE_ROCHE_1]			= Load412;
		flpLoadSprite[SPEC_GGROSSE_ROCHE_2]			= Load413;
		flpLoadSprite[SPEC_GGROSSE_ROCHE_3]			= Load414;

      flpLoadSprite[SPEC_FOUNTAIN_G ]            = Load415;
      flpLoadSprite[SPEC_FOUNTAIN_GI]            = Load415;
      flpLoadSprite[SPEC_HUTE_FGOB_G ]           = Load416;
      flpLoadSprite[SPEC_HUTE_FGOB_GI]           = Load416;
      flpLoadSprite[SPEC_HUTE_GOB_G ]            = Load417;
      flpLoadSprite[SPEC_HUTE_GOB_GI]            = Load417;

      flpLoadSprite[SPEC_MONT_ENTRE_G ]          = Load418;
      flpLoadSprite[SPEC_MONT_ENTRE_GI]          = Load418;
      flpLoadSprite[SPEC_MONT_G ]                = Load419;
      flpLoadSprite[SPEC_MONT_GI]                = Load419;
      flpLoadSprite[SPEC_MONT2_G ]               = Load420;
      flpLoadSprite[SPEC_MONT2_GI]               = Load420;

      flpLoadSprite[NBRICKWALLS3_SE]                  = Load421;
      flpLoadSprite[NBRICKWALLS3_NE]                  = Load421;
      flpLoadSprite[NBRICKWALLS3_SE_WINDOW]           = Load421;
      flpLoadSprite[NBRICKWALLS3_NE_WINDOW]           = Load421;
      flpLoadSprite[NBRICKWALLS3_NE_DOORB]            = Load421;
      flpLoadSprite[NBRICKWALLS3_NE_DOORT]            = Load421;
      flpLoadSprite[NBRICKWALLS3_SE_DOORB]            = Load421;
      flpLoadSprite[NBRICKWALLS3_SE_DOORT]            = Load421;
      flpLoadSprite[NBRICKWALLS3_CORNER_R]            = Load421;
      flpLoadSprite[NBRICKWALLS3_CORNER_T]            = Load421;
      flpLoadSprite[NBRICKWALLS3_CORNER_L]            = Load421;
      flpLoadSprite[NBRICKWALLS3_CORNER_B]            = Load421; 
      flpLoadSprite[NBRICKWALLS3_CORNER_X]            = Load421;
      flpLoadSprite[NBRICKWALLS3_TNE]                 = Load421;
      flpLoadSprite[NBRICKWALLS3_TNW]                 = Load421;
      flpLoadSprite[NBRICKWALLS3_TSE]                 = Load421;
      flpLoadSprite[NBRICKWALLS3_TSW]                 = Load421;

      flpLoadSprite[NBRICKWALLS4_SE]                  = Load422;
      flpLoadSprite[NBRICKWALLS4_NE]                  = Load422;
      flpLoadSprite[NBRICKWALLS4_SE_WINDOW]           = Load422;
      flpLoadSprite[NBRICKWALLS4_NE_WINDOW]           = Load422;
      flpLoadSprite[NBRICKWALLS4_NE_DOORB]            = Load422;
      flpLoadSprite[NBRICKWALLS4_NE_DOORT]            = Load422;
      flpLoadSprite[NBRICKWALLS4_SE_DOORB]            = Load422;
      flpLoadSprite[NBRICKWALLS4_SE_DOORT]            = Load422;
      flpLoadSprite[NBRICKWALLS4_CORNER_R]            = Load422;
      flpLoadSprite[NBRICKWALLS4_CORNER_T]            = Load422;
      flpLoadSprite[NBRICKWALLS4_CORNER_L]            = Load422;
      flpLoadSprite[NBRICKWALLS4_CORNER_B]            = Load422; 
      flpLoadSprite[NBRICKWALLS4_CORNER_X]            = Load422;
      flpLoadSprite[NBRICKWALLS4_TNE]                 = Load422;
      flpLoadSprite[NBRICKWALLS4_TNW]                 = Load422;
      flpLoadSprite[NBRICKWALLS4_TSE]                 = Load422;
      flpLoadSprite[NBRICKWALLS4_TSW]                 = Load422;

      flpLoadSprite[NBRICKWALLS5_SE]                  = Load423;
      flpLoadSprite[NBRICKWALLS5_NE]                  = Load423;
      flpLoadSprite[NBRICKWALLS5_SE_WINDOW]           = Load423;
      flpLoadSprite[NBRICKWALLS5_NE_WINDOW]           = Load423;
      flpLoadSprite[NBRICKWALLS5_NE_DOORB]            = Load423;
      flpLoadSprite[NBRICKWALLS5_NE_DOORT]            = Load423;
      flpLoadSprite[NBRICKWALLS5_SE_DOORB]            = Load423;
      flpLoadSprite[NBRICKWALLS5_SE_DOORT]            = Load423;
      flpLoadSprite[NBRICKWALLS5_CORNER_R]            = Load423;
      flpLoadSprite[NBRICKWALLS5_CORNER_T]            = Load423;
      flpLoadSprite[NBRICKWALLS5_CORNER_L]            = Load423;
      flpLoadSprite[NBRICKWALLS5_CORNER_B]            = Load423; 
      flpLoadSprite[NBRICKWALLS5_CORNER_X]            = Load423;
      flpLoadSprite[NBRICKWALLS5_TNE]                 = Load423;
      flpLoadSprite[NBRICKWALLS5_TNW]                 = Load423;
      flpLoadSprite[NBRICKWALLS5_TSE]                 = Load423;
      flpLoadSprite[NBRICKWALLS5_TSW]                 = Load423;

      flpLoadSprite[NBRICKWALLS6_SE]                  = Load424;
      flpLoadSprite[NBRICKWALLS6_NE]                  = Load424;
      flpLoadSprite[NBRICKWALLS6_SE_WINDOW]           = Load424;
      flpLoadSprite[NBRICKWALLS6_NE_WINDOW]           = Load424;
      flpLoadSprite[NBRICKWALLS6_NE_DOORB]            = Load424;
      flpLoadSprite[NBRICKWALLS6_NE_DOORT]            = Load424;
      flpLoadSprite[NBRICKWALLS6_SE_DOORB]            = Load424;
      flpLoadSprite[NBRICKWALLS6_SE_DOORT]            = Load424;
      flpLoadSprite[NBRICKWALLS6_CORNER_R]            = Load424;
      flpLoadSprite[NBRICKWALLS6_CORNER_T]            = Load424;
      flpLoadSprite[NBRICKWALLS6_CORNER_L]            = Load424;
      flpLoadSprite[NBRICKWALLS6_CORNER_B]            = Load424; 
      flpLoadSprite[NBRICKWALLS6_CORNER_X]            = Load424;
      flpLoadSprite[NBRICKWALLS6_TNE]                 = Load424;
      flpLoadSprite[NBRICKWALLS6_TNW]                 = Load424;
      flpLoadSprite[NBRICKWALLS6_TSE]                 = Load424;
      flpLoadSprite[NBRICKWALLS6_TSW]                 = Load424;

      flpLoadSprite[NBRICKWALLRS3_CORNER_B ]          = Load425;
      flpLoadSprite[NBRICKWALLRS3_SE       ]          = Load425;
      flpLoadSprite[NBRICKWALLRS3_NE       ]          = Load425;
      flpLoadSprite[NBRICKWALLRS3_SE_WINDOW]          = Load425;
      flpLoadSprite[NBRICKWALLRS3_NE_WINDOW]          = Load425;

      flpLoadSprite[NBRICKWALLRS4_CORNER_B ]          = Load426;
      flpLoadSprite[NBRICKWALLRS4_SE       ]          = Load426;
      flpLoadSprite[NBRICKWALLRS4_NE       ]          = Load426;
      flpLoadSprite[NBRICKWALLRS4_SE_WINDOW]          = Load426;
      flpLoadSprite[NBRICKWALLRS4_NE_WINDOW]          = Load426;

      flpLoadSprite[NBRICKWALLRS5_CORNER_B ]          = Load427;
      flpLoadSprite[NBRICKWALLRS5_SE       ]          = Load427;
      flpLoadSprite[NBRICKWALLRS5_NE       ]          = Load427;
      flpLoadSprite[NBRICKWALLRS5_SE_WINDOW]          = Load427;
      flpLoadSprite[NBRICKWALLRS5_NE_WINDOW]          = Load427;

      flpLoadSprite[NBRICKWALLRS6_CORNER_B ]          = Load428;
      flpLoadSprite[NBRICKWALLRS6_SE       ]          = Load428;
      flpLoadSprite[NBRICKWALLRS6_NE       ]          = Load428;
      flpLoadSprite[NBRICKWALLRS6_SE_WINDOW]          = Load428;
      flpLoadSprite[NBRICKWALLRS6_NE_WINDOW]          = Load428;

      flpLoadSprite[SPEC_BARILPIED]                   = Load429,
      flpLoadSprite[SPEC_BARILPIED_I]                 = Load429,
      flpLoadSprite[SPEC_TOUR_FRONT]                  = Load430,
      flpLoadSprite[SPEC_TOUR_FRONT_I]                = Load430,
      flpLoadSprite[SPEC_TOUR_BACK]                   = Load431,
      flpLoadSprite[SPEC_TOUR_BACK_I]                 = Load431,

      flpLoadSprite[V2_FLOWER_01]                     = Load432,
      flpLoadSprite[V2_FLOWER_02]                     = Load432,
      flpLoadSprite[V2_FLOWER_03]                     = Load432,
      flpLoadSprite[V2_FLOWER_04]                     = Load432,
      flpLoadSprite[V2_FLOWER_05]                     = Load432,
      flpLoadSprite[V2_FLOWER_06]                     = Load432,
      flpLoadSprite[V2_FLOWER_07]                     = Load432,


      flpLoadSprite[V2CASTLEWALL_SE]                  = Load433,
      flpLoadSprite[V2CASTLEWALL_NE]                  = Load433,
      flpLoadSprite[V2CASTLEWALL_NE_DOORB]            = Load433,
      flpLoadSprite[V2CASTLEWALL_NE_DOORT]            = Load433,
      flpLoadSprite[V2CASTLEWALL_SE_DOORB]            = Load433,
      flpLoadSprite[V2CASTLEWALL_SE_DOORT]            = Load433,
      flpLoadSprite[V2CASTLEWALL_CORNER_R]            = Load433,
      flpLoadSprite[V2CASTLEWALL_CORNER_T]            = Load433,
      flpLoadSprite[V2CASTLEWALL_CORNER_L]            = Load433,
      flpLoadSprite[V2CASTLEWALL_CORNER_B]            = Load433, 
      flpLoadSprite[V2CASTLEWALL_CORNER_X]            = Load433,
      flpLoadSprite[V2CASTLEWALL_TNE]                 = Load433,
      flpLoadSprite[V2CASTLEWALL_TNW]                 = Load433,
      flpLoadSprite[V2CASTLEWALL_TSE]                 = Load433,
      flpLoadSprite[V2CASTLEWALL_TSW]                 = Load433, 
      flpLoadSprite[V2CASTLEWALL_POUTRE]              = Load433,
      flpLoadSprite[V2CASTLEWALL_POUTRE_I]            = Load433,
      flpLoadSprite[V2CASTLEWALL_POUTRE2]             = Load433,
      flpLoadSprite[V2CASTLEWALL_POUTRE3]             = Load433,
      flpLoadSprite[V2CASTLEWALL_NE_DDOORB]           = Load433,
      flpLoadSprite[V2CASTLEWALL_NE_DDOORT]           = Load433,
      flpLoadSprite[V2CASTLEWALL_SE_DDOORB]           = Load433,
      flpLoadSprite[V2CASTLEWALL_SE_DDOORT]           = Load433,
      

      flpLoadSprite[V2CASTLE_RAMPART_SE]              = Load434,
	   flpLoadSprite[V2CASTLE_RAMPART_NE]              = Load434,
      flpLoadSprite[V2CASTLE_RAMPART_TOWER_SE]        = Load434,
      flpLoadSprite[V2CASTLE_RAMPART_TOWER_NE]        = Load434,
	   flpLoadSprite[V2CASTLE_RAMPART_COIN_L]          = Load434,
      flpLoadSprite[V2CASTLE_RAMPART_COIN_T]          = Load434,
      flpLoadSprite[V2CASTLE_RAMPART_COIN_R]          = Load434,
	   flpLoadSprite[V2CASTLE_RAMPART_COIN_B]          = Load434,
      flpLoadSprite[V2CASTLE_RAMPART_END_D]           = Load434,
      flpLoadSprite[V2CASTLE_RAMPART_END_F]           = Load434,
      flpLoadSprite[V2CASTLE_COLLONNE]                = Load434,

      flpLoadSprite[CEDRE_NAIN]                       = Load435,

      flpLoadSprite[V2SCASTLEWALL_SE]                 = Load436,
      flpLoadSprite[V2SCASTLEWALL_NE]                 = Load436,
      flpLoadSprite[V2SCASTLEWALL_MERGE1]             = Load436,
      flpLoadSprite[V2SCASTLEWALL_MERGE2]             = Load436,
      flpLoadSprite[V2SCASTLEWALL_MERGE3]             = Load436,
      flpLoadSprite[V2SCASTLEWALL_MERGE4]             = Load436,
      flpLoadSprite[V2SCASTLEWALL_CORNER_R]           = Load436,
      flpLoadSprite[V2SCASTLEWALL_CORNER_T]           = Load436,
      flpLoadSprite[V2SCASTLEWALL_CORNER_L]           = Load436,
      flpLoadSprite[V2SCASTLEWALL_CORNER_B]           = Load436, 
      flpLoadSprite[V2SCASTLEWALL_CORNER_X]           = Load436,
      flpLoadSprite[V2SCASTLEWALL_TNE]                = Load436,
      flpLoadSprite[V2SCASTLEWALL_TNW]                = Load436,
      flpLoadSprite[V2SCASTLEWALL_TSE]                = Load436,
      flpLoadSprite[V2SCASTLEWALL_TSW]                = Load436, 

      flpLoadSprite[SKELETON_DRAKE01]                = Load1291;
      flpLoadSprite[SKELETON_DRAKE02]                = Load1292;
      flpLoadSprite[SKELETON_DRAKE03]                = Load1293;
      flpLoadSprite[SKELETON_DRAKE04]                = Load1294;

      flpLoadSprite[BANC_NOIR1]                      = Load4251; 
      flpLoadSprite[BANC_NOIR1_I]                    = Load4251; 
      
      flpLoadSprite[MUMMY_BOX1]                      = Load4253; 
      flpLoadSprite[MUMMY_BOX1_I]                    = Load4253; 
      flpLoadSprite[MUMMY_BOX2]                      = Load4253; 
      flpLoadSprite[MUMMY_BOX2_I]                    = Load4253;

      flpLoadSprite[SQUARE_TOWER1]                   = Load4257; 
      flpLoadSprite[SQUARE_TOWER1_I]                 = Load4257; 
      flpLoadSprite[SQUARE_TOWER2]                   = Load4257; 
      flpLoadSprite[SQUARE_TOWER2_I]                 = Load4257;

      flpLoadSprite[STATUE_MODEL1_1]                 = Load4261;
      flpLoadSprite[STATUE_MODEL1_2]                 = Load4261;
      flpLoadSprite[STATUE_MODEL1_3]                 = Load4261;
      flpLoadSprite[STATUE_MODEL1_4]                 = Load4261;
      flpLoadSprite[STATUE_MODEL2_1]                 = Load4261;
      flpLoadSprite[STATUE_MODEL2_2]                 = Load4261;
      flpLoadSprite[STATUE_MODEL2_3]                 = Load4261;
      flpLoadSprite[STATUE_MODEL2_4]                 = Load4261;

      flpLoadSprite[NSTUCOWALL_SE       ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_NE       ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_NE_WINDOWO]             = Load4269;
      flpLoadSprite[NSTUCOWALL_SE_WINDOWO]             = Load4269;
      flpLoadSprite[NSTUCOWALL_NE_WINDOWE]             = Load4269;
      flpLoadSprite[NSTUCOWALL_SE_WINDOWE]             = Load4269;
      flpLoadSprite[NSTUCOWALL_NE_DOORB ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_NE_DOORT ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_SE_DOORB ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_SE_DOORT ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_CORNER_R ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_CORNER_T ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_CORNER_L ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_CORNER_B ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_CORNER_X ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_TNE      ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_TNW      ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_TSE      ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_TSW      ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_PSE      ]              = Load4269;
      flpLoadSprite[NSTUCOWALL_PNE      ]              = Load4269;

      flpLoadSprite[MUR_POUTRE01        ]              = Load4290;
      flpLoadSprite[MUR_POUTRE01        ]              = Load4290;
      flpLoadSprite[MUR_POUTRE01        ]              = Load4290;
      flpLoadSprite[MUR_POUTRE01        ]              = Load4290;
      flpLoadSprite[MUR_POUTRE01        ]              = Load4290;
      flpLoadSprite[MUR_POUTRE01        ]              = Load4290;
      flpLoadSprite[MUR_POUTRE01        ]              = Load4290;

      flpLoadSprite[TOUR_GARDE_HG       ]              = Load4297;
      flpLoadSprite[TOUR_GARDE_HD       ]              = Load4297;
      flpLoadSprite[TOUR_GARDE_BG       ]              = Load4297;
      flpLoadSprite[TOUR_GARDE_BD       ]              = Load4297;

      
     
      
      

      flpLoadSprite[FLOOR_SNOW]						     = Load6000;
      flpLoadSprite[FLOOR_EARTHI]   				     = Load6001;
      flpLoadSprite[FLOOR_CAVERNG]   				     = Load6002;
      flpLoadSprite[FLOOR_ICE]                       = Load6003;
      flpLoadSprite[FLOOR_TMPL1]                     = Load6004;
      flpLoadSprite[FLOOR_TMPL2]                     = Load6005;
      flpLoadSprite[FLOOR_ROCK2]                     = Load6006;
      flpLoadSprite[FLOOR_TMPL1S]                    = Load6007;
      flpLoadSprite[FLOOR_TMPL2S]                    = Load6008;
      flpLoadSprite[FLOOR_ANIM_WATER01]              = Load6009;
      flpLoadSprite[FLOOR_NEWLAVA]                   = Load6011;
      flpLoadSprite[FLOOR_CENDRE]   				     = Load6013;


      flpLoadSprite[ROCFLO2_1]					   	  = Load7261;
		flpLoadSprite[ROCFLO2_2]						     = Load7261;
		flpLoadSprite[ROCFLO2_3]					   	  = Load7261;
		flpLoadSprite[ROCFLO2_4]					   	  = Load7261;
      flpLoadSprite[ROCFLO3_1]						     = Load7265;
		flpLoadSprite[ROCFLO3_2]					    	  = Load7265;
		flpLoadSprite[ROCFLO3_3]					   	  = Load7265;
		flpLoadSprite[ROCFLO3_4]					   	  = Load7265;
      flpLoadSprite[ROCFLO4_1]						     = Load7269;
		flpLoadSprite[ROCFLO4_2]					    	  = Load7269;
		flpLoadSprite[ROCFLO4_3]					   	  = Load7269;
		flpLoadSprite[ROCFLO4_4]					   	  = Load7269;
      flpLoadSprite[ROCFLO5_1]						     = Load7273;
		flpLoadSprite[ROCFLO5_2]					    	  = Load7273;
		flpLoadSprite[ROCFLO5_3]					   	  = Load7273;
		flpLoadSprite[ROCFLO5_4]					   	  = Load7273;
      flpLoadSprite[ROCFLO6_1]						     = Load7277;
		flpLoadSprite[ROCFLO6_2]					    	  = Load7277;
		flpLoadSprite[ROCFLO6_3]					   	  = Load7277;
		flpLoadSprite[ROCFLO6_4]					   	  = Load7277;
      

      AllSmoothLoad(GRASSSNOW    , Load6500);
      AllSmoothLoad(EARTHSNOW    , Load6532);
      AllSmoothLoad(SNOWEARTHI   , Load6564);
      AllSmoothLoad(GRASSEARTHI  , Load6596);
      AllSmoothLoad(EARTHEARTHI  , Load6628);
      AllSmoothLoad(SNOWCAVG     , Load6660);
      AllSmoothLoad(EARTHICAVG   , Load6692);
      AllSmoothLoad(ICEWATER     , Load6724);
      AllSmoothLoad(ICESNOW      , Load6756);
      AllSmoothLoad3(GRAFL1      , Load6788);
      AllSmoothLoad3(ROCFL1      , Load6792);
      AllSmoothLoad3(SNOFL1      , Load6816);
      AllSmoothLoad3(GRAFL2      , Load6796);
      AllSmoothLoad3(ROCFL2      , Load6800);
      AllSmoothLoad3(SNOFL2      , Load6820);

      AllSmoothLoad3(WO2FL2      , Load6804);
      AllSmoothLoad3(TUIFL2      , Load6808);
      AllSmoothLoad3(TUITU2      , Load6812);


      AllSmoothLoad(FLO2GRA, Load7003);
		AllSmoothLoad(FLO2FOR, Load7035);
		AllSmoothLoad(FLO2TER, Load7057);
		AllSmoothLoad(FLO2DES, Load7089);
      AllSmoothLoad(FLO3GRA, Load7122);
		AllSmoothLoad(FLO3FOR, Load7154);
		AllSmoothLoad(FLO3TER, Load7176);
		AllSmoothLoad(FLO3DES, Load7209);

      AllSmoothLoad(SNOROC , Load7251);
      AllSmoothLoad3(SNOWO1, Load7313);
      AllSmoothLoad3(SNOWO2, Load7317);
      AllSmoothLoad3(SNOWO3, Load7321);

      AllSmoothLoad3(RC2TM1, Load7325);


      AllSmoothLoad3(RC2TS1, Load7329);
      AllSmoothLoad3(RC2TS2, Load7333);
      AllSmoothLoad3(GRATS1, Load7337);
      AllSmoothLoad3(GRATS2, Load7341);
      AllSmoothLoad3(ROCTS1, Load7345);
      AllSmoothLoad3(ROCTS2, Load7349);
      AllSmoothLoad3(SNOTS1, Load7353);
      AllSmoothLoad3(SNOTS2, Load7357);
      AllSmoothLoad3(TS1TS2, Load7361);
      AllSmoothLoad3(WO1TS1, Load7365);
      AllSmoothLoad3(WO1TS2, Load7369);
      AllSmoothLoad3(WO2TS1, Load7373);
      AllSmoothLoad3(WO2TS2, Load7377);
      AllSmoothLoad3(WO3TS1, Load7381);
      AllSmoothLoad3(WO3TS2, Load7385);
      AllSmoothLoad3(TM1TS1, Load7389);
      AllSmoothLoad3(TM2TS2, Load7393);

      AllSmoothLoad(WATROC  , Load7397);
  	   AllSmoothLoad(WA1WA2  , Load7429);
      AllSmoothLoad(GRAROCK , Load7461);
      AllSmoothLoad(CENROC  , Load7493);
      AllSmoothLoad(CENCAV  , Load7525);
      AllSmoothLoad(CENWAT  , Load7557);
      
      
      

      AllSmoothLoad(AWAT01TER, Load7850);
      AllSmoothLoad(AWAT01ROC, Load7882);
      AllSmoothLoad(AWAT01RC2, Load7914);

      AllSmoothLoad(AWAT01DES, Load7946);
      AllSmoothLoad(AWAT01GRA, Load7978);
      AllSmoothLoad(AWAT01FOR, Load8010);
      
      AllSmoothLoad(LAV01CEN, Load8042);
      
      
            



    
      /*      
          Rampart[2].LoadSprite("Rampart 7");
          Rampart[3].LoadSprite("Rampart 1");
          WatSmooth[0].LoadSprite("Watemp 1");
          WatSmooth[1].LoadSprite("Watemp 2");
          WatSmooth[2].LoadSprite("Watemp 3");
          WatSmooth[3].LoadSprite("Watemp 4");
          WatSmooth[4].LoadSprite("Watemp 5");
          WatSmooth[5].LoadSprite("Watemp 6");
          WatSmooth[6].LoadSprite("Watemp 7");
          WatSmooth[7].LoadSprite("Watemp 8");
          WatSmooth[8].LoadSprite("Watemp 9");
          WatSmooth[9].LoadSprite("Watemp 10");
          WatSmooth[10].LoadSprite("Watemp 11");
          WatSmooth[11].LoadSprite("Watemp 12");
          WatSmooth[12].LoadSprite("Watemp 13");
          WatSmooth[13].LoadSprite("Watemp 14");
          WatSmooth[14].LoadSprite("Watemp 15");
          WatSmooth[15].LoadSprite("Watemp 16");
          WatSmooth[16].LoadSprite("Watemp 17");
          WatSmooth[17].LoadSprite("Watemp 18");
          WatSmooth[18].LoadSprite("Watemp 19");
          WatSmooth[19].LoadSprite("Watemp 20");
          WatSmooth[20].LoadSprite("Watemp 21");
          WatSmooth[21].LoadSprite("Watemp 22");
          WatSmooth[22].LoadSprite("Watemp 23");
          WatSmooth[23].LoadSprite("Watemp 24");
          WatSmooth[24].LoadSprite("Watemp 25");
          WatSmooth[25].LoadSprite("Watemp 26");
          WatSmooth[26].LoadSprite("Watemp 27");
          WatSmooth[27].LoadSprite("Watemp 28");
          WatSmooth[28].LoadSprite("Watemp 29");
          WatSmooth[29].LoadSprite("Watemp 30");
          WatSmooth[30].LoadSprite("Watemp 31");
          WatSmooth[31].LoadSprite("Watemp 32");
          
            Object[140].LoadSprite("DungeonDoor");
      */

		Border[4].LoadSprite("Status");
		Border[5].LoadSprite("Corner");

		Pp1.LoadSprite("pp 1");
		Pp2.LoadSprite("pp 2");

		HPString = new char [strlen(g_LocalString[156])+1];
		strcpy(HPString, g_LocalString[156]);
		MPString = new char [strlen(g_LocalString[157])+1];
		strcpy(MPString, g_LocalString[157]);
		XPString = new char [strlen(g_LocalString[158])+1];
		strcpy(XPString, g_LocalString[158]);
 
      //////////////////////////
      // NightMare --- Add 2005-03-17
      // Load Weather Sprite...
      m_VSFWeatherRain        .LoadSprite("Weather_Rain");
      m_VSFWeatherLightning[0].LoadSprite("lightning1");
      m_VSFWeatherLightning[1].LoadSprite("lightning2");

      m_VSFWeatherSnow1        .LoadSprite("Weather_Snow");
      m_VSFWeatherSnow2        .LoadSprite("Weather_Snow2");

      m_VSFWeatherRain1       .LoadSprite("Weather_Rain0");
      m_VSFWeatherSemiTransFog.LoadSprite("Weather_Fog");

      //END NightMare
      //////////////////////////

//#ifdef _DEBUG
   } else {
		//ASSERT(FALSE);
   }
//#endif
}

void GraphicStructure::Draw(int TileSet,int xScreen, int yScreen, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE lpSurface, int a) {
		
   switch (TileSet-WallSet) {
      case NW_SE:
			World.CustomDarkWall[iDarkWall][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case NE_SW: 
			World.CustomWall[iWall][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

      case B_SE:
         World.CustomBorder[iBorder][3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

      case B_SW:
         World.CustomBorder[iBorder][1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

      case B_NE:
         World.CustomBorder[iBorder][0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
                                 
      case B_NW:                      
         World.CustomBorder[iBorder][2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      
      case NW_SE_EXCP_1:
         World.CustomDarkExcp1[iDarkExcp1][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
		
      case NE_SW_EXCP_1:
			World.CustomExcp1[iExcp1][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case NW_SE_EXCP_2:                
         World.CustomDarkExcp2[iDarkExcp2][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
		
      case NE_SW_EXCP_2:
			World.CustomExcp2[iExcp2][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case NW_SE_EXCP_3:
         World.CustomDarkExcp3[iDarkExcp3][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
		
      case NE_SW_EXCP_3:
			World.CustomExcp3[iExcp3][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case NW_SE_EXCP_4:
         World.CustomDarkExcp4[iDarkExcp4][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
		
      case NE_SW_EXCP_4:
			World.CustomExcp4[iExcp4][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case NW_SE_EXCP_5:
         World.CustomDarkExcp5[iDarkExcp5][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
		
      case NE_SW_EXCP_5:
			World.CustomExcp5[iExcp5][((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%iG)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case WNE: 
         World.CustomCorner[iCorner][2].DrawSprite(xScreen-(((iG-1)/2)*32), yScreen+(((iG-1)/2)*16), Clip, OutBound, Wrap, lpSurface);
		break;
		
		case NWS: 
         World.CustomCorner[iCorner][0].DrawSprite(xScreen, yScreen-(((iG-1)/2)*16), Clip, OutBound, Wrap, lpSurface);
		break;

		case NES: 
         World.CustomCorner[iCorner][1].DrawSprite(xScreen-(((iG-1)/2)*32), yScreen-(((iG-1)/2)*16), Clip, OutBound, Wrap, lpSurface);
		break;

		case WSE: 
         World.CustomCorner[iCorner][3].DrawSprite(xScreen-(((iG-1)/2)*32), yScreen-(((iG-1)/2)*16), Clip, OutBound, Wrap, lpSurface);
		break;
   }
}

void GraphicStructure::Load(void) {
   char Temp[100];

   // CORNER
   strcpy(Temp, Name);
   strcat(Temp, "Corner");
	World.CustomCorner[iCorner][0].LoadSprite(Temp);
   strcpy(Temp, Name);
   strcat(Temp, "Corner2");
	World.CustomCorner[iCorner][1].LoadSprite(Temp);
   strcpy(Temp, Name);
   strcat(Temp, "CornerN");
	World.CustomCorner[iCorner][2].LoadSprite(Temp);
   strcpy(Temp, Name);
   strcat(Temp, "CornerV");
	World.CustomCorner[iCorner][3].LoadSprite(Temp);
      
   // BORDER
   strcpy(Temp, Name);
   strcat(Temp, "Border1 1");
	World.CustomBorder[iBorder][0].LoadSprite(Temp);
   strcpy(Temp, Name);
   strcat(Temp, "Border2 1");
	World.CustomBorder[iBorder][1].LoadSprite(Temp);
   strcpy(Temp, Name);
   strcat(Temp, "Border1Dark 1");
	World.CustomBorder[iBorder][2].LoadSprite(Temp);
   strcpy(Temp, Name);
   strcat(Temp, "Border2Dark 1");
	World.CustomBorder[iBorder][3].LoadSprite(Temp);
      
   // WALL
   strcpy(Temp, Name);
   strcat(Temp, " 1");
      World.CustomWall[iWall][0].LoadSprite(Temp);
   if (iG >= 3) {
      strcpy(Temp, Name);
      strcat(Temp, " 2");
      World.CustomWall[iWall][1].LoadSprite(Temp);
      strcpy(Temp, Name);
      strcat(Temp, " 3");
      World.CustomWall[iWall][2].LoadSprite(Temp);
   }

   // WALL
   strcpy(Temp, Name);
   strcat(Temp, "Dark 1");
   World.CustomDarkWall[iDarkWall][0].LoadSprite(Temp);
   if (iG >= 3) {
      strcpy(Temp, Name);
      strcat(Temp, "Dark 2");
      World.CustomDarkWall[iDarkWall][1].LoadSprite(Temp);
      strcpy(Temp, Name);
      strcat(Temp, "Dark 3");
      World.CustomDarkWall[iDarkWall][2].LoadSprite(Temp);
   }

   if (iE >= 1) {
      // EXCP 1
      strcpy(Temp, Name);
      strcat(Temp, "Exception1 1");
      World.CustomExcp1[iExcp1][0].LoadSprite(Temp);
      if (iG >= 3) {
         strcpy(Temp, Name);
         strcat(Temp, "Exception1 2");
         World.CustomExcp1[iExcp1][1].LoadSprite(Temp);
         strcpy(Temp, Name);
         strcat(Temp, "Exception1 3");
         World.CustomExcp1[iExcp1][2].LoadSprite(Temp);
      }

      // EXCP 1
      strcpy(Temp, Name);
      strcat(Temp, "Exception1Dark 1");
      World.CustomDarkExcp1[iDarkExcp1][0].LoadSprite(Temp);
      if (iG >= 3) {
         strcpy(Temp, Name);
         strcat(Temp, "Exception1Dark 2");
         World.CustomDarkExcp1[iDarkExcp1][1].LoadSprite(Temp);
         strcpy(Temp, Name);
         strcat(Temp, "Exception1Dark 3");
         World.CustomDarkExcp1[iDarkExcp1][2].LoadSprite(Temp);
      }
   }

   if (iE >= 2) {
      // EXCP 2
      strcpy(Temp, Name);
      strcat(Temp, "Exception2 1");
      World.CustomExcp2[iExcp2][0].LoadSprite(Temp);
      if (iG >= 3) {
         strcpy(Temp, Name);
         strcat(Temp, "Exception2 2");
         World.CustomExcp2[iExcp2][1].LoadSprite(Temp);
         strcpy(Temp, Name);
         strcat(Temp, "Exception2 3");
         World.CustomExcp2[iExcp2][2].LoadSprite(Temp);
      }

      // EXCP 2
      strcpy(Temp, Name);
      strcat(Temp, "Exception2Dark 1");
      World.CustomDarkExcp2[iDarkExcp2][0].LoadSprite(Temp);
      if (iG >= 3) {
         strcpy(Temp, Name);
         strcat(Temp, "Exception2Dark 2");
         World.CustomDarkExcp2[iDarkExcp2][1].LoadSprite(Temp);
         strcpy(Temp, Name);
         strcat(Temp, "Exception2Dark 3");
         World.CustomDarkExcp2[iDarkExcp2][2].LoadSprite(Temp);
      }
   }

   if (iE >= 3) {
      // EXCP 3
      strcpy(Temp, Name);
      strcat(Temp, "Exception3 1");
      World.CustomExcp3[iExcp3][0].LoadSprite(Temp);
      if (iG >= 3) {
         strcpy(Temp, Name);
         strcat(Temp, "Exception3 2");
         World.CustomExcp3[iExcp3][1].LoadSprite(Temp);
         strcpy(Temp, Name);
         strcat(Temp, "Exception3 3");
         World.CustomExcp3[iExcp3][2].LoadSprite(Temp);
      }

      // EXCP 3
      strcpy(Temp, Name);
      strcat(Temp, "Exception3Dark 1");
      World.CustomDarkExcp3[iDarkExcp3][0].LoadSprite(Temp);
      if (iG >= 3) {
         strcpy(Temp, Name);
         strcat(Temp, "Exception3Dark 2");
         World.CustomDarkExcp3[iDarkExcp3][1].LoadSprite(Temp);
         strcpy(Temp, Name);
         strcat(Temp, "Exception3Dark 3");
         World.CustomDarkExcp3[iDarkExcp3][2].LoadSprite(Temp);
      }
   }

   if (iE >= 4) {
      // EXCP 4
      strcpy(Temp, Name);
      strcat(Temp, "Exception4 1");
      World.CustomExcp4[iExcp4][0].LoadSprite(Temp);
      if (iG >= 3) {
         strcpy(Temp, Name);
         strcat(Temp, "Exception4 2");
         World.CustomExcp4[iExcp4][1].LoadSprite(Temp);
         strcpy(Temp, Name);
         strcat(Temp, "Exception1 3");
         World.CustomExcp4[iExcp4][2].LoadSprite(Temp);
      }

      // EXCP 4
      strcpy(Temp, Name);
      strcat(Temp, "Exception4Dark 1");
      World.CustomDarkExcp4[iDarkExcp4][0].LoadSprite(Temp);
      if (iG >= 3) {
         strcpy(Temp, Name);
         strcat(Temp, "Exception4Dark 2");
         World.CustomDarkExcp4[iDarkExcp4][1].LoadSprite(Temp);
         strcpy(Temp, Name);
         strcat(Temp, "Exception4Dark 3");
         World.CustomDarkExcp4[iDarkExcp4][2].LoadSprite(Temp);
      }
   }

   if (iE >= 5) {
      // EXCP 5
      strcpy(Temp, Name);
      strcat(Temp, "Exception5 1");
      World.CustomExcp5[iExcp5][0].LoadSprite(Temp);
      if (iG >= 3) {
         strcpy(Temp, Name);
         strcat(Temp, "Exception5 2");
         World.CustomExcp5[iExcp5][1].LoadSprite(Temp);
         strcpy(Temp, Name);
         strcat(Temp, "Exception5 3");
         World.CustomExcp5[iExcp5][2].LoadSprite(Temp);
      }

      // EXCP 5
      strcpy(Temp, Name);
      strcat(Temp, "Exception5Dark 1");
      World.CustomDarkExcp5[iDarkExcp5][0].LoadSprite(Temp);
      if (iG >= 3) {
         strcpy(Temp, Name);
         strcat(Temp, "Exception5Dark 2");
         World.CustomDarkExcp5[iDarkExcp5][1].LoadSprite(Temp);
         strcpy(Temp, Name);
         strcat(Temp, "Exception5Dark 3");
         World.CustomDarkExcp5[iDarkExcp5][2].LoadSprite(Temp);
      }
   }
}


void TileSet::Register(int WALLSET, char *Name, int iG, int iE) {
   GraphicStructure *gsNew = new GraphicStructure;
   flpDeletePtr[iNbGSPtr] = gsNew;
   ++iNbGSPtr;

	flpLoadSprite2[WALLSET+WNE]				= gsNew;
	flpLoadSprite2[WALLSET+NWS]				= gsNew;
	flpLoadSprite2[WALLSET+NES]				= gsNew;
	flpLoadSprite2[WALLSET+WSE]				= gsNew;
	flpLoadSprite2[WALLSET+NW_SE]				= gsNew;
	flpLoadSprite2[WALLSET+NE_SW]				= gsNew;
	flpLoadSprite2[WALLSET+B_NW]				= gsNew;
	flpLoadSprite2[WALLSET+B_NE]				= gsNew;
	flpLoadSprite2[WALLSET+B_SE]				= gsNew;
	flpLoadSprite2[WALLSET+B_SW]				= gsNew;
   if (iE >= 1) {
      flpLoadSprite2[WALLSET+NW_SE_EXCP_1]	= gsNew;
	   flpLoadSprite2[WALLSET+NE_SW_EXCP_1]	= gsNew;
   }
   if (iE >= 2) {
      flpLoadSprite2[WALLSET+NW_SE_EXCP_2]	= gsNew;
	   flpLoadSprite2[WALLSET+NE_SW_EXCP_2]	= gsNew;
   }
   if (iE >= 3) {
	   flpLoadSprite2[WALLSET+NW_SE_EXCP_3]	= gsNew;
	   flpLoadSprite2[WALLSET+NE_SW_EXCP_3]	= gsNew;
   }
   if (iE >= 4) {
	   flpLoadSprite2[WALLSET+NW_SE_EXCP_4]	= gsNew;
	   flpLoadSprite2[WALLSET+NE_SW_EXCP_4]	= gsNew;
   }
   if (iE >= 5) {
	   flpLoadSprite2[WALLSET+NW_SE_EXCP_5]	= gsNew;
	   flpLoadSprite2[WALLSET+NE_SW_EXCP_5]	= gsNew;
   }
  
   CustomCorner[iCustomCorner] = new CV2Sprite [4];
   CustomBorder[iCustomBorder] = new CV2Sprite [4];
   CustomWall[iCustomWall] = new CV2Sprite [iG];
   CustomDarkWall[iCustomDarkWall] = new CV2Sprite [iG];
   if (iE >= 1) {
      CustomExcp1[iCustomExcp1] = new CV2Sprite [iG];
      CustomDarkExcp1[iCustomDarkExcp1] = new CV2Sprite [iG];
   }
   if (iE >= 2) {
      CustomExcp2[iCustomExcp2] = new CV2Sprite [iG];
      CustomDarkExcp2[iCustomDarkExcp2] = new CV2Sprite [iG];
   }
   if (iE >= 3) {
      CustomExcp3[iCustomExcp3] = new CV2Sprite [iG];
      CustomDarkExcp3[iCustomDarkExcp3] = new CV2Sprite [iG];
   }
   if (iE >= 4) {
      CustomExcp4[iCustomExcp4] = new CV2Sprite [iG];
      CustomDarkExcp4[iCustomDarkExcp4] = new CV2Sprite [iG];
   }
   if (iE >= 5) {
      CustomExcp5[iCustomExcp5] = new CV2Sprite [iG];
      CustomDarkExcp5[iCustomDarkExcp5] = new CV2Sprite [iG];
   }

   gsNew->WallSet       = WALLSET;
   gsNew->iCorner       = iCustomCorner;
   gsNew->iBorder       = iCustomBorder;
   gsNew->iWall         = iCustomWall;
   gsNew->iDarkWall     = iCustomDarkWall;
   gsNew->iExcp1        = iCustomExcp1;
   gsNew->iDarkExcp1    = iCustomDarkExcp1;
   gsNew->iExcp2        = iCustomExcp2;
   gsNew->iDarkExcp2    = iCustomDarkExcp2;
   gsNew->iExcp3        = iCustomExcp3;
   gsNew->iDarkExcp3    = iCustomDarkExcp3;
   gsNew->iExcp4        = iCustomExcp4;
   gsNew->iDarkExcp4    = iCustomDarkExcp4;
   gsNew->iExcp5        = iCustomExcp5;
   gsNew->iDarkExcp5    = iCustomDarkExcp5;
   gsNew->iG            = iG;
   gsNew->iE            = iE;
   
   gsNew->Name = new char [strlen(Name)+1];
   strcpy(gsNew->Name, Name);
   
   ++iCustomCorner;
   ++iCustomBorder;
   ++iCustomWall;
   ++iCustomDarkWall;
   ++iCustomExcp1;
   ++iCustomDarkExcp1;
   ++iCustomExcp2;
   ++iCustomDarkExcp2;
   ++iCustomExcp3;
   ++iCustomDarkExcp3;
   ++iCustomExcp4;
   ++iCustomDarkExcp4;
   ++iCustomExcp5;
   ++iCustomDarkExcp5;

   if ( iCustomCorner    >= MAXCUSTOMCORNER    ||
        iCustomBorder    >= MAXCUSTOMBORDER    ||
        iCustomWall      >= MAXCUSTOMWALL      ||
        iCustomDarkWall  >= MAXCUSTOMDARKWALL  ||
        iCustomExcp1     >= MAXCUSTOMEXCP1     ||
        iCustomDarkExcp1 >= MAXCUSTOMDARKEXCP1 ||
        iCustomExcp2     >= MAXCUSTOMEXCP2     ||
        iCustomDarkExcp2 >= MAXCUSTOMDARKEXCP2 ||
        iCustomExcp3     >= MAXCUSTOMEXCP3     ||
        iCustomDarkExcp3 >= MAXCUSTOMDARKEXCP3 ||
        iCustomExcp4     >= MAXCUSTOMEXCP4     ||
        iCustomDarkExcp4 >= MAXCUSTOMDARKEXCP4 ||
        iCustomExcp5     >= MAXCUSTOMEXCP5     ||
        iCustomDarkExcp5 >= MAXCUSTOMDARKEXCP5) {
      LOG << "Not Enough Custom... [" << iCustomCorner << ", " << iCustomWall << ", " << iCustomDarkWall << "]\r\n";
      exit(0);
   }
}

BOOL OVERLAP = FALSE;

bool TileSet::DrawTileSet (int xScreen, int yScreen, int TileSet, RECT *Clip, RECT *OutBound, RECT *Wrap, bool Chk, LPDIRECTDRAWSURFACE lpSurface, bool Overlap, BOOL ParamDither,UINT uiID /*=0*/,UINT uiAnimCount/*=0*/) 
{
// This function draw the image for the land type.
 //Time critical function.
	//Random Rnd(1, 12);
	try {
	if (Overlap) 
      OVERLAP = TRUE;
   int DROP = 4;
   int RandomTo10 = (rand() % 10);
   
   if (Chk)
		if (TileSet > SPECIAL_S && TileSet <= SPECIAL_E) 
      {
			OVERLAP = FALSE;
			return false;
		}

	switch (TileSet) {
//		case NOTHING:    Bug.Draw(xScreen, yScreen, 2, 0, lpSurface, Clip, Wrap); break;
      case SPEC_TEST: Object[140].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case MOUNTAIN_NEW_12:
			Mountain[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_13:
			Mountain[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_14:
			Mountain[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_21:
			Mountain[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_23:
			Mountain[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_24:
			Mountain[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_31:
			Mountain[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_32:
			Mountain[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_34:
			Mountain[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_41:
			Mountain[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_42:
			Mountain[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case MOUNTAIN_NEW_43:
			Mountain[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
			Mountain[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

      case MOUNTAIN_2: 
        Mountain[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

      case MOUNTAIN_2_I: 
         Mountain[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
      break;

      case MOUNTAIN_NE_1:
			Mountain[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;
      case MOUNTAIN_NE_2:
			Mountain[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;
		case MOUNTAIN_NE_3:
			Mountain[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;
		case MOUNTAIN_NE_4:
			Mountain[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;
		
      case MOUNTAIN_NW_1:     
			Mountain[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		case MOUNTAIN_NW_2:     
			Mountain[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		case MOUNTAIN_NW_3:     
			Mountain[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		case MOUNTAIN_NW_4:     
			Mountain[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

      case MOUNTAIN_LEFT: Mountain[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case MOUNTAIN_RIGHT: Mountain[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
   
      case CAVERN_ENTRANCE_NE:
			Mountain[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;
      
      case CAVERN_ENTRANCE_NW:
			Mountain[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
      
      case GRASS:
      case GRASS_BREAK1:
      case GRASS_BREAK2:
			Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

	  case FLOOR_LAVA:
		  FloorLava[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 4)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 4)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 

	  case FLOOR_ICE_B:
		  FloorLava[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 10)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 9)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 

     /*
	  case GRASS_BREAK1:
        {
         // X + 1 tasse vers la gauche
         // Y + 1 tasse vers le haut.
         // Le +1 dans la formule des X viens du fait que le map converter a un bug.
			GrassBreak1[(((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 15) + (15-9)-2) % 15][(((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 13) + (13-8)-1) % 13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
        }
      break;

      case GRASS_BREAK2: 
      {
         // X + 1 tasse vers la gauche
         // Y + 1 tasse vers le haut.
         // Le +1 dans la formule des X viens du fait que le map converter a un bug.
         GrassBreak2[(((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 15) + (15-7)-5) % 15][(((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 13) + (13-5)+1) % 13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      }
      break;
      */

      case DUNGEONROCK_FLOOR:
			DungeonRockFloor[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 6)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 7)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

      case DUNGEON_FLOOR:
			DungeonFloor[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 5)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 5)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case DUNGEON_LIGHT:
			DungeonLight[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()+2) % 5)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()-2) % 5)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

      case DUNGEON_LIGHT2:
			FlipDungeonLight[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()-2) % 5)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()-2) % 5)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

	  case RIVER:		  Water1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 0, 0, ParamDither); break;
	  case WATER:		  Water2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER2_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 0, 0, ParamDither); break;

		case WOODEN_FLOOR:     
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2:
					Floor[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				default:
					Floor[(rand() % 5)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
			}
		break;
      case WOODEN_FLOOR2: 
         /*
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2:
					Floor2[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				default:
					Floor2[(rand() % 5)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
			}
         */
         Floor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_2WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_2WOODEN_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		break;
      case WOODEN_FLOOR3:     
         /*
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2:
					Floor3[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				default:
					Floor3[(rand() % 5)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
			}
         */
         Floor3[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_3WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_3WOODEN_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		break;
		case CAVERN:     Cavern[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 14)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 14)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case DUNGEONOVERHEAD:     DungeonOver1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()-2) % 10)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()-2) % 10)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case DUNGEONOVERHEAD2:    DungeonOver2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()-7) % 15)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()-7) % 15)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case DESERT:    
         Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
		//case WATER:      Water[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 0, 0, ParamDither); break;
		case LAVA:       Lava[(rand() % 10)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ICE:        Ice[(rand() % 10)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SUPER_BLACK:  
         Bug[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
         break;
		case DGRASS:	  DGrass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_DGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_DGRASS_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case FOREST: 
         Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case CHEMIN: case EARTH:
         Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);

      break;
      case ROUTE_DALLER:
         OldRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_OLDROCK_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_OLDROCK_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case ROUTE_ROCHE:     
         Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case MOUNTAIN_ROAD:     
			MountainRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_MOUNT_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_MOUNT_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

      case SPEC_NEWTRUNK:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: 
               Object[126].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
            break;
            case 5: case 6: case 7: case 8: 
               Object[127].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
            break;
            case 9: case 10: case 11: case 12: 
               Object[128].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
            break;
         }
      break;

      case SPEC_DISPLAY_1:   Object[152].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_DISPLAY_2:   Object[153].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_DISPLAY_3:   Object[154].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_DISPLAY_2_I: Object[153].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_DISPLAY_3_I: Object[154].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
         
      case SPEC_WOODPILE: Object[139].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_KNIGHT: Object[151].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      
      case SPEC_WEB_1: Object[129].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_WEB_2: Object[130].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_WEB_3: Object[131].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_WEB_4: Object[132].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_WEB_5: Object[133].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_WEB_6: Object[134].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_WEB_7: Object[135].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_WEB_8: Object[136].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_WEB_9: Object[137].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_WEB_10: Object[138].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_WEB_1_I: Object[129].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_WEB_2_I: Object[130].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_WEB_3_I: Object[131].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_WEB_4_I: Object[132].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_WEB_5_I: Object[133].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_WEB_6_I: Object[134].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_WEB_7_I: Object[135].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_WEB_8_I: Object[136].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_WEB_9_I: Object[137].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_WEB_10_I: Object[138].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      
      case SPEC_CAVERN_I_ENTRANCE: 
         CavernWall[((Player.xPos+a-Square+7-g_Global.GetNbrTileXDiv2())%9)+1].DrawSprite(xScreen, yScreen-122+DROP, Clip, OutBound, Wrap, lpSurface); 
         Object[161].DrawSprite(xScreen-32, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case SPEC_CAVERN_I_ENTRANCE_I: 
         CavernWall[((Player.xPos+a-Square+3-g_Global.GetNbrTileXDiv2())%9)+10].DrawSprite(xScreen, yScreen-122+DROP, Clip, OutBound, Wrap, lpSurface); 
         Object[161].DrawSprite(xScreen+32, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
      break;
      case STAIRS_1: Object[122].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case STAIRS_1_I: Object[122].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case STAIRS_2: Object[124].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case STAIRS_2_I: Object[124].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case STAIRSUP_1: Object[123].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case STAIRSUP_1_I: Object[123].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case STAIRSUP_2: Object[125].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case STAIRSUP_2_I: Object[125].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case NEWROCK_1: Object[118].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case NEWROCK_2: Object[119].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case NEWROCK_3: Object[120].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case NEWROCK_4: Object[121].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      
		case GRAROC2_N: Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case GRAROC2_S: Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case GRAROC2_E: Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case GRAROC2_W: Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case GRAROC2_NE:  GraRoc2[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case GRAROC2_SE:  GraRoc2[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case GRAROC2_SW:  GraRoc2[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case GRAROC2_NW:  GraRoc2[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
	
		case ROCFLO_1:   SRockFloor[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO_2:	  SRockFloor[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO_3:   SRockFloor[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO_4:	  SRockFloor[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case ROCFLO2_1:   SRockFloor2[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO2_2:   SRockFloor2[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO2_3:   SRockFloor2[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO2_4:   SRockFloor2[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case ROCFLO3_1:   SRockFloor3[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO3_2:   SRockFloor3[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO3_3:   SRockFloor3[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO3_4:   SRockFloor3[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case ROCFLO4_1:   SRockFloor4[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO4_2:   SRockFloor4[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO4_3:   SRockFloor4[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO4_4:   SRockFloor4[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case ROCFLO5_1:   SRockFloor5[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO5_2:   SRockFloor5[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO5_3:   SRockFloor5[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO5_4:   SRockFloor5[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case ROCFLO6_1:   SRockFloor6[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO6_2:   SRockFloor6[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO6_3:   SRockFloor6[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case ROCFLO6_4:   SRockFloor6[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case FLOOR_SNOW:
			Snow[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_EARTHI:
			EarthI[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTHI_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTHI_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_CAVERNG:
			CavernG[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTHI_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTHI_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_ICE:
			IceFloor[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ICE_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ICE_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_TMPL1:
			TuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE1_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_TMPL2:
			TuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE2_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_ROCK2:
			RockFloor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK2_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_TMPL1S:
			STuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE1_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_TMPL2S:
			STuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE2_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_ANIM_WATER01:
   	   AnimWater01[g_Global.GetAnimWaterFrame()][((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_AWATER01_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_AWATER01_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_ANIM_WATER01D:
         AnimWater01[uiAnimCount][((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_AWATER01_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_AWATER01_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_NEWLAVA:
			//AnimLava01[g_Global.GetAnimLavaFrame()][((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ALAVA01_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ALAVA01_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
         NewLava[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NLAVA_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NLAVA_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case FLOOR_CENDRE:
			Cendre[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_CENDRE_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_CENDRE_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      

      
		
      AllSmooth(GRATER, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)], Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)]);
	  AllSmooth(GRAFOR, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)], Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      AllSmooth(FORTER, Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)], Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)]);
      AllSmooth2(WATDES, Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)], Water1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X)]);
      AllSmooth2(WATFOR, Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)], Water1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X)]);
      AllSmooth2(WATGRA, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)], Water1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X)]);
      AllSmooth2(FLOGRA, Floor[(RandomTo10/2)], Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)]);
      AllSmooth2(FLOFOR, Floor[(RandomTo10/2)], Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      AllSmooth2(FLOTER, Floor[(RandomTo10/2)], Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)]);
      AllSmooth2(FLODES, Floor[(RandomTo10/2)], Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)]);
      AllSmooth2(GRADES, Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)], Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)]);
      AllSmooth2(TERDES, Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)], Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)]);
      
	  AllSmooth2(FORDES, Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)], Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      
	  AllSmooth2(WATCAV, Cavern[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 14)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 14)], Water1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X)]);
      AllSmooth(MTNDES, MountainRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_MOUNT_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_MOUNT_X)], Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)]);
      AllSmooth(MTNFOR, MountainRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_MOUNT_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_MOUNT_X)], Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      AllSmooth(MTNTER, MountainRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_MOUNT_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_MOUNT_X)], Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)]);
      AllSmooth(MTNGRA, MountainRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_MOUNT_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_MOUNT_X)], Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)]);
      AllSmooth2(WATTER, Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)], Water1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X)]);
      AllSmooth2(WATMTN, MountainRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_MOUNT_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_MOUNT_X)], Water1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X)]);
      AllSmooth(GRADUN, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)], DungeonRockFloor[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 6)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 7)]);
      AllSmooth(GRACAV, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)], Cavern[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 14)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 14)]);
      AllSmooth3(GRAROC, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)], OldRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_OLDROCK_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_OLDROCK_X)]);


      AllSmooth3(GRARC2, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)], Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X)]);

	  
      AllSmooth3(ROCROC, Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X)], OldRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_OLDROCK_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_OLDROCK_X)]);
      AllSmooth3(MTNROC, MountainRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_MOUNT_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_MOUNT_X)], OldRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_OLDROCK_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_OLDROCK_X)]);

      AllSmooth(SWPWAT, DGrass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_DGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_DGRASS_X)], Water1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X)]);
      AllSmooth(SWPMNT, DGrass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_DGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_DGRASS_X)], MountainRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_MOUNT_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_MOUNT_X)]);
      AllSmooth(SWPTER, DGrass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_DGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_DGRASS_X)], Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)]);
      AllSmooth(SWPGRA, DGrass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_DGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_DGRASS_X)], Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)]);
      AllSmooth(SWPFOR, DGrass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_DGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_DGRASS_X)], Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      AllSmooth(SWPDES, DGrass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_DGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_DGRASS_X)], Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)]);

      AllSmooth2(GRASSSNOW  , Snow   [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y)   ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X)   ] , Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      AllSmooth2(EARTHSNOW  , Snow   [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y)   ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X)   ] , Earth [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X) ]);
      AllSmooth2(SNOWEARTHI , EarthI [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTHI_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTHI_X) ] , Snow  [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y)  ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X)  ]);
      AllSmooth2(GRASSEARTHI, EarthI [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTHI_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTHI_X) ] , Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      AllSmooth(EARTHEARTHI , EarthI [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTHI_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTHI_X) ] , Earth [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X) ]);
      AllSmooth(SNOWCAVG    , CavernG[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_CAVERNG_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_CAVERNG_X)] , Snow  [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y)  ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X)  ]);
      AllSmooth2(EARTHICAVG , CavernG[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_CAVERNG_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_CAVERNG_X)] , EarthI[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTHI_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTHI_X)]);

      AllSmooth2(ICEWATER   , IceFloor[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ICE_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ICE_X)], Water1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X)]);
      //AllSmooth2(ICESNOW    , IceFloor[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ICE_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ICE_X)], Snow [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X) ]);
      //AllSmooth(ICEWATER   , Water[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER_X)], IceFloor[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ICE_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ICE_X)]);
      AllSmooth(ICESNOW    , Snow [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X) ], IceFloor[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ICE_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ICE_X)]);

      AllSmooth3(GRAFL1, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)], TuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE1_X)]);
      AllSmooth3(ROCFL1, Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X  )], TuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE1_X)]);
      AllSmooth3(SNOFL1, Snow [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X  )], TuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE1_X)]);
      AllSmooth3(GRAFL2, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)], TuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE2_X)]);
      AllSmooth3(ROCFL2, Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X  )], TuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE2_X)]);
      AllSmooth3(SNOFL2, Snow [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X  )], TuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE2_X)]);

      AllSmooth3(WO2FL2 , Floor2    [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_2WOODEN_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_2WOODEN_X  )], TuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE2_X)]);
      AllSmooth3(TUIFL2 , OldRock   [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_OLDROCK_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_OLDROCK_X  )], TuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE2_X)]);
      AllSmooth3(TUITU2 , TuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE1_Y   )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE1_X   )], TuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE2_X)]);

      AllSmooth2(FLO2GRA, Floor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_2WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_2WOODEN_X)], Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)]);
      AllSmooth2(FLO2FOR, Floor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_2WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_2WOODEN_X)], Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      AllSmooth2(FLO2TER, Floor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_2WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_2WOODEN_X)], Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)]);
      AllSmooth2(FLO2DES, Floor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_2WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_2WOODEN_X)], Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)]);
      AllSmooth2(FLO3GRA, Floor3[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_3WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_3WOODEN_X)], Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)]);
      AllSmooth2(FLO3FOR, Floor3[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_3WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_3WOODEN_X)], Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      AllSmooth2(FLO3TER, Floor3[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_3WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_3WOODEN_X)], Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)]);
      AllSmooth2(FLO3DES, Floor3[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_3WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_3WOODEN_X)], Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)]);

      AllSmooth2(SNOROC , Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X  )]   , Snow [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X) ]);

      
      AllSmooth3(SNOWO1, Snow [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X  )], Floor[(RandomTo10/2)]);
      AllSmooth3(SNOWO2, Snow [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X  )], Floor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_2WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_2WOODEN_X)]);
      AllSmooth3(SNOWO3, Snow [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X  )], Floor3[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_3WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_3WOODEN_X)]);
      AllSmooth3(RC2TM1, RockFloor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK2_Y )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK2_X )], TuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE1_X)]);


      AllSmooth3(RC2TS1, RockFloor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK2_Y )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK2_X )], STuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE1_X)]);
      AllSmooth3(RC2TS2, RockFloor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK2_Y )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK2_X )], STuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE2_X)]);

      AllSmooth3(GRATS1, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X )], STuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE1_X)]);
      AllSmooth3(GRATS2, Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X )], STuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE2_X)]);

      AllSmooth3(ROCTS1, Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X )], STuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE1_X)]);
      AllSmooth3(ROCTS2, Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X )], STuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE2_X)]);

      AllSmooth3(SNOTS1, Snow[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X )], STuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE1_X)]);
      AllSmooth3(SNOTS2, Snow[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SNOW_Y )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SNOW_X )], STuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE2_X)]);

      AllSmooth3(TS1TS2, STuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE1_Y )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE1_X )], STuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE2_X)]);

      AllSmooth3(WO1TS1, Floor[(RandomTo10/2)], STuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE1_X)]);
      AllSmooth3(WO1TS2, Floor[(RandomTo10/2)], STuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE2_X)]);

      AllSmooth3(WO2TS1, Floor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_2WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_2WOODEN_X)], STuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE1_X)]);
      AllSmooth3(WO2TS2, Floor2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_2WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_2WOODEN_X)], STuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE2_X)]);

      AllSmooth3(WO3TS1, Floor3[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_3WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_3WOODEN_X)], STuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE1_X)]);
      AllSmooth3(WO3TS2, Floor3[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_3WOODEN_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_3WOODEN_X)], STuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE2_X)]);

      AllSmooth3(TM1TS1, TuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE1_X)], STuileTmps1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE1_X)]);
      AllSmooth3(TM2TS2, TuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_TUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_TUILE2_X)], STuileTmps2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_STUILE2_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_STUILE2_X)]);

      AllSmooth2(WATROC, Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X  )],Water1 [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X) ]);
	   AllSmooth2(WA1WA2, Water2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER2_Y  )][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER2_X )],Water1 [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X) ]);

      AllSmooth2(AWAT01TER, Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)], AnimWater01[uiAnimCount][((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_AWATER01_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_AWATER01_X)]);
      AllSmooth2(AWAT01ROC, MountainRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_MOUNT_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_MOUNT_X)       ], AnimWater01[uiAnimCount][((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_AWATER01_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_AWATER01_X)]);
      AllSmooth2(AWAT01RC2, Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X) ], AnimWater01[uiAnimCount][((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_AWATER01_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_AWATER01_X)]);
      AllSmooth2(AWAT01DES, Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y  ) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)   ], AnimWater01[uiAnimCount][((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_AWATER01_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_AWATER01_X)]);
      AllSmooth2(AWAT01GRA, Grass [((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_Y) ], AnimWater01[uiAnimCount][((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_AWATER01_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_AWATER01_X)]);
      AllSmooth2(AWAT01FOR, Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X) ], AnimWater01[uiAnimCount][((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_AWATER01_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_AWATER01_X)]);

      AllSmooth2(LAV01CEN, Cendre[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_CENDRE_Y) ][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_CENDRE_X) ], NewLava[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NLAVA_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NLAVA_X)]);




      AllSmooth2(GRAROCK, Rock2[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_ROCK_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_ROCK_X)],Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)]);
      AllSmooth2(CENROC , MountainRock[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_MOUNT_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_MOUNT_X)],Cendre[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_CENDRE_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_CENDRE_X)]);
      AllSmooth (CENCAV , Cendre[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_CENDRE_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_CENDRE_X)], Cavern[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 14)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 14)]);
      AllSmooth2(CENWAT , Water1[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_WATER1_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_WATER1_X)],Cendre[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_CENDRE_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_CENDRE_X)]);
      
      


      //AllSmooth2(FLO2GRA, Floor2[(RandomTo10/2)], Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)]);
      //AllSmooth2(FLO2FOR, Floor2[(RandomTo10/2)], Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      //AllSmooth2(FLO2TER, Floor2[(RandomTo10/2)], Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)]);
      //AllSmooth2(FLO2DES, Floor2[(RandomTo10/2)], Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)]);
      //AllSmooth2(FLO3GRA, Floor3[(RandomTo10/2)], Grass[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_NGRASS_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_NGRASS_X)]);
      //AllSmooth2(FLO3FOR, Floor3[(RandomTo10/2)], Forest[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_FOREST_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_FOREST_X)]);
      //AllSmooth2(FLO3TER, Floor3[(RandomTo10/2)], Earth[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_EARTH_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_EARTH_X)]);
      //AllSmooth2(FLO3DES, Floor3[(RandomTo10/2)], Desert[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_SAND_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_SAND_X)]);

      case RIVWAT_1: RivWat[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case RIVWAT_2: RivWat[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case RIVWAT_3: RivWat[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case RIVWAT_4: RivWat[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case RIVWAT_5: RivWat[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case RIVWAT_6: RivWat[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

		case SIMPLE_BED_1_SE: Object[27].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case SIMPLE_BED_1_NE: Object[27].DrawSprite(xScreen+2, yScreen-1, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case DOUBLE_BED_1_SE: Object[26].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
			break;
		case DOUBLE_BED_1_NE: Object[26].DrawSprite(xScreen-30, yScreen-13, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case DOUBLE_BED_2_SE: Object[28].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case DOUBLE_BED_2_NE: Object[28].DrawSprite(xScreen-20, yScreen-16, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case WOODEN_ARMOIR_SE: Object[0].DrawSprite(xScreen+30, yScreen-15, Clip, OutBound, Wrap, lpSurface, 1); 
		break;
		case WOODEN_ARMOIR_SW: Object[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case WOODEN_ARMOIR_NE: Object[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case WOODEN_ARMOIR_NW: Object[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;
		
		case ETAGERE_SE: Object[1].DrawSprite(xScreen+30, yScreen-15, Clip, OutBound, Wrap, lpSurface, 1); 
		break;
		case ETAGERE_SW: Object[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case ETAGERE_NE: Object[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case ETAGERE_NW: Object[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case SPEC_BIBLIOTHEQUE:   Object[55].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case SPEC_BIBLIOTHEQUE_I: Object[55].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case SPEC_LITTLE_BIBLIOTHEQUE:   Object[54].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case SPEC_LITTLE_BIBLIOTHEQUE_I: Object[54].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case BUREAU_SE: Object[2].DrawSprite(xScreen+30, yScreen-15, Clip, OutBound, Wrap, lpSurface, 1); 
		break;
		case BUREAU_SW: Object[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case BUREAU_NE: Object[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case BUREAU_NW: Object[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case REDWOODEN_ARMOIR_SE: Object[3].DrawSprite(xScreen+30, yScreen-15, Clip, OutBound, Wrap, lpSurface, 1); 
		break;
		case REDWOODEN_ARMOIR_SW: Object[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case REDWOODEN_ARMOIR_NE: Object[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case REDWOODEN_ARMOIR_NW: Object[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case COLONNE_CE_B: Object[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case COLONNE_CE_BH: Object[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case COLONNE_CA_BH: Object[6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case COLONNE_GREQUE: Object[7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

      case SPEC_TABLEAU:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: 
					Object[87].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 5: case 6: case 7: case 8: 
					Object[88].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 9: case 10: case 11: case 12: 
					Object[89].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
			}
		break;												

      case SPEC_TABLEAU_I:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: 
					Object[87].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
            case 5: case 6: case 7: case 8: 
					Object[88].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
            case 9: case 10: case 11: case 12: 
					Object[89].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
			}
		break;												
      
      case SPEC_HEADPILE:
			Object[111].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;                                                         

      case SPEC_HEADPILE_I:
		   Object[111].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;                                                         

      case SPEC_BROKEN_CRYPT:
		   Object[166].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;                                                         

      case SPEC_BROKEN_CRYPT_I:
		   Object[166].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;                                                         

      case SPEC_BROKEN_FOUNTAIN:
         Object[167].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_BROKEN_FOUNTAIN_I:
         Object[167].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_CATAPULT_BOULDER1:
		   Object[168].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_CATAPULT_BOULDER2:
		   Object[169].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_GREEN_COLUMN:
		   Object[170].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_BLUE_COLUMN:
		   Object[171].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_BROWN_COLUMN:
		   Object[172].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_BALISTA:
		   Object[173].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_BALISTA_I:
		   Object[173].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_BALISTA2:
		   Object[174].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_BALISTA2_I:
		   Object[174].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_CATAPULT:
		   Object[175].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_CATAPULT_I:
		   Object[175].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_TREBUCHET:
		   Object[176].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_TREBUCHET_I:
		   Object[176].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_TREBUCHET2:
		   Object[177].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
						  
      case SPEC_TREBUCHET2_I:
		   Object[177].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;
				  
      case SPEC_SKELETON_CORPSE: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: 
					Object[115].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 5: case 6: case 7: 
					Object[116].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 4: case 9: case 10:
					Object[115].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
            case 8: case 11: case 12:
					Object[116].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
			}
		break;												

      case SPEC_MANACLE1:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: case 9: case 10:
					Object[113].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 5: case 6: case 7: case 8: case 11: case 12:
					Object[114].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
			}
		break;												

      case SPEC_LITTLE_TREE:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: case 9: case 10:
					Object[155].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
            case 5: case 6: case 7: case 8: case 11: case 12:
					Object[155].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
			}
		break;												

      case SPEC_MANACLE1_I:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: case 9: case 10:
					Object[113].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
            case 5: case 6: case 7: case 8: case 11: case 12:
					Object[114].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
			}
		break;												

      case SPEC_DUNGEON_TOMB:
         Object[117].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_DUNGEON_TOMB_I:
         Object[117].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_BLOOD:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3:
					Object[101].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 4: case 5: case 6: 
					Object[101].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
            case 7: case 8: case 9: 
					Object[102].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
            case 10: case 11: case 12: 
					Object[102].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
            break;
			}
		break;												

      case SPEC_SWORD:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: case 5: case 6:
					Object[76].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 7: case 8: case 9: case 10: case 11: case 12:
					Object[77].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
			}
		break;												

      case SPEC_SWORD_I:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: case 5: case 6:
					Object[76].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
            case 7: case 8: case 9: case 10: case 11: case 12:
					Object[77].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
			}
		break;												

      case SPEC_DUNGEON_WELL: 
         Object[108].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case SPEC_BIG_DUNGEON_WELL: 
         Object[109].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case SPEC_SKEL_HEAD: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: 
					Object[110].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 4: case 5: case 6:
               Object[110].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
            break;
            case 7: case 8: case 9: 
					Object[112].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
            break;
				case 10: case 11: case 12:
				   Object[112].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
            break;
			}
		break; 

      case SPEC_DEBRIS_1:
			Object[104].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
      case SPEC_DEBRIS_1_I:
         Object[104].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_DEBRIS_2:
			Object[105].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
      case SPEC_DEBRIS_2_I:
         Object[105].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_DEBRIS_3:
			Object[106].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
      case SPEC_DEBRIS_3_I:
         Object[106].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_DEBRIS_4:
			Object[107].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
      case SPEC_DEBRIS_4_I:
         Object[107].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_ANVIL: 
   		Object[75].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;												
    
      case SPEC_ANVIL_I: 
			Object[75].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;												
    
      case SPEC_DEBRIS_WALL:   Object[103].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_DEBRIS_WALL_I: Object[103].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_FIREPLACE:   Object[99].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_FIREPLACE_I: Object[99].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_OVEN:   Object[100].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_OVEN_I: Object[100].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_RUINED_BED:   Object[74].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_RUINED_BED_I: Object[74].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_DESK_1:   Object[79].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_DESK_1_I: Object[79].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_DESK_2: 
	     Object[80].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
      break;
      case SPEC_DESK_2_I: 
	      Object[80].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

      case SPEC_CEREBUS:   Object[81].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_CEREBUS_I: Object[81].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_MONK:   Object[82].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_MONK_I: Object[82].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_LUTRIN:   Object[83].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_LUTRIN_I: Object[83].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_THRONE:   Object[84].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_THRONE_I: Object[84].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_CRYPT:   Object[86].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_CRYPT_I: Object[86].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_BONFIRE:   Object[98].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_BONFIRE_I: Object[98].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_BANQUET:   Object[97].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_BANQUET_I: Object[97].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_TAPIS:   Object[90].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_TAPIS_I: Object[90].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_MONOLITH_2: Object[156].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_MONOLITH:   Object[164].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_MONOLITH_I: Object[91].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_TAPIS_ROUGE_1:   Object[94].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_TAPIS_ROUGE_1_I: Object[94].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_TAPIS_ROUGE_2:   Object[95].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_TAPIS_ROUGE_2_I: Object[95].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_TAPIS_ROUGE_3:   Object[96].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_TAPIS_ROUGE_3_I: Object[96].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_BOULDER:   Object[163].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_BOULDER_I: Object[163].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;

      case SPEC_PRISON_BED:   Object[165].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
      case SPEC_PRISON_BED_I: Object[165].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_RUINED_MIRROR: 
				Object[73].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
    
      case SPEC_RUINED_MIRROR_I: 
				Object[73].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;												

      case SPEC_BIG_VASE:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: case 5: case 6:
					Object[85].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 7: case 8: case 9: case 10: case 11: case 12:
					Object[85].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
			}
		break;												

      case SPEC_RUINED_TABLE: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: case 5: case 6:
					Object[72].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 7: case 8: case 9: case 10: case 11: case 12:
					Object[72].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
			}
		break;												

      case SPEC_WEAPONS_RACK_2: 
         if (!Overlap)
            Object[70].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case SPEC_WEAPONS_RACK_2_I:
         if (!Overlap)
            Object[70].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;
      case SPEC_WEAPONS_RACK_1:
         if (!Overlap)
            Object[71].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case SPEC_WEAPONS_RACK_1_I: 
         if (!Overlap)
            Object[71].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_DUMMY: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3:
				case 4: case 5: case 6:
					Object[68].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
				case 7: case 8: case 9:
				case 10: case 11: case 12:
					Object[69].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
			}
		break;												

      case SPEC_DUMMY_I: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3:
				case 4: case 5: case 6:
					Object[68].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
				case 7: case 8: case 9:
				case 10: case 11: case 12:
					Object[69].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
			}
		break;												

      case SPEC_ARMOR_ON_A_RACK: 
					Object[67].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;												

      case SPEC_ARMOR_ON_A_RACK_I: 
					Object[67].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;												

      case SPEC_ARCHER: 
				Object[66].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;												

      case SPEC_ARCHER_I: 
				Object[66].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;												

      case SPEC_BLACK_SMITH_FORGERY: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: case 5: case 6:
					Object[65].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
            case 7: case 8: case 9: case 10: case 11: case 12:
					Object[65].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
			}
		break;
		
		case SPEC_WOOD_PRISON:
			{
				Object[178].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
			}
		break;

		case SPEC_PENTACLE:
			{
				Object[179].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
			}
		break;

		case SPEC_DARK_PENTACLE:
			{
				Object[180].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
			}
		break;

		case SPEC_OLD_PENTACLE:
			{
				Object[181].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
			}
		break;

		case SPEC_TOMB_OF_DARKNESS:
			{
				Object[182].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
			}
		break;
		case SPEC_BANC_TEMPLE:  
			{ 
                Object[183].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  

			} 

         break; 
		 case SPEC_CRYPT2:  
			{ 
                Object[184].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  

			} 

         break; 
		   case SPEC_FORGE1:  
                Object[185].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
         case SPEC_FORGE2:  
                Object[185].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
         break; 
         case SPEC_HORSE_WATER:  
                Object[186].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
         case SPEC_HORSE_WATER_I:  
                Object[186].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
         break; 
         case SPEC_HUTE_GOB1:  
                Object[187].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 0);  
         break; 
   	   case SPEC_HUTE_GOB2:  
                Object[187].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
         break; 
         case SPEC_BAR:  
                Object[188].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
         case SPEC_BAR_I:  
                Object[188].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
         break; 
         case SPEC_ORGUE:  
            Object[189].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
         case SPEC_ORGUE_I:  
            Object[189].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  
         break; 
		   case SPEC_TABLE_FORGE1:  
                Object[190].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
         case SPEC_TABLE_FORGE1_I:  
                Object[190].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  
         break; 
		   case SPEC_TABLE_TEMPLE1:  
                Object[191].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break;
         case SPEC_TABLE_TEMPLE1_I:  
                Object[191].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  
         break; 
		   case SPEC_TORTUR1:  
                Object[192].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
         case SPEC_TORTUR1_I:  
                Object[192].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
         break; 
		   case SPEC_TORTUR2:  
                Object[193].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
    	   case SPEC_TORTUR3:  
                Object[194].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
		   case SPEC_TORTUR4:  
                Object[195].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
		   case SPEC_TORTUR5:  
                Object[196].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
		   case SPEC_TORTUR6:  
                Object[197].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break; 
         case SPEC_TORTUR6_I:  
                Object[197].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
         break; 
		   case SPEC_TRONE_KING1:  
                Object[198].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface );  
         break; 
		   case SPEC_TRONE_KING2:  
                Object[199].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
         break;
		 case SPEC_BLOCKFLECHES:  
			{ 
                Object[200].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  

			} 
         break;
		 case SPEC_ENTREEMINE:  
			{ 
                Object[201].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  

			} 
         break;
		 case SPEC_PONTPIERRE:  
			{ 
                Object[202].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  

			} 
         break;
		 case SPEC_PONTPIERRETOP:  
			{ 
                Object[203].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  

			} 
         break;
		 case SPEC_TENTE1:  
			{ 
                Object[204].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  

			} 
         break;
		 case SPEC_TENTE2:  
			{ 
                Object[205].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  

			} 
         break;
		 case SPEC_TENTE3:  
			{ 
                Object[206].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  

			} 
         break;
      case SPEC_LANTERNE1:  
			{ 
                Object[207].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  

			} 
         break;
      case SPEC_LH_BRIDGE:    Object[208].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;
      case SPEC_LH_BRIDGE_B:  Object[209].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;

      case SPEC_CASTLE_BRIDGE:     Object[237].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;
      case SPEC_CASTLE_BRIDGE_B1:  Object[238].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;
      case SPEC_CASTLE_BRIDGE_B2:  Object[241].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;
      case SPEC_CASTLE_BRIDGE_I:   Object[237].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  break;
      case SPEC_CASTLE_BRIDGE_B1_I:Object[238].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  break;
      case SPEC_CASTLE_BRIDGE_B2_I:Object[241].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  break;


      case SPEC_ESC_ESCALIER  :  EscalierObject[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;
      case SPEC_ESC_ESCALIER_I:  EscalierObject[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  break;
      case SPEC_ESC_ESCALIER_C:  EscalierObject[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;
      case SPEC_ESC_TUILE1    :  EscalierObject[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;
      case SPEC_ESC_TUILE2    :  EscalierObject[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;
      case SPEC_ESC_TUILE3    :  EscalierObject[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;
      case SPEC_ESC_TUILE4    :  EscalierObject[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;

      case SPEC_SMALL_BRIDGE  :Object[240].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  break;
      case SPEC_SMALL_BRIDGE_I:Object[240].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  break;

      case SKELETON_DRAKE01       :  DrakeSkeleton[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SKELETON_DRAKE02       :  DrakeSkeleton[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SKELETON_DRAKE03       :  DrakeSkeleton[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SKELETON_DRAKE04       :  DrakeSkeleton[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_FOUNTAIN_G:  
                Object[221].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
      break;
      case SPEC_FOUNTAIN_GI:  
                Object[221].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  
      break;
      case SPEC_HUTE_FGOB_G:  
                Object[222].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
      break;
      case SPEC_HUTE_FGOB_GI:  
                Object[222].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  
      break;
      case SPEC_HUTE_GOB_G:  
                Object[223].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
      break;
      case SPEC_HUTE_GOB_GI:  
                Object[223].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  
      break;
      case SPEC_MONT_ENTRE_G:  
                Object[224].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
      break;
      case SPEC_MONT_ENTRE_GI:  
                Object[224].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  
      break;
      case SPEC_MONT_G:  
                Object[225].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
      break;
      case SPEC_MONT_GI:  
                Object[225].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  
      break;
      case SPEC_MONT2_G:  
                Object[226].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
      break;
      case SPEC_MONT2_GI:  
                Object[226].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);  
      break;

      


      case TOMB_SW: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3:
					Object[8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
				case 4: case 5: case 6:
					Object[9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
				case 7: case 8: case 9:
					Object[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
				case 10: case 11: case 12:
					Object[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
				break;
			}
		break;												
		
		case TOMB_SE: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3:
					Object[8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
				case 4: case 5: case 6:
					Object[9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
				case 7: case 8: case 9:
					Object[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
				case 10: case 11: case 12:
					Object[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
				break;
			}
		break;

		case GOLDEN_MIRROR_NE: 
         if (!Overlap)
            Object[12].DrawSprite(xScreen-17, yScreen, Clip, OutBound, Wrap, lpSurface, 0); 
		break;
		case GOLDEN_MIRROR_NW: 
         if (!Overlap)
            Object[12].DrawSprite(xScreen-17, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;
 
		case HORLOGE_GP_SE: Object[13].DrawSprite(xScreen+30, yScreen-15, Clip, OutBound, Wrap, lpSurface, 1); 
		break;								
		case HORLOGE_GP_SW: Object[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case HORLOGE_GP_NE: Object[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		case HORLOGE_GP_NW: Object[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case ROCHE_1: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3: case 4: case 5: case 6: Object[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
				case 7: case 8: case 9: case 10: case 11: case 12: Object[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
			}
		break;

		case ROCHE_3: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3: case 4: case 5: case 6: Object[32].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
				case 7: case 8: case 9: case 10: case 11: case 12: Object[32].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
			}
		break;

		case ROCHE_2: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3: case 4: case 5: case 6: Object[33].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
				case 7: case 8: case 9: case 10: case 11: case 12: Object[33].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
			}
		break;

		case SHOP_SIGN_1:
			Object[37].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case PLAQUE_1:
			Object[38].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case PLAQUE_2:
			Object[39].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case PLAQUE_3:
			Object[40].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case PLAQUE_4:
			Object[41].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case PLAQUE_5: 
			Object[42].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SHOP_SIGN_1_I:
			Object[37].DrawSprite(xScreen+34, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case SHOP_SIGN_2:
			Object[57].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SHOP_SIGN_2_I:
			Object[57].DrawSprite(xScreen+34, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case PLAQUE_1_I:
			Object[38].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case PLAQUE_2_I: 
			Object[39].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case PLAQUE_3_I:
			Object[40].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case PLAQUE_4_I:
			Object[41].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case PLAQUE_5_I: 
			Object[42].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case WOOD_SIGN_1: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3: case 4: case 5: case 6: Object[43].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
				case 7: case 8: case 9: case 10: case 11: case 12: Object[43].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
			}
		break;		 

		case WOOD_SIGN_2: 
			Object[44].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case WOOD_SIGN_2_I: 
			Object[44].DrawSprite(xScreen-1, yScreen-13, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case SPEC_SHELF:
			Object[92].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_SHELF_I:
			Object[92].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case SPEC_TRUNK: 
			Object[30].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_CHAUDRON: 
			Object[50].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_BIG_CHANDELLE: 
			Object[51].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
      case SPEC_BIG_CHANDELLE2: 
			Object[242].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
      case SPEC_BIG_CHANDELLE3: 
			Object[243].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

      case SPEC_BRIDGE1:
         Object[157].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 0);
      break;
      
      case SPEC_BRIDGE1_I:
         Object[157].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;
      
      case SPEC_BRIDGE2:
         Object[158].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 0);
      break;
      
      case SPEC_BRIDGE2_I:
         Object[158].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_BRIDGE3:
         Object[162].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 0);
      break;
      
      case SPEC_BRIDGE3_I:
         Object[162].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
      break;
      
      case SPEC_BORDER_BRIDGE1:
         Object[159].DrawSprite(xScreen-200, yScreen-120, Clip, OutBound, Wrap, lpSurface, 0);
      break;
      
      case SPEC_BORDER_BRIDGE1B:
         Object[159].DrawSprite(xScreen-93, yScreen-158, Clip, OutBound, Wrap, lpSurface, 0);
      break;

      case SPEC_BORDER_BRIDGE1_I:
         Object[159].DrawSprite(xScreen+95, yScreen-120, Clip, OutBound, Wrap, lpSurface, 1);
      break;
      
      case SPEC_BORDER_BRIDGE1_IB:
         Object[159].DrawSprite(xScreen+55, yScreen-158, Clip, OutBound, Wrap, lpSurface, 1);
      break;

      case SPEC_BORDER_BRIDGE2:
         Object[160].DrawSprite(xScreen-252, yScreen-115, Clip, OutBound, Wrap, lpSurface, 0);
      break;
      
      case SPEC_BORDER_BRIDGE2B:
         Object[160].DrawSprite(xScreen-87, yScreen-182, Clip, OutBound, Wrap, lpSurface, 0);
      break;
      
      case SPEC_BORDER_BRIDGE2_I:
         Object[160].DrawSprite(xScreen+110, yScreen-115, Clip, OutBound, Wrap, lpSurface, 1);
      break;
      
      case SPEC_BORDER_BRIDGE2_IB:
         Object[160].DrawSprite(xScreen+10, yScreen-182, Clip, OutBound, Wrap, lpSurface, 1);
      break;
      
      case SPEC_HARP: 
			Object[45].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case SPEC_HARP_I:
			Object[45].DrawSprite(xScreen-1, yScreen-13, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_MARBLE_BANC: 
			Object[48].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case SPEC_MARBLE_BANC_I:
			Object[48].DrawSprite(xScreen-1, yScreen-13, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_WOODEN_BANC: 
			Object[49].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case SPEC_WOODEN_BANC_I:
			Object[49].DrawSprite(xScreen-1, yScreen-13, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_CHANDELLE: 
			Object[52].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_CHANDELLE_I:
			Object[52].DrawSprite(xScreen-1, yScreen-13, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case SPEC_CHANDELLE_2: 
			Object[53].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_CHANDELLE_2_I:
			Object[53].DrawSprite(xScreen-1, yScreen-13, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case SPEC_SPINNING_WHEEL:
			Object[46].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_SPINNING_WHEEL_I:
			Object[46].DrawSprite(xScreen-1, yScreen-13, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case SPEC_BUCHE:
			Object[47].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case SPEC_BUCHE_I:
			Object[47].DrawSprite(xScreen-1, yScreen-13, Clip, OutBound, Wrap, lpSurface); 
		break;

		case FONTAINE_EAU:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3: case 4: case 5: case 6: Object[29].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
				case 7: case 8: case 9: case 10: case 11: case 12: Object[29].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
			}
		break;

		case SPEC_TAPIS_R:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3: case 4: case 5: case 6: Object[93].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
				case 7: case 8: case 9: case 10: case 11: case 12: Object[93].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
			}
		break;

		case MOTTE_TERRE: 
			Object[31].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case TORTURE_1:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3: case 4: case 5: case 6: Object[56].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
				case 7: case 8: case 9: case 10: case 11: case 12: Object[56].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
			}
		break;

		case CHAMP: Object[62].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case BIG_FONTAINE_1: 
         if(!g_SaveGame.GetOptionsParam()->bShowAnimDecorsLight)
            Object[61].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
         else
         {
            if(Overlap)
               Objects.AddOverlapAnim(uiID,__OBJANIM_FONTAIN_02,xScreen, yScreen, *Clip);
         }
      break;
      case BIG_MOULIND: 
         if(!g_SaveGame.GetOptionsParam()->bShowAnimDecorsLight)
            Moulin[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
         else
         {
            if(Overlap)
               Objects.AddOverlapAnim(uiID,__OBJANIM_MOULIND,xScreen, yScreen, *Clip);
         }
      break;
      case BIG_MOULING: 
         if(!g_SaveGame.GetOptionsParam()->bShowAnimDecorsLight)
            Moulin[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
         else
         {
            if(Overlap)
               Objects.AddOverlapAnim(uiID,__OBJANIM_MOULING,xScreen, yScreen, *Clip);
         }
      break;

		case BAR_1: Object[63].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
		case BAR_2: Object[63].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		case SHOP_COUNTER_1: Object[64].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
		case SHOP_COUNTER_2: Object[64].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			
		case WHELL_1:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3: case 4: case 5: case 6: Object[59].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
				case 7: case 8: case 9: case 10: case 11: case 12: Object[59].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
			}
		break;
			
		case TOWN_BOARD_1:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3: case 4: case 5: case 6: Object[60].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
				case 7: case 8: case 9: case 10: case 11: case 12: Object[60].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
			}
		break;
			
		case AUTEL_1:
			  Object[58].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
		break;

		case AUTEL_1_I:
			  Object[58].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
		break;

		case OBELISK_1: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3: case 4: case 5: case 6:    Object[23].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
				case 7: case 8: case 9: case 10: case 11: case 12: Object[23].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
			}
		break;

      case SPEC_STALAGTIC_1: Object[141].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_STALAGTIC_2: Object[142].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_STALAGTIC_3: Object[143].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_STALAGTIC_4: Object[144].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_STALAGTIC_5: Object[145].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_STALAGTIC_6: Object[146].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_STALAGTIC_7: Object[147].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_GROSSE_ROCHE_1: Object[148].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_GROSSE_ROCHE_2: Object[149].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_GROSSE_ROCHE_3: Object[150].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_GSTALAGTIC_1: Object[210].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_GSTALAGTIC_2: Object[211].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_GSTALAGTIC_3: Object[212].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_GSTALAGTIC_4: Object[213].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_GSTALAGTIC_5: Object[214].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_GSTALAGTIC_6: Object[215].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_GSTALAGTIC_7: Object[216].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case SPEC_GGROSSE_ROCHE_1: Object[217].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_GGROSSE_ROCHE_2: Object[218].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_GGROSSE_ROCHE_3: Object[219].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
         
		case SPEC_TROLL_BED: Troll[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

      case SPEC_TROLL_BED_I: Troll[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case SPEC_TROLL_ROUND_TABLE: Troll[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_TROLL_TABLE: Troll[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_TROLL_TABLE_I: Troll[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case SPEC_TROLL_CHAIR_1: Troll[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case SPEC_TROLL_CHAIR_1_I: Troll[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_TROLL_CHAIR_2: Troll[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

      case MENIR_ROCHE: Object[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case MENIR_NOIR: Object[17].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case ROUND_WOODEN_TABLE: Object[18].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case LITTLE_WOODEN_TABLE_SE: Object[19].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case LITTLE_WOODEN_TABLE_SW: Object[19].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case LITTLE_ROUND_WOODEN_TABLE: Object[20].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SQUARE_WOODEN_TABLE: Object[21].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case LARGE_WOODEN_TABLE_SE: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
         case 1: case 5: case 9: case 2:
               Object[22].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
				break;
            case 3: case 7: case 11: case 6:
               Object[25].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
				break;
            case 4: case 8: case 12:  case 10:
               Object[78].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
				break;
			}
		break;

		case LARGE_WOODEN_TABLE_SW: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 5: case 9: case 2:
               Object[22].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
   			break;
            case 3: case 7: case 11: case 6:
               Object[25].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
            case 4: case 8: case 12: case 10:
               Object[78].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;
		
		case SPEC_TREE_SET_1: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 5: case 9:
					Tree[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 2: case 6: case 10: 			
					Tree[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 3: case 7: case 11: 
					Tree[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 4: case 8: case 12: 			
					Tree[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;
		
		case SPEC_TREE_SET_2: 
//			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
//            case 5: case 6: case 7: case 8: case 1: case 2:
					Tree[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
//				break;
//         }
      break;

		case SPEC_TREE_SET_3: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: case 9: case 10:
					Tree[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
            case 5: case 6: case 7: case 8: case 11: case 12:
					Tree[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;

		case SPEC_TREE_SET_4: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3: case 4:
					Tree[7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 5: case 6: case 7: case 8:
					Tree[8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 9: case 10: case 11: case 12:
					Tree[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;

		case SPEC_TREE_SET_5: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4: case 5: case 6:
				   Tree[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
            break;
            case 7: case 8: case 9: case 10: case 11: case 12:
				   Tree[9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
            break;
         }
		break;

		case SPEC_TREE_SET_6: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3: case 4: case 5: case 6:
					Tree[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 7: case 8: case 9: case 10: case 11: case 12:
					Tree[6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;

		case SPEC_TREE_SET_7: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3: case 4: case 5: case 6:
					Tree[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 7: case 8: case 9: case 10: case 11: case 12:
					Tree[28].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;

		case SPEC_TREE_SET_8: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3: case 4: case 5: case 6:
					Tree[19].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 7: case 8: case 9: case 10: case 11: case 12:
					Tree[20].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;

		case SPEC_TREE_SET_9: 
				Tree[23].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_TREE_SET_10: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3: case 4:
					Tree[18].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 5: case 6: case 7: case 8:
					Tree[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 9: case 10: case 11: case 12:
					Tree[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;

		case SPEC_TREE_SET_11: 
				Tree[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

		case SPEC_TREE_SET_12: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
				case 1: case 2: case 3: case 4:
					Tree[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 5: case 6: case 7: case 8:
					Tree[21].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 9: case 10: case 11: case 12:
					Tree[22].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;

		case SPEC_TREE_SET_13: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3:
					Tree[27].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 4: case 5: case 6: 			
					Tree[24].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 7: case 8: case 9: 
					Tree[25].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 10: case 11: case 12: 			
					Tree[26].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;
		
		case SPEC_TREE_SET_14: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: case 3:
					Tree[29].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 4: case 5: case 6: 			
					Tree[30].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 7: case 8: case 9: 
					Tree[31].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 10: case 11: case 12: 			
					Tree[32].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;

      case SPEC_TREE_SET_15:
			Tree[17].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

		case SPEC_TREE_SET_16: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
			   case 1: case 2: 
					Tree[33].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 4: case 5: 
					Tree[34].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 7: case 8: 
					Tree[35].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 10: case 11: 
					Tree[36].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
				case 3: case 6:
					Tree[37].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
            case 9: case 12: 
					Tree[38].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;

      case SPEC_TREE_SET_17:
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 4:
					Tree[40].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
            case 5: case 6: case 7: case 8:
					Tree[41].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
            case 9: case 10: case 11: case 12:
               Tree[42].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;
		
      case SPEC_TREE_SET_18:
  			Tree[39].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case DUNGEON_SPEC_2_BORDER_NW_SE_2:
			DungeonWall[25].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case DUNGEON_SPEC_2_BORDER_NW_SE_1_1:
			DungeonWall[27].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); 
		break;

		case DUNGEON_SPEC_2_BORDER_NW_SE_1_2:
			DungeonWall[26].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); 
		break;

		case DUNGEON_SPEC_2_BORDER_NE_SW_2:
			DungeonWall[22].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); 
		break;

		case DUNGEON_SPEC_2_BORDER_NE_SW_1_1:
			DungeonWall[24].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case DUNGEON_SPEC_2_BORDER_NE_SW_1_2:
			DungeonWall[23].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface, 1); 
		break;

		case DUNGEON_BORDER_2_1:
         DungeonWall[16].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEON_BORDER_2_2:
         DungeonWall[14].DrawSprite(xScreen+3-44, yScreen-122, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEON_BORDER_2_3:
         DungeonWall[15].DrawSprite(xScreen, yScreen-122+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEON_BORDER_2_4:
         DungeonWall[17].DrawSprite(xScreen+3-52, yScreen-122+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEON_BORDER_1:
         DungeonWall[20].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEON_BORDER_2:
         DungeonWall[18].DrawSprite(xScreen+2, yScreen-122, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEON_BORDER_3:
         DungeonWall[19].DrawSprite(xScreen, yScreen-122+15, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEON_BORDER_4:
         DungeonWall[21].DrawSprite(xScreen, yScreen-122+15, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEON_SPEC_1_NW_SE: 
			FlipDungeonWallSpec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEON_SPEC_1_NE_SW: 
			DungeonWallSpec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEON_SPEC_2_NW_SE: 
			FlipDungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEON_SPEC_2_NE_SW: 
			DungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEON_SPEC_3_NW_SE: 
			FlipDungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEON_SPEC_3_NE_SW: 
			DungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEON_B_NE_SW: 	
		case DUNGEON_NE_SW: 
			DungeonWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEON_B_NW_SE:
		case DUNGEON_NW_SE: 
			FlipDungeonWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEON_WSE: 
         DungeonWall[5].DrawSprite(xScreen-64, yScreen-32-122, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEON_WNE:
         DungeonWall[6].DrawSprite(xScreen-64, yScreen-122+8, Clip, OutBound, Wrap, lpSurface);
		break;
      
      case DUNGEON_NES:
         DungeonWall[7].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEON_NWS:
         DungeonWall[9].DrawSprite(xScreen-14, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEON_X:
         DungeonWall[8].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEON_T_NES:
         DungeonWall[10].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEON_T_NEW:
         DungeonWall[11].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEON_T_NSW:
         DungeonWall[13].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEON_T_ESW:
         DungeonWall[12].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEON_TOP:    case DUNGEON_TOP_2:  case DUNGEON_TOP_3:  case DUNGEON_TOP_4: 
      case DUNGEON_TOP_16: case DUNGEON_TOP_15: case DUNGEON_TOP_5:  case DUNGEON_TOP_7: 
      case DUNGEON_TOP_9:  case DUNGEON_TOP_11: case DUNGEON_TOP_13: case DUNGEON_TOP_6: 
      case DUNGEON_TOP_8:  case DUNGEON_TOP_10:  case DUNGEON_TOP_12:  case DUNGEON_TOP_14: 
      break;

		
		case CASTLEWALL_BORDER_1:
         CastleWall[16].DrawSprite(xScreen, yScreen-127+7, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case CASTLEWALL_BORDER_2:
         CastleWall[14].DrawSprite(xScreen, yScreen-127+2, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case CASTLEWALL_BORDER_3:
         CastleWall[15].DrawSprite(xScreen, yScreen-127+16, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case CASTLEWALL_BORDER_4:
         CastleWall[17].DrawSprite(xScreen, yScreen-127+10, Clip, OutBound, Wrap, lpSurface);
      break;
        
      case CASTLEWALL_SPEC_1_BORDER_NW_SE_1_1: 
			DungeonWall[14].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NW_SE_1_2: 
			DungeonWall[15].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NW_SE_1_3: 
			DungeonWall[16].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NW_SE_1_4: 
			DungeonWall[17].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NW_SE_1_5: 
			DungeonWall[18].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NW_SE_2_1: 
			DungeonWall[19].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NW_SE_2_2: 
			DungeonWall[20].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NW_SE_2_3: 
			DungeonWall[21].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NW_SE_2_4: 
			DungeonWall[22].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NW_SE_2_5: 
			DungeonWall[23].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NE_SW_1_1: 
			DungeonWall[24].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NE_SW_1_2: 
			DungeonWall[25].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NE_SW_1_3: 
			DungeonWall[26].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NE_SW_1_4: 
			DungeonWall[27].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NE_SW_1_5: 
			DungeonWall[28].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NE_SW_2_1: 
			DungeonWall[29].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NE_SW_2_2: 
			DungeonWall[30].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NE_SW_2_3: 
			DungeonWall[31].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NE_SW_2_4: 
			DungeonWall[32].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_BORDER_NE_SW_2_5: 
			DungeonWall[33].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_SPEC_1_NW_SE: 
			FlipDungeonWallSpec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case CASTLEWALL_SPEC_1_NE_SW: 
			DungeonWallSpec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); 
		break;
		
      case CASTLEWALL_SPEC_2_NW_SE: 
			FlipDungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case CASTLEWALL_SPEC_2_NE_SW: 
			DungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case CASTLEWALL_SPEC_3_NW_SE: 
			FlipDungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case CASTLEWALL_SPEC_3_NE_SW: 
			DungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case CASTLEWALL_B_NE_SW: 	
		case CASTLEWALL_NE_SW: 
			FlipCastleWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()+1)%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case CASTLEWALL_B_NW_SE:
		case CASTLEWALL_NW_SE: 
			CastleWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()+1)%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case CASTLEWALL_WSE: 
			CastleWall[5].DrawSprite(xScreen-64, yScreen-32-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_WNE:
         CastleWall[6].DrawSprite(xScreen-64, yScreen-127+7, Clip, OutBound, Wrap, lpSurface);
		break;
      
      case CASTLEWALL_NES:
         CastleWall[7].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_NWS:
         CastleWall[9].DrawSprite(xScreen, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_X:
         CastleWall[8].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_T_NES:
         CastleWall[10].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_T_NEW:
         CastleWall[11].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_T_NSW:
         CastleWall[13].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_T_ESW:
         CastleWall[12].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CASTLEWALL_TOP:    case CASTLEWALL_TOP_2:  case CASTLEWALL_TOP_3:  case CASTLEWALL_TOP_4: 
      case CASTLEWALL_TOP_16: case CASTLEWALL_TOP_15: case CASTLEWALL_TOP_5:  case CASTLEWALL_TOP_7: 
      case CASTLEWALL_TOP_9:  case CASTLEWALL_TOP_11: case CASTLEWALL_TOP_13: case CASTLEWALL_TOP_6: 
      case CASTLEWALL_TOP_8:  case CASTLEWALL_TOP_10:  case CASTLEWALL_TOP_12:  case CASTLEWALL_TOP_14: 
      break;

		case DUNGEONROCK_BORDER_2_1:
         DungeonRockWall[36].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_BORDER_2_2:
         DungeonRockWall[34].DrawSprite(xScreen+3-44, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_BORDER_2_3:
         DungeonRockWall[35].DrawSprite(xScreen, yScreen-127+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_BORDER_2_4:
         DungeonRockWall[37].DrawSprite(xScreen+3-52, yScreen-127+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_BORDER_1:
         DungeonRockWall[40].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_BORDER_2:
         DungeonRockWall[38].DrawSprite(xScreen+3, yScreen-127+4, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_BORDER_3:
         DungeonRockWall[39].DrawSprite(xScreen, yScreen-127+15+3, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_BORDER_4:
         DungeonRockWall[41].DrawSprite(xScreen+20, yScreen-127+15, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_SPEC_1_BORDER_NW_SE_1_1: 
			DungeonRockWall[14].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NW_SE_1_2: 
			DungeonRockWall[15].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NW_SE_1_3: 
			DungeonRockWall[16].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NW_SE_1_4: 
			DungeonRockWall[17].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NW_SE_1_5: 
			DungeonRockWall[18].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NW_SE_2_1: 
			DungeonRockWall[19].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NW_SE_2_2: 
			DungeonRockWall[20].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NW_SE_2_3: 
			DungeonRockWall[21].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NW_SE_2_4: 
			DungeonRockWall[22].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NW_SE_2_5: 
			DungeonRockWall[23].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NE_SW_1_1: 
			DungeonRockWall[24].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NE_SW_1_2: 
			DungeonRockWall[25].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NE_SW_1_3: 
			DungeonRockWall[26].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NE_SW_1_4: 
			DungeonRockWall[27].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NE_SW_1_5: 
			DungeonRockWall[28].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NE_SW_2_1: 
			DungeonRockWall[29].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NE_SW_2_2: 
			DungeonRockWall[30].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NE_SW_2_3: 
			DungeonRockWall[31].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NE_SW_2_4: 
			DungeonRockWall[32].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_BORDER_NE_SW_2_5: 
			DungeonRockWall[33].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_SPEC_1_NW_SE: 
			FlipDungeonRockWallSpec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case DUNGEONROCK_SPEC_1_NE_SW: 
			DungeonRockWallSpec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); 
		break;
		
      case DUNGEONROCK_SPEC_2_NW_SE: 
			FlipDungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_SPEC_2_NE_SW: 
			DungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEONROCK_SPEC_3_NW_SE: 
			FlipDungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_SPEC_3_NE_SW: 
			DungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_B_NE_SW: 	
		case DUNGEONROCK_NE_SW: 
			DungeonRockWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEONROCK_B_NW_SE:
		case DUNGEONROCK_NW_SE: 
			FlipDungeonRockWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_WSE: 
         DungeonRockWall[5].DrawSprite(xScreen-64, yScreen-32-124, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_WNE:
         DungeonRockWall[6].DrawSprite(xScreen-64, yScreen-127+8, Clip, OutBound, Wrap, lpSurface);
		break;
      
      case DUNGEONROCK_NES:
         DungeonRockWall[7].DrawSprite(xScreen-64, yScreen-123-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_NWS:
         DungeonRockWall[9].DrawSprite(xScreen-13, yScreen-124-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_X:
         DungeonRockWall[8].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_T_NES:
         DungeonRockWall[10].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_T_NEW:
         DungeonRockWall[11].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_T_NSW:
         DungeonRockWall[13].DrawSprite(xScreen-64, yScreen-127-32+2, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_T_ESW:
         DungeonRockWall[12].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_TOP:    case DUNGEONROCK_TOP_2:  case DUNGEONROCK_TOP_3:  case DUNGEONROCK_TOP_4: 
      case DUNGEONROCK_TOP_16: case DUNGEONROCK_TOP_15: case DUNGEONROCK_TOP_5:  case DUNGEONROCK_TOP_7: 
      case DUNGEONROCK_TOP_9:  case DUNGEONROCK_TOP_11: case DUNGEONROCK_TOP_13: case DUNGEONROCK_TOP_6: 
      case DUNGEONROCK_TOP_8:  case DUNGEONROCK_TOP_10:  case DUNGEONROCK_TOP_12:  case DUNGEONROCK_TOP_14: 
      break;

		case DUNGEONROCK_2_BORDER_2_1:
         DungeonRockWall2[36].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_2_BORDER_2_2:
         DungeonRockWall2[34].DrawSprite(xScreen+3-44, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_2_BORDER_2_3:
         DungeonRockWall2[35].DrawSprite(xScreen, yScreen-127+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_2_BORDER_2_4:
         DungeonRockWall2[37].DrawSprite(xScreen+3-52, yScreen-127+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_2_BORDER_1:
         DungeonRockWall2[40].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_2_BORDER_2:
         DungeonRockWall2[38].DrawSprite(xScreen+3, yScreen-127+4, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_2_BORDER_3:
         DungeonRockWall2[39].DrawSprite(xScreen, yScreen-127+15+3, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_2_BORDER_4:
         DungeonRockWall2[41].DrawSprite(xScreen+20, yScreen-127+15, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_1_1: 
			DungeonRockWall2[14].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_1_2: 
			DungeonRockWall2[15].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_1_3: 
			DungeonRockWall2[16].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_1_4: 
			DungeonRockWall2[17].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_1_5: 
			DungeonRockWall2[18].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_2_1: 
			DungeonRockWall2[19].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_2_2: 
			DungeonRockWall2[20].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_2_3: 
			DungeonRockWall2[21].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_2_4: 
			DungeonRockWall2[22].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NW_SE_2_5: 
			DungeonRockWall2[23].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_1_1: 
			DungeonRockWall2[24].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_1_2: 
			DungeonRockWall2[25].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_1_3: 
			DungeonRockWall2[26].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_1_4: 
			DungeonRockWall2[27].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_1_5: 
			DungeonRockWall2[28].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_2_1: 
			DungeonRockWall2[29].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_2_2: 
			DungeonRockWall2[30].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_2_3: 
			DungeonRockWall2[31].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_2_4: 
			DungeonRockWall2[32].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_BORDER_NE_SW_2_5: 
			DungeonRockWall2[33].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_SPEC_1_NW_SE: 
			FlipDungeonRockWall2Spec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case DUNGEONROCK_2_SPEC_1_NE_SW: 
			DungeonRockWall2Spec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); 
		break;
		
      case DUNGEONROCK_2_SPEC_2_NW_SE: 
			FlipDungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_2_SPEC_2_NE_SW: 
			DungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEONROCK_2_SPEC_3_NW_SE: 
			FlipDungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_2_SPEC_3_NE_SW: 
			DungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_2_B_NE_SW: 	
		case DUNGEONROCK_2_NE_SW: 
			DungeonRockWall2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEONROCK_2_B_NW_SE:
		case DUNGEONROCK_2_NW_SE: 
			FlipDungeonRockWall2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_2_WSE: 
         DungeonRockWall2[5].DrawSprite(xScreen-64, yScreen-32-124, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_WNE:
         DungeonRockWall2[6].DrawSprite(xScreen-64, yScreen-127+8, Clip, OutBound, Wrap, lpSurface);
		break;
      
      case DUNGEONROCK_2_NES:
         DungeonRockWall2[7].DrawSprite(xScreen-64, yScreen-123-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_NWS:
         DungeonRockWall2[9].DrawSprite(xScreen-13, yScreen-124-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_X:
         DungeonRockWall2[8].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_T_NES:
         DungeonRockWall2[10].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_T_NEW:
         DungeonRockWall2[11].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_T_NSW:
         DungeonRockWall2[13].DrawSprite(xScreen-64, yScreen-127-32+2, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_T_ESW:
         DungeonRockWall2[12].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_2_TOP:    case DUNGEONROCK_2_TOP_2:  case DUNGEONROCK_2_TOP_3:  case DUNGEONROCK_2_TOP_4: 
      case DUNGEONROCK_2_TOP_16: case DUNGEONROCK_2_TOP_15: case DUNGEONROCK_2_TOP_5:  case DUNGEONROCK_2_TOP_7: 
      case DUNGEONROCK_2_TOP_9:  case DUNGEONROCK_2_TOP_11: case DUNGEONROCK_2_TOP_13: case DUNGEONROCK_2_TOP_6: 
      case DUNGEONROCK_2_TOP_8:  case DUNGEONROCK_2_TOP_10:  case DUNGEONROCK_2_TOP_12:  case DUNGEONROCK_2_TOP_14: 
      break;

		case DUNGEONROCK_3_BORDER_2_1:
         DungeonRockWall3[36].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_3_BORDER_2_2:
         DungeonRockWall3[34].DrawSprite(xScreen+3-44, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_3_BORDER_2_3:
         DungeonRockWall3[35].DrawSprite(xScreen, yScreen-127+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_3_BORDER_2_4:
         DungeonRockWall2[37].DrawSprite(xScreen+3-52, yScreen-127+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_3_BORDER_1:
         DungeonRockWall3[40].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_3_BORDER_2:
         DungeonRockWall3[38].DrawSprite(xScreen+3, yScreen-127+4, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_3_BORDER_3:
         DungeonRockWall3[39].DrawSprite(xScreen, yScreen-127+15+3, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_3_BORDER_4:
         DungeonRockWall3[41].DrawSprite(xScreen+20, yScreen-127+15, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_1_1: 
			DungeonRockWall3[14].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_1_2: 
			DungeonRockWall3[15].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_1_3: 
			DungeonRockWall3[16].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_1_4: 
			DungeonRockWall3[17].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_1_5: 
			DungeonRockWall3[18].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_2_1: 
			DungeonRockWall3[19].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_2_2: 
			DungeonRockWall3[20].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_2_3: 
			DungeonRockWall3[21].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_2_4: 
			DungeonRockWall3[22].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NW_SE_2_5: 
			DungeonRockWall3[23].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_1_1: 
			DungeonRockWall3[24].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_1_2: 
			DungeonRockWall3[25].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_1_3: 
			DungeonRockWall3[26].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_1_4: 
			DungeonRockWall3[27].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_1_5: 
			DungeonRockWall3[28].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_2_1: 
			DungeonRockWall3[29].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_2_2: 
			DungeonRockWall3[30].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_2_3: 
			DungeonRockWall3[31].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_2_4: 
			DungeonRockWall3[32].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_BORDER_NE_SW_2_5: 
			DungeonRockWall3[33].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_SPEC_1_NW_SE: 
			FlipDungeonRockWall3Spec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case DUNGEONROCK_3_SPEC_1_NE_SW: 
			DungeonRockWall3Spec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); 
		break;
		
      case DUNGEONROCK_3_SPEC_2_NW_SE: 
			FlipDungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_3_SPEC_2_NE_SW: 
			DungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEONROCK_3_SPEC_3_NW_SE: 
			FlipDungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_3_SPEC_3_NE_SW: 
			DungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_3_B_NE_SW: 	
		case DUNGEONROCK_3_NE_SW: 
			DungeonRockWall3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEONROCK_3_B_NW_SE:
		case DUNGEONROCK_3_NW_SE: 
			FlipDungeonRockWall3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_3_WSE: 
         DungeonRockWall3[5].DrawSprite(xScreen-64, yScreen-32-124, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_WNE:
         DungeonRockWall3[6].DrawSprite(xScreen-64, yScreen-127+8, Clip, OutBound, Wrap, lpSurface);
		break;
      
      case DUNGEONROCK_3_NES:
         DungeonRockWall3[7].DrawSprite(xScreen-64, yScreen-123-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_NWS:
         DungeonRockWall3[9].DrawSprite(xScreen-13, yScreen-124-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_X:
         DungeonRockWall3[8].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_T_NES:
         DungeonRockWall3[10].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_T_NEW:
         DungeonRockWall3[11].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_T_NSW:
         DungeonRockWall3[13].DrawSprite(xScreen-64, yScreen-127-32+2, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_T_ESW:
         DungeonRockWall3[12].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_3_TOP:    case DUNGEONROCK_3_TOP_2:  case DUNGEONROCK_3_TOP_3:  case DUNGEONROCK_3_TOP_4: 
      case DUNGEONROCK_3_TOP_16: case DUNGEONROCK_3_TOP_15: case DUNGEONROCK_3_TOP_5:  case DUNGEONROCK_3_TOP_7: 
      case DUNGEONROCK_3_TOP_9:  case DUNGEONROCK_3_TOP_11: case DUNGEONROCK_3_TOP_13: case DUNGEONROCK_3_TOP_6: 
      case DUNGEONROCK_3_TOP_8:  case DUNGEONROCK_3_TOP_10:  case DUNGEONROCK_3_TOP_12:  case DUNGEONROCK_3_TOP_14: 
      break;

		case DUNGEONROCK_4_BORDER_2_1:
         DungeonRockWall4[36].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_4_BORDER_2_2:
         DungeonRockWall4[34].DrawSprite(xScreen+3-44, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_4_BORDER_2_3:
         DungeonRockWall4[35].DrawSprite(xScreen, yScreen-127+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_4_BORDER_2_4:
         DungeonRockWall4[37].DrawSprite(xScreen+3-52, yScreen-127+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_4_BORDER_1:
         DungeonRockWall4[40].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_4_BORDER_2:
         DungeonRockWall4[38].DrawSprite(xScreen+3, yScreen-127+4, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_4_BORDER_3:
         DungeonRockWall4[39].DrawSprite(xScreen, yScreen-127+15+3, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_4_BORDER_4:
         DungeonRockWall4[41].DrawSprite(xScreen+20, yScreen-127+15, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_1_1: 
			DungeonRockWall4[14].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_1_2: 
			DungeonRockWall4[15].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_1_3: 
			DungeonRockWall4[16].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_1_4: 
			DungeonRockWall4[17].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_1_5: 
			DungeonRockWall4[18].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_2_1: 
			DungeonRockWall4[19].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_2_2: 
			DungeonRockWall4[20].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_2_3: 
			DungeonRockWall4[21].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_2_4: 
			DungeonRockWall4[22].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NW_SE_2_5: 
			DungeonRockWall4[23].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_1_1: 
			DungeonRockWall4[24].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_1_2: 
			DungeonRockWall4[25].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_1_3: 
			DungeonRockWall4[26].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_1_4: 
			DungeonRockWall4[27].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_1_5: 
			DungeonRockWall4[28].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_2_1: 
			DungeonRockWall4[29].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_2_2: 
			DungeonRockWall4[30].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_2_3: 
			DungeonRockWall4[31].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_2_4: 
			DungeonRockWall4[32].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_BORDER_NE_SW_2_5: 
			DungeonRockWall4[33].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_SPEC_1_NW_SE: 
			FlipDungeonRockWall4Spec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case DUNGEONROCK_4_SPEC_1_NE_SW: 
			DungeonRockWall4Spec[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); 
		break;
		
      case DUNGEONROCK_4_SPEC_2_NW_SE: 
			FlipDungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_4_SPEC_2_NE_SW: 
			DungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEONROCK_4_SPEC_3_NW_SE: 
			FlipDungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_4_SPEC_3_NE_SW: 
			DungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_4_B_NE_SW: 	
		case DUNGEONROCK_4_NE_SW: 
			DungeonRockWall4[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
      case DUNGEONROCK_4_B_NW_SE:
		case DUNGEONROCK_4_NW_SE: 
			FlipDungeonRockWall4[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-127, Clip, OutBound, Wrap, lpSurface); break;
		break;
		
		case DUNGEONROCK_4_WSE: 
         DungeonRockWall4[5].DrawSprite(xScreen-64, yScreen-32-124, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_WNE:
         DungeonRockWall4[6].DrawSprite(xScreen-64, yScreen-127+8, Clip, OutBound, Wrap, lpSurface);
		break;
      
      case DUNGEONROCK_4_NES:
         DungeonRockWall4[7].DrawSprite(xScreen-64, yScreen-123-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_NWS:
         DungeonRockWall4[9].DrawSprite(xScreen-13, yScreen-124-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_X:
         DungeonRockWall4[8].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_T_NES:
         DungeonRockWall4[10].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_T_NEW:
         DungeonRockWall4[11].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_T_NSW:
         DungeonRockWall4[13].DrawSprite(xScreen-64, yScreen-127-32+2, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_T_ESW:
         DungeonRockWall4[12].DrawSprite(xScreen-64, yScreen-127-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case DUNGEONROCK_4_TOP:    case DUNGEONROCK_4_TOP_2:  case DUNGEONROCK_4_TOP_3:  case DUNGEONROCK_4_TOP_4: 
      case DUNGEONROCK_4_TOP_16: case DUNGEONROCK_4_TOP_15: case DUNGEONROCK_4_TOP_5:  case DUNGEONROCK_4_TOP_7: 
      case DUNGEONROCK_4_TOP_9:  case DUNGEONROCK_4_TOP_11: case DUNGEONROCK_4_TOP_13: case DUNGEONROCK_4_TOP_6: 
      case DUNGEONROCK_4_TOP_8:  case DUNGEONROCK_4_TOP_10:  case DUNGEONROCK_4_TOP_12:  case DUNGEONROCK_4_TOP_14: 
      break;

      case CAVERN_BORDER_1:
         DungeonWall[16].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case CAVERN_BORDER_2:
         DungeonWall[14].DrawSprite(xScreen+3-44, yScreen-122, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case CAVERN_BORDER_3:
         DungeonWall[15].DrawSprite(xScreen, yScreen-122+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case CAVERN_BORDER_4:
         DungeonWall[17].DrawSprite(xScreen+3-52, yScreen-122+12, Clip, OutBound, Wrap, lpSurface);
      break;
         
      case CAVERN_SPEC_1_NW_SE: 
        CavernWall[((Player.xPos+a-Square+2-g_Global.GetNbrTileXDiv2())%9)+19].DrawSprite(xScreen, yScreen-122+80, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case CAVERN_SPEC_1_NE_SW: 
         CavernWall[((Player.xPos+a-Square+2-g_Global.GetNbrTileXDiv2())%9)+28].DrawSprite(xScreen, yScreen-122+80, Clip, OutBound, Wrap, lpSurface); 
		break;
		
      case CAVERN_SPEC_2_NW_SE: 
			FlipDungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case CAVERN_SPEC_2_NE_SW: 
			DungeonWallSpec2[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface);
		break;
		
      case CAVERN_SPEC_3_NW_SE: 
			FlipDungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case CAVERN_SPEC_3_NE_SW: 
			DungeonWallSpec3[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%5)].DrawSprite(xScreen, yScreen-122, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case CAVERN_B_NE_SW: 	
		case CAVERN_NE_SW: 
         CavernWall[((Player.xPos+a-Square+3-g_Global.GetNbrTileXDiv2())%9)+10].DrawSprite(xScreen, yScreen-122+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;
		
      case CAVERN_B_NW_SE:
		case CAVERN_NW_SE: 
         CavernWall[((Player.xPos+a-Square+7-g_Global.GetNbrTileXDiv2())%9)+1].DrawSprite(xScreen, yScreen-122+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case CAVERN_WSE: 
         CavernWall[37].DrawSprite(xScreen-128, yScreen-122+20, Clip, OutBound, Wrap, lpSurface); 
		break;

      case CAVERN_WNE:
         CavernWall[0].DrawSprite(xScreen-128, yScreen-122+32+DROP, Clip, OutBound, Wrap, lpSurface);
		break;
      
      case CAVERN_NES:
			CavernWall[39].DrawSprite(xScreen-128, yScreen-122-37+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;

      case CAVERN_NWS:
         CavernWall[38].DrawSprite(xScreen-248+161, yScreen-122-50+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;

		case CAVERN_WSE_2: 
         CavernWall[41].DrawSprite(xScreen-128, yScreen+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;

      case CAVERN_WNE_2:
         CavernWall[40].DrawSprite(xScreen-128, yScreen+DROP, Clip, OutBound, Wrap, lpSurface);
		break;
      
      case CAVERN_NES_2:
			CavernWall[43].DrawSprite(xScreen-128, yScreen+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;

      case CAVERN_NWS_2:
         CavernWall[42].DrawSprite(xScreen-(CavernWall[42].GetWidth()-128), yScreen+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;

      case CAVERN_X:
         DungeonWall[8].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CAVERN_T_NES:
         DungeonWall[10].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CAVERN_T_NEW:
         DungeonWall[11].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CAVERN_T_NSW:
         DungeonWall[13].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CAVERN_T_ESW:
         DungeonWall[12].DrawSprite(xScreen-64, yScreen-122-32, Clip, OutBound, Wrap, lpSurface);
		break;

      case CAVERN_SKIP_1: case CAVERN_SKIP_2: case CAVERN_SKIP_3: case CAVERN_SKIP_4:
         Cavern[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % 14)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 14)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case CAVERN_SKIP_5: case CAVERN_SKIP_6: case CAVERN_SKIP_7: case CAVERN_SKIP_8:
		break;
      case CAVERN_SKIP_5_2: case CAVERN_SKIP_6_2: case CAVERN_SKIP_7_2: case CAVERN_SKIP_8_2:
		break;

		case RAMPART_B_NW_SE:
		case RAMPART_NW_SE: 
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 4) {
            case 0: Rampart[6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
            case 1: Rampart[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
            case 2: Rampart[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
            case 3: Rampart[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
		
		case RAMPART_B_NE_SW: 	
		case RAMPART_NE_SW: 
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 4) {
            case 0: Rampart[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
            case 1: Rampart[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
            case 2: Rampart[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
            case 3: Rampart[6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1); break;
         }
		break;
		
		case RAMPART_TOWER_1: Rampart[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case RAMPART_TOWER_2: Rampart[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

      case GRAVEYARD_B_NW_SE:
		case GRAVEYARD_NW_SE: Grave[0].DrawSprite(xScreen, yScreen-35, Clip, OutBound, Wrap, lpSurface, 1); 
		break;
		
		case GRAVEYARD_B_NE_SW: 	
		case GRAVEYARD_NE_SW: Grave[0].DrawSprite(xScreen, yScreen-35, Clip, OutBound, Wrap, lpSurface);  
		break;
		
		case GRAVEYARD_WNE: Grave[3].DrawSprite(xScreen, yScreen-27, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case GRAVEYARD_NWS: Grave[4].DrawSprite(xScreen+8, yScreen-36, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case GRAVEYARD_NES: Grave[4].DrawSprite(xScreen, yScreen-36, Clip, OutBound, Wrap, lpSurface);
		break;

		case GRAVEYARD_WSE: Grave[5].DrawSprite(xScreen, yScreen-35, Clip, OutBound, Wrap, lpSurface);
		break;

		case GRAVEYARD_T_NES: Grave[6].DrawSprite(xScreen, yScreen-35, Clip, OutBound, Wrap, lpSurface);
		break;

		case GRAVEYARD_T_ESW: Grave[7].DrawSprite(xScreen, yScreen-35, Clip, OutBound, Wrap, lpSurface);
		break;

		case GRAVEYARD_T_NSW: Grave[7].DrawSprite(xScreen, yScreen-35, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case GRAVEYARD_T_NEW: Grave[6].DrawSprite(xScreen, yScreen-35, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case GRAVEYARD_X: Grave[8].DrawSprite(xScreen, yScreen-35, Clip, OutBound, Wrap, lpSurface);
		break;

		case GRAVEYARD_C_WSE:
		case GRAVEYARD_NE:
		case GRAVEYARD_NW:
		case GRAVEYARD_EW: Grave[1].DrawSprite(xScreen, yScreen-20, Clip, OutBound, Wrap, lpSurface);  
		break;
		
		case GRAVEYARD_ENS: Grave[2].DrawSprite(xScreen, yScreen-36, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case GRAVEYARD_WNS: Grave[2].DrawSprite(xScreen, yScreen-36, Clip, OutBound, Wrap, lpSurface);  
		break;

		case GRAVEYARD_C_NWS:
		case GRAVEYARD_SE: Grave[1].DrawSprite(xScreen, yScreen-20, Clip, OutBound, Wrap, lpSurface);  
							   Grave[2].DrawSprite(xScreen, yScreen-36, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case GRAVEYARD_C_NES:
		case GRAVEYARD_SW: Grave[1].DrawSprite(xScreen, yScreen-20, Clip, OutBound, Wrap, lpSurface);  
							   Grave[2].DrawSprite(xScreen, yScreen-36, Clip, OutBound, Wrap, lpSurface);  
		break;

		case GRAVEYARD_C_WNE:
		case GRAVEYARD_PLUS:	Grave[1].DrawSprite(xScreen, yScreen-20, Clip, OutBound, Wrap, lpSurface);  				  
							   Grave[2].DrawSprite(xScreen, yScreen-36, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case GRAVEYARD_1_NW_SE: 	Grave[9].DrawSprite(xScreen-8+12, yScreen+4+6, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case GRAVEYARD_1_NE_SW:    Grave[9].DrawSprite(xScreen-8, yScreen+4, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case GRAVEYARD_BS_NW_SE: 	Grave[0].DrawSprite(xScreen, yScreen-35, Clip, OutBound, Wrap, lpSurface, 1);
								Grave[2].DrawSprite(xScreen, yScreen-36, Clip, OutBound, Wrap, lpSurface);  
		break;

		case GRAVEYARD_BS_NE_SW:	Grave[0].DrawSprite(xScreen, yScreen-35, Clip, OutBound, Wrap, lpSurface);
								Grave[2].DrawSprite(xScreen, yScreen-36, Clip, OutBound, Wrap, lpSurface, 1);  
		break;
    
      case WHITEWALL_BORDER_WG_I:
      	WhiteWall[30].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case WHITEWALL_BORDER_WD:
			WhiteWall[29].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case WHITEWALL_BORDER_WG:
			WhiteWall[28].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case WHITEWALL_BORDER_WD_I:
			WhiteWall[27].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		
      case WHITEWALL_X_NW_SE:
		   WhiteWall[21].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

      case WHITEWALL_X_NE_SW:
		   WhiteWall[20].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

      case WHITEWALL_BORDER_NW_SE:
		   WhiteWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
	
		case WHITEWALL_BORDER_NE_SW:
		   WhiteWall[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;			
		
		case WHITEWALL_TOP_NW_SE:
		   WhiteWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case WHITEWALL_TOP_NE_SW:
		   WhiteWall[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case WHITEWALL_B_NW_SE:
		case WHITEWALL_NW_SE: 
         WhiteWall[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
		
		case WHITEWALL_B_NE_SW: 	
		case WHITEWALL_NE_SW: 
         WhiteWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
      break;
		
		case WHITEWALL_WNE: WhiteWall[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case WHITEWALL_NWS: WhiteWall[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case WHITEWALL_NES: WhiteWall[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case WHITEWALL_WSE: WhiteWall[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case WHITEWALL_T_NES: WhiteWall[6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case WHITEWALL_T_ESW: WhiteWall[7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case WHITEWALL_T_NSW: WhiteWall[19].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case WHITEWALL_T_NEW: WhiteWall[18].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case WHITEWALL_X: WhiteWall[8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case WHITEWALL_C_WSE:
		case WHITEWALL_NE:
		case WHITEWALL_NW:
		case WHITEWALL_EW: WhiteWall[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;
		
		case WHITEWALL_ENS: WhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case WHITEWALL_WNS: WhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;

		case WHITEWALL_C_NWS:
		case WHITEWALL_SE: WhiteWall[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
							   WhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case WHITEWALL_C_NES:
		case WHITEWALL_SW: WhiteWall[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
							   WhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;

		case WHITEWALL_C_WNE:
		case WHITEWALL_PLUS:	WhiteWall[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  				  
							   WhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

      case WHITEWALL_W_NW_SE: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 10:
               WhiteWall[22].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
            case 4: case 5: case 6: case 11:		
					WhiteWall[24].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
            case 7: case 8: case 9: case 12:
					WhiteWall[26].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;
		
		case WHITEWALL_W_NE_SW:  
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 10:
               WhiteWall[9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
            case 4: case 5: case 6: case 11:		
					WhiteWall[23].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
            case 7: case 8: case 9: case 12:
					WhiteWall[25].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
				break;
			}
		break;

		case WHITEWALL_P_NW_SE: 	WhiteWall[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case WHITEWALL_P_NE_SW:    WhiteWall[17].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;

		case WHITEWALL_H1_NW_SE: 	WhiteWall[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case WHITEWALL_H1_NE_SW: 
         WhiteWall[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
      break;

		case WHITEWALL_H2_NW_SE: 	WhiteWall[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;                     
		
		case WHITEWALL_H2_NE_SW:   WhiteWall[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;

		case WHITEWALL_BS_NW_SE: 	WhiteWall[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
								WhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;

		case WHITEWALL_BS_NE_SW:	WhiteWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
								WhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

      case RUINED_WHITEWALL_BORDER_WG_I:
      	RuinedWhiteWall[30].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_WHITEWALL_BORDER_WD:
			RuinedWhiteWall[29].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case RUINED_WHITEWALL_BORDER_WG:
			RuinedWhiteWall[28].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_WHITEWALL_BORDER_WD_I:
			RuinedWhiteWall[27].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		
      case RUINED_WHITEWALL_X_NW_SE:
		   RuinedWhiteWall[21].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

      case RUINED_WHITEWALL_X_NE_SW:
		   RuinedWhiteWall[20].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;

      case RUINED_WHITEWALL_BORDER_NW_SE:
		   RuinedWhiteWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
	
		case RUINED_WHITEWALL_BORDER_NE_SW:
		   RuinedWhiteWall[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;			
		
		case RUINED_WHITEWALL_TOP_NW_SE:
		   RuinedWhiteWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case RUINED_WHITEWALL_TOP_NE_SW:
		   RuinedWhiteWall[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case RUINED_WHITEWALL_B_NW_SE:
		case RUINED_WHITEWALL_NW_SE: RuinedWhiteWall[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
		
		case RUINED_WHITEWALL_B_NE_SW: 	
		case RUINED_WHITEWALL_NE_SW: RuinedWhiteWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
      break;
		
		case RUINED_WHITEWALL_WNE: RuinedWhiteWall[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case RUINED_WHITEWALL_NWS: RuinedWhiteWall[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_WHITEWALL_NES: RuinedWhiteWall[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_WHITEWALL_WSE: RuinedWhiteWall[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_WHITEWALL_T_NES: RuinedWhiteWall[6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_WHITEWALL_T_ESW: RuinedWhiteWall[7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_WHITEWALL_T_NSW: RuinedWhiteWall[19].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_WHITEWALL_T_NEW: RuinedWhiteWall[18].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_WHITEWALL_X: RuinedWhiteWall[8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_WHITEWALL_C_WSE:
		case RUINED_WHITEWALL_NE:
		case RUINED_WHITEWALL_NW:
		case RUINED_WHITEWALL_EW: RuinedWhiteWall[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;
		
		case RUINED_WHITEWALL_ENS: RuinedWhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case RUINED_WHITEWALL_WNS: RuinedWhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;

		case RUINED_WHITEWALL_C_NWS:
		case RUINED_WHITEWALL_SE: RuinedWhiteWall[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
							   RuinedWhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case RUINED_WHITEWALL_C_NES:
		case RUINED_WHITEWALL_SW: RuinedWhiteWall[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
							   RuinedWhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;

		case RUINED_WHITEWALL_C_WNE:
		case RUINED_WHITEWALL_PLUS:	RuinedWhiteWall[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  				  
							   RuinedWhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

      case RUINED_WHITEWALL_W_NW_SE: 
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 10:
               RuinedWhiteWall[26].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); //22
				break;
            case 4: case 5: case 6: case 11:		
					RuinedWhiteWall[22].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); //24
				break;
            case 7: case 8: case 9: case 12:
					RuinedWhiteWall[24].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); //26
				break;
			}
		break;
		
		case RUINED_WHITEWALL_W_NE_SW:  
			switch (Rnd(((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()))+(Player.yPos+b-Square-g_Global.GetNbrTileYDiv2())*3072)) {
            case 1: case 2: case 3: case 10:
               RuinedWhiteWall[25].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); //9
				break;
            case 4: case 5: case 6: case 11:		
					RuinedWhiteWall[9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); //23
				break;
            case 7: case 8: case 9: case 12:
					RuinedWhiteWall[23].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); //25
				break;
			}
		break;

		case RUINED_WHITEWALL_P_NW_SE: 	RuinedWhiteWall[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case RUINED_WHITEWALL_P_NE_SW:    RuinedWhiteWall[17].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;

		case RUINED_WHITEWALL_H1_NW_SE:
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
			case 0: RuinedWhiteWall[36].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			case 1: RuinedWhiteWall[34].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			case 2: RuinedWhiteWall[35].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			}
		break;
		
		case RUINED_WHITEWALL_H1_NE_SW: 
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
			case 0: RuinedWhiteWall[33].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			case 1: RuinedWhiteWall[31].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			case 2: RuinedWhiteWall[32].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			}
	    break;

		case RUINED_WHITEWALL_H2_NW_SE:
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) 
         {
			case 0: RuinedWhiteWall[42].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			case 1: RuinedWhiteWall[40].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			case 2: RuinedWhiteWall[41].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			}
		break;                     
		
		case RUINED_WHITEWALL_H2_NE_SW:  
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
			case 0: RuinedWhiteWall[39].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			case 1: RuinedWhiteWall[37].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			case 2: RuinedWhiteWall[38].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
			}
		break;

			case RUINED_WHITEWALL_BS_NW_SE: 	RuinedWhiteWall[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
								RuinedWhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);  
		break;

		case RUINED_WHITEWALL_BS_NE_SW:	RuinedWhiteWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
								RuinedWhiteWall[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case BRICKWALL_BORDER_WG_I:
      	BrickWall[30].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case BRICKWALL_BORDER_WD:
			BrickWall[29].DrawSprite(xScreen, yScreen-95+6, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case BRICKWALL_BORDER_WG:
			BrickWall[28].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case BRICKWALL_BORDER_WD_I:
			BrickWall[27].DrawSprite(xScreen, yScreen-95+6, Clip, OutBound, Wrap, lpSurface);
		break;
		
      case BRICKWALL_BORDER_NW_SE:
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: BrickWall[13].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: BrickWall[17].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: BrickWall[20].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
	
		case BRICKWALL_BORDER_NE_SW:
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: BrickWall[0].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: BrickWall[10].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: BrickWall[21].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;			
		
		case BRICKWALL_TOP_NW_SE:
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: BrickWall[13].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: BrickWall[17].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: BrickWall[20].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
		
		case BRICKWALL_TOP_NE_SW:
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: BrickWall[0].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: BrickWall[10].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: BrickWall[21].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
		
		case BRICKWALL_B_NW_SE:
		case BRICKWALL_NW_SE: 
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: BrickWall[0].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: BrickWall[10].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: BrickWall[21].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
		
		case BRICKWALL_B_NE_SW: 	
		case BRICKWALL_NE_SW: 
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: BrickWall[13].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: BrickWall[17].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: BrickWall[20].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
		
		case BRICKWALL_WNE: BrickWall[3].DrawSprite(xScreen, yScreen-87, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case BRICKWALL_NWS: BrickWall[16].DrawSprite(xScreen+4, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case BRICKWALL_NES: BrickWall[4].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case BRICKWALL_WSE: BrickWall[5].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case BRICKWALL_T_NES: BrickWall[6].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case BRICKWALL_T_ESW: BrickWall[7].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case BRICKWALL_T_NSW: BrickWall[19].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case BRICKWALL_T_NEW: BrickWall[18].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case BRICKWALL_X: BrickWall[8].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case BRICKWALL_C_WSE:
		case BRICKWALL_NE:
		case BRICKWALL_NW:
		case BRICKWALL_EW: BrickWall[1].DrawSprite(xScreen, yScreen-80, Clip, OutBound, Wrap, lpSurface);  
		break;
		
		case BRICKWALL_ENS: BrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case BRICKWALL_WNS: BrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface);  
		break;

		case BRICKWALL_C_NWS:
		case BRICKWALL_SE: BrickWall[1].DrawSprite(xScreen, yScreen-80, Clip, OutBound, Wrap, lpSurface);  
							   BrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case BRICKWALL_C_NES:
		case BRICKWALL_SW: BrickWall[1].DrawSprite(xScreen, yScreen-80, Clip, OutBound, Wrap, lpSurface);  
							   BrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface);  
		break;

		case BRICKWALL_C_WNE:
		case BRICKWALL_PLUS:	BrickWall[1].DrawSprite(xScreen, yScreen-80, Clip, OutBound, Wrap, lpSurface);  				  
							   BrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

      case BRICKWALL_W_NW_SE: 
         BrickWall[22].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case BRICKWALL_W_NE_SW:  
         BrickWall[9].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); 
		break;

		case BRICKWALL_P_NW_SE: 	BrickWall[26].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case BRICKWALL_P_NE_SW:    BrickWall[25].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  
		break;

		case BRICKWALL_H1_NW_SE: 	BrickWall[24].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case BRICKWALL_H1_NE_SW:    BrickWall[23].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  
		break;

		case BRICKWALL_H2_NW_SE: 	BrickWall[20].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); 
		break;                     
		
		case BRICKWALL_H2_NE_SW:   BrickWall[25].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  
		break;

		case BRICKWALL_BS_NW_SE: 	
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: BrickWall[0].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: BrickWall[10].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: BrickWall[21].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }

								BrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface);  
		break;

		case BRICKWALL_BS_NE_SW:	
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: BrickWall[13].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: BrickWall[17].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: BrickWall[20].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }

								BrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case RUINED_BRICKWALL_BORDER_WG_I:
      	RuinedBrickWall[30].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_BRICKWALL_BORDER_WD:
			RuinedBrickWall[29].DrawSprite(xScreen, yScreen-95+6, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case RUINED_BRICKWALL_BORDER_WG:
			RuinedBrickWall[28].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_BRICKWALL_BORDER_WD_I:
			RuinedBrickWall[27].DrawSprite(xScreen, yScreen-95+6, Clip, OutBound, Wrap, lpSurface);
		break;
		
      case RUINED_BRICKWALL_BORDER_NW_SE:
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: RuinedBrickWall[20].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: RuinedBrickWall[13].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: RuinedBrickWall[17].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
	
		case RUINED_BRICKWALL_BORDER_NE_SW:
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: RuinedBrickWall[21].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: RuinedBrickWall[0 ].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: RuinedBrickWall[10].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;			
		
		case RUINED_BRICKWALL_TOP_NW_SE:
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: RuinedBrickWall[20].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: RuinedBrickWall[13].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: RuinedBrickWall[17].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
		
		case RUINED_BRICKWALL_TOP_NE_SW:
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: RuinedBrickWall[21].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: RuinedBrickWall[0 ].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: RuinedBrickWall[10].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
		
		case RUINED_BRICKWALL_B_NW_SE:
		case RUINED_BRICKWALL_NW_SE: 
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: RuinedBrickWall[21].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: RuinedBrickWall[0 ].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: RuinedBrickWall[10].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
		
		case RUINED_BRICKWALL_B_NE_SW: 	
		case RUINED_BRICKWALL_NE_SW: 
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: RuinedBrickWall[13].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: RuinedBrickWall[17].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: RuinedBrickWall[20].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }
		break;
		
		case RUINED_BRICKWALL_WNE: RuinedBrickWall[3].DrawSprite(xScreen, yScreen-87, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case RUINED_BRICKWALL_NWS: RuinedBrickWall[16].DrawSprite(xScreen+4, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_BRICKWALL_NES: RuinedBrickWall[4].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_BRICKWALL_WSE: RuinedBrickWall[5].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_BRICKWALL_T_NES: RuinedBrickWall[6].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_BRICKWALL_T_ESW: RuinedBrickWall[7].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_BRICKWALL_T_NSW: RuinedBrickWall[19].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_BRICKWALL_T_NEW: RuinedBrickWall[18].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_BRICKWALL_X: RuinedBrickWall[8].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;

		case RUINED_BRICKWALL_C_WSE:
		case RUINED_BRICKWALL_NE:
		case RUINED_BRICKWALL_NW:
		case RUINED_BRICKWALL_EW: RuinedBrickWall[1].DrawSprite(xScreen, yScreen-80, Clip, OutBound, Wrap, lpSurface);  
		break;
		
		case RUINED_BRICKWALL_ENS: RuinedBrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case RUINED_BRICKWALL_WNS: RuinedBrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface);  
		break;

		case RUINED_BRICKWALL_C_NWS:
		case RUINED_BRICKWALL_SE: RuinedBrickWall[1].DrawSprite(xScreen, yScreen-80, Clip, OutBound, Wrap, lpSurface);  
							   RuinedBrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case RUINED_BRICKWALL_C_NES:
		case RUINED_BRICKWALL_SW: RuinedBrickWall[1].DrawSprite(xScreen, yScreen-80, Clip, OutBound, Wrap, lpSurface);  
							   RuinedBrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface);  
		break;

		case RUINED_BRICKWALL_C_WNE:
		case RUINED_BRICKWALL_PLUS:	RuinedBrickWall[1].DrawSprite(xScreen, yScreen-80, Clip, OutBound, Wrap, lpSurface);  				  
							   RuinedBrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

      case RUINED_BRICKWALL_W_NW_SE: 
         RuinedBrickWall[22].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case RUINED_BRICKWALL_W_NE_SW:  
         RuinedBrickWall[9].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); 
		break;

		case RUINED_BRICKWALL_P_NW_SE: 	RuinedBrickWall[26].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case RUINED_BRICKWALL_P_NE_SW:    RuinedBrickWall[25].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  
		break;

		case RUINED_BRICKWALL_H1_NW_SE: 	
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
			case 0: RuinedBrickWall[9].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
			case 1: RuinedBrickWall[1].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  break;
			case 2: RuinedBrickWall[2].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  break;
			}
		break;
		
		case RUINED_BRICKWALL_H1_NE_SW:    
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
			case 0: RuinedBrickWall[24].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
			case 1: RuinedBrickWall[22].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  break;
			case 2: RuinedBrickWall[23].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  break;
			}
		break;

		case RUINED_BRICKWALL_H2_NW_SE: 
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
			case 0: RuinedBrickWall[11].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
			case 1: RuinedBrickWall[12].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  break;
			case 2: RuinedBrickWall[14].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  break;
			}
		break;                     
		
		case RUINED_BRICKWALL_H2_NE_SW: 
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
			case 0: RuinedBrickWall[15].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
			case 1: RuinedBrickWall[25].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  break;
			case 2: RuinedBrickWall[26].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface);  break;
			}
		break;

		case RUINED_BRICKWALL_BS_NW_SE: 	
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: RuinedBrickWall[21].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: RuinedBrickWall[0 ].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: RuinedBrickWall[10].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }

		RuinedBrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface);  
		break;

		case RUINED_BRICKWALL_BS_NE_SW:	
			switch((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % 3) {
            case 0: RuinedBrickWall[20].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 1: RuinedBrickWall[13].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
            case 2: RuinedBrickWall[17].DrawSprite(xScreen, yScreen-95, Clip, OutBound, Wrap, lpSurface); break;
         }

								RuinedBrickWall[2].DrawSprite(xScreen, yScreen-96, Clip, OutBound, Wrap, lpSurface, 1);  
		break;

		case HAIE_B_NW_SE:
		case HAIE_NW_SE: Haie[0].DrawSprite(xScreen, yScreen-43, Clip, OutBound, Wrap, lpSurface, 1); 
		break;
		
		case HAIE_B_NE_SW: 	
		case HAIE_NE_SW: Haie[0].DrawSprite(xScreen, yScreen-43, Clip, OutBound, Wrap, lpSurface);  
		break;
		
		case HAIE_WNE: Haie[1].DrawSprite(xScreen, yScreen-36, Clip, OutBound, Wrap, lpSurface);
		break;
		
		case HAIE_NWS: Haie[2].DrawSprite(xScreen+8, yScreen-36, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case HAIE_NES: Haie[2].DrawSprite(xScreen, yScreen-36, Clip, OutBound, Wrap, lpSurface);
		break;

		case HAIE_WSE: Haie[3].DrawSprite(xScreen, yScreen-43, Clip, OutBound, Wrap, lpSurface);
		break;

		case HAIE_T_NES: Haie[4].DrawSprite(xScreen, yScreen-43, Clip, OutBound, Wrap, lpSurface);
		break;

		case HAIE_T_ESW: Haie[5].DrawSprite(xScreen, yScreen-43, Clip, OutBound, Wrap, lpSurface);
		break;

		case HAIE_T_NSW: Haie[5].DrawSprite(xScreen, yScreen-43, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case HAIE_T_NEW: Haie[4].DrawSprite(xScreen, yScreen-43, Clip, OutBound, Wrap, lpSurface, 1);
		break;

		case HAIE_X: Haie[6].DrawSprite(xScreen, yScreen-43, Clip, OutBound, Wrap, lpSurface);
		break;

      case WOODWALL_SE :        WoodWall[ 0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_NE :        WoodWall[ 1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_NE_WINDOW : WoodWall[ 2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_SE_WINDOW : WoodWall[ 3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_NE_DOORB :  WoodWall[ 4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_NE_DOORT :  WoodWall[ 5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_SE_DOORB :  WoodWall[ 6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_SE_DOORT :  WoodWall[ 7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_CORNER_R :  WoodWall[ 8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_CORNER_T :  WoodWall[ 9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_CORNER_L :  WoodWall[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_CORNER_B :  WoodWall[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_CORNER_X :  WoodWall[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_TNE :       WoodWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_TNW :       WoodWall[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_TSE :       WoodWall[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_TSW :       WoodWall[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_NE_MID :    WoodWall[17].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case WOODWALL_SE_MID :    WoodWall[18].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case GSPEC_CAVERN_I_ENTRANCE: 
         GCavernWall[((Player.xPos+a-Square+7-g_Global.GetNbrTileXDiv2())%9)+1].DrawSprite(xScreen, yScreen-122+DROP, Clip, OutBound, Wrap, lpSurface); 
         Object[220].DrawSprite(xScreen-32, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;
      case GSPEC_CAVERN_I_ENTRANCE_I: 
         GCavernWall[((Player.xPos+a-Square+3-g_Global.GetNbrTileXDiv2())%9)+10].DrawSprite(xScreen, yScreen-122+DROP, Clip, OutBound, Wrap, lpSurface); 
         Object[220].DrawSprite(xScreen+32, yScreen, Clip, OutBound, Wrap, lpSurface, 1); 
      break;
     
      case GCAVERN_SPEC_1_NW_SE: 
        GCavernWall[((Player.xPos+a-Square+2-g_Global.GetNbrTileXDiv2())%9)+19].DrawSprite(xScreen, yScreen-122+80, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case GCAVERN_SPEC_1_NE_SW: 
         GCavernWall[((Player.xPos+a-Square+2-g_Global.GetNbrTileXDiv2())%9)+28].DrawSprite(xScreen, yScreen-122+80, Clip, OutBound, Wrap, lpSurface); 
		break;
	
      case GCAVERN_B_NE_SW: 	
		case GCAVERN_NE_SW: 
         GCavernWall[((Player.xPos+a-Square+3-g_Global.GetNbrTileXDiv2())%9)+10].DrawSprite(xScreen, yScreen-122+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;
		
      case GCAVERN_B_NW_SE:
		case GCAVERN_NW_SE: 
         GCavernWall[((Player.xPos+a-Square+7-g_Global.GetNbrTileXDiv2())%9)+1].DrawSprite(xScreen, yScreen-122+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;
		
		case GCAVERN_WSE: 
         GCavernWall[37].DrawSprite(xScreen-128, yScreen-122+20, Clip, OutBound, Wrap, lpSurface); 
		break;

      case GCAVERN_WNE:
         GCavernWall[0].DrawSprite(xScreen-128, yScreen-122+32+DROP, Clip, OutBound, Wrap, lpSurface);
		break;
      
      case GCAVERN_NES:
			GCavernWall[39].DrawSprite(xScreen-128, yScreen-122-37+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;

      case GCAVERN_NWS:
         GCavernWall[38].DrawSprite(xScreen-248+161, yScreen-122-50+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;

		case GCAVERN_WSE_2: 
         GCavernWall[41].DrawSprite(xScreen-128, yScreen+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;

      case GCAVERN_WNE_2:
         GCavernWall[40].DrawSprite(xScreen-128, yScreen+DROP, Clip, OutBound, Wrap, lpSurface);
		break;
      
      case GCAVERN_NES_2:
			GCavernWall[43].DrawSprite(xScreen-128, yScreen+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;

      case GCAVERN_NWS_2:
         GCavernWall[42].DrawSprite(xScreen-(CavernWall[42].GetWidth()-128), yScreen+DROP, Clip, OutBound, Wrap, lpSurface); 
		break;

      case GCAVERN_SKIP_1: case GCAVERN_SKIP_2: case GCAVERN_SKIP_3: case GCAVERN_SKIP_4:
         CavernG[((Player.yPos+b+Square-g_Global.GetNbrTileYDiv2()) % T_CAVERNG_Y)][((Player.xPos+a+Square-g_Global.GetNbrTileXDiv2()) % T_CAVERNG_X)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); 
      break;

      case NBRICKWALL_SE :        NBrickWall[ 0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_NE :        NBrickWall[ 1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_NE_WINDOW : NBrickWall[ 2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_SE_WINDOW : NBrickWall[ 3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_NE_DOORB :  NBrickWall[ 4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_NE_DOORT :  NBrickWall[ 5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_SE_DOORB :  NBrickWall[ 6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_SE_DOORT :  NBrickWall[ 7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_CORNER_R :  NBrickWall[ 8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_CORNER_T :  NBrickWall[ 9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_CORNER_L :  NBrickWall[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_CORNER_B :  NBrickWall[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_CORNER_X :  NBrickWall[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_TNE :       NBrickWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_TNW :       NBrickWall[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_TSE :       NBrickWall[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALL_TSW :       NBrickWall[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case NBRICKWALLS_SE :        NBrickWallS[ 0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_NE :        NBrickWallS[ 1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_SE_WINDOW : NBrickWallS[ 2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_NE_WINDOW : NBrickWallS[ 3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_NE_DOORB :  NBrickWallS[ 4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_NE_DOORT :  NBrickWallS[ 5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_SE_DOORB :  NBrickWallS[ 6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_SE_DOORT :  NBrickWallS[ 7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_CORNER_R :  NBrickWallS[ 8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_CORNER_T :  NBrickWallS[ 9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_CORNER_L :  NBrickWallS[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_CORNER_B :  NBrickWallS[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_CORNER_X :  NBrickWallS[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_TNE :       NBrickWallS[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_TNW :       NBrickWallS[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_TSE :       NBrickWallS[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_TSW :       NBrickWallS[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_SE_S :       NBrickWallS[17].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_SE_E :       NBrickWallS[18].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_NE_S :       NBrickWallS[19].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS_NE_E :       NBrickWallS[20].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case NBRICKWALLS2_SE :        NBrickWallS2[ 0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_NE :        NBrickWallS2[ 1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_SE_WINDOW : NBrickWallS2[ 2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_NE_WINDOW : NBrickWallS2[ 3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_NE_DOORB :  NBrickWallS2[ 4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_NE_DOORT :  NBrickWallS2[ 5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_SE_DOORB :  NBrickWallS2[ 6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_SE_DOORT :  NBrickWallS2[ 7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_CORNER_R :  NBrickWallS2[ 8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_CORNER_T :  NBrickWallS2[ 9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_CORNER_L :  NBrickWallS2[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_CORNER_B :  NBrickWallS2[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_CORNER_X :  NBrickWallS2[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_TNE :       NBrickWallS2[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_TNW :       NBrickWallS2[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_TSE :       NBrickWallS2[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;
      case NBRICKWALLS2_TSW :       NBrickWallS2[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);
      break;

      case NBRICKWALLS3_SE :        NBrickWallS3[ 0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_NE :        NBrickWallS3[ 1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_SE_WINDOW : NBrickWallS3[ 2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_NE_WINDOW : NBrickWallS3[ 3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_NE_DOORB :  NBrickWallS3[ 4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_NE_DOORT :  NBrickWallS3[ 5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_SE_DOORB :  NBrickWallS3[ 6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_SE_DOORT :  NBrickWallS3[ 7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_CORNER_R :  NBrickWallS3[ 8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_CORNER_T :  NBrickWallS3[ 9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_CORNER_L :  NBrickWallS3[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_CORNER_B :  NBrickWallS3[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_CORNER_X :  NBrickWallS3[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_TNE :       NBrickWallS3[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_TNW :       NBrickWallS3[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_TSE :       NBrickWallS3[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS3_TSW :       NBrickWallS3[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case NBRICKWALLS4_SE :        NBrickWallS4[ 0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_NE :        NBrickWallS4[ 1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_SE_WINDOW : NBrickWallS4[ 2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_NE_WINDOW : NBrickWallS4[ 3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_NE_DOORB :  NBrickWallS4[ 4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_NE_DOORT :  NBrickWallS4[ 5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_SE_DOORB :  NBrickWallS4[ 6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_SE_DOORT :  NBrickWallS4[ 7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_CORNER_R :  NBrickWallS4[ 8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_CORNER_T :  NBrickWallS4[ 9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_CORNER_L :  NBrickWallS4[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_CORNER_B :  NBrickWallS4[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_CORNER_X :  NBrickWallS4[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_TNE :       NBrickWallS4[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_TNW :       NBrickWallS4[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_TSE :       NBrickWallS4[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS4_TSW :       NBrickWallS4[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case NBRICKWALLS5_SE :        NBrickWallS5[ 0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_NE :        NBrickWallS5[ 1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_SE_WINDOW : NBrickWallS5[ 2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_NE_WINDOW : NBrickWallS5[ 3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_NE_DOORB :  NBrickWallS5[ 4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_NE_DOORT :  NBrickWallS5[ 5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_SE_DOORB :  NBrickWallS5[ 6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_SE_DOORT :  NBrickWallS5[ 7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_CORNER_R :  NBrickWallS5[ 8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_CORNER_T :  NBrickWallS5[ 9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_CORNER_L :  NBrickWallS5[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_CORNER_B :  NBrickWallS5[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_CORNER_X :  NBrickWallS5[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_TNE :       NBrickWallS5[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_TNW :       NBrickWallS5[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_TSE :       NBrickWallS5[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS5_TSW :       NBrickWallS5[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case NBRICKWALLS6_SE :        NBrickWallS6[ 0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_NE :        NBrickWallS6[ 1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_SE_WINDOW : NBrickWallS6[ 2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_NE_WINDOW : NBrickWallS6[ 3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_NE_DOORB :  NBrickWallS6[ 4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_NE_DOORT :  NBrickWallS6[ 5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_SE_DOORB :  NBrickWallS6[ 6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_SE_DOORT :  NBrickWallS6[ 7].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_CORNER_R :  NBrickWallS6[ 8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_CORNER_T :  NBrickWallS6[ 9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_CORNER_L :  NBrickWallS6[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_CORNER_B :  NBrickWallS6[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_CORNER_X :  NBrickWallS6[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_TNE :       NBrickWallS6[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_TNW :       NBrickWallS6[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_TSE :       NBrickWallS6[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLS6_TSW :       NBrickWallS6[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case NBRICKWALLRS3_CORNER_B :  NBrickWallRS3[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS3_SE       :  NBrickWallRS3[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS3_NE       :  NBrickWallRS3[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS3_SE_WINDOW:  NBrickWallRS3[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS3_NE_WINDOW:  NBrickWallRS3[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case NBRICKWALLRS4_CORNER_B :  NBrickWallRS4[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS4_SE       :  NBrickWallRS4[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS4_NE       :  NBrickWallRS4[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS4_SE_WINDOW:  NBrickWallRS4[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS4_NE_WINDOW:  NBrickWallRS4[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case NBRICKWALLRS5_CORNER_B :  NBrickWallRS5[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS5_SE       :  NBrickWallRS5[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS5_NE       :  NBrickWallRS5[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS5_SE_WINDOW:  NBrickWallRS5[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS5_NE_WINDOW:  NBrickWallRS5[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case NBRICKWALLRS6_CORNER_B :  NBrickWallRS6[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS6_SE       :  NBrickWallRS6[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS6_NE       :  NBrickWallRS6[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS6_SE_WINDOW:  NBrickWallRS6[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NBRICKWALLRS6_NE_WINDOW:  NBrickWallRS6[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case SPEC_BARILPIED         :  Object[227].DrawSprite(xScreen+32, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_BARILPIED_I       :  Object[227].DrawSprite(xScreen+32, yScreen, Clip, OutBound, Wrap, lpSurface,1); break;
      case SPEC_TOUR_FRONT        :  Object[228].DrawSprite(xScreen+32, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_TOUR_FRONT_I      :  Object[228].DrawSprite(xScreen+32, yScreen, Clip, OutBound, Wrap, lpSurface,1); break;
      case SPEC_TOUR_BACK         :  Object[229].DrawSprite(xScreen+32, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SPEC_TOUR_BACK_I       :  Object[229].DrawSprite(xScreen+32, yScreen, Clip, OutBound, Wrap, lpSurface,1); break;

      case V2_FLOWER_01           :  Object[230].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case V2_FLOWER_02           :  Object[231].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case V2_FLOWER_03           :  Object[232].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case V2_FLOWER_04           :  Object[233].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case V2_FLOWER_05           :  Object[234].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case V2_FLOWER_06           :  Object[235].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case V2_FLOWER_07           :  Object[236].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case CEDRE_NAIN             :  Object[239].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

      case V2CASTLEWALL_SE       :V2CastleWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%4)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_NE       :V2CastleWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%4)+4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_NE_DOORB :V2CastleWall[ 8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_NE_DOORT :V2CastleWall[ 9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_SE_DOORB :V2CastleWall[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_SE_DOORT :V2CastleWall[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_CORNER_R :V2CastleWall[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_CORNER_T :V2CastleWall[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_CORNER_L :V2CastleWall[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_CORNER_B :V2CastleWall[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_CORNER_X :V2CastleWall[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_TNE      :V2CastleWall[17].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_TNW      :V2CastleWall[18].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_TSE      :V2CastleWall[19].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_TSW      :V2CastleWall[20].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_POUTRE   :V2CastleWall[21].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_POUTRE_I :V2CastleWall[21].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1);break;
      case V2CASTLEWALL_POUTRE2  :V2CastleWall[22].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_POUTRE3  :V2CastleWall[23].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_NE_DDOORB:V2CastleWall[24].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_NE_DDOORT:V2CastleWall[25].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_SE_DDOORB:V2CastleWall[26].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLEWALL_SE_DDOORT:V2CastleWall[27].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case V2CASTLE_RAMPART_SE       :V2CastleRampart[0] .DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLE_RAMPART_NE       :V2CastleRampart[1] .DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLE_RAMPART_TOWER_SE :V2CastleRampart[2] .DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLE_RAMPART_TOWER_NE :V2CastleRampart[3] .DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLE_RAMPART_COIN_L   :V2CastleRampart[4] .DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLE_RAMPART_COIN_T   :V2CastleRampart[5] .DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLE_RAMPART_COIN_R   :V2CastleRampart[6] .DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLE_RAMPART_COIN_B   :V2CastleRampart[7] .DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLE_RAMPART_END_D    :V2CastleRampart[8] .DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLE_RAMPART_END_F    :V2CastleRampart[9] .DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2CASTLE_COLLONNE         :V2CastleRampart[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;

      case V2SCASTLEWALL_SE       :V2CastleWallS[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%4)].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_NE       :V2CastleWallS[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2())%4)+4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_MERGE1   :V2CastleWallS[ 8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_MERGE2   :V2CastleWallS[ 9].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_MERGE3   :V2CastleWallS[10].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_MERGE4   :V2CastleWallS[11].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_CORNER_R :V2CastleWallS[12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_CORNER_T :V2CastleWallS[13].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_CORNER_L :V2CastleWallS[14].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_CORNER_B :V2CastleWallS[15].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_CORNER_X :V2CastleWallS[16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_TNE      :V2CastleWallS[17].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_TNW      :V2CastleWallS[18].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_TSE      :V2CastleWallS[19].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case V2SCASTLEWALL_TSW      :V2CastleWallS[20].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;


      case NSTUCOWALL_SE         : NStucoWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()+3)%4)   ].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_NE         : NStucoWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()+3)%4)+4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_NE_WINDOWO : NStucoWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()+3)%4)+8].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_SE_WINDOWO : NStucoWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()+3)%4)+12].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_NE_WINDOWE : NStucoWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()+3)%4)+16].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_SE_WINDOWE : NStucoWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()+3)%4)+20].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_CORNER_R   : NStucoWall[24].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_CORNER_T   : NStucoWall[25].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_CORNER_L   : NStucoWall[26].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_CORNER_B   : NStucoWall[27].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_CORNER_X   : NStucoWall[28].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_TNE        : NStucoWall[29].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_TNW        : NStucoWall[30].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_TSE        : NStucoWall[31].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_TSW        : NStucoWall[32].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_NE_DOORB   : NStucoWall[33].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_NE_DOORT   : NStucoWall[34].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_SE_DOORB   : NStucoWall[35].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_SE_DOORT   : NStucoWall[36].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case NSTUCOWALL_PSE        : NStucoWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()+3)%4)+37].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;
      case NSTUCOWALL_PNE        : NStucoWall[((Player.xPos+a-Square-g_Global.GetNbrTileXDiv2()+3)%4)+41].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break;


      case MUR_POUTRE01          : NMurPoutre[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case MUR_POUTRE02          : NMurPoutre[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case MUR_POUTRE03          : NMurPoutre[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case MUR_POUTRE04          : NMurPoutre[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case MUR_POUTRE05          : NMurPoutre[4].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case MUR_POUTRE06          : NMurPoutre[5].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case MUR_POUTRE07          : NMurPoutre[6].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case TOUR_GARDE_HG         : NTourGarde[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case TOUR_GARDE_HD         : NTourGarde[1].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case TOUR_GARDE_BG         : NTourGarde[2].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 
      case TOUR_GARDE_BD         : NTourGarde[3].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface);break; 

      case BANC_NOIR1       :Object[244].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case BANC_NOIR1_I     :Object[244].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1); break;
      case MUMMY_BOX1       :Object[245].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case MUMMY_BOX1_I     :Object[245].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1); break;
      case MUMMY_BOX2       :Object[246].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case MUMMY_BOX2_I     :Object[246].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1); break;
      case SQUARE_TOWER1    :Object[247].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SQUARE_TOWER1_I  :Object[247].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1); break;
      case SQUARE_TOWER2    :Object[248].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case SQUARE_TOWER2_I  :Object[248].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface,1); break;
      case STATUE_MODEL1_1  :Object[249].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case STATUE_MODEL1_2  :Object[250].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case STATUE_MODEL1_3  :Object[251].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case STATUE_MODEL1_4  :Object[252].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case STATUE_MODEL2_1  :Object[253].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case STATUE_MODEL2_2  :Object[254].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case STATUE_MODEL2_3  :Object[255].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
      case STATUE_MODEL2_4  :Object[256].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

   	default: 
         if (TileSet > 0 && TileSet < LASTONE) 
         {
            if (flpLoadSprite2[TileSet] != NULL)//BLBLBL  
            {
               flpLoadSprite2[TileSet]->Draw(TileSet, xScreen, yScreen, Clip, OutBound, Wrap, lpSurface, a);
            } 
            else 
            {
               Bug[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
            }
         } 
         else 
         {
            Bug[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;
         }
         //Bug[0].DrawSprite(xScreen, yScreen, Clip, OutBound, Wrap, lpSurface); break;

		}

	OVERLAP = FALSE;	
	return true;
	} catch (...) {
      { 
         char Temp[1000];
         
         vir::FileLogger f;
         f.SetLogName("Error.Log");
         f.SetLogLevel(vir::LL_ALL);
         sprintf(Temp, "TileSetError: %d\r\n", TileSet);
         OutputDebugString(Temp);//NMNMNM ++ADD debug ti show tile error in debug mode
         f.Log(vir::LL_ERROR, Temp);
      }
      //throw;
      return true;
	}
}

void TileSet::ReloadMap(void) {
// Does Virtual Map Need Reload ?
	if (xPos > (VirtualWidth-ViewWidth-2) || yPos > (VirtualHeight-ViewHeight-2) || xPos < ViewWidth+1 || 
		yPos < ViewHeight+1) {
		// Need Relead ?
		xPos = VirtualWidth  >> 1;
		yPos = VirtualHeight >> 1;
		LoadVirtualMap(Player.xPos - xPos, Player.yPos - yPos, wPos);
	}
}

void TileSet::Switch2(void) {
}

void TileSet::Switch(void) {
   for (int i = 0; i < 4; i++)	
		lpSmoothSurface2->Blt(&DestRect[i], lpFractalSurface, &SourceRect[i], DDBLT_WAIT, NULL);
	RealFading = SetFading;

	Done2 = Done;
	MovX2 = MovX;
	MovY2 = MovY;
   SepX2 = SepX;
   SepY2 = SepY;

	Objects.GetMainObject()->Direction = Objects.Direction2;

	memcpy(&SmoothSource2, &SmoothSource, sizeof(RECT));
   LOG << "(" << MovX << ", " << MOVX << ", " << MovY << ", " << MOVY << ", " << Done << ")\r\n";
   SmoothSource.left += (MovX*MOVX*Done);
	SmoothSource.top += (MovY*MOVY*Done);
	SmoothSource.right += (MovX*MOVX*Done);
	SmoothSource.bottom += (MovY*MOVY*Done);

	Done = 0;

}	

extern DWORD TOTALOUT;
extern DWORD OUTREALPEEK;
extern DWORD OUTREALLOW;
extern DWORD OUTPEEK;
extern DWORD TIMESTART;
extern DWORD TOTALIN;
extern DWORD INREALPEEK;
extern DWORD INREALLOW;
extern DWORD INPEEK;

extern DWORD dwCriticalTime;

DWORD g_dwPacketLost = 0;
DWORD g_dwPacketAck = 0;

/*void DisplayRect(int iN, int iD, int iS, int iC) {


      double  dNumerator;   
      double  dDenominator;
      double  dFraction;
      RECT    rDisp;
      DDBLTFX ddbltfx;
      
      dNumerator   = iN;
      dDenominator = iD;
      
      if (dNumerator <= 0.000) {
         dNumerator   = 0;
         dDenominator = 1.000;
      } else if (dDenominator <= 0.000) {
         dNumerator   = 1.000;
         dDenominator = 1.000;
      }

      if (dNumerator > dDenominator)
         dNumerator = dDenominator;

      dFraction       = dNumerator / dDenominator;
	
		rDisp.left = iS;
		rDisp.top = 14;
      rDisp.right = iS+(dFraction*91.0);
		rDisp.bottom = 14+7;
		
      ddbltfx.dwSize = sizeof(ddbltfx);
      switch (iC) {
         case 1: {
            if (dFraction >= 0.500) {
               ddbltfx.dwFillColor = ((int)((1.000-dFraction) * lpDXDirectDraw->wRMask * 2) & lpDXDirectDraw->wRMask) + (lpDXDirectDraw->wGMask);
            } else {
               ddbltfx.dwFillColor = (lpDXDirectDraw->wRMask) + ((int)((dFraction)*lpDXDirectDraw->wGMask * 2) & lpDXDirectDraw->wGMask);
            }
         } break;
         case 2: {
            if (dFraction >= 0.500) {
               ddbltfx.dwFillColor = ((int)(((dFraction-0.5)*2) * lpDXDirectDraw->wRMask) & lpDXDirectDraw->wRMask) + (lpDXDirectDraw->wBMask) + (lpDXDirectDraw->wGMask);
            } else {
               ddbltfx.dwFillColor = (lpDXDirectDraw->wBMask) + ((int)((dFraction*2)*lpDXDirectDraw->wGMask) & lpDXDirectDraw->wGMask);
            }
         } break;
         case 3: {
            ddbltfx.dwFillColor = lpDXDirectDraw->wRMask;
         } break;
      }
		lpDXComplexSurface->Blt(&rDisp, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);
		
		rDisp.left = iS+(dFraction*91.0);
		rDisp.top = 14;
      rDisp.right = iS+91;
		rDisp.bottom = 14+7;
		
      ddbltfx.dwSize = sizeof(ddbltfx);
      ddbltfx.dwFillColor = ((lpDXDirectDraw->wRMask >> 2) & lpDXDirectDraw->wRMask) + 
                            ((lpDXDirectDraw->wGMask >> 2) & lpDXDirectDraw->wGMask) + 
                            ((lpDXDirectDraw->wBMask >> 2) & lpDXDirectDraw->wBMask);
		lpDXComplexSurface->Blt(&rDisp, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &ddbltfx);

}*/


void TileSet::UpdateView(bool Draw, bool bNewInt, int xPos, int yPos) 
{
   if (Done2 && !bNewInt) 
   {
      SmoothSource2.left   += MovX2*MOVX;
      SmoothSource2.top    += MovY2*MOVY;
      SmoothSource2.right  += MovX2*MOVX;
      SmoothSource2.bottom += MovY2*MOVY;
      Done2--;
   } 
   
   
   if (!bNewInt)
   {
      lpPreLightSurface->Blt(&ScreenDest, lpSmoothSurface2, &SmoothSource2, DDBLT_WAIT, NULL);
   }

   if(g_SaveGame.GetOptionsParam()->bAnimatedWater)
      DrawWaterLevel(true,MovX2*(Done2)*MOVX, MovY2*(Done2)*MOVY,lpPreLightSurface);

   Objects.Lock(0);
   Objects.Sort();

   Objects.DrawObjectAnim(MovX2*(Done2)*MOVX, MovY2*(Done2)*MOVY, Draw, lpPreLightSurface, true, false, bNewInt);
   Objects.DrawObjectSelect(MovX2*(Done2)*MOVX, MovY2*(Done2)*MOVY, Draw, lpPreLightSurface, true, false, bNewInt);
   Objects.DrawObjectShadow(MovX2*(Done2)*MOVX, MovY2*(Done2)*MOVY, Draw, lpPreLightSurface, true, false, bNewInt);
   Objects.DrawObject(MovX2*(Done2)*MOVX, MovY2*(Done2)*MOVY, Draw, lpPreLightSurface, true, false, bNewInt);
   int dwFound = Objects.FoundAnimListID(99999999);
   if(dwFound>=0)
   {
      Objects.DrawObjectAnimOverLapID(dwFound,true,MovX2*(Done2)*MOVX, MovY2*(Done2)*MOVY,lpPreLightSurface);
   }

   

   Objects.Unlock(0);

   ////////////////////////////////////////
   //  PLUIE / RAIN
   ////////////////////////////////////////
   if(Custom.bEnableGameRain && m_wRainStatus == 0)
   {
      m_wRainStatus   = 1;
      m_wRainAlphaVal = 0;
   }
   else if(Custom.bEnableGameRain && m_wRainStatus == 1 )
   {
      if(m_wRainAlphaVal < 76)
      {
         if(m_dwRainCntGlobal++ %2)
            m_wRainAlphaVal+=4;
      }
      else
         m_wRainStatus = 2;
   }
   else if(!Custom.bEnableGameRain && m_wRainStatus != 0)
   {
      if(m_wRainAlphaVal > 0)
      {
         if(m_dwRainCntGlobal++ %2)
            m_wRainAlphaVal-=4;
      }
      else
      {
         m_wRainStatus   = 0;
         m_wRainAlphaVal = 0;
      }
   }

   bool brainZone = false;   

   //if((CWeather::GetInstance()->GetRainState()) && (Player.World == 0 || (Player.World == 3 && Player.yPos < 1536)))
   if((Player.World == 0 || Player.World == 4 || (Player.World == 3 && Player.yPos < 1536)) && m_wRainStatus != 0 && g_SaveGame.GetOptionsParam()->bShowWeatherEffects )
   {
	  //if(!bSnowZone)
	  {
		  V2SPRITEFX fx;
		  memset( &fx, 0, sizeof( fx ) );
		  // Setup the clipping rectangle
		  RECT rect = { 0, 0, g_Global.GetScreenW(), g_Global.GetScreenH()};
		  fx.lpClipRect = &rect;
		  // Specify no correction.
		  fx.dwFX |= FX_NOCORRECTION;
		  fx.lpDirectDrawSurface = lpPreLightSurface;
		  CWeather::GetInstance()->DrawRain(&m_VSFWeatherRain,m_VSFWeatherLightning,&fx);
	  }
   }

   ////////////////////////////////////////
   //  NEIGE / SNOW
   ////////////////////////////////////////
   if(Custom.bEnableGameSnow && m_wSnowStatus == 0)
   {
      m_wSnowStatus   = 1;
      m_wSnowAlphaVal = 0;
   }
   else if(Custom.bEnableGameSnow && m_wSnowStatus == 1 )
   {
      if(m_wSnowAlphaVal < 76)
      {
         if(m_dwSnowCntGlobal++ %2)
            m_wSnowAlphaVal+=4;
      }
      else
         m_wSnowStatus = 2;
   }
   else if(!Custom.bEnableGameSnow && m_wSnowStatus != 0)
   {
      if(m_wSnowAlphaVal > 0)
      {
         if(m_dwSnowCntGlobal++ %2)
            m_wSnowAlphaVal-=4;
      }
      else
      {
         m_wSnowStatus   = 0;
         m_wSnowAlphaVal = 0;
      }
   }
   
   if( m_wSnowStatus != 0 && g_SaveGame.GetOptionsParam()->bShowWeatherEffects)
   //if( m_wSnowStatus != 0 && Player.World == 0 || Player.World == 4 || (Player.World == 3 && Player.yPos < 1536) && g_SaveGame.GetOptionsParam()->bShowWeatherEffects )
   {
      V2SPRITEFX fx;
      memset( &fx, 0, sizeof( fx ) );
      // Setup the clipping rectangle
      RECT rect = { 0, 0, g_Global.GetScreenW(), g_Global.GetScreenH()};
      fx.lpClipRect = &rect;
      // Specify no correction.
      fx.dwFX |= FX_NOCORRECTION;
      fx.lpDirectDrawSurface = lpPreLightSurface;
      CWeather::GetInstance()->DrawSnow(&m_VSFWeatherSnow1,&m_VSFWeatherSnow2,&fx,m_wSnowAlphaVal);
   }

   ////////////////////////////////////////
   //  BROUILLARD / FOG
   ////////////////////////////////////////
   if(Custom.bEnableGameFog && m_wFogStatus == 0)
   {
      m_wFogStatus   = 1;
      m_wFogAlphaVal = 0;
   }
   else if(Custom.bEnableGameFog && m_wFogStatus == 1 )
   {
      if(m_wFogAlphaVal < 140)
      {
         if(m_dwFogCntGlobal++ %2)
            m_wFogAlphaVal+=4;
      }
      else
         m_wFogStatus = 2;
   }
   else if(!Custom.bEnableGameFog && m_wFogStatus != 0)
   {
      if(m_wFogAlphaVal > 0)
      {
         if(m_dwFogCntGlobal++ %2)
            m_wFogAlphaVal-=4;
      }
      else
      {
         m_wFogStatus   = 0;
         m_wFogAlphaVal = 0;
      }
   }
   
   if((Player.World == 0 || Player.World == 4 || (Player.World == 3 && Player.yPos < 1536)) && m_wFogStatus != 0 && g_SaveGame.GetOptionsParam()->bShowWeatherEffects)
   {
      // change le mode du fog progressivement, quand le brouillard apparait,
      // on le fais apparaitre progressivement et pareil quand il arrete...
      // pour ne pas passer de tout a rien direct...
      
      static unsigned int suiFogCnt = 0;
      V2SPRITEFX fx;
      memset( &fx, 0, sizeof( fx ) );
      // Setup the clipping rectangle
      RECT rect = { 0, 0, g_Global.GetScreenW(), g_Global.GetScreenH()};
      fx.lpClipRect = &rect;
      // Specify no correction.
      fx.lpDirectDrawSurface = lpPreLightSurface;
      fx.dwFX |= FX_NOCORRECTION;
      
      for(int i=0;i<15;i++)
      {
         if(m_dwFogX[i]+511 >0 && m_dwFogY[i]+383 >0 && m_dwFogX[i] <g_Global.GetScreenW()-1 && m_dwFogY[i] <g_Global.GetScreenH()-1)
            m_VSFWeatherSemiTransFog.DrawSpriteNSemiTrans(m_dwFogX[i],m_dwFogY[i],&fx,TRUE,(BYTE)m_wFogAlphaVal);   
         
         if(suiFogCnt ==1)
         {
            m_dwFogX[i]-=4;
            m_fFoyY[i]-=3.00;
            m_dwFogY[i] = (int)m_fFoyY[i];
         }
      }
      ValidFogMoving();
      suiFogCnt++;
      if(suiFogCnt == 2)
         suiFogCnt = 0;
   }
   
   
   if (!bNewInt) 
   {
      
      DDSURFACEDESC Surface;
      memset(&Surface, 0, sizeof(Surface));
      Surface.dwSize = sizeof(Surface);
      DDSURFACEDESC Surface2;
      memset(&Surface2, 0, sizeof(Surface2));
      Surface2.dwSize = sizeof(Surface2);


      LPDIRECTDRAWSURFACE lpBackBuffer;
      DDSURFACEDESC SpriteZoom;
            
      int dwZoomStatus = g_Global.GetZoomStatus();
      if(dwZoomStatus == 0)
      {
         lpBackBuffer = lpDXDirectDraw->PicklockBackBuffer( NULL, &Surface, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL );
         if( lpBackBuffer == NULL )
         {
            return;
         }
      }
      else
      {
         memset(&SpriteZoom, 0, sizeof(SpriteZoom));
         SpriteZoom.dwSize            = sizeof(SpriteZoom);
         SpriteZoom.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
         SpriteZoom.ddsCaps.dwCaps    = DDSCAPS_SYSTEMMEMORY;
         SpriteZoom.dwWidth           = g_Global.GetScreenW();
         SpriteZoom.dwHeight          = g_Global.GetScreenW();
         
         DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&SpriteZoom, &lpBackBuffer, NULL), "Surface Zoom");
         lpBackBuffer->Lock(NULL, &Surface, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
      }

      if( lpPreLightSurface->IsLost() != DD_OK )
      {
         lpBackBuffer->Unlock(Surface.lpSurface);
         return;
      }
      lpPreLightSurface->Lock(NULL, &Surface2, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);
      LightMap::MakeLightingFX((LPBYTE)Surface.lpSurface, (LPBYTE)Surface2.lpSurface,Surface.lPitch, Surface2.lPitch, xPos, yPos);

      lpPreLightSurface->Unlock(Surface2.lpSurface);
      lpBackBuffer->Unlock(Surface.lpSurface);

      if(dwZoomStatus >0)
      {
         int dwNewWidth  =   g_Global.GetScreenW()- (g_Global.GetScreenW()*(5*dwZoomStatus)/100);
         int dwNewHeight =   g_Global.GetScreenH()- (g_Global.GetScreenH()*(5*dwZoomStatus)/100);

         RECT destRectTmp;
         destRectTmp.left   = 0;
         destRectTmp.top    = 0; 
         destRectTmp.right  = g_Global.GetScreenW();
         destRectTmp.bottom = g_Global.GetScreenH();
         
         RECT rcNewScr;
         rcNewScr.left   = (g_Global.GetScreenW()-dwNewWidth)/2;
         rcNewScr.top    = (g_Global.GetScreenH()-dwNewHeight)/2;
         rcNewScr.right  = rcNewScr.left+dwNewWidth;
         rcNewScr.bottom = rcNewScr.top +dwNewHeight;
        
         LPDIRECTDRAWSURFACE ToBlit = lpDXDirectDraw->GetBackBufferDirect();
         ToBlit->Blt(&destRectTmp, lpBackBuffer, &rcNewScr, DDBLT_WAIT,NULL);
         lpBackBuffer->Release();
         
 
         int xPos2 = Objects.ViewWidth *32-32;
         int yPos2 = (int)(Objects.ViewHeight*16)-(int)(70.00*((double)g_Global.GetScreenH()/(double)dwNewHeight));


         Objects.Lock(0);
         Objects.DrawObjectSelect(MovX2*(Done2)*MOVX+(xPos/2), MovY2*(Done2)*MOVY+(yPos/2), Draw, lpDXDirectDraw->GetBackBufferDirect(), true, true, false);
         Objects.DrawObjectShadow(MovX2*(Done2)*MOVX+(xPos/2), MovY2*(Done2)*MOVY+(yPos/2), Draw, lpDXDirectDraw->GetBackBufferDirect(), true, true, false);
         Objects.DrawObject(MovX2*(Done2)*MOVX+(xPos/2), MovY2*(Done2)*MOVY+(yPos/2), Draw, lpDXDirectDraw->GetBackBufferDirect(), true, true, false);
         Objects.GetMainObject()->DrawTalkText(xPos2,yPos2);
         Objects.Unlock(0);
      }
      else
      {
         //Dessine le texte au dessus des PNJ...
         Objects.Lock(0);
         Objects.DrawObjectSelect(MovX2*(Done2)*MOVX+(xPos/2), MovY2*(Done2)*MOVY+(yPos/2), Draw, lpDXDirectDraw->GetBackBufferDirect(), true, true, false);
         Objects.DrawObjectShadow(MovX2*(Done2)*MOVX+(xPos/2), MovY2*(Done2)*MOVY+(yPos/2), Draw, lpDXDirectDraw->GetBackBufferDirect(), true, true, false);
         Objects.DrawObject(MovX2*(Done2)*MOVX+(xPos/2), MovY2*(Done2)*MOVY+(yPos/2), Draw, lpDXDirectDraw->GetBackBufferDirect(), true, true, false);
         Objects.GetMainObject()->DrawTalkText(Objects.ViewWidth*32-32+(xPos/2),Objects.ViewHeight*16-67+(yPos/2));
         Objects.Unlock(0);
      }
     

      if (LevelUp) 
      {
         HDC hdc;
         DXDGetDC(&hdc, 29);
         
         int s = g_Global.GetScreenWDiv2()+(xPos/2);
         s -= (fMenuFont->GetLen(hdc, g_LocalString[86])/2);

         fMenuFont->DrawFont(hdc, s-1, 150+(yPos/2)-1, 0, 0, g_LocalString[ 86 ] );
         fMenuFont->DrawFont(hdc, s+1, 150+(yPos/2)+1, 0, 0, g_LocalString[ 86 ] );
         fMenuFont->DrawFont(hdc, s, 150+(yPos/2), RGB(255, 255, 255), 0, g_LocalString[ 86 ] );
         
         DXDReleaseDC(hdc, 29);
         
      }

     
      if (Player.AttackMode & 1) 
      {
         V2SPRITEFX fx;
         memset( &fx, 0, sizeof( fx ) );
         // Setup the clipping rectangle
         RECT rect = { 0, 0, g_Global.GetScreenW(), g_Global.GetScreenH() };
         fx.lpClipRect = &rect;
         // Specify no correction.
         fx.dwFX |= FX_NOCORRECTION;
         int dwOffsetDraw = 0;
         if(g_SaveGame.GetOptionsParam()->dwMenuState == MENU_CHAT || g_SaveGame.GetOptionsParam()->dwMenuState == MENU_BOTH)
            dwOffsetDraw = 112;
         AttackCursorIcon.DrawSpriteN(g_Global.GetScreenW()-AttackCursorIcon.GetWidth(), g_Global.GetScreenH()-AttackCursorIcon.GetHeight()+yPos-2-dwOffsetDraw, &fx );
      } 
   } 
   
  
   /*
   //Press F1 for help String
   // If this is a beginner
   if (Player.Exp < 200 &&!bNewInt) 
   {
      DXDGetDC(&hdc, 43);
      DWORD width = fSkillFont->GetLen( hdc, g_LocalString[ 443 ] );
      fSkillFont->DrawFont(hdc, (g_Global.GetScreenW()-30) - width, 100, RGB(255, 255, 255), 0, g_LocalString[443]);
      DXDReleaseDC(hdc, 43);
   }
   */
}

int TileSet::MoveToPosition(int NewX, int NewY) {
// Make the map move to x,y position.
   int j;
   int Flag = 0;
	
   if (!Done) 
   {
      MovX = NewX - Player.xPos;
		MovY = NewY - Player.yPos;

      if (MovX > 1 || MovY > 1) {
         LOG << "* ERR = MovX || MovY > 1\r\n" ;
         exit(0);
      }

		Done = DONE;

		xPos  += MovX;
		yPos  += MovY;
		Player.xPos += MovX;
		Player.yPos += MovY;

      g_Global.ValidMapZonePosition(Player.xPos,Player.yPos);
      
		ReloadMap();

		OldSepX = SepX;
		OldSepY = SepY;

		SepX += MovX * TileWidth;
		SepY += MovY * TileHeight;

		if (SepX < 0) SepX += TileWidth*ViewWidth+TileWidth*2;
		if (SepY < 0) SepY += TileHeight*ViewHeight+TileHeight*2;
	
		SepX = SepX % (TileWidth*ViewWidth+TileWidth*2);
		SepY = SepY % (TileHeight*ViewHeight+TileHeight*2);

		if (MovX) 
			Flag |= 2;
		if (MovY)
			Flag |= 1;
		
		DestRect[0].left     = 0;
		DestRect[0].top      = 0;
		DestRect[0].right    = ViewWidth *TileWidth +TileWidth *2-SepX;
		DestRect[0].bottom   = ViewHeight*TileHeight+TileHeight*2-SepY;

		DestRect[1].left     = 0;
		DestRect[1].top      = ViewHeight*TileHeight+TileHeight*2-SepY;
		DestRect[1].right    = ViewWidth *TileWidth +TileWidth *2-SepX;
		DestRect[1].bottom   = ViewHeight*TileHeight+TileHeight*2;

		DestRect[2].left     = ViewWidth *TileWidth +TileWidth *2-SepX;
		DestRect[2].top      = 0;
		DestRect[2].right    = ViewWidth *TileWidth +TileWidth *2;
		DestRect[2].bottom   = ViewHeight*TileHeight+TileHeight*2-SepY;

		DestRect[3].left     = ViewWidth *TileWidth +TileWidth *2-SepX;
		DestRect[3].top      = ViewHeight*TileHeight+TileHeight*2-SepY;
		DestRect[3].right    = ViewWidth *TileWidth +TileWidth *2;
		DestRect[3].bottom   = ViewHeight*TileHeight+TileHeight*2;

		SourceRect[0].left   = SepX;
		SourceRect[0].top    = SepY;
		SourceRect[0].right  = TileWidth *ViewWidth +TileWidth *2;
		SourceRect[0].bottom = TileHeight*ViewHeight+TileHeight*2;

		SourceRect[1].left   = SepX;
		SourceRect[1].top    = 0;
		SourceRect[1].right  = TileWidth*ViewWidth+TileWidth*2;
		SourceRect[1].bottom = SepY;

		SourceRect[2].left   = 0;
		SourceRect[2].top    = SepY;
		SourceRect[2].right  = SepX;
		SourceRect[2].bottom = TileHeight*ViewHeight+TileHeight*2;

		SourceRect[3].left   = 0;
		SourceRect[3].top    = 0;
		SourceRect[3].right  = SepX;
		SourceRect[3].bottom = SepY;
      


         CompileView(Flag);
      
		for (j = 0; j < Done; j++) {
			SmoothSource.left   -= MovX*MOVX;
			SmoothSource.top    -= MovY*MOVY;
			SmoothSource.right  -= MovX*MOVX;
			SmoothSource.bottom -= MovY*MOVY;
		}

   }

   if (MovX == 1 && MovY == 0)
		return 6;
	else if (MovX == -1 && MovY == 0)
		return 4;
	else if (MovX == 0 && MovY == 1)
		return 2;
	else if (MovX == 0 && MovY == -1)
		return 8;
	else if (MovX == 1 && MovY == 1)
		return 3;
	else if (MovX == 1 && MovY == -1)
		return 9;
	else if (MovX == -1 && MovY == 1)
		return 1;
	else if (MovX == -1 && MovY == -1)
		return 7;
	return 0;
}

void TileSet::DrawWaterLevel(bool bOffset, int XCor, int YCor,LPDIRECTDRAWSURFACE lpBlitSurface) 
{
   static int RepeatCnt = 0;
   static int RepeatCntLave = 0;

   if(++RepeatCnt>=3)
   {
      RepeatCnt = 0;
      g_Global.StepAnimWaterFrame();
   }

   /*
   if(++RepeatCntLave>=4) 
   {
      RepeatCntLave = 0;
      g_Global.StepAnimLavaFrame();
   }
   */


   RECT Clip;
   Clip.left   = 0;
   Clip.right  = g_Global.GetScreenW();
   Clip.top    = 0;
   Clip.bottom = g_Global.GetScreenH();

   XCor -= 48;
   YCor += - 24 + 37;
 

   Middle = 0;
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
   
   int yPos =  b1;
   int xPos =  a1;

   xPos -= 64;
	yPos += 21;

  
   int xScreen, yScreen;

   int dwXMin = 99999;
   int dwYMin = 99999;
   int dwXMax = 0;
   int dwYMax = 0;

   BOOL bWaterFound = FALSE;
   yScreen = -(Square*TileHeight)-16;
   for (int j = 0; j < ViewHeight+Square+Square; j++) 
   {
      // Loop thru all the y component.
      xScreen = (ViewWidth+Square-1)*TileWidth-64;
      
      //int DrawMe = 0;
      for (int i = ViewWidth+Square+Square-1; i > -1; i--)
      {
         // Loop thru all the x component.
         a = i;
			b = j;
         {
            if (CompiledView[i+j*(ViewWidth+Square+Square)] == FLOOR_ANIM_WATER01)
            {
               DrawTileSet(xScreen-xPos+XCor, yScreen-yPos+YCor, FLOOR_ANIM_WATER01D,&Clip, NULL, NULL, false, lpBlitSurface, false, FALSE,0,g_Global.GetAnimWaterFrame());
               bWaterFound = TRUE;
            }
            /*
            else if (CompiledView[i+j*(ViewWidth+Square+Square)] == FLOOR_ANIM_LAVA01)
            {
               DrawTileSet(xScreen-xPos+XCor, yScreen-yPos+YCor, FLOOR_ANIM_LAVA01,&Clip, NULL, NULL, false, lpBlitSurface, false, FALSE,0,g_Global.GetAnimLavaFrame());
               bWaterFound = TRUE;
            }
            */
            else if(CompiledView[i+j*(ViewWidth+Square+Square)] >= N_AWAT01TER && CompiledView[i+j*(ViewWidth+Square+Square)] <= X5NW_AWAT01FOR)
            {
               DrawTileSet(xScreen-xPos+XCor, yScreen-yPos+YCor, CompiledView[i+j*(ViewWidth+Square+Square)],&Clip, NULL, NULL, false, lpBlitSurface, false, FALSE,0,g_Global.GetAnimWaterFrame());
               bWaterFound = TRUE;
            }
            /*
            else if(CompiledView[i+j*(ViewWidth+Square+Square)] >= N_ALAV01CEN && CompiledView[i+j*(ViewWidth+Square+Square)] <= X5NW_ALAV01CEN)
            {
               DrawTileSet(xScreen-xPos+XCor, yScreen-yPos+YCor, CompiledView[i+j*(ViewWidth+Square+Square)],&Clip, NULL, NULL, false, lpBlitSurface, false, FALSE,0,g_Global.GetAnimLavaFrame());
               bWaterFound = TRUE;
            }
            */
         }
         xScreen -= TileWidth;
      }
      yScreen += TileHeight;
   }

   /*
   Clip.left   = dwXMin;
   Clip.right  = dwXMax+32;
   Clip.top    = dwYMin;
   Clip.bottom = dwYMax+16;

   if(Clip.left <0)
      Clip.left = 0;
   if(Clip.top <0)
      Clip.top = 0;
   */

   //maintennat on boucle pour la liste des overlaps...
   if(bWaterFound)
   {
      yScreen = -(Square*TileHeight)-16;
	  int j;
      for (j = 0; j < ViewHeight+Square+Square; j++) 
      {
         // Loop thru all the y component.
         xScreen = (ViewWidth+Square-1)*TileWidth-64;
      
         int DrawMe = 0;
         for (int i = ViewWidth+Square+Square-1; i > -1; i--)
         {
            // Loop thru all the x component.
            a = i;
			   b = j;
            {
               if((CompiledView[i+j*(ViewWidth+Square+Square)] > OBJECT_DO_OVERLAP_S && CompiledView[i+j*(ViewWidth+Square+Square)] < OBJECT_DO_OVERLAP_E) ||
                  (CompiledView[i+j*(ViewWidth+Square+Square)] == SPEC_CASTLE_BRIDGE) || (CompiledView[i+j*(ViewWidth+Square+Square)] == SPEC_CASTLE_BRIDGE_I) ||
                  (CompiledView[i+j*(ViewWidth+Square+Square)] == SPEC_SMALL_BRIDGE ) || (CompiledView[i+j*(ViewWidth+Square+Square)] == SPEC_SMALL_BRIDGE_I )   )
               
               {
                  DrawTileSet(xScreen-xPos+XCor, yScreen-yPos+YCor, CompiledView[i+j*(ViewWidth+Square+Square)],&Clip, NULL, NULL, false, lpBlitSurface, false, FALSE,0,g_Global.GetAnimWaterFrame());
               }
            }
            xScreen -= TileWidth;
         }
         yScreen += TileHeight;
      }
   }
};

void TileSet::CompileView(int Direction) {
/* This fucntion draw all the land type.
 * Time critical function.
 */
	RECT Wrap;
	int i, j, k;
	int xScreen, yScreen;
	RECT Clip;
	
	Wrap.left = 0;
	Wrap.top = 0;
	Wrap.right = (ViewWidth+2)  * TileWidth;
	Wrap.bottom = (ViewHeight+2)  * TileHeight;

	RECT OutBound;

	OutBound.left = 0;
	OutBound.top = 0;
	OutBound.right = (ViewWidth+2)  * TileWidth;
	OutBound.bottom = (ViewHeight+2)  * TileHeight;
   
   try {
   for (i = 0; i < ViewWidth+Square+Square; i++) {
		for (j = 0; j < ViewHeight+Square+Square; j++) {
			if (i-1-(ViewWidth>>1)-Square+xPos < 0 || i-1-(ViewWidth>>1)-Square+xPos > 3071 || 
            j-2-(ViewHeight>>1)-Square+yPos < 0 || j-2-(ViewHeight>>1)-Square+yPos > 3071) {
            CompiledView[i+j*(ViewWidth+Square+Square)] = 20;
         } else {
            CompiledView[i+j*(ViewWidth+Square+Square)] = VirtualView[i-1-(ViewWidth>>1)-Square+xPos+(j-2-(ViewHeight>>1)-Square+yPos)*VirtualWidth];
         }
      } 
	} 
   } catch (...) {
      {
         vir::FileLogger Log;
         Log.SetLogName("Error.Log");
         Log.SetLogLevel(vir::LL_ALL);
         char Temp[1000];
         sprintf(Temp, "i = %d, j = %d, ViewWidth = %d, ViewHeight = %d, Square = %d, " 
                       "xPos = %d, yPos = %d\r\n", i, j, ViewWidth, ViewHeight, Square, xPos, yPos);
         Log.Log(vir::LL_ERROR, Temp);
      } 
      throw;
   }

   if (Direction & 1) {
	// Horizontal
		yScreen = OldSepY;

		if (MovY == 1)
			j = ViewHeight+1+Square;
		else

			j = 1+Square;
		Middle = SepX/TileWidth;

		// Draw Normal
		xScreen = Middle*TileWidth;
		for (i=Middle+Square; i < ViewWidth+2+Square; i++) {
			a = i-Middle;
			b = j;
			if (CompiledView[(i-Middle)+j*(ViewWidth+Square+Square)] < SPECIAL_S || CompiledView[(i-Middle)+j*(ViewWidth+Square+Square)] > SPECIAL_E)
				DrawTileSet(xScreen, yScreen, CompiledView[(i-Middle)+j*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, false, lpFractalSurface, false, FALSE);
			else if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1)) {
            if(DrawTileSet(xScreen, yScreen, CompiledView[((i)-Middle)+(j+1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i-1)-Middle)+(j+1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i+1)-Middle)+(j+1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i-1)-Middle)+(j)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i-1)-Middle)+(j-1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i)-Middle)+(j-1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i+1)-Middle)+(j-1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i+1)-Middle)+(j)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
			}
			xScreen += TileWidth;
		}
		xScreen = 0;
		for (i=Square; i < Middle+Square; i++) {
			a = i+ViewWidth+2-Middle;
			b = j;
			if (CompiledView[(i+ViewWidth+2-Middle)+j*(ViewWidth+Square+Square)] < SPECIAL_S || CompiledView[(i+ViewWidth+2-Middle)+j*(ViewWidth+Square+Square)] > SPECIAL_E)
				DrawTileSet(xScreen, yScreen, CompiledView[(i+ViewWidth+2-Middle)+j*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, false, lpFractalSurface, false, FALSE);
			else if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1)) {
            if(DrawTileSet(xScreen, yScreen, CompiledView[((i)+ViewWidth+2-Middle)+(j+1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i-1)+ViewWidth+2-Middle)+(j+1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i+1)+ViewWidth+2-Middle)+(j+1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i-1)+ViewWidth+2-Middle)+(j)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i-1)+ViewWidth+2-Middle)+(j-1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[((i)+ViewWidth+2-Middle)+(j-1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i+1)+ViewWidth+2-Middle)+(j-1)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if(DrawTileSet(xScreen, yScreen, CompiledView[((i+1)+ViewWidth+2-Middle)+(j)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
			}
			xScreen += TileWidth;
		}

		if (MovY == 1) {
		// Search and Draw Special Down
			yScreen = OldSepY-(Square)*TileHeight;
			Clip.left = Middle*TileWidth;
			Clip.top  = OldSepY;
			Clip.right = (Middle+ViewWidth+2)*TileWidth;
			Clip.bottom = OldSepY+TileHeight;
			yScreen -= TileHeight;

			for (k = ViewHeight+1; k < ViewHeight+Square+Square; k++) {
				xScreen = Middle*TileWidth+(ViewWidth+Square-1)*TileWidth;
				yScreen += TileHeight;
				b = k;
				for (i=ViewWidth+Square+Square-1; i > -1; i--) {
					a = i;
					if (CompiledView[(i)+k*(ViewWidth+Square+Square)] > SPECIAL_S && CompiledView[(i)+k*(ViewWidth+Square+Square)] <= SPECIAL_E) 
						DrawTileSet(xScreen, yScreen, CompiledView[(i)+k*(ViewWidth+Square+Square)], &Clip, &OutBound, &Wrap, false, lpFractalSurface, false, FALSE);
					xScreen -= TileWidth;
				}
			}
		} else {
		// Search and Draw Special Up
			yScreen = OldSepY-TileHeight-Square*TileHeight;
			Clip.left = Middle*TileWidth;
			Clip.top = OldSepY-TileHeight;
			Clip.right = (Middle+ViewWidth+2)*TileWidth;
			Clip.bottom = OldSepY+TileHeight;

			for (k = 1; k < Square+Square; k++) {
				xScreen = Middle*TileWidth+(ViewWidth+Square-1)*TileWidth;
				yScreen += TileHeight;
				b = k;
				for (i=ViewWidth+Square+Square-1; i > -1; i--) {
					a = i;
					if (CompiledView[(i)+k*(ViewWidth+Square+Square)] > SPECIAL_S && CompiledView[(i)+k*(ViewWidth+Square+Square)] <= SPECIAL_E) 
						DrawTileSet(xScreen, yScreen, CompiledView[(i)+k*(ViewWidth+Square+Square)], &Clip, &OutBound, &Wrap, false, lpFractalSurface, false, FALSE);
					xScreen -= TileWidth;
				}
			}
		}
	}

	if (Direction & 2) {
	// Vertical
		xScreen= OldSepX;

		if (MovX == 1)
			i = ViewWidth+1+Square;
		else
			i = 1+Square;
		Middle = SepY/TileHeight;

		// Draw Normal
		yScreen = Middle*TileHeight;
		for (j=Middle+Square; j < ViewHeight+Square+2; j++) {
			a = i;
			b = j-Middle;
			if (CompiledView[i+(j-Middle)*(ViewWidth+Square+Square)] < SPECIAL_S || CompiledView[i+(j-Middle)*(ViewWidth+Square+Square)] > SPECIAL_E)
				DrawTileSet(xScreen, yScreen, CompiledView[i+(j-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, false, lpFractalSurface, false, FALSE);
			else if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1)) {
            if (DrawTileSet(xScreen, yScreen, CompiledView[(i)+((j+1)-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i-1)+((j+1)-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i+1)+((j+1)-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i-1)+((j)-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i-1)+((j-1)-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i)+((j-1)-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i+1)+((j-1)-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i+1)+((j)-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
			}
			yScreen += TileHeight;
		}
		yScreen = 0;
		for (j=Square; j < Middle+Square; j++) {
			a = i;
			b = j+ViewHeight+2-Middle;
		if (CompiledView[i+(j+ViewHeight+2-Middle)*(ViewWidth+Square+Square)] < SPECIAL_S || CompiledView[i+(j+ViewHeight+2-Middle)*(ViewWidth+Square+Square)] > SPECIAL_E)
				DrawTileSet(xScreen, yScreen, CompiledView[i+(j+ViewHeight+2-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, false, lpFractalSurface, false, FALSE);
			else if (i > 0 && i < (ViewWidth+Square+Square-1) && j > 0 && j < (ViewHeight+Square+Square-1)) {
            if (DrawTileSet(xScreen, yScreen, CompiledView[(i)+((j+1)+ViewHeight+2-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i-1)+((j+1)+ViewHeight+2-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i+1)+((j+1)+ViewHeight+2-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i-1)+((j)+ViewHeight+2-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i-1)+((j-1)+ViewHeight+2-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i)+((j-1)+ViewHeight+2-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i+1)+((j-1)+ViewHeight+2-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
            else if (DrawTileSet(xScreen, yScreen, CompiledView[(i+1)+((j)+ViewHeight+2-Middle)*(ViewWidth+Square+Square)], NULL, &OutBound, &Wrap, true, lpFractalSurface, false, FALSE)) {}
			}
			yScreen += TileHeight;
		}

		if (MovX == 1) {
		// Search and Draw Special Right
			Clip.left   = OldSepX;
			Clip.top    = Middle*TileHeight;
			Clip.right  = OldSepX+TileWidth;
			Clip.bottom = (Middle+ViewHeight+2) * TileHeight;
			
			yScreen = SepY-(Square*TileHeight);
			for (j=0; j < ViewHeight+Square+Square; j++) {
				xScreen = OldSepX+(Square-2)*TileWidth;
				b = j;
				for (k = ViewWidth+Square+Square-1; k > ViewWidth; k--) {
					a = k;
					if (CompiledView[k+j*(ViewWidth+Square+Square)] > SPECIAL_S && CompiledView[k+j*(ViewWidth+Square+Square)] <= SPECIAL_E) 
						DrawTileSet(xScreen, yScreen, CompiledView[k+j*(ViewWidth+Square+Square)], &Clip, &OutBound, &Wrap, false, lpFractalSurface, false, FALSE);
					xScreen -= TileWidth;
				}
				yScreen += TileHeight;
			}
		} else {
		// Search and Draw Special Left
			
			Clip.left = OldSepX;
			Clip.top = Middle*TileHeight;
			Clip.right = OldSepX+TileWidth;
			Clip.bottom = (Middle+ViewHeight+2) * TileHeight;

			yScreen = SepY-(Square*TileHeight);
			for (j=0; j < ViewHeight+Square+Square; j++) {
				xScreen = OldSepX-TileWidth+((Square-1)*TileWidth);
				b = j;
				for (k = Square+Square-1; k > 0; k--) {
					a = k;
					if (CompiledView[k+(j)*(ViewWidth+Square+Square)] > SPECIAL_S && CompiledView[k+(j)*(ViewWidth+Square+Square)] <= SPECIAL_E) 
						DrawTileSet(xScreen, yScreen, CompiledView[k+(j)*(ViewWidth+Square+Square)], &Clip, &OutBound, &Wrap, false, lpFractalSurface, false, FALSE);
					xScreen -= TileWidth;
				}
				yScreen += TileHeight;
			}
		}
	}
}

bool TileSet::Block(signed char x, signed char y) {
// Chk if something block the way
	if (CompiledView[Square+11+x+(Square+16+y)*(ViewWidth+Square+Square)] > SPECIAL_S && CompiledView[Square+11+x+(Square+16+y)*(ViewWidth+Square+Square)] <= SPECIAL_E) 
   {
		return true;
	} 
	return false;
}

void TileSet::Resize(int x, int y) {
	x+=1;
	y+=1;
	int xdif = (ViewWidth-x)<<4;
	int ydif = (ViewHeight-y)<<3;

   Objects.Lock(0);
	Objects.SetView(x, y);	
   Objects.Unlock(0);
	SizeWidth = x;
	SizeHeight = y;
	
	SmoothSource.left       = TileWidth+xdif+TileWidth/2;
	SmoothSource.top        = TileHeight+ydif+TileHeight/2;
	SmoothSource.right      = TileWidth *SizeWidth +TileWidth+xdif-TileWidth/2;
	SmoothSource.bottom     = TileHeight*SizeHeight+TileHeight+ydif-TileHeight/2;

	ScreenDest.left         = 0;
	ScreenDest.top          = 0;
	ScreenDest.right        = (SizeWidth-1) *TileWidth;
	ScreenDest.bottom       = (SizeHeight-1)*TileHeight;

	memcpy(&SmoothSource2, &SmoothSource, sizeof(RECT));
}

void TileSet::GetHeight(int x, int y, int *X, int *Y, signed char *C) {
	y+=2;

	int i, j;
	*X = 0;
	*Y = 0;
	            
	i  = Square+x;
	j  = Square+y;
   if (i > 2 && i < (ViewWidth+Square+Square-3) && j > 2 && j < (ViewHeight+Square+Square-3)) {
	   if (CompiledView[i+j*(ViewWidth+Square+Square)] == ROUND_WOODEN_TABLE) {
   		*Y = -3;
		   *X = 0;
		   *C = 3;
	   } else if (CompiledView[i+(j+1)*(ViewWidth+Square+Square)] == ROUND_WOODEN_TABLE) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i+1+(j+1)*(ViewWidth+Square+Square)] == ROUND_WOODEN_TABLE) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i-1+(j+1)*(ViewWidth+Square+Square)] == ROUND_WOODEN_TABLE) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i+(j+2)*(ViewWidth+Square+Square)] == ROUND_WOODEN_TABLE) {
   		*Y = -3;
		   *X = 0;
		   *C = 5;
	   } else if (CompiledView[i+j*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SE) {
   		*Y = -3;
		   *X = 0;
		   *C = 3;
	   } else if (CompiledView[i+(j+1)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SE) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i-1+(j+1)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SE) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i+1+(j+1)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SE) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i+(j+2)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SE) {
   		*Y = -3;
		   *X = 0;
		   *C = 5;
	   } else if (CompiledView[i+1+(j+2)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SE) {
   		*Y = -3;
		   *X = 0;
		   *C = 5;
	   } else if (CompiledView[i+2+(j+2)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SE) {
   		*Y = -3;
   		*X = 0;
		   *C = 5;
	   } else if (CompiledView[i+1+(j+3)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SE) {
   		*Y = -3;
		   *X = 0;
		   *C = 6;
	   } else if (CompiledView[i+j*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SW) {
   		*Y = -3;
	   	*X = 0;
		   *C = 3;
	   } else if (CompiledView[i+(j+1)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SW) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i-1+(j+1)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SW) {
		   *Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i+1+(j+1)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SW) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i+(j+2)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SW) {
   		*Y = -3;
		   *X = 0;
		   *C = 5;
	   } else if (CompiledView[i-1+(j+2)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SW) {
   		*Y = -3;
		   *X = 0;
		   *C = 5;
	   } else if (CompiledView[i-2+(j+2)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SW) {
   		*Y = -3;
		   *X = 0;
		   *C = 5;
	   } else if (CompiledView[i-1+(j+3)*(ViewWidth+Square+Square)] == LARGE_WOODEN_TABLE_SW) {
   		*Y = -3;
		   *X = 0;
		   *C = 6;
	   } else if (CompiledView[i+j*(ViewWidth+Square+Square)] == SQUARE_WOODEN_TABLE) {
   		*Y = -3;
		   *X = 0;
		   *C = 3;
	   } else if (CompiledView[i+(j+1)*(ViewWidth+Square+Square)] == SQUARE_WOODEN_TABLE) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i+1+(j+1)*(ViewWidth+Square+Square)] == SQUARE_WOODEN_TABLE) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
   	} else if (CompiledView[i-1+(j+1)*(ViewWidth+Square+Square)] == SQUARE_WOODEN_TABLE) {
		   *Y = -3;
		   *X = 0;
   		*C = 4;
   	} else if (CompiledView[i+(j+2)*(ViewWidth+Square+Square)] == SQUARE_WOODEN_TABLE) {
		   *Y = -3;
		   *X = 0;
		   *C = 5;
	   } else if (CompiledView[i+j*(ViewWidth+Square+Square)] == LITTLE_ROUND_WOODEN_TABLE) {
   		*Y = -3;
		   *X = 0;
		   *C = 3;
	   } else if (CompiledView[i+j*(ViewWidth+Square+Square)] == LITTLE_WOODEN_TABLE_SE) {
   		*Y = -3;
		   *X = 0;
		   *C = 3;
	   } else if (CompiledView[i+1+j*(ViewWidth+Square+Square)] == LITTLE_WOODEN_TABLE_SE) {
   		*Y = -3;
		   *X = 0;
		   *C = 3;
	   } else if (CompiledView[i+1+(j+1)*(ViewWidth+Square+Square)] == LITTLE_WOODEN_TABLE_SE) {
   		*Y = -3;
		   *X = 0;
   		*C = 4;
   	} else if (CompiledView[i+2+(j+1)*(ViewWidth+Square+Square)] == LITTLE_WOODEN_TABLE_SE) {
		   *Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i+j*(ViewWidth+Square+Square)] == LITTLE_WOODEN_TABLE_SW) {
   		*Y = -3;
		   *X = 0;
		   *C = 3;
	   } else if (CompiledView[i-1+j*(ViewWidth+Square+Square)] == LITTLE_WOODEN_TABLE_SW) {
   		*Y = -3;
   		*X = 0;
		   *C = 3;
	   } else if (CompiledView[i-1+(j+1)*(ViewWidth+Square+Square)] == LITTLE_WOODEN_TABLE_SW) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   } else if (CompiledView[i-2+(j+1)*(ViewWidth+Square+Square)] == LITTLE_WOODEN_TABLE_SW) {
   		*Y = -3;
		   *X = 0;
		   *C = 4;
	   }
   }
}

void TileSet::ValidFogMoving()
{
   if(m_dwFogX[0] <= -1536)
   {
      m_dwFogX[0] = 0;
      m_dwFogY[0] = 768;
      m_fFoyY [0] = (float)m_dwFogY[0];
   }
   if(m_dwFogX[1] <= -1024)
   {
      m_dwFogX[1] = 512;
      m_dwFogY[1] = 768;
      m_fFoyY [1] = (float)m_dwFogY[1];
   }
   if(m_dwFogX[2] <= -512)
   {
      m_dwFogX[2] = 1024;
      m_dwFogY[2] = 768;
      m_fFoyY [2] = (float)m_dwFogY[2];
   }
   if(m_dwFogY[3] <= -384)
   {
      m_dwFogX[3] = 1536;
      m_dwFogY[3] = 768;
      m_fFoyY [3] = (float)m_dwFogY[3];
      m_dwFogX[4] = 2048;
      m_dwFogY[4] = 768;
      m_fFoyY [4] = (float)m_dwFogY[4];
   }
   
   if(m_dwFogX[5] <= -1536)
   {
      m_dwFogX[5] = 0;
      m_dwFogY[5] = 768;
      m_fFoyY [5] = (float)m_dwFogY[5];
   }
   if(m_dwFogX[6] <= -1024)
   {
      m_dwFogX[6] = 512;
      m_dwFogY[6] = 768;
      m_fFoyY [6] = (float)m_dwFogY[6];
   }
   if(m_dwFogX[7] <= -512)
   {
      m_dwFogX[7] = 1024;
      m_dwFogY[7] = 768;
      m_fFoyY [7] = (float)m_dwFogY[7];
   }
   if(m_dwFogY[8] <= -384)
   {
      m_dwFogX[8] = 1536;
      m_dwFogY[8] = 768;
      m_fFoyY [8] = (float)m_dwFogY[8];
      m_dwFogX[9] = 2048;
      m_dwFogY[9] = 768;
      m_fFoyY [9] = (float)m_dwFogY[9];
   }
   
   if(m_dwFogX[10] <= -1536)
   {
      m_dwFogX[10] = 0;
      m_dwFogY[10] = 768;
      m_fFoyY [10] = (float)m_dwFogY[10];
   }
   if(m_dwFogX[11] <= -1024)
   {
      m_dwFogX[11] = 512;
      m_dwFogY[11] = 768;
      m_fFoyY [11] = (float)m_dwFogY[11];
   }
   if(m_dwFogX[12] <= -512)
   {
      m_dwFogX[12] = 1024;
      m_dwFogY[12] = 768;
      m_fFoyY [12] = (float)m_dwFogY[12];
   }
   if(m_dwFogY[13] <= -384)
   {
      m_dwFogX[13] = 1536;
      m_dwFogY[13] = 768;
      m_fFoyY [13] = (float)m_dwFogY[13];
      m_dwFogX[14] = 2048;
      m_dwFogY[14] = 768;
      m_fFoyY [14] = (float)m_dwFogY[14];
   }
}