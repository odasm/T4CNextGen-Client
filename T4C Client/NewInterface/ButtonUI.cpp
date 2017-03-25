// ButtonUI.cpp
//

#include "../pch.h"

#include "ButtonUI.h"
#include "../MemoryManager.h"

namespace{

    ButtonUI *globalCurrentPressedUI = NULL;

    T3VSBSound *buttonDownSound = NULL;
    T3VSBSound *buttonUpSound = NULL;

    void InitSound(){
        if( !buttonDownSound ){
            buttonDownSound = new T3VSBSound;
            buttonUpSound = new T3VSBSound;

            buttonDownSound->Create( "Generic Drop Item", TS_MEMORY );
            buttonUpSound->Create( "Button Release Sound", TS_MEMORY );
        }

    }
};
 
ButtonUI::ButtonUI( EventVisitor &eventVisitor, std::string theText )
    :   buttonEvent( eventVisitor ),
        buttonText  ( theText ),
        pressed( true ),
        enableDragCycle( false ),
        soundEnabled( true )
{
    InitSound();
}

//////////////////////////////////////////////////////////////////////////////////////////
void ButtonUI::Draw( V2SPRITEFX *vsfFX )
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the button text on top of the normal sprite.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If the button was pressed.
    if( pressed && GameUI::IsLeftMouseDown() && downUI.IsSpriteDefined() ){
        // Draw the down button version.
        downUI.Draw( vsfFX );

        return;
    }else{
        pressed = false;
    }

    // If the mouse is hovering over the button.
    MousePos mousePos( MouseDevice::xPos, MouseDevice::yPos );

    if( GetPos().MousePosInFrame( mousePos ) && selectionUI.IsSpriteDefined() ){
        // Draw the selection UI
        selectionUI.Draw( vsfFX );

        return;
    }

    // Draw the button normally.
    GameUI::Draw( vsfFX );
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ButtonUI::LeftMouseUp( MousePos mousePos )
//////////////////////////////////////////////////////////////////////////////////////////
// Mouse button was released on top of this control.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    InitSound();

    if( IsDblClick() ){
        if( soundEnabled ){
            buttonUpSound->Play( TRUE );
        }
        buttonEvent.LeftDblClicked();
    }else{
        if( soundEnabled ){
            buttonUpSound->Play( TRUE );
        }
        // Notify the button's event.
        buttonEvent.LeftClicked();
    }

    return true;
}

bool ButtonUI::LeftMouseDown( MousePos mousePos )
{
    InitSound();

    if( soundEnabled ){
        buttonDownSound->Play( TRUE );
    }
    pressed = true;
    return true;
}

bool ButtonUI::DragCycle( MousePos mousePos ){
    
    if( enableDragCycle ){
        buttonEvent.LeftClicked();
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ButtonUI::SetPos
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the button's position
// 
(
 FramePos framePos // The position
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Assign the button's position to the down and highlight UIs.
    downUI.SetPos( framePos );
    selectionUI.SetPos( framePos );

    GameUI::SetPos( framePos );
}