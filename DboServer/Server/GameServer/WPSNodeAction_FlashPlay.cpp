#include "stdafx.h"
#include "WPSNodeAction_FlashPlay.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlDirection.h"


CWPSNodeAction_FlashPlay::CWPSNodeAction_FlashPlay(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_ADD_FLASH_PLAY, lpszName)
{
	m_eDirectionType = DIRECTION_FLASH;
	m_directTblidx = INVALID_TBLIDX;
	m_bSynchronize = false;
	m_dwPlayTime = 0;
	m_dwWaitTime = 0;
}

CWPSNodeAction_FlashPlay::~CWPSNodeAction_FlashPlay()
{

}

CWPSNodeAction_FlashPlay::sPARAMETER_INFO* CWPSNodeAction_FlashPlay::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_FlashPlay::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "id") == 0) // direction link tblidx
	{
		m_directTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_FlashPlay: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_FlashPlay::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	printf("CWPSNodeAction_FlashPlay: [%s] not exist \n", name);
	return false;
}