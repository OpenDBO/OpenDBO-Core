#include "stdafx.h"
#include "WpsScriptAlgoAction_DWC.h"


CWpsScriptAlgoAction_DWC::CWpsScriptAlgoAction_DWC(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_DWC, "SCRIPTCONTROL_ACTION_DWC")
{
}


CWpsScriptAlgoAction_DWC::~CWpsScriptAlgoAction_DWC()
{
}

int CWpsScriptAlgoAction_DWC::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	// marks the begin of the dwc script, the dungeon is set up before the script starts
	m_status = COMPLETED;
	return m_status;
}
