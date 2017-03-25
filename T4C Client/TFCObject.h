// TFCObject.h: interface for the TFCObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TFCOBJECT_H__A3C19A62_D282_11D0_BEA9_444553540000__INCLUDED_)
#define AFX_TFCOBJECT_H__A3C19A62_D282_11D0_BEA9_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Disp.h"
#include "Sprite.h"
#include "TemplateQueues.h"
#include "GameMusic.h"

struct Deplacement {
	unsigned long ID;
	signed long NewX;
	signed long NewY;
	char Type;
	char AnimType;
	char Direction;
	bool Miss;
	T3VSBSound *Victim;
	T3VSBSound *Parry;
};

class Sprite3D;
class TextObject;

class TFCObject  
{
public:
	TFCObject();
	virtual ~TFCObject();

	signed char OX;
	signed char OY;
	signed char OC;
	unsigned short AbsX;
	unsigned short AbsY;
	unsigned short Type;
	unsigned long  ID;
	unsigned long  NewID;
	unsigned char  SpriteNumber;
	signed   char  SpriteMove;
	unsigned char  Speed;
	unsigned char  AttFrame;
	signed short   MovX;       
	signed short   MovY;       
   float    fMovX;
   float    fMovY;
	float    SpeedX;     
	float    SpeedY;     
	unsigned char  Direction;
	char           Chose;
	signed char    Movement;
	unsigned long  Faces;
	unsigned char  AttFaces;
   char CorpseFaces;
	int MaxWidth;
	int MaxHeight;
	int MaxxCor;
	int MaxyCor;
	BOOL fTransparent;
	int TransColor;
	Sprite Overlap;		
	char MouseCursor;
	bool Extended;
	DWORD Temp;
	DWORD IdentifyTemp;
	TemplateQueue<Deplacement> MovingQueue;
	char Slow;
	char nbSprite4Move;
	char nbSprite4Att;
	unsigned char AttSpeed;
	char Flush;
	char Up;
   char Down;
   char Left;
   char Right;
	char *Name;
	DWORD NameColor;
	char *GuildName;//BLBLBL
	DWORD GuildColor;
	short TextCorrection;
	T3VSBSound onAttack[4];
	T3VSBSound onAttacked[3];
	T3VSBSound onUse;
	T3VSBSound onKilled[2];
	T3VSBSound onParry;
	bool  Killed;
	short KillType;
	DWORD KillTimer;
	signed char Format;
   bool Dynamic;
   BYTE Standing;
   signed char Light;
   BYTE Anim;
   signed int Type3D;
   char pHp;
   char CurrentCorpseFrame;
   char FacesStA;
   char FacesStM;
   BOOL NoMove;
   BOOL DeleteMe;
   char Friendly;
	BOOL Parry;
   unsigned long AttachID;
   BYTE Count;
	signed char *pOX;
	signed char *pOY;
	signed short   *pMovX; 
	signed short   *pMovY; 
	Sprite *pOverlap;
   unsigned long SummonID;
   BYTE PuppetInfo[32];
   BYTE OldPuppetInfo[32];
   DWORD VisiblePart;
   BYTE nbBodyPart;
   Sprite3D *BodyPart[32];
	BOOL DrawOverlap;
	BOOL KnownPuppet;
   BOOL bInvisible;
   bool allowOutOfBound;
   long spellEffectID;
   long spellChildID;

   bool operator < (const TFCObject *&x );

   void DisplayName();
   void DrawName( int x, int y );
   void StopNameDisplay();
   bool IsNameDrawn();


   void SetTalkText( const char *txt, COLORREF textColor );
   void DrawTalkText( int x, int y );
   bool IsTalkTextDrawn();
   void StopTalkText();
   LPCTSTR GetTalkText();
   void SetTalkOffset( DWORD offset );

private:
    void InitTalkText( COLORREF textColor );
    void InitNameText();
    void InitGuildNameText();//BLBLBL

    TextObject *m_TalkingText;
    TextObject *m_DisplayName;
	TextObject *m_DisplayGuildName;//BLBLBL
};

#endif // !defined(AFX_TFCOBJECT_H__A3C19A62_D282_11D0_BEA9_444553540000__INCLUDED_)










