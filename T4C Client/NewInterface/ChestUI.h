// *******************************************************************************************
// ***                                                                                     ***
//      File Name: ChestUI.h
//      Project:   TFC Client
//      Plateform: Win32
//      Creation:  11/04/2004
//      Author:    Scotch
// ***                                                                                     ***
// *******************************************************************************************
// ***                                                                                     ***
//      Change History
//
//         Date            Ver.      Author         Purpose
//         ----            ----      ------         -------
//         11/04/2004      1.0       Scotch         Initial developpement
//
//      Description
//          In Game Chest Interface header file.
//          
// ***                                                                                     ***
//	*******************************************************************************************
// ***           Copyright (c) 2004 Dialsoft           . All rights reserved.              ***

// Includes

#if !defined(ChestUI_h)
#define ChestUI_h

#include "BoxUI.h"
#include "ButtonUI.h"
#include "GridUI.h"
#include "GraphUI.h"
#include "DropZoneUI.h"
#include "StaticTextUI.h"
#include "EditUI.h"
#include "SliderUI.h"
#include "InventoryUI.h"

#endif // ChestUI_h

class ChestUI : public BoxUI
{
public:
	~ChestUI( void );

	static ChestUI *GetInstance( void );
   static unsigned int Display( void *null );
   void Open( void );
   void Close( void ); 
   
   void UpdateChest( TemplateList< BAG_ITEM > *newChest );
   void UpdateGold( void );
   BOOL UpdateItemsName (DWORD dwItemID, LPBYTE lpbItemName);

   void ClientShutdown( void );
   
   virtual void Draw( V2SPRITEFX *vsfFX );
   int  TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);
   bool VKeyInput( VKey vKey );
   
   void RequestMoveItemFromChestToBackpack( DWORD dwItemID, DWORD dwQty );
   void RequestMoveItemFromBackpackToChest( DWORD dwItemID, DWORD dwQty );


private:
	friend InventoryUI::InventoryGridEvent;
	friend InventoryUI::JunkButtonEvent;
	friend InventoryUI::DropButtonEvent;

	ChestUI( void );


	class ChestGridEvent : public EventVisitor{
    public:
        void Dropped( EventVisitor *sourceEvent );
        bool Dropping( void );
        void Dragged( void );
    } chestGridEvent;
	friend ChestGridEvent;

	typedef InventoryUI::ItemUI ItemUI;

    class QtySelectPopupUI : public BoxUI{
		public:
			QtySelectPopupUI();
         void Draw( V2SPRITEFX *vsfFX );
         int TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);

			enum State{
				FromBackpackToChest, FromChestToBackpack
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
         StaticTextUI textAreaDJ;
			EditUI amountEdit;

			GraphUI  backPopup;
         GraphUI  graphDJ;

			State state;
    } qtySelectPopup;


    void SaveChest( void );
    void LoadChest( void );
    
    ButtonUI  *junkButton;
    ButtonUI  *dropButton;
    ButtonUI  X_Button;
    
    GridUI	  *inventoryGrid;
    GridUI	  chestGrid;
    
    GraphUI   *invGridScrollUpGraph;
    GraphUI   *invGridScrollDownGraph;
    GraphUI   backChestT;
    GraphUI   backChestB;
    GraphUI   gridSelectedTile;
    ButtonUI  *iconGold;
    StaticTextUI *statGold;
    
    GraphUI   chestGridScrollUpGraph;
    GraphUI   chestGridScrollDownGraph;
    
    StaticTextUI *instantItemInfo;
    
    TemplateList< BAG_ITEM > tlChest;
};