// GameIcons.cpp: implementation of the GameIcons class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning(disable:4786)
#include "pch.h"
#include "GameIcons.h"
#include "VisualObjectList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GameIcons ItemIcons;
GameIcons SkillIcons;
GameIcons SpellIcons;
GameSounds ItemDragSounds;
GameIcons InvItemIcons;

GameIcons::GameIcons()
{

}

GameIcons::~GameIcons()
{
    std::map< DWORD, LPV2Sprite >::iterator i;

    // Flush all Sprites.
    for( i = iconMap.begin(); i != iconMap.end(); i++ )
    {
        delete (*i).second;
        (*i).second = NULL;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
void GameIcons::BindSprite
//////////////////////////////////////////////////////////////////////////////////////////
//  Binds an id to a sprite id.
// 
(
 std::string spriteId,  // The sprite Id.
 DWORD dwId,             // The internal numeric Id.
 int pal
)
//////////////////////////////////////////////////////////////////////////////////////////
{        
    LPV2Sprite vsf = new CV2Sprite;

    vsf->LoadSprite( spriteId.c_str(), 0, pal );
    
    // Insert the CV2Sprite
    std::pair< IconMap::iterator, bool > i = iconMap.insert( 
        IconMap::value_type( dwId, vsf ) 
    );

    // If the insert failed
    if( !i.second ){
        delete vsf;
    }

}

class ErrSprite{
public:
    ErrSprite(){
        sprite.LoadSprite( "???" );
    }
    CV2Sprite sprite;
};

//////////////////////////////////////////////////////////////////////////////////////////
LPV2Sprite GameIcons::operator()
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the CV2Sprite binded to a particular ID, or the default standard sprite
//  if none were found.
// 
(
 DWORD dwId
)
// Return: LPVSF, A valid CV2Sprite (function cannot fail).
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Find the icon in the game icons.
    IconMap::iterator i = iconMap.find( dwId );

    // If the icon was found
    if( i != iconMap.end() ){
        // return its CV2Sprite.
        return (*i).second;
    }

    static ErrSprite err;

    // Otherwise return the default invalid sprite.
    return &err.sprite;
}

GameSounds::GameSounds()
{

}

GameSounds::~GameSounds()
{
    SoundMap::iterator i;

    // Flush all VSFs.
    for( i = soundMap.begin(); i != soundMap.end(); i++ ){
        delete (*i).second;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
void GameSounds::BindSound
//////////////////////////////////////////////////////////////////////////////////////////
//  Binds an id to a sprite id.
// 
(
 std::string soundId,  // The sprite Id.
 DWORD dwId             // The internal numeric Id.
)
//////////////////////////////////////////////////////////////////////////////////////////
{        
    T3VSBSound *vsb = new T3VSBSound;

    vsb->Create( const_cast< char * >( soundId.c_str() ), TS_MEMORY );
    
    // Insert the CV2Sprite
    std::pair< SoundMap::iterator, bool > i = soundMap.insert( 
        SoundMap::value_type( dwId, vsb ) 
    );

    // If the insert failed
    if( !i.second ){
        delete vsb;
    }

}

class ErrSound{
public:
    ErrSound(){
        sound.Create( "Generic Drop Item", TS_MEMORY );
    }
    T3VSBSound sound;
};

//////////////////////////////////////////////////////////////////////////////////////////
T3VSBSound *GameSounds::operator()
//////////////////////////////////////////////////////////////////////////////////////////
//  Returns the CV2Sprite binded to a particular ID, or the default standard sprite
//  if none were found.
// 
(
 DWORD dwId
)
// Return: LPVSF, A valid CV2Sprite (function cannot fail).
//////////////////////////////////////////////////////////////////////////////////////////
{
    // Find the icon in the game icons.
    SoundMap::iterator i = soundMap.find( dwId );

    // If the icon was found
    if( i != soundMap.end() ){
        // return its CV2Sprite.
        return (*i).second;
    }

    static ErrSound err;

    // Otherwise return the default invalid sprite.
    return &err.sound;
}
//////////////////////////////////////////////////////////////////////////////////////////
bool GameIcons::Exists
//////////////////////////////////////////////////////////////////////////////////////////
// Returns true if the given Id has an icon.
// 
(
 DWORD dwId  // The item Id.
)
// Return: bool, true if the item has a binded icon.
//////////////////////////////////////////////////////////////////////////////////////////
{
    IconMap::iterator i = iconMap.find( dwId );

    // If the icon was found
    if( i != iconMap.end() ){
        // return its CV2Sprite.
        return true;
    }

    return false;
}