#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpUTG_Nop								(0x02000000)
#define OpUTG_SayMessage						(0x02000001)
#define OpUTG_MouseMovement						(0x02000002)
#define OpUTG_KeyboardMovement					(0x02000003)
#define OpUTG_EnterGame							(0x02000004)
#define OpUTG_LeaveGame							(0x02000005)
#define OpUTG_RequestNeighborSpawns				(0x02000006)
#define OpUTG_Heartbeat							(0x02000007)
#define OpUTG_RequestAttack						(0x02000008)
#define OpUTG_RequestEndAttack					(0x02000009)
//#define OpUTG_EquipItem							(0x0200000A)
//#define OpUTG_TakeOffItem						(0x0200000B)
//#define OpUTG_MoveItemInInventory				(0x0200000C)
#define OpUTG_UseItem							(0x0200000D)
#define OpUTG_RequestRegen						(0x0200000E)
#define OpUTG_MoveItem							(0x0200000F)
#define OpUTG_AttackFollow						(0x02000010)
#define OpUTG_RequestJump						(0x02000011)
#define OpUTG_RequestJumpEnd					(0x02000012)
#define OpUTG_RequestKamehameha					(0x02000013)
#define OpUTG_RequestSitDown					(0x02000014)
#define OpUTG_RequestStandUp					(0x02000015)		

extern SNtlPacketName* g_pNtlPacketName_UTG[];

#pragma pack(1)

struct SUTG_Nop
{
	SPacketHeader	header;
};

struct SUTG_SayMessage
{
	SPacketHeader	header;
	UInt16			wMessageLengthInUnicode;
	Byte			byVariableField[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE * sizeof(WChar)];
};

struct SUTG_MouseMovement
{
	SPacketHeader	header;
	float			fDestinationX;
	float			fDestinationY;
	float			fDestinationZ;
	UInt32			uiTimeStamp;
};

struct SUTG_KeyboardMovement
{
	SPacketHeader	header;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fCurrentHeadingVectorX;
	float			fCurrentHeadingVectorZ;
	UInt16			wDirection;
	UInt32			uiTimeStamp;
};

struct SUTG_EnterGame
{
	SPacketHeader	header;
	UInt32			uiCharID;
	Byte			byUUID[NTL_MAX_LENGTH_OF_UUID];
};

struct SUTG_LeaveGame
{
	SPacketHeader	header;
};

struct SUTG_RequestNeighborSpawns
{
	SPacketHeader	header;
};

struct SUTG_Heartbeat
{
	SPacketHeader	header;
};

struct SUTG_RequestAttack
{
	SPacketHeader	header;
	UInt32			uiDefenderSpawnID;
};

struct SUTG_RequestEndAttack
{
	SPacketHeader	header;
};

struct SUTG_UseItem
{
	SPacketHeader	header;
	UInt32			uiItemInstanceId;
	UInt32			uiCapsuleIndex;
	UInt32			uiOrderInCapsule;
};

struct SUTG_RequestRegen
{
	SPacketHeader	header;
};

struct tagItemLocationInfo_UTG
{
	Byte			byIsEquipment;		// 0x00 : FALSE
										// 0x01 : TRUE
	union
	{
		struct tagEquipmentSlot
		{
			UInt32	uiSlot;
			UInt32	uiPadding;
		} equipmentSlot;
		struct tagItemLocation
		{
			UInt32	uiCapsuleIndex;
			UInt32	uiOrderInCapsule;
		} itemLocation;
	};
};

struct SUTG_MoveItem
{
	SPacketHeader	header;
	UInt32			uiItemInstanceId;
	tagItemLocationInfo_UTG		itemLocation;
	tagItemLocationInfo_UTG		itemDestination;
};


struct SUTG_AttackFollow
{
	SPacketHeader	header;
	UInt32			uiAttackerSpawnID;
	UInt32			uiDefenderSpawnID;
	UInt32			uiTimeStamp;
};

struct SUTG_RequestJump
{
	SPacketHeader	header;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fCurrentHeadingVectorX;
	float			fCurrentHeadingVectorZ;
	UInt16			wDirection;
	UInt32			uiTimeStamp;
};

struct SUTG_RequestJumpEnd
{
	SPacketHeader	header;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fCurrentHeadingVectorX;
	float			fCurrentHeadingVectorZ;
	UInt16			wDirection;
	UInt32			uiTimeStamp;
};

struct SUTG_RequestKamehameha
{
	SPacketHeader	header;
	UInt32			uiAttackerSpawnID;
	UInt32			uiDefenderSpawnID;
	float			fCurrentHeadingVectorX;
	float			fCurrentHeadingVectorZ;
	BYTE			byAttackType;
};

struct SUTG_RequestSitDown
{
	SPacketHeader	header;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fCurrentHeadingVectorX;
	float			fCurrentHeadingVectorZ;
};

struct SUTG_RequestStandUp
{
	SPacketHeader	header;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fCurrentHeadingVectorX;
	float			fCurrentHeadingVectorZ;
};

#pragma pack()