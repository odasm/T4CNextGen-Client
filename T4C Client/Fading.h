// ************************************************************************
// ***                                                                  ***
//      File Name: Fading.h
//      Project:   The 4th Coming
//      Creation:  25 Mars, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***

#ifndef FADING_H
#define FADING_H

// Include
#include "cs.h"

class PalManagement {
// Class that take care of the Fade Out.

	private:
		CriticalSection csLock;				// Fading Lock
		bool boFadeOut;						// In Fade out ?
		LPPALETTEENTRY lpPalCurrent;		// Current Palette
		LPPALETTEENTRY lpPalVisible;		// VisiblePalette

	public:
		PalManagement(void);		// Constructor
		~PalManagement(void);	// Destructor

		void FadeToBlack(void);								// Fade the Screen to Total Black.
		void SetToBlack(void);								// Set the Screen to Total Black.
		bool inFadeOut(void);								// Return TRUE if the process is making a fade out.
		void SetCurrentPal(LPPALETTEENTRY);				// Set the Current Pal
		void SetVisiblePal(LPPALETTEENTRY = NULL);	// Set the Visible Pal
		LPPALETTEENTRY GetCurrentPal(void);				// Get the Current Pal
		LPPALETTEENTRY GetVisiblePal(void);				// Get the Visible Pal
};

extern PalManagement g_Pal;

#endif