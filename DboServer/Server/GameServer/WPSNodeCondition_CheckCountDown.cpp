#include "stdafx.h"
#include "WPSNodeCondition_CheckCountDown.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeCondition_CheckCountDown::CWPSNodeCondition_CheckCountDown(const char* lpszName)
:CControlScriptNodeCondition(WPS_NODE_CONDITION_CHECK_COUNTDOWN, lpszName)
{
	m_dwWaitTime = 0;
}

CWPSNodeCondition_CheckCountDown::~CWPSNodeCondition_CheckCountDown()
{

}

bool CWPSNodeCondition_CheckCountDown::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "time") == 0)
	{
		m_dwWaitTime = (DWORD)floor(1000.0 * pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeCondition_CheckCountDown: [%s] not exist \n", name);
	return false;
}
