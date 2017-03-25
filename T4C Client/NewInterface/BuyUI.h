// BuyUI.h
//

#if !defined(BuyUI_h)
#define BuyUI_h

#include "BoxUI.h"
#include "ListUI.h"
#include "ButtonUI.h"
#include "GraphUI.h"
#include "StaticTextUI.h"
#include <list>

class BuyUI : public BoxUI
{
public:    
    ~BuyUI( void );

    struct SellItemInfo{
        SellItemInfo() : dwID( 0 ), appearance( 0 ), price( 0 ), qty( 0 ){
        }

	    DWORD  dwID;
        WORD   appearance;
        DWORD  price;
        string name;
        string reqs;
        DWORD  qty;
        bool   canEquip;
    };

    
    static BuyUI *GetInstance();
    void Draw( V2SPRITEFX *vsfFX );
    int  TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);

    void Buy( const std::list< SellItemInfo > &sellItemList );

private:
    BuyUI( void );

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

    GraphUI  backBuy;
    GraphUI  listArrowUp;
    GraphUI  listArrowDown;

    ButtonUI  sellButton;
    ButtonUI  X_Button;
    ListUI   itemList;
    StaticTextUI reqDisp;
    StaticTextUI totalDisp;
    StaticTextUI leftDisp;
    GraphUI  selectionSprite;

};

#endif /* BuyUI_h */
