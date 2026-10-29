#ifndef __SERVER_WPSNODE_ACTION_DWC_REWARD__
#define __SERVER_WPSNODE_ACTION_DWC_REWARD__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_DWC_reward : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_DWC_reward(const char* lpszName);
	virtual ~CWpsNodeAction_DWC_reward();


public:

	virtual bool				CheckParameter() { return true; }

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

public:

	TBLIDX						m_dwcTblidx;
	BYTE						m_byMissionIndex; // index into sDWC_TBLDAT::aMissionTblidx, the script counts from 1

};


#endif
