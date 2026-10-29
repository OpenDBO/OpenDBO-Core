#include "stdafx.h"
#include "WPSNodeAction_EventStatusClear.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeAction_EventStatusClear::CWPSNodeAction_EventStatusClear(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_EVENTSTATUS_CLEAR, lpszName)
{
	m_byGroup = INVALID_BYTE;
}

CWPSNodeAction_EventStatusClear::~CWPSNodeAction_EventStatusClear()
{

}

CWPSNodeAction_EventStatusClear::sPARAMETER_INFO* CWPSNodeAction_EventStatusClear::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_EventStatusClear::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "group") == 0)
	{
		m_byGroup = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_EventStatusClear: [%s] not exist \n", name);
	return false;
}
