#ifndef QUEUE_H
#define QUEUE_H

#if _MSC_VER > 1000
#pragma once
#endif

#define NM_MUTEX_TIMEOUT 1

#include <deque>
#include "Semaphore.h"
#include "Mutex.h"

template <class T> class WaitQueue
{
protected:
   std::deque<T*> Q;
   CNMSemaphore *Semaphore;
   CNMMutex Mutex;
   
   WaitQueue(const WaitQueue& b);
   WaitQueue& operator=(WaitQueue& b);
   
public:
   WaitQueue(unsigned int WaitMax=1000)
   {
      Semaphore = new CNMSemaphore(0,WaitMax);
   }
   
   virtual ~WaitQueue()
   {
      Q.clear();
      delete Semaphore;
   }
   
   void Push(T *Element)
   {
      TempLock TmpMut(Mutex);
      Q.push_back(Element);
      Semaphore->Post();
   }
   
   T* Pop()
   {
      T *Element;
      
      if (Semaphore->Wait() != NM_MUTEX_TIMEOUT )
      {
         TempLock TmpMut(Mutex);
         Element = Q.front();
         Q.pop_front();
         return Element;
      }
      else
      {
         return NULL;
      }
   }
   
   bool isAvailable() const
   {
      TempLock TmpMut(Mutex);
      return (Q.size() > 0);
   }
   
   typename std::deque<T>::size_type Size() const
   {
      TempLock TmpMut(Mutex);
      return Q.size();
   }
};

#endif
