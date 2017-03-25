// AnimUI.h: interface for the AnimUI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMUI_H__5A72D64A_FAD4_11D3_864D_00E02922FA40__INCLUDED_)
#define AFX_ANIMUI_H__5A72D64A_FAD4_11D3_864D_00E02922FA40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ControlUI.h"
#include "GraphUI.h"
#include <string>

// Creates an animates graphic control
class AnimUI : public ControlUI  
{
public:
	AnimUI();
	virtual ~AnimUI();

    virtual void Draw( V2SPRITEFX *vsfFx );

    void AddFrame( std::string frameId ,bool bDrawAlpha = false);
    void LoadSequence( std::string seedId, DWORD frameQty ,bool bSemiTrans = false);

    void Pause( void ){
        stillState = true;   
    };
    void Play( void ){
        stillState = false;
    };
    void Stop( void );        

private:
    vector< GraphUI * > frames;
    DWORD currentFrame;
    bool stillState;
};

#endif // !defined(AFX_ANIMUI_H__5A72D64A_FAD4_11D3_864D_00E02922FA40__INCLUDED_)
