#include <queue>
#include <map>
#include <process.h>
#include <mmsystem.h>

//#define NM_CRYPT_SERVER 

#ifndef NM_CRYPT_SERVER

class TFCCrypt
{

public:
  TFCCrypt(UINT dwKey);
	int DecryptC(LPBYTE &pBuffer, int &pBufferSize, unsigned int dwKey = 0xFFFFFFFF); // Decrypts pBuffer and returns TRUE on success and FALSE on failure
	int EncryptC(LPBYTE &pBuffer, int &pBufferSize, unsigned int dwKey = 0xFFFFFFFF); // Encrypts pBuffer and returns the seed used on encryption
private:
	DWORD EncryptC2(LPBYTE &pBuffer, int &pBufferSize,unsigned int dwKey = 0xFFFFFFFF);

protected:
   UINT m_dwKEY;
};

#else

   namespace TFCCrypt 
   {   
	   int DecryptS(LPBYTE &pBuffer, int &pBufferSize, unsigned int dwKey = 0xFFFFFFFF); // Decrypts pBuffer and returns TRUE on success and FALSE on failure
	   int EncryptS(LPBYTE &pBuffer, int &pBufferSize, unsigned int dwKey = 0xFFFFFFFF); // Encrypts pBuffer and returns the seed used on encryption
      DWORD DecryptS2(LPBYTE &pBuffer, int &pBufferSize,unsigned int dwKey = 0xFFFFFFFF);
   }
#endif