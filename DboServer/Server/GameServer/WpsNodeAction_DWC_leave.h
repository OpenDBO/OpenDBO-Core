#ifndef __SERVER_WPSNODE_ACTION_DWC_LEAVE__
#define __SERVER_WPSNODE_ACTION_DWC_LEAVE__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_DWC_leave : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_DWC_leave(const char* lpszName);
	virtual ~CWpsNodeAction_DWC_leave();


public:

	virtual bool				CheckParameter() { return true; }

};


#endif
