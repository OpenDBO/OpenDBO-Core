/*****************************************************************************
 *
 * File			: DisplayPaser.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 20	
 * Abstract		: DBO gui displayer paser
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __DISPLAY_PARSER__
#define __DISPLAY_PARSER__

#include "NtlTokenOption.h"

class CDisplayParser
{
public:

	typedef std::map<std::string, RwInt32> MapDefOption;
	typedef std::map<std::wstring, RwInt32> MapDefOptionW;

private:

	CNtlTokenOptions m_DataOption;
	MapDefOption m_mapDefOption;

	CNtlTokenOptionsW m_DataOptionW;
	MapDefOptionW	m_mapDefOptionW;

	RwBool ParseData(const std::string &strFileName);
	RwBool ParseDef(const std::string &strFileName);

	RwBool ParseUnicodeData(const std::string &strFilename);
	RwBool ParseUnicodeDef(const std::string &strFilename);

public:

	CDisplayParser(const std::string &strDefFileName, const std::string &strDataFileName);

	const MapDefOption& GetDefOptions(void) const { return m_mapDefOption; }
	const CNtlTokenOptions& GetDataOptions(void) const { return m_DataOption; }

	const MapDefOptionW& GetDefOptionsW(void) const { return m_mapDefOptionW; }
	const CNtlTokenOptionsW& GetDataOptionsW(void) const { return m_DataOptionW; }
};

#endif