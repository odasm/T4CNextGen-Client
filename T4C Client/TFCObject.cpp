// TFCObject.cpp: implementation of the TFCObject class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <windows.h>
#include "TFCObject.h"
#include "Icon3D.h"
#include "Global.h"
#include "MemoryManager.h"
#include "NewFont.h"

const int MaxOverheadLines = 3;

extern short TalkToOffset;
extern unsigned long TalkToID;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TFCObject::TFCObject() 
{
    Name = NULL;
	GuildName = NULL;
    m_DisplayName = NULL;
	m_DisplayGuildName = NULL;//BLBLBL
    m_TalkingText = NULL;
    allowOutOfBound = false;
}

//////////////////////////////////////////////////////////////////////////////////////////
TFCObject::~TFCObject()
{
    Deplacement *D;
    
    while ((D = MovingQueue.Retreive())) {
        delete D;
    }
    
    if (Name)
        delete Name;
	if (GuildName)//BLBLBL
		delete GuildName;//BLBLBL
    
    delete m_DisplayName;    
	delete m_DisplayGuildName;//BLBLBL
    delete m_TalkingText;
    
    onAttack[0].Release(FALSE);
    onAttack[1].Release(FALSE);
    onAttack[2].Release(FALSE);
    onAttacked[0].Release(FALSE);
    onAttacked[1].Release(FALSE);
    onAttacked[2].Release(FALSE);
    onKilled[0].Release(FALSE);
    onKilled[1].Release(FALSE); 
}

//////////////////////////////////////////////////////////////////////////////////////////
bool TFCObject::operator <( const TFCObject *&x ){
    if( OC + OY > x->OC + x->OY ){
        return true;
    }
    if( OX > x->OX ){
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCObject::InitTalkText( COLORREF textColor )
{
    delete m_TalkingText;
    m_TalkingText = FontManager::GetInstance()->GetTalkFont(TRUE);
    
    const int MaxOverheadTextWidth = 300;
    const int OverheadLineSpacing = 0;
    
    m_TalkingText->EnableOutline();
    m_TalkingText->EnableCenterJustification();
    m_TalkingText->SetText( ".", textColor, MaxOverheadTextWidth, OverheadLineSpacing, false, true, false );
    m_TalkingText->ResetText("");
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCObject::SetTalkText( const char *txt, COLORREF textColor )
{
    if( m_TalkingText == NULL ){
        InitTalkText( textColor );
    }
    
    m_TalkingText->SetMaxLines( MaxOverheadLines, 0 );
    bool forceRedo = false;
    if( stricmp( txt, m_TalkingText->GetText() ) == 0 ){
        forceRedo = true;
    }
    m_TalkingText->ResetText( txt );
    
    if( forceRedo ){
        m_TalkingText->Redo();
    }
    
    if( ID == TalkToID ){
        TalkToOffset = m_TalkingText->GetTruncatedTextLineOffset();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCObject::SetTalkOffset( DWORD talkOffset )
{
    if( !IsTalkTextDrawn() ){
        return;
    }
    
    m_TalkingText->SetMaxLines( MaxOverheadLines, talkOffset );
    m_TalkingText->Redo();
    
    if( ID == TalkToID ){
        TalkToOffset = m_TalkingText->GetTruncatedTextLineOffset();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool TFCObject::IsTalkTextDrawn()
{
    if( m_TalkingText == NULL ){
        return false;
    }
    if( m_TalkingText->IsEmpty() ){
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCObject::DrawTalkText( int x, int y ){
    if( !IsTalkTextDrawn() )
    {
        return;
    }    
    
//    int toto = m_TalkingText->GetWidth();//BLBLBL 29 mars 2009 suppression des trucs inutilisés
    x -= m_TalkingText->GetWidth() / 2;
    y -= m_TalkingText->GetHeight() / 2;
    m_TalkingText->Draw( x, y, NULL );
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCObject::StopTalkText()
{
    delete m_TalkingText;
    m_TalkingText = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////
LPCTSTR TFCObject::GetTalkText()
{
    if( !IsTalkTextDrawn() ){
        return "";
    }
    return m_TalkingText->GetText();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCObject::InitNameText()
{
    delete m_DisplayName;
    m_DisplayName = FontManager::GetInstance()->GetTalkFont(TRUE);
	    
    const int MaxOverheadTextWidth = 300;
    const int OverheadLineSpacing = 0;
    
    m_DisplayName->EnableOutline();
    m_DisplayName->EnableCenterJustification();
    m_DisplayName->SetText( ".", NameColor, MaxOverheadTextWidth, OverheadLineSpacing, false, true, false );
    m_DisplayName->ResetText("");
}

void TFCObject::InitGuildNameText()//BLBLBL
{
	
	delete m_DisplayGuildName;//BLBLBL
	m_DisplayGuildName = FontManager::GetInstance()->GetTalkFont(TRUE);//BLBLBL
    
    const int MaxOverheadTextWidth = 300;
    const int OverheadLineSpacing = 0;
    
    m_DisplayGuildName->EnableOutline();//BLBLBL
    m_DisplayGuildName->EnableCenterJustification();//BLBLBL
    m_DisplayGuildName->SetText( ".", GuildColor , MaxOverheadTextWidth, OverheadLineSpacing, false, true, false );//BLBLBL
    m_DisplayGuildName->ResetText("");//BLBLBL

}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCObject::DisplayName()
{
    if( m_DisplayName == NULL ){
        InitNameText();
    }

    m_DisplayName->ResetText( Name );

	if( m_DisplayGuildName == NULL ){//BLBLBL
		InitGuildNameText();//BLBLBL
	}

	m_DisplayGuildName->ResetText( GuildName );//BLBLBL
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCObject::DrawName( int x, int y )
{
    if( !IsNameDrawn() ){
        return;
    }    
       	 	
	if( lstrlen( m_DisplayGuildName->GetText() ) > 0 )
	{
		y -= m_DisplayGuildName->GetHeight() / 2;//BLBLBL	
		m_DisplayGuildName->Draw( x-m_DisplayGuildName->GetWidth() / 2, y, NULL );//BLBLBL
		y -= 4;
	}

	//x -= m_DisplayName->GetWidth() / 2;
    y -= m_DisplayName->GetHeight() / 2;	
    m_DisplayName->Draw( x-m_DisplayName->GetWidth() / 2, y, NULL );    

}

//////////////////////////////////////////////////////////////////////////////////////////
bool TFCObject::IsNameDrawn()
{
    if( m_DisplayName == NULL || m_DisplayGuildName == NULL ){
        return false;
    }
    if( m_DisplayName->IsEmpty() ){
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCObject::StopNameDisplay()
{
    delete m_DisplayName;
	delete m_DisplayGuildName;//BLBLBL
    m_DisplayName = NULL;
	m_DisplayGuildName = NULL;//BLBLBL
}