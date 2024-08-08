//***********************************************************************************
//
//	File		:	Protocol.h
//
//	Begin		:	2006-01-09
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__


const unsigned int MAX_SERVER_ADDRESS = 16;
const unsigned int MAX_SIZE_USER_ID  = 24;
const unsigned int MAX_SIZE_CHAT_DATA = 255;

enum SAMPLE_RESULT_CODE
{
	CS_RESULT_OK,
	CS_RESULT_NO,
};


enum SAMPLE_PROTOCOL
{
	SYSTEM_ALIVE = 1,
	SYSTEM_PING,

	SYSTEM_ECHO,

	CS_USER_JOIN_REQ,
	CS_USER_JOIN_RES,
	CS_USER_CHAT,
	CS_USER_EXIT_NFY,

	SS_SERVER_REGISTER_NFY,
	SS_USER_CHAT_NFY,
};


struct mSAMPLE_PROTOCOL_HEADER
{
	mSAMPLE_PROTOCOL_HEADER(WORD id)
		:wProtocolID(id) {}

	WORD	wProtocolID;
};

#define BEG_PROTOCOL(id)							\
struct m##id : public mSAMPLE_PROTOCOL_HEADER		\
{													\
	m##id():mSAMPLE_PROTOCOL_HEADER(id) {}					
#define END_PROTOCOL()	};


//------------------------------------------------------------------
BEG_PROTOCOL( SYSTEM_ALIVE )
END_PROTOCOL()
//------------------------------------------------------------------
BEG_PROTOCOL( SYSTEM_PING )
	DWORD	dwTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEG_PROTOCOL( SYSTEM_ECHO )
	DWORD	dwTime;
	BYTE	byEchoType; // 0 : echo, 1 : echo broadcast
	DWORD	dwParam; // Reserved Parameter
	WORD	wDataLen;
	BYTE	abyData[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEG_PROTOCOL( CS_USER_JOIN_REQ )
	BYTE	abyUserID[MAX_SIZE_USER_ID + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEG_PROTOCOL( CS_USER_JOIN_RES )
	BYTE	byResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEG_PROTOCOL( CS_USER_CHAT )
	BYTE	abyUserID[MAX_SIZE_USER_ID + 1];
	BYTE	byChatLen;
	BYTE	abyChatData[MAX_SIZE_CHAT_DATA +  1];
END_PROTOCOL()
//------------------------------------------------------------------
BEG_PROTOCOL( SS_USER_CHAT_NFY )
	BYTE	abyUserID[MAX_SIZE_USER_ID + 1];
	BYTE	byChatLen;
	BYTE	abyChatData[MAX_SIZE_CHAT_DATA +  1];
END_PROTOCOL()
//------------------------------------------------------------------
BEG_PROTOCOL( SS_SERVER_REGISTER_NFY )
END_PROTOCOL()
//------------------------------------------------------------------


#endif // __PROTOCOL_H__
