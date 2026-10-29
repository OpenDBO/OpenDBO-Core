#include "stdafx.h"
#include "WpsScriptAlgoAction_DWC_leave.h"
#include "DwcDungeon.h"


CWpsScriptAlgoAction_DWC_leave::CWpsScriptAlgoAction_DWC_leave(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_DWC_LEAVE, "SCRIPTCONTROL_ACTION_DWC_LEAVE")
{
	m_bDone = false;
}


CWpsScriptAlgoAction_DWC_leave::~CWpsScriptAlgoAction_DWC_leave()
{
}

int CWpsScriptAlgoAction_DWC_leave::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	// the members are teleported back to where they entered. The dwc world and its scripts are destroyed
	// once the last member left, so the node never completes (same as CCBD leave).
	if (m_bDone == false)
	{
		m_bDone = true;

		if (CDwcDungeon* pDwc = GetOwner()->GetDwcDungeon())
			pDwc->Leave();
	}

	return m_status;
}
