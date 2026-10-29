#include "stdafx.h"
#include "ScriptAlgoAction_EventStatusRegister.h"
#include "WPSNodeAction_EventStatusRegister.h"
#include "WpsAlgoObject.h"
#include "NtlPacket.h"
#include "NtlPacketGU.h"


CWpsAlgoAction_EventStatusRegister::CWpsAlgoAction_EventStatusRegister(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_EVENT_STATUS_REGISTER, "SCRIPTCONTROL_ACTION_EVENT_STATUS_REGISTER")
{
}


CWpsAlgoAction_EventStatusRegister::~CWpsAlgoAction_EventStatusRegister()
{
}


bool CWpsAlgoAction_EventStatusRegister::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_EventStatusRegister* pAction = dynamic_cast<CWPSNodeAction_EventStatusRegister*>(pControlScriptNode);
	if (pAction)
	{
		m_sEventStatusData = pAction->m_sEventStatusData;
		m_eType = pAction->m_eType;
		m_mobSpawnGroupId = pAction->m_mobSpawnGroupId;
		m_eventId = pAction->m_eventId;
		m_wMaxCount = pAction->m_wMaxCount;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_EventStatusClear", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_EventStatusRegister::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CNtlPacket packet(sizeof(sGU_SERVERSCRIPT_EVENTSTATUS_REGISTER_NFY));
	sGU_SERVERSCRIPT_EVENTSTATUS_REGISTER_NFY* res = (sGU_SERVERSCRIPT_EVENTSTATUS_REGISTER_NFY*)packet.GetPacketData();
	res->wOpCode = GU_SERVERSCRIPT_EVENTSTATUS_REGISTER_NFY;
	res->sEventStatusInfo.byGroup = m_sEventStatusData.byGroup;
	res->sEventStatusInfo.byIndex = m_sEventStatusData.byIndex;
	res->sEventStatusInfo.textTblidx = m_sEventStatusData.textTblidx;
	res->sEventStatusInfo.byType = (BYTE)m_eType;

	if (m_eType == SERVERSCRIPT_EVENTSTATUS_MOBKILL_COUNT)
	{
		res->sEventStatusInfo.sMobKillCount.spawnGroupId = m_mobSpawnGroupId;
		res->sEventStatusInfo.sMobKillCount.wMaxKillCount = m_wMaxCount;

		GetOwner()->RegisterEventStatusMobCount(m_sEventStatusData.byGroup, m_sEventStatusData.byIndex, m_mobSpawnGroupId, m_wMaxCount);
	}
	else if (m_eType == SERVERSCRIPT_EVENTSTATUS_SCRIPTEVENT_COUNT)
	{
		res->sEventStatusInfo.sScriptEventCount.eventId = m_eventId;
		res->sEventStatusInfo.sScriptEventCount.wMaxCount = m_wMaxCount;
	}

	packet.SetPacketLen(sizeof(sGU_SERVERSCRIPT_EVENTSTATUS_REGISTER_NFY));
	GetOwner()->SendToPlayers(&packet);

	m_status = COMPLETED;
	return m_status;
}
