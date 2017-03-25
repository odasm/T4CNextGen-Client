// Global.h: interface for the Global class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBAL_H__BE9CC3E6_9953_49D5_86F2_D3E2D40E7912__INCLUDED_)
#define AFX_GLOBAL_H__BE9CC3E6_9953_49D5_86F2_D3E2D40E7912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

//extern Global g_Global;

#define NBR_ZONE_MAP 8

typedef struct _sItemBoust
{
   char chBoustStat;// see below for codes 
   long lMinVal;
   long lMaxVal;
}sItemBoust;

typedef struct _sItemInfo
{
   bool   bSet;
   unsigned char *Name;
   short  shAppearance;
   char   chRadiance;
   short  shAC;
   short  shDodge;
   short  shEnd;
   long   lMinDamage;
   long   lMaxDamage;
   short  shAttack;
   short  shStr;
   short  shAgi;
   short  shWis;
   short  shInt;
   short  shNbrBoust; // number of boosts of the object 
   ULONG  ulID;
   bool   bInventory;
   sItemBoust *pBoust;
}sItemInfo;

typedef struct _ClientConfig
{
   char strSavePath[1024];
   char strPath[512];
   char strAccountName[50];
   char strAccountIP[50];
   int  dwAfkStatus;
   char strAfkMessage[2048];
   int  dwDebug;
   int  dwNMDebug;
   int  dwNMShowFPS;
   int  dwNMShowPOS;
   int  dwNMFPSValue;
   int  FirstTimeAddon;
   int  WebpatchEnable;
}ClientConfig;


class Global  
{
public:
	Global();
	virtual ~Global();





protected:
   int  m_dwDisplayW;
   int  m_dwDisplayH;
   int  m_dwDisplayWDiv2;
   int  m_dwDisplayHDiv2;

   int  m_dwScreenW;
   int  m_dwScreenH;
   int  m_dwScreenWDiv2;
   int  m_dwScreenHDiv2;

   int  m_dwNbrTileX;
   int  m_dwNbrTileY;
   int  m_dwNbrTileXDiv2;
   int  m_dwNbrTileYDiv2;

   int  m_dwMainTorchW;
   int  m_dwMainTorchH;

   unsigned long m_crBtnEText;
   unsigned long m_crBtnDText;

   int m_dwAlphaValue;

   int m_dwLastScrollStatus;

   int m_dwZoomStatus;

   int m_dwAnimWaterCount;
   int m_dwAnimLavaCount;

   bool          m_bCanDisplayChange;
   bool          m_bDisplayChangeZone;
   bool          m_bForceDisplayZone;
   int           m_dwCurMapsZone;
   int           m_dwCurPosZone;
   int           m_dwChangeX;
   int           m_dwChangeY;
   unsigned char *m_pchZoneMaps;

   //int   m_dwLoadZoneXS;
   //int   m_dwLoadZoneYS;
   
   typedef struct _ZoneMapInfo
   {
      //char strMapName[20];
      bool bEnable;
   }ZoneMapInfo;

   typedef struct _ZoneInfo
   {
      char strZoneName[30];
      bool bEnable;
   }ZoneInfo;

   ZoneInfo     m_ZoneInfo[NBR_ZONE_MAP][256];
   ZoneMapInfo  m_ZoneMapInfo[NBR_ZONE_MAP];

   char m_strPath[512];
   char m_ZoneFile[512];

   sItemInfo *m_pItemInfo;

   int m_dwLastWorldX;
   int m_dwLastWorldY;


public:
   void Lock(void); 
   void Unlock(void);
   void SetDisplaySize(int dwW, int dwH);

   int GetDisplayW(){return m_dwDisplayW;}
   int GetDisplayH(){return m_dwDisplayH;}
   int GetDisplayWDiv2(){return m_dwDisplayWDiv2;}
   int GetDisplayHDiv2(){return m_dwDisplayHDiv2;}

   int GetScreenW(){return m_dwScreenW;}
   int GetScreenH(){return m_dwScreenH;}
   int GetScreenWDiv2(){return m_dwScreenWDiv2;}
   int GetScreenHDiv2(){return m_dwScreenHDiv2;}

   int GetNbrTileX(){return m_dwNbrTileX;}
   int GetNbrTileY(){return m_dwNbrTileY;}
   int GetNbrTileXDiv2(){return m_dwNbrTileXDiv2;}
   int GetNbrTileYDiv2(){return m_dwNbrTileYDiv2;}


   int GetMainTorchW(){return m_dwMainTorchW;}
   int GetMainTorchH(){return m_dwMainTorchH;}

   unsigned long GetBtnTCE(){return m_crBtnEText;}
   unsigned long GetBtnTCD(){return m_crBtnDText;}
   int           GetAlphaValue(){return m_dwAlphaValue;}
   void          SetlastScrollStatus(int dwVal){m_dwLastScrollStatus = dwVal;}
   int           GetlastScrollStatus(){return m_dwLastScrollStatus;}

   void          SetZoomStatus(int dwIncVal);
   int           GetZoomStatus(){return m_dwZoomStatus;}
   
   int           GetAnimWaterFrame(){return m_dwAnimWaterCount;}
   void          StepAnimWaterFrame();
   void          ResetAnimWater(){m_dwAnimWaterCount = 0;}

   //int           GetAnimLavaFrame(){return m_dwAnimLavaCount;}
   //void          StepAnimLavaFrame();
   //void          ResetAnimLava(){m_dwAnimLavaCount = 0;}
   

   void          LoadZoneMapWorld(int dwWorld, int dwX, int dwY);
   void          ValidMapZonePosition(int dwPosX, int dwPosY);
   void          ForceDisplayZone(int dwPosX, int dwPosY);
   int           GetDisplayZone();
   char*         GetDisplayZoneName();

   sItemInfo *   GetItemInfo(){return m_pItemInfo;};
   void          ResetItemInfo();
   void          SetItemInfoID(ULONG ulID);
   void          SetItemInfoInventory(bool bInventory);
   bool          IsItemInfoAvailable(){return m_pItemInfo->bSet;}


   ClientConfig *ReadClientConfig();
   ClientConfig *GetClientConfig();
   void          WriteClientConfig();


   void          SetMouseLastWorld(int dwX, int dwY)
   {
      m_dwLastWorldX = dwX;
      m_dwLastWorldY = dwY;
   }
   int           GetLastMouseWorldX(){return m_dwLastWorldX;}
   int           GetLastMouseWorldY(){return m_dwLastWorldY;}
   
   

protected:
   int LoadImageDataToBmpFile(char * lpszFileName,unsigned char *pData);
   int SaveCurrentImage8(BYTE *pshImage,BYTE *pPal,int dwWidth, int dwHeight,char *pstrName,BOOL bFlipV);


   ClientConfig m_ClientConfig;
   BOOL m_bConfigRead;


private:
   CRITICAL_SECTION csInternal;


};

#endif // !defined(AFX_GLOBAL_H__BE9CC3E6_9953_49D5_86F2_D3E2D40E7912__INCLUDED_)
