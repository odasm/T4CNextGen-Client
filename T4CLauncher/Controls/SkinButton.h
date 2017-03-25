//--FILE DEFINITION-----------------------------------------------------------
//
/*! \file
   
   Method(Public)       Description
   ------------------   --------------------------------------	
   SetBackColor         BackGround Button Color
   SetForeColor         Fore Color text
   SetText              Text Caption (CString)
   SetFontSize          The Font Size 
   SetFontBold          Bold or Normal
   SetFontName          The Font Name
   SetFont3D            Text Effect Raised, Sunken, Normal
   SetToolTipText       ToolTip  Text
   SetIcon              Add Icon form Resource
   SetIconAlign         How the Icon should be aligned.
   Refresh              Ask control to redraw.
   SetScale             Dimension of Pict to Scale.

   -------------------------------------------------
   If you are using the Graphic Option (2 Methods)
   -------------------------------------------------

   SetBitmap            Simple Picture Graphic Button

   SetScale             For Strech Effect on Bitmap Bouton
                        Use as % 100 = 1 = Original Size

   Modifications:

   Name        Date        Notes
 
*/
//----------------------------------------------------------------------------
#if !defined(AFX_SKINBUTTON_H__5C9D8DD3_150E_11D5_8923_204C4F4F5020__INCLUDED_)
#define AFX_SKINBUTTON_H__5C9D8DD3_150E_11D5_8923_204C4F4F5020__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//--CLASS DEFINITION----------------------------------------------------------
//
//  Class name:   CSkinButton
//
/*! Notes:        Subclass Button Control for MFC    
*/
//----------------------------------------------------------------------------
class CSkinButton : public CButton
{
public:
   //--CONSTRUCTOR-------------------------------------
	CSkinButton();
   //--DESTRUCTOR--------------------------------------
   virtual ~CSkinButton();

// Attributes
public:

   //--IN-CLASS DEFINITIONS------------------------------------------------------
   enum Type3D 
   {Raised, Sunken};

   enum IconAlign 
   {Horizontal, Vertical};
  
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

public:
   //--PUBLIC METHODS----------------------------------
  	void           Refresh();
   CSkinButton&   ActivateToolTip(BOOL _bActive = TRUE);
   CSkinButton&   SetIconAlign(IconAlign _Type = Horizontal);
   CSkinButton&   SetScale(int _XScale = 100, int _YScale = 100);
   CSkinButton&   SetToolTipText(LPCTSTR _szText);
   CSkinButton&   SetText(UINT _nTextID);
   CSkinButton&   SetForeColor(COLORREF _nColor);
   CSkinButton&   SetText(LPCTSTR _szText);
   CSkinButton&   SetFontName(LPTSTR _szFontName);
   CSkinButton&   SetFontSize(int _nSize);
   CSkinButton&   SetFont3D(BOOL _bSet, Type3D _Type = Raised);
   CSkinButton&   SetBitmap(CBitmap *pBitMap);
   CSkinButton&   SetIcon(UINT _nIconID);
   CSkinButton&   SetToolTipText(UINT _nID);
   CSkinButton&   SetFontBold(BOOL _bVal);
   CSkinButton&   SetBackColor(COLORREF _nColor);
   
	// Generated message map functions
protected:
   //{{AFX_MSG(CSkinButton)
   afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSysColorChange();
   afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
   afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

private:
   //--PRIVATE ATTRIBUTES------------------------------
   BOOL     m_bIsDefault;
   CBitmap* m_pBmpOldBk;
   int      m_YScale;
   int      m_XScale;
   BOOL     m_bStrech;
   BOOL     m_bUseBitmap;
   BOOL     m_bHover;
   BOOL     m_bCursorOnButton;
   CBitmap  *m_pButton;
   CBitmap  m_DisablePict;
   CBitmap  m_MaskBitmap;
   int      m_nWidth;         //Stretch
	int      m_nHeight;
   int      m_Width;          //Button Size
   int      m_Height;

   CDC      m_dcBk;
   CBitmap  m_BmpBk;
   int      m_nAlign;
   BYTE     m_cXicon;
   BYTE     m_cYicon;
   HICON    m_hIcon;
   COLORREF m_3DHiliteColor;
   Type3D   m_3dType;
   BOOL     m_bFont3D;
   CToolTipCtrl m_ToolTip;
   BOOL     m_bForeColor;
   BOOL     m_bBackColor;
   CString  m_szText;
   LOGFONT  m_LogFont;
   CFont    m_Font;
   COLORREF m_ForeColor;
   COLORREF m_BackColor;

   //--PRIVATE METHODS---------------------------------
   void  BuildDisable(CDC* _pDC, CBitmap& _BmpSource, CBitmap* _pBmpDisable);
   void  StroutText(CDC* _pDC, CRect _ItemRect, BOOL _bIsDisabled);
   void  GrabClient(CDC* _pDC);
   void  DrawTransparentBitmap(CDC* _pDC, CDC* _pTmpDC, 
                              CBitmap* _pBmpMask, int _iSelected);
                                        
   void  PrepareMask(CBitmap& _BmpSource, CBitmap* _pBmpMask);
   void  DrawIcon(CDC* _pDC, RECT* _prcItem, CRect* _pCaptionrect, 
                  BOOL _bIsPressed, BOOL _bIsDisabled);
   void  InitToolTip();
   void  RebuildFont();

};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINBUTTON_H__5C9D8DD3_150E_11D5_8923_204C4F4F5020__INCLUDED_)
