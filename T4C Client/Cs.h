#ifndef __CS_H
#define __CS_H

#include "PowerDebug.h"

class CriticalSection {
	private:
		static double    OldPriority;
		static char      OldName[100];
		static char	     OldID[100];
		char             Name[100];
		double           Priority;
		double           SavePriority;
		char             SaveName[100];
		char			 SaveID[100];
		CRITICAL_SECTION csLock;
	
	public:
		CriticalSection(void) {
			strcpy(Name, "Not Identified");
			Priority = 0.0;
			InitializeCriticalSection(&csLock);
		}

		CriticalSection(char *N, double P) {
			strcpy(Name, N);
			Priority = P;
			InitializeCriticalSection(&csLock);
		}

		void SetCriticalSection(char *N, double P) {
			strcpy(Name, N);
			Priority = P;
		}

		~CriticalSection(void) {
			DeleteCriticalSection(&csLock);
		}

		void Lock(char *ID) {
			EnterCriticalSection(&csLock);
/*			if (OldPriority < Priority && Priority > 0) {
				LOG(0) << "Lock Conflic between [" << OldName << ", " << OldPriority << ", " << OldID << "] and [" << Name << ", " << Priority << ", " << ID << "]\r\n";
				LOG.SaveLogFile("PowerLog.Log");
			} else {
				SavePriority = OldPriority;
				OldPriority = Priority;
				strcpy(SaveName, OldName);
				strcpy(SaveID, OldID);
				strcpy(OldName, Name);
				strcpy(OldID, ID);
			}
*/		}

		void Unlock(char *ID) {
			LeaveCriticalSection(&csLock);
/*			if (OldPriority != Priority && Priority > 0) {
				LOG(0) << "Unlock Conflic between [" << OldName << ", " << OldPriority << ", " << OldID << "] and [" << Name << ", " << Priority << ", " << ID << "]\r\n";
				LOG.SaveLogFile("PowerLog.Log");
			} else {
				OldPriority = SavePriority;
				strcpy(OldName, SaveName);
				strcpy(OldID, SaveID);
			}
*/		}
};

#endif