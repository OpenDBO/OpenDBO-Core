//***********************************************************************************
//
//	File		:	NtlService.h
//
//	Begin		:	2008-05-18
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#define SERVER_SETTING_FILE "serverOption"

enum eDBO_ALLOWED_FUNC_FOR_DEV
{
	DBO_ALLOWED_FUNC_FOR_DEV_HUMAN =0,
	DBO_ALLOWED_FUNC_FOR_DEV_NAMEK,
	DBO_ALLOWED_FUNC_FOR_DEV_MAJIN,

	DBO_ALLOWED_FUNC_FOR_DEV_COUNT,

	DBO_ALLOWED_FUNC_FOR_DEV_FIRST = DBO_ALLOWED_FUNC_FOR_DEV_MAJIN,
	DBO_ALLOWED_FUNC_FOR_DEV_LAST = DBO_ALLOWED_FUNC_FOR_DEV_COUNT - 1
};

enum eDBO_ALLOWED_FUNC_FOR_DEV_FLAG
{
	DBO_ALLOWED_FUNC_FOR_DEV_FLAG_HUMAN = 1 << DBO_ALLOWED_FUNC_FOR_DEV_HUMAN,
	DBO_ALLOWED_FUNC_FOR_DEV_FLAG_NAMEK = 1 << DBO_ALLOWED_FUNC_FOR_DEV_NAMEK,
	DBO_ALLOWED_FUNC_FOR_DEV_FLAG_MAJIN = 1 << DBO_ALLOWED_FUNC_FOR_DEV_MAJIN,
};

enum eDBO_SERVICE_PUBLISHER
{
	eDBO_SERVICE_PUBLISHER_NTL = 0,
	eDBO_SERVICE_PUBLISHER_CJI_KOR,

	eDBO_SERVICE_PUBLISHER_UNDEFINED = 0xFFui8
};

struct sDBO_SERVER_SETTING
{
	bool	bAllowHuman;
	bool	bAllowNamek;
	bool	bAllowMajin;
	int		nMaxLevel;
};

bool ReadServerSetting( sDBO_SERVER_SETTING &sInfo );

bool Dbo_AnyInvalidCharacterInName(BYTE byServicePublisher, WCHAR* pwszName, DWORD dwMaxNameLength);
bool Dbo_AnyInvalidCharacterInName_Ntl(WCHAR* pwszName, DWORD dwMaxNameLength);
bool Dbo_AnyInvalidCharacterInName_CjiKorea(WCHAR* pwszName, DWORD dwMaxNameLength);
bool Dbo_AnyInvalidCharacterInName_Japan(WCHAR* pwszName, DWORD dwMaxNameLength);

bool Dbo_IsNameTooShort(BYTE byServicePublisher, WCHAR* pwszName);
bool Dbo_IsNameTooShort_Ntl(WCHAR* pwszName);
bool Dbo_IsNameTooShort_CjiKorea(WCHAR* pwszName);
bool Dbo_IsNameTooShort_Japan(WCHAR* pwszName);

bool Dbo_IsNameTooLong(BYTE byServicePublisher, WCHAR* pwszName);
bool Dbo_IsNameTooLong_Ntl(WCHAR* pwszName);
bool Dbo_IsNameTooLong_CjiKorea(WCHAR* pwszName);
bool Dbo_IsNameTooLong_Japan(WCHAR* pwszName);