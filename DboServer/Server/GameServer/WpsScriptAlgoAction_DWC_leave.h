#ifndef __SCRIPT_ALGO_DBOG_ACTION_DWC_LEAVE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_DWC_LEAVE_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_DWC_leave : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_DWC_leave(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_DWC_leave();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	bool						m_bDone;

};

#endif
