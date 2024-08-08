#include "stdafx.h"
#include "NtlPacketUTG.h"

SNtlPacketName* g_pNtlPacketName_UTG[] =
{
	new SNtlPacketName(OpUTG_Nop, "OpUTG_Nop"),
	new SNtlPacketName(OpUTG_SayMessage, "OpUTG_SayMessage"),
	new SNtlPacketName(OpUTG_MouseMovement, "OpUTG_MouseMovement"),
	new SNtlPacketName(OpUTG_KeyboardMovement, "OpUTG_KeyboardMovement"),
	new SNtlPacketName(OpUTG_EnterGame, "OpUTG_EnterGame"),
	new SNtlPacketName(OpUTG_LeaveGame, "OpUTG_LeaveGame"),
	new SNtlPacketName(OpUTG_RequestNeighborSpawns, "OpUTG_RequestNeighborSpawns"),
	new SNtlPacketName(OpUTG_Heartbeat, "OpUTG_Heartbeat"),
	new SNtlPacketName(OpUTG_RequestAttack, "OpUTG_RequestAttack"),
	new SNtlPacketName(OpUTG_RequestEndAttack, "OpUTG_RequestEndAttack"),
	new SNtlPacketName(OpUTG_UseItem, "OpUTG_UseItem"),
	new SNtlPacketName(OpUTG_RequestRegen, "OpUTG_RequestRegen"),
	new SNtlPacketName(OpUTG_MoveItem, "OpUTG_MoveItem"),
	new SNtlPacketName(OpUTG_AttackFollow, "OpUTG_AttackFollow"),
	new SNtlPacketName(OpUTG_RequestJump, "OpUTG_RequestJump"),
	new SNtlPacketName(OpUTG_RequestJumpEnd, "OpUTG_RequestJumpEnd"),
	new SNtlPacketName(OpUTG_RequestKamehameha, "OpUTG_RequestKamehameha"),
	new SNtlPacketName(OpUTG_RequestSitDown, "OpUTG_RequestSitDown"),
	new SNtlPacketName(OpUTG_RequestStandUp, "OpUTG_RequestStandUp"),
	NULL
};