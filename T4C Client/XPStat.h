#ifndef XPSTAT_H
#define XPSTAT_H


#include <windows.h>
#include <string>
class XpStat{

	public:
		XpStat(void);
		~XpStat(void);

		void Init(__int64 pXpStart);
		void CalculateAndShowXpStat(__int64 pXpNow, __int64 pExp2Go);
	private:
		__int64 mXpStart;
		__int32 mStartMs;

		bool mStarted;

};
#endif XPSTAT_H