// BoxUI.cpp
//
#include "../pch.h"
#include "BoxUI.h"
#include "RootBoxUI.h"
#include "../MemoryManager.h"
#include "SideMenu.h"
#include "ChatterUI.h"
#include "MacroUI.h"
#include "..\App.h"

#include "..\SaveGame.h"
extern CSaveGame g_SaveGame;


namespace{
    T3VSBSound *boxShowSound = NULL;

    void InitSound(){
        if( !boxShowSound ){
            boxShowSound = new T3VSBSound;
            boxShowSound->Create( "UI Box open and close", TS_MEMORY );
        }
    }

};

BoxUI::BoxUI() : lastDrag( 0 )
{
    InitSound();
}

BoxUI::~BoxUI(){
}

extern char Force;

//////////////////////////////////////////////////////////////////////////////////////////
GameUI *BoxUI::GetHitChild
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the child hit by a mouse position.
// 
(
 MousePos mousePos
)
// Return: GameUI, the gameUI object. NULL if non were found.
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If there is a foreground child.
    // If that foreground child is not the current box.
    // If that foreground child isn't a parent of this box.
    if( foregroundChild != NULL && foregroundChild != this && !IsParent( foregroundChild, this ) ){
        // Return it.
        return foregroundChild;
    }else{
        // Analyze all children of this UI.
        ChildQueue *childList = GetChildList();        

        ChildQueue::iterator i;
        for( i = childList->begin(); i != childList->end(); i++ )
        {
            // If the child is showed and the position is within the child's frame.
            if( (*i)->IsShown() && (*i)->GetPos().MousePosInFrame( mousePos ) )
            {
                // Return the child.
                return( *i );
            }            
        }
    }

    return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool BoxUI::LeftMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
//  Left mouse pressed.
// 
(
 MousePos mousePos // Mouse position.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Get the hit child
    GameUI *hitChild = GetHitChild( mousePos );

    // If a child was targetted by this mouse position.
    if( hitChild != NULL ){
        lastClickedItem = hitChild;

        // Call the child's function.
        hitChild->LeftMouseDown( mousePos );
        return true;
    }
    
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool BoxUI::LeftMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Left mouse released
// 
(
 MousePos mousePos // Mouse position
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Get the hit child
    GameUI *hitChild = GetHitChild( mousePos );

    // If a child was targetted by this mouse position.
    if( hitChild != NULL ){
        lastClickedItem = hitChild;

        // Call the child's function.
        hitChild->LeftMouseUp( mousePos );

        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool BoxUI::RightMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
// Right mouse pushed.
// 
(
 MousePos mousePos // Mouse position.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Get the hit child
    GameUI *hitChild = GetHitChild( mousePos );

    // If a child was targetted by this mouse position.
    if( hitChild != NULL ){
                
        // Call the child's function.
        hitChild->RightMouseDown( mousePos );
    
        return true;
    }else{
        // Display help text.

        // TODO.
    }
    
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool BoxUI::RightMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
// Right mouse released.
// 
(
 MousePos mousePos // Mouse position.
)
//////////////////////////////////////////////////////////////////////////////////////////
{    
    // Get the hit child
    GameUI *hitChild = GetHitChild( mousePos );

    // If a child was targetted by this mouse position.
    if( hitChild != NULL ){
                
        // Call the child's function.
        hitChild->RightMouseUp( mousePos );

        return true;
    }

    // If this was a click.
    if( IsClick() ){
        // Display the boxe's help.
        DisplayHelp( true, mousePos.x, mousePos.y );
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool BoxUI::WheelUp
//////////////////////////////////////////////////////////////////////////////////////////
// Mouse wheel up.
// 
(
 MousePos mousePos // Mouse position.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Get the hit child
    GameUI *hitChild = GetHitChild( mousePos );

    // If a child was targetted by this mouse position.
    if( hitChild != NULL ){
        
        // Call the child's function.
        hitChild->WheelUp( mousePos );

        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool BoxUI::WheelDown
//////////////////////////////////////////////////////////////////////////////////////////
// Mouse wheel down.
// 
(
 MousePos mousePos // Mouse position.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Get the hit child
    GameUI *hitChild = GetHitChild( mousePos );

    // If a child was targetted by this mouse position.
    if( hitChild != NULL ){
        
        // Call the child's function.
        hitChild->WheelDown( mousePos );

        return true;
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool BoxUI::DragCycle
//////////////////////////////////////////////////////////////////////////////////////////
//  Left mouse released
// 
(
 MousePos mousePos // Mouse position
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Get the hit child
    GameUI *hitChild = GetHitChild( mousePos );

    if( GetTickCount() >= lastDrag ){
        // Regulate drag messages to 200ms
        lastDrag = GetTickCount() + 100; //ms

        // If a child was targetted by this mouse position.
        if( hitChild != NULL ){        
            lastClickedItem = hitChild;
        
            // Call the child's function.
            hitChild->DragCycle( mousePos );

            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool BoxUI::TextInput
//////////////////////////////////////////////////////////////////////////////////////////
// Keyboard struck
// 
(
 char ch       // The struck keyboard character.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    bool boChildHit = false;

    // Send a KbHit to all children.
    ChildQueue *childList = GetChildList();

    ChildQueue::iterator i;
    for( i = childList->begin(); i != childList->end(); i++ ){
        // If this child is not hidden.
        if( (*i)->IsShown() ){
            // Send a KbHit to this child.
            if( (*i)->TextInput( ch ) ){
                boChildHit = true;
            }
        }
    }

    return boChildHit;
}


//////////////////////////////////////////////////////////////////////////////////////////
bool BoxUI::VKeyInput
//////////////////////////////////////////////////////////////////////////////////////////
// Keyboard struck
// 
(
 VKey vKey       // The struck keyboard character.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    bool boChildHit = false;
    // Escape pressed (you can't hide the root box UI).
    if( vKey.GetKey() == VK_ESCAPE && 
        this != static_cast< GameUI *>( RootBoxUI::GetInstance() ) &&
        !IsMinimized()
        ){
        // Hide box.
        Hide();

        boChildHit = true;
    }else{
        // Send a KbHit to all children.
        ChildQueue *childList = GetChildList();

        ChildQueue::iterator i;
        for( i = childList->begin(); i != childList->end(); i++ ){
            // If this child is not hidden.
            if( (*i)->IsShown() ){
                // Send a KbHit to this child.
                if( (*i)->VKeyInput( vKey ) ){
                    boChildHit = true;
                }
            }
        }
    }

    return boChildHit;
}


//////////////////////////////////////////////////////////////////////////////////////////
void BoxUI::Hide( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Hides a box
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If there isn't any foreground child or the foreground child is this box.
    if( foregroundChild == NULL || foregroundChild == this ){
        GameUI::Hide();        


        if( foregroundChild == this ){
            DismissForegroundControl();
        }

        //RootBoxUI *rootBox = RootBoxUI::GetInstance();

		//ChatterUI::GetInstance()->Hide(); // DaP
		ChildQueue *rootChildren = this->GetChildList();// rootBox->GetChildList();
        ChildQueue::iterator i = find( rootChildren->begin(), rootChildren->end(), this );

        // If this box is a non-minimized root box child.
        if( i != rootChildren->end() && !IsMinimized() )
        {
            // Hide the side menu and chat menu.
            // The RootBoxUI will re-show them in the draw phase if needs be.
            SideMenu *sideMenu = SideMenu::GetInstance();
            ChatterUI *chatUI  = ChatterUI::GetInstance();
            MacroUI   *macroUI = MacroUI::GetInstance();
            if( chatUI != this && chatUI->IsShown() && chatUI->IsMinimized() )
            {
               chatUI->Hide();
            }
            if( sideMenu != this && sideMenu->IsShown() )
            {
                sideMenu->Hide();
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void BoxUI::Show
//////////////////////////////////////////////////////////////////////////////////////////
//  Shows a box
// 
(
 bool putForeground,bool bPlaySound
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If a maximized BoxUI pops up, reset mouse cursor.
    if( !IsMinimized() ){
        Force = 0;
    }

    if( foregroundChild == NULL ){    
        
        if( putForeground ){
            //RootBoxUI *rootBox = RootBoxUI::GetInstance(); // TODO DaP
            ChildQueue *rootChildren = this->GetChildList(); //rootBox->GetChildList();

            ChildQueue::iterator i = find( rootChildren->begin(), rootChildren->end(), this );

            // If this box was found in the root box's children.
            if( i != rootChildren->end() ){
                // Remove it
                rootChildren->erase( i );
        
                // And re-push it, this time up front.
                rootChildren->push_front( this );
            }
        }
        if(bPlaySound)
           boxShowSound->Play( TRUE );

        GameUI::Show();
    }
}

