// ************************************************************************
// ***                                                                  ***
//      File Name: Fading.cpp
//      Project:   The 4th Coming
//      Creation:  25 Mars, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***

// Include
#include "pch.h"
#include <windows.h>
#include "Fading.h"
#include "IDirectXDraw.h"
#include "TileSet.h"
#include "Global.h"
#include "MemoryManager.h"


PalManagement g_Pal;

//*********************************************************************************
PalManagement::PalManagement( void )
/**********************************************************************************
 * Constructor
 */
{
// Constructor.
   //lpPalCurrent = new PALETTEENTRY [DEFAULT_COLORS];
	//lpPalVisible = new PALETTEENTRY [DEFAULT_COLORS];
}

//*********************************************************************************
PalManagement::~PalManagement( void )
/**********************************************************************************
 * Destructor
 */
{
// Destructor
/*)   if (lpPalCurrent) {
	// Current pal is not deleted ?
      // Then release Current Pal
		delete [] lpPalCurrent;
      lpPalCurrent = NULL;
   }

   if (lpPalVisible) {
	// Visible Pal is not deleted ?
      // Then release Visible Pal
		delete [] lpPalVisible;
      lpPalVisible = NULL;
   }
*/
}

//*********************************************************************************
void PalManagement::FadeToBlack( void )
/**********************************************************************************
 * Fade the current screen to black.
 */
{
// Fade the Screen to black
   
   // Lock
	csLock.Lock("FadeLock 1");
	
   // Start our Fade Out.
	boFadeOut = true;
   World.SetFading = World.RealFading = false;

   // Create Temp Pal.
	//LPPALETTEENTRY TempPal;
	//TempPal = new PALETTEENTRY [DEFAULT_COLORS];

   // Set the Pal in Fade Out.
	//for (int j = 100; j > -1; j-=9) {
	//	DXDFadeEntry(TempPal, lpPalVisible, j, j, j);
	//	DXDSetPalette(TempPal);
	//   Sleep(1000/16);
   //}
		      
   // Delete Temp Pal.
	//delete TempPal;

   // Unlock
	csLock.Unlock("FadeLock 1");
   
	// End our FadeOut.
	boFadeOut = false;

}

//*********************************************************************************
void PalManagement::SetToBlack( void )
/**********************************************************************************
 * Set the current pallette to black.
 */
{
// Set the Pal to black
   
	// Lock
	csLock.Lock("FadeLock 1");

	// Start the fade out (one shot fade out).
   boFadeOut = true;
   World.SetFading = World.RealFading = false;

   // Create a Temp Pal.
	//LPPALETTEENTRY TempPal;
	//TempPal = new PALETTEENTRY [DEFAULT_COLORS];

	// Set the Pal to Black.
	//DXDFadeEntry(TempPal, lpPalVisible, 0, 0, 0);
	//DXDSetPalette(TempPal);
		      
   // Delete the Temp Pal.
	//delete TempPal;

   // Unlock
	csLock.Unlock("FadeLock 1");
   
	// End of the one shot fade out.
	boFadeOut = false;
}

//*********************************************************************************
bool PalManagement::inFadeOut( void )
/**********************************************************************************
 * Are we currently making a fade out ?
 * 
 * @RETURN true, we are in fade out, false, we are not in fade out
 */
{
// Is the process in Fade Out ?
	return boFadeOut;
}

//*********************************************************************************
void PalManagement::SetCurrentPal
/**********************************************************************************
 * Set the Current pallette.
 */
(
 LPPALETTEENTRY lpPal           // a user-defined pallette to set as the current pallette
)
//*********************************************************************************
{
// Set the Current Pal.
	//memcpy(lpPalCurrent, lpPal, sizeof(PALETTEENTRY) << DEFAULT_BPP);
}

//*********************************************************************************
LPPALETTEENTRY PalManagement::GetCurrentPal( void )
/**********************************************************************************
 * Get the Current palette.
 */
{
// Get the Current Pal.
   return NULL;
	//return lpPalCurrent;
}

//*********************************************************************************
LPPALETTEENTRY PalManagement::GetVisiblePal( void )
/**********************************************************************************
 * Get the Visible palette.
 * 
 * @REMARK The visible palette is the current palette + all the modificiation. If
 * a color in the current pallette is RGB 100,100,100 then it may be to RGB
 * 50,50,50 in the visible palette due to fade out (or other) modification.
 */
{
// Get the Visible Pal.
   return NULL;
	//return lpPalVisible;
}


//*********************************************************************************
void PalManagement::SetVisiblePal
/**********************************************************************************
 * Set the visible pallette
 */
(
 LPPALETTEENTRY lpPal           // a user-defined palette to be set as the visible pallette
)
//*********************************************************************************
{
// Set the Visible Pal.
	//if (lpPal)
	// Is a Pal specified ?
		//memcpy(lpPalVisible, lpPal, sizeof(PALETTEENTRY) << DEFAULT_BPP);
	//else
	// No ? then Set the Visible pal.
		//DXDSetPalette(lpPalVisible);
}