#include "XpStat.h"
#include <math.h>
#include <string>

#include "NewInterface/ChatterUI.h"



XpStat::XpStat(void)
{
	this->mXpStart = 0;
	this->mStartMs = 0;
	this->mStarted = false;
	
}
XpStat::~XpStat(void){}

void XpStat::Init(__int64 pXpStart){

	this->mXpStart = pXpStart;
	this->mStartMs = GetTickCount();

	this->mStarted = true;
	
	ChatterUI::GetInstance()->AddBackscrollItem("", "XPstat: system started", RGB(255, 0, 0), true);
}
void XpStat::CalculateAndShowXpStat(__int64 pXpNow, __int64 pExp2Go){
	try{

		if(this->mStarted == false){
			Init(pXpNow);
			return;
		}


		//caculation of elapsed time and elapsed xp
		__int64 elapsed_xp = pXpNow-this->mXpStart;

		__int32 now = GetTickCount();
		__int32 elapsed_time = (now - this->mStartMs) / 1000;

		//unsigned short hour1 = floor(static_cast<double>(elapsed_time) /(60.0*60.0));
		//unsigned short min1  = floor(static_cast<double>(elapsed_time) / 60.0 - ((double)hour1 * 60.0));
		//unsigned short sec1  = floor(static_cast<double>(elapsed_time) - static_cast<double>(hour1)*3600.0 - static_cast<double>(min1) *60.0);

		// By Asteryth
		int hour_byAste = elapsed_time / 3600;
		int remaining_time_byAste = elapsed_time - (3600 * hour_byAste);
		int min_byAste  = remaining_time_byAste / 60;
		remaining_time_byAste -= min_byAste * 60;
		int sec_byAste  = remaining_time_byAste;
		
		char texte1[200];
		// by Asteryth : modified "Started at" by "Calculation time" and hour1, min1 and sec1 by mine
		int t1n = sprintf(texte1,"Calculation time: {%02i:%02i:%02i}. You've earned: {%i xp}.",hour_byAste,min_byAste,sec_byAste,elapsed_xp);
		ChatterUI::GetInstance()->AddBackscrollItem("",texte1,RGB(255,0,0),true);

		long double rate_moy = 0;
		long double speed_moy = 0;

		
		if(elapsed_time > 0){

			rate_moy = static_cast<long double>((long double)elapsed_xp / elapsed_time);
			speed_moy = static_cast<long double>(rate_moy * 3600.0);

		}
		// by Asteryth modification of xp written
		int billion = (int)speed_moy / 1000000000;
		int remaining_xp = speed_moy - (billion * 1000000000);
		int million = remaining_xp / 1000000;
		remaining_xp -= million * 1000000;
		int thousand = remaining_xp / 1000;
		remaining_xp -= thousand * 1000;

		char buffer[100];

		if (billion > 0)
			sprintf(buffer,"%i %03i %03i %03i",billion, million, thousand, remaining_xp);
		else if (million > 0)
			sprintf(buffer,"%i %03i %03i",million, thousand, remaining_xp);
		else if (thousand > 0)
			sprintf(buffer,"%i %03i",thousand, remaining_xp);
		else
			sprintf(buffer,"%i",remaining_xp);

		char texte2[200];
		// by Asteryth modified {%0.2f xp/h}. ", speed_moy); by  {%s xp/h}.",buffer);
		int t2n = sprintf(texte2,"Average speed: {%s xp/h}.",buffer);
		ChatterUI::GetInstance()->AddBackscrollItem("",texte2,RGB(255,0,0),true);



	}
	catch(...)
	{
	}
}
