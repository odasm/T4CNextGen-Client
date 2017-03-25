// CharacterUI.cpp
//
#pragma warning( disable : 4291 )

#include "../pch.h"

#include "RootBoxUI.h"
#include "CharacterUI.h"
#include "../Bitmap.h"
#include "../MemoryManager.h"
#include "ChatterUI.h"
#include "InventoryUI.h"
#include "../tileset.h"
#include "../app.h"
#include "..\SaveGame.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]


extern Bitmap GridID;
extern DWORD TargetID;
extern Font *fSkillFont;
extern Global g_Global;
extern CSaveGame g_SaveGame;
extern Font *fNewGuiBtnFont;
extern Font *fNewGuiFont;
                 
const int STRid = 1;
const int ENDid = 2;
const int AGIid = 3;
const int WILid = 4;
const int INTid = 5;
const int WISid = 6;
const int LCKid = 7;

namespace{
    T3VSBSound *naSound = NULL;

    void InitSound(){
        if( naSound == NULL ){
            naSound = new T3VSBSound;
            naSound->Create( "General NA sound", TS_MEMORY );
        }
    }


    enum TargetPriority{
        monsterPriority = 0,
        equalPriority   = 1,
        playerPriority  = 2
    };
};

//////////////////////////////////////////////////////////////////////////////////////////
CharacterUI::CharacterUI( void )
:    
    skillList( skillEvent ),
    usedStatPts( 0 ),
    upgSTR( 0 ),
    upgEND( 0 ),
    upgAGI( 0 ),
    upgWIS( 0 ),
    upgINT( 0 ),
    upgSTRbutton( upgSTRevent ),
    upgENDbutton( upgENDevent ),
    upgAGIbutton( upgAGIevent ),
    upgWISbutton( upgWISevent ),
    upgINTbutton( upgINTevent ),
    dngSTRbutton( dngSTRevent ),
    dngENDbutton( dngENDevent ),
    dngAGIbutton( dngAGIevent ),
    dngWISbutton( dngWISevent ),
    dngINTbutton( dngINTevent ),
    iconSTR( EmptyEvent::GetInstance() ),
    iconEND( EmptyEvent::GetInstance() ),
    iconAGI( EmptyEvent::GetInstance() ),
    iconINT( EmptyEvent::GetInstance() ),
    iconWIS( EmptyEvent::GetInstance() ),
	iconLuck( EmptyEvent::GetInstance() ),
    iconAC(  EmptyEvent::GetInstance() ),
    iconLevel( EmptyEvent::GetInstance() ),
    iconHP( EmptyEvent::GetInstance() ),
    iconMP( EmptyEvent::GetInstance() ),
    iconWeight( EmptyEvent::GetInstance() ),
    statsCommitButton( statsCommitEvent ),
    macroButton( macroButtonEvent ),
    X_Button( hideEvent )

//////////////////////////////////////////////////////////////////////////////////////////
{
    InitSound();

    int dwXPos = (g_Global.GetScreenW()-576)/2;
    int dwYPos = (g_Global.GetScreenH()-150-368)/2;
    if(dwYPos <0)
       dwYPos =0;
    
    SetPos( FramePos( dwXPos, dwYPos, dwXPos+576, dwYPos+368 ) );
    SetHelpId( this, 297 );
    
    backStat.SetSpriteId( "GUI_BackStat" );
    backStat.SetPos( FramePos( dwXPos, dwYPos, dwXPos+576, dwYPos+368 ) );
    backStat.DrawAlpha(true);

    X_Button.SetPos        ( FramePos( dwXPos+576-28, dwYPos+0, dwXPos+576-4, dwYPos+0+24 ) );
    X_Button.SetSpriteId   ( "GUI_X_ButtonUp" );
    X_Button.SetDownUI     ( "GUI_X_ButtonDown" );
    X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
    SetHelpId( X_Button, 261 );


    // Setup the events.
    skillEvent.SetUI( this );

    // Setup the stat positions.
    statSTR.SetPos ( FramePos( dwXPos+52, dwYPos+51 , dwXPos+139, dwYPos+65  ) );
    statEND.SetPos ( FramePos( dwXPos+52, dwYPos+94 , dwXPos+139, dwYPos+108 ) );
    statAGI.SetPos ( FramePos( dwXPos+52, dwYPos+137, dwXPos+139, dwYPos+151 ) );
    statWIS.SetPos ( FramePos( dwXPos+52, dwYPos+180, dwXPos+139, dwYPos+194 ) );
    statINT.SetPos ( FramePos( dwXPos+52, dwYPos+223, dwXPos+139, dwYPos+237 ) );
	statLCK.SetPos ( FramePos( dwXPos+203, dwYPos+94 , dwXPos+290, dwYPos+108 ) );
    statAC.SetPos  ( FramePos( dwXPos+220,dwYPos+53 , dwXPos+247, dwYPos+64  ) );
  
    statHP.SetPos ( FramePos( dwXPos+203, dwYPos+137 , dwXPos+290, dwYPos+151 ) );
    statMP.SetPos ( FramePos( dwXPos+203, dwYPos+180 , dwXPos+290, dwYPos+194 ) );
    weight.SetPos ( FramePos( dwXPos+203, dwYPos+223 , dwXPos+290, dwYPos+237 ) );
	//statLCK.SetPos( FramePos( dwXPos+203, dwYPos+266,  dwXPos+290, dwYPos+237 ) );

    statPnts.SetPos ( FramePos( dwXPos+334, dwYPos+171 , dwXPos+396, dwYPos+182 ) );
    skillPnts.SetPos( FramePos( dwXPos+334, dwYPos+120 , dwXPos+396, dwYPos+130 ) );
    SetHelpId( statPnts, 288 );
    SetHelpId( skillPnts, 289 );
    
    statXP.SetPos   ( FramePos( dwXPos+39, dwYPos+301 , dwXPos+210, dwYPos+312 ) );
    SetHelpId( statXP, 292 );

    statXP2GO.SetPos( FramePos( dwXPos+233, dwYPos+301 , dwXPos+404, dwYPos+312 ) );
    SetHelpId( statXP2GO, 293 );

    karma.SetPos    ( FramePos( dwXPos+426, dwYPos+301 , dwXPos+535, dwYPos+312 ) );
    SetHelpId( karma, 294 );

    statLevel.SetPos( FramePos( dwXPos+258, dwYPos+53 , dwXPos+283, dwYPos+64 ) );
    statLevel.SetCenter(true);

    // Setup the skill list.
    skillList.SetPos       ( FramePos( dwXPos+413, dwYPos+58 , dwXPos+548, dwYPos+230 ) );
    skillList.SetupList    ( 2, 36, FramePos( 0, 0, 7, 8 ) );
    skillList.SetColumnSize( 0, 42 );
    skillList.SetColumnSize( 1, 46 );
    skillList.CenterColumn( 0 );
    skillList.CenterColumn( 1 );
    skillList.SetScrollRegions( 
        FramePos( dwXPos+521, dwYPos+54 , dwXPos+548, dwYPos+76  ), 
        FramePos( dwXPos+521, dwYPos+207, dwXPos+548, dwYPos+230 ), 
        FramePos( dwXPos+521, dwYPos+77 , dwXPos+548, dwYPos+195 ) 
    );
    skillScrollUp.SetPos  ( FramePos( dwXPos+521, dwYPos+54 , dwXPos+548, dwYPos+76 ) );
    skillScrollDown.SetPos( FramePos( dwXPos+521, dwYPos+207, dwXPos+548, dwYPos+230 ) );
    skillList.SetScrollUpDownUI( &skillScrollUp );
    skillList.SetScrollDownDownUI( &skillScrollDown );

    skillSelectionGraphPatch.SetPos( FramePos( 0, 0, 46, 17 ) );
    skillSelectionGraphPatch.SetSpriteId( "GUI_BackStatSelect" );
    skillSelectionGraphPatch.Show();
    skillList.SetSelectionSprite( &skillSelectionGraphPatch, 47, 10 );

    SetHelpId( skillList, 296 );

    // Setup the upgrade events.
    upgSTRevent.stat = STRid;
    upgENDevent.stat = ENDid;
    upgAGIevent.stat = AGIid;
    upgWISevent.stat = WISid;
    upgINTevent.stat = INTid;
    dngSTRevent.stat = STRid;
    dngENDevent.stat = ENDid;
    dngAGIevent.stat = AGIid;
    dngWISevent.stat = WISid;
    dngINTevent.stat = INTid;

    upgSTRevent.SetUI( this );
    upgENDevent.SetUI( this );
    upgAGIevent.SetUI( this );
    upgWISevent.SetUI( this );
    upgINTevent.SetUI( this );
    dngSTRevent.SetUI( this );
    dngENDevent.SetUI( this );
    dngAGIevent.SetUI( this );
    dngWISevent.SetUI( this );
    dngINTevent.SetUI( this );

    // Setup the upgrade/downgrade buttons.
    upgSTRbutton.SetSpriteId   ( "GUI_SpinUp" );
    upgSTRbutton.SetHighlightUI( "GUI_SpinHUp" );
    upgENDbutton.SetSpriteId   ( "GUI_SpinUp" );
    upgENDbutton.SetHighlightUI( "GUI_SpinHUp" );
    upgAGIbutton.SetSpriteId   ( "GUI_SpinUp" );
    upgAGIbutton.SetHighlightUI( "GUI_SpinHUp" );
    upgWISbutton.SetSpriteId   ( "GUI_SpinUp" );
    upgWISbutton.SetHighlightUI( "GUI_SpinHUp" );
    upgINTbutton.SetSpriteId   ( "GUI_SpinUp" );
    upgINTbutton.SetHighlightUI( "GUI_SpinHUp" );

    dngSTRbutton.SetSpriteId   ( "GUI_SpinDown" );
    dngSTRbutton.SetHighlightUI( "GUI_SpinHDown" );
    dngENDbutton.SetSpriteId   ( "GUI_SpinDown" );
    dngENDbutton.SetHighlightUI( "GUI_SpinHDown" );
    dngAGIbutton.SetSpriteId   ( "GUI_SpinDown" );
    dngAGIbutton.SetHighlightUI( "GUI_SpinHDown" );
    dngWISbutton.SetSpriteId   ( "GUI_SpinDown" );
    dngWISbutton.SetHighlightUI( "GUI_SpinHDown" );
    dngINTbutton.SetSpriteId   ( "GUI_SpinDown" );
    dngINTbutton.SetHighlightUI( "GUI_SpinHDown" );

    upgSTRbutton.SetPos( FramePos( dwXPos+145, dwYPos+ 45, dwXPos+145+13, dwYPos+ 45+13) );
    upgENDbutton.SetPos( FramePos( dwXPos+145, dwYPos+ 88, dwXPos+145+13, dwYPos+ 88+13) );
    upgAGIbutton.SetPos( FramePos( dwXPos+145, dwYPos+131, dwXPos+145+13, dwYPos+131+13) );    
    upgINTbutton.SetPos( FramePos( dwXPos+145, dwYPos+217, dwXPos+145+13, dwYPos+217+13) );
    upgWISbutton.SetPos( FramePos( dwXPos+145, dwYPos+174, dwXPos+145+13, dwYPos+174+13) );

    dngSTRbutton.SetPos( FramePos( dwXPos+145, dwYPos+ 59, dwXPos+145+13, dwYPos+ 59+13) );
    dngENDbutton.SetPos( FramePos( dwXPos+145, dwYPos+102, dwXPos+145+13, dwYPos+102+13) );
    dngAGIbutton.SetPos( FramePos( dwXPos+145, dwYPos+145, dwXPos+145+13, dwYPos+145+13) );    
    dngINTbutton.SetPos( FramePos( dwXPos+145, dwYPos+231, dwXPos+145+13, dwYPos+231+13) );
    dngWISbutton.SetPos( FramePos( dwXPos+145, dwYPos+188, dwXPos+145+13, dwYPos+188+13) );

    SetHelpId( upgSTRbutton, 298 );
    SetHelpId( upgENDbutton, 298 );
    SetHelpId( upgAGIbutton, 298 );
    SetHelpId( upgWISbutton, 298 );
    SetHelpId( upgINTbutton, 298 );
    SetHelpId( dngSTRbutton, 299 );
    SetHelpId( dngENDbutton, 299 );
    SetHelpId( dngAGIbutton, 299 );
    SetHelpId( dngWISbutton, 299 );
    SetHelpId( dngINTbutton, 299 );
    
    statsCommitEvent.SetUI( this );
    statsCommitButton.SetSpriteId     ( "GUI_ButtonUp"   );
    statsCommitButton.SetDownUI       ( "GUI_ButtonDown" );
    statsCommitButton.SetHighlightUI  ( "GUI_ButtonHUp"  );
    statsCommitButton.SetPos( FramePos( dwXPos+337, dwYPos+198, dwXPos+337+60, dwYPos+198+32 ) );    
    statsCommitButton.Hide();
    SetHelpId( statsCommitButton, 295 );

    // Hide all upgrade buttons by default.
    upgSTRbutton.Hide();
    upgENDbutton.Hide();
    upgAGIbutton.Hide();
    upgWISbutton.Hide();
    upgINTbutton.Hide();
    dngSTRbutton.Hide();
    dngENDbutton.Hide();
    dngAGIbutton.Hide();
    dngWISbutton.Hide();
    dngINTbutton.Hide();

    statSTR.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
    statEND.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
    statAGI.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
    statWIS.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
    statINT.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
	statLCK.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
    statAC.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
    statHP.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
    statMP.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
    weight.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );

    iconSTR.SetPos   ( FramePos(dwXPos+15, dwYPos+44,  dwXPos+58, dwYPos+73 ) );
    iconEND.SetPos   ( FramePos(dwXPos+15, dwYPos+87,  dwXPos+58, dwYPos+116) );
    iconAGI.SetPos   ( FramePos(dwXPos+15, dwYPos+130, dwXPos+58, dwYPos+159) );
    iconWIS.SetPos   ( FramePos(dwXPos+15, dwYPos+173, dwXPos+58, dwYPos+202) );
    iconINT.SetPos   ( FramePos(dwXPos+15, dwYPos+216, dwXPos+58, dwYPos+245) );
    iconAC.SetPos    ( FramePos(dwXPos+184, dwYPos+44, dwXPos+213, dwYPos+73) );
    iconLevel.SetPos ( FramePos(dwXPos+254, dwYPos+44, dwXPos+282, dwYPos+73) );
    iconHP.SetPos    ( FramePos(dwXPos+167, dwYPos+130, dwXPos+196, dwYPos+159) );
    iconMP.SetPos    ( FramePos(dwXPos+167, dwYPos+173, dwXPos+196, dwYPos+202) );
    iconWeight.SetPos( FramePos(dwXPos+167, dwYPos+216, dwXPos+196, dwYPos+245) );
	iconLuck.SetPos  ( FramePos(dwXPos+167, dwYPos+87,  dwXPos+196, dwYPos+116) );

    iconSTR.EnableSound   ( false );
    iconEND.EnableSound   ( false );
    iconAGI.EnableSound   ( false );
    iconWIS.EnableSound   ( false );
    iconINT.EnableSound   ( false );
    iconAC.EnableSound    ( false );
    iconLevel.EnableSound ( false );
    iconHP.EnableSound    ( false );
    iconMP.EnableSound    ( false );
    iconWeight.EnableSound( false );
	iconLuck.EnableSound  ( false );

    SetHelpId( iconSTR, 262 );
    SetHelpId( iconEND, 263 );
    SetHelpId( iconAGI, 264 );
    SetHelpId( iconWIS, 265 );
    SetHelpId( iconINT, 266 );
    SetHelpId( iconHP,  286 );
    SetHelpId( iconMP,  287 );
    SetHelpId( iconAC,  445 );
    SetHelpId( iconLevel, 446 );
    SetHelpId( iconWeight, 486 );
	SetHelpId( iconLuck, 583 );

    macroButton.SetPos ( FramePos( dwXPos+576-35-60, dwYPos+1, dwXPos+576-35, dwYPos+1+22 ) );
    macroButton.SetSpriteId( "GUI_ButtonTUp" );
    macroButton.SetDownUI( "GUI_ButtonTDown" );
    macroButton.SetHighlightUI( "GUI_ButtonTHUp" );
    macroButtonEvent.SetUI( this );
    SetHelpId( macroButton, 447 );
   
    m_MainStatBack .SetSpriteId( "GUI_BackChStat" );
    m_MainStatBack .SetPos( FramePos( g_Global.GetScreenW()-220, 0, g_Global.GetScreenW(), 94 ) );
    m_MainStatBack .DrawAlpha(true);
    m_MainStatEmpty.SetSpriteId( "GUI_BackChStat_Empty" );
    m_MainStatHP   .SetSpriteId( "GUI_BackChStat_HP" );
    m_MainStatHP   .SetPos( FramePos( g_Global.GetScreenW()-118, 33, g_Global.GetScreenW()-16, 45 ) );
    m_MainStatMP   .SetSpriteId( "GUI_BackChStat_MP" );
    m_MainStatMP   .SetPos( FramePos( g_Global.GetScreenW()-118, 52, g_Global.GetScreenW()-16, 64 ) );
    m_MainStatXP   .SetSpriteId( "GUI_BackChStat_XP" );
    m_MainStatXP   .SetPos( FramePos( g_Global.GetScreenW()-118, 71, g_Global.GetScreenW()-16, 83 ) );


    m_MainStatEmptyHP2.SetSpriteId( "GUI_BackChStat_Empty" );
    m_MainStatEmptyMP2.SetSpriteId( "GUI_BackChStat_Empty" );
    m_MainStatEmptyXP2.SetSpriteId( "GUI_BackChStat_Empty" );
    m_MainStatHP2     .SetSpriteId( "GUI_BackChStat_HP" );
    m_MainStatMP2     .SetSpriteId( "GUI_BackChStat_MP" );
    m_MainStatXP2     .SetSpriteId( "GUI_BackChStat_XP" );

    m_MainStatEmptyHP2.SetPos( FramePos( g_Global.GetScreenW()-382, 15, g_Global.GetScreenW()-278, 27 ) );
    m_MainStatEmptyMP2.SetPos( FramePos( g_Global.GetScreenW()-248, 15, g_Global.GetScreenW()-144, 27 ) );
    m_MainStatEmptyXP2.SetPos( FramePos( g_Global.GetScreenW()-114, 15, g_Global.GetScreenW()-10 , 27 ) );

    m_MainStatHP2     .SetPos( FramePos( g_Global.GetScreenW()-382, 15, g_Global.GetScreenW()-278, 27 ) );
    m_MainStatMP2     .SetPos( FramePos( g_Global.GetScreenW()-248, 15, g_Global.GetScreenW()-144, 27 ) );
    m_MainStatXP2     .SetPos( FramePos( g_Global.GetScreenW()-114, 15, g_Global.GetScreenW()-10 , 27 ) );


    // Register all children.
    AddChild( &backStat);
    AddChild( &X_Button);
    AddChild( &iconHP );
    AddChild( &iconMP );
    AddChild( &iconWeight );
    AddChild( &statLevel );
    AddChild( &iconLevel );
    AddChild( &iconAC );
    AddChild( &statAC );
    AddChild( &karma );
    AddChild( &weight );
    AddChild( &iconSTR );
    AddChild( &iconEND );
    AddChild( &iconAGI );
    AddChild( &iconWIS );
    AddChild( &iconINT );
    AddChild( &statSTR );
    AddChild( &statEND );
    AddChild( &statAGI );
    AddChild( &statWIS );
    AddChild( &statINT );
    AddChild( &statHP );
    AddChild( &statMP );
    AddChild( &statPnts );
    AddChild( &skillPnts );
    AddChild( &statXP );
    AddChild( &statXP2GO );
    AddChild( &skillList );
    AddChild( &upgSTRbutton );
    AddChild( &upgENDbutton );
    AddChild( &upgAGIbutton );
    AddChild( &upgWISbutton );
    AddChild( &upgINTbutton );
    AddChild( &dngSTRbutton );
    AddChild( &dngENDbutton );
    AddChild( &dngAGIbutton );
    AddChild( &dngWISbutton );
    AddChild( &dngINTbutton );
    AddChild( &statsCommitButton );
    AddChild( &macroButton );
	AddChild( &statLCK );
	AddChild( &iconLuck );
    
    // Get the skill list.
    TFCPacket sending;
    sending << (RQ_SIZE)RQ_GetSkillList;	
    SEND_PACKET( sending );

    RootBoxUI::GetInstance()->AddChild( this );
}

// Destructor ////////////////////////////////////////////////////////////////////////////
CharacterUI::~CharacterUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
   
}

void CharacterUI::Draw(V2SPRITEFX *vsfFX)
{
   int dwXPos = (g_Global.GetScreenW()-576)/2;
   int dwYPos = (g_Global.GetScreenH()-150-368)/2;
   if(dwYPos <0)
       dwYPos =0;
    
   GameUI::Draw( vsfFX );

   HDC hdc;
   DXDGetDC(&hdc, 705);

   // Draw les texte...
   char strBtnTexte[20];
   // Draw Box Header
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[15]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 237+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 237  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[18]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,65)+ 333  , dwYPos+100    , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[19]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,65)+ 333  , dwYPos+152   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[20]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,174)+ 38  , dwYPos+281   , g_Global.GetBtnTCE(), 0, strBtnTexte);
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[21]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,174)+ 232  , dwYPos+281   , g_Global.GetBtnTCE(), 0, strBtnTexte);
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[22]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,112)+ 425  , dwYPos+281   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[16]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 576-35-60, dwYPos , RGB(0, 0, 0)    , 0, strBtnTexte);

   if(statsCommitButton.IsShown())
   {
      sprintf(strBtnTexte  ,"%s",g_GUILocalString[17]);
      fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 337, dwYPos+198+4 , RGB(0, 0, 0)    , 0, strBtnTexte);
   }


   DXDReleaseDC(hdc, 705);
}

int CharacterUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::DrawMinimized( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Draws the minimized version of the character sheet.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
}

//////////////////////////////////////////////////////////////////////////////////////////
CharacterUI *CharacterUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the sole character sheet instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static CharacterUI charSheet;

    return &charSheet;
}

//////////////////////////////////////////////////////////////////////////////////////////
DWORD GetColor
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the color according to the given variables.
// 
(
 int stat,          // The stat
 int boostedStat,   // The boosted stat
 int upg            // The upgrade
)
// Return: DWORD, the RGB color.
//////////////////////////////////////////////////////////////////////////////////////////
{
    DWORD color;

    const int lightValue = 50;
    
    // If stat is boosted.
    if( boostedStat > stat )
    {
        // If the stat was upgraded. (stat train).
        if( upg != 0 )
            color = RGB( lightValue, 255, lightValue ); // Pale green
        else
            color = RGB( 5, 197, 37 ); // Green
    }
    else if( boostedStat < stat )// If stat is under normal.
    {
        // If the stat was upgraded.
        if( upg != 0 )
            color = RGB( 255, lightValue, lightValue ); // Pink
        else
            color = RGB( 255, 0, 0 ); // Red
    }
    else // If stat is normal
    {
        // If the stat was upgraded. (stat train).
        if( upg != 0 )
            color = /*g_Global.GetBtnTCE();//*/RGB( 255, 255, 255 ); // Bright white.
        else
            color = /*g_Global.GetBtnTCE();//*/RGB( 190, 190, 190 ); // Light grey.
    }
   
    return color;
}

//////////////////////////////////////////////////////////////////////////////////////////
string GetStatBoostString
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns a string which displays boosted and normal stats (for help).
// 
(
 int boost, // The boosted stat.
 int stat   // The normal stat.
)
// Return: string, the string.
//////////////////////////////////////////////////////////////////////////////////////////
{
    char buf[ 256 ];
    
    string str( itoa( boost, buf, 10 ) );

    str += " / ";
    str += itoa( stat, buf, 10 );

    return str;
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::UpdateCharacterSheet( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the character sheet.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( RootBoxUI::GetInstance()->GetLock() );
    
    char buf[ 256 ];

    statSTR.SetText( itoa( Player.bStr + upgSTR, buf, 10 ), GetColor( Player.Str, Player.bStr, upgSTR ) );
    statEND.SetText( itoa( Player.bEnd + upgEND, buf, 10 ), GetColor( Player.End, Player.bEnd, upgEND ) );
    statAGI.SetText( itoa( Player.bAgi + upgAGI, buf, 10 ), GetColor( Player.Agi, Player.bAgi, upgAGI ) );
    statWIS.SetText( itoa( Player.bWis + upgWIS, buf, 10 ), GetColor( Player.Wis, Player.bWis, upgWIS ) );
    statINT.SetText( itoa( Player.bInt + upgINT, buf, 10 ), GetColor( Player.Int, Player.bInt, upgINT ) );
	
    statAC.SetText( itoa( Player.bAC, buf, 10 ), GetColor( Player.AC, Player.bAC, 0 ) );

    FontManager *font = FontManager::GetInstance();
    
    statSTR.GetHelpText()->SetText( GetStatBoostString( Player.bStr, Player.Str ).c_str() );
    statEND.GetHelpText()->SetText( GetStatBoostString( Player.bEnd, Player.End ).c_str() );
    statAGI.GetHelpText()->SetText( GetStatBoostString( Player.bAgi, Player.Agi ).c_str() );
    statWIS.GetHelpText()->SetText( GetStatBoostString( Player.bWis, Player.Wis ).c_str() );
    statINT.GetHelpText()->SetText( GetStatBoostString( Player.bInt, Player.Int ).c_str() );
    statAC .GetHelpText()->SetText( GetStatBoostString( Player.bAC, Player.AC ).c_str() );

	// Luck
	
	string str( itoa( Player.bLck, buf, 10 ) );
	str += " / ";
	str += itoa( Player.Lck, buf, 10 );
	statLCK.SetText( str.c_str(), GetColor( Player.Lck, Player.bLck, 0 ) );	
	statLCK.GetHelpText()->SetText( GetStatBoostString( Player.bLck, Player.Lck ).c_str() );

    // HP
    str = itoa( Player.Hp, buf, 10 );
    str += " / ";
    str += itoa( Player.MaxHp, buf, 10 );
    statHP.SetText( str.c_str(), GetColor( Player.TrueMaxHp, Player.MaxHp, 0 ) );

    if( Player.TrueMaxHp != Player.MaxHp ){
        str = itoa( Player.Hp, buf, 10 );
        str += " / ";
        str += itoa( Player.MaxHp, buf, 10 );
        str += "(";
        str += itoa( Player.TrueMaxHp, buf, 10 );
        str += ")";
    }
    statHP.GetHelpText()->SetText( str.c_str() );


    // MP
    str =  itoa( Player.Mana, buf, 10 );
    str += " / ";
    str += itoa( Player.MaxMana, buf, 10 );
    statMP.SetText( str.c_str() );
    statMP.GetHelpText()->SetText( str.c_str() );

    statPnts.SetText( itoa( Player.StatsPts - usedStatPts, buf, 10 ) );
    skillPnts.SetText( itoa( Player.SkillPts, buf, 10 ) );

    statXP.SetText( _i64toa( Player.Exp, buf, 10 ) );
    statXP2GO.SetText( _i64toa( Player.Exp2Go-Player.Exp, buf, 10 ) );

    statLevel.SetText( itoa( Player.Level, buf, 10 ) );

    str =  itoa( Player.Weight, buf, 10 );
    str += " / ";
    str += itoa( Player.MaxWeight, buf, 10 );
    weight.SetText( str.c_str() );
    weight.GetHelpText()->SetText( str.c_str() );

    if( Player.Karma >= 400 ){
        karma.SetText( g_LocalString[ 300 ] );
    }else if( Player.Karma >= 300 ){
        karma.SetText( g_LocalString[ 301 ] );
    }else if( Player.Karma >= 200 ){
        karma.SetText( g_LocalString[ 302 ] );
    }else if( Player.Karma >= 100 ){
        karma.SetText( g_LocalString[ 303 ] );
    }else if( Player.Karma >= -100 ){
        karma.SetText( g_LocalString[ 304 ] );
    }else if( Player.Karma >= -200 ){
        karma.SetText( g_LocalString[ 305 ] );
    }else if( Player.Karma >= -300 ){
        karma.SetText( g_LocalString[ 306 ] );
    }else if( Player.Karma >= -400 ){
        karma.SetText( g_LocalString[ 307 ] );
    }else{
        karma.SetText( g_LocalString[ 308 ] );
    }    
    
    
    if( Player.StatsPts != 0 ){
        upgSTRbutton.Show();
        upgENDbutton.Show();
        upgAGIbutton.Show();
        upgWISbutton.Show();
        upgINTbutton.Show();
        dngSTRbutton.Show();
        dngENDbutton.Show();
        dngAGIbutton.Show();
        dngWISbutton.Show();
        dngINTbutton.Show();

        statsCommitButton.Show();
    }else{
        upgSTRbutton.Hide();
        upgENDbutton.Hide();
        upgAGIbutton.Hide();
        upgWISbutton.Hide();
        upgINTbutton.Hide();
        dngSTRbutton.Hide();
        dngENDbutton.Hide();
        dngAGIbutton.Hide();
        dngWISbutton.Hide();
        dngINTbutton.Hide();

        statsCommitButton.Hide();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::UpdateSkills( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the skill list.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    const int lightValue = 50;
    
    CAutoLock autoLock( RootBoxUI::GetInstance()->GetLock() );

    skillList.DeleteList();

    Player.tlUserSkill.Lock( "CharacterUI::UpdateSkills" );
    Player.tlUserSkill.ToHead();
    while( Player.tlUserSkill.QueryNext() ){
        
        USER_SKILL *userSkill = Player.tlUserSkill.GetObject();

        SkillUI *skillUI = new SkillUI;

        DWORD color;

        // If skill cannot be used.
        if( userSkill->bUse == 0 ){
            if( userSkill->dwStrength == userSkill->dwTrueStrength ){
                // Gray shall be its color!
                color = RGB( 175, 175, 175 );
            }else if( userSkill->dwStrength > userSkill->dwTrueStrength ){
                // Dark green
                color = RGB( 5, 197, 37 );
            }else{
                // Dark red.
                color = RGB( 255, 0, 0 );
            }
        }else{
            if( userSkill->dwStrength == userSkill->dwTrueStrength ){
                // White shall be its color!
                color = RGB( 255, 255, 255 );
            }else if( userSkill->dwStrength > userSkill->dwTrueStrength ){
                // Pale green
                color = RGB( lightValue, 255, lightValue );
            }else{
                // Pale red.
                color = RGB( 255, lightValue, lightValue );
            }
        }

        char buf[ 256 ];
        
        string skillDesc( " " );
        skillDesc += userSkill->desc;
        skillDesc += " ";
        skillDesc +=  itoa( userSkill->dwStrength, buf, 10 );
        skillDesc += " / ";
        skillDesc += itoa( userSkill->dwTrueStrength, buf, 10 );      

        skillUI->SetVSF( SkillIcons( userSkill->wID ) );
        skillUI->userSkill = userSkill;
        // Set the pos equal to the text's pos.
        skillUI->SetPos( FramePos( 0, 0, skillUI->GetVSF()->GetWidth(), skillUI->GetVSF()->GetHeight() ) );

        skillUI->SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, skillDesc.c_str() ) );

        skillList.AddNewItem();
        skillList.AddColumnItem( 0, skillUI );

        
        {
            StaticTextUI *skillUI = new StaticTextUI;

            char buf[ 256 ];
            skillUI->SetText( itoa( userSkill->dwStrength, buf, 10 ), color );
            // Set the pos equal to the text's pos.
            skillUI->SetPos( FramePos( 0, 0, skillUI->GetWidth(), skillUI->GetHeight() ) );
            // The list UI will be responsible for centering this item.
            skillUI->SetCenter( false );

            skillUI->SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, skillDesc.c_str() ) );

            skillList.AddColumnItem( 1, skillUI );
        }
    }


    Player.tlUserSkill.Unlock( "CharacterUI::UpdateSkills" );
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int CharacterUI::Display( void *null )
//////////////////////////////////////////////////////////////////////////////////////////
//  Callback displaying the inventory macro.
// 
// Return: unsigned int, 0
//////////////////////////////////////////////////////////////////////////////////////////
{
	// Lock the root box since this call came from some unknown thread.
    CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );

	 g_EventHelp.neverOpenStat = false;

    // If inventory is already shown.
    if( GetInstance()->IsShown() ){    
        // Hide box
        GetInstance()->Hide();
    }else{
	     TFCPacket Send, Send2;	
	     
        // Get the stats
        Send << (RQ_SIZE)RQ_GetStatus;	
	     SEND_PACKET(Send);

        // Get the skill list.
        Send2 << (RQ_SIZE)RQ_GetSkillList;	
	     SEND_PACKET(Send2);
        
        // Show box.
        GetInstance()->Show();
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::SkillEvent::LeftDblClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left dbl clicked on skill
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    
    CharacterUI *charUI = static_cast< CharacterUI * >( GetUI() );
    
    GameUI *skill = charUI->skillList.GetSelection();

    if( skill == NULL ){
        return;
    }
    
    if( typeid( *skill ) == typeid( SkillUI ) ){
        
        SkillUI *skillUI = static_cast< SkillUI * >( skill );

        charUI->UseSkill( skillUI );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::UseSkill
//////////////////////////////////////////////////////////////////////////////////////////
//  Uses a skill
// 
(
 SkillUI *skillUI
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    TFCPacket sending;

    LPUSER_SKILL skillItem = skillUI->userSkill;

    char buf[ 256 ];

    switch( skillItem->bUse ){
	// Skill cannot be used.
	case 0: 
        naSound->Play( TRUE );

        ChatterUI::GetInstance()->AddBackscrollItem( 
            "",
            g_LocalString[ 440 ],
            SYSTEM_COLOR,
            true
        );
        break;
	// Skill used without any parameters.
	case 1:
        sprintf( buf, g_LocalString[ 439 ], skillItem->bName );

        ChatterUI::GetInstance()->AddBackscrollItem( 
            "",
            buf,
            SYSTEM_COLOR,
            true
        );

        LOG << "\r\n--Using skill " << (char *)skillItem->bName;
		// Just use the skill. Just do it!
		sending << (RQ_SIZE)RQ_UseSkill;
		sending << (short)skillItem->wID;
		sending << (short)0;
		sending << (short)0;
		sending << (long) 0;

		SEND_PACKET( sending );
	break;
	// Skill used using a world position.
	case 2: {
		LOG << "\r\n--Mousing (2) skill " << (char *)skillItem->bName;
		
		// Set a lock on the mouse
		MouseDevice md;
        if( md.LockNextEvent( DM_CLICK, skillItem, UseSkillPosition, 0 ) ){
            sprintf( buf, g_LocalString[ 441 ], skillItem->bName );
            ChatterUI::GetInstance()->AddBackscrollItem( 
                "",
                buf,
                SYSTEM_COLOR,
                true
            );

            if( IsShown() ){
               Hide();
            }
        }else{
            naSound->Play( TRUE );

            // Could not lock mouse
            ChatterUI::GetInstance()->AddBackscrollItem( 
                "", 
                g_LocalString[ 444 ], SYSTEM_COLOR, true
            );
        }
        
	}break;
	// Skill used using a unit.
	case 3:{
		LOG << "\r\n--Mousing (3) skill " << (char *)skillItem->bName;
		// Set a lock on the mouse
		MouseDevice md;
        if( md.LockNextEvent( DM_CLICK, skillItem, UseSkillUnit, 0 ) ){
            sprintf( buf, g_LocalString[ 441 ], skillItem->bName );

            ChatterUI::GetInstance()->AddBackscrollItem( 
                "",
                buf,
                SYSTEM_COLOR,
                true
            );
            if( IsShown() ){
               Hide();
            }
        }else{
            naSound->Play( TRUE );

            // Could not lock mouse.
            ChatterUI::GetInstance()->AddBackscrollItem( 
                "", 
                g_LocalString[ 444 ], SYSTEM_COLOR, true
            );
        }
	}break;
    }        
}

	
//////////////////////////////////////////////////////////////////////////////////////////
UINT CharacterUI::UseSkillPosition
//////////////////////////////////////////////////////////////////////////////////////////
// Called by the mouse event handler when the user has clicked on a position.
// 
(
 LPVOID lpData // The skillUI
)
// Return: static UINT, 0
//////////////////////////////////////////////////////////////////////////////////////////
{
    LPUSER_SKILL userSkill = reinterpret_cast< LPUSER_SKILL >( lpData );

	CMouseCursor::GetInstance()->SetCustomCursor( NULL );

	// Send the use skill with a world position but without a unit ID.
	TFCPacket sending;
	POINT mousePos;

	// Get the mouse pos.
    MouseDevice md;
    md.GetPosition( (int *)&mousePos.x, (int *)&mousePos.y );

	// then convert the mouse coordinates into world coordinates
	Player.ScreenPosToWL( &mousePos, &mousePos );	

	sending << (RQ_SIZE)RQ_UseSkill;
	sending << (short)userSkill->wID;

	signed char xPos, yPos;	
	Objects.Lock(2000);
   if (TargetID == Player.ID) {
      sending << (short)( Player.xPos );
      sending << (short)( Player.yPos );
      sending << (long)TargetID;
   } else if (Objects.Found( &xPos, &yPos, TargetID )) {
      sending << (short)( xPos+Player.xPos );
      sending << (short)( yPos+Player.yPos );
      sending << (long)TargetID;
   } else {
      sending << (short)( mousePos.x );
      sending << (short)( mousePos.y );
      sending << (long)NULL;
   }
   Objects.Unlock(2000);
	
	SEND_PACKET( sending );

    return 0;
}
	
//////////////////////////////////////////////////////////////////////////////////////////
UINT CharacterUI::UseSkillUnit
//////////////////////////////////////////////////////////////////////////////////////////
// Called by the mouse event handler when the user has clicked on a unit.
// 
(
 LPVOID lpData // The skillUI.
)
// Return: 	static UINT, 0
//////////////////////////////////////////////////////////////////////////////////////////
{
   LOG << "[USESKILLUNIT]";
   
   CMouseCursor::GetInstance()->SetCustomCursor( NULL );
   
   POINT mousePos;
   
   LPUSER_SKILL userSkill = reinterpret_cast< LPUSER_SKILL >( lpData );
   
  
   TFCPacket sending;
   sending << (RQ_SIZE)RQ_UseSkill;
   sending << (short)userSkill->wID;
   
   // Get the mouse pos.
   MouseDevice md;
   md.GetPosition( (int *)&mousePos.x, (int *)&mousePos.y );
   
   // Get the world pos.
   Player.ScreenPosToWL( &mousePos, &mousePos );
   
   // If mouse hit the player.
   /*if( mousePos.x > 285 && mousePos.x < 350 &&
   mousePos.y > 223 && mousePos.y < 282 ){
   
     sending << (short)userSkill->wID;
     sending << (short)worldPos.x;
     sending << (short)worldPos.y;
     sending << (long)Player.ID;
     
       SEND_PACKET( sending );
       }
       // Otherwise check if it hit another unit.
   else{ */
   signed char xPos, yPos;	
   Objects.Lock(2000);
   if (TargetID == Player.ID) {
      sending << (short)( Player.xPos );
      sending << (short)( Player.yPos );
      sending << (long)TargetID;
      SEND_PACKET( sending );
   } else if (Objects.Found( &xPos, &yPos, TargetID )) {
      sending << (short)( xPos+Player.xPos );
      sending << (short)( yPos+Player.yPos );
      sending << (long)TargetID;
      SEND_PACKET( sending );
   } else {
      //      sending << (short)( mousePos.x );
      //      sending << (short)( mousePos.y );
      //      sending << (long)NULL;
   }
   Objects.Unlock(2000);
   
   return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::StatUpgEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on a stat upgrade event.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CharacterUI *charUI = static_cast< CharacterUI * >( GetUI() );
    
    if( charUI->usedStatPts < Player.StatsPts ){

        int *statPtr;
        
        switch( stat ){
        case STRid:  statPtr = &charUI->upgSTR; break;
        case ENDid:  statPtr = &charUI->upgEND; break;
        case AGIid:  statPtr = &charUI->upgAGI; break;
        case WISid:  statPtr = &charUI->upgWIS; break;
        case INTid:  statPtr = &charUI->upgINT; break;
        default: return;
        }

        (*statPtr)++;
        
        charUI->usedStatPts++;
        
        charUI->UpdateCharacterSheet();
    }else{
        naSound->Play( TRUE );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::StatDngEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on a stat downgrade event.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CharacterUI *charUI = static_cast< CharacterUI * >( GetUI() );

    int *statPtr;
    
    switch( stat ){
    case STRid:  statPtr = &charUI->upgSTR; break;
    case ENDid:  statPtr = &charUI->upgEND; break;
    case AGIid:  statPtr = &charUI->upgAGI; break;
    case WISid:  statPtr = &charUI->upgWIS; break;
    case INTid:  statPtr = &charUI->upgINT; break;
    default: return;
    }

    if( *statPtr > 0 ){
        (*statPtr)--;
    
        charUI->usedStatPts--;
    
        charUI->UpdateCharacterSheet();
    }else{
        naSound->Play( TRUE );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::StatCommitEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Someone pushed the stat commit button!!.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CharacterUI *charUI = static_cast< CharacterUI * >( GetUI() );
    
    bool send = false;

    // If any of the stats changed.
    if( charUI->upgSTR != 0 ||
        charUI->upgEND != 0 ||
        charUI->upgAGI != 0 ||
        charUI->upgINT != 0 ||
        charUI->upgWIS != 0 ){

        // Send a stat train packet.
        TFCPacket sending;
	    sending << (RQ_SIZE)RQ_SendStatTrain;
	    
	    sending << (char)( charUI->upgSTR ); // Str
	    sending << (char)( charUI->upgEND ); // End
	    sending << (char)( charUI->upgAGI ); // Agi
	    sending << (char)( 0 ); // Wil
	    sending << (char)( charUI->upgWIS ); // Wis
	    sending << (char)( charUI->upgINT ); // Int
	    sending << (char)( 0 ); // Lck	    

        SEND_PACKET( sending );

        // Assume the packet will pass and update the stats before receiving an update.
        Player.Str += charUI->upgSTR;
        Player.End += charUI->upgEND;
        Player.Agi += charUI->upgAGI;
        Player.Int += charUI->upgINT;
        Player.Wis += charUI->upgWIS;

        Player.bStr += charUI->upgSTR;
        Player.bEnd += charUI->upgEND;
        Player.bAgi += charUI->upgAGI;
        Player.bInt += charUI->upgINT;
        Player.bWis += charUI->upgWIS;

        Player.StatsPts -= charUI->usedStatPts;

        charUI->upgSTR = 0;
        charUI->upgEND = 0;
        charUI->upgAGI = 0;
        charUI->upgWIS = 0;
        charUI->upgINT = 0;

        charUI->usedStatPts = 0;	    

        charUI->UpdateCharacterSheet();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::MacroButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Add a macro.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CharacterUI *charUI = static_cast< CharacterUI * >( GetUI() );

    SkillUI *skillUI = static_cast< SkillUI * >( charUI->skillList.GetSelection() );

    if( skillUI == NULL ){
        naSound->Play( TRUE );
        ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 351 ], SYSTEM_COLOR, false );
        return;
    }
    
    if( skillUI->userSkill->bUse == 0 ){
        naSound->Play( TRUE );
        ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 352 ], SYSTEM_COLOR, false );
        return;
    }

    SkillMacroUI *macroUI = new SkillMacroUI;

    macroUI->skillId = skillUI->userSkill->wID;
    macroUI->name    = reinterpret_cast< const char * >( skillUI->userSkill->bName );

    macroUI->SetVSF( SkillIcons( macroUI->skillId ) );
    macroUI->SetPos( FramePos( 0, 0, macroUI->GetVSF()->GetWidth(), macroUI->GetVSF()->GetHeight() ) );

    // Add macro for skill.
    MacroUI::GetInstance()->AddMacro( 
        macroUI, 
        reinterpret_cast< const char * >( skillUI->userSkill->bName ),
        &charUI->skillMacroEvent 
    );
    MacroUI::GetInstance()->SaveMacros();
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::SkillMacroEvent::ExecMacro
//////////////////////////////////////////////////////////////////////////////////////////
//  Executes a spell macro.
// 
(
 MacroDataUI *imacroUI
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CharacterUI *charUI = CharacterUI::GetInstance();
    SkillMacroUI *macroUI = static_cast< SkillMacroUI * >( imacroUI );
    
    vector< vector< ListUI::GridItem > > &skillGrid = *charUI->skillList.GetGrid();

    // Search for the skill in the skill list.
    UINT i;
    for( i = 0; i < skillGrid[ 0 ].size(); i++ ){
        SkillUI *skillUI = static_cast< SkillUI * >( skillGrid[ 0 ][ i ] );
       
        if( skillUI->userSkill->wID == macroUI->skillId ){
            // Use the skill.
            charUI->UseSkill( skillUI );

            return;
        }
    }

    naSound->Play( TRUE );
    ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 353 ], SYSTEM_COLOR, true );
}

//////////////////////////////////////////////////////////////////////////////////////////
// Saves the given macro.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::SkillMacroEvent::SaveMacro(const list< MacroDataUI * > &macroList)
{
   g_SaveGame.bFreeMacroSkill();
   list< MacroDataUI * >::const_iterator i;
   for( i = macroList.begin(); i != macroList.end(); i++ )
   {
      const SkillMacroUI *macroUI = static_cast< const SkillMacroUI * >( *i );
      sMacro sNewMacro;
      
      sNewMacro.dwID       = macroUI->skillId;
      sNewMacro.dwIcon     = macroUI->skillId;
      sNewMacro.dwFastPos  = macroUI->gridPos;
      sNewMacro.dwKey      = macroUI->vKey.AsLong();
      sprintf(sNewMacro.strName,"%s",macroUI->name.c_str());
      g_SaveGame.AddMacroSkill(sNewMacro);
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Client initializes, load the skill macros.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::ClientInitialize( void )
{
   int dwNbrSkillMacro = g_SaveGame.GetNbrMSkill();
   if(dwNbrSkillMacro > 0)
   {
      for(int i =0;i<dwNbrSkillMacro; i++ )
      {
         SkillMacroUI *macroUI = new SkillMacroUI;
         
         sMacro sNewMacro;
         g_SaveGame.GetMSkill(i,&sNewMacro);
         
         macroUI->skillId     = sNewMacro.dwID;
         macroUI->gridPos     = sNewMacro.dwFastPos;
         macroUI->name        = sNewMacro.strName;
         macroUI->vKey.SetLong(sNewMacro.dwKey);
         
         macroUI->SetVSF( SkillIcons( macroUI->skillId ) );
         macroUI->SetPos( FramePos( 0, 0, macroUI->GetVSF()->GetWidth(), macroUI->GetVSF()->GetHeight() ) );
         
         if( macroUI->skillId == 0)
            delete macroUI;
         else // Add the macro
            MacroUI::GetInstance()->AddMacro(macroUI, macroUI->name, &skillMacroEvent, false);
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::DrawSingleStatusItem
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws a single item
// 
(
 GraphUI &fullBar,  // The full bar to display 
 int percent        // The percentage
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    FramePos fullPos = fullBar.GetPos();
    FramePos emptyPos = fullBar.GetPos();

    // Create a clipping area according to the group member's hp.
    long newWidth = fullPos.Width() * ( 100 - percent ) / 100;

    RECT clipRect = { fullPos.x, fullPos.y, fullPos.maxx - newWidth, fullPos.maxy };

    V2SPRITEFX fx;
    memset( &fx, 0, sizeof( fx ) );

    fx.lpClipRect = &clipRect;
    fullBar.Draw( &fx );
}


//////////////////////////////////////////////////////////////////////////////////////////
void CharacterUI::DrawStatus( void )
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the user's current status bar
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
	if (g_SaveGame.GetOptionsParam()->bOldStatBar)
	{
		DrawSingleStatusItem(m_MainStatEmptyHP2, 100);
		DrawSingleStatusItem(m_MainStatEmptyMP2, 100);
		DrawSingleStatusItem(m_MainStatEmptyXP2, 100);
	}
	else
		m_MainStatBack.Draw(NULL);


	int percent = Player.MaxHp == 0 ? 0 : Player.Hp * 100 / Player.MaxHp;
	if (g_SaveGame.GetOptionsParam()->bOldStatBar)
		DrawSingleStatusItem(m_MainStatHP2, percent);
	else
		DrawSingleStatusItem(m_MainStatHP, percent);


	double curXP = (double)(Player.Exp - Player.ExpLastLevel);
	double maxXP = (double)(Player.Exp2Go - Player.ExpLastLevel);
	percent = (int)(maxXP == 0 ? 0 : (curXP * 100 / maxXP));

	if (g_SaveGame.GetOptionsParam()->bOldStatBar)
		DrawSingleStatusItem(m_MainStatXP2, percent);
	else
		DrawSingleStatusItem(m_MainStatXP, percent);


	percent = Player.MaxMana == 0 ? 0 : Player.Mana * 100 / Player.MaxMana;
	if (g_SaveGame.GetOptionsParam()->bOldStatBar)
		DrawSingleStatusItem(m_MainStatMP2, percent);
	else
		DrawSingleStatusItem(m_MainStatMP, percent);


	// Name the 3 bars at the top of the screen.
	HDC hdc;
	DXDGetDC(&hdc, 42);



	if (g_SaveGame.GetOptionsParam()->bOldStatBar)
	{
		fSkillFont->DrawFont(hdc, g_Global.GetScreenW() - 407, 13, RGB(223, 157, 0), 0, World.HPString);
		fSkillFont->DrawFont(hdc, g_Global.GetScreenW() - 273, 13, RGB(223, 157, 0), 0, World.MPString);
		fSkillFont->DrawFont(hdc, g_Global.GetScreenW() - 138, 13, RGB(223, 157, 0), 0, World.XPString);

		//BLBL tentative d'affichage de l'HP/MP/XP restante même sur la vielle barre d'hp/mp/xp (améliorer par shayds) :
		if (g_SaveGame.GetOptionsParam()->bDisplayXPBarText)
		{
			char displayedHp[50]; // steph ajout
			sprintf(displayedHp, "%li", Player.Hp); // steph ajout
			char displayedMana[50]; // steph ajout
			sprintf(displayedMana, "%li", Player.Mana); // steph ajout
			char strXP2Go[50];
			sprintf(strXP2Go, "%li", Player.Exp2Go - Player.Exp);
			int dw1Size = (int)fNewGuiFont->GetLen(hdc, displayedHp); // steph ajout
			int dw2Size = (int)fNewGuiFont->GetLen(hdc, displayedMana); // steph ajout
			int dw3Size = (int)fNewGuiFont->GetLen(hdc, strXP2Go); // steph dw3Size au lieu de dwSize
			int dw1Offset = (102 - dw1Size) / 2; // steph ajout
			int dw2Offset = (102 - dw2Size) / 2; // steph ajout
			int dw3Offset = (102 - dw3Size) / 2; // steph dw3Size au lieu de dwSize
			fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 380 + dw1Offset, 15, RGB(0, 0, 0), 0, displayedHp); // steph ajout
			fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 246 + dw2Offset, 15, RGB(223, 157, 0), 0, displayedMana); // steph ajout
			fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 112 + dw3Offset, 15, RGB(223, 157, 0), 0, strXP2Go); // steph dw3Offset au lieu de dwOffset
		}

	}
	else
	{
		fSkillFont->DrawFont(hdc, g_Global.GetScreenW() - 143, 33, RGB(223, 157, 0), 0, World.HPString);
		fSkillFont->DrawFont(hdc, g_Global.GetScreenW() - 143, 52, RGB(223, 157, 0), 0, World.MPString);
		fSkillFont->DrawFont(hdc, g_Global.GetScreenW() - 143, 70, RGB(223, 157, 0), 0, World.XPString);
		//Name and level
		char strLevel[10];
		sprintf(strLevel, "%d", Player.Level);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 176, 8, RGB(223, 157, 0), 0, strLevel);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 138, 8, RGB(223, 157, 0), 0, Player.Name);

		if (g_SaveGame.GetOptionsParam()->bDisplayXPBarText)
		{
			char displayedHp[50]; // steph ajout
			sprintf(displayedHp, "%li", Player.Hp); // steph ajout
			char displayedMana[50]; // steph ajout
			sprintf(displayedMana, "%li", Player.Mana); // steph ajout
			char strXP2Go[50];
			sprintf(strXP2Go, "%li", Player.Exp2Go - Player.Exp);
			int dw1Size = (int)fNewGuiFont->GetLen(hdc, displayedHp); // steph ajout
			int dw2Size = (int)fNewGuiFont->GetLen(hdc, displayedMana); // steph dw3Size au lieu de dwSize
			int dw3Size = (int)fNewGuiFont->GetLen(hdc, strXP2Go);
			int dw1Offset = (102 - dw1Size) / 2; // steph ajout
			int dw2Offset = (102 - dw3Size) / 2; // steph ajout
			int dw3Offset = (102 - dw3Size) / 2; // steph dw3Size au lieu de dwSize
			fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 112 + dw1Offset, 33, RGB(0, 0, 0), 0, displayedHp); // steph ajout
			fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 112 + dw2Offset, 52, RGB(223, 157, 0), 0, displayedMana); // steph ajout
			fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 112 + dw3Offset, 71, RGB(223, 157, 0), 0, strXP2Go); // steph dw3Offset au lieu de dwOffset
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////
	//  The user's current resist/power
	//  steph ajout
	//////////////////////////////////////////////////////////////////////////////////////////
	if (g_SaveGame.GetOptionsParam()->bDisplayResistPower)
	{
		char displayedPowerDarkTxt[50];
		sprintf(displayedPowerDarkTxt, "PD:");
		char displayedPowerDark[50];
		sprintf(displayedPowerDark, "%li", Player.Power.Dark);
		char displayedPowerLightTxt[50];
		sprintf(displayedPowerLightTxt, "PL:");
		char displayedPowerLight[50];
		sprintf(displayedPowerLight, "%li", Player.Power.Light);
		char displayedPowerEarthTxt[50];
		sprintf(displayedPowerEarthTxt, "PE:");
		char displayedPowerEarth[50];
		sprintf(displayedPowerEarth, "%li", Player.Power.Earth);
		char displayedPowerFireTxt[50];
		sprintf(displayedPowerFireTxt, "PF:");
		char displayedPowerFire[50];
		sprintf(displayedPowerFire, "%li", Player.Power.Fire);
		char displayedPowerWaterTxt[50];
		sprintf(displayedPowerWaterTxt, "PW:");
		char displayedPowerWater[50];
		sprintf(displayedPowerWater, "%li", Player.Power.Water);
		char displayedPowerAirTxt[50];
		sprintf(displayedPowerAirTxt, "PA:");
		char displayedPowerAir[50];
		sprintf(displayedPowerAir, "%li", Player.Power.Air);
		char displayedResistDarkTxt[50];
		sprintf(displayedResistDarkTxt, "RD:");
		char displayedResistDark[50];
		sprintf(displayedResistDark, "%li", Player.Resist.Dark);
		char displayedResistEarthTxt[50];
		sprintf(displayedResistEarthTxt, "RE:");
		char displayedResistEarth[50];
		sprintf(displayedResistEarth, "%li", Player.Resist.Earth);
		char displayedResistFireTxt[50];
		sprintf(displayedResistFireTxt, "RF:");
		char displayedResistFire[50];
		sprintf(displayedResistFire, "%li", Player.Resist.Fire);
		char displayedResistWaterTxt[50];
		sprintf(displayedResistWaterTxt, "RW:");
		char displayedResistWater[50];
		sprintf(displayedResistWater, "%li", Player.Resist.Water);
		char displayedResistAirTxt[50];
		sprintf(displayedResistAirTxt, "RA:");
		char displayedResistAir[50];
		sprintf(displayedResistAir, "%li", Player.Resist.Air);
		int dw1Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerDarkTxt);
		int dw2Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerDark);
		int dw3Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerLightTxt);
		int dw4Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerLight);
		int dw5Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerEarthTxt);
		int dw6Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerEarth);
		int dw7Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerFireTxt);
		int dw8Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerFire);
		int dw9Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerWaterTxt);
		int dw10Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerWater);
		int dw11Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerAirTxt);
		int dw12Size = (int)fNewGuiFont->GetLen(hdc, displayedPowerAir);
		int dw13Size = (int)fNewGuiFont->GetLen(hdc, displayedResistDarkTxt);
		int dw14Size = (int)fNewGuiFont->GetLen(hdc, displayedResistDark);
		int dw15Size = (int)fNewGuiFont->GetLen(hdc, displayedResistEarthTxt);
		int dw16Size = (int)fNewGuiFont->GetLen(hdc, displayedResistEarth);
		int dw17Size = (int)fNewGuiFont->GetLen(hdc, displayedResistFireTxt);
		int dw18Size = (int)fNewGuiFont->GetLen(hdc, displayedResistFire);
		int dw19Size = (int)fNewGuiFont->GetLen(hdc, displayedResistWaterTxt);
		int dw20Size = (int)fNewGuiFont->GetLen(hdc, displayedResistWater);
		int dw21Size = (int)fNewGuiFont->GetLen(hdc, displayedResistAirTxt);
		int dw22Size = (int)fNewGuiFont->GetLen(hdc, displayedResistAir);
		int dw1Offset = (102 - dw1Size) / 2;
		int dw2Offset = (102 - dw2Size) / 2;
		int dw3Offset = (102 - dw3Size) / 2;
		int dw4Offset = (102 - dw4Size) / 2;
		int dw5Offset = (102 - dw5Size) / 2;
		int dw6Offset = (102 - dw6Size) / 2;
		int dw7Offset = (102 - dw7Size) / 2;
		int dw8Offset = (102 - dw8Size) / 2;
		int dw9Offset = (102 - dw9Size) / 2;
		int dw10Offset = (102 - dw10Size) / 2;
		int dw11Offset = (102 - dw11Size) / 2;
		int dw12Offset = (102 - dw12Size) / 2;
		int dw13Offset = (102 - dw13Size) / 2;
		int dw14Offset = (102 - dw14Size) / 2;
		int dw15Offset = (102 - dw15Size) / 2;
		int dw16Offset = (102 - dw16Size) / 2;
		int dw17Offset = (102 - dw17Size) / 2;
		int dw18Offset = (102 - dw18Size) / 2;
		int dw19Offset = (102 - dw19Size) / 2;
		int dw20Offset = (102 - dw20Size) / 2;
		int dw21Offset = (102 - dw21Size) / 2;
		int dw22Offset = (102 - dw22Size) / 2;
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 500 + dw1Offset, 15, RGB(255, 255, 255), 0, displayedPowerDarkTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 477 + dw2Offset, 15, RGB(255, 255, 255), 0, displayedPowerDark);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 553 + dw3Offset, 15, RGB(255, 255, 255), 0, displayedPowerLightTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 531 + dw4Offset, 15, RGB(255, 255, 255), 0, displayedPowerLight);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 605 + dw5Offset, 15, RGB(255, 255, 255), 0, displayedPowerEarthTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 582 + dw6Offset, 15, RGB(255, 255, 255), 0, displayedPowerEarth);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 657 + dw7Offset, 15, RGB(255, 255, 255), 0, displayedPowerFireTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 635 + dw8Offset, 15, RGB(255, 255, 255), 0, displayedPowerFire);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 712 + dw9Offset, 15, RGB(255, 255, 255), 0, displayedPowerWaterTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 687 + dw10Offset, 15, RGB(255, 255, 255), 0, displayedPowerWater);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 767 + dw11Offset, 15, RGB(255, 255, 255), 0, displayedPowerAirTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 744 + dw12Offset, 15, RGB(255, 255, 255), 0, displayedPowerAir);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 821 + dw13Offset, 15, RGB(255, 255, 255), 0, displayedResistDarkTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 797 + dw14Offset, 15, RGB(255, 255, 255), 0, displayedResistDark);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 875 + dw15Offset, 15, RGB(255, 255, 255), 0, displayedResistEarthTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 852 + dw16Offset, 15, RGB(255, 255, 255), 0, displayedResistEarth);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 929 + dw17Offset, 15, RGB(255, 255, 255), 0, displayedResistFireTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 906 + dw18Offset, 15, RGB(255, 255, 255), 0, displayedResistFire);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 984 + dw19Offset, 15, RGB(255, 255, 255), 0, displayedResistWaterTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 959 + dw20Offset, 15, RGB(255, 255, 255), 0, displayedResistWater);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 1040 + dw21Offset, 15, RGB(255, 255, 255), 0, displayedResistAirTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 1017 + dw22Offset, 15, RGB(255, 255, 255), 0, displayedResistAir);
	}


	//////////////////////////////////////////////////////////////////////////////////////////
	//  The user's current remaining weight
	//  steph ajout
	//////////////////////////////////////////////////////////////////////////////////////////
	if (g_SaveGame.GetOptionsParam()->bDisplayWeight)
	{
		char displayedWeightTxt[50];
		sprintf(displayedWeightTxt, "Remaining weight:");
		char displayedWeight[50];
		sprintf(displayedWeight, "%li", Player.MaxWeight - Player.Weight);
		int dw1Size = (int)fNewGuiFont->GetLen(hdc, displayedWeightTxt);
		int dw2Size = (int)fNewGuiFont->GetLen(hdc, displayedWeight);
		int dw1Offset = (102 - dw1Size) / 2;
		int dw2Offset = (102 - dw2Size) / 2;
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 993 + dw1Offset, 25, RGB(255, 255, 255), 0, displayedWeightTxt);
		fNewGuiFont->DrawFont(hdc, g_Global.GetScreenW() - 921 + dw2Offset, 25, RGB(255, 255, 255), 0, displayedWeight);
	}



	DXDReleaseDC(hdc, 42);
}




