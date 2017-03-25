#include "pch.h"
#include <windows.h>
#include "pf.h"
#include "TFCPlayer.h"
#include "Mouse.h"
#include "PowerDebug.h"
#include "Global.h"
#include "Packet.h"
#include "MemoryManager.h"


int  pfSaveXPosition;
int  pfSaveYPosition;
int  pfLastPosX;
int  pfLastPosY;
int  pfLastDirection;
BOOL pfBoolStop;

extern char Force;
extern int NBCURSOR;

void pfNearPosition(void) {
   if (Player.xPos < pfSaveXPosition)
      pfSaveXPosition--;
   if (Player.xPos > pfSaveXPosition)
      pfSaveXPosition++;
   if (Player.yPos < pfSaveYPosition)
      pfSaveYPosition--;
   if (Player.yPos > pfSaveYPosition)
      pfSaveYPosition++;
}

void pfSetPosition(int x, int y) {
   pfSaveXPosition = x;
   pfSaveYPosition = y;
   pfBoolStop = FALSE;
   pfLastPosX = Player.xPos;
   pfLastPosY = Player.yPos;
   pfLastDirection = 0;
}

BOOL  pfIsAtPosition(void) {
   if ((pfSaveXPosition == Player.xPos && pfSaveYPosition == Player.yPos) || pfBoolStop) 
      return TRUE;
   
   return FALSE;
}

short pfGetNextMovement(void) {
   if (pfBoolStop) {
      return 0;
   }
 
   if      (pfLastDirection == 0) {}
   else if (pfLastDirection == 1 && (Player.xPos - pfLastPosX) == 0 && (Player.yPos - pfLastPosY) == -1)   {}
   else if (pfLastDirection == 2 && (Player.xPos - pfLastPosX) == 1 && (Player.yPos - pfLastPosY) == -1)   {}
   else if (pfLastDirection == 3 && (Player.xPos - pfLastPosX) == 1 && (Player.yPos - pfLastPosY) == 0)   {}
   else if (pfLastDirection == 4 && (Player.xPos - pfLastPosX) == 1 && (Player.yPos - pfLastPosY) == 1)  {}
   else if (pfLastDirection == 5 && (Player.xPos - pfLastPosX) == 0 && (Player.yPos - pfLastPosY) == 1)  {}
   else if (pfLastDirection == 6 && (Player.xPos - pfLastPosX) == -1 && (Player.yPos - pfLastPosY) == 1) {}
   else if (pfLastDirection == 7 && (Player.xPos - pfLastPosX) == -1 && (Player.yPos - pfLastPosY) == 0)  {}
   else if (pfLastDirection == 8 && (Player.xPos - pfLastPosX) == -1 && (Player.yPos - pfLastPosY) == -1)  {}
   else {
      return 0;
   }

   pfLastPosX = Player.xPos;
   pfLastPosY = Player.yPos;

   static MouseDevice Mouse2;
   
   int xDif = pfSaveXPosition - Player.xPos;
   int yDif = pfSaveYPosition - Player.yPos;
   static int xPos, yPos;
   
	Mouse2.GetPosition(&xPos, &yPos);
   
   int dwMouseDistance = g_Global.GetScreenWDiv2()*3;

   if (Force && Force != 9+NBCURSOR) {
		int a = xPos*3;
		int b = yPos*4;
		if (b > dwMouseDistance) {
			if (a > dwMouseDistance) {
				a -= dwMouseDistance;
				b -= dwMouseDistance;
				if (a > b*2)
					return 3;
				else if (b > a*2)
					return 5;
				else
				   return 4;
			} else {
				b -= dwMouseDistance;
				a = dwMouseDistance - a;
				if (a > b*2)
					return 7;
				else if (b > a*2)
				   return 5;
				else
				   return 6;
			}
		} else {
			if (a > dwMouseDistance) {
				a -= dwMouseDistance;
				b = dwMouseDistance - b;
				if (a > b*2)
               return 3;
				else if (b > a*2)
				   return 1;
				else
				   return 2;
			} else {
				a = dwMouseDistance - a;
				b = dwMouseDistance - b;
				if (a > b*2)
					return 7;
				else if (b > a*2)
				   return 1;
				else
				   return 8;
			}			
		}
   } else if (xDif < 0 && !yDif) {
		pfLastDirection = 7;
      return 7;
	} else if (xDif > 0 && !yDif) {
		pfLastDirection = 3;
		return 3;
	} else if (!xDif && yDif < 0) {
		pfLastDirection = 1;
		return 1;
	} else if (!xDif && yDif > 0) {
		pfLastDirection = 5;
		return 5;
	} else if (xDif > 0 && yDif > 0) {
		pfLastDirection = 4;
		return 4;
	} else if (xDif > 0 && yDif < 0) {
		pfLastDirection = 2;
		return 2;
	} else if (xDif < 0 && yDif > 0) {
		pfLastDirection = 6;
		return 6;
	} else if (xDif < 0 && yDif < 0) {
		pfLastDirection = 8;
		return 8;
	}
   return 0;
}

void pfStopMovement(char *s) {
   pfBoolStop = TRUE;
}