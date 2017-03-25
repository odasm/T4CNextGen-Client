//--FILE DEFINITION-----------------------------------------------------------
//
//----------------------------------------------------------------------------

//--FILE INCLUDES-------------------------------------------------------------

#include "..\stdafx.h"
#include "SkinButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//!--CONSTRUCTOR--------------------------------------------------------------
//
//  Method Name:  CSkinButton()
//
/*!
    Notes         Constructor.
*/
//-----------------------------------------------------------------------------
CSkinButton::CSkinButton()
{
   m_bIsDefault      = FALSE;
   m_bHover          = FALSE;
   m_bCursorOnButton	= FALSE;
   //** Color Background & Foreground default
   m_BackColor = ::GetSysColor(COLOR_BTNFACE);
   m_ForeColor = ::GetSysColor(COLOR_BTNTEXT);
   //** Color Back & Fore from system
   m_bBackColor =FALSE;
   m_bForeColor = FALSE;
   //** No ToolTip
   m_ToolTip.m_hWnd = NULL;
   InitToolTip();
   //** No 3D font
   m_bFont3D = FALSE;
   m_3dType = Raised;
   m_3DHiliteColor = RGB(255,255,255);
   //** Option Icon
   m_hIcon = NULL;
   m_cYicon= m_cXicon=0;
   m_nAlign = Horizontal;
   m_bUseBitmap = FALSE;
   m_bStrech = FALSE;
   m_pBmpOldBk = NULL;
   m_pButton = NULL;
}


//!--DESTRUCTOR-----------------------------------------------------------------
//
// Method Name:   ~CSkinButton()
//
/*!

   Notes          Destructor. Clean-up
*/			
//-----------------------------------------------------------------------------
CSkinButton::~CSkinButton()
{
   if ( m_hIcon )
      ::DeleteObject(m_hIcon);
   if( m_dcBk.m_hDC  && m_pBmpOldBk )
      m_dcBk.SelectObject(m_pBmpOldBk);
   if( m_ToolTip.m_hWnd )
      m_ToolTip.DestroyWindow();

   m_Font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSkinButton, CButton)
	//{{AFX_MSG_MAP(CSkinButton)
   ON_WM_MOUSEMOVE()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR_REFLECT()
   ON_WM_ENABLE()
   ON_MESSAGE(WM_MOUSEHOVER,OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////
//                         Private Functions
///////////////////////////////////////////////////////////////////////////


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   OnEnable
//
/*!Access:        Private(OverRide)

   Parameters

   IN             Description

   <BOOL>         Enabled or not

   OUT            Description

   <None>

   Notes          Run-time enabled message to enabled or not.

*/
//--------------------------------------------------------------------------
void CSkinButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	Invalidate();
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  InitToolTip
//
/*! Access:       Private

    Parameters

    IN            Description

    None

    OUT           Description

    None

    Notes         Create ToolTip Instance.
*/
//--------------------------------------------------------------------------
void CSkinButton::InitToolTip()
{
   if( m_ToolTip.m_hWnd == NULL )
   {
      //** Creation ToolTip controle
      m_ToolTip.Create(this);
      //** Don't activate it yet
      m_ToolTip.Activate(FALSE);
   }
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  CtlColor
//
/*! Access:       Private(OverRide)

    Parameters

    IN            Description

    <>            Regular param's

    OUT           Description

    <HBRUSH>      Background Color

    Notes         Message Reflection for Windows Controls 
*/
//--------------------------------------------------------------------------
HBRUSH CSkinButton::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	m_dcBk.DeleteDC();
	m_BmpBk.DeleteObject();	
	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
} // End of CtlColor



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   BuildDisable
//
/*!Access:        Private

   Parameters

   IN             Description

   <CDC*>         Source Context

   <CBitmap&>     Source Bitmap

   <CBitmap*>     Destination Bitmap

   OUT            Description

   <None>

   Notes          Build Bitmap Disable like 
*/
//--------------------------------------------------------------------------
void CSkinButton::BuildDisable(CDC* _pDC, CBitmap& _BmpSource, CBitmap* _pBmpDisable)
{
   BITMAP bm;
   BYTE B, R1, G1, B1;
   BYTE R2, G2, B2;

   //** Dimension source
   _BmpSource.GetObject(sizeof(BITMAP) , &bm);

   //** Create Disable Button
   _pBmpDisable->DeleteObject();
   _pBmpDisable->CreateCompatibleBitmap(_pDC, bm.bmWidth, bm.bmHeight);

   CDC hdcSrc, hdcDst;
   hdcSrc.CreateCompatibleDC(_pDC);
   hdcDst.CreateCompatibleDC(_pDC);

   //** Put Bitmap in memory DC
   CBitmap* hbmSrcT =  hdcSrc.SelectObject(&_BmpSource);
   CBitmap* hbmDstT =  hdcDst.SelectObject(_pBmpDisable);

   //** Need the Color for Transparency Effect
   COLORREF TransColor;
   //** Color at position 0,0
   TransColor = hdcSrc.GetPixel(0, 0);
	
   //** Build a disabled Button Like
   for( int i=1; i< bm.bmWidth-2; i++)
   {
      for(int j=1; j < bm.bmHeight-2; j++)
      {
         COLORREF cResult= hdcSrc.GetPixel(i-1, j-1);
         if( cResult != TransColor )
         {
            B1 = GetBValue(cResult);
            R1 = GetRValue(cResult);
            G1 = GetGValue(cResult);
            COLORREF cResult2 = hdcSrc.GetPixel(i+1, j+1);
            B2 = GetBValue(cResult2);
            R2 = GetRValue(cResult2);
            G2 = GetGValue(cResult2);
            B = (R2 + G2 + B2) - (R1 + G1 + B1) + 190 ;
            B = B < 0 ? 0:B;
            B =B > 255 ? 255: B;
            hdcDst.SetPixelV(i, j, RGB(B, B, B) );
         }
         else
            hdcDst.SetPixelV(i, j, TransColor);
      }//end for
   }//end for
	
   hdcSrc.SelectObject(hbmSrcT);
   hdcDst.SelectObject(hbmDstT);

   hdcSrc.DeleteDC();
   hdcDst.DeleteDC();
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   DrawIcon
//
/*!Access:        Private

   Parameters

   IN             Description

   <CDC*>         Device Context Destnation

   <RECT*>        Dimension Destination

   <CRect*>       Dimension of Text Source

   <BOOL>         Button pressed or not

   <BOOL>         Button enabled or not

   OUT            Description

   <None>

   Notes
*/
//--------------------------------------------------------------------------

void CSkinButton::DrawIcon(CDC* _pDC, RECT* _prcItem, 
                           CRect* _pCaptionrect, BOOL _bIsPressed, 
                           BOOL _bIsDisabled)
{
   CRect IconRect = _prcItem;

   switch(m_nAlign)
   {
      case Horizontal:
         //** Horizontal Align
         if ( m_szText.IsEmpty() )
            //** No text so center the Icon
            IconRect.left += ((IconRect.Width() - m_cXicon)/2);
         else
         {
            //** We have some Text
            IconRect.left +=3;
            _pCaptionrect->left += m_cXicon +3;
		   }
         IconRect.top += ((IconRect.Height() - m_cYicon)/2);
			break;

      case Vertical:
         //** Vertical Align
         IconRect.left += ((IconRect.Width() - m_cXicon)/2);
         if ( m_szText.IsEmpty() )
            //** no text to print so center Icon
            IconRect.top += ((IconRect.Height() - m_cYicon)/2);
			else
            //** Fit vertical of text
            _pCaptionrect->top += m_cYicon;
         break;
   }//end switch

   //** If Button pressed add offset
   if ( _bIsPressed )
      IconRect.OffsetRect(1,1);
	
   //** Draw the Icon
   _pDC->DrawState(IconRect.TopLeft(), 
                     IconRect.Size(),
                     m_hIcon,
                     (_bIsDisabled ? DSS_DISABLED:DSS_NORMAL),
                     (CBrush*)NULL);
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   PreSubclassWindow
//
/*! Access:       Private(Virtual OverRide)

   Parameters

   IN             Description

   None

   OUT            Description

   None

   Notes          First time control created so make it Owner Draw.
*/
//-------------------------------------------------------------------------
void CSkinButton::PreSubclassWindow() 
{
   UINT nBS;
   nBS = GetButtonStyle();

   //** Initialize Owner draw only once 
   if( nBS & BS_DEFPUSHBUTTON) 
      m_bIsDefault=TRUE;
	
   //** Owner draw style
   SetButtonStyle(nBS | BS_OWNERDRAW);
   GetWindowText(m_szText);
	
   //** Fill LOGFONT struct
   GetFont()->GetObject(sizeof(m_LogFont), &m_LogFont);
   RebuildFont();
	
   CButton::PreSubclassWindow();
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  OnSysColorChange
//
/*! Access:       Private(OverRide)

    Parameters

    IN            Description

    None

    OUT           Description

    None

    Notes         Msg from system to change Fore & Back Color
*/
//--------------------------------------------------------------------------
void CSkinButton::OnSysColorChange() 
{
   CButton::OnSysColorChange();
	
   if ( !m_bBackColor )
      //** Change the Back Color if not defined
      m_BackColor = ::GetSysColor(COLOR_BTNFACE);

   if ( !m_bForeColor )
      //** Do the same for the Fore Color
      m_ForeColor = ::GetSysColor(COLOR_BTNTEXT);

   m_dcBk.DeleteDC();
   m_BmpBk.DeleteObject();	
   Invalidate();
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  GrabClient
//
/*! Access:       Private

    Parameters

    IN            Description

    <CDC*>        Device Context

    OUT           Description

    None

    Notes         We take a snapshot of the Client Background
*/
//--------------------------------------------------------------------------
void CSkinButton::GrabClient(CDC* _pDC)
{
	CClientDC clDC(GetParent());
	CRect rect;
	CRect rect1;
	GetClientRect(rect);
	GetWindowRect(rect1);
	GetParent()->ScreenToClient(rect1);

	if( m_dcBk.m_hDC == NULL )
	{
		m_dcBk.CreateCompatibleDC(&clDC);
		m_BmpBk.CreateCompatibleBitmap(&clDC, rect.Width(), rect.Height());
		m_pBmpOldBk = m_dcBk.SelectObject(&m_BmpBk);
		m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), 
					&clDC, rect1.left, rect1.top, SRCCOPY);
	}
	_pDC->BitBlt(0, 0, rect.Width(), rect.Height(),
				&m_dcBk, 0, 0, SRCCOPY);

}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   PrepareMask
//
/*!Access:        Private

   Parameters

   IN             Description

   <CBipmap&>     Source image

   <CBitmap*>     Destination Image Mask

   OUT            Description

   <None>

   Notes          Take the whole Bitmap and build Mask & Sprite
                  in one pass.
*/
//--------------------------------------------------------------------------
void CSkinButton::PrepareMask(CBitmap& _BmpSource, CBitmap* _pBmpMask)
{
   //** Create Mask
   BITMAP bm;
   _BmpSource.GetBitmap(&bm);

   _pBmpMask->DeleteObject();
   _pBmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, 0);

   CDC hdcSrc, hdcDst;
   hdcSrc.CreateCompatibleDC(NULL);
   hdcDst.CreateCompatibleDC(NULL);

   //** Put Bitmap in memory DC
   CBitmap* hbmSrcT =  hdcSrc.SelectObject(&_BmpSource);
	CBitmap* hbmDstT =  hdcDst.SelectObject(_pBmpMask);

   //** Need the Color for Transparency Effect
   COLORREF TransColor;
   //** Color at position 0,0
   TransColor = hdcSrc.GetPixel(0, 0);

   //** Change Background a Transparence
   COLORREF clrSaveBK = hdcSrc.SetBkColor(TransColor);
   hdcDst.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &hdcSrc,
                  0, 0, SRCCOPY);

   COLORREF clsSaveDstText = hdcSrc.SetTextColor(RGB(255, 255, 255));
   hdcSrc.SetBkColor(RGB(0, 0, 0));

   hdcSrc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight,
                  &hdcDst, 0, 0 , SRCAND);

   //** Clean up
   hdcSrc.SetTextColor(clsSaveDstText);
   hdcSrc.SetBkColor(clrSaveBK);
   hdcSrc.SelectObject(hbmSrcT);
   hdcDst.SelectObject(hbmDstT);

   hdcSrc.DeleteDC();
   hdcDst.DeleteDC();
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  PreTranslateMessage
//
/*! Access:       Private(Virtual OverRide)

    Parameters

    IN            Description

    <MSG*>        The Tooltip Message

    OUT           Description

    None

    Notes         Need to intercept ToolTip Message 


*/
//--------------------------------------------------------------------------
BOOL CSkinButton::PreTranslateMessage(MSG* pMsg) 
{
   InitToolTip();
   m_ToolTip.RelayEvent(pMsg);
	return CButton::PreTranslateMessage(pMsg);
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   StroutText
//
/*!Access:        Private

   Parameters

   IN             Description

   <CDC*>         Device Context Destination

   <CRect>        Destination Dimension

   <BOOL>         Enabled or not

   OUT            Description

   <None>

   Notes          Print Caption Text for Graphic Button 
*/
//--------------------------------------------------------------------------
void CSkinButton::StroutText(CDC* _pDC, CRect _ItemRect, BOOL _bIsDisabled)
{
   CRect CaptionRect = _ItemRect;
   CRect CenterRect = _ItemRect;

   //** If any text then proceed
   if( !m_szText.IsEmpty() )
   {
      CFont* pOldFont= _pDC->SelectObject(&m_Font);
      _pDC->SetTextColor(m_ForeColor);
		
		// Center text
		_pDC->DrawText(m_szText, -1, CaptionRect, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
		CaptionRect.OffsetRect((CenterRect.Width() - CaptionRect.Width())/2, 
                           (CenterRect.Height() - CaptionRect.Height())/2);

      CaptionRect.top -=1;

      _pDC->DrawState(CaptionRect.TopLeft(), CaptionRect.Size(),
				(LPCTSTR)m_szText, (_bIsDisabled? DSS_DISABLED:DSS_NORMAL),
				TRUE, 0, (CBrush*)NULL);

      _pDC->SelectObject(pOldFont);	
   }
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  DrawItem
//
/*! Access:       Private(OverRide)

    Parameters

    IN            Description

    <>            Regular Param's

    OUT           Description

    None

    Notes         Use this instead of Paint Event.
*/
//--------------------------------------------------------------------------
void CSkinButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC*  pDC = CDC::FromHandle(lpDIS->hDC);
	CRect ItemRect = lpDIS->rcItem;
	UINT  State = lpDIS->itemState;
	BOOL  bIsPressed = (State & ODS_SELECTED);
	BOOL  bIsDisabled = (State & ODS_DISABLED);
	
	//**  Use Double Buffering Technique
	CBitmap bitmapTemp;
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	bitmapTemp.CreateCompatibleBitmap(pDC, ItemRect.Width(), 
		ItemRect.Height());
	
	CBitmap *pOldBitmapTemp = MemDC.SelectObject(&bitmapTemp);
	MemDC.SetBkMode(TRANSPARENT);
	pDC->SetBkMode(TRANSPARENT);
	
	if ( m_bUseBitmap )  // Use Bitmap Button with Skin Effect
	{
		GrabClient(&MemDC); 
		CDC TempDC;
		TempDC.CreateCompatibleDC(pDC);
		CBitmap* pOldBitmap;
		
		if( bIsDisabled )
		{
			//** Button disabled
			if(m_MaskBitmap.m_hObject == NULL )
				PrepareMask(*m_pButton, &m_MaskBitmap);
			
			pOldBitmap= TempDC.SelectObject(m_pButton);
			DrawTransparentBitmap(pDC, &TempDC, &m_MaskBitmap, 2);
		}
		else
		{
			//** Button enabled
			if ( bIsPressed )
			{
				//** Button DN
				if(m_MaskBitmap.m_hObject == NULL )
					PrepareMask(*m_pButton, &m_MaskBitmap);
				
				pOldBitmap= TempDC.SelectObject(m_pButton);
				DrawTransparentBitmap(pDC, &TempDC, &m_MaskBitmap, 1); 
			}
			else if(m_bHover)
			{
				//** Button Up
				if ( m_MaskBitmap.m_hObject == NULL )
					PrepareMask(*m_pButton, &m_MaskBitmap);
				
				pOldBitmap= TempDC.SelectObject(m_pButton);
				DrawTransparentBitmap(pDC, &TempDC, &m_MaskBitmap, 3); 
			}
         else
			{
				//** Button Up
				if ( m_MaskBitmap.m_hObject == NULL )
					PrepareMask(*m_pButton, &m_MaskBitmap);
				
				pOldBitmap= TempDC.SelectObject(m_pButton);
				DrawTransparentBitmap(pDC, &TempDC, &m_MaskBitmap, 0); 
			}
		}
		
		TempDC.SelectObject(pOldBitmap);
		TempDC.DeleteDC();
		//** Draw the Text if any
		StroutText(pDC, lpDIS->rcItem, bIsDisabled);
		//** If Icon then draw it
		CRect CaptionRect = lpDIS->rcItem;
		if( m_hIcon != NULL )
			DrawIcon(pDC, &lpDIS->rcItem, &CaptionRect, bIsPressed, bIsDisabled);
	}
	else
	{
		//** We using Button in normal Way Caption Icon Bold etc....
		if ( bIsPressed )
			//** Clicked
			MemDC.DrawFrameControl(ItemRect, DFC_BUTTON, 
			DFCS_BUTTONPUSH | DFCS_PUSHED);
		else
			//** Not Clicked  
			MemDC.DrawFrameControl(ItemRect, DFC_BUTTON, DFCS_BUTTONPUSH);
		
		CRect LeRect = ItemRect;
		
		//** If Background Color then do it
		if( m_bBackColor)
		{
            LeRect.DeflateRect(2,2);
            CBrush br;
            br.CreateSolidBrush(m_BackColor);
            MemDC.FillRect(&LeRect, &br);
		}
		
		//** Now the Caption Text
		CRect CaptionRect = lpDIS->rcItem;
		
		//** If Icon then draw it
		if( m_hIcon != NULL )
			DrawIcon(&MemDC, &lpDIS->rcItem, &CaptionRect, bIsPressed, bIsDisabled);
		
		MemDC.SetTextColor(m_ForeColor);
		//** Center Text
		CRect CenterRect = CaptionRect;
		
		//** get text font
		CFont* pOldFont= MemDC.SelectObject(&m_Font);
		
		MemDC.DrawText(m_szText, -1, CaptionRect, DT_SINGLELINE|DT_CALCRECT);
		
		CaptionRect.OffsetRect((CenterRect.Width() - CaptionRect.Width())/2,
			(CenterRect.Height() - CaptionRect.Height())/2);
		
		MemDC.DrawState(CaptionRect.TopLeft(), 
			CaptionRect.Size(),
			(LPCTSTR)m_szText, 
			(bIsDisabled? DSS_DISABLED:DSS_NORMAL),
			TRUE, 0, (CBrush*)NULL);
		
		//** If Font 3D effect then do it
		if ( m_bFont3D )
		{	
			MemDC.SetTextColor(m_3DHiliteColor);
			if ( m_3dType == Raised )
			{
				CaptionRect.OffsetRect(-1,-1);
				MemDC.DrawState(CaptionRect.TopLeft(), 
					CaptionRect.Size(),
					(LPCTSTR)m_szText, 
					(bIsDisabled? DSS_DISABLED:DSS_NORMAL),
					TRUE, 0, (CBrush*)NULL);
				
				CaptionRect.OffsetRect(1,1);
			}
			else
			{
				CaptionRect.OffsetRect(1,1);
				MemDC.DrawState(CaptionRect.TopLeft(), 
					CaptionRect.Size(),
					(LPCTSTR)m_szText, 
					(bIsDisabled? DSS_DISABLED:DSS_NORMAL),
					TRUE, 0, (CBrush*)NULL);
				
				CaptionRect.OffsetRect(-1,-1);
			}
			MemDC.SelectObject(pOldFont);	
		}//end if
		
		//** Now Blit the whole result to original DC
		pDC->BitBlt(0, 0, ItemRect.Width(), ItemRect.Height(), &MemDC, 0, 0, SRCCOPY);
		
		//** Clean up
		MemDC.SelectObject(pOldBitmapTemp);
		MemDC.DeleteDC();
		ReleaseDC(pDC);
		
	}	
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  RebuildFont
//
/*! Access:       Private

    Parameters

    IN            Description

    None

    OUT           Description

    None

    Notes         Rebuild CFont class
*/
//--------------------------------------------------------------------------
void CSkinButton::RebuildFont()
{
	//** Delete the Object
	m_Font.DeleteObject();
	//** Rebuild it
	m_Font.CreateFontIndirect(&m_LogFont);
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   DrawTransparentBitmap
//
/*!Access:        Private

   Parameters

   IN             Description

   <CDC*>         Device Context Source

   <CDC*>         Device Context 

   <CBitmap*>     Bitmap Mask

   <int>          0 = Left side Bitmap and 1 = Right Side Bitmap

   OUT            Description

   <None>

   Notes          We send on the Screen The Mask and Sprite 
                  This makes Transparent effect
*/
//--------------------------------------------------------------------------
void CSkinButton::DrawTransparentBitmap(CDC* _pDC, CDC* _pTmpDC, CBitmap* _pBmpMask,
                                        int _iSelected)
{
   CDC hdcMem;
   hdcMem.CreateCompatibleDC(_pDC);
   int iPos= m_Width * _iSelected;

	CBitmap* phbmT = hdcMem.SelectObject(_pBmpMask);

	//** Send Mask
	_pDC->StretchBlt(0, 0, m_nWidth, m_nHeight, &hdcMem,
					      iPos, 0, m_Width, m_Height, SRCAND);
	//** Send Sprite
	_pDC->StretchBlt(0, 0, m_nWidth, m_nHeight, _pTmpDC,
                     iPos, 0, m_Width, m_Height, SRCPAINT);

	//** Clean Up
	hdcMem.SelectObject(phbmT);
	hdcMem.DeleteDC();
}



///////////////////////////////////////////////////////////////////////////
//                         Public Functions
///////////////////////////////////////////////////////////////////////////



//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  SetText
//
/*! Access:       Public

    Parameters

    IN            Description

    <UINT>        Resource ID text

    OUT           Description

   <CSkinButton&>	Reference to Class

    Notes
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetText(UINT _nTextID)
{
   CString szTemp;
   try
   {
      //** Load String resource
      szTemp.LoadString(_nTextID);
      m_szText = szTemp;
   }
   catch(...)
   {
      m_szText= "";
   }
   Refresh();
   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetToolTipText
//
/*!Access:        Public

   Parameters
   IN             Description

   <LPCTSTR>      Set ToolTip text 

   OUT            Description

   <CSkinButton&>	Reference to Class

   Notes          Need to activate Tip to see ToolTip
                  See ** ActivateToolTip
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetToolTipText(LPCTSTR _szText)
{
   //** The string is empty
	if( _szText == NULL )
      return *this;

   //** Initialise ToolTip
   InitToolTip();
   if( m_ToolTip.GetToolCount() == 0 )
   {
      //** Add Tooltip
      CRect rectBtn;
      GetClientRect(rectBtn);
      m_ToolTip.AddTool(this, _szText, rectBtn,1);
   }

   //** Set the ToolTip Text
   m_ToolTip.UpdateTipText(_szText, this, 1);
   m_ToolTip.Activate(TRUE);
   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  SetScale
//
/*! Access:       Public

    Parameters

    IN            Description

    <int>         Scale Express as %

    OUT           Description

   <CSkinButton&> Reference to the class

    Notes         100 = Original Size
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetScale(int _XScale, int _YScale)
{
   m_bStrech= TRUE;
   m_XScale = _XScale;
   m_YScale = _YScale;
   m_nHeight = (_YScale *  m_Height)/100;
   m_nWidth = (_XScale * m_Width)/100;
   SetWindowPos(NULL, 0, 0, m_nWidth, m_nHeight,
                  SWP_NOMOVE | SWP_NOOWNERZORDER);
   Invalidate();
   return *this; 
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  SetIconAlign
//
/*! Access:       Public

    Parameters

    IN            Description

    <IconAlign>   Alignment type 

    OUT           Description

   <CSkinButton&> Reference to the class

    Notes         Horizontal is Default

*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetIconAlign(IconAlign _Type)
{
   m_nAlign = _Type;
   Invalidate();
   return *this; 
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   ActivateToolTip
//
/*!Access:        Public

   Parameters

   IN             Description

   <BOOL>         TRUE -> Tooltip is enabled

   OUT            Description

   <CSkinButton&> Reference to Class

   Notes
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::ActivateToolTip(BOOL _bActive)
{
   if( m_ToolTip.GetToolCount() > 0 )
      m_ToolTip.Activate(_bActive);
   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  SetBackColor
//
/*! Access:       Public

    Parameters

    IN            Description

    <COLORREF>    The new Background Color

    OUT           Description

   <CSkinButton&> Reference to Class

    Notes
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetBackColor(COLORREF _nColor)
{
   m_BackColor = _nColor;
	//** OnSysColorChange will consider this
   m_bBackColor= TRUE;
   Refresh();
   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  SetForeColor
//
/*! Access:       Public

    Parameters

    IN            Description

    <COLORREF>    The new Fore Color (Text Color)

    OUT           Description

   <CSkinButton&> Reference to Class

    Notes
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetForeColor(COLORREF _nColor)
{
   m_ForeColor = _nColor;
   m_bForeColor= TRUE;
   Refresh();
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  SetFontBold
//
/*! Access:       Public

    Parameters

    IN            Description

    <BOOL>        Turn BOLD or Not the Text

    OUT           Description

   <CSkinButton&> Reference to Class

    Notes
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetFontBold(BOOL _bVal)
{
   m_LogFont.lfWeight = _bVal ? FW_BOLD: FW_NORMAL;
   RebuildFont();
   Refresh();
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  SetFontName
//
/*! Access:       Public

    Parameters

    IN            Description

    <LPTSTR>      The Font Name

    OUT           Description

   <CSkinButton&> Reference to Class

    Notes
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetFontName(LPTSTR _szFontName)
{
   m_LogFont.lfCharSet = DEFAULT_CHARSET;
   //** Unicode Copy
   _tcscpy(m_LogFont.lfFaceName, _szFontName);
	
   RebuildFont();
   Refresh();
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  SetText
//
/*! Access:       Public

    Parameters

    IN            Description

    <LPCTSTR>     The Text caption 

    OUT           Description

   <CSkinButton&> Reference to Class

    Notes
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetText(LPCTSTR _szText)
{
   m_szText= _szText;
   Refresh();
	return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
//  Method Name:  SetFontSize
//
/*! Access:       Private

    Parameters

    IN            Description

    <int>         Caption Font Size (6 to 72)

    OUT           Description

   <CSkinButton&> Reference to Class

    Notes
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetFontSize(int _nSize)
{
   int      iTempSize;
   CFont    cf;
   LOGFONT  lf;

   iTempSize = (_nSize < 6) ? 6: _nSize;
   iTempSize = (_nSize >72) ? 72: _nSize;

   //** Create the Font
   cf.CreatePointFont(iTempSize * 10 , m_LogFont.lfFaceName);
   cf.GetLogFont(&lf);

   //** Update Font Struct
   m_LogFont.lfHeight = lf.lfHeight;
   m_LogFont.lfWidth = lf.lfWidth;

   RebuildFont();
   Refresh();
   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetFont3D
//
/*!Access:        Public

   Parameters

   IN             Description

   <BOOL>         TRUE -> 3D effect else Normal

   <Type3D>       Enum type Raised or Sunken

    OUT           Description

   <CSkinButton&> Reference to Class

    Notes
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetFont3D(BOOL _bSet, Type3D _Type)
{
   m_bFont3D = _bSet;
   m_3dType = _Type;
	
   Refresh();
   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetToolTipText
//
/*!Access:        Public

   Parameters

   IN             Description

   <UINT>         Tooltip text from resource

   OUT            Description

   <CSkinButton&> Reference to Class

   Notes          Need to activate Tip to see ToolTip
                  See ** ActivateToolTip
*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetToolTipText(UINT _nID)
{
   CString szText;

   try
   {
      //** Load string from resource
      szText.LoadString(_nID);
      if ( !szText.IsEmpty() )
         SetToolTipText(szText);
   }
   catch(...)
   {
      //** Error de-active  tooltip
      ActivateToolTip(FALSE);
   }
   
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetIcon
//
/*!Access:        Public

   Parameters

   IN             Description

   <UINT>         Iconid from Resource

   OUT            Description

   <CSkinButton&> Reference to Class

   Notes          Add Icon to Button.

*/
//--------------------------------------------------------------------------
CSkinButton& CSkinButton::SetIcon(UINT _nIconID)
{
   HINSTANCE hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(_nIconID),
                                                   RT_GROUP_ICON);

   m_hIcon = (HICON)::LoadImage(hInstResource,MAKEINTRESOURCE(_nIconID),
                                IMAGE_ICON,0,0,0);

   //** Icon dimensions
   ICONINFO ii;
   ZeroMemory(&ii, sizeof(ICONINFO));
   ::GetIconInfo(m_hIcon, &ii);

   m_cXicon = (BYTE)(ii.xHotspot * 2);
   m_cYicon = (BYTE)(ii.yHotspot * 2);
   Refresh();

   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetBitmap
//
/*!Access:        Public

   Parameters

   IN             Description

   <UINT>         Image from Resource

   OUT            Description

   <ref>          Reference to Class

   Notes          The Graphic Button must hold the 2 Pictures Side
                  by side.
*/
//--------------------------------------------------------------------------
CSkinButton&   CSkinButton::SetBitmap(CBitmap *pBitMap)
{
   m_pButton = pBitMap;

   //** Get the Picture Size and resize the Button 
   BITMAP bm;
   m_pButton->GetBitmap(&bm);
   m_Height= bm.bmHeight;
	m_Width = bm.bmWidth/4;
   m_nHeight = m_Height;
   m_nWidth = m_Width;
	
   if ( m_bStrech )
   {
      m_nHeight = (m_YScale *  m_Height)/100;
      m_nWidth = (m_XScale * m_Width)/100;
   }
	
   SetWindowPos(NULL, 0, 0, m_nWidth, m_nHeight,
               SWP_NOMOVE | SWP_NOOWNERZORDER);

   m_bUseBitmap = TRUE;

   //** Clean up
   m_MaskBitmap.DeleteObject();
   m_DisablePict.DeleteObject();
   m_dcBk.DeleteDC();
   m_BmpBk.DeleteObject();

   Refresh();
   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:    Refresh
//
/*!Access:         Public

   Parameters

   IN              Description

   <None>

   OUT             Description

   <ref>          Reference to the class

   Notes          Doing no more Invalidate(). Instead we tell
                  to parent invalidate the Ctrl Rect Area.
*/
//--------------------------------------------------------------------------
void CSkinButton::Refresh()
{
   //** Clear the rectangle and Redraw for correct display
	CRect rc;
	GetWindowRect (rc);
	//** Translate to Parent Coord;
   GetParent()->ScreenToClient(rc);
	//** Tell parent to redraw the control only
   GetParent()->InvalidateRect(rc,TRUE);
}

LRESULT CSkinButton::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover = TRUE;
	
	Invalidate();
	return 0L;
}

LRESULT CSkinButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bHover		   	= FALSE;
   m_bCursorOnButton	= FALSE;
	
	Invalidate();
	return 0L;
}



void CSkinButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// we wanna get WM_MOUSEHOVER,WM_MOUSELEAVE message, so we call _TrackMouseEvent
	if( m_bCursorOnButton == FALSE )
	{
		TRACKMOUSEEVENT tme;
		ZeroMemory(&tme,sizeof(TRACKMOUSEEVENT));
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bCursorOnButton = _TrackMouseEvent(&tme);
	}
	
	CButton::OnMouseMove(nFlags, point);
}