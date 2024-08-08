#pragma once

#include "NtlGame.h"

#define NTL_MAX_LENGTH_OF_CHAT_MESSAGE				(120)
#define NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE		(120)
#define NTL_NUMBER_OF_PLAYERS_IN_SAY_PACKET			(50)
#define NTL_MAX_LENGTH_OF_ID_AND_PASS				(12)
#define NTL_MAX_LENGTH_OF_ID_AND_PASS_UNICODE		(12)
#define NTL_MAX_LENGTH_OF_UUID						(16)
#define NTL_MAX_LENGTH_OF_CHARACTER					(20)
#define NTL_MAX_LENGTH_OF_CHARACTER_NAME_UNICODE	(20)
#define NTL_MAX_LENGTH_OF_IP_AND_PORT				(64)
#define NTL_NUMBER_OF_PLAYERS						(5)

struct SNtlPacketName
{
	SNtlPacketName(UInt32 uiNewOpCode, char* pszNewPacketName)
	{
		uiOpCode = uiNewOpCode;
		pszPacketName = pszNewPacketName;
	};

	UInt32 uiOpCode;
	char* pszPacketName;
};