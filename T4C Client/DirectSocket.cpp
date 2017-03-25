#include "pch.h"
#include <windows.h>
#include <stdio.h>
#include "DirectSocket.h"
#include "PowerDebug.h"
#include "Global.h"
#include "MemoryManager.h"


//********************************************************//
DirectSocket::DirectSocket( void ) 
//********************************************************//
// Contructeur par Default.
// 
{
	isCreated = false;	
	isSendBlocking = false;
	isReceiveBlocking = false;
	HostInfo = NULL;
   Port = 1;
}

//********************************************************//
DirectSocket::~DirectSocket( void ) 
//********************************************************//
// Destructeur par Default.
// 
{
	if (isCreated)
		CloseSocket();
}

//********************************************************//
void DirectSocket::SetIPAddr
//********************************************************//
// Definit l'addresse IP
// 
( 
 char *IP // IP Address
) 
{
	strcpy(ServerName, IP);
}

//********************************************************//
void DirectSocket::SetAddrPort
//********************************************************//
// Definit le Port
// 
(
 int P // Port
) 
{
	Port = P;
}

//********************************************************//
void DirectSocket::SetType
//********************************************************//
// Definit le Type
// 
(
 int T // Type
)
{
	SocketType = T;
}

//********************************************************//
void DirectSocket::SetBlocking
//********************************************************//
// Definit le Blocking de Send et Receive
// 
(
 bool Send,   // Send Blocking
 bool Receive // Receive Blocking
) 
{
	isSendBlocking = Send;
	isReceiveBlocking = Receive;
}

//********************************************************//
void DirectSocket::Send
//********************************************************//
// Envoie d'un String au Socket
// 
(
 char *Data, // Data to Send to the Socket
 int   Len  // Length of the Data
) 
{
	int Result; // Return Value of the send function

//	do {
		Result = send(ConnSocket, Data, Len, 0);
		if (Result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
		// An Error occur.
         LOG << "ERROR DURING SENDING\r\n";
		}
//	} while (Result == SOCKET_ERROR && // An Error occur.
//		      WSAGetLastError() == WSAEWOULDBLOCK && // Information cannot be sent.
//				isSendBlocking); // Socket sould retry sending data.
      Sleep(10);
}

//********************************************************//
int DirectSocket::Receive
//********************************************************//
// Recoie un String du Socket
// 
( 
 char *Data, // Data receive from the Socket
 int   len   // Maximum Length of the Data.
) 

//********************************************************//
{
	int Result; // Return Value of the send function
		
	do {
		Result = recv(ConnSocket, Data, len, 0 );
		if (Result == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) {
		// An Error occur.
         Exception("Receive");
		}

		if (!Result) {
		// Server closed Connection if TCP.
			Exception("Receive");
		}
	} while (Result == SOCKET_ERROR && // An Error occur.
		      WSAGetLastError() == WSAEWOULDBLOCK && // Information cannot be sent.
				isReceiveBlocking); // Socket sould retry sending data.

	return Result;
}

//********************************************************//
BOOL DirectSocket::CreateSocket( void )
//********************************************************//
// Creation d'un Socket
// 
{
	isCreated = true;
	if (WSAStartup(0x202,&wsaData) == SOCKET_ERROR) {
	// An Error occur
		Exception("Create 1");
	}

   LOG << "HIHA! ON CHK CE VALEUR LA: [" << (int)wsaData.iMaxUdpDg << "]\r\n";

	ConnSocket = socket(AF_INET, SocketType, 0); 

	memset(&Server, 0, sizeof(Server));
	Server.sin_family = AF_INET;
	Server.sin_port   = htons(Port);
	if (!isdigit(ServerName[0])) {   
	// Server Name is Alphanumeric
		HostInfo = gethostbyname(ServerName);
		if (!HostInfo)
			return Exception("Create 2");
		memcpy(&(Server.sin_addr), HostInfo->h_addr, HostInfo->h_length);
	} else  { 
	// Server Name is a nnn.nnn.nnn.nnn
		Server.sin_addr.s_addr = inet_addr(ServerName);
	}

	if (ConnSocket < 0) {
	// Cannot Create the Socket
		Exception("Create 3");
	}

	if (connect(ConnSocket,(struct sockaddr*)&Server,sizeof(Server)) == SOCKET_ERROR) {
	// Cannot Connect.
		Exception("Create 4");
	}

	if (isSendBlocking && isReceiveBlocking)
	// Both Send and Receive are Blocking.
		ioctlsocket(ConnSocket, FIONBIO, NULL);
	else {
	// Not Both Send and Receive are Blocking
		DWORD value = 1;
		ioctlsocket(ConnSocket, FIONBIO, &value);
	}
   return TRUE;
}

//********************************************************//
void DirectSocket::CloseSocket( void ) 
//********************************************************//
// Fermeture du Socket
// 
{
	if (isCreated) {
	// The Socket was not already closed.
		isCreated = false;
		closesocket(ConnSocket);
		WSACleanup();
	}
}

//********************************************************//
BOOL DirectSocket::Exception( char *s )
//********************************************************//
// Exception Generer par le Socket
// 
{
   OutputDebugString(s);
   Sleep(10);
   return FALSE;
}
