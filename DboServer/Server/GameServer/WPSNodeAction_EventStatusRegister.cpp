#include "stdafx.h"
#include "WPSNodeAction_EventStatusRegister.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"


CWPSNodeAction_EventStatusRegister::CWPSNodeAction_EventStatusRegister(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_EVENTSTATUS_REGISTER, lpszName)
{
	memset(&m_sEventStatusData, 0, sizeof(m_sEventStatusData));
	m_sEventStatusData.textTblidx = INVALID_TBLIDX;
	m_sEventStatusData.wpsId = INVALID_TBLIDX;
	m_eType = SERVERSCRIPT_EVENTSTATUS_NORMAL;
	m_mobSpawnGroupId = INVALID_SPAWNGROUPID;
	m_eventId = INVALID_DWORD;
	m_wMaxCount = 0;
}

CWPSNodeAction_EventStatusRegister::~CWPSNodeAction_EventStatusRegister()
{

}

CWPSNodeAction_EventStatusRegister::sPARAMETER_INFO* CWPSNodeAction_EventStatusRegister::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_EventStatusRegister::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "group") == 0)
	{
		m_sEventStatusData.byGroup = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "index") == 0)
	{
		m_sEventStatusData.byIndex = (BYTE)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "text tblidx") == 0)
	{
		m_sEventStatusData.textTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "mob group") == 0)
	{
		m_mobSpawnGroupId = (SPAWNGROUPID)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "event id") == 0)
	{
		m_eventId = (DWORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}
	if (_stricmp(name, "max count") == 0)
	{
		m_wMaxCount = (WORD)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_EventStatusRegister: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_EventStatusRegister::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();
	const char* value = pNode->GetValue();

	if (_stricmp(name, "type") == 0)
	{
		if (_stricmp(value, "normal") == 0)
			m_eType = SERVERSCRIPT_EVENTSTATUS_NORMAL;
		else if (_stricmp(value, "mob count") == 0)
			m_eType = SERVERSCRIPT_EVENTSTATUS_MOBKILL_COUNT;
		else if (_stricmp(value, "event count") == 0)
			m_eType = SERVERSCRIPT_EVENTSTATUS_SCRIPTEVENT_COUNT;
		else
		{
			printf("CWPSNodeAction_EventStatusRegister: type [%s] not exist \n", value);
			return false;
		}

		m_sEventStatusData.eType = m_eType;
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_EventStatusRegister: [%s] not exist \n", name);
	return false;
}
