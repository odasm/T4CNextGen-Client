// ListUI.h
//

#if !defined(ListUI_h)
#define ListUI_h

#include "ControlUI.h"
#include "../ScrollUI.h"
#include <vector>

class EventVisitor;

class ListUI : public ControlUI, public ScrollUI
{
public:
    ListUI( EventVisitor &listVisitor );
    ~ListUI();


    virtual bool WheelUp( MousePos mousePos );
    virtual bool WheelDown( MousePos mousePos );
    virtual void Draw( V2SPRITEFX *vsfFX );
    virtual bool RightMouseUp( MousePos mousePos );
    virtual bool LeftMouseDown( MousePos mousePos );
    virtual bool LeftMouseUp( MousePos mousePos );
    virtual bool DragCycle( MousePos mousePos );
    virtual void DrawHelp( V2SPRITEFX *vsfFX );

    typedef GameUI *GridItem;

    const GridItem GetSelection();
    int  GetSelectedColumn();

    const GridItem GetSelectedRowItem( int column );

    void DeleteList( void );
    void AddNewItem( bool autoScroll = false );
    void AddColumnItem( int columnNumber, GameUI *listItem );

    void SetupList( int columns, int rowSize, FramePos spacing );
    void SetColumnSize( int columnNumber, int xSize );
    void CenterColumn( int columnNumber );

    void DeleteSelection( void );

    std::vector< std::vector< GridItem > > *GetGrid(){
        return &itemGrid;
    }

    void SetVariableHeight( bool newState ){
        variableHeight = newState;
    }

    void SetSelectionSprite( GameUI *ui, int xOff = 0, int yOff = 0 ){
        selectionXoff = xOff;
        selectionYoff = yOff;
        selectionSprite = ui;
    }

    void SetSelection( GameUI *newSelection ){
        selection = newSelection;
    }

private:

    void    DblClickItem( MousePos mousePos );
    void    ClickItem( MousePos mousePos );
    GameUI *GetItemFromMousePos( MousePos mousePos );
    int     GetColumnFromMousePos( MousePos mousePos );
    int     GetRowFromMousePos( MousePos mousePos );
    
    class ColumnAttr{
    public:
        ColumnAttr() : columnSize( 0 ), centerColumn( false ){};

        int  columnSize;
        bool centerColumn;
    };

    EventVisitor &listEvent;


    std::vector< std::vector< GridItem > > itemGrid;

    int columns;
    FramePos spacing;

    std::vector< ColumnAttr > columnAttr;

    int rowSize;

    bool    variableHeight;

    GameUI *selectionSprite;
    int     selectionXoff;
    int     selectionYoff;
    GameUI *selection;
    int     selectedColumn;
    int     selectedRow;
};

#endif /* ListUI_h */
