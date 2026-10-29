/*****************************************************************************
* File			: DboPackethandler_GUEventReward.cpp
* Abstract		: event reward packet handles of the event manager npc
*****************************************************************************
* Desc         : port of the tw client handlers
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// simulation
#include "NtlSLPacketGenerator.h"

// dbo
#include "DboEvent.h"
#include "AlarmManager.h"


void PacketHandler_GUEventRewardLoadInfo(void* pPacket)
{
	// tw: the rewards come first, the load answer completes the list
	CDboEventGenerator::NpcReward(NPC_REWARD_EVENT_LOAD_INFO, pPacket);
}

void PacketHandler_GUEventRewardLoadRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_EVENT_REWARD_LOAD_RES);

	sGU_EVENT_REWARD_LOAD_RES* pResult = (sGU_EVENT_REWARD_LOAD_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_EVENT_REWARD_LOAD_RES"), TRUE);
		return;
	}

	CDboEventGenerator::NpcReward(NPC_REWARD_EVENT_LOAD, pResult);
}

void PacketHandler_GUEventRewardSelectRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_EVENT_REWARD_SELECT_RES);

	sGU_EVENT_REWARD_SELECT_RES* pResult = (sGU_EVENT_REWARD_SELECT_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_EVENT_REWARD_SELECT_RES"), TRUE);
		return;
	}

	CDboEventGenerator::NpcReward(NPC_REWARD_EVENT_SELECT, pResult, pResult->eventTblidx);
}
