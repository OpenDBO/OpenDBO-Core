#ifndef __SERVER_WPSNODE_ACTION_DWC_FAIL__
#define __SERVER_WPSNODE_ACTION_DWC_FAIL__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_DWC_fail : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_DWC_fail(const char* lpszName);
	virtual ~CWpsNodeAction_DWC_fail();


public:

	virtual bool				CheckParameter() { return true; }

};


#endif
