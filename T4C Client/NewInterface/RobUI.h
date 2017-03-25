// RobUI.h: interface for the RobUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBUI_H__B28850E5_16DC_11D4_8681_00E02922FA40__INCLUDED_)
#define AFX_ROBUI_H__B28850E5_16DC_11D4_8681_00E02922FA40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoxUI.h"
#include "GridUI.h"
#include "ButtonUI.h"
#include "StaticTextUI.h"
#include <list>
#include <string>

class RobUI : public BoxUI  
{
public:	
	virtual ~RobUI();

    static RobUI *GetInstance();

    class Item{
    public:
        std::string itemName;
        WORD appearance;
        WORD baseId;
        DWORD id;
        DWORD qty;
    };

    virtual void Draw( V2SPRITEFX *vsfFX );
    int TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);

    void Rob( bool canRob, std::string targetName, std::list< Item > &itemList );
    void DispellRob();

    
    virtual void Hide();
private:
    RobUI();

    // A GameUI class to hold backpack items.
    class ItemUI : public GraphUI{
    public:
        Item itemData;
    };

    class ItemGridEvent : public EventVisitor{
    public:
        void LeftDblClicked();
        virtual bool Dropping();
    } itemGridEvent;

    class RobButtonEvent : public EventVisitor{
    public:
        void LeftClicked();
    } robButtonEvent;   

    friend ItemGridEvent;
    friend RobButtonEvent;

    void Rob();

    bool canRob;

    GraphUI   gridSelectedTile;
    GraphUI   gridScrollUpGraph;
    GraphUI   gridScrollDownGraph;

    GraphUI  backInv;
    GridUI   itemGrid;
    ButtonUI robButton;
    ButtonUI disabledRobButton;
    ButtonUI X_Button;
    StaticTextUI targetName;
    StaticTextUI instantItemInfo;
};

#endif // !defined(AFX_ROBUI_H__B28850E5_16DC_11D4_8681_00E02922FA40__INCLUDED_)
