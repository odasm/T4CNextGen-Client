#include "pch.h"
#include <windows.h>
#include <math.h>
#include "Puppet.h"
#include "TFCPlayer.h"
#include "TFCObject.h"
#include "COMM.h"
#include "Global.h"
#include "MemoryManager.h"


UINT Puppet::Pow2[32];
BOOL Puppet::Pow2Init = FALSE;
      
void Puppet::Release(void) {
   int i;
   for (i = 0; i < MAX_PUP_PART; i++) {
      LOG << "PUP PART: " << i << "\r\n";
      PuppetPart[i].Release();
   }
}

BOOL Puppet::Release(DWORD Wait) {
   return FALSE;
}

Puppet::Puppet(void) {
   LastAccess = timeGetTime();
   if (!Pow2Init) {
      Pow2Init = TRUE;
      for (int i = 0; i < 32; i++)
         Pow2[i] = (UINT)pow(2, i);
   }
}

Puppet::~Puppet(void) {
   Release();
}

#define KEEPALIVE	if (timeGetTime() - dwLoadTime > 1500) {\
		TFCPacket Send; \
		dwLoadTime = timeGetTime();\
		Send << (short)10;\
		SEND_PACKET(Send);	\
		COMM.KeepAlive();\
	}
		//07/12/2010 je remet le sleep(50) là dedans ^^ 
		//Sleep(50);\ je le revire.
      
void Puppet::LoadBodyPart(void) {
	DWORD dwLoadTime = timeGetTime();
	//DWORD dwLoadTime2 = timeGetTime();//BLBLBL 29 mars 2009 suppression des trucs inutilisés
	
	PuppetPart[0].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedHandL",  0, 0, 0);
	PuppetPart[1].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherGloveL", 0, 0, 0); 
	PuppetPart[2].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL",  0, 0, 0);
	PuppetPart[3].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedArmL",   0, 0, 0); 
	PuppetPart[4].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmL",   0, 0, 0); 
	PuppetPart[5].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedFoot",   0, 0, 0); 
	PuppetPart[6].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedLegs",   0, 0, 0); 
	PuppetPart[7].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherPants",  0, 0, 0);
	PuppetPart[8].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs",   0, 0, 0); 
	PuppetPart[9].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupChainMailLegs",   0, 0, 0);
	
	KEEPALIVE;
	
	PuppetPart[10].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLegsClothSet1", 0, 0, 0);
	PuppetPart[11].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupStuddedLegs", 0, 0, 0);
	PuppetPart[12].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedBody",   0, 0, 0); 
	PuppetPart[13].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherBody",   0, 0, 0);
	PuppetPart[14].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody",   0, 0, 0); 
	PuppetPart[15].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupChainMailBody",   0, 0, 0); 
	PuppetPart[16].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNecromanRobe", 0, 0, 0);
	PuppetPart[17].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupStuddedBodyArmor", 0, 0, 0);
	PuppetPart[18].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupBodyClothSet1", 0, 0, 0);
	PuppetPart[19].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0); 
	
	KEEPALIVE;
	
	PuppetPart[20].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedRobe",   0, 0, 0);
	PuppetPart[21].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupMageRobe", 0, 0, 0);
	PuppetPart[22].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedHead",   0, 0, 0);
	PuppetPart[23].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm",   0, 0, 0);
	PuppetPart[24].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupChainMailCoif",   0, 0, 0);
	PuppetPart[25].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupHornedHelmet", 0, 0, 0);
	PuppetPart[26].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedHandR",  0, 0, 0);
	PuppetPart[27].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherGloveR", 0, 0, 0); 
	PuppetPart[28].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR",  0, 0, 0);
	PuppetPart[29].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedArmR",   0, 0, 0); 
	
	KEEPALIVE;
	
	PuppetPart[30].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmR",   0, 0, 0); 
	PuppetPart[31].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupBattleAxe",   0, 0, 0);
	PuppetPart[32].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupDarkSword",   0, 0, 0);
	PuppetPart[33].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupFlail",		  0, 0, 0);
	PuppetPart[34].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupNormalSword", 0, 0, 0);
	PuppetPart[35].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupBattleSword", 0, 0, 0);
	PuppetPart[36].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupRealDarkSword", 0, 0, 0);
	PuppetPart[37].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupMorningStar", 0, 0, 0);
	PuppetPart[38].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupGoldenMorningStar", 0, 0, 0);
	PuppetPart[39].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupBattleDagger",   0, 0, 0);
	
    KEEPALIVE;
	
	PuppetPart[40].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupWarHammer", 0, 0, 0);
	PuppetPart[41].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupMace", 0, 0, 0);
	PuppetPart[42].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupRomanShield", 0, 0, 0);
	PuppetPart[43].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupBarossaShield", 0, 0, 0);
	PuppetPart[44].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupOrcShield",     0, 0, 0);
	PuppetPart[45].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherBoots",  0, 0, 0);
	PuppetPart[46].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot",   0, 0, 0); 
	PuppetPart[47].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupBlackLeatherBoots", 0, 0, 0);
	PuppetPart[48].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupGoldenCrown",   0, 0, 0);
	PuppetPart[49].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupElvenHat", 0, 0, 0);
	
    KEEPALIVE;
	
	PuppetPart[50].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape",   0, 0, 0);
	PuppetPart[51].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupArmoredRobeUnder",   0, 0, 0);
	PuppetPart[52].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0);
	PuppetPart[53].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedHead", 0, 0, 0);
	PuppetPart[54].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedBody", 0, 0, 0);
	PuppetPart[55].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedLegs", 0, 0, 0);
	PuppetPart[56].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedFeet", 0, 0, 0);
	PuppetPart[57].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedHandR", 0, 0, 0);
	PuppetPart[58].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedHandL", 0, 0, 0);
	PuppetPart[59].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedArmR", 0, 0, 0);
	
	KEEPALIVE;   
	
	PuppetPart[60].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedArmL", 0, 0, 0);
	PuppetPart[61].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0);
	PuppetPart[62].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherLegs", 0, 0, 0);
	PuppetPart[63].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherBody", 0, 0, 0);
	PuppetPart[64].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoStuddedLegs", 0, 0, 0);
	PuppetPart[65].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoStuddedBody", 0, 0, 0);
	PuppetPart[66].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoChainLegs", 0, 0, 0);
	PuppetPart[67].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoChainBody", 0, 0, 0);
	PuppetPart[68].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0);
	PuppetPart[69].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0);
	
	KEEPALIVE;
	
	PuppetPart[70].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoClothRobe", 0, 0, 0);
	PuppetPart[71].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoClothBody", 0, 0, 0);
	PuppetPart[72].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoHairPonyTail", 0, 0, 0);
   
	PuppetPart[73].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherArms", 0, 0, 0);
	PuppetPart[74].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherBoots", 0, 0, 0);
	PuppetPart[75].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherGloveL", 0, 0, 0);
	PuppetPart[76].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherGloveR", 0, 0, 0);
	PuppetPart[77].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupRedCapeTop", 0, 0, 0); //NAD
	PuppetPart[78].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoHairPonyTail", 0, 0, 0);
	PuppetPart[79].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupWoodenStaff", 0, 0, 0);
	
	KEEPALIVE;
	
	PuppetPart[80].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupLichStaff", 0, 0, 0);
	PuppetPart[81].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupGemStaff", 0, 0, 0);
	PuppetPart[82].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupSimpleStaff", 0, 0, 0);
	PuppetPart[83].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0);
	PuppetPart[84].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0);
	PuppetPart[85].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0);
	PuppetPart[86].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoBlackLeatherBoots", 0, 0, 0);
	PuppetPart[87].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoChainMailCoif", 0, 0, 0);
	PuppetPart[88].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNormalHair", 0, 0, 0);
	
	
	KEEPALIVE;
	
	PuppetPart[90].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherHelm", 0, 0, 0);
	PuppetPart[91].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherHelm", 0, 0, 0);
	PuppetPart[92].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNecromanRobe", 0, 0, 0);
	PuppetPart[93].LoadSprite3D(13, 0, 0, 0, 0, 9, "PupShortBow", 0, 0, 0 );
	PuppetPart[94].LoadSprite3D(13, 0, 0, 0, 0, 9, "PupLongBow", 0, 0, 0 );
	PuppetPart[95].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoRedRobe", 0, 0, 0 );
	PuppetPart[96].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupOgreClub", 0, 0, 0 );
	PuppetPart[97].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoMageRobe", 0, 0, 0 );
	PuppetPart[98].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoMageRobeUnder", 0, 0, 0 );
	PuppetPart[99].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupMageRobeCollar", 0, 0, 0 );
	
	KEEPALIVE;
	
	PuppetPart[100].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupCentaurShield2", 0, 0, 0 );
	PuppetPart[101].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupCentaurShield", 0, 0, 0 );
	PuppetPart[102].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupShamanHelm", 0, 0, 0 );
	PuppetPart[103].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupSkavenClub", 0, 0, 0 );
	PuppetPart[104].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupSkavenKnife", 0, 0, 0 );
	PuppetPart[105].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupSkavenShield1", 0, 0, 0 );
	PuppetPart[106].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupSkavenShield2", 0, 0, 0 );
	PuppetPart[107].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupSkavenShield3", 0, 0, 0 );
	PuppetPart[108].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupSkeletonAxe", 0, 0, 0 );
	PuppetPart[109].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSkeletonHelm", 0, 0, 0 );
	
	KEEPALIVE;
	
	PuppetPart[110].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupSkeletonShield", 0, 0, 0 );
	PuppetPart[111].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupSkeletonSword", 0, 0, 0 );
	PuppetPart[112].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphBlackWings", 0, 0, 0 );
	PuppetPart[113].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0 );
	PuppetPart[114].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupHalberd", 0, 0, 0 );
	
	//Dialsoft new graphics
	PuppetPart[115].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedFeHelmet", 0, 0, 0 );
	PuppetPart[116].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupFireFlail", 0, 0, 0 );
	PuppetPart[117].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSpikeLeatherBody", 0, 0, 0 );
	
	// Wings & derives
	PuppetPart[118].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphDarkWings", 0, 0, 0 );
	PuppetPart[119].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 3 );
	
	KEEPALIVE;
	
	PuppetPart[120].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 2 );
	PuppetPart[121].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 4 );
	PuppetPart[122].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 5 );
	PuppetPart[123].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 6 );
	PuppetPart[124].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 7 );
	PuppetPart[125].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 8 );
	
	// Robe derives
	PuppetPart[126].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 2); 
	PuppetPart[127].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 3); 
	PuppetPart[128].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 4); 
	PuppetPart[129].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 5); 
	
	KEEPALIVE;
	
	PuppetPart[130].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 6); 
	PuppetPart[131].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 7); 
	PuppetPart[132].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 8); 
	PuppetPart[133].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 9); 
	PuppetPart[134].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 10); 
	PuppetPart[135].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 11); 
	PuppetPart[136].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe",   0, 0, 0, 12); 
	PuppetPart[137].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 2); 
	PuppetPart[138].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 3); 
	PuppetPart[139].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 4); 
	
	KEEPALIVE;
	
	PuppetPart[140].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 5); 
	PuppetPart[141].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 6); 
	PuppetPart[142].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 7); 
	PuppetPart[143].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 8); 
	PuppetPart[144].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 9); 
	PuppetPart[145].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 10); 
	PuppetPart[146].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 11); 
	PuppetPart[147].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe",   0, 0, 0, 12); 

	KEEPALIVE

	//Tiamat Changes
	//GOLD
	PuppetPart[148].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL",  0, 0, 0, 2);
	PuppetPart[149].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmL",   0, 0, 0, 2); 
	PuppetPart[150].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs",   0, 0, 0, 2); 
	PuppetPart[151].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody",   0, 0, 0, 2); 
	PuppetPart[152].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm",   0, 0, 0, 2);
	PuppetPart[153].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR",  0, 0, 0, 2);
	PuppetPart[154].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmR",   0, 0, 0, 2); 
	PuppetPart[155].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot",   0, 0, 0, 2); 

	KEEPALIVE

	//SILVER
	PuppetPart[156].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL",  0, 0, 0, 3);
	PuppetPart[157].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmL",   0, 0, 0, 3); 
	PuppetPart[158].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs",   0, 0, 0, 3); 
	PuppetPart[159].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody",   0, 0, 0, 3); 
	PuppetPart[160].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm",   0, 0, 0, 3);
	PuppetPart[161].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR",  0, 0, 0, 3);
	PuppetPart[162].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmR",   0, 0, 0, 3); 
	PuppetPart[163].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot",   0, 0, 0, 3); 
	
	KEEPALIVE

	//BLACK
	PuppetPart[164].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL",  0, 0, 0, 4);
	PuppetPart[165].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmL",   0, 0, 0, 4); 
	PuppetPart[166].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs",   0, 0, 0, 4); 
	PuppetPart[167].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody",   0, 0, 0, 4); 
	PuppetPart[168].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm",   0, 0, 0, 4);
	PuppetPart[169].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR",  0, 0, 0, 4);
	PuppetPart[170].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmR",   0, 0, 0, 4); 
	PuppetPart[171].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot",   0, 0, 0, 4); 
	
	KEEPALIVE

	//BLUE
	PuppetPart[172].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL",  0, 0, 0, 5);
	PuppetPart[173].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmL",   0, 0, 0, 5); 
	PuppetPart[174].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs",   0, 0, 0, 5); 
	PuppetPart[175].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody",   0, 0, 0, 5); 
	PuppetPart[176].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm",   0, 0, 0, 5);
	PuppetPart[177].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR",  0, 0, 0, 5);
	PuppetPart[178].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmR",   0, 0, 0, 5); 
	PuppetPart[179].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot",   0, 0, 0, 5); 
	
	KEEPALIVE

	//GREEN
	PuppetPart[180].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL",  0, 0, 0, 6);
	PuppetPart[181].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmL",   0, 0, 0, 6); 
	PuppetPart[182].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs",   0, 0, 0, 6); 
	PuppetPart[183].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody",   0, 0, 0, 6); 
	PuppetPart[184].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm",   0, 0, 0, 6);
	PuppetPart[185].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR",  0, 0, 0, 6);
	PuppetPart[186].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmR",   0, 0, 0, 6); 
	PuppetPart[187].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot",   0, 0, 0, 6); 
	
	KEEPALIVE

	//RED
	PuppetPart[188].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL",  0, 0, 0, 7);
	PuppetPart[189].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmL",   0, 0, 0, 7); 
	PuppetPart[190].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs",   0, 0, 0, 7); 
	PuppetPart[191].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody",   0, 0, 0, 7); 
	PuppetPart[192].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm",   0, 0, 0, 7);
	PuppetPart[193].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR",  0, 0, 0, 7);
	PuppetPart[194].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmR",   0, 0, 0, 7); 
	PuppetPart[195].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot",   0, 0, 0, 7); 
	
	KEEPALIVE

	//PURPLE
	PuppetPart[196].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL",  0, 0, 0, 8);
	PuppetPart[197].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmL",   0, 0, 0, 8); 
	PuppetPart[198].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs",   0, 0, 0, 8); 
	PuppetPart[199].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody",   0, 0, 0, 8); 
	PuppetPart[200].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm",   0, 0, 0, 8);
	PuppetPart[201].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR",  0, 0, 0, 8);
	PuppetPart[202].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmR",   0, 0, 0, 8); 
	PuppetPart[203].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot",   0, 0, 0, 8); 
	
	KEEPALIVE

	//EMERALD
	PuppetPart[204].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL",  0, 0, 0, 9);
	PuppetPart[205].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmL",   0, 0, 0, 9); 
	PuppetPart[206].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs",   0, 0, 0, 9); 
	PuppetPart[207].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody",   0, 0, 0, 9); 
	PuppetPart[208].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm",   0, 0, 0, 9);
	PuppetPart[209].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR",  0, 0, 0, 9);
	PuppetPart[210].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmR",   0, 0, 0, 9); 
	PuppetPart[211].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot",   0, 0, 0, 9); 
	
	KEEPALIVE

	//FEMALE GOLD
	PuppetPart[212].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 2);
	PuppetPart[213].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 2);
	PuppetPart[214].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 2);
	PuppetPart[215].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 2);
	PuppetPart[216].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 2);
	PuppetPart[217].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 2);
	
	KEEPALIVE

	//FEMALE SILVER
	PuppetPart[218].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 3);
	PuppetPart[219].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 3);
	PuppetPart[220].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 3);
	PuppetPart[221].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 3);
	PuppetPart[222].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 3);
	PuppetPart[223].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 3);
	
	KEEPALIVE

	//FEMALE BLACK
	PuppetPart[224].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 4);
	PuppetPart[225].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 4);
	PuppetPart[226].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 4);
	PuppetPart[227].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 4);
	PuppetPart[228].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 4);
	PuppetPart[229].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 4);
	
	KEEPALIVE

	//FEMALE BLUE
	PuppetPart[230].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 5);
	PuppetPart[231].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 5);
	PuppetPart[232].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 5);
	PuppetPart[233].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 5);
	PuppetPart[234].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 5);
	PuppetPart[235].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 5);
	
	KEEPALIVE

	//FEMALE GREEN
	PuppetPart[236].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 6);
	PuppetPart[237].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 6);
	PuppetPart[238].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 6);
	PuppetPart[239].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 6);
	PuppetPart[240].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 6);
	PuppetPart[241].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 6);
	
	KEEPALIVE

	//FEMALE RED
	PuppetPart[242].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 7);
	PuppetPart[243].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 7);
	PuppetPart[244].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 7);
	PuppetPart[245].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 7);
	PuppetPart[246].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 7);
	PuppetPart[247].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 7);
	
	KEEPALIVE

	//FEMALE PURPLE
	PuppetPart[248].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 8);
	PuppetPart[249].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 8);
	PuppetPart[250].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 8);
	PuppetPart[251].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 8);
	PuppetPart[252].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 8);
	PuppetPart[253].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 8);
	
	KEEPALIVE

	//FEMALE EMERALD
	PuppetPart[254].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 9);
	PuppetPart[255].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 9);
	PuppetPart[256].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 9);
	PuppetPart[257].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 9);
	PuppetPart[258].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 9);
	PuppetPart[259].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 9);
	
	//NEW WEAPONS
	PuppetPart[260].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupTigerAxe", 0, 0, 0);
	PuppetPart[261].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupAncientAxe", 0, 0, 0);
	PuppetPart[262].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupGobmask", 0, 0, 0);
	PuppetPart[263].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupFlower", 0, 0, 0);
	PuppetPart[264].LoadSprite3D(13, 9, 0, 0, 0, 0, "PupNecroStaff", 0, 0, 0);
	//Dialsoft END
   KEEPALIVE

   // NEW CAPES    
   PuppetPart[265].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape", 0, 0, 0, 2);	
   PuppetPart[266].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape", 0, 0, 0, 3);
   PuppetPart[267].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape", 0, 0, 0, 4);
   PuppetPart[268].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape", 0, 0, 0, 5);
   PuppetPart[269].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape", 0, 0, 0, 6);
   PuppetPart[270].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape", 0, 0, 0, 7);
   PuppetPart[271].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape", 0, 0, 0, 8);

   KEEPALIVE;

   PuppetPart[272].LoadSprite3D(13, 9, 0, 0, 0, 9, "WitchHat1", 0, 0, 0);
   PuppetPart[273].LoadSprite3D(13, 9, 0, 0, 0, 9, "WitchHat2", 0, 0, 0);
   PuppetPart[274].LoadSprite3D(13, 9, 0, 0, 0, 9, "WitchHat3", 0, 0, 0);
   PuppetPart[275].LoadSprite3D(13, 0, 0, 0, 0, 9, "NewBow01", 0, 0, 0 );
   PuppetPart[276].LoadSprite3D(13, 9, 0, 0, 0, "DwarfHammer", 0, 0, 0); 
   PuppetPart[277].LoadSprite3D(13, 9, 0, 0, 0, "AxeDestruction", 0, 0, 0); 
   PuppetPart[278].LoadSprite3D(13, 9, 0, 0, 0, "SwordAngel", 0, 0, 0); 
   PuppetPart[279].LoadSprite3D(13, 9, 0, 0, 0, "Clay2blade", 0, 0, 0); 
   PuppetPart[280].LoadSprite3D(13, 9, 0, 0, 0, "SwordAngel", 0, 0, 0); 

   KEEPALIVE;

   PuppetPart[281].LoadSprite3D(13, 9, 0, 0, 0, 9, "ButterFlyWing", 0, 0, 0);
   PuppetPart[282].LoadSprite3D(13, 9, 0, 0, 0, 9, "ButterFlyWing", 0, 0, 0,2);
   PuppetPart[283].LoadSprite3D(13, 9, 0, 0, 0, 9, "ButterFlyWing", 0, 0, 0,3);
   PuppetPart[284].LoadSprite3D(13, 9, 0, 0, 0, 9, "ButterFlyWing", 0, 0, 0,4);
   KEEPALIVE;
   PuppetPart[285].LoadSprite3D(13, 9, 0, 0, 0, "Ham", 0, 0, 0); 
   PuppetPart[286].LoadSprite3D(13, 9, 0, 0, 0, 9, "ArchWings", 0, 0, 0);
   PuppetPart[287].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoArmoredRobe", 0, 0, 0);
   PuppetPart[288].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupArmoredRobe", 0, 0, 0 );
   PuppetPart[289].LoadSprite3D(13, 9, 0, 0, 0, 9, "NoelHat", 0, 0, 0); 
   PuppetPart[290].LoadSprite3D(13, 9, 0, 0, 0, "PupOrcShield2", 0, 0, 0 );
   PuppetPart[291].LoadSprite3D(13, 9, 0, 0, 0, "SkShield"    , 0, 0, 0 );
   KEEPALIVE;
   PuppetPart[292].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Haume01", 0, 0, 0);
   PuppetPart[293].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Haume02", 0, 0, 0);
   PuppetPart[294].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Haume03", 0, 0, 0);
   PuppetPart[295].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Haume04", 0, 0, 0); 
   PuppetPart[296].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Hat01", 0, 0, 0);
   PuppetPart[297].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Mask01", 0, 0, 0);
   KEEPALIVE;
   PuppetPart[298].LoadSprite3D(13, 9, 0, 0, 0, "V2_Viperine", 0, 0, 0);
   PuppetPart[299].LoadSprite3D(13, 9, 0, 0, 0, "V2_Dague01" , 0, 0, 0);
   PuppetPart[300].LoadSprite3D(13, 9, 0, 0, 0, "V2_Dague02" , 0, 0, 0);
   PuppetPart[301].LoadSprite3D(13, 9, 0, 0, 0, "V2_Dague03" , 0, 0, 0); 
   PuppetPart[302].LoadSprite3D(13, 9, 0, 0, 0, "V2_Dague04" , 0, 0, 0);
   PuppetPart[303].LoadSprite3D(13, 9, 0, 0, 0, "V2_Dague05" , 0, 0, 0);
   KEEPALIVE;
   //NM_TAG_ADD_SWORD
   PuppetPart[304].LoadSprite3D(13, 9, 0, 0, 0, "V2_BusterSlayer01" , 0, 0, 0);
   PuppetPart[305].LoadSprite3D(13, 9, 0, 0, 0, "V2_BusterSlayer02" , 0, 0, 0);
   PuppetPart[306].LoadSprite3D(13, 9, 0, 0, 0, "V2_BusterSlayer03" , 0, 0, 0);
   PuppetPart[307].LoadSprite3D(13, 9, 0, 0, 0, "V2_Claymore01" , 0, 0, 0);
   PuppetPart[308].LoadSprite3D(13, 9, 0, 0, 0, "V2_Claymore02" , 0, 0, 0);
   PuppetPart[309].LoadSprite3D(13, 9, 0, 0, 0, "V2_GlaiveSlayer01" , 0, 0, 0);
   PuppetPart[310].LoadSprite3D(13, 9, 0, 0, 0, "V2_GlaiveSlayer02" , 0, 0, 0);
   KEEPALIVE;
   PuppetPart[311].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword01" , 0, 0, 0);
   PuppetPart[312].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword02" , 0, 0, 0);
   PuppetPart[313].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword03" , 0, 0, 0);
   PuppetPart[314].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword04" , 0, 0, 0);
   PuppetPart[315].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword05" , 0, 0, 0);
   PuppetPart[316].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword06" , 0, 0, 0);
   PuppetPart[317].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword07" , 0, 0, 0);
   PuppetPart[318].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword08" , 0, 0, 0);
   KEEPALIVE;
   PuppetPart[319].LoadSprite3D(13, 9, 0, 0, 0, "V2_2BusterSlayer01" , 0, 0, 0);
   PuppetPart[320].LoadSprite3D(13, 9, 0, 0, 0, "V2_2BusterSlayer02" , 0, 0, 0);
   PuppetPart[321].LoadSprite3D(13, 9, 0, 0, 0, "V2_2BusterSlayer03" , 0, 0, 0);
   PuppetPart[322].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Claymore01" , 0, 0, 0);
   PuppetPart[323].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Claymore02" , 0, 0, 0);
   PuppetPart[324].LoadSprite3D(13, 9, 0, 0, 0, "V2_2GlaiveSlayer01" , 0, 0, 0);
   PuppetPart[325].LoadSprite3D(13, 9, 0, 0, 0, "V2_2GlaiveSlayer02" , 0, 0, 0);
   KEEPALIVE;
   PuppetPart[326].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword01" , 0, 0, 0);
   PuppetPart[327].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword02" , 0, 0, 0);
   PuppetPart[328].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword03" , 0, 0, 0);
   PuppetPart[329].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword04" , 0, 0, 0);
   PuppetPart[330].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword05" , 0, 0, 0);
   PuppetPart[331].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword06" , 0, 0, 0);
   PuppetPart[332].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword07" , 0, 0, 0);
   PuppetPart[333].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword08" , 0, 0, 0);
   KEEPALIVE;
   PuppetPart[334].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache01" , 0, 0, 0);
   PuppetPart[335].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache02" , 0, 0, 0);
   PuppetPart[336].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache03" , 0, 0, 0);
   PuppetPart[337].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache04" , 0, 0, 0);
   PuppetPart[338].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache05" , 0, 0, 0);
   PuppetPart[339].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache06" , 0, 0, 0);
   PuppetPart[340].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache07" , 0, 0, 0);
   PuppetPart[341].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache08" , 0, 0, 0);
   PuppetPart[342].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache09" , 0, 0, 0);
   PuppetPart[343].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache10" , 0, 0, 0);
   PuppetPart[344].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache11" , 0, 0, 0);
   KEEPALIVE;
   PuppetPart[345].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache01" , 0, 0, 0);
   PuppetPart[346].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache02" , 0, 0, 0);
   PuppetPart[347].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache03" , 0, 0, 0);
   PuppetPart[348].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache04" , 0, 0, 0);
   PuppetPart[349].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague01" , 0, 0, 0);
   PuppetPart[350].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague02" , 0, 0, 0);
   PuppetPart[351].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague03" , 0, 0, 0); 
   PuppetPart[352].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague04" , 0, 0, 0);
   PuppetPart[353].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague05" , 0, 0, 0);
   PuppetPart[354].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague06" , 0, 0, 0);
   KEEPALIVE;
   
   
   PuppetPart[355].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow01", 0, 0, 0); 
   PuppetPart[356].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow02", 0, 0, 0); 
   PuppetPart[357].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow03", 0, 0, 0); 
   PuppetPart[358].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow04", 0, 0, 0); 
   PuppetPart[359].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow05", 0, 0, 0); 
   PuppetPart[360].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow06", 0, 0, 0); 
   PuppetPart[361].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow07", 0, 0, 0); 
   PuppetPart[362].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow08", 0, 0, 0); 
   PuppetPart[363].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow09", 0, 0, 0); 
   PuppetPart[364].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow10", 0, 0, 0); 
   KEEPALIVE;
   PuppetPart[365].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sceptre01", 0, 0, 0); 
   PuppetPart[366].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sceptre02", 0, 0, 0); 
   PuppetPart[367].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sceptre03", 0, 0, 0); 
   PuppetPart[368].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special01", 0, 0, 0);
   PuppetPart[369].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special02", 0, 0, 0);
   PuppetPart[370].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special03", 0, 0, 0);
   PuppetPart[371].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special04", 0, 0, 0);
   PuppetPart[372].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special05", 0, 0, 0);
   PuppetPart[373].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special06", 0, 0, 0);
   PuppetPart[374].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special07", 0, 0, 0);
   KEEPALIVE;
   PuppetPart[375].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special01", 0, 0, 0);
   PuppetPart[376].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special02", 0, 0, 0);
   PuppetPart[377].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special03", 0, 0, 0);
   PuppetPart[378].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special04", 0, 0, 0);
   PuppetPart[379].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special05", 0, 0, 0);
   PuppetPart[380].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special06", 0, 0, 0);
   KEEPALIVE;
   PuppetPart[381].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer01", 0, 0, 0);
   PuppetPart[382].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer02", 0, 0, 0);
   PuppetPart[383].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer03", 0, 0, 0);
   PuppetPart[384].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer04", 0, 0, 0);
   PuppetPart[385].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer05", 0, 0, 0);
   PuppetPart[386].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer06", 0, 0, 0);
   PuppetPart[387].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer07", 0, 0, 0);
   PuppetPart[388].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hammer01", 0, 0, 0);
   PuppetPart[389].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hammer02", 0, 0, 0);
   KEEPALIVE;
   PuppetPart[390].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache05" , 0, 0, 0);
   PuppetPart[391].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache06" , 0, 0, 0);
   PuppetPart[392].LoadSprite3D(13, 9, 0, 0, 0, "V2_Shield01", 0, 0, 0 );
   PuppetPart[393].LoadSprite3D(13, 9, 0, 0, 0, "V2_Shield02", 0, 0, 0 );
   PuppetPart[394].LoadSprite3D(13, 9, 0, 0, 0, "V2_IceSword01" , 0, 0, 0);
   PuppetPart[395].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_IceBow01", 0, 0, 0); 
   PuppetPart[396].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupGobmask", 0, 0, 0,2); 
   PuppetPart[397].LoadSprite3D(13, 9, 0, 0, 0, "V2_2IceSword01" , 0, 0, 0);
   KEEPALIVE;
   PuppetPart[398].LoadSprite3D(13, 9, 0, 0, 0, 9, "WitchHat4", 0, 0, 0);
   PuppetPart[399].LoadSprite3D(13, 9, 0, 0, 0, 9, "WitchHat5", 0, 0, 0);
   PuppetPart[400].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBody01" , 0, 0, 0);
	PuppetPart[401].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLegs01" , 0, 0, 0);
	PuppetPart[402].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLGlove01", 0, 0, 0); 
	PuppetPart[403].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBoots01", 0, 0, 0);
   PuppetPart[404].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBody01" , 0, 0, 0,2);
	PuppetPart[405].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLegs01" , 0, 0, 0,2);
   KEEPALIVE;
	PuppetPart[406].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLGlove01", 0, 0, 0,2); 
	PuppetPart[407].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBoots01", 0, 0, 0,2);
   PuppetPart[408].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorRGlove01", 0, 0, 0); 
	PuppetPart[409].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorRGlove01", 0, 0, 0,2); 
   PuppetPart[410].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Haume04", 0, 0, 0,2); 
   PuppetPart[411].LoadSprite3D(13, 9, 0, 0, 0, 9, "NM_DechuWings", 0, 0, 0); 
   KEEPALIVE;   
   
   PuppetPart[412].LoadSprite3D(13, 9, 0, 0, 0, 9, "T4CP_Casque", 0, 0, 0); 
   PuppetPart[413].LoadSprite3D(13, 9, 0, 0, 0, 9, "T4CP_Casquette", 0, 0, 0); 
   PuppetPart[414].LoadSprite3D(13, 9, 0, 0, 0, "T4CP_Shield", 0, 0, 0 );
   PuppetPart[415].LoadSprite3D(13, 9, 0, 0, 0, "T4CP_Matraque" , 0, 0, 0);
   PuppetPart[416].LoadSprite3D(13, 9, 0, 0, 0, "TankBat" , 0, 0, 0);
   PuppetPart[417].LoadSprite3D(13, 9, 0, 0, 0, "T4CP_12" , 0, 0, 0);
   PuppetPart[418].LoadSprite3D(13, 9, 0, 0, 0, "V2_IceShield01", 0, 0, 0 );
   KEEPALIVE;   
   PuppetPart[419].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupBlackLeatherBoots", 0, 0, 0,2);
   PuppetPart[420].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoBlackLeatherBoots", 0, 0, 0,2);

    PuppetPart[422].LoadSprite3D(13, 9, 0, 0, 0, 9, "ManLichRobeOri", 0, 0, 0);//BLBLBL ajouté la liche robe
    PuppetPart[423].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLichRobeOri", 0, 0, 0);//BLBLBL ajouté la liche robe

	
   PuppetPart[421].LoadSprite3D(13, 9, 0, 0, 0, 9, "NM_DechuWings", 0, 0, 0); //BLBLBL remplacé NM2_DechuWings (peut être buggé) par les normales NM_DechuWings
   
   //BLBLBL adds :
   //PuppetPart[422].LoadSprite3D(13, 9, 0, 0, 0, 9, "ManLichRobeOri", 0, 0, 0);//BLBLBL ajouté la liche robe
   //PuppetPart[423].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLichRobeOri", 0, 0, 0);//BLBLBL ajouté la liche robe

   KEEPALIVE;   
   
}

bool IsRangedObject( TFCObject *obj );

void Puppet::DrawSprite3D(int Direction, int Frame, int X, int Y, BYTE Stand, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *Object) 
{
   
   LastAccess = timeGetTime();
   if (Stand == ST_ATTACK_STANDING || Stand == ST_ATTACK) 
   {
      if( IsRangedObject( Object ) )
      {
         for (int i = 0; i < Object->nbBodyPart; i++) 
         {
            if (Object->VisiblePart & Pow2[BodyOrderAR[Direction][i][0]]) 
            {
               if(Object->BodyPart[BodyOrderAR[Direction][i][0]]!=NULL) //NMNMNM NMNMNM
                  Object->BodyPart[BodyOrderAR[Direction][i][0]]->DrawSprite3DR(Direction, 0, X, Y, Clip, boOutline, bOutlineColor, Surface, Format, Object);
               //Object->BodyPart[BodyOrderAR[Direction][i][0]]->DrawSprite3DR(Direction, 0, X, Y, Stand, Clip, boOutline, bOutlineColor, Surface, Format, Object);
            }
         }
      }
      else
      {
         for (int i = 0; i < Object->nbBodyPart; i++) 
         {
            if (Object->VisiblePart & Pow2[BodyOrderA[Direction][i][0]]) 
            {
               if(Object->BodyPart[BodyOrderA[Direction][i][0]]!=NULL)//NMNMNM NMNMNM
                  Object->BodyPart[BodyOrderA[Direction][i][0]]->DrawSprite3D(Direction, 0, X, Y, Stand, Clip, boOutline, bOutlineColor, Surface, Format, Object);
            }
         }
      }
   } 
   else 
   {
      int i = 0;
      try 
      {
         if( IsRangedObject( Object ) )
         {
            for (i = 0; i < Object->nbBodyPart; i++) 
            {
               if (Object->VisiblePart & Pow2[BodyOrderR[Direction][i][Frame]]) 
               {
                  if(Object->BodyPart[BodyOrderR[Direction][i][Frame]]!=NULL)//NMNMNM NMNMNM
                     Object->BodyPart[BodyOrderR[Direction][i][Frame]]->DrawSprite3D(Direction, Frame, X, Y, Stand, Clip, boOutline, bOutlineColor, Surface, Format, Object);
               }
            }
         }
         else
         {
            for (i = 0; i < Object->nbBodyPart; i++) 
            {
               if (Object->VisiblePart & Pow2[BodyOrder[Direction][i][Frame]]) 
               {
                  if(Object->BodyPart[BodyOrder[Direction][i][Frame]]!=NULL)//NMNMNM NMNMNM
                     Object->BodyPart[BodyOrder[Direction][i][Frame]]->DrawSprite3D(Direction, Frame, X, Y, Stand, Clip, boOutline, bOutlineColor, Surface, Format, Object);
               }
            }
         }
      } 
      catch (...) 
      {
         LOG << "* ERR = x9-2, ";
         try 
         {
            LOG << (int)Object << ", " << Object->nbBodyPart << ", " << Object->VisiblePart << ", " << 
               (int)BodyOrder << ", " << Direction << ", " << i << ", " << Frame << ", " << 
               (int)Object->BodyPart[BodyOrder[Direction][i][Frame]] << ", " <<
               (int)BodyOrder[Direction][i][Frame] << "\r\n";
            
            for (int j = 0; j < 75; j++) 
            {
               LOG << (int)j << " -> " << (int)&PuppetPart[j] << "\r\n";
            }
            
         } 
         catch (...) 
         {
            LOG << "* ERR = x18\r\n";
            throw;
         }
         throw;
      }
   }
}

void Puppet::DrawSprite3DA(int Direction, int Frame, int X, int Y, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *Object) {
   LastAccess = timeGetTime();
   for (int i = 0; i < Object->nbBodyPart; i++) 
   {
      if (Object->VisiblePart & Pow2[BodyOrderA[Direction][i][Frame]]) 
      {
         if(Object->BodyPart[BodyOrderA[Direction][i][Frame]])
            Object->BodyPart[BodyOrderA[Direction][i][Frame]]->DrawSprite3DA(Direction, Frame, X, Y, Clip, boOutline, bOutlineColor, Surface, Format, Object);
		}
   }
}

void Puppet::DrawSprite3DR(int Direction, int Frame, int X, int Y, RECT *Clip, BOOL boOutline, BYTE bOutlineColor, LPDIRECTDRAWSURFACE Surface, signed char Format, TFCObject *Object){
   LastAccess = timeGetTime();
   for (int i = 0; i < Object->nbBodyPart; i++) 
   {
      if (Object->VisiblePart & Pow2[BodyOrderAR[Direction][i][Frame]]) 
      {
         if(Object->BodyPart[BodyOrderAR[Direction][i][Frame]])
            Object->BodyPart[BodyOrderAR[Direction][i][Frame]]->DrawSprite3DR(Direction, Frame, X, Y, Clip, boOutline, bOutlineColor, Surface, Format, Object);
		}
   }
}

void Puppet::DrawCorpse(int CorpseFrame, int X, int Y, RECT *Clip, RECT *OutBound, RECT *Wrap, LPDIRECTDRAWSURFACE Surface, signed char Format, int AbsX, int AbsY, TFCObject *Object) {
   LastAccess = timeGetTime();
   for (int i = 0; i < Object->nbBodyPart; i++) {
      if (Object->VisiblePart & Pow2[BodyOrder[1][i][CorpseFrame]]) {
         Object->BodyPart[BodyOrder[1][i][CorpseFrame]]->DrawCorpse(CorpseFrame, X, Y, Clip, OutBound, Wrap, Surface, Format, AbsX, AbsY, Object);
		}
   }
}

void Puppet::SetPuppet(TFCObject *Object) {
   LastAccess = timeGetTime();
  
	Object->VisiblePart = 0;
		
	// HAND LEFT
	Object->VisiblePart |= Pow2[0];
   switch (Object->PuppetInfo[0]) {
		case 0: 
         if (Object->Type == 10011) {
            PuppetPart[0].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedHandL", 0, 0, 0); 
			   Object->BodyPart[0] = &PuppetPart[0];
         } else {
            PuppetPart[58].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedHandL", 0, 0, 0); 
			   Object->BodyPart[0] = &PuppetPart[58];
         }

		break;
      case 1: 
         if (Object->Type == 10011) {
            PuppetPart[1].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherGloveL", 0, 0, 0); 
			   Object->BodyPart[0] = &PuppetPart[1]; 
         } else {
            PuppetPart[75].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherGloveL", 0, 0, 0); 
			   Object->BodyPart[0] = &PuppetPart[75]; 
         }
      break;
      case 2: 
         if (Object->Type == 10011) {
   			PuppetPart[2].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL", 0, 0, 0); 
	   		Object->BodyPart[0] = &PuppetPart[2]; 
         } else {
            PuppetPart[85].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0); 
			   Object->BodyPart[0] = &PuppetPart[85]; 
         }
		break;
      case 3: 
         if (Object->Type == 10011) {
            PuppetPart[1].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherGloveL", 0, 0, 0); 
			   Object->BodyPart[0] = &PuppetPart[1]; 
         } else {
            PuppetPart[75].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherGloveL", 0, 0, 0); 
			   Object->BodyPart[0] = &PuppetPart[75]; 
         }
		break;
		//Tiamat changes
	  case PUPEQ_PLATE_GOLD:
		 if (Object->Type == 10011) {
   			PuppetPart[148].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL", 0, 0, 0, 2); 
	   		Object->BodyPart[0] = &PuppetPart[148]; 
		 } else {
			PuppetPart[217].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 2); 
			   Object->BodyPart[0] = &PuppetPart[217]; 
		 }
		break;
	  case PUPEQ_PLATE_SILVER:
		 if (Object->Type == 10011) {
   			PuppetPart[156].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL", 0, 0, 0, 3); 
	   		Object->BodyPart[0] = &PuppetPart[156]; 
		 } else {
			PuppetPart[223].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 3); 
			   Object->BodyPart[0] = &PuppetPart[223]; 
		 }
		break;
	  case PUPEQ_PLATE_BLACK:
		 if (Object->Type == 10011) {
   			PuppetPart[164].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL", 0, 0, 0, 4); 
	   		Object->BodyPart[0] = &PuppetPart[164]; 
		 } else {
			PuppetPart[229].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 4); 
			   Object->BodyPart[0] = &PuppetPart[229]; 
		 }
		break;
	case PUPEQ_PLATE_BLUE:
		 if (Object->Type == 10011) {
   			PuppetPart[172].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL", 0, 0, 0, 5); 
	   		Object->BodyPart[0] = &PuppetPart[172]; 
		 } else {
			PuppetPart[235].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 5); 
			   Object->BodyPart[0] = &PuppetPart[235]; 
		 }
		break;
	case PUPEQ_PLATE_GREEN:
		 if (Object->Type == 10011) {
   			PuppetPart[180].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL", 0, 0, 0, 6); 
	   		Object->BodyPart[0] = &PuppetPart[180]; 
		 } else {
			PuppetPart[241].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 6); 
			   Object->BodyPart[0] = &PuppetPart[241]; 
		 }
		break;
	case PUPEQ_PLATE_RED:
		 if (Object->Type == 10011) {
   			PuppetPart[188].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL", 0, 0, 0, 7); 
	   		Object->BodyPart[0] = &PuppetPart[188]; 
		 } else {
			PuppetPart[247].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 7); 
			   Object->BodyPart[0] = &PuppetPart[247]; 
		 }
		break;
	case PUPEQ_PLATE_PURPLE:
		 if (Object->Type == 10011) {
   			PuppetPart[196].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL", 0, 0, 0, 8); 
	   		Object->BodyPart[0] = &PuppetPart[196]; 
		 } else {
			PuppetPart[253].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 8); 
			   Object->BodyPart[0] = &PuppetPart[253]; 
		 }
		break;
	case PUPEQ_PLATE_EMERALD:
		 if (Object->Type == 10011) {
   			PuppetPart[204].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveL", 0, 0, 0, 9); 
	   		Object->BodyPart[0] = &PuppetPart[204]; 
		 } else {
			PuppetPart[259].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveL", 0, 0, 0, 9); 
			   Object->BodyPart[0] = &PuppetPart[259]; 
		 }
		break;
      case PUPEQ_V2_MAN_ARMOR01GR:
         if (Object->Type == 10011) 
         {
            PuppetPart[402].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorRGlove01" , 0, 0, 0);
			   Object->BodyPart[0] = &PuppetPart[402]; 
         } 
         else 
         {
            PuppetPart[402].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorRGlove01" , 0, 0, 0);
			   Object->BodyPart[0] = &PuppetPart[402]; 
         }
      break;
      case PUPEQ_V2_MAN_ARMOR01BL:
         if (Object->Type == 10011) 
         {
            PuppetPart[406].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorRGlove01" , 0, 0, 0,2);
			   Object->BodyPart[0] = &PuppetPart[406]; 
         } 
         else 
         {
            PuppetPart[406].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorRGlove01" , 0, 0, 0,2);
			   Object->BodyPart[0] = &PuppetPart[406]; 
         }
      break;
		//Tiamat Changes End
   }
      
	// ARM LEFT
	Object->VisiblePart |= Pow2[1];
	switch (Object->PuppetInfo[1]) {
      case PUPEQ_NACKED: 
         if (Object->Type == 10011) {
            PuppetPart[3].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedArmL", 0, 0, 0); 
			   Object->BodyPart[1] = &PuppetPart[3]; 
         } else {
            PuppetPart[60].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedArmL", 0, 0, 0); 
			   Object->BodyPart[1] = &PuppetPart[60]; 
         }
		break;
		case PUPEQ_SET1:			
         if (Object->Type == 10011) 
         {
            PuppetPart[3].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedArmL", 0, 0, 0); 
			   Object->BodyPart[1] = &PuppetPart[3]; 
         } 
         else 
         {
            Object->VisiblePart -= Pow2[1];
         }
		break;
      case PUPEQ_LEATHER: 
      case PUPEQ_SPIKEDLEATHER:  // Dialsoft spiked armor arm left
			Object->VisiblePart -= Pow2[1]; 
		break;
		//Tiamat Changes Start
      case PUPEQ_PLATE_GOLD: 
      case PUPEQ_PLATE_SILVER: 
      case PUPEQ_PLATE_BLACK: 
      case PUPEQ_PLATE_BLUE:
      case PUPEQ_PLATE_GREEN: 
      case PUPEQ_PLATE_RED: 
      case PUPEQ_PLATE_PURPLE:
      case PUPEQ_PLATE_EMERALD: 
      case PUPEQ_PLATE: 
      case PUPEQ_V2_MAN_ARMOR01GR:
      case PUPEQ_V2_MAN_ARMOR01BL:
			Object->VisiblePart -= Pow2[1]; 
		break;
		//Tiamat changes End
      case PUPEQ_CHAIN: 
			Object->VisiblePart -= Pow2[1]; 
		break;
		case PUPEQ_NECROROBE:   
			Object->VisiblePart -= Pow2[1]; 
		break;
		case PUPEQ_WHITEROBE:
      case PUPEQ_LICHROBE:
      case PUPEQ_ARMORED_ROBE:
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_REDROBE:   
			Object->VisiblePart -= Pow2[1]; 
		break;
		case PUPEQ_MAGEROBE:   
			Object->VisiblePart -= Pow2[1]; 
		break;
		case PUPEQ_STUDDED:   
			Object->VisiblePart -= Pow2[1]; 
		break;
		case PUPEQ_FWHITEROBE_BLUE: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_FWHITEROBE_GREEN: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_FWHITEROBE_DARKBLUE: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_FWHITEROBE_VIOLET: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_FWHITEROBE_PURPLE: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_FWHITEROBE_YELLOW: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_FWHITEROBE_GRAY: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_FWHITEROBE_MAUVE: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_FWHITEROBE_ORANGE: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_FWHITEROBE_BRUN: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
		case PUPEQ_FWHITEROBE_DARK: 
			Object->VisiblePart -= Pow2[1]; 
		break; 
   };

	// FOOT
   Object->VisiblePart |= Pow2[2];
   switch (Object->PuppetInfo[2]) {
      case PUPEQ_NACKED:		 
         if (Object->Type == 10011) {
            PuppetPart[5].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedFoot", 0, 0, 0); 
			   Object->BodyPart[2] = &PuppetPart[5]; 
         } else {
            PuppetPart[56].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedFoot", 0, 0, 0); 
			   Object->BodyPart[2] = &PuppetPart[56]; 
         }
		break;
      case PUPEQ_LEATHER:		 
			Object->VisiblePart -= Pow2[2]; 
		break;
	//Tiamat changes start
	case PUPEQ_PLATE_GOLD:
	case PUPEQ_PLATE_SILVER:
	case PUPEQ_PLATE_BLACK:
	case PUPEQ_PLATE_BLUE:
	case PUPEQ_PLATE_GREEN:
	case PUPEQ_PLATE_RED:
	case PUPEQ_PLATE_PURPLE:
	case PUPEQ_PLATE_EMERALD:
   case PUPEQ_PLATE:
   case PUPEQ_V2_MAN_ARMOR01GR:
   case PUPEQ_V2_MAN_ARMOR01BL:
			Object->VisiblePart -= Pow2[2]; 
		break;
	//Tiamat changes end
      case PUPEQ_CHAIN:			 
			Object->VisiblePart -= Pow2[2]; 
		break;
		case PUPEQ_BLACKLEATHER: 
			Object->VisiblePart -= Pow2[2]; 
		break;
      case PUPEQ_WHITELEATHER: 
			Object->VisiblePart -= Pow2[2]; 
		break;
		case PUPEQ_NECROROBE:   
			Object->VisiblePart -= Pow2[2]; 
		break;
		case PUPEQ_WHITEROBE: 
      case PUPEQ_LICHROBE:
      case PUPEQ_ARMORED_ROBE:
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_REDROBE:   
			Object->VisiblePart -= Pow2[2]; 
		break;
		case PUPEQ_MAGEROBE:   
			Object->VisiblePart -= Pow2[2]; 
		break;
		case PUPEQ_FWHITEROBE_BLUE: 
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_FWHITEROBE_GREEN: 
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_FWHITEROBE_DARKBLUE: 
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_FWHITEROBE_VIOLET: 
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_FWHITEROBE_PURPLE: 
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_FWHITEROBE_YELLOW: 
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_FWHITEROBE_GRAY: 
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_FWHITEROBE_MAUVE: 
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_FWHITEROBE_ORANGE: 
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_FWHITEROBE_BRUN: 
			Object->VisiblePart -= Pow2[2]; 
		break; 
		case PUPEQ_FWHITEROBE_DARK: 
			Object->VisiblePart -= Pow2[2]; 
		break; 

	}

   // LEGS
	Object->VisiblePart |= Pow2[3];
   switch (Object->PuppetInfo[3]) {
      case PUPEQ_NACKED:  
         if (Object->Type == 10011) {
            PuppetPart[6].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedLegs", 0, 0, 0); 
			   Object->BodyPart[3] = &PuppetPart[6]; 
         } else {
            PuppetPart[55].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedLegs", 0, 0, 0); 
			   Object->BodyPart[3] = &PuppetPart[55]; 
         }
		break;
      case PUPEQ_LEATHER: 
         if (Object->Type == 10011) {
            PuppetPart[7].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherPants", 0, 0, 0); 
			   Object->BodyPart[3] = &PuppetPart[7]; 
         } else {
            PuppetPart[62].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherLegs", 0, 0, 0); 
			   Object->BodyPart[3] = &PuppetPart[62]; 
         }
		break;
	//Tiamat Changes start
      case PUPEQ_PLATE:   
         if (Object->Type == 10011) {
   			PuppetPart[8].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs", 0, 0, 0); 
	   		Object->BodyPart[3] = &PuppetPart[8]; 
         } else {
   			PuppetPart[69].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0); 
	   		Object->BodyPart[3] = &PuppetPart[69]; 
         }
		break;
	case PUPEQ_PLATE_GOLD:   
         if (Object->Type == 10011) {
   			PuppetPart[150].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs", 0, 0, 0, 2); 
	   		Object->BodyPart[3] = &PuppetPart[150]; 
         } else {
   			PuppetPart[214].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 2); 
	   		Object->BodyPart[3] = &PuppetPart[214]; 
         }
		break;
	case PUPEQ_PLATE_SILVER:   
         if (Object->Type == 10011) {
   			PuppetPart[158].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs", 0, 0, 0, 3); 
	   		Object->BodyPart[3] = &PuppetPart[158]; 
         } else {
   			PuppetPart[220].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 3); 
	   		Object->BodyPart[3] = &PuppetPart[220]; 
         }
		break;
	case PUPEQ_PLATE_BLACK:   
         if (Object->Type == 10011) {
   			PuppetPart[166].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs", 0, 0, 0, 4); 
	   		Object->BodyPart[3] = &PuppetPart[166]; 
         } else {
   			PuppetPart[226].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 4); 
	   		Object->BodyPart[3] = &PuppetPart[226]; 
         }
		break;
	case PUPEQ_PLATE_BLUE:   
         if (Object->Type == 10011) {
   			PuppetPart[174].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs", 0, 0, 0, 5); 
	   		Object->BodyPart[3] = &PuppetPart[174]; 
         } else {
   			PuppetPart[232].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 5); 
	   		Object->BodyPart[3] = &PuppetPart[232]; 
         }
		break;
	case PUPEQ_PLATE_GREEN:   
         if (Object->Type == 10011) {
   			PuppetPart[182].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs", 0, 0, 0, 6); 
	   		Object->BodyPart[3] = &PuppetPart[182]; 
         } else {
   			PuppetPart[238].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 6); 
	   		Object->BodyPart[3] = &PuppetPart[238]; 
         }
		break;
	case PUPEQ_PLATE_RED:   
         if (Object->Type == 10011) {
   			PuppetPart[190].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs", 0, 0, 0, 7); 
	   		Object->BodyPart[3] = &PuppetPart[190]; 
         } else {
   			PuppetPart[244].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 7); 
	   		Object->BodyPart[3] = &PuppetPart[244]; 
         }
		break;
	case PUPEQ_PLATE_PURPLE:   
         if (Object->Type == 10011) {
   			PuppetPart[198].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs", 0, 0, 0, 8); 
	   		Object->BodyPart[3] = &PuppetPart[198]; 
         } else {
   			PuppetPart[214].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 8); 
	   		Object->BodyPart[3] = &PuppetPart[250]; 
         }
		break;
	case PUPEQ_PLATE_EMERALD:   
         if (Object->Type == 10011) {
   			PuppetPart[206].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateLegs", 0, 0, 0, 9); 
	   		Object->BodyPart[3] = &PuppetPart[206]; 
         } else {
   			PuppetPart[256].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateLegs", 0, 0, 0, 9); 
	   		Object->BodyPart[3] = &PuppetPart[256]; 
         }
		break;
	//Tiamat changes end
      case PUPEQ_CHAIN:   
         if (Object->Type == 10011) {
            PuppetPart[9].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupChainMailLegs", 0, 0, 0); 
			   Object->BodyPart[3] = &PuppetPart[9]; 
         } else {
            PuppetPart[66].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoChainMailLegs", 0, 0, 0); 
			   Object->BodyPart[3] = &PuppetPart[66]; 
         }
      break;
		case PUPEQ_SET1:    
         if (Object->Type == 10011) {
            PuppetPart[10].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLegsClothSet1", 0, 0, 0); 
			   Object->BodyPart[3] = &PuppetPart[10]; 
         } else {
            Object->VisiblePart -= Pow2[3];
         }
		break;
		case PUPEQ_STUDDED: 
         if (Object->Type == 10011) {
            PuppetPart[11].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupStuddedLegs", 0, 0, 0); 
			   Object->BodyPart[3] = &PuppetPart[11]; 
         } else {
            PuppetPart[64].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoStuddedLegs", 0, 0, 0); 
			   Object->BodyPart[3] = &PuppetPart[64]; 
         }
		break;
		case PUPEQ_NECROROBE:   
			Object->VisiblePart -= Pow2[3]; 
		break;
		case PUPEQ_WHITEROBE: 
      case PUPEQ_LICHROBE:
      case PUPEQ_ARMORED_ROBE:
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_REDROBE:   
			Object->VisiblePart -= Pow2[3]; 
		break;
		case PUPEQ_MAGEROBE:   
			Object->VisiblePart -= Pow2[3]; 
		break;
		case PUPEQ_FWHITEROBE_BLUE: 
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_FWHITEROBE_GREEN: 
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_FWHITEROBE_DARKBLUE: 
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_FWHITEROBE_VIOLET: 
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_FWHITEROBE_PURPLE: 
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_FWHITEROBE_YELLOW: 
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_FWHITEROBE_GRAY: 
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_FWHITEROBE_MAUVE:
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_FWHITEROBE_ORANGE: 
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_FWHITEROBE_BRUN: 
			Object->VisiblePart -= Pow2[3]; 
		break; 
		case PUPEQ_FWHITEROBE_DARK: 
			Object->VisiblePart -= Pow2[3]; 
		break; 
      case PUPEQ_V2_MAN_ARMOR01GR:
         if (Object->Type == 10011) 
         {
            PuppetPart[401].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLegs01" , 0, 0, 0);
			   Object->BodyPart[3] = &PuppetPart[401]; 
         } 
         else 
         {
            PuppetPart[401].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLegs01" , 0, 0, 0);
			   Object->BodyPart[3] = &PuppetPart[401]; 
         }
      break;
      case PUPEQ_V2_MAN_ARMOR01BL:
         if (Object->Type == 10011) 
         {
            PuppetPart[405].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLegs01" , 0, 0, 0,2);
			   Object->BodyPart[3] = &PuppetPart[405]; 
         } 
         else 
         {
            PuppetPart[405].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLegs01" , 0, 0, 0,2);
			   Object->BodyPart[3] = &PuppetPart[405]; 
         }
      break;
   }
      
	// BODY
	Object->VisiblePart |= Pow2[4];
   switch (Object->PuppetInfo[4]) {
		case PUPEQ_NACKED:		
         if (Object->Type == 10011) {
            PuppetPart[12].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedBody", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[12]; 
         } else {
            PuppetPart[54].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedBody", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[54]; 
         }
		break;
      case PUPEQ_LEATHER:		
         if (Object->Type == 10011) {
            PuppetPart[13].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherBody", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[13]; 
         } else {
            PuppetPart[73].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherArms", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[73]; 
         }
		break;
		//Tiamat Changes
      case PUPEQ_PLATE:			
         if (Object->Type == 10011) {
            PuppetPart[14].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[14]; 
         } else {
            PuppetPart[68].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[68]; 
         }
		break;
	case PUPEQ_PLATE_GOLD:			
         if (Object->Type == 10011) {
            PuppetPart[151].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody", 0, 0, 0, 2); 
			   Object->BodyPart[4] = &PuppetPart[151]; 
         } else {
            PuppetPart[213].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 2); 
			   Object->BodyPart[4] = &PuppetPart[213]; 
         }
		break;
	case PUPEQ_PLATE_SILVER:			
         if (Object->Type == 10011) {
            PuppetPart[159].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody", 0, 0, 0, 3); 
			   Object->BodyPart[4] = &PuppetPart[159]; 
         } else {
            PuppetPart[219].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 3); 
			   Object->BodyPart[4] = &PuppetPart[219]; 
         }
		break;
	case PUPEQ_PLATE_BLACK:			
         if (Object->Type == 10011) {
            PuppetPart[167].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody", 0, 0, 0, 4); 
			   Object->BodyPart[4] = &PuppetPart[167]; 
         } else {
            PuppetPart[225].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 4); 
			   Object->BodyPart[4] = &PuppetPart[225]; 
         }
		break;
	case PUPEQ_PLATE_BLUE:			
         if (Object->Type == 10011) {
            PuppetPart[175].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody", 0, 0, 0, 5); 
			   Object->BodyPart[4] = &PuppetPart[175]; 
         } else {
            PuppetPart[231].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 5); 
			   Object->BodyPart[4] = &PuppetPart[231]; 
         }
		break;
	case PUPEQ_PLATE_GREEN:			
         if (Object->Type == 10011) {
            PuppetPart[183].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody", 0, 0, 0, 6); 
			   Object->BodyPart[4] = &PuppetPart[183]; 
         } else {
            PuppetPart[237].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 6); 
			   Object->BodyPart[4] = &PuppetPart[237]; 
         }
		break;
	case PUPEQ_PLATE_RED:			
         if (Object->Type == 10011) {
            PuppetPart[191].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody", 0, 0, 0, 7); 
			   Object->BodyPart[4] = &PuppetPart[191]; 
         } else {
            PuppetPart[243].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 7); 
			   Object->BodyPart[4] = &PuppetPart[243]; 
         }
		break;
	case PUPEQ_PLATE_PURPLE:			
         if (Object->Type == 10011) {
            PuppetPart[199].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody", 0, 0, 0, 8); 
			   Object->BodyPart[4] = &PuppetPart[199]; 
         } else {
            PuppetPart[249].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 8); 
			   Object->BodyPart[4] = &PuppetPart[249]; 
         }
		break;
	case PUPEQ_PLATE_EMERALD:			
         if (Object->Type == 10011) {
            PuppetPart[207].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateBody", 0, 0, 0, 9); 
			   Object->BodyPart[4] = &PuppetPart[207]; 
         } else {
            PuppetPart[255].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBody", 0, 0, 0, 9); 
			   Object->BodyPart[4] = &PuppetPart[255]; 
         }
		break;
		//Tiamat Changes End
      case PUPEQ_CHAIN:			
         if (Object->Type == 10011) {
            PuppetPart[15].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupChainMailBody", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[15]; 
         } else {
            PuppetPart[67].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoChainBody", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[67]; 
         }
		break;
		case PUPEQ_NECROROBE:
         if (Object->Type == 10011) {
            PuppetPart[16].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNecromanRobe", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[16]; 
         } else {
            PuppetPart[92].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNecromanRobe", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[92]; 
         }
		break;
		case PUPEQ_WHITEROBE:   
         if (Object->Type == 10011) {
            PuppetPart[19].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0); 
	   		Object->BodyPart[4] = &PuppetPart[19]; 
         } else {
            PuppetPart[52].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0); 
	   		Object->BodyPart[4] = &PuppetPart[52]; 
         }
		break;
      case PUPEQ_LICHROBE://NAD
         if (Object->Type == 10011) 
         {
            PuppetPart[422].LoadSprite3D(13, 9, 0, 0, 0, 9, "ManLichRobeOri", 0, 0, 0); //BLBLBL 25 mars 2009 : changé le armored robe en lich robe
	   		Object->BodyPart[4] = &PuppetPart[422]; 
         } 
         else 
         {
            PuppetPart[423].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLichRobeOri", 0, 0, 0);  //BLBLBL 25 mars 2009 : changé le armored robe en lich robe
	   		Object->BodyPart[4] = &PuppetPart[423]; 
         }
      break;
      case PUPEQ_ARMORED_ROBE:
         if (Object->Type == 10011) 
         {
            PuppetPart[288].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupArmoredRobe", 0, 0, 0); 
	   		Object->BodyPart[4] = &PuppetPart[288]; 
         } 
         else 
         {
            PuppetPart[287].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoArmoredRobe", 0, 0, 0);
	   		Object->BodyPart[4] = &PuppetPart[287]; 
         }
      break;
		case PUPEQ_REDROBE: 
         if (Object->Type == 10011) {
   			PuppetPart[20].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedRobe", 0, 0, 0); 
	   		Object->BodyPart[4] = &PuppetPart[20]; 
         } else {
   			PuppetPart[95].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoRedRobe", 0, 0, 0); 
	   		Object->BodyPart[4] = &PuppetPart[95]; 
         }
		break;
		case PUPEQ_MAGEROBE:    
         if (Object->Type == 10011) {
   			PuppetPart[21].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupMageRobe", 0, 0, 0); 
	   		Object->BodyPart[4] = &PuppetPart[21]; 
         } else {
   			PuppetPart[97].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoMageRobe", 0, 0, 0); 
	   		Object->BodyPart[4] = &PuppetPart[97]; 
         }
		break;
		case PUPEQ_STUDDED:
         if (Object->Type == 10011) {
            PuppetPart[17].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupStuddedBodyArmor", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[17]; 
         } else {
            PuppetPart[65].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoStuddedBody", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[65]; 
         }
		break;
		case PUPEQ_SET1:			
         if (Object->Type == 10011) {
            PuppetPart[18].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupBodyClothSet1", 0, 0, 0); 
   			Object->BodyPart[4] = &PuppetPart[18]; 
         } else {
            PuppetPart[71].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoClothBody", 0, 0, 0); 
   			Object->BodyPart[4] = &PuppetPart[71]; 
         }
		break;
		//Dialsoft added spiked armor body
		case PUPEQ_SPIKEDLEATHER:		 
         if (Object->Type == 10011) 
         {
            PuppetPart[117].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSpikeLeatherBody", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[117]; 
         } 
         else 
         {
            PuppetPart[73].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherArms", 0, 0, 0); 
			   Object->BodyPart[4] = &PuppetPart[73]; 
			   //Note : no woman skin for the Spiked leather yet
			   //using classical leather instead
         } 
		break;
		case PUPEQ_FWHITEROBE_BLUE:   
         if (Object->Type == 10011) {
            PuppetPart[126].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 2); 
	   		Object->BodyPart[4] = &PuppetPart[126];  // 4 = PUP_BODY
         } else {
            PuppetPart[137].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 2); 
	   		Object->BodyPart[4] = &PuppetPart[137]; 
         }
		break;
		case PUPEQ_FWHITEROBE_GREEN:   
         if (Object->Type == 10011) {
            PuppetPart[127].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 3); 
	   		Object->BodyPart[4] = &PuppetPart[127]; 
         } else {
            PuppetPart[138].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 3); 
	   		Object->BodyPart[4] = &PuppetPart[138]; 
         }
		break;
		case PUPEQ_FWHITEROBE_DARKBLUE:   
         if (Object->Type == 10011) {
            PuppetPart[128].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 4); 
	   		Object->BodyPart[4] = &PuppetPart[128]; 
         } else {
            PuppetPart[139].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 4); 
	   		Object->BodyPart[4] = &PuppetPart[139]; 
         }
		break;
		case PUPEQ_FWHITEROBE_VIOLET:   
         if (Object->Type == 10011) {
            PuppetPart[129].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 5); 
	   		Object->BodyPart[4] = &PuppetPart[129]; 
         } else {
            PuppetPart[140].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 5); 
	   		Object->BodyPart[4] = &PuppetPart[140]; 
         }
		break;
		case PUPEQ_FWHITEROBE_PURPLE:   
         if (Object->Type == 10011) {
            PuppetPart[130].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 6); 
	   		Object->BodyPart[4] = &PuppetPart[130]; 
         } else {
            PuppetPart[141].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 6); 
	   		Object->BodyPart[4] = &PuppetPart[141]; 
         }
		break;
		case PUPEQ_FWHITEROBE_YELLOW:   
         if (Object->Type == 10011) {
            PuppetPart[131].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 7); 
	   		Object->BodyPart[4] = &PuppetPart[131]; 
         } else {
            PuppetPart[142].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 7); 
	   		Object->BodyPart[4] = &PuppetPart[142]; 
         }
		break;
		case PUPEQ_FWHITEROBE_GRAY:   
         if (Object->Type == 10011) {
            PuppetPart[132].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 8); 
	   		Object->BodyPart[4] = &PuppetPart[132]; 
         } else {
            PuppetPart[143].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 8); 
	   		Object->BodyPart[4] = &PuppetPart[143]; 
         }
		break;
		case PUPEQ_FWHITEROBE_MAUVE:   
         if (Object->Type == 10011) {
            PuppetPart[133].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 9); 
	   		Object->BodyPart[4] = &PuppetPart[133]; 
         } else {
            PuppetPart[144].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 9); 
	   		Object->BodyPart[4] = &PuppetPart[144]; 
         }
		break;
		case PUPEQ_FWHITEROBE_ORANGE:   
         if (Object->Type == 10011) {
            PuppetPart[134].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 10); 
	   		Object->BodyPart[4] = &PuppetPart[134]; 
         } else {
            PuppetPart[145].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 10); 
	   		Object->BodyPart[4] = &PuppetPart[145]; 
         }
		break;
		case PUPEQ_FWHITEROBE_BRUN:   
         if (Object->Type == 10011) {
            PuppetPart[135].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 11); 
	   		Object->BodyPart[4] = &PuppetPart[135]; 
         } else {
            PuppetPart[146].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 11); 
	   		Object->BodyPart[4] = &PuppetPart[146]; 
         }
		break;
		case PUPEQ_FWHITEROBE_DARK:   
         if (Object->Type == 10011) {
            PuppetPart[136].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupWhiteRobe", 0, 0, 0, 12); 
	   		Object->BodyPart[4] = &PuppetPart[136]; 
         } else {
            PuppetPart[147].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoWhiteRobe", 0, 0, 0, 12); 
	   		Object->BodyPart[4] = &PuppetPart[147]; 
         }
		break;
      case PUPEQ_V2_MAN_ARMOR01GR:
         if (Object->Type == 10011) 
         {
            PuppetPart[400].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBody01" , 0, 0, 0);
			   Object->BodyPart[4] = &PuppetPart[400]; 
         } 
         else 
         {
            PuppetPart[400].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBody01" , 0, 0, 0);
			   Object->BodyPart[4] = &PuppetPart[400]; 
         }
      break;
      case PUPEQ_V2_MAN_ARMOR01BL:
         if (Object->Type == 10011) 
         {
            PuppetPart[404].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBody01" , 0, 0, 0,2);
			   Object->BodyPart[4] = &PuppetPart[404]; 
         } 
         else 
         {
            PuppetPart[404].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBody01" , 0, 0, 0,2);
			   Object->BodyPart[4] = &PuppetPart[404]; 
         }
      break;
		//Dialsoft end
   }
   
	// HEAD
	Object->VisiblePart |= Pow2[5];
	switch (Object->PuppetInfo[5]) {
		case PUPEQ_NACKED: 
         if (Object->Type == 10011) {
            PuppetPart[22].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedHead", 0, 0, 0); 
			   Object->BodyPart[5] = &PuppetPart[22]; 
         } else {
            PuppetPart[53].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedHead", 0, 0, 0); 
			   Object->BodyPart[5] = &PuppetPart[53]; 
         }
		break;
		case PUPEQ_LEATHER: 
         Object->VisiblePart -= Pow2[5];
		break;
		//Tiamat Changes
	case PUPEQ_PLATE_GOLD: 
	case PUPEQ_PLATE_SILVER:
	case PUPEQ_PLATE_BLACK:
	case PUPEQ_PLATE_BLUE: 
	case PUPEQ_PLATE_GREEN: 
	case PUPEQ_PLATE_RED: 
	case PUPEQ_PLATE_PURPLE: 
	case PUPEQ_PLATE_EMERALD: 
	case PUPEQ_PLATE: 
	      Object->VisiblePart -= Pow2[5];
		break;
		//Tiamat Changes end
      case PUPEQ_CHAIN: 
	      Object->VisiblePart -= Pow2[5];
		break;
      case PUPEQ_HORNED: 
	      Object->VisiblePart -= Pow2[5];
		break;
	}


   // HAND R   
	Object->VisiblePart |= Pow2[6];
   switch (Object->PuppetInfo[6]) {
      case 0: 
         if (Object->Type == 10011) {
            PuppetPart[26].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedHandR", 0, 0, 0); 
			   Object->BodyPart[6] = &PuppetPart[26]; 
         } else {
            PuppetPart[57].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedHandR", 0, 0, 0); 
			   Object->BodyPart[6] = &PuppetPart[57]; 
         }
		break;
      case 1: 
         if (Object->Type == 10011) {
            PuppetPart[27].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherGloveR", 0, 0, 0); 
			   Object->BodyPart[6] = &PuppetPart[27]; 
         } else {
            PuppetPart[76].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherGloveR", 0, 0, 0); 
			   Object->BodyPart[6] = &PuppetPart[76]; 
         }
		break;
		//Tiamat Changes
      case 2: 
         if (Object->Type == 10011) {
            PuppetPart[28].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR", 0, 0, 0); 
	   		Object->BodyPart[6] = &PuppetPart[28]; 
         } else {
            PuppetPart[84].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0); 
	   		Object->BodyPart[6] = &PuppetPart[84]; 
         }
		break;
	case PUPEQ_PLATE_GOLD: 
         if (Object->Type == 10011) {
            PuppetPart[153].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR", 0, 0, 0, 2); 
	   		Object->BodyPart[6] = &PuppetPart[153]; 
         } else {
            PuppetPart[216].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 2); 
	   		Object->BodyPart[6] = &PuppetPart[216]; 
         }
		break;
	case PUPEQ_PLATE_SILVER: 
         if (Object->Type == 10011) {
            PuppetPart[161].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR", 0, 0, 0, 3); 
	   		Object->BodyPart[6] = &PuppetPart[161]; 
         } else {
            PuppetPart[222].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 3); 
	   		Object->BodyPart[6] = &PuppetPart[222]; 
         }
		break;
	case PUPEQ_PLATE_BLACK: 
         if (Object->Type == 10011) {
            PuppetPart[169].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR", 0, 0, 0, 4); 
	   		Object->BodyPart[6] = &PuppetPart[169]; 
         } else {
            PuppetPart[228].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 4); 
	   		Object->BodyPart[6] = &PuppetPart[228]; 
         }
		break;
	case PUPEQ_PLATE_BLUE: 
         if (Object->Type == 10011) {
            PuppetPart[177].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR", 0, 0, 0, 5); 
	   		Object->BodyPart[6] = &PuppetPart[177]; 
         } else {
            PuppetPart[234].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 5); 
	   		Object->BodyPart[6] = &PuppetPart[234]; 
         }
		break;
	case PUPEQ_PLATE_GREEN: 
         if (Object->Type == 10011) {
            PuppetPart[185].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR", 0, 0, 0, 6); 
	   		Object->BodyPart[6] = &PuppetPart[185]; 
         } else {
            PuppetPart[240].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 6); 
	   		Object->BodyPart[6] = &PuppetPart[240]; 
         }
		break;
	case PUPEQ_PLATE_RED: 
         if (Object->Type == 10011) {
            PuppetPart[193].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR", 0, 0, 0, 7); 
	   		Object->BodyPart[6] = &PuppetPart[193]; 
         } else {
            PuppetPart[246].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 7); 
	   		Object->BodyPart[6] = &PuppetPart[246]; 
         }
		break;
	case PUPEQ_PLATE_PURPLE: 
         if (Object->Type == 10011) {
            PuppetPart[201].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR", 0, 0, 0, 8); 
	   		Object->BodyPart[6] = &PuppetPart[201]; 
         } else {
            PuppetPart[252].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 8); 
	   		Object->BodyPart[6] = &PuppetPart[252]; 
         }
		break;
	case PUPEQ_PLATE_EMERALD: 
         if (Object->Type == 10011) {
            PuppetPart[209].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateGloveR", 0, 0, 0, 9); 
	   		Object->BodyPart[6] = &PuppetPart[209]; 
         } else {
            PuppetPart[258].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateGloveR", 0, 0, 0, 9); 
	   		Object->BodyPart[6] = &PuppetPart[258]; 
         }
		break;
		//Tiamat changes end
      case 3: 
         if (Object->Type == 10011) {
            PuppetPart[27].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherGloveR", 0, 0, 0); 
			   Object->BodyPart[6] = &PuppetPart[27]; 
         } else {
            PuppetPart[76].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherGloveR", 0, 0, 0); 
			   Object->BodyPart[6] = &PuppetPart[76]; 
         }
		break;
      case PUPEQ_V2_MAN_ARMOR01GR:
         if (Object->Type == 10011) 
         {
            PuppetPart[408].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLGlove01" , 0, 0, 0);
			   Object->BodyPart[6] = &PuppetPart[408]; 
         } 
         else 
         {
            PuppetPart[408].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLGlove01" , 0, 0, 0);
			   Object->BodyPart[6] = &PuppetPart[408]; 
         }
      break;
      case PUPEQ_V2_MAN_ARMOR01BL:
         if (Object->Type == 10011) 
         {
            PuppetPart[409].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLGlove01" , 0, 0, 0,2);
			   Object->BodyPart[6] = &PuppetPart[409]; 
         } 
         else 
         {
            PuppetPart[409].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorLGlove01" , 0, 0, 0,2);
			   Object->BodyPart[6] = &PuppetPart[409]; 
         }
      break;
   }

   // ARM R
	Object->VisiblePart |= Pow2[7]; 	
	switch (Object->PuppetInfo[7]) {
      case PUPEQ_NACKED:		
         if (Object->Type == 10011) {
            PuppetPart[29].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedArmR", 0, 0, 0); 
			   Object->BodyPart[7] = &PuppetPart[29]; 
         } else {
            PuppetPart[59].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedArmR", 0, 0, 0); 
			   Object->BodyPart[7] = &PuppetPart[59]; 
         }
		break;
		case PUPEQ_SET1:			
         if (Object->Type == 10011) 
         {
            PuppetPart[29].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedArmR", 0, 0, 0); 
			   Object->BodyPart[7] = &PuppetPart[29]; 
         } 
         else 
         {
            Object->VisiblePart -= Pow2[7];
         }
		break;
      case PUPEQ_LEATHER:
	   case PUPEQ_SPIKEDLEATHER:  // Dialsoft spiked armor arm right
			Object->VisiblePart -= Pow2[7]; 
		break;
	//Tiamat changes
	case PUPEQ_PLATE_GOLD:
	case PUPEQ_PLATE_SILVER:
	case PUPEQ_PLATE_BLACK:
	case PUPEQ_PLATE_BLUE:
	case PUPEQ_PLATE_GREEN:
	case PUPEQ_PLATE_RED:
	case PUPEQ_PLATE_PURPLE:
	case PUPEQ_PLATE_EMERALD:
    case PUPEQ_PLATE:			
			Object->VisiblePart -= Pow2[7]; 
/*         if (Object->Type == 10011) {
            PuppetPart[30].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateArmR", 0, 0, 0); 
   			Object->BodyPart[7] = &PuppetPart[30]; 
			   Object->VisiblePart -= Pow2[6]; 
         } else {
            Object->VisiblePart -= Pow2[7];
         } */
		break;
	//Tiamat changes end
      case PUPEQ_CHAIN:			
			Object->VisiblePart -= Pow2[7]; 
		break;
		case PUPEQ_NECROROBE:   
			Object->VisiblePart -= Pow2[7]; 
		break;
		case PUPEQ_WHITEROBE:  
      case PUPEQ_LICHROBE:
      case PUPEQ_ARMORED_ROBE:
			Object->VisiblePart -= Pow2[7]; 
		break;
		case PUPEQ_REDROBE: 
			Object->VisiblePart -= Pow2[7]; 
		break;
		case PUPEQ_MAGEROBE:		
			Object->VisiblePart -= Pow2[7]; 
		break;
		case PUPEQ_STUDDED:		
			Object->VisiblePart -= Pow2[7]; 
		break;
		case PUPEQ_FWHITEROBE_BLUE: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
		case PUPEQ_FWHITEROBE_GREEN: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
		case PUPEQ_FWHITEROBE_DARKBLUE: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
		case PUPEQ_FWHITEROBE_VIOLET: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
		case PUPEQ_FWHITEROBE_PURPLE: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
		case PUPEQ_FWHITEROBE_YELLOW: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
		case PUPEQ_FWHITEROBE_GRAY: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
		case PUPEQ_FWHITEROBE_MAUVE: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
		case PUPEQ_FWHITEROBE_ORANGE: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
		case PUPEQ_FWHITEROBE_BRUN: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
		case PUPEQ_FWHITEROBE_DARK: 
			Object->VisiblePart -= Pow2[7]; 
		break; 
      case PUPEQ_V2_MAN_ARMOR01GR:
         Object->VisiblePart -= Pow2[7]; 
      break;
      case PUPEQ_V2_MAN_ARMOR01BL:
         Object->VisiblePart -= Pow2[7]; 
      break;
   }

   // WEAPON   
	Object->VisiblePart |= Pow2[8];
	switch (Object->PuppetInfo[8]) {
		case PUPEQ_NACKED:		 
			Object->VisiblePart -= Pow2[8]; 
		break;
		case PUPEQ_AXE:			 
			PuppetPart[31].LoadSprite3D(13, 9, 0, 0, 0, "PupBattleAxe", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[31]; 
		break;
		case PUPEQ_DARK:			 
			PuppetPart[32].LoadSprite3D(13, 9, 0, 0, 0, "PupDarkSword", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[32]; 
		break;
		case PUPEQ_FLAIL:		 	 
			PuppetPart[33].LoadSprite3D(13, 9, 0, 0, 0, "PupFlail", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[33]; 
		break;
		case PUPEQ_NORMALSWORD:  
			PuppetPart[34].LoadSprite3D(13, 9, 0, 0, 0, "PupNormalSword", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[34]; 
		break;
      case PUPEQ_OGRECLUB:
         PuppetPart[96].LoadSprite3D(13, 0, 0, 0, 0, 9, "PupOgreClub", 0, 0, 0 );
			Object->BodyPart[8] = &PuppetPart[96]; 
      break;
		case PUPEQ_BATTLESWORD:  
			PuppetPart[35].LoadSprite3D(13, 9, 0, 0, 0, "PupBattleSword", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[35]; 
		break;
		case PUPEQ_REALDARK:		 
			PuppetPart[36].LoadSprite3D(13, 9, 0, 0, 0, "PupRealDarkSword", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[36]; 
		break;
		case PUPEQ_MORNINGSTAR:  
			PuppetPart[37].LoadSprite3D(13, 9, 0, 0, 0, "PupMorningStar", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[37]; 
		break;
		case PUPEQ_GOLDENSTAR:   
			PuppetPart[38].LoadSprite3D(13, 9, 0, 0, 0, "PupGoldenMorningStar", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[38]; 
		break;
		case PUPEQ_BATTLEDAGGER: 
			PuppetPart[39].LoadSprite3D(13, 9, 0, 0, 0, "PupBattleDagger", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[39]; 
		break;
		case PUPEQ_WARHAMMER:    
			PuppetPart[40].LoadSprite3D(13, 9, 0, 0, 0, "PupWarhammer", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[40]; 
		break;
		case PUPEQ_MACE:         
			PuppetPart[41].LoadSprite3D(13, 9, 0, 0, 0, "PupMace", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[41]; 
		break;
		case PUPEQ_STAFF1:         
			PuppetPart[79].LoadSprite3D(13, 9, 0, 0, 0, "PupWoodenStaff", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[79]; 
		break;
		case PUPEQ_STAFF2:         
			PuppetPart[80].LoadSprite3D(13, 9, 0, 0, 0, "PupLichStaff", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[80]; 
		break;
		case PUPEQ_STAFF3:         
			PuppetPart[81].LoadSprite3D(13, 9, 0, 0, 0, "PupGemStaff", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[81]; 
		break;
		case PUPEQ_STAFF4:
			PuppetPart[82].LoadSprite3D(13, 9, 0, 0, 0, "PupSimpleStaff", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[82]; 
		break;
      case PUPEQ_SMALL_BOW:
         PuppetPart[93].LoadSprite3D(13, 0, 0, 0, 0, 9, "PupShortBow", 0, 0, 0); 
         Object->BodyPart[8] = &PuppetPart[93];
         break;
      case PUPEQ_LARGE_BOW:
         PuppetPart[94].LoadSprite3D(13, 0, 0, 0, 0, 9, "PupLongBow", 0, 0, 0); 
         Object->BodyPart[8] = &PuppetPart[94]; 
         break;
      case PUPEQ_NEW_BOW01:
         PuppetPart[275].LoadSprite3D(13, 0, 0, 0, 0, 9, "NewBow01", 0, 0, 0); 
         Object->BodyPart[8] = &PuppetPart[275]; 
         break;
      case PUPEQ_DWARF_HAMMER:
         PuppetPart[276].LoadSprite3D(13, 9, 0, 0, 0, "DwarfHammer", 0, 0, 0); 
         Object->BodyPart[8] = &PuppetPart[276]; 
         break;
      case PUPEQ_AXE_DESTRUCTION:
         PuppetPart[277].LoadSprite3D(13, 9, 0, 0, 0, "AxeDestruction", 0, 0, 0); 
         Object->BodyPart[8] = &PuppetPart[277]; 
         break;
      case PUPEQ_SWORD_ANGEL:
         PuppetPart[278].LoadSprite3D(13, 9, 0, 0, 0, "SwordAngel", 0, 0, 0); 
         Object->BodyPart[8] = &PuppetPart[278]; 
      break;
      case PUPEQ_CLAY_2BLADES:
         PuppetPart[279].LoadSprite3D(13, 9, 0, 0, 0, "Clay2blade", 0, 0, 0); 
         Object->BodyPart[8] = &PuppetPart[279]; 
      break;
      case PUPEQ_SWORD_HERO:
         PuppetPart[280].LoadSprite3D(13, 9, 0, 0, 0, "SwordAngel", 0, 0, 0); 
         Object->BodyPart[8] = &PuppetPart[280]; 
      break;
		case PUPEQ_SKAVENCLUB:
			PuppetPart[103].LoadSprite3D(13, 9, 0, 0, 0, "PupSkavenClub", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[103]; 
         break;
		case PUPEQ_SKAVENKNIFE:
			PuppetPart[104].LoadSprite3D(13, 9, 0, 0, 0, "PupSkavenKnife", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[104]; 
		   break;
		case PUPEQ_SKELAXE:
			PuppetPart[108].LoadSprite3D(13, 9, 0, 0, 0, "PupSkeletonAxe", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[108]; 
		   break;
		case PUPEQ_HALBERD:
			PuppetPart[114].LoadSprite3D(13, 9, 0, 0, 0, "PupHalberd", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[114]; 
		   break;
		case PUPEQ_SKELSWORD:
			PuppetPart[111].LoadSprite3D(13, 9, 0, 0, 0, "PupSkeletonSword", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[111]; 
		   break;
//Dialsoft new weapons
		case PUPEQ_FIREFLAIL:		 	 
			PuppetPart[116].LoadSprite3D(13, 9, 0, 0, 0, "PupFireFlail", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[116]; 
		break;
		case PUPEQ_TIGERAXE:		 	 
			PuppetPart[260].LoadSprite3D(13, 9, 0, 0, 0, "PupTigerAxe", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[260]; 
		break;
		case PUPEQ_ANCIENTAXE:		 	 
			PuppetPart[261].LoadSprite3D(13, 9, 0, 0, 0, "PupAncientAxe", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[261]; 
		break;
		case PUPEQ_FLOWERS:		 	 
			PuppetPart[263].LoadSprite3D(13, 9, 0, 0, 0, "PupFlower", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[263]; 
		break;
		case PUPEQ_NECROSTAFF:		 	 
			PuppetPart[264].LoadSprite3D(13, 9, 0, 0, 0, "PupNecroStaff", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[264]; 
		break;
      case PUPEQ_HAMM:
         PuppetPart[285].LoadSprite3D(13, 9, 0, 0, 0, "Ham", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[285]; 
      break;
      case PUPEQ_V2_DAGGER_VIP:
          PuppetPart[298].LoadSprite3D(13, 9, 0, 0, 0, "V2_Viperine", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[298]; 
      break;
      case PUPEQ_V2_DAGGER_01:
         PuppetPart[299].LoadSprite3D(13, 9, 0, 0, 0, "V2_Dague01" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[299]; 
      break;
      case PUPEQ_V2_DAGGER_02:
         PuppetPart[300].LoadSprite3D(13, 9, 0, 0, 0, "V2_Dague02" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[300]; 
      break;
      case PUPEQ_V2_DAGGER_03:
        PuppetPart[301].LoadSprite3D(13, 9, 0, 0, 0, "V2_Dague03" , 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[301]; 
      break;
      case PUPEQ_V2_DAGGER_04:
         PuppetPart[302].LoadSprite3D(13, 9, 0, 0, 0, "V2_Dague04" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[302]; 
      break;
      case PUPEQ_V2_DAGGER_05:
         PuppetPart[303].LoadSprite3D(13, 9, 0, 0, 0, "V2_Dague05" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[303]; 
      break;

      //NM_TAG_ADD_SWORD
      case PUPEQ_V2_BUSSWORD01:
         PuppetPart[304].LoadSprite3D(13, 9, 0, 0, 0, "V2_BusterSlayer01" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[304]; 
      break;
      case PUPEQ_V2_BUSSWORD02:
         PuppetPart[305].LoadSprite3D(13, 9, 0, 0, 0, "V2_BusterSlayer02" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[305]; 
      break;
      case PUPEQ_V2_BUSSWORD03:
         PuppetPart[306].LoadSprite3D(13, 9, 0, 0, 0, "V2_BusterSlayer03" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[306]; 
      break;
      case PUPEQ_V2_CLAYSWORD01:
         PuppetPart[307].LoadSprite3D(13, 9, 0, 0, 0, "V2_Claymore01" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[307]; 
      break;
      case PUPEQ_V2_CLAYSWORD02:
         PuppetPart[308].LoadSprite3D(13, 9, 0, 0, 0, "V2_Claymore02" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[308]; 
      break;
      case PUPEQ_V2_GLAIVESWORD01:
         PuppetPart[309].LoadSprite3D(13, 9, 0, 0, 0, "V2_GlaiveSlayer01" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[309]; 
      break;
      case PUPEQ_V2_GLAIVESWORD02:
         PuppetPart[310].LoadSprite3D(13, 9, 0, 0, 0, "V2_GlaiveSlayer02" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[310]; 
      break;
      case PUPEQ_V2_GLAIVESWORD02_ICE:
         PuppetPart[394].LoadSprite3D(13, 9, 0, 0, 0, "V2_Claymore02" , 0, 0, 0,2);
			Object->BodyPart[8] = &PuppetPart[394]; 
      break;
      case PUPEQ_V2_SWORD01:
         PuppetPart[311].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword01" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[311]; 
      break;
      case PUPEQ_V2_SWORD02:
         PuppetPart[312].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword02" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[312]; 
      break;
      case PUPEQ_V2_SWORD03:
         PuppetPart[313].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword03" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[313]; 
      break;
      case PUPEQ_V2_SWORD04:
         PuppetPart[314].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword04" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[314]; 
      break;
      case PUPEQ_V2_SWORD05:
         PuppetPart[315].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword05" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[315]; 
      break;
      case PUPEQ_V2_SWORD06:
         PuppetPart[316].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword06" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[316]; 
      break;
      case PUPEQ_V2_SWORD07:
         PuppetPart[317].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword07" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[317]; 
      break;
      case PUPEQ_V2_SWORD08:
         PuppetPart[318].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sword08" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[318]; 
      break;
      case PUPEQ_V2_HACHE01:
         PuppetPart[334].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache01" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[334]; 
      break;
      case PUPEQ_V2_HACHE02:
         PuppetPart[335].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache02" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[335]; 
      break;
      case PUPEQ_V2_HACHE03:
         PuppetPart[336].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache03" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[336]; 
      break;
      case PUPEQ_V2_HACHE04:
         PuppetPart[337].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache04" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[337]; 
      break;
      case PUPEQ_V2_HACHE05:
         PuppetPart[338].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache05" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[338]; 
      break;
      case PUPEQ_V2_HACHE06:
         PuppetPart[339].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache06" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[339]; 
      break;
      case PUPEQ_V2_HACHE07:
         PuppetPart[340].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache07" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[340]; 
      break;
      case PUPEQ_V2_HACHE08:
         PuppetPart[341].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache08" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[341]; 
      break;
      case PUPEQ_V2_HACHE09:
         PuppetPart[342].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache09" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[342]; 
      break;
      case PUPEQ_V2_HACHE10:
         PuppetPart[343].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache10" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[343]; 
      break;
      case PUPEQ_V2_HACHE11:
         PuppetPart[344].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hache11" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[344]; 
      break;

     
      case PUPEQ_V2_BOW01:
         PuppetPart[355].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow01", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[355]; 
      break;
      case PUPEQ_V2_BOW02:
         PuppetPart[356].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow02", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[356]; 
      break;
      case PUPEQ_V2_BOW03:
         PuppetPart[357].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow03", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[357]; 
      break;
      case PUPEQ_V2_BOW04:
         PuppetPart[358].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow04", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[358]; 
      break;
      case PUPEQ_V2_BOW05:
         PuppetPart[359].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow05", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[359]; 
      break;
      case PUPEQ_V2_BOW06:
         PuppetPart[360].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow06", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[360]; 
      break;
      case PUPEQ_V2_BOW07:
         PuppetPart[361].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow07", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[361]; 
      break;
      case PUPEQ_V2_BOW08:
         PuppetPart[362].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow08", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[362]; 
      break;
      case PUPEQ_V2_BOW09:
         PuppetPart[363].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow09", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[363]; 
      break;
      case PUPEQ_V2_BOW10:
         PuppetPart[364].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_Bow10", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[364]; 
      break;

      case PUPEQ_V2_SCEPTRE01:		 	 
			PuppetPart[365].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sceptre01", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[365]; 
		break;
      case PUPEQ_V2_SCEPTRE02:		 	 
			PuppetPart[366].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sceptre02", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[366]; 
		break;
      case PUPEQ_V2_SCEPTRE03:		 	 
			PuppetPart[367].LoadSprite3D(13, 9, 0, 0, 0, "V2_Sceptre03", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[367]; 
		break;

      case PUPEQ_V2_SP01:
         PuppetPart[368].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special01" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[368]; 
      break;
      case PUPEQ_V2_SP02:
         PuppetPart[369].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special02" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[369]; 
      break;
      case PUPEQ_V2_SP03:
         PuppetPart[370].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special03" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[370]; 
      break;
      case PUPEQ_V2_SP04:
         PuppetPart[371].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special04" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[371]; 
      break;
      case PUPEQ_V2_SP05:
         PuppetPart[372].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special05" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[372]; 
      break;
      case PUPEQ_V2_SP06:
         PuppetPart[373].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special06" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[373]; 
      break;
      case PUPEQ_V2_SP07:
         PuppetPart[374].LoadSprite3D(13, 9, 0, 0, 0, "V2_Special07" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[374]; 
      break;

      case PUPEQ_V2_HAMMER01:
         PuppetPart[381].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer01" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[381]; 
      break;
      case PUPEQ_V2_HAMMER02:
         PuppetPart[382].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer02" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[382]; 
      break;
      case PUPEQ_V2_HAMMER03:
         PuppetPart[383].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer03" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[383]; 
      break;
      case PUPEQ_V2_HAMMER04:
         PuppetPart[384].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer04" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[384]; 
      break;
      case PUPEQ_V2_HAMMER05:
         PuppetPart[385].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer05" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[385]; 
      break;
      case PUPEQ_V2_HAMMER06:
         PuppetPart[386].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer06" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[386]; 
      break;
      case PUPEQ_V2_HAMMER07:
         PuppetPart[387].LoadSprite3D(13, 9, 0, 0, 0, "V2_Hammer07" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[387]; 
      break;

     
      case PUPEQ_V2_BOW05_ICE:
         PuppetPart[395].LoadSprite3D(13, 0, 0, 0, 0, 9, "V2_IceBow01", 0, 0, 0); 
			Object->BodyPart[8] = &PuppetPart[395]; 
      break;

      case PUPEQ_T4CP_MATRAQUE:
         PuppetPart[415].LoadSprite3D(13, 9, 0, 0, 0, "T4CP_Matraque" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[415]; 
      break;
      case PUPEQ_T4CP_BAT:
         PuppetPart[416].LoadSprite3D(13, 9, 0, 0, 0, "TankBat" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[416]; 
      break;
      case PUPEQ_T4CP_12:
         PuppetPart[417].LoadSprite3D(13, 9, 0, 0, 0, "T4CP_12" , 0, 0, 0);
			Object->BodyPart[8] = &PuppetPart[417]; 
      break;

      
      


//Dialsoft end

	}

   // Shield
	Object->VisiblePart |= Pow2[9];	
	switch (Object->PuppetInfo[9]) {
		case PUPEQ_NACKED:			
			Object->VisiblePart -= Pow2[9];
		break;
		case PUPEQ_ROMANSHIELD:		
			PuppetPart[42].LoadSprite3D(13, 9, 0, 0, 0, "PupRomanShield", 0, 0, 0); 
			Object->BodyPart[9] = &PuppetPart[42]; 
		break;
		case PUPEQ_BAROSSASHIELD:  
			PuppetPart[43].LoadSprite3D(13, 9, 0, 0, 0, "PupBarossaShield", 0, 0, 0); 
			Object->BodyPart[9] = &PuppetPart[43]; 
		break;
		case PUPEQ_ORCSHIELD:		
			PuppetPart[44].LoadSprite3D(13, 9, 0, 0, 0, "PupOrcShield", 0, 0, 0); 
			Object->BodyPart[9] = &PuppetPart[44]; 
		break;
		case PUPEQ_CENTAURSHIELD1:		
			PuppetPart[101].LoadSprite3D(13, 9, 0, 0, 0, "PupCentaurShield", 0, 0, 0); 
			Object->BodyPart[9] = &PuppetPart[101]; 
		break;
		case PUPEQ_CENTAURSHIELD2:		
			PuppetPart[100].LoadSprite3D(13, 9, 0, 0, 0, "PupCentaurShield2", 0, 0, 0); 
			Object->BodyPart[9] = &PuppetPart[100]; 
		break;
		case PUPEQ_SKAVENSHIELD1:
			PuppetPart[105].LoadSprite3D(13, 9, 0, 0, 0, "PupSkavenShield1", 0, 0, 0); 
			Object->BodyPart[9] = &PuppetPart[105];                      
		break;
		case PUPEQ_SKAVENSHIELD2:
			PuppetPart[106].LoadSprite3D(13, 9, 0, 0, 0, "PupSkavenShield2", 0, 0, 0); 
			Object->BodyPart[9] = &PuppetPart[106]; 
		break;
		case PUPEQ_SKAVENSHIELD3:
			PuppetPart[107].LoadSprite3D(13, 9, 0, 0, 0, "PupSkavenShield3", 0, 0, 0); 
			Object->BodyPart[9] = &PuppetPart[107]; 
		break;
		case PUPEQ_SKELSHIELD:
			PuppetPart[110].LoadSprite3D(13, 9, 0, 0, 0, "PupSkeletonShield", 0, 0, 0); 
			Object->BodyPart[9] = &PuppetPart[110]; 
		break;
      case PUPEQ_ORCSHIELD2:
			PuppetPart[290].LoadSprite3D(13, 9, 0, 0, 0, "PupOrcShield2", 0, 0, 0 );
			Object->BodyPart[9] = &PuppetPart[290]; 
		break;
      case PUPEQ_SKSHIELD:
			PuppetPart[291].LoadSprite3D(13, 9, 0, 0, 0, "SkShield", 0, 0, 0 );
			Object->BodyPart[9] = &PuppetPart[291]; 
		break;
      case PUPEQ_V2_SHIELD01:
			PuppetPart[392].LoadSprite3D(13, 9, 0, 0, 0, "V2_Shield01", 0, 0, 0 );
			Object->BodyPart[9] = &PuppetPart[392]; 
		break;
      case PUPEQ_V2_SHIELD02:
			PuppetPart[393].LoadSprite3D(13, 9, 0, 0, 0, "V2_Shield02", 0, 0, 0 );
			Object->BodyPart[9] = &PuppetPart[393]; 
		break;
      case PUPEQ_T4CP_SHIELD:
			PuppetPart[414].LoadSprite3D(13, 9, 0, 0, 0, "T4CP_Shield", 0, 0, 0 );
			Object->BodyPart[9] = &PuppetPart[414]; 
		break;
      case PUPEQ_V2_ICESHIELD01:
			PuppetPart[418].LoadSprite3D(13, 9, 0, 0, 0, "V2_IceShield01", 0, 0, 0 );
			Object->BodyPart[9] = &PuppetPart[418]; 
		break;
      

      
  	}

	// BOOT
	Object->VisiblePart |= Pow2[10];
   switch (Object->PuppetInfo[10]) {
      case PUPEQ_NACKED:		 
			Object->VisiblePart -= Pow2[10]; 
		break;
      case PUPEQ_LEATHER:		 
         if (Object->Type == 10011) {
            PuppetPart[45].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherBoots", 0, 0, 0); 
			   Object->BodyPart[10] = &PuppetPart[45]; 
         } else {
            PuppetPart[74].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherBoots", 0, 0, 0);
			   Object->BodyPart[10] = &PuppetPart[74]; 
         }
		break;
	//Tiamat changes start
      case PUPEQ_PLATE:			 
         if (Object->Type == 10011) {
            PuppetPart[46].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot", 0, 0, 0); 
	   		Object->BodyPart[10] = &PuppetPart[46]; 
         } else {
            PuppetPart[83].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0); 
	   		Object->BodyPart[10] = &PuppetPart[83]; 
         } 
		break;
	case PUPEQ_PLATE_GOLD:			 
         if (Object->Type == 10011) {
            PuppetPart[155].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot", 0, 0, 0, 2); 
	   		Object->BodyPart[10] = &PuppetPart[155]; 
         } else {
            PuppetPart[215].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 2); 
	   		Object->BodyPart[10] = &PuppetPart[215]; 
         } 
		break;
	case PUPEQ_PLATE_SILVER:			 
         if (Object->Type == 10011) {
            PuppetPart[163].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot", 0, 0, 0, 3); 
	   		Object->BodyPart[10] = &PuppetPart[163]; 
         } else {
            PuppetPart[221].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 3); 
	   		Object->BodyPart[10] = &PuppetPart[221]; 
         } 
		break;
	case PUPEQ_PLATE_BLACK:			 
         if (Object->Type == 10011) {
            PuppetPart[171].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot", 0, 0, 0, 4); 
	   		Object->BodyPart[10] = &PuppetPart[171]; 
         } else {
            PuppetPart[227].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 4); 
	   		Object->BodyPart[10] = &PuppetPart[227]; 
         } 
		break;
	case PUPEQ_PLATE_BLUE:			 
         if (Object->Type == 10011) {
            PuppetPart[179].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot", 0, 0, 0, 5); 
	   		Object->BodyPart[10] = &PuppetPart[179]; 
         } else {
            PuppetPart[233].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 5); 
	   		Object->BodyPart[10] = &PuppetPart[233]; 
         } 
		break;
	case PUPEQ_PLATE_GREEN:			 
         if (Object->Type == 10011) {
            PuppetPart[187].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot", 0, 0, 0, 6); 
	   		Object->BodyPart[10] = &PuppetPart[187]; 
         } else {
            PuppetPart[239].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 6); 
	   		Object->BodyPart[10] = &PuppetPart[239]; 
         } 
		break;
	case PUPEQ_PLATE_RED:			 
         if (Object->Type == 10011) {
            PuppetPart[195].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot", 0, 0, 0, 7); 
	   		Object->BodyPart[10] = &PuppetPart[195]; 
         } else {
            PuppetPart[245].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 7); 
	   		Object->BodyPart[10] = &PuppetPart[245]; 
         } 
		break;
	case PUPEQ_PLATE_PURPLE:			 
         if (Object->Type == 10011) {
            PuppetPart[203].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot", 0, 0, 0, 8); 
	   		Object->BodyPart[10] = &PuppetPart[203]; 
         } else {
            PuppetPart[251].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 8); 
	   		Object->BodyPart[10] = &PuppetPart[251]; 
         } 
		break;
	case PUPEQ_PLATE_EMERALD:			 
         if (Object->Type == 10011) {
            PuppetPart[211].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateFoot", 0, 0, 0, 9); 
	   		Object->BodyPart[10] = &PuppetPart[211]; 
         } else {
            PuppetPart[257].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateBoots", 0, 0, 0, 9); 
	   		Object->BodyPart[10] = &PuppetPart[257]; 
         } 
		break;
	//Tiamat Changes End
		case PUPEQ_BLACKLEATHER: 
			if (Object->Type == 10011) 
         {
            PuppetPart[47].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupBlackLeatherBoots", 0, 0, 0); 
			   Object->BodyPart[10] = &PuppetPart[47]; 
         } 
         else 
         {
            PuppetPart[86].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoBlackLeatherBoots", 0, 0, 0); 
			   Object->BodyPart[10] = &PuppetPart[86]; 
         }
		break;
      case PUPEQ_WHITELEATHER: 
			if (Object->Type == 10011) 
         {
            PuppetPart[419].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupBlackLeatherBoots", 0, 0, 0,2); 
			   Object->BodyPart[10] = &PuppetPart[419]; 
         } 
         else 
         {
            PuppetPart[420].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoBlackLeatherBoots", 0, 0, 0,2); 
			   Object->BodyPart[10] = &PuppetPart[420]; 
         }
		break;
      


		case PUPEQ_NECROROBE:   
			Object->VisiblePart -= Pow2[10]; 
		break;
		case PUPEQ_WHITEROBE:   
      case PUPEQ_LICHROBE: 
      case PUPEQ_ARMORED_ROBE:
			Object->VisiblePart -= Pow2[10]; 
		break;
		case PUPEQ_REDROBE: 
			Object->VisiblePart -= Pow2[10]; 
		break;
		case PUPEQ_MAGEROBE:		
			Object->VisiblePart -= Pow2[10]; 
		break;
		case PUPEQ_FWHITEROBE_BLUE: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
		case PUPEQ_FWHITEROBE_GREEN: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
		case PUPEQ_FWHITEROBE_DARKBLUE: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
		case PUPEQ_FWHITEROBE_VIOLET: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
		case PUPEQ_FWHITEROBE_PURPLE: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
		case PUPEQ_FWHITEROBE_YELLOW: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
		case PUPEQ_FWHITEROBE_GRAY: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
		case PUPEQ_FWHITEROBE_MAUVE: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
		case PUPEQ_FWHITEROBE_ORANGE: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
		case PUPEQ_FWHITEROBE_BRUN: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
		case PUPEQ_FWHITEROBE_DARK: 
			Object->VisiblePart -= Pow2[10]; 
		break; 
      case PUPEQ_V2_MAN_ARMOR01GR:
         if (Object->Type == 10011) 
         {
            PuppetPart[403].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBoots01" , 0, 0, 0);
			   Object->BodyPart[10] = &PuppetPart[403]; 
         } 
         else 
         {
            PuppetPart[403].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBoots01" , 0, 0, 0);
			   Object->BodyPart[10] = &PuppetPart[403]; 
         }
      break;
      case PUPEQ_V2_MAN_ARMOR01BL:
         if (Object->Type == 10011) 
         {
            PuppetPart[407].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBoots01" , 0, 0, 0,2);
			   Object->BodyPart[10] = &PuppetPart[407]; 
         } 
         else 
         {
            PuppetPart[407].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_ManArmorBoots01" , 0, 0, 0,2);
			   Object->BodyPart[10] = &PuppetPart[407]; 
         }
      break;
	}

	// HAT
	Object->VisiblePart |= Pow2[11];
   switch (Object->PuppetInfo[11]) {
      case PUPEQ_NACKED:	 	   
			Object->VisiblePart -= Pow2[11]; 
		break;
		case PUPEQ_GOLDENCROWN:		
			PuppetPart[48].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupGoldenCrown", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[48]; 
		break;	
		case PUPEQ_ELVEN:				
			PuppetPart[49].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupElvenHat", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[49]; 
		break;
      case PUPEQ_WITCHHAT1:
         PuppetPart[272].LoadSprite3D(13, 9, 0, 0, 0, 9, "WitchHat1", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[272]; 
      break;
      case PUPEQ_WITCHHAT2:
         PuppetPart[273].LoadSprite3D(13, 9, 0, 0, 0, 9, "WitchHat2", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[273]; 
      break;
      case PUPEQ_WITCHHAT3:
         PuppetPart[274].LoadSprite3D(13, 9, 0, 0, 0, 9, "WitchHat3", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[274]; 
      break;
      case PUPEQ_WITCHHAT4:
         PuppetPart[398].LoadSprite3D(13, 9, 0, 0, 0, 9, "WitchHat4", 0, 0, 0);
			Object->BodyPart[11] = &PuppetPart[398]; 
      break;
      case PUPEQ_WITCHHAT5:
         PuppetPart[399].LoadSprite3D(13, 9, 0, 0, 0, 9, "WitchHat5", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[399]; 
      break;
      case PUPEQ_NOEL_HAT:
         PuppetPart[289].LoadSprite3D(13, 9, 0, 0, 0, 9, "NoelHat", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[289]; 
      break;

      case PUPEQ_T4CP_CASQUE:
         PuppetPart[412].LoadSprite3D(13, 9, 0, 0, 0, 9, "T4CP_Casque", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[412]; 
      break;
      case PUPEQ_T4CP_CASQUETTE:
         PuppetPart[413].LoadSprite3D(13, 9, 0, 0, 0, 9, "T4CP_Casquette", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[413]; 
      break;
      
      

      case PUPEQ_V2_HAUME01:
          PuppetPart[292].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Haume01", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[292]; 
      break;
      case PUPEQ_V2_HAUME02:
          PuppetPart[293].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Haume02", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[293]; 
      break;
      case PUPEQ_V2_HAUME03:
          PuppetPart[294].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Haume03", 0, 0, 0);
			Object->BodyPart[11] = &PuppetPart[294]; 
      break;
      case PUPEQ_V2_HAUME04:
          PuppetPart[295].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Haume04", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[295]; 
      break;
      case PUPEQ_V2_HAT01:
          PuppetPart[296].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Hat01", 0, 0, 0);
			Object->BodyPart[11] = &PuppetPart[296]; 
      break;
      case PUPEQ_V2_HAUME04_BL:
          PuppetPart[410].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Haume04", 0, 0, 0,2); 
			Object->BodyPart[11] = &PuppetPart[410]; 
      break;
      


      
      case PUPEQ_LEATHER:
         if (Object->Type == 10011) {
            PuppetPart[90].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupLeatherHelm", 0, 0, 0); 
			   Object->BodyPart[11] = &PuppetPart[90]; 
         } else {
            PuppetPart[91].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherHelm", 0, 0, 0); 
			   Object->BodyPart[11] = &PuppetPart[91]; 
         }
      break;
	  //Tiamat changes start
      case PUPEQ_PLATE: 
         if (Object->Type == 10011) {
            PuppetPart[23].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm", 0, 0, 0); 
			   Object->BodyPart[11] = &PuppetPart[23]; 
         } else {
            PuppetPart[61].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0); 
			   Object->BodyPart[11] = &PuppetPart[61]; 
         }
		break;
	case PUPEQ_PLATE_GOLD: 
         if (Object->Type == 10011) {
            PuppetPart[152].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm", 0, 0, 0, 2); 
			   Object->BodyPart[11] = &PuppetPart[152]; 
         } else {
            PuppetPart[212].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 2); 
			   Object->BodyPart[11] = &PuppetPart[212]; 
         }
		break;
	case PUPEQ_PLATE_SILVER: 
         if (Object->Type == 10011) {
            PuppetPart[160].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm", 0, 0, 0, 3); 
			   Object->BodyPart[11] = &PuppetPart[160]; 
         } else {
            PuppetPart[218].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 3); 
			   Object->BodyPart[11] = &PuppetPart[218]; 
         }
		break;
	case PUPEQ_PLATE_BLACK: 
         if (Object->Type == 10011) {
            PuppetPart[168].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm", 0, 0, 0, 4); 
			   Object->BodyPart[11] = &PuppetPart[168]; 
         } else {
            PuppetPart[224].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 4); 
			   Object->BodyPart[11] = &PuppetPart[224]; 
         }
		break;
	case PUPEQ_PLATE_BLUE: 
         if (Object->Type == 10011) {
            PuppetPart[176].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm", 0, 0, 0, 5); 
			   Object->BodyPart[11] = &PuppetPart[176]; 
         } else {
            PuppetPart[230].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 5); 
			   Object->BodyPart[11] = &PuppetPart[230]; 
         }
		break;
	case PUPEQ_PLATE_GREEN: 
         if (Object->Type == 10011) {
            PuppetPart[184].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm", 0, 0, 0, 6); 
			   Object->BodyPart[11] = &PuppetPart[184]; 
         } else {
            PuppetPart[236].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 6); 
			   Object->BodyPart[11] = &PuppetPart[236]; 
         }
		break;
	case PUPEQ_PLATE_RED:
         if (Object->Type == 10011) {
            PuppetPart[192].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm", 0, 0, 0, 7); 
			   Object->BodyPart[11] = &PuppetPart[192]; 
         } else {
            PuppetPart[242].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 7); 
			   Object->BodyPart[11] = &PuppetPart[242]; 
         }
		break;
	case PUPEQ_PLATE_PURPLE: 
         if (Object->Type == 10011) {
            PuppetPart[200].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm", 0, 0, 0, 8); 
			   Object->BodyPart[11] = &PuppetPart[200]; 
         } else {
            PuppetPart[248].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 8); 
			   Object->BodyPart[11] = &PuppetPart[248]; 
         }
		break;
	case PUPEQ_PLATE_EMERALD: 
         if (Object->Type == 10011) {
            PuppetPart[208].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupPlateHelm", 0, 0, 0, 9); 
			   Object->BodyPart[11] = &PuppetPart[208]; 
         } else {
            PuppetPart[254].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoPlateHelm", 0, 0, 0, 9); 
			   Object->BodyPart[11] = &PuppetPart[254]; 
         }
		break;
		// Tiamat changes end
      case PUPEQ_CHAIN: 
         if (Object->Type == 10011) {
   			PuppetPart[24].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupChainMailCoif", 0, 0, 0); 
	   		Object->BodyPart[11] = &PuppetPart[24]; 
         } else {
   			PuppetPart[87].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoChainMailCoif", 0, 0, 0); 
	   		Object->BodyPart[11] = &PuppetPart[87]; 
         }
		break;
      case PUPEQ_HORNED: 
			PuppetPart[25].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupHornedHelmet", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[25]; 
		break;
      case PUPEQ_SHAMAN: 
			PuppetPart[102].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupShamanHelm", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[102]; 
		break;
      case PUPEQ_SKEL: 
			PuppetPart[109].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSkeletonHelm", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[109]; 
		break;
	//Dialsoft new helmets
	case PUPEQ_REDFEATHER: 
			PuppetPart[115].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedFeHelmet", 0, 0, 0); 
			Object->BodyPart[11] = &PuppetPart[115]; 
		break;
	//Dialsoft end
	}

	// CAPE
	Object->VisiblePart |= Pow2[12];
   switch (Object->PuppetInfo[12]) {
      case PUPEQ_NACKED: 	   
			Object->VisiblePart -= Pow2[12]; 
		break;
		case PUPEQ_REDCAPE:		
			PuppetPart[50].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape", 0, 0, 0); 
			Object->BodyPart[12] = &PuppetPart[50]; 
		break;	
		case PUPEQ_BLACKWINGS:		
			PuppetPart[112].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphBlackWings", 0, 0, 0); 
			Object->BodyPart[12] = &PuppetPart[112]; 
		break;
      case PUPEQ_NM_DECHUWINGS:		
			PuppetPart[411].LoadSprite3D(13, 9, 0, 0, 0, 9, "NM_DechuWings", 0, 0, 0); 
			Object->BodyPart[12] = &PuppetPart[411]; 
		break;
      case PUPEQ_NM2_DECHUWINGS:		
			PuppetPart[421].LoadSprite3D(13, 9, 0, 0, 0, 9, "NM2_DechuWings", 0, 0, 0); 
			Object->BodyPart[12] = &PuppetPart[421]; 
		break;
		case PUPEQ_WHITEWINGS:		
			PuppetPart[113].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0); 
			Object->BodyPart[12] = &PuppetPart[113]; 
		break;
		case PUPEQ_DARKWINGS:
			PuppetPart[118].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphDarkWings", 0, 0, 0);
			Object->BodyPart[12] = &PuppetPart[118];
		break;
		case PUPEQ_REDWINGS:		
			PuppetPart[120].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 2); 
			Object->BodyPart[12] = &PuppetPart[120]; 
		break;
		case PUPEQ_NEUTRALWINGS:		
			PuppetPart[119].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 3); 
			Object->BodyPart[12] = &PuppetPart[119]; 
		break;
		case PUPEQ_GOLDWINGS:		
			PuppetPart[121].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 4); 
			Object->BodyPart[12] = &PuppetPart[121]; 
		break;
		case PUPEQ_DARKREDWINGS:		
			PuppetPart[122].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 5); 
			Object->BodyPart[12] = &PuppetPart[122]; 
		break;
		case PUPEQ_DARKGOLDWINGS:		
			PuppetPart[123].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 6); 
			Object->BodyPart[12] = &PuppetPart[123]; 
		break;
		case PUPEQ_LIGHTBLUEWINGS:		
			PuppetPart[124].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 7); 
			Object->BodyPart[12] = &PuppetPart[124]; 
		break;
		case PUPEQ_ORANGEWINGS:		
			PuppetPart[125].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupSeraphWhiteWings", 0, 0, 0, 8); 
			Object->BodyPart[12] = &PuppetPart[125]; 
		break;
      case PUPEQ_GREENCAPE:		
			PuppetPart[265].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape",   0, 0, 0, 2); 
			Object->BodyPart[12] = &PuppetPart[265];
        break;
		case PUPEQ_YELLOWCAPE:	
			PuppetPart[266].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape",   0, 0, 0, 3); 
			Object->BodyPart[12] = &PuppetPart[266];
        break;
		case PUPEQ_SILVERCAPE:		
			PuppetPart[267].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape",   0, 0, 0, 4); 
			Object->BodyPart[12] = &PuppetPart[267];
        break;
		case PUPEQ_PURPLECAPE:		
			PuppetPart[268].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape",   0, 0, 0, 5); 
			Object->BodyPart[12] = &PuppetPart[268];
        break;
		case PUPEQ_BLACKCAPE:		
			PuppetPart[269].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape",   0, 0, 0, 6); 
			Object->BodyPart[12] = &PuppetPart[269];
        break;
		case PUPEQ_AQUACAPE:		
			PuppetPart[270].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape",   0, 0, 0, 7); 
			Object->BodyPart[12] = &PuppetPart[270];
        break;
		case PUPEQ_BLUECAPE:		
			PuppetPart[271].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCape",   0, 0, 0, 8); 
			Object->BodyPart[12] = &PuppetPart[271];
      break;
      case PUPEQ_BUTTERFLYWINGS1:
         PuppetPart[281].LoadSprite3D(13, 9, 0, 0, 0, 9, "ButterFlyWing", 0, 0, 0);
			Object->BodyPart[12] = &PuppetPart[281];
      break;
      case PUPEQ_BUTTERFLYWINGS2:
         PuppetPart[282].LoadSprite3D(13, 9, 0, 0, 0, 9, "ButterFlyWing", 0, 0, 0,2);
			Object->BodyPart[12] = &PuppetPart[282];
      break;
      case PUPEQ_BUTTERFLYWINGS3:
         PuppetPart[283].LoadSprite3D(13, 9, 0, 0, 0, 9, "ButterFlyWing", 0, 0, 0,3);
			Object->BodyPart[12] = &PuppetPart[283];
      break;
      case PUPEQ_BUTTERFLYWINGS4:
         PuppetPart[284].LoadSprite3D(13, 9, 0, 0, 0, 9, "ButterFlyWing", 0, 0, 0,4);
			Object->BodyPart[12] = &PuppetPart[284];
      break;
      case PUPEQ_ARCHWINGSW:
         PuppetPart[286].LoadSprite3D(13, 9, 0, 0, 0, 9, "ArchWings", 0, 0, 0, 0);
			Object->BodyPart[12] = &PuppetPart[286];
      break;
      

	}

   // BACKBODY
	Object->VisiblePart |= Pow2[13];
   switch (Object->PuppetInfo[13]) {
      case PUPEQ_NACKED: 	   
			Object->VisiblePart -= Pow2[13]; 
		break;
		case PUPEQ_ARMORED_ROBE:	
         if (Object->Type == 10011) 
         {
   			PuppetPart[51].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupArmoredRobeUnder", 0, 0, 0); 
	   		Object->BodyPart[13] = &PuppetPart[51]; 
         } 
         else 
         {
            Object->VisiblePart -= Pow2[13];
         }
		break;	
		case PUPEQ_MAGEROBE:	
         if (Object->Type == 10011) 
         {
            Object->VisiblePart -= Pow2[13];
         } 
         else 
         {
   			PuppetPart[98].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoMageRobeUnder", 0, 0, 0); 
	   		Object->BodyPart[13] = &PuppetPart[98];
         }
		break;	
      case PUPEQ_LEATHER:		
         if (Object->Type == 10011) {
   			Object->VisiblePart -= Pow2[13]; 
         } else {
            PuppetPart[63].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherBody", 0, 0, 0); 
			   Object->BodyPart[13] = &PuppetPart[63]; 
         }
		break;
      case PUPEQ_SPIKEDLEATHER:
         if (Object->Type == 10011) 
         {
   		   Object->VisiblePart -= Pow2[13]; 
         } 
         else 
         {
            PuppetPart[63].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoLeatherBody", 0, 0, 0); 
			   Object->BodyPart[13] = &PuppetPart[63]; 
         }

	}

	// CAPE 2
	Object->VisiblePart |= Pow2[14];
   switch (Object->PuppetInfo[14]) {
      case PUPEQ_NACKED: 	   
			Object->VisiblePart -= Pow2[14]; 
		break;
		case PUPEQ_REDCAPE:		
			PuppetPart[77].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupRedCapeTop", 0, 0, 0); 
			Object->BodyPart[14] = &PuppetPart[77]; 
		break;	
      case PUPEQ_MAGEROBE:
         PuppetPart[99].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupMageRobeCollar", 0, 0, 0 );
			Object->BodyPart[14] = &PuppetPart[99];
      break;
	}

	// HAIR
   //being Tiamat Changes
	Object->VisiblePart |= Pow2[15];
   switch (Object->PuppetInfo[15]) 
   {
      case PUPEQ_NACKED: 	   
         if (Object->PuppetInfo[11] == PUPEQ_PLATE_GOLD   || Object->PuppetInfo[11] == PUPEQ_PLATE         || Object->PuppetInfo[11] == PUPEQ_CHAIN     || 
             Object->PuppetInfo[11] == PUPEQ_PLATE_SILVER || Object->PuppetInfo[11] == PUPEQ_HORNED        || Object->PuppetInfo[11] == PUPEQ_LEATHER   ||
			    Object->PuppetInfo[11] == PUPEQ_PLATE_BLUE   || Object->PuppetInfo[11] == PUPEQ_PLATE_GREEN   || Object->PuppetInfo[11] == PUPEQ_PLATE_RED ||
			    Object->PuppetInfo[11] == PUPEQ_PLATE_PURPLE || Object->PuppetInfo[11] == PUPEQ_PLATE_EMERALD ||
             Object->PuppetInfo[11] == PUPEQ_PLATE_BLACK  || Object->PuppetInfo[11] == PUPEQ_STUDDED       || 
             Object->PuppetInfo[11] == PUPEQ_HORNED       || 
             //Object->PuppetInfo[11] == PUPEQ_WITCHHAT     || Object->PuppetInfo[11] == PUPEQ_WITCHHATRED  || Object->PuppetInfo[11] == PUPEQ_WITCHHATBLUE  || 
             //Object->PuppetInfo[11] == PUPEQ_NOEL_HAT     ||
             Object->PuppetInfo[11] == PUPEQ_V2_HAUME01   || Object->PuppetInfo[11] == PUPEQ_V2_HAUME02    || Object->PuppetInfo[11] == PUPEQ_V2_HAUME04  || Object->PuppetInfo[11] == PUPEQ_V2_HAUME04_BL 
             ) 
         {
   			Object->VisiblePart -= Pow2[15]; 
         } 
         else 
         {
            if (Object->Type == 10011) 
            {
               PuppetPart[88].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNormalHair", 0, 0, 0);
			      Object->BodyPart[15] = &PuppetPart[88];   
            } 
            else 
            {
//               if (Player.Name[0] & 1) {
                  PuppetPart[78].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoHairPonyTail", 0, 0, 0);
			         Object->BodyPart[15] = &PuppetPart[78];   
/*               } else {
                  PuppetPart[89].LoadSprite3D(13, 9, 0, 0, 0, "WoSuperHair", 0, 0, 0);
			         Object->BodyPart[15] = &PuppetPart[89];   
               } */
            }
         }
		break;
		case PUPEQ_HAIR1:		
         if ( Object->PuppetInfo[11] == PUPEQ_PLATE_GOLD   || Object->PuppetInfo[11] == PUPEQ_PLATE         || Object->PuppetInfo[11] == PUPEQ_CHAIN   || 
              Object->PuppetInfo[11] == PUPEQ_PLATE_SILVER || Object->PuppetInfo[11] == PUPEQ_HORNED        || Object->PuppetInfo[11] == PUPEQ_LEATHER ||
			     Object->PuppetInfo[11] == PUPEQ_PLATE_GREEN  || Object->PuppetInfo[11] == PUPEQ_PLATE_RED     ||
			     Object->PuppetInfo[11] == PUPEQ_PLATE_PURPLE || Object->PuppetInfo[11] == PUPEQ_PLATE_EMERALD ||
              Object->PuppetInfo[11] == PUPEQ_PLATE_BLUE   || Object->PuppetInfo[11] == PUPEQ_PLATE_BLACK   ||Object->PuppetInfo[11] == PUPEQ_STUDDED  ||
              Object->PuppetInfo[11] == PUPEQ_HORNED       || 
              //Object->PuppetInfo[11] == PUPEQ_WITCHHAT     || Object->PuppetInfo[11] == PUPEQ_WITCHHATRED  || Object->PuppetInfo[11] == PUPEQ_WITCHHATBLUE  || 
              //Object->PuppetInfo[11] == PUPEQ_NOEL_HAT     ||
              Object->PuppetInfo[11] == PUPEQ_V2_HAUME01   || Object->PuppetInfo[11] == PUPEQ_V2_HAUME02    || Object->PuppetInfo[11] == PUPEQ_V2_HAUME04  || Object->PuppetInfo[11] == PUPEQ_V2_HAUME04_BL
              ) 
         {
   			  Object->VisiblePart -= Pow2[15]; 
         } 
         else 
         {
            if (Object->Type == 10011) 
            {
               PuppetPart[87].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNormalHair", 0, 0, 0);
			      Object->BodyPart[15] = &PuppetPart[87];   
            } 
            else 
            {
               PuppetPart[78].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoHairPonyTail", 0, 0, 0);
			      Object->BodyPart[15] = &PuppetPart[78];   
            }
         }
		break;
		//End Tiamat Changes
	}

	// ROBE LEGS
	Object->VisiblePart |= Pow2[16];
   switch (Object->PuppetInfo[16]) {
      case PUPEQ_NACKED: 	   
			Object->VisiblePart -= Pow2[16]; 
		break;
      case PUPEQ_SET1:	
         if (Object->Type == 10011) 
         {
            Object->VisiblePart -= Pow2[16];
         } 
         else 
         {
            PuppetPart[70].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoClothRobe", 0, 0, 0); 
			   Object->BodyPart[16] = &PuppetPart[70]; 
         }
      break;
      case PUPEQ_LICHROBE:
         PuppetPart[60].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedArmL", 0, 0, 0); 
   	   Object->BodyPart[16] = &PuppetPart[60]; 
      break;
      case PUPEQ_ARMORED_ROBE:
         if (Object->Type == 10011) 
         {
            PuppetPart[60].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupNakedArmL", 0, 0, 0); 
   	      Object->BodyPart[16] = &PuppetPart[60]; 
         } 
         else 
         {
            PuppetPart[60].LoadSprite3D(13, 9, 0, 0, 0, 9, "WoNakedArmL", 0, 0, 0); 
   	      Object->BodyPart[16] = &PuppetPart[60]; 
         }
      break;
	}
   Object->VisiblePart |= Pow2[17];
   switch (Object->PuppetInfo[17]) 
   {
      case PUPEQ_NACKED: 	   
			Object->VisiblePart -= Pow2[17]; 
		break;
      case PUPEQ_GOBMASK:
			PuppetPart[262].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupGobmask", 0, 0, 0); 
			Object->BodyPart[17] = &PuppetPart[262]; 
		break;
      case PUPEQ_V2_MASK01:		 	 
			PuppetPart[297].LoadSprite3D(13, 9, 0, 0, 0, 9, "V2_Mask01", 0, 0, 0);
			Object->BodyPart[17] = &PuppetPart[297]; 
		break;
      case PUPEQ_V2_GOBMASK_ICE:		 	 
			PuppetPart[396].LoadSprite3D(13, 9, 0, 0, 0, 9, "PupGobmask", 0, 0, 0,2); 
			Object->BodyPart[17] = &PuppetPart[396]; 
		break;
   }

   Object->VisiblePart |= Pow2[18];
   switch (Object->PuppetInfo[18]) 
   {
      case PUPEQ_NACKED: 	   
			Object->VisiblePart -= Pow2[18]; 
		break;
      //NM_TAG_ADD_SWORD_hand2
      case PUPEQ_V2_2BUSSWORD01:
         PuppetPart[319].LoadSprite3D(13, 9, 0, 0, 0, "V2_2BusterSlayer01" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[319]; 
      break;
      case PUPEQ_V2_2BUSSWORD02:
         PuppetPart[320].LoadSprite3D(13, 9, 0, 0, 0, "V2_2BusterSlayer02" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[320]; 
      break;
      case PUPEQ_V2_2BUSSWORD03:
         PuppetPart[321].LoadSprite3D(13, 9, 0, 0, 0, "V2_2BusterSlayer03" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[321]; 
      break;
      case PUPEQ_V2_2CLAYSWORD01:
         PuppetPart[322].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Claymore01" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[322]; 
      break;
      case PUPEQ_V2_2CLAYSWORD02:
         PuppetPart[323].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Claymore02" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[323]; 
      break;
      case PUPEQ_V2_2GLAIVESWORD01:
         PuppetPart[324].LoadSprite3D(13, 9, 0, 0, 0, "V2_2GlaiveSlayer01" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[324]; 
      break;
      case PUPEQ_V2_2GLAIVESWORD02:
         PuppetPart[325].LoadSprite3D(13, 9, 0, 0, 0, "V2_2GlaiveSlayer02" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[325]; 
      break;
      case PUPEQ_V2_2GLAIVESWORD02_ICE:
         PuppetPart[397].LoadSprite3D(13, 9, 0, 0, 0, "V2_2GlaiveSlayer02" , 0, 0, 0,2);
			Object->BodyPart[18] = &PuppetPart[397]; 
      break;
      case PUPEQ_V2_2SWORD01:
         PuppetPart[326].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword01" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[326]; 
      break;
      case PUPEQ_V2_2SWORD02:
         PuppetPart[327].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword02" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[327]; 
      break;
      case PUPEQ_V2_2SWORD03:
         PuppetPart[328].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword03" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[328]; 
      break;
      case PUPEQ_V2_2SWORD04:
         PuppetPart[329].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword04" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[329]; 
      break;
      case PUPEQ_V2_2SWORD05:
         PuppetPart[330].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword05" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[330]; 
      break;
      case PUPEQ_V2_2SWORD06:
         PuppetPart[331].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword06" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[331]; 
      break;
      case PUPEQ_V2_2SWORD07:
         PuppetPart[332].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword07" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[332]; 
      break;
      case PUPEQ_V2_2SWORD08:
         PuppetPart[333].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Sword08" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[333]; 
      break;

      case PUPEQ_V2_2HACHE01:
         PuppetPart[345].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache01" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[345]; 
      break;
      case PUPEQ_V2_2HACHE02:
         PuppetPart[346].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache02" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[346]; 
      break;
      case PUPEQ_V2_2HACHE03:
         PuppetPart[347].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache03" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[347]; 
      break;
      case PUPEQ_V2_2HACHE04:
         PuppetPart[348].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache04" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[348]; 
      break;
      case PUPEQ_V2_2HACHE05:
         PuppetPart[390].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache05" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[390]; 
      break;
      case PUPEQ_V2_2HACHE06:
         PuppetPart[391].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hache06" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[391]; 
      break;


      case PUPEQ_V2_2DAGUE01:
         PuppetPart[349].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague01" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[349]; 
      break;
      case PUPEQ_V2_2DAGUE02:
         PuppetPart[350].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague02" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[350]; 
      break;
      case PUPEQ_V2_2DAGUE03:
         PuppetPart[351].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague03" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[351]; 
      break;
      case PUPEQ_V2_2DAGUE04:
         PuppetPart[352].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague04" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[352]; 
      break;
      case PUPEQ_V2_2DAGUE05:
         PuppetPart[353].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague05" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[353]; 
      break;
      case PUPEQ_V2_2DAGUE06:
         PuppetPart[354].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Dague06" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[354]; 
      break;

      case PUPEQ_V2_2SP01:
         PuppetPart[375].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special01" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[375]; 
      break;
      case PUPEQ_V2_2SP02:
         PuppetPart[376].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special02" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[376]; 
      break;
      case PUPEQ_V2_2SP03:
         PuppetPart[377].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special03" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[377]; 
      break;
      case PUPEQ_V2_2SP04:
         PuppetPart[378].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special04" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[378]; 
      break;
      case PUPEQ_V2_2SP05:
         PuppetPart[379].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special05" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[379]; 
      break;
      case PUPEQ_V2_2SP06:
         PuppetPart[380].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Special06" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[380]; 
      break;
      case PUPEQ_V2_2HAMMER01:
         PuppetPart[388].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hammer01" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[388]; 
      break;
      case PUPEQ_V2_2HAMMER02:
         PuppetPart[389].LoadSprite3D(13, 9, 0, 0, 0, "V2_2Hammer02" , 0, 0, 0);
			Object->BodyPart[18] = &PuppetPart[389]; 
      break;

      

   }

   Object->nbBodyPart = 19;
   


   //##NMNMNM --[ Z-Order des partie du player a corriger certain...]
    // Ranged weapons!
    LOG << "\r\nRANGED WEAPON PUPPET!";

        for (int i = 0; i < 13; i++) { 
        // 045	       
        BodyOrderR[1][ 0][i] = PUP_CAPE;
        BodyOrderR[1][ 1][i] = PUP_BACKBODY;
        BodyOrderR[1][ 2][i] = PUP_HAND_RIGHT;
        BodyOrderR[1][ 3][i] = PUP_ARM_RIGHT;
        BodyOrderR[1][ 4][i] = PUP_FOOT;
        BodyOrderR[1][ 5][i] = PUP_LEGS;
        BodyOrderR[1][ 6][i] = PUP_BOOT;
        BodyOrderR[1][ 7][i] = PUP_ROBELEGS;
        BodyOrderR[1][ 8][i] = PUP_BODY;          
        BodyOrderR[1][ 9][i] = PUP_CAPE_2;
        BodyOrderR[1][10][i] = PUP_WEAPON;
        BodyOrderR[1][11][i] = PUP_HAND_LEFT;
        BodyOrderR[1][12][i] = PUP_ARM_LEFT;
        BodyOrderR[1][13][i] = PUP_HEAD;
        BodyOrderR[1][14][i] = PUP_HAIR;
        BodyOrderR[1][15][i] = PUP_HAT;
        BodyOrderR[1][16][i] = PUP_MASK;
        BodyOrderR[1][17][i] = PUP_SHIELD;
        BodyOrderR[1][18][i] = PUP_WEAPON2;

        // 180
        BodyOrderR[8][ 0][i] = PUP_WEAPON;
        BodyOrderR[8][ 1][i] = PUP_HAND_RIGHT;
        BodyOrderR[8][ 2][i] = PUP_HAND_LEFT;
        BodyOrderR[8][ 3][i] = PUP_FOOT;
        BodyOrderR[8][ 4][i] = PUP_LEGS;
        BodyOrderR[8][ 5][i] = PUP_BOOT;
        BodyOrderR[8][ 6][i] = PUP_ROBELEGS;
        BodyOrderR[8][ 7][i] = PUP_ARM_RIGHT;
        BodyOrderR[8][ 8][i] = PUP_ARM_LEFT;
        BodyOrderR[8][ 9][i] = PUP_SHIELD;
        BodyOrderR[8][10][i] = PUP_WEAPON2;
        BodyOrderR[8][11][i] = PUP_CAPE_2;
        BodyOrderR[8][12][i] = PUP_BODY;
        BodyOrderR[8][13][i] = PUP_MASK;
        BodyOrderR[8][14][i] = PUP_HEAD;
        BodyOrderR[8][15][i] = PUP_HAIR;
        BodyOrderR[8][16][i] = PUP_HAT;
        BodyOrderR[8][17][i] = PUP_BACKBODY;
        BodyOrderR[8][18][i] = PUP_CAPE;

        // 000
        BodyOrderR[2][ 0][i] = PUP_CAPE;
        BodyOrderR[2][ 1][i] = PUP_BACKBODY;
        BodyOrderR[2][ 2][i] = PUP_FOOT;
        BodyOrderR[2][ 3][i] = PUP_LEGS;
        BodyOrderR[2][ 4][i] = PUP_BOOT;
        BodyOrderR[2][ 5][i] = PUP_ROBELEGS;
        BodyOrderR[2][ 6][i] = PUP_BODY;
        BodyOrderR[2][ 7][i] = PUP_CAPE_2;
        BodyOrderR[2][ 8][i] = PUP_WEAPON;
        BodyOrderR[2][ 9][i] = PUP_HAND_RIGHT;
        BodyOrderR[2][10][i] = PUP_HAND_LEFT;
        BodyOrderR[2][11][i] = PUP_ARM_RIGHT;
        BodyOrderR[2][12][i] = PUP_ARM_LEFT;
        BodyOrderR[2][13][i] = PUP_HEAD; 
        BodyOrderR[2][14][i] = PUP_HAIR; 
        BodyOrderR[2][15][i] = PUP_HAT;
        BodyOrderR[2][16][i] = PUP_MASK;
        BodyOrderR[2][17][i] = PUP_SHIELD;
        BodyOrderR[2][18][i] = PUP_WEAPON2;
 
        // 315   	    
        BodyOrderR[3][ 0][i] = PUP_CAPE;
        BodyOrderR[3][ 1][i] = PUP_BACKBODY;
        BodyOrderR[3][ 2][i] = PUP_HAND_RIGHT;
        BodyOrderR[3][ 3][i] = PUP_ARM_RIGHT;
        BodyOrderR[3][ 4][i] = PUP_FOOT;
        BodyOrderR[3][ 5][i] = PUP_LEGS;
        BodyOrderR[3][ 6][i] = PUP_BOOT;
        BodyOrderR[3][ 7][i] = PUP_ROBELEGS;
        BodyOrderR[3][ 8][i] = PUP_BODY;
        BodyOrderR[3][ 9][i] = PUP_CAPE_2;
        BodyOrderR[3][10][i] = PUP_WEAPON;
        BodyOrderR[3][11][i] = PUP_HAND_LEFT;
        BodyOrderR[3][12][i] = PUP_ARM_LEFT;
        BodyOrderR[3][13][i] = PUP_HEAD;
        BodyOrderR[3][14][i] = PUP_HAIR;
        BodyOrderR[3][15][i] = PUP_HAT;
        BodyOrderR[3][16][i] = PUP_MASK;
        BodyOrderR[3][17][i] = PUP_SHIELD;
        BodyOrderR[3][18][i] = PUP_WEAPON2;

        // 090	       
        BodyOrderR[4][ 0][i] = PUP_HAND_RIGHT;
        BodyOrderR[4][ 1][i] = PUP_ARM_RIGHT;
        BodyOrderR[4][ 2][i] = PUP_FOOT;
        BodyOrderR[4][ 3][i] = PUP_LEGS;
        BodyOrderR[4][ 4][i] = PUP_BOOT;
        BodyOrderR[4][ 5][i] = PUP_ROBELEGS;
        BodyOrderR[4][ 6][i] = PUP_CAPE_2;
        BodyOrderR[4][ 7][i] = PUP_BODY;
        BodyOrderR[4][ 8][i] = PUP_HEAD;
        BodyOrderR[4][ 9][i] = PUP_HAIR;
        BodyOrderR[4][10][i] = PUP_HAT;
        BodyOrderR[4][11][i] = PUP_MASK;
        BodyOrderR[4][12][i] = PUP_CAPE;
        BodyOrderR[4][13][i] = PUP_WEAPON;
        BodyOrderR[4][14][i] = PUP_HAND_LEFT;
        BodyOrderR[4][15][i] = PUP_ARM_LEFT;
        BodyOrderR[4][16][i] = PUP_BACKBODY;
        BodyOrderR[4][17][i] = PUP_SHIELD;
        BodyOrderR[4][18][i] = PUP_WEAPON2;

        // 270

        BodyOrderR[6][ 0][i] = PUP_HAND_RIGHT;
        BodyOrderR[6][ 1][i] = PUP_ARM_RIGHT;
        BodyOrderR[6][ 2][i] = PUP_FOOT;
        BodyOrderR[6][ 3][i] = PUP_LEGS;
        BodyOrderR[6][ 4][i] = PUP_BOOT;
        BodyOrderR[6][ 5][i] = PUP_ROBELEGS;
        BodyOrderR[6][ 6][i] = PUP_CAPE_2;
        BodyOrderR[6][ 7][i] = PUP_BODY;
        BodyOrderR[6][ 8][i] = PUP_HEAD;
        BodyOrderR[6][ 9][i] = PUP_HAIR;
        BodyOrderR[6][10][i] = PUP_HAT;
        BodyOrderR[6][11][i] = PUP_MASK;
        BodyOrderR[6][12][i] = PUP_BACKBODY;
        BodyOrderR[6][13][i] = PUP_CAPE;
        BodyOrderR[6][14][i] = PUP_WEAPON;
        BodyOrderR[6][15][i] = PUP_HAND_LEFT;
        BodyOrderR[6][16][i] = PUP_ARM_LEFT;
        BodyOrderR[6][17][i] = PUP_SHIELD;
        BodyOrderR[6][18][i] = PUP_WEAPON2;

        // 135

        BodyOrderR[7][ 0][i] = PUP_HAND_RIGHT;
        BodyOrderR[7][ 1][i] = PUP_ARM_RIGHT;
        BodyOrderR[7][ 2][i] = PUP_FOOT;
        BodyOrderR[7][ 3][i] = PUP_LEGS;
        BodyOrderR[7][ 4][i] = PUP_BOOT;
        BodyOrderR[7][ 5][i] = PUP_ROBELEGS;
        BodyOrderR[7][ 6][i] = PUP_CAPE_2;
        BodyOrderR[7][ 7][i] = PUP_BODY;
        BodyOrderR[7][ 8][i] = PUP_MASK;
        BodyOrderR[7][ 9][i] = PUP_HEAD;
        BodyOrderR[7][10][i] = PUP_HAIR;
        BodyOrderR[7][11][i] = PUP_HAT;
        BodyOrderR[7][12][i] = PUP_WEAPON;
        BodyOrderR[7][13][i] = PUP_HAND_LEFT;
        BodyOrderR[7][14][i] = PUP_ARM_LEFT;           
        BodyOrderR[7][15][i] = PUP_BACKBODY;
        BodyOrderR[7][16][i] = PUP_CAPE;
        BodyOrderR[7][17][i] = PUP_SHIELD;
        BodyOrderR[7][18][i] = PUP_WEAPON2;

        // 225			  	       
        BodyOrderR[9][ 0][i] = PUP_HAND_RIGHT;
        BodyOrderR[9][ 1][i] = PUP_ARM_RIGHT;
        BodyOrderR[9][ 2][i] = PUP_FOOT;
        BodyOrderR[9][ 3][i] = PUP_LEGS;
        BodyOrderR[9][ 4][i] = PUP_BOOT;
        BodyOrderR[9][ 5][i] = PUP_ROBELEGS;
        BodyOrderR[9][ 6][i] = PUP_CAPE_2;
        BodyOrderR[9][ 7][i] = PUP_BODY;
        BodyOrderR[9][ 8][i] = PUP_MASK;
        BodyOrderR[9][ 9][i] = PUP_HEAD;
        BodyOrderR[9][10][i] = PUP_HAIR;
        BodyOrderR[9][11][i] = PUP_HAT;
        BodyOrderR[9][12][i] = PUP_WEAPON;
        BodyOrderR[9][13][i] = PUP_HAND_LEFT;
        BodyOrderR[9][14][i] = PUP_ARM_LEFT;
        BodyOrderR[9][15][i] = PUP_BACKBODY;
        BodyOrderR[9][16][i] = PUP_CAPE;
        BodyOrderR[9][17][i] = PUP_SHIELD;
        BodyOrderR[9][18][i] = PUP_WEAPON2;
    }

    // ATTACK
    for (int i = 0; i < 9; i++) {
        // 045
        BodyOrderAR[1][ 0][i] = PUP_BACKBODY;        
        BodyOrderAR[1][ 1][i] = PUP_HAND_RIGHT;
        BodyOrderAR[1][ 2][i] = PUP_ARM_RIGHT;
        BodyOrderAR[1][ 3][i] = PUP_FOOT;
        BodyOrderAR[1][ 4][i] = PUP_LEGS;
        BodyOrderAR[1][ 5][i] = PUP_BOOT;
        BodyOrderAR[1][ 6][i] = PUP_ROBELEGS;
        BodyOrderAR[1][ 7][i] = PUP_BODY;
        BodyOrderAR[1][ 8][i] = PUP_CAPE;
        BodyOrderAR[1][ 9][i] = PUP_CAPE_2;
        BodyOrderAR[1][10][i] = PUP_WEAPON;
        BodyOrderAR[1][11][i] = PUP_HAND_LEFT;
        BodyOrderAR[1][12][i] = PUP_ARM_LEFT;
        BodyOrderAR[1][13][i] = PUP_HEAD;
        BodyOrderAR[1][14][i] = PUP_HAIR;
        BodyOrderAR[1][15][i] = PUP_HAT;
        BodyOrderAR[1][16][i] = PUP_MASK;
        BodyOrderAR[1][17][i] = PUP_SHIELD;
        BodyOrderAR[1][18][i] = PUP_WEAPON2;

        // 315			  
        BodyOrderAR[3][ 0][i] = PUP_BACKBODY;        
        BodyOrderAR[3][ 1][i] = PUP_HAND_RIGHT;
        BodyOrderAR[3][ 2][i] = PUP_ARM_RIGHT;
        BodyOrderAR[3][ 3][i] = PUP_FOOT;
        BodyOrderAR[3][ 4][i] = PUP_LEGS;
        BodyOrderAR[3][ 5][i] = PUP_BOOT;
        BodyOrderAR[3][ 6][i] = PUP_ROBELEGS;
        BodyOrderAR[3][ 7][i] = PUP_BODY;
        BodyOrderAR[3][ 8][i] = PUP_CAPE;
        BodyOrderAR[3][ 9][i] = PUP_CAPE_2;
        BodyOrderAR[3][10][i] = PUP_WEAPON;
        BodyOrderAR[3][11][i] = PUP_HAND_LEFT;
        BodyOrderAR[3][12][i] = PUP_ARM_LEFT;
        BodyOrderAR[3][13][i] = PUP_HEAD;
        BodyOrderAR[3][14][i] = PUP_HAIR;
        BodyOrderAR[3][15][i] = PUP_HAT;
        BodyOrderAR[3][16][i] = PUP_MASK;
        BodyOrderAR[3][17][i] = PUP_SHIELD;
        BodyOrderAR[3][18][i] = PUP_WEAPON2;

    }

    for (int i = 0; i < 2; i++) {
        // 090        
        BodyOrderAR[4][ 0][i] = PUP_HAND_RIGHT;
        BodyOrderAR[4][ 1][i] = PUP_ARM_RIGHT;
        BodyOrderAR[4][ 2][i] = PUP_FOOT;
        BodyOrderAR[4][ 3][i] = PUP_LEGS;
        BodyOrderAR[4][ 4][i] = PUP_BOOT;
        BodyOrderAR[4][ 5][i] = PUP_ROBELEGS;
        BodyOrderAR[4][ 6][i] = PUP_CAPE_2;
        BodyOrderAR[4][ 7][i] = PUP_BODY;
        BodyOrderAR[4][ 8][i] = PUP_HEAD;
        BodyOrderAR[4][ 9][i] = PUP_HAIR;
        BodyOrderAR[4][10][i] = PUP_HAT;
        BodyOrderAR[4][11][i] = PUP_MASK;
        BodyOrderAR[4][12][i] = PUP_WEAPON;
        BodyOrderAR[4][13][i] = PUP_HAND_LEFT;
        BodyOrderAR[4][14][i] = PUP_ARM_LEFT;
        BodyOrderAR[4][15][i] = PUP_BACKBODY;
        BodyOrderAR[4][16][i] = PUP_CAPE;
        BodyOrderAR[4][17][i] = PUP_SHIELD;
        BodyOrderAR[4][18][i] = PUP_WEAPON2;
   			        
        // 270
        BodyOrderAR[6][ 0][i] = PUP_HAND_RIGHT;
        BodyOrderAR[6][ 1][i] = PUP_ARM_RIGHT;
        BodyOrderAR[6][ 2][i] = PUP_FOOT;
        BodyOrderAR[6][ 3][i] = PUP_LEGS;
        BodyOrderAR[6][ 4][i] = PUP_BOOT;
        BodyOrderAR[6][ 5][i] = PUP_ROBELEGS;
        BodyOrderAR[6][ 6][i] = PUP_CAPE_2;
        BodyOrderAR[6][ 7][i] = PUP_BODY;
        BodyOrderAR[6][ 8][i] = PUP_HEAD;
        BodyOrderAR[6][ 9][i] = PUP_HAIR;
        BodyOrderAR[6][10][i] = PUP_HAT;
        BodyOrderAR[6][11][i] = PUP_MASK;
        BodyOrderAR[6][12][i] = PUP_WEAPON;
        BodyOrderAR[6][13][i] = PUP_HAND_LEFT;
        BodyOrderAR[6][14][i] = PUP_ARM_LEFT;
        BodyOrderAR[6][15][i] = PUP_BACKBODY;
        BodyOrderAR[6][16][i] = PUP_CAPE;
        BodyOrderAR[6][17][i] = PUP_SHIELD;
        BodyOrderAR[6][18][i] = PUP_WEAPON2;
    }

    for (int i = 2; i < 8; i++) {
        // 090        
        BodyOrderAR[4][ 0][i] = PUP_HAND_RIGHT;
        BodyOrderAR[4][ 1][i] = PUP_ARM_RIGHT;
        BodyOrderAR[4][ 2][i] = PUP_FOOT;
        BodyOrderAR[4][ 3][i] = PUP_LEGS;
        BodyOrderAR[4][ 4][i] = PUP_BOOT;
        BodyOrderAR[4][ 5][i] = PUP_ROBELEGS;
        BodyOrderAR[4][ 6][i] = PUP_WEAPON;
        BodyOrderAR[4][ 7][i] = PUP_CAPE_2;
        BodyOrderAR[4][ 8][i] = PUP_BODY;
        BodyOrderAR[4][ 9][i] = PUP_HEAD;
        BodyOrderAR[4][10][i] = PUP_HAIR;
        BodyOrderAR[4][11][i] = PUP_HAT;
        BodyOrderAR[4][12][i] = PUP_MASK;
        BodyOrderAR[4][13][i] = PUP_HAND_LEFT;
        BodyOrderAR[4][14][i] = PUP_ARM_LEFT;
        BodyOrderAR[4][15][i] = PUP_BACKBODY;
        BodyOrderAR[4][16][i] = PUP_CAPE;
        BodyOrderAR[4][17][i] = PUP_SHIELD;
        BodyOrderAR[4][18][i] = PUP_WEAPON2;
   			        
        // 270
        BodyOrderAR[6][ 0][i] = PUP_HAND_RIGHT;
        BodyOrderAR[6][ 1][i] = PUP_ARM_RIGHT;
        BodyOrderAR[6][ 2][i] = PUP_FOOT;
        BodyOrderAR[6][ 3][i] = PUP_LEGS;
        BodyOrderAR[6][ 4][i] = PUP_BOOT;
        BodyOrderAR[6][ 5][i] = PUP_ROBELEGS;
        BodyOrderAR[6][ 6][i] = PUP_WEAPON;
        BodyOrderAR[6][ 7][i] = PUP_CAPE_2;
        BodyOrderAR[6][ 8][i] = PUP_BODY;
        BodyOrderAR[6][ 9][i] = PUP_HEAD;
        BodyOrderAR[6][10][i] = PUP_HAIR;
        BodyOrderAR[6][11][i] = PUP_HAT;
        BodyOrderAR[6][12][i] = PUP_MASK;
        BodyOrderAR[6][13][i] = PUP_HAND_LEFT;
        BodyOrderAR[6][14][i] = PUP_ARM_LEFT;
        BodyOrderAR[6][15][i] = PUP_BACKBODY;
        BodyOrderAR[6][16][i] = PUP_CAPE;
        BodyOrderAR[6][17][i] = PUP_SHIELD;
        BodyOrderAR[6][18][i] = PUP_WEAPON2;
    }

    for (int i = 8; i < 9; i++) {
        // 090        
        BodyOrderAR[4][ 0][i] = PUP_HAND_RIGHT;
        BodyOrderAR[4][ 1][i] = PUP_ARM_RIGHT;
        BodyOrderAR[4][ 2][i] = PUP_FOOT;
        BodyOrderAR[4][ 3][i] = PUP_LEGS;
        BodyOrderAR[4][ 4][i] = PUP_BOOT;
        BodyOrderAR[4][ 5][i] = PUP_ROBELEGS;
        BodyOrderAR[4][ 6][i] = PUP_CAPE_2;
        BodyOrderAR[4][ 7][i] = PUP_BODY;
        BodyOrderAR[4][ 8][i] = PUP_HEAD;
        BodyOrderAR[4][ 9][i] = PUP_HAIR;
        BodyOrderAR[4][10][i] = PUP_HAT;
        BodyOrderAR[4][11][i] = PUP_MASK;
        BodyOrderAR[4][12][i] = PUP_WEAPON;
        BodyOrderAR[4][13][i] = PUP_HAND_LEFT;
        BodyOrderAR[4][14][i] = PUP_ARM_LEFT;
        BodyOrderAR[4][15][i] = PUP_BACKBODY;
        BodyOrderAR[4][16][i] = PUP_CAPE;
        BodyOrderAR[4][17][i] = PUP_SHIELD;
        BodyOrderAR[4][18][i] = PUP_WEAPON2;
   			        
        // 270
        BodyOrderAR[6][ 0][i] = PUP_HAND_RIGHT;
        BodyOrderAR[6][ 1][i] = PUP_ARM_RIGHT;
        BodyOrderAR[6][ 2][i] = PUP_FOOT;
        BodyOrderAR[6][ 3][i] = PUP_LEGS;
        BodyOrderAR[6][ 4][i] = PUP_BOOT;
        BodyOrderAR[6][ 5][i] = PUP_ROBELEGS;
        BodyOrderAR[6][ 6][i] = PUP_CAPE_2;
        BodyOrderAR[6][ 7][i] = PUP_BODY;
        BodyOrderAR[6][ 8][i] = PUP_HEAD;
        BodyOrderAR[6][ 9][i] = PUP_HAIR;
        BodyOrderAR[6][10][i] = PUP_HAT;
        BodyOrderAR[6][11][i] = PUP_MASK;
        BodyOrderAR[6][12][i] = PUP_WEAPON;
        BodyOrderAR[6][13][i] = PUP_HAND_LEFT;
        BodyOrderAR[6][14][i] = PUP_ARM_LEFT;
        BodyOrderAR[6][15][i] = PUP_BACKBODY;
        BodyOrderAR[6][16][i] = PUP_CAPE;
        BodyOrderAR[6][17][i] = PUP_SHIELD;
        BodyOrderAR[6][18][i] = PUP_WEAPON2;
    }

    // 180
    for (int i = 0; i < 2; i++) {

        BodyOrderAR[8][ 0][i] = PUP_WEAPON2;
        BodyOrderAR[8][ 1][i] = PUP_SHIELD;
        BodyOrderAR[8][ 2][i] = PUP_WEAPON;
        BodyOrderAR[8][ 3][i] = PUP_HAND_RIGHT;
        BodyOrderAR[8][ 4][i] = PUP_HAND_LEFT;
        BodyOrderAR[8][ 5][i] = PUP_FOOT;
        BodyOrderAR[8][ 6][i] = PUP_LEGS;
        BodyOrderAR[8][ 7][i] = PUP_BOOT;
        BodyOrderAR[8][ 8][i] = PUP_ROBELEGS;
        BodyOrderAR[8][ 9][i] = PUP_ARM_RIGHT;
        BodyOrderAR[8][10][i] = PUP_ARM_LEFT;
        BodyOrderAR[8][11][i] = PUP_CAPE_2;
        BodyOrderAR[8][12][i] = PUP_BODY;
        BodyOrderAR[8][13][i] = PUP_MASK;
        BodyOrderAR[8][14][i] = PUP_HEAD;
        BodyOrderAR[8][15][i] = PUP_HAIR;
        BodyOrderAR[8][16][i] = PUP_HAT;
        BodyOrderAR[8][17][i] = PUP_BACKBODY;
        BodyOrderAR[8][18][i] = PUP_CAPE;
   }

    // 180
    for (int i = 2; i < 5; i++) {
        BodyOrderAR[8][ 0][i] = PUP_WEAPON2;
        BodyOrderAR[8][ 1][i] = PUP_SHIELD;
        BodyOrderAR[8][ 2][i] = PUP_HAND_LEFT;
        BodyOrderAR[8][ 3][i] = PUP_FOOT;
        BodyOrderAR[8][ 4][i] = PUP_LEGS;
        BodyOrderAR[8][ 5][i] = PUP_BOOT;
        BodyOrderAR[8][ 6][i] = PUP_ROBELEGS;
        BodyOrderAR[8][ 7][i] = PUP_ARM_RIGHT;
        BodyOrderAR[8][ 8][i] = PUP_ARM_LEFT;
        BodyOrderAR[8][ 9][i] = PUP_CAPE_2;
        BodyOrderAR[8][10][i] = PUP_BODY;
        BodyOrderAR[8][11][i] = PUP_MASK;
        BodyOrderAR[8][12][i] = PUP_HEAD;
        BodyOrderAR[8][13][i] = PUP_HAIR;
        BodyOrderAR[8][14][i] = PUP_HAT;
        BodyOrderAR[8][15][i] = PUP_BACKBODY;
        BodyOrderAR[8][16][i] = PUP_CAPE;
        BodyOrderAR[8][17][i] = PUP_WEAPON;
        BodyOrderAR[8][18][i] = PUP_HAND_RIGHT;
    }

    // 180
    for (int i = 5; i < 9; i++) {
        BodyOrderAR[8][ 0][i] = PUP_WEAPON2;
        BodyOrderAR[8][ 1][i] = PUP_SHIELD;
        BodyOrderAR[8][ 2][i] = PUP_WEAPON;
        BodyOrderAR[8][ 3][i] = PUP_HAND_RIGHT;
        BodyOrderAR[8][ 4][i] = PUP_HAND_LEFT;
        BodyOrderAR[8][ 5][i] = PUP_FOOT;
        BodyOrderAR[8][ 6][i] = PUP_LEGS;
        BodyOrderAR[8][ 7][i] = PUP_BOOT;
        BodyOrderAR[8][ 8][i] = PUP_ROBELEGS;
        BodyOrderAR[8][ 9][i] = PUP_ARM_RIGHT;
        BodyOrderAR[8][10][i] = PUP_ARM_LEFT;
        BodyOrderAR[8][11][i] = PUP_CAPE_2;
        BodyOrderAR[8][12][i] = PUP_BODY;
        BodyOrderAR[8][13][i] = PUP_MASK;
        BodyOrderAR[8][14][i] = PUP_HEAD;
        BodyOrderAR[8][15][i] = PUP_HAIR;
        BodyOrderAR[8][16][i] = PUP_HAT;
        BodyOrderAR[8][17][i] = PUP_BACKBODY;
        BodyOrderAR[8][18][i] = PUP_CAPE;
    }

    // 135
    for (int i = 0; i < 2; i++) {
        BodyOrderAR[7][ 0][i] = PUP_WEAPON2;
        BodyOrderAR[7][ 1][i] = PUP_SHIELD;
        BodyOrderAR[7][ 2][i] = PUP_WEAPON;
        BodyOrderAR[7][ 3][i] = PUP_HAND_RIGHT;
        BodyOrderAR[7][ 4][i] = PUP_ARM_RIGHT;
        BodyOrderAR[7][ 5][i] = PUP_FOOT;
        BodyOrderAR[7][ 6][i] = PUP_LEGS;
        BodyOrderAR[7][ 7][i] = PUP_BOOT;
        BodyOrderAR[7][ 8][i] = PUP_ROBELEGS;
        BodyOrderAR[7][ 9][i] = PUP_CAPE_2;
        BodyOrderAR[7][10][i] = PUP_BODY;
        BodyOrderAR[7][11][i] = PUP_MASK;
        BodyOrderAR[7][12][i] = PUP_HEAD;
        BodyOrderAR[7][13][i] = PUP_HAIR;
        BodyOrderAR[7][14][i] = PUP_HAT;
        BodyOrderAR[7][15][i] = PUP_HAND_LEFT;
        BodyOrderAR[7][16][i] = PUP_ARM_LEFT;
        BodyOrderAR[7][17][i] = PUP_BACKBODY;
        BodyOrderAR[7][18][i] = PUP_CAPE;
    }

    // 135
    for (int i = 2; i < 9; i++) {
        BodyOrderAR[7][ 0][i] = PUP_WEAPON;
        BodyOrderAR[7][ 1][i] = PUP_HAND_RIGHT;
        BodyOrderAR[7][ 2][i] = PUP_ARM_RIGHT;
        BodyOrderAR[7][ 3][i] = PUP_FOOT;
        BodyOrderAR[7][ 4][i] = PUP_LEGS;
        BodyOrderAR[7][ 5][i] = PUP_BOOT;
        BodyOrderAR[7][ 6][i] = PUP_ROBELEGS;
        BodyOrderAR[7][ 7][i] = PUP_CAPE_2;
        BodyOrderAR[7][ 8][i] = PUP_HAND_LEFT;
        BodyOrderAR[7][ 9][i] = PUP_ARM_LEFT;
        BodyOrderAR[7][10][i] = PUP_BODY;
        BodyOrderAR[7][11][i] = PUP_MASK;
        BodyOrderAR[7][12][i] = PUP_HEAD;
        BodyOrderAR[7][13][i] = PUP_HAIR;
        BodyOrderAR[7][14][i] = PUP_HAT;
        BodyOrderAR[7][15][i] = PUP_BACKBODY;
        BodyOrderAR[7][16][i] = PUP_CAPE;
        BodyOrderAR[7][17][i] = PUP_SHIELD;
        BodyOrderAR[7][18][i] = PUP_WEAPON2;
    }

    // 225
    for (int i = 0; i < 2; i++) {
        BodyOrderAR[9][ 0][i] = PUP_WEAPON2;
        BodyOrderAR[9][ 1][i] = PUP_SHIELD;
        BodyOrderAR[9][ 2][i] = PUP_WEAPON;
        BodyOrderAR[9][ 3][i] = PUP_HAND_RIGHT;
        BodyOrderAR[9][ 4][i] = PUP_ARM_RIGHT;
        BodyOrderAR[9][ 5][i] = PUP_FOOT;
        BodyOrderAR[9][ 6][i] = PUP_LEGS;
        BodyOrderAR[9][ 7][i] = PUP_BOOT;
        BodyOrderAR[9][ 8][i] = PUP_ROBELEGS;
        BodyOrderAR[9][ 9][i] = PUP_CAPE_2;
        BodyOrderAR[9][10][i] = PUP_BODY;
        BodyOrderAR[9][11][i] = PUP_MASK;
        BodyOrderAR[9][12][i] = PUP_HEAD;
        BodyOrderAR[9][13][i] = PUP_HAIR;
        BodyOrderAR[9][14][i] = PUP_HAT;
        BodyOrderAR[9][15][i] = PUP_HAND_LEFT;
        BodyOrderAR[9][16][i] = PUP_ARM_LEFT;
        BodyOrderAR[9][17][i] = PUP_BACKBODY;
        BodyOrderAR[9][18][i] = PUP_CAPE;
    }

    // 225		
    for (int i = 2; i < 9; i++) {
        BodyOrderAR[9][ 0][i] = PUP_WEAPON;
        BodyOrderAR[9][ 1][i] = PUP_HAND_RIGHT;
        BodyOrderAR[9][ 2][i] = PUP_ARM_RIGHT;
        BodyOrderAR[9][ 3][i] = PUP_FOOT;
        BodyOrderAR[9][ 4][i] = PUP_LEGS;
        BodyOrderAR[9][ 5][i] = PUP_BOOT;
        BodyOrderAR[9][ 6][i] = PUP_ROBELEGS;
        BodyOrderAR[9][ 7][i] = PUP_CAPE_2;
        BodyOrderAR[9][ 8][i] = PUP_HAND_LEFT;
        BodyOrderAR[9][ 9][i] = PUP_ARM_LEFT;
        BodyOrderAR[9][10][i] = PUP_BODY;
        BodyOrderAR[9][11][i] = PUP_MASK;
        BodyOrderAR[9][12][i] = PUP_HEAD;
        BodyOrderAR[9][13][i] = PUP_HAIR;
        BodyOrderAR[9][14][i] = PUP_HAT;
        BodyOrderAR[9][15][i] = PUP_BACKBODY;
        BodyOrderAR[9][16][i] = PUP_CAPE;
        BodyOrderAR[9][17][i] = PUP_SHIELD;
        BodyOrderAR[9][18][i] = PUP_WEAPON2;
        }
						          
    // 000
    for (int i = 0; i < 5; i++) {
        BodyOrderAR[2][ 0][i] = PUP_CAPE;
        BodyOrderAR[2][ 1][i] = PUP_BACKBODY;
        BodyOrderAR[2][ 2][i] = PUP_FOOT;
        BodyOrderAR[2][ 3][i] = PUP_LEGS;
        BodyOrderAR[2][ 4][i] = PUP_BOOT;
        BodyOrderAR[2][ 5][i] = PUP_ROBELEGS;
        BodyOrderAR[2][ 6][i] = PUP_BODY;
        BodyOrderAR[2][ 7][i] = PUP_CAPE_2;
        BodyOrderAR[2][ 8][i] = PUP_HEAD;
        BodyOrderAR[2][ 9][i] = PUP_HAIR;
        BodyOrderAR[2][10][i] = PUP_HAT;
        BodyOrderAR[2][11][i] = PUP_MASK;
        BodyOrderAR[2][12][i] = PUP_SHIELD;
        BodyOrderAR[2][13][i] = PUP_WEAPON2;
        BodyOrderAR[2][14][i] = PUP_HAND_RIGHT;
        BodyOrderAR[2][15][i] = PUP_ARM_RIGHT;
        BodyOrderAR[2][16][i] = PUP_HAND_LEFT;
        BodyOrderAR[2][17][i] = PUP_WEAPON;
        BodyOrderAR[2][18][i] = PUP_ARM_LEFT;
        }

    // 000
    for (int i = 5; i < 9; i++) {
        BodyOrderAR[2][ 0][i] = PUP_WEAPON2;
        BodyOrderAR[2][ 1][i] = PUP_SHIELD;
        BodyOrderAR[2][ 2][i] = PUP_CAPE;
        BodyOrderAR[2][ 3][i] = PUP_BACKBODY;
        BodyOrderAR[2][ 4][i] = PUP_FOOT;
        BodyOrderAR[2][ 5][i] = PUP_LEGS;
        BodyOrderAR[2][ 6][i] = PUP_BOOT;
        BodyOrderAR[2][ 7][i] = PUP_ROBELEGS;
        BodyOrderAR[2][ 8][i] = PUP_BODY;
        BodyOrderAR[2][ 9][i] = PUP_CAPE_2;
        BodyOrderAR[2][10][i] = PUP_HEAD;
        BodyOrderAR[2][11][i] = PUP_HAIR;
        BodyOrderAR[2][12][i] = PUP_HAT;
        BodyOrderAR[2][13][i] = PUP_MASK;
        BodyOrderAR[2][14][i] = PUP_HAND_RIGHT;
        BodyOrderAR[2][15][i] = PUP_ARM_RIGHT;
        BodyOrderAR[2][16][i] = PUP_HAND_LEFT;
        BodyOrderAR[2][17][i] = PUP_WEAPON;
        BodyOrderAR[2][18][i] = PUP_ARM_LEFT;
    }

    // Non-ranged weapons!
    LOG << "\r\nNORMAL WEAPON PUPPET!";

    // WALKING
    for (int i = 0; i < 13; i++) { 
        // 045
        BodyOrder[1][ 0][i] = PUP_WEAPON;
        BodyOrder[1][ 1][i] = PUP_CAPE;
        BodyOrder[1][ 2][i] = PUP_BACKBODY;
        BodyOrder[1][ 3][i] = PUP_HAND_RIGHT;
        BodyOrder[1][ 4][i] = PUP_ARM_RIGHT;
        BodyOrder[1][ 5][i] = PUP_FOOT;
        BodyOrder[1][ 6][i] = PUP_LEGS;
        BodyOrder[1][ 7][i] = PUP_BOOT;
        BodyOrder[1][ 8][i] = PUP_ROBELEGS;
        BodyOrder[1][ 9][i] = PUP_BODY;
        BodyOrder[1][10][i] = PUP_CAPE_2;
        BodyOrder[1][11][i] = PUP_HAND_LEFT;
        BodyOrder[1][12][i] = PUP_ARM_LEFT;
        BodyOrder[1][13][i] = PUP_HEAD;
        BodyOrder[1][14][i] = PUP_HAIR;
        BodyOrder[1][15][i] = PUP_HAT;
        BodyOrder[1][16][i] = PUP_MASK;
        BodyOrder[1][17][i] = PUP_SHIELD;
        BodyOrder[1][18][i] = PUP_WEAPON2;

        // 180
        BodyOrder[8][ 0][i] = PUP_WEAPON;
        BodyOrder[8][ 1][i] = PUP_WEAPON2;
        BodyOrder[8][ 2][i] = PUP_HAND_RIGHT;
        BodyOrder[8][ 3][i] = PUP_HAND_LEFT;
        BodyOrder[8][ 4][i] = PUP_FOOT;
        BodyOrder[8][ 5][i] = PUP_LEGS;
        BodyOrder[8][ 6][i] = PUP_BOOT;
        BodyOrder[8][ 7][i] = PUP_ROBELEGS;
        BodyOrder[8][ 8][i] = PUP_ARM_RIGHT;
        BodyOrder[8][ 9][i] = PUP_ARM_LEFT;
        BodyOrder[8][10][i] = PUP_SHIELD;
        BodyOrder[8][11][i] = PUP_CAPE_2;
        BodyOrder[8][12][i] = PUP_BODY;
        BodyOrder[8][13][i] = PUP_MASK;
        BodyOrder[8][14][i] = PUP_HEAD;
        BodyOrder[8][15][i] = PUP_HAIR;
        BodyOrder[8][16][i] = PUP_HAT;
        BodyOrder[8][17][i] = PUP_BACKBODY;
        BodyOrder[8][18][i] = PUP_CAPE;

        // 000
        BodyOrder[2][ 0][i] = PUP_CAPE;
        BodyOrder[2][ 1][i] = PUP_BACKBODY;
        BodyOrder[2][ 2][i] = PUP_FOOT;
        BodyOrder[2][ 3][i] = PUP_LEGS;
        BodyOrder[2][ 4][i] = PUP_BOOT;
        BodyOrder[2][ 5][i] = PUP_ROBELEGS;
        BodyOrder[2][ 6][i] = PUP_BODY;
        BodyOrder[2][ 7][i] = PUP_CAPE_2;
        BodyOrder[2][ 8][i] = PUP_HAND_RIGHT;
        BodyOrder[2][ 9][i] = PUP_HAND_LEFT;
        BodyOrder[2][10][i] = PUP_ARM_RIGHT;
        BodyOrder[2][11][i] = PUP_ARM_LEFT;
        BodyOrder[2][12][i] = PUP_HEAD;
        BodyOrder[2][13][i] = PUP_HAIR;
        BodyOrder[2][14][i] = PUP_HAT;
        BodyOrder[2][15][i] = PUP_MASK;
        BodyOrder[2][16][i] = PUP_WEAPON;
        BodyOrder[2][17][i] = PUP_SHIELD;
        BodyOrder[2][18][i] = PUP_WEAPON2;

        // 315
        BodyOrder[3][ 0][i] = PUP_WEAPON;
        BodyOrder[3][ 1][i] = PUP_CAPE;
        BodyOrder[3][ 2][i] = PUP_BACKBODY;
        BodyOrder[3][ 3][i] = PUP_HAND_RIGHT;
        BodyOrder[3][ 4][i] = PUP_ARM_RIGHT;
        BodyOrder[3][ 5][i] = PUP_FOOT;
        BodyOrder[3][ 6][i] = PUP_LEGS;
        BodyOrder[3][ 7][i] = PUP_BOOT;
        BodyOrder[3][ 8][i] = PUP_ROBELEGS;
        BodyOrder[3][ 9][i] = PUP_BODY;
        BodyOrder[3][10][i] = PUP_CAPE_2;
        BodyOrder[3][11][i] = PUP_HAND_LEFT;
        BodyOrder[3][12][i] = PUP_ARM_LEFT;
        BodyOrder[3][13][i] = PUP_HEAD;
        BodyOrder[3][14][i] = PUP_HAIR;
        BodyOrder[3][15][i] = PUP_HAT;
        BodyOrder[3][16][i] = PUP_MASK;
        BodyOrder[3][17][i] = PUP_SHIELD;
        BodyOrder[3][18][i] = PUP_WEAPON2;

        // 090
        BodyOrder[4][ 0][i] = PUP_WEAPON;
        BodyOrder[4][ 1][i] = PUP_HAND_RIGHT;
        BodyOrder[4][ 2][i] = PUP_ARM_RIGHT;
        BodyOrder[4][ 3][i] = PUP_FOOT;
        BodyOrder[4][ 4][i] = PUP_LEGS;
        BodyOrder[4][ 5][i] = PUP_BOOT;
        BodyOrder[4][ 6][i] = PUP_ROBELEGS;
        BodyOrder[4][ 7][i] = PUP_CAPE_2;
        BodyOrder[4][ 8][i] = PUP_BODY;
        BodyOrder[4][ 9][i] = PUP_HEAD;
        BodyOrder[4][10][i] = PUP_HAIR;
        BodyOrder[4][11][i] = PUP_HAT;
        BodyOrder[4][12][i] = PUP_MASK;
        BodyOrder[4][13][i] = PUP_CAPE;
        BodyOrder[4][14][i] = PUP_HAND_LEFT;
        BodyOrder[4][15][i] = PUP_ARM_LEFT;
        BodyOrder[4][16][i] = PUP_BACKBODY;
        BodyOrder[4][17][i] = PUP_SHIELD;
        BodyOrder[4][18][i] = PUP_WEAPON2;

        // 270
        BodyOrder[6][ 0][i] = PUP_WEAPON;
        BodyOrder[6][ 1][i] = PUP_HAND_RIGHT;
        BodyOrder[6][ 2][i] = PUP_ARM_RIGHT;
        BodyOrder[6][ 3][i] = PUP_FOOT;
        BodyOrder[6][ 4][i] = PUP_LEGS;
        BodyOrder[6][ 5][i] = PUP_BOOT;
        BodyOrder[6][ 6][i] = PUP_ROBELEGS;
        BodyOrder[6][ 7][i] = PUP_CAPE_2;
        BodyOrder[6][ 8][i] = PUP_BODY;
        BodyOrder[6][ 9][i] = PUP_HEAD;
        BodyOrder[6][10][i] = PUP_HAIR;
        BodyOrder[6][11][i] = PUP_HAT;
        BodyOrder[6][12][i] = PUP_MASK;
        BodyOrder[6][13][i] = PUP_BACKBODY;
        BodyOrder[6][14][i] = PUP_CAPE;
        BodyOrder[6][15][i] = PUP_HAND_LEFT;
        BodyOrder[6][16][i] = PUP_ARM_LEFT;
        BodyOrder[6][17][i] = PUP_SHIELD;
        BodyOrder[6][18][i] = PUP_WEAPON2;

        // 135
        BodyOrder[7][ 0][i] = PUP_WEAPON;
        BodyOrder[7][ 1][i] = PUP_HAND_RIGHT;
        BodyOrder[7][ 2][i] = PUP_ARM_RIGHT;
        BodyOrder[7][ 3][i] = PUP_FOOT;
        BodyOrder[7][ 4][i] = PUP_LEGS;
        BodyOrder[7][ 5][i] = PUP_BOOT;
        BodyOrder[7][ 6][i] = PUP_ROBELEGS;
        BodyOrder[7][ 7][i] = PUP_CAPE_2;
        BodyOrder[7][ 8][i] = PUP_BODY;
        BodyOrder[7][ 9][i] = PUP_MASK;
        BodyOrder[7][10][i] = PUP_HEAD;
        BodyOrder[7][11][i] = PUP_HAIR;
        BodyOrder[7][12][i] = PUP_HAT;
        BodyOrder[7][13][i] = PUP_HAND_LEFT;
        BodyOrder[7][14][i] = PUP_ARM_LEFT;
        BodyOrder[7][15][i] = PUP_BACKBODY;
        BodyOrder[7][16][i] = PUP_CAPE;
        BodyOrder[7][17][i] = PUP_SHIELD;
        BodyOrder[7][18][i] = PUP_WEAPON2;

        // 225			  
        BodyOrder[9][ 0][i] = PUP_WEAPON;
        BodyOrder[9][ 1][i] = PUP_HAND_RIGHT;
        BodyOrder[9][ 2][i] = PUP_ARM_RIGHT;
        BodyOrder[9][ 3][i] = PUP_FOOT;
        BodyOrder[9][ 4][i] = PUP_LEGS;
        BodyOrder[9][ 5][i] = PUP_BOOT;
        BodyOrder[9][ 6][i] = PUP_ROBELEGS;
        BodyOrder[9][ 7][i] = PUP_CAPE_2;
        BodyOrder[9][ 8][i] = PUP_BODY;
        BodyOrder[9][ 9][i] = PUP_MASK;
        BodyOrder[9][10][i] = PUP_HEAD;
        BodyOrder[9][11][i] = PUP_HAIR;
        BodyOrder[9][12][i] = PUP_HAT;
        BodyOrder[9][13][i] = PUP_HAND_LEFT;
        BodyOrder[9][14][i] = PUP_ARM_LEFT;
        BodyOrder[9][15][i] = PUP_BACKBODY;
        BodyOrder[9][16][i] = PUP_CAPE;
        BodyOrder[9][17][i] = PUP_SHIELD; 
        BodyOrder[9][18][i] = PUP_WEAPON2;
    }

    // ATTACK
    for (int i = 0; i < 9; i++) 
    {
        // 090
        BodyOrderA[4][ 0][i] = PUP_WEAPON;
        BodyOrderA[4][ 1][i] = PUP_HAND_RIGHT;
        BodyOrderA[4][ 2][i] = PUP_ARM_RIGHT;
        BodyOrderA[4][ 3][i] = PUP_FOOT;
        BodyOrderA[4][ 4][i] = PUP_LEGS;
        BodyOrderA[4][ 5][i] = PUP_BOOT;
        BodyOrderA[4][ 6][i] = PUP_ROBELEGS;
        BodyOrderA[4][ 7][i] = PUP_CAPE_2;
        BodyOrderA[4][ 8][i] = PUP_BODY;
        BodyOrderA[4][ 9][i] = PUP_HEAD;
        BodyOrderA[4][10][i] = PUP_HAIR;
        BodyOrderA[4][11][i] = PUP_HAT;
        BodyOrderA[4][12][i] = PUP_MASK;
        BodyOrderA[4][13][i] = PUP_HAND_LEFT;
        BodyOrderA[4][14][i] = PUP_ARM_LEFT;
        BodyOrderA[4][15][i] = PUP_BACKBODY;
        BodyOrderA[4][16][i] = PUP_SHIELD;
        BodyOrderA[4][17][i] = PUP_WEAPON2;
        BodyOrderA[4][18][i] = PUP_CAPE;
        
   			        
        // 270
        BodyOrderA[6][ 0][i] = PUP_WEAPON;
        BodyOrderA[6][ 1][i] = PUP_HAND_RIGHT;
        BodyOrderA[6][ 2][i] = PUP_ARM_RIGHT;
        BodyOrderA[6][ 3][i] = PUP_FOOT;
        BodyOrderA[6][ 4][i] = PUP_LEGS;
        BodyOrderA[6][ 5][i] = PUP_BOOT;
        BodyOrderA[6][ 6][i] = PUP_ROBELEGS;
        BodyOrderA[6][ 7][i] = PUP_CAPE_2;
        BodyOrderA[6][ 8][i] = PUP_BODY;
        BodyOrderA[6][ 9][i] = PUP_HEAD;
        BodyOrderA[6][10][i] = PUP_HAIR;
        BodyOrderA[6][11][i] = PUP_HAT;
        BodyOrderA[6][12][i] = PUP_MASK;
        BodyOrderA[6][13][i] = PUP_HAND_LEFT;
        BodyOrderA[6][14][i] = PUP_ARM_LEFT;
        BodyOrderA[6][15][i] = PUP_BACKBODY;
        BodyOrderA[6][16][i] = PUP_SHIELD;
        BodyOrderA[6][17][i] = PUP_WEAPON2;
        BodyOrderA[6][18][i] = PUP_CAPE;
        
    }

    // 180
    for (int i = 0; i < 2; i++) {
        BodyOrderA[8][ 0][i] = PUP_WEAPON2;
        BodyOrderA[8][ 1][i] = PUP_SHIELD;
        BodyOrderA[8][ 2][i] = PUP_WEAPON;
        BodyOrderA[8][ 3][i] = PUP_HAND_RIGHT;
        BodyOrderA[8][ 4][i] = PUP_HAND_LEFT;
        BodyOrderA[8][ 5][i] = PUP_FOOT;
        BodyOrderA[8][ 6][i] = PUP_LEGS;
        BodyOrderA[8][ 7][i] = PUP_BOOT;
        BodyOrderA[8][ 8][i] = PUP_ROBELEGS;
        BodyOrderA[8][ 9][i] = PUP_ARM_RIGHT;
        BodyOrderA[8][10][i] = PUP_ARM_LEFT;
        BodyOrderA[8][11][i] = PUP_CAPE_2;
        BodyOrderA[8][12][i] = PUP_BODY;
        BodyOrderA[8][13][i] = PUP_MASK;
        BodyOrderA[8][14][i] = PUP_HEAD;
        BodyOrderA[8][15][i] = PUP_HAIR;
        BodyOrderA[8][16][i] = PUP_HAT;
        BodyOrderA[8][17][i] = PUP_BACKBODY;
        BodyOrderA[8][18][i] = PUP_CAPE;
    }

    // 180
    for (int i = 2; i < 5; i++) {
        BodyOrderA[8][ 0][i] = PUP_WEAPON2;
        BodyOrderA[8][ 1][i] = PUP_SHIELD;
        BodyOrderA[8][ 2][i] = PUP_HAND_LEFT;
        BodyOrderA[8][ 3][i] = PUP_FOOT;
        BodyOrderA[8][ 4][i] = PUP_LEGS;
        BodyOrderA[8][ 5][i] = PUP_BOOT;
        BodyOrderA[8][ 6][i] = PUP_ROBELEGS;
        BodyOrderA[8][ 7][i] = PUP_ARM_RIGHT;
        BodyOrderA[8][ 8][i] = PUP_ARM_LEFT;
        BodyOrderA[8][ 9][i] = PUP_CAPE_2;
        BodyOrderA[8][10][i] = PUP_BODY;
        BodyOrderA[8][11][i] = PUP_MASK;
        BodyOrderA[8][12][i] = PUP_HEAD;
        BodyOrderA[8][13][i] = PUP_HAIR;
        BodyOrderA[8][14][i] = PUP_HAT;
        BodyOrderA[8][15][i] = PUP_BACKBODY;
        BodyOrderA[8][16][i] = PUP_CAPE;
        BodyOrderA[8][17][i] = PUP_WEAPON;
        BodyOrderA[8][18][i] = PUP_HAND_RIGHT;
    }

    // 180
    for (int i = 5; i < 9; i++) {
        BodyOrderA[8][ 0][i] = PUP_WEAPON2;
        BodyOrderA[8][ 1][i] = PUP_SHIELD;
        BodyOrderA[8][ 2][i] = PUP_WEAPON;
        BodyOrderA[8][ 3][i] = PUP_HAND_RIGHT;
        BodyOrderA[8][ 4][i] = PUP_HAND_LEFT;
        BodyOrderA[8][ 5][i] = PUP_FOOT;
        BodyOrderA[8][ 6][i] = PUP_LEGS;
        BodyOrderA[8][ 7][i] = PUP_BOOT;
        BodyOrderA[8][ 8][i] = PUP_ROBELEGS;
        BodyOrderA[8][ 9][i] = PUP_ARM_RIGHT;
        BodyOrderA[8][10][i] = PUP_ARM_LEFT;
        BodyOrderA[8][11][i] = PUP_CAPE_2;
        BodyOrderA[8][12][i] = PUP_BODY;
        BodyOrderA[8][13][i] = PUP_MASK;
        BodyOrderA[8][14][i] = PUP_HEAD;
        BodyOrderA[8][15][i] = PUP_HAIR;
        BodyOrderA[8][16][i] = PUP_HAT;
        BodyOrderA[8][17][i] = PUP_BACKBODY;
        BodyOrderA[8][18][i] = PUP_CAPE;
    }

    for (int i = 0; i < 5; i++) 
    { 
        // 045
        BodyOrderA[1][ 0][i] = PUP_BACKBODY;
        BodyOrderA[1][ 1][i] = PUP_WEAPON;
        BodyOrderA[1][ 2][i] = PUP_HAND_RIGHT;
        BodyOrderA[1][ 3][i] = PUP_ARM_RIGHT;
        BodyOrderA[1][ 4][i] = PUP_FOOT;
        BodyOrderA[1][ 5][i] = PUP_LEGS;
        BodyOrderA[1][ 6][i] = PUP_BOOT;
        BodyOrderA[1][ 7][i] = PUP_ROBELEGS;
        BodyOrderA[1][ 8][i] = PUP_BODY;
        BodyOrderA[1][ 9][i] = PUP_HEAD;
        BodyOrderA[1][10][i] = PUP_HAIR;
        BodyOrderA[1][11][i] = PUP_HAT;
        BodyOrderA[1][12][i] = PUP_MASK;
        BodyOrderA[1][13][i] = PUP_CAPE;
        BodyOrderA[1][14][i] = PUP_CAPE_2;
        BodyOrderA[1][15][i] = PUP_HAND_LEFT;
        BodyOrderA[1][16][i] = PUP_ARM_LEFT;
        BodyOrderA[1][17][i] = PUP_SHIELD;
        BodyOrderA[1][18][i] = PUP_WEAPON2;
        // 315			  
        BodyOrderA[3][ 0][i] = PUP_BACKBODY;
        BodyOrderA[3][ 1][i] = PUP_WEAPON;
        BodyOrderA[3][ 2][i] = PUP_HAND_RIGHT;
        BodyOrderA[3][ 3][i] = PUP_ARM_RIGHT;
        BodyOrderA[3][ 4][i] = PUP_FOOT;
        BodyOrderA[3][ 5][i] = PUP_LEGS;
        BodyOrderA[3][ 6][i] = PUP_BOOT;
        BodyOrderA[3][ 7][i] = PUP_ROBELEGS;
        BodyOrderA[3][ 8][i] = PUP_BODY;
        BodyOrderA[3][ 9][i] = PUP_HEAD;
        BodyOrderA[3][10][i] = PUP_HAIR;
        BodyOrderA[3][11][i] = PUP_HAT;
        BodyOrderA[3][12][i] = PUP_MASK;
        BodyOrderA[3][13][i] = PUP_CAPE;
        BodyOrderA[3][14][i] = PUP_CAPE_2;
        BodyOrderA[3][15][i] = PUP_HAND_LEFT;
        BodyOrderA[3][16][i] = PUP_ARM_LEFT;
        BodyOrderA[3][17][i] = PUP_SHIELD;
        BodyOrderA[3][18][i] = PUP_WEAPON2;
        /// 135
        BodyOrderA[7][ 0][i] = PUP_WEAPON2;
        BodyOrderA[7][ 1][i] = PUP_SHIELD;
        BodyOrderA[7][ 2][i] = PUP_WEAPON;
        BodyOrderA[7][ 3][i] = PUP_HAND_RIGHT;
        BodyOrderA[7][ 4][i] = PUP_ARM_RIGHT;
        BodyOrderA[7][ 5][i] = PUP_FOOT;
        BodyOrderA[7][ 6][i] = PUP_LEGS;
        BodyOrderA[7][ 7][i] = PUP_BOOT;
        BodyOrderA[7][ 8][i] = PUP_ROBELEGS;
        BodyOrderA[7][ 9][i] = PUP_CAPE_2;
        BodyOrderA[7][10][i] = PUP_BODY;
        BodyOrderA[7][11][i] = PUP_MASK;
        BodyOrderA[7][12][i] = PUP_HEAD;
        BodyOrderA[7][13][i] = PUP_HAIR;
        BodyOrderA[7][14][i] = PUP_HAT;
        BodyOrderA[7][15][i] = PUP_HAND_LEFT;
        BodyOrderA[7][16][i] = PUP_ARM_LEFT;
        BodyOrderA[7][17][i] = PUP_BACKBODY;
        BodyOrderA[7][18][i] = PUP_CAPE;
    }

    // 135
    for (int i = 5; i < 9; i++) 
    {
        // 045
        BodyOrderA[1][ 0][i] = PUP_BACKBODY;
        BodyOrderA[1][ 1][i] = PUP_WEAPON;
        BodyOrderA[1][ 2][i] = PUP_HAND_RIGHT;
        BodyOrderA[1][ 3][i] = PUP_ARM_RIGHT;
        BodyOrderA[1][ 4][i] = PUP_FOOT;
        BodyOrderA[1][ 5][i] = PUP_LEGS;
        BodyOrderA[1][ 6][i] = PUP_BOOT;
        BodyOrderA[1][ 7][i] = PUP_ROBELEGS;
        BodyOrderA[1][ 8][i] = PUP_BODY;
        BodyOrderA[1][ 9][i] = PUP_CAPE;
        BodyOrderA[1][10][i] = PUP_CAPE_2;
        BodyOrderA[1][11][i] = PUP_HAND_LEFT;
        BodyOrderA[1][12][i] = PUP_ARM_LEFT;
        BodyOrderA[1][13][i] = PUP_HEAD;
        BodyOrderA[1][14][i] = PUP_HAIR;
        BodyOrderA[1][15][i] = PUP_HAT;
        BodyOrderA[1][16][i] = PUP_MASK;
        BodyOrderA[1][17][i] = PUP_SHIELD;
        BodyOrderA[1][18][i] = PUP_WEAPON2;
        // 315			  
        BodyOrderA[3][ 0][i] = PUP_BACKBODY;
        BodyOrderA[3][ 1][i] = PUP_WEAPON;
        BodyOrderA[3][ 2][i] = PUP_HAND_RIGHT;
        BodyOrderA[3][ 3][i] = PUP_ARM_RIGHT;
        BodyOrderA[3][ 4][i] = PUP_FOOT;
        BodyOrderA[3][ 5][i] = PUP_LEGS;
        BodyOrderA[3][ 6][i] = PUP_BOOT;
        BodyOrderA[3][ 7][i] = PUP_ROBELEGS;
        BodyOrderA[3][ 8][i] = PUP_BODY;
        BodyOrderA[3][ 9][i] = PUP_CAPE;
        BodyOrderA[3][10][i] = PUP_CAPE_2;
        BodyOrderA[3][11][i] = PUP_HAND_LEFT;
        BodyOrderA[3][12][i] = PUP_ARM_LEFT;
        BodyOrderA[3][13][i] = PUP_HEAD;
        BodyOrderA[3][14][i] = PUP_HAIR;
        BodyOrderA[3][15][i] = PUP_HAT;
        BodyOrderA[3][16][i] = PUP_MASK;
        BodyOrderA[3][17][i] = PUP_SHIELD;
        BodyOrderA[3][18][i] = PUP_WEAPON2;






        //135
        BodyOrderA[7][ 0][i] = PUP_WEAPON;
        BodyOrderA[7][ 1][i] = PUP_HAND_RIGHT;
        BodyOrderA[7][ 2][i] = PUP_ARM_RIGHT;
        BodyOrderA[7][ 3][i] = PUP_FOOT;
        BodyOrderA[7][ 4][i] = PUP_LEGS;
        BodyOrderA[7][ 5][i] = PUP_BOOT;
        BodyOrderA[7][ 6][i] = PUP_ROBELEGS;
        BodyOrderA[7][ 7][i] = PUP_CAPE_2;
        BodyOrderA[7][ 8][i] = PUP_BODY;
        BodyOrderA[7][ 9][i] = PUP_MASK;
        BodyOrderA[7][10][i] = PUP_HEAD;
        BodyOrderA[7][11][i] = PUP_HAIR;
        BodyOrderA[7][12][i] = PUP_HAT;
        BodyOrderA[7][13][i] = PUP_HAND_LEFT;
        BodyOrderA[7][14][i] = PUP_ARM_LEFT;
        BodyOrderA[7][15][i] = PUP_BACKBODY;
        BodyOrderA[7][16][i] = PUP_CAPE;
        BodyOrderA[7][17][i] = PUP_SHIELD;
        BodyOrderA[7][18][i] = PUP_WEAPON2;
    }

    // 225
    for (int i = 0; i < 5; i++) {
        BodyOrderA[9][ 0][i] = PUP_WEAPON2;
        BodyOrderA[9][ 1][i] = PUP_SHIELD;
        BodyOrderA[9][ 2][i] = PUP_WEAPON;
        BodyOrderA[9][ 3][i] = PUP_HAND_RIGHT;
        BodyOrderA[9][ 4][i] = PUP_ARM_RIGHT;
        BodyOrderA[9][ 5][i] = PUP_FOOT;
        BodyOrderA[9][ 6][i] = PUP_LEGS;
        BodyOrderA[9][ 7][i] = PUP_BOOT;
        BodyOrderA[9][ 8][i] = PUP_ROBELEGS;
        BodyOrderA[9][ 9][i] = PUP_CAPE_2;
        BodyOrderA[9][10][i] = PUP_MASK;
        BodyOrderA[9][11][i] = PUP_BODY;
        BodyOrderA[9][12][i] = PUP_HEAD;
        BodyOrderA[9][13][i] = PUP_HAIR;
        BodyOrderA[9][14][i] = PUP_HAT;
        BodyOrderA[9][15][i] = PUP_HAND_LEFT;
        BodyOrderA[9][16][i] = PUP_ARM_LEFT;
        BodyOrderA[9][17][i] = PUP_BACKBODY;
        BodyOrderA[9][18][i] = PUP_CAPE;
    }

    // 225		
    for (int i = 5; i < 9; i++) {
        BodyOrderA[9][ 0][i] = PUP_WEAPON;
        BodyOrderA[9][ 1][i] = PUP_HAND_RIGHT;
        BodyOrderA[9][ 2][i] = PUP_ARM_RIGHT;
        BodyOrderA[9][ 3][i] = PUP_FOOT;
        BodyOrderA[9][ 4][i] = PUP_LEGS;
        BodyOrderA[9][ 5][i] = PUP_BOOT;
        BodyOrderA[9][ 6][i] = PUP_ROBELEGS;
        BodyOrderA[9][ 7][i] = PUP_CAPE_2;
        BodyOrderA[9][ 8][i] = PUP_BODY;
        BodyOrderA[9][ 9][i] = PUP_MASK;
        BodyOrderA[9][10][i] = PUP_HEAD;
        BodyOrderA[9][11][i] = PUP_HAIR;
        BodyOrderA[9][12][i] = PUP_HAT;
        BodyOrderA[9][13][i] = PUP_HAND_LEFT;
        BodyOrderA[9][14][i] = PUP_ARM_LEFT;
        BodyOrderA[9][15][i] = PUP_BACKBODY;
        BodyOrderA[9][16][i] = PUP_CAPE;
        BodyOrderA[9][17][i] = PUP_SHIELD;
        BodyOrderA[9][18][i] = PUP_WEAPON2;
    }
						      
    // 000
    for (int i = 0; i < 5; i++) {
        BodyOrderA[2][ 0][i] = PUP_CAPE;
        BodyOrderA[2][ 1][i] = PUP_BACKBODY;
        BodyOrderA[2][ 2][i] = PUP_FOOT;
        BodyOrderA[2][ 3][i] = PUP_LEGS;
        BodyOrderA[2][ 4][i] = PUP_BOOT;
        BodyOrderA[2][ 5][i] = PUP_ROBELEGS;
        BodyOrderA[2][ 6][i] = PUP_WEAPON;
        BodyOrderA[2][ 7][i] = PUP_HAND_RIGHT;
        BodyOrderA[2][ 8][i] = PUP_ARM_RIGHT;
        BodyOrderA[2][ 9][i] = PUP_BODY;
        BodyOrderA[2][10][i] = PUP_HAND_LEFT;
        BodyOrderA[2][11][i] = PUP_ARM_LEFT;
        BodyOrderA[2][12][i] = PUP_CAPE_2;
        BodyOrderA[2][13][i] = PUP_HEAD;
        BodyOrderA[2][14][i] = PUP_HAIR;
        BodyOrderA[2][15][i] = PUP_HAT;
        BodyOrderA[2][16][i] = PUP_MASK;
        BodyOrderA[2][17][i] = PUP_SHIELD;
        BodyOrderA[2][18][i] = PUP_WEAPON2;
    }

    // 000
    for (int i = 5; i < 9; i++) {
        BodyOrderA[2][ 0][i] = PUP_WEAPON2;
        BodyOrderA[2][ 1][i] = PUP_SHIELD;
        BodyOrderA[2][ 2][i] = PUP_CAPE;
        BodyOrderA[2][ 3][i] = PUP_BACKBODY;
        BodyOrderA[2][ 4][i] = PUP_FOOT;
        BodyOrderA[2][ 5][i] = PUP_LEGS;
        BodyOrderA[2][ 6][i] = PUP_BOOT;
        BodyOrderA[2][ 7][i] = PUP_ROBELEGS;
        BodyOrderA[2][ 8][i] = PUP_WEAPON;
        BodyOrderA[2][ 9][i] = PUP_HAND_RIGHT;
        BodyOrderA[2][10][i] = PUP_HAND_LEFT;
        BodyOrderA[2][11][i] = PUP_ARM_RIGHT;
        BodyOrderA[2][12][i] = PUP_ARM_LEFT;
        BodyOrderA[2][13][i] = PUP_BODY;
        BodyOrderA[2][14][i] = PUP_CAPE_2;
        BodyOrderA[2][15][i] = PUP_HEAD;
        BodyOrderA[2][16][i] = PUP_HAIR;
        BodyOrderA[2][17][i] = PUP_HAT;
        BodyOrderA[2][18][i] = PUP_MASK;
    } 

   fTransparent = Object->BodyPart[0]->fTransparent;
   TransColor = Object->BodyPart[0]->TransColor;

   MaxWidth = 215;  //115
   MaxHeight = 200; //100
   MaxxCor = -100; //-50
   MaxyCor = -130; //-80

   Type = Object->BodyPart[0]->Type;
   Faces = Object->BodyPart[0]->Faces;
   FacesA = Object->BodyPart[0]->FacesA;
   FacesC = Object->BodyPart[0]->FacesC;
   FacesStA = Object->BodyPart[0]->FacesStA;
   FacesStM = Object->BodyPart[0]->FacesStM;
}
																	  