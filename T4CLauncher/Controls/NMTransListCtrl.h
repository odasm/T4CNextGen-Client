#if !defined(AFX_NMTRANSLISTCTRL_H__3525E921_75DE_406C_A6FA_A0C755D851FC__INCLUDED_)
#define AFX_NMTRANSLISTCTRL_H__3525E921_75DE_406C_A6FA_A0C755D851FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NMTransListCtrl.h : header file
//

#include <afxtempl.h>

#define LVNF_BGTILE      22             // l'image en BG est tile
#define LVNF_BGCENTER    23             // l'image en BG est centre par la rapport a la zone client
#define LVNF_FMT_CENTER  24             // Item Centré
#define LVNF_FMT_RIGHT   25             // Item Justifié à droite
#define LVNF_FMT_LEFT    26             // Item Justifié à gauche


/////////////////////////////////////////////////////////////////////////////
// CNMTransListCtrl window

class CNMTransListCtrl : public CListCtrl
{
// Construction
public:
	CNMTransListCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNMTransListCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNMTransListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNMTransListCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
   afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	DECLARE_MESSAGE_MAP()
////////////////////////////
//   VARIABLE
////////////////////////////
public :

protected :
   // variable pour l'image en arriere plan...
   CPalette m_BGBitmapPalette;
   CBitmap  m_BGBitmap;
   CFont    m_Font;
   LOGFONT  m_LogFont;
   int      m_dwBGBitmapFormat;
   int      m_dwBGBitmapCX;
   int      m_dwBGBitmapCY;

   int      m_dwSXpos;
   int      m_dwSYpos;
   int      m_dwRowHeight;

   COLORREF m_crText;
   COLORREF m_crSelectedText;
   COLORREF m_crBack;

   BOOL     m_bSortAscending;
   BOOL     m_bLockDraw;
   BOOL     m_bTrans;
   BOOL     m_bColorBack;
   int      m_dwItemFormat;

private:


////////////////////////////
//   FONCTIONS
////////////////////////////
public :
   void vSetRowHeight(int dwHeight);
   BOOL bSetBGImage(LPCTSTR lpszResourceName,int dwBGBitmapFormat = LVNF_BGTILE);
   BOOL bSetBGImage(UINT nIDResource,int dwBGBitmapFormat = LVNF_BGTILE);
   BOOL bSetBGImageFile(CString strBmp,int dwBGBitmapFormat/* = LVNF_BGTILE*/);
   BOOL bSetBGImageTrans(BOOL bColor,COLORREF crBack,CString strBmp,int dwXPos,int dwYPos);
   BOOL bSetBGImageTransRes(BOOL bColor,COLORREF crBack,UINT dwCtrlID,int dwXPos,int dwYPos);

   void SetFontSize(int _nSize);
   void SetFontName(LPTSTR _szFontName);
   void SetFontBold(BOOL _bVal = FALSE);
   void SettextColor(COLORREF crtext,COLORREF crTextSel);

   void SetNextItem();
   void SetPreviousItem();
   void SetNextPage();
   void SetPreviousPage();

   virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

protected :
   void RebuildFont();
   BOOL LoadBMPImage( LPCTSTR sBMPFile, CBitmap& bitmap, CPalette *pPal );
   HRESULT LoadImageDataToBmpFile(LPCTSTR lpszFileName,BYTE ** pData,int *pdwWidth,int *pdwHeight, int *pdwImageBytesPixel);

private:



};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NMTRANSLISTCTRL_H__3525E921_75DE_406C_A6FA_A0C755D851FC__INCLUDED_)
