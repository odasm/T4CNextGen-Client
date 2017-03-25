// RTItemI.h: interface for the RTItemI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTITEMI_H__INCLUDED_)
#define AFX_RTITEMI_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoxUI.h"
#include "ButtonUI.h"
#include "GraphUI.h"


class RTItemI : public BoxUI  
{
public:	
	virtual ~RTItemI();

    static RTItemI *GetInstance();

    virtual void Draw( V2SPRITEFX *vsfFX );
    int TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);
    void Hide();

private:
    RTItemI();

    class CancelButtonEvent : public EventVisitor
    {
      public:
          void LeftClicked( void );        
    } cancelButtonEvent;
    
    friend CancelButtonEvent;
    
    ButtonUI cancelButton;
    
    GraphUI  backPopup;
    char m_strBoustName[44][30];
};

#endif // !defined(AFX_RTITEMI_H__INCLUDED_)
