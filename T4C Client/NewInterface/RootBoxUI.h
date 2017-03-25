// RootBoxUI.h
//

#if !defined(RootBoxUI_h)
#define RootBoxUI_h

#include "BoxUI.h"
#include "..\Lock.h"

#define DISABLE_TMI_MAP 0

#define MENU_NONE 0
#define MENU_BOTH 1
#define MENU_CHAT 2
#define MENU_TMI  3

// Singleton.
class RootBoxUI : public BoxUI
{
public:
    friend GameUI;

    // The root box is responsible for making the user functions
    // public and implement them thread safe.

    // Make these members public.
    virtual bool LeftMouseDown( MousePos mousePos );
    virtual bool LeftMouseUp( MousePos mousePos );
    virtual bool RightMouseDown( MousePos mousePos );
    virtual bool RightMouseUp( MousePos mousePos );
    virtual bool WheelUp( MousePos mousePos );
    virtual bool WheelDown( MousePos mousePos );
    virtual bool DragCycle( MousePos mousePos );
    virtual bool TextInput( char ch );
    virtual bool VKeyInput( VKey vKey );


    // Make this function public.
    virtual void Draw( V2SPRITEFX *vsfFX );
    virtual void DrawHelp( V2SPRITEFX *vsfFX );

    // Make these members public.
    virtual void AddChild( GameUI *child );
    virtual void RemoveChild( GameUI *child );
    virtual void DeleteChild( GameUI *child );

    bool IsMouseOwned( MousePos mousePos );

    static RootBoxUI *GetInstance();

    void Lock( void ){
        threadLock.Lock();
    };

    void Unlock( void ){
        threadLock.Unlock();
    }

    CLock *GetLock(){
        return &threadLock;
    }

    bool IsMaximized( void );

	void SetSideMenuState( void );

    MousePos GetRootScreenOffset();

private:
    BoxUI::GetHitChild;

    CLock threadLock;

    bool mouseOwned;
    bool chatShown;
    bool menuShown;

    RootBoxUI();
};

#endif /* RootBoxUI_h */
