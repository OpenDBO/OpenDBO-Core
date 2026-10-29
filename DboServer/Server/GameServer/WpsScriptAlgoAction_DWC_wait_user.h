#ifndef __SCRIPT_ALGO_DBOG_ACTION_DWC_WAIT_USER_H__
#define __SCRIPT_ALGO_DBOG_ACTION_DWC_WAIT_USER_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_DWC_wait_user : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_DWC_wait_user(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_DWC_wait_user();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif
