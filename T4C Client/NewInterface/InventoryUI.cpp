// InventoryUI.cpp
//
#pragma warning( disable : 4291 )

#include "../pch.h"

#include "InventoryUI.h"
#include "RootBoxUI.h"
#include "RTItemI.h"
#include "GraphUI.h"
#include "ChestUI.h"
#include "TradeUI.h"
#include "RTHelp.h"
#include "../MemoryManager.h"
#include "ChatterUI.h"
#include "..\SaveGame.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]
#include "..\Zlib/zlib.h"

extern Font *fNewGuiBtnFont;
extern Font *fNewGuiInfoFont;
extern Font *fNewGuiInfoFont2;
extern CSaveGame g_SaveGame;

#define V2IMAGEUSENAME_HEADER   "\\Game Files\\UseItemBDI.dat"
#define V2IMAGEUSENAME_DATA     "\\Game Files\\UseItemBDD.dat"

using namespace std;

namespace{
    T3VSBSound *dropSound = NULL;
    T3VSBSound *naSound = NULL;

    void InitSound()
    {
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
    
    //////////////////////////////////////////////////////////////////////////////////////////
    //  Drops back the given item UI in its original ui.
    // 
    //////////////////////////////////////////////////////////////////////////////////////////
    bool DropBack2(GameUI *itemUI )
    {
        GameUI *src = GameUI::GetDragParentUI();
        if( src != NULL )
        {
            // If the source is a grid UI
            if( typeid( *src ) == typeid( GridUI ) )
            {

                // Try to see if the grid has space left.
                GridUI *srcGrid = static_cast< GridUI * >( src );
                if( srcGrid->CanDrop( GameUI::GetDragInitialPos() ) )
                {
                   srcGrid->DropItem(GameUI::GetDragInitialPos());//LastPos();
                   GameUI::CancelDrag();
                   CMouseCursor::GetInstance()->SetCustomCursor( NULL );
                }
                else
                {
                  srcGrid->AddItem( itemUI );
                  GameUI::CancelDrag();
                  CMouseCursor::GetInstance()->SetCustomCursor( NULL );
                }
            }
            return true;
        }
        return false;
    };
}

// Constructor ///////////////////////////////////////////////////////////////////////////
 InventoryUI::InventoryUI( void )
     :  junkButton( junkButtonEvent ),
        dropButton( dropButtonEvent ),
        TradeButton( tradeButtonEvent ),
        inventoryGrid( inventoryGridEvent ),
        mainDropZone( equipEvent ),
        headDropZone( equipEvent ),
        bodyDropZone( equipEvent ),
        leftDropZone( equipEvent ),
        rightDropZone( equipEvent ),
        legsDropZone( equipEvent ),
        feetDropZone( equipEvent ),
        capeDropZone( equipEvent ),
        glovesDropZone( equipEvent ),
        neckDropZone( equipEvent ),
        wristDropZone( equipEvent ),
        beltDropZone( equipEvent ),
        ring1DropZone( equipEvent ),
        ring2DropZone( equipEvent ),
        macroButton( macroButtonEvent ),
        X_Button( hideEvent ),
        iconSTR( EmptyEvent::GetInstance() ),
        iconEND( EmptyEvent::GetInstance() ),
        iconAGI( EmptyEvent::GetInstance() ),
        iconWIS( EmptyEvent::GetInstance() ),
        iconINT( EmptyEvent::GetInstance() ),
        iconAC( EmptyEvent::GetInstance() ),
        iconGold( EmptyEvent::GetInstance() )
     
//////////////////////////////////////////////////////////////////////////////////////////
{
     InitSound();

     // Set this boxe's sprite and position.
      
     int dwXPos = (g_Global.GetScreenW()-550)/2;
     int dwYPos = (g_Global.GetScreenH()-150-430)/2;
     if(dwYPos <0)
        dwYPos =0;
     
     SetPos( FramePos( dwXPos, dwYPos, dwXPos+550, dwYPos+430 ) );
     SetHelpId( this, 285 );
     
     backInvT.SetSpriteId( "GUI_InvBackT" );
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

     TradeButton.SetPos ( FramePos( dwXPos+478, dwYPos+280, dwXPos+478+60, dwYPos+280+32 ) );
     TradeButton.SetSpriteId   ( "GUI_ButtonUp" );
     TradeButton.SetDownUI     ( "GUI_ButtonDown" );
     TradeButton.SetHighlightUI( "GUI_ButtonHUp" );
     SetHelpId( TradeButton, 574 );
     

     // Setup the different control positions.     
     dropButton.SetPos ( FramePos( dwXPos+478, dwYPos+330, dwXPos+478+60, dwYPos+330+32 ) );
     dropButton.SetSpriteId( "GUI_ButtonUp" );     
     dropButton.SetDownUI( "GUI_ButtonDown" );
     dropButton.SetHighlightUI( "GUI_ButtonHUp" );
     SetHelpId( dropButton, 280 );     
     
    
     junkButton.SetPos ( FramePos( dwXPos+478, dwYPos+366, dwXPos+478+60, dwYPos+366+32 ) );
     junkButton.SetSpriteId   ( "GUI_ButtonUp" );
     junkButton.SetDownUI     ( "GUI_ButtonDown" );
     junkButton.SetHighlightUI( "GUI_ButtonHUp" );
     SetHelpId( junkButton, 281 );

     macroButton.SetPos ( FramePos( dwXPos+550-35-60, dwYPos+1, dwXPos+550-35, dwYPos+1+22 ) );
     macroButton.SetSpriteId( "GUI_ButtonTUp" );
     macroButton.SetDownUI( "GUI_ButtonTDown" );
     macroButton.SetHighlightUI( "GUI_ButtonTHUp" );
     macroButtonEvent.SetUI( this );

     inventoryGrid.SetPos( FramePos( dwXPos+24, dwYPos+270, dwXPos+459, dwYPos+386 ) );
     inventoryGrid.SetScrollRegions(
         FramePos( dwXPos+437, dwYPos+276, dwXPos+454, dwYPos+297 ), 
         FramePos( dwXPos+437, dwYPos+362, dwXPos+454, dwYPos+382 ),
         FramePos( dwXPos+437, dwYPos+298, dwXPos+454, dwYPos+354 ),
         NULL
     );
     inventoryGrid.SetGrid( 14, 4, FramePos( 0, 0, 26, 26 ), FramePos( 0, 0, 3, 3 ) );
     inventoryGrid.AllowHelp(false);
     SetHelpId( inventoryGrid, 284 );

     gridScrollUpGraph  .SetPos( FramePos( dwXPos+433, dwYPos+276, dwXPos+454, dwYPos+297 ) );
     gridScrollDownGraph.SetPos( FramePos( dwXPos+433, dwYPos+362, dwXPos+454, dwYPos+382 ) );

     inventoryGrid.SetScrollUpDownUI( &gridScrollUpGraph );
     inventoryGrid.SetScrollDownDownUI( &gridScrollDownGraph );

     // Set the dropzones positions.
     mainDropZone  .SetPos(  FramePos( dwXPos+417, dwYPos+72, dwXPos+550, dwYPos+246 ) );
     headDropZone  .SetPos(  FramePos( dwXPos+85, dwYPos+32, dwXPos+144, dwYPos+88 ) );
     bodyDropZone  .SetPos(  FramePos( dwXPos+85, dwYPos+97, dwXPos+144, dwYPos+186 ) );
     legsDropZone  .SetPos(  FramePos( dwXPos+85, dwYPos+194,dwXPos+144, dwYPos+250 ) );
     rightDropZone .SetPos(  FramePos( dwXPos+14, dwYPos+97, dwXPos+73, dwYPos+185 ) );
     leftDropZone  .SetPos(  FramePos( dwXPos+156, dwYPos+97, dwXPos+216, dwYPos+185 ) );
     feetDropZone  .SetPos(  FramePos( dwXPos+14, dwYPos+193, dwXPos+73, dwYPos+250 ) );
     capeDropZone  .SetPos(  FramePos( dwXPos+227, dwYPos+97, dwXPos+286, dwYPos+185 ) );
     glovesDropZone.SetPos(  FramePos( dwXPos+203, dwYPos+205, dwXPos+238, dwYPos+239 ) );
     neckDropZone  .SetPos(  FramePos( dwXPos+39, dwYPos+44,  dwXPos+74,  dwYPos+77 ) );
     wristDropZone .SetPos(  FramePos( dwXPos+156, dwYPos+44, dwXPos+190, dwYPos+77 ) );
     beltDropZone  .SetPos(  FramePos( dwXPos+156, dwYPos+205,dwXPos+190, dwYPos+239 ) );
     ring1DropZone .SetPos(  FramePos( dwXPos+203, dwYPos+44, dwXPos+238, dwYPos+77 ) );
     ring2DropZone .SetPos(  FramePos( dwXPos+253, dwYPos+44, dwXPos+287, dwYPos+77 ) );
     
     SetHelpId( capeDropZone,   267 );
     SetHelpId( leftDropZone,   271 );
     SetHelpId( headDropZone,   269 );
     SetHelpId( bodyDropZone,   270 );
     SetHelpId( rightDropZone,  268 );
     SetHelpId( legsDropZone,   272 );
     SetHelpId( feetDropZone,   273 );
     SetHelpId( glovesDropZone, 274 );
     SetHelpId( mainDropZone,   275 );
     SetHelpId( neckDropZone,   276 );
     SetHelpId( wristDropZone,  277 );
     SetHelpId( beltDropZone,   278 );
     SetHelpId( ring1DropZone,  279 );
     SetHelpId( ring2DropZone,  279 );

     // Set the UI's for the buttons.
     junkButtonEvent.SetUI( this );
     dropButtonEvent.SetUI( this );
     tradeButtonEvent.SetUI( this );
     equipEvent.SetUI( this );
     
     gridSelectedTile.SetSpriteId( "GUI_BackInvGridSelect" );


     inventoryGrid.SetSelectedTileUI( &gridSelectedTile );

     instantItemInfo.SetPos( FramePos( dwXPos+208, dwYPos+398, dwXPos+453, dwYPos+414 ) );
     instantItemInfo.SetCenter( false );

     statAC .SetPos ( FramePos( dwXPos+353, dwYPos+51 , dwXPos+399, dwYPos+67  ) );
     statSTR.SetPos ( FramePos( dwXPos+353, dwYPos+96 , dwXPos+399, dwYPos+112  ) );
     statEND.SetPos ( FramePos( dwXPos+353, dwYPos+130, dwXPos+399, dwYPos+146  ) );
     statAGI.SetPos ( FramePos( dwXPos+353, dwYPos+164, dwXPos+399, dwYPos+180  ) );
     statWIS.SetPos ( FramePos( dwXPos+353, dwYPos+198, dwXPos+399, dwYPos+214  ) );
     statINT.SetPos ( FramePos( dwXPos+353, dwYPos+232, dwXPos+399, dwYPos+248  ) );
     statGold.SetPos( FramePos( dwXPos+89 , dwYPos+398, dwXPos+174, dwYPos+413  ) );
     
     statSTR.SetHelpText ( FontManager::GetInstance()->GetSmallFont(FALSE) );
     statEND.SetHelpText ( FontManager::GetInstance()->GetSmallFont(FALSE) );
     statAGI.SetHelpText ( FontManager::GetInstance()->GetSmallFont(FALSE) );
     statWIS.SetHelpText ( FontManager::GetInstance()->GetSmallFont(FALSE) );
     statINT.SetHelpText ( FontManager::GetInstance()->GetSmallFont(FALSE) );
     statAC.SetHelpText  ( FontManager::GetInstance()->GetSmallFont(FALSE) );

     iconAC.SetPos  ( FramePos( dwXPos+317, dwYPos+45 , dwXPos+346, dwYPos+73 ) );
     iconSTR.SetPos ( FramePos( dwXPos+317, dwYPos+91 , dwXPos+346, dwYPos+118 ) );
     iconEND.SetPos ( FramePos( dwXPos+317, dwYPos+125, dwXPos+346, dwYPos+152 ) );
     iconAGI.SetPos ( FramePos( dwXPos+317, dwYPos+158, dwXPos+346, dwYPos+186 ) );
     iconWIS.SetPos ( FramePos( dwXPos+317, dwYPos+192, dwXPos+346, dwYPos+219 ) );
     iconINT.SetPos ( FramePos( dwXPos+317, dwYPos+226, dwXPos+346, dwYPos+254 ) );
     iconGold.SetPos( FramePos( dwXPos+29 , dwYPos+398, dwXPos+81 , dwYPos+413  ) );

     iconSTR.EnableSound( false );
     iconEND.EnableSound( false );
     iconAGI.EnableSound( false );
     iconWIS.EnableSound( false );
     iconINT.EnableSound( false );
     iconAC .EnableSound( false );
     iconGold.EnableSound( false );

     SetHelpId( iconSTR, 262 );
     SetHelpId( iconEND, 263 );
     SetHelpId( iconAGI, 264 );
     SetHelpId( iconWIS, 265 );
     SetHelpId( iconINT, 266 );
     SetHelpId( iconAC,  448 );
     SetHelpId( iconGold, 450 );
     SetHelpId( macroButton, 449 );

     // Add this box's children
     AddChild( &backInvT);
     AddChild( &backInvB);
     AddChild( &X_Button);
     AddChild( &iconSTR );
     AddChild( &iconEND );
     AddChild( &iconAGI );
     AddChild( &iconWIS );
     AddChild( &iconINT );
     AddChild( &iconAC );
     AddChild( &iconGold );
     AddChild( &inventoryGrid );
     AddChild( &TradeButton);
     AddChild( &dropButton );
     AddChild( &junkButton );
     AddChild( &macroButton );
     AddChild( &mainDropZone );
     AddChild( &headDropZone );
     AddChild( &bodyDropZone );
     AddChild( &leftDropZone );
     AddChild( &rightDropZone );
     AddChild( &legsDropZone );
     AddChild( &feetDropZone );
     AddChild( &capeDropZone );
     AddChild( &glovesDropZone );
     AddChild( &neckDropZone );
     AddChild( &wristDropZone );
     AddChild( &beltDropZone );
     AddChild( &ring1DropZone );
     AddChild( &ring2DropZone );

     AddChild( &statAC );
     AddChild( &statGold );
     AddChild( &statSTR );
     AddChild( &statEND );
     AddChild( &statAGI );
     AddChild( &statWIS );
     AddChild( &statINT );
    

     // Load the backpack items.
     LoadBackpack();

     InitializeMacro();

     RootBoxUI::GetInstance()->AddChild( &junkDropPopup );
     
     // Add the inventory to the root box.
     RootBoxUI::GetInstance()->AddChild( this );  
     
     ReadDisplayBDImage();

   
}

// Destructor ////////////////////////////////////////////////////////////////////////////
 InventoryUI::~InventoryUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
    m_vImageDisplay.clear();
}

void InventoryUI::ReadDisplayBDImage()
{
   int dwNbrImage = 0;
  
   unsigned long dwHeader       = (1000*54);
   unsigned char *pHeader       = new unsigned char[dwHeader];
   memset(pHeader,0x00,dwHeader);


   unsigned long dwHeaderC      = 0;
   unsigned char *pHeaderC      = NULL;
  

   ClientConfig *pConfig = g_Global.GetClientConfig();
   char strPathFile[1024];
   sprintf(strPathFile,"%s%s",pConfig->strPath,V2IMAGEUSENAME_HEADER);


   FILE *pfH;
   pfH=fopen(strPathFile,"rb");
   if(pfH)
   {
      fread(&dwNbrImage ,4,1,pfH);
      fread(&dwHeaderC  ,4,1,pfH);
      pHeaderC       = new unsigned char[dwHeaderC];
      fread(pHeaderC    ,dwHeaderC,1,pfH);
      fclose(pfH);
   }
   else
   {
      if(pHeader)
         delete []pHeader;
      if(pHeaderC)
         delete []pHeaderC;
      return;
   }


   if( uncompress(pHeader, &dwHeader, pHeaderC, dwHeaderC) != Z_OK )
   {
      if(pHeader)
         delete []pHeader;
      if(pHeaderC)
         delete []pHeaderC;
      return;
   }

   unsigned char *pHeaderOTmp = pHeader;
   unsigned char *pHeaderNTmp = pHeader+(1000*4);

   for(int i=0;i<dwNbrImage;i++)
   {
      int dwOffset;
      memcpy(&dwOffset,pHeaderOTmp,4);

      sImagesDisplay newImage;
      sprintf(newImage.strName,"%s",pHeaderNTmp);
      newImage.iOffset = dwOffset;

      m_vImageDisplay.push_back(newImage);

      pHeaderNTmp+=50;
      pHeaderOTmp+=4;
   }

   int dwSize = m_vImageDisplay.size();
  
   if(pHeader)
      delete []pHeader;
   if(pHeaderC)
      delete []pHeaderC;
}

void InventoryUI::ReadImageFile(int dwOffset, BYTE *pImage, BYTE *pPal)
{
   ClientConfig *pConfig = g_Global.GetClientConfig();
   char strPathFile[1024];
   sprintf(strPathFile,"%s%s",pConfig->strPath,V2IMAGEUSENAME_DATA);

   FILE *pfD;
   pfD=fopen(strPathFile,"rb");
   if(pfD)
   {
      fseek( pfD, dwOffset, SEEK_SET);

      unsigned long dwData       = (640*480)+(256*3);
      unsigned char *pData       = new unsigned char[dwData];
      
      
      unsigned long dwDataC      = 0;
      fread(&dwDataC  ,4,1,pfD);
      unsigned char *pDataC      = new unsigned char[dwDataC];
      
      fread(pDataC    ,dwDataC,1,pfD);
 
      fclose(pfD);

      if( uncompress(pData, &dwData, pDataC, dwDataC) != Z_OK )
      {
         if(pData)
            delete []pData;
         if(pDataC)
            delete []pDataC;

         return;
      }

      BYTE *pchImageByte = new BYTE[640*480];

      int dwPosDest = 0;
      
      for(int y=480-1;y>=0;y--)
      {
         for(int x=0;x<640;x++)
         {
            pchImageByte[(y*640)+x] = (BYTE) pData[(dwPosDest*640)+x];
         }
         dwPosDest++;
      }

      memcpy(pImage,pchImageByte,640*480);
      memcpy(pPal  ,pData+(640*480),256*3);

      if(pData)
         delete []pData;
      if(pDataC)
         delete []pDataC;
      if(pchImageByte)
         delete []pchImageByte;
   }
}


//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::ClientShutdown( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Called when client is shutting down, saves the backpack information. (Safer than
// waiting for the destructor to be called.)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Save the user's backpack.
    SaveBackpack();
}

//////////////////////////////////////////////////////////////////////////////////////////
InventoryUI *InventoryUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the sole inventory instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static InventoryUI soleInstance;

    return &soleInstance;
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int InventoryUI::Display( void *null )
//////////////////////////////////////////////////////////////////////////////////////////
//  Callback displaying the inventory macro.
// 
// Return: unsigned int, 0
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Lock the root box since this call came from some unknown thread.
    CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );

    // If inventory is already shown.
    if( GetInstance()->IsShown() ){    
        // Hide box
        GetInstance()->Hide();
    }else{

	     TFCPacket sending;	
	     // Get the stats.
         sending << (RQ_SIZE)RQ_GetStatus;	
	     SEND_PACKET( sending );

	     sending.Destroy();
         // Get the equipment.
         sending << (RQ_SIZE)RQ_ViewEquiped;
	     SEND_PACKET( sending );

         sending.Destroy();
         sending << (RQ_SIZE)RQ_ViewBackpack;
         sending << (short)0;
         SEND_PACKET( sending );



        // Show box.
        GetInstance()->Show();
    }

    return 0;
} 


void InventoryUI::Hide()
{
   ItemUI *item;
   if( GetDragItem() != NULL )
   {
      item = static_cast< ItemUI * >( GetDragItem() );
      DropBack2( item);
   }
   CMouseCursor::GetInstance()->SetCustomCursor( NULL );
   //:CR_NMNMNM_SAVE 2006-10-10 //Add save on minimuze satte of GUI...
   g_SaveGame.bSave();

   BoxUI::Hide();
}


//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the inventory UI.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::Draw(V2SPRITEFX *vsfFX)
{
   int dwXPos = (g_Global.GetScreenW()-550)/2;
   int dwYPos = (g_Global.GetScreenH()-150-430)/2;
   if(dwYPos <0)
       dwYPos =0;

   GameUI::Draw( vsfFX );

   HDC hdc;
   DXDGetDC(&hdc, 670);

   // Draw les texte...
   char strBtnTexte[20];
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[16]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 550-35-60, dwYPos , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[25]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 478, dwYPos+282+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[26]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 478, dwYPos+332+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   sprintf(strBtnTexte  ,"%s",g_GUILocalString[27]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 478, dwYPos+368+2 , RGB(0, 0, 0)    , 0, strBtnTexte);

   // Draw GOLD id...
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[24]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,52)+ 26+1, dwYPos+395+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,52)+ 26  , dwYPos+395   , g_Global.GetBtnTCE(), 0, strBtnTexte);

   // Draw Box Header
   sprintf(strBtnTexte  ,"%s",g_GUILocalString[23]);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,99)+ 226+1, dwYPos+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
   fNewGuiBtnFont->DrawFont(hdc, dwXPos+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,99)+ 226  , dwYPos+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);


   DXDReleaseDC(hdc, 670);

   
   // Get the current mouse coordinates.
   MousePos mousePos( MouseDevice::xPos, MouseDevice::yPos );
   
   ItemUI * itemUI = NULL;
   if( inventoryGrid.GetPos().MousePosInFrame( mousePos ) )
   {
      itemUI = static_cast< ItemUI * >( inventoryGrid.GetGridItemAt( mousePos ) );
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
  			sending << (char)PL_SEARCHBACKPACK;
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
            instantItemInfo.SetText(to->GetText(),RGB( 223, 157, 0 ),246);
            instantItemInfo.Draw( vsfFX );
         }
      }        
   }

   //0 ==    
   //1 == /  bas gauche
   //2 == |  de face 
   //3 == \  bas droit
   //4 == <  vers la gauche
   //5 == 
   //6 == >  vers la droite
   //7 == \  haut gauche
   //8 == |  de dos
   //9 == /  haut droit

   static int dwCnt     = 0;
   static int dwDir     = 0;
   static int dwRealDir = 2;
   dwCnt++;
   static int dwOffset = 0;
   if(dwCnt >8)
   {
      dwCnt = 0;
      dwDir++; 
      if(dwDir >7)
         dwDir = 0;
      switch(dwDir)
      {
         case 0: dwRealDir = 2; dwOffset = 0;break;
         case 1: dwRealDir = 1; dwOffset = 0;break;
         case 2: dwRealDir = 4; dwOffset = 0;break;
         case 3: dwRealDir = 7; dwOffset = 0;break;
         case 4: dwRealDir = 8; dwOffset = 0;break;
         case 5: dwRealDir = 9; dwOffset = 5;break;
         case 6: dwRealDir = 6; dwOffset = 5;break;
         case 7: dwRealDir = 3; dwOffset = 5;break;
      }

   }

   
   Objects.Lock(0);
   Objects.DrawPuppet( dwXPos+459+dwOffset, dwYPos+200,dwRealDir );
   Objects.Unlock(0);
}

int InventoryUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::RequestBackpackUpdateFromServer()
//////////////////////////////////////////////////////////////////////////////////////////
//  Ask server to send a backpack update packet :D
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
	  TFCPacket sending;
     sending << (RQ_SIZE)RQ_ViewBackpack;
     sending << (short)0;
     SEND_PACKET( sending );
}


//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::UpdateInventory
//////////////////////////////////////////////////////////////////////////////////////////
//  Udpates the user's inventory.
// 
(
 TemplateList< BAG_ITEM > *newInventory // Old BAG_ITEM list
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    LOG << "\r\nReceived intenventory update.";
    
    
    vector< GameUI * >   exceptList;
    vector< BAG_ITEM * > newList;

    // Scroll through the new inventory.
    newInventory->Lock( "InventoryUI::UpdateInventory" );
    newInventory->ToHead();
    while( newInventory->QueryNext() )
    {

        BAG_ITEM *bagItem = newInventory->GetObject();

        // If this bag already has an associated bag item.
        if( bagItem->associatedUI != NULL )
        {
            // Add it to the except list.
            exceptList.push_back( bagItem->associatedUI );
        }
        else
        {
            // Otherwise add it to the new list.
            newList.push_back( bagItem );
        }
    }
    newInventory->Unlock( "InventoryUI::UpdateInventory" );

    // Lock the root box since this call came from some unknown thread.
    CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );

    // Delete all items in the grid which are not in the exceptList.
    inventoryGrid.DeleteGridExcept( exceptList );

	// If there is currently an item being dragged.
    if( GetDragItem() != NULL ){    
		
		if (GetDragSourceEvent() == &inventoryGridEvent || GetDragSourceEvent() == &equipEvent) {

			
			// Add all equipped items to the except list.
			if( Player.lpbEquipped[ body ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ body ]->associatedUI );
			}
			if( Player.lpbEquipped[ feet ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ feet ]->associatedUI );
			}
			if( Player.lpbEquipped[ gloves ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ gloves ]->associatedUI );
			}
			if( Player.lpbEquipped[ helm ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ helm ]->associatedUI );
			}
			if( Player.lpbEquipped[ legs ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ legs ]->associatedUI );
			}
			if( Player.lpbEquipped[ bracelets ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ bracelets ]->associatedUI );
			}
			if( Player.lpbEquipped[ necklace ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ necklace ]->associatedUI );
			}
			if( Player.lpbEquipped[ weapon_right ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ weapon_right ]->associatedUI );
			}
			if( Player.lpbEquipped[ weapon_left ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ weapon_left ]->associatedUI );
			}
			if( Player.lpbEquipped[ ring1 ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ ring1 ]->associatedUI );
			}
			if( Player.lpbEquipped[ ring2 ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ ring2 ]->associatedUI );
			}
			if( Player.lpbEquipped[ belt ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ belt ]->associatedUI );
			}
			if( Player.lpbEquipped[ sleeves ] != NULL ){
				exceptList.push_back( Player.lpbEquipped[ sleeves ]->associatedUI );
			}

			// If the drag item could not be found in the except list.
			if( find( exceptList.begin(), exceptList.end(), GetDragItem() ) == exceptList.end() ){
				// Stop dragging.
				delete GetDragItem();

				// Reset mouse cursor.
				CMouseCursor::GetInstance()->SetCustomCursor( NULL );

				// Cancel drag operation.
				CancelDrag();
			}
		}
    }

    // Insert all the newly found items.    
    UINT i;
    for( i = 0; i < newList.size(); i++ )
    {
        
        // If a sprite was specified.
        if( newList[ i ]->lpSprite != NULL )
        {
            // Create a new GameUI.
            ItemUI *newItem = new ItemUI;

            // Associate both bag item and item UI.
            newItem->bagItem = newList[ i ];
            newList[ i ]->associatedUI = newItem;
            
            newItem->Setup();

            // Add the UI to the inventory grid.
            inventoryGrid.AddItem( newItem );
        }
    }

    // This save the backpack for precaution
    SaveBackpack();

}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::ItemUI::Setup( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Setups the given item UI according to its binded BAG_ITEM data.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Set a 0, 0 pos to tell the grid to select a position.
    SetPos( FramePos( 0, 0, bagItem->lpSprite->GetWidth(), bagItem->lpSprite->GetHeight() ) );

    SetVSF( bagItem->lpSprite );

    UpdateName();
}

//////////////////////////////////////////////////////////////////////////////////////////
bool InventoryUI::ItemUI::RightMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Help requested for that object.
// 
(
 MousePos mousePos
)
// Return: bool, true.
//////////////////////////////////////////////////////////////////////////////////////////
{

    // If the item doesn't have a name.
    if( bagItem->chName[ 0 ] == 0 )
    {
        // Send a query item's name.
        TFCPacket sending;
        sending << (RQ_SIZE)RQ_QueryItemName;
   	  sending << (char)PL_SEARCHBACKPACK;
        sending << (long)bagItem->dwID;
        SEND_PACKET( sending );
    }
    
    g_Global.ResetItemInfo();
    g_Global.SetItemInfoID(bagItem->dwID);
    g_Global.SetItemInfoInventory(true);
    TFCPacket sending2;
    sending2 << (RQ_SIZE)RQ_QueryItemInfo;
    sending2 << (long)bagItem->dwID;
    SEND_PACKET( sending2 );
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::ShowItemName(UINT uiID)
{
   // If the item doesn't have a name.
   // Send a query item's name.
   TFCPacket sending;
   sending << (RQ_SIZE)RQ_QueryItemName;
   sending << (char)PL_SEARCHBACKPACK;
   sending << (long)uiID;
   SEND_PACKET( sending );
}

void FormatChargesLeft( char *buf, LPBAG_ITEM bagItem ){
    LOG.Lock();
    LOG << "\r\nCharges=" << bagItem->charges << ".";
    LOG.Unlock();

    if( bagItem->charges != 0 ){
        if( bagItem->charges > 0 ){
            bagItem->charges--;
            sprintf( buf, g_LocalString[ 433 ], bagItem->chName, bagItem->charges );

            static_cast< InventoryUI::ItemUI * >( bagItem->associatedUI )->UpdateName();
        }else{
            sprintf( buf, g_LocalString[ 534 ], bagItem->chName );
        }
    }else{
        sprintf( buf, g_LocalString[ 433 ], bagItem->chName, bagItem->dwQty-1 );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::InventoryGridEvent::LeftDblClicked()
//////////////////////////////////////////////////////////////////////////////////////////
// Uses an item in the inventory grid.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
   ItemUI *item;
   
   LOG << "\r\nInventory DblClick.";
   OutputDebugString("Inventory DblClick.\n");
   
   if( GetDragItem() == NULL )
   {
      return;
   }
   else
   {
      item = static_cast< ItemUI * >( GetDragItem() );
   }

   if (!strcmp( (const char *)item->bagItem->chName, "Unsealed letter from Owain" )    )
   {
      DropBack2( item);
      RTHelp   *pRTHelp  = RTHelp::GetInstance();
      pRTHelp->Show(true,1,1);
   }
   else if (!strcmp( (const char *)item->bagItem->chName, "Lettre d'Owain" )    )
   {
      DropBack2( item);
      RTHelp   *pRTHelp  = RTHelp::GetInstance();
      pRTHelp->Show(true,1,1);
   }
   else if (!strcmp( (const char *)item->bagItem->chName, "Crimsonscale's letter" ))
   {
      DropBack2( item);
      RTHelp   *pRTHelp  = RTHelp::GetInstance();
      pRTHelp->Show(true,1,0);
   }
   else if (!strcmp( (const char *)item->bagItem->chName, "Lettre de Crimsonscale" ))
   {
      DropBack2( item);
      RTHelp   *pRTHelp  = RTHelp::GetInstance();
      pRTHelp->Show(true,1,0);
   } 
   else if (!strcmp( (const char *)item->bagItem->chName, "Carte du labyrinthe" ))
   {
      DropBack2( item);
      RTHelp   *pRTHelp  = RTHelp::GetInstance();
      pRTHelp->Show(true,3,0);
   }
   else 
   {
      bool bFound = false;
      int  iIndex = 0;
      
      for(unsigned long i=0;i<InventoryUI::GetInstance()->m_vImageDisplay.size();i++)
      {
         char strTmp[1000];
         sprintf(strTmp,"[%s]      [%s]\n",(const char *)item->bagItem->chName,InventoryUI::GetInstance()->m_vImageDisplay[i].strName);
         OutputDebugString(strTmp);
         
         if (!stricmp( (const char *)item->bagItem->chName, InventoryUI::GetInstance()->m_vImageDisplay[i].strName ))
         {
            bFound = true;
            iIndex = i;
            i = InventoryUI::GetInstance()->m_vImageDisplay.size()+1;
         }
      }

      //NMNMNM FERIA-----
      if(bFound)
      {
         unsigned char *pImage = new unsigned char[640*480];
         unsigned char *pPal   = new unsigned char[256*3];
         InventoryUI::GetInstance()->ReadImageFile(InventoryUI::GetInstance()->m_vImageDisplay[iIndex].iOffset,pImage,pPal);

         DropBack2( item);
         RTHelp   *pRTHelp  = RTHelp::GetInstance();
         pRTHelp->ShowSpecial(pImage,pPal,640,480,InventoryUI::GetInstance()->m_vImageDisplay[iIndex].strName);

         delete []pImage;
         delete []pPal;

      }
      else
      {
         // standard object, send the use request to the server.
         char buf[ 256 ];
         FormatChargesLeft( buf, item->bagItem );
      
         ChatterUI::GetInstance()->AddBackscrollItem( 
            "", 
            buf, 
            SYSTEM_COLOR,
            false
            );

         //dois fermer la page inventaire...
      
      
         LOG << "\r\nUsing item!";
      
         // Create the packet
         TFCPacket sending;
         sending << (RQ_SIZE)RQ_UseObject;
         sending << (short)0;	// item in backpack.
         sending << (short)0;
         sending << (long)( item->bagItem->dwID );
      
         DropBack2( item);
         // send the use packet
         SEND_PACKET( sending );
         //InventoryUI::GetInstance()->Hide();
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool InventoryUI::InventoryGridEvent::Dropping( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Verify that the dropped item is ok.
// 
// Return: bool, true if dropped item is ok, false otherwise.
//////////////////////////////////////////////////////////////////////////////////////////
{
    GameUI *dragItem = GetDragItem();

    if( dragItem == NULL ){
        return false;
    }

    // If the dropped item is an ItemUI
    if( typeid( *dragItem ) == typeid( ItemUI ) || typeid( *dragItem ) == typeid( ChestUI::ItemUI ) )
    {
       // TradeUI items are handled by packets, so no dropping handling allowed for them.
       if ( GetUI()->GetDragSourceEvent() == &(TradeUI::GetInstance()->tradeGridEvent) ) {
          TradeUI::GetInstance()->EventDroppingItemUIFromTradeToBackpack();
          return false;
       }

		// ChestUI items are handled by packets, so no dropping handling allowed for them.
		if ( GetUI()->GetDragSourceEvent() == &(ChestUI::GetInstance()->chestGridEvent) ) 
      {
			ItemUI *iuiItem = (ItemUI*)dragItem;
			ChestUI *chestUI = ChestUI::GetInstance();

			//ChestUI::GetInstance()->RequestMoveItemFromChestToBackpack( iuiItem->bagItem->dwID, iuiItem->bagItem->dwQty );
			chestUI->qtySelectPopup.Show();
			chestUI->qtySelectPopup.SetState( ChestUI::QtySelectPopupUI::FromChestToBackpack, iuiItem->bagItem->dwQty, iuiItem );
			if( chestUI->qtySelectPopup.IsShown() ){
				chestUI->RequestForegroundControl( &chestUI->qtySelectPopup );
			}

			DropBack(dragItem);

			return false;
		}
        
		return true;
    }


    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool InventoryUI::OtherGridEvent::Dropping( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Checks that the dropped item is an item.
// 
// Return: bool, true or false.
//////////////////////////////////////////////////////////////////////////////////////////
{
    GameUI *dragItem = GetDragItem();

    if( dragItem == NULL ){
        return false;
    }

    // If the dropped item is an ItemUI
    if( typeid( *dragItem ) == typeid( ItemUI ) ){        
        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::TradeButtonEvent::LeftClicked( void )
{
   TradeUI::GetInstance()->Display( NULL );
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::JunkButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Junk button click
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    InventoryUI *invUI = static_cast< InventoryUI * >( GetUI() );

    GameUI *dragItem = GetDragItem();
    if( dragItem == NULL ){
        return;
    }

    // If the dropped item is not an ItemUI
    if( typeid( *dragItem ) != typeid( ItemUI )	){        
        return;
    }

	//If the item comes from the chest, tell the player its not allowed
	if ( GetUI()->GetDragSourceEvent() == &(ChestUI::GetInstance()->chestGridEvent) ) {
        ChatterUI::GetInstance()->AddBackscrollItem( 
            "", 
            g_LocalString[ 560 ], 
            SYSTEM_COLOR,
            false
        );

		return;
	}

    
    ItemUI *itemUI = static_cast< ItemUI * >( dragItem );  

    GameUI *src = GameUI::GetDragParentUI();    
    bool nonGridSource = false;
    if( typeid( *src ) != typeid( GridUI ) ){
        nonGridSource = true;
    }


    DropBack( itemUI );
    
    if( nonGridSource ){
        // Notify the user how we must use this button.
        ChatterUI::GetInstance()->AddBackscrollItem( 
            "", 
            g_LocalString[ 506 ], 
            SYSTEM_COLOR,
            false
        );
        
        naSound->Play( TRUE );
        return;
    }

	if( itemUI->bagItem->dwQty == 0 ){
		return;
	}

    
    invUI->junkDropPopup.Show();
    // Summon the JunkDrop popupUI.    
    invUI->junkDropPopup.SetState( JunkDropPopupUI::Junk, itemUI->bagItem->dwQty, itemUI );
    
    
    if( invUI->junkDropPopup.IsShown() ){
        invUI->RequestForegroundControl( &invUI->junkDropPopup );
    }

}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::DropButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Drop button click.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    InventoryUI *invUI = static_cast< InventoryUI * >( GetUI() );

    GameUI *dragItem = GetDragItem();
    if( dragItem == NULL ){
        return;
    }

    // If the dropped item is not an ItemUI
    if( typeid( *dragItem ) != typeid( ItemUI ) ){        
        return;
    }

	//If the item comes from the chest, tell the player its not allowed
	if ( GetUI()->GetDragSourceEvent() == &(ChestUI::GetInstance()->chestGridEvent) ) {
        ChatterUI::GetInstance()->AddBackscrollItem( 
            "", 
            g_LocalString[ 561 ], 
            SYSTEM_COLOR,
            false
        );
		return;
	}


    ItemUI *itemUI = static_cast< ItemUI * >( dragItem );

    GameUI *src = GameUI::GetDragParentUI();    
    bool nonGridSource = false;
    if( typeid( *src ) != typeid( GridUI ) ){
        nonGridSource = true;
    }
    
    DropBack( itemUI );

    if( nonGridSource ){
        // Notify the user how we must use this button.
        ChatterUI::GetInstance()->AddBackscrollItem( 
            "", 
            g_LocalString[ 505 ], 
            SYSTEM_COLOR,
            false
        );
        
        naSound->Play( TRUE );
        return;
    }

	if( itemUI->bagItem->dwQty == 0 ){
		return;
	}

    // Summon the JunkDrop popupUI.
    invUI->junkDropPopup.Show();
    // Summon the JunkDrop popupUI.    
    invUI->junkDropPopup.SetState( JunkDropPopupUI::Drop, itemUI->bagItem->dwQty, itemUI );
    
    if( invUI->junkDropPopup.IsShown() ){
        invUI->RequestForegroundControl( &invUI->junkDropPopup );
    }    
}

//////////////////////////////////////////////////////////////////////////////////////////
bool InventoryUI::EquipEvent::CanDrag
//////////////////////////////////////////////////////////////////////////////////////////
// Query wether the item can be dragged away from the equipment box. Some items are
// fixed in the equip boxes, such as Seraph wings.
// 
(
 GameUI *idragUI // The item that is about to be dragged.
)
// Return: bool, true if the item can be dragged.
//////////////////////////////////////////////////////////////////////////////////////////
{
    ItemUI *dragUI = static_cast< ItemUI * >( idragUI );
    
/*    if( dragUI->bagItem->Type == __OBJGROUP_SERAPH_BLACK_WINGS ||
        dragUI->bagItem->Type == __OBJGROUP_SERAPH_WHITE_WINGS ){
        return false;
    }
*/
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::EquipEvent::Dropped
//////////////////////////////////////////////////////////////////////////////////////////
// Called when an item was successfully dropped from the equipment box.
// 
(
 EventVisitor *sourceEvent
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    ItemUI *dragUI = static_cast< ItemUI * >( GetDragItem() );
    
    // Must drop an item.
    if( dragUI == NULL ){
        return;
    }

    LOG << "\r\nUnequip item.";    
           
    // Send an unequip packet.
    TFCPacket sending;
    sending << (RQ_SIZE)RQ_UnequipObject;

    bool proceed = true;
    
    InventoryUI *invUI = static_cast< InventoryUI * >( GetUI() );

    DropZoneUI *dropZone = static_cast< DropZoneUI * >( GetDragParentUI() );

    char equipPos = 0;
    
    // Determine from which event came this item.
    if( dropZone == &invUI->bodyDropZone ){        
        equipPos = body;
    }else if( dropZone == &invUI->headDropZone ){
        equipPos = helm;
    }else if( dropZone == &invUI->leftDropZone ){
        equipPos = weapon_left;
    }else if( dropZone == &invUI->rightDropZone ){
        equipPos = weapon_right;
    }else if( dropZone == &invUI->legsDropZone ){
        equipPos = legs;
    }else if( dropZone == &invUI->feetDropZone ){
        equipPos = feet;
    }else if( dropZone == &invUI->capeDropZone ){
        equipPos = sleeves;
    }else if( dropZone == &invUI->glovesDropZone ){
        equipPos = gloves;
    }else if( dropZone == &invUI->neckDropZone ){
        equipPos = necklace;
    }else if( dropZone == &invUI->wristDropZone ){
        equipPos = bracelets;
    }else if( dropZone == &invUI->beltDropZone ){
        equipPos = belt;
    }else if( dropZone == &invUI->ring1DropZone ){
        equipPos = ring1;
    }else if( dropZone == &invUI->ring2DropZone ){
        equipPos = ring2;
    }else{
        proceed = false;
    }

    
    if( proceed ){
        // Add the item to the backpack.
        Player.tlBackpack.Lock( "EquipEvent::Dropped" );
    
        bool found = false;
        // Try to find the item in the backpack.
        Player.tlBackpack.ToHead();
        while( Player.tlBackpack.QueryNext() ){
            if( Player.tlBackpack.GetObject()->associatedUI == dragUI ){
                found = true;
            }
        }
        
        dragUI->bagItem->associatedUI = dragUI;

        // If it wasn't in the backpack.
        if( !found ){
            // Add it.
            Player.tlBackpack.AddToHead( dragUI->bagItem );
        }
        Player.tlBackpack.Unlock( "EquipEvent::Dropped" );

        dropSound->Play( TRUE );

        // Remove the bag item from its equip position.
        Player.lpbEquipped[ equipPos ] = NULL;

        sending << (char)equipPos;

        SEND_PACKET( sending );

        LOG << "\r\nSent unequip packet.";
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool InventoryUI::EquipEvent::Dropping( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Called when an item is dropped in the equipment box.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    bool resetDrag = true;
    
    // If the source event is the equipment event.
    if( GetUI()->GetDragSourceEvent() == this ){
        // Re-sync the equipment box.
        static_cast< InventoryUI * >( GetUI() )->SyncEquipment();
    }else{        
        ItemUI *itemUI = static_cast< ItemUI * >( GetDragItem() );

        bool destroyItem = true;
        
        // If the item is a stack
        if( itemUI->bagItem->dwQty > 1 ){
            
            // Drop the item back to its original position
            if( DropBack( itemUI ) ){
                // Do not destroy the item anymore
                destroyItem = false;
            }
        }
        
        // Retreive the bag item ID.
        DWORD itemID = itemUI->bagItem->dwID;
        
        if( destroyItem ){
            // De-associate the UI from the bag item.
            itemUI->bagItem->associatedUI = NULL;        
            
            // Delete the itemUI.
            delete itemUI;
        }
        
        dropSound->Play( TRUE );

        // Send an equip item packet.
        TFCPacket sending;

        sending << (RQ_SIZE)RQ_EquipObject;
        sending << (long)itemID;
        SEND_PACKET( sending );
    }

    if( resetDrag ){
        // Reset mouse cursor.
        CMouseCursor::GetInstance()->SetCustomCursor( NULL );

        // Cancel drag mode.
        CancelDrag();
    }
    
    // Never let an item be dropped in the equipment box, its handling will
    // be done by the packets.
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::UpdateEquipDropZone
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates one equip position.
// 
(
 int bodyPos,
 DropZoneUI *dropZone
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If there is an equipped item at this position.
    if( Player.lpbEquipped[ bodyPos ] != NULL ){
        // If there is already an item
        if( dropZone->GetItem() != NULL ){
            ItemUI *item = static_cast< ItemUI * >( dropZone->GetItem() );
            // Update its bag item.
            item->bagItem = Player.lpbEquipped[ bodyPos ];
            Player.lpbEquipped[ bodyPos ]->associatedUI = item;
            item->UpdateName();
        }else{
            // Create a new UI.
            ItemUI *newUI = new ItemUI;

            // Set its bag item.
            newUI->bagItem = Player.lpbEquipped[ bodyPos ];

            newUI->bagItem->associatedUI = newUI;

            newUI->SetVSF( Player.lpbEquipped[ bodyPos ]->lpSprite );
            newUI->SetPos( 
                FramePos( 
                    0, 
                    0, 
                    Player.lpbEquipped[ bodyPos ]->lpSprite->GetWidth(),
                    Player.lpbEquipped[ bodyPos ]->lpSprite->GetHeight() 
                )
            );


            newUI->UpdateName();

            // Set the drop zone's item
            dropZone->SetItem( newUI );
        }
    }else{
        // If the drop zone already has the item.
        if( dropZone->GetItem() != NULL ){
            // Reset the drop zone's UI.
            delete dropZone->GetItem();            
            dropZone->SetItem( NULL );            
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::SyncEquipment( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Synchronizes the equipment content.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    LOG << "\r\nSyncEquip1";
    
    // Lock the root box since this call came from some unknown thread.
    CAutoLock theLock( RootBoxUI::GetInstance()->GetLock() );

    LOG << "\r\nSyncEquip2";

    UpdateEquipDropZone( body,          &bodyDropZone );
    UpdateEquipDropZone( feet,          &feetDropZone );
    UpdateEquipDropZone( gloves,        &glovesDropZone );
    UpdateEquipDropZone( helm,          &headDropZone );
	 UpdateEquipDropZone( legs,          &legsDropZone );	
	 UpdateEquipDropZone( bracelets,     &wristDropZone );
	 UpdateEquipDropZone( necklace,      &neckDropZone );
	 UpdateEquipDropZone( weapon_right,  &rightDropZone );
	 UpdateEquipDropZone( weapon_left,   &leftDropZone );
    UpdateEquipDropZone( ring1,         &ring1DropZone );
	 UpdateEquipDropZone( ring2,         &ring2DropZone );	
	 UpdateEquipDropZone( belt,          &beltDropZone );
	 UpdateEquipDropZone( sleeves,       &capeDropZone );

     LOG << "\r\nSyncEquip3";
}

// Import these functions from CharacterUI.
DWORD  GetColor( int stat, int boostedStat, int upg );
string GetStatBoostString( int boost,  int stat );

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::UpdateCharacterSheet( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the character stats.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( RootBoxUI::GetInstance()->GetLock() );
    
    char buf[ 256 ];

    statSTR.SetText( itoa( Player.bStr, buf, 10 ), GetColor( Player.Str, Player.bStr, 0 ) );
    statEND.SetText( itoa( Player.bEnd, buf, 10 ), GetColor( Player.End, Player.bEnd, 0 ) );
    statAGI.SetText( itoa( Player.bAgi, buf, 10 ), GetColor( Player.Agi, Player.bAgi, 0 ) );
    statWIS.SetText( itoa( Player.bWis, buf, 10 ), GetColor( Player.Wis, Player.bWis, 0 ) );
    statINT.SetText( itoa( Player.bInt, buf, 10 ), GetColor( Player.Int, Player.bInt, 0 ) );

    statAC.SetText( itoa( Player.bAC, buf, 10 ), GetColor( Player.AC, Player.bAC, 0 ) );
    statGold.SetText( itoa( Player.Gold, buf, 10 ), RGB( 223, 157, 0 ) );

    FontManager *font = FontManager::GetInstance();
    
    statSTR.GetHelpText()->SetText( GetStatBoostString( Player.bStr, Player.Str ).c_str() );
    statEND.GetHelpText()->SetText( GetStatBoostString( Player.bEnd, Player.End ).c_str() );
    statAGI.GetHelpText()->SetText( GetStatBoostString( Player.bAgi, Player.Agi ).c_str() );
    statWIS.GetHelpText()->SetText( GetStatBoostString( Player.bWis, Player.Wis ).c_str() );
    statINT.GetHelpText()->SetText( GetStatBoostString( Player.bInt, Player.Int ).c_str() );
    statAC.GetHelpText()->SetText(  GetStatBoostString( Player.bAC,  Player.AC ).c_str() );
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::UpdateGold( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the gold on the user.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( RootBoxUI::GetInstance()->GetLock() );
    char buf[ 256 ];
    statGold.SetText( itoa( Player.Gold, buf, 10 ), RGB( 223, 157, 0 ) );
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Saves the backpack into the system's registry.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::SaveBackpack( void )
{

   g_SaveGame.bFreeInv();

   list< GridUI::GridItem > itemList;
   list< GridUI::GridItem >::iterator i;
   inventoryGrid.GetGridItemList( itemList );
   for( i = itemList.begin(); i != itemList.end(); i++ )
   {
      GridUI::GridItem &gridItem = *i;
      ItemUI *itemUI = static_cast< ItemUI * >( gridItem.ui );

      sInventory sNewInv;
      
      sNewInv.dwID         = itemUI->bagItem->wBaseID;
      sNewInv.dwX          = gridItem.gridPos.x;
      sNewInv.dwY          = gridItem.gridPos.y;
      sNewInv.dwType       = itemUI->bagItem->Type;
      g_SaveGame.AddInv(sNewInv);
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Loads the backpack from the system's registry.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::LoadBackpack( void )
{
   int dwNbrInv = g_SaveGame.GetNbrInv();

   set< DWORD > addedIds;
   if(dwNbrInv > 0)
   {
      for(int i =0;i<dwNbrInv; i++ )
      {
         sInventory sNewInv;
         g_SaveGame.GetInv(i,&sNewInv);

         GridUI::GridPos gridPos;
         gridPos.x = sNewInv.dwX;
         gridPos.y = sNewInv.dwY;


         // Create a new UI.
         ItemUI *itemUI = new ItemUI;
         
         LPBAG_ITEM bagItem = new BAG_ITEM;
         
         bagItem->associatedUI = itemUI;
         bagItem->wBaseID      = (WORD)sNewInv.dwID;
         bagItem->dwID         = 0; // Assign a null ID.
         bagItem->Type         = (WORD)sNewInv.dwType;
         bagItem->lpSprite     = InvItemIcons( sNewInv.dwType );
         bagItem->chName[ 0 ] = 0;
         
         itemUI->bagItem = bagItem; // Associate the BAG_ITEM to the UI
         itemUI->Setup(); // Setup the item UI.

         // If the item ID was already added or the grid cannot accept the item.
         if( addedIds.find( bagItem->wBaseID ) != addedIds.end() || !inventoryGrid.AddGridItemAtPos( gridPos, itemUI ) )
         {
            // If the spot wasn't valid, junk this item UI.
            delete itemUI;
            delete bagItem;
         }
         else
         {
            addedIds.insert( bagItem->wBaseID );
            Player.tlBackpack.Lock( "EquipEvent::Load" );
            Player.tlBackpack.AddToTail( bagItem );
            Player.tlBackpack.Unlock( "EquipEvent::Load" );
         }
      }
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::ItemUI::UpdateName( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the name of this item UI
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( strlen( reinterpret_cast< const char * >( bagItem->chName ) ) == 0 )
    {
        return;
    }

    //NMNMNMNMNMNM
  
    char name[ 512 ];
    if( bagItem->dwQty <= 1 )
    {
        if( bagItem->charges > 0 )
        {
            sprintf( name, "%s (x%u)", bagItem->chName, bagItem->charges );
        }
        else if( bagItem->charges < 0 )
        {
            sprintf( name, g_LocalString[ 535 ], bagItem->chName );            
        }
        else
        {
            sprintf( name, "%s", bagItem->chName );
        }
    }
    else
    {
        sprintf( name, "%s x%u", bagItem->chName, bagItem->dwQty );
    }

    LOG << "\r\nUpdating name from " << (char *)bagItem->chName << " to " << name << "..";
    LOG << "qty=" << bagItem->dwQty << "..";


    SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE,reinterpret_cast< const char * >( name ) ) );

}


//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::MacroButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Left clicked on the macro button.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Check which item was brough to the macro box.
    ItemUI *itemUI = static_cast< ItemUI * >( GetDragItem() );
    
    if( itemUI == NULL ){
        // Notify the user how we must use this button.
        ChatterUI::GetInstance()->AddBackscrollItem( 
            "", 
            g_LocalString[ 431 ], 
            SYSTEM_COLOR,
            false
        );

        naSound->Play( TRUE );

        return;
    }
    
    DropBack( itemUI );

    if( !ItemIcons.Exists( itemUI->bagItem->Type ) ){
        naSound->Play( TRUE );
        return;       
    }


    InventoryUI *invUI = static_cast< InventoryUI * >( GetUI() );

    ItemMacroUI *macroUI = new ItemMacroUI;

    macroUI->iconId = itemUI->bagItem->Type;
    macroUI->name   = reinterpret_cast< const char * >( itemUI->bagItem->chName );
    macroUI->baseId = itemUI->bagItem->wBaseID;
    macroUI->SetVSF( ItemIcons( macroUI->iconId ) );
    macroUI->SetPos( FramePos( 0, 0, macroUI->GetVSF()->GetWidth(), macroUI->GetVSF()->GetHeight() ) );

    // Add macro for spell.
    MacroUI::GetInstance()->AddMacro( 
        macroUI, 
        macroUI->name.c_str(), 
        &invUI->itemMacroEvent 
    );
    MacroUI::GetInstance()->SaveMacros();
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::ItemMacroEvent::ExecMacro
//////////////////////////////////////////////////////////////////////////////////////////
//  Executes a spell macro.
// 
(
 MacroDataUI *imacroUI
)
//////////////////////////////////////////////////////////////////////////////////////////
{

    ItemMacroUI *itemMacro = static_cast< ItemMacroUI * >( imacroUI );

    // Check the backpack for items with this requested Id.
    Player.tlBackpack.Lock( "EquipEvent::Macro" );
    Player.tlBackpack.ToHead();
    while( Player.tlBackpack.QueryNext() ){
        LPBAG_ITEM bagItem = Player.tlBackpack.GetObject();

        if( bagItem->wBaseID == itemMacro->baseId ){
            char buf[ 256 ];
            FormatChargesLeft( buf, bagItem );

            ChatterUI::GetInstance()->AddBackscrollItem( 
                "", 
                buf, 
                SYSTEM_COLOR,
                true
            );

            
            // Use the item.
            TFCPacket sending;
            sending << (RQ_SIZE)72;
            sending << (short)itemMacro->baseId;

            // Send the use packet
            SEND_PACKET( sending );
            Player.tlBackpack.Unlock( "EquipEvent::Macro" );

            return;
        }
    }
    Player.tlBackpack.Unlock( "EquipEvent::Macro" );

    naSound->Play( TRUE );

    char buf[ 256 ];
    sprintf( buf, g_LocalString[ 432 ], itemMacro->name.c_str() );

    ChatterUI::GetInstance()->AddBackscrollItem( 
        "", 
        buf, 
        SYSTEM_COLOR,
        true
    );
}

void InventoryUI::ItemMacroEvent::SaveMacro(const list< MacroDataUI * > &macroList)
{
   g_SaveGame.bFreeMacroItem();
   list< MacroDataUI * >::const_iterator i;
   for( i = macroList.begin(); i != macroList.end(); i++ )
   {
      const ItemMacroUI *macroUI = static_cast< const ItemMacroUI * >( *i );
      sMacro sNewMacro;
      
      sNewMacro.dwID       = macroUI->baseId;
      sNewMacro.dwIcon     = macroUI->iconId;
      sNewMacro.dwFastPos  = macroUI->gridPos;
      sNewMacro.dwKey      = macroUI->vKey.AsLong();
      sprintf(sNewMacro.strName,"%s",macroUI->name.c_str());
      g_SaveGame.AddMacroItem(sNewMacro);
   }
}


//////////////////////////////////////////////////////////////////////////////////////////
//  Client initializes the spell UI.
//  Load the spell macros.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::InitializeMacro( void )
{
   int dwNbrItemMacro  = g_SaveGame.GetNbrMItem();
   int dwNbrSkillMacro = g_SaveGame.GetNbrMSkill();
   int dwNbrSpellMacro = g_SaveGame.GetNbrMSpell();


   int dwNbrMacro = dwNbrItemMacro+dwNbrSkillMacro+dwNbrSpellMacro;
      
   if(dwNbrMacro == 0)
      SetDefaultMacro();
   else
   {
      for(int i =0;i<dwNbrItemMacro; i++ )
      {
         ItemMacroUI *macroUI = new ItemMacroUI;
         
         sMacro sNewMacro;
         g_SaveGame.GetMItem(i,&sNewMacro);
         
         macroUI->baseId  = sNewMacro.dwID;
         macroUI->iconId  = sNewMacro.dwIcon;
         macroUI->gridPos = sNewMacro.dwFastPos;
         macroUI->vKey.SetLong(sNewMacro.dwKey);
         macroUI->name = sNewMacro.strName;
         
         macroUI->SetVSF( ItemIcons( macroUI->iconId ) );
         macroUI->SetPos( FramePos( 0, 0, macroUI->GetVSF()->GetWidth(), macroUI->GetVSF()->GetHeight() ) );
         
         if( macroUI->baseId == 0 || macroUI->iconId == 0 )
            delete macroUI;
         else // Add the macro
            MacroUI::GetInstance()->AddMacro(macroUI, macroUI->name, &itemMacroEvent, false);
      }
   }
}


//////////////////////////////////////////////////////////////////////////////////////////
InventoryUI::JunkDropPopupUI::JunkDropPopupUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
: okButton( okButtonEvent ),
  cancelButton( cancelButtonEvent ),
  amountSlider( sliderUpdateQtyEvent )
{
    int dwOffX = (g_Global.GetScreenW()-240)/2;
    int dwOffY = (g_Global.GetScreenH()-164-150)/2;

    SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );

    backPopup.SetSpriteId( "GUI_PopupBack" );
    backPopup.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+164 ) );
    graphAsk.SetSpriteId( "GUI_TextPopupBack" );
    graphAsk.SetPos( FramePos( dwOffX, dwOffY, dwOffX+240, dwOffY+85 ) );
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

    textAreaAsk.SetPos( FramePos( dwOffX+22, dwOffY+23, dwOffX+217, dwOffY+78 ) );
    textAreaAsk.SetCenter( true );
    char strAsk[100];
    sprintf(strAsk,"%s",g_GUILocalString[37]);
    textAreaAsk.SetText( strAsk, RGB( 223, 157, 0 ), 190 );

    textAreaDJ .SetPos( FramePos( dwOffX+22, dwOffY+23, dwOffX+217, dwOffY+47 ) );
    textAreaDJ .SetCenter( true );
  
    amountSlider.SetRange( 0, 0, FramePos( dwOffX+48, dwOffY+57, dwOffX+113, dwOffY+77 ) );
    amountSlider.SetPos( FramePos( dwOffX+29, dwOffY+57, dwOffX+132, dwOffY+77 ) );
    amountSlider.SetScrollRegions(
        FramePos( dwOffX+29, dwOffY+57, dwOffX+48, dwOffY+77 ),
        FramePos( dwOffX+112, dwOffY+57, dwOffX+132, dwOffY+77 ),
        "",""); 

    sliderUpdateQtyEvent.SetUI( this );

    amountEdit.SetPos( FramePos( dwOffX+145, dwOffY+57, dwOffX+219, dwOffY+80 ) );
    amountEdit.SetFilter( &amountEditFilter );
    amountEdit.SetMaxTextLength( 9 );
    amountEdit.EnableCursor( true );
    SetHelpId( amountEdit, 451 );


    AddChild( &ackGraphPatch );
    AddChild( &graphAsk );
    AddChild( &graphDJ);
    AddChild( &okButton );
    AddChild( &cancelButton );
    AddChild( &amountSlider );
    AddChild( &amountEdit );
    AddChild( &textAreaDJ );
    AddChild( &textAreaAsk );
}

void InventoryUI::JunkDropPopupUI::Draw(V2SPRITEFX *vsfFX)
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

int InventoryUI::JunkDropPopupUI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}




bool InventoryUI::JunkDropPopupUI::VKeyInput(VKey ivkey) {
	if (ivkey == VK_RETURN) {
		okButtonEvent.LeftClicked();
	} else if (ivkey == VK_ESCAPE) {
		cancelButtonEvent.LeftClicked();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::JunkDropPopupUI::SetState
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
    
    amountSlider.SetRange( 0, maxQty, FramePos( dwOffX+48, dwOffY+57, dwOffX+108, dwOffY+77) );
    amountSlider.SetSliderPos( 0 );

    switch( state ) 
    {
      case Junk:
         graphAsk.Hide();
         textAreaAsk.Hide();
         amountSlider.Show();
         amountEdit.Show();

         graphDJ.Show();
         textAreaDJ.SetText( g_GUILocalString[36], RGB( 223, 157, 0 ), 190 );
         textAreaDJ.Show();
         amountEdit.SetText( "0" );
      break;
      case Drop:    
         graphAsk.Hide();
         textAreaAsk.Hide();
         amountSlider.Show();
         amountEdit.Show();

         graphDJ.Show();
         textAreaDJ.SetText( g_GUILocalString[38], RGB( 223, 157, 0 ), 190 );
         textAreaDJ.Show();
         amountEdit.SetText( "0" );
         
         // Automatically click OK of the quantity is 1.
         if( maxQty == 1 )
         {
            amountEdit.SetText( "1" );
            okButtonEvent.LeftClicked();
            
            Hide();
            return;
         }
         amountEdit.SetText( "0" );
      break;
      case JunkAck:
      {
         graphAsk.Show();
         textAreaAsk.Show();
         
         graphDJ.Hide();
         textAreaDJ.Hide();
         amountSlider.Hide();
         amountEdit.Hide();
      }
      break;
    };
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::JunkDropPopupUI::OkButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  OK
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    JunkDropPopupUI *popupUI = static_cast< InventoryUI::JunkDropPopupUI * >( GetUI() );
    
    switch( popupUI->state ){
    // User wants to junk the items
    case Junk:{
        LOG << "\r\nJUNK OK SETTING JUNK ACK";
        // Ask the user
        popupUI->SetState( JunkAck, popupUI->itemUI->bagItem->dwQty, popupUI->itemUI );
    }break;
    // User wants to drop the items.
    case Drop:{
        TFCPacket sending;

        DWORD qty = atoi( popupUI->amountEdit.GetText().c_str() );

        // If the quantity is nil
        if( qty == 0 ){
            popupUI->Hide();
            return;
        }

        // If the player put too much items.
        if( qty > popupUI->itemUI->bagItem->dwQty ){
            qty = popupUI->itemUI->bagItem->dwQty;
        }

        // Fill in a drop packet.
        sending << (RQ_SIZE)RQ_DepositObject;
        sending << (short)Player.xPos;        
        sending << (short)Player.yPos;
        sending << (long)popupUI->itemUI->bagItem->dwID;
        sending << (long)qty;

        // Send the packet.
        SEND_PACKET( sending );
        popupUI->Hide();
    }break;
    // User agrees to junk the items.
    case JunkAck:{
        //const RQ_JunkItems = 85;
    
        TFCPacket sending;

        DWORD qty = atoi( popupUI->amountEdit.GetText().c_str() );

        // If the quantity is nil
        if( qty == 0 ){
            popupUI->Hide();
            return;
        }

        // If the player put too much items.
        if( qty > popupUI->itemUI->bagItem->dwQty ){
            qty = popupUI->itemUI->bagItem->dwQty;
        }

        // Fill in a drop packet.
        sending << (RQ_SIZE)RQ_JunkItems;
        sending << (long)popupUI->itemUI->bagItem->dwID;
        sending << (long)qty;

        // Send the packet.
        SEND_PACKET( sending );
        popupUI->Hide();
    }break;
    }

    
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::JunkDropPopupUI::CancelButtonEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  CANCEL
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    JunkDropPopupUI *popupUI = static_cast< InventoryUI::JunkDropPopupUI * >( GetUI() );
    
    // Quit
    popupUI->Hide();    
}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::JunkDropPopupUI::SliderUpdateQtyEvent::LeftClicked( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Clicked on the slider, update the displayed amount.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    JunkDropPopupUI *popupUI = static_cast< InventoryUI::JunkDropPopupUI * >( GetUI() );

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
bool InventoryUI::JunkDropPopupUI::AmountEditFilter::IsOK
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

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::InventoryGridEvent::Dragged( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Dragged something from the inventory grid.
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
void InventoryUI::EquipEvent::Dragged( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Dragged something from the equip dropzone
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
void InventoryUI::InventoryGridEvent::Dropped
//////////////////////////////////////////////////////////////////////////////////////////
//  Dropped something in the inventory grid
// 
(
 EventVisitor *sourceEvent // Unused.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    dropSound->Play( TRUE ); 

}

//////////////////////////////////////////////////////////////////////////////////////////
void InventoryUI::SetDefaultMacro( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Reset the Macro.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
        // Add the default hardcoded macros.
        // Provided for backward compatibility with the original F2 to F4 macros.
        ItemMacroUI *macroUI = new ItemMacroUI;

        // Light healing potions at F2
        macroUI->vKey.SetKey( VK_F2 );
        macroUI->iconId = 241;
        macroUI->name   = g_LocalString[ 434 ];
        macroUI->baseId = 40623;
        macroUI->SetVSF( ItemIcons( macroUI->iconId ) );
        macroUI->SetPos( FramePos( 0, 0, macroUI->GetVSF()->GetWidth(), macroUI->GetVSF()->GetHeight() ) );

        MacroUI::GetInstance()->AddMacro( 
            macroUI, 
            macroUI->name.c_str(), 
            &itemMacroEvent,
            false
        );

        // Mana potion at F3
        macroUI = new ItemMacroUI;
        macroUI->vKey.SetKey( VK_F3 );
        macroUI->iconId = 240;
        macroUI->name   = g_LocalString[ 436 ];
        macroUI->baseId = 40004;
        macroUI->SetVSF( ItemIcons( macroUI->iconId ) );
        macroUI->SetPos( FramePos( 0, 0, macroUI->GetVSF()->GetWidth(), macroUI->GetVSF()->GetHeight() ) );

        MacroUI::GetInstance()->AddMacro( 
            macroUI, 
            macroUI->name.c_str(), 
            &itemMacroEvent,
            false
        );

        // Torch at F4
        macroUI = new ItemMacroUI;
        macroUI->vKey.SetKey( VK_F4 );
        macroUI->iconId = 244;
        macroUI->name   = g_LocalString[ 435 ];
        macroUI->baseId = 40015;
        macroUI->SetVSF( ItemIcons( macroUI->iconId ) );
        macroUI->SetPos( FramePos( 0, 0, macroUI->GetVSF()->GetWidth(), macroUI->GetVSF()->GetHeight() ) );

        MacroUI::GetInstance()->AddMacro( 
            macroUI, 
            macroUI->name.c_str(), 
            &itemMacroEvent,
            false
        );
}



