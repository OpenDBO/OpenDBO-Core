#include "stdafx.h"
#include "NtlPacket.h"

UInt32 NtlPacket_GetOpCode(void* pvPacket)
{
	SPacketHeader* pHeader = (SPacketHeader*)pvPacket;
	if (FALSE != IsBadReadPtr(pHeader, sizeof(*pHeader)))
		return (UInt32)0xFFFFFFFF;

	return (pHeader->dwOp);
}

ENtlPacketSentFrom NtlPacket_SentFrom(Byte* pbyPacket)
{
	SPacketHeader* pHeader = (SPacketHeader*)pbyPacket;
	UInt32 uiOp = pHeader->dwOp;
	UInt32 uiSentFrom = (UInt32)0xF0000000 & uiOp;

	switch (uiSentFrom)
	{
	case SENT_FROM_USER :
	case SENT_FROM_ACCOUNT_SERVER :
	case SENT_FROM_GAME_SERVER :
	case SENT_FROM_CHAT_SERVER :
	case SENT_FROM_DB_SERVER :
	case SENT_FROM_LOG_SERVER :
	case SENT_FROM_MONITORING_SERVER :
	case SENT_FROM_CHARACTER_SERVER :
		return (ENtlPacketSentFrom)uiSentFrom;
		break;

	default :
		break;
	};

	return (ENtlPacketSentFrom)SENT_FROM_UNKNOWN_SERVER;
}

ENtlPacketSentTo NtlPacket_SentTo(Byte* pbyPacket)
{
	SPacketHeader* pHeader = (SPacketHeader*)pbyPacket;
	UInt32 uiOp = pHeader->dwOp;
	UInt32 uiSentTo = (DWORD)0x0F000000 & uiOp;

	switch (uiSentTo)
	{
	case SENT_TO_USER :
	case SENT_TO_ACCOUNT_SERVER :
	case SENT_TO_GAME_SERVER :
	case SENT_TO_CHAT_SERVER :
	case SENT_TO_DB_SERVER :
	case SENT_TO_LOG_SERVER :
	case SENT_TO_MONITORING_SERVER :
	case SENT_TO_CHARACTER_SERVER :
		return (ENtlPacketSentTo)uiSentTo;
		break;

	default :
		break;
	};

	return (ENtlPacketSentTo)SENT_TO_UNKNOWN_SERVER;
}

char* NtlPacket_GetPacketName(void* pvPacket)
{
	SPacketHeader* pHeader = (SPacketHeader*)pvPacket;
	if (FALSE != IsBadReadPtr(pHeader, sizeof(*pHeader)))
		return NULL;

	UInt32 uiOpCode = NtlPacket_GetOpCode(pvPacket);
	UInt32 uiIdentifier = 0xFF000000 & uiOpCode;
	SNtlPacketName** ppPacketName = NULL;

	switch (uiIdentifier)
	{
	case 0x17000000 :		// ATCh
		ppPacketName = g_pNtlPacketName_ATCh;
		break;

	case 0x10000000 :		// ATU
		ppPacketName = g_pNtlPacketName_ATU;
		break;

	case 0x71000000 :		// ChTA
		ppPacketName = g_pNtlPacketName_ChTA;
		break;

	case 0x73000000 :		// ChTC
		ppPacketName = g_pNtlPacketName_ChTC;
		break;

	case 0x72000000 :		// ChTG
		ppPacketName = g_pNtlPacketName_ChTG;
		break;

	case 0x70000000 :		// ChTU
		ppPacketName = g_pNtlPacketName_ChTU;
		break;

	case 0x37000000 :		// CTCh
		ppPacketName = g_pNtlPacketName_CTCh;
		break;

	case 0x32000000 :		// CTG
		ppPacketName = g_pNtlPacketName_CTG;
		break;

	case 0x30000000 :		// CTU
		ppPacketName = g_pNtlPacketName_CTU;
		break;

	case 0x23000000 :		// GTC
		ppPacketName = g_pNtlPacketName_GTC;
		break;

	case 0x27000000 :		// GTCh
		ppPacketName = g_pNtlPacketName_GTCh;
		break;

	case 0x20000000 :		// GTU
		ppPacketName = g_pNtlPacketName_GTU;
		break;

	case 0x01000000 :		// UTA
		ppPacketName = g_pNtlPacketName_UTA;
		break;

	case 0x03000000 :		// UTC
		ppPacketName = g_pNtlPacketName_UTC;
		break;

	case 0x07000000 :		// UTCh
		ppPacketName = g_pNtlPacketName_UTCh;
		break;

	case 0x02000000 :		// UTG
		ppPacketName = g_pNtlPacketName_UTG;
		break;

	default :
		break;
	}

	if (NULL == ppPacketName)
		return NULL;

	DWORD dwIndex = 0;
	while (NULL != ppPacketName[dwIndex])
	{
		if (uiOpCode == ppPacketName[dwIndex]->uiOpCode)
		{
			return ppPacketName[dwIndex]->pszPacketName;
		}

		dwIndex++;
	}

	return NULL;
}