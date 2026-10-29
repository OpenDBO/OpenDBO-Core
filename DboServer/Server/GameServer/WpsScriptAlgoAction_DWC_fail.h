#ifndef __SCRIPT_ALGO_DBOG_ACTION_DWC_FAIL_H__
#define __SCRIPT_ALGO_DBOG_ACTION_DWC_FAIL_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_DWC_fail : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_DWC_fail(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_DWC_fail();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bDone;

};

#endif
