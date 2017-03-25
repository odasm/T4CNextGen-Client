// RobUI.cpp: implementation of the RobUI class.
//
//////////////////////////////////////////////////////////////////////
#include "../pch.h"
#pragma warning(disable: 4786 )
#include "RobUI.h"
#include "RootBoxUI.h"
#include "ChatterUI.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]

extern Font *fNewGuiBtnFont;

using namespace std;

namespace{
    T3VSBSound *naSound = NULL;

    void InitSound(){
        if( naSound == NULL ){
            naSound = new T3VSBSound;
            naSound->Create( "General NA sound", TS_MEMORY );
        }
    }
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RobUI::RobUI()
:
   itemGrid( itemGridEvent ),
   X_Button( hideEvent ),
   robButton( robButtonEvent ),
   disabledRobButton( EmptyEvent::GetInstance() )
{
   InitSound();

   int dwXPos = (g_Global.GetScreenW()-550)/2;
   int dwYPos = (g_Global.GetScreenH()-150-199)/2;
   if(dwYPos <0)
       dwYPos =0;
   
   SetPos( FramePos( dwXPos, dwYPos, dwXPos+550, dwYPos+199 ) );
   SetHelpId( this, 285 );
   
   backInv.SetSpriteId( "GUI_BackRob" );
   backInv.SetPos( FramePos( dwXPos, dwYPos, dwXPos+550, dwYPos+199 ) );
   backInv.DrawAlpha(true);
   
   X_Button.SetPos( FramePos( dwXPos+550-28, dwYPos+0, dwXPos+550-4, dwYPos+0+24 ) );
   X_Button.SetSpriteId( "GUI_X_ButtonUp" );
   X_Button.SetDownUI( "GUI_X_ButtonDown" );
   X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
   SetHelpId( X_Button, 261 );
   
   
   // 248, 116    x148 y176
   itemGrid.SetPos( FramePos( dwXPos+24, dwYPos+39, dwXPos+459, dwYPos+155 ) );
   itemGrid.SetScrollRegions(
      FramePos( dwXPos+437, dwYPos+45 , dwXPos+454, dwYPos+66 ), 
      FramePos( dwXPos+437, dwYPos+131, dwXPos+454, dwYPos+151 ),
      FramePos( dwXPos+437, dwYPos+67 , dwXPos+454, dwYPos+123 ),
      NULL
      );
   itemGrid.SetGrid( 14, 4, FramePos( 0, 0, 26, 26 ), FramePos( 0, 0, 3, 3 ) );
   gridScrollUpGraph .SetPos ( FramePos( dwXPos+437, dwYPos+45 , dwXPos+454, dwYPos+66 ) );
   gridScrollDownGraph.SetPos( FramePos( dwXPos+437, dwYPos+131, dwXPos+454, dwYPos+151 ) );
   itemGrid.SetScrollUpDownUI( &gridScrollUpGraph );
   itemGrid.SetScrollDownDownUI( &gridScrollDownGraph );
   gridSelectedTile.SetSpriteId( "GUI_BackInvGridSelect" );
   itemGrid.SetSelectedTileUI( &gridSelectedTile );
   SetHelpId( itemGrid, 522 );
   
   instantItemInfo.SetPos( FramePos( dwXPos+208, dwYPos+167, dwXPos+453, dwYPos+183 ) );
   instantItemInfo.SetCenter( false );

   targetName     .SetPos( FramePos( dwXPos+30, dwYPos+167, dwXPos+173, dwYPos+183 ) );
   targetName     .SetCenter( false );
   
   robButton.SetPos( FramePos( dwXPos+478, dwYPos+124, dwXPos+478+60, dwYPos+124+32 ) );
   robButton.SetSpriteId   ( "GUI_ButtonUp"   );
   robButton.SetDownUI     ( "GUI_ButtonDown" );
   robButton.SetHighlightUI( "GUI_ButtonHUp"  );
   SetHelpId( &robButton, 518 );
   
   disabledRobButton.SetPos( FramePos( dwXPos+478, dwYPos+124, dwXPos+478+60, dwYPos+124+32 ) );
   disabledRobButton.SetSpriteId( "GUI_ButtonDisabled" );
   
   
   itemGridEvent.SetUI( this );
   robButtonEvent.SetUI( this );
   
   targetName.SetText( "" );
   instantItemInfo.SetText( "" );
   
   AddChild( &backInv );
   AddChild( &disabledRobButton );
   AddChild( &X_Button );
   AddChild( &robButton );
   AddChild( &itemGrid );
   AddChild( &targetName );
   AddChild( &instantItemInfo );
   
   RootBoxUI::GetInstance()->AddChild( this );
}

RobUI::~RobUI()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the rob UI.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void RobUI::Draw(V2SPRITEFX *vsfFX)
{
   int dwXPos = (g_Global.GetScreenW()-550)/2;
   int dwYPos = (g_Global.GetScreenH()-150-199)/2;
   if(dwYPos <0)
       dwYPos =0;
   
   GameUI::Draw( vsfFX );
   
   HDC hdc;
   DXDGetDC(&hdc, 710);
   
   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[82]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 478, dwYPos+124+4 , RGB(0, 0, 0)    , 0, strBtnTexte);
   DXDReleaseDC(hdc, 710);
   
   // Get the current mouse coordinates.
   MousePos mousePos( MouseDevice::xPos, MouseDevice::yPos );
   
   ItemUI * itemUI = NULL;
   if( itemGrid.GetPos().MousePosInFrame( mousePos ) ){
      itemUI = 
         static_cast< ItemUI * >( itemGrid.GetGridItemAt( mousePos ) );
   }
   
   // Display the instant help text.
   if( itemUI != NULL )
   {
      TextObject *to = itemUI->GetHelpText();
      
      if( to != NULL )
      {
         instantItemInfo.SetText(to->GetText(),RGB( 223, 157, 0 ),246);
         
         instantItemInfo.Draw( vsfFX );
      }
   }
}

int RobUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
RobUI *RobUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns a single instance of the RobUI
//////////////////////////////////////////////////////////////////////////////////////////
{
    static RobUI instance;
    return &instance;
}


//////////////////////////////////////////////////////////////////////////////////////////
void RobUI::Rob
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the user in rob mode.
// 
(
 bool canRob,                   // false if the user only peeks.
 string targetName,             // The target's name.
 list< RobUI::Item > &itemList  // The target backpack's items.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( RootBoxUI::GetInstance()->GetLock() );

    // If there is already a box in foreground control.
    if( IsForegroundControlled() ){
        return;
    }

    
    list< Item >::iterator i;

    // Destroy the previous rob.
    itemGrid.DeleteGrid();

    // Create an item UI for all items in the target's backpack.
    for( i = itemList.begin(); i != itemList.end(); i++ ){
        ItemUI *itemUI = new ItemUI;
        
        itemUI->itemData = *i;
        itemUI->SetVSF( InvItemIcons( (*i).appearance ) );
        itemUI->SetPos( FramePos( 0, 0, itemUI->GetVSF()->GetWidth(), itemUI->GetVSF()->GetHeight() ) );

        itemUI->SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, (*i).itemName.c_str() ) );

        itemGrid.AddItem( itemUI );
    }

    RobUI::targetName.SetText( targetName.c_str(),RGB( 223, 157, 0 ) );
    
    if( canRob ){
        disabledRobButton.Hide();
        robButton.Show();
    }else{
        disabledRobButton.Show();
        robButton.Hide();
    }

    RobUI::canRob = canRob;

        
    // Put this window in foreground control.
    Show();
    RequestForegroundControl( this );
}


//////////////////////////////////////////////////////////////////////////////////////////
void RobUI::DispellRob( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Prematurly abort the robbing
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( RootBoxUI::GetInstance()->GetLock() );
    
    // Hide the window
    Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void RobUI::Hide( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Destroy the rob ressources when hiding.
//////////////////////////////////////////////////////////////////////////////////////////
{
    itemGrid.DeleteGrid();
    BoxUI::Hide();

    // If the player had selected an item
    GameUI *itemUI = GetDragItem();
    if( itemUI != NULL ){
        // Reset drag.
        CancelDrag();
        CMouseCursor::GetInstance()->SetCustomCursor( NULL );
        
        // Delete it.
        delete itemUI;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
void RobUI::Rob( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Robs the current drag item.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    ItemUI *itemUI = static_cast< ItemUI * >( GetDragItem() );   

    if( itemUI == NULL ){
        naSound->Play( TRUE );

        ChatterUI::GetInstance()->AddBackscrollItem(
            "",
            g_LocalString[ 516 ],
            SYSTEM_COLOR,
            true
        );
        
        return;
    }

    TFCPacket sending;
    sending << (RQ_SIZE)RQ_Rob;
    sending << (long)itemUI->itemData.id;
    SEND_PACKET( sending );

    Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void RobUI::ItemGridEvent::LeftDblClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Double-clicked the item grid.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    RobUI *robUI = static_cast< RobUI * >( GetUI() );
    if( robUI->canRob ){
        static_cast< RobUI * >( GetUI() )->Rob();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void RobUI::RobButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Clicked the rob button.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    static_cast< RobUI * >( GetUI() )->Rob();
}

//////////////////////////////////////////////////////////////////////////////////////////
bool RobUI::ItemGridEvent::Dropping( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Dropping something on the item grid.
// 
// Return: bool, true if the item is an ItemUI
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( GetDragItem() == NULL ){
        return false;
    }
    if( typeid( *GetDragItem() ) == typeid( RobUI::ItemUI ) ){
        return true;
    }

    return false;
}
