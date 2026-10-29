#ifndef __SCRIPT_ALGO_DBOG_ACTION_DWC_REWARD_H__
#define __SCRIPT_ALGO_DBOG_ACTION_DWC_REWARD_H__

#include "ScriptAlgoAction.h"


class CWpsScriptAlgoAction_DWC_reward : public CScriptAlgoAction
{

public:

	CWpsScriptAlgoAction_DWC_reward(CWpsAlgoObject* pObject);
	virtual	~CWpsScriptAlgoAction_DWC_reward();

public:

	virtual bool				AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int					OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	TBLIDX						m_dwcTblidx;
	BYTE						m_byMissionIndex;

};

#endif
