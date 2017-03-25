// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "t4clauncher.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_SHOW_MACRO_DIALOG      (WM_USER +2280)
#define TIMER_SHOW_MACRO_DIALOG_KILL (WM_USER +2281)


/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

#define NM_TIMER_UPDATELIST (WM_USER)+2277


CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_strName = _T("");
	m_strIP = _T("");
	m_strLogin = _T("");
	m_strPass = _T("");
	m_bRes800 = FALSE;
	m_bWindowed = FALSE;
	m_dwCBLanguage = 0;
	m_bRes640 = FALSE;
	m_bLauncherMusic = TRUE;
	//}}AFX_DATA_INIT

   m_bINIT     = FALSE;
   m_dwAnimPos = 0;
   m_dwSelectItemList = -1;

   m_pMacroDlg = NULL;
   m_bMessageMacro = FALSE;
}


void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_STATIC_LAUNCHER_SOUND, m_staticMusique);
	DDX_Control(pDX, IDC_STATIC_640x480, m_static640);
	DDX_Control(pDX, IDC_BTN_MACRO, m_btnMacro);
	DDX_Control(pDX, IDC_STATIC_WINDOWED, m_staticWindowed);
	DDX_Control(pDX, IDC_STATIC_800X600, m_static800);
	DDX_Control(pDX, IDC_BTN_FERMER, m_btnClose);
	DDX_Control(pDX, IDC_STR_DLG_SERVER_TITLE, m_staticTitle);
	DDX_Control(pDX, IDC_STATIC_PASSWORD, m_staticPass);
	DDX_Control(pDX, IDC_STATIC_NAME, m_staticName);
	DDX_Control(pDX, IDC_STATIC_LOGIN, m_staticLogin);
	DDX_Control(pDX, IDC_STATIC_IP, m_staticIP);
	DDX_Control(pDX, IDC_STR_PASSWORD, m_editServerPass);
	DDX_Control(pDX, IDC_STR_LOGIN, m_editServerLogin);
	DDX_Control(pDX, IDC_STR_IP, m_editServerIP);
	DDX_Control(pDX, IDC_STR_NAME, m_editServerName);
	DDX_Control(pDX, IDC_LIST_CONNECTION, m_listServer);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAddInf);
	DDX_Control(pDX, IDC_BTN_DEL, m_btnDelete);
	DDX_Control(pDX, IDOK, m_btnFremer);
	DDX_Text(pDX, IDC_STR_NAME, m_strName);
	DDX_Text(pDX, IDC_STR_IP, m_strIP);
	DDX_Text(pDX, IDC_STR_LOGIN, m_strLogin);
	DDX_Text(pDX, IDC_STR_PASSWORD, m_strPass);
	DDX_Check(pDX, IDC_CHECK_800X600, m_bRes800);
	DDX_Check(pDX, IDC_CHECK_WINDOWED, m_bWindowed);
	DDX_CBIndex(pDX, IDC_CB_LANGUAGE, m_dwCBLanguage);
	DDX_Check(pDX, IDC_CHECK_640X480, m_bRes640);
	DDX_Check(pDX, IDC_CHECK_LAUNCHER_SOUND, m_bLauncherMusic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
   ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, OnBtnDel)
	ON_BN_CLICKED(IDC_BTN_FERMER, OnBtnFermer)
	ON_CBN_SELCHANGE(IDC_CB_LANGUAGE, OnSelchangeCbLanguage)
	ON_BN_CLICKED(IDC_BTN_MACRO, OnBtnMacro)
	ON_BN_CLICKED(IDC_CHECK_800X600, OnCheck800x600)
	ON_BN_CLICKED(IDC_CHECK_640X480, OnCheck640x480)
	ON_BN_CLICKED(IDC_CHECK_LAUNCHER_SOUND, OnCheckLauncherSound)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

void CServerDlg::SetDlgPos(int dwLeft, int dwTop)
{
   m_dwLeft = dwLeft;
   m_dwTop  = dwTop;
}

BOOL CServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

   m_bRes640 = FALSE;
   m_bRes800 = FALSE;
   if(m_spLauncherConfig->bResolution == 1)
      m_bRes800 = TRUE;
   else if(m_spLauncherConfig->bResolution == 2)
      m_bRes640 = TRUE;
   m_bWindowed      = m_spLauncherConfig->bWindowed;
   m_dwCBLanguage   = m_spLauncherConfig->dwLanguage;
   m_bLauncherMusic = m_spLauncherConfig->bLauncherMusic;


   m_Language.Release();
   m_Language.Create(m_dwCBLanguage);
	
   LoadSetSkin();

   m_btnFremer.SetFontName("T4C Beaulieux");
   m_btnFremer.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnFremer.SetFontSize(16);
   m_btnFremer.SetFontBold(TRUE);
   m_btnFremer.SetForeColor(RGB(200,200,200));
   

   m_btnClose.SetFontName("T4C Beaulieux");
   m_btnClose.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnClose.SetFontSize(12);
   m_btnClose.SetFontBold(TRUE);
   m_btnClose.SetForeColor(RGB(0,0,0));

   m_btnAddInf.SetFontName("T4C Beaulieux");
   m_btnAddInf.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnAddInf.SetFontSize(12);
   m_btnAddInf.SetFontBold(TRUE);
   m_btnAddInf.SetForeColor(RGB(0,0,0));

   m_btnDelete.SetFontName("T4C Beaulieux");
   m_btnDelete.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnDelete.SetFontSize(12);
   m_btnDelete.SetFontBold(TRUE);
   m_btnDelete.SetForeColor(RGB(0,0,0));

   m_btnMacro.SetFontName("T4C Beaulieux");
   m_btnMacro.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnMacro.SetFontSize(11);
   m_btnMacro.SetFontBold(TRUE);
   m_btnMacro.SetForeColor(RGB(0,0,0));
   m_btnMacro.SetScale(110,100);

   m_listServer.SetExtendedStyle(LVS_EX_FLATSB);
   m_listServer.SetWindowPos(NULL,45,58,215,225,NULL);

 
   CRect rc;
   m_listServer.GetWindowRect(&rc);
   ScreenToClient(&rc);
   m_listServer.bSetBGImageTransRes(FALSE,RGB(0,0,0),IDB_BACK_SERVER_DLG,rc.left,rc.top);
   m_listServer.vSetRowHeight(17);
   m_listServer.SetFontSize(10);
   m_listServer.SetFontName("Arial");
   m_listServer.SetFontBold(TRUE);
   m_listServer.SettextColor(RGB(200,200,200),RGB(150,10,10));
   m_listServer.InsertColumn(0,"Name",LVCFMT_LEFT,rc.Width()-20);

   m_editServerName .SetTextColor(RGB(200,200,200));
   m_editServerIP   .SetTextColor(RGB(200,200,200));
   m_editServerLogin.SetTextColor(RGB(200,200,200));
   m_editServerPass .SetTextColor(RGB(200,200,200));

   
   
   m_staticName.SetFontBold(TRUE);
   m_staticName.SetFontName("Arial");
   m_staticName.SetFontSize(10);
   m_staticName.SetForeColor(RGB(200,200,200));
   m_staticName.SetFont3D(TRUE,uicLabel::Type3D::Sunken);
   m_staticName.SetCenter(2);
   
   m_staticIP.SetFontBold(TRUE);
   m_staticIP.SetFontName("Arial");
   m_staticIP.SetFontSize(10);
   m_staticIP.SetForeColor(RGB(200,200,200));
   m_staticIP.SetFont3D(TRUE,uicLabel::Type3D::Sunken);
   m_staticIP.SetCenter(2);

   m_staticLogin.SetFontBold(TRUE);
   m_staticLogin.SetFontName("Arial");
   m_staticLogin.SetFontSize(10);
   m_staticLogin.SetForeColor(RGB(200,200,200));
   m_staticLogin.SetFont3D(TRUE,uicLabel::Type3D::Sunken);
   m_staticLogin.SetCenter(2);

   m_staticPass.SetFontBold(TRUE);
   m_staticPass.SetFontName("Arial");
   m_staticPass.SetFontSize(10);
   m_staticPass.SetForeColor(RGB(200,200,200));
   m_staticPass.SetFont3D(TRUE,uicLabel::Type3D::Sunken);
   m_staticPass.SetCenter(2);

   m_staticWindowed.SetFontBold(TRUE);
   m_staticWindowed.SetFontName("Arial");
   m_staticWindowed.SetFontSize(10);
   m_staticWindowed.SetForeColor(RGB(200,200,200));
   m_staticWindowed.SetFont3D(TRUE,uicLabel::Type3D::Sunken);
   m_staticWindowed.SetCenter(0);

   m_static800.SetFontBold(TRUE);
   m_static800.SetFontName("Arial");
   m_static800.SetFontSize(10);
   m_static800.SetForeColor(RGB(200,200,200));
   m_static800.SetFont3D(TRUE,uicLabel::Type3D::Sunken);
   m_static800.SetCenter(0);

   m_static640.SetFontBold(TRUE);
   m_static640.SetFontName("Arial");
   m_static640.SetFontSize(10);
   m_static640.SetForeColor(RGB(200,200,200));
   m_static640.SetFont3D(TRUE,uicLabel::Type3D::Sunken);
   m_static640.SetCenter(0);

   m_staticMusique.SetFontBold(TRUE);
   m_staticMusique.SetFontName("Arial");
   m_staticMusique.SetFontSize(10);
   m_staticMusique.SetForeColor(RGB(200,200,200));
   m_staticMusique.SetFont3D(TRUE,uicLabel::Type3D::Sunken);
   m_staticMusique.SetCenter(0);


 



   m_staticTitle.SetFontBold(TRUE);
   m_staticTitle.SetFontName("Arial");
   m_staticTitle.SetFontSize(10);
   m_staticTitle.SetForeColor(RGB(222, 158,  0));
   m_staticTitle.SetText("--[ Setup Windows ]--");
   m_staticTitle.SetFont3D(TRUE,uicLabel::Type3D::Sunken);


   m_btnClose .SetText(m_Language[142]);
   m_btnAddInf.SetText(m_Language[143]);
   m_btnDelete.SetText(m_Language[141]);
   m_btnMacro .SetText(m_Language[154]);

   m_staticWindowed.SetText(m_Language[144]);
   m_staticName    .SetText(m_Language[145]);
   m_staticIP      .SetText(m_Language[146]);
   m_staticLogin   .SetText(m_Language[147]);
   m_staticPass    .SetText(m_Language[148]);
   m_static800     .SetText(m_Language[153]);
   m_static640     .SetText(m_Language[159]);
   m_staticMusique .SetText(m_Language[167]);


  

   m_TxtCB.CreateFont(16, 8, 0, 0, FW_BOLD, FALSE, FALSE,0,0,0,0,0,0, "Arial");
   GetDlgItem(IDC_CB_SERVER_LIST)->GetWindowRect(&rc);
   ScreenToClient(rc);
   m_pDropServerList.Create("", rc, this, BTN_ID_DROP_SERVERL_BTN,
						          RGB(0,0,0), RGB(0,0,0),
						          RGB(200,200,200), RGB(222, 158,  0), &m_TxtCB);

   m_pDropServerList.SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),NULL);
   m_pDropServerList.GetWindowRect(&rc); 
   ScreenToClient(&rc);
   m_pDropServerList.bSetBGImageTransRes(FALSE,RGB(0,0,0),IDB_BACK_SERVER_DLG,rc.left,rc.top);
	// Set OPTIONAL parameters
	m_pDropServerList.SetListHeight(rc.Height()*3);		// Set the Width Height
	m_pDropServerList.SetListWidth(rc.Width());	      // Set the List Width
	m_pDropServerList.ChangeTitleWithSelection(true);   // When the users selects an item change the title in the top


   m_dwNewLanguage = -1;
   UpdateServerListIP();
   m_pDropServerList.SetSelection(0);

   UpdateServerList();


   SetTimer(NM_TIMER_UPDATELIST,250,0);
   m_bINIT = TRUE;
   UpdateData(FALSE);
	return TRUE;

}

void CServerDlg::OnCancel() 
{
   return;
}

void CServerDlg::OnOK() 
{
   KillTimer(NM_TIMER_UPDATELIST);
   m_bINIT = FALSE;
   CDialog::OnOK();
   
}

void CServerDlg::OnBtnFermer() 
{
	OnOK();
}


void CServerDlg::OnDestroy() 
{
   m_Language.Release();
	CDialog::OnDestroy();
}

BOOL CServerDlg::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(rc);
	if (TileBitmap(pDC, rc) == TRUE)
	{
		return TRUE;
	}
	return CDialog::OnEraseBkgnd(pDC);
}

void CServerDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CServerDlg::OnTimer(UINT nIDEvent) 
{
   switch(nIDEvent)
   {
      
      case NM_TIMER_UPDATELIST:
      {
         UpdateData(TRUE);

         // UpdateIP Server List
         CDropList* pList = m_pDropServerList.GetList();
         int dwSelTmp = pList->GetCurSel();
         if(dwSelTmp != 0)
         {
            CString strT;
            pList->GetText(dwSelTmp,strT);
            if(strT != m_strIP)
            {
               m_strIP = strT;
               UpdateData(FALSE);
               CRect rc(399,141,594,161);
               RedrawWindow(rc);
            }
         }


         dwSelTmp = -1;
         POSITION pos = m_listServer.GetFirstSelectedItemPosition();
         if(pos)
            dwSelTmp = m_listServer.GetNextSelectedItem(pos);
         if(dwSelTmp != m_dwSelectItemList && dwSelTmp >=0)
         {
            m_strName .Format("%s",m_spLauncherConfig->pConnection[dwSelTmp].strName);
            m_strIP   .Format("%s",m_spLauncherConfig->pConnection[dwSelTmp].strIP);
            m_strLogin.Format("%s",m_spLauncherConfig->pConnection[dwSelTmp].strLogin);
            m_strPass .Format("%s",m_spLauncherConfig->pConnection[dwSelTmp].strPass);
            m_dwSelectItemList = dwSelTmp;
            UpdateData(FALSE);
            CRect rc(394,63,609,228);
            RedrawWindow(&rc);
            m_pDropServerList.SetSelection(0);
         }
         else if(dwSelTmp != m_dwSelectItemList && dwSelTmp ==-1)
         {
            m_strName = "";
            m_strIP   = "";
            m_strLogin= "";
            m_strPass = "";
            m_dwSelectItemList = dwSelTmp;
            UpdateData(FALSE);
            CRect rc(394,63,609,228);
            RedrawWindow(&rc);
            m_pDropServerList.SetSelection(0);
         }

         if(!m_bRes800 && !m_bRes640)
            m_spLauncherConfig->bResolution = FALSE;
         else if(m_bRes800)
            m_spLauncherConfig->bResolution = 1;
         else if(m_bRes640)
            m_spLauncherConfig->bResolution = 2;
         else
            m_spLauncherConfig->bResolution = 0;
         m_spLauncherConfig->bWindowed      = m_bWindowed;
         m_spLauncherConfig->bLauncherMusic = m_bLauncherMusic;


         
         UpdateData(FALSE);
      }         
      break;
      case TIMER_SHOW_MACRO_DIALOG:
         if(m_pMacroDlg && !m_pMacroDlg->IsInit())
         {
            KillTimer(TIMER_SHOW_MACRO_DIALOG);
            m_bMessageMacro = FALSE;
            SetTimer(TIMER_SHOW_MACRO_DIALOG_KILL,200,0);
         }
         else if(!m_pMacroDlg)
            KillTimer(TIMER_SHOW_MACRO_DIALOG);
      break;
      case TIMER_SHOW_MACRO_DIALOG_KILL:
         KillTimer(TIMER_SHOW_MACRO_DIALOG_KILL);
         if(m_pMacroDlg)
         {
            m_pMacroDlg->DestroyWindow();
            delete m_pMacroDlg;
            m_pMacroDlg = NULL;
         }
         //Reset tous les control de ce dialog...
         GetDlgItem(IDC_LIST_CONNECTION)->EnableWindow(TRUE);
         GetDlgItem(IDC_STR_NAME)->EnableWindow(TRUE);
         GetDlgItem(IDC_STR_IP)->EnableWindow(TRUE);
         GetDlgItem(IDC_STR_LOGIN)->EnableWindow(TRUE);
         GetDlgItem(IDC_STR_PASSWORD)->EnableWindow(TRUE);
         GetDlgItem(IDC_BTN_ADD)->EnableWindow(TRUE);
         GetDlgItem(IDC_BTN_DEL)->EnableWindow(TRUE);
         GetDlgItem(IDC_CHECK_WINDOWED)->EnableWindow(TRUE);
         GetDlgItem(IDC_CHECK_800X600)->EnableWindow(TRUE);
         GetDlgItem(IDC_CB_LANGUAGE)->EnableWindow(TRUE);
         GetDlgItem(IDC_BTN_MACRO)->EnableWindow(TRUE);
         GetDlgItem(IDC_BTN_FERMER)->EnableWindow(TRUE);
         GetDlgItem(IDOK)->EnableWindow(TRUE);
         m_pDropServerList.EnableWindow(TRUE);
         
      break;
   }
	
	CDialog::OnTimer(nIDEvent);
}

///////////////////////////////////////////////////////////////////////////////////////////
// SKIN
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CServerDlg::LoadSetSkin()
{
	DeleteSkinBmp();
	// Set les bitmap du skin Interne
   m_bmpBackground  .LoadBitmap(IDB_BACK_SERVER_DLG);
   m_bmpBtnFrame2   .LoadBitmap(IDB_BTN_FRAME2);
   m_bmpBtnFrame3   .LoadBitmap(IDB_BTN_FRAME3);
   m_bmpBtnFrame4   .LoadBitmap(IDB_BTN_FRAME4);
   
	m_btnFremer .SetBitmap(&m_bmpBtnFrame2);
   m_btnAddInf .SetBitmap(&m_bmpBtnFrame3);
   m_btnDelete .SetBitmap(&m_bmpBtnFrame3);
   m_btnClose  .SetBitmap(&m_bmpBtnFrame3);
   m_btnMacro  .SetBitmap(&m_bmpBtnFrame4);
   
  
	Invalidate();
	return TRUE;
}

BOOL CServerDlg::DeleteSkinBmp()
{
	m_bmpBackground    .DeleteObject();
   m_bmpBtnFrame2     .DeleteObject();
   m_bmpBtnFrame3     .DeleteObject();
   m_bmpBtnFrame4     .DeleteObject();
	return TRUE;
}

BOOL CServerDlg::TileBitmap(CDC* _pDC, CRect _rc)
{
	CDC			MemDC;
	CBitmap		*pOldBitmap;
	BITMAP		bm;
	int			x = 0;
	int			y = 0;
	

	//** If there is a bitmap loaded
	if (m_bmpBackground.m_hObject != NULL)
	{
		MemDC.CreateCompatibleDC(_pDC);
		pOldBitmap = MemDC.SelectObject(&m_bmpBackground);
		m_bmpBackground.GetBitmap(&bm);
		//** Tile the bitmap
		while (y < _rc.Height()) 
		{
			while(x < _rc.Width()) 
			{
				_pDC->BitBlt(x, y, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
				x += bm.bmWidth;
			}
			x = 0;
			y += bm.bmHeight;
		}
		MemDC.SelectObject(pOldBitmap);
		return TRUE;
	}
	return FALSE; 
}

void CServerDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(rc);
	if (TileBitmap(&dc, rc) == TRUE)
	{
		return ;
	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CServerDlg::UpdateServerList()
{
   m_listServer.DeleteAllItems();

   for(int i=0;i<m_spLauncherConfig->dwNbrConnection;i++)
      m_listServer.InsertItem(i,m_spLauncherConfig->pConnection[i].strName,0);
}

void CServerDlg::OnBtnAdd() 
{
   UpdateData(TRUE);
   //Check all data if not Empty...
   
   if(m_strName == "" || m_strIP == "" || m_strLogin == "" || m_strPass == "")
   {
      MessageBox("Server Name,  IP ,  Login or password are EMPTY...","Error !",MB_OK+MB_ICONSTOP);
      return;
   }
   
   sConnectParam NewConnection;
   sprintf(NewConnection.strName ,"%s",m_strName.GetBuffer(0));
   sprintf(NewConnection.strIP   ,"%s",m_strIP.GetBuffer(0));
   sprintf(NewConnection.strLogin,"%s",m_strLogin.GetBuffer(0));
   sprintf(NewConnection.strPass ,"%s",m_strPass.GetBuffer(0));
   
   m_spLauncherConfig->pConnection.Add(NewConnection);
   m_spLauncherConfig->dwNbrConnection++;
   
   UpdateServerList();
   
   m_strName  = "";
   m_strIP    = "";
   m_strLogin = "";
   m_strPass  = "";
   UpdateData(FALSE);
   RedrawWindow();
}


void CServerDlg::OnBtnDel() 
{
   int dwSelTmp = -1;
   POSITION pos = m_listServer.GetFirstSelectedItemPosition();
   if(pos)
      dwSelTmp = m_listServer.GetNextSelectedItem(pos);

   if(dwSelTmp == -1)
   {
      MessageBox("NO Server Selected...","Error !",MB_OK+MB_ICONSTOP);
      return;
   }
   m_spLauncherConfig->pConnection.RemoveAt(dwSelTmp);
   m_spLauncherConfig->dwNbrConnection--;
   UpdateServerList();
}

void CServerDlg::ProcessMessage() 
{
   if(m_bMessageMacro)
   {
      m_pMacroDlg ->ProcessMessage();
   }

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

void CServerDlg::UpdateServerListIP()
{

   // Set OPTIONAL paramters
   CDropList* pList = m_pDropServerList.GetList();
   pList->ResetContent();

	pList->SetBkColor(RGB(50,50,50));  		// List background COlor
	pList->SetHighlightBkColor(RGB(150,10,10)); 	// List highlight background color

   CString strFile;
   strFile.Format("%s\\serverlist.txt",m_strPathGame);

   pList->AddString(m_Language[149],0,RGB(255,255,255),RGB(255,255,255));

   char strLine[1000];
   char *pRet;
   FILE *pf1;
   pf1 = fopen(strFile.GetBuffer(0),"rt");
   if(pf1)
   {
      do 
      {
         pRet = fgets(strLine,1000,pf1);
         if(pRet)
         {
            strLine[strlen(strLine)] = 0x00;//BLBLBL petit bug ici, le dernier caractère saute à chaque fois si on met -1, je vire le -1 dans le tableau
            pList->AddString(strLine,0,RGB(255,255,255),RGB(255,255,255));
         }
      } while(pRet );
   }
}

void CServerDlg::OnSelchangeCbLanguage() 
{
	UpdateData(TRUE);
   if(m_dwCBLanguage != m_spLauncherConfig->dwLanguage)
   {
      m_spLauncherConfig->dwLanguage = m_dwCBLanguage;
      m_dwNewLanguage = m_dwCBLanguage;

      m_Language.Release();
      m_Language.Create(m_spLauncherConfig->dwLanguage);

      // change la langue ici :P


      m_btnClose .SetText(m_Language[142]);
      m_btnAddInf.SetText(m_Language[143]);
      m_btnDelete.SetText(m_Language[141]);
      m_btnMacro .SetText(m_Language[154]);
      
      
      m_staticWindowed.SetText(m_Language[144]);
      m_staticName    .SetText(m_Language[145]);
      m_staticIP      .SetText(m_Language[146]);
      m_staticLogin   .SetText(m_Language[147]);
      m_staticPass    .SetText(m_Language[148]);
      m_static800     .SetText(m_Language[153]);
      m_static640     .SetText(m_Language[159]);
      m_staticMusique .SetText(m_Language[167]);
   }
}

void CServerDlg::OnBtnMacro() 
{
   m_pMacroDlg = new CMacroDialog();
   m_pMacroDlg->SetServerList(m_spLauncherConfig);
   //m_pMacroDlg->SetPath(m_strPath.GetBuffer(0));

   m_pMacroDlg->Create(IDD_MACRO_DLG,this);
   m_pMacroDlg->CenterWindow();
   m_pMacroDlg->ShowWindow(SW_SHOW);
   SetTimer(TIMER_SHOW_MACRO_DIALOG,250,NULL);
   m_bMessageMacro = TRUE;

   //Reset tous les control de ce dialog...
   GetDlgItem(IDC_LIST_CONNECTION)->EnableWindow(FALSE);
   GetDlgItem(IDC_STR_NAME)->EnableWindow(FALSE);
   GetDlgItem(IDC_STR_IP)->EnableWindow(FALSE);
   GetDlgItem(IDC_STR_LOGIN)->EnableWindow(FALSE);
   GetDlgItem(IDC_STR_PASSWORD)->EnableWindow(FALSE);
   GetDlgItem(IDC_BTN_ADD)->EnableWindow(FALSE);
   GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
   GetDlgItem(IDC_CHECK_WINDOWED)->EnableWindow(FALSE);
   GetDlgItem(IDC_CHECK_800X600)->EnableWindow(FALSE);
   GetDlgItem(IDC_CB_LANGUAGE)->EnableWindow(FALSE);
   GetDlgItem(IDC_BTN_MACRO)->EnableWindow(FALSE);
   GetDlgItem(IDC_BTN_FERMER)->EnableWindow(FALSE);
   GetDlgItem(IDOK)->EnableWindow(FALSE);
   m_pDropServerList.EnableWindow(FALSE);
}

void CServerDlg::OnCheck800x600() 
{
   UpdateData(TRUE);
   if(m_bRes800)
      m_bRes640 = FALSE;
   UpdateData(FALSE);	
}

void CServerDlg::OnCheck640x480() 
{
	UpdateData(TRUE);
   if(m_bRes640)
      m_bRes800 = FALSE;
   UpdateData(FALSE);	
}

void CServerDlg::OnCheckLauncherSound() 
{
	UpdateData(TRUE);
   

   UpdateData(FALSE);	
}
