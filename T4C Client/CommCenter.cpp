// CommCenter.cpp: implementation of the CCommCenter class.
//
//////////////////////////////////////////////////////////////////////

//si non definie USE_CLIENT_CONNECTION
    //#include "stdafx.h"
//else
   #include "windows.h"
#include <process.h>
#include <time.h>
#include "CommCenter.h"
#include "Crypt.h"
//#include "../../CryptMestoph/crypt.h"

#ifndef USE_CLIENT_CONNECTION
   #include "T4CLog.h"
   #include "DeadlockDetector.h"
   #include "ThreadMonitor.h"
#else
   #include "Random.h"
#endif


#ifdef TRACE
#undef TRACE
#endif
#define TRACE true ? (void)0 : printf
#define TRACELOG true ? (void)0 : CT4CLog::GetLog( LOG_DEBUG)->AsyncLog

#ifndef USE_CLIENT_CONNECTION
   #define NM_WAIT_IO  60000 // BLBLBL 10/12/2010 60000, je baisse à 5000 comme pour le serveur, pour voir//13/12/2010 ça change rien car cette ligne c'est pour le serveur..
#else
   #define NM_WAIT_IO  5000
#endif

// < operator on sockaddr_in for map<> operations.
bool operator < ( const sockaddr_in &sock1, const sockaddr_in &sock2 )
{    
    return sock1.sin_addr.S_un.S_addr < sock2.sin_addr.S_un.S_addr ? true : sock1.sin_port < sock2.sin_port;        
}

// Construction //////////////////////////////////////////////////////////////////////////
CCommCenter::CCommCenter( CCommMonitor *lpMon)
{

   srand( (unsigned)time( NULL ) );

   sRecvSocket = NULL;
   sSendSocket = NULL;

   bUDPReceiveDataThread   = TRUE;    
   bUDPSendDataThread      = TRUE;       
   bUDPReceivePacketThread = TRUE;  
   bUDPSendPacketThread    = TRUE;     
   bUDPMaintenancePacket   = TRUE;    
   bUDPAnalyseThread       = TRUE;


   #ifdef USE_CLIENT_CONNECTION
      m_bIsWindowNT = FALSE;

      OSVERSIONINFO VersionInformation;
      VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      if (  GetVersionEx(&VersionInformation) )
      {
         if ( VersionInformation.dwPlatformId == VER_PLATFORM_WIN32_NT )
            m_bIsWindowNT = TRUE;
      }
   #endif
   

   hhUDPReceivePacketIO    = NULL;
   hhUDPSendPacketIO       = NULL;
   hhUDPPreSendPacketIO    = NULL;
   hhUDPAnalyseIO          = NULL;

   hhUDPReceiveDataThread   = NULL;
   hhUDPSendDataThread      = NULL;
   hhUDPReceivePacketThread = NULL;
   hhUDPSendPacketThread    = NULL;
   hhUDPMaintenancePacket   = NULL;
   hhUDPAnalyseThread       = NULL;
   #ifdef USE_CLIENT_CONNECTION
      //hhUDPCheckServer       = NULL;
      //hhUDPStopClient        = NULL;
      //bUDPCheckServer        = TRUE;
      //m_bSendCRC             = FALSE;
   #endif
      
   if (lpMon != NULL) 
   {
      lpMonitor = lpMon;
   } 
   else 
   {
      //just to make sure lpMonitor is NEVER NULL, so we can call lpMonitor-> members without worring.
      lpMonitor = new CCommMonitorEmpty();
   }
}

#ifndef USE_CLIENT_CONNECTION
   #define WAIT_THREAD( __thread ) if( WaitForSingleObject( __thread, 5000 ) == WAIT_TIMEOUT ) {\
                                    TerminateThread( __thread, 1 );\
                                }
#else
   #define WAIT_THREAD( __thread ) if( WaitForSingleObject( __thread, 500 ) == WAIT_TIMEOUT ) {\
                                    TerminateThread( __thread, 1 );\
                                }
#endif


// Destructor ////////////////////////////////////////////////////////////////////////////
CCommCenter::~CCommCenter( void )
{
   // Set all thread to non-running.
   bUDPReceiveDataThread   = FALSE;    
   bUDPSendDataThread      = FALSE;       
   bUDPReceivePacketThread = FALSE;  
   bUDPSendPacketThread    = FALSE;     
   bUDPMaintenancePacket   = FALSE;    
   bUDPAnalyseThread       = FALSE;


   #ifdef USE_CLIENT_CONNECTION
      if(m_bIsWindowNT)
      {
   #endif
         // Signal sending and interpretation queues to start running.
         int toto1 = CancelIo( hhUDPReceivePacketIO );
         int toto3 = CancelIo( hhUDPPreSendPacketIO );
         int toto2 = CancelIo( hhUDPSendPacketIO );
         int toto4 = CancelIo( hhUDPAnalyseIO );
   #ifdef USE_CLIENT_CONNECTION
      }
   #endif

   
   
   // Wait for each threads to complete
   #ifdef USE_CLIENT_CONNECTION
//      bUDPCheckServer = FALSE;
//      WAIT_THREAD( hhUDPCheckServer);
   #endif
   WAIT_THREAD( hhUDPReceivePacketThread );
   WAIT_THREAD( hhUDPSendPacketThread );

   WAIT_THREAD( hhUDPReceiveDataThread );    
   WAIT_THREAD( hhUDPSendDataThread );
   WAIT_THREAD( hhUDPAnalyseThread );
   WAIT_THREAD( hhUDPMaintenancePacket);

   
      
   hhUDPReceiveDataThread   = NULL;
   hhUDPSendDataThread      = NULL;
   hhUDPReceivePacketThread = NULL;
   hhUDPSendPacketThread    = NULL;
   hhUDPMaintenancePacket   = NULL;
   hhUDPAnalyseThread       = NULL;
   #ifdef USE_CLIENT_CONNECTION
//      hhUDPCheckServer       = NULL;
//      hhUDPCheckServer       = NULL;
   #endif

   if( sRecvSocket != NULL )
   {
      // If the receiving socket is the sending socket.
      if( sRecvSocket == sSendSocket )
      {        
         closesocket( sRecvSocket );
      }
      else
      {
         closesocket( sRecvSocket );
         closesocket( sSendSocket );
      }
   }

   
   
   ConnectionMap::iterator i;
   
   for( i = m_Connections.begin(); i != m_Connections.end(); i++ )
   {
      // Destroy all connections.
      CNMConnection* lpConnection = (*i).second;
      delete lpConnection;
   }
   // Erase all connections.
   m_Connections.erase( m_Connections.begin(), m_Connections.end() );
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Initializes and binds a socket
// 
// WORD wPort,        // The port to bind to
// LPCSTR lpszIP,     // The IP to bind on.
// SOCKET &sSocket,   // The socket to initialize.
// sockaddr_in &sockAddr // The address to bind the socket to.
//
// Return: bool, 
//////////////////////////////////////////////////////////////////////////////////////////
bool CCommCenter::InitSocket(WORD wPort,LPCSTR lpszIP,SOCKET &sSocket,sockaddr_in &sockAddr)
{
   sSocket = socket(AF_INET, SOCK_DGRAM, 0);
   
   if( sSocket != INVALID_SOCKET )
   {
      // Initilize the adress.
      sockAddr.sin_family = AF_INET;
      sockAddr.sin_port = htons( wPort );
      if( lpszIP == NULL )
      {
         #ifndef USE_CLIENT_CONNECTION
            _LOG_DEBUG
               LOG_DEBUG_LVL4,
               "szIP NULL"
               LOG_
         #endif
            
            
         sockAddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
         if( sockAddr.sin_addr.S_un.S_addr == INADDR_NONE )
         {
            #ifndef USE_CLIENT_CONNECTION
               _LOG_DEBUG
                  LOG_DEBUG_LVL4,
                  "Failed to find INADDR_ANY"
                  LOG_
            #endif
               
            closesocket( sSocket );
            sSocket = NULL;
            return FALSE;
         }
      }
      else
      {
         sockAddr.sin_addr.S_un.S_addr = inet_addr( lpszIP );
      }
      
      #ifndef USE_CLIENT_CONNECTION
         _LOG_DEBUG
            LOG_DEBUG_LVL4,
            "Trying to bind to socket"
            LOG_
      #endif
         
      if( bind( sSocket, (sockaddr *)&sockAddr, sizeof( sockaddr_in ) ) == 0 )
      {
         int nLen = sizeof( sockaddr_in );
         // Fetch the adress set by the system.
         getsockname( sSocket, (sockaddr *)&sockAddr, &nLen );
         return true;
      }
      
      #ifndef USE_CLIENT_CONNECTION
         if( lpszIP != NULL )
         {
            _LOG_DEBUG
               LOG_DEBUG_LVL1,
               "Failed to bind socket on address %s p%u",
               lpszIP,
               wPort
               LOG_
         }
         else
         {
            _LOG_DEBUG
               LOG_DEBUG_LVL1,
               "Failed to bind socket on port %u.",
               wPort
               LOG_
         }
      #endif
   }
   closesocket( sSocket );
   sSocket = NULL;
   return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Creates a communication interface for a certain port on a certain IP.
// 
// COMM_INTR_CALLBACK lpRead, // The packet interpretation function
// WORD wRecvPort,            // The port to assign the receiving connection to. 0 for random port.
// WORD wSendPort,            // The port to assign the sending to. 0 for same.
// LPCSTR lpszRecvIP,         // The IP to assign the connection to. NULL for default IP. 
// LPCSTR lpszSendIP          // The IP 
//////////////////////////////////////////////////////////////////////////////////////////
BOOL CCommCenter::Create(COMM_INTR_CALLBACK lpRead,WORD wRecvPort/*=0*/,WORD wSendPort/*=0*/,
                         LPCSTR lpszRecvIP/*=0*/,LPCSTR lpszSendIP/*=0*/)
{
   bool boSameIP = false;
   bool boInitOK = true;
   // If the IPs aren't null
   if( lpszRecvIP != NULL && lpszSendIP != NULL )
   {
      // If the IPs are the same.
      if( stricmp( lpszRecvIP, lpszSendIP ) == 0 )
      {
         boSameIP = true;
      }
   }
   // Otherwise if both are null.
   else if( lpszRecvIP == lpszSendIP )
   {
      boSameIP = true;
   }    
   
   // If the IPs are the same and the ports are the same.
   if( boSameIP && wRecvPort == wSendPort )
   {
      // Only initialize the receiving socket
      boInitOK = InitSocket( wRecvPort, lpszRecvIP, sRecvSocket, sockRecvAddr );
      // And copy the data for the sending socket.
      sSendSocket = sRecvSocket;
      sockSendAddr = sockRecvAddr;        
   }
   else
   {
      // First initialize the receiving socket.
      boInitOK = InitSocket( wRecvPort, lpszRecvIP, sRecvSocket, sockRecvAddr );
      
      // If receiving socket initialized.
      if( boInitOK )
      {
         // Initialize sending socket.
         boInitOK = InitSocket( wSendPort, lpszSendIP, sSendSocket, sockSendAddr );
      }
   }
   
   // If socket initialization went on correctly.
   if( boInitOK )
   {
      #ifndef USE_CLIENT_CONNECTION
         _LOG_DEBUG
            LOG_DEBUG_LVL4,
            "Starting CCommCenter threads."
            LOG_            
      #endif
         
      lpReadCallback = lpRead;

      m_uiNbrInterpQueueCnt = 0;

      #ifdef USE_CLIENT_CONNECTION
         if(m_bIsWindowNT)
         {
      #endif
            hhUDPAnalyseIO        = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 1 );
            hhUDPPreSendPacketIO  = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 1 );
            hhUDPSendPacketIO     = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 1 );
            hhUDPReceivePacketIO  = CreateIoCompletionPort( INVALID_HANDLE_VALUE, NULL, 0, 1 );
      #ifdef USE_CLIENT_CONNECTION
         }
      #endif

      hhUDPReceiveDataThread   = (HANDLE)_beginthread( UDPReceiveDataThread  , 0, this );
      hhUDPSendDataThread      = (HANDLE)_beginthread( UDPSendDataThread     , 0, this );
      hhUDPReceivePacketThread = (HANDLE)_beginthread( UDPReceivePacketThread, 0, this );            
      hhUDPSendPacketThread    = (HANDLE)_beginthread( UDPSendPacketThread   , 0, this );        
      hhUDPMaintenancePacket   = (HANDLE)_beginthread( UDPMaintenancePacket  , 0, this );   
      hhUDPAnalyseThread       = (HANDLE)_beginthread( UDPAnalyseThread      , 0, this );   
/*      #ifdef USE_CLIENT_CONNECTION //BLBLBL 08/12/2010 ??? un thread à la con ?_
         hhUDPCheckServer      = (HANDLE)_beginthread( UDPCheckServer      , 0, this );   
      #endif*/
      
      
      #ifndef USE_CLIENT_CONNECTION
         SetThreadPriority( hhUDPReceivePacketThread    , THREAD_PRIORITY_HIGHEST );
         SetThreadPriority( hhUDPSendPacketThread       , THREAD_PRIORITY_HIGHEST );
         SetThreadPriority( hhUDPReceiveDataThread      , THREAD_PRIORITY_ABOVE_NORMAL );      
         SetThreadPriority( hhUDPSendDataThread         , THREAD_PRIORITY_ABOVE_NORMAL );      
         SetThreadPriority( hhUDPAnalyseThread          , THREAD_PRIORITY_ABOVE_NORMAL );      
      #else

      #endif

   }
   else
   {
      #ifndef USE_CLIENT_CONNECTION
         _LOG_DEBUG
            LOG_DEBUG_LVL4,
            "Communication sockets failed to initialize."
            LOG_
      #endif
      return FALSE;
   }
   
   #ifndef USE_CLIENT_CONNECTION
      _LOG_DEBUG
         LOG_DEBUG_LVL4,
         "Successfully initialized main communication socket."
         LOG_
   #endif


      
   return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Allows sending a packet to a socket.
// 
// sockaddr_in sockAddr,  // The adress to send packet to.
// LPBYTE lpBuffer,       // The data buffer.
// int nBufferSize,       // The data buffer size.
// DWORD dwAckDelay,      // The delay between acks for packet security. Unused if packet isn't secured.
// DWORD dwMaxAck,        // The maximum number of acks before declaring packet lost. 0 if packet isn't secure.
//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::SendPacket(sockaddr_in sockAddr,LPBYTE lpBuffer,int nBufferSize,DWORD dwAckDelay,DWORD dwMaxAck)
{
   //OutputDebugString("SendLock LOCK\n");
   m_SendLock.Lock();
   CNMConnection* lpConnection = NULL;
   lpConnection = GetConnection( sockAddr );

   if(nBufferSize <= MAX_DATA_SIZE) //None splitted packet...
   {
       UDPPacket *pPacket    = new UDPPacket;
       pPacket->lpBuffer     = new BYTE[nBufferSize+HEADER_SIZE+CHKSUM_SIZE];

       // Initialise Buffer Data
       pPacket->packetHeader = (UDPPacketHeader*)pPacket->lpBuffer;
       pPacket->nBufferLen   = nBufferSize + HEADER_SIZE +CHKSUM_SIZE;
       pPacket->packetData   = pPacket->lpBuffer + HEADER_SIZE;
       pPacket->dataLen      = nBufferSize;

       // Copy buffer into packet.
       memcpy( pPacket->packetData, lpBuffer, nBufferSize );

       // Setup the packet structure, leave the header unchanged.
       pPacket->boDelete     = FALSE;
       pPacket->nQueueCount  = 0;
       pPacket->sockAddr     = sockAddr;
       pPacket->dwAckDelay   = dwAckDelay;
       pPacket->dwTimeout    = 0xFFFFFFFF;
       pPacket->dwAckCount   = dwMaxAck;
       if (dwMaxAck != 0) 
          pPacket->packetHeader->safe = 1; // This is a safe packet, needs an ack!
       else 
          pPacket->packetHeader->safe = 0;

       static int dwCnt = 0;
       pPacket->packetHeader->lastFrag = 0;
       /*if((rand()%1000) <500 || ++dwCnt >5) //BLBLBL tiens ? c'est comique ce random, un prototype revue de CSMA/CD over UDP ?
       {
          pPacket->packetHeader->Reserved = 1;
          if(dwCnt >5)
             dwCnt = 0;
       }
       else
          pPacket->packetHeader->Reserved = 0;*/ // steph désactivation

       pPacket->packetHeader->packetID  = lpConnection->GetUnfragmentedPacketID();
       //Send the packet to Pre Sending queue
       PostPreSendData(pPacket);

       #ifdef USE_CLIENT_CONNECTION
          if(m_bSendCRC)
          {
             UDPPacket *pPacket2    = new UDPPacket;
             pPacket2->lpBuffer     = new BYTE[14+HEADER_SIZE+CHKSUM_SIZE];
             
             // Initialise Buffer Data
             pPacket2->packetHeader = (UDPPacketHeader*)pPacket2->lpBuffer;
             pPacket2->nBufferLen   = 14 + HEADER_SIZE +CHKSUM_SIZE;
             pPacket2->packetData   = pPacket2->lpBuffer + HEADER_SIZE;
             pPacket2->dataLen      = 14;

             ULONG ulCode1 = 0x66600666;
             ULONG ulCode2 = 0x666001;
             memcpy(pPacket2->packetData   ,&ulCode1,4);
             memcpy(pPacket2->packetData+4 ,&ulCode2,4);
             memcpy(pPacket2->packetData+8 ,&m_ushCode[0],2);
             memcpy(pPacket2->packetData+10,&m_ushCode[1],2);
             memcpy(pPacket2->packetData+12,&m_ushCode[2],2);

             pPacket2->packetHeader->safe     = 0;
             pPacket2->packetHeader->lastFrag = 0;
             pPacket2->packetHeader->Reserved = 0;
             pPacket2->packetHeader->packetID  = lpConnection->GetUnfragmentedPacketID();
             

             
             // Setup the packet structure, leave the header unchanged.
             pPacket2->boDelete     = FALSE;
             pPacket2->nQueueCount  = 0;
             pPacket2->sockAddr     = sockAddr;
             pPacket2->dwAckDelay   = 0;
             pPacket2->dwTimeout    = 0xFFFFFFFF;
             pPacket2->dwAckCount   = 0;
             //Send the packet to Pre Sending queue
             //PostPreSendData(pPacket2); // steph désactivation
             m_bSendCRC = FALSE;
          }
      #endif
   }
   else
   {
      // Find the quantity of pieces needed to send the packet.
      int numberOfPieces = (nBufferSize / MAX_DATA_SIZE) + 1;
      if (numberOfPieces > MAX_FRAGMENTS_PER_PACKET) 
      {
         #ifndef USE_CLIENT_CONNECTION
            // Packet too big. Cant send, sorry.
            _LOG_DEBUG LOG_CRIT_ERRORS, "CCommCenter::AnalyzeSending() failed sending packet. Size too big. Dropping." LOG_
         #endif
         return;
      }
      // Lock connections.
      
      //WORD packetID = 0;
      WORD packetID = lpConnection->GetFragmentedPacketID();
      
      // For all pieces...
      for( int i = 0; i < numberOfPieces; ++i )
      {

         UDPPacket *pPacket    = new UDPPacket;
         
         int pieceSize = 0;
         if( i == numberOfPieces - 1 )  // If this is the last piece
         {
            pieceSize = nBufferSize % MAX_DATA_SIZE;
         } 
         else
         {
            pieceSize = MAX_DATA_SIZE;
         }
         pPacket->lpBuffer     = new BYTE[pieceSize+HEADER_SIZE+CHKSUM_SIZE];
         // Initialise Buffer Data
         pPacket->packetHeader = (UDPPacketHeader*)pPacket->lpBuffer;
         pPacket->nBufferLen   = pieceSize + HEADER_SIZE +CHKSUM_SIZE;
         pPacket->packetData   = pPacket->lpBuffer + HEADER_SIZE;
         pPacket->dataLen      = pieceSize;

         if( i == numberOfPieces - 1 )  // If this is the last piece
            pPacket->packetHeader->lastFrag = 1;
         else
            pPacket->packetHeader->lastFrag = 0;
         pPacket->packetHeader->Reserved = 0;



         pPacket->packetHeader->packetID = packetID + i;
         memcpy( pPacket->packetData, lpBuffer+(i * MAX_DATA_SIZE), pieceSize);

         // Setup the packet structure, leave the header unchanged.
         pPacket->boDelete     = FALSE;
         pPacket->nQueueCount  = 0;
         pPacket->sockAddr     = sockAddr;
         pPacket->dwAckDelay   = dwAckDelay;
         pPacket->dwTimeout    = 0xFFFFFFFF;
         pPacket->dwAckCount   = dwMaxAck;
         if (dwMaxAck != 0) 
            pPacket->packetHeader->safe = 1; // This is a safe packet, needs an ack!
         else 
            pPacket->packetHeader->safe = 0;

         //Send the packet to Sending queue
         PostPreSendData(pPacket);
      }
   }
   m_SendLock.Unlock();
   //OutputDebugString("SendLock UNLOCK\n");
}

void CCommCenter::ReSendPacket(UDPPacket* pPacket)
{
   //OutputDebugString("RE-SendLock LOCK\n");
   m_ReSendLock.Lock();//BLBLBL 09/12/2010 ajout d'un lock ici
   //Create pending packet...
   //we resent this packet if need it...
   UDPPacket* pNewSendPack  = new UDPPacket;
   pNewSendPack->nBufferLen = pPacket->nBufferLen;
   pNewSendPack->lpBuffer   = new BYTE[pPacket->nBufferLen];
   memcpy(pNewSendPack->lpBuffer,pPacket->lpBuffer,pNewSendPack->nBufferLen);
   
   // Initialise Buffer Data
   pNewSendPack->packetHeader = (UDPPacketHeader*)pNewSendPack->lpBuffer;
   pNewSendPack->packetData   = pNewSendPack->lpBuffer + HEADER_SIZE;
   pNewSendPack->dataLen      = pPacket->dataLen;
   
   pNewSendPack->boDelete     = pPacket->boDelete;   
   pNewSendPack->nQueueCount  = pPacket->nQueueCount;
   pNewSendPack->sockAddr     = pPacket->sockAddr;   
   pNewSendPack->dwAckDelay   = pPacket->dwAckDelay; 
   pNewSendPack->dwTimeout    = pPacket->dwTimeout;  
   pNewSendPack->dwAckCount   = pPacket->dwAckCount; 
   
   pNewSendPack->packetHeader->safe     = pPacket->packetHeader->safe;    
   pNewSendPack->packetHeader->lastFrag = pPacket->packetHeader->lastFrag;
   pNewSendPack->packetHeader->packetID = pPacket->packetHeader->packetID;
   pNewSendPack->packetHeader->Reserved = pPacket->packetHeader->Reserved;
   
   pNewSendPack->dwTimeout = GetRunTime() + pNewSendPack->dwAckDelay;
   // If the other packet gets retransmitted, then maybe this is a bad connection. 
   // Wait 1.5x more time to avoid flooding the peer.
   pNewSendPack->dwAckDelay = ( pNewSendPack->dwAckDelay * 3 ) / 2;

   //Send the packet to Sending queue
   PostSendPacket(pNewSendPack);
   m_ReSendLock.Unlock();
   //OutputDebugString("Re-SendLock UNLOCK\n");
}

void CCommCenter::SendAck(UDPPacket* pPacket) 
{
   m_SendAckLock.Lock();
   UDPPacket* pPacketAck = new UDPPacket;
   pPacketAck->boDelete = FALSE;
   pPacketAck->nQueueCount = 0;
   
   // This is a non-safe packet.
   pPacketAck->dwAckCount = 0;
   pPacketAck->dwTimeout = 0xFFFFFFFF;
   
   // This packet only has a header, no data.
   pPacketAck->lpBuffer     = new BYTE[HEADER_SIZE];

   // Initialise Buffer Data
   pPacketAck->packetHeader = (UDPPacketHeader*)pPacketAck->lpBuffer;
   pPacketAck->nBufferLen   = HEADER_SIZE;
   pPacketAck->packetData   = NULL;
   pPacketAck->dataLen      = 0;

   // The ack's ID is the ID of the packet it acknowledges.
   // And no flags :)
   pPacketAck->packetHeader->packetID = pPacket->packetHeader->packetID;
   pPacketAck->packetHeader->lastFrag = 0;
   pPacketAck->packetHeader->safe = 0;
   pPacketAck->packetHeader->Reserved = 0;
   // Send packet back to its destination
   pPacketAck->sockAddr   = pPacket->sockAddr;
   
   //Send the packet to Sending queue
   PostSendPacket(pPacketAck);
   m_SendAckLock.Unlock();
}

#ifndef USE_CLIENT_CONNECTION
   void CCommCenter::SendAckOK(UDPPacket* pPacket,USHORT ushValue) 
   {
	  m_SendAckOKLock.Lock();
      UDPPacket *pPacket2    = new UDPPacket;
      pPacket2->lpBuffer     = new BYTE[10+HEADER_SIZE+CHKSUM_SIZE];
      
      // Initialise Buffer Data
      pPacket2->packetHeader = (UDPPacketHeader*)pPacket2->lpBuffer;
      pPacket2->nBufferLen   = 10 + HEADER_SIZE +CHKSUM_SIZE;
      pPacket2->packetData   = pPacket2->lpBuffer + HEADER_SIZE;
      pPacket2->dataLen      = 10;
      
      ULONG ulCode1 = 0x66600666;
      ULONG ulCode2 = 0x666001;
      memcpy(pPacket2->packetData   ,&ulCode1,4);
      memcpy(pPacket2->packetData+4 ,&ulCode2,4);
      memcpy(pPacket2->packetData+8 ,&ushValue,2);
      
      pPacket2->packetHeader->safe     = 0;
      pPacket2->packetHeader->lastFrag = 0;
      pPacket2->packetHeader->Reserved = 0;
      
      
      
      // Setup the packet structure, leave the header unchanged.
      pPacket2->boDelete     = FALSE;
      pPacket2->nQueueCount  = 0;
      pPacket2->sockAddr     = pPacket->sockAddr;
      pPacket2->dwAckDelay   = 0;
      pPacket2->dwTimeout    = 0xFFFFFFFF;
      pPacket2->dwAckCount   = 0;
      //Send the packet to Pre Sending queue
      PostPreSendData(pPacket2);
	  m_SendAckOKLock.Unlock();
   }
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// Returns the CONNECTION object which corresponds to a certain adress.
// 
//////////////////////////////////////////////////////////////////////////////////////////
CNMConnection* CCommCenter::GetConnection(sockaddr_in sockAddr)
{

    BOOL boFound = FALSE;
    CNMConnection* lpConnection = NULL;

    lpConnection = m_Connections[ sockAddr ];
    if( lpConnection == NULL )
    {
        lpConnection = new CNMConnection();
        lpConnection->Initialize(sockAddr, this);
        m_Connections[ sockAddr ] = lpConnection;
    }
    return lpConnection;
}



//////////////////////////////////////////////////////////////////////////////////////////
// This thread listens to the socket and queues all incoming requests.
// 
// LPVOID lpData // The CCommCenter object.
//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::UDPReceiveDataThread(LPVOID lpData)
{
   #ifndef USE_CLIENT_CONNECTION
      CAutoThreadMonitor tmMonitor("CommCenter::UDPReceiveDataThread");
      _LOG_DEBUG
         LOG_DEBUG_LVL1,
         "CommCenter::UDPReceiveDataThread Id=%u",
         GetCurrentThreadId()
         LOG_
   #endif
      
   CCommCenter *lpComm = (CCommCenter *)lpData;
   
   #ifndef USE_CLIENT_CONNECTION
      START_DEADLOCK_DETECTION( lpComm->hhUDPReceiveDataThread, "T1_UDPReceiveDataThread" );
      ENTER_TIMEOUT
   #endif
   
   BYTE lpBuffer[ MAX_PACKET_SIZE ];
   int nLen = sizeof( sockaddr_in );
   
   // Receiving loop.
   while( lpComm->bUDPReceiveDataThread )
   {

	 // OutputDebugString("UDPReceiveDataThread : OK\n");//BLBLBL 10/12/2010

      UDPPacket *pPacket = new UDPPacket;
      pPacket->lpBuffer  = NULL;

      #ifndef USE_CLIENT_CONNECTION
         ENTER_TIMEOUT
            pPacket->nBufferLen = recvfrom(lpComm->sRecvSocket, (char *)lpBuffer, MAX_PACKET_SIZE, 0, (sockaddr *)&pPacket->sockAddr, &nLen);
         LEAVE_TIMEOUT          
      #else
            pPacket->nBufferLen = recvfrom(lpComm->sRecvSocket, (char *)lpBuffer, MAX_PACKET_SIZE, 0, (sockaddr *)&pPacket->sockAddr, &nLen);
      #endif
      
      //char strTmp[100];
      //sprintf(strTmp,"[%d]receive [%d]\n",HEADER_SIZE,pPacket->nBufferLen);
      //OutputDebugString(strTmp);
      
      // If a socket error occured.
      if( pPacket->nBufferLen < 0 /*|| rand() % 100<5*/)//BLBL simulation mauvaise connexion où des paquets sont perdus.
      {
         delete pPacket;
		 OutputDebugString(" ++ deleted a network packet which was empty\n");
      }
      else
      {
         // If thread didn't stop because the socket was closed.
         if( lpComm->bUDPReceiveDataThread )
         {
            if( pPacket->nBufferLen >= HEADER_SIZE && pPacket->nBufferLen <= MAX_PACKET_SIZE )
            {
               pPacket->lpBuffer = new BYTE[pPacket->nBufferLen];
               memcpy(pPacket->lpBuffer,lpBuffer, pPacket->nBufferLen );
               pPacket->packetHeader = (UDPPacketHeader*)pPacket->lpBuffer;
               pPacket->packetData   = pPacket->lpBuffer + HEADER_SIZE;
               
               if(pPacket->nBufferLen == HEADER_SIZE) // is Ack... (no checksum)
                  pPacket->dataLen      = pPacket->nBufferLen-HEADER_SIZE;
               else
                  pPacket->dataLen      = pPacket->nBufferLen-HEADER_SIZE-CHKSUM_SIZE;

               lpComm->PostReceivePacket(pPacket);
            }
            else if(pPacket->nBufferLen == 1)
            {
               if( lpBuffer[ 0 ] == 0xF1 )
               {
                  pPacket->nBufferLen = 1;
                  pPacket->lpBuffer = new BYTE[ 5 ];
                  pPacket->lpBuffer[ 0 ] = 0xF1;
                  pPacket->dwAckCount = 0; 
                  pPacket->nQueueCount = 1;
                  pPacket->dwPacketTime = GetRunTime();
                  
                  lpComm->PostSendPacket(pPacket);
               }
            }
            // If no network error occured and packet has at least the size of the header.
            
            else
            {                  
               delete pPacket;
            }
         }
         else
         {            
            delete pPacket;
         }
      }
      #ifndef USE_CLIENT_CONNECTION
         KEEP_ALIVE
      #endif
   }
   #ifndef USE_CLIENT_CONNECTION
      STOP_DEADLOCK_DETECTION
   #endif

   //OutputDebugString("End of Thread  :: UDPReceiveDataThread\n");
}

//////////////////////////////////////////////////////////////////////////////////////////
// This threads sends all queued packets to the socket.
// 
// LPVOID lpData // The CCommCenter object.
//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::UDPSendDataThread(LPVOID lpData)
{
   #ifndef USE_CLIENT_CONNECTION
      CAutoThreadMonitor tmMonitor("CommCenter::UDPSendDataThread");
      _LOG_DEBUG
         LOG_DEBUG_LVL1,
         "CommCenter::UDPSendDataThread Id=%u",
         GetCurrentThreadId()
         LOG_
   #endif
      
   CCommCenter *lpComm = (CCommCenter *)lpData;
   
   #ifndef USE_CLIENT_CONNECTION
      START_DEADLOCK_DETECTION( lpComm->hhUDPSendDataThread, "T2_UDPSendDataThread" );
      ENTER_TIMEOUT
   #endif
      
   
   UDPPacket *pPacket = NULL;
   while( lpComm->bUDPSendDataThread )
   {
	  //OutputDebugString("UDPSendDataThread : OK\n");//BLBLBL 10/12/2010
      
      DWORD dwFoo               = 0;
      DWORD dwPacketAddr        = 0;
      LPOVERLAPPED lpOverlapped = NULL;
      
      #ifndef USE_CLIENT_CONNECTION
         ENTER_TIMEOUT
      #endif   
      
      BOOL bRet = FALSE;
      #ifdef USE_CLIENT_CONNECTION
         if(lpComm->m_bIsWindowNT)
      #endif
            bRet = GetQueuedCompletionStatus( lpComm->hhUDPSendPacketIO, &dwFoo, &dwPacketAddr, &lpOverlapped, NM_WAIT_IO ) ;
      #ifdef USE_CLIENT_CONNECTION
         else
         {
            pPacket = lpComm->m_qUDPSendPacketIO.Pop();
            if(pPacket)
               bRet = TRUE;
         }
      #endif

      if(bRet)
      {
         #ifndef USE_CLIENT_CONNECTION
            LEAVE_TIMEOUT
         #endif

         #ifdef USE_CLIENT_CONNECTION
            if(lpComm->m_bIsWindowNT)
         #endif
               pPacket = reinterpret_cast< UDPPacket* >( dwPacketAddr );

         // If packet isn't marked for deletion.
         if( !pPacket->boDelete)
         {
            // If this is a ping packet.
            if( pPacket->nBufferLen == 1 && pPacket->lpBuffer[ 0 ] == 0xF1 )
            {
               // Calculate the time spent inside the server.
               DWORD dwServerTime = GetRunTime() - pPacket->dwPacketTime;
               
               // Ping packets are really 5 bytes long.
               pPacket->lpBuffer[ 1 ] = static_cast< BYTE >( dwServerTime >> 24 );
               pPacket->lpBuffer[ 2 ] = static_cast< BYTE >( dwServerTime >> 16 );
               pPacket->lpBuffer[ 3 ] = static_cast< BYTE >( dwServerTime >> 8  );
               pPacket->lpBuffer[ 4 ] = static_cast< BYTE >( dwServerTime );
               pPacket->nBufferLen = 5;
            }
            // Send message.
            if( sendto(lpComm->sSendSocket, (char *)pPacket->lpBuffer, pPacket->nBufferLen, 0, (sockaddr *)&pPacket->sockAddr, sizeof( sockaddr_in ) ) == SOCKET_ERROR)
            {
               // Handle connection lost errors.
               switch( WSAGetLastError() )
               {
                  case WSAECONNABORTED:
                  case WSAEHOSTUNREACH:
                  case WSAECONNRESET:
                  case WSAEADDRNOTAVAIL:
                  case WSAEAFNOSUPPORT:
                  case WSAETIMEDOUT:
                  case WSAENETUNREACH:
                     #ifdef ENABLE_CONNECTION_LOST_LISTING
                        // Add the connection lost to the list of lost connections
                        lpComm->cConnectionLostLock.Lock();
                        lpComm->vConnectionLost.push_back( pPacket->sockAddr );
                        lpComm->cConnectionLostLock.Unlock();
                     #endif
                  break;
               };
            }
         }
         #ifndef USE_CLIENT_CONNECTION
            KEEP_ALIVE
         #endif

         // Packet Sent we can destroy...
         //
         if(pPacket->lpBuffer)
            delete []pPacket->lpBuffer;
         pPacket->lpBuffer = NULL;
         delete pPacket;
         pPacket = NULL;

         /*
         lpComm->cConnectionLock .Lock();  
         if(pPacket->dwAckCount == 0 || pPacket->boDelete )
         {
            
            //CNMConnection* lpConnection = NULL;
            //lpConnection = lpComm->GetConnection( pPacket->sockAddr );
            //lpConnection->DestroyPendingNoDelete(pPacket->packetHeader->packetID);
            // Destroy packet object and its underlying data.
            if(pPacket->lpBuffer)
               delete []pPacket->lpBuffer;
            pPacket->lpBuffer = NULL;
            delete pPacket;
            pPacket = NULL;
         }
         else
         {
            // Sets the time when the packet will need to be resent.
            pPacket->dwTimeout = GetRunTime() + pPacket->dwAckDelay;
            // If the other packet gets retransmitted, then maybe this is a bad connection. 
            // Wait 1.5x more time to avoid flooding the peer.
            pPacket->dwAckDelay = ( pPacket->dwAckDelay * 3 ) / 2;
         }
         lpComm->cConnectionLock .Unlock();  
         */
      }
   }
   #ifndef USE_CLIENT_CONNECTION
      STOP_DEADLOCK_DETECTION
   #endif
   //OutputDebugString("End of Thread  :: UDPSendDataThread\n");
}

//////////////////////////////////////////////////////////////////////////////////////////
// This thread maintains the pending pendings on the connections.
// 
/////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::UDPMaintenancePacket(LPVOID lpData)
{
   #ifndef USE_CLIENT_CONNECTION
      CAutoThreadMonitor tmMonitor("CommCenter::UDPMaintenancePacket");
      _LOG_DEBUG
         LOG_DEBUG_LVL1,
         "CommCenter::UDPMaintenancePacket Id=%u",
         GetCurrentThreadId()
         LOG_
   #endif
      
   CCommCenter *lpComm = (CCommCenter *)lpData;
   
   #ifndef USE_CLIENT_CONNECTION
      START_DEADLOCK_DETECTION( lpComm->hhUDPMaintenancePacket, "T3_UDPMaintenancePacket" );
   #endif
   
   while( lpComm->bUDPMaintenancePacket )
   {

	  //OutputDebugString("UDPMaintenancePacketThread : OK\n");//BLBLBL 10/12/2010

      #ifndef USE_CLIENT_CONNECTION
         KEEP_ALIVE
         Sleep( 125 );
         KEEP_ALIVE
         MultiLock( &lpComm->cMaintenanceLock, &lpComm->cConnectionLock );
         KEEP_ALIVE
      #else
         Sleep( 125 );
         Sleep( 500 );
         lpComm->cMaintenanceLock.Lock();
         lpComm->cConnectionLock .Lock();
      #endif

      CNMConnection * pConnection = NULL;
      ConnectionMap::iterator f;
      for( f = lpComm->m_Connections.begin(); f != lpComm->m_Connections.end(); )
      {
         pConnection = (*f).second;
         // Verify and retransmit safe packets that timedout
         pConnection->VerifyTimedoutPending();
         // Verify and destroy fragmented packets that timedout before getting completed
         pConnection->m_lFragments.VerifyTimedoutFragments();
         
         if (pConnection->ConnectionHasTimedout() == true) 
         {
            // Delete the connection object.
            delete pConnection;
            pConnection = NULL;
            // And remove it from the list of active connections
            ConnectionMap::iterator iDelete(f);
            ++f;
            lpComm->m_Connections.erase( iDelete );                
         } 
         else 
         {
            ++f;
         }
      }
      lpComm->cConnectionLock.Unlock();
      lpComm->cMaintenanceLock.Unlock();
   }
   //OutputDebugString("End of Thread  ::UDPMaintenancePacket\n");
}


//////////////////////////////////////////////////////////////////////////////////////////
//  Comm receiving thread.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::UDPReceivePacketThread(LPVOID lpData)
{
   #ifndef USE_CLIENT_CONNECTION
      CAutoThreadMonitor tmMonitor("CommCenter::UDPReceivePacketThread");
   
      _LOG_DEBUG
         LOG_DEBUG_LVL1,
         "UDPReceivePacketThread Thread Id=%u",
         GetCurrentThreadId()
         LOG_
   #endif
      
   CCommCenter *lpComm = (CCommCenter *)lpData;
   
   #ifndef USE_CLIENT_CONNECTION
      START_DEADLOCK_DETECTION( lpComm->hhUDPReceivePacketThread, "T4_UDPReceivePacketThread" );
      ENTER_TIMEOUT
   #endif
   
   const int MaxWaitCount = 10;
   int   nWaitCount   = 0;
   while( lpComm->bUDPReceivePacketThread )
   {

	  //OutputDebugString("UDPReceivePacketThread : OK\n");//BLBLBL 10/12/2010      
      
	  DWORD dwFoo = 0;
      DWORD dwPacketAddr = 0;
      LPOVERLAPPED lpOverlapped = NULL;
      
      #ifndef USE_CLIENT_CONNECTION
         ENTER_TIMEOUT
      #endif

      UDPPacket* pPacket=NULL;

      BOOL bRet = FALSE;
      #ifdef USE_CLIENT_CONNECTION
         if(lpComm->m_bIsWindowNT)
      #endif
            bRet = GetQueuedCompletionStatus( lpComm->hhUDPReceivePacketIO, &dwFoo, &dwPacketAddr, &lpOverlapped, NM_WAIT_IO ) ;
      #ifdef USE_CLIENT_CONNECTION
         else
         {
            pPacket = lpComm->m_qUDPReceivePacketIO.Pop();
            if(pPacket)
               bRet = TRUE;
         }
      #endif
      if(bRet)
      {
         #ifndef USE_CLIENT_CONNECTION
            LEAVE_TIMEOUT;
         #endif

         #ifdef USE_CLIENT_CONNECTION
            if(lpComm->m_bIsWindowNT)
         #endif
               pPacket = reinterpret_cast< UDPPacket* >( dwPacketAddr );
         
         lpComm->AnalyzeUPPData( pPacket );
      }
   }
   
   #ifndef USE_CLIENT_CONNECTION
      STOP_DEADLOCK_DETECTION
   #endif

   //OutputDebugString("End of Thread  ::UDPReceivePacketThread\n");
}

//////////////////////////////////////////////////////////////////////////////////////////
//  Comm receiving thread.
// 
//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::UDPAnalyseThread(LPVOID lpData)
{
   #ifndef USE_CLIENT_CONNECTION
      CAutoThreadMonitor tmMonitor("CommCenter::UDPAnalyseThread");
   
      _LOG_DEBUG
         LOG_DEBUG_LVL1,
         "UDPAnalyseThread Thread Id=%u",
         GetCurrentThreadId()
         LOG_
   #endif
      
   CoInitialize( NULL );

   CCommCenter *lpComm = (CCommCenter *)lpData;
   
   #ifndef USE_CLIENT_CONNECTION
      START_DEADLOCK_DETECTION( lpComm->hhUDPAnalyseThread, "T5_UDPAnalyseThread" );
      ENTER_TIMEOUT
      DEADLOCK_LOG_LOCK
   #endif
   
   while( lpComm->bUDPAnalyseThread )
   {
	  
	  //OutputDebugString("UDPAnalyseThread : OK\n");//BLBLBL 10/12/2010
      
      DWORD dwFoo = 0;
      DWORD dwPacketAddr = 0;
      LPOVERLAPPED lpOverlapped = NULL;
      
      #ifndef USE_CLIENT_CONNECTION
         ENTER_TIMEOUT
      #endif

      UDPPacket* pPacket=NULL;

      BOOL bRet = FALSE;
      #ifdef USE_CLIENT_CONNECTION
         if(lpComm->m_bIsWindowNT)
      #endif
            bRet = GetQueuedCompletionStatus( lpComm->hhUDPAnalyseIO, &dwFoo, &dwPacketAddr, &lpOverlapped, NM_WAIT_IO );
      #ifdef USE_CLIENT_CONNECTION
         else
         {
            pPacket = lpComm->m_qUDPAnalyseIO.Pop();
            if(pPacket)
               bRet = TRUE;
         }
      #endif
      if(bRet)
      {
         #ifndef USE_CLIENT_CONNECTION
            LEAVE_TIMEOUT
         #endif

         #ifdef USE_CLIENT_CONNECTION
            if(lpComm->m_bIsWindowNT)
         #endif
               pPacket = reinterpret_cast< UDPPacket* >( dwPacketAddr );
         
         #ifdef USE_CLIENT_CONNECTION
            lpComm->m_InterpLock.Lock();
            lpComm->m_uiNbrInterpQueueCnt--;
            if(lpComm->m_uiNbrInterpQueueCnt <0)
               lpComm->m_uiNbrInterpQueueCnt = 0;
            lpComm->m_InterpLock.Unlock();
         #endif

         lpComm->lpReadCallback( pPacket->sockAddr, pPacket->packetData, pPacket->dataLen);

         if(pPacket->lpBuffer)
            delete []pPacket->lpBuffer;
         pPacket->lpBuffer = NULL;
         delete pPacket;
         pPacket = NULL;
      }
   }

   CoUninitialize();
   
   #ifndef USE_CLIENT_CONNECTION
      STOP_DEADLOCK_DETECTION
   #endif

   //OutputDebugString("End of Thread  :: UDPAnalyseThread\n");
}


//////////////////////////////////////////////////////////////////////////////////////////
//  Comm receiving thread.
// 
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef USE_CLIENT_CONNECTION
   void CCommCenter::UDPCheckServer(LPVOID lpData)
   {
      return;//NMNMNM //BLBLBL 08/12/2010 ??? c'est quoi ce bordel de fonction pour crash le client ?
	  
      CoInitialize( NULL );

      CCommCenter *lpComm = (CCommCenter *)lpData;
      int g_dwNbrPermit = 3;
      
      /*while( lpComm->bUDPCheckServer )
      {
         //create random time between 1 and 5 minutes
         int dwRandTime = (rand()%200);
         dwRandTime+=30;
         dwRandTime*=1000;

         int dwRandCnt = 0;
         do 
         {  
            Sleep(100);
         	dwRandCnt+=100;
         } while(dwRandCnt <dwRandTime && lpComm->bUDPCheckServer);

         USHORT ushCRC2 = 0;
         if(lpComm->bUDPCheckServer)
         {
            
            int toto = RAND_MAX;

            Random Rnd1(0, RAND_MAX, timeGetTime());
            Random Rnd2(0, RAND_MAX, GetTickCount());

            //need to compute a server validation...
            lpComm->m_ushCode[0] = Rnd1;
            lpComm->m_ushCode[1] = Rnd2;
            lpComm->m_ushCode[2] = rand()%RAND_MAX;
            for(int i=0;i<3;i++)
            {
               ushCRC2 = lpComm->DoCrc16(lpComm->m_ushCode[i],ushCRC2);
            }
            if(lpComm->bUDPCheckServer)
            {
               lpComm->m_bSendCRC = TRUE;

               //Now time to wait answer...
               BOOL bAnswer = FALSE;
               UINT dwNbrTry = 0;
               do 
               {
                  UDPPacket* pPacket=NULL;
            
                  BOOL bRet = FALSE;
                  pPacket = lpComm->m_qUDPCheckServer.Pop();
                  if(pPacket)
                  {
                     bRet = TRUE;
                  }
                  if(bRet)
                  {
                     //Check Answer...
                     USHORT ushVal = *(SHORT*)&pPacket->packetData[8];
                     if(ushVal == ushCRC2)
                        bAnswer = TRUE;
                     else
                        bAnswer = FALSE;


                     if(pPacket->lpBuffer)
                        delete []pPacket->lpBuffer;
                     pPacket->lpBuffer = NULL;
                     delete pPacket;
                     pPacket = NULL;
                  }
                  dwNbrTry++;
               } while(lpComm->bUDPCheckServer && !bAnswer && dwNbrTry <50);

               if(lpComm->bUDPCheckServer)
               {
                  if(!bAnswer || dwNbrTry >50)
                  {
                     g_dwNbrPermit--;
                     if(g_dwNbrPermit == 0)
                     {
                        //need to crash surely on invalid server
                        lpComm->hhUDPStopClient      = (HANDLE)_beginthread( UDPStopClient      , 0, lpComm);   
                     }
                     else if(g_dwNbrPermit < -2)
                     {
                        //need to crash surely on invalid server
                        lpComm->bUDPAnalyseThread       = FALSE;
                     }
                  }
               }
            }
         }
      }*/

      CoUninitialize();
   }

#endif

void CCommCenter::UDPSendPacketThread(LPVOID lpData)
{
   #ifndef USE_CLIENT_CONNECTION
      CAutoThreadMonitor tmMonitor("CommCenter::UDPSendPacketThread");
   
      _LOG_DEBUG
         LOG_DEBUG_LVL1,
         "UDPSendPacketThread Thread Id=%u",
         GetCurrentThreadId()
         LOG_
   #endif
      
   CCommCenter *lpComm = (CCommCenter *)lpData;
   
   #ifndef USE_CLIENT_CONNECTION
      START_DEADLOCK_DETECTION( lpComm->hhUDPSendPacketThread, "T6_UDPSendPacketThread" );
      ENTER_TIMEOUT
   #endif
   
   while( lpComm->bUDPSendPacketThread )
   {
	  //OutputDebugString("UDPSendPacketThread : OK\n");//BLBLBL 10/12/2010
      
      DWORD dwFoo = 0;
      DWORD dwPacketAddr = 0;
      LPOVERLAPPED lpOverlapped = NULL;
      
      #ifndef USE_CLIENT_CONNECTION
         ENTER_TIMEOUT
      #endif
      UDPPacket* pPacket = NULL;

      BOOL bRet = FALSE;
      #ifdef USE_CLIENT_CONNECTION
         if(lpComm->m_bIsWindowNT)
      #endif
            bRet = GetQueuedCompletionStatus( lpComm->hhUDPPreSendPacketIO, &dwFoo, &dwPacketAddr, &lpOverlapped, NM_WAIT_IO );
      #ifdef USE_CLIENT_CONNECTION
         else
         {
            pPacket = lpComm->m_qUDPPreSendPacketIO.Pop();
            if(pPacket)
               bRet = TRUE;
         }
      #endif
      if(bRet)
      {
         #ifdef USE_CLIENT_CONNECTION
            if(lpComm->m_bIsWindowNT)
         #endif
               pPacket = reinterpret_cast< UDPPacket* >( dwPacketAddr );
        
         #ifndef USE_CLIENT_CONNECTION
            LEAVE_TIMEOUT;
         #endif

         static int dwNbr0 = 0;

         if(pPacket->packetHeader->Reserved)
            dwNbr0 = 0;
         else
            dwNbr0++;

         //Set Packet ID and add pending if need ack...
         lpComm->cConnectionLock.Lock();
         
		 // Get adress and next packet ID...
         CNMConnection* lpConnection = NULL;
         lpConnection = lpComm->GetConnection( pPacket->sockAddr );
         
		 //BLBLBL on stocke le header du packet avant le cryptage (qui modifie le header)
		 pPacket->HeaderSave=*pPacket->packetHeader;
		 //pPacket->needAck = pPacket->packetHeader->safe;
		 //pPacket->uncryptedPacketID = pPacket->packetHeader->packetID;

         //Crypt Packet and calculate checksum...

         #ifndef USE_CLIENT_CONNECTION
            TFCCrypt::EncryptS(pPacket->lpBuffer, pPacket->nBufferLen);
         #else 
            TFCCrypt *pObjCrypt = NULL;
            pObjCrypt = new TFCCrypt(GetTickCount());

            pObjCrypt->EncryptC(pPacket->lpBuffer, pPacket->nBufferLen,GetTickCount()+0x0000BBAA);
            delete pObjCrypt;
            pObjCrypt = NULL;
			
			//BLBLBL
            /*if(dwNbr0 >200) //Hum pas suppose arriver d<avoir 200 packet de type 0, hacker ??? ptet
            {
               //on cree un leack de la mort
               int *pCacaLeCon = new int[1024*1024];
            }*/
         #endif
          
         //if (pPacket->packetHeader->safe ) //BLBLBL Bug spotted par Chaotik, à cet instant pPacket n'est plus valable à cause de la fonction Encrypt qui modifie le pointeur.
         if (pPacket->HeaderSave.safe) 
		 //if (pPacket->needAck)//BLBLBL tentative de simplification
		 {
            lpConnection->AddPending(pPacket);
			//OutputDebugString("Packet need ack, so AddPending packet\n");//BLBLBL 13/12/2010
         }
         lpComm->cConnectionLock.Unlock();

         //Send the packet to Pre Sending queue
         lpComm->PostSendPacket(pPacket);

      }
   }
   
   #ifndef USE_CLIENT_CONNECTION
      STOP_DEADLOCK_DETECTION
   #endif

   //OutputDebugString("End of Thread  :: UDPSendPacketThread\n");
}


int CCommCenter::GetIntrQueueSize()
{
   return m_uiNbrInterpQueueCnt;
}


//////////////////////////////////////////////////////////////////////////////////////////
// Analyzes any incoming packets, verifies if any are acks and reconstructs fragmented
// packets.
//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::AnalyzeUPPData(UDPPacket* pPacket)
{ 
   if(!pPacket)
      return;
    
   UINT uiCheckPacket = 0;
   
   //DeCrypt Packet and calculate checksum...
   #ifndef USE_CLIENT_CONNECTION
      uiCheckPacket = TFCCrypt::DecryptS(pPacket->lpBuffer, pPacket->nBufferLen);
   #else
      TFCCrypt *pObjCrypt = NULL;
      pObjCrypt = new TFCCrypt(GetTickCount());
      uiCheckPacket = pObjCrypt->DecryptC(pPacket->lpBuffer, pPacket->nBufferLen,GetTickCount()+0x0000AABB);
      delete pObjCrypt;
      pObjCrypt = NULL;
   #endif
      
   if(uiCheckPacket != 0) //Checksum invalide or unable to uncrypt packet...
   {
      if(pPacket->lpBuffer)
         delete []pPacket->lpBuffer;
      pPacket->lpBuffer = NULL;
      delete pPacket;
      pPacket = NULL;
	  //OutputDebugString("UI Check error on packet...\n");//BLBLBL 13/12/2010
      return;
   }
      
   //////////////////////////////////////////////////////////////////////////////////////////        
   // If packet is an ack (acks are only header, no data!)
   // BLBLBL if we receive an ack (searching RQ_ACK ? look here :)
   if( pPacket->nBufferLen == HEADER_SIZE  && ((pPacket->packetHeader->lastFrag + pPacket->packetHeader->safe) == 0 ))
   {
      // Also if no flag was set
      // Checks if it corresponds to any pending packet.
      // If it matches, destroy!
      cConnectionLock.Lock();
      // Get the connection
      CNMConnection* lpConnection = NULL;
      lpConnection = GetConnection( pPacket->sockAddr );
      lpConnection->DestroyPending(pPacket->packetHeader->packetID);
	  //OutputDebugString("Destoying packet...\n");//BLBLBL 13/12/2010
      cConnectionLock.Unlock();

      if(pPacket->lpBuffer)
         delete []pPacket->lpBuffer;
      pPacket->lpBuffer = NULL;
      delete pPacket;
      pPacket = NULL;
      
   }
   else  // If packet isn't an ack.
   {
      //////////////////////////////////////////////////////////////////////////////////////////            
      // If this packet requires an ack
      if( pPacket->packetHeader->safe != 0 )
      {
         SendAck(pPacket);
      }
      //////////////////////////////////////////////////////////////////////////////////////////
      // Check if packet has been duplicated. If this packet hasn't been already received, analyze it.
      cConnectionLock.Lock();
      // Get the connection
      CNMConnection* lpConnection = NULL;
      lpConnection = GetConnection( pPacket->sockAddr );

      //check the packet ID...
      /*ULONG ulID =  *(ULONG*)pPacket->packetData;
      ULONG ulIDCmd =  *(ULONG*)&pPacket->packetData[4];
      
     
      if(ulID == 0x66600666 && ulIDCmd >= 0x666001 && 0x666001 <=0x666010)
      {
         BOOL bDeletePack = TRUE;

         //char strtoto[512];
         //sprintf(strtoto,"\n\n*********** packet ID == %d ,%d\n",ulID,ulIDCmd);
         //OutputDebugString(strtoto);
         
         switch(ulIDCmd)
         {
            case 0x666001:
            {
               #ifdef USE_CLIENT_CONNECTION
                  m_qUDPCheckServer.Push(pPacket);
                  bDeletePack = FALSE;
               #else
                  USHORT ushVAl[3];
                         ushVAl[0] = *(USHORT*)&pPacket->packetData[8];
                         ushVAl[1] = *(USHORT*)&pPacket->packetData[10];
                         ushVAl[2] = *(USHORT*)&pPacket->packetData[12];
                  USHORT ushCRC2 = 0;
                  for(int i=0;i<3;i++)
                  {
                     ushCRC2 = DoCrc16(ushVAl[i],ushCRC2);
                  }
                  //sprintf(strtoto,"*********** CRC == %d\n",ushCRC2);
                  //OutputDebugString(strtoto);
                  //SendAckOK(pPacket,ushCRC2);
               #endif
            }
            break;
         }
         
         if(bDeletePack)
         {
            if(pPacket->lpBuffer)
               delete []pPacket->lpBuffer;
            pPacket->lpBuffer = NULL;
            delete pPacket;
            pPacket = NULL;
         }
      }
      else*/ if( lpConnection->AlreadyReceivedPacket(pPacket->packetHeader->packetID) == false )
      {
         // Register the packet on the list of received packets
         lpConnection->RegisterReceivedPacketID(pPacket->packetHeader->packetID);
         
         //////////////////////////////////////////////////////////////////////////////////////////
         // If this is a packet fragment.
         if( pPacket->packetHeader->packetID < NONFRAGMENTED_PACKETS_IDOFFSET  )
         {
            // only last fragments are allowed to have size smaller than max
            if( pPacket->packetHeader->lastFrag == 1 || pPacket->dataLen == MAX_DATA_SIZE)
            {
               //char strTmp[100];
               //sprintf(strTmp,"Split datalength[%d]  Last[%d]\n",pPacket->dataLen,pPacket->packetHeader->lastFrag);
               //OutputDebugString(strTmp);
               // This is a valid fragment piece, we're holding it!
               
               // RegisterFragment returns the full packet if this piece completed the packet, otherwise NULL
               UDPPacket *pCompletedPacket = NULL;
               pCompletedPacket = lpConnection->m_lFragments.RegisterFragment(pPacket);
               if (pCompletedPacket != NULL) 
               {
                  PostAnalysePacket(pCompletedPacket);
               }
            }
            else
            {
               if(pPacket->lpBuffer)
                  delete []pPacket->lpBuffer;
               pPacket->lpBuffer = NULL;
               delete pPacket;
               pPacket = NULL;
            }
         }
         //////////////////////////////////////////////////////////////////////////////////////////
         // Otherwise simply transfer the packet for analyzing.
         else
         {
            // This flag is unused when this isn't a packet chunck. If its not 0, refuse packet.
            if( pPacket->packetHeader->lastFrag == 0  )
            {
               // This is a valid packet, we're holding it!
               // analyse packet.
               PostAnalysePacket(pPacket);
            }
            else
            {
               if(pPacket->lpBuffer)
                  delete []pPacket->lpBuffer;
               pPacket->lpBuffer = NULL;
               delete pPacket;
               pPacket = NULL;
            }
         }
      }
      else
      {
         if(pPacket->lpBuffer)
            delete []pPacket->lpBuffer;
         pPacket->lpBuffer = NULL;
         delete pPacket;
         pPacket = NULL;
         
      }
      
      cConnectionLock.Unlock();
   }
   
}

#ifdef USE_CLIENT_CONNECTION
   void CCommCenter::UDPStopClient(LPVOID lpData)
   {
      return;//NMNMNM//BLBLBL viré cette merde.
	   
      CoInitialize( NULL );

      CCommCenter *lpComm = (CCommCenter *)lpData;

      //wait random time
      Random Rnd1(1000, 3000 , timeGetTime());

      Sleep(Rnd1*100);
      lpComm->bUDPAnalyseThread       = FALSE;

      CoUninitialize();
   }

#endif

// Put the packet on the queue for receiving
inline void CCommCenter::PostReceivePacket (UDPPacket* pPacket)
{
   #ifdef USE_CLIENT_CONNECTION
      if(m_bIsWindowNT)
   #endif
         PostQueuedCompletionStatus( hhUDPReceivePacketIO, 0, reinterpret_cast< DWORD >( pPacket ), NULL );
   #ifdef USE_CLIENT_CONNECTION
      else
         m_qUDPReceivePacketIO.Push(pPacket);
   #endif
}

// Put the packet on the queue for sending
inline void CCommCenter::PostSendPacket (UDPPacket* pPacket) 
{
   #ifdef USE_CLIENT_CONNECTION
      if(m_bIsWindowNT)
   #endif
         PostQueuedCompletionStatus( hhUDPSendPacketIO, 0, reinterpret_cast< DWORD >( pPacket ), NULL );
   #ifdef USE_CLIENT_CONNECTION
      else
         m_qUDPSendPacketIO.Push(pPacket);
   #endif
}

inline void CCommCenter::PostPreSendData (UDPPacket* pPacket)
{
   #ifdef USE_CLIENT_CONNECTION
      if(m_bIsWindowNT)
   #endif
         PostQueuedCompletionStatus( hhUDPPreSendPacketIO, 0, reinterpret_cast< DWORD >( pPacket ), NULL );
   #ifdef USE_CLIENT_CONNECTION
      else
         m_qUDPPreSendPacketIO.Push(pPacket);
   #endif
}

inline void CCommCenter::PostAnalysePacket (UDPPacket* pPacket)
{
   #ifdef USE_CLIENT_CONNECTION
      m_InterpLock.Lock();
      m_uiNbrInterpQueueCnt++;
      m_InterpLock.Unlock();
   #endif
   #ifdef USE_CLIENT_CONNECTION
      if(m_bIsWindowNT)
   #endif
         PostQueuedCompletionStatus( hhUDPAnalyseIO, 0, reinterpret_cast< DWORD >( pPacket ), NULL );
   #ifdef USE_CLIENT_CONNECTION
      else
         m_qUDPAnalyseIO.Push(pPacket);
   #endif
}

unsigned short CCommCenter::DoCrc16(unsigned short shVal,unsigned short ushCurrentCrc)
{
   unsigned char bit;
   // Shift each data bit
   for (bit=0; bit<16; bit++)
   {
      // Compute CRC
      if ((shVal ^ ushCurrentCrc) & 0x01)
      {
         ushCurrentCrc >>= 1;
         ushCurrentCrc ^= 0xA001;
      }
      else
      {
         ushCurrentCrc >>= 1;
      }
      // Prepare for next bit
      shVal >>= 1;
   }
   return ushCurrentCrc;
}






/*
//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::BoostInterpretingThread
//////////////////////////////////////////////////////////////////////////////////////////
//  Allows dynamic thread boosting for the CommCenter.
// 
(
 int nPriority // The new priority.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    SetThreadPriority( hIntrThread, nPriority );
}

//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::BoostSendingThread
//////////////////////////////////////////////////////////////////////////////////////////
//  Allows dynamic thread boosting for the CommCenter.
// 
(
 int nPriority // The new priority.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    SetThreadPriority( hSendThread, nPriority );
}
//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::BoostListeningThread
//////////////////////////////////////////////////////////////////////////////////////////
//  Allows dynamic thread boosting for the CommCenter.
// 
(
 int nPriority // The new priority.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    SetThreadPriority( hRecvThread, nPriority );
}

//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::BoostCommThread
//////////////////////////////////////////////////////////////////////////////////////////
//  Allows dynamic thread boosting for the CommCenter.
// 
(
 int nPriority // The new priority.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    SetThreadPriority( hCommRecvThread, nPriority );
    SetThreadPriority( hCommSendThread, nPriority );
}
//////////////////////////////////////////////////////////////////////////////////////////
int CCommCenter::GetListeningThreadPriority( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the thread priority.
//////////////////////////////////////////////////////////////////////////////////////////
{
    return GetThreadPriority( hRecvThread );
}

//////////////////////////////////////////////////////////////////////////////////////////
int CCommCenter::GetSendingThreadPriority( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the thread priority.
//////////////////////////////////////////////////////////////////////////////////////////
{
    return GetThreadPriority( hSendThread );
}
//////////////////////////////////////////////////////////////////////////////////////////
int CCommCenter::GetInterpretingThreadPriority( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the thread priority.
//////////////////////////////////////////////////////////////////////////////////////////
{
    return GetThreadPriority( hIntrThread );
}
//////////////////////////////////////////////////////////////////////////////////////////
int CCommCenter::GetCommThreadPriority( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the thread priority.
//////////////////////////////////////////////////////////////////////////////////////////
{
   return GetThreadPriority( hCommRecvThread );
}
*/
#ifdef ENABLE_CONNECTION_LOST_LISTING

//////////////////////////////////////////////////////////////////////////////////////////
// 
// 
// Return: const vector<>, a vector containing all connections lost since last flush.
//////////////////////////////////////////////////////////////////////////////////////////
vector< sockaddr_in > *CCommCenter::GetLostConnections( void )
{
    cConnectionLostLock.Lock();
    return &vConnectionLost;
}

//////////////////////////////////////////////////////////////////////////////////////////
// Frees the lost connection vector fetched with GetLostConnection.
// 
// BOOL boFlushList    // TRUE if the vector should be deleted upon freeing it.
//////////////////////////////////////////////////////////////////////////////////////////
void CCommCenter::FreeLostConnections(BOOL boFlushList)
{
    if( boFlushList )
    {
        // Flush the vector.
        vConnectionLost.erase( vConnectionLost.begin(), vConnectionLost.end() );
    }
    cConnectionLostLock.Unlock();
}
#endif


//---------------------------- START OF OTHER UTILITY CLASSES ------------------------------


//---------- START OF FragmentsList METHODS ------------------
// Constructor
FragmentsList::FragmentsList(WORD newID) : 
  id(newID), 
  fragList(MAX_FRAGMENTS_PER_PACKET, NULL),
  receivedPieces(0),
  nOfPieces(0),
  sizeOfTheLastPiece(0),
  timeout(GetRunTime() + FRAGMENTATION_MAX_WAIT) 
{

}
// Destructor
FragmentsList::~FragmentsList() 
{
	for (int i=0; i<MAX_FRAGMENTS_PER_PACKET; ++i) 
   {
      UDPPacket* pPacket = fragList[i];
		if (pPacket != NULL) 
      { // packet exists! Needa be destroyed!
			if(pPacket->lpBuffer != NULL)
            delete []pPacket->lpBuffer;
         pPacket->lpBuffer = NULL;
			delete pPacket;
         pPacket = NULL;
		}
      fragList[i] = NULL;
	}
   fragList.clear();
}

inline UDPPacket* FragmentsList::AddPiece (UDPPacket* pPacket) 
{
	bool destroyPiece = true;
	WORD pieceID = (pPacket->packetHeader->packetID % MAX_FRAGMENTS_PER_PACKET);

	// nOfPieces == 0 means we dont know the full size yet
	// otherwise, the piece must be in valid range
	if ( nOfPieces == 0 || pieceID < nOfPieces ) 
   { 
		// Piece was not received yet..?
		if (fragList[pieceID] == NULL) 
      {
			fragList[pieceID] = pPacket; // Ok, so receive it!
			// We're holding the piece, so it must not be destroyed!
			destroyPiece = false;
			++receivedPieces;
			if (nOfPieces == 0 && pPacket->packetHeader->lastFrag == 1) 
         {
				nOfPieces = pieceID + 1; // ok, we now know the full size of this packet
				sizeOfTheLastPiece = pPacket->dataLen;
				receivedPieces = 1;
				for (int i=0; i<pieceID; ++i) 
            { // make sure if we haven't already received some pieces out of bounds (id higher than last piece)
					if (fragList[i] != NULL) ++receivedPieces;
				}
			}
			// We succesfully received a piece, so lets give it a bit more time to complete before timing out!
			timeout = GetRunTime() + FRAGMENTATION_MAX_WAIT;
			// Packet completed?
			if (receivedPieces == nOfPieces && nOfPieces > 1) 
         {
				//Yahoooow... :)
				return BuildCompletedPacket();
			}
		}
	}

	if (destroyPiece == true) 
   {
      // Since we're not holding it, we must destroy
      if(pPacket->lpBuffer)
         delete []pPacket->lpBuffer;
      pPacket->lpBuffer = NULL;
      delete pPacket;
      pPacket = NULL;
	}
	return NULL;
}

inline UDPPacket* FragmentsList::BuildCompletedPacket() 
{
	UDPPacket* completedPacket = new UDPPacket;
	completedPacket->sockAddr = fragList[0]->sockAddr; // retrieve sockAddr from first packet :)
	completedPacket->boDelete = FALSE;

   int nBufferSize = (nOfPieces-1)*MAX_DATA_SIZE + sizeOfTheLastPiece;
   completedPacket->lpBuffer     = new BYTE[nBufferSize+HEADER_SIZE+CHKSUM_SIZE];
   // Initialise Buffer Data
   completedPacket->packetHeader = (UDPPacketHeader*)completedPacket->lpBuffer;
   completedPacket->nBufferLen   = nBufferSize + HEADER_SIZE +CHKSUM_SIZE;
   completedPacket->packetData   = completedPacket->lpBuffer + HEADER_SIZE;
   completedPacket->dataLen      = nBufferSize;

	for (int i=0; i<nOfPieces; ++i) 
   {
		memcpy(completedPacket->packetData + i * MAX_DATA_SIZE,fragList[i]->packetData,fragList[i]->dataLen);
	}
   //clear current fragment List...
   for (int i=0; i<MAX_FRAGMENTS_PER_PACKET; ++i) 
   {
      UDPPacket* pPacket = fragList[i];
		if (pPacket != NULL) 
      { // packet exists! Needa be destroyed!
			if(pPacket->lpBuffer != NULL)
            delete []pPacket->lpBuffer;
         pPacket->lpBuffer = NULL;
			delete pPacket;
         pPacket = NULL;
		}
      fragList[i] = NULL;
	}
	
	return completedPacket; // return the built packet ;] everyone's happy
}
//---------- END   OF FragmentsList METHODS ------------------


//---------- START OF FragmentedPackets METHODS ------------------
UDPPacket* FragmentedPackets::RegisterFragment (UDPPacket* pPacket) 
{
   // Search if we have at lesat one piece from this packet..
   list<FragmentsList>::iterator i;
   if ( AlreadyReceivedPiecesFromThisPacket(pPacket, i) == false ) 
   { 
      // This is the first piece we receive from this packet.. lets set it up for receiving others..
      
      fragPackets.push_back( FragmentsList( pPacket->packetHeader->packetID / MAX_FRAGMENTS_PER_PACKET ) );
      fragPackets.back().AddPiece(pPacket);
   } 
   else 
   {
      // We already received other pieces for this packet, lets put them together.
      
      UDPPacket* completedPacket = i->AddPiece(pPacket);
      // Add piece returns the full packet if it got complete, otherwise NULL
      if ( completedPacket != NULL ) 
      {
         //fragPackets.erase(i); // its completed, lets remove from the frag list.
         list<FragmentsList>::iterator del(i);
			fragPackets.erase(del);
         return completedPacket;
      }
      
   }
   return NULL;
}

// Verify if there are fragments that timed out before getting completed.
// If they exists, remove them :)
inline void FragmentedPackets::VerifyTimedoutFragments() 
{
	DWORD currentTime = GetRunTime();
	list<FragmentsList>::iterator i = fragPackets.begin();
	while (i != fragPackets.end()) 
   {
		if (i->timeout <= currentTime) 
      {
			// This fragment expired! Lets kick it out!
			list<FragmentsList>::iterator del(i);
			++i;
			fragPackets.erase(del);
		} 
      else 
      {
			++i;
		}
	}
}

// Returns true if the list of fragmented packets is empty.
inline bool FragmentedPackets::IsEmpty() 
{
	return fragPackets.empty();
}

bool FragmentedPackets::AlreadyReceivedPiecesFromThisPacket (UDPPacket* pPacket, list<FragmentsList>::iterator &i) 
{
	WORD packetID = pPacket->packetHeader->packetID / MAX_FRAGMENTS_PER_PACKET;
	int n=0;
	i = fragPackets.begin();
	while (i != fragPackets.end()) 
   {
		if (i->id == packetID) 
         return true;
		++i;
		++n;
	}
	// This list can't grow bigger than MAX_INCOMPLETE_FRAGMENTED_LIST
	// If it has more elements, remove the first one.
	if (n >= MAX_INCOMPLETE_FRAGMENTED_LIST) 
   {
		fragPackets.pop_front();
	}
	return false;
}
//---------- END   OF FragmentedPackets METHODS ------------------


//---------- START   OF CNMConnection METHODS ------------------
CNMConnection::CNMConnection()
{
	m_CommCenter = NULL;

   m_FragmentedPacketID    = 0;
   m_UnfragmentedPacketID  = 0;
   m_wReceivedPacketOffset = 0;
   for(int i=0;i<MAX_RECEIVED_PACKET_QUEUE;i++)
      m_wReceivedPacketIDs[i] = -1;
   //ZeroMemory(m_wReceivedPacketIDs, sizeof(WORD)*MAX_RECEIVED_PACKET_QUEUE);
   m_dwConnectionTimeout = GetRunTime() + PACKET_BACKLOG_TIMEOUT;
}

CNMConnection::~CNMConnection( void )
{
   DestroyAllPending();
}

int CNMConnection::Initialize(const sockaddr_in &sAddr, CCommCenter *pCommCenter)
{
   m_sockAddr   = sAddr;
	m_CommCenter = pCommCenter;

   return 0;
}


inline WORD CNMConnection::GetFragmentedPacketID() 
{
	// Fragmented packets have IDs < NONFRAGMENTED_PACKETS_IDOFFSET
	m_FragmentedPacketID += MAX_FRAGMENTS_PER_PACKET;
	if (m_FragmentedPacketID >= NONFRAGMENTED_PACKETS_IDOFFSET ) 
   {
		m_FragmentedPacketID = 0;
	}
	return m_FragmentedPacketID;
}

inline WORD CNMConnection::GetUnfragmentedPacketID() 
{
	// Unfragmented packets have ids >= NONFRAGMENTED_PACKETS_IDOFFSET
	++m_UnfragmentedPacketID;
	if ( m_UnfragmentedPacketID < NONFRAGMENTED_PACKETS_IDOFFSET || m_UnfragmentedPacketID >= MAX_PACKETS_IDOFFSET) 
   {
		m_UnfragmentedPacketID = NONFRAGMENTED_PACKETS_IDOFFSET;
	}
	return m_UnfragmentedPacketID;
}

inline bool CNMConnection::AlreadyReceivedPacket(WORD packetID) 
{
	WORD searchEnd = m_wReceivedPacketOffset + 1;
	if (searchEnd >= MAX_RECEIVED_PACKET_QUEUE) 
      searchEnd = 0; // Make sure we dont run out of bounds.
	WORD i = m_wReceivedPacketOffset;
	while (i != searchEnd) 
   {
		if (m_wReceivedPacketIDs[i] == packetID) 
         return true;
		// advance i (wrapping if we hit the end)
		if (i == 0) 
			i = MAX_RECEIVED_PACKET_QUEUE - 1;
      else 
			--i;
	}
	return false;
}
inline void CNMConnection::RegisterReceivedPacketID(WORD packetID) 
{
	// Register the packet id
	++m_wReceivedPacketOffset;
	if (m_wReceivedPacketOffset >= MAX_RECEIVED_PACKET_QUEUE) 
   {
		// We reached the end. Restart.
		m_wReceivedPacketOffset = 0;
	}
	m_wReceivedPacketIDs[m_wReceivedPacketOffset] = packetID;
	// Update the timeout for this connection
	m_dwConnectionTimeout = GetRunTime() + PACKET_BACKLOG_TIMEOUT;
}

inline void CNMConnection::AddPending(UDPPacket* pPacket) 
{
   m_pendingLock.Lock();
	if (m_pendingPackets.size() < MAX_PACKET_PENDING_LIST) 
   {
      //Create pending packet...
      //we resent this packet if need it...
      UDPPacket* pPacketPend  = new UDPPacket;
      pPacketPend->nBufferLen = pPacket->nBufferLen;
      pPacketPend->lpBuffer   = new BYTE[pPacket->nBufferLen];
      memcpy(pPacketPend->lpBuffer,pPacket->lpBuffer,pPacketPend->nBufferLen);
      
      // Initialise Buffer Data
      pPacketPend->packetHeader = (UDPPacketHeader*)pPacketPend->lpBuffer;
      pPacketPend->packetData   = pPacketPend->lpBuffer + HEADER_SIZE;
      pPacketPend->dataLen      = pPacket->dataLen;
      
      pPacketPend->boDelete     = pPacket->boDelete;   
      pPacketPend->nQueueCount  = pPacket->nQueueCount;
      pPacketPend->sockAddr     = pPacket->sockAddr;   
      pPacketPend->dwAckDelay   = pPacket->dwAckDelay; 
      pPacketPend->dwTimeout    = pPacket->dwTimeout;  
      pPacketPend->dwAckCount   = pPacket->dwAckCount; 
      
      pPacketPend->packetHeader->safe     = pPacket->packetHeader->safe;    
      pPacketPend->packetHeader->lastFrag = pPacket->packetHeader->lastFrag;
      pPacketPend->packetHeader->packetID = pPacket->packetHeader->packetID;
      pPacketPend->packetHeader->Reserved = pPacket->packetHeader->Reserved;

	  pPacketPend->HeaderSave=pPacket->HeaderSave;//BLBLBL Correctif de Chaotik pour pouvoir accéder aux infos header après cryptage.

      pPacketPend->dwTimeout = GetRunTime() + pPacketPend->dwAckDelay;
      // If the other packet gets retransmitted, then maybe this is a bad connection. 
      // Wait 1.5x more time to avoid flooding the peer.
      pPacketPend->dwAckDelay = ( pPacketPend->dwAckDelay * 3 ) / 2;
		m_pendingPackets.push_back(pPacketPend);
	} 
   else 
   {
		pPacket->dwAckCount = 0;
	}
   m_pendingLock.Unlock();
}

bool CNMConnection::DestroyPending (WORD packetID) 
{
   m_pendingLock.Lock();
   //OutputDebugString("DestroyPending.. BEGIN\n");//BLBLBL 13/12/2010
   std::list<UDPPacket*>::iterator i = m_pendingPackets.begin();//BLBLBL remplacement du vector par une std::list // std::list au lieu de std:list
	while (i != m_pendingPackets.end()) 
   {
		//OutputDebugString("Looking in the m_pendingPackets list...\n");//BLBLBL 13/12/2010
		//if ((*i)->packetHeader->packetID == packetID)
		if ((*i)->HeaderSave.packetID == packetID)//BLBLBL Correctif de chaotik pour que ce soit pas foireux ^^
      {
		 //BLBLBL à voir si on entre effectivement dans ce bloc un jour, à tester avec un breakpoint.
         UDPPacket* pPacket = (*i);

			if(pPacket->lpBuffer)
            delete []pPacket->lpBuffer;
         pPacket->lpBuffer = NULL;
         delete pPacket;
         pPacket = NULL;

		 //OutputDebugString("Removing packet type from m_pendingPackets\n");//BLBLBL 13/12/2010

			m_pendingPackets.erase(i);
         m_pendingLock.Unlock();
			return true;
		}
		++i;
	}
   m_pendingLock.Unlock();
   //OutputDebugString("Destroy pending .. END\n");//BLBLBL 13/12/2010
	return false;
}

void CNMConnection::DestroyAllPending() 
{
   m_pendingLock.Lock();
   std::list<UDPPacket*>::iterator i = m_pendingPackets.begin();//BLBLBL 06/12/2010 remplace par une list
	while ( i != m_pendingPackets.end() ) 
   {
      UDPPacket* pPacket = (*i);
      
      if(pPacket->lpBuffer)
         delete []pPacket->lpBuffer;
      pPacket->lpBuffer = NULL;
      delete pPacket;
      pPacket = NULL;
		++i;
	}
	m_pendingPackets.clear();
   m_pendingLock.Unlock();
}

inline bool CNMConnection::ConnectionHasTimedout() 
{
	if (m_dwConnectionTimeout <= GetRunTime()) 
   {
		if ( m_pendingPackets.empty() && m_lFragments.IsEmpty()) 
      {
			return true;
		}
	}
	return false;
}

inline void CNMConnection::VerifyTimedoutPending()
{
   if(!m_CommCenter)
      return;
   

   UDPPacket* pResentPacket = NULL;
   DWORD dwCurrentTime = GetRunTime();
   
   m_pendingLock.Lock();
   std::list<UDPPacket*>::iterator i = m_pendingPackets.begin();///BLBLBL remplacé vector par une std::list 06/12/2010
   while ( i != m_pendingPackets.end() ) 
   {
      if ( /*(*i) != NULL && */(*i)->dwTimeout <= dwCurrentTime ) //BLBLBL ajout d'un test null//07/12/2010 je le vire
      {
         pResentPacket = *i;
         if(pResentPacket!=NULL) //BLBLBL ajout d'un test different de null
         {
            // It timed out, lets get it ready for retransmission!
            pResentPacket->dwTimeout = GetRunTime() + pResentPacket->dwAckDelay;//0xFFFFFFFF;//BLBLBL remplacé (*i) par pResentPacket et mis valeur correcte pour le delai
            //pResentPacket->dwTimeout = 0xFFFFFFFF;//BLBLBL pb de timeout je remet FFFFF pour voir
			pResentPacket->dwAckCount--;
            if(pResentPacket->dwAckCount == 0)
            {
               //(*i)->boDelete = TRUE;
               UDPPacket* pPackDel = pResentPacket;
               //vector<UDPPacket*>::iterator iDelete(i);
               //++i;
               //m_pendingPackets.erase(iDelete);

			   i=m_pendingPackets.erase(i);
			   //OutputDebugString("Deleted a pending packet, due to timeout.\n");//BLBLBL 13/12/2010

               if(pPackDel->lpBuffer)
                  delete []pPackDel->lpBuffer;
               pPackDel->lpBuffer = NULL;
               delete pPackDel;
               pPackDel = NULL;
               
               // not destroy packet it will be destroyed on send packet function...
               // because dwAckCount == 0 :P
            }
            else
			{/// 06/12/2010 : BLBLBL ajout de braces il ne faut pas envoyer un pak supprimé
              
            //Send the packet to Sending queue
            m_CommCenter->ReSendPacket(pResentPacket);
			 ++i;
			}
         }
         else
            ++i;
         
      } 
      else 
      {
         ++i;
      }
   }
   m_pendingLock.Unlock();
}
//---------- END   OF CNMConnection METHODS ------------------

//---------------------------- END OF   OTHER UTILITY CLASSES ------------------------------
