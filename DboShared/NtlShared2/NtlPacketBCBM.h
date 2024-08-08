#pragma once
//***********************************************************************************
//
//	File		:	NtlPacketBCBM.h
//
//	Begin		:	2007-08-09 오전 11:29:12
//
//	Copyright	:	ⓒ NTL-Inc Co. Ltd
//
//	strAuthor	:	Seung Goon. Chi   ( sgpro@ntl-inc.com )
//
//	strDesc		:	
//
//***********************************************************************************

//-- include ------------------------------------------------------------------------
#include "NtlPacketCommon.h"
#include "NtlSharedBotSystemDef.h"
//-----------------------------------------------------------------------------------

enum eOPCODE_BCBM
{
	BCBM_OPCODE_BEGIN = 64200,
	BCBM_ENTER_REQ,
	BCBM_SERVERDATA_REQ,
	BCBM_BOTCOMMAND_RES,
	
	BCBM_PROFILE_BEGIN,
	BCBM_PROFILE_DATA,
	BCBM_PROFILE_END,

	BCBM_BOTLIST_DATA,
	BCBM_BOTLIST_RES,
	
	BCBM_SENDTEST_RES,
	BCBM_CLIENT_SENDTEST_REQ,

	BCBM_CLIENT_LOG_NFY,

	BCBM_OPCODE_DUMMY,
	BCBM_OPCODE_END = BCBM_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_BCBM(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_ENTER_REQ )
	unsigned int botgroupID;
	unsigned int botagentID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_SERVERDATA_REQ )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_BOTCOMMAND_RES )
	WORD	wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_PROFILE_BEGIN )
	WORD				wResultCode;
	unsigned int		uiProfileDataCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_PROFILE_DATA )
	DWORD				dwCmd;				//명령어
	unsigned int		uiBotClientGroupID;	//Group ID
	unsigned int		uiBotClientID;		//Bot Client ID
	unsigned int		uiBotID;			//Bot ID
	unsigned __int64	uiCmdRunStartTime;	//명령어가 최초로 시작된 시간
	DWORD				dwStartTime;		//명령어 시작 시간
	DWORD				dwEndTime;			//명령어 종료 시간
	char				szComment[NTL_MAX_PROFILECOMMENT_STRING_SIZE + 1]; //프로파일 코멘트
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_PROFILE_END )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_BOTLIST_DATA )
	DWORD dwBotID;
	WCHAR awchUserId[NTL_MAX_SIZE_USERID_UNICODE + 1];
	WCHAR awchPasswd[NTL_MAX_SIZE_USERPW_UNICODE + 1];
	DWORD dwCodePage;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_BOTLIST_RES )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_SENDTEST_RES )
	DWORD dwSendCount;
	BYTE byDummy[ 1024 ];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_CLIENT_SENDTEST_REQ )
	DWORD dwSendCount;
	BYTE byDummy[ 1024 ];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BCBM_CLIENT_LOG_NFY )
	char szLog[ NTL_MAX_SIZE_TOOL_COMMENT + 1 ];
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()
