// *******************************************************************************************
// ***                                                                                     ***
//      File Name: CommCenter.h
//      Project:   TFC Server
//      Plateform: Windows NT 4.0 Workstation/Server
//      Creation:  8/9/1998
//      Author:    Francois Leblanc (FL)
// ***                                                                                     ***
// *******************************************************************************************
// ***                                                                                     ***
//      Change History
//
//         Date            Ver.      Author         Purpose
//         ----            ----      ------         -------
//         8/9/1998      1.0        FL             Initial developpement
//
//      Description
//          This is a packet interface for the socket. Works on UDP.
//          
// ***                                                                                     ***
//	*******************************************************************************************
// ***           Copyright (c) 1997-1998 Virtual Dreams. All rights reserved.              ***


#if !defined(AFX_COMMCENTER_H__3742E71A_4727_11D2_83EF_00E02922FA40__INCLUDED_)
#define AFX_COMMCENTER_H__3742E71A_4727_11D2_83EF_00E02922FA40__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "ComPacketHeader.h"
#pragma warning( disable:4786 )
#ifndef USE_CLIENT_CONNECTION
   #include <afxsock.h>
#else
   #include <windows.h>
   #include <queue>
   #include "UDP/Queue.h"
#endif
   
#include <vector>
#include <list>
#include <map>
#include "Lock.h"


using namespace std;

#define COMM_INTR_PROTOTYPE sockaddr_in sockAddr, LPBYTE lpbBuffer, int nBufferSize

typedef void( *COMM_INTR_CALLBACK )( COMM_INTR_PROTOTYPE );
typedef bool (*MSCOMFUNC)( BYTE bCode, LPBYTE &lpSendBuffer, int &nBufferSize );

class CNMConnection;
class CCommCenter  
{
public:
   // Construction
   CCommCenter( CCommMonitor *lpCommMonitor = NULL );
   ~CCommCenter( void );
   
   BOOL Create( COMM_INTR_CALLBACK lpRead, WORD wRecvPort=0, WORD wSendPort=0, LPCSTR lpszRecvIP=0, LPCSTR lpszSendIP=0 );
   void Destroy( void );
   
   // Allows to send a packet.
   void SendPacket( sockaddr_in sockAddr, LPBYTE lpBuffer, int nBufferSize, DWORD dwAckDelay, DWORD dwMaxAck);
   void ReSendPacket(UDPPacket* pPacket);
   void SendAck(UDPPacket* pPacket) ;
   #ifndef USE_CLIENT_CONNECTION
      void SendAckOK(UDPPacket* pPacket,USHORT ushValue);
   #endif
   
   /*
   // Allows boosting certain components of the CommCenter
   void BoostListeningThread( int nPriority );
   void BoostSendingThread( int nPriority );
   void BoostInterpretingThread( int nPriority );
   void BoostCommThread( int nPriority );
   
   int GetListeningThreadPriority( void );
   int GetSendingThreadPriority( void );
   int GetInterpretingThreadPriority( void );
   int GetCommThreadPriority( void );
   */
   
#ifdef ENABLE_CONNECTION_LOST_LISTING   
   // Returns the lost connection vector. The use *MUST* free the lost connection list using the FreeLostConnections
   vector< sockaddr_in > *GetLostConnections( void );
   void FreeLostConnections( BOOL boFlushList = TRUE );
#endif

   int GetIntrQueueSize();
   
private:
   // Utility functions
   inline CNMConnection* GetConnection( sockaddr_in sockAddr );
   bool InitSocket( WORD wPort, LPCSTR lpszIP, SOCKET &sSocket, sockaddr_in &sockAddr );

   void AnalyzeUPPData(UDPPacket* pPacket);

   inline void PostReceivePacket     (UDPPacket* pPacket); // 
   inline void PostSendPacket        (UDPPacket* pPacket); // 
   inline void PostPreSendData       (UDPPacket* pPacket); // 
   inline void PostAnalysePacket     (UDPPacket* pPacket); // 
   
   static void UDPReceiveDataThread  ( LPVOID lpParam );
   static void UDPSendDataThread     ( LPVOID lpParam );
   static void UDPReceivePacketThread( LPVOID lpParam );
   static void UDPSendPacketThread   ( LPVOID lpParam );
   static void UDPMaintenancePacket  ( LPVOID lpParam );
   static void UDPAnalyseThread      ( LPVOID lpParam ); 
   #ifdef USE_CLIENT_CONNECTION
      static void UDPCheckServer        ( LPVOID lpParam ); 
      static void UDPStopClient         ( LPVOID lpParam ); 
   #endif

   unsigned short DoCrc16(unsigned short shVal,unsigned short ushCurrentCrc);

private:
   // Interpret thread.    
   COMM_INTR_CALLBACK      lpReadCallback; 
   // Monitoring instance.
   CCommMonitor           *lpMonitor;
   // Socket.
   SOCKET                  sSendSocket;
   SOCKET                  sRecvSocket;
   sockaddr_in             sockRecvAddr;
   sockaddr_in             sockSendAddr;

   // Packet integrety and fragmentation queues.
   typedef map< sockaddr_in, CNMConnection* > ConnectionMap;
   ConnectionMap           m_Connections;
   CLock                   cConnectionLock;
   CLock                   cMaintenanceLock;
   CLock                   m_SendLock;
   CLock                   m_ReSendLock;
   CLock                   m_SendAckLock;
   CLock                   m_SendAckOKLock;

   //Bool for running thread
   BOOL      bUDPReceiveDataThread;    // for Thread who receive UDP data
   BOOL      bUDPSendDataThread;       // for Thread who write   UDP Data
   BOOL      bUDPReceivePacketThread;  // for Thread who receive UDP Packet
   BOOL      bUDPSendPacketThread;     // for Thread who Send    UDP Packet
   BOOL      bUDPMaintenancePacket;    // for Thread who maintenance comm Packet
   BOOL      bUDPAnalyseThread;

   // Thread Handle
   HANDLE    hhUDPReceiveDataThread;
   HANDLE    hhUDPSendDataThread;
   HANDLE    hhUDPReceivePacketThread;
   HANDLE    hhUDPSendPacketThread;
   HANDLE    hhUDPMaintenancePacket;
   HANDLE    hhUDPAnalyseThread;
   #ifdef USE_CLIENT_CONNECTION
      //BOOL      bUDPCheckServer;
      //HANDLE    hhUDPCheckServer;
      //HANDLE    hhUDPStopClient;

      USHORT    m_ushCode[3];
      BOOL      m_bSendCRC;
   #endif


   #ifdef USE_CLIENT_CONNECTION
      WaitQueue<UDPPacket> m_qUDPReceivePacketIO;
      WaitQueue<UDPPacket> m_qUDPSendPacketIO;
      WaitQueue<UDPPacket> m_qUDPPreSendPacketIO;
      WaitQueue<UDPPacket> m_qUDPAnalyseIO;
      WaitQueue<UDPPacket> m_qUDPCheckServer;
   #endif

   // COmpletion Queue IO
   HANDLE    hhUDPReceivePacketIO;  // Queue with all received packet
   HANDLE    hhUDPSendPacketIO;     // Queue for sending packet to UDP layer
   HANDLE    hhUDPPreSendPacketIO;  // Queue for analysing and send to SEND packet IO 
   HANDLE    hhUDPAnalyseIO;        // Queue for analysing packet and give callback to upper layer...


   int       m_uiNbrInterpQueueCnt;
   CLock     m_InterpLock;

   #ifdef USE_CLIENT_CONNECTION
      BOOL      m_bIsWindowNT;
   #endif

   
   

   #ifdef ENABLE_CONNECTION_LOST_LISTING
      CLock cConnectionLostLock;
      vector< sockaddr_in > vConnectionLost;
   #endif
};





class FragmentedPackets;


//
// This is the class responsible for holding the pieces of a fragmented packet
// while its being received.
//
class FragmentsList 
{
   friend FragmentedPackets;
public:
   // Constructor
   FragmentsList(WORD newID);
   // Destructor
   ~FragmentsList();
   
private:
   WORD id; // ID of the fragment
   DWORD timeout; // Time when this packet expires and gets removed from queue.
   WORD nOfPieces; // Number of pieces that make this packet
   WORD receivedPieces; // how many pieces we already received.
   vector<UDPPacket*> fragList; // vector with the pieces.
   WORD sizeOfTheLastPiece;
   
   UDPPacket* AddPiece (UDPPacket* pPacket); // register a new received piece.
   UDPPacket* BuildCompletedPacket();
};


//
// This class is responsible for holding the fragmented packets for the connection that holds the instance
// (each fragmented packet is responsible for holding its pieces, which is handled by FragmentsList class)
//
class FragmentedPackets 
{
public:
   UDPPacket* RegisterFragment (UDPPacket* pPacket);
   
   // Verify if there are fragments that timed out before getting completed.
   // If they exists, remove them :)
   void VerifyTimedoutFragments();
   
   // Returns true if the list of fragmented packets is empty.
   bool IsEmpty();
private:
   bool AlreadyReceivedPiecesFromThisPacket (UDPPacket* pPacket, list<FragmentsList>::iterator &i);
   
   list<FragmentsList> fragPackets; // List of fragmented packets (with its pieces)
};



class CNMConnection
{
public:
   
   CNMConnection();
   ~CNMConnection();
   
   int Initialize(const sockaddr_in &sAddr, CCommCenter *pCommCenter);
   
   inline WORD GetFragmentedPacketID();
   inline WORD GetUnfragmentedPacketID();
   
   inline bool AlreadyReceivedPacket(WORD packetID);
   inline void RegisterReceivedPacketID(WORD packetID);
   
   inline void AddPending(UDPPacket* pPacket); // Add this packet to the list of 'waiting for ack' packets
   inline void VerifyTimedoutPending();
   
   inline bool ConnectionHasTimedout() ;
   
   bool DestroyPending (WORD packetID);
   void DestroyAllPending();
   
public:
   
   CCommCenter	*m_CommCenter;  // The CCommCenter instance that this connection is attached to
   sockaddr_in     m_sockAddr;    // The socket for that connection 
   
   FragmentedPackets				m_lFragments;
   
   WORD  m_FragmentedPacketID:14;    // IDs used for sending fragmented packets 
   WORD  m_UnfragmentedPacketID:14;  // IDs used for sending unfragmented packets 
   
   //vector<UDPPacket*> m_pendingPackets; // List of packets that did not receive an ACK yet.
   std::list<UDPPacket*> m_pendingPackets; ///BLBLBL 06/12/2010 remplacement par une liste (plus rapide)
protected:
   
   
protected:
   CLock  m_pendingLock;
   DWORD	 m_dwConnectionTimeout;
   
   WORD	 m_wReceivedPacketOffset;
   long	 m_wReceivedPacketIDs[MAX_RECEIVED_PACKET_QUEUE];
   
};

#endif // !defined(AFX_COMMCENTER_H__3742E71A_4727_11D2_83EF_00E02922FA40__INCLUDED_)
 