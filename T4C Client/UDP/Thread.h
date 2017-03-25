#ifndef NM_THREAD_H
#define NM_THREAD_H

#if _MSC_VER > 1000
#pragma once
#endif

//--FILE INCLUDES--------------------------------------------------------
//#ifndef  _WIN32_WINNT
//  #define  _WIN32_WINNT 0x0400 // for SignalObjectAndWait
//#endif
#include <windows.h>
#include <string>

//--TYPE DEFINITIONS-----------------------------------------------------
#define PSleep(x)    ::Sleep(x)

extern "C" int CommonThreadFunction(void*);

class CThread 
{
public:
   //--CONSTRUCTOR-------------------------------------
   CThread(std::string lThreadName);
   //--DESTRUCTOR--------------------------------------
   virtual ~CThread();
   
   //Do not allow copy constructor or copy operator
   CThread& operator=(const CThread&); // No Copy
   CThread(const CThread&);            // No copy Ctor
   
   friend int CommonThreadFunction(void*);
   
public:
   //--PUBLIC METHODS----------------------------------
   virtual void WaitThread(int *Status = 0);
   virtual void KillThread(void);
   void  Start();    // start the Thread
   bool  SetPriority(int _iPriority);
   HANDLE GetThreadID() const;
   std::string GetThreadName(void);
   
protected:
   bool Destroy(void);
   virtual int ThreadFunction(void);
   
private:
   //--PRIVATE ATTRIBUTES------------------------------
   std::string m_ThreadName;
   HANDLE      m_hThread;
   bool        m_bInit;
   bool        m_bRunning;
   int         m_iPriority;
};

#endif //NM_THREAD

