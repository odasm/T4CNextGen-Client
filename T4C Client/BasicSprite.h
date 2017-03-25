#ifndef __BASIC_SPRITE
#define __BASIC_SPRITE

#include "TFCObject.h"
#include "PowerDebug.h"

class BasicSprite {
   public:
		int MaxWidth;
		int MaxHeight;
		int MaxxCor;
		int MaxyCor;
		int fTransparent;
		int TransColor;
      signed char StAtt, StMov;
      DWORD LastAccess;

		char Type, Faces, FacesA, FacesC, FacesR, FacesStA, FacesStM;

   public:
      virtual INFO *GetCurrentVSF(int Direction, int Frame, BYTE Stand) { LOG << "* ERR = x2, 1\r\n"; throw; return NULL; };
		virtual void LoadSprite3D(int, int, int, int, int, char *, int, int, int, int = 1) { LOG << "* ERR = x2, 2\r\n"; throw; };
		virtual void LoadSprite3D2(int, int, int, int, int, char *, int, int, int, int = 1) { LOG << "* ERR = x2, 2\r\n"; throw; };
		virtual void LoadSprite3D(int, int, int, int, int, int, char *, int, int, int, int = 1) { LOG << "* ERR = x2, 2\r\n"; throw; };
	   virtual void LoadSprite3D(int, int, int, char *, int, int, int = 1) { LOG << "* ERR = x2, 3\r\n"; throw; };
      virtual void LoadSpriteC(int, int, int, char *, int = 1) { LOG << "* ERR = x2, 4\r\n"; throw; };
      virtual int  GetTextCorrection(void) { LOG << "* ERR = x2, 5\r\n"; throw; return 0; };
		virtual void DrawSprite3D(int Direction, int Frame, int X, int Y, BYTE Stand, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *) { LOG << "* ERR = x2, 6\r\n"; throw; };
      virtual void DrawSprite3DA(int Direction, int Frame, int X, int Y, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *) { LOG << "* ERR = x2, 7\r\n"; throw; };
      virtual void DrawSprite3DR(int Direction, int Frame, int X, int Y, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *) { LOG << "* ERR = x2, 7\r\n"; throw; };
		virtual void DrawCorpse(int, int X, int Y, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE Surface, signed char Format, int AbsX, int AbsY, TFCObject *) { LOG << "* ERR = x2, 8\r\n"; throw; };
      virtual void LoadBodyPart(void) { LOG << "* ERR = x2, 9\r\n"; throw; };
      virtual void LoadSpell(int, char *, int = 1) { LOG << "* ERR = x2, 10\r\n"; throw; };
      virtual BOOL Release(DWORD) { LOG << "* ERR = x2, 11\r\n"; throw; return FALSE; }; 
      virtual void Release(void) { LOG << "* ERR = x2, 12\r\n"; throw; };
      virtual void LoadSprite3D(int, char *, int = 1) { LOG << "* ERR = x2, 13\r\n"; throw; };
      virtual void SetPuppet(TFCObject *) { LOG << "* ERR = x2, 14\r\n"; throw; };
};

#endif