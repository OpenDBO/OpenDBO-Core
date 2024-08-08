#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpChTU_Nop								(0x70000000)
#define OpChTU_CharLoadResult					(0x70000001)
#define OpChTU_CharCreateResult					(0x70000002)
#define OpChTU_CharDeleteResult					(0x70000003)
#define OpChTU_CharSelectResult					(0x70000004)

extern SNtlPacketName* g_pNtlPacketName_ChTU[];

#pragma pack(1)

struct SChTU_Nop
{
	SPacketHeader	header;
};

struct tagCharInfo
{
	UInt32			uiCharID;
	WChar			wchName[NTL_MAX_LENGTH_OF_CHARACTER_NAME_UNICODE];
	Byte			byRace;
	Byte			byClass;
	Byte			bySex;
	float			fPositionX;
	float			fPositionY;
	float			fPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
	UInt32			uiZoneID;
	Byte			byLevel;
};

struct SChTU_CharLoadResult
{
	SPacketHeader	header;
	WORD         	wRet;
	Byte            byCount;
	//{
	tagCharInfo sChar[NTL_NUMBER_OF_PLAYERS]; // Maximum of Character is 5 (Temporary)   
	//}
	//Repeat information by Count of Character
};

struct SChTU_CharCreateResult
{
	SPacketHeader	header;
	WORD            wRet;
	tagCharInfo		charInfo;
};

struct SChTU_CharDeleteResult
{
	SPacketHeader	header;
	WORD            wRet;
	UInt32			uiCharID;
};

struct SChTU_CharSelectResult
{
	SPacketHeader	header;
	Byte            byUUID[NTL_MAX_LENGTH_OF_UUID];
	UInt32			uiCharID;
	WORD			wRet;
	char			szGameServerIP[NTL_MAX_LENGTH_OF_IP_AND_PORT + 1];
	UInt32			uiGameServerPortForClient;
	char			szChatServerIP[NTL_MAX_LENGTH_OF_IP_AND_PORT + 1];
	UInt32			uiChatServerPortForClient;
};
#pragma pack()