// RTHelp.h: interface for the RTHelp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTHELP_H__INCLUDED_)
#define AFX_RTHELP_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoxUI.h"
#include "ButtonUI.h"
#include "GraphUI.h"

#define NBR_HELP_PAGE     12
#define NBR_LETTER_PAGE    2
#define NBR_MAP_PAGE       4

class RTHelp : public BoxUI  
{
public:	
	virtual ~RTHelp();

    static RTHelp *GetInstance();

    virtual void Draw( V2SPRITEFX *vsfFX );
    int TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);
    void    Show(bool putForeground = true,int dwShowWhat = 0,int dwValDef = 0);
    void    ShowSpecial(BYTE* pData, BYTE* pPal, int dwW, int dwH,char* pStrName);

private:
    RTHelp();

    class PrevButtonEvent : public EventVisitor
    {
      public:
           void LeftClicked( void );
    } prevButtonEvent;

    class NextButtonEvent : public EventVisitor
    {
      public:
           void LeftClicked( void );
    } nextButtonEvent;

    friend PrevButtonEvent;
    friend NextButtonEvent;


    void    DrawMap( void );
    void    SetHelpPageName();
    

    GraphUI   m_HelpPage[NBR_HELP_PAGE];
    GraphUI   m_LetterPage[NBR_LETTER_PAGE];
    GraphUI   m_MapPage[NBR_MAP_PAGE];
    GraphUI   m_LabyPage;
    GraphUI   m_BDImage;
    
    ButtonUI  X_Button;

    ButtonUI prevButton;
    ButtonUI nextButton;

    int m_dwCurrentPage;
    int m_dwShowWhat;

    char  m_strHeaderText[100];
};

#endif // !defined(AFX_RTHELP_H__INCLUDED_)
