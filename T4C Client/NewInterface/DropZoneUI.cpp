// DropZoneUI.cpp
//
#include "../pch.h"

#include "DropZoneUI.h"
#include "EventVisitor.h"

#include "InventoryUI.h"
#include "../MemoryManager.h"

DropZoneUI::DropZoneUI( EventVisitor &eventVisitor )
    :   dropzoneEvent( eventVisitor ),
        currentUI( NULL )
{
}

//////////////////////////////////////////////////////////////////////////////////////////
void DropZoneUI::Draw
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the dropzone item at its place.
// 
(
 V2SPRITEFX *vsfFX
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Draw the dropzone.
    GameUI::Draw( vsfFX );
                    
    // If there is a dropped item.
    if( currentUI != NULL ){
        V2SPRITEFX fx;
        memset( &fx, 0, sizeof( fx ) );

        // Setup the clipping rectangle
        RECT rect = { GetPos().x, GetPos().y, GetPos().maxx, GetPos().maxy };
        fx.lpClipRect = &rect;
    
        // Specify no correction.
        fx.dwFX |= FX_NOCORRECTION;

        FramePos uiPos = currentUI->GetPos();
        // Setup a new offset to the ui's frame.
        uiPos.SetNewOffset(
            MousePos(
                GetPos().Width()  / 2 - currentUI->GetPos().Width()  / 2 + GetPos().x,
                GetPos().Height() / 2 - currentUI->GetPos().Height() / 2 + GetPos().y
            )
        );

        // Set the UI's position.
        currentUI->SetPos( uiPos );
        
        // Draw the UI.
        currentUI->Draw( &fx );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void DropZoneUI::DrawHelp
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws help
// 
(
 V2SPRITEFX *vsfFX
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If there is a UI sitting in the dropzone.
    if( currentUI != NULL ){
        // Display its help text.
        currentUI->DrawHelp( vsfFX );
    }else{
        // Display the dropzone's help text.
        GameUI::DrawHelp( vsfFX );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool DropZoneUI::LeftMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
//  Left mouse down.
// 
(
 MousePos mousePos // Mouse position.
)
// Return: bool, true.
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If this is a double click
    if( IsDblClick() )
    {
        dropzoneEvent.LeftDblClicked();
    }
    else
    {
        if( GetDragItem() == NULL )
        {
            // Drag.
            DragItem( mousePos );

        }
        else
        {
            // Drop.
            DropItem( mousePos );
        }
    }
        
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool DropZoneUI::LeftMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Left mouse button was released on this control
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If this is not a click, and there is a dragged item.
    if( !IsClick() && GetDragItem() != NULL ){
        // Drop.
        DropItem( mousePos );
    }    
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool DropZoneUI::RightMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Help click on drop zone.
// 
(
 MousePos mousePos
)
// Return: bool, true
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If there is no current UI.
    if( currentUI == NULL ){
        // Display the dropzone's help text.
        DisplayHelp( true, MouseDevice::xPos - 2, MouseDevice::yPos - 2 );
    }else{
        // Display the ui's help text.
        currentUI->DisplayHelp( true, MouseDevice::xPos - 2, MouseDevice::yPos - 2 );
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void DropZoneUI::DragItem
//////////////////////////////////////////////////////////////////////////////////////////
//  Drags an item from the drop zone.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If there is no current UI, we cannot drag from the drop zone.
    if( currentUI == NULL ){
        return;
    }

    // Verify that the mouse points inside the UI.
    if( !currentUI->GetPos().MousePosInFrame( mousePos ) ){       
        return;
    }
    if( !dropzoneEvent.CanDrag( currentUI ) ){
        return;
    }
    
    SetDragItem( currentUI, &dropzoneEvent, this, mousePos );

    // Create a custom cursor with the ui's graphic.
    SetUIMouseCursor( currentUI, 2, 2 );

    // No more current UI.
    currentUI = NULL;

    dropzoneEvent.Dragged();
}


//////////////////////////////////////////////////////////////////////////////////////////
void DropZoneUI::DropItem
//////////////////////////////////////////////////////////////////////////////////////////
//  Drops an item into the dropzone.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{        
    // If the currentUI is non existent and the event accepts the drop.
    if( currentUI == NULL && dropzoneEvent.Dropping() ){

        // Set the currentUI as being the dragged item.
        currentUI = GetDragItem();

        // Notify the source event that the item was successfully dropped.
        if( GetDragSourceEvent != NULL ){
            GetDragSourceEvent()->Dropped( &dropzoneEvent );
        }

        // Reset mouse cursor.
        CMouseCursor::GetInstance()->SetCustomCursor( NULL );

        // Cancel drag mode.
        CancelDrag();
    }
}