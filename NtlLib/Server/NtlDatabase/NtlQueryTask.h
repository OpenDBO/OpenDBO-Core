//***********************************************************************************
//
//	File		:	NtlQueryTask.h
//
//	Begin		:	2006-02-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#ifndef __NTLQUERYTASK_H__
#define __NTLQUERYTASK_H__

#include "NtlThreadPool.h"


class CNtlQuery;
class CNtlDatabaseConnection;
class CNtlQueryTask : public CNtlWorkItem
{
public:

	CNtlQueryTask(CNtlQuery * pQuery, CNtlDatabaseConnection * pConnection);

	virtual ~CNtlQueryTask();


public:

	void Run();


private:

	CNtlQuery *					m_pQuery;

	CNtlDatabaseConnection *	m_pConnection;
};

#endif // __NTLQUERYTASK_H__
