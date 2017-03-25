// EffectStatusUI.cpp
//
#include "../pch.h"
#include "EffectStatusUI.h"
#include "../MemoryManager.h"
#include "RootBoxUI.h"
#include "SideMenu.h"
#include "SpellUI.h"
#include "ChatterUI.h"
#include <stdlib.h>
#include "..\SaveGame.h"

extern CSaveGame g_SaveGame;

void FormatChargesLeft( char *buf, LPBAG_ITEM bagItem );

// Constructor ///////////////////////////////////////////////////////////////////////////
EffectStatusUI::EffectStatusUI( void )
:
 scrollEnabled( false ),
 enabled( true ),
 displayedEffects( 7 ),
 scrollUpButton( scrollUpEvent ),
 scrollDownButton( scrollDownEvent )
//////////////////////////////////////////////////////////////////////////////////////////
{  
    const int x = 65;
    const int y = 30;

    SetPos( FramePos( x, y, x + 49, y + 42 ) );

    // 27x 17y
    scrollUpButton.SetSpriteId( "64kStatusBarUp" );
    scrollUpButton.SetPos( FramePos( 11, y - 17, 38, y ) );
    scrollUpEvent.SetUI( this );

    scrollUpButton.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
    
    scrollDownButton.SetSpriteId( "64kStatusBarDown" );
    scrollDownButton.SetPos( 
        FramePos( 
            11,
            y + 42 * ( displayedEffects.size() ),
            38,
            y + 42 * ( displayedEffects.size() + 1 ) 
        )
    );
    scrollDownButton.SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE) );
    scrollDownEvent.SetUI( this );

    // Add all displayed effects
    UINT i;
    for( i = 0; i < displayedEffects.size(); i++ ){
        displayedEffects[ i ].SetPos( 
            FramePos( 
                x, 
                y + ( 42 * i ), 
                x + 49, 
                y + (42 * (i + 1) ) 
            )
        );

        displayedEffects[ i ].SetSpriteId( "64kStatusBackGround" );
        displayedEffects[ i ].SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, "" ) );
        displayedEffects[ i ].emptyGauge.SetSpriteId( "64kStatusEmptyBar" );
        AddChild( &displayedEffects[ i ] );
    }

    Show();

    scrollUpButton.Hide();
    scrollDownButton.Hide();

    AddChild( &scrollUpButton );
    AddChild( &scrollDownButton );
    RootBoxUI::GetInstance()->AddChild( this );
    // This UI is always minimized
    Minimize( true );
}

//////////////////////////////////////////////////////////////////////////////////////////
EffectStatusUI *EffectStatusUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the status UI's instance
//////////////////////////////////////////////////////////////////////////////////////////
{
    static EffectStatusUI instance;
    return &instance;
}

//////////////////////////////////////////////////////////////////////////////////////////
void EffectStatusUI::AddEffect
//////////////////////////////////////////////////////////////////////////////////////////
//  Adds an effect to the status UI.
// 
(
 DWORD id,          // A unique identifier for the effect.
 DWORD time,        // The time which the effect lasts.
 DWORD totalTime,   // Total duration of effect
 DWORD iconId,      // The icon to display.
 std::string desc   // The effect's description.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( RootBoxUI::GetInstance()->GetLock() );

    if( time == 0 ){
        return;
    }

    if( totalTime < time ){
        totalTime = time;
    }

    // Try to find a corresponding effect in the effects list.
    list< Effect >::iterator i;
    for( i = effects.begin(); i != effects.end(); i++ ){
        // If one was found
        if( (*i).id == id ){
            if( time == 0xFFFFFFFF ){
                (*i).infinite = true;
                (*i).expireTime = 0xFFFFFFFF;
            }else{
                (*i).infinite = false;
                (*i).expireTime = GetTickCount() + time;
            }
            // Update its expireTime;            
            (*i).initialTime = totalTime;

            CalcVisibleEffects();
            
            return;
        }
    }

    Effect effect;

    effect.id = id;
    if( time == 0xFFFFFFFF ){
        effect.infinite = true;
        effect.expireTime = 0xFFFFFFFF;
    }else{
        effect.infinite = false;
        effect.expireTime = GetTickCount() + time;
    }
    
    effect.initialTime = totalTime;
    effect.desc = desc;
    effect.SetVSF( SpellIcons( iconId ) );
    effect.SetPos( FramePos( 0, 0, effect.GetVSF()->GetWidth(), effect.GetVSF()->GetHeight() ) );

    effects.push_back( effect );
    
    int size = effects.size() - displayedEffects.size();
    if( size > 0 ){
        scrollEnabled = true;
        SetListSize( size + 1 );
    }else{
        scrollEnabled = false;
        SetListSize( 0 );
    }


    CalcVisibleEffects();
}
//////////////////////////////////////////////////////////////////////////////////////////
void EffectStatusUI::DispellEffect
//////////////////////////////////////////////////////////////////////////////////////////
//  Dispells a given effect.
// 
(
 DWORD id // The id of the effect to dispell.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( RootBoxUI::GetInstance()->GetLock() );

    list< Effect >::iterator i;
    for( i = effects.begin(); i != effects.end(); i++ ){
        if( (*i).id == id ){
            effects.erase( i );
            break;
        }
    }

    int size = effects.size() - displayedEffects.size();
    if( size > 0 ){
        scrollEnabled = true;
        SetListSize( size + 1 );
    }else{
        scrollEnabled = false;
        SetListSize( 0 );
    }

    CalcVisibleEffects();
}

//////////////////////////////////////////////////////////////////////////////////////////
void EffectStatusUI::CalcVisibleEffects( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Calculates which effects are currently visibible on screen and setups the 
//  displayedEffects vector accordingly.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // First sort the effect list.
    effects.sort();

    DWORD j, ySize = 0;
    list< Effect >::iterator i;
    // Move the the scroll offset
    i = effects.begin();
    j = 0;
    while( i != effects.end() && j < linePos ){
        i++;
        j++;
    }
    LOG << "\r\nLinePOs=" << linePos << "..!";

    // Setup the displayable effects while there is displayable space.
    j = 0;
    while( i != effects.end() && j < displayedEffects.size() ){
        // Point the displayed effect to this effect.
        displayedEffects[ j ].effect = &(*i);

        ySize += 42;
        j++;
        i++;
    }

    // All non-displayed effects
    for( ; j < displayedEffects.size(); j++ ){
        displayedEffects[ j ].effect = NULL;
    }

    DWORD yOffset = 30;

    
    if( linePos == 0 )
    {
        scrollUpButton.Hide();
    }
    else
    {
        scrollUpButton.Show();
        yOffset -= 17;
    }

    LOG.Lock();
    LOG << "\r\nListSize=" << GetListSize() << "..!";
    LOG.Unlock();

    if( linePos >= GetListSize() - 1 || GetListSize() == 0 )
    {
        scrollDownButton.Hide();
    }
    else
    {
        ySize += 17;
        scrollDownButton.Show();
    }

    FramePos thisPos = GetPos();
    thisPos.maxy = 30 + ySize;
    thisPos.y = yOffset;
    SetPos( thisPos );
}

//////////////////////////////////////////////////////////////////////////////////////////
void EffectStatusUI::CalcEffectInfo( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the effect information
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    DWORD curTick = GetTickCount();

    list< DWORD > expireList;

    // Check for any expired effects    
    list< Effect >::iterator k;
    for( k = effects.begin(); k != effects.end(); k++ ){
        if( !(*k).infinite && curTick >= (*k).expireTime ){
            expireList.push_back( (*k).id );            
        }
    }
    // Dispell all effects which have expired.
    list< DWORD >::iterator l;
    for( l = expireList.begin(); l != expireList.end(); l++ ){
        DispellEffect( *l );
    }


    // Update the displayed effects info.
    UINT i;
    for( i = 0; i < displayedEffects.size(); i++ ){
        EffectDisplay *e = &displayedEffects[ i ];
        
        if( e->effect == NULL ){
            continue;
        }

        // If the information is currently being displayed
        if( e->IsHelpDisplayed() ){
            // Update it.
            string helpText = e->effect->desc;

            // Get the remaining time in friendly format.
            int secondsRemaining = ( e->effect->expireTime - curTick ) / 1000;
    		
            int sec, min, hour;
	    	sec   = (secondsRemaining % 60);
		    min   = (secondsRemaining % 3600) / 60;
		    hour  = (secondsRemaining % 86400) / 3600;

            char buf[ 256 ];

            // INTL
            helpText += g_LocalString[ 442 ];

            if( e->effect->infinite ){
                helpText += g_LocalString[ 531 ];
            }else{
                if( hour != 0 ){
                    sprintf( buf, "%02u:", hour );
                    helpText += buf;
                }

                if( hour != 0 || min != 0 ){
                    sprintf( buf, "%02u:", min );
                    helpText += buf;
                }
                if( hour != 0 || min != 0 || sec != 0 ){
                    sprintf( buf, "%02u", sec );
                    helpText += buf;
                }
            }

            e->GetHelpText()->SetText( helpText.c_str() );
            e->ResyncHelpText();
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool EffectStatusUI::LeftMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
//  Left mouse down on the status UI (check for scroll bar)
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( !scrollEnabled ){
        return false;
    }

    // Click the scrollbar.
    if( ScrollClick( mousePos ) ){
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
void EffectStatusUI::ScrollChanged( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  If the scroll state changed
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    LOG << "\r\nSCROLL CHANGED!!!";

    // Recalculate the visible items
    CalcVisibleEffects();
}

//////////////////////////////////////////////////////////////////////////////////////////
void EffectStatusUI::DrawMinimized
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the effect status UI.
//
(
 V2SPRITEFX *vsfFX
)
//////////////////////////////////////////////////////////////////////////////////////////
{
   if(g_Global.GetScreenW() == 640 && ChatterUI::GetInstance()->IsMinimizedHight() )
      return;

   if(SideMenu::GetInstance()->GetStartOffsetX()  <0)
      return; 
   const int x = 65;
   const int y = 30;
   
   if( displayedEffects.size() != (unsigned int)SideMenu::GetInstance()->GetStartOffsetX() )
   {
      for(unsigned int k=0;k<displayedEffects.size();k++)
         RemoveChild(&displayedEffects[k]);
      displayedEffects.clear();
      displayedEffects.resize(SideMenu::GetInstance()->GetStartOffsetX());
      scrollDownButton.SetPos( FramePos( 11,y + 42 * ( displayedEffects.size() ),
                                         38,y + 42 * ( displayedEffects.size() + 1 ) ));
      for(unsigned int i = 0; i < displayedEffects.size(); i++ )
      {
         displayedEffects[ i ].SetPos( FramePos( x, y + ( 42 * i ), x + 49, y + (42 * (i + 1) ) ));
         displayedEffects[ i ].SetSpriteId( "64kStatusBackGround" );
         displayedEffects[ i ].SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, "" ) );
         displayedEffects[ i ].emptyGauge.SetSpriteId( "64kStatusEmptyBar" );
         AddChild( &displayedEffects[ i ] );
      }
      CalcVisibleEffects();
   } 
    // Calculate effect info.
    CalcEffectInfo();

    if( !enabled ){
        LOG << "\r\nEffectStatus disabled.";

        return;
    }

    FramePos thisPos = GetPos();
    FramePos scrollUpPos = scrollUpButton.GetPos();
    FramePos scrollDownPos = scrollDownButton.GetPos();

    if( SideMenu::GetInstance()->IsShown() )
    {
        thisPos.x          = 0;
        thisPos.maxx       = 49;
        scrollDownPos.x    = 11;
        scrollDownPos.maxx = 38;
        scrollUpPos.x      = 11;
        scrollUpPos.maxx   = 38;        
    }
    else
    {
        thisPos.x          = 0;
        thisPos.maxx       = 49;
        scrollDownPos.x    = 11;
        scrollDownPos.maxx = 38;
        scrollUpPos.x      = 11;
        scrollUpPos.maxx   = 38;        
    }

/*    LOG.Lock();
    LOG << "\r\nEffectStatus::ThisPos " << thisPos.x << ", " << thisPos.y << ".";
    LOG.Unlock();*/

    SetPos( thisPos );
    scrollUpButton.SetPos( scrollUpPos );
    scrollDownButton.SetPos( scrollDownPos );

    scrollUpButton.Draw( vsfFX );
    scrollDownButton.Draw( vsfFX );

    // Draw all displayed effects
    UINT i;
    for( i = 0; i < displayedEffects.size(); i++ ){
        displayedEffects[ i ].Draw( vsfFX );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the displayed effect status.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void EffectStatusUI::EffectDisplay::Draw(V2SPRITEFX *vsfFX)
{
   if(ChatterUI::GetInstance()->IsMinimizedHight() && g_SaveGame.GetOptionsParam()->bHyperChat)
      return; //dont draw TMI in hyper chat mode...
   if(g_Global.GetScreenW() == 640 && ChatterUI::GetInstance()->IsMinimizedHight() )
      return;

    if( effect != NULL ){
        FramePos thisPos = GetPos();
        
        // If the side menu is on.
        if( SideMenu::GetInstance()->IsShown() )
        {
           thisPos.x    = 0;
           thisPos.maxx = 49;
        }
        else
        {
            thisPos.x    = 0;
            thisPos.maxx = 49;
        }
        SetPos( thisPos );


        GameUI::Draw( vsfFX );
        
        // Center the icon
        FramePos itemPos = effect->GetPos();
        itemPos.SetNewOffset( 
            MousePos( 
                ( thisPos.x + 8 ) - itemPos.Width() / 2 
                  + ( thisPos.Width() - 8 ) / 2,
                thisPos.y - itemPos.Height() / 2
                  + thisPos.Height() / 2
            )
        );

        effect->SetPos( itemPos );

        // Draw the effect's icon
        bool drawIcon = true;
        DWORD curTick = GetTickCount();
    
        // If there is less than 15 seconds remaining for the effect.
        if( ( effect->expireTime - curTick ) / 1000 <= 15 ){
            // Make it blink (undisplayed 300ms per second).        
            if( ( effect->expireTime - curTick ) % 1000 < 300 ){
                drawIcon = false;    
            }
        }

        if( drawIcon ){
            // Draw the icon.
            effect->Draw( vsfFX );
        }
       
        const int gaugeHeight = 39;

        // Query the emptyGauge's total size.
        int emptyGaugeSize = gaugeHeight -
            ( effect->expireTime - curTick ) * gaugeHeight / effect->initialTime;

        if( effect->infinite ){
            emptyGaugeSize = 0;
        }

        // Set the gauge's pos.
        emptyGauge.SetPos( FramePos( thisPos.x + 2, thisPos.y + 1, thisPos.maxx, thisPos.maxy ) );

        // Clip the empty bar's according to its current size.
        RECT clipRect = { 
            thisPos.x, 
            thisPos.y, 
            thisPos.maxx, 
            thisPos.y + 1 + emptyGaugeSize
        };

        V2SPRITEFX fx;
        memset( &fx, 0, sizeof( fx ) );

        fx.lpClipRect = &clipRect;

        emptyGauge.Draw( &fx );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void EffectStatusUI::ScrollUpEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    EffectStatusUI *e = static_cast< EffectStatusUI * >( GetUI() );    
    
    e->ScrollDown( 1 );
    LOG.Lock();
    LOG << "\r\nScroll up clicked, linePos=" << e->linePos << ", listSize=" << e->GetListSize() << "..!";
    LOG.Unlock();
}

//////////////////////////////////////////////////////////////////////////////////////////
void EffectStatusUI::ScrollDownEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    EffectStatusUI *e = static_cast< EffectStatusUI * >( GetUI() );

    e->ScrollUp( 1 );

    LOG.Lock();
    LOG << "\r\nScroll down clicked, linePos=" << e->linePos << ", listSize=" << e->GetListSize() << "..!";
    LOG.Unlock();
}

//////////////////////////////////////////////////////////////////////////////////////////
bool EffectStatusUI::EffectDisplay::LeftMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Checks for double-clicks on the status effects
// 
(
 MousePos mousePos // Mouse pos
)
// Return: virtual bool, true
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( !IsDblClick() || effect == NULL ){
        return true;
    }
    
    std::string step = "Before try";
    
    LOG.Lock();
    LOG << "\r\nCasting effect " << effect->id << ".";
    LOG.Unlock();
    
   
    if( !SpellUI::GetInstance()->CastSpell( effect->id, true ) ){
        LOG << "\r\nSpell Casting failed.";
        
        // This is a way to re-use an item with the information avaiable on the client side.
        // This is not a perfect system and should be removed if it cause any troubles.
        
        // Get the name of the item from its description.
        UINT pos = effect->desc.find(":",0);
        std::string fxName = effect->desc;
        if( pos < fxName.size() ){                
            fxName[pos] = 0;
        }else{
            fxName = "";
        }
               
        // Compare the name of the FX to the name of the objects in the backpack.
        // If it finds a match, it use the object.
        bool bF = false;
        Player.tlBackpack.Lock("Lock backpack");
        Player.tlBackpack.ToHead();
        while (Player.tlBackpack.QueryNext()) {
            
            BAG_ITEM *obj = Player.tlBackpack.GetObject();
            if (!strcmp(fxName.c_str(), (char *)obj->chName)) {
                TFCPacket sending;
                sending << (RQ_SIZE)RQ_UseObject;
                sending << (short)0;
                sending << (short)0;
                sending << (long)( obj->dwID );
               
                // send the use packet
                SEND_PACKET( sending );
                Player.tlBackpack.SetQueryState(STOP_QUERY);
                bF = true;
                
                char buf[ 256 ];
                FormatChargesLeft( buf, obj );
                
                ChatterUI::GetInstance()->AddBackscrollItem( 
                    "", 
                    buf, 
                    SYSTEM_COLOR,
                    true
                );
            }
        }
        Player.tlBackpack.Unlock("Lock backpack");
        
        if (!bF) {              
            ChatterUI::GetInstance()->AddBackscrollItem( "", g_LocalString[ 376 ], SYSTEM_COLOR, true );
        }
    }
    
    return true;
}