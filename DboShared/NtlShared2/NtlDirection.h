//***********************************************************************************
//
//	File		:	NtlDirection.h
//
//	Begin		:	2007-06-21
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

//-----------------------------------------------------------------------------------
// 클라이언트 연출 타입
//-----------------------------------------------------------------------------------
enum eDIRECTION_TYPE
{
	DIRECTION_FLASH, // 2D 연출
	DIRECTION_CINEMATIC, // 씨네마틱 연출
	DIRECTION_PLAY, // 서버에서 지정한 애니메이션등의 연출 (씨네마틱과는 달리 동기화 된다 ) -> 삭제 예정

	MAX_DIECTION_TYPE,
	INVALID_DIRECTION_TYPE = 0xFF,
};

//-----------------------------------------------------------------------------------
// 캐릭터 연출 타입 ( 상태레벨의 동기화 )
//-----------------------------------------------------------------------------------
enum eDIRECT_PLAY_TYPE
{
	DIRECT_PLAY_TQS_START,
	DIRECT_PLAY_TQS_ARRIVE,
	DIRECT_PLAY_TQS_LEAVE,

	DIRECT_PLAY_NORMAL,

	DIRECT_PLAY_VEHICLE,

	MAX_DIRECT_PLAY_TYPE,
	INVALID_DIRECT_PLAY_TYPE = 0xFF,
};

//-----------------------------------------------------------------------------------
// 방송(트렁크스 무전기 메시지 타입 )
//-----------------------------------------------------------------------------------
enum eTELECAST_MESSAGE_TYPE
{
	TELECAST_MESSAGE_TYPE_NORMAL,
	TELECAST_MESSAGE_TYPE_WARNING,
	TELECAST_MESSAGE_TYPE_DANGER,
    TELECAST_MESSAGE_TYPE_NOTIFY,               ///< 서버에서 날라오는 통지 메시지 (클라이언트내에서만 사용)

	MAX_TELECAST_MESSAGE_TYPE,
	INVALID_TELECAST_MESSAGE_TYPE = 0xFF,
};

//-----------------------------------------------------------------------------------
// 진행 메시지 ( TMQ등에서 현재 진행상황을 안내하는 메시지 )
//-----------------------------------------------------------------------------------
enum ePROGRESS_MESSAGE_TYPE
{
	PROGRESS_MESSAGE_TYPE_TMQ,
	PGOGRESS_MESSAGE_TYPE_TMQ_STAGE,

	MAX_PROGRESS_MESSAGE_TYPE,
	INVALID_PROGRESS_MESSAGE_TYPE = 0xFF,
};

//-----------------------------------------------------------------------------------
// 방향 지시 메시지
//-----------------------------------------------------------------------------------
enum eDIRECTION_INDICATE_TYPE
{
	DIRECTION_INDICATE_TYPE_TOBJECT, // 오브젝트를 지시한다
	DIRECTION_INDICATE_TYPE_NPC, // npc를 지시한다
	DIRECTION_INDICATE_TYPE_POINT, // 하나의 지점을 지시한다

	MAX_DIRECTION_INDICATE_TYPE,
	INVALID_DIRECTION_INDICATE_TYPE = 0xFF,
};