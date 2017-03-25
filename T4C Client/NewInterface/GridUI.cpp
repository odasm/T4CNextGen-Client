// GridUI.cpp
//
#include "../pch.h"
#include "GridUI.h"
#include "EventVisitor.h"
#include <algorithm>
#include "../MemoryManager.h"

using namespace std;

// Constructor ///////////////////////////////////////////////////////////////////////////
 GridUI::GridUI( EventVisitor &eventVisitor )
//////////////////////////////////////////////////////////////////////////////////////////
    : gridEvent( eventVisitor ),
      dragAllowed( true ),
      scrollAllowed( true ),
      allowHelp(true),
      selectedTileUI( NULL ),
      selectedLockedTileUI( NULL ),
      currentSelection( NULL )
{
}

//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::Draw( V2SPRITEFX *vsfFX )
//////////////////////////////////////////////////////////////////////////////////////////
// Draws all items in the grid.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Setup the V2SPRITEFX for all sprites.
    V2SPRITEFX fx;

    memset( &fx, 0, sizeof( fx ) );

    // Setup the clipping rectangle
    RECT rect = { GetPos().x, GetPos().y, GetPos().maxx, GetPos().maxy };
    fx.lpClipRect = &rect;
    
    // Specify no correction.
    fx.dwFX |= FX_NOCORRECTION;

    // If a UI was provided to display the selected tiles.
    if( selectedTileUI != NULL ){               
        // If the mouse position is currently over the grid.
        MousePos mousePos( MouseDevice::xPos, MouseDevice::yPos );

        if( GetPos().MousePosInFrame( mousePos ) ){
            // Get the targetted grid spot.
            GridPos gridPos = GetGridPosFromMouse( mousePos ); 

            FramePos uiPos = selectedTileUI->GetPos();

            // If the grid spot is valid
            if( gridPos.x >= 0 && gridPos.x < grid.size() &&
                gridPos.y >= 0 && gridPos.y < grid[ 0 ].size() &&
                !grid[ gridPos.x ][ gridPos.y ].empty()
            ){
                // Get the targetted UI.
                GameUI *targetUI = grid[ gridPos.x ][ gridPos.y ].ui;

                // Scan the viewable grid.
                UINT i, j;
                for( i = 0; i < grid.size(); i++ ){
                    for( j = linePos; j < linePos + ySize && j < grid[ 0 ].size(); j++ ){
                        if( grid[ i ][ j ].ui == targetUI ){
                            // Print the tile.
                            uiPos.SetNewOffset(
                                MousePos(
                                    i * ( squareSize.Width()  + gridSpacing.Width()  ) + GetPos().x,
                                    ( j - linePos ) * ( squareSize.Height() + gridSpacing.Height() ) + GetPos().y
                                )
                            );

                            selectedTileUI->SetPos( uiPos );
                            selectedTileUI->Draw( vsfFX );
                        }
                    }
                }
            }
            // If the grid at the mouse pos is not empty.
            else{
                // If we have a drag item.
                if( GetDragItem() != NULL ){
                    GameUI *dragItem = GetDragItem();
                    // Correct the mouse pos to take into account the item's size.
                    MousePos correctedMousePos(
                        mousePos.x - dragItem->GetPos().Width()  / 2,
                        mousePos.y - dragItem->GetPos().Height() / 2
                    );

                    // Get the position according the the item's top left corner.
                    GridPos gridPos = GetGridPosFromMouse( correctedMousePos );    

                    // Get the item size in grid squares.
                    unsigned long itemGridSizeX = ( dragItem->GetPos().Width()  / ( squareSize.Width()  + gridSpacing.Width()  ) ) + 1;
                    unsigned long itemGridSizeY = ( dragItem->GetPos().Height() / ( squareSize.Height() + gridSpacing.Height() ) ) + 1;

                    // If the item can fit in the grid.
                    if( itemGridSizeX + gridPos.x <= grid.size() ){
                        // Scan the grid occupied by the item.
                        int i, j;
                        for( i = gridPos.x; i < gridPos.x + itemGridSizeX; i++ ){
                            for( j = gridPos.y; j < gridPos.y + itemGridSizeY && j < ySize + linePos; j++ ){
                                // Print the tile.
                                uiPos.SetNewOffset(
                                    MousePos(
                                        i * ( squareSize.Width()  + gridSpacing.Width()  ) + GetPos().x,
                                        ( j - linePos ) * ( squareSize.Height() + gridSpacing.Height() ) + GetPos().y
                                    )
                                );
                                
                                selectedTileUI->SetPos( uiPos );
                                selectedTileUI->Draw( vsfFX );
                            }
                        }
                    }
                }
            }
        }
    }

     // Scroll through all items in the grid.
    list< GridItem >::reverse_iterator i;
    for( i = gridItems.rbegin(); i != gridItems.rend(); i++ ){
        FramePos uiPos = (*i).ui->GetPos();

        // Setup a new offset to the ui's frame.
        uiPos.SetNewOffset(
            MousePos(
                (*i).gridPos.x * ( squareSize.Width() + gridSpacing.Width() ) + GetPos().x + (*i).xOffset,
                ( (*i).gridPos.y * squareSize.Height() + (*i).gridPos.y * gridSpacing.Height() ) + GetPos().y -
                    ( linePos * ( squareSize.Height() + gridSpacing.Height() ) ) + (*i).yOffset
            )
        );

        // Set the ui's new drawing pos.
        (*i).ui->SetPos( uiPos );
                

        // If this is the current selection and a locked selection tile was provided.
        if( (*i).ui == currentSelection && selectedLockedTileUI != NULL ){
            // Scan the viewable grid.
            int i, j;
            for( i = 0; i < grid.size(); i++ ){
                for( j = linePos; j < linePos + ySize && j < grid[ 0 ].size(); j++ ){
                    if( grid[ i ][ j ].ui == selectedLockedTileUI ){
                        // Print the tile over each tiles covered by the grid item.
                        uiPos.SetNewOffset(
                            MousePos(
                                i * ( squareSize.Width()  + gridSpacing.Width()  ) + GetPos().x,
                                ( j - linePos ) * ( squareSize.Height() + gridSpacing.Height() ) + GetPos().y
                            )
                        );

                        selectedLockedTileUI->SetPos( uiPos );
                        selectedLockedTileUI->Draw( vsfFX );
                    }
                }
            }
        }
        // Draw the UI.
        (*i).ui->Draw( &fx );
    }

    DrawScroll( vsfFX );
}

//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::DrawHelp
//////////////////////////////////////////////////////////////////////////////////////////
//  Overrides DrawHelp
// 
(
 V2SPRITEFX *vsfFX // Clipping info.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Draw the grid's help text.
    GameUI::DrawHelp( vsfFX );

    // Draw all the grid's children help texts.
    list< GridItem >::iterator i;
    for( i = gridItems.begin(); i != gridItems.end(); i++ ){
        (*i).ui->DrawHelp( vsfFX );
    }

}
//////////////////////////////////////////////////////////////////////////////////////////
bool GridUI::WheelUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Wheel up.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( scrollAllowed ){
        ScrollUp( 1 );
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool GridUI::WheelDown
//////////////////////////////////////////////////////////////////////////////////////////
//  Wheel down.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( scrollAllowed ){
        ScrollDown( 1 );
    }

    return true;
}

bool GridUI::RightMouseDown( MousePos mousePos )
{
    // Set the new selection
    currentSelection = GetGridItemAt( mousePos );

    return true;
}

bool GridUI::RightMouseUp( MousePos mousePos )
{
    // If this is a right mouse click.
    if( IsClick() )
    {
        // Get the grid pos from the mouse pos.
        GridPos gridPos = GetGridPosFromMouse( mousePos );

        LOG.Lock();
        LOG << "\r\nRIGHT CLICK ON GRID " << gridPos.x << ", " << gridPos.y << " grid size: " << grid.size() << ", " << grid[ 0 ].size();
        LOG.Unlock();

        // If the mouse click fits in the grid.
        if( gridPos.x < grid.size() && gridPos.y < grid[ 0 ].size() )
        {
            
            // If the grid pos is empty.
            if( grid[ gridPos.x ][ gridPos.y ].empty() )
            {
                LOG << "\r\nDISPLAYING THE GRID'S HELP";
                
                // Display the grid's help.
                DisplayHelp( true, mousePos.x, mousePos.y );
            }
            else
            {
                LOG << "\r\nDISPLAYING THE TARGET UI'S HELP";
                
                // Display the target UI's help.
                if(allowHelp)
                   grid[ gridPos.x ][ gridPos.y ].ui->DisplayHelp( true, mousePos.x, mousePos.y );                
                
                // Send the right mouse up message to the target UI.
                grid[ gridPos.x ][ gridPos.y ].ui->RightMouseUp( mousePos );
            }
        }else{
            LOG << "\r\nDISPLAYING GRID'S HELP2";
            
            // Display the target UI's help.
            DisplayHelp( true, mousePos.x, mousePos.y );                
        }        
    }


    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool GridUI::LeftMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
// Left mouse was downed on top of the grid.
// 
(
 MousePos mousePos // The mouse position.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( ScrollClick( mousePos ) ){
        return true;
    }


    // If this would be a double click.
    if( IsDblClick() )
    {   
        // Use item.
        DblClickItem( mousePos );
    }// Otherwise its a single mouse down.
    else{
        // If there isn't already a dragged item.
        if( GetDragItem() == NULL )
        {
            // Grab the referenced object from the grid.            
            DragItem( mousePos );
        }
        else
        {
            // Drop the item.
            DropItem( mousePos );
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool GridUI::LeftMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Left mouse was released on top of the grid.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( IsMouseInScrollBar( mousePos ) ){
        return true;
    }

    // If mouse was clicked, this means the user doesn't want to stop dragging. So if not clicked...
    if( !IsClick() ){
        // Check if there was a drag item.
        if( GetDragItem() != NULL ){            
            LOG << "\r\nGRID DROP";
            
            // Drop the dragged item.                
            DropItem( mousePos );
        }
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
MousePos GridUI::AddItem
//////////////////////////////////////////////////////////////////////////////////////////
// Adds an item to the grid box. Searches for the best spot to put the item in.
// This function cannot be used to put an item at a given spot.
// 
(
 GameUI *displayItem // The item to add.
)
// Return: MousePos, The new position of the grid item.
//////////////////////////////////////////////////////////////////////////////////////////
{
    bool found = false;
    int  foundX = 0, foundY = 0;
   
    // Determine the spacing, in grid squares, that the item will take.
    unsigned long itemGridSizeX = ( displayItem->GetPos().Width()  / ( squareSize.Width()  + gridSpacing.Width()  ) ) + 1;
    unsigned long itemGridSizeY = ( displayItem->GetPos().Height() / ( squareSize.Height() + gridSpacing.Height() ) ) + 1;

    LOG.Lock();
    LOG << "\r\nItem grid spacing " << itemGridSizeX << ", " << itemGridSizeY;
    LOG << "\r\nGrid size: " << grid.size() << ", " << grid[ 0 ].size();
    LOG.Unlock();

    // Scroll through all the grid.
    int  i, j;
    for( j = 0; j < grid[ 0 ].size(); j++ ){
        for( i = 0; i < grid.size(); i++ ){
            // If we hit an empty spot.
            // And the desired spot wouldn't mean putting the item out of the grid.
            if( grid[ i ][ j ].empty() && 
                i + itemGridSizeX <= grid.size()      &&
                j + itemGridSizeY <= grid[ i ].size()               
             ){
                // By default we found the good spot.
                bool wrongSpot = false;

                // Verify that all spots to fit the item are empty.
                int x, y;
                for( x = 0; x < itemGridSizeX; x++ ){
                    for( y = 0; y < itemGridSizeY; y++ ){                        
                        // If we hit a non-empty spot.
                        if( !grid[ i + x ][ j + y ].empty() ){                            
                            wrongSpot = true;
                            
                            // Make for loop end.
                            x = y = 32767;
                        }
                    }
                }

                // If this wasn't the wrong spot
                if( !wrongSpot ){
                    // We found a spot.
                    found = true;

                    foundX = i;
                    foundY = j;

                    // Make for loop end.
                    i = j = 32767;
                }
            }
        }
    }

    // If we didn't fing a valid spot.
    if( !found ){
        
        // Find out how much ylines could enter at the bottom of the grid.
        int lines = 0;
        
        LOG.Lock();
                
        bool done = false;
        // Scroll bottom to top
        for( j = grid[ 0 ].size() - 1; !done && j > 0; j-- ){
            for( i = 0; !done && i < grid.size(); i++ ){                
                LOG << "\r\nFound i,j " << i << ", " << j;
                
                // If a non-empty grid was found.
                if( !grid[ i ][ j ].empty() ){
                                        
                    //LOG << "  GRID NOT EMPTY -> check this " << grid[ i ][ j ].gridPos.x << ", " << grid[ i ][ j ].gridPos.y << "  ui=0x"<< (DWORD)grid[ i ][ j ].ui;
                    // Make for loop break.
                    done = true;
                }
            }            
            // If line scan wasn't aborted.
            if( !done ){
                // Count one more good line.
                lines++;
            }
        }
        
        LOG << "\r\nFound " << lines << " empty lines, so we'll add " << itemGridSizeY - lines << " lines.";
        
        LOG.Unlock();
        
        // Set the foundY before growing the grid.
        foundY = grid[ 0 ].size() - lines;

        // Grow the grid big enough to hold the new item.
        GrowGrid( itemGridSizeY - lines );
        // Set the foundX and Y pos.
        foundX = 0;
    }


    // Create a new grid item to reference the display item.
    GridItem newGridItem;
    newGridItem.ui      = displayItem;
    newGridItem.gridPos = MousePos( foundX, foundY );
    newGridItem.xOffset = ( ( squareSize.Width()  + gridSpacing.Width()  ) * itemGridSizeX - displayItem->GetPos().Width()  ) / 2;
    newGridItem.yOffset = ( ( squareSize.Height() + gridSpacing.Height() ) * itemGridSizeY - displayItem->GetPos().Height() ) / 2;

    LOG.Lock();

    LOG << "\r\nFound spot at " << foundX << ", " << foundY;
    LOG << "\r\nItem size: " << displayItem->GetPos().Width() << ", " << displayItem->GetPos().Height();
    LOG << "\r\nGrid size: " << itemGridSizeX << ", " << itemGridSizeY;

    // Fill the found squares with references to the new grid item.
    for( i = foundX; i < itemGridSizeX + foundX; i++ ){
        for( j = foundY; j < itemGridSizeY + foundY; j++ ){
            if( i < grid.size() ){                        
                LOG << "\r\nMonopolizing grid spot " << i << ", " << j;            
            
                grid[ i ][ j ] = newGridItem;
            }
        }
    }

    LOG.Unlock();

    // Add the grid item to the list of grid items.
    gridItems.push_back( newGridItem );

    return MousePos( foundX, foundY );
}

//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::DeleteGrid( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  This function deletes all GameUI pointers in the grid, and frees the grid.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    currentSelection = NULL;

    list< GridItem >::iterator i;
    for( i = gridItems.begin(); i != gridItems.end(); i++ ){
        delete( (*i).ui );        
    }
    gridItems.clear();
    // For all x.
    int j;
    for( j = 0; j < grid.size(); j++ ){
        // Resize y to 0.
        grid[ j ].resize( 0 );
    }
    SetListSize( 0 );
}

//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::DeleteGridExcept
//////////////////////////////////////////////////////////////////////////////////////////
//  Deletes all items in the grid which are not in the provided list.
// 
(
 const vector< GameUI * > &exceptList // The items to spare.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    list< GridItem >::iterator i;

    vector< GameUI * > deleteList;

    for( i = gridItems.begin(); i != gridItems.end(); i++ ){
        // If the UI is not in the except list.
        if( find( exceptList.begin(), exceptList.end(), (*i).ui ) == exceptList.end() ){
            // Add it to the deletion list.
            deleteList.push_back( (*i).ui );
        }
    }

    // Delete all items that were put in the deletion list.
    int j;
    for( j = 0; j != deleteList.size(); j++ ){
        RemoveItem( deleteList[ j ] );
    }

}

//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::GrowGrid
//////////////////////////////////////////////////////////////////////////////////////////
//  Grows the grid downwards for the specified quantity of lines.
// 
(
 int lines // How many lines to grow.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( lines != 0 ){    
        int newSize = grid[ 0 ].size() + lines;

        if( newSize > 0 ){           
            LOG << "\r\nOld " << grid[ 0 ].size() << ", new lines growth size: " << newSize << " lines=" << lines;

            // For all x.
            int i;
            for( i = 0; i < grid.size(); i++ ){
                // Resize y to fit new asked lines.
                grid[ i ].resize( newSize );
            }

            SetListSize( grid[ 0 ].size() );
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::DragItem
//////////////////////////////////////////////////////////////////////////////////////////
// Tries to drag an item from the grid.
// 
(
 MousePos mousePos // Targetted item.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If there is already a drag item or dragging is not allowed on this grid.
    if( GetDragItem() != NULL || !dragAllowed ){
        return;
    }
    
    GridPos gridPos = GetGridPosFromMouse( mousePos );
    
    LOG << "\r\nDragged at grid " << gridPos.x << ", " << gridPos.y;

    // If we clicked within the grid.
    if( gridPos.x < grid.size() && gridPos.y < grid[ 0 ].size() ){

        // If the grid at this point isn't empty.
        if( !grid[ gridPos.x ][ gridPos.y ].empty() ){            
            // Get the UI.
            GameUI *theUI = grid[ gridPos.x ][ gridPos.y ].ui;

            // Query the event
            if( !gridEvent.CanDrag( theUI ) ){
                return;
            }

            // Remove the target item.
            RemoveItem( grid[ gridPos.x ][ gridPos.y ].ui );

            // Set the drag item.
            SetDragItem( theUI, &gridEvent, this, mousePos );

            // Create a custom cursor with the ui's graphic.
            SetUIMouseCursor( theUI, 2, 2 );
        }
    }

    // Set the selection to null.
    currentSelection = NULL;

    // Notify the grid event.
    gridEvent.Dragged();
}

void GridUI::DropItemLastPos()
{
   DropItem(m_LastMousePos);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Drops a dragged item on the grid.
// void GridUI::DropItem(MousePos mousePos 
//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::DropItem(MousePos mousePos )
{
    GameUI *dragItem = GetDragItem();
    
    // If no drag item exist.    
    if( dragItem == NULL )
    {
        LOG << "\r\nDrag item NULL";

        return;
    }    

    // Correct the mouse pos to take into account the item's size.
    MousePos correctedMousePos(
        mousePos.x - dragItem->GetPos().Width()  / 2,
        mousePos.y - dragItem->GetPos().Height() / 2
    );
    
    // Get the position according the the item's top left corner.
    GridPos gridPos = GetGridPosFromMouse( correctedMousePos );
    

    // Get the item size in grid squares.
    unsigned long itemGridSizeX = ( dragItem->GetPos().Width()  / ( squareSize.Width()  + gridSpacing.Width()  ) ) + 1;
    unsigned long itemGridSizeY = ( dragItem->GetPos().Height() / ( squareSize.Height() + gridSpacing.Height() ) ) + 1;
   
    // If putting the item at the requested spot would mean going oustide the grid.
    if( !( gridPos.x + itemGridSizeX - 1 < grid.size() ) ){
        return;
    }

    // Check if one or less UI's are sitting on the requested spot.
    GameUI *underUI = NULL;
    GridPos uiPos;
    bool abortDrop = false;
    UINT i, j;

    for( i = gridPos.x; i != gridPos.x + itemGridSizeX; i++ ){
        for( j = gridPos.y; ( j != gridPos.y + itemGridSizeY ) && j < grid[ 0 ].size(); j++ ){
            // If there is an item.
            if( grid[ i ][ j ].ui != NULL && grid[ i ][ j ].ui != underUI ){
                // If this is the first item found.
                if( underUI == NULL ){
                    underUI = grid[ i ][ j ].ui;
                    uiPos.x = i;
                    uiPos.y = j;
                }else{
                    // Abort drop, we cannot drop where there are that many items.
                    abortDrop = true;                                
                }
            }
        }
    }
    
    // If drop was aborted because too many items layed under the desired spot.
    if( abortDrop ){
        return;
    }

    // Notify the grid event. 
    // If the grid event acknowledged the item as being allowed to be dropped in this grid.
    if( gridEvent.Dropping() ){

        // Notify the parent event that the item was successfully dropped.
        if( GetDragSourceEvent() != NULL ){
            // If the parent event finds that the target event is ok.
            GetDragSourceEvent()->Dropped( &gridEvent );
        }        

        // If an underUI was found.
        if( underUI != NULL ){
            // Remove the underUI from the grid.
            RemoveItem( grid[ uiPos.x ][ uiPos.y ].ui );
        }

        // If the desired spot is too far in y.
        if( gridPos.y + itemGridSizeY >= grid[ 0 ].size() ){

            LOG << "\r\nGrid is too small. Requesting " << gridPos.y + itemGridSizeY - grid[ 0 ].size() << " lines..";

            // Grow the grid big enough to hold the new item.
            GrowGrid( gridPos.y + itemGridSizeY - grid[ 0 ].size() );
        }
    
        // Create a new grid item to reference the display item.
        GridItem newGridItem;
        newGridItem.ui      = dragItem;
        newGridItem.gridPos = gridPos;
        newGridItem.xOffset = ( ( squareSize.Width()  + gridSpacing.Width()  ) * itemGridSizeX - dragItem->GetPos().Width()  ) / 2;
        newGridItem.yOffset = ( ( squareSize.Height() + gridSpacing.Height() ) * itemGridSizeY - dragItem->GetPos().Height() ) / 2;

        // Fill the found squares with references to the new grid item.
        for( i = gridPos.x; i < gridPos.x + itemGridSizeX; i++ ){
            for( j = gridPos.y; j < gridPos.y + itemGridSizeY; j++ ){
                grid[ i ][ j ] = newGridItem;
            }
        }

        // Add the item to the list.
        gridItems.push_back( newGridItem );

        // Cancel dragging.
        CancelDrag();

        // If an underUI was found.
        if( underUI != NULL ){
            // Set the new drag item.
            SetDragItem( underUI, &gridEvent, this, mousePos );

            // Create a custom cursor with the ui's graphic.
            SetUIMouseCursor( underUI, 2, 2 );
        }else{
            // Reset mouse cursor.
            CMouseCursor::GetInstance()->SetCustomCursor( NULL );
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::DblClickItem
//////////////////////////////////////////////////////////////////////////////////////////
//  Uses an item on the grid.
// 
(
 MousePos mousePos // The targetted mouse item.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    m_LastMousePos = mousePos;
    // Notify the grid event.
    gridEvent.LeftDblClicked();

    // Drop the item.
    DropItem( mousePos );    
}

//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::RemoveItem
//////////////////////////////////////////////////////////////////////////////////////////
//  Removes an item at a given position.
// 
(
 GameUI *wantedUI
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If an item was found at that pos.
    if( wantedUI != NULL ){
        if( wantedUI == currentSelection ){
            currentSelection = NULL;
        }

        // Scan all the grid.
        int i, j;
        for( i = 0; i < grid.size(); i++ ){
            for( j = 0; j < grid[ 0 ].size(); j++ ){
                // If we found the wanted UI in the grid.
                if( grid[ i ][ j ].ui == wantedUI ){
                    // Remove the reference from it.
                    grid[ i ][ j ].ui = NULL;
                }
            }
        }

        // Remove any empty lines which may have been left at the bottom of the grid.
        bool done = false;
        for( j = grid[ 0 ].size() - 1; j > 0 && !done; j-- ){
            for( i = 0; i < grid.size() && !done; i++ ){
                if( !grid[ i ][ j ].empty() ){
                    done = true;
                }
            }
        }

        LOG << "\r\nRemoving " << grid[ 0 ].size() << " - " << ( j + 2 ) << " lines from bottom.";
        // Remove any exces lines. j + 2 for the -1 in the for loop, and -1 when done = true;
        GrowGrid( -( grid[ 0 ].size() - ( j + 2 ) ) );


        GridItem refItem;
        refItem.ui = wantedUI;

        // Find the ui in the list.
        list< GridItem >::iterator z = find( gridItems.begin(), gridItems.end(), refItem );

        // If the item was found.
        if( z != gridItems.end() ){
            // Erase it.
            gridItems.erase( z );
        }
    }

}

//////////////////////////////////////////////////////////////////////////////////////////
bool GridUI::MouseInGrid
//////////////////////////////////////////////////////////////////////////////////////////
// Determines if a mouse pos hit the grid.
// 
(
 MousePos mousePos // The mouse pos.
)
// Return: bool, true if the mouse position is within the grid area.
//////////////////////////////////////////////////////////////////////////////////////////
{
    GridPos gridPos = GetGridPosFromMouse( mousePos );

    // If we clicked within the grid.
    if( gridPos.x < grid.size() && gridPos.y < grid[ 0 ].size() ){
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
GridUI::GridPos GridUI::GetGridPosFromMouse
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the grid pos according to the current mouse position.
// 
(
 MousePos mousePos // The mouse position.
)
// Return: GridUI::GridPos, The grid pos.
//////////////////////////////////////////////////////////////////////////////////////////
{
    GridPos gridPos;

    // Get the x-y grid coordinates
    gridPos.x = ( mousePos.x - GetPos().x ) /                  // Mouse offset in grid.
                ( squareSize.Width()  + gridSpacing.Width() ); // Space taken by a square.        
    gridPos.y = ( mousePos.y - GetPos().y ) /
                ( squareSize.Height() + gridSpacing.Height() ) +
                linePos;    // Current scroll position.

    return gridPos;
}

//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::GetGridItems
//////////////////////////////////////////////////////////////////////////////////////////
//  Copies the grid items into the given container.
// 
(
 std::vector< GameUI * > &itemContainer
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    list< GridItem >::iterator i;
    for( i = gridItems.begin(); i != gridItems.end(); i++ ){
        itemContainer.push_back( (*i).ui );
    }

}

//////////////////////////////////////////////////////////////////////////////////////////
void GridUI::GetGridItemList
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns a list of grid items
// 
(
 std::list< GridItem > &itemList // The item list.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    list< GridItem >::iterator i;
    for( i = gridItems.begin(); i != gridItems.end(); i++ ){
        itemList.push_back( *i );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool GridUI::AddGridItemAtPos
//////////////////////////////////////////////////////////////////////////////////////////
//  Addes an item the the given position in the grid
// 
(
 MousePos gridPos, // The position.
 GameUI  *theUI   // The UI
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Get the item size in grid squares.
    unsigned long itemGridSizeX = ( theUI->GetPos().Width()  / ( squareSize.Width()  + gridSpacing.Width()  ) ) + 1;
    unsigned long itemGridSizeY = ( theUI->GetPos().Height() / ( squareSize.Height() + gridSpacing.Height() ) ) + 1;
   
    // If putting the item at the requested spot would mean going oustide the grid.
    if( !( gridPos.x + itemGridSizeX - 1 < grid.size() ) ){
        return false; 
    }

    // Check if one or less UI's are sitting on the requested spot.
    GridPos uiPos;
    int i, j;

    for( i = gridPos.x; i != gridPos.x + itemGridSizeX; i++ ){
        for( j = gridPos.y; ( j != gridPos.y + itemGridSizeY ) && j < grid[ 0 ].size(); j++ ){
            // If there is an item.
            if( grid[ i ][ j ].ui != NULL ){
                // Item is NOT added! The caller is responsible for finding
                // another pos or using the standard AddItem function.
                return false;
            }
        }
    }
    
    // If the desired spot is too far in y.
    if( gridPos.y + itemGridSizeY >= grid[ 0 ].size() ){

        LOG << "\r\nGrid is too small. Requesting " << gridPos.y + itemGridSizeY - grid[ 0 ].size() << " lines..";

        // Grow the grid big enough to hold the new item.
        GrowGrid( gridPos.y + itemGridSizeY - grid[ 0 ].size() );
    }
    
    // Create a new grid item to reference the display item.
    GridItem newGridItem;
    newGridItem.ui      = theUI;
    newGridItem.gridPos = gridPos;
    newGridItem.xOffset = ( ( squareSize.Width()  + gridSpacing.Width()  ) * itemGridSizeX - theUI->GetPos().Width()  ) / 2;
    newGridItem.yOffset = ( ( squareSize.Height() + gridSpacing.Height() ) * itemGridSizeY - theUI->GetPos().Height() ) / 2;

    // Fill the found squares with references to the new grid item.
    for( i = gridPos.x; i < gridPos.x + itemGridSizeX; i++ ){
        for( j = gridPos.y; j < gridPos.y + itemGridSizeY; j++ ){
            grid[ i ][ j ] = newGridItem;
        }
    }

    // Add the item to the list.
    gridItems.push_back( newGridItem );

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
GameUI *GridUI::GetGridItemAt
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the grid item pin-pointed by the mouse position.
// 
(
 MousePos mousePos // 
)
// Return: void GameUI, NULL if none were found.
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( !MouseInGrid( mousePos ) ){
        return NULL;
    }

    GridPos gridPos = GetGridPosFromMouse( mousePos );
    
    // If the mouse did not hit an existing portion of the grid.
    if( gridPos.x >= grid.size() || gridPos.y >= grid[ 0 ].size() ){
        return NULL;
    }
            
    // If the grid pos is not empty.
    if( !grid[ gridPos.x ][ gridPos.y ].empty() ){
        // Return the grid item at this position.
        return grid[ gridPos.x ][ gridPos.y ].ui;
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool GridUI::CanDrop
//////////////////////////////////////////////////////////////////////////////////////////
//  Determines if the currently dragged item can be dropped at the given position.
// 
(
 MousePos mousePos // The position
)
// Return: bool, true if the item can be dropped
//////////////////////////////////////////////////////////////////////////////////////////
{
    GameUI *dragItem = GetDragItem();
    
    // If no drag item exist.    
    if( dragItem == NULL ){
        LOG << "\r\nDrag item NULL";

        return false;
    }    

    // Correct the mouse pos to take into account the item's size.
    MousePos correctedMousePos(
        mousePos.x - dragItem->GetPos().Width()  / 2,
        mousePos.y - dragItem->GetPos().Height() / 2
    );
    
    // Get the position according the the item's top left corner.
    GridPos gridPos = GetGridPosFromMouse( correctedMousePos );
    
    // Get the item size in grid squares.
    unsigned long itemGridSizeX = ( dragItem->GetPos().Width()  / ( squareSize.Width()  + gridSpacing.Width()  ) ) + 1;
    unsigned long itemGridSizeY = ( dragItem->GetPos().Height() / ( squareSize.Height() + gridSpacing.Height() ) ) + 1;
   
    // If putting the item at the requested spot would mean going oustide the grid.
    if( !( gridPos.x + itemGridSizeX - 1 < grid.size() ) ){
        return false;
    }

    // Check if a UI is sitting on the requested spot.
    int i, j;
    for( i = gridPos.x; i != gridPos.x + itemGridSizeX; i++ ){
        for( j = gridPos.y; ( j != gridPos.y + itemGridSizeY ) && j < grid[ 0 ].size(); j++ ){
            // If there is an item.
            if( grid[ i ][ j ].ui != NULL ){
                return false;
            }
        }
    }
    
    return true;
}