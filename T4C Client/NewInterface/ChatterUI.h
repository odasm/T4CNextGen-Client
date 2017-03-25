// ChatterUI.h
//

#if !defined(ChatterUI_h)
#define ChatterUI_h

#pragma warning( disable : 4786 )

#include "BoxUI.h"
#include "ListUI.h"
#include "StaticTextUI.h"
#include "ButtonUI.h"
#include "EditUI.h"
#include "GraphUI.h"
#include "TextPageUI.h"
#include <string>
#include <list>
#include <set>
#include <map>

#define SYSTEM_COLOR RGB( 0, 100, 255 )


class ChatterUI : public BoxUI
{
public:    
    ~ChatterUI();

    void UpdateMacroHelpID();

    typedef std::set< std::string, StringCompareI > IgnoreSet;
    
    class Channel{
    public:
        std::string channelID;
        bool        listenState;
    };
    class User{
    public:
        std::string userName;
        std::string title;
        bool        listenState;
    };

    void UpdateChannels    ( const std::vector< Channel > &suscribedChannels );
    void UpdateChannelUsers( const std::vector< User >    &channelUsers );
    void AddBackscrollItem ( std::string fromUser, std::string newText, DWORD baseColor, bool screenEcho, bool allowNewLine = true );
    void ClearBackScroll();
    void ClientInitialize();

    virtual bool TextInput( char ch );
    virtual bool VKeyInput( VKey vKey );

    virtual void SendMessageToCurrentChannel( const string &msg );

    void ClientShutdown( void ){ Save(); };

	ChatterUI(ChatterUI const&) = delete;             // Copy construct
	ChatterUI(ChatterUI&&) = delete;                  // Move construct
	ChatterUI& operator=(ChatterUI const&) = delete;  // Copy assign
	ChatterUI& operator=(ChatterUI &&) = delete;      // Move assign

    static ChatterUI *GetInstance();
    static unsigned int Display( void *null );

    std::string GetSelectedChannel(){
        return selectedChannel;
    }

    IgnoreSet *GetIgnoreList(){
        return &ignoreList;
    }

    void EnterChatterChannel( std::string channelID, std::string pwd, DWORD color ,bool bEnable);

    bool IsUserIgnored( std::string userName );

    void SetBacklogFile( char *fileName );
    char * GetBacklogFile();


    void SetInputText( const char *txt );
    std::string GetInputText();

    DWORD GetChannelColor( std::string channelId );

private:
    ChatterUI();
	/* Here will be the instance stored. */
	static ChatterUI* instance;

    
    void Load( void );
    void Save( void );

    static UINT  WINAPI ChannelRefreshThread( void *nil );
    static HANDLE channelRefreshThreadHandle;

	list<string> rollbackTyped; // List to allow rolling back the text you have already typed.
	list<string>::iterator ii;

    class JoinButtonEvent;
    class JoinPopupUI : public BoxUI{
    public:
        class AcceptEvent : public EventVisitor{
        public:
            void LeftClicked( void );
        } acceptEvent;   
        JoinPopupUI();

        void ResetUI( void );

        virtual bool TextInput( char ch );
        virtual bool VKeyInput( VKey vKey );

        void Accept( void );
        void Draw( V2SPRITEFX *vsfFX );
        int  TxtOff(Font *, HDC, char*,int) ;

    private:     

        ButtonUI acceptButton;
        ButtonUI cancelButton;
        EditUI   channelEdit;
        EditUI   pwdEdit;

        GraphUI  graphPatch;
        GraphUI  JoinBack;        
    } joinPopup;

    class IgnoreListPopupUI : public BoxUI{
    public:
        IgnoreListPopupUI();

        void ResetUI( void );
        void Draw( V2SPRITEFX *vsfFX );
        int  TxtOff(Font *, HDC, char*,int) ;
       
        void Add( void );
        void Close( void );

        virtual bool TextInput( char ch );        

    private:
        class UserIgnoreUI : public StaticTextUI{
        public:
            std::string userID;
        };

        class RememberButtonEvent : public EventVisitor{
        public:
            void LeftClicked( void );
        } rememberButtonEvent;
        
        class AddButtonEvent : public EventVisitor{
        public:
            void LeftClicked( void );
        } addButtonEvent;

        friend AddButtonEvent;
        friend RememberButtonEvent;

        ListUI   ignoreList;
        ButtonUI rememberButton;
        ButtonUI addButton;
        ButtonUI quitButton;
        EditUI   addEdit;

        GraphUI  scrollUpGraph;
        GraphUI  scrollDownGraph;
        GraphUI  selSprite;

        GraphUI  IgnoreBack;
    } ignorePopup;

    friend IgnoreListPopupUI;

    friend JoinPopupUI;
    friend JoinPopupUI::AcceptEvent;

    class ChannelUI : public StaticTextUI{
    public:
        enum color{
            cyan   = RGB( 17 , 181, 205 )/*,
            red    = RGB( 255,  33,  75 ),
            blue   = RGB( 90 , 119, 216 ),
            yellow = RGB( 220, 202,   6 ),
            green  = RGB( 5  , 197,  37 ),
            purple = RGB( 199,  10, 204 ),
            grey   = RGB( 192, 192, 192 ),
            orange = RGB( 224, 100,  10 )*/
        };

        std::string channelID;
        bool        listenState;
        color       channelColor;
    };

    

    class ChannelToggleUI : public ButtonUI{
    public:
        ChannelToggleUI() : ButtonUI( EmptyEvent::GetInstance() ){};

        std::string channelID;
        bool        listenState;
    };

    class UserUI : public StaticTextUI{
    public:
        std::string userID;
    };

    class ChannelListEvent : public EventVisitor{
    public:
        void LeftClicked( void );
        void LeftDblClicked( void );
    } channelListEvent;

    class UserListEvent : public EventVisitor{
    public:
        void LeftClicked( void );
        void LeftDblClicked( void );
    } userListEvent;

    class LeaveButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } leaveButtonEvent;

    friend UserListEvent;
    friend ChannelListEvent;
    friend LeaveButtonEvent;


    class Macro0Event : public EventVisitor{
    public:
        void LeftDblClicked( void );
    } macro0Event;
    class Macro1Event : public EventVisitor{
    public:
        void LeftDblClicked( void );
    } macro1Event;
    class Macro2Event : public EventVisitor{
    public:
        void LeftDblClicked( void );
    } macro2Event;
    class Macro3Event : public EventVisitor{
    public:
        void LeftDblClicked( void );
    } macro3Event;
    class Macro4Event : public EventVisitor{
    public:
        void LeftDblClicked( void );
    } macro4Event;
    class Macro5Event : public EventVisitor{
    public:
        void LeftDblClicked( void );
    } macro5Event;


    friend Macro0Event;
    friend Macro1Event;
    friend Macro2Event;
    friend Macro3Event;
    friend Macro4Event;
    friend Macro5Event;

    ButtonUI macroBtn0;
    ButtonUI macroBtn1;
    ButtonUI macroBtn2;
    ButtonUI macroBtn3;
    ButtonUI macroBtn4;
    ButtonUI macroBtn5;

    ListUI channelList;
    ListUI userList;

    // Button events.
    class JoinButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } joinButtonEvent;

    friend JoinButtonEvent;
    
    class PageToggleButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } pageToggleButtonEvent;

    class ChannelToggleButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } channelToggleButtonEvent;

    class IngameToggleButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } ingameToggleButtonEvent;

    class IgnoreButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } ignoreButtonEvent;

    class RememberButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } rememberButtonEvent;

    class PageButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } pageButtonEvent;

    class IgnoreListButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } ignoreListButtonEvent;

    friend IgnoreButtonEvent;
    friend RememberButtonEvent;
    friend PageToggleButtonEvent;
    friend ChannelToggleButtonEvent;
    friend IngameToggleButtonEvent;
    friend PageButtonEvent;
    friend IgnoreListButtonEvent;

   
    ButtonUI joinButton;
    ButtonUI leaveButton;
    ButtonUI ignoreButton;
    ButtonUI rememberButton;
    ButtonUI pageButton;
    ButtonUI pageoffButton;
    ButtonUI ignoreListButton;

    
   
    GraphUI  channelListingSelectionGraphPatch;
    GraphUI  userListingSelectionGraphPatch;

    GraphUI  channelListScrollUp;
    GraphUI  channelListScrollDown;
    GraphUI  userListScrollUp;
    GraphUI  userListScrollDown;

    StaticTextUI userQtyTxt;

    TextPageUI textBackscroll;
    EditUI     textInput;

    ButtonUI pageToggleButton;
    ButtonUI channelToggleButton;
    ButtonUI ingameToggleButton;
    ButtonUI X_Button;

    GraphUI  backscrollScroll;
    GraphUI  backscrollScrollUp;
    GraphUI  backscrollScrollDown;

    GraphUI  backChatCC;

    EditUI   pageNameEdit;
    GraphUI  pageNameGraphPatch;

    enum TextInputState{
        SendToChannel = 1,
        SendToPage = 2,
        SendToGame = 3
    } textInputState;

    std::string selectedChannel;
    std::string selectedUser;

    IgnoreSet ignoreList;

    class RegChannel{
    public:
        std::string channelID;
        std::string pwd;
        DWORD color;
        bool  bActive;

        bool operator < ( const RegChannel &r ) const{
            return( stricmp( channelID.c_str(), r.channelID.c_str() ) < 0 );
        }
    };

    std::set< RegChannel > regChannels;
    std::map< std::string, DWORD > savedColor;
    std::map< std::string, bool  > savedStatus;


    //////////////////////////////////////////////////////////////////////////////////////////
    // MINIMIZED PORTION
public:
    virtual void DrawMinimized( V2SPRITEFX *vsfFX );
    virtual void Draw( V2SPRITEFX *vsfFX );
    virtual void Minimize( bool newMinimizedState );
    int TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);
    void SetMinimizedHight(bool bMax);
    bool IsMinimizedHight(){return m_bMinH;}
    void UpdateChatViewSize(){textBackscroll.UpdateViewSize();};
    static unsigned int ToggleChatSize(void *);

private:

   class MinHButtonEvent : public EventVisitor{
   public:
        void LeftClicked( void );
   } minHButtonEvent;

   friend MinHButtonEvent;

   bool m_bMinimizedMax;
   bool m_bMinH;
   bool m_bOldMinH;
   GraphUI minimizedGraph;
   GraphUI minimizedGraphHL;
   GraphUI minimizedGraphHR;
   GraphUI minimizedGraphHHHB;
   GraphUI minimizedGraphHHHT;
   GraphUI sideFill;
   GraphUI sideFillH;
   GraphUI sideFillHHH;
   ButtonUI MinHButton;

   enum MenuItem
   {
        CharSheet,
        Inv,
        Spells,
        GroupPlay,
        Chatter,
        Macro,
        Options
    };

    class MenuItemEvent : public EventVisitor
    {
       public:
           MenuItemEvent( MenuItem i )
           {
               mi = i;
           };

           void LeftClicked( void );
       private:
           MenuItem mi;

    }   charSheetEvent,
        spellsEvent,
        invEvent,
        groupPlayEvent,
        chatterEvent,
        macroEvent,
        optionsEvent;

    friend MenuItemEvent;

    ButtonUI charSheetButton;
    ButtonUI invButton;
    ButtonUI spellsButton;
    ButtonUI groupPlayButton;
    ButtonUI chatterButton;
    ButtonUI macroButton;
    ButtonUI optionsButton;
};

#endif /* ChatterUI_h */
