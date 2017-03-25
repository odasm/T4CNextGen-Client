#ifndef __NEWSOUND
#define __NEWSOUND

#include "T3Sound.h"//BLBLBL 27 mars 2009 réintégré le son dans le projet..
#include "database2.h"
#include "PowerDebug.h"
#include "MacroHandler.h"

class T3VSBFilter : public T3Filter {
	private:
		LPVSBINFO lpvsbInfo;
		WAVEFORMATEX wfmtx;
		DWORD dwIdxPos;

	public:
		~T3VSBFilter( VOID );
		T3VSBFilter( VOID );
		VOID LoadSegment( DWORD, DWORD, LPBYTE );
		LPWAVEFORMATEX GetWaveFormatEx( VOID );
		DWORD GetSize( VOID );
		VOID Create( LPTSTR, DWORD );
		VOID Release( VOID );
};

class T3VSBSound {
	public:
		T3SoundBuffer sbBuffer;
		T3VSBFilter  wfFilter;
		BOOL Load;
      DWORD dwCount;
      char SoundName[100];
      bool currentlyPlaying;

	public:
		T3VSBSound( VOID );
		~T3VSBSound( VOID );
		VOID Create( LPTSTR lpszFile, DWORD dwType);
		VOID Play( BOOL f );
		VOID Stop( BOOL f );
		VOID SetVolume( DWORD v );
		VOID CopyWave( T3VSBSound *vsb );
		VOID Release( BOOL );
      bool isPlaying(void);
};

#endif