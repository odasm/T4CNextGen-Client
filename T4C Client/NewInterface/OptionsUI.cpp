// OptionsUI.cpp
//
#include "../pch.h"

#include "OptionsUI.h"
#include "RootBoxUI.h"
#include "../MemoryManager.h"
#include "ChatterUI.h"
#include "EffectStatusUI.h"
#include "..\SaveGame.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]

extern T3VSBSound SoundFX[250];
extern CSaveGame g_SaveGame;
extern Font *fNewGuiBtnFont;
extern Font *fNewGuiFont;

//////////////////////////////////////////////////////////////////////////////////////////
OptionsUI::OptionsUI( void ) 
 :
    graphList( graphListEvent ),
    logList( logListEvent ),
    musicVolume( musicVolumeEvent ),
    fxVolume( fxVolumeEvent ),
    brightness( brightnessEvent ),
    cdMusic( cdMusicEvent ),
    disabledCdMusic( cdMusicEvent ),
    quitButton( quitButtonEvent ),
    cancelButton( hideEvent ),
    lowCPUButton( lowCpuButtonEvent ),
    logFileNameButton( fileNameEvent ),
    X_Button( hideEvent )
//////////////////////////////////////////////////////////////////////////////////////////
{

    int dwXPos = (g_Global.GetScreenW()-512)/2;
    int dwYPos = (g_Global.GetScreenH()-150-368)/2;
    if(dwYPos <0)
       dwYPos =0;

    
    SetPos( FramePos( dwXPos, dwYPos, dwXPos+512, dwYPos+368 ) );
    SetHelpId( this, 387 );

    backOption.SetSpriteId( "GUI_BackOption" );    
    backOption.SetPos( FramePos( dwXPos, dwYPos, dwXPos+512, dwYPos+368 ) );
    backOption.DrawAlpha(true);

    X_Button.SetPos( FramePos( dwXPos+512-28, dwYPos+0, dwXPos+512-4, dwYPos+0+24 ) );
    X_Button.SetSpriteId   ( "GUI_X_ButtonUp" );
    X_Button.SetDownUI     ( "GUI_X_ButtonDown" );
    X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
    SetHelpId( X_Button, 261 );
   
    //Option sonores....
    musicVolume.SetPos(         FramePos( dwXPos+105, dwYPos+65, dwXPos+242, dwYPos+85 ) );
    musicVolume.SetRange( 0, 10, FramePos( dwXPos+127, dwYPos+65, dwXPos+211, dwYPos+85 ) );
    musicVolume.SetScrollRegions(
        FramePos( dwXPos+105, dwYPos+65, dwXPos+126, dwYPos+85 ),
        FramePos( dwXPos+220, dwYPos+65, dwXPos+242, dwYPos+85 ),
        "",
        ""
    );
    musicVolumeEvent.SetUI( this );
    SetHelpId( musicVolume, 388 );

    fxVolume.SetPos(         FramePos( dwXPos+105, dwYPos+96, dwXPos+242, dwYPos+116 ) );
    fxVolume.SetRange( 0, 10, FramePos( dwXPos+127, dwYPos+96, dwXPos+211, dwYPos+116 ) );
    fxVolume.SetScrollRegions(
        FramePos( dwXPos+105, dwYPos+96, dwXPos+126, dwYPos+116 ),
        FramePos( dwXPos+220, dwYPos+96, dwXPos+242, dwYPos+116 ),
        "",
        ""
    );
    fxVolumeEvent.SetUI( this );
    SetHelpId( fxVolume, 389 );



    //Option Graphiques...

    brightness.SetPos  (        FramePos( dwXPos+373, dwYPos+267, dwXPos+488, dwYPos+287 ) );
    brightness.SetRange( 2, 10, FramePos( dwXPos+395, dwYPos+267, dwXPos+457, dwYPos+287 ) );
    brightness.SetScrollRegions(
        FramePos( dwXPos+373, dwYPos+267, dwXPos+394, dwYPos+287 ),
        FramePos( dwXPos+466, dwYPos+267, dwXPos+488, dwYPos+287 ),
        "",
        "" 
    );
    brightnessEvent.SetUI( this );
    SetHelpId( brightness, 390 );

    //Liste des option graphiques...
    graphListScrollUp.SetPos  ( FramePos( dwXPos+468, dwYPos+64  , dwXPos+488, dwYPos+86 ) );
    graphListScrollDown.SetPos( FramePos( dwXPos+468, dwYPos+224 , dwXPos+488, dwYPos+243 ) );

    graphList.SetPos( FramePos( dwXPos+267, dwYPos+62, dwXPos+494, dwYPos+245 ) );
    graphList.SetupList( 2, 16, FramePos( 0, 0, 5, 5 ) );
    graphList.SetColumnSize( 0, 169 );
    graphList.SetColumnSize( 1, 16  );
    graphList.CenterColumn( 1 );
    graphList.SetScrollRegions(
        FramePos( dwXPos+468, dwYPos+64  , dwXPos+491, dwYPos+86 ),
        FramePos( dwXPos+468, dwYPos+224 , dwXPos+491, dwYPos+243 ),
        FramePos( dwXPos+468, dwYPos+87  , dwXPos+491, dwYPos+214 )
    );
    graphListingSelectionGraphPatch.SetPos( FramePos( 0, 0, 171, 16 ) );
    graphListingSelectionGraphPatch.SetSpriteId( "GUI_OptionGraphSelect" );
    graphListingSelectionGraphPatch.Show();
    graphList.SetSelectionSprite( &graphListingSelectionGraphPatch, -2);
    graphListEvent.SetUI( this );
    graphList.SetScrollDownDownUI( &graphListScrollDown );
    graphList.SetScrollUpDownUI( &graphListScrollUp );


    //Liste des option Logs
    logListScrollUp.SetPos  ( FramePos( dwXPos+223, dwYPos+190  , dwXPos+243, dwYPos+212 ) );
    logListScrollDown.SetPos( FramePos( dwXPos+223, dwYPos+242 , dwXPos+243, dwYPos+262 ) );

    logList.SetPos( FramePos( dwXPos+22, dwYPos+188, dwXPos+211, dwYPos+266 ) );
    logList.SetupList( 2, 16, FramePos( 0, 0, 5, 5 ) );
    logList.SetColumnSize( 0, 169 );
    logList.SetColumnSize( 1, 16  );
    logList.CenterColumn( 1 );
    logList.SetScrollRegions(
        FramePos( dwXPos+223, dwYPos+190 , dwXPos+246, dwYPos+212 ),
        FramePos( dwXPos+223, dwYPos+242 , dwXPos+246, dwYPos+264 ),
        FramePos( dwXPos+223, dwYPos+213 , dwXPos+246, dwYPos+218 )
    );
    logListingSelectionGraphPatch.SetPos( FramePos( 0, 0, 171, 16 ) );
    logListingSelectionGraphPatch.SetSpriteId( "GUI_OptionGraphSelect" );
    logListingSelectionGraphPatch.Show();
    logList.SetSelectionSprite( &logListingSelectionGraphPatch, -2);
    logListEvent.SetUI( this );
    logList.SetScrollDownDownUI( &logListScrollDown );
    logList.SetScrollUpDownUI( &logListScrollUp );

    
   
    //cdMusic.SetPos( FramePos( dwXPos+233, dwYPos+132 , dwXPos+248, dwYPos+147 ) );
    //cdMusic.SetSpriteId   ( "CheckONLightUP" );
    //cdMusic.SetDownUI     ( "CheckONLightDown" );
    //cdMusic.SetHighlightUI( "CheckONLightHUP" );
    //cdMusicEvent.SetUI( this );
    //SetHelpId( cdMusic, 391 );

    //disabledCdMusic.SetPos( FramePos( dwXPos+233, dwYPos+132 , dwXPos+248, dwYPos+147 ) );
    //disabledCdMusic.SetSpriteId    ( "CheckOFFLightUP" );
    //disabledCdMusic.SetDownUI      ( "CheckOFFLightDown" );
    //disabledCdMusic.SetHighlightUI ( "CheckOFFLightHUP" );
    //SetHelpId( disabledCdMusic, 391 );


    quitButton.SetPos ( FramePos( dwXPos+274, dwYPos+300, dwXPos+274+60, dwYPos+300+32 ) );
    quitButton.SetSpriteId   ( "GUI_ButtonUp" );
    quitButton.SetDownUI     ( "GUI_ButtonDown" );
    quitButton.SetHighlightUI( "GUI_ButtonHUp" );
    quitButtonEvent.SetUI( this );
    SetHelpId( quitButton, 392 );

    cancelButton.SetPos ( FramePos( dwXPos+174, dwYPos+300, dwXPos+174+60, dwYPos+300+32 ) );
    cancelButton.SetSpriteId   ( "GUI_ButtonUp" );
    cancelButton.SetDownUI     ( "GUI_ButtonDown" );
    cancelButton.SetHighlightUI( "GUI_ButtonHUp" );
    SetHelpId( cancelButton, 261 );


    lowCPUButton.SetPos ( FramePos( dwXPos+437, dwYPos+300, dwXPos+437+60, dwYPos+300+32 ) );
    lowCPUButton.SetSpriteId   ( "GUI_ButtonUp" );
    lowCPUButton.SetDownUI     ( "GUI_ButtonDown" );
    lowCPUButton.SetHighlightUI( "GUI_ButtonHUp" );
    lowCpuButtonEvent.SetUI( this );
    SetHelpId( lowCPUButton, 579 );

    //lowCPUButton


    logFileName.SetPos      ( FramePos( dwXPos+146, dwYPos+274, dwXPos+242, dwYPos+285 ) );
    logFileNameButton.SetPos( FramePos( dwXPos+144, dwYPos+272, dwXPos+244, dwYPos+287 ) );
    SetHelpId( logFileName, 474 );
    
    fileNameEvent.SetUI( this );

    AddChild( &backOption );
    AddChild( &X_Button );
    AddChild( &graphList );
    AddChild( &logList );

    AddChild( &logFileName );
    AddChild( &musicVolume );
    AddChild( &fxVolume );
    AddChild( &brightness );    
    //AddChild( &cdMusic );
    //AddChild( &disabledCdMusic );
    AddChild( &quitButton );
    AddChild( &cancelButton );
    AddChild( &lowCPUButton);
    AddChild( &logFileNameButton );

    brightness .SetSliderPos(  g_SaveGame.GetOptionsParam()->dwBrightness );
    fxVolume   .SetSliderPos(  g_SaveGame.GetOptionsParam()->dwSoundVol );
    musicVolume.SetSliderPos(  g_SaveGame.GetOptionsParam()->dwMusicVol );

    LoadOptions();

    RootBoxUI::GetInstance()->AddChild( this );
}

// Destructor ////////////////////////////////////////////////////////////////////////////
OptionsUI::~OptionsUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
{


}

void OptionsUI::Draw(V2SPRITEFX *vsfFX)
{
   int dwXPos = (g_Global.GetScreenW()-512)/2;
   int dwYPos = (g_Global.GetScreenH()-150-368)/2;
   if(dwYPos <0)
       dwYPos =0;
    
   GameUI::Draw( vsfFX );

   HDC hdc;
   DXDGetDC(&hdc, 720);

   // Draw les texte...
   char strBtnTexte[25];
   // Draw Box Header
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[41]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 206+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 206  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[44]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,74)+ 22  , dwYPos+69    , g_Global.GetBtnTCE(), 0, strBtnTexte);
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[45]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,74)+ 22  , dwYPos+100   , g_Global.GetBtnTCE(), 0, strBtnTexte);
   //sprintf(strBtnTexte  ,"%s",g_GUILocalString[46]);//CD Audio // Audio CD
   sprintf(strBtnTexte  ,"t4c-rebirth"); // steph
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,122)+ 104  , dwYPos+134   , g_Global.GetBtnTCE(), 0, strBtnTexte);


   sprintf(strBtnTexte  ,"%s",g_GUILocalString[51]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,116)+ 21  , dwYPos+274   , g_Global.GetBtnTCE(), 0, strBtnTexte);
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[65]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,98)+ 266  , dwYPos+271   , g_Global.GetBtnTCE(), 0, strBtnTexte);


   sprintf(strBtnTexte  ,"%s",g_GUILocalString[42]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,99 )+ 83  , dwYPos+43   , g_Global.GetBtnTCE(), 0, strBtnTexte);
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[43]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,116)+ 295 , dwYPos+43   , g_Global.GetBtnTCE(), 0, strBtnTexte);
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[47]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,116)+ 50 , dwYPos+169   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[11]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 174, dwYPos+300+4 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[66]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 274, dwYPos+300+4 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_LocalString[580]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 437, dwYPos+300+4 , RGB(0, 0, 0)    , 0, strBtnTexte);



   DXDReleaseDC(hdc, 720);
}

int OptionsUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
OptionsUI *OptionsUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the macro UI's instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static OptionsUI instance;
    return &instance;
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int OptionsUI::Display
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

    // If already shown.
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

void OptionsUI::Hide()
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
   //:CR_NMNMNM_SAVE 2006-10-10 //Add save on minimuze satte of GUI...
   g_SaveGame.bSave();
	BoxUI::Hide();
}

void OptionsUI::ShowQuitWindows()
{
   quitPopup.Show();
   RequestForegroundControl( &quitPopup );
}

void OptionsUI::HideQuitWindows()
{
   quitPopup.Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
OptionsUI::FileNamePopup::FileNamePopup( void ) 
 :
   cancelButton( hideEvent ),
   okButton( okEvent )
//////////////////////////////////////////////////////////////////////////////////////////
{
    //Back du popup box...
    int dwOffX = (g_Global.GetScreenW()-240)/2;
    int dwOffY = (g_Global.GetScreenH()-164-150)/2;
    SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );

    NameBack.SetSpriteId( "GUI_PopupBack" );
    NameBack.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );
    NameBack.DrawAlpha(true);
    // patch pour la zone de texte
    graphPatch.SetSpriteId( "GUI_MacroPopupBack" );
    graphPatch.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+85 ) );
    

    okButton.SetSpriteId   ( "GUI_ButtonUp" );
    okButton.SetDownUI     ( "GUI_ButtonDown" );
    okButton.SetHighlightUI( "GUI_ButtonHUp" );
    okButton.SetPos( FramePos( dwOffX+52, dwOffY+94, dwOffX+52+60, dwOffY+94+32 ) );
    okEvent.SetUI( this );
    SetHelpId( okButton, 456 );

    cancelButton.SetSpriteId   ( "GUI_ButtonUp" );
    cancelButton.SetDownUI     ( "GUI_ButtonDown" );
    cancelButton.SetHighlightUI( "GUI_ButtonHUp" );
    cancelButton.SetPos( FramePos( dwOffX+128, dwOffY+94, dwOffX+128+60, dwOffY+94+32 ) );
    hideEvent.SetUI( this );
    SetHelpId( cancelButton, 457 );

    fileName.SetPos( FramePos( dwOffX+100, dwOffY+42, dwOffX+219, dwOffY+71 ) );
    fileName.EnableCursor( true );
    fileName.SetMaxTextLength( 20 );
    SetHelpId( fileName, 475 );

    AddChild( &NameBack );
    AddChild( &graphPatch );
    AddChild( &okButton );
    AddChild( &cancelButton );
    AddChild( &fileName );

    RootBoxUI::GetInstance()->AddChild( this );
}

void OptionsUI::FileNamePopup::Draw(V2SPRITEFX *vsfFX)
{
   int dwOffX = (g_Global.GetScreenW()-240)/2;
   int dwOffY = (g_Global.GetScreenH()-164-150)/2;

   GameUI::Draw( vsfFX );

   HDC hdc;
   DXDGetDC(&hdc, 730);
   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[10]);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 52, dwOffY+96 , RGB(0, 0, 0)    , 0, strBtnTexte);
   
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[11]);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 128, dwOffY+96 , RGB(0, 0, 0)    , 0, strBtnTexte);
   
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[99]);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,70)+ 20+1, dwOffY+35+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,70)+ 20  , dwOffY+35   , g_Global.GetBtnTCE(), 0, strBtnTexte);
   
   DXDReleaseDC(hdc, 730);
}

int OptionsUI::FileNamePopup::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::FileNamePopup::OkEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    OptionsUI::FileNamePopup *o = static_cast< OptionsUI::FileNamePopup * >( GetUI() );
    
    ChatterUI::GetInstance()->SetBacklogFile( (char *)o->fileName.GetText().c_str() );

    OptionsUI::GetInstance()->LoadOptions();

    o->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::MusicVolumeEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Music volume changed
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    OptionsUI *ui = static_cast< OptionsUI * >( GetUI() );

    if(ui->musicVolume.GetSliderPos() <3)
    { 
       g_GameMusic.Stop();
       g_GameMusic.Reset();
       g_GameMusic.LoadNewSound();
    }
    g_SaveGame.GetOptionsParam()->dwMusicVol = ui->musicVolume.GetSliderPos();
    g_GameMusic.SetNewVolume();
}
//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::FxVolumeEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Sound FX volume changed
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    OptionsUI *ui = static_cast< OptionsUI * >( GetUI() );
    
    g_SaveGame.GetOptionsParam()->dwSoundVol = ui->fxVolume.GetSliderPos();
    g_GameMusic.SetNewVolume();
}
//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::BrightnessEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Brightness changed
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    OptionsUI *ui = static_cast< OptionsUI * >( GetUI() );

    g_SaveGame.GetOptionsParam()->dwBrightness = ui->brightness.GetSliderPos();
    
    Objects.Lock(1017);
    Objects.lmPlayerLight     .Release();
    Objects.lmPlayerLight     .Create("TorchPlayer.rgb", g_Global.GetMainTorchW(), g_Global.GetMainTorchH());
    Objects.lmOtherPlayerLight.Release();
    Objects.lmOtherPlayerLight.Create("TorchOtherPlayer.rgb", 160, 120);
    Objects.lmTorchLight      .Release();
    Objects.lmTorchLight      .Create("Torch.rgb", 160, 120);
    Objects.lmTorchLanterne1  .Release();
    Objects.lmTorchLanterne1  .Create("TorchLanterne1.rgb", 160, 130);
    Objects.lmTorchPoutre     .Release();
    Objects.lmTorchPoutre     .Create("TorchPoutre.rgb", 80, 90);
    Objects.lmTorchChandelle2 .Release();
    Objects.lmTorchChandelle2 .Create("TorchChandelle2.rgb", 80, 90);
    Objects.Unlock(1017);


    Objects.Lock(30);
    Objects.UpdateLight(TRUE);
    Objects.Unlock(30);
}
//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::CdMusicEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// CD Music state changed
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    OptionsUI *ui = static_cast< OptionsUI * >( GetUI() );
  
    g_GameMusic.Stop();
    g_GameMusic.Reset();
    
    g_SaveGame.GetOptionsParam()->bUseCD = !g_SaveGame.GetOptionsParam()->bUseCD;

    if( g_SaveGame.GetOptionsParam()->bUseCD ){
        ui->cdMusic.Show();
        ui->disabledCdMusic.Hide();
    }else{
        ui->cdMusic.Hide();
        ui->disabledCdMusic.Show();

    }

    
    g_GameMusic.LoadNewSound();
}

//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::LowCpuButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// CD Music state changed
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
   OptionsUI *ui = static_cast< OptionsUI * >( GetUI() );
   
   OptionParam *pOption = g_SaveGame.GetOptionsParam();

   pOption->bLightHightGraph        = 1;
   pOption->bHightGraphEffect       = 0;

   pOption->bAnimatedWater          = 0; 
   g_Global.ResetAnimWater();
   //g_Global.ResetAnimLava();
   World.Lock(0);
   Objects.Lock(0);
   World.SetPosition(Player.xPos, Player.yPos, Player.World, TRUE,FALSE);
   Objects.Unlock(0);
   World.Unlock(0);

   pOption->bDitherGraph            = 1;
   pOption->bGuiAlpha               = 0;
   pOption->bSeraphAnim             = 0;
   pOption->bDisplayStatus          = 1;
   pOption->bDisplayXPBarText       = 0;
   pOption->bDisplayResistPower		= 0; // steph ajout
   pOption->bDisplayWeight			= 0; // steph ajout
   pOption->bEnableDisplayGold		= 1; // steph ajout
   pOption->bDisplayMacroFullScreen = 0;
   pOption->bLockResize				= 0; // steph 0 au lieu de 1
   pOption->bLockTarget				= 0;
   
   pOption->bHighFont               = 0; 
   ChatterUI::GetInstance()->SetMinimizedHight(ChatterUI::GetInstance()->IsMinimizedHight());

   pOption->bEnableZoom             = 0; 
   g_Global.SetZoomStatus(0);

   pOption->bHyperChat              = 0; 

   pOption->bShowItemSpec           = 0; 
   pOption->bShowNewLife            = 0; 
   pOption->bShowNewOmbrage         = 0;
   pOption->bShowAnimDecorsLight    = 0;
   pOption->bShowWeatherEffects		= 0;
   pOption->bOldStatBar				= 1;
      
   ui->UpdateGraphList();
}


//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::QuitEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Quit the game
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    OptionsUI *o = static_cast< OptionsUI * >( GetUI() );    
    
    // Summon the JunkDrop popupUI.
    o->quitPopup.Show();
    
    o->RequestForegroundControl( &o->quitPopup );
}

//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::LoadOptions( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Loads the options
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
   UpdateGraphList();
   UpdateLogList();
}

void OptionsUI::UpdateGraphList()
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
   int dwScrollPos = 0;
   if(graphList.GetGrid()->size())
      dwScrollPos = graphList.GetScrollPos();
   graphList.DeleteList();
    

   OptionParam *pOption = g_SaveGame.GetOptionsParam();
   char strParam[50];
   sprintf(strParam,"%s",g_GUILocalString[52]);
   AddGraphparameters(strParam,0, pOption->bLightHightGraph);
   sprintf(strParam,"%s",g_GUILocalString[53]);
   AddGraphparameters(strParam,1, pOption->bHightGraphEffect);
   sprintf(strParam,"%s",g_GUILocalString[168]);
   AddGraphparameters(strParam,2, pOption->bAnimatedWater);

   sprintf(strParam,"%s",g_GUILocalString[217]);
   AddGraphparameters(strParam,3, pOption->bShowAnimDecorsLight);

	// Destiny, weather on/off
   sprintf( strParam, "%s", g_GUILocalString[218] );
   AddGraphparameters( strParam, 4, pOption->bShowWeatherEffects );

   sprintf(strParam, "%s", g_GUILocalString[54]);
   AddGraphparameters(strParam, 5, pOption->bDitherGraph);
   sprintf(strParam, "%s", g_GUILocalString[55]);
   AddGraphparameters(strParam, 6, pOption->bGuiAlpha);
   sprintf(strParam, "%s", g_GUILocalString[56]);
   AddGraphparameters(strParam, 7, pOption->bSeraphAnim);
   sprintf(strParam, "%s", g_GUILocalString[57]);
   AddGraphparameters(strParam, 8, pOption->bDisplayStatus);
   sprintf(strParam, "%s", g_GUILocalString[58]);
   AddGraphparameters(strParam, 9, pOption->bDisplayXPBarText);
   sprintf(strParam, "%s", g_GUILocalString[226]); // steph ajout
   AddGraphparameters(strParam, 20, pOption->bDisplayResistPower); // steph ajout
   sprintf(strParam, "%s", g_GUILocalString[227]); // steph ajout
   AddGraphparameters(strParam, 21, pOption->bDisplayWeight); // steph ajout
   sprintf(strParam, "%s", g_GUILocalString[228]); // steph ajout
   AddGraphparameters(strParam, 22, pOption->bEnableDisplayGold); // steph ajout
   sprintf(strParam, "%s", g_GUILocalString[59]);
   AddGraphparameters(strParam, 10, pOption->bDisplayMacroFullScreen);
   //sprintf(strParam,"%s",g_GUILocalString[60]); 
   //AddGraphparameters(strParam,11, pOption->bLockResize); // steph désactivation
   sprintf(strParam, "%s", g_GUILocalString[61]);
   AddGraphparameters(strParam, 12, pOption->bLockTarget);
   sprintf(strParam, "%s", g_GUILocalString[62]);
   AddGraphparameters(strParam, 13, pOption->bHighFont);
   sprintf(strParam, "%s", g_GUILocalString[63]);
   AddGraphparameters(strParam,14, pOption->bEnableZoom);
   sprintf(strParam,"%s",g_GUILocalString[64]);
   AddGraphparameters(strParam,15, pOption->bHyperChat);
   sprintf(strParam,"%s",g_GUILocalString[115]);
   AddGraphparameters(strParam,16, pOption->bShowItemSpec);
   sprintf(strParam,"%s",g_GUILocalString[171]);
   AddGraphparameters(strParam,17, pOption->bShowNewLife);
   sprintf(strParam,"%s",g_GUILocalString[173]);
   AddGraphparameters(strParam,18, pOption->bOldStatBar);
   sprintf(strParam,"%s",g_GUILocalString[174]);
   AddGraphparameters(strParam,19, pOption->bShowNewOmbrage);
   

   if(pOption->bDisplayStatus)
       EffectStatusUI::GetInstance()->Enable( true );
   else
       EffectStatusUI::GetInstance()->Enable( false );

   //ChatterUI::GetInstance()->SetMinimizedHight(ChatterUI::GetInstance()->IsMinimizedHight());

   if(pOption->bLockResize)
   {
      //on vas remettre le client en taille par defaut...
      RECT rc;
      GetWindowRect(siGethWnd(),&rc);
      int dwHtmp = g_Global.GetScreenH();
      SetWindowPos(siGethWnd(),NULL,rc.left,rc.top,g_Global.GetDisplayW()+8,g_Global.GetDisplayH()+27,NULL);
   }
   graphList.SetScrollPos(dwScrollPos );
}
 
void OptionsUI::UpdateLogList()
{
   CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
   logList.DeleteList();

   OptionParam *pOption = g_SaveGame.GetOptionsParam();
   char strParam[50];
   sprintf(strParam,"%s",g_GUILocalString[48]);
   AddLogParameters(strParam,0, pOption->blogNPC);
   sprintf(strParam,"%s",g_GUILocalString[49]);
   AddLogParameters(strParam,1, pOption->bLogUser);
   sprintf(strParam,"%s",g_GUILocalString[50]);
   AddLogParameters(strParam,2, pOption->bLogEnable);

   logFileName.SetText( ChatterUI::GetInstance()->GetBacklogFile() );
}


void OptionsUI::AddGraphparameters(char *pstrName,int dwID, int dwStatus)
{
   bool bStatus = false;
   if(dwStatus)
      bStatus = true;

   graphList.AddNewItem();
   OptDlgUI *OptUI = new OptDlgUI;
   
   OptUI->SetText( pstrName, RGB(220,220,220), 640, 0, 0 );
   OptUI->SetCenter( false );
   OptUI->SetPos( FramePos( 0, 0, OptUI->GetWidth(), OptUI->GetHeight() ) );
   OptUI->optionID = pstrName;
   OptUI->bState = bStatus;   
   OptUI->dwID   = dwID;
   graphList.AddColumnItem( 0, OptUI );

  
   OptDlgToggleUI *optDlgToggleUI = new OptDlgToggleUI;
   if( bStatus )
   {
      optDlgToggleUI->SetSpriteId   ( "CheckONLightUP" );
      optDlgToggleUI->SetDownUI     ( "CheckONLightDown" );
      optDlgToggleUI->SetHighlightUI( "CheckONLightHUP" );
   }else
   {
      optDlgToggleUI->SetSpriteId   ( "CheckOFFLightUP" );
      optDlgToggleUI->SetDownUI     ( "CheckOFFLightDown" );
      optDlgToggleUI->SetHighlightUI( "CheckOFFLightHUP" );
   }
   //channelToggleUI->SetCenter( false ); 
   optDlgToggleUI->SetPos( FramePos( 1, 0, 15, 16 ) );
   optDlgToggleUI->optionID = pstrName;
   optDlgToggleUI->bState = bStatus;
   graphList.AddColumnItem( 1, optDlgToggleUI );
}

void OptionsUI::AddLogParameters(char *pstrName,int dwID, int dwStatus)
{
   bool bStatus = false;
   if(dwStatus)
      bStatus = true;

   logList.AddNewItem();
   OptDlgUI *OptUI = new OptDlgUI;
   
   OptUI->SetText( pstrName, RGB(220,220,220), 640, 0, 0 );
   OptUI->SetCenter( false );
   OptUI->SetPos( FramePos( 0, 0, OptUI->GetWidth(), OptUI->GetHeight() ) );
   OptUI->optionID = pstrName;
   OptUI->bState = bStatus;   
   OptUI->dwID   = dwID;
   logList.AddColumnItem( 0, OptUI );

  
   OptDlgToggleUI *optDlgToggleUI = new OptDlgToggleUI;
   if( bStatus )
   {
      optDlgToggleUI->SetSpriteId   ( "CheckONLightUP" );
      optDlgToggleUI->SetDownUI     ( "CheckONLightDown" );
      optDlgToggleUI->SetHighlightUI( "CheckONLightHUP" );
   }else
   {
      optDlgToggleUI->SetSpriteId   ( "CheckOFFLightUP" );
      optDlgToggleUI->SetDownUI     ( "CheckOFFLightDown" );
      optDlgToggleUI->SetHighlightUI( "CheckOFFLightHUP" );
   }
   //channelToggleUI->SetCenter( false ); 
   optDlgToggleUI->SetPos( FramePos( 1, 0, 15, 16 ) );
   optDlgToggleUI->optionID = pstrName;
   optDlgToggleUI->bState = bStatus;
   logList.AddColumnItem( 1, optDlgToggleUI );
}

//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::FileNameEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Clicked in the option file name
// 
//////////////////////////////////////////////////////////////////////////////////////////
{   
    OptionsUI *o = static_cast< OptionsUI * >( GetUI() );
    o->fileNamePopup.Show();
    o->fileNamePopup.SetFileName( ChatterUI::GetInstance()->GetBacklogFile() );
    o->RequestForegroundControl( &o->fileNamePopup );
}

//////////////////////////////////////////////////////////////////////////////////////////
OptionsUI::QuitPopup::QuitPopup( void )
//////////////////////////////////////////////////////////////////////////////////////////
:
 okButton( quitEvent ),
 cancelButton( hideEvent )
{
    //Back du popup box...
    int dwOffX = (g_Global.GetScreenW()-240)/2;
    int dwOffY = (g_Global.GetScreenH()-164-150)/2;
    SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );

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
    sprintf(strText,"%s",g_GUILocalString[83]);
    textArea.SetText( strText, RGB( 223, 157, 0 ), 190 );

    RootBoxUI::GetInstance()->AddChild( this );
}

void OptionsUI::QuitPopup::Draw(V2SPRITEFX *vsfFX )
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

int OptionsUI::QuitPopup::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::QuitPopup::QuitEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Quit the game..!
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    T3VSBSound PopupSound;

    PopupSound.CopyWave(&SoundFX[110]);
    PopupSound.Play(FALSE);

    Sleep(10);

   LOG << "* CLS = 8\r\n";
   PostMessage(siGethWnd(), WM_CLOSE, NULL, 0x6666);
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Called when the user selects a channel in the list.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::GraphListEvent::LeftClicked( void )
{
    OptionsUI *optionUI = static_cast< OptionsUI * >( GetUI() );

    int col = optionUI->graphList.GetSelectedColumn();

    string channelId;
    
    // Click in names listing
    if( col == 0 )
    {
        OptDlgUI *optUI = static_cast< OptDlgUI * >( optionUI->graphList.GetSelection() );

        if( optUI == NULL )
           return;
    }
    // Clicked on the toggle button
    else if (col == 1)
    {
        OptDlgUI *optUI = static_cast< OptDlgUI * >( optionUI->graphList.GetSelection() );

        if( optUI == NULL )
           return;

        OptionParam *pOption = g_SaveGame.GetOptionsParam();		

        switch(optUI->dwID)
        {
           case 0 :pOption->bLightHightGraph        = !pOption->bLightHightGraph        ; break;
           case 1 :pOption->bHightGraphEffect       = !pOption->bHightGraphEffect       ; break;
           case 2 :
              pOption->bAnimatedWater          = !pOption->bAnimatedWater; 
              g_Global.ResetAnimWater();
              //g_Global.ResetAnimLava();
              World.Lock(0);
              Objects.Lock(0);
              Objects.SDAnimClear();
              World.SetPosition(Player.xPos, Player.yPos, Player.World, TRUE,FALSE);
              Objects.Unlock(0);
              World.Unlock(0);
           break;
           case 3 :
              pOption->bShowAnimDecorsLight    = !pOption->bShowAnimDecorsLight    ; 
              World.Lock(0);
              Objects.Lock(0);
              World.SetPosition(Player.xPos, Player.yPos, Player.World, TRUE,FALSE);
              Objects.Unlock(0);
              World.Unlock(0);
           break;
		   case 4 :
			   pOption->bShowWeatherEffects	    = !pOption->bShowWeatherEffects;
			
		   break;
           case 5 :pOption->bDitherGraph            = !pOption->bDitherGraph            ; break;
           case 6 :pOption->bGuiAlpha               = !pOption->bGuiAlpha               ; break;
           case 7 :pOption->bSeraphAnim             = !pOption->bSeraphAnim             ; break;
           case 8 :pOption->bDisplayStatus          = !pOption->bDisplayStatus          ; break;
           case 9 :pOption->bDisplayXPBarText       = !pOption->bDisplayXPBarText       ; break;
		   case 20:pOption->bDisplayResistPower		= !pOption->bDisplayResistPower		; break; // steph ajout
		   case 21:pOption->bDisplayWeight			= !pOption->bDisplayWeight			; break; // steph ajout
		   case 22:pOption->bEnableDisplayGold		= !pOption->bEnableDisplayGold		; break; // steph ajout
		   case 10:pOption->bDisplayMacroFullScreen = !pOption->bDisplayMacroFullScreen	; break;
		 //case 11 :pOption->bLockResize		    = !pOption->bLockResize		        ; break; // steph désactivation
           case 12 :pOption->bLockTarget             = !pOption->bLockTarget            ; break;
           case 13:
              pOption->bHighFont               = !pOption->bHighFont               ; 
              ChatterUI::GetInstance()->SetMinimizedHight(ChatterUI::GetInstance()->IsMinimizedHight());
           break;
           case 14: 
              pOption->bEnableZoom               = !pOption->bEnableZoom; 
              g_Global.SetZoomStatus(0);
           break;
           case 15:
              pOption->bHyperChat                = !pOption->bHyperChat; 
              if(g_Global.GetDisplayW() <=640)
                 pOption->bHyperChat = FALSE;

           break;
           case 16:
              pOption->bShowItemSpec             = !pOption->bShowItemSpec; 
           break;
           case 17:
              pOption->bShowNewLife              = !pOption->bShowNewLife; 
           break;
           case 18:
              pOption->bOldStatBar               = !pOption->bOldStatBar; 
           break;
           
           case 19:
              pOption->bShowNewOmbrage           = !pOption->bShowNewOmbrage; 
           break;
           
              
        }
        optionUI->UpdateGraphList();
    }   
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Called when the user selects a channel in the list.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void OptionsUI::LogListEvent::LeftClicked( void )
{
    OptionsUI *optionUI = static_cast< OptionsUI * >( GetUI() );

    int col = optionUI->logList.GetSelectedColumn();

    string channelId;
    
    // Click in names listing
    if( col == 0 )
    {
        OptDlgUI *optUI = static_cast< OptDlgUI * >( optionUI->logList.GetSelection() );

        if( optUI == NULL )
           return;
    }
    // Clicked on the toggle button
    else if (col == 1)
    {
        OptDlgUI *optUI = static_cast< OptDlgUI * >( optionUI->logList.GetSelection() );

        if( optUI == NULL )
           return;

        OptionParam *pOption = g_SaveGame.GetOptionsParam();
        switch(optUI->dwID)
        {
           case 0:pOption->blogNPC        = !pOption->blogNPC        ; break;
           case 1:pOption->bLogUser       = !pOption->bLogUser       ; break;
           case 2:pOption->bLogEnable     = !pOption->bLogEnable     ; break;
        }
        optionUI->UpdateLogList();
    }   
}