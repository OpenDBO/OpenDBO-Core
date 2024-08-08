#pragma once

#include "NtlParty.h"

#include "NtlPacketCommon.h"
#include "NtlGuild.h"
#include "NtlDojo.h"

enum eOPCODE_TQ
{
	TQ_OPCODE_BEGIN = 57000,

	TQ_GUILD_CREATE_REQ = TQ_OPCODE_BEGIN,
	TQ_GUILD_DATA_REQ,
	TQ_GUILD_DISBAND_REQ,				// 길드를 해체 대기 상태로 만들기
	TQ_GUILD_DISBAND_CANCEL_REQ,		// 길드 해체 대기 상태를 취소하기
	TQ_GUILD_DESTROY_REQ,				// 길드를 실제로 삭제하기

	TQ_GUILD_INVITE_REQ,				// 길드 초대
	TQ_GUILD_LEAVE_REQ,					// 길드 탈퇴
	TQ_GUILD_KICK_OUT_REQ,				// 길드 멤버 강제 탈퇴

	TQ_GUILD_APPOINT_SECOND_MASTER_REQ,		// 길드 Second Master 임명
	TQ_GUILD_DISMISS_SECOND_MASTER_REQ,		// 길드 Second Master 직위 해제
	TQ_GUILD_CHANGE_GUILD_MASTER_REQ,		// 길드 Master의 이양

	TQ_FRIEND_ADD_REQ,			// 친구 추가
	TQ_FRIEND_DEL_REQ,			// 친구 삭제
	TQ_FRIEND_MOVE_REQ,			// 친구 -> 블랙리스트로 이동
	TQ_FRIEND_LIST_LOAD_REQ,	// 친구 로드
	TQ_FRIEND_BLACK_ADD_REQ,		// 친구 블랙 리스트 추가
	TQ_FRIEND_BLACK_DEL_REQ,		// 친구 블랙 리스트 삭제

	TQ_RANKBATTLE_RANK_LIST_REQ,			//
	TQ_RANKBATTLE_RANK_FIND_CHARACTER_REQ,	//
	
	TQ_GUILD_FUNCTION_ADD_REQ,
	TQ_GUILD_GIVE_ZENNY_REQ,
	TQ_GUILD_CHANGE_NOTICE_REQ,
	TQ_GUILD_CREATE_MARK_REQ,							// GuildMark Create
	TQ_GUILD_CHANGE_MARK_REQ,							// GuildMark Change
	TQ_GUILD_CHANGE_REPUTATION_REQ,
	TQ_GUILD_CHANGE_NAME_REQ,

	TQ_TMQ_RECORD_LIST_REQ,
	TQ_TMQ_MEMBER_LIST_REQ,

	TQ_DOJO_DATA_REQ,
	TQ_DOJO_CREATE_REQ,
	TQ_DOJO_UPDATE_REQ,
	TQ_DOJO_DEL_REQ,
	TQ_DOJO_FUNCTION_ADD_REQ,
	TQ_DOGI_CREATE_REQ,
	TQ_DOGI_CHANGE_REQ,

	TQ_CONNECT_WAIT_CHECK_RES,
	TQ_CONNECT_WAIT_COUNT_NFY,
	TQ_CONNECT_WAIT_CANCEL_RES,
	
	TQ_DOJO_SCRAMBLE_REQ,
	TQ_DOJO_SCRAMBLE_REJECT_REQ,
	TQ_DOJO_SCRAMBLE_RESET,

	TQ_DOJO_BUDOKAI_SEED_ADD_REQ,
	TQ_DOJO_BUDOKAI_SEED_DEL_REQ,
	TQ_DOJO_SCRAMBLE_REWARD_REQ,
	TQ_DOJO_CHANGE_NOTICE_REQ,

	TQ_OPCODE_END_DUMMY,
	TQ_OPCODE_END = TQ_OPCODE_END_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_TQ(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_CREATE_REQ)
	BYTE				byServerChannelIndex;
	WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
	DWORD64				qwGuildFunctionFlag;
	DWORD				dwRequiredZenny;
	ACCOUNTID			creatorAccountId;
	PARTYID				partyId;
	// 'memberCharId[0]' is a leader's CHARACTERID.
	// 'memberCharId[0]'이 길드 리더의 CHARACTERID이다.
	// by YOSHIKI(2007-03-27)
	BYTE				byMemberCount;
	CHARACTERID			memberCharId[NTL_MAX_MEMBER_IN_PARTY];
	DWORD				dwMaxGuildPointEver;		// 파티원의 명성치 합 + 길드명성
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_DATA_REQ)
	CHARACTERID			requesterCharId;
	GUILDID				guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_DISBAND_REQ)
	GUILDID				guildId;
	time_t				timeToDisband;
	bool				bByUser;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_DISBAND_CANCEL_REQ)
	GUILDID				guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_DESTROY_REQ)
	GUILDID				guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_INVITE_REQ)
	CHARACTERID			inviteeCharId;
	GUILDID				guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_LEAVE_REQ)
	CHARACTERID			leavingMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_KICK_OUT_REQ)
	CHARACTERID			kickedOutMemberCharId;
	GUILDID				guildId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_APPOINT_SECOND_MASTER_REQ)
	GUILDID				guildId;
	CHARACTERID			targetMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_DISMISS_SECOND_MASTER_REQ)
	GUILDID				guildId;
	CHARACTERID			targetMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_CHANGE_GUILD_MASTER_REQ)
	GUILDID				guildId;
	CHARACTERID			targetMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_FRIEND_ADD_REQ)	// 친구 추가
	CHARACTERID			charID;
	WCHAR				wchName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_FRIEND_DEL_REQ)	// 친구 삭제
	CHARACTERID			charID;
	CHARACTERID			targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_FRIEND_MOVE_REQ)	// 친구 -> 블랙리스트로 이동
	CHARACTERID			charID;
	CHARACTERID			targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_FRIEND_LIST_LOAD_REQ)	// 친구 로딩
	CHARACTERID			charID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_FRIEND_BLACK_ADD_REQ)	// 친구 블랙 리스트 추가
	CHARACTERID			charID;
	WCHAR				wchName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_FRIEND_BLACK_DEL_REQ)	// 친구 블랙 리스트 삭제
	CHARACTERID			charID;
	CHARACTERID			targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_RANKBATTLE_RANK_LIST_REQ)
	CHARACTERID			charId;					// -- 커뮤니티 서버에서 사용
	BYTE				byCompareDay;			// 비교일 -- 커뮤니티 서버에서 사용
	bool				bIsCompareRankReq;		// CompareRank 변경을 위한 요청인가? -- 커뮤니티 서버에서 사용

	DWORD				dwPage;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_RANKBATTLE_RANK_FIND_CHARACTER_REQ)	// 친구 로딩
	CHARACTERID			charId;										// 요청한 character	-- 커뮤니티 서버에서 사용
	BYTE				byCompareDay;								// 비교일 -- 커뮤니티 서버에서 사용

	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];// 찾을 character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_FUNCTION_ADD_REQ)
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	CHARACTERID			masterCharId;
	GUILDID				guildId;
	DWORD				dwZenny;
	DWORD64				qwGuildFunctionFlag;
	BYTE				byFunction;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_GIVE_ZENNY_REQ)
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	CHARACTERID			charId;
	GUILDID				guildId;
	DWORD				dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_CHANGE_NOTICE_REQ)
	CHARACTERID			charId;
	GUILDID				guildId;
	WCHAR				awchCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WORD				wNoticeLengthInUnicode;	
	WCHAR				awchNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_GUILD_CREATE_MARK_REQ )
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	CHARACTERID			charId;
	GUILDID				guildId;
	sDBO_GUILD_MARK		sMark;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_GUILD_CHANGE_MARK_REQ )
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	CHARACTERID			charId;
	GUILDID				guildId;
	sDBO_GUILD_MARK		sMark;
	DWORD				dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_GUILD_CHANGE_REPUTATION_REQ)
	GUILDID				guildId;
	DWORD				dwReputation;
	DWORD				dwMaxGuildPointEver;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_GUILD_CHANGE_NAME_REQ )
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	CHARACTERID			charId;
	GUILDID				guildId;
	WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
	DWORD				dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_TMQ_RECORD_LIST_REQ )
	CHARACTERID			charId;
	TBLIDX				tmqTblidx;
	BYTE				byDifficult;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_TMQ_MEMBER_LIST_REQ )
	CHARACTERID			charId;
	TBLIDX				tmqTblidx;
	BYTE				byDifficult;
	BYTE				byRank;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_DATA_REQ)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_CREATE_REQ)
	BYTE				byServerChannelId;
	GUILDID				guildId;
	CHARACTERID			charId;
	TBLIDX				dojoTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_UPDATE_REQ)
	BYTE				byServerChannelId;
	sDBO_DOJO_DATA		sDojoData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_DEL_REQ)
	BYTE				byServerChannelId;
	GUILDID				guildId;
	CHARACTERID			charId;
	TBLIDX				dojoTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_FUNCTION_ADD_REQ)
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	CHARACTERID			masterCharId;
	GUILDID				guildId;
	DWORD				dwZenny;
	DWORD64				qwGuildFunctionFlag;
	BYTE				byFunction;
	BYTE				byPlace;
	BYTE				byPos;
	BYTE				byStack;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_DOGI_CREATE_REQ )
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	CHARACTERID			charId;
	sDBO_DOGI_DATA		sDogi;
	bool				bIsDojo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_DOGI_CHANGE_REQ )
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	CHARACTERID			charId;
	sDBO_DOGI_DATA		sDogi;
	DWORD				dwZenny;
	bool				bIsDojo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_CONNECT_WAIT_CHECK_RES )
	WORD			wResultCode;
	ACCOUNTID		accountID;
	SERVERCHANNELID gameServerChannelID;
	SERVERINDEX		characterServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_CONNECT_WAIT_COUNT_NFY )
	ACCOUNTID		accountID;
	SERVERINDEX		characterServerIndex;
	DWORD			dwTicketNumber;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_CONNECT_WAIT_CANCEL_RES )
	WORD			wResultCode;
	ACCOUNTID		accountId;
	BYTE			byCharacterServerIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_SCRAMBLE_REQ)
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	GUILDID				guildId;			// 신청길드
	CHARACTERID			charId;
	TBLIDX				dojoTblidx;		
	DWORD				dwReqZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_SCRAMBLE_REJECT_REQ)
	BYTE				byServerChannelIndex;
	BYTE				byServerIndex;
	GUILDID				guildId;			// 신청길드
	TBLIDX				dojoTblidx;		
	DWORD				dwReqZenny;
	CHARACTERID			leader;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_SCRAMBLE_RESET)
	TBLIDX				dojoTblidx;		
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_BUDOKAI_SEED_ADD_REQ)
	CHARACTERID			charId;
	TBLIDX				dojoTblidx;
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];// 등록할 character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_BUDOKAI_SEED_DEL_REQ)	
	CHARACTERID			charId;
	TBLIDX				dojoTblidx;
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];// 삭제할 character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TQ_DOJO_SCRAMBLE_REWARD_REQ )				
	TBLIDX				dojoTblidx;
	GUILDID				guildId;// winguild
	DWORD				dwAddGuildReputation;
	DWORD64				qwGuildFunctionFlag;
	sITEM_DATA			sItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TQ_DOJO_CHANGE_NOTICE_REQ)
	CHARACTERID			charId;
	TBLIDX				dojoTblidx;
	WCHAR				awchCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WORD				wNoticeLengthInUnicode;	
	WCHAR				awchNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
#pragma pack()
