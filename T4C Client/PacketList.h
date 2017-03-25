#ifndef __PACKET_LIST_H
#define __PACKET_LIST_H

#include "PowerDebug.h"

struct FullSocketList {
   TFCPacket *Packet;
   DWORD      ID;
   DWORD      PlayerID;
};

class FullSocket {

public:
   TemplateList <FullSocketList> tlItem;

   void AddPacket(DWORD ID, DWORD PlayerID, TFCPacket *Packet) {
      FullSocketList *t = new FullSocketList;
      t->ID = ID; 
      t->Packet = Packet;
      t->PlayerID = PlayerID;
      tlItem.Lock("Packet 1");
	   tlItem.AddToTail(t);
	   tlItem.Unlock("Packet 1");
   }
   void RemovePacket(DWORD ID, DWORD PlayerID) {
     tlItem.Lock("Packet 2");
	  tlItem.ToHead();
      while (tlItem.QueryNext()) {
         FullSocketList *f;
         f = tlItem.GetObject();
         if (f->ID == ID && f->PlayerID == PlayerID) {
            LOG(0) << "Removing code [" << ID << "]\r\n";
            delete f->Packet;
            delete f;
            tlItem.RemoveObject();
         }
      }
	  tlItem.Unlock("Packet 2");
   }
};

extern FullSocket ug_PacketList;


#endif