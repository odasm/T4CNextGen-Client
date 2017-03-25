//#include "stdafx.h"
#include <windows.h>
#include "encrypter.h"

#define ATLTRACE (NULL)

void Encrypt( char *target, const char *buffer, int bufSize, const char *key );
void Decrypt( char *target, const char *buffer, int bufSize, const char *key );
void BinaryToString( char *str, const char *buffer, int bufSize );
void StringToBinary( char *buf, const char *str );

//////////////////////////////////////////////////////////////////////////////////////////
void Encrypt
//////////////////////////////////////////////////////////////////////////////////////////
// Encrypts a given buffer using the given char as the key.
// 
(
 char *target, // 
 const char *buffer, // 
 int bufSize, // 
 const char *key // 
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    DWORD seed = 0;
    int i;
    int len = strlen( key );
    for( i = 0; i < len; ++i ){
        if( isalpha( key[i] ) ){
            seed += toupper( key[ i ] );
        }
    }
    srand( seed );
    for( i = 0; i < bufSize; i++ ){
        target[ i ] = buffer[ i ] ^ ( rand() % 256 );
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
void Decrypt
//////////////////////////////////////////////////////////////////////////////////////////
// Encrypts a given buffer using the given char as the key.
// 
(
 char *target, // 
 const char *buffer, // 
 int bufSize, // 
 const char *key // 
 )
 //////////////////////////////////////////////////////////////////////////////////////////
{
    // Mutual exclusive encryption scheme
    Encrypt( target, buffer, bufSize, key );
}

//////////////////////////////////////////////////////////////////////////////////////////
void BinaryToString
//////////////////////////////////////////////////////////////////////////////////////////
// Converts a binary buffer into a user readable string (in hexadecimal format).
// 
(
 char *lpString,	// Pointer to the resulting string data, must be large enough (dwBufferSize * 2) !.
 const char *lpBuffer,	// The binary buffer
 int dwBufferSize // The buffer size
)
 //////////////////////////////////////////////////////////////////////////////////////////
{
    int i;
    BYTE bData;
    
    for( i = 0; i < dwBufferSize * 2; i++ ){
        
        // Fetch first 4 bits
        if( !( i % 2 ) ){
            bData = (BYTE)( (BYTE)lpBuffer[ i >> 1 ] & 0xF0 ) >> 4;
        }
        // Fetch second 4 bits
        else{
            bData = (BYTE)( (BYTE)lpBuffer[ i >> 1 ] & 0x0F );
        }
        
        switch( bData ){
        case 0:  lpString[ i ] = '0'; break;
        case 1:  lpString[ i ] = '1'; break;
        case 2:  lpString[ i ] = '2'; break;
        case 3:  lpString[ i ] = '3'; break;	
        case 4:  lpString[ i ] = '4'; break;
        case 5:  lpString[ i ] = '5'; break;
        case 6:  lpString[ i ] = '6'; break;
        case 7:  lpString[ i ] = '7'; break;	
        case 8:  lpString[ i ] = '8'; break;
        case 9:  lpString[ i ] = '9'; break;
        case 10: lpString[ i ] = 'A'; break;
        case 11: lpString[ i ] = 'B'; break;	
        case 12: lpString[ i ] = 'C'; break;
        case 13: lpString[ i ] = 'D'; break;
        case 14: lpString[ i ] = 'E'; break;
        case 15: lpString[ i ] = 'F'; break;	
        }
    }
    lpString[ i ] = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void StringToBinary
//////////////////////////////////////////////////////////////////////////////////////////
// Converts an hexadecimal data string into a binary buffer
// 
( 
 char *lpbBuffer,	// The resulting buffer. Must be able to hold dwSize / 2 bytes.
 const char *lpszString // The string to convert from. 
)
 //////////////////////////////////////////////////////////////////////////////////////////
{
    int i;
    int dwSize = strlen( lpszString );
    
    for( i = 0; i < ( dwSize >> 1 ); i++ ){
        // First byte is the higher 4 bits
        switch( lpszString[ i << 1 ] ){
        case '0': lpbBuffer[ i ] = (char)(0x0 << 4); ATLTRACE("0"); break;
        case '1': lpbBuffer[ i ] = (char)(0x1 << 4); ATLTRACE("1"); break;
        case '2': lpbBuffer[ i ] = (char)(0x2 << 4); ATLTRACE("2"); break;
        case '3': lpbBuffer[ i ] = (char)(0x3 << 4); ATLTRACE("3"); break;
        case '4': lpbBuffer[ i ] = (char)(0x4 << 4); ATLTRACE("4"); break;
        case '5': lpbBuffer[ i ] = (char)(0x5 << 4); ATLTRACE("5"); break;
        case '6': lpbBuffer[ i ] = (char)(0x6 << 4); ATLTRACE("6"); break;
        case '7': lpbBuffer[ i ] = (char)(0x7 << 4); ATLTRACE("7"); break;
        case '8': lpbBuffer[ i ] = (char)(0x8 << 4); ATLTRACE("8"); break;
        case '9': lpbBuffer[ i ] = (char)(0x9 << 4); ATLTRACE("9"); break;
        case 'A': lpbBuffer[ i ] = (char)(0xA << 4); ATLTRACE("a"); break;
        case 'B': lpbBuffer[ i ] = (char)(0xB << 4); ATLTRACE("b"); break;
        case 'C': lpbBuffer[ i ] = (char)(0xC << 4); ATLTRACE("c"); break;
        case 'D': lpbBuffer[ i ] = (char)(0xD << 4); ATLTRACE("d"); break;
        case 'E': lpbBuffer[ i ] = (char)(0xE << 4); ATLTRACE("e"); break;
        case 'F': lpbBuffer[ i ] = (char)(0xF << 4); ATLTRACE("f"); break;
        default:  lpbBuffer[ i ] = (char)(0x0 << 4); ATLTRACE("-"); break;
        }
        // Second byte is the lower 4 bits
        switch( lpszString[ ( i << 1 ) + 1 ] ){
        case '0': lpbBuffer[ i ] |= (char)0x0; ATLTRACE("0"); break;
        case '1': lpbBuffer[ i ] |= (char)0x1; ATLTRACE("1"); break;
        case '2': lpbBuffer[ i ] |= (char)0x2; ATLTRACE("2"); break;
        case '3': lpbBuffer[ i ] |= (char)0x3; ATLTRACE("3"); break;
        case '4': lpbBuffer[ i ] |= (char)0x4; ATLTRACE("4"); break;
        case '5': lpbBuffer[ i ] |= (char)0x5; ATLTRACE("5"); break;
        case '6': lpbBuffer[ i ] |= (char)0x6; ATLTRACE("6"); break;
        case '7': lpbBuffer[ i ] |= (char)0x7; ATLTRACE("7"); break;
        case '8': lpbBuffer[ i ] |= (char)0x8; ATLTRACE("8"); break;
        case '9': lpbBuffer[ i ] |= (char)0x9; ATLTRACE("9"); break;
        case 'A': lpbBuffer[ i ] |= (char)0xA; ATLTRACE("a"); break;
        case 'B': lpbBuffer[ i ] |= (char)0xB; ATLTRACE("b"); break;
        case 'C': lpbBuffer[ i ] |= (char)0xC; ATLTRACE("c"); break;		
        case 'D': lpbBuffer[ i ] |= (char)0xD; ATLTRACE("d"); break;
        case 'E': lpbBuffer[ i ] |= (char)0xE; ATLTRACE("e"); break;
        case 'F': lpbBuffer[ i ] |= (char)0xF; ATLTRACE("f"); break;
        default:  lpbBuffer[ i ] |= (char)0x0; ATLTRACE("-"); break;
        }
    }    
}

///////////////////////////////////////////////////////////////////////////////////////////
void EncryptToString
//////////////////////////////////////////////////////////////////////////////////////////
// Encrypts a buffer to a string.
// 
(
 std::string &str, // The string
 const char *buffer, // The buffer
 int bufSize, // The size of the buffer
 const char *key // The key
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    char *enc = new char[ bufSize ];
    // Encrypt
    Encrypt( enc, buffer, bufSize, key );

    char *newStr = new char[ bufSize * 2 + 1];
    // Convert the encrypted data to a string.
    BinaryToString( newStr, enc, bufSize );
    
    str = newStr;
    
    delete newStr;
    delete enc;
}
//////////////////////////////////////////////////////////////////////////////////////////
void DecryptFromString
//////////////////////////////////////////////////////////////////////////////////////////
// Decrypts an encrypted string into a buffer.
// 
(
 char *buffer, // The buffer
 const std::string &str, // The string
 const char *key // The key
)
//////////////////////////////////////////////////////////////////////////////////////////
{
    StringToBinary( buffer, str.c_str() );

    // Encrypt
    Decrypt( buffer, buffer, str.size() / 2, key );
    buffer[ str.size() / 2 ] = 0;
}