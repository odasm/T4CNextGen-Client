// ************************************************************************
// ***                                                                  ***
//      File Name: App.h
//      Project:   The 4th Coming
//      Creation:  25 Mars, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***

#ifndef APP_H
#define APP_H


#define WINDOWED 0

#include "V2Sprite.h"
#include "Lock.h"

// Const
const int APP_MOUSE_THREAD		  = 1;
const int APP_CD_THREAD			  = 2;
const int APP_DRAWING_THREAD	  = 3;
const int APP_MAINTENANCE_THREAD = 4;
const int APP_SOUND_THREAD		  = 5;
const int APP_VOLUME_THREAD		  = 6;

class AppManagement {
// Class that manage the exit of T4C.

	private:
		DWORD dwErrNb;			// Code d'erreur lors de sortie
		char  *lpszErrStr;	// String d'erreur lors de sortie
		bool  boFocus;			// Is the App on Focus ?

		bool SMT_Exit;			// Flag for the Sound Management Thread
		bool SCT_Exit;			// Flag for the Sound Control Thread
		bool DT_Exit;			// Flag for the Drawing Thread
		bool MT_Exit;			// Flag for the Maintenance Thread
		bool MS_Exit;			// Flag for the Mouse Thread
		bool CD_Exit;			// Flag for the CD Thread

        static UINT CALLBACK AsyncClose( LPVOID theApp );

	public:
        void LockDrawThread();
        void UnlockDrawThread();
        void LockMainThread();
        void UnlockMainThread();
        void LockMouseThread();
        void UnlockMouseThread();
        void LockPacketThread();
        void UnlockPacketThread();

        void SetLoading( bool loadValue );
        bool IsLoading();

		AppManagement(void);		  // Constructor
		~AppManagement(void);	  // Destructor

		void Close(char *);						// Close App.
		void SetError(DWORD, char *);		// Set Exit Error.
		void ExitThread(DWORD);				// Set Exit Thread.
		void StartThread(DWORD);			// Set Starting Thread.
		bool isFocus(void);					// is the App on Focus ?
		void SetFocus(bool);					// Set the App Focus.
};

class AppVariable {
// Class that hold all App State Variable
   public:
      AppVariable(void);
      
      DWORD tWaitFPSTime;
      bool minimizeState;
      bool inGame;
      bool InvalideSaveGame;
      bool bMemory;
      WORD wDisplayDisk;
      WORD wDisplayMap;
      WORD wDisplayMapZone;
      char strMapZoneName[30];
      CV2Sprite  vsfDisk;
      CV2Sprite  vsfNetwork;
      CV2Sprite  vsfMap;
      CV2Sprite  vsfMapZone;
      WORD wProgressCnt;
      int  wProgressInc;
      CV2Sprite  vsfProgressB;
      CV2Sprite  vsfProgressT;
      CV2Sprite  vsfCode[5];
};

class EVENTHELP : public CLock {
public:
	EVENTHELP(void) {
		Nacked = false;
		Light = false;
		noWeapon = false;
		Attack = false;
		HP = false;
		BuyPotion = false;
		NoMoney = false;
		OutSide = 0;
		Stats = false;
		Skills = false;
		BuyTorch = false;
		OnOff = true;
		neverOpenMap = true;
		neverOpenStat = true;
	}

	bool Nacked;
	bool Light;
	bool noWeapon;
	bool Attack;
	bool HP;
	bool BuyPotion;
	bool NoMoney;
	int OutSide;
	bool Stats;
	bool Skills;
	bool OnOff;
	bool BuyTorch;
	bool neverOpenMap;
	bool neverOpenStat;
};

// Global Var
extern AppManagement g_App;
extern AppVariable   g_Var;
extern bool          g_boQuitApp;		// Si on est en train de sortir du jeux
extern EVENTHELP	 g_EventHelp;

#endif // APP_H