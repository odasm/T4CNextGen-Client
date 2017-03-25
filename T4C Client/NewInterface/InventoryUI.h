// InventoryUI.h
//

#if !defined(InventoryUI_h)
#define InventoryUI_h

#include "BoxUI.h"
#include "ButtonUI.h"
#include "GridUI.h"
#include "GraphUI.h"
#include "DropZoneUI.h"
#include "StaticTextUI.h"
#include "MacroUI.h"
#include "SliderUI.h"
#include "EditUI.h"

class EquipEvent;
class GroundEvent;
class JunkEvent;
class ItemGridEvent;
class ItemMacroEvent;
class ChestUI;
class TradeUI;


class InventoryUI : public BoxUI
{
public:
  	friend ChestUI;
 	friend TradeUI;


    ~InventoryUI();

    void ReadDisplayBDImage();
    void ReadImageFile(int dwOffset, BYTE *pImage, BYTE *pPal);

    JunkEvent	*theJunkEvent;
    ItemGridEvent	*theItemGridEvent;
    ItemMacroEvent	*theItemMacroEvent;

    static InventoryUI *GetInstance();

    static unsigned int Display( void *null );
    void Hide();

    void RequestBackpackUpdateFromServer();
    void UpdateInventory( TemplateList< BAG_ITEM > *newInventory );
    void UpdateGold( void );

    // Synchonizes the equipment.
    void SyncEquipment( void );

    // Called when client is init/shutting
    void ClientShutdown( void );

    // Updates the character sheet.
    void UpdateCharacterSheet( void );

    void ShowItemName(UINT uiID);


    // A GameUI class to hold backpack items.
    class ItemUI : public GraphUI{
    public:
        // Called for help on item.
        bool RightMouseUp( MousePos mousePos );
        void UpdateName( void );

        void Setup();

        BAG_ITEM *bagItem;
    };

    virtual void Draw( V2SPRITEFX *vsfFX );

    void SetDefaultMacro( void );
    int TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);


private:
    // The different control events.    
    // Inventory grid click.
    class InventoryGridEvent : public EventVisitor{
    public:
        // Uses an inventory item.
        void LeftDblClicked();
        // Acknowledges dropped items.
        void Dropped( EventVisitor *sourceEvent );
        bool Dropping( void );
        void Dragged( void );
    } inventoryGridEvent;    
    class OtherGridEvent : public EventVisitor{
    public:
        // Acknowledges dropped items.
        bool Dropping( void );
    } otherGridEvent;

    // Junk button click
    class JunkButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } junkButtonEvent;  

    // Trade button click
    class TradeButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } tradeButtonEvent;    
    // Drop button click.
    class DropButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } dropButtonEvent;
    // Equipment event.
    class EquipEvent : public EventVisitor{
    public:
        void Dropped( EventVisitor *sourceEvent );
        bool Dropping( void );
        void Dragged( void );
        bool CanDrag( GameUI *dragUI );
    } equipEvent;

    friend EquipEvent;
    friend DropButtonEvent;
    friend JunkButtonEvent;

    void UpdateEquipDropZone( int bodyPos, DropZoneUI *dropZone );

    InventoryUI();

    void LoadBackpack( void );
    void SaveBackpack( void );

    // Macro callbacks.
    class MacroButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } macroButtonEvent;
    
    class ItemMacroEvent : public MacroEvent{
        virtual void ExecMacro( MacroDataUI *graphUI );
        virtual void SaveMacro( const list< MacroDataUI * > &macroList );
    } itemMacroEvent;

    friend ItemMacroEvent;
    friend MacroButtonEvent;
    
    class ItemMacroUI : public MacroDataUI{    
    public:
        string name;
        DWORD baseId;
        WORD  iconId;
    };

    void InitializeMacro( void );

    
    class JunkDropPopupUI : public BoxUI
    {
    public:
        JunkDropPopupUI();
        void Draw( V2SPRITEFX *vsfFX );
        int TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);

        enum State{
            Junk, Drop, JunkAck
        };

        void SetState( State s, DWORD maxQty, ItemUI *itemUI );
    private:
        ItemUI *itemUI;

        class OkButtonEvent : public EventVisitor{
        public:
            void LeftClicked( void );
        } okButtonEvent;

        class CancelButtonEvent : public EventVisitor{
        public:
            void LeftClicked( void );        
        } cancelButtonEvent;

        class SliderUpdateQtyEvent : public EventVisitor{
        public:
            void LeftClicked( void );
        } sliderUpdateQtyEvent;

        friend OkButtonEvent;
        friend CancelButtonEvent;
        friend SliderUpdateQtyEvent;

        // Text filter, only accepts numbers.
        class AmountEditFilter : public EditUI::Filter{
        public:
            bool IsOK( char nextCh, const std::string curText );
        } amountEditFilter;


		bool VKeyInput(VKey ivkey);

        ButtonUI okButton;
        ButtonUI cancelButton;
        
        
        GraphUI  backPopup;
        GraphUI  graphDJ;
        GraphUI  graphAsk;

        StaticTextUI textAreaDJ;
        StaticTextUI textAreaAsk;

        SliderUI amountSlider;
        EditUI amountEdit;

        GraphUI ackGraphPatch;

        State state;
    } junkDropPopup;

    friend JunkDropPopupUI;

    ButtonUI  macroButton;
    ButtonUI  TradeButton;
    ButtonUI  junkButton;
    ButtonUI  dropButton;
    ButtonUI  X_Button;
 public:
    GridUI    inventoryGrid;
 private:
    GraphUI   backInvT;
    GraphUI   backInvB;
    GraphUI   gridSelectedTile;

    GraphUI   gridScrollUpGraph;
    GraphUI   gridScrollDownGraph;


    // Equipment drop zones.
    DropZoneUI mainDropZone;
    DropZoneUI headDropZone;
    DropZoneUI bodyDropZone;
    DropZoneUI leftDropZone;
    DropZoneUI rightDropZone;
    DropZoneUI legsDropZone;
    DropZoneUI feetDropZone;
    DropZoneUI capeDropZone;
    DropZoneUI glovesDropZone;
    DropZoneUI neckDropZone;
    DropZoneUI wristDropZone;
    DropZoneUI beltDropZone;
    DropZoneUI ring1DropZone;
    DropZoneUI ring2DropZone;

    ButtonUI     iconSTR;
    ButtonUI     iconEND;
    ButtonUI     iconAGI;
    ButtonUI     iconWIS;
    ButtonUI     iconINT;
    ButtonUI     iconAC;
    ButtonUI     iconGold;

    StaticTextUI statSTR;
    StaticTextUI statEND;
    StaticTextUI statAGI;
    StaticTextUI statWIS;
    StaticTextUI statINT;
    StaticTextUI statAC;
    StaticTextUI statGold;

    StaticTextUI instantItemInfo;


    typedef struct _sImages
    {
       char strName[50];
       int  iOffset;
    }sImagesDisplay;

public:
    vector< sImagesDisplay > m_vImageDisplay;

    
};



#endif /* InventoryUI_h */
