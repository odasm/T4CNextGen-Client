#include "../pch.h"
#include "../ScrollUI.h"

namespace{
    const int ScrollUpdate = 100; // 100 ms scroll updates.

};

namespace{

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

//////////////////////////////////////////////////////////////////////////////////////////
void ScrollUI::DrawScroll
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the scrollUI slider
// 
(
 V2SPRITEFX *vsfFX
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( scrollButton == NULL ){
        return;
    }

    if( dragging && GameUI::IsLeftMouseDown() ){
        MousePos mousePos( MouseDevice::xPos, MouseDevice::yPos );
        
        DragScrollButton( mousePos );
    }else{
        dragging = false;
    }
    
    int y;
    if( listSize == 0 ){
        y = freeScrollRegion.y;
    }else{
        int sz = listSize - 1;
        if( sz <= 0 ){
            sz = 1;
        }

        double relPos = static_cast< double >( linePos ) / static_cast< double >( sz );
        y = (int)(relPos * freeScrollRegion.Height() - 
            relPos * ( scrollButton->GetPos().Height() / 2 ) + 
            freeScrollRegion.y);
    }       

    int x = freeScrollRegion.Width() / 2 - scrollButton->GetPos().Width() / 2 + freeScrollRegion.x;
        
    FramePos buttonPos = scrollButton->GetPos();
    buttonPos.SetNewOffset( MousePos( x, y ) );
    scrollButton->SetPos( buttonPos );
    scrollButton->Draw( vsfFX );    
    
    if( scrollUpDownUI != NULL && pressUp && GameUI::IsLeftMouseDown() ){
        scrollUpDownUI->Draw( vsfFX );

        // If its time to upgrade the up scrolling
        if( GetTickCount() >= nextPress ){
            ScrollDown( 1 );

            // 100ms scroll updates
            nextPress = GetTickCount() + ScrollUpdate;
        }

    }else{
        pressUp = false;
    }

    if( scrollDownDownUI != NULL && pressDown && GameUI::IsLeftMouseDown() ){
        scrollDownDownUI->Draw( vsfFX );

        // If its time to upgrade the down scrolling
        if( GetTickCount() >= nextPress ){
            ScrollUp( 1 );

            // 100ms scroll updates
            nextPress = GetTickCount() + ScrollUpdate;
        }

    }else{
        pressDown = false;
    }


}

//////////////////////////////////////////////////////////////////////////////////////////
bool ScrollUI::ScrollClick
//////////////////////////////////////////////////////////////////////////////////////////
//  Clicked on the scroll bar.
// 
(
 MousePos mousePos // The mouse position
)
// Return: bool, true if the position clicked on the scroll bar.
//////////////////////////////////////////////////////////////////////////////////////////
{
    InitSound();
    if( scrollUpRegion.MousePosInFrame( mousePos ) ){
        nextPress = GetTickCount() + ScrollUpdate;
        
        buttonDownSound->Play( TRUE );
        
        pressUp = true;
        ScrollDown( 1 );
    }else if( scrollDownRegion.MousePosInFrame( mousePos ) ){
        nextPress = GetTickCount() + ScrollUpdate;

        buttonDownSound->Play( TRUE );

        pressDown = true;
        ScrollUp( 1 );
    }else if( freeScrollRegion.MousePosInFrame( mousePos ) ){
        // If a scroll button was defined.
        if( scrollButton != NULL ){
            // If the mouse hit it.
            if( scrollButton->GetPos().MousePosInFrame( mousePos ) ){
                // Start dragging this button.
                dragging = true;
            }else{
                // If the mouse hit on top of the button.
                if( mousePos.y < scrollButton->GetPos().y ){
                    // Do a pagedown.
                    ScrollDown( 5 );
                }
                // Otherwise it must have hit the bottom (it would have hit the button otherwise).
                else{
                    // Do a pageup.
                    ScrollUp( 5 );
                }            
            }
        }
    }else{
        return false;
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ScrollUI::IsMouseInScrollBar
//////////////////////////////////////////////////////////////////////////////////////////
//  Clicked on the scroll bar.
// 
(
 MousePos mousePos // The mouse position
)
// Return: bool, true if the position clicked on the scroll bar.
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( scrollUpRegion.MousePosInFrame( mousePos ) || 
        scrollDownRegion.MousePosInFrame( mousePos ) ||
        freeScrollRegion.MousePosInFrame( mousePos ) ){
        return true;
    }
    return false;
}


//////////////////////////////////////////////////////////////////////////////////////////
void ScrollUI::DragScrollButton
//////////////////////////////////////////////////////////////////////////////////////////
//  Drags the scroll button to its new location.
// 
(
 MousePos mousePos // The mouse position
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If the cursor is not in y range.
    if( mousePos.y < freeScrollRegion.y || mousePos.y >= freeScrollRegion.maxy ){
        return;
    }

    // Calculate the relative relative position of the cursor.
    double relPos = static_cast< double >( mousePos.y - freeScrollRegion.y ) / 
                    static_cast< double >( freeScrollRegion.Height() );

    linePos = (unsigned long)(relPos * listSize);

    ScrollChanged();
}

//////////////////////////////////////////////////////////////////////////////////////////
void ScrollUI::SetScrollUpDownUI
//////////////////////////////////////////////////////////////////////////////////////////
// Sets the upper arrow down graphic
// 
(
 GameUI *newUI // 
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    scrollUpDownUI = newUI;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ScrollUI::SetScrollDownDownUI
//////////////////////////////////////////////////////////////////////////////////////////
// Sets the upper arrow down graphic
// 
(
 GameUI *newUI // 
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    scrollDownDownUI = newUI;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ScrollUI::SetScrollUI
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the scroll UI.
// 
(
 GraphUI *newUI // 
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    scrollButton = newUI;
}
