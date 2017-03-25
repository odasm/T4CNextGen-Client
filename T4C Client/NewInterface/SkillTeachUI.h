// SkillTeachUI.h
//

#if !defined(SkillTeachUI_h)
#define SkillTeachUI_h

#include "BoxUI.h"
#include "ListUI.h"
#include "ButtonUI.h"
#include "GraphUI.h"
#include "StaticTextUI.h"
#include <list>

class SkillTeachUI : public BoxUI
{
public:    
    ~SkillTeachUI( void );

    struct SellItemInfo{
        SellItemInfo() : skillID( 0 ), canHave( true ), price( 0 ), teach(false){
        }

	    DWORD  skillID;
        bool   canHave;
        string name;
        string reqs;
        DWORD  price;
        bool   teach;
        DWORD  skillPnts;
        DWORD  icon;
    };

    
    static SkillTeachUI *GetInstance();
    void Draw( V2SPRITEFX *vsfFX );
    int  TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);

    void Train( WORD skillPnts, const std::list< SellItemInfo > &sellItemList );

private:
    SkillTeachUI( void );

    class SellItemUI : public GraphUI{
    public:
        SellItemInfo itemInfo;
    };
    
    class SellButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } sellButtonEvent;

    class ItemListEvent : public EventVisitor{
    public:
        void LeftClicked( void );
        void LeftDblClicked();
    } itemListEvent;

    friend ItemListEvent;
    friend SellButtonEvent;

    GraphUI  sellGraphPatch;
    ButtonUI sellButton;
    ButtonUI  X_Button;
    ListUI   itemList;
    StaticTextUI reqDisp;
    StaticTextUI totalDisp;
    StaticTextUI leftDisp;
    StaticTextUI skillDisp;
    GraphUI  selectionSprite;
    GraphUI  backSell;
    GraphUI  listArrowDown;
    GraphUI  listArrowUp;

    WORD skillPnts;

};

#endif /* SellUI_h */
