#include "../T4C Client/MemoryManager.h"
#include "newstring.h"

// *** CONSTRUCTOR ********************************************************** //
String::String( VOID ) 
// ************************************************************************** //
// This consturctor initialize all String local data.
{
   lpszString = 0;
}

// *** DESTRUCTOR *********************************************************** //
String::~String( VOID ) 
// ************************************************************************** //
// This destructor clean up all String local data.
{
   Release();
}

// ************************************************************************** //
VOID String::Release( VOID ) 
// ************************************************************************** //
// This function clean up all String local data.
{
   if (lpszString) {
      delete lpszString;
      lpszString = NULL;
   }
}

// ************************************************************************** //
VOID String::operator = 
// ************************************************************************** //
(
 LPTSTR lpszSource // String to initialize with
) 
// ************************************************************************** //
// This function initialize the String
{
   if (lpszString)
      delete lpszString;
   lpszString = new TCHAR [strlen(lpszSource)+1];
   strcpy(lpszString, lpszSource);
}

// ************************************************************************** //
VOID String::operator += 
// ************************************************************************** //
(
 LPTSTR lpszSource // String to be appended with
) 
// ************************************************************************** //
// This function append a Sting to the existing String.
{
   LPTSTR lpszTemp;
   
   if (lpszString) {
      lpszTemp = new TCHAR [strlen(lpszSource) + strlen(lpszString) + 1];
      strcpy(lpszTemp, lpszString);
      strcat(lpszTemp, lpszSource);
      delete lpszString;
      lpszString = lpszTemp;
   } else {
      lpszString = new TCHAR [strlen(lpszSource)+1];
      strcpy(lpszString, lpszSource);
   }
}

// ************************************************************************** //
VOID String::operator += 
// ************************************************************************** //
(
 BYTE cSource // Byte to be appended with
) 
// ************************************************************************** //
// This function append a Byte to the existing String.
{
   LPTSTR lpszTemp;
  
   if (lpszString) {
      lpszTemp = new TCHAR [strlen(lpszString) + 2];
      strcpy(lpszTemp, lpszString);
      lpszTemp[strlen(lpszString)] = cSource;
      lpszTemp[strlen(lpszString)+1] = 0;
      delete lpszString;
      lpszString = lpszTemp;
   } else {
      lpszString = new TCHAR [2];
      lpszString[0] = cSource;
      lpszString[1] = 0;
   }
}

// ************************************************************************** //
String::operator LPTSTR ( VOID ) 
// ************************************************************************** //
// This function polymorph the string tobe a LPTSRT
{
   return lpszString;
}

// ************************************************************************** //
BOOL String::operator == 
// ************************************************************************** //
(
 LPTSTR lpszSource // String to compare
) 
// ************************************************************************** //
// This function compare a String
{
   return (!strcmp(lpszString, lpszSource));
}

// ************************************************************************** //
BOOL String::operator /= 
(
 LPTSTR lpszSource // String
) 
// ************************************************************************** //
// This function compare a string with the len of the String
{
   return (!strncmp(lpszString, lpszSource, strlen(lpszString)));
}