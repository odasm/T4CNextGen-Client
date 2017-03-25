#if !defined(AFX_MACRODIALOG_H__5C360413_862A_41FD_8022_B56E3B0EBCBC__INCLUDED_)
#define AFX_MACRODIALOG_H__5C360413_862A_41FD_8022_B56E3B0EBCBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacroDialog.h : header file
//
#include "resource.h" 
#include <afxtempl.h> 
#include "GUILocalString.h"
#include "Controls\Dib.h"
#include "Controls\Common.h"
#include "Controls\ColoredListCtrl.h"


typedef struct _sConnectParam
{
   char strName [50];
   char strIP   [50];
   char strLogin[50];
   char strPass [50];
}sConnectParam;


typedef struct _sLauncherParam
{
   char strAppPath[512];
   int dwNbrConnection;
   CArray <sConnectParam,sConnectParam> pConnection;
   int dwNbrColor;
   CArray <int,int> CCColor;
   int  dwSelectedServer;
   BOOL bWindowed;
   BOOL bResolution;
   BOOL bLauncherMusic;
   int  dwLanguage;
   BOOL bMacro[12];
   BYTE chMacroKey[12];
}sLauncherParam;


/////////////////////////////////////////////////////////////////////////////
// CMacroDialog dialog

class CMacroDialog : public CDialog
{
// Construction
public:
	CMacroDialog(CWnd* pParent = NULL);   // standard constructor
   ~CMacroDialog();

// Dialog Data
	//{{AFX_DATA(CMacroDialog)
	enum { IDD = IDD_MACRO_DLG };
	CColoredListCtrl	m_ListColor;
	BOOL	m_bCharacter;
	BOOL	m_bChatter;
	BOOL	m_bGroup;
	BOOL	m_bInventory;
	BOOL	m_bMacro;
	BOOL	m_bOption;
	BOOL	m_bScreenShot;
	BOOL	m_bSpell;
	BOOL	m_bTrade;
	CString  m_chCharacter;
	CString  m_chChatter;
	CString  m_chGroup;
	CString	m_chInventory;
	CString	m_chMacro;
	CString	m_chOption;
	CString	m_chScreenShot;
	CString	m_chSpell;
	CString	m_chTrade;
	CString	m_chSwitchChat;
	BOOL	m_bSwitchChat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacroDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMacroDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChkInventory();
	afx_msg void OnChkCharacterui();
	afx_msg void OnChkChatterui();
	afx_msg void OnChkGroupui();
	afx_msg void OnChkSpellui();
	afx_msg void OnChkMacroui();
	afx_msg void OnChkTrade();
	afx_msg void OnChkOption();
	afx_msg void OnChkScreenshot();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnBtnMod();
	afx_msg void OnBtnRestaureDefaultColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
   void ProcessMessage() ;
   BOOL IsInit(){return m_bINIT;}
   void SetServerList(sLauncherParam *spLauncherConfig){m_spLauncherConfig = spLauncherConfig;};
   //void SetPath(char *pstrPath){m_strPath.Format("%s",pstrPath);};

protected:
   BOOL m_bINIT;
   //CString m_strPath;

   sLauncherParam *m_spLauncherConfig;
   GUILocalString m_Language;


   void SetDIBPalette();
   void CalcRects();
   void DrawHSB(CDC *pDC);
   void DrawMarkers(CDC *pDC);
   void TrackPoint(CPoint pt);
   BOOL InCircle(CPoint pt);
	BOOL InBright(CPoint pt);
   void LoadMappedBitmap(CBitmap& bitmap,UINT nIdResource,CSize& size);
   void CreateBrightDIB();
   void DrawFilledColor(CDC *pDC,CRect cr,COLORREF c);
   void DrawAll();
   void DrawListColor();
   
   
   
   CDIB m_BrightDIB;
   CDC memDC;

   CPoint m_Centre;
   CPoint Vertex,Top,Left,Right;

   int m_nMouseIn;
   int hsbWidth,hsbHeight;
   int nIndex;
	int RedLen,GreenLen,BlueLen;
   CBitmap m_HsbBitmap;
   
   CRect hsbRect;
   CRect brightRect;
   CRect m_CurrentRect,brightMark;
   CRect OldColorRect,NewColorRect;

   HSVType hsvColor;	
	RGBType color, m_OldColor;
	
   BOOL m_bInitOver,m_bInDrawAll,m_bInMouse;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACRODIALOG_H__5C360413_862A_41FD_8022_B56E3B0EBCBC__INCLUDED_)
