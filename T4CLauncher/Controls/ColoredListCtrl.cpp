// ColoredListCtrl.cpp : implementation file
//

#include "..\StdAfx.h"
#include "ColoredListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColoredListCtrl

CColoredListCtrl::CColoredListCtrl()
{
}

CColoredListCtrl::~CColoredListCtrl()
{
}


BEGIN_MESSAGE_MAP(CColoredListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CColoredListCtrl)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColoredListCtrl message handlers

void CColoredListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
  *pResult = 0;

  LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
  int iRow = lplvcd->nmcd.dwItemSpec;

  switch(lplvcd->nmcd.dwDrawStage)
  {
    case CDDS_PREPAINT :
    {
      *pResult = CDRF_NOTIFYITEMDRAW;
      return;
    }

    // Modify item text and or background
    case CDDS_ITEMPREPAINT:
    {
      lplvcd->clrText = RGB(0,0,0);
      // If you want the sub items the same as the item,
      // set *pResult to CDRF_NEWFONT
      *pResult = CDRF_NOTIFYSUBITEMDRAW;
      return;
    }

    // Modify sub item text and/or background
    case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
       {
          CString s;
          
          s = GetItemText(iRow,0);
          int dwColor;
          sscanf( s.GetBuffer(0), "0x%06X", &dwColor );
          
          lplvcd->clrTextBk = RGB(GetRValue(dwColor),GetGValue(dwColor),GetBValue(dwColor));
          
          *pResult = CDRF_DODEFAULT;
          return;
       }
  }
}

BOOL CColoredListCtrl::OnEraseBkgnd(CDC* pDC) 
{
   // TODO: Add your message handler code here and/or call default
   CRect rect;
   CColoredListCtrl::GetClientRect(rect);
   
   CBrush brush0(RGB(0,0,0));
  
   int chunk_height=GetCountPerPage();
   pDC->FillRect(&rect,&brush0);
   brush0.DeleteObject();
   return FALSE;
}
