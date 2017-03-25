#ifndef ENCRYPTER_H_09032001
#define ENCRYPTER_H_09032001

#include <string>

// Make sure the string is always twice as big as the buffer.
void EncryptToString( std::string &str, const char *buffer, int bufSize, const char *key );
void DecryptFromString( char *buffer, const std::string &str, const char *key );

#endif