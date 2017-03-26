#pragma warning( disable : 4291 )

#include "pch.h"
#include <windows.h>
#include "Spell.h"
#include "Global.h"
#include "MemoryManager.h"

Spell::Spell(void) {
   bAlreadyLoad = FALSE;
   SpellAnim = 0;
}

Spell::~Spell(void) {
   Release();
}

BOOL Spell::Release(DWORD Wait) {
   if ((timeGetTime() - LastAccess) > Wait) { 
      LastAccess = -1; 
      return TRUE; 
   } 
   return FALSE;
}

void Spell::Release(void) {
   bAlreadyLoad = FALSE;
   if (SpellAnim) {
      delete [] SpellAnim;
      SpellAnim = 0;
   }
}

void Spell::LoadSpell(int f, char *Name, int p) {
   if (bAlreadyLoad)
		return;
   
   bAlreadyLoad = TRUE;

   LastAccess = timeGetTime();
   char ID[26*6+1];
   
   int ChoseX = 0;
	int ChoseY = 0;

   Faces = f;
   FacesA = 0;
   FacesC = 0;
   FacesStA = 0;
   FacesStM = 0;
   Type = 3;
   
   MaxxCor = 0;
   MaxyCor = 0;

   //Spell Frames - 32 fps?
   SpellAnim = new CV2Sprite [Faces];

   for (int i = 0; i < Faces; i++) {
	   strcpy(ID, Name);
      ID[strlen(ID)+1] = 0;
      if (i < 26) {
         ID[strlen(ID)] = i+'a';
      } else if (i < 26*2) {
         int Len = strlen(ID);
         ID[Len] = '2';
         ID[Len+1] = (i-26)+'a';
         ID[Len+2] = 0;
      } else if (i < 26*3) {
         int Len = strlen(ID);
         ID[Len] = '3';
         ID[Len+1] = (i-26*2)+'a';
         ID[Len+2] = 0;
      } else if (i < 26*4) {
         int Len = strlen(ID);
         ID[Len] = '4';
         ID[Len+1] = (i-26*3)+'a';
         ID[Len+2] = 0;
      } else if (i < 26*5) {
         int Len = strlen(ID);
         ID[Len] = '5';
         ID[Len+1] = (i-26*4)+'a';
         ID[Len+2] = 0;
      } else if (i < 26*6) {
         int Len = strlen(ID);
         ID[Len] = '6';
         ID[Len+1] = (i-26*5)+'a';
         ID[Len+2] = 0;
      }

      SpellAnim[i].CreateSprite(ID, 0, p);
		if (MaxWidth < SpellAnim[i].GetWidth()) MaxWidth = SpellAnim[i].GetWidth();
		if (MaxHeight < SpellAnim[i].GetHeight()) MaxHeight = SpellAnim[i].GetHeight();
		if (MaxxCor > SpellAnim[i].GetCX()) MaxxCor = SpellAnim[i].GetCX();
		if (MaxyCor > SpellAnim[i].GetCY()) MaxyCor = SpellAnim[i].GetCY();
		if (ChoseX < SpellAnim[i].GetCX() + SpellAnim[i].GetWidth()) ChoseX = SpellAnim[i].GetCX() + SpellAnim[i].GetWidth();
		if (ChoseY < SpellAnim[i].GetCY() + SpellAnim[i].GetHeight()) ChoseY = SpellAnim[i].GetCY() + SpellAnim[i].GetHeight();
   }

	fTransparent = SpellAnim[0].GetfTransparent();
	TransColor = SpellAnim[0].GetTransColor();
	MaxWidth += ChoseX - (MaxxCor + MaxWidth);
	MaxHeight += ChoseY - (MaxyCor + MaxHeight);
   if (MaxWidth > 60000 || MaxHeight > 60000)
      throw;
}

void Spell::DrawSprite3D(int Direction, int Frame, int X, int Y, BYTE Stand, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *Object) {
   LastAccess = timeGetTime();
   V2SPRITEFX FX;
   ZeroMemory(&FX, sizeof(FX));
   FX.lpClipRect = Clip;
   FX.lpDirectDrawSurface = Surface;
   SpellAnim[Frame].DrawSpriteN(X, Y, &FX);
}
