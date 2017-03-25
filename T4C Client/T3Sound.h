#ifndef __T3SOUND_H
#define __T3SOUND_H
#include <windows.h>
#include <dsound.h>
#include "NewString.h"
#include <process.h>
#include <string>

#define TS_SOUNDDRIVER 0x0001
#define FLAG DWORD

#define TS_MONO      0x0001
#define TS_STEREO    0x0002

#define TS_STREAMING 0x0001
#define TS_MEMORY    0x0002
#define TS_PLAYING   0x0004

class T3Filter {
	public:
		virtual VOID  			  LoadSegment( DWORD, DWORD, LPBYTE ) = 0;
		virtual DWORD			  GetSize( VOID )							  = 0;
		virtual LPWAVEFORMATEX GetWaveFormatEx( VOID )				  = 0;
};

class T3SoundBuffer {
	public:
		LPDIRECTSOUNDBUFFER lpdsb;
		DWORD dwFilePos;
		DWORD dwLoadSize;
		DWORD dwBufferPos;
		DWORD dwFileSize;
		LPBYTE lpbData;
		DWORD dwType;
		T3Filter *t3Filter;
		CRITICAL_SECTION csInternal;
		
	public:
		T3SoundBuffer( VOID );
		~T3SoundBuffer( VOID );
		VOID Create( T3Filter *, DWORD dwType = TS_MEMORY, DWORD dwBufferTime = 0);
		VOID Play  ( BOOL );
		VOID Stop  ( VOID );
		VOID LoadNextChunck( VOID );
		VOID Release( VOID );
		VOID SetVolume( DWORD );
};

class T3SoundManager {
	friend class T3SoundBuffer;
	friend VOID SoundManagerThread( LPVOID lpParam );

	public:
		CRITICAL_SECTION csInternal;
		T3SoundBuffer *FadeOut;
		int SoundVolume;	  
		int Modifier;
		int MaxVolume;
		FLAG fFlag;
		LPDIRECTSOUND lpds;
		BYTE bStreamingSound;
		T3SoundBuffer *StreamingBuffer[10];
		DWORD dwBufferTime;
		HANDLE hEvent;

	public:
		T3SoundManager(void);
        bool Create( HWND, DWORD, WORD, WORD, std::string &errMsg );
		void FadeOutF(T3SoundBuffer *, int);
		void FadeInF(T3SoundBuffer *, int);
		~T3SoundManager(void);
};

class T3WaveFilter : public T3Filter {
	public:	
		String szFileName;
		MMCKINFO child;
		HMMIO hmfr;
		WAVEFORMATEX wfmtx;

	public:
		T3WaveFilter( VOID );
		~T3WaveFilter( VOID );
		VOID LoadSegment( DWORD, DWORD, LPBYTE );
		LPWAVEFORMATEX GetWaveFormatEx( VOID );
		DWORD GetSize( VOID );
		VOID Create( LPTSTR );
		VOID Release( VOID );
};

class T3WaveSound {
	public:
		T3SoundBuffer sbBuffer;
		T3WaveFilter  wfFilter;

	public:
		VOID Create( LPTSTR lpszFile, DWORD dwType ) { wfFilter.Create( lpszFile ); sbBuffer.Create( &wfFilter, dwType, 1000 ); }
		VOID Play( VOID ) { sbBuffer.Play(TRUE); }
		VOID Stop( VOID ) { sbBuffer.Stop(); }
		VOID Release( VOID ) { sbBuffer.Release(); wfFilter.Release(); }
		VOID SetVolume( DWORD v ) { sbBuffer.SetVolume( v ); }
};

extern T3SoundManager *ug_T3SoundManager;

void PlayCDAudio(int i, BOOL r);
void StopCDAudio(void);
void OpenCDAudio(void);
void CloseCDAudio(void);
BOOL QueryCDAudio(void);

extern DWORD GlobalCDTrack;
extern HWND  GlobalCDHandle;

#endif