#include "pch.h"
#include "V2PalManager.h"
#include <string>
#include "Zlib/zlib.h"
#include "MD5\MD5Checksum.h"

using namespace std;


#define V2DATABASE_PALETTE  "Game Files\\V2ColorI.dpd"

CV2PalManager::CV2PalManager()
{
   char   strChksumMd5[33];
   char   *pstrMD5Part2 = &strChksumMd5[16];
   ULONG  ulTailleUnpack;
   ULONG  ulTaillePack;
   BYTE   uchChksum;

   FILE *pFilePal = fopen(V2DATABASE_PALETTE,"rb");
   if(!pFilePal)
      return ;
   fread(strChksumMd5    ,1,16,pFilePal);
   fread(&ulTailleUnpack ,4,1,pFilePal);
   fread(&ulTaillePack   ,4,1,pFilePal);
   fread(pstrMD5Part2    ,1,17,pFilePal);

   BYTE *pchData    = new unsigned char[ulTailleUnpack];
   BYTE *pCompressed= new unsigned char[ulTaillePack];
   
   fread(pCompressed,1,ulTaillePack,pFilePal);
   fread(&uchChksum ,1,1,pFilePal);
   fclose(pFilePal);

   BYTE uchVal = CalcChecksumComp2(pCompressed,ulTaillePack);
   if(uchVal != uchChksum)
   {
      delete []pchData;
      delete []pCompressed;
      return;
   }


   if(uncompress(pchData,&ulTailleUnpack,pCompressed,ulTaillePack) != S_OK)
      OutputDebugString("*********** IMP ********  Probleme de load du fichier de palette...\n");

   char *pstrMD5 = CMD5Checksum::GetMD5(pchData,ulTailleUnpack);
   if(strcmp(pstrMD5,strChksumMd5)!= 0)
   {
      delete []pchData;
      delete []pCompressed;
      delete []pstrMD5;
      return;
   }

   for(UINT i=0;i<ulTailleUnpack;i++)
      pchData[i] ^= 0x66;

   m_dwNbrPalette = (ulTailleUnpack/sizeof(PalInfo));
   m_pV2Palette = new PalInfo[m_dwNbrPalette];

   BYTE *pchDataTmp = pchData;
   for(int i=0;i<m_dwNbrPalette;i++)
   {
      pchDataTmp = pchData+(i*sizeof(PalInfo));
      memcpy(&m_pV2Palette[i],pchDataTmp,sizeof(PalInfo));
      if (m_pV2Palette[i].lpszID[strlen(m_pV2Palette[i].lpszID)-1] == 'P')
			m_pV2Palette[i].lpszID[strlen(m_pV2Palette[i].lpszID)-1] = 0;
   }
   
   delete []pchData;
   delete []pCompressed;
   delete []pstrMD5;

     
   //m_plRefPal.LoadPalette("Bright1"); //TODO DaP
   

}

CV2PalManager::~CV2PalManager()
{
    delete []m_pV2Palette;
}

LPBYTE CV2PalManager::GetPal(const char *lpszID, int PalNb) 
{
	UINT iLen = 0;//NM
	LPBYTE lpbBestPal = m_plRefPal.lpSpritePal;
   //if(m_plRefPal.lpSpritePal != NULL)
   //   return lpbBestPal;

	char Temp[100];
	for (int i = m_dwNbrPalette-1; i >= 0; i--) 
	{
		if (PalNb == 1) 
		{
			if (!strnicmp(m_pV2Palette[i].lpszID, lpszID, strlen(m_pV2Palette[i].lpszID)) && iLen < strlen(m_pV2Palette[i].lpszID))  {
				lpbBestPal = (LPBYTE)(m_pV2Palette[i]).lpSpritePal;
				iLen = strlen(m_pV2Palette[i].lpszID);
			}
		} 
		else 
		{
			if( PalNb < 10 ) 
			{
				if (m_pV2Palette[i].lpszID[strlen(m_pV2Palette[i].lpszID)-1] == PalNb + '0')  
				{
					strcpy(Temp, m_pV2Palette[i].lpszID);
					Temp[strlen(Temp)-1] = 0;
					if (!strnicmp(Temp, lpszID, strlen(Temp)) && iLen < strlen(Temp))  
					{
						LOG << "\r\nSelecting pallet for sprite (pallet under 10) " << m_pV2Palette[i].lpszID;
						LOG << "\r\nSelected best pallet ("<< PalNb << ") : " << (unsigned char)(m_pV2Palette[i]).lpSpritePal;
						lpbBestPal = (LPBYTE)(m_pV2Palette[i]).lpSpritePal;
						iLen = strlen(Temp);
					}
				}
			} 
			else 
			{
				string strTemp(m_pV2Palette[i].lpszID);
				if (atoi(strTemp.substr(strlen(strTemp.c_str())-2, 2).c_str()) == PalNb)  
				{
					strcpy(Temp, m_pV2Palette[i].lpszID);
					Temp[strlen(Temp)-2] = 0;
					if (!strnicmp(Temp, lpszID, strlen(Temp)) && iLen < strlen(Temp))  
					{
						LOG << "\r\nSelecting pallet for sprite (pallet above 10) " << m_pV2Palette[i].lpszID;
						LOG << "\r\nSelected best pallet ("<< PalNb << ") : " << (unsigned char)(m_pV2Palette[i]).lpSpritePal;
						lpbBestPal = (LPBYTE)(m_pV2Palette[i]).lpSpritePal;
						iLen = strlen(Temp);
					}
				}
			}
		}
	}
	return lpbBestPal;
}

LPBYTE CV2PalManager::GetPalIndex(const char *lpszID, int PalNb) 
{
	//m_plRefPal.LoadPalette("Bright1"); //TODO DaP

   LPBYTE pPalTmp = GetPal(lpszID,PalNb);
   LPBYTE lpSpritePal = new BYTE [768];
   memcpy(lpSpritePal,pPalTmp,768);
   return lpSpritePal;
}


CV2PalManager* CV2PalManager::GetInstance(void) 
{
   static CV2PalManager pmInstance;
   return &pmInstance;
}

unsigned char  CV2PalManager::CalcChecksumComp2(unsigned char	*pData, unsigned long dwNbrData)
{
   if(!pData || dwNbrData <=0)
      return 0x00; // pas de données a calculer
   
   unsigned char chkSum;
   unsigned char sumByte = 0x00;
   // on additionne tous les BYTES sauf le checksum...
   for(DWORD i=0;i<dwNbrData-1;i++)
   {
      sumByte += pData[i];
   }

   chkSum = 256 - (sumByte);

   return chkSum;
}