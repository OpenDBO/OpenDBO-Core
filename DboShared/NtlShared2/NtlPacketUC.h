#pragma once

#include "NtlPacketCommon.h"
#include "NtlCharacter.h"


enum eOPCODE_UC
{
	UC_OPCODE_BEGIN = 2000,

	UC_LOGIN_REQ,
	UC_CHARACTER_SERVERLIST_REQ,
	UC_CHARACTER_SERVERLIST_ONE_REQ,
	UC_CHARACTER_ADD_REQ,
	UC_CHARACTER_DEL_REQ,
	UC_CHARACTER_SELECT_REQ,
	UC_CHARACTER_EXIT_REQ,
	UC_CHARACTER_LOAD_REQ,
	UC_CHARACTER_DEL_CANCEL_REQ,
	UC_CONNECT_WAIT_CHECK_REQ,
	UC_CONNECT_WAIT_CANCEL_REQ,
	UC_CHARACTER_RENAME_REQ,
	UC_CASHITEM_HLSHOP_REFRESH_REQ,
	UC_CASHITEM_BUY_REQ,
	UC_CHAR_SERVERLIST_REQ,

	UC_OPCODE_DUMMY,
	UC_OPCODE_END = UC_OPCODE_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_UC(WORD wOpCode);
//------------------------------------------------------------------

#pragma warning( push )
#pragma warning( disable : 26495 ) // Variable 'x' is uninitialized.Always initialize a member variable(type.6)


#pragma pack(1)

//------------------------------------------------------------------
BEGIN_PROTOCOL(UC_LOGIN_REQ)
	ACCOUNTID		accountId;
	BYTE			abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
	SERVERFARMID	serverID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UC_CHARACTER_SERVERLIST_REQ)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UC_CHARACTER_SERVERLIST_ONE_REQ)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UC_CHARACTER_ADD_REQ)
	WCHAR			awchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	BYTE			byRace;
	BYTE			byClass;
	BYTE			byGender;
	sPC_SHAPE		sPcShape;
	BYTE			byBlood;// to be removed
	TBLIDX			superiorID;// to be removed
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UC_CHARACTER_DEL_REQ)
	CHARACTERID		charId;
	WCHAR			awcDeletePassword[NTL_MAX_SIZE_USERPW + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UC_CHARACTER_SELECT_REQ)
	CHARACTERID		charId;
	SERVERCHANNELID	serverChannelId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UC_CHARACTER_EXIT_REQ)
	BYTE			byMoveType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UC_CHARACTER_LOAD_REQ)
	ACCOUNTID		accountId;
	SERVERFARMID	serverFarmId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(UC_CHARACTER_DEL_CANCEL_REQ)
	CHARACTERID		charId;
	WCHAR			awcDeletePassword[NTL_MAX_SIZE_USERPW + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UC_CONNECT_WAIT_CHECK_REQ )
	SERVERCHANNELID	serverChannelId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UC_CONNECT_WAIT_CANCEL_REQ )
	SERVERCHANNELID	serverChannelId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( UC_CHARACTER_RENAME_REQ )
	CHARACTERID		charId;
	//DWORD			dwCodePage;
	WCHAR			awchCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
END_PROTOCOL()
//------------------------------------------------------------------
#pragma pack()

#pragma warning( pop ) // (26495) Variable 'x' is uninitialized.Always initialize a member variable(type.6)