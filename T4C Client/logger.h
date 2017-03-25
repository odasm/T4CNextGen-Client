// ********************************************************************************************
// ***                                                                                      ***
//      File Name: Logger.h
//      Project:   Vircom Lib
//      Creation:  29 January 1999
//      Author:    Thomas (TH)
// ***                                                                                      ***
// ********************************************************************************************
// ***                                                                                      ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         01-29-1999      1.00    TH          Initial development
//		   01-30-1999	   1.01	   FL          AutoLock class added.
//		   02-11-1999	   1.02    TH          Added comment.
//         02-17-1999      1.03    FL          Changed namespace to 'vir'
//         03-05-1999      1.04    FL          Added DL_ALL debugging level.
//         07-10-1999      1.05    TH          Added LOG_LEVEL that *will* replace DEBUG_LEVEL  
//                                             in version 2.0.
//
//      Description
//         Provide an Interface for the logger.
// ***                                                                                      ***
// ********************************************************************************************
// *** Copyright (c) 1998-1999 Vircom. All rights reserved.                                 ***

// The following prevent the Logger to be included multiple time in a single cpp file. I used
// the following: VIRCOMLIB to identify the library, LOGGER for the name of the object, TH for
// my name and 07:22:16 is the time, 01/29/99 the date.
#ifndef VIRCOMLIB_LOGGER_TH072216012999
#define VIRCOMLIB_LOGGER_TH072216012999

// The namespace (ref: Francois E-Mail) is use to prevent the "polution" of the global namespace
// If you want to be able to use the following object in your code, just add the following line
// 
// using namespace VircomLib; 
//
// in your cpp files.
namespace vir {

// Debug Level will be use to set the debug level of the logging. It currently contain 32 bit field
// but could be expanded if someone is crazy enough to write a class for it.
#define LOG_LEVEL unsigned long

// The next few consts are Reserved Debug Level, you can start your own custom Debug Level starting
// at number 0x0020.
// This Debug Level is used to log Debugging Information.
const LOG_LEVEL LL_DEBUG   = 0x0001; 
// This Debug Level is used to explain in detail errors that occurs with the use of your class.
const LOG_LEVEL LL_ERROR   = 0x0002; 
// This Debug Level is used to warn about at non-fatal mis-use of your class.
const LOG_LEVEL LL_WARNING = 0x0004;
// This sets all bits to 1, enabling all logs to trigger.
const LOG_LEVEL LL_ALL     = static_cast< LOG_LEVEL >( -1 );

// *** Will be removed in Version 2.0
#define DEBUG_LEVEL unsigned long
const DEBUG_LEVEL DL_DEBUG   = 0x0001; 
const DEBUG_LEVEL DL_ERROR   = 0x0002; 
const DEBUG_LEVEL DL_WARNING = 0x0004;
const DEBUG_LEVEL DL_ALL     = static_cast< DEBUG_LEVEL >( -1 );
// ***

// Here are 2 object function use to convert Ansi to Unicode, and Unicode to Ansi. The syntax of a
// object function is the following: 
//
// AnsiToUnicode()(PARAM); 
//
// AnsiToUnicode() create an unamed object (more effecient for the compiler to optimize the code. 
// This temporary object will call the operator () function with the specified PARAM. After the call
// the temprary object is automatically destroyed.

class AnsiToUnicode {
// This class transform Ansi String to Unicode String.

	public:
		
		wchar_t *operator() ( const char *const s, wchar_t ws[] ) { 
		// Transform Ansi string to Unicode string.
         //MultiByteToWideChar(CP_ACP, 0, s, -1, ws, strlen(s));
         mbstowcs(ws, s, strlen(s));












         return ws;
		} 
}; // End of Class AnsiToUnicode.

class UnicodeToAnsi {
// This class transform Unicode String to Ansi String.

	public:

	    char *operator () ( const wchar_t *const ws, char s[] ) {
		// Transfrom Unicode string to Ansi string.

			// Converstion.
			//WideCharToMultiByte( CP_ACP, NULL, ws, -1, s, wcslen( ws ), NULL, NULL );
         wcstombs(s, ws, wcslen(ws));
			return s;
		}
}; // End of Class UnicodeToAnsi.

// Here come the fun part. This is the base Logger class. Because it contains pure virtual function
// (function with this syntax: virtual void func(void) = 0;) it cannot be instantiate, that means,
// you can only make pointers of Logger. The reason behind that is that the compiler cannot find
// any body for the member function void func(void);

class Logger {
// This class is the base logger class. It containt all base function for any derived 
// Logger class. 

	public:
	// The following function are public. If someone thing one of this function should be
	// put protected or private, please let-me know.

		// Constructor.
		Logger( void ) { 
		// By default, the Logger won't log anything.
			llLogLevel = 0; 
		} 
		
		// Thread safety function. I didn't put them pure virtual, so by default, they 
		// simply do nothing.
		virtual void Lock    ( void ) { };
		virtual void Unlock  ( void ) { };
		virtual bool PickLock( void ) { Lock(); return true; }

		// User Logging Functions.
		virtual void Log( LOG_LEVEL ll, char *s ) {
		// Ansi Version. Here we use the autolock class instead of the Lock/Unlock function call.
		// What we did is put a this->Lock call in the constructor of AutoLock, called when the an
		// AutoLock object is first instantiated, and a call to this->Unlock in the destructor, called 
		// when the AutoLock object goes out of scope. The AutoLock syntax is better than the normal
		// Lock/Unlock call because it will ALWAYS call unlock, even if LogAnsi throw an exception.
		// Another way to write the following 2 lines of code would have been:
		//
		// Lock();
		// try {
		//		LogAnsi( dl, s );
		//		Unlock();
		// } catch (...) {
		//		Unlock();
		//		throw;
		// }
			AutoLock al( this );
			LogAnsi( ll, s );
		}
		virtual void Log( LOG_LEVEL ll, wchar_t *ws ) {
		// Unicode Version. Same explanation as the Ansi version.
			AutoLock al( this );
			LogUnicode( ll, ws );
		}

		// Utilities Functions.
		virtual bool QueryDebugLevel( LOG_LEVEL ll ) { 
		// The default implementation of QueryDebugLevel return true if the queryied debug level
		// was set to true by the user. (hmmm... I think the code is more clear than this comment.)
			return (ll & llLogLevel) ? true : false; 
		}

	protected:
	// The following function are protected. Here is an explanation why all those function are 
	// protected. The AutoLock class is not public because we don't want the client of your Logger
	// to be able to call it, instead, it should use the lock/unlock function. Also, I didn't put 
	// it protected because you may need it in your own implementation of your logger. The 
	// SetLogLevel function is also protected, because I didn't want any client of your Logger 
	// to be able to change the Log Setting. LogAnsi and LogUnicode are sub function use by the 
	// public function LOG. Also, LogAnsi and LogUnicode should stay protected in your own 
	// derived logger.

		class AutoLock{
		// Lock-safety class. Automatically manages unlocking on object's destruction. The explanation
		// of this type of class is giving in the comment of the Log function.
			public:
				AutoLock( Logger *lpLogger ){
				// Constructor, locking the lpLogger class.
					lpLoggerInstance = lpLogger;
					lpLoggerInstance->Lock();
				}

				~AutoLock(){
				// Destructor, unlocking the lpLogger class.
					lpLoggerInstance->Unlock();
				}
			private:
				Logger *lpLoggerInstance; // The lpLoggerInstance, needed by the destructor.
		};
		
		LOG_LEVEL llLogLevel;			// Debug Level Value (32 BitFields).
		
        // *** Will be remove in Version 2.0
        virtual void SetDebugLevel( DEBUG_LEVEL dl) { SetLogLevel( dl ); }
        // ***

        // Utilities Functions.
        virtual void SetLogLevel( LOG_LEVEL ll ) = 0;
		// There is no default behavior for SetLogLevel. This function should be public in your
		// derived logger. The reason Why this function has no default behavior is that this function
        // need to be protected in this class.

		// Internal Logging Functions.
		// There is no default implementation of LogAnsi and LogUnicode. An example can be fond
		// in my TraceLogger class.
        virtual void LogAnsi   ( LOG_LEVEL ll, char *s  )   = 0;
		virtual void LogUnicode( LOG_LEVEL ll, wchar_t *s ) = 0;
}; // End of class Logger

} // End of namespace vir

#endif // VIRCOMLIB_LOGGER_TH072216012899


