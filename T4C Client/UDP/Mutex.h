#ifndef CMUTEX_H
#define CMUTEX_H

#if _MSC_VER > 1000
#pragma once
#endif

//#ifndef  _WIN32_WINNT
//  #define  _WIN32_WINNT 0x0400 // for SignalObjectAndWait
//#endif
#include <windows.h>

typedef HANDLE pthread_mutex_t;

class CNMMutex
{
protected:
   pthread_mutex_t* m_pMutex ;
   
public: 
   CNMMutex(bool locked = false);
   virtual ~CNMMutex();

    //Do not allow copy constructor or copy operator
   CNMMutex& operator=(CNMMutex &M); // No Copy
   CNMMutex(const CNMMutex&);        // No copy Ctor
   
   void Lock() const;
   void Unlock() const;
};


class TempLock
{
private:
   CNMMutex& Mutex;
public:
   TempLock(CNMMutex const& lMutex):
      Mutex(const_cast<CNMMutex&>(lMutex))
      {
         Mutex.Lock();
      }
      
      virtual ~TempLock()
      {
         Mutex.Unlock();
      }
};

#endif
