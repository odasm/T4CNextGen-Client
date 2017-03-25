// SaveGame.cpp: implementation of the CSaveGame class.
//
//////////////////////////////////////////////////////////////////////
#include "SaveGame.h"
#include "App.h"
#include "Zlib/zlib.h"
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <shlobj.h>
#include "NewInterface/RootBoxUI.h"

#define  MEM_COMPRESS_NEED(ActualLen) (ULONG)(ActualLen *1.1 + 12);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSaveGame g_SaveGame;

CSaveGame::CSaveGame()
{
   m_bIsLoaded         = FALSE;
   m_strPlayerName[0]  = 0x00;
   m_strAccountName[0] = 0x00;


   m_pOptionParams.bPageEnable             = 1;
   m_pOptionParams.dwMenuState             = 1;

   // Option Sound
   m_pOptionParams.dwMusicVol              = 9;
   m_pOptionParams.dwSoundVol              = 9;
   m_pOptionParams.bUseCD                  = 0;
   m_pOptionParams.bPageSound              = 1;
   // Option Graphic
   m_pOptionParams.dwBrightness = 6;
   m_pOptionParams.bLightHightGraph = 0;
   m_pOptionParams.bHightGraphEffect = 1;
   m_pOptionParams.bAnimatedWater = 1; // steph 1 au lieu de 0
   m_pOptionParams.bDitherGraph = 1;
   m_pOptionParams.bGuiAlpha = 1;
   m_pOptionParams.bSeraphAnim = 1;
   m_pOptionParams.bDisplayStatus = 1;
   m_pOptionParams.bDisplayXPBarText = 1;
   m_pOptionParams.bDisplayResistPower = 0; // steph ajout
   m_pOptionParams.bDisplayWeight = 0; // steph ajout
   m_pOptionParams.bEnableDisplayGold = 1; // steph ajout
   m_pOptionParams.bDisplayMacroFullScreen = 1;
   m_pOptionParams.bLockTarget = 0;
   m_pOptionParams.bLockResize = 0;
   m_pOptionParams.bHighFont = 0;
   m_pOptionParams.bEnableZoom = 1;
   m_pOptionParams.bHyperChat = 0;
   m_pOptionParams.bShowItemSpec = 1;
   m_pOptionParams.bShowNewLife = 1;
   m_pOptionParams.bOldStatBar = 0;
   m_pOptionParams.bShowNewOmbrage = 0;
   m_pOptionParams.bShowAnimDecorsLight = 1;
   m_pOptionParams.bShowWeatherEffects = 1;

   //option Log
   m_pOptionParams.blogNPC    = 1;
   m_pOptionParams.bLogUser   = 1;
   m_pOptionParams.bLogEnable = 0;

   //RTMap
   for(int w=0;w<10;w++)
   {
      for(int y=0;y<TR_MAP_Y;y++)
      {
         for(int x=0;x<TR_MAP_X;x++)
         {
            m_uchRTMap[w][y][x] = 0x00;
         }
      }
   }

   SYSTEMTIME sysTime; 
	GetLocalTime(&sysTime);
   sprintf(m_pOptionParams.LogName,"%02d%02d%02d_log.txt",sysTime.wMonth,sysTime.wDay,sysTime.wYear-2000);
}

CSaveGame::~CSaveGame()
{

}


BOOL CSaveGame::bLoad(char *pstrAccountName,char *pstrPlayerName)
{
   
   if(m_bIsLoaded)
      bFree();

   ClientConfig *pConfig = g_Global.GetClientConfig();

   // create directory savegame if not exist...
   char strSaveGameDir[MAX_PATH];
   sprintf(strSaveGameDir,"%s\\SaveGame\\",pConfig->strSavePath);
   _mkdir(strSaveGameDir);

   sprintf(m_strAccountName,"%s",pstrAccountName);
   sprintf(m_strPlayerName ,"%s",pstrPlayerName);
   sprintf(m_strFileName,"%s%s_%s.dat",strSaveGameDir,pstrAccountName,pstrPlayerName);

   // load currentFile...
   FILE *pf = fopen(m_strFileName,"rb");
   if(pf)
   {

      ULONG dwTailleOri;
      ULONG dwTailleComp;
      BYTE *pFileDataTmp = NULL;
      BYTE *pFileDataTmpC = NULL;
      
	  fread(&dwTailleOri,4,1,pf);
      fread(&dwTailleComp,4,1,pf);

      try{
	  pFileDataTmp = new BYTE[dwTailleOri];
      pFileDataTmpC= new BYTE[dwTailleComp]; 
	  }catch(std::bad_alloc e){
		MessageBox(NULL,"Impossible d'allouer la memoire pour charger la sauvegarde !","",MB_OK);
	  }
	  
	  fread(pFileDataTmpC,1,dwTailleComp,pf);
      fclose(pf);
       
      if(uncompress(pFileDataTmp,&dwTailleOri,pFileDataTmpC,dwTailleComp) != S_OK)
          MessageBox(NULL,"Erreur UnCompress SaveGame","",MB_OK);
	  
      int dwNbrInv; 
      int dwNbrMacroItem;
      int dwNbrMacroSpell;
      int dwNbrMacroSkill;
      int dwNbrChannel;
      int dwNbrIgnore;
      int dwNbrChest;
      sInventory  pInventory;
      sMacro      pMacro;
      sChannel    pChannel;
      sIgnoreList pIgnore;
	  int i;

      unsigned char *pchBuff = pFileDataTmp;

      char chHeader[19];
      memcpy(&chHeader,pchBuff,18);   pchBuff+=18;
      chHeader[18] = 0x00;
      if(strcmp(chHeader,"NM_V2SAVEGAME_V004") == 0)
      {
		  
		 memcpy(&dwNbrInv,pchBuff,4);
         pchBuff+=4;
         for(int i=0;i<dwNbrInv;i++)
         {
            memcpy(&pInventory,pchBuff,sizeof(sInventory)); pchBuff+=sizeof(sInventory);
            AddInv(pInventory);
         }

         memcpy(&dwNbrMacroItem,pchBuff,4);
         pchBuff+=4;
         for(i=0;i<dwNbrMacroItem;i++)
         {
            memcpy(&pMacro,pchBuff,sizeof(sMacro)); pchBuff+=sizeof(sMacro);
            AddMacroItem(pMacro);
         }

         memcpy(&dwNbrMacroSpell,pchBuff,4);
         pchBuff+=4;
         for(i=0;i<dwNbrMacroSpell;i++)
         {
            memcpy(&pMacro,pchBuff,sizeof(sMacro)); pchBuff+=sizeof(sMacro);
            AddMacroSpell(pMacro);
         }

         memcpy(&dwNbrMacroSkill,pchBuff,4);
         pchBuff+=4;
         for(i=0;i<dwNbrMacroSkill;i++)
         {
            memcpy(&pMacro,pchBuff,sizeof(sMacro)); pchBuff+=sizeof(sMacro);
            AddMacroSkill(pMacro);
         }

         memcpy(&dwNbrChannel,pchBuff,4);
         pchBuff+=4;
         for(i=0;i<dwNbrChannel;i++)
         {
            memcpy(&pChannel,pchBuff,sizeof(sChannel)); pchBuff+=sizeof(sChannel);
            AddChannel(pChannel);
         }

         memcpy(&dwNbrIgnore,pchBuff,4);
         pchBuff+=4;
         for(i=0;i<dwNbrIgnore;i++)
         {
            memcpy(&pIgnore,pchBuff,sizeof(sIgnoreList)); pchBuff+=sizeof(sIgnoreList);
            AddIgnore(pIgnore);
         }

         memcpy(&dwNbrChest,pchBuff,4);
         pchBuff+=4;
         for(i=0;i<dwNbrChest;i++)
         {
            memcpy(&pInventory,pchBuff,sizeof(sInventory)); pchBuff+=sizeof(sInventory);
            AddChest(pInventory);
         }

         memcpy(&m_pOptionParams,pchBuff,sizeof(OptionParam)); pchBuff+=sizeof(OptionParam);


         if(DISABLE_TMI_MAP == 1)
         {
            if(m_pOptionParams.dwMenuState == MENU_BOTH || m_pOptionParams.dwMenuState == MENU_TMI)
            {
               m_pOptionParams.dwMenuState = MENU_CHAT;
            }
         }
		 
         // read les info des RT MAp...
         memcpy(&m_uchRTMap,pchBuff,10*TR_MAP_Y*TR_MAP_X); pchBuff+=(10*TR_MAP_Y*TR_MAP_X);		 		 
         
		 //BL : visiblement la libération de ces ressources provoque un crash.. j'ajoute un test sur NULL
		 if (pFileDataTmp != NULL) delete pFileDataTmp;//BLBLBL []pFileDataTmp...
         if (pFileDataTmpC != NULL) delete pFileDataTmpC;
  
      }
      else
      {
         g_Var.InvalideSaveGame = true;
         delete []pFileDataTmp;//BL : visiblement la libération de ces ressources provoque un crash..
         delete []pFileDataTmpC;
         bFree();
		 unlink(m_strFileName);//BLBLBL si le fichier de sauvegarde n'est pas bon, autant le supprimer...
      }
   }


   SYSTEMTIME sysTime; 
   GetLocalTime(&sysTime);
   sprintf(m_pOptionParams.LogName,"%02d%02d%02d_log.txt",sysTime.wMonth,sysTime.wDay,sysTime.wYear-2000);

   if(g_Global.GetDisplayW() <=640)
      m_pOptionParams.bHyperChat = FALSE;

   m_bIsLoaded = TRUE;
   return m_bIsLoaded;
}

BOOL CSaveGame::bSave()
{
   if(!m_bIsLoaded)
      return FALSE;


   ClientConfig *pConfig = g_Global.GetClientConfig();

  
   // create directory savegame if not exist...
   char strSaveGameDir[MAX_PATH];
   sprintf(strSaveGameDir,"%s\\SaveGame\\",pConfig->strSavePath);
   _mkdir(strSaveGameDir);

   // load currentFile...
   FILE *pf = fopen(m_strFileName,"wb+");//BLBLBL Create file if not exist
   if(pf)
   {
       if(g_Global.GetDisplayW() <=640)
         m_pOptionParams.bHyperChat = FALSE;

       int dwNbrInv        = m_pInventoryL .size();
       int dwNbrMacroItem  = m_pMacroItemL .size();
       int dwNbrMacroSpell = m_pMacroSpellL.size();
       int dwNbrMacroSkill = m_pMacroSkillL.size();
       int dwNbrChannel    = m_pChannelL   .size();
       int dwNbrIgnore     = m_pIgnoreL    .size();
       int dwNbrChest      = m_pChestL     .size();
	   int i;
       char chHeader[19];
       sprintf(chHeader,"NM_V2SAVEGAME_V004");
       fwrite(&chHeader                          ,1,strlen(chHeader),pf);
       fwrite(&dwNbrInv,1,4,pf);

       for(i=0;i<dwNbrInv;i++)
       {
          memset(m_pInventoryL[i].chPad,0x00,52);
          fwrite(&m_pInventoryL[i],sizeof(sInventory),1,pf);
       }
       fwrite(&dwNbrMacroItem,1,4,pf);
       for(i=0;i<dwNbrMacroItem;i++)
       {
          memset(m_pMacroItemL[i].chPad,0x00,52);
          fwrite(&m_pMacroItemL[i],sizeof(sMacro),1,pf);
       }
       fwrite(&dwNbrMacroSpell,1,4,pf);
       for(i=0;i<dwNbrMacroSpell;i++)
       {
          memset(m_pMacroSpellL[i].chPad,0x00,52);
          fwrite(&m_pMacroSpellL[i],sizeof(sMacro),1,pf);
       }
       fwrite(&dwNbrMacroSkill,1,4,pf);
       for(i=0;i<dwNbrMacroSkill;i++)
       {
          memset(m_pMacroSkillL[i].chPad,0x00,52);
          fwrite(&m_pMacroSkillL[i],sizeof(sMacro),1,pf);
       }
       fwrite(&dwNbrChannel,1,4,pf);
       for(i=0;i<dwNbrChannel;i++)
       {
          memset(m_pChannelL[i].chPad,0x00,52);
          fwrite(&m_pChannelL[i],sizeof(sChannel),1,pf);
       }
       fwrite(&dwNbrIgnore,1,4,pf);
       for(i=0;i<dwNbrIgnore;i++)
       {
          memset(m_pIgnoreL[i].chPad,0x00,52);
          fwrite(&m_pIgnoreL[i],sizeof(sIgnoreList),1,pf);
       }
       fwrite(&dwNbrChest,1,4,pf);
       for(i=0;i<dwNbrChest;i++)
       {
          memset(m_pChestL[i].chPad,0x00,52);
          fwrite(&m_pChestL[i],sizeof(sInventory),1,pf);
       }
       memset(m_pOptionParams.chPad,0x00,64);
       fwrite(&m_pOptionParams,sizeof(OptionParam),1,pf);

       // read les info des RT MAp...
       fwrite(&m_uchRTMap,10*TR_MAP_Y*TR_MAP_X,1,pf);
       
       fclose(pf);

       // on compresse le fichier et le resave...
       ULONG dwTaille;
       FILE *pf1=fopen(m_strFileName,"rb");
       fseek(pf1,0,SEEK_END);
       dwTaille = ftell(pf1);
       fseek(pf1,0,SEEK_SET);
       unsigned char *pFileDataTmp = NULL;
       pFileDataTmp = new unsigned char[dwTaille ];
       fread(pFileDataTmp,dwTaille,1,pf1);
       fclose(pf1);

       unsigned char *pFileDataTmpC = NULL;
       ULONG  dwTailleC = MEM_COMPRESS_NEED(dwTaille);
       pFileDataTmpC= new unsigned char[dwTailleC];

       if( compress(pFileDataTmpC,&dwTailleC,pFileDataTmp,dwTaille) != Z_OK )
          MessageBox(NULL,"Erreur Compress SaveGame","",MB_OK);

       // save le fichier...
       FILE *pf2=fopen(m_strFileName,"wb+");//BLBLBL create file if not exist
       fwrite(&dwTaille,4,1,pf2);
       fwrite(&dwTailleC,4,1,pf2);
       fwrite(pFileDataTmpC,1,dwTailleC,pf2);
       fclose(pf2); 
       
       delete []pFileDataTmp;
       delete []pFileDataTmpC;
       
   }
   return TRUE;
}

BOOL CSaveGame::bFree()
{
   m_pInventoryL .empty();
   m_pInventoryL .clear();
   m_pMacroItemL .empty();
   m_pMacroItemL .clear();
   m_pMacroSpellL.empty();
   m_pMacroSpellL.clear();
   m_pMacroSkillL.empty();
   m_pMacroSkillL.clear();
   m_pChannelL   .empty();
   m_pChannelL   .clear();
   m_pIgnoreL    .empty();
   m_pIgnoreL    .clear();
   m_pChestL     .empty();
   m_pChestL     .clear();
   m_bIsLoaded = FALSE;
   return TRUE;
}

BOOL CSaveGame::bFreeInv()
{
   m_pInventoryL .empty();
   m_pInventoryL .clear();
   return TRUE;
}

BOOL CSaveGame::bFreeMacroItem()
{
   m_pMacroItemL .empty();
   m_pMacroItemL .clear();
   return TRUE;
}

BOOL CSaveGame::bFreeMacroSpell()
{
   m_pMacroSpellL.empty();
   m_pMacroSpellL.clear();
   return TRUE;
}

BOOL CSaveGame::bFreeMacroSkill()
{
   m_pMacroSkillL.empty();
   m_pMacroSkillL.clear();
   return TRUE;
}

BOOL CSaveGame::bFreeChannel()
{
   m_pChannelL   .empty();
   m_pChannelL   .clear();
   return TRUE;
}

BOOL CSaveGame::bFreeIgnore()
{
   m_pIgnoreL    .empty();
   m_pIgnoreL    .clear();
   return TRUE;
}

BOOL CSaveGame::bFreeChest()
{
   m_pChestL .empty();
   m_pChestL .clear();
   return TRUE;
}

BOOL CSaveGame::AddInv       (sInventory  pInventory)
{
   m_pInventoryL.push_back(pInventory);
   return TRUE;
}

BOOL CSaveGame::AddMacroItem (sMacro      pMacroItem)
{
   m_pMacroItemL.push_back(pMacroItem);
   return TRUE;
}

BOOL CSaveGame::AddMacroSpell(sMacro      pMacroSpell)
{
   m_pMacroSpellL.push_back(pMacroSpell);
   return TRUE;
}

BOOL CSaveGame::AddMacroSkill(sMacro      pMacroSkill)
{
   m_pMacroSkillL.push_back(pMacroSkill);
   return TRUE;
}

BOOL CSaveGame::AddChannel   (sChannel    pChannel)
{
   m_pChannelL.push_back(pChannel);
   return TRUE;
}

BOOL CSaveGame::AddIgnore    (sIgnoreList pIgnore)
{
   m_pIgnoreL.push_back(pIgnore);
   return TRUE;
}

BOOL CSaveGame::AddChest (sInventory  pChest)
{
   m_pChestL.push_back(pChest);
   return TRUE;
}

BOOL CSaveGame::GetInv    (UINT dwIndex,sInventory  *pInventory )
{
   if(dwIndex < m_pInventoryL.size())
      memcpy(pInventory,&m_pInventoryL[dwIndex],sizeof(sInventory));
   return TRUE;
}

BOOL CSaveGame::GetMItem  (UINT dwIndex,sMacro      *pMacroItem )
{
   if(dwIndex < m_pMacroItemL.size())
      memcpy(pMacroItem,&m_pMacroItemL[dwIndex],sizeof(sMacro));
   return TRUE;
}

BOOL CSaveGame::GetMSpell (UINT dwIndex,sMacro      *pMacroSpell)
{
   if(dwIndex < m_pMacroSpellL.size())
      memcpy(pMacroSpell,&m_pMacroSpellL[dwIndex],sizeof(sMacro));
   return TRUE;
}

BOOL CSaveGame::GetMSkill (UINT dwIndex,sMacro      *pMacroSkill)
{
   if(dwIndex < m_pMacroSkillL.size())
      memcpy(pMacroSkill,&m_pMacroSkillL[dwIndex],sizeof(sMacro));
   return TRUE;
}

BOOL CSaveGame::GetChannel(UINT dwIndex,sChannel    *pChannel   )
{
   if(dwIndex < m_pChannelL.size())
      memcpy(pChannel,&m_pChannelL[dwIndex],sizeof(sChannel));
   return TRUE;
}

BOOL CSaveGame::GetIgnore (UINT dwIndex,sIgnoreList *pIgnore    )
{
   if(dwIndex < m_pIgnoreL.size())
      memcpy(pIgnore,&m_pIgnoreL[dwIndex],sizeof(sIgnoreList));
   return TRUE;
}

BOOL CSaveGame::GetChest    (UINT dwIndex,sInventory  *pChest )
{
   if(dwIndex < m_pChestL.size())
      memcpy(pChest,&m_pChestL[dwIndex],sizeof(sInventory));
   return TRUE;
}

void CSaveGame::SetRTMapVal(USHORT ushX,USHORT ushY,USHORT ushW )
{
   int dwRtX = ushX/TR_MAP_XDIV;
   int dwRtY = ushY/TR_MAP_YDIV;

   m_uchRTMap[ushW][dwRtY][dwRtX] = 1;
}

BOOL CSaveGame::GetRTMapVal(USHORT ushX,USHORT ushY,USHORT ushW , int *pdwX,int *pdwY)
{  
   int dwRtX = ushX/TR_MAP_XDIV;
   int dwRtY = ushY/TR_MAP_YDIV;

   *pdwX = dwRtX;
   *pdwY = dwRtY;

   if(m_uchRTMap[ushW][dwRtY][dwRtX]==1)
      return TRUE;
   else
      return FALSE;
}

BOOL CSaveGame::GetRTMapValCase(USHORT ushX,USHORT ushY,USHORT ushW )
{  
   if(ushX >= TR_MAP_X || ushY >= TR_MAP_Y)
      return FALSE;

   if(m_uchRTMap[ushW][ushY][ushX]==1)
      return TRUE;
   else
      return FALSE;
}