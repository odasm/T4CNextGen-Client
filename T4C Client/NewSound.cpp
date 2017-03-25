#include "pch.h"
#include <windows.h>
#include "NewSound.h"
#include "Global.h"
#include "Packet.h"
#include "MemoryManager.h"
#include "App.h"
#include "SaveGame.h"
extern CSaveGame g_SaveGame;


T3VSBFilter::T3VSBFilter( VOID ) {
	dwIdxPos = 0;
	lpvsbInfo = NULL;
}

T3VSBFilter::~T3VSBFilter( VOID ) {
	Release();
}


LPWAVEFORMATEX T3VSBFilter::GetWaveFormatEx( VOID ) {
	if (ug_T3SoundManager->fFlag & TS_SOUNDDRIVER) {
		return &wfmtx;
	}
	return NULL;
}

DWORD T3VSBFilter::GetSize( VOID ) {
	if (ug_T3SoundManager->fFlag & TS_SOUNDDRIVER) {
		return lpvsbInfo->dwSize;
	}
	return 0;
}

VOID T3VSBFilter::Release( VOID ) {
   if (ug_T3SoundManager && ug_T3SoundManager->fFlag & TS_SOUNDDRIVER) {
      if (dwIdxPos) {
         DatabaseReleaseVSB(dwIdxPos);
         dwIdxPos = 0;
      }
	}
}

VOID T3VSBFilter::LoadSegment( DWORD dwStartPos, DWORD dwEndPos, LPBYTE lpbData ) {
	if (ug_T3SoundManager->fFlag & TS_SOUNDDRIVER) {
	   DatabaseLoadVSBChunck(dwIdxPos, &lpvsbInfo, dwStartPos, dwEndPos-dwStartPos, lpbData);
	}
}

VOID T3VSBFilter::Create( LPTSTR lpszUserID, DWORD dwType ) {
	if (ug_T3SoundManager->fFlag & TS_SOUNDDRIVER) {
	   dwIdxPos = DatabaseLoadVSB(lpszUserID, &lpvsbInfo);
      if (dwIdxPos == 0xFFFFFFFF)
         dwIdxPos = DatabaseLoadVSB("General error sound", &lpvsbInfo);

	   wfmtx.wFormatTag      = WAVE_FORMAT_PCM;
		wfmtx.nChannels       = dwType == TS_STREAMING ? 2 : 1;
	   wfmtx.nSamplesPerSec  = lpvsbInfo->wHertz;
	   wfmtx.nAvgBytesPerSec = (lpvsbInfo->bBits / 8) * lpvsbInfo->wHertz * wfmtx.nChannels;
	   wfmtx.nBlockAlign     = lpvsbInfo->bBits / 8 * wfmtx.nChannels;
	   wfmtx.wBitsPerSample  = lpvsbInfo->bBits;
		wfmtx.cbSize          = 0;
		if (dwType == TS_STREAMING) {
			LOG << "SoundName [" << lpszUserID << "] Info [" << lpvsbInfo->wHertz << ", " << lpvsbInfo->bBits << ", " << wfmtx.nChannels << "]\r\n";
		}
	}
};

T3VSBSound::T3VSBSound( VOID ) { 
   Load = FALSE; 
   dwCount = 0;
   currentlyPlaying = false;
}

T3VSBSound::~T3VSBSound( VOID ) { 
   Release(TRUE);
}


VOID T3VSBSound::Create( LPTSTR lpszFile, DWORD dwType) 
{ 
   /*
   if (Custom.gNoMusic) 
   // Si l'option de pas decompresser la music est TRUE.
      return; 
   */

   dwCount++;

   if (!Load) {
   // Si c pas deja loader, on load.
      strcpy(SoundName, lpszFile);
      
      Load = TRUE; 
      wfFilter.Create( lpszFile, dwType ); 
      sbBuffer.Create( &wfFilter, dwType, 1000 ); 
      SetVolume(10);
   } 
}

VOID T3VSBSound::Play( BOOL f ) 
{
   /*
   if (Custom.gNoMusic)
   // Si l'option de pas decompresser la music est TRUE.
      return; 
   */

   if (!Load)
   // Si le dessins est pas loader
      return;

 
   if (g_Var.inGame) {
      if (g_SaveGame.GetOptionsParam()->dwSoundVol && sbBuffer.dwType & TS_MEMORY) {
         SetVolume(g_SaveGame.GetOptionsParam()->dwSoundVol);
         sbBuffer.Play((sbBuffer.dwType & TS_STREAMING) ? TRUE : FALSE); 
         currentlyPlaying = true;
      } else if (sbBuffer.dwType & TS_STREAMING) {
         sbBuffer.Play((sbBuffer.dwType & TS_STREAMING) ? TRUE : FALSE);
         currentlyPlaying = true;
      }
   } 
   else if (sbBuffer.dwType & TS_STREAMING) 
   {
      sbBuffer.Play((sbBuffer.dwType & TS_STREAMING) ? TRUE : FALSE);
      currentlyPlaying = true;
   }
   else if(f==2)
   {
      sbBuffer.Play(FALSE);
      currentlyPlaying = true;
   }
}

VOID T3VSBSound::Stop( BOOL f ) 
{
   /*if (Custom.gNoMusic) 
   // Si l'option de pas decompresser la music est TRUE.
      return; 
   */
   
   if (!Load)
   // Si le dessins est pas loader
      return;

      sbBuffer.Stop(); 
      currentlyPlaying = false;
   /*
   }*/ 
}

VOID T3VSBSound::SetVolume( DWORD v ) 
{ 
   /*
   if (Custom.gNoMusic) 
   // Si l'option de pas decompresser la music est TRUE.
      return; 
   */
   
   if (!Load)
   // Si le dessins est pas loader
      return;

   if (v == 0) { 
   // Si le volume est a 0.
      Stop(FALSE); 
   } else { 
      sbBuffer.SetVolume( -166 * (10 - v) ); 
   } 
}

VOID T3VSBSound::CopyWave( T3VSBSound *vsb ) 
{
   /*
   if (Custom.gNoMusic) 
   // Si l'option de pas decompresser la music est TRUE.
      return; 
   */

   if (ug_T3SoundManager->fFlag & TS_SOUNDDRIVER) { 
      Load = TRUE;
      dwCount++;
      
      strcpy(SoundName, "Copy of ");
      strcat(SoundName, vsb->SoundName);

      ug_T3SoundManager->lpds->DuplicateSoundBuffer(vsb->sbBuffer.lpdsb, &(sbBuffer.lpdsb)); 
      sbBuffer.dwType = TS_MEMORY; 
   } 
}

VOID T3VSBSound::Release( BOOL bForce ) 
{
   /*
   if (Custom.gNoMusic) 
   // Si l'option de pas decompresser la music est TRUE.
      return; 
   */
   
   if (!Load) {
      dwCount = 0;
      return;
   }
   
   dwCount--;
  
   if (!dwCount || bForce) {

      if (bForce)
         dwCount = 0;
      sbBuffer.Release(); 
      wfFilter.Release(); 
      Load = FALSE; 
      currentlyPlaying = false;
   }
}

bool T3VSBSound::isPlaying(void) {
   return currentlyPlaying;
}