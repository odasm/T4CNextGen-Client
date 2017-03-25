
#ifdef T4C_LAUNCHER_EXPORTS
   #define T4C_LAUNCHER_API __declspec(dllexport)
#else
   #define T4C_LAUNCHER_API __declspec(dllimport)
#endif

#define LAST_CONNECT_ACTION_IDLE    0
#define LAST_CONNECT_ACTION_EXIT    1
#define LAST_CONNECT_ACTION_CONENCT 2
#define LAST_CONNECT_ACTION_RESET   3
#define LAST_CHANGE_LANGUAGE        4
 


// This class is exported from the CTest.dll
class T4C_LAUNCHER_API CT4CLauncher 
{
public:
	CT4CLauncher();
   ~CT4CLauncher();

   void CreateNMDialog(char *pstrTitle,char *pstrVersion,char *pstrPathGame,char *pstrPathSave,char *pstrLogin,char *pstrIP);
   void FreeNMDialog();
   BOOL IsLuncherAvailable();
   UINT GetLastAction();
   void AddConnectMessage(char *pstrMsg, int dwLineIndex,BOOL bChangeCOnnect = FALSE);
   void AddGeneralMessage(char *pstrMsg);
   void ForceConnectState(BOOL bState);
   void ProcessMessage();
   BOOL GetWindowed();
   int  GetWidth();
   int  GetHeight();
   char* GetLogin();
   char* GetPassword();
   char* GetIP();
   char* GetAccountLogin();
   char* GetAccountIP();
   int   GetLanguage();


protected:
	// TODO: add your methods here.
};

