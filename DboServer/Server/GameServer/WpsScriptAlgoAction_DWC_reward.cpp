#include "stdafx.h"
#include "WpsScriptAlgoAction_DWC_reward.h"
#include "WpsNodeAction_DWC_reward.h"
#include "CPlayer.h"
#include "DwcDungeon.h"


CWpsScriptAlgoAction_DWC_reward::CWpsScriptAlgoAction_DWC_reward(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_DWC_REWARD, "SCRIPTCONTROL_ACTION_DWC_REWARD")
{
	m_dwcTblidx = INVALID_TBLIDX;
	m_byMissionIndex = INVALID_BYTE;
}


CWpsScriptAlgoAction_DWC_reward::~CWpsScriptAlgoAction_DWC_reward()
{
}


bool CWpsScriptAlgoAction_DWC_reward::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWpsNodeAction_DWC_reward* pAction = dynamic_cast<CWpsNodeAction_DWC_reward*>(pControlScriptNode);
	if (pAction)
	{
		m_dwcTblidx = pAction->m_dwcTblidx;
		m_byMissionIndex = pAction->m_byMissionIndex;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWpsNodeAction_DWC_reward", pControlScriptNode);
	return false;
}

int CWpsScriptAlgoAction_DWC_reward::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CDwcDungeon* pDwc = GetOwner()->GetDwcDungeon();

	if (pDwc && pDwc->GetState() == DWC_DUNGEON_STATE_PLAY) // no reward once the dwc failed
	{
		CPlayer* pPlayer = GetOwner()->GetPlayersFirst();
		while (pPlayer)
		{
			if (pPlayer->GetDWC() == pDwc && pPlayer->GetWorldID() == GetOwner()->GetWorld()->GetID())
				pPlayer->RewardDwcMission(m_dwcTblidx, m_byMissionIndex);

			pPlayer = GetOwner()->GetPlayersNext();
		}
	}

	m_status = COMPLETED;
	return m_status;
}
