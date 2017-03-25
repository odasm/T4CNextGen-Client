#if !defined(AFX_MAINDIALOG_H__ECED43D8_DA17_4135_ADB5_A03EB04A9523__INCLUDED_)
#define AFX_MAINDIALOG_H__ECED43D8_DA17_4135_ADB5_A03EB04A9523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDialog.h : header file
//
#include "Resource.h"
#include "Controls\SkinButton.h"
#include "Controls\uicLabel.h"
#include "controls\NMTransListCtrl.h"
#include "controls\DropBtn.h"
#include "controls\CreditsCtrl.h"
#include "controls\EditTrans.h"
#include "NMDShow.h"
#include "ServerDlg.h"
#include "afxtempl.h"
#include "GUILocalString.h"

#define BTN_ID_DROP_SERVER_BTN          14206


 
/////////////////////////////////////////////////////////////////////////////
// CMainDialog dialog

class CMainDialog : public CDialog
{
// Construction
public:
	CMainDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDialog)
	enum { IDD = IDD_MAIN_DIALOG };
	uicLabel	m_staticPassword;
	uicLabel	m_staticLogin;
	uicLabel	m_staticIP;
	CEditTrans	m_ctrlPassword;
	CEditTrans	m_ctrlLogin;
	CEditTrans	m_ctrlIP;
	CCreditsCtrl     	m_ctrlCredit;
	CNMTransListCtrl	m_ListInfo;
	uicLabel 	m_staticConnectGeneral;
	uicLabel  	m_staticLinkWeb1;
	uicLabel  	m_staticLinkWeb2;
	uicLabel  	m_staticTitle;
	CSkinButton	m_btnMinimize;
	CSkinButton	m_btnQuitter;
	CSkinButton	m_btnConnect;
	CSkinButton	m_btnCredit;
	CSkinButton	m_btnServer;
	CSkinButton		m_btnIntro;
   CDropButton	   m_pDropServer;
	CString	m_strDirectIP;
	CString	m_strDirectLogin;
	CString	m_strDirectPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDialog)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
   afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnCancel();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnIntro();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnMinimize();
	afx_msg void OnBtnServerInfo();
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnCredit();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
   static void DrawCreditsBackground(CDC *pDC, RECT rect, BOOL bAnimate, DWORD lParam,CBitmap *pBitMap);
   BOOL ISInit(){return m_bInit;}
   void SetTitle(char *pstrTitle);
   void SetVersion(char *pstrVersion);
   void SetPathGame(char *pstrPath);
   void SetPathSave(char *pstrPath);
   void SetAccountLogin(char *pstr);
   void SetAccountIP(char *pstr);
   UINT GetLastAction();
   void AddConnectMessage(char *pstrMsg, int dwLineIndex,BOOL bChangeConnect = FALSE);
   void AddGeneralMessage(char *pstrMsg);
   void ForceConnectState(BOOL bState);
   void ProcessMessage();
   void freeDialog();
   void UpdateServerList();

   BOOL GetWindowed();
   int  GetWidth();
   int  GetHeight();
   char* GetLogin();
   char* GetPassword();
   char* GetIP();
   int   GetLanguage();

   char* GetAccountLogin();
   char* GetAccountIP();


protected:
   void ChangeConnectStat(BOOL bStat);
   BOOL LoadSetSkin();
   BOOL DeleteSkinBmp();
   BOOL TileBitmap(CDC* _pDC, CRect _rc);


   void LoadBASS(int n);
   void FreeBASS();
   void PlaySonAmbiant(int dwIndex,char *pStrSoundName);
   void StopSonAmbiant(int dwIndex);


   void ReadConfig();
   void WriteConfig();

   void LoadAndPlayMusic();
   void CheckIfNeedToStartStopSound();
   

public:


protected:
   CServerDlg   *m_pServerDlg;
   CNMDShow   *m_pNMShowAVI;
   CFont      m_TxtCB;
   
   CString  m_strTitle;
   CString  m_strVersion;
   CString  m_strPathGame;
   CString  m_strPathSave;
   CString  m_strNMAccountName;
   CString  m_strNMAccountIP;
   CString  m_strAmbiantSound1;
   CString  m_strAmbiantSound2;
   CString  m_strAmbiantEffect1;
   CString  m_strIntroDls;
   BOOL m_bMusicInitialised;
   BOOL m_bCanPlayMusic;
   BOOL m_bInit;
   BOOL m_bIntroStatus;
   BOOL m_bCreditStatus;
   BOOL m_bForceConnectState;
   BOOL m_bMessageServer;
   BOOL m_bFirstredraw;
   UINT m_dwAction;
   int  m_dwSelectItemList;
   int  m_dwOLDVolume;

   // BMP pour les skins......
   CBitmap  m_bmpBackground;
   CBitmap  m_bmpBtnFrame2;
   CBitmap  m_bmpBtnFrame3;
   CBitmap  m_BGBitmapC;

   
   CDC      m_dcBitmap;

   sLauncherParam m_sLauncherConfig;


   GUILocalString m_Language;
   //char m_Language[1000][100];




};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDIALOG_H__ECED43D8_DA17_4135_ADB5_A03EB04A9523__INCLUDED_)
