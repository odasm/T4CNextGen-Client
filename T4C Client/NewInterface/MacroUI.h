// MacroUI.h
//

#if !defined(MacroUI_h)
#define MacroUI_h

#include "BoxUI.h"
#include "ListUI.h"
#include "ButtonUI.h"
#include "GridUI.h"
#include "GraphUI.h"
#include "StaticTextUI.h"

class MacroDataUI : public GraphUI{
public:
    MacroDataUI() : gridPos( 0 ), autoTargetSelf( false ){};
    VKey vKey;
    BYTE gridPos;
    bool autoTargetSelf;
};

class MacroEvent{
public:
    virtual void ExecMacro( MacroDataUI *macroUI ) = 0;
    virtual void SaveMacro( const list< MacroDataUI * > &macroList ) = 0;
};


class MacroUI : public BoxUI
{
public:

    static MacroUI *GetInstance();
    static unsigned int Display( void *null );
    void Hide();
    virtual void Draw( V2SPRITEFX *vsfFX );
    virtual void DrawMinimized( V2SPRITEFX *vsfFX );
    virtual void Minimize( bool newMinimizedState );

    void AddMacro( MacroDataUI *macroUI, string macroName, MacroEvent *macroEvent, bool echo = true );
    void ResetMacro( void );

    virtual void ClientShutdown( void );

    bool CallMacro( VKey vKey );

    void DisableMacro( bool state ){
        disableMacro = state;
    }

    void SaveMacros();

private:
    MacroUI();

    bool KeyExists( VKey vKey );
    int TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);

    class MacroPopupUI : public BoxUI{
    public:
		class AcceptButtonEvent : public EventVisitor {
		public:
			void LeftClicked();
		} acceptButtonEvent;
        MacroPopupUI();
        void Draw( V2SPRITEFX *vsfFX );
        int  TxtOff(Font *pFont, HDC hdc, char *pStr,int dwBoxZise);

        bool SetKey( VKey ch );
       
        virtual bool VKeyInput( VKey vKey );

        VKey GetKey();

        DWORD macroId;
        MacroUI *parentUI;
        
    private:
        VKey vKey;
        class ClearButtonEvent : public EventVisitor{
        public:
            void LeftClicked();
        } clearButtonEvent;
        class CancelButtonEvent : public EventVisitor{
        public:
            void LeftClicked();
        } cancelButtonEvent;
        
        friend ClearButtonEvent;
        friend AcceptButtonEvent;
        friend CancelButtonEvent;

        ButtonUI        clearButton;
        ButtonUI        acceptButton;
        ButtonUI        cancelButton;
        StaticTextUI    keyText;
        GraphUI         intlText;
    } macroPopup;

    friend MacroPopupUI;
    friend MacroPopupUI::AcceptButtonEvent;

    class MacroItemUI : public ControlUI{
    public:
        MacroItemUI( MacroEvent *imacroEvent, MacroDataUI *macroUI, string macroName );
        ~MacroItemUI();

        void Draw( V2SPRITEFX *fx ){
            if( macroUI != NULL ){
                macroUI->SetPos( GetPos() );
                macroUI->Draw( fx );
            }
        }

        DWORD        macroId;
        MacroDataUI *macroUI;
        MacroEvent  *macroEvent;
        string       macroName;
    };

    class MacroListEvent : public EventVisitor{
    public:
        void LeftDblClicked();
    } macroListEvent;
    friend MacroListEvent;

    ListUI macroList;
    GraphUI macroListSelection;

    class AddSpellButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );        
    } addSpellButtonEvent;
    class AddSkillButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );        
    } addSkillButtonEvent;
    class AddItemButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );        
    } addItemButtonEvent;
    friend AddSpellButtonEvent;
    friend AddItemButtonEvent;
    friend AddSkillButtonEvent;

    class RemoveMacroButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } removeMacroButtonEvent;
    friend RemoveMacroButtonEvent;

    ButtonUI addSpellButton;
    ButtonUI addSkillButton;
    ButtonUI addItemButton;
    ButtonUI removeMacroButton;

    // Macro grid
    class AddToGridButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } addToGridButtonEvent;

    class RemoveFromGridButtonEvent : public EventVisitor{
    public:
        void LeftClicked( void );
    } removeFromGridButtonEvent;
    friend AddToGridButtonEvent;
    friend RemoveFromGridButtonEvent;

    class MacroGridItemUI : public GraphUI{
    public:
        MacroGridItemUI();
        virtual void Draw( V2SPRITEFX *fx );

        virtual bool LeftMouseDown( MousePos mousePos );

        MacroItemUI *macroItem;
        GraphUI macroItemHighlight;
    };

    friend MacroGridItemUI;

    MacroItemUI *selectedGridItem;   

    bool AddToGrid( MacroItemUI *macroItem ,int dwPos = 0);
    void RemoveFromGrid( MacroItemUI *macroItem );

    int gridSize;
    

    ButtonUI addToGridButton;
    ButtonUI removeFromGridButton;

    GraphUI  scrollUpGraph;
    GraphUI  scrollDownGraph;

    GraphUI   backMacro;

    bool disableMacro;

    ButtonUI  X_Button;
 public:
   vector< MacroGridItemUI > macroGrid;
};

#endif /* MacroUI_h */
