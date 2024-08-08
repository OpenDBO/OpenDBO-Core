//***********************************************************************************
//
//	File		:	NtlSqlUnitMap.cpp
//
//	Begin		:	2006-02-22
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Ntl Sql Unit Map class
//
//***********************************************************************************

#include "stdafx.h"
#include "Utils.h"
#include "NtlSqlUnitMap.h"
#include "NtlLog.h"

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSqlUnitMap::CNtlSqlUnitMap()
:
m_maxSqlUnitID( 0 ),
m_ppSqlUnitMap( NULL )
{
	
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSqlUnitMap::~CNtlSqlUnitMap()
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CNtlSqlUnitMap::Create(SQLUNITID maxSqlUnitID)
{
	if( m_ppSqlUnitMap )
	{
		return FALSE;
	}
	

	m_ppSqlUnitMap = new CNtlSqlUnit*[maxSqlUnitID];
	if( NULL == m_ppSqlUnitMap )
	{
		return FALSE;
	}


	ZeroMemory(m_ppSqlUnitMap, sizeof(CNtlSqlUnit*) * maxSqlUnitID );


	m_maxSqlUnitID = maxSqlUnitID;


	return TRUE;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSqlUnitMap::Destroy()
{
	SAFE_DELETE_ARRAY( m_ppSqlUnitMap );
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlSqlUnit * CNtlSqlUnitMap::Find(SQLUNITID sqlUnitID)
{
	if( sqlUnitID >= m_maxSqlUnitID )
	{
		return NULL;
	}

	if (NULL == m_ppSqlUnitMap)
	{
		NTL_LOG_ASSERT("NULL == m_ppSqlUnitMap");
		return NULL;
	}

	return m_ppSqlUnitMap[ sqlUnitID ];
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
BOOL CNtlSqlUnitMap::Add(CNtlSqlUnit * pSqlUnit)
{
	if (NULL == check_cast<CNtlSqlUnit*>(pSqlUnit))
	{
		NTL_LOG_ASSERT("\"pSqlUnit\" is not CNtlSqlUnit type., pSqlUnit = %016x", pSqlUnit);
		return FALSE;
	}

	if (NULL == m_ppSqlUnitMap)
	{
		NTL_LOG_ASSERT("NULL == m_ppSqlUnitMap");
		return FALSE;
	}

	if( pSqlUnit->GetSqlUnitID() >= m_maxSqlUnitID )
	{
		return FALSE;
	}

	m_ppSqlUnitMap[ pSqlUnit->GetSqlUnitID() ] = pSqlUnit;

	return TRUE;
}

 
//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlSqlUnitMap::Remove(CNtlSqlUnit * pSqlUnit)
{
	if (NULL == check_cast<CNtlSqlUnit*>(pSqlUnit))
	{
		NTL_LOG_ASSERT("\"pSqlUnit\" is not CNtlSqlUnit type., pSqlUnit = %016x", pSqlUnit);
		return;
	}

	if (NULL == m_ppSqlUnitMap)
	{
		NTL_LOG_ASSERT("NULL == m_ppSqlUnitMap");
		return;
	}

	if( pSqlUnit->GetSqlUnitID() >= m_maxSqlUnitID )
	{
		return;
	}

	m_ppSqlUnitMap[ pSqlUnit->GetSqlUnitID() ] = NULL;
}
