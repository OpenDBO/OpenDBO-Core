#pragma once
#include "..\NtlNetworkLayerTypeDef.h"
#include "NtlPacketInternal.h"

#define OpGTU_Nop								(0x20000000)
#define OpGTU_Heartbeat							(0x20000001)
#define OpGTU_Player							(0x20000002)
#define OpGTU_SpawnMovement						(0x20000003)
#define OpGTU_EnterGameResult					(0x20000004)
#define OpGTU_AvatarInfo						(0x20000005)
#define OpGTU_Despawn							(0x20000006)
#define OpGTU_PlayerLogout						(0x20000007)
#define OpGTU_LeaveGameResult					(0x20000008)
#define OpGTU_SpawnMouseMovement				(0x20000009)
#define OpGTU_Attack							(0x2000000A)
#define OpGTU_SetAttackMode						(0x2000000B)
#define OpGTU_UpdateEXP							(0x2000000C)
#define OpGTU_UpdateLevel						(0x2000000D)
#define OpGTU_ContentInCapsuleSlot				(0x2000000E)
#define OpGTU_NotifyAllAvatarInfoSent			(0x2000000F)
#define OpGTU_ChangedEquipment					(0x20000010)
#define OpGTU_Npc								(0x20000011)
#define OpGTU_Mob								(0x20000012)
#define OpGTU_SetRegenPlayer					(0x20000013)
#define OpGTU_ChangedHPMPCP						(0x20000014)
#define OpGTU_UseItemResult						(0x20000015)
#define OpGTU_PlayerDie							(0x20000016)
#define OpGTU_MoveItemResult					(0x20000017)
#define OpGTU_AttackFollow						(0x20000018)
#define OpGTU_MovementNone						(0x20000019)
#define OpGTU_Jump								(0x2000001A)
#define OpGTU_JumpEnd							(0x2000001B)
#define OpGTU_Kamehameha						(0x2000001C)
#define OpGTU_SitDown							(0x2000001D)
#define OpGTU_StandUp							(0x2000001E)
#define OpGTU_SpawnStatus						(0x2000001F)

extern SNtlPacketName* g_pNtlPacketName_GTU[];

#pragma pack(1)

struct SGTU_Nop
{
	SPacketHeader	header;
};

struct SGTU_Heartbeat
{
	SPacketHeader	header;
	UInt32			uiTimeStamp;
};

struct SGTU_Player
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	UInt32			uiCharID;
	WChar			wchCharName[NTL_MAX_LENGTH_OF_CHARACTER_NAME_UNICODE];
	Byte			byRace;
	Byte			byClass;
	Byte			bySex;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
	Byte			byLevel;
	UInt32			uiEquipment[NTL_EQUIP_SLOT_TOTAL_NUMBER_OF_SLOT];
	BYTE			byMoveStatus;
	UInt32			uiCurrentHP;
	UInt32			uiCurrentMP;
	bool			bIsRegen;
	
};

struct SGTU_SpawnMovement
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
	UInt16			wMovementDirection;
	UInt32			uiTimeStamp;
};

struct SGTU_EnterGameResult
{
	SPacketHeader	header;
	UInt16			wLoginResult;
};

struct SGTU_AvatarInfo
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	WChar			wchCharName[NTL_MAX_LENGTH_OF_CHARACTER_NAME_UNICODE];
	Byte			byRace;
	Byte			byClass;
	Byte			bySex;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
	Byte			byLevel;
	UInt32			uiEquipment[NTL_EQUIP_SLOT_TOTAL_NUMBER_OF_SLOT];
	UInt32			uiEquipmentItemInstanceID[NTL_EQUIP_SLOT_TOTAL_NUMBER_OF_SLOT];
	UInt32			uiCurrentHP;
	UInt32			uiCurrentMP;
};

struct SGTU_Despawn
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
};

struct SGTU_PlayerLogout
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
};

struct SGTU_LeaveGameResult
{
	SPacketHeader	header;
	UInt16			wLeaveGameResult;
};

struct SGTU_SpawnMouseMovement
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	float			fDestinationX;
	float			fDestinationY;
	float			fDestinationZ;
	UInt32			uiTimeStamp;
};

struct SGTU_Attack
{
	SPacketHeader	header;
	UInt32			uiAttackerSpawnID;
	UInt32			uiDefenderSpawnID;
	UInt32			uiDamage;
	UInt32			uiCurrentHP;
	bool			bIsCritical; // true: Critical, false: Normal
};

struct SGTU_SetAttackMode
{
	SPacketHeader	header;
	UInt32			uiAttackerSpawnID;
	bool			bType; // true: Attack, false: None Attack
};

struct SGTU_UpdateEXP
{
	SPacketHeader	header;
	UInt32			uiCurrentEXP;
};

struct SGTU_UpdateLevel
{
	SPacketHeader	header;
	UInt32			uiLevel;
};

struct tagItem
{
	Byte			byOrderInCapsule;
	UInt32			uiItemId;
	UInt32			uiItemInstanceId;
	Byte			byNumberInStack;
};

struct SGTU_ContentInCapsuleSlot
{
	SPacketHeader	header;
	UInt32			uiCapsuleIndex;
	UInt32			uiNumberOfItems;
	tagItem			byContentInCapsule[NTL_MAX_NUMBER_OF_SLOT_IN_BAG + 1];
};

struct SGTU_NotifyAllAvatarInfoSent
{
	SPacketHeader	header;
};

struct SGTU_ChangedEquipment
{
	SPacketHeader	header;
	UInt32			uiSpawnId;
	UInt32			uiSlot1;
	UInt32			uiItemId1;
	UInt32			uiSlot2;
	UInt32			uiItemId2;
};

struct SGTU_Npc
{
	SPacketHeader	header;
	UInt32			uiNpcId;
	UInt32			uiSpawnId;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
	Byte			byLevel;
	UInt32			uiCurrentHP;
	UInt32			uiCurrentMP;
};

struct SGTU_Mob
{
	SPacketHeader	header;
	UInt32			uiMobId;
	UInt32			uiSpawnId;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
	Byte			byLevel;
	UInt32			uiCurrentHP;
	UInt32			uiCurrentMP;
};

struct SGTU_SetRegenPlayer
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	UInt32			uiCharID;
	Byte			byRace;
	Byte			byClass;
	Byte			bySex;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
	Byte			byLevel;
	UInt32			uiEquipment[NTL_EQUIP_SLOT_TOTAL_NUMBER_OF_SLOT];
	UInt32			uiEquipmentItemInstanceID[NTL_EQUIP_SLOT_TOTAL_NUMBER_OF_SLOT];
	UInt32			uiCurrentHP;
	UInt32			uiCurrentMP;
};

struct SGTU_ChangedHPMPCP
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	UInt32			uiCurrentHP;
	UInt32			uiCurrentMP;
	UInt32			uiCurrentCP;
};

struct SGTU_UseItemResult
{
	SPacketHeader	header;
	UInt32			uiItemInstanceId;
	UInt32			uiNumberInStack;
	UInt16			wResult;
};

struct SGTU_PlayerDie
{
	SPacketHeader	header;
};

struct tagItemLocationInfo_GTU
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

struct SGTU_MoveItemResult
{
	SPacketHeader	header;
	UInt32			uiItemInstanceId;
	tagItemLocationInfo_GTU		newLocation;
	UInt32			uiMovedItemInstanceId1;
	tagItemLocationInfo_GTU		movedItemLocation1;
	UInt32			uiMovedItemInstanceId2;
	tagItemLocationInfo_GTU		movedItemLocation2;
	UInt16			wResult;
};

struct SGTU_AttackFollow
{
	SPacketHeader	header;
	UInt32			uiAttackerSpawnID;
	UInt32			uiDefenderSpawnID;
	UInt32			uiTimeStamp;
};

struct SGTU_MovementNone
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
	UInt32			uiTimeStamp;
};

struct SGTU_Jump
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
	UInt16			wMovementDirection;
	UInt32			uiTimeStamp;
};

struct SGTU_JumpEnd
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
	UInt16			wMovementDirection;
	UInt32			uiTimeStamp;
};

struct SGTU_Kamehameha
{
	SPacketHeader	header;
	UInt32			uiAttackerSpawnID;
	UInt32			uiDefenderSpawnID;
	float			fCurrentHeadingVectorX;
	float			fCurrentHeadingVectorZ;
	BYTE			byAttackType;
	//UInt32			uiDamage;
	//UInt32			uiCurrentHP;
};

struct SGTU_SitDown
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
};

struct SGTU_StandUp
{
	SPacketHeader	header;
	UInt32			uiSpawnID;
	float			fCurrentPositionX;
	float			fCurrentPositionY;
	float			fCurrentPositionZ;
	float			fHeadingVectorX;
	float			fHeadingVectorZ;
};

struct SGTU_SpawnStatus
{
	SPacketHeader	header;
	UInt32			uiSpawnId;
	UInt32			uiStatus;
	UInt32			uiCurrentHp;
	UInt32			uiCurrentMp;
	UInt32			uiTimeStamp;
};

#pragma pack()