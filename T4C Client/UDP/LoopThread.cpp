#include "LoopThread.h"

CLoopThread::CLoopThread(std::string lThreadName):
CThread(lThreadName)
{
   Suspended = false;
   Terminate = false;
   Running = false;
}

CLoopThread::~CLoopThread()
{
   int toto = 0;
}

void CLoopThread::Start()
{
   if(Suspended)
   {
      Suspended = false;
      SuspendMutex.Unlock();
   }
   else if(!Running)
   {
      CThread::Start();
   }
}

void CLoopThread::Suspend()
{
   if(!Suspended)
   {
      Suspended = true;
      SuspendMutex.Lock();
   }
}

void CLoopThread::Stop()
{
   Terminate = true;
}

void CLoopThread::Wait(int *Status)
{
   CThread::WaitThread(Status);
}


bool CLoopThread::isRunning()
{
   return Running;
}

void CLoopThread::LoopFunction(void)
{
}

int CLoopThread::ThreadFunction()
{
   SuspendMutex.Lock();
   SuspendMutex.Unlock();
   
   Running = true;
   
   while(!Terminate)
   {
      LoopFunction();
      SuspendMutex.Lock();
      SuspendMutex.Unlock();
   }
   
   Running = false;
   
   return 0;
}
