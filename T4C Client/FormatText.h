#ifndef __FORMATTEXT_H
#define __FORMATTEXT_H

#include "Disp.h"
#include "NewFont.h"

class FormatText {
   private:
      char *String;
      int nbLine;
      char **Text;
		BOOL SystemFormat;
   public:
      FormatText(void);
      ~FormatText(void);
      void  SetText(const char *);
      void  Format(int, Font *, bool indentNewLine = false );
      void  Format(int, FontObject *, bool indentNewLine = false);
      char *GetText(int);
      int   GetnbLine(void);
      void  Format(UINT, bool indentNewLine = false);
		char *GetString(void);
		void  SetSystem(BOOL);
};

#endif