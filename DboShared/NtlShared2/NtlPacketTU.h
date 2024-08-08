#pragma once

#include "NtlPacketCommon.h"
#include "NtlGuild.h"
#include "NtlFriend.h"
#include "NtlRankBattle.h"
#include "NtlTimeQuest.h"
#include "NtlBudokai.h"
#include "NtlPetition.h" // [ by sooshia 2008-10-18 ]
#include "NtlDojo.h"

enum eOPCODE_TU
{
	TU_OPCODE_BEGIN = 7000,

	TU_ENTER_CHAT_RES = TU_OPCODE_BEGIN,
	TU_DISCONNECT,
	TU_DISCONNECTED_NFY,
	TU_HEARTBEAT,

	TU_SYSTEM_DISPLAY_TEXT,

	TU_CHAT_MESSAGE_SAY,
	TU_CHAT_MESSAGE_SHOUT,
	TU_CHAT_MESSAGE_WHISPER,
	TU_CHAT_MESSAGE_WHISPER_FAILED_NFY,
	TU_CHAT_MESSAGE_PARTY,
	TU_CHAT_MESSAGE_GUILD,

	// Guild Creation/Disbanding
	TU_GUILD_CREATED_NFY,						// 길드가 생성되었음을 공지
	TU_GUILD_INFO,								// 길드 정보
	TU_GUILD_MEMBER_INFO,						// 길드 멤버 정보

	// Guild Member Invitation/Leaving/Kicking Out
	TU_GUILD_DISBAND_RES,						// 길드 해체 요청 결과
	TU_GUILD_BEING_DISBANDED_NFY,				// 길드 해체 대기 상태 공지
	TU_GUILD_DISBAND_CANCEL_RES,				// 길드 해체 취소 요청 결과
	TU_GUILD_DISBAND_CANCELED_NFY,				// 길드 해체가 취소되었음을 공지
	TU_GUILD_DISBANDED_NFY,						// 실제 길드가 해체되었음을 공지
	TU_GUILD_INVITED_NFY,						// 길드에 초대받았음을 알림
	TU_GUILD_RESPONSE_INVITATION_RES,			// 길드 초대에 대한 대답 결과
	TU_GUILD_MEMBER_JOINED_NFY,					// 새 길드원 가입 공지
	TU_GUILD_INVITATION_DECLINED_NFY,			// 길드 초대를 거절
	TU_GUILD_INVITATION_EXPIRED_NFY,			// 길드 초대의 유효 시간 경과
	TU_GUILD_LEAVE_RES,							// 길드 탈퇴 요청 결과
	TU_GUILD_MEMBER_LEFT_NFY,					// 길드원 탈퇴 공지
	TU_GUILD_KICK_OUT_RES,						// 길드원의 강제 탈퇴 요청 결과
	TU_GUILD_MEMBER_KICKED_OUT_NFY,				// 길드원 강제 탈퇴 공지

	// Appointment of Guild Master/Guild Second Master
	TU_GUILD_APPOINT_SECOND_MASTER_RES,			// Second Master의 임명 결과
	TU_GUILD_SECOND_MASTER_APPOINTED_NFY,		// Second Master의 임명 공지
	TU_GUILD_DISMISS_SECOND_MASTER_RES,			// Second Master의 직위 해제 결과
	TU_GUILD_SECOND_MASTER_DISMISSED_NFY,		// Second Master의 직위 해제 공지
	TU_GUILD_CHANGE_GUILD_MASTER_RES,			// Guild Master의 이양 결과
	TU_GUILD_GUILD_MASTER_CHANGED_NFY,			// Guild Master의 변경 공지

	// Sync. of Guild Members' Info
	TU_GUILD_MEMBER_POSITION_CHANGED_NFY,		// 길드원 위치 변경 알림
	TU_GUILD_MEMBER_CLASS_CHANGED_NFY,			// 길드원 직업 변경 알림
	TU_GUILD_MEMBER_LEVEL_CHANGED_NFY,			// 길드원 레벨 변경 알림
	TU_GUILD_MEMBER_REPUTATION_CHANGED_NFY,		// 길드원 평판 변경 알림
	TU_GUILD_MEMBER_ONLINE_NFY,					// 길드원 게임 입장 알림
	TU_GUILD_MEMBER_OFFLINE_NFY,				// 길드원 게임 퇴장 알림
	
	TU_CHAT_MESSAGE_PRIVATESHOP_BUSINESS,		// 개인상점 흥정 채팅 [10/31/2007 SGpro]

	TU_FRIEND_ADD_RES,		// 친구 추가
	TU_FRIEND_DEL_RES,		// 친구 삭제
	TU_FRIEND_MOVE_RES,		// 친구 -> 블랙리스트로 이동
	TU_FRIEND_LIST_INFO,	// 친구 리스트 정보
	TU_FRIEND_BLACK_ADD_RES,		// 친구 블랙 리스트 추가
	TU_FRIEND_BLACK_DEL_RES,		// 친구 블랙 리스트 삭제
	TU_FRIEND_INFO,					// 온라인 친구 상세 정보
	TU_FRIEND_INFO_CHANGE,			// 친구 상태변경 알림

	TU_RANKBATTLE_RANK_LIST_RES,
	TU_RANKBATTLE_RANK_FIND_CHARACTER_RES,
	TU_RANKBATTLE_RANK_COMPARE_DAY_RES,	

	TU_GUILD_REPUTATION_CHANGE_NFY,					// 길드명성 변경 알림
	TU_GUILD_FUNCTION_CHANGE_NFY,
	TU_GUILD_CHANGE_NOTICE_NFY,
	TU_GUILD_CHANGE_NOTICE_RES,
	TU_GUILD_GIVE_ZENNY_NFY,
		
	TU_TMQ_RECORD_LIST_RES,
	TU_TMQ_MEMBER_LIST_RES,

	TU_BUDOKAI_NOTICE_NFY,						// 천하제일무도회 공지

	TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES,
	TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES,
	TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES,
	TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES,

	TU_PETITION_CHAT_START_REQ,						// 진정 채팅 요청 [ by sooshia 2008-10-15 ]
	TU_PETITION_CHAT_GM_SAY_REQ,				// GM이 말한 내용 [ by sooshia 2008-10-16 ]
	TU_PETITION_CHAT_USER_SAY_RES,				// [ by sooshia 2008-10-16 ]
	TU_PETITION_CHAT_GM_END_NFY,					// [ by sooshia 2008-10-17 ]
	TU_PETITION_USER_INSERT_RES,					// [ by sooshia 2008-10-18 ]
	TU_PETITION_CONTENT_MODIFY_RES,					// [ by sooshia 2008-10-18 ]
	//TU_PETITION_SATISFACTION_RES,					// [ by sooshia 2008-10-18 ]
	TU_PETITION_USER_CANCEL_RES,					// [ by sooshia 2008-10-18 ]
	TU_PETITION_FINISH_NFY,

	TU_DOJO_CREATED_NFY,
	TU_DOJO_DESTROYED_NFY,
	
	TU_DOJO_BRIEF_NFY,
	TU_DOJO_SCRAMBLE_NFY,							// 길드리더가 밖에 나갔다 왔거나 응답이 필요한 상태 일때 다시 알려준다.
	TU_DOJO_SCRAMBLE_INFORM_NFY,					// 도장전 방송 메세지
	TU_DOJO_SCRAMBLE_REJECT_NFY,					// 신청했던 유파에게 거절하였을때
	
	TU_DOJO_BUDOKAI_SEED_ADD_RES,
	TU_DOJO_BUDOKAI_SEED_DEL_RES,
	TU_DOJO_SCRAMBLE_POINT_NFY,						// 도장 점수 방송 메세지
	TU_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY,			// 도장 인장 획득 방송

	TU_DOJO_NOTICE_CHANGE_RES,
	TU_DOJO_NOTICE_CHANGE_NFY,

	TU_DOJO_SCRAMBLE_RECEIVE_NFY,					// 신청했던 유파에게 승락되었을때
	TU_DOJO_SCRAMBLE_RECEIVE_INFO_NFY,
	TU_DOJO_SCRAMBLE_REWARD_NFY,
	TU_DOJO_SCRAMBLE_RESULT_NFY,

	TU_OPCODE_END_DUMMY,
	TU_OPCODE_END = TU_OPCODE_END_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_TU(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_ENTER_CHAT_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_DISCONNECT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_DISCONNECTED_NFY)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_SYSTEM_DISPLAY_TEXT)
	WCHAR				wszGmCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE				byDisplayType;
	WORD				wMessageLengthInUnicode;
	WCHAR				wszMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_SAY)
	HOBJECT				hSubject;
	WCHAR				awchSenderCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_SHOUT)
	HOBJECT				hSubject;
	WCHAR				awchSenderCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_WHISPER)
	WCHAR				awchSenderCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_WHISPER_FAILED_NFY)
	WCHAR				awchReceiverCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_PARTY)
	HOBJECT				hSubject;
	WCHAR				awchSenderCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE				byChattingType;		// eDBO_CHATTING_TYPE
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_GUILD)
	WCHAR				wszSenderCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE				byChattingType;		// eDBO_CHATTING_TYPE
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_CREATED_NFY)
	WCHAR						wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_INFO)
	sDBO_GUILD_INFO				guildInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_INFO)
	sDBO_GUILD_MEMBER_INFO		guildMemberInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_DISBAND_RES)
	WORD						wResultCode;
	time_t						timeToDisband;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_BEING_DISBANDED_NFY)
	time_t						timeToDisband;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_DISBAND_CANCEL_RES)
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_DISBAND_CANCELED_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_DISBANDED_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_INVITED_NFY)
	WCHAR						wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_RESPONSE_INVITATION_RES)
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_JOINED_NFY)
	sDBO_GUILD_MEMBER_INFO		memberInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_INVITATION_DECLINED_NFY)
	WCHAR						wszTargetName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_INVITATION_EXPIRED_NFY)
	WCHAR						wszTargetName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_LEAVE_RES)
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_LEFT_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_KICK_OUT_RES)
	WORD						wResultCode;
	CHARACTERID					kickedOutMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_KICKED_OUT_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_APPOINT_SECOND_MASTER_RES)
	WORD						wResultCode;
	CHARACTERID					secondMasterCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_SECOND_MASTER_APPOINTED_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_DISMISS_SECOND_MASTER_RES)
	WORD						wResultCode;
	CHARACTERID					secondMasterCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_SECOND_MASTER_DISMISSED_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_CHANGE_GUILD_MASTER_RES)
	WORD						wResultCode;
	CHARACTERID					newMasterCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_GUILD_MASTER_CHANGED_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_POSITION_CHANGED_NFY)
	CHARACTERID					memberCharId;
	TBLIDX						newMapNameTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_CLASS_CHANGED_NFY)
	CHARACTERID					memberCharId;
	BYTE						byNewClass;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_LEVEL_CHANGED_NFY)
	CHARACTERID					memberCharId;
	BYTE						byNewLevel;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_REPUTATION_CHANGED_NFY)
	CHARACTERID					memberCharId;
	DWORD						dwNewReputation;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_ONLINE_NFY)
	CHARACTERID					memberCharId;
	TBLIDX						newMapNameTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_MEMBER_OFFLINE_NFY)
	CHARACTERID					memberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_CHAT_MESSAGE_PRIVATESHOP_BUSINESS)
	WCHAR				awchSenderCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_ADD_RES)
	WORD						wResultCode;
	WCHAR						wchName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_DEL_RES)
	WORD						wResultCode;
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_MOVE_RES)
	WORD						wResultCode;
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_LIST_INFO)
	BYTE						byCount;
	sFRIEND_FULL_INFO			asInfo[NTL_MAX_COUNT_FRIEND + NTL_MAX_COUNT_FRIEND];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_BLACK_ADD_RES)
	WORD						wResultCode;
	WCHAR						wchName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_BLACK_DEL_RES)
	WORD						wResultCode;
	CHARACTERID					targetID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_INFO)
	sFRIEND_INFO				sInfo;
	bool						bIsFirst;		// true : 처음 false : 나중 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_FRIEND_INFO_CHANGE)
	CHARACTERID					targetID;
	BYTE						byChangeType;	// NtlFriend.h eFRIEND_CHANGE_TYPE
	DWORD						dwChangeValue;	// Online 1:On 2:Off , Class , Level , Channel, PositionMapTblidx
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_RANKBATTLE_RANK_LIST_RES)
	WORD						wResultCode;
	DWORD						dwPage;
	BYTE						byCompareDay;

	BYTE						byRankInfoCount;
	WORD						wRankInfo;		// sRANKBATTLE_RANK_INFO []

	sVARIABLE_DATA				sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_RANKBATTLE_RANK_FIND_CHARACTER_RES)
	WORD						wResultCode;
	DWORD						dwPage;
	BYTE						byCompareDay;

	WCHAR						wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];

	BYTE						byRankInfoCount;
	WORD						wRankInfo;		// sRANKBATTLE_RANK_INFO asRankInfo []

	sVARIABLE_DATA				sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_RANKBATTLE_RANK_COMPARE_DAY_RES)
	WORD						wResultCode;
	DWORD						dwPage;
	BYTE						byCompareDay;

	bool						bIsFullList;	// true = wRankInfo 참조 | false = wCompareInfo 참조
	BYTE						byInfoCount;
	union
	{
		WORD					wRankInfo;		// sRANKBATTLE_RANK_INFO asRankInfo []
		WORD					wCompareInfo;	// DWORD adwCompareRank []
	};

	sVARIABLE_DATA				sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_REPUTATION_CHANGE_NFY)
	DWORD						dwReputation;
	DWORD						dwMaxGuildPointEver;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_FUNCTION_CHANGE_NFY)
	DWORD64						qwGuildFunctionFlag;
	BYTE						byNewFunc;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_CHANGE_NOTICE_NFY)
	WCHAR						awchCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WORD						wNoticeLengthInUnicode;	
	WCHAR						awchNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_CHANGE_NOTICE_RES)
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_GUILD_GIVE_ZENNY_NFY)
	CHARACTERID					charId; 
	DWORD						dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_TMQ_RECORD_LIST_RES )
	WORD						wResultCode;
	TBLIDX						tmqTblidx;
	BYTE						byDifficult;
	BYTE						byTeamCount;
	sTIMEQUEST_TEAM_RANK_DATA	asTeam[DBO_TIMEQUEST_DEFAULT_TOTAL_RANK_COUNT];		// 용량 주의
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_TMQ_MEMBER_LIST_RES )
	WORD						wResultCode;
	TBLIDX						tmqTblidx;
	BYTE						byDifficult;
	BYTE						byRank;
	sTIMEQUEST_TEAM_MEMBER_DATA	sTeam;		
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_BUDOKAI_NOTICE_NFY)
	BYTE			byNoticeType;		// eBUDOKAI_NOTICE_TYPE

	union
	{
		// byNoticeType == BUDOKAI_NOTICE_SYSTEM 사용
		TBLIDX		tblidxNotice;		

		// byNoticeType == BUDOKAI_NOTICE_GM 사용
		struct sGM_NOTICE
		{
			BYTE		byNoticeLength;
			WCHAR		wszNotice[BUDOKAI_MAX_NOTICE_LENGTH + 1];
		};
		sGM_NOTICE	sGmNotice;
	};
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_RES )
	WORD									wResultCode;
	WORD									wEntryTeam;			// sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR	asEntryTeam[BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER];
	BYTE									byMatchDataCount;
	WORD									wMatchData;			// sBUDOKAI_TOURNAMENT_MATCH_DATA aMatchData[BUDOKAI_MAJOR_MATCH_PLAYER_NUMBER * 2 - 1];
	sVARIABLE_DATA							sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_RES )
	WORD										wResultCode;
	TEAMTYPE									awTeamType[2];		// sBUDOKAI_TOURNAMENT_INDIVIDUAL_ENTRY_DATA_VAR asEntryData[2];
	sVARIABLE_DATA								sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_BUDOKAI_TOURNAMENT_TEAM_LIST_RES )
	WORD									wResultCode;
	WORD									wEntryTeam;			// sBUDOKAI_TOURNAMENT_ENTRY_TEAM_INFO_VAR	asEntryTeam[BUDOKAI_MAJOR_MATCH_TEAM_NUMBER];
	BYTE									byMatchDataCount;
	WORD									wMatchData;			// sBUDOKAI_TOURNAMENT_MATCH_DATA aMatchData[BUDOKAI_MAJOR_MATCH_TEAM_NUMBER * 2 - 1];
	sVARIABLE_DATA							sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_BUDOKAI_TOURNAMENT_TEAM_INFO_RES )
	WORD								wResultCode;
	TEAMTYPE							awTeamType[2];			//	sBUDOKAI_TOURNAMENT_TEAM_ENTRY_DATA_VAR	asEntryData[2];
	sVARIABLE_DATA						sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_CHAT_START_REQ )
	PETITIONID			petitionID;
	bool				bNeedSatisfactionRate;
	ACCOUNTID			gmAccountID;
	WCHAR				awchGMCharName_Consult[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_CHAT_GM_SAY_REQ )
	ACCOUNTID			gmAccountID;
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_CHAT_USER_SAY_RES )
	WORD				wResultCode;
	//ACCOUNTID			gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_CHAT_GM_END_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_USER_INSERT_RES )
	WORD				wResultCode;
	PETITIONID			petitionID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_CONTENT_MODIFY_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
//BEGIN_PROTOCOL( TU_PETITION_SATISFACTION_RES )
//	WORD				wResultCode;
//END_PROTOCOL()
////----------------------------------------------------------------
BEGIN_PROTOCOL( TU_PETITION_USER_CANCEL_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_PETITION_FINISH_NFY )
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_CREATED_NFY )
	sDBO_DOJO_DATA		sDojoData;	
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_DESTROYED_NFY )
	TBLIDX				dojoTblidx;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_BRIEF_NFY )
	sDBO_DOJO_BRIEF		sDojoData;	
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_NFY )			// 길드리더가 밖에 나갔다 왔거나 응답이 필요한 상태 일때 다시 알려준다.
	WCHAR				wszName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1]; // 신청길드명
END_PROTOCOL()
//------------------------------------------------------------------	
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_INFORM_NFY )
	TBLIDX						dojoTblidx;
	BYTE						byState;	// 도장쟁탈전 상태
	DOJOTIME					tmNextStepTime;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_REJECT_NFY )   // 신청했던 도장이 거절했을때
END_PROTOCOL()
//------------------------------------------------------------------	
BEGIN_PROTOCOL(TU_DOJO_BUDOKAI_SEED_ADD_RES)
	WORD				wResultCode;
	TBLIDX				dojoTblidx;
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];// 등록할 character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TU_DOJO_BUDOKAI_SEED_DEL_RES)	
	WORD				wResultCode;
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];// 삭제할 character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_POINT_NFY )
	sDOJO_SCRAMBLE_POINT sData;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_SHIEFT_SEAL_STATE_NFY )
	TBLIDX								dojoTblidx;
	TBLIDX								objectTblidx;
	GUILDID								guildId;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_NOTICE_CHANGE_RES )
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_DOJO_NOTICE_CHANGE_NFY )
	TBLIDX						dojoTblidx;
	WCHAR						awchCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WORD						wNoticeLengthInUnicode;	
	WCHAR						awchNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_RECEIVE_NFY )
	TBLIDX								dojoTblidx;
	GUILDID								guildId;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_RECEIVE_INFO_NFY )
	TBLIDX								dojoTblidx;
	GUILDID								defGuildId;
	GUILDID								attGuildId;
	WCHAR								wszDefGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
	WCHAR								wszAttGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
	sDBO_GUILD_MARK						sDefMark;
	sDBO_GUILD_MARK						sAttMark;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_REWARD_NFY )
	DWORD						dwAddGuildReputation;
	TBLIDX						itemTblidx;
	BYTE						byCount;
END_PROTOCOL()
//------------------------------------------------------------------ 
BEGIN_PROTOCOL( TU_DOJO_SCRAMBLE_RESULT_NFY )
	GUILDID						winGuildId;	
END_PROTOCOL()
//------------------------------------------------------------------ 
#pragma pack()
