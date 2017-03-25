#ifndef CSEMAPHORE_H
#define CSEMAPHORE_H

#if _MSC_VER > 1000
#pragma once
#endif

//#ifndef  _WIN32_WINNT
//  #define  _WIN32_WINNT 0x0400 // for SignalObjectAndWait
//#endif

#include <windows.h>

class CNMSemaphore
{
   
public: 
   CNMSemaphore(unsigned char count=1, unsigned int waitsec=0);
   virtual ~CNMSemaphore();
   
   int Wait();
   int Post();

private:
   
   long initCount;  // initialCount 
   long WaitSec;    // max count
   
   HANDLE semaphore;
   LPSECURITY_ATTRIBUTES sa;
};


#endif
