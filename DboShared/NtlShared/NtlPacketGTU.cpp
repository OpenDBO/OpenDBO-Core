#include "stdafx.h"
#include "NtlPacketGTU.h"

SNtlPacketName* g_pNtlPacketName_GTU[] =
{
	new SNtlPacketName(OpGTU_Nop, "OpGTU_Nop"),
	new SNtlPacketName(OpGTU_Heartbeat, "OpGTU_Heartbeat"),
	new SNtlPacketName(OpGTU_Player, "OpGTU_Player"),
	new SNtlPacketName(OpGTU_SpawnMovement, "OpGTU_SpawnMovement"),
	new SNtlPacketName(OpGTU_EnterGameResult, "OpGTU_EnterGameResult"),
	new SNtlPacketName(OpGTU_AvatarInfo, "OpGTU_AvatarInfo"),
	new SNtlPacketName(OpGTU_Despawn, "OpGTU_Despawn"),
	new SNtlPacketName(OpGTU_PlayerLogout, "OpGTU_PlayerLogout"),
	new SNtlPacketName(OpGTU_LeaveGameResult, "OpGTU_LeaveGameResult"),
	new SNtlPacketName(OpGTU_SpawnMouseMovement, "OpGTU_SpawnMouseMovement"),
	new SNtlPacketName(OpGTU_Attack, "OpGTU_Attack"),
	new SNtlPacketName(OpGTU_SetAttackMode, "OpGTU_SetAttackMode"),
	new SNtlPacketName(OpGTU_UpdateEXP, "OpGTU_UpdateEXP"),
	new SNtlPacketName(OpGTU_UpdateLevel, "OpGTU_UpdateLevel"),
	new SNtlPacketName(OpGTU_ContentInCapsuleSlot, "OpGTU_ContentInCapsuleSlot"),
	new SNtlPacketName(OpGTU_NotifyAllAvatarInfoSent, "OpGTU_NotifyAllAvatarInfoSent"),
	new SNtlPacketName(OpGTU_ChangedEquipment, "OpGTU_ChangedEquipment"),
	new SNtlPacketName(OpGTU_Npc, "OpGTU_Npc"),
	new SNtlPacketName(OpGTU_Mob, "OpGTU_Mob"),
	new SNtlPacketName(OpGTU_SetRegenPlayer, "OpGTU_SetRegenPlayer"),
	new SNtlPacketName(OpGTU_ChangedHPMPCP, "OpGTU_ChangedHPMPCP"),
	new SNtlPacketName(OpGTU_UseItemResult, "OpGTU_UseItemResult"),
	new SNtlPacketName(OpGTU_PlayerDie, "OpGTU_PlayerDie"),
	new SNtlPacketName(OpGTU_MoveItemResult, "OpGTU_MoveItemResult"),
	new SNtlPacketName(OpGTU_AttackFollow, "OpGTU_AttackFollow"),
	new SNtlPacketName(OpGTU_MovementNone, "OpGTU_MovementNone"),
	new SNtlPacketName(OpGTU_Jump, "OpGTU_Jump"),
	new SNtlPacketName(OpGTU_JumpEnd, "OpGTU_JumpEnd"),
	new SNtlPacketName(OpGTU_Kamehameha, "OpGTU_Kamehameha"),
	new SNtlPacketName(OpGTU_SitDown, "OpGTU_SitDown"),
	new SNtlPacketName(OpGTU_StandUp, "OpGTU_StandUp"),
	new SNtlPacketName(OpGTU_SpawnStatus, "OpGTU_SpawnStatus"),

	NULL
};