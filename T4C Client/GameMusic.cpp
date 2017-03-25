// ************************************************************************
// ***                                                                  ***
//      File Name: GameMusic.Cpp
//      Project:   The 4th Coming
//      Creation:  25 Mars, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// *** Copyright (c) 1997-1999 Vircom. All rights reserved.             ***

// Include File
#include "pch.h"
#include <windows.h>
#include "GameMusic.h"
#include "TFCPlayer.h"
#include "Global.h"
#include "MemoryManager.h"
#include "App.h"
#include "SaveGame.h"
extern CSaveGame g_SaveGame;

#define Track45(a,b,c,d) ((Y >= -X + a) && (Y >= X + b) && (Y <= -X + c) && (Y <= X + d))
#define Track90(a,b,c,d) ((X >= a && X <= b) && (Y >= c && Y <= d))

// Gloval Var
GameMusic g_GameMusic;

// const
static const int MAIN_WORLD			= 0;
static const int DUNGEON_WORLD		= 1;
static const int CAVERN_WORLD		= 2;
static const int EDITOR_WORLD     = 3;
static const int LEO_WORLD			= 4;

static const int BOSS_MUSIC			= 0;
static const int OUTDOORS_MUSIC	= 1;
static const int FOREST_MUSIC		= 2;
static const int DUNGEON_MUSIC		= 3;
static const int CAVERN_MUSIC		= 4;
static const int SADNESS_MUSIC		= 5;
static const int SILENCE_MUSIC		= 6;
static const int NOISES_MUSIC		= 7;

GameMusic::GameMusic(void) {
// Constructor
	dwOldMusicVolume = INFINITE;	
	dwMusicNumber	  = INFINITE;
	dwOldMusicNumber = -1;
	InitializeCriticalSection(&csLock);
}

GameMusic::~GameMusic(void) {
// Destructor
   Stop();
	DeleteCriticalSection(&csLock);
}

void GameMusic::SetNewVolume(void) {
// Set a New Music Volume
	// Lock
	EnterCriticalSection(&csLock);

	// Set the new music volume
	t3Music.SetVolume(g_SaveGame.GetOptionsParam()->dwMusicVol);
	if (g_SaveGame.GetOptionsParam()->dwMusicVol && dwOldMusicVolume == 0 && !g_SaveGame.GetOptionsParam()->bUseCD) 
   {
	// If the music was stop and need to be restarted.
		t3Music.Play(TRUE);
	}

	// Set the Old Music Volume.
	dwOldMusicVolume = g_SaveGame.GetOptionsParam()->dwMusicVol;

	// Unlock
	LeaveCriticalSection(&csLock);
}

void GameMusic::Stop(void) {
// Stop the Music.
  /* if (g_SaveGame.GetOptionsParam()->bUseCD) {
      StopCDAudio();
   } else {*/
	   t3Music.Stop(FALSE);
  // }
}

void GameMusic::Reset(void) {
// Reset the Music.
	if (!g_SaveGame.GetOptionsParam()->bUseCD)
      t3Music.Stop(FALSE);
	dwOldMusicNumber = INFINITE;
}

void GameMusic::Play(void) {
   if (g_SaveGame.GetOptionsParam()->dwMusicVol) {
      t3Music.SetVolume(g_SaveGame.GetOptionsParam()->dwMusicVol);
      t3Music.Play(TRUE);
   }
}

void GameMusic::Start(void) {
// Start the Music.
	// Lock
	EnterCriticalSection(&csLock);

	if (!g_SaveGame.GetOptionsParam()->bUseCD) {
	// Is there some volume AND we are not playing CD music.
		// Stop the Old Music
		t3Music.Stop(FALSE);
		// Release the Memory of the Old Sound.
		t3Music.Release(TRUE);
		// Load the New Sound.
		t3Music.Create("Sadness Music", TS_STREAMING);
      //dwOldMusicVolume = g_SaveGame.GetOptionsParam()->dwMusicVol;
      Play();
	}

	// Unlock
	LeaveCriticalSection(&csLock);
}

void GameMusic::LoadNewSound(BOOL bPlay) {
// Load a new sound for the new area.
	
	// Lock
	EnterCriticalSection(&csLock);

	g_EventHelp.Lock();
	if (Player.Level <= 1) {
		g_EventHelp.OutSide = 1;
	} else {
		g_EventHelp.OutSide = 0;
	}
				
	// Temp Var (X & Y are more readable than Player.xPos & Player.yPos)
	int X = Player.xPos;
	int Y = Player.yPos;

	switch (Player.World) {
	// Set a Default Music number depending on the World.
		case MAIN_WORLD:		dwMusicNumber = FOREST_MUSIC;		break;
		case DUNGEON_WORLD:	if (Player.Level <= 1) { g_EventHelp.OutSide = 2; }; dwMusicNumber = DUNGEON_MUSIC;	break;
		case CAVERN_WORLD:	dwMusicNumber = CAVERN_MUSIC;		break;
		case LEO_WORLD:	dwMusicNumber = FOREST_MUSIC;	break;
	};

   if (Player.World == LEO_WORLD) 
   {
      if ( Track90( 2700, 3071, 0, 800 ) ) 
      {
         dwMusicNumber = OUTDOORS_MUSIC;
      }
      if ( Track90( 2600, 3071, 800 , 1700 ) ) 
      {
         dwMusicNumber = CAVERN_MUSIC;
      }

   }

   if (Player.World == EDITOR_WORLD) {
      dwMusicNumber = FOREST_MUSIC;

      if ( Track90( 0, 1536, 1535, 3071 ) ) {
         dwMusicNumber = DUNGEON_MUSIC;
      }

      if ( Track90( 1536, 1536, 3071, 3071 ) ) {
         dwMusicNumber = CAVERN_MUSIC;
      }
   }

   if (Player.World == DUNGEON_WORLD) {
		// Balork and Jarko dungoen
		if (((X >= 135 && X <= 285) && (Y >= 410 && Y <= 560)) || 
			 ((X >= 960 && X <= 1185) && (Y >= 75  && Y <= 300))) {
			dwMusicNumber = BOSS_MUSIC;
		}
   } 

	if (Player.World == MAIN_WORLD) {
      // StoneHeim Music
      // Town Souds
		if ( Track45(823,407,1135,723) ||
           Track45(1136,406,1208,664) ||
           Track45(568,248,712,312) ||
           Track90(993,1023,945,951) || 
           Track90(952,1029,952,957) || 
           Track90(939,1034,958,963) || 
           Track90(904,1040,964,969) || 
           Track90(885,1063,970,978) || 
           Track90(873,1070,979,987) || 
           Track90(865,1093,988,1001) || 
           Track90(853,1240,1002,1087) || 
           Track90(844,1255,1088,1320)) {
			dwMusicNumber = OUTDOORS_MUSIC;
		}

      // Boss Sounds
      if ( Track45(555,-115,625,-45) ) {
			dwMusicNumber = BOSS_MUSIC;
      }

      // Sadness Souds
      if ( Track45(1571,807,1699,933) ) {
			dwMusicNumber = SADNESS_MUSIC;
      }

      // Cavern Souds
		if ( Track45(879,-777,1017,-547) ||
           Track45(1018,-744,1072,-602) ||
           Track45(1720,-624,1818,-612) ||
           Track45(1437,-695,1883,-625) ||
           Track45(1460,-732,1890,-696) ||
           Track45(1521,-761,1891,-733) ||
           Track45(1546,-802,1908,-762) ||
           Track45(1575,-833,1915,-803) ) {
			dwMusicNumber = CAVERN_MUSIC;
		}

      // Forest Noises
      if ( Track45(1222,-134,1394,36) ||
           Track90(2552,2975,1874,2138)) {
			dwMusicNumber = NOISES_MUSIC;
      }

      // Dungeon Sounds
      if ( Track90(1622,1814,1745,1938) ||
           Track90(2696,2847,2193,2607) ) {
         dwMusicNumber = DUNGEON_MUSIC;
      }

      // End Stoneheim
 
      // Town 1
		if (((X >= 2928 && X <= 3022) && (Y >= 908 && Y <= 966)) ||
			 ((X >= 2896 && X <= 3022) && (Y >= 967 && Y <= 1020)) ||
			 ((X >= 2785 && X <= 3022) && (Y >= 1021 && Y <= 1026)) ||
			 ((X >= 2779 && X <= 3022) && (Y >= 1027 && Y <= 1030)) ||
			 ((X >= 2761 && X <= 3022) && (Y >= 1031 && Y <= 1046)) ||
			 ((X >= 2744 && X <= 3022) && (Y >= 1047 && Y <= 1060)) ||
			 ((X >= 2730 && X <= 3022) && (Y >= 1061 && Y <= 1075)) ||
			 ((X >= 2725 && X <= 3022) && (Y >= 1076 && Y <= 1275))) {
			dwMusicNumber = OUTDOORS_MUSIC;
			g_EventHelp.OutSide = 0;
		}

		// Town 2
		if (((Y >= -X + 2767) && (Y >= X - 567) && (Y <= -X + 2947) && (Y <= X - 317)) || 
			 ((Y >= -X + 2948) && (Y >= X - 566) && (Y <= -X + 3112) && (Y <= X - 302))) {
			dwMusicNumber = OUTDOORS_MUSIC;
		}

	
		// Simetiere Town 1
		if ((Y >= -X + 3848) && (Y >= X - 1722) && (Y <= -X + 3902) && (Y <= X - 1662)) {
			dwMusicNumber = CAVERN_MUSIC;
		}
				
		// Silence Simetiere Town 1
		if ((Y >= -X + 3903) && (Y >= X - 1701) && (Y <= -X + 3907) && (Y <= X - 1687)) {
			dwMusicNumber = CAVERN_MUSIC;
		}

      // Silence RD Zone 1
		//if ((Y >= -X + 2100) && (Y >= X + 1336) && (Y <= -X + 2106) && (Y <= X + 1354)) {
		//	dwMusicNumber = SILENCE_MUSIC;
		//}

      // Silence RD Zone 2
		//if ((Y >= -X + 4023) && (Y >= X + 1105) && (Y <= -X + 4035) && (Y <= X + 1111)) {
		//	dwMusicNumber = SILENCE_MUSIC;
		//}

      // Silence RD Zone 3
		//if ((Y >= -X + 3785) && (Y >= X +  795) && (Y <= -X + 3791) && (Y <= X +  821)) {
		//	dwMusicNumber = SILENCE_MUSIC;
		//}

      // Silence RD Zone 4
		//if ((Y >= -X + 4036) && (Y >= X + 1106) && (Y <= -X + 4066) && (Y <= X + 1110)) {
		//	dwMusicNumber = SILENCE_MUSIC;
		//} 

      // Silence RD Zone 5
		//if ((Y >= -X + 2560) && (Y >= X + 1716) && (Y <= -X + 2580) && (Y <= X + 1722)) {
		//	dwMusicNumber = SILENCE_MUSIC;
		//}

		// Outdoor RD Zone 1
      if ((Y >= -X + 3673) && (Y >= X +  841) && (Y <= -X + 3791) && (Y <= X + 1103)) {
			dwMusicNumber = OUTDOORS_MUSIC;
		}

		// Outdoor RD Zone 2
      if ((Y >= -X + 3792) && (Y >= X +  778) && (Y <= -X + 3858) && (Y <= X + 1104)) {
			dwMusicNumber = OUTDOORS_MUSIC;
		}

		// Outdoor RD Zone 3
      if ((Y >= -X + 3859) && (Y >= X +  759) && (Y <= -X + 3951) && (Y <= X + 1103)) {
			dwMusicNumber = OUTDOORS_MUSIC;
		}

		// Outdoor RD Zone 4
      if ((Y >= -X + 3952) && (Y >= X +  796) && (Y <= -X + 4036) && (Y <= X + 1104)) {
			dwMusicNumber = OUTDOORS_MUSIC;
		}

		// Outdoor RD Zone 5
      if ((Y >= -X + 4037) && (Y >= X +  821) && (Y <= -X + 4311) && (Y <= X + 1105)) {
			dwMusicNumber = OUTDOORS_MUSIC;
		}

		// Sadness RD Zone 1
      if ((Y >= -X + 1950) && (Y >= X + 1410) && (Y <= -X + 1970) && (Y <= X + 1430)) {
			dwMusicNumber = SADNESS_MUSIC;
		}

		// Sadness RD Zone 2
      if ((Y >= -X + 1950) && (Y >= X + 1260) && (Y <= -X + 1970) && (Y <= X + 1280)) {
			dwMusicNumber = SADNESS_MUSIC;
		}

		// Sadness RD Zone 3
      if ((Y >= -X + 2090) && (Y >= X + 1260) && (Y <= -X + 2110) && (Y <= X + 1280)) {
			dwMusicNumber = SADNESS_MUSIC;
		}

		// Sadness RD Zone 4
      if ((Y >= -X + 2090) && (Y >= X + 1410) && (Y <= -X + 2110) && (Y <= X + 1430)) {
			dwMusicNumber = SADNESS_MUSIC;
		}

		// Sadness RD Zone 5
      if ((Y >= -X + 1961) && (Y >= X + 1281) && (Y <= -X + 2099) && (Y <= X + 1409)) {
			dwMusicNumber = SADNESS_MUSIC;
		}

		// RD Tower 1
		if ((Y >= -X + 2535) && (Y >= X + 1645) && (Y <= -X + 2605) && (Y <= X + 1715)) {
			dwMusicNumber = BOSS_MUSIC;
		}

   }

	if (dwMusicNumber != dwOldMusicNumber) {
	// If the new Music Number is not the same as the Old Music Number (made a continuous play) AND
	// There is some volume on on CD music playing.
		// Set the Old Music Number.
		dwOldMusicNumber = dwMusicNumber;
		/*if (g_SaveGame.GetOptionsParam()->bUseCD) 
      {
		// CD Music Playing
			switch (dwMusicNumber) 
         {
			// Play the appropriate Music
				case BOSS_MUSIC:		PlayCDAudio(2, TRUE); break;
				case OUTDOORS_MUSIC: PlayCDAudio(3, TRUE); break;
				case FOREST_MUSIC:	PlayCDAudio(4, TRUE); break;
				case DUNGEON_MUSIC:	PlayCDAudio(5, TRUE); break;
				case CAVERN_MUSIC:	PlayCDAudio(6, TRUE); break;
				case SADNESS_MUSIC:  PlayCDAudio(7, TRUE); break;
            case NOISES_MUSIC:   PlayCDAudio(8, TRUE); break;
			}
		} 
      else 
      {*/
		// HD Music Playing
			if (dwMusicNumber != SILENCE_MUSIC) 
         {
			// There is music ?
				// Stop the Music and release the Memory.
				t3Music.Stop(FALSE);
				t3Music.Release(TRUE);

				switch (dwMusicNumber) {
				// Load the New Music
					case BOSS_MUSIC:		t3Music.Create("Boss Music", TS_STREAMING);			break;
					case OUTDOORS_MUSIC: t3Music.Create("Outdoors Music", TS_STREAMING);		break;
					case FOREST_MUSIC:	t3Music.Create("Forest Music", TS_STREAMING);		break;
					case DUNGEON_MUSIC:	t3Music.Create("Dungeons Music", TS_STREAMING);		break;
					case CAVERN_MUSIC:	t3Music.Create("Caverns Music", TS_STREAMING);		break;
					case SADNESS_MUSIC:  t3Music.Create("Sadness Music", TS_STREAMING);		break;
               case NOISES_MUSIC:   t3Music.Create("Noises Music", TS_STREAMING);      break;
				}
				
				// Set the Music Volume and Start playing.
            if (bPlay) 
               Play();
			} else {
			// There is no music so stop the old music.
				t3Music.Stop(TRUE);
			}
	//	}
	}

	g_EventHelp.Unlock();
	// Unlock
	LeaveCriticalSection(&csLock);
}