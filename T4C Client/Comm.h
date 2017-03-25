#ifndef __COMM_H
#define __COMM_H

#include "CommCenter.h"
#include "TFCPacket.h"
#include "PacketTypes.h"
#include "TemplateQueues.h"

class PacketCenter  {
   public:
      PacketCenter(void);
      ~PacketCenter(void);
      void SendPacket(TFCPacket *);
      void Close(void);
      void SetIPAddr(char *);
      char *GetIPAddr();
      void SetAddrPort(short);
      int  Receive(TFCPacket * &);
      int State;
      void AddPacket(TFCPacket *);
      BOOL Create(COMM_INTR_CALLBACK);
      CCommCenter *Ctr;
		void KeepAlive(void);
		BOOL isAlive(void);
      void SetAlive(DWORD);
		BOOL isHalf(void);
		void LongLive(void);
		void Lock(void);
		void Unlock(void);
      sockaddr_in sockAddr;
		DWORD dwLastPacketTime;

      CCommCenter *GetCommCenter();

   private:
      char *AddrIP;
      short AddrPort;
      TemplateQueue <TFCPacket> tqQueue;
		DWORD dwTime;
		CRITICAL_SECTION csInternal;
};

extern PacketCenter COMM;

#define GET_WORD( var )	msg->Get( (short *)&var );
#define GET_LONG( var ) msg->Get( (long *)&var );
#define GET_BYTE( var ) msg->Get( (char *)&var );
#define GET_STRING( var )	{int i; WORD len;\
								msg->Get( (short *)&len );\
								var = new BYTE[ len + 1 ];\
								for( i = 0; i < len; i++ ){\
									msg->Get( (char *)&var[ i ] );\
								} var[ i ] = 0;\
							}

#define SEND_PACKET(packet) { \
	LOG << "* OUT = " << packet.GetPacketID() << "\r\n"; \
   COMM.SendPacket(&packet) ; \
}

#endif