#include "precomp_trigger.h"
#include "DboTSClickNPC.h"


/**
	Click NPC event
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSClickNPC, CNtlTSEvent )


CDboTSClickNPC::CDboTSClickNPC( void )
: m_uiNPCIdx( 0xffffffff )
{
}

void CDboTSClickNPC::SetNPCIdx( unsigned int uiNPCIdx )
{
	m_uiNPCIdx = uiNPCIdx;
}

void CDboTSClickNPC::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "npcidx" ) )
	{
		SetNPCIdx( clProperty.GetValueAsInt( "npcidx" ) );
	}
}

void CDboTSClickNPC::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSEvent::TakeScriptDataForScript( clProperty );

	sprintf_s( g_NtlTSString, "%d", GetNPCIdx() );
	clProperty.m_defProperty["npcidx"] = g_NtlTSString;
}
