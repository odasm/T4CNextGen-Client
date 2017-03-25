// GuildUI.h
//

#if !defined(GuildUI_h)
#define GuildUI_h

#include "BoxUI.h"
#include "ButtonUI.h"
#include "ListUI.h"
#include "StaticTextUI.h"
#include "GraphUI.h"
#include <list>

class GuildUI : public BoxUI
{
public:    
    void Draw( V2SPRITEFX *vsfFX );
    int  TxtOff(Font *, HDC, char*,int) ;
   
    static unsigned int InvitePlayer( void* lpData );
	void Join( const DWORD dwGuildID, const string guildName, const string sourceName );
    void Close( void );

	static unsigned int Display( void* null );

	void ShowButtons();

	static GuildUI* GetInstance();	 

	class MemberUI : public StaticTextUI
	{
		public:
			DWORD userID;
			std::string username;
			std::string userRights;
			short level;
    };

	void UpdateUsersList( std::vector< MemberUI > &vMembers );	

	class Guild
	{
		public:
			DWORD		dwID;
			std::string strName;
			std::string strRights;
	};

	void UpdateGuild( const DWORD dwID, const std::string strName, const std::string strRights );	

	DWORD dwInvitationGuild;

private:

	GuildUI();    	

	Guild currentGuild;	

    class KickButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } kickButtonEvent;
    
    class LeaveButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } leaveButtonEvent;

	class InviteButtonEvent : public EventVisitor
	{
		public:
			void LeftClicked( void );
	} inviteButtonEvent;

	class MemberListEvent : public EventVisitor
	{
		public:
			void LeftClicked( void );
	} memberListEvent;

    friend LeaveButtonEvent;
    friend KickButtonEvent;	
	friend InviteButtonEvent;
	friend MemberListEvent;

    ListUI   memberList;
    ButtonUI leaveButton;
    ButtonUI quitButton;
	ButtonUI inviteButton;
	ButtonUI kickButton;
//    EditUI   addEdit;

	StaticTextUI guildNameText;

    GraphUI  scrollUpGraph;
    GraphUI  scrollDownGraph;
    GraphUI  selSprite;

    GraphUI  MemberBack;    

	MemberUI selectedUser;

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

	class QuitPopup : public BoxUI
	{
		public:
			QuitPopup();

		private:
			class QuitEvent : public EventVisitor
			{
				public:
					void LeftClicked();
			} quitEvent;

			void Draw( V2SPRITEFX *vsfFX );
			int  TxtOff(Font *, HDC, char*,int) ;

			ButtonUI cancelButton;
			ButtonUI okButton;
			GraphUI  intlText;
			GraphUI  graphPatch;
			GraphUI  quitBack;
			StaticTextUI textArea;
    } quitPopup;

    friend QuitPopup;    
    friend JoinPopupUI;
};

#endif /* GuildUI_h */
