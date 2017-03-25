// GridUI.h
//

#if !defined(GridUI_h)
#define GridUI_h

#include "ControlUI.h"
#include <vector>
#include <list>
#include "../ScrollUI.h"

#pragma warning( disable : 4786 )

class EventVisitor;

class GridUI : public ControlUI, public ScrollUI
{
public:

    GridUI( EventVisitor &eventVisitor );

    void Draw( V2SPRITEFX *vsfFX );
    void DrawHelp( V2SPRITEFX *vsfFX );
    bool WheelUp( MousePos mousePos );
    bool WheelDown( MousePos mousePos );
    bool RightMouseDown( MousePos mousePos );
    bool RightMouseUp( MousePos mousePos );
    bool LeftMouseDown( MousePos mousePos );
    bool LeftMouseUp( MousePos mousePos );

    void AllowScroll( bool newState ){scrollAllowed = newState;};
    void AllowDrag( bool newState ){dragAllowed = newState;}
    void AllowHelp( bool newState ){allowHelp = newState;}

    


    MousePos AddItem( GameUI *displayItem );
    void     DeleteGrid( void );
    void     DeleteGridExcept( const std::vector< GameUI * > &exceptList );

    void     SetGrid( unsigned long newXSize, unsigned long newYSize, FramePos newSquareSize, FramePos newGridSpacing ){
        squareSize  = newSquareSize;
        gridSpacing = newGridSpacing;
        xSize       = newXSize;
        ySize       = newYSize;

        // Setup the grid.
        grid.resize( xSize );
        
        // for each xSize
        unsigned long i;
        for( i = 0; i != xSize; i++ ){
            // Reserver the ySize
            grid[ i ].resize( ySize );
        }

        SetListSize( grid[ 0 ].size() );
    }

    void GetGridItems( std::vector< GameUI * > &itemContainer );
    
    void SetSelectedTileUI( GameUI *newUI ){
        selectedTileUI = newUI;
    }

    void SetSelectedLockedTileUI( GameUI *newUI ){
        selectedLockedTileUI = newUI;
    }

    GameUI *GetCurrentSelection(){
        return currentSelection;
    }

    typedef MousePos GridPos;
    
    bool AddGridItemAtPos( GridPos thePos, GameUI  *theUI );    

    class GridItem{
    public:
        friend GridUI;

        GridItem()
            :   ui( NULL ){};

        bool empty(){
            return ui == NULL;
        }

        bool operator == ( const GridItem &r ) const{
            return ui == r.ui;
        }

        GameUI  *ui;        
        MousePos gridPos;

    private:
        int xOffset;
        int yOffset;
    };

    void GetGridItemList( std::list< GridItem > &itemList );

    GameUI *GetGridItemAt( MousePos mousePos );

    bool CanDrop( MousePos mousePos );

public:
    // Helper functions.
    bool MouseInGrid( MousePos mousePos );
    GridPos GetGridPosFromMouse( MousePos mousePos );

    void RemoveItem( GameUI *wantedUI );

    void DragItem( MousePos mousePos );
    void DropItem( MousePos mousePos );
    void DropItemLastPos();
    void DblClickItem( MousePos mousePos );


    void GrowGrid( int lines );

    typedef std::vector< std::vector< GridItem > > Grid;

    Grid                    grid;
    std::list< GridItem >   gridItems;

    unsigned int listSize;

    FramePos        squareSize;
    FramePos        gridSpacing;
    unsigned long   xSize;
    unsigned long   ySize;
    
    int             maxy;

    GameUI         *selectedTileUI; // This UI contains the sprite to be displayed
                                    // to cover a selected tile.
    GameUI         *selectedLockedTileUI; // This UI contains the sprite to be displayed
                                          // when an a grid item has been selected.

    GameUI         *currentSelection;

    // Control flags.
    bool    dragAllowed;
    bool    scrollAllowed;
    bool    allowHelp;

    EventVisitor &gridEvent;
    MousePos   m_LastMousePos;

};

#endif /* GridUI_h */
