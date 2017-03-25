#ifndef __VIRCOM_C2STRING_02301005100298__
#define __VIRCOM_C2STRING_02301005100298__

#include <windows.h>

typedef unsigned int FLAG;

// *** CLASS ***************************************************************** //
class String {
// This class manage all Windows Class (This is a wrap around RegisterClassEx).
// *****************************
   private:
   // DATA
      LPTSTR lpszString;  // String

// ******************************
   public:
   // CODE
      String( VOID );               // Constructor
      ~String( VOID );              // Destructor
      VOID Release( VOID );         // Release Function

      // String Logical Operator
      VOID operator = ( LPTSTR );   // Init the String
      BOOL operator == ( LPTSTR );  // Compare to another String
      BOOL operator /= ( LPTSTR );  // Comapre a String to another String using only the len of another String.
      VOID operator += ( LPTSTR );  // Append another String
      VOID operator += ( BYTE );    // Aooend a Byte

      operator LPTSTR ( VOID );     // Polymorph into LPTSTR

};

#define DF_READ               0x00000001
#define DF_WRITE              0x00000002
#define DF_SHARE_DELETE       0x00000004
#define DF_SHARE_READ         0x00000008
#define DF_SHARE_WRITE        0x00000010
#define DF_CREATE_NEW         0x00000020
#define DF_CREATE_ALWAYS      0x00000040
#define DF_OPEN_EXISTING      0x00000080
#define DF_OPEN_ALWAYS        0x00000100
#define DF_TRUNCATE_EXISTING  0x00000200

class File {
   public:
      BOOL Open(LPTSTR, FLAG);
      LPTSTR Read(LPTSTR, DWORD);
      DWORD GetFileSize(VOID);
      VOID Write(LPTSTR);
      VOID Close(VOID);

   private:
      HANDLE hFile;
      DWORD dwFileSize;
};

#endif // __VIRCOM_C2STRING_02301005100298__