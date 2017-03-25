// ********************************************************************************************
// ***                                                                                      ***
//      File Name: FileLogger.h
//      Project:   Vircom Lib
//      Creation:  25 Mars 1999
//      Author:    Thomas (TH)
// ***                                                                                      ***
//	********************************************************************************************
// ***                                                                                      ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         03-25-1999      1.0     TH          Initial development
//
//      Description
//         Provide an Interface for loggin in a file.
// ***                                                                                      ***
//	********************************************************************************************
// *** Copyright (c) 1998-1999 Vircom. All rights reserved.                                 ***

#ifndef VIRCOMLIB_FILELOGGER_TH093930032599
#define VIRCOMLIB_FILELOGGER_TH093930032599

#include <string>
#include <vector>
#include "Logger.h"

namespace vir {

class FileLogger : public Logger {
// This class is a logger that log to the Debug Screen.
	
	public: 
		FileLogger( void );  // Constructor
        ~FileLogger( void ); // Destructor
        
        void SetLogLevel( LOG_LEVEL ll );     // Set the Debug Level.
		void SetLogName( char *lpszLogName ); // Set the Log File.
        void Flush( void );

	protected:
        HANDLE hFile;                                                   // File Handle.
        bool   bDeleteObject;                                            // true = Object will be Deleted.
        std::string szLogName;                                           // Log File Name.
        std::vector< std::basic_string< char > >    vAnsiLogBuffer;      // Buffer for Anis Logging.
        std::vector< std::basic_string< wchar_t > > vUnicodeLogBuffer;   // Buffer for Unicode Logging.
        
		void LogAnsi( LOG_LEVEL ll, char *s );              // Log a String in Ansi.
		void LogUnicode( LOG_LEVEL ll, wchar_t *ws );       // Log a String in Unicode.
        HANDLE OpenLogFile( const char *s );                // Open Log File.
        void CloseLogFile( HANDLE h );                      // Close Log File.
        void Log2File( HANDLE h );                          // Log everything to File.
};

} // End of namespace vir

#endif