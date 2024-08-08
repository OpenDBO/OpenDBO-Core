#pragma once

#include "NtlPacketCommon.h"
#include "NtlSystemTool.h"
#include "NtlPetition.h"


enum eOPCODE_PT
{
	PT_OPCODE_BEGIN = 60000,

	PT_HEARTBEAT = PT_OPCODE_BEGIN,

	PT_REPEAT_NOTICE_REPLACE,						// 공지 추가/변경
	PT_REPEAT_NOTICE_DELETE,						// 공지 삭제
	PT_REALTIME_NOTICE,								// 실시간 공지

	PT_GM_GROUP_AUTH_NFY,
	PT_GM_GROUP_AUTH_DEL_NFY,
	PT_CHARACTER_WATCH_REQ,							// [ by sooshia 2008-09-18 ]
	PT_CHARACTER_WATCH_END_NFY,						// [ by sooshia 2008-09-22 ]

	PT_PETITION_CHAT_START_REQ,							// [ by sooshia 2008-10-15 ]
	PT_PETITION_CHAT_GM_SAY_REQ,					// [ by sooshia 2008-10-16 ]
	PT_PETITION_CHAT_USER_SAY_RES,					// [ by sooshia 2008-10-16 ]
	PT_PETITION_CHAT_GM_END_NFY,					// [ by sooshia 2008-10-18 ]
	PT_PETITION_FINISH_NFY,
	PT_PETITION_USER_CANCEL_RES,
	PT_PETITION_USER_INSERT_RES,
	PT_PETITION_CONTENT_MODIFY_RES,

	PT_SERVER_STATUS_REQ,

	PT_OPCODE_DUMMY,
	PT_OPCODE_END = PT_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_PT(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(PT_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(PT_REPEAT_NOTICE_REPLACE)
	sGM_REPEAT_NOTICE_ENTITY	sNotice;		// 주기적 공지의 데이터
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(PT_REPEAT_NOTICE_DELETE)
	DWORD						dwSerial;			// 삭제할 주기적 공지의 시리얼
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(PT_REALTIME_NOTICE)
	BYTE			byNoticeType;			// eSERVER_TEXT_TYPE
	BYTE			byChannelID;
	WCHAR			wszNotice[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE+1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PT_GM_GROUP_AUTH_NFY )			// 커뮤니티서버가 켜지거나 그룹권한이 변경 되었을때
	DWORD					dwGroupCount;
	sGM_GROUP_ENTITY		aGroupList[NTL_MAX_COUNT_GM_GROUP];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PT_GM_GROUP_AUTH_DEL_NFY )			// 그룹권한이 삭제 되었을때
	DWORD					dwGroupSerial;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PT_CHARACTER_WATCH_REQ )
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PT_CHARACTER_WATCH_END_NFY )
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PT_PETITION_CHAT_START_REQ )
	PETITIONID			petitionID;
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
	bool				bNeedSatisfactionRate;
	WCHAR				awchGMCharName_Consult[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PT_PETITION_CHAT_GM_SAY_REQ )
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PT_PETITION_CHAT_USER_SAY_RES )
	WORD				wResultCode;
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PT_PETITION_CHAT_GM_END_NFY )
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PT_PETITION_FINISH_NFY )
	ACCOUNTID			userAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( PT_PETITION_USER_CANCEL_RES )
	WORD				wResultCode;
	ACCOUNTID			userAccountID;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( PT_PETITION_USER_INSERT_RES )
	WORD				wResultCode;
	ACCOUNTID			userAccountID;
	PETITIONID			petitionID;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( PT_PETITION_CONTENT_MODIFY_RES )
	WORD				wResultCode;
	ACCOUNTID			userAccountID;
	//PETITIONID			petitionID;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( PT_SERVER_STATUS_REQ )
	ACCOUNTID				managerID;		      // GM ID
	DWORD					dwSendServerSendTick;	// 출발지 시작틱	
	DWORD					dwRecServerTick;		// 도착지 도착틱
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()