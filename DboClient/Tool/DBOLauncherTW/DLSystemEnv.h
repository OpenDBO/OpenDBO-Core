/*****************************************************************************
*
* File			: DLSystemEnv.h
* Abstract		: Reader/writer of the client's ".\user\SystemEnv.txt".
*
*                 The retail launcher links the client's CNtlStorage
*                 system group (CNtlStorageManager, CNtlStorageGroupSystem,
*                 CNtlStorageTextSerializer). Only the system keys are used by
*                 the option dialog, so this class implements exactly that
*                 group with the key table, types and defaults of the CT_TW
*                 build, in the same text format:
*
*                     [GRAPHIC]
*                     VIDEO_WIDTH = 1280;
*                     ...
*                     [SOUND]
*                     MAIN_VOLUME = 1.0;
*
*                 Keys the table does not know are kept and written back.
*
*****************************************************************************/


#pragma once


// Storage key ids (dSTORAGE_* of the client)
#define DL_STORAGE_GRAPHIC_SELECT_VIDEOWIDTH		2000
#define DL_STORAGE_GRAPHIC_SELECT_VIDEOHEIGHT		2001
#define DL_STORAGE_GRAPHIC_SELECT_VIDEODEPTH		2002
#define DL_STORAGE_GRAPHIC_GAMMA					2003
#define DL_STORAGE_GRAPHIC_WINDOW_MODE				2004
#define DL_STORAGE_GRAPHIC_PRESET					2005
#define DL_STORAGE_GRAPHIC_TERRAIN_RANGE			2006
#define DL_STORAGE_GRAPHIC_OBJECT_RANGE				2007
#define DL_STORAGE_GRAPHIC_TERRAIN_SHADOW			2010
#define DL_STORAGE_GRAPHIC_WATER_EFFECT				2011
#define DL_STORAGE_GRAPHIC_CHARACTER_RANGE			2012
#define DL_STORAGE_GRAPHIC_CHARACTER_EFFECT			2013
#define DL_STORAGE_GRAPHIC_CHARACTER_SHADOW			2014
#define DL_STORAGE_GRAPHIC_MAGIC_EFFECT				2015
#define DL_STORAGE_GRAPHIC_WEATHER_EFFECT			2016
#define DL_STORAGE_GRAPHIC_TEXTURE_LEVEL			2017
#define DL_STORAGE_GRAPHIC_SHADER_HDR				2018
#define DL_STORAGE_GRAPHIC_CHARACTER_EDGE			2019
#define DL_STORAGE_SOUND_MAIN_VOLUME				4000
#define DL_STORAGE_SOUND_BACK_VOLUME				4001
#define DL_STORAGE_SOUND_EFFECT_VOLUME				4002
#define DL_STORAGE_SOUND_ENV_VOLUME					4003

#define DL_SYSTEM_ENV_FOLDER						".\\user"
#define DL_SYSTEM_ENV_FILE_NAME						".\\user\\SystemEnv.txt"


class CDLSystemEnv
{
public:
	enum eTYPE
	{
		eTYPE_INT,
		eTYPE_FLOAT,
		eTYPE_BOOL,
		eTYPE_UINT
	};

	struct sKEY_DEF
	{
		unsigned int		uiID;
		eTYPE				eType;
		const char*			pszGroup;
		const char*			pszKey;
		const char*			pszDefault;
	};

	struct sEXTRA_KEY
	{
		std::string			strGroup;
		std::string			strKey;
		std::string			strValue;
	};

protected:
	std::map< unsigned int, std::string >	m_mapValue;
	std::vector< sEXTRA_KEY >				m_vecExtra;

public:
	CDLSystemEnv( void );

public:
	static CDLSystemEnv*	GetInstance( void );

	bool					Load( void );
	bool					Save( void );

	// Restores every key of the system group to its default
	void					Reset( void );

	int						GetInt( unsigned int uiID );
	float					GetFloat( unsigned int uiID );
	bool					GetBool( unsigned int uiID );

	void					SetInt( unsigned int uiID, int nValue );
	void					SetFloat( unsigned int uiID, float fValue );
	void					SetBool( unsigned int uiID, bool bValue );

protected:
	static const sKEY_DEF*	FindKeyDef( unsigned int uiID );
	static const sKEY_DEF*	FindKeyDef( const char* pszKey );

	static std::string		FormatFloat( float fValue );
};
