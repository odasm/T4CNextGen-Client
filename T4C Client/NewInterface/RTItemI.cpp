// RTItemI.cpp: implementation of the RTItemI class.
//
//////////////////////////////////////////////////////////////////////
#include "../pch.h"

#include "RTItemI.h"
#include "RootBoxUI.h"
#include "InventoryUI.h"

#include "..\GUILocalString.h"  //g_GUILocalString[]
#include "..\SaveGame.h"

extern Font *fNewGuiBtnFont;
extern Font *fNewGuiInfoFont;
extern Font *fNewGuiInfoFont2;
extern CSaveGame g_SaveGame;


#define STAT_SKILL_INVALID		0
#define STAT_INT			1
#define STAT_END			2
#define STAT_STR			3
#define STAT_WIS			4
#define STAT_AGI			6
#define STAT_ATTACK			8
#define STAT_DODGE			9
#define STAT_DAMAGE			10
#define STAT_RADIANCE       		11
#define STAT_AIR_RESIST     		12
#define STAT_FIRE_RESIST    		13
#define STAT_WATER_RESIST   		14
#define STAT_EARTH_RESIST   		15
#define STAT_AIR_POWER      		16
#define STAT_FIRE_POWER     		17
#define STAT_WATER_POWER    		18
#define STAT_EARTH_POWER    		19
#define STAT_AC             		20
#define STAT_LIGHT_RESIST   		21
#define STAT_DARK_RESIST    		22
#define STAT_LIGHT_POWER    		23
#define STAT_DARK_POWER     		24
#define STAT_MAX_HP         		25
#define STAT_MAX_MANA       		26
#define SKILL_STUN_BLOW     		27
#define SKILL_POWERUL_BLOW		28
#define SKILL_FIRST_AID			29
#define SKILL_PARRY			30
#define SKILL_MEDITATE			31
#define SKILL_DODGE			32
#define SKILL_ATTACK			33
#define SKILL_HIDE			34
#define SKILL_ROB			35
#define SKILL_SNEAK			36
#define SKILL_SEARCH			37
#define SKILL_PICKLOCK			38
#define SKILL_ARMOR_PENETRATION		39
#define SKILL_PEEK			40
#define SKILL_RAPID_HEALING		41
#define SKILL_ARCHERY			42
#define SKILL_DUALWEAPONS			43
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RTItemI::RTItemI():
cancelButton( cancelButtonEvent )
{
   sprintf(m_strBoustName[STAT_SKILL_INVALID       ],"%s",g_GUILocalString[175]);
   sprintf(m_strBoustName[STAT_INT	      		   ],"%s",g_GUILocalString[176]);
   sprintf(m_strBoustName[STAT_END	        		   ],"%s",g_GUILocalString[177]);
   sprintf(m_strBoustName[STAT_STR			         ],"%s",g_GUILocalString[178]);
   sprintf(m_strBoustName[STAT_WIS	      	   	],"%s",g_GUILocalString[179]);
   sprintf(m_strBoustName[STAT_AGI	      	   	],"%s",g_GUILocalString[180]);
   sprintf(m_strBoustName[STAT_ATTACK              ],"%s",g_GUILocalString[181]);
   sprintf(m_strBoustName[STAT_DODGE			      ],"%s",g_GUILocalString[182]);
   sprintf(m_strBoustName[STAT_DAMAGE			      ],"%s",g_GUILocalString[183]);
   sprintf(m_strBoustName[STAT_RADIANCE         	],"%s",g_GUILocalString[184]);
   sprintf(m_strBoustName[STAT_AIR_RESIST       	],"%s",g_GUILocalString[185]);
   sprintf(m_strBoustName[STAT_FIRE_RESIST      	],"%s",g_GUILocalString[186]);
   sprintf(m_strBoustName[STAT_WATER_RESIST     	],"%s",g_GUILocalString[187]);
   sprintf(m_strBoustName[STAT_EARTH_RESIST     	],"%s",g_GUILocalString[188]);
   sprintf(m_strBoustName[STAT_AIR_POWER        	],"%s",g_GUILocalString[189]);
   sprintf(m_strBoustName[STAT_FIRE_POWER       	],"%s",g_GUILocalString[190]);
   sprintf(m_strBoustName[STAT_WATER_POWER      	],"%s",g_GUILocalString[191]);
   sprintf(m_strBoustName[STAT_EARTH_POWER      	],"%s",g_GUILocalString[192]);
   sprintf(m_strBoustName[STAT_AC               	],"%s",g_GUILocalString[193]);
   sprintf(m_strBoustName[STAT_LIGHT_RESIST     	],"%s",g_GUILocalString[194]);
   sprintf(m_strBoustName[STAT_DARK_RESIST      	],"%s",g_GUILocalString[195]);
   sprintf(m_strBoustName[STAT_LIGHT_POWER      	],"%s",g_GUILocalString[196]);
   sprintf(m_strBoustName[STAT_DARK_POWER       	],"%s",g_GUILocalString[197]);
   sprintf(m_strBoustName[STAT_MAX_HP           	],"%s",g_GUILocalString[198]);
   sprintf(m_strBoustName[STAT_MAX_MANA         	],"%s",g_GUILocalString[199]);
   sprintf(m_strBoustName[SKILL_STUN_BLOW       	],"%s",g_GUILocalString[200]);
   sprintf(m_strBoustName[SKILL_POWERUL_BLOW	   	],"%s",g_GUILocalString[201]);
   sprintf(m_strBoustName[SKILL_FIRST_AID		   	],"%s",g_GUILocalString[202]);
   sprintf(m_strBoustName[SKILL_PARRY			      ],"%s",g_GUILocalString[203]);
   sprintf(m_strBoustName[SKILL_MEDITATE		   	],"%s",g_GUILocalString[204]);
   sprintf(m_strBoustName[SKILL_DODGE			      ],"%s",g_GUILocalString[205]);
   sprintf(m_strBoustName[SKILL_ATTACK			      ],"%s",g_GUILocalString[206]);
   sprintf(m_strBoustName[SKILL_HIDE			      ],"%s",g_GUILocalString[207]);
   sprintf(m_strBoustName[SKILL_ROB			         ],"%s",g_GUILocalString[208]);
   sprintf(m_strBoustName[SKILL_SNEAK			      ],"%s",g_GUILocalString[209]);
   sprintf(m_strBoustName[SKILL_SEARCH			      ],"%s",g_GUILocalString[210]);
   sprintf(m_strBoustName[SKILL_PICKLOCK		   	],"%s",g_GUILocalString[211]);
   sprintf(m_strBoustName[SKILL_ARMOR_PENETRATION  ],"%s",g_GUILocalString[212]);
   sprintf(m_strBoustName[SKILL_PEEK			      ],"%s",g_GUILocalString[213]);
   sprintf(m_strBoustName[SKILL_RAPID_HEALING	   ],"%s",g_GUILocalString[214]);
   sprintf(m_strBoustName[SKILL_ARCHERY			   ],"%s",g_GUILocalString[215]);
   sprintf(m_strBoustName[SKILL_DUALWEAPONS  	   ],"%s",g_GUILocalString[216]);
   

   int dwOffX = (g_Global.GetScreenW()-220)/2;
   int dwOffY = (g_Global.GetScreenH()-340-150)/2;
   SetPos( FramePos( dwOffX, dwOffY, dwOffX+220, dwOffY+340 ) );
   RootBoxUI::GetInstance()->AddChild( this );     
   
   
   backPopup.SetSpriteId( "ItemInfoBack" );
   backPopup.SetPos( FramePos( dwOffX, dwOffY, dwOffX+220, dwOffY+340 ) );
   
   
   cancelButton.SetSpriteId   ( "GUI_ButtonTUp" );
   cancelButton.SetDownUI     ( "GUI_ButtonTDown" );
   cancelButton.SetHighlightUI( "GUI_ButtonTHUp" );
   cancelButton.SetPos( FramePos( dwOffX+80, dwOffY+307, dwOffX+80+60, dwOffY+307+22 ) );
   cancelButtonEvent.SetUI( this );
   //SetHelpId( cancelButton, 453 );
   
   AddChild( &cancelButton );

   

   Hide();
}

RTItemI::~RTItemI()
{
   
}

//////////////////////////////////////////////////////////////////////////////////////////
RTItemI *RTItemI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the side menu instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static RTItemI instance;
    return &instance;
}
 
//////////////////////////////////////////////////////////////////////////////////////////
void RTItemI::Draw(V2SPRITEFX *vsfFX)
{
   int dwOffX = (g_Global.GetScreenW()-220)/2;
   int dwOffY = (g_Global.GetScreenH()-340-150)/2;

   g_Global.Lock();
   if(g_Global.IsItemInfoAvailable())
   {
      sItemInfo *pInfo = g_Global.GetItemInfo();
      char strtmp[100];

      UINT dwTypeItem = 1;
      COLORREF crName;
      if(pInfo->shAC     == 0 && pInfo->shDodge == 0 && pInfo->shEnd == 0 && pInfo->lMinDamage == 0 && pInfo->lMaxDamage == 0 && 
         pInfo->shAttack == 0 && pInfo->shStr   == 0 && pInfo->shAgi == 0 && pInfo->shWis      == 0 && pInfo->shInt      == 0 && 
         pInfo->shNbrBoust == 0)
      {
         dwTypeItem = 0;
         crName = RGB(225,225,225);
      }

      if(dwTypeItem != 0)
      {
         backPopup.Draw(vsfFX);
         GameUI::Draw( vsfFX );
         
         
         HDC hdc;
         DXDGetDC(&hdc, 671);

         //regarde les boust pour definir la couleur des items...
         if(pInfo->shNbrBoust >= 9)
         {
            dwTypeItem = 4;
            crName = RGB(0,192,0);
         }
         else if(pInfo->shNbrBoust >= 6)
         {
            dwTypeItem = 3;
            crName = RGB(225,209,35);
         }
         else if(pInfo->shNbrBoust >= 3)
         {
            dwTypeItem = 2;
            crName = RGB(159,80,230);
         }
         else
         {
            dwTypeItem = 1;
            crName = RGB(30,130,255);
         }

         sprintf(strtmp  ,"%s",pInfo->Name);
         fNewGuiInfoFont2->DrawFont(hdc, dwOffX+TxtOff(fNewGuiInfoFont2,hdc,strtmp ,220), dwOffY+10 , crName , 0, strtmp  );

         int dwWriteYPos = 34;
         if(dwTypeItem != 0)
         {
            if(pInfo->shStr != 0 || pInfo->shEnd != 0 || pInfo->shAgi    != 0 ||
               pInfo->shWis != 0 || pInfo->shInt != 0 || pInfo->shAttack != 0 )
            {
               sprintf(strtmp  ,"%s",g_GUILocalString[87]);
               fNewGuiInfoFont2->DrawFont(hdc, dwOffX+TxtOff(fNewGuiInfoFont2,hdc,strtmp ,220), dwOffY+dwWriteYPos , RGB(190,190,190) , 0, strtmp  );
               sprintf(strtmp  ,"%s","____________________");
               fNewGuiInfoFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiInfoFont,hdc,strtmp ,220), dwOffY+dwWriteYPos+5 , RGB(190,190,190) , 0, strtmp  );
               dwWriteYPos+=5;
               //On Commence par les requirement !!!
               dwWriteYPos+=21;
               if(pInfo->shStr != 0)
               {
                  sprintf(strtmp  ,"%s",g_GUILocalString[88]);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+20, dwOffY+dwWriteYPos  , RGB(255,255,255) , 0, strtmp  );
                  sprintf(strtmp  ,":  %d",pInfo->shStr);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+110, dwOffY+dwWriteYPos  , crName , 0, strtmp  );
                  dwWriteYPos+=15;
               }
               if(pInfo->shEnd != 0)
               {
                  sprintf(strtmp  ,"%s",g_GUILocalString[89]);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+20, dwOffY+dwWriteYPos  , RGB(255,255,255) , 0, strtmp  );
                  sprintf(strtmp  ,":  %d",pInfo->shEnd);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+110, dwOffY+dwWriteYPos  , crName , 0, strtmp  );
                  dwWriteYPos+=15;
               }
               if(pInfo->shAgi != 0)
               {
                  sprintf(strtmp  ,"%s",g_GUILocalString[90]);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+20, dwOffY+dwWriteYPos  , RGB(255,255,255) , 0, strtmp  );
                  sprintf(strtmp  ,":  %d",pInfo->shAgi);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+110, dwOffY+dwWriteYPos  , crName , 0, strtmp  );
                  dwWriteYPos+=15;
               }
               if(pInfo->shWis != 0)
               {
                  sprintf(strtmp  ,"%s",g_GUILocalString[91]);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+20, dwOffY+dwWriteYPos  , RGB(255,255,255) , 0, strtmp  );
                  sprintf(strtmp  ,":  %d",pInfo->shWis);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+110, dwOffY+dwWriteYPos  , crName , 0, strtmp  );
                  dwWriteYPos+=15;
               }
               if(pInfo->shInt != 0)
               {
                  sprintf(strtmp  ,"%s",g_GUILocalString[92]);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+20, dwOffY+dwWriteYPos  , RGB(255,255,255) , 0, strtmp  );
                  sprintf(strtmp  ,":  %d",pInfo->shInt);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+110, dwOffY+dwWriteYPos  , crName , 0, strtmp  );
                  dwWriteYPos+=15;
               }
               if(pInfo->shAttack != 0)
               {
                  sprintf(strtmp  ,"%s",g_GUILocalString[93]);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+20, dwOffY+dwWriteYPos  , RGB(255,255,255) , 0, strtmp  );
                  sprintf(strtmp  ,":  %d",pInfo->shAttack);
                  fNewGuiInfoFont->DrawFont(hdc, dwOffX+110, dwOffY+dwWriteYPos  , crName , 0, strtmp  );
                  dwWriteYPos+=15;
               }
               dwWriteYPos+=5;
            } 

            if(pInfo->lMinDamage >0 || pInfo->lMaxDamage >0)
            {
               // les boust Direct....
               sprintf(strtmp  ,"%s",g_GUILocalString[94]);
               fNewGuiInfoFont2->DrawFont(hdc, dwOffX+TxtOff(fNewGuiInfoFont2,hdc,strtmp ,220), dwOffY+dwWriteYPos , RGB(190,190,190) , 0, strtmp  );
               sprintf(strtmp  ,"%s","____________________");
               fNewGuiInfoFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiInfoFont,hdc,strtmp ,220), dwOffY+dwWriteYPos+5 , RGB(190,190,190) , 0, strtmp  );
               dwWriteYPos+=5;
               dwWriteYPos+=21;

               sprintf(strtmp  ,"%s",g_GUILocalString[94]);
               fNewGuiInfoFont->DrawFont(hdc, dwOffX+20, dwOffY+dwWriteYPos  , RGB(255,255,255) , 0, strtmp  );
               sprintf(strtmp  ,":  ( %d - %d )",pInfo->lMinDamage,pInfo->lMaxDamage);
               fNewGuiInfoFont->DrawFont(hdc, dwOffX+110, dwOffY+dwWriteYPos  , crName , 0, strtmp  );
               dwWriteYPos+=15;
               dwWriteYPos+=5;
            }

            if(pInfo->shDodge !=0 || pInfo->shAC != 0 || pInfo->shNbrBoust >0)
            {
               // les boust Direct....
               sprintf(strtmp  ,"%s",g_GUILocalString[95]);
               fNewGuiInfoFont2->DrawFont(hdc, dwOffX+TxtOff(fNewGuiInfoFont2,hdc,strtmp ,220), dwOffY+dwWriteYPos , RGB(190,190,190) , 0, strtmp  );
               sprintf(strtmp  ,"%s","____________________");
               fNewGuiInfoFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiInfoFont,hdc,strtmp ,220), dwOffY+dwWriteYPos+5 , RGB(190,190,190) , 0, strtmp  );
               dwWriteYPos+=5;
               dwWriteYPos+=21;

               //commence par le boust de AC
               int dwBoustAC = 0;
               if(pInfo->shAC != 0 || pInfo->shNbrBoust >0)
               {
                  if(pInfo->shAC != 0)
                     dwBoustAC += pInfo->shAC;

                  for(int i=0;i<pInfo->shNbrBoust;i++)
                  {
                     if(pInfo->pBoust[i].chBoustStat == 20)
                     {
                        dwBoustAC += pInfo->pBoust[i].lMaxVal;
                     }
                  }
                  if(dwBoustAC !=0)
                  {
                     sprintf(strtmp  ,"%+d",dwBoustAC);
                     fNewGuiInfoFont->DrawFont(hdc, dwOffX+15, dwOffY+dwWriteYPos  , RGB(255,255,255) , 0, strtmp  );
                     sprintf(strtmp  ,"%s",m_strBoustName[20]);
                     fNewGuiInfoFont->DrawFont(hdc, dwOffX+60, dwOffY+dwWriteYPos  , crName , 0, strtmp  );
                     dwWriteYPos+=13;
                  }
               }
               //ensuite la dodge...
               int dwBoustDodge = 0;
               if(pInfo->shDodge != 0 || pInfo->shNbrBoust >0)
               {
                  if(pInfo->shDodge != 0)
                     dwBoustDodge += (-1*pInfo->shDodge);

                  for(int i=0;i<pInfo->shNbrBoust;i++)
                  {
                     if(pInfo->pBoust[i].chBoustStat == 9)
                     {
                        dwBoustDodge += pInfo->pBoust[i].lMaxVal;
                     }
                  }
                  if(dwBoustDodge !=0)
                  {
                     sprintf(strtmp  ,"%+d",dwBoustDodge);
                     fNewGuiInfoFont->DrawFont(hdc, dwOffX+15, dwOffY+dwWriteYPos  , RGB(255,255,255) , 0, strtmp  );
                     sprintf(strtmp  ,"%s",m_strBoustName[9]);
                     fNewGuiInfoFont->DrawFont(hdc, dwOffX+60, dwOffY+dwWriteYPos  , crName , 0, strtmp  );
                     dwWriteYPos+=13;
                  }
               }
               if(dwBoustAC != 0 || dwBoustDodge != 0)
                  dwWriteYPos+=6;
               //maintenant on scan la liste des autre boust...
               for(int i=0;i<pInfo->shNbrBoust;i++)
               {
                  if(pInfo->pBoust[i].chBoustStat != 0 && pInfo->pBoust[i].chBoustStat != 9 && pInfo->pBoust[i].chBoustStat != 20)
                  {
                     sprintf(strtmp  ,"%+d",pInfo->pBoust[i].lMaxVal);
                     fNewGuiInfoFont->DrawFont(hdc, dwOffX+15, dwOffY+dwWriteYPos  , RGB(255,255,255) , 0, strtmp  );
                     sprintf(strtmp  ,"%s",m_strBoustName[pInfo->pBoust[i].chBoustStat]);
                     fNewGuiInfoFont->DrawFont(hdc, dwOffX+60, dwOffY+dwWriteYPos  , crName , 0, strtmp  );
                     dwWriteYPos+=13;
                  }
               }
               dwWriteYPos+=5;
            }
         }
         char strBtnTexte[20];
         sprintf(strBtnTexte  ,"%s",g_GUILocalString[86]);
         fNewGuiBtnFont->DrawFont(hdc, dwOffX+TxtOff(fNewGuiBtnFont,hdc,strBtnTexte ,60)+ 80, dwOffY+306 , RGB(0, 0, 0)    , 0, strBtnTexte);
         DXDReleaseDC(hdc, 671);
      }
      else
      {
         Hide();
         if(pInfo->bInventory)
         {
         }
         else
         {
            Objects.Lock(40);
            Objects.Identify(pInfo->ulID);
            Objects.Unlock(40);
         }
      }
   }
   g_Global.Unlock();   
}

int RTItemI::TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise)
{
   int dwSize = (int)pFont->GetLen(hdc,pStr);
   return((dwBoxZise-dwSize)/2);
}

void RTItemI::Hide()
{
   BoxUI::Hide();
   g_Global.ResetItemInfo();
}

void RTItemI::CancelButtonEvent::LeftClicked( void )
{
   GetUI()->Hide(); 
   g_Global.ResetItemInfo();
}
