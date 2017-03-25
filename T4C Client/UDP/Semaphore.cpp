#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Semaphore.h"

CNMSemaphore::CNMSemaphore(unsigned char count, unsigned int waitsec)
{
   WaitSec   = waitsec;
   initCount = count;
   // initialization
	sa = NULL;
	semaphore = NULL;
	// create it

   // security handling
   sa = (LPSECURITY_ATTRIBUTES)HeapAlloc(GetProcessHeap(),0,sizeof(SECURITY_ATTRIBUTES));
   sa->nLength = sizeof(SECURITY_ATTRIBUTES);
   sa->lpSecurityDescriptor = NULL; //no restrictions
   sa->bInheritHandle = FALSE;

   // create the semaphore
   if ( (semaphore = CreateSemaphore(sa,0, 0x7fffffff,0)) == NULL)
   {
      //int caca = 0;//BLBLBL 29 mars 2009 suppression des choses inutilisées
      return;
   }
}

CNMSemaphore::~CNMSemaphore()
{
   if (sa) 
      HeapFree(GetProcessHeap(),0,sa);
   
   if ( semaphore ) 
   {
      if ( !CloseHandle(semaphore) )
      {
         //int caca = 0;//BLBLBL 29 mars 2009 suppression des choses inutilisées
      }
      semaphore = NULL;
   }
}

int CNMSemaphore::Wait()
{
   if (WaitSec)
	{
      DWORD retVal = WaitForSingleObject(semaphore,WaitSec);

		if ( retVal == WAIT_TIMEOUT ) 
         return 1;
		if ( retVal == WAIT_OBJECT_0) 
         return 0;

		if ( retVal == WAIT_FAILED )
		{
			return -1;
		}
	}
	else 
	{
      if ( WaitForSingleObject(semaphore,INFINITE) == WAIT_FAILED )
		{
			return -1;
		}
		return 0;
	}
   return -1;
}

int CNMSemaphore::Post()
{
   if ( !ReleaseSemaphore(semaphore,1,NULL) )
	{
		return -1;
	}
   return 0;
}

