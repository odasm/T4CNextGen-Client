// StaticTextUI.h
//

#if !defined(StaticTextUI_h)
#define StaticTextUI_h

#include "ControlUI.h"


class StaticTextUI : public ControlUI
{
public:
    StaticTextUI();
    ~StaticTextUI();

    void SetCenter( bool newState ){
        center = newState;
    }

    void Draw( V2SPRITEFX *vsfFX );

    void SetText( std::string newText, DWORD color = RGB( 255, 255, 255 ), int = g_Global.GetScreenW(), int = 0, bool = false );

    int GetHeight(){
        if( text != NULL ){
            return text->GetHeight();
        }
        return 0;
    }
    int GetWidth(){
        if( text != NULL ){
            return text->GetWidth();
        }
        return 0;
    }

    TextObject *GetText( void ){
        return text;
    }

    void AutoTruncate( DWORD maxSize );

    virtual void SetPos( FramePos framePos );

private:
    bool center;

    TextObject *text;

};

#endif /* StaticTextUI_h */
