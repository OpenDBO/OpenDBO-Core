//***********************************************************************************
//
//	File		:	NtlPacketEncoder_XOR.cpp
//
//	Begin		:	2007-01-29
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	YOSHIKI
//
//	Desc		:	
//
//***********************************************************************************


#include "StdAfx.h"
#include "NtlPacketEncoder_XOR.h"

#include "NtlPacket.h"

#include "NtlError.h"



//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#define NTL_ENCRYPTION_MAGIC_PATTERN_UPPER		(0x6B)
#define NTL_ENCRYPTION_MAGIC_PATTERN_LOWER		(0x31)
#define NTL_ENCRYPTION_MAGIC_PATTERN		(NTL_ENCRYPTION_MAGIC_PATTERN_UPPER << 8 | NTL_ENCRYPTION_MAGIC_PATTERN_LOWER)
//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD Encrypt(void* pvOriginalString, DWORD dwOriginalLength, void* pvBuffer, DWORD dwBufferSize)
{
#ifdef _DEVEL
	if (FALSE != IsBadReadPtr(pvOriginalString, dwOriginalLength))
		return 0;
	if (FALSE != IsBadWritePtr(pvBuffer, dwBufferSize))
		return 0;
#else
	if (NULL == pvOriginalString)
		return 0;
	if (NULL == pvBuffer)
		return 0;
#endif
	if (dwBufferSize < dwOriginalLength)
		return 0;

	DWORD dwCurrentOffset = 0;

	while (dwCurrentOffset < dwOriginalLength)
	{
		if (dwOriginalLength - dwCurrentOffset >= 2)
		{
			*(WORD*)((BYTE*)pvBuffer + dwCurrentOffset) = NTL_ENCRYPTION_MAGIC_PATTERN ^ *(WORD*)((BYTE*)pvOriginalString + dwCurrentOffset);
		}
		else
		{
			// Assumes that this routine is executed on intel CPU or its compatible CPU(little endian).
			// - YOSHIKI
			*((BYTE*)pvBuffer + dwCurrentOffset) = NTL_ENCRYPTION_MAGIC_PATTERN_LOWER ^ *((BYTE*)pvOriginalString + dwCurrentOffset);
		}
		dwCurrentOffset++;
	}

	return dwCurrentOffset;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
DWORD Decrypt(void* pvOriginalString, DWORD dwOriginalLength, void* pvBuffer, DWORD dwBufferSize)
{
	return Encrypt(pvOriginalString, dwOriginalLength, pvBuffer, dwBufferSize);
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
#include <stdio.h>
bool EncryptUnitTest()
{
	char szOriginalString[] = "Hello! This is the test sentence!";
	char szEncryptedString[100];
	char szDecryptedString[100];

	printf("szOriginalString(%d bytes) = %s\n", strlen(szOriginalString), szOriginalString);

	DWORD dwWrittenBytes = Encrypt(szOriginalString, (DWORD)(strlen(szOriginalString)), szEncryptedString, sizeof(szEncryptedString));
	if (0 == dwWrittenBytes)
	{
		printf("encryption ERROR!\n");
		return false;
	}

	szEncryptedString[dwWrittenBytes] = '\0';
	printf("szEncryptedString(%d bytes) = %s\n", dwWrittenBytes, szEncryptedString);

	dwWrittenBytes = Decrypt(szEncryptedString, dwWrittenBytes, szDecryptedString, sizeof(szDecryptedString));
	if (0 == dwWrittenBytes)
	{
		printf("decryption ERROR!\n");
		return false;
	}

	szDecryptedString[dwWrittenBytes] = '\0';
	printf("szDecryptedString(%d bytes) = %s\n", dwWrittenBytes, szDecryptedString);

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacketEncoder_XOR::CNtlPacketEncoder_XOR(void)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacketEncoder_XOR::~CNtlPacketEncoder_XOR(void)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPacketEncoder_XOR::RxDecrypt(CNtlPacket& rPacket)
{
	Decrypt( rPacket.GetPacketData(), rPacket.GetPacketDataSize(), rPacket.GetPacketData(), rPacket.GetPacketDataSize() );

	return NTL_SUCCESS;
}

 
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPacketEncoder_XOR::TxEncrypt(CNtlPacket& rPacket)
{
	Encrypt( rPacket.GetPacketData(), rPacket.GetPacketDataSize(), rPacket.GetPacketData(), rPacket.GetPacketDataSize() );

	return NTL_SUCCESS;
}
