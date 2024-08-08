//***********************************************************************************
//
//	File		:	NtlFriend.h
//
//	Begin		:	2007-11-29
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	john
//
//	Desc		:	친구 시스템
//
//***********************************************************************************
#pragma once
#include "NtlSharedDef.h"
struct sFRIEND_FULL_DATA
{
	bool		bIsBlack;
	CHARACTERID charID;
	WCHAR		wchName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
};

struct sFRIEND_FULL_INFO
{
	bool		bIsBlack;
	CHARACTERID charID;
	WCHAR		wchName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
};

struct sFRIEND_FULL_LIST
{
	sFRIEND_FULL_DATA sData[NTL_MAX_COUNT_FRIEND * 2];
};

struct sFRIEND_INFO
{
	CHARACTERID charID;
	HOBJECT		hHandle;
	BYTE		byClass;
	BYTE		byChannel;
	BYTE		byLevel;	
	TBLIDX		mapNameTblidx;
};
enum eFRIEND_CHANGE_TYPE
{
	eFRIEND_CHANGE_TYPE_OFFLINE = 1,
	eFRIEND_CHANGE_TYPE_CLASS,
	eFRIEND_CHANGE_TYPE_LEVEL,
	eFRIEND_CHANGE_TYPE_POSITION,

	eFRIEND_CHANGE_TYPE_COUNT,
	eFRIEND_CHANGE_TYPE_INVALID = 0xFF
};
