// TextPageUI.h: interface for the TextPageUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTPAGEUI_H__E665B792_96BD_11D3_85B7_00E02922FA40__INCLUDED_)
#define AFX_TEXTPAGEUI_H__E665B792_96BD_11D3_85B7_00E02922FA40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ControlUI.h"
#include "../ScrollUI.h"
#include "../NewFont.h"
#include <list>

class TextPageUI : public ControlUI, public ScrollUI
{
public:
	TextPageUI();
	virtual ~TextPageUI();

    virtual bool WheelUp( MousePos mousePos );
    virtual bool WheelDown( MousePos mousePos );
    virtual void Draw( V2SPRITEFX *vsfFX );
    virtual bool RightClick( MousePos mousePos );
    virtual bool LeftMouseDown( MousePos mousePos );
    virtual bool LeftMouseUp( MousePos mousePos );
    virtual bool DragCycle( MousePos mousePos );
    virtual void SetPos( FramePos framePos );

    void AddText( std::string str, DWORD textColor, bool allowNewLine );
    void ClearList();

    void SetTextArea( FramePos itextFrame );
    void UpdateViewSize( void );
    void UpdateBacklogView( void );

private:    
    virtual void ScrollChanged( void );

    
    

    struct Paragraph{
        std::string text;
        DWORD textColor;
        DWORD fontSize;
        int lines;
        bool allowNewLine;
    };

    int maxLines;
    int minLineSize;
    int totalLines;
    int initialOffset;

    FramePos textFrame;

    list< Paragraph > textList;

    vector< TextObject * > screenText;
};

#endif // !defined(AFX_TEXTPAGEUI_H__E665B792_96BD_11D3_85B7_00E02922FA40__INCLUDED_)
