// ************************************************************************
// ***                                                                  ***
//      Project:   TFC Client
//      Creation:  29 June, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         06-29-1997      1.0     TH          Initial development
//
//      Description
//         Provide fonction to load a Sprite from a Image or an ID
// ***                                                                  ***
//	************************************************************************
// *** Copyright (c) 1997-1998 Vircom. All rights reserved.             ***
#include "pch.h"
#pragma warning( disable : 4291 )

#include <windows.h>
#include "Icon3D.h" 
#include "PowerDebug.h"
#include "Random.h" 
#include "Global.h"
#include "MemoryManager.h"
#include "V2PalManager.h"

inline int Abs(int x) {
	return ((x < 0) ? (-x) : (x));
}

Sprite3D::Sprite3D()
// Constructeur
{
   bAlreadyLoad = FALSE;
	
	LastAccess = timeGetTime();
	MaxWidth = 0;
	MaxHeight = 0;
	MaxxCor = 0;
	MaxyCor = 0;

   View000 = NULL;
	View045 = NULL;
	View090 = NULL;
	View135 = NULL;
	View180 = NULL;

   Att000 = NULL;
	Att045 = NULL;
	Att090 = NULL;
	Att135 = NULL;
	Att180 = NULL;

   Rng000 = NULL;
	Rng045 = NULL;
	Rng090 = NULL;
	Rng135 = NULL;
	Rng180 = NULL;

   StMov000 = NULL;
   StMov045 = NULL;
   StMov090 = NULL;
   StMov135 = NULL;
   StMov180 = NULL;

   StAtt000 = NULL;
   StAtt045 = NULL;
   StAtt090 = NULL;
   StAtt135 = NULL;
   StAtt180 = NULL;

   View000Shd = NULL;
   View045Shd = NULL;
   View090Shd = NULL;
   View135Shd = NULL;
   View180Shd = NULL;
   View225Shd = NULL;
   View270Shd = NULL;
   View315Shd = NULL;

   Att000Shd = NULL;
   Att045Shd = NULL;
   Att090Shd = NULL;
   Att135Shd = NULL;
   Att180Shd = NULL;
   Att225Shd = NULL;
   Att270Shd = NULL;
   Att315Shd = NULL;

   StMov000Shd = NULL;
   StMov045Shd = NULL;
   StMov090Shd = NULL;
   StMov135Shd = NULL;
   StMov180Shd = NULL;
   StMov225Shd = NULL;
   StMov270Shd = NULL;
   StMov315Shd = NULL;

   StAtt000Shd = NULL;
   StAtt045Shd = NULL;
   StAtt090Shd = NULL;
   StAtt135Shd = NULL;
   StAtt180Shd = NULL;
   StAtt225Shd = NULL;
   StAtt270Shd = NULL;
   StAtt315Shd = NULL;

   Corpse = NULL;
   StAtt = -1;
   StMov = -1;
   bColorG2R[0] = 28;
   bColorG2R[1] = 41;
   bColorG2R[2] = 40;
   bColorG2R[3] = 223;
   bColorG2R[4] = 210;
   bColorG2R[5] = 208;
   bColorG2R[6] = 97;
   bColorG2R[7] = 2;
   bColorG2R[8] = 2;
   bColorG2R[9] = 2;
}

Sprite3D::~Sprite3D()
// Destructeur
{
   Release();
}

void Sprite3D::Release(void) {
   bAlreadyLoad = FALSE;
   
   if (View000) {
      delete [] View000;
	   View000 = NULL;
   }

   if (View045) {
   	delete [] View045;
	  	View045 = NULL;
   }

   if (View090) {
   	delete [] View090;
	   View090 = NULL;
   }

   if (View135) {
	   delete [] View135;
	   View135 = NULL;
   }

   if (View180) {
	   delete [] View180;
	   View180 = NULL;
   }

   if (Att000) {
      delete [] Att000;
		Att000 = NULL;
   }

   if (Att045) {
		delete [] Att045;
		Att045 = NULL;
   }

   if (Att090) {
		delete [] Att090;
		Att090 = NULL;
   }

   if (Att135) {
		delete [] Att135;
		Att135 = NULL;
   }

   if (Att180) {
		delete [] Att180;
		Att180 = NULL;
   }

   if (Rng000) {
      delete [] Rng000;
		Rng000 = NULL;
   }

   if (Rng045) {
		delete [] Rng045;
		Rng045 = NULL;
   }

   if (Rng090) {
		delete [] Rng090;
		Rng090 = NULL;
   }

   if (Rng135) {
		delete [] Rng135;
		Rng135 = NULL;
   }

   if (Rng180) {
		delete [] Rng180;
		Rng180 = NULL;
   }

   if (Corpse) {
      delete [] Corpse;
      Corpse = NULL;
   }

   if (StMov000) {
      delete [] StMov000;
      StMov000 = NULL;
   }

   if (StMov045) {
      delete [] StMov045;
      StMov045 = NULL;
   }

   if (StMov090) {
      delete [] StMov090;
      StMov090 = NULL;
   }

   if (StMov135) {
      delete [] StMov135;
      StMov135 = NULL;
   }

   if (StMov180) {
      delete [] StMov180;
      StMov180 = NULL;
   }

   if (StAtt000) {
      delete [] StAtt000;
      StAtt000 = NULL;
   }

   if (StAtt045) {
      delete [] StAtt045;
      StAtt045 = NULL;
   }

   if (StAtt090) {
      delete [] StAtt090;
      StAtt090 = NULL;
   }

   if (StAtt135) {
      delete [] StAtt135;
      StAtt135 = NULL;
   }

   if (StAtt180) {
      delete [] StAtt180;
      StAtt180 = NULL;
   }

   if (View000Shd) {
      delete [] View000Shd;
	   View000Shd = NULL;
   }

   if (View045Shd) {
   	delete [] View045Shd;
	  	View045Shd = NULL;
   }

   if (View090Shd) {
   	delete [] View090Shd;
	   View090Shd = NULL;
   }

   if (View135Shd) {
	   delete [] View135Shd;
	   View135Shd = NULL;
   }

   if (View180Shd) {
	   delete [] View180Shd;
	   View180Shd = NULL;
   }

   if (View225Shd) {
   	delete [] View225Shd;
	   View225Shd = NULL;
   }

   if (View270Shd) {
	   delete [] View270Shd;
	   View270Shd = NULL;
   }

   if (View315Shd) {
	   delete [] View315Shd;
	   View315Shd = NULL;
   }

   if (Att000Shd) {
      delete [] Att000Shd;
		Att000Shd = NULL;
   }

   if (Att045Shd) {
		delete [] Att045Shd;
		Att045Shd = NULL;
   }

   if (Att090Shd) {
		delete [] Att090Shd;
		Att090Shd = NULL;
   }

   if (Att135Shd) {
		delete [] Att135Shd;
		Att135Shd = NULL;
   }

   if (Att180Shd) {
		delete [] Att180Shd;
		Att180Shd = NULL;
   }

   if (Att225Shd) {
		delete [] Att225Shd;
		Att225Shd = NULL;
   }

   if (Att270Shd) {
		delete [] Att270Shd;
		Att270Shd = NULL;
   }

   if (Att315Shd) {
		delete [] Att315Shd;
		Att315Shd = NULL;
   }

/*   if (CorpseShd) {
      delete [] CorpseShd;
      CorpseShd = NULL;
   } */ 

   if (StMov000Shd) {
      delete [] StMov000Shd;
      StMov000Shd = NULL;
   }

   if (StMov045Shd) {
      delete [] StMov045Shd;
      StMov045Shd = NULL;
   }

   if (StMov090Shd) {
      delete [] StMov090Shd;
      StMov090Shd = NULL;
   }

   if (StMov135Shd) {
      delete [] StMov135Shd;
      StMov135Shd = NULL;
   }

   if (StMov180Shd) {
      delete [] StMov180Shd;
      StMov180Shd = NULL;
   }

   if (StMov225Shd) {
      delete [] StMov225Shd;
      StMov225Shd = NULL;
   }

   if (StMov270Shd) {
      delete [] StMov270Shd;
      StMov270Shd = NULL;
   }

   if (StMov315Shd) {
      delete [] StMov315Shd;
      StMov315Shd = NULL;
   }

   if (StAtt000Shd) {
      delete [] StAtt000Shd;
      StAtt000Shd = NULL;
   }

   if (StAtt045Shd) {
      delete [] StAtt045Shd;
      StAtt045Shd = NULL;
   }

   if (StAtt090Shd) {
      delete [] StAtt090Shd;
      StAtt090Shd = NULL;
   }

   if (StAtt135Shd) {
      delete [] StAtt135Shd;
      StAtt135Shd = NULL;
   }

   if (StAtt180Shd) {
      delete [] StAtt180Shd;
      StAtt180Shd = NULL;
   }

   if (StAtt225Shd) {
      delete [] StAtt225Shd;
      StAtt225Shd = NULL;
   }

   if (StAtt270Shd) {
      delete [] StAtt270Shd;
      StAtt270Shd = NULL;
   }

   if (StAtt315Shd) {
      delete [] StAtt315Shd;
      StAtt315Shd = NULL;
   }
}

void Sprite3D::LoadSprite3D(int faces, char *ViewID, int palID) {
   if (bAlreadyLoad)
		return;
	
	strcpy(NAME, ViewID);
   
   bAlreadyLoad = TRUE;

	LastAccess = timeGetTime();
   
   // Load a 3D Sprite
	char *ID = new char [80];
	char *Temp = new char [80];
	char Temp2[2];
	Temp2[1] = 0;
	
	Type = 2;
	faces /= FRAMING;
	Faces = faces;
	FacesA = 0;
   FacesC = 0;
   FacesR = 0;
   FacesStA = 0;
   FacesStM = 0;
   StMov = 0;
	StAtt = 0;
	
   View000 = new CV2Sprite [faces];
	View045 = new CV2Sprite [faces];
	View090 = new CV2Sprite [faces];
	View135 = new CV2Sprite [faces];
	View180 = new CV2Sprite [faces];

   MaxxCor = 1000;
	MaxyCor = 1000;
	int ChoseX = 0;
	int ChoseY = 0;
   int i;
   int iB000 = 0, iB045 = 0, iB090 = 0, iB135 = 0, iB180 = 0;

	//LPBYTE bestPal = CV2PalManager::GetInstance()->GetPal(ViewID, palID);

	for (i = 0; i < faces; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		//iB000 = View000[i].CreateSpriteByPal(ID, iB000, bestPal,palID);
      iB000 = View000[i].CreateSprite(ID, iB000, palID);
      
		if (MaxWidth < View000[i].GetWidth()) MaxWidth = View000[i].GetWidth();
		if (MaxHeight < View000[i].GetHeight()) MaxHeight = View000[i].GetHeight();
		if (MaxxCor > View000[i].GetCX()) MaxxCor = View000[i].GetCX();
		if (MaxyCor > View000[i].GetCY()) MaxyCor = View000[i].GetCY();
		if (ChoseX < View000[i].GetCX() + View000[i].GetWidth()) ChoseX = View000[i].GetCX() + View000[i].GetWidth();
		if (ChoseY < View000[i].GetCY() + View000[i].GetHeight()) ChoseY = View000[i].GetCY() + View000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		//iB045 = View045[i].CreateSpriteByPal(ID, iB045, bestPal,palID);
      iB045 = View045[i].CreateSprite(ID, iB045, palID);

		if (MaxWidth < View045[i].GetWidth()) MaxWidth = View045[i].GetWidth();
		if (MaxHeight < View045[i].GetHeight()) MaxHeight = View045[i].GetHeight();
		if (MaxxCor > View045[i].GetCX()) MaxxCor = View045[i].GetCX();
		if (MaxyCor > View045[i].GetCY()) MaxyCor = View045[i].GetCY();
		if (ChoseX < View045[i].GetCX() + View045[i].GetWidth()) ChoseX = View045[i].GetCX() + View045[i].GetWidth();
		if (ChoseY < View045[i].GetCY() + View045[i].GetHeight()) ChoseY = View045[i].GetCY() + View045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		//iB090 = View090[i].CreateSpriteByPal(ID, iB090, bestPal,palID);
      iB090 = View090[i].CreateSprite(ID, iB090, palID);
		if (MaxWidth < View090[i].GetWidth()) MaxWidth = View090[i].GetWidth();
		if (MaxHeight < View090[i].GetHeight()) MaxHeight = View090[i].GetHeight();
		if (MaxxCor > View090[i].GetCX()) MaxxCor = View090[i].GetCX();
		if (MaxyCor > View090[i].GetCY()) MaxyCor = View090[i].GetCY();
		if (ChoseX < View090[i].GetCX() + View090[i].GetWidth()) ChoseX = View090[i].GetCX() + View090[i].GetWidth();
		if (ChoseY < View090[i].GetCY() + View090[i].GetHeight()) ChoseY = View090[i].GetCY() + View090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		//iB135 = View135[i].CreateSpriteByPal(ID, iB135, bestPal,palID);
      iB135 = View135[i].CreateSprite(ID, iB135,palID);

		if (MaxWidth < View135[i].GetWidth()) MaxWidth = View135[i].GetWidth();
		if (MaxHeight < View135[i].GetHeight()) MaxHeight = View135[i].GetHeight();
		if (MaxxCor > View135[i].GetCX()) MaxxCor = View135[i].GetCX();
		if (MaxyCor > View135[i].GetCY()) MaxyCor = View135[i].GetCY();
		if (ChoseX < View135[i].GetCX() + View135[i].GetWidth()) ChoseX = View135[i].GetCX() + View135[i].GetWidth();
		if (ChoseY < View135[i].GetCY() + View135[i].GetHeight()) ChoseY = View135[i].GetCY() + View135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		//iB180 = View180[i].CreateSpriteByPal(ID, iB180, bestPal,palID);
      iB180 = View180[i].CreateSprite(ID, iB180, palID);
      
		if (MaxWidth < View180[i].GetWidth()) MaxWidth = View180[i].GetWidth();
		if (MaxHeight < View180[i].GetHeight()) MaxHeight = View180[i].GetHeight();
		if (MaxxCor > View180[i].GetCX()) MaxxCor = View180[i].GetCX();
		if (MaxyCor > View180[i].GetCY()) MaxyCor = View180[i].GetCY();
		if (ChoseX < View180[i].GetCX() + View180[i].GetWidth()) ChoseX = View180[i].GetCX() + View180[i].GetWidth();
		if (ChoseY < View180[i].GetCY() + View180[i].GetHeight()) ChoseY = View180[i].GetCY() + View180[i].GetHeight();
		
   }

   fTransparent = View000[0].GetfTransparent();
	TransColor = View000[0].GetTransColor();
   if ((MaxxCor + MaxyCor) % 2) {
      MaxxCor--;
      MaxWidth++;
   }
	MaxWidth += ChoseX - (MaxxCor + MaxWidth);
	MaxHeight += ChoseY - (MaxyCor + MaxHeight);

   if (Type == 3)
      Type = 0;

	delete ID;
	delete Temp;
}

void Sprite3D::LoadSprite3D(int faces, int faces2, int faces3, int faces4, int faces5, char *ViewID, int type, int stAtt, int stMov, int palID) {
   if (bAlreadyLoad)
		return;
	bAlreadyLoad = TRUE;

	strcpy(NAME, ViewID);

   LastAccess = timeGetTime();
   // Load a 3D Sprite
	char *ID = new char [80];
	char *Temp = new char [80];
	char Temp2[2];
	Temp2[1] = 0;
	
	Type = type;
	faces /= FRAMING;
	faces2 /= FRAMING;
	Faces = faces;
	FacesA = faces2;
	FacesC = faces5;
   StAtt = stAtt;
   StMov = stMov;
   FacesStA = faces3;
   FacesStM = faces4;
	
	View000 = new CV2Sprite [faces];
	View045 = new CV2Sprite [faces];
	View090 = new CV2Sprite [faces];
	View135 = new CV2Sprite [faces];
	View180 = new CV2Sprite [faces];

	Att000 = new CV2Sprite [faces2];
	Att045 = new CV2Sprite [faces2];
	Att090 = new CV2Sprite [faces2];
	Att135 = new CV2Sprite [faces2];
	Att180 = new CV2Sprite [faces2];

   if (faces5)
		Corpse = new CV2Sprite[faces5];

	View000Shd = new CV2Sprite [faces];
	View045Shd = new CV2Sprite [faces];
	View090Shd = new CV2Sprite [faces];
	View135Shd = new CV2Sprite [faces];
	View180Shd = new CV2Sprite [faces];
	View225Shd = new CV2Sprite [faces];
	View270Shd = new CV2Sprite [faces];
	View315Shd = new CV2Sprite [faces];

	Att000Shd = new CV2Sprite [faces2];
	Att045Shd = new CV2Sprite [faces2];
	Att090Shd = new CV2Sprite [faces2];
	Att135Shd = new CV2Sprite [faces2];
	Att180Shd = new CV2Sprite [faces2];
	Att225Shd = new CV2Sprite [faces2];
	Att270Shd = new CV2Sprite [faces2];
	Att315Shd = new CV2Sprite [faces2];

//   CorpseShd = new CV2Sprite[faces5];

   if (StAtt == -1) {
      StAtt000 = new CV2Sprite [faces3];
      StAtt045 = new CV2Sprite [faces3];
      StAtt090 = new CV2Sprite [faces3];
      StAtt135 = new CV2Sprite [faces3];
      StAtt180 = new CV2Sprite [faces3];
      StAtt000Shd = new CV2Sprite [faces3];
      StAtt045Shd = new CV2Sprite [faces3];
      StAtt090Shd = new CV2Sprite [faces3];
      StAtt135Shd = new CV2Sprite [faces3];
      StAtt180Shd = new CV2Sprite [faces3];
      StAtt225Shd = new CV2Sprite [faces3];
      StAtt270Shd = new CV2Sprite [faces3];
      StAtt315Shd = new CV2Sprite [faces3];
   }
   
   if (StMov == -1) {
      StMov000 = new CV2Sprite [faces4];
      StMov045 = new CV2Sprite [faces4];
      StMov090 = new CV2Sprite [faces4];
      StMov135 = new CV2Sprite [faces4];
      StMov180 = new CV2Sprite [faces4];
      StMov000Shd = new CV2Sprite [faces4];
      StMov045Shd = new CV2Sprite [faces4];
      StMov090Shd = new CV2Sprite [faces4];
      StMov135Shd = new CV2Sprite [faces4];
      StMov180Shd = new CV2Sprite [faces4];
      StMov225Shd = new CV2Sprite [faces4];
      StMov270Shd = new CV2Sprite [faces4];
      StMov315Shd = new CV2Sprite [faces4];
   }

	int ChoseX = 0;
	int ChoseY = 0;
   int iBC = 0;

   // New pallet loading system
	char *ViewIDA = new char[80];
	char *ViewIDStAtt = new char[80];
	char *ViewIDStMov = new char[80];
	char *ViewIDC = new char[80];
	strcpy(ViewIDC, ViewID);
	strcat(ViewIDC, "C");

	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1"); 
	LPBYTE bestPal = CV2PalManager::GetInstance()->GetPal(ViewIDC, palID);

   for (int i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      iBC = Corpse[i].CreateSpriteByPal(ID, iBC, bestPal,palID);
   }
	
/*   for (i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      strcat(ID, "Shd");
      CorpseShd[i].CreateSprite(ID);
   } */
	
   // strcpy(ViewIDA, ViewID);
	// strcat(ViewIDA, "");

   if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
	   CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	bestPal = CV2PalManager::GetInstance()->GetPal(ViewID, palID);
	int iB000 = 0, iB045 = 0, iB090 = 0, iB135 = 0, iB180 = 0;
   for (int i = 0; i < faces; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = View000[i].CreateSpriteByPal(ID, iB000, bestPal,palID);
		if (MaxWidth < View000[i].GetWidth()) MaxWidth = View000[i].GetWidth();
		if (MaxHeight < View000[i].GetHeight()) MaxHeight = View000[i].GetHeight();
		if (MaxxCor > View000[i].GetCX()) MaxxCor = View000[i].GetCX();
		if (MaxyCor > View000[i].GetCY()) MaxyCor = View000[i].GetCY();
		if (ChoseX < View000[i].GetCX() + View000[i].GetWidth()) ChoseX = View000[i].GetCX() + View000[i].GetWidth();
		if (ChoseY < View000[i].GetCY() + View000[i].GetHeight()) ChoseY = View000[i].GetCY() + View000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = View045[i].CreateSpriteByPal(ID, iB045, bestPal,palID);
		if (MaxWidth < View045[i].GetWidth()) MaxWidth = View045[i].GetWidth();
		if (MaxHeight < View045[i].GetHeight()) MaxHeight = View045[i].GetHeight();
		if (MaxxCor > View045[i].GetCX()) MaxxCor = View045[i].GetCX();
		if (MaxyCor > View045[i].GetCY()) MaxyCor = View045[i].GetCY();
		if (ChoseX < View045[i].GetCX() + View045[i].GetWidth()) ChoseX = View045[i].GetCX() + View045[i].GetWidth();
		if (ChoseY < View045[i].GetCY() + View045[i].GetHeight()) ChoseY = View045[i].GetCY() + View045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = View090[i].CreateSpriteByPal(ID, iB090, bestPal,palID);
		if (MaxWidth < View090[i].GetWidth()) MaxWidth = View090[i].GetWidth();
		if (MaxHeight < View090[i].GetHeight()) MaxHeight = View090[i].GetHeight();
		if (MaxxCor > View090[i].GetCX()) MaxxCor = View090[i].GetCX();
		if (MaxyCor > View090[i].GetCY()) MaxyCor = View090[i].GetCY();
		if (ChoseX < View090[i].GetCX() + View090[i].GetWidth()) ChoseX = View090[i].GetCX() + View090[i].GetWidth();
		if (ChoseY < View090[i].GetCY() + View090[i].GetHeight()) ChoseY = View090[i].GetCY() + View090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = View135[i].CreateSpriteByPal(ID, iB135, bestPal,palID);
		if (MaxWidth < View135[i].GetWidth()) MaxWidth = View135[i].GetWidth();
		if (MaxHeight < View135[i].GetHeight()) MaxHeight = View135[i].GetHeight();
		if (MaxxCor > View135[i].GetCX()) MaxxCor = View135[i].GetCX();
		if (MaxyCor > View135[i].GetCY()) MaxyCor = View135[i].GetCY();
		if (ChoseX < View135[i].GetCX() + View135[i].GetWidth()) ChoseX = View135[i].GetCX() + View135[i].GetWidth();
		if (ChoseY < View135[i].GetCY() + View135[i].GetHeight()) ChoseY = View135[i].GetCY() + View135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = View180[i].CreateSpriteByPal(ID, iB180, bestPal,palID);
		if (MaxWidth < View180[i].GetWidth()) MaxWidth = View180[i].GetWidth();
		if (MaxHeight < View180[i].GetHeight()) MaxHeight = View180[i].GetHeight();
		if (MaxxCor > View180[i].GetCX()) MaxxCor = View180[i].GetCX();
		if (MaxyCor > View180[i].GetCY()) MaxyCor = View180[i].GetCY();
		if (ChoseX < View180[i].GetCX() + View180[i].GetWidth()) ChoseX = View180[i].GetCX() + View180[i].GetWidth();
		if (ChoseY < View180[i].GetCY() + View180[i].GetHeight()) ChoseY = View180[i].GetCY() + View180[i].GetHeight();
		
	}

	strcpy(ViewIDA, ViewID);
	strcat(ViewIDA, "A");
	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	bestPal = CV2PalManager::GetInstance()->GetPal(ViewIDA, palID);
	iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces2; i++) {
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = Att000[i].CreateSpriteByPal(ID, iB000, bestPal,palID);
		if (MaxWidth < Att000[i].GetWidth()) MaxWidth = Att000[i].GetWidth();
		if (MaxHeight < Att000[i].GetHeight()) MaxHeight = Att000[i].GetHeight();
		if (MaxxCor > Att000[i].GetCX()) MaxxCor = Att000[i].GetCX();
		if (MaxyCor > Att000[i].GetCY()) MaxyCor = Att000[i].GetCY();
		if (ChoseX < Att000[i].GetCX() + Att000[i].GetWidth()) ChoseX = Att000[i].GetCX() + Att000[i].GetWidth();
		if (ChoseY < Att000[i].GetCY() + Att000[i].GetHeight()) ChoseY = Att000[i].GetCY() + Att000[i].GetHeight();

		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = Att045[i].CreateSpriteByPal(ID, iB045, bestPal,palID);
		if (MaxWidth < Att045[i].GetWidth()) MaxWidth = Att045[i].GetWidth();
		if (MaxHeight < Att045[i].GetHeight()) MaxHeight = Att045[i].GetHeight();
		if (MaxxCor > Att045[i].GetCX()) MaxxCor = Att045[i].GetCX();
		if (MaxyCor > Att045[i].GetCY()) MaxyCor = Att045[i].GetCY();
		if (ChoseX < Att045[i].GetCX() + Att045[i].GetWidth()) ChoseX = Att045[i].GetCX() + Att045[i].GetWidth();
		if (ChoseY < Att045[i].GetCY() + Att045[i].GetHeight()) ChoseY = Att045[i].GetCY() + Att045[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = Att090[i].CreateSpriteByPal(ID, iB090, bestPal,palID);
		if (MaxWidth < Att090[i].GetWidth()) MaxWidth = Att090[i].GetWidth();
		if (MaxHeight < Att090[i].GetHeight()) MaxHeight = Att090[i].GetHeight();
		if (MaxxCor > Att090[i].GetCX()) MaxxCor = Att090[i].GetCX();
		if (MaxyCor > Att090[i].GetCY()) MaxyCor = Att090[i].GetCY();
		if (ChoseX < Att090[i].GetCX() + Att090[i].GetWidth()) ChoseX = Att090[i].GetCX() + Att090[i].GetWidth();
		if (ChoseY < Att090[i].GetCY() + Att090[i].GetHeight()) ChoseY = Att090[i].GetCY() + Att090[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = Att135[i].CreateSpriteByPal(ID, iB135, bestPal,palID);
		if (MaxWidth < Att135[i].GetWidth()) MaxWidth = Att135[i].GetWidth();
		if (MaxHeight < Att135[i].GetHeight()) MaxHeight = Att135[i].GetHeight();
		if (MaxxCor > Att135[i].GetCX()) MaxxCor = Att135[i].GetCX();
		if (MaxyCor > Att135[i].GetCY()) MaxyCor = Att135[i].GetCY();
		if (ChoseX < Att135[i].GetCX() + Att135[i].GetWidth()) ChoseX = Att135[i].GetCX() + Att135[i].GetWidth();
		if (ChoseY < Att135[i].GetCY() + Att135[i].GetHeight()) ChoseY = Att135[i].GetCY() + Att135[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = Att180[i].CreateSpriteByPal(ID, iB180, bestPal,palID);
		if (MaxWidth < Att180[i].GetWidth()) MaxWidth = Att180[i].GetWidth();
		if (MaxHeight < Att180[i].GetHeight()) MaxHeight = Att180[i].GetHeight();
		if (MaxxCor > Att180[i].GetCX()) MaxxCor = Att180[i].GetCX();
		if (MaxyCor > Att180[i].GetCY()) MaxyCor = Att180[i].GetCY();
		if (ChoseX < Att180[i].GetCX() + Att180[i].GetWidth()) ChoseX = Att180[i].GetCX() + Att180[i].GetWidth();
		if (ChoseY < Att180[i].GetCY() + Att180[i].GetHeight()) ChoseY = Att180[i].GetCY() + Att180[i].GetHeight();
		
	}

	strcpy(ViewIDStAtt, ViewID);
	strcat(ViewIDStAtt, "StAtt");
	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	bestPal = CV2PalManager::GetInstance()->GetPal(ViewIDStAtt, palID);
   iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces3; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StAtt000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = StAtt000[i].CreateSpriteByPal(ID, iB000, bestPal,palID);
		if (MaxWidth < StAtt000[i].GetWidth()) MaxWidth = StAtt000[i].GetWidth();
		if (MaxHeight < StAtt000[i].GetHeight()) MaxHeight = StAtt000[i].GetHeight();
		if (MaxxCor > StAtt000[i].GetCX()) MaxxCor = StAtt000[i].GetCX();
		if (MaxyCor > StAtt000[i].GetCY()) MaxyCor = StAtt000[i].GetCY();
		if (ChoseX < StAtt000[i].GetCX() + StAtt000[i].GetWidth()) ChoseX = StAtt000[i].GetCX() + StAtt000[i].GetWidth();
		if (ChoseY < StAtt000[i].GetCY() + StAtt000[i].GetHeight()) ChoseY = StAtt000[i].GetCY() + StAtt000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StAtt045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = StAtt045[i].CreateSpriteByPal(ID, iB045, bestPal,palID);
		if (MaxWidth < StAtt045[i].GetWidth()) MaxWidth = StAtt045[i].GetWidth();
		if (MaxHeight < StAtt045[i].GetHeight()) MaxHeight = StAtt045[i].GetHeight();
		if (MaxxCor > StAtt045[i].GetCX()) MaxxCor = StAtt045[i].GetCX();
		if (MaxyCor > StAtt045[i].GetCY()) MaxyCor = StAtt045[i].GetCY();
		if (ChoseX < StAtt045[i].GetCX() + StAtt045[i].GetWidth()) ChoseX = StAtt045[i].GetCX() + StAtt045[i].GetWidth();
		if (ChoseY < StAtt045[i].GetCY() + StAtt045[i].GetHeight()) ChoseY = StAtt045[i].GetCY() + StAtt045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = StAtt090[i].CreateSpriteByPal(ID, iB090, bestPal,palID);
		if (MaxWidth < StAtt090[i].GetWidth()) MaxWidth = StAtt090[i].GetWidth();
		if (MaxHeight < StAtt090[i].GetHeight()) MaxHeight = StAtt090[i].GetHeight();
		if (MaxxCor > StAtt090[i].GetCX()) MaxxCor = StAtt090[i].GetCX();
		if (MaxyCor > StAtt090[i].GetCY()) MaxyCor = StAtt090[i].GetCY();
		if (ChoseX < StAtt090[i].GetCX() + StAtt090[i].GetWidth()) ChoseX = StAtt090[i].GetCX() + StAtt090[i].GetWidth();
		if (ChoseY < StAtt090[i].GetCY() + StAtt090[i].GetHeight()) ChoseY = StAtt090[i].GetCY() + StAtt090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = StAtt135[i].CreateSpriteByPal(ID, iB135, bestPal,palID);
		if (MaxWidth < StAtt135[i].GetWidth()) MaxWidth = StAtt135[i].GetWidth();
		if (MaxHeight < StAtt135[i].GetHeight()) MaxHeight = StAtt135[i].GetHeight();
		if (MaxxCor > StAtt135[i].GetCX()) MaxxCor = StAtt135[i].GetCX();
		if (MaxyCor > StAtt135[i].GetCY()) MaxyCor = StAtt135[i].GetCY();
		if (ChoseX < StAtt135[i].GetCX() + StAtt135[i].GetWidth()) ChoseX = StAtt135[i].GetCX() + StAtt135[i].GetWidth();
		if (ChoseY < StAtt135[i].GetCY() + StAtt135[i].GetHeight()) ChoseY = StAtt135[i].GetCY() + StAtt135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = StAtt180[i].CreateSpriteByPal(ID, iB180, bestPal,palID);
		if (MaxWidth < StAtt180[i].GetWidth()) MaxWidth = StAtt180[i].GetWidth();
		if (MaxHeight < StAtt180[i].GetHeight()) MaxHeight = StAtt180[i].GetHeight();
		if (MaxxCor > StAtt180[i].GetCX()) MaxxCor = StAtt180[i].GetCX();
		if (MaxyCor > StAtt180[i].GetCY()) MaxyCor = StAtt180[i].GetCY();
		if (ChoseX < StAtt180[i].GetCX() + StAtt180[i].GetWidth()) ChoseX = StAtt180[i].GetCX() + StAtt180[i].GetWidth();
		if (ChoseY < StAtt180[i].GetCY() + StAtt180[i].GetHeight()) ChoseY = StAtt180[i].GetCY() + StAtt180[i].GetHeight();
		
	}

	strcpy(ViewIDStMov, ViewID);
	strcat(ViewIDStMov, "StMov");
	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	bestPal = CV2PalManager::GetInstance()->GetPal(ViewIDStMov, palID);
	iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces4; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StMov000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = StMov000[i].CreateSpriteByPal(ID, iB000, bestPal,palID);
		if (MaxWidth < StMov000[i].GetWidth()) MaxWidth = StMov000[i].GetWidth();
		if (MaxHeight < StMov000[i].GetHeight()) MaxHeight = StMov000[i].GetHeight();
		if (MaxxCor > StMov000[i].GetCX()) MaxxCor = StMov000[i].GetCX();
		if (MaxyCor > StMov000[i].GetCY()) MaxyCor = StMov000[i].GetCY();
		if (ChoseX < StMov000[i].GetCX() + StMov000[i].GetWidth()) ChoseX = StMov000[i].GetCX() + StMov000[i].GetWidth();
		if (ChoseY < StMov000[i].GetCY() + StMov000[i].GetHeight()) ChoseY = StMov000[i].GetCY() + StMov000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StMov045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = StMov045[i].CreateSpriteByPal(ID, iB045, bestPal,palID);
		if (MaxWidth < StMov045[i].GetWidth()) MaxWidth = StMov045[i].GetWidth();
		if (MaxHeight < StMov045[i].GetHeight()) MaxHeight = StMov045[i].GetHeight();
		if (MaxxCor > StMov045[i].GetCX()) MaxxCor = StMov045[i].GetCX();
		if (MaxyCor > StMov045[i].GetCY()) MaxyCor = StMov045[i].GetCY();
		if (ChoseX < StMov045[i].GetCX() + StMov045[i].GetWidth()) ChoseX = StMov045[i].GetCX() + StMov045[i].GetWidth();
		if (ChoseY < StMov045[i].GetCY() + StMov045[i].GetHeight()) ChoseY = StMov045[i].GetCY() + StMov045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = StMov090[i].CreateSpriteByPal(ID, iB090, bestPal,palID);
		if (MaxWidth < StMov090[i].GetWidth()) MaxWidth = StMov090[i].GetWidth();
		if (MaxHeight < StMov090[i].GetHeight()) MaxHeight = StMov090[i].GetHeight();
		if (MaxxCor > StMov090[i].GetCX()) MaxxCor = StMov090[i].GetCX();
		if (MaxyCor > StMov090[i].GetCY()) MaxyCor = StMov090[i].GetCY();
		if (ChoseX < StMov090[i].GetCX() + StMov090[i].GetWidth()) ChoseX = StMov090[i].GetCX() + StMov090[i].GetWidth();
		if (ChoseY < StMov090[i].GetCY() + StMov090[i].GetHeight()) ChoseY = StMov090[i].GetCY() + StMov090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = StMov135[i].CreateSpriteByPal(ID, iB135, bestPal,palID);
		if (MaxWidth < StMov135[i].GetWidth()) MaxWidth = StMov135[i].GetWidth();
		if (MaxHeight < StMov135[i].GetHeight()) MaxHeight = StMov135[i].GetHeight();
		if (MaxxCor > StMov135[i].GetCX()) MaxxCor = StMov135[i].GetCX();
		if (MaxyCor > StMov135[i].GetCY()) MaxyCor = StMov135[i].GetCY();
		if (ChoseX < StMov135[i].GetCX() + StMov135[i].GetWidth()) ChoseX = StMov135[i].GetCX() + StMov135[i].GetWidth();
		if (ChoseY < StMov135[i].GetCY() + StMov135[i].GetHeight()) ChoseY = StMov135[i].GetCY() + StMov135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = StMov180[i].CreateSpriteByPal(ID, iB180, bestPal,palID);
		if (MaxWidth < StMov180[i].GetWidth()) MaxWidth = StMov180[i].GetWidth();
		if (MaxHeight < StMov180[i].GetHeight()) MaxHeight = StMov180[i].GetHeight();
		if (MaxxCor > StMov180[i].GetCX()) MaxxCor = StMov180[i].GetCX();
		if (MaxyCor > StMov180[i].GetCY()) MaxyCor = StMov180[i].GetCY();
		if (ChoseX < StMov180[i].GetCX() + StMov180[i].GetWidth()) ChoseX = StMov180[i].GetCX() + StMov180[i].GetWidth();
		if (ChoseY < StMov180[i].GetCY() + StMov180[i].GetHeight()) ChoseY = StMov180[i].GetCY() + StMov180[i].GetHeight();
		
	}

/*   for (i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      strcat(ID, "Shd");
      CorpseShd[i].CreateSprite(ID);
   } */
	
/*
   for (i = 0; i < faces; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View000Shd[i].CreateSprite(ID);
		if (MaxWidth < View000Shd[i].GetWidth()) MaxWidth = View000Shd[i].GetWidth();
		if (MaxHeight < View000Shd[i].GetHeight()) MaxHeight = View000Shd[i].GetHeight();
		if (MaxxCor > View000Shd[i].GetCX()) MaxxCor = View000Shd[i].GetCX();
		if (MaxyCor > View000Shd[i].GetCY()) MaxyCor = View000Shd[i].GetCY();
		if (ChoseX < View000Shd[i].GetCX() + View000Shd[i].GetWidth()) ChoseX = View000Shd[i].GetCX() + View000Shd[i].GetWidth();
		if (ChoseY < View000Shd[i].GetCY() + View000Shd[i].GetHeight()) ChoseY = View000Shd[i].GetCY() + View000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View045Shd[i].CreateSprite(ID);
		if (MaxWidth < View045Shd[i].GetWidth()) MaxWidth = View045Shd[i].GetWidth();
		if (MaxHeight < View045Shd[i].GetHeight()) MaxHeight = View045Shd[i].GetHeight();
		if (MaxxCor > View045Shd[i].GetCX()) MaxxCor = View045Shd[i].GetCX();
		if (MaxyCor > View045Shd[i].GetCY()) MaxyCor = View045Shd[i].GetCY();
		if (ChoseX < View045Shd[i].GetCX() + View045Shd[i].GetWidth()) ChoseX = View045Shd[i].GetCX() + View045Shd[i].GetWidth();
		if (ChoseY < View045Shd[i].GetCY() + View045Shd[i].GetHeight()) ChoseY = View045Shd[i].GetCY() + View045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View090Shd[i].CreateSprite(ID);
		if (MaxWidth < View090Shd[i].GetWidth()) MaxWidth = View090Shd[i].GetWidth();
		if (MaxHeight < View090Shd[i].GetHeight()) MaxHeight = View090Shd[i].GetHeight();
		if (MaxxCor > View090Shd[i].GetCX()) MaxxCor = View090Shd[i].GetCX();
		if (MaxyCor > View090Shd[i].GetCY()) MaxyCor = View090Shd[i].GetCY();
		if (ChoseX < View090Shd[i].GetCX() + View090Shd[i].GetWidth()) ChoseX = View090Shd[i].GetCX() + View090Shd[i].GetWidth();
		if (ChoseY < View090Shd[i].GetCY() + View090Shd[i].GetHeight()) ChoseY = View090Shd[i].GetCY() + View090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View135Shd[i].CreateSprite(ID);
		if (MaxWidth < View135Shd[i].GetWidth()) MaxWidth = View135Shd[i].GetWidth();
		if (MaxHeight < View135Shd[i].GetHeight()) MaxHeight = View135Shd[i].GetHeight();
		if (MaxxCor > View135Shd[i].GetCX()) MaxxCor = View135Shd[i].GetCX();
		if (MaxyCor > View135Shd[i].GetCY()) MaxyCor = View135Shd[i].GetCY();
		if (ChoseX < View135Shd[i].GetCX() + View135Shd[i].GetWidth()) ChoseX = View135Shd[i].GetCX() + View135Shd[i].GetWidth();
		if (ChoseY < View135Shd[i].GetCY() + View135Shd[i].GetHeight()) ChoseY = View135Shd[i].GetCY() + View135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View180Shd[i].CreateSprite(ID);
		if (MaxWidth < View180Shd[i].GetWidth()) MaxWidth = View180Shd[i].GetWidth();
		if (MaxHeight < View180Shd[i].GetHeight()) MaxHeight = View180Shd[i].GetHeight();
		if (MaxxCor > View180Shd[i].GetCX()) MaxxCor = View180Shd[i].GetCX();
		if (MaxyCor > View180Shd[i].GetCY()) MaxyCor = View180Shd[i].GetCY();
		if (ChoseX < View180Shd[i].GetCX() + View180Shd[i].GetWidth()) ChoseX = View180Shd[i].GetCX() + View180Shd[i].GetWidth();
		if (ChoseY < View180Shd[i].GetCY() + View180Shd[i].GetHeight()) ChoseY = View180Shd[i].GetCY() + View180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View225Shd[i].CreateSprite(ID);
		if (MaxWidth < View225Shd[i].GetWidth()) MaxWidth = View225Shd[i].GetWidth();
		if (MaxHeight < View225Shd[i].GetHeight()) MaxHeight = View225Shd[i].GetHeight();
		if (MaxxCor > View225Shd[i].GetCX()) MaxxCor = View225Shd[i].GetCX();
		if (MaxyCor > View225Shd[i].GetCY()) MaxyCor = View225Shd[i].GetCY();
		if (ChoseX < View225Shd[i].GetCX() + View225Shd[i].GetWidth()) ChoseX = View225Shd[i].GetCX() + View225Shd[i].GetWidth();
		if (ChoseY < View225Shd[i].GetCY() + View225Shd[i].GetHeight()) ChoseY = View225Shd[i].GetCY() + View225Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View270Shd[i].CreateSprite(ID);
		if (MaxWidth < View270Shd[i].GetWidth()) MaxWidth = View270Shd[i].GetWidth();
		if (MaxHeight < View270Shd[i].GetHeight()) MaxHeight = View270Shd[i].GetHeight();
		if (MaxxCor > View270Shd[i].GetCX()) MaxxCor = View270Shd[i].GetCX();
		if (MaxyCor > View270Shd[i].GetCY()) MaxyCor = View270Shd[i].GetCY();
		if (ChoseX < View270Shd[i].GetCX() + View270Shd[i].GetWidth()) ChoseX = View270Shd[i].GetCX() + View270Shd[i].GetWidth();
		if (ChoseY < View270Shd[i].GetCY() + View270Shd[i].GetHeight()) ChoseY = View270Shd[i].GetCY() + View270Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View315Shd[i].CreateSprite(ID);
		if (MaxWidth < View315Shd[i].GetWidth()) MaxWidth = View315Shd[i].GetWidth();
		if (MaxHeight < View315Shd[i].GetHeight()) MaxHeight = View315Shd[i].GetHeight();
		if (MaxxCor > View315Shd[i].GetCX()) MaxxCor = View315Shd[i].GetCX();
		if (MaxyCor > View315Shd[i].GetCY()) MaxyCor = View315Shd[i].GetCY();
		if (ChoseX < View315Shd[i].GetCX() + View315Shd[i].GetWidth()) ChoseX = View315Shd[i].GetCX() + View315Shd[i].GetWidth();
		if (ChoseY < View315Shd[i].GetCY() + View315Shd[i].GetHeight()) ChoseY = View315Shd[i].GetCY() + View315Shd[i].GetHeight();
	}

	for (i = 0; i < faces2; i++) {
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att000Shd[i].CreateSprite(ID);
		if (MaxWidth < Att000Shd[i].GetWidth()) MaxWidth = Att000Shd[i].GetWidth();
		if (MaxHeight < Att000Shd[i].GetHeight()) MaxHeight = Att000Shd[i].GetHeight();
		if (MaxxCor > Att000Shd[i].GetCX()) MaxxCor = Att000Shd[i].GetCX();
		if (MaxyCor > Att000Shd[i].GetCY()) MaxyCor = Att000Shd[i].GetCY();
		if (ChoseX < Att000Shd[i].GetCX() + Att000Shd[i].GetWidth()) ChoseX = Att000Shd[i].GetCX() + Att000Shd[i].GetWidth();
		if (ChoseY < Att000Shd[i].GetCY() + Att000Shd[i].GetHeight()) ChoseY = Att000Shd[i].GetCY() + Att000Shd[i].GetHeight();

		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att045Shd[i].CreateSprite(ID);
		if (MaxWidth < Att045Shd[i].GetWidth()) MaxWidth = Att045Shd[i].GetWidth();
		if (MaxHeight < Att045Shd[i].GetHeight()) MaxHeight = Att045Shd[i].GetHeight();
		if (MaxxCor > Att045Shd[i].GetCX()) MaxxCor = Att045Shd[i].GetCX();
		if (MaxyCor > Att045Shd[i].GetCY()) MaxyCor = Att045Shd[i].GetCY();
		if (ChoseX < Att045Shd[i].GetCX() + Att045Shd[i].GetWidth()) ChoseX = Att045Shd[i].GetCX() + Att045Shd[i].GetWidth();
		if (ChoseY < Att045Shd[i].GetCY() + Att045Shd[i].GetHeight()) ChoseY = Att045Shd[i].GetCY() + Att045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att090Shd[i].CreateSprite(ID);
		if (MaxWidth < Att090Shd[i].GetWidth()) MaxWidth = Att090Shd[i].GetWidth();
		if (MaxHeight < Att090Shd[i].GetHeight()) MaxHeight = Att090Shd[i].GetHeight();
		if (MaxxCor > Att090Shd[i].GetCX()) MaxxCor = Att090Shd[i].GetCX();
		if (MaxyCor > Att090Shd[i].GetCY()) MaxyCor = Att090Shd[i].GetCY();
		if (ChoseX < Att090Shd[i].GetCX() + Att090Shd[i].GetWidth()) ChoseX = Att090Shd[i].GetCX() + Att090Shd[i].GetWidth();
		if (ChoseY < Att090Shd[i].GetCY() + Att090Shd[i].GetHeight()) ChoseY = Att090Shd[i].GetCY() + Att090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att135Shd[i].CreateSprite(ID);
		if (MaxWidth < Att135Shd[i].GetWidth()) MaxWidth = Att135Shd[i].GetWidth();
		if (MaxHeight < Att135Shd[i].GetHeight()) MaxHeight = Att135Shd[i].GetHeight();
		if (MaxxCor > Att135Shd[i].GetCX()) MaxxCor = Att135Shd[i].GetCX();
		if (MaxyCor > Att135Shd[i].GetCY()) MaxyCor = Att135Shd[i].GetCY();
		if (ChoseX < Att135Shd[i].GetCX() + Att135Shd[i].GetWidth()) ChoseX = Att135Shd[i].GetCX() + Att135Shd[i].GetWidth();
		if (ChoseY < Att135Shd[i].GetCY() + Att135Shd[i].GetHeight()) ChoseY = Att135Shd[i].GetCY() + Att135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att180Shd[i].CreateSprite(ID);
		if (MaxWidth < Att180Shd[i].GetWidth()) MaxWidth = Att180Shd[i].GetWidth();
		if (MaxHeight < Att180Shd[i].GetHeight()) MaxHeight = Att180Shd[i].GetHeight();
		if (MaxxCor > Att180Shd[i].GetCX()) MaxxCor = Att180Shd[i].GetCX();
		if (MaxyCor > Att180Shd[i].GetCY()) MaxyCor = Att180Shd[i].GetCY();
		if (ChoseX < Att180Shd[i].GetCX() + Att180Shd[i].GetWidth()) ChoseX = Att180Shd[i].GetCX() + Att180Shd[i].GetWidth();
		if (ChoseY < Att180Shd[i].GetCY() + Att180Shd[i].GetHeight()) ChoseY = Att180Shd[i].GetCY() + Att180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att225Shd[i].CreateSprite(ID);
		if (MaxWidth < Att225Shd[i].GetWidth()) MaxWidth = Att225Shd[i].GetWidth();
		if (MaxHeight < Att225Shd[i].GetHeight()) MaxHeight = Att225Shd[i].GetHeight();
		if (MaxxCor > Att225Shd[i].GetCX()) MaxxCor = Att225Shd[i].GetCX();
		if (MaxyCor > Att225Shd[i].GetCY()) MaxyCor = Att225Shd[i].GetCY();
		if (ChoseX < Att225Shd[i].GetCX() + Att225Shd[i].GetWidth()) ChoseX = Att225Shd[i].GetCX() + Att225Shd[i].GetWidth();
		if (ChoseY < Att225Shd[i].GetCY() + Att225Shd[i].GetHeight()) ChoseY = Att225Shd[i].GetCY() + Att225Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att270Shd[i].CreateSprite(ID);
		if (MaxWidth < Att270Shd[i].GetWidth()) MaxWidth = Att270Shd[i].GetWidth();
		if (MaxHeight < Att270Shd[i].GetHeight()) MaxHeight = Att270Shd[i].GetHeight();
		if (MaxxCor > Att270Shd[i].GetCX()) MaxxCor = Att270Shd[i].GetCX();
		if (MaxyCor > Att270Shd[i].GetCY()) MaxyCor = Att270Shd[i].GetCY();
		if (ChoseX < Att270Shd[i].GetCX() + Att270Shd[i].GetWidth()) ChoseX = Att270Shd[i].GetCX() + Att270Shd[i].GetWidth();
		if (ChoseY < Att270Shd[i].GetCY() + Att270Shd[i].GetHeight()) ChoseY = Att270Shd[i].GetCY() + Att270Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att315Shd[i].CreateSprite(ID);
		if (MaxWidth < Att315Shd[i].GetWidth()) MaxWidth = Att315Shd[i].GetWidth();
		if (MaxHeight < Att315Shd[i].GetHeight()) MaxHeight = Att315Shd[i].GetHeight();
		if (MaxxCor > Att315Shd[i].GetCX()) MaxxCor = Att315Shd[i].GetCX();
		if (MaxyCor > Att315Shd[i].GetCY()) MaxyCor = Att315Shd[i].GetCY();
		if (ChoseX < Att315Shd[i].GetCX() + Att315Shd[i].GetWidth()) ChoseX = Att315Shd[i].GetCX() + Att315Shd[i].GetWidth();
		if (ChoseY < Att315Shd[i].GetCY() + Att315Shd[i].GetHeight()) ChoseY = Att315Shd[i].GetCY() + Att315Shd[i].GetHeight();
		
	}

   for (i = 0; i < faces3; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StAtt000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt000Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt000Shd[i].GetWidth()) MaxWidth = StAtt000Shd[i].GetWidth();
		if (MaxHeight < StAtt000Shd[i].GetHeight()) MaxHeight = StAtt000Shd[i].GetHeight();
		if (MaxxCor > StAtt000Shd[i].GetCX()) MaxxCor = StAtt000Shd[i].GetCX();
		if (MaxyCor > StAtt000Shd[i].GetCY()) MaxyCor = StAtt000Shd[i].GetCY();
		if (ChoseX < StAtt000Shd[i].GetCX() + StAtt000Shd[i].GetWidth()) ChoseX = StAtt000Shd[i].GetCX() + StAtt000Shd[i].GetWidth();
		if (ChoseY < StAtt000Shd[i].GetCY() + StAtt000Shd[i].GetHeight()) ChoseY = StAtt000Shd[i].GetCY() + StAtt000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StAtt045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt045Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt045Shd[i].GetWidth()) MaxWidth = StAtt045Shd[i].GetWidth();
		if (MaxHeight < StAtt045Shd[i].GetHeight()) MaxHeight = StAtt045Shd[i].GetHeight();
		if (MaxxCor > StAtt045Shd[i].GetCX()) MaxxCor = StAtt045Shd[i].GetCX();
		if (MaxyCor > StAtt045Shd[i].GetCY()) MaxyCor = StAtt045Shd[i].GetCY();
		if (ChoseX < StAtt045Shd[i].GetCX() + StAtt045Shd[i].GetWidth()) ChoseX = StAtt045Shd[i].GetCX() + StAtt045Shd[i].GetWidth();
		if (ChoseY < StAtt045Shd[i].GetCY() + StAtt045Shd[i].GetHeight()) ChoseY = StAtt045Shd[i].GetCY() + StAtt045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt090Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt090Shd[i].GetWidth()) MaxWidth = StAtt090Shd[i].GetWidth();
		if (MaxHeight < StAtt090Shd[i].GetHeight()) MaxHeight = StAtt090Shd[i].GetHeight();
		if (MaxxCor > StAtt090Shd[i].GetCX()) MaxxCor = StAtt090Shd[i].GetCX();
		if (MaxyCor > StAtt090Shd[i].GetCY()) MaxyCor = StAtt090Shd[i].GetCY();
		if (ChoseX < StAtt090Shd[i].GetCX() + StAtt090Shd[i].GetWidth()) ChoseX = StAtt090Shd[i].GetCX() + StAtt090Shd[i].GetWidth();
		if (ChoseY < StAtt090Shd[i].GetCY() + StAtt090Shd[i].GetHeight()) ChoseY = StAtt090Shd[i].GetCY() + StAtt090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt135Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt135Shd[i].GetWidth()) MaxWidth = StAtt135Shd[i].GetWidth();
		if (MaxHeight < StAtt135Shd[i].GetHeight()) MaxHeight = StAtt135Shd[i].GetHeight();
		if (MaxxCor > StAtt135Shd[i].GetCX()) MaxxCor = StAtt135Shd[i].GetCX();
		if (MaxyCor > StAtt135Shd[i].GetCY()) MaxyCor = StAtt135Shd[i].GetCY();
		if (ChoseX < StAtt135Shd[i].GetCX() + StAtt135Shd[i].GetWidth()) ChoseX = StAtt135Shd[i].GetCX() + StAtt135Shd[i].GetWidth();
		if (ChoseY < StAtt135Shd[i].GetCY() + StAtt135Shd[i].GetHeight()) ChoseY = StAtt135Shd[i].GetCY() + StAtt135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt180Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt180Shd[i].GetWidth()) MaxWidth = StAtt180Shd[i].GetWidth();
		if (MaxHeight < StAtt180Shd[i].GetHeight()) MaxHeight = StAtt180Shd[i].GetHeight();
		if (MaxxCor > StAtt180Shd[i].GetCX()) MaxxCor = StAtt180Shd[i].GetCX();
		if (MaxyCor > StAtt180Shd[i].GetCY()) MaxyCor = StAtt180Shd[i].GetCY();
		if (ChoseX < StAtt180Shd[i].GetCX() + StAtt180Shd[i].GetWidth()) ChoseX = StAtt180Shd[i].GetCX() + StAtt180Shd[i].GetWidth();
		if (ChoseY < StAtt180Shd[i].GetCY() + StAtt180Shd[i].GetHeight()) ChoseY = StAtt180Shd[i].GetCY() + StAtt180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt225Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt225Shd[i].GetWidth()) MaxWidth = StAtt225Shd[i].GetWidth();
		if (MaxHeight < StAtt225Shd[i].GetHeight()) MaxHeight = StAtt225Shd[i].GetHeight();
		if (MaxxCor > StAtt225Shd[i].GetCX()) MaxxCor = StAtt225Shd[i].GetCX();
		if (MaxyCor > StAtt225Shd[i].GetCY()) MaxyCor = StAtt225Shd[i].GetCY();
		if (ChoseX < StAtt225Shd[i].GetCX() + StAtt225Shd[i].GetWidth()) ChoseX = StAtt225Shd[i].GetCX() + StAtt225Shd[i].GetWidth();
		if (ChoseY < StAtt225Shd[i].GetCY() + StAtt225Shd[i].GetHeight()) ChoseY = StAtt225Shd[i].GetCY() + StAtt225Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt270Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt270Shd[i].GetWidth()) MaxWidth = StAtt270Shd[i].GetWidth();
		if (MaxHeight < StAtt270Shd[i].GetHeight()) MaxHeight = StAtt270Shd[i].GetHeight();
		if (MaxxCor > StAtt270Shd[i].GetCX()) MaxxCor = StAtt270Shd[i].GetCX();
		if (MaxyCor > StAtt270Shd[i].GetCY()) MaxyCor = StAtt270Shd[i].GetCY();
		if (ChoseX < StAtt270Shd[i].GetCX() + StAtt270Shd[i].GetWidth()) ChoseX = StAtt270Shd[i].GetCX() + StAtt270Shd[i].GetWidth();
		if (ChoseY < StAtt270Shd[i].GetCY() + StAtt270Shd[i].GetHeight()) ChoseY = StAtt270Shd[i].GetCY() + StAtt270Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt315Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt315Shd[i].GetWidth()) MaxWidth = StAtt315Shd[i].GetWidth();
		if (MaxHeight < StAtt315Shd[i].GetHeight()) MaxHeight = StAtt315Shd[i].GetHeight();
		if (MaxxCor > StAtt315Shd[i].GetCX()) MaxxCor = StAtt315Shd[i].GetCX();
		if (MaxyCor > StAtt315Shd[i].GetCY()) MaxyCor = StAtt315Shd[i].GetCY();
		if (ChoseX < StAtt315Shd[i].GetCX() + StAtt315Shd[i].GetWidth()) ChoseX = StAtt315Shd[i].GetCX() + StAtt315Shd[i].GetWidth();
		if (ChoseY < StAtt315Shd[i].GetCY() + StAtt315Shd[i].GetHeight()) ChoseY = StAtt315Shd[i].GetCY() + StAtt315Shd[i].GetHeight();
		
	}

	for (i = 0; i < faces4; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StMov000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov000Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov000Shd[i].GetWidth()) MaxWidth = StMov000Shd[i].GetWidth();
		if (MaxHeight < StMov000Shd[i].GetHeight()) MaxHeight = StMov000Shd[i].GetHeight();
		if (MaxxCor > StMov000Shd[i].GetCX()) MaxxCor = StMov000Shd[i].GetCX();
		if (MaxyCor > StMov000Shd[i].GetCY()) MaxyCor = StMov000Shd[i].GetCY();
		if (ChoseX < StMov000Shd[i].GetCX() + StMov000Shd[i].GetWidth()) ChoseX = StMov000Shd[i].GetCX() + StMov000Shd[i].GetWidth();
		if (ChoseY < StMov000Shd[i].GetCY() + StMov000Shd[i].GetHeight()) ChoseY = StMov000Shd[i].GetCY() + StMov000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StMov045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov045Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov045Shd[i].GetWidth()) MaxWidth = StMov045Shd[i].GetWidth();
		if (MaxHeight < StMov045Shd[i].GetHeight()) MaxHeight = StMov045Shd[i].GetHeight();
		if (MaxxCor > StMov045Shd[i].GetCX()) MaxxCor = StMov045Shd[i].GetCX();
		if (MaxyCor > StMov045Shd[i].GetCY()) MaxyCor = StMov045Shd[i].GetCY();
		if (ChoseX < StMov045Shd[i].GetCX() + StMov045Shd[i].GetWidth()) ChoseX = StMov045Shd[i].GetCX() + StMov045Shd[i].GetWidth();
		if (ChoseY < StMov045Shd[i].GetCY() + StMov045Shd[i].GetHeight()) ChoseY = StMov045Shd[i].GetCY() + StMov045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov090Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov090Shd[i].GetWidth()) MaxWidth = StMov090Shd[i].GetWidth();
		if (MaxHeight < StMov090Shd[i].GetHeight()) MaxHeight = StMov090Shd[i].GetHeight();
		if (MaxxCor > StMov090Shd[i].GetCX()) MaxxCor = StMov090Shd[i].GetCX();
		if (MaxyCor > StMov090Shd[i].GetCY()) MaxyCor = StMov090Shd[i].GetCY();
		if (ChoseX < StMov090Shd[i].GetCX() + StMov090Shd[i].GetWidth()) ChoseX = StMov090Shd[i].GetCX() + StMov090Shd[i].GetWidth();
		if (ChoseY < StMov090Shd[i].GetCY() + StMov090Shd[i].GetHeight()) ChoseY = StMov090Shd[i].GetCY() + StMov090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov135Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov135Shd[i].GetWidth()) MaxWidth = StMov135Shd[i].GetWidth();
		if (MaxHeight < StMov135Shd[i].GetHeight()) MaxHeight = StMov135Shd[i].GetHeight();
		if (MaxxCor > StMov135Shd[i].GetCX()) MaxxCor = StMov135Shd[i].GetCX();
		if (MaxyCor > StMov135Shd[i].GetCY()) MaxyCor = StMov135Shd[i].GetCY();
		if (ChoseX < StMov135Shd[i].GetCX() + StMov135Shd[i].GetWidth()) ChoseX = StMov135Shd[i].GetCX() + StMov135Shd[i].GetWidth();
		if (ChoseY < StMov135Shd[i].GetCY() + StMov135Shd[i].GetHeight()) ChoseY = StMov135Shd[i].GetCY() + StMov135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov180Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov180Shd[i].GetWidth()) MaxWidth = StMov180Shd[i].GetWidth();
		if (MaxHeight < StMov180Shd[i].GetHeight()) MaxHeight = StMov180Shd[i].GetHeight();
		if (MaxxCor > StMov180Shd[i].GetCX()) MaxxCor = StMov180Shd[i].GetCX();
		if (MaxyCor > StMov180Shd[i].GetCY()) MaxyCor = StMov180Shd[i].GetCY();
		if (ChoseX < StMov180Shd[i].GetCX() + StMov180Shd[i].GetWidth()) ChoseX = StMov180Shd[i].GetCX() + StMov180Shd[i].GetWidth();
		if (ChoseY < StMov180Shd[i].GetCY() + StMov180Shd[i].GetHeight()) ChoseY = StMov180Shd[i].GetCY() + StMov180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov225Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov225Shd[i].GetWidth()) MaxWidth = StMov225Shd[i].GetWidth();
		if (MaxHeight < StMov225Shd[i].GetHeight()) MaxHeight = StMov225Shd[i].GetHeight();
		if (MaxxCor > StMov225Shd[i].GetCX()) MaxxCor = StMov225Shd[i].GetCX();
		if (MaxyCor > StMov225Shd[i].GetCY()) MaxyCor = StMov225Shd[i].GetCY();
		if (ChoseX < StMov225Shd[i].GetCX() + StMov225Shd[i].GetWidth()) ChoseX = StMov225Shd[i].GetCX() + StMov225Shd[i].GetWidth();
		if (ChoseY < StMov225Shd[i].GetCY() + StMov225Shd[i].GetHeight()) ChoseY = StMov225Shd[i].GetCY() + StMov225Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov270Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov270Shd[i].GetWidth()) MaxWidth = StMov270Shd[i].GetWidth();
		if (MaxHeight < StMov270Shd[i].GetHeight()) MaxHeight = StMov270Shd[i].GetHeight();
		if (MaxxCor > StMov270Shd[i].GetCX()) MaxxCor = StMov270Shd[i].GetCX();
		if (MaxyCor > StMov270Shd[i].GetCY()) MaxyCor = StMov270Shd[i].GetCY();
		if (ChoseX < StMov270Shd[i].GetCX() + StMov270Shd[i].GetWidth()) ChoseX = StMov270Shd[i].GetCX() + StMov270Shd[i].GetWidth();
		if (ChoseY < StMov270Shd[i].GetCY() + StMov270Shd[i].GetHeight()) ChoseY = StMov270Shd[i].GetCY() + StMov270Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov315Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov315Shd[i].GetWidth()) MaxWidth = StMov315Shd[i].GetWidth();
		if (MaxHeight < StMov315Shd[i].GetHeight()) MaxHeight = StMov315Shd[i].GetHeight();
		if (MaxxCor > StMov315Shd[i].GetCX()) MaxxCor = StMov315Shd[i].GetCX();
		if (MaxyCor > StMov315Shd[i].GetCY()) MaxyCor = StMov315Shd[i].GetCY();
		if (ChoseX < StMov315Shd[i].GetCX() + StMov315Shd[i].GetWidth()) ChoseX = StMov315Shd[i].GetCX() + StMov315Shd[i].GetWidth();
		if (ChoseY < StMov315Shd[i].GetCY() + StMov315Shd[i].GetHeight()) ChoseY = StMov315Shd[i].GetCY() + StMov315Shd[i].GetHeight();
		
	}
*/
	fTransparent = View000[0].GetfTransparent();
	TransColor = View000[0].GetTransColor();
   if ((MaxxCor % 2)) {
      MaxxCor--;
      MaxWidth++;
   }
   if (!(MaxyCor % 2)) {
      MaxyCor--;
      MaxHeight++;
   }

	MaxWidth += ChoseX - (MaxxCor + MaxWidth);
	MaxHeight += ChoseY - (MaxyCor + MaxHeight);

   if (Type == 3)
      Type = 0;

	delete ID;
	delete Temp;
	delete ViewIDA;
	delete ViewIDC;
	delete ViewIDStAtt;
	delete ViewIDStMov;
}


void Sprite3D::LoadSprite3D(int faces, int faces4, int faces5, char *ViewID, int type, int stMov, int palID) {
   if (bAlreadyLoad)
		return;
	bAlreadyLoad = TRUE;

	strcpy(NAME, ViewID);

   LastAccess = timeGetTime();
   
   // Load a 3D Sprite
	char *ID = new char [80];
	char *Temp = new char [80];
	char Temp2[2];
	Temp2[1] = 0;
	
	Type = type;
	faces /= FRAMING; // Conflicts with Darkfang
	Faces = faces;
	FacesA = 0;
   FacesC = faces5;
   FacesStA = 0;
   FacesStM = faces4;
   StMov = stMov;
	StAtt = 0;
	
   View000 = new CV2Sprite [faces];
	View045 = new CV2Sprite [faces];
	View090 = new CV2Sprite [faces];
	View135 = new CV2Sprite [faces];
	View180 = new CV2Sprite [faces];

   Corpse = new CV2Sprite[faces5];

   if (StMov == -1) {
      StMov000 = new CV2Sprite [faces4];
      StMov045 = new CV2Sprite [faces4];
      StMov090 = new CV2Sprite [faces4];
      StMov135 = new CV2Sprite [faces4];
      StMov180 = new CV2Sprite [faces4];
   }

   MaxxCor = 1000;
	MaxyCor = 1000;
	int ChoseX = 0;
	int ChoseY = 0;
   int iBC = 0;

   for (int i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      iBC = Corpse[i].CreateSprite(ID, iBC, palID);
   }
	
/*   for (i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      strcat(ID, "Shd");
      CorpseShd[i].CreateSprite(ID);
   } */

	int iB000 = 0, iB045 = 0, iB090 = 0, iB135 = 0, iB180 = 0;
   
   for (int i = 0; i < faces; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = View000[i].CreateSprite(ID, iB000, palID);
		if (MaxWidth < View000[i].GetWidth()) MaxWidth = View000[i].GetWidth();
		if (MaxHeight < View000[i].GetHeight()) MaxHeight = View000[i].GetHeight();
		if (MaxxCor > View000[i].GetCX()) MaxxCor = View000[i].GetCX();
		if (MaxyCor > View000[i].GetCY()) MaxyCor = View000[i].GetCY();
		if (ChoseX < View000[i].GetCX() + View000[i].GetWidth()) ChoseX = View000[i].GetCX() + View000[i].GetWidth();
		if (ChoseY < View000[i].GetCY() + View000[i].GetHeight()) ChoseY = View000[i].GetCY() + View000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = View045[i].CreateSprite(ID, iB045, palID);
		if (MaxWidth < View045[i].GetWidth()) MaxWidth = View045[i].GetWidth();
		if (MaxHeight < View045[i].GetHeight()) MaxHeight = View045[i].GetHeight();
		if (MaxxCor > View045[i].GetCX()) MaxxCor = View045[i].GetCX();
		if (MaxyCor > View045[i].GetCY()) MaxyCor = View045[i].GetCY();
		if (ChoseX < View045[i].GetCX() + View045[i].GetWidth()) ChoseX = View045[i].GetCX() + View045[i].GetWidth();
		if (ChoseY < View045[i].GetCY() + View045[i].GetHeight()) ChoseY = View045[i].GetCY() + View045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = View090[i].CreateSprite(ID, iB090, palID);
		if (MaxWidth < View090[i].GetWidth()) MaxWidth = View090[i].GetWidth();
		if (MaxHeight < View090[i].GetHeight()) MaxHeight = View090[i].GetHeight();
		if (MaxxCor > View090[i].GetCX()) MaxxCor = View090[i].GetCX();
		if (MaxyCor > View090[i].GetCY()) MaxyCor = View090[i].GetCY();
		if (ChoseX < View090[i].GetCX() + View090[i].GetWidth()) ChoseX = View090[i].GetCX() + View090[i].GetWidth();
		if (ChoseY < View090[i].GetCY() + View090[i].GetHeight()) ChoseY = View090[i].GetCY() + View090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = View135[i].CreateSprite(ID, iB135, palID);
		if (MaxWidth < View135[i].GetWidth()) MaxWidth = View135[i].GetWidth();
		if (MaxHeight < View135[i].GetHeight()) MaxHeight = View135[i].GetHeight();
		if (MaxxCor > View135[i].GetCX()) MaxxCor = View135[i].GetCX();
		if (MaxyCor > View135[i].GetCY()) MaxyCor = View135[i].GetCY();
		if (ChoseX < View135[i].GetCX() + View135[i].GetWidth()) ChoseX = View135[i].GetCX() + View135[i].GetWidth();
		if (ChoseY < View135[i].GetCY() + View135[i].GetHeight()) ChoseY = View135[i].GetCY() + View135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = View180[i].CreateSprite(ID, iB180, palID);
		if (MaxWidth < View180[i].GetWidth()) MaxWidth = View180[i].GetWidth();
		if (MaxHeight < View180[i].GetHeight()) MaxHeight = View180[i].GetHeight();
		if (MaxxCor > View180[i].GetCX()) MaxxCor = View180[i].GetCX();
		if (MaxyCor > View180[i].GetCY()) MaxyCor = View180[i].GetCY();
		if (ChoseX < View180[i].GetCX() + View180[i].GetWidth()) ChoseX = View180[i].GetCX() + View180[i].GetWidth();
		if (ChoseY < View180[i].GetCY() + View180[i].GetHeight()) ChoseY = View180[i].GetCY() + View180[i].GetHeight();
		
   }

	iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces4; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StMov000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = StMov000[i].CreateSprite(ID, iB000, palID);
		if (MaxWidth < StMov000[i].GetWidth()) MaxWidth = StMov000[i].GetWidth();
		if (MaxHeight < StMov000[i].GetHeight()) MaxHeight = StMov000[i].GetHeight();
		if (MaxxCor > StMov000[i].GetCX()) MaxxCor = StMov000[i].GetCX();
		if (MaxyCor > StMov000[i].GetCY()) MaxyCor = StMov000[i].GetCY();
		if (ChoseX < StMov000[i].GetCX() + StMov000[i].GetWidth()) ChoseX = StMov000[i].GetCX() + StMov000[i].GetWidth();
		if (ChoseY < StMov000[i].GetCY() + StMov000[i].GetHeight()) ChoseY = StMov000[i].GetCY() + StMov000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StMov045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = StMov045[i].CreateSprite(ID, iB045, palID);
		if (MaxWidth < StMov045[i].GetWidth()) MaxWidth = StMov045[i].GetWidth();
		if (MaxHeight < StMov045[i].GetHeight()) MaxHeight = StMov045[i].GetHeight();
		if (MaxxCor > StMov045[i].GetCX()) MaxxCor = StMov045[i].GetCX();
		if (MaxyCor > StMov045[i].GetCY()) MaxyCor = StMov045[i].GetCY();
		if (ChoseX < StMov045[i].GetCX() + StMov045[i].GetWidth()) ChoseX = StMov045[i].GetCX() + StMov045[i].GetWidth();
		if (ChoseY < StMov045[i].GetCY() + StMov045[i].GetHeight()) ChoseY = StMov045[i].GetCY() + StMov045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = StMov090[i].CreateSprite(ID, iB090, palID);
		if (MaxWidth < StMov090[i].GetWidth()) MaxWidth = StMov090[i].GetWidth();
		if (MaxHeight < StMov090[i].GetHeight()) MaxHeight = StMov090[i].GetHeight();
		if (MaxxCor > StMov090[i].GetCX()) MaxxCor = StMov090[i].GetCX();
		if (MaxyCor > StMov090[i].GetCY()) MaxyCor = StMov090[i].GetCY();
		if (ChoseX < StMov090[i].GetCX() + StMov090[i].GetWidth()) ChoseX = StMov090[i].GetCX() + StMov090[i].GetWidth();
		if (ChoseY < StMov090[i].GetCY() + StMov090[i].GetHeight()) ChoseY = StMov090[i].GetCY() + StMov090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = StMov135[i].CreateSprite(ID, iB135, palID);
		if (MaxWidth < StMov135[i].GetWidth()) MaxWidth = StMov135[i].GetWidth();
		if (MaxHeight < StMov135[i].GetHeight()) MaxHeight = StMov135[i].GetHeight();
		if (MaxxCor > StMov135[i].GetCX()) MaxxCor = StMov135[i].GetCX();
		if (MaxyCor > StMov135[i].GetCY()) MaxyCor = StMov135[i].GetCY();
		if (ChoseX < StMov135[i].GetCX() + StMov135[i].GetWidth()) ChoseX = StMov135[i].GetCX() + StMov135[i].GetWidth();
		if (ChoseY < StMov135[i].GetCY() + StMov135[i].GetHeight()) ChoseY = StMov135[i].GetCY() + StMov135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = StMov180[i].CreateSprite(ID, iB180, palID);
		if (MaxWidth < StMov180[i].GetWidth()) MaxWidth = StMov180[i].GetWidth();
		if (MaxHeight < StMov180[i].GetHeight()) MaxHeight = StMov180[i].GetHeight();
		if (MaxxCor > StMov180[i].GetCX()) MaxxCor = StMov180[i].GetCX();
		if (MaxyCor > StMov180[i].GetCY()) MaxyCor = StMov180[i].GetCY();
		if (ChoseX < StMov180[i].GetCX() + StMov180[i].GetWidth()) ChoseX = StMov180[i].GetCX() + StMov180[i].GetWidth();
		if (ChoseY < StMov180[i].GetCY() + StMov180[i].GetHeight()) ChoseY = StMov180[i].GetCY() + StMov180[i].GetHeight();
		
	}

/*
   
   for (i = 0; i < faces; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View000Shd[i].CreateSprite(ID);
		if (MaxWidth < View000Shd[i].GetWidth()) MaxWidth = View000Shd[i].GetWidth();
		if (MaxHeight < View000Shd[i].GetHeight()) MaxHeight = View000Shd[i].GetHeight();
		if (MaxxCor > View000Shd[i].GetCX()) MaxxCor = View000Shd[i].GetCX();
		if (MaxyCor > View000Shd[i].GetCY()) MaxyCor = View000Shd[i].GetCY();
		if (ChoseX < View000Shd[i].GetCX() + View000Shd[i].GetWidth()) ChoseX = View000Shd[i].GetCX() + View000Shd[i].GetWidth();
		if (ChoseY < View000Shd[i].GetCY() + View000Shd[i].GetHeight()) ChoseY = View000Shd[i].GetCY() + View000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View045Shd[i].CreateSprite(ID);
		if (MaxWidth < View045Shd[i].GetWidth()) MaxWidth = View045Shd[i].GetWidth();
		if (MaxHeight < View045Shd[i].GetHeight()) MaxHeight = View045Shd[i].GetHeight();
		if (MaxxCor > View045Shd[i].GetCX()) MaxxCor = View045Shd[i].GetCX();
		if (MaxyCor > View045Shd[i].GetCY()) MaxyCor = View045Shd[i].GetCY();
		if (ChoseX < View045Shd[i].GetCX() + View045Shd[i].GetWidth()) ChoseX = View045Shd[i].GetCX() + View045Shd[i].GetWidth();
		if (ChoseY < View045Shd[i].GetCY() + View045Shd[i].GetHeight()) ChoseY = View045Shd[i].GetCY() + View045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View090Shd[i].CreateSprite(ID);
		if (MaxWidth < View090Shd[i].GetWidth()) MaxWidth = View090Shd[i].GetWidth();
		if (MaxHeight < View090Shd[i].GetHeight()) MaxHeight = View090Shd[i].GetHeight();
		if (MaxxCor > View090Shd[i].GetCX()) MaxxCor = View090Shd[i].GetCX();
		if (MaxyCor > View090Shd[i].GetCY()) MaxyCor = View090Shd[i].GetCY();
		if (ChoseX < View090Shd[i].GetCX() + View090Shd[i].GetWidth()) ChoseX = View090Shd[i].GetCX() + View090Shd[i].GetWidth();
		if (ChoseY < View090Shd[i].GetCY() + View090Shd[i].GetHeight()) ChoseY = View090Shd[i].GetCY() + View090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View135Shd[i].CreateSprite(ID);
		if (MaxWidth < View135Shd[i].GetWidth()) MaxWidth = View135Shd[i].GetWidth();
		if (MaxHeight < View135Shd[i].GetHeight()) MaxHeight = View135Shd[i].GetHeight();
		if (MaxxCor > View135Shd[i].GetCX()) MaxxCor = View135Shd[i].GetCX();
		if (MaxyCor > View135Shd[i].GetCY()) MaxyCor = View135Shd[i].GetCY();
		if (ChoseX < View135Shd[i].GetCX() + View135Shd[i].GetWidth()) ChoseX = View135Shd[i].GetCX() + View135Shd[i].GetWidth();
		if (ChoseY < View135Shd[i].GetCY() + View135Shd[i].GetHeight()) ChoseY = View135Shd[i].GetCY() + View135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View180Shd[i].CreateSprite(ID);
		if (MaxWidth < View180Shd[i].GetWidth()) MaxWidth = View180Shd[i].GetWidth();
		if (MaxHeight < View180Shd[i].GetHeight()) MaxHeight = View180Shd[i].GetHeight();
		if (MaxxCor > View180Shd[i].GetCX()) MaxxCor = View180Shd[i].GetCX();
		if (MaxyCor > View180Shd[i].GetCY()) MaxyCor = View180Shd[i].GetCY();
		if (ChoseX < View180Shd[i].GetCX() + View180Shd[i].GetWidth()) ChoseX = View180Shd[i].GetCX() + View180Shd[i].GetWidth();
		if (ChoseY < View180Shd[i].GetCY() + View180Shd[i].GetHeight()) ChoseY = View180Shd[i].GetCY() + View180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View225Shd[i].CreateSprite(ID);
		if (MaxWidth < View225Shd[i].GetWidth()) MaxWidth = View225Shd[i].GetWidth();
		if (MaxHeight < View225Shd[i].GetHeight()) MaxHeight = View225Shd[i].GetHeight();
		if (MaxxCor > View225Shd[i].GetCX()) MaxxCor = View225Shd[i].GetCX();
		if (MaxyCor > View225Shd[i].GetCY()) MaxyCor = View225Shd[i].GetCY();
		if (ChoseX < View225Shd[i].GetCX() + View225Shd[i].GetWidth()) ChoseX = View225Shd[i].GetCX() + View225Shd[i].GetWidth();
		if (ChoseY < View225Shd[i].GetCY() + View225Shd[i].GetHeight()) ChoseY = View225Shd[i].GetCY() + View225Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View270Shd[i].CreateSprite(ID);
		if (MaxWidth < View270Shd[i].GetWidth()) MaxWidth = View270Shd[i].GetWidth();
		if (MaxHeight < View270Shd[i].GetHeight()) MaxHeight = View270Shd[i].GetHeight();
		if (MaxxCor > View270Shd[i].GetCX()) MaxxCor = View270Shd[i].GetCX();
		if (MaxyCor > View270Shd[i].GetCY()) MaxyCor = View270Shd[i].GetCY();
		if (ChoseX < View270Shd[i].GetCX() + View270Shd[i].GetWidth()) ChoseX = View270Shd[i].GetCX() + View270Shd[i].GetWidth();
		if (ChoseY < View270Shd[i].GetCY() + View270Shd[i].GetHeight()) ChoseY = View270Shd[i].GetCY() + View270Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View315Shd[i].CreateSprite(ID);
		if (MaxWidth < View315Shd[i].GetWidth()) MaxWidth = View315Shd[i].GetWidth();
		if (MaxHeight < View315Shd[i].GetHeight()) MaxHeight = View315Shd[i].GetHeight();
		if (MaxxCor > View315Shd[i].GetCX()) MaxxCor = View315Shd[i].GetCX();
		if (MaxyCor > View315Shd[i].GetCY()) MaxyCor = View315Shd[i].GetCY();
		if (ChoseX < View315Shd[i].GetCX() + View315Shd[i].GetWidth()) ChoseX = View315Shd[i].GetCX() + View315Shd[i].GetWidth();
		if (ChoseY < View315Shd[i].GetCY() + View315Shd[i].GetHeight()) ChoseY = View315Shd[i].GetCY() + View315Shd[i].GetHeight();
	}

	for (i = 0; i < faces4; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StMov000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov000Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov000Shd[i].GetWidth()) MaxWidth = StMov000Shd[i].GetWidth();
		if (MaxHeight < StMov000Shd[i].GetHeight()) MaxHeight = StMov000Shd[i].GetHeight();
		if (MaxxCor > StMov000Shd[i].GetCX()) MaxxCor = StMov000Shd[i].GetCX();
		if (MaxyCor > StMov000Shd[i].GetCY()) MaxyCor = StMov000Shd[i].GetCY();
		if (ChoseX < StMov000Shd[i].GetCX() + StMov000Shd[i].GetWidth()) ChoseX = StMov000Shd[i].GetCX() + StMov000Shd[i].GetWidth();
		if (ChoseY < StMov000Shd[i].GetCY() + StMov000Shd[i].GetHeight()) ChoseY = StMov000Shd[i].GetCY() + StMov000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StMov045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov045Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov045Shd[i].GetWidth()) MaxWidth = StMov045Shd[i].GetWidth();
		if (MaxHeight < StMov045Shd[i].GetHeight()) MaxHeight = StMov045Shd[i].GetHeight();
		if (MaxxCor > StMov045Shd[i].GetCX()) MaxxCor = StMov045Shd[i].GetCX();
		if (MaxyCor > StMov045Shd[i].GetCY()) MaxyCor = StMov045Shd[i].GetCY();
		if (ChoseX < StMov045Shd[i].GetCX() + StMov045Shd[i].GetWidth()) ChoseX = StMov045Shd[i].GetCX() + StMov045Shd[i].GetWidth();
		if (ChoseY < StMov045Shd[i].GetCY() + StMov045Shd[i].GetHeight()) ChoseY = StMov045Shd[i].GetCY() + StMov045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov090Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov090Shd[i].GetWidth()) MaxWidth = StMov090Shd[i].GetWidth();
		if (MaxHeight < StMov090Shd[i].GetHeight()) MaxHeight = StMov090Shd[i].GetHeight();
		if (MaxxCor > StMov090Shd[i].GetCX()) MaxxCor = StMov090Shd[i].GetCX();
		if (MaxyCor > StMov090Shd[i].GetCY()) MaxyCor = StMov090Shd[i].GetCY();
		if (ChoseX < StMov090Shd[i].GetCX() + StMov090Shd[i].GetWidth()) ChoseX = StMov090Shd[i].GetCX() + StMov090Shd[i].GetWidth();
		if (ChoseY < StMov090Shd[i].GetCY() + StMov090Shd[i].GetHeight()) ChoseY = StMov090Shd[i].GetCY() + StMov090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov135Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov135Shd[i].GetWidth()) MaxWidth = StMov135Shd[i].GetWidth();
		if (MaxHeight < StMov135Shd[i].GetHeight()) MaxHeight = StMov135Shd[i].GetHeight();
		if (MaxxCor > StMov135Shd[i].GetCX()) MaxxCor = StMov135Shd[i].GetCX();
		if (MaxyCor > StMov135Shd[i].GetCY()) MaxyCor = StMov135Shd[i].GetCY();
		if (ChoseX < StMov135Shd[i].GetCX() + StMov135Shd[i].GetWidth()) ChoseX = StMov135Shd[i].GetCX() + StMov135Shd[i].GetWidth();
		if (ChoseY < StMov135Shd[i].GetCY() + StMov135Shd[i].GetHeight()) ChoseY = StMov135Shd[i].GetCY() + StMov135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov180Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov180Shd[i].GetWidth()) MaxWidth = StMov180Shd[i].GetWidth();
		if (MaxHeight < StMov180Shd[i].GetHeight()) MaxHeight = StMov180Shd[i].GetHeight();
		if (MaxxCor > StMov180Shd[i].GetCX()) MaxxCor = StMov180Shd[i].GetCX();
		if (MaxyCor > StMov180Shd[i].GetCY()) MaxyCor = StMov180Shd[i].GetCY();
		if (ChoseX < StMov180Shd[i].GetCX() + StMov180Shd[i].GetWidth()) ChoseX = StMov180Shd[i].GetCX() + StMov180Shd[i].GetWidth();
		if (ChoseY < StMov180Shd[i].GetCY() + StMov180Shd[i].GetHeight()) ChoseY = StMov180Shd[i].GetCY() + StMov180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov225Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov225Shd[i].GetWidth()) MaxWidth = StMov225Shd[i].GetWidth();
		if (MaxHeight < StMov225Shd[i].GetHeight()) MaxHeight = StMov225Shd[i].GetHeight();
		if (MaxxCor > StMov225Shd[i].GetCX()) MaxxCor = StMov225Shd[i].GetCX();
		if (MaxyCor > StMov225Shd[i].GetCY()) MaxyCor = StMov225Shd[i].GetCY();
		if (ChoseX < StMov225Shd[i].GetCX() + StMov225Shd[i].GetWidth()) ChoseX = StMov225Shd[i].GetCX() + StMov225Shd[i].GetWidth();
		if (ChoseY < StMov225Shd[i].GetCY() + StMov225Shd[i].GetHeight()) ChoseY = StMov225Shd[i].GetCY() + StMov225Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov270Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov270Shd[i].GetWidth()) MaxWidth = StMov270Shd[i].GetWidth();
		if (MaxHeight < StMov270Shd[i].GetHeight()) MaxHeight = StMov270Shd[i].GetHeight();
		if (MaxxCor > StMov270Shd[i].GetCX()) MaxxCor = StMov270Shd[i].GetCX();
		if (MaxyCor > StMov270Shd[i].GetCY()) MaxyCor = StMov270Shd[i].GetCY();
		if (ChoseX < StMov270Shd[i].GetCX() + StMov270Shd[i].GetWidth()) ChoseX = StMov270Shd[i].GetCX() + StMov270Shd[i].GetWidth();
		if (ChoseY < StMov270Shd[i].GetCY() + StMov270Shd[i].GetHeight()) ChoseY = StMov270Shd[i].GetCY() + StMov270Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov315Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov315Shd[i].GetWidth()) MaxWidth = StMov315Shd[i].GetWidth();
		if (MaxHeight < StMov315Shd[i].GetHeight()) MaxHeight = StMov315Shd[i].GetHeight();
		if (MaxxCor > StMov315Shd[i].GetCX()) MaxxCor = StMov315Shd[i].GetCX();
		if (MaxyCor > StMov315Shd[i].GetCY()) MaxyCor = StMov315Shd[i].GetCY();
		if (ChoseX < StMov315Shd[i].GetCX() + StMov315Shd[i].GetWidth()) ChoseX = StMov315Shd[i].GetCX() + StMov315Shd[i].GetWidth();
		if (ChoseY < StMov315Shd[i].GetCY() + StMov315Shd[i].GetHeight()) ChoseY = StMov315Shd[i].GetCY() + StMov315Shd[i].GetHeight();
		
	}

*/

   fTransparent = View000[0].GetfTransparent();
	TransColor = View000[0].GetTransColor();
   if ((MaxxCor + MaxyCor) % 2) {
      MaxxCor--;
      MaxWidth++;
   }
	MaxWidth += ChoseX - (MaxxCor + MaxWidth);
	MaxHeight += ChoseY - (MaxyCor + MaxHeight);

   if (Type == 3)
      Type = 0;

	delete ID;
	delete Temp;
}

inline bool IsRangedObject( TFCObject *obj );

void Sprite3D::DrawSprite3D(int Direction, int Frame, int X, int Y, BYTE Stand, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *Object) {
	LastAccess = timeGetTime();
   V2SPRITEFX FX;
   ZeroMemory(&FX, sizeof(FX));
   FX.lpClipRect = Clip;
   FX.lpDirectDrawSurface = Surface;
   if (bOutlineColor > 100 && bOutlineColor != 150)
      bOutlineColor = 100;
   if (Object)
      FX.dwFX |= Object->bInvisible ? FX_NODRAW : NULL;
   if (boOutline) {
      FX.dwFX |= FX_OUTLINE;
		if (bOutlineColor == 150) {
			FX.dwOutlineColor = RGB(150,150,255);
		} else {
         float dFraction = (float)bOutlineColor/100;
         if (dFraction >= 0.500) {
            FX.dwOutlineColor = ((int)((1.000-dFraction) * lpDXDirectDraw->wRMask * 2) & lpDXDirectDraw->wRMask) + (lpDXDirectDraw->wGMask);
         } else {
            FX.dwOutlineColor = (lpDXDirectDraw->wRMask) + ((int)((dFraction)*lpDXDirectDraw->wGMask * 2) & lpDXDirectDraw->wGMask);
         }
		}
   }
   if ((Stand == ST_ATTACK_STANDING || Stand == ST_ATTACK) && FacesA && View045) {
    if (StAtt > -1) {
         switch (Direction) {
            case 1: 
//               Att045Shd[StAtt].DrawSpriteN(X, Y, &FX);
                if( IsRangedObject( Object ) && Rng045)
                {
                    Rng045[StAtt].DrawSpriteN(X, Y, &FX);
                }else{
                    Att045[StAtt].DrawSpriteN(X, Y, &FX);
                }
            break;
            case 2: 
                if( IsRangedObject( Object ) && Rng000){
                  Rng000[StAtt].DrawSpriteN(X, Y, &FX);
                }else{
                   Att000[StAtt].DrawSpriteN(X, Y, &FX); 
                } 
            break;
		      case 3: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
                if( IsRangedObject( Object ) && Rng045){
                    Rng045[StAtt].DrawSpriteN(X, Y, &FX);
                }else{
                    Att045[StAtt].DrawSpriteN(X, Y, &FX);
                }
            break;
            case 4:
                if( IsRangedObject( Object ) && Rng090){
                   Rng090[StAtt].DrawSpriteN(X, Y, &FX);
                }else{
                   Att090[StAtt].DrawSpriteN(X, Y, &FX); 
                }
            break;
		      case 6: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
                if( IsRangedObject( Object ) && Rng090){
                    Rng090[StAtt].DrawSpriteN(X, Y, &FX);
                }else{
                    Att090[StAtt].DrawSpriteN(X, Y, &FX);
                }
            break;
            case 7: 
                if( IsRangedObject( Object ) && Rng135){
                    Rng135[StAtt].DrawSpriteN(X, Y, &FX);
                }else{
                    Att135[StAtt].DrawSpriteN(X, Y, &FX);
                }
            break;
            case 8: 
                if( IsRangedObject( Object )&& Rng180 ){
                    Rng180[StAtt].DrawSpriteN(X, Y, &FX);
                }else{
                    Att180[StAtt].DrawSpriteN(X, Y, &FX); 
                }
            break;
		      case 9: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
                if( IsRangedObject( Object) && Rng135){
                     Rng135[StAtt].DrawSpriteN(X, Y, &FX);
                }else{
                    Att135[StAtt].DrawSpriteN(X, Y, &FX);
                }
            break;
         }
      } else {
         switch (Direction) {
            case 1: 
//               StAtt045Shd[StAtt].DrawSpriteN(X, Y, &FX);
               StAtt045[Frame].DrawSpriteN(X, Y, &FX); 
            break;
            case 2: 
//               StAtt000Shd[StAtt].DrawSpriteN(X, Y, &FX);
               StAtt000[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 3: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               StAtt315Shd[StAtt].DrawSpriteN(X, Y, &FX);
               StAtt045[Frame].DrawSpriteN(X, Y, &FX);
            break;
            case 4: 
//               StAtt090Shd[StAtt].DrawSpriteN(X, Y, &FX);
               StAtt090[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 6: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               StAtt270Shd[StAtt].DrawSpriteN(X, Y, &FX);
               StAtt090[Frame].DrawSpriteN(X, Y, &FX);
            break;
            case 7: 
//               StAtt135Shd[StAtt].DrawSpriteN(X, Y, &FX);
               StAtt135[Frame].DrawSpriteN(X, Y, &FX); 
            break;
            case 8: 
//               StAtt180Shd[StAtt].DrawSpriteN(X, Y, &FX);
               StAtt180[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 9: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               StAtt225Shd[StAtt].DrawSpriteN(X, Y, &FX);
               StAtt135[Frame].DrawSpriteN(X, Y, &FX);
            break;
         }
      }
   } else if (Stand == ST_STANDING && View045) {
    if (StMov > -1) {
         switch (Direction) {
            case 1: 
//               View045Shd[StMov].DrawSpriteN(X, Y, &FX); 
               View045[StMov].DrawSpriteN(X, Y, &FX); 
            break;
            case 2: 
//               View000Shd[StMov].DrawSpriteN(X, Y, &FX); 
               View000[StMov].DrawSpriteN(X, Y, &FX); 
            break;
		      case 3: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               View315Shd[StMov].DrawSpriteN(X, Y, &FX); 
               View045[StMov].DrawSpriteN(X, Y, &FX);
            break;
            case 4: 
//               View090Shd[StMov].DrawSpriteN(X, Y, &FX); 
               View090[StMov].DrawSpriteN(X, Y, &FX); 
            break;
		      case 6: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               View270Shd[StMov].DrawSpriteN(X, Y, &FX); 
               View090[StMov].DrawSpriteN(X, Y, &FX);
            break;
            case 7: 
//               View135Shd[StMov].DrawSpriteN(X, Y, &FX); 
               View135[StMov].DrawSpriteN(X, Y, &FX); 
            break;
            case 8: 
//               View180Shd[StMov].DrawSpriteN(X, Y, &FX); 
               View180[StMov].DrawSpriteN(X, Y, &FX); 
            break;
		      case 9: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               View225Shd[StMov].DrawSpriteN(X, Y, &FX); 
               View135[StMov].DrawSpriteN(X, Y, &FX);
            break;
         }
      } else {
         switch (Direction) {
            case 1: 
//               StMov045Shd[Frame].DrawSpriteN(X, Y, &FX); 
               StMov045[Frame].DrawSpriteN(X, Y, &FX); 
            break;
            case 2: 
//               StMov000Shd[Frame].DrawSpriteN(X, Y, &FX); 
               StMov000[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 3: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               StMov315Shd[Frame].DrawSpriteN(X, Y, &FX); 
               StMov045[Frame].DrawSpriteN(X, Y, &FX);
            break;
            case 4: 
//               StMov090Shd[Frame].DrawSpriteN(X, Y, &FX); 
               StMov090[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 6: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               StMov270Shd[Frame].DrawSpriteN(X, Y, &FX); 
               StMov090[Frame].DrawSpriteN(X, Y, &FX);
            break;
            case 7: 
//               StMov135Shd[Frame].DrawSpriteN(X, Y, &FX); 
               StMov135[Frame].DrawSpriteN(X, Y, &FX); 
            break;
            case 8:
//               StMov180Shd[Frame].DrawSpriteN(X, Y, &FX); 
               StMov180[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 9: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               StMov135Shd[Frame].DrawSpriteN(X, Y, &FX); 
               StMov135[Frame].DrawSpriteN(X, Y, &FX);
            break;
         }
      }
   } else {
      if (View045) {
         switch (Direction) {
            case 1: 
//               View045Shd[Frame].DrawSpriteN(X, Y, &FX); 
               View045[Frame].DrawSpriteN(X, Y, &FX); 
            break;
	         case 2: 
//               View000Shd[Frame].DrawSpriteN(X, Y, &FX); 
               View000[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 3: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               View315Shd[Frame].DrawSpriteN(X, Y, &FX);
               View045[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 4: 
//               View090Shd[Frame].DrawSpriteN(X, Y, &FX); 
               View090[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 6: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               View270Shd[Frame].DrawSpriteN(X, Y, &FX); 
               View090[Frame].DrawSpriteN(X, Y, &FX);
            break;
		      case 7: 
//               View135Shd[Frame].DrawSpriteN(X, Y, &FX); 
               View135[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 8: 
//               View180Shd[Frame].DrawSpriteN(X, Y, &FX); 
               View180[Frame].DrawSpriteN(X, Y, &FX); 
            break;
		      case 9: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
//               View225Shd[Frame].DrawSpriteN(X, Y, &FX); 
               View135[Frame].DrawSpriteN(X, Y, &FX);
            break;
	      }
      } else {
//         View000Shd[Frame].DrawSpriteN(X, Y, &FX);
         View000[Frame].DrawSpriteN(X, Y, &FX);
      }
   }
}

void Sprite3D::DrawSprite3DA(int Direction, int Frame, int X, int Y, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *Object) {
   if (FacesA) {
	LastAccess = timeGetTime();
   V2SPRITEFX FX;                                                                                    
   ZeroMemory(&FX, sizeof(FX));
   FX.lpClipRect = Clip;
   FX.lpDirectDrawSurface = Surface;
   if (Object)
      FX.dwFX |= Object->bInvisible ? FX_NODRAW : NULL;
   if (boOutline) {
      FX.dwFX |= FX_OUTLINE;
		if (bOutlineColor == 150) {
			FX.dwOutlineColor = RGB(150,150,255);
		} else {
         float dFraction = (float)bOutlineColor/100;
         if (dFraction >= 0.500) {
            FX.dwOutlineColor = ((int)((1.000-dFraction) * lpDXDirectDraw->wRMask * 2) & lpDXDirectDraw->wRMask) + (lpDXDirectDraw->wGMask);
         } else {
            FX.dwOutlineColor = (lpDXDirectDraw->wRMask) + ((int)((dFraction)*lpDXDirectDraw->wGMask * 2) & lpDXDirectDraw->wGMask);
         }
		}
   }
   if (View045) {
      switch (Direction) {
      	case 1: 
//            Att045Shd[Frame].DrawSpriteN(X, Y, &FX); 
            Att045[Frame].DrawSpriteN(X, Y, &FX); 
         break;
	      case 2: 
//            Att045Shd[Frame].DrawSpriteN(X, Y, &FX); 
            Att000[Frame].DrawSpriteN(X, Y, &FX); 
         break;
		   case 3: 
            FX.dwFX |= FX_LEFTRIGHTMIRROR;
//            Att270Shd[Frame].DrawSpriteN(X, Y, &FX); 
            Att045[Frame].DrawSpriteN(X, Y, &FX);
         break;
	      case 4: 
//            Att090Shd[Frame].DrawSpriteN(X, Y, &FX); 
            Att090[Frame].DrawSpriteN(X, Y, &FX); 
         break;
         case 6: 
            FX.dwFX |= FX_LEFTRIGHTMIRROR;
//            Att270Shd[Frame].DrawSpriteN(X, Y, &FX); 
            Att090[Frame].DrawSpriteN(X, Y, &FX);
         break;
	      case 7: 
//            Att135Shd[Frame].DrawSpriteN(X, Y, &FX); 
            Att135[Frame].DrawSpriteN(X, Y, &FX); 
         break;
	      case 8: 
//            Att180Shd[Frame].DrawSpriteN(X, Y, &FX); 
            Att180[Frame].DrawSpriteN(X, Y, &FX); 
         break;
	      case 9: 
            FX.dwFX |= FX_LEFTRIGHTMIRROR;
//            Att225Shd[Frame].DrawSpriteN(X, Y, &FX); 
            Att135[Frame].DrawSpriteN(X, Y, &FX);
         break;
      }
   } else {
//      Att000Shd[Frame].DrawSpriteN(X, Y, &FX);
      Att000[Frame].DrawSpriteN(X, Y, &FX);
   }
	} else {
		LOG << "* ERR = x10\r\n";
	}
}

void Sprite3D::DrawSprite3DR(int Direction, int Frame, int X, int Y, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *Object) {
   if (FacesR) 
   {
      LastAccess = timeGetTime();
      V2SPRITEFX FX;                                                                                    
      ZeroMemory(&FX, sizeof(FX));
      FX.lpClipRect = Clip;
      FX.lpDirectDrawSurface = Surface;
      if (Object)
         FX.dwFX |= Object->bInvisible ? FX_NODRAW : NULL;
      if (boOutline) 
      {
         FX.dwFX |= FX_OUTLINE;
         if (bOutlineColor == 150) 
         {
            FX.dwOutlineColor = RGB(150,150,255);
         } 
         else 
         {
            float dFraction = (float)bOutlineColor/100;
            if (dFraction >= 0.500) 
            {
               FX.dwOutlineColor = ((int)((1.000-dFraction) * lpDXDirectDraw->wRMask * 2) & lpDXDirectDraw->wRMask) + (lpDXDirectDraw->wGMask);
            } 
            else 
            {
               FX.dwOutlineColor = (lpDXDirectDraw->wRMask) + ((int)((dFraction)*lpDXDirectDraw->wGMask * 2) & lpDXDirectDraw->wGMask);
            }
         }
      }
      if (View045) 
      {
         switch (Direction) 
         {
            case 1: 
               //            Rng045Shd[Frame].DrawSpriteN(X, Y, &FX); 
               if(Rng045)
                  Rng045[Frame].DrawSpriteN(X, Y, &FX);
               else
                  Att045[Frame].DrawSpriteN(X, Y, &FX);

            break;
            case 2: 
               //            Rng045Shd[Frame].DrawSpriteN(X, Y, &FX); 
               if(Rng000)
                  Rng000[Frame].DrawSpriteN(X, Y, &FX); 
               else
                  Att000[Frame].DrawSpriteN(X, Y, &FX);
            break;
            case 3: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
               //            Rng270Shd[Frame].DrawSpriteN(X, Y, &FX); 
               if(Rng045)
                  Rng045[Frame].DrawSpriteN(X, Y, &FX);
               else
                  Att045[Frame].DrawSpriteN(X, Y, &FX);
            break;
            case 4: 
               //            Rng090Shd[Frame].DrawSpriteN(X, Y, &FX); 
               if(Rng090)
                  Rng090[Frame].DrawSpriteN(X, Y, &FX);
               else
                  Att090[Frame].DrawSpriteN(X, Y, &FX);
            break;
            case 6: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
               //            Rng270Shd[Frame].DrawSpriteN(X, Y, &FX); 
               if(Rng090)
                  Rng090[Frame].DrawSpriteN(X, Y, &FX);
               else
                  Att090[Frame].DrawSpriteN(X, Y, &FX);
            break;
            case 7: 
               //            Rng135Shd[Frame].DrawSpriteN(X, Y, &FX);
               if(Rng135)
                  Rng135[Frame].DrawSpriteN(X, Y, &FX); 
               else
                  Att135[Frame].DrawSpriteN(X, Y, &FX);
            break;
            case 8: 
               //            Rng180Shd[Frame].DrawSpriteN(X, Y, &FX); 
               if(Rng180)
                  Rng180[Frame].DrawSpriteN(X, Y, &FX); 
               else
                  Att180[Frame].DrawSpriteN(X, Y, &FX);
            break;
            case 9: 
               FX.dwFX |= FX_LEFTRIGHTMIRROR;
               //            Rng225Shd[Frame].DrawSpriteN(X, Y, &FX); 
               if(Rng135)
                  Rng135[Frame].DrawSpriteN(X, Y, &FX);
               else
                  Att135[Frame].DrawSpriteN(X, Y, &FX);
            break;
         }
      } 
      else 
      {
         //      Rng000Shd[Frame].DrawSpriteN(X, Y, &FX);
         if(Rng000)
            Rng000[Frame].DrawSpriteN(X, Y, &FX);
      }
   } 
   else 
   {
      LOG << "* ERR = x10\r\n";
   }
}


Random r(0, 1);

void Sprite3D::DrawCorpse(int CorpseFrame, int X, int Y, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE Surface, signed char Format, int AbsX, int AbsY, TFCObject *) {
   LastAccess = timeGetTime();
   V2SPRITEFX FX;
   ZeroMemory(&FX, sizeof(FX));
   FX.lpClipRect = Clip;
   FX.lpDirectDrawSurface = Surface;
   if (r(AbsX*3072+AbsY)) 
      FX.dwFX = FX_LEFTRIGHTMIRROR;
//	CorpseShd[CorpseFrame].DrawSpriteN(X, Y, &FX);
	Corpse[CorpseFrame].DrawSpriteN(X, Y, &FX);
}

void Sprite3D::LoadSpriteC(int faces, int faces2, int faces5, char *ViewID, int palID) {
   if (bAlreadyLoad)
		return;
	bAlreadyLoad = TRUE;

	strcpy(NAME, ViewID);
   LastAccess = timeGetTime();
   
   // Load a 3D Sprite
	char *ID = new char [80];
	char *Temp = new char [80];
	char Temp2[2];
	Temp2[1] = 0;
	
	Type = 2;
	faces /= FRAMING;
	Faces = faces;
	FacesA = faces2;
   FacesC = faces5;
   FacesStA = 0;
   FacesStM = 0;
   StMov = 0;
	StAtt = 0;
	
   View000 = new CV2Sprite [faces];
   Att000 = new CV2Sprite [faces2];
   View045 = 0;

   Corpse = new CV2Sprite[faces5];

   MaxxCor = 1000;
	MaxyCor = 1000;
	int ChoseX = 0;
	int ChoseY = 0;
   int iBC = 0;

   for (int i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      iBC = Corpse[i].CreateSprite(ID, iBC, palID);
   }
	
/*   for (i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      strcat(ID, "Shd");
      CorpseShd[i].CreateSprite(ID);
   } */
	
	int iB000 = 0;
   int leFrame = 0;
   for (int i = 0; i < faces; i++) {
		strcpy(ID, ViewID);
		if ((i*FRAMING)+1 < 100)
         strcat(ID, "0");
		if ((i*FRAMING)+1 < 10)
         strcat(ID, "0");
		if ((i*FRAMING)+1 < 1)
         strcat(ID, "0");
      leFrame = (i*FRAMING)+1;
      if (leFrame >= 100) {
         Temp2[0] = (leFrame/100)+'0';
         leFrame -= (leFrame/100)*100;
   		strcat(ID, Temp2);
      }
      if (leFrame >= 10) {
         Temp2[0] = (leFrame/10)+'0';
         leFrame -= (leFrame/10)*10;
   		strcat(ID, Temp2);
      }
      Temp2[0] = leFrame+'0';
		strcat(ID, Temp2);
		iB000 = View000[i].CreateSprite(ID, iB000, palID);
		if (MaxWidth < View000[i].GetWidth()) MaxWidth = View000[i].GetWidth();
		if (MaxHeight < View000[i].GetHeight()) MaxHeight = View000[i].GetHeight();
		if (MaxxCor > View000[i].GetCX()) MaxxCor = View000[i].GetCX();
		if (MaxyCor > View000[i].GetCY()) MaxyCor = View000[i].GetCY();
		if (ChoseX < View000[i].GetCX() + View000[i].GetWidth()) ChoseX = View000[i].GetCX() + View000[i].GetWidth();
		if (ChoseY < View000[i].GetCY() + View000[i].GetHeight()) ChoseY = View000[i].GetCY() + View000[i].GetHeight();
/*
      strcat(ID, "Shd");
		View000Shd[i].CreateSprite(ID);
		if (MaxWidth < View000Shd[i].GetWidth()) MaxWidth = View000Shd[i].GetWidth();
		if (MaxHeight < View000Shd[i].GetHeight()) MaxHeight = View000Shd[i].GetHeight();
		if (MaxxCor > View000Shd[i].GetCX()) MaxxCor = View000Shd[i].GetCX();
		if (MaxyCor > View000Shd[i].GetCY()) MaxyCor = View000Shd[i].GetCY();
		if (ChoseX < View000Shd[i].GetCX() + View000Shd[i].GetWidth()) ChoseX = View000Shd[i].GetCX() + View000Shd[i].GetWidth();
		if (ChoseY < View000Shd[i].GetCY() + View000Shd[i].GetHeight()) ChoseY = View000Shd[i].GetCY() + View000Shd[i].GetHeight();
*/      
   }

	iB000 = 0;
   for (int i = 0; i < faces2; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "a");
      if ((i*FRAMING)+1 < 10)
         strcat(ID, "0");
		if ((i*FRAMING)+1 < 1)
         strcat(ID, "0");
      leFrame = (i*FRAMING)+1;
      if (leFrame >= 100) {
         Temp2[0] = (leFrame/100)+'0';
         leFrame -= (leFrame/100)*100;
   		strcat(ID, Temp2);
      }
      if (leFrame >= 10) {
         Temp2[0] = (leFrame/10)+'0';
         leFrame -= (leFrame/10)*10;
   		strcat(ID, Temp2);
      }
      Temp2[0] = leFrame+'0';
		strcat(ID, Temp2);
		iB000 = Att000[i].CreateSprite(ID, iB000, palID);
		if (MaxWidth < Att000[i].GetWidth()) MaxWidth = Att000[i].GetWidth();
		if (MaxHeight < Att000[i].GetHeight()) MaxHeight = Att000[i].GetHeight();
		if (MaxxCor > Att000[i].GetCX()) MaxxCor = Att000[i].GetCX();
		if (MaxyCor > Att000[i].GetCY()) MaxyCor = Att000[i].GetCY();
		if (ChoseX < Att000[i].GetCX() + Att000[i].GetWidth()) ChoseX = Att000[i].GetCX() + Att000[i].GetWidth();
		if (ChoseY < Att000[i].GetCY() + Att000[i].GetHeight()) ChoseY = Att000[i].GetCY() + Att000[i].GetHeight();

/*		strcat(ID, "Shd");
		Att000Shd[i].CreateSprite(ID);
		if (MaxWidth < Att000Shd[i].GetWidth()) MaxWidth = Att000Shd[i].GetWidth();
		if (MaxHeight < Att000Shd[i].GetHeight()) MaxHeight = Att000Shd[i].GetHeight();
		if (MaxxCor > Att000Shd[i].GetCX()) MaxxCor = Att000Shd[i].GetCX();
		if (MaxyCor > Att000Shd[i].GetCY()) MaxyCor = Att000Shd[i].GetCY();
		if (ChoseX < Att000Shd[i].GetCX() + Att000Shd[i].GetWidth()) ChoseX = Att000Shd[i].GetCX() + Att000Shd[i].GetWidth();
		if (ChoseY < Att000Shd[i].GetCY() + Att000Shd[i].GetHeight()) ChoseY = Att000Shd[i].GetCY() + Att000Shd[i].GetHeight();
*/
   }

	fTransparent = View000[0].GetfTransparent();
	TransColor = View000[0].GetTransColor();
   if ((MaxxCor + MaxyCor) % 2) {
      MaxxCor--;
      MaxWidth++;
   }
	MaxWidth += ChoseX - (MaxxCor + MaxWidth);
	MaxHeight += ChoseY - (MaxyCor + MaxHeight);

	delete ID;
	delete Temp;
}

INFO *Sprite3D::GetCurrentVSF(int Direction, int Frame, BYTE Stand) {
   LastAccess = timeGetTime();
   INFO *Return = new INFO;
   if ((Stand == ST_ATTACK_STANDING || Stand == ST_ATTACK) && FacesA && View045) {
    if (StAtt > -1) {
         switch (Direction) {
            case 1: 
               Return->Width = Att045[StAtt].GetWidth();
               Return->Height = Att045[StAtt].GetHeight();
               Return->CX = Att045[StAtt].GetCX();
               Return->CY = Att045[StAtt].GetCY();
               return Return; 
            break;
            case 2: 
               Return->Width = Att000[StAtt].GetWidth();
               Return->Height = Att000[StAtt].GetHeight();
               Return->CX = Att000[StAtt].GetCX();
               Return->CY = Att000[StAtt].GetCY();
               return Return; 
            break;
            case 3:  
               Return->Width = Att045[StAtt].GetWidth();
               Return->Height = Att045[StAtt].GetHeight();
               Return->CX = Att045[StAtt].GetFCX();
               Return->CY = Att045[StAtt].GetFCY();
               return Return; 
            break;
            case 4: 
               Return->Width = Att090[StAtt].GetWidth();
               Return->Height = Att090[StAtt].GetHeight();
               Return->CX = Att090[StAtt].GetCX();
               Return->CY = Att090[StAtt].GetCY();
               return Return; 
            break;
            case 6: 
               Return->Width = Att090[StAtt].GetWidth();
               Return->Height = Att090[StAtt].GetHeight();
               Return->CX = Att090[StAtt].GetFCX();
               Return->CY = Att090[StAtt].GetFCY();
               return Return; 
            break;
            case 7: 
               Return->Width = Att135[StAtt].GetWidth();
               Return->Height = Att135[StAtt].GetHeight();
               Return->CX = Att135[StAtt].GetCX();
               Return->CY = Att135[StAtt].GetCY();
               return Return; 
            break;
            case 8: 
               Return->Width = Att180[StAtt].GetWidth();
               Return->Height = Att180[StAtt].GetHeight();
               Return->CX = Att180[StAtt].GetCX();
               Return->CY = Att180[StAtt].GetCY();
               return Return; 
            break;
            case 9: 
               Return->Width = Att135[StAtt].GetWidth();
               Return->Height = Att135[StAtt].GetHeight();
               Return->CX = Att135[StAtt].GetFCX();
               Return->CY = Att135[StAtt].GetFCY();
               return Return; 
            break;
         }
      } else {
         switch (Direction) {
            case 1: 
               Return->Width = StAtt045[Frame].GetWidth();
               Return->Height = StAtt045[Frame].GetHeight();
               Return->CX = StAtt045[Frame].GetCX();
               Return->CY = StAtt045[Frame].GetCY();
               return Return; 
            break;
            case 2: 
               Return->Width = StAtt000[Frame].GetWidth();
               Return->Height = StAtt000[Frame].GetHeight();
               Return->CX = StAtt000[Frame].GetCX();
               Return->CY = StAtt000[Frame].GetCY();
               return Return; 
            break;
            case 3:  
               Return->Width = StAtt045[Frame].GetWidth();
               Return->Height = StAtt045[Frame].GetHeight();
               Return->CX = StAtt045[Frame].GetFCX();
               Return->CY = StAtt045[Frame].GetFCY();
               return Return; 
            break;
            case 4: 
               Return->Width = StAtt090[Frame].GetWidth();
               Return->Height = StAtt090[Frame].GetHeight();
               Return->CX = StAtt090[Frame].GetCX();
               Return->CY = StAtt090[Frame].GetCY();
               return Return; 
            break;
            case 6: 
               Return->Width = StAtt090[Frame].GetWidth();
               Return->Height = StAtt090[Frame].GetHeight();
               Return->CX = StAtt090[Frame].GetFCX();
               Return->CY = StAtt090[Frame].GetFCY();
               return Return; 
            break;
            case 7: 
               Return->Width = StAtt135[Frame].GetWidth();
               Return->Height = StAtt135[Frame].GetHeight();
               Return->CX = StAtt135[Frame].GetCX();
               Return->CY = StAtt135[Frame].GetCY();
               return Return; 
            break;
            case 8: 
               Return->Width = StAtt180[Frame].GetWidth();
               Return->Height = StAtt180[Frame].GetHeight();
               Return->CX = StAtt180[Frame].GetCX();
               Return->CY = StAtt180[Frame].GetCY();
               return Return; 
            break;
            case 9: 
               Return->Width = StAtt135[Frame].GetWidth();
               Return->Height = StAtt135[Frame].GetHeight();
               Return->CX = StAtt135[Frame].GetFCX();
               Return->CY = StAtt135[Frame].GetFCY();
               return Return; 
            break;
         }
      }
   } else if (Stand == ST_STANDING && View045) {
    if (StMov > -1) {
         switch (Direction) {
            case 1: 
               Return->Width = View045[StMov].GetWidth();
               Return->Height = View045[StMov].GetHeight();
               Return->CX = View045[StMov].GetCX();
               Return->CY = View045[StMov].GetCY();
               return Return; 
            break;
            case 2: 
               Return->Width = View000[StMov].GetWidth();
               Return->Height = View000[StMov].GetHeight();
               Return->CX = View000[StMov].GetCX();
               Return->CY = View000[StMov].GetCY();
               return Return; 
            break;
            case 3:  
               Return->Width = View045[StMov].GetWidth();
               Return->Height = View045[StMov].GetHeight();
               Return->CX = View045[StMov].GetFCX();
               Return->CY = View045[StMov].GetFCY();
               return Return; 
            break;
            case 4: 
               Return->Width = View090[StMov].GetWidth();
               Return->Height = View090[StMov].GetHeight();
               Return->CX = View090[StMov].GetCX();
               Return->CY = View090[StMov].GetCY();
               return Return; 
            break;
            case 6: 
               Return->Width = View090[StMov].GetWidth();
               Return->Height = View090[StMov].GetHeight();
               Return->CX = View090[StMov].GetFCX();
               Return->CY = View090[StMov].GetFCY();
               return Return; 
            break;
            case 7: 
               Return->Width = View135[StMov].GetWidth();
               Return->Height = View135[StMov].GetHeight();
               Return->CX = View135[StMov].GetCX();
               Return->CY = View135[StMov].GetCY();
               return Return; 
            break;
            case 8: 
               Return->Width = View180[StMov].GetWidth();
               Return->Height = View180[StMov].GetHeight();
               Return->CX = View180[StMov].GetCX();
               Return->CY = View180[StMov].GetCY();
               return Return; 
            break;
            case 9: 
               Return->Width = View135[StMov].GetWidth();
               Return->Height = View135[StMov].GetHeight();
               Return->CX = View135[StMov].GetFCX();
               Return->CY = View135[StMov].GetFCY();
               return Return; 
            break;
         }
      } else {
         switch (Direction) {
            case 1: 
               Return->Width = StMov045[Frame].GetWidth();
               Return->Height = StMov045[Frame].GetHeight();
               Return->CX = StMov045[Frame].GetCX();
               Return->CY = StMov045[Frame].GetCY();
               return Return; 
            break;
            case 2: 
               Return->Width = StMov000[Frame].GetWidth();
               Return->Height = StMov000[Frame].GetHeight();
               Return->CX = StMov000[Frame].GetCX();
               Return->CY = StMov000[Frame].GetCY();
               return Return; 
            break;
            case 3:  
               Return->Width = StMov045[Frame].GetWidth();
               Return->Height = StMov045[Frame].GetHeight();
               Return->CX = StMov045[Frame].GetFCX();
               Return->CY = StMov045[Frame].GetFCY();
               return Return; 
            break;
            case 4: 
               Return->Width = StMov090[Frame].GetWidth();
               Return->Height = StMov090[Frame].GetHeight();
               Return->CX = StMov090[Frame].GetCX();
               Return->CY = StMov090[Frame].GetCY();
               return Return; 
            break;
            case 6: 
               Return->Width = StMov090[Frame].GetWidth();
               Return->Height = StMov090[Frame].GetHeight();
               Return->CX = StMov090[Frame].GetFCX();
               Return->CY = StMov090[Frame].GetFCY();
               return Return; 
            break;
            case 7: 
               Return->Width = StMov135[Frame].GetWidth();
               Return->Height = StMov135[Frame].GetHeight();
               Return->CX = StMov135[Frame].GetCX();
               Return->CY = StMov135[Frame].GetCY();
               return Return; 
            break;
            case 8: 
               Return->Width = StMov180[Frame].GetWidth();
               Return->Height = StMov180[Frame].GetHeight();
               Return->CX = StMov180[Frame].GetCX();
               Return->CY = StMov180[Frame].GetCY();
               return Return; 
            break;
            case 9: 
               Return->Width = StMov135[Frame].GetWidth();
               Return->Height = StMov135[Frame].GetHeight();
               Return->CX = StMov135[Frame].GetFCX();
               Return->CY = StMov135[Frame].GetFCY();
               return Return; 
            break;
         }
      }
   } else {
      if (View045) {
         switch (Direction) {
            case 1: 
               Return->Width = View045[Frame].GetWidth();
               Return->Height = View045[Frame].GetHeight();
               Return->CX = View045[Frame].GetCX();
               Return->CY = View045[Frame].GetCY();
               return Return; 
            break;
            case 2: 
               Return->Width = View000[Frame].GetWidth();
               Return->Height = View000[Frame].GetHeight();
               Return->CX = View000[Frame].GetCX();
               Return->CY = View000[Frame].GetCY();
               return Return; 
            break;
            case 3:  
               Return->Width = View045[Frame].GetWidth();
               Return->Height = View045[Frame].GetHeight();
               Return->CX = View045[Frame].GetFCX();
               Return->CY = View045[Frame].GetFCY();
               return Return; 
            break;
            case 4: 
               Return->Width = View090[Frame].GetWidth();
               Return->Height = View090[Frame].GetHeight();
               Return->CX = View090[Frame].GetCX();
               Return->CY = View090[Frame].GetCY();
               return Return; 
            break;
            case 6: 
               Return->Width = View090[Frame].GetWidth();
               Return->Height = View090[Frame].GetHeight();
               Return->CX = View090[Frame].GetFCX();
               Return->CY = View090[Frame].GetFCY();
               return Return; 
            break;
            case 7: 
               Return->Width = View135[Frame].GetWidth();
               Return->Height = View135[Frame].GetHeight();
               Return->CX = View135[Frame].GetCX();
               Return->CY = View135[Frame].GetCY();
               return Return; 
            break;
            case 8: 
               Return->Width = View180[Frame].GetWidth();
               Return->Height = View180[Frame].GetHeight();
               Return->CX = View180[Frame].GetCX();
               Return->CY = View180[Frame].GetCY();
               return Return; 
            break;
            case 9: 
               Return->Width = View135[Frame].GetWidth();
               Return->Height = View135[Frame].GetHeight();
               Return->CX = View135[Frame].GetFCX();
               Return->CY = View135[Frame].GetFCY();
               return Return; 
            break;
	      }
      } else {
          Return->Width = View000[Frame].GetWidth();
          Return->Height = View000[Frame].GetHeight();
          Return->CX = View000[Frame].GetFCX();
          Return->CY = View000[Frame].GetFCY();
          return Return; 
      }
   }

   // Default.
   Return->Width = View000[Frame].GetWidth();
   Return->Height = View000[Frame].GetHeight();
   Return->CX = View000[Frame].GetFCX();
   Return->CY = View000[Frame].GetFCY();
   return Return; 
}

void Sprite3D::LoadSprite3D(int faces, int faces2, int faces3, int faces4, int faces5, int faces6, char *ViewID, int type, int stAtt, int stMov, int palID) {
	if (bAlreadyLoad)
		return;
	bAlreadyLoad = TRUE;
	
	strcpy(NAME, ViewID);
	
	LastAccess = timeGetTime();
	// Load a 3D Sprite
	char *ID = new char [80];
	char *Temp = new char [80];
	char Temp2[2];
	Temp2[1] = 0;
	
	Type = type;
	faces /= FRAMING;
	faces2 /= FRAMING;
	faces6 /= FRAMING;
	Faces = faces;
	FacesA = faces2;
	FacesC = faces5;
	FacesR = faces6;
	StAtt = stAtt;
	StMov = stMov;
	FacesStA = faces3;
	FacesStM = faces4;
	
	if (faces) {
		View000 = new CV2Sprite [faces];
		View045 = new CV2Sprite [faces];
		View090 = new CV2Sprite [faces];
		View135 = new CV2Sprite [faces];
		View180 = new CV2Sprite [faces];
	}
	
	if (faces2) {
		Att000 = new CV2Sprite [faces2];
		Att045 = new CV2Sprite [faces2];
		Att090 = new CV2Sprite [faces2];
		Att135 = new CV2Sprite [faces2];
		Att180 = new CV2Sprite [faces2];
	}
	
	if (faces6) {
		Rng000 = new CV2Sprite [faces6];
		Rng045 = new CV2Sprite [faces6];
		Rng090 = new CV2Sprite [faces6];
		Rng135 = new CV2Sprite [faces6];
		Rng180 = new CV2Sprite [faces6];
	}
	
	if (faces5)
		Corpse = new CV2Sprite[faces5];
	
	if (faces) {
		View000Shd = new CV2Sprite [faces];
		View045Shd = new CV2Sprite [faces];
		View090Shd = new CV2Sprite [faces];
		View135Shd = new CV2Sprite [faces];
		View180Shd = new CV2Sprite [faces];
		View225Shd = new CV2Sprite [faces];
		View270Shd = new CV2Sprite [faces];
		View315Shd = new CV2Sprite [faces];
	}
	
	if (faces2) {
		Att000Shd = new CV2Sprite [faces2];
		Att045Shd = new CV2Sprite [faces2];
		Att090Shd = new CV2Sprite [faces2];
		Att135Shd = new CV2Sprite [faces2];
		Att180Shd = new CV2Sprite [faces2];
		Att225Shd = new CV2Sprite [faces2];
		Att270Shd = new CV2Sprite [faces2];
		Att315Shd = new CV2Sprite [faces2];
	}
	
	//   CorpseShd = new CV2Sprite[faces5];
	
	if (StAtt == -1) {
		StAtt000 = new CV2Sprite [faces3];
		StAtt045 = new CV2Sprite [faces3];
		StAtt090 = new CV2Sprite [faces3];
		StAtt135 = new CV2Sprite [faces3];
		StAtt180 = new CV2Sprite [faces3];
		StAtt000Shd = new CV2Sprite [faces3];
		StAtt045Shd = new CV2Sprite [faces3];
		StAtt090Shd = new CV2Sprite [faces3];
		StAtt135Shd = new CV2Sprite [faces3];
		StAtt180Shd = new CV2Sprite [faces3];
		StAtt225Shd = new CV2Sprite [faces3];
		StAtt270Shd = new CV2Sprite [faces3];
		StAtt315Shd = new CV2Sprite [faces3];
	}
	
	if (StMov == -1) {
		StMov000 = new CV2Sprite [faces4];
		StMov045 = new CV2Sprite [faces4];
		StMov090 = new CV2Sprite [faces4];
		StMov135 = new CV2Sprite [faces4];
		StMov180 = new CV2Sprite [faces4];
		StMov000Shd = new CV2Sprite [faces4];
		StMov045Shd = new CV2Sprite [faces4];
		StMov090Shd = new CV2Sprite [faces4];
		StMov135Shd = new CV2Sprite [faces4];
		StMov180Shd = new CV2Sprite [faces4];
		StMov225Shd = new CV2Sprite [faces4];
		StMov270Shd = new CV2Sprite [faces4];
		StMov315Shd = new CV2Sprite [faces4];
	}

	int ChoseX = 0;
	int ChoseY = 0;
	int iBC = 0;

	// New pallet loading system
	char *ViewIDA = new char[80];
	char *ViewIDB = new char[80];
	char *ViewIDStAtt = new char[80];
	char *ViewIDStMov = new char[80];
	char *ViewIDC = new char[80];
	strcpy(ViewIDC, ViewID);
	strcat(ViewIDC, "C");
	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	LPBYTE bestPal = CV2PalManager::GetInstance()->GetPal(ViewIDC, palID);

	for (int i = 0; i < faces5; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "C-");
		ID[strlen(ID)+1] = 0;
		ID[strlen(ID)] = i+'a';
		//iBC = Corpse[i].CreateSprite(ID, iBC, palID);
		iBC = Corpse[i].CreateSpriteByPal(ID, iBC, bestPal,palID);
	}
	
/*   for (i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      strcat(ID, "Shd");
      CorpseShd[i].CreateSprite(ID);
   } */
	

	// strcpy(ViewIDA, ViewID);
	// strcat(ViewIDA, "");
	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	bestPal = CV2PalManager::GetInstance()->GetPal(ViewID, palID);
	int iB000 = 0, iB045 = 0, iB090 = 0, iB135 = 0, iB180 = 0;
   for (int i = 0; i < faces; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = View000[i].CreateSpriteByPal(ID, iB000, bestPal,palID);
		if (MaxWidth < View000[i].GetWidth()) MaxWidth = View000[i].GetWidth();
		if (MaxHeight < View000[i].GetHeight()) MaxHeight = View000[i].GetHeight();
		if (MaxxCor > View000[i].GetCX()) MaxxCor = View000[i].GetCX();
		if (MaxyCor > View000[i].GetCY()) MaxyCor = View000[i].GetCY();
		if (ChoseX < View000[i].GetCX() + View000[i].GetWidth()) ChoseX = View000[i].GetCX() + View000[i].GetWidth();
		if (ChoseY < View000[i].GetCY() + View000[i].GetHeight()) ChoseY = View000[i].GetCY() + View000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = View045[i].CreateSpriteByPal(ID, iB045, bestPal,palID);
		if (MaxWidth < View045[i].GetWidth()) MaxWidth = View045[i].GetWidth();
		if (MaxHeight < View045[i].GetHeight()) MaxHeight = View045[i].GetHeight();
		if (MaxxCor > View045[i].GetCX()) MaxxCor = View045[i].GetCX();
		if (MaxyCor > View045[i].GetCY()) MaxyCor = View045[i].GetCY();
		if (ChoseX < View045[i].GetCX() + View045[i].GetWidth()) ChoseX = View045[i].GetCX() + View045[i].GetWidth();
		if (ChoseY < View045[i].GetCY() + View045[i].GetHeight()) ChoseY = View045[i].GetCY() + View045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = View090[i].CreateSpriteByPal(ID, iB090, bestPal,palID);
		if (MaxWidth < View090[i].GetWidth()) MaxWidth = View090[i].GetWidth();
		if (MaxHeight < View090[i].GetHeight()) MaxHeight = View090[i].GetHeight();
		if (MaxxCor > View090[i].GetCX()) MaxxCor = View090[i].GetCX();
		if (MaxyCor > View090[i].GetCY()) MaxyCor = View090[i].GetCY();
		if (ChoseX < View090[i].GetCX() + View090[i].GetWidth()) ChoseX = View090[i].GetCX() + View090[i].GetWidth();
		if (ChoseY < View090[i].GetCY() + View090[i].GetHeight()) ChoseY = View090[i].GetCY() + View090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = View135[i].CreateSpriteByPal(ID, iB135, bestPal,palID);
		if (MaxWidth < View135[i].GetWidth()) MaxWidth = View135[i].GetWidth();
		if (MaxHeight < View135[i].GetHeight()) MaxHeight = View135[i].GetHeight();
		if (MaxxCor > View135[i].GetCX()) MaxxCor = View135[i].GetCX();
		if (MaxyCor > View135[i].GetCY()) MaxyCor = View135[i].GetCY();
		if (ChoseX < View135[i].GetCX() + View135[i].GetWidth()) ChoseX = View135[i].GetCX() + View135[i].GetWidth();
		if (ChoseY < View135[i].GetCY() + View135[i].GetHeight()) ChoseY = View135[i].GetCY() + View135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = View180[i].CreateSpriteByPal(ID, iB180, bestPal,palID);
		if (MaxWidth < View180[i].GetWidth()) MaxWidth = View180[i].GetWidth();
		if (MaxHeight < View180[i].GetHeight()) MaxHeight = View180[i].GetHeight();
		if (MaxxCor > View180[i].GetCX()) MaxxCor = View180[i].GetCX();
		if (MaxyCor > View180[i].GetCY()) MaxyCor = View180[i].GetCY();
		if (ChoseX < View180[i].GetCX() + View180[i].GetWidth()) ChoseX = View180[i].GetCX() + View180[i].GetWidth();
		if (ChoseY < View180[i].GetCY() + View180[i].GetHeight()) ChoseY = View180[i].GetCY() + View180[i].GetHeight();
		
	}

	strcpy(ViewIDA, ViewID);
	strcat(ViewIDA, "A");
	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	bestPal = CV2PalManager::GetInstance()->GetPal(ViewIDA, palID);
	iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces2; i++) {
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = Att000[i].CreateSpriteByPal(ID, iB000, bestPal,palID);
		if (MaxWidth < Att000[i].GetWidth()) MaxWidth = Att000[i].GetWidth();
		if (MaxHeight < Att000[i].GetHeight()) MaxHeight = Att000[i].GetHeight();
		if (MaxxCor > Att000[i].GetCX()) MaxxCor = Att000[i].GetCX();
		if (MaxyCor > Att000[i].GetCY()) MaxyCor = Att000[i].GetCY();
		if (ChoseX < Att000[i].GetCX() + Att000[i].GetWidth()) ChoseX = Att000[i].GetCX() + Att000[i].GetWidth();
		if (ChoseY < Att000[i].GetCY() + Att000[i].GetHeight()) ChoseY = Att000[i].GetCY() + Att000[i].GetHeight();

		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = Att045[i].CreateSpriteByPal(ID, iB045, bestPal,palID);
		if (MaxWidth < Att045[i].GetWidth()) MaxWidth = Att045[i].GetWidth();
		if (MaxHeight < Att045[i].GetHeight()) MaxHeight = Att045[i].GetHeight();
		if (MaxxCor > Att045[i].GetCX()) MaxxCor = Att045[i].GetCX();
		if (MaxyCor > Att045[i].GetCY()) MaxyCor = Att045[i].GetCY();
		if (ChoseX < Att045[i].GetCX() + Att045[i].GetWidth()) ChoseX = Att045[i].GetCX() + Att045[i].GetWidth();
		if (ChoseY < Att045[i].GetCY() + Att045[i].GetHeight()) ChoseY = Att045[i].GetCY() + Att045[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = Att090[i].CreateSpriteByPal(ID, iB090, bestPal,palID);
		if (MaxWidth < Att090[i].GetWidth()) MaxWidth = Att090[i].GetWidth();
		if (MaxHeight < Att090[i].GetHeight()) MaxHeight = Att090[i].GetHeight();
		if (MaxxCor > Att090[i].GetCX()) MaxxCor = Att090[i].GetCX();
		if (MaxyCor > Att090[i].GetCY()) MaxyCor = Att090[i].GetCY();
		if (ChoseX < Att090[i].GetCX() + Att090[i].GetWidth()) ChoseX = Att090[i].GetCX() + Att090[i].GetWidth();
		if (ChoseY < Att090[i].GetCY() + Att090[i].GetHeight()) ChoseY = Att090[i].GetCY() + Att090[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = Att135[i].CreateSpriteByPal(ID, iB135, bestPal,palID);
		if (MaxWidth < Att135[i].GetWidth()) MaxWidth = Att135[i].GetWidth();
		if (MaxHeight < Att135[i].GetHeight()) MaxHeight = Att135[i].GetHeight();
		if (MaxxCor > Att135[i].GetCX()) MaxxCor = Att135[i].GetCX();
		if (MaxyCor > Att135[i].GetCY()) MaxyCor = Att135[i].GetCY();
		if (ChoseX < Att135[i].GetCX() + Att135[i].GetWidth()) ChoseX = Att135[i].GetCX() + Att135[i].GetWidth();
		if (ChoseY < Att135[i].GetCY() + Att135[i].GetHeight()) ChoseY = Att135[i].GetCY() + Att135[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = Att180[i].CreateSpriteByPal(ID, iB180, bestPal,palID);
		if (MaxWidth < Att180[i].GetWidth()) MaxWidth = Att180[i].GetWidth();
		if (MaxHeight < Att180[i].GetHeight()) MaxHeight = Att180[i].GetHeight();
		if (MaxxCor > Att180[i].GetCX()) MaxxCor = Att180[i].GetCX();
		if (MaxyCor > Att180[i].GetCY()) MaxyCor = Att180[i].GetCY();
		if (ChoseX < Att180[i].GetCX() + Att180[i].GetWidth()) ChoseX = Att180[i].GetCX() + Att180[i].GetWidth();
		if (ChoseY < Att180[i].GetCY() + Att180[i].GetHeight()) ChoseY = Att180[i].GetCY() + Att180[i].GetHeight();
		
	}

	strcpy(ViewIDB, ViewID);
	strcat(ViewIDB, "B");
	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	bestPal = CV2PalManager::GetInstance()->GetPal(ViewIDB, palID);
	iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces6; i++) {
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "B");
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = Rng000[i].CreateSpriteByPal(ID, iB000, bestPal,palID);
		if (MaxWidth < Rng000[i].GetWidth()) MaxWidth = Rng000[i].GetWidth();
		if (MaxHeight < Rng000[i].GetHeight()) MaxHeight = Rng000[i].GetHeight();
		if (MaxxCor > Rng000[i].GetCX()) MaxxCor = Rng000[i].GetCX();
		if (MaxyCor > Rng000[i].GetCY()) MaxyCor = Rng000[i].GetCY();
		if (ChoseX < Rng000[i].GetCX() + Rng000[i].GetWidth()) ChoseX = Rng000[i].GetCX() + Rng000[i].GetWidth();
		if (ChoseY < Rng000[i].GetCY() + Rng000[i].GetHeight()) ChoseY = Rng000[i].GetCY() + Rng000[i].GetHeight();

		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "B");
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = Rng045[i].CreateSpriteByPal(ID, iB045, bestPal,palID);
		if (MaxWidth < Rng045[i].GetWidth()) MaxWidth = Rng045[i].GetWidth();
		if (MaxHeight < Rng045[i].GetHeight()) MaxHeight = Rng045[i].GetHeight();
		if (MaxxCor > Rng045[i].GetCX()) MaxxCor = Rng045[i].GetCX();
		if (MaxyCor > Rng045[i].GetCY()) MaxyCor = Rng045[i].GetCY();
		if (ChoseX < Rng045[i].GetCX() + Rng045[i].GetWidth()) ChoseX = Rng045[i].GetCX() + Rng045[i].GetWidth();
		if (ChoseY < Rng045[i].GetCY() + Rng045[i].GetHeight()) ChoseY = Rng045[i].GetCY() + Rng045[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "B");
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = Rng090[i].CreateSpriteByPal(ID, iB090, bestPal,palID);
		if (MaxWidth < Rng090[i].GetWidth()) MaxWidth = Rng090[i].GetWidth();
		if (MaxHeight < Rng090[i].GetHeight()) MaxHeight = Rng090[i].GetHeight();
		if (MaxxCor > Rng090[i].GetCX()) MaxxCor = Rng090[i].GetCX();
		if (MaxyCor > Rng090[i].GetCY()) MaxyCor = Rng090[i].GetCY();
		if (ChoseX < Rng090[i].GetCX() + Rng090[i].GetWidth()) ChoseX = Rng090[i].GetCX() + Rng090[i].GetWidth();
		if (ChoseY < Rng090[i].GetCY() + Rng090[i].GetHeight()) ChoseY = Rng090[i].GetCY() + Rng090[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "B");
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = Rng135[i].CreateSpriteByPal(ID, iB135, bestPal,palID);
		if (MaxWidth < Rng135[i].GetWidth()) MaxWidth = Rng135[i].GetWidth();
		if (MaxHeight < Rng135[i].GetHeight()) MaxHeight = Rng135[i].GetHeight();
		if (MaxxCor > Rng135[i].GetCX()) MaxxCor = Rng135[i].GetCX();
		if (MaxyCor > Rng135[i].GetCY()) MaxyCor = Rng135[i].GetCY();
		if (ChoseX < Rng135[i].GetCX() + Rng135[i].GetWidth()) ChoseX = Rng135[i].GetCX() + Rng135[i].GetWidth();
		if (ChoseY < Rng135[i].GetCY() + Rng135[i].GetHeight()) ChoseY = Rng135[i].GetCY() + Rng135[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "B");
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = Rng180[i].CreateSpriteByPal(ID, iB180, bestPal,palID);
		if (MaxWidth < Rng180[i].GetWidth()) MaxWidth = Rng180[i].GetWidth();
		if (MaxHeight < Rng180[i].GetHeight()) MaxHeight = Rng180[i].GetHeight();
		if (MaxxCor > Rng180[i].GetCX()) MaxxCor = Rng180[i].GetCX();
		if (MaxyCor > Rng180[i].GetCY()) MaxyCor = Rng180[i].GetCY();
		if (ChoseX < Rng180[i].GetCX() + Rng180[i].GetWidth()) ChoseX = Rng180[i].GetCX() + Rng180[i].GetWidth();
		if (ChoseY < Rng180[i].GetCY() + Rng180[i].GetHeight()) ChoseY = Rng180[i].GetCY() + Rng180[i].GetHeight();
		
	}

	strcpy(ViewIDStAtt, ViewID);
	strcat(ViewIDStAtt, "StAtt");
	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	bestPal = CV2PalManager::GetInstance()->GetPal(ViewIDStAtt, palID);
   iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces3; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StAtt000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = StAtt000[i].CreateSpriteByPal(ID, iB000, bestPal,palID);
		if (MaxWidth < StAtt000[i].GetWidth()) MaxWidth = StAtt000[i].GetWidth();
		if (MaxHeight < StAtt000[i].GetHeight()) MaxHeight = StAtt000[i].GetHeight();
		if (MaxxCor > StAtt000[i].GetCX()) MaxxCor = StAtt000[i].GetCX();
		if (MaxyCor > StAtt000[i].GetCY()) MaxyCor = StAtt000[i].GetCY();
		if (ChoseX < StAtt000[i].GetCX() + StAtt000[i].GetWidth()) ChoseX = StAtt000[i].GetCX() + StAtt000[i].GetWidth();
		if (ChoseY < StAtt000[i].GetCY() + StAtt000[i].GetHeight()) ChoseY = StAtt000[i].GetCY() + StAtt000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StAtt045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = StAtt045[i].CreateSpriteByPal(ID, iB045, bestPal,palID);
		if (MaxWidth < StAtt045[i].GetWidth()) MaxWidth = StAtt045[i].GetWidth();
		if (MaxHeight < StAtt045[i].GetHeight()) MaxHeight = StAtt045[i].GetHeight();
		if (MaxxCor > StAtt045[i].GetCX()) MaxxCor = StAtt045[i].GetCX();
		if (MaxyCor > StAtt045[i].GetCY()) MaxyCor = StAtt045[i].GetCY();
		if (ChoseX < StAtt045[i].GetCX() + StAtt045[i].GetWidth()) ChoseX = StAtt045[i].GetCX() + StAtt045[i].GetWidth();
		if (ChoseY < StAtt045[i].GetCY() + StAtt045[i].GetHeight()) ChoseY = StAtt045[i].GetCY() + StAtt045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = StAtt090[i].CreateSpriteByPal(ID, iB090, bestPal,palID);
		if (MaxWidth < StAtt090[i].GetWidth()) MaxWidth = StAtt090[i].GetWidth();
		if (MaxHeight < StAtt090[i].GetHeight()) MaxHeight = StAtt090[i].GetHeight();
		if (MaxxCor > StAtt090[i].GetCX()) MaxxCor = StAtt090[i].GetCX();
		if (MaxyCor > StAtt090[i].GetCY()) MaxyCor = StAtt090[i].GetCY();
		if (ChoseX < StAtt090[i].GetCX() + StAtt090[i].GetWidth()) ChoseX = StAtt090[i].GetCX() + StAtt090[i].GetWidth();
		if (ChoseY < StAtt090[i].GetCY() + StAtt090[i].GetHeight()) ChoseY = StAtt090[i].GetCY() + StAtt090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = StAtt135[i].CreateSpriteByPal(ID, iB135, bestPal,palID);
		if (MaxWidth < StAtt135[i].GetWidth()) MaxWidth = StAtt135[i].GetWidth();
		if (MaxHeight < StAtt135[i].GetHeight()) MaxHeight = StAtt135[i].GetHeight();
		if (MaxxCor > StAtt135[i].GetCX()) MaxxCor = StAtt135[i].GetCX();
		if (MaxyCor > StAtt135[i].GetCY()) MaxyCor = StAtt135[i].GetCY();
		if (ChoseX < StAtt135[i].GetCX() + StAtt135[i].GetWidth()) ChoseX = StAtt135[i].GetCX() + StAtt135[i].GetWidth();
		if (ChoseY < StAtt135[i].GetCY() + StAtt135[i].GetHeight()) ChoseY = StAtt135[i].GetCY() + StAtt135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = StAtt180[i].CreateSpriteByPal(ID, iB180, bestPal,palID);
		if (MaxWidth < StAtt180[i].GetWidth()) MaxWidth = StAtt180[i].GetWidth();
		if (MaxHeight < StAtt180[i].GetHeight()) MaxHeight = StAtt180[i].GetHeight();
		if (MaxxCor > StAtt180[i].GetCX()) MaxxCor = StAtt180[i].GetCX();
		if (MaxyCor > StAtt180[i].GetCY()) MaxyCor = StAtt180[i].GetCY();
		if (ChoseX < StAtt180[i].GetCX() + StAtt180[i].GetWidth()) ChoseX = StAtt180[i].GetCX() + StAtt180[i].GetWidth();
		if (ChoseY < StAtt180[i].GetCY() + StAtt180[i].GetHeight()) ChoseY = StAtt180[i].GetCY() + StAtt180[i].GetHeight();
		
	}

   	strcpy(ViewIDStMov, ViewID);
	strcat(ViewIDStMov, "StMov");
	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	bestPal = CV2PalManager::GetInstance()->GetPal(ViewIDStMov, palID);
	iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces4; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StMov000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = StMov000[i].CreateSpriteByPal(ID, iB000, bestPal,palID);
		if (MaxWidth < StMov000[i].GetWidth()) MaxWidth = StMov000[i].GetWidth();
		if (MaxHeight < StMov000[i].GetHeight()) MaxHeight = StMov000[i].GetHeight();
		if (MaxxCor > StMov000[i].GetCX()) MaxxCor = StMov000[i].GetCX();
		if (MaxyCor > StMov000[i].GetCY()) MaxyCor = StMov000[i].GetCY();
		if (ChoseX < StMov000[i].GetCX() + StMov000[i].GetWidth()) ChoseX = StMov000[i].GetCX() + StMov000[i].GetWidth();
		if (ChoseY < StMov000[i].GetCY() + StMov000[i].GetHeight()) ChoseY = StMov000[i].GetCY() + StMov000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StMov045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = StMov045[i].CreateSpriteByPal(ID, iB045, bestPal,palID);
		if (MaxWidth < StMov045[i].GetWidth()) MaxWidth = StMov045[i].GetWidth();
		if (MaxHeight < StMov045[i].GetHeight()) MaxHeight = StMov045[i].GetHeight();
		if (MaxxCor > StMov045[i].GetCX()) MaxxCor = StMov045[i].GetCX();
		if (MaxyCor > StMov045[i].GetCY()) MaxyCor = StMov045[i].GetCY();
		if (ChoseX < StMov045[i].GetCX() + StMov045[i].GetWidth()) ChoseX = StMov045[i].GetCX() + StMov045[i].GetWidth();
		if (ChoseY < StMov045[i].GetCY() + StMov045[i].GetHeight()) ChoseY = StMov045[i].GetCY() + StMov045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = StMov090[i].CreateSpriteByPal(ID, iB090, bestPal,palID);
		if (MaxWidth < StMov090[i].GetWidth()) MaxWidth = StMov090[i].GetWidth();
		if (MaxHeight < StMov090[i].GetHeight()) MaxHeight = StMov090[i].GetHeight();
		if (MaxxCor > StMov090[i].GetCX()) MaxxCor = StMov090[i].GetCX();
		if (MaxyCor > StMov090[i].GetCY()) MaxyCor = StMov090[i].GetCY();
		if (ChoseX < StMov090[i].GetCX() + StMov090[i].GetWidth()) ChoseX = StMov090[i].GetCX() + StMov090[i].GetWidth();
		if (ChoseY < StMov090[i].GetCY() + StMov090[i].GetHeight()) ChoseY = StMov090[i].GetCY() + StMov090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = StMov135[i].CreateSpriteByPal(ID, iB135, bestPal,palID);
		if (MaxWidth < StMov135[i].GetWidth()) MaxWidth = StMov135[i].GetWidth();
		if (MaxHeight < StMov135[i].GetHeight()) MaxHeight = StMov135[i].GetHeight();
		if (MaxxCor > StMov135[i].GetCX()) MaxxCor = StMov135[i].GetCX();
		if (MaxyCor > StMov135[i].GetCY()) MaxyCor = StMov135[i].GetCY();
		if (ChoseX < StMov135[i].GetCX() + StMov135[i].GetWidth()) ChoseX = StMov135[i].GetCX() + StMov135[i].GetWidth();
		if (ChoseY < StMov135[i].GetCY() + StMov135[i].GetHeight()) ChoseY = StMov135[i].GetCY() + StMov135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = StMov180[i].CreateSpriteByPal(ID, iB180, bestPal,palID);
		if (MaxWidth < StMov180[i].GetWidth()) MaxWidth = StMov180[i].GetWidth();
		if (MaxHeight < StMov180[i].GetHeight()) MaxHeight = StMov180[i].GetHeight();
		if (MaxxCor > StMov180[i].GetCX()) MaxxCor = StMov180[i].GetCX();
		if (MaxyCor > StMov180[i].GetCY()) MaxyCor = StMov180[i].GetCY();
		if (ChoseX < StMov180[i].GetCX() + StMov180[i].GetWidth()) ChoseX = StMov180[i].GetCX() + StMov180[i].GetWidth();
		if (ChoseY < StMov180[i].GetCY() + StMov180[i].GetHeight()) ChoseY = StMov180[i].GetCY() + StMov180[i].GetHeight();
		
	}


   /*   for (i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      strcat(ID, "Shd");
      CorpseShd[i].CreateSprite(ID);
   } */
	
/*
   for (i = 0; i < faces; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View000Shd[i].CreateSprite(ID);
		if (MaxWidth < View000Shd[i].GetWidth()) MaxWidth = View000Shd[i].GetWidth();
		if (MaxHeight < View000Shd[i].GetHeight()) MaxHeight = View000Shd[i].GetHeight();
		if (MaxxCor > View000Shd[i].GetCX()) MaxxCor = View000Shd[i].GetCX();
		if (MaxyCor > View000Shd[i].GetCY()) MaxyCor = View000Shd[i].GetCY();
		if (ChoseX < View000Shd[i].GetCX() + View000Shd[i].GetWidth()) ChoseX = View000Shd[i].GetCX() + View000Shd[i].GetWidth();
		if (ChoseY < View000Shd[i].GetCY() + View000Shd[i].GetHeight()) ChoseY = View000Shd[i].GetCY() + View000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View045Shd[i].CreateSprite(ID);
		if (MaxWidth < View045Shd[i].GetWidth()) MaxWidth = View045Shd[i].GetWidth();
		if (MaxHeight < View045Shd[i].GetHeight()) MaxHeight = View045Shd[i].GetHeight();
		if (MaxxCor > View045Shd[i].GetCX()) MaxxCor = View045Shd[i].GetCX();
		if (MaxyCor > View045Shd[i].GetCY()) MaxyCor = View045Shd[i].GetCY();
		if (ChoseX < View045Shd[i].GetCX() + View045Shd[i].GetWidth()) ChoseX = View045Shd[i].GetCX() + View045Shd[i].GetWidth();
		if (ChoseY < View045Shd[i].GetCY() + View045Shd[i].GetHeight()) ChoseY = View045Shd[i].GetCY() + View045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View090Shd[i].CreateSprite(ID);
		if (MaxWidth < View090Shd[i].GetWidth()) MaxWidth = View090Shd[i].GetWidth();
		if (MaxHeight < View090Shd[i].GetHeight()) MaxHeight = View090Shd[i].GetHeight();
		if (MaxxCor > View090Shd[i].GetCX()) MaxxCor = View090Shd[i].GetCX();
		if (MaxyCor > View090Shd[i].GetCY()) MaxyCor = View090Shd[i].GetCY();
		if (ChoseX < View090Shd[i].GetCX() + View090Shd[i].GetWidth()) ChoseX = View090Shd[i].GetCX() + View090Shd[i].GetWidth();
		if (ChoseY < View090Shd[i].GetCY() + View090Shd[i].GetHeight()) ChoseY = View090Shd[i].GetCY() + View090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View135Shd[i].CreateSprite(ID);
		if (MaxWidth < View135Shd[i].GetWidth()) MaxWidth = View135Shd[i].GetWidth();
		if (MaxHeight < View135Shd[i].GetHeight()) MaxHeight = View135Shd[i].GetHeight();
		if (MaxxCor > View135Shd[i].GetCX()) MaxxCor = View135Shd[i].GetCX();
		if (MaxyCor > View135Shd[i].GetCY()) MaxyCor = View135Shd[i].GetCY();
		if (ChoseX < View135Shd[i].GetCX() + View135Shd[i].GetWidth()) ChoseX = View135Shd[i].GetCX() + View135Shd[i].GetWidth();
		if (ChoseY < View135Shd[i].GetCY() + View135Shd[i].GetHeight()) ChoseY = View135Shd[i].GetCY() + View135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View180Shd[i].CreateSprite(ID);
		if (MaxWidth < View180Shd[i].GetWidth()) MaxWidth = View180Shd[i].GetWidth();
		if (MaxHeight < View180Shd[i].GetHeight()) MaxHeight = View180Shd[i].GetHeight();
		if (MaxxCor > View180Shd[i].GetCX()) MaxxCor = View180Shd[i].GetCX();
		if (MaxyCor > View180Shd[i].GetCY()) MaxyCor = View180Shd[i].GetCY();
		if (ChoseX < View180Shd[i].GetCX() + View180Shd[i].GetWidth()) ChoseX = View180Shd[i].GetCX() + View180Shd[i].GetWidth();
		if (ChoseY < View180Shd[i].GetCY() + View180Shd[i].GetHeight()) ChoseY = View180Shd[i].GetCY() + View180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View225Shd[i].CreateSprite(ID);
		if (MaxWidth < View225Shd[i].GetWidth()) MaxWidth = View225Shd[i].GetWidth();
		if (MaxHeight < View225Shd[i].GetHeight()) MaxHeight = View225Shd[i].GetHeight();
		if (MaxxCor > View225Shd[i].GetCX()) MaxxCor = View225Shd[i].GetCX();
		if (MaxyCor > View225Shd[i].GetCY()) MaxyCor = View225Shd[i].GetCY();
		if (ChoseX < View225Shd[i].GetCX() + View225Shd[i].GetWidth()) ChoseX = View225Shd[i].GetCX() + View225Shd[i].GetWidth();
		if (ChoseY < View225Shd[i].GetCY() + View225Shd[i].GetHeight()) ChoseY = View225Shd[i].GetCY() + View225Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View270Shd[i].CreateSprite(ID);
		if (MaxWidth < View270Shd[i].GetWidth()) MaxWidth = View270Shd[i].GetWidth();
		if (MaxHeight < View270Shd[i].GetHeight()) MaxHeight = View270Shd[i].GetHeight();
		if (MaxxCor > View270Shd[i].GetCX()) MaxxCor = View270Shd[i].GetCX();
		if (MaxyCor > View270Shd[i].GetCY()) MaxyCor = View270Shd[i].GetCY();
		if (ChoseX < View270Shd[i].GetCX() + View270Shd[i].GetWidth()) ChoseX = View270Shd[i].GetCX() + View270Shd[i].GetWidth();
		if (ChoseY < View270Shd[i].GetCY() + View270Shd[i].GetHeight()) ChoseY = View270Shd[i].GetCY() + View270Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View315Shd[i].CreateSprite(ID);
		if (MaxWidth < View315Shd[i].GetWidth()) MaxWidth = View315Shd[i].GetWidth();
		if (MaxHeight < View315Shd[i].GetHeight()) MaxHeight = View315Shd[i].GetHeight();
		if (MaxxCor > View315Shd[i].GetCX()) MaxxCor = View315Shd[i].GetCX();
		if (MaxyCor > View315Shd[i].GetCY()) MaxyCor = View315Shd[i].GetCY();
		if (ChoseX < View315Shd[i].GetCX() + View315Shd[i].GetWidth()) ChoseX = View315Shd[i].GetCX() + View315Shd[i].GetWidth();
		if (ChoseY < View315Shd[i].GetCY() + View315Shd[i].GetHeight()) ChoseY = View315Shd[i].GetCY() + View315Shd[i].GetHeight();
	}

	for (i = 0; i < faces2; i++) {
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att000Shd[i].CreateSprite(ID);
		if (MaxWidth < Att000Shd[i].GetWidth()) MaxWidth = Att000Shd[i].GetWidth();
		if (MaxHeight < Att000Shd[i].GetHeight()) MaxHeight = Att000Shd[i].GetHeight();
		if (MaxxCor > Att000Shd[i].GetCX()) MaxxCor = Att000Shd[i].GetCX();
		if (MaxyCor > Att000Shd[i].GetCY()) MaxyCor = Att000Shd[i].GetCY();
		if (ChoseX < Att000Shd[i].GetCX() + Att000Shd[i].GetWidth()) ChoseX = Att000Shd[i].GetCX() + Att000Shd[i].GetWidth();
		if (ChoseY < Att000Shd[i].GetCY() + Att000Shd[i].GetHeight()) ChoseY = Att000Shd[i].GetCY() + Att000Shd[i].GetHeight();

		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att045Shd[i].CreateSprite(ID);
		if (MaxWidth < Att045Shd[i].GetWidth()) MaxWidth = Att045Shd[i].GetWidth();
		if (MaxHeight < Att045Shd[i].GetHeight()) MaxHeight = Att045Shd[i].GetHeight();
		if (MaxxCor > Att045Shd[i].GetCX()) MaxxCor = Att045Shd[i].GetCX();
		if (MaxyCor > Att045Shd[i].GetCY()) MaxyCor = Att045Shd[i].GetCY();
		if (ChoseX < Att045Shd[i].GetCX() + Att045Shd[i].GetWidth()) ChoseX = Att045Shd[i].GetCX() + Att045Shd[i].GetWidth();
		if (ChoseY < Att045Shd[i].GetCY() + Att045Shd[i].GetHeight()) ChoseY = Att045Shd[i].GetCY() + Att045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att090Shd[i].CreateSprite(ID);
		if (MaxWidth < Att090Shd[i].GetWidth()) MaxWidth = Att090Shd[i].GetWidth();
		if (MaxHeight < Att090Shd[i].GetHeight()) MaxHeight = Att090Shd[i].GetHeight();
		if (MaxxCor > Att090Shd[i].GetCX()) MaxxCor = Att090Shd[i].GetCX();
		if (MaxyCor > Att090Shd[i].GetCY()) MaxyCor = Att090Shd[i].GetCY();
		if (ChoseX < Att090Shd[i].GetCX() + Att090Shd[i].GetWidth()) ChoseX = Att090Shd[i].GetCX() + Att090Shd[i].GetWidth();
		if (ChoseY < Att090Shd[i].GetCY() + Att090Shd[i].GetHeight()) ChoseY = Att090Shd[i].GetCY() + Att090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att135Shd[i].CreateSprite(ID);
		if (MaxWidth < Att135Shd[i].GetWidth()) MaxWidth = Att135Shd[i].GetWidth();
		if (MaxHeight < Att135Shd[i].GetHeight()) MaxHeight = Att135Shd[i].GetHeight();
		if (MaxxCor > Att135Shd[i].GetCX()) MaxxCor = Att135Shd[i].GetCX();
		if (MaxyCor > Att135Shd[i].GetCY()) MaxyCor = Att135Shd[i].GetCY();
		if (ChoseX < Att135Shd[i].GetCX() + Att135Shd[i].GetWidth()) ChoseX = Att135Shd[i].GetCX() + Att135Shd[i].GetWidth();
		if (ChoseY < Att135Shd[i].GetCY() + Att135Shd[i].GetHeight()) ChoseY = Att135Shd[i].GetCY() + Att135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att180Shd[i].CreateSprite(ID);
		if (MaxWidth < Att180Shd[i].GetWidth()) MaxWidth = Att180Shd[i].GetWidth();
		if (MaxHeight < Att180Shd[i].GetHeight()) MaxHeight = Att180Shd[i].GetHeight();
		if (MaxxCor > Att180Shd[i].GetCX()) MaxxCor = Att180Shd[i].GetCX();
		if (MaxyCor > Att180Shd[i].GetCY()) MaxyCor = Att180Shd[i].GetCY();
		if (ChoseX < Att180Shd[i].GetCX() + Att180Shd[i].GetWidth()) ChoseX = Att180Shd[i].GetCX() + Att180Shd[i].GetWidth();
		if (ChoseY < Att180Shd[i].GetCY() + Att180Shd[i].GetHeight()) ChoseY = Att180Shd[i].GetCY() + Att180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att225Shd[i].CreateSprite(ID);
		if (MaxWidth < Att225Shd[i].GetWidth()) MaxWidth = Att225Shd[i].GetWidth();
		if (MaxHeight < Att225Shd[i].GetHeight()) MaxHeight = Att225Shd[i].GetHeight();
		if (MaxxCor > Att225Shd[i].GetCX()) MaxxCor = Att225Shd[i].GetCX();
		if (MaxyCor > Att225Shd[i].GetCY()) MaxyCor = Att225Shd[i].GetCY();
		if (ChoseX < Att225Shd[i].GetCX() + Att225Shd[i].GetWidth()) ChoseX = Att225Shd[i].GetCX() + Att225Shd[i].GetWidth();
		if (ChoseY < Att225Shd[i].GetCY() + Att225Shd[i].GetHeight()) ChoseY = Att225Shd[i].GetCY() + Att225Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att270Shd[i].CreateSprite(ID);
		if (MaxWidth < Att270Shd[i].GetWidth()) MaxWidth = Att270Shd[i].GetWidth();
		if (MaxHeight < Att270Shd[i].GetHeight()) MaxHeight = Att270Shd[i].GetHeight();
		if (MaxxCor > Att270Shd[i].GetCX()) MaxxCor = Att270Shd[i].GetCX();
		if (MaxyCor > Att270Shd[i].GetCY()) MaxyCor = Att270Shd[i].GetCY();
		if (ChoseX < Att270Shd[i].GetCX() + Att270Shd[i].GetWidth()) ChoseX = Att270Shd[i].GetCX() + Att270Shd[i].GetWidth();
		if (ChoseY < Att270Shd[i].GetCY() + Att270Shd[i].GetHeight()) ChoseY = Att270Shd[i].GetCY() + Att270Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att315Shd[i].CreateSprite(ID);
		if (MaxWidth < Att315Shd[i].GetWidth()) MaxWidth = Att315Shd[i].GetWidth();
		if (MaxHeight < Att315Shd[i].GetHeight()) MaxHeight = Att315Shd[i].GetHeight();
		if (MaxxCor > Att315Shd[i].GetCX()) MaxxCor = Att315Shd[i].GetCX();
		if (MaxyCor > Att315Shd[i].GetCY()) MaxyCor = Att315Shd[i].GetCY();
		if (ChoseX < Att315Shd[i].GetCX() + Att315Shd[i].GetWidth()) ChoseX = Att315Shd[i].GetCX() + Att315Shd[i].GetWidth();
		if (ChoseY < Att315Shd[i].GetCY() + Att315Shd[i].GetHeight()) ChoseY = Att315Shd[i].GetCY() + Att315Shd[i].GetHeight();
		
	}

   for (i = 0; i < faces3; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StAtt000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt000Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt000Shd[i].GetWidth()) MaxWidth = StAtt000Shd[i].GetWidth();
		if (MaxHeight < StAtt000Shd[i].GetHeight()) MaxHeight = StAtt000Shd[i].GetHeight();
		if (MaxxCor > StAtt000Shd[i].GetCX()) MaxxCor = StAtt000Shd[i].GetCX();
		if (MaxyCor > StAtt000Shd[i].GetCY()) MaxyCor = StAtt000Shd[i].GetCY();
		if (ChoseX < StAtt000Shd[i].GetCX() + StAtt000Shd[i].GetWidth()) ChoseX = StAtt000Shd[i].GetCX() + StAtt000Shd[i].GetWidth();
		if (ChoseY < StAtt000Shd[i].GetCY() + StAtt000Shd[i].GetHeight()) ChoseY = StAtt000Shd[i].GetCY() + StAtt000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StAtt045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt045Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt045Shd[i].GetWidth()) MaxWidth = StAtt045Shd[i].GetWidth();
		if (MaxHeight < StAtt045Shd[i].GetHeight()) MaxHeight = StAtt045Shd[i].GetHeight();
		if (MaxxCor > StAtt045Shd[i].GetCX()) MaxxCor = StAtt045Shd[i].GetCX();
		if (MaxyCor > StAtt045Shd[i].GetCY()) MaxyCor = StAtt045Shd[i].GetCY();
		if (ChoseX < StAtt045Shd[i].GetCX() + StAtt045Shd[i].GetWidth()) ChoseX = StAtt045Shd[i].GetCX() + StAtt045Shd[i].GetWidth();
		if (ChoseY < StAtt045Shd[i].GetCY() + StAtt045Shd[i].GetHeight()) ChoseY = StAtt045Shd[i].GetCY() + StAtt045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt090Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt090Shd[i].GetWidth()) MaxWidth = StAtt090Shd[i].GetWidth();
		if (MaxHeight < StAtt090Shd[i].GetHeight()) MaxHeight = StAtt090Shd[i].GetHeight();
		if (MaxxCor > StAtt090Shd[i].GetCX()) MaxxCor = StAtt090Shd[i].GetCX();
		if (MaxyCor > StAtt090Shd[i].GetCY()) MaxyCor = StAtt090Shd[i].GetCY();
		if (ChoseX < StAtt090Shd[i].GetCX() + StAtt090Shd[i].GetWidth()) ChoseX = StAtt090Shd[i].GetCX() + StAtt090Shd[i].GetWidth();
		if (ChoseY < StAtt090Shd[i].GetCY() + StAtt090Shd[i].GetHeight()) ChoseY = StAtt090Shd[i].GetCY() + StAtt090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt135Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt135Shd[i].GetWidth()) MaxWidth = StAtt135Shd[i].GetWidth();
		if (MaxHeight < StAtt135Shd[i].GetHeight()) MaxHeight = StAtt135Shd[i].GetHeight();
		if (MaxxCor > StAtt135Shd[i].GetCX()) MaxxCor = StAtt135Shd[i].GetCX();
		if (MaxyCor > StAtt135Shd[i].GetCY()) MaxyCor = StAtt135Shd[i].GetCY();
		if (ChoseX < StAtt135Shd[i].GetCX() + StAtt135Shd[i].GetWidth()) ChoseX = StAtt135Shd[i].GetCX() + StAtt135Shd[i].GetWidth();
		if (ChoseY < StAtt135Shd[i].GetCY() + StAtt135Shd[i].GetHeight()) ChoseY = StAtt135Shd[i].GetCY() + StAtt135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt180Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt180Shd[i].GetWidth()) MaxWidth = StAtt180Shd[i].GetWidth();
		if (MaxHeight < StAtt180Shd[i].GetHeight()) MaxHeight = StAtt180Shd[i].GetHeight();
		if (MaxxCor > StAtt180Shd[i].GetCX()) MaxxCor = StAtt180Shd[i].GetCX();
		if (MaxyCor > StAtt180Shd[i].GetCY()) MaxyCor = StAtt180Shd[i].GetCY();
		if (ChoseX < StAtt180Shd[i].GetCX() + StAtt180Shd[i].GetWidth()) ChoseX = StAtt180Shd[i].GetCX() + StAtt180Shd[i].GetWidth();
		if (ChoseY < StAtt180Shd[i].GetCY() + StAtt180Shd[i].GetHeight()) ChoseY = StAtt180Shd[i].GetCY() + StAtt180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt225Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt225Shd[i].GetWidth()) MaxWidth = StAtt225Shd[i].GetWidth();
		if (MaxHeight < StAtt225Shd[i].GetHeight()) MaxHeight = StAtt225Shd[i].GetHeight();
		if (MaxxCor > StAtt225Shd[i].GetCX()) MaxxCor = StAtt225Shd[i].GetCX();
		if (MaxyCor > StAtt225Shd[i].GetCY()) MaxyCor = StAtt225Shd[i].GetCY();
		if (ChoseX < StAtt225Shd[i].GetCX() + StAtt225Shd[i].GetWidth()) ChoseX = StAtt225Shd[i].GetCX() + StAtt225Shd[i].GetWidth();
		if (ChoseY < StAtt225Shd[i].GetCY() + StAtt225Shd[i].GetHeight()) ChoseY = StAtt225Shd[i].GetCY() + StAtt225Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt270Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt270Shd[i].GetWidth()) MaxWidth = StAtt270Shd[i].GetWidth();
		if (MaxHeight < StAtt270Shd[i].GetHeight()) MaxHeight = StAtt270Shd[i].GetHeight();
		if (MaxxCor > StAtt270Shd[i].GetCX()) MaxxCor = StAtt270Shd[i].GetCX();
		if (MaxyCor > StAtt270Shd[i].GetCY()) MaxyCor = StAtt270Shd[i].GetCY();
		if (ChoseX < StAtt270Shd[i].GetCX() + StAtt270Shd[i].GetWidth()) ChoseX = StAtt270Shd[i].GetCX() + StAtt270Shd[i].GetWidth();
		if (ChoseY < StAtt270Shd[i].GetCY() + StAtt270Shd[i].GetHeight()) ChoseY = StAtt270Shd[i].GetCY() + StAtt270Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt315Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt315Shd[i].GetWidth()) MaxWidth = StAtt315Shd[i].GetWidth();
		if (MaxHeight < StAtt315Shd[i].GetHeight()) MaxHeight = StAtt315Shd[i].GetHeight();
		if (MaxxCor > StAtt315Shd[i].GetCX()) MaxxCor = StAtt315Shd[i].GetCX();
		if (MaxyCor > StAtt315Shd[i].GetCY()) MaxyCor = StAtt315Shd[i].GetCY();
		if (ChoseX < StAtt315Shd[i].GetCX() + StAtt315Shd[i].GetWidth()) ChoseX = StAtt315Shd[i].GetCX() + StAtt315Shd[i].GetWidth();
		if (ChoseY < StAtt315Shd[i].GetCY() + StAtt315Shd[i].GetHeight()) ChoseY = StAtt315Shd[i].GetCY() + StAtt315Shd[i].GetHeight();
		
	}

	for (i = 0; i < faces4; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StMov000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov000Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov000Shd[i].GetWidth()) MaxWidth = StMov000Shd[i].GetWidth();
		if (MaxHeight < StMov000Shd[i].GetHeight()) MaxHeight = StMov000Shd[i].GetHeight();
		if (MaxxCor > StMov000Shd[i].GetCX()) MaxxCor = StMov000Shd[i].GetCX();
		if (MaxyCor > StMov000Shd[i].GetCY()) MaxyCor = StMov000Shd[i].GetCY();
		if (ChoseX < StMov000Shd[i].GetCX() + StMov000Shd[i].GetWidth()) ChoseX = StMov000Shd[i].GetCX() + StMov000Shd[i].GetWidth();
		if (ChoseY < StMov000Shd[i].GetCY() + StMov000Shd[i].GetHeight()) ChoseY = StMov000Shd[i].GetCY() + StMov000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StMov045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov045Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov045Shd[i].GetWidth()) MaxWidth = StMov045Shd[i].GetWidth();
		if (MaxHeight < StMov045Shd[i].GetHeight()) MaxHeight = StMov045Shd[i].GetHeight();
		if (MaxxCor > StMov045Shd[i].GetCX()) MaxxCor = StMov045Shd[i].GetCX();
		if (MaxyCor > StMov045Shd[i].GetCY()) MaxyCor = StMov045Shd[i].GetCY();
		if (ChoseX < StMov045Shd[i].GetCX() + StMov045Shd[i].GetWidth()) ChoseX = StMov045Shd[i].GetCX() + StMov045Shd[i].GetWidth();
		if (ChoseY < StMov045Shd[i].GetCY() + StMov045Shd[i].GetHeight()) ChoseY = StMov045Shd[i].GetCY() + StMov045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov090Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov090Shd[i].GetWidth()) MaxWidth = StMov090Shd[i].GetWidth();
		if (MaxHeight < StMov090Shd[i].GetHeight()) MaxHeight = StMov090Shd[i].GetHeight();
		if (MaxxCor > StMov090Shd[i].GetCX()) MaxxCor = StMov090Shd[i].GetCX();
		if (MaxyCor > StMov090Shd[i].GetCY()) MaxyCor = StMov090Shd[i].GetCY();
		if (ChoseX < StMov090Shd[i].GetCX() + StMov090Shd[i].GetWidth()) ChoseX = StMov090Shd[i].GetCX() + StMov090Shd[i].GetWidth();
		if (ChoseY < StMov090Shd[i].GetCY() + StMov090Shd[i].GetHeight()) ChoseY = StMov090Shd[i].GetCY() + StMov090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov135Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov135Shd[i].GetWidth()) MaxWidth = StMov135Shd[i].GetWidth();
		if (MaxHeight < StMov135Shd[i].GetHeight()) MaxHeight = StMov135Shd[i].GetHeight();
		if (MaxxCor > StMov135Shd[i].GetCX()) MaxxCor = StMov135Shd[i].GetCX();
		if (MaxyCor > StMov135Shd[i].GetCY()) MaxyCor = StMov135Shd[i].GetCY();
		if (ChoseX < StMov135Shd[i].GetCX() + StMov135Shd[i].GetWidth()) ChoseX = StMov135Shd[i].GetCX() + StMov135Shd[i].GetWidth();
		if (ChoseY < StMov135Shd[i].GetCY() + StMov135Shd[i].GetHeight()) ChoseY = StMov135Shd[i].GetCY() + StMov135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov180Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov180Shd[i].GetWidth()) MaxWidth = StMov180Shd[i].GetWidth();
		if (MaxHeight < StMov180Shd[i].GetHeight()) MaxHeight = StMov180Shd[i].GetHeight();
		if (MaxxCor > StMov180Shd[i].GetCX()) MaxxCor = StMov180Shd[i].GetCX();
		if (MaxyCor > StMov180Shd[i].GetCY()) MaxyCor = StMov180Shd[i].GetCY();
		if (ChoseX < StMov180Shd[i].GetCX() + StMov180Shd[i].GetWidth()) ChoseX = StMov180Shd[i].GetCX() + StMov180Shd[i].GetWidth();
		if (ChoseY < StMov180Shd[i].GetCY() + StMov180Shd[i].GetHeight()) ChoseY = StMov180Shd[i].GetCY() + StMov180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov225Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov225Shd[i].GetWidth()) MaxWidth = StMov225Shd[i].GetWidth();
		if (MaxHeight < StMov225Shd[i].GetHeight()) MaxHeight = StMov225Shd[i].GetHeight();
		if (MaxxCor > StMov225Shd[i].GetCX()) MaxxCor = StMov225Shd[i].GetCX();
		if (MaxyCor > StMov225Shd[i].GetCY()) MaxyCor = StMov225Shd[i].GetCY();
		if (ChoseX < StMov225Shd[i].GetCX() + StMov225Shd[i].GetWidth()) ChoseX = StMov225Shd[i].GetCX() + StMov225Shd[i].GetWidth();
		if (ChoseY < StMov225Shd[i].GetCY() + StMov225Shd[i].GetHeight()) ChoseY = StMov225Shd[i].GetCY() + StMov225Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov270Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov270Shd[i].GetWidth()) MaxWidth = StMov270Shd[i].GetWidth();
		if (MaxHeight < StMov270Shd[i].GetHeight()) MaxHeight = StMov270Shd[i].GetHeight();
		if (MaxxCor > StMov270Shd[i].GetCX()) MaxxCor = StMov270Shd[i].GetCX();
		if (MaxyCor > StMov270Shd[i].GetCY()) MaxyCor = StMov270Shd[i].GetCY();
		if (ChoseX < StMov270Shd[i].GetCX() + StMov270Shd[i].GetWidth()) ChoseX = StMov270Shd[i].GetCX() + StMov270Shd[i].GetWidth();
		if (ChoseY < StMov270Shd[i].GetCY() + StMov270Shd[i].GetHeight()) ChoseY = StMov270Shd[i].GetCY() + StMov270Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov315Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov315Shd[i].GetWidth()) MaxWidth = StMov315Shd[i].GetWidth();
		if (MaxHeight < StMov315Shd[i].GetHeight()) MaxHeight = StMov315Shd[i].GetHeight();
		if (MaxxCor > StMov315Shd[i].GetCX()) MaxxCor = StMov315Shd[i].GetCX();
		if (MaxyCor > StMov315Shd[i].GetCY()) MaxyCor = StMov315Shd[i].GetCY();
		if (ChoseX < StMov315Shd[i].GetCX() + StMov315Shd[i].GetWidth()) ChoseX = StMov315Shd[i].GetCX() + StMov315Shd[i].GetWidth();
		if (ChoseY < StMov315Shd[i].GetCY() + StMov315Shd[i].GetHeight()) ChoseY = StMov315Shd[i].GetCY() + StMov315Shd[i].GetHeight();
		
	}
*/
	fTransparent = View000[0].GetfTransparent();
	TransColor = View000[0].GetTransColor();
   if ((MaxxCor % 2)) {
      MaxxCor--;
      MaxWidth++;
   }
   if (!(MaxyCor % 2)) {
      MaxyCor--;
      MaxHeight++;
   }

	MaxWidth += ChoseX - (MaxxCor + MaxWidth);
	MaxHeight += ChoseY - (MaxyCor + MaxHeight);

   if (Type == 3)
      Type = 0;

	delete ID;
	delete Temp;
	delete ViewIDA;
	delete ViewIDB;
	delete ViewIDStAtt;
	delete ViewIDStMov;
	delete ViewIDC;
}


void Sprite3D::LoadSprite3D2(int faces, int faces2, int faces3, int faces4, int faces5, char *ViewID, int type, int stAtt, int stMov, int palID) {
   if (bAlreadyLoad)
		return;
	bAlreadyLoad = TRUE;

	strcpy(NAME, ViewID);

   LastAccess = timeGetTime();
   // Load a 3D Sprite
	char *ID = new char [80];
	char *Temp = new char [80];
	char Temp2[2];
	Temp2[1] = 0;
	
	Type = type;
	faces /= FRAMING;
	faces2 /= FRAMING;
	Faces = faces;
	FacesA = faces2;
	FacesC = faces5;
   StAtt = stAtt;
   StMov = stMov;
   FacesStA = faces3;
   FacesStM = faces4;
	
	View000 = new CV2Sprite [faces];
	View045 = new CV2Sprite [faces];
	View090 = new CV2Sprite [faces];
	View135 = new CV2Sprite [faces];
	View180 = new CV2Sprite [faces];

	Att000 = new CV2Sprite [faces2];
	Att045 = new CV2Sprite [faces2];
	Att090 = new CV2Sprite [faces2];
	Att135 = new CV2Sprite [faces2];
	Att180 = new CV2Sprite [faces2];

   if (faces5)
		Corpse = new CV2Sprite[faces5];

	View000Shd = new CV2Sprite [faces];
	View045Shd = new CV2Sprite [faces];
	View090Shd = new CV2Sprite [faces];
	View135Shd = new CV2Sprite [faces];
	View180Shd = new CV2Sprite [faces];
	View225Shd = new CV2Sprite [faces];
	View270Shd = new CV2Sprite [faces];
	View315Shd = new CV2Sprite [faces];

	Att000Shd = new CV2Sprite [faces2];
	Att045Shd = new CV2Sprite [faces2];
	Att090Shd = new CV2Sprite [faces2];
	Att135Shd = new CV2Sprite [faces2];
	Att180Shd = new CV2Sprite [faces2];
	Att225Shd = new CV2Sprite [faces2];
	Att270Shd = new CV2Sprite [faces2];
	Att315Shd = new CV2Sprite [faces2];

//   CorpseShd = new CV2Sprite[faces5];

   if (StAtt == -1) {
      StAtt000 = new CV2Sprite [faces3];
      StAtt045 = new CV2Sprite [faces3];
      StAtt090 = new CV2Sprite [faces3];
      StAtt135 = new CV2Sprite [faces3];
      StAtt180 = new CV2Sprite [faces3];
      StAtt000Shd = new CV2Sprite [faces3];
      StAtt045Shd = new CV2Sprite [faces3];
      StAtt090Shd = new CV2Sprite [faces3];
      StAtt135Shd = new CV2Sprite [faces3];
      StAtt180Shd = new CV2Sprite [faces3];
      StAtt225Shd = new CV2Sprite [faces3];
      StAtt270Shd = new CV2Sprite [faces3];
      StAtt315Shd = new CV2Sprite [faces3];
   }
   
   if (StMov == -1) {
      StMov000 = new CV2Sprite [faces4];
      StMov045 = new CV2Sprite [faces4];
      StMov090 = new CV2Sprite [faces4];
      StMov135 = new CV2Sprite [faces4];
      StMov180 = new CV2Sprite [faces4];
      StMov000Shd = new CV2Sprite [faces4];
      StMov045Shd = new CV2Sprite [faces4];
      StMov090Shd = new CV2Sprite [faces4];
      StMov135Shd = new CV2Sprite [faces4];
      StMov180Shd = new CV2Sprite [faces4];
      StMov225Shd = new CV2Sprite [faces4];
      StMov270Shd = new CV2Sprite [faces4];
      StMov315Shd = new CV2Sprite [faces4];
   }

	int ChoseX = 0;
	int ChoseY = 0;
   int iBC = 0;

   for (int i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      iBC = Corpse[i].CreateSprite(ID, iBC, palID);
   }
	
/*   for (i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      strcat(ID, "Shd");
      CorpseShd[i].CreateSprite(ID);
   } */
	
	int iB000 = 0, iB045 = 0, iB090 = 0, iB135 = 0, iB180 = 0;
   for (int i = 0; i < faces; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = View000[i].CreateSprite(ID, iB000, palID);
		if (MaxWidth < View000[i].GetWidth()) MaxWidth = View000[i].GetWidth();
		if (MaxHeight < View000[i].GetHeight()) MaxHeight = View000[i].GetHeight();
		if (MaxxCor > View000[i].GetCX()) MaxxCor = View000[i].GetCX();
		if (MaxyCor > View000[i].GetCY()) MaxyCor = View000[i].GetCY();
		if (ChoseX < View000[i].GetCX() + View000[i].GetWidth()) ChoseX = View000[i].GetCX() + View000[i].GetWidth();
		if (ChoseY < View000[i].GetCY() + View000[i].GetHeight()) ChoseY = View000[i].GetCY() + View000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = View045[i].CreateSprite(ID, iB045, palID);
		if (MaxWidth < View045[i].GetWidth()) MaxWidth = View045[i].GetWidth();
		if (MaxHeight < View045[i].GetHeight()) MaxHeight = View045[i].GetHeight();
		if (MaxxCor > View045[i].GetCX()) MaxxCor = View045[i].GetCX();
		if (MaxyCor > View045[i].GetCY()) MaxyCor = View045[i].GetCY();
		if (ChoseX < View045[i].GetCX() + View045[i].GetWidth()) ChoseX = View045[i].GetCX() + View045[i].GetWidth();
		if (ChoseY < View045[i].GetCY() + View045[i].GetHeight()) ChoseY = View045[i].GetCY() + View045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = View090[i].CreateSprite(ID, iB090, palID);
		if (MaxWidth < View090[i].GetWidth()) MaxWidth = View090[i].GetWidth();
		if (MaxHeight < View090[i].GetHeight()) MaxHeight = View090[i].GetHeight();
		if (MaxxCor > View090[i].GetCX()) MaxxCor = View090[i].GetCX();
		if (MaxyCor > View090[i].GetCY()) MaxyCor = View090[i].GetCY();
		if (ChoseX < View090[i].GetCX() + View090[i].GetWidth()) ChoseX = View090[i].GetCX() + View090[i].GetWidth();
		if (ChoseY < View090[i].GetCY() + View090[i].GetHeight()) ChoseY = View090[i].GetCY() + View090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = View135[i].CreateSprite(ID, iB135, palID);
		if (MaxWidth < View135[i].GetWidth()) MaxWidth = View135[i].GetWidth();
		if (MaxHeight < View135[i].GetHeight()) MaxHeight = View135[i].GetHeight();
		if (MaxxCor > View135[i].GetCX()) MaxxCor = View135[i].GetCX();
		if (MaxyCor > View135[i].GetCY()) MaxyCor = View135[i].GetCY();
		if (ChoseX < View135[i].GetCX() + View135[i].GetWidth()) ChoseX = View135[i].GetCX() + View135[i].GetWidth();
		if (ChoseY < View135[i].GetCY() + View135[i].GetHeight()) ChoseY = View135[i].GetCY() + View135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = View180[i].CreateSprite(ID, iB180, palID);
		if (MaxWidth < View180[i].GetWidth()) MaxWidth = View180[i].GetWidth();
		if (MaxHeight < View180[i].GetHeight()) MaxHeight = View180[i].GetHeight();
		if (MaxxCor > View180[i].GetCX()) MaxxCor = View180[i].GetCX();
		if (MaxyCor > View180[i].GetCY()) MaxyCor = View180[i].GetCY();
		if (ChoseX < View180[i].GetCX() + View180[i].GetWidth()) ChoseX = View180[i].GetCX() + View180[i].GetWidth();
		if (ChoseY < View180[i].GetCY() + View180[i].GetHeight()) ChoseY = View180[i].GetCY() + View180[i].GetHeight();
		
	}

	iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces2; i++) {
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = Att000[i].CreateSprite(ID, iB000, palID);
		if (MaxWidth < Att000[i].GetWidth()) MaxWidth = Att000[i].GetWidth();
		if (MaxHeight < Att000[i].GetHeight()) MaxHeight = Att000[i].GetHeight();
		if (MaxxCor > Att000[i].GetCX()) MaxxCor = Att000[i].GetCX();
		if (MaxyCor > Att000[i].GetCY()) MaxyCor = Att000[i].GetCY();
		if (ChoseX < Att000[i].GetCX() + Att000[i].GetWidth()) ChoseX = Att000[i].GetCX() + Att000[i].GetWidth();
		if (ChoseY < Att000[i].GetCY() + Att000[i].GetHeight()) ChoseY = Att000[i].GetCY() + Att000[i].GetHeight();

		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = Att045[i].CreateSprite(ID, iB045, palID);
		if (MaxWidth < Att045[i].GetWidth()) MaxWidth = Att045[i].GetWidth();
		if (MaxHeight < Att045[i].GetHeight()) MaxHeight = Att045[i].GetHeight();
		if (MaxxCor > Att045[i].GetCX()) MaxxCor = Att045[i].GetCX();
		if (MaxyCor > Att045[i].GetCY()) MaxyCor = Att045[i].GetCY();
		if (ChoseX < Att045[i].GetCX() + Att045[i].GetWidth()) ChoseX = Att045[i].GetCX() + Att045[i].GetWidth();
		if (ChoseY < Att045[i].GetCY() + Att045[i].GetHeight()) ChoseY = Att045[i].GetCY() + Att045[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = Att090[i].CreateSprite(ID, iB090, palID);
		if (MaxWidth < Att090[i].GetWidth()) MaxWidth = Att090[i].GetWidth();
		if (MaxHeight < Att090[i].GetHeight()) MaxHeight = Att090[i].GetHeight();
		if (MaxxCor > Att090[i].GetCX()) MaxxCor = Att090[i].GetCX();
		if (MaxyCor > Att090[i].GetCY()) MaxyCor = Att090[i].GetCY();
		if (ChoseX < Att090[i].GetCX() + Att090[i].GetWidth()) ChoseX = Att090[i].GetCX() + Att090[i].GetWidth();
		if (ChoseY < Att090[i].GetCY() + Att090[i].GetHeight()) ChoseY = Att090[i].GetCY() + Att090[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = Att135[i].CreateSprite(ID, iB135, palID);
		if (MaxWidth < Att135[i].GetWidth()) MaxWidth = Att135[i].GetWidth();
		if (MaxHeight < Att135[i].GetHeight()) MaxHeight = Att135[i].GetHeight();
		if (MaxxCor > Att135[i].GetCX()) MaxxCor = Att135[i].GetCX();
		if (MaxyCor > Att135[i].GetCY()) MaxyCor = Att135[i].GetCY();
		if (ChoseX < Att135[i].GetCX() + Att135[i].GetWidth()) ChoseX = Att135[i].GetCX() + Att135[i].GetWidth();
		if (ChoseY < Att135[i].GetCY() + Att135[i].GetHeight()) ChoseY = Att135[i].GetCY() + Att135[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = Att180[i].CreateSprite(ID, iB180, palID);
		if (MaxWidth < Att180[i].GetWidth()) MaxWidth = Att180[i].GetWidth();
		if (MaxHeight < Att180[i].GetHeight()) MaxHeight = Att180[i].GetHeight();
		if (MaxxCor > Att180[i].GetCX()) MaxxCor = Att180[i].GetCX();
		if (MaxyCor > Att180[i].GetCY()) MaxyCor = Att180[i].GetCY();
		if (ChoseX < Att180[i].GetCX() + Att180[i].GetWidth()) ChoseX = Att180[i].GetCX() + Att180[i].GetWidth();
		if (ChoseY < Att180[i].GetCY() + Att180[i].GetHeight()) ChoseY = Att180[i].GetCY() + Att180[i].GetHeight();
		
	}

   iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces3; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StAtt000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = StAtt000[i].CreateSprite(ID, iB000, palID);
		if (MaxWidth < StAtt000[i].GetWidth()) MaxWidth = StAtt000[i].GetWidth();
		if (MaxHeight < StAtt000[i].GetHeight()) MaxHeight = StAtt000[i].GetHeight();
		if (MaxxCor > StAtt000[i].GetCX()) MaxxCor = StAtt000[i].GetCX();
		if (MaxyCor > StAtt000[i].GetCY()) MaxyCor = StAtt000[i].GetCY();
		if (ChoseX < StAtt000[i].GetCX() + StAtt000[i].GetWidth()) ChoseX = StAtt000[i].GetCX() + StAtt000[i].GetWidth();
		if (ChoseY < StAtt000[i].GetCY() + StAtt000[i].GetHeight()) ChoseY = StAtt000[i].GetCY() + StAtt000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StAtt045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = StAtt045[i].CreateSprite(ID, iB045, palID);
		if (MaxWidth < StAtt045[i].GetWidth()) MaxWidth = StAtt045[i].GetWidth();
		if (MaxHeight < StAtt045[i].GetHeight()) MaxHeight = StAtt045[i].GetHeight();
		if (MaxxCor > StAtt045[i].GetCX()) MaxxCor = StAtt045[i].GetCX();
		if (MaxyCor > StAtt045[i].GetCY()) MaxyCor = StAtt045[i].GetCY();
		if (ChoseX < StAtt045[i].GetCX() + StAtt045[i].GetWidth()) ChoseX = StAtt045[i].GetCX() + StAtt045[i].GetWidth();
		if (ChoseY < StAtt045[i].GetCY() + StAtt045[i].GetHeight()) ChoseY = StAtt045[i].GetCY() + StAtt045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = StAtt090[i].CreateSprite(ID, iB090, palID);
		if (MaxWidth < StAtt090[i].GetWidth()) MaxWidth = StAtt090[i].GetWidth();
		if (MaxHeight < StAtt090[i].GetHeight()) MaxHeight = StAtt090[i].GetHeight();
		if (MaxxCor > StAtt090[i].GetCX()) MaxxCor = StAtt090[i].GetCX();
		if (MaxyCor > StAtt090[i].GetCY()) MaxyCor = StAtt090[i].GetCY();
		if (ChoseX < StAtt090[i].GetCX() + StAtt090[i].GetWidth()) ChoseX = StAtt090[i].GetCX() + StAtt090[i].GetWidth();
		if (ChoseY < StAtt090[i].GetCY() + StAtt090[i].GetHeight()) ChoseY = StAtt090[i].GetCY() + StAtt090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = StAtt135[i].CreateSprite(ID, iB135, palID);
		if (MaxWidth < StAtt135[i].GetWidth()) MaxWidth = StAtt135[i].GetWidth();
		if (MaxHeight < StAtt135[i].GetHeight()) MaxHeight = StAtt135[i].GetHeight();
		if (MaxxCor > StAtt135[i].GetCX()) MaxxCor = StAtt135[i].GetCX();
		if (MaxyCor > StAtt135[i].GetCY()) MaxyCor = StAtt135[i].GetCY();
		if (ChoseX < StAtt135[i].GetCX() + StAtt135[i].GetWidth()) ChoseX = StAtt135[i].GetCX() + StAtt135[i].GetWidth();
		if (ChoseY < StAtt135[i].GetCY() + StAtt135[i].GetHeight()) ChoseY = StAtt135[i].GetCY() + StAtt135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = StAtt180[i].CreateSprite(ID, iB180, palID);
		if (MaxWidth < StAtt180[i].GetWidth()) MaxWidth = StAtt180[i].GetWidth();
		if (MaxHeight < StAtt180[i].GetHeight()) MaxHeight = StAtt180[i].GetHeight();
		if (MaxxCor > StAtt180[i].GetCX()) MaxxCor = StAtt180[i].GetCX();
		if (MaxyCor > StAtt180[i].GetCY()) MaxyCor = StAtt180[i].GetCY();
		if (ChoseX < StAtt180[i].GetCX() + StAtt180[i].GetWidth()) ChoseX = StAtt180[i].GetCX() + StAtt180[i].GetWidth();
		if (ChoseY < StAtt180[i].GetCY() + StAtt180[i].GetHeight()) ChoseY = StAtt180[i].GetCY() + StAtt180[i].GetHeight();
		
	}

	iB000 = 0; iB045 = 0; iB090 = 0; iB135 = 0; iB180 = 0;
   for (int i = 0; i < faces4; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StMov000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB000 = StMov000[i].CreateSprite(ID, iB000, palID);
		if (MaxWidth < StMov000[i].GetWidth()) MaxWidth = StMov000[i].GetWidth();
		if (MaxHeight < StMov000[i].GetHeight()) MaxHeight = StMov000[i].GetHeight();
		if (MaxxCor > StMov000[i].GetCX()) MaxxCor = StMov000[i].GetCX();
		if (MaxyCor > StMov000[i].GetCY()) MaxyCor = StMov000[i].GetCY();
		if (ChoseX < StMov000[i].GetCX() + StMov000[i].GetWidth()) ChoseX = StMov000[i].GetCX() + StMov000[i].GetWidth();
		if (ChoseY < StMov000[i].GetCY() + StMov000[i].GetHeight()) ChoseY = StMov000[i].GetCY() + StMov000[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StMov045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB045 = StMov045[i].CreateSprite(ID, iB045, palID);
		if (MaxWidth < StMov045[i].GetWidth()) MaxWidth = StMov045[i].GetWidth();
		if (MaxHeight < StMov045[i].GetHeight()) MaxHeight = StMov045[i].GetHeight();
		if (MaxxCor > StMov045[i].GetCX()) MaxxCor = StMov045[i].GetCX();
		if (MaxyCor > StMov045[i].GetCY()) MaxyCor = StMov045[i].GetCY();
		if (ChoseX < StMov045[i].GetCX() + StMov045[i].GetWidth()) ChoseX = StMov045[i].GetCX() + StMov045[i].GetWidth();
		if (ChoseY < StMov045[i].GetCY() + StMov045[i].GetHeight()) ChoseY = StMov045[i].GetCY() + StMov045[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB090 = StMov090[i].CreateSprite(ID, iB090, palID);
		if (MaxWidth < StMov090[i].GetWidth()) MaxWidth = StMov090[i].GetWidth();
		if (MaxHeight < StMov090[i].GetHeight()) MaxHeight = StMov090[i].GetHeight();
		if (MaxxCor > StMov090[i].GetCX()) MaxxCor = StMov090[i].GetCX();
		if (MaxyCor > StMov090[i].GetCY()) MaxyCor = StMov090[i].GetCY();
		if (ChoseX < StMov090[i].GetCX() + StMov090[i].GetWidth()) ChoseX = StMov090[i].GetCX() + StMov090[i].GetWidth();
		if (ChoseY < StMov090[i].GetCY() + StMov090[i].GetHeight()) ChoseY = StMov090[i].GetCY() + StMov090[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB135 = StMov135[i].CreateSprite(ID, iB135, palID);
		if (MaxWidth < StMov135[i].GetWidth()) MaxWidth = StMov135[i].GetWidth();
		if (MaxHeight < StMov135[i].GetHeight()) MaxHeight = StMov135[i].GetHeight();
		if (MaxxCor > StMov135[i].GetCX()) MaxxCor = StMov135[i].GetCX();
		if (MaxyCor > StMov135[i].GetCY()) MaxyCor = StMov135[i].GetCY();
		if (ChoseX < StMov135[i].GetCX() + StMov135[i].GetWidth()) ChoseX = StMov135[i].GetCX() + StMov135[i].GetWidth();
		if (ChoseY < StMov135[i].GetCY() + StMov135[i].GetHeight()) ChoseY = StMov135[i].GetCY() + StMov135[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
		iB180 = StMov180[i].CreateSprite(ID, iB180, palID);
		if (MaxWidth < StMov180[i].GetWidth()) MaxWidth = StMov180[i].GetWidth();
		if (MaxHeight < StMov180[i].GetHeight()) MaxHeight = StMov180[i].GetHeight();
		if (MaxxCor > StMov180[i].GetCX()) MaxxCor = StMov180[i].GetCX();
		if (MaxyCor > StMov180[i].GetCY()) MaxyCor = StMov180[i].GetCY();
		if (ChoseX < StMov180[i].GetCX() + StMov180[i].GetWidth()) ChoseX = StMov180[i].GetCX() + StMov180[i].GetWidth();
		if (ChoseY < StMov180[i].GetCY() + StMov180[i].GetHeight()) ChoseY = StMov180[i].GetCY() + StMov180[i].GetHeight();
		
	}

/*   for (i = 0; i < faces5; i++) {
	   strcpy(ID, ViewID);
   	strcat(ID, "C-");
      ID[strlen(ID)+1] = 0;
      ID[strlen(ID)] = i+'a';
      strcat(ID, "Shd");
      CorpseShd[i].CreateSprite(ID);
   } */
	
/*
   for (i = 0; i < faces; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View000Shd[i].CreateSprite(ID);
		if (MaxWidth < View000Shd[i].GetWidth()) MaxWidth = View000Shd[i].GetWidth();
		if (MaxHeight < View000Shd[i].GetHeight()) MaxHeight = View000Shd[i].GetHeight();
		if (MaxxCor > View000Shd[i].GetCX()) MaxxCor = View000Shd[i].GetCX();
		if (MaxyCor > View000Shd[i].GetCY()) MaxyCor = View000Shd[i].GetCY();
		if (ChoseX < View000Shd[i].GetCX() + View000Shd[i].GetWidth()) ChoseX = View000Shd[i].GetCX() + View000Shd[i].GetWidth();
		if (ChoseY < View000Shd[i].GetCY() + View000Shd[i].GetHeight()) ChoseY = View000Shd[i].GetCY() + View000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View045Shd[i].CreateSprite(ID);
		if (MaxWidth < View045Shd[i].GetWidth()) MaxWidth = View045Shd[i].GetWidth();
		if (MaxHeight < View045Shd[i].GetHeight()) MaxHeight = View045Shd[i].GetHeight();
		if (MaxxCor > View045Shd[i].GetCX()) MaxxCor = View045Shd[i].GetCX();
		if (MaxyCor > View045Shd[i].GetCY()) MaxyCor = View045Shd[i].GetCY();
		if (ChoseX < View045Shd[i].GetCX() + View045Shd[i].GetWidth()) ChoseX = View045Shd[i].GetCX() + View045Shd[i].GetWidth();
		if (ChoseY < View045Shd[i].GetCY() + View045Shd[i].GetHeight()) ChoseY = View045Shd[i].GetCY() + View045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View090Shd[i].CreateSprite(ID);
		if (MaxWidth < View090Shd[i].GetWidth()) MaxWidth = View090Shd[i].GetWidth();
		if (MaxHeight < View090Shd[i].GetHeight()) MaxHeight = View090Shd[i].GetHeight();
		if (MaxxCor > View090Shd[i].GetCX()) MaxxCor = View090Shd[i].GetCX();
		if (MaxyCor > View090Shd[i].GetCY()) MaxyCor = View090Shd[i].GetCY();
		if (ChoseX < View090Shd[i].GetCX() + View090Shd[i].GetWidth()) ChoseX = View090Shd[i].GetCX() + View090Shd[i].GetWidth();
		if (ChoseY < View090Shd[i].GetCY() + View090Shd[i].GetHeight()) ChoseY = View090Shd[i].GetCY() + View090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View135Shd[i].CreateSprite(ID);
		if (MaxWidth < View135Shd[i].GetWidth()) MaxWidth = View135Shd[i].GetWidth();
		if (MaxHeight < View135Shd[i].GetHeight()) MaxHeight = View135Shd[i].GetHeight();
		if (MaxxCor > View135Shd[i].GetCX()) MaxxCor = View135Shd[i].GetCX();
		if (MaxyCor > View135Shd[i].GetCY()) MaxyCor = View135Shd[i].GetCY();
		if (ChoseX < View135Shd[i].GetCX() + View135Shd[i].GetWidth()) ChoseX = View135Shd[i].GetCX() + View135Shd[i].GetWidth();
		if (ChoseY < View135Shd[i].GetCY() + View135Shd[i].GetHeight()) ChoseY = View135Shd[i].GetCY() + View135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View180Shd[i].CreateSprite(ID);
		if (MaxWidth < View180Shd[i].GetWidth()) MaxWidth = View180Shd[i].GetWidth();
		if (MaxHeight < View180Shd[i].GetHeight()) MaxHeight = View180Shd[i].GetHeight();
		if (MaxxCor > View180Shd[i].GetCX()) MaxxCor = View180Shd[i].GetCX();
		if (MaxyCor > View180Shd[i].GetCY()) MaxyCor = View180Shd[i].GetCY();
		if (ChoseX < View180Shd[i].GetCX() + View180Shd[i].GetWidth()) ChoseX = View180Shd[i].GetCX() + View180Shd[i].GetWidth();
		if (ChoseY < View180Shd[i].GetCY() + View180Shd[i].GetHeight()) ChoseY = View180Shd[i].GetCY() + View180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View225Shd[i].CreateSprite(ID);
		if (MaxWidth < View225Shd[i].GetWidth()) MaxWidth = View225Shd[i].GetWidth();
		if (MaxHeight < View225Shd[i].GetHeight()) MaxHeight = View225Shd[i].GetHeight();
		if (MaxxCor > View225Shd[i].GetCX()) MaxxCor = View225Shd[i].GetCX();
		if (MaxyCor > View225Shd[i].GetCY()) MaxyCor = View225Shd[i].GetCY();
		if (ChoseX < View225Shd[i].GetCX() + View225Shd[i].GetWidth()) ChoseX = View225Shd[i].GetCX() + View225Shd[i].GetWidth();
		if (ChoseY < View225Shd[i].GetCY() + View225Shd[i].GetHeight()) ChoseY = View225Shd[i].GetCY() + View225Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View270Shd[i].CreateSprite(ID);
		if (MaxWidth < View270Shd[i].GetWidth()) MaxWidth = View270Shd[i].GetWidth();
		if (MaxHeight < View270Shd[i].GetHeight()) MaxHeight = View270Shd[i].GetHeight();
		if (MaxxCor > View270Shd[i].GetCX()) MaxxCor = View270Shd[i].GetCX();
		if (MaxyCor > View270Shd[i].GetCY()) MaxyCor = View270Shd[i].GetCY();
		if (ChoseX < View270Shd[i].GetCX() + View270Shd[i].GetWidth()) ChoseX = View270Shd[i].GetCX() + View270Shd[i].GetWidth();
		if (ChoseY < View270Shd[i].GetCY() + View270Shd[i].GetHeight()) ChoseY = View270Shd[i].GetCY() + View270Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		View315Shd[i].CreateSprite(ID);
		if (MaxWidth < View315Shd[i].GetWidth()) MaxWidth = View315Shd[i].GetWidth();
		if (MaxHeight < View315Shd[i].GetHeight()) MaxHeight = View315Shd[i].GetHeight();
		if (MaxxCor > View315Shd[i].GetCX()) MaxxCor = View315Shd[i].GetCX();
		if (MaxyCor > View315Shd[i].GetCY()) MaxyCor = View315Shd[i].GetCY();
		if (ChoseX < View315Shd[i].GetCX() + View315Shd[i].GetWidth()) ChoseX = View315Shd[i].GetCX() + View315Shd[i].GetWidth();
		if (ChoseY < View315Shd[i].GetCY() + View315Shd[i].GetHeight()) ChoseY = View315Shd[i].GetCY() + View315Shd[i].GetHeight();
	}

	for (i = 0; i < faces2; i++) {
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att000Shd[i].CreateSprite(ID);
		if (MaxWidth < Att000Shd[i].GetWidth()) MaxWidth = Att000Shd[i].GetWidth();
		if (MaxHeight < Att000Shd[i].GetHeight()) MaxHeight = Att000Shd[i].GetHeight();
		if (MaxxCor > Att000Shd[i].GetCX()) MaxxCor = Att000Shd[i].GetCX();
		if (MaxyCor > Att000Shd[i].GetCY()) MaxyCor = Att000Shd[i].GetCY();
		if (ChoseX < Att000Shd[i].GetCX() + Att000Shd[i].GetWidth()) ChoseX = Att000Shd[i].GetCX() + Att000Shd[i].GetWidth();
		if (ChoseY < Att000Shd[i].GetCY() + Att000Shd[i].GetHeight()) ChoseY = Att000Shd[i].GetCY() + Att000Shd[i].GetHeight();

		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att045Shd[i].CreateSprite(ID);
		if (MaxWidth < Att045Shd[i].GetWidth()) MaxWidth = Att045Shd[i].GetWidth();
		if (MaxHeight < Att045Shd[i].GetHeight()) MaxHeight = Att045Shd[i].GetHeight();
		if (MaxxCor > Att045Shd[i].GetCX()) MaxxCor = Att045Shd[i].GetCX();
		if (MaxyCor > Att045Shd[i].GetCY()) MaxyCor = Att045Shd[i].GetCY();
		if (ChoseX < Att045Shd[i].GetCX() + Att045Shd[i].GetWidth()) ChoseX = Att045Shd[i].GetCX() + Att045Shd[i].GetWidth();
		if (ChoseY < Att045Shd[i].GetCY() + Att045Shd[i].GetHeight()) ChoseY = Att045Shd[i].GetCY() + Att045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att090Shd[i].CreateSprite(ID);
		if (MaxWidth < Att090Shd[i].GetWidth()) MaxWidth = Att090Shd[i].GetWidth();
		if (MaxHeight < Att090Shd[i].GetHeight()) MaxHeight = Att090Shd[i].GetHeight();
		if (MaxxCor > Att090Shd[i].GetCX()) MaxxCor = Att090Shd[i].GetCX();
		if (MaxyCor > Att090Shd[i].GetCY()) MaxyCor = Att090Shd[i].GetCY();
		if (ChoseX < Att090Shd[i].GetCX() + Att090Shd[i].GetWidth()) ChoseX = Att090Shd[i].GetCX() + Att090Shd[i].GetWidth();
		if (ChoseY < Att090Shd[i].GetCY() + Att090Shd[i].GetHeight()) ChoseY = Att090Shd[i].GetCY() + Att090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att135Shd[i].CreateSprite(ID);
		if (MaxWidth < Att135Shd[i].GetWidth()) MaxWidth = Att135Shd[i].GetWidth();
		if (MaxHeight < Att135Shd[i].GetHeight()) MaxHeight = Att135Shd[i].GetHeight();
		if (MaxxCor > Att135Shd[i].GetCX()) MaxxCor = Att135Shd[i].GetCX();
		if (MaxyCor > Att135Shd[i].GetCY()) MaxyCor = Att135Shd[i].GetCY();
		if (ChoseX < Att135Shd[i].GetCX() + Att135Shd[i].GetWidth()) ChoseX = Att135Shd[i].GetCX() + Att135Shd[i].GetWidth();
		if (ChoseY < Att135Shd[i].GetCY() + Att135Shd[i].GetHeight()) ChoseY = Att135Shd[i].GetCY() + Att135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att180Shd[i].CreateSprite(ID);
		if (MaxWidth < Att180Shd[i].GetWidth()) MaxWidth = Att180Shd[i].GetWidth();
		if (MaxHeight < Att180Shd[i].GetHeight()) MaxHeight = Att180Shd[i].GetHeight();
		if (MaxxCor > Att180Shd[i].GetCX()) MaxxCor = Att180Shd[i].GetCX();
		if (MaxyCor > Att180Shd[i].GetCY()) MaxyCor = Att180Shd[i].GetCY();
		if (ChoseX < Att180Shd[i].GetCX() + Att180Shd[i].GetWidth()) ChoseX = Att180Shd[i].GetCX() + Att180Shd[i].GetWidth();
		if (ChoseY < Att180Shd[i].GetCY() + Att180Shd[i].GetHeight()) ChoseY = Att180Shd[i].GetCY() + Att180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att225Shd[i].CreateSprite(ID);
		if (MaxWidth < Att225Shd[i].GetWidth()) MaxWidth = Att225Shd[i].GetWidth();
		if (MaxHeight < Att225Shd[i].GetHeight()) MaxHeight = Att225Shd[i].GetHeight();
		if (MaxxCor > Att225Shd[i].GetCX()) MaxxCor = Att225Shd[i].GetCX();
		if (MaxyCor > Att225Shd[i].GetCY()) MaxyCor = Att225Shd[i].GetCY();
		if (ChoseX < Att225Shd[i].GetCX() + Att225Shd[i].GetWidth()) ChoseX = Att225Shd[i].GetCX() + Att225Shd[i].GetWidth();
		if (ChoseY < Att225Shd[i].GetCY() + Att225Shd[i].GetHeight()) ChoseY = Att225Shd[i].GetCY() + Att225Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att270Shd[i].CreateSprite(ID);
		if (MaxWidth < Att270Shd[i].GetWidth()) MaxWidth = Att270Shd[i].GetWidth();
		if (MaxHeight < Att270Shd[i].GetHeight()) MaxHeight = Att270Shd[i].GetHeight();
		if (MaxxCor > Att270Shd[i].GetCX()) MaxxCor = Att270Shd[i].GetCX();
		if (MaxyCor > Att270Shd[i].GetCY()) MaxyCor = Att270Shd[i].GetCY();
		if (ChoseX < Att270Shd[i].GetCX() + Att270Shd[i].GetWidth()) ChoseX = Att270Shd[i].GetCX() + Att270Shd[i].GetWidth();
		if (ChoseY < Att270Shd[i].GetCY() + Att270Shd[i].GetHeight()) ChoseY = Att270Shd[i].GetCY() + Att270Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
      if (Type != 3)
         strcat(ID, "A");
		strcat(ID, "315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		Att315Shd[i].CreateSprite(ID);
		if (MaxWidth < Att315Shd[i].GetWidth()) MaxWidth = Att315Shd[i].GetWidth();
		if (MaxHeight < Att315Shd[i].GetHeight()) MaxHeight = Att315Shd[i].GetHeight();
		if (MaxxCor > Att315Shd[i].GetCX()) MaxxCor = Att315Shd[i].GetCX();
		if (MaxyCor > Att315Shd[i].GetCY()) MaxyCor = Att315Shd[i].GetCY();
		if (ChoseX < Att315Shd[i].GetCX() + Att315Shd[i].GetWidth()) ChoseX = Att315Shd[i].GetCX() + Att315Shd[i].GetWidth();
		if (ChoseY < Att315Shd[i].GetCY() + Att315Shd[i].GetHeight()) ChoseY = Att315Shd[i].GetCY() + Att315Shd[i].GetHeight();
		
	}

   for (i = 0; i < faces3; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StAtt000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt000Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt000Shd[i].GetWidth()) MaxWidth = StAtt000Shd[i].GetWidth();
		if (MaxHeight < StAtt000Shd[i].GetHeight()) MaxHeight = StAtt000Shd[i].GetHeight();
		if (MaxxCor > StAtt000Shd[i].GetCX()) MaxxCor = StAtt000Shd[i].GetCX();
		if (MaxyCor > StAtt000Shd[i].GetCY()) MaxyCor = StAtt000Shd[i].GetCY();
		if (ChoseX < StAtt000Shd[i].GetCX() + StAtt000Shd[i].GetWidth()) ChoseX = StAtt000Shd[i].GetCX() + StAtt000Shd[i].GetWidth();
		if (ChoseY < StAtt000Shd[i].GetCY() + StAtt000Shd[i].GetHeight()) ChoseY = StAtt000Shd[i].GetCY() + StAtt000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StAtt045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt045Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt045Shd[i].GetWidth()) MaxWidth = StAtt045Shd[i].GetWidth();
		if (MaxHeight < StAtt045Shd[i].GetHeight()) MaxHeight = StAtt045Shd[i].GetHeight();
		if (MaxxCor > StAtt045Shd[i].GetCX()) MaxxCor = StAtt045Shd[i].GetCX();
		if (MaxyCor > StAtt045Shd[i].GetCY()) MaxyCor = StAtt045Shd[i].GetCY();
		if (ChoseX < StAtt045Shd[i].GetCX() + StAtt045Shd[i].GetWidth()) ChoseX = StAtt045Shd[i].GetCX() + StAtt045Shd[i].GetWidth();
		if (ChoseY < StAtt045Shd[i].GetCY() + StAtt045Shd[i].GetHeight()) ChoseY = StAtt045Shd[i].GetCY() + StAtt045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt090Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt090Shd[i].GetWidth()) MaxWidth = StAtt090Shd[i].GetWidth();
		if (MaxHeight < StAtt090Shd[i].GetHeight()) MaxHeight = StAtt090Shd[i].GetHeight();
		if (MaxxCor > StAtt090Shd[i].GetCX()) MaxxCor = StAtt090Shd[i].GetCX();
		if (MaxyCor > StAtt090Shd[i].GetCY()) MaxyCor = StAtt090Shd[i].GetCY();
		if (ChoseX < StAtt090Shd[i].GetCX() + StAtt090Shd[i].GetWidth()) ChoseX = StAtt090Shd[i].GetCX() + StAtt090Shd[i].GetWidth();
		if (ChoseY < StAtt090Shd[i].GetCY() + StAtt090Shd[i].GetHeight()) ChoseY = StAtt090Shd[i].GetCY() + StAtt090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt135Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt135Shd[i].GetWidth()) MaxWidth = StAtt135Shd[i].GetWidth();
		if (MaxHeight < StAtt135Shd[i].GetHeight()) MaxHeight = StAtt135Shd[i].GetHeight();
		if (MaxxCor > StAtt135Shd[i].GetCX()) MaxxCor = StAtt135Shd[i].GetCX();
		if (MaxyCor > StAtt135Shd[i].GetCY()) MaxyCor = StAtt135Shd[i].GetCY();
		if (ChoseX < StAtt135Shd[i].GetCX() + StAtt135Shd[i].GetWidth()) ChoseX = StAtt135Shd[i].GetCX() + StAtt135Shd[i].GetWidth();
		if (ChoseY < StAtt135Shd[i].GetCY() + StAtt135Shd[i].GetHeight()) ChoseY = StAtt135Shd[i].GetCY() + StAtt135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt180Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt180Shd[i].GetWidth()) MaxWidth = StAtt180Shd[i].GetWidth();
		if (MaxHeight < StAtt180Shd[i].GetHeight()) MaxHeight = StAtt180Shd[i].GetHeight();
		if (MaxxCor > StAtt180Shd[i].GetCX()) MaxxCor = StAtt180Shd[i].GetCX();
		if (MaxyCor > StAtt180Shd[i].GetCY()) MaxyCor = StAtt180Shd[i].GetCY();
		if (ChoseX < StAtt180Shd[i].GetCX() + StAtt180Shd[i].GetWidth()) ChoseX = StAtt180Shd[i].GetCX() + StAtt180Shd[i].GetWidth();
		if (ChoseY < StAtt180Shd[i].GetCY() + StAtt180Shd[i].GetHeight()) ChoseY = StAtt180Shd[i].GetCY() + StAtt180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt225Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt225Shd[i].GetWidth()) MaxWidth = StAtt225Shd[i].GetWidth();
		if (MaxHeight < StAtt225Shd[i].GetHeight()) MaxHeight = StAtt225Shd[i].GetHeight();
		if (MaxxCor > StAtt225Shd[i].GetCX()) MaxxCor = StAtt225Shd[i].GetCX();
		if (MaxyCor > StAtt225Shd[i].GetCY()) MaxyCor = StAtt225Shd[i].GetCY();
		if (ChoseX < StAtt225Shd[i].GetCX() + StAtt225Shd[i].GetWidth()) ChoseX = StAtt225Shd[i].GetCX() + StAtt225Shd[i].GetWidth();
		if (ChoseY < StAtt225Shd[i].GetCY() + StAtt225Shd[i].GetHeight()) ChoseY = StAtt225Shd[i].GetCY() + StAtt225Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt270Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt270Shd[i].GetWidth()) MaxWidth = StAtt270Shd[i].GetWidth();
		if (MaxHeight < StAtt270Shd[i].GetHeight()) MaxHeight = StAtt270Shd[i].GetHeight();
		if (MaxxCor > StAtt270Shd[i].GetCX()) MaxxCor = StAtt270Shd[i].GetCX();
		if (MaxyCor > StAtt270Shd[i].GetCY()) MaxyCor = StAtt270Shd[i].GetCY();
		if (ChoseX < StAtt270Shd[i].GetCX() + StAtt270Shd[i].GetWidth()) ChoseX = StAtt270Shd[i].GetCX() + StAtt270Shd[i].GetWidth();
		if (ChoseY < StAtt270Shd[i].GetCY() + StAtt270Shd[i].GetHeight()) ChoseY = StAtt270Shd[i].GetCY() + StAtt270Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StAtt315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StAtt315Shd[i].CreateSprite(ID);
		if (MaxWidth < StAtt315Shd[i].GetWidth()) MaxWidth = StAtt315Shd[i].GetWidth();
		if (MaxHeight < StAtt315Shd[i].GetHeight()) MaxHeight = StAtt315Shd[i].GetHeight();
		if (MaxxCor > StAtt315Shd[i].GetCX()) MaxxCor = StAtt315Shd[i].GetCX();
		if (MaxyCor > StAtt315Shd[i].GetCY()) MaxyCor = StAtt315Shd[i].GetCY();
		if (ChoseX < StAtt315Shd[i].GetCX() + StAtt315Shd[i].GetWidth()) ChoseX = StAtt315Shd[i].GetCX() + StAtt315Shd[i].GetWidth();
		if (ChoseY < StAtt315Shd[i].GetCY() + StAtt315Shd[i].GetHeight()) ChoseY = StAtt315Shd[i].GetCY() + StAtt315Shd[i].GetHeight();
		
	}

	for (i = 0; i < faces4; i++) {
		strcpy(ID, ViewID);
		strcat(ID, "StMov000-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov000Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov000Shd[i].GetWidth()) MaxWidth = StMov000Shd[i].GetWidth();
		if (MaxHeight < StMov000Shd[i].GetHeight()) MaxHeight = StMov000Shd[i].GetHeight();
		if (MaxxCor > StMov000Shd[i].GetCX()) MaxxCor = StMov000Shd[i].GetCX();
		if (MaxyCor > StMov000Shd[i].GetCY()) MaxyCor = StMov000Shd[i].GetCY();
		if (ChoseX < StMov000Shd[i].GetCX() + StMov000Shd[i].GetWidth()) ChoseX = StMov000Shd[i].GetCX() + StMov000Shd[i].GetWidth();
		if (ChoseY < StMov000Shd[i].GetCY() + StMov000Shd[i].GetHeight()) ChoseY = StMov000Shd[i].GetCY() + StMov000Shd[i].GetHeight();

		strcpy(ID, ViewID);
		strcat(ID, "StMov045-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov045Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov045Shd[i].GetWidth()) MaxWidth = StMov045Shd[i].GetWidth();
		if (MaxHeight < StMov045Shd[i].GetHeight()) MaxHeight = StMov045Shd[i].GetHeight();
		if (MaxxCor > StMov045Shd[i].GetCX()) MaxxCor = StMov045Shd[i].GetCX();
		if (MaxyCor > StMov045Shd[i].GetCY()) MaxyCor = StMov045Shd[i].GetCY();
		if (ChoseX < StMov045Shd[i].GetCX() + StMov045Shd[i].GetWidth()) ChoseX = StMov045Shd[i].GetCX() + StMov045Shd[i].GetWidth();
		if (ChoseY < StMov045Shd[i].GetCY() + StMov045Shd[i].GetHeight()) ChoseY = StMov045Shd[i].GetCY() + StMov045Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov090-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov090Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov090Shd[i].GetWidth()) MaxWidth = StMov090Shd[i].GetWidth();
		if (MaxHeight < StMov090Shd[i].GetHeight()) MaxHeight = StMov090Shd[i].GetHeight();
		if (MaxxCor > StMov090Shd[i].GetCX()) MaxxCor = StMov090Shd[i].GetCX();
		if (MaxyCor > StMov090Shd[i].GetCY()) MaxyCor = StMov090Shd[i].GetCY();
		if (ChoseX < StMov090Shd[i].GetCX() + StMov090Shd[i].GetWidth()) ChoseX = StMov090Shd[i].GetCX() + StMov090Shd[i].GetWidth();
		if (ChoseY < StMov090Shd[i].GetCY() + StMov090Shd[i].GetHeight()) ChoseY = StMov090Shd[i].GetCY() + StMov090Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov135-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov135Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov135Shd[i].GetWidth()) MaxWidth = StMov135Shd[i].GetWidth();
		if (MaxHeight < StMov135Shd[i].GetHeight()) MaxHeight = StMov135Shd[i].GetHeight();
		if (MaxxCor > StMov135Shd[i].GetCX()) MaxxCor = StMov135Shd[i].GetCX();
		if (MaxyCor > StMov135Shd[i].GetCY()) MaxyCor = StMov135Shd[i].GetCY();
		if (ChoseX < StMov135Shd[i].GetCX() + StMov135Shd[i].GetWidth()) ChoseX = StMov135Shd[i].GetCX() + StMov135Shd[i].GetWidth();
		if (ChoseY < StMov135Shd[i].GetCY() + StMov135Shd[i].GetHeight()) ChoseY = StMov135Shd[i].GetCY() + StMov135Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov180-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov180Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov180Shd[i].GetWidth()) MaxWidth = StMov180Shd[i].GetWidth();
		if (MaxHeight < StMov180Shd[i].GetHeight()) MaxHeight = StMov180Shd[i].GetHeight();
		if (MaxxCor > StMov180Shd[i].GetCX()) MaxxCor = StMov180Shd[i].GetCX();
		if (MaxyCor > StMov180Shd[i].GetCY()) MaxyCor = StMov180Shd[i].GetCY();
		if (ChoseX < StMov180Shd[i].GetCX() + StMov180Shd[i].GetWidth()) ChoseX = StMov180Shd[i].GetCX() + StMov180Shd[i].GetWidth();
		if (ChoseY < StMov180Shd[i].GetCY() + StMov180Shd[i].GetHeight()) ChoseY = StMov180Shd[i].GetCY() + StMov180Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov225-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov225Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov225Shd[i].GetWidth()) MaxWidth = StMov225Shd[i].GetWidth();
		if (MaxHeight < StMov225Shd[i].GetHeight()) MaxHeight = StMov225Shd[i].GetHeight();
		if (MaxxCor > StMov225Shd[i].GetCX()) MaxxCor = StMov225Shd[i].GetCX();
		if (MaxyCor > StMov225Shd[i].GetCY()) MaxyCor = StMov225Shd[i].GetCY();
		if (ChoseX < StMov225Shd[i].GetCX() + StMov225Shd[i].GetWidth()) ChoseX = StMov225Shd[i].GetCX() + StMov225Shd[i].GetWidth();
		if (ChoseY < StMov225Shd[i].GetCY() + StMov225Shd[i].GetHeight()) ChoseY = StMov225Shd[i].GetCY() + StMov225Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov270-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov270Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov270Shd[i].GetWidth()) MaxWidth = StMov270Shd[i].GetWidth();
		if (MaxHeight < StMov270Shd[i].GetHeight()) MaxHeight = StMov270Shd[i].GetHeight();
		if (MaxxCor > StMov270Shd[i].GetCX()) MaxxCor = StMov270Shd[i].GetCX();
		if (MaxyCor > StMov270Shd[i].GetCY()) MaxyCor = StMov270Shd[i].GetCY();
		if (ChoseX < StMov270Shd[i].GetCX() + StMov270Shd[i].GetWidth()) ChoseX = StMov270Shd[i].GetCX() + StMov270Shd[i].GetWidth();
		if (ChoseY < StMov270Shd[i].GetCY() + StMov270Shd[i].GetHeight()) ChoseY = StMov270Shd[i].GetCY() + StMov270Shd[i].GetHeight();
		
		strcpy(ID, ViewID);
		strcat(ID, "StMov315-");
		Temp2[0] = (i*FRAMING) + 'a';
		strcat(ID, Temp2);
      strcat(ID, "Shd");
		StMov315Shd[i].CreateSprite(ID);
		if (MaxWidth < StMov315Shd[i].GetWidth()) MaxWidth = StMov315Shd[i].GetWidth();
		if (MaxHeight < StMov315Shd[i].GetHeight()) MaxHeight = StMov315Shd[i].GetHeight();
		if (MaxxCor > StMov315Shd[i].GetCX()) MaxxCor = StMov315Shd[i].GetCX();
		if (MaxyCor > StMov315Shd[i].GetCY()) MaxyCor = StMov315Shd[i].GetCY();
		if (ChoseX < StMov315Shd[i].GetCX() + StMov315Shd[i].GetWidth()) ChoseX = StMov315Shd[i].GetCX() + StMov315Shd[i].GetWidth();
		if (ChoseY < StMov315Shd[i].GetCY() + StMov315Shd[i].GetHeight()) ChoseY = StMov315Shd[i].GetCY() + StMov315Shd[i].GetHeight();
		
	}
*/
	fTransparent = View000[0].GetfTransparent();
	TransColor = View000[0].GetTransColor();
   if ((MaxxCor % 2)) {
      MaxxCor--;
      MaxWidth++;
   }
   if (!(MaxyCor % 2)) {
      MaxyCor--;
      MaxHeight++;
   }

	MaxWidth += ChoseX - (MaxxCor + MaxWidth);
	MaxHeight += ChoseY - (MaxyCor + MaxHeight);

   if (Type == 3)
      Type = 0;

	delete ID;
	delete Temp;
}


int Sprite3D::GetTextCorrection(void) {
   LastAccess = timeGetTime();
   return View000[0].GetHeight();
}

BOOL Sprite3D::Release(DWORD Wait) {
   if ((timeGetTime() - LastAccess) > Wait) { 
      LastAccess = -1; 
      return TRUE; 
   } 
   return FALSE;
}