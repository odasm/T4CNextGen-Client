// GroupPlayUI.cpp
//
#pragma warning( disable : 4291 )

#include "../pch.h"
#include "GroupPlayUI.h"
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
GroupPlayUI::GroupPlayUI( void ) 
:
    autoShare( false ),    
    weAreTheLeader( false ),
    memberSize( 0 ),
    inviteButton( inviteButtonEvent ),
    leaveButton( leaveButtonEvent ),
    expelButton( expelButtonEvent ),
    memberList( EmptyEvent::GetInstance() ),
    autoShareButtonON( autoShareButtonEvent ),
    M_Button( minimizeEvent ),
    X_Button( hideEvent ),
    disabledInviteButton( EmptyEvent::GetInstance() ),
    disabledLeaveButton( EmptyEvent::GetInstance() ),
    disabledExpelButton( EmptyEvent::GetInstance() ),
    disabledAutoShareButtonON( EmptyEvent::GetInstance() )
//////////////////////////////////////////////////////////////////////////////////////////
{  

    int dwXPos = (g_Global.GetScreenW()-340)/2;
    int dwYPos = (g_Global.GetScreenH()-150-328)/2;
    if(dwYPos <0)
       dwYPos =0;

    SetPos( FramePos( dwXPos , dwYPos, dwXPos+340, dwYPos+328 ) );
    SetHelpId( this, 329 );
    
    backImage.SetSpriteId( "GUI_BackGroupBk" );
    backImage.SetPos( FramePos( dwXPos , dwYPos, dwXPos+340, dwYPos+328 ) );
    backImage.DrawAlpha(true);
    

    
 

    M_Button.SetPos( FramePos( dwXPos+3, dwYPos+300, dwXPos+3+24, dwYPos+300+24 ) );
    M_Button.SetSpriteId( "GUI_M_ButtonUp" );
    M_Button.SetDownUI( "GUI_M_ButtonDown" );
    M_Button.SetHighlightUI( "GUI_M_ButtonHUp" );
    SetHelpId( M_Button, 455 );

    X_Button.SetPos( FramePos( dwXPos+31, dwYPos+300, dwXPos+31+24, dwYPos+300+24 ) );
    X_Button.SetSpriteId( "GUI_X_ButtonUp" );
    X_Button.SetDownUI( "GUI_X_ButtonDown" );
    X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
    SetHelpId( X_Button, 261 );


    // Buttons
    
    
    inviteButtonEvent.SetUI( this );
    inviteButton.SetPos( FramePos( dwXPos+84, dwYPos+290, dwXPos+84+60, dwYPos+290+32 ) );
    inviteButton.SetSpriteId( "GUI_ButtonUp" );
    inviteButton.SetDownUI( "GUI_ButtonDown" );
    inviteButton.SetHighlightUI( "GUI_ButtonHUp" );
    SetHelpId( inviteButton, 324 );
    disabledInviteButton.SetPos( FramePos( dwXPos+84, dwYPos+290, dwXPos+84+60, dwYPos+290+32 ) );
    disabledInviteButton.SetSpriteId( "GUI_ButtonDisabled" );
    SetHelpId( disabledInviteButton, 324 );


    leaveButtonEvent.SetUI( this );
    leaveButton.SetPos( FramePos( dwXPos+147, dwYPos+290, dwXPos+147+60, dwYPos+290+32 ) );
    leaveButton.SetSpriteId( "GUI_ButtonUp" );
    leaveButton.SetDownUI( "GUI_ButtonDown" );
    leaveButton.SetHighlightUI( "GUI_ButtonHUp" );
    SetHelpId( leaveButton, 325 );
    disabledLeaveButton.SetPos( FramePos( dwXPos+147, dwYPos+290, dwXPos+147+60, dwYPos+290+32 ) );
    disabledLeaveButton.SetSpriteId( "GUI_ButtonDisabled" );    
    SetHelpId( disabledLeaveButton, 325 );

    expelButtonEvent.SetUI( this );
    expelButton.SetPos( FramePos( dwXPos+211, dwYPos+290, dwXPos+211+60, dwYPos+290+32 ) );
    expelButton.SetSpriteId( "GUI_ButtonUp" );
    expelButton.SetDownUI( "GUI_ButtonDown" );
    expelButton.SetHighlightUI( "GUI_ButtonHUp" );
    SetHelpId( expelButton, 326 );
    disabledExpelButton.SetPos( FramePos( dwXPos+211, dwYPos+290, dwXPos+211+60, dwYPos+290+32 ) );
    disabledExpelButton.SetSpriteId( "GUI_ButtonDisabled" );    
    SetHelpId( disabledExpelButton, 326 );


    autoShareButtonEvent.SetUI( this );
    autoShareButtonON.SetPos( FramePos( dwXPos+274, dwYPos+290, dwXPos+274+60, dwYPos+290+32 ) );
    autoShareButtonON.SetSpriteId( "GUI_ButtonUp" );
    autoShareButtonON.SetDownUI( "GUI_ButtonDown" );
    autoShareButtonON.SetHighlightUI( "GUI_ButtonHUp" );
    SetHelpId( autoShareButtonON, 327 );
    disabledAutoShareButtonON.SetPos( FramePos( dwXPos+274, dwYPos+290, dwXPos+274+60, dwYPos+290+32 ) );
    disabledAutoShareButtonON.SetSpriteId( "GUI_ButtonDisabled" );
    SetHelpId( disabledAutoShareButtonON, 327 );

    backBtnSplitD.SetSpriteId("GUI_ButtonOff");
    backBtnSplitD.SetPos( FramePos( dwXPos+274, dwYPos+290, dwXPos+274+60, dwYPos+290+32 ) );
    m_bShowDisableSplit = FALSE;

    // Member list.
    memberList.SetPos( FramePos( dwXPos+8, dwYPos+39, dwXPos+333, dwYPos+276 ) );    
    memberList.SetupList( 3, 24, FramePos( 0, 0, 7, 7 ) );
    memberList.SetColumnSize( 0, 148 );
    memberList.SetColumnSize( 1, 121 );
    memberList.SetColumnSize( 2, 42 );
    memberList.CenterColumn( 2 );
    memberSelectionGraphPatch.SetSpriteId( "GUI_Group_Select" );
    memberList.SetSelectionSprite( &memberSelectionGraphPatch, -1, 0 );
    SetHelpId( memberList, 454 );

    for(int i=0;i<7;i++)
    {
      m_MinimizedInfo[i].grfBack.SetSpriteId( "GUI_BackGroupChStat");
      m_MinimizedInfo[i].grfBack.SetPos( FramePos( g_Global.GetScreenW()-148, 93+(i*50), g_Global.GetScreenW(), 50+93+(i*50)) );
      m_MinimizedInfo[i].grfBack.DrawAlpha(true);
      m_MinimizedInfo[i].grfHP  .SetSpriteId( "GUI_BackChStat_HP");
      m_MinimizedInfo[i].grfHP  .SetPos( FramePos( g_Global.GetScreenW()-118, 122+(i*50), g_Global.GetScreenW()-16, 12+122+(i*50)) );
    }
    
    AddChild( &backImage );
    AddChild( &disabledInviteButton );
    AddChild( &disabledLeaveButton );
    AddChild( &disabledExpelButton );
    AddChild( &disabledAutoShareButtonON );
    AddChild( &M_Button );
    AddChild( &X_Button );
    AddChild( &inviteButton );
    AddChild( &leaveButton );
    AddChild( &expelButton );
    AddChild( &autoShareButtonON );
    AddChild( &memberList );
    

    ResolveGraphPatches(); 

    RootBoxUI::GetInstance()->AddChild( this );
}
//////////////////////////////////////////////////////////////////////////////////////////
// Join popup.

// Constructor ///////////////////////////////////////////////////////////////////////////
GroupPlayUI::JoinPopupUI::JoinPopupUI( void )
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



void GroupPlayUI::JoinPopupUI::Draw(V2SPRITEFX *vsfFX )
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

int GroupPlayUI::JoinPopupUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::JoinPopupUI::Decline( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Refuses an invitation.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    //const RQ_GroupLeave = 80;

    // Send a GroupJoin packet.
    TFCPacket sending;
    sending << (RQ_SIZE)RQ_GroupLeave;
    SEND_PACKET( sending );
}

//////////////////////////////////////////////////////////////////////////////////////////
bool GroupPlayUI::JoinPopupUI::VKeyInput
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
void GroupPlayUI::JoinPopupUI::AcceptButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on the accept.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    JoinPopupUI *popupUI = static_cast< GroupPlayUI::JoinPopupUI * >( GetUI() );
    
    //const RQ_GroupJoin = 79;

    // Send a GroupJoin packet.
    TFCPacket sending;
    sending << (RQ_SIZE)RQ_GroupJoin;
    SEND_PACKET( sending );

    popupUI->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::JoinPopupUI::DeclineButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on decline.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    JoinPopupUI *popupUI = static_cast< GroupPlayUI::JoinPopupUI * >( GetUI() );

    popupUI->Decline();
    popupUI->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::JoinPopupUI::SetText
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
    
// Destructor ////////////////////////////////////////////////////////////////////////////
GroupPlayUI::~GroupPlayUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
}


//////////////////////////////////////////////////////////////////////////////////////////
GroupPlayUI *GroupPlayUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the sole group play box instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static GroupPlayUI groupPlay;

    return &groupPlay;
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int GroupPlayUI::Display
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
    
    // If already shown.
    if( GetInstance()->IsShown() && !GetInstance()->IsMinimized() ){
        GetInstance()->Hide();
    }
    else
    {
        if( GetInstance()->IsMinimized() )
        {
            GetInstance()->Minimize( false );
        }
        GetInstance()->Show();
    }

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::InviteButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Invite button got pressed.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    GroupPlayUI *groupUI = static_cast< GroupPlayUI * >( GetUI() );

    MouseDevice md;
    if( md.LockNextEvent( DM_CLICK, groupUI, InvitePlayer, 0 ) ){
        groupUI->Hide();
    }else{
        // Could not lock mouse.
        ChatterUI::GetInstance()->AddBackscrollItem( 
            "", 
            g_LocalString[ 444 ], SYSTEM_COLOR, true
        );
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
unsigned int GroupPlayUI::InvitePlayer
//////////////////////////////////////////////////////////////////////////////////////////
// Called when the mouse event has selected a player.
// 
(
 void *data // The gameUI.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
    
    GroupPlayUI *groupUI = reinterpret_cast< GroupPlayUI * >( data );
    
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
       //const RQ_GroupInvite = 78;
       
       signed char xPos, yPos;
       Objects.Lock(101);
       Objects.Found( &xPos, &yPos, dwID );
       Objects.Unlock(101);
       
       TFCPacket sending;
       
       // Send an invitation to the user.
       sending << (RQ_SIZE)RQ_GroupInvite;
       sending << (long)dwID;
       sending << (short)( xPos + Player.xPos );
       sending << (short)( yPos + Player.yPos );        
       
       SEND_PACKET( sending );
    }

    groupUI->Show();

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::LeaveButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Leave button got pressed.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    //const RQ_GroupLeave = 80;    

    // Send a GroupLeave request.
    TFCPacket sending;

    sending << (RQ_SIZE)RQ_GroupLeave;

    SEND_PACKET( sending );
}

//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::ExpelButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Expel button got clicked.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    GroupPlayUI *groupUI = static_cast< GroupPlayUI * >( GetUI() );

    // All stocked items are GroupMembers by multiple inheritance.
    TextMemberUI *member = static_cast< TextMemberUI * >( groupUI->memberList.GetSelection() );

    // If no selection.
    if( member == NULL )
    {
        return;
    }

    //const RQ_GroupKick = 81;
    TFCPacket sending;
    // Send a group kick packet.
    sending << (RQ_SIZE)RQ_GroupKick;
    sending << (long)member->id;

    SEND_PACKET( sending );
}

//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::AutoShareButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Clicked on the autoshare button.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
   GroupPlayUI *groupUI = static_cast< GroupPlayUI * >( GetUI() );
   
   //const RQ_GroupToggleAutoSplit = 88;
   
   TFCPacket sending;
   
   sending << (RQ_SIZE)RQ_GroupToggleAutoSplit;
   sending << (char)( groupUI->autoShare ? 0 : 1 );
   
   
   SEND_PACKET( sending );
}

//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::Join(DWORD sourceID,string sourceName)
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
   
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
         sprintf( buf, g_LocalString[ 354 ], sourceName.c_str() );
         joinPopup.SetText( buf );
         joinPopup.Show();
         RequestForegroundControl( &joinPopup );
      }
      
   }
}


//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::UpdateMembers(const list< GroupPlayUI::GroupMember > &members,bool newShare)
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
   
   memberList.DeleteList();
   list< GroupMember >::const_iterator i;
   weAreTheLeader = false;
   autoShare = newShare;
   memberSize = members.size();
   
   for( i = members.begin(); i != members.end(); i++ )
   {
      memberList.AddNewItem();
      //LOG << "\r\nUpdating: found user " << (char*)(*i).name.c_str();
      if( (*i).leader && (*i).id == Player.ID )
         weAreTheLeader = true;
      
      // Add the user name.
      {
         TextMemberUI *memberUI = new TextMemberUI;
         if( (*i).leader )
            memberUI->SetText( (*i).name.c_str(), NearLeaderColor, g_Global.GetScreenW(), 0, 0 );
         else
            memberUI->SetText( (*i).name.c_str(), NearMemberColor, g_Global.GetScreenW(), 0, 0 );
         memberUI->SetCenter( false );
         memberUI->SetPos( FramePos( 0, 0, memberUI->GetWidth(), memberUI->GetHeight() ) );
         memberUI->SetMember( (*i) );
         SetHelpId( memberUI, 501 );
         memberList.AddColumnItem( 0, memberUI );
      }
      // Add the user HP graph bar.
      {
         HpMemberUI *hpUI = new HpMemberUI;
         hpUI->SetPos( FramePos( 0, 0, 102, 12 ) );
         hpUI->SetMember( *i );
         memberList.AddColumnItem( 1, hpUI );
      }
      
      // Add the user level.
      {
         TextMemberUI *memberUI = new TextMemberUI;
         
         char buf[ 256 ];
         
         if( (*i).leader )
            memberUI->SetText( itoa( (*i).level, buf, 10 ), NearLeaderColor, 640, 0, 0 );
         else
            memberUI->SetText( itoa( (*i).level, buf, 10 ), NearMemberColor, 640, 0, 0 );
         memberUI->SetCenter( false );
         memberUI->SetPos( FramePos( 0, 0, memberUI->GetWidth(), memberUI->GetHeight() ) );        
         memberUI->SetMember( *i );
         
         memberList.AddColumnItem( 2, memberUI );
      }
   }
   
   ResolveGraphPatches();    
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  This functions simply hide or shows the right graph patches/controls
// 
//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::ResolveGraphPatches( void )
{
   if( weAreTheLeader )
   {
      // Enable leader controls.
      leaveButton.Show();
      expelButton.Show();
      inviteButton.Show();
      autoShareButtonON.Show();
      
      if( autoShare )
      {
         m_bShowDisableSplit = FALSE;
      }
      else
      {
         m_bShowDisableSplit = TRUE;
      }
      
      disabledLeaveButton.Hide();
      disabledExpelButton.Hide();
      disabledInviteButton.Hide();
      disabledAutoShareButtonON.Hide();
   }
   else
   {
      // If there are members in the group.
      if( memberSize != 0 )
      {
         // Enable member functions.
         leaveButton.Show();
         expelButton.Hide();
         inviteButton.Hide();
         
         if( autoShare )
         {
            disabledAutoShareButtonON.Show();
            autoShareButtonON.Hide();
            m_bShowDisableSplit = FALSE;
         }
         else
         {
            disabledAutoShareButtonON.Hide();
            autoShareButtonON.Show();
            m_bShowDisableSplit = TRUE;
         }                
         disabledLeaveButton.Hide();
         disabledExpelButton.Show();
         disabledInviteButton.Show();            
      }
      else
      {
         // Enable non-group functions.
         leaveButton.Hide();
         expelButton.Hide();
         inviteButton.Show();
         autoShareButtonON.Hide();
         
         disabledLeaveButton.Show();
         disabledExpelButton.Show();
         disabledInviteButton.Hide();
         disabledAutoShareButtonON.Show();
         m_bShowDisableSplit = TRUE;
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the auto-split status of the group.
//
//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::UpdateAutoShare(bool newShare)
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
   
   autoShare = newShare;
   
   if( autoShare )
      ChatterUI::GetInstance()->AddBackscrollItem("",g_LocalString[ 523 ],SYSTEM_COLOR,true);
   else
      ChatterUI::GetInstance()->AddBackscrollItem("",g_LocalString[ 524 ],SYSTEM_COLOR,true);
   
   ResolveGraphPatches();
}
//////////////////////////////////////////////////////////////////////////////////////////
//  Real-time update of a group member's hp.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::UpdateMemberHp(DWORD id,WORD hpPercent)
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
   
   // Get the list's grid
   vector< vector< ListUI::GridItem > > *groupGrid = memberList.GetGrid();
   
   // Scroll through all the graph hp bars in the list.
   UINT i;
   for( i = 0; i != (*groupGrid)[ 1 ].size(); i++ )
   {
      // Get the HpMemberUI.
      HpMemberUI *hpUI = static_cast< HpMemberUI * >( (*groupGrid)[ 1 ][ i ] );
      
      // If this is the player we are looking for.
      if( hpUI->id == id )
      {
         // Update its hp percentage.
         hpUI->hpPercent = hpPercent;
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the text member.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::TextMemberUI::Draw(V2SPRITEFX *vsfFX )
{
   Objects.Lock(143);
   // If the user is within range.
   if( Objects.FoundID( id ) || id == Player.ID )
   {
      Objects.Unlock(143);
      
      // Set its text. The function should be very fast as it shouldn't do
      // anything else than a couple of tests if the text and color didn't change.
      if( leader )
         SetText( GetText()->GetText(), NearLeaderColor, g_Global.GetScreenW(), 0, 0 );
      else
         SetText( GetText()->GetText(), NearMemberColor, g_Global.GetScreenW(), 0, 0 );
   }
   else
   {
      Objects.Unlock(143);
      
      if( leader )
         SetText( GetText()->GetText(), FarLeaderColor, g_Global.GetScreenW(), 0, 0 );
      else
         SetText( GetText()->GetText(), FarMemberColor, g_Global.GetScreenW(), 0, 0 );
   }
   StaticTextUI::Draw( vsfFX );    
}

//////////////////////////////////////////////////////////////////////////////////////////
GroupPlayUI::HpMemberUI::HpMemberUI( void )
{
    fullBar.SetSpriteId( "GUI_BackChStat_HP" );
    emptyBar.SetSpriteId( "GUI_BackChStat_Empty" );
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Draws a member's HP.
//  
//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::HpMemberUI::Draw(V2SPRITEFX *vsfFX)
{ 
    // Display the full bar.
    FramePos emptyPos = GetPos();
    emptyPos.y -=1;
    emptyPos.maxy -=1;
    emptyPos.x += 7;
    emptyPos.maxx += 7;
    emptyBar.SetPos( emptyPos );    
    emptyBar.Draw( vsfFX );
 
    fullBar.SetPos( emptyPos );
 
    // Create a clipping area according to the group member's hp.
    long newWidth = emptyPos.Width() * ( 100 - hpPercent ) / 100;
    RECT clipRect = { emptyPos.x, emptyPos.y, emptyPos.maxx - newWidth, emptyPos.maxy };
    V2SPRITEFX fx;
    memset( &fx, 0, sizeof( fx ) );
    fx.lpClipRect = &clipRect;
    fullBar.Draw( &fx );
}

void GroupPlayUI::Draw(V2SPRITEFX *vsfFX)
{
   if(IsMinimized())
   {
      GameUI::Draw( vsfFX );
      return;
   }
   
   GameUI::Draw( vsfFX );
   // AddOverDraw...
   int dwXPos = (g_Global.GetScreenW()-340)/2;
   int dwYPos = (g_Global.GetScreenH()-150-328)/2;
   if(dwYPos <0)
       dwYPos =0;

   HDC hdc;
   DXDGetDC(&hdc, 666);

   char strName [20];
   char strHP   [20];
   char strLevel[20];
   sprintf(strName ,"%s",g_GUILocalString[39]);
   sprintf(strHP   ,"%s",g_GUILocalString[40]);
   sprintf(strLevel,"%s",g_GUILocalString[120]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strName ,67)+ 48, dwYPos+12 , RGB(223, 157, 0), 0, strName);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strHP   ,50)+198, dwYPos+12 , RGB(223, 157, 0), 0, strHP);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strLevel,42)+291, dwYPos+12 , RGB(223, 157, 0), 0, strLevel);

   //texte sur les boutons
   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[116]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 84, dwYPos+292+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[117]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 147, dwYPos+292+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[118]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 211, dwYPos+292+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[119]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 274, dwYPos+292+2 , RGB(0, 0, 0)    , 0, strBtnTexte);
   DXDReleaseDC(hdc, 666);

   if(m_bShowDisableSplit)
      backBtnSplitD.Draw(vsfFX);
}

int GroupPlayUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the minimized grouplay UI.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::DrawMinimized( V2SPRITEFX *vsfFX )
{
    std::vector< std::vector< ListUI::GridItem > > &grid = *memberList.GetGrid();

    const int space = 22;
    int pass = 0;
    UINT i;
    int dwNbrGrp = 0;  
    for( i = 0; i < grid[0].size(); i++ )
    {
        // Do not display the user's HP twice.        
        TextMemberUI *playerName = static_cast< TextMemberUI * >( grid[ 0 ][ i ] );
        if( stricmp( playerName->GetText()->GetText(), Player.Name ) == 0 )
        {
            pass += space;
            continue;
        }
        m_MinimizedInfo[dwNbrGrp].grfBack.Draw(vsfFX);
         
        HpMemberUI *hpUI = static_cast< HpMemberUI * >( grid[ 1 ][ i ] );
        FramePos HPPos = m_MinimizedInfo[dwNbrGrp].grfHP.GetPos();
        
        long newWidth = HPPos.Width() * ( 100 - hpUI->hpPercent ) / 100;
        RECT clipRect = { HPPos.x, HPPos.y, HPPos.maxx - newWidth, HPPos.maxy };
        V2SPRITEFX fx;
        memset( &fx, 0, sizeof( fx ) );
        fx.lpClipRect = &clipRect;
        m_MinimizedInfo[dwNbrGrp].grfHP  .Draw( &fx );

        Objects.Lock(668);
        BOOL bNear = Objects.FoundID( hpUI->id );
        Objects.Unlock(668);

        HDC hdc;
        DXDGetDC(&hdc, 667);
        fSkillFont ->DrawFont(hdc, g_Global.GetScreenW()-143, 122+(dwNbrGrp*50), RGB(223, 157, 0), 0, World.HPString);
         
        
        if(bNear || hpUI->id == Player.ID)
           fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW()-138, 100+(dwNbrGrp*50), RGB(223, 157, 0), 0, (char *)playerName->GetText()->GetText());
        else
           fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW()-138, 100+(dwNbrGrp*50), RGB(180, 120, 0), 0, (char *)playerName->GetText()->GetText());

        DXDReleaseDC(hdc, 667);
        dwNbrGrp++;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
//  Minimizes the group play UI.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void GroupPlayUI::Minimize(bool newMinimizedState)
{
   LOG.Lock();
   LOG << "\r\nCalling GroupPlayer::Minimize (" << newMinimizedState << " )";
   LOG.Unlock();
   
   GameUI::Minimize( newMinimizedState );
   
   //std::vector< std::vector< ListUI::GridItem > > &grid = *memberList.GetGrid();//BLBLBL 29 mars 2009 suppression des choses inutilisées
   
   if( newMinimizedState == false )
   {
      int dwXPos = (g_Global.GetScreenW()-340)/2;
      int dwYPos = (g_Global.GetScreenH()-150-328)/2;
      if(dwYPos <0)
         dwYPos =0;

      SetPos( FramePos( dwXPos , dwYPos, dwXPos+340, dwYPos+328 ) );
   }
   else
   {       
      SetPos( FramePos( g_Global.GetScreenW()-148, 94, g_Global.GetScreenW(), 94/*+grid[0].size()*50*/ ) );
   }
} 