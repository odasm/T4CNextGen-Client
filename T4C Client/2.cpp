/* this file is a part of amp software, (C) tomislav uzelac 1996,1997

	Origional code by: tomislav uzelac
	Modified by:
	* Dan Nelson - BSD mods.
	* Andrew Richards - moved code from audio.c and added mixer support etc

 */


#include <windows.h>
#include <process.h>
#include <mmsystem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "audioIO.h"
#include "global.h"
#include "MemoryManager.h"

#define WAV_BUFS (32)
#define WAV_BUFL (16384)

/* declare these static to effectively isolate the audio device */

static int audio_fd;
static int mixer_fd;
static int volumeIoctl;
HWAVEOUT audioHandler;
LPWAVEHDR pwh;
HANDLE pipeRead,pipeWrite;
int freeBuffers=WAV_BUFS;

/* audioOpen() */
/* should open the audio device, perform any special initialization		 */
/* Set the frequency, no of channels and volume. Volume is only set if */
/* it is not -1 */



__inline int read_safe(HANDLE filed,char *buff,int size) {
	int i;
	DWORD bread;

	for (i=0;i<size;i+=bread) {
		ReadFile(filed,buff+i,size-i,&bread,NULL);
	}
	return(0);
}



ULONG WINAPI play_thread(LPVOID lpParam) {
	int i;

   waveOutPause(audioHandler);
   for(i=0;i<WAV_BUFS;i++) {
      while(freeBuffers==0) Sleep(100);
      read_safe(pipeRead,pwh[i].lpData,WAV_BUFL);
      freeBuffers--;
      waveOutWrite(audioHandler,pwh+i,sizeof(WAVEHDR));
   }
   waveOutRestart(audioHandler);
   while(1) {
      for(i=0;i<WAV_BUFS;i++) {
         while(freeBuffers==0) Sleep(100);
         read_safe(pipeRead,pwh[i].lpData,WAV_BUFL);
         freeBuffers--;
         waveOutWrite(audioHandler,pwh+i,sizeof(WAVEHDR));
      }
   }

   return 1;
}



void CALLBACK audioProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2){
	if(uMsg==WOM_DONE) {
		freeBuffers++;
	}
}


void audioOpen(int frequency, int stereo, int volume)
{
	int i;
	char *pwd;
	WAVEFORMATEX outFormatex;

	outFormatex.wFormatTag=WAVE_FORMAT_PCM;
	outFormatex.nChannels=(stereo ? 2 : 1);
	outFormatex.nSamplesPerSec=frequency;
	outFormatex.nAvgBytesPerSec=frequency*(stereo ? 2 : 1)*16/8;
	outFormatex.nBlockAlign=(stereo ? 2 : 1)*16/8;
	outFormatex.wBitsPerSample=16;

	waveOutOpen(&audioHandler, WAVE_MAPPER, &outFormatex, (DWORD)audioProc, 0, CALLBACK_FUNCTION);

	pwh=(LPWAVEHDR)GlobalLock(GlobalAlloc(GPTR, sizeof(WAVEHDR)*WAV_BUFS+WAV_BUFL*WAV_BUFS));
	for(int i=0;i<WAV_BUFS;i++) {
		pwd=((char*)(pwh))+sizeof(WAVEHDR)*WAV_BUFS+i*WAV_BUFL;
		pwh[i].lpData=pwd;
		pwh[i].dwBufferLength=WAV_BUFL;
		waveOutPrepareHeader(audioHandler,pwh+i,sizeof(WAVEHDR));
	}

	CreatePipe(&pipeRead,&pipeWrite,NULL,WAV_BUFS*WAV_BUFL);

	DWORD ThreadID;
   CreateThread(NULL, 0, play_thread, NULL, 0, &ThreadID);
}


/* audioSetVolume - only code this if your system can change the volume while */
/*									playing. sets the output volume 0-100 */

void
audioSetVolume(int volume)
{
}


/* audioClose() */
/* should close the audio device and perform any special shutdown */

void
audioClose()
{
	int i;

	while(freeBuffers<WAV_BUFS) Sleep(10);
	for(int i=0;i<WAV_BUFS;i++)
		waveOutUnprepareHeader(audioHandler,pwh+i,sizeof(WAVEHDR));
	waveOutClose(audioHandler);
	GlobalFree(GlobalHandle(pwh));
}


/* audioWrite */
/* writes count bytes from buffer to the audio device */
/* returns the number of bytes actually written */

int
audioWrite(char *buffer, int count)
{
	DWORD bwrote;

	WriteFile(pipeWrite,buffer,count,&bwrote,NULL);

	return(bwrote);
}

