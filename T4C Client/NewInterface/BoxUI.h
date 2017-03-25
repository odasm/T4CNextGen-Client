// BoxUI.h
//

#if !defined(BoxUI_h)
#define BoxUI_h

#include "ButtonUI.h"
#include "GraphUI.h"

// Abstract class.
class BoxUI : public GameUI
{
public:
    BoxUI();
    virtual ~BoxUI() = 0;

    // The BoxUI redefines the user events to distribute them amongst the controls.
    virtual bool LeftMouseDown( MousePos mousePos );
    virtual bool LeftMouseUp( MousePos mousePos );
    virtual bool RightMouseDown( MousePos mousePos );
    virtual bool RightMouseUp( MousePos mousePos );
    virtual bool WheelUp( MousePos mousePos );
    virtual bool WheelDown( MousePos mousePos );
    virtual bool DragCycle( MousePos mousePos );
    virtual bool TextInput( char ch );
    virtual bool VKeyInput( VKey vKey );

    
    GameUI *GetLastClickedItem( void ){
        return lastClickedItem;
    }

    GameUI *GetHitChild( MousePos mousePos );

    GameUI::SetDragItem;
    GameUI::GetDragItem;

    virtual void Hide();
    virtual void Show( bool putForeground = true ,bool bPlaySound =true);

protected:
    // Simply perpetuate the protected status of these functions.    
    GameUI::DrawMinimized;
    GameUI::GetChildList;
    GameUI::IsDblClick;
    GameUI::IsClick;
    GameUI::RequestForegroundControl;
    GameUI::DismissForegroundControl;    
    DWORD lastDrag;

private:

};

#endif /* BoxUI_h */
