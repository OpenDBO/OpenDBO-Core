//***********************************************************************************
//
//	File		:	NtlQuery.h
//
//	Begin		:	2006-2-14
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Database Query Class
//
//***********************************************************************************

#ifndef __NTLQUERY_H__
#define __NTLQUERY_H__


class CNtlDatabaseConnection;
class CNtlQuery
{
public:

	CNtlQuery(void);

	virtual ~CNtlQuery(void);


public:


	virtual int					ExecuteQuery(CNtlDatabaseConnection * pConnection) = 0;

	virtual int					ExecuteResult() = 0;

};


#endif // __NTLQUERY_H__
