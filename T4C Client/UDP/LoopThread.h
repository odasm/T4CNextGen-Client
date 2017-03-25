#ifndef CLOOPTHREAD_H
#define CLOOPTHREAD_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "Thread.h"
#include "Mutex.h"

class CLoopThread : public CThread  
{
private:
   CNMMutex SuspendMutex;
   volatile bool Suspended;
   volatile bool Terminate;
   volatile bool Running;
   
   virtual int ThreadFunction(void);
   
protected:
   virtual void LoopFunction(void);
   
public:
   CLoopThread(std::string lThreadName);
   virtual ~CLoopThread();
   
   virtual void Start();
   virtual void Suspend();
   virtual void Stop();
   
   virtual void Wait(int *Status = 0);
   
   bool isRunning();
};

#endif

