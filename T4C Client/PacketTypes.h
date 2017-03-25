// *******************************************************************************************
// ***                                                                                     ***
//      File Name: PacketTypes.h
//      Project:   TFC Server
//      Plateform: Windows NT 4.0 Workstation/Server
//      Creation:  6/2/1998
//      Author:    Francois Leblanc (FL)
// ***                                                                                     ***
// *******************************************************************************************
// ***                                                                                     ***
//      Change History
//
//         Date            Ver.      Author         Purpose
//         ----            ----      ------         -------
//         6/2/1998      1.0        FL             Initial developpement
//
//      Description
//          This file defines all the different packet types
//          
// ***                                                                                     ***
//	*******************************************************************************************
// ***           Copyright (c) 1997-1998 Vircom. All rights reserved.                      ***

#ifndef __PACKET_TYPES
#define __PACKET_TYPES

typedef short RQ_SIZE;

/*#define	RQ_MoveNorth					1 
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
#define RQ_GuildInviteAnswer			130*/

/*#define	RQ_MoveNorth					1 
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
#define RQ_GetPvpRanking				131*/

#define	RQ_MoveNorth					1 //BLBLBL 07/12/2010 from server TFC_MAIN.cpp
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
#define RQ_PutPlayerInGame				13 // async
#define RQ_RegisterAccount				14 // async
#define RQ_DeletePlayer					15 // async
#define RQ_SendPeriphericObjects		16 
#define RQ_GodCreateObject				17 
#define RQ_ViewBackpack					18
#define RQ_ViewEquiped					19 
#define RQ_ExitGame						20 
#define RQ_EquipObject					21 
#define RQ_UnequipObject				22 
#define RQ_UseObject					23 
#define RQ_Attack						24 
#define RQ_CreatePlayer					25 // async
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
#define RQ_QueryServerVersion           65
#define RQ_MessageOfDay                 66
#define RQ_ManaChanged                  67
#define RQ_PuppetInformation            68
#define RQ_UnitUpdate                   69
#define RQ_MissingUnit                  70
#define RQ_QueryUnitExistence           71
#define RQ_UseItemByAppearance          72
#define RQ_CannotFindItemByAppearance   73
#define RQ_EnterChatterChannel		    48
#define RQ_SendChatterMessage	          49
#define RQ_GetChatterUserList		       50
#define RQ_RemoveFromChatterChannel     74
#define RQ_GetChatterChannelList        75
#define RQ_ToggleChatterListening       86
#define RQ_UpdateGroupMembers           76
#define RQ_UpdateGroupInviteList        77
#define RQ_GroupInvite                  78
#define RQ_GroupJoin                    79
#define RQ_GroupLeave                   80
#define RQ_GroupKick                    81
#define RQ_NotifyGroupDisband           82
#define RQ_CreateEffectStatus           83
#define RQ_DispellEffectStatus          84
#define RQ_JunkItems                    85
#define RQ_UpdateGroupMemberHp          87
#define RQ_GroupToggleAutoSplit         88
#define RQ_TogglePage                   89
#define RQ_QueryNameExistence           90
#define RQ_QueryPatchServerInfo         91
#define RQ_UpdateWeight                 92
#define RQ_Rob                          93
#define RQ_DispellRob                   94
#define RQ_ArrowMiss                    95
#define RQ_ArrowHit                     96
#define RQ_GodFlagUpdate                97
#define RQ_SeraphArrival                98
#define RQ_AuthenticateServerVersion    99
#define RQ_Remort                       100

#define RQ_InfoMessage					102
#define RQ_MaxCharactersPerAccountInfo  103
#define RQ_WeatherMsg					104
#define RQ_OpenURL						105
// Chest Packets :D
#define RQ_ChestContents				106
#define RQ_ChestAddItemFromBackpack		107
#define RQ_ChestRemoveItemToBackpack	108
#define RQ_ShowChest					109
#define RQ_HideChest					110
// Trade Packets :D
#define RQ_TradeContents				111
#define RQ_TradeAddItemFromBackpack		112
#define RQ_TradeRemoveItemToBackpack	113
#define RQ_TradeInvite					116
#define RQ_TradeSetStatus				117
#define	RQ_TradeCancel					118
#define RQ_TradeClear					119
#define RQ_TradeStarted					120
#define RQ_TradeFinish					121

#define RQ_QueryItemInfo				122

#define RQ_SafePlug						123

// Guilds
#define RQ_GuildGetMembers				124//usused for now.
#define RQ_GuildInvite					125
#define RQ_GuildKick					126
#define RQ_GuildLeave					127
#define RQ_GuildAlterRights				128
#define RQ_GuildRename					129
#define RQ_GuildInviteAnswer			130
//by asteryth pvp ranking
#define RQ_GetPvpRanking				131

enum ePlacesToSearch { PL_SEARCHBACKPACK=0, PL_SEARCHCHEST=1, PL_SEARCHMYTRADE=2, PL_SEARCHOTHERTRADE=3 };


#endif