#include "stdafx.h"
#include <algorithm>
#include "DLSystemEnv.h"


//////////////////////////////////////////////////////////////////////////
//
// Key table of the CT_TW build (id, type, group, key, default)
//
//////////////////////////////////////////////////////////////////////////


static const CDLSystemEnv::sKEY_DEF s_asKeyDef[] =
{
	{ DL_STORAGE_GRAPHIC_SELECT_VIDEOWIDTH,		CDLSystemEnv::eTYPE_UINT,	"GRAPHIC",	"VIDEO_WIDTH",			"1280"	},
	{ DL_STORAGE_GRAPHIC_SELECT_VIDEOHEIGHT,	CDLSystemEnv::eTYPE_UINT,	"GRAPHIC",	"VIDEO_HEIGHT",			"1024"	},
	{ DL_STORAGE_GRAPHIC_SELECT_VIDEODEPTH,		CDLSystemEnv::eTYPE_UINT,	"GRAPHIC",	"VIDEO_DEPTH",			"32"	},
	{ DL_STORAGE_GRAPHIC_GAMMA,					CDLSystemEnv::eTYPE_FLOAT,	"GRAPHIC",	"VIDEO_GAMMA",			"1.0"	},
	{ DL_STORAGE_GRAPHIC_WINDOW_MODE,			CDLSystemEnv::eTYPE_BOOL,	"GRAPHIC",	"WINDOW_MODE",			"false"	},
	{ DL_STORAGE_GRAPHIC_PRESET,				CDLSystemEnv::eTYPE_INT,	"GRAPHIC",	"PRESET",				"2"		},
	{ DL_STORAGE_GRAPHIC_TERRAIN_RANGE,			CDLSystemEnv::eTYPE_INT,	"GRAPHIC",	"TERRAIN_RANGE",		"3"		},
	{ DL_STORAGE_GRAPHIC_OBJECT_RANGE,			CDLSystemEnv::eTYPE_INT,	"GRAPHIC",	"OBJECT_RANGE",			"3"		},
	{ DL_STORAGE_GRAPHIC_TERRAIN_SHADOW,		CDLSystemEnv::eTYPE_BOOL,	"GRAPHIC",	"TERRAIN_SHADOW",		"true"	},
	{ DL_STORAGE_GRAPHIC_WATER_EFFECT,			CDLSystemEnv::eTYPE_BOOL,	"GRAPHIC",	"WATER_EFFECT",			"true"	},
	{ DL_STORAGE_GRAPHIC_CHARACTER_RANGE,		CDLSystemEnv::eTYPE_INT,	"GRAPHIC",	"CHARACTER_RANGE",		"3"		},
	{ DL_STORAGE_GRAPHIC_CHARACTER_EFFECT,		CDLSystemEnv::eTYPE_BOOL,	"GRAPHIC",	"CHARACTER_EFFECT",		"true"	},
	{ DL_STORAGE_GRAPHIC_CHARACTER_SHADOW,		CDLSystemEnv::eTYPE_BOOL,	"GRAPHIC",	"CHARACTER_SHADOW",		"true"	},
	{ DL_STORAGE_GRAPHIC_MAGIC_EFFECT,			CDLSystemEnv::eTYPE_INT,	"GRAPHIC",	"MAGIC_EFFECT",			"3"		},
	{ DL_STORAGE_GRAPHIC_WEATHER_EFFECT,		CDLSystemEnv::eTYPE_INT,	"GRAPHIC",	"WEATHER_EFFECT",		"0"		},
	{ DL_STORAGE_GRAPHIC_TEXTURE_LEVEL,			CDLSystemEnv::eTYPE_INT,	"GRAPHIC",	"TEXTURE_LEVEL",		"2"		},
	{ DL_STORAGE_GRAPHIC_SHADER_HDR,			CDLSystemEnv::eTYPE_BOOL,	"GRAPHIC",	"HDR_SHADER",			"true"	},
	{ DL_STORAGE_GRAPHIC_CHARACTER_EDGE,		CDLSystemEnv::eTYPE_BOOL,	"GRAPHIC",	"CHARACTER_EDGE",		"true"	},
	{ DL_STORAGE_SOUND_MAIN_VOLUME,				CDLSystemEnv::eTYPE_FLOAT,	"SOUND",	"MAIN_VOLUME",			"1.0"	},
	{ DL_STORAGE_SOUND_BACK_VOLUME,				CDLSystemEnv::eTYPE_FLOAT,	"SOUND",	"BACK_VOLUME",			"0.9"	},
	{ DL_STORAGE_SOUND_EFFECT_VOLUME,			CDLSystemEnv::eTYPE_FLOAT,	"SOUND",	"EFFECT_VOLUME",		"0.5"	},
	{ DL_STORAGE_SOUND_ENV_VOLUME,				CDLSystemEnv::eTYPE_FLOAT,	"SOUND",	"ENV_VOLUME",			"0.75"	},
};

static const char* s_apszGroup[] = { "GRAPHIC", "SOUND" };


static std::string TrimString( const std::string& str )
{
	size_t nBegin = str.find_first_not_of( " \t\r\n" );
	if ( std::string::npos == nBegin )
	{
		return std::string();
	}

	size_t nEnd = str.find_last_not_of( " \t\r\n" );
	return str.substr( nBegin, nEnd - nBegin + 1 );
}


//////////////////////////////////////////////////////////////////////////
//
// CDLSystemEnv
//
//////////////////////////////////////////////////////////////////////////


CDLSystemEnv::CDLSystemEnv( void )
{
	Reset();
}

CDLSystemEnv* CDLSystemEnv::GetInstance( void )
{
	static CDLSystemEnv s_clSystemEnv;

	return &s_clSystemEnv;
}

bool CDLSystemEnv::Load( void )
{
	DWORD dwAttr = ::GetFileAttributesA( DL_SYSTEM_ENV_FILE_NAME );
	if ( INVALID_FILE_ATTRIBUTES != dwAttr && ( dwAttr & FILE_ATTRIBUTE_READONLY ) )
	{
		::SetFileAttributesA( DL_SYSTEM_ENV_FILE_NAME, dwAttr & ~FILE_ATTRIBUTE_READONLY );
	}

	FILE* pFile = NULL;
	if ( 0 != fopen_s( &pFile, DL_SYSTEM_ENV_FILE_NAME, "rt" ) || NULL == pFile )
	{
		return false;
	}

	std::string strGroup;
	char szLine[1024];

	while ( fgets( szLine, sizeof( szLine ), pFile ) )
	{
		std::string strLine = TrimString( szLine );

		if ( strLine.empty() )
		{
			continue;
		}

		if ( strLine[0] == '[' )
		{
			size_t nEnd = strLine.find( ']' );
			strGroup = strLine.substr( 1, std::string::npos == nEnd ? std::string::npos : nEnd - 1 );
			continue;
		}

		size_t nEqual = strLine.find( '=' );
		if ( std::string::npos == nEqual )
		{
			continue;
		}

		std::string strKey = TrimString( strLine.substr( 0, nEqual ) );
		std::string strValue = strLine.substr( nEqual + 1 );

		size_t nSemicolon = strValue.find( ';' );
		if ( std::string::npos != nSemicolon )
		{
			strValue = strValue.substr( 0, nSemicolon );
		}

		strValue = TrimString( strValue );

		const sKEY_DEF* pDef = FindKeyDef( strKey.c_str() );
		if ( pDef )
		{
			m_mapValue[pDef->uiID] = strValue;
		}
		else
		{
			bool bFound = false;

			for ( std::vector< sEXTRA_KEY >::iterator it = m_vecExtra.begin(); it != m_vecExtra.end(); ++it )
			{
				if ( it->strGroup == strGroup && it->strKey == strKey )
				{
					it->strValue = strValue;
					bFound = true;
					break;
				}
			}

			if ( !bFound )
			{
				sEXTRA_KEY sExtra;
				sExtra.strGroup = strGroup;
				sExtra.strKey = strKey;
				sExtra.strValue = strValue;
				m_vecExtra.push_back( sExtra );
			}
		}
	}

	fclose( pFile );

	return true;
}

bool CDLSystemEnv::Save( void )
{
	DWORD dwAttr = ::GetFileAttributesA( DL_SYSTEM_ENV_FILE_NAME );
	if ( INVALID_FILE_ATTRIBUTES != dwAttr && ( dwAttr & FILE_ATTRIBUTE_READONLY ) )
	{
		::SetFileAttributesA( DL_SYSTEM_ENV_FILE_NAME, dwAttr & ~FILE_ATTRIBUTE_READONLY );
	}

	FILE* pFile = NULL;
	if ( 0 != fopen_s( &pFile, DL_SYSTEM_ENV_FILE_NAME, "wb" ) || NULL == pFile )
	{
		return false;
	}

	std::vector< std::string > vecGroup( s_apszGroup, s_apszGroup + _countof( s_apszGroup ) );

	for ( std::vector< sEXTRA_KEY >::iterator it = m_vecExtra.begin(); it != m_vecExtra.end(); ++it )
	{
		if ( std::find( vecGroup.begin(), vecGroup.end(), it->strGroup ) == vecGroup.end() )
		{
			vecGroup.push_back( it->strGroup );
		}
	}

	for ( std::vector< std::string >::iterator itGroup = vecGroup.begin(); itGroup != vecGroup.end(); ++itGroup )
	{
		fprintf( pFile, "[%s]\r\n", itGroup->c_str() );

		for ( int i = 0; i < _countof( s_asKeyDef ); ++i )
		{
			if ( *itGroup == s_asKeyDef[i].pszGroup )
			{
				fprintf( pFile, "%s = %s;\r\n", s_asKeyDef[i].pszKey, m_mapValue[s_asKeyDef[i].uiID].c_str() );
			}
		}

		for ( std::vector< sEXTRA_KEY >::iterator it = m_vecExtra.begin(); it != m_vecExtra.end(); ++it )
		{
			if ( *itGroup == it->strGroup )
			{
				fprintf( pFile, "%s = %s;\r\n", it->strKey.c_str(), it->strValue.c_str() );
			}
		}
	}

	fclose( pFile );

	return true;
}

void CDLSystemEnv::Reset( void )
{
	for ( int i = 0; i < _countof( s_asKeyDef ); ++i )
	{
		m_mapValue[s_asKeyDef[i].uiID] = s_asKeyDef[i].pszDefault;
	}
}

int CDLSystemEnv::GetInt( unsigned int uiID )
{
	return (int)atof( m_mapValue[uiID].c_str() );
}

float CDLSystemEnv::GetFloat( unsigned int uiID )
{
	return (float)atof( m_mapValue[uiID].c_str() );
}

bool CDLSystemEnv::GetBool( unsigned int uiID )
{
	const std::string& strValue = m_mapValue[uiID];

	return 0 == _stricmp( strValue.c_str(), "true" ) || 0 != atoi( strValue.c_str() );
}

void CDLSystemEnv::SetInt( unsigned int uiID, int nValue )
{
	const sKEY_DEF* pDef = FindKeyDef( uiID );

	if ( pDef && eTYPE_FLOAT == pDef->eType )
	{
		SetFloat( uiID, (float)nValue );
		return;
	}

	if ( pDef && eTYPE_BOOL == pDef->eType )
	{
		SetBool( uiID, 0 != nValue );
		return;
	}

	char szBuffer[64];
	sprintf_s( szBuffer, "%d", nValue );

	m_mapValue[uiID] = szBuffer;
}

void CDLSystemEnv::SetFloat( unsigned int uiID, float fValue )
{
	m_mapValue[uiID] = FormatFloat( fValue );
}

void CDLSystemEnv::SetBool( unsigned int uiID, bool bValue )
{
	m_mapValue[uiID] = bValue ? "true" : "false";
}

const CDLSystemEnv::sKEY_DEF* CDLSystemEnv::FindKeyDef( unsigned int uiID )
{
	for ( int i = 0; i < _countof( s_asKeyDef ); ++i )
	{
		if ( s_asKeyDef[i].uiID == uiID )
		{
			return &s_asKeyDef[i];
		}
	}

	return NULL;
}

const CDLSystemEnv::sKEY_DEF* CDLSystemEnv::FindKeyDef( const char* pszKey )
{
	for ( int i = 0; i < _countof( s_asKeyDef ); ++i )
	{
		if ( 0 == strcmp( s_asKeyDef[i].pszKey, pszKey ) )
		{
			return &s_asKeyDef[i];
		}
	}

	return NULL;
}

std::string CDLSystemEnv::FormatFloat( float fValue )
{
	// Same look as the client writes them: "1.0", "0.9", "0.75"
	char szBuffer[64];
	sprintf_s( szBuffer, "%.2f", fValue );

	std::string strValue = szBuffer;

	while ( strValue.size() > 3 && strValue[strValue.size() - 1] == '0' && strValue[strValue.size() - 2] != '.' )
	{
		strValue.erase( strValue.size() - 1 );
	}

	return strValue;
}
