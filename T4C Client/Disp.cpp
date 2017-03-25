// *******************************************************************************************
// ***                                                                                     ***
//      File Name: Disp.cpp
//      Project:   TFC Client
//      Creation:  23/11/1997
//      Author:    Benoit Thomas (TH)
// ***                                                                                     ***
// *******************************************************************************************
// ***                                                                                     ***
//      Change History
//
//         Date            Ver.      Author         Purpose
//         ----            ----      ------         -------
//         23/11/1997      1.0        TH             Initial developpement
//
//      Description
//          Font Structure.
//          
// ***                                                                                     ***
//	*******************************************************************************************
// ***         Copyright (c) 1997-1998 Vircom. All rights reserved.                        ***

#pragma warning( disable : 4291 )
#pragma warning( disable : 4786 )

#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include "IDirectXDraw.h"
#include "PowerDebug.h"
#include "Disp.h"
#include "Global.h"
#include "MemoryManager.h"
#include "Filelogger.h"

FontHandler * UniqueFontHandler = NULL;

//**************************************************************************************//
int CALLBACK FontHandlerCallBack
//**************************************************************************************//
// Load all the Current System Font.
// 
(
 LPLOGFONT lplf,    // Logical Description of a Font.
 LPTEXTMETRIC lptm, // Physical Description of a Font.
 DWORD FontType,    // FontType.
 LPARAM lParam      // User-Defined Parameter.
)
// Return: int, 1 Continue enumeration, 0 Stop enumeration 
//**************************************************************************************//
{
	FontList *fl = new FontList;

   memcpy(fl->lplfLogical, lplf, sizeof(LOGFONT));
	fl->lpszFontName = new TCHAR [strlen(lplf->lfFaceName)+1];
	strcpy(fl->lpszFontName, lplf->lfFaceName);

	LOG << "* FNT = " << fl->lpszFontName << "\r\n";

	UniqueFontHandler->AddToTail(fl);
	return 1; 
} 

//**************************************************************************************//
Font::Font( void )
//**************************************************************************************//
// Default Constructor.
// 
//**************************************************************************************//
{
	lplfLogical = new LOGFONT;
   hf = NULL;
}

//*** Destructor ***********************************************************************//
Font::~Font( void )
//**************************************************************************************//
// Default Constructor.
// 
//**************************************************************************************//
{
	delete [] lplfLogical;
   if (hf)
      DeleteObject(hf);
   hf = NULL;
}

//**************************************************************************************//
void Font::SetLogical
//**************************************************************************************//
// Set the Logical Font format
// 
(
 LPLOGFONT lplf // Logical Font Format
)
//**************************************************************************************//
{
	memcpy(lplfLogical, lplf, sizeof(LOGFONT));
}

//**************************************************************************************//
void Font::Create( void )
//**************************************************************************************//
// Create the Font to use
// 
//**************************************************************************************//
{
   hf = CreateFontIndirect(lplfLogical); 
}

//**************************************************************************************//
void Font::Release( void )
//**************************************************************************************//
// Delete the Font.
// 
//**************************************************************************************//
{
	DeleteObject(hf);
   hf = NULL;
}

//**************************************************************************************//
int Font::DrawFont
//**************************************************************************************//
// Draw the Text on the Screen
// 
(
 HDC hdc,
 int x,             // X Position of the Text 
 int y,             // Y Position of the Text
 int DefaultColor,  // Default Color of the Text
 int CurrentColor,  // Curremt Color of the Text
 char *Text,       // The Text
 int Flag           // Flag
)
//**************************************************************************************//
{
	int DebugHelp = 0;
	char *ptr;
	int C = 0;
   unsigned int i;
	LPTSTR NText = new TCHAR [strlen(Text)+1];
	strcpy(NText, Text);
	LPTSTR NText2 = new TCHAR [strlen(Text)+1];
	strcpy(NText2, Text);
	COLORREF Color = CurrentColor ? CurrentColor : DefaultColor;
	if (Flag & NDT_CENTER) {
      for (i = 0; i < strlen(NText2); i++) {
         if (NText2[i] == '{' || NText2[i] == '}' || NText2[i] == '\"')
            C++;
         else
            NText2[i-C] = NText2[i];
      }
      NText2[i-C] = NULL;
      x -= (GetLen(hdc, NText2)/2);
	}
	C = 0;
	SelectObject(hdc, hf);
	SetBkMode(hdc, TRANSPARENT);
	ptr = NText;
	unsigned int Len = strlen(NText);
	for (i=0; i < Len; i++) {
		if (NText[i] == '\"') {
			NText[i] = 0;
			DebugHelp = 1;
			SetTextColor(hdc, Color);
			DebugHelp = 5;
			if (Color != RGB(255, 255, 255) && Color != 0)
				Color = RGB(255, 255, 255);
			else
				Color = DefaultColor;
			::TextOut(hdc, x+C, y, ptr, strlen(ptr));
			C += GetLen(hdc, ptr);
			ptr = (char *)(NText+i+1);
		}
		if (NText[i] == '{') {
			NText[i] = 0;
			DebugHelp = 2;
			SetTextColor(hdc, Color);
			DebugHelp = 6;
			if (Color != RGB(150, 150, 150) && Color != 0)
				Color = RGB(150, 150, 150);
			else
				Color = DefaultColor;
			::TextOut(hdc, x+C, y, ptr, strlen(ptr));
			C += GetLen(hdc, ptr);
			ptr = (char *)(NText+i+1);
		}
		if (NText[i] == '}') {
			NText[i] = 0;
			DebugHelp = 3;
			SetTextColor(hdc, Color);
			DebugHelp = 7;
			if (Color != RGB(150, 150, 150) && Color != 0)
				Color = RGB(150, 150, 150);
			else
				Color = DefaultColor;
			::TextOut(hdc, x+C, y, ptr, strlen(ptr));
			C += GetLen(hdc, ptr);
			ptr = (char *)(NText+i+1);
		}
	}
	DebugHelp = 4;
	SetTextColor(hdc, DefaultColor);
	DebugHelp = 8;
	::TextOut(hdc, x+C, y, ptr, strlen(ptr));  
	delete NText;
	delete NText2; 
   return 0; 
}

//**************************************************************************************//
void Font::SetSize
//**************************************************************************************//
// Set both Width and Height;
// 
(
 long Width, // Font Width
 long Height, // Font Height
 long Weight
)
//**************************************************************************************//
{

	if (!Width) 
   {
		SetSize((float)(Height / lplfLogical->lfHeight));
	} 
   else if (!Height) 
   {
		SetSize((float)(Width / lplfLogical->lfWidth));
	} 
   else if (Width && Height) 
   {
		lplfLogical->lfHeight = Height;
		lplfLogical->lfWidth  = Width;
	}
   lplfLogical->lfWeight = Weight;
}

//**************************************************************************************//
void Font::SetSize
//**************************************************************************************//
// Set the Font Size in %
// 
(
 double Pc // % of the Original Font Size
)
//**************************************************************************************//
{
	lplfLogical->lfWidth  = (long)(lplfLogical->lfWidth  * Pc);
	lplfLogical->lfHeight = (long)(lplfLogical->lfHeight * Pc);
}

//**************************************************************************************//
int Font::GetLen
//**************************************************************************************//
// Give the size of the String
// 
(
 HDC hdc,        // Handle du Device Context
 LPTSTR lpszText // the Text String
)
// Return: int, Size of the String in Pixel.
//**************************************************************************************//
{
	SIZE sz;
   sz.cx = 0;
   sz.cy = 0;

	SelectObject(hdc, hf);
	GetTextExtentPoint32(hdc, lpszText, lstrlen(lpszText), &sz); 
	
	return sz.cx;
}

//**************************************************************************************//
int Font::GetHeight
//**************************************************************************************//
// Get the Font Height
// 
(
 HDC hdc // Handle du Device Context
)
// Return: int, Height of the Letter X in Pixel
//**************************************************************************************//
{
	SIZE sz;
   sz.cx = 0;
   sz.cy = 0;

   HGDIOBJ r = SelectObject(hdc, hf);
   //char Temp[1000];
   /* allocated memory reduced */
   //char *Temp;
   //Temp = (char*)malloc(1000);
   //sprintf(Temp, "R : %d, GDI_ERROR : %d", r, GDI_ERROR);
   //OutputDebugString(Temp);
	GetTextExtentPoint32(hdc, "X", 1, &sz); 
	//free (Temp);
	
	return sz.cy;
}

//**************************************************************************************//
FontList::FontList( void )
//**************************************************************************************//
// Default Constructor
// 
//**************************************************************************************//
{
	lplfLogical = new LOGFONT;
   lpszFontName = NULL;
}
 
//*** Destructor ***********************************************************************//
FontList::~FontList( void )
//**************************************************************************************//
// Default Destructor
// 
//**************************************************************************************//
{
	delete lplfLogical;
   if (lpszFontName)
      delete lpszFontName;
}

//**************************************************************************************//
FontHandler::FontHandler( void )
//**************************************************************************************//
// Default Constructor.
// 
//**************************************************************************************//
{

}

//*** Destructor ***********************************************************************//
FontHandler::~FontHandler( void )
//**************************************************************************************//
// Default Destructor.
// 
//**************************************************************************************//
{
	ToHead();
	while(QueryNext()) {
   	FontList *flObject;
		flObject = GetObject();
      RemoveObject();
      delete flObject;
	}
}

//**************************************************************************************//
void FontHandler::Initialize( void )
//**************************************************************************************//
// Load all the Current System Font.
// 
//**************************************************************************************//
{
   int iDebug = 0;
      HDC hdc;
      iDebug = 1;
	
   	DXDGetDC(&hdc, 1);
      iDebug = 2;
   	EnumFonts(hdc, NULL, (FONTENUMPROC) FontHandlerCallBack, NULL);
      iDebug = 3;
   	DXDReleaseDC(hdc, 1);
      iDebug = 4;
}

//**************************************************************************************//
Font *FontHandler::GetFont
//**************************************************************************************//
// Get a System Font
// 
(
 LPTSTR lpszFontName // Name of a Font.
)
// Return: Font *, the Font Structure
//**************************************************************************************//
{
	Font     *foReturn = new Font;
	FontList *flObject;
	bool Found = false;

	ToHead();
	while(QueryNext()) {
		flObject = GetObject();
		if (!strcmp(flObject->lpszFontName, lpszFontName)) {
			foReturn->SetLogical(flObject->lplfLogical);
			Found = true;
			SetQueryState(STOP_QUERY);
		}
	}

	if (!Found) {
		ToHead();
		while(QueryNext()) {
			flObject = GetObject();
			if (!strcmp(flObject->lpszFontName, "System")) {
				foReturn->SetLogical(flObject->lplfLogical);
				Found = true;
				SetQueryState(STOP_QUERY);
			}
		}
	}

	return foReturn;
}

int DisplayText(HDC hdc, Font *fFont, int x, int y, const int NumLine, int Color, char *Text, UINT StartPos, int Flag) {
		int NbLine = 0;
		UINT Num = 0;
		const short MAX = 300;
			
		char ShortText[3][MAX+1]; // NumLine
		char Temp[2500];
		char *NewWord;
		unsigned int len = 0;
		char *ptrStart;
		char *ptrEnd;
		bool Exit = false;
		BOOL Repeat;
			
		while (Text[strlen(Text)-1] == ' ') Text[strlen(Text)-1] = 0;
		int i, j;
		j = strlen(Text);
		Repeat = TRUE;
		while (Repeat) {
			Repeat = FALSE;
			for (i = 0; i < (j-1); i++) {
				if (Text[i] == '\"' && Text[i+1] == '\"') {
					strcpy(&Text[i], &Text[i+1]);
					i = j;
					Repeat = TRUE;
				}
			}
		}
		
		for (i = 0; i < NumLine; i++)
			ShortText[i][0] = 0;

		if (StartPos == strlen(Text)+1) 
			return StartPos;
		strcpy(Temp, &Text[StartPos]);
		
		ptrStart = Temp;
		NewWord = strtok(Temp, " ");
		if (fFont->GetLen(hdc, NewWord) > MAX) {
			if (strlen(NewWord) > 30) {
				NewWord[30] = 0;
			}
		}
	
		for (i = 0; i < NumLine; i++) {
			len = 0;		
			Exit = false;
			if (NewWord) {
				NbLine++;
				while (fFont->GetLen(hdc, ShortText[i]) + len < MAX && NewWord && !Exit) {
					ptrEnd = NewWord + strlen(NewWord) + 1;
					strcat(ShortText[i], NewWord);
					if (fFont->GetLen(hdc, ShortText[i]) < MAX)
						strcat(ShortText[i], " ");

					NewWord = strtok(NULL, " ");
					if (NewWord) {
						len = fFont->GetLen(hdc, NewWord);
						if (!strncmp(NewWord, "NN", 2)) {
							Exit = true;
							strcpy(NewWord, &NewWord[2]);
							len = fFont->GetLen(hdc, NewWord);
						}
						if (len > MAX) {
							if (strlen(NewWord) > 30) {
								NewWord[30] = 0;
								len = fFont->GetLen(hdc, NewWord);
							}
						}
					}
				};
			}
		}

		Num = (ptrEnd - ptrStart) + StartPos;

//		int ch[NumLine-1];
//		for (i = 0; i < NumLine-1; i++) {
//			ch[i] = 0;
//		}
			
		
      if (Num != strlen(Text)+1 && NbLine == 3) {
         //char Temp[10];
		 /* allocated memory reduced */
		 char *Temp;
         Temp = (char*)malloc(10);
         sprintf(Temp, "\"%c\"", 254);
         strcat(ShortText[2], Temp);
		 free (Temp);
      }
      
      int ch1 = 0, ch2 = 0;
				
		if (NbLine == 3) {
			ch1 = fFont->lplfLogical->lfHeight*2;
			ch2 = fFont->lplfLogical->lfHeight; 
		} else if (NbLine == 2) {
			ch1 = fFont->lplfLogical->lfHeight;
		}

//		for (int Nb = NbLine; Nb >= 0; Nb--)
//			for (i = 1; i < NbLine+1; i++)
//				if (Nb > i)
//					ch[i-1] += 12;

		int h;
      h = fFont->DrawFont(hdc, x, y-ch1, Color, 0, ShortText[0], Flag);
		h = fFont->DrawFont(hdc, x, y-ch2, Color, h, ShortText[1], Flag);
		fFont->DrawFont(hdc, x, y, Color, h, ShortText[2], Flag);

		return Num;
}
