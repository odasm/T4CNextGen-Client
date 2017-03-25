#include "../T4C Client/MemoryManager.h"
#include "T3Sound.h"

LPWAVEFORMATEX T3WaveFilter::GetWaveFormatEx( VOID ) {
	return &wfmtx;
}

DWORD T3WaveFilter::GetSize( VOID ) {
	return child.cksize;
}

VOID T3WaveFilter::Release( VOID ) {
	if (hmfr) {
		mmioClose(hmfr, 0);
		hmfr = NULL;
		Sleep(100);
	}
}

T3WaveFilter::T3WaveFilter( VOID ) {
	hmfr = NULL;
}

T3WaveFilter::~T3WaveFilter( VOID ) {
	Release();
}

VOID T3WaveFilter::LoadSegment( DWORD dwStartPos, DWORD dwEndPos, LPBYTE lpbData ) {
	mmioSeek(hmfr, dwStartPos, SEEK_SET);
	mmioRead(hmfr, (char *)lpbData, dwEndPos-dwStartPos);
}

VOID T3WaveFilter::Create( LPTSTR lpszUserFileName ) {
	MMCKINFO		 parent;

	parent.ckid		     = (FOURCC)0;
	parent.cksize       = 0;
	parent.fccType      = (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags      = 0;

	child = parent;

	hmfr = mmioOpen(lpszUserFileName, NULL, MMIO_READ | MMIO_ALLOCBUF);

	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmioDescend(hmfr, &parent, NULL, MMIO_FINDRIFF);

	child.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hmfr, &child, &parent, 0);

	mmioRead(hmfr, (char *)&wfmtx, sizeof(wfmtx));

	mmioAscend(hmfr, &child, 0);

	child.ckid = mmioFOURCC('d', 'a', 't', 'a');

	mmioDescend(hmfr, &child, &parent, MMIO_FINDCHUNK);
};
