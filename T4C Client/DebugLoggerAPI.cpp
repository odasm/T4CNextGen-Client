// DebugLoggerAPI.cpp: implementation of the DebugLoggerAPI class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "DebugLoggerAPI.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace{
    const int MapFileSize = 20480;
    const int BufferSize  = MapFileSize - sizeof( DWORD );

    HANDLE hDebugLoggerMutex;
    HANDLE hDebugLoggerEvent;
    HANDLE hFileMap;

    struct View{
        DWORD nextWritePos;
        char  buffer[ BufferSize ];        
    } *sharedView;
    
    struct BufItem{
        DWORD processId;
        DWORD threadId;
    };
}

DebugLoggerAPI::DebugLoggerAPI()
{
    hDebugLoggerEvent = CreateEvent( NULL, FALSE, FALSE, "DebugLoggerAPI Event" );
    hDebugLoggerMutex = CreateMutex( NULL, FALSE, "DebugLoggerAPI Mutex" );

    // Get the mutex now.
    WaitForSingleObject( hDebugLoggerMutex, INFINITE );

    hFileMap = CreateFileMapping( 
        reinterpret_cast< HANDLE >( 0xFFFFFFFF ), 
        NULL, 
        PAGE_READWRITE,
        0,
        MapFileSize,
        "DebugLoggerAPI ShareSpace"
    );

    sharedView = reinterpret_cast< View * >( MapViewOfFile( hFileMap, FILE_MAP_WRITE, 0, 0, MapFileSize ) );

    sharedView->buffer[ 0 ] = 0;
    sharedView->buffer[ 1 ] = 0;
    sharedView->nextWritePos = 0;

    ReleaseMutex( hDebugLoggerMutex );
}

DebugLoggerAPI::~DebugLoggerAPI()
{
    CloseHandle( hDebugLoggerEvent );
    CloseHandle( hDebugLoggerMutex );

    UnmapViewOfFile( sharedView );
    CloseHandle( hFileMap );
}

DebugLoggerAPI *DebugLoggerAPI::GetInstance(){
    static DebugLoggerAPI logger;
    return &logger;
}


//////////////////////////////////////////////////////////////////////////////////////////
void DebugLoggerAPI::Log
//////////////////////////////////////////////////////////////////////////////////////////
// Logs a string.
// 
(
 const char *sz // The string to log.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If the length of this string is bigger then the mapped memory space.
    int len = strlen( sz );
    if( len + 3 > MapFileSize || len == 0 ){
        return;
    }

    // Wait for the mutex no more than 1 second
    if( WaitForSingleObject( hDebugLoggerMutex, 1000 ) == WAIT_TIMEOUT ){
        return;
    }
    
    // If this write pos would mean writting past the mapped memory space.
    if( sharedView->nextWritePos + len + 2 * sizeof( BufItem ) + 3 > BufferSize ){
        // Junk all previous logs.
        sharedView->nextWritePos = 0;        
    }
    
    BufItem *bufItem = reinterpret_cast< BufItem * >( &sharedView->buffer[ sharedView->nextWritePos ] );

    bufItem->processId = GetCurrentProcessId();
    bufItem->threadId = GetCurrentThreadId();
    
    // Write the string at the current pos.
    strcpy( &sharedView->buffer[ sharedView->nextWritePos + sizeof( BufItem ) ], sz );
    
    // Next write is after this string.
    sharedView->nextWritePos = sharedView->nextWritePos + len + 1 + sizeof( BufItem );

    // Cast to the next item and write empty entries.
    bufItem = reinterpret_cast< BufItem * >( &sharedView->buffer[ sharedView->nextWritePos ] );
    bufItem->processId = 0;
    bufItem->threadId = 0;
    

    // Release the mutex.
    ReleaseMutex( hDebugLoggerMutex );

    // Notify the debug server that a string is there.
    SetEvent( hDebugLoggerEvent );
}