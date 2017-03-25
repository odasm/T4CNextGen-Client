//--FILE DEFINITION-----------------------------------------------------------


//--FILE INCLUDES-------------------------------------------------------------
#include "..\stdafx.h"
#include "uicLabel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//!--CONSTRUCTOR--------------------------------------------------------------
//
//  Method Name:  uicLabel()
//
/*!

   Notes
*/
//-----------------------------------------------------------------------------
uicLabel::uicLabel()
{
	//** Fore Color default
	m_ForeColor = GetSysColor(COLOR_WINDOWTEXT);
	//** No back color as  default (Transparent)
	m_bTextFlash= FALSE;
	m_bFlashTextState   = FALSE;
	m_hCursor = NULL;
	m_bLink = FALSE;
	m_bFont3D = FALSE;
	m_3DHiliteColor = RGB(50,50,50);
	m_bVertical = FALSE;
	m_bForeColor= FALSE;
	m_uiCaption = NULL;
   m_bCenter   = TRUE;
}



//!--DESTRUCTOR-----------------------------------------------------------------
//
//  Method Name:  ~uicLabel()
//
/*!
    
    Notes
*/			
//-----------------------------------------------------------------------------
uicLabel::~uicLabel()
{
   m_Font.DeleteObject();
   m_BackBrush.DeleteObject();
}


BEGIN_MESSAGE_MAP(uicLabel, CStatic)
	//{{AFX_MSG_MAP(uicLabel)
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



///////////////////////////////////////////////////////////////////////////
//                         Private Functions
///////////////////////////////////////////////////////////////////////////


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   PreSubclassWindow
//
/*! Access:       Private

   Parameters

   IN             Description

   None

   OUT            Description

   None

   Notes       First time control created so make it Owner Draw.
*/
//-------------------------------------------------------------------------
void uicLabel::PreSubclassWindow() 
{
   //** Mode Owndraw
   ModifyStyle(0,SS_OWNERDRAW);
   //** Enable mouse Click and so on
   ModifyStyle(0, SS_NOTIFY);
   ModifyStyleEx(0,WS_EX_TRANSPARENT);

   //** Read the caption text as default
   GetWindowText(m_szText);
   m_szTempText = m_szText;

   //** fill LOGFONT structure
   GetFont()->GetObject(sizeof(m_LogFont), &m_LogFont);
   RebuildFont();

   UpdateSurface();
   CStatic::PreSubclassWindow();
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   RebuildFont
//
/*!Access:        Private

   Parameters

   IN             Description

   None

   OUT            Description

   None

   Notes    Every time there is a change on Font we go

            here cause we need to rebuild font.	
*/
//--------------------------------------------------------------------------
void uicLabel::RebuildFont()
{
   m_Font.DeleteObject();
   m_Font.CreateFontIndirect(& m_LogFont);
   //** Update the Display
   UpdateSurface();
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   OnPaint
//
/*!Access:        Private(OverRide)

   Parameters

   IN             Description

   <None>

   OUT            Description

   <None>

   Notes          Display processing event
*/
//--------------------------------------------------------------------------
void uicLabel::OnPaint() 
{
	CPaintDC dc(this); 
		
	DWORD dwFlags =0;
 	CRect rc;
	CBitmap bmp;		      //** For non Transparent Mode
   CBitmap* pOldBitmap;
	GetClientRect(rc);
	CDC*  pMemDC= NULL;
	

	if ( m_BackBrush.m_hObject !=NULL )
	{
		//** We have a Back Color
      pMemDC->CreateCompatibleDC(&dc);
      pMemDC->SetBkMode(TRANSPARENT);
		bmp.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());
		pOldBitmap = pMemDC->SelectObject(&bmp);
		pMemDC->FillRect(rc, &m_BackBrush);
	}
	else
		//** Transparent Mode
		pMemDC = &dc;

	UINT nMode = pMemDC->SetBkMode(TRANSPARENT);
	COLORREF ColorText = pMemDC->SetTextColor(m_ForeColor);
	CFont *pOldFont = pMemDC->SelectObject(&m_Font);

	//** Get the Label Style
	DWORD style = GetStyle();
   /*
	switch ( style & SS_TYPEMASK )
	{
		case SS_RIGHT:
			dwFlags = DT_RIGHT | DT_WORDBREAK;
			break;
		case SS_CENTER:
			dwFlags = SS_CENTER | DT_WORDBREAK;
			break;
		case SS_LEFTNOWORDWRAP:
			dwFlags = DT_LEFT;
			break;
		default:
			dwFlags = DT_LEFT | DT_WORDBREAK;
			break;
	}
	if ( style & SS_CENTERIMAGE )
	{
		dwFlags = DT_CENTER;
		if(m_szText.Find(_T("\r\n")) == -1)
		{
			dwFlags |= DT_VCENTER;
			dwFlags |= DT_SINGLELINE;
		}
	}
   */
   if(m_bCenter == 0)
      dwFlags = DT_LEFT | DT_WORDBREAK;
   else if(m_bCenter == 1)
      dwFlags = SS_CENTER | DT_WORDBREAK;
   else if(m_bCenter == 2)
      dwFlags = DT_RIGHT | DT_WORDBREAK;

	//** if using Font3D
	if ( m_bFont3D )
	{
		pMemDC->SetTextColor(m_3DHiliteColor);
		if ( m_3dType == Raised )
		{
			rc.OffsetRect(-1,-1);
			pMemDC->DrawText(m_szText,rc,dwFlags);
			rc.OffsetRect(1,1);
		}
		else
		{
			rc.OffsetRect(1,1);
			pMemDC->DrawText(m_szText,rc,dwFlags);
			rc.OffsetRect(-1,-1);
		}
	}

	pMemDC->SetTextColor(m_ForeColor);
	pMemDC->DrawText(m_szText,rc,dwFlags);

		//** Clean up
	pMemDC->SetBkMode(nMode);
	pMemDC->SelectObject(pOldFont);
	pMemDC->SetTextColor(ColorText);

	if ( m_BackBrush.m_hObject !=NULL )
   {
		//** Bitblt pour mode opaque
		dc.BitBlt(0, 0, rc.Width(), rc.Height(), pMemDC, 0, 0, SRCCOPY);
      pMemDC->SelectObject(pOldBitmap);
   }

   if(pMemDC)
      ReleaseDC(pMemDC);
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   OnSysColorChange
//
/*!Access:        Private

   Parameters

   IN             Description

   <None>

   OUT	         Description

   <None>

   Notes          Desktop setup has changed
*/
//--------------------------------------------------------------------------
void uicLabel::OnSysColorChange() 
{
	CStatic::OnSysColorChange();
	//** If no Text Color defined then we are using the
	//** The current system color and we must read again
	if ( ! m_bForeColor )
	{
		m_ForeColor = GetSysColor(COLOR_WINDOWTEXT);
		UpdateSurface();
	}
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   UpdateSurface
//
/*!Access:        Private

   Parameters

   IN             Description

   <None>

   OUT	         Description

   <None>

   Notes          We clear only the control and advise parent

                  then the Paint event is called.
*/
//--------------------------------------------------------------------------
void uicLabel::UpdateSurface()
{
	CRect (rc);
	GetWindowRect (rc);
	GetParent()->ScreenToClient(rc);
	//** Call indirectly  OnPaint()
	GetParent()->InvalidateRect(rc, TRUE);
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   OnSetCursor
//
/*!Access:			Private(OverRide)

   Parameters

   IN             Description

   <>             Standard Param's Inputs.

   OUT	         Description

   <BOOL>         Nonzero to halt further processing, or 0 to continue.

   Notes          We Trap the Message ON_WM_SETCURSOR()

                  and set the cursor (See SetLinkEffect).
*/
//--------------------------------------------------------------------------
BOOL uicLabel::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if ( m_hCursor )
   {
    ::SetCursor(m_hCursor);
	return TRUE;
   }
	//** Else let the message parent
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
//Method Name:    OnTimer
//
/*!Access:        Private

   Parameters

   IN             Description

   <UINT>         The Timer ID			

   OUT            Description

   <None>

   Notes          Timer used for Flashing Effect.
*/
//--------------------------------------------------------------------------
void uicLabel::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
   {
    case TIMERID : // TEXT FLASH
		if ( m_bTextFlash ) 
		{
			m_bFlashTextState = !m_bFlashTextState;
			if ( m_bFlashTextState )
				m_szText="";
			else
				m_szText = m_szTempText;
			UpdateSurface();
		}
        break;
	default:
		break;
	}
	CStatic::OnTimer(nIDEvent);
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   OnLButtonDown
//
/*!Access:        Private(OverRide)

   Parameters

   IN             Description

   <>             Standard Input Mouse

   OUT            Description

   None

   Notes       If the LinkEffect is ON then we use Shell	command.

               Example: If Caption = "http://www.microsoft.com"

               Then Internet Explorer is Called
*/
//--------------------------------------------------------------------------
void uicLabel::OnLButtonDown(UINT nFlags, CPoint point) 
{
   if (m_bLink)
      ShellExecute(NULL, "open", m_szTextLink, NULL, NULL, SW_SHOWNORMAL);
   else
      PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
   
   CStatic::OnLButtonDown(nFlags, point);
}


///////////////////////////////////////////////////////////////////////////
//                         Public Functions
///////////////////////////////////////////////////////////////////////////


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetText
//
/*!Access:        Public

   Parameters

   IN             Description

   <UINT>         Resource ID

   OUT            Description

   <uicLabel&>    Class reference

   Notes          If you later want to switch from English to French
                  or whatever you better use this one
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetText(UINT _nID)
{
   m_szText.LoadString(_nID);
   m_uiCaption = _nID;

   if( m_bVertical )
      SetVerticalEffect(TRUE);
   else
      UpdateSurface();
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetText
//
/*!Access:        Public

   Parameters

   IN             Description

   <LPCTSTR>      The text to pass

   OUT            Description

   <uicLabel&>    Class reference

   Notes
*/			
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetText(LPCTSTR _szText)
{
   m_szText= _szText;
   m_szTempText = m_szText;

   if( m_bVertical )
      SetVerticalEffect(TRUE);
   else
   UpdateSurface();

   return *this;
}

uicLabel& uicLabel::SetTextLink(LPCTSTR _szText)
{
   m_szTextLink = _szText;

   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   GetText
//
/*!Access:        Public

   Parameters

   IN             Description

   None

   OUT            Description

   <CString>      Return the caption text

   Notes
*/
//--------------------------------------------------------------------------
CString uicLabel::GetText()
{
   return m_szText;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:      SetFontName
//
/*!Access:           Public

   Parameters

   IN                Description

   <LPCTSTR>         The font name

   OUT               Description

   <uicLabel&>       Class reference

   Notes
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetFontName(LPCTSTR _szFontName)
{
   m_LogFont.lfCharSet = DEFAULT_CHARSET;
   _tcscpy(m_LogFont.lfFaceName, _szFontName);
   RebuildFont();
   UpdateSurface();
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetFontBold
//
/*!Access:        Public

   Parameters

   IN             Description

   <BOOL>         TRUE ->Bold else Normal
			
   OUT            Description

   <uicLabel&>    Class reference

   Notes
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetFontBold(BOOL _bBold)
{
   m_LogFont.lfWeight = _bBold ? FW_BOLD: FW_NORMAL;
   RebuildFont();
   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetFontSize
//
/*!Access:        Public

   Parameters

   IN             Description

   <int>          Font size from 6 to 72

   OUT            Description

   <uicLabel>     Class reference

   Notes          The font size is point scale
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetFontSize(int _iSize)
{
   int  iTempSize = _iSize;

   if ( _iSize < 6 )
      iTempSize=6;
   if ( _iSize > 72 )
      iTempSize=72;

   CFont cf;
   LOGFONT lf;
   //** Create the new font
   cf.CreatePointFont(iTempSize * 10 ,m_LogFont.lfFaceName);
   cf.GetLogFont(&lf);

   //** Update structure
   m_LogFont.lfHeight = lf.lfHeight;
   m_LogFont.lfWidth = lf.lfWidth;

   RebuildFont();
   return *this;
}  


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetForeColor
//
/*!Access:        Public

   Parameters

   IN             Description

   <COLORREF>     The new Text Color

   OUT            Description

   <uicLabel&>    Class reference

   Notes
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetForeColor(COLORREF _crColorText)
{
	m_ForeColor = _crColorText;
	//** Set flag telling to use Color from Client
	m_bForeColor = TRUE;
	UpdateSurface();
	return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetBackColor
//
/*!Access:        Public

   Parameters

   IN             Description

   <COLORREF>     The back Color

   OUT            Description

   <uicLabel&>    Class reference

   Notes          If you set a BackColor the control is no more
                  in transparent Mode.
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetBackColor(COLORREF _crBkColor)
{
   if ( m_BackBrush.m_hObject )
      m_BackBrush.DeleteObject();

   m_BackBrush.CreateSolidBrush(_crBkColor);

   UpdateSurface();
   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:      SetBorder
//
/*!Access:           Public

   Parameters

   IN                Description

   <BOOL>            Border Frame or Not

   OUT               Description

   <uicLabel&>       Class reference

   Notes
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetBorder(BOOL _bSet)
{
   ModifyStyle(_bSet ? 0 : WS_BORDER, _bSet ? WS_BORDER : 0, SWP_DRAWFRAME);
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetSunken
//
/*!Access:        Public

   Parameters

   IN             Description

   <BOOL>         TRUE -> Sunken frame else Normal or no frame

   OUT            Description

   <uicLabel&>    Class reference

   Notes
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetSunken(BOOL _bSet)
{
   if ( _bSet )
      //** Sunken effet
      ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
   else
      //** Normal
      ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
   return *this;
}



//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetFont3D
//
/*!Access:        Public

   Parameters

   IN             Description

   <BOOL>         TRUE -> Font3D  else Normal

   <Type3D>       Raised or Sunken

   OUT            Description

   <uicLabel&>    Class reference

   Notes          Example: SetFont3D(TRUE, uicLabel::Raised);
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetFont3D(BOOL _bSet, Type3D _Type )
{
   m_bFont3D = _bSet;
   m_3dType = _Type;
   UpdateSurface();
   return *this;
}

uicLabel& uicLabel::SetCenter(BOOL _bSet)
{
   m_bCenter = _bSet;
   UpdateSurface();
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetVerticalEffect
//
/*!Access:        Public

   Parameters

   IN             Description

   <BOOL>         TRUE  ->	Vertical Effect
                  FALSE ->	Horizontal Effect

   OUT            Description

   <uicLabel&>    Class reference

   Notes
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetVerticalEffect(BOOL _bSet)
{
   m_bVertical = _bSet;

   if ( !m_szText.IsEmpty() )
   {
	   //** Vertical Effect
      if ( _bSet )
      {	
         TCHAR szTemp[255];
         _stprintf(szTemp, (LPCSTR)m_szText);
         CString szTemp1;
         int nLength = m_szText.GetLength();
         for ( int iCount=0; iCount < nLength; iCount++ )
         {
            if ( szTemp[iCount] != '\n' )
            {	
               szTemp1 += szTemp[iCount];
               szTemp1 += '\n';
            }//end if
         }//end for
         m_szText = szTemp1;
      }//end if
      else
         //** Horizontal Effect
         m_szText.Remove(_T('\n'));
   }//end if
   UpdateSurface();
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   SetLinkEffect
//
/*!Access:        Public

   Parameters

   IN             Description

   <BOOL>         TRUE  ->	IE effect
                  FALSE ->	Normal

   <UINT>         Cursor from resource 

   OUT            Description

   <uicLabel&>    Class reference

   Notes          If you have no Cursor in the resource that is OK
                  It will work anyway.

                  If the user Click() on the Control then
                  OnLButtonDown will consider the Link
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::SetLinkEffect(BOOL _bSet, UINT _uiCursorID, BOOL _bKeepForColor)
{
   m_bLink = _bSet;
   m_LogFont.lfUnderline = _bSet;
   if ( !_bSet )
   {
      m_ForeColor =  GetSysColor(COLOR_WINDOWTEXT);
      m_hCursor = NULL;
   }
   else
   {
      if(!_bKeepForColor)
         m_ForeColor = RGB(0,0,255);
      m_hCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(_uiCursorID));
   }
   RebuildFont();
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   FlashText
//
/*!Access:        Public

   Parameters

   IN             Description

   <BOOL>         TRUE ->  ForeColor Text Flash 2 Times/Second
                  FALSE -> Normal effect

   OUT            Description

   <uicLabel&>    Class reference

   Notes          Flashing Speed is defined FLASHRATE
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::FlashText(BOOL _bSet)
{
   if (m_bTextFlash)
   {
      //** Timer is Working so stop it
      m_szText= m_szTempText;
      KillTimer(TIMERID);
      UpdateSurface();
   }

   if (_bSet)
      SetTimer(TIMERID, FLASHRATE, NULL);

   m_bTextFlash = _bSet;
   return *this;
}


//-METHOD IMPLEMENTATION----------------------------------------------------
//
// Method Name:   LangueFix
//
/*!Access:        Public

   Parameters

   IN             Description

   None

   OUT            Description

   <CMenuItem&>   Reference to the class

   Notes          Tell to the control to reload value.

                  Caption & ToolTip from Resource
*/
//--------------------------------------------------------------------------
uicLabel& uicLabel::LangueFix()
{
	if( m_uiCaption != NULL )
		SetText(m_uiCaption);
	return *this;
}
