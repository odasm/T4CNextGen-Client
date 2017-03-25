// *******************************************************************************************
// ***                                                                                     ***
//      File Name: TradeUI.cpp
//      Project:   TFC Client
//      Plateform: Win32
//      Creation:  08/12/2005
//      Author:    Carlos Lima <carlos@dialsoft.com>
// ***                                                                                     ***
// *******************************************************************************************
// ***                                                                                     ***
//      Description
//          In Game Trade Interface.
//          
// ***                                                                                     ***
//	*******************************************************************************************
// ***           Copyright (c) 2005 Dialsoft           . All rights reserved.              ***

// Includes

#include "../pch.h"
#include "TradeUI.h"
#include "RootBoxUI.h"
#include "GraphUI.h"
#include "../MemoryManager.h"
#include "ChatterUI.h"
#include "../Bitmap.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]

extern Bitmap GridID;
extern Font  *fNewGuiBtnFont;

using namespace std;

namespace{
    T3VSBSound *dropSound = NULL;
    T3VSBSound *naSound = NULL;

    void InitSound(){
        if( dropSound == NULL ){
            dropSound = new T3VSBSound;
            dropSound->Create( "Generic drop item", TS_MEMORY );

            naSound = new T3VSBSound;
            naSound->Create( "General NA sound", TS_MEMORY );
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    bool DropBack
    //////////////////////////////////////////////////////////////////////////////////////////
    //  Drops back the given item UI in its original ui.
    // 
    (
     GameUI *itemUI // 
    )
    //////////////////////////////////////////////////////////////////////////////////////////
    {
        GameUI *src = GameUI::GetDragParentUI();
        if( src != NULL ){
            // If the source is a grid UI
            if( typeid( *src ) == typeid( GridUI ) ){
                // Try to see if the grid has space left.
                GridUI *srcGrid = static_cast< GridUI * >( src );

                // If the grid spot isn't taken
                if( srcGrid->CanDrop( GameUI::GetDragInitialPos() ) ){
                    // Redrop the item
                    src->LeftMouseDown( GameUI::GetDragInitialPos() );
                }else{
                    // Otherwise add the item.
                    srcGrid->AddItem( itemUI );
                    GameUI::CancelDrag();
                    CMouseCursor::GetInstance()->SetCustomCursor( NULL );
                }

            }else{
                // Drop the item where it was first fetched.
                src->LeftMouseDown( GameUI::GetDragInitialPos() );
            }
            return true;
        }
        return false;
    };
}

TradeUI::TradeUI( void ) : junkButton( &(InventoryUI::GetInstance()->junkButton) ),
						   dropButton( &(InventoryUI::GetInstance()->dropButton) ),
						   mReadyButtonON( readyButtonEvent ),
						   mReadyButtonOFF( readyButtonEvent ),
						   mConfirmButtonON( confirmButtonEvent ),
						   mConfirmButtonOFF( confirmButtonEvent ),
						   clearButton( clearButtonEvent ),
						   disabledReadyButton( EmptyEvent::GetInstance() ),
						   disabledConfirmButton( EmptyEvent::GetInstance() ),
						   disabledClearButton( EmptyEvent::GetInstance() ),
						   inviteButton( inviteButtonEvent ),
						   cancelButton( cancelButtonEvent ),
						   mOtherReadyButton( EmptyEvent::GetInstance() ),
						   mOtherConfirmButton( EmptyEvent::GetInstance() ),
						   mDisabledOtherConfirmButton( EmptyEvent::GetInstance() ),
						   mDisabledOtherReadyButton( EmptyEvent::GetInstance() ),
						   inventoryGrid( &(InventoryUI::GetInstance()->inventoryGrid) ),
						   tradeMyGrid( tradeGridEvent ),
						   tradeOtherGrid( EmptyEvent::GetInstance() ),
						   invGridScrollDownGraph( &(InventoryUI::GetInstance()->gridScrollDownGraph) ),
						   invGridScrollUpGraph( &(InventoryUI::GetInstance()->gridScrollUpGraph) ),
						   iconGold( &(InventoryUI::GetInstance()->iconGold) ),
						   statGold( &(InventoryUI::GetInstance()->statGold) ),
						   instantItemInfo( &(InventoryUI::GetInstance()->instantItemInfo) ),
                     X_Button( hideEvent )
{
   InitSound();

   // Create the GUI
   int dwXPos = (g_Global.GetScreenW()-550)/2;
   int dwYPos = (g_Global.GetScreenH()-150-430)/2;
   if(dwYPos <0)
       dwYPos =0;
   
   SetPos( FramePos( dwXPos, dwYPos, dwXPos+550, dwYPos+430 ) );
   
   backInvT.SetSpriteId( "GUI_BackTrade" );
   backInvT.SetPos( FramePos( dwXPos, dwYPos, dwXPos+550, dwYPos+260 ) );
   backInvT.DrawAlpha(true);
   
   backInvB.SetSpriteId( "GUI_InvBackB" );
   backInvB.SetPos( FramePos( dwXPos, dwYPos+260, dwXPos+550, dwYPos+430 ) );
   backInvB.DrawAlpha(true);

   X_Button.SetPos( FramePos( dwXPos+550-28, dwYPos+0, dwXPos+550-4, dwYPos+0+24 ) );
   X_Button.SetSpriteId( "GUI_X_ButtonUp" );
   X_Button.SetDownUI( "GUI_X_ButtonDown" );
   X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
   SetHelpId( X_Button, 261 );
   
   // Setup the different control positions.
   mReadyButtonON.SetPos ( FramePos( dwXPos+28, dwYPos+210, dwXPos+28+32, dwYPos+210+30 ) );
   mReadyButtonON.SetSpriteId( "ready_button_ON" );
   mReadyButtonON.SetDownUI( "ready_button_ON_HL" );
   mReadyButtonON.SetHighlightUI( "ready_button_ON_HL" );
   readyButtonEvent.SetUI( this );
   SetHelpId( mReadyButtonON, 562 );
   
   mReadyButtonOFF.SetPos ( FramePos( dwXPos+28, dwYPos+210, dwXPos+28+32, dwYPos+210+30 ) );
   mReadyButtonOFF.SetSpriteId( "ready_button_OFF" );
   mReadyButtonOFF.SetDownUI( "ready_button_OFF_HL" );
   mReadyButtonOFF.SetHighlightUI( "ready_button_OFF_HL" );
   readyButtonEvent.SetUI( this );
   SetHelpId( mReadyButtonOFF, 562 );     
   
   mConfirmButtonON.SetPos ( FramePos( dwXPos+66, dwYPos+210, dwXPos+66+32, dwYPos+210+30 ) );
   mConfirmButtonON.SetSpriteId( "confirm" );     
   mConfirmButtonON.SetDownUI( "confirm_clicked" );
   mConfirmButtonON.SetHighlightUI( "confirm_hl" );
   confirmButtonEvent.SetUI( this );
   SetHelpId( mConfirmButtonON, 563 );     
   
   mConfirmButtonOFF.SetPos ( FramePos( dwXPos+66, dwYPos+210, dwXPos+66+32, dwYPos+210+30 ) );
   mConfirmButtonOFF.SetSpriteId( "confirm_off" );     
   mConfirmButtonOFF.SetDownUI( "confirm_off_clicked" );
   mConfirmButtonOFF.SetHighlightUI( "confirm_off_hl" );
   confirmButtonEvent.SetUI( this );
   SetHelpId( mConfirmButtonOFF, 563 );     
   
   disabledConfirmButton.SetPos( FramePos( dwXPos+66, dwYPos+210, dwXPos+66+32, dwYPos+210+30 ) );
   disabledConfirmButton.SetSpriteId( "64kEquipmentButtonXUp" );
   disabledConfirmButton.EnableSound(false);
   SetHelpId( disabledConfirmButton, 563 );
   
   clearButton.SetPos ( FramePos( dwXPos+122, dwYPos+210, dwXPos+122+32, dwYPos+210+30  ) );
   clearButton.SetSpriteId( "Clear_button" );     
   clearButton.SetDownUI( "Clear_button_HL" );
   clearButton.SetHighlightUI( "Clear_button_HL" );
   clearButtonEvent.SetUI( this );
   SetHelpId( clearButton, 564 );      

	inviteButton.SetPos ( FramePos( dwXPos+493, dwYPos+280, dwXPos+493+32, dwYPos+280+30 ) );
	inviteButton.SetSpriteId( "invite" );     
	inviteButton.SetDownUI( "invite_clicked" );
	inviteButton.SetHighlightUI( "invite_hl" );
	inviteButtonEvent.SetUI( this );
	SetHelpId( inviteButton, 566 );     

	cancelButton.SetPos ( FramePos(dwXPos+160, dwYPos+210, dwXPos+160+32, dwYPos+210+30 ) );
	cancelButton.SetSpriteId( "cancel" );     
	cancelButton.SetDownUI( "cancel_clicked" );
	cancelButton.SetHighlightUI( "cancel_hl" );
	cancelButtonEvent.SetUI( this );
	SetHelpId( cancelButton, 565 );

	mOtherReadyButton.SetPos( FramePos(dwXPos+352, dwYPos+210, dwXPos+352+32, dwYPos+210+30 ) );
	mOtherReadyButton.SetSpriteId( "ready_button_OFF_HL" );
	mOtherReadyButton.EnableSound(false);
	SetHelpId( mOtherReadyButton, 567 );

	mOtherConfirmButton.SetPos( FramePos(dwXPos+392, dwYPos+210, dwXPos+392+32, dwYPos+210+30 ) );
	mOtherConfirmButton.SetSpriteId( "ready_button_OFF_HL" );
	mOtherConfirmButton.EnableSound(false);
	SetHelpId( mOtherConfirmButton, 568 );

   gridSelectedTile.SetSpriteId( "GUI_BackInvGridSelect" );

	// Setup my grid
	tradeMyGrid.SetPos( FramePos( dwXPos+23, dwYPos+78, dwXPos+227, dwYPos+194 ) );
	tradeMyGrid.SetScrollRegions(
		FramePos( dwXPos+204, dwYPos+82 , dwXPos+222, dwYPos+103 ),
		FramePos( dwXPos+204, dwYPos+168, dwXPos+222, dwYPos+188 ),
		FramePos( dwXPos+204, dwYPos+104, dwXPos+222, dwYPos+160 ),
		NULL
	);
	tradeMyGrid.SetGrid( 6, 4, FramePos( 0, 0, 26, 26 ), FramePos( 0, 0, 3, 3 ) );
	tradeMyGridScrollUpGraph  .SetPos( FramePos( dwXPos+204, dwYPos+82 , dwXPos+222, dwYPos+103 ) );
	tradeMyGridScrollDownGraph.SetPos( FramePos( dwXPos+204, dwYPos+168, dwXPos+222, dwYPos+188 ) );
	tradeMyGrid.SetScrollUpDownUI( &tradeMyGridScrollUpGraph );
	tradeMyGrid.SetScrollDownDownUI( &tradeMyGridScrollDownGraph );
	tradeMyGrid.SetSelectedTileUI( &gridSelectedTile );
	SetHelpId(tradeMyGrid, 569);

	// Setup other party's grid
	tradeOtherGrid.SetPos( FramePos( dwXPos+253, dwYPos+78, dwXPos+459, dwYPos+194 ) );
	tradeOtherGrid.SetScrollRegions(
		FramePos( dwXPos+436, dwYPos+82 , dwXPos+454, dwYPos+103 ), 
      FramePos( dwXPos+436, dwYPos+168, dwXPos+454, dwYPos+188 ),
      FramePos( dwXPos+436, dwYPos+104, dwXPos+454, dwYPos+160 ),
		NULL
	);
   
	tradeOtherGrid.SetGrid( 6, 4, FramePos( 0, 0, 26, 26 ), FramePos( 0, 0, 3, 3 ) );
	tradeOtherGridScrollUpGraph  .SetPos( FramePos( dwXPos+436, dwYPos+82 , dwXPos+454, dwYPos+103 ) );
	tradeOtherGridScrollDownGraph.SetPos( FramePos( dwXPos+436, dwYPos+168, dwXPos+454, dwYPos+188 ) );
	tradeOtherGrid.SetScrollUpDownUI( &tradeOtherGridScrollUpGraph );
	tradeOtherGrid.SetScrollDownDownUI( &tradeOtherGridScrollDownGraph );
	tradeOtherGrid.AllowDrag( false );
	SetHelpId(tradeOtherGrid, 570);

   sprintf(m_strMyInfo   ,"");
   sprintf(m_strOtherInfo,"");

   AddChild( &backInvT);
   AddChild( &backInvB);
   AddChild( &X_Button);
   AddChild( &inviteButton );
	AddChild( inventoryGrid );
	AddChild( &tradeMyGrid );
	AddChild( &tradeOtherGrid );
	AddChild( dropButton );
	AddChild( junkButton );
	AddChild( &intlTitleGraphPatch );
	AddChild( iconGold );
	AddChild( statGold );
	AddChild( &disabledConfirmButton );
	AddChild( &mReadyButtonON );
	AddChild( &mReadyButtonOFF );
	AddChild( &mConfirmButtonON );
	AddChild( &mConfirmButtonOFF );
	AddChild( &clearButton );
	AddChild( &cancelButton );
	AddChild( &mOtherReadyButton );
	AddChild( &mOtherConfirmButton );


	// Add the trade to the root box.
	RootBoxUI::GetInstance()->AddChild( this ); 

	trading = FALSE;
	UpdateInterfaceStatus();

}

TradeUI::~TradeUI( void )
{
}

TradeUI *TradeUI::GetInstance( void )
{
	static TradeUI m_pInstance;
	return &m_pInstance;
}

unsigned int TradeUI::Display( void *null )
{
	CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );

	if( GetInstance()->IsShown() ){    
		GetInstance()->Hide();
	}else{
		// Update backpack
		InventoryUI::GetInstance()->RequestBackpackUpdateFromServer();
		// Show box.
		GetInstance()->Show();
	}

	return 0;
}

void TradeUI::Hide()
{
	BoxUI::Hide();
	if (trading == TRUE) RequestTradeCancel();
}

void TradeUI::Show(bool putForeground)
{
	UpdateInterfaceStatus();
	BoxUI::Show(putForeground);
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the inventory UI.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::Draw(V2SPRITEFX *vsfFX)
{
   int dwXPos = (g_Global.GetScreenW()-550)/2;
   int dwYPos = (g_Global.GetScreenH()-150-430)/2;
   if(dwYPos <0)
       dwYPos =0;
   GameUI::Draw( vsfFX );

   HDC hdc;
   DXDGetDC(&hdc, 740);

   // Draw les texte...
   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[26]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 478, dwYPos+332+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[27]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 478, dwYPos+368+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   // Draw GOLD id...
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[24]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,52)+ 26+1, dwYPos+395+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,52)+ 26  , dwYPos+395   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   // My Pack Info
   sprintf(strBtnTexte  ,"%s",m_strMyInfo);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,160)+ 32+1, dwYPos+47+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,160)+ 32  , dwYPos+47   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   // Other Pack Info...
   sprintf(strBtnTexte  ,"%s",m_strOtherInfo);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,160)+ 265+1, dwYPos+47+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,160)+ 265  , dwYPos+47   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   // Draw Box Header
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[25]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,99)+ 226+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,99)+ 226  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);


   DXDReleaseDC(hdc, 740);
   
   // Get the current mouse coordinates.
   MousePos mousePos( MouseDevice::xPos, MouseDevice::yPos );
   
   ItemUI * itemUI = NULL;
   ePlacesToSearch whereToSearch = PL_SEARCHBACKPACK;
   if( inventoryGrid->GetPos().MousePosInFrame( mousePos ) )
   {
      itemUI = static_cast< ItemUI * >( inventoryGrid->GetGridItemAt( mousePos ) );
      whereToSearch = PL_SEARCHBACKPACK;
   } 
   else if( tradeMyGrid.GetPos().MousePosInFrame( mousePos ) ) 
   {
      itemUI = static_cast< ItemUI * >( tradeMyGrid.GetGridItemAt( mousePos ) );
      whereToSearch = PL_SEARCHMYTRADE;
   } 
   else if( tradeOtherGrid.GetPos().MousePosInFrame( mousePos ) ) 
   {
      itemUI = static_cast< ItemUI * >( tradeOtherGrid.GetGridItemAt( mousePos ) );
      whereToSearch = PL_SEARCHOTHERTRADE;
   }
   
   // Display the instant help text.
   if( itemUI != NULL )
   {
      TextObject *to = itemUI->GetHelpText();
      LPBAG_ITEM bagItem = itemUI->bagItem;
      
      // If the item doesn't have a name.
      if( to == NULL && bagItem->chName[ 0 ] == 0 )
      {
         // Send a query item's name.
         TFCPacket sending;
         
         sending << (RQ_SIZE)RQ_QueryItemName;
         sending << (char)whereToSearch;
         sending << (long)bagItem->dwID;
         SEND_PACKET( sending );
         
         // Avoid the drawing thread to send continuous QueryItemName packets.
         bagItem->chName[ 0 ] = ' ';
         bagItem->chName[ 1 ] = 0;
      }
      else
      {
         if( to != NULL )
         {
            instantItemInfo->SetText(to->GetText(),RGB( 223, 157, 0 ),246);
            instantItemInfo->Draw( vsfFX );
         }
      }        
   }
}

int TradeUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}


//////////////////////////////////////////////////////////////////////////////////////////
bool TradeUI::VKeyInput
//////////////////////////////////////////////////////////////////////////////////////////
// Virtual key inputs
// 
(
 VKey vKey
)
// Return: virtual bool, true
//////////////////////////////////////////////////////////////////////////////////////////
{
	if (vKey.GetKey() == VK_ESCAPE) Hide();
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::UpdateTrade( TemplateList< BAG_ITEM > *newTrade, AffectedCharacter pAffected )
//////////////////////////////////////////////////////////////////////////////////////////
// Updates the tradepack
{
    LOG << "\r\nReceived tradepack update.";
    
    // Lock the root box since this call came from some unknown thread.
    CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );

	TemplateList< BAG_ITEM > *tlTheTrade;
	GridUI *theGrid;
	ePlacesToSearch whereToSearch;

	if (pAffected == AffectedCharacter::AC_MYSELF) {
		tlTheTrade = &tlMyTrade;
		theGrid		= &tradeMyGrid;
		whereToSearch = PL_SEARCHMYTRADE;
	} else if (pAffected == AffectedCharacter::AC_OTHER) {
		tlTheTrade = &tlOtherTrade;
		theGrid = &tradeOtherGrid;
		whereToSearch = PL_SEARCHOTHERTRADE;
	} else return;

	BOOL boDraggingItemComesFromTrade = FALSE;
	GameUI *draggingItem = GetDragItem(); // If there is an item being dragged, save it for comparition

	tlTheTrade->Lock( "TradeUI::UpdateTrade" );
	newTrade->Lock( "TradeUI::UpdateTrade" );

	// Removes the deleted items from the container list and update qty and charges for the others
	tlTheTrade->ToHead();
	while ( tlTheTrade->QueryNext() ) {
		BOOL boCureentTradeItemIsStillOnTheList = FALSE;
		BAG_ITEM *bagItem = tlTheTrade->GetObject();

		newTrade->ToHead();
		while ( newTrade->QueryNext() ) {
			BAG_ITEM *newBagItem = newTrade->GetObject();
        
			// If the item doesn't have an ID yet and an item of the same
			// type is found.
			if( bagItem->dwID == 0 && 
				bagItem->associatedUI != NULL && 
				bagItem->wBaseID == newBagItem->wBaseID ){
            
				strcpy( 
					reinterpret_cast< char * >( bagItem->chName ),
					reinterpret_cast< char * >( newBagItem->chName )
					);
            
				// Use this new item's ID as the item's ID.
				bagItem->dwID = newBagItem->dwID;
			}
        
			if( newBagItem->dwID == bagItem->dwID ){
				// Update the bag item's item count
				bagItem->dwQty = newBagItem->dwQty;
				bagItem->charges = newBagItem->charges;
            
				// Update the associatedUI's name.
				if( bagItem->associatedUI != NULL ){
					static_cast< InventoryUI::ItemUI * >( 
						bagItem->associatedUI
						)->UpdateName();
				}
            
				boCureentTradeItemIsStillOnTheList = TRUE;
				break;
			}
		}

		if (boCureentTradeItemIsStillOnTheList == FALSE) { // If the object is not on the pack anymore, purge it!
			theGrid->RemoveItem( bagItem->associatedUI ); // Remove its ItemUI from the grid.
			delete bagItem->associatedUI;					// Delete the ItemUI
			tlTheTrade->DeleteObject();							// Delete the bag_info data
		} else { // If the object is still on the list, keep it there and remove it from the newList. In the end, new list will have only new added items
			newTrade->DeleteObject();
			if ( draggingItem != NULL && draggingItem == bagItem->associatedUI ) { // If this item is being dragged, allow client to keep dragging
				boDraggingItemComesFromTrade = TRUE;
			}
		}
	}

	// newTrade list now contains only the items that werent on the list before
	// So, now adds those new items to the trade list
	newTrade->ToHead();
	while ( newTrade->QueryNext() ) {
		BAG_ITEM *newBagItem = newTrade->GetObject();

		tlTheTrade->AddToTail( newBagItem );

        // If a sprite was specified.
        if( newBagItem->lpSprite != NULL ){
            // Create a new GameUI.
            ItemUI *newItemUI = new ItemUI;

            // Associate both bag item and item UI.
            newItemUI->bagItem = newBagItem;
            newBagItem->associatedUI = newItemUI;
            
            newItemUI->Setup();

            // Add the UI to the inventory grid.
            theGrid->AddItem( newItemUI );

			// If the item does not have a name, request it! :)
			if (!newBagItem->chName[0]) {
				TFCPacket sending;
				sending << (RQ_SIZE)RQ_QueryItemName;
				sending << (char)whereToSearch;
				sending << (long)newBagItem->dwID;
				SEND_PACKET( sending );
			}
        }
	}

    // If there is currently an item being dragged.
    if( draggingItem != NULL ){
		// Receiving an update of my own tradepack and im dragging an item from it
		if (GetDragSourceEvent() == &tradeGridEvent && pAffected == AC_MYSELF) {

			// If the drag item could not be found in the trade list nor on the inventory.
			if( boDraggingItemComesFromTrade == FALSE ){
				// Stop dragging.
				delete GetDragItem();

				// Reset mouse cursor.
				CMouseCursor::GetInstance()->SetCustomCursor( NULL );

				// Cancel drag operation.
				CancelDrag();
			}
		}
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::UpdateItemsName
//////////////////////////////////////////////////////////////////////////////////////////
//  Setups the given item UI according to its binded BAG_ITEM data.
//
(
 DWORD dwItemID,
 LPBYTE lpbItemName,
 ePlacesToSearch pWhereToLook
)
//////////////////////////////////////////////////////////////////////////////////////////
{
	TemplateList<BAG_ITEM> *theTrade;
	if (pWhereToLook == PL_SEARCHMYTRADE) {
		theTrade = &tlMyTrade;
	} else if (pWhereToLook == PL_SEARCHOTHERTRADE) {
		theTrade = &tlOtherTrade;
	} else return;

	theTrade->Lock("TradeUI::UpdateItemsName");
	theTrade->ToHead();
	while (theTrade->QueryNext()) {
		LPBAG_ITEM lpBagItem = theTrade->GetObject();
    
		// Update each item with the same base ID.
		if( lpBagItem->dwID == dwItemID ){
			strcpy((char *)lpBagItem->chName, (char *)lpbItemName);
        
			// If there is an associated UI.
			if( lpBagItem->associatedUI != NULL ){
				// Set its help text according to the UI's text.
				static_cast< TradeUI::ItemUI * >( lpBagItem->associatedUI )->
					UpdateName();
			}
			break;
		}
	}
	theTrade->Unlock("TradeUI::UpdateItemsName");
}


//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::TradeGridEvent::Dragged( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Dragged something from the trade
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
	ItemUI *item;
    if( GetDragItem() == NULL ){
        return;
    }else{
        item = static_cast< ItemUI * >( GetDragItem() );
    }

    ItemDragSounds( item->bagItem->Type )->Play( TRUE );

}

//////////////////////////////////////////////////////////////////////////////////////////
bool TradeUI::TradeGridEvent::Dropping( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Verify that the dropped item is ok.
// 
// Return: bool, true if dropped item is ok, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////
{
    
    // If the source event is the trade event.
    if( GetUI()->GetDragSourceEvent() == this ){
        return true;
    }else{        
        ItemUI *itemUI = static_cast< ItemUI * >( GetDragItem() );
		TradeUI *tradeUI = TradeUI::GetInstance();

        // Retreive the bag item ID.
        DWORD itemID = itemUI->bagItem->dwID;
        
        dropSound->Play( TRUE );

		tradeUI->qtySelectPopup.Show();
		tradeUI->qtySelectPopup.SetState( QtySelectPopupUI::ST_FROMBACKPACKTOTRADE, itemUI->bagItem->dwQty, itemUI );
		tradeUI->qtySelectPopup.SetDelay(500);
		if( tradeUI->qtySelectPopup.IsShown() ){
			tradeUI->RequestForegroundControl( &tradeUI->qtySelectPopup );
		}

		DropBack( GetDragItem() );
    }


    // Never let an item be dropped in the trade box, its handling will
    // be done by the packets.
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::TradeGridEvent::Dropped ( EventVisitor *sourceEvent )
//////////////////////////////////////////////////////////////////////////////////////////
// Called when an item was successfully dropped from the trade
//////////////////////////////////////////////////////////////////////////////////////////
{
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::ReadyButtonEvent::LeftClicked() 
//////////////////////////////////////////////////////////////////////////////////////////
// Ready button clicked!
//////////////////////////////////////////////////////////////////////////////////////////
{
	TradeUI::CharacterStatus myStatus;
	myStatus = TradeUI::CharacterStatus(TradeUI::GetInstance()->mMyStatus);
	if (myStatus == CS_READY || myStatus == CS_CONFIRMED) {
		TradeUI::RequestTradeSetStatus(CS_EDITINGITEMS);
	} else {
		TradeUI::RequestTradeSetStatus(CS_READY);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::ConfirmButtonEvent::LeftClicked() 
//////////////////////////////////////////////////////////////////////////////////////////
// Confirm button clicked!
//////////////////////////////////////////////////////////////////////////////////////////
{
	TradeUI::CharacterStatus myStatus;
	myStatus = TradeUI::CharacterStatus(TradeUI::GetInstance()->mMyStatus);
	if (myStatus == CS_READY) {
		TradeUI::RequestTradeSetStatus(CS_CONFIRMED);
	} else if (myStatus == CS_CONFIRMED) {
		TradeUI::RequestTradeSetStatus(CS_READY);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::ClearButtonEvent::LeftClicked() 
//////////////////////////////////////////////////////////////////////////////////////////
// Clear button clicked
//////////////////////////////////////////////////////////////////////////////////////////
{
	TradeUI::RequestTradeClear();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::InviteButtonEvent::LeftClicked() 
//////////////////////////////////////////////////////////////////////////////////////////
// Invite buttom clicked
//////////////////////////////////////////////////////////////////////////////////////////
{
    TradeUI *tradeUI = static_cast< TradeUI * >( GetUI() );

    MouseDevice md;
    if( md.LockNextEvent( DM_CLICK, tradeUI, InvitePlayer, 0 ) ){
        tradeUI->Hide();
    }else{
        // Could not lock mouse.
        ChatterUI::GetInstance()->AddBackscrollItem( 
            "", 
            g_LocalString[ 444 ], SYSTEM_COLOR, true
        );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::CancelButtonEvent::LeftClicked() 
//////////////////////////////////////////////////////////////////////////////////////////
// Cancel buttom clicked
//////////////////////////////////////////////////////////////////////////////////////////
{
	TradeUI *tradeUI = static_cast< TradeUI * >( GetUI() );
	TradeUI::RequestTradeCancel();
	tradeUI->UpdateInterfaceStatus();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::RequestMoveItemFromTradeToBackpack( DWORD dwItemID, DWORD dwQty )
//////////////////////////////////////////////////////////////////////////////////////////
// Send a packet to the server asking to move the item from trade to backpack
//////////////////////////////////////////////////////////////////////////////////////////
{
	TFCPacket packet;
	packet << (RQ_SIZE)RQ_TradeRemoveItemToBackpack;
	packet << (long)dwItemID;
	packet << (long)dwQty;
	SEND_PACKET( packet );
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::RequestMoveItemFromBackpackToTrade( DWORD dwItemID, DWORD dwQty )
//////////////////////////////////////////////////////////////////////////////////////////
// Send a packet to the server asking to move the item from backpack to trade
//////////////////////////////////////////////////////////////////////////////////////////
{
	TFCPacket packet;
	packet << (RQ_SIZE)RQ_TradeAddItemFromBackpack;
	packet << (long)dwItemID;
	packet << (long)dwQty;
	SEND_PACKET( packet );
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::RequestTradeSetStatus( TradeUI::CharacterStatus Status )
//////////////////////////////////////////////////////////////////////////////////////////
// Send a packet to the server asking to change status
//////////////////////////////////////////////////////////////////////////////////////////
{
	TFCPacket packet;
	packet << (RQ_SIZE)RQ_TradeSetStatus;
	packet << (short)Status;
	SEND_PACKET( packet );
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::RequestTradeCancel()
//////////////////////////////////////////////////////////////////////////////////////////
// Send a packet to the server asking to cancel the trade
//////////////////////////////////////////////////////////////////////////////////////////
{
	TFCPacket packet;
	packet << (RQ_SIZE)RQ_TradeCancel;
	SEND_PACKET( packet );
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::RequestTradeInvite( DWORD dwID, short xPos, short yPos )
//////////////////////////////////////////////////////////////////////////////////////////
// Send a packet to the server asking to trade with someone
//////////////////////////////////////////////////////////////////////////////////////////
{
	TFCPacket packet;
	packet << (RQ_SIZE)RQ_TradeInvite;
	packet << (long)dwID;
	packet << (short)xPos;
	packet << (short)yPos;
	SEND_PACKET( packet );
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::RequestTradeClear()
//////////////////////////////////////////////////////////////////////////////////////////
// Send a packet to the server asking to clear all items from the tradepack
//////////////////////////////////////////////////////////////////////////////////////////
{
	TFCPacket packet;
	packet << (RQ_SIZE)RQ_TradeClear;
	SEND_PACKET( packet );
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int TradeUI::InvitePlayer
//////////////////////////////////////////////////////////////////////////////////////////
// When the player clicks the invite button, the trade interface closes and the player
// can click on some other player to ask him to trade.
// When he clicks, this function is called to make the invite to the clicked player
(
 void *data // The gameUI.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock lock( RootBoxUI::GetInstance()->GetLock() );
    
    TradeUI *tradeUI = reinterpret_cast< TradeUI * >( data );
    
    POINT pntMousePos;
    POINT pntWorldPos;

    // Get the mouse pos.
    MouseDevice md;
    md.GetPosition( (int *)&pntMousePos.x, (int *)&pntMousePos.y );

    Player.ScreenPosToWL( &pntMousePos, &pntWorldPos );
    
    int nX = ( pntMousePos.x + 48 ) / 32;
    int nY = ( pntMousePos.y - 8 ) / 16;

    DWORD dwID = GridID( nX, nY );
    // If found an ID.
    if( dwID != 0 ){

        signed char xPos, yPos;
        Objects.Lock(101);
		Objects.Found( &xPos, &yPos, dwID );
		Objects.Unlock(101);

        RequestTradeInvite(dwID, xPos + Player.xPos, yPos + Player.yPos);
    }

    tradeUI->Show();

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::UpdateInterfaceStatus()
//////////////////////////////////////////////////////////////////////////////////////////
// Update the interface buttons/grids according to the current status of the trade
//////////////////////////////////////////////////////////////////////////////////////////
{
	if (trading == TRUE) {
		if (mMyStatus == CS_EDITINGITEMS) {
			mReadyButtonON.Hide();
			mConfirmButtonON.Hide();
			mConfirmButtonOFF.Hide();
			mReadyButtonOFF.Show();
			disabledConfirmButton.Show();
			//PutChildInBackground(&mReadyButtonOFF);
			//PutChildInBackground(&disabledConfirmButton);
		} else if (mMyStatus == CS_READY) {
			mReadyButtonOFF.Hide();
			mReadyButtonON.Show();
			//PutChildInBackground(&mReadyButtonON);
			if (mOtherStatus == CS_READY || mOtherStatus == CS_CONFIRMED) {
				mConfirmButtonON.Hide();
				disabledConfirmButton.Hide();
				mConfirmButtonOFF.Show();
				//PutChildInBackground(&mConfirmButtonOFF);
			} else {
				mConfirmButtonON.Hide();
				mConfirmButtonOFF.Hide();
				disabledConfirmButton.Show();
				//PutChildInBackground(&disabledConfirmButton);
			}
		} else if (mMyStatus == CS_CONFIRMED) {
			mReadyButtonOFF.Hide();
			mConfirmButtonOFF.Hide();
			disabledConfirmButton.Hide();
			mReadyButtonON.Show();
			mConfirmButtonON.Show();
			//PutChildInBackground(&mReadyButtonON);
			//PutChildInBackground(&mConfirmButtonON);
		}

		if (mOtherStatus == CS_EDITINGITEMS) {
			mOtherReadyButton.SetSpriteId("ready_button_OFF");
			mOtherConfirmButton.SetSpriteId("ready_button_OFF");
		} else if (mOtherStatus == CS_READY) {
			mOtherReadyButton.SetSpriteId("ready_button_ON");
			mOtherConfirmButton.SetSpriteId("ready_button_OFF");
		} else if (mOtherStatus == CS_CONFIRMED) {
			mOtherReadyButton.SetSpriteId("ready_button_ON");
			mOtherConfirmButton.SetSpriteId("ready_button_ON");
		}

/*		disabledClearButton.Hide();
		disabledReadyButton.Hide();
		mDisabledOtherReadyButton.Hide();
		mDisabledOtherConfirmButton.Hide();*/
		inviteButton.Hide();

		tradeMyGrid.Show();
		tradeOtherGrid.Show();
		cancelButton.Show();
		clearButton.Show();

		//PutChildInBackground(&cancelButton);
		//PutChildInBackground(&clearButton);

		mOtherReadyButton.Hide(); // Hide first so it updates the sprite
		mOtherConfirmButton.Hide(); // Hide first so it updates the sprite
		mOtherReadyButton.Show();
		mOtherConfirmButton.Show();
	} else {
		tradeMyGrid.Hide();
		tradeOtherGrid.Hide();
		clearButton.Hide();
		mConfirmButtonON.Hide();
		mConfirmButtonOFF.Hide();
		mReadyButtonON.Hide();
		mReadyButtonOFF.Hide();
		cancelButton.Hide();
		mOtherReadyButton.Hide();
		mOtherConfirmButton.Hide();

		disabledConfirmButton.Hide();
		inviteButton.Show();

      sprintf(m_strMyInfo   ,"");
      sprintf(m_strOtherInfo,"");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::ResetTrade()
//////////////////////////////////////////////////////////////////////////////////////////
// Reset the trade status variables and update the interface
//////////////////////////////////////////////////////////////////////////////////////////
{
	trading = false;
	mMyStatus = mOtherStatus = CS_EDITINGITEMS;
	UpdateInterfaceStatus();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::EventPlayerInvited(char *pCharacterName, DWORD pInvitorID, short pX, short pY)
//////////////////////////////////////////////////////////////////////////////////////////
// Someone is inviting me! Show the invite popup so the user can select to accept or not
//////////////////////////////////////////////////////////////////////////////////////////
{
	if (pCharacterName == NULL) return;
	if ( IsForegroundControlled() ) {
		joinPopup.Decline();
	} else {
        if( ChatterUI::GetInstance()->IsUserIgnored( pCharacterName) ){
            // Decline.
            joinPopup.Decline();
        }else{
            char buf[ 1024 ];
            sprintf( buf, g_LocalString[ 573 ], pCharacterName );

			joinPopup.SetText(buf);
			joinPopup.SetInvitorInfo(pInvitorID, pX, pY);
			joinPopup.Show();
			RequestForegroundControl( &joinPopup );
        }
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::EventTradeStarted(char *pCharacterName)
//////////////////////////////////////////////////////////////////////////////////////////
// The trade started! Setup interface and status :)
//////////////////////////////////////////////////////////////////////////////////////////
{
	if (pCharacterName == NULL)
   {
      sprintf(m_strMyInfo   ,"");
      sprintf(m_strOtherInfo,"");
      return;
   }
   sprintf(m_strMyInfo   ,"My TradePack");
   sprintf(m_strOtherInfo,"%s",pCharacterName);
	mMyStatus = mOtherStatus = CS_EDITINGITEMS;
	trading = true;
	Show();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::EventTradeCanceled() {
//////////////////////////////////////////////////////////////////////////////////////////
// Trade got canceled. Reset interface and status
//////////////////////////////////////////////////////////////////////////////////////////
	ResetTrade();
	Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::EventTradeFinished()
//////////////////////////////////////////////////////////////////////////////////////////
// Trade finished. Reset interface and status
//////////////////////////////////////////////////////////////////////////////////////////
{
	ResetTrade();
	Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::EventTradeStatusChanged(CharacterStatus pMyStatus, CharacterStatus pOtherStatus)
//////////////////////////////////////////////////////////////////////////////////////////
// Status changes. Setup interface and status acordingly
//////////////////////////////////////////////////////////////////////////////////////////
{
	// new status MUST be in a valid range!
	if (pMyStatus <= pOtherStatus && pOtherStatus < CS_EDITINGITEMS && pMyStatus >= pOtherStatus && pOtherStatus > CS_CONFIRMED) {
		return;
	}
	mMyStatus = pMyStatus;
	mOtherStatus = pOtherStatus;
	UpdateInterfaceStatus();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::EventDroppingItemUIFromTradeToBackpack()
//////////////////////////////////////////////////////////////////////////////////////////
// Item dropped on backpack from trade
//////////////////////////////////////////////////////////////////////////////////////////
{
	GameUI *dragItem = GetDragItem();
	ItemUI *iuiItem = (ItemUI*)dragItem;
	TradeUI *tradeUI = TradeUI::GetInstance();

	tradeUI->qtySelectPopup.Show();
	tradeUI->qtySelectPopup.SetState( TradeUI::QtySelectPopupUI::ST_FROMTRADETOBACKPACK, iuiItem->bagItem->dwQty, iuiItem );
	if( tradeUI->qtySelectPopup.IsShown() ){
		tradeUI->RequestForegroundControl( &tradeUI->qtySelectPopup );
	}

	DropBack(dragItem);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Join popup.

// Constructor ///////////////////////////////////////////////////////////////////////////
TradeUI::JoinPopupUI::JoinPopupUI( void )
:
    acceptButton( acceptButtonEvent ),
    declineButton( declineButtonEvent )

//////////////////////////////////////////////////////////////////////////////////////////
{
    //Back du popup box...
    int dwOffX = (g_Global.GetScreenW()-240)/2;
    int dwOffY = (g_Global.GetScreenH()-164)/2;
    SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );

    JoinBack.SetSpriteId( "GUI_PopupBack" );
    JoinBack.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );
    JoinBack.DrawAlpha(true);
    // patch pour la zone de texte
    graphPatch.SetSpriteId( "GUI_TextPopupBack" );
    graphPatch.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+85 ) );
    

    acceptButton.SetSpriteId   ( "GUI_ButtonUp" );
    acceptButton.SetDownUI     ( "GUI_ButtonDown" );
    acceptButton.SetHighlightUI( "GUI_ButtonHUp" );
    acceptButton.SetPos( FramePos( dwOffX+52, dwOffY+94, dwOffX+52+60, dwOffY+94+32 ) );
    acceptButtonEvent.SetUI( this );
    SetHelpId( acceptButton, 456 );

    declineButton.SetSpriteId   ( "GUI_ButtonUp" );
    declineButton.SetDownUI     ( "GUI_ButtonDown" );
    declineButton.SetHighlightUI( "GUI_ButtonHUp" );
    declineButton.SetPos( FramePos( dwOffX+128, dwOffY+94, dwOffX+128+60, dwOffY+94+32 ) );
    declineButtonEvent.SetUI( this );
    SetHelpId( declineButton, 457 );

    textArea.SetPos( FramePos( dwOffX+22, dwOffY+23, dwOffX+217, dwOffY+78 ) );
    textArea.SetCenter( true );

    AddChild( &JoinBack );
    AddChild( &graphPatch );
    AddChild( &acceptButton );
    AddChild( &declineButton );
    AddChild( &textArea );

    RootBoxUI::GetInstance()->AddChild( this );
}

void TradeUI::JoinPopupUI::Draw(V2SPRITEFX *vsfFX )
{
    int dwXPos = (g_Global.GetScreenW()-240)/2;
    int dwYPos = (g_Global.GetScreenH()-164)/2;
    
    GameUI::Draw( vsfFX );
    
    HDC hdc;
    DXDGetDC(&hdc, 669);
    char strBtnTexte[20];
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[10]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 52, dwYPos+96 , RGB(0, 0, 0)    , 0, strBtnTexte);
    
    sprintf(strBtnTexte  ,"%s",g_GUILocalString[11]);
    fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 128, dwYPos+96 , RGB(0, 0, 0)    , 0, strBtnTexte);
    DXDReleaseDC(hdc, 669);
}

int TradeUI::JoinPopupUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::JoinPopupUI::Decline( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Refuses an invitation.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
}

//////////////////////////////////////////////////////////////////////////////////////////
bool TradeUI::JoinPopupUI::VKeyInput
//////////////////////////////////////////////////////////////////////////////////////////
//  Intercepts ESC so it decline the invitation before closing the box.
// 
(
 VKey vKey
)
// Return: bool, true
//////////////////////////////////////////////////////////////////////////////////////////
{
	if (vKey == VK_RETURN) {
		acceptButtonEvent.LeftClicked();
	} else if (vKey == VK_ESCAPE) {
		declineButtonEvent.LeftClicked();
	}
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::JoinPopupUI::AcceptButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on the accept.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    JoinPopupUI *popupUI = static_cast< TradeUI::JoinPopupUI * >( GetUI() );

	TradeUI::RequestTradeInvite(popupUI->mInvitorID, popupUI->mInvitorXPos, popupUI->mInvitorYPos);
    popupUI->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::JoinPopupUI::DeclineButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on decline.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    JoinPopupUI *popupUI = static_cast< TradeUI::JoinPopupUI * >( GetUI() );

    popupUI->Decline();
    popupUI->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::JoinPopupUI::SetText
//////////////////////////////////////////////////////////////////////////////////////////
//  Set the text area's text.
// 
(
 string text // the text.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    textArea.SetText( text, RGB( 255, 255, 255 ), 152 );
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::JoinPopupUI::SetInvitorInfo(DWORD pCharID, WORD pXPos, WORD pYPos)
{
	mInvitorID = pCharID;
	mInvitorXPos = pXPos;
	mInvitorYPos = pYPos;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Quantity select popup

//////////////////////////////////////////////////////////////////////////////////////////
TradeUI::QtySelectPopupUI::QtySelectPopupUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
: okButton( okButtonEvent ),
  cancelButton( cancelButtonEvent ),
  amountSlider( sliderUpdateQtyEvent ),
  dwDelayUntill( GetTickCount() )
{
   
   int dwOffX = (g_Global.GetScreenW()-240)/2;
   int dwOffY = (g_Global.GetScreenH()-164-150)/2;
   
   SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );
   
   backPopup.SetSpriteId( "GUI_PopupBack" );
   backPopup.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );
   graphDJ.SetSpriteId( "GUI_InvPopupBack" );
   graphDJ.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+85 ) );
   
   okButton.SetSpriteId   ( "GUI_ButtonUp" );
   okButton.SetDownUI     ( "GUI_ButtonDown" );
   okButton.SetHighlightUI( "GUI_ButtonHUp" );
   okButton.SetPos( FramePos( dwOffX+52, dwOffY+94, dwOffX+52+60, dwOffY+94+32 ) );
   okButtonEvent.SetUI( this );
   SetHelpId( okButton, 452 );
   
   cancelButton.SetSpriteId   ( "GUI_ButtonUp" );
   cancelButton.SetDownUI     ( "GUI_ButtonDown" );
   cancelButton.SetHighlightUI( "GUI_ButtonHUp" );
   cancelButton.SetPos( FramePos( dwOffX+128, dwOffY+94, dwOffX+128+60, dwOffY+94+32 ) );
   cancelButtonEvent.SetUI( this );
   SetHelpId( cancelButton, 453 );
   
   
   amountSlider.SetRange( 0, 0, FramePos( dwOffX+48, dwOffY+61, dwOffX+113, dwOffY+72 ) );
   amountSlider.SetPos( FramePos( dwOffX+29, dwOffY+57, dwOffX+132, dwOffY+77 ) );
   amountSlider.SetScrollRegions(
      FramePos( dwOffX+29, dwOffY+57, dwOffX+48, dwOffY+77 ),
      FramePos( dwOffX+112, dwOffY+57, dwOffX+132, dwOffY+77 ),
      "",""); 
   
   textAreaDJ .SetText( g_GUILocalString[38], RGB( 223, 157, 0 ), 190 );
   textAreaDJ .SetPos( FramePos( dwOffX+22, dwOffY+23, dwOffX+217, dwOffY+47 ) );
   textAreaDJ .SetCenter( true );
   
   sliderUpdateQtyEvent.SetUI( this );
   
   amountEdit.SetPos( FramePos( dwOffX+145, dwOffY+57, dwOffX+219, dwOffY+80 ) );
   amountEdit.SetFilter( &amountEditFilter );
   amountEdit.SetMaxTextLength( 9 );
   amountEdit.EnableCursor( true );
   SetHelpId( amountEdit, 451 );
   
   
   AddChild( &backPopup );
   AddChild( &graphDJ );
   AddChild( &okButton );
   AddChild( &cancelButton );
   AddChild( &amountSlider );
   AddChild( &amountEdit );
   AddChild( &textAreaDJ);
}

void TradeUI::QtySelectPopupUI::Draw( V2SPRITEFX *vsfFX )
{
   int dwOffX = (g_Global.GetScreenW()-240)/2;
   int dwOffY = (g_Global.GetScreenH()-164-150)/2;

   backPopup.Draw(vsfFX);
   GameUI::Draw( vsfFX );
    
   HDC hdc;
   DXDGetDC(&hdc, 671);

   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[10]);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 52+1, dwOffY+96+2 , RGB(0, 0, 0)    , 0, strBtnTexte);
   
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[11]);
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 128+1, dwOffY+96+2 , RGB(0, 0, 0)    , 0, strBtnTexte);
   
   DXDReleaseDC(hdc, 671);
}

int TradeUI::QtySelectPopupUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
bool TradeUI::QtySelectPopupUI::VKeyInput(VKey ivkey) {
//////////////////////////////////////////////////////////////////////////////////////////
	if (ivkey == VK_RETURN) {
		okButtonEvent.LeftClicked();
	} else if (ivkey == VK_ESCAPE) {
		cancelButtonEvent.LeftClicked();
	} else if (ivkey == VK_LEFT) {
		MousePos X(257,175);
		amountSlider.LeftMouseDown(X);
		amountSlider.LeftMouseUp(X);
	} else if (ivkey == VK_RIGHT) {
		MousePos X(369,175);
		amountSlider.LeftMouseDown(X);
		amountSlider.LeftMouseUp(X);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::QtySelectPopupUI::SetState
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the junk drop popup's current state.
// 
(
 State s,       // New state
 DWORD maxQty,   // Maximum amount quantity.
 ItemUI *i
)
//////////////////////////////////////////////////////////////////////////////////////////
{
   state = s;
   itemUI = i;

   int dwOffX = (g_Global.GetScreenW()-240)/2;
   int dwOffY = (g_Global.GetScreenH()-164-150)/2;
   
   amountSlider.SetRange( 0, maxQty, FramePos( dwOffX+48, dwOffY+61, dwOffX+113, dwOffY+72 ) );
   amountSlider.SetSliderPos( 0 );
   amountSlider.Show();
   amountEdit.Show();
   graphDJ.Show();
   
   // Automatically click OK of the quantity is 1.
    if( maxQty == 1 ){
        
        amountEdit.SetText( "1" );
        okButtonEvent.LeftClicked();

        Hide();
        return;
    }
    
    amountEdit.SetText( "0" );
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::QtySelectPopupUI::SetDelay
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the junk drop popup's current state.
// 
(
 DWORD dwDelay_ms       // delay in milliseconds till it accepts clicks
)
//////////////////////////////////////////////////////////////////////////////////////////
{
	dwDelayUntill = GetTickCount() + dwDelay_ms;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::QtySelectPopupUI::OkButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  OK
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    QtySelectPopupUI *popupUI = static_cast< TradeUI::QtySelectPopupUI * >( GetUI() );

	// If its still in the blocking (delay) time, ignore the click
	if (GetTickCount() < popupUI->dwDelayUntill) return;


    DWORD qty = atoi( popupUI->amountEdit.GetText().c_str() );

    // If the player put too much items.
    if( qty > popupUI->itemUI->bagItem->dwQty ){
        qty = popupUI->itemUI->bagItem->dwQty;
    }
    // If no item to move, juz leave :D
    if( qty == 0 ){
        popupUI->Hide();
        return;
    }

    switch( popupUI->state ){
    case ST_FROMBACKPACKTOTRADE:{
		TradeUI::GetInstance()->RequestMoveItemFromBackpackToTrade(popupUI->itemUI->bagItem->dwID, qty);
    }break;
    case ST_FROMTRADETOBACKPACK:{
		TradeUI::GetInstance()->RequestMoveItemFromTradeToBackpack(popupUI->itemUI->bagItem->dwID, qty);
    }break;
    }

    popupUI->Hide();

    
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::QtySelectPopupUI::CancelButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  CANCEL
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    QtySelectPopupUI *popupUI = static_cast< TradeUI::QtySelectPopupUI * >( GetUI() );

	// If its still in the blocking (delay) time, ignore the click
	if (GetTickCount() < popupUI->dwDelayUntill) return;
    
    // Quit
    popupUI->Hide();    
}

//////////////////////////////////////////////////////////////////////////////////////////
void TradeUI::QtySelectPopupUI::SliderUpdateQtyEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Clicked on the slider, update the displayed amount.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    QtySelectPopupUI *popupUI = static_cast< TradeUI::QtySelectPopupUI * >( GetUI() );

    LOG.Lock();
    LOG << "\r\nNew amount slider pos=" << popupUI->amountSlider.GetSliderPos() << ".!";
    LOG.Unlock();
    
    
    char buf[ 256 ];

    // Update the edit box to the current slider's position.
    popupUI->amountEdit.SetText(
        itoa( popupUI->amountSlider.GetSliderPos(), buf, 10 )
    );

}

//////////////////////////////////////////////////////////////////////////////////////////
bool TradeUI::QtySelectPopupUI::AmountEditFilter::IsOK
//////////////////////////////////////////////////////////////////////////////////////////
//  Filters the text input box and only lets numbers pass through.
// 
(
 char nextCh, // The next char
 const std::string curText // The current text.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    return isdigit( nextCh ) != 0 ? true : false;
}
