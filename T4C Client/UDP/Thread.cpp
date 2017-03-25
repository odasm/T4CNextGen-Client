//--FILE INCLUDES--------------------------------------------------------
#include "Thread.h"

extern "C" int CommonThreadFunction(void* Data)
{
   if(CThread* Thread = reinterpret_cast<CThread*>(Data))
   {
      return Thread->ThreadFunction();
   }
   
   return -1;
}

CThread::CThread(std::string lThreadName)
{
   m_ThreadName = lThreadName;
   m_hThread    = NULL;
   m_bInit      = false;
   m_iPriority  = THREAD_PRIORITY_NORMAL;      //THREAD_PRIORITY_NORMAL
   m_bRunning   = false;
}

CThread::~CThread()
{
   if (m_bInit)
   {
      WaitThread();
      KillThread();
   }
}

void CThread::Start()
{
   if( !m_bRunning )
   {
      if(!m_hThread)
         m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CommonThreadFunction, reinterpret_cast<void*>(this), CREATE_SUSPENDED, 0);
      if( m_hThread )
      {
         m_bInit = true;
         ::ResumeThread(m_hThread);
         m_bRunning = true;
      }
   }
   return;
}


bool CThread::SetPriority(int _iPriority)
{
   m_iPriority = _iPriority;
   if( m_hThread )
   {
      if( m_bRunning )
         ::SuspendThread(m_hThread);
      
      ::SetThreadPriority(m_hThread, m_iPriority);
      
      if( m_bRunning )
         ::ResumeThread(m_hThread);
   }
   return true;
}

void CThread::WaitThread(int *Status)
{
   Destroy();
}

void CThread::KillThread()
{
   Destroy();
}

std::string CThread::GetThreadName(void)
{
   return m_ThreadName;
}

HANDLE CThread::GetThreadID() const
{
   return m_hThread;
}

bool CThread::Destroy()
{
   bool bstatus = true;
   if( m_hThread && m_bInit )
   {
      switch( ::WaitForSingleObject(m_hThread, 5000) )
      {
         case WAIT_TIMEOUT:
            // Probably will create leaks but it is better than freeze program
            ::TerminateThread(m_hThread, 0);
            ::CloseHandle(m_hThread);
            m_hThread = NULL;
            m_bInit = m_bRunning = false;
            bstatus = false;
         break;
      }
      ::CloseHandle(m_hThread);
   }
   m_hThread = NULL;
   m_bInit = m_bRunning = false;
   return bstatus;
}

int CThread::ThreadFunction(void)
{
   return 0;
}
