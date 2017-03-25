// TextPageUI.cpp: implementation of the TextPageUI class.
//
//////////////////////////////////////////////////////////////////////
#include "../pch.h"

#include "TextPageUI.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TextPageUI::TextPageUI() : minLineSize( g_Global.GetScreenW() )
{

}

TextPageUI::~TextPageUI()
{
    unsigned int i;
    for( i = 0; i < screenText.size(); i++ ){
        delete screenText[ i ];
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void TextPageUI::Draw
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the text page.
// 
(
 V2SPRITEFX *vsfFX
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    DrawScroll( vsfFX );

    // Setup the clipping rectangle
    RECT rect = { textFrame.x, textFrame.y, textFrame.maxx + 1, textFrame.maxy };
    V2SPRITEFX fx;
    memset( &fx, 0, sizeof( fx ) );

    fx.lpClipRect = &rect;

    // Specify no correction.
    fx.dwFX |= FX_NOCORRECTION;

    // Draw the currently displayed backlog portion.
    unsigned int i;
    int offset = initialOffset;
    for( i = 0; i < screenText.size(); i++ ){
        screenText[i]->Draw( 
            textFrame.x,
            textFrame.y + offset,
            &fx
        );
        offset += screenText[i]->GetHeight();
    }

}

//////////////////////////////////////////////////////////////////////////////////////////
bool TextPageUI::WheelUp
//////////////////////////////////////////////////////////////////////////////////////////
// Mouse wheel went up! Move list up (scroll up).
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    ScrollUp( 1 );

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool TextPageUI::WheelDown
//////////////////////////////////////////////////////////////////////////////////////////
// Mouse wheel went down! Move list down (scroll down).
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    ScrollDown( 1 );

    return true;
}

bool TextPageUI::RightClick( MousePos mousePos )
{
    return true;
}

bool TextPageUI::LeftMouseDown( MousePos mousePos )
{
    ScrollClick( mousePos );
    
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool TextPageUI::LeftMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
// The mouse was released on top of this control.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TextPageUI::AddText
//////////////////////////////////////////////////////////////////////////////////////////
//  Adds a paragraph to the page. The block is line feeded
// 
(
 std::string str,
 DWORD textColor,
 bool allowNewLine
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    Paragraph p;

    // Create an exact, temporary copy of what would show up on screen.
    TextObject *tempText = FontManager::GetInstance()->GetSmallFont(TRUE);
    tempText->SetText( str.c_str(), textColor, textFrame.Width(), 0, 0, allowNewLine, true );

    // Setup a paragraph object according to the information supplied
    // by the temporary formatted TextObject;
    p.text  = str;
    p.lines = tempText->GetLineCount();
    p.textColor = textColor;
    p.fontSize = tempText->GetFontSize();
    p.allowNewLine = allowNewLine;
    textList.push_back( p );

    const int maxLoggedLines = 10000;
    const int removedLines = 500;

    // If the backlog is getting too big
    if( textList.size() > maxLoggedLines ){
        list< Paragraph >::iterator k;
        // Get the removeLines'th iterator
        int z = 0;
        for( k = textList.begin(); z < removedLines; z++, k++ );
        
        // Erase the 1st removedLines entries
        textList.erase( textList.begin(), k );
        
        totalLines -= removedLines;
        
        // Scroll down to keep the current view insync.
        ScrollDown( removedLines );
    }

    // If the font size is smaller then in any previous line.
    int fontSize = tempText->GetFontSize();
    if( fontSize < minLineSize )
    {
        minLineSize = fontSize;
        maxLines = textFrame.Height() / minLineSize;

        UpdateViewSize();
        //LOG.Lock();
        //LOG << "\r\n(AddText)TextPage Max lines " << maxLines << "...";
        //LOG.Unlock();
    }
    else if(fontSize > minLineSize)
    {
       minLineSize = fontSize;
       maxLines = textFrame.Height() / minLineSize;
       
       UpdateViewSize();
       //LOG.Lock();
       //LOG << "\r\n(AddText)TextPage Max lines " << maxLines << "...";
       //LOG.Unlock();
    }

    // Setup the new list size.
    int prevListSize = totalLines - maxLines;
    if( prevListSize < 0 ){
        prevListSize = 0;
    }

    totalLines += p.lines;
    
    int listSize = totalLines - maxLines + 1;
    if( listSize < 0 ){
        listSize = 0;
    }
    
    SetListSize( listSize );

    // If the scroll position pointed to the end of the previous text area.
    if( linePos == (unsigned long)prevListSize ){
        // Scroll down to the end of the text area.
        ScrollUp( totalLines );        
    }

    delete tempText;
}

void TextPageUI::ClearList()
{
   textList.clear();
   int dwNbrLine = totalLines;
   totalLines -= dwNbrLine;
   ScrollDown( dwNbrLine );
   UpdateViewSize();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TextPageUI::SetPos
//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
(
 FramePos framePos
)
// Return: virtual void, 
//////////////////////////////////////////////////////////////////////////////////////////
{
    GameUI::SetPos( framePos );
}

//////////////////////////////////////////////////////////////////////////////////////////
void TextPageUI::SetTextArea
//////////////////////////////////////////////////////////////////////////////////////////
// Reset the text area.
// 
(
 FramePos itextFrame
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    //int prevSize = maxLines;//BLBLBL 29 mars 2009 suppression des choses inutilisées
    
    textFrame = itextFrame;
    
    // Determine the new maximum quantity of lines.
    maxLines = textFrame.Height() / minLineSize;

    LOG.Lock();
    LOG << "\r\n(SetTextArea)TextPage Max lines " << maxLines << "...";
    LOG.Unlock();

    UpdateViewSize();    

    int listSize = totalLines - maxLines + 1;
    if( listSize < 0 ){
        listSize = 0;
    }

    SetListSize( listSize );

    ScrollUp( totalLines );
};


//////////////////////////////////////////////////////////////////////////////////////////
bool TextPageUI::DragCycle
//////////////////////////////////////////////////////////////////////////////////////////
//  Received a drag notification.
// 
(
 MousePos mousePos // The mouse pos.
)

//////////////////////////////////////////////////////////////////////////////////////////
{
    LeftMouseUp( mousePos );
    return true;
}


//////////////////////////////////////////////////////////////////////////////////////////
void TextPageUI::ScrollChanged( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Scroll changed, requery the currently displayed items.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    UpdateBacklogView();
}

//////////////////////////////////////////////////////////////////////////////////////////
void TextPageUI::UpdateBacklogView( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the current backlog view
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    int totalLineCount = 0;
    list< Paragraph >::iterator i;
    for( i = textList.begin(); i != textList.end(); i++ ){        
        
        // If we found the scroll offset
        if( (unsigned long)((*i).lines + totalLineCount) > linePos ){
            // Get the initial offset if inside a multiline paragraph.
            initialOffset = 0;
            if( GetListSize() != 0 ){
                initialOffset = ( totalLineCount - linePos ) * (*i).fontSize;
            }
            // Set the texts for all visible lines.
            unsigned int c;
            for( c = 0; c < screenText.size(); c++ ){
                screenText[ c ]->SetText(
                    (*i).text.c_str(),
                    (*i).textColor,
                    textFrame.Width(),
                    0,
                    0,
                    (*i).allowNewLine,
                    true
                );
                
                // If any not visible lines remain, clear them.
                i++;
                if( i == textList.end() ){                    
                    for( c++; c < screenText.size(); c++ ){
                        screenText[ c ]->SetText("");
                    }
                    return;
                }
            }
            return;
        }
        totalLineCount += (*i).lines;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void TextPageUI::UpdateViewSize( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Updates the viewing size. Does not keep data from the previous screenText
// buffer.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    unsigned int i;
    // Delete all previous text entries
    for( i = 0; i < screenText.size(); i++ )
    {
        delete screenText[i];
    }

    TextObject *tempText = FontManager::GetInstance()->GetSmallFont(TRUE);
    tempText->SetText("zZ");
    int fontSize = tempText->GetFontSize();
    minLineSize = fontSize;
    maxLines = textFrame.Height() / minLineSize;
    delete tempText;

    screenText.resize( maxLines + 5 );
    // Re-create all text entries.
    for( i = 0; i < screenText.size(); i++ )
    {
        screenText[i] = FontManager::GetInstance()->GetSmallFont(TRUE);
    }
}