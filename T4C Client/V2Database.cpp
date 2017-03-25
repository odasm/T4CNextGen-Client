// Include.
#include "pch.h"
#include <windows.h>
#include <stdio.h>
#pragma warning( disable: 4786 )
#include "V2Database.h"
#include "WarningBox.h"
#include "PowerDebug.h"
#include "IDirectXDraw.h"
#include "App.h"
#include "Global.h"
#include "MemoryManager.h"
#include "V2PalManager.h"
#include "MemMapFile.h"
#include "TFCPlayer.h"
#include "Zlib/zlib.h"
#include "MD5\MD5Checksum.h"

namespace
{
	struct stricmppred : public std::binary_function<std::string, std::string, bool>
	{
		bool operator()(const std::string& x, const std::string& y) const
		{
			return(stricmp(x.c_str(), y.c_str()) < 0);
		}
	};

	typedef std::map< std::string, LPDATAHEADER, stricmppred > IdMap;


	IdMap    idInfo;
	IdMap    palInfo;
}

#define V2DATABASE_INDEX  "Game Files\\V2DataI.did"
#define V2DATABASE_DATA   "Game Files\\V2Data" //need to add index and .dda

#define _RGB16BIT555(r,g,b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))
#define _RGB16BIT565(r,g,b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))
#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

// Set the Unique Global
CV2Database g_V2Database;

//////////////////////////////////////////////////////////////////////////////////////////
CV2Database::CV2Database()
{
	m_pDataBuffer = new MemMapFile[NB_FILES];
	for (int i = 0; i < NB_FILES; i++)
	{
		m_bDataBufferLoaded[i] = false;
	}
	m_dwLastFileindexLoaded = -1;

	m_bIndexLoaded = FALSE;

	InitializeCriticalSection(&csThreadLock);
}

// Destructor ////////////////////////////////////////////////////////////////////////////
CV2Database::~CV2Database()
{
	DeleteCriticalSection(&csThreadLock);

	for (int i = 0; i < NB_FILES; i++)
	{
		m_pDataBuffer[i].Release();
		m_bDataBufferLoaded[i] = false;
	}
	if (m_pDataBuffer)
		delete[] m_pDataBuffer;
	m_pDataBuffer = NULL;

	// Delete IndexInfo
	Release();
}

void CV2Database::Release(void)
{
	IdMap::iterator itm;
	for (itm = idInfo.begin(); itm != idInfo.end(); itm++)
	{
		LPDATAHEADER ii = (*itm).second;
		if (ii->pstrNom)
		{
			delete[]ii->pstrNom;
			ii->pstrNom = NULL;
		}
		if (ii->lpSpriteInfo)
		{
			if (ii->lpSpriteInfo->Data.lpbChunck)
			{
				delete[]ii->lpSpriteInfo->Data.lpbChunck;
				ii->lpSpriteInfo->Data.lpbChunck = NULL;
			}
			else if (ii->lpSpriteInfo->Data.lpDDSurface && !g_boQuitApp)
			{
				ii->lpSpriteInfo->Data.lpDDSurface->Release();
				ii->lpSpriteInfo->Data.lpDDSurface = NULL;
			}
			delete (ii->lpSpriteInfo);
			ii->lpSpriteInfo = NULL;
		}
		delete ii;
		ii = NULL;
	}
	idInfo.clear();
}

//////////////////////////////////////////////////////////////////////////////////////////
// Release an Object into the Database.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void CV2Database::Release(LPCTSTR lpszID)
{
	EnterCriticalSection(&csThreadLock);

	IdMap::iterator itm = idInfo.find(lpszID);
	if (itm == idInfo.end())
	{
		LeaveCriticalSection(&csThreadLock);
		return;
	}

	LPDATAHEADER ii = (*itm).second;
	if (ii->dwCount)
	{
		ii->dwCount--;
	}

	if (!ii->dwCount && ii->lpSpriteInfo)
	{
		if (ii->lpSpriteInfo->Data.lpbChunck)
		{
			delete (ii->lpSpriteInfo->Data.lpbChunck);
			ii->lpSpriteInfo->Data.lpbChunck = NULL;
		}
		else if (ii->lpSpriteInfo->Data.lpDDSurface && !g_boQuitApp)
		{
			ii->lpSpriteInfo->Data.lpDDSurface->Release();
			ii->lpSpriteInfo->Data.lpDDSurface = NULL;
		}
		delete ii->lpSpriteInfo;
		ii->lpSpriteInfo = NULL;
	}

	LeaveCriticalSection(&csThreadLock);
}

//////////////////////////////////////////////////////////////////////////////////////////
// Load all Index.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void  CV2Database::LoadIndex()
{
	// load temporary index files...
	char   strChksumMd5[33];
	char   *pstrMD5Part2 = &strChksumMd5[16];
	ULONG  ulTailleUnpack;
	ULONG  ulTaillePack;
	BYTE   uchChksum;

	FILE *pFile = fopen(V2DATABASE_INDEX, "rb");
	if (!pFile)
		return;
	fread(strChksumMd5, 1, 16, pFile);
	fread(&ulTailleUnpack, 4, 1, pFile);
	fread(&ulTaillePack, 4, 1, pFile);
	fread(pstrMD5Part2, 1, 17, pFile);

	BYTE *pchData = new unsigned char[ulTailleUnpack];
	BYTE *pCompressed = new unsigned char[ulTaillePack];

	fread(pCompressed, 1, ulTaillePack, pFile);
	fread(&uchChksum, 1, 1, pFile);
	fclose(pFile);

	BYTE uchVal = CalcChecksumComp2(pCompressed, ulTaillePack);
	if (uchVal != uchChksum)
	{
		delete[]pchData;
		delete[]pCompressed;
		return;
	}


	if (uncompress(pchData, &ulTailleUnpack, pCompressed, ulTaillePack) != S_OK)
		OutputDebugString("*********** IMP ********  Probleme de load du fichier Index...\n");

	char *pstrMD5 = CMD5Checksum::GetMD5(pchData, ulTailleUnpack);
	if (strcmp(pstrMD5, strChksumMd5) != 0)
	{
		delete[]pchData;
		delete[]pCompressed;
		delete[]pstrMD5;
		return;
	}

	for (UINT i = 0; i < ulTailleUnpack; i++)
		pchData[i] ^= 0x99;

	UINT dwNbrIndex = (ulTailleUnpack / sizeof(sHeaderFile));
	BYTE *pchDataTmp = pchData;
	for (int i = 0; i < dwNbrIndex; i++)
	{
		sHeaderFile newHeaderTmp;
		pchDataTmp = pchData + (i * sizeof(sHeaderFile));
		memcpy(&newHeaderTmp, pchDataTmp, sizeof(sHeaderFile));

		LPDATAHEADER ii = new DATAHEADER;
		ZeroMemory(ii, sizeof(DATAHEADER));

		ii->chNbrNom = strlen(newHeaderTmp.strNom);
		ii->pstrNom = new char[ii->chNbrNom + 1];
		memcpy(ii->pstrNom, newHeaderTmp.strNom, ii->chNbrNom + 1);
		ii->dwFileOffset = newHeaderTmp.dwFileOffset;
		ii->dwDataFileIndex = newHeaderTmp.dwDataFileIndex;

		IdMap::iterator itm = idInfo.find(ii->pstrNom);
		if (itm == idInfo.end())
		{
			idInfo[ii->pstrNom] = ii;
		}
		else
		{
			// Delete the previous entry.
			if ((*itm).second->pstrNom)
			{
				delete (*itm).second->pstrNom;
				(*itm).second->pstrNom = NULL;
			}
			delete (*itm).second;
			(*itm).second = ii;
		}
	}

	delete[]pchData;
	delete[]pCompressed;
	delete[]pstrMD5;

	//load data memory map file...
	char strName[128];

	for (int i = 0; i < NB_FILES; i++)
	{
		sprintf(strName, "%s%02d.dda", V2DATABASE_DATA, i);
		m_bDataBufferLoaded[i] = m_pDataBuffer[i].Create(strName, false);
		if (m_bDataBufferLoaded[i] == false)
		{
			m_dwLastFileindexLoaded = i - 1;
		}
	}
	m_bIndexLoaded = TRUE;
}

unsigned char  CV2Database::CalcChecksumComp2(unsigned char	*pData, unsigned long dwNbrData)
{
	if (!pData || dwNbrData <= 0)
		return 0x00; // pas de données a calculer

	unsigned char chkSum;
	unsigned char sumByte = 0x00;
	// on additionne tous les BYTES sauf le checksum...
	for (DWORD i = 0; i < dwNbrData - 1; i++)
	{
		sumByte += pData[i];
	}

	chkSum = 256 - (sumByte);

	return chkSum;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Load a Sprite Info Object.
// 
//////////////////////////////////////////////////////////////////////////////////////////
DWORD CV2Database::LoadSprite(LPCTSTR lpszID, LPV2SPRITEINFO *lplpSpriteInfo, DWORD dwRelativeIndex,
	LPBYTE  lpPal, int iPalIndex)
{
	EnterCriticalSection(&csThreadLock);

	if (!m_bIndexLoaded)
		LoadIndex();

	IdMap::iterator itm;
	itm = idInfo.find(lpszID);
	if (itm == idInfo.end())
	{
		LeaveCriticalSection(&csThreadLock);
		return 0;
	}
	LPDATAHEADER ii = (*itm).second;

	ii->dwCount++; // Increment Usage Count

	if ((iPalIndex == 0 && ii->lpSpriteInfo && ii->lpSpriteInfo->Data.lpbChunck) || (iPalIndex == 1 && ii->lpSpriteInfo))
	{
		*lplpSpriteInfo = ii->lpSpriteInfo; // This Graphics is already loaded in memory.
	}
	else
	{
		*lplpSpriteInfo = new V2SPRITEINFO;

		//get the data buffer
		m_pDataBuffer[ii->dwDataFileIndex].CpyMemory(&(*lplpSpriteInfo)->Header, sizeof(V2SPRITEHEADER), ii->dwFileOffset);

		(*lplpSpriteInfo)->Header.dwCompType ^= 0xAAAA;
		(*lplpSpriteInfo)->Header.flag1 ^= 0x1458;
		(*lplpSpriteInfo)->Header.dwWidth ^= 0x1234;
		(*lplpSpriteInfo)->Header.dwHeight ^= 0x6242;
		(*lplpSpriteInfo)->Header.shOffX1 ^= 0x2355;
		(*lplpSpriteInfo)->Header.shOffY1 ^= 0xF6C3;
		(*lplpSpriteInfo)->Header.shOffX2 ^= 0xAAF3;
		(*lplpSpriteInfo)->Header.shOffY2 ^= 0xAAAA;
		(*lplpSpriteInfo)->Header.ushTransparency ^= 0x4321;
		(*lplpSpriteInfo)->Header.ushTransColor ^= 0x1234;
		(*lplpSpriteInfo)->Header.dwDataUnpack ^= 0xDDCCBBAA;
		(*lplpSpriteInfo)->Header.dwDataPack ^= 0xAABBCCDD;

		int dwOffsetData = ii->dwFileOffset + sizeof(V2SPRITEHEADER);

		// Type of Sprite Format
		switch ((*lplpSpriteInfo)->Header.dwCompType)
		{
		case COMP_DD:  // Raw Format.
			LoadSprite_DD(lplpSpriteInfo, ii, dwOffsetData);
			break;
		case COMP_NCK: // No Color Key Format.
			LoadSprite_NCK(lplpSpriteInfo, ii, lpszID, lpPal, iPalIndex, dwOffsetData);
			break;
		case COMP_ZIP:
			LoadSprite_ZIP(lplpSpriteInfo, ii, lpszID, lpPal, iPalIndex, dwOffsetData);
			break;
		case COMP_NULL:
			ii->lpSpriteInfo = *lplpSpriteInfo;
			(*lplpSpriteInfo)->Data.lpbChunck = 0;
			(*lplpSpriteInfo)->Data.lpDDSurface = 0;
			break;
		}
	}
	LeaveCriticalSection(&csThreadLock);
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Load a Sprite No Color Keying.
//////////////////////////////////////////////////////////////////////////////////////////
void CV2Database::LoadSprite_NCK(LPV2SPRITEINFO *lplpSpriteInfo, LPDATAHEADER ii, LPCTSTR lpszID, LPBYTE lpPal, int iPalIndex, int dwDataCompIndex)
{
	if ((*lplpSpriteInfo)->Header.dwWidth > 180 || (*lplpSpriteInfo)->Header.dwHeight > 180)//:CR_NMNMNM Valid
	{
		BYTE *pDdata = new BYTE[(*lplpSpriteInfo)->Header.dwDataPack];
		// copy the compressed data...
		m_pDataBuffer[ii->dwDataFileIndex].CpyMemory(pDdata, (*lplpSpriteInfo)->Header.dwDataPack, dwDataCompIndex);

		//Unpack Data
		(*lplpSpriteInfo)->Data.lpbChunck = new BYTE[(*lplpSpriteInfo)->Header.dwDataUnpack];
		uncompress((*lplpSpriteInfo)->Data.lpbChunck, &(*lplpSpriteInfo)->Header.dwDataUnpack, pDdata, (*lplpSpriteInfo)->Header.dwDataPack);
		delete[]pDdata;
		pDdata = NULL;
	}
	else
	{
		(*lplpSpriteInfo)->Data.lpbChunck = new BYTE[(*lplpSpriteInfo)->Header.dwDataUnpack];
		m_pDataBuffer[ii->dwDataFileIndex].CpyMemory((*lplpSpriteInfo)->Data.lpbChunck, (*lplpSpriteInfo)->Header.dwDataPack, dwDataCompIndex);
	}

	// now data will be like old format...

	if (iPalIndex != 0 || (*lplpSpriteInfo)->Header.ushTransparency == 6)
	{
		int i, x, y, nb_pix;
		BYTE	*pt_pack;

		int lg = (*lplpSpriteInfo)->Header.dwWidth;
		int ht = (*lplpSpriteInfo)->Header.dwHeight;
		lg = (lg + 3)&~3;
		BYTE *pchUnpack = new unsigned char[lg*ht];
		FillMemory(pchUnpack, lg*ht, (*lplpSpriteInfo)->Header.ushTransColor);

		// decompresse l'image...
		pt_pack = (*lplpSpriteInfo)->Data.lpbChunck;
		x = y = 0;
		int dwCntData = 0;
		BOOL bShadow = FALSE;
		while (1) // ombrage
		{
			x = *(short *)pt_pack;	pt_pack += 2;
			nb_pix = (pt_pack[0] * 4) + pt_pack[1]; pt_pack += 2;
			if (1)
			{
				if (*pt_pack == 1)
				{
					bShadow = TRUE;
					if (nb_pix > 1)
					{
						for (int i = 0; i < nb_pix; i++)
						{
							if (!(y % 2))
							{
								if ((i + x) % 2)
									pchUnpack[i + x + (y*lg)] = 0;
							}
							else if (!((i + x) % 2))
							{
								pchUnpack[i + x + (y*lg)] = 0;
							}

						}
					}
				}
			}
			if (*pt_pack == 0)
			{
				for (i = 0; i < nb_pix; i++)
				{
					pt_pack++;
					pchUnpack[i + x + (y*lg)] = *pt_pack;
				}
			}
			pt_pack++;
			if (*pt_pack == 0)		// Fin de sprite
				break;
			if (*pt_pack == 1)      // Fin de l'ombrage
			{
				pt_pack++;
			}
			else if (*pt_pack == 2)	// Fin de ligne
			{
				y++;
				pt_pack++;
			}
			if (y == ht)
				break;
		};

		// on dois construire la surface directX et changer le type
		// pour dire que maintenant il est non compresser...

		LoadSpriteUnpack_DD(lplpSpriteInfo, ii, pchUnpack, lpszID, lpPal);
		(*lplpSpriteInfo)->Header.dwCompType = COMP_DD;

		if (pchUnpack)
			delete[]pchUnpack;
		pchUnpack = NULL;

		if ((*lplpSpriteInfo)->Data.lpbChunck)
		{
			delete[](*lplpSpriteInfo)->Data.lpbChunck;
			(*lplpSpriteInfo)->Data.lpbChunck = NULL;
		}
	}

	ii->lpSpriteInfo = *lplpSpriteInfo;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Load a Sprite No Color Keying.
// 
// LPV2SPRITEINFO *lplpSpriteInfo,  // Ptr of a Sprite Info.
// LPINDEXINFO ii,         // Position de l'Object dans l'Index.
// LPCTSTR    lpszID,
// LPBYTE     lpPal,
// int        iPalIndex
//////////////////////////////////////////////////////////////////////////////////////////
VOID CV2Database::LoadSprite_ZIP(LPV2SPRITEINFO *lplpSpriteInfo, LPDATAHEADER ii, LPCTSTR lpszID, LPBYTE lpPal, int iPalIndex, int dwDataCompIndex)
{
	if ((*lplpSpriteInfo)->Header.dwWidth > 180 || (*lplpSpriteInfo)->Header.dwHeight > 180)//:CR_NMNMNM Valid
	{
		BYTE *pDdata = new BYTE[(*lplpSpriteInfo)->Header.dwDataPack];
		// copy the compressed data...
		m_pDataBuffer[ii->dwDataFileIndex].CpyMemory(pDdata, (*lplpSpriteInfo)->Header.dwDataPack, dwDataCompIndex);

		//Unpack Data
		(*lplpSpriteInfo)->Data.lpbChunck = new BYTE[(*lplpSpriteInfo)->Header.dwDataUnpack];
		uncompress((*lplpSpriteInfo)->Data.lpbChunck, &(*lplpSpriteInfo)->Header.dwDataUnpack, pDdata, (*lplpSpriteInfo)->Header.dwDataPack);
		delete[]pDdata;
		pDdata = NULL;
	}
	else
	{
		(*lplpSpriteInfo)->Data.lpbChunck = new BYTE[(*lplpSpriteInfo)->Header.dwDataUnpack];
		m_pDataBuffer[ii->dwDataFileIndex].CpyMemory((*lplpSpriteInfo)->Data.lpbChunck, (*lplpSpriteInfo)->Header.dwDataPack, dwDataCompIndex);
	}


	// now data will be like old format...

	if (iPalIndex != 0 || (*lplpSpriteInfo)->Header.ushTransparency == 6)
	{
		unsigned long dwTailleUnc = *(unsigned int *)&(*lplpSpriteInfo)->Data.lpbChunck[0];
		BYTE *pData = (*lplpSpriteInfo)->Data.lpbChunck + 4;
		BYTE *pchUnpack = new BYTE[dwTailleUnc];

		uncompress(pchUnpack, &dwTailleUnc, pData, (*lplpSpriteInfo)->Header.dwDataUnpack - 4);



		// on dois construire la surface directX et changer le type
		// pour dire que maintenant il est non compresser...
		LoadSpriteUnpack_DD(lplpSpriteInfo, ii, pchUnpack, lpszID, lpPal);

		if (pchUnpack)
			delete[]pchUnpack;
		pchUnpack = NULL;

		(*lplpSpriteInfo)->Header.dwCompType = COMP_DD;

		if ((*lplpSpriteInfo)->Data.lpbChunck)
		{
			delete[](*lplpSpriteInfo)->Data.lpbChunck;
			(*lplpSpriteInfo)->Data.lpbChunck = NULL;
		}
	}

	ii->lpSpriteInfo = *lplpSpriteInfo;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Load a Sprite RAW.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void CV2Database::LoadSprite_DD(LPV2SPRITEINFO *lplpSpriteInfo, LPDATAHEADER ii, int dwDataCompIndex)
{

	if ((*lplpSpriteInfo)->Header.dwWidth > 180 || (*lplpSpriteInfo)->Header.dwHeight > 180)//:CR_NMNMNM Valid
	{
		BYTE *pDdata = new BYTE[(*lplpSpriteInfo)->Header.dwDataPack];
		// copy the compressed data...
		m_pDataBuffer[ii->dwDataFileIndex].CpyMemory(pDdata, (*lplpSpriteInfo)->Header.dwDataPack, dwDataCompIndex);

		//Unpack Data
		(*lplpSpriteInfo)->Data.lpbChunck = new BYTE[(*lplpSpriteInfo)->Header.dwDataUnpack];
		uncompress((*lplpSpriteInfo)->Data.lpbChunck, &(*lplpSpriteInfo)->Header.dwDataUnpack, pDdata, (*lplpSpriteInfo)->Header.dwDataPack);
		delete[]pDdata;
		pDdata = NULL;
	}
	else
	{
		(*lplpSpriteInfo)->Data.lpbChunck = new BYTE[(*lplpSpriteInfo)->Header.dwDataUnpack];
		m_pDataBuffer[ii->dwDataFileIndex].CpyMemory((*lplpSpriteInfo)->Data.lpbChunck, (*lplpSpriteInfo)->Header.dwDataPack, dwDataCompIndex);
	}

	WORD wWidth = (*lplpSpriteInfo)->Header.dwWidth;
	WORD wHeight = (*lplpSpriteInfo)->Header.dwHeight;


	{
		DDSURFACEDESC Surface;
		ZeroMemory(&Surface, sizeof(Surface));
		Surface.dwSize = sizeof(Surface);
		Surface.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
		Surface.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;
		Surface.dwWidth = wWidth;
		Surface.dwHeight = wHeight;
		//		LOG << "NAME : " << IndexInfo[dwIdxPos].lpszID << ", " << wWidth << ", " << wHeight << "\r\n";
		DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Surface, &(*lplpSpriteInfo)->Data.lpDDSurface, NULL), "Cheap Ass");

		DDCOLORKEY ddColorKey;
		memset(&ddColorKey, (*lplpSpriteInfo)->Header.ushTransColor, sizeof(DDCOLORKEY));
		(*lplpSpriteInfo)->Data.lpDDSurface->SetColorKey(DDCKEY_SRCBLT, &ddColorKey);
	}

	DDSURFACEDESC Surface;
	memset(&Surface, 0, sizeof(Surface));
	Surface.dwSize = sizeof(Surface);


	(*lplpSpriteInfo)->Data.lpDDSurface->Lock(NULL, &Surface, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

	LPBYTE lpbDest = (LPBYTE)Surface.lpSurface;
	LPBYTE lpbSource = (*lplpSpriteInfo)->Data.lpbChunck;
	long   lPitch = Surface.lPitch;


	WORD wPal, wRed, wGreen, wBlue, wColor;

	if (CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr) // TODO DaP Ajout
		CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
	LPBYTE lpPal = CV2PalManager::GetInstance()->GetPal("", 1);
	for (int i = 0; i < wHeight; i++)
	{
		for (int j = 0; j < wWidth; j++)
		{
			wPal = (*(BYTE *)(lpbSource + j)) * 3;
			wRed = (lpPal[wPal]) >> lpDXDirectDraw->bRDown;
			wGreen = (lpPal[wPal + 1]) >> lpDXDirectDraw->bGDown;
			wBlue = (lpPal[wPal + 2]) >> lpDXDirectDraw->bBDown;

			wColor = (wRed << lpDXDirectDraw->bRUp) + (wGreen << lpDXDirectDraw->bGUp) + (wBlue << lpDXDirectDraw->bBUp);

			lpbDest[(j << 1) + 1] = (BYTE)(wColor >> 8);
			lpbDest[(j << 1)] = (BYTE)(wColor & 0xFF);
		}
		lpbDest += lPitch;
		lpbSource += wWidth;
	}


	(*lplpSpriteInfo)->Data.lpDDSurface->Unlock(Surface.lpSurface);

	if ((*lplpSpriteInfo)->Data.lpbChunck)
		delete[](*lplpSpriteInfo)->Data.lpbChunck;
	(*lplpSpriteInfo)->Data.lpbChunck = NULL;

	ii->lpSpriteInfo = *lplpSpriteInfo;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Load a Sprite RAW.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void CV2Database::LoadSpriteUnpack_DD(LPV2SPRITEINFO *lplpSpriteInfo, LPDATAHEADER ii, BYTE *pchUnpackData, LPCTSTR lpszID, LPBYTE lpPal)
{
	WORD wWidth = (*lplpSpriteInfo)->Header.dwWidth;
	wWidth = (wWidth + 3)&~3;
	WORD wHeight = (*lplpSpriteInfo)->Header.dwHeight;

	DDSURFACEDESC Surface;
	ZeroMemory(&Surface, sizeof(Surface));
	Surface.dwSize = sizeof(Surface);
	Surface.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	Surface.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;
	Surface.dwWidth = wWidth;
	Surface.dwHeight = wHeight;
	DXDErrorHandle(lpDXDirectDraw->lpdd->CreateSurface(&Surface, &(*lplpSpriteInfo)->Data.lpDDSurface, NULL), "Cheap Ass");

	WORD wPal, wRed, wGreen, wBlue, wColor;

	if (lpPal == nullptr) // TODO DaP ajout de ce bordel
	{
		if(CV2PalManager::GetInstance()->m_plRefPal.GetPalette() == nullptr)
			CV2PalManager::GetInstance()->m_plRefPal.LoadPalette("Bright1");
		lpPal = CV2PalManager::GetInstance()->GetPal(lpszID, 1);
	}
	//LPBYTE lpPal = CV2PalManager::GetInstance()->GetPal(lpszID, 1);

	wPal = (*lplpSpriteInfo)->Header.ushTransColor * 3;

	wRed = (lpPal[wPal]) >> lpDXDirectDraw->bRDown;
	wGreen = (lpPal[wPal + 1]) >> lpDXDirectDraw->bGDown;
	wBlue = (lpPal[wPal + 2]) >> lpDXDirectDraw->bBDown;


	DDCOLORKEY ColorKey;
	ColorKey.dwColorSpaceLowValue = _RGB16BIT565(wRed, wGreen, wBlue);
	ColorKey.dwColorSpaceHighValue = _RGB16BIT565(wRed, wGreen, wBlue);
	(*lplpSpriteInfo)->Data.lpDDSurface->SetColorKey(DDCKEY_SRCBLT, &ColorKey);


	(*lplpSpriteInfo)->Data.lpDDSurface->Lock(NULL, &Surface, DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, NULL);

	LPBYTE lpbDest = (LPBYTE)Surface.lpSurface;
	LPBYTE lpbSource = pchUnpackData;
	long   lPitch = Surface.lPitch;

	//WORD wPal, wRed, wGreen, wBlue, wColor;


	for (int i = 0; i < wHeight; i++)
	{
		for (int j = 0; j < wWidth; j++)
		{
			wPal = (*(BYTE *)(lpbSource + j)) * 3;
			wRed = (lpPal[wPal]) >> lpDXDirectDraw->bRDown;
			wGreen = (lpPal[wPal + 1]) >> lpDXDirectDraw->bGDown;
			wBlue = (lpPal[wPal + 2]) >> lpDXDirectDraw->bBDown;
			wColor = (wRed << lpDXDirectDraw->bRUp) + (wGreen << lpDXDirectDraw->bGUp) + (wBlue << lpDXDirectDraw->bBUp);

			lpbDest[(j << 1) + 1] = (BYTE)(wColor >> 8);
			lpbDest[(j << 1)] = (BYTE)(wColor & 0xFF);
		}
		lpbDest += lPitch;
		lpbSource += wWidth;
	}
	(*lplpSpriteInfo)->Data.lpDDSurface->Unlock(Surface.lpSurface);
}

void CV2Database::GetOffsetAndDB(LPSTR lpszID, long &dwOffset, char &bDB)
{
	if (!m_bIndexLoaded)
		LoadIndex();

	IdMap::iterator itd = idInfo.find(lpszID);
	if (itd == idInfo.end())
	{
		dwOffset = 0;
		bDB = 0;
		return;
	}
	dwOffset = (*itd).second->dwFileOffset;
	bDB = (*itd).second->dwDataFileIndex;
}



#if 0



DWORD Sum(DWORD *V, int Nb)
{
	DWORD T = 0;
	for (int i = 0; i < Nb + 1; i++)
	{
		T += V[i];
	}
	return T;
}





void Database::GetOffsetAndDBPAL(LPSTR lpszID, long &dwOffset, char &bDB)
{
	if (!boIndexLoad)
		LoadIndex();

	IdMap::iterator i = palInfo.find(lpszID);
	if (i == palInfo.end())
	{
		dwOffset = 0;
		bDB = 0;
		LOG << "* ERR = Cannot find Palette ID " << lpszID << ", NbIndex [" << dwNbIndex << "] \r\n";
		return;
	}

	dwOffset = (*i).second->dwOffset;
	bDB = (*i).second->bVSFnb;
}

bool Database::GetNextPalName(LPSTR lpszID)
{
	if (!boIndexLoad)
		LoadIndex();

	static IdMap::iterator i = palInfo.begin();

	if (i == palInfo.end())
	{
		return false;
	}

	// Remove the palette from the global map.
	strcpy(lpszID, (*i).second->lpszID);
	i++;
	return true;
}










//////////////////////////////////////////////////////////////////////////////////////////


#endif



