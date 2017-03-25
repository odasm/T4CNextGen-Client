#include "PvpRanking.h"
#include <math.h>
#include <string>

#include "NewInterface/ChatterUI.h"



PvpRanking::PvpRanking(void){}

PvpRanking::~PvpRanking(void){}

void PvpRanking::myPvpStat(long TotalDeathNumber,
	long TotalKillNumber,
	long CurrentDeathNumber,
	long CurrentKillNumber,
	long BestSerieMurder,
	long CurrentSerieMurder,
	long CurrentPvpPoint)
{
	try
	{
		char txt1[100];
		int strlen = 0;
		strlen = sprintf(txt1,"PVP Points: {%i}.",CurrentPvpPoint);
		ChatterUI::GetInstance()->AddBackscrollItem("", txt1, RGB(174,238,238), true);
		strlen = sprintf(txt1,"Current kills: {%i}.",CurrentKillNumber);
		ChatterUI::GetInstance()->AddBackscrollItem("", txt1, RGB(174,238,238), true);
		strlen = sprintf(txt1,"Current serial killing: {%i}.",CurrentSerieMurder);
		ChatterUI::GetInstance()->AddBackscrollItem("", txt1, RGB(174,238,238), true);
		strlen = sprintf(txt1,"Current deaths: {%i}.",CurrentDeathNumber);
		ChatterUI::GetInstance()->AddBackscrollItem("", txt1, RGB(174,238,238), true);
		strlen = sprintf(txt1,"Best serial killing: {%i}.",BestSerieMurder);
		ChatterUI::GetInstance()->AddBackscrollItem("", txt1, RGB(174,238,238), true);
		strlen = sprintf(txt1,"Total kills: {%i}.",TotalKillNumber);
		ChatterUI::GetInstance()->AddBackscrollItem("", txt1, RGB(174,238,238), true);
		strlen = sprintf(txt1,"Total deaths: {%i}.",TotalDeathNumber);
		ChatterUI::GetInstance()->AddBackscrollItem("", txt1, RGB(174,238,238), true); // RGB(174,238,238)
	}
	catch(...)
	{
	}
}
