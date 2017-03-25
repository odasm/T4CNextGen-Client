#ifndef __Vircom_Handler_061743111897
#define __Vircom_Handler_061743111897

#include "TemplateList.h"
#include "VKey.h"

using namespace vir;

//*******************************************************************************************//
struct HandlerDesc
//*******************************************************************************************//
// Structure of
//
{
	public:
        VKey vKey;

		UINT (* CallBack)(LPVOID);

		LPVOID lpParam;
};

//*******************************************************************************************//
class MacroHandler : public TemplateList <HandlerDesc>
//*******************************************************************************************//
// Handle all of the User-Defined Macro
//
{
	public:
		MacroHandler ( void ); // Constructor.
		~MacroHandler( void ); // Destructor.

		BOOL CallMacro( VKey vKey );
		void AddNewMacro( VKey vKey, UINT (* CallBack)(LPVOID), LPVOID);
        void DeleteMacro( VKey vKey );
        BOOL MacroExists( VKey vKey  );
        void DisableMacroCall( void );
        void EnableMacroCall( void );

};

struct IgnoreName {
	char *Name;
};

struct UserCustom {
   unsigned int  gTextColor;
   BOOL          gWindowedSizing; // NightMare -- Add 2005-02-17 true if Windowed is sizing
   MacroHandler  gMacro;
   bool		     gfirstTimeAddOn;
   BOOL          gNoMusic;
   BOOL          gFirstConnect;

   int           g_dwNbrCCColor;
   int           *g_pNewCCColor;

   bool          bEnableGameFog;
   bool          bEnableGameRain;
   bool          bEnableGameSnow;
   BOOL          bEnableCArrow;

   BOOL          gWindowed;
   BOOL          gWebPatchEnabled;
   

   BOOL          NMShowPosition;
   BOOL          NMShowFPS;
   UINT          NMFPSValue;

   BOOL          NMDebug;
   BOOL          Debug;
   BOOL          TakeScreenShot;
   
   
   
   

};

extern UserCustom Custom;

#endif
