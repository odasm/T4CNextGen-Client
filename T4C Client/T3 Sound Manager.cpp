#include "../T4C Client/MemoryManager.h"
#include "T3Sound.h"
#include "../T4C Client/App.h"

T3SoundManager *ug_T3SoundManager = NULL;

DWORD ThreadJTimeStamp = 0;

VOID SoundManagerThread( LPVOID lpParam ) {
	g_App.StartThread(APP_SOUND_THREAD);

   int i;
	DWORD sTime, eTime, lTime;
	sTime = timeGetTime();
	while ( !g_boQuitApp ) {
      Sleep(25);
		eTime = timeGetTime();
		lTime = (eTime - sTime);
		if (lTime > ug_T3SoundManager->dwBufferTime)
			lTime = ug_T3SoundManager->dwBufferTime;
		WaitForSingleObject(ug_T3SoundManager->hEvent, ug_T3SoundManager->dwBufferTime-lTime);
      if ( !g_boQuitApp ) {
         sTime = timeGetTime();
		   DWORD Start, End;
         Start = timeGetTime();
         for (i = 0; i < ug_T3SoundManager->bStreamingSound; i++) {
   			ug_T3SoundManager->StreamingBuffer[i]->LoadNextChunck();
		   }
         End = timeGetTime();
         ThreadJTimeStamp = End - Start;
      }

	  //BLBLBL	fusion des 2 threads
	  EnterCriticalSection(&ug_T3SoundManager->csInternal);
		if (ug_T3SoundManager->FadeOut) {
			ug_T3SoundManager->FadeOut->SetVolume(ug_T3SoundManager->SoundVolume);
			ug_T3SoundManager->SoundVolume += ug_T3SoundManager->Modifier;
			if (ug_T3SoundManager->Modifier < 0) {
				if (ug_T3SoundManager->SoundVolume < -2000) {
					ug_T3SoundManager->FadeOut->Stop();
					ug_T3SoundManager->FadeOut->Release();
					ug_T3SoundManager->FadeOut = NULL;
				}
			} else {
				if (ug_T3SoundManager->SoundVolume > ug_T3SoundManager->MaxVolume) {
					ug_T3SoundManager->FadeOut = NULL;	  
				}
			}
		} 
		LeaveCriticalSection(&ug_T3SoundManager->csInternal);

	}
	g_App.ExitThread(APP_SOUND_THREAD);
	_endthreadex(1);
}

VOID SoundControlThread( LPVOID lpParam ) {
	g_App.StartThread(APP_VOLUME_THREAD);
	while ( !g_boQuitApp ) {
		Sleep(50);
/*		EnterCriticalSection(&ug_T3SoundManager->csInternal);
		if (ug_T3SoundManager->FadeOut) {
			ug_T3SoundManager->FadeOut->SetVolume(ug_T3SoundManager->SoundVolume);
			ug_T3SoundManager->SoundVolume += ug_T3SoundManager->Modifier;
			if (ug_T3SoundManager->Modifier < 0) {
				if (ug_T3SoundManager->SoundVolume < -2000) {
					ug_T3SoundManager->FadeOut->Stop();
					ug_T3SoundManager->FadeOut->Release();
					ug_T3SoundManager->FadeOut = NULL;
				}
			} else {
				if (ug_T3SoundManager->SoundVolume > ug_T3SoundManager->MaxVolume) {
					ug_T3SoundManager->FadeOut = NULL;	  
				}
			}
		} 
		LeaveCriticalSection(&ug_T3SoundManager->csInternal);*/
	}
	g_App.ExitThread(APP_VOLUME_THREAD);
	_endthreadex(1);
}

T3SoundManager::T3SoundManager(void) {
	FadeOut = NULL;
	InitializeCriticalSection(&csInternal);
	hEvent = NULL;
   lpds = NULL;
}

T3SoundManager::~T3SoundManager(void) {
	DeleteCriticalSection(&csInternal);
	CloseHandle(hEvent);
   lpds->Release();
}

VOID T3SoundManager::FadeOutF(T3SoundBuffer *s, int Volume) {
	EnterCriticalSection(&csInternal);
	SoundVolume = Volume;
	Modifier = -50;
	if (FadeOut) {
		FadeOut->Stop();
	}
	
	FadeOut = s;
	LeaveCriticalSection(&csInternal);
}

VOID T3SoundManager::FadeInF(T3SoundBuffer *s, int Volume) {
	EnterCriticalSection(&csInternal);
	SoundVolume = -2000;
	MaxVolume = Volume;
	Modifier = 50;
	if (FadeOut) {
		FadeOut->Stop();
	}
	FadeOut = s;
	FadeOut->SetVolume(SoundVolume);
	FadeOut->Play(TRUE);
	LeaveCriticalSection(&csInternal);
}

#define LOGDSERR( dserr ) case dserr: errMsg += #dserr; break;
bool T3SoundManager::Create( HWND hWnd, DWORD nSamplesPerSec, WORD wBitsPerSample, 
                            WORD nChannels, std::string &errMsg ) {
	ug_T3SoundManager = this;

	DSBUFFERDESC  dsbdesc;
	LPDIRECTSOUNDBUFFER lpdsbPrimary;
	WAVEFORMATEX wfmtx;

	fFlag = FALSE;
    HRESULT hr = DirectSoundCreate(NULL, &lpds, NULL);
    if( hr == DS_OK) {
		fFlag |= TS_SOUNDDRIVER;
	}

    if (!(fFlag & TS_SOUNDDRIVER)){
        errMsg = "DirectSoundCreate was not sucessful: hr=";
        switch( hr ){
        LOGDSERR( DSERR_ALLOCATED );
        LOGDSERR( DSERR_INVALIDPARAM );
        LOGDSERR( DSERR_NOAGGREGATION );
        LOGDSERR( DSERR_NODRIVER );
        LOGDSERR( DSERR_OUTOFMEMORY );
        default:
            char buf[ 256 ];
            errMsg += "Unknown value ";
            errMsg += itoa( hr, buf, 10 );
            break;
        }        
		return false;
    }

	lpds->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);

   wfmtx.wFormatTag = 1;
   wfmtx.nChannels = nChannels;
   wfmtx.nSamplesPerSec = nSamplesPerSec;
   wfmtx.nAvgBytesPerSec = nSamplesPerSec * (wBitsPerSample >> 3);
   wfmtx.nBlockAlign = (wBitsPerSample >> 3);
   wfmtx.wBitsPerSample = wBitsPerSample;
   wfmtx.cbSize = 0;

	memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
	dsbdesc.dwSize        = sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags       = DSBCAPS_PRIMARYBUFFER;
   dsbdesc.dwBufferBytes = 0;
	dsbdesc.lpwfxFormat   = NULL;
   //dsbdesc.guid3DAlgorithm = GUID_NULL;
	
   hr = lpds->CreateSoundBuffer(&dsbdesc, &lpdsbPrimary, NULL);
   if( hr != DS_OK ){
       errMsg = "Could not create primary sound buffer. hr=";
       switch( hr ){
       LOGDSERR( DSERR_ALLOCATED )
       LOGDSERR( DSERR_CONTROLUNAVAIL )
       LOGDSERR( DSERR_BADFORMAT )
       LOGDSERR( DSERR_INVALIDPARAM )
       LOGDSERR( DSERR_NOAGGREGATION )  
       LOGDSERR( DSERR_OUTOFMEMORY )
       LOGDSERR( DSERR_UNINITIALIZED )
       LOGDSERR( DSERR_UNSUPPORTED )
       default:
           char buf[ 256 ];
           errMsg += "Unknown value ";
           errMsg += itoa( hr, buf, 10 );
           break;
       }
   	 fFlag = FALSE;
       return false;
   }

   lpdsbPrimary->SetFormat(&wfmtx);


	hEvent = CreateEvent(NULL, FALSE, FALSE, "Sound Manager Event");
	bStreamingSound = 0;

	HANDLE SMID;
	SMID = (HANDLE)_beginthread(SoundManagerThread, 0, NULL);
	SetThreadPriority(SMID, THREAD_PRIORITY_ABOVE_NORMAL);
	//HANDLE SCID;
	//SCID = (HANDLE)_beginthread(SoundControlThread, 0, NULL);
   return true;
}

