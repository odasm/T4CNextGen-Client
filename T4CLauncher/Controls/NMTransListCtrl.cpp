// NMTransListCtrl.cpp : implementation file
//

#include "..\stdafx.h"
#include "NMTransListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNMTransListCtrl

CNMTransListCtrl::CNMTransListCtrl()
{
   m_crText         = RGB(255,0,255);
   m_crSelectedText = RGB(0,0,255);
   m_crBack         = RGB(0,0,0);

   m_dwBGBitmapFormat   = LVNF_BGTILE;
   m_dwItemFormat       = LVNF_FMT_LEFT;
   m_dwRowHeight        = -1;

   m_bSortAscending = TRUE;
   m_bLockDraw      = FALSE;
   m_bTrans         = FALSE;
   m_bColorBack     = FALSE;
}

CNMTransListCtrl::~CNMTransListCtrl()
{
   m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CNMTransListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CNMTransListCtrl)
	ON_WM_ERASEBKGND()
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_WM_DESTROY()
	ON_WM_MEASUREITEM_REFLECT( )
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNMTransListCtrl message handlers


BOOL CNMTransListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style &= ~LVS_TYPEMASK;
   cs.style |= LVS_REPORT | LVS_OWNERDRAWFIXED;

	return CListCtrl::PreCreateWindow(cs);
}

void CNMTransListCtrl::PreSubclassWindow() 
{
   GetFont()->GetObject(sizeof(m_LogFont), &m_LogFont);
   RebuildFont();	
	CListCtrl::PreSubclassWindow();
}

BOOL CNMTransListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	if(m_BGBitmap.m_hObject == NULL)
   {
      CListCtrl::OnEraseBkgnd(pDC);
      return 0 ;
   }

   if(m_bLockDraw)
   {
      Invalidate(FALSE);
   }
   return 0;
}

void CNMTransListCtrl::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
   *pResult = -1;
   switch(pLVKeyDow->wVKey)
   {
      case VK_RIGHT:
         PostMessage(WM_HSCROLL,MAKELONG(SB_LINERIGHT,0),NULL);
      break;
      case VK_LEFT:
         PostMessage(WM_HSCROLL,MAKELONG(SB_LINELEFT,0),NULL);
      break;
      case VK_DOWN:
      {
         int nOldItem = GetNextItem(-1,LVNI_FOCUSED );
         int nItem    = min(nOldItem+1,GetItemCount()-1);
         
         if(nItem != nOldItem)
         {
            UINT uiStateSel = ListView_GetItemState(this->m_hWnd,nItem,LVIS_SELECTED );
            UINT uiStateFoc = ListView_GetItemState(this->m_hWnd,nItem,LVIS_FOCUSED  );
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateSel,LVIS_SELECTED);
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateFoc,LVIS_FOCUSED);
         }
         
         SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
         if(GetTopIndex()+GetCountPerPage()<=nItem)
            PostMessage(WM_VSCROLL,MAKELONG(SB_LINERIGHT,0),NULL);
      }
      break;
      case VK_UP:
      {
         int nOldItem = GetNextItem(-1,LVNI_FOCUSED );
         int nItem    = max(nOldItem-1,0);
         if(nItem != nOldItem)
         {
            UINT uiStateSel = ListView_GetItemState(this->m_hWnd,nItem,LVIS_SELECTED );
            UINT uiStateFoc = ListView_GetItemState(this->m_hWnd,nItem,LVIS_FOCUSED  );
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateSel,LVIS_SELECTED);
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateFoc,LVIS_FOCUSED);
         }
         
         SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
         if(GetTopIndex()>nItem)
            PostMessage(WM_VSCROLL,MAKELONG(SB_LINELEFT,0),NULL);
      }
      break;
      case VK_NEXT:
      {
         int nOldItem = GetNextItem(-1,LVNI_FOCUSED );
         int nItem    = min(nOldItem+GetCountPerPage(),GetItemCount()-1);
         if(nItem != nOldItem)
         {
            UINT uiStateSel = ListView_GetItemState(this->m_hWnd,nItem,LVIS_SELECTED );
            UINT uiStateFoc = ListView_GetItemState(this->m_hWnd,nItem,LVIS_FOCUSED  );
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateSel,LVIS_SELECTED);
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateFoc,LVIS_FOCUSED);
         }
         
         SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
         if(GetTopIndex()+GetCountPerPage()<=nItem)
            PostMessage(WM_VSCROLL,MAKELONG(SB_PAGERIGHT,0),NULL);
      }
      break;
      case VK_PRIOR:
      {
         int nOldItem = GetNextItem(-1,LVNI_FOCUSED );
         int nItem    = max(nOldItem-GetCountPerPage(),0);
         if(nItem != nOldItem)
         {
            UINT uiStateSel = ListView_GetItemState(this->m_hWnd,nItem,LVIS_SELECTED );
            UINT uiStateFoc = ListView_GetItemState(this->m_hWnd,nItem,LVIS_FOCUSED  );
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateSel,LVIS_SELECTED);
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateFoc,LVIS_FOCUSED);
         }
         
         
         SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
         if(GetTopIndex()>nItem)
            PostMessage(WM_VSCROLL,MAKELONG(SB_PAGELEFT,0),NULL);
      }
      break;
      case VK_END:
      {
         int nOldItem = GetNextItem(-1,LVNI_FOCUSED );
         int nItem    = GetItemCount()-1;
         if(nItem != nOldItem)
         {
            UINT uiStateSel = ListView_GetItemState(this->m_hWnd,nItem,LVIS_SELECTED );
            UINT uiStateFoc = ListView_GetItemState(this->m_hWnd,nItem,LVIS_FOCUSED  );
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateSel,LVIS_SELECTED);
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateFoc,LVIS_FOCUSED);
         }
         
         SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
         if(GetTopIndex()+GetCountPerPage()<=nItem)
            PostMessage(WM_VSCROLL,MAKELONG(SB_BOTTOM,0),NULL);
      }
      break;
      case VK_HOME:
      {
         int nOldItem = GetNextItem(-1,LVNI_FOCUSED );
         int nItem    = 0;
         if(nItem != nOldItem)
         {
            UINT uiStateSel = ListView_GetItemState(this->m_hWnd,nItem,LVIS_SELECTED );
            UINT uiStateFoc = ListView_GetItemState(this->m_hWnd,nItem,LVIS_FOCUSED  );
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateSel,LVIS_SELECTED);
            ListView_SetItemState(this->m_hWnd,nOldItem,uiStateFoc,LVIS_FOCUSED);
         }
         
         SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
         if(GetTopIndex()>0)
            PostMessage(WM_VSCROLL,MAKELONG(SB_TOP,0),NULL);
      }
      break;
   }
   //*pResult = 0;
}

void CNMTransListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(m_BGBitmap.m_hObject != NULL)
      Invalidate(FALSE);
	
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CNMTransListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(m_BGBitmap.m_hObject != NULL)
      Invalidate(FALSE);
	
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CNMTransListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(m_BGBitmap.m_hObject != NULL)
      Invalidate(FALSE);
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}


void CNMTransListCtrl::OnPaint() 
{
	if(m_BGBitmap.m_hObject == NULL && !m_bTrans)
   {
      CListCtrl::OnPaint();
      return;
   }

   if(m_bLockDraw)
      return;

  
   CPaintDC dc(this);
   CRect rcClip, rcClient,headerRect;
   
   GetWindowRect(&rcClient);
   ScreenToClient(&rcClient);

   RECT r;
   HWND hwnd = ListView_GetHeader(this->GetSafeHwnd());
   ::GetWindowRect(hwnd, &r);
   headerRect = r;
   
   dc.GetClipBox(&rcClip);
   rcClip.top  =  max(headerRect.Height(),rcClip.top);

   CDC memDC;
   memDC.CreateCompatibleDC(&dc);
   CBitmap bitmap, bmpImage;

   bitmap.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
   memDC.SelectObject(&bitmap);
   memDC.SelectStockObject(DEFAULT_GUI_FONT);

   if(m_bTrans)
   {
      if(m_bColorBack)
      {
         memDC.FillRect(rcClient, &CBrush(m_crBack));
      }
      else
      {
         if(m_BGBitmap.m_hObject != NULL)
         {
            if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE &&
               m_BGBitmapPalette.m_hObject != NULL)
            {
               dc.SelectPalette(&m_BGBitmapPalette,FALSE);
               dc.RealizePalette();
               memDC.SelectPalette(&m_BGBitmapPalette,FALSE);
            }
            memDC.FillRect(rcClient, &CBrush(RGB(255,255,255)));
         }
      }
      if(!m_bColorBack && m_BGBitmap.m_hObject != NULL)
      {
         CDC dcBitmap;
         dcBitmap.CreateCompatibleDC(&dc);
         dcBitmap.SelectObject(&m_BGBitmap);

         memDC.BitBlt( rcClient.left, 
                       rcClient.top, 
                       rcClient.Width(), 
                       rcClient.Height(), 
                       &dcBitmap, 
                       m_dwSXpos, 
                       m_dwSYpos, 
                       SRCCOPY);
      }
      if(GetItemCount()>0)
      {
         DRAWITEMSTRUCT DrawItemStruct;
         RECT rectItem;

         int dwFirstItem = GetTopIndex();
         int dwLastItem = dwFirstItem + GetCountPerPage();
         if(dwLastItem >= GetItemCount())
            dwLastItem = GetItemCount();
   
         DrawItemStruct.CtlType = ODT_LISTVIEW;
         DrawItemStruct.hDC =   memDC.GetSafeHdc();
         DrawItemStruct.itemAction = ODA_DRAWENTIRE; 
         DrawItemStruct.hwndItem = GetSafeHwnd(); 

         for(int i=dwFirstItem;i<dwLastItem;i++)
         {
            GetItemRect(i,&rectItem,LVIR_BOUNDS);
            DrawItemStruct.rcItem = rectItem;
            DrawItemStruct.itemID = i;
            DrawItem(&DrawItemStruct);
         }
      }
      dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(),
                 &memDC, rcClip.left,rcClip.top, SRCCOPY );

   }
   else if(m_BGBitmap.m_hObject != NULL)
   {
      if(dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE &&
         m_BGBitmapPalette.m_hObject != NULL)
      {
         dc.SelectPalette(&m_BGBitmapPalette,FALSE);
         dc.RealizePalette();
         memDC.SelectPalette(&m_BGBitmapPalette,FALSE);
      }
   
      memDC.FillRect(rcClient, &CBrush(RGB(255,255,255)));

      if(GetItemCount()>0)
      {
         DRAWITEMSTRUCT DrawItemStruct;
         RECT rectItem;

         int dwFirstItem = GetTopIndex();
         int dwLastItem = dwFirstItem + GetCountPerPage();
         if(dwLastItem >= GetItemCount())
            dwLastItem = GetItemCount();
   
         DrawItemStruct.CtlType = ODT_LISTVIEW;
         DrawItemStruct.hDC =   memDC.GetSafeHdc();
         DrawItemStruct.itemAction = ODA_DRAWENTIRE; 
         DrawItemStruct.hwndItem = GetSafeHwnd(); 

         for(int i=dwFirstItem;i<dwLastItem;i++)
         {
            GetItemRect(i,&rectItem,LVIR_BOUNDS);
            DrawItemStruct.rcItem = rectItem;
            DrawItemStruct.itemID = i;
            DrawItem(&DrawItemStruct);
         }
      }
      CDC dcBitmap;
      dcBitmap.CreateCompatibleDC(&dc);
      dcBitmap.SelectObject(&m_BGBitmap);
   
      switch (m_dwBGBitmapFormat)
      {
         case LVNF_BGCENTER:
         {
            CPoint Point = rcClient.CenterPoint();
            Point.Offset(-m_dwBGBitmapCX/2,-m_dwBGBitmapCY/2);
            memDC.BitBlt( Point.x, 
                          Point.y, 
                          m_dwBGBitmapCX, 
                          m_dwBGBitmapCY, 
                          &dcBitmap, 
                          0, 
                          0, 
                          SRCAND);
         }
         break;
         case LVNF_BGTILE:
         {
            for(int i= rcClient.left;i<rcClient.right;i+=
               m_dwBGBitmapCX)
               for(int j = rcClient.top;j<rcClient.bottom;j+=m_dwBGBitmapCY)
                  memDC.BitBlt(i,j,m_dwBGBitmapCX,m_dwBGBitmapCY,&dcBitmap,0,0,SRCAND);
         }
         break;
         default:
            break;
      }
   
      dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(),
                 &memDC, rcClip.left,rcClip.top, SRCCOPY );
   }
   memDC.DeleteDC();
   //ReleaseDC(&dc);
}

void CNMTransListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
   NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

   if(pNMListView->iItem == 0)
      Update(pNMListView->iItem);
   
	*pResult = 0;
}

void CNMTransListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
   if (m_dwRowHeight == -1)
   {
	   TEXTMETRIC tm;
	   HDC hDC = ::GetDC(NULL);
	   CFont* pFont = GetFont();
	   HFONT hFontOld = (HFONT)SelectObject(hDC, pFont->GetSafeHandle());
	   GetTextMetrics(hDC, &tm);
	   lpMeasureItemStruct->itemHeight = tm.tmHeight + tm.tmExternalLeading + 1;
	   SelectObject(hDC, hFontOld);
	   ::ReleaseDC(NULL, hDC);
   }
   else
   {
		lpMeasureItemStruct->itemHeight = m_dwRowHeight; 
   }
}

void CNMTransListCtrl::OnDestroy() 
{
   // Detruit tout les items ratachés.
   DeleteAllItems();

   CImageList *pImageList;
   pImageList = GetImageList(LVSIL_NORMAL);
   if (pImageList)
      delete pImageList;
   pImageList = GetImageList(LVSIL_SMALL);
   if (pImageList)
      delete pImageList;
   pImageList = GetImageList(LVSIL_STATE);
   if (pImageList)
      delete pImageList;
   CListCtrl::OnDestroy();
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
///
///
///
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void CNMTransListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
   CDC*   pDC;
   int    dwSavedDC;
	int    dwItem;
	LVITEM lvi;

   CRect rcItem;
	CRect rcBounds;
   CRect rcLabel;
   CRect rcIcon;
   CRect rcCol;

   CImageList     *pImageList;

   pImageList = GetImageList(LVSIL_NORMAL);

   rcItem     = lpDrawItemStruct->rcItem;
   dwItem     = lpDrawItemStruct->itemID;

   pDC       = CDC::FromHandle(lpDrawItemStruct->hDC);
   dwSavedDC = pDC->SaveDC();

	// Get item image and state info
	lvi.mask      = LVIF_STATE | LVIF_PARAM;
	lvi.iItem     = dwItem;
	lvi.iSubItem  = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
   GetItem(&lvi);
   
   // Should the item be highlighted
	BOOL bHighlight =	
        ((lvi.state & LVIS_DROPHILITED) || 
        ((lvi.state & LVIS_SELECTED) && 
        ((GetFocus() == this) || (GetStyle() & LVS_SHOWSELALWAYS) ) ) );

	// Get rectangles for drawing
	GetItemRect(dwItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(dwItem, rcLabel,  LVIR_LABEL);
	GetItemRect(dwItem, rcIcon,   LVIR_ICON);
	
   rcCol = rcBounds; 

	CString sLabel = GetItemText( dwItem, 0 );

	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character
	int offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2;

	CRect rcHighlight;
	CRect rcClient;

	rcHighlight = rcCol;

	// Set la couleut du texte, selon si ye selectionner ou pas...
	if( bHighlight )
	{
		pDC->SetTextColor(RGB(255,255,255));
      pDC->FillSolidRect(rcHighlight,m_crSelectedText);
	}
   else
   {
  		pDC->SetTextColor(m_crText);
   }

   

	CRgn rgn;
	rgn.CreateRectRgnIndirect(&rcCol);
	pDC->SelectClipRgn(&rgn);
	rgn.DeleteObject();

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;
   
   int dwCount    = GetHeaderCtrl()->GetItemCount();
   int dwImageOffset = 0;
   for(int nColumn=0 ; GetColumn(nColumn, &lvc) ; nColumn++)
	{
      rcCol.right = rcCol.left + lvc.cx;
      
      CString sLabel = GetItemText(dwItem, nColumn);

      if(!bHighlight)
      {
         pDC->SetTextColor(m_crText);
         pDC->SetBkMode(TRANSPARENT);
      }
      else
      {
		   pDC->SetTextColor(RGB(255,255,255));
         pDC->SetBkMode(TRANSPARENT);
      }

      if (lvi.state & LVIS_STATEIMAGEMASK)
      {
         int nImage = ((lvi.state & LVIS_STATEIMAGEMASK)>>12) - 1;
		   if (pImageList)
		   {
			   pImageList->Draw(pDC, nImage,CPoint(rcCol.left, rcCol.top), ILD_TRANSPARENT);
		   }
         IMAGEINFO ImageInfo;
         int value = pImageList->GetImageInfo(nImage,&ImageInfo);
         dwImageOffset = ImageInfo.rcImage.right - ImageInfo.rcImage.left;
      }
      else
      {
         dwImageOffset = 0;
      }

		if (sLabel.GetLength() != 0)
      {
		   // Get the text justification
		   UINT nJustify = DT_LEFT;
         int dwSubItemFormat;
         CString strDigit;
         CString strDecimal;

         rcLabel       =  rcCol;
		   rcLabel.left  += offset + dwImageOffset;
		   rcLabel.right -= offset;
         
         dwSubItemFormat = m_dwItemFormat;

         CFont* pOldFont=NULL;
		   pOldFont= pDC->SelectObject(&m_Font);
		   
         switch(dwSubItemFormat)
		   {
		      case LVNF_FMT_RIGHT:
			      nJustify = DT_RIGHT;
               pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE 
                             | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
               break;
		      case LVNF_FMT_CENTER:
			      nJustify = DT_CENTER;
               pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE 
                             | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
               break;
		      case LVNF_FMT_LEFT:
			      nJustify = DT_LEFT;
               pDC->DrawText(sLabel, -1, rcLabel, nJustify | DT_SINGLELINE 
                             | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);
               break;
		      default:
               break;
		   }
         pDC->SelectObject(pOldFont);
      }
		rcCol.left += lvc.cx;
	}
	// Restore dc
	pDC->RestoreDC( dwSavedDC );
}

BOOL CNMTransListCtrl::bSetBGImage(UINT nIDResource,int dwBGBitmapFormat/* = LVNF_BGTILE*/)
{
   return bSetBGImage((LPCTSTR) nIDResource,dwBGBitmapFormat);
}

BOOL CNMTransListCtrl::bSetBGImage(LPCTSTR lpszResourceName,int dwBGBitmapFormat/* = LVNF_BGTILE*/)
{
	// If this is not the first call then Delete GDI objects
	if( m_BGBitmap.m_hObject != NULL )
		m_BGBitmap.DeleteObject();
	if( m_BGBitmapPalette.m_hObject != NULL )
		m_BGBitmapPalette.DeleteObject();
	
	
	HBITMAP hBmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(), 
                                        lpszResourceName, IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION );

	if( hBmp == NULL ) 
		return FALSE;

	m_BGBitmap.Attach( hBmp );
	BITMAP bm;
	m_BGBitmap.GetBitmap( &bm );
	m_dwBGBitmapCX     = bm.bmWidth;
	m_dwBGBitmapCY     = bm.bmHeight;
   m_dwBGBitmapFormat = dwBGBitmapFormat;

	// Create a logical palette for the bitmap
	DIBSECTION ds;
	BITMAPINFOHEADER &bmInfo = ds.dsBmih;
	m_BGBitmap.GetObject( sizeof(ds), &ds );

	int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

	// Create a halftone palette if colors > 256. 
	CClientDC dc(NULL);			// Desktop DC
	if( nColors > 256 )
		m_BGBitmapPalette.CreateHalftonePalette( &dc );
	else
	{
		// Create the palette

		RGBQUAD *pRGB = new RGBQUAD[nColors];
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		memDC.SelectObject( &m_BGBitmap );
		::GetDIBColorTable( memDC, 0, nColors, pRGB );

		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

		pLP->palVersion = 0x300;
		pLP->palNumEntries = nColors;

		for( int i=0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = pRGB[i].rgbRed;
			pLP->palPalEntry[i].peGreen = pRGB[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = pRGB[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}

		m_BGBitmapPalette.CreatePalette( pLP );

		delete[] pLP;
		delete[] pRGB;
	}
	Invalidate(FALSE);

	return TRUE;

}

BOOL CNMTransListCtrl::bSetBGImageFile(CString strFile ,int dwBGBitmapFormat/* = LVNF_BGTILE*/)
{
	// If this is not the first call then Delete GDI objects
	if( m_BGBitmap.m_hObject != NULL )
		m_BGBitmap.DeleteObject();
	if( m_BGBitmapPalette.m_hObject != NULL )
		m_BGBitmapPalette.DeleteObject();
	
   m_dwBGBitmapFormat = dwBGBitmapFormat;
   
   LoadBMPImage(strFile,m_BGBitmap,&m_BGBitmapPalette);
   BITMAP bm;
	m_BGBitmap.GetBitmap( &bm );
	m_dwBGBitmapCX     = bm.bmWidth;
	m_dwBGBitmapCY     = bm.bmHeight;
   
	Invalidate(FALSE);

	return TRUE;
}

BOOL CNMTransListCtrl::bSetBGImageTrans(BOOL bColor,COLORREF crBack,CString strBmp,int dwXPos,int dwYPos)
{
   m_bTrans = TRUE;

   if( m_BGBitmap.m_hObject != NULL )
		m_BGBitmap.DeleteObject();
	if( m_BGBitmapPalette.m_hObject != NULL )
		m_BGBitmapPalette.DeleteObject();
   
   m_bColorBack = bColor;
   m_crBack     = crBack;

   if(!m_bColorBack)
   {
      BOOL bOK = LoadBMPImage(strBmp,m_BGBitmap,&m_BGBitmapPalette);
      if(!bOK)
      {
         m_crBack     = RGB(0,0,0);
         m_bColorBack = TRUE;
         return TRUE;
      }
      BITMAP bm;
	   m_BGBitmap.GetBitmap( &bm );
	   m_dwBGBitmapCX     = bm.bmWidth;
	   m_dwBGBitmapCY     = bm.bmHeight;

      m_dwSXpos   = dwXPos;
      m_dwSYpos   = dwYPos;
   }
   Invalidate(FALSE);
   return TRUE;
}

BOOL CNMTransListCtrl::bSetBGImageTransRes(BOOL bColor,COLORREF crBack,UINT dwCtrlID,int dwXPos,int dwYPos)
{
   m_bTrans = TRUE;

   if( m_BGBitmap.m_hObject != NULL )
		m_BGBitmap.DeleteObject();
	if( m_BGBitmapPalette.m_hObject != NULL )
		m_BGBitmapPalette.DeleteObject();
   
   m_bColorBack = bColor;
   m_crBack     = crBack;

   if(!m_bColorBack)
   {
      BOOL bOK = m_BGBitmap.LoadBitmap(dwCtrlID);
      if(!bOK)
      {
         m_crBack     = RGB(0,0,0);
         m_bColorBack = TRUE;
         return TRUE;
      }
      BITMAP bm;
	   m_BGBitmap.GetBitmap( &bm );
	   m_dwBGBitmapCX     = bm.bmWidth;
	   m_dwBGBitmapCY     = bm.bmHeight;

      m_dwSXpos   = dwXPos;
      m_dwSYpos   = dwYPos;
   }
   Invalidate(FALSE);
   return TRUE;
}

void CNMTransListCtrl::vSetRowHeight(int dwHeight)
{
	CRect rc;

   m_dwRowHeight = dwHeight;

   GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd  = m_hWnd;
	wp.cx    = rc.Width();
	wp.cy    = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
}


BOOL CNMTransListCtrl::LoadBMPImage( LPCTSTR sBMPFile, CBitmap& bitmap, CPalette *pPal )
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) )
		return FALSE;

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;

	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return FALSE;

	// Read the remainder of the bitmap file.
	if (file.Read((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		return FALSE;
	}


	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 
						1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	// Create the logical palette
	if( pPal != NULL )
	{
		// Create the palette
		if( nColors <= 256 )
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;

			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}

			pPal->CreatePalette( pLP );

			delete[] pLP;
		}
	}

	CClientDC dc(NULL);
	CPalette* pOldPalette = NULL;
	if( pPal )
	{
		pOldPalette = dc.SelectPalette( pPal, FALSE );
		dc.RealizePalette();
	}

	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC,		// handle to device context 
				&bmiHeader,	// pointer to bitmap size and format data 
				CBM_INIT,	// initialization flag 
				lpDIBBits,	// pointer to initialization data 
				&bmInfo,	// pointer to bitmap color-format data 
				DIB_RGB_COLORS);		// color-data usage 
	bitmap.Attach( hBmp );

	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );

	::GlobalFree(hDIB);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
//Fonction:      LoadImageDataToBmpFile(LPCTSTR lpszFileName,BYTE ** pData,
//                                      int *pdwWidth,int *pdwHeight,
//                                      int *pdwImageBytesPixel) 
//
//Descroption:   Load un fichier BMP et le place dans un buffer
//
//Param:         LPCTSTR lpszFileName     Le nom du fichier a loader
//               BYTE ** pData            Contiendra les data de l'image
//               int *pdwWidth            Contiendra la largeur de l'image
//               int *pdwHeight           Contiendra la hauteur de l'image
//               int *pdwImageBytesPixel  Contiendra le nombre de byte/pixel
//
//
//Retour:        TRUE  OK
//               NULL  ERREUR
//
//Notes:         Creation         
//               Carl Vachon     2002/04/16
//
////////////////////////////////////////////////////////////////////////////////
HRESULT CNMTransListCtrl::LoadImageDataToBmpFile(LPCTSTR lpszFileName,BYTE ** pData,int *pdwWidth,int *pdwHeight, int *pdwImageBytesPixel) 
{
   LPVOID lpBits;
   HANDLE hSection = NULL;
   DWORD  dwOffset = 0;
   
   CFile file;
   if( !file.Open( lpszFileName, CFile::modeRead) )
      return NULL;
   
   BITMAPFILEHEADER bmfHeader;
   long nFileLen;
   
   nFileLen = file.GetLength();
   
   // Read file header
   if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
      return NULL;
   
   // File type should be 'BM'
   if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
      return NULL;
   
   BITMAPINFO *pbmInfo;
   pbmInfo = (BITMAPINFO *)::GlobalAlloc(GMEM_FIXED, 
      sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256 );
   if (pbmInfo == NULL)
      return NULL;
   
   // Read the BITMAPINFO
   file.Read( pbmInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD)*256 );
   
   BITMAPINFO &bmInfo = *pbmInfo ;
   
   HBITMAP hBmp = CreateDIBSection( NULL, pbmInfo, DIB_RGB_COLORS, &lpBits, 
						hSection, dwOffset );
   
   LPBYTE  lpDIBBits;              // Pointer to DIB bits
   int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
   1 << bmInfo.bmiHeader.biBitCount;
   
   if( bmInfo.bmiHeader.biBitCount > 8 )
      lpDIBBits = (LPBYTE)((LPDWORD)(bmInfo.bmiColors + 
      bmInfo.bmiHeader.biClrUsed) + 
      ((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
   else
      lpDIBBits = (LPBYTE)(bmInfo.bmiColors + nColors);
   
   *pdwWidth = bmInfo.bmiHeader.biWidth;
   *pdwHeight = bmInfo.bmiHeader.biHeight;
   *pdwImageBytesPixel = (bmInfo.bmiHeader.biBitCount);

   
   int nOffset = sizeof(BITMAPFILEHEADER) + (lpDIBBits - (LPBYTE)pbmInfo);
   file.Seek( nOffset, CFile::begin);
   file.Read((LPSTR)lpBits, nFileLen - nOffset);
   
      
   ::GlobalFree(pbmInfo);
   
   BYTE *pDataTmp = new BYTE [nFileLen - nOffset];;
   memcpy(pDataTmp,lpBits,nFileLen - nOffset);


   // on recopie les donnee dans le buffer 32 bits
   if((*pData))
   {
      delete[](*pData);
   }
   (*pData) = new BYTE [((*pdwWidth) * (*pdwHeight))*4];

   if(*pdwImageBytesPixel == 32)
   {
      memcpy((*pData),pDataTmp,((*pdwWidth) * (*pdwHeight))*4);
   }
   if(*pdwImageBytesPixel == 24)
   {
      for(int y=(*pdwHeight)-1;y>=0;y--)
      {
         int dwCnt = 0;
         for(int x=0;x<((*pdwWidth)*4);x+=4)
         {
            (*pData)[(y*((*pdwWidth)*4))+x+0] = pDataTmp[((((*pdwHeight)-1)-y)*((*pdwWidth)*3))+dwCnt+2];
            (*pData)[(y*((*pdwWidth)*4))+x+1] = pDataTmp[((((*pdwHeight)-1)-y)*((*pdwWidth)*3))+dwCnt+1];
            (*pData)[(y*((*pdwWidth)*4))+x+2] = pDataTmp[((((*pdwHeight)-1)-y)*((*pdwWidth)*3))+dwCnt+0];
            (*pData)[(y*((*pdwWidth)*4))+x+3] = 0x00;
            dwCnt+=3;
         }
      }
   }
   else if(*pdwImageBytesPixel == 8)
   {
      for(int y=(*pdwHeight)-1;y>=0;y--)
      {
         int dwCnt = 0;
         for(int x=0;x<((*pdwWidth)*4);x+=4)
         {
            (*pData)[(y*((*pdwWidth)*4))+x+0] = pDataTmp[((((*pdwHeight)-1)-y)*((*pdwWidth)))+dwCnt];
            (*pData)[(y*((*pdwWidth)*4))+x+1] = pDataTmp[((((*pdwHeight)-1)-y)*((*pdwWidth)))+dwCnt];
            (*pData)[(y*((*pdwWidth)*4))+x+2] = pDataTmp[((((*pdwHeight)-1)-y)*((*pdwWidth)))+dwCnt];
            (*pData)[(y*((*pdwWidth)*4))+x+3] = 0x00;
            dwCnt++;
         }
      }
   }

   delete []pDataTmp;
   DeleteObject(hBmp);
   return TRUE;
} 


void CNMTransListCtrl::RebuildFont()
{
   //** Delete the Object
   m_Font.DeleteObject();
   //** Rebuild it
   m_Font.CreateFontIndirect(&m_LogFont);
}

void CNMTransListCtrl::SetFontBold(BOOL _bVal)
{
	m_LogFont.lfWeight = _bVal ? FW_BOLD: FW_NORMAL;
	RebuildFont();
	Invalidate();
}

void CNMTransListCtrl::SettextColor(COLORREF crtext,COLORREF crTextSel)
{
   m_crText         = crtext;
   m_crSelectedText = crTextSel;
   Invalidate();
}

void CNMTransListCtrl::SetFontName(LPTSTR _szFontName)
{
	m_LogFont.lfCharSet = DEFAULT_CHARSET;
	_tcscpy(m_LogFont.lfFaceName, _szFontName);
	RebuildFont();
   Invalidate();
}

void CNMTransListCtrl::SetFontSize(int _nSize)
{
   int  iTempSize = _nSize;

   iTempSize = _nSize < 6 ? 6: _nSize;
   iTempSize = _nSize > 72 ? 72: _nSize;
 
   CFont cf;
   LOGFONT lf;
   //** Create the Font
   cf.CreatePointFont(iTempSize * 10 ,m_LogFont.lfFaceName);
   cf.GetLogFont(&lf);

   //** Update Font Struct
   m_LogFont.lfHeight = lf.lfHeight;
   m_LogFont.lfWidth = lf.lfWidth;

   RebuildFont();
   Invalidate();
}

void CNMTransListCtrl::SetNextItem()
{
   int nOldItem = GetNextItem(-1,LVNI_FOCUSED );
   int nItem    = min(nOldItem+1,GetItemCount()-1);
   
   if(nItem != nOldItem)
   {
      UINT uiStateSel = ListView_GetItemState(this->m_hWnd,nItem,LVIS_SELECTED );
      UINT uiStateFoc = ListView_GetItemState(this->m_hWnd,nItem,LVIS_FOCUSED  );
      ListView_SetItemState(this->m_hWnd,nOldItem,uiStateSel,LVIS_SELECTED);
      ListView_SetItemState(this->m_hWnd,nOldItem,uiStateFoc,LVIS_FOCUSED);
   }
   
   SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
   if(GetTopIndex()+GetCountPerPage()<=nItem)
      PostMessage(WM_VSCROLL,MAKELONG(SB_LINERIGHT,0),NULL);
}

void CNMTransListCtrl::SetPreviousItem()
{
   int nOldItem = GetNextItem(-1,LVNI_FOCUSED );
   int nItem    = max(nOldItem-1,0);
   if(nItem != nOldItem)
   {
      UINT uiStateSel = ListView_GetItemState(this->m_hWnd,nItem,LVIS_SELECTED );
      UINT uiStateFoc = ListView_GetItemState(this->m_hWnd,nItem,LVIS_FOCUSED  );
      ListView_SetItemState(this->m_hWnd,nOldItem,uiStateSel,LVIS_SELECTED);
      ListView_SetItemState(this->m_hWnd,nOldItem,uiStateFoc,LVIS_FOCUSED);
   }
   
   SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
   if(GetTopIndex()>nItem)
      PostMessage(WM_VSCROLL,MAKELONG(SB_LINELEFT,0),NULL);
}

void CNMTransListCtrl::SetNextPage()
{
   int nOldItem = GetNextItem(-1,LVNI_FOCUSED );
   int nItem    = min(nOldItem+GetCountPerPage(),GetItemCount()-1);
   if(nItem != nOldItem)
   {
      UINT uiStateSel = ListView_GetItemState(this->m_hWnd,nItem,LVIS_SELECTED );
      UINT uiStateFoc = ListView_GetItemState(this->m_hWnd,nItem,LVIS_FOCUSED  );
      ListView_SetItemState(this->m_hWnd,nOldItem,uiStateSel,LVIS_SELECTED);
      ListView_SetItemState(this->m_hWnd,nOldItem,uiStateFoc,LVIS_FOCUSED);
   }
   
   SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
   if(GetTopIndex()+GetCountPerPage()<=nItem)
      PostMessage(WM_VSCROLL,MAKELONG(SB_PAGERIGHT,0),NULL);
}

void CNMTransListCtrl::SetPreviousPage()
{
   int nOldItem = GetNextItem(-1,LVNI_FOCUSED );
   int nItem    = max(nOldItem-GetCountPerPage(),0);
   if(nItem != nOldItem)
   {
      UINT uiStateSel = ListView_GetItemState(this->m_hWnd,nItem,LVIS_SELECTED );
      UINT uiStateFoc = ListView_GetItemState(this->m_hWnd,nItem,LVIS_FOCUSED  );
      ListView_SetItemState(this->m_hWnd,nOldItem,uiStateSel,LVIS_SELECTED);
      ListView_SetItemState(this->m_hWnd,nOldItem,uiStateFoc,LVIS_FOCUSED);
   }
   
   
   SetItemState(nItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);
   if(GetTopIndex()>nItem)
      PostMessage(WM_VSCROLL,MAKELONG(SB_PAGELEFT,0),NULL);
}

