#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpUTCh_Nop								(0x07000000)
#define OpUTCh_CharLoadRequest					(0x07000001)
#define OpUTCh_CharCreateRequest				(0x07000002)
#define OpUTCh_CharDeleteRequest				(0x07000003)
#define OpUTCh_CharSelectRequest				(0x07000004)
#define OpUTCh_CharLogOutRequest				(0x07000005)

extern SNtlPacketName* g_pNtlPacketName_UTCh[];

#pragma pack(1)

struct SUTCh_Nop
{
	SPacketHeader	header;
};

struct SUTCh_CharLoadRequest
{
	SPacketHeader	header;
	Byte            byUUID[NTL_MAX_LENGTH_OF_UUID];
};

struct SUTCh_CharCreateRequest
{
	SPacketHeader	header;
	WChar			wchCharName[NTL_MAX_LENGTH_OF_CHARACTER_NAME_UNICODE];
	UInt16			wCodePage;
	Byte			byRace;
	Byte			byClass;
	Byte			bySex;
};

struct SUTCh_CharDeleteRequest
{
	SPacketHeader	header;
	UInt32			uiCharID;
};

struct SUTCh_CharSelectRequest
{
	SPacketHeader	header;
	UInt32			uiCharID;
	UInt32			uiZoneID;
};

struct SUTCh_CharLogOutRequest
{
	SPacketHeader	header;
};

#pragma pack()