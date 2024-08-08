#include "precomp_trigger.h"
#include "DboTSCheckProgQuest.h"


/**
	Check progress quest condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckProgQuest, CNtlTSCond )


CDboTSCheckProgQuest::CDboTSCheckProgQuest( void )
: m_tQuestID( NTL_TS_T_ID_INVALID )
{
}

void CDboTSCheckProgQuest::SetQuestID( NTL_TS_T_ID tQID )
{
	m_tQuestID = tQID;
}

void CDboTSCheckProgQuest::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "qid" ) )
	{
		SetQuestID( (NTL_TS_T_ID)clProperty.GetValueAsInt( "qid" ) );
	}
}

void CDboTSCheckProgQuest::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetQuestID() );
	clProperty.m_defProperty["qid"] = g_NtlTSString;
}
