// ************************************************************************
// ***                                                                  ***
//      File Name: GameMusic.h
//      Project:   The 4th Coming
//      Creation:  25 Mars, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***

#ifndef GAMEMUSIC_H
#define GAMEMUSIC_H

// Include File
#include "NewSound.h"

class GameMusic {
// Class that manage the Music of T4C.
	private:	
		T3VSBSound			t3Music;				// The Music Currently Playing.
		CRITICAL_SECTION 	csLock;				// The Lock.
		DWORD             dwOldMusicVolume;	// The Old Music Volume.
		DWORD					dwMusicNumber;		// The Music Number current playing.
		DWORD					dwOldMusicNumber; // The Old Music Number playing.

	public:
		GameMusic(void);		// Constuctor.
		~GameMusic(void);		// Destructor.
		
		void SetNewVolume(void); // Set a New music volume.
		void Play(void);			 // Play the Music.
		void Stop(void);			 // Stop the Music.
		void Start(void);			 // Start the Music.
		void LoadNewSound(BOOL = TRUE); // Load a new sound for the new area.
		void Reset(void);			 // Reset the Music.
}; 

// Global Var
extern GameMusic g_GameMusic;

#endif