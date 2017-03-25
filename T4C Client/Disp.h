#ifndef __DISP
#define __DISP

#include "TemplateList.h"

class Font {
	public:
		Font(void);
		~Font(void);
		
		HFONT hf;
		LPLOGFONT lplfLogical;

		void SetLogical(LPLOGFONT);
		void Create(void);
		void Release(void);
		int  DrawFont(HDC, int, int, int, int, char *, int = NULL);
		void SetSize(long, long,long Weight=FW_NORMAL);
      void SetWeight(int);
		void SetSize(double);
		int  GetLen(HDC, LPTSTR);
		int  GetHeight(HDC);
};

class FontList {
	public:
		FontList(void);
		~FontList(void);

		LPTSTR    lpszFontName;
		LPLOGFONT lplfLogical;
};

class FontHandler : public TemplateList <FontList> {
	public:
		FontHandler(void);
		~FontHandler(void);

		void  Initialize(void);
		Font  *GetFont(LPTSTR);
};

#define NDT_CENTER 1

int DisplayText(HDC, Font *, int x, int y, const int NumLine, int Color, char *Text, UINT StartPos, int Flag);

extern FontHandler * UniqueFontHandler;

#endif