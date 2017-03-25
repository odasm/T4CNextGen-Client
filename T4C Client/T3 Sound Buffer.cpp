#include "../T4C Client/MemoryManager.h"
#include "T3Sound.h"
#include "../T4C Client/App.h"

T3SoundBuffer::T3SoundBuffer( VOID ) {
	lpdsb = NULL;
	lpbData = NULL;
	InitializeCriticalSection(&csInternal);
	//int x = 0;//BLBLBL 29 mars 2009 suppression des trucs inutilisés
}

T3SoundBuffer::~T3SoundBuffer( VOID ) {
   if (!g_boQuitApp)
      Release();
	DeleteCriticalSection(&csInternal);
}

VOID T3SoundBuffer::Release( VOID ) {
	if (lpdsb && !g_boQuitApp) {
		lpdsb->Release();
		lpdsb = NULL;
	}

	if (lpbData) {
		delete lpbData;
		lpbData = NULL;
	}
}

VOID T3SoundBuffer::Create(T3Filter *t3filter, DWORD dwtype, DWORD dwBufferTime) {
	if (ug_T3SoundManager->fFlag & TS_SOUNDDRIVER) {

		t3Filter = t3filter;
		dwType = dwtype;
		dwFileSize = t3Filter->GetSize();

		if (dwType & TS_STREAMING) {
	  	   dwLoadSize = (t3Filter->GetWaveFormatEx()->nAvgBytesPerSec * (dwBufferTime * 2)) / 1000;
			ug_T3SoundManager->dwBufferTime = dwBufferTime / 2;
		} else
			dwLoadSize = dwFileSize;

		lpbData = new BYTE [dwLoadSize];

		dwFilePos   = 0;
		dwBufferPos = 0;

		DSBUFFERDESC  dsbdesc;

	   memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
		dsbdesc.dwSize        = sizeof(DSBUFFERDESC);
		                        // Used to be DSBCAPS_CTRLDEFAULT | DSBCAPS_STATIC
        dsbdesc.dwFlags       = (DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY) | DSBCAPS_STATIC;
		dsbdesc.dwBufferBytes = dwLoadSize;
		dsbdesc.lpwfxFormat   = (LPWAVEFORMATEX)t3Filter->GetWaveFormatEx();

	   ug_T3SoundManager->lpds->CreateSoundBuffer(&dsbdesc, &lpdsb, NULL);

		LPVOID Write1;
		LPVOID Write2;
		DWORD  Len1;
		DWORD  Len2;

		t3Filter->LoadSegment(0, dwLoadSize, lpbData);
		
		lpdsb->Lock(0, dwLoadSize, &Write1, &Len1, &Write2, &Len2, 0);
		CopyMemory(Write1, lpbData, Len1);
		if (Write2 != NULL)
			CopyMemory(Write2, lpbData+Len1, Len2);
		lpdsb->Unlock(Write1, Len1, Write2, Len2);

		dwFilePos += dwLoadSize;
	}
}

VOID T3SoundBuffer::LoadNextChunck( VOID ) {
	EnterCriticalSection(&csInternal);

	if (ug_T3SoundManager->fFlag & TS_SOUNDDRIVER && dwType & TS_PLAYING) {
	   static DWORD DebugStartTime = timeGetTime();
	
	   DWORD dwPlay, dwWrite;
		LPVOID Write1;
		LPVOID Write2;
		DWORD  Len1;
		DWORD  Len2;
   
	   lpdsb->GetCurrentPosition(&dwPlay, &dwWrite);

	   DWORD dwLockLen;

	   if (dwBufferPos < dwPlay) {
			dwLockLen = dwPlay - dwBufferPos;
		} else {
			dwLockLen = dwLoadSize - dwBufferPos + dwPlay;
		}

		DWORD dwFileEnd = dwFilePos+dwLockLen;
		if (dwFileEnd > dwFileSize) {
			t3Filter->LoadSegment(dwFilePos, dwFileSize, lpbData);
			t3Filter->LoadSegment(0, dwFileEnd - dwFileSize, lpbData+dwFileSize-dwFilePos);
			dwFilePos = dwFileEnd - dwFileSize;
		} else {
			t3Filter->LoadSegment(dwFilePos, dwFileEnd, lpbData);
			dwFilePos = dwFileEnd;
		}
	
		lpdsb->Lock(dwBufferPos, dwLockLen, &Write1, &Len1, &Write2, &Len2, 0);

		dwBufferPos = (dwBufferPos + dwLockLen) % dwLoadSize;

		CopyMemory(Write1, lpbData, Len1);
 		if (Write2 != NULL)
			CopyMemory(Write2, lpbData+Len1, Len2);
		lpdsb->Unlock(Write1, Len1, Write2, Len2);
	}

	LeaveCriticalSection(&csInternal);
}

VOID T3SoundBuffer::Play( BOOL bRepeat ) {
	EnterCriticalSection(&csInternal);

	if (dwType & TS_STREAMING && !(dwType & TS_PLAYING)) {
		SetEvent(ug_T3SoundManager->hEvent);
		ug_T3SoundManager->StreamingBuffer[ug_T3SoundManager->bStreamingSound] = this;
		ug_T3SoundManager->bStreamingSound++;
		bRepeat = TRUE;
	}

	if ((ug_T3SoundManager->fFlag & TS_SOUNDDRIVER) && lpdsb && (!(dwType & TS_PLAYING) || !bRepeat))  {
		dwType |= TS_PLAYING;
		lpdsb->Play(0, 0, bRepeat ? DSBPLAY_LOOPING : 0);
	}

	LeaveCriticalSection(&csInternal);
}

VOID T3SoundBuffer::Stop(VOID) {
	EnterCriticalSection(&csInternal);

   if ((ug_T3SoundManager && ug_T3SoundManager->fFlag & TS_SOUNDDRIVER) && lpdsb && (dwType & TS_PLAYING)) {
		if (dwType & TS_STREAMING)
			ug_T3SoundManager->bStreamingSound--;
		if (dwType & TS_PLAYING) dwType ^= TS_PLAYING;
		lpdsb->Stop();
   }

	LeaveCriticalSection(&csInternal);
}

VOID T3SoundBuffer::SetVolume( DWORD Volume ) {
	EnterCriticalSection(&csInternal);

   if ((ug_T3SoundManager->fFlag & TS_SOUNDDRIVER) && lpdsb) {
		lpdsb->SetVolume( Volume );
	}

	LeaveCriticalSection(&csInternal);
}