// DropZoneUI.h
//

#if !defined(DropZoneUI_h)
#define DropZoneUI_h

#include "ControlUI.h"

class DropZoneUI : public ControlUI
{
public:
    DropZoneUI( EventVisitor &eventVisitor );

    void Draw( V2SPRITEFX *vsfFX );
    void DrawHelp( V2SPRITEFX *vsfFX );

    bool LeftMouseDown( MousePos mousePos );
    bool LeftMouseUp( MousePos mousePos );
    bool RightMouseUp( MousePos mousePos );

    void SetItem( GameUI *newUI ){
        currentUI = newUI;
    }

    GameUI *GetItem( void ){
        return currentUI;
    }

private:
    void DragItem( MousePos mousePos );
    void DropItem( MousePos mousePos );

    GameUI  *currentUI;

    EventVisitor &dropzoneEvent;
};

#endif /* DropZoneUI_h */
