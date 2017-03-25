#include "IDirectXDraw.h"
#include "SystemInfo.h"

#ifndef __WARNINGBOX
#define __WARNINGBOX
class WarningBox {
public:
	void operator << (char *s) {
		DXDRestoreView();
      MessageBox(siGethWnd(), s, "Error", MB_OK);
		throw;
	}
};

extern WarningBox WARNINGBOX;
#endif
