//***********************************************************************************
//
//	File		:	SessionList.h
//
//	Begin		:	2007-01-02
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlLinkArray.h"
#include "NtlMutex.h"


class CNtlNetwork;
class CNtlSession;

class CNtlSessionList
{
public:

	CNtlSessionList();

	virtual ~CNtlSessionList();


public:

	int									Create(CNtlNetwork * pNetwork, int nSessionSize, int nExtraSize);

	void								Destroy();


public:

	bool								Add(CNtlSession * pSession);

	bool								Remove(CNtlSession * pSession);

	CNtlSession *						Find(HSESSION hSession);

	void								ValidCheck(DWORD dwTickTime);


public:

	int									GetCurCount();

	int									GetMaxCount();


protected:

	void								Init();


private:

	typedef CNtlLinkArray_Static<CNtlSession*> LIST;
	typedef LIST::POSITION LISTIT;

	LIST								m_sessionList;

	CNtlMutex							m_mutex;

	CNtlNetwork *							m_pNetworkRef;

};

