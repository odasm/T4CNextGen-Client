// SpellUI.cpp
//
#include "../pch.h"

#pragma warning( disable : 4786 )

#include "SpellUI.h"
#include "../MemoryManager.h"

#include "RootBoxUI.h"
#include "ChatterUI.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]

#include "..\SaveGame.h"
extern CSaveGame g_SaveGame;

extern Font *fNewGuiBtnFont;
extern Font *fNewGuiSpellFont;

#define TARGET_UNIT_ANY								0
#define TARGET_UNIT_OBJECT							1
#define TARGET_UNIT_NPC								2
#define TARGET_UNIT_PC								3
#define TARGET_UNIT_LIVING							4
#define TARGET_SELF									5
#define TARGET_POSITION								6
#define TARGET_UNIT_PC_NONSEL						7
#define TARGET_UNIT_ANY_NONSELF					    8
#define TARGET_UNIT_LIVING_NONSELF		 		    9
#define TARGET_UNIT_LIVING_MONSTER				    10
#define TARGET_UNIT_LIVING_MONSTER_NONSELF          11
#define TARGET_UNIT_LIVING_PLAYER				    12
#define TARGET_UNIT_ANY_PLAYER					    13
#define TARGET_GROUP_UNIT                           14
#define TARGET_GROUP_SELF                           15
#define TARGET_GROUP_POSITION                       16
#define TARGET_NONGROUP_UNIT                        17
#define TARGET_NONGROUP_SELF                        18
#define TARGET_NONGROUP_POSITION                    19

namespace{
    struct SpellCallbackData{
        SpellCallbackData( const SpellUI::Spell &ispell, bool allowSelf )
            : spell( ispell ), allowSelf( allowSelf ), targetPriority( SpellUI::equalPriority )
        {};
        SpellUI::Spell spell;
        bool     allowSelf;
        int      targetPriority;
    };

    T3VSBSound *turnPageSound = NULL;
    T3VSBSound *noMorePageSound = NULL;
    T3VSBSound *selectSound = NULL;

    void InitSound(){
        if( turnPageSound == NULL ){
            turnPageSound = new T3VSBSound;
            turnPageSound->Create( "Page turning sound", TS_MEMORY );

            noMorePageSound = new T3VSBSound;
            noMorePageSound->Create( "General NA sound", TS_MEMORY );

            selectSound = new T3VSBSound;
            selectSound->Create( "Generic Drop Item", TS_MEMORY );
        }
    }
};

extern DWORD TargetID;

bool SpellUI::Spell::operator < ( const Spell &spell2 ) const{
    if( element < spell2.element ){
        return true;
    }else if( element > spell2.element ){
        return false;
    }
    return( level < spell2.level );
}


//////////////////////////////////////////////////////////////////////////////////////////
SpellUI::SpellUI( void )
:
    currentPage( 0 ),
    spellEvent1( 1 ),
    spellEvent2( 2 ),
    spellEvent3( 3 ),
    spellEvent4( 4 ),
    spell1( spellEvent1 ),
    spell2( spellEvent2 ),
    spell3( spellEvent3 ),
    spell4( spellEvent4 ),
    prevButton( prevClickEvent ),
    nextButton( nextClickEvent ),
    airEvent( Spell::air ),
    waterEvent( Spell::water ),
    earthEvent( Spell::earth ),
    fireEvent( Spell::fire ),
    darkEvent( Spell::dark ),
    lightEvent( Spell::light ),
    normalEvent( Spell::normal ),
    airButton( airEvent ),
    fireButton( fireEvent ),
    earthButton( earthEvent ),
    waterButton( waterEvent ),
    lightButton( lightEvent ),
    darkButton( darkEvent ),
    normalButton( normalEvent ),
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
    SetHelpId( this, 348 );
    
    backSpell.SetSpriteId( "GUI_BackSpell" );
    backSpell.SetPos( FramePos( dwXPos, dwYPos, dwXPos+576, dwYPos+368 ) );
    backSpell.DrawAlpha(true);

    X_Button.SetPos        ( FramePos( dwXPos+576-28, dwYPos+0, dwXPos+576-4, dwYPos+0+24 ) );
    X_Button.SetSpriteId   ( "GUI_X_ButtonUp" );
    X_Button.SetDownUI     ( "GUI_X_ButtonDown" );
    X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
    SetHelpId( X_Button, 261 );


    spell1.SetPos( FramePos( dwXPos-65+120, dwYPos+ 74, dwXPos-65+300, dwYPos+185 ) );
    spell2.SetPos( FramePos( dwXPos-65+120, dwYPos+196, dwXPos-65+300, dwYPos+307 ) );
    spell3.SetPos( FramePos( dwXPos-65+364, dwYPos+ 74, dwXPos-65+543, dwYPos+185 ) );
    spell4.SetPos( FramePos( dwXPos-65+364, dwYPos+196, dwXPos-65+543, dwYPos+307 ) );

    SetHelpId( spell1, 349 );
    SetHelpId( spell2, 349 );
    SetHelpId( spell3, 349 );
    SetHelpId( spell4, 349 );

    m_dwHighLightSpellID = -1; //aucun spell de selectionner

    spellEvent1.SetUI( this );
    spellEvent2.SetUI( this );
    spellEvent3.SetUI( this );
    spellEvent4.SetUI( this );

    char buf[ 256 ];
    int p;

    
    
    prevButton.SetPos( FramePos( dwXPos-65+106, dwYPos+ 69, dwXPos-65+147, dwYPos+113 ) );
    prevButton.EnableSound( false );
    prevButton.flapAnim = &flapAnimPrev;
    flapAnimPrev.SetPos( FramePos( dwXPos-65+82, dwYPos+ 50, dwXPos-65+83, dwYPos+50 ) );
    
    
    flapAnimPrev.LoadSequence( "64kSpellBookCornerR", 8 ,true);
    for( p = 'g'; p >= 'a'; p-- )
    {
        sprintf( buf, "64kSpellBookCornerR-%c", p );
        flapAnimPrev.AddFrame( buf,true);
    }

    prevClickEvent.SetUI( this );
    SetHelpId( prevButton, 346 );
    
    
       
    nextButton.SetPos( FramePos( dwXPos-65+560, dwYPos+ 69, dwXPos-65+605, dwYPos+113 ) );
    nextButton.EnableSound( false );
    nextButton.flapAnim = &flapAnimNext;
    flapAnimNext.SetPos( FramePos( dwXPos-65+513, dwYPos+ 52, dwXPos-65+523, dwYPos+50 ) );
    flapAnimNext.LoadSequence( "64kSpellBookCornerL", 8 ,true);
    for( p = 'g'; p >= 'a'; p-- )
    {
        sprintf( buf, "64kSpellBookCornerL-%c", p);
        flapAnimNext.AddFrame( buf,true);
    }
    nextClickEvent.SetUI( this );
    SetHelpId( nextButton, 347 );

    // The only two index buttons which never change place.
    fireButton.SetSpriteId( "64kSpellBookSignetFireLeft" );
    fireButton.SetPos( FramePos( dwXPos-65+304, dwYPos+ 311, dwXPos-65+339, dwYPos+360 ) );
    fireButton.Hide();
    
    normalButton.SetSpriteId( "64kSpellBookSignetNoneRight" );
    normalButton.SetPos( FramePos( dwXPos-65+541, dwYPos+ 311, dwXPos-65+575, dwYPos+360 ) );
    normalButton.Show();

    SetHelpId( fireButton,   507 );
    SetHelpId( waterButton,  508 );
    SetHelpId( earthButton,  509 );
    SetHelpId( airButton,    510 );
    SetHelpId( lightButton,  511 );
    SetHelpId( darkButton,   512 );
    SetHelpId( normalButton, 513 );
    
    airEvent.SetUI( this );
    earthEvent.SetUI( this );
    fireEvent.SetUI( this );
    waterEvent.SetUI( this );
    lightEvent.SetUI( this );
    darkEvent.SetUI( this );
    normalEvent.SetUI( this );

    macroButton.SetPos ( FramePos( dwXPos+576-35-60, dwYPos+1, dwXPos+576-35, dwYPos+1+22 ) );
    macroButton.SetSpriteId( "GUI_ButtonTUp" );
    macroButton.SetDownUI( "GUI_ButtonTDown" );
    macroButton.SetHighlightUI( "GUI_ButtonTHUp" );
    macroButtonEvent.SetUI( this );
    SetHelpId( macroButton, 345 );
        

    castCursor.CreateSprite( "64kCursorSpell-a" );
    castCursor.AddFrame( "64kCursorSpell-b" );
    castCursor.AddFrame( "64kCursorSpell-c" );
    castCursor.AddFrame( "64kCursorSpell-d" );
    castCursor.AddFrame( "64kCursorSpell-e" );
    castCursor.AddFrame( "64kCursorSpell-f" );
    castCursor.AddFrame( "64kCursorSpell-g" );
    castCursor.AddFrame( "64kCursorSpell-h" );
    castCursor.AddFrame( "64kCursorSpell-i" );
    castCursor.AddFrame( "64kCursorSpell-j" );
    castCursor.AddFrame( "64kCursorSpell-k" );
    castCursor.AddFrame( "64kCursorSpell-l" );

    AddChild( &backSpell );
    AddChild( &flapAnimPrev );
    AddChild( &flapAnimNext );
    AddChild( &spell1 );
    AddChild( &spell2 );
    AddChild( &spell3 );
    AddChild( &spell4 );
    AddChild( &X_Button );
    AddChild( &airButton );
    AddChild( &fireButton );
    AddChild( &waterButton );
    AddChild( &earthButton );
    AddChild( &lightButton );    
    AddChild( &normalButton );
    AddChild( &darkButton );
    AddChild( &macroButton );
    AddChild( &prevButton );
    AddChild( &nextButton );
    
    
    TFCPacket sending;
    sending << (RQ_SIZE)RQ_SendSpellList;
    sending << (char)1;
    
    SEND_PACKET( sending );
    
    RootBoxUI::GetInstance()->AddChild( this );
}


// Destructor ////////////////////////////////////////////////////////////////////////////
SpellUI::~SpellUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
}

void SpellUI::Draw( V2SPRITEFX *vsfFX )
{
   GameUI::Draw(vsfFX);

   HDC hdc;
   DXDGetDC(&hdc, 701);

   int dwXPos = (g_Global.GetScreenW()-576)/2;
   int dwYPos = (g_Global.GetScreenH()-150-368)/2;
   if(dwYPos <0)
       dwYPos =0;

   // Draw les texte...
   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[16]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 576-35-60, dwYPos , RGB(0, 0, 0)    , 0, strBtnTexte);
      // Draw Box Header
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[28]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 237+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 237  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   //Dessine les 4 info sur les spells
   COLORREF rcSpell0 = RGB(0,0,0);
   COLORREF rcSpell1 = RGB(0,0,0);
   COLORREF rcSpell2 = RGB(0,0,0);
   COLORREF rcSpell3 = RGB(0,0,0);
   
   if( spell1.GetSelectSprite()->IsShown() && spell1.spell.Id != 0 )
      rcSpell0 = RGB(40,35,105);
   else if( spell2.GetSelectSprite()->IsShown() && spell2.spell.Id != 0 )
      rcSpell1 = RGB(40,35,105);
   else if( spell3.GetSelectSprite()->IsShown() && spell3.spell.Id != 0 )
      rcSpell2 = RGB(40,35,105);
   else if( spell4.GetSelectSprite()->IsShown() && spell4.spell.Id != 0 )
      rcSpell3 = RGB(40,35,105);


   fNewGuiSpellFont->DrawFont(hdc, dwXPos+60 ,dwYPos+128 , rcSpell0, 0, g_GUILocalString[29]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+60 ,dwYPos+142 , rcSpell0, 0, g_GUILocalString[30]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+60 ,dwYPos+156 , rcSpell0, 0, g_GUILocalString[31]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+60 ,dwYPos+170 , rcSpell0, 0, g_GUILocalString[32]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+191,dwYPos+170 , rcSpell0, 0, g_GUILocalString[33]);

   fNewGuiSpellFont->DrawFont(hdc, dwXPos+304 ,dwYPos+128 , rcSpell2, 0, g_GUILocalString[29]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+304 ,dwYPos+142 , rcSpell2, 0, g_GUILocalString[30]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+304 ,dwYPos+156 , rcSpell2, 0, g_GUILocalString[31]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+304 ,dwYPos+170 , rcSpell2, 0, g_GUILocalString[32]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+435 ,dwYPos+170 , rcSpell2, 0, g_GUILocalString[33]);

   fNewGuiSpellFont->DrawFont(hdc, dwXPos+60 ,dwYPos+250 , rcSpell1, 0, g_GUILocalString[29]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+60 ,dwYPos+264 , rcSpell1, 0, g_GUILocalString[30]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+60 ,dwYPos+278 , rcSpell1, 0, g_GUILocalString[31]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+60 ,dwYPos+292 , rcSpell1, 0, g_GUILocalString[32]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+191,dwYPos+292 , rcSpell1, 0, g_GUILocalString[33]);

   fNewGuiSpellFont->DrawFont(hdc, dwXPos+304 ,dwYPos+250 , rcSpell3, 0, g_GUILocalString[29]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+304 ,dwYPos+264 , rcSpell3, 0, g_GUILocalString[30]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+304 ,dwYPos+278 , rcSpell3, 0, g_GUILocalString[31]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+304 ,dwYPos+292 , rcSpell3, 0, g_GUILocalString[32]);
   fNewGuiSpellFont->DrawFont(hdc, dwXPos+435 ,dwYPos+292 , rcSpell3, 0, g_GUILocalString[33]);



   DXDReleaseDC(hdc, 701);

}

int SpellUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}



//////////////////////////////////////////////////////////////////////////////////////////
SpellUI *SpellUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the spellUI instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static SpellUI instance;
    return &instance;
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int SpellUI::Display( void *null )
//////////////////////////////////////////////////////////////////////////////////////////
//  Callback displaying the inventory macro.
// 
// Return: unsigned int, 0
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Lock the root box since this call came from some unknown thread.
    CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );

    // If inventory is already shown.
    if( GetInstance()->IsShown() ){    
        // Hide box
        GetInstance()->Hide();
    }else{

    	TFCPacket sending;
        sending << (RQ_SIZE)RQ_SendSpellList;
	    sending << (char)1;
	
	    SEND_PACKET( sending );
        

        // Show box.
        GetInstance()->Show();
    }

    return 0;
}

void SpellUI::Hide()
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
   spell1.Select( false );
   spell2.Select( false );
   spell3.Select( false );
   spell4.Select( false );
   UpdateIndexes();
   //:CR_NMNMNM_SAVE 2006-10-10 //Add save on minimuze satte of GUI...
   g_SaveGame.bSave();
	BoxUI::Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::UpdateSpells
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the current spell list.
// 
(
 const list< Spell > &newSpellList
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
    
    spellList = newSpellList;

    spellList.sort();
    UpdateSpellPages();
}

//////////////////////////////////////////////////////////////////////////////////////////
 SpellUI::SpellPageUI::SpellPageUI
//////////////////////////////////////////////////////////////////////////////////////////
(
 EventVisitor &event
)
: spellEvent( event )
//////////////////////////////////////////////////////////////////////////////////////////
{
    name.SetCenter( false );
    spellType.SetCenter( false );
    duration.SetCenter( false );
    level.SetCenter( false );
    manaCost.SetCenter( false );
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::SpellPageUI::SetPos
//////////////////////////////////////////////////////////////////////////////////////////
//  Update all relative control positions.
// 
(
 FramePos framePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    GameUI::SetPos( framePos );

    int dwXPos = (g_Global.GetScreenW()-576)/2;
    int dwYPos = (g_Global.GetScreenH()-150-368)/2;
    if(dwYPos <0)
       dwYPos =0;

    int x = framePos.x;
    int y = framePos.y;

    // Set relative positions.
    name.SetPos(      FramePos(  70+x,  54+y, 217+x,  63+y ) );
    spellType.SetPos( FramePos(  70+x,  68+y, 217+x,  76+y ) );
    duration.SetPos(  FramePos(  70+x,  82+y, 131+x,  90+y ) );
    level.SetPos(     FramePos( 190+x,  96+y, 217+x, 105+y ) );
    manaCost.SetPos(  FramePos(  70+x,  96+y, 131+x, 105+y ) );
    initialIconPos.SetPos( 90+x,   5+y, 134+x,  45+y );
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::SpellPageUI::Select
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the new selection state of the page.
// 
(
 bool selectState // true if page is selected
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( selectState )
    {
        selectSound->Play( TRUE );
        selectSprite.Show();
    }
    else
    {
        selectSprite.Hide();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::SpellPageUI::ClearPage( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Clears the spell page.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    spell.Id = 0;

    level.SetText( "" );
    name.SetText( "" );
    duration.SetText( "" );
    spellType.SetText( "" );
    manaCost.SetText( "" );

    icon.SetVSF( NULL );
}


//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::SpellPageUI::FillSpellPage
//////////////////////////////////////////////////////////////////////////////////////////
// Fills a spell list
// 
(
 const Spell &ispell
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    char buf[ 256 ];

    spell = ispell;

    // Set the text controls
    level.SetText( itoa( spell.level, buf, 10 ), RGB( 9, 9, 9 ) );
    name.SetText( spell.name.c_str(), RGB( 9, 9, 9 ) );
            
    if( spell.duration > 1000 * 60 ){
        DWORD min = spell.duration / ( 60 * 1000 );
        sprintf( buf, g_LocalString[ 364 ], spell.duration / ( 60 * 1000 ) );
        
        // Get the seconds part
        int sec = spell.duration / 1000 - min * 60;

        if( sec > 0 ){
            char buf2[ 256 ];
            sprintf( buf2, g_LocalString[ 365 ], sec );
            strcat( buf, buf2 );
        }
        
    }
    else if( spell.duration > 1000 )
    {
        sprintf( buf, g_LocalString[ 365 ], spell.duration / 1000 );
    }
    else if( spell.duration == 0 )
    {
        strcpy( buf, g_LocalString[ 366 ] );
    }
    else
    {
        sprintf( buf, g_LocalString[ 367 ], spell.duration );
    }
    
    duration.SetText( buf, RGB( 9, 9, 9 ) );
    
    if( spell.type == Spell::physical ){
        spellType.SetText( g_LocalString[ 514 ], RGB( 9, 9, 9 ) );
    }else{
        spellType.SetText( g_LocalString[ 515 ], RGB( 9, 9, 9 ) );        
    }

    // Put a centered icon.
    CV2Sprite *lpVSF = SpellIcons( spell.icon );    
    icon.SetVSF( lpVSF );
    FramePos newPos = initialIconPos;
    newPos.x = initialIconPos.Width() / 2 - lpVSF->GetWidth() / 2 + initialIconPos.x;
    newPos.maxx = newPos.x + lpVSF->GetWidth();
    newPos.y = initialIconPos.Height() / 2 - lpVSF->GetHeight() / 2 + initialIconPos.y;
    newPos.maxy = newPos.y + lpVSF->GetHeight();
    icon.SetPos( newPos );
        

    manaCost.SetText( itoa( spell.manaCost, buf, 10 ), RGB( 9, 9, 9 ) );

    GetHelpText()->SetText( spell.desc.c_str() );
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::SpellPageUI::Draw
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the spell page control
// 
(
 V2SPRITEFX *vsfFX
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Draw the control's controls.

    selectSprite.Draw( vsfFX );
    name        .Draw( vsfFX );
    level       .Draw( vsfFX );
    duration    .Draw( vsfFX );
    spellType   .Draw( vsfFX );
    manaCost    .Draw( vsfFX );
    icon        .Draw( vsfFX );
}

//////////////////////////////////////////////////////////////////////////////////////////
bool SpellUI::SpellPageUI::LeftMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
// Mouse click events on the spell control.
// 
(
 MousePos mousePos
)
// Return: bool, true
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( IsDblClick() ){
        spellEvent.LeftDblClicked();
    }else{
        // Notify the button's event.
        spellEvent.LeftClicked();
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::ClearIndex
//////////////////////////////////////////////////////////////////////////////////////////
//  Clears the index associated with an element.
// 
(
 const Spell::Element e
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    switch( e ){
    case Spell::air:    airButton.Hide();    break;
    case Spell::water:  waterButton.Hide();  break;
    case Spell::fire:   fireButton.Hide();   break;
    case Spell::earth:  earthButton.Hide();  break;
    case Spell::light:  lightButton.Hide();  break;
    case Spell::dark:   darkButton.Hide();   break;
    case Spell::normal: normalButton.Hide(); break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::UpdateIndexes( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the indexes according to the currently displayed spells.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    Spell::Element firstElement = Spell::fire;
    
    if( spell1.spell.Id != 0 ){
        firstElement = spell1.spell.element;        
    }

    fireButton.Hide(); 
    waterButton.Hide();
    airButton.Hide();
    earthButton.Hide(); 
    lightButton.Hide(); 
    darkButton.Hide(); 
    normalButton.Hide(); 

    int dwXPos = (g_Global.GetScreenW()-576)/2;
    int dwYPos = (g_Global.GetScreenH()-150-368)/2;
    if(dwYPos <0)
       dwYPos =0;

    // If water is before the first element.
    if( Spell::water < firstElement ){
        waterButton.SetSpriteId( "64kSpellBookSignetWaterLeft" );
        waterButton.SetPos( FramePos( dwXPos-65+266, dwYPos+ 309, dwXPos-65+299, dwYPos+360 ) );		
    }else{
        waterButton.SetSpriteId( "64kSpellBookSignetWaterRight" );
        waterButton.SetPos( FramePos( dwXPos-65+365, dwYPos+ 311, dwXPos-65+399, dwYPos+360 ) );
    }

    // If air is before the first element.
    if( Spell::air < firstElement ){
        airButton.SetSpriteId( "64kSpellBookSignetAirLeft" );
        airButton.SetPos( FramePos( dwXPos-65+228, dwYPos+ 309, dwXPos-65+262, dwYPos+361 ) );
    }else{
        airButton.SetSpriteId( "64kSpellBookSignetAirRight" );
        airButton.SetPos( FramePos( dwXPos-65+401, dwYPos+ 311, dwXPos-65+435, dwYPos+360 ) );
    }

    // If earth is before the first element.
    if( Spell::earth < firstElement ){
        earthButton.SetSpriteId( "64kSpellBookSignetEarthLeft" );
        earthButton.SetPos( FramePos( dwXPos-65+194, dwYPos+ 309, dwXPos-65+225, dwYPos+361 ) );
    }else{
        earthButton.SetSpriteId( "64kSpellBookSignetEarthRight" );
        earthButton.SetPos( FramePos( dwXPos-65+437, dwYPos+ 311, dwXPos-65+468, dwYPos+360 ) );
    }

    // If light is before the first element.
    if( Spell::light < firstElement ){
        lightButton.SetSpriteId( "64kSpellBookSignetLightLeft" );
        lightButton.SetPos( FramePos( dwXPos-65+153, dwYPos+ 309, dwXPos-65+187, dwYPos+360 ) );
    }else{
        lightButton.SetSpriteId( "64kSpellBookSignetLightRight" );
        lightButton.SetPos( FramePos( dwXPos-65+470, dwYPos+ 311, dwXPos-65+504, dwYPos+360 ) );
    }

    
    // If dark is before the first element.
    if( Spell::dark < firstElement ){
        darkButton.SetSpriteId( "64kSpellBookSignetDarkLeft" );
        darkButton.SetPos( FramePos( dwXPos-65+115, dwYPos+ 309, dwXPos-65+147, dwYPos+360 ) );
    }else{
        darkButton.SetSpriteId( "64kSpellBookSignetDarkRight" );
        darkButton.SetPos( FramePos( dwXPos-65+506, dwYPos+ 311, dwXPos-65+539, dwYPos+360 ) );
    }

    fireButton.Show(); 
    waterButton.Show();
    airButton.Show();
    earthButton.Show(); 
    lightButton.Show(); 
    darkButton.Show(); 
    normalButton.Show(); 

    // Remove all indexes which are currently displayed.
    if( spell1.spell.Id != 0 ){
        ClearIndex( spell1.spell.element );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::UpdateSpellPages( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the spell pages according to the current page.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Fetch the first spell's iterator.
    list< Spell >::iterator i = spellList.begin();
    int c;
    for( c = 0; c < currentPage; c++ ){
        i++;
        // If we reached the end of the spell list before the offset.
        if( i == spellList.end() ){
            spell1.ClearPage();
            spell2.ClearPage();
            spell3.ClearPage();
            spell4.ClearPage();
            UpdateIndexes();
            return;
        }
    }

    if( i == spellList.end() ){
        spell1.ClearPage();
        spell2.ClearPage();
        spell3.ClearPage();
        spell4.ClearPage();
        UpdateIndexes();
        return;
    }
    
    // Update the first spell.
    spell1.FillSpellPage( *i );    

    i++;
    if( i == spellList.end() ){
        spell2.ClearPage(); spell3.ClearPage(); spell4.ClearPage();
        UpdateIndexes();
        return; 
    };
    // Update the second spell.
    spell2.FillSpellPage( *i );
    
    i++;
    if( i == spellList.end() ){ 
        spell3.ClearPage(); spell4.ClearPage();
        UpdateIndexes();
        return; 
    };
    // Update the third spell.
    spell3.FillSpellPage( *i );

    i++;
    if( i == spellList.end() ){ 
        spell4.ClearPage();
        UpdateIndexes();
        return; 
    };
    // Update the fourth spell.
    spell4.FillSpellPage( *i );

    UpdateIndexes();
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::PrevClickEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on the previous button.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SpellUI *spellUI = static_cast< SpellUI * >( GetUI() );

    spellUI->currentPage -= 4;
    if( spellUI->currentPage < 0 ){
        spellUI->currentPage = 0;

        noMorePageSound->Play( TRUE );
    }else{
        turnPageSound->Play( TRUE );
    }

    spellUI->UpdateSpellPages();
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::NextClickEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on the next button.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SpellUI *spellUI = static_cast< SpellUI * >( GetUI() );

    spellUI->currentPage += 4;
    if( spellUI->currentPage >= (int)spellUI->spellList.size() ){
        spellUI->currentPage -= 4;

        noMorePageSound->Play( TRUE );
    }else{
        spellUI->UpdateSpellPages();
        turnPageSound->Play( TRUE );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::SpellClickEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Selected a spell.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SpellUI *spellUI = static_cast< SpellUI * >( GetUI() );
    
    spellUI->spell1.Select( false );
    spellUI->spell2.Select( false );
    spellUI->spell3.Select( false );
    spellUI->spell4.Select( false );
    
    // Get the spell according to which event got triggered.
    switch( spellNum ){
    case 1: spellUI->spell1.Select( true ); break;
    case 2: spellUI->spell2.Select( true ); break;
    case 3: spellUI->spell3.Select( true ); break;
    case 4: spellUI->spell4.Select( true ); break;
    default: return;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::SpellClickEvent::LeftDblClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  If one of the spells got double-clicked.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SpellUI *spellUI = static_cast< SpellUI * >( GetUI() );

    Spell spell;
    
    LOG << "\r\nCLICKED ON A SPELL";

    // Get the spell according to which event got triggered.
    switch( spellNum ){
    case 1: spell = spellUI->spell1.spell; break;
    case 2: spell = spellUI->spell2.spell; break;
    case 3: spell = spellUI->spell3.spell; break;
    case 4: spell = spellUI->spell4.spell; break;
    default: return;
    }

    if( spell.Id == 0 ){
        noMorePageSound->Play( TRUE );
        return;
    }
    
    spellUI->CastSpell( spell, false );
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::CastSpell
//////////////////////////////////////////////////////////////////////////////////////////
//  Casts the given spell
// 
(
 const Spell &spell, // The spell to cast
 bool autoTargetSelf,
 bool noCallback
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Whether the spell can be cast on self or not.
    bool allowSelf = false;

    lastSpell = spell.Id;

    if( autoTargetSelf && spell.targetType != TARGET_SELF && 
        spell.targetType != TARGET_GROUP_SELF &&
        spell.targetType != TARGET_GROUP_POSITION
    ){
        TFCPacket sending;

		sending << (RQ_SIZE)RQ_CastSpell;
        sending << (short)spell.Id;
		sending << (short)Player.xPos;
        sending << (short)Player.yPos;
		sending << (long)Player.ID;
		
		SEND_PACKET( sending );
        return;        
    }


    switch( spell.targetType ){
    /////////////////////////////
    // Target Self
    case TARGET_GROUP_SELF:
    case TARGET_NONGROUP_SELF:
    case TARGET_SELF:{
        TFCPacket sending;
        
        sending << (RQ_SIZE)RQ_CastSpell;
		sending << (short)spell.Id;
		sending << (short)0;
		sending << (short)0;
		sending << (long)0;

        SEND_PACKET( sending );
    }break;

    ////////////////////////////
    // Target Position
    case TARGET_GROUP_POSITION:
    case TARGET_NONGROUP_POSITION:
    case TARGET_POSITION:{
		// Spell cast using a world position.			
		SpellCallbackData *mouseData = 
            new SpellCallbackData( spell, false );

        if( noCallback ){
            CastSpellPosition( mouseData );
        }else{
            // Set a lock on the mouse
            if( !mouse.LockNextEvent( DM_CLICK, mouseData, CastSpellPosition, 0 ) ){
                // Could not lock mouse.
                ChatterUI::GetInstance()->AddBackscrollItem( 
                    "", 
                    g_LocalString[ 444 ], SYSTEM_COLOR, true
                );

                delete mouseData;
            }else{
                CMouseCursor::GetInstance()->SetCustomCursor( &castCursor, 0, 0, true );
            }
        }
	}break;

	/////////////////////////////////
    // Favor NPC targetting
    case TARGET_NONGROUP_UNIT:
    case TARGET_UNIT_OBJECT:
    case TARGET_UNIT_NPC: 
	case TARGET_UNIT_LIVING_MONSTER: 
		allowSelf = true;
	case TARGET_UNIT_LIVING_MONSTER_NONSELF: {
		SpellCallbackData *mouseData = 
            new SpellCallbackData( spell,allowSelf );
		
        if( noCallback ){
            CastSpellUnit( mouseData );
        }else{
            // Set a lock on the mouse        
            if( !mouse.LockNextEvent( DM_CLICK, mouseData, CastSpellUnit, 0 ) ){
                // Could not lock mouse.
                ChatterUI::GetInstance()->AddBackscrollItem( 
                    "", 
                    g_LocalString[ 444 ], SYSTEM_COLOR, true
                );
                delete mouseData;
            }else{
                CMouseCursor::GetInstance()->SetCustomCursor( &castCursor, 0, 0, true );
            }
        }
	}break;

    ////////////////////////////////
    // Favor PC targetting
    case TARGET_GROUP_UNIT:
	case TARGET_UNIT_LIVING_PLAYER:
	case TARGET_UNIT_ANY_PLAYER:
	case TARGET_UNIT_PC: 
		allowSelf = true;
	case TARGET_UNIT_PC_NONSEL: {
		Objects.Lock(98);
		Objects.SetGridMode( playerPriority );
		Objects.Unlock(98);
			        
		SpellCallbackData *mouseData = 
            new SpellCallbackData( spell, allowSelf );

        if( noCallback ){
            CastSpellUnit( mouseData );
        }else{
            // Set a lock on the mouse
            if( !mouse.LockNextEvent( DM_CLICK, mouseData, CastSpellUnit, 0 ) ){
                // Could not lock mouse.
                ChatterUI::GetInstance()->AddBackscrollItem( 
                    "", 
                    g_LocalString[ 444 ], SYSTEM_COLOR, true
                );
                delete mouseData;
            }else{
                CMouseCursor::GetInstance()->SetCustomCursor( &castCursor, 0, 0, true );
            }
        }
	} break;

	////////////////////////////////
    // Favor any unit.
    case TARGET_UNIT_ANY:
	case TARGET_UNIT_LIVING:
		allowSelf = true;
	case TARGET_UNIT_LIVING_NONSELF:
	case TARGET_UNIT_ANY_NONSELF: {
		Objects.Lock(99);
		Objects.SetGridMode( equalPriority );
		Objects.Unlock(99);
		
		SpellCallbackData *mouseData = 
            new SpellCallbackData( spell, allowSelf );

        if( noCallback ){
            CastSpellUnit( mouseData );
        }else{
            // Set a lock on the mouse
            if( !mouse.LockNextEvent( DM_CLICK, mouseData, CastSpellUnit, 0 ) ){
                // Could not lock mouse.
                ChatterUI::GetInstance()->AddBackscrollItem( 
                    "", 
                    g_LocalString[ 444 ], SYSTEM_COLOR, true
                );
                delete mouseData;
            }else{
                CMouseCursor::GetInstance()->SetCustomCursor( &castCursor, 0, 0, true );
            }
        }
	}break;
    }

    if( IsShown() ){
        Hide();
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
UINT SpellUI::CastSpellPosition
//////////////////////////////////////////////////////////////////////////////////////////
// Callback fonction called after choosing a world position for using a skill
// 
(
 LPVOID rawData // The item data.
 )
 // Return: UINT, unused
 //////////////////////////////////////////////////////////////////////////////////////////
{	
   SpellCallbackData *mouseData = reinterpret_cast< SpellCallbackData * >( rawData );
   
   CMouseCursor::GetInstance()->SetCustomCursor( NULL );
   
   // Send the use skill with a world position but without a unit ID.
   TFCPacket sending;
   POINT mousePos;
      
   // Get the mouse pos.
   MouseDevice md;
   md.GetPosition( (int *)&mousePos.x, (int *)&mousePos.y );
   
   // Then convert the mouse coordinates into world coordinates
   Player.ScreenPosToWL( &mousePos, &mousePos );   
   
   sending << (RQ_SIZE)RQ_CastSpell;
   sending << (short)mouseData->spell.Id;	
   
   signed char xPos, yPos;
   Objects.Lock(100);
   if (TargetID == Player.ID) {
      sending << (short)( Player.xPos );
      sending << (short)( Player.yPos );
      sending << (long)TargetID;
   } else if (Objects.FoundValidID( &xPos, &yPos, TargetID )) {
      sending << (short)( xPos+Player.xPos );
      sending << (short)( yPos+Player.yPos );
      sending << (long)TargetID;
   } else {
      sending << (short)( mousePos.x );
      sending << (short)( mousePos.y );
      sending << (long)NULL;
   }
   Objects.Unlock(100);
   
   
   SEND_PACKET( sending );
   
   delete mouseData;
   
   return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
UINT SpellUI::CastSpellUnit
//////////////////////////////////////////////////////////////////////////////////////////
// Callback function called after choosing a unit.
// 
(
 LPVOID rawData // The item data.
)
// Return: UINT, unused.
//////////////////////////////////////////////////////////////////////////////////////////
{
	LOG << "[USESPELLUNIT]";

    CMouseCursor::GetInstance()->SetCustomCursor( NULL );
    
    POINT mousePos;
	//POINT worldPos;    

	SpellCallbackData *mouseData = reinterpret_cast< SpellCallbackData * >( rawData );
    	

    TFCPacket sending;
	sending << (RQ_SIZE)RQ_CastSpell;
		sending << (short)mouseData->spell.Id;

	// Get the mouse pos.
	MouseDevice md;
	md.GetPosition( (int *)&mousePos.x, (int *)&mousePos.y );
	
	// Get the world pos.
	Player.ScreenPosToWL( &mousePos, &mousePos );


   signed char xPos, yPos;
   Objects.Lock(100);
   if (TargetID == Player.ID) {
      sending << (short)( Player.xPos );
      sending << (short)( Player.yPos );
      sending << (long)TargetID;
      SEND_PACKET( sending );
   } else if (Objects.FoundValidID( &xPos, &yPos, TargetID )) {
      sending << (short)( xPos+Player.xPos );
      sending << (short)( yPos+Player.yPos );
      sending << (long)TargetID;
      SEND_PACKET( sending );
   } else {
//      sending << (short)( mousePos.x );
//      sending << (short)( mousePos.y );
//      sending << (long)NULL;
   }
   Objects.Unlock(100);
   delete mouseData;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::ElementEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked an elemental search.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SpellUI *spellUI = static_cast< SpellUI * >( GetUI() );

    list< Spell >::iterator i = spellUI->spellList.begin();
    
    int oldCurrentPage = spellUI->currentPage;
    
    // Find the first spell corresponding to the event's element.
    spellUI->currentPage = 0;
    while( i != spellUI->spellList.end() ){
        if( (*i).element == element ){
            spellUI->UpdateSpellPages();
            return;
        }
        spellUI->currentPage++;
        i++;
    }

    // Restore page state if the element wasn't found.
    spellUI->currentPage = oldCurrentPage;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool SpellUI::CastSpell
//////////////////////////////////////////////////////////////////////////////////////////
//  Casts a spell using its spell Id.
// 
(
 DWORD spellId,         // The spell Id.
 bool autoTargetSelf    // True if the target is automatically self.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    SpellUI *spellUI = SpellUI::GetInstance();

    list< Spell >::iterator i = spellUI->spellList.begin();
    // Search the spell in the spellbook.
    for( i = spellUI->spellList.begin(); i != spellUI->spellList.end(); i++ ){    
        if( (*i).Id == spellId ){
            // Cast the found spell.
            spellUI->CastSpell( *i, autoTargetSelf );
            return true;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::MacroButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on the macro button.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SpellUI *spellUI = static_cast< SpellUI * >( GetUI() );

    // Check which spell is selected
    Spell spell;
    spell.Id = 0;
    
    if( spellUI->spell1.GetSelectSprite()->IsShown() && spellUI->spell1.spell.Id != 0 ){
        spell = spellUI->spell1.spell;
    }else if( spellUI->spell2.GetSelectSprite()->IsShown() && spellUI->spell2.spell.Id != 0 ){
        spell = spellUI->spell2.spell;        
    }else if( spellUI->spell3.GetSelectSprite()->IsShown() && spellUI->spell3.spell.Id != 0 ){
        spell = spellUI->spell3.spell;
    }else if( spellUI->spell4.GetSelectSprite()->IsShown() && spellUI->spell4.spell.Id != 0 ){
        spell = spellUI->spell4.spell;
    }

    if( spell.Id == 0 ){
        ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 375 ], SYSTEM_COLOR, false );
        return;
    }

    SpellMacroUI *macroUI = new SpellMacroUI;

    macroUI->iconId = (unsigned short)spell.icon;
    macroUI->spell = spell;
    macroUI->SetVSF( SpellIcons( macroUI->iconId ) );
    macroUI->SetPos( FramePos( 0, 0, macroUI->GetVSF()->GetWidth(), macroUI->GetVSF()->GetHeight() ) );

    // Add macro for spell.
    MacroUI::GetInstance()->AddMacro( macroUI, spell.name.c_str(), &spellUI->spellMacroEvent );
    MacroUI::GetInstance()->SaveMacros();
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::SpellMacroEvent::ExecMacro
//////////////////////////////////////////////////////////////////////////////////////////
//  Executes a spell macro.
// 
(
 MacroDataUI *imacroUI
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    SpellMacroUI *spellMacro = static_cast< SpellMacroUI * >( imacroUI );

    SpellUI *spellUI = SpellUI::GetInstance();

    list< Spell >::iterator i = spellUI->spellList.begin();
    // Search the spell in the spellbook.
    for( i = spellUI->spellList.begin(); i != spellUI->spellList.end(); i++ ){    
        if( (*i).Id == spellMacro->spell.Id ){
            // Cast the found spell.
            spellUI->CastSpell( *i, spellMacro->autoTargetSelf );
            return;
        }
    }

    ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 376 ], SYSTEM_COLOR, true );
}

//////////////////////////////////////////////////////////////////////////////////////////
// Saves the given macro.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::SpellMacroEvent::SaveMacro(const list< MacroDataUI * > &macroList)
{
   g_SaveGame.bFreeMacroSpell();
   list< MacroDataUI * >::const_iterator i;
   for( i = macroList.begin(); i != macroList.end(); i++ )
   {
      const SpellMacroUI *macroUI = static_cast< const SpellMacroUI * >( *i );
      sMacro sNewMacro;
      
      sNewMacro.dwID       = macroUI->spell.Id;
      sNewMacro.dwIcon     = macroUI->iconId;
      sNewMacro.dwFastPos  = macroUI->gridPos;
      sNewMacro.dwKey      = macroUI->vKey.AsLong();
      sprintf(sNewMacro.strName,"%s",macroUI->spell.name.c_str());
      g_SaveGame.AddMacroSpell(sNewMacro);
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Client initializes the spell UI.
//  Load the spell macros.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::ClientInitialize( void )
{
   int dwNbrSpellMacro = g_SaveGame.GetNbrMSpell();
   if(dwNbrSpellMacro > 0)
   {
      for(int i =0;i<dwNbrSpellMacro; i++ )
      {
         SpellMacroUI *macroUI = new SpellMacroUI;
         
         sMacro sNewMacro;
         g_SaveGame.GetMSpell(i,&sNewMacro);
         
         macroUI->spell.Id    = sNewMacro.dwID;
         macroUI->iconId      = sNewMacro.dwIcon;
         macroUI->gridPos     = sNewMacro.dwFastPos;
         macroUI->spell.name  = sNewMacro.strName;
         macroUI->vKey.SetLong(sNewMacro.dwKey);
         
         macroUI->SetVSF( SpellIcons( macroUI->iconId ) );
         macroUI->SetPos( FramePos( 0, 0, macroUI->GetVSF()->GetWidth(), macroUI->GetVSF()->GetHeight() ) );
         
         if( macroUI->spell.Id == 0 || macroUI->iconId == 0 )
            delete macroUI;
         else // Add the macro
            MacroUI::GetInstance()->AddMacro(macroUI, macroUI->spell.name, &spellMacroEvent, false);
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::SpellPageButtonUI::Draw
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the flapping page UI
// 
(
 V2SPRITEFX *vsfFx
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    LOG << "\r\nDRAWING THE SPELL BUTTON UI";
    MousePos mousePos( MouseDevice::xPos, MouseDevice::yPos );
    if( GetPos().MousePosInFrame( mousePos ) ){
        flapAnim->Play();
    }else{
        flapAnim->Pause();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void SpellUI::CastLastSpell( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Casts the last spell
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    list< Spell >::iterator i = spellList.begin();
    // Search the spell in the spellbook.
    for( i = spellList.begin(); i != spellList.end(); i++ ){    
        if( (*i).Id == lastSpell ){
            // Cast the found spell.
            CastSpell( *i, false, true );
            return;
        }
    }
}