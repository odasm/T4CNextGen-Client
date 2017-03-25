// TFCPacket.h: interface for the TFCPacket class.
//
// THIS CLASS HANDLES PACKETS!
// That is, it includes functions to work with packets:
// setup the header, add a number (char, int, long)
// retreive a number (char, int, long) with Packet << value
// or Packet >> value.
//
//
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TFCPACKET_H__CCD2D7A2_B380_11D0_9B9E_444553540000__INCLUDED_)
#define AFX_TFCPACKET_H__CCD2D7A2_B380_11D0_9B9E_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>

using namespace std;

class TFCPacketException{
public:
	TFCPacketException(UINT error);
	UINT m_cause;
};

typedef short RQ_SIZE;

#define KEY_SIZE        2
#define CHECKSUM_SIZE   2

typedef WORD KEY;
typedef WORD CHECKSUM;

class TFCPacket 
{
public:
	TFCPacket();
	virtual ~TFCPacket();

	void Create(unsigned int length); // Creates a packet
	void Destroy();                   // Destroys a packet
	void Seek(signed long where, char how);    // Seeks within a packet for << and >>

	void Get(short *);				  // Same as >>
	void Get(char *);				  // Gets a "number" from the packet	
	void Get(long *);	
	void Get(unsigned short *);
	void Get(unsigned char *);
	void Get(unsigned long *);
	BOOL isEnd(void);

	void EncryptPacket( void );
	BOOL DecryptPacket( unsigned int seedNumber = 0 );

	TFCPacket & operator << (long);   // Insertion operators
	TFCPacket & operator << (short);
	TFCPacket & operator << (char);
   TFCPacket & operator << (char *);

	int GetPos( void ){ return nPos; };
//#ifdef _AFXDLL
    //TFCPacket & operator << (CString &);
	//CString GetDebugPacketString( void );
//#endif

    BOOL SetBuffer( LPBYTE lpBuffer, int nBufferSize );
    void GetBuffer( LPBYTE &lpBuffer, int &nBufferSize );
	unsigned int GetPacketSeedID(void);
	void		 SetPacketSeedID(unsigned int newPacketSeedID);

    RQ_SIZE GetPacketID( void );
private:
    
    vector< BYTE > vBuffer;

    int     nPos;
	unsigned int packetSeedID;

//  int     nLen;

//  BOOL    boUserBuffer;
};



#endif // !defined(AFX_TFCPACKET_H__CCD2D7A2_B380_11D0_9B9E_444553540000__INCLUDED_)
