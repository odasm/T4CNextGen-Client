#include "pch.h"
#include "SysMsg.h"
#include "NewFont.h"


SysMsg::SysMsg() : m_Time( 0 ), m_Text( NULL )
{
}

SysMsg::~SysMsg(){
    delete m_Text;
}

//////////////////////////////////////////////////////////////////////////////////////////
void SysMsg::SetText( const char *txt, COLORREF color, DWORD time, bool allowNewLine, int dwMaxWidth )
{
    m_Time = time + timeGetTime();
    delete m_Text;

    m_Text = FontManager::GetInstance()->GetTalkFont(TRUE);
    m_Text->EnableOutline();
    m_Text->SetText( txt, color, dwMaxWidth, 0, false, allowNewLine, true );
}

//////////////////////////////////////////////////////////////////////////////////////////
void SysMsg::Draw( int x, int y )
{
    if( m_Text == NULL ){
        return;
    }
    m_Text->Draw( x, y );
}

//////////////////////////////////////////////////////////////////////////////////////////
DWORD SysMsg::GetHeight()
{
    if( m_Text == NULL ){
        return 0;
    }
    return m_Text->GetHeight();
}