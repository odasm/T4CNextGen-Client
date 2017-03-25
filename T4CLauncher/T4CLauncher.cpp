// T4CLauncher.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#define T4C_LAUNCHER_EXPORTS
#include "T4CLauncher.h"
#include "MainDialog.h"
  
static CMainDialog *gDialog = NULL;
 
/*
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
*/ 

// This is the constructor of a class that has been exported.
// see CTest.h for the class definition
CT4CLauncher::CT4CLauncher()
{ 
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   gDialog = NULL;
	return; 
}

CT4CLauncher::~CT4CLauncher()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if(gDialog)
   {
      gDialog->DestroyWindow();
      delete gDialog;
      gDialog = NULL;
   }
}

void CT4CLauncher::CreateNMDialog(char *pstrTitle,char *pstrVersion,char *pstrPathGame,char *pstrPathSave,char *pstrLogin,char *pstrIP)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   gDialog = new CMainDialog();
   gDialog->SetTitle(pstrTitle);
   gDialog->SetVersion(pstrVersion);
   gDialog->SetPathGame(pstrPathGame);
   gDialog->SetPathSave(pstrPathSave);
   gDialog->SetAccountLogin(pstrLogin);
   gDialog->SetAccountIP(pstrIP);
   gDialog->Create(IDD_MAIN_DIALOG,NULL);
   gDialog->ShowWindow(SW_SHOW);
}

void CT4CLauncher::FreeNMDialog()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   gDialog->freeDialog();
}

BOOL CT4CLauncher::IsLuncherAvailable()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if(gDialog)
      return gDialog->ISInit();
   return FALSE;
}

UINT CT4CLauncher::GetLastAction()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if(gDialog)
      return gDialog->GetLastAction();
   return 0;
}

void CT4CLauncher::AddConnectMessage(char *pstrMsg, int dwLineIndex,BOOL bChangeCOnnect)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if(gDialog)
      gDialog->AddConnectMessage(pstrMsg,dwLineIndex,bChangeCOnnect);
   return ;
}

void CT4CLauncher::AddGeneralMessage(char *pstrMsg)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if(gDialog)
      gDialog->AddGeneralMessage(pstrMsg);
   return ;
}

void CT4CLauncher::ForceConnectState(BOOL bState)
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   if(gDialog)
      gDialog->ForceConnectState(bState);
   return ;
}

void CT4CLauncher::ProcessMessage()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   gDialog->ProcessMessage();
   Sleep(10);
}

BOOL CT4CLauncher::GetWindowed()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return gDialog->GetWindowed();
}

int  CT4CLauncher::GetWidth()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return gDialog->GetWidth();
}

int  CT4CLauncher::GetHeight()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return gDialog->GetHeight();
}

char* CT4CLauncher::GetLogin()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return gDialog->GetLogin();
}

char* CT4CLauncher::GetPassword()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return gDialog->GetPassword();
}

char* CT4CLauncher::GetIP()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return gDialog->GetIP();
}

char* CT4CLauncher::GetAccountLogin()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return gDialog->GetAccountLogin();
}

char* CT4CLauncher::GetAccountIP()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return gDialog->GetAccountIP();
}

int  CT4CLauncher::GetLanguage()
{
   AFX_MANAGE_STATE(AfxGetStaticModuleState());
   return gDialog->GetLanguage();
}

