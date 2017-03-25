// EditUI.h
//

#if !defined(EditUI_h)
#define EditUI_h

#include "ControlUI.h"
#include <string>

class EditUI : public ControlUI
{
public:

    EditUI( const std::string defaultText = "", unsigned long maxTextLength = 256 );

    virtual bool TextInput( char ch );
    virtual void Draw( V2SPRITEFX *vsfFX );
    virtual bool LeftClick( MousePos mousePos );

    std::string GetText( void ){
        return currentText;
    }
    void SetText( const std::string newText );
    void SetMaxTextLength( unsigned long newMaxTextLength );
    void EnableCursor( bool newState );

    void SetParent( GameUI *parentUI );

    void SetCursor( GameUI *newCursor ){
        cursor = newCursor;        
    }

	bool DeleteChar(void); //Delete one character at the cursor pos (hitting DELETE key)	
	void MoveCursorPos (int positions, int modifiers=0); // Move cursor positions to right (negative values go left)
    void SetPos( FramePos framePos );

    class Filter{
    public:
        virtual bool IsOK( char nextCh, const std::string currentText ) = 0;
    };

    void SetFilter( Filter *f );

private:

    void SetCursorCorrection( void );

    GameUI         *parentUI;

    TextObject     *text;

    GameUI         *cursor;
    Filter         *filter;

    // Cursor state vars.
    unsigned long   cursorBlinkOffTime;
    unsigned long   cursorBlinkOnTime;    
    unsigned long   nextCursorStateChange;
    bool            cursorState;
    bool            cursorEnabled;    

    // Text vars.
    std::string	  currentText;
    unsigned long   maxTextLength;
    unsigned long   cursorPos;
    unsigned long   textOffset;
};

#endif /* EditUI_h */
