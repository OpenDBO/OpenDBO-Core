//***********************************************************************************
//
//	File		:	NtlPacketEncoder_RandKey.cpp
//
//	Begin		:	2007-01-31
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "NtlPacketEncoder_RandKey.h"

#include "NtlPacket.h"

#include "NtlError.h"
#include "NtlDebug.h"


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#define __PACKET_ENCODE_OPTIMIZE__
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
const BYTE				CHECKSUM_INITIAL_VALUE = 0x7F;


const DWORD				RX_RANDOM_SEED_HEADER = 0xFFEECCBB;

const DWORD				TX_RANDOM_SEED_HEADER = 0xBBCCEEFF;

const DWORD				RX_RANDOM_SEED_BODY = 0xAABBCCDD;

const DWORD				TX_RANDOM_SEED_BODY = 0xDDBBCCAA;


CNtlRandomGenerator		s_rxHeaderKeyGenerator( RX_RANDOM_SEED_HEADER );

CNtlRandomGenerator		s_txHeaderKeyGenerator( TX_RANDOM_SEED_HEADER );

CNtlRandomGenerator		s_rxBodyKeyGenerator( RX_RANDOM_SEED_BODY );

CNtlRandomGenerator		s_txBodyKeyGenerator( TX_RANDOM_SEED_BODY );
//-----------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------------
const BYTE s_checksum_Table[ ] =
{
	0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
	157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
	35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
	190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
	70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
	219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
	101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
	248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
	140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
	17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
	175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
	50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
	202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
	87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
	233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
	116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
} ;
//-----------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	: 8bit crc value
//-----------------------------------------------------------------------------------
int Encrypt(BYTE * pPlain, int nLength, DWORD dwCipherKey, BYTE * pbyCheckSum = NULL)
{
	if ( NULL == pPlain )
	{
		return 0;
	}


	if( pbyCheckSum )
	{
		*pbyCheckSum = CHECKSUM_INITIAL_VALUE;
	}


#ifdef __PACKET_ENCODE_OPTIMIZE__

	int nRound = nLength / sizeof(DWORD);
	DWORD * pdwRoundPlain = (DWORD*) pPlain;
	for( int i = 0; i < nRound; i++ )
	{
		if( pbyCheckSum )
		{
			*pbyCheckSum = s_checksum_Table[ *pbyCheckSum ^ (BYTE) pdwRoundPlain[ i ] ];
		}

		pdwRoundPlain[ i ] ^= (DWORD) dwCipherKey;
	}


	int nRest = nLength % sizeof(DWORD);
	BYTE * pbyRestPlain = (BYTE*) ( pdwRoundPlain + nRound );
	for( int i = 0; i < nRest; i++ )
	{
		if( pbyCheckSum )
		{
			*pbyCheckSum = s_checksum_Table[ *pbyCheckSum ^ pbyRestPlain[ i ] ] ;
		}

		pbyRestPlain[ i ] ^= (BYTE) dwCipherKey;
	}
#else
	for( int i = 0; i < nLength; i++ )
	{
		if( pbyCheckSum )
		{
			*pbyCheckSum = s_checksum_Table[ *pbyCheckSum ^ pPlain[ i ] ] ;
		}

		pPlain[ i ] ^= (BYTE) dwCipherKey;
	}
#endif


	return nLength;
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int Decrypt(BYTE * pPlain, int nLength, DWORD dwCipherKey, BYTE * pbyCheckSum = NULL)
{
	if ( NULL == pPlain )
	{
		return 0;
	}


	if( pbyCheckSum )
	{
		*pbyCheckSum = CHECKSUM_INITIAL_VALUE;
	}


#ifdef __PACKET_ENCODE_OPTIMIZE__
	int nRound = nLength / sizeof(DWORD);
	DWORD * pdwRoundPlain = (DWORD*) pPlain;
	for( int i = 0; i < nRound; i++ )
	{
		pdwRoundPlain[ i ] ^= (DWORD) dwCipherKey;

		if( pbyCheckSum )
		{
			*pbyCheckSum = s_checksum_Table[ *pbyCheckSum ^ (BYTE) pdwRoundPlain[ i ] ];
		}
	}


	int nRest = nLength % sizeof(DWORD);
	BYTE * pbyRestPlain = (BYTE*) ( pdwRoundPlain + nRound );
	for( int i = 0; i < nRest; i++ )
	{
		pbyRestPlain[ i ] ^= (BYTE) dwCipherKey;

		if( pbyCheckSum )
		{
			*pbyCheckSum = s_checksum_Table[ *pbyCheckSum ^ pbyRestPlain[ i ] ] ;
		}
	}
#else
	for( int i = 0; i < nLength; i++ )
	{
		pPlain[ i ] ^= (BYTE) dwCipherKey;

		if( pbyCheckSum )
		{
			*pbyCheckSum = s_checksum_Table[ *pbyCheckSum ^ pPlain[ i ] ] ;
		}
	}
#endif

	return nLength;
}



//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacketEncoder_RandKey::CNtlPacketEncoder_RandKey(bool bSeedSwap)
{
	if( bSeedSwap )
	{
		m_rxHeaderKeyGenerator = s_txHeaderKeyGenerator;

		m_txHeaderKeyGenerator = s_rxHeaderKeyGenerator;

		m_rxBodyKeyGenerator = s_txBodyKeyGenerator;

		m_txBodyKeyGenerator = s_rxBodyKeyGenerator;	
	}
	else
	{
		m_rxHeaderKeyGenerator = s_rxHeaderKeyGenerator;

		m_txHeaderKeyGenerator = s_txHeaderKeyGenerator;

		m_rxBodyKeyGenerator = s_rxBodyKeyGenerator;

		m_txBodyKeyGenerator = s_txBodyKeyGenerator;	
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPacketEncoder_RandKey::~CNtlPacketEncoder_RandKey(void)
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPacketEncoder_RandKey::RxDecrypt(void * pvPacketHeader)
{
	PACKETHEADER* pPacketHeader = (PACKETHEADER*)pvPacketHeader;

	if( pPacketHeader->bEncrypt )
	{
		Decrypt( (BYTE*) pPacketHeader, PACKET_HEADSIZE, m_rxHeaderKeyGenerator.Generate() );
		pPacketHeader->bEncrypt = false;
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPacketEncoder_RandKey::TxEncrypt(void * pvPacketHeader)
{
	PACKETHEADER* pPacketHeader = (PACKETHEADER*)pvPacketHeader;

	if( false == pPacketHeader->bEncrypt )
	{
		Encrypt( (BYTE*) pPacketHeader, PACKET_HEADSIZE, m_txHeaderKeyGenerator.Generate() );
		pPacketHeader->bEncrypt = true;
	}

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPacketEncoder_RandKey::RxDecrypt(CNtlPacket& rPacket)
{
	// Decrypt Header
	RxDecrypt( rPacket.GetPacketHeader() );


	// Decrypt Body
	BYTE byCheckSum = CHECKSUM_INITIAL_VALUE;
	if( rPacket.GetPacketDataSize() != Decrypt( rPacket.GetPacketData(), rPacket.GetPacketDataSize(), m_rxBodyKeyGenerator.Generate(), &byCheckSum  ) )
	{
		return NTL_ERR_NET_PACKET_DECRYPT_FAIL;
	}

	// Checksum
	NTL_PRINT( PRINT_SYSTEM, "Rx PacketLen[%u] Sequence[%u] Checksum[%u]", rPacket.GetPacketHeader()->wPacketLen, rPacket.GetPacketHeader()->bySequence, rPacket.GetPacketHeader()->byChecksum );
	if( byCheckSum != rPacket.GetPacketHeader()->byChecksum )
	{
		return NTL_ERR_NET_PACKET_CHECKSUM_FAIL;
	}


	return NTL_SUCCESS;
}

 
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPacketEncoder_RandKey::TxEncrypt(CNtlPacket& rPacket)
{
	// Encrypt Body
	BYTE byCheckSum = CHECKSUM_INITIAL_VALUE;
	if( rPacket.GetPacketDataSize() != Encrypt( rPacket.GetPacketData(), rPacket.GetPacketDataSize(), m_txBodyKeyGenerator.Generate(), &byCheckSum ) )
	{
		return NTL_ERR_NET_PACKET_ENCRYPT_FAIL;
	}


	// Checksum
	rPacket.GetPacketHeader()->byChecksum = byCheckSum;


	NTL_PRINT( PRINT_SYSTEM, "TxEncryptPacketLen[%u] Sequence[%u] Checksum[%u]", rPacket.GetPacketHeader()->wPacketLen, rPacket.GetPacketHeader()->bySequence, rPacket.GetPacketHeader()->byChecksum );

	// Encrypt Header
	TxEncrypt( rPacket.GetPacketHeader() );



	return NTL_SUCCESS;
}

