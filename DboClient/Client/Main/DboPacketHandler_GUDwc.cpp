/*****************************************************************************
* File			: DboPackethandler_GUDwc.cpp
* Abstract		: Dragon World Challenge, achievement and server script packet handles
*****************************************************************************
* Desc         : port of the tw 1.57.51 client handlers
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// shared
#include "NtlDWC.h"
#include "DwcTable.h"
#include "NPCTable.h"
#include "ObjectTable.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobManager.h"
#include "NtlSobGroup.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSobTriggerObjectProxy.h"

// dbo
#include "DboEvent.h"
#include "AlarmManager.h"
#include "MsgBoxManager.h"


void PacketHandler_GUDwcWorldCountInfoRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DWC_WORLDCOUNT_INFO_RES);

	sGU_DWC_WORLDCOUNT_INFO_RES* pResult = (sGU_DWC_WORLDCOUNT_INFO_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DWC_WORLDCOUNT_INFO_RES"), TRUE);
		Logic_CancelNpcFacing();
		return;
	}

	GetNtlSLGlobal()->GetDwcInfo()->byRemainCount = pResult->byLimitCount;

	CDboEventGenerator::DwcNotify(DWC_EVENT_WORLDCOUNT_INFO, pResult->hNpc, INVALID_TBLIDX, pResult->byLimitCount, FALSE, pResult);
}

void PacketHandler_GUDwcEnterRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DWC_ENTER_RES);

	sGU_DWC_ENTER_RES* pResult = (sGU_DWC_ENTER_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DWC_ENTER_RES"), TRUE);
}

void PacketHandler_GUDwcEnterProposalNfy(void* pPacket)
{
	sGU_DWC_ENTER_PROPOSAL_NFY* pResult = (sGU_DWC_ENTER_PROPOSAL_NFY*)pPacket;

	GetNtlSLGlobal()->GetDwcInfo()->byRemainCount = pResult->byDWCLimitCount;
	CDboEventGenerator::DwcNotify(DWC_EVENT_LIMIT_COUNT, INVALID_SERIAL_ID, pResult->dwcTblidx, pResult->byDWCLimitCount);

	// every party member answers, the box answers no when the wait time runs out
	GetMsgBoxManager()->DeleteMsgBox("DST_DWC_ENTER_CONFIRM");
	GetAlarmManager()->FormattedAlarmMessage("DST_DWC_ENTER_CONFIRM", FALSE, (RwReal)(pResult->dwWaitTime / 1000), NULL, pResult->byDWCLimitCount);
}

void PacketHandler_GUDwcEnterConfirmRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DWC_ENTER_CONFIRM_RES);

	sGU_DWC_ENTER_CONFIRM_RES* pResult = (sGU_DWC_ENTER_CONFIRM_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DWC_ENTER_CONFIRM_RES"), TRUE);
		return;
	}

	CDboEventGenerator::DwcNotify(DWC_EVENT_ENTER_CONFIRM, INVALID_SERIAL_ID, INVALID_TBLIDX, 0, pResult->bEnter);
}

void PacketHandler_GUDwcEnterCanceledNfy(void* pPacket)
{
	sGU_DWC_ENTER_CANCELED_NFY* pResult = (sGU_DWC_ENTER_CANCELED_NFY*)pPacket;

	GetMsgBoxManager()->DeleteMsgBox("DST_DWC_ENTER_CONFIRM");

	if (pResult->wResultCode != GAME_SUCCESS)
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DWC_ENTER_CANCELED_NFY"), TRUE);
}

// tw CNtlSobManager::DWCEnterNotify (0x473250): the statue next to the dwc npc (npc table contentsTblidx, a DWC_C_STATUE_001
// trigger object) goes to main state 1 and plays its animation 2 once, then loops animation 1 again
static void DwcPlayStatueDirection(SERIAL_HANDLE hNpc)
{
	CNtlSob* pSob = GetNtlSobManager()->GetSobObject(hNpc);
	if (pSob == NULL || pSob->GetClassID() != SLCLASS_NPC)
		return;

	sNPC_TBLDAT* pNpcTbldat = static_cast<CNtlSobNpcAttr*>(pSob->GetSobAttr())->GetNpcTbl();
	if (pNpcTbldat == NULL || pNpcTbldat->contentsTblidx == INVALID_TBLIDX)
		return;

	CNtlSobGroup* pGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_TRIGGER_OBJECT);
	if (pGroup == NULL)
		return;

	for (CNtlSobGroup::MapObject_Iter it = pGroup->Begin(); it != pGroup->End(); ++it)
	{
		CNtlSob* pObject = it->second;
		sOBJECT_TBLDAT* pObjectTbldat = static_cast<CNtlSobTriggerObjectAttr*>(pObject->GetSobAttr())->GetTriggerObjectTbl();
		if (pObjectTbldat == NULL || pObjectTbldat->tblidx != pNpcTbldat->contentsTblidx)
			continue;

		CNtlSobTriggerObjectProxy::ANIMATION_CHAIN listAnim;
		listAnim.push_back(2);
		listAnim.push_back(1);

		static_cast<CNtlSobTriggerObjectProxy*>(pObject->GetSobProxy())->PlayAnimChain(1, listAnim);
		return;
	}
}

void PacketHandler_GUDwcEnterDirectionNfy(void* pPacket)
{
	sGU_DWC_ENTER_DIRECTION_NFY* pResult = (sGU_DWC_ENTER_DIRECTION_NFY*)pPacket;

	GetMsgBoxManager()->DeleteMsgBox("DST_DWC_ENTER_CONFIRM");

	if (pResult->hNpc != INVALID_HOBJECT)
		DwcPlayStatueDirection(pResult->hNpc);

	CDboEventGenerator::DwcNotify(DWC_EVENT_ENTER_DIRECTION, pResult->hNpc, pResult->dwcTblidx);
}

void PacketHandler_GUDwcMissionRewardNfy(void* pPacket)
{
	sGU_DWC_MISSION_REWARD_NFY* pResult = (sGU_DWC_MISSION_REWARD_NFY*)pPacket;

	// the mission table index is 1-based inside the scenario, find the slot of the mission
	SDwcInfo* pDwcInfo = GetNtlSLGlobal()->GetDwcInfo();
	sDWC_SCENARIO_INFO* pScenario = pDwcInfo->AddScenario(pResult->dwcTblidx);

	sDWC_TBLDAT* pDwcTbldat = (sDWC_TBLDAT*)API_GetTableContainer()->GetDwcTable()->FindData(pResult->dwcTblidx);
	if (pScenario && pDwcTbldat)
	{
		for (RwInt32 i = 0; i < MAX_DWC_MISSION_COUNT_PER_SCENARIO; ++i)
		{
			if (pDwcTbldat->aMissionTblidx[i] != pResult->dwcMissionTblidx)
				continue;

			pScenario->asMission[i].byRewardGrade = pResult->byRewardGrade;
			pScenario->asMission[i].wClearCount = pResult->wClearCount;
			break;
		}
	}

	CDboEventGenerator::DwcNotify(DWC_EVENT_SCENARIO_INFO, INVALID_SERIAL_ID, pResult->dwcTblidx);
	CDboEventGenerator::DwcNotify(DWC_EVENT_MISSION_REWARD, INVALID_SERIAL_ID, pResult->dwcTblidx, pResult->dwcMissionTblidx, pResult->bIsBasicReward, pResult);
}

void PacketHandler_GUDwcScenarioInfoRes(void* pPacket)
{
	sGU_DWC_SCENARIO_INFO_RES* pResult = (sGU_DWC_SCENARIO_INFO_RES*)pPacket;

	SDwcInfo* pDwcInfo = GetNtlSLGlobal()->GetDwcInfo();
	pDwcInfo->byScenarioCount = 0;

	for (RwUInt8 i = 0; i < pResult->byScenarioCount && i < MAX_DWC_COUNT; ++i)
	{
		if (sDWC_SCENARIO_INFO* pScenario = pDwcInfo->AddScenario(pResult->asScenarioInfo[i].dwcTblidx))
			memcpy(pScenario, &pResult->asScenarioInfo[i], sizeof(sDWC_SCENARIO_INFO));
	}

	CDboEventGenerator::DwcNotify(DWC_EVENT_SCENARIO_INFO, INVALID_SERIAL_ID, INVALID_TBLIDX, 0, TRUE);
}

void PacketHandler_GUDwcScenarioUpdateNfy(void* pPacket)
{
	sGU_DWC_SCENARIO_UPDATE_NFY* pResult = (sGU_DWC_SCENARIO_UPDATE_NFY*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS || pResult->byMissionIndex >= MAX_DWC_MISSION_COUNT_PER_SCENARIO)
		return;

	if (sDWC_SCENARIO_INFO* pScenario = GetNtlSLGlobal()->GetDwcInfo()->AddScenario(pResult->dwcTblidx))
	{
		pScenario->asMission[pResult->byMissionIndex].byRewardGrade = pResult->byRewardGrade;
		pScenario->asMission[pResult->byMissionIndex].wClearCount = pResult->wClearCount;
	}

	CDboEventGenerator::DwcNotify(DWC_EVENT_SCENARIO_INFO, INVALID_SERIAL_ID, pResult->dwcTblidx);
}

void PacketHandler_GUDwcLimitCountNfy(void* pPacket)
{
	sGU_DWC_LIMIT_COUNT_NFY* pResult = (sGU_DWC_LIMIT_COUNT_NFY*)pPacket;

	GetNtlSLGlobal()->GetDwcInfo()->byRemainCount = pResult->byDWCLimitCount;
	CDboEventGenerator::DwcNotify(DWC_EVENT_LIMIT_COUNT, INVALID_SERIAL_ID, INVALID_TBLIDX, pResult->byDWCLimitCount);
}

void PacketHandler_GUDwcRewardList(void* pPacket)
{
	// rewards are given when a mission is cleared, nothing is listed
	UNREFERENCED_PARAMETER(pPacket);
}

void PacketHandler_GUDwcGetRewardRes(void* pPacket)
{
	UNREFERENCED_PARAMETER(pPacket);
}

void PacketHandler_GUAchievementsCompleteInfo(void* pPacket)
{
	sGU_ACHIEVEMENTS_COMPLETE_INFO* pResult = (sGU_ACHIEVEMENTS_COMPLETE_INFO*)pPacket;

	// the dwc admission conditions all fit in the first chunk
	if (pResult->byIdx != 0)
		return;

	SDwcInfo* pDwcInfo = GetNtlSLGlobal()->GetDwcInfo();
	memcpy(pDwcInfo->abyAchievementComplete, pResult->abyAchievementsCompleteInfo, sizeof(pDwcInfo->abyAchievementComplete));

	// a new avatar, the card progress of the last one is gone
	pDwcInfo->byScenarioCount = 0;

	CDboEventGenerator::DwcNotify(DWC_EVENT_ACHIEVEMENT);
}

void PacketHandler_GUAchievementCompleteNfy(void* pPacket)
{
	sGU_ACHIEVEMENT_COMPLETE_NFY* pResult = (sGU_ACHIEVEMENT_COMPLETE_NFY*)pPacket;

	GetNtlSLGlobal()->GetDwcInfo()->SetAchievementComplete(pResult->nIdxAchievement);

	CDboEventGenerator::DwcNotify(DWC_EVENT_ACHIEVEMENT, INVALID_SERIAL_ID, INVALID_TBLIDX, pResult->nIdxAchievement);
}

void PacketHandler_GUServerScriptCountdownNfy(void* pPacket)
{
	sGU_SERVERSCRIPT_COUNTDOWN_NFY* pResult = (sGU_SERVERSCRIPT_COUNTDOWN_NFY*)pPacket;

	// tw 0xE1F640: the time is already in seconds, g_EventCounterDown(bStart, (float)seconds)
	CDboEventGenerator::DwcNotify(DWC_EVENT_COUNTDOWN, INVALID_SERIAL_ID, INVALID_TBLIDX, pResult->dwCountdownTime, pResult->bStart);
}

void PacketHandler_GUServerScriptEventStatusRegisterNfy(void* pPacket)
{
	sGU_SERVERSCRIPT_EVENTSTATUS_REGISTER_NFY* pResult = (sGU_SERVERSCRIPT_EVENTSTATUS_REGISTER_NFY*)pPacket;
	sSERVERSCRIPT_EVENTSTATUS_INFO& rInfo = pResult->sEventStatusInfo;

	LIST_SERVERSCRIPT_EVENTSTATUS* pList = GetNtlSLGlobal()->GetServerScriptEventStatus();

	SServerScriptEventStatus* pStatus = NULL;
	for (LIST_SERVERSCRIPT_EVENTSTATUS::iterator it = pList->begin(); it != pList->end(); ++it)
	{
		if (it->byGroup == rInfo.byGroup && it->byIndex == rInfo.byIndex)
		{
			pStatus = &(*it);
			break;
		}
	}

	if (pStatus == NULL)
	{
		pList->push_back(SServerScriptEventStatus());
		pStatus = &pList->back();
	}

	pStatus->byGroup = rInfo.byGroup;
	pStatus->byIndex = rInfo.byIndex;
	pStatus->textTblidx = rInfo.textTblidx;
	pStatus->byType = rInfo.byType;
	pStatus->wCount = 0;
	pStatus->bSelected = FALSE;

	if (rInfo.byType == SERVERSCRIPT_EVENTSTATUS_MOBKILL_COUNT)
		pStatus->wMaxCount = rInfo.sMobKillCount.wMaxKillCount;
	else if (rInfo.byType == SERVERSCRIPT_EVENTSTATUS_SCRIPTEVENT_COUNT)
		pStatus->wMaxCount = rInfo.sScriptEventCount.wMaxCount;
	else
		pStatus->wMaxCount = 0;

	CDboEventGenerator::DwcNotify(DWC_EVENT_SCRIPT_STATUS);
}

void PacketHandler_GUServerScriptEventStatusUpdateNfy(void* pPacket)
{
	sGU_SERVERSCRIPT_EVENTSTATUS_UPDATE_NFY* pResult = (sGU_SERVERSCRIPT_EVENTSTATUS_UPDATE_NFY*)pPacket;

	LIST_SERVERSCRIPT_EVENTSTATUS* pList = GetNtlSLGlobal()->GetServerScriptEventStatus();
	for (LIST_SERVERSCRIPT_EVENTSTATUS::iterator it = pList->begin(); it != pList->end(); ++it)
	{
		if (it->byGroup == pResult->byGroup && it->byIndex == pResult->byIndex)
			it->wCount = pResult->wCount;
	}

	CDboEventGenerator::DwcNotify(DWC_EVENT_SCRIPT_STATUS);
}

void PacketHandler_GUServerScriptEventStatusSelectNfy(void* pPacket)
{
	sGU_SERVERSCRIPT_EVENTSTATUS_SELECT_NFY* pResult = (sGU_SERVERSCRIPT_EVENTSTATUS_SELECT_NFY*)pPacket;

	LIST_SERVERSCRIPT_EVENTSTATUS* pList = GetNtlSLGlobal()->GetServerScriptEventStatus();
	for (LIST_SERVERSCRIPT_EVENTSTATUS::iterator it = pList->begin(); it != pList->end(); ++it)
	{
		if (it->byGroup == pResult->byGroup && it->byIndex == pResult->byIndex)
			it->bSelected = TRUE;
	}

	CDboEventGenerator::DwcNotify(DWC_EVENT_SCRIPT_STATUS);
}

void PacketHandler_GUServerScriptEventStatusClearNfy(void* pPacket)
{
	sGU_SERVERSCRIPT_EVENTSTATUS_CLEAR_NFY* pResult = (sGU_SERVERSCRIPT_EVENTSTATUS_CLEAR_NFY*)pPacket;

	LIST_SERVERSCRIPT_EVENTSTATUS* pList = GetNtlSLGlobal()->GetServerScriptEventStatus();
	for (LIST_SERVERSCRIPT_EVENTSTATUS::iterator it = pList->begin(); it != pList->end(); )
	{
		if (it->byGroup == pResult->byGroup)
			it = pList->erase(it);
		else
			++it;
	}

	CDboEventGenerator::DwcNotify(DWC_EVENT_SCRIPT_STATUS);
}
