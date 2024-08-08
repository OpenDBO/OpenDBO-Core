/*****************************************************************************
 *
 * File			: DisplayStringManager.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 22	
 * Abstract		: DBO display string manager.h
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DISPLAY_STRING_MANAGER_H__
#define __DISPLAY_STRING_MANAGER_H__

#include "DisplayParser.h"
#include "DisplayStringDef.h"

class CDisplayStringManager
{
public:

	enum EDispType
	{
		CLIENT_ONLY_MSG,
        SERVER_SYNC_RESULT_MSG
	};


private:

	std::vector<std::wstring> m_vecClientString;
	std::vector<std::wstring> m_vecServerString;

	void Resize(RwUInt32 eType, RwInt32 iSize);
	void AddString(WCHAR **pBuffer, RwInt32& iBuffLen, RwUInt32 eType, RwInt32 iIdx, const std::string& str);
	void AddString(WCHAR **pBuffer, RwInt32& iBuffLen, RwUInt32 eType, RwInt32 iIdx, const std::wstring& wstr);

public:

	static CDisplayStringManager* GetInstance(void);
	
	const WCHAR* GetString(RwInt32 iIdx, RwUInt32 eType = CLIENT_ONLY_MSG) const;

	void InsertOptions(const CDisplayParser& Parser, RwUInt32 eType = CLIENT_ONLY_MSG);
};

static CDisplayStringManager* GetDisplayStringManager(void)
{
	return CDisplayStringManager::GetInstance(); 
}

#endif
