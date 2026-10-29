#include "stdafx.h"
#include "ScriptAlgoAction_EventStatusClear.h"
#include "WPSNodeAction_EventStatusClear.h"
#include "WpsAlgoObject.h"
#include "NtlPacket.h"
#include "NtlPacketGU.h"


CWpsAlgoAction_EventStatusClear::CWpsAlgoAction_EventStatusClear(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_EVENT_STATUS_CLEAR, "SCRIPTCONTROL_ACTION_EVENT_STATUS_CLEAR")
{
}


CWpsAlgoAction_EventStatusClear::~CWpsAlgoAction_EventStatusClear()
{
}


bool CWpsAlgoAction_EventStatusClear::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_EventStatusClear* pAction = dynamic_cast<CWPSNodeAction_EventStatusClear*>(pControlScriptNode);
	if (pAction)
	{
		m_byGroup = pAction->m_byGroup;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_EventStatusClear", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_EventStatusClear::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	GetOwner()->UnregisterEventStatusGroup(m_byGroup);

	CNtlPacket packet(sizeof(sGU_SERVERSCRIPT_EVENTSTATUS_CLEAR_NFY));
	sGU_SERVERSCRIPT_EVENTSTATUS_CLEAR_NFY* res = (sGU_SERVERSCRIPT_EVENTSTATUS_CLEAR_NFY*)packet.GetPacketData();
	res->wOpCode = GU_SERVERSCRIPT_EVENTSTATUS_CLEAR_NFY;
	res->byGroup = m_byGroup;
	packet.SetPacketLen(sizeof(sGU_SERVERSCRIPT_EVENTSTATUS_CLEAR_NFY));
	GetOwner()->SendToPlayers(&packet);

	m_status = COMPLETED;
	return m_status;
}
