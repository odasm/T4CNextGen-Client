#pragma warning( disable : 4291 )
#pragma warning(disable:4786)

#include "pch.h"
#include <windows.h>
#include "Mouse.h"
#include "TFCPlayer.h"
#include "Bitmap.h"
#include "PowerDebug.h"
#include "TileSet.h"
#include "VisualObjectList.h"
#include "DirectSocket.h"
#include "TFCPacket.h"
#include "TFCSocket.h"
#include "MacroHandler.h"
#include "TileSet.h"
#include "Franck.h"
#include "pf.h"
#include <stdio.h>
#include "Global.h"
#include "Packet.h"
#include "UIMouseEvent.h"
#include "MemoryManager.h"
#include "SaveGame.h"
extern CSaveGame g_SaveGame;

#include "NewInterface/RootBoxUI.h"
#include "NewInterface/SpellUI.h"
#include "NewInterface/ChestUI.h"
#include "NewInterface/TradeUI.h"


#define DM_NONE  0
#define DM_DOWN  1
#define DM_CLICK  2
#define DM_DOUBLE_CLICK  3
#define DM_UP  4
#define DM_DRAG  5
#define DM_DROP  6

extern DWORD Try;

extern bool NeedRedraw;

time_t Old = 0;
time_t Old2 = 0;

extern bool Move;
extern bool Move2;
const int MAXMOV = 20;
extern char Movement[MAXMOV];
extern bool Attack;
extern bool Get;
extern bool Use;
extern DWORD TargetID;

extern Bitmap Grid;
extern Bitmap GridID;
extern bool ForceSpell;
extern int XAttack;
extern int YAttack;
extern int XGet;
extern int YGet;
extern int XUse;
extern int YUse;
extern int TalkToX;
extern int TalkToY;
extern unsigned long TalkToID;
static unsigned int TalkTime = 0;
extern short TalkToOffset;
extern sockaddr_in ServerIPaddr;
extern unsigned char Direction;
extern bool Follow;
extern int FollowID;
extern bool SHIFT_State;

extern volatile unsigned long Round;
extern BACK Backpack[24];
extern TFCSocket Remote;


int x2, y2;

void Talking(int b[4], LPPOINT);
void UseOb(int b[4], LPPOINT);
void UseObOnSite(int b[4], LPPOINT);
void GetOb(int b[4], LPPOINT);
void Combat(int b[4], LPPOINT);
void See(void);
void MovePl(int b[4], LPPOINT);
void Nothing(void);
	
DWORD FreezeID = 0;

extern char Force;
extern int nbMovement;
extern time_t Old;

BOOL inAttack = FALSE;

inline int Abs(int x) {
	return ((x < 0) ? (-x) : (x));
}

void MouseAction(void) 
{
   MouseDevice Mouse2;
   int xp, yp;
   int button[4];
   int TrueObject = -1;
   if (!Player.Dead) 
   {
      static BOOL boGetDrag = FALSE;
      
      Mouse2.GetStatus(&xp, &yp, button);
      int dwLastZoomoption = g_Global.GetlastScrollStatus();
      g_Global.SetlastScrollStatus(0);
      
      POINT pt;
      pt.x = xp;
      pt.y = yp;
      
      //LOG << "Timing: " << timeGetTime() << "\r\n";
      
      if( UIMouseEvent::GetInstance()->ResolveQueuedEvent() ){
         return;
      }
      
      if( RootBoxUI::GetInstance()->IsMouseOwned( MousePos( pt ) ) ){
         return;
      }
      
      
      BOOL GetMsgBox = FALSE;
#ifdef GRBX_MOUSE	
      if (button[0] == DM_DRAG) {
         if(!boGetDrag) {
            Mouse2.GetDrag(&xp, &yp, 0);
            boGetDrag = TRUE;
            pt.x = xp;
            pt.y = yp;
         }
      } else {
         boGetDrag = FALSE;
      }
      
      /*      switch (button[0]) {
      case DM_DRAG: GetMsgBox = BoxHandler.PostBoxMessage(GWIN_MSG_DRAG, 0, NULL, (LPVOID)&pt); break;
      case DM_DROP: GetMsgBox = BoxHandler.PostBoxMessage(GWIN_MSG_DROP, 0, NULL, (LPVOID)&pt); break;
      case DM_CLICK: GetMsgBox = BoxHandler.PostBoxMessage(GWIN_MSG_CLICK, 0, NULL, (LPVOID)&pt); break;
      case DM_DOUBLE_CLICK: GetMsgBox = BoxHandler.PostBoxMessage(GWIN_MSG_DBLCLICK, 0, NULL, (LPVOID)&pt); break;
      case DM_DOWN: GetMsgBox = BoxHandler.PostBoxMessage(GWIN_MSG_CHKBOX, 0, NULL, (LPVOID)&pt); break;
      }	
      switch (button[1]) {
      case DM_CLICK: GetMsgBox = BoxHandler.PostBoxMessage(GWIN_MSG_CLICK_2, 0, NULL, (LPVOID)&pt); break;
   } */
      
#endif
      
      if (GetMsgBox) return;
      
      if (button[0] == DM_NONE) {
         Old = 0;
         if (Force) {
            Force = 0;
            pfStopMovement("Move 1");
            Attack = false;
            Use = false;
            Get = false;
         }
         inAttack = FALSE;
      }

      // need to manage X and Y to support Zoom format :P
      int dwZoomStatus = g_Global.GetZoomStatus();
      if(dwZoomStatus)
      {
         int dwNewWidth  =   g_Global.GetScreenW()- (g_Global.GetScreenW()*(5*dwZoomStatus)/100);
         int dwNewHeight =   g_Global.GetScreenH()- (g_Global.GetScreenH()*(5*dwZoomStatus)/100);
         int dwNX = (xp * dwNewWidth ) / g_Global.GetScreenW();
         int dwNY = (yp * dwNewHeight) / g_Global.GetScreenH();
         xp = dwNX + ((g_Global.GetScreenW()-dwNewWidth)/2);
         yp = dwNY + ((g_Global.GetScreenH()-dwNewHeight)/2);

         pt.x = xp;
         pt.y = yp;
      }


      x2 = (xp+48-16)/32;
      y2 = (yp-8)/16;

      
      
      Objects.Lock(33);
      TargetID = GridID(x2, y2);

      POINT wp;
      Player.ScreenPosToWL(&pt, &wp);

      g_Global.SetMouseLastWorld(wp.x,wp.y);

      Objects.Unlock(33);
      
      
      BOOL boIsAnyInterfaceUIBlockingMove = FALSE;
		{	// Checks if chest interfaces are blocking movements
			if (
				   ChestUI::GetInstance()->IsShown()
				|| TradeUI::GetInstance()->IsShown()
				) 
			{
				boIsAnyInterfaceUIBlockingMove = TRUE;
			}
		}

      if (boIsAnyInterfaceUIBlockingMove == FALSE && ( button[0] == DM_DRAG || button[0] == DM_DROP || button[0] == DM_DOWN || button[0] == DM_CLICK || button[0] == DM_DOUBLE_CLICK || button[1] == DM_CLICK || button[1] == DM_DOUBLE_CLICK || button[1] == DM_DRAG || button[1] == DM_DROP ) )  
      {
         if (Grid(x2, y2) == 3 && !Force && !ForceSpell && !SHIFT_State) {
            // TALKING
            Talking(button, &pt);
         }else if (Grid(x2, y2) == 6 && !Force && !ForceSpell && !SHIFT_State) {
            // USE_ONSITE
            UseObOnSite(button, &pt);            
         }else if (Grid(x2, y2) == 4 && !Force && !ForceSpell && !SHIFT_State) {
            // USE
            UseOb(button, &pt);
         }	else if (Grid(x2, y2) == 5 && !Force && !ForceSpell && !SHIFT_State) {
            // GET
            GetOb(button, &pt);
         } else if (Grid(x2, y2) == 2 && !Force || ForceSpell || SHIFT_State) {
            // COMBAT
            Combat(button, &pt);
         } else if (Grid(x2, y2) == 1 && !Force && !ForceSpell && !SHIFT_State) {
            // SEE
            See();																							
         } else if ((Grid(x2, y2) == 0 ||	Force) && !ForceSpell && !SHIFT_State) {
            // MOVE
            MovePl(button, &pt);
         } 
         else 
         {
            // NOTHING
            Nothing();
         }
      }

      if(dwLastZoomoption)
         g_Global.SetZoomStatus(dwLastZoomoption);
     
   }
}	


void Talking(int button[4], LPPOINT pt) {
	// TALKING
	if (button[1] == DM_CLICK || button[1] == DM_DOUBLE_CLICK || button[1] == DM_DROP) {
		signed char xDif = 0;
		signed char yDif = 0;
		unsigned long ID = 0;
		Objects.Lock(34);
		ID = GridID(x2, y2);
      Objects.Identify(ID); 
		Objects.Unlock(34);
	} else if (button[0] == DM_CLICK || button[0] == DM_DOUBLE_CLICK || button[0] == DM_DROP) {
		signed char xDif = 0;
		signed char yDif = 0;
      if (TalkToID != GridID(x2,y2) || timeGetTime() - TalkTime > 5000) {
         TalkToID = GridID(x2, y2);
         TalkTime = timeGetTime();
   		Objects.Lock(35);
		   Objects.Found(&xDif, &yDif, TalkToID);
		   Objects.NoIdentify(TalkToID);
		   Objects.Unlock(35);
		   Direction = 1;
		   int a = (xDif+11)*3;
		   int b = (yDif+16)*2;
		   if (b > 30) {
   			if (a > 30) {
	   			a -= 30;
		   		b -= 30;
			   	if (a > b*2) {
				   	Direction = 6;
   				} else if (b > a*2) {
	   				Direction = 2;
		   		} else {
			   		Direction = 3;
				   }
   			} else {
	   			b -= 30;
		   		a = 30 - a;
			   	if (a > b*2) {
				   	Direction = 4;
   				} else if (b > a*2) {
	   				Direction = 2;
		   		} else {
			   		Direction = 1;
				   }
   			}
	   	} else {
		   	if (a > 30) {
			   	a -= 30;
				   b = 30 - b;
   					if (a > b*2) {
   					Direction = 6;
	   			} else if (b > a*2) {
		   			Direction = 8;
			   	} else {
				   	Direction = 9;
   				}
	   		} else {
		   		a = 30 - a;
			   	b = 30 - b;
				   if (a > b*2) {
					   Direction = 4;
   				} else if (b > a*2) {
	   				Direction = 8;
		   		} else {
			   		Direction = 7;
				   }
   			}			
	   	}
	      
   		TFCPacket Send; 
      
		   TalkToX = (Player.xPos+xDif);
   		TalkToY = (Player.yPos+yDif);

         Send << (short)RQ_DirectedTalk;
	   	Send << (short)(Player.xPos+xDif);
		   Send << (short)(Player.yPos+yDif);
	   	TalkToOffset = 0;
	   	Send << (long)TalkToID;
		   Send << (char)Direction;
   		Send << (long)Custom.gTextColor;
	   	Send << (short)0;
		   Objects.Lock(37);
         Objects.GetMainObject()->Direction = Direction;
         if (Objects.FoundID(TalkToID)) 
         {
		      SEND_PACKET(Send);
         }
         Objects.Unlock(37);
	   }
   }       
}

void UseOb(int button[4], LPPOINT pt) {
	if (button[1] == DM_CLICK || button[1] == DM_DOUBLE_CLICK || button[1] == DM_DROP) {

		signed char xDif = 0;
		signed char yDif = 0;
		unsigned long ID = 0;
		Objects.Lock(38);
		ID = GridID(x2, y2);

		Objects.Identify(ID); 
		Objects.Unlock(38);
	} else if (button[0] == DM_CLICK || button[0] == DM_DOUBLE_CLICK || button[0] == DM_DROP) {
		Follow = true;
		Use = true;

		signed char xDif = 0;
		signed char yDif = 0;
		Objects.Lock(39);
		FollowID = GridID(x2, y2);
		Objects.Found(&xDif, &yDif, FollowID);
		Objects.NoIdentify(FollowID);
		Objects.Unlock(39);
	
		XUse = xDif + Player.xPos;
		YUse = yDif + Player.yPos;

		pfSetPosition(XUse, YUse);
      pfNearPosition();
      pfNearPosition();

        TalkToX = (Player.xPos+xDif);
   		TalkToY = (Player.yPos+yDif);
        //TalkToID = FollowID;

		if (Move && Move2) {
			Move = false;
			TFCPacket Send;
			
			if (pfIsAtPosition()) {
				Follow = false;
				Move = true;
				TFCPacket Send; 
				TFCPacket Info;

				Send << (short)23;
				Send << (short)XUse;
				Send << (short)YUse;
				Send << (long)FollowID;
			
				SEND_PACKET(Send);
				Use = false;
			} else {
				short Menrage = (short)pfGetNextMovement();
				if (Menrage) {
				   Send << Menrage;
					   LOG << "(" << timeGetTime() << ") MOUSE MOVEMENT 1\r\n";
				   Try = timeGetTime();

				   SEND_PACKET(Send);
				}
			}
		}
	}
}

void UseObOnSite(int button[4], LPPOINT pt) {
	if (button[1] == DM_CLICK || button[1] == DM_DOUBLE_CLICK || button[1] == DM_DROP) {

		signed char xDif = 0;
		signed char yDif = 0;
		unsigned long ID = 0;
		Objects.Lock(38);
		ID = GridID(x2, y2);
      Objects.Identify(ID); 

      Objects.Unlock(38);
	} else if (button[0] == DM_CLICK || button[0] == DM_DOUBLE_CLICK || button[0] == DM_DROP) {
		//Use = true;

        DWORD foundID;

		signed char xDif = 0;
		signed char yDif = 0;
		Objects.Lock(39);
		foundID = GridID(x2, y2);
		Objects.Found(&xDif, &yDif, foundID);
		Objects.NoIdentify(foundID);
		Objects.Unlock(39);
	
		XUse = xDif + Player.xPos;                                                                               
		YUse = yDif + Player.yPos;

        /*TalkToX = (Player.xPos+xDif);
   		TalkToY = (Player.yPos+yDif);
        TalkToID = foundID;*/

		pfSetPosition(XUse, YUse);
        pfNearPosition();
        pfNearPosition();

		TFCPacket Send; 
		TFCPacket Info;

		Send << (short)23;
		Send << (short)XUse;
		Send << (short)YUse;
		Send << (long)foundID;
	
		SEND_PACKET(Send);
	}
}

void GetOb(int button[4], LPPOINT pt) {//BLBL info : pick up object, get object pickup, ramasser, leftclick left click
// GET
	
	//si bouton droit :
	if (button[1] == DM_CLICK || button[1] == DM_DOUBLE_CLICK || button[1] == DM_DROP) {

		signed char xDif = 0;
		signed char yDif = 0;
		unsigned long ID = 0;
		Objects.Lock(40);
		ID = GridID(x2, y2);
	
		//++NMNMNM --[Add item Specification on right click item ON FLOOR]
      if(g_SaveGame.GetOptionsParam()->bShowItemSpec)
      {
         g_Global.ResetItemInfo();
         g_Global.SetItemInfoID(ID);
         g_Global.SetItemInfoInventory(false);
         TFCPacket sending2;
         sending2 << (RQ_SIZE)RQ_QueryItemInfo;
         sending2 << (long)ID;
         SEND_PACKET( sending2 );
      }
      else
		   Objects.Identify(ID); 
      

		Objects.Unlock(40);
	
	//si bouton gauche :
	} else if (button[0] == DM_CLICK || button[0] == DM_DOUBLE_CLICK || button[0] == DM_DROP) { 
      
		Follow = true;
		Get = true;

		signed char xDif = 0;
		signed char yDif = 0;
		Objects.Lock(41);//entrée en section critique
		FollowID = GridID(x2, y2);
		Objects.Found(&xDif, &yDif, FollowID);
		Objects.NoIdentify(FollowID);
		Objects.Unlock(41);//sortie section critique
	
		XGet = xDif + Player.xPos;
		YGet = yDif + Player.yPos;

		pfSetPosition(XGet, YGet);
        pfNearPosition();

		if (Move && Move2) {
			Move = false;
			TFCPacket Send;
			
			if (pfIsAtPosition()) {
				Follow = false;
				Move = true;
				TFCPacket Send; 
				TFCPacket Info;

				Send << (short)11;
				Send << (short)XGet;
				Send << (short)YGet;
				Send << (long)FollowID;
			
				SEND_PACKET(Send);
				Get = false;
			} else {
				short Menrage = (short)pfGetNextMovement();
            if (Menrage) {
               Send << Menrage;
				   LOG << "(" << timeGetTime() << ") MOUSE MOVEMENT 1\r\n";
               Try = timeGetTime();
			
               SEND_PACKET(Send);
            }
			}
		}
	}
}

void Combat(int button[4], LPPOINT pt) {
	// COMBAT
   static DWORD dwLastTime = 0;
   if (button[0] == DM_DOUBLE_CLICK && g_SaveGame.GetOptionsParam()->bLockTarget) {
		Objects.Lock(42);
		FreezeID = GridID(x2, y2);
		Objects.Unlock(42);
	}
	
	if (button[1] == DM_CLICK || button[1] == DM_DOUBLE_CLICK || button[1] == DM_DROP) {
		signed char xDif = 0;
		signed char yDif = 0;
		unsigned long ID = 0;
		Objects.Lock(43);
		ID = GridID(x2, y2);
	
		Objects.Identify(ID); 
		Objects.Unlock(43);
   } 
   else if ((button[0] == DM_DRAG || button[0] == DM_CLICK || button[0] == DM_DOUBLE_CLICK) && (!inAttack || FollowID != (int)GridID(x2, y2))) 
   {
      if (SHIFT_State && (timeGetTime() - dwLastTime) > 500) {
         dwLastTime = timeGetTime();
         TFCPacket Send;
         TFCPacket Info;
         
         Send << (short)24;
         Send << (short)Player.xPos;
         Send << (short)Player.yPos;
         Send << (long)NULL;
         
         SEND_PACKET(Send);
      } else if (!SHIFT_State) {
         
         if (button[0] == DM_DRAG)
            inAttack = TRUE;

         signed char xDif = 0;
         signed char yDif = 0;
         
         if( Player.rangedAttack ){
            if (ForceSpell) {
               Move = true;
               Attack = false;

               SpellUI::GetInstance()->CastLastSpell();
               return;
            }


             DWORD attackId;
             Objects.Lock(44);
             
             if (FreezeID && Objects.FoundID(FreezeID)){
                attackId = FreezeID;
             }else{
                attackId = GridID(x2, y2);
             }
             Objects.Found(&xDif, &yDif, attackId);
             Objects.Unlock(44);
             
             if( attackId != Player.ID ){
                short xPos = xDif + Player.xPos;
                short yPos = yDif + Player.yPos;

                TFCPacket Send;
                Send << (short)24;
                Send << (short)xPos;
                Send << (short)yPos;
                Send << (long)attackId;
          
                SEND_PACKET(Send);
             }

             return;
         }

         Follow = true;
         Attack = true;
         
         Objects.Lock(44);
         if (FreezeID && Objects.FoundID(FreezeID)) 
            FollowID = FreezeID;
         else
            FollowID = GridID(x2, y2);
         Objects.Found(&xDif, &yDif, FollowID);
         Objects.Unlock(44);
         
         XAttack = xDif + Player.xPos;
         YAttack = yDif + Player.yPos;
         
         if (!ForceSpell) {
            pfSetPosition(XAttack, YAttack);
            pfNearPosition();
         }
         
         if (Move && Move2) {
            Move = false;
            
            TFCPacket Send;
            TFCPacket Info;
            
            if (pfIsAtPosition() && !ForceSpell) {
               Follow = false;
               Move = true;
               if (FollowID) {
                  TFCPacket Send; 
                  TFCPacket Info;
                  
                  Send << (short)24;
                  Send << (short)XAttack;
                  Send << (short)YAttack;
                  Send << (long)FollowID;
                  
                  SEND_PACKET(Send);
               }
               Attack = false;
            } else if (!ForceSpell) {
               short Menrage = (short)pfGetNextMovement();
               if (Menrage) {
                  Send << Menrage;
                  
                  SEND_PACKET(Send);
               }
            }
            if (ForceSpell) {
               Move = true;
               Attack = false;

               SpellUI::GetInstance()->CastLastSpell();

               /*SpellBook *lpBox = static_cast< SpellBook * >( BoxHandler.GetBox( GWIDBOX_SPELLBOOK ) );
               
               if( lpBox != NULL ){
                  lpBox->HideWindow();
                  lpBox->CastLastSpell(FALSE, 0, 0, 0, TRUE);
                  lpBox->Unlock();
               }else{
                  lpBox = new SpellBook;
                  
                  lpBox->Lock();
                  GraphBoxHandler::RegisterBox( lpBox );	
                  lpBox->HideWindow();
                  lpBox->CastLastSpell(FALSE, 0, 0, 0, TRUE);
                  lpBox->Unlock();
               }*/
            }
         }
      }
   }
}

void See(void) {
// SEE
	int xDif = 0;
	int yDif = 0;
	xDif = x2 - 11;
	yDif = y2 - 16;
	signed char XTemp, YTemp;
	Objects.Lock(47);
	if (!Objects.RealPos(xDif, yDif, &XTemp, &YTemp))  {
		xDif++;
		if (!Objects.RealPos(xDif, yDif, &XTemp, &YTemp)) { 
			xDif-=2;
			Objects.RealPos(xDif, yDif, &XTemp, &YTemp);
		}
	}
	xDif = XTemp; yDif = YTemp;
	Objects.SetExtended(xDif, yDif);
	Objects.Unlock(47);
}

void MovePl(int button[4], LPPOINT pt) 
{
   if (FreezeID) 
   {
      Objects.Lock(49);
      if (!Objects.FoundID(FreezeID)) 
      {
         FreezeID = NULL;
      }
      Objects.Unlock(49);
   }
   
   if (button[0] == DM_DOUBLE_CLICK) 
   {
      FreezeID = NULL;
   }
   
   if (button[1] == DM_CLICK || button[1] == DM_DOUBLE_CLICK || button[1] == DM_DROP) 
   {
      signed char xDif = 0;
      signed char yDif = 0;
      unsigned long ID = 0;
      Objects.Lock(50);
      ID = GridID(x2, y2);
      
      if (ID)
         Objects.Identify(ID); 
      
      Objects.Unlock(50);
   }
   if (button[1] == DM_NONE && FreezeID && GridID(x2, y2) && !Force) 
   {
      Combat(button, pt);
   } 
   else if (button[1] == DM_NONE && !inAttack && (button[0] == DM_CLICK || button[0] == DM_DOUBLE_CLICK || button[0] == DM_DRAG)) 
   {
      Follow = false;
      int xDif = 0;
      int yDif = 0;
      xDif = x2 - 11;
      yDif = y2 - 16;
      
      POINT wp;
      
      Player.ScreenPosToWL(pt, &wp);
      
      pfSetPosition(wp.x, wp.y);
      
      if (button[0] == DM_DRAG)
         Force = 1;
      
      if (Move && !NeedRedraw && Move2) 
      {
         Move = false;
         TFCPacket Send;
         TFCPacket Info;
         
         short Menrage = (short)pfGetNextMovement();
         if (Menrage) 
         {
            Send << Menrage;
            
            SEND_PACKET(Send);
            Try = timeGetTime();
         }
      }
   }
}

void Nothing(void) {
   FollowID = NULL;
}
