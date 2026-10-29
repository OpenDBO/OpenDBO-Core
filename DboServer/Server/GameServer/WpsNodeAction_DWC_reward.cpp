#include "stdafx.h"
#include "WpsNodeAction_DWC_reward.h"
#include "ControlScriptNodeParam_Number.h"



CWpsNodeAction_DWC_reward::CWpsNodeAction_DWC_reward(const char* lpszName)
	:CControlScriptNodeAction(WPS_NODE_ACTION_DWC_REWARD, lpszName)
{
	m_dwcTblidx = INVALID_TBLIDX;
	m_byMissionIndex = INVALID_BYTE;
}

CWpsNodeAction_DWC_reward::~CWpsNodeAction_DWC_reward()
{

}



bool CWpsNodeAction_DWC_reward::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "dwc tblidx") == 0)
	{
		m_dwcTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "mission index") == 0)
	{
		BYTE byMission = (BYTE)pNode->GetValue();
		m_byMissionIndex = byMission > 0 ? byMission - 1 : INVALID_BYTE;
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWpsNodeAction_DWC_reward: [%s] not exist \n", name);
	return false;
}
