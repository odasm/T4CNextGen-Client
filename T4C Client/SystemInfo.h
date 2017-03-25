//
//
//



// Include Files.
#ifndef __SYSTEMINFO
#define __SYSTEMINFO


#define SI_WIN_NT_351 51
#define SI_WIN_NT_40  4
#define SI_WIN_95     95

//********************************************************//
class SystemInfo 
//********************************************************//
// Store the System Information
//
{
	private:
		HINSTANCE hInstance;   // Windows Instance.
		HWND      hWnd;        // Window Handle.
        BYTE      OpSystem;    // Operating System
        DWORD     mainWndThreadId; // Thread Id of main thread.
        HHOOK     hMainWndHook;

	public:
		SystemInfo ( void );  // Constructor.
		~SystemInfo( void ); // Destructor.

		void      SethInstance( HINSTANCE ); // Set the Windows Instance.
		HINSTANCE GethInstance( void );      // Get the Windows Instance.

		void      SethWnd( HWND ); // Set the Window Handle.
		HWND      GethWnd( void ); // Get the Window Handle.

        void      SetMainWndThreadId( DWORD dw );
        DWORD     GetMainWndThreadId();

        void      SetMainWndHook( HHOOK hHook );
        HHOOK     GetMainWndHook();

      void      SetSystemOS( void ); // Set the Operating System
      BYTE      GetSystemOS( void ); // Get the Operating System
};

#define siCreate() \
   if (!ug_siApp_22486) \
      ug_siApp_22486 = new SystemInfo;

#define siSethInstance(hInstance) \
   ug_siApp_22486->SethInstance(hInstance)

#define siGethInstance() \
   ug_siApp_22486->GethInstance()

#define siRelease() \
   if (ug_siApp_22486) \
      delete ug_siApp_22486

#define siSethWnd(hWnd) \
   ug_siApp_22486->SethWnd(hWnd)

#define siGethWnd() \
   ug_siApp_22486->GethWnd()

#define siGetSystemOS() \
   ug_siApp_22486->GetSystemOS()

#define siSetSystemOS() \
   ug_siApp_22486->SetSystemOS()

#define siSetMainWndThreadId\
   ug_siApp_22486->SetMainWndThreadId

#define siGetMainWndThreadId\
   ug_siApp_22486->GetMainWndThreadId

#define siSetMainWndHook\
    ug_siApp_22486->SetMainWndHook

#define siGetMainWndHook\
    ug_siApp_22486->GetMainWndHook


extern SystemInfo  *ug_siApp_22486;

#endif