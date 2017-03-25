#include <string.h>
#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include "T3Sound.h"//BLBLBL 27 mars 2009 réintégration du sound dans le projet
#include "templatequeues.h"
#include "PowerDebug.h"
#include "App.h"
#include "Global.h"
#include "MemoryManager.h"


#define CD_MSG_OPEN   1
#define CD_MSG_CLOSE  2
#define CD_MSG_PLAY   3
#define CD_MSG_PLAY_R 4
#define CD_MSG_STOP   5
#define CD_MSG_QUERY  6

TemplateQueue <DWORD> tqCDAudio;
HANDLE CDAudioEvent;
DWORD GlobalCDTrack;
HWND GlobalCDHandle;

int ReturnValue;
BOOL GetValue;
BOOL WaitState;
BOOL CDActive = false;

HANDLE hCDThread;

UINT WINAPI CDAudioThread(LPVOID pParam) {
   CDActive = true;
   g_App.StartThread(APP_CD_THREAD);
   DWORD *Msg;
   BYTE lpszReturnString[100];
	DWORD dwLastPos = (INFINITE/2);
	DWORD dwCurrPos;
	BOOL Repeat = FALSE;
	int Counter = 0;
   while (TRUE && !g_boQuitApp) {
      if (WaitForSingleObject(CDAudioEvent, 250) == WAIT_TIMEOUT) {
         if (Repeat) {
				LPBYTE lpszCommandString = new BYTE [100];
				wsprintf((char *)lpszCommandString, "status cdaudio position");
				mciSendString((char *)lpszCommandString, (char *)lpszReturnString, 
					sizeof(lpszReturnString), NULL);
				dwCurrPos = lpszReturnString[strlen((char *)lpszReturnString)-1] - '0';
				dwCurrPos += (lpszReturnString[strlen((char *)lpszReturnString)-2] - '0') * 10;
				dwCurrPos += (lpszReturnString[strlen((char *)lpszReturnString)-3] - '0') * 100;
				dwCurrPos += (lpszReturnString[strlen((char *)lpszReturnString)-4] - '0') * 1000;
				OutputDebugString("QueryState: ");
				OutputDebugString((char *)lpszReturnString);
				OutputDebugString("\r\n");
				if ((dwCurrPos - dwLastPos) <= 10) {
					GetValue = TRUE;
					ReturnValue = TRUE;
					PlayCDAudio(GlobalCDTrack, TRUE);
				} else {
					dwLastPos = dwCurrPos;
				}
			}
		} else {
			Msg = tqCDAudio.Retreive();
         if(Msg)
         {
	         if (tqCDAudio.NbObjects()) 
				   SetEvent(CDAudioEvent);
   
			   switch (*Msg) {
				   case CD_MSG_OPEN: {
					   mciSendString("open cdaudio", (char *)lpszReturnString,
	                  sizeof(lpszReturnString), NULL);
					   mciSendString("set cdaudio time format tmsf", (char *)lpszReturnString, 
	                  sizeof(lpszReturnString), NULL);
				   } break;

				   case CD_MSG_CLOSE: {
	               mciSendString("stop cdaudio", (char *)lpszReturnString, 
						   sizeof(lpszReturnString), NULL);
					   mciSendString("close cdaudio", (char *)lpszReturnString, 
	                  sizeof(lpszReturnString), NULL);  
		         } break;

				   case CD_MSG_PLAY: {
	               LPBYTE lpszCommandString = new BYTE [100];
					   wsprintf((char *)lpszCommandString, "play cdaudio from %d to %d", GlobalCDTrack, GlobalCDTrack+1);
					   mciSendString((char *)lpszCommandString, (char *)lpszReturnString, 
	                  sizeof(lpszReturnString), NULL);
					   Repeat = FALSE;
				   } break;

				   case CD_MSG_QUERY: {
	               BYTE lpszCommandString[100];
					   wsprintf((char *)lpszCommandString, "status cdaudio number of tracks");
					   mciSendString((char *)lpszCommandString, (char *)lpszReturnString, 
		                 sizeof(lpszReturnString), NULL);
					   ReturnValue = atoi((char *)lpszReturnString);					
					   if (ReturnValue > 1)
						   ReturnValue = TRUE;
					   else
						   ReturnValue = FALSE;
					   GetValue = TRUE;
				   } break;

			   
				   case CD_MSG_PLAY_R: {
	               BYTE lpszCommandString[100];
					   wsprintf((char *)lpszCommandString, "play cdaudio from %d to %d", GlobalCDTrack, GlobalCDTrack+1);
					   DWORD err = mciSendString((char *)lpszCommandString, (char *)lpszReturnString, 
	                  sizeof(lpszReturnString), GlobalCDHandle);
					   mciGetErrorString(err, (char *)lpszReturnString, sizeof(lpszReturnString));
					   OutputDebugString((char *)lpszReturnString);
					   Repeat = TRUE;
					   //SendMessage(siGethWnd(), MM_MCINOTIFY, NULL, NULL);
				   } break;

				   case CD_MSG_STOP: {
	               ReturnValue = mciSendString("stop cdaudio", (char *)lpszReturnString, 
						   sizeof(lpszReturnString), NULL);
					   Repeat = FALSE;
				   }
			   }

			   delete Msg;
         }
		}
	}

   g_App.ExitThread(APP_CD_THREAD);
	_endthreadex(1);
	return 1;
}

void SendCDAudio(DWORD Msg) {
/*   DWORD *SendMsg = new DWORD;
   *SendMsg = Msg;
   tqCDAudio.AddToQueue(SendMsg);
   SetEvent(CDAudioEvent);*/
}

void OpenCDAudio(void) {
/*   UINT FakeID;
   CDAudioEvent = CreateEvent(NULL, FALSE, FALSE, "CD Audio Event");
   hCDThread = (HANDLE)_beginthreadex(NULL, 0, CDAudioThread, NULL, NULL, &FakeID);
   SendCDAudio(CD_MSG_OPEN); */
}

void CloseCDAudio(void) {
/*   if (CDActive) {
      SendCDAudio(CD_MSG_CLOSE);
      CloseHandle(CDAudioEvent);
   }*/
}

void PlayCDAudio(int i, BOOL r) {
  /* if (CDActive) {
    	GlobalCDTrack = i;
      if (r) {
         SendCDAudio(CD_MSG_PLAY_R);
      } else {
         SendCDAudio(CD_MSG_PLAY); 
      }
   }*/
}

void StopCDAudio(void) {
/*   if (CDActive) {
   	SendCDAudio(CD_MSG_STOP);
   }*/
}

BOOL QueryCDAudio(void) {
/*   if (CDActive) {
   	ReturnValue = 0;
   	GetValue = FALSE;
   	SendCDAudio(CD_MSG_QUERY);
   	while (!GetValue) Sleep(15);
   }
   return ReturnValue;*/
	return 0;
}