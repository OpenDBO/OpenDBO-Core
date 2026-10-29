#include "stdafx.h"
#include "WpsScriptAlgoAction_DWC_fail.h"
#include "DwcDungeon.h"


CWpsScriptAlgoAction_DWC_fail::CWpsScriptAlgoAction_DWC_fail(CWpsAlgoObject* pObject) :
	CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_DWC_FAIL, "SCRIPTCONTROL_ACTION_DWC_FAIL")
{
	m_bDone = false;
}


CWpsScriptAlgoAction_DWC_fail::~CWpsScriptAlgoAction_DWC_fail()
{
}

int CWpsScriptAlgoAction_DWC_fail::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	// the dungeon plays the fail flash and sends everyone home after the fail wait time.
	// The node never completes so the script does not go on while the members leave.
	if (m_bDone == false)
	{
		m_bDone = true;

		if (CDwcDungeon* pDwc = GetOwner()->GetDwcDungeon())
			pDwc->Fail();
	}

	return m_status;
}
