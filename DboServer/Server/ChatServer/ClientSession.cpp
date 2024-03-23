#include "stdafx.h"
#include "ClientSession.h"
#include "ChatServer.h"

#include "ut_opcodes.h"
#include "PacketEventObj.h"
#include "NtlPacketUtil.h"


int CClientSession::OnAccept()
{
	cPlayer = NULL;

	m_dwLastMessage = 0;
	m_dwLastGlobalMessage = 0;

	//start handshake (with client)
	unsigned char buf[] = { 0x03, 0x00, 0xac, 0x86, 0xf5, 0x74 };
	CNtlPacket packet(buf, 0x06);
	g_pApp->Send(GetHandle(), &packet);

	return CNtlSession::OnAccept();
}



void CClientSession::OnClose()
{
	//NTL_PRINT(PRINT_APP, "CClientSession::OnClose()");
	cPlayer = NULL;
}

//#include "NtlPacketUtil.h"
int CClientSession::OnDispatch(CNtlPacket * pPacket)
{
	CChatServer * app = (CChatServer*)g_pApp;

	sNTLPACKETHEADER * pHeader = (sNTLPACKETHEADER *)pPacket->GetPacketData();
	if (pHeader->wOpCode > 1)
		NTL_PRINT(PRINT_APP, "%u | received opcode %u [%s]", GetHandle(), pHeader->wOpCode, NtlGetPacketName(pHeader->wOpCode));
	
	if (pHeader->wOpCode == 4)
	{
		unsigned char buf[] = { 0x10, 0x00, 0x84, 0xfb, 0x48, 0xf4, 0x8e, 0x5a, 0xb6, 0x67, 0xe2, 0x3d,
			0x6e, 0x14, 0xb4, 0xa3, 0xc3, 0x24, 0x9e, 0x5f, 0xe3, 0xd1, 0xd5, 0x88, 0x10, 0x0d, 0x68,
			0x4f, 0x3b, 0xa5, 0xed, 0x37, 0xed, 0x4a };
		CNtlPacket packet(buf, 0x22);
		g_pApp->Send(GetHandle(), &packet);
	}
	if (cPlayer)
	{
		if (cPlayer->GetClientSessionID() != GetHandle())
		{
			ERR_LOG(LOG_USER, "Clientsesion ID does not match %u != %u", cPlayer->GetClientSessionID(), GetHandle());
			return NTL_FAIL;
		}
	}

	
	OpcodeHandler<CClientSession> const* opHandle = ut_opcodeTable->LookupOpcode(pHeader->wOpCode);
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

