// ********************************************************************************************
// ***                                                                                      ***
//      File Name: FileLogger.cpp
//      Project:   Vircom Lib
//      Creation:  10 July 1999
//      Author:    Thomas (TH)
// ***                                                                                      ***
// ********************************************************************************************
// *** Copyright (c) 1998-1999 Vircom. All rights reserved.                                 ***

#pragma warning ( disable : 4786 )
#include "pch.h"
#include <windows.h>
#include "FileLogger.h"
#include "MemoryManager.h"
 

#define DEFAULT_LOG_NAME    "LogUndefined.Log"
#define ERROR_LOG_NAME      "LogFileError.Log"

namespace vir {

// ********************************************************************************************
FileLogger::FileLogger( void )
// Constructor
{
    szLogName = DEFAULT_LOG_NAME;
    hFile = NULL;
    bDeleteObject = false;
}

// ********************************************************************************************
FileLogger::~FileLogger( void )
// Destructor
{
    bDeleteObject = true;
    Flush();
}

// ********************************************************************************************
void FileLogger::SetLogLevel
(
 LOG_LEVEL ll // Log Level
) 
// Set the Debug Level.
{
    llLogLevel = ll;
}

// ********************************************************************************************
void FileLogger::SetLogName( 
 char *lpszFileName // Namme of the Log File.
) 
// Store in Memory the name of the log file.
{
    szLogName = lpszFileName; 
};

// ********************************************************************************************
void FileLogger::LogAnsi
( 
 LOG_LEVEL ll,  // Log Level.
 char *s        // String to Log.
) 
// Log a String in Ansi.
{
    if ( ll & llLogLevel )  {
	// The Log Level asked is active.
        vAnsiLogBuffer.push_back(s);
	}
}

// ********************************************************************************************
void FileLogger::LogUnicode
( 
 LOG_LEVEL ll,  // Log Level.
 wchar_t *ws    // String to Log.
) 
// Log a String in Unicode.
{
    if ( ll & llLogLevel )  {
	// The Log Level asked is active.
        vUnicodeLogBuffer.push_back(ws);
	}
}

// ********************************************************************************************
HANDLE FileLogger::OpenLogFile
( 
 const char *s // Log File Name.
)
// Open log file.
{
    // Create File.
   return CreateFile( s, 
                      GENERIC_WRITE, 
                      NULL, 
                      NULL, 
                      CREATE_ALWAYS, 
                      FILE_ATTRIBUTE_NORMAL,
                      NULL 
                    );
}

// ********************************************************************************************
void FileLogger::CloseLogFile
( 
 HANDLE h // Handle of the file.
)
// Close log file.
{
    if (h != INVALID_HANDLE_VALUE)
        CloseHandle(h);
}

// ********************************************************************************************
void FileLogger::Log2File
( 
 HANDLE h // Handle of the file.
)
// Log everything to file.
{
    DWORD dwTemp; // Temp var.
    
    // Iterator for Ansi and Unicode Buffer.
    std::vector< std::basic_string< char > >::iterator i;
    std::vector< std::basic_string< wchar_t > >::iterator j;

    // Log to file Ansi string.
    for (i = vAnsiLogBuffer.begin(); i < vAnsiLogBuffer.end(); ++i) {
        WriteFile( h, (*i).c_str(), (*i).size(), &dwTemp, NULL );
    }

    // Log to file Unicode string.
    for (j = vUnicodeLogBuffer.begin(); j < vUnicodeLogBuffer.end(); ++j) {
        WriteFile( h, (*j).c_str(), (*j).size(), &dwTemp, NULL );
    }
}

// ********************************************************************************************
void FileLogger::Flush( void )
// Flush all Memory Logging into disk.
{
    // Create File.
    hFile = OpenLogFile( szLogName.c_str() );

    if ( hFile != INVALID_HANDLE_VALUE ) {
    // File opened successfuly
        Log2File( hFile );
    } else if (bDeleteObject) {
    // File was not opened successfully, try an alternate filename.
        // Create File.
        hFile = OpenLogFile( ERROR_LOG_NAME );

        if ( hFile != INVALID_HANDLE_VALUE ) {
        // File opened successfuly
            Log2File( hFile );
        } else {
        // Alternate filename was not p[ened successfuly....
        // No alternate solution.
        }
    }

    CloseLogFile(hFile);
}

} // End of namespace vir
