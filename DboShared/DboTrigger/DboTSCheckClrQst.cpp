#include "precomp_trigger.h"
#include "DboTSCheckClrQst.h"


/**
	Check cleared quest condition
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSCheckClrQst, CNtlTSCond )


CDboTSCheckClrQst::CDboTSCheckClrQst( void )
{
}


bool CDboTSCheckClrQst::SetAndIdList( const std::string& strIDList )
{
	std::string strNum;
	strNum.reserve( 10 );

	std::string::const_iterator cit = strIDList.begin();
	for ( ; cit != strIDList.end(); ++cit )
	{
		if ( ';' == *cit )
		{
			m_vecAndIdList.push_back( (NTL_TS_T_ID)atoi( strNum.c_str() ) );
			strNum.clear();
		}
		else if ( '0' > *cit || '9' < *cit )
		{
			m_vecAndIdList.clear();
			CNtlTSLog::Log( "Only must have numbers. Info[%s]. [%s]", strIDList.c_str(), TS_CODE_TRACE() );
			return false;
		}
		else
		{
			strNum += *cit;
		}
	}

	if ( !strNum.empty() )
	{
		m_vecAndIdList.push_back( (NTL_TS_T_ID)atoi( strNum.c_str() ) );
	}

	return true;
}

void CDboTSCheckClrQst::ClearAndIdList( void )
{
	m_vecAndIdList.clear();
}

bool CDboTSCheckClrQst::SetOrIdList( const std::string& strIDList )
{
	std::string strNum;
	strNum.reserve( 10 );

	std::string::const_iterator cit = strIDList.begin();
	for ( ; cit != strIDList.end(); ++cit )
	{
		if ( ';' == *cit )
		{
			m_vecOrIdList.push_back( (NTL_TS_T_ID)atoi( strNum.c_str() ) );
			strNum.clear();
		}
		else if ( '0' > *cit || '9' < *cit )
		{
			m_vecOrIdList.clear();
			CNtlTSLog::Log( "Only must have numbers. Info[%s]. [%s]", strIDList.c_str(), TS_CODE_TRACE() );
			return false;
		}
		else
		{
			strNum += *cit;
		}
	}

	if ( !strNum.empty() )
	{
		m_vecOrIdList.push_back( (NTL_TS_T_ID)atoi( strNum.c_str() ) );
	}

	return true;
}

void CDboTSCheckClrQst::ClearOrIdList( void )
{
	m_vecOrIdList.clear();
}

void CDboTSCheckClrQst::ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::ApplyScriptDataForScript( clProperty );

	if ( clProperty.IsExist( "and" ) )
	{
		SetAndIdList( clProperty.GetValue( "and" ) );
	}

	if ( clProperty.IsExist( "or" ) )
	{
		SetOrIdList( clProperty.GetValue( "or" ) );
	}
}

void CDboTSCheckClrQst::TakeScriptDataForScript( CNtlTSScrProperty& clProperty )
{
	CNtlTSCond::TakeScriptDataForScript( clProperty );

	std::string strIdList;
	std::vector<NTL_TS_T_ID>::iterator it;

	it = m_vecAndIdList.begin();
	for ( ; it != m_vecAndIdList.end(); ++it )
	{
		sprintf_s( g_NtlTSString, "%d", *it );
		strIdList += g_NtlTSString;
		strIdList += ";";
	}
	if ( !strIdList.empty() )
	{
		clProperty.m_defProperty["and"] = strIdList;
	}

	strIdList.clear();
	it = m_vecOrIdList.begin();
	for ( ; it != m_vecOrIdList.end(); ++it )
	{
		sprintf_s( g_NtlTSString, "%d", *it );
		strIdList += g_NtlTSString;
		strIdList += ";";
	}
	if ( !strIdList.empty() )
	{
		clProperty.m_defProperty["or"] = strIdList;
	}
}