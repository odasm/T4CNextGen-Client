#ifndef __DIRECTSOCKET
#define __DIRECTSOCKET
//
//
//






// Define Constance.
#define UDP SOCK_DGRAM
#define TCP SOCK_STREAM



//********************************************************//
class DirectSocket
//********************************************************//
// Control the Window Socket for TFC or UDP Connection
//
{
	private:
		WSADATA  wsaData;     // Store Windows Sockets Initialization Information.

		sockaddr_in Server;   // Store the Address Information
		SOCKET  ConnSocket;   // Socket Desciption
		hostent *HostInfo;    // Host Information

		char ServerName[100];// Name or IP Address of the Server
		unsigned short Port; // Port of the Server
		int SocketType;      // Type of the Socket (TCP or UDP)

		bool isCreated;         // True if the Socket is Created.
		bool isSendBlocking;    // True if Send is Blocking.
		bool isReceiveBlocking; // True if Receive is Blocking.

		BOOL Exception( char * ); // Exception

	public:
		DirectSocket ( void );  // Default Constructor
		~DirectSocket( void );  // Default Destructor

		BOOL CreateSocket( void ); // Create the Socket
		void CloseSocket ( void ); // Close the Socket

		void SetIPAddr  ( char * );     // Set the IP Address
		void SetAddrPort( int );        // Set the Port
		void SetType    ( int );        // Set the Type (TCP or UDP)
		void SetBlocking( bool, bool ); // Set Send and Receive Blocking.

		void Send   ( char *, int ); // Send a String to the Socket
		int  Receive( char *, int ); // Receive a String from the Socket
};
#endif