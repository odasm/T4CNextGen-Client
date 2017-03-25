// BuyUI.cpp
//
#include "../pch.h"
#include "BuyUI.h"
#include "../MemoryManager.h"
#include "RootBoxUI.h"
#include "../Global.h"
#include "ChatterUI.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]

extern Font *fNewGuiBtnFont;
extern Font *fNewGuiFont;

extern int TalkToX, TalkToY;
extern DWORD TalkToID;

namespace{
    T3VSBSound *qtySound = NULL;
    T3VSBSound *naSound = NULL;
    
    void InitSound(){
        if( qtySound == NULL ){
            qtySound = new T3VSBSound;
            qtySound->Create( "Quantity Buttons", TS_MEMORY );

            naSound = new T3VSBSound;
            naSound->Create( "General NA sound", TS_MEMORY );
        }
    };

    const int ItemNameSize = 171;
};


// Constructor //////////////////////////////////////////////////////////////////////////
BuyUI::BuyUI( void ) :
    itemList( itemListEvent ),
    sellButton( sellButtonEvent ),
    X_Button( hideEvent )
//////////////////////////////////////////////////////////////////////////////////////////
    {             

    InitSound();

    int dwXPos = (g_Global.GetScreenW()-576)/2;
    int dwYPos = (g_Global.GetScreenH()-150-368)/2;
    if(dwYPos <0)
       dwYPos =0;

    
    SetPos( FramePos( dwXPos, dwYPos, dwXPos+576, dwYPos+368 ) );
    SetHelpId( this, 332 );
    
    backBuy.SetSpriteId( "GUI_BackBuy" );
    backBuy.SetPos( FramePos( dwXPos, dwYPos, dwXPos+576, dwYPos+368 ) );
    backBuy.DrawAlpha(true);

    X_Button.SetPos        ( FramePos( dwXPos+576-28, dwYPos+0, dwXPos+576-4, dwYPos+0+24 ) );
    X_Button.SetSpriteId   ( "GUI_X_ButtonUp" );
    X_Button.SetDownUI     ( "GUI_X_ButtonDown" );
    X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
    SetHelpId( X_Button, 261 );



    sellButton.SetSpriteId   ( "GUI_ButtonUp"   );
    sellButton.SetDownUI     ( "GUI_ButtonDown" );
    sellButton.SetHighlightUI( "GUI_ButtonHUp"  );
    sellButton.SetPos        ( FramePos( dwXPos+472, dwYPos+283, dwXPos+472+60, dwYPos+283+32 ) );
    sellButtonEvent.SetUI( this );
    SetHelpId( sellButton, 336 );


    itemList.SetPos( FramePos( dwXPos+14, dwYPos+50, dwXPos+429, dwYPos+317 ) );
    itemList.SetupList( 5, 35, FramePos( 0, 0, 12, 11 ) );
    itemList.CenterColumn( 0 );
    itemList.CenterColumn( 1 );
    itemList.CenterColumn( 2 );
    itemList.CenterColumn( 3 );
    itemList.CenterColumn( 4 );
    itemList.SetColumnSize( 0, 53 );
    itemList.SetColumnSize( 1, 172 );
    itemList.SetColumnSize( 2, 60 );
    itemList.SetColumnSize( 3, 34 );
    itemList.SetColumnSize( 4, 15 );
    itemListEvent.SetUI( this );
    selectionSprite.SetSpriteId( "GUI_BackBuySelect" );
    itemList.SetSelectionSprite( &selectionSprite, 63, 10 );
    itemList.SetScrollRegions(
        FramePos( dwXPos+406, dwYPos+39 , dwXPos+433, dwYPos+61 ),
        FramePos( dwXPos+406, dwYPos+282, dwXPos+433, dwYPos+305 ),
        FramePos( dwXPos+406, dwYPos+62 , dwXPos+433, dwYPos+270 )
    );
    listArrowDown.SetPos( FramePos( dwXPos+406, dwYPos+39 , dwXPos+429, dwYPos+61  ) );
    listArrowUp  .SetPos( FramePos( dwXPos+406, dwYPos+282, dwXPos+429, dwYPos+305 ) );
    itemList.SetScrollDownDownUI( &listArrowDown );
    itemList.SetScrollUpDownUI( &listArrowUp );


    totalDisp.SetPos( FramePos( dwXPos+459, dwYPos+103 , dwXPos+549, dwYPos+115 ) );
    SetHelpId( totalDisp, 333 );

    reqDisp.SetPos( FramePos( dwXPos+459, dwYPos+152 , dwXPos+549, dwYPos+164 ) );
    SetHelpId( reqDisp, 334 ); 

    leftDisp.SetPos( FramePos( dwXPos+459, dwYPos+201 , dwXPos+549, dwYPos+213 ) );
    SetHelpId( leftDisp, 335 );

    AddChild( &backBuy );
    AddChild( &X_Button );
    AddChild( &sellButton );
    AddChild( &itemList );
    AddChild( &reqDisp );
    AddChild( &totalDisp );
    AddChild( &leftDisp );

    RootBoxUI::GetInstance()->AddChild( this );
}

// Destructor ////////////////////////////////////////////////////////////////////////////
BuyUI::~BuyUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
}

void BuyUI::Draw(V2SPRITEFX *vsfFX)
{
   int dwXPos = (g_Global.GetScreenW()-576)/2;
   int dwYPos = (g_Global.GetScreenH()-150-368)/2;
   if(dwYPos <0)
       dwYPos =0;
    
   GameUI::Draw( vsfFX );

   HDC hdc;
   DXDGetDC(&hdc, 695);

   // Draw les texte...
   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[70]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 472, dwYPos+283+4 , RGB(0, 0, 0)    , 0, strBtnTexte);

   // Draw Box Header
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[70]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 237+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,101)+ 237  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);

    
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[24]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,92)+ 457+1, dwYPos+40+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,92)+ 457  , dwYPos+40   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[74]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,92)+ 458  , dwYPos+84    , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[75]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,92)+ 458  , dwYPos+133   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[76]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,92)+ 458  , dwYPos+182   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[71]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,98)+ 117  , dwYPos+42    , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[72]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,35)+ 275  , dwYPos+42   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[73]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,35)+ 335  , dwYPos+42   , g_Global.GetBtnTCE(), 0, strBtnTexte);


   DXDReleaseDC(hdc, 695);
}

int BuyUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
BuyUI *BuyUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the only sell UI instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static BuyUI instance;
    return &instance;
}

//////////////////////////////////////////////////////////////////////////////////////////
void BuyUI::Buy
//////////////////////////////////////////////////////////////////////////////////////////
//  Called when a sell packet comes in.
// 
(
 const list< SellItemInfo > &sellItemList // The list of items to sell.
)
//////////////////////////////////////////////////////////////////////////////////////////
{       
    CAutoLock autoLock( RootBoxUI::GetInstance()->GetLock() );
    
    // If there are items in the sell list.
    if( sellItemList.size() != 0 ){

        // Remove previous items from grid
        itemList.DeleteList();
        
        // For each items in the list.
        list< SellItemInfo >::const_iterator i;
        for( i = sellItemList.begin(); i != sellItemList.end(); i++)
        {
            char buf[ 256 ];
            itemList.AddNewItem();

            // Create the item's icon.
            SellItemUI *newGraphItem = new SellItemUI;
            CV2Sprite *lpVSF = ItemIcons( (*i).appearance );
            newGraphItem->itemInfo = *i;
            newGraphItem->SetVSF( lpVSF );
            newGraphItem->SetPos( FramePos( 0, 0, lpVSF->GetWidth(), lpVSF->GetHeight() ) );
            sprintf(buf, "%s. %s", (*i).name.c_str(), (*i).reqs.c_str());
            newGraphItem->SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, buf ) );
            
            itemList.AddColumnItem( 0, newGraphItem );

            DWORD textColor;
            if( (*i).canEquip )
            {
                textColor = RGB( 255, 255, 255);
            }
            else
            {
                textColor = RGB( 255, 76, 54 );
            }

            // Create a new text objects.
            // The name
            StaticTextUI *newTextItem = new StaticTextUI;
            newTextItem->SetText( (*i).name, textColor );
            newTextItem->AutoTruncate( ItemNameSize );
            newTextItem->SetPos( FramePos( 0, 0, newTextItem->GetWidth(), newTextItem->GetHeight() ) );
            newTextItem->SetCenter( false );
            itemList.AddColumnItem( 1, newTextItem );

            // The price
            newTextItem = new StaticTextUI;            
            newTextItem->SetText( itoa( (*i).price, buf, 10 ), textColor );
            newTextItem->SetPos( FramePos( 0, 0, newTextItem->GetWidth(), newTextItem->GetHeight() ) );
            newTextItem->SetCenter( false );
            itemList.AddColumnItem( 2, newTextItem );

            // The current qty.
            newTextItem = new StaticTextUI;
            newTextItem->SetText( "0", textColor );
            newTextItem->SetPos( FramePos( 0, 0, newTextItem->GetWidth(), newTextItem->GetHeight() ) );
            newTextItem->SetCenter( false );
            itemList.AddColumnItem( 3, newTextItem );
            
            ListUI *newItemData = new ListUI( EmptyEvent::GetInstance() );
            ButtonUI *upButton = new ButtonUI( EmptyEvent::GetInstance() );
            ButtonUI *downButton = new ButtonUI( EmptyEvent::GetInstance() );

            upButton->SetHighlightUI( "GUI_SpinHUp" );
            upButton->SetPos( FramePos( 0, 0, 13, 13 ) );
            SetHelpId( upButton, 337 );
            downButton->SetHighlightUI( "GUI_SpinHDown" );
            downButton->SetPos( FramePos( 0, 0, 13, 13 ) );
            SetHelpId( downButton, 338 );

            newItemData->SetPos( FramePos( 0, 0, 13, 21 ) );
            newItemData->SetupList( 1, 10, FramePos( 0, 0, 0, 1 ) );
            newItemData->SetColumnSize( 0, 10 );
            newItemData->AddNewItem();
            newItemData->AddColumnItem( 0, upButton );
            newItemData->AddNewItem();
            newItemData->AddColumnItem( 0, downButton );

            itemList.AddColumnItem( 4, newItemData );

            reqDisp.SetText( "0" );
            totalDisp.SetText( itoa( Player.Gold, buf, 10 ) );
            leftDisp.SetText( itoa( Player.Gold, buf, 10 ) );
        }

        // Display sell box.
        Show();
    }   
}

//////////////////////////////////////////////////////////////////////////////////////////
void BuyUI::SellButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Player wants to sell the items it has selected.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    BuyUI *sellUI = static_cast< BuyUI * >( GetUI() );

    // Fetch all items to sell.
    vector< vector< ListUI::GridItem > > *items = sellUI->itemList.GetGrid();

    //bool send = false;//BLBLBL 29 mars 2009 suppression des choses inutilisées
    	
    // Get all items from the 1st column (SellItemUIs).    
    vector< ListUI::GridItem >::iterator i;
    vector< SellItemInfo > bought;
    
    for( i = (*items)[0].begin(); i != (*items)[0].end(); i++ ){
        SellItemUI *sellItem = static_cast< SellItemUI * >( *i );

        // If we bought this item
        if( sellItem->itemInfo.qty != 0 ){
            bought.push_back( sellItem->itemInfo );
        }
    }
    // If any items were bought.
    if( bought.size() != 0 ){
       TFCPacket sending;
       sending << (short)RQ_SendBuyItemList;
       sending << (short)TalkToX;
       sending << (short)TalkToY;
       sending << (long)TalkToID;      
       
       sending << (short)bought.size();
       vector< SellItemInfo >::iterator j;
       for( j = bought.begin(); j != bought.end(); j++ ){
          sending << (short)( (*j).dwID );
          sending << (short)( (*j).qty );
       }
       
       SEND_PACKET( sending );
    }

    sellUI->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void BuyUI::ItemListEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Players selected a new item from the list.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    BuyUI *sellUI = static_cast< BuyUI * >( GetUI() );

    int col = sellUI->itemList.GetSelectedColumn();

    // If the selected column is the +/- column
    if( col == 4 ){        
        const GameUI *selection = sellUI->itemList.GetSelection();

        if( selection == NULL ){
            return;
        }
        
        SellItemUI *item = static_cast< SellItemUI * >( 
            const_cast< GameUI * >( selection ) 
        );

        // If clicked on the upper + part of the arrow column
        if( MouseDevice::yPos < 
            sellUI->itemList.GetSelectedRowItem(4)->GetPos().y + 
            sellUI->itemList.GetSelectedRowItem(4)->GetPos().Height() / 2 ){

            int currentPrice = atoi( sellUI->reqDisp.GetText()->GetText() );

            if( item->itemInfo.price <= Player.Gold - currentPrice ){
                char buf[ 256 ];

                item->itemInfo.qty++;

                StaticTextUI *qtyUI = static_cast< StaticTextUI * >( sellUI->itemList.GetSelectedRowItem( 3 ) );
                qtyUI->SetText( itoa( item->itemInfo.qty, buf, 10 ) );
            
                currentPrice += item->itemInfo.price;
            
                sellUI->reqDisp.SetText( itoa( currentPrice, buf, 10 ) );
                sellUI->leftDisp.SetText( itoa( Player.Gold - currentPrice, buf, 10 ) );

                qtySound->Play( TRUE );
            }else{
                ChatterUI::GetInstance()->AddBackscrollItem(
                    "",
                    g_LocalString[ 536 ],
                    SYSTEM_COLOR,
                    false
                );

                naSound->Play( TRUE );
            }
        }
        // If clicked on the lower - part of the arrow column
        else{
            if( item->itemInfo.qty != 0 ){
                char buf[ 256 ];

                item->itemInfo.qty--;

                int currentPrice = atoi( sellUI->reqDisp.GetText()->GetText() );

                currentPrice -= item->itemInfo.price;
            
                sellUI->reqDisp.SetText( itoa( currentPrice, buf, 10 ) );
                sellUI->leftDisp.SetText( itoa( Player.Gold - currentPrice, buf, 10 ) );                    

                StaticTextUI *qtyUI = static_cast< StaticTextUI * >( sellUI->itemList.GetSelectedRowItem( 3 ) );
                qtyUI->SetText( itoa( item->itemInfo.qty, buf, 10 ) );
                
                qtySound->Play( TRUE );
            }else{
                naSound->Play( TRUE );
            }
        }
    }else{
        LOG << "\r\nPressed NOT!!";
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void BuyUI::ItemListEvent::LeftDblClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Double-clicking on an item in the list buys it.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    BuyUI *sellUI = static_cast< BuyUI * >( GetUI() );
    
    // If the selecti
    int col = sellUI->itemList.GetSelectedColumn();
    // If the player dbl-clicked the up/down arrow
    if( col == 4 ){
        LeftClicked();
        return;
    }

    const GameUI *selection = sellUI->itemList.GetSelection();
    if( selection == NULL ){
       return;
    }
    
    SellItemUI *item = static_cast< SellItemUI * >( 
       const_cast< GameUI * >( selection ) 
       );
    
    // Buy the selected item.
    TFCPacket sending;
    sending << (short)RQ_SendBuyItemList;
    sending << (short)TalkToX;
    sending << (short)TalkToY;
    sending << (long)TalkToID;      
    
    sending << (short)1;
    sending << (short)( item->itemInfo.dwID );
    sending << (short)( 1 );
    
    SEND_PACKET( sending );
    
    sellUI->Hide();
}