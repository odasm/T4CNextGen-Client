#include "pch.h"
#include <windows.h>
#include "FormatText.h"
#include "IDirectXDraw.h"
#include "PowerDebug.h"
#include "Global.h"
#include "MemoryManager.h"

FormatText::FormatText(void) {
   String = NULL;
   Text = NULL;
   nbLine = 0;
	SystemFormat = TRUE;
}

FormatText::~FormatText(void) {
   if (String)
      delete String;
   if (Text) {
      for (int i = 0; i < nbLine; i++)
         delete Text[i];
      delete Text;
   }
}

void FormatText::SetText(const char *s) {
   if (s) {
      if (String) {
         char *NewString = new char [strlen(String) + strlen(s) + 2];
         strcpy(NewString, String);
         strcat(NewString, " ");
         strcat(NewString, s);
         delete String;
         String = NewString;
      } else {
         char *NewString = new char [strlen(s) + 1];
         strcpy(NewString, s);
         String = NewString;
      }
   }
}

char * FormatText::GetString(void) {
	return String;
}

void FormatText::Format(int PixelPerLine, Font *fFont, bool indentNewLine) 
{
   if (Text) {
      for (int i = 0; i < nbLine; i++)
         delete Text[i];
      delete Text;
   }
   
   char *Temp = new char [strlen(String) + 1];
   char *NewWord;
   int Len = 0;
   int OldnbLine = 0;
   int extraIndentLen = 0;
   char *tText = new char [2500];
   bool Exit = false;
			
   strcpy(Temp, String);
   
   NewWord = strtok(Temp, " ");
   
   nbLine = 0;
   Len  = 0;
   
   HDC hdc;
   DXDGetDC(&hdc, 2);
   
   Text = new char * [2500];
   
  
   while (NewWord) 
   {
      ZeroMemory(tText, 2500);
      Exit = false;
      if (NewWord)
         Len = fFont->GetLen(hdc, NewWord);
      
      if( nbLine != 0 && extraIndentLen == 0 )
      {
         extraIndentLen = fFont->GetLen( hdc, "  " );
      }
      
      while (Len + fFont->GetLen(hdc, tText) + extraIndentLen >= PixelPerLine) 
      {
         int len = strlen(NewWord);
         NewWord[len-1] = NULL;
         Len = fFont->GetLen(hdc, NewWord);
      }
      
      while (fFont->GetLen(hdc, tText) + Len + extraIndentLen < PixelPerLine && NewWord && !Exit) 
      {
         strcat(tText, NewWord);
         
         if (fFont->GetLen(hdc, tText) < PixelPerLine)
            strcat(tText, " ");
         
         NewWord = strtok(NULL, " ");
         if (NewWord) 
         {
            Len = fFont->GetLen(hdc, NewWord);
            if (!strncmp(NewWord, "<>", 2) && SystemFormat) 
            {
               Exit = true;
               strcpy(NewWord, &NewWord[2]);
               Len = fFont->GetLen(hdc, NewWord);
            }
         }
      }
      
      if( nbLine == 0 || !indentNewLine && indentNewLine )
      {
         Text[nbLine] = new char [strlen(tText)+1];
         strcpy(Text[nbLine], tText);
      }else
      {
         Text[nbLine] = new char [strlen(tText)+3];
         strcpy(Text[nbLine], "  ");
         strcat(Text[nbLine], tText);
      }
      nbLine++;
   }
   DXDReleaseDC(hdc, 2);
   
   delete Temp;
   delete tText;
}

void FormatText::Format(int PixelPerLine, FontObject *fFont, bool indentNewLine ) {
   if (Text) {
      for (int i = 0; i < nbLine; i++)
         delete Text[i];
		delete Text;
	}
		
   char *Temp = new char [strlen(String) + 1];
   char *NewWord;
   int Len;
   int OldnbLine = 0;
   char *tText = new char [2500];
	bool Exit = false;
			
   strcpy(Temp, String);
		
   NewWord = strtok(Temp, " ");
	
   nbLine = 0;
   Len  = 0;
	
	HDC hdc;
	DXDGetDC(&hdc, 3);
 
    int extraIndentLen = 0;


   Text = new char * [2500]; 

   while (NewWord) {
      ZeroMemory(tText, 2500);
      Exit = false;
    if (NewWord) {
			Len = (int)fFont->GetWidth(hdc, NewWord);
    }

        if( nbLine != 0 && extraIndentLen == 0 && indentNewLine )
        {
            extraIndentLen = (int)fFont->GetWidth( hdc, "  " );
        }

		while (Len + fFont->GetWidth(hdc, tText) + extraIndentLen >= PixelPerLine) {
			int len = strlen(NewWord);
			NewWord[len-1] = NULL;
			Len = (int)fFont->GetWidth(hdc, NewWord);
		}
	
		while (fFont->GetWidth(hdc, tText) + Len + extraIndentLen < PixelPerLine && NewWord && !Exit) {
			strcat(tText, NewWord);
	 
			if (fFont->GetWidth(hdc, tText) < PixelPerLine)
				strcat(tText, " ");

 			NewWord = strtok(NULL, " ");
			if (NewWord) {
				Len = (int)fFont->GetWidth(hdc, NewWord);
				if (!strncmp(NewWord, "<>", 2) && SystemFormat) {
					Exit = true;
					strcpy(NewWord, &NewWord[2]);
					Len = (int)fFont->GetWidth(hdc, NewWord);
				}
			}
		}

        if( nbLine == 0 || !indentNewLine ){
            Text[nbLine] = new char [strlen(tText)+1];
		    strcpy(Text[nbLine], tText);
        }else{
            Text[nbLine] = new char [strlen(tText)+3];
            strcpy(Text[nbLine], "  ");
		    strcat(Text[nbLine], tText);
        }
		nbLine++;
   }

	DXDReleaseDC(hdc, 3);

	delete Temp;
	delete tText;
}

void FormatText::SetSystem(BOOL f) {
	SystemFormat = f;
}

void FormatText::Format(UINT Len, bool indentNewLine ) {
   if (Text) {
      for (int i = 0; i < nbLine; i++)
         delete Text[i];
		delete Text;
	}

   char *Temp = new char [strlen(String) + 1];
   char *NewWord;
   UINT Len2 = 0;
   int OldnbLine = 0;
   char *tText = new char [2500];
	bool Exit = false;

   strcpy(Temp, String);

   NewWord = strtok(Temp, " ");

   Text = new char * [2500];

    int extraIndentLen = 0;

	while (NewWord) {
      ZeroMemory(tText, 2500);

      if (Len2 >= Len) {
         NewWord[Len-1] = 0;
         Len2 = Len-1;
      }

     if( nbLine != 0 && extraIndentLen == 0 && indentNewLine ){
        extraIndentLen = 2;
     }

      Exit = false;
		while (strlen(tText) + Len2 + extraIndentLen < Len && NewWord && !Exit) {
         strcat(tText, NewWord);
	 
         if (strlen(tText) < Len)
				strcat(tText, " ");

 			NewWord = strtok(NULL, " ");
			if (NewWord) {
				Len2 = strlen(NewWord);
				if (!strncmp(NewWord, "<>", 2) && SystemFormat) {
					Exit = true;
					strcpy(NewWord, &NewWord[2]);
   				Len2 = strlen(NewWord);
				}
   		} 
		}

        if( nbLine == 0 || !indentNewLine ){
            Text[nbLine] = new char [strlen(tText)+1];
		    strcpy(Text[nbLine], tText);
        }else{
            Text[nbLine] = new char [strlen(tText)+3];
            strcpy(Text[nbLine], "  ");
		    strcat(Text[nbLine], tText);
        }
      nbLine++;
   }

	delete Temp;
	delete tText; 
}

int FormatText::GetnbLine(void) {
   return nbLine;
}

char *FormatText::GetText(int nb) {
   return Text[nb];
}