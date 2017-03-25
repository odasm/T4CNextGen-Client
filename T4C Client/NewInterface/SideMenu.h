// SideMenu.h: interface for the SideMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIDEMENU_H__9BEF6891_871C_11D3_85A5_00E02922FA40__INCLUDED_)
#define AFX_SIDEMENU_H__9BEF6891_871C_11D3_85A5_00E02922FA40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoxUI.h"
#include "ButtonUI.h"
#include "GraphUI.h"

class SideMenu : public BoxUI  
{
public:	
	 virtual ~SideMenu();

    static SideMenu *GetInstance();

    virtual void Draw( V2SPRITEFX *vsfFX );
    int GetStartOffsetX();


private:
    SideMenu();

    char  m_TMIFile[512];
    int   m_dwCurrentLoadedWorld;
    //int   m_dwLoadZoneXS;
    //int   m_dwLoadZoneYS;

    WORD *m_pTMIData;
    BYTE *m_pTMIMap;
    BYTE *m_pTMIPal;

    void    DrawTMI( void );
    BOOL    CreateTMIMap(WORD*,USHORT,USHORT,USHORT);
    void    LoadTmiWorld(int iWorld,int dwX,int dwY);
    void    SetupViewObject();
    int     SaveCurrentImage8(BYTE *pshImage,BYTE *pPal,int dwWidth, int dwHeight,char *pstrName,BOOL bFlipV);


    GraphUI   m_MainTMI;
    GraphUI   m_MainTMIMask;
    GraphUI   m_MainTMIPPos;

    BOOL      m_bShowThisObjType[__OBJGROUP_LASTGROUP];
};

#endif // !defined(AFX_SIDEMENU_H__9BEF6891_871C_11D3_85A5_00E02922FA40__INCLUDED_)
