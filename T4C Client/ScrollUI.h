#ifndef SCROLLUI_H
#define SCROLLUI_H

#include "GraphUI.h"

// Implements scrolling capabilities.
class ScrollUI{
public:
    ScrollUI()
        :   listSize( 0 ),
            scrollUpRegion( 0, 0, 0, 0 ),
            scrollDownRegion( 0, 0, 0, 0 ),
            freeScrollRegion( 0, 0, 0, 0 ),
            scrollButton( NULL ),
            linePos( 0 ),
            dragging( false ),
            scrollUpDownUI( NULL ),
            scrollDownDownUI( NULL ),
            pressUp( false ),
            pressDown( false ),
            nextPress( 0xFFFFFFFF ),
            soundEnabled( true )
    {
        defaultScroll.SetSpriteId( "GUI_ScrollTick" );
        defaultScroll.SetPos( FramePos( 0, 0, 24, 22 ) );
        defaultScroll.Show();

        scrollButton = &defaultScroll;
    }
     
    void SetScrollRegions( const FramePos &theScrollUpRegion, const FramePos &theScrollDownRegion, const FramePos &theFreeScrollRegion, GraphUI *theScrollButton = NULL ){
        scrollUpRegion   = theScrollUpRegion;
        scrollDownRegion = theScrollDownRegion;
        freeScrollRegion = theFreeScrollRegion;
        if( theScrollButton != NULL ){
            scrollButton     = theScrollButton;
        }
    }

    void ScrollDown( unsigned long lines ){
        // Scroll down.
        
        // If current line position is smaller than the quantity of liens we want to scroll down.
        if( linePos < lines ){
            // Set to max.
            linePos = 0;
        }else{
            // Otherwise remove the desired quantity of lines.
            linePos -= lines;
        }
        ScrollChanged();
    }

    void ScrollUp( unsigned long lines ){
        linePos += lines;
    
        // If the line is further than the end.
        if( linePos >= listSize ){
            // Set line pos to last position.
            if( listSize == 0 ){
                linePos = 0;
            }else{
                linePos = listSize - 1;
            }
        }
        ScrollChanged();
    }

    void SetScrollPos( DWORD pos ){
        linePos = pos;
        // If current line position is smaller than the quantity of liens we want to scroll down.
        if( linePos >= listSize ){
            // Set line pos to last position.
            if( listSize == 0 ){
                linePos = 0;
            }else{
                linePos = listSize - 1;
            }
        }
        ScrollChanged();
    }
    DWORD GetScrollPos(){
        return linePos;
    }

    // Provide a facility to draw the scroll bar slider button.
    void DrawScroll( V2SPRITEFX *vsfFX );

    bool ScrollClick( MousePos mousePos );
    bool IsMouseInScrollBar( MousePos mousePos );

    virtual void SetScrollUpDownUI( GameUI *newUI );
    virtual void SetScrollDownDownUI( GameUI *newUI );
    virtual void SetScrollUI( GraphUI *newUI );

    // Virtual callback called when the scroll bar position has changed.
    virtual void ScrollChanged( void ){};

    void EnableSound( bool enable ){
        soundEnabled = enable;
    }

protected:
    inline void SetListSize( unsigned long newSize ){
        listSize = newSize;
        // If the line is further than the end.
        if( linePos >= listSize ){
            // Set line pos to last position.
            if( listSize == 0 ){
                linePos = 0;
            }else{
                linePos = listSize - 1;
            }
        }

        ScrollChanged();
    }
    unsigned long GetListSize(){
        return listSize;
    }

    inline void DragScrollButton( MousePos mousePos );

    DWORD nextPress;

    FramePos scrollUpRegion;
    FramePos scrollDownRegion;
    FramePos freeScrollRegion;

    GraphUI *scrollButton;

    GraphUI defaultScroll;

    GameUI *scrollUpDownUI;
    GameUI *scrollDownDownUI;

    bool pressUp;
    bool pressDown;

    unsigned long linePos;

    bool dragging;

    bool soundEnabled;

private:
    unsigned long listSize;
};

#endif