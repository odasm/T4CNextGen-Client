//--FILE DEFINITION-----------------------------------------------------------


#if !defined(AFX_LABEL_H__8A00C0CE_F998_11D4_88EA_204C4F4F5020__INCLUDED_)
#define AFX_LABEL_H__8A00C0CE_F998_11D4_88EA_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//--LOCAL DEFINITIONS----------------------------------------------------------
#define FLASHRATE 500
#define	TIMERID 22

//--CLASS DEFINITION----------------------------------------------------------
//
//  Class name:   uicLabel
//
/*! Notes:        SubClass for CStatic Text Control 
*/
//----------------------------------------------------------------------------
class uicLabel : public CStatic
{
public:
//--IN-CLASS DEFINITIONS--------------------------------------------------

   static enum Type3D {Raised, Sunken };

//--CONSTRUCTORS-----------------------------------
	uicLabel();

//--Attributes-------------------------------------
public:

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLabel)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	//--PUBLIC METHODS----------------------------
	virtual uicLabel& SetText(UINT _nID);
	virtual uicLabel& SetText(LPCTSTR _szText);
   virtual uicLabel& SetTextLink(LPCTSTR _szText);
	virtual uicLabel& SetFontName(LPCTSTR _szFontName);
	virtual uicLabel& SetFontBold(BOOL _bBold=FALSE);
	virtual uicLabel& SetFontSize(int _iSize);
	virtual uicLabel& SetForeColor(COLORREF _crColorText);
	virtual uicLabel& SetBackColor(COLORREF _crBkColor);
	virtual uicLabel& SetBorder(BOOL _bSet=FALSE);
	virtual uicLabel& SetFont3D(BOOL _bSet, Type3D _Type = Raised);
	virtual uicLabel& SetVerticalEffect(BOOL _bSet=FALSE);
	virtual uicLabel& SetSunken(BOOL _bSet=FALSE);
	virtual uicLabel& SetLinkEffect(BOOL _bSet, UINT _uiCursorID= NULL, BOOL _bKeepForColor= FALSE);
	virtual uicLabel& FlashText(BOOL _bSet=FALSE);
  	virtual uicLabel& LangueFix();
   virtual uicLabel& SetCenter(BOOL _bSet=TRUE);
	CString GetText();

	//--DESTRUCTOR--------------------------------------
	virtual ~uicLabel();

	// Generated message map functions
protected:
	//{{AFX_MSG(uicLabel)
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	//--PRIVATE ATTRIBUTES------------------------------
	BOOL			m_bForeColor;
	BOOL			m_bVertical;
   BOOL        m_bCenter;
	COLORREF		m_3DHiliteColor;
	Type3D		m_3dType;
	BOOL			m_bFont3D;
	BOOL			m_bLink;
	HCURSOR		m_hCursor;
	CString		m_szTempText;
	UINT			m_uiCaption;
	BOOL			m_bFlashTextState;
	BOOL			m_bTextFlash;
	CString		m_szText;			//	Caption
	CBrush		m_BackBrush;		//	Back Color by user
	COLORREF		m_ForeColor;		//	Fore Color
	CFont			m_Font;				//	Wrapper class GDI
	LOGFONT		m_LogFont;			//	LOGFONT structure
   CString		m_szTextLink;			//	Caption

	//--PRIVATE METHODS---------------------------------
	void UpdateSurface();
	void RebuildFont();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABEL_H__8A00C0CE_F998_11D4_88EA_204C4F4F5020__INCLUDED_)
