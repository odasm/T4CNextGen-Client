// SellUI.h
//

#if !defined(SellUI_h)
#define SellUI_h

#include "BoxUI.h"
#include "ListUI.h"
#include "ButtonUI.h"
#include "GraphUI.h"
#include "StaticTextUI.h"
#include <list>

class SellUI : public BoxUI
{
public:    
    ~SellUI( void );

    struct SellItemInfo{
        SellItemInfo() : dwID( 0 ), appearance( 0 ), price( 0 ), qty( 0 ){
        }

	    DWORD  dwID;
        WORD   appearance;
        DWORD  price;
        string name;
        DWORD  qty;
        DWORD  maxQty;
    };

    
    static SellUI *GetInstance();
    void Draw( V2SPRITEFX *vsfFX );
    int  TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);

    void Sell( const std::list< SellItemInfo > &sellItemList );

private:
    SellUI( void );

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

    ButtonUI sellButton;
    ButtonUI  X_Button;
    ListUI   itemList;
    StaticTextUI reqDisp;
    StaticTextUI totalDisp;
    StaticTextUI leftDisp;
    GraphUI  selectionSprite;
    GraphUI  backSell;

    GraphUI  listArrowDown;
    GraphUI  listArrowUp;

};

#endif /* SellUI_h */
