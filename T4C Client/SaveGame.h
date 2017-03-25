// SaveGame.h: interface for the CSaveGame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEGAME_H__BE9CC3E6_9953_49D5_86F2_D3E2D40E7912__INCLUDED_)
#define AFX_SAVEGAME_H__BE9CC3E6_9953_49D5_86F2_D3E2D40E7912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <direct.h>
#include <vector>


#define TR_MAP_Y 192
#define TR_MAP_X 192

#define TR_MAP_YDIV (3072/TR_MAP_Y)
#define TR_MAP_XDIV (3072/TR_MAP_X)



typedef struct _sInventory
{
   int dwID;
   int dwType;
   int dwX;
   int dwY;
   char chPad[52]; // we can add another X bytes
}sInventory;

typedef struct _sMacro
{
   int  dwIcon;
   int  dwID;
   int  dwKey;
   int  dwFastPos;
   char strName[100]; 
   char chPad[52]; // we can add another X bytes
}sMacro;

typedef struct _sChannel
{
   char strName[50];
   char strPassword[50];
   int  dwColor;
   int  chEnable;
   char chPad[52]; // we can add another X bytes
}sChannel;

typedef struct _sIgnoreList
{
   char strName[48];
   char chPad[52]; // we can add another X bytes
}sIgnoreList;

typedef struct _sOptionParam
{
   // General Config
   int  bPageEnable;
   int  dwMenuState;

   // Option Sound
   int  dwMusicVol;
   int  dwSoundVol;
   int  bUseCD;
   int  bPageSound;
   // Option Graphic
   int  dwBrightness;
   int  bLightHightGraph;
   int  bHightGraphEffect;
   int  bAnimatedWater;
   int  bDitherGraph;
   int  bGuiAlpha;
   int  bSeraphAnim;
   int  bDisplayStatus;
   int  bDisplayXPBarText;
   int  bDisplayResistPower; // steph ajout
   int  bDisplayWeight; // steph ajout
   int  bEnableDisplayGold; // steph ajout
   int  bDisplayMacroFullScreen;
   int  bLockTarget;

   //option Log
   int  blogNPC;
   int  bLogUser;
   int  bLogEnable;
   char LogName[48];

   //new option
   int  bLockResize;
   int  bHighFont;
   int  bEnableZoom;
   int  bHyperChat;
   int  bShowItemSpec;
   int  bShowNewLife;
   int  bOldStatBar;
   int  bShowNewOmbrage;
   int  bShowAnimDecorsLight;
   int  bShowWeatherEffects;
      
   char chPad[48]; // we can add another  bytes
}OptionParam;



class CSaveGame  
{
public:
	CSaveGame();
	virtual ~CSaveGame();
   BOOL bLoad(char *pstrAccountName,char *pstrPlayerName);
   BOOL bSave();
   BOOL bFree();

   BOOL bFreeInv();
   BOOL bFreeMacroItem();
   BOOL bFreeMacroSpell();
   BOOL bFreeMacroSkill();
   BOOL bFreeChannel();
   BOOL bFreeIgnore();
   BOOL bFreeChest();

   BOOL AddInv       (sInventory  pInventory );
   BOOL AddMacroItem (sMacro      pMacroItem );
   BOOL AddMacroSpell(sMacro      pMacroSpell);
   BOOL AddMacroSkill(sMacro      pMacroSkill);
   BOOL AddChannel   (sChannel    pChannel   );
   BOOL AddIgnore    (sIgnoreList pIgnore    );
   BOOL AddChest     (sInventory  pChest );

   int GetNbrInv    (){return m_pInventoryL.size();}
   int GetNbrMItem  (){return m_pMacroItemL.size();}
   int GetNbrMSpell (){return m_pMacroSpellL.size();}
   int GetNbrMSkill (){return m_pMacroSkillL.size();}
   int GetNbrChannel(){return m_pChannelL.size();}
   int GetNbrIgnore (){return m_pIgnoreL.size();}
   int GetNbrChest  (){return m_pChestL.size();}
   OptionParam* GetOptionsParam(){return &m_pOptionParams;}

   BOOL GetInv    (UINT dwIndex,sInventory  *pInventory );
   BOOL GetMItem  (UINT dwIndex,sMacro      *pMacroItem );
   BOOL GetMSpell (UINT dwIndex,sMacro      *pMacroSpell);
   BOOL GetMSkill (UINT dwIndex,sMacro      *pMacroSkill);
   BOOL GetChannel(UINT dwIndex,sChannel    *pChannel   );
   BOOL GetIgnore (UINT dwIndex,sIgnoreList *pIgnore    );
   BOOL GetChest  (UINT dwIndex,sInventory  *pChest );

   void SetRTMapVal(USHORT ushX,USHORT ushY,USHORT ushW );
   BOOL GetRTMapVal(USHORT ushX,USHORT ushY,USHORT ushW , int *pdwX,int *pdwY);
   BOOL GetRTMapValCase(USHORT ushX,USHORT ushY,USHORT ushW );

protected:
   BOOL m_bIsLoaded;
   char m_strAccountName[50];
   char m_strPlayerName[50];
   char m_strFileName[512];

   unsigned char m_uchRTMap[10][TR_MAP_Y][TR_MAP_X];

  
   std::vector <sInventory>  m_pInventoryL;
   std::vector <sMacro>      m_pMacroItemL;
   std::vector <sMacro>      m_pMacroSpellL;
   std::vector <sMacro>      m_pMacroSkillL;
   std::vector <sChannel>    m_pChannelL;
   std::vector <sIgnoreList> m_pIgnoreL;
   std::vector <sInventory>  m_pChestL;

   OptionParam  m_pOptionParams;
   

public:
   
protected:


};

#endif // !defined(AFX_SAVEGAME_H__BE9CC3E6_9953_49D5_86F2_D3E2D40E7912__INCLUDED_)
