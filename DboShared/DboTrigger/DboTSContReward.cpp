#include "precomp_trigger.h"
#include "DboTSContReward.h"
#include "NtlTSLog.h"
#include "QuestRewardTable.h"


/**
	Reward container
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSContReward, CDboTSContGAct )


CDboTSContReward::CDboTSContReward( void )
: m_uiDesc( 0xffffffff ),
  m_tcNextLinkId( NTL_TS_TC_ID_INVALID ),
  m_tcCancelLinkId( NTL_TS_TC_ID_INVALID )
{
	m_eRwdContType = eREWARD_CONTAINER_TYPE_QUEST;
	m_uiLimitTime = 0xffffffff;

	m_bUseTable = false;
	m_bLoadTable = false;

	m_uiRewardTblIdx = 0xffffffff;

	memset( m_asDefReward, 0xff, sizeof(m_asDefReward) );
	memset( m_asSelReward, 0xff, sizeof(m_asSelReward) );
}

bool CDboTSContReward::AttachNextLink( NTL_TS_TC_ID tcId )
{
	m_tcNextLinkId = tcId;
	return true;
}

void CDboTSContReward::DetachNextLink( void )
{
	m_tcNextLinkId = NTL_TS_TC_ID_INVALID;
}

bool CDboTSContReward::AttachCancelLink( NTL_TS_TC_ID tcId )
{
	m_tcCancelLinkId = tcId;
	return true;
}

void CDboTSContReward::DetachCancelLink( void )
{
	m_tcCancelLinkId = NTL_TS_TC_ID_INVALID;
}

void CDboTSContReward::SetRewardContType( eREWARD_CONTAINER_TYPE eRwdContType )
{
	m_eRwdContType = eRwdContType;
}

void CDboTSContReward::SetLimitTime( unsigned int uiLimitTime )
{
	m_uiLimitTime = uiLimitTime;
}

void CDboTSContReward::SetDesc( unsigned int uiDesc )
{
	m_uiDesc = uiDesc;
}

const sREWARD_INFO& CDboTSContReward::GetDefRewardInfo( int nIdx, CQuestRewardTable* pRewardTbl )
{
	if ( IsUseTable() && !m_bLoadTable )
	{
		m_bLoadTable = true;
		LoadTable( pRewardTbl );
	}

	return m_asDefReward[nIdx];
}

const sREWARD_INFO& CDboTSContReward::GetSelRewardInfo( int nIdx, CQuestRewardTable* pRewardTbl )
{
	if ( IsUseTable() && !m_bLoadTable )
	{
		m_bLoadTable = true;
		LoadTable( pRewardTbl );
	}

	return m_asSelReward[nIdx];
}

void CDboTSContReward::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "nextlnk" ) )
	{
		AttachNextLink((NTL_TS_TC_ID)clProperty.GetValueAsInt( "nextlnk" ) );
	}

	if ( clProperty.IsExist( "canclnk" ) )
	{
		AttachCancelLink((NTL_TS_TC_ID)clProperty.GetValueAsInt( "canclnk" ) );
	}

	if ( clProperty.IsExist( "rwdtype" ) )
	{
		SetRewardContType( (eREWARD_CONTAINER_TYPE)clProperty.GetValueAsInt( "rwdtype" ) );
	}

	if ( clProperty.IsExist( "ltime" ) )
	{
		SetLimitTime( clProperty.GetValueAsInt( "ltime" ) );
	}

	if ( clProperty.IsExist( "desc" ) )
	{
		SetDesc( clProperty.GetValueAsInt( "desc" ) );
	}

	if ( clProperty.IsExist( "usetbl" ) )
	{
		m_bUseTable = clProperty.GetValueAsBool( "usetbl" ) ? true : false;
	}

	if ( clProperty.IsExist( "rwdtbl" ) )
	{
		m_uiRewardTblIdx = clProperty.GetValueAsInt( "rwdtbl" );
	}

	if ( !IsUseTable() )
	{
		int i = 0;
		sREWARD_INFO stInfo;

		// 기본 보상
		for ( i = 0; i < 4; ++i )
		{
			stInfo.m_eType = eREWARD_TYPE_INVALID;
			stInfo.m_uiIdx = 0xffffffff;
			stInfo.m_nValue = 0xffffffff;

			sprintf_s( g_NtlTSString, "dtype%d", i );
			if ( clProperty.IsExist( g_NtlTSString ) )
			{
				stInfo.m_eType = (eREWARD_TYPE)clProperty.GetValueAsInt( g_NtlTSString );
			}

			sprintf_s( g_NtlTSString, "didx%d", i );
			if ( clProperty.IsExist( g_NtlTSString ) )
			{
				stInfo.m_uiIdx = clProperty.GetValueAsInt( g_NtlTSString );
			}

			sprintf_s( g_NtlTSString, "dval%d", i );
			if ( clProperty.IsExist( g_NtlTSString ) )
			{
				stInfo.m_nValue = clProperty.GetValueAsInt( g_NtlTSString );
			}

			m_asDefReward[i] = stInfo;
		}

		// 선택 보상
		for ( i = 0; i < 4; ++i )
		{
			stInfo.m_eType = eREWARD_TYPE_INVALID;
			stInfo.m_uiIdx = 0xffffffff;
			stInfo.m_nValue = 0xffffffff;

			sprintf_s( g_NtlTSString, "stype%d", i );
			if ( clProperty.IsExist( g_NtlTSString ) )
			{
				stInfo.m_eType = (eREWARD_TYPE)clProperty.GetValueAsInt( g_NtlTSString );
			}

			sprintf_s( g_NtlTSString, "sidx%d", i );
			if ( clProperty.IsExist( g_NtlTSString ) )
			{
				stInfo.m_uiIdx = clProperty.GetValueAsInt( g_NtlTSString );
			}

			sprintf_s( g_NtlTSString, "sval%d", i );
			if ( clProperty.IsExist( g_NtlTSString ) )
			{
				stInfo.m_nValue = clProperty.GetValueAsInt( g_NtlTSString );
			}

			m_asSelReward[i] = stInfo;
		}
	}
}

void CDboTSContReward::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCont::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetNextLinkID() );
	clProperty.m_defProperty["nextlnk"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetCancelLinkID() );
	clProperty.m_defProperty["canclnk"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetRewardContType() );
	clProperty.m_defProperty["rwdtype"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetLimitTime() );
	clProperty.m_defProperty["ltime"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetDesc() );
	clProperty.m_defProperty["desc"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", IsUseTable() ? 1 : 0 );
	clProperty.m_defProperty["usetbl"] = g_NtlTSString;

	sprintf_s( g_NtlTSString, "%d", GetRewardTableIndex() );
	clProperty.m_defProperty["rwdtbl"] = g_NtlTSString;

	if ( !IsUseTable() )
	{
		int i;
		sREWARD_INFO stInfo;
		char szKey[128];

		// 기본 보상
		for ( i = 0; i < 4; ++i )
		{
			memcpy( &stInfo, &m_asDefReward[i], sizeof( sREWARD_INFO ) );

			if ( eREWARD_TYPE_INVALID != stInfo.m_eType )
			{
				sprintf_s( szKey, "dtype%d", i );
				sprintf_s( g_NtlTSString, "%d", stInfo.m_eType );
				clProperty.m_defProperty[szKey] = g_NtlTSString;

				sprintf_s( szKey, "didx%d", i );
				sprintf_s( g_NtlTSString, "%d", stInfo.m_uiIdx );
				clProperty.m_defProperty[szKey] = g_NtlTSString;

				sprintf_s( szKey, "dval%d", i );
				sprintf_s( g_NtlTSString, "%d", stInfo.m_nValue );
				clProperty.m_defProperty[szKey] = g_NtlTSString;
			}
		}

		// 선택 보상
		for ( i = 0; i < 4; ++i )
		{
			memcpy( &stInfo, &m_asSelReward[i], sizeof( sREWARD_INFO ) );

			if ( eREWARD_TYPE_INVALID != stInfo.m_eType )
			{
				sprintf_s( szKey, "stype%d", i );
				sprintf_s( g_NtlTSString, "%d", stInfo.m_eType );
				clProperty.m_defProperty[szKey] = g_NtlTSString;

				sprintf_s( szKey, "sidx%d", i );
				sprintf_s( g_NtlTSString, "%d", stInfo.m_uiIdx );
				clProperty.m_defProperty[szKey] = g_NtlTSString;

				sprintf_s( szKey, "sval%d", i );
				sprintf_s( g_NtlTSString, "%d", stInfo.m_nValue );
				clProperty.m_defProperty[szKey] = g_NtlTSString;
			}
		}
	}
}

void CDboTSContReward::LoadTable( CQuestRewardTable* pRewardTbl )
{
#define GET_TRIGGER_ID (m_pParent ? ( m_pParent->GetParent() ? ((CNtlTSTrigger*)m_pParent->GetParent())->GetID() : 0 ) : 0)

	memset( m_asDefReward, 0xff, sizeof(m_asDefReward) );
	memset( m_asSelReward, 0xff, sizeof(m_asSelReward) );

	if ( NULL == pRewardTbl )
	{
		CNtlTSLog::Log( "Can not find the reward table. [%d]", GET_TRIGGER_ID, TS_CODE_TRACE() );
		return;
	}

	sQUEST_REWARD_TBLDAT* pTblData = (sQUEST_REWARD_TBLDAT*) pRewardTbl->FindData( GetRewardTableIndex() );

	if ( NULL == pTblData )
	{
		CNtlTSLog::Log( "Can not find the reward table. [%d, %d]", GET_TRIGGER_ID, GetRewardTableIndex(), TS_CODE_TRACE() );
		return;
	}

	int i = 0;

	for ( i = 0; i < QUEST_REWARD_DEF_MAX_CNT; ++i )
	{
		m_asDefReward[i].m_eType = pTblData->arsDefRwd[i].byRewardType == 255 ? eREWARD_TYPE_INVALID : (eREWARD_TYPE)pTblData->arsDefRwd[i].byRewardType;
		m_asDefReward[i].m_uiIdx = pTblData->arsDefRwd[i].dwRewardIdx;
		m_asDefReward[i].m_nValue = pTblData->arsDefRwd[i].dwRewardVal;
	}

	for ( i = 0; i < QUEST_REWARD_SEL_MAX_CNT; ++i )
	{
		m_asSelReward[i].m_eType = pTblData->arsSelRwd[i].byRewardType == 255 ? eREWARD_TYPE_INVALID : (eREWARD_TYPE)pTblData->arsSelRwd[i].byRewardType;
		m_asSelReward[i].m_uiIdx = pTblData->arsSelRwd[i].dwRewardIdx;
		m_asSelReward[i].m_nValue = pTblData->arsSelRwd[i].dwRewardVal;
	}
}
