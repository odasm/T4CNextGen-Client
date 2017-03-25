#ifndef PVPRANKING_H
#define PVPRANKING_H


#include <windows.h>
#include <string>

class PvpRanking{

	public:
		PvpRanking(void);
		~PvpRanking(void);

		void PvpRanking::myPvpStat(
			long TotalDeathNumber,
			long TotalKillNumber,
			long CurrentDeathNumber,
			long CurrentKillNumber,
			long BestSerieMurder,
			long CurrentSerieMurder,
			long CurrentPvpPoint);
};
#endif PVPRANKING_H