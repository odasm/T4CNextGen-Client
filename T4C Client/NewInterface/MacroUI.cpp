// MacroUI.cpp
//
#include "../pch.h"
#pragma warning( disable:4786 )

#include "MacroUI.h"
#include "../MemoryManager.h"
#include "RootBoxUI.h"
#include "ChatterUI.h"

#include "SpellUI.h"
#include "CharacterUI.h"
#include "InventoryUI.h"
#include "../SystemAction.h"
#include <set>
#include "..\SaveGame.h"
extern CSaveGame g_SaveGame;
#include "..\GUILocalString.h"  //g_GUILocalString[]


extern Font *fNewGuiBtnFont;
extern Font *fNewGuiFont;

namespace{    
    set< MacroEvent * > eventSet;

    const int MacroGridSize = 6;

    const int MacroNameSize = 183;

    T3VSBSound *naSound = NULL;

    void InitSound(){
        if( naSound == NULL ){
            naSound = new T3VSBSound;
            naSound->Create( "General NA sound", TS_MEMORY );
        }
    }

};

//////////////////////////////////////////////////////////////////////////////////////////
inline bool IsKeyValidWithCtrl
//////////////////////////////////////////////////////////////////////////////////////////
//  Determines if a VK_ key is a valid macro used in conjunction with CTRL
// 
(
 WORD key // The VK_
)
// Return: bool, true if its valid.
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( ( key >= 'A' && key <= 'Z' ) ||
        ( key >= '0' && key <= '9' ) ){
        return true;
    }
    switch( key ){
    case VK_BACK:
    case VK_TAB:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_END:
    case VK_HOME:
    case VK_INSERT:
    case VK_DELETE:
    case VK_NUMPAD0:
    case VK_NUMPAD1:
    case VK_NUMPAD2:
    case VK_NUMPAD3:
    case VK_NUMPAD4:
    case VK_NUMPAD5:
    case VK_NUMPAD6:
    case VK_NUMPAD7:
    case VK_NUMPAD8:
    case VK_NUMPAD9:
    case VK_MULTIPLY:
    case VK_ADD:
    case VK_SEPARATOR:
    case VK_SUBTRACT:
    case VK_DECIMAL:
    case VK_DIVIDE:
    case VK_F1: // steph ajout
    case VK_F2:
    case VK_F3:
    case VK_F4:
    case VK_F5:
    case VK_F6:
    case VK_F7:
    case VK_F8:
    case VK_F9:
    case VK_F10:
    case VK_F11:
    case VK_F12:
    case VK_F13:
    case VK_F14:
    case VK_F15:
    case VK_F16:
    case VK_F17:
    case VK_F18:
    case VK_F19:
    case VK_F20:
    case VK_F21:
    case VK_F22:
    case VK_F23:
    case VK_F24:
        return true;
    };

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
inline bool IsKeyValidWithShiftOrNothing
//////////////////////////////////////////////////////////////////////////////////////////
//  Determines if a VK_ is a valid macro used in conjucnction with SHIFT or no key
// combination at all.
// 
(
 WORD key // The VK_
)
// Return: bool, true if its valid.
//////////////////////////////////////////////////////////////////////////////////////////
{
    switch( key ){
    case VK_F1: // steph ajout
    case VK_F2:
    case VK_F3:
    case VK_F4:
    case VK_F5:
    case VK_F6:
    case VK_F7:
    case VK_F8:
    case VK_F9:
    case VK_F10:
    case VK_F11:
    case VK_F12:
    case VK_F13:
    case VK_F14:
    case VK_F15:
    case VK_F16:
    case VK_F17:
    case VK_F18:
    case VK_F19:
    case VK_F20:
    case VK_F21:
    case VK_F22:
    case VK_F23:
    case VK_F24:
        return true;
    };

    return false;
}


//////////////////////////////////////////////////////////////////////////////////////////
MacroUI::MacroUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
:
    macroList( macroListEvent ),
    addSpellButton( addSpellButtonEvent ),
    addSkillButton( addSkillButtonEvent ),
    addItemButton( addItemButtonEvent ),
    removeMacroButton( removeMacroButtonEvent ),
    addToGridButton( addToGridButtonEvent ),
    removeFromGridButton( removeFromGridButtonEvent ),
    disableMacro( false ),
    macroGrid( MacroGridSize ),
    gridSize( 0 ),
    selectedGridItem( NULL ),
    X_Button( hideEvent )
{
    InitSound();
    
    
    int dwXPos = (g_Global.GetScreenW()-576)/2;
    int dwYPos = (g_Global.GetScreenH()-150-368)/2;
    if(dwYPos <0)
       dwYPos =0;
    
    SetPos( FramePos( dwXPos, dwYPos, dwXPos+576, dwYPos+368 ) );
    SetHelpId( this, 377 );
    
    backMacro.SetSpriteId( "GUI_BackMacro" );
    backMacro.SetPos( FramePos( dwXPos, dwYPos, dwXPos+576, dwYPos+368 ) );
    backMacro.DrawAlpha(true);
    
    X_Button.SetPos        ( FramePos( dwXPos+576-28, dwYPos+0, dwXPos+576-4, dwYPos+0+24 ) );
    X_Button.SetSpriteId   ( "GUI_X_ButtonUp" );
    X_Button.SetDownUI     ( "GUI_X_ButtonDown" );
    X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
    SetHelpId( X_Button, 261 );
    
    scrollUpGraph  .SetPos( FramePos( dwXPos+382, dwYPos+41 , dwXPos+402, dwYPos+60  ) );
    scrollDownGraph.SetPos( FramePos( dwXPos+382, dwYPos+282, dwXPos+402, dwYPos+303 ) );

    macroListSelection.SetPos( FramePos( dwXPos+64, dwYPos+27, dwXPos+64+300, dwYPos+27+16 ) );
    macroListSelection.SetSpriteId( "GUI_BackMacroSelect" );
    macroListSelection.Show(); 
    
    macroList.SetPos( FramePos( dwXPos+8, dwYPos+50 , dwXPos+409, dwYPos+323 ) );
    macroList.SetupList( 3, 37, FramePos( 0, 0, 17, 9 ) );
    macroList.SetColumnSize( 0, 64 );
    macroList.SetColumnSize( 1, 160 );
    macroList.SetColumnSize( 2, 106 );
    macroList.CenterColumn( 0 );
    macroList.CenterColumn( 1 ); 
    macroList.CenterColumn( 2 );
    macroListEvent.SetUI( this );
    macroList.SetSelectionSprite( &macroListSelection, 69, 10 );
    macroList.SetScrollRegions(
       FramePos( dwXPos+382, dwYPos+41 , dwXPos+402, dwYPos+60 ),
       FramePos( dwXPos+382, dwYPos+282, dwXPos+402, dwYPos+303 ),
       FramePos( dwXPos+382, dwYPos+61 , dwXPos+402, dwYPos+270 )
       );
    
    macroList.SetScrollDownDownUI( &scrollDownGraph );
    macroList.SetScrollUpDownUI  ( &scrollUpGraph );
    
    
    
    SetHelpId( macroList, 378 );
    
    
    
    macroGrid[ 0 ].SetPos( FramePos( dwXPos+423, dwYPos+216, dwXPos+466, dwYPos+258 ) );
    macroGrid[ 1 ].SetPos( FramePos( dwXPos+469, dwYPos+216, dwXPos+512, dwYPos+258 ) );
    macroGrid[ 2 ].SetPos( FramePos( dwXPos+514, dwYPos+216, dwXPos+557, dwYPos+258 ) );
    macroGrid[ 3 ].SetPos( FramePos( dwXPos+423, dwYPos+277, dwXPos+466, dwYPos+319 ) );
    macroGrid[ 4 ].SetPos( FramePos( dwXPos+469, dwYPos+277, dwXPos+512, dwYPos+319 ) );
    macroGrid[ 5 ].SetPos( FramePos( dwXPos+514, dwYPos+277, dwXPos+557, dwYPos+319 ) );
    
    UINT z;
    for( z = 0; z < macroGrid.size(); z++ )
    {
       SetHelpId( macroGrid[ z ], 386 );
       macroGrid[z].SetSpriteId( "GUI_MacroUp" );
       macroGrid[z].macroItemHighlight.SetSpriteId( "GUI_MacroHUp" );
       macroGrid[z].macroItemHighlight.SetPos( macroGrid[z].GetPos() );
       macroGrid[z].Show();
    }

   addSpellButton.SetPos( FramePos( dwXPos+420, dwYPos+40, dwXPos+459, dwYPos+79 ) );
   addSpellButtonEvent.SetUI( this );
   addSpellButton.SetSpriteId   ( "GUI_ChatBtnSpellUp" );
   addSpellButton.SetDownUI     ( "GUI_ChatBtnSpellDown" );
   addSpellButton.SetHighlightUI( "GUI_ChatBtnSpellHUp" );
   SetHelpId( addSpellButton, 380 );

   addSkillButton.SetPos( FramePos( dwXPos+519, dwYPos+40, dwXPos+558, dwYPos+79 ) );
   addSkillButtonEvent.SetUI( this );
   addSkillButton.SetSpriteId   ( "GUI_ChatBtnCharCheetUp" );
   addSkillButton.SetDownUI     ( "GUI_ChatBtnCharCheetDown" );
   addSkillButton.SetHighlightUI( "GUI_ChatBtnCharCheetHUp" );
   SetHelpId( addSkillButton, 381 );

   addItemButton.SetPos( FramePos( dwXPos+469, dwYPos+40, dwXPos+508, dwYPos+79 ) );
   addItemButtonEvent.SetUI( this );
   addItemButton.SetSpriteId   ( "GUI_ChatBtnBackPackUp" );
   addItemButton.SetDownUI     ( "GUI_ChatBtnBackPackDown" );
   addItemButton.SetHighlightUI( "GUI_ChatBtnBackPackHUp" );
   SetHelpId( addItemButton, 382 );

   removeMacroButton.SetPos( FramePos( dwXPos+459, dwYPos+90, dwXPos+518, dwYPos+121 ) );
   removeMacroButton.SetSpriteId   ( "GUI_ButtonUp"   );
   removeMacroButton.SetDownUI     ( "GUI_ButtonDown" );
   removeMacroButton.SetHighlightUI( "GUI_ButtonHUp"  );
   removeMacroButtonEvent.SetUI( this );
   SetHelpId( removeMacroButton, 383 );

   addToGridButton.SetPos( FramePos( dwXPos+446, dwYPos+146, dwXPos+485, dwYPos+185 ) );
   addToGridButton.SetSpriteId   ( "GUI_MacroBtnAddUp" );
   addToGridButton.SetDownUI     ( "GUI_MacroBtnAddDown" );
   addToGridButton.SetHighlightUI( "GUI_MacroBtnAddHUp" );
   addToGridButtonEvent.SetUI( this );
   SetHelpId( addToGridButton, 384 );
   
   removeFromGridButton.SetPos( FramePos( dwXPos+494, dwYPos+146, dwXPos+533, dwYPos+185 ) );
   removeFromGridButton.SetSpriteId   ( "GUI_MacroBtnDelUp" );
   removeFromGridButton.SetDownUI     ( "GUI_MacroBtnDelDown" );
   removeFromGridButton.SetHighlightUI( "GUI_MacroBtnDelHUp" );
   removeFromGridButtonEvent.SetUI( this );
   SetHelpId( removeFromGridButton, 385 );
   
   AddChild( &backMacro );
   AddChild( &macroList );
   AddChild( &addSpellButton );
   AddChild( &addSkillButton );
   AddChild( &addItemButton );
   AddChild( &removeMacroButton );
   AddChild( &addToGridButton );
   AddChild( &removeFromGridButton );
   AddChild( &X_Button );

   Minimize(true);

   RootBoxUI::GetInstance()->AddChild( this );
}


//////////////////////////////////////////////////////////////////////////////////////////
MacroUI *MacroUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the macro UI's instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static MacroUI instance;
    return &instance;
}


//////////////////////////////////////////////////////////////////////////////////////////
unsigned int MacroUI::Display
//////////////////////////////////////////////////////////////////////////////////////////
//  Callback which displays the macro box.
// 
(
 void *null
)
//////////////////////////////////////////////////////////////////////////////////////////
{
   // Lock the root box since this call came from some unknown thread.
   CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );
   
   if( GetInstance()->IsShown() && !GetInstance()->IsMinimized() )
   {
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

void MacroUI::Hide()
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
   //:CR_NMNMNM_SAVE 2006-10-10 //Add save on minimuze satte of GUI...
   g_SaveGame.bSave();
	BoxUI::Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the Macri UI.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::Draw(V2SPRITEFX *vsfFX)
{
   if(IsMinimized())
   {
      GameUI::Draw( vsfFX );
      return;
   }

   int dwXPos = (g_Global.GetScreenW()-576)/2;
   int dwYPos = (g_Global.GetScreenH()-150-368)/2;
   if(dwYPos <0)
       dwYPos =0;
    
   GameUI::Draw( vsfFX );
   HDC hdc;
   DXDGetDC(&hdc, 680);

   // Draw les texte...
   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[14]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 459, dwYPos+92+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[34]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,102 )+ 117  , dwYPos+42   , g_Global.GetBtnTCE(), 0, strBtnTexte);
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[35]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,71 )+ 283  , dwYPos+42   , g_Global.GetBtnTCE(), 0, strBtnTexte);
   

   // Draw Box Header
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[16]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 237+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 237  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);


   DXDReleaseDC(hdc, 680);
}

int MacroUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}


//////////////////////////////////////////////////////////////////////////////////////////
//  Draws only the minimized controls.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::DrawMinimized(V2SPRITEFX *vsfFX )
{
    if(g_SaveGame.GetOptionsParam()->bDisplayMacroFullScreen && !ChatterUI::GetInstance()->IsShown())
    {
       int i; 
       for( i = 0; i < MacroGridSize; i++ )
       {
           if( macroGrid[ i ].macroItem != NULL )
           {
               macroGrid[ i ].Draw( vsfFX );
           }
       }
    }
    else
    {
       //Minimize(false);
       //Hide();
    }

}

//////////////////////////////////////////////////////////////////////////////////////////
// Return: virtual void, 
//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::Minimize(bool newMinimizedState)
{
    GameUI::Minimize( newMinimizedState );

    int dwXPos = (g_Global.GetScreenW()-576)/2;
    int dwYPos = (g_Global.GetScreenH()-150-368)/2;
    if(dwYPos <0)
       dwYPos =0;
    if( newMinimizedState == false )
    {
        SetPos( FramePos( dwXPos, dwYPos, dwXPos+576, dwYPos+368 ) );

        macroGrid[ 0 ].SetPos( FramePos( dwXPos+423, dwYPos+216, dwXPos+466, dwYPos+258 ) );
        macroGrid[ 1 ].SetPos( FramePos( dwXPos+469, dwYPos+216, dwXPos+512, dwYPos+258 ) );
        macroGrid[ 2 ].SetPos( FramePos( dwXPos+514, dwYPos+216, dwXPos+557, dwYPos+258 ) );
        macroGrid[ 3 ].SetPos( FramePos( dwXPos+423, dwYPos+277, dwXPos+466, dwYPos+319 ) );
        macroGrid[ 4 ].SetPos( FramePos( dwXPos+469, dwYPos+277, dwXPos+512, dwYPos+319 ) );
        macroGrid[ 5 ].SetPos( FramePos( dwXPos+514, dwYPos+277, dwXPos+557, dwYPos+319 ) );
    }
    else
    {
        SetPos( FramePos( g_Global.GetScreenW()-285, g_Global.GetScreenH()-60, g_Global.GetScreenW(), g_Global.GetScreenH()-20 ) );

        macroGrid[ 0 ].SetPos( FramePos( g_Global.GetScreenW()-275, g_Global.GetScreenH()-60,   g_Global.GetScreenW()-232, g_Global.GetScreenH()-18 ) );
        macroGrid[ 1 ].SetPos( FramePos( g_Global.GetScreenW()-229, g_Global.GetScreenH()-60,   g_Global.GetScreenW()-186, g_Global.GetScreenH()-18 ) );
        macroGrid[ 2 ].SetPos( FramePos( g_Global.GetScreenW()-183, g_Global.GetScreenH()-60,   g_Global.GetScreenW()-140, g_Global.GetScreenH()-18 ) );
        macroGrid[ 3 ].SetPos( FramePos( g_Global.GetScreenW()-137, g_Global.GetScreenH()-60,   g_Global.GetScreenW()-94, g_Global.GetScreenH()-18 ) );
        macroGrid[ 4 ].SetPos( FramePos( g_Global.GetScreenW()-91 , g_Global.GetScreenH()-60,   g_Global.GetScreenW()-48 , g_Global.GetScreenH()-18 ) );
        macroGrid[ 5 ].SetPos( FramePos( g_Global.GetScreenW()-45 , g_Global.GetScreenH()-60,   g_Global.GetScreenW()-2 , g_Global.GetScreenH()-18 ) );
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
MacroUI::MacroItemUI::MacroItemUI
//////////////////////////////////////////////////////////////////////////////////////////
(
 MacroEvent *imacroEvent, // The template from which to get a macro event.
 MacroDataUI *imacroUI,
 string imacroName
)
: macroUI( imacroUI ), macroName( imacroName ), macroEvent( imacroEvent )
//////////////////////////////////////////////////////////////////////////////////////////
{
    static int globalMacroId = 1;

    macroId = globalMacroId++;
}

// Destructor ////////////////////////////////////////////////////////////////////////////
 MacroUI::MacroItemUI::~MacroItemUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::AddMacro
//////////////////////////////////////////////////////////////////////////////////////////
//  Adds a macro to the macro UI.
// 
(
 MacroDataUI *macroUI,  // The UI displayed by the macro UI. z
 string       macroName,  // The display name of the macro.
 MacroEvent *macroEvent, // The macro event visitor.
 bool echo  // true if text is displayed in the chat UI.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Search the macro list for the macro.
    vector< vector< ListUI::GridItem > > &grid = *macroList.GetGrid();

    // Register this event.
    eventSet.insert( macroEvent );
    
    bool exists = false;
    vector< ListUI::GridItem >::iterator i;
    for( i = grid[0].begin(); i != grid[0].end(); i++ ){
        MacroItemUI *macro = static_cast< MacroItemUI * >( *i );
        // If the macro name and the specified event are the same (coming
        // from the same source).
        if( macro->macroName == macroName &&
            typeid( *macro->macroEvent ) == typeid( *macroEvent ) ){
            exists = true;
            break;
        }
    }

    char str[ 256 ];

    if( exists ){
        if( echo ){
            naSound->Play( TRUE );
            sprintf( str, g_LocalString[ 355 ], macroName.c_str() );
            ChatterUI::GetInstance()->AddBackscrollItem( "", str, SYSTEM_COLOR, false );
        }
        return;
    }

    if( echo ){
        sprintf( str, g_LocalString[ 356 ], macroName.c_str() );
        ChatterUI::GetInstance()->AddBackscrollItem( "", str, SYSTEM_COLOR, false );
    }

    // Macro item UI's
    MacroItemUI *macroItemUI = new MacroItemUI( macroEvent, macroUI, macroName );
    macroItemUI->SetPos( macroUI->GetPos() );

    macroItemUI->SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, macroName.c_str() ) );
    macroList.AddNewItem();
    macroList.AddColumnItem( 0, macroItemUI );

    if( macroUI->gridPos != 0 )
    {
        AddToGrid( macroItemUI,macroUI->gridPos );
    }

    StaticTextUI *macroText = new StaticTextUI;
    macroText->SetText( macroName.c_str() );
    macroText->AutoTruncate( MacroNameSize );
    macroText->SetPos( FramePos( 0, 0, macroText->GetHeight(), macroText->GetWidth() ) );    
    macroList.AddColumnItem( 1, macroText );


    

    macroText = new StaticTextUI;
    macroText->SetText( macroUI->vKey.GetName() );
    macroText->SetPos( FramePos( 0, 0, macroText->GetHeight(), macroText->GetWidth() ) );
    macroList.AddColumnItem( 2, macroText );
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::ClientShutdown( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Tell all macro owners to save them
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SaveMacros();

}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::SaveMacros( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Save all macros.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    typedef vector< vector< ListUI::GridItem > > Grid;
    
    Grid *gridList = macroList.GetGrid();

    LOG.Lock();
    // For each event
    set< MacroEvent * >::iterator z;
    for( z = eventSet.begin(); z != eventSet.end(); z++ ){
        LOG << "\r\nNew event.";
        MacroEvent *macroEvent = *z;
        list< MacroDataUI * > macroList;

        UINT i;
        // Scan the list of macros.
        for( i = 0; i != (*gridList)[0].size(); i++ ){
            MacroItemUI *macroItemUI = static_cast< MacroItemUI * >( (*gridList)[ 0 ][ i ] );

            // If the macro is owned by this macro event
            if( typeid( *macroItemUI->macroEvent ) == typeid( *macroEvent ) ){
                LOG << "\r\nAdding saving " << (char*)macroItemUI->macroName.c_str() << ".!";
                
                // Add it to the list.
                macroList.push_back( macroItemUI->macroUI );
            }
        }
        // Save all macros owned by this event
        macroEvent->SaveMacro( macroList );
    }
    LOG.Unlock();
}


//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::AddSpellButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Popup the spell book
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SpellUI::GetInstance()->Display( NULL );
}
//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::AddSkillButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Popup the character sheet
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CharacterUI::GetInstance()->Display( NULL );
}
//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::AddItemButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Popup the inventory
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    InventoryUI::GetInstance()->Display( NULL );
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::MacroListEvent::LeftDblClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Double-clicked on the macro lists.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    MacroUI *macroUI = static_cast< MacroUI * >( GetUI() );

    MacroItemUI *macroItem = static_cast< MacroItemUI * >( macroUI->macroList.GetSelectedRowItem( 0 ) );

    // Show the join popup box.
    macroUI->macroPopup.Show();

    macroUI->macroPopup.SetKey( macroItem->macroUI->vKey );

    macroUI->macroPopup.macroId = macroItem->macroId;
    macroUI->macroPopup.parentUI = macroUI;

    // Disable macro calls to catch CTRL- messages.
    Custom.gMacro.DisableMacroCall();
    macroUI->DisableMacro( true );

    // Put the join popup in foreground.
    macroUI->RequestForegroundControl( &macroUI->macroPopup );
}

//////////////////////////////////////////////////////////////////////////////////////////
MacroUI::MacroPopupUI::MacroPopupUI( void ) :
    clearButton(  clearButtonEvent ),
    cancelButton( cancelButtonEvent ),
    acceptButton( acceptButtonEvent )
//////////////////////////////////////////////////////////////////////////////////////////
{
    int dwOffX = (g_Global.GetScreenW()-240)/2;
    int dwOffY = (g_Global.GetScreenH()-164-150)/2;

    SetSpriteId( "GUI_PopupBack" );
    SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );
    intlText.SetSpriteId( "GUI_MacroPopupBack" );
    intlText.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );


    clearButton.SetPos        ( FramePos( dwOffX+17, dwOffY+94, dwOffX+17+60, dwOffY+94+32 ) );
    clearButton.SetSpriteId   ( "GUI_ButtonUp" );
    clearButton.SetDownUI     ( "GUI_ButtonDown" );
    clearButton.SetHighlightUI( "GUI_ButtonHUp" );
    clearButtonEvent.SetUI( this );
    SetHelpId( clearButton, 466 );
    
    cancelButton.SetPos( FramePos( dwOffX+161, dwOffY+94, dwOffX+89+161, dwOffY+94+32 ) );
    cancelButton.SetSpriteId   ( "GUI_ButtonUp" );
    cancelButton.SetDownUI     ( "GUI_ButtonDown" );
    cancelButton.SetHighlightUI( "GUI_ButtonHUp" );
    cancelButtonEvent.SetUI( this );    
    SetHelpId( cancelButton, 468 );
    
    
    acceptButton.SetPos( FramePos( dwOffX+89, dwOffY+94, dwOffX+89+60, dwOffY+94+32 ) );
    acceptButton.SetSpriteId     ( "GUI_ButtonUp" );
    acceptButton.SetDownUI       ( "GUI_ButtonDown" );
    acceptButton.SetHighlightUI  ( "GUI_ButtonHUp" );
    acceptButtonEvent.SetUI( this );
    SetHelpId( acceptButton, 467 );

    keyText.SetPos( FramePos( dwOffX+93, dwOffY+33, dwOffX+219, dwOffY+62 ) );
    SetHelpId( keyText, 465 );


    AddChild( &intlText );
    AddChild( &clearButton );
    AddChild( &cancelButton );
    AddChild( &acceptButton );
    AddChild( &keyText );
    

    RootBoxUI::GetInstance()->AddChild( this );
}

void MacroUI::MacroPopupUI::Draw(V2SPRITEFX *vsfFX)
{
   int dwOffX = (g_Global.GetScreenW()-240)/2;
   int dwOffY = (g_Global.GetScreenH()-164-150)/2;

   GameUI::Draw( vsfFX );
    
   HDC hdc;
   DXDGetDC(&hdc, 681);


   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[97]);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 17+1, dwOffY+96+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[10]);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 89+1, dwOffY+96+2 , RGB(0, 0, 0)    , 0, strBtnTexte);
   
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[11]);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 161+1, dwOffY+96+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[98]);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,70)+ 20+1, dwOffY+35+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,70)+ 20  , dwOffY+35   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   
   DXDReleaseDC(hdc, 681);
}

int MacroUI::MacroPopupUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
bool MacroUI::MacroPopupUI::SetKey
//////////////////////////////////////////////////////////////////////////////////////////
//  Resets the join popup box.
// 
(
 VKey vParamKey
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    keyText.SetText( vParamKey.GetName() );
    vKey = vParamKey;
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
VKey MacroUI::MacroPopupUI::GetKey( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the key.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    return vKey;
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::MacroPopupUI::CancelButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Cancel the current box
// 
//////////////////////////////////////////////////////////////////////////////////////////
{   
    MacroPopupUI *ui = static_cast< MacroUI::MacroPopupUI * >( GetUI() );

    Custom.gMacro.EnableMacroCall();
    
    ui->parentUI->DisableMacro( false );

    ui->Hide();    
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::MacroPopupUI::AcceptButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Accept the new key.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    MacroPopupUI *ui = static_cast< MacroUI::MacroPopupUI * >( GetUI() );

    // Search the corresponding macro in the macro list.
    vector< vector< ListUI::GridItem > > &macroGrid = *ui->parentUI->macroList.GetGrid();
    
    UINT row;
    for( row = 0; row != macroGrid[ 0 ].size(); row++ ){
        MacroItemUI *macroItem = static_cast< MacroItemUI * >( macroGrid[ 0 ][ row ] );

        if( macroItem->macroId == ui->macroId ){
            break;
        }
    }

    // if the macroid was found before reaching the end of the list.
    if( row != macroGrid[ 0 ].size() ){
        MacroItemUI *macroItem = static_cast< MacroItemUI * >( macroGrid[ 0 ][ row ] );
        macroItem->macroUI->vKey = ui->vKey;
        
        StaticTextUI *keyName = static_cast< StaticTextUI * >( macroGrid[ 2 ][ row ] );        
        keyName->SetText( ui->keyText.GetText()->GetText() );
    }
    
    Custom.gMacro.EnableMacroCall();
    ui->parentUI->DisableMacro( false );
    ui->parentUI->SaveMacros();

    ui->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::MacroPopupUI::ClearButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Clear (unassign) a key.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    MacroPopupUI *ui = static_cast< MacroUI::MacroPopupUI * >( GetUI() );
    
    ui->SetKey( 0 );
}

//////////////////////////////////////////////////////////////////////////////////////////
bool MacroUI::MacroPopupUI::VKeyInput
//////////////////////////////////////////////////////////////////////////////////////////
//  Keyboard hit
// 
(
 VKey ivKey
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    bool isValid = false;
    if( ivKey.CTRL() ){
        isValid = IsKeyValidWithCtrl( ivKey.GetKey() );
    }else{
        isValid = IsKeyValidWithShiftOrNothing( ivKey.GetKey() );
    }

	if (!ivKey.CTRL() ) {
		if (ivKey == VK_ESCAPE) {
			if (GetKey() == 0) {
				cancelButtonEvent.LeftClicked();
				return true;
			} else {
				SetKey(0);
				return true;
			}
		} else if (ivKey == VK_RETURN) {
			acceptButtonEvent.LeftClicked();
			return true;
		}
	}

    if( !isValid ){
        naSound->Play( TRUE );
        ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 357 ], SYSTEM_COLOR, false );
        return true;
    }
    
    // If a custom macro of macroUI macro already exist.
    if( Custom.gMacro.MacroExists( ivKey ) || parentUI->KeyExists( ivKey ) ){
        naSound->Play( TRUE );
        ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 358 ], SYSTEM_COLOR, false );
        return true;
    }

    keyText.SetText( ivKey.GetName() );
    vKey = ivKey;

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool MacroUI::KeyExists
//////////////////////////////////////////////////////////////////////////////////////////
//  Searches the macro database for existing keys.
// 
(
 VKey ivKey // The key to check for
)
// Return: bool, true if the key exists.
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Lock the root box since this call came from some unknown thread.
    CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );
 
     // Search the corresponding macro in the macro list.
    vector< vector< ListUI::GridItem > > &macroGrid = *macroList.GetGrid();
    
    UINT row;
    for( row = 0; row != macroGrid[ 0 ].size(); row++ ){
        MacroItemUI *macroItem = static_cast< MacroItemUI * >( macroGrid[ 0 ][ row ] );

        if( macroItem->macroUI->vKey == ivKey ){
            return true;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool MacroUI::CallMacro
//////////////////////////////////////////////////////////////////////////////////////////
// Calls a macro.
// 
(
 VKey vKey // The pressed key.
)
// Return: bool, true if a macro was executed
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Lock the root box since this call came from some unknown thread.
    CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );
 
    if( disableMacro ){
        return false;
    }

     // Search the corresponding macro in the macro list.
    vector< vector< ListUI::GridItem > > &macroGrid = *macroList.GetGrid();
    
    UINT row;
    for( row = 0; row != macroGrid[ 0 ].size(); row++ ){
        MacroItemUI *macroItem = static_cast< MacroItemUI * >( macroGrid[ 0 ][ row ] );

        if( macroItem->macroUI->vKey == vKey ){
            break;
        }
    }

    // if the macroid was found before reaching the end of the list.
    if( row != macroGrid[ 0 ].size() ){
        MacroItemUI *macroItem = static_cast< MacroItemUI * >( macroGrid[ 0 ][ row ] );

        LOG.Lock();
        LOG << "\r\nCALLED MACRO " << (char*)macroItem->macroName.c_str() << "..!";
        LOG.Unlock();
        
        // Call the macro visitor
        macroItem->macroEvent->ExecMacro( macroItem->macroUI );

        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::RemoveMacroButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Removing a macro
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    MacroUI *ui = static_cast< MacroUI * >( GetUI() );


    MacroItemUI *macroItem = static_cast< MacroItemUI *>( ui->macroList.GetSelection() );

    if( macroItem == NULL ){
        naSound->Play( TRUE );
        ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 359 ], SYSTEM_COLOR, false );
        return;
    }

    // Try to find the macro in the grid.
    ui->RemoveFromGrid( macroItem );

    ui->macroList.DeleteSelection();
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::AddToGridButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Adding the selected macro to the macro grid.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    MacroUI *ui = static_cast< MacroUI * >( GetUI() );
    
    // Get the selected UI.
    MacroItemUI *macroItem = static_cast< MacroItemUI * >( ui->macroList.GetSelection() );

    if( macroItem == NULL ){
        naSound->Play( TRUE );
                               
        return;
    }

    LOG.Lock();
    LOG << "\r\nAdding macro " << (char*)macroItem->macroName.c_str() << "..";
    LOG.Unlock();

    ui->AddToGrid( macroItem );

    ui->SaveMacros();
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::RemoveFromGridButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Removing the selected macro from the macro grid.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    MacroUI *ui = static_cast< MacroUI * >( GetUI() );

    if( ui->selectedGridItem != NULL ){
        ui->RemoveFromGrid( ui->selectedGridItem );
    }else{
        naSound->Play( TRUE );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Adds a macroItem to the grid.
// 
//////////////////////////////////////////////////////////////////////////////////////////
bool MacroUI::AddToGrid(MacroItemUI *macroItem ,int dwPos)
{
    if( gridSize >= MacroGridSize )
    {
        naSound->Play( TRUE );
        ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 360 ], SYSTEM_COLOR, false );
        return false;
    }

    // verify is this macro already exist
    int i;
    for( i = 0; i < MacroGridSize; i++ )
    {
        if( macroGrid[ i ].macroItem == macroItem )
        {
            naSound->Play( TRUE );
            ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 361 ], SYSTEM_COLOR, false );
            return false;
        }
    }
    if(dwPos == 0)
    {
       // Find an empty spot.
       for( i = 0; i < MacroGridSize; i++ )
       {
           if( macroGrid[ i ].macroItem == NULL )
           {
               macroGrid[ i ].macroItem = macroItem;
               macroGrid[ i ].SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, macroItem->macroName.c_str() ) );
            
               macroItem->macroUI->gridPos = i+1;
            
               gridSize++;
               // Add the child for events.
               AddChild( &macroGrid[ i ] );

               ChatterUI::GetInstance()->UpdateMacroHelpID();
               return true;
           }
       }
    }
    else
    {
       // on ajoute la macro a une position fixe...
       dwPos--;
       if( macroGrid[ dwPos ].macroItem == NULL )
       {
          macroGrid[ dwPos ].macroItem = macroItem;
          macroGrid[ dwPos ].SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, macroItem->macroName.c_str() ) );
          
          //macroItem->macroUI->gridPos = i+1;
          
          //gridSize++;
          // Add the child for events.
          AddChild( &macroGrid[ dwPos ] );
          ChatterUI::GetInstance()->UpdateMacroHelpID();
          return true;
       }
    
    }
    naSound->Play( TRUE );
    return false;
}



//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::ResetMacro( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Removes all macros.
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );
    g_SaveGame.bFreeMacroItem();
    g_SaveGame.bFreeMacroSpell();
    g_SaveGame.bFreeMacroSkill();

    int i;
    for( i = 0; i < MacroGridSize; i++ )
    {
       macroGrid[ i ].macroItem = NULL;
       macroGrid[ i ].SetHelpText( NULL );
       selectedGridItem = NULL;

       RemoveChild( &macroGrid[ i ] );
       ChatterUI::GetInstance()->UpdateMacroHelpID();
    }
    gridSize = 0;
    macroList.DeleteList();
    InventoryUI::GetInstance()->SetDefaultMacro();
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::RemoveFromGrid
//////////////////////////////////////////////////////////////////////////////////////////
//  Removes the macro item from the macro grid.
// 
(
 MacroItemUI *macroItem
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Search the macro item in the grid.
    int i;
    for( i = 0; i < MacroGridSize; i++ )
    {
        if( macroGrid[ i ].macroItem == macroItem )
        {
            macroItem->macroUI->gridPos = 0;
            macroGrid[ i ].macroItem = NULL;
            macroGrid[ i ].SetHelpText( NULL );

            if( selectedGridItem == macroItem ){
                selectedGridItem = NULL;
            }

            gridSize--;
            
            RemoveChild( &macroGrid[ i ] );
            ChatterUI::GetInstance()->UpdateMacroHelpID();
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
 MacroUI::MacroGridItemUI::MacroGridItemUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
 : macroItem( NULL )
{
}

//////////////////////////////////////////////////////////////////////////////////////////
void MacroUI::MacroGridItemUI::Draw
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the macro grid item.
// 
(
 V2SPRITEFX *fx
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Draw the underlying macroUI using the grid item's position.
    if( macroItem != NULL && macroItem->macroUI != NULL )
    {
        if( MacroUI::GetInstance()->selectedGridItem == macroItem && !MacroUI::GetInstance()->IsMinimized())
        {
            macroItemHighlight.Draw( NULL );
        }
        else
        {        
            GameUI::Draw( NULL );
        }
        FramePos itemPos = macroItem->macroUI->GetPos();
        FramePos thisPos = GetPos();
        itemPos.SetNewOffset( 
            MousePos( 
                thisPos.x - itemPos.Width() / 2 
                  + thisPos.Width() / 2,
                thisPos.y - itemPos.Height() / 2
                  + thisPos.Height() / 2
            )
        );
        macroItem->macroUI->SetPos( itemPos );
        macroItem->macroUI->Draw( fx );
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
bool MacroUI::MacroGridItemUI::LeftMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
//  Mouse up on a macro grid item
// 
(
 MousePos mousePos // The mouse position
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( macroItem == NULL ){
        return false;
    }
    
    MacroUI *ui = MacroUI::GetInstance();

    ui->selectedGridItem = macroItem;

    if( IsDblClick() )
    {
        // Call the macro visitor
        macroItem->macroEvent->ExecMacro( macroItem->macroUI );
    }    
    
    return true;
}
