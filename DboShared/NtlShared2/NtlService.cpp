//***********************************************************************************
//
//	File		:	NtlService.cpp
//
//	Begin		:	2008-06-12
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "NtlService.h"
#include "NtlSharedDef.h"

#include <fstream>

bool Dbo_AnyInvalidCharacterInName(BYTE byServicePublisher, WCHAR* pwszName, DWORD dwMaxNameLength)
{
	switch (byServicePublisher)
	{
	case eDBO_SERVICE_PUBLISHER_NTL:
		return Dbo_AnyInvalidCharacterInName_Ntl(pwszName, dwMaxNameLength);

	case eDBO_SERVICE_PUBLISHER_CJI_KOR:
		return Dbo_AnyInvalidCharacterInName_CjiKorea(pwszName, dwMaxNameLength);

	default:
		return false;
	}
}

bool Dbo_AnyInvalidCharacterInName_Ntl(WCHAR* pwszName, DWORD dwMaxNameLength)
{
	if ( !Dbo_AnyInvalidCharacterInName_CjiKorea(pwszName, dwMaxNameLength) ) return false; // valid by Korean system
	if ( !Dbo_AnyInvalidCharacterInName_Japan(pwszName, dwMaxNameLength) ) return false; // valid by Japanese system
	
	// contain invalid unicode
	return true;
}

bool Dbo_AnyInvalidCharacterInName_CjiKorea(WCHAR* pwszName, DWORD dwMaxNameLength)
{
	if (NULL == pwszName)
	{
		return false;
	}

	for (DWORD dwIndex = 0 ; dwIndex < dwMaxNameLength ; dwIndex++)
	{
		if (0x0000 == pwszName[dwIndex])
			break;
		if (0x0030 <= pwszName[dwIndex] && 0x0039 >= pwszName[dwIndex])		// 숫자
			continue;
		if (0x0041 <= pwszName[dwIndex] && 0x005A >= pwszName[dwIndex])		// 영문대문자
 			continue;
		if (0x0061 <= pwszName[dwIndex] && 0x007A >= pwszName[dwIndex])		// 영문소문자
			continue;
//		if (0x002D == pwszName[dwIndex])									// -
//			continue;
//		if (0x005F == pwszName[dwIndex])									// _
//			continue;
//		if (0x1100 <= pwszName[dwIndex] && 0x1159 >= pwszName[dwIndex])		// 한글초성
//			continue;
//		if (0x1161 <= pwszName[dwIndex] && 0x11A2 >= pwszName[dwIndex])		// 한글중성
//			continue;
//		if (0x11A8 <= pwszName[dwIndex] && 0x11F9 >= pwszName[dwIndex])		// 한글종성
//			continue;
//		if (0x3131 <= pwszName[dwIndex] && 0x3163 >= pwszName[dwIndex])		// 한글 호환 자모
//			continue;
//		if (0x3165 <= pwszName[dwIndex] && 0x318E >= pwszName[dwIndex])		// 한글 호환 자모
//			continue;
		if (0xAC00 <= pwszName[dwIndex] && 0xD7A3 >= pwszName[dwIndex])		// 한글 음절
			continue;

		// contain invalid unicode
		return true;
	}

	return false;
}

bool Dbo_AnyInvalidCharacterInName_Japan(WCHAR* pwszName, DWORD dwMaxNameLength)
{
	if (NULL == pwszName)
	{
		return false;
	}

	for (DWORD dwIndex = 0 ; dwIndex < dwMaxNameLength ; dwIndex++)
	{
		if (0x0000 == pwszName[dwIndex])
		if (0x0030 <= pwszName[dwIndex] && 0x0039 >= pwszName[dwIndex])	continue;	// 숫자
		if (0x0041 <= pwszName[dwIndex] && 0x005A >= pwszName[dwIndex])	continue;	// 영문대문자
		if (0x0061 <= pwszName[dwIndex] && 0x007A >= pwszName[dwIndex])	continue;	// 영문소문자
		if (0x002D == pwszName[dwIndex]) continue;									// -
		if (0x005F == pwszName[dwIndex]) continue;									// _
		
		// Kanji
		if (0x8140 == pwszName[dwIndex]) continue;
		if (0x8145 == pwszName[dwIndex]) continue;
		if (0x815B <= pwszName[dwIndex] && 0x815C >= pwszName[dwIndex]) continue;
		if (0x8260 <= pwszName[dwIndex] && 0x8279 >= pwszName[dwIndex]) continue;
		if (0x8281 <= pwszName[dwIndex] && 0x829A >= pwszName[dwIndex]) continue;
		if (0x829F <= pwszName[dwIndex] && 0x82F1 >= pwszName[dwIndex]) continue;
		if (0x8340 <= pwszName[dwIndex] && 0x8396 >= pwszName[dwIndex]) continue;
		if (0x889F <= pwszName[dwIndex] && 0x88FC >= pwszName[dwIndex]) continue;
		if (0x8940 <= pwszName[dwIndex] && 0x89FC >= pwszName[dwIndex]) continue;
		if (0x8A40 <= pwszName[dwIndex] && 0x8AFC >= pwszName[dwIndex]) continue;
		if (0x8B40 <= pwszName[dwIndex] && 0x8BFC >= pwszName[dwIndex]) continue;
		if (0x8C40 <= pwszName[dwIndex] && 0x8CFC >= pwszName[dwIndex]) continue;
		if (0x8D40 <= pwszName[dwIndex] && 0x8DFC >= pwszName[dwIndex]) continue;
		if (0x8E40 <= pwszName[dwIndex] && 0x8EFC >= pwszName[dwIndex]) continue;
		if (0x8F40 <= pwszName[dwIndex] && 0x8FFC >= pwszName[dwIndex]) continue;
		if (0x9040 <= pwszName[dwIndex] && 0x90FC >= pwszName[dwIndex]) continue;
		if (0x9140 <= pwszName[dwIndex] && 0x91FC >= pwszName[dwIndex]) continue;
		if (0x9240 <= pwszName[dwIndex] && 0x92FC >= pwszName[dwIndex]) continue;
		if (0x9340 <= pwszName[dwIndex] && 0x93FC >= pwszName[dwIndex]) continue;
		if (0x9440 <= pwszName[dwIndex] && 0x94FC >= pwszName[dwIndex]) continue;
		if (0x9540 <= pwszName[dwIndex] && 0x95FC >= pwszName[dwIndex]) continue;
		if (0x9640 <= pwszName[dwIndex] && 0x96FC >= pwszName[dwIndex]) continue;
		if (0x9740 <= pwszName[dwIndex] && 0x97FC >= pwszName[dwIndex]) continue;
		if (0x9840 <= pwszName[dwIndex] && 0x9872 >= pwszName[dwIndex]) continue;
		if (0x989F <= pwszName[dwIndex] && 0x98FC >= pwszName[dwIndex]) continue;
		if (0x9940 <= pwszName[dwIndex] && 0x99FC >= pwszName[dwIndex]) continue;
		if (0x9A40 <= pwszName[dwIndex] && 0x9AFC >= pwszName[dwIndex]) continue;
		if (0x9B40 <= pwszName[dwIndex] && 0x9BFC >= pwszName[dwIndex]) continue;
		if (0x9C40 <= pwszName[dwIndex] && 0x9CFC >= pwszName[dwIndex]) continue;
		if (0x9D40 <= pwszName[dwIndex] && 0x9DFC >= pwszName[dwIndex]) continue;
		if (0x9E40 <= pwszName[dwIndex] && 0x9EFC >= pwszName[dwIndex]) continue;
		if (0x9F40 <= pwszName[dwIndex] && 0x9FFC >= pwszName[dwIndex]) continue;
		if (0xE040 <= pwszName[dwIndex] && 0xE0FC >= pwszName[dwIndex]) continue;
		if (0xE140 <= pwszName[dwIndex] && 0xE1FC >= pwszName[dwIndex]) continue;
		if (0xE240 <= pwszName[dwIndex] && 0xE2FC >= pwszName[dwIndex]) continue;
		if (0xE340 <= pwszName[dwIndex] && 0xE3FC >= pwszName[dwIndex]) continue;
		if (0xE440 <= pwszName[dwIndex] && 0xE4FC >= pwszName[dwIndex]) continue;
		if (0xE540 <= pwszName[dwIndex] && 0xE5FC >= pwszName[dwIndex]) continue;
		if (0xE640 <= pwszName[dwIndex] && 0xE6FC >= pwszName[dwIndex]) continue;
		if (0xE740 <= pwszName[dwIndex] && 0xE7FC >= pwszName[dwIndex]) continue;
		if (0xE840 <= pwszName[dwIndex] && 0xE8FC >= pwszName[dwIndex]) continue;
		if (0xE940 <= pwszName[dwIndex] && 0xE9FC >= pwszName[dwIndex]) continue;
		if (0xEA40 <= pwszName[dwIndex] && 0xEAA4 >= pwszName[dwIndex]) continue;
		if (0xFA5C <= pwszName[dwIndex] && 0xFA67 >= pwszName[dwIndex]) continue;
		if (0xFA69 <= pwszName[dwIndex] && 0xFAFC >= pwszName[dwIndex]) continue;
		if (0xFB40 <= pwszName[dwIndex] && 0xFB7E >= pwszName[dwIndex]) continue;
		if (0xFB80 <= pwszName[dwIndex] && 0xFBFC >= pwszName[dwIndex]) continue;
		if (0xFC40 <= pwszName[dwIndex] && 0xFC4B >= pwszName[dwIndex]) continue;

		// contain invalid unicode
		return true;
	}

	return false;
}

bool Dbo_IsNameTooShort(BYTE byServicePublisher, WCHAR* pwszName)
{
	switch (byServicePublisher)
	{
	case eDBO_SERVICE_PUBLISHER_NTL:
		return Dbo_IsNameTooShort_Ntl(pwszName);

	case eDBO_SERVICE_PUBLISHER_CJI_KOR:
		return Dbo_IsNameTooShort_CjiKorea(pwszName);

	default:
		return false;
	}
}

bool Dbo_IsNameTooShort_Ntl(WCHAR* pwszName)
{
	return Dbo_IsNameTooShort_CjiKorea(pwszName);
}

bool Dbo_IsNameTooShort_CjiKorea(WCHAR* pwszName)
{
	if (wcslen(pwszName) < NTL_MIN_SIZE_CHAR_NAME_UNICODE)
	{
		return true;
	}

	return false;
}

bool Dbo_IsNameTooShort_Japan(WCHAR* pwszName)
{
	return Dbo_IsNameTooShort_CjiKorea(pwszName);
}

bool Dbo_IsNameTooLong(BYTE byServicePublisher, WCHAR* pwszName)
{
	switch (byServicePublisher)
	{
	case eDBO_SERVICE_PUBLISHER_NTL:
		return Dbo_IsNameTooLong_Ntl(pwszName);

	case eDBO_SERVICE_PUBLISHER_CJI_KOR:
		return Dbo_IsNameTooLong_CjiKorea(pwszName);

	default:
		return false;
	}
}

bool Dbo_IsNameTooLong_Ntl(WCHAR* pwszName)
{
	return Dbo_IsNameTooLong_CjiKorea(pwszName);
}

bool Dbo_IsNameTooLong_CjiKorea(WCHAR* pwszName)
{
	int nRequiredBytes = ::WideCharToMultiByte(::GetACP(), 0, pwszName, -1, NULL, 0, NULL, NULL);
	if (NTL_MAX_SIZE_CHAR_NAME < (nRequiredBytes - 1))
	{
		return true;
	}

	return false;
}

bool Dbo_IsNameTooLong_Japan(WCHAR* pwszName)
{
	return Dbo_IsNameTooLong_CjiKorea(pwszName);
}

bool ReadServerSetting( sDBO_SERVER_SETTING &sInfo )
{
	std::string path("../");
	path += SERVER_SETTING_FILE;
	std::ifstream fs( path.c_str(), std::ios::binary );
	if ( false == fs.is_open() )
	{
		return false;
	}

	fs.read((char*)&sInfo, sizeof(sDBO_SERVER_SETTING));
	fs.close();

	return true;
}