// ControlUI.h
//

#if !defined(ControlUI_h)
#define ControlUI_h

#include "GameUI.h"

// Abstract class.
class ControlUI : public GameUI
{
public:
    ControlUI();
    virtual ~ControlUI() = 0;

    virtual void SetSpriteId( std::string newSpriteId );

protected:
    // Simply perpetuate the protected status of these functions.
    GameUI::GetChildList;
    GameUI::IsDblClick;
    GameUI::IsClick;
    GameUI::SetDragItem;
    GameUI::GetDragItem;

private:
    // Disable DrawMinimized capabilities for controls.
    GameUI::DrawMinimized;

};

#endif /* ControlUI_h */
