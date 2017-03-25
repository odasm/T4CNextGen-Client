// NMDShow.cpp: implementation of the CNMDShow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "t4clauncher.h"
#include "NMDShow.h"
#include <tchar.h>
#include <atlbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//
// Macros
//
#define REGISTER_FILTERGRAPH

#define SAFE_RELEASE(x) { if (x) x->Release(); x = NULL; }

#define JIF(x) if (FAILED(hr=(x))) \
    {return hr;}

#define LIF(x) if (FAILED(hr=(x))) \
    {}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNMDShow::CNMDShow(HWND hParent)
{
   m_hParent = hParent;

   m_pGB = NULL;
   m_pMC = NULL;
   m_pME = NULL;
   m_pVW = NULL;
   m_pBA = NULL;
   m_pBV = NULL;
   m_pMS = NULL;
   m_pMP = NULL;

   m_bAudioOnly = FALSE;
   CoInitialize(NULL);
}

CNMDShow::~CNMDShow()
{
   CloseInterfaces();
   CoUninitialize();
}

int CNMDShow::PlayVideoFile(char *pstrFileName)
{

    USES_CONVERSION;
    WCHAR wFile[MAX_PATH];
    HRESULT hr;

    // Convert filename to wide character string
    wcscpy(wFile, T2W(pstrFileName));

    // Get the interface for DirectShow's GraphBuilder
    JIF(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                         IID_IGraphBuilder, (void **)&m_pGB));

    if(SUCCEEDED(hr))
    {
        // Have the graph builder construct its the appropriate graph automatically
        JIF(m_pGB->RenderFile(wFile, NULL));

        // QueryInterface for DirectShow interfaces
        JIF(m_pGB->QueryInterface(IID_IMediaControl, (void **)&m_pMC));
        JIF(m_pGB->QueryInterface(IID_IMediaEventEx, (void **)&m_pME));
        JIF(m_pGB->QueryInterface(IID_IMediaSeeking, (void **)&m_pMS));
        JIF(m_pGB->QueryInterface(IID_IMediaPosition, (void **)&m_pMP));

        // Query for video interfaces, which may not be relevant for audio files
        JIF(m_pGB->QueryInterface(IID_IVideoWindow, (void **)&m_pVW));
        JIF(m_pGB->QueryInterface(IID_IBasicVideo, (void **)&m_pBV));

        // Query for audio interfaces, which may not be relevant for video-only files
        JIF(m_pGB->QueryInterface(IID_IBasicAudio, (void **)&m_pBA));

        // Is this an audio-only file (no video component)?
        CheckVisibility();

        // Have the graph signal event via window callbacks for performance
        //JIF(m_pME->SetNotifyWindow((OAHWND)m_hParent, WM_GRAPHNOTIFY, 0));

        if (!m_bAudioOnly) 
        {
           RECT rc;
           ::GetWindowRect( m_hParent, &rc );
           JIF(m_pVW->put_Owner((LONG_PTR)m_hParent));
           JIF(m_pVW->put_MessageDrain((LONG_PTR)m_hParent));
           JIF(m_pVW->put_Left(0));
           JIF(m_pVW->put_Top(0));
           JIF(m_pVW->put_Width( (rc.right - rc.left)));
           JIF(m_pVW->put_Height( (rc.bottom - rc.top) ));
           JIF(m_pVW->put_WindowStyle( WS_CHILD| WS_CLIPCHILDREN | WS_CLIPSIBLINGS));
           JIF(m_pVW->put_Visible( OATRUE));
        }

        JIF(m_pMC->Run());
    }
    return hr;
}

void CNMDShow::CloseInterfaces(void)
{
    HRESULT hr;

    if(m_pMC)
        hr = m_pMC->Stop();

    // Relinquish ownership (IMPORTANT!) after hiding video window
    if(m_pVW)
    {
        hr = m_pVW->put_Visible(OAFALSE);
        hr = m_pVW->put_Owner(NULL);
    }

    // Disable event callbacks
    if (m_pME)
        hr = m_pME->SetNotifyWindow((OAHWND)NULL, 0, 0);

    // Release and zero DirectShow interfaces
    SAFE_RELEASE(m_pME);
    SAFE_RELEASE(m_pMS);
    SAFE_RELEASE(m_pMP);
    SAFE_RELEASE(m_pMC);
    SAFE_RELEASE(m_pBA);
    SAFE_RELEASE(m_pBV);
    SAFE_RELEASE(m_pVW);
    SAFE_RELEASE(m_pGB);
}



void CNMDShow::CheckVisibility(void)
{
    long lVisible;
    HRESULT hr;

    if ((!m_pVW) || (!m_pBV))
    {
        // Audio-only files have no video interfaces.  This might also
        // be a file whose video component uses an unknown video codec.
        m_bAudioOnly = TRUE;
        return;
    }
    else
    {
        // Clear the global flag
        m_bAudioOnly = FALSE;
    }

    hr = m_pVW->get_Visible(&lVisible);
    if (FAILED(hr))
    {
        // If this is an audio-only clip, get_Visible() won't work.
        //
        // Also, if this video is encoded with an unsupported codec,
        // we won't see any video, although the audio will work if it is
        // of a supported format.
        //
        if (hr == E_NOINTERFACE)
        {
            m_bAudioOnly = TRUE;
        }
    }
}

BOOL CNMDShow::CheckCompleteEvent()
{
   LONG evCode, evParam1, evParam2;
   HRESULT hr=S_OK;
   
   // Make sure that we don't access the media event interface
   // after it has already been released.
   if (!m_pME)
      return TRUE;
   
   // Process all queued events
   while(SUCCEEDED(m_pME->GetEvent(&evCode, (LONG_PTR *) &evParam1,
      (LONG_PTR *) &evParam2, 0)))
   {
      // Free memory associated with callback, since we're not using it
      hr = m_pME->FreeEventParams(evCode, evParam1, evParam2);
      
      // If this is the end of the clip, reset to beginning
      if(EC_COMPLETE == evCode)
         return TRUE;
   }
   return FALSE;
}

HRESULT CNMDShow::Play()
{
   HRESULT hr=S_OK;
   // Run the graph to play the media file
   JIF(m_pMC->Run());
   return hr;
}

void CNMDShow::RefreshPreview()
{

   ::PostMessage(m_hParent,WM_PAINT,0,0);
}