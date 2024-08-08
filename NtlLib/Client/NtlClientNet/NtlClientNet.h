//***********************************************************************************
//
//	File		:	NtlClientNet.h
//
//	Begin		:	2006-01-17
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	NTL Client Library header file
//
//***********************************************************************************

#pragma once

#include "ClentNetCompileOption.h"
#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
#include "BotsystemLog_Client.h"
#endif

/*-------------------------------------------------------------------------------------
	[1/10/2007 zeroera] 
---------------------------------------------------------------------------------------

@ 클라이언트 네트워크 라이브러리를 사용하는 방법은 다음의 두가지 잆니다.

	1. Windows Message Base 

		ClientNetwork 객체를 생성하고 Create시 메시지를 받을 윈도우 핸들을 전달합니다.
		
	2. Event Base

		ClientNetwork 객체를 생성하고 Create시 윈도우 핸들을 NULL로 전달 합니다.

---------------------------------------------------------------------------------------*/


//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
#define WM_NETWORK_IO						(WM_USER + 0x1000) // WM_USER = 0x0400
#define WM_INCOMING_PACKET					(WM_USER + 0x1001)
#define WM_CONNECTED						(WM_USER + 0x1002)
#define WM_DISCONNECTED						(WM_USER + 0x1003)
#define WM_CONNECT_FAIL						(WM_USER + 0x1004)

#define NETCLIENT_EVENT_CONNECTED			0x0001
#define NETCLIENT_EVENT_DISCONNECTED		0x0002
#define NETCLIENT_EVENT_CONNECT_FAIL		0x0003

#define INVALID_HSERVER						INVALID_SOCKET
//---------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
const unsigned int ALIVE_PACKET_FREQUENCY	= 3 * 1000; // 서버에 Alive Ping을 보내는 시간 간격
//---------------------------------------------------------------------------------------
typedef SOCKET HSERVER;	// 서버 핸들 값 ( = 소켓 값 )
typedef void (*NtlClientNetLogCallback)(const char * format); // Application Log Callback
typedef void (*NtlClientNetEventCallback)(HSERVER hServer, int nEvent);	// Application Event Callback
//-- define -------------------------------------------------------------------------
class CRecvPacketProcess
{
public:
	CRecvPacketProcess( const HSERVER hServer )
		: m_hServer( hServer )
	{
	}

	~CRecvPacketProcess(){};

public:
	virtual bool RecvPacketProcess( void* const pData ) = 0;
	virtual bool IsActivePop( void ) { return true; }

public:
	HSERVER m_hServer;

};//end of class CRecvPacketProcess
//---------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
enum eNETCLIENT_ENCODE_TYPE
{
	NETCLIENT_ENCODE_NONE = 0,
	NETCLIENT_ENCODE_XOR,
	NETCLIENT_ENCODE_RANDKEY,

	MAX_NETCLIENT_ENCODE_TYPE
};
//---------------------------------------------------------------------------------------

void NtlClientNetSetPrintFlag( unsigned int dwFlag );

//---------------------------------------------------------------------------------------
// NtlClientNet Class
//---------------------------------------------------------------------------------------
class CClientNetImp;
class CNtlClientNet
{
public:

	CNtlClientNet();

	virtual ~CNtlClientNet();


public:

	// 콜백함수를 등록하면 이 함수는 더이상 호출되지 않습니다.
	virtual void				OnNetEventProc(HSERVER hServer, int nEvent) { UNREFERENCED_PARAMETER(hServer), UNREFERENCED_PARAMETER(nEvent); }


public:


	bool						Create(HWND hWnd = NULL, NtlClientNetEventCallback EventCallback = NULL, NtlClientNetLogCallback LogCallback = NULL, DWORD dwAliveTime = ALIVE_PACKET_FREQUENCY, int nMaxConnection = WSA_MAXIMUM_WAIT_EVENTS);

	void						Destroy();


public:

#if __SGPRO_BOTSYSTEM_SENDANDRECVTESTCODE__
	bool						Connect( __int64 nBotID, char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync = false, eNETCLIENT_ENCODE_TYPE encode = NETCLIENT_ENCODE_NONE, int nMaxPacketCount = 0, int nMaxPacketSize = 0 );

	bool						Connect( __int64 nBotID, WCHAR* pwszServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync /* = false */, eNETCLIENT_ENCODE_TYPE encode /* = NETCLIENT_ENCODE_NONE */, int nMaxPacketCount /* = 0 */, int nMaxPacketSize /* = 0 */);

	void *						SGPeekPacket(HSERVER hServer);

#else
	bool						Connect(char* szServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync = false, eNETCLIENT_ENCODE_TYPE encode = NETCLIENT_ENCODE_NONE, int nMaxPacketCount = 0, int nMaxPacketSize = 0 );

	bool						Connect(WCHAR* pwszServerAddress, WORD wPort, HSERVER * pServerHandle, bool bAsync /* = false */, eNETCLIENT_ENCODE_TYPE encode /* = NETCLIENT_ENCODE_NONE */, int nMaxPacketCount /* = 0 */, int nMaxPacketSize /* = 0 */);

#endif

public:

	bool						Disconnect(HSERVER hServer);

	bool						NetMsgProc(WPARAM wParam, LPARAM lParam);		

	void *						PeekPacket(HSERVER hServer);	

	void 						PopPacket(HSERVER hServer);

	bool						PushPacket(HSERVER hServer, void * pData, int nDataSize);

	bool						IsProgress(HSERVER hServer);


public:

	LPCSTR						GetServerAddress(HSERVER hServer);

	WORD						GetServerPort(HSERVER hServer);

	LPCSTR						GetLocalAddress(HSERVER hServer);

	DWORD						GetLocalAddr(HSERVER hServer);

	WORD						GetLocalPort(HSERVER hServer);

public:

	void						RecvPacketProcess( CRecvPacketProcess& rProc );

private:

	CClientNetImp *				m_pImp;

};