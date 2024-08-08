#ifndef _NCCOIN_CIPHER_LIB_H
#define _NCCOIN_CIPHER_LIB_H

#include "Cipher.h"

class CNtlCipher
{
public :
	CNtlCipher(void);
	~CNtlCipher(void);

	int	SetKey(unsigned char cipher, const char * cipherKey, int cipherKeyLength);	
	unsigned char	GetCipherType(void) {return mCipherType;};
	
	int  Encrypt(const char* originalData, int originalDataLength, char* encData,int encDataSize);
	int  Decrypt(const char* originalData, int originalDataLength, char* decData,int decDataSize);
	
	CCipher	*GetCipher(void) {return mCipherPtr;};
private:
	unsigned char	mCipherType;
	CCipher				*mCipherPtr;

};

#endif /*_NCCOIN_CIPHER_LIB_H*/