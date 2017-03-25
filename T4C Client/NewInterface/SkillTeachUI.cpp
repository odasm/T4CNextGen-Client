// SkillTeachUI.cpp
//
#include "../pch.h"
#include "SkillTeachUI.h"
#include "../MemoryManager.h"
#include "RootBoxUI.h"
#include "../Global.h"
#include "../PacketTypes.h"
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

    const int SkillNameSize = 171;
};


// Constructor //////////////////////////////////////////////////////////////////////////
SkillTeachUI::SkillTeachUI( void ) :
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
    SetHelpId( this, 339 );
    
    backSell.SetSpriteId( "GUI_BackSkill" );
    backSell.SetPos( FramePos( dwXPos, dwYPos, dwXPos+576, dwYPos+368 ) );
    backSell.DrawAlpha(true);

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
    SetHelpId( sellButton, 342 );

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

    skillDisp.SetPos( FramePos( dwXPos+459, dwYPos+253 , dwXPos+549, dwYPos+265 ) );
    SetHelpId( skillDisp, 341 );
    
    AddChild( &backSell);
    AddChild( &X_Button );
    AddChild( &sellButton );
    AddChild( &itemList );
    AddChild( &reqDisp );
    AddChild( &totalDisp );
    AddChild( &leftDisp );
    AddChild( &skillDisp );



    RootBoxUI::GetInstance()->AddChild( this );
}

// Destructor ////////////////////////////////////////////////////////////////////////////
SkillTeachUI::~SkillTeachUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
}

void SkillTeachUI::Draw(V2SPRITEFX *vsfFX)
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
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[78]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 472, dwYPos+283+4 , RGB(0, 0, 0)    , 0, strBtnTexte);

   // Draw Box Header
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[78]);
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

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[114]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,92)+ 458  , dwYPos+234   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[79]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,98)+ 117  , dwYPos+42    , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[72]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,35)+ 275  , dwYPos+42   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[80]);
   fNewGuiFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiFont,hdc,strBtnTexte ,35)+ 335  , dwYPos+42   , g_Global.GetBtnTCE(), 0, strBtnTexte);


   DXDReleaseDC(hdc, 695);
}

int SkillTeachUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
SkillTeachUI *SkillTeachUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the only sell UI instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static SkillTeachUI instance;
    return &instance;
}

//////////////////////////////////////////////////////////////////////////////////////////
void SkillTeachUI::Train
//////////////////////////////////////////////////////////////////////////////////////////
//  Called when a sell packet comes in.
// 
(
 WORD newSkillPnts,
 const list< SellItemInfo > &sellItemList // The list of items to sell.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( RootBoxUI::GetInstance()->GetLock() );

    skillPnts = newSkillPnts;
    
    // If there are items in the sell list.
    if( sellItemList.size() != 0 ){

        // Remove previous items from grid
        itemList.DeleteList();
        
        // For each items in the list.
        list< SellItemInfo >::const_iterator i;
        for( i = sellItemList.begin(); i != sellItemList.end(); i++ ){
            DWORD textColor;
            if( (*i).canHave ){
                textColor = RGB( 255, 255, 255 );
            }else{
                textColor = RGB( 255, 76, 54 );
            }

            char buf[ 256 ];
            itemList.AddNewItem();

            // Create the item's icon.
            SellItemUI *newGraphItem = new SellItemUI;
            CV2Sprite *lpVSF;
            if( (*i).skillID < 10000 )
            {
                lpVSF = SkillIcons( (*i).icon );
            }else{
                lpVSF = SpellIcons( (*i).icon );
            }

            string helpText( (*i).name.c_str() );
            helpText += "; ";
            helpText += (*i).reqs;

            newGraphItem->itemInfo = *i;
            newGraphItem->SetVSF( lpVSF );
            newGraphItem->SetPos( FramePos( 0, 0, lpVSF->GetWidth(), lpVSF->GetHeight() ) );
            newGraphItem->SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, helpText.c_str() ) );
            
            itemList.AddColumnItem( 0, newGraphItem );

            // Create a new text objects.
            // The name
            StaticTextUI *newTextItem = new StaticTextUI;
            newTextItem->SetText( (*i).name, textColor );            
            newTextItem->AutoTruncate( SkillNameSize );
            newTextItem->SetPos( FramePos( 0, 0, newTextItem->GetWidth(), newTextItem->GetHeight() ) );
            newTextItem->SetCenter( false );
            itemList.AddColumnItem( 1, newTextItem );

            // The price
            newTextItem = new StaticTextUI;            
            newTextItem->SetText( itoa( (*i).price, buf, 10 ), textColor );
            newTextItem->SetPos( FramePos( 0, 0, newTextItem->GetWidth(), newTextItem->GetHeight() ) );
            newTextItem->SetCenter( false );
            itemList.AddColumnItem( 2, newTextItem );

            // The skill cost in points
            newTextItem = new StaticTextUI;
            newTextItem->SetText( "0", textColor );
            newTextItem->SetPos( FramePos( 0, 0, newTextItem->GetWidth(), newTextItem->GetHeight() ) );
            newTextItem->SetCenter( false );
            itemList.AddColumnItem( 3, newTextItem );
            
            // Up/Down arrows
            ListUI *newItemData = new ListUI( EmptyEvent::GetInstance() );
            ButtonUI *upButton = new ButtonUI( EmptyEvent::GetInstance() );
            ButtonUI *downButton = new ButtonUI( EmptyEvent::GetInstance() );

            upButton->SetHighlightUI( "GUI_SpinHUp" );
            upButton->SetPos( FramePos( 0, 0, 13, 13 ) );
            SetHelpId( upButton, 343 );
            downButton->SetHighlightUI( "GUI_SpinHDown" );
            downButton->SetPos( FramePos( 0, 0, 13, 13 ) );
            SetHelpId( downButton, 344 );

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
            skillDisp.SetText( itoa( skillPnts, buf, 10 ) );
        }

        // Display sell box.
        Show();
    }   
}

//////////////////////////////////////////////////////////////////////////////////////////
void SkillTeachUI::SellButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Player wants to sell the items it has selected.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SkillTeachUI *sellUI = static_cast< SkillTeachUI * >( GetUI() );

    // Fetch all items to sell.
    vector< vector< ListUI::GridItem > > &items = *sellUI->itemList.GetGrid();

    // Get all items from the 4th column (SellItemUIs).
    vector< ListUI::GridItem >::iterator i;

    vector< SellItemInfo > bought;

    // If there is only one item to teach
    if( items[0].size() == 1 ){
        // Teach this single skill.
        SellItemUI *sellItem = static_cast< SellItemUI * >( *items[0].begin() ); 
        bought.push_back( sellItem->itemInfo );
    }else{
        for( i = items[0].begin(); i != items[0].end(); i++ ){
            SellItemUI *sellItem = static_cast< SellItemUI * >( *i ); 

            // If we teach this item
            if( sellItem->itemInfo.teach ){
                bought.push_back( sellItem->itemInfo );            
            }
        }
    }

    if( bought.size() > 0 ){
        TFCPacket sending;
			
        sending << (short)RQ_SendTeachSkillList;
		sending << (short)TalkToX;
		sending << (short)TalkToY;
		sending << (long)TalkToID;

        vector< SellItemInfo >::iterator i;
        for( i = bought.begin(); i != bought.end(); i++ ){					
			sending << (short)(*i).skillID;	// ID of skill to train.													
		}

		SEND_PACKET( sending );
    }

    sellUI->Hide();
}

//////////////////////////////////////////////////////////////////////////////////////////
void SkillTeachUI::ItemListEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Players selected a new item from the list.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SkillTeachUI *sellUI = static_cast< SkillTeachUI * >( GetUI() );

    int col = sellUI->itemList.GetSelectedColumn();

    // If the selected column is the +/- column
    if( col == 4 ){        
        const GameUI *selection = sellUI->itemList.GetSelection();

        if( selection != NULL ){
            SellItemUI *item = static_cast< SellItemUI * >( const_cast< GameUI * >( selection ) );
            
            // If clicked on the upper + part of the arrow column
            if( MouseDevice::yPos < 
                sellUI->itemList.GetSelectedRowItem(4)->GetPos().y + 
                sellUI->itemList.GetSelectedRowItem(4)->GetPos().Height() / 2 ){
                
                
                // If the player can spend more skill points.
                if( !item->itemInfo.teach ){
                    // If player doesn't have enough skill points.
                    if( item->itemInfo.skillPnts > sellUI->skillPnts ){
                        ChatterUI::GetInstance()->AddBackscrollItem(
                            "",
                            g_LocalString[ 538 ],
                            SYSTEM_COLOR,
                            false
                        );                            

                        naSound->Play( TRUE );
                        return;
                    }

                    int currentPrice = atoi( sellUI->reqDisp.GetText()->GetText() );
                    // If the price still is enough.
                    if( currentPrice + item->itemInfo.price <= Player.Gold && item->itemInfo.canHave ){
                        char buf[ 256 ];

                        item->itemInfo.teach = true;
            
                        StaticTextUI *qtyUI = static_cast< StaticTextUI * >( sellUI->itemList.GetSelectedRowItem( 3 ) );
                        qtyUI->SetText( itoa( item->itemInfo.skillPnts, buf, 10 ) );
                
                        currentPrice += item->itemInfo.price;

                        sellUI->skillPnts -= item->itemInfo.skillPnts;
                    
                        sellUI->reqDisp.SetText( itoa( currentPrice, buf, 10 ) );
                        sellUI->leftDisp.SetText( itoa( Player.Gold - currentPrice, buf, 10 ) );

                        qtySound->Play( TRUE );
                    }else{
                        if( !item->itemInfo.canHave ){
                            ChatterUI::GetInstance()->AddBackscrollItem(
                                "",
                                g_LocalString[ 537 ],
                                SYSTEM_COLOR,
                                false
                            );                            
                        } else if( currentPrice + item->itemInfo.price > Player.Gold ){
                            ChatterUI::GetInstance()->AddBackscrollItem(
                                "",
                                g_LocalString[ 536 ],
                                SYSTEM_COLOR,
                                false
                            );
                        }
                        naSound->Play( TRUE );
                        LOG << "\r\nPrice too high!!!!";
                    }
                }else{
                    LOG << "\r\nQty not OK!!!!";

                }
            }
            // If clicked on the lower - part of the arrow column
            else{
                if( item->itemInfo.teach ){
                    char buf[ 256 ];

                    int currentPrice = atoi( sellUI->reqDisp.GetText()->GetText() );

                    currentPrice -= item->itemInfo.price;

                    sellUI->skillPnts += item->itemInfo.skillPnts;
                
                    sellUI->reqDisp.SetText( itoa( currentPrice, buf, 10 ) );
                    sellUI->leftDisp.SetText( itoa( Player.Gold - currentPrice, buf, 10 ) );

                    item->itemInfo.teach = false;

                    StaticTextUI *qtyUI = static_cast< StaticTextUI * >( sellUI->itemList.GetSelectedRowItem( 3 ) );
                    qtyUI->SetText( "0" );

                    qtySound->Play( TRUE );
                }else{
                    naSound->Play( TRUE );
                }                
            }
        }
    }else{
        LOG << "\r\nPressed NOT!!";
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void SkillTeachUI::ItemListEvent::LeftDblClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Double-clicking on an item in the list buys it.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    SkillTeachUI *sellUI = static_cast< SkillTeachUI * >( GetUI() );

    // If the selecti
    int col = sellUI->itemList.GetSelectedColumn();
    // If the player dbl-clicked the up/down arrow
    if( col == 4 ){
        return;
    }

    const GameUI *selection = sellUI->itemList.GetSelection();
    if( selection == NULL ){
        return;
    }

    SellItemUI *item = static_cast< SellItemUI * >( 
        const_cast< GameUI * >( selection ) 
    );

    if( !item->itemInfo.canHave ){
        ChatterUI::GetInstance()->AddBackscrollItem(
            "",
            g_LocalString[ 537 ],
            SYSTEM_COLOR,
            false
        );                            

        return;
    }

    TFCPacket sending;
    sending << (short)RQ_SendTeachSkillList;
    sending << (short)TalkToX;
	sending << (short)TalkToY;
	sending << (long)TalkToID;

    sending << (short)item->itemInfo.skillID;	// ID of skill to teach

	SEND_PACKET( sending );

    sellUI->Hide();
}