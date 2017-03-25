// VisualObjectList.h: interface for the VisualObjectList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISUALOBJECTLIST_H__003154E2_FA13_11D0_BEAA_444553540000__INCLUDED_)
#define AFX_VISUALOBJECTLIST_H__003154E2_FA13_11D0_BEAA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "TemplateList.h"
#include "Icon3D.h"
#include "V2Sprite.h"
#include "TFCObject.h"
#include "Apparence.h"
#include "cs.h"
#include "BasicSprite.h"
#include "GameMusic.h"
#include "LightMap.h"
#include "Tileset.h"



class BACK {
public:
	int nbObjects;
	int Type;
	int ID;
	int Def;
	int Time;
	char Name[100];
};

#define LOCK_OBJECTS(s) Objects.Lock(s);

//NMNMNM
//#define StandardSprite(obj) (obj ? &Objects.VObject[obj] : &Objects.VObject[274])
#define StandardSprite(obj) (obj ? GetVSFObject(obj) : GetVSFObject(274)])
#define UNLOCK_OBJECTS(s) Objects.Unlock(s);

#define NBBASICSPRITE 250

struct FXType {
   DWORD Time;
   T3VSBSound Sound;
	BOOL Debug;
};

class VisualObjectList : public TemplateList<TFCObject> 
{

protected:
   CV2Sprite       VObject[__OBJGROUP_LASTGROUP+__OBJANIM_LASTGROUP];
   CV2Sprite       m_v2SpriteLifeS[51];
   CV2Sprite       m_v2SpriteLifeSB[51];
   CV2Sprite       m_v2SpritePShadow;

public:
 
	LightMap lmPlayerLight;
   LightMap lmOtherPlayerLight;
   LightMap lmTorchLight;
   LightMap lmTorchLanterne1;
   LightMap lmTorchPoutre;
   LightMap lmTorchChandelle2;
      
   
   CV2Sprite*       GetVSFObject(int dwIndex);
	BasicSprite *VObject3D[NBBASICSPRITE];
	bool      Object3DLoad[NBBASICSPRITE];
   WORD      Object3DSound[NBBASICSPRITE][11];
	bool      DoUnlock;
	TemplateObject<class TFCObject> * SavePtr;
   ListStatus SaveQuery;
   TemplateList <struct FXType> SoundFXList;
   CriticalSection csLock;
   BOOL NoMove;

   short SDObjNb;
   short SDID;
   short SDObjX[1500];
   short SDObjY[1500];
   char  SDObjW[1500];
   short SDObjType[1500];
   short SDObjID[1500];
   char  SDObjLight[1500];



   TFCObject MainObject;
   BOOL      m_bAskForSkinChange;
   UINT      m_dwNewSkin;
   
	
	signed   char  Direction2;
	int            Set;
	short          nbUnit;
	unsigned char  ViewWidth, ViewHeight;
	unsigned char  RangeWidth, RangeHeight;

	unsigned long LastAttempt;
   unsigned long LastAttemptIDAll;


   short m_animObjNbr;
   short m_animObjID;

   typedef struct _sanimObj
   {
      short SDObjX;
      short SDObjY;
      char  SDObjW;
      short SDObjType;
      short SDObjID;
      char  SDObjLight;
   }sanimObj;
   sanimObj m_sAnimObj[1000];

   typedef struct _sAnimOverlap
   {
      UINT  uiID;
      UINT  uiIDSprite;
      short ObjX;
      short ObjY;
      RECT  rcClip;
   }sAnimOverlap;

   std::vector< sAnimOverlap > m_sAnimOverlap;





public:
   void SDAnimClear(void);
   void SDAnimAddObject(short,short,short,char,short);
   void ClearAnimID(UINT id);

   void AddOverlapAnim(UINT,UINT,short,short,RECT);
   int  FoundAnimListID(UINT dwID);

public:
	VisualObjectList();
	virtual ~VisualObjectList();
    
    TFCObject *GetMainObject();
	void Add(unsigned long  ID, unsigned short Type, unsigned short AbsX, unsigned short AbsY, signed char Light, unsigned char Friend, unsigned long AttachID, char pHp, char defaultDirection = 2, bool allowOutOfBound = false, long spellEffectID = 0, long spellChildID = 0 );
   void MovePlayer(signed char X, signed char Y);
	void DrawObject    (int XCor, int YCor, bool Draw, LPDIRECTDRAWSURFACE, bool Flag, bool, bool);
   void DrawObjectSelect(int XCor, int YCor, bool Draw, LPDIRECTDRAWSURFACE lpBlitSurface, bool Flag, bool bDrawText, bool bNewInt);
   void DrawObjectShadow(int XCor, int YCor, bool Draw, LPDIRECTDRAWSURFACE lpBlitSurface, bool Flag, bool bDrawText, bool bNewInt);
   void DrawObjectAnim(int XCor, int YCor, bool Draw, LPDIRECTDRAWSURFACE, bool Flag, bool, bool); 
   void DrawObjectAnimOverLap(int XCor, int YCor,LPDIRECTDRAWSURFACE) ;
   void DrawObjectAnimOverLapID(int ID,bool bOffset,int XCor, int YCor,LPDIRECTDRAWSURFACE) ;
	void Create(void);
	unsigned short Found(signed char X, signed char Y);
	unsigned long FoundID2(signed char X, signed char Y);
	void SetView(int ViewWidth, int ViewHeight);
	void SetRange(int ViewWidth, int ViewHeight);
	void MoveObject(unsigned long, signed long, signed long, BOOL, unsigned long, char, unsigned short, signed char, unsigned char, bool = false);
	void Delete(unsigned long ID);
   void DeleteType(unsigned long ID);
	int  ChangeType(unsigned long ID, unsigned short Type);
	int  ChangePlayerType(unsigned short Type);
   void IsNeedChangeSkin();
	int  Modify(signed char X, signed char Y);
	void Text(unsigned long, char *, unsigned int, unsigned char);
	void Anim(unsigned long, char);
	int  RealPos(signed char X, signed char Y, signed char *NX, signed char *NY);
	void SetGrid(TFCObject *);
	void Sort(void);
	void SetSet(BOOL);
	BOOL Found(signed char *xDif, signed char *yDif, unsigned long FollowID);
	BOOL FoundValidID(signed char *xDif, signed char *yDif, unsigned long FollowID);
	int  SetDirection(unsigned long, unsigned long, bool = false);
	void LoadObject(int);
	void Release(void);
	void CreatePlayer(void);
	void SetTotalGrid(void);
	void MoveObject2(Deplacement *, BOOL = FALSE);
	void SetExtended(signed char X, signed char Y);
	int  ChkText(void);
	void SetAttack(unsigned long, unsigned long, char, char, char, DWORD forcedDirection = 0 );
	void SetOffset(unsigned long, short);
	void SetName(unsigned long, char *, BOOL DisplayNow, DWORD color = RGB(255,0,0) );
	void SetGuildName(unsigned long, char *, DWORD color = RGB( 0, 129, 192 ));//BLBLBL

	T3VSBSound *GetSound(unsigned long);
	void Identify(unsigned long ID);
   void IdentifyAll();
	void NoIdentify(unsigned long ID);
//	void Definition(BACK *, int, int);
	void OrderObject(void);
   void DeleteAll(void);
   void DrawOverlap(int XCor, int YCor, bool Draw, LPDIRECTDRAWSURFACE, bool Flag, LPRECT Clip);
   void SavePointer(void);
   void RestorePointer(void);
   void AttackMode(bool = false);
   void SetGridMode(int);
   void PlAttack(DWORD, char);
   BOOL FoundID(unsigned long, bool = false);
   BOOL FoundChild(unsigned long);
	Sprite Overlap;
   void Lock(int);
   void Unlock(int);
   void AddSDObject(void);
   void AddSDAnimObject(void);
   unsigned long FoundType(short);
   void UpdateLight(BOOL = FALSE, int = 100);
   void Dither(void);
   void SetMonsterStats(int, TFCObject *);
   void ReleaseAll(void);
   void SetPuppet(int);
	void SetPuppet(DWORD, WORD, WORD, WORD, WORD, WORD, WORD, WORD, WORD);
	void SetUnit(DWORD, short, char, char, char);
	void SetHP(DWORD, char);
	BOOL CheckDead(DWORD);
	BOOL CheckDelete(DWORD);
	BOOL SetMissing(DWORD, BOOL);
	WORD GetType(DWORD ID);
   char *GetName(DWORD ID);
   void SDClear(void);
   void SDAddObject(short,short,short,char,short);
   void KillObject(DWORD);
   void DrawPuppet(int xPos, int yPos,int dwDirection);
   void SetEvil(unsigned long);
};

extern VisualObjectList Objects;

#endif // !defined(AFX_VISUALOBJECTLIST_H__003154E2_FA13_11D0_BEAA_444553540000__INCLUDED_)