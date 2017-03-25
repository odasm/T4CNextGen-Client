// SkillTrainUI.h
//

#if !defined(SkillTrainUI_h)
#define SkillTrainUI_h

#include "BoxUI.h"
#include "ListUI.h"
#include "ButtonUI.h"
#include "GraphUI.h"
#include "StaticTextUI.h"
#include <list>

class SkillTrainUI : public BoxUI
{
public:    
    ~SkillTrainUI( void );

    struct SellItemInfo{
        SellItemInfo() : skillID( 0 ), canHave( true ), price( 0 ), qty( 0 ), maxQty( 0 ){
        }

	    DWORD  skillID;
        bool   canHave;
        string name;
        DWORD  price;
        WORD   qty;
        WORD   initialQty;
        WORD   maxQty;
    };

    
    static SkillTrainUI *GetInstance();
    void Draw( V2SPRITEFX *vsfFX );
    int  TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);

    void Train( WORD skillPnts, const std::list< SellItemInfo > &sellItemList );

private:
    SkillTrainUI( void );

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
