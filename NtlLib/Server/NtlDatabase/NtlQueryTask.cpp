//***********************************************************************************
//
//	File		:	NtlQueryTask.cpp
//
//	Begin		:	2006-02-17
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Database Query Task Class Implementation
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlQueryTask.h"
#include "NtlQuery.h"
#include "NtlDatabase.h"
#include "NtlDatabaseConnection.h"

#include "NtlLog.h"

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlQueryTask::CNtlQueryTask(CNtlQuery * pQuery, CNtlDatabaseConnection * pConnection)
:
m_pQuery( pQuery ),
m_pConnection( pConnection )
{

}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlQueryTask::~CNtlQueryTask()
{
	SAFE_DELETE( m_pQuery );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlQueryTask::Run()
{
	int rc = m_pQuery->ExecuteQuery( m_pConnection );

	if( NTL_SUCCESS != rc )
	{
		if( false == m_pConnection->IsAlive() )
		{
			m_pConnection->Reconnect();
		}
	}


	CNtlDatabase * pDatabase = m_pConnection->GetParent();
	if( NULL == pDatabase )
	{
		NTL_LOG_ASSERT("NULL == pDatabase");
		return;
	}


	pDatabase->PushConnection( m_pConnection );


	m_pQuery->ExecuteResult();
}