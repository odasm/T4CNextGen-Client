#ifndef NM_PACKET_HEADER_H
#define NM_PACKET_HEADER_H


//#define MAX_PACKET_SIZE     1024//BLBLBL 07/12/2010 Je remet à 1024
#define MAX_PACKET_SIZE     512 //BLBL en UDP, les paquets plus gros que 512 ont parfois du mal à être routés correctement.
#define HEADER_SIZE         sizeof( UDPPacketHeader )
#define CHKSUM_SIZE         1
#define MAX_DATA_SIZE       ( MAX_PACKET_SIZE - HEADER_SIZE -CHKSUM_SIZE) //1 is checksum...


#define MAX_PACKETS_IDOFFSET            0x1FFF // ID maximum  //0x3FFF
#define NONFRAGMENTED_PACKETS_IDOFFSET  5760   // Higher ids are non-fragmented //14336
#define MAX_FRAGMENTS_PER_PACKET          32   // Maximum number of fragments a packet can have.  IMP : Must be a power of 2.
#define MAX_PACKET_PENDING_LIST            5
#define MAX_INCOMPLETE_FRAGMENTED_LIST    10   // how many packts can sit there waiting for completion?
#define MAX_RECEIVED_PACKET_QUEUE        128   // Must be power of 2 to avoid border effect

// Maximum time to wait for fragmented packet to be restored.
#define FRAGMENTATION_MAX_WAIT      10000
#define PACKET_BACKLOG_TIMEOUT      15000


#define ENABLE_CONNECTION_LOST_LISTING

#define USE_CLIENT_CONNECTION

#ifdef USE_CLIENT_CONNECTION
   #define GetRunTime() GetTickCount()
#endif

typedef struct _UDPPacketHeader
{
    // Minimize header size by regrouping data into WORD.
    WORD
      safe : 1,          // needs ack?
      lastFrag : 1,      // is the last fragment?
      Reserved : 1,      // ??????
      packetID   : 13;   // Id :)
} UDPPacketHeader;
 
typedef struct _UDPPacket
{
   // Data members
   sockaddr_in          sockAddr;
   DWORD                dwAckDelay;
   DWORD                dwTimeout;
   DWORD                dwAckCount;
   BOOL                 boDelete; 
   int                  nQueueCount;   // Quantity of packets currently queued.
   DWORD                dwPacketTime;  // Time at which the packet arrived.
   LPBYTE               lpBuffer;      // The buffer
   int                  nBufferLen;    // size of the full buffer (header + data)
   int					   dataLen;       // size of the data part of the buffer
   BYTE				      *packetData;   // Pointer to the data part of the buffer (for ease of use)
   UDPPacketHeader		*packetHeader; // Pointer to the header part of the buffer (for ease of use)
   UDPPacketHeader HeaderSave;//BLBLBL Ajout de Chaotik pour mémoriser l'état, et tenter de corriger le bug des ack cryptés
   //BOOL					needAck; // BLBLBL simplification de la solution pour voir si ça evite les crashs

} UDPPacket;

 



// This abstract base class is used as a template for adding packet internal implementation
// specific performance data.
class CPacketMonitor
{
public:
    // Called each time the packet is being sent through the network
    virtual void PacketSent( void )   = 0;
    // Called each time the packet is resent (ack timed out).
    virtual void PacketResent( void ) = 0;
    // Called when the CommCenter no longer needs the packet.
    virtual void Delete( void )       = 0;
};

// This abstract base class is used as a template for adding performance monitoring to
// the CommCenter interface.
class CCommMonitor
{
public:
    enum QueueAction
    {
        Queued = 1,
        Unqueued = 2
    };
    
    // Called each time a packet is sent through the network
    virtual void PacketSent( DWORD dwPacketSize ) = 0;
    // Called each time a packet is being received.
    virtual void PacketReceived( DWORD dwPacketSize ) = 0;
    // Called each time a packet ack was received.
    virtual void AckReceived( void ) = 0;
    // Called each time a packet is being resent through the resend mechanism.
    virtual void AckRetransmit( void ) = 0;
    // Called each time the sending packet queue changes size.
    virtual void SendQueueUpdate( DWORD dwSize, QueueAction qAction ) = 0;
    // Called each time the interpretation queue changes size.
    virtual void IntrQueueUpdate( DWORD dwSize, QueueAction qAction ) = 0;
    // Called each time the receiving queue changes size.
    virtual void RecvQueueUpdate( DWORD dwSize, QueueAction qAction ) = 0;    
    // Called earch time the PreSendQueue changes size.
    virtual void PreSendQueueUpdate( DWORD dwSize, QueueAction qAction ) = 0;
};

class CCommMonitorEmpty : public CCommMonitor 
{
    // Called each time a packet is sent through the network
    inline virtual void PacketSent( DWORD dwPacketSize ) {};
    // Called each time a packet is being received.
    inline virtual void PacketReceived( DWORD dwPacketSize ) {};
    // Called each time a packet ack was received.
    inline virtual void AckReceived( void ) {};
    // Called each time a packet is being resent through the resend mechanism.
    inline virtual void AckRetransmit( void ) {};
    // Called each time the sending packet queue changes size.
    inline virtual void SendQueueUpdate( DWORD dwSize, QueueAction qAction ) {};
    // Called each time the interpretation queue changes size.
    inline virtual void IntrQueueUpdate( DWORD dwSize, QueueAction qAction ) {};
    // Called each time the receiving queue changes size.
    inline virtual void RecvQueueUpdate( DWORD dwSize, QueueAction qAction ) {};    
    // Called earch time the PreSendQueue changes size.
    inline virtual void PreSendQueueUpdate( DWORD dwSize, QueueAction qAction ) {};
};

#endif //NM_PACKET_HEADER_H

