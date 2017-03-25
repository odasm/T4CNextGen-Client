// MacroDialog.cpp : implementation file
//

#include "stdafx.h"
#include "t4clauncher.h"
#include "MacroDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RADUIS	100

#define IN_CIRCLE	1
#define IN_BRIGHT	2
#define IN_NOTHING	0

/////////////////////////////////////////////////////////////////////////////
// CMacroDialog dialog


CMacroDialog::CMacroDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMacroDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMacroDialog)
	m_bCharacter = TRUE;
	m_bChatter = TRUE;
	m_bGroup = TRUE;
	m_bInventory = TRUE;
	m_bMacro = TRUE;
	m_bOption = TRUE;
	m_bScreenShot = TRUE;
	m_bSpell = TRUE;
	m_bTrade = TRUE;
	m_chCharacter  = _T("S");
	m_chChatter    = _T("L");
	m_chGroup      = _T("G");
	m_chInventory  = _T("I");
	m_chMacro      = _T("M");
	m_chOption     = _T("O");
	m_chScreenShot = _T("H");
	m_chSpell      = _T("P");
	m_chTrade      = _T("T");
	m_chSwitchChat = _T("A");
	m_bSwitchChat = FALSE;
	//}}AFX_DATA_INIT

   m_bINIT     = FALSE;



   Vertex = CPoint(102,108);
	Top    = CPoint(102,9);
	Left   = CPoint(23,147);
	Right  = CPoint(181,147);

	color.r = 255;
   color.g = 255;
   color.b = 255;

	m_OldColor = color;
	hsvColor = color.toHSV();
	m_bInMouse = FALSE;
	m_bInitOver = FALSE;
	m_bInDrawAll = FALSE;
}

CMacroDialog::~CMacroDialog()
{
   if(m_HsbBitmap.GetSafeHandle())
	{
		m_HsbBitmap.DeleteObject();
	}
}


void CMacroDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMacroDialog)
	DDX_Control(pDX, IDC_LIST_COLOR, m_ListColor);
	DDX_Check(pDX, IDC_CHK_CHARACTERUI, m_bCharacter);
	DDX_Check(pDX, IDC_CHK_CHATTERUI, m_bChatter);
	DDX_Check(pDX, IDC_CHK_GROUPUI, m_bGroup);
	DDX_Check(pDX, IDC_CHK_INVENTORY, m_bInventory);
	DDX_Check(pDX, IDC_CHK_MACROUI, m_bMacro);
	DDX_Check(pDX, IDC_CHK_OPTION, m_bOption);
	DDX_Check(pDX, IDC_CHK_SCREENSHOT, m_bScreenShot);
	DDX_Check(pDX, IDC_CHK_SPELLUI, m_bSpell);
	DDX_Check(pDX, IDC_CHK_TRADE, m_bTrade);
	DDX_Text(pDX, IDC_EDIT_CHARACTERUI, m_chCharacter);
	DDV_MaxChars(pDX, m_chCharacter, 1);
	DDX_Text(pDX, IDC_EDIT_CHATTER, m_chChatter);
	DDV_MaxChars(pDX, m_chChatter, 1);
	DDX_Text(pDX, IDC_EDIT_GROUPUI, m_chGroup);
	DDV_MaxChars(pDX, m_chGroup, 1);
	DDX_Text(pDX, IDC_EDIT_INVENTORY, m_chInventory);
	DDV_MaxChars(pDX, m_chInventory, 1);
	DDX_Text(pDX, IDC_EDIT_MACROUI, m_chMacro);
	DDV_MaxChars(pDX, m_chMacro, 1);
	DDX_Text(pDX, IDC_EDIT_OPTION, m_chOption);
	DDV_MaxChars(pDX, m_chOption, 1);
	DDX_Text(pDX, IDC_EDIT_SCREENSHOT, m_chScreenShot);
	DDV_MaxChars(pDX, m_chScreenShot, 1);
	DDX_Text(pDX, IDC_EDIT_SPELLUI, m_chSpell);
	DDV_MaxChars(pDX, m_chSpell, 1);
	DDX_Text(pDX, IDC_EDIT_TRADE, m_chTrade);
	DDV_MaxChars(pDX, m_chTrade, 1);
	DDX_Text(pDX, IDC_EDIT_SWITCHCHAT, m_chSwitchChat);
	DDX_Check(pDX, IDC_CHK_SWITCH_CHAT_SIZE, m_bSwitchChat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMacroDialog, CDialog)
	//{{AFX_MSG_MAP(CMacroDialog)
	ON_BN_CLICKED(IDC_CHK_INVENTORY, OnChkInventory)
	ON_BN_CLICKED(IDC_CHK_CHARACTERUI, OnChkCharacterui)
	ON_BN_CLICKED(IDC_CHK_CHATTERUI, OnChkChatterui)
	ON_BN_CLICKED(IDC_CHK_GROUPUI, OnChkGroupui)
	ON_BN_CLICKED(IDC_CHK_SPELLUI, OnChkSpellui)
	ON_BN_CLICKED(IDC_CHK_MACROUI, OnChkMacroui)
	ON_BN_CLICKED(IDC_CHK_TRADE, OnChkTrade)
	ON_BN_CLICKED(IDC_CHK_OPTION, OnChkOption)
	ON_BN_CLICKED(IDC_CHK_SCREENSHOT, OnChkScreenshot)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_MOD, OnBtnMod)
	ON_BN_CLICKED(IDC_BTN_RESTAURE_DEFAULT_COLOR, OnBtnRestaureDefaultColor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacroDialog message handlers

void CMacroDialog::OnOK() 
{
	return;
	CDialog::OnOK();
}

void CMacroDialog::OnCancel() 
{
   UpdateData(TRUE);
   m_spLauncherConfig->bMacro[0] = m_bInventory ;
   m_spLauncherConfig->bMacro[1] = m_bCharacter ;
   m_spLauncherConfig->bMacro[2] = m_bChatter   ;
   m_spLauncherConfig->bMacro[3] = m_bGroup     ;
   m_spLauncherConfig->bMacro[4] = m_bSpell     ;
   m_spLauncherConfig->bMacro[5] = m_bMacro     ;
   m_spLauncherConfig->bMacro[6] = m_bTrade     ;
   m_spLauncherConfig->bMacro[7] = m_bOption    ;
   m_spLauncherConfig->bMacro[8] = m_bScreenShot;
   m_spLauncherConfig->bMacro[9] = m_bScreenShot;

   m_spLauncherConfig->chMacroKey[0] = m_chInventory[0]; //Inventory
   m_spLauncherConfig->chMacroKey[1] = m_chCharacter[0]; //Charecter
   m_spLauncherConfig->chMacroKey[2] = m_chChatter[0];   //Chatter
   m_spLauncherConfig->chMacroKey[3] = m_chGroup[0];     //GroupPlay
   m_spLauncherConfig->chMacroKey[4] = m_chSpell[0];     //Spell
   m_spLauncherConfig->chMacroKey[5] = m_chMacro[0];     //Macro
   m_spLauncherConfig->chMacroKey[6] = m_chTrade[0];     //Trade
   m_spLauncherConfig->chMacroKey[7] = m_chOption[0];    //Option
   m_spLauncherConfig->chMacroKey[8] = m_chScreenShot[0];//Screenshot
   m_spLauncherConfig->chMacroKey[9] = m_chSwitchChat[0];//Screenshot
   
   m_bINIT = FALSE;
	CDialog::OnCancel();
}


BOOL CMacroDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

   m_Language.Release();
   m_Language.Create(m_spLauncherConfig->dwLanguage);

   SetWindowText(m_Language[158]);


   GetDlgItem(IDC_CHK_INVENTORY  )->SetWindowText(m_Language[23]);
   GetDlgItem(IDC_CHK_CHARACTERUI)->SetWindowText(m_Language[15]);
   GetDlgItem(IDC_CHK_CHATTERUI  )->SetWindowText(m_Language[1]);
   GetDlgItem(IDC_CHK_GROUPUI    )->SetWindowText(m_Language[155]);
   GetDlgItem(IDC_CHK_SPELLUI    )->SetWindowText(m_Language[28]);
   GetDlgItem(IDC_CHK_MACROUI    )->SetWindowText(m_Language[16]);
   GetDlgItem(IDC_CHK_TRADE      )->SetWindowText(m_Language[25]);
   GetDlgItem(IDC_CHK_OPTION     )->SetWindowText(m_Language[41]);
   GetDlgItem(IDC_CHK_SCREENSHOT )->SetWindowText(m_Language[156]);
   GetDlgItem(IDC_STATIC_MAP     )->SetWindowText(m_Language[101]);
   GetDlgItem(IDC_STATIC_COMBAT  )->SetWindowText(m_Language[157]);

   GetDlgItem(IDC_STR_TITLE                  )->SetWindowText(m_Language[162]);
   GetDlgItem(IDC_BTN_ADD                    )->SetWindowText(m_Language[163]);
   GetDlgItem(IDC_BTN_MOD                    )->SetWindowText(m_Language[164]);
   GetDlgItem(IDC_BTN_DEL                    )->SetWindowText(m_Language[165]);
   GetDlgItem(IDC_BTN_RESTAURE_DEFAULT_COLOR )->SetWindowText(m_Language[166]);

   m_bInventory = m_spLauncherConfig->bMacro[0];
   m_bCharacter = m_spLauncherConfig->bMacro[1];
   m_bChatter   = m_spLauncherConfig->bMacro[2];
   m_bGroup     = m_spLauncherConfig->bMacro[3];
   m_bSpell     = m_spLauncherConfig->bMacro[4];
   m_bMacro     = m_spLauncherConfig->bMacro[5];
   m_bTrade     = m_spLauncherConfig->bMacro[6];
   m_bOption    = m_spLauncherConfig->bMacro[7];
   m_bScreenShot= m_spLauncherConfig->bMacro[8];
   m_bSwitchChat= m_spLauncherConfig->bMacro[9];

   m_chInventory  .Format("%C",m_spLauncherConfig->chMacroKey[0]);//Inventory
   m_chCharacter  .Format("%C",m_spLauncherConfig->chMacroKey[1]);//Charecter
   m_chChatter    .Format("%C",m_spLauncherConfig->chMacroKey[2]);//Chatter
   m_chGroup      .Format("%C",m_spLauncherConfig->chMacroKey[3]);//GroupPlay
   m_chSpell      .Format("%C",m_spLauncherConfig->chMacroKey[4]);//Spell
   m_chMacro      .Format("%C",m_spLauncherConfig->chMacroKey[5]);//Macro
   m_chTrade      .Format("%C",m_spLauncherConfig->chMacroKey[6]);//Trade
   m_chOption     .Format("%C",m_spLauncherConfig->chMacroKey[7]);//Option
   m_chScreenShot .Format("%C",m_spLauncherConfig->chMacroKey[8]);//Screenshot
   m_chSwitchChat .Format("%C",m_spLauncherConfig->chMacroKey[9]);//Switch Chat Size

   if(!m_bInventory)
      GetDlgItem(IDC_EDIT_INVENTORY)->EnableWindow(FALSE);
   if(!m_bCharacter)
      GetDlgItem(IDC_EDIT_CHARACTERUI)->EnableWindow(FALSE);
   if(!m_bChatter)
      GetDlgItem(IDC_EDIT_CHATTER)->EnableWindow(FALSE);
   if(!m_bGroup)
      GetDlgItem(IDC_EDIT_GROUPUI)->EnableWindow(FALSE);
   if(!m_bSpell)
      GetDlgItem(IDC_EDIT_SPELLUI)->EnableWindow(FALSE);
   if(!m_bMacro)
      GetDlgItem(IDC_EDIT_MACROUI)->EnableWindow(FALSE);
   if(!m_bTrade)
      GetDlgItem(IDC_EDIT_TRADE)->EnableWindow(FALSE);
   if(!m_bOption)
      GetDlgItem(IDC_EDIT_OPTION)->EnableWindow(FALSE);
   if(!m_bScreenShot)
      GetDlgItem(IDC_EDIT_SCREENSHOT)->EnableWindow(FALSE);

   m_ListColor.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT);
   m_ListColor.InsertColumn(0,"",LVCFMT_LEFT,92);
   
   DrawListColor();


	GetDlgItem(IDC_STATIC_COLOR_PICK)->GetWindowRect(&hsbRect);
	ScreenToClient(&hsbRect);

	GetDlgItem(IDC_STATIC_COLOR)->GetWindowRect(&NewColorRect);
	ScreenToClient(&NewColorRect);



   CWindowDC dc(NULL);
   CSize bmSize;
   //	Set Up HSB
   
   memDC.CreateCompatibleDC(&dc);
   
   LoadMappedBitmap(m_HsbBitmap,IDB_BITMAP_HSB,bmSize);
   hsbWidth = bmSize.cx;
   hsbHeight = bmSize.cy;
   
   hsbRect.InflateRect(-5,-5);
   hsbRect.top += 20;
   hsbRect.left += 10;
   
   m_Centre = CPoint(RADIUS,RADIUS);
   m_Centre += CPoint(hsbRect.left,hsbRect.top);
   
   brightRect = CRect(hsbRect.left+hsbWidth+20,hsbRect.top,hsbRect.left+hsbWidth+20+20,hsbRect.top + hsbHeight);
   CreateBrightDIB();
   CalcRects();
   SetDIBPalette();
   
   m_bInitOver = TRUE;
   m_bINIT     = TRUE;
   UpdateData(FALSE);
   DrawAll();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMacroDialog::ProcessMessage() 
{
   MSG msg;
   while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
   {
      if(!IsDialogMessage(&msg))
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);  
      }
   }
}


void CMacroDialog::OnChkInventory() 
{
	UpdateData(TRUE);
   if(m_bInventory)
      GetDlgItem(IDC_EDIT_INVENTORY)->EnableWindow(TRUE);
   else
      GetDlgItem(IDC_EDIT_INVENTORY)->EnableWindow(FALSE);
}

void CMacroDialog::OnChkCharacterui() 
{
	UpdateData(TRUE);
   if(m_bCharacter)
      GetDlgItem(IDC_EDIT_CHARACTERUI)->EnableWindow(TRUE);
   else
      GetDlgItem(IDC_EDIT_CHARACTERUI)->EnableWindow(FALSE);
}

void CMacroDialog::OnChkChatterui() 
{
	UpdateData(TRUE);
   if(m_bChatter)
      GetDlgItem(IDC_EDIT_CHATTER)->EnableWindow(TRUE);
   else
      GetDlgItem(IDC_EDIT_CHATTER)->EnableWindow(FALSE);
}

void CMacroDialog::OnChkGroupui() 
{
	UpdateData(TRUE);
   if(m_bGroup)
      GetDlgItem(IDC_EDIT_GROUPUI)->EnableWindow(TRUE);
   else
      GetDlgItem(IDC_EDIT_GROUPUI)->EnableWindow(FALSE);
}

void CMacroDialog::OnChkSpellui() 
{
	UpdateData(TRUE);
   if(m_bSpell)
      GetDlgItem(IDC_EDIT_SPELLUI)->EnableWindow(TRUE);
   else
      GetDlgItem(IDC_EDIT_SPELLUI)->EnableWindow(FALSE);
}

void CMacroDialog::OnChkMacroui() 
{
	UpdateData(TRUE);
   if(m_bMacro)
      GetDlgItem(IDC_EDIT_MACROUI)->EnableWindow(TRUE);
   else
      GetDlgItem(IDC_EDIT_MACROUI)->EnableWindow(FALSE);
}

void CMacroDialog::OnChkTrade() 
{
	UpdateData(TRUE);
   if(m_bTrade)
      GetDlgItem(IDC_EDIT_TRADE)->EnableWindow(TRUE);
   else
      GetDlgItem(IDC_EDIT_TRADE)->EnableWindow(FALSE);
}

void CMacroDialog::OnChkOption() 
{
	UpdateData(TRUE);
   if(m_bOption)
      GetDlgItem(IDC_EDIT_OPTION)->EnableWindow(TRUE);
   else
      GetDlgItem(IDC_EDIT_OPTION)->EnableWindow(FALSE);
}

void CMacroDialog::OnChkScreenshot() 
{
	UpdateData(TRUE);
   if(m_bScreenShot)
      GetDlgItem(IDC_EDIT_SCREENSHOT)->EnableWindow(TRUE);
   else
      GetDlgItem(IDC_EDIT_SCREENSHOT)->EnableWindow(FALSE);
}

void CMacroDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(hsbRect.PtInRect(point))
	{
		m_bInMouse = FALSE;
		if(InCircle(point))
		{
			m_nMouseIn = IN_CIRCLE;
		}
		else
		if(InBright(point))
		{
			m_nMouseIn = IN_BRIGHT;
		}
		else
		{
			m_nMouseIn = IN_NOTHING;
		}
		if(m_nMouseIn)
		{
			SetCapture();
			TrackPoint(point);
		}
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CMacroDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(GetCapture() == this)
	{
		ReleaseCapture();
		m_bInMouse = FALSE;
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CMacroDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
   if(GetCapture() == this && m_nMouseIn)
   {
      TrackPoint(point);
   }
   else if(GetCapture() == this && m_bInMouse)
   {
      hsvColor = color.toHSV();
      //SetEditVals();
      CClientDC dc(this);
      DrawMarkers(&dc);
      CalcRects();
      SetDIBPalette();
      
      InvalidateRect(&brightRect,FALSE);
      DrawHSB(&dc);
   }
   CDialog::OnMouseMove(nFlags, point);
}






BOOL CMacroDialog::InCircle(CPoint pt)
{
	return Distance(pt,m_Centre) <= RADIUS;
}

BOOL CMacroDialog::InBright(CPoint pt)
{
	return brightRect.PtInRect(pt);
}

void CMacroDialog::TrackPoint(CPoint pt)
{
	if(m_nMouseIn == IN_CIRCLE)
	{
	   CClientDC dc(this);
		DrawMarkers(&dc);
		
		hsvColor.h = (int)RAD2DEG(AngleFromPoint(pt,m_Centre));
		if(hsvColor.h < 0)
		{
			hsvColor.h += 360;
		}
		hsvColor.s = (int)SCALETOMAX(Distance(pt,m_Centre));
		if(hsvColor.s > 255) hsvColor.s = 255;

		SetDIBPalette();
		CalcRects();


		InvalidateRect(&brightRect,FALSE);

		DrawMarkers(&dc);

		color = hsvColor.toRGB();
      DrawFilledColor(&dc,NewColorRect,color.color());
      //DrawLines(&dc);
		//SetEditVals();
		//DrawLines(&dc);
		//CalcCuboid();
		//DrawRGB(&dc);

	}
	else if(m_nMouseIn == IN_BRIGHT)
	{
		double d;
		d = brightRect.bottom - pt.y;
		d *= 255;
		d /= brightRect.Height();
		if(d < 0 ) d = 0;
		if(d > 255) d = 255;
		CClientDC dc(this);
		DrawMarkers(&dc);
		hsvColor.v = (int)d;
		CalcRects();
		DrawMarkers(&dc);

		color = hsvColor.toRGB();
      DrawFilledColor(&dc,NewColorRect,color.color());
		//SetEditVals();
		//DrawLines(&dc);
		//CalcCuboid();
		//DrawRGB(&dc);

	}
}

void CMacroDialog::DrawMarkers(CDC *pDC)
{
   if(m_CurrentRect.Width())
   {
      CPen *oldPen;
      CBrush *oldBrush;
      int oldMode;
      CRect cr = m_CurrentRect;
      
      oldPen = (CPen *)pDC->SelectStockObject(WHITE_PEN);
      oldBrush = (CBrush *)pDC->SelectStockObject(NULL_BRUSH);
      
      oldMode = pDC->SetROP2(R2_XORPEN);
      
      pDC->Rectangle(&cr);
      CPen pen;
      pen.CreatePen(PS_SOLID,2,RGB(255,255,255));
      pDC->SelectObject(&pen);
      pDC->Rectangle(&brightMark);
      
      pDC->SelectObject(oldPen);
      pDC->SelectObject(oldBrush);
      pDC->SetROP2(oldMode);
      pen.DeleteObject();
   }
}

void CMacroDialog::SetDIBPalette()
{
   BYTE palette[768],*p;
   HSVType h = hsvColor;
   double d;
   d = 255.0/brightRect.Height();
   p = palette;
   for(int i=brightRect.Height()-1; i >= 0 ;i--,p+=3)
   {
      h.v = (int)((double)i * d);
      RGBType rgb = h.toRGB();
      p[0] = rgb.r;
      p[1] = rgb.g;
      p[2] = rgb.b;
   }
   m_BrightDIB.SetPalette(palette);
}

void CMacroDialog::CalcRects()
{
   CPoint pt;
   pt = PtFromAngle(hsvColor.h,hsvColor.s,m_Centre);
   m_CurrentRect = CRect(pt.x - RECT_WIDTH,pt.y - RECT_WIDTH,pt.x+RECT_WIDTH,pt.y + RECT_WIDTH);	
   
   int y;
   y = (int)(((double)hsvColor.v/255)*brightRect.Height()); 	
   y = brightRect.bottom - y;
   brightMark = CRect(brightRect.left - 2, y - 4, brightRect.right+2,y+4);
}


void CMacroDialog::DrawHSB(CDC *pDC)
{
   if(m_HsbBitmap.GetSafeHandle())
   {
      CBitmap *pOldBitmap ;
      pOldBitmap  = (CBitmap *)memDC.SelectObject(&m_HsbBitmap);
      pDC->BitBlt(hsbRect.left,hsbRect.top,hsbWidth,hsbHeight,&memDC,0,0,SRCCOPY);
      m_BrightDIB.BitBlt(pDC->m_hDC,brightRect.left,brightRect.top,brightRect.Width(),brightRect.Height(),0,0);
      DrawMarkers(pDC);
      memDC.SelectObject(pOldBitmap);
   }
}

void CMacroDialog::LoadMappedBitmap(CBitmap& bitmap,UINT nIdResource,CSize& size)
{
   CBitmap *pOldBitmap;
   
   if(bitmap.GetSafeHandle()) bitmap.DeleteObject();
   
   if(bitmap.LoadBitmap(nIdResource))
   {
      
      int width,height;
      BITMAP bmInfo;
      ::GetObject(bitmap.m_hObject,sizeof(bmInfo),&bmInfo);	
      width = bmInfo.bmWidth;
      height = bmInfo.bmHeight;
      
      COLORREF colorWindow = ::GetSysColor(COLOR_3DFACE);
      COLORREF sourceColor = RGB(192,192,192);
      
      pOldBitmap = (CBitmap *)memDC.SelectObject(&bitmap);
      
      int i,j;
      
      for(int i=0; i < height; i++)
      {
         for(j=0; j < width; j++)
         {
            if(memDC.GetPixel(j,i) == sourceColor)
            {
               memDC.SetPixel(j,i,colorWindow);
            }
         }
      }
      
      memDC.SelectObject(&pOldBitmap);
      size = CSize(width,height);
   }
   
}

void CMacroDialog::CreateBrightDIB()
{
   CDIB& d = m_BrightDIB;
   d.Create(brightRect.Width(),brightRect.Height(),8);
   for(int i=0; i < d.Height(); i++)
   {
      memset(d.GetLinePtr(i),i,d.Width());
   }
}

void CMacroDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
   DrawFilledColor(&dc,OldColorRect,m_OldColor.color());
	DrawHSB(&dc);
}

void CMacroDialog::DrawFilledColor(CDC *pDC,CRect cr,COLORREF c)
{
	pDC->FillSolidRect(&cr,c);
	pDC->Draw3dRect(&cr,RGB(0,0,0),RGB(0,0,0));
	cr.InflateRect(-1,-1);
	pDC->Draw3dRect(&cr,RGB(192,192,192),RGB(128,128,128));
}

void CMacroDialog::DrawAll()
{
	if(m_bInitOver && !m_bInDrawAll)
	{
	   CClientDC dc(this);
		DrawMarkers(&dc);
		DrawFilledColor(&dc,NewColorRect,color.color());
		m_bInDrawAll = TRUE;
		CalcRects();
		SetDIBPalette();
		DrawHSB(&dc);
		m_bInDrawAll = FALSE;
	}
}

void  CMacroDialog::DrawListColor()
{
   m_ListColor.DeleteAllItems();
   for(int i=0;i<m_spLauncherConfig->dwNbrColor;i++)
   {
      CString strTmp;
      //strTmp.Format("%03d,%03d,%03d",GetRValue(m_spLauncherConfig->CCColor[i]),GetGValue(m_spLauncherConfig->CCColor[i]),GetBValue(m_spLauncherConfig->CCColor[i]));
      strTmp.Format("0x%06X",m_spLauncherConfig->CCColor[i]);
      m_ListColor.InsertItem(i,strTmp,0);
   }
}

void CMacroDialog::OnBtnAdd() 
{
   int dwColor = RGB(color.r,color.g,color.b);
   m_spLauncherConfig->CCColor.Add(dwColor);
   m_spLauncherConfig->dwNbrColor++;
   DrawListColor();
}

void CMacroDialog::OnBtnDel() 
{
   POSITION pos = m_ListColor.GetFirstSelectedItemPosition();
   if (pos)
   {
      int nItem = m_ListColor.GetNextSelectedItem(pos);
      if(nItem == 0)
      {
         MessageBox("You cant change or delete COLOR 0...","Error !",MB_OK+MB_ICONSTOP);
         return;
      }
      m_spLauncherConfig->CCColor.RemoveAt(nItem);
      m_spLauncherConfig->dwNbrColor--;
   }
   DrawListColor();
}



void CMacroDialog::OnBtnMod() 
{
	POSITION pos = m_ListColor.GetFirstSelectedItemPosition();
   if (pos)
   {
      int nItem = m_ListColor.GetNextSelectedItem(pos);
      if(nItem == 0)
      {
         MessageBox("You cant change or delete COLOR 0...","Error !",MB_OK+MB_ICONSTOP);
         return;
      }
      int dwColor = RGB(color.r,color.g,color.b);
      m_spLauncherConfig->CCColor[nItem] = dwColor;
   }
	DrawListColor();
}

void CMacroDialog::OnBtnRestaureDefaultColor() 
{
   m_spLauncherConfig->CCColor.RemoveAll();
   m_spLauncherConfig->CCColor.Add(RGB(17,181,205));
   m_spLauncherConfig->CCColor.Add(RGB(255,33,75));	
   m_spLauncherConfig->CCColor.Add(RGB(90,119,216));
   m_spLauncherConfig->CCColor.Add(RGB(220,202,6));
   m_spLauncherConfig->CCColor.Add(RGB(5,197,37));
   m_spLauncherConfig->CCColor.Add(RGB(199,10,204));
   m_spLauncherConfig->CCColor.Add(RGB(192,192,192));
   m_spLauncherConfig->CCColor.Add(RGB(224,100,10));
   m_spLauncherConfig->dwNbrColor = 8;
   DrawListColor();
}
