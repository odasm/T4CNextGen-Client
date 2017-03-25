// GuildUI.cpp
//
#pragma warning( disable : 4291 )

#include "../pch.h"
#include "GuildUI.h"
#include "RootBoxUI.h"
#include "ChatterUI.h"
#include "../Bitmap.h"
#include "../Packet.h"
#include "../VisualObjectList.h"
#include "../MemoryManager.h"
#include "../tileset.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]

using namespace std;

extern Bitmap GridID;
extern Font *fNewGuiFont;
extern Font *fNewGuiBtnFont;
extern Font *fSkillFont;

namespace{
    const DWORD NearMemberColor = RGB( 255, 255, 255 );
    const DWORD NearLeaderColor = RGB( 0,   255, 255 );
    const DWORD FarMemberColor  = RGB( 150, 150, 150 );
    const DWORD FarLeaderColor  = RGB( 0,   150, 150 );
};

//////////////////////////////////////////////////////////////////////////////////////////
inline bool PosInRange
//////////////////////////////////////////////////////////////////////////////////////////
// Determines if two positions are in range of each other.
// 
(
 int rxPos,
 int ryPos,
 int lxPos,
 int lyPos
)
// Return: inline bool, true if positions are in range, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////
{
    return( ::abs( rxPos - lxPos ) <= 20 && ::abs( ryPos - lyPos ) <= 20 );
}


// Constructor ///////////////////////////////////////////////////////////////////////////
GuildUI::GuildUI( void )
:
	dwInvitationGuild( 0 ),
    kickButton( kickButtonEvent ),
    leaveButton( leaveButtonEvent ),
	inviteButton( inviteButtonEvent ),
    memberList( memberListEvent ),
    quitButton( hideEvent )	
//////////////////////////////////////////////////////////////////////////////////////////
{
	currentGuild.dwID = 0;
	currentGuild.strName = "";	
	currentGuild.strRights = "000000";

    int dwXPos = (g_Global.GetScreenW()-232)/2;
    int dwYPos = (g_Global.GetScreenH()-279-150)/2;
    SetPos( FramePos( dwXPos, dwYPos, dwXPos+240, dwYPos+280 ) );

    MemberBack.SetSpriteId( "GUI_BackIgnore" );
    MemberBack.SetPos( FramePos( dwXPos, dwYPos, dwXPos+232, dwYPos+279 ) );

    quitButton.SetPos        ( FramePos( dwXPos+232-28, dwYPos+0, dwXPos+232-4, dwYPos+0+24 ) );
    quitButton.SetSpriteId   ( "GUI_X_ButtonUp" );
    quitButton.SetDownUI     ( "GUI_X_ButtonDown" );
    quitButton.SetHighlightUI( "GUI_X_ButtonHUp" );
    SetHelpId( quitButton, 261 );

	// 28
	inviteButton.SetPos( FramePos( dwXPos+28, dwYPos+220, dwXPos+28+60, dwYPos+242 ) );
	inviteButton.SetSpriteId( "GUI_ButtonTUp" );
	inviteButton.SetDownUI( "GUI_ButtonTDown" );
	inviteButton.SetHighlightUI( "GUI_ButtonTHUp" );
	inviteButtonEvent.SetUI( this );	
	SetHelpId( inviteButton, 586 );

    leaveButton.SetPos( FramePos( dwXPos+22, dwYPos+31, dwXPos+77, dwYPos+57 ) );
    leaveButton.SetSpriteId   ( "GUI_ButtonSUp" );
    leaveButton.SetDownUI     ( "GUI_ButtonSDown" );
    leaveButton.SetHighlightUI( "GUI_ButtonSHUp" );
    leaveButtonEvent.SetUI( this );	
    SetHelpId( leaveButton,  587 );
       
    kickButton.SetPos( FramePos( dwXPos+143, dwYPos+220, dwXPos+203, dwYPos+242 ) );
    kickButton.SetSpriteId   ( "GUI_ButtonTUp" );
    kickButton.SetDownUI     ( "GUI_ButtonTDown" );
    kickButton.SetHighlightUI( "GUI_ButtonTHUp" );
    kickButtonEvent.SetUI( this );     
    SetHelpId( kickButton, 588 );
     
    
/*    addEdit.SetPos( FramePos( dwXPos+82, dwYPos+37, dwXPos+203, dwYPos+51 ) );
    addEdit.SetParent( this );
    addEdit.EnableCursor( true );
    addEdit.SetMaxTextLength( 15 );
    SetHelpId( addEdit, 458 );	*/
    
    guildNameText.SetPos( FramePos( dwXPos+82, dwYPos+37, dwXPos+203, dwYPos+51 ) );
    guildNameText.SetCenter( true );

    scrollUpGraph.SetPos  ( FramePos( dwXPos+180, dwYPos+66 , dwXPos+203, dwYPos+88  ) );
    scrollDownGraph.SetPos( FramePos( dwXPos+180, dwYPos+183, dwXPos+203, dwYPos+207 ) );

    memberList.SetPos( FramePos( dwXPos+28, dwYPos+66, dwXPos+203, dwYPos+207 ) );
    memberList.SetupList( 1, 12, FramePos( 0, 0, 0, 10 ) );
    memberList.SetScrollUpDownUI( &scrollUpGraph );
    memberList.SetScrollDownDownUI( &scrollDownGraph );
    memberList.SetScrollRegions(
        FramePos( dwXPos+180, dwYPos+66 , dwXPos+203, dwYPos+88  ),
        FramePos( dwXPos+180, dwYPos+183, dwXPos+203, dwYPos+207 ),
        FramePos( dwXPos+180, dwYPos+89 , dwXPos+203, dwYPos+185 )
    );	
	memberListEvent.SetUI( this );

    selSprite.SetSpriteId( "GUI_IgnoreSelect" );
    selSprite.SetPos( FramePos( 0, 0, 146, 16 ) );


    memberList.SetSelectionSprite( &selSprite );
    memberList.SetColumnSize( 0, 146 );
    SetHelpId( memberList, 460 );

    AddChild( &MemberBack );
    AddChild( &quitButton );
    AddChild( &kickButton );
    AddChild( &leaveButton );
    AddChild( &inviteButton );
    AddChild( &memberList );
	AddChild( &guildNameText );
    
    RootBoxUI::GetInstance()->AddChild( this );

	ShowButtons();
}

GuildUI* GuildUI::GetInstance()
{
	static GuildUI m_pInstance;
	return &m_pInstance;
}

unsigned int GuildUI::Display( void* null )
{
	CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );

	GuildUI* guildUI = GetInstance();

	if( guildUI->IsShown() && guildUI->IsMinimized() )
		guildUI->Hide();
	else
	{
		if( GetInstance()->IsMinimized() )
        {
            GetInstance()->Minimize( false );
        }

		guildUI->Show();		

		 // Send request to get members listing
        TFCPacket sending;
        sending << (RQ_SIZE)RQ_GuildGetMembers;

        SEND_PACKET( sending );
	}

	return 0;
}

void GuildUI::ShowButtons()
{
	if( currentGuild.strRights[1] == '1' )	
		inviteButton.Show();
	else
		inviteButton.Hide();

	if( currentGuild.strRights[2] == '1' )
		kickButton.Show();
	else
		kickButton.Hide();

	if( currentGuild.dwID <= 0 )
		leaveButton.Hide();
	else
		leaveButton.Show();
}

void GuildUI::UpdateUsersList( std::vector< MemberUI > &vMembers )
{
	CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
    
    DWORD oldPos = memberList.GetScrollPos();
    memberList.DeleteList();
    
    vector< MemberUI >::const_iterator i;
    for( i = vMembers.begin(); i != vMembers.end(); i++ ){
        memberList.AddNewItem();

        MemberUI *userUI = new MemberUI;

		userUI->userID		= (*i).userID;
		userUI->username	= (*i).username;
		userUI->userRights	= (*i).userRights;
		userUI->level		= (*i).level;
		
		// If this user is leader
        if( userUI->userRights[0] == '1' )
		{
            userUI->SetText( (*i).username.c_str(), RGB( 0, 255, 255 ), 640, 0, 0 );
        }
		else
		{
            userUI->SetText( (*i).username.c_str(), RGB( 255, 255, 255 ), 640, 0, 0 );
        }

        userUI->SetCenter( false );
        userUI->SetPos( FramePos( 0, 0, userUI->GetWidth(), userUI->GetHeight() ) );        
        memberList.AddColumnItem( 0, userUI );

        /*userUI = new UserUI;
        if( (*i).listenState ){
            userUI->SetText( (*i).title.c_str(), RGB( 255, 255, 255 ), 640, 0, 0 );
        }else{
            userUI->SetText( (*i).title.c_str(), RGB( 180, 180, 180 ), 640, 0, 0 );
        }
        userUI->SetCenter( false );
        userUI->SetPos( FramePos( 0, 0, userUI->GetWidth(), userUI->GetHeight() ) );
        userUI->userID = (*i).title;
        userList.AddColumnItem( 1, userUI );*/
    }    

    memberList.SetScrollPos( oldPos );
}

void GuildUI::UpdateGuild( const DWORD dwID, const std::string strName, const std::string strRights )
{
	currentGuild.dwID      = dwID;
	currentGuild.strName   = strName;
	currentGuild.strRights = strRights;

	guildNameText.SetText( strName, g_Global.GetBtnTCE() );

	ShowButtons();
}

void GuildUI::Draw(V2SPRITEFX *vsfFX )
{
    int dwXPos = (g_Global.GetScreenW()-232)/2;
    int dwYPos = (g_Global.GetScreenH()-279-150)/2;
    
    GameUI::Draw( vsfFX );
    
    HDC hdc;
    DXDGetDC(&hdc, 716);
    char strBtnTexte[20];

    // Draw Box Header
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[223]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,102)+ 59+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,102)+ 59  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);

	// Leave
	if( currentGuild.dwID != 0 )
	{
		sprintf(strBtnTexte  ,"%s",g_GUILocalString[224]);
		fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,53)+ 22, dwYPos+35 , RGB(0, 0, 0)    , 0, strBtnTexte);
	}
  
	// Invite
	if( currentGuild.strRights[1] == '1' )
	{
		sprintf(strBtnTexte  ,"%s",g_GUILocalString[116]);
		fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 28, dwYPos+219 , RGB(0, 0, 0)    , 0, strBtnTexte);
	}

	// Kick
	if( currentGuild.strRights[2] == '1' )
	{
		sprintf(strBtnTexte, "%s", g_GUILocalString[118] );
		fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 143, dwYPos+219 , RGB(0, 0, 0)    , 0, strBtnTexte);
	}

    DXDReleaseDC(hdc, 716);
}

int GuildUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

void GuildUI::InviteButtonEvent::LeftClicked( void )
{
	GuildUI* guildUI = static_cast< GuildUI* >( GetUI() );

	LOG << "CLICK";

    MouseDevice md;
    if( md.LockNextEvent( DM_CLICK, guildUI, InvitePlayer, 0 ) )
	{
		LOG << "HIDE";
        guildUI->Hide();		
    }
	else
	{
        // Could not lock mouse.
        ChatterUI::GetInstance()->AddBackscrollItem( 
            "", 
            g_LocalString[ 444 ], SYSTEM_COLOR, true
        );
    }
}

unsigned int GuildUI::InvitePlayer( void* lpData )
{
	LOG.Lock();
	LOG << "INVITE";
	LOG.Unlock();
	CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );    

	GuildUI* guildUI = reinterpret_cast< GuildUI* >( lpData );
    
    POINT pntMousePos;
    POINT pntWorldPos;

    // Get the mouse pos.
    MouseDevice md;
    md.GetPosition( (int *)&pntMousePos.x, (int *)&pntMousePos.y );

    Player.ScreenPosToWL( &pntMousePos, &pntWorldPos );
    
    int nX = ( pntMousePos.x + 48 ) / 32;
    int nY = ( pntMousePos.y - 8 ) / 16;

    DWORD dwID = GridID( nX, nY );
    // If found an ID.
    if( dwID != 0 )
    {      
       signed char xPos, yPos;
       Objects.Lock(101);
       Objects.Found( &xPos, &yPos, dwID );
       Objects.Unlock(101);
       
       TFCPacket sending;
       
       // Send an invitation to the user.
       sending << (RQ_SIZE)RQ_GuildInvite;
       sending << (long)dwID;
       sending << (short)( xPos + Player.xPos );
       sending << (short)( yPos + Player.yPos );        
       
       SEND_PACKET( sending );

	   LOG.Lock();
	   LOG << "SEND";
	   LOG.Unlock();
    }

    guildUI->Show();

	return 0;
}

void GuildUI::Join( const DWORD guildID, const string guildName, const string sourceName )
{
	CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );

	this->dwInvitationGuild = guildID;
   
   // If there is already a box in foreground control.
   if( IsForegroundControlled() )
   {
      //LOG << "\r\nFOREGROUND CONTROLLED DECLINE.";
      // Automatically decline
      joinPopup.Decline();
   }
   else
   {    
      //LOG << "\r\nASKING FOREGROUND FOR JOIN POPUP BOX.";
      // If the user was ignored.
      if( ChatterUI::GetInstance()->IsUserIgnored( sourceName ) )
      {
         // Decline.
         joinPopup.Decline();
      }
      else
      {
         char buf[ 1024 ];
         sprintf( buf, g_LocalString[585], sourceName.c_str(), guildName.c_str() );
         joinPopup.SetText( buf );
         joinPopup.Show();
         RequestForegroundControl( &joinPopup );
      }      
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
void GuildUI::LeaveButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Leave button was clicked.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    GuildUI *guildUI = static_cast< GuildUI * >( GetUI() );
    
    // Summon the JunkDrop popupUI.
    guildUI->quitPopup.Show();
    
    guildUI->RequestForegroundControl( &guildUI->quitPopup );
}

//////////////////////////////////////////////////////////////////////////////////////////
void GuildUI::KickButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Kick button was clicked.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
	GuildUI *guildUI = static_cast< GuildUI * >( GetUI() );

	if( guildUI->selectedUser.userID != 0 )
	{
		TFCPacket sending;

		sending << (RQ_SIZE)RQ_GuildKick;
		sending << (long) guildUI->selectedUser.userID;

		SEND_PACKET( sending );

		LOG.Lock();
		LOG << "SEND kick";
		LOG.Unlock();
	}
}

void GuildUI::MemberListEvent::LeftClicked( void )
{
	GuildUI* guildUI = static_cast< GuildUI * >( GetUI() );

	MemberUI* memberUI = static_cast< MemberUI * >( guildUI->memberList.GetSelection() );
	if( guildUI != NULL )
	{        
        guildUI->selectedUser.userID     = memberUI->userID;
		guildUI->selectedUser.username   = memberUI->username;
		guildUI->selectedUser.userRights = memberUI->userRights;
		guildUI->selectedUser.level		 = memberUI->level;
    }
	else
	{
        guildUI->selectedUser.userID	 = 0;
		guildUI->selectedUser.username   = "";
		guildUI->selectedUser.userRights = "000000";
		guildUI->selectedUser.level		 = 0;
    }	
}

//////////////////////////////////////////////////////////////////////////////////////////
void GuildUI::Close( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Closes the ignore list popup UI.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    Hide();    
}

//////////////////////////////////////////////////////////////////////////////////////////
GuildUI::QuitPopup::QuitPopup( void )
//////////////////////////////////////////////////////////////////////////////////////////
:
 okButton( quitEvent ),
 cancelButton( hideEvent )
{
    //Back du popup box...
    int dwOffX = (g_Global.GetScreenW()-240)/2;
    int dwOffY = (g_Global.GetScreenH()-164-150)/2;
    SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+190 ) );

    quitBack.SetSpriteId( "GUI_PopupBack" );
    quitBack.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );
    quitBack.DrawAlpha(true);
    // patch pour la zone de texte
    graphPatch.SetSpriteId( "GUI_TextPopupBack" );
    graphPatch.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+85 ) );
    

    okButton.SetSpriteId   ( "GUI_ButtonUp" );
    okButton.SetDownUI     ( "GUI_ButtonDown" );
    okButton.SetHighlightUI( "GUI_ButtonHUp" );
    okButton.SetPos( FramePos( dwOffX+52, dwOffY+94, dwOffX+52+60, dwOffY+94+32 ) );
    quitEvent.SetUI( this );
    SetHelpId( okButton, 456 );

    cancelButton.SetSpriteId   ( "GUI_ButtonUp" );
    cancelButton.SetDownUI     ( "GUI_ButtonDown" );
    cancelButton.SetHighlightUI( "GUI_ButtonHUp" );
    cancelButton.SetPos( FramePos( dwOffX+128, dwOffY+94, dwOffX+128+60, dwOffY+94+32 ) );
    hideEvent.SetUI( this );
    SetHelpId( cancelButton, 457 );

    textArea.SetPos( FramePos( dwOffX+22, dwOffY+23, dwOffX+217, dwOffY+78 ) );
    textArea.SetCenter( true );

    AddChild( &quitBack );
    AddChild( &graphPatch );
    AddChild( &okButton );
    AddChild( &cancelButton );
    AddChild( &textArea );

    char strText[100];
    sprintf(strText,"%s",g_GUILocalString[225]);
    textArea.SetText( strText, RGB( 223, 157, 0 ), 190 );

    RootBoxUI::GetInstance()->AddChild( this );
}

void GuildUI::QuitPopup::Draw(V2SPRITEFX *vsfFX )
{
    int dwXPos = (g_Global.GetScreenW()-240)/2;
    int dwYPos = (g_Global.GetScreenH()-164-150)/2;
    
    GameUI::Draw( vsfFX );
    
    HDC hdc;
    DXDGetDC(&hdc, 669);
    char strBtnTexte[20];
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[84]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 52, dwYPos+96 , RGB(0, 0, 0)    , 0, strBtnTexte);
    
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[85]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 128, dwYPos+96 , RGB(0, 0, 0)    , 0, strBtnTexte);
    DXDReleaseDC(hdc, 669);
}

int GuildUI::QuitPopup::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GuildUI::QuitPopup::QuitEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Quit the guild..!
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
	QuitPopup *quitUI = static_cast< GuildUI::QuitPopup * >( GetUI() );

	// Sending the packet
    TFCPacket sending;
	
	sending << (RQ_SIZE)RQ_GuildLeave;
	SEND_PACKET( sending );	

	quitUI->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Join popup.

// Constructor ///////////////////////////////////////////////////////////////////////////
GuildUI::JoinPopupUI::JoinPopupUI( void )
:
    acceptButton( acceptButtonEvent ),
    declineButton( declineButtonEvent )

//////////////////////////////////////////////////////////////////////////////////////////
{
    //Back du popup box...
    int dwOffX = (g_Global.GetScreenW()-240)/2;
    int dwOffY = (g_Global.GetScreenH()-164)/2;
    SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );

    JoinBack.SetSpriteId( "GUI_PopupBack" );
    JoinBack.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );
    JoinBack.DrawAlpha(true);
    // patch pour la zone de texte
    graphPatch.SetSpriteId( "GUI_TextPopupBack" );
    graphPatch.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+85 ) );
    

    acceptButton.SetSpriteId   ( "GUI_ButtonUp" );
    acceptButton.SetDownUI     ( "GUI_ButtonDown" );
    acceptButton.SetHighlightUI( "GUI_ButtonHUp" );
    acceptButton.SetPos( FramePos( dwOffX+52, dwOffY+94, dwOffX+52+60, dwOffY+94+32 ) );
    acceptButtonEvent.SetUI( this );
    SetHelpId( acceptButton, 456 );

    declineButton.SetSpriteId   ( "GUI_ButtonUp" );
    declineButton.SetDownUI     ( "GUI_ButtonDown" );
    declineButton.SetHighlightUI( "GUI_ButtonHUp" );
    declineButton.SetPos( FramePos( dwOffX+128, dwOffY+94, dwOffX+128+60, dwOffY+94+32 ) );
    declineButtonEvent.SetUI( this );
    SetHelpId( declineButton, 457 );

    textArea.SetPos( FramePos( dwOffX+22, dwOffY+23, dwOffX+217, dwOffY+78 ) );
    textArea.SetCenter( true );

    AddChild( &JoinBack );
    AddChild( &graphPatch );
    AddChild( &acceptButton );
    AddChild( &declineButton );
    AddChild( &textArea );

    RootBoxUI::GetInstance()->AddChild( this );
}



void GuildUI::JoinPopupUI::Draw(V2SPRITEFX *vsfFX )
{
    int dwXPos = (g_Global.GetScreenW()-240)/2;
    int dwYPos = (g_Global.GetScreenH()-164)/2;
    
    GameUI::Draw( vsfFX );
    
    HDC hdc;
    DXDGetDC(&hdc, 669);
    char strBtnTexte[20];
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[10]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 52, dwYPos+96 , RGB(0, 0, 0)    , 0, strBtnTexte);
    
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[11]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 128, dwYPos+96 , RGB(0, 0, 0)    , 0, strBtnTexte);
    DXDReleaseDC(hdc, 669);
}

int GuildUI::JoinPopupUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GuildUI::JoinPopupUI::Decline( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Refuses an invitation.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
	GuildUI* guildUI = GuildUI::GetInstance();

    // Send a guild join packet.
    TFCPacket sending;
    sending << (RQ_SIZE)RQ_GuildInviteAnswer;
	sending << (long) guildUI->dwInvitationGuild;
	sending << (char) 0; // Decline
    SEND_PACKET( sending );
}

//////////////////////////////////////////////////////////////////////////////////////////
bool GuildUI::JoinPopupUI::VKeyInput
//////////////////////////////////////////////////////////////////////////////////////////
//  Intercepts ESC so it decline the invitation before closing the box.
// 
(
 VKey vKey
)
// Return: bool, true
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( vKey.GetKey() == 27 ){
        Decline();
        Hide();
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void GuildUI::JoinPopupUI::AcceptButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on the accept.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    JoinPopupUI *popupUI = static_cast< GuildUI::JoinPopupUI * >( GetUI() );
    
    TFCPacket sending;
    sending << (RQ_SIZE)RQ_GuildInviteAnswer;
	sending << (long) GuildUI::GetInstance()->dwInvitationGuild;
	sending << (char) 1; // Accept
    SEND_PACKET( sending );

    popupUI->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void GuildUI::JoinPopupUI::DeclineButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on decline.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    JoinPopupUI *popupUI = static_cast< GuildUI::JoinPopupUI * >( GetUI() );

    popupUI->Decline();
    popupUI->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void GuildUI::JoinPopupUI::SetText
//////////////////////////////////////////////////////////////////////////////////////////
//  Set the text area's text.
// 
(
 string text // the text.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    textArea.SetText( text, RGB( 223, 157, 0 ), 190 );
}