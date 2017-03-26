#ifndef __Vircom_Sprite3D_105113070897
#define __Vircom_Sprite3D_105113070897


#define FRAMING 1
#define FRAMING32 2

struct INFO {
   int Width;
   int Height;
   int CX;
   int CY; 
	BOOL DrawOverlap;
};

#include "V2Sprite.h"
#include "BasicSprite.h"

#define ST_STANDING  1
#define ST_MOVING  2
#define ST_ATTACK  3
#define ST_ATTACK_STANDING 4

class Sprite3D : public BasicSprite {
	public:
		char NAME[1000];
      BOOL bAlreadyLoad;
		int bColorG2R[10];
      DWORD LastAccess;
      
      Sprite3D();
		virtual ~Sprite3D();

		CV2Sprite *View000, *View045, *View090, *View135, 
				 *View180;
		CV2Sprite *Att000, *Att045, *Att090, *Att135, 
				 *Att180;
		CV2Sprite *Rng000, *Rng045, *Rng090, *Rng135, 
				 *Rng180;
		CV2Sprite *StAtt000, *StAtt045, *StAtt090, *StAtt135, 
				 *StAtt180;
		CV2Sprite *StMov000, *StMov045, *StMov090, *StMov135, 
				 *StMov180;
		CV2Sprite *Corpse;

		CV2Sprite *View000Shd, *View045Shd, *View090Shd, *View135Shd, 
				 *View180Shd, *View225Shd, *View270Shd, *View315Shd;
		CV2Sprite *Att000Shd, *Att045Shd, *Att090Shd, *Att135Shd,
				 *Att180Shd, *Att225Shd, *Att270Shd, *Att315Shd;
		CV2Sprite *StAtt000Shd, *StAtt045Shd, *StAtt090Shd, *StAtt135Shd, 
				 *StAtt180Shd, *StAtt225Shd, *StAtt270Shd, *StAtt315Shd;
		CV2Sprite *StMov000Shd, *StMov045Shd, *StMov090Shd, *StMov135Shd, 
				 *StMov180Shd, *StMov225Shd, *StMov270Shd, *StMov315Shd;
//		CV2Sprite *CorpseShd;
      
      void LoadSprite3D(int, int, int, int, int, char *, int, int, int, int = 1);
	  void LoadSprite3D2(int, int, int, int, int, char *, int, int, int, int = 1);
      void LoadSprite3D(int, int, int, int, int, int, char *, int, int, int, int = 1);
	   void LoadSprite3D(int, int, int, char *, int, int, int = 1);
      void LoadSprite3D(int, char *, int = 1);
      void LoadSpriteC(int, int, int, char *, int = 1);
		void DrawSprite3D(int Direction, int Frame, int X, int Y, BYTE Stand, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *);
		void DrawSprite3DA(int Direction, int Frame, int X, int Y, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *);
		void DrawSprite3DR(int Direction, int Frame, int X, int Y, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *);
		void DrawCorpse(int, int X, int Y, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE Surface, signed char Format, int AbsX, int AbsY, TFCObject *);
		void Release(void);
      BOOL Release(DWORD);
      int  GetTextCorrection(void);
      INFO *GetCurrentVSF(int Direction, int Frame, BYTE Stand);
};

#endif