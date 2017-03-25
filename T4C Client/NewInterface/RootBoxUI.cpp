// RootBoxUI.cpp
//
#include "../pch.h"
#include "RootBoxUI.h"
#include <mmsystem.h>
#include "../MemoryManager.h"

#include "ChatterUI.h"
#include "SideMenu.h"
#include "MacroUI.h"
#include "RTMap.h"
#include "RTHelp.h"
#include "RTItemI.h"

#include "..\SaveGame.h"
extern CSaveGame g_SaveGame;


RootBoxUI::RootBoxUI() : menuShown( false ), chatShown( false ){
    // Always show the root box IU.
    Show();
}


//////////////////////////////////////////////////////////////////////////////////////////
RootBoxUI *RootBoxUI::GetInstance( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the sole RootBoxUI instance.
//////////////////////////////////////////////////////////////////////////////////////////
{
    static RootBoxUI rootBox;

    return &rootBox;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::LeftMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
//  Left mouse down. Thread safe.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );
   
    LOG.Lock();

    LOG << "\r\nLast click time: " << lastClickTime << " current time: " << timeGetTime();
    
    // If this click happens shorter than the click-aware time.
    if( timeGetTime() - lastClickTime <= ClickTime ){
        if( currentMouseState == MouseClick ){
            LOG << "\r\nFOUND DOUBLE CLICK";

            currentMouseState = MouseDblClick;
        }
    }else{
        LOG << "\r\nFOUND UNRELATED MOUSE DOWN";
        currentMouseState = MouseNone;        
    }

    LOG.Unlock();

    SideMenu *sideMenu = SideMenu::GetInstance();
    if( sideMenu->IsShown() && sideMenu->GetPos().MousePosInFrame( mousePos ) )
    {
        sideMenu->LeftMouseDown( mousePos );
    }

    leftMouseDown = true;
    
    lastClickTime = timeGetTime();

    if( BoxUI::LeftMouseDown( mousePos ) )
    {
        mouseOwned = true;
        return true;
    }
    return false;
}
//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::LeftMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Left mouse up. Thread safe.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );

    LOG.Lock();
    if( timeGetTime() - lastClickTime <= ClickTime ){
        if( currentMouseState == MouseNone ){
            LOG << "\r\nFOUND CLICK";

            currentMouseState = MouseClick;
        }
    }else{
        LOG << "\r\nFOUND UNRELATED MOUSE UP";
        currentMouseState = MouseNone;
    }
    LOG.Unlock();

    SideMenu *sideMenu = SideMenu::GetInstance();
    if( sideMenu->IsShown() && sideMenu->GetPos().MousePosInFrame( mousePos ) ){
        sideMenu->LeftMouseDown( mousePos );
    }

    leftMouseDown = false;

    if( BoxUI::LeftMouseUp( mousePos ) ){
        mouseOwned = true;
        return true;
    }
    return false;

}
//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::RightMouseDown
//////////////////////////////////////////////////////////////////////////////////////////
// Right mouse down. Thread safe.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );

    LOG.Lock();

    LOG << "\r\nLast click time: " << lastClickTime << " current time: " << timeGetTime();
    
    // If this click happens shorter than the click-aware time.
    if( timeGetTime() - lastClickTime <= ClickTime ){
        if( currentMouseState == MouseClick ){
            LOG << "\r\nFOUND DOUBLE CLICK";

            currentMouseState = MouseDblClick;
        }
    }else{
        LOG << "\r\nFOUND UNRELATED MOUSE DOWN";
        currentMouseState = MouseNone;        
    }

    LOG.Unlock();

    SideMenu *sideMenu = SideMenu::GetInstance();
    if( sideMenu->IsShown() && sideMenu->GetPos().MousePosInFrame( mousePos ) ){
        sideMenu->LeftMouseDown( mousePos );
    }

    lastClickTime = timeGetTime();

    return BoxUI::RightMouseDown( mousePos );
}
//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::RightMouseUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Right mouse up. Thread safe.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );

    LOG.Lock();
    if( timeGetTime() - lastClickTime <= ClickTime ){
        if( currentMouseState == MouseNone ){
            LOG << "\r\nFOUND CLICK";

            currentMouseState = MouseClick;
        }
    }else{
        LOG << "\r\nFOUND UNRELATED MOUSE UP";
        currentMouseState = MouseNone;
    }
    LOG.Unlock();

    SideMenu *sideMenu = SideMenu::GetInstance();
    if( sideMenu->IsShown() && sideMenu->GetPos().MousePosInFrame( mousePos ) ){
        sideMenu->LeftMouseDown( mousePos );
    }

    return BoxUI::RightMouseUp( mousePos );
}

//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::DragCycle
//////////////////////////////////////////////////////////////////////////////////////////
//  Drag cycle, thread safe.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );

    return BoxUI::DragCycle( mousePos );
}

//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::WheelUp
//////////////////////////////////////////////////////////////////////////////////////////
//  Wheel up. Thread safe.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );

    return BoxUI::WheelUp( mousePos );
}
//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::WheelDown
//////////////////////////////////////////////////////////////////////////////////////////
//  Wheel down. Thread safe.
// 
(
 MousePos mousePos
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );

    return BoxUI::WheelDown( mousePos );
}
//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::TextInput
//////////////////////////////////////////////////////////////////////////////////////////
//  KbHit, thread safe.
// 
(
 char ch
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );

    if( foregroundChild != NULL ){
        // Send the keyboard event to it.
        foregroundChild->TextInput( ch );        
        return true;
    }

    return BoxUI::TextInput( ch );
}    

void RootBoxUI::SetSideMenuState( void ) 
{
   MacroUI   *macroUI  = MacroUI::GetInstance();
   ChatterUI *chatUI   = ChatterUI::GetInstance();
   SideMenu  *sideMenu = SideMenu::GetInstance();
   
   switch( g_SaveGame.GetOptionsParam()->dwMenuState ) 
   {
   case MENU_NONE:
      macroUI->Minimize(true);
      macroUI->Show();
      chatUI->Hide();
      sideMenu->Hide();
      menuShown = false;
      chatShown = false;
      break;
   case MENU_BOTH:
      chatUI->Minimize( true );
      chatUI->Show();
      sideMenu->Show();
      menuShown = true;
      chatShown = true;
      break;
   case MENU_CHAT:
      chatUI->Minimize( true );
      chatUI->Show();
      sideMenu->Hide();
      chatShown = true;
      menuShown = false;
      break;
   case MENU_TMI:
      macroUI->Minimize(true);
      macroUI->Show();
      chatUI->Minimize( true );
      chatUI->Hide();
      sideMenu->Show();
      chatShown = false;
      menuShown = true;
      break;
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::VKeyInput(VKey vKey)
{
   CAutoLock autoLock( &threadLock );
   
   bool sendChar = true;
   
   // If there is a defined foreground child.
   if( foregroundChild != NULL )
   {
      // Send the keyboard event to it.
      foregroundChild->VKeyInput( vKey );
      return true;
   }
   
   
   if ( vKey.CTRL() == true && (vKey.GetKey() == VK_UP || vKey.GetKey() == VK_DOWN) ) 
   {
      if ( ChatterUI::GetInstance()->VKeyInput( vKey ) ) 
         return true;
   } 
   else if( vKey.GetKey() == VK_ESCAPE )
   {
      ChatterUI *chatUI   = ChatterUI::GetInstance();
      SideMenu  *sideMenu = SideMenu::GetInstance();
      MacroUI   *macroUI  = MacroUI::GetInstance();
      
      bool toggleMenu = true;
      ChildQueue::iterator i;
      for( i = GetChildList()->begin(); i != GetChildList()->end(); i++ )
      {
         // If any maximized UI not chatUI or sideMenu.
         if( (*i)->IsShown() && !(*i)->IsMinimized() && (*i) != chatUI && (*i) != sideMenu )
         {
            toggleMenu = false;
            
            chatUI->Hide();
            sideMenu->Hide();
            if(g_SaveGame.GetOptionsParam()->dwMenuState == MENU_TMI || g_SaveGame.GetOptionsParam()->dwMenuState == MENU_NONE)
            {
               macroUI->Minimize(true);
               macroUI->Show();
            }
            break;
         }
      }
      if( toggleMenu )
      {
         sendChar = false;

         if(DISABLE_TMI_MAP == 0)
         {
            // If both the chatUI and menu are shown.
            if( chatUI->IsShown() && chatUI->IsMinimized() && sideMenu->IsShown())
            {                
               // Dismiss the side menu.                
               g_SaveGame.GetOptionsParam()->dwMenuState = MENU_CHAT;
               sideMenu->Hide();
               menuShown = false;
               macroUI->Minimize(false);
               macroUI->Hide();
            }
            else if( chatUI->IsShown() && chatUI->IsMinimized() && !sideMenu->IsShown())
            {
               // Dismiss the chat menu.
               g_SaveGame.GetOptionsParam()->dwMenuState = MENU_TMI;
               chatUI->Hide();
               chatShown = false;
               sideMenu->Show();
               menuShown = true;
               macroUI->Minimize(true);
               macroUI->Show();
            }
            else if( !chatUI->IsShown() && !sideMenu->IsShown())
            {
               // Popup the menu and the chat box.
               g_SaveGame.GetOptionsParam()->dwMenuState = MENU_BOTH;
               chatUI->Minimize( true );
               chatUI->Show( false );
               sideMenu->Show( false );
               chatShown = menuShown = true;
               macroUI->Minimize(false);
               macroUI->Hide();
            }
            else if( !chatUI->IsShown() && sideMenu->IsShown())
            {
               // Dismiss the chat menu.
               g_SaveGame.GetOptionsParam()->dwMenuState = MENU_NONE;
               sideMenu->Hide();
               menuShown = false;
               macroUI->Minimize(true);
               macroUI->Show();
            }
            else
            {
               sendChar = true;
            }
         }
         else
         {
            if(sideMenu->IsShown())
            {
               sideMenu->Hide();
               menuShown = false;
            }

            // If both the chatUI and menu are shown.
            if( !chatUI->IsShown())
            {
               // Popup the menu and the chat box.
               g_SaveGame.GetOptionsParam()->dwMenuState = MENU_CHAT;
               chatUI->Minimize( true );
               chatUI->Show( false );
               sideMenu->Show( false );
               chatShown =  true;
               macroUI->Minimize(false);
               macroUI->Hide();
            }
            else if( chatUI->IsShown())
            {
               // Dismiss the chat menu.
               g_SaveGame.GetOptionsParam()->dwMenuState = MENU_NONE;
               chatUI->Hide();
               chatShown = false;
               macroUI->Minimize(true);
               macroUI->Show();
            }
            else
            {
               sendChar = true;
            }
         }
      }
   }
   else if( vKey.GetKey() == VK_TAB )
   {
      sendChar = false;
      RTMap   *pRTMap  = RTMap::GetInstance();

      if(pRTMap->IsShown())
         pRTMap->Hide();
      else
         pRTMap->Show();
   }
   else if( vKey.GetKey() == VK_F19) // steph VK_F19 au lieu de VK_F1
   {
      sendChar = false;
      RTHelp   *pRTHelp  = RTHelp::GetInstance();

      if(pRTHelp->IsShown())
         pRTHelp->Hide();
      else
         pRTHelp->Show(true,0,1);
   }

   RTItemI   *pRTItemI = RTItemI::GetInstance();
   if(g_Global.IsItemInfoAvailable())
   {
      pRTItemI->Hide();
      g_Global.ResetItemInfo();
      sendChar = false;
   }
   
   if( sendChar )
   {
      return BoxUI::VKeyInput( vKey );
   }
   return true;
}

//////////////////////////////////////////////////////////////////////////////////////////
void RootBoxUI::Draw( V2SPRITEFX *vsfFX )
//////////////////////////////////////////////////////////////////////////////////////////
//  Drawing thread safe.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{
   CAutoLock autoLock( &threadLock );
   
   if( !IsLeftMouseDown() )
   {
      mouseOwned = false;        
   }
   
   vector< GameUI * > drawList;
   vector< GameUI * > minimizedDrawList;
   
   bool fullscreenBoxAdded = false;
   bool chatUIadded = false;
   
   SideMenu  *sideMenu = SideMenu ::GetInstance();
   ChatterUI *chatUI   = ChatterUI::GetInstance();
   RTMap     *pRTMap   = RTMap    ::GetInstance();
   RTHelp    *pRTHelp  = RTHelp   ::GetInstance();
   MacroUI   *macroUI  = MacroUI  ::GetInstance();
   RTItemI   *pRTItemI = RTItemI  ::GetInstance();
   
   // Check all children to query those that need to be drawn.
   ChildQueue::iterator i;
   for( i = GetChildList()->begin(); i != GetChildList()->end(); i++ )
   {
      // chatUI.
      if( (*i) == chatUI )
      {
         // If a box was already displayed.
         if( fullscreenBoxAdded )
         {
            // Make sure the chat box is minimized.                
            if( !chatUI->IsMinimized() )
            {
               chatUI->Minimize( true );
            }
         }
         else
         {
            // If the chat UI is full screen.
            if( !chatUI->IsMinimized() && chatUI->IsShown() )
            {
               fullscreenBoxAdded = true;
               chatUIadded = true;
            }
         }
         // If the UI is to be shown.
      }
      else if( (*i)->IsShown() && (*i) != sideMenu && (*i) != pRTMap && (*i) != pRTHelp && (*i) != pRTItemI)
      {
         // If the UI is either minimized, or is the first
         // fullscreen box to be added.
         if( (*i)->IsMinimized() || ( !fullscreenBoxAdded && (*i) != foregroundChild ) )
         {
            
            // If at least one box is not minimized
            if( !(*i)->IsMinimized() )
            {
               
               fullscreenBoxAdded = true;
               
               // Add the ui to the list of UIs to draw.
               drawList.push_back( *i );
            }
            else
            {
               minimizedDrawList.push_back( *i );
            }
            
         }
         else
         {
            // Hide the box, since it won't be displayed.
            if( (*i) != foregroundChild && (*i) != sideMenu )
            {
               (*i)->Hide();
            }
         }
      }
   }

   /*

   if( fullscreenBoxAdded )
   {
      // Display chatter and menu UIs.
      if( !sideMenu->IsShown() )
      {
         sideMenu->Show( false );
      }
      if( !chatUIadded )
      {
         if( !chatUI->IsMinimized() )
            chatUI->Minimize( true );                        
         if( !chatUI->IsShown() ){
            chatUI->Show( false );
         }
      }
   }
   */
   
   // Draw everything in the minimized draw list (minimized are under fullscreen UIs).
   vector< GameUI * >::iterator j;
   for( j = minimizedDrawList.begin(); j != minimizedDrawList.end(); j++ )
   {
      (*j)->Draw( NULL );
   }
   
   // Draw everything in the draw list.
   for( j = drawList.begin(); j != drawList.end(); j++ )
   {
      (*j)->Draw( NULL );        
   }

   // validate if GUI change... if Close button are pressed,
   // we need to change GUI on good mode.. (MACRO fullscreen...)
   if(chatUI->IsShown() && macroUI->IsShown() && macroUI->IsMinimized())
   {
     macroUI->Minimize(false);
     macroUI->Hide();
   }
   else if(!chatUI->IsShown() && !macroUI->IsShown() )
   {
     macroUI->Minimize(true);
     macroUI->Show();
   }
   
   
   // If the chat or menu are currently not showing but should be shown.
   if( !chatUI->IsShown() && chatShown )
   {
      chatUI->Minimize( true );
      chatUI->Show( false );
   }
   if( !sideMenu->IsShown() && menuShown )
   {
      sideMenu->Show( false );
   }
    
   if( chatUI->IsShown() )
   {
      chatUI->Draw( NULL );
   }
   // Draw the side menu on top of everyting else.
   if( sideMenu->IsShown() )
   {
      sideMenu->Draw( NULL );
   }

   if(g_Global.IsItemInfoAvailable())
   {
      sItemInfo *pInfo = g_Global.GetItemInfo();
      if(pInfo->shAC     == 0 && pInfo->shDodge == 0 && pInfo->shEnd == 0 && pInfo->lMinDamage == 0 && pInfo->lMaxDamage == 0 && 
         pInfo->shAttack == 0 && pInfo->shStr   == 0 && pInfo->shAgi == 0 && pInfo->shWis      == 0 && pInfo->shInt      == 0 && 
         pInfo->shNbrBoust == 0)
      {
         g_Global.ResetItemInfo();
         Objects.Lock(40);
         Objects.Identify(pInfo->ulID);
         Objects.Unlock(40);
      }
      else
      {
         if(pRTItemI->IsShown() )
            pRTItemI->Draw(NULL);
         else
            pRTItemI->Show();
      }
   }
   else
   {
      if(pRTItemI->IsShown() )
         pRTItemI->Hide();

   }


   if( pRTMap->IsShown() )
   {
      pRTMap->Draw( NULL );
   }
   if( pRTHelp->IsShown() )
   {
      pRTHelp->Draw( NULL );
   }

   
   // If there is a foreground child.
   if( foregroundChild != NULL )
   {
      // Draw it
      foregroundChild->Draw( NULL );
   }
}

//////////////////////////////////////////////////////////////////////////////////////////
void RootBoxUI::DrawHelp
//////////////////////////////////////////////////////////////////////////////////////////
//  Draws the help texts
// 
(
 V2SPRITEFX *vsfFX
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );
    
    GameUI::DrawHelp( vsfFX );
}


//////////////////////////////////////////////////////////////////////////////////////////
void RootBoxUI::AddChild
//////////////////////////////////////////////////////////////////////////////////////////
//  Children adding, thread safe.
// 
(
 GameUI *child
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );

    GameUI::AddChild( child );
}

//////////////////////////////////////////////////////////////////////////////////////////
void RootBoxUI::RemoveChild
//////////////////////////////////////////////////////////////////////////////////////////
//  Children removing. Thread safe.
// 
(
 GameUI *child
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );

    GameUI::RemoveChild( child );
}
//////////////////////////////////////////////////////////////////////////////////////////
void RootBoxUI::DeleteChild
//////////////////////////////////////////////////////////////////////////////////////////
//  Children deletion. Thread safe.
// 
(
 GameUI *child
)
//////////////////////////////////////////////////////////////////////////////////////////
{
     CAutoLock autoLock( &threadLock );

     GameUI::DeleteChild( child );
}

//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::IsMouseOwned
//////////////////////////////////////////////////////////////////////////////////////////
// Determines if the mouse is owned by the UI.
// 
(
 MousePos mousePos // The mouse pos.
)
// Return: bool, true if the mouse is owned by the UI.
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );

    if( mouseOwned ){
        return true;
    }

    // If the chatterUI is minimized alone.
    if( ChatterUI::GetInstance() && ChatterUI::GetInstance()->IsShown() && ChatterUI::GetInstance()->IsMinimized() && !SideMenu::GetInstance()->IsShown() ){
        FramePos filler( 0, 367, 64, 479 );
        if( filler.MousePosInFrame( mousePos ) ){
            return true;
        }
    }

    // If a child box was hit by the mouse position.
    if( GetHitChild( mousePos ) != NULL ){
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
bool RootBoxUI::IsMaximized( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Determines whether any UIs are maximized.
// 
// Return: bool, true if any box is maximized.
//////////////////////////////////////////////////////////////////////////////////////////
{
    CAutoLock autoLock( &threadLock );
    
    // Check all children to query those that need to be drawn.
    ChildQueue::iterator i;
    for( i = GetChildList()->begin(); i != GetChildList()->end(); i++ ){
        if( (*i)->IsShown() &&
            (*i) != SideMenu::GetInstance() &&
            !(*i)->IsMinimized() &&
            (*i) != foregroundChild
         ){
            return true;
        }
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
MousePos RootBoxUI::GetRootScreenOffset( void )
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the screen offset.
//////////////////////////////////////////////////////////////////////////////////////////
{
    MousePos offset( 0, 0 );
    
    return offset;
}
