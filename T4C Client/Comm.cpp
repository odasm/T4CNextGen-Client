#pragma warning( disable : 4291 )
#include "pch.h"
#include <windows.h>
#include "COMM.h"
#include "PowerDebug.h"
#include "Global.h"
#include "MemoryManager.h"
#include "App.h"
#include "PacketTypes.h"//BLBLBL 07/12/2010

#define SETACK( __maxdelay, __maxack ) dwMaxDelay = __maxdelay; dwMaxAck = __maxack; break;//07/12/2010 BLLBLBL importé du serveur
 
PacketCenter COMM;
 
PacketCenter::PacketCenter(void) {
// Constructor.
   Ctr = NULL;
   State = 0;
	InitializeCriticalSection(&csInternal);
	AddrIP = NULL;
} 
  
PacketCenter::~PacketCenter(void) {
// Destructor.
	DeleteCriticalSection(&csInternal);
	if (AddrIP)
		delete AddrIP;
}
 
void PacketCenter::Close(void) {
   if (Ctr) {
      delete Ctr;
      Ctr = NULL;
   }
   WSACleanup();
}

void PacketCenter::SetIPAddr(char *a) {
   AddrIP = new char [strlen(a) + 1];
   strcpy(AddrIP, a);
}

char *PacketCenter::GetIPAddr()
{
   return AddrIP;
}

void PacketCenter::SetAddrPort(short p) {
   AddrPort = p;
}

void PacketCenter::Lock(void) {
	EnterCriticalSection(&csInternal);
}

void PacketCenter::Unlock(void) {
	LeaveCriticalSection(&csInternal);
}

int PacketCenter::Receive(TFCPacket * &P) {
   if (tqQueue.NbObjects()) {
      P = tqQueue.Retreive();
      return 1;
   } 
   return 0;
}

void PacketCenter::AddPacket(TFCPacket *P) {
   tqQueue.AddToQueue(P);
}

BOOL PacketCenter::Create( COMM_INTR_CALLBACK lpIntr ) {
	Ctr = new CCommCenter;
   
   WSADATA  wsaData;
	if (WSAStartup(MAKEWORD( 1, 1 ) ,&wsaData) == SOCKET_ERROR) {
		LOG << "* ERR = x25\r\n";
	}
   
   Ctr->Create( lpIntr);

	memset(&sockAddr, 0, sizeof(sockaddr));	
	sockAddr.sin_family = AF_INET;

	hostent *HostInfo;    

	if (!isdigit(AddrIP[0])) {   
	// Server Name is Alphanumeric

      HostInfo = gethostbyname(AddrIP);
		if (!HostInfo) {
			return FALSE;
			}
		memcpy(&(sockAddr.sin_addr), HostInfo->h_addr, HostInfo->h_length);
	} else  { 
	// Server Name is a nnn.nnn.nnn.nnn
		sockAddr.sin_addr.s_addr = inet_addr(AddrIP);
	}

	sockAddr.sin_port = htons(AddrPort);

   return TRUE;
}

void PacketCenter::SendPacket(TFCPacket *P) {

   if ( g_boQuitApp && P->GetPacketID() != 20)
      return;

   DWORD dwMaxAck = 0;
   DWORD dwMaxDelay = 0;

   LPBYTE lpbBuffer;
   INT    nbBuffer;
   
   RQ_SIZE PacketID = P->GetPacketID();

/* BL : For reference, the numbered values of pack types :
#define	RQ_MoveNorth					1 
#define	RQ_MoveNorthEast				2 
#define	RQ_MoveEast						3 
#define	RQ_MoveSouthEast				4 
#define	RQ_MoveSouth					5 
#define	RQ_MoveSouthWest				6 
#define	RQ_MoveWest						7 
#define RQ_MoveNorthWest				8 
#define RQ_GetPlayerPos					9 
#define RQ_Ack							10 
#define RQ_GetObject					11 
#define RQ_DepositObject				12 
#define RQ_PutPlayerInGame				13 
#define RQ_RegisterAccount				14 
#define RQ_DeletePlayer					15 
#define RQ_SendPeriphericObjects		16 
#define RQ_GodCreateObject				17 
#define RQ_ViewBackpack					18
#define RQ_ViewEquiped					19 
#define RQ_ExitGame						20 
#define RQ_EquipObject					21 
#define RQ_UnequipObject				22 
#define RQ_UseObject					23 
#define RQ_Attack						24 
#define RQ_CreatePlayer					25 
#define RQ_GetPersonnalPClist			26 		
#define RQ_IndirectTalk					27 
#define RQ_Shout						28 
#define RQ_Page							29 
#define RQ_DirectedTalk					30 
#define RQ_Reroll						31
#define RQ_CastSpell					32 
#define RQ_HPchanged					33
#define RQ_BroadcastTextChange			34
#define RQ_GetUnitName					35
#define RQ_BreakConversation			36
#define RQ_LevelUp						37
#define RQ_ReturnToMenu					38
#define RQ_GetSkillList					39
#define RQ_SendTrainSkillList			40
#define RQ_SendBuyItemList				41
#define RQ_UseSkill						42
#define RQ_GetStatus					43
#define RQ_XPchanged					44
#define RQ_GetTime						45
#define RQ_FromPreInGameToInGame		46
#define RQ_YouDied						47
#define RQ_AddRemoveChatterChannel		48
#define RQ_SendChatterChannelMessage	49
#define RQ_GetChatterUserList		    50
#define RQ_GetOnlinePlayerList			51
#define RQ_GetSkillStatPoints			52
#define RQ_GoldChange					53
#define RQ_ViewGroundItemIndentContent	54
#define RQ_SendTeachSkillList			55
#define RQ_SendSellItemList				56
#define RQ_TeleportPlayer				57
#define RQ_SendStatTrain				58
#define RQ_QueryItemName				59
#define RQ_GetNearItems					60
#define RQ_PlayerFastMode				61
#define RQ_SendSpellList				62
#define RQ_ServerMessage				63
#define RQ_SpellEffect                  64
#define RQ_ManaChanged                  67
#define RQ_GetChatterChannelList        75
#define RQ_CreateEffectStatus           83
#define RQ_DispellEffectStatus          84
#define RQ_UpdateGroupMemberHp          87
#define RQ_UpdateWeight                 92
#define RQ_Rob                          93
#define RQ_DispellRob                   94
#define RQ_ArrowMiss                    95
#define RQ_ArrowHit                     96
#define RQ_GodFlagUpdate                97
#define RQ_SeraphArrival                98
#define RQ_AuthenticateServerVersion    99
#define RQ_Seraph                       100
#define RQ_InfoMessage                  102
#define RQ_MaxCharactersPerAccountInfo  103
#define RQ_WeatherMsg					104
#define RQ_OpenURL						105
// Chest Packets
#define RQ_ChestContents				106
#define RQ_ChestAddItemFromBackpack		107
#define RQ_ChestRemoveItemToBackpack	108
#define RQ_ShowChest					109
#define RQ_HideChest					110
// Trade Packets
#define RQ_TradeContents				111
#define RQ_TradeAddItemFromBackpack		112
#define RQ_TradeRemoveItemToBackpack	113
#define RQ_TradeInvite					116
#define RQ_TradeSetStatus				117
#define	RQ_TradeCancel					118
#define RQ_TradeClear					119
#define RQ_TradeStarted					120
#define RQ_TradeFinish					121
#define RQ_QueryItemInfo   			122
// Guilds
#define RQ_SafePlug					123
#define RQ_GuildGetMembers				124//usused for now.
#define RQ_GuildInvite					125
#define RQ_GuildKick					126
#define RQ_GuildLeave					127
#define RQ_GuildAlterRights				128
#define RQ_GuildRename					129
#define RQ_GuildInviteAnswer			130	
#define RQ_GetPvpRanking				131
	*/	
		 
   switch (PacketID) { 
	    case RQ_MoveNorth:              		SETACK( 0, 0 );
		case RQ_MoveNorthEast:          		SETACK( 0, 0 );
		case RQ_MoveEast:               		SETACK( 0, 0 );
		case RQ_MoveSouthEast:          		SETACK( 0, 0 );
		case RQ_MoveSouth:              		SETACK( 0, 0 );
		case RQ_MoveSouthWest:          		SETACK( 0, 0 );
		case RQ_MoveWest:               		SETACK( 0, 0 );
		case RQ_MoveNorthWest:          		SETACK( 0, 0 );
		case RQ_GetPlayerPos:           		SETACK( 0, 0 );
		case RQ_Ack:                    		SETACK( 0, 0 );
		case RQ_GetObject:              		SETACK( 1000, 5 );
		case RQ_DepositObject:          		SETACK( 1000, 5 );
		case RQ_PutPlayerInGame:        		SETACK( 1000, 5 );
		case RQ_RegisterAccount:        		SETACK( 1000, 5 );
		case RQ_DeletePlayer:           		SETACK( 1000, 5 );
		case RQ_SendPeriphericObjects:  		SETACK( 1000, 5 );
		case RQ_GodCreateObject:        		SETACK( 1000, 5 );
		case RQ_ViewBackpack:           		SETACK(  500, 5 );
		case RQ_ViewEquiped:            		SETACK(  500, 5 );
		case RQ_ExitGame:               		SETACK( 1000, 5 );
		case RQ_EquipObject:            		SETACK( 1000, 5 );
		case RQ_UnequipObject:          		SETACK( 1000, 5 );
		case RQ_UseObject:              		SETACK( 1000, 5 );
		case RQ_Attack:                 		SETACK(  500, 3 );
		case RQ_CreatePlayer:           		SETACK( 1000, 5 );
		case RQ_GetPersonnalPClist:     		SETACK( 1000, 5 );
		case RQ_IndirectTalk:           		SETACK( 1500, 3 );
		case RQ_Page:                   		SETACK( 1500, 3 );
		case RQ_DirectedTalk:           		SETACK( 1500, 2 );
		case RQ_Reroll:                 		SETACK( 1000, 2 );
		case RQ_CastSpell:              		SETACK( 1000, 5 );
		case RQ_HPchanged:              		SETACK( 0, 0 );
		case RQ_BroadcastTextChange:    		SETACK( 1000, 3 );
		case RQ_GetUnitName:            		SETACK( 1000, 3 );
		case RQ_BreakConversation:      		SETACK( 1000, 5 );
		case RQ_LevelUp:                		SETACK( 2000, 5 );
		case RQ_ReturnToMenu:           		SETACK( 0, 0 );
		case RQ_GetSkillList:           		SETACK( 1000, 3 );
		case RQ_SendTrainSkillList:     		SETACK( 1500, 3 );
		case RQ_SendBuyItemList:        		SETACK( 1500, 3 );
		case RQ_UseSkill:               		SETACK( 1000, 3 );
		case RQ_GetStatus:              		SETACK( 1500, 3 );
		case RQ_XPchanged:              		SETACK( 0, 0 );
		case RQ_FromPreInGameToInGame:  		SETACK( 1000, 5 );
		case RQ_YouDied:                		SETACK( 1000, 5 );
		case RQ_EnterChatterChannel:    		SETACK( 1000, 5 );
		case RQ_SendChatterMessage:     		SETACK( 1000, 5 );
		case RQ_GetChatterUserList:     		SETACK( 1000, 5 );
		case RQ_GetOnlinePlayerList:    		SETACK( 1500, 2 );
		case RQ_GetSkillStatPoints:     		SETACK( 1000, 3 );
		case RQ_GoldChange:             		SETACK( 0, 0 );
		case RQ_ViewGroundItemIndentContent:    SETACK( 0, 0 );
		case RQ_SendTeachSkillList:     		SETACK( 1000, 3 );
		case RQ_SendSellItemList:       		SETACK( 1000, 3 );
		case RQ_TeleportPlayer:         		SETACK( 750,  5 );
		case RQ_SendStatTrain:          		SETACK( 1000, 3 );
		case RQ_QueryItemName:          		SETACK( 750, 3 );
		case RQ_GetNearItems:           		SETACK( 1000, 1 );
		case RQ_PlayerFastMode:         		SETACK( 1000, 2 );
		case RQ_SendSpellList:          		SETACK( 1250, 3 );
		case RQ_ServerMessage:          		SETACK( 1500, 5 );
		case RQ_SpellEffect:            		SETACK( 500, 5 );
		case RQ_ManaChanged:            		SETACK( 0, 0 );
		case RQ_GetTime:                		SETACK( 0, 0 );
		case RQ_PuppetInformation:      		SETACK( 750, 3 );
		case RQ_QueryServerVersion:     		SETACK( 1000, 3 );
		case RQ_MessageOfDay:           		SETACK( 1000, 3 );
		case RQ_UnitUpdate:             		SETACK( 500, 2 );
		case RQ_MissingUnit:            		SETACK( 1000, 2 );
		case RQ_QueryUnitExistence:     		SETACK( 1000, 3 );
		case RQ_UseItemByAppearance:    		SETACK( 1000, 3 );
		case RQ_CannotFindItemByAppearance: 	SETACK( 1000, 3 );
		case RQ_RemoveFromChatterChannel:   	SETACK( 1000, 3 );
		case RQ_GetChatterChannelList:      	SETACK( 1000, 3 );
		case RQ_CreateEffectStatus:         	SETACK( 1000, 3 );
		case RQ_DispellEffectStatus:        	SETACK( 1000, 3 );
		case RQ_UpdateGroupMembers:         	SETACK( 1000, 3 );
		case RQ_UpdateGroupInviteList:      	SETACK( 1000, 3 );
		case RQ_GroupInvite:                	SETACK( 1000, 3 );
		case RQ_GroupJoin:                  	SETACK( 1000, 3 );
		case RQ_GroupLeave:                 	SETACK( 1000, 3 );
		case RQ_GroupKick:                  	SETACK( 1000, 3 );
		case RQ_NotifyGroupDisband:         	SETACK( 1000, 3 );

		case RQ_ToggleChatterListening:     SETACK( 1000, 3 );    
		case RQ_UpdateGroupMemberHp:        SETACK( 1000, 3 );
		case RQ_GroupToggleAutoSplit:       SETACK( 1000, 3 );
		case RQ_QueryNameExistence:         SETACK( 1000, 3 );
		case RQ_QueryPatchServerInfo:       SETACK( 1000, 8 );
		case RQ_UpdateWeight:               SETACK( 1000, 3 );
		case RQ_Rob:                        SETACK( 1000, 3 );
		case RQ_DispellRob:                 SETACK( 1000, 3 );
		case RQ_ArrowMiss:                  SETACK( 500, 3 );
		case RQ_ArrowHit:                   SETACK( 500, 3 );
		case RQ_GodFlagUpdate:               SETACK( 1000, 3 );
		case RQ_SeraphArrival:          SETACK( 1000, 3 );
		case RQ_AuthenticateServerVersion: SETACK( 1000, 5 );
		case RQ_Remort: SETACK( 1000, 8 );
		case 10000:    SETACK( 750, 5 );
		case 10001:            SETACK( 500, 3 );
		case 10002:              SETACK( 0, 0 );
		case 10003:        SETACK( 1000, 3 );
		case 10004:                     SETACK( 1000, 3 );
		case RQ_MaxCharactersPerAccountInfo: SETACK( 1000, 3 );
		case RQ_WeatherMsg:					 SETACK( 1000, 3 );
		case RQ_OpenURL:					SETACK( 1000, 3 );
		case RQ_ChestContents:				SETACK( 500, 5 );
		case RQ_ChestAddItemFromBackpack:	SETACK( 500, 5 );
		case RQ_ChestRemoveItemToBackpack:	SETACK( 500, 5 );
		case RQ_ShowChest:					SETACK( 500, 5 );
		case RQ_HideChest:					SETACK( 500, 5 );

		case RQ_TradeInvite: case RQ_TradeCancel: case RQ_TradeSetStatus: 
		case RQ_TradeAddItemFromBackpack:	case RQ_TradeRemoveItemToBackpack:
		case RQ_TradeClear: case RQ_TradeStarted: case RQ_TradeContents: 
		case RQ_TradeFinish: SETACK( 500, 5 );
		
		// by Asteryth for pvp ranking
		case RQ_GetPvpRanking: SETACK( 500, 3 );
		
		case RQ_QueryItemInfo: SETACK(1000,3);
      default: 
         dwMaxAck = 0;
         dwMaxDelay = 0;
      break;

	//BLBLBL Version simplifiée, pour voir :
	/*    06/12/2010 apparement on a pas mal de prises rouges avec ça + la nouvelle gestion des accusés
	    case 10: //Ack, on ne demande pas un ACK à un ACK...
		    dwMaxAck = 0;
            dwMaxDelay = 0;
        break;

		case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8://mouvements
		case 9:  case 16: case 17: case 33: case 37: case 44: case 47: case 53:
		case 54: case 55: case 56:		
			dwMaxAck = 1;
			dwMaxDelay = 250;
		break;

      default: 
         dwMaxAck = 3;
         dwMaxDelay = 500;
      break;*/
   };
      
   //P->EncryptPacket(); Its no longer used. Replaced by new crypt.
   P->GetBuffer(lpbBuffer, nbBuffer);
   Ctr->SendPacket(sockAddr, lpbBuffer, nbBuffer, dwMaxDelay, dwMaxAck);
}

void PacketCenter::KeepAlive(void) {//BLBLBL ça met à jour une variable à chaque "tour de boucle" pour s'assurer que le client a pas foiré (ou qu'on est pas dans le cas d'une attente infinie de reste de packet)
	
	/*char strtoto[100];//BLBLBL 08/12/2010
	sprintf(strtoto,"===KEEP ALIVE===\n");
    OutputDebugString(strtoto);*/
	dwTime = GetTickCount();
	LOG << "===KEEP ALIVE===\n" << "\r\n";
	LOG << "* KEP = " << dwTime << "\r\n";
	TFCPacket Send; 
	Send << (short)10;
	SEND_PACKET(Send);
}

BOOL PacketCenter::isAlive(void) {
	//return true;
	DWORD dwTest = GetTickCount();			
	if ((dwTime < dwTest) && ((dwTest - dwTime) > 120000)) {//BLBLBL Au bout de 2 (120000) minutes on considère que le client est foutu
		LOG << "* BRK = " << dwTime << ", " << dwTest << "\r\n";//BLBLBL je vais baisser ce délai à 30 secondes, c'est déjà TRES long.
		return FALSE;										    //06/12/2010 BLBLBL je met à 60000
	}															//07/12/2010 BLBLBL je remet à 120000
	
	return TRUE;
}

void PacketCenter::SetAlive(DWORD dwT) {
   dwTime = (GetTickCount() - (120000-dwT));//BLBLBL 120000=>60000 07/12/2010 ==> 120000
}


BOOL PacketCenter::isHalf(void) {
	DWORD dwTest = GetTickCount();			
	if ((dwTime < dwTest) && ((dwTest - dwTime) > 5000)) {//1500 //BLBLBL : c'est ça qui fait afficher la Prise rouge, je le met à la moitiée du temps qui considère qu'on est out après.//07/12/2010 je remet 1500 08/12/2010 je met 3000 // steph 5000 au lieu de 3000
		return FALSE;									  
	}
	
	return TRUE;
}

void PacketCenter::LongLive(void) {
	dwTime = -1;
}

CCommCenter *PacketCenter::GetCommCenter(){
    return Ctr;
}