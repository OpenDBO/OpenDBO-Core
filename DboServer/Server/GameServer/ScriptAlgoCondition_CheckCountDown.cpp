#include "stdafx.h"
#include "ScriptAlgoCondition_CheckCountDown.h"
#include "WPSNodeCondition_CheckCountDown.h"
#include "WpsAlgoObject.h"
#include "NtlPacket.h"
#include "NtlPacketGU.h"


CWpsAlgoCondition_CheckCountDown::CWpsAlgoCondition_CheckCountDown(CWpsAlgoObject* pObject) :
CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_CHECK_COUNTDOWN, "SCRIPTCONTROL_CONDITION_CHECK_COUNTDOWN")
{
	m_dwWaitTime = 0;
	m_bStarted = false;
}


CWpsAlgoCondition_CheckCountDown::~CWpsAlgoCondition_CheckCountDown()
{
}


bool CWpsAlgoCondition_CheckCountDown::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeCondition_CheckCountDown* pCond = dynamic_cast<CWPSNodeCondition_CheckCountDown*>(pControlScriptNode);
	if (pCond)
	{
		m_dwWaitTime = pCond->m_dwWaitTime;

		return true;
	}

	return false;
}

int CWpsAlgoCondition_CheckCountDown::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	// the client shows the counter, the condition is met once it ran out
	if (m_bStarted == false)
	{
		m_bStarted = true;

		CNtlPacket packet(sizeof(sGU_SERVERSCRIPT_COUNTDOWN_NFY));
		sGU_SERVERSCRIPT_COUNTDOWN_NFY* res = (sGU_SERVERSCRIPT_COUNTDOWN_NFY*)packet.GetPacketData();
		res->wOpCode = GU_SERVERSCRIPT_COUNTDOWN_NFY;
		res->bStart = true;
		res->dwCountdownTime = m_dwWaitTime / 1000; // seconds
		packet.SetPacketLen(sizeof(sGU_SERVERSCRIPT_COUNTDOWN_NFY));
		GetOwner()->SendToPlayers(&packet);

		return m_status;
	}

	m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickDiff);
	if (m_dwWaitTime == 0)
	{
		CNtlPacket packet(sizeof(sGU_SERVERSCRIPT_COUNTDOWN_NFY));
		sGU_SERVERSCRIPT_COUNTDOWN_NFY* res = (sGU_SERVERSCRIPT_COUNTDOWN_NFY*)packet.GetPacketData();
		res->wOpCode = GU_SERVERSCRIPT_COUNTDOWN_NFY;
		res->bStart = false;
		res->dwCountdownTime = 0;
		packet.SetPacketLen(sizeof(sGU_SERVERSCRIPT_COUNTDOWN_NFY));
		GetOwner()->SendToPlayers(&packet);

		m_status = COMPLETED;
	}

	return m_status;
}
