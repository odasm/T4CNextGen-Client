// UIMouseEvent.cpp: implementation of the UIMouseEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "UIMouseEvent.h"
#include "Lock.h"
#include "NewInterface/RootBoxUI.h"
#include <deque>
#include "MemoryManager.h"

using namespace std;

namespace{
enum MouseEventType{
    LeftMouseUpEvent = 1,
    LeftMouseDownEvent = 2,
    RightMouseUpEvent = 3,
    RightMouseDownEvent = 4,
    WheelUpEvent = 5,
    WheelDownEvent = 6,
    DragEvent = 7
};

};


class MouseEventItem{
public:
    MouseEventType eventType;
    MousePos mousePos;
};


// UIMouseEvent variables.
namespace{

    CLock cLock;

    deque< MouseEventItem > mouseEvents;
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UIMouseEvent::UIMouseEvent()
{

}

UIMouseEvent::~UIMouseEvent()
{

}


//////////////////////////////////////////////////////////////////////////////////////////
void UIMouseEvent::QueueLeftMouseUp( MousePos mousePos )
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &cLock );

    MouseEventItem mouseItem;

    mouseItem.eventType = LeftMouseUpEvent;
    mouseItem.mousePos = mousePos;

    mouseEvents.push_back( mouseItem );
}
//////////////////////////////////////////////////////////////////////////////////////////
void UIMouseEvent::QueueLeftMouseDown( MousePos mousePos )
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &cLock );

    MouseEventItem mouseItem;

    mouseItem.eventType = LeftMouseDownEvent;
    mouseItem.mousePos = mousePos;

    mouseEvents.push_back( mouseItem );
}
//////////////////////////////////////////////////////////////////////////////////////////
void UIMouseEvent::QueueRightMouseUp( MousePos mousePos )
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &cLock );

    MouseEventItem mouseItem;

    mouseItem.eventType = RightMouseUpEvent;
    mouseItem.mousePos = mousePos;

    mouseEvents.push_back( mouseItem );
}
//////////////////////////////////////////////////////////////////////////////////////////
void UIMouseEvent::QueueRightMouseDown( MousePos mousePos )
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////
{    
    CAutoLock autoLock( &cLock );

    MouseEventItem mouseItem;

    mouseItem.eventType = RightMouseDownEvent;
    mouseItem.mousePos = mousePos;

    mouseEvents.push_back( mouseItem );
}
//////////////////////////////////////////////////////////////////////////////////////////
void UIMouseEvent::QueueWheelUp( MousePos mousePos )
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &cLock );

    MouseEventItem mouseItem;

    mouseItem.eventType = WheelUpEvent;
    mouseItem.mousePos = mousePos;

    mouseEvents.push_back( mouseItem );
}
//////////////////////////////////////////////////////////////////////////////////////////
void UIMouseEvent::QueueWheelDown( MousePos mousePos )
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &cLock );

    MouseEventItem mouseItem;

    mouseItem.eventType = WheelDownEvent;
    mouseItem.mousePos = mousePos;

    mouseEvents.push_back( mouseItem );
}

//////////////////////////////////////////////////////////////////////////////////////////
void UIMouseEvent::QueueDrag
//////////////////////////////////////////////////////////////////////////////////////////
//  Queues a dragging event.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &cLock );

    MouseEventItem mouseItem;

    mouseItem.eventType = DragEvent;
    mouseItem.mousePos = mousePos;

    mouseEvents.push_back( mouseItem );
}

//////////////////////////////////////////////////////////////////////////////////////////
UIMouseEvent *UIMouseEvent::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the sole instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static UIMouseEvent mouseEvent;

    return &mouseEvent;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool UIMouseEvent::ResolveQueuedEvent( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Resolves the next queued mouse event.
// 
// Return: bool, true if the mouse event was handled by a UI.
//////////////////////////////////////////////////////////////////////////////////////////
{       
    bool returnValue = false;
    bool ret = false;
    
    MouseEventItem mouseItem;

    while(1){
        {
            CAutoLock autoLock( &cLock );

            // If there aren't any items in the queue.
            if( mouseEvents.empty() ){
                return ret;
            }

            // Get the front mouse event.
            mouseItem = mouseEvents.front();
            mouseEvents.pop_front();
        }

        switch( mouseItem.eventType ){
        case LeftMouseUpEvent:
            returnValue = RootBoxUI::GetInstance()->LeftMouseUp( mouseItem.mousePos );
            break;    
        case LeftMouseDownEvent:
            returnValue = RootBoxUI::GetInstance()->LeftMouseDown( mouseItem.mousePos );
            break;
        case RightMouseUpEvent:
            returnValue = RootBoxUI::GetInstance()->RightMouseUp( mouseItem.mousePos );
            break;    
        case RightMouseDownEvent:
            returnValue = RootBoxUI::GetInstance()->RightMouseDown( mouseItem.mousePos );
            break;
        case WheelUpEvent:
            returnValue = RootBoxUI::GetInstance()->WheelUp( mouseItem.mousePos );
            break;
        case WheelDownEvent:
            returnValue = RootBoxUI::GetInstance()->WheelDown( mouseItem.mousePos );
            break;
        case DragEvent:
            returnValue = RootBoxUI::GetInstance()->DragCycle( mouseItem.mousePos );
            break;
        } 
        if( returnValue ){
            ret = true;
        }
    }
}