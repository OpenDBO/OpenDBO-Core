//***********************************************************************************
//
//	File		:	NtlSqlUnitHelper.cpp
//
//	Begin		:	2006-02-22
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Sql unit Helper class Implementation
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlSqlUnitHelper.h"
#include "NtlSqlUnit.h"
#include "NtlDatabaseConnection.h"


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlSqlUnitHelper::Create(SQLUNITID maxSqlUnitID)
{
	m_maxSqlUnitID = maxSqlUnitID;

	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CNtlSqlUnitHelper::MakeSqlUnit(CNtlDatabaseConnection * pConnection, CNtlSqlUnit * pSqlUnit)
{
	if( NULL == pSqlUnit )
	{
		return FALSE;
	}


	if( FALSE == pSqlUnit->Prepare( pConnection->GetHDBC() ) ) 
	{
		SAFE_DELETE( pSqlUnit );
		return FALSE;
	}


	if( FALSE ==  pConnection->RegisterSqlUnit( pSqlUnit ) )
	{
		SAFE_DELETE( pSqlUnit );
		return FALSE;
	}


	return TRUE;
}