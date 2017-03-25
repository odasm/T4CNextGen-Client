// UIMouseEvent.h: interface for the UIMouseEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIMOUSEEVENT_H__44302F53_27EC_11D3_8531_00E02922FA40__INCLUDED_)
#define AFX_UIMOUSEEVENT_H__44302F53_27EC_11D3_8531_00E02922FA40__INCLUDED_

#include <windows.h>
#include "NewInterface/GameUI.h"

class UIMouseEvent  
{
public:	
	virtual ~UIMouseEvent();

    void QueueLeftMouseUp( MousePos mousePos );
    void QueueLeftMouseDown( MousePos mousePos );
    void QueueRightMouseUp( MousePos mousePos );
    void QueueRightMouseDown( MousePos mousePosd );
    void QueueWheelUp( MousePos mousePos );
    void QueueWheelDown( MousePos mousePos );
    void QueueDrag( MousePos mousePos );

    bool ResolveQueuedEvent( void );

    static UIMouseEvent *GetInstance();

private:
    UIMouseEvent();


};

#endif // !defined(AFX_UIMOUSEEVENT_H__44302F53_27EC_11D3_8531_00E02922FA40__INCLUDED_)
