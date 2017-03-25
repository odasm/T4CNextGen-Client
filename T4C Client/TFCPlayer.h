// TFCPlayer.h: interface for the TFCPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TFCPLAYER_H__08FC6543_CFEA_11D0_BEA9_444553540000__INCLUDED_)
#define AFX_TFCPLAYER_H__08FC6543_CFEA_11D0_BEA9_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define HUMAN_SWORDMAN  10001
#define ELF_MAGICIAN    10002
#define DWARF_CLERIC    10003
#define DROW_THIEF      10004

#include "TemplateList.h"
#include "TFCObject.h"
#include "IDirectXSprite.h"
#include "IDirectXImage.h"
#include <string>
#include "V2Sprite.h"

#define	body			0
#define	feet			1
#define	gloves			2
#define	helm			3
#define	legs			4
#define	rings			5
#define	bracelets		6
#define	necklace		7
#define	weapon_right	8
#define	weapon_left		9
#define	two_hands		10
#define	ring1			11
#define	ring2			12
#define	weapon			13
#define	belt			14
#define	sleeves			15

//////////////////////////////////////////////////////////////////////////////////////////
// Define the bag item structure global
class GameUI;
typedef struct _BAG_ITEM
{
    _BAG_ITEM()
    {
        nX = nY = 0;
        dwID = 0;
        Type = 0;
        //strcpy( (char*)chName, "empty" );
        memset(chName,0x00,512);
        lpSprite = NULL;
        associatedUI = NULL;
    }
    ~_BAG_ITEM()
    {
    }
    
    int		nX;			// X pos in bag <pixels>
	int		nY;			// Y pos in bag <pixels>
	DWORD	dwID;		// Unique ID of item.
    WORD    Type;       // Appearance
    WORD    wBaseID;    // Base reference ID.
    DWORD   dwQty;
    int     charges;

    GameUI  *associatedUI;

	unsigned char chName[512];		// Name of the item.
   unsigned char chName22[2048];		// Name of the item.
	CV2Sprite *lpSprite;		// Pointer to sprite assigned to the bag item.

} BAG_ITEM, *LPBAG_ITEM;

typedef struct _USER_SKILL{
	unsigned char *bName;			// Name of skill.
    std::string      desc;
	WORD		wID;			// ID of skill.
	DWORD		dwStrength;		// Strength of skill.
    DWORD       dwTrueStrength; // True strength of skill.
	BYTE		bUse;			// Use of skill.
} USER_SKILL, *LPUSER_SKILL;

#define SPELL_TARGET_NONE				0x01
#define SPELL_TARGET_LOCATION			0x02
#define SPELL_TARGET_UNIT				0x04

#define SPELL_TYPE_MAGE		0x01
#define SPELL_TYPE_PRIEST	0x02
#define SPELL_TYPE_FIX_MANA	0x80

typedef struct _USER_SPELL{
	unsigned char *bName;
	WORD wID;
	WORD wSkillPnts;
	BYTE bSpellType;
	BYTE bSpellTarget;
	WORD wManaCost;
	BOOL UnGrid;
} USER_SPELL, *LPUSER_SPELL;

struct WorldElements 
{
	unsigned long Earth;
	unsigned long Fire;
	unsigned long Water;
	unsigned long Air;
	unsigned long Dark;
	unsigned long Light;
};

class TFCPlayer  
{
public:
	TFCPlayer();
	virtual ~TFCPlayer();
	
	char ViewObjectFlag;
	char ViewObjectFlag2;
	//IDirectXSprite Fleche;
	unsigned long ID;

	void ScreenPosToWL( LPPOINT lppScreenPos, LPPOINT lppWorldPos );

	WORD Version;
   signed short xPos, yPos;
	char  Sexx;
	unsigned short Object[36];
	char LastObject;	
	char Name[256];
	char Account[256];
	char Password[256];
	short World;
	bool Dead;
   int AttackMode;
	int SetGridMode;
   DWORD LastAttack;
   char Light;
   char szLanguage[256];
   bool rangedAttack;
   bool isSeraph;

	// Stats
	unsigned long Str, End, Int, Wil, Wis, Cha, Lck, Agi, Level, Gold, AC;
	int bStr, bEnd, bInt, bWil, bWis, bCha, bLck, bAgi, bAC;
    unsigned long TrueMaxHp;
	__int64 Exp, Exp2Go, ExpLastLevel;
    short StatsPts;
    short SkillPts;
	unsigned long Hp, Mana, MaxHp, MaxMana, Faith, MaxFaith;

	// by Asteryth for Pvp Ranking
	long TotalDeathNumber;
	long TotalKillNumber;
	long CurrentDeathNumber;
	long CurrentKillNumber;
	long BestSerieMurder;
	long CurrentSerieMurder;
	long CurrentPvpPoint;

    WORD Weight;
    WORD MaxWeight;

	WorldElements Power;
	WorldElements Resist;

    signed short Karma;
	// by Asteryth : Not use anymore...
    //signed short Crime;
    //signed short Honor;

	// Skills

	TemplateList <BAG_ITEM> tlBackpack;
	TemplateList <USER_SKILL> tlUserSkill;

	BAG_ITEM *lpbEquipped[ 16 ];

	void Lock( void );
	void Unlock( void );

	TemplateList< USER_SPELL > tlsSpellList;

    bool CanRunScripts;
    bool CanSlayUsers;

private:
	CRITICAL_SECTION csThreadLock;
};

extern TFCPlayer Player;

#endif // !defined(AFX_TFCPLAYER_H__08FC6543_CFEA_11D0_BEA9_444553540000__INCLUDED_)