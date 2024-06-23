#pragma once

#include "NtlPacketCommon.h"
#include "NtlPetition.h"


enum eOPCODE_UT
{
	UT_OPCODE_BEGIN = 6000,


	UT_ENTER_CHAT = 6001,
	UT_CHAT_MESSAGE_SAY,
	UT_CHAT_MESSAGE_SHOUT,
	UT_CHAT_MESSAGE_WHISPER,
	UT_CHAT_MESSAGE_PARTY,
	UT_CHAT_MESSAGE_GUILD,
	//New Packets
	UT_CHAT_MESSAGE_TRADE,
	UT_CHAT_MESSAGE_FIND_PARTY,
	UT_CHAT_USER_ON_CHANNEL_UPDATE_NFY,

	UT_GUILD_DISBAND_REQ,                               // 길드 해체하기
	UT_GUILD_DISBAND_CANCEL_REQ,                        // 길드 해체 취소하기
	UT_GUILD_RESPONSE_INVITATION,                       // 길드 초대에 대한 대답 보내기
	UT_GUILD_LEAVE_REQ,                                 // 길드 탈퇴하기
	UT_GUILD_KICK_OUT_REQ,                              // 길드 멤버 강제로 탈퇴시키기
	UT_GUILD_APPOINT_SECOND_MASTER_REQ,                 // 길드 멤버를 Second Master로 임명하기
	UT_GUILD_DISMISS_SECOND_MASTER_REQ,                 // 길드 Second Master의 직위를 해제하기
	UT_GUILD_CHANGE_GUILD_MASTER_REQ,                   // 길드 마스터를 이양하기

	UT_CHAT_MESSAGE_PRIVATESHOP_BUSINESS,               // 개인상점 흥정 대화 [10/31/2007 SGpro]

	UT_FRIEND_ADD_REQ,      // 친구 추가
	UT_FRIEND_DEL_REQ,      // 친구 삭제
	UT_FRIEND_MOVE_REQ,     // 친구 -> 블랙리스트로 이동
	UT_FRIEND_BLACK_ADD_REQ,        // 친구 블랙 리스트 추가
	UT_FRIEND_BLACK_DEL_REQ,        // 친구 블랙 리스트 삭제

	UT_RANKBATTLE_RANK_LIST_REQ,
	UT_RANKBATTLE_RANK_FIND_CHARACTER_REQ,
	UT_RANKBATTLE_RANK_COMPARE_DAY_REQ,

	UT_GUILD_CHANGE_NOTICE_REQ,
	UT_TMQ_RECORD_LIST_REQ,
	UT_TMQ_MEMBER_LIST_REQ,

	UT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ,
	UT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ,
	UT_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ,
	UT_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ,

	UT_PETITION_CHAT_START_RES,                         // [ by sooshia 2008-10-15 ]
	UT_PETITION_CHAT_USER_SAY_REQ,                      // 유저가 말한 내용 [ by sooshia 2008-10-16 ]
	UT_PETITION_CHAT_USER_END_NFY,                      // [ by sooshia 2008-10-17 ]
	UT_PETITION_CHAT_GM_SAY_RES,                        // [ by sooshia 2008-10-18 ]
	UT_PETITION_USER_INSERT_REQ,                        // [ by sooshia 2008-10-18 ]
	UT_PETITION_CONTENT_MODIFY_REQ,                     // [ by sooshia 2008-10-18 ]
	UT_PETITION_SATISFACTION_NFY,
	UT_PETITION_USER_CANCEL_REQ,

	UT_DOJO_BUDOKAI_SEED_ADD_REQ,
	UT_DOJO_BUDOKAI_SEED_DEL_REQ,
	UT_DOJO_NOTICE_CHANGE_REQ,
	//New Packets
	UT_HLS_SLOT_MACHINE_INFO_REQ,
	UT_HLS_SLOT_MACHINE_EXTRACT_REQ,
	UT_HLS_SLOT_MACHINE_WINNER_INFO_REQ,


	UT_OPCODE_END_DUMMY,
	UT_OPCODE_END = UT_OPCODE_END_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_UT(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_ENTER_CHAT)
	ACCOUNTID			accountId;
	BYTE				abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
	DWORD				dwOnChannelBitFlag; //new
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_CHAT_MESSAGE_SAY)
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_CHAT_MESSAGE_SHOUT)
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_CHAT_MESSAGE_WHISPER)
	WCHAR				awchReceiverCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_CHAT_MESSAGE_PARTY)
	BYTE				byChattingType;		// eDBO_CHATTING_TYPE
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_CHAT_MESSAGE_GUILD)
	BYTE				byChattingType;		// eDBO_CHATTING_TYPE
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_CHAT_MESSAGE_TRADE)
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_CHAT_MESSAGE_FIND_PARTY)
	WORD				wMessageLengthInUnicode;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_CHAT_USER_ON_CHANNEL_UPDATE_NFY)
	DWORD			dwUserOnChannelFlag;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_GUILD_DISBAND_REQ)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_GUILD_DISBAND_CANCEL_REQ)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_GUILD_RESPONSE_INVITATION)
	BYTE				byResponse;		// eNTL_INVITATION_RESPONSE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_GUILD_LEAVE_REQ)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_GUILD_KICK_OUT_REQ)
CHARACTERID			targetMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_GUILD_APPOINT_SECOND_MASTER_REQ)
	CHARACTERID			targetMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_GUILD_DISMISS_SECOND_MASTER_REQ)
	CHARACTERID			targetMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_GUILD_CHANGE_GUILD_MASTER_REQ)
	CHARACTERID			targetMemberCharId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_FRIEND_ADD_REQ)	// Ä£±¸ Ãß°¡
	WCHAR				wchName[NTL_MAX_SIZE_CHAR_NAME + 1];	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_FRIEND_DEL_REQ)	// Ä£±¸ »èÁ¦
	CHARACTERID			targetID;
	WCHAR				wchName[NTL_MAX_SIZE_CHAR_NAME + 1];//new
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_FRIEND_MOVE_REQ)	// Ä£±¸ -> ºí·¢¸®½ºÆ®·Î ÀÌµ¿
	CHARACTERID			targetID;
	WCHAR				wchName[NTL_MAX_SIZE_CHAR_NAME + 1];//new
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UT_FRIEND_BLACK_ADD_REQ )	// Ä£±¸ ºí·¢ ¸®½ºÆ® Ãß°¡
	WCHAR				awchName[NTL_MAX_SIZE_CHAR_NAME + 1];	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UT_FRIEND_BLACK_DEL_REQ )	// Ä£±¸ ºí·¢ ¸®½ºÆ® »èÁ¦
	CHARACTERID			targetID;
	WCHAR				wchName[NTL_MAX_SIZE_CHAR_NAME + 1];//new
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_RANKBATTLE_RANK_LIST_REQ)
	DWORD				dwPage;
	BYTE				byCompareDay;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_RANKBATTLE_RANK_FIND_CHARACTER_REQ)
	DWORD				dwPage;
	BYTE				byCompareDay;
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];// Ã£À» character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_RANKBATTLE_RANK_COMPARE_DAY_REQ)
	DWORD				dwPage;
	BYTE				byCompareDay;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_GUILD_CHANGE_NOTICE_REQ)
	WORD				wNoticeLengthInUnicode;	
	WCHAR				awchNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_TMQ_RECORD_LIST_REQ)
	TBLIDX						tmqTblidx;
	BYTE						byDifficult;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_TMQ_MEMBER_LIST_REQ)
	TBLIDX						tmqTblidx;
	BYTE						byDifficult;
	BYTE						byRank;		// 0 : Best 1~5: ÇöÀç·©Å·
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_BUDOKAI_TOURNAMENT_INDIVIDUAL_LIST_REQ)
	// Initial value: INVALID_BYTE - Send the JoinId of the finalists together
	// The number of lists (game information) stored in the client
	BYTE						byReceivedListCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_BUDOKAI_TOURNAMENT_INDIVIDUAL_INFO_REQ)
	WORD						awJoinId[2];
END_PROTOCOL()
//------------------------------------------------------------------
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_BUDOKAI_TOURNAMENT_TEAM_LIST_REQ)
	// Initial value: INVALID_BYTE - Send the JoinId of the finalists together
	// The number of lists (game information) stored in the client
	BYTE						byReceivedListCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_BUDOKAI_TOURNAMENT_TEAM_INFO_REQ)
	WORD						awJoinId[2];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UT_PETITION_CHAT_START_RES )
	WORD						wResultCode;
	ACCOUNTID					gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UT_PETITION_CHAT_USER_SAY_REQ )
	ACCOUNTID					gmAccountID;
	WORD						wMessageLengthInUnicode;
	WCHAR						awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UT_PETITION_CHAT_USER_END_NFY )
	ACCOUNTID					gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UT_PETITION_CHAT_GM_SAY_RES )
	WORD						wResultCode;
	ACCOUNTID					gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UT_PETITION_USER_INSERT_REQ )
	WCHAR						awchUserCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WCHAR						awchQuestionContent[NTL_MAX_SIZE_QUESTION_CONTENT + 1];
	PETITIONCATEGORY			nCategory1;//ePETITIONSYSTEM_CATEGORY_1
	PETITIONCATEGORY			nCategory2;//ePETITIONSYSTEM_CATEGORY_2
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UT_PETITION_CONTENT_MODIFY_REQ )
	PETITIONID					petitionID;
	WCHAR						awchQuestionContent[NTL_MAX_SIZE_QUESTION_CONTENT + 1];
	PETITIONCATEGORY			nCategory1;//ePETITIONSYSTEM_CATEGORY_1
	PETITIONCATEGORY			nCategory2;//ePETITIONSYSTEM_CATEGORY_2
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UT_PETITION_SATISFACTION_NFY )
	PETITIONID					petitionID;
	ACCOUNTID					gmAccountID;
	BYTE						bySatisfactionRate;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UT_PETITION_USER_CANCEL_REQ )
	PETITIONID					petitionID;
	ACCOUNTID					gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_DOJO_BUDOKAI_SEED_ADD_REQ)
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];// µî·ÏÇÒ character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_DOJO_BUDOKAI_SEED_DEL_REQ)	
	WCHAR				wszCharName[NTL_MAX_SIZE_CHAR_NAME + 1];// »èÁ¦ÇÒ character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_DOJO_NOTICE_CHANGE_REQ)
	WORD				wNoticeLengthInUnicode;	
	WCHAR				awchNotice[NTL_MAX_LENGTH_OF_DOJO_NOTICE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_HLS_SLOT_MACHINE_INFO_REQ)
	BYTE				byType;	
	BYTE				byPage;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_HLS_SLOT_MACHINE_EXTRACT_REQ)
	WORD				wMachineIndex;	
	BYTE				byExtractCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UT_HLS_SLOT_MACHINE_WINNER_INFO_REQ)
	WORD				wMachineIndex;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()
