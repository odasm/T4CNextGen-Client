// GroupPlayUI.h
//

#if !defined(GroupPlayUI_h)
#define GroupPlayUI_h

#include "BoxUI.h"
#include "ButtonUI.h"
#include "ListUI.h"
#include "StaticTextUI.h"
#include "GraphUI.h"
#include <list>

class GroupPlayUI : public BoxUI
{
public:
    ~GroupPlayUI();

    class GroupMember{
    public:
        DWORD       id;
        WORD        level;
        WORD        hpPercent;
        bool        leader;
        std::string name;
    };

    static unsigned int Display( void *nil );    
    static GroupPlayUI *GetInstance();

    void Join( DWORD sourceID, std::string sourceName );

    void UpdateMembers( const std::list< GroupMember > &members, bool autoShare );

    void UpdateMemberHp( DWORD id, WORD hpPercent );

    void UpdateAutoShare( bool autoShare );

private:

   int  TxtOff(Font *, HDC, char*,int) ;
   void ResolveGraphPatches( void );


    static unsigned int InvitePlayer( void *data );

    GroupPlayUI();

    class JoinPopupUI : public BoxUI{
    public:
        JoinPopupUI();

        // Used to intercept ESC.
        virtual bool VKeyInput( VKey vKey );

        void SetText( std::string text );
        void Draw( V2SPRITEFX *vsfFX );

        void Decline( void );
        int  TxtOff(Font *, HDC, char*,int) ;

    private:
        class AcceptButtonEvent : public EventVisitor{
        public:
            void LeftClicked( void );
        } acceptButtonEvent;

        class DeclineButtonEvent : public EventVisitor{
        public:
            void LeftClicked( void );        
        } declineButtonEvent;

        friend AcceptButtonEvent;
        friend DeclineButtonEvent;        

        ButtonUI acceptButton;
        ButtonUI declineButton;
        
        GraphUI  JoinBack;
        GraphUI  graphPatch;
        StaticTextUI textArea;
    } joinPopup;

    friend JoinPopupUI;

    class TextMemberUI : public StaticTextUI, public GroupMember{
    public:
        void SetMember( const GroupMember &member ){
            id = member.id;
            level = member.level;
            hpPercent = member.hpPercent;
            leader = member.leader;
            name = member.name;
        }

        void Draw( V2SPRITEFX *vsfFX );
    };

    class HpMemberUI : public ControlUI, public GroupMember{
    public:
        HpMemberUI( void );
        void SetMember( const GroupMember &member )
        {
            id = member.id;
            level = member.level;
            hpPercent = member.hpPercent;
            leader = member.leader;
            name = member.name;
        }

        // Overload the drawing of the HP.
        void Draw( V2SPRITEFX *vsfFX );
    private:
        GraphUI emptyBar;
        GraphUI fullBar;
    };


    class InviteButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } inviteButtonEvent;

    class LeaveButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } leaveButtonEvent;

    class ExpelButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );        
    } expelButtonEvent;

    class AutoShareButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } autoShareButtonEvent;

    friend LeaveButtonEvent;
    friend InviteButtonEvent;
    friend ExpelButtonEvent;
    friend AutoShareButtonEvent;

    ButtonUI inviteButton;
    ButtonUI leaveButton;
    ButtonUI expelButton;
    ButtonUI autoShareButtonON;
    //ButtonUI autoShareButtonOFF;
    

    ButtonUI disabledInviteButton;
    ButtonUI disabledLeaveButton;
    ButtonUI disabledExpelButton;
    ButtonUI disabledAutoShareButtonON;
    //ButtonUI disabledAutoShareButtonOFF;

    //GraphUI  intlTitle;

    GraphUI  backImage;
    GraphUI  backBtnSplitD;
    GraphUI  memberSelectionGraphPatch;
    BOOL     m_bShowDisableSplit;

    ListUI   memberList;

    int      memberSize;
    bool     autoShare;
    bool     weAreTheLeader;

   typedef struct _sMinimizedInfo
   {
      BOOL     bVisible;
      GraphUI  grfBack;
      GraphUI  grfHP;

   }sMinimizedInfo;

   sMinimizedInfo m_MinimizedInfo[7];



    // Minimized
    ButtonUI M_Button;
    ButtonUI X_Button;

    virtual void Minimize( bool newMinimizedState );
    virtual void DrawMinimized( V2SPRITEFX *vsfFX );
    virtual void Draw(V2SPRITEFX *vsfFX);

};

#endif /* GroupPlayUI_h */
