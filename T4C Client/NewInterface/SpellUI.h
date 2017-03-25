// SpellUI.h
//

#if !defined(SpellUI_h)
#define SpellUI_h

#include "BoxUI.h"
#include "ListUI.h"
#include "StaticTextUI.h"
#include "ButtonUI.h"
#include "GraphUI.h"
#include "MacroUI.h"
#include "AnimUI.h"
#include <list>

class SpellUI : public BoxUI
{
public:
    ~SpellUI();


    static SpellUI *GetInstance();
    virtual void Draw( V2SPRITEFX *vsfFX );
    int  TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);
    static unsigned int Display( void *null );
    void Hide();

    struct Spell{
        string name;
        string desc;
        WORD   Id;
        DWORD  duration;
        enum{ mental, physical } type;
        BYTE   targetType;
        WORD   manaCost;
        WORD   level;
        DWORD  icon;

        enum Element{
            fire = 1, water = 2, air = 3, earth = 4, light = 5, dark = 6, normal = 7
        } element;

        inline bool operator < ( const Spell &spell2 ) const;
    };

    void UpdateSpells( const list< Spell > &spellList );

    enum TargetPriority{
        monsterPriority = 0,
        equalPriority   = 1,
        playerPriority  = 2
    };   

    bool CastSpell( DWORD spellId, bool autoTargetSelf );

    void CastLastSpell();

protected:


private:
    SpellUI();

    class SpellClickEvent : public EventVisitor{
    public:
        SpellClickEvent( int newSpellNum ) : spellNum( newSpellNum ){}

        void LeftDblClicked();
        void LeftClicked();

        int spellNum;
    } spellEvent1,
      spellEvent2,
      spellEvent3,
      spellEvent4;
    friend SpellClickEvent;

    class PrevClickEvent : public EventVisitor{
    public:
        void LeftClicked();
    } prevClickEvent;

    class NextClickEvent : public EventVisitor{
    public:
        void LeftClicked();
    } nextClickEvent;
    
    friend PrevClickEvent;
    friend NextClickEvent;

    // Internal SpellPage controls.
    class SpellPageUI : public ControlUI{
    public:
        SpellPageUI( EventVisitor &event );

        Spell spell;

        virtual void Draw( V2SPRITEFX *vsfFX );
        virtual bool LeftMouseUp( MousePos mousePos );
        virtual void SetPos( FramePos framePos );

        void Select( bool selectState );

        inline void FillSpellPage( const Spell &spell );
        inline void ClearPage( void );
    
        inline GraphUI *GetSelectSprite( void ){
            return &selectSprite;
        }

    private:
        EventVisitor &spellEvent;

        StaticTextUI name;
        StaticTextUI level;
        StaticTextUI duration;
        StaticTextUI spellType;
        StaticTextUI manaCost;
        GraphUI      icon;

        GraphUI selectSprite;

        FramePos initialIconPos;
    } spell1, spell2, spell3, spell4;    

    list< Spell > spellList;

    void UpdateSpellPages( void );
    void UpdateIndexes( void );
    void ClearIndex( Spell::Element element );
    int  currentPage;

    class SpellPageButtonUI : public ButtonUI{
    public:
        SpellPageButtonUI( EventVisitor &eventVisitor ) : ButtonUI( eventVisitor )
        {};
        
        virtual void Draw( V2SPRITEFX *vsfFx );

        AnimUI *flapAnim;

    } prevButton, nextButton;
    friend SpellPageButtonUI;

    AnimUI flapAnimPrev;
    AnimUI flapAnimNext;

    DWORD  lastSpell;

    class ElementEvent : public EventVisitor{
    public:
        ElementEvent( Spell::Element ielement ) : element( ielement )
        {}

        void LeftClicked( void );

        Spell::Element element;
    } airEvent, 
      fireEvent, 
      earthEvent, 
      waterEvent, 
      lightEvent, 
      darkEvent, 
      normalEvent;

    friend ElementEvent;

    ButtonUI airButton;
    ButtonUI fireButton;
    ButtonUI earthButton;
    ButtonUI waterButton;
    ButtonUI lightButton;
    ButtonUI darkButton;
    ButtonUI normalButton;

    ButtonUI X_Button;

    int      m_dwHighLightSpellID;

    GraphUI intlText;
    GraphUI intlPage;   
    GraphUI backSpell;

    AnimV2Sprite castCursor;

    // Mouse callbacks
    MouseDevice mouse;

    void CastSpell( const Spell &spell, bool autoTargetSelf, bool noCallback = false );

    static UINT CastSpellPosition( void *data );
    static UINT CastSpellUnit( void *data );

    // Macro callbacks.
    class SpellMacroEvent : public MacroEvent{
        virtual void ExecMacro( MacroDataUI *graphUI );
        virtual void SaveMacro( const list< MacroDataUI * > &macroList );
    } spellMacroEvent;

    friend SpellMacroEvent;
    
    class SpellMacroUI : public MacroDataUI{
    public:
        Spell spell;
        WORD  iconId;
    };

    class MacroButtonEvent : public EventVisitor{
    public:
        void LeftClicked();
    } macroButtonEvent;
    friend MacroButtonEvent;

    ButtonUI macroButton;

    virtual void ClientInitialize( void );

//public:
//    void CastLastSpell( int targetPriority, WORD ID, WORD ManaCost, BYTE SpellTarget, BOOL NoCallBack);
};

#endif /* SpellUI_h */
