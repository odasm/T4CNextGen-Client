// NMDShow.h: interface for the CNMDShow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMDSHOW_H__CC9DFEC4_CA44_484E_9440_B5480F37FAD5__INCLUDED_)
#define AFX_NMDSHOW_H__CC9DFEC4_CA44_484E_9440_B5480F37FAD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\DirectX8Lib\include\dshow.h"

class CNMDShow  
{
public:
	CNMDShow(HWND hParent);
	virtual ~CNMDShow();

   int PlayVideoFile(char *pstrFileName);
   void CloseInterfaces(void);
   BOOL CheckCompleteEvent();
   HRESULT Play();
   void RefreshPreview();
protected:
   void CheckVisibility(void);

   

protected:
   HWND m_hParent;
   BOOL m_bAudioOnly;

   // DirectShow interfaces
   IGraphBuilder   *m_pGB;
   IMediaControl   *m_pMC;
   IMediaEventEx   *m_pME;
   IVideoWindow    *m_pVW;
   IBasicAudio     *m_pBA;
   IBasicVideo     *m_pBV;
   IMediaSeeking   *m_pMS;
   IMediaPosition  *m_pMP;

};

#endif // !defined(AFX_NMDSHOW_H__CC9DFEC4_CA44_484E_9440_B5480F37FAD5__INCLUDED_)
