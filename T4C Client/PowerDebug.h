// PowerDebug.h: interface for the PowerDebug class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POWERDEBUG_H__044C13E2_F97C_11D0_BEAA_444553540000__INCLUDED_)
#define AFX_POWERDEBUG_H__044C13E2_F97C_11D0_BEAA_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#include "WarningBox.h"
#include "TemplateQueues.h"
#include "DirectSocket.h"

class PowerLog {
public:
	CRITICAL_SECTION csInternal;
	
	PowerLog(void);
	~PowerLog(void);
	PowerLog &operator << (char *);
	PowerLog &operator << (__int64);

   void Lock(void);
   void Unlock(void);

   void Enable( bool newState, bool logToMonitor );

private:
   bool enabled;
   bool logToMonitor;
};

extern PowerLog LOG;

#endif