#if !defined(AFX_SERVERDLG_H__8F54F664_4474_4D15_B412_6DDA5B9D3569__INCLUDED_)
#define AFX_SERVERDLG_H__8F54F664_4474_4D15_B412_6DDA5B9D3569__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerDlg.h : header file
//
#include "resource.h" 
#include "Controls\SkinButton.h"
#include "Controls\uicLabel.h"
#include "controls\NMTransListCtrl.h"
#include "controls\EditTrans.h"
#include "controls\DropBtn.h"
#include "GUILocalString.h"
#include "MacroDialog.h"


#define BTN_ID_DROP_SERVERL_BTN          14207

 

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DLG };
	uicLabel	m_staticMusique;
	uicLabel	m_static640;
	CSkinButton	m_btnMacro;
	uicLabel	m_staticWindowed;
	uicLabel	m_static800;
	CSkinButton	m_btnClose;
	uicLabel	m_staticTitle;
	uicLabel	m_staticPass;
	uicLabel	m_staticName;
	uicLabel	m_staticLogin;
	uicLabel	m_staticIP;
	CEditTrans	m_editServerPass;
	CEditTrans	m_editServerLogin;
	CEditTrans	m_editServerIP;
	CEditTrans	m_editServerName;
	CNMTransListCtrl	m_listServer;
	CSkinButton	m_btnAddInf;
	CSkinButton	m_btnDelete;
	CSkinButton	m_btnFremer;
	CString	m_strName;
	CString	m_strIP;
	CString	m_strLogin;
	CString	m_strPass;
	BOOL	m_bRes800;
	BOOL	m_bWindowed;
   CDropButton	   m_pDropServerList;
	int		m_dwCBLanguage;
	BOOL	m_bRes640;
	BOOL	m_bLauncherMusic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
   afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnFermer();
	afx_msg void OnSelchangeCbLanguage();
	afx_msg void OnBtnMacro();
	afx_msg void OnCheck800x600();
	afx_msg void OnCheck640x480();
	afx_msg void OnCheckLauncherSound();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
   void ProcessMessage() ;
   BOOL IsInit(){return m_bINIT;}
   void SetDlgPos(int dwLeft, int dwTop);
   void SetServerList(sLauncherParam *spLauncherConfig){m_spLauncherConfig = spLauncherConfig;};
   void SetPathGame(char *pstrPath){m_strPathGame.Format("%s",pstrPath);};
   void SetPathSave(char *pstrPath){m_strPathSave.Format("%s",pstrPath);};

   int m_dwNewLanguage;

protected:
   BOOL LoadSetSkin();
   BOOL DeleteSkinBmp();
   BOOL TileBitmap(CDC* _pDC, CRect _rc);

   void UpdateServerList();
   void UpdateServerListIP();

protected:
   CMacroDialog   *m_pMacroDlg;
   CString m_strPathGame;
   CString m_strPathSave;
   CFont      m_TxtCB;
   BOOL m_bINIT;
   BOOL m_bMessageMacro;
   int  m_dwAnimPos;
   int  m_dwLeft;
   int  m_dwTop;
   int  m_dwSelectItemList;
   

   sLauncherParam *m_spLauncherConfig;

   // BMP pour les skins......
   CBitmap m_bmpBackground;
   CBitmap m_bmpBtnFrame2;
   CBitmap m_bmpBtnFrame3;
   CBitmap m_bmpBtnFrame4;

   GUILocalString m_Language;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__8F54F664_4474_4D15_B412_6DDA5B9D3569__INCLUDED_)
