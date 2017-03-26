// ChatterUI.cpp
//
#pragma warning( disable : 4291 )

#include "../pch.h"

#include "ChatterUI.h"
#include "RootBoxUI.h"
#include <process.h>
#include "../MemoryManager.h"
#include "../SysMsg.h"
#include "../App.h"
#include "../Global.h"
#include "../SaveGame.h"

#include "CharacterUI.h"
#include "InventoryUI.h"
#include "SpellUI.h"
#include "GroupPlayUI.h"
#include "OptionsUI.h"
#include "chestUI.h"
#include "RTMap.h"
#include "RTHelp.h"
#include "RobUI.h"
#include "SkillTeachUI.h"
#include "SkillTrainUI.h"
#include "TradeUI.h"
#include "..\ObjectListing.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]
#include "..\PacketTypes.h"

using namespace std;

//const RQ_SendChatterMessage = 49;
//const RQ_GetChannelUsers = 50;

HANDLE ChatterUI::channelRefreshThreadHandle = NULL;

extern Global g_Global;
extern CSaveGame g_SaveGame;
extern DWORD SpellID;
extern Font *fNewGuiBtnFont;
extern Font *fNewGuiFont;

void ParseText( char *pText);

extern TemplateList <SysMsg> tlSystemMessage;
void ShowSystemMessage(char *szText, int iColor, int iTime, bool allowNewLine ) 
{ 
    SysMsg *Msg = new SysMsg; 
    
    Msg->SetText( szText, iColor, iTime, allowNewLine ,g_Global.GetScreenW()-180);
    
    tlSystemMessage.Lock("Add Message");
    if(!ChatterUI::GetInstance()->IsShown())
    {
       tlSystemMessage.AddToTail(Msg); 
       if(tlSystemMessage.GetNbObjects() >5)
       {
         tlSystemMessage.ToHead();
         SysMsg *Msg = tlSystemMessage.GetObject();
         delete Msg;
         tlSystemMessage.RemoveObject();
       }

    }
    else
    {
       tlSystemMessage.DestroyList(); 
    }
    tlSystemMessage.Unlock("Add Message");
    
}

namespace{
    T3VSBSound *naSound = NULL;

    void InitSound(){
        if( naSound == NULL ){
            naSound = new T3VSBSound;
            naSound->Create( "General NA sound", TS_MEMORY );
        }
    }

};

// Constructor ///////////////////////////////////////////////////////////////////////////
ChatterUI::ChatterUI( void )
:
    channelList( channelListEvent ),
    userList( userListEvent ),
    joinButton( joinButtonEvent ),
    leaveButton( leaveButtonEvent ),
    pageToggleButton( pageToggleButtonEvent ),
    channelToggleButton( channelToggleButtonEvent ),
    ingameToggleButton( ingameToggleButtonEvent ),
    ignoreButton( ignoreButtonEvent ),
    rememberButton( rememberButtonEvent ),
    pageoffButton( pageButtonEvent ),
    pageButton( pageButtonEvent ),
    ignoreListButton( ignoreListButtonEvent ),
    textInputState( SendToGame ),
    X_Button( hideEvent ),
    //option Bar
    charSheetButton( charSheetEvent ),
    invButton( invEvent ),
    spellsButton( spellsEvent ),
    groupPlayButton( groupPlayEvent ),
    chatterButton( chatterEvent ),
    macroButton( macroEvent ),
    optionsButton( optionsEvent ),
    charSheetEvent( CharSheet ),
    invEvent( Inv ),
    spellsEvent( Spells ),
    groupPlayEvent( GroupPlay ),
    chatterEvent( Chatter ),
    macroEvent( Macro ),
    optionsEvent( Options ),
    MinHButton(minHButtonEvent),
    macroBtn0(macro0Event),
    macroBtn1(macro1Event),
    macroBtn2(macro2Event),
    macroBtn3(macro3Event),
    macroBtn4(macro4Event),
    macroBtn5(macro5Event)
//////////////////////////////////////////////////////////////////////////////////////////
{
    InitSound();

    int dwXPos = (g_Global.GetScreenW()-576)/2;
    int dwYPos = (g_Global.GetScreenH()-150-368)/2;
    if(dwYPos <0)
       dwYPos =0;

    
    SetPos( FramePos( 0, dwYPos, g_Global.GetScreenW(), g_Global.GetScreenH() ) );
    SetHelpId( this, 323 );

    X_Button.SetPos( FramePos( dwXPos+576-28, dwYPos+0, dwXPos+576-4, dwYPos+0+24 ) );
    X_Button.SetSpriteId( "GUI_X_ButtonUp" );
    X_Button.SetDownUI( "GUI_X_ButtonDown" );
    X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
    SetHelpId( X_Button, 261 );
    

    backChatCC.SetSpriteId( "GUI_ChatterBack" );    
    backChatCC.SetPos( FramePos( dwXPos, dwYPos, dwXPos+576, dwYPos+368 ) );
    backChatCC.DrawAlpha(true);
    
    

    //dwXPos+, dwYPos+, dwXPos+, dwYPos+
    channelListScrollUp.SetPos  ( FramePos( dwXPos+183, dwYPos+61 , dwXPos+206, dwYPos+81 ) );
    channelListScrollDown.SetPos( FramePos( dwXPos+183, dwYPos+178 , dwXPos+206, dwYPos+198 ) );

    userListScrollUp.SetPos     ( FramePos( dwXPos+530, dwYPos+61 , dwXPos+550, dwYPos+81 ) );
    userListScrollDown.SetPos   ( FramePos( dwXPos+530, dwYPos+261, dwXPos+550, dwYPos+282 ) );
    
    userQtyTxt.SetPos( FramePos( dwXPos+227, dwYPos+294, dwXPos+518, dwYPos+305 ) );
    userQtyTxt.SetCenter(true);

    selectedChannel = "Main";

    // Setup the channel listing.
    channelList.SetPos( FramePos( dwXPos+24, dwYPos+59, dwXPos+209, dwYPos+200 ) );
    channelList.SetupList( 2, 16, FramePos( 0, 0, 5, 5 ) );
    channelList.SetColumnSize( 0, 127 );
    channelList.SetColumnSize( 1, 16  );
    channelList.CenterColumn( 1 );
    channelList.SetScrollRegions(
        FramePos( dwXPos+183, dwYPos+61  , dwXPos+206, dwYPos+81 ),
        FramePos( dwXPos+183, dwYPos+178 , dwXPos+206, dwYPos+198 ),
        FramePos( dwXPos+183, dwYPos+82  , dwXPos+206, dwYPos+170 )
    );
    channelListingSelectionGraphPatch.SetPos( FramePos( 0, 0, 132, 16 ) );
    channelListingSelectionGraphPatch.SetSpriteId( "CUI_ChatSelectCC" );
    channelListingSelectionGraphPatch.Show();
    channelList.SetSelectionSprite( &channelListingSelectionGraphPatch, -3);
    channelListEvent.SetUI( this );
    channelList.SetScrollDownDownUI( &channelListScrollDown );
    channelList.SetScrollUpDownUI( &channelListScrollUp );
    SetHelpId( channelList, 316 );

    // Setup the user listing
    userList.SetPos( FramePos( dwXPos+227, dwYPos+59, dwXPos+552, dwYPos+284 ) );
    userList.SetupList( 2, 16, FramePos( 0, 0, 5, 5 ) );
    userList.SetColumnSize( 0, 144 );
    userList.SetColumnSize( 1, 146 );
    userList.SetScrollRegions(
        FramePos( dwXPos+530, dwYPos+61 , dwXPos+553, dwYPos+81 ),
        FramePos( dwXPos+530, dwYPos+261, dwXPos+553, dwYPos+282 ),
        FramePos( dwXPos+530, dwYPos+82 , dwXPos+553, dwYPos+253 )
    );
    userListingSelectionGraphPatch.SetSpriteId( "CUI_ChatSelectUser" );
    userListingSelectionGraphPatch.SetPos( FramePos( 0, 0, 300, 16 ) );
    userListingSelectionGraphPatch.Show();
    userList.SetSelectionSprite( &userListingSelectionGraphPatch, -3 );
    userListEvent.SetUI( this );
    userList.SetScrollDownDownUI( &userListScrollDown );
    userList.SetScrollUpDownUI( &userListScrollUp );
    SetHelpId( userList, 317 );

    // Setup the buttons
    joinButton.SetPos( FramePos( dwXPos+16, dwYPos+218 , dwXPos+16+44, dwYPos+218+44 ) );
    joinButtonEvent.SetUI( this );
    SetHelpId( joinButton, 309 );
    joinButton.SetSpriteId   ( "GUI_ChatAddCCUp");
    joinButton.SetDownUI     ( "GUI_ChatAddCCDown" );
    joinButton.SetHighlightUI( "GUI_ChatAddCCHUp" );

    leaveButton.SetPos( FramePos( dwXPos+76, dwYPos+218 , dwXPos+76+44, dwYPos+218+44 ) );
    leaveButtonEvent.SetUI( this );
    SetHelpId( leaveButton, 310 );
    leaveButton.SetSpriteId   ( "GUI_ChatDelCCUp");
    leaveButton.SetDownUI     ( "GUI_ChatDelCCDown" );
    leaveButton.SetHighlightUI( "GUI_ChatDelCCHUp" );

    pageButton.SetPos( FramePos( dwXPos+136, dwYPos+218 , dwXPos+136+44, dwYPos+218+44 ) );
    pageButtonEvent.SetUI( this );
    SetHelpId( pageButton, 313 );
    pageButton.SetSpriteId   ( "GUI_ChatPageUp");
    pageButton.SetDownUI     ( "GUI_ChatPageDown" );
    pageButton.SetHighlightUI( "GUI_ChatPageHUp" );

    pageoffButton.SetSpriteId( "GUI_ButtonIconOff" );
    pageoffButton.SetPos( FramePos( dwXPos+136, dwYPos+218 , dwXPos+136+44, dwYPos+218+44 ) );
    pageoffButton.Hide();
    SetHelpId( pageoffButton, 313 );

    

    ignoreButton.SetPos( FramePos( dwXPos+16, dwYPos+271 , dwXPos+16+44, dwYPos+271+44 ) );
    ignoreButtonEvent.SetUI( this );
    SetHelpId( ignoreButton, 311 );
    ignoreButton.SetSpriteId   ( "GUI_ChatAddIgnoreUp" );
    ignoreButton.SetDownUI     ( "GUI_ChatAddIgnoreDown" );
    ignoreButton.SetHighlightUI( "GUI_ChatAddIgnoreHUp" );

    rememberButton.SetPos( FramePos( dwXPos+76, dwYPos+271 , dwXPos+76+44, dwYPos+271+44 ) );
    rememberButtonEvent.SetUI( this );
    SetHelpId( rememberButton, 312 );
    rememberButton.SetSpriteId   ( "GUI_ChatDelIgnoreUp" );
    rememberButton.SetDownUI     ( "GUI_ChatDelIgnoreDown" );
    rememberButton.SetHighlightUI( "GUI_ChatDelIgnoreHUp" );

    ignoreListButton.SetPos( FramePos( dwXPos+136, dwYPos+271 , dwXPos+136+44, dwYPos+271+44 ) );
    ignoreListButtonEvent.SetUI( this );
    SetHelpId( ignoreListButton, 314 );
    ignoreListButton.SetSpriteId   ( "GUI_ChatIgnoreUp" );
    ignoreListButton.SetDownUI     ( "GUI_ChatIgnoreDown" );
    ignoreListButton.SetHighlightUI( "GUI_ChatIgnoreHUp" );

    // Text input always has a cursor.
	// by PoPo modified to be equal 0 for a default resolution 1920 with 1912px sprite
	//int dwOffset = 729+8-(g_Global.GetScreenW()-287);
	//dwOffset*=-1;
	int dwOffset = 1625+8-(g_Global.GetScreenW()-287);
	dwOffset*=-1;

    textInput.EnableCursor( true );
    textInput.SetPos( FramePos( 40,g_Global.GetScreenH()-28, dwOffset+8+737-48-48, g_Global.GetScreenH()-8 ) );
    //textInput.SetPos( FramePos( dwOffset-40,g_Global.GetScreenH()-28, dwOffset+8+737-48-48, g_Global.GetScreenH()-8 ) );
    SetHelpId( textInput, 259 );
    
    // The different chat toggles.   
    channelToggleButton.SetSpriteId( "GUI_MinChatBtnInChatUp" );
    channelToggleButton.SetPos( FramePos( 8, g_Global.GetScreenH()-33, 32, g_Global.GetScreenH()-8) );
    channelToggleButtonEvent.SetUI( this );
    SetHelpId( channelToggleButton, 255 );
    channelToggleButton.SetDownUI( "GUI_MinChatBtnInChatDown" );
    channelToggleButton.SetHighlightUI( "GUI_MinChatBtnInChatHUp" );
    channelToggleButton.Hide();
        
    ingameToggleButton.SetSpriteId( "GUI_MinChatBtnInGameUp");
    ingameToggleButton.SetPos( FramePos( 8, g_Global.GetScreenH()-33, 32, g_Global.GetScreenH()-8) );
    ingameToggleButtonEvent.SetUI( this );
    SetHelpId( ingameToggleButton, 256 );
    ingameToggleButton.SetDownUI( "GUI_MinChatBtnInGameDown" );
    ingameToggleButton.SetHighlightUI( "GUI_MinChatBtnInGameHUp" );
    
    pageToggleButton.SetSpriteId( "GUI_MinChatBtnInPageUp" );
    pageToggleButton.SetPos( FramePos( 8, g_Global.GetScreenH()-33, 32, g_Global.GetScreenH()-8) );    
    pageToggleButtonEvent.SetUI( this );
    SetHelpId( pageToggleButton, 257 );
    pageToggleButton.SetDownUI( "GUI_MinChatBtnInPageDown" );
    pageToggleButton.SetHighlightUI( "GUI_MinChatBtnInPageHUp" );
    pageToggleButton.Hide();

    pageNameEdit.EnableCursor( true );
    pageNameEdit.SetPos( FramePos( 40,g_Global.GetScreenH()-28, 135 , g_Global.GetScreenH()-8 ) );
    pageNameEdit.SetParent( this );
    pageNameEdit.Hide();
    SetHelpId( pageNameEdit, 258 );
    
    pageNameGraphPatch.SetSpriteId( "GUI_MinChatPage" );
    pageNameGraphPatch.SetPos( FramePos( 130, g_Global.GetScreenH()-39, 158, g_Global.GetScreenH() ) );
    pageNameGraphPatch.Hide();


    backscrollScroll.SetSpriteId( "GUI_ScrollTick" );
    backscrollScroll.SetPos( FramePos( 0, 0, 24, 22) );
    backscrollScrollUp  .SetPos( FramePos( dwOffset+8+636, g_Global.GetScreenH()-141, dwOffset+8+660, g_Global.GetScreenH()-125 ) );
    backscrollScrollDown.SetPos( FramePos( dwOffset+8+636, g_Global.GetScreenH()-58 , dwOffset+8+660, g_Global.GetScreenH()-44 ) );

    textBackscroll.SetPos     ( FramePos( 0 , g_Global.GetScreenH()-150, dwOffset+8+660, g_Global.GetScreenH()-40 ) );
    textBackscroll.SetTextArea( FramePos( 12, g_Global.GetScreenH()-133, dwOffset+8+610, g_Global.GetScreenH()-44 ) );
    textBackscroll.SetScrollRegions(
        FramePos( dwOffset+8+636, g_Global.GetScreenH()-141, dwOffset+8+660, g_Global.GetScreenH()-119),
        FramePos( dwOffset+8+636, g_Global.GetScreenH()-67 , dwOffset+8+660, g_Global.GetScreenH()-45),
        FramePos( dwOffset+8+636, g_Global.GetScreenH()-123, dwOffset+8+649, g_Global.GetScreenH()-68)
    );
    SetHelpId( textBackscroll, 254 );
    textBackscroll.SetScrollDownDownUI( &backscrollScrollDown );
    textBackscroll.SetScrollUpDownUI( &backscrollScrollUp );
    textBackscroll.SetScrollUI( &backscrollScroll );

    //DEBUT New GUI menu toujours en bas pour le CHAT...

	// Be careful your sprite GUI_backChat must have a horizontal size of your (resolution-8) 
	// by PoPo dwOffset+8+1272 => dwOffset+8+1912 with the new sprites [+640]
	// by PoPo dwOffset+8+988 => dwOffset+8+1628 with the new sprites [+640]
	minimizedGraph.SetSpriteId("GUI_backChat");
	minimizedGraph.SetPos(FramePos(dwOffset+8, g_Global.GetScreenH()-150, dwOffset+8+1912, g_Global.GetScreenH()));
	minimizedGraph.DrawAlpha(true);

	minimizedGraphHR.SetSpriteId("GUI_backHRChat");
	minimizedGraphHR.SetPos(FramePos(dwOffset +8, g_Global.GetScreenH()-150, dwOffset+8+1912, g_Global.GetScreenH()));
	minimizedGraphHR.DrawAlpha(true);

	minimizedGraphHL.SetSpriteId("GUI_backHLChat");
	minimizedGraphHL.SetPos(FramePos(dwOffset+8, g_Global.GetScreenH()-300, dwOffset+8+1628, g_Global.GetScreenH()));
	minimizedGraphHL.DrawAlpha(true);

	minimizedGraphHHHB.SetSpriteId("GUI_backHHHBChat");
	minimizedGraphHHHB.SetPos(FramePos(dwOffset+8, g_Global.GetScreenH()-300, dwOffset+8+1628, g_Global.GetScreenH()));
	minimizedGraphHHHB.DrawAlpha(true);

	minimizedGraphHHHT.SetSpriteId("GUI_backHHHTChat");
	minimizedGraphHHHT.SetPos(FramePos(dwOffset+8, g_Global.GetScreenH()-592, dwOffset+8+1628, g_Global.GetScreenH()-300));
	minimizedGraphHHHT.DrawAlpha(true);

    sideFill .SetSpriteId( "GUI_backChatLeft" );
    sideFill .SetPos( FramePos( 0, g_Global.GetScreenH()-150, 8, g_Global.GetScreenH()));
    sideFill .DrawAlpha(true);

    sideFillH.SetSpriteId( "GUI_backHChatLeft" );
    sideFillH.SetPos( FramePos( 0, g_Global.GetScreenH()-300, 8, g_Global.GetScreenH()));
    sideFillH.DrawAlpha(true);

    sideFillHHH.SetSpriteId( "GUI_backHHHChatLeft" );
    sideFillHHH.SetPos( FramePos( 0, g_Global.GetScreenH()-592, 8, g_Global.GetScreenH()));
    sideFillHHH.DrawAlpha(true);
    
    MinHButton.SetPos( FramePos( g_Global.GetScreenW()-354+8, g_Global.GetScreenH()-33,   g_Global.GetScreenW()-354+28, g_Global.GetScreenH()-8 ) );
    MinHButton.SetSpriteId   ("GUI_MinChatBtnHChatUp"   );
    MinHButton.SetDownUI     ("GUI_MinChatBtnHChatDown" );
    MinHButton.SetHighlightUI("GUI_MinChatBtnHChatHUp"  );
    minHButtonEvent.SetUI( this );
    //SetHelpId( MinHButton, 255 );
    //MinHButton.Hide();

    // Ajoute les bouton pour les options....

    charSheetButton.SetPos( FramePos( g_Global.GetScreenW()-287+8, g_Global.GetScreenH()-48,   g_Global.GetScreenW()-287+48, g_Global.GetScreenH()-8 ) );
    invButton      .SetPos( FramePos( g_Global.GetScreenW()-241+8, g_Global.GetScreenH()-48,   g_Global.GetScreenW()-241+48, g_Global.GetScreenH()-8 ) );
    spellsButton   .SetPos( FramePos( g_Global.GetScreenW()-195+8, g_Global.GetScreenH()-48,   g_Global.GetScreenW()-195+48, g_Global.GetScreenH()-8 ) );
    macroButton    .SetPos( FramePos( g_Global.GetScreenW()-149+8, g_Global.GetScreenH()-48,   g_Global.GetScreenW()-149+48, g_Global.GetScreenH()-8 ) );
    groupPlayButton.SetPos( FramePos( g_Global.GetScreenW()-103+8, g_Global.GetScreenH()-48,   g_Global.GetScreenW()-103+48, g_Global.GetScreenH()-8 ) );
    optionsButton  .SetPos( FramePos( g_Global.GetScreenW()-57 +8, g_Global.GetScreenH()-48,   g_Global.GetScreenW()-57 +48, g_Global.GetScreenH()-8 ) );
    chatterButton  .SetPos( FramePos( g_Global.GetScreenW()-328+8, g_Global.GetScreenH()-33,   g_Global.GetScreenW()-328+28, g_Global.GetScreenH()-8 ) );

    charSheetButton.SetSpriteId("GUI_ChatBtnCharCheetUp");
    invButton      .SetSpriteId("GUI_ChatBtnBackPackUp");
    spellsButton   .SetSpriteId("GUI_ChatBtnSpellUp");
    macroButton    .SetSpriteId("GUI_ChatBtnMacroUp");
    groupPlayButton.SetSpriteId("GUI_ChatBtnGroupPlayUp");
    optionsButton  .SetSpriteId("GUI_ChatBtnOptionUp");
    chatterButton  .SetSpriteId("GUI_MinChatBtnInChatUp");
    

    charSheetButton.SetHighlightUI( "GUI_ChatBtnCharCheetHUp");
    invButton      .SetHighlightUI( "GUI_ChatBtnBackPackHUp");
    spellsButton   .SetHighlightUI( "GUI_ChatBtnSpellHUp");
    macroButton    .SetHighlightUI( "GUI_ChatBtnMacroHUp");
    groupPlayButton.SetHighlightUI( "GUI_ChatBtnGroupPlayHUp");
    optionsButton  .SetHighlightUI( "GUI_ChatBtnOptionHUp");
    chatterButton  .SetHighlightUI( "GUI_MinChatBtnInChatHUp");
    
    charSheetButton.SetDownUI( "GUI_ChatBtnCharCheetDown");
    invButton      .SetDownUI( "GUI_ChatBtnBackPackDown");
    spellsButton   .SetDownUI( "GUI_ChatBtnSpellDown");
    macroButton    .SetDownUI( "GUI_ChatBtnMacroDown");
    groupPlayButton.SetDownUI( "GUI_ChatBtnGroupPlayDown");
    optionsButton  .SetDownUI( "GUI_ChatBtnOptionDown");
    chatterButton  .SetDownUI( "GUI_MinChatBtnInChatDown");


    macroBtn0.SetPos( FramePos( g_Global.GetScreenW()-277, g_Global.GetScreenH()-101,   g_Global.GetScreenW()-240, g_Global.GetScreenH()-62 ) );
    macroBtn1.SetPos( FramePos( g_Global.GetScreenW()-231, g_Global.GetScreenH()-101,   g_Global.GetScreenW()-194, g_Global.GetScreenH()-62 ) );
    macroBtn2.SetPos( FramePos( g_Global.GetScreenW()-185, g_Global.GetScreenH()-101,   g_Global.GetScreenW()-148, g_Global.GetScreenH()-62 ) );
    macroBtn3.SetPos( FramePos( g_Global.GetScreenW()-140, g_Global.GetScreenH()-101,   g_Global.GetScreenW()-101, g_Global.GetScreenH()-62 ) );
    macroBtn4.SetPos( FramePos( g_Global.GetScreenW()-95 , g_Global.GetScreenH()-101,   g_Global.GetScreenW()-56 , g_Global.GetScreenH()-62 ) );
    macroBtn5.SetPos( FramePos( g_Global.GetScreenW()-49 , g_Global.GetScreenH()-101,   g_Global.GetScreenW()-10 , g_Global.GetScreenH()-62 ) );
    

    SetHelpId( charSheetButton, 478 );
    SetHelpId( invButton, 479 );
    SetHelpId( spellsButton, 480 );
    SetHelpId( groupPlayButton, 481 );
    SetHelpId( chatterButton, 482 );
    SetHelpId( macroButton, 483 );
    SetHelpId( optionsButton, 484 );
	 SetHelpId( this, 544 );


    
    // Add the children uis.
    AddChild( &backChatCC);
    AddChild( &X_Button);
    AddChild( &macroBtn0);
    AddChild( &macroBtn1);
    AddChild( &macroBtn2);
    AddChild( &macroBtn3);
    AddChild( &macroBtn4);
    AddChild( &macroBtn5);
    AddChild( &channelList );
    AddChild( &userList );
    AddChild( &joinButton );
    AddChild( &leaveButton );
    AddChild( &textBackscroll );
    AddChild( &ingameToggleButton );
    AddChild( &channelToggleButton );        
    AddChild( &pageNameGraphPatch );
    AddChild( &textInput );
    AddChild( &pageNameEdit );
    AddChild( &pageToggleButton );
    AddChild( &ignoreButton );
    AddChild( &rememberButton );
    AddChild( &pageButton );
    AddChild( &pageoffButton );
    AddChild( &ignoreListButton );
    AddChild( &userQtyTxt );
    AddChild( &MinHButton);
    //Bouton pour l<option Bar
    AddChild( &charSheetButton );
    AddChild( &invButton );
    AddChild( &spellsButton );
    AddChild( &groupPlayButton );
    AddChild( &chatterButton );
    AddChild( &macroButton );
    AddChild( &optionsButton );


    // Send to game.
    textInputState = SendToGame;

	//Initialize the typed history list
	rollbackTyped.push_back("");
	ii = rollbackTyped.end();
    
    // If its the first time the object initializes
    if( channelRefreshThreadHandle == NULL ){
        // Start the channel refreshing thread.
        unsigned int threadId = 0;
        channelRefreshThreadHandle = 
            reinterpret_cast< HANDLE >( 
                _beginthreadex( NULL, 0, ChannelRefreshThread, NULL, 0, &threadId )
            );
    }

    Load();

	Hide();
    Minimize( true );
    SetMinimizedHight(false);
    m_bOldMinH = false;

    // Add this box.
    RootBoxUI::GetInstance()->AddChild( this );
}

void ChatterUI::SetBacklogFile( char *fileName )
{
   OptionParam *pOption = g_SaveGame.GetOptionsParam();
   sprintf(pOption ->LogName,"%s",fileName);
}
char * ChatterUI::GetBacklogFile()
{
   OptionParam *pOption = g_SaveGame.GetOptionsParam();
   return pOption ->LogName;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Join popup box

//////////////////////////////////////////////////////////////////////////////////////////
ChatterUI::JoinPopupUI::JoinPopupUI( void )
:
    acceptButton( acceptEvent ),
    cancelButton( hideEvent )
//////////////////////////////////////////////////////////////////////////////////////////
{
    int dwOffX = (g_Global.GetScreenW()-240)/2;
    int dwOffY = (g_Global.GetScreenH()-164-150)/2;
    SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );

    JoinBack.SetSpriteId( "GUI_PopupBack" );
    JoinBack.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );
    // patch pour la zone de texte
    graphPatch.SetSpriteId( "GUI_JoinCCPopupBack" );
    graphPatch.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+85 ) );
    

    acceptButton.SetSpriteId   ( "GUI_ButtonUp" );
    acceptButton.SetDownUI     ( "GUI_ButtonDown" );
    acceptButton.SetHighlightUI( "GUI_ButtonHUp" );
    acceptButton.SetPos( FramePos( dwOffX+52, dwOffY+94, dwOffX+52+60, dwOffY+94+32 ) );
    acceptEvent.SetUI( this );
    SetHelpId( acceptButton, 456 );

    cancelButton.SetSpriteId   ( "GUI_ButtonUp" );
    cancelButton.SetDownUI     ( "GUI_ButtonDown" );
    cancelButton.SetHighlightUI( "GUI_ButtonHUp" );
    cancelButton.SetPos( FramePos( dwOffX+128, dwOffY+94, dwOffX+128+60, dwOffY+94+32 ) );
    hideEvent.SetUI( this );
    SetHelpId( cancelButton, 457 );

    channelEdit .SetPos( FramePos( dwOffX+111, dwOffY+20, dwOffX+215, dwOffY+40 ) );
    pwdEdit     .SetPos( FramePos( dwOffX+111, dwOffY+56, dwOffX+215, dwOffY+76 ) );
    SetHelpId( channelEdit, 461 );
    SetHelpId( pwdEdit    , 462 );

    // Writting should occur in the channel edit by default.
    lastClickedItem = &channelEdit;

    // Max 16 characters.
    channelEdit.SetMaxTextLength( 16 );
    pwdEdit.SetMaxTextLength( 16 );

    channelEdit.SetParent( this );
    pwdEdit.SetParent( this );

    channelEdit.EnableCursor( true );
    pwdEdit.EnableCursor( true );

    AddChild( &JoinBack);
    AddChild( &graphPatch );
    AddChild( &acceptButton );
    AddChild( &cancelButton );
    AddChild( &channelEdit );
    AddChild( &pwdEdit );

    RootBoxUI::GetInstance()->AddChild( this );
}


void ChatterUI::JoinPopupUI::Draw(V2SPRITEFX *vsfFX )
{
    int dwXPos = (g_Global.GetScreenW()-240)/2;
    int dwYPos = (g_Global.GetScreenH()-164-150)/2;
    
    GameUI::Draw( vsfFX );
    
    HDC hdc;
    DXDGetDC(&hdc, 715);
    char strBtnTexte[20];
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[10]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 52, dwYPos+96 , RGB(0, 0, 0)    , 0, strBtnTexte);
    
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[11]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 128, dwYPos+96 , RGB(0, 0, 0)    , 0, strBtnTexte);

    sprintf(strBtnTexte  ,"%s",g_GUILocalString[8]);
    fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,81)+ 23, dwYPos+23 , g_Global.GetBtnTCE()  , 0, strBtnTexte);
    
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[9]);
    fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,81)+ 23, dwYPos+59 , g_Global.GetBtnTCE()  , 0, strBtnTexte);
    
    DXDReleaseDC(hdc, 715);
}

int ChatterUI::JoinPopupUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::JoinPopupUI::ResetUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Resets the join popup box.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Writting should occur in the channel edit by default.
    lastClickedItem = &channelEdit;

    channelEdit.SetText( "" );
    pwdEdit.SetText( "" );
}
//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::JoinPopupUI::AcceptEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Accept clicked on the join popup box.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI::JoinPopupUI *popup = static_cast< ChatterUI::JoinPopupUI * >( GetUI() );

    popup->Accept();
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ChatterUI::JoinPopupUI::TextInput
//////////////////////////////////////////////////////////////////////////////////////////
// Keyboard hit in the join popup UI.
// 
(
 char ch   // The pressed character.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Enter pressed.        
    if( ch == '\r' ){
        Accept();
    }else 
    // Tab pressed
    if( ch == '\t' ){

        if( GetLastClickedItem() == &channelEdit ){
            lastClickedItem = &pwdEdit;
        }else if( GetLastClickedItem() == &pwdEdit ){
            lastClickedItem = &channelEdit;
        }
        
    }else if( ch != 0 ){
        // Give the kbhit event to the last clicked control.
        if( GetLastClickedItem() != NULL ){
            GetLastClickedItem()->TextInput( ch );
        }
    }

    

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ChatterUI::JoinPopupUI::VKeyInput
//////////////////////////////////////////////////////////////////////////////////////////
// Process escape character.
// 
(
 VKey vKey
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( vKey.GetKey() == VK_ESCAPE ){
        Hide();
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::JoinPopupUI::Accept( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Called when the user has accepted.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI::GetInstance()->EnterChatterChannel( channelEdit.GetText(), pwdEdit.GetText(), ChannelUI::cyan ,true);

    Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Ignore list popup box.

// Constructor ///////////////////////////////////////////////////////////////////////////
ChatterUI::IgnoreListPopupUI::IgnoreListPopupUI( void )
:
    rememberButton( rememberButtonEvent ),
    addButton( addButtonEvent ),
    ignoreList( EmptyEvent::GetInstance() ),
    quitButton( hideEvent )
//////////////////////////////////////////////////////////////////////////////////////////
{
    int dwXPos = (g_Global.GetScreenW()-232)/2;
    int dwYPos = (g_Global.GetScreenH()-279-150)/2;
    SetPos( FramePos( dwXPos, dwYPos, dwXPos+240, dwYPos+164 ) );

    IgnoreBack.SetSpriteId( "GUI_BackIgnore" );
    IgnoreBack.SetPos( FramePos( dwXPos, dwYPos, dwXPos+232, dwYPos+279 ) );

    quitButton.SetPos        ( FramePos( dwXPos+232-28, dwYPos+0, dwXPos+232-4, dwYPos+0+24 ) );
    quitButton.SetSpriteId   ( "GUI_X_ButtonUp" );
    quitButton.SetDownUI     ( "GUI_X_ButtonDown" );
    quitButton.SetHighlightUI( "GUI_X_ButtonHUp" );
    SetHelpId( quitButton, 261 );

    addButton.SetPos( FramePos( dwXPos+22, dwYPos+31, dwXPos+77, dwYPos+57 ) );
    addButton.SetSpriteId   ( "GUI_ButtonSUp" );
    addButton.SetDownUI     ( "GUI_ButtonSDown" );
    addButton.SetHighlightUI( "GUI_ButtonSHUp" );
    addButtonEvent.SetUI( this );
    SetHelpId( addButton, 459 );
       
    rememberButton.SetPos( FramePos( dwXPos+86, dwYPos+220, dwXPos+86+60, dwYPos+242 ) );
    rememberButton.SetSpriteId   ( "GUI_ButtonTUp" );
    rememberButton.SetDownUI     ( "GUI_ButtonTDown" );
    rememberButton.SetHighlightUI( "GUI_ButtonTHUp" );
    rememberButtonEvent.SetUI( this );     
    SetHelpId( rememberButton, 312 );
     
    

    addEdit.SetPos( FramePos( dwXPos+82, dwYPos+37, dwXPos+203, dwYPos+51 ) );
    addEdit.SetParent( this );
    addEdit.EnableCursor( true );
    addEdit.SetMaxTextLength( 15 );
    SetHelpId( addEdit, 458 );
    
    

    scrollUpGraph.SetPos  ( FramePos( dwXPos+180, dwYPos+66 , dwXPos+203, dwYPos+88  ) );
    scrollDownGraph.SetPos( FramePos( dwXPos+180, dwYPos+183, dwXPos+203, dwYPos+207 ) );

    ignoreList.SetPos( FramePos( dwXPos+28, dwYPos+66, dwXPos+203, dwYPos+207 ) );
    ignoreList.SetupList( 1, 12, FramePos( 0, 0, 0, 5 ) );
    ignoreList.SetScrollUpDownUI( &scrollUpGraph );
    ignoreList.SetScrollDownDownUI( &scrollDownGraph );
    ignoreList.SetScrollRegions(
        FramePos( dwXPos+180, dwYPos+66 , dwXPos+203, dwYPos+88  ),
        FramePos( dwXPos+180, dwYPos+183, dwXPos+203, dwYPos+207 ),
        FramePos( dwXPos+180, dwYPos+89 , dwXPos+203, dwYPos+185 )
    );
    selSprite.SetSpriteId( "GUI_IgnoreSelect" );
    selSprite.SetPos( FramePos( 0, 0, 146, 16 ) );
    ignoreList.SetSelectionSprite( &selSprite );
    ignoreList.SetColumnSize( 0, 146 );
    SetHelpId( ignoreList, 460 );

    AddChild( &IgnoreBack );
    AddChild( &quitButton );
    AddChild( &rememberButton );
    AddChild( &addButton );
    AddChild( &addEdit );
    AddChild( &ignoreList );
    
    RootBoxUI::GetInstance()->AddChild( this );
}

void ChatterUI::IgnoreListPopupUI::Draw(V2SPRITEFX *vsfFX )
{
    int dwXPos = (g_Global.GetScreenW()-232)/2;
    int dwYPos = (g_Global.GetScreenH()-279-150)/2;
    
    GameUI::Draw( vsfFX );
    
    HDC hdc;
    DXDGetDC(&hdc, 716);
    char strBtnTexte[20];

    // Draw Box Header
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[12]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,102)+ 59+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,102)+ 59  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);

    sprintf(strBtnTexte  ,"%s",g_GUILocalString[13]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,53)+ 22, dwYPos+35 , RGB(0, 0, 0)    , 0, strBtnTexte);
    
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[14]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 86, dwYPos+219 , RGB(0, 0, 0)    , 0, strBtnTexte);

    DXDReleaseDC(hdc, 716);
}

int ChatterUI::IgnoreListPopupUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}


//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::IgnoreListPopupUI::ResetUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Resets the UI each time it pops up.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ignoreList.DeleteList();

    ChatterUI *chatterUI = ChatterUI::GetInstance();

    IgnoreSet::iterator i;
    for( i = chatterUI->ignoreList.begin(); i != chatterUI->ignoreList.end(); i++ ){
        ignoreList.AddNewItem();

        UserIgnoreUI *userUI = new UserIgnoreUI;

        userUI->userID = (*i);
        userUI->SetText( (*i) );
        userUI->SetPos( FramePos( 0, 0, userUI->GetWidth(), userUI->GetHeight() ) );

        ignoreList.AddColumnItem( 0, userUI );
    }
    
    addEdit.SetText( "" );
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::IgnoreListPopupUI::AddButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Add button was clicked.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    IgnoreListPopupUI *ignorePopup = static_cast< IgnoreListPopupUI * >( GetUI() );
    
    ignorePopup->Add();
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::IgnoreListPopupUI::RememberButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Remember button was clicked.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    IgnoreListPopupUI *ignorePopup = static_cast< IgnoreListPopupUI * >( GetUI() );

    UserIgnoreUI *userUI = static_cast< UserIgnoreUI * >( ignorePopup->ignoreList.GetSelection() );

    // If a user was selected.
    if( userUI != NULL ){
        ChatterUI::GetInstance()->GetIgnoreList()->erase( userUI->userID );

        ignorePopup->ResetUI();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::IgnoreListPopupUI::Add( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Add the currently typed user name to the list of users.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    string newUser = addEdit.GetText();
    
    // If a name was written in the add edit box.
    if( !newUser.empty() && stricmp( newUser.c_str(), Player.Name ) != 0 ){    
        // If the name could be inserted.
        if( ChatterUI::GetInstance()->GetIgnoreList()->insert( newUser ).second ){
            ResetUI();
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::IgnoreListPopupUI::Close( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Closes the ignore list popup UI.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    Hide();    
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ChatterUI::IgnoreListPopupUI::TextInput
//////////////////////////////////////////////////////////////////////////////////////////
//  Keyboard hit in ignore list popup.
// 
(
 char ch
)
// Return: bool, true
//////////////////////////////////////////////////////////////////////////////////////////
{
    switch( ch ){
    case '\r':
        Add();
        break;
    case '\t':
        break;
    case 27:
        Close();
        break;
    default:
        addEdit.TextInput( ch );
    };

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
// ChatterUI

 // Destructor ////////////////////////////////////////////////////////////////////////////
ChatterUI::~ChatterUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
{


}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::EnterChatterChannel
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
(
 string channelID, // 
 string pwd, // 
 DWORD  color,
 bool   bEnable
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    //const RQ_EnterChatterChannel = 48;

    // If text was typed in the channel box.
    if( !channelID.empty() ){
        // Send a joins packet
        TFCPacket sending;

        sending << (RQ_SIZE)RQ_EnterChatterChannel;
        sending << (short)channelID.size();
        sending << (char *)channelID.c_str();

        // If a password was provided.
        if( !pwd.empty() ){
            sending << (short)pwd.size();
            sending << (char *)pwd.c_str();
        }else{
            sending << (short)0;
        }
        
        // Add to the list of channels to save.
        RegChannel regChannel;
        regChannel.channelID = channelID;
        regChannel.pwd       = pwd;
        regChannel.color     = color;
        regChannel.bActive   = bEnable;
        regChannels.insert( regChannel );

        SEND_PACKET( sending );

        // Preemtively add the channel
        channelList.AddNewItem();

        ChannelUI *channelUI = new ChannelUI;
        channelUI->channelColor = static_cast< ChannelUI::color >( color );
        channelUI->SetText( channelID.c_str(), channelUI->channelColor, 640, 0, 0 );
        channelUI->SetCenter( false );
        channelUI->SetPos( FramePos( 0, 0, channelUI->GetWidth(), channelUI->GetHeight() ) );
        channelUI->channelID = channelID;
        channelUI->listenState = bEnable;
        channelList.AddColumnItem( 0, channelUI );

        if( channelID == selectedChannel ){
            channelList.SetSelection( channelUI );
        }

        ChannelToggleUI *channelToggleUI = new ChannelToggleUI;
        if(bEnable)
        {
            channelToggleUI->SetSpriteId   ( "CheckONLightUP" );
            channelToggleUI->SetDownUI     ( "CheckONLightDown" );
            channelToggleUI->SetHighlightUI( "CheckONLightHUP" );
        }
        else
        {
            channelToggleUI->SetSpriteId   ( "CheckOFFLightUP" );
            channelToggleUI->SetDownUI     ( "CheckOFFLightDown" );
            channelToggleUI->SetHighlightUI( "CheckOFFLightHUP" );

            //const RQ_ToggleChatterListening = 86;
            TFCPacket sending;
            sending << (short)RQ_ToggleChatterListening;
            sending << (short)channelUI->channelID.size();
            sending << (char *)channelUI->channelID.c_str();
            sending << (char)( 0);
            SEND_PACKET( sending );
        }

        //channelToggleUI->SetCenter( false );
        channelToggleUI->SetPos( FramePos( 1, 0, 15, 16 ) );
        channelToggleUI->channelID = channelID;
        channelToggleUI->listenState = bEnable;
        channelList.AddColumnItem( 1, channelToggleUI );

        savedColor [ channelUI->channelID ] = channelUI->channelColor;
        savedStatus[ channelUI->channelID ] = channelUI->listenState;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
// Return: bool, true
//////////////////////////////////////////////////////////////////////////////////////////
bool ChatterUI::TextInput(char ch)
{       
   bool analyzeCh = true;
   
   // Escape.
   if( ch == 27 )
   {
      
      return true;
   }
   
   // If the input is SendToPage.
   if( textInputState == SendToPage )
   {
      // If the last clicked item is the page name edit control.
      if( GetLastClickedItem() == &pageNameEdit )
      {
         analyzeCh = false;
         
         if( ch == '\r' || ch == '\t' ){
            lastClickedItem = &textInput;
         }
         else
         {
            pageNameEdit.TextInput( ch );
         }
      }        
   }

    if( analyzeCh )
    {

        if( ch == '\r' )
        {   
            /*
            if(textInput.GetText()== ".help")
            {
               AddBackscrollItem("NightMare","--------------------------------------------",RGB(255,255,255),false);
               AddBackscrollItem("NightMare","Debug List Command :",RGB(255,255,255),false);
               AddBackscrollItem("NightMare",".fog on/off           // Enable fog." ,RGB(255,255,255),false);
               AddBackscrollItem("NightMare",".rain on/off          // Enable rain.",RGB(255,255,255),false);
               AddBackscrollItem("NightMare",".snow on/off          // Enable snow.",RGB(255,255,255),false);
               AddBackscrollItem("NightMare",".star on/off          // Replace Arrow by Star.",RGB(255,255,255),false);
               AddBackscrollItem("NightMare",".dagger on/off        // Replace Arrow by Dagger.",RGB(255,255,255),false);
               AddBackscrollItem("NightMare",".spell 1/2/3/4/5/6/7  // Play some spell effect.",RGB(255,255,255),false);
               AddBackscrollItem("NightMare",".help                 // This Command.",RGB(255,255,255),false);
               AddBackscrollItem("NightMare","--------------------------------------------",RGB(255,255,255),false);
               textInput.SetText( "" );
               return true;
            }
            else if(textInput.GetText()== ".fog on")
            {
               AddBackscrollItem("NightMare","FOG Enable (TEST)",SYSTEM_COLOR,false);
               textInput.SetText( "" );
               Custom.bEnableDemoFog = true;
               return true;
            }
            else if(textInput.GetText()== ".fog off")
            {
               AddBackscrollItem("NightMare","FOG Disable (TEST)",SYSTEM_COLOR,false);
               textInput.SetText( "" );
               Custom.bEnableDemoFog = false;
               return true;
            }
            else if(textInput.GetText()== ".rain on")
            {
               AddBackscrollItem("NightMare","RAIN Enable (TEST)",SYSTEM_COLOR,false);
               textInput.SetText( "" );
               Custom.bEnableDemoRain = true;
               return true;
            }
            else if(textInput.GetText()== ".rain off")
            {
               AddBackscrollItem("NightMare","RAIN Disable (TEST)",SYSTEM_COLOR,false);
               textInput.SetText( "" );
               Custom.bEnableDemoRain = false;
               return true;
            }
            else if(textInput.GetText()== ".snow on")
            {
               AddBackscrollItem("NightMare","SNOW Enable (TEST)",SYSTEM_COLOR,false);
               textInput.SetText( "" );
               Custom.bEnableDemoSnow = true;
               return true;
            }
            else if(textInput.GetText()== ".snow off")
            {
               AddBackscrollItem("NightMare","SNOW Disable (TEST)",SYSTEM_COLOR,false);
               textInput.SetText( "" );
               Custom.bEnableDemoSnow = false;
               return true;
            }
            else if(textInput.GetText()== ".spell 1")
            {
               AddBackscrollItem("NightMare","Show spell \"Skill Spell\" ",SYSTEM_COLOR,false);
               Objects.Lock(0);
               Objects.Add(SpellID, __SPELL_SKILLS_EFFECT, Player.xPos,Player.yPos, 100, 0, 0, 100);
               Objects.Unlock(0);
               return true;
            }
            */

            
            /*
            else if(textInput.GetText()== ".star on")
            {
               AddBackscrollItem("NightMare","Change Arrow for Star01 (TEST)",SYSTEM_COLOR,false);
               textInput.SetText( "" );
               Custom.bEnableCArrow = 1;
               return true;
            }
            else if(textInput.GetText()== ".star off")
            {
               AddBackscrollItem("NightMare","RESET Arrow (TEST)",SYSTEM_COLOR,false);
               textInput.SetText( "" );
               Custom.bEnableCArrow = 0;
               return true;
            }
            else if(textInput.GetText()== ".dagger on")
            {
               AddBackscrollItem("NightMare","Change Arrow for Dagger01 (TEST)",SYSTEM_COLOR,false);
               textInput.SetText( "" );
               Custom.bEnableCArrow = 2;
               return true;
            }
            else if(textInput.GetText()== ".dagger off")
            {
               AddBackscrollItem("NightMare","RESET Arrow (TEST)",SYSTEM_COLOR,false);
               textInput.SetText( "" );
               Custom.bEnableCArrow = 0;
               return true;
            }
            else if(textInput.GetText()== ".spell 1")
            {
               AddBackscrollItem("NightMare","Show spell \"Boulder Fire\" ",SYSTEM_COLOR,false);
               Objects.Lock(0);
               Objects.Add(SpellID, __SPELL_A_BOULDER_FIRE, Player.xPos,Player.yPos, 100, 0, 0, 100);
               Objects.Unlock(0);
               return true;
            }
            else if(textInput.GetText()== ".spell 2")
            {
               AddBackscrollItem("NightMare","Show spell \"NecroA01\" ",SYSTEM_COLOR,false);
               Objects.Lock(0);
               Objects.Add(SpellID, __SPELL_A_NECRO_A01, Player.xPos,Player.yPos, 100, 0, 0, 100);
               Objects.Unlock(0);
               return true;
            }
            else if(textInput.GetText()== ".spell 3")
            {
               AddBackscrollItem("NightMare","Show spell \"PoisonA01\" ",SYSTEM_COLOR,false);
               Objects.Lock(0);
               Objects.Add(SpellID, __SPELL_A_POISON_A01, Player.xPos,Player.yPos, 100, 0, 0, 100);
               Objects.Unlock(0);
               return true;
            }
            else if(textInput.GetText()== ".spell 4")
            {
               AddBackscrollItem("NightMare","Show spell \"FireA01\" ",SYSTEM_COLOR,false);
               Objects.Lock(0);
               Objects.Add(SpellID, __SPELL_A_FIRE_A01A, Player.xPos,Player.yPos, 100, 0, 0, 100);
               Objects.Unlock(0);
               return true;
            }
            else if(textInput.GetText()== ".spell 5")
            {
               AddBackscrollItem("NightMare","Show spell \"ProtectA01\" ",SYSTEM_COLOR,false);
               Objects.Lock(0);
               Objects.Add(SpellID, __SPELL_A_PROTECT_A01, Player.xPos,Player.yPos, 100, 0, 0, 100);
               Objects.Unlock(0);
               return true;
            }
            else if(textInput.GetText()== ".spell 6")
            {
               AddBackscrollItem("NightMare","Show spell \"HealA01\" ",SYSTEM_COLOR,false);
               Objects.Lock(0);
               Objects.Add(SpellID, __SPELL_A_HEAL_A01, Player.xPos,Player.yPos, 100, 0, 0, 100);
               Objects.Unlock(0);
               return true;
            }
            else if(textInput.GetText()== ".spell 7")
            {
               AddBackscrollItem("NightMare","Show spell \"HealA01AAA\" ",SYSTEM_COLOR,false);
               Objects.Lock(0);
               Objects.Add(SpellID, __SPELL_A_FIRE_A02, Player.xPos,Player.yPos, 100, 0, 0, 100);
               Objects.Unlock(0);
               return true;
            }
            else if(textInput.GetText()== ".spell 8")
            {
               AddBackscrollItem("NightMare","Show spell \"ThunderStorm\" ",SYSTEM_COLOR,false);
               Objects.Lock(0);
               Objects.Add(SpellID, __SPELL_A_THUNDERSTROM_A, Player.xPos,Player.yPos, 100, 0, 0, 100);
               Objects.Unlock(0);
               return true;
            }
            */
            

            
            
            // Determine where to send the text according to the UI's current state.
            switch( textInputState )
            {
               case SendToGame:
               {
                  char *theText = new char[ textInput.GetText().size() + 1 ];
                  strcpy( theText, textInput.GetText().c_str() );
                  ParseText( theText );
                  delete theText;
               }
               break;
               case SendToPage:
               {
                  ClientConfig *pConfig = g_Global.GetClientConfig();
                  if(!pConfig->dwAfkStatus)
                  {
                     OptionParam *pOption = g_SaveGame.GetOptionsParam();
                     if (!pOption->bPageEnable) 
                     {
                        // user ignore all page.
                        /*ChatterUI::GetInstance()->*/AddBackscrollItem("",g_LocalString[100],RGB(255,255,255),true); // DaP
                     } 
                     else 
                     {
                        if( textInput.GetText().empty() || pageNameEdit.GetText().empty() )
                        {
                           naSound->Play( TRUE );
                           break;
                        }
                        
                        TFCPacket sending;
                        sending << (RQ_SIZE)RQ_Page;
                        sending << (short)pageNameEdit.GetText().size();
                        sending << (char *)pageNameEdit.GetText().c_str();
                        sending << (short)textInput.GetText().size();
                        sending << (char *)textInput.GetText().c_str();
                        
                        if (!/*ChatterUI::GetInstance()->*/IsUserIgnored( pageNameEdit.GetText().c_str())) //TODO DaP
                        {
                           SEND_PACKET( sending );
                        } 
                        else 
                        {
							/*ChatterUI::GetInstance()->*/AddBackscrollItem( "", g_LocalString[517], RGB(155,155,155), true, false ); // DaP
                        }
                     }
                  } 
                  else 
                  {
					  /*ChatterUI::GetInstance()->*/AddBackscrollItem( "", "You cannot page while AFK!", RGB(155, 155, 155), true, false); //DaP
                  }
               }
               break;
               case SendToChannel:
               {
                  // If there is no selected channel
                  if( selectedChannel.empty() )
                  {
                     naSound->Play( TRUE );
                     AddBackscrollItem("",g_LocalString[ 487 ],SYSTEM_COLOR,false);
                     break;
                  }                           
                  
                  string msg = textInput.GetText();
                  
                  // If the text input isn't empty.
                  if( !msg.empty() )
                  {
                     LOG.Lock();
                     LOG << "\r\nCHAT: Sending " << (char*)msg.c_str() << " to channel " << (char*)selectedChannel.c_str() << ".";
                     LOG.Unlock();
                     
                     TFCPacket sending;
                     // Create a send chatter message packet.
                     sending << (RQ_SIZE)RQ_SendChatterMessage;
                     sending << (short)selectedChannel.size();
                     sending << (char *)selectedChannel.c_str();
                     sending << (short)msg.size();
                     sending << (char *)msg.c_str();
                     
                     SEND_PACKET( sending );
                  }
                  else
                  {
                     naSound->Play( TRUE );
                     LOG << "\r\nCHAT: No message";
                  }
               }
               break;
            }
            
            if ( textInput.GetText().size() > 0 ) 
            {
               rollbackTyped.push_back(textInput.GetText());
               ii = rollbackTyped.end();
               if (rollbackTyped.size() > 128) 
                  rollbackTyped.pop_front();
               textInput.SetText( "" );
            }
        }
        else
        {    
           textInput.TextInput( ch );
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ChatterUI::VKeyInput
//////////////////////////////////////////////////////////////////////////////////////////
// Virtual key inputs
// 
(
 VKey vKey
)
// Return: virtual bool, true
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( vKey.ALT() ){
		return false;
	}
    if( vKey.CTRL() ){
		switch (vKey.GetKey()) {
		case VK_LEFT: case VK_RIGHT: case VK_UP: case VK_DOWN:
			break;
		default:
			return false;
		}
    }

    switch( vKey.GetKey() )
    {
      case VK_ESCAPE:
         Hide();
      break;
      case VK_UP:
       if (vKey.CTRL()) 
       {
          if (ii != rollbackTyped.begin()) 
          {
             ii--;
             textInput.SetText(*ii);
          }
       } 
       else if ( /*RootBoxUI::GetInstance()->*/!IsMinimized() ) 
       {
          textBackscroll.ScrollDown( 1 );
       }
     break;
     case VK_DOWN:
       if (vKey.CTRL()) 
       {
          try {
             if (ii == rollbackTyped.end()) textInput.SetText("");
             else {
                ii++;
                if (ii == rollbackTyped.end()) textInput.SetText("");
                else textInput.SetText(*ii);
             }
          } catch (...) {
             textBackscroll.AddText("Program error! If you're reading this, please contact your hgm regarding command history exception at Chatter::CTRL+DOWN.", 255, true);
          }
       } else if ( /*RootBoxUI::GetInstance()->*/!IsMinimized() ) {
          textBackscroll.ScrollUp( 1 );
       }
     break;
     case VK_PRIOR:
       if( /*RootBoxUI::GetInstance()->*/!IsMinimized() )
          textBackscroll.ScrollDown( 14 );
       break;
     case VK_NEXT:
       if( /*RootBoxUI::GetInstance()->*/!IsMinimized() ){
          textBackscroll.ScrollUp( 14 );
       }
       break;
    case VK_LEFT:
       if (vKey.CTRL()) 
       {
          textInput.MoveCursorPos(-1);
       } else if (vKey.SHIFT()) 
       {
          textInput.MoveCursorPos(-1,1);
       }
       break;
    case VK_RIGHT:
       if (vKey.CTRL()) 
       {
          textInput.MoveCursorPos(1);
       } 
       else if (vKey.SHIFT()) 
       {
          textInput.MoveCursorPos(1,1);
       }
       break;
    case VK_HOME:
       if (vKey.SHIFT()) 
       {
          textInput.MoveCursorPos(-10000);
       }
       break;
    case VK_END:
       if (vKey.SHIFT()) 
       {
          textInput.MoveCursorPos(10000);
       }
       break;
    case VK_DELETE:
       if (vKey.SHIFT()) 
       {
          textInput.SetText("");
       }
       else
       {
          textInput.DeleteChar();
       }
       break;
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
ChatterUI *ChatterUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the sole instance of the chatter channel UI.
//////////////////////////////////////////////////////////////////////////////////////////
{
	if (instance == 0)
	{
		instance = new ChatterUI();
	}

	return instance;
	//static ChatterUI chatterUI;
 //   return &chatterUI;
}

/* Null, because instance will be initialized on demand. */
ChatterUI* ChatterUI::instance = 0;

unsigned int  ChatterUI::ToggleChatSize(void *null)
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
    
   ChatterUI *chatterUI = GetInstance();

   if(chatterUI->m_bOldMinH)
      return 0;
   
   if(chatterUI->IsMinimizedHight())
      chatterUI->SetMinimizedHight(false);
   else
      chatterUI->SetMinimizedHight(true);
   return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int ChatterUI::Display
//////////////////////////////////////////////////////////////////////////////////////////
//  Displays the ChatterUI
// 
(
 void *null // null
)
// Return: static unsigned, 0
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
    
    ChatterUI *chatterUI = GetInstance();

    if( chatterUI->IsShown() && !chatterUI->IsMinimized() )
    {        
        chatterUI->Hide();
    }
    else
    {
        // Maximize chatterUI.
        chatterUI->Minimize( false );    

        //const RQ_GetPublicChannelList = 75;

        chatterUI->Show();

        // Send request for list of channels on server.
        TFCPacket sending;
        sending << (short)RQ_GetChatterChannelList;//RQ_GetPublicChannelList;

        SEND_PACKET( sending );

        // If a channel is selected
        if( !chatterUI->selectedChannel.empty() ){
            // Get its user list immediatly.
            TFCPacket sending;           
            sending << (short)RQ_GetChatterUserList;//RQ_GetChannelUsers;
            sending << (short)chatterUI->selectedChannel.size();
            sending << (char *)chatterUI->selectedChannel.c_str();
            SEND_PACKET( sending );
        }
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::UpdateChannels
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the public channel list.
// 
(
 const std::vector< Channel > &publicChannels // The new list of public channels.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
    
    int dwScrollPos = 0;
    if(channelList.GetGrid()->size())
      dwScrollPos = channelList.GetScrollPos();
    channelList.DeleteList();

    set< RegChannel >::iterator j;
    vector< Channel >::const_iterator i;
    for( i = publicChannels.begin(); i != publicChannels.end(); i++ )
    {
        DWORD color   = ChannelUI::cyan;
        bool  bStatus = true;

        // Add to the list of channels to save.
        bool found = false;
        for( j = regChannels.begin(); j != regChannels.end(); j++ )
        {
            if( stricmp( (*j).channelID.c_str(), (*i).channelID.c_str() ) == 0 )
            {                
                found = true;
                break;
            }
        }
        if( !found )
        {
            RegChannel regChannel;
            regChannel.channelID = (*i).channelID;
            regChannel.color     = color;
            regChannel.bActive   = (*i).listenState;

            regChannels.insert( regChannel );
        }
        else
           

        if( savedColor.find( (*i).channelID ) != savedColor.end() )
            color = savedColor[ (*i).channelID ];    
        if( savedStatus.find( (*i).channelID ) != savedStatus.end() )
            bStatus = savedStatus[ (*i).channelID ];    

        channelList.AddNewItem();

        ChannelUI *channelUI = new ChannelUI;
        channelUI->channelColor = static_cast< ChannelUI::color >( color );
        channelUI->SetText( (*i).channelID.c_str(), color, 640, 0, 0 );
        channelUI->SetCenter( false );
        channelUI->SetPos( FramePos( 0, 0, channelUI->GetWidth(), channelUI->GetHeight() ) );
        channelUI->channelID = (*i).channelID;
        channelUI->listenState = bStatus;        
        channelList.AddColumnItem( 0, channelUI );

        if( (*i).channelID == selectedChannel ){
            channelList.SetSelection( channelUI );
        }

        ChannelToggleUI *channelToggleUI = new ChannelToggleUI;
        if( bStatus )
        {
            channelToggleUI->SetSpriteId   ( "CheckONLightUP" );
            channelToggleUI->SetDownUI     ( "CheckONLightDown" );
            channelToggleUI->SetHighlightUI( "CheckONLightHUP" );
        }else
        {
            channelToggleUI->SetSpriteId   ( "CheckOFFLightUP" );
            channelToggleUI->SetDownUI     ( "CheckOFFLightDown" );
            channelToggleUI->SetHighlightUI( "CheckOFFLightHUP" );
        }
        //channelToggleUI->SetCenter( false ); 
        channelToggleUI->SetPos( FramePos( 1, 0, 15, 16 ) );
        channelToggleUI->channelID = (*i).channelID;
        channelToggleUI->listenState = bStatus;
        channelList.AddColumnItem( 1, channelToggleUI );
    }

    // Scroll through the list of regChannels to save.    
    for( j = regChannels.begin(); j != regChannels.end(); j++ )
    {
        bool found = false;
        // Make sure the channel to save is in the provided list of channels.
        for( i = publicChannels.begin(); i != publicChannels.end() && !found; i++ )
        {
            // If the channel was found.
            if( stricmp( (*i).channelID.c_str(), (*j).channelID.c_str() ) == 0 )
            {
                found = true;
            }            
        }
        // If the channel was not found.
        if( !found ){
            // Remove it from the channels to save.
            j = regChannels.erase( j );

            if( j == regChannels.end() ){
                break; 
            }
        }
    }

    channelList.SetScrollPos( dwScrollPos );
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::UpdateChannelUsers
//////////////////////////////////////////////////////////////////////////////////////////
// Updates the list of users in the currently selected channel
// 
(
 const std::vector< User > &channelUsers // The list of users.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
    
    DWORD oldPos = userList.GetScrollPos();
    userList.DeleteList();
    
    vector< User >::const_iterator i;
    for( i = channelUsers.begin(); i != channelUsers.end(); i++ ){
        userList.AddNewItem();

        UserUI *userUI = new UserUI;
        if( (*i).listenState ){
            userUI->SetText( (*i).userName.c_str(), RGB( 255, 255, 255 ), 640, 0, 0 );
        }else{
            userUI->SetText( (*i).userName.c_str(), RGB( 180, 180, 180 ), 640, 0, 0 );
        }
        userUI->SetCenter( false );
        userUI->SetPos( FramePos( 0, 0, userUI->GetWidth(), userUI->GetHeight() ) );
        userUI->userID = (*i).userName;
        userList.AddColumnItem( 0, userUI );

        if( (*i).userName == selectedUser ){
            userList.SetSelection( userUI );
            
        }

        userUI = new UserUI;
        if( (*i).listenState ){
            userUI->SetText( (*i).title.c_str(), RGB( 255, 255, 255 ), 640, 0, 0 );
        }else{
            userUI->SetText( (*i).title.c_str(), RGB( 180, 180, 180 ), 640, 0, 0 );
        }
        userUI->SetCenter( false );
        userUI->SetPos( FramePos( 0, 0, userUI->GetWidth(), userUI->GetHeight() ) );
        userUI->userID = (*i).title;
        userList.AddColumnItem( 1, userUI );
    }

    if( channelUsers.size() == 1 ){
        userQtyTxt.SetText( g_LocalString[ 528 ] );
    }else{
        char buf[ 1024 ];
        sprintf( buf, g_LocalString[ 527 ], channelUsers.size() );
        userQtyTxt.SetText( buf );
    }
    

    userList.SetScrollPos( oldPos );

    // This save the setting of the chatter channel for precaution
    Save();
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::ChannelListEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Called when the user selects a channel in the list.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );

    int col = chatterUI->channelList.GetSelectedColumn();

    string channelId;
    
    // Click in channel names listing
    if( col == 0 ){
        ChannelUI *channelUI = static_cast< ChannelUI * >( chatterUI->channelList.GetSelection() );

        if( channelUI == NULL ){
            return;
        }        

        chatterUI->selectedChannel = channelUI->channelID;

        TFCPacket sending;           
        sending << (short)RQ_GetChatterUserList;//RQ_GetChannelUsers;
        sending << (short)channelUI->channelID.size();
        sending << (char *)channelUI->channelID.c_str();

        SEND_PACKET( sending );
    }
    // Clicked on the channel toggle button
    else
    {
        ChannelUI *channelUI = static_cast< ChannelUI * >( chatterUI->channelList.GetSelection() );

        // If nothing was selected.
        if( channelUI == NULL ){
            return;
        }

        // If we double clicked on the toggle listening icon.
        if( chatterUI->channelList.GetSelectedColumn() == 1 )
        {
           //const RQ_ToggleChatterListening = 86;
           TFCPacket sending;
           sending << (short)RQ_ToggleChatterListening;
           sending << (short)channelUI->channelID.size();
           sending << (char *)channelUI->channelID.c_str();
           sending << (char)( channelUI->listenState == true ? 0 : 1 );
           SEND_PACKET( sending );

           channelUI->listenState = !channelUI->listenState;

           chatterUI->savedStatus[ channelUI->channelID ] = channelUI->listenState;
           
           // Scroll through the list of regChannels to save.
           set< RegChannel >::iterator j; // TODO DaP
           for( j = chatterUI->regChannels.begin(); j != chatterUI->regChannels.end(); j++ )
           {
              /*if( stricmp( (*j).channelID.c_str(), channelUI->channelID.c_str() ) == 0 )
              {
                 (*j).bActive = channelUI->listenState;
              }*/
           }
        }
    }   
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::ChannelListEvent::LeftDblClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Called when the user selects a channel in the list.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );

    int col = chatterUI->channelList.GetSelectedColumn();

    string channelId;

    
    LOG << "\r\nDBL-CLICKED channel listing.";
    
    // Cycle the channel's colors
    if( col == 0 ){
        LOG << "\r\nCol = 0.";

        ChannelUI *channelUI = static_cast< ChannelUI * >( chatterUI->channelList.GetSelection() );

        if( channelUI == NULL ){
            return;
        }        

        LOG << "\r\nSwitching color.";

        /*
        switch( channelUI->channelColor )
        {
           case ChannelUI::cyan:        channelUI->channelColor = ChannelUI::red; break;
           case ChannelUI::red:         channelUI->channelColor = ChannelUI::red2; break;
           case ChannelUI::red2:        channelUI->channelColor = ChannelUI::blue; break;
           case ChannelUI::blue:        channelUI->channelColor = ChannelUI::blue2; break;
           case ChannelUI::blue2:       channelUI->channelColor = ChannelUI::yellow; break;
           case ChannelUI::yellow:      channelUI->channelColor = ChannelUI::green; break;
           case ChannelUI::green:       channelUI->channelColor = ChannelUI::green2; break;
           case ChannelUI::green2:      channelUI->channelColor = ChannelUI::pink; break;
           case ChannelUI::pink:        channelUI->channelColor = ChannelUI::purple; break;
           case ChannelUI::purple:      channelUI->channelColor = ChannelUI::purple2; break;
           case ChannelUI::purple2:     channelUI->channelColor = ChannelUI::grey; break;
           case ChannelUI::grey:        channelUI->channelColor = ChannelUI::orange; break;
           case ChannelUI::orange:      channelUI->channelColor = ChannelUI::cyan; break;
           default: channelUI->channelColor = ChannelUI::cyan; break;
        }
        */
        if(Custom.g_pNewCCColor)
        {
           int dwIndex = -1;
           for(int i=0;i<Custom.g_dwNbrCCColor;i++)
           {
              if(Custom.g_pNewCCColor[i] == channelUI->channelColor)
              {
                 dwIndex = i;
                 i = Custom.g_dwNbrCCColor;
              }
           }
           if(dwIndex >=0)
           {
               dwIndex++;
               if(dwIndex >= Custom.g_dwNbrCCColor)
                  dwIndex = 0;
               channelUI->channelColor = (ChannelUI::color)Custom.g_pNewCCColor[dwIndex];
           }
           else
              channelUI->channelColor = ChannelUI::cyan;
        }
        else
           channelUI->channelColor = ChannelUI::cyan;

        LOG << "\r\nSetting new color..!";
        
        channelUI->SetText( 
            channelUI->channelID.c_str(), 
            channelUI->channelColor, 
            640, 
            0, 
            0 
        );

        chatterUI->savedColor[ channelUI->channelID ] = channelUI->channelColor;

        // Scroll through the list of regChannels to save.
        set< RegChannel >::iterator j; // TODO DaP
        for( j = chatterUI->regChannels.begin(); j != chatterUI->regChannels.end(); j++ )
        {
           /* if( stricmp( (*j).channelID.c_str(), channelUI->channelID.c_str() ) == 0 )
            {
                (*j).color = channelUI->channelColor;
            }*/
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::UserListEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Called when the user selects a player in the list.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );
 
    UserUI *userUI = static_cast< UserUI * >( chatterUI->userList.GetSelection() );    
    
    if( userUI != NULL ){
        // If the chat ui is in page mode.
        if( chatterUI->textInputState == SendToPage ){
            // Set the page name to the clicked user name.
            chatterUI->pageNameEdit.SetText( userUI->userID );            
        }

        chatterUI->selectedUser = userUI->userID;
    }else{
        chatterUI->selectedUser = "";
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::UserListEvent::LeftDblClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Set page mode with the selected user.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );
    UserUI *userUI = static_cast< UserUI * >( chatterUI->userList.GetSelection() );    
    
    if( userUI != NULL ){
        // Set the page name to the clicked user name.
        chatterUI->pageNameEdit.SetText( userUI->userID );            

        // Change input to page.
        chatterUI->textInputState = SendToPage;

        // Hide the channel toggle button.
        chatterUI->channelToggleButton.Hide();
        chatterUI->ingameToggleButton.Hide();

        // Show the page and graph patch.
        chatterUI->pageToggleButton.Show();
        chatterUI->pageNameEdit.Show();
        chatterUI->pageNameGraphPatch.Show();

        chatterUI->textInput.SetParent( chatterUI );

        chatterUI->lastClickedItem = &chatterUI->textInput;

        // Resize the text input to let the pageNameEdit be.
        chatterUI->textInput.SetPos(FramePos(155,g_Global.GetScreenH()-28,chatterUI->textInput.GetPos().maxx,chatterUI->textInput.GetPos().maxy));
        chatterUI->selectedUser = userUI->userID;
    }else{
        chatterUI->selectedUser = "";
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::JoinButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Called when the user clicks on the Join button.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );
    
    chatterUI->joinPopup.ResetUI();

    // Show the join popup box.
    chatterUI->joinPopup.Show();

    // Put the join popup in foreground.
    chatterUI->RequestForegroundControl( &chatterUI->joinPopup );
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::LeaveButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Clicked by the user who wants to leave a channel.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    //const RQ_RemoveFromChatterChannel = 74;
    
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );

    string selectedChannel = chatterUI->GetSelectedChannel();

    // If there is a selected channel.
    if( !selectedChannel.empty() ){
        
        TFCPacket sending;
        sending << (RQ_SIZE)RQ_RemoveFromChatterChannel;
        sending << (short)selectedChannel.size();
        sending << (char *)selectedChannel.c_str();

        SEND_PACKET( sending );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::AddBackscrollItem
//////////////////////////////////////////////////////////////////////////////////////////
//  Adds a string to the backlog.
// 
(
 string fromUser, // The user from which the message came.
 string newText,   // The string.
 DWORD  baseColor,
 bool   screenEcho,
 bool   allowNewLine
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );

    // If the fromUser cannot be found in the ignore list.
    if( ignoreList.find( fromUser ) == ignoreList.end() )
    {

        LOG.Lock();
        LOG << "\r\nAdding text " << (char *)newText.c_str() << " !!";
        LOG.Unlock();
            
        if( screenEcho )
        {
            char *txt = new char[ newText.size() + 1 ];
            strcpy( txt, newText.c_str() );
            ShowSystemMessage( txt, baseColor, 7500, allowNewLine );
            delete [] txt;
        }
        
        textBackscroll.AddText( newText.c_str(), baseColor, allowNewLine );

        OptionParam *pOption = g_SaveGame.GetOptionsParam();
        if( pOption->bLogEnable)
        {
            ClientConfig *pConfig = g_Global.GetClientConfig(); 
            
            char strLogFile[1024];

            sprintf(strLogFile,"%s\\Logs",pConfig->strSavePath);
            CreateDirectory( strLogFile, NULL );

            strcat(strLogFile,"\\");
            strcat(strLogFile,pOption->LogName);
            
            FILE *f = fopen( strLogFile, "ab" );
            if( f != NULL ){
                SYSTEMTIME curTime;
                GetLocalTime( &curTime );

                char buf[ 1024 ];
                sprintf( 
                    buf, 
                    "%02u/%02u/%u %02u:%02u:%02u-- ", 
                    curTime.wMonth,
                    curTime.wDay,
                    curTime.wYear,
                    curTime.wHour,
                    curTime.wMinute,
                    curTime.wSecond
                );
                
                fputs( buf, f );
                fputs( newText.c_str(), f );
                fputs( "\r\n", f );
                fclose( f );
            }
        }
    }
}

void ChatterUI::ClearBackScroll()
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
   textBackscroll.ClearList();
}


//////////////////////////////////////////////////////////////////////////////////////////
UINT WINAPI ChatterUI::ChannelRefreshThread
//////////////////////////////////////////////////////////////////////////////////////////
// Refreshes the channel each 10 seconds.
// 
(
 void *nothing
)
// Return: UINT, 0
//////////////////////////////////////////////////////////////////////////////////////////
{
    while( g_boQuitApp ){
        Sleep( 10000 );

        // If the box is currently displayed.
        if( GetInstance()->IsShown() ){
            // If a channel is currently selected.
            if( !GetInstance()->selectedChannel.empty() ){
                // Send a user channel refresh.
                //const RQ_GetChannelUsers = 50;

                TFCPacket sending;           
                sending << (short)RQ_GetChatterUserList;//RQ_GetChannelUsers;
                sending << (short)GetInstance()->selectedChannel.size();
                sending << (char *)GetInstance()->selectedChannel.c_str();

                SEND_PACKET( sending );
            }
        }
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::PageToggleButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Page button was pressed. Move to ingame
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );
    
    chatterUI->textInputState = SendToGame;

    // Hide the page and graph patch.
    chatterUI->pageToggleButton.Hide();
    chatterUI->pageNameEdit.Hide();
    chatterUI->pageNameGraphPatch.Hide();

    chatterUI->textInput.SetParent( NULL );

    // Resize the text input to remove the pageNameEdit.    
    chatterUI->textInput.SetPos(FramePos(40,g_Global.GetScreenH()-28,chatterUI->textInput.GetPos().maxx,chatterUI->textInput.GetPos().maxy));

    // Show the speak toggle button.
    chatterUI->ingameToggleButton.Show();

}
//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::ChannelToggleButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Channel button pressed. Move to pages.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );
    
    chatterUI->textInputState = SendToPage;

    // Hide the channel toggle button.
    chatterUI->channelToggleButton.Hide();

    // Show the page and graph patch.
    chatterUI->pageToggleButton.Show();
    chatterUI->pageNameEdit.Show();
    chatterUI->pageNameGraphPatch.Show();

    chatterUI->textInput.SetParent( chatterUI );

   
    

    // Resize the text input to let the pageNameEdit be.
    chatterUI->textInput.SetPos(FramePos(155,g_Global.GetScreenH()-28,chatterUI->textInput.GetPos().maxx,chatterUI->textInput.GetPos().maxy));
}
//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::IngameToggleButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Speak button pressed. Move to channels
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );
    
    chatterUI->textInputState = SendToChannel;

    // Hide the speak toggle button.
    chatterUI->ingameToggleButton.Hide();

    // Show the channel
    chatterUI->channelToggleButton.Show();
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::IgnoreButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// The ignore button was selected.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );

    // Set the page name to the clicked user name.
    if( !chatterUI->selectedUser.empty() && stricmp( chatterUI->selectedUser.c_str(), Player.Name ) != 0 ){
        chatterUI->ignoreList.insert( chatterUI->selectedUser );        
    }else{
        naSound->Play( TRUE );
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::RememberButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// The remember button was pressed.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );

    // Set the page name to the clicked user name.
    if( !chatterUI->selectedUser.empty() ){
        chatterUI->ignoreList.erase( chatterUI->selectedUser );
    }else{
        naSound->Play( TRUE );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::PageButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Page toggle got hit!
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );

    // Switch paging mode.
    OptionParam *pOption = g_SaveGame.GetOptionsParam();
    pOption->bPageEnable = !pOption->bPageEnable;

    //const RQ_TogglePage = 89;

    // If pages are now allowed.
    if(pOption->bPageEnable )
    {
        chatterUI->pageoffButton.Hide();

        // Tell server to turn pages on.
        TFCPacket sending;
        sending << (RQ_SIZE)RQ_TogglePage;
        sending << (char)1;

        SEND_PACKET( sending );
    }
    else
    {
        chatterUI->pageoffButton.Show();

        // Tell server to turn pages off.
        TFCPacket sending;
        sending << (RQ_SIZE)RQ_TogglePage;
        sending << (char)0;

        SEND_PACKET( sending );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::IgnoreListButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked the ignore button.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );
 
    // Show the join popup box.
    chatterUI->ignorePopup.Show();

    chatterUI->ignorePopup.ResetUI();

    // Put the join popup in foreground.
    chatterUI->RequestForegroundControl( &chatterUI->ignorePopup );
}

void ChatterUI::ClientInitialize( void )
{
   //Load les ignore List
   int dwNbrIgnore = g_SaveGame.GetNbrIgnore();
   if(dwNbrIgnore > 0)
   {
      for(int i =0;i<dwNbrIgnore; i++ )
      {
         sIgnoreList sNewIgnore;
         g_SaveGame.GetIgnore(i,&sNewIgnore);
         ignoreList.insert( sNewIgnore.strName );
      }
   }
   
   
   //Load la Liste des CC
   int dwNbrCC = g_SaveGame.GetNbrChannel();
   if(dwNbrCC > 0)
   {
      for(int i =0;i<dwNbrCC; i++ )
      {
         sChannel sNewCC;
         g_SaveGame.GetChannel(i,&sNewCC);
         bool bEnable = true;
         if(sNewCC.chEnable==0)
         {
            bEnable = false;
            // on dois envoyer au serveur que le CC ets desactiver...
         }
         EnterChatterChannel( sNewCC.strName, sNewCC.strPassword, sNewCC.dwColor,bEnable);
      }
   }

   OptionParam *pOption = g_SaveGame.GetOptionsParam();
   
   //const RQ_TogglePage = 89;
   if( pOption->bPageEnable == 0 )
   {
      pageoffButton.Show();
      
      // Tell server to turn pages off.
      TFCPacket sending;
      sending << (RQ_SIZE)RQ_TogglePage;
      sending << (char)0;
      
      SEND_PACKET( sending );
   }
   else
   {
      pageoffButton.Hide();
      // Tell server to turn pages on.
      TFCPacket sending;
      sending << (RQ_SIZE)RQ_TogglePage;
      sending << (char)1;
      
      SEND_PACKET( sending );
   }
}



//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::MinHButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Channel button pressed. Move to pages.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
   
    ChatterUI *chatterUI = static_cast< ChatterUI * >( GetUI() );
    if(chatterUI ->m_bOldMinH)
      return;
    
    if(chatterUI->IsMinimizedHight())
       chatterUI->SetMinimizedHight(false);
    else
       chatterUI->SetMinimizedHight(true);
}
 

//////////////////////////////////////////////////////////////////////////////////////////
//  Loads all information from the registry.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::Load( void )
{
    /*
    OptionParam *pOption = g_SaveGame.GetOptionsParam();

    const RQ_TogglePage = 89;
    if( pOption->bPageEnable == 0 )
    {
        pageoffButton.Show();

        // Tell server to turn pages off.
        TFCPacket sending;
        sending << (RQ_SIZE)RQ_TogglePage;
        sending << (char)0;

        SEND_PACKET( sending );
    }
    else
    {
        pageoffButton.Hide();
        // Tell server to turn pages on.
        TFCPacket sending;
        sending << (RQ_SIZE)RQ_TogglePage;
        sending << (char)1;

        SEND_PACKET( sending );
    }
    */
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::Save( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Saves all information into the registry.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    //Save IgnoreList
    g_SaveGame.bFreeIgnore();
    IgnoreSet::iterator i;
    for( i = ignoreList.begin(); i != ignoreList.end(); i++ )
    {
       sIgnoreList sNewIgnore;
       sprintf(sNewIgnore.strName,"%s",(*i).c_str());
       g_SaveGame.AddIgnore(sNewIgnore);
    }
    
    //Save CCUser
    g_SaveGame.bFreeChannel();
    set< RegChannel >::iterator c;
    for( c = regChannels.begin(); c != regChannels.end(); c++ )
    {
       sChannel sNewCC;
       sprintf(sNewCC.strName,"%s",(*c).channelID.c_str());
       sprintf(sNewCC.strPassword,"%s",(*c).pwd.c_str());
       sNewCC.dwColor = (*c).color;
       if((*c).bActive)
          sNewCC.chEnable = 1;
       else
          sNewCC.chEnable = 0;

       g_SaveGame.AddChannel(sNewCC);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ChatterUI::IsUserIgnored
//////////////////////////////////////////////////////////////////////////////////////////
//  Determines if a user has been ignored.
// 
(
 string userName // The user
)
// Return: bool, true if the user was ignored.
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );

    return( ignoreList.find( userName ) != ignoreList.end() );
}


//////////////////////////////////////////////////////////////////////////////////////////
// MINIMIZED PORTION

//////////////////////////////////////////////////////////////////////////////////////////
//  Intercept minimize state changes to resize the backlog area.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::Minimize(bool newMinimizedState)
{
    GameUI::Minimize( newMinimizedState );
     
    int dwXPos = (g_Global.GetScreenW()-576)/2;
    int dwYPos = (g_Global.GetScreenH()-150-368)/2;
    if(dwYPos <0)
       dwYPos =0;
    
	// by Asteryth : is it really used?
    //int dwOffset = 729+8-(g_Global.GetScreenW()-287);
    //dwOffset*=-1;
    if( newMinimizedState == true )
    {
       if(m_bMinH && g_SaveGame.GetOptionsParam()->bHyperChat)
          SetPos( FramePos( 0, g_Global.GetScreenH()-592, g_Global.GetScreenW(), g_Global.GetScreenH() ) );
       else if(m_bMinH && !g_SaveGame.GetOptionsParam()->bHyperChat)
          SetPos( FramePos( 0, g_Global.GetScreenH()-300, g_Global.GetScreenW(), g_Global.GetScreenH() ) );
       else
          SetPos( FramePos( 0, g_Global.GetScreenH()-150, g_Global.GetScreenW(), g_Global.GetScreenH() ) );
       
       // Move the Chatter to the end...
       textBackscroll.SetScrollPos( 0xFFFFFFFF);
       
       //:CR_NMNMNM_SAVE 2006-10-10 //Add save on minimuze satte of GUI...
       //g_SaveGame.bSave();
    }
    else
    {
       SetPos( FramePos( 0, dwYPos, g_Global.GetScreenW(), g_Global.GetScreenH() ) );
    }
}

void ChatterUI::SetMinimizedHight(bool bMax)
{
    // Text input always has a cursor.
   int dwOffset = 729+8-(g_Global.GetScreenW()-287);
   dwOffset*=-1;

   m_bMinH = bMax;
   if(m_bMinH && g_SaveGame.GetOptionsParam()->bHyperChat)
   {
      if(IsMinimized())
         SetPos( FramePos( 0, g_Global.GetScreenH()-592, g_Global.GetScreenW(), g_Global.GetScreenH() ) );
      backscrollScroll.SetPos( FramePos( 0, 0, 24, 22) );
      backscrollScrollUp  .SetPos( FramePos( dwOffset+8+636, g_Global.GetScreenH()-583, dwOffset+8+660, g_Global.GetScreenH()-567 ) );
      backscrollScrollDown.SetPos( FramePos( dwOffset+8+636, g_Global.GetScreenH()-58 , dwOffset+8+660, g_Global.GetScreenH()-44 ) );
      
      textBackscroll.SetPos     ( FramePos( 0 , g_Global.GetScreenH()-592, dwOffset+8+660, g_Global.GetScreenH()-40 ) );
      textBackscroll.SetTextArea( FramePos( 12, g_Global.GetScreenH()-574, dwOffset+8+610, g_Global.GetScreenH()-42 ) );
      textBackscroll.SetScrollRegions(
         FramePos( dwOffset+8+636, g_Global.GetScreenH()-583, dwOffset+8+660, g_Global.GetScreenH()-561),
         FramePos( dwOffset+8+636, g_Global.GetScreenH()-67 , dwOffset+8+660, g_Global.GetScreenH()-45),
         FramePos( dwOffset+8+636, g_Global.GetScreenH()-565, dwOffset+8+649, g_Global.GetScreenH()-68)
         );
   }
   else if(m_bMinH && !g_SaveGame.GetOptionsParam()->bHyperChat)
   {
      if(IsMinimized())
         SetPos( FramePos( 0, g_Global.GetScreenH()-300, g_Global.GetScreenW(), g_Global.GetScreenH() ) );
      backscrollScroll.SetPos( FramePos( 0, 0, 24, 22) );
      backscrollScrollUp  .SetPos( FramePos( dwOffset+8+636, g_Global.GetScreenH()-291, dwOffset+8+660, g_Global.GetScreenH()-275 ) );
      backscrollScrollDown.SetPos( FramePos( dwOffset+8+636, g_Global.GetScreenH()-58 , dwOffset+8+660, g_Global.GetScreenH()-44 ) );
      
      textBackscroll.SetPos     ( FramePos( 0 , g_Global.GetScreenH()-300, dwOffset+8+660, g_Global.GetScreenH()-40 ) );
      textBackscroll.SetTextArea( FramePos( 12, g_Global.GetScreenH()-282, dwOffset+8+610, g_Global.GetScreenH()-42 ) );
      textBackscroll.SetScrollRegions(
         FramePos( dwOffset+8+636, g_Global.GetScreenH()-291, dwOffset+8+660, g_Global.GetScreenH()-269),
         FramePos( dwOffset+8+636, g_Global.GetScreenH()-67 , dwOffset+8+660, g_Global.GetScreenH()-45),
         FramePos( dwOffset+8+636, g_Global.GetScreenH()-273, dwOffset+8+649, g_Global.GetScreenH()-68)
         );
   }
   else
   {
      if(IsMinimized())
         SetPos( FramePos( 0, g_Global.GetScreenH()-150, g_Global.GetScreenW(), g_Global.GetScreenH() ) );
      backscrollScroll.SetPos( FramePos( 0, 0, 24, 22) );
      backscrollScrollUp  .SetPos( FramePos( dwOffset+8+636, g_Global.GetScreenH()-141, dwOffset+8+660, g_Global.GetScreenH()-125 ) );
      backscrollScrollDown.SetPos( FramePos( dwOffset+8+636, g_Global.GetScreenH()-58 , dwOffset+8+660, g_Global.GetScreenH()-44 ) );
      
      textBackscroll.SetPos     ( FramePos( 0 , g_Global.GetScreenH()-150, dwOffset+8+660, g_Global.GetScreenH()-40 ) );
      textBackscroll.SetTextArea( FramePos( 12, g_Global.GetScreenH()-131, dwOffset+8+610, g_Global.GetScreenH()-40 ) );
      textBackscroll.SetScrollRegions(
         FramePos( dwOffset+8+636, g_Global.GetScreenH()-141, dwOffset+8+660, g_Global.GetScreenH()-119),
         FramePos( dwOffset+8+636, g_Global.GetScreenH()-67 , dwOffset+8+660, g_Global.GetScreenH()-45),
         FramePos( dwOffset+8+636, g_Global.GetScreenH()-123, dwOffset+8+649, g_Global.GetScreenH()-68)
         );
   }
   textBackscroll.UpdateViewSize();
   textBackscroll.UpdateBacklogView(); 

   //textBackscroll.SetScrollPos( 0xFFFFFFFF);
}

void ChatterUI::Draw( V2SPRITEFX *vsfFX )
{
   // check si un des GUI est maximizer...
   if(IsMinimizedHight() || m_bOldMinH)
   {
      if( CharacterUI ::GetInstance()->IsShown()     ||
          InventoryUI ::GetInstance()->IsShown()     ||
          SpellUI     ::GetInstance()->IsShown()     ||
          (GroupPlayUI ::GetInstance()->IsShown()     && !GroupPlayUI::GetInstance()->IsMinimized() )||
          
          !ChatterUI   ::GetInstance()->IsMinimized()||
          (MacroUI     ::GetInstance()->IsShown()     && !MacroUI::GetInstance()->IsMinimized())||
          OptionsUI   ::GetInstance()->IsShown()     ||
          ChestUI     ::GetInstance()->IsShown()     ||
          RobUI       ::GetInstance()->IsShown()     ||
          RTMap       ::GetInstance()->IsShown()     ||
          RTHelp      ::GetInstance()->IsShown()     ||
          SpellUI     ::GetInstance()->IsShown()     ||
          SkillTeachUI::GetInstance()->IsShown()     ||
          SkillTrainUI::GetInstance()->IsShown()     ||
          TradeUI     ::GetInstance()->IsShown()      )
      {
         if(!m_bOldMinH)
         {
            m_bOldMinH = true;
            SetMinimizedHight(false);
         }
      }
      else if(m_bOldMinH)
      {
         m_bOldMinH = false;
         SetMinimizedHight(true);
      }
   }

    
    

   DrawMinimized(vsfFX);
   if(IsMinimized())
      return;

   // sinon on draw aussi le panneau CC
   int dwXPos = (g_Global.GetScreenW()-576)/2;
   int dwYPos = (g_Global.GetScreenH()-150-368)/2;
   if(dwYPos <0)
       dwYPos =0;
    
   GameUI::Draw( vsfFX );

   HDC hdc;
   DXDGetDC(&hdc, 690);

   char strBtnTexte[20];
   // Draw Box Header
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[1]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 237+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 237  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[2]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,117)+ 40, dwYPos+40 , g_Global.GetBtnTCE()  , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[3]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,108)+ 242, dwYPos+41 , g_Global.GetBtnTCE()  , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[4]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,71)+ 412, dwYPos+41 , g_Global.GetBtnTCE()  , 0, strBtnTexte);

   DXDReleaseDC(hdc, 690);
}

int ChatterUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the minized chat box.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::DrawMinimized(V2SPRITEFX *vsfFX)
{
    // Draw the minimized chatter graphUI.
    if(IsMinimizedHight() && g_SaveGame.GetOptionsParam()->bHyperChat)
    {
       minimizedGraphHR  .Draw( vsfFX);
       minimizedGraphHHHB.Draw( vsfFX);
       minimizedGraphHHHT.Draw( vsfFX);
       sideFillHHH       .Draw( vsfFX);
    }
    else if(IsMinimizedHight() && !g_SaveGame.GetOptionsParam()->bHyperChat)
    {
       minimizedGraphHR.Draw( vsfFX);
       minimizedGraphHL.Draw( vsfFX);
       sideFillH       .Draw( vsfFX);
    }
    else
    {
       minimizedGraph.Draw( vsfFX);
       sideFill      .Draw( vsfFX);
    }

    charSheetButton.Draw( vsfFX );
    invButton      .Draw( vsfFX );
    spellsButton   .Draw( vsfFX );
    macroButton    .Draw( vsfFX );
    groupPlayButton.Draw( vsfFX );
    optionsButton  .Draw( vsfFX );
    chatterButton  .Draw( vsfFX );
    MinHButton     .Draw( vsfFX );

    //Dessine les macro si macro sont presente...
    for(int k=0;k<6;k++)
    {
       if(MacroUI::GetInstance()->macroGrid[k].macroItem != NULL)
       {
          MacroDataUI *macroUI = MacroUI::GetInstance()->macroGrid[k].macroItem->macroUI;
          if(macroUI != NULL)
          {
             int xOffset = (k*91)-1;
             int yOffset = 2;

             FramePos itemPos = macroUI->GetPos();
             FramePos thisPos = FramePos(g_Global.GetScreenW()-280+xOffset,g_Global.GetScreenH()-105+yOffset,g_Global.GetScreenW()-280+44,g_Global.GetScreenH()-105+43+yOffset);
             itemPos.SetNewOffset(MousePos( thisPos.x - itemPos.Width() / 2 + thisPos.Width() / 2,thisPos.y - itemPos.Height() / 2+ thisPos.Height() / 2));
       
             macroUI->SetPos( itemPos );
             macroUI->Draw(vsfFX);
          }
       }
    }
    
    // Draw the minimized active controls.    
    if( pageToggleButton.IsShown() )
       pageToggleButton.Draw( vsfFX ); 
    
    if( channelToggleButton.IsShown() )
       channelToggleButton.Draw( vsfFX );
    
    if( ingameToggleButton.IsShown() )
       ingameToggleButton.Draw( vsfFX );
    
    if( pageNameGraphPatch.IsShown() )
       pageNameGraphPatch.Draw( vsfFX );
    
    if( pageNameEdit.IsShown() )
       pageNameEdit.Draw( vsfFX );

    // Always shown.
    textBackscroll.Draw( vsfFX );
    textInput.Draw( vsfFX );
}

void ChatterUI::UpdateMacroHelpID()
{
   for(int k=0;k<6;k++)
   {
      if(MacroUI::GetInstance()->macroGrid[k].macroItem != NULL)
      {
         MacroDataUI *macroUI = MacroUI::GetInstance()->macroGrid[k].macroItem->macroUI;
         if(macroUI != NULL)
         {
            switch(k)
            {
               case 0: macroBtn0.SetHelpText(FontManager::GetInstance()->GetSmallFont(FALSE, MacroUI::GetInstance()->macroGrid[k].macroItem->macroName.c_str() ));break;
               case 1: macroBtn1.SetHelpText(FontManager::GetInstance()->GetSmallFont(FALSE, MacroUI::GetInstance()->macroGrid[k].macroItem->macroName.c_str() ));break;
               case 2: macroBtn2.SetHelpText(FontManager::GetInstance()->GetSmallFont(FALSE, MacroUI::GetInstance()->macroGrid[k].macroItem->macroName.c_str() ));break;
               case 3: macroBtn3.SetHelpText(FontManager::GetInstance()->GetSmallFont(FALSE, MacroUI::GetInstance()->macroGrid[k].macroItem->macroName.c_str() ));break;
               case 4: macroBtn4.SetHelpText(FontManager::GetInstance()->GetSmallFont(FALSE, MacroUI::GetInstance()->macroGrid[k].macroItem->macroName.c_str() ));break;
               case 5: macroBtn5.SetHelpText(FontManager::GetInstance()->GetSmallFont(FALSE, MacroUI::GetInstance()->macroGrid[k].macroItem->macroName.c_str() ));break;
            }
         }
      }
   }
}
//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::SetInputText
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the text input text
// 
( 
 const char *txt
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    textInput.SetText( txt );
}

//////////////////////////////////////////////////////////////////////////////////////////
string ChatterUI::GetInputText( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the text currently typed.
//////////////////////////////////////////////////////////////////////////////////////////
{
    return textInput.GetText();
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::SendMessageToCurrentChannel
//////////////////////////////////////////////////////////////////////////////////////////
//  Sends a message to the currently selected channel
// 
(
 const string &msg // The message
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( !selectedChannel.empty() ){
        TFCPacket sending;
        // Create a send chatter message packet.
        sending << (RQ_SIZE)RQ_SendChatterMessage;
		sending << (short)selectedChannel.size();
		sending << (char *)selectedChannel.c_str();
        sending << (short)msg.size();
		sending << (char *)msg.c_str();

        SEND_PACKET( sending );
    }else{
        AddBackscrollItem(
            "",
            g_LocalString[ 487 ],
            SYSTEM_COLOR,
            false
        );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
DWORD ChatterUI::GetChannelColor
//////////////////////////////////////////////////////////////////////////////////////////
//  Gets the color of the given channel
// 
(
 std::string channelId // The Id of the channel
)
// Return: DWORD, The color
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );   
    
    vector< vector< ListUI::GridItem > > &grid = *channelList.GetGrid();

    UINT i;
    for( i = 0; i < grid[ 0 ].size(); i++ ){
        ChannelUI *channel = static_cast< ChannelUI * >( grid[ 0 ][ i ] );
        if( channel != NULL && stricmp( channel->channelID.c_str(), channelId.c_str() ) == 0 ){
            return channel->channelColor;
        }
    }
    return RGB( 255, 255, 255 );
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChatterUI::MenuItemEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked one of the menu items
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
   LOG << "\r\nA MENU ITEM WAS CLICKED";
    
   g_Global.ResetItemInfo();
   switch( mi )
   {
      case CharSheet: 
        if( !CharacterUI::GetInstance()->IsShown() )
        {
            CharacterUI::Display( NULL ); 
        }
      break;
      case Inv: 
        if( !InventoryUI::GetInstance()->IsShown() )
        {
            InventoryUI::Display( NULL ); 
        }
      break;
      case Spells: 
        if( !SpellUI::GetInstance()->IsShown() )
        {
            SpellUI::Display( NULL ); 
        }
      break;
      case GroupPlay: 
        if( !GroupPlayUI::GetInstance()->IsShown() || GroupPlayUI::GetInstance()->IsMinimized()  )
        {
            GroupPlayUI::Display( NULL );
        }
      break;
      case Chatter: 
        if( !ChatterUI::GetInstance()->IsShown() || ChatterUI::GetInstance()->IsMinimized() )
        {
            ChatterUI::Display( NULL );
        }
      break;
      case Macro: 
        if( !MacroUI::GetInstance()->IsShown() || MacroUI::GetInstance()->IsMinimized() )
        {
            MacroUI::Display( NULL );
        }
      break;
      case Options: 
        if( !OptionsUI::GetInstance()->IsShown() )
        {
            OptionsUI::Display( NULL );
        }
      break;
   };
}

void ChatterUI::Macro0Event::LeftDblClicked( void )
{
    if(MacroUI::GetInstance()->macroGrid[0].macroItem != NULL)
    {
       MacroDataUI *macroUI    = MacroUI::GetInstance()->macroGrid[0].macroItem->macroUI;
       MacroEvent  *macroEvent = MacroUI::GetInstance()->macroGrid[0].macroItem->macroEvent;
       if(macroUI != NULL  && macroEvent != NULL)
       {
          macroEvent->ExecMacro(macroUI);
       }
    }
}

void ChatterUI::Macro1Event::LeftDblClicked( void )
{
    if(MacroUI::GetInstance()->macroGrid[1].macroItem != NULL)
    {
       MacroDataUI *macroUI    = MacroUI::GetInstance()->macroGrid[1].macroItem->macroUI;
       MacroEvent  *macroEvent = MacroUI::GetInstance()->macroGrid[1].macroItem->macroEvent;
       if(macroUI != NULL  && macroEvent != NULL)
       {
          macroEvent->ExecMacro(macroUI);
       }
    }
}

void ChatterUI::Macro2Event::LeftDblClicked( void )
{
    if(MacroUI::GetInstance()->macroGrid[2].macroItem != NULL)
    {
       MacroDataUI *macroUI    = MacroUI::GetInstance()->macroGrid[2].macroItem->macroUI;
       MacroEvent  *macroEvent = MacroUI::GetInstance()->macroGrid[2].macroItem->macroEvent;
       if(macroUI != NULL  && macroEvent != NULL)
       {
          macroEvent->ExecMacro(macroUI);
       }
    }
}

void ChatterUI::Macro3Event::LeftDblClicked( void )
{
    if(MacroUI::GetInstance()->macroGrid[3].macroItem != NULL)
    {
       MacroDataUI *macroUI    = MacroUI::GetInstance()->macroGrid[3].macroItem->macroUI;
       MacroEvent  *macroEvent = MacroUI::GetInstance()->macroGrid[3].macroItem->macroEvent;
       if(macroUI != NULL  && macroEvent != NULL)
       {
          macroEvent->ExecMacro(macroUI);
       }
    }
}

void ChatterUI::Macro4Event::LeftDblClicked( void )
{
    if(MacroUI::GetInstance()->macroGrid[4].macroItem != NULL)
    {
       MacroDataUI *macroUI    = MacroUI::GetInstance()->macroGrid[4].macroItem->macroUI;
       MacroEvent  *macroEvent = MacroUI::GetInstance()->macroGrid[4].macroItem->macroEvent;
       if(macroUI != NULL  && macroEvent != NULL)
       {
          macroEvent->ExecMacro(macroUI);
       }
    }
}

void ChatterUI::Macro5Event::LeftDblClicked( void )
{
    if(MacroUI::GetInstance()->macroGrid[5].macroItem != NULL)
    {
       MacroDataUI *macroUI    = MacroUI::GetInstance()->macroGrid[5].macroItem->macroUI;
       MacroEvent  *macroEvent = MacroUI::GetInstance()->macroGrid[5].macroItem->macroEvent;
       if(macroUI != NULL  && macroEvent != NULL)
       {
          macroEvent->ExecMacro(macroUI);
       }
    }
}


