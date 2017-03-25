

#include "Mutex.h"

CNMMutex::CNMMutex(bool Locked)
{
   m_pMutex = NULL;
   pthread_mutex_t* lMutex = new pthread_mutex_t;
   if( (*lMutex = ::CreateMutex(0, 0, 0)) == 0 )
   {
      //Unable to create Mutex
      //throw CException(eOtherError, "Mutex::Mutex() can't create!");
   }
   else
      m_pMutex = lMutex;
   
   if(Locked) 
      Lock();
}

CNMMutex::~CNMMutex()
{
   pthread_mutex_t* lMutex = (pthread_mutex_t*) m_pMutex;
   while(::CloseHandle(*lMutex) == 0 )
   {
      // wait for mutex to unlock
      Lock();
      Unlock();
   }//end while
   
   delete m_pMutex;
   m_pMutex = NULL;
}

void CNMMutex::Lock() const
{
   pthread_mutex_t* lMutex = (pthread_mutex_t*) m_pMutex;
   if( ::WaitForSingleObject(*lMutex, INFINITE) != WAIT_OBJECT_0 )
   {
      //throw CException(eWouldDeadLock, "Mutex::lock() can't lock!");
   }
}

void CNMMutex::Unlock() const
{
   pthread_mutex_t* lMutex = (pthread_mutex_t*) m_pMutex;
   if( ::ReleaseMutex(*lMutex) == 0 )
   {
      //throw CException(eMutexNotOwned, "Mutex::unlock() can't unlock!");
   }
}

