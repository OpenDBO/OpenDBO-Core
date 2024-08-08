//***********************************************************************************
//
//	File		:	NtlHlsItem.h
//
//	Begin		:	2009-8-14
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	chung,DooSup ( john@ntl-inc.com )
//
//	Desc		: HLS는 호이포이 럭셔리 샵의 약자로 Cash를 갖고 구매하는 상점을 뜻한다.	
//
//***********************************************************************************
#pragma once
#include "NtlSharedType.h"


//-----------------------------------------------------------------------------------
// 캐쉬 아이템 타입
//-----------------------------------------------------------------------------------
enum eCASHITEM_TYPE
{
	CASHITEM_TYPE_ITEM_CREATE,			// 캡슐킷으로 이동
	CASHITEM_TYPE_PACKAGE,				// 패키지
	CASHITEM_TYPE_GMT,					// 게임매니아타임재설정횟수증가
	CASHITEM_TYPE_ACCOUNT_USE,			// 계정창고 사용기간 아이템
	CASHITEM_TYPE_EXP_ZENNY,			// 경험치 제니 추가 획득기간 업데이트

	CASHITEM_TYPE_COUNT,

	CASHITEM_TYPE_INVALID = 0xff,
};
struct sCASHITEM_BRIEF
{
	DWORD			dwProductId;
	TBLIDX			HLSitemTblidx; // item table index
	BYTE			byStackCount;
};

//-----------------------------------------------------------------------------------
// 캐쉬 아이템 디스프레이 펑션 타입 플래그
//----------------------------------------------------------------------------------
enum eHLS_TAB_TYPE
{
	HLS_TAB_TYPE_NORMAL,
	HLS_TAB_TYPE_ALL,
	HLS_TAB_TYPE_NEW,
	HLS_TAB_TYPE_HOT,
	HLS_TAB_TYPE_LIMITED,

	HLS_TAB_TYPE_COUNT,

	HLS_TAB_TYPE_FIRST = HLS_TAB_TYPE_NORMAL,
	HLS_TAB_TYPE_LAST = HLS_TAB_TYPE_COUNT - 1,
};

enum eHLS_SELL_TYPE
{
	HLS_SELL_TYPE_NORMAL,
	HLS_SELL_TYPE_PERIOD,	// 기간
	HLS_SELL_TYPE_TOTAL_COUNT,	// 개수
	
	HLS_SELL_TYPE_COUNT,

	HLS_SELL_TYPE_FIRST = HLS_SELL_TYPE_NORMAL,
	HLS_SELL_TYPE_LAST = HLS_SELL_TYPE_COUNT - 1,
};

enum eHLS_DISPLAY_ITEM_FUNC
{
	HLS_DISPLAY_ITEM_FUNC_NORMAL,
	HLS_DISPLAY_ITEM_FUNC_NEW,
	HLS_DISPLAY_ITEM_FUNC_HOT,
	HLS_DISPLAY_ITEM_FUNC_LIMITED,
};

enum eHLS_DISPLAY_ITEM_FUNC_BIT_FLAG
{
	HLS_DISPLAY_ITEM_FUNC_BIT_FLAG_NORMAL	= 0x01 << HLS_DISPLAY_ITEM_FUNC_NORMAL,
	HLS_DISPLAY_ITEM_FUNC_BIT_FLAG_NEW		= 0x01 << HLS_DISPLAY_ITEM_FUNC_NEW,
	HLS_DISPLAY_ITEM_FUNC_BIT_FLAG_HOT		= 0x01 << HLS_DISPLAY_ITEM_FUNC_HOT,
	HLS_DISPLAY_ITEM_FUNC_BIT_FLAG_LIMITED  = 0x01 << HLS_DISPLAY_ITEM_FUNC_LIMITED,
};

const DWORD		NTL_CASHITEM_RELOAD_TIME    = 1 * 60 * 1000;    // DB 내용과 동기화 시간 조정 필요
const DWORD		NTL_MAX_CASHITEM_COUNT		= 255;				// 표시하거나 한번에 로딩할수 있는 최대 캐쉬아이템// 제한없지만 로딩에는 한정해야함
const DWORD		DBO_MAX_COUNT_HLSITEM_SUB_ITEM = 5;