#include "stdafx.h"
#include "ClientSession.h"
#include "AuthServer.h"

#include "NtlPacketUA.h"
#include "NtlPacketAU.h"
#include "NtlResultCode.h"


CClientSession::~CClientSession()
{
}


int CClientSession::OnAccept()
{
	this->AccountID = NULL;
	m_byLoginTrys = 0;

	//start handshake (with client)
	unsigned char buf[] = { 0x03, 0x00, 0xac, 0x86, 0xf5, 0x74 };
	CNtlPacket packet(buf, 0x06);
	g_pApp->Send(GetHandle(), &packet);

	return CNtlSession::OnAccept();
}

void CClientSession::OnClose()
{
	CAuthServer * app = (CAuthServer*) NtlSfxGetApp();

	if(this->AccountID != NULL)
		app->DelPlayer(this->AccountID);
}

int CClientSession::OnDispatch(CNtlPacket * pPacket)
{
	CAuthServer * app = (CAuthServer*)NtlSfxGetApp();

	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();

	//printf("receive packet %u \n", pHeader->wOpCode);

	switch( pHeader->wOpCode )
	{
	case 4:
	{
		unsigned char buf[] = { 0x10, 0x00, 0x84, 0xfb, 0x48, 0xf4, 0x8e, 0x5a, 0xb6, 0x67, 0xe2, 0x3d,
			0x6e, 0x14, 0xb4, 0xa3, 0xc3, 0x24, 0x9e, 0x5f, 0xe3, 0xd1, 0xd5, 0x88, 0x10, 0x0d, 0x68,
			0x4f, 0x3b, 0xa5, 0xed, 0x37, 0xed, 0x4a };
		CNtlPacket packet(buf, 0x22);
		g_pApp->Send(GetHandle(), &packet);
		break;
	}
		case UA_LOGIN_REQ:				SendCharLogInReq(pPacket, app);	break;//Login Korean
		case UA_LOGIN_TW_REQ:			SendCharLogInReq(pPacket, app);	break;
		case UA_LOGIN_CREATEUSER_REQ:	SendLoginDcReq(pPacket, app);	break;//Disconect Korean
		case UA_LOGIN_DISCONNECT_REQ:	SendLoginDcReq(pPacket, app);	break;

		default:	return CNtlSession::OnDispatch(pPacket);			break;
	}

	return NTL_SUCCESS;
}