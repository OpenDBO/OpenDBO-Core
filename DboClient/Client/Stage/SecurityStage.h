/*****************************************************************************
 *
 * File			: SecurityStage.h
 * Author		: Hong Sungbock
 * Copyright	: (аж)NTL
 * Date			: 2009. 2. 26
 * Abstract		: Security stage class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __SECURITY_STAGE_H__
#define __SECURITY_STAGE_H__

#include "NtlStage.h"
#include "ceventhandler.h"
#include "eventtimer.h"


class CSecurityStage : public CNtlStage, public RWS::CEventHandler
{	
public:
	CSecurityStage(const char *pStageName);
	~CSecurityStage();

	virtual bool			Create();
	virtual void			Destroy();

	virtual void			HandleEvents(RWS::CMsg &pMsg);

protected:
};

#endif