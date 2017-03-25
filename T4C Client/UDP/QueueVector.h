#ifndef QUEUE_VECTOR_H
#define QUEUE_VECTOR_H

#if _MSC_VER > 1000
#pragma once
#endif

#define NM_MUTEX_TIMEOUT 1

#include <vector>
#include "Semaphore.h"
#include "Mutex.h"


template <class T> class WaitQueueV
{
protected:
   std::vector<T*> Q;
   CNMSemaphore *Semaphore;
   CNMMutex Mutex;
   T *pElementGlobal;
  
   WaitQueueV(const WaitQueueV& b);
   WaitQueueV& operator=(WaitQueueV& b);
   
public:
   WaitQueueV(unsigned int WaitMax=1000)
   {
      Semaphore = new CNMSemaphore(0,WaitMax);
      pElementGlobal = new T;
   }
   
   virtual ~WaitQueueV()
   {
      for(int i=0;i<Q.size();i++)
         delete Q[i];
      Q.clear();
      delete Semaphore;
      delete pElementGlobal;
   }
   
   void Push(T *Element)
   {
      TempLock TmpMut(Mutex);
      Q.push_back(Element);
      Semaphore->Post();
   }

   
   T* Pop(int dwIndex)
   { 
      if (Semaphore->Wait() != NM_MUTEX_TIMEOUT )
      {
         TempLock TmpMut(Mutex);
         
         pElementGlobal->Copy(Q[dwIndex]);
         delete Q[dwIndex];
         Q.erase(Q.begin() + dwIndex/*it*/);
         return pElementGlobal;
      }
      else
      {
         return NULL;
      }
   }
   
   T* Peek(int dwIndex)
   {
      T *Element;
      
      if (Semaphore->Wait() != NM_MUTEX_TIMEOUT )
      {
         TempLock TmpMut(Mutex);
         
         Element = Q[dwIndex];
         Semaphore->Post();
         return Element;
      }
      else
      {
         return NULL;
      }
   }

   bool ReAllocAndCopy(int dwIndex,int dwLength,unsigned char *pData)
   {
      T *Element;
      
      if (Semaphore->Wait() != NM_MUTEX_TIMEOUT )
      {
         TempLock TmpMut(Mutex);
         
         Element = Q[dwIndex];
         Element->ReAllocAndChangeData(dwLength,pData);
         Semaphore->Post();
         return true;
      }
      else
      {
         return false;
      }
   }
   
   bool isAvailable() const
   {
      TempLock TmpMut(Mutex);
      return (Q.size() > 0);
   }

  
   typename std::vector<T>::size_type Size() const
   {
      TempLock TmpMut(Mutex);
      return Q.size();
   }
};

#endif
