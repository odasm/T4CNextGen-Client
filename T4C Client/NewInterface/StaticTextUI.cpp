// StaticTextUI.cpp
//
#include "../pch.h"

#include "StaticTextUI.h"
#include "../MemoryManager.h"


//////////////////////////////////////////////////////////////////////////////////////////
 StaticTextUI::StaticTextUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
:
    text( NULL ),
    center( true )
{
}

// Destructor ////////////////////////////////////////////////////////////////////////////
 StaticTextUI::~StaticTextUI( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Deletes the associated text object.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
     if( text != NULL ){
        delete text;
     }
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticTextUI::Draw( V2SPRITEFX *vsfFX )
//////////////////////////////////////////////////////////////////////////////////////////
//  Draw
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Draw the under sprite.
    GameUI::Draw( vsfFX );

    if( text != NULL ){
        // If the width or height is non 0.
        if( center ){
            // Draw the static text.
            text->Draw( 
                GetPos().Width()  / 2 - text->GetWidth()  / 2 + GetPos().x,
                GetPos().Height() / 2 - text->GetHeight() / 2 + GetPos().y,
                vsfFX
            );
        }else{
            text->Draw( 
                GetPos().x, 
                GetPos().y,
                vsfFX
            );
        }
    }

    // Draw the help.
    GameUI::DrawHelp( vsfFX );
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticTextUI::SetText
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the text for the control.
// 
(
 std::string newText,// The text.
 DWORD color,        // the color.
 int x,
 int y,
 bool background
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If the text is non-existent.
    if( text == NULL ){
        // Create a new one with the set text.
        text = FontManager::GetInstance()->GetSmallFont(FALSE);
        text->SetText( const_cast< char * >( newText.c_str() ), color, x, y, background );
    }else{
        // Reset the item's text.
        text->SetText( const_cast< char * >( newText.c_str() ), color, x, y, background );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticTextUI::SetPos
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
(
 FramePos framePos // 
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    GameUI::SetPos( framePos );
    if( text == NULL ){
        return;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void StaticTextUI::AutoTruncate
//////////////////////////////////////////////////////////////////////////////////////////
// Truncates a text if its too long for the given position
// 
(
 DWORD maxSize  // Maximum size of text item before truncating
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If the text is not too large for the given position.
    if( text->GetWidth() <= maxSize ){
        return;
    }

    string str = text->GetText();

    if( str.size() <= 4 ){
        text->ResetText( "..." );
        return;
    }

    str.resize( str.size() - 4 );
    str += "...";
    
    DWORD width = text->GetWidth( str.c_str() );
    while( width > maxSize ){
        if( str.size() <= 4 ){
            text->ResetText( "..." );            
            //LOG.Unlock();
            return;
        }
        
        str.resize( str.size() - 4 );
        str += "...";

        width = text->GetWidth( str.c_str() );
    }

    text->ResetText( str.c_str() );
}