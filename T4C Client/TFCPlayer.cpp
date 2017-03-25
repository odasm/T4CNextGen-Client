// TFCPlayer.cpp: implementation of the TFCPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <windows.h>
#include "TFCPlayer.h"
#include "Global.h"
#include "Packet.h"
#include <stdio.h>
#include "MemoryManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include "VisualObjectList.h"

TFCPlayer Player;

TFCPlayer::TFCPlayer()
{
    CanRunScripts = false;
    CanSlayUsers = false;
	LastAttack = 0;
   LastObject = 0;
	ViewObjectFlag = 0;
   Light = 0;
	AttackMode = 0;
	SetGridMode = 0;
	Dead = false;
   rangedAttack = false;
   Level = 10000;
   isSeraph = false;

	// by Asteryth for Pvp Ranking
	TotalDeathNumber = 0;
	TotalKillNumber = 0;
	CurrentDeathNumber = 0;
	CurrentKillNumber = 0;
	BestSerieMurder = 0;
	CurrentSerieMurder = 0;
	CurrentPvpPoint = 0;

//	Hp = MaxHp = Mana = MaxMana = 0;
	
	for( int i = 0; i < 13; i++ ){
		lpbEquipped[ i ] = NULL;
	}

	InitializeCriticalSection( &csThreadLock );
}

TFCPlayer::~TFCPlayer()
{
	tlBackpack.ToHead();
   while (tlBackpack.QueryNext())
      tlBackpack.DeleteObject();

   tlUserSkill.ToHead();
   while (tlUserSkill.QueryNext()) {
      LPUSER_SKILL s = tlUserSkill.GetObject();
      delete s->bName;
      tlUserSkill.DeleteObject();
   }

   for (int i = 0; i < 16; i++)
      if (Player.lpbEquipped[i])
         delete Player.lpbEquipped[i];

   DeleteCriticalSection( &csThreadLock );
}

void TFCPlayer::Lock( void ){
	EnterCriticalSection( &csThreadLock );
}

void TFCPlayer::Unlock( void ){
	LeaveCriticalSection( &csThreadLock );
}


//////////////////////////////////////////////////////////////////////////////////////////
void TFCPlayer::ScreenPosToWL
//////////////////////////////////////////////////////////////////////////////////////////
// This function converts pixel screen coordinates into world coordinates.
// 
(
 LPPOINT lppScreenPos,	// Pointer to screen position
 LPPOINT lppWorldPos	// Pointer to world position
)
//////////////////////////////////////////////////////////////////////////////////////////
{
   lppWorldPos->x = ((lppScreenPos->x-(g_Global.GetScreenWDiv2()-16))/32) + xPos + ((lppScreenPos->x < (g_Global.GetScreenWDiv2()-16)) ? -1 : 0);
	lppWorldPos->y = ((lppScreenPos->y-(g_Global.GetScreenHDiv2()+24))/16) + yPos + ((lppScreenPos->y < (g_Global.GetScreenHDiv2()+24)) ? -1 : 0);
}
