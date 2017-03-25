// ListUI.cpp
//
#include "../pch.h"

#include "ListUI.h"
#include "EventVisitor.h"
#include "../MemoryManager.h"

namespace{
    T3VSBSound *selectSound = NULL;

    void InitSound(){
        if( !selectSound ){
            selectSound = new T3VSBSound;

            selectSound->Create( "Generic Drop Item", TS_MEMORY );
        }

    }
};

ListUI::ListUI( EventVisitor &newVisitor )
    :   listEvent( newVisitor ),
        variableHeight( false ),
        selectionSprite( NULL ),
        selection( NULL )
{
    InitSound();
}

ListUI::~ListUI(){
    DeleteList();
}

//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::Draw( V2SPRITEFX *vsfFX )
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the list box.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Draw the listUI's background.
    GameUI::Draw( vsfFX );

    // Setup the V2SPRITEFX for all sprites.
    V2SPRITEFX fx;

    memset( &fx, 0, sizeof( fx ) );

    // Setup the clipping rectangle
    RECT rect = { GetPos().x, GetPos().y, GetPos().maxx, GetPos().maxy };
    fx.lpClipRect = &rect;
    
    // Specify no correction.
    fx.dwFX |= FX_NOCORRECTION;
    

    // Draw the list's items
    // For all the lines.
    int newY = GetPos().y - ( linePos * ( rowSize + spacing.Height() ) );
    
    int j;    
    for( j = 0; j < itemGrid[ 0 ].size(); j++ ){
        // For all the columns               
        int i;
        for( i = 0; i < itemGrid.size(); i++ ){
            GameUI *targetUI = itemGrid[ i ][ j ];
            if( targetUI != NULL ){
                FramePos newPos = targetUI->GetPos();

                int columnOffset = 0;
                int z;
                for( z = 0; z < i; z++ ){
                    columnOffset += columnAttr[ z ].columnSize + spacing.Width();
                }

                if( columnAttr[ i ].centerColumn ){                    
                    newPos.SetNewOffset(
                        MousePos(
                            
                            ( static_cast< double >( columnAttr[ i ].columnSize ) / 2.0 - 
                              static_cast< double >( targetUI->GetPos().Width() ) / 2.0 ) +
                            columnOffset + GetPos().x,
                            variableHeight ? 
                                newY + ( static_cast< double >( rowSize ) / 2.0 - 
                                         static_cast< double >( targetUI->GetPos().Height() ) ) / 2.0
                                :
                                ( j - linePos ) * ( rowSize + spacing.Height() ) + 
                                    ( static_cast< double >( rowSize ) / 2.0 - 
                                      static_cast< double >( targetUI->GetPos().Height() ) / 2.0 ) +
                                GetPos().y

                        )
                    );
                }else{
                    newPos.SetNewOffset( 
                        MousePos(
                            columnOffset + GetPos().x,
                            variableHeight ? 
                                newY + rowSize / 2 - targetUI->GetPos().Height() / 2
                                :
                                ( j - linePos ) * ( rowSize + spacing.Height() ) + 
                                rowSize / 2 - targetUI->GetPos().Height() / 2 +
                                GetPos().y
                        )
                    );
                }

                // If a selection sprite was provided and it corresponds to this UI.
                if( selection == targetUI && selectionSprite != NULL ){
                    FramePos selectPos = selectionSprite->GetPos();

                    selectPos.SetNewOffset(
                        MousePos(
                            columnOffset + GetPos().x + selectionXoff,
                            variableHeight ? 
                                newY
                                :
                                ( j - linePos ) * ( rowSize + spacing.Height() ) +                                 
                                GetPos().y + selectionYoff
                        )
                    );
                    
                    selectionSprite->SetPos( selectPos );
                    selectionSprite->Draw( &fx );
                }

                targetUI->SetPos( newPos );

                // Draw the item at its position.            
                targetUI->Draw( &fx );
            }
        }
        
        newY += itemGrid[ 0 ][ j ]->GetPos().Height();
    }

    ScrollUI::DrawScroll( vsfFX );
}


//////////////////////////////////////////////////////////////////////////////////////////
bool ListUI::WheelUp
//////////////////////////////////////////////////////////////////////////////////////////
// Mouse wheel went up! Move list up (scroll up).
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    ScrollUp( 1 );

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ListUI::WheelDown
//////////////////////////////////////////////////////////////////////////////////////////
// Mouse wheel went down! Move list down (scroll down).
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    ScrollDown( 1 );

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ListUI::RightMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Mouse went up
// 
(
 MousePos mousePos // 
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( IsClick() ){
        GameUI *item = GetItemFromMousePos( mousePos );

        if( item != NULL ){
            LOG << "\r\nDISPLAYING LIST ITEM HELP!!";
            item->DisplayHelp( true, MouseDevice::xPos - 2, MouseDevice::yPos - 2 );
        }else{
            LOG << "\r\nDISPLAYING DEFAULT LIST HELP!!!";
            GameUI::RightMouseUp( mousePos );
        }

        selection = item;
        selectedColumn = GetColumnFromMousePos( mousePos );
        selectedRow    = GetRowFromMousePos( mousePos );

        selectSound->Play( TRUE );
    }
    return true;
}

bool ListUI::LeftMouseDown( MousePos mousePos )
{
    // If this didn't hit the scroll bar.
    if( !ScrollClick( mousePos ) ){        
        if( IsDblClick() ){
            // Use item.
            DblClickItem( mousePos );
        }
    }
    
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ListUI::LeftMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
// The mouse was released on top of this control.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( !IsMouseInScrollBar( mousePos ) && IsClick() ){
        ClickItem( mousePos );
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
const ListUI::GridItem ListUI::GetSelection( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the currently selected ListItem
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Return the grid item at the selected position.
    return selection;
}


//////////////////////////////////////////////////////////////////////////////////////////
int ListUI::GetSelectedColumn( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the selected column, -1 if none.
//////////////////////////////////////////////////////////////////////////////////////////
{
    return selectedColumn;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::SetupList
//////////////////////////////////////////////////////////////////////////////////////////
// Set the list's size.
// 
(
 int theColumns,       // Number of columns in the list.
 int theRowSize,
 FramePos theSpacing   // Spacing between colums and rows.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    rowSize = theRowSize;
    spacing = theSpacing;
    columns = theColumns;

    // Resize the vectors to fit the new column size.
    itemGrid.resize( theColumns );
    columnAttr.resize( theColumns );

}
//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::SetColumnSize
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the x size of a specific column
// 
(
 int columnNumber, // The column
 int xSize         // The x size.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    columnAttr[ columnNumber ].columnSize = xSize;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::DeleteList( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Deletes everything in the list UI.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If the list is already empty.
    if( itemGrid[ 0 ].size() == 0 ){
        return;
    }
    
    int i, j;
    for( i = 0; i < itemGrid.size(); i++ ){
        for( j = 0; j < itemGrid[ i ].size(); j++ ){
            
            if( itemGrid[ i ][ j ] != NULL ){
                // Delete the gameUI.
                delete itemGrid[ i ][ j ];
                itemGrid[ i ][ j ] = NULL;
            }
        }
        // Resive this column to 0.
        itemGrid[ i ].resize( 0 );
    }

    selection = NULL;

    linePos = 0;

    SetListSize( 0 );
}
//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::AddNewItem( bool autoScroll )
//////////////////////////////////////////////////////////////////////////////////////////
//  Adds a new row to the list.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    LOG << "\r\nADDING NEW ITEM, Size=" << itemGrid[ 0 ].size();

    int listSize = itemGrid[ 0 ].size();

    // Remove the visible quantity of items.
    listSize -= GetPos().Height() / ( rowSize + spacing.Height()+1 );

    bool scroll = false;

    if( linePos == listSize - 1 ){
        scroll = true;
    }

    int i;
    // Grow all columns to fit the new row.
    for( i = 0; i < itemGrid.size(); i++ ){
        itemGrid[ i ].resize( itemGrid[ i ].size() + 1 );
        itemGrid[ i ][ itemGrid[ i ].size() - 1 ] = NULL;
    }

    listSize = itemGrid[ 0 ].size();

    // Remove the visible quantity of items.
    listSize -= GetPos().Height() / ( rowSize + spacing.Height()+1 );

    if( listSize < 0 ){
        listSize = 0;
    }

    SetListSize( listSize );

    if( scroll && autoScroll ){
        ScrollUp( 32767 );
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::AddColumnItem
//////////////////////////////////////////////////////////////////////////////////////////
//  Modifies a column of the current row.
// 
(
 int columnNumber, // The column.
 GameUI *listItem  // The item to put there.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    LOG << "\r\nUPDATING NEW ITEM AT " << columnNumber << ", " << (itemGrid[ 0 ].size() - 1);
    itemGrid[ columnNumber ][ itemGrid[ 0 ].size() - 1 ] = listItem;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::CenterColumn
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the specified column as being centered.
// 
(
 int columnNumber // The columns.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    columnAttr[ columnNumber ].centerColumn = true;
}

//////////////////////////////////////////////////////////////////////////////////////////
GameUI *ListUI::GetItemFromMousePos
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the GameUI pointed by the mouse cursor.
// 
(
 MousePos mousePos // The mouse position.
)
// Return: GameUI, the gameUI, or NULL if none were found.
//////////////////////////////////////////////////////////////////////////////////////////
{
           
    int line = ( mousePos.y - GetPos().y ) / ( rowSize + spacing.Height() );

    line += linePos;

    if( line < itemGrid[ 0 ].size() ){

        LOG.Lock();
        LOG << "\r\nFound item " << (DWORD)itemGrid[ 0 ][ line ] << " at line " << line;
        LOG.Unlock();


        return itemGrid[ 0 ][ line ];
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////
int ListUI::GetColumnFromMousePos
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the selected column
// 
(
 MousePos mousePos
)
// Return: int, the hit column.
//////////////////////////////////////////////////////////////////////////////////////////
{    
    int len = GetPos().x;
    
    int i;
    for( i = 0; i < columnAttr.size(); i++ ){    
        len += columnAttr[ i ].columnSize;

        if( mousePos.x < len ){
            return i;
        }

        len += spacing.Width();
    }

    return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////
int ListUI::GetRowFromMousePos
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the row selected by the current mouse pos.
// 
(
 MousePos mousePos
)
// Return: int, 
//////////////////////////////////////////////////////////////////////////////////////////
{
    int line = ( mousePos.y - GetPos().y ) / ( rowSize + spacing.Height() );

    line += linePos;

    if( line < itemGrid[ 0 ].size() ){
        return line;
    }
    return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::DblClickItem
//////////////////////////////////////////////////////////////////////////////////////////
// An item got dbl clicked.
// 
(
 MousePos mousePos // the mouse pos.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    GameUI *item = GetItemFromMousePos( mousePos );

    selection = item;
    selectedColumn = GetColumnFromMousePos( mousePos );
    selectedRow    = GetRowFromMousePos( mousePos );

    LOG << "\r\n DBL-CLICKED LIST." ;
    
    if( item != NULL ){        

        listEvent.LeftDblClicked();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::ClickItem
//////////////////////////////////////////////////////////////////////////////////////////
//  Clicked on an item.
// 
(
 MousePos mousePos // the mouse pos.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    GameUI *item = GetItemFromMousePos( mousePos );

    selection = item;
    selectedColumn = GetColumnFromMousePos( mousePos );
    selectedRow    = GetRowFromMousePos( mousePos );

    if( item != NULL ){
        selectSound->Play( TRUE );
        listEvent.LeftClicked();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool ListUI::DragCycle
//////////////////////////////////////////////////////////////////////////////////////////
//  Received a drag notification.
// 
(
 MousePos mousePos // The mouse pos.
)

//////////////////////////////////////////////////////////////////////////////////////////
{
    if( IsMouseInScrollBar( mousePos ) ){
        return true;
    }
    
    GameUI *item = GetItemFromMousePos( mousePos );

    selection = item;
    selectedColumn = GetColumnFromMousePos( mousePos );
    selectedRow    = GetRowFromMousePos( mousePos );

    if( item != NULL ){        
        listEvent.LeftClicked();
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
const ListUI::GridItem ListUI::GetSelectedRowItem
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the item in the specified column of the currently selected row.
// 
(
 int column // The column
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( selectedRow == -1 ){
        return NULL;
    }

    return itemGrid[ column ][ selectedRow ];
}

//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::DeleteSelection( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Delete the current list selection.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{    
    // Find the selected iterator
    // Grow all columns to fit the new row.
    int z;
    for( z = 0; z < itemGrid.size(); z++ ){
        int theRow = 0;
        vector< GridItem >::iterator i;
        for( i = itemGrid[ z ].begin(); i != itemGrid[ z ].end(); i++ ){        
            if( theRow == selectedRow ){
                delete (*i);
                itemGrid[ z ].erase( i );
                break;
            }
            theRow++;
        }
    }
    selection = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ListUI::DrawHelp
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the help of all list items
// 
(
 V2SPRITEFX *vsfFX
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    int j;    
    for( j = 0; j < itemGrid[ 0 ].size(); j++ ){
        GameUI *targetUI = itemGrid[ 0 ][ j ];
        if( targetUI != NULL ){
            targetUI->DrawHelp( vsfFX );
        }
    }
}