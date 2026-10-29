#ifndef __SERVER_WPSNODE_ACTION_DWC__
#define __SERVER_WPSNODE_ACTION_DWC__


#include "ControlScriptNodeAction.h"


class CWpsNodeAction_DWC : public CControlScriptNodeAction
{

public:
	CWpsNodeAction_DWC(const char* lpszName);
	virtual ~CWpsNodeAction_DWC();


public:

	virtual bool				CheckParameter() { return true; }

};


#endif
