// NTime.h: interface for the NTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTIME_H__CD03AB84_64D0_11D1_BEAA_00E0290586FC__INCLUDED_)
#define AFX_NTIME_H__CD03AB84_64D0_11D1_BEAA_00E0290586FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class NTime  
{
public:
	int ForceRed, ForceGreen, ForceBlue;
	int Seconde, Minute, Hour, Week, Day, Month, Year;

	NTime();
	virtual ~NTime();

	void SetLight(void);
	void AddSeconde(void);
   char CurrentLight;
   char CurrentLow;

};

extern NTime g_TimeStructure;

#endif // !defined(AFX_NTIME_H__CD03AB84_64D0_11D1_BEAA_00E0290586FC__INCLUDED_)
