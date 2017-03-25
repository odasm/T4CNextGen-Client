// **************************************************************************
// ***                                                                    ***
//      File Name: TFCSocket.h
//      Project:   The Fourth Coming
//      Creation:  May 30th, 1997
//      Author:    Benoit Thomas (TH)
// ***                                                                    ***
// **************************************************************************
// ***                                                                    ***
//      Change History
//
//         Date            Ver.	  Author	     Purpose
//         ----            ----    ------      -------
//         06-27-1997      1.0     TH          Initial development.
//
//		  Fonctions:
//			  Provide Socket Receive Function
//
// ***                                                                    ***
//	**************************************************************************
// *** Copyright (c) 1996-1997 Vritual Dreams (r). All rights reserved.   ***

#ifndef __Virtual_Dreams_TFCSocket_123524062797
#define __Virtual_Dreams_TFCSocket_123524062797

#include "DirectSocket.h"    

static const int MAXPacketLenght = 2500;

static const int TFCMoveID                              = 1;

static const int TFCQueryPlayerPosition                 = 9;

static const int TFCStillConnected							 = 10;

static const int TFCDeleteObject                        = 11;

static const int TFCChangeObject                        = 12;

static const int TFCInGame                              = 13;
// Use Same Error Code as TFCCreateNewPlayer

static const int TFCCreateNewPlayer                     = 25;
static const int TFCCreateNewPlayer_CreatedPlayer       = 0;
static const int TFCCreateNewPlayer_AccountInGame       = 1;
static const int TFCCreateNewPlayer_NoCredits           = 3;
static const int TFCCreateNewPlayer_TooManyAccounts     = 4;
static const int TFCCreateNewPlayer_PlayerAlreadyExists = 5;
static const int TFCCreateNewPlayer_PlayerDontExists    = 6;
static const int TFCCreateNewPlayer_PlayerAlreadyGame   = 7;
static const int TFCCreateNewPlayer_InvalidNameSpec     = 8;

static const int TFCRegisterAccount                     = 14;
static const int TFCRegisterAccount_Registred           = 0;
static const int TFCRegisterAccount_WrongPassword       = 1;
static const int TFCRegisterAccount_NoCredits           = 2;
static const int TFCRegisterAccount_DoNotExists         = 3;
static const int TFCRegisterAccount_AlreadyRegistred    = 4;
static const int TFCRegisterAccount_WrongVersion			 = 6;

static const int TFCAddObject                           = 16;

static const int TFCGetPlayerList                       = 26;

static const int TFCReroll                              = 31;

#define NM_CONNECT_SCREEN_NBR_BACK_ANIM             7             

class TFCSocket {
   private:
      CRITICAL_SECTION csTFCSocket;

   public:
		TFCSocket();
		virtual ~TFCSocket();
              
		void MainThread(void);
		void MenuThread(void);
};

#endif

extern sockaddr_in ServerIPaddr;
