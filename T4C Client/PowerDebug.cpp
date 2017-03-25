// PowerDebug.cpp: implementation of the PowerDebug class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <windows.h>
#include "PowerDebug.h"
#include "IDirectXDraw.h"
#include "WarningBox.h"
#include <stdlib.h> 
#include <mmsystem.h>
#include <stdio.h>
#include "Global.h"
#include "MemoryManager.h"
#include "DebugLoggerAPI.h"

//#define __LOG__  //BLBLBL 26 mars 2009 : mode LOG activé.

PowerLog::PowerLog(void)
#ifdef __LOG__
: enabled( true )
#else
: enabled( false )
#endif
{
    logToMonitor = false;
    InitializeCriticalSection(&csInternal);
}

PowerLog::~PowerLog(void) {
	DeleteCriticalSection(&csInternal);
}

PowerLog &PowerLog::operator << (char *s) {
    if( enabled ){
        if( logToMonitor ){
            DebugLoggerAPI::GetInstance()->Log( s );
        }else{
            EnterCriticalSection(&csInternal);
	        OutputDebugString(s);
	        FILE *File;
            File = fopen("DEBUG.LOG", "ab+");
            fseek(File, 0, SEEK_END);
	        fwrite(s, 1, strlen((const char *)s), File); 
            fclose(File);  
            LeaveCriticalSection(&csInternal);
        }
    }
                                                  
	return *this;
}

PowerLog &PowerLog::operator << (__int64 i) {
    if( enabled ){
        if( logToMonitor ){
            char Temp[ 100 ];
            DebugLoggerAPI::GetInstance()->Log(_i64toa(i, Temp, 10));
        }else{
            EnterCriticalSection(&csInternal);
	        char Temp[100];
	        OutputDebugString(_i64toa(i, Temp, 10));
	        FILE *File;
            File = fopen("DEBUG.LOG", "ab+");
            fseek(File, 0, SEEK_END);
	        fwrite(_i64toa(i, Temp, 10), 1, strlen(_i64toa(i, Temp, 10)), File); 
	        fclose(File); 
	        LeaveCriticalSection(&csInternal); 
        }
    }

	return *this; 
}

void PowerLog::Lock(void) {
    if( enabled ){
        //EnterCriticalSection(&csInternal);
    }
}

void PowerLog::Unlock(void) {
    if( enabled ){
        //LeaveCriticalSection(&csInternal);
    }
}

void PowerLog::Enable( bool newState, bool logToMonitor ){
    enabled = newState;
    this->logToMonitor = logToMonitor;
}


PowerLog LOG;