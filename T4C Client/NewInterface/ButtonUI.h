// ButtonUI.h
//

#if !defined(ButtonUI_h)
#define ButtonUI_h

#include "GraphUI.h"
#include <string>

class ButtonUI : public ControlUI
{
public:
    ButtonUI( EventVisitor &eventVisitor, std::string buttonText = "" );
    
    // Buttons respond to mouse up/down (clicks).
    bool LeftMouseUp( MousePos mousePos );
    bool LeftMouseDown( MousePos mousePos );
    bool DragCycle( MousePos mousePos );

    // Needed to draw the button's text.
    void Draw( V2SPRITEFX *vsfFX );

    void SetDownUI( std::string downSpriteId ){
        downUI.SetSpriteId( downSpriteId );
    }

    void SetHighlightUI( std::string highlightSpriteId ){
        selectionUI.SetSpriteId( highlightSpriteId );
    }

    void SetPos( FramePos framePos );

    void EnableSound( bool enable ){
        soundEnabled = enable;
    }

private:
    GraphUI downUI;
    GraphUI selectionUI;

    bool pressed;
    bool enableDragCycle;
    bool soundEnabled;

    std::string	  buttonText;
    EventVisitor &buttonEvent;

};

#endif /* ButtonUI_h */
