// RTMap.h: interface for the RTMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RTMAP_H__INCLUDED_)
#define AFX_RTMAP_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoxUI.h"
#include "ButtonUI.h"
#include "GraphUI.h"

class RTMap : public BoxUI  
{
public:	
	virtual ~RTMap();

    static RTMap *GetInstance();

    virtual void Draw( V2SPRITEFX *vsfFX );
    void Hide();

private:
    RTMap();

    WORD *m_pTMIData;
    WORD *m_pTMIDataTmp;
    BYTE *m_pMapImage;
    BYTE *m_pMapImageM;
    BYTE *m_pRTMapS;
    BYTE *m_pRTPal;

    char m_RTFile[512];
    int  m_dwCurrentLoadedWorld;

    void    DrawMap       (void);
    BOOL    CreateRTMap   (WORD*,USHORT,USHORT,USHORT);
    void    LoadRTWorld   (int iWorld,int dwX,int dwY);
    void    LoadMapZone   (BYTE *pMap,BYTE *pData,int dwX, int dwY,int dwW,int dwH);

    HRESULT CreateMapMask (BYTE *pData,int ptMap[15][11],int dwW,int dwH);
    int SaveCurrentImage8(BYTE *pshImage,BYTE *pPal,int dwWidth, int dwHeight,char *pstrName,BOOL bFlipV);
    


    GraphUI   m_MainMap;
    GraphUI   m_MainMapMask;
    GraphUI   m_MainMapPPos;
    GraphUI   m_rtMask;

    int       m_dwLoadX;
    int       m_dwLoadY;
    int       m_dwLoadW;

    //int       m_dwLoadZoneXS;
    //int       m_dwLoadZoneYS;
};

#endif // !defined(AFX_RTMAP_H__INCLUDED_)
