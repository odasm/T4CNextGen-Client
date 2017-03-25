#ifndef MOUSEDEVICE_H
#define MOUSEDEVICE_H

#include "dinput.h"
#include "V2Sprite.h"

UINT WINAPI QueryMouseEventThread(LPVOID pParam);

#define DM_NONE          0
#define DM_DOWN          1
#define DM_CLICK         2
#define DM_DOUBLE_CLICK  3
#define DM_UP            4
#define DM_DRAG          5
#define DM_DROP          6
#define DM_WAIT          7

class MouseDevice{
	private:
		static UINT QueryMouseEventID;
		static int DeviceCount;
      static LPVOID LockCustomPtr[4];
      static UINT (* LockEventCallBack[4])(LPVOID);
      int Check;
	
	public:
      static DWORD LockEvent[4];
		static int xPos;
		static int yPos;
		static UINT Buttons[4];
		static UINT ButtonsState[4];
		static DWORD Click[4];
		static int xClick[4];
		static int yClick[4];
		static DWORD ButtonDown[4];

		MouseDevice(void);
		~MouseDevice(void);
	
		void GetPosition(int *, int *);
		void GetStatus(int *, int *, int *);
      bool LockNextEvent(DWORD, LPVOID, UINT (* CallBack)(LPVOID), int);
      void GetDrag(int *, int *, int Button);
      void SetPosition(int, int);
};

class CMouseCursor {
   private:
      CV2Sprite *vsfCursor;
      bool correction;
      int iXCor;
      int iYCor;
      CRITICAL_SECTION csInternal;

   public:
      CMouseCursor(void);
      ~CMouseCursor(void);
      void SetCustomCursor(CV2Sprite * vsf = NULL, int x = 0, int y = 0, bool correction = false );
      void DrawCursor(int x, int y);
      static CMouseCursor *GetInstance(void);
};

#endif
