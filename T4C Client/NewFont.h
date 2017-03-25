#include <windows.h>
#include "TemplateList.h"
#include "Disp.h"
#include "Sprite.h"
#include "V2Sprite.h"
#include <string>

#ifndef NEWFONT_HEADER
#define NEWFONT_HEADER

class FontObject;
class TextObject;

const DWORD TO_DEFAULT     = 0;
const DWORD TO_DRAW_QUOTES = 1;

extern Global g_Global;

class FontManager : public TemplateList <FontList> {
   public:
      ~FontManager(void);
      static FontManager * GetInstance(void);
      void Initialize(void);
      TextObject *GetSmallFont(BOOL bValidSize,LPCTSTR szDefaultText = NULL);
      TextObject *GetTalkFont(BOOL bValidSize);
      void Release(void);

   private:
      FontObject *lpFont;

      FontManager(void);
      void CreateFont(LPTSTR, FontObject *, double Pc);
};

class FontObject {
   friend FontManager;
   friend TextObject;

   public:
      ~FontObject(void);

      double GetHeight(HDC);
      double GetWidth(HDC, LPCTSTR);

   private:
		HFONT hf;

      FontObject(void);
      void Create(LPLOGFONT lplf, double Pc);
};

class TextObject {

   friend FontManager;

   public:
      ~TextObject(void);
      // Set Text(
         // LPTSTR lpszString   -> String que tu veux afficher
         // DWORD  dwTextColor  -> Couleur de la String en RGB
         // int    iPixelWidth  -> Width Maximum du text (le Height est automatique)
         // int    iExtraHeight -> Espacement supplementaire entre chaque ligne de texte
         // int    iFlag        -> 0 pas de background noir, 1 background noir.
      // );
      void SetText(LPCTSTR, DWORD textColor = RGB( 255, 255, 255 ), int maxWidth = g_Global.GetScreenW(), int lineSpacing = 0, bool blackBackground = true, bool allowNewLine = true, bool indentNewLine = false );
      void SetMaxLines( DWORD maxLines, DWORD startLineOffset );
      void ResetText(LPCTSTR);
      void Test(void);
      void Release(void);
      LPCTSTR GetText( void ){
        return m_Text.c_str();
      }
      bool IsEmpty(){
          return m_Text.empty();
      }
      void Draw(int x, int y, V2SPRITEFX * = NULL);
      void SetFlag(DWORD);

      int GetLineCount(){
        return iNbLine;
      }

      int GetHeight(void);
      int GetFontSize(void);
      int GetWidth(LPCTSTR = NULL);

      void Format( int maxX, int maxY = -1 );
      
      int GetTruncatedTextLineOffset();

      void EnableOutline();
      void EnableCenterJustification();

      void Redo(void);

   private:      
      
      void TextColor(
          HDC, 
          const char *text, 
          int xPos, 
          int yPos, 
          DWORD &currentColor, 
          DWORD &newColor, 
          bool skipColorCoding, 
          bool transparentBackground
      );

      FontObject *foFont;
      std::string m_Text;
      Sprite TextSprite;
      int iNbLine;
      int  m_LineSpacing;
      int  m_MaxWidth;
      bool m_BlackBackground;
      bool bRelease;
      bool allowNewLine;
      DWORD dwFlag;
      bool indentNewLine;
      DWORD m_MaxLines;
      DWORD m_StartLineOffset;
      bool  m_Outline;
      bool  m_CenterJustify;

      int  m_TruncatedTextLineOffset;
      
      DWORD oldColor;
      DWORD dwUse;

      TextObject(void);
};

#endif