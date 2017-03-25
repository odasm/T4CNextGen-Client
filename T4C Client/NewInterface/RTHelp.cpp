// RTHelp.cpp: implementation of the RTHelp class.
//
//////////////////////////////////////////////////////////////////////
#include "../pch.h"

#include "RTHelp.h"
#include "RootBoxUI.h"
#include "ChatterUI.h"
#include "CharacterUI.h"
#include "InventoryUI.h"
#include "SpellUI.h"
#include "GroupPlayUI.h"
#include "SideMenu.h"
#include "OptionsUI.h"
#include "chestUI.h"
#include "RTMap.h"
#include "RTHelp.h"
#include "RobUI.h"
#include "SkillTeachUI.h"
#include "SkillTrainUI.h"
#include "TradeUI.h"
#include "..\GUILocalString.h"  //g_GUILocalString[]

#include "..\SaveGame.h"
extern Font *fNewGuiBtnFont;
extern CSaveGame g_SaveGame;


#define RT_VIEW_X  640
#define RT_VIEW_Y  480
#define RT_VIEW_X2 (RT_VIEW_X/2)
#define RT_VIEW_Y2 (RT_VIEW_Y/2)
#define RT_VIEW_X4 (RT_VIEW_X/4)
#define RT_VIEW_Y4 (RT_VIEW_Y/4)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RTHelp::RTHelp():
prevButton( prevButtonEvent ),
nextButton( nextButtonEvent ),
X_Button( hideEvent )
{
   // Setup the chatter channel
   int dwX = (g_Global.GetScreenW()-RT_VIEW_X)/2;
   int dwY = (g_Global.GetScreenH()-150-RT_VIEW_Y)/2;
   if(dwY < 0)
      dwY = 0;

   SetPos( FramePos(dwX, dwY, dwX+RT_VIEW_X, dwY+RT_VIEW_Y) );
   RootBoxUI::GetInstance()->AddChild( this );


   char strHelpName[100];
   for(int i=0;i<NBR_HELP_PAGE;i++)
   {
      sprintf(strHelpName,"%sHelp%d",Player.szLanguage,i); 
      m_HelpPage[i]  .SetSpriteId(strHelpName);
      m_HelpPage[i]  .SetPos( FramePos(dwX, dwY, dwX+RT_VIEW_X, dwY+RT_VIEW_Y) );
      m_HelpPage[i]  .DrawAlpha(true);
   }

   for(int i=0;i<NBR_LETTER_PAGE;i++)
   {
      sprintf(strHelpName,"%sLetter%d",Player.szLanguage,i); 
      m_LetterPage[i]  .SetSpriteId(strHelpName);
      m_LetterPage[i]  .SetPos( FramePos(dwX, dwY, dwX+RT_VIEW_X, dwY+RT_VIEW_Y) );
      m_LetterPage[i]  .DrawAlpha(true);
   }
   for(int i=0;i<NBR_MAP_PAGE;i++)
   {
      sprintf(strHelpName,"Map_World%d",i); 
      m_MapPage[i]  .SetSpriteId(strHelpName);
      m_MapPage[i]  .SetPos( FramePos(dwX, dwY, dwX+RT_VIEW_X, dwY+RT_VIEW_Y) );
      m_MapPage[i]  .DrawAlpha(true);
   }

   sprintf(strHelpName,"LabyMap"); 
   m_LabyPage  .SetSpriteId(strHelpName);
   m_LabyPage  .SetPos( FramePos(dwX, dwY, dwX+RT_VIEW_X, dwY+RT_VIEW_Y) );
   m_LabyPage  .DrawAlpha(true);

   sprintf(strHelpName,"LabyMap"); 
   m_BDImage  .SetSpriteId(strHelpName);
   m_BDImage  .SetPos( FramePos(dwX, dwY, dwX+RT_VIEW_X, dwY+RT_VIEW_Y) );
   m_BDImage  .DrawAlpha(true);

   
   

   X_Button.SetPos        ( FramePos( dwX+640-28, dwY+0, dwX+640-4, dwY+0+24 ) );
   X_Button.SetSpriteId   ( "GUI_X_ButtonUp" );
   X_Button.SetDownUI     ( "GUI_X_ButtonDown" );
   X_Button.SetHighlightUI( "GUI_X_ButtonHUp" );
   SetHelpId( X_Button, 261 );


   prevButton.SetPos( FramePos( dwX+10, dwY+425, dwX+10+80, dwY+425+22 ) );
   prevButton.SetSpriteId   ( "BtnHelpPrevUp" );
   prevButton.SetDownUI     ( "BtnHelpPrevUp" );
   prevButton.SetHighlightUI( "BtnHelpPrevHUp" );
   prevButtonEvent.SetUI( this );
   //SetHelpId( prevButton, 384 );
   
   nextButton.SetPos( FramePos( dwX+549, dwY+425, dwX+549+80, dwY+425+22 ) );
   nextButton.SetSpriteId   ( "BtnHelpNextUp" );
   nextButton.SetDownUI     ( "BtnHelpNextUp" );
   nextButton.SetHighlightUI( "BtnHelpNextHUp" );
   nextButtonEvent.SetUI( this );
   //SetHelpId( nextButton, 385 );

   
   for(int i=0;i<NBR_HELP_PAGE;i++)
   {
      AddChild( &m_HelpPage[i]);
      if(i>0)
         m_HelpPage[i].Hide();
   }
   for(int i=0;i<NBR_LETTER_PAGE;i++)
   {
      AddChild( &m_LetterPage[i]);
      m_LetterPage[i].Hide();
   }
   for(int i=0;i<NBR_MAP_PAGE;i++)
   {
      AddChild( &m_MapPage[i]);
      m_MapPage[i].Hide();
   }
   AddChild( &m_LabyPage);
   m_LabyPage.Hide();

   AddChild( &m_BDImage);
   m_BDImage.Hide();
   
   
   AddChild( &prevButton );
   AddChild( &nextButton );
   AddChild( &X_Button );

   m_dwCurrentPage = 0;

   Hide();
}

RTHelp::~RTHelp()
{
}

//////////////////////////////////////////////////////////////////////////////////////////
RTHelp *RTHelp::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the side menu instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static RTHelp instance;
    return &instance;
}
 
//////////////////////////////////////////////////////////////////////////////////////////
void RTHelp::Draw(V2SPRITEFX *vsfFX)
{
   int dwX = (g_Global.GetScreenW()-RT_VIEW_X)/2;
   int dwY = (g_Global.GetScreenH()-150-RT_VIEW_Y)/2;
   if(dwY < 0)
      dwY = 0;
   
   if(m_dwShowWhat == 0)
   {
      SetHelpPageName();
      if(m_dwCurrentPage==0)
      {
         if(prevButton.IsShown())
            prevButton.Hide();
         if(!nextButton.IsShown())
            nextButton.Show();
      }
      else if(m_dwCurrentPage==NBR_HELP_PAGE-1)
      {
         if(nextButton.IsShown())
            nextButton.Hide();
         if(!prevButton.IsShown())
            prevButton.Show();
      }
      else
      {
         if(!nextButton.IsShown())
            nextButton.Show();
         if(!prevButton.IsShown())
            prevButton.Show();
      }
   }
   else if(m_dwShowWhat == 1)
   {
      if(prevButton.IsShown())
         prevButton.Hide();
      if(nextButton.IsShown())
         nextButton.Hide();
   }
   else if(m_dwShowWhat == 2)
   {
      if(m_dwCurrentPage==0)
      {
         if(prevButton.IsShown())
            prevButton.Hide();
         if(!nextButton.IsShown())
            nextButton.Show();
      }
      else if(m_dwCurrentPage==NBR_MAP_PAGE-1)
      {
         if(nextButton.IsShown())
            nextButton.Hide();
         if(!prevButton.IsShown())
            prevButton.Show();
      }
      else
      {
         if(!nextButton.IsShown())
            nextButton.Show();
         if(!prevButton.IsShown())
            prevButton.Show();
      }
   }
   else if(m_dwShowWhat == 4)
   {
      if(nextButton.IsShown())
         nextButton.Hide();
      if(prevButton.IsShown())
         prevButton.Hide();
   }


   GameUI::Draw(vsfFX);

   if(m_strHeaderText[0] != 0x00)
   {
      HDC hdc;
      DXDGetDC(&hdc, 745);
      // Draw les texte...
      char strBtnTexte[50];
      // Draw Box Header
      sprintf(strBtnTexte  ,"%s",m_strHeaderText);
      fNewGuiBtnFont->DrawFont(hdc, dwX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,212)+ 214+1, dwY+0+1 , RGB(0, 0, 0)    , 0, strBtnTexte);
      fNewGuiBtnFont->DrawFont(hdc, dwX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,212)+ 214  , dwY+0   , g_Global.GetBtnTCE(), 0, strBtnTexte);
      DXDReleaseDC(hdc, 745);
   }
   

   if( CharacterUI ::GetInstance()->IsShown()     ||
       InventoryUI ::GetInstance()->IsShown()     ||
       SpellUI     ::GetInstance()->IsShown()     ||
       (GroupPlayUI ::GetInstance()->IsShown()     && !GroupPlayUI::GetInstance()->IsMinimized() )||
       !ChatterUI   ::GetInstance()->IsMinimized()||
       (MacroUI     ::GetInstance()->IsShown()     && !MacroUI::GetInstance()->IsMinimized())||
       OptionsUI   ::GetInstance()->IsShown()     ||
       ChestUI     ::GetInstance()->IsShown()     ||
       RobUI       ::GetInstance()->IsShown()     ||
       RTMap       ::GetInstance()->IsShown()     ||
       SpellUI     ::GetInstance()->IsShown()     ||
       SkillTeachUI::GetInstance()->IsShown()     ||
       SkillTrainUI::GetInstance()->IsShown()     ||
       TradeUI     ::GetInstance()->IsShown()        )
   {
      Hide();
   }
   
}

int RTHelp::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

//////////////////////////////////////////////////////////////////////////////////////////
void RTHelp::PrevButtonEvent::LeftClicked( void )
{
    RTHelp *ui = static_cast< RTHelp * >( GetUI() );

    if(ui->m_dwShowWhat== 0)
    {
       ui->m_HelpPage[ui->m_dwCurrentPage].Hide();
       ui->m_dwCurrentPage--;
       if(ui->m_dwCurrentPage <0)
          ui->m_dwCurrentPage = 0;
       ui->m_HelpPage[ui->m_dwCurrentPage].Show();
    }
    else if(ui->m_dwShowWhat== 2)
    {
       ui->m_MapPage[ui->m_dwCurrentPage].Hide();
       ui->m_dwCurrentPage--;
       if(ui->m_dwCurrentPage <0)
          ui->m_dwCurrentPage = 0;
       ui->m_MapPage[ui->m_dwCurrentPage].Show();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void RTHelp::NextButtonEvent::LeftClicked( void )
{
    RTHelp *ui = static_cast< RTHelp * >( GetUI() );

    if(ui->m_dwShowWhat== 0)
    {
       ui->m_HelpPage[ui->m_dwCurrentPage].Hide();
       ui->m_dwCurrentPage++;
       if(ui->m_dwCurrentPage >= NBR_HELP_PAGE)
          ui->m_dwCurrentPage = NBR_HELP_PAGE-1;
       ui->m_HelpPage[ui->m_dwCurrentPage].Show();
    }
    else if(ui->m_dwShowWhat== 2)
    {
       ui->m_MapPage[ui->m_dwCurrentPage].Hide();
       ui->m_dwCurrentPage++;
       if(ui->m_dwCurrentPage >= NBR_HELP_PAGE)
          ui->m_dwCurrentPage = NBR_HELP_PAGE-1;
       ui->m_MapPage[ui->m_dwCurrentPage].Show();
    }
} 

void RTHelp::Show(bool putForeground,int dwShowWhat,int dwValDef)
{
   if( CharacterUI ::GetInstance()->IsShown()     ||
       InventoryUI ::GetInstance()->IsShown()     ||
       SpellUI     ::GetInstance()->IsShown()     ||
       (GroupPlayUI ::GetInstance()->IsShown()     && !GroupPlayUI::GetInstance()->IsMinimized() )||
       !ChatterUI   ::GetInstance()->IsMinimized()||
       (MacroUI     ::GetInstance()->IsShown()     && !MacroUI::GetInstance()->IsMinimized())||
       OptionsUI   ::GetInstance()->IsShown()     ||
       ChestUI     ::GetInstance()->IsShown()     ||
       RobUI       ::GetInstance()->IsShown()     ||
       RTMap       ::GetInstance()->IsShown()     ||
       SpellUI     ::GetInstance()->IsShown()     ||
       SkillTeachUI::GetInstance()->IsShown()     ||
       SkillTrainUI::GetInstance()->IsShown()     ||
       TradeUI     ::GetInstance()->IsShown()        )
   {
      CharacterUI ::GetInstance()->Hide();
      InventoryUI ::GetInstance()->Hide();
      SpellUI     ::GetInstance()->Hide();
      GroupPlayUI ::GetInstance()->Hide();
      ChatterUI   ::GetInstance()->Hide();
      MacroUI     ::GetInstance()->Hide();
      OptionsUI   ::GetInstance()->Hide();
      ChestUI     ::GetInstance()->Hide();
      RobUI       ::GetInstance()->Hide();
      RTMap       ::GetInstance()->Hide();
      SpellUI     ::GetInstance()->Hide();
      SkillTeachUI::GetInstance()->Hide();
      SkillTrainUI::GetInstance()->Hide();
      TradeUI     ::GetInstance()->Hide();
      
   }
   for(int i=0;i<NBR_HELP_PAGE;i++)
      m_HelpPage[i].Hide();
   for(int i=0;i<NBR_LETTER_PAGE;i++)
      m_LetterPage[i].Hide();
   for(int i=0;i<NBR_MAP_PAGE;i++)
      m_MapPage[i].Hide();
   m_LabyPage.Hide();
   m_BDImage.Hide();

   m_dwShowWhat = dwShowWhat;
	if(m_dwShowWhat == 0) //Show HELP
   {
      m_dwCurrentPage = dwValDef;
      m_HelpPage[m_dwCurrentPage].Show();
      if(dwValDef == 0)
      {
         nextButton.Show();
         prevButton.Hide();
      }
      else
      {
         nextButton.Show();
         prevButton.Show();
      }
   }
   else if(m_dwShowWhat == 1)
   {
      sprintf(m_strHeaderText,g_GUILocalString[100]);
      m_LetterPage[dwValDef].Show();
   }
   else if(m_dwShowWhat == 2)
   {
      sprintf(m_strHeaderText,g_GUILocalString[101]);
      m_dwCurrentPage = dwValDef;
      m_MapPage[m_dwCurrentPage].Show();
      if(dwValDef == 0)
      {
         nextButton.Show();
         prevButton.Hide();
      }
      else
      {
         nextButton.Show();
         prevButton.Show();
      }
   }
   else if(m_dwShowWhat == 3)
   {
      sprintf(m_strHeaderText,"Carte du labyrinthe");
      m_LabyPage.Show();
      
   }
   
	BoxUI::Show(putForeground);
}

void RTHelp::ShowSpecial(BYTE* pData, BYTE* pPal, int dwW, int dwH,char* pStrName)
{
   if( CharacterUI ::GetInstance()->IsShown()     ||
       InventoryUI ::GetInstance()->IsShown()     ||
       SpellUI     ::GetInstance()->IsShown()     ||
       (GroupPlayUI ::GetInstance()->IsShown()     && !GroupPlayUI::GetInstance()->IsMinimized() )||
       !ChatterUI   ::GetInstance()->IsMinimized()||
       (MacroUI     ::GetInstance()->IsShown()     && !MacroUI::GetInstance()->IsMinimized())||
       OptionsUI   ::GetInstance()->IsShown()     ||
       ChestUI     ::GetInstance()->IsShown()     ||
       RobUI       ::GetInstance()->IsShown()     ||
       RTMap       ::GetInstance()->IsShown()     ||
       SpellUI     ::GetInstance()->IsShown()     ||
       SkillTeachUI::GetInstance()->IsShown()     ||
       SkillTrainUI::GetInstance()->IsShown()     ||
       TradeUI     ::GetInstance()->IsShown()        )
   {
      CharacterUI ::GetInstance()->Hide();
      InventoryUI ::GetInstance()->Hide();
      SpellUI     ::GetInstance()->Hide();
      GroupPlayUI ::GetInstance()->Hide();
      ChatterUI   ::GetInstance()->Hide();
      MacroUI     ::GetInstance()->Hide();
      OptionsUI   ::GetInstance()->Hide();
      ChestUI     ::GetInstance()->Hide();
      RobUI       ::GetInstance()->Hide();
      RTMap       ::GetInstance()->Hide();
      SpellUI     ::GetInstance()->Hide();
      SkillTeachUI::GetInstance()->Hide();
      SkillTrainUI::GetInstance()->Hide();
      TradeUI     ::GetInstance()->Hide();
      
   }
   for(int i=0;i<NBR_HELP_PAGE;i++)
      m_HelpPage[i].Hide();
   for(int i=0;i<NBR_LETTER_PAGE;i++)
      m_LetterPage[i].Hide();
   for(int i=0;i<NBR_MAP_PAGE;i++)
      m_MapPage[i].Hide();
   m_LabyPage.Hide();
   m_BDImage.Hide();
 
   nextButton.Hide();
   prevButton.Hide();

   m_dwShowWhat = 4;


   //int dwX = (g_Global.GetScreenW()-RT_VIEW_X)/2;
   //int dwY = (g_Global.GetScreenH()-150-RT_VIEW_Y)/2;
   //if(dwY < 0)
   //   dwY = 0;

   sprintf(m_strHeaderText,"%s",pStrName);

   m_BDImage.ShowLoad(pData,pPal,dwW,dwH);

   BoxUI::Show(true);
}

void RTHelp::SetHelpPageName()
{
   m_strHeaderText[0] = 0x00;
   switch(m_dwCurrentPage)
   {
      case 1 : sprintf(m_strHeaderText,g_GUILocalString[102]); break;
      case 2 : sprintf(m_strHeaderText,g_GUILocalString[103]); break;
      case 3 : sprintf(m_strHeaderText,g_GUILocalString[104]); break;
      case 4 : sprintf(m_strHeaderText,g_GUILocalString[105]); break;
      case 5 : sprintf(m_strHeaderText,g_GUILocalString[106]); break;
      case 6 : sprintf(m_strHeaderText,g_GUILocalString[107]); break;
      case 7 : sprintf(m_strHeaderText,g_GUILocalString[108]); break;
      case 8 : sprintf(m_strHeaderText,g_GUILocalString[109]); break;
      case 9 : sprintf(m_strHeaderText,g_GUILocalString[110]); break;
      case 10: sprintf(m_strHeaderText,g_GUILocalString[111]); break;
      case 11: sprintf(m_strHeaderText,g_GUILocalString[112]); break;
   }
}


