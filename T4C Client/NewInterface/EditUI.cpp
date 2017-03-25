// EditUI.cpp
//
#pragma warning( disable : 4291 )

#include "../pch.h"
#include "EditUI.h"
#include "StaticTextUI.h"
#include "../MemoryManager.h"


EditUI::EditUI( const std::string defaultText, unsigned long defaultMaxTextLength )
    :   cursorBlinkOffTime( 300 ),
        cursorBlinkOnTime ( 300 ),
        nextCursorStateChange( 0 ),
        cursorState  ( false ),    
        cursorEnabled( false ),
        currentText( defaultText ),
        maxTextLength( defaultMaxTextLength ),
        cursorPos( 0 ),
        textOffset( 0 ),
        filter( NULL )
        
{
    text = FontManager::GetInstance()->GetSmallFont(FALSE);

    text->SetFlag( TO_DRAW_QUOTES );

    StaticTextUI *theCursor = new StaticTextUI;

    theCursor->SetText( "|" );    

    theCursor->SetPos( FramePos( 0, 0, theCursor->GetWidth(), theCursor->GetHeight() ) );

    cursor = theCursor;
}

//////////////////////////////////////////////////////////////////////////////////////////
void EditUI::EnableCursor
//////////////////////////////////////////////////////////////////////////////////////////
// Enables or disables the cursor for edit boxes.
// 
(
 bool newState // The new cursor enable state.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    cursorEnabled = newState;
}

//////////////////////////////////////////////////////////////////////////////////////////
void EditUI::SetText
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the edit box's text.
// 
(
 const std::string newText // The new text.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    currentText = newText;

    //text->SetText( currentText.c_str(), RGB( 255, 255, 255 ), 640, 0, 0 );

    cursorPos = currentText.size();

    // Set the cursor.
    SetCursorCorrection();

}

//////////////////////////////////////////////////////////////////////////////////////////
void EditUI::SetMaxTextLength
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets tha maximum length of a string. If this maximum is reached, the control will
//  no longer accept keyboard input.
// 
(
 unsigned long newMaxTextLength // The new max length.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    maxTextLength = newMaxTextLength;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool EditUI::DeleteChar
//////////////////////////////////////////////////////////////////////////////////////////
// Delete one character at the cursor pos (hitting DELETE key)
//
(
    void
)
//////////////////////////////////////////////////////////////////////////////////////////
{
	if (cursorPos < currentText.size()) {
		int newSize = currentText.size() - 1;

		if( newSize <= 0 ){
			currentText = "";
			cursorPos = 0;
		}else{
			currentText = currentText.substr(0, cursorPos) + currentText.substr(cursorPos+1, currentText.size()-cursorPos);
		}            
		// Set the cursor.
		SetCursorCorrection();
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////
bool EditUI::TextInput
//////////////////////////////////////////////////////////////////////////////////////////
// Add a character to the string.
//
(
    char ch
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If this isn't an extended character, and the character isn't 0.
    if( ch != 0 ){
        switch( ch ){
        // Enter
        case VK_RETURN:
            // Do nothing
        break;
      case VK_BACK:{
			if (cursorPos > 0) {
				int newSize = currentText.size() - 1;

				if( newSize <= 0 ){
					currentText = "";
					cursorPos = 0;
				}else{
					currentText = currentText.substr(0, cursorPos-1) + currentText.substr(cursorPos, currentText.size()-cursorPos);
					cursorPos--;
				}            
				// Set the cursor.
				SetCursorCorrection();
			}
        }break;
        // For all other characters.
        default:            
            
            bool goOn = true;
            // If the filter does not accept this character
            if( filter != NULL && !filter->IsOK( ch, currentText ) ){
                goOn = false;
            }
            
            // If adding a character doesn't make the string larger than accepted.
            if( goOn && currentText.size() < maxTextLength ){
                // If the cursor is at the end of the string.
                if( cursorPos == currentText.size() ){
                    // Append the new character to the string.
                    currentText += ch;
                    // Advance the cursor.
                    cursorPos++;
                }else 
                // Otherwise if the cursor is at the beginning of the string.    
                if( cursorPos == 0 ){
                    // Set ch as beginning of string.
                    std::string newText;
                    newText = ch;

                    newText += currentText;
                    // Set new current text.
                    currentText = newText;
                    // Advance the cursor.
                    cursorPos++;
                }
                // If cursor was in the middle of the string.
                else{
                    // Get the string until the cursor position.
                    std::string newText( currentText.substr( 0, cursorPos ) );
                    // Append ch after it.
                    newText += ch;
                    // Then add the rest of the string.
                    newText += currentText.substr( cursorPos, currentText.size() );                    
                    // Set the new current text.
                    currentText = newText;
                    // Advance the cursor.
                    cursorPos++;
                }
            
                // Set the cursor.
                SetCursorCorrection();

                //text->SetText( currentText.c_str(), RGB( 255, 255, 255 ), 640, 0, 0 );
            }else{
                // Beep.
                // TODO.
            }
        }
    }

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void EditUI::Draw( V2SPRITEFX *vsfFX )
//////////////////////////////////////////////////////////////////////////////////////////
// Draws the edit box.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    if( !IsShown() ){
        return;
    }
    
    V2SPRITEFX fx;
    memset( &fx, 0, sizeof( fx ) );

    RECT clipRect = { GetPos().x, GetPos().y, GetPos().maxx, GetPos().maxy };
    
    fx.lpClipRect = &clipRect;
    
    // Draw normal UI
    GameUI::Draw( vsfFX );

    // Display edit text.
    text->Draw( GetPos().x - textOffset, GetPos().y, &fx );

    // If the cursor is enabled
    if( cursorEnabled && cursor != NULL ){

        bool proceed = true;

        // If a parent UI was provided and its last clicked item
        // doesn't correspond to this editUI.
        //if( parentUI != NULL && parentUI->GetLastClickedItem() != this ){
        //    // Don't draw.
        //    proceed = false;
        //}
        //
        // If cursor isn't blinked off
        if( cursorState && proceed ){
            // Display cursor
            cursor->Draw( vsfFX );
        }

        // If the currrent state has timed out.
        if( GetTickCount() >= nextCursorStateChange ){
            // Set new state and determine next state change.
            switch( cursorState ){
            case true:
                cursorState = false;
                nextCursorStateChange = GetTickCount() + cursorBlinkOffTime;
                break;
            case false:
                cursorState = true;
                nextCursorStateChange = GetTickCount() + cursorBlinkOnTime;
                break;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool EditUI::LeftClick
//////////////////////////////////////////////////////////////////////////////////////////
//  Allows changing the cursor at different parts of the string.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // TODO.

    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void EditUI::SetParent
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the parent UI. This helps the edit UI know when to blink.
// 
(
 GameUI *theParentUI // The parent UI.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    parentUI = theParentUI;
}

//////////////////////////////////////////////////////////////////////////////////////////
void EditUI::MoveCursorPos
//////////////////////////////////////////////////////////////////////////////////////////
//  Overrides to allow positionning the cursor.
// 
(
 int positions, // How many positions to walk (positive -> move right AND negative -> move left)
 int modifiers // CTRL, ALT, SHIFT States -> 0=none; 1=CTRL; 2=ALT; 4=SHIFT
)
//////////////////////////////////////////////////////////////////////////////////////////
{
	if (modifiers == 0) 
   {
		UINT test = cursorPos + positions;
		if (test < 0) 
         cursorPos = 0;
		else if (test > currentText.size()) 
         cursorPos = currentText.size();
		else 
         cursorPos = test;
	} 
   else if (modifiers == 1) 
   { //CTRL down
		if (positions > 0) 
      {
			while (cursorPos < currentText.size()) 
         {
				cursorPos++;
				if (cursorPos < currentText.size() && currentText.at(cursorPos) == ' ') 
            {
					cursorPos++;
					break;
				}
			}
		} 
      else 
      {
			while (cursorPos > 0) 
         {
				cursorPos--;
				if (cursorPos > 0 && currentText.at(cursorPos-1) == ' ') 
            {
					break;
				}
			}
		}
		if (cursorPos < 0) 
         cursorPos = 0;
		else if (cursorPos > currentText.size()) 
         cursorPos = currentText.size();
	}
	SetCursorCorrection();
}


//////////////////////////////////////////////////////////////////////////////////////////
void EditUI::SetPos
//////////////////////////////////////////////////////////////////////////////////////////
//  Overrides to allow positionning the cursor.
// 
(
 FramePos framePos // The position.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    GameUI::SetPos( framePos );

    FramePos curFrame = cursor->GetPos();

    curFrame.SetNewOffset(
        MousePos(
            framePos.x,
            framePos.y
        )
    );
    
    cursor->SetPos( curFrame );
}

//////////////////////////////////////////////////////////////////////////////////////////
void EditUI::SetCursorCorrection( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Calibrates the edit line.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
   char strTmp[2000];
   sprintf(strTmp,"%s",currentText.c_str());
   strTmp[cursorPos] = 0x00;
   
   //text->SetText( currentText.c_str(), RGB( 255, 255, 255 ), 0, 0, 0 );
   text->SetText(strTmp, RGB( 255, 255, 255 ), 0, 0, 0 );

   textOffset = 0;
   // If the text is bigger than the control
   if( text->GetWidth() > GetPos().Width()  && cursorPos >0)
   {
      // If the cursor is at the end of the text.
      if( cursorPos >= strlen(strTmp)/*currentText.size()*/ )
      {
         // Put cursor at end of text.            
         textOffset = text->GetWidth() - GetPos().Width();
      }
      /*
      else
      {
         
         text->SetText( strTmp, RGB( 255, 255, 255 ), 0, 0, 0 );
         textOffset = text->GetWidth() - GetPos().Width();
      }
      */
   }
   text->SetText( currentText.c_str(), RGB( 255, 255, 255 ), 0, 0, 0 );
   
   // Calculate the new cursor position.
   FramePos curFrame = cursor->GetPos();
   
   DWORD textWidth = 0;
   if( cursorPos > 0 )
   {
      std::string cursorText( currentText.substr( 0, cursorPos ) );
      textWidth = text->GetWidth( cursorText.c_str() );
   }
   
   curFrame.SetNewOffset( MousePos(GetPos().x + textWidth - textOffset,GetPos().y));
   cursor->SetPos( curFrame );
}

//////////////////////////////////////////////////////////////////////////////////////////
void EditUI::SetFilter
//////////////////////////////////////////////////////////////////////////////////////////
//  Sets the current filter.
// 
(
 Filter * f // ..
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    filter = f;
}
