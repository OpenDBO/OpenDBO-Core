#pragma once

#include "NtlPacketCommon.h"
#include "NtlPetition.h" // [ by sooshia 2008-10-21 ]

enum eOPCODE_TP
{
	TP_OPCODE_BEGIN = 60500,

	TP_NOTIFY_SERVER_BEGIN = TP_OPCODE_BEGIN,
	TP_HEARTBEAT,
	TP_PLAYER_ENTER_NFY,
	TP_PLAYER_LEAVE_NFY,
	TP_CHARACTER_WATCH_RES,

	TP_PETITION_CHAT_START_RES,
	TP_PETITION_CHAT_GM_SAY_RES, 
	TP_PETITION_CHAT_USER_SAY_REQ,
	TP_PETITION_CHAT_USER_END_NFY,
	TP_PETITION_USER_INSERT_REQ,
	TP_PETITION_SATISFACTION_NFY,
	TP_PETITION_CONTENT_MODIFY_REQ,
	TP_PETITION_USER_CANCEL_REQ,

	TP_SERVER_STATUS_RES,

	TP_OPCODE_DUMMY,
	TP_OPCODE_END = TP_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_TP(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(TP_NOTIFY_SERVER_BEGIN)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TP_HEARTBEAT)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TP_PLAYER_ENTER_NFY)
	ACCOUNTID			accountId;
	ACCOUNTID			gmAccountID;
	SERVERFARMID		serverFarmID;
	WCHAR				CharacterName[NTL_MAX_SIZE_CHAR_NAME_UNICODE+1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TP_PLAYER_LEAVE_NFY)
	ACCOUNTID			accountId;
	ACCOUNTID			gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(TP_CHARACTER_WATCH_RES)
	bool				bIsConnecting;
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
	SERVERFARMID		serverFarmID;
	WCHAR				CharacterName[NTL_MAX_SIZE_CHAR_NAME_UNICODE+1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TP_PETITION_CHAT_START_RES )
	PETITIONID			petitionID;
	WORD				wResultCode;
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TP_PETITION_CHAT_GM_SAY_RES )
	PETITIONID			petitionID;
	WORD				wResultCode;
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TP_PETITION_CHAT_USER_SAY_REQ )
	PETITIONID			petitionID;
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
	WORD				wMessageLengthInUnicode;
	SERVERFARMID		serverFarmID;
	WCHAR				awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1];
	WCHAR				CharacterName[NTL_MAX_SIZE_CHAR_NAME_UNICODE+1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TP_PETITION_CHAT_USER_END_NFY )
	ACCOUNTID			userAccountID;
	ACCOUNTID			gmAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TP_PETITION_USER_INSERT_REQ )
	WCHAR						awchUserCharName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WCHAR						awchUserID[NTL_MAX_SIZE_USERID_UNICODE + 1];
	WCHAR						awchQuestionContent[NTL_MAX_SIZE_QUESTION_CONTENT_UNICODE + 1];
	PETITIONCATEGORY			nCategory1;//ePETITIONSYSTEM_CATEGORY_1
	PETITIONCATEGORY			nCategory2;//ePETITIONSYSTEM_CATEGORY_2
	SERVERFARMID				serverFarmID;
	ACCOUNTID					userAccountID;
	CHARACTERID					charID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TP_PETITION_SATISFACTION_NFY )
	PETITIONID					petitionID;
	//ACCOUNTID					gmAccountID;
	BYTE						bySatisfactionRate;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TP_PETITION_CONTENT_MODIFY_REQ )
	PETITIONID					petitionID;
	PETITIONCATEGORY			nCategory1;//ePETITIONSYSTEM_CATEGORY_1
	PETITIONCATEGORY			nCategory2;//ePETITIONSYSTEM_CATEGORY_2
	WCHAR						awchQuestionContent[NTL_MAX_SIZE_QUESTION_CONTENT_UNICODE + 1];
	WCHAR						CharacterName[NTL_MAX_SIZE_CHAR_NAME_UNICODE+1];
	SERVERFARMID				serverFarmID;
	ACCOUNTID					userAccountID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TP_PETITION_USER_CANCEL_REQ )
	PETITIONID					petitionID;			
	ACCOUNTID					userAccountID;
	SERVERFARMID				serverFarmID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( TP_SERVER_STATUS_RES )
	ACCOUNTID				managerID;		  // GM ID
	DWORD					dwSendServerSendTick;	// Ãâ¹ßÁö ½ÃÀÛÆ½		
	DWORD					dwRecServerTick;		// µµÂøÁö µµÂøÆ½	
	DWORD					dwRecServerSendTick;	// µµÂøÁö ½ÃÀÛÆ½
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()