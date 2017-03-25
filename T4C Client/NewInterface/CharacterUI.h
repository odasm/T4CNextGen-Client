// CharacterUI.h
//

#if !defined(CharacterUI_h)
#define CharacterUI_h

#include "BoxUI.h"
#include "StaticTextUI.h"
#include "ListUI.h"
#include "ButtonUI.h"
#include "GraphUI.h"
#include "MacroUI.h"

class CharacterUI : public BoxUI
{
public:
    ~CharacterUI();

    void DrawMinimized();
    void Draw( V2SPRITEFX *vsfFX );
    int  TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);

    static CharacterUI *GetInstance();
    static unsigned int Display( void *null );

    void UpdateCharacterSheet( void );

    void UpdateSkills( void );


    void ClientInitialize();

    void DrawStatus( void );

private:
    CharacterUI();


	// The use skill position callback
	static UINT UseSkillPosition( LPVOID lpData );
	// The use skill unit callback
	static UINT UseSkillUnit( LPVOID lpData );


    void DrawSingleStatusItem( GraphUI &fullBar, int percent );

    // List containers.
    class SkillUI : public GraphUI{
    public:
        USER_SKILL *userSkill;
    };

    void UseSkill( SkillUI *skillUI );

    class SkillEvent : public EventVisitor{
    public:
        void LeftDblClicked( void );
    } skillEvent;


    class StatUpgEvent : public EventVisitor{
    public:
        void LeftClicked( void );

        int stat;
    } upgSTRevent,
      upgENDevent,
      upgAGIevent,
      upgWISevent,
      upgINTevent;

    class StatDngEvent : public EventVisitor{
    public:
        void LeftClicked( void );

        int stat;
    } dngSTRevent,
      dngENDevent,
      dngAGIevent,
      dngWISevent,
      dngINTevent;

    class StatCommitEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } statsCommitEvent;

    friend StatCommitEvent;
    friend StatUpgEvent;
    friend StatDngEvent;
    friend SkillEvent;
    

    ButtonUI upgSTRbutton;
    ButtonUI upgENDbutton;
    ButtonUI upgAGIbutton;
    ButtonUI upgWISbutton;
    ButtonUI upgINTbutton;

    ButtonUI dngSTRbutton;
    ButtonUI dngENDbutton;
    ButtonUI dngAGIbutton;
    ButtonUI dngWISbutton;
    ButtonUI dngINTbutton;

    ButtonUI statsCommitButton;

    ButtonUI  iconSTR;
    ButtonUI  iconEND;
    ButtonUI  iconAGI;
    ButtonUI  iconWIS;
    ButtonUI  iconINT;
    ButtonUI  iconAC;
    ButtonUI  iconLevel;
    ButtonUI  iconHP;
    ButtonUI  iconMP;
    ButtonUI  iconWeight;
	ButtonUI  iconLuck;

    ButtonUI  X_Button;
    GraphUI   backStat;

    GraphUI   skillScrollUp;
    GraphUI   skillScrollDown;

    GraphUI   m_MainStatBack;
    GraphUI   m_MainStatEmpty;
    GraphUI   m_MainStatHP;
    GraphUI   m_MainStatMP;
    GraphUI   m_MainStatXP;

    GraphUI   m_MainStatEmptyHP2;
    GraphUI   m_MainStatEmptyMP2;
    GraphUI   m_MainStatEmptyXP2;
    GraphUI   m_MainStatHP2;
    GraphUI   m_MainStatMP2;
    GraphUI   m_MainStatXP2;

    // Displayed stats.
    StaticTextUI statSTR;
    StaticTextUI statEND;
    StaticTextUI statAGI;
    StaticTextUI statWIS;
    StaticTextUI statINT;
	StaticTextUI statLCK;
    StaticTextUI statAC;
    StaticTextUI statLevel;
    
    StaticTextUI statHP;
    StaticTextUI statMP;

    StaticTextUI statPnts;
    StaticTextUI skillPnts;
    StaticTextUI statXP;
    StaticTextUI statXP2GO;
    StaticTextUI weight;
    StaticTextUI karma;

    ListUI       skillList;
    GraphUI      skillSelectionGraphPatch;

    int upgSTR;
    int upgEND;
    int upgAGI;
    int upgWIS;
    int upgINT;

    int usedStatPts;

    // Macro callbacks.
    class SkillMacroEvent : public MacroEvent{
        virtual void ExecMacro( MacroDataUI *graphUI );
        virtual void SaveMacro( const list< MacroDataUI * > &macroList );
    } skillMacroEvent;
    class SkillMacroUI : public MacroDataUI{
    public:
        string name;
        DWORD skillId;
    };

    friend SkillMacroEvent;

    class MacroButtonEvent : public EventVisitor{
    public:
        void LeftClicked();
    } macroButtonEvent;
    friend MacroButtonEvent;

    ButtonUI macroButton;
};

#endif /* CharacterUI_h */
