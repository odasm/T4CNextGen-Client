// GameIcons.h: interface for the GameIcons class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEICONS_H__622A0EA5_D8EC_11D3_8614_00E02922FA40__INCLUDED_)
#define AFX_GAMEICONS_H__622A0EA5_D8EC_11D3_8614_00E02922FA40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "V2Sprite.h"
#include "NewSound.h"
#include <map>
#include <string>

class GameIcons  
{
public:
	GameIcons();
	virtual ~GameIcons();

    LPV2Sprite operator()( DWORD id );

    // Binds a sprite Id to an ID.
    void BindSprite( std::string spriteId, DWORD id, int pal = 1 );

    bool Exists( DWORD spriteId );
private:
    typedef std::map< DWORD, LPV2Sprite > IconMap;

    IconMap iconMap;
};

class GameSounds
{
public:
	GameSounds();
	virtual ~GameSounds();

    T3VSBSound *operator()( DWORD id );

    // Binds a sprite Id to an ID.
    void BindSound( std::string spriteId, DWORD id );
private:
    typedef std::map< DWORD, T3VSBSound * > SoundMap;

    SoundMap soundMap;
};

extern GameIcons ItemIcons;
extern GameIcons SkillIcons;
extern GameIcons SpellIcons;
extern GameSounds ItemDragSounds;
extern GameIcons InvItemIcons;

#endif // !defined(AFX_GAMEICONS_H__622A0EA5_D8EC_11D3_8614_00E02922FA40__INCLUDED_)
