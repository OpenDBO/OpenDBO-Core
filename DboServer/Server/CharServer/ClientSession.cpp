#include "stdafx.h"
#include "ClientSession.h"
#include "CharServer.h"
#include "NtlPacketCM.h"
#include "NtlResultCode.h"
#include "uc_opcodes.h"
#include "Player.h"
#include "PacketEventObj.h"



CClientSession::~CClientSession()
{
}


int CClientSession::OnAccept()
{
	eUserState = NTL_USER_STATE_NONE;
	m_pPlayer = NULL;

	//start handshake (with client)
	unsigned char buf[] = { 0x03, 0x00, 0xac, 0x86, 0xf5, 0x74 };
	CNtlPacket packet(buf, 0x06);
	g_pApp->Send(GetHandle(), &packet);

	return CNtlSession::OnAccept();
}


void CClientSession::OnClose()
{
	CCharServer* app = (CCharServer*)g_pApp;

	//send to master server remove player from char list
	if(m_pPlayer)
	{
		//check if in login queue
#ifdef USE_LOGIN_QUEUE_SYSTEM
		if (m_pPlayer->CheckQueue())
		{
			g_pQueueManager->RemoveFromQueue(m_pPlayer->GetAccountID());
			m_pPlayer->SetQueue(false);
		}
#endif

		CNtlPacket packet(sizeof(sCM_LOGOUT_REQ));
		sCM_LOGOUT_REQ * res = (sCM_LOGOUT_REQ *)packet.GetPacketData();
		res->wOpCode = CM_LOGOUT_REQ;
		res->accountId = m_pPlayer->GetAccountID();
		app->SendTo(app->m_pMasterServerSession, &packet);

		m_pPlayer->SetSession(NULL);
		m_pPlayer->SetSessionHandle(INVALID_HSESSION);

		g_PlrMgr->RemovePlayer(m_pPlayer);

		m_pPlayer = NULL;
	}
}


int CClientSession::OnDispatch(CNtlPacket * pPacket)
{
	CCharServer * app = (CCharServer*)g_pApp;

	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();
	//printf("receive packet %u \n", pHeader->wOpCode);
	if (pHeader->wOpCode == 4)
	{
		unsigned char buf[] = { 0x10, 0x00, 0x84, 0xfb, 0x48, 0xf4, 0x8e, 0x5a, 0xb6, 0x67, 0xe2, 0x3d,
			0x6e, 0x14, 0xb4, 0xa3, 0xc3, 0x24, 0x9e, 0x5f, 0xe3, 0xd1, 0xd5, 0x88, 0x10, 0x0d, 0x68,
			0x4f, 0x3b, 0xa5, 0xed, 0x37, 0xed, 0x4a };
		CNtlPacket packet(buf, 0x22);
		g_pApp->Send(GetHandle(), &packet);
	}
	OpcodeHandler<CClientSession> const* opHandle = uc_opcodeTable->LookupOpcode(pHeader->wOpCode);
	if (opHandle)
	{
		if (opHandle->packetProcessing == PROCESS_INPLACE)
			(this->*opHandle->handler)(pPacket);
		else
			app->PostClientPacketEvent(new TPacketEventObj<CClientSession>(this, opHandle->handler, GetHandle(), pPacket, GetUniqueHandle()));

		return NTL_SUCCESS;
	}

	return CNtlSession::OnDispatch(pPacket);
}

