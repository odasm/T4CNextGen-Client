// MainDialog.cpp : implementation file
// Player.Password[i] ^= 128;

#include "stdafx.h"
#include "t4clauncher.h"
#include "MainDialog.h"
#include "Zlib/zlib.h"

#define devices 2
#define BASSDEF(f) (WINAPI *f[devices])	// define the functions as pointers
#include "bass.h"

#ifdef _DEBUG 
#define new DEBUG_NEW  
#undef THIS_FILE  
static char THIS_FILE[] = __FILE__;
#endif 

static char bassfile[devices-1][MAX_PATH];	// temporary BASS.DLL files
static HINSTANCE bass[devices]={0};		// bass handles
static HSTREAM   g_shChan[2];	// the streams

#define TIMER_INTRO_FIRST_PLAY        (WM_USER +2266)
#define TIMER_INTRO_DLS               (WM_USER +2267)
#define TIMER_SHOW_SERVER_DIALOG      (WM_USER +2268)
#define TIMER_SHOW_SERVER_DIALOG_KILL (WM_USER +2269)
#define TIMER_CHECK_CONNECT_STAT      (WM_USER +2270)
#define TIMER_CREDIT_FIRST_PLAY       (WM_USER +2271)

#define  MEM_COMPRESS_NEED(ActualLen) (ULONG)(ActualLen *1.1 + 12);

 
/////////////////////////////////////////////////////////////////////////////
// CMainDialog dialog


CMainDialog::CMainDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDialog)
	m_strDirectIP = _T("");
	m_strDirectLogin = _T("");
	m_strDirectPassword = _T("");
	//}}AFX_DATA_INIT

   m_pServerDlg    = NULL;
   m_pNMShowAVI    = NULL;
   m_bInit         = FALSE;
   m_bIntroStatus  = FALSE;
   m_bCreditStatus = FALSE;
   m_bMessageServer   = FALSE;
   m_dwAction      = LAST_CONNECT_ACTION_IDLE;

}


void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDialog)
	DDX_Control(pDX, IDC_MSTATIC_PASSWORD, m_staticPassword);
	DDX_Control(pDX, IDC_MSTATIC_LOGIN, m_staticLogin);
	DDX_Control(pDX, IDC_MSTATIC_IP, m_staticIP);
	DDX_Control(pDX, IDCM_STR_PASSWORD, m_ctrlPassword);
	DDX_Control(pDX, IDCM_STR_LOGIN, m_ctrlLogin);
	DDX_Control(pDX, IDCM_STR_IP, m_ctrlIP);
	DDX_Control(pDX, IDC_LIST_INFO, m_ListInfo);
	DDX_Control(pDX, IDC_CONNECT_STATUS_GEN, m_staticConnectGeneral);
	DDX_Control(pDX, IDC_STR_WEB_LINK1, m_staticLinkWeb1); // steph ajout
	DDX_Control(pDX, IDC_STR_WEB_LINK2, m_staticLinkWeb2); // steph ajout
	DDX_Control(pDX, IDC_STR_DLG_TITLE, m_staticTitle);
	DDX_Control(pDX, IDC_BTN_MINIMIZE, m_btnMinimize);
	DDX_Control(pDX, IDC_BTN_EXIT, m_btnQuitter);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_btnConnect);
	DDX_Control(pDX, IDC_BTN_CREDIT, m_btnCredit);
	DDX_Control(pDX, IDC_BTN_SERVER_INFO, m_btnServer);
	DDX_Control(pDX, IDC_BTN_INTRO, m_btnIntro);
	DDX_Text(pDX, IDCM_STR_IP, m_strDirectIP);
	DDX_Text(pDX, IDCM_STR_LOGIN, m_strDirectLogin);
	DDX_Text(pDX, IDCM_STR_PASSWORD, m_strDirectPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	//{{AFX_MSG_MAP(CMainDialog)
   ON_WM_ERASEBKGND()
   ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_INTRO, OnBtnIntro)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_MINIMIZE, OnBtnMinimize)
	ON_BN_CLICKED(IDC_BTN_SERVER_INFO, OnBtnServerInfo)
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_CREDIT, OnBtnCredit)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainDialog message handlers


void CMainDialog::OnCancel() 
{
   return;
}

void CMainDialog::OnOK() 
{
	return;
}

BOOL CMainDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

   m_strDirectLogin.Format("%s",m_strNMAccountName);
   m_strDirectIP   .Format("%s",m_strNMAccountIP);

   SetWindowPos(NULL,0,0,640,480,NULL);
   CenterWindow();
	
   LoadSetSkin();

   m_btnIntro.SetFontName("T4C Beaulieux");
   m_btnIntro.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnIntro.SetFontSize(12);
   m_btnIntro.SetFontBold(TRUE);
   m_btnIntro.SetForeColor(RGB(0,0,0));
   //m_btnIntro.SetText(m_Language[151]);
  
   m_btnServer.SetFontName("T4C Beaulieux");
   m_btnServer.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnServer.SetFontSize(12);
   m_btnServer.SetFontBold(TRUE);
   m_btnServer.SetForeColor(RGB(0,0,0));
   //m_btnServer.SetText(m_Language[137]);

   m_btnCredit.SetFontName("T4C Beaulieux"); 
   m_btnCredit.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnCredit.SetFontSize(12);
   m_btnCredit.SetFontBold(TRUE);
   m_btnCredit.SetForeColor(RGB(0,0,0));
   //m_btnCredit.SetText(m_Language[139]);
   

   m_btnConnect.SetFontName("T4C Beaulieux");
   m_btnConnect.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnConnect.SetFontSize(12);
   m_btnConnect.SetFontBold(TRUE);
   m_btnConnect.SetForeColor(RGB(0,0,0));
   //m_btnConnect.SetText(m_Language[152]);

   m_btnQuitter.SetFontName("T4C Beaulieux");
   m_btnQuitter.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnQuitter.SetFontSize(16);
   m_btnQuitter.SetFontBold(TRUE);
   m_btnQuitter.SetForeColor(RGB(200,200,200));

   m_btnMinimize.SetFontName("T4C Beaulieux");
   m_btnMinimize.SetFont3D(TRUE,CSkinButton::Type3D::Sunken);
   m_btnMinimize.SetFontSize(16);
   m_btnMinimize.SetFontBold(TRUE);
   m_btnMinimize.SetForeColor(RGB(200,200,200));


   SetWindowText(m_strTitle);

   m_staticTitle.SetFontBold(TRUE);
   m_staticTitle.SetFontName("Arial");
   m_staticTitle.SetFontSize(10);
   m_staticTitle.SetForeColor(RGB(222, 158,  0));
   m_staticTitle.SetText(m_strTitle);
   m_staticTitle.SetFont3D(TRUE,uicLabel::Type3D::Sunken);

   m_staticLinkWeb1.SetFontBold(TRUE); // steph ajout
   m_staticLinkWeb1.SetFontName("Arial"); // steph ajout
   m_staticLinkWeb1.SetFontSize(10); // steph ajout
   m_staticLinkWeb1.SetForeColor(RGB(200,200,200)); // steph ajout
   m_staticLinkWeb1.SetFont3D(TRUE,uicLabel::Type3D::Sunken); // steph ajout
   m_staticLinkWeb1.SetCenter(TRUE); // steph ajout
   m_staticLinkWeb1.SetLinkEffect(TRUE,IDC_LINK_CURSOR,TRUE); // steph ajout
   m_staticLinkWeb1.SetText("Shop"); // steph ajout
   m_staticLinkWeb1.SetTextLink("https://t4c-rebirth.com/shop.php"); // steph ajout

   m_staticLinkWeb2.SetFontBold(TRUE); // steph ajout
   m_staticLinkWeb2.SetFontName("Arial"); // steph ajout
   m_staticLinkWeb2.SetFontSize(10); // steph ajout
   m_staticLinkWeb2.SetForeColor(RGB(200,200,200)); // steph ajout
   m_staticLinkWeb2.SetFont3D(TRUE,uicLabel::Type3D::Sunken); // steph ajout
   m_staticLinkWeb2.SetCenter(TRUE); // steph ajout
   m_staticLinkWeb2.SetLinkEffect(TRUE,IDC_LINK_CURSOR,TRUE); // steph ajout
   m_staticLinkWeb2.SetText("RPG Paradize"); // steph ajout
   m_staticLinkWeb2.SetTextLink("http://www.rpg-paradize.com/?page=vote&vote=107434"); // steph ajout

   m_staticConnectGeneral.SetFontBold(TRUE);
   m_staticConnectGeneral.SetFontName("Arial");
   m_staticConnectGeneral.SetFontSize(10);
   m_staticConnectGeneral.SetForeColor(RGB(200,200,200));
   m_staticConnectGeneral.SetFont3D(TRUE,uicLabel::Type3D::Sunken);
   m_staticConnectGeneral.SetCenter(TRUE);

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

   m_staticPassword.SetFontBold(TRUE);
   m_staticPassword.SetFontName("Arial");
   m_staticPassword.SetFontSize(10);
   m_staticPassword.SetForeColor(RGB(200,200,200));
   m_staticPassword.SetFont3D(TRUE,uicLabel::Type3D::Sunken);
   m_staticPassword.SetCenter(2);
   

   CRect rc;
   m_TxtCB.CreateFont(16, 8, 0, 0, FW_BOLD, FALSE, FALSE,0,0,0,0,0,0, "Arial");
   GetDlgItem(IDC_CB_SERVER)->GetWindowRect(&rc);
   ScreenToClient(rc);
   m_pDropServer.Create("", rc, this, BTN_ID_DROP_SERVER_BTN,
						    RGB(0,0,0), RGB(0,0,0),
						    RGB(200,200,200), RGB(222, 158,  0), &m_TxtCB);
   m_pDropServer         .SetWindowPos(NULL,170,222,245,34 ,NULL);

   m_pDropServer.GetWindowRect(&rc); 
   ScreenToClient(&rc);
   m_pDropServer.bSetBGImageTransRes(FALSE,RGB(0,0,0),IDB_BACK_MAIN_DLG,rc.left,rc.top);
	// Set OPTIONAL parameters
	m_pDropServer.SetListHeight(rc.Height()*3);		// Set the Width Height
	m_pDropServer.SetListWidth(rc.Width());	      // Set the List Width
	m_pDropServer.ChangeTitleWithSelection(true);   // When the users selects an item change the title in the top


   GetDlgItem(IDC_BMP_BACK_INTRO)->SetWindowPos(NULL,161,143,450,306,NULL);
   GetDlgItem(IDC_BMP_BACK_IP   )->SetWindowPos(NULL,164,148,445,60,NULL);
   
   GetDlgItem(IDC_AVI_INTRO_1)   ->SetWindowPos(NULL,161+4,143+4,450-8-4,306-8-2,NULL);
   GetDlgItem(IDC_CREDIT)        ->SetWindowPos(NULL,161,143,450,306,NULL);
   m_staticConnectGeneral.SetWindowPos(NULL,171,278,436,22,NULL);
   m_ListInfo            .SetWindowPos(NULL,171,305,436,140,NULL);

   m_staticConnectGeneral  .ShowWindow(SW_HIDE);
   m_ListInfo              .ShowWindow(SW_HIDE);
   m_pDropServer           .ShowWindow(SW_HIDE);
   m_btnConnect            .ShowWindow(SW_HIDE);
   GetDlgItem(IDC_CREDIT) ->ShowWindow(SW_HIDE);

  
   m_ListInfo.SetExtendedStyle(LVS_EX_FLATSB);
   m_ListInfo.GetWindowRect(&rc);
   ScreenToClient(&rc);
   m_ListInfo.bSetBGImageTransRes(FALSE,RGB(0,0,0),IDB_BACK_MAIN_DLG,rc.left,rc.top);
   m_ListInfo.vSetRowHeight(16);
   m_ListInfo.SetFontSize(9);
   m_ListInfo.SetFontName("System");//BLBLBL je change pour une fixed 
   m_ListInfo.SetFontBold(TRUE);
   m_ListInfo.SettextColor(RGB(200,200,200),RGB(115,38,40));
   m_ListInfo.InsertColumn(0,"Name",LVCFMT_LEFT,rc.Width()-20);


   m_ctrlIP      .SetBackColor(RGB(0,0,0));	
   m_ctrlIP      .SetTextColor(RGB(200,200,200));
   m_ctrlLogin   .SetBackColor(RGB(0,0,0));	
   m_ctrlLogin   .SetTextColor(RGB(200,200,200));
   m_ctrlPassword.SetBackColor(RGB(0,0,0));	
   m_ctrlPassword.SetTextColor(RGB(200,200,200));




   
   // Read Connection Server...
   ReadConfig();

   m_Language.Release();
   m_Language.Create(m_sLauncherConfig.dwLanguage);
   

   m_bMusicInitialised = FALSE;
   m_bCanPlayMusic     = TRUE;
   m_strAmbiantSound1  .Format("%s\\Game Files\\T4CLauncher.bin",m_strPathGame);
   m_strAmbiantSound2  .Format("%s\\Game Files\\T4CLauncher2.bin",m_strPathGame);
   m_strAmbiantEffect1 .Format("%s\\Game Files\\Effect01.bin",m_strPathGame);
   m_strIntroDls.Format("%s\\Game Files\\T4CLIDls.Bin",m_strPathGame);
   LoadAndPlayMusic();
   

   m_btnIntro.SetText(m_Language[151]);
   m_btnServer.SetText(m_Language[137]);
   m_btnCredit.SetText(m_Language[139]);
   m_btnConnect.SetText(m_Language[152]);

   m_staticLogin   .SetText(m_Language[147]);
   m_staticPassword.SetText(m_Language[148]);

  //200,185,120
   //200,200,200

   CString s;
   s =  "<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br>";
   s += "<img src='#%d'>";
   s += "<br><br>";
   s += "<font size='22' color='200,200,200' face='Arial'>";
   s += m_strVersion;
   s += "</font><br><br><br>";
   s += "<font color='200,200,200' size='20' face='Arial' style='b' style='u'>About</font><br><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>(T4C)The 4th Coming <br>Copyright © Dialsoft </font>";
   //s += "<br><img src='#%d'><br><br>";
   //s += "<font color='200,200,200' size='14' face='Arial'>(T4C)The 4th Coming</font><font color='230,50,10' size='16' style='b'> V2</font><font color='200,200,200' size='14'><br>Created with original T4C Sources and Copyright © Dialsoft </font>";
   s += "<br><br><img src='#%d'><br><br><br>";
      // Credit Zone...

   s += "<font color='200,200,200' size='20' face='Arial' style='b' style='u'>==T4C Rebirth Project Team==</font><br><br>"; // steph
	// -- 1.6 Coordination Unit-- // steph
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.68V2 Coordination Unit--</font><br>"; // steph
   s += "<font color='200,200,200' size='14' face='Arial'>"; // steph
   s += "Henry, Project Leader<br>"; // steph
   s += "Shainon, Steph, Coders Leader<br>"; // steph ajout de Steph
   s += "<br></font><br><br>"; // steph
	// -- 1.6 Coding Unit-- // steph
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.68V2 Coding Unit--</font><br>"; // steph
   s += "<font color='200,200,200' size='14' face='Arial'>"; // steph
   s += "Shainon, Steph, Core, Interface && Security Coders Leader<br>"; // steph ajout de Steph
   s += "<br></font><br><br>"; // steph


   s += "<font color='200,200,200' size='20' face='Arial' style='b' style='u'>==T4C V2.0 Project Team==</font><br><br>";

   // -- 1.6 Coordination Unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.6 Coordination Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Marc Frega, Dialsoft Foundator && CEO<br>";
   s += "Loïc \"Black Lemming\" Jean-Fulcrand, Project Leader<br>";
   s += "Carl \"Nightmare\" Vachon, Core, Interface && Security Coders Leader<br>";
   s += "Kitten, Writing Unit Leader<br>";
   s += "<br></font><br><br>";

   // -- 1.6 Coding Unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.6 Coding Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Carl \"Nightmare\" Vachon, Core, Interface && Security Coders Leader<br>";
   s += "Tiamat, Network && Internal Tools Coder<br>";
   s += "Sébastien \"The Destiny\" Mériot, Editor's Coder<br>";
   s += "<br></font><br><br>";

   // --Graphics Unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.6 Graphics Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Heschal<br>";
   s += "Loïc \"Black Lemming\" Jean-Fulcrand<br>";
   s += "Nadine \"Lune\" Pelletier<br>";
   s += "<br></font><br><br>";

   // --World Design Unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.6 World Design Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Nadine \"Lune\" Pelletier<br>";
   s += "<br></font><br><br>";
 
   // --Sounds & Musics--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.6 Sounds & Musics--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Loïc \"Black Lemming\" Jean-Fulcrand<br>";
   s += "<br></font><br><br>";

   // -- 1.6 writing unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.6 Writers Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Kitten<br>";
   s += "Mouse<br>";
   s += "Kujo<br>";
   s += "Scarlett<br>";
   s += "Redskull<br>";
   s += "Kenko<br>";
   s += "<br></font><br><br>";





    // -- 1.5 coordination unit --
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.5 Coordination Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Marc Frega, Dialsoft Foundator && CEO<br>";
   s += "Loïc \"Black Lemming\" Jean-Fulcrand, Project Leader<br>";
   s += "Carlos \"FPGA\" Lima, Coders Leader<br>";
   s += "Romain \"Leo\" Wagner, World Design Unit Leader<br>";
   s += "Kitten, Writing Unit Leader<br>";
   s += "<br></font><br><br>";

   // -- 1.5 Coding Unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.5 Coding Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Carlos \"FPGA\" Lima<br>";
   s += "Ophise<br>";
   s += "Tiamat<br>";
   s += "Scotch<br>";
   s += "Sébastien \"The Destiny\" Mériot<br>";
   s += "Desboys<br>";
   s += "Nara<br>";
   s += "Tyrion<br>";
   s += "<br></font><br><br>";

   // --1.5 Graphics Unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.5 Graphics Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Heschal<br>";
   s += "Loïc \"Black Lemming\" Jean-Fulcrand<br>";
   s += "<br></font><br><br>";

   // -- 1.5 world design--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.5 World Design Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Romain \"Leo\" Wagner<br>";
   s += "Anadroow<br>";
   s += "Clariss<br>";
   s += "<br></font><br><br>";

   // -- 1.5 writing unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.5 Writers Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Kitten<br>";
   s += "Blade<br>";
   s += "Ostian<br>";
   s += "Reorx<br>";
   s += "Redskull<br>";
   s += "<br></font><br><br>";





    // -- 1.4 coordination unit --
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.4 Coordination Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Marc Frega, Dialsoft Foundator && CEO<br>";
   s += "Loïc \"Black Lemming\" Jean-Fulcrand, Project Leader<br>";
   s += "Carlos \"FPGA\" Lima, Coders Leader<br>";
   s += "Romain \"Leo\" Wagner, World Design Unit Leader<br>";
   s += "Blade, Writing Leader<br>";
   s += "<br></font><br><br>";

   // -- 1.4 Coding Unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.4 Coding Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Carlos \"FPGA\" Lima<br>";
   s += "Ophise<br>";
   s += "Tiamat<br>";
   s += "Scotch<br>";
   s += "<br></font><br><br>";

   // --1.4 Graphics Unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.4 Graphics Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Loïc \"Black Lemming\" Jean-Fulcrand<br>";
   s += "<br></font><br><br>";

   // -- 1.4 world design--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.4 World Design Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Romain \"Leo\" Wagner<br>";
   s += "Anadroow<br>";
   s += "<br></font><br><br>";

   // -- 1.4 writing unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.4 Writers Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Blade<br>";
   s += "Ostian<br>";
   s += "Reorx<br>";
   s += "<br></font><br><br>";




    // -- 1.3 coordination unit --
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.3 Coordination Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Marc Frega, Dialsoft Foundator && CEO<br>";
   s += "Carlos \"FPGA\" Lima, Coders Leader<br>";
   s += "Kilivan, Project Leader<br>";
   s += "Romain \"Leo\" Wagner, World Design Unit Leader<br>";
   s += "Blade, Writing Leader<br>";
   s += "<br></font><br><br>";

   // -- 1.3 Coding Unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.3 Coding Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Carlos \"FPGA\" Lima<br>";
   s += "Scotch<br>";
   s += "<br></font><br><br>";

   // --1.3 Graphics Unit--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.3 Graphics Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Loïc \"Black Lemming\" Jean-Fulcrand<br>";
   s += "<br></font><br><br>";

   // -- 1.3 world design--
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >--1.3 World Design Unit--</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Romain \"Leo\" Wagner<br>";
   s += "<br></font><br><br>";


   // ==Special Thanks To==
   s += "<font color='200,200,200' size='16' face='Arial' style='b' >==Special Thanks To==</font><br>";
   s += "<font color='200,200,200' size='14' face='Arial'>";
   s += "Thanks to all the Sysops, HGM and GM who shared their<br>";
   s += "time to help us hunting the bugs !<br><br>";
   s += "Thanks to all players, thanks for your patience,<br>";
   s += "suggestions and support ! Thanks to Marc, who made this<br>";
   s += "project possible.<br><br>";
   s += "Thanks to all people we forgot and who helped us<br>";
   s += " at one time or an other !<br>";
   s += "Thanks to Darkfang for giving gold to newbies..<br>";
   s += "<br></font><br><br>";
   s += "<br><br>";
   s += "<img src='#%d'><br><br><br><br><br><br><br><br>";




   m_BGBitmapC.LoadBitmap(IDB_BMP_BACK_INTRO);
   m_dcBitmap.CreateCompatibleDC(NULL);
 
	// Content Data
	m_ctrlCredit.FormatDataString(s,IDB_CREDIT_INTRO,IDB_CREDIT_DIALSOFT,IDB_CREDIT_V2);
	
	// use our own function for background drawing
	m_ctrlCredit.m_pBackgroundPaint = CMainDialog::DrawCreditsBackground;
	m_ctrlCredit.m_dwBackgroundPaintLParam = TRUE;
   m_ctrlCredit.m_pBitmapBack = &m_BGBitmapC;

	// as our background is going to be black, use black as default transparent color
	m_ctrlCredit.m_crInternalTransparentColor = RGB(0,0,0);
	
	// now, we're ready to begin... create the window
	m_ctrlCredit.Create(NULL,WS_VISIBLE|WS_CHILD,IDC_CREDIT,this,0,IDC_POINTER,0);
   m_ctrlCredit.ShowWindow(SW_HIDE);

   SetTimer(TIMER_CHECK_CONNECT_STAT,250,0);
   

   HWND hwndPreview = NULL;
   GetDlgItem( IDC_AVI_INTRO_1, &hwndPreview );
   m_pNMShowAVI = new CNMDShow(hwndPreview);

   /*

   GetDlgItem(IDC_AVI_INTRO_1)->ShowWindow(SW_HIDE);
   SetTimer(TIMER_INTRO_FIRST_PLAY,300,0);
   m_staticConnectGeneral.ShowWindow(SW_HIDE);
   m_ListInfo            .ShowWindow(SW_HIDE);
   m_pDropServer         .ShowWindow(SW_HIDE);
   m_btnConnect          .ShowWindow(SW_HIDE);
   GetDlgItem(IDC_BMP_BACK_INTRO)->ShowWindow(SW_SHOW);
   */

   m_bIntroStatus = FALSE;
   m_pNMShowAVI->CloseInterfaces();
   KillTimer(TIMER_INTRO_DLS);
   GetDlgItem(IDC_AVI_INTRO_1)->ShowWindow(SW_HIDE);
   GetDlgItem(IDC_BMP_BACK_INTRO)->ShowWindow(SW_HIDE);
   GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_HIDE);
   m_staticConnectGeneral.ShowWindow(SW_SHOW);
   m_ListInfo            .ShowWindow(SW_SHOW);
   m_pDropServer         .ShowWindow(SW_SHOW);
   m_btnConnect          .ShowWindow(SW_SHOW);
   m_btnIntro.SetText("WWW"/*m_Language[138]*/);
   GetDlgItem(IDC_BTN_SERVER_INFO)->EnableWindow(TRUE);
   GetDlgItem(IDC_BTN_CREDIT     )->EnableWindow(TRUE);
   ChangeConnectStat(TRUE);

   UpdateServerList();


   UpdateData(FALSE);
	m_bInit = TRUE;
	return TRUE;
}


void CMainDialog::freeDialog()
{
    if( m_BGBitmapC.m_hObject != NULL )
		m_BGBitmapC.DeleteObject();

    WriteConfig();
    StopSonAmbiant(0);
    StopSonAmbiant(1);
    FreeBASS();
    if(m_pNMShowAVI)
    {
      delete m_pNMShowAVI ;
    }
    m_pNMShowAVI = NULL;

    m_sLauncherConfig.pConnection.RemoveAll();
    m_Language.Release();

    m_strNMAccountName.Format("%s",m_strDirectLogin);
    m_strNMAccountIP   .Format("%s",m_strDirectIP);
}

void CMainDialog::LoadAndPlayMusic()
{
   if(m_bMusicInitialised && m_sLauncherConfig.bLauncherMusic)
   {
      BASS_SetVolume[0](40);
      PlaySonAmbiant(0,m_strAmbiantSound1.GetBuffer(0));
   }
   else if(!m_bMusicInitialised && m_sLauncherConfig.bLauncherMusic)
   {
      LoadBASS(0);
      if(m_bCanPlayMusic)
      {
         BASS_Init[0](0,44100,0,this->m_hWnd);
         BASS_Start[0]();
         m_dwOLDVolume = BASS_GetVolume[0]();
         BASS_SetVolume[0](40);
      }
      m_bMusicInitialised = TRUE;

      PlaySonAmbiant(0,m_strAmbiantSound1.GetBuffer(0));
   }
   else if(m_bMusicInitialised && !m_sLauncherConfig.bLauncherMusic)
   {
      // on doi deleter les objets de musique...
      StopSonAmbiant(0);
      StopSonAmbiant(1);
      FreeBASS();
      m_bMusicInitialised = FALSE;
   }
}

void CMainDialog::CheckIfNeedToStartStopSound()
{
   if(m_bMusicInitialised && !m_sLauncherConfig.bLauncherMusic)
   {
      // on doi deleter les objets de musique...
      StopSonAmbiant(0);
      StopSonAmbiant(1);
      FreeBASS();
      m_bMusicInitialised = FALSE;
   }
   else if(!m_bMusicInitialised && m_sLauncherConfig.bLauncherMusic)
   {
      LoadBASS(0);
      if(m_bCanPlayMusic)
      {
         BASS_Init[0](0,44100,0,this->m_hWnd);
         BASS_Start[0]();
         m_dwOLDVolume = BASS_GetVolume[0]();
         BASS_SetVolume[0](40);
      }
      m_bMusicInitialised = TRUE;

      if(m_bMessageServer)
      {
         BASS_SetVolume[0](20);
         PlaySonAmbiant(0,m_strAmbiantSound2.GetBuffer(0));
      }
      else
      {
         PlaySonAmbiant(0,m_strAmbiantSound1.GetBuffer(0));
      }
   }
}


BOOL CMainDialog::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(rc);
	if (TileBitmap(pDC, rc) == TRUE)
	{
		return TRUE;
	}
	return CDialog::OnEraseBkgnd(pDC);
}

void CMainDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}


UINT CMainDialog::GetLastAction()
{
   UINT dwAction = m_dwAction;
   m_dwAction = LAST_CONNECT_ACTION_IDLE;
   return dwAction ;
}

void CMainDialog::AddConnectMessage(char *pstrMsg,int dwLineIndex,BOOL bChangeConnect)
{
   if(dwLineIndex == 0)
   {
      m_ListInfo.DeleteAllItems();
      
      if(bChangeConnect)
         m_btnConnect.SetText(m_Language[172]);
      else
         m_btnConnect.SetText(m_Language[152]);
   }

   m_ListInfo.InsertItem(dwLineIndex,pstrMsg,0);

}

void CMainDialog::AddGeneralMessage(char *pstrMsg)
{
   CString strTmp; 
   strTmp = m_staticConnectGeneral.GetText();
   if(strcmp(strTmp.GetBuffer(0),pstrMsg))
      m_staticConnectGeneral.SetText(pstrMsg);
}


void CMainDialog::ForceConnectState(BOOL bState)
{
   ChangeConnectStat(bState);
}

void CMainDialog::ProcessMessage() 
{
   CheckIfNeedToStartStopSound();
   if(m_bMessageServer/* && m_pServerDlg && m_pServerDlg->IsInit() == TRUE*/)
   {
      m_pServerDlg ->ProcessMessage();
   }
   
   int toto = 5;
   MSG msg;
   while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
   {
      if(!IsDialogMessage(&msg))
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);  
      }
      else
      {
         if (msg.message == WM_KEYDOWN)
         {
            if (msg.wParam == VK_RETURN)
            {
               OnBtnConnect();
            }
            else if(msg.wParam == VK_ESCAPE)
            {
               OnBtnExit();
            }
            else if(msg.wParam ==VK_UP)
            {
               CDropList* pList = m_pDropServer.GetList();
               
               m_sLauncherConfig.dwSelectedServer--;
               if(m_sLauncherConfig.dwSelectedServer < 0)
                  m_sLauncherConfig.dwSelectedServer = 0;
               m_pDropServer.SetSelection(m_sLauncherConfig.dwSelectedServer);
               
               if(m_sLauncherConfig.dwSelectedServer == 0)
               {
                  // show DirectConnect Parameters
                  GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_SHOW);
                  GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_SHOW);
                  GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_SHOW);
                  GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_SHOW);
                  GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_SHOW);
                  GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_SHOW);
                  GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_SHOW);
                  ::SetFocus(GetDlgItem(IDCM_STR_IP)->m_hWnd);
               }
               else
               {
                  // hide DirectConnect Parameters
                  GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_HIDE);
                  GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_HIDE);
                  GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_HIDE);
                  GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_HIDE);
                  GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_HIDE);
                  GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_HIDE);
                  GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_HIDE);
               }
            }
            else if(msg.wParam == VK_DOWN)
            {
               CDropList* pList = m_pDropServer.GetList();
               
               m_sLauncherConfig.dwSelectedServer++;
               if(m_sLauncherConfig.dwSelectedServer >= pList->GetCount())
                  m_sLauncherConfig.dwSelectedServer = pList->GetCount()-1;
               m_pDropServer.SetSelection(m_sLauncherConfig.dwSelectedServer);
               
               if(m_sLauncherConfig.dwSelectedServer == 0)
               {
                  // show DirectConnect Parameters
                  GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_SHOW);
                  GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_SHOW);
                  GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_SHOW);
                  GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_SHOW);
                  GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_SHOW);
                  GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_SHOW);
                  GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_SHOW);
                  ::SetFocus(GetDlgItem(IDCM_STR_IP)->m_hWnd);
               }
               else
               {
                  // hide DirectConnect Parameters
                  GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_HIDE);
                  GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_HIDE);
                  GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_HIDE);
                  GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_HIDE);
                  GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_HIDE);
                  GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_HIDE);
                  GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_HIDE);
               }
            }
         }
      }
   }
   
}

void CMainDialog::SetTitle(char *pstrTitle)
{
   m_strTitle.Format("%s",pstrTitle);
}

void CMainDialog::SetVersion(char *pstrVersion)
{
   m_strVersion.Format("%s",pstrVersion);
}

void CMainDialog::SetPathGame(char *pstrPath)
{
   m_strPathGame.Format("%s",pstrPath);
}

void CMainDialog::SetPathSave(char *pstrPath)
{
   m_strPathSave.Format("%s",pstrPath);
}

void CMainDialog::SetAccountLogin(char *pstr)
{
   m_strNMAccountName.Format("%s",pstr);
}

void CMainDialog::SetAccountIP(char *pstr)
{
   m_strNMAccountIP.Format("%s",pstr);
}

///////////////////////////////////////////////////////////////////////////////////////////
// SKIN
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CMainDialog::LoadSetSkin()
{
	DeleteSkinBmp();
	// Set les bitmap du skin Interne
   m_bmpBackground  .LoadBitmap(IDB_BACK_MAIN_DLG);
   m_bmpBtnFrame2   .LoadBitmap(IDB_BTN_FRAME2);
   m_bmpBtnFrame3   .LoadBitmap(IDB_BTN_FRAME3);
   

   //m_btnLienWeb.SetBitmap(IDB_BTN_WEB);
   //m_btnLienWeb.SetDefaultCursor();
	
	m_btnIntro		   .SetBitmap(&m_bmpBtnFrame3);
   m_btnServer		   .SetBitmap(&m_bmpBtnFrame3);
   m_btnCredit		   .SetBitmap(&m_bmpBtnFrame3);
   m_btnConnect      .SetBitmap(&m_bmpBtnFrame3);
   m_btnQuitter      .SetBitmap(&m_bmpBtnFrame2);
   m_btnMinimize     .SetBitmap(&m_bmpBtnFrame2);


	Invalidate();
	return TRUE;
}

BOOL CMainDialog::DeleteSkinBmp()
{
	m_bmpBackground    .DeleteObject();
   m_bmpBtnFrame2     .DeleteObject();
   m_bmpBtnFrame3     .DeleteObject();
	return TRUE;
}

BOOL CMainDialog::TileBitmap(CDC* _pDC, CRect _rc)
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

void CMainDialog::OnBtnMinimize() 
{
	ShowWindow(SW_MINIMIZE);
}

void CMainDialog::OnBtnExit() 
{
	m_dwAction = LAST_CONNECT_ACTION_EXIT;
}

void CMainDialog::OnBtnIntro() 
{
	ShellExecute(0,0,"https://t4c-rebirth.com/",0,0,1); // steph
   if(m_bIntroStatus)
   {
      /*m_bIntroStatus = FALSE;
      m_pNMShowAVI->CloseInterfaces();
      KillTimer(TIMER_INTRO_DLS);
      GetDlgItem(IDC_AVI_INTRO_1)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_BMP_BACK_INTRO)->ShowWindow(SW_HIDE);
      if(m_sLauncherConfig.dwSelectedServer == 0)
      {
         GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_SHOW);
         GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_SHOW);
         GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_SHOW);
      }
      m_staticConnectGeneral.ShowWindow(SW_SHOW);
      m_ListInfo            .ShowWindow(SW_SHOW);
      m_pDropServer         .ShowWindow(SW_SHOW);
      m_btnConnect          .ShowWindow(SW_SHOW);
      m_btnIntro.SetText(m_Language[138]);
      GetDlgItem(IDC_BTN_SERVER_INFO)->EnableWindow(TRUE);
      GetDlgItem(IDC_BTN_CREDIT     )->EnableWindow(TRUE);
      ChangeConnectStat(TRUE);*/
   }
   else
   {
      /*SetTimer(TIMER_INTRO_FIRST_PLAY,300,0);
      m_staticConnectGeneral.ShowWindow(SW_HIDE);
      m_ListInfo            .ShowWindow(SW_HIDE);
      m_pDropServer         .ShowWindow(SW_HIDE);
      m_btnConnect          .ShowWindow(SW_HIDE);
      GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_HIDE);
      GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_HIDE);
      GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_HIDE);
	  GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_BMP_BACK_INTRO)->ShowWindow(SW_SHOW);*/
   }
   
}


void CMainDialog::OnBtnCredit() 
{
	if(m_bCreditStatus)
   {
      m_bCreditStatus = FALSE;
      m_ctrlCredit.ShowWindow(SW_HIDE);
      GetDlgItem(IDC_BMP_BACK_INTRO)->ShowWindow(SW_HIDE);
      m_staticConnectGeneral.ShowWindow(SW_SHOW);
      m_ListInfo            .ShowWindow(SW_SHOW);
      m_pDropServer         .ShowWindow(SW_SHOW);
      m_btnConnect          .ShowWindow(SW_SHOW);
      if(m_sLauncherConfig.dwSelectedServer == 0)
      {
         GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_SHOW);
         GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_SHOW);
         GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_SHOW);
		 GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_SHOW);
      }
      m_btnCredit.SetText(m_Language[139]);
      GetDlgItem(IDC_BTN_SERVER_INFO)->EnableWindow(TRUE);
      GetDlgItem(IDC_BTN_INTRO      )->EnableWindow(TRUE);
      ChangeConnectStat(TRUE);
   }
   else
   {
      SetTimer(TIMER_CREDIT_FIRST_PLAY,250,0);
   }
}


void CMainDialog::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
   {
      case TIMER_INTRO_FIRST_PLAY:
      {
         KillTimer(TIMER_INTRO_FIRST_PLAY);
         m_bIntroStatus = TRUE;
         m_btnIntro.SetText("WWW"/*m_Language[151]*/);
         //m_staticConnectGeneral.ShowWindow(SW_HIDE);
         //m_ListInfo            .ShowWindow(SW_HIDE);
         //m_pDropServer         .ShowWindow(SW_HIDE);
         //m_btnConnect          .ShowWindow(SW_HIDE);
         m_pNMShowAVI->PlayVideoFile(m_strIntroDls.GetBuffer(0));
         //GetDlgItem(IDC_BMP_BACK_INTRO)->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_AVI_INTRO_1)->ShowWindow(SW_SHOW);
         GetDlgItem(IDC_AVI_INTRO_1)->RedrawWindow();
         GetDlgItem(IDC_BTN_SERVER_INFO)->EnableWindow(FALSE);
         GetDlgItem(IDC_BTN_CREDIT     )->EnableWindow(FALSE);
         ChangeConnectStat(FALSE);
         SetTimer(TIMER_INTRO_DLS,250,0);
         m_bFirstredraw = TRUE;
      }
      break;
      case TIMER_INTRO_DLS :
         if(m_pNMShowAVI && m_pNMShowAVI->CheckCompleteEvent())
         {
            m_bIntroStatus = FALSE;
            m_pNMShowAVI->CloseInterfaces();
            KillTimer(TIMER_INTRO_DLS);
            GetDlgItem(IDC_AVI_INTRO_1)->ShowWindow(SW_HIDE);
            GetDlgItem(IDC_BMP_BACK_INTRO)->ShowWindow(SW_HIDE);
            m_staticConnectGeneral.ShowWindow(SW_SHOW);
            m_ListInfo            .ShowWindow(SW_SHOW);
            m_pDropServer         .ShowWindow(SW_SHOW);
            m_btnConnect          .ShowWindow(SW_SHOW);
            m_btnIntro.SetText(m_Language[138]);
            GetDlgItem(IDC_BTN_SERVER_INFO)->EnableWindow(TRUE);
            GetDlgItem(IDC_BTN_CREDIT     )->EnableWindow(TRUE);
            ChangeConnectStat(TRUE);
         }
         else if(m_bFirstredraw)
         {
            GetDlgItem(IDC_AVI_INTRO_1)->RedrawWindow();
            m_bFirstredraw = FALSE;
         }
      break;
      case TIMER_CREDIT_FIRST_PLAY:
      {

         KillTimer(TIMER_CREDIT_FIRST_PLAY);
         m_bCreditStatus = TRUE;
         m_btnCredit.SetText(m_Language[151]);
         m_staticConnectGeneral.ShowWindow(SW_HIDE);
         m_ListInfo            .ShowWindow(SW_HIDE);
         m_pDropServer         .ShowWindow(SW_HIDE);
         m_btnConnect          .ShowWindow(SW_HIDE);
         GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_HIDE);
         GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_HIDE);
         GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_HIDE);
		 GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_HIDE);
         GetDlgItem(IDC_BMP_BACK_INTRO)->ShowWindow(SW_SHOW);
         m_ctrlCredit.ShowWindow(SW_SHOW);
         GetDlgItem(IDC_BTN_SERVER_INFO)->EnableWindow(FALSE);
         GetDlgItem(IDC_BTN_INTRO      )->EnableWindow(FALSE);
         ChangeConnectStat(FALSE);
      }
      break;
      case TIMER_SHOW_SERVER_DIALOG:
         if(m_pServerDlg && !m_pServerDlg->IsInit())
         {
            KillTimer(TIMER_SHOW_SERVER_DIALOG);
            m_bMessageServer = FALSE;
            SetTimer(TIMER_SHOW_SERVER_DIALOG_KILL,200,0);
         }
         else if(!m_pServerDlg)
            KillTimer(TIMER_SHOW_SERVER_DIALOG);
         else
         {
            if(m_pServerDlg)
            {
               if(m_pServerDlg->m_dwNewLanguage != -1)
               {
                  m_pServerDlg->m_dwNewLanguage = -1;
                  UpdateData(TRUE);
                  // un changement de langue...
                  m_Language.Release();
                  m_Language.Create(m_sLauncherConfig.dwLanguage);
                  // on dois reaffecter tous les textes du GUI du main dialog et avertir le client que la langue a changer...
                  m_dwAction = LAST_CHANGE_LANGUAGE;
                  UpdateServerList();

                  m_btnIntro.SetText(m_Language[151]);
                  m_btnServer.SetText(m_Language[137]);
                  m_btnCredit.SetText(m_Language[139]);
                  m_btnConnect.SetText(m_Language[152]);
                  UpdateData(FALSE);
               }
            }
         }
      break;
      case TIMER_SHOW_SERVER_DIALOG_KILL:
         KillTimer(TIMER_SHOW_SERVER_DIALOG_KILL);
         if(m_pServerDlg)
         {
            m_pServerDlg->DestroyWindow();
            delete m_pServerDlg;
            m_pServerDlg = NULL;

            LoadAndPlayMusic();

            m_sLauncherConfig.dwSelectedServer = -1;
            UpdateServerList();
         }

      break;
      case TIMER_CHECK_CONNECT_STAT:
      {
         CDropList* pList = m_pDropServer.GetList();
         int dwSelTmp = pList->GetCurSel();
         if(dwSelTmp != m_sLauncherConfig.dwSelectedServer)
         {
            m_dwAction = LAST_CONNECT_ACTION_RESET;
            m_sLauncherConfig.dwSelectedServer = dwSelTmp ;
            ChangeConnectStat(TRUE);
            if(m_sLauncherConfig.dwSelectedServer == 0)
            {
               // show DirectConnect Parameters
               GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_SHOW);
               GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_SHOW);
               GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_SHOW);
               GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_SHOW);
               GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_SHOW);
               GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_SHOW);
			      GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_SHOW);
               ::SetFocus(GetDlgItem(IDCM_STR_IP)->m_hWnd);
            }
            else
            {
               // hide DirectConnect Parameters
               GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_HIDE);
               GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_HIDE);
               GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_HIDE);
               GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_HIDE);
               GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_HIDE);
               GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_HIDE);
			      GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_HIDE);
            }
         }
      }
      break;
   }
	
	CDialog::OnTimer(nIDEvent);
}



void CMainDialog::LoadBASS(int n)
{
	char tempfile[MAX_PATH];

	if (n) 
   {
		char dllfile[MAX_PATH],temppath[MAX_PATH],*c;
		if (!SearchPath(NULL,"bass.dll",NULL,MAX_PATH,dllfile,&c)) 
      {
         m_bCanPlayMusic = FALSE;
			return;
		}

		// get a temporary filename and copy BASS.DLL there 
		GetTempPath(MAX_PATH,temppath);
		GetTempFileName(temppath,"bas",0,tempfile);
		CopyFile(dllfile,tempfile,0);
		strcpy(bassfile[n-1],tempfile);
	} 
   else
		strcpy(tempfile,"bass.dll");

	// load the BASS.DLL library 
	if (!(bass[n]=LoadLibrary(tempfile))) 
   {
		m_bCanPlayMusic = FALSE;
		return; 
   } 
	// load all the BASS functions that are to be used 
#define LOADBASSFUNCTION(f) ((void**)f)[n]=GetProcAddress(bass[n],#f)
	LOADBASSFUNCTION(BASS_GetVersion);
	LOADBASSFUNCTION(BASS_ErrorGetCode);
	LOADBASSFUNCTION(BASS_GetDeviceDescription);
	LOADBASSFUNCTION(BASS_Init);
	LOADBASSFUNCTION(BASS_Free);
	LOADBASSFUNCTION(BASS_Start);
	LOADBASSFUNCTION(BASS_StreamCreateFile);
	LOADBASSFUNCTION(BASS_StreamPlay);
	LOADBASSFUNCTION(BASS_StreamFree);
   LOADBASSFUNCTION(BASS_SetVolume);
   LOADBASSFUNCTION(BASS_GetVolume);

   m_bCanPlayMusic = TRUE;
}

// free the BASS library from memory and delete the temporary files 
void CMainDialog::FreeBASS()
{
   if(!m_bCanPlayMusic || !m_bMusicInitialised)
      return;
   
   BASS_SetVolume[0](m_dwOLDVolume);
   BASS_Free[0]();
	int c;
	for (c=0;c<devices;c++) {
		if (bass[c]) {
			FreeLibrary(bass[c]);
			bass[c]=0;
			if (c) DeleteFile(bassfile[c-1]);
		}
	}
   
}

void CMainDialog::PlaySonAmbiant(int dwIndex,char *pStrSoundName)
{
   
   if(!m_bCanPlayMusic || !m_sLauncherConfig.bLauncherMusic || !m_bMusicInitialised)
      return;

   // si le son joue deja ou que le volume est a 0, on joue pas....
   if(g_shChan[dwIndex])
      StopSonAmbiant(dwIndex);

   if (!(g_shChan[dwIndex] = BASS_StreamCreateFile[0](FALSE,pStrSoundName,0,0,0))) 
      return ;
   if(dwIndex == 0)
      BASS_StreamPlay[0](g_shChan[dwIndex],0,BASS_SAMPLE_LOOP);
   else
      BASS_StreamPlay[0](g_shChan[dwIndex],TRUE,0);

}

void CMainDialog::StopSonAmbiant(int dwIndex)
{
   if(!m_bCanPlayMusic || !m_bMusicInitialised)
      return ;

	if(g_shChan[dwIndex])
	   BASS_StreamFree[0](g_shChan[dwIndex]);
}


void CMainDialog::OnBtnServerInfo() 
{
   if(m_bCanPlayMusic && m_sLauncherConfig.bLauncherMusic && m_bMusicInitialised)
   {
      BASS_SetVolume[0](20);
      PlaySonAmbiant(0,m_strAmbiantSound2.GetBuffer(0));
      PlaySonAmbiant(1,m_strAmbiantEffect1.GetBuffer(0));
   }
   m_pServerDlg = new CServerDlg();
   CRect rc;
   GetWindowRect(&rc);
   m_pServerDlg->SetDlgPos(rc.left,rc.top);
   m_pServerDlg->SetServerList(&m_sLauncherConfig);
   m_pServerDlg->SetPathGame(m_strPathGame.GetBuffer(0));
   m_pServerDlg->SetPathSave(m_strPathSave.GetBuffer(0));

   m_pServerDlg->Create(IDD_SERVER_DLG,this);
   m_pServerDlg->ShowWindow(SW_SHOW);
   SetTimer(TIMER_SHOW_SERVER_DIALOG,250,NULL);
   m_bMessageServer = TRUE;
}

void CMainDialog::ReadConfig()
{
   CString strConfig;
   strConfig.Format("%s\\V2Launcher.dat",m_strPathSave);

   FILE *pf1 = fopen(strConfig.GetBuffer(0),"rb");
   if(!pf1)
   {
      // set default config...

      m_sLauncherConfig.dwNbrConnection  = 0;
      m_sLauncherConfig.bResolution      = FALSE;
      m_sLauncherConfig.bWindowed        = TRUE;
      m_sLauncherConfig.bLauncherMusic   = FALSE; // steph FALSE au lieu de TRUE
      m_sLauncherConfig.dwLanguage       = 0;
      m_sLauncherConfig.dwSelectedServer = -1;
      for(int i=0;i<10;i++)
         m_sLauncherConfig.bMacro[i] = TRUE;
      m_sLauncherConfig.chMacroKey[0] = 'I';//Inventory
      m_sLauncherConfig.chMacroKey[1] = 'S';//Charecter
      m_sLauncherConfig.chMacroKey[2] = 'L';//Chatter
      m_sLauncherConfig.chMacroKey[3] = 'G';//GroupPlay
      m_sLauncherConfig.chMacroKey[4] = 'P';//Spell
      m_sLauncherConfig.chMacroKey[5] = 'M';//Macro
      m_sLauncherConfig.chMacroKey[6] = 'T';//Trade
      m_sLauncherConfig.chMacroKey[7] = 'O';//Option
      m_sLauncherConfig.chMacroKey[8] = 'H';//Screenshot
      m_sLauncherConfig.chMacroKey[9] = 'A';//Switch Chat Size

      m_sLauncherConfig.dwNbrColor = 8;
      m_sLauncherConfig.CCColor.Add(RGB(17,181,205));
      m_sLauncherConfig.CCColor.Add(RGB(255,33,75));
      m_sLauncherConfig.CCColor.Add(RGB(90,119,216));
      m_sLauncherConfig.CCColor.Add(RGB(220,202,6));
      m_sLauncherConfig.CCColor.Add(RGB(5,197,37));
      m_sLauncherConfig.CCColor.Add(RGB(199,10,204));
      m_sLauncherConfig.CCColor.Add(RGB(192,192,192));
      m_sLauncherConfig.CCColor.Add(RGB(224,100,10));
   }
   else
   {
      ULONG dwTailleOri;
      ULONG dwTailleComp;
      BYTE *pFileDataTmp = NULL;
      BYTE *pFileDataTmpC = NULL;
      
      fread(&dwTailleOri,4,1,pf1);
      fread(&dwTailleComp,4,1,pf1);
      pFileDataTmp = new BYTE[dwTailleOri];
      pFileDataTmpC= new BYTE[dwTailleComp];
      fread(pFileDataTmpC,1,dwTailleComp,pf1);
      fclose(pf1);
      
      if(uncompress((BYTE*)pFileDataTmp,&dwTailleOri,(BYTE*)pFileDataTmpC,dwTailleComp) != Z_OK)
          MessageBox("Erreur UnCompress Config File","",MB_OK);

      unsigned char *pchBuff = pFileDataTmp;

      char chHeader[23];
      memcpy(&chHeader,pchBuff,22);   pchBuff+=22;
      chHeader[22] = 0x00;
      if(strcmp(chHeader,"NM_V2LAUNCHER_CFG_V004") != 0)
      {
         // set default config...
         m_sLauncherConfig.dwNbrConnection  = 0;
         m_sLauncherConfig.bResolution      = FALSE;
         m_sLauncherConfig.bWindowed        = TRUE;
         m_sLauncherConfig.bLauncherMusic   = TRUE;
         m_sLauncherConfig.dwLanguage       = 0;
         m_sLauncherConfig.dwSelectedServer = -1;
         for(int i=0;i<10;i++)
            m_sLauncherConfig.bMacro[i] = TRUE;
         m_sLauncherConfig.chMacroKey[0] = 'I';//Inventory
         m_sLauncherConfig.chMacroKey[1] = 'S';//Charecter
         m_sLauncherConfig.chMacroKey[2] = 'L';//Chatter
         m_sLauncherConfig.chMacroKey[3] = 'G';//GroupPlay
         m_sLauncherConfig.chMacroKey[4] = 'P';//Spell
         m_sLauncherConfig.chMacroKey[5] = 'M';//Macro
         m_sLauncherConfig.chMacroKey[6] = 'T';//Trade
         m_sLauncherConfig.chMacroKey[7] = 'O';//Option
         m_sLauncherConfig.chMacroKey[8] = 'H';//Screenshot
         m_sLauncherConfig.chMacroKey[9] = 'A';//Switch Chat Size
         
         m_sLauncherConfig.dwNbrColor = 8;
         m_sLauncherConfig.CCColor.Add(RGB(17,181,205));
         m_sLauncherConfig.CCColor.Add(RGB(255,33,75));
         m_sLauncherConfig.CCColor.Add(RGB(90,119,216));
         m_sLauncherConfig.CCColor.Add(RGB(220,202,6));
         m_sLauncherConfig.CCColor.Add(RGB(5,197,37));
         m_sLauncherConfig.CCColor.Add(RGB(199,10,204));
         m_sLauncherConfig.CCColor.Add(RGB(192,192,192));
         m_sLauncherConfig.CCColor.Add(RGB(224,100,10));
         delete []pFileDataTmp;
         delete []pFileDataTmpC;

         MessageBox("Invalid Launcher Configuration File...\nDefault Configuration Loaded !","Error !",MB_OK+MB_ICONSTOP);
         return;
      }
 

      memcpy(&m_sLauncherConfig.bResolution     ,pchBuff,4);   pchBuff+=4;
      memcpy(&m_sLauncherConfig.bWindowed       ,pchBuff,4);   pchBuff+=4;
      memcpy(&m_sLauncherConfig.bLauncherMusic  ,pchBuff,4);   pchBuff+=4;
      memcpy(&m_sLauncherConfig.dwLanguage      ,pchBuff,4);   pchBuff+=4;
      memcpy(&m_sLauncherConfig.dwSelectedServer,pchBuff,4);   pchBuff+=4;
      memcpy(&m_sLauncherConfig.bMacro          ,pchBuff,4*12); pchBuff+= (4*12);
      memcpy(&m_sLauncherConfig.chMacroKey      ,pchBuff,12  ); pchBuff+= 12;

      memcpy(&m_sLauncherConfig.dwNbrColor ,pchBuff,4); pchBuff+=4;
      if(m_sLauncherConfig.dwNbrColor >0)
      {
         for(int i=0;i<m_sLauncherConfig.dwNbrColor;i++)
         {
            int dwColor;
            memcpy(&dwColor,pchBuff,sizeof(int)); pchBuff+=sizeof(int);
            m_sLauncherConfig.CCColor.Add(dwColor);
         }
      }

      memcpy(&m_sLauncherConfig.dwNbrConnection ,pchBuff,4); pchBuff+=4;
      if(m_sLauncherConfig.dwNbrConnection >0)
      {
         for(int i=0;i<m_sLauncherConfig.dwNbrConnection;i++)
         {
            sConnectParam NewConnection;
            memcpy(&NewConnection ,pchBuff,sizeof(sConnectParam)); pchBuff+=sizeof(sConnectParam);
            m_sLauncherConfig.pConnection.Add(NewConnection);
         }
      }
      delete []pFileDataTmp;
      delete []pFileDataTmpC;
   }
}

void CMainDialog::WriteConfig()
{
   CString strConfig;
   strConfig.Format("%s\\V2Launcher.dat",m_strPathSave);

 
   FILE *pf1 = fopen(strConfig.GetBuffer(0),"wb");
   if(pf1)
   {
      char chHeader[23];
      sprintf(chHeader,"NM_V2LAUNCHER_CFG_V004");

      fwrite(&chHeader                          ,1,strlen(chHeader),pf1);
      fwrite(&m_sLauncherConfig.bResolution     ,4,1,pf1);
      fwrite(&m_sLauncherConfig.bWindowed       ,4,1,pf1);
      fwrite(&m_sLauncherConfig.bLauncherMusic  ,4,1,pf1);
      fwrite(&m_sLauncherConfig.dwLanguage      ,4,1,pf1);
      fwrite(&m_sLauncherConfig.dwSelectedServer,4,1,pf1);
      fwrite(&m_sLauncherConfig.bMacro          ,4,12,pf1);
      fwrite(&m_sLauncherConfig.chMacroKey      ,1,12,pf1);
      
      fwrite(&m_sLauncherConfig.dwNbrColor ,4,1,pf1);
      for(int i=0;i<m_sLauncherConfig.dwNbrColor;i++)
      {
         fwrite(&m_sLauncherConfig.CCColor[i] ,sizeof(int),1,pf1);
      }

      fwrite(&m_sLauncherConfig.dwNbrConnection ,4,1,pf1);
      for(int i=0;i<m_sLauncherConfig.dwNbrConnection;i++)
      {
         fwrite(&m_sLauncherConfig.pConnection[i] ,sizeof(sConnectParam),1,pf1);
      }
      fclose(pf1);
   }
 
   // on compresse le fichier et le resave...
   ULONG dwTaille;
   
   FILE *pf2=fopen(strConfig.GetBuffer(0),"rb");
   fseek(pf2,0,SEEK_END);
   dwTaille = ftell(pf2);
   fseek(pf2,0,SEEK_SET);
   unsigned char *pFileDataTmp = NULL;
   
   pFileDataTmp = new unsigned char[dwTaille ];
   fread(pFileDataTmp,dwTaille,1,pf2);
   fclose(pf2); 

   ULONG  dwTailleC = MEM_COMPRESS_NEED(dwTaille);

   unsigned char *pFileDataTmpC = NULL;
   pFileDataTmpC= new unsigned char[dwTailleC];
   
   if( compress(pFileDataTmpC, &dwTailleC, pFileDataTmp, dwTaille) != Z_OK )
      MessageBox("Erreur Compress Config File","",MB_OK);
   
   // save le fichier...
   FILE *pf3=fopen(strConfig.GetBuffer(0),"wb");
   fwrite(&dwTaille,4,1,pf3);
   fwrite(&dwTailleC,4,1,pf3);
   fwrite(pFileDataTmpC,1,dwTailleC,pf3);
   fclose(pf3);
   
   delete []pFileDataTmp;
   delete []pFileDataTmpC;

}

void CMainDialog::UpdateServerList()
{
   // Set OPTIONAL paramters
   CDropList* pList = m_pDropServer.GetList();
   pList->ResetContent();

	pList->SetBkColor(RGB(50,50,50));  		// List background COlor
	pList->SetHighlightBkColor(RGB(150,10,10)); 	// List highlight background color
	
   pList->AddString(m_Language[140],0,RGB(255,255,255),RGB(255,255,255));
	
   for(int i=0;i<m_sLauncherConfig.dwNbrConnection;i++)
      pList->AddString(m_sLauncherConfig.pConnection[i].strName,0,RGB(255,255,255),RGB(255,255,255));

   if(m_sLauncherConfig.dwSelectedServer >=0)
      m_pDropServer.SetSelection(m_sLauncherConfig.dwSelectedServer);
   else if(pList->GetCount() >0)
   {
      m_dwAction = LAST_CONNECT_ACTION_RESET;
      m_sLauncherConfig.dwSelectedServer = 0;
      m_pDropServer.SetSelection(m_sLauncherConfig.dwSelectedServer);
      m_btnConnect.EnableWindow(TRUE);
   }
   else
      m_pDropServer.SetWindowText(m_Language[150]);

   if(m_sLauncherConfig.dwSelectedServer == 0)
   {
      // show DirectConnect Parameters
      GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_SHOW);
      GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_SHOW);
      GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_SHOW);
      GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_SHOW);
	  GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_SHOW);
      ::SetFocus(GetDlgItem(IDCM_STR_IP)->m_hWnd);
   }
   else
   {
      // hide DirectConnect Parameters
      GetDlgItem(IDCM_STR_IP      )->ShowWindow(SW_HIDE);
      GetDlgItem(IDCM_STR_LOGIN   )->ShowWindow(SW_HIDE);
      GetDlgItem(IDCM_STR_PASSWORD)->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_MSTATIC_IP      )->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_MSTATIC_LOGIN   )->ShowWindow(SW_HIDE);
      GetDlgItem(IDC_MSTATIC_PASSWORD)->ShowWindow(SW_HIDE);
	  GetDlgItem(IDC_BMP_BACK_IP   )->ShowWindow(SW_HIDE);
   }
}

void CMainDialog::ChangeConnectStat(BOOL bStat)
{
    if(m_sLauncherConfig.dwSelectedServer>=0 && bStat)
       m_btnConnect.EnableWindow(TRUE);
    else
       m_btnConnect.EnableWindow(FALSE);

}


void CMainDialog::OnBtnConnect() 
{
	m_dwAction = LAST_CONNECT_ACTION_CONENCT; // Connect;
	
}

void CMainDialog::DrawCreditsBackground(CDC *pDC, RECT rect, BOOL bAnimate, DWORD lParam,CBitmap *pBitMap)
{
	static int on1,on2,oon1,oon2;
	pDC->FillSolidRect(&rect,0x00000000);

   CDC dcBitmap;
   dcBitmap.CreateCompatibleDC((CDC *)pDC);

   dcBitmap.SelectObject(pBitMap);
   
   pDC->BitBlt(rect.left,rect.top, rect.right-rect.left, rect.bottom-rect.top, &dcBitmap, 0, 0, SRCCOPY);
   

	
	
	if(bAnimate || (!lParam)) // return now if we are not supposed to do the animation
		return;

	int n1,n2;
	n1 = rand()*200/RAND_MAX-100;
	n2 = rand()*200/RAND_MAX-100;

	// 2/10 chance of (prehaps) making some wild stuff on one of the curves
	if(rand() < RAND_MAX/10)
		n1 = rand()*400/RAND_MAX-200;
	else if(rand() < RAND_MAX/10)
		n2 = rand()*400/RAND_MAX-200;

   rect.left    +=5;
   rect.right   -=5;
   rect.top     +=5;
   rect.bottom  -=5;


	POINT points1[4] = {
		rect.right,5,
		(rect.right-rect.left)/2+n1,(rect.bottom-rect.top)/2,
		(rect.right-rect.left)/2,(rect.bottom-rect.top)/2-n1,
		0,rect.bottom
	};
	POINT points2[4] = {
		5,5,
		(rect.right-rect.left)/2-n2,(rect.bottom-rect.top)/2,
		(rect.right-rect.left)/2,(rect.bottom-rect.top)/2-n2,
		rect.right,rect.bottom
	};
	POINT opoints1[4] = {
		rect.right,0,
		(rect.right-rect.left)/2+on1,(rect.bottom-rect.top)/2,
		(rect.right-rect.left)/2,(rect.bottom-rect.top)/2-on1,
		5,rect.bottom
	};
	POINT opoints2[4] = {
		5,5,
		(rect.right-rect.left)/2-on2,(rect.bottom-rect.top)/2,
		(rect.right-rect.left)/2,(rect.bottom-rect.top)/2-on2,
		rect.right,rect.bottom
	};
	POINT oopoints1[4] = {
		rect.right,5,
		(rect.right-rect.left)/2+oon1,(rect.bottom-rect.top)/2,
		(rect.right-rect.left)/2,(rect.bottom-rect.top)/2-oon1,
		5,rect.bottom
	};
	POINT oopoints2[4] = {
		5,5,
		(rect.right-rect.left)/2-oon2,(rect.bottom-rect.top)/2,
		(rect.right-rect.left)/2,(rect.bottom-rect.top)/2-oon2,
		rect.right,rect.bottom
	};

	CPen wpen(PS_SOLID ,1,RGB(225,210,140));
	CPen pen(PS_SOLID  ,2,RGB(75,58,32   ));
	CPen open(PS_SOLID ,1,RGB(75,58,32   ));
	CPen oopen(PS_SOLID,1,RGB(175,140,90 ));
	CPen *pOldPen = pDC->SelectObject(&oopen);
	pDC->PolyBezier(oopoints1,4);
	pDC->PolyBezier(oopoints2,4);
	pDC->SelectObject(&open);
	pDC->PolyBezier(opoints1,4);
	pDC->PolyBezier(opoints2,4);
	pDC->SelectObject(&pen);
	pDC->PolyBezier(points1,4);
	pDC->PolyBezier(points2,4);
	pDC->SelectObject(&wpen);
	pDC->PolyBezier(points1,4);
	pDC->PolyBezier(points2,4);
	pDC->SelectObject(pOldPen);

	oon1 = on1;
	oon2 = on2;
	on1 = n1;
	on2 = n2;
}


BOOL CMainDialog::GetWindowed()
{
   return m_sLauncherConfig.bWindowed;
}

int  CMainDialog::GetWidth()
{
   if(m_sLauncherConfig.bResolution == 0)
      return 1280; // steph 1280 au lieu de 1024
   else if(m_sLauncherConfig.bResolution == 1)
      return 1024;
   else if(m_sLauncherConfig.bResolution == 2)
      return 800;
   else
      return 1280; // steph 1280 au lieu de 1024

}

int  CMainDialog::GetHeight()
{
   if(m_sLauncherConfig.bResolution == 0)
      return 800; // steph 800 au lieu de 768
   else if(m_sLauncherConfig.bResolution == 1)
      return 768;
   else if(m_sLauncherConfig.bResolution == 2)
      return 600;
   else
      return 800; // steph 800 au lieu de 768
}

int   CMainDialog::GetLanguage()
{
   return m_sLauncherConfig.dwLanguage;
}

char* CMainDialog::GetAccountLogin()
{
   return m_strNMAccountName.GetBuffer(0);
}

char* CMainDialog::GetAccountIP()
{
   return m_strNMAccountIP.GetBuffer(0);
}

char* CMainDialog::GetLogin()
{
   UpdateData(TRUE);
   if(m_sLauncherConfig.dwSelectedServer == 0)
      return m_strDirectLogin.GetBuffer(0);
      
   return m_sLauncherConfig.pConnection[m_sLauncherConfig.dwSelectedServer-1].strLogin;
}

char* CMainDialog::GetPassword()
{
   UpdateData(TRUE);
   if(m_sLauncherConfig.dwSelectedServer == 0)
      return m_strDirectPassword.GetBuffer(0);

   return m_sLauncherConfig.pConnection[m_sLauncherConfig.dwSelectedServer-1].strPass;
}

char* CMainDialog::GetIP()
{
   UpdateData(TRUE);
   if(m_sLauncherConfig.dwSelectedServer == 0)
      return m_strDirectIP.GetBuffer(0);

   return m_sLauncherConfig.pConnection[m_sLauncherConfig.dwSelectedServer-1].strIP;
}

void CMainDialog::OnDestroy() 
{
   m_Language.Release();
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
} 

