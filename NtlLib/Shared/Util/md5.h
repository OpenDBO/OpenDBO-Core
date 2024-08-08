#pragma once
#include "Cipher.h"

/* MD5.H - header file for MD5C.C
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */

typedef struct{
  DWORD state[4];                                   /* state (ABCD) */
  DWORD count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
}MD5_CTX;

class MD5 
	:public CCipher
{
public:
    MD5();
	~MD5();

    void Update(unsigned char *, unsigned int, MD5_CTX *pMD5Ctx);
    void Final(unsigned char digest[16], MD5_CTX *pMD5Ctx);
	
	virtual int SetKey(unsigned char *cipherKey,int cipherKeyLength);
	virtual int Encrypt(unsigned char *originalData, int originalDataLength,unsigned char *encData, int encDataSize);
	virtual int Decrypt(unsigned char *originalData, int originalDataLength,unsigned char *decData, int decDataSize);
	virtual void ReleaseCipher(void){return;};

};



