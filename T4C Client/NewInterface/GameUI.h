// GameUI.h
//

#if !defined(GameUI_h)
//#ifndef GameUI_h
#define GameUI_h

#include <windows.h>
#include <deque>
#include <string>
#include "EventVisitor.h"
#include "EmptyEvent.h"
#include "../V2Sprite.h"
#include "../PowerDebug.h"
#include "../VisualObjectList.h"
#include "../TFCPlayer.h"
#include "../mouse.h"
#include "../Comm.h"
#include "../NewFont.h"
#include "../VKey.h"
#include "../LocalString.h"
#include "../GameIcons.h"

using namespace vir;

#pragma warning( disable : 4786 )

class MousePos{
public:
    MousePos()
        : x( 0 ), y( 0 ){};
    MousePos( int newX, int newY ) 
        : x( newX ), y( newY ){};
    MousePos( POINT pnt )
        : x( pnt.x ), y( pnt.y ){};

    
    int x, y;
};

class FramePos{
public:
    FramePos()
        : x( 0 ), y( 0 ), maxx( 0 ), maxy( 0 ){};
    FramePos( int newX, int newY, int newMaxX, int newMaxY ) 
        : x( newX ), y( newY ), maxx( newMaxX ), maxy( newMaxY ){};

    void SetPos( int newX, int newY, int newMaxX, int newMaxY ){
        x = newX; y = newY; maxx = newMaxX; maxy = newMaxY;
    }

    bool MousePosInFrame( const MousePos &mousePos ){
        // If the mouse position is within this frame
        return( mousePos.x >= x &&
                mousePos.y >= y &&
                mousePos.x <  maxx &&
                mousePos.y <  maxy );
    }

    void SetNewOffset( MousePos posOffset ){
        // Set current variables in relative mode.
        maxx -= x;
        maxy -= y;
        x = 0;
        y = 0;

        // Add the offset;
        x    += posOffset.x;
        y    += posOffset.y;
        maxx += posOffset.x;
        maxy += posOffset.y;
    }

    int Width(){
        return maxx - x;
    }
    int Height(){
        return maxy - y;
    }

    int x, y;
    int maxx, maxy;
};



class StringCompareI{
public:
    bool operator()( const std::string &r, const std::string &l ) const{
        return( stricmp( r.c_str(), l.c_str() ) < 0 );
    }
};    

// Abstract base class.
class GameUI
{
public:    
    GameUI();
    virtual ~GameUI();

    virtual void Hide();
    virtual void Show( bool putForeground = true );
    virtual void ShowLoad(BYTE *pData, BYTE* pPal,int dwW, int dwH, bool putForeground = true );
    virtual void DrawAlpha(bool bAlpha = false);
    bool IsShown( void );
    bool IsMinimized( void ){return minimizedState;}
    bool IsDrawAlpha( void ){return bDrawBlending;}

    FramePos GetPos() const;

    // User event functions.
    virtual bool LeftMouseDown( MousePos mousePos );
    virtual bool LeftMouseUp( MousePos mousePos );
    virtual bool RightMouseDown( MousePos mousePos );
    virtual bool RightMouseUp( MousePos mousePos );
    virtual bool DragCycle( MousePos mousePos );
    virtual bool WheelUp( MousePos mousePos );
    virtual bool WheelDown( MousePos mousePos );
    virtual bool TextInput( char ch );
    virtual bool VKeyInput( VKey vKey );

    // Window child functions
    virtual void AddChild( GameUI *childUI );
    virtual void RemoveChild( GameUI *childUI );
    virtual void DeleteChild( GameUI *childUI );
    virtual void PutChildInForeground( GameUI *child );
    virtual void PutChildInBackground( GameUI *child );
    static GameUI *GetRoot();
            
    // Drawing functions.
    virtual void Draw( V2SPRITEFX *vsfFX );    
    // Draws the help text.
    virtual void DrawHelp( V2SPRITEFX *vsfFX );

    // Accessors.
    virtual void SetPos( FramePos framePos );
    virtual CV2Sprite *GetVSF( void ){
        return &vsfSprite;
    }

    // General functionnalities function.
    virtual void          Minimize( bool newMinimizedState );    
    virtual unsigned long GetUnitAt( MousePos mousePos );
    virtual void          ClientInitialize( void );
    virtual void          ClientShutdown( void );

    // Global drag and drop functions.    
    static bool          SetDragItem( GameUI *dragItem, EventVisitor *parentEvent, GameUI *parentUI, MousePos initialPos );
    static GameUI       *GetDragItem( void );
    static EventVisitor *GetDragSourceEvent( void );
    static GameUI       *GetDragParentUI( void );
    static void          CancelDrag( void );
    static MousePos      GetDragInitialPos( void );

    virtual void SetSpriteId( std::string newSpriteId );
    virtual std::string GetSpriteId( void );
    virtual bool IsSpriteDefined();

    void SetHelpText( TextObject *text )
    {
       if( helpText != NULL )
          delete helpText;
       helpText = text;
       ResyncHelpText();
    }
    void ResyncHelpText( void );

    TextObject *GetHelpText( void ){
        return helpText;
    }

    void DisplayHelp( bool newDisplayState, int x, int y );

    GameUI *GetLastClickedItem( void ){
        return lastClickedItem;
    }

    static bool IsLeftMouseDown();

    bool IsHelpDisplayed(){
        return helpDisplayState;
    }

protected:
    typedef std::deque< GameUI * > ChildQueue;

    // Protected child functions.
    ChildQueue *GetChildList();

    // Draw minimized.
    virtual void DrawMinimized( V2SPRITEFX *vsfFX );
    
    // Global user event functions
    static bool IsDblClick();
    static bool IsClick();
    
    // Global foreground request functions.
    void RequestForegroundControl( GameUI *child );
    void DismissForegroundControl( void );
    bool IsForegroundControlled( void );

    // Data
    static GameUI      *foregroundChild;

    // Provide a default event for closing a UI.
    class HideEvent : public EventVisitor{
    public:
        void LeftClicked( void ){
            GetUI()->Hide();
        }
    } hideEvent;

    // Provide a default event for minimizing a UI.
    class MinimizeEvent : public EventVisitor{
    public:        
        void LeftClicked( void ){
            GetUI()->Minimize( true );
        }
    } minimizeEvent;


    // Mouse state variables.
    enum{
        ClickTime = 250
    };

    static DWORD        lastClickTime;
    static enum MouseState{
        MouseNone,
        MouseClick,
        MouseDblClick
    } currentMouseState;

    static void SetUIMouseCursor( GameUI *gameUI, int xOff, int yOff );

    static bool IsParent( GameUI *parent, GameUI *child );

    GameUI *lastClickedItem;

private:       
    
    FramePos     framePos;

    ChildQueue   childList;
    

    std::string  spriteId;
    CV2Sprite    vsfSprite;
    TextObject  *helpText;
    bool         minimizedState;
    bool         hiddenState;
    bool         helpDisplayState;
    bool         bDrawBlending;
    int          helpDisplayX;
    int          helpDisplayY;
    int          helpInitialX;
    int          helpInitialY;

    // Global variables
    static GameUI       *draggedItem;
    static EventVisitor *draggedEvent;
    static GameUI       *draggedParentUI;
    static MousePos      draggedInitialPos;
protected:
    static bool          leftMouseDown;
};

extern 

inline void SetHelpId( GameUI *gameUI, DWORD helpId ){
    gameUI->SetHelpText( FontManager::GetInstance()->GetSmallFont(FALSE, g_LocalString[ helpId ] ) );
}
inline void SetHelpId( GameUI &gameUI, DWORD helpId ){
    SetHelpId( &gameUI, helpId );
}

#endif /* GameUI_h */
