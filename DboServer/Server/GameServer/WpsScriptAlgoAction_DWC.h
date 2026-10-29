#ifndef __SCRIPT_ALGO_DBOG_ACTION_DWC_H__
#define __SCRIPT_ALGO_DBOG_ACTION_DWC_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_DWC : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_DWC(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_DWC();

public:

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

};

#endif
