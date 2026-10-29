#include "stdafx.h"
#include "WPSNodeAction_EventStatusSelect.h"
#include "ControlScriptNodeParam_Number.h"


CWPSNodeAction_EventStatusSelect::CWPSNodeAction_EventStatusSelect(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_EVENTSTATUS_SELECT, lpszName)
{
	m_byGroup = INVALID_BYTE;
	m_byIndex = INVALID_BYTE;
}

CWPSNodeAction_EventStatusSelect::~CWPSNodeAction_EventStatusSelect()
{

}

CWPSNodeAction_EventStatusSelect::sPARAMETER_INFO* CWPSNodeAction_EventStatusSelect::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_EventStatusSelect::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "group") == 0)
	{
		m_byGroup = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "index") == 0)
	{
		m_byIndex = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_EventStatusSelect: [%s] not exist \n", name);
	return false;
}
