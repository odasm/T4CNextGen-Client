// *******************************************************************************************
// ***                                                                                     ***
//      File Name: TradeUI.h
//      Project:   TFC Client
//      Plateform: Win32
//      Creation:  08/12/2004
//      Author:    Carlos
// ***                                                                                     ***
// *******************************************************************************************
// ***                                                                                     ***
//      Description
//          In Game Trade Interface header file.
//          
// ***                                                                                     ***
//	*******************************************************************************************
// ***           Copyright (c) 2004 Dialsoft           . All rights reserved.              ***

// Includes

#if !defined(TradeUI_h)
#define TradeUI_h

#include "BoxUI.h"
#include "ButtonUI.h"
#include "GridUI.h"
#include "GraphUI.h"
#include "DropZoneUI.h"
#include "StaticTextUI.h"
#include "EditUI.h"
#include "SliderUI.h"
#include "InventoryUI.h"

#endif // TradeUI_h

class TradeUI : public BoxUI
{
	friend bool InventoryUI::InventoryGridEvent::Dropping();
public:
	enum CharacterStatus
	{
		CS_EDITINGITEMS=0,
		CS_READY=1,
		CS_CONFIRMED=2
	};
	enum AffectedCharacter
	{
		AC_MYSELF=0,
		AC_OTHER=1,
		AC_BOTH=2
	};
public:
	~TradeUI( void );

	static TradeUI *GetInstance( void );
	static unsigned int Display( void *null );
	void Hide();
	void Show(bool putForeground = true);
   void Draw( V2SPRITEFX *vsfFX );
   int  TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);
	bool VKeyInput( VKey vKey );

	void UpdateTrade( TemplateList< BAG_ITEM > *newTrade, AffectedCharacter pAffected );
	void UpdateGold( void );
	void UpdateItemsName (DWORD dwItemID, LPBYTE lpbItemName, ePlacesToSearch pWhereToLook);

	
	static void RequestTradeInvite(DWORD dwID, short xPos, short yPos);
	static void RequestTradeCancel();
	static void RequestTradeSetStatus(TradeUI::CharacterStatus Status);
	static void RequestTradeClear();
	static void RequestMoveItemFromTradeToBackpack( DWORD dwItemID, DWORD dwQty );
	static void RequestMoveItemFromBackpackToTrade( DWORD dwItemID, DWORD dwQty );
	static unsigned int InvitePlayer( void *data );
	void EventPlayerInvited( char *pCharacterName, DWORD pInvitorID, short pX, short pY);
	void EventTradeStarted ( char *pCharacterName );
	void EventTradeCanceled ();
	void EventTradeFinished();
	void EventTradeStatusChanged(CharacterStatus pMyStatus, CharacterStatus pOtherStatus);
	void EventDroppingItemUIFromTradeToBackpack();

private:

	TradeUI( void );
	void UpdateInterfaceStatus();
	void ResetTrade(); // Reset all trade data
   
   // Data members //
   
   ButtonUI  *junkButton;
   ButtonUI  *dropButton;
   
   GridUI	  *inventoryGrid;
   GridUI	  tradeMyGrid;
   GridUI    tradeOtherGrid;
   
   GraphUI   *invGridScrollUpGraph;
   GraphUI   *invGridScrollDownGraph;
   GraphUI   intlTitleGraphPatch;
   GraphUI   gridSelectedTile;
   ButtonUI  *iconGold;
   StaticTextUI *statGold;

   GraphUI   backInvT;
   GraphUI   backInvB;
   
   GraphUI   tradeMyGridScrollUpGraph;
   GraphUI   tradeMyGridScrollDownGraph;
   GraphUI   tradeOtherGridScrollUpGraph;
   GraphUI   tradeOtherGridScrollDownGraph;
   ButtonUI  X_Button;
   ButtonUI	mReadyButtonON, mReadyButtonOFF, disabledReadyButton;
   ButtonUI	mConfirmButtonON, mConfirmButtonOFF, disabledConfirmButton;
   ButtonUI	clearButton, disabledClearButton;
   ButtonUI	inviteButton;
   ButtonUI	cancelButton;
   ButtonUI	mOtherReadyButton, mDisabledOtherReadyButton;
   ButtonUI	mOtherConfirmButton, mDisabledOtherConfirmButton;
   
   StaticTextUI *instantItemInfo;

   char m_strMyInfo[100];
   char m_strOtherInfo[100];
   
   TemplateList< BAG_ITEM > tlMyTrade;
	TemplateList< BAG_ITEM > tlOtherTrade;
	BOOL	trading;
	CharacterStatus mMyStatus;
	CharacterStatus mOtherStatus;

// Friends List //
//	friend InventoryUI::InventoryGridEvent;
//	friend InventoryUI::JunkButtonEvent;
//	friend InventoryUI::DropButtonEvent;

	// Internal classes... //
	
	// Start of EVENT HANDLERS //
	
	// My trade grid events
	class TradeGridEvent : public EventVisitor{
    public:
        void Dropped( EventVisitor *sourceEvent );
        bool Dropping( void );
        void Dragged( void );
    } tradeGridEvent;
	friend TradeGridEvent;

    // Ready button click
    class ReadyButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } readyButtonEvent;    
	friend ReadyButtonEvent;

    // Confirm button click
    class ConfirmButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } confirmButtonEvent;    
	friend ConfirmButtonEvent;

    // Clear button click
    class ClearButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } clearButtonEvent;    
	friend ClearButtonEvent;

    // Cancel button click
    class CancelButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } cancelButtonEvent;    
	friend CancelButtonEvent;

    // Invite button click
    class InviteButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } inviteButtonEvent;    
	friend InviteButtonEvent;

	// End of EVENT HANDLERS //


    // Popup Box for trade invite (accept/ignore)
    class JoinPopupUI : public BoxUI {
    public:
        JoinPopupUI();

        // Used to intercept ESC.
        virtual bool VKeyInput( VKey vKey );

        void Draw( V2SPRITEFX *vsfFX );
        int  TxtOff(Font *, HDC, char*,int) ;

        void SetText( std::string text );

        void Decline( void );

		void SetInvitorInfo(DWORD pCharID, WORD pXPos, WORD pYPos);

    private:
        class AcceptButtonEvent : public EventVisitor{
        public:
            void LeftClicked( void );
        } acceptButtonEvent;

        class DeclineButtonEvent : public EventVisitor{
        public:
            void LeftClicked( void );        
        } declineButtonEvent;

        friend AcceptButtonEvent;
        friend DeclineButtonEvent;        

        ButtonUI acceptButton;
        ButtonUI declineButton;
        GraphUI  JoinBack;
        GraphUI  graphPatch;
        
        StaticTextUI textArea;

		DWORD mInvitorID;
		WORD mInvitorXPos;
		WORD mInvitorYPos;

    } joinPopup;
    friend JoinPopupUI;

	typedef InventoryUI::ItemUI ItemUI;

    // Popup Box for quantity selecting
	class QtySelectPopupUI : public BoxUI{
		public:
			QtySelectPopupUI();
         void Draw( V2SPRITEFX *vsfFX );
         int  TxtOff(Font *, HDC, char*,int) ;

			enum State{
				ST_FROMBACKPACKTOTRADE, ST_FROMTRADETOBACKPACK
			};

			void SetState( State s, DWORD maxQty, ItemUI *itemUI );
			void SetDelay( DWORD dwDelay_ms );
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

			DWORD dwDelayUntill;
			
			ButtonUI okButton;
			ButtonUI cancelButton;
        
			SliderUI amountSlider;
			EditUI amountEdit;
         StaticTextUI textAreaDJ;

         GraphUI  backPopup;
         GraphUI  graphDJ;


			State state;
    } qtySelectPopup;
};