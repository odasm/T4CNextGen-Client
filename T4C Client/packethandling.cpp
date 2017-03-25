// ************************************************************************
// ***                                                                  ***
//      File Name: PacketHandling.cpp
//      Project:   The 4th Coming
//      Creation:  5 May, 1999
//      Author:    Benoit Thomas (TH)
//
//      THESE ARE SENT PACKETS TO SERVER.
//		for paquets received by server, go to Packet.cpp
//
// ***                                                                  ***
// ************************************************************************
// ***                                                                  ***

#pragma warning( disable : 4291 )
#pragma warning( disable : 4786 )

// Include
#include "pch.h"
#include <windows.h>
#include "App.h"
#include "PacketHandling.h"
#include "PowerDebug.h"
#include "VisualObjectList.h"
#include "ObjectListing.h"
#include "TFCPlayer.h"
#include "LocalString.h"
#include "Packet.h"
#include "pf.h"
#include "Fading.h"
#include "TileSet.h"
#include "COMM.h"
#include "NTime.h"
#include "Global.h"
#include "MemoryManager.h"
#include "Filelogger.h"
#include "SaveGame.h"
extern CSaveGame g_SaveGame;

#include "NewInterface/ChatterUI.h"

// Global var.
extern BOOL DoNotMove;
extern int DrawLastMoving;
extern bool Draw;
extern BOOL INGAME;
extern HANDLE hDrawThread;
extern UINT iDrawThreadID;
extern bool stShow;
extern BOOL FirstTime;
extern UINT CALLBACK DrawThread(LPVOID pParam);
extern bool EnterGame;
extern bool seraphDisplay;
extern bool goodSeraph;
extern T3VSBSound SoundFX[250];

void CloseAllUI(void);


//using namespace vir;//DC a mettre en commentaire apres debeug

// Define
#define LogStPacket(PacketNumber) \
    int i483PacketNumber = ##PacketNumber; \
LOG << "* PAK = " << i483PacketNumber << " {\r\n"; 

#define LogEndPacket() \
LOG << "* PAK = " << i483PacketNumber << " }\r\n";

void PacketHandling::SetUnitStat(TFCPacket *Msg) {
    // Set unit statistic, permet de faire des changements de Light, Status et Hp.
    // Log packet type of 69.
    LogStPacket(69);
    
    short TYPE;
    DWORD ID;
    char LIGHT;
    char STATUS;
    char PHP;
    
    Msg->Get((short *)&TYPE);
    Msg->Get((long  *)&ID);
    Msg->Get((char *)&LIGHT);
    Msg->Get((char *)&STATUS);
    Msg->Get((char *)&PHP);
    
    LOG << "* PAK = 69, " << ID << ", " << LIGHT << ", " << PHP << "\r\n";
    
    Objects.Lock(3);
    Objects.SetUnit(ID, TYPE, LIGHT, STATUS, PHP);
    Objects.Unlock(3);
    
    LogEndPacket();
}

void PacketHandling::IndirectTalk(TFCPacket *Msg) 
{
    // IndirectTalk (evey talking in the game)
    // Log packet type of 27.
    LogStPacket(27);
    
    unsigned long ID;
    short Len;
    unsigned long Color;
    unsigned char Direction = 2;
    char *Temp;
    char isNpc = 0;
    
    Msg->Get((long *)&ID);           // Get talking ID.
    Msg->Get((char *)&Direction);	   // Get talking Direction.
    Msg->Get((long *)&Color);        // Get Text Color.
    Msg->Get((char *)&isNpc );
    
    LOG.Lock();
    LOG << "\r\nisNPC=" << isNpc << ".";
    LOG.Unlock();
    
    // If Dragon then Direction = South!
    Objects.Lock(4);
    if (ID == Objects.FoundType(__MONSTER_DRAGON))
        Direction = 2;
    Objects.Unlock(4);
    
    // Set static color.
    Color=RGB(0,190,190);
    
    Msg->Get((short *)&Len);   // Get text Len
    
    // Allocate Temp var.
    Temp = new char [Len+1];   
    
    // Get Text.
    for (int j = 0; j < Len; j++) {
        Msg->Get((char *)&Temp[j]);
    }
    Temp[Len] = 0;
    
    LOG << "* PAK = 27, ID(" << ID << ") Text Color (" << Color << ") Direction (" << Direction << ") Text Len (" << Len << ") Text (" << Temp << ")";
    
    short unitNameLen = 0;
    Msg->Get((short*)&unitNameLen);
    
    char *unitName = new char [unitNameLen+1];
    
    for (int j = 0; j < unitNameLen; j++) {
        Msg->Get((char *)&unitName[j]);
    }
    unitName[unitNameLen] = 0;

    DWORD color = RGB(255,0,0);
    Msg->Get((DWORD *)&color);
    
    if (Len && unitNameLen) 
    {
        // Have we receive any text ?
        if (ID == Player.ID) 
        {
            // Is the player talking ?
            
            Objects.Lock(5);
            
            

            Objects.GetMainObject()->SetTalkText( Temp, Custom.gTextColor );

           
            Objects.GetMainObject()->Temp = timeGetTime();
            if (Direction){
                Objects.GetMainObject()->Direction = Direction;
            }         
            
            Objects.Unlock(5);
            
            
            if( g_SaveGame.GetOptionsParam()->bLogUser )
            {
                // Log to backscroll
                string str( g_LocalString[ 529 ] );
                str += Temp;
                
                ChatterUI::GetInstance()->AddBackscrollItem(
                    "",
                    str.c_str(),
                    SYSTEM_COLOR,
                    false,
                    false
                    );
            }
            
        } 
        else 
        {
            // Is another character talking
            // Put text on top of unit.
            Objects.Lock(6);
            Objects.SetName(ID, unitName, FALSE, color);
            Objects.Text(ID, Temp, Color, Direction);
            Objects.Unlock(6);
            
            if( g_SaveGame.GetOptionsParam()->blogNPC || !isNpc ){         
                char *backscrollText = new char[ strlen( unitName ) + strlen( Temp ) + 10 ];
                strcpy( backscrollText, "{" );
                strcat( backscrollText, unitName );
                strcat( backscrollText, "}" );
                strcat( backscrollText, "\":\" " );
                strcat( backscrollText, Temp );
                
                ChatterUI::GetInstance()->AddBackscrollItem( unitName, backscrollText, Color, false, false );
                
                delete backscrollText;
            }
            
            
        }
    }
    delete Temp;
    delete unitName;
    
    LogEndPacket();
}

void PacketHandling::TeleportPlayer(TFCPacket *Msg) {
    // Teleport
    // Log Packet Type 57
    LogStPacket(57);
				
    // Stop the game music (and reset it).
    if (g_Var.inGame) {
        DoNotMove = TRUE;
        CloseAllUI();
        Sleep(150);
        g_GameMusic.Reset();
        World.Lock(12);
        
        signed short WORLD, X, Y;
        Msg->Get((short *)&X);
        Msg->Get((short *)&Y);
        Msg->Get((short *)&WORLD);
        
        if (X < 0 || X > 3072 || Y < 0 || Y > 3072 || WORLD < 0 || WORLD > (World.NumberofWorld-1)) {
            ChatterUI::GetInstance()->AddBackscrollItem(
                "",
                g_LocalString[98],
                RGB(255,255,255),
                true
                );
            Sleep(2500);
            LOG << "* CLS = 7\r\n";
            PostMessage(siGethWnd(), WM_CLOSE, NULL, 0x6969);
            return;
        }
        
        pfStopMovement("Move 17");
        Player.xPos = X;
        Player.yPos = Y;
        Player.World = WORLD;
        
        Objects.Lock(7);
        Objects.DeleteAll();
        //Objects.ReleaseAll();
        Objects.Unlock(7);
        World.ChangeStatic();
        signed char lMovX = X - Player.xPos;
        signed char lMovY = Y - Player.yPos;
        
        DrawLastMoving = 0;
        World.Done = 0;
        Draw = true;
        
        Player.xPos = X;
        Player.yPos = Y;
        Player.World = WORLD;
   
        // on commence par loader la map du new world...
        g_Global.LoadZoneMapWorld(Player.World,Player.xPos,Player.yPos);
        g_Global.ForceDisplayZone(Player.xPos,Player.yPos);
        //g_Global.ValidMapZonePosition(Player.xPos,Player.yPos);
        
        Objects.Lock(1015);
        Objects.AddSDObject();
        Objects.AddSDAnimObject();
        World.SetPosition(Player.xPos, Player.yPos, Player.World);
        Objects.MovePlayer(lMovX, lMovY);
        Objects.SetSet(TRUE);
        Objects.Unlock(1015);
        World.SetFading = true;
        World.FadeLevel = 0;
        World.Switch();
        
        g_GameMusic.LoadNewSound();
        
		{
			TFCPacket Send;        
			Send << (short)60;//GetNearUnits
			SEND_PACKET(Send);
			EnterGame = true;
		}
		{
			// BLBLBL // Destiny: Ensures NO black screen
			TFCPacket sending;
			sending << (short)46; // RQ_FromPreInGameToInGame
			SEND_PACKET( sending );
		}

        World.Unlock(12);
    }
    
    LogEndPacket();
}
void PacketHandling::FromPreInGameToInGame(TFCPacket *Msg) {
    // Send to Server when loading is finsih... receive from ser er for a confirmation.
    try {
        LogStPacket(46); 

        // If the player is a seraph, play the seraph sound
        DoNotMove = FALSE;
        g_Var.inGame = true;
        EnterGame = false;

        if(g_Var.InvalideSaveGame)
        {
           g_Var.InvalideSaveGame = false;
           ChatterUI::GetInstance()->AddBackscrollItem("", "Invalid Savegame...", RGB(255, 0, 0), true);
           ChatterUI::GetInstance()->AddBackscrollItem("", "Default will be used.", RGB(255, 0, 0), true);
           
        }

        Objects.Lock(1017);
        Objects.lmPlayerLight     .Release();
        Objects.lmPlayerLight     .Create("TorchPlayer.rgb", g_Global.GetMainTorchW(), g_Global.GetMainTorchH());
        Objects.lmOtherPlayerLight.Release();
        Objects.lmOtherPlayerLight.Create("TorchOtherPlayer.rgb", 160, 120);
        Objects.lmTorchLight      .Release();
        Objects.lmTorchLight      .Create("Torch.rgb", 160, 120);
        Objects.lmTorchLanterne1  .Release();
        Objects.lmTorchLanterne1  .Create("TorchLanterne1.rgb", 160, 130);
        Objects.lmTorchPoutre     .Release();
        Objects.lmTorchPoutre     .Create("TorchPoutre.rgb", 80, 90);
        Objects.lmTorchChandelle2 .Release();
        Objects.lmTorchChandelle2 .Create("TorchChandelle2.rgb", 80, 90);
        Objects.Unlock(1017);

        Objects.Lock(30);
        Objects.UpdateLight(TRUE);
        Objects.Unlock(30);
        /*
        
        signed char LIGHT;
        unsigned short X, Y, TYPE, nbObject;
        unsigned long ID;
        unsigned char STATUS;
        char PHP;
        Msg->Get((char *)&X); // Dummy Junk Discard
        Msg->Get((short *)&X); // Dummy Junk Discard
        Msg->Get((short *)&nbObject);
        
        LOG << "* PAK = 46, " << nbObject << "\r\n";
        for (unsigned int i = 0; i < nbObject; i++) {
            Msg->Get((short *)&X);
            Msg->Get((short *)&Y);
            Msg->Get((short *)&TYPE);
            Msg->Get((long  *)&ID);
            Msg->Get((char *)&LIGHT);
            Msg->Get((char *)&STATUS);
            Msg->Get((char *)&PHP);
            LOG << "* PAK = 46, " << X << ", " << Y << ", " << TYPE << ", " << ID << ", " << LIGHT << ", " << STATUS << "\r\n";
            if (TYPE == 10002)
                TYPE = 20039;
            if (TYPE == 15002)
                TYPE = 25039;
            if (TYPE == 10004)
                TYPE = 20042;
            if (TYPE == 15004)
                TYPE = 25042;
            if (TYPE == 10001)
                TYPE = 20043;
            if (TYPE == 15001)
                TYPE = 25043;
            if (TYPE == 10003)
                TYPE = 20044;
            if (TYPE == 15003)
                TYPE = 25044;
            
            // Addition.
            if (ID != Player.ID) {
                World.Lock(21);
                Objects.Lock(1016);
                Objects.Add(ID, TYPE, X, Y, LIGHT, STATUS, NULL, PHP);
                Objects.Unlock(1016);
                World.Unlock(21);
            }
            
        }*/
        
        
        LOG << "* PAK = 46 }\r\n";
    } catch (TFCPacketException *e) {
        // Imcomplete or corrupted packet, we don't care, continue....
        e; // disable a warning.
    }
}