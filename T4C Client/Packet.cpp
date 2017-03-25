///////////////////////////////////////////////////////////////////////
//  Packet.cpp                                                       //
//  PACKETS RECEIVED FROM THE SERVER                                 //
//  for packets sent by client, go to packethandling.cpp             //
//																	 //
///////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4786 )
#pragma warning( disable : 4291 )

#include "pch.h"
#include <windows.h>
#include <math.h>
#include "powerdebug.h"
#include "TFCPlayer.h"
#include "NTime.h"
#include "VisualObjectList.h"
#include "TFCSocket.h"
#include "TileSet.h"
#include "NTime.h"
#include "ObjectListing.h"
#include "Packet.h"
#include "pf.h"
#include <process.h>
#include "MacroHandler.h"
#include <stdio.h>
#include "old.h"
#include <eh.h>
#include "ExpFltr.h"
#include "LocalString.h"
#include "Fading.h"
#include "App.h"
#include "PacketHandling.h"
#include "Global.h"
#include "FileLogger.h"
#include "Puppet.h"

#include "NewInterface/InventoryUI.h"
#include "NewInterface/RootBoxUI.h"
#include "NewInterface/CharacterUI.h"
#include "NewInterface/ChatterUI.h"
#include "NewInterface/GroupPlayUI.h"
#include "NewInterface/SellUI.h"
#include "NewInterface/BuyUI.h"
#include "NewInterface/SkillTrainUI.h"
#include "NewInterface/SkillTeachUI.h"
#include "NewInterface/SpellUI.h"
#include "NewInterface/EffectStatusUI.h"
#include "NewInterface/OptionsUI.h"
#include "NewInterface/RobUI.h"
#include "NewInterface/ChestUI.h"
#include "NewInterface/TradeUI.h"
#include "NewInterface/RTMap.h"
#include "NewInterface/RTHelp.h"
#include "NewInterface/RTItemI.h"
#include "NewInterface/GuildUI.h"


#include "MemoryManager.h"

#include "Weather.h"
#include "SaveGame.h"
extern CSaveGame g_SaveGame;

extern bool seraphDisplay;
extern bool goodSeraph;

static BOOL    g_bEnterGamePart2Complete = FALSE;
void    EntergamePart2(LPVOID pParam);
bool boInterruptLogoff = FALSE;//BLBLBL antiplug variable
bool boForceLogoff = FALSE;//BLBLBL antiplug variable

__int64 XPStatValue=0,XPStatLastValue=0;//XPSTAT

QWORD FileTime2QWord(FILETIME *f) {
    QWORD q;
    q = f->dwHighDateTime;
    q = q << 32;
    q |= f->dwLowDateTime;
    return q;
}

void CloseAllUI(void) {
    if (CharacterUI::GetInstance()->IsShown()) {
        CharacterUI::GetInstance()->Hide();
    }
    if (InventoryUI::GetInstance()->IsShown()) {
        InventoryUI::GetInstance()->Hide();
    }
    if (SpellUI::GetInstance()->IsShown()) {
        SpellUI::GetInstance()->Hide();
    }
    if (GroupPlayUI::GetInstance()->IsShown() && !GroupPlayUI::GetInstance()->IsMinimized() ) {
        GroupPlayUI::GetInstance()->Hide();
    }
    if (ChatterUI::GetInstance()->IsShown()) {
        ChatterUI::GetInstance()->Hide();
    }
    if (MacroUI::GetInstance()->IsShown() && !MacroUI::GetInstance()->IsMinimized() ){//&& !MacroUI::GetInstance()->IsShown()){
        MacroUI::GetInstance()->Hide();
    }
    if (OptionsUI::GetInstance()->IsShown()) {
        OptionsUI::GetInstance()->Hide();
    }
}

BOOL DoNotMove = FALSE;
BOOL INGAME = FALSE;

DWORD SpellID = -10000;

bool noException = true;

HANDLE hDrawThread = NULL;
UINT iDrawThreadID = NULL;

HANDLE hMouseActionThread = NULL;
UINT iMouseActionThreadID = 0;

extern bool g_UiInit;
extern int SlowMeDown;
bool Code13 = false;
bool EnterGame = false;

extern bool Attack;
extern bool Get;
extern bool Use;

extern int xResize;
extern int yResize;
extern int ResizeCorrection;

BOOL FirstTime = TRUE;

extern BOOL ReAsk;

extern DWORD Try;

extern int DrawLastMoving;

/// TEMP
//extern FormatText g_ftStory;
extern Font *fMenuDescFont;
bool g_DisplayLetter = false;
/// END TEMP

extern bool Draw;

extern bool Move;

extern sockaddr_in ServerIPaddr;

extern bool NeedRedraw;

extern bool Retrace;

extern Font *fFont;

inline int Abs(int x) {
    return ((x < 0) ? (-x) : (x));
}

extern bool Follow;

extern bool ChangePal;

extern DWORD dwTimeAsk;
DWORD dwCriticalTime = 0;

extern bool stShow;

extern UINT CALLBACK DrawThread(LPVOID pParam);
extern UINT CALLBACK MouseActionThread(LPVOID pParam);

extern bool Err;

extern TFCSocket Remote;

extern char LastState;

static const int TFC_MENU						= 5;

extern char TFC_State;

extern bool Register;

extern volatile unsigned long Round;

extern T3VSBSound *Victim;

extern BACK Backpack[24];

extern int TalkToX, TalkToY;
extern unsigned long TalkToID;
extern short TalkToOffset;

extern DWORD LastAsk3;
extern DWORD LastAsk6;

extern T3VSBSound SoundFX[250];
extern T3VSBSound ConnectSoundFX;

extern bool LevelUp;

extern bool WantPreGame;

extern bool bDisplayList;
extern char DisplayList[25][100];
extern int  DisplayInt[25];
extern int  nbList;

extern TFCPlayer Player;
extern DWORD LAG2;

extern TFCSocket Remote;


HANDLE InterPacketEvent = NULL;

//////////////////////////////////////////////////////////////////////////////////////////
inline void GetEquipItem
//////////////////////////////////////////////////////////////////////////////////////////
//  Gets an equipped item from an equip packet.
// 
(
 int equipPos,  // The equipment position.
 TFCPacket *msg // The packet.
 )
 // Return: inline void, 
 //////////////////////////////////////////////////////////////////////////////////////////
{
    DWORD  itemID;
    WORD   appear;
    LPBYTE name;
    WORD   qty;
    WORD   baseID;
    long   charges;
    
    GET_LONG( itemID );
    GET_WORD( appear );
    GET_WORD( baseID );
    GET_WORD( qty );
    GET_LONG( charges );
    GET_STRING( name );    
    
    // If the itemID is empty.
    if( itemID == 0 ){
        // If there was an equipped item at that position.
        if( Player.lpbEquipped[ equipPos ] != NULL ){
            // Delete the bag item.
            delete Player.lpbEquipped[ equipPos ];
            
            Player.lpbEquipped[ equipPos ] = NULL;
        }
        delete name;
    }else{
        // If the item is currently NULL.
        if( Player.lpbEquipped[ equipPos ] == NULL ){
            Player.lpbEquipped[ equipPos ] = new BAG_ITEM;
        }
        
        BAG_ITEM *lpBagItem = Player.lpbEquipped[ equipPos ];
        
        lpBagItem->nX = -1;	// assign random pos to item
        lpBagItem->nY = -1;
        lpBagItem->dwID = itemID;
        lpBagItem->wBaseID = baseID;
        strcpy( reinterpret_cast< char * >( lpBagItem->chName ), reinterpret_cast< char * >( name ) );
        
        lpBagItem->Type = appear;
        lpBagItem->lpSprite = InvItemIcons( appear );
        lpBagItem->dwQty = qty;
        lpBagItem->charges = charges;
        
        delete name;
    }
}

class InterPacket : public TemplateList <unsigned char *> {
public:
    void Set(void) {
        SetEvent(InterPacketEvent);
    }
};

InterPacket ug_InterPacket;

QWORD qwPacketUseCounter[150];
QWORD qwPacketUseTime[150];
QWORD qwPacketUsePeek[150];

//////////////////////////////////////////////////////////////////////////////////////////
void ShootArrow
//////////////////////////////////////////////////////////////////////////////////////////
// Shoots an arrow
// 
(
 DWORD CASTER,  // The arrow's caster
 DWORD TARGET,  // The arrow's target. Can be 0.
 SHORT TPOSX,   // The target X pos.
 SHORT TPOSY,   // The target Y pos.
 bool  collide, // Destroy on site or continue forever.
 char  targetHp // The new target's relative hp, if a target was specified.
 )
 //////////////////////////////////////////////////////////////////////////////////////////
{
    SHORT CPOSX, CPOSY;
    signed char x2, y2;
    World.Lock(54);
    Objects.Lock(1000);
    
    if( TARGET ){
        // Get the position of the target unit.
        if (Player.ID == TARGET) {
            TPOSX = Player.xPos;
            TPOSY = Player.yPos;
        }else{
            if( Objects.Found((signed char *)&x2, (signed char *)&y2, TARGET) ) {
                TPOSX = x2 + Player.xPos;
                TPOSY = y2 + Player.yPos;
                
                Objects.SetHP( TARGET, targetHp );
            }
        }
    }
    if (!CASTER) {
        World.Unlock(54);
        Objects.Unlock(1000);
        
        return;
    }
    // If the player is the caster, determine the arrow's path according to its pos.
    if ( Player.ID == CASTER ){
        CPOSX = Player.xPos;
        CPOSY = Player.yPos;
		Objects.GetMainObject()->Direction = 1;
        int a = ((TPOSX-Player.xPos)+11)*3;
        int b = ((TPOSY-Player.yPos)+16)*2;
        // Determine the position according to its quadrant.
        if (b > 30) {
            if (a > 30) {
                a -= 30;
                b -= 30;
                if (a > b*2) {
                    Objects.GetMainObject()->Direction = 6;
                } else if (b > a*2) {
                    Objects.GetMainObject()->Direction = 2;
                } else {
                    Objects.GetMainObject()->Direction = 3;	
                }
            } else {
                b -= 30;
                a = 30 - a;
                if (a > b*2) {
                    Objects.GetMainObject()->Direction = 4;
                } else if (b > a*2) {
                    Objects.GetMainObject()->Direction = 2;
                } else {
                    Objects.GetMainObject()->Direction = 1;
                }
            }
        } else {
            if (a > 30) {
                a -= 30;
                b = 30 - b;
                if (a > b*2) {
                    Objects.GetMainObject()->Direction = 6;
                } else if (b > a*2) {
                    Objects.GetMainObject()->Direction = 8;
                } else {
                    Objects.GetMainObject()->Direction = 9;
                }
            } else {
                a = 30 - a;
                b = 30 - b;
                if (a > b*2) {
                    Objects.GetMainObject()->Direction = 4;
                } else if (b > a*2) {
                    Objects.GetMainObject()->Direction = 8;
                } else {
                    Objects.GetMainObject()->Direction = 7;
                }
            }			
        }
    }else{
        // Set the object's direction according to the target's pos.
        if (Objects.Found((signed char *)&x2, (signed char *)&y2, CASTER)) {
            CPOSX = x2 + Player.xPos;
            CPOSY = y2 + Player.yPos;
            if( TARGET != NULL ){
                Objects.SetDirection(CASTER, TARGET, true);
            }
        }
    }
    
    
    // If the arrow didn't collide with anything
    if( !collide ){
        if( TPOSY == CPOSY ){
            if( TPOSX < CPOSX ){
                TPOSX = Player.xPos - 14;
            }else{
                TPOSX = Player.xPos + 14;
            }
        }else if( TPOSX == CPOSX ){
            if( TPOSY < CPOSY ){
                TPOSY = Player.yPos - 18;
            }else{
                TPOSY = Player.yPos + 18;
            }
        }else{
            // Extrapolate it out of the screen.
            double slope = static_cast< double >( CPOSY - TPOSY ) / 
                static_cast< double >( CPOSX - TPOSX );
            
            if( slope > 1 || slope < -1 ){
                // Grow the Y part and extrapolate the X part.
                slope = 1 / slope;
                
                if( TPOSY < CPOSY ){
                    TPOSY = TPOSY - 20;
                }else{
                    TPOSY = TPOSY + 20;
                }
                
                TPOSX = (short)( (double)( TPOSY - CPOSY ) * slope ) + CPOSX;
            }else{
                // Grow the X part and extrapolate the Y part.
                if( TPOSX < CPOSX ){
                    TPOSX = TPOSX - 20;
                }else{
                    TPOSX = TPOSX + 20;
                }
                
                TPOSY = (short)( (double)( TPOSX - CPOSX ) * slope ) + CPOSY;
            }
            
        }
    }
    
    int xDiff = TPOSX - CPOSX;
    int yDiff = TPOSY - CPOSY;
    
    DWORD direction = 1;
    
    if (xDiff < 0 && yDiff == 0){
        direction = 4;
    }else if (xDiff > 0 && yDiff == 0){
        direction = 6;
    }else if (xDiff == 0 && yDiff > 0){
        direction = 2;
    }else if (xDiff == 0 && yDiff < 0){
        direction = 8;
    }else if (xDiff < 0 && yDiff > 0){
        direction = 1;
    }else if (xDiff < 0 && yDiff < 0){
        direction = 7;
    }else if (xDiff > 0 && yDiff > 0){
        direction = 3;
    }else if (xDiff > 0 && yDiff < 0){
        direction = 9;
    }
    
    Follow = NULL;
    
    //++NMNMNM --[NEW TROWING Weopen Draw1]
    /*
    if(Custom.bEnableCArrow==1)
	Objects.Add(SpellID, __SPELL_TROWING_STAR1, CPOSX, CPOSY, 0, 0, Follow, 100, (char)direction, true );
    else if(Custom.bEnableCArrow==2)
	Objects.Add(SpellID, __SPELL_TROWING_DAGGER1, CPOSX, CPOSY, 0, 0, Follow, 100, (char)direction, true );
    else
    */
	Objects.Add(SpellID, __SPELL_ARROW, CPOSX, CPOSY, 0, 0, Follow, 100, (char)direction, true );
	
    if (TPOSX && TPOSY) 
    {
        // Arrow ID.
        DWORD SummonID = 0;
        if( collide )
        {
            //SummonID = __SPELL_BROKEN_ARROW;
        }
        
        ///++NMNMNM --[NEW TROWING Weopen Draw2]
        /*
        if(Custom.bEnableCArrow == 1)
		Objects.MoveObject(SpellID, TPOSX, TPOSY, TRUE, SummonID, 100, __SPELL_TROWING_STAR1, 0, 100);
        else if(Custom.bEnableCArrow==2)
		Objects.MoveObject(SpellID, TPOSX, TPOSY, TRUE, SummonID, 100, __SPELL_TROWING_DAGGER1, 0, 100);
        else
        */
		Objects.MoveObject(SpellID, TPOSX, TPOSY, TRUE, SummonID, 100, __SPELL_ARROW, 0, 100);
    }
    
    
    if ( CASTER == Player.ID ) {        
        Objects.PlAttack( TARGET, targetHp );
        
        Objects.GetMainObject()->Chose = 1;
        Objects.GetMainObject()->Speed = 0;
        Objects.GetMainObject()->AttSpeed = 0;        
        if( Objects.GetMainObject()->AttFrame > 3 ){
            Objects.GetMainObject()->AttFrame = 3;
        }
        Objects.GetMainObject()->Direction = (unsigned char)direction;
        
        int xDiff = TPOSX - Player.xPos;
        int yDiff = TPOSY - Player.yPos;
        
        if (xDiff < 0 && yDiff == 0){
            Objects.GetMainObject()->Direction = 4;
        }else if (xDiff > 0 && yDiff == 0){
            Objects.GetMainObject()->Direction = 6;
        }else if (xDiff == 0 && yDiff > 0){
            Objects.GetMainObject()->Direction = 2;
        }else if (xDiff == 0 && yDiff < 0){
            Objects.GetMainObject()->Direction = 8;
        }else if (xDiff < 0 && yDiff > 0){
            Objects.GetMainObject()->Direction = 1;
        }else if (xDiff < 0 && yDiff < 0){
            Objects.GetMainObject()->Direction = 7;
        }else if (xDiff > 0 && yDiff > 0){
            Objects.GetMainObject()->Direction = 3;
        }else if (xDiff > 0 && yDiff < 0)
        {
            Objects.GetMainObject()->Direction = 9;
        }
        else
        {
			Objects.GetMainObject()->Direction = 1;
        }
    }else{
        Objects.SetAttack( CASTER, TARGET, 1, 1, targetHp, direction );
    }
    
    SpellID--;
    Objects.Unlock(1000);
    World.Unlock(54);
}


void HandlePacket(TFCPacket *Msg) {
	short Type;
	
	try 
	{
		
		COMM.KeepAlive();
		
		/* 	  char strtoto[100];
		sprintf(strtoto,"++++++++IN PAK\n");
		OutputDebugString(strtoto);*/
		
		Msg->Get((short *)&Type);//BLBL possible cause du client qui freeze en attente éternelle, avec detection du délai par prise rouge.
		//BLBL il faudrait trouver un moyen d'annuler la lecture si ça prend trop de temps.
		
		/*      sprintf(strtoto,"++++++++AFTER PAK Type was %d\n",Type);
		OutputDebugString(strtoto);*/
		
		{
			DWORD intrQueueLen = 0;
			if( COMM.GetCommCenter() != NULL )
			{
				intrQueueLen = COMM.GetCommCenter()->GetIntrQueueSize();
			}
			if( intrQueueLen > 10 ){
				LOG.Lock();
				LOG << "\r\nQUEUELEN=" << intrQueueLen << ".";
				LOG.Unlock();
				// If the packet is of any of these type, JUNK IT
				switch( Type ){
				case RQ_MoveNorth:
				case RQ_MoveNorthEast:
				case RQ_MoveEast:
				case RQ_MoveSouthEast:
				case RQ_MoveSouth:
				case RQ_MoveSouthWest:
				case RQ_MoveWest:
				case RQ_MoveNorthWest: 
					if( intrQueueLen > 25 )
					{
						return;
					}
					break;
				case RQ_Attack:
				case RQ_CastSpell:
				case RQ_HPchanged:
				case RQ_GetStatus:
				case RQ_XPchanged:
				case RQ_GetOnlinePlayerList:
				case RQ_GoldChange:
				case RQ_SpellEffect:
				case RQ_ManaChanged:
				case RQ_GetChatterUserList:
				case RQ_GetChatterChannelList:
				case RQ_UpdateGroupMemberHp:
				case RQ_UpdateWeight:
				case RQ_ArrowMiss:
				case RQ_ArrowHit:
					return;
				}
			};
		}
		
		if (Type == 10) //RQ_Ack // quand on recoit une demande d'accusé on renvoi juste un accusé
		{
			TFCPacket Send; 
			
			Send << (short)10;//RQ_Ack
			
			SEND_PACKET(Send);
		} 
		else 
		{
			
			int CounterType = Type;
			
			if (Type >= 10000) {
				// Some packer are too high too fit in the 150 array. They become 100+ Type instead of
				// 10000+ Type.
				CounterType -= 10000;
				CounterType += 100;
			}
			
			//const RQ_GroupInvite          = 78;
			//const RQ_UpdateGroupMembers   = 76;
			//const RQ_NotifyGroupDisband   = 82;
			//const RQ_GroupLeave           = 80;
			//   const RQ_UpdateGroupMemberHp  = 87;
			//const RQ_GroupToggleAutoSplit = 88;
			
			LOG.Lock();
			LOG << "\r\nReceived Packet " << Type << "..!";
			LOG.Unlock();
			
			
			switch(Type) {
				// Type of Packet
				// Get Equipped list	
				//////////////////////////////////////////////////////////////////////////////////////////
				// Returns the stats of a player.
				
				//////////////////////////////////////////////////////////////////////////////////////////
				// Update the autosplit status of the group.
			case RQ_SeraphArrival: 
				{
					// 10004 part
					LOG << "* SERAPH ARRIVAL\r\n";
					LOG << "* PAK = 1-4 {\r\n";
					signed char LIGHT;
					unsigned short X, Y, TYPE;
					unsigned long ID;
					unsigned char STATUS;
					char PHP;
					unsigned long junk;
					Msg->Get((short *)&junk); // 10004
					Msg->Get((short *)&X);
					Msg->Get((short *)&Y);
					Msg->Get((short *)&TYPE);
					Msg->Get((long  *)&ID);
					Msg->Get((char *)&LIGHT);
					Msg->Get((char *)&STATUS);
					Msg->Get((char *)&PHP);
					
					// 68 Part
					
					WORD  BODY, FEET, GLOVES, HELM, LEGS, W_RIGHT, W_LEFT, CAPE;
					
					Msg->Get((short *)&junk);
					Msg->Get((long *)&ID);
					Msg->Get((short *)&BODY);
					Msg->Get((short *)&FEET);
					Msg->Get((short *)&GLOVES);
					Msg->Get((short *)&HELM);
					Msg->Get((short *)&LEGS);
					Msg->Get((short *)&W_RIGHT);
					Msg->Get((short *)&W_LEFT);
					Msg->Get((short *)&CAPE);
					
					// Addition.
					if( g_SaveGame.GetOptionsParam()->bSeraphAnim)
					{
						if (ID != Player.ID) 
						{
							World.Lock(75);
							Objects.Lock(1002);
							if (TYPE == __PLAYER_HUMAN_PUPPET) 
							{
								if (CAPE == __OBJGROUP_SERAPH_WHITE_WINGS) 
								{
									Objects.Add(ID, __SPELL_SERAPH_MALE, X, Y, 100, STATUS, NULL, PHP);
								}
								else if (CAPE == __OBJGROUP_SERAPH_BLACK_WINGS) 
								{
									Objects.Add(ID, __SPELL_EVIL_SERAPH_MALE, X, Y, 0, STATUS, NULL, PHP);
								}
								else if (CAPE == __OBJGROUP_DARKWINGS) 
								{
									Objects.Add(ID, __SPELL_EVIL_SERAPH_MALE, X, Y, 0, STATUS, NULL, PHP);
								}
								else if (CAPE == __OBJGROUP_SERAPH_DECHU_WINGS || CAPE == __OBJGROUP_SERAPH_DECHU2_WINGS) 
								{
									Objects.Add(ID, __SPELL_DECHU_MALE, X, Y, 0, STATUS, NULL, PHP);
								}
								else 
								{
									Objects.Add(ID, TYPE, X, Y, LIGHT, STATUS, NULL, PHP);
								}
								
							} 
							else if (TYPE == __PLAYER_HUMAN_FEMALE) 
							{
								if (CAPE == __OBJGROUP_SERAPH_WHITE_WINGS) 
								{
									Objects.Add(ID, __SPELL_SERAPH_FEMALE, X, Y, 100, STATUS, NULL, PHP);
								} 
								else if (CAPE == __OBJGROUP_SERAPH_BLACK_WINGS) 
								{
									Objects.Add(ID, __SPELL_EVIL_SERAPH_FEMALE, X, Y, 0, STATUS, NULL, PHP);
								} 
								else if (CAPE == __OBJGROUP_DARKWINGS) 
								{
									Objects.Add(ID, __SPELL_EVIL_SERAPH_FEMALE, X, Y, 0, STATUS, NULL, PHP);
								}
								else if (CAPE == __OBJGROUP_SERAPH_DECHU_WINGS || CAPE == __OBJGROUP_SERAPH_DECHU2_WINGS) 
								{
									Objects.Add(ID, __SPELL_DECHU_FEMALE, X, Y, 0, STATUS, NULL, PHP);
								}
								else 
								{
									Objects.Add(ID, TYPE, X, Y, LIGHT, STATUS, NULL, PHP);
								}
							}
							Objects.Unlock(1002);
							World.Unlock(75);
						} 
						else 
						{
							Player.xPos = X;
							Player.yPos = Y;
							Player.Light = LIGHT;
							
							g_Global.ValidMapZonePosition(Player.xPos,Player.yPos);
							
							Objects.Lock(0);
							if (CAPE == __OBJGROUP_SERAPH_WHITE_WINGS) 
							{
								Objects.Add(SpellID, __SPELL_SERAPH, X, Y, 100, STATUS, NULL, PHP);
							} 
							else if (CAPE == __OBJGROUP_SERAPH_BLACK_WINGS) 
							{
								Objects.Add(SpellID, __SPELL_EVIL_SERAPH, X, Y, 0, STATUS, NULL, PHP);
							} 
							else if (CAPE == __OBJGROUP_DARKWINGS) 
							{
								Objects.Add(SpellID, __SPELL_EVIL_SERAPH, X, Y, 0, STATUS, NULL, PHP);
							} 
							else if (CAPE == __OBJGROUP_SERAPH_DECHU_WINGS || CAPE == __OBJGROUP_SERAPH_DECHU2_WINGS) 
							{
								Objects.Add(ID, __SPELL_DECHU, X, Y, 0, STATUS, NULL, PHP);
							}
							Objects.Unlock(0);
							SpellID--;
						}
					} 
					else 
					{
						
						if (ID != Player.ID) 
						{ // .. and its not my own entrance..
							Objects.Lock(1);
							Objects.Add(ID, TYPE, X, Y, LIGHT, STATUS, NULL, PHP);
							Objects.Unlock(1);
						}
					}
					
					Objects.Lock(57);
					Objects.SetPuppet(ID, BODY, FEET, GLOVES, HELM, LEGS, W_RIGHT, W_LEFT, CAPE);
					Objects.Unlock(57);
            } break;
            
        case RQ_GroupToggleAutoSplit:{
			TFCPacket *msg = Msg;
			
			BYTE bAutoShare = 0;
			
			GET_BYTE( bAutoShare );
			
			GroupPlayUI::GetInstance()->UpdateAutoShare( bAutoShare == 0 ? false : true );
			
                                     } break;
			//////////////////////////////////////////////////////////////////////////////////////////
			// Update the HP of a single group member.
        case RQ_UpdateGroupMemberHp:{
			TFCPacket *msg = Msg;
			
			DWORD id;
			WORD  hpPercent;
			
			GET_LONG( id );
			GET_WORD( hpPercent );
			
			// Update the member's hp.
			GroupPlayUI::GetInstance()->UpdateMemberHp( id, hpPercent );
			Objects.Lock(0);
			Objects.SetHP( id, (char)hpPercent );
			Objects.Unlock(0);
                                    } break;
			
			//////////////////////////////////////////////////////////////////////////////////////////   
			// Player left group.
        case RQ_GroupLeave:{
			LOG << "* PAK = 80\r\n";
			
			ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 520 ], SYSTEM_COLOR, false );
			
			list< GroupPlayUI::GroupMember > emptyMemberList;
			GroupPlayUI::GetInstance()->UpdateMembers( emptyMemberList, false );
			
                           }break;
			
			//////////////////////////////////////////////////////////////////////////////////////////   
			// Group got disbanded.
        case RQ_NotifyGroupDisband:{
			LOG << "* PAK = 82\r\n";
			
			ChatterUI::GetInstance()->AddBackscrollItem( "", 
				g_LocalString[ 519 ], SYSTEM_COLOR, true );
			
			list< GroupPlayUI::GroupMember > emptyMemberList;
			GroupPlayUI::GetInstance()->UpdateMembers( emptyMemberList, false );
			
                                   }break;
			//////////////////////////////////////////////////////////////////////////////////////////
			// Receive the group members.
        case RQ_UpdateGroupMembers:{
			LOG << "* PAK = 76\r\n";
			TFCPacket *msg = Msg;
			
			WORD groupSize = 0;
			
			BYTE bAutoShare = 0;
			
			GET_BYTE( bAutoShare );
			
			GET_WORD( groupSize );
			
			list< GroupPlayUI::GroupMember > memberList;
			
			WORD i;
			for( i = 0; i < groupSize; i++ ){
				GroupPlayUI::GroupMember member;
				BYTE *userName;
				BYTE  leader;
				
				GET_LONG( member.id );
				GET_WORD( member.level );
				GET_WORD( member.hpPercent );
				GET_BYTE( leader );
				
				if( leader != 0 ){
					member.leader = true;
				}else{
					member.leader = false;
				}
				
				GET_STRING( userName );
				
				LOG << "\r\nGroupUserName=" << (char*)userName;
				
				member.name = reinterpret_cast< const char * >( userName );
				
				memberList.push_back( member );
				
				delete userName;
			}
			
			GroupPlayUI::GetInstance()->UpdateMembers( memberList, bAutoShare == 0 ? false : true );
			
                                   }break;
			//////////////////////////////////////////////////////////////////////////////////////////
			// Received a group invitation.
        case RQ_GroupInvite:{
			LOG << "* PAK = 78\r\n";
			
			TFCPacket *msg = Msg;
			
			DWORD sourceID = 0;
			BYTE *sourceName;
			
			GET_LONG( sourceID );
			GET_STRING( sourceName );
			
			GroupPlayUI::GetInstance()->Join( sourceID, reinterpret_cast< const char * >( sourceName ) );
			
			delete sourceName;       
                            } break;
			
			//////////////////////////////////////////////////////////////////////////////////////////   
        case 75: {//RQ_GetChatterChannelList
			// Get Chatter List
			LOG << "* PAK = 75\r\n";
			
			short wChannelQuantity = 0;
			int i = 0, j = 0;
			BYTE *channel;
			BYTE bListen;
			
			TFCPacket *msg = Msg;
			
			// Get number of channels.
			GET_WORD( wChannelQuantity );
			
			vector< ChatterUI::Channel > channelList( wChannelQuantity );
			
			for (j = 0; j < wChannelQuantity; j++) {
				
				GET_STRING( channel );
				GET_BYTE  ( bListen );
				
				LOG.Lock();
				LOG << "\r\nGot channel " << (char *)channel << " listen = " << bListen;
				LOG.Unlock();
				
				ChatterUI::Channel chan;
				chan.channelID   = reinterpret_cast< const char * >( channel );
				chan.listenState = bListen == 0 ? false : true;
				
				channelList[ j ] = chan;
				
				delete channel;
			}
			
			ChatterUI::GetInstance()->UpdateChannels( channelList );
                 } break;
			
			//////////////////////////////////////////////////////////////////////////////////////////   
        case 50: {//RQ_GetChatterUserList
			// Get Chatter User List
			LOG << "* PAK = 50\r\n";
			
			short wUserQuantity = 0;
			short Len = 0;
			int i = 0, j = 0;
			BYTE *szChannel;
			BYTE *szUser;
			BYTE *szTitle;
			BYTE bListen;
			
			// Get Channel
			TFCPacket *msg = Msg;		
			
			GET_STRING( szChannel );
			
			// Get User
			GET_WORD( wUserQuantity );
			
			vector< ChatterUI::User > users( wUserQuantity );
			
			for (j = 0; j < wUserQuantity; j++) {
				GET_STRING( szUser );
				GET_STRING( szTitle );
				GET_BYTE( bListen );  // Is user listening.
				
				LOG << "\r\nGOT USER " << (char *)szUser << " listen = " << bListen;
				
				ChatterUI::User user;
				user.userName    = reinterpret_cast< const char * >( szUser );
				user.title       = reinterpret_cast< const char * >( szTitle );
				user.listenState = bListen == 0 ? false : true;
				
				users[ j ] = user;
				
				delete szUser;
				delete szTitle;
			}
			
			if( ChatterUI::GetInstance()->GetSelectedChannel() == reinterpret_cast< const char * >( szChannel ) )
			{
				ChatterUI::GetInstance()->UpdateChannelUsers( users );
			}
			
			delete szChannel;
			
                 } break;
			
        case 48: {//RQ_AddRemoveChatterChannel
			// Last Chatter Set didn't work.
			LOG << "* PAK = 48\r\n";
			
                 } break;
			
        case 49: {//RQ_SendChatterChannelMessage
			// Receive Chatter Message.
			LOG << "* PAK = 49\r\n";
			
			char *szChannel;
			char *szSender;
			char *szMessage;
			short Len = 0;
			int i = 0;
			
			// Get Channel
			Msg->Get((short *)&Len);
			szChannel = new char [Len+1];
			for (i = 0; i < Len; i++)
				Msg->Get((char *)&szChannel[i]);
			szChannel[Len] = 0;
			
			// Get Sender
			Msg->Get((short *)&Len);
			szSender = new char [Len+1];
			for (i = 0; i < Len; i++)
				Msg->Get((char *)&szSender[i]);
			szSender[Len] = 0;
			
			// Get Message
			Msg->Get((short *)&Len);
			szMessage = new char [Len+1];
			for (i = 0; i < Len; i++)
				Msg->Get((char *)&szMessage[i]);
			szMessage[Len] = 0;
			
			// Form a readable string for backlog.
			string backMsg( "[\"CC " );
			backMsg += szChannel;
			backMsg += "\"] \"";
			backMsg += szSender;
			backMsg += "\": ";
			backMsg += szMessage;
			
			DWORD color = ChatterUI::GetInstance()->GetChannelColor( szChannel );
			
			
			ChatterUI::GetInstance()->AddBackscrollItem( szSender, backMsg, color, true, false );
			
			delete szSender;
			delete szChannel;
			delete szMessage;
			
                 } break;
			
        case 73: {//Unknown message ??
			LOG << "PAK = 73 {\r\n";
			
			WORD TYPE;
			BYTE StringNumber;
			
			Msg->Get((short *)&TYPE);
			
			switch (TYPE) {
			case 240: StringNumber = 192; break;//no more potion/healing potion// you cannot find the object
			case 241: StringNumber = 191; break;
			case 244: StringNumber = 193; break;
			default:  StringNumber = 197; break;
			}
			
			ChatterUI::GetInstance()->AddBackscrollItem(
				"",
				g_LocalString[StringNumber],
				RGB(255,255,255),
				true
				);
			
			LOG << "PAK = 73 }\r\n";
                 } break;
			
        case 20: {//RQ_ExitGame
			g_App.SetError(1, g_LocalString[500]);//server shutdown message ^^
			g_App.Close(g_LocalString[500]);
			COMM.SetAlive(15000);
                 } break;
			
        case 70: {
			// Missing Unit
			LOG << "PAK = 70 {\r\n";
			
			DWORD ID;
			WORD  TYPE;
			
			Msg->Get((long *)&ID);
			Msg->Get((short *)&TYPE);
			
			LOG << "PAK = 70, " << ID << ", " << TYPE << "\r\n";
			
			if (TYPE == 68) {
				// Packet Type == QueryPuppetInfo
				
			} 
			else if (TYPE == 24) //RQ_Attack
			{
				// Packet Type == Attack
				Objects.Lock(51);
				if (Objects.CheckDead(ID) && !Objects.CheckDelete(ID)) 
				{
					
					Objects.PlAttack(ID, 0);
					Objects.GetMainObject()->Chose = 1;
					Objects.GetMainObject()->Speed = 0;
					Objects.GetMainObject()->AttSpeed = 0;
					signed char X, Y;
					Objects.Found(&X, &Y, ID);
					if (X < 0 && Y == 0)
						Objects.GetMainObject()->Direction = 4;
					else if (X > 0 && Y == 0)
						Objects.GetMainObject()->Direction = 6;
					else if (X == 0 && Y > 0)
						Objects.GetMainObject()->Direction = 2;
					else if (X == 0 && Y < 0)
						Objects.GetMainObject()->Direction = 8;
					else if (X < 0 && Y > 0)
						Objects.GetMainObject()->Direction = 1;
					else if (X < 0 && Y < 0)
						Objects.GetMainObject()->Direction = 7;
					else if (X > 0 && Y > 0)
						Objects.GetMainObject()->Direction = 3;
					else if (X > 0 && Y < 0)
						Objects.GetMainObject()->Direction = 9;
					else 
						Objects.GetMainObject()->Direction = 1;
					
					//sprintf(strtoto,"===If Missing [%d]\n",ID);
					//OutputDebugString(strtoto);
					if (Objects.SetMissing(ID, TRUE)) 
					{
						//sprintf(strtoto,"===Missing go delete[%d]\n",ID);
						//OutputDebugString(strtoto);
						//WORD Type = Objects.GetType(ID);
						
						//if (((Type >= 10000 && Type <= 15000) || (Type >= 20000 && Type <= 25000)) && Type != 10011 && Type != 10012) 
						//{
						//   Type += 5000;
						//   Objects.ChangeType(ID, Type);
						//ChatterUI::GetInstance()->AddBackscrollItem("",g_LocalString[195],RGB(0,100,255),true);
						//} 
						//else 
						{
							//if (Objects.CheckDead(ID) && !Objects.CheckDelete(ID)) 
							//{
							//sprintf(strtoto,"===Before Delete delete[%d]\n",ID);
							//OutputDebugString(strtoto);
							Objects.SetMissing(ID, FALSE);
							//sprintf(strtoto,"===After Delete delete[%d]\n",ID);
							//OutputDebugString(strtoto);
							//}
						}
					}
					//sprintf(strtoto,"===End Missing [%d]\n",ID);
					//OutputDebugString(strtoto);
				}
				Objects.Unlock(51);
			} 
			else if (TYPE == 11) //RQ_GetObject
			{
				if (Objects.FoundID(ID)) 
				{
					ChatterUI::GetInstance()->AddBackscrollItem("",g_LocalString[196],RGB(0,100,255),true);//"The object is broken and cannot be used."
					Objects.Lock(52);
					Objects.Delete(ID);
					Objects.Unlock(52);
				}else{
					//ChatterUI::GetInstance()->AddBackscrollItem("","Uh ? Something is wrong with getting this object, it was not found in your object list.",RGB(0,100,255),true);
				}
			} 
			else 
			{
				// Something Else.
				if (Objects.CheckDead(ID) && !Objects.CheckDelete(ID)) 
				{
					// 				 ChatterUI::GetInstance()->AddBackscrollItem("",g_LocalString[197],RGB(0,100,255),true);//"{You cannot find the object you are looking for.}"
					Objects.Lock(53);	
					Objects.SetMissing(ID, TRUE);
					Objects.Unlock(53);
				}else{
					//ChatterUI::GetInstance()->AddBackscrollItem("","Uh ? Something is wrong with this object, but no idea what.",RGB(0,100,255),true);
				}
			}
			
			LOG << "PAK = 70 }\r\n";
                 } break;
				 
        case 69: PacketHandling::SetUnitStat(Msg); break;
			
        case 64: {//RQ_SpellEffect
			WORD SPELLID;
			DWORD CASTER, TARGET;
			SHORT TPOSX, TPOSY;
			SHORT CPOSX, CPOSY;
			signed char x2, y2;
			DWORD Follow = NULL;
			DWORD SummonID = 0;
			bool EvilSpell = false;
			DWORD spellEffectId;
			DWORD spellChildId;
			
			LOG << "* PAK = 64 {\r\n";
			
			Msg->Get((short *)&SPELLID);
			Msg->Get((long *)&CASTER);
			Msg->Get((long *)&TARGET);
			Msg->Get((short *)&TPOSX);
			Msg->Get((short *)&TPOSY);
			Msg->Get((short *)&CPOSX);
			Msg->Get((short *)&CPOSY);
			Msg->Get((long *)&spellEffectId);  // The spell effect's unique ID.
			Msg->Get((long *)&spellChildId);  // The spell effect's child effect ID.
			
			
			LOG << "SPELL ID [" << SPELLID << "]\r\n";
			LOG << "EFFCT ID [" << spellEffectId << "]\r\n";
			LOG << "CHILD ID [" << spellChildId << "]\r\n";
			
			LOG << "Original Data: " << TARGET << ", " << CASTER << ", " << TPOSX << ", " << TPOSY << ", " << CPOSX << ", " << CPOSY << ", " << Player.xPos << ", " << Player.yPos << "\r\n";
			
			if ( Player.ID == CASTER && TARGET != Player.ID) {
				TalkToID = NULL;
			}
			
			World.Lock(54);
			Objects.Lock(1000);
			
			switch (SPELLID) 
			{
			case __SPELL_SMALLEXPLOSION:
			case __SPELL_GREATEXPLOSION:
			case __SPELL_FLAK2:
			case __SPELL_FLAK:
			case __SPELL_PENTACLE:
			case __SPELL_SHOCKWAVE:
			case __SPELL_GREATBOLT:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_GREATBOLT_AZURE:
			case __SPELL_GREATBOLT_CRIMSON:
			case __SPELL_GREATBOLT_LIME:
			case __SPELL_GREATBOLT_EMERALD:
			case __SPELL_GREATBOLT_GREEN:
			case __SPELL_GREATBOLT_CANDY:
			case __SPELL_GREATBOLT_RED:
			case __SPELL_GREATBOLT_BLACK:
				// END DIALSOFT NEW SPELLS
			case __SPELL_CURSE:
			case __SPELL_ELECTRICSHIELD:
			case __SPELL_SEMIBIGEXPLOSION:
			case __SPELL_FREEZE:
			case __SPELL_ROCKFLY:
			case __SPELL_ICECLOUD:
			case __SPELL_POISONCLOUD:
			case __SPELL_ENTANGLE:
			case __SPELL_GLACIER:
			case __SPELL_GATEWAY_1:
			case __SPELL_METEORS:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_METEOR_BLACK:
			case __SPELL_METEOR_WHITE:
			case __SPELL_METEOR_PURPLE:
			case __SPELL_METEOR_COBOLT:
			case __SPELL_METEOR_GREEN:
			case __SPELL_METEOR_CANDY:
			case __SPELL_METEOR_GOLD:
			case __SPELL_METEOR_BROWN:
			case __SPELL_METEOR_DARK:
			case __SPELL_METEOR_BLUE:
			case __SPELL_METEOR_VIOLET:
				// END DIALSOFT NEW SPELLS
				
			case __SPELL_BOULDERS: 
			case __SPELL_FIRECIRCLE: 
			case __SPELL_FIREBALL_CIRCLE:
				EvilSpell = true;
			case __SPELL_BLESS:
			case __SPELL_HEALING:
			case __SPELL_SERAPH_MALE:
			case __SPELL_SERAPH_FEMALE:
			case __SPELL_SERAPH:
			case __SPELL_HEALSERIOUS:
			case __SPELL_GREENWIPE:
			case __SPELL_BLUEWIPE:
			case __SPELL_FIREWIPE:
			case __SPELL_REDWIPE:
				//Dialsoft new spells
			case __SPELL_MEDIUM_HEALING:
			case __SPELL_ICE_TREE:
			case __SPELL_SUPRAHEAL:
			case __SPELL_SKILLS_EFFECT:
			case __SPELL_LEVELUP:
			case __SPELL_NEW_LIGHTNING1:
			case __SPELL_NEW_FIRE1:
			case __SPELL_NEW_POISON1:
			case __SPELL_NEW_SUPRAHEAL://BLBL tentative d'ajouter deux nouveaux sorts depuis les DDA
			case	__SPELL_NEW_BOULDERS://BLBL tentative d'ajouter deux nouveaux sorts depuis les DDA
				
				//Dialsoft end
				LOG << "* DBG = SpellCasting 1, " << TARGET << ", " << CASTER << ", " << TPOSX << ", " << TPOSY << ", " << CPOSX << ", " << CPOSY << "\r\n";
				if (!TARGET && !TPOSX && !TPOSY)
					TARGET = CASTER;
				
				if (TARGET) 
				{
					if (Player.ID == TARGET) 
					{
						CPOSX = Player.xPos;
						CPOSY = Player.yPos;
					} 
					else 
					{
						if (Objects.Found(&x2, &y2, TARGET)) 
						{
							CPOSX = x2 + Player.xPos;
							CPOSY = y2 + Player.yPos;
						} 
						else 
						{
							CPOSX = TPOSX;
							CPOSY = TPOSY;
						}
					} 
					TPOSX = 0;
					TPOSY = 0;
					Follow = TARGET;
				} else {
					CPOSX = TPOSX;
					CPOSY = TPOSY;
					TPOSX = 0;
					TPOSY = 0;
				} 
				break;
				
			case __SPELL_FIREBALL:
			case __SPELL_FIREBOLT:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_FIREBOLT_BLACK:
			case __SPELL_FIREBOLT_WHITE:
			case __SPELL_FIREBOLT_YELLOW:
			case __SPELL_FIREBOLT_GREEN:
			case __SPELL_FIREBOLT_BLUE:
			case __SPELL_FIREBOLT_PURPLE:
				// END DIALSOFT NEW SPELLS
				
			case __SPELL_LIGHTNINGBOLT:
			case __SPELL_ICESHARD:
			case __SPELL_STONESHARD:
			case __SPELL_SWORD01:
			case __SPELL_POISONARROW:
			case __SPELL_CURSE_WITH_RED_BALL:
			case __SPELL_FIREWIPE_WITH_RED_BALL:
			case __SPELL_FLAK_WITH_RED_BALL:
			case __SPELL_FLAK2_WITH_RED_BALL:
			case __SPELL_GREATEXPLOSION_WITH_RED_BALL:
			case __SPELL_METEOR_WITH_RED_BALL:
			case __SPELL_PENTACLE_WITH_RED_BALL:
			case __SPELL_SEMIBIGEXPLOSION_WITH_RED_BALL:
			case __SPELL_SMALLEXPLOSION_WITH_RED_BALL:
			case __SPELL_FIRECIRCLE_WITH_RED_BALL:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_METEOR_BLACK_WITH_RED_BALL:
			case __SPELL_METEOR_WHITE_WITH_RED_BALL:
			case __SPELL_METEOR_PURPLE_WITH_RED_BALL:
			case __SPELL_METEOR_COBOLT_WITH_RED_BALL:
			case __SPELL_METEOR_GREEN_WITH_RED_BALL:
			case __SPELL_METEOR_CANDY_WITH_RED_BALL:
			case __SPELL_METEOR_GOLD_WITH_RED_BALL:
			case __SPELL_METEOR_BROWN_WITH_RED_BALL:
			case __SPELL_METEOR_DARK_WITH_RED_BALL:
			case __SPELL_METEOR_BLUE_WITH_RED_BALL:
			case __SPELL_METEOR_VIOLET_WITH_RED_BALL:
			case __SPELL_GREATBOLT_AZURE_WITH_RED_BALL:
			case __SPELL_GREATBOLT_CRIMSON_WITH_RED_BALL:
			case __SPELL_GREATBOLT_LIME_WITH_RED_BALL:
			case __SPELL_GREATBOLT_EMERALD_WITH_RED_BALL:
			case __SPELL_GREATBOLT_GREEN_WITH_RED_BALL:
			case __SPELL_GREATBOLT_CANDY_WITH_RED_BALL:
			case __SPELL_GREATBOLT_RED_WITH_RED_BALL:
			case __SPELL_GREATBOLT_BLACK_WITH_RED_BALL:
				// END DIALSOFT NEW SPELLS
				
			case __SPELL_FREEZE_WITH_BLUE_BALL:
			case __SPELL_GLACIER_WITH_BLUE_BALL:
			case __SPELL_GREATBOLT_WITH_BLUE_BALL:
			case __SPELL_ICECLOUD_WITH_BLUE_BALL:
			case __SPELL_POISONCLOUD_WITH_BLUE_BALL:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_METEOR_WITH_BLUE_BALL:
			case __SPELL_METEOR_BLACK_WITH_BLUE_BALL:
			case __SPELL_METEOR_WHITE_WITH_BLUE_BALL:
			case __SPELL_METEOR_PURPLE_WITH_BLUE_BALL:
			case __SPELL_METEOR_COBOLT_WITH_BLUE_BALL:
			case __SPELL_METEOR_GREEN_WITH_BLUE_BALL:
			case __SPELL_METEOR_CANDY_WITH_BLUE_BALL:
			case __SPELL_METEOR_GOLD_WITH_BLUE_BALL:
			case __SPELL_METEOR_BROWN_WITH_BLUE_BALL:
			case __SPELL_METEOR_DARK_WITH_BLUE_BALL:
			case __SPELL_METEOR_BLUE_WITH_BLUE_BALL:
			case __SPELL_METEOR_VIOLET_WITH_BLUE_BALL:
			case __SPELL_GREATBOLT_AZURE_WITH_BLUE_BALL:
			case __SPELL_GREATBOLT_CRIMSON_WITH_BLUE_BALL:
			case __SPELL_GREATBOLT_LIME_WITH_BLUE_BALL:
			case __SPELL_GREATBOLT_EMERALD_WITH_BLUE_BALL:
			case __SPELL_GREATBOLT_GREEN_WITH_BLUE_BALL:
			case __SPELL_GREATBOLT_CANDY_WITH_BLUE_BALL:
			case __SPELL_GREATBOLT_RED_WITH_BLUE_BALL:
			case __SPELL_GREATBOLT_BLACK_WITH_BLUE_BALL:
				// END DIALSOFT NEW SPELLS
				
			case __SPELL_FREEZE_WITH_YELLOW_BALL:
			case __SPELL_GREATBOLT_WITH_YELLOW_BALL:
			case __SPELL_ICECLOUD_WITH_YELLOW_BALL:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_METEOR_WITH_YELLOW_BALL:
			case __SPELL_METEOR_BLACK_WITH_YELLOW_BALL:
			case __SPELL_METEOR_WHITE_WITH_YELLOW_BALL:
			case __SPELL_METEOR_PURPLE_WITH_YELLOW_BALL:
			case __SPELL_METEOR_COBOLT_WITH_YELLOW_BALL:
			case __SPELL_METEOR_GREEN_WITH_YELLOW_BALL:
			case __SPELL_METEOR_CANDY_WITH_YELLOW_BALL:
			case __SPELL_METEOR_GOLD_WITH_YELLOW_BALL:
			case __SPELL_METEOR_BROWN_WITH_YELLOW_BALL:
			case __SPELL_METEOR_DARK_WITH_YELLOW_BALL:
			case __SPELL_METEOR_BLUE_WITH_YELLOW_BALL:
			case __SPELL_METEOR_VIOLET_WITH_YELLOW_BALL:
			case __SPELL_GREATBOLT_AZURE_WITH_YELLOW_BALL:
			case __SPELL_GREATBOLT_CRIMSON_WITH_YELLOW_BALL:
			case __SPELL_GREATBOLT_LIME_WITH_YELLOW_BALL:
			case __SPELL_GREATBOLT_EMERALD_WITH_YELLOW_BALL:
			case __SPELL_GREATBOLT_GREEN_WITH_YELLOW_BALL:
			case __SPELL_GREATBOLT_CANDY_WITH_YELLOW_BALL:
			case __SPELL_GREATBOLT_RED_WITH_YELLOW_BALL:
			case __SPELL_GREATBOLT_BLACK_WITH_YELLOW_BALL:
				// END DIALSOFT NEW SPELLS
				
			case __SPELL_BOULDERS_WITH_GREEN_BALL:
			case __SPELL_ENTANGLE_WITH_GREEN_BALL:
			case __SPELL_FLAK_WITH_GREEN_BALL:
			case __SPELL_FLAK2_WITH_GREEN_BALL:
			case __SPELL_PENTACLE_WITH_GREEN_BALL:
			case __SPELL_POISONCLOUD_WITH_GREEN_BALL:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_METEOR_WITH_GREEN_BALL:
			case __SPELL_METEOR_BLACK_WITH_GREEN_BALL:
			case __SPELL_METEOR_WHITE_WITH_GREEN_BALL:
			case __SPELL_METEOR_PURPLE_WITH_GREEN_BALL:
			case __SPELL_METEOR_COBOLT_WITH_GREEN_BALL:
			case __SPELL_METEOR_GREEN_WITH_GREEN_BALL:
			case __SPELL_METEOR_CANDY_WITH_GREEN_BALL:
			case __SPELL_METEOR_GOLD_WITH_GREEN_BALL:
			case __SPELL_METEOR_BROWN_WITH_GREEN_BALL:
			case __SPELL_METEOR_DARK_WITH_GREEN_BALL:
			case __SPELL_METEOR_BLUE_WITH_GREEN_BALL:
			case __SPELL_METEOR_VIOLET_WITH_GREEN_BALL:
			case __SPELL_GREATBOLT_AZURE_WITH_GREEN_BALL:
			case __SPELL_GREATBOLT_CRIMSON_WITH_GREEN_BALL:
			case __SPELL_GREATBOLT_LIME_WITH_GREEN_BALL:
			case __SPELL_GREATBOLT_EMERALD_WITH_GREEN_BALL:
			case __SPELL_GREATBOLT_GREEN_WITH_GREEN_BALL:
			case __SPELL_GREATBOLT_CANDY_WITH_GREEN_BALL:
			case __SPELL_GREATBOLT_RED_WITH_GREEN_BALL:
			case __SPELL_GREATBOLT_BLACK_WITH_GREEN_BALL:
				// END DIALSOFT NEW SPELLS
				
			case __SPELL_FLAK_WITH_WHITE_BALL:
			case __SPELL_FLAK2_WITH_WHITE_BALL:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_METEOR_WITH_WHITE_BALL:
			case __SPELL_METEOR_BLACK_WITH_WHITE_BALL:
			case __SPELL_METEOR_WHITE_WITH_WHITE_BALL:
			case __SPELL_METEOR_PURPLE_WITH_WHITE_BALL:
			case __SPELL_METEOR_COBOLT_WITH_WHITE_BALL:
			case __SPELL_METEOR_GREEN_WITH_WHITE_BALL:
			case __SPELL_METEOR_CANDY_WITH_WHITE_BALL:
			case __SPELL_METEOR_GOLD_WITH_WHITE_BALL:
			case __SPELL_METEOR_BROWN_WITH_WHITE_BALL:
			case __SPELL_METEOR_DARK_WITH_WHITE_BALL:
			case __SPELL_METEOR_BLUE_WITH_WHITE_BALL:
			case __SPELL_METEOR_VIOLET_WITH_WHITE_BALL:
			case __SPELL_GREATBOLT_AZURE_WITH_WHITE_BALL:
			case __SPELL_GREATBOLT_CRIMSON_WITH_WHITE_BALL:
			case __SPELL_GREATBOLT_LIME_WITH_WHITE_BALL:
			case __SPELL_GREATBOLT_EMERALD_WITH_WHITE_BALL:
			case __SPELL_GREATBOLT_GREEN_WITH_WHITE_BALL:
			case __SPELL_GREATBOLT_CANDY_WITH_WHITE_BALL:
			case __SPELL_GREATBOLT_RED_WITH_WHITE_BALL:
			case __SPELL_GREATBOLT_BLACK_WITH_WHITE_BALL:
				// END DIALSOFT NEW SPELLS
				
			case __SPELL_CURSE_WITH_BLACK_BALL:
			case __SPELL_ENTANGLE_WITH_BLACK_BALL:
			case __SPELL_GREATEXPLOSION_WITH_BLACK_BALL:
			case __SPELL_METEOR_WITH_BLACK_BALL:
			case __SPELL_SEMIBIGEXPLOSION_WITH_BLACK_BALL:
			case __SPELL_SMALLEXPLOSION_WITH_BLACK_BALL:
			case __SPELL_FIRECIRCLE_WITH_BLACK_BALL:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_METEOR_BLACK_WITH_BLACK_BALL:
			case __SPELL_METEOR_WHITE_WITH_BLACK_BALL:
			case __SPELL_METEOR_PURPLE_WITH_BLACK_BALL:
			case __SPELL_METEOR_COBOLT_WITH_BLACK_BALL:
			case __SPELL_METEOR_GREEN_WITH_BLACK_BALL:
			case __SPELL_METEOR_CANDY_WITH_BLACK_BALL:
			case __SPELL_METEOR_GOLD_WITH_BLACK_BALL:
			case __SPELL_METEOR_BROWN_WITH_BLACK_BALL:
			case __SPELL_METEOR_DARK_WITH_BLACK_BALL:
			case __SPELL_METEOR_BLUE_WITH_BLACK_BALL:
			case __SPELL_METEOR_VIOLET_WITH_BLACK_BALL:
			case __SPELL_GREATBOLT_AZURE_WITH_BLACK_BALL:
			case __SPELL_GREATBOLT_CRIMSON_WITH_BLACK_BALL:
			case __SPELL_GREATBOLT_LIME_WITH_BLACK_BALL:
			case __SPELL_GREATBOLT_EMERALD_WITH_BLACK_BALL:
			case __SPELL_GREATBOLT_GREEN_WITH_BLACK_BALL:
			case __SPELL_GREATBOLT_CANDY_WITH_BLACK_BALL:
			case __SPELL_GREATBOLT_RED_WITH_BLACK_BALL:
			case __SPELL_GREATBOLT_BLACK_WITH_BLACK_BALL:
				// END DIALSOFT NEW SPELLS
				
			case __SPELL_FLAK_WITH_PURPLE_BALL:
			case __SPELL_FLAK2_WITH_PURPLE_BALL:
			case __SPELL_GREATEXPLOSION_WITH_PURPLE_BALL:
			case __SPELL_METEOR_WITH_PURPLE_BALL:
			case __SPELL_PENTACLE_WITH_PURPLE_BALL:
			case __SPELL_POISONCLOUD_WITH_PURPLE_BALL:
			case __SPELL_SEMIBIGEXPLOSION_WITH_PURPLE_BALL:
			case __SPELL_SMALLEXPLOSION_WITH_PURPLE_BALL:
			case __SPELL_FIRECIRCLE_WITH_PURPLE_BALL:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_METEOR_BLACK_WITH_PURPLE_BALL:
			case __SPELL_METEOR_WHITE_WITH_PURPLE_BALL:
			case __SPELL_METEOR_PURPLE_WITH_PURPLE_BALL:
			case __SPELL_METEOR_COBOLT_WITH_PURPLE_BALL:
			case __SPELL_METEOR_GREEN_WITH_PURPLE_BALL:
			case __SPELL_METEOR_CANDY_WITH_PURPLE_BALL:
			case __SPELL_METEOR_GOLD_WITH_PURPLE_BALL:
			case __SPELL_METEOR_BROWN_WITH_PURPLE_BALL:
			case __SPELL_METEOR_DARK_WITH_PURPLE_BALL:
			case __SPELL_METEOR_BLUE_WITH_PURPLE_BALL:
			case __SPELL_METEOR_VIOLET_WITH_PURPLE_BALL:
			case __SPELL_GREATBOLT_AZURE_WITH_PURPLE_BALL:
			case __SPELL_GREATBOLT_CRIMSON_WITH_PURPLE_BALL:
			case __SPELL_GREATBOLT_LIME_WITH_PURPLE_BALL:
			case __SPELL_GREATBOLT_EMERALD_WITH_PURPLE_BALL:
			case __SPELL_GREATBOLT_GREEN_WITH_PURPLE_BALL:
			case __SPELL_GREATBOLT_CANDY_WITH_PURPLE_BALL:
			case __SPELL_GREATBOLT_RED_WITH_PURPLE_BALL:
			case __SPELL_GREATBOLT_BLACK_WITH_PURPLE_BALL:
				// END DIALSOFT NEW SPELLS
				
			case __SPELL_NEWFIREBALL:
			case __SPELL_NEWFIREBALL2:
				// BEGIN DIALSOFT NEW SPELLS
			case __SPELL_NEWFIREBALL_BLACK:
			case __SPELL_NEWFIREBALL_WHITE:
			case __SPELL_NEWFIREBALL_AZURE:
			case __SPELL_NEWFIREBALL_MAGENTA:
			case __SPELL_NEWFIREBALL_YELLOW:
			case __SPELL_NEWFIREBALL_PURPLE:
				// END DIALSOFT NEW SPELLS
				
			case __SPELL_ELECTRICSHIELD_WITH_PURPLE_BALL:
				EvilSpell = true;
			case __SPELL_CURSE_WITH_PURPLE_BALL:
			case __SPELL_RED_ENERGY_BALL:
			case __SPELL_BLUE_ENERGY_BALL:
			case __SPELL_YELLOW_ENERGY_BALL:
			case __SPELL_GREEN_ENERGY_BALL:
			case __SPELL_WHITE_ENERGY_BALL:
			case __SPELL_BLACK_ENERGY_BALL:
			case __SPELL_PURPLE_ENERGY_BALL:
			case __SPELL_GATEWAY_1_WITH_RED_BALL:
			case __SPELL_REDWIPE_WITH_RED_BALL:
			case __SPELL_BLUEWIPE_WITH_BLUE_BALL:
			case __SPELL_CURSE_WITH_BLUE_BALL:
			case __SPELL_ELECTRICSHIELD_WITH_BLUE_BALL:
			case __SPELL_GREENWIPE_WITH_BLUE_BALL:
			case __SPELL_HEALING_WITH_BLUE_BALL:
			case __SPELL_HEALSERIOUS_WITH_BLUE_BALL:
			case __SPELL_BLESS_WITH_YELLOW_BALL:
			case __SPELL_BLUEWIPE_WITH_YELLOW_BALL:
			case __SPELL_CURSE_WITH_YELLOW_BALL:
			case __SPELL_ELECTRICSHIELD_WITH_YELLOW_BALL:
			case __SPELL_GATEWAY_1_WITH_YELLOW_BALL:
			case __SPELL_CURSE_WITH_GREEN_BALL:
			case __SPELL_GREENWIPE_WITH_GREEN_BALL:
			case __SPELL_ROCKFLY_WITH_GREEN_BALL:
			case __SPELL_BLESS_WITH_WHITE_BALL:
			case __SPELL_BLUEWIPE_WITH_WHITE_BALL:
			case __SPELL_CURSE_WITH_WHITE_BALL:
			case __SPELL_ELECTRICSHIELD_WITH_WHITE_BALL:
			case __SPELL_HEALING_WITH_WHITE_BALL:
			case __SPELL_HEALSERIOUS_WITH_WHITE_BALL:
			case __SPELL_ICECLOUD_WITH_WHITE_BALL:
			case __SPELL_FIREWIPE_WITH_BLACK_BALL:
			case __SPELL_GREENWIPE_WITH_BLACK_BALL:
			case __SPELL_REDWIPE_WITH_BLACK_BALL:
			case __SPELL_BLUEWIPE_WITH_PURPLE_BALL:
			case __SPELL_GATEWAY_1_WITH_PURPLE_BALL:
			case __SPELL_GREENWIPE_WITH_PURPLE_BALL:
				// Drake5
				if (TARGET) 
				{
					Follow = TARGET;
					if (Player.ID == TARGET) 
					{
						TPOSX = Player.xPos;
						TPOSY = Player.yPos;
					} 
					else 
					{
						if (Objects.Found((signed char *)&x2, (signed char *)&y2, TARGET)) 
						{
							TPOSX = x2 + Player.xPos;
							TPOSY = y2 + Player.yPos;
						}
					}
				}
				
				LOG << "SPELL FX: " << CASTER << ", " << CPOSX << ", " << CPOSY << "\r\n";
				if (CASTER) 
				{
					if (Player.ID == CASTER && !(TPOSX == Player.xPos && TPOSY == Player.yPos)) {
						CPOSX = Player.xPos;
						CPOSY = Player.yPos;
						Objects.GetMainObject()->Direction = 1;
						int a = ((TPOSX-Player.xPos)+11)*3;
						int b = ((TPOSY-Player.yPos)+16)*2;
						if (b > 30) 
						{
							if (a > 30) 
							{
								a -= 30;
								b -= 30;
								if (a > b*2) 
								{
									Objects.GetMainObject()->Direction = 6;
								} 
								else if (b > a*2) 
								{
									Objects.GetMainObject()->Direction = 2;
								} 
								else 
								{
									Objects.GetMainObject()->Direction = 3;	
								}
							} 
							else 
							{
								b -= 30;
								a = 30 - a;
								if (a > b*2) 
								{
									Objects.GetMainObject()->Direction = 4;
								} 
								else if (b > a*2) 
								{
									Objects.GetMainObject()->Direction = 2;
								} 
								else 
								{
									Objects.GetMainObject()->Direction = 1;
								}
							}
						} 
						else 
						{
							if (a > 30) 
							{
								a -= 30;
								b = 30 - b;
								if (a > b*2) 
								{
									Objects.GetMainObject()->Direction = 6;
								} 
								else if (b > a*2) 
								{
									Objects.GetMainObject()->Direction = 8;
								} 
								else 
								{
									Objects.GetMainObject()->Direction = 9;
								}
							} 
							else 
							{
								a = 30 - a;
								b = 30 - b;
								if (a > b*2) 
								{
									Objects.GetMainObject()->Direction = 4;
								} 
								else if (b > a*2) 
								{
									Objects.GetMainObject()->Direction = 8;
								} 
								else 
								{
									Objects.GetMainObject()->Direction = 7;
								}
							}			
						}
					} 
					else 
					{
						if (Objects.Found((signed char *)&x2, (signed char *)&y2, CASTER)) {
							CPOSX = x2 + Player.xPos;
							CPOSY = y2 + Player.yPos;
							Objects.SetDirection(CASTER, TARGET, true);
						}
					}
				}
				break;
      }
      
      switch (SPELLID) {
      case __SPELL_FIREBALL_CIRCLE:
		  {
			  float radius = 7;
			  int nbProjectile = 20;
			  
			  double pi = 3.141592654;
			  static int iAngle = 0; //DaP
			  double angle = (float)iAngle;
			  iAngle++;            
			  for (int ite = 0; ite < nbProjectile; ite++) 
			  {
				  Objects.Add(SpellID, __SPELL_FIREBALL, CPOSX, CPOSY, 0, 0, Follow, 100, 2, false, spellEffectId, spellChildId);
				  Objects.MoveObject(SpellID, (long)(CPOSX+( cos( angle ) * radius )), (long)(CPOSY+( sin( angle ) * radius )), TRUE, SummonID, 100, SPELLID, 0, 100);
				  SpellID--;
				  angle += (pi*2)/(float)nbProjectile;
			  }
		  } break;
      default:
		  LOG << "* DBG = SpellCasting 2, " << TARGET << ", " << CASTER << ", " << TPOSX << ", " << TPOSY << ", " << CPOSX << ", " << CPOSY << ", " << Follow << "\r\n";
		  
		  
		  Objects.Add(SpellID, SPELLID, CPOSX, CPOSY, 0, 0, Follow, 100, 2, false, spellEffectId, spellChildId);
		  if (TPOSX && TPOSY) {
			  switch (SPELLID) {
			  case __SPELL_FIREBALL:      SummonID = __SPELL_GREATEXPLOSION;  break;
			  case __SPELL_FIREBOLT:      SummonID = __SPELL_SMALLEXPLOSION;  break;
				  // BEGIN DIALSOFT NEW SPELLS
			  case __SPELL_FIREBOLT_BLACK:		SummonID = __SPELL_SMALLEXPLOSION;  break;
			  case __SPELL_FIREBOLT_WHITE:			SummonID = __SPELL_SMALLEXPLOSION;  break;
			  case __SPELL_FIREBOLT_YELLOW:		SummonID = __SPELL_SMALLEXPLOSION;  break;
			  case __SPELL_FIREBOLT_GREEN:		SummonID = __SPELL_SMALLEXPLOSION;  break;
			  case __SPELL_FIREBOLT_BLUE:			SummonID = __SPELL_SMALLEXPLOSION;  break;
			  case __SPELL_FIREBOLT_PURPLE:		SummonID = __SPELL_SMALLEXPLOSION;  break;
				  // END DIALSOFT NEW SPELLS
			  case __SPELL_NEWFIREBALL:   SummonID = __SPELL_GREATEXPLOSION;  break;
			  case __SPELL_NEWFIREBALL2:  SummonID = __SPELL_GREATEXPLOSION;  break;
				  // BEGIN DIALSOFT NEW SPELLS
			  case __SPELL_NEWFIREBALL_BLACK:   SummonID = __SPELL_GREATEXPLOSION;  break;
			  case __SPELL_NEWFIREBALL_WHITE:   SummonID = __SPELL_GREATEXPLOSION;  break;
			  case __SPELL_NEWFIREBALL_AZURE:   SummonID = __SPELL_GREATEXPLOSION;  break;
			  case __SPELL_NEWFIREBALL_MAGENTA:   SummonID = __SPELL_GREATEXPLOSION;  break;
			  case __SPELL_NEWFIREBALL_YELLOW:   SummonID = __SPELL_GREATEXPLOSION;  break;
			  case __SPELL_NEWFIREBALL_PURPLE:   SummonID = __SPELL_GREATEXPLOSION;  break;
				  // END DIALSOFT NEW SPELLS
				  
			  case __SPELL_LIGHTNINGBOLT: SummonID = __SPELL_ELECTRICSHIELD;  break;
			  case __SPELL_ICESHARD:      SummonID = __SPELL_ICECLOUD;        break;
			  case __SPELL_STONESHARD:    SummonID = __SPELL_ROCKFLY;         break;
			  case __SPELL_POISONARROW:   SummonID = __SPELL_POISONCLOUD;     break;
			  case __SPELL_BLESS_WITH_YELLOW_BALL:				SummonID = __SPELL_BLESS;  break;
			  case __SPELL_BLESS_WITH_WHITE_BALL:					SummonID = __SPELL_BLESS;  break;
			  case __SPELL_BLUEWIPE_WITH_BLUE_BALL:				SummonID = __SPELL_BLUEWIPE;  break;
			  case __SPELL_BLUEWIPE_WITH_YELLOW_BALL:			SummonID = __SPELL_BLUEWIPE;  break;
			  case __SPELL_BLUEWIPE_WITH_WHITE_BALL:				SummonID = __SPELL_BLUEWIPE;  break;
			  case __SPELL_BLUEWIPE_WITH_PURPLE_BALL:			SummonID = __SPELL_BLUEWIPE;  break;
			  case __SPELL_BOULDERS_WITH_GREEN_BALL:				SummonID = __SPELL_BOULDERS;  break;
			  case __SPELL_CURSE_WITH_RED_BALL:					SummonID = __SPELL_CURSE;  break;
			  case __SPELL_CURSE_WITH_BLUE_BALL:					SummonID = __SPELL_CURSE;  break;
			  case __SPELL_CURSE_WITH_YELLOW_BALL:				SummonID = __SPELL_CURSE;  break;
			  case __SPELL_CURSE_WITH_GREEN_BALL:					SummonID = __SPELL_CURSE;  break;
			  case __SPELL_CURSE_WITH_WHITE_BALL:					SummonID = __SPELL_CURSE;  break;
			  case __SPELL_CURSE_WITH_BLACK_BALL:					SummonID = __SPELL_CURSE;  break;
			  case __SPELL_CURSE_WITH_PURPLE_BALL:				SummonID = __SPELL_CURSE;  break;
			  case __SPELL_ELECTRICSHIELD_WITH_BLUE_BALL:		SummonID = __SPELL_ELECTRICSHIELD;  break;
			  case __SPELL_ELECTRICSHIELD_WITH_YELLOW_BALL:	SummonID = __SPELL_ELECTRICSHIELD;  break;
			  case __SPELL_ELECTRICSHIELD_WITH_WHITE_BALL:		SummonID = __SPELL_ELECTRICSHIELD;  break;
			  case __SPELL_ELECTRICSHIELD_WITH_PURPLE_BALL:	SummonID = __SPELL_ELECTRICSHIELD;  break;
			  case __SPELL_ENTANGLE_WITH_GREEN_BALL:				SummonID = __SPELL_ENTANGLE;  break;
			  case __SPELL_ENTANGLE_WITH_BLACK_BALL:				SummonID = __SPELL_ENTANGLE;  break;
			  case __SPELL_FIREWIPE_WITH_RED_BALL:				SummonID = __SPELL_FIREWIPE;  break;
			  case __SPELL_FIREWIPE_WITH_BLACK_BALL:				SummonID = __SPELL_FIREWIPE;  break;
			  case __SPELL_FLAK_WITH_RED_BALL:						SummonID = __SPELL_FLAK;  break;
			  case __SPELL_FLAK_WITH_GREEN_BALL:					SummonID = __SPELL_FLAK;  break;
			  case __SPELL_FLAK_WITH_WHITE_BALL:					SummonID = __SPELL_FLAK;  break;
			  case __SPELL_FLAK_WITH_PURPLE_BALL:					SummonID = __SPELL_FLAK;  break;
			  case __SPELL_FLAK2_WITH_RED_BALL:					SummonID = __SPELL_FLAK2;  break;
			  case __SPELL_FLAK2_WITH_GREEN_BALL:					SummonID = __SPELL_FLAK2;  break;
			  case __SPELL_FLAK2_WITH_WHITE_BALL:					SummonID = __SPELL_FLAK2;  break;
			  case __SPELL_FLAK2_WITH_PURPLE_BALL:				SummonID = __SPELL_FLAK2;  break;
			  case __SPELL_FREEZE_WITH_BLUE_BALL:					SummonID = __SPELL_FREEZE;  break;
			  case __SPELL_FREEZE_WITH_YELLOW_BALL:				SummonID = __SPELL_FREEZE;  break;
			  case __SPELL_GATEWAY_1_WITH_RED_BALL:				SummonID = __SPELL_GATEWAY_1;  break;
			  case __SPELL_GATEWAY_1_WITH_YELLOW_BALL:			SummonID = __SPELL_GATEWAY_1;  break;
			  case __SPELL_GATEWAY_1_WITH_PURPLE_BALL:			SummonID = __SPELL_GATEWAY_1;  break;
			  case __SPELL_GLACIER_WITH_BLUE_BALL:				SummonID = __SPELL_GLACIER;  break;
			  case __SPELL_GREATBOLT_WITH_BLUE_BALL:				SummonID = __SPELL_GREATBOLT;  break;
			  case __SPELL_GREATBOLT_WITH_YELLOW_BALL:			SummonID = __SPELL_GREATBOLT;  break;
				  // BEGIN DIALSOFT NEW SPELLS
			  case __SPELL_GREATBOLT_AZURE_WITH_RED_BALL:			SummonID = __SPELL_GREATBOLT_AZURE;  break;
			  case __SPELL_GREATBOLT_CRIMSON_WITH_RED_BALL:		SummonID = __SPELL_GREATBOLT_CRIMSON;  break;
			  case __SPELL_GREATBOLT_LIME_WITH_RED_BALL:			SummonID = __SPELL_GREATBOLT_LIME;  break;
			  case __SPELL_GREATBOLT_EMERALD_WITH_RED_BALL:		SummonID = __SPELL_GREATBOLT_EMERALD;  break;
			  case __SPELL_GREATBOLT_GREEN_WITH_RED_BALL:			SummonID = __SPELL_GREATBOLT_GREEN;  break;
			  case __SPELL_GREATBOLT_CANDY_WITH_RED_BALL:			SummonID = __SPELL_GREATBOLT_CANDY;  break;
			  case __SPELL_GREATBOLT_RED_WITH_RED_BALL:			SummonID = __SPELL_GREATBOLT_RED;  break;
			  case __SPELL_GREATBOLT_BLACK_WITH_RED_BALL:			SummonID = __SPELL_GREATBOLT_BLACK;  break;
			  case __SPELL_GREATBOLT_AZURE_WITH_BLUE_BALL:			SummonID = __SPELL_GREATBOLT_AZURE;  break;
			  case __SPELL_GREATBOLT_CRIMSON_WITH_BLUE_BALL:		SummonID = __SPELL_GREATBOLT_CRIMSON;  break;
			  case __SPELL_GREATBOLT_LIME_WITH_BLUE_BALL:			SummonID = __SPELL_GREATBOLT_LIME;  break;
			  case __SPELL_GREATBOLT_EMERALD_WITH_BLUE_BALL:		SummonID = __SPELL_GREATBOLT_EMERALD;  break;
			  case __SPELL_GREATBOLT_GREEN_WITH_BLUE_BALL:		SummonID = __SPELL_GREATBOLT_GREEN;  break;
			  case __SPELL_GREATBOLT_CANDY_WITH_BLUE_BALL:		SummonID = __SPELL_GREATBOLT_CANDY;  break;
			  case __SPELL_GREATBOLT_RED_WITH_BLUE_BALL:			SummonID = __SPELL_GREATBOLT_RED;  break;
			  case __SPELL_GREATBOLT_BLACK_WITH_BLUE_BALL:		SummonID = __SPELL_GREATBOLT_BLACK;  break;
			  case __SPELL_GREATBOLT_AZURE_WITH_YELLOW_BALL:		SummonID = __SPELL_GREATBOLT_AZURE;  break;
			  case __SPELL_GREATBOLT_CRIMSON_WITH_YELLOW_BALL:	SummonID = __SPELL_GREATBOLT_CRIMSON;  break;
			  case __SPELL_GREATBOLT_LIME_WITH_YELLOW_BALL:		SummonID = __SPELL_GREATBOLT_LIME;  break;
			  case __SPELL_GREATBOLT_EMERALD_WITH_YELLOW_BALL:	SummonID = __SPELL_GREATBOLT_EMERALD;  break;
			  case __SPELL_GREATBOLT_GREEN_WITH_YELLOW_BALL:		SummonID = __SPELL_GREATBOLT_GREEN;  break;
			  case __SPELL_GREATBOLT_CANDY_WITH_YELLOW_BALL:		SummonID = __SPELL_GREATBOLT_CANDY;  break;
			  case __SPELL_GREATBOLT_RED_WITH_YELLOW_BALL:		SummonID = __SPELL_GREATBOLT_RED;  break;
			  case __SPELL_GREATBOLT_BLACK_WITH_YELLOW_BALL:		SummonID = __SPELL_GREATBOLT_BLACK;  break;
			  case __SPELL_GREATBOLT_AZURE_WITH_GREEN_BALL:		SummonID = __SPELL_GREATBOLT_AZURE;  break;
			  case __SPELL_GREATBOLT_CRIMSON_WITH_GREEN_BALL:		SummonID = __SPELL_GREATBOLT_CRIMSON;  break;
			  case __SPELL_GREATBOLT_LIME_WITH_GREEN_BALL:		SummonID = __SPELL_GREATBOLT_LIME;  break;
			  case __SPELL_GREATBOLT_EMERALD_WITH_GREEN_BALL:		SummonID = __SPELL_GREATBOLT_EMERALD;  break;
			  case __SPELL_GREATBOLT_GREEN_WITH_GREEN_BALL:		SummonID = __SPELL_GREATBOLT_GREEN;  break;
			  case __SPELL_GREATBOLT_CANDY_WITH_GREEN_BALL:		SummonID = __SPELL_GREATBOLT_CANDY;  break;
			  case __SPELL_GREATBOLT_RED_WITH_GREEN_BALL:			SummonID = __SPELL_GREATBOLT_RED;  break;
			  case __SPELL_GREATBOLT_BLACK_WITH_GREEN_BALL:		SummonID = __SPELL_GREATBOLT_BLACK;  break;
			  case __SPELL_GREATBOLT_AZURE_WITH_WHITE_BALL:		SummonID = __SPELL_GREATBOLT_AZURE;  break;
			  case __SPELL_GREATBOLT_CRIMSON_WITH_WHITE_BALL:		SummonID = __SPELL_GREATBOLT_CRIMSON;  break;
			  case __SPELL_GREATBOLT_LIME_WITH_WHITE_BALL:		SummonID = __SPELL_GREATBOLT_LIME;  break;
			  case __SPELL_GREATBOLT_EMERALD_WITH_WHITE_BALL:		SummonID = __SPELL_GREATBOLT_EMERALD;  break;
			  case __SPELL_GREATBOLT_GREEN_WITH_WHITE_BALL:		SummonID = __SPELL_GREATBOLT_GREEN;  break;
			  case __SPELL_GREATBOLT_CANDY_WITH_WHITE_BALL:		SummonID = __SPELL_GREATBOLT_CANDY;  break;
			  case __SPELL_GREATBOLT_RED_WITH_WHITE_BALL:			SummonID = __SPELL_GREATBOLT_RED;  break;
			  case __SPELL_GREATBOLT_BLACK_WITH_WHITE_BALL:		SummonID = __SPELL_GREATBOLT_BLACK;  break;
			  case __SPELL_GREATBOLT_AZURE_WITH_BLACK_BALL:		SummonID = __SPELL_GREATBOLT_AZURE;  break;
			  case __SPELL_GREATBOLT_CRIMSON_WITH_BLACK_BALL:		SummonID = __SPELL_GREATBOLT_CRIMSON;  break;
			  case __SPELL_GREATBOLT_LIME_WITH_BLACK_BALL:		SummonID = __SPELL_GREATBOLT_LIME;  break;
			  case __SPELL_GREATBOLT_EMERALD_WITH_BLACK_BALL:		SummonID = __SPELL_GREATBOLT_EMERALD;  break;
			  case __SPELL_GREATBOLT_GREEN_WITH_BLACK_BALL:		SummonID = __SPELL_GREATBOLT_GREEN;  break;
			  case __SPELL_GREATBOLT_CANDY_WITH_BLACK_BALL:		SummonID = __SPELL_GREATBOLT_CANDY;  break;
			  case __SPELL_GREATBOLT_RED_WITH_BLACK_BALL:			SummonID = __SPELL_GREATBOLT_RED;  break;
			  case __SPELL_GREATBOLT_BLACK_WITH_BLACK_BALL:		SummonID = __SPELL_GREATBOLT_BLACK;  break;
			  case __SPELL_GREATBOLT_AZURE_WITH_PURPLE_BALL:		SummonID = __SPELL_GREATBOLT_AZURE;  break;
			  case __SPELL_GREATBOLT_CRIMSON_WITH_PURPLE_BALL:	SummonID = __SPELL_GREATBOLT_CRIMSON;  break;
			  case __SPELL_GREATBOLT_LIME_WITH_PURPLE_BALL:		SummonID = __SPELL_GREATBOLT_LIME;  break;
			  case __SPELL_GREATBOLT_EMERALD_WITH_PURPLE_BALL:	SummonID = __SPELL_GREATBOLT_EMERALD;  break;
			  case __SPELL_GREATBOLT_GREEN_WITH_PURPLE_BALL:		SummonID = __SPELL_GREATBOLT_GREEN;  break;
			  case __SPELL_GREATBOLT_CANDY_WITH_PURPLE_BALL:		SummonID = __SPELL_GREATBOLT_CANDY;  break;
			  case __SPELL_GREATBOLT_RED_WITH_PURPLE_BALL:		SummonID = __SPELL_GREATBOLT_RED;  break;
			  case __SPELL_GREATBOLT_BLACK_WITH_PURPLE_BALL:		SummonID = __SPELL_GREATBOLT_BLACK;  break;
				  // END DIALSOFT NEW SPELLS
				  
			  case __SPELL_GREATEXPLOSION_WITH_RED_BALL:		SummonID = __SPELL_GREATEXPLOSION;  break;
			  case __SPELL_GREATEXPLOSION_WITH_BLACK_BALL:		SummonID = __SPELL_GREATEXPLOSION;  break;
			  case __SPELL_GREATEXPLOSION_WITH_PURPLE_BALL:	SummonID = __SPELL_GREATEXPLOSION;  break;
			  case __SPELL_GREENWIPE_WITH_BLUE_BALL:				SummonID = __SPELL_GREENWIPE;  break;
			  case __SPELL_GREENWIPE_WITH_GREEN_BALL:			SummonID = __SPELL_GREENWIPE;  break;
			  case __SPELL_GREENWIPE_WITH_BLACK_BALL:			SummonID = __SPELL_GREENWIPE;  break;
			  case __SPELL_GREENWIPE_WITH_PURPLE_BALL:			SummonID = __SPELL_GREENWIPE;  break;
			  case __SPELL_HEALING_WITH_BLUE_BALL:				SummonID = __SPELL_HEALING;  break;
			  case __SPELL_HEALING_WITH_WHITE_BALL:				SummonID = __SPELL_HEALING;  break;
			  case __SPELL_HEALSERIOUS_WITH_BLUE_BALL:			SummonID = __SPELL_HEALSERIOUS;  break;
			  case __SPELL_HEALSERIOUS_WITH_WHITE_BALL:			SummonID = __SPELL_HEALSERIOUS;  break;
			  case __SPELL_ICECLOUD_WITH_BLUE_BALL:				SummonID = __SPELL_ICECLOUD;  break;
			  case __SPELL_ICECLOUD_WITH_YELLOW_BALL:			SummonID = __SPELL_ICECLOUD;  break;
			  case __SPELL_ICECLOUD_WITH_WHITE_BALL:				SummonID = __SPELL_ICECLOUD;  break;
			  case __SPELL_METEOR_WITH_RED_BALL:					SummonID = __SPELL_METEORS;  break;
			  case __SPELL_METEOR_WITH_BLACK_BALL:				SummonID = __SPELL_METEORS;  break;
			  case __SPELL_METEOR_WITH_PURPLE_BALL:				SummonID = __SPELL_METEORS;  break;
				  // BEGIN DIALSOFT NEW SPELLS
			  case __SPELL_METEOR_BLACK_WITH_RED_BALL:			SummonID = __SPELL_METEOR_BLACK;  break;
			  case __SPELL_METEOR_WHITE_WITH_RED_BALL:			SummonID = __SPELL_METEOR_WHITE;  break;
			  case __SPELL_METEOR_PURPLE_WITH_RED_BALL:			SummonID = __SPELL_METEOR_PURPLE;  break;
			  case __SPELL_METEOR_COBOLT_WITH_RED_BALL:			SummonID = __SPELL_METEOR_COBOLT;  break;
			  case __SPELL_METEOR_GREEN_WITH_RED_BALL:			SummonID = __SPELL_METEOR_GREEN;  break;
			  case __SPELL_METEOR_CANDY_WITH_RED_BALL:			SummonID = __SPELL_METEOR_CANDY;  break;
			  case __SPELL_METEOR_GOLD_WITH_RED_BALL:				SummonID = __SPELL_METEOR_GOLD;  break;
			  case __SPELL_METEOR_BROWN_WITH_RED_BALL:			SummonID = __SPELL_METEOR_BROWN;  break;
			  case __SPELL_METEOR_DARK_WITH_RED_BALL:				SummonID = __SPELL_METEOR_DARK;  break;
			  case __SPELL_METEOR_BLUE_WITH_RED_BALL:				SummonID = __SPELL_METEOR_BLUE;  break;
			  case __SPELL_METEOR_VIOLET_WITH_RED_BALL:			SummonID = __SPELL_METEOR_VIOLET;  break;
			  case __SPELL_METEOR_WITH_BLUE_BALL:					SummonID = __SPELL_METEORS;  break;
			  case __SPELL_METEOR_BLACK_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_BLACK;  break;
			  case __SPELL_METEOR_WHITE_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_WHITE;  break;
			  case __SPELL_METEOR_PURPLE_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_PURPLE;  break;
			  case __SPELL_METEOR_COBOLT_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_COBOLT;  break;
			  case __SPELL_METEOR_GREEN_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_GREEN;  break;
			  case __SPELL_METEOR_CANDY_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_CANDY;  break;
			  case __SPELL_METEOR_GOLD_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_GOLD;  break;
			  case __SPELL_METEOR_BROWN_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_BROWN;  break;
			  case __SPELL_METEOR_DARK_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_DARK;  break;
			  case __SPELL_METEOR_BLUE_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_BLUE;  break;
			  case __SPELL_METEOR_VIOLET_WITH_BLUE_BALL:			SummonID = __SPELL_METEOR_VIOLET;  break;
			  case __SPELL_METEOR_WITH_YELLOW_BALL:				SummonID = __SPELL_METEORS;  break;
			  case __SPELL_METEOR_BLACK_WITH_YELLOW_BALL:			SummonID = __SPELL_METEOR_BLACK;  break;
			  case __SPELL_METEOR_WHITE_WITH_YELLOW_BALL:			SummonID = __SPELL_METEOR_WHITE;  break;
			  case __SPELL_METEOR_PURPLE_WITH_YELLOW_BALL:		SummonID = __SPELL_METEOR_PURPLE;  break;
			  case __SPELL_METEOR_COBOLT_WITH_YELLOW_BALL:		SummonID = __SPELL_METEOR_COBOLT;  break;
			  case __SPELL_METEOR_GREEN_WITH_YELLOW_BALL:			SummonID = __SPELL_METEOR_GREEN;  break;
			  case __SPELL_METEOR_CANDY_WITH_YELLOW_BALL:			SummonID = __SPELL_METEOR_CANDY;  break;
			  case __SPELL_METEOR_GOLD_WITH_YELLOW_BALL:			SummonID = __SPELL_METEOR_GOLD;  break;
			  case __SPELL_METEOR_BROWN_WITH_YELLOW_BALL:			SummonID = __SPELL_METEOR_BROWN;  break;
			  case __SPELL_METEOR_DARK_WITH_YELLOW_BALL:			SummonID = __SPELL_METEOR_DARK;  break;
			  case __SPELL_METEOR_BLUE_WITH_YELLOW_BALL:			SummonID = __SPELL_METEOR_BLUE;  break;
			  case __SPELL_METEOR_VIOLET_WITH_YELLOW_BALL:		SummonID = __SPELL_METEOR_VIOLET;  break;
			  case __SPELL_METEOR_WITH_GREEN_BALL:				SummonID = __SPELL_METEORS;  break;
			  case __SPELL_METEOR_BLACK_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_BLACK;  break;
			  case __SPELL_METEOR_WHITE_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_WHITE;  break;
			  case __SPELL_METEOR_PURPLE_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_PURPLE;  break;
			  case __SPELL_METEOR_COBOLT_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_COBOLT;  break;
			  case __SPELL_METEOR_GREEN_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_GREEN;  break;
			  case __SPELL_METEOR_CANDY_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_CANDY;  break;
			  case __SPELL_METEOR_GOLD_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_GOLD;  break;
			  case __SPELL_METEOR_BROWN_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_BROWN;  break;
			  case __SPELL_METEOR_DARK_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_DARK;  break;
			  case __SPELL_METEOR_BLUE_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_BLUE;  break;
			  case __SPELL_METEOR_VIOLET_WITH_GREEN_BALL:			SummonID = __SPELL_METEOR_VIOLET;  break;
			  case __SPELL_METEOR_WITH_WHITE_BALL:				SummonID = __SPELL_METEORS;  break;
			  case __SPELL_METEOR_BLACK_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_BLACK;  break;
			  case __SPELL_METEOR_WHITE_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_WHITE;  break;
			  case __SPELL_METEOR_PURPLE_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_PURPLE;  break;
			  case __SPELL_METEOR_COBOLT_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_COBOLT;  break;
			  case __SPELL_METEOR_GREEN_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_GREEN;  break;
			  case __SPELL_METEOR_CANDY_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_CANDY;  break;
			  case __SPELL_METEOR_GOLD_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_GOLD;  break;
			  case __SPELL_METEOR_BROWN_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_BROWN;  break;
			  case __SPELL_METEOR_DARK_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_DARK;  break;
			  case __SPELL_METEOR_BLUE_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_BLUE;  break;
			  case __SPELL_METEOR_VIOLET_WITH_WHITE_BALL:			SummonID = __SPELL_METEOR_VIOLET;  break;
			  case __SPELL_METEOR_BLACK_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_BLACK;  break;
			  case __SPELL_METEOR_WHITE_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_WHITE;  break;
			  case __SPELL_METEOR_PURPLE_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_PURPLE;  break;
			  case __SPELL_METEOR_COBOLT_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_COBOLT;  break;
			  case __SPELL_METEOR_GREEN_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_GREEN;  break;
			  case __SPELL_METEOR_CANDY_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_CANDY;  break;
			  case __SPELL_METEOR_GOLD_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_GOLD;  break;
			  case __SPELL_METEOR_BROWN_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_BROWN;  break;
			  case __SPELL_METEOR_DARK_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_DARK;  break;
			  case __SPELL_METEOR_BLUE_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_BLUE;  break;
			  case __SPELL_METEOR_VIOLET_WITH_BLACK_BALL:			SummonID = __SPELL_METEOR_VIOLET;  break;
			  case __SPELL_METEOR_BLACK_WITH_PURPLE_BALL:			SummonID = __SPELL_METEOR_BLACK;  break;
			  case __SPELL_METEOR_WHITE_WITH_PURPLE_BALL:			SummonID = __SPELL_METEOR_WHITE;  break;
			  case __SPELL_METEOR_PURPLE_WITH_PURPLE_BALL:		SummonID = __SPELL_METEOR_PURPLE;  break;
			  case __SPELL_METEOR_COBOLT_WITH_PURPLE_BALL:		SummonID = __SPELL_METEOR_COBOLT;  break;
			  case __SPELL_METEOR_GREEN_WITH_PURPLE_BALL:			SummonID = __SPELL_METEOR_GREEN;  break;
			  case __SPELL_METEOR_CANDY_WITH_PURPLE_BALL:			SummonID = __SPELL_METEOR_CANDY;  break;
			  case __SPELL_METEOR_GOLD_WITH_PURPLE_BALL:			SummonID = __SPELL_METEOR_GOLD;  break;
			  case __SPELL_METEOR_BROWN_WITH_PURPLE_BALL:			SummonID = __SPELL_METEOR_BROWN;  break;
			  case __SPELL_METEOR_DARK_WITH_PURPLE_BALL:			SummonID = __SPELL_METEOR_DARK;  break;
			  case __SPELL_METEOR_BLUE_WITH_PURPLE_BALL:			SummonID = __SPELL_METEOR_BLUE;  break;
			  case __SPELL_METEOR_VIOLET_WITH_PURPLE_BALL:		SummonID = __SPELL_METEOR_VIOLET;  break;
				  // END DIALSOFT NEW SPELLS
				  
			  case __SPELL_PENTACLE_WITH_RED_BALL:				SummonID = __SPELL_PENTACLE;  break;
			  case __SPELL_PENTACLE_WITH_GREEN_BALL:				SummonID = __SPELL_PENTACLE;  break;
			  case __SPELL_PENTACLE_WITH_PURPLE_BALL:			SummonID = __SPELL_PENTACLE;  break;
			  case __SPELL_POISONCLOUD_WITH_BLUE_BALL:			SummonID = __SPELL_POISONCLOUD;  break;
			  case __SPELL_POISONCLOUD_WITH_GREEN_BALL:			SummonID = __SPELL_POISONCLOUD;  break;
			  case __SPELL_POISONCLOUD_WITH_PURPLE_BALL:		SummonID = __SPELL_POISONCLOUD;  break;
			  case __SPELL_REDWIPE_WITH_RED_BALL:					SummonID = __SPELL_REDWIPE;  break;
			  case __SPELL_REDWIPE_WITH_BLACK_BALL:				SummonID = __SPELL_REDWIPE;  break;
			  case __SPELL_ROCKFLY_WITH_GREEN_BALL:				SummonID = __SPELL_ROCKFLY;  break;
			  case __SPELL_SEMIBIGEXPLOSION_WITH_RED_BALL:		SummonID = __SPELL_SEMIBIGEXPLOSION;  break;
			  case __SPELL_SEMIBIGEXPLOSION_WITH_BLACK_BALL:	SummonID = __SPELL_SEMIBIGEXPLOSION;  break;
			  case __SPELL_SEMIBIGEXPLOSION_WITH_PURPLE_BALL: SummonID = __SPELL_SEMIBIGEXPLOSION;  break;
			  case __SPELL_SMALLEXPLOSION_WITH_RED_BALL:		SummonID = __SPELL_SMALLEXPLOSION;  break;
			  case __SPELL_SMALLEXPLOSION_WITH_BLACK_BALL:		SummonID = __SPELL_SMALLEXPLOSION;  break;
			  case __SPELL_SMALLEXPLOSION_WITH_PURPLE_BALL:	SummonID = __SPELL_SMALLEXPLOSION;  break;
			  case __SPELL_FIRECIRCLE_WITH_RED_BALL:	         SummonID = __SPELL_FIRECIRCLE;  break;
			  case __SPELL_FIRECIRCLE_WITH_BLACK_BALL:        SummonID = __SPELL_FIRECIRCLE;  break;
			  case __SPELL_FIRECIRCLE_WITH_PURPLE_BALL:       SummonID = __SPELL_FIRECIRCLE;  break;
            };
            Objects.MoveObject(SpellID, TPOSX, TPOSY, TRUE, SummonID, 100, SPELLID, 0, 100);
         }
         SpellID--;
      }
      Objects.Unlock(1000);
      World.Unlock(54);
      LOG << "* PAK = 64 }\r\n";
      
      if (EvilSpell && CASTER != Player.ID && TARGET == Player.ID) 
      {
		  Objects.Lock(0);
		  Objects.SetEvil(CASTER);
		  Objects.Unlock(0);
      }
      
   } break;
   //////////////////////////////////////////////////////////////////////////////////////////
   // Arrow hit
    case RQ_ArrowHit:{
		TFCPacket *msg = Msg;
		
		DWORD CASTER = 0;
		DWORD TARGET = 0;
		char  newHp = 0;
		
		GET_LONG( CASTER );
		GET_LONG( TARGET );
		GET_BYTE( newHp );
		
		ShootArrow( CASTER, TARGET, 0, 0, true, newHp );
                     } break;
		//////////////////////////////////////////////////////////////////////////////////////////   
		// Arrow misses.
    case RQ_ArrowMiss:{
		TFCPacket *msg = Msg;
		
		DWORD CASTER;
		SHORT TPOSX, TPOSY;
		BYTE COLLIDE;
		bool collide;
		
		GET_LONG( CASTER );
		GET_WORD( TPOSX );
		GET_WORD( TPOSY );
		GET_BYTE( COLLIDE );
		collide = COLLIDE != 0 ? true : false;
		
		ShootArrow( CASTER, 0, TPOSX, TPOSY, collide, 0 );        
		
                      }; break;
		
    case RQ_TeleportPlayer: PacketHandling::TeleportPlayer(Msg); break;
		
    case 60: //RQ_GetNearItems  
		{
			LOG << "* PAK = 60\r\n";
			if (Code13) {
				Objects.Lock(17);
				Objects.SetSet(TRUE);
				Objects.AddSDObject();
				Objects.AddSDAnimObject();
				Objects.Unlock(17); 
				
				Objects.Lock(19);
				Objects.Direction2 = 1;
				Objects.GetMainObject()->SpriteNumber = 1;
				Objects.Unlock(19);
				
				g_GameMusic.LoadNewSound();
				
				g_Var.inGame = false;
				
				Objects.Lock(13);
				Objects.Direction2 = 1;
				Objects.GetMainObject()->SpriteNumber = 1;
				Objects.Unlock(13);
				
				INGAME = TRUE;
				if (!hDrawThread) {
					DXDClear();
					DXDFlip();
					DXDClear();  //BLBLBL : 26 mars 2009 retiré les appels en doublon
					DXDFlip();   //modif annulée
					hDrawThread = (HANDLE)_beginthreadex(NULL, 0, DrawThread, NULL, CREATE_SUSPENDED, &iDrawThreadID);
					LOG << "* DRW = Begin Thread, 1\r\n";
				}
				if( !hMouseActionThread ){
					hMouseActionThread = (HANDLE)_beginthreadex(NULL, 0, MouseActionThread, NULL, 0, &iMouseActionThreadID);
				}
				
				
				stShow = true; 
				
				LOG << "* DRW = Resume Thread, 1\r\n";
				ResumeThread(hDrawThread);     
				
				g_Pal.SetToBlack();
				
				World.Lock(20);
				World.SetFading = true;
				World.FadeLevel = 0;
				World.Switch();
				World.Unlock(20);
				
				Code13 = false;
				noException = false;
			}
			
			if (EnterGame) {
				TFCPacket Send;
				Send << (short)46; //FromPreInGameToInGame 
				SEND_PACKET(Send);
			}
		} break;
		
    case RQ_Remort://RQ_Seraph: 
		{
			MacroUI::GetInstance()->ResetMacro();
		} 
		break;
		
    case RQ_GetStatus: { 
		
		LOG << "* PAK = 43\r\n";
		
		DWORD High;
		DWORD Low;
		
		// Get the status information.
		Msg->Get((long *)&Player.Hp);
		Msg->Get((long *)&Player.MaxHp);
		Msg->Get((short *)&Player.Mana);
		Msg->Get((short *)&Player.MaxMana);
		Msg->Get((long *)&High);
		Msg->Get((long *)&Low);
		Player.Exp = ((__int64)(High) << 32) + (__int64)(Low);
		Msg->Get((short *)&Player.bAC );
		Msg->Get((short *)&Player.AC );
		Msg->Get((short *)&Player.bStr);
		Msg->Get((short *)&Player.bEnd);
		Msg->Get((short *)&Player.bAgi);
		Msg->Get((short *)&Player.bWil);
		Msg->Get((short *)&Player.bWis);
		Msg->Get((short *)&Player.bInt);
		Msg->Get((short *)&Player.bLck);
		Msg->Get((short *)&Player.StatsPts);
		Msg->Get((short *)&Player.Str);
		Msg->Get((short *)&Player.End);
		Msg->Get((short *)&Player.Agi);
		Msg->Get((short *)&Player.Wil);
		Msg->Get((short *)&Player.Wis);
		Msg->Get((short *)&Player.Int);
		Msg->Get((short *)&Player.Lck);
		Msg->Get((short *)&Player.Level);
		Msg->Get((short *)&Player.SkillPts );
		Msg->Get((short *)&Player.Weight );
		Msg->Get((short *)&Player.MaxWeight );
		Msg->Get((short *)&Player.Karma );
		Msg->Get((short *)&Player.TrueMaxHp );
		
		Msg->Get((short*)&Player.Power.Water);
		Msg->Get((short*)&Player.Power.Earth);
		Msg->Get((short*)&Player.Power.Air);
		Msg->Get((short*)&Player.Power.Fire);
		Msg->Get((short*)&Player.Power.Light);
		Msg->Get((short*)&Player.Power.Dark);
		Msg->Get((short*)&Player.Resist.Water);
		Msg->Get((short*)&Player.Resist.Earth);
		Msg->Get((short*)&Player.Resist.Air);
		Msg->Get((short*)&Player.Resist.Fire);
		Msg->Get((short*)&Player.Resist.Light);
		Msg->Get((short*)&Player.Resist.Dark);
		//Msg->Get((short *)&Player.Crime );
		//Msg->Get((short *)&Player.Honor );
		
		/*StatsBox *lpStatsBox = static_cast< StatsBox *>( GraphBoxHandler::GetBox( GWIDBOX_STATS ) );
		// If no stat box is currently displayed
		if( lpStatsBox == NULL ){
		// Create and register a new one
		lpStatsBox = new StatsBox( 0 );						
		GraphBoxHandler::RegisterBox( lpStatsBox );
		lpStatsBox->Lock();
		}
		
		  lpStatsBox->AddPacketStats( Msg );
		lpStatsBox->Unlock();*/
		
		// Update the character sheet.
		CharacterUI::GetInstance()->UpdateCharacterSheet();
		InventoryUI::GetInstance()->UpdateCharacterSheet();
		
       }
		break;

    case RQ_GetPvpRanking: // by Asteryth PVP RANKING : RQ_GetPvpRanking 131 
		{ 
		
		LOG << "* PAK = 131\r\n";

		// Get the status information.
		Msg->Get((long *)&Player.TotalDeathNumber);
		Msg->Get((long *)&Player.TotalKillNumber);
		Msg->Get((long *)&Player.CurrentDeathNumber);
		Msg->Get((long *)&Player.CurrentKillNumber);
		Msg->Get((long *)&Player.BestSerieMurder);
		Msg->Get((long *)&Player.CurrentSerieMurder);
		Msg->Get((long *)&Player.CurrentPvpPoint);
        } break;
		
		
	case 53: {//RQ_GoldChange
		LOG << "* PAK = 53";
		long Gold;
		Msg->Get((long *)&Gold);
		if (g_SaveGame.GetOptionsParam()->bEnableDisplayGold) // steph ajout
		{ // steph ajout
			if (Gold > Player.Gold && Player.Gold != -1) { // steph activation
				char Temp[1000];
				sprintf(Temp, g_LocalString[87], Gold - Player.Gold);

				ChatterUI::GetInstance()->AddBackscrollItem("", Temp, SYSTEM_COLOR, true);
			} // steph activation
		} // steph ajout
		Player.Gold = Gold;
		InventoryUI::GetInstance()->UpdateGold();
		LOG << ", " << Player.Gold;
		LOG << "\r\n";
	} break;
		
		
    case RQ_Page: {
		LOG << "* PAK = 29 {\r\n";
		//       BOOL IgnoreUser = FALSE;
		int iPageType;
		char szText[1024];
		char szName[256];
		DWORD dwColor; 
		
		iPageType = 1;
		
		bool confirm = false;
		short Len;
		memset(szText, 0, 1024);
		
		Msg->Get((short *)&Len);
		for (int i = 0; i < Len; i++)
			Msg->Get((char *)&szName[i]);
		
		szName[Len] = 0;
		
		if (Len == 0) {
			iPageType = 2;
		}
		
		Msg->Get((short *)&Len);           
		for (int j = 0; j < Len; j++) 
			Msg->Get((char *)&szText[j]);
		
		szText[Len] = 0;
		
		if (!szName[0] && Len) {
			// Mean that is a page confirmation
			sprintf(szName, g_LocalString[89], szText);
			int iColor = RGB(255, 255, 100);
			if( !ChatterUI::GetInstance()->IsUserIgnored( szText )) 
			{
				if (g_SaveGame.GetOptionsParam()->bPageSound) 
				{
					SoundFX[44].Play(FALSE);
				}
				ChatterUI::GetInstance()->AddBackscrollItem( szText, szName, iColor, true, false );
			}
		} 
		else if( !ChatterUI::GetInstance()->IsUserIgnored( szName ) )
		{
			if (g_SaveGame.GetOptionsParam()->bPageSound) 
			{
				SoundFX[44].Play(FALSE);
			}
			
			
			dwColor = RGB(255, 0, 255);
			
			if (Len == 0) 
				iPageType = 3;
			
			char Temp[1024];
			int iColor;
			bool show = true;
			if (iPageType == 1) { // Someone pages you
				sprintf(Temp, g_LocalString[88], szName, szText);
				iColor = RGB(197, 197, 197);
				ClientConfig *pConfig = g_Global.GetClientConfig();
				if (pConfig->dwAfkStatus)
				{
					// So we won't flood if we are ignored or pages self,
					if( (strcmp( szText, g_LocalString[ 502 ] ) != 0 ) && (stricmp( szText, pConfig->strAfkMessage ))) 
					{
						TFCPacket sending;
						sending << (RQ_SIZE)RQ_Page;
						sending << (short)strlen( szName );
						sending << (char *)szName;
						sending << (short)strlen( pConfig->strAfkMessage );
						sending << (char *)pConfig->strAfkMessage;
						
						SEND_PACKET( sending );
					}
				}
			}
			if (iPageType == 2) {
				sprintf(Temp, g_LocalString[89], szText);
				strcpy( szName, szText );
				iColor = RGB(255, 255, 100);
				//show = false;
			}
			if (iPageType == 3) { // User not found
				sprintf(Temp, g_LocalString[90]);
				iColor = RGB(100, 255, 255);
			}
			
			if( show ){
				ChatterUI::GetInstance()->AddBackscrollItem( szName, Temp, iColor, true, false );
			}
		}else{
			// If the received text is the ignore text, do not resend
			// another ignoring page (avoid loops).
			if( strcmp( szText, g_LocalString[ 502 ] ) != 0 ){
				TFCPacket sending;
				sending << (RQ_SIZE)RQ_Page;
				sending << (short)strlen( szName );
				sending << (char *)szName;
				sending << (short)strlen( g_LocalString[ 502 ] );
				sending << (char *)g_LocalString[ 502 ];
				
				SEND_PACKET( sending ); 
			}
		}
		
		//LOG << "ID(" << ") Text Color (" << dwColor << ") Text Len (" << Len << ") Text (" << ")";
		
		LOG << "PAK = 29 }\r\n";
                  } break;
                  
    case RQ_Shout: break;
		
    case RQ_FromPreInGameToInGame: 
		
		if( EnterGame )
		{
			PacketHandling::FromPreInGameToInGame(Msg); 
			if( g_SaveGame.GetOptionsParam()->bSeraphAnim && Custom.gFirstConnect && Player.isSeraph)
			{ 
				if (goodSeraph) 
				{
					ConnectSoundFX.Create("Seraph", TS_MEMORY);
					ConnectSoundFX.Play(FALSE);
					Custom.gFirstConnect = FALSE;
				} 
				else 
				{
					ConnectSoundFX.Create("Evil Seraph", TS_MEMORY);
					ConnectSoundFX.Play(FALSE);
					Custom.gFirstConnect = FALSE;
				}
			}
		}
		break;
		
    case 47: {//RQ_YouDied
		
             } break;
		
    case 37: {//RQ_LevelUp
		LOG << "* PAK = 37";
		unsigned long High, Low;
		unsigned long oldLevel = Player.Level;
		
		Msg->Get((short *)&Player.Level);
		Msg->Get((long *)&High);
		Msg->Get((long *)&Low);
		Msg->Get((long *)&Player.Hp);
		Msg->Get((long *)&Player.MaxHp);
		Msg->Get((short *)&Player.Mana);
		Msg->Get((short *)&Player.MaxMana);
		// hp, max hp, mana, max mana
		Player.ExpLastLevel = Player.Exp2Go;
		Player.Exp2Go = ((__int64)(High) << 32) + (__int64)(Low);
		
		if (oldLevel < Player.Level) 
		{
			LevelUp = true;
			LastAsk3 = timeGetTime();
			//++NMNMNM --[Add Spell levelUP]
			Objects.Lock(0);
			Objects.Add(SpellID, __SPELL_LEVELUP, Player.xPos, Player.yPos, 100, 0, NULL, 100);
			SpellID--;
			Objects.Unlock(0);
		}
		
		//LOG << "Level (" << Player.Level << ") Xp2Go (" << (int)Player.Exp2Go << ")";
		LOG << "\r\n";
             } break;
		
    case 44: {//RQ_XPchanged
		LOG << "* PAK = 44 {";
		/*       unsigned long High, Low;
		
		  Msg->Get((long *)&High);
		  Msg->Get((long *)&Low);
		  XPStatValue+=(((__int64)(High) << 32) + (__int64)Low)-XPStatLastValue;//XPSTAT
		  XPStatLastValue=Player.Exp;//XPSTAT
		Player.Exp = ((__int64)(High) << 32) + (__int64)Low;*/
		unsigned long High, Low;
		
		Msg->Get((long *)&High);
		Msg->Get((long *)&Low);
		__int64 XpVal=((__int64)(High) << 32) + (__int64)Low;
		XPStatValue+=XpVal-XPStatLastValue;
		XPStatLastValue=Player.Exp;
		Player.Exp = XpVal;
		
		LOG << "* PAK = 44, " << High << ", " << Low << "\r\n";
		LOG << "* PAK = 44 }\r\n";
             } break;
		//////////////////////////////////////////////////////////////////////////////////////////
		// Server messages
    case RQ_GetOnlinePlayerList:{
		
                                } break;
		
		//////////////////////////////////////////////////////////////////////////////////////////
		// Server messages
    case RQ_ServerMessage:{
		LOG << "* PAK = 63";
		
		// Retreive the width and height of the message box.
		WORD wX = 0, wY = 0;
		WORD wI;
		WORD wLen;
		char *lpbText;
		DWORD color = RGB( 0, 100, 255 );
		
		LOG << "\r\nRECEIVE SERVER MESSAGE\r\n";
		
		Msg->Get( (short *)&wX );
		Msg->Get( (short *)&wY );
		Msg->Get( (short *)&wLen );
		
		LOG << "PAK = 63, " << wX << "\r\n";
		LOG << "PAK = 63, " << wLen << "\r\n";
		if( wLen > 0 ){
			lpbText = new char[ wLen + 1 ];
			
			for( wI = 0; wI < wLen; wI++ ){
				Msg->Get( (char *)&lpbText[ wI ] );
			}
			lpbText[ wI ] = 0;
			
			//LOG << "--" << lpbText;
			LOG << "SERVER MESSAGE: [" << lpbText << "]\r\n";
			
			Msg->Get((DWORD *)&color);
			
			ChatterUI::GetInstance()->AddBackscrollItem( "", lpbText, color, true );
			delete lpbText;
		}      
		LOG << "\r\n";
		
                          } break;
		
		////////////////////////
		// NightMare -- Add 2005-03-17 (Update Ophiste code by NightMare)
		// Add Weather packet info and change state of weather...
		// Now Support only Rain
    case RQ_WeatherMsg:
		{
			LOG << "* PAK = 104";
			
			enum { OFF, ON };
			
			short value;
			bool bValue;
			DWORD dwEffect;
			
			Msg->Get( (long *)&dwEffect ); 
			Msg->Get( (short *)&value );
			
			switch(value)
			{
			case OFF:
				bValue = false;
				break;
			case ON:
				bValue = true;
				break;
			default:
				bValue = false;
				break;
			}
			
			if(dwEffect == 1)
			{
				Custom.bEnableGameRain = bValue;
				CWeather::GetInstance()->SetRainState( bValue );
			}
			else if(dwEffect == 2)
			{
				Custom.bEnableGameSnow = bValue;
				CWeather::GetInstance()->SetSnowState( bValue );
			}
			else if(dwEffect == 3)
			{
				Custom.bEnableGameFog = bValue;
			}
			
			LOG << "\r\n";
		}
		break;
		// END NM
		/////////////////////////
		
		//////////////////////////////////////////////////////////////////////////////////////////
		// Info messages
    case RQ_OpenURL:{
		
		LOG << "\r\nRECEIVED OpenURL MESSAGE\r\n";
		
		char *lpbText;
		WORD wLen;
		Msg->Get( (short *)&wLen );
		
		if( wLen > 0 ){
			lpbText = new char[ wLen + 1 + 7 ];
			strcpy(lpbText, "http://");
			int wI;
			for( wI = 0; wI < wLen; wI++ ){
				Msg->Get( (char *)&lpbText[ wI + 7 ] );
			}
			lpbText[ wI + 7 ] = 0;
			
			ShellExecute(NULL, "open", lpbText, NULL, NULL, SW_MAXIMIZE);
			delete lpbText;
		}
		
                    } break;
    case RQ_InfoMessage:
		{
			LOG << "* PAK = 102";
			
			// Retreive the width and height of the message box.
			WORD wI;
			WORD wLen;
			char *lpbText;
			DWORD type;
			DWORD color;
			
			LOG << "\r\nRECEIVE INFO MESSAGE\r\n";
			
			Msg->Get( (long *)&type ); // TODO: implement a way to allow user to disable displaying of some message types
			Msg->Get( (long *)&color );
			Msg->Get( (short *)&wLen );
			
			LOG << "PAK = 102, " << wLen << "\r\n";
			if( wLen > 0 )
			{
				lpbText = new char[ wLen + 1 ];
				
				for( wI = 0; wI < wLen; wI++ )
				{
					Msg->Get( (char *)&lpbText[ wI ] );
				}
				lpbText[ wI ] = 0;
				
				//LOG << "--" << lpbText;
				LOG << "INFO MESSAGE: [" << lpbText << "]\r\n";
				
				ChatterUI::GetInstance()->AddBackscrollItem( "", lpbText, color, true );
				delete lpbText;
			}      
			LOG << "\r\n";
		} 
		break;
		
		
		//////////////////////////////////////////////////////////////////////////////////////////				
		// Update equipment list.
    case RQ_ViewEquiped: {
		LOG << "* PAK = 19";
		
		char rangedAttack = 0;
		Msg->Get( (char *)&rangedAttack );
		
		if( rangedAttack != 0 )
		{
			Player.rangedAttack = true;
		}
		else
		{ 
			Player.rangedAttack = false;
		}
		
		// Get the packet information.
		GetEquipItem( body, Msg );
		GetEquipItem( gloves, Msg );
		GetEquipItem( helm, Msg );
		GetEquipItem( legs, Msg );
		GetEquipItem( bracelets, Msg );
		GetEquipItem( necklace, Msg );
		GetEquipItem( weapon_right, Msg );
		GetEquipItem( weapon_left, Msg );
		GetEquipItem( ring1, Msg );
		GetEquipItem( ring2, Msg );
		GetEquipItem( belt, Msg );
		GetEquipItem( sleeves, Msg );
		GetEquipItem( feet, Msg );
		
		// Synchronize the inventory UI.
		InventoryUI::GetInstance()->SyncEquipment();
		
		// Lock the root box since this call came from some unknown thread.
		//CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );
		
		// Update the old stats box.
		//StatsBox::PacketEquip( Msg );
		
                         } break;
		
		//////////////////////////////////////////////////////////////////////////////////////////						
		// Train skill list
    case RQ_SendTrainSkillList: {
		LOG << "* PAK = 40\r\n";
		
		LOG << "* PAK = 56\r\n";
		
		TFCPacket *msg = Msg;
		
		DWORD currentGold = 0;
		WORD nbItems = 0;
		std::list< SkillTrainUI::SellItemInfo > itemList;
		
		WORD skillPnts;
		
		GET_WORD( skillPnts );
		GET_WORD( nbItems );
		
		// Get the item list from the packet.
		WORD i;
		for( i = 0; i < nbItems; i++ ){
			LPBYTE name;                
			SkillTrainUI::SellItemInfo trainInfo;
			
			BYTE canHave = 0;
			
			GET_BYTE( canHave );	        // TRUE if skill can be trained.
			trainInfo.canHave = canHave != 0 ? true : false;
			GET_WORD( trainInfo.skillID );	// ID of the skill.
			GET_WORD( trainInfo.qty );		// Current skill strength.
			
			GET_WORD( trainInfo.maxQty );	// Maximum points this NPC can train.
			GET_LONG( trainInfo.price );	// Cost of skill training per skill points.
			
			GET_STRING( name );		// Length of skill name
			
			trainInfo.initialQty = trainInfo.qty;
			
			trainInfo.name = reinterpret_cast< const char *>( name );
			itemList.push_back( trainInfo );
			
			delete name;
		}
		SkillTrainUI::GetInstance()->Train( skillPnts, itemList );
                                } break;
		
		//////////////////////////////////////////////////////////////////////////////////////////						
		// Teach skill list
    case RQ_SendTeachSkillList:{
		LOG << "* PAK = 55\r\n";
		
		TFCPacket *msg = Msg;
		DWORD currentGold = 0;
		WORD nbItems = 0;
		std::list< SkillTeachUI::SellItemInfo > itemList;
		
		WORD skillPnts;
		
		GET_WORD( skillPnts );
		GET_WORD( nbItems );
		
		// Get the item list from the packet.
		WORD i;
		for( i = 0; i < nbItems; i++ ){
			LPBYTE name;
			LPBYTE req;
			SkillTeachUI::SellItemInfo trainInfo;
			
			BYTE canHave = 0;
			
			GET_BYTE( canHave );	        // TRUE if skill can be trained.
			trainInfo.canHave = canHave != 0 ? true : false;
			GET_WORD( trainInfo.skillID );	// ID of the skill.
			GET_LONG( trainInfo.price );	// Cost of skill training per skill points.
			GET_STRING( name );		        // Length of skill name
			GET_STRING( req );
			GET_LONG( trainInfo.skillPnts );
			GET_LONG( trainInfo.icon );
			
			
			LOG << "\r\nGot skill " << (char*)name << "reqs: " << (char*)req << ".";
			
			trainInfo.name = reinterpret_cast< const char *>( name );
			trainInfo.reqs = reinterpret_cast< const char *>( req );
			itemList.push_back( trainInfo );
			
			delete req;
			delete name;
		}
		SkillTeachUI::GetInstance()->Train( skillPnts, itemList );
		
                               } break;
		
		//////////////////////////////////////////////////////////////////////////////////////////
		// Sent by server to update the client's time
    case RQ_GetTime: {
		LOG << "* PAK = 45";
		Msg->Get((char *)&g_TimeStructure.Seconde);
		Msg->Get((char *)&g_TimeStructure.Minute);
		Msg->Get((char *)&g_TimeStructure.Hour);
		Msg->Get((char *)&g_TimeStructure.Day);
		Msg->Get((char *)&g_TimeStructure.Week);
		Msg->Get((char *)&g_TimeStructure.Month);
		Msg->Get((short *)&g_TimeStructure.Year);
		
		LOG << "\r\n";
		//OutputDebugString("Received GetTime Pak");//BLBLBL 10/12/2010
                     } break;				
		//////////////////////////////////////////////////////////////////////////////////////////
		// Receive skill list for skill use.
    case RQ_GetSkillList:{
		LOG << "* PAK = 39\r\n";
		// First update the Player.tlUserSkill list.
		WORD skillQuantity = 0;
		
		// For the GET_ macros to work.
		TFCPacket *msg = Msg;
		
		GET_WORD( skillQuantity );
		
		Player.Lock();
		
		// Destroy the previous user skills.
		Player.tlUserSkill.ToHead();
		while( Player.tlUserSkill.QueryNext() ){
			delete Player.tlUserSkill.GetObject()->bName;
			
			Player.tlUserSkill.DeleteObject();
		}
		
		
		// Get all the skills in the packet.
		WORD i;
		for( i = 0; i < skillQuantity; i++ ){
			WORD skillID;
			BYTE skillUse;
			WORD skillPnts;
			WORD truePnts;
			BYTE *skillName;
			BYTE *skillDesc;
			
			GET_WORD( skillID );
			GET_BYTE( skillUse );
			GET_WORD( skillPnts );
			GET_WORD( truePnts );
			GET_STRING( skillName );
			GET_STRING( skillDesc );
			
			// Add new user skill for checking
			LPUSER_SKILL lpUserSkill = new USER_SKILL;
			
			lpUserSkill->bName      = skillName;
			lpUserSkill->desc       = reinterpret_cast< char * >( skillDesc );
			lpUserSkill->wID        = skillID;
			lpUserSkill->dwStrength = skillPnts;
			lpUserSkill->bUse       = skillUse;
			lpUserSkill->dwTrueStrength = truePnts;
			
			delete skillDesc;
			
			Player.tlUserSkill.AddToTail( lpUserSkill );
		}
		
		Player.Unlock();
		
		CharacterUI::GetInstance()->UpdateSkills();
                         } break;
		//////////////////////////////////////////////////////////////////////////////////////////
		// Received a shop buying request
    case RQ_SendBuyItemList:{
		LOG << "* PAK = 41\r\n";
		TFCPacket *msg = Msg;
		
		DWORD currentGold = 0;
		WORD nbItems = 0;
		std::list< BuyUI::SellItemInfo > itemList;
		
		GET_LONG( currentGold );
		GET_WORD( nbItems     );
		// Get the item list from the packet.
		WORD i;
		for( i = 0; i < nbItems; i++ ){
			LPBYTE name;
			LPBYTE req;
			BuyUI::SellItemInfo sellInfo;                
			
			sellInfo.dwID = 0;
			GET_WORD  ( sellInfo.dwID );
			GET_WORD  ( sellInfo.appearance );
			GET_LONG  ( sellInfo.price );
			BYTE bEquip = 0;
			GET_BYTE  ( bEquip );
			GET_STRING( name );                
			GET_STRING( req );
			
			if( bEquip != 0 ){
				sellInfo.canEquip = true;
			}else{
				sellInfo.canEquip = false;
			}
			
			sellInfo.name = reinterpret_cast< const char *>( name );
			sellInfo.reqs = reinterpret_cast< const char *>( req );
			itemList.push_back( sellInfo );
			
			delete req;
			delete name;
		}
		BuyUI::GetInstance()->Buy( itemList );
                            } break;
		//////////////////////////////////////////////////////////////////////////////////////////
		// Received a shop selling request
    case RQ_SendSellItemList:{
		LOG << "* PAK = 56\r\n";
		
		TFCPacket *msg = Msg;
		
		DWORD currentGold = 0;
		WORD nbItems = 0;
		std::list< SellUI::SellItemInfo > itemList;
		
		GET_LONG( currentGold );
		GET_WORD( nbItems     );
		// Get the item list from the packet.
		WORD i;
		for( i = 0; i < nbItems; i++ ){
			LPBYTE name;                
			SellUI::SellItemInfo sellInfo;
			
			GET_LONG  ( sellInfo.dwID );
			GET_WORD  ( sellInfo.appearance );
			GET_LONG  ( sellInfo.price );
			GET_LONG  ( sellInfo.maxQty );
			GET_STRING( name );
			
			sellInfo.name = reinterpret_cast< const char *>( name );
			itemList.push_back( sellInfo );
			
			delete name;
		}
		SellUI::GetInstance()->Sell( itemList );
                             } break;
		//////////////////////////////////////////////////////////////////////////////////////////				
		// Receive requested spell list.
    case RQ_SendSpellList:{
		
		TFCPacket *msg = Msg;
		
		WORD spellCount;
		WORD i;
		
		BYTE bUpdate = 0;
		
		GET_BYTE( bUpdate );
		
		GET_WORD( Player.Mana );
		GET_WORD( Player.MaxMana );
		GET_WORD( spellCount );
		
		list< SpellUI::Spell > spellList;
		
		for( i = 0; i < spellCount; i++ ){
			SpellUI::Spell spell;
			
			// Get each spells in the packet.
			GET_WORD( spell.Id );
			
			if( spell.Id != 0 ){
				LPBYTE name;
				LPBYTE desc;
				WORD element, type;
				
				GET_BYTE( spell.targetType );
				GET_WORD( spell.manaCost );
				
				GET_LONG( spell.duration );
				GET_WORD( spell.level );
				GET_WORD( element );
				GET_WORD( type );
				GET_LONG( spell.icon );
				
				GET_STRING( desc );            
				GET_STRING( name );
				
				spell.desc = reinterpret_cast< const char * >( desc );
				spell.name = reinterpret_cast< const char * >( name );
				
				switch( element ){
				case 0: spell.element = SpellUI::Spell::normal; break;
				case 1: spell.element = SpellUI::Spell::fire; break;
				case 2: spell.element = SpellUI::Spell::earth; break;
				case 3: spell.element = SpellUI::Spell::air; break;
				case 4: spell.element = SpellUI::Spell::water; break;
				case 5: spell.element = SpellUI::Spell::light; break;
				case 6: spell.element = SpellUI::Spell::dark; break;
				};
				switch( type ){
				case 1: spell.type = SpellUI::Spell::physical; break;
				case 2: spell.type = SpellUI::Spell::mental; break;
				};
				
				
				delete name;
				delete desc;
				
				spellList.push_back( spell );
			}
		}
		
		SpellUI::GetInstance()->UpdateSpells( spellList );
                          } break;
		
    case RQ_GodFlagUpdate:{
		TFCPacket *msg = Msg;
		
		LOG << "PAK = 97";
		
		BYTE flag;
		BYTE status;
		GET_BYTE( flag );
		GET_BYTE( status );
		
		switch( flag ){
		case 1:
			if( status != 0 ){
				Player.CanRunScripts = true;
			}else{
				Player.CanRunScripts = false;
			}
			break;
		case 2:
			if( status != 0 ){
				LOG << "\r\nGod can now slay users.";
				Player.CanSlayUsers = true;
			}else{
				LOG << "\r\nGod can no longer slay users.";
				Player.CanSlayUsers = false;
			}
			break;
		}
                          } break;
		
    case 10003: {
		LOG << "PAK = 1-3";
		short SkillID, Return;
		Msg->Get((short *)&SkillID);
		Msg->Get((short *)&Return);
		LOG << " Skill ID (" << SkillID << ") Return Value (" << Return << ") ";
		switch (SkillID) {
		case 5: // WarCry
			LOG << "WarCrySkill, Success";
			//									SoundFX[10].Play(false);
			break;
		}
		LOG << "\r\n";
                } break;
		
    case 33: {//RQ_HPchanged
		LOG << "* PAK = 33";
		Msg->Get((long *)&Player.Hp);            
		
		LOG << ", " << Player.Hp;
		LOG << "\r\n";
		
		// HELP ADD-ON
		if (Player.Hp < Player.MaxHp/2 && Player.Level == 1) {
			g_EventHelp.HP = true;
		} else {
			g_EventHelp.HP = false;
		}
		
		CharacterUI::GetInstance()->UpdateCharacterSheet();
             } break;
		
    case 67: {//RQ_HPchanged//a voir ca flood un peu trop parait il
		LOG << "* PAK = 67";
		Msg->Get((WORD *)&Player.Mana);            
		LOG << ", " << Player.Mana;
		LOG << "\r\n";
		
		CharacterUI::GetInstance()->UpdateCharacterSheet();
             } break; 
		
    case 68: {//SetPuppet
		LOG << "* PAK = 68 {\r\n";
		
		DWORD ID;
		WORD  BODY, FEET, GLOVES, HELM, LEGS, W_RIGHT, W_LEFT, CAPE;
		
		Msg->Get((long *)&ID);
		Msg->Get((short *)&BODY);
		Msg->Get((short *)&FEET);
		Msg->Get((short *)&GLOVES);
		Msg->Get((short *)&HELM);
		Msg->Get((short *)&LEGS);
		Msg->Get((short *)&W_RIGHT);
		Msg->Get((short *)&W_LEFT);
		Msg->Get((short *)&CAPE);
		
		// HELP ADD-ON
		g_EventHelp.Lock();
		if (ID == Player.ID && BODY == PUPEQ_NACKED && FEET == PUPEQ_NACKED && GLOVES == PUPEQ_NACKED &&
			HELM == PUPEQ_NACKED && LEGS == PUPEQ_NACKED && W_RIGHT == PUPEQ_NACKED &&
			W_LEFT == PUPEQ_NACKED && CAPE == PUPEQ_NACKED && Player.Level == 1) {
			g_EventHelp.Nacked = true;
		} else if (ID == Player.ID) {
			g_EventHelp.Nacked = false;
		}
		
		if (ID == Player.ID && W_RIGHT == PUPEQ_NACKED && Player.Level == 1) {
			g_EventHelp.noWeapon = true;
		} else if (ID == Player.ID) {
			g_EventHelp.noWeapon = false;
			g_EventHelp.Attack = false;
		}
		g_EventHelp.Unlock();
		
		
		Objects.Lock(57);
		Objects.SetPuppet(ID, BODY, FEET, GLOVES, HELM, LEGS, W_RIGHT, W_LEFT, CAPE);
		Objects.Unlock(57);
		
		LOG << "* PAK = 68 }\r\n";
             } break;
		
    case RQ_UseObject:
		break;
		
    case RQ_UseSkill:
		break;
		
    case RQ_EquipObject: {
		LOG << "* PAK = 21";
		LOG << " **\r\n";
                         } break;
		
    case 35: {//RQ_GetUnitName
		LOG << "* PAK = 35";
		unsigned long ID = 0;
		short Len;
		short LenGuild;//BLBLBL
		
		char *Name = new char [100];
		memset(Name, 0, 100);
		
		char *GuildName = new char [100];//BLBLBL
		memset(GuildName, 0, 100);//BLBLBL
		
		DWORD color;
		
		
		//Lecture de l'IR recue :
		Msg->Get((long *)&ID);
		
		//Lecture du nom de l'unité :
		Msg->Get((short *)&Len);
		for (int j = 0; j < Len && j < 100; j++) Msg->Get((char *)&Name[j]);
		Name[Len < 100 ? Len : 100] = 0;
		
		//Lecture de la couleur à afficher :
		Msg->Get((unsigned long *)&color);
		
		//BLBLBL Version 1.63g2 lecture Guilde dans paquet
		Msg->Get((short *)&LenGuild);
		for (int jg = 0; jg < LenGuild && jg < 100; jg++) Msg->Get((char *)&GuildName[jg]);
		GuildName[LenGuild < 100 ? LenGuild : 100] = 0;
		
		Objects.Lock(58);
		
		//if (LenGuild>0){//BLBLBL//GuildName[0]!=0
		if( strlen( GuildName ) > 0 )
		{
			Objects.SetGuildName(ID,GuildName, RGB( 0, 129, 192 ) );//BLBLBL
			//}//BLBLBL
		}
		
		Objects.SetName(ID, Name, TRUE, color);
		Objects.Identify(ID);
		
		Objects.Unlock(58);
		LOG << "NPC ID (" << ID << ") Len (" << Len << ") Name (" << Name << ") GuildName (" << GuildName << ")";
		LOG << "\r\n";
		
		delete Name;
		delete GuildName;
		
		
		
             } break;
		
    case 36: {//RQ_BreakConversation
		LOG << "* PAK = 36";
		TalkToX = 0;
		TalkToY = 0;
		TalkToID = NULL;
		TalkToOffset = 0;
		LOG << "\r\n";
             } break;
		
    case 34: {//RQ_BroadcastTextChange
		LOG << "* PAK = 34 {\r\n";
		DWORD Err = 0;
		// Deplacer le texte de NPC.
		unsigned long ID = 0;
		short Offset = 0;
		
		Err = 1;
		Msg->Get((long *)&ID);
		Err = 2;
		Msg->Get((short *)&Offset);
		
		
		Objects.Lock(60);
		Objects.SetOffset(ID, Offset);
		Objects.Unlock(60);
		
		LOG << "* PAK = 34, " << ID << ", " << Offset << "\r\n";
		LOG << "* PAK = 34 }\r\n";
             } break;
		//////////////////////////////////////////////////////////////////////////////////////////
		// Sent by the server for the client to update its backpack
    case RQ_ViewBackpack: {
		
		// View Backpack
		LOG << "* PAK = 18\r\n";            
		
		char *lpbName = NULL;
		
		unsigned short nbObjects = 0;
		//short Len;
		DWORD dwContainerID;	// ID of the container object.
		WORD  wNbObjects;		// Quantity of objects.
		WORD  wAppearance;		// Appearance of the object.
		DWORD dwID;				// ID of the object.
		BOOL  boFound;
		WORD  wBaseID;
		DWORD dwQty;
		int i;
		char cDisplay;			// 1 if box should be displayed
		long lCharges;
		
		Msg->Get( (char *)&cDisplay );
		Msg->Get( (long *)&dwContainerID );
		Msg->Get( (short *)&wNbObjects );			
		
		TemplateList <BAG_ITEM> tlTempBackpack;
		
		LOG << "-1";
		LPBAG_ITEM lpBagItem;
		//				LOG << "Has " << wNbObjects << " objects";		
		for ( i = 0; i < wNbObjects; i++ ) 
		{
			Msg->Get( (short *)&wAppearance );
			Msg->Get( (long  *)&dwID );
			Msg->Get( (short *)&wBaseID );
			Msg->Get( (long  *)&dwQty );
			Msg->Get( (long  *)&lCharges );
			
			// Create a new bag item.
			lpBagItem = new BAG_ITEM;
			lpBagItem->nX = -1;	// assign random pos to item
			lpBagItem->nY = -1;
			lpBagItem->dwID = dwID;
			lpBagItem->chName[0] = 0;
			lpBagItem->Type = wAppearance;
			lpBagItem->lpSprite = InvItemIcons( wAppearance );		// TODO
			lpBagItem->wBaseID = wBaseID;
			lpBagItem->dwQty     = dwQty;
			lpBagItem->charges   = lCharges;
			lpBagItem->associatedUI = NULL;
			// and add it to the backpack
			
			LOG << "\r\nBaseID=" << lpBagItem->wBaseID << "..";
			
			//					LOG << "\r\n[Objects(" << i+1 << ") Type(" << wAppearance << ") ID (" << lpBagItem->dwID << ")]";
			tlTempBackpack.AddToTail( lpBagItem );
		}
		
		LOG << "-3";
		// First check if there are items which were removed
		Player.tlBackpack.Lock("HandlePacket 2");
		
		LOG.Lock();
		LOG << "-3.1";
		Player.tlBackpack.ToHead();
		while( Player.tlBackpack.QueryNext() )
		{
			lpBagItem = Player.tlBackpack.GetObject();
			
			LOG << "-3.2";
			boFound = FALSE;			
			tlTempBackpack.ToHead();
			while( tlTempBackpack.QueryNext() && !boFound )
			{
				LPBAG_ITEM newBagItem = tlTempBackpack.GetObject();
				LOG << "-3.3";
				
				// If the item doesn't have an ID yet and an item of the same
				// type is found.
				if( lpBagItem->dwID == 0 && 
					lpBagItem->associatedUI != NULL && 
					lpBagItem->wBaseID == newBagItem->wBaseID )
				{
					
					strcpy((char*)lpBagItem->chName ,(char*)newBagItem->chName );
					
					// Use this new item's ID as the item's ID.
					lpBagItem->dwID = newBagItem->dwID;
				}
				
				if( newBagItem->dwID == lpBagItem->dwID )
				{
					// Update the bag item's item count
					lpBagItem->dwQty = newBagItem->dwQty;
					lpBagItem->charges = newBagItem->charges;
					
					// Update the associatedUI's name.
					if( lpBagItem->associatedUI != NULL )
					{
						static_cast< InventoryUI::ItemUI * >(lpBagItem->associatedUI)->UpdateName();
					}
					
					boFound = TRUE;
				}
			}
			
			LOG << "-3.4";
			// If the object wasn't found in the new backpack, remove it
			if( !boFound )//						LOG << "\r\nBackpack has one too many object";
			{
				Player.tlBackpack.DeleteObject();
				LOG << "-3.5";
			}
		}
		
		LOG.Unlock();
		
		LOG << "-4";
		tlTempBackpack.ToHead();
		while( tlTempBackpack.QueryNext() )
		{
			lpBagItem = tlTempBackpack.GetObject();
			
			boFound = FALSE;
			Player.tlBackpack.ToHead();
			while( Player.tlBackpack.QueryNext() && !boFound )
			{
				if( Player.tlBackpack.GetObject()->dwID == lpBagItem->dwID )
				{
					boFound = TRUE;
				}
			}
			
			// If the object wasn't found in the old backpack, add it
			if( !boFound ){
				//						LOG << "\r\nPlayer's backpack has one more object.";
				Player.tlBackpack.AddToTail( lpBagItem );
				tlTempBackpack.RemoveObject();
			}
		}
		
		tlTempBackpack.ToHead();
		while( tlTempBackpack.QueryNext() )
		{
			tlTempBackpack.DeleteObject();
		}
		
		LOG << "-5";
		
		Player.tlBackpack.ToHead();
		while (Player.tlBackpack.QueryNext()) 
		{
			BAG_ITEM *obj = Player.tlBackpack.GetObject();
			if (!obj->chName[0]) 
			{
				TFCPacket sending;
				sending << (RQ_SIZE)RQ_QueryItemName;
				sending << (char)PL_SEARCHBACKPACK;
				sending << (long)obj->dwID;
				SEND_PACKET( sending );
			}
		}
		
		Player.tlBackpack.Unlock("HandlePacket 2");
		
		// Update the new interface inventory.
		InventoryUI::GetInstance()->UpdateInventory( &Player.tlBackpack );
		
       } break;
       
       //////////////////////////////////////////////////////////////////////////////////////////
       // Sent by the server for the client to show the chest interface
    case RQ_ShowChest: {
		ChestUI::GetInstance()->Show();
                       }; break;
		//////////////////////////////////////////////////////////////////////////////////////////
		// Sent by the server for the client to hide the chest interface
    case RQ_HideChest: {
		ChestUI::GetInstance()->Hide();
                       }; break;
		//////////////////////////////////////////////////////////////////////////////////////////
		// Sent by the server for the client to update the chest contents
    case RQ_ChestContents: {
		// View Backpack
		LOG << "* PAK = ChestContents\r\n";            
		
		char *lpbName = NULL;
		
		WORD  wNbObjects;		// Quantity of objects.
		WORD  wAppearance;		// Appearance of the object.
		DWORD dwID;				// ID of the object.
		WORD  wBaseID;
		DWORD dwQty;
		//BOOL  boFound;
		long lCharges;
		int i;
		
		Msg->Get( (short *)&wNbObjects );			
		
		TemplateList <BAG_ITEM> tlTempBackpack;
		
		LOG << "-1";
		LPBAG_ITEM lpBagItem;
		//				LOG << "Has " << wNbObjects << " objects";		
		for ( i = 0; i < wNbObjects; i++ ) {
			Msg->Get( (short *)&wAppearance );
			Msg->Get( (long  *)&dwID );
			Msg->Get( (short *)&wBaseID );
			Msg->Get( (long  *)&dwQty );
			Msg->Get( (long  *)&lCharges );
			
			// Create a new bag item.
			lpBagItem = new BAG_ITEM;
			lpBagItem->nX = -1;	// assign random pos to item
			lpBagItem->nY = -1;
			lpBagItem->dwID = dwID;
			lpBagItem->chName[0] = 0;
			lpBagItem->Type = wAppearance;
			lpBagItem->lpSprite = InvItemIcons( wAppearance );		// TODO
			lpBagItem->wBaseID = wBaseID;
			lpBagItem->dwQty     = dwQty;
			lpBagItem->charges   = lCharges;
			// and add it to the backpack
			
			LOG << "\r\nBaseID=" << lpBagItem->wBaseID << "..";
			
			//					LOG << "\r\n[Objects(" << i+1 << ") Type(" << wAppearance << ") ID (" << lpBagItem->dwID << ")]";
			tlTempBackpack.AddToTail( lpBagItem );
		}
		
		// Update the new interface inventory.
		
		ChestUI::GetInstance()->UpdateChest( &tlTempBackpack );
						   } break;
        //////////////////////////////////////////////////////////////////////////////////////////
		// Sent by the server to notify someone is inviting the player to trade
    case RQ_TradeInvite: {
        LOG << "* PAK = TradeInvite\r\n";
		
		TFCPacket *msg = Msg;
		BYTE *invitorName;
		DWORD invitorID;
		
		int x, y;
		GET_STRING( invitorName );
		GET_LONG( invitorID );
		GET_WORD(x);
		GET_WORD(y);
		
		
		//BLBLBL 26 mars 2009 : on n'ouvre pas le trade si la personne qui invite est ignorée
		char szName[256];
		sprintf( szName, "%s", invitorName);
		
        if( !ChatterUI::GetInstance()->IsUserIgnored( szName )) //BLBLBL 26 mars 2009 test pour voir
        {
			
			TradeUI::GetInstance()->EventPlayerInvited((char*)invitorName, invitorID, x, y);
			
		}else{
			
			
			//Si le demandeur est ignoré, on annule le trade.
			TFCPacket packet;
			packet << (RQ_SIZE)RQ_TradeCancel;
			SEND_PACKET( packet );
			
		}
						 }; break;
		//////////////////////////////////////////////////////////////////////////////////////////
		// Sent by the server to notify that the trade has started
	case RQ_TradeStarted: {
        LOG << "* PAK = TradeStarted\r\n";
		
		TFCPacket *msg = Msg;
		BYTE *invitorName;
		GET_STRING( invitorName );
		TradeUI::GetInstance()->EventTradeStarted((char*)invitorName);
						  }; break;
		//////////////////////////////////////////////////////////////////////////////////////////
		// Sent by the server to notify that the trade has started
	case RQ_TradeCancel: {
        LOG << "* PAK = TradeCancel\r\n";
		
		TradeUI::GetInstance()->EventTradeCanceled();
						 }; break;
		//////////////////////////////////////////////////////////////////////////////////////////
		// Sent by the server to notify that the trade has finished
	case RQ_TradeFinish: {
        LOG << "* PAK = TradeFinish\r\n";
		TradeUI::GetInstance()->EventTradeFinished();
						 }; break;
        //////////////////////////////////////////////////////////////////////////////////////////
        // Sent by the server for the client to update the trade contents
    case RQ_TradeContents: {
        // View Backpack
        LOG << "* PAK = TradeContents\r\n";
        
		TradeUI::AffectedCharacter affectedChar;
		WORD tempAffectedChar;
		Msg->Get( (short*)&tempAffectedChar );
		affectedChar = TradeUI::AffectedCharacter(tempAffectedChar);
		
		char *lpbName = NULL;
        
        WORD  wNbObjects;		// Quantity of objects.
        WORD  wAppearance;		// Appearance of the object.
        DWORD dwID;				// ID of the object.
        WORD  wBaseID;
        DWORD dwQty;
        //BOOL  boFound;
        long lCharges;
        int i;
		
        Msg->Get( (short *)&wNbObjects );			
        
        TemplateList <BAG_ITEM> tlTempBackpack;
        
        LOG << "-1";
        LPBAG_ITEM lpBagItem;
        //				LOG << "Has " << wNbObjects << " objects";		
        for ( i = 0; i < wNbObjects; i++ ) {
			
            Msg->Get( (short *)&wAppearance );
            Msg->Get( (long  *)&dwID );
            Msg->Get( (short *)&wBaseID );
            Msg->Get( (long  *)&dwQty );
            Msg->Get( (long  *)&lCharges );
            
            // Create a new bag item.
            lpBagItem = new BAG_ITEM;
            lpBagItem->nX = -1;	// assign random pos to item
            lpBagItem->nY = -1;
            lpBagItem->dwID = dwID;
            lpBagItem->chName[0] = 0;
            lpBagItem->Type = wAppearance;
            lpBagItem->lpSprite = InvItemIcons( wAppearance );		// TODO
            lpBagItem->wBaseID = wBaseID;
            lpBagItem->dwQty     = dwQty;
            lpBagItem->charges   = lCharges;
            // and add it to the backpack
            
            LOG << "\r\nBaseID=" << lpBagItem->wBaseID << "..";
            
            //					LOG << "\r\n[Objects(" << i+1 << ") Type(" << wAppearance << ") ID (" << lpBagItem->dwID << ")]";
            tlTempBackpack.AddToTail( lpBagItem );
        }
		
        // Update the new interface inventory.
		
        if (affectedChar == TradeUI::AC_MYSELF) {
			TradeUI::GetInstance()->UpdateTrade( &tlTempBackpack, TradeUI::AC_MYSELF );
		} else if (affectedChar == TradeUI::AC_OTHER) {
			// Update other's trade contents
			TradeUI::GetInstance()->UpdateTrade( &tlTempBackpack, TradeUI::AC_OTHER );
		}
        
						   } break;
		
	case RQ_TradeSetStatus:
		{
			LOG << "* PAK = TradeSetStatus\r\n";
			
			TFCPacket *msg = Msg;
			TradeUI::CharacterStatus myStatus, otherStatus;
			WORD temp;
			GET_WORD(temp);
			myStatus = TradeUI::CharacterStatus(temp);
			GET_WORD(temp);
			otherStatus = TradeUI::CharacterStatus(temp);
			TradeUI::GetInstance()->EventTradeStatusChanged(myStatus, otherStatus);
			
		} break;
		
		
		//////////////////////////////////////////////////////////////////////////////////////////
		// Rob a user!
    case RQ_Rob:{
		TFCPacket *msg = Msg;
		
		list< RobUI::Item > itemList;
		
		char canRob = 0;
		DWORD targetId = 0;
		BYTE *targetName;
		GET_BYTE( canRob );
		GET_LONG( targetId );
		GET_STRING( targetName );
		
		WORD nbObjects;
		GET_WORD( nbObjects );
		
		WORD i;
		for ( i = 0; i < nbObjects; i++ ) {
			RobUI::Item item;
			
			BYTE *name;
			
			GET_WORD( item.appearance );
			GET_LONG( item.id );
			GET_WORD( item.baseId );
			GET_LONG( item.qty );
			GET_STRING( name );
			item.itemName = reinterpret_cast< char * >( name );
			
			char buf[ 256 ];
			item.itemName += " x";
			item.itemName += itoa( item.qty, buf, 10 );
			
			itemList.push_back( item );
			
			delete name;
		}
		
		RobUI::GetInstance()->Rob( 
			canRob != 0 ? true : false, 
			reinterpret_cast< char * >( targetName ), 
			itemList 
			);
		
		delete targetName;
                } break;
		//////////////////////////////////////////////////////////////////////////////////////////    /
		// Force a premature dispell of the robbing window
    case RQ_DispellRob:{
		RobUI::GetInstance()->DispellRob();
		
                       } break;
		//////////////////////////////////////////////////////////////////////////////////////////
		// This request is sent as a response to a QueryItemName packet
    case RQ_QueryItemName:
		{
			//OutputDebugString("Entre RQ_QueryItemName\n");
			LOG << "* PAK = 59\r\n";
			LPBAG_ITEM lpBagItem = NULL;
			LPBYTE lpbName = NULL;
			DWORD dwID;
			BYTE  whereToSearchFrom;
			BOOL boFound = FALSE;								
			TFCPacket *msg = Msg;
			
			GET_BYTE( whereToSearchFrom );
			GET_LONG( dwID );
			GET_STRING( lpbName );
			
			if(strlen((char*)lpbName) >255)
				lpbName[255] = NULL;
			
			
			LOG.Lock();
			LOG << "\r\nGot name " << (char *)lpbName;
			
			
			if (whereToSearchFrom == PL_SEARCHBACKPACK) 
			{
				
				Player.tlBackpack.Lock("HandlePacket 3");
				Player.tlBackpack.ToHead();
				while( Player.tlBackpack.QueryNext() )
				{
					lpBagItem = Player.tlBackpack.GetObject();
					
					// Update each item with the same base ID.
					if( lpBagItem->dwID == dwID)
					{
						//char strtoto[512];
						//sprintf(strtoto,"[%s] [%s]\n",lpbName,lpBagItem->chName);
						//OutputDebugString(strtoto);
						strcpy((char *)lpBagItem->chName, (char *)lpbName);
						//strcpy((char *)lpBagItem->chName,"Batard de marde de criss");
						strcpy((char *)lpBagItem->chName22,"Batard de marde de criss Batard de marde de criss Batard de marde de criss Batard de marde de criss Batard de marde de criss Batard de marde de criss Batard de marde de criss Batard de marde de criss Batard de marde de criss Batard de marde de criss");
						
						
						LOG << " item found..!";
						// If there is an associated UI.
						if( lpBagItem->associatedUI != NULL )
						{
							// Set its help text according to the UI's text.
							static_cast< InventoryUI::ItemUI * >( lpBagItem->associatedUI )->UpdateName();
						}
						break;
					}
				}
				
				Player.tlBackpack.Unlock("HandlePacket 3");
			}
			else if (whereToSearchFrom == PL_SEARCHCHEST) 
			{
				ChestUI::GetInstance()->UpdateItemsName(dwID, lpbName);
			}
			else if (whereToSearchFrom == PL_SEARCHMYTRADE || whereToSearchFrom == PL_SEARCHOTHERTRADE) 
			{
				TradeUI::GetInstance()->UpdateItemsName(dwID, lpbName, (ePlacesToSearch)whereToSearchFrom);
			}
			if(lpbName)
				delete []lpbName;
			lpbName = NULL;
			
			LOG.Unlock();
		}
		break;	
    case RQ_QueryItemInfo:
		{
#if 0
			
			
			/***********************************************/ 
Annex : stat values ( from Unit.h ) 
			
#define STAT_INT 1 
#define STAT_END 2 
#define STAT_STR 3 
#define STAT_WIS 4
#define STAT_LCK 5
#define STAT_AGI 6 
#define STAT_ATTACK 8 
#define STAT_DODGE 9 
#define STAT_DAMAGE 10 
#define STAT_RADIANCE       11 
#define STAT_AIR_RESIST     12 
#define STAT_FIRE_RESIST    13 
#define STAT_WATER_RESIST   14 
#define STAT_EARTH_RESIST   15 
#define STAT_AIR_POWER      16 
#define STAT_FIRE_POWER     17 
#define STAT_WATER_POWER    18 
#define STAT_EARTH_POWER    19 
#define STAT_AC             20 
#define STAT_LIGHT_RESIST   21 
#define STAT_DARK_RESIST    22 
#define STAT_LIGHT_POWER    23 
#define STAT_DARK_POWER     24 
#define STAT_MAX_HP         25 
#define STAT_MAX_MANA       26 
			
			
#endif     
			
			
			
			
			LOG.Lock();
		LOG << "* PAK = 122 (RQ_QueryItemInfo)\r\n";
		
		TFCPacket *msg = Msg;
		
		
		BYTE   chStatus;  // 0 = OK, data follows. 1 = no such unit. 2 = not an object 
		g_Global.Lock();
		g_Global.ResetItemInfo();
		sItemInfo *pNewInfo = g_Global.GetItemInfo();
		
		
		GET_BYTE  ( chStatus );
		if(chStatus == 0)
		{
			GET_STRING( pNewInfo->Name );
			GET_WORD  ( pNewInfo->shAppearance);
			GET_BYTE  ( pNewInfo->chRadiance );
			GET_WORD  ( pNewInfo->shAC);
			GET_WORD  ( pNewInfo->shDodge);
			GET_WORD  ( pNewInfo->shEnd);
			GET_LONG  ( pNewInfo->lMinDamage);
			GET_LONG  ( pNewInfo->lMaxDamage);
			GET_WORD  ( pNewInfo->shAttack);
			GET_WORD  ( pNewInfo->shStr);
			GET_WORD  ( pNewInfo->shAgi);
			GET_WORD  ( pNewInfo->shWis);
			GET_WORD  ( pNewInfo->shInt);
			GET_WORD  ( pNewInfo->shNbrBoust);
			
			pNewInfo->pBoust = new sItemBoust[pNewInfo->shNbrBoust];
			for(int n=0;n<pNewInfo->shNbrBoust;n++)
			{
                GET_BYTE  ( pNewInfo->pBoust[n].chBoustStat);
                GET_LONG  ( pNewInfo->pBoust[n].lMinVal);
                GET_LONG  ( pNewInfo->pBoust[n].lMaxVal);
			}
			pNewInfo->bSet = true;
		}
		else
			pNewInfo->bSet = false;
		
		g_Global.Unlock();
		
		LOG.Unlock();
		}
		break;
		
	case RQ_GuildGetMembers:
		{
			LOG.Lock();
            LOG << "* PAK = 123 (RQ_GuildGetMembers)\r\n";
			
			short wUserQuantity = 0;
			short Len = 0;
			int i = 0, j = 0;
			BYTE *szGuild;
			BYTE *szUser;
			BYTE *szRights;
            short level;
			DWORD userID;
			DWORD guildID;
			std::string myRights( "000000" );			
			
            // Get guild name
            TFCPacket *msg = Msg;		
			
			GET_LONG( guildID );
            GET_STRING( szGuild );
			
            // Get User
            GET_WORD( wUserQuantity );
			
            vector< GuildUI::MemberUI > users( wUserQuantity );
			
            for (j = 0; j < wUserQuantity; j++) 
			{
				GET_LONG( userID );
				GET_STRING( szUser );
				GET_STRING( szRights );
				GET_WORD( level );
				
				LOG << "\r\nGOT USER " << (char *)szUser << " rights = " << (char*)szRights;
				
				GuildUI::MemberUI user;
				user.userID	   = userID;
				user.username    = reinterpret_cast< const char * >( szUser );
				user.userRights  = reinterpret_cast< const char * >( szRights );
				user.level       = level;
				
				users[j] = user;
				
				// if current user is me, keep rights
				if( userID == Player.ID )
					myRights = reinterpret_cast< const char * >( szRights );
				
				delete szUser;
				delete szRights;			  
			}
			
			GuildUI::GetInstance()->UpdateUsersList( users );
			GuildUI::GetInstance()->UpdateGuild( guildID, reinterpret_cast< const char * >( szGuild ), myRights );
			
			LOG.Unlock();
			
			break;
		}
	case RQ_GuildInvite:
		{
			LOG.Lock();
			LOG << "* PAK GuildInvite";
			TFCPacket *msg = Msg;
			
			DWORD dwGuildID;
            BYTE *guildName;
            BYTE *sourceName;
			
			GET_LONG( dwGuildID );
            GET_STRING( guildName );
            GET_STRING( sourceName );
			
            GuildUI::GetInstance()->Join( dwGuildID, reinterpret_cast< const char * >( guildName ), reinterpret_cast< const char * >( sourceName ) );
            
            delete sourceName;   
			delete guildName;
			
			LOG.Unlock();
			break;
		}
		
    case RQ_UpdateWeight:{
		DWORD w, maxW;
		
		TFCPacket *msg = Msg;
		GET_LONG( w );
		GET_LONG( maxW );
		
		Player.Weight    = (USHORT)w;
		Player.MaxWeight = (USHORT)maxW;
		
		
		CharacterUI::GetInstance()->UpdateCharacterSheet();
		
                         }break;
		
    case 10001: {
		// Attaque Reussie
		LOG << "* PAK = 1-1 {\r\n";
		unsigned long IDAttack;
		unsigned long IDDefend;
		char TypeAttack;
		char TypeDefend;
		char pHp;
		short AXPos;
		short AYPos;
		short DXPos;
		short DYPos;
		
		Msg->Get((long *)&IDAttack);
		Msg->Get((long *)&IDDefend);
		Msg->Get((char *)&TypeAttack);
		Msg->Get((char *)&TypeDefend);
		Msg->Get((char *)&pHp);
		Msg->Get((short *)&AXPos);
		Msg->Get((short *)&AYPos);
		Msg->Get((short *)&DXPos);
		Msg->Get((short *)&DYPos);
		
		//LOG << "ID Attack (" << IDAttack << ") ID Defend (" << IDDefend << ") Type Attack (" << TypeAttack << ") Type Defend (" << TypeDefend << ")";
		
		if (IDAttack == Player.ID) {
			TalkToID = NULL;
			
			// HELP ADD-ON
			g_EventHelp.Lock();
			if (g_EventHelp.noWeapon) {
				g_EventHelp.Attack = true;
			} else {
				g_EventHelp.Attack = false;
			}
			g_EventHelp.Unlock();
		}
		
		Objects.Lock(61);
		if (IDAttack != Player.ID && !Objects.FoundID(IDAttack)) {
			TFCPacket Send;
			
			Send << (RQ_SIZE)71;
			Send << (long)IDAttack;
			Send << (short)AXPos;
			Send << (short)AYPos;
			
			SEND_PACKET(Send);
		}
		
		if (IDDefend != Player.ID && !Objects. FoundID(IDDefend)) {
			TFCPacket Send;
			
			Send << (RQ_SIZE)71;
			Send << (long)IDDefend;
			Send << (short)DXPos;
			Send << (short)DYPos;
			
			SEND_PACKET(Send);
		}
		Objects.Unlock(61);
		
		Objects.Lock(63);
		Objects.MoveObject(IDAttack, AXPos, AYPos, FALSE, 0, -1, 0, 0, 0);
		Objects.MoveObject(IDDefend, DXPos, DYPos, FALSE, 0, -1, 0, 0, 0);
		Objects.Unlock(63);
		
		if (IDAttack == Player.ID) {
			Objects.Lock(64);
			Objects.PlAttack(IDDefend, pHp);
			Objects.GetMainObject()->Chose = 1;
			Objects.GetMainObject()->Speed = 0;
			Objects.GetMainObject()->AttSpeed = 0;
			Objects.Unlock(64);
			signed char X, Y;
			Objects.Lock(65);
			Objects.Found(&X, &Y, IDDefend);
			if (X < 0 && Y == 0)
				Objects.GetMainObject()->Direction = 4;
			else if (X > 0 && Y == 0)
				Objects.GetMainObject()->Direction = 6;
			else if (X == 0 && Y > 0)
				Objects.GetMainObject()->Direction = 2;
			else if (X == 0 && Y < 0)
				Objects.GetMainObject()->Direction = 8;
			else if (X < 0 && Y > 0)
				Objects.GetMainObject()->Direction = 1;
			else if (X < 0 && Y < 0)
				Objects.GetMainObject()->Direction = 7;
			else if (X > 0 && Y > 0)
				Objects.GetMainObject()->Direction = 3;
			else if (X > 0 && Y < 0)
				Objects.GetMainObject()->Direction = 9;
			else
				Objects.GetMainObject()->Direction = 1;
			Objects.Unlock(65);
			
			Err = true;
		} else {
			Objects.Lock(66);
			Objects.SetAttack(IDAttack, IDDefend, 1, 1, pHp);
			//Objects.Anim(IDAttack, 1);
			//Objects.SetDirection(IDAttack, IDDefend);
			Objects.Unlock(66);
		}
		
		LOG << "* PAK = 1-1 }\r\n";
        } break;
        
        case 10002: {
			// Attaque Misser
			LOG << "* PAK = 1-2 {\r\n";
			unsigned long IDAttack;
			unsigned long IDDefend;
			short AXPos;
			short AYPos;
			short DXPos;
			short DYPos;
			
			Msg->Get((long *)&IDAttack);
			Msg->Get((long *)&IDDefend);
			Msg->Get((short *)&AXPos);
			Msg->Get((short *)&AYPos);
			Msg->Get((short *)&DXPos);
			Msg->Get((short *)&DYPos);
			
			if (IDAttack == Player.ID) {
				TalkToID = NULL;
				// HELP ADD-ON
				g_EventHelp.Lock();
				if (g_EventHelp.noWeapon) {
					g_EventHelp.Attack = true;
				} else {
					g_EventHelp.Attack = false;
				}
				g_EventHelp.Unlock();
			}
			
			LOG << "PAK = 1-2, ID Attack (" << IDAttack << ") ID Defend (" << IDDefend << ")\r\n";
			
			Objects.Lock(67);
			if (IDAttack != Player.ID && !Objects.FoundID(IDAttack) ) {
				TFCPacket Send;
				
				Send << (RQ_SIZE)71;
				Send << (long)IDAttack;
				Send << (short)AXPos;
				Send << (short)AYPos;
				
				SEND_PACKET(Send);
			}
			
			if (IDDefend) {
				
				if (IDDefend && IDDefend != Player.ID && !Objects. FoundID(IDDefend)) {
					TFCPacket Send;
					
					Send << (RQ_SIZE)71;
					Send << (long)IDDefend;
					Send << (short)DXPos;
					Send << (short)DYPos;
					
					SEND_PACKET(Send);
				}
				Objects.Unlock(67);
				
				Objects.Lock(68);
				Objects.MoveObject(IDAttack, AXPos, AYPos, FALSE, 0, -1, 0, 0, 0);
				Objects.MoveObject(IDDefend, DXPos, DYPos, FALSE, 0, -1, 0 ,0 ,0);
				Objects.Unlock(68);
				
				if (IDAttack == Player.ID) {
					Objects.Lock(69);
					Objects.PlAttack(NULL, 0);
					Objects.GetMainObject()->Chose = 1;
					Objects.GetMainObject()->Speed = 0;
					Objects.Unlock(69);
					signed char X, Y;
					if (IDDefend) {
						Objects.Lock(70);
						Objects.Found(&X, &Y, IDDefend);
						if (X < 0 && Y == 0)
							Objects.GetMainObject()->Direction = 4;
						else if (X > 0 && Y == 0)
							Objects.GetMainObject()->Direction = 6;
						else if (X == 0 && Y > 0)
							Objects.GetMainObject()->Direction = 2;
						else if (X == 0 && Y < 0)
							Objects.GetMainObject()->Direction = 8;
						else if (X < 0 && Y > 0)
							Objects.GetMainObject()->Direction = 1;
						else if (X < 0 && Y < 0)
							Objects.GetMainObject()->Direction = 7;
						else if (X > 0 && Y > 0)
							Objects.GetMainObject()->Direction = 3;
						else if (X > 0 && Y < 0)
							Objects.GetMainObject()->Direction = 9;
						else
							Objects.GetMainObject()->Direction = 1;
						
						Objects.Unlock(70);
					}
					Err = true;
				} else {
					Objects.Lock(71);
					Objects.SetAttack(IDAttack, IDDefend, 1, 0, -1);
					Objects.Unlock(71);
				}
			} else {
				if (IDAttack == Player.ID) {
					Objects.PlAttack(NULL, 0);
					Objects.GetMainObject()->Chose = 1;
					Objects.GetMainObject()->Speed = 0;
				} else {
					Objects.SetAttack(IDAttack, IDDefend, 1, 0, -1, 0);
				}
				Objects.Unlock(67);
			}
			
			LOG << "* PAK = 1-2 }\r\n";
        } break;
        
        case RQ_IndirectTalk: PacketHandling::IndirectTalk(Msg); break;
			
        case RQ_DirectedTalk: break;
			
        case 38: {
			LOG << "* PAK = 38";
			unsigned char Validation;
			Msg->Get((char *)&Validation);
			LOG << "Validation Code(" << Validation << ")\r\n";
			switch (Validation) {
			case 0: // Account registred
				Register = true;
				TFC_State = TFC_MENU;
				LastState = 0;
				Remote.MenuThread();
				//MenuManager();
				break;
			}
			LOG << "\r\n";
                 } break;
			
        case TFCInGame: {
			
			if (WantPreGame==false) break;//BLBLBL : 26 mars 2009 Dans le cas où on recoit une deuxieme fois la mise en jeu on saute.
			
			LOG << "* PAK = 13 {\r\n";
			
			unsigned long High = 0;
			unsigned long Low = 0;
			WantPreGame = false;
			
			//BLBLBL déplacé ici au lieu de //**//
			IDirectXImage diLoading;
			try 
			{
				// Load.PCX for 800 and 1024
				// BL : used during the phase AFTER the menu as a background image behind the progress bar
				char strLOADName[20];
				sprintf(strLOADName,"LOAD%d.PCX",g_Global.GetScreenW());
				if (diLoading.LoadImage(strLOADName,g_Global.GetScreenW(),g_Global.GetScreenH())) {
					DXDClear();
					DXDBlt(diLoading);
					DXDFlip();
					//DXDSetPalette(diLoading);
					LOG << "* END = 0-2-c\r\n";
					//diLoading.Release();
				}
			} 
			catch (...) 
			{
				LOG << "* WRN = LOAD.PCX\r\n";
			};
			//fin déplacement
			
			unsigned char ERR;
			Msg->Get((char *)&ERR);
			Msg->Get((long  *)&Player.ID);
			Msg->Get((short *)&Player.xPos);
			Msg->Get((short *)&Player.yPos);
			Msg->Get((short *)&Player.World);
			LOG << "* PAK = 13, " << Player.xPos << ", " << Player.yPos << ", " << Player.World << "\r\n";
			Msg->Get((long *)&Player.Hp);
			Msg->Get((long *)&Player.MaxHp);
			Msg->Get((short *)&Player.Mana);
			Msg->Get((short *)&Player.MaxMana);
			Msg->Get((long *)&High);
			LOG << "* PAK = 13, " << 8 << "\r\n";
			Msg->Get((long *)&Low);
			Player.Exp = ((__int64)(High) << 32) + (__int64)(Low);
			Msg->Get((long *)&High);
			Msg->Get((long *)&Low);
			Player.Exp2Go = ((__int64)(High) << 32) + (__int64)(Low);
			Msg->Get((short *)&Player.Str);
			Msg->Get((short *)&Player.End);
			Msg->Get((short *)&Player.Agi);
			Msg->Get((short *)&Player.Wil);
			Msg->Get((short *)&Player.Wis);
			Msg->Get((short *)&Player.Int);
			Msg->Get((short *)&Player.Lck);
			LOG << "* PAK = 13, " << 9 << "\r\n";
			Msg->Get((char *)&g_TimeStructure.Seconde);
			Msg->Get((char *)&g_TimeStructure.Minute);
			Msg->Get((char *)&g_TimeStructure.Hour);
			Msg->Get((char *)&g_TimeStructure.Day);
			Msg->Get((char *)&g_TimeStructure.Week);
			Msg->Get((char *)&g_TimeStructure.Month);
			Msg->Get((short *)&g_TimeStructure.Year);
			Msg->Get((long *)&Player.Gold);
			Msg->Get((short *)&Player.Level);
			Msg->Get((long *)&High);
			Msg->Get((long *)&Low);
			Player.ExpLastLevel = ((__int64)(High) << 32) + (__int64)(Low);
			LOG << "* PAK = 13, " << 10 << "\r\n";
			
			//LOG << "Time Structure S(" << g_TimeStructure.Seconde << ") M(" << g_TimeStructure.Minute << ") H(" << g_TimeStructure.Hour << ") D(" << g_TimeStructure.Day << ") W(" << g_TimeStructure.Week << ") M(" << g_TimeStructure.Month<< ") Y(" << g_TimeStructure.Year << ")";
			
			
			// Force load these UI's
			g_UiInit = true;
			OptionsUI  ::GetInstance();
			CharacterUI::GetInstance();
			SpellUI    ::GetInstance();         
			ChatterUI  ::GetInstance();
			InventoryUI::GetInstance();
			RTMap      ::GetInstance();
			RTHelp     ::GetInstance();
			RTItemI    ::GetInstance();
			
			ChatterUI::GetInstance()->ClientInitialize();
			RootBoxUI::GetInstance()->ClientInitialize();
			// Update the character sheet.
			CharacterUI::GetInstance()->UpdateCharacterSheet();
			InventoryUI::GetInstance()->UpdateCharacterSheet();
			
			// Get the skill list.
			TFCPacket sending;
			sending << (RQ_SIZE)RQ_GetSkillList;	
			SEND_PACKET( sending );
			//Sleep(50);
			
			//**//
			
			g_bEnterGamePart2Complete = FALSE;
			//::MessageBox(NULL,"Start Loadingpart2","",MB_OK);
			_beginthread( EntergamePart2, 0, NULL );
			
			int dwOffset = g_Var.vsfProgressB.GetHeight()+10;
			/*
			if(g_Global.GetScreenW() == 640)
			dwOffset = 76;
			else if(g_Global.GetScreenW() == 800)
			dwOffset = 70;
            */
			
			// CV2Sprite for Progressbar
			int dwX = (g_Global.GetScreenW()-g_Var.vsfProgressB.GetWidth())/2;
			V2SPRITEFX VsfFX;
			ZeroMemory(&VsfFX, sizeof(V2SPRITEFX));
			VsfFX.lpClipRect = new RECT; 
			VsfFX.lpClipRect->left   = dwX;
			VsfFX.lpClipRect->top    = g_Global.GetScreenH()-dwOffset;
			VsfFX.lpClipRect->right  = VsfFX.lpClipRect->left + g_Var.vsfProgressB.GetWidth();
			VsfFX.lpClipRect->bottom = VsfFX.lpClipRect->top  + g_Var.vsfProgressB.GetHeight();
			while(!g_bEnterGamePart2Complete)
			{
				
				//BL : toutes les 100 ms on affiche l'image de fond puis on affiche par dessus la progress bar.
				Sleep(100);
				if(diLoading)
					DXDBlt(diLoading);
				
				g_Var.vsfProgressB.DrawSpriteNSemiTrans(dwX,g_Global.GetScreenH()-dwOffset, &VsfFX,TRUE,200);
				int dwXT =  (dwX+53)+(g_Var.wProgressCnt*7);
				g_Var.vsfProgressT.DrawSpriteNSemiTrans(dwXT,g_Global.GetScreenH()-dwOffset+20, &VsfFX,TRUE,200);
				DXDFlip();
				g_Var.wProgressCnt += g_Var.wProgressInc;
				if(g_Var.wProgressCnt >=39 || g_Var.wProgressCnt <1)
				{
					g_Var.wProgressInc*=-1;
				}
			}
			if(diLoading)
				diLoading.Release();
			
			delete VsfFX.lpClipRect;
			DXDClear();//BL : on efface l'écran après que tout ait été chargé correctement.
			
			//::MessageBox(NULL,"END Loadingpart2","",MB_OK);
			
			
			switch (ERR) {
			case TFCCreateNewPlayer_CreatedPlayer: 
				// TODO
				break;
				
			case TFCCreateNewPlayer_AccountInGame:
				// TODO
				break;
				
			case TFCCreateNewPlayer_NoCredits:
				// TODO
				throw;
				break;
				
			case TFCCreateNewPlayer_TooManyAccounts:
				// TODO
				throw;
				break;
				
			case TFCCreateNewPlayer_PlayerAlreadyExists:
				// TODO
				throw;
				break;
				
			case TFCCreateNewPlayer_PlayerDontExists:
				// TODO
				throw;
				break;
				
			case TFCCreateNewPlayer_PlayerAlreadyGame:
				// TODO
				throw;
				break;
				
			case TFCCreateNewPlayer_InvalidNameSpec:
				// TODO
				throw;
				break;
				
			case 12:
				throw;
				break;
			}
			
			if ((!Player.Exp && FirstTime) || Custom.gfirstTimeAddOn) {
				FirstTime = FALSE;
				if (Player.Exp == 0 || Custom.gfirstTimeAddOn)
					Custom.gfirstTimeAddOn = false;
				RTHelp::GetInstance()->Show(true,0,0);
			}
			
			
			{
				TFCPacket Send;    //Le tour premier paquet qui demande les objets autour du joueur
				Send << (short)60; //GetNearItems
				Code13 = true; 
				EnterGame = true;
				SEND_PACKET(Send);
			}
			{
				//Il faut s'assurer que le client aie la réponse, sinon c'est le black screen assuré.
				TFCPacket Send;
				Send << (short)46; // RQ_FromPreInGameToInGame
				SEND_PACKET( Send );
			}
			
			RootBoxUI::GetInstance()->SetSideMenuState();
			
			LOG << "* PAK = 13 }\r\n";
        } break; 
        
        case 10004: {
			LOG << "* PAK = 1-4 {\r\n";
			signed char LIGHT;
			unsigned short X, Y, TYPE;
			unsigned long ID;
			unsigned char STATUS;
			char PHP;
			Msg->Get((short *)&X);
			Msg->Get((short *)&Y);
			Msg->Get((short *)&TYPE);
			Msg->Get((long  *)&ID);
			Msg->Get((char *)&LIGHT);
			Msg->Get((char *)&STATUS);
			Msg->Get((char *)&PHP);
			
			if (TYPE == 10002)
				TYPE = 20039;
			if (TYPE == 15002)
				TYPE = 25039;
			if (TYPE == 10004)
				TYPE = 20042;
			if (TYPE == 15004)
				TYPE = 25042;
			if (TYPE == 10001)
				TYPE = 20043;
			if (TYPE == 15001)
				TYPE = 25043;
			if (TYPE == 10003)
				TYPE = 20044;
			if (TYPE == 15003)
				TYPE = 25044;
			if (TYPE == 0)
				TYPE = 274;
			
			LOG << " [Object ID(" << ID << ") Pos(" << X << ", " << Y << ") Type(" << TYPE << ") Light(" << LIGHT << ")]\r\n";
			
			// Addition.
			if (ID != Player.ID) {
				
				World.Lock(75);
				Objects.Lock(1002);
				Objects.Add(ID, TYPE, X, Y, LIGHT, STATUS, NULL, PHP);
				Objects.Unlock(1002);
				World.Unlock(75);
			} else {
				LOG << "[Player POS (" << Player.xPos << ", " <<  Player.yPos << ")]\r\n";
				if (abs(Player.xPos-X) > 5 || abs(Player.yPos-Y) > 5) 
				{
					Player.xPos = X;
					Player.yPos = Y;
					
					g_Global.ValidMapZonePosition(Player.xPos,Player.yPos);
				}
				Player.Light = LIGHT;
			}
			
			//LOG << "\r\n";
			LOG << "* PAK = 1-4 }\r\n";
                    } break;
			
        case TFCQueryPlayerPosition: { 
			LOG << "* PAK = 9\r\n";
			signed short WORLD, X, Y;
			while (NeedRedraw) Sleep(10);
			//							Msg->Get((short *)&GeX);
			//							Msg->Get((short *)&GeY);
			Msg->Get((short *)&X);
			Msg->Get((short *)&Y);
			Msg->Get((short *)&WORLD);
			
			//char Temp[100];
			//sprintf(Temp, "Server Coor: %d, %d, %d", X, Y, WORLD);
			//ShowSystemMessage(Temp, RGB(255,255,255), 0);
			
			//LOG << " NewPos (" << X << ", " << Y << ", " << WORLD << ") OldPos (" << Player.xPos << ", " << Player.yPos << ", " << Player.World << ")";
			
			if (X < 0 || X > 3072 || Y < 0 || Y > 3072 || WORLD < 0 || WORLD > (World.NumberofWorld-1)) {
				ChatterUI::GetInstance()->AddBackscrollItem(
					"",
					g_LocalString[98],
					RGB(255,255,255),
					true
					);
				Sleep(2500);
				LOG << "* CLS = 2\r\n";
				PostMessage(siGethWnd(), WM_CLOSE, NULL, 0x6969);
				return;
			}
			
			if (X == Player.xPos && Y == Player.yPos && WORLD == Player.World) {
				if (ReAsk) {
					ReAsk = FALSE;
					if (!pfIsAtPosition() && !Move) {
						short Menrage = (short)pfGetNextMovement();
						if (Menrage) {
							TFCPacket Send;
							
							Send << Menrage;
							
							SEND_PACKET(Send);
						}
						Try = timeGetTime();
					} else {
						Try = timeGetTime();
						Move = true;
					}
				} else {
					Attack = false;
					Use = false;
					Get = false;
					Follow = false;
					Move = true;
					pfStopMovement("Move 2");
				}
			} else {
				//									if (Abs(Player.xPos - X) > 1 || Abs(Player.yPos - Y) > 1) {
				//										CString Text;
				//										Text.Format("Erreur dans l'ordre des position Last (%d, %d) New (%d %d)", Player.xPos, Player.yPos, X, Y);
				//										WARNINGBOX << Text;
				//									}
				Objects.Direction2 = 1;
				
				
				//AfxBeginThread(ChoseFunction, NULL);
				signed char lMovX = X - Player.xPos;
				signed char lMovY = Y - Player.yPos;
				
				if ((Abs(lMovX) > 10) || (abs(lMovY) > 10) || WORLD != Player.World) {
					//LOG << " [Teleport]";
					CloseAllUI();
					DoNotMove = FALSE;
					DrawLastMoving = 0;
					World.Done = 0;
					Draw = true;
					Player.xPos = X;
					Player.yPos = Y;
					Player.World = WORLD;
					// on commence par loader la map du new world...
					g_Global.LoadZoneMapWorld(Player.World,Player.xPos,Player.yPos);
					g_Global.ForceDisplayZone(Player.xPos,Player.yPos);
					//g_Global.ValidMapZonePosition(Player.xPos,Player.yPos);
					
					
					
					Objects.Lock(77);
					Objects.AddSDObject();
					Objects.AddSDAnimObject();
					Objects.Unlock(77); 
					World.Lock(78);
					Objects.Lock(1003);
					World.SetPosition(Player.xPos, Player.yPos, Player.World, FALSE);
					Objects.Unlock(1003);
					World.Unlock(78);
					Objects.Lock(80);
					Objects.MovePlayer(lMovX, lMovY);
					Objects.SetSet(TRUE);
					Objects.Unlock(80);
					World.Lock(81);
					World.Switch();
					World.Unlock(81);
					Objects.Lock(82);
					Objects.Direction2 = 1;
					Objects.GetMainObject()->SpriteNumber = 1;
					Objects.Unlock(82);
				} else {
					if (Abs(lMovX) > 1 || Abs(lMovY) > 1) {
						//LOG << " [Smooth]";
						if (X-Player.xPos > 1)
							X = Player.xPos + 1;
						else if (X-Player.xPos < 0)
							X = Player.xPos - 1;
						if (Y-Player.yPos > 1)
							Y = Player.yPos + 1;
						else if (Y-Player.yPos < 0)
							Y = Player.yPos - 1;
						
						Retrace = true;
						
					}
					
					World.Lock(83);
					Objects.Lock(1004);
					Objects.Direction2 = World.MoveToPosition(X, Y);
					Objects.Unlock(1004);
					World.Unlock(83);
					NeedRedraw = true;
				}
			}
			//LOG << "";
      } break;
      
        case TFCMoveID: {
			LOG << "* PAK = 1 {\r\n";
			
			
			signed char LIGHT;
			signed short X, Y;
			unsigned long ID;
			unsigned short TYPE;
			unsigned char STATUS;
			char PHP;
			//							Msg->Get((short *)&GeX);
			//							Msg->Get((short *)&GeY);
			Msg->Get((short *)&X);
			Msg->Get((short *)&Y);
			Msg->Get((short *)&TYPE);
			Msg->Get((long  *)&ID);
			Msg->Get((char *)&LIGHT);
			Msg->Get((char *)&STATUS);
			Msg->Get((char *)&PHP);
			if (!DoNotMove || (DoNotMove && ID != Player.ID) ) {
				if (TYPE == 10002)
					TYPE = 20039;
				if (TYPE == 15002)
					TYPE = 25039;
				if (TYPE == 10004)
					TYPE = 20042;
				if (TYPE == 15004)
					TYPE = 25042;
				if (TYPE == 10001)
					TYPE = 20043;
				if (TYPE == 15001)
					TYPE = 25043;
				if (TYPE == 10003)
					TYPE = 20044;
				if (TYPE == 15003)
					TYPE = 25044;
				
				LOG << "* PAK = 1 " << ", " << X << ", " << Y << ", " << TYPE << ", " << ID << ", " << LIGHT << ", " << STATUS << ", " << Player.xPos << ", " << Player.yPos << "\r\n";
				
				if (X < 0 || X > 3072 || Y < 0 || Y > 3072) {
					ChatterUI::GetInstance()->AddBackscrollItem(
						"",
						g_LocalString[98],
						RGB(255,255,255),
						true
						);
					Sleep(2500);
					LOG << "* CLS = 3\r\n";
					PostMessage(siGethWnd(), WM_CLOSE, NULL, 0x6969);
					return;
				}
				
				LOG << "* PAK = 1, 1\r\n";
				
				if (ID == Player.ID) {
					g_EventHelp.Lock();
					g_EventHelp.Attack = false;
					g_EventHelp.Unlock();
					
					LOG << "* PAK = 1, 2, 1\r\n";
					
					g_GameMusic.LoadNewSound();
					
					LOG << "* PAK = 1, 2, 2\r\n";
					dwCriticalTime = timeGetTime() - dwTimeAsk;
					if (TalkToX || TalkToY) {
						if (Abs(TalkToX - Player.xPos) >= 15 || Abs(TalkToY - Player.yPos) >= 15) {
							TFCPacket Send;
							
							Send << (short)36;
							Send << (long)TalkToID;
							Send << (short)TalkToX;
							Send << (short)TalkToY;
							
							TalkToX = 0;
							TalkToY = 0;
							TalkToID = 0;
							TalkToOffset = 0;
							
							SEND_PACKET(Send);
						}
					}
					LOG << "* PAK = 1, 2, 3\r\n";
					
					if (X == Player.xPos && Y == Player.yPos) {
						pfStopMovement("Move 3");
						//                           Attack = false;
						//                           Use = false;
						//                           Get = false;
						//                           Follow = false;
						Move = true;
					} else {
						LOG << "* PAK = 1, 2, 4\r\n";
						//									if (Abs(Player.xPos - X) > 1 || Abs(Player.yPos - Y) > 1) {
						//										CString Text;
						//										Text.Format("Erreur dans l'ordre des position Last (%d, %d) New (%d %d)", Player.xPos, Player.yPos, X, Y);
						//										WARNINGBOX << Text;
						//									}
						//AfxBeginThread(ChoseFunction, NULL);
						
						signed char lMovX = X - Player.xPos;
						signed char lMovY = Y - Player.yPos;
						
						if (Abs(lMovX) > 10 || Abs(lMovY) > 10) {
							// hide all UI.                                                              
							CloseAllUI();
							DoNotMove = FALSE;
							DrawLastMoving = 0;
							World.Done = 0;
							Draw = true;
							Player.xPos = X;
							Player.yPos = Y;
							
							g_Global.ValidMapZonePosition(Player.xPos,Player.yPos);
							
							
							Objects.Lock(77);
							Objects.AddSDObject();
							Objects.AddSDAnimObject();
							Objects.Unlock(77); 
							World.Lock(78);
							Objects.Lock(1005);
							World.SetPosition(Player.xPos, Player.yPos, Player.World, FALSE);
							Objects.Unlock(1005);
							World.Unlock(78);
							Objects.Lock(80);
							Objects.MovePlayer(lMovX, lMovY);
							Objects.SetSet(TRUE);
							Objects.Unlock(80);
							World.Lock(81);
							World.Switch();
							World.Unlock(81);
							Objects.Lock(82);
							Objects.Direction2 = 1;
							Objects.GetMainObject()->SpriteNumber = 1;
							Objects.Unlock(82);
							
							TFCPacket Send;
							
							Send << (short)60; //GetNearItems
							
							SEND_PACKET(Send);
						} else {
							
							if (Abs(lMovX) > 1 || Abs(lMovY) > 1) {
								//LOG << "NewPos(" << X << ", " << Y << ") OldPos(" << Player.xPos << ", " << Player.yPos << ")\r\n";
								if (X-Player.xPos > 1)
									X = Player.xPos + 1;
								else if (X-Player.xPos < 0)
									X = Player.xPos - 1;
								if (Y-Player.yPos > 1)
									Y = Player.yPos + 1;
								else if (Y-Player.yPos < 0)
									Y = Player.yPos - 1;
								
								Retrace = true;
							}
							LOG << "* PAK = 1, 2, 5\r\n";
							
							Player.Light = LIGHT;
							World.Lock(88);
							Objects.Lock(1006);
							Objects.Direction2 = World.MoveToPosition(X, Y);
							Objects.Unlock(1006);
							World.Unlock(88);
							
							NeedRedraw = true;
							LOG << "* PAK = 1, 2, 6\r\n";
						}
					}
            } else {
				LOG << "* PAK = 1, 3\r\n";
				World.Lock(90);
				Objects.Lock(1007);
				Objects.MoveObject(ID, X, Y, FALSE, 0, PHP, TYPE, LIGHT, STATUS, true);
				Objects.Unlock(1007);
				World.Unlock(90);
            }
            }
            LOG << "* PAK = 1 }\r\n";
      } break;
      
        case TFCStillConnected: {
			LOG << "* PAK = 10\r\n";
			TFCPacket Send; 
			
			Send << (short)10;//RQ_Ack
			
			SEND_PACKET(Send);
                                } break;
			
        case TFCDeleteObject: {
			LOG << "* PAK = 11";
			unsigned long ID;
			char Temp;
			
			Msg->Get((char *)&Temp);
			Msg->Get((long  *)&ID);
			//LOG << "Object ID(" << ID << ")";
			
			// Delete object.
			if (ID == Player.ID) {
			} else {
				Objects.Lock(91);
				Objects.Delete(ID);
				Objects.Unlock(91);
			}
			LOG << "\r\n";
                              } break;
			
        case RQ_DepositObject: {
			LOG << "* PAK = 12 {\r\n";
			unsigned short TYPE;
			unsigned long ID;
			Msg->Get((short *)&TYPE);
			Msg->Get((long  *)&ID);
			
			LOG << "* PAK = 12, 1\r\n";
			if (TYPE == 10002)
				TYPE = 20039;
			if (TYPE == 15002)
				TYPE = 25039;
			if (TYPE == 10004)
				TYPE = 20042;
			if (TYPE == 15004)
				TYPE = 25042;
			if (TYPE == 10001)
				TYPE = 20043;
			if (TYPE == 15001)
				TYPE = 25043;
			if (TYPE == 10003)
				TYPE = 20044;
			if (TYPE == 15003)
				TYPE = 25044;
			
			
			LOG << "Object ID (" << ID << ") Type (" << TYPE << ")";
			
			// Changement de type.
			int x;
			LOG << "* PAK = 12, 2\r\n";
			if (Player.ID != ID) 
			{
				LOG << "* PAK = 12, 3\r\n";
				World.Lock(90);
				Objects.Lock(92);
				x = Objects.ChangeType(ID, TYPE);
				Objects.Unlock(92);
				World.Unlock(90);
			} 
			else 
			{
				LOG << "* PAK = 12, 4\r\n";
				//World.Lock(93);
				World.Lock(93);
				Objects.Lock(93);
				x = Objects.ChangePlayerType(TYPE);
				Objects.Unlock(93);
				World.Unlock(93);
				//World.Unlock(93);
			}
			LOG << "* PAK = 12, 5\r\n";
			LOG << "* PAK = 12 }\r\n";
                               } break;
			
			
			// Adds an effect to the EffectStatusUI
        case RQ_CreateEffectStatus:{
			
			LOG << "* PAK = 83";
			
			TFCPacket *msg = Msg;
			
			DWORD effectId, time, iconId, totalTime;
			LPBYTE desc = NULL;
			
			GET_LONG( effectId );
			GET_LONG( time );
			GET_LONG( totalTime );
			GET_LONG( iconId );
			GET_STRING( desc );
			
			EffectStatusUI::GetInstance()->AddEffect(
				effectId,
				time,
				totalTime,
				iconId,
				reinterpret_cast< char * >( desc )
				);
			
			delete desc;
			
                                   } break;
			// Removes an effect to the EffectStatusUI
        case RQ_DispellEffectStatus:{
			LOG << "* PAK = 84";
			TFCPacket *msg = Msg;
			
			DWORD effectId;
			
			GET_LONG( effectId );
			
			EffectStatusUI::GetInstance()->DispellEffect(
				effectId
				);
                                    } break;
			
			
        case TFCAddObject: {
			LOG << "* PAK = 16 {\r\n";
			
			signed char LIGHT;
			unsigned short X, Y, TYPE, nbObject;
			unsigned long ID;
			unsigned char STATUS;
			char PHP;
			Msg->Get((short *)&nbObject);
			
			for (unsigned int i = 0; i < nbObject; i++) {
				Msg->Get((short *)&X);
				Msg->Get((short *)&Y);
				Msg->Get((short *)&TYPE);
				Msg->Get((long  *)&ID);
				Msg->Get((char *)&LIGHT);
				Msg->Get((char *)&STATUS);
				Msg->Get((char *)&PHP);
				LOG << "* PAK = 16, " << X << ", " << Y << ", " << TYPE << ", " << ID << ", " << LIGHT << ", " << STATUS << "\r\n";
				if (TYPE == 10002)
					TYPE = 20039;
				if (TYPE == 15002)
					TYPE = 25039;
				if (TYPE == 10004)
					TYPE = 20042;
				if (TYPE == 15004)
					TYPE = 25042;
				if (TYPE == 10001)
					TYPE = 20043;
				if (TYPE == 15001)
					TYPE = 25043;
				if (TYPE == 10003)
					TYPE = 20044;
				if (TYPE == 15003)
					TYPE = 25044;
				
				//LOG << " [Object ID(" << ID << ") Pos(" << X << ", " << Y << ") Type(" << TYPE << ")]";
				
				// Addition.
				if (ID != Player.ID) {
					World.Lock(94);
					Objects.Lock(1008);
					Objects.Add(ID, TYPE, X, Y, LIGHT, STATUS, NULL, PHP);
					Objects.Unlock(1008);
					World.Unlock(94);
				}
				
			}
			
			if (Code13) {
				Objects.Lock(17);
				Objects.SetSet(TRUE);
				Objects.AddSDObject();
				Objects.AddSDAnimObject();
				Objects.Unlock(17); 
				
				Objects.Lock(19);
				Objects.Direction2 = 1;
				Objects.GetMainObject()->SpriteNumber = 1;
				Objects.Unlock(19);
				
				g_GameMusic.LoadNewSound();
				
				g_Var.inGame = false;
				
				Objects.Lock(13);
				Objects.Direction2 = 1;
				Objects.GetMainObject()->SpriteNumber = 1;
				Objects.Unlock(13);
				
				INGAME = TRUE;
				if (!hDrawThread) {
					DXDClear();
					DXDFlip();
					DXDClear(); //BLBLBL 26 mars 2009 : retiré les doublons
					DXDFlip();  //modif annulée
					hDrawThread = (HANDLE)_beginthreadex(NULL, 0, DrawThread, NULL, CREATE_SUSPENDED, &iDrawThreadID);
					LOG << "* DRW = Begin Thread, 1\r\n";
				}
				if( !hMouseActionThread ){
					hMouseActionThread = (HANDLE)_beginthreadex(NULL, 0, MouseActionThread, NULL, 0, &iMouseActionThreadID);
				}
				
				stShow = true; 
				
				LOG << "* DRW = Resume Thread, 1\r\n";
				ResumeThread(hDrawThread);      
				
				g_Pal.SetToBlack();
				
				World.Lock(20);
				World.SetFading = true;
				World.FadeLevel = 0;
				World.Switch();
				World.Unlock(20);
				
				Code13 = false;
				noException = false;
			}
			
			if (EnterGame) {
				TFCPacket Send;
				Send << (short)46;//FromPreInGameToInGame 
				SEND_PACKET(Send);
			}
			
			LOG << "* PAK = 16 }\r\n";
        } break;		
		
		
        case RQ_SafePlug:{//BLBLBL antiplug system
			LOG << "* PAK = 123\r\n";
			TFCPacket *msg = Msg;
			
			BYTE bSafePlugStatus = 0;
			
			GET_BYTE( bSafePlugStatus );
			
			if (bSafePlugStatus == 0 ) {
				boInterruptLogoff = true;//si le joueur n'est pas safe on interrompt le log off
				boForceLogoff = false;
			}else if (bSafePlugStatus == 1){
				boInterruptLogoff = false;
				boForceLogoff = true;//autrement si le serveur lui envoie le feu vert, on déco direct
			}
			
						 }break;
			
    } // End of Switch(Type)
   }
   }
   catch (TFCPacketException *e) 
   {
	   delete e;
	   LOG << "* PAK = 0, ";
	   try {
		   RQ_SIZE ID = Msg->GetPacketID();
		   LOG << ID << "\r\n";
	   } catch (TFCPacketException *e) {
		   delete e;
		   LOG << "-1\r\n";
	   }
	   throw;
   }
}	

void EntergamePart2(LPVOID pParam)
{
	Objects.Lock(72);
	Objects.SetSet(TRUE);
	Objects.Unlock(72);
	World.Lock(73);
	Objects.Lock(1001); 
	World.SetPosition(Player.xPos, Player.yPos, Player.World, FALSE); 
	Objects.Direction2 = 1;
	Objects.GetMainObject()->SpriteNumber = 1;
	Objects.Unlock(1001);
	World.Unlock(73);
	g_bEnterGamePart2Complete = TRUE;
}  


