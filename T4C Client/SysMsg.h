#ifndef SYSMSG_H_
#define SYSMSG_H_

#include "Global.h"
extern Global g_Global;

class TextObject;
class SysMsg {
public:
    SysMsg();
    ~SysMsg();

    void SetText( const char *txt, COLORREF color, DWORD time, bool allowNewLine, int dwMaxWidth /*= g_Global.GetScreenW()*/ );

    void Draw( int x, int y );

    DWORD GetHeight();

    DWORD GetTime(){ return m_Time; }

private:
    TextObject *m_Text;
    DWORD m_Time;
};


#endif