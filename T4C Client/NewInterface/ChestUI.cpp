// *******************************************************************************************
// ***                                                                                     ***
//      File Name: ChestUI.cpp
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
//          In Game Chest Interface.
//          
// ***                                                                                     ***
//	*******************************************************************************************
// ***           Copyright (c) 2004 Dialsoft           . All rights reserved.              ***

// Includes

#include "../pch.h"
#include "ChestUI.h"
#include "RootBoxUI.h"
#include "GraphUI.h"
#include "../MemoryManager.h"
#include "ChatterUI.h"
#include "..\SaveGame.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]

extern Font *fNewGuiBtnFont;
extern CSaveGame g_SaveGame;

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

ChestUI::ChestUI( void ) : 
   junkButton( &(InventoryUI::GetInstance()->junkButton) ),
   dropButton( &(InventoryUI::GetInstance()->dropButton) ),
   inventoryGrid( &(InventoryUI::GetInstance()->inventoryGrid) ),
   chestGrid( chestGridEvent ),
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

   backChestT.SetSpriteId( "GUI_BackChest" );
   backChestT.SetPos( FramePos( dwXPos, dwYPos, dwXPos+550, dwYPos+260 ) );
   backChestT.DrawAlpha(true);
    
   backChestB.SetSpriteId( "GUI_InvBackB" );
   backChestB.SetPos( FramePos( dwXPos, dwYPos+260, dwXPos+550, dwYPos+430 ) );
   backChestB.DrawAlpha(true);

   X_Button.SetPos( FramePos( dwXPos+550-28, dwYPos+0, dwXPos+550-4, dwYPos+0+24 ) );
   X_Button.SetSpriteId( "GUI_X_ButtonUp" );
   X_Button.SetDownUI( "GUI_X_ButtonDown" );
   X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
   SetHelpId( X_Button, 261 );
	



	chestGrid.SetPos( FramePos( dwXPos+88, dwYPos+53, dwXPos+389, dwYPos+233 ) );
	chestGrid.SetScrollRegions(
		FramePos( dwXPos+364, dwYPos+58, dwXPos+387, dwYPos+83 ),
		FramePos( dwXPos+364, dwYPos+205, dwXPos+387, dwYPos+231 ),
		FramePos( dwXPos+364, dwYPos+85, dwXPos+387, dwYPos+195 ),
		NULL
	);
	chestGrid.SetGrid( 9, 6, FramePos( 0, 0, 26, 26 ), FramePos( 0, 0, 4, 4 ) );
   chestGrid.AllowHelp(false);
	SetHelpId(chestGrid, 559);

	chestGridScrollUpGraph  .SetPos( FramePos(dwXPos+364, dwYPos+58, dwXPos+387, dwYPos+83) );
	chestGridScrollDownGraph.SetPos( FramePos(dwXPos+364, dwYPos+205, dwXPos+387, dwYPos+231) );
	chestGrid.SetScrollUpDownUI( &chestGridScrollUpGraph );
	chestGrid.SetScrollDownDownUI( &chestGridScrollDownGraph );

	gridSelectedTile.SetSpriteId( "GUI_Chest_HUp" );
	chestGrid.SetSelectedTileUI( &gridSelectedTile );

   AddChild( &backChestT);
   AddChild( &backChestB);
   AddChild( &X_Button);
	AddChild( inventoryGrid );
	AddChild( &chestGrid );
	AddChild( dropButton );
	AddChild( junkButton );
	AddChild( iconGold );
	AddChild( statGold );

   
   LoadChest();


	// Add the inventory to the root box.
	RootBoxUI::GetInstance()->AddChild( this ); 

	// Running as minimized window allow us to draw game background normally.
	//Minimize(true);
}

ChestUI::~ChestUI( void )
{
}

void ChestUI::ClientShutdown( void )
{
    SaveChest();
}


ChestUI *ChestUI::GetInstance( void )
{
	static ChestUI m_pInstance;
	return &m_pInstance;
}

unsigned int ChestUI::Display( void *null )
{
	CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );

	if( GetInstance()->IsShown() ){    
		GetInstance()->Hide();
	}else{
		 TFCPacket sending;
         sending << (RQ_SIZE)RQ_ViewBackpack;
         sending << (short)0;
         SEND_PACKET( sending );

		// Show box.
		GetInstance()->Show();
	}

	return 0;
}



//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the Chest UI
// 
//////////////////////////////////////////////////////////////////////////////////////////
void ChestUI::Draw(V2SPRITEFX *vsfFX)
{
   int dwXPos = (g_Global.GetScreenW()-550)/2;
   int dwYPos = (g_Global.GetScreenH()-150-430)/2;
   if(dwYPos <0)
       dwYPos =0;
    
   GameUI::Draw( vsfFX );
   
   
   HDC hdc;
   DXDGetDC(&hdc, 690);

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

   // Draw Box Header
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[96]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,99)+ 194+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,99)+ 194  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);


   DXDReleaseDC(hdc, 690);


   
   // Get the current mouse coordinates.
   MousePos mousePos( MouseDevice::xPos, MouseDevice::yPos );
   
   ItemUI * itemUI = NULL;
   if( inventoryGrid->GetPos().MousePosInFrame( mousePos ) )
      itemUI = static_cast< ItemUI * >( inventoryGrid->GetGridItemAt( mousePos ) );
   else if( chestGrid.GetPos().MousePosInFrame( mousePos ) ) 
      itemUI = static_cast< ItemUI * >( chestGrid.GetGridItemAt( mousePos ) );
   
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
  			sending << (char)PL_SEARCHCHEST;
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

int ChestUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}


//////////////////////////////////////////////////////////////////////////////////////////
bool ChestUI::VKeyInput
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
void ChestUI::UpdateChest( TemplateList< BAG_ITEM > *newChest )
//////////////////////////////////////////////////////////////////////////////////////////
// Updates the chest
{
   LOG << "\r\nReceived chest update.";
   
   // Lock the root box since this call came from some unknown thread.
   CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );
   
   BOOL boDraggingItemComesFromChest = FALSE;
   GameUI *draggingItem = GetDragItem(); // If there is an item being dragged, save it for comparition
   
   tlChest.Lock( "ChestUI::UpdateChest" );
   newChest->Lock( "ChestUI::UpdateChest" );
   
   // Removes the deleted items from the chest list and update qty and charges for the others
   tlChest.ToHead();
   while ( tlChest.QueryNext() ) 
   {
      BOOL boCureentChestItemIsStillOnTheList = FALSE;
      BAG_ITEM *bagItem = tlChest.GetObject();
      
      newChest->ToHead();
      while ( newChest->QueryNext() ) 
      {
         BAG_ITEM *newBagItem = newChest->GetObject();
         
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
         
         if( newBagItem->dwID == bagItem->dwID )
         {
            // Update the bag item's item count
            bagItem->dwQty = newBagItem->dwQty;
            bagItem->charges = newBagItem->charges;
            
            // Update the associatedUI's name.
            if( bagItem->associatedUI != NULL )
            {
               static_cast< InventoryUI::ItemUI * >( bagItem->associatedUI)->UpdateName();
            }
            
            boCureentChestItemIsStillOnTheList = TRUE;
            break;
         }
      }
      
      if (boCureentChestItemIsStillOnTheList == FALSE) 
      { // If the object is not on the chest anymore, purge it!
         chestGrid.RemoveItem( bagItem->associatedUI ); // Remove its ItemUI from the grid.
         delete bagItem->associatedUI;					// Delete the ItemUI
         tlChest.DeleteObject();							// Delete the bag_info data
      } 
      else 
      { // If the object is still on the list, keep it there and remove it from the newList. In the end, new list will have only new added items
         newChest->DeleteObject();
         // If the item being dragged comes from chest, allow client to keep dragging
         if ( draggingItem != NULL && draggingItem == bagItem->associatedUI ) 
         { 
            boDraggingItemComesFromChest = TRUE;
         }
      }
   }
   

	// newChest list now contains only the items that werent on the list before
	// So, now adds those new items to the chest list
	newChest->ToHead();
   while ( newChest->QueryNext() ) 
   {
      BAG_ITEM *newBagItem = newChest->GetObject();
      // If a sprite was specified.
      if( newBagItem->lpSprite != NULL )
      {
         tlChest.AddToTail( newBagItem );
         // Create a new GameUI.
         ItemUI *newItemUI = new ItemUI;
         
         // Associate both bag item and item UI.
         newItemUI->bagItem = newBagItem;
         newBagItem->associatedUI = newItemUI;
         
         newItemUI->Setup();
         
         // Add the UI to the inventory grid.
         chestGrid.AddItem( newItemUI );
         
         // If the item does not have a name, request it! :)
         if (!newBagItem->chName[0]) 
         {
            TFCPacket sending;
            sending << (RQ_SIZE)RQ_QueryItemName;
  				sending << (char)PL_SEARCHCHEST;
            sending << (long)newBagItem->dwID;
            SEND_PACKET( sending );
         }
      }
   }

    // If there is currently an item being dragged.
   if( draggingItem != NULL ){
      
      if (GetDragSourceEvent() == &chestGridEvent) 
      {
         
         
         // Gets the vector with all inventory items to check if the dragged items is not from the inventory
         std::vector< GameUI * > vInventoryItemsList;
         inventoryGrid->GetGridItems(vInventoryItemsList);
         
         // If the drag item could not be found in the chest list nor on the inventory.
         if( boDraggingItemComesFromChest == FALSE
            && find( vInventoryItemsList.begin(), vInventoryItemsList.end(), GetDragItem() ) == vInventoryItemsList.end()
            ){
            // Stop dragging.
            delete GetDragItem();
            
            // Reset mouse cursor.
            CMouseCursor::GetInstance()->SetCustomCursor( NULL );
            
            // Cancel drag operation.
            CancelDrag();
         }
      }
   }
   
   tlChest.Unlock("ChestUI::UpdateChest");
   newChest->Unlock("ChestUI::UpdateChest");
   SaveChest();
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Saves the chest into the system's registry.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void ChestUI::SaveChest( void )
{
   g_SaveGame.bFreeChest();

   list< GridUI::GridItem > itemList;
   list< GridUI::GridItem >::iterator i;
   chestGrid.GetGridItemList( itemList );
   for( i = itemList.begin(); i != itemList.end(); i++ )
   {
      GridUI::GridItem &gridItem = *i;
      ItemUI *itemUI = static_cast< ItemUI * >( gridItem.ui );

      sInventory sNewChest;
      
      sNewChest.dwID         = itemUI->bagItem->wBaseID;
      sNewChest.dwX          = gridItem.gridPos.x;
      sNewChest.dwY          = gridItem.gridPos.y;
      sNewChest.dwType       = itemUI->bagItem->Type;
      g_SaveGame.AddChest(sNewChest);
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Loads the chest from the system's registry.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void ChestUI::LoadChest( void )
{
   int dwNbrChest = g_SaveGame.GetNbrChest();

   set< DWORD > addedIds;
   if(dwNbrChest > 0)
   {
      for(int i =0;i<dwNbrChest; i++ )
      {
         sInventory sNewChest;
         g_SaveGame.GetChest(i,&sNewChest);

         GridUI::GridPos gridPos;
         gridPos.x = sNewChest.dwX;
         gridPos.y = sNewChest.dwY;


         // Create a new UI.
         ItemUI *itemUI = new ItemUI;
         
         LPBAG_ITEM bagItem = new BAG_ITEM;
         
         bagItem->associatedUI = itemUI;
         bagItem->wBaseID      = (WORD)sNewChest.dwID;
         bagItem->dwID         = 0; // Assign a null ID.
         bagItem->Type         = (WORD)sNewChest.dwType;
         bagItem->lpSprite     = InvItemIcons( sNewChest.dwType );
         bagItem->chName[0] = 0;
         
         itemUI->bagItem = bagItem; // Associate the BAG_ITEM to the UI
         itemUI->Setup(); // Setup the item UI.

         // If the item ID was already added or the grid cannot accept the item.
         if( addedIds.find( bagItem->wBaseID ) != addedIds.end() || !chestGrid.AddGridItemAtPos( gridPos, itemUI ) )
         {
            // If the spot wasn't valid, junk this item UI.
            delete itemUI;
            delete bagItem;
         }
         else
         {
            addedIds.insert( bagItem->wBaseID );
            tlChest.AddToTail( bagItem);
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChestUI::RequestMoveItemFromBackpackToChest
//////////////////////////////////////////////////////////////////////////////////////////
//  Setups the given item UI according to its binded BAG_ITEM data.
//
(
 DWORD dwItemID,
 DWORD dwQty
)
//////////////////////////////////////////////////////////////////////////////////////////
{
	TFCPacket packet;
	packet << (RQ_SIZE)RQ_ChestAddItemFromBackpack;
	packet << (long)dwItemID;
	packet << (long)dwQty;
	SEND_PACKET( packet );
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChestUI::RequestMoveItemFromChestToBackpack
//////////////////////////////////////////////////////////////////////////////////////////
//  Setups the given item UI according to its binded BAG_ITEM data.
//
(
 DWORD dwItemID,
 DWORD dwQty
)
//////////////////////////////////////////////////////////////////////////////////////////
{
	TFCPacket packet;
	packet << (RQ_SIZE)RQ_ChestRemoveItemToBackpack;
	packet << (long)dwItemID;
	packet << (long)dwQty;
	SEND_PACKET( packet );
}

//////////////////////////////////////////////////////////////////////////////////////////
BOOL ChestUI::UpdateItemsName
//////////////////////////////////////////////////////////////////////////////////////////
//  Setups the given item UI according to its binded BAG_ITEM data.
//
(
 DWORD dwItemID,
 LPBYTE lpbItemName
)
//////////////////////////////////////////////////////////////////////////////////////////
{
 	BOOL returnValue = FALSE;
	tlChest.Lock("ChestUI::UpdateItemsName");
	tlChest.ToHead();
	while (tlChest.QueryNext()) 
   {
		LPBAG_ITEM lpBagItem = tlChest.GetObject();
    
		// Update each item with the same base ID.
		if( lpBagItem->dwID == dwItemID )
      {
			strcpy((char *)lpBagItem->chName, (char *)lpbItemName);
        
			// If there is an associated UI.
			if( lpBagItem->associatedUI != NULL )
         {
				// Set its help text according to the UI's text.
				static_cast< ChestUI::ItemUI * >( lpBagItem->associatedUI )->UpdateName();
			}
  			returnValue = TRUE;
			break;
		}
	}
	tlChest.Unlock("ChestUI::UpdateItemsName");
  	return returnValue;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChestUI::ChestGridEvent::Dragged( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Dragged something from the chest
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
bool ChestUI::ChestGridEvent::Dropping( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Verify that the dropped item is ok.
// 
// Return: bool, true if dropped item is ok, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////
{
    
    // If the source event is the chest event.
    if( GetUI()->GetDragSourceEvent() == this ){
        return true;
    }else{        
        ItemUI *itemUI = static_cast< ItemUI * >( GetDragItem() );
		ChestUI *chestUI = ChestUI::GetInstance();

        // Retreive the bag item ID.
        DWORD itemID = itemUI->bagItem->dwID;
        
        dropSound->Play( TRUE );

		chestUI->qtySelectPopup.Show();
		chestUI->qtySelectPopup.SetState( QtySelectPopupUI::FromBackpackToChest, itemUI->bagItem->dwQty, itemUI );
		chestUI->qtySelectPopup.SetDelay(500);
		if( chestUI->qtySelectPopup.IsShown() ){
			chestUI->RequestForegroundControl( &chestUI->qtySelectPopup );
		}

		DropBack( GetDragItem() );
    }


    // Never let an item be dropped in the chest box, its handling will
    // be done by the packets.
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChestUI::ChestGridEvent::Dropped ( EventVisitor *sourceEvent )
//////////////////////////////////////////////////////////////////////////////////////////
// Called when an item was successfully dropped from the chest
//////////////////////////////////////////////////////////////////////////////////////////
{

	return;
    ItemUI *dragUI = static_cast< ItemUI * >( GetDragItem() );
    
    // Must drop an item.
    if( dragUI == NULL ){
        return;
    }

	DropBack( GetDragItem() );
         
	dropSound->Play( TRUE );
}






















/////////////////////////////////////////////////













//////////////////////////////////////////////////////////////////////////////////////////
ChestUI::QtySelectPopupUI::QtySelectPopupUI( void )
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

void ChestUI::QtySelectPopupUI::Draw( V2SPRITEFX *vsfFX )
{
   int dwOffX = (g_Global.GetScreenW()-240)/2;
   int dwOffY = (g_Global.GetScreenH()-164-150)/2;

   backPopup.Draw(vsfFX);
   GameUI::Draw( vsfFX );
    
   HDC hdc;
   DXDGetDC(&hdc, 671);

   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s","Accept");
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 52+1, dwOffY+96+2 , RGB(0, 0, 0)    , 0, strBtnTexte);
   
   sprintf(strBtnTexte  ,"%s","Cancel");
   fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 128+1, dwOffY+96+2 , RGB(0, 0, 0)    , 0, strBtnTexte);
   
   DXDReleaseDC(hdc, 671);
}

int ChestUI::QtySelectPopupUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}


//////////////////////////////////////////////////////////////////////////////////////////
bool ChestUI::QtySelectPopupUI::VKeyInput(VKey ivkey) {
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
void ChestUI::QtySelectPopupUI::SetState
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
void ChestUI::QtySelectPopupUI::SetDelay
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
void ChestUI::QtySelectPopupUI::OkButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  OK
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    QtySelectPopupUI *popupUI = static_cast< ChestUI::QtySelectPopupUI * >( GetUI() );

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
    case FromBackpackToChest:{
		ChestUI::GetInstance()->RequestMoveItemFromBackpackToChest(popupUI->itemUI->bagItem->dwID, qty);
    }break;
    case FromChestToBackpack:{
		ChestUI::GetInstance()->RequestMoveItemFromChestToBackpack(popupUI->itemUI->bagItem->dwID, qty);
    }break;
    }

    popupUI->Hide();

    
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChestUI::QtySelectPopupUI::CancelButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  CANCEL
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    QtySelectPopupUI *popupUI = static_cast< ChestUI::QtySelectPopupUI * >( GetUI() );

	// If its still in the blocking (delay) time, ignore the click
	if (GetTickCount() < popupUI->dwDelayUntill) return;
    
    // Quit
    popupUI->Hide();    
}

//////////////////////////////////////////////////////////////////////////////////////////
void ChestUI::QtySelectPopupUI::SliderUpdateQtyEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Clicked on the slider, update the displayed amount.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    QtySelectPopupUI *popupUI = static_cast< ChestUI::QtySelectPopupUI * >( GetUI() );

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
bool ChestUI::QtySelectPopupUI::AmountEditFilter::IsOK
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
