#pragma once
//***********************************************************************************
//
//	File		:	NtlPacketBMBC.h
//
//	Begin		:	2007-08-09 오전 11:31:00
//
//	Copyright	:	ⓒ NTL-Inc Co. Ltd
//
//	strAuthor	:	Seung Goon. Chi   ( sgpro@ntl-inc.com )
//
//	strDesc		:	
//
//***********************************************************************************

//-- include ------------------------------------------------------------------------
#include "NtlSharedBotSystemDef.h"
#include "NtlPacketCommon.h"
#include "NtlSharedDef.h"
//-----------------------------------------------------------------------------------

enum eOPCODE_BMBC
{
	BMBC_OPCODE_BEGIN = 64300,
	BMBC_HEARTBEAT,
	BMBC_ENTER_RES,
	BMBC_SERVERDATA_RES,
	BMBC_BOTCOMMAND_REQ,
	BMBC_BOTCLIENTCLOSE_NFY,
	
	BMBC_BOTLIST_REQ,

	BMBC_SENDTEST_REQ,
	BMBC_CLIENT_SENDTEST_NFY,
	BMBC_CLIENT_SENDTEST_RES,

	BMBC_OPCODE_DUMMY,
	BMBC_OPCODE_END = BMBC_OPCODE_DUMMY - 1
};

//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_BMBC(WORD wOpCode);
//------------------------------------------------------------------

#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBC_HEARTBEAT )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBC_ENTER_RES )
	WORD wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBC_SERVERDATA_RES )
	struct sAUTHSERVER_INFO
	{
		char			szAuthServerIP[NTL_MAX_LENGTH_OF_IP + 1];
		WORD			wAuthServerPortForBotClient;
	};

	sAUTHSERVER_INFO	AuthSrvInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBC_BOTCOMMAND_REQ )
	char				szBotCommand[NTL_MAX_BOTCOMMAND_STRING_SIZE + 1];
	DWORD				dwCmd;										//명령어
	unsigned int		uiBotClientID;								//Bot Client ID;
	unsigned __int64	uiCmdRunStartTime;							//명령어가 최초로 시작된 시간
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBC_BOTCLIENTCLOSE_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBC_BOTLIST_REQ )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBC_SENDTEST_REQ )
	DWORD dwSendCount;
	BYTE byDummy[ 1024 ];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBC_CLIENT_SENDTEST_NFY )
	DWORD dwSpinCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( BMBC_CLIENT_SENDTEST_RES )
	DWORD dwSendCount;
	BYTE byDummy[ 1024 ];
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack()
