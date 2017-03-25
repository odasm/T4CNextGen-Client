// TFCPacket.cpp: implementation of the TFCPacket class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning( disable : 4291 )

//#include <stdio.h>

//#include "stdafx.h"
#include "pch.h"
#include <windows.h>
#include "TFCPacket.h"

/* Old-crypt stuff. Turned off.
#ifdef _AFXDLL
#include "EncRandom.h"
#else
#include "Random.h"
#endif
*/

#include "PowerDebug.h"

#include <iterator>
#include <algorithm>
#include <functional>
#include "Global.h"

#include "MemoryManager.h"

#ifdef  TRACE
#undef  TRACE
#endif
#define TRACE 1 ? (void) 0 : printf

#define GROW_BY		5

#ifdef HEADER_SIZE
#undef HEADER_SIZE
#endif
#define HEADER_SIZE     ( KEY_SIZE + CHECKSUM_SIZE )


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TFCPacketException::TFCPacketException(UINT cause){
	m_cause = cause;
}

// Construction //////////////////////////////////////////////////////////////////////////
TFCPacket::TFCPacket( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
	// At least create header in vector.
    BYTE bHeader[ HEADER_SIZE ] = { 0, 0, 0, 0 };
    
    copy( bHeader, bHeader + HEADER_SIZE, inserter( vBuffer, vBuffer.begin() ) );
        
    nPos = 0;
	packetSeedID = 0;
}

// Destructor ////////////////////////////////////////////////////////////////////////////
 TFCPacket::~TFCPacket( void )
//////////////////////////////////////////////////////////////////////////////////////////
{
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCPacket::Destroy( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Destroys a packet. Allows reusing a packet object.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{		        
    BYTE bHeader[ HEADER_SIZE ] = { 0, 0, 0, 0 };

    // Destroy the vector, but not its header.
    vBuffer.erase( vBuffer.begin() + HEADER_SIZE, vBuffer.end() );
   
	nPos = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
void TFCPacket::Seek
//////////////////////////////////////////////////////////////////////////////////////////
// Seeks the position of in the packet.
// 
(
 signed long where, // Where to seek.
 char how           // From how to seek.  
)
//////////////////////////////////////////////////////////////////////////////////////////
{
	switch(how)
	{
		case 0: nPos = where; break;
		case 1: nPos += where; break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCPacket::EncryptPacket( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Encrypts the current packet to send.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{	
/* FUNCTION OFF. No longer using this encryption method.
   #ifdef _AFXDLL    
       EncRandom rndKey( 0, sizeof( KEY ) * 256 );
   #else
       Random rndKey( 0, sizeof( KEY ) * 256 );
   #endif    
       // Fetch key value
       KEY kKey = rndKey;
    
       // Set new seed according to key.
   #ifdef _AFXDLL
       EncRandom rndEncrypt( 0, 255, kKey );
   #else
       Random rndEncrypt( 0, 255, kKey, packetSeedID );
   #endif
    
       // Copy key into buffer.
       int i;
       for( i = 0; i < KEY_SIZE; i++ )
       {
           vBuffer[ i ] = *((LPBYTE)( &kKey ) + i );
       }
    
       // Calculate checksum.
       CHECKSUM kCheckSum = 0;
       for( i = 0; i < vBuffer.size() - HEADER_SIZE; i++ )
       {
           kCheckSum += vBuffer[ i + HEADER_SIZE ];
       }

       // Copy checksum into buffer
       for( i = 0; i < CHECKSUM_SIZE; i++ )
       {
           vBuffer[ i + 2 ] = *((LPBYTE)( &kCheckSum ) + i );
       }
       TRACE( "Checksum: %u.", kCheckSum );

       TRACE( "\r\nSending packet: " );
       // Encrypt the data.	
	    for( i = 0; i < vBuffer.size() - HEADER_SIZE; i++ )
       {
          TRACE( "%u( %c ) ", vBuffer[ i + HEADER_SIZE ], vBuffer[ i + HEADER_SIZE ] );
		    vBuffer[ i + HEADER_SIZE ] = vBuffer[ i + HEADER_SIZE ] ^ rndEncrypt;
       }
FUNCTION OFF. No longer using this encryption method. */
}

//////////////////////////////////////////////////////////////////////////////////////////
BOOL TFCPacket::DecryptPacket( unsigned int seedNumber )
//////////////////////////////////////////////////////////////////////////////////////////
// Decrypts an incoming packet from the client.
// 
//////////////////////////////////////////////////////////////////////////////////////////
{	
	return TRUE;
/* FUNCTION OFF. No longer using this encryption method.
	   // Minimal packet length
	   int i;

	   //Lets backup the buffer
	   vector<BYTE> vBackupBuffer(vBuffer);

       // First fetch the key.
       KEY kKey;
       for( i = 0; i < KEY_SIZE; i++ ){
           *((LPBYTE)( &kKey ) + i ) = vBuffer[ i ];
       }

       CHECKSUM kCheckSum;
       for( i = 0; i < CHECKSUM_SIZE; i++ ){
           *((LPBYTE)( &kCheckSum ) + i ) = vBuffer[ i + 2 ];
       }

   #ifdef _AFXDLL
       EncRandom rndEncrypt( 0, 255, kKey );
   #else
       Random rndEncrypt( 0, 255, kKey, seedNumber );
   #endif

       TRACE( "\r\nDecrypted data: " );
       // Decrypt data.
       for( i = 0; i < vBuffer.size() - HEADER_SIZE; i++ ){
		   vBuffer[ i + HEADER_SIZE ] = vBuffer[ i + HEADER_SIZE ] ^ rndEncrypt;
           TRACE( " %u( %c )", vBuffer[ i + HEADER_SIZE ], vBuffer[ i + HEADER_SIZE ] );
	   }

	   CHECKSUM kCheck = 0;
       // Calculate checksum.
       for( i = 0; i < vBuffer.size() - HEADER_SIZE; i++ ){
           kCheck += vBuffer[ i + HEADER_SIZE ];
       }

       TRACE( "\r\nkCheck=%u, kCheckSum=%u", kCheck, kCheckSum );
       // If the two values are the same.
       if( kCheck == kCheckSum ){
		   packetSeedID = seedNumber;
           return TRUE;
       }

	   //Restores the buffer before returning false
	   //so it can be decrypted again if needed
	   vBuffer = vBackupBuffer;
      return FALSE;
FUNCTION OFF. No longer using this encryption method. */
}

//////////////////////////////////////////////////////////////////////////////////////////
TFCPacket &TFCPacket::operator << (short value)
//////////////////////////////////////////////////////////////////////////////////////////
{	
    vBuffer.push_back( HIBYTE(value) );
	vBuffer.push_back( LOBYTE(value) );

	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////
TFCPacket &TFCPacket::operator << (char value)
//////////////////////////////////////////////////////////////////////////////////////////
{
    vBuffer.push_back( (BYTE)value );
	
    return(*this);
}

//////////////////////////////////////////////////////////////////////////////////////////
TFCPacket &TFCPacket::operator << (long value)
//////////////////////////////////////////////////////////////////////////////////////////
{
	vBuffer.push_back( HIBYTE(HIWORD(value)) );
	vBuffer.push_back( LOBYTE(HIWORD(value)) );
	vBuffer.push_back( HIBYTE(LOWORD(value)) );
	vBuffer.push_back( LOBYTE(LOWORD(value)) );

	return(*this);
}

//////////////////////////////////////////////////////////////////////////////////////////
TFCPacket & TFCPacket:: operator << (char * lpszString){
//////////////////////////////////////////////////////////////////////////////////////////
	int nStrLen = strlen(lpszString);

	// Stored string length.
   //vBuffer.push_back( HIBYTE( nStrLen ) );
	//vBuffer.push_back( LOBYTE( nStrLen ) );
	
    // Copy string into vector.
    copy( lpszString, lpszString + nStrLen, back_inserter( vBuffer ) );
		
	return *this;
}

/*
#ifdef _AFXDLL
//////////////////////////////////////////////////////////////////////////////////////////
TFCPacket & TFCPacket:: operator << (CString &csString){
//////////////////////////////////////////////////////////////////////////////////////////
	int nStrLen = csString.GetLength();
	
	// Stored string length.
    vBuffer.push_back( HIBYTE( nStrLen ) );
	vBuffer.push_back( LOBYTE( nStrLen ) );
	
    LPBYTE lpStringBuffer = (LPBYTE)csString.GetBuffer( 0 );

    // Copy string buffer into vector
    copy( lpStringBuffer, lpStringBuffer + nStrLen, back_inserter( vBuffer ) );

    csString.ReleaseBuffer( nStrLen );
		
	return *this;
}
#endif
*/

//////////////////////////////////////////////////////////////////////////////////////////
void TFCPacket::Get(long *i)
//////////////////////////////////////////////////////////////////////////////////////////
{	
	*i = 0;
	if( HEADER_SIZE + nPos + sizeof( long ) <= vBuffer.size() ){
        *i =  vBuffer[ HEADER_SIZE + nPos++ ] << 24;
        *i += vBuffer[ HEADER_SIZE + nPos++ ] << 16;
        *i += vBuffer[ HEADER_SIZE + nPos++ ] << 8;
        *i += vBuffer[ HEADER_SIZE + nPos++ ];	
	}else{
        TFCPacketException *e = new TFCPacketException(1);
        throw(e);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
void TFCPacket::Get(short *i)
//////////////////////////////////////////////////////////////////////////////////////////
{
    *i = 0;
    if( HEADER_SIZE + nPos + sizeof( short ) <= vBuffer.size() ){
        *i =  vBuffer[ HEADER_SIZE + nPos++ ] << 8;
        *i += vBuffer[ HEADER_SIZE + nPos++ ];		
    }else{
		TFCPacketException *e = new TFCPacketException(1);
        throw(e);
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
void TFCPacket::Get(char *i)
//////////////////////////////////////////////////////////////////////////////////////////
{
	*i = 0;
	if( HEADER_SIZE + nPos + sizeof( char ) <= vBuffer.size() ){
        *i = vBuffer[ HEADER_SIZE + nPos++ ];	
	}else{
        TFCPacketException *e = new TFCPacketException(1);
        throw(e);
    }
}
////////////////////////////////////////////////////////////////////////////////////////////----------------------------------
void TFCPacket::Get(unsigned long *i)
//////////////////////////////////////////////////////////////////////////////////////////
{	
	*i = 0;
	if( HEADER_SIZE + nPos + sizeof( long ) <= vBuffer.size() ){
        *i =  vBuffer[ HEADER_SIZE + nPos++ ] << 24;
        *i += vBuffer[ HEADER_SIZE + nPos++ ] << 16;
        *i += vBuffer[ HEADER_SIZE + nPos++ ] << 8;
        *i += vBuffer[ HEADER_SIZE + nPos++ ];	
	}else{
        TFCPacketException *e = new TFCPacketException(1);
        throw(e);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
void TFCPacket::Get(unsigned short *i)
//////////////////////////////////////////////////////////////////////////////////////////
{
    *i = 0;
    if( HEADER_SIZE + nPos + sizeof( short ) <= vBuffer.size() ){
        *i =  vBuffer[ HEADER_SIZE + nPos++ ] << 8;
        *i += vBuffer[ HEADER_SIZE + nPos++ ];		
    }else{
        TFCPacketException *e = new TFCPacketException(1);
        throw(e);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCPacket::Get(unsigned char *i)
//////////////////////////////////////////////////////////////////////////////////////////
{
	*i = 0;
	if( HEADER_SIZE + nPos + sizeof( char ) <= vBuffer.size() ){
        *i = vBuffer[ HEADER_SIZE + nPos++ ];	
	}else{
        TFCPacketException *e = new TFCPacketException(1);
        throw(e);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
BOOL TFCPacket::isEnd(void)
//////////////////////////////////////////////////////////////////////////////////////////
{
	if( HEADER_SIZE + nPos <= vBuffer.size() ){
		return TRUE;
	}else{
		return FALSE;
   }
}

/*
#ifdef _AFXDLL
//////////////////////////////////////////////////////////////////////////////////////////
CString TFCPacket::GetDebugPacketString( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the packet content inside a user-viewable string.
// 
// Return: CString, The returned packet
//////////////////////////////////////////////////////////////////////////////////////////
{
	CString csTemp;
	CString csFinal;

	int i;

	// Scroll through packet
	for( i = 0; i < vBuffer.size() - HEADER_SIZE; i++ ){
		csTemp.Format( "%u ", vBuffer[ HEADER_SIZE + i ] );
		csFinal += csTemp;
	}

	return csFinal;
}
#endif
*/

//////////////////////////////////////////////////////////////////////////////////////////
BOOL TFCPacket::SetBuffer
//////////////////////////////////////////////////////////////////////////////////////////
// Sets the buffer for the packet.
// 
(
 LPBYTE lpNewBuffer, // The buffer.
 int nBufferSize
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If given packet buffer is big enough to hold wanted information.
    if( nBufferSize < HEADER_SIZE + sizeof( RQ_SIZE ) ){
        return FALSE;
    }
    // Destroy previous packet.
    vBuffer.erase( vBuffer.begin(), vBuffer.end() );
    
    copy( lpNewBuffer, lpNewBuffer + nBufferSize, back_inserter( vBuffer ) );    

    nPos = 0;

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCPacket::GetBuffer
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the packet's buffer and its size.
// 
(
 LPBYTE &lpNewBuffer,   // The buffer to put the value to.
 int &nBufferSize       // The size of the buffer.
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    lpNewBuffer = &*vBuffer.begin();
    nBufferSize = vBuffer.size();
}

//////////////////////////////////////////////////////////////////////////////////////////
unsigned int TFCPacket::GetPacketSeedID
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the packet's seed ID
// 
(
)
//////////////////////////////////////////////////////////////////////////////////////////
{
	return packetSeedID;
}

//////////////////////////////////////////////////////////////////////////////////////////
void TFCPacket::SetPacketSeedID
//////////////////////////////////////////////////////////////////////////////////////////
// Sets the packet's seed ID
// 
(
 unsigned int newPacketSeedID
)
//////////////////////////////////////////////////////////////////////////////////////////
{
	packetSeedID = newPacketSeedID;
}

//////////////////////////////////////////////////////////////////////////////////////////
RQ_SIZE TFCPacket::GetPacketID( void )
//////////////////////////////////////////////////////////////////////////////////////////
// Returns the packet type.
// 
// Return: RQ_SIZE, the packet type.
//////////////////////////////////////////////////////////////////////////////////////////
{
    // If there is at least a packetID.
    if( vBuffer.size() - HEADER_SIZE >= sizeof( RQ_SIZE ) ){
        int nOldPos = nPos;

        // Go to the beginning of the packet.
        nPos = 0;
        RQ_SIZE rqPacketID = 0;

        // Fetch the packetID.
        Get( (RQ_SIZE *)&rqPacketID );

        // Restore old position.
        nPos = nOldPos;

        return rqPacketID;
    }

    return 0;
}