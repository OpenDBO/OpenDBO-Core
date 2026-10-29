#include "stdafx.h"
#include "DungeonManager.h" //include DwcDungeon.h
#include "GameServer.h"
#include "GameMain.h"
#include "TableContainerManager.h"
#include "ServerConfigTable.h"
#include "ScriptAlgoObjectManager.h"
#include "WpsAlgoObject.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "NtlResultCode.h"


CDwcDungeon::CDwcDungeon(DWORD dwID, sDWC_TBLDAT* pTbldat, HOBJECT hNpc) :
	m_dwID(dwID), m_pTbldat(pTbldat), m_hNpc(hNpc)
{
	m_pWorld = NULL;
	m_pScript = NULL;
	m_byState = DWC_DUNGEON_STATE_PROPOSAL;
	m_dwWaitTime = 0;
	m_bLeaving = false;
}


CDwcDungeon::~CDwcDungeon()
{
	Destroy();
}


bool CDwcDungeon::Create(const std::vector<CPlayer*>& rMembers, bool bProposal)
{
	if (rMembers.empty())
		return false;

	if (bProposal == false)
	{
		for (std::vector<CPlayer*>::const_iterator it = rMembers.begin(); it != rMembers.end(); it++)
			AddMember(*it, true);

		// a failed start already told the members and ends the dungeon on the next tick
		StartDirection(false);
		return true;
	}

	sSERVERCONFIG_DATA* pConfig = g_pTableContainer->GetServerConfigTable()->GetServerConfigData();

	DWORD dwProposalWaitTime = pConfig->sDWCData.dwEnterProposalWaitTime;
	if (dwProposalWaitTime == INVALID_DWORD || dwProposalWaitTime == 0)
		dwProposalWaitTime = 30;

	for (std::vector<CPlayer*>::const_iterator it = rMembers.begin(); it != rMembers.end(); it++)
	{
		CPlayer* pMember = *it;

		AddMember(pMember, false);

		CNtlPacket packet(sizeof(sGU_DWC_ENTER_PROPOSAL_NFY));
		sGU_DWC_ENTER_PROPOSAL_NFY* res = (sGU_DWC_ENTER_PROPOSAL_NFY*)packet.GetPacketData();
		res->wOpCode = GU_DWC_ENTER_PROPOSAL_NFY;
		res->dwcTblidx = m_pTbldat->tblidx;
		res->byDWCLimitCount = pMember->GetDwcRemainCount();
		res->dwWaitTime = dwProposalWaitTime;
		packet.SetPacketLen(sizeof(sGU_DWC_ENTER_PROPOSAL_NFY));
		pMember->SendPacket(&packet);
	}

	m_byState = DWC_DUNGEON_STATE_PROPOSAL;
	m_dwWaitTime = dwProposalWaitTime * 1000;

	return true;
}


void CDwcDungeon::Destroy()
{
	for (std::map<HOBJECT, sMEMBER>::iterator it = m_mapMembers.begin(); it != m_mapMembers.end(); it++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(it->first);
		if (pMember && pMember->GetDWC() == this)
			pMember->SetDWC(NULL);
	}

	m_mapMembers.clear();

	if (m_pWorld)
	{
		CGameServer* app = (CGameServer*)g_pApp;

		app->GetGameMain()->GetWorldManager()->DestroyWorld(m_pWorld);
		m_pWorld = NULL;
		m_pScript = NULL;
	}
}


bool CDwcDungeon::TickProcess(DWORD dwTickDiff)
{
	switch (m_byState)
	{
		case DWC_DUNGEON_STATE_PROPOSAL:
		{
			m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickDiff);
			if (m_dwWaitTime == 0)
				Cancel(GAME_COMMON_EXIST_USER_WHO_DO_NOT_AGREE);
		}
		break;

		case DWC_DUNGEON_STATE_DIRECTION:
		{
			m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickDiff);
			if (m_dwWaitTime == 0)
				EnterWorld();
		}
		break;

		case DWC_DUNGEON_STATE_FAIL:
		{
			m_dwWaitTime = UnsignedSafeDecrease<DWORD>(m_dwWaitTime, dwTickDiff);
			if (m_dwWaitTime == 0)
				Leave();
		}
		break;

		default: break;
	}

	return m_byState == DWC_DUNGEON_STATE_END;
}


void CDwcDungeon::Confirm(CPlayer* pPlayer, bool bEnter)
{
	WORD wResultCode = GAME_SUCCESS;

	std::map<HOBJECT, sMEMBER>::iterator it = m_mapMembers.find(pPlayer->GetID());
	if (it == m_mapMembers.end() || m_byState != DWC_DUNGEON_STATE_PROPOSAL)
		wResultCode = DWC_FAIL;

	CNtlPacket packet(sizeof(sGU_DWC_ENTER_CONFIRM_RES));
	sGU_DWC_ENTER_CONFIRM_RES* res = (sGU_DWC_ENTER_CONFIRM_RES*)packet.GetPacketData();
	res->wOpCode = GU_DWC_ENTER_CONFIRM_RES;
	res->wResultCode = wResultCode;
	res->bEnter = bEnter;
	packet.SetPacketLen(sizeof(sGU_DWC_ENTER_CONFIRM_RES));
	pPlayer->SendPacket(&packet);

	if (wResultCode != GAME_SUCCESS)
		return;

	if (bEnter == false)
	{
		Cancel(GAME_COMMON_EXIST_USER_WHO_DO_NOT_AGREE);
		return;
	}

	it->second.bAccepted = true;

	for (it = m_mapMembers.begin(); it != m_mapMembers.end(); it++)
	{
		if (it->second.bAccepted == false)
			return;
	}

	// retail: the party waits the config enter direction time (9 seconds) while the clients play the
	// statue next to the npc (GU_DWC_ENTER_DIRECTION_NFY), then goes in
	StartDirection(true);
}


void CDwcDungeon::AddMember(CPlayer* pMember, bool bAccepted)
{
	sMEMBER sMember;
	sMember.charId = pMember->GetCharID();
	sMember.bAccepted = bAccepted;
	sMember.bInside = false;
	sMember.returnWorldId = pMember->GetWorldID();
	sMember.vReturnLoc = pMember->GetCurLoc();
	sMember.vReturnDir = pMember->GetCurDir();

	m_mapMembers.insert({ pMember->GetID(), sMember });

	pMember->SetDWC(this);
}


void CDwcDungeon::Cancel(WORD wResultCode)
{
	CNtlPacket packet(sizeof(sGU_DWC_ENTER_CANCELED_NFY));
	sGU_DWC_ENTER_CANCELED_NFY* res = (sGU_DWC_ENTER_CANCELED_NFY*)packet.GetPacketData();
	res->wOpCode = GU_DWC_ENTER_CANCELED_NFY;
	res->wResultCode = wResultCode;
	packet.SetPacketLen(sizeof(sGU_DWC_ENTER_CANCELED_NFY));

	for (std::map<HOBJECT, sMEMBER>::iterator it = m_mapMembers.begin(); it != m_mapMembers.end(); it++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(it->first);
		if (pMember && pMember->GetDWC() == this)
		{
			pMember->SendPacket(&packet);
			pMember->SetDWC(NULL);
		}
	}

	m_mapMembers.clear();

	m_byState = DWC_DUNGEON_STATE_END;
}


void CDwcDungeon::StartDirection(bool bDirection)
{
	sWORLD_TBLDAT* pWorldTbldat = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(m_pTbldat->worldTblidx);
	if (pWorldTbldat == NULL)
	{
		ERR_LOG(LOG_GENERAL, "DWC %u: world %u not found", m_pTbldat->tblidx, m_pTbldat->worldTblidx);
		Cancel(DWC_FAIL);
		return;
	}

	CGameServer* app = (CGameServer*)g_pApp;

	m_pWorld = app->GetGameMain()->GetWorldManager()->CreateWorld(pWorldTbldat);
	if (m_pWorld == NULL)
	{
		Cancel(GAME_WORLD_CAN_NOT_CREATE_DYNAMIC_WORLD);
		return;
	}

	m_pScript = (CWpsAlgoObject*)m_pWorld->GetScript(pWorldTbldat->wpsLinkIndex);
	if (m_pScript == NULL)
	{
		ERR_LOG(LOG_SCRIPT, "DWC %u: could not create wps script %u for world %u", m_pTbldat->tblidx, pWorldTbldat->wpsLinkIndex, pWorldTbldat->tblidx);
		Cancel(DWC_FAIL);
		return;
	}

	sSERVERCONFIG_DATA* pConfig = g_pTableContainer->GetServerConfigTable()->GetServerConfigData();

	// the members wait the enter direction time standing, a quest entry goes in at once. The config direction
	// (10004, SIT_DOWN) is not played on them: it only sat them down and stood them up again
	DWORD dwDirectionTime = pConfig->sDWCData.dwEnterDirectionPlayTime;
	if (dwDirectionTime == INVALID_DWORD)
		dwDirectionTime = 0;

	CNtlPacket packet(sizeof(sGU_DWC_ENTER_DIRECTION_NFY));
	sGU_DWC_ENTER_DIRECTION_NFY* res = (sGU_DWC_ENTER_DIRECTION_NFY*)packet.GetPacketData();
	res->wOpCode = GU_DWC_ENTER_DIRECTION_NFY;
	res->hNpc = m_hNpc;
	res->dwcTblidx = m_pTbldat->tblidx;
	packet.SetPacketLen(sizeof(sGU_DWC_ENTER_DIRECTION_NFY));

	for (std::map<HOBJECT, sMEMBER>::iterator it = m_mapMembers.begin(); it != m_mapMembers.end(); it++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(it->first);
		if (pMember == NULL || pMember->IsInitialized() == false)
			continue;

		pMember->AddDwcEnterCount();
		pMember->SendPacket(&packet);
	}

	m_byState = DWC_DUNGEON_STATE_DIRECTION;
	m_dwWaitTime = bDirection ? dwDirectionTime * 1000 : 0;
}


void CDwcDungeon::EnterWorld()
{
	for (std::map<HOBJECT, sMEMBER>::iterator it = m_mapMembers.begin(); it != m_mapMembers.end(); )
	{
		CPlayer* pMember = g_pObjectManager->GetPC(it->first);
		if (pMember == NULL || pMember->IsInitialized() == false || pMember->GetDWC() != this)
		{
			it = m_mapMembers.erase(it);
			continue;
		}

		// a stuck or completed system quest of an earlier run would keep the scripts of this run from going on
		pMember->GetQuests()->ResetQuestRange(DWC_SYSTEM_QUEST_MIN, DWC_SYSTEM_QUEST_MAX);

		m_pScript->AddPlayer(pMember->GetID());
		pMember->AddScript(m_pScript);

		pMember->StartTeleport(m_pWorld->GetTbldat()->vStart1Loc, m_pWorld->GetTbldat()->vStart1Dir, m_pWorld->GetID(), TELEPORT_TYPE_DWC);

		it->second.bInside = true;
		++it;
	}

	if (m_mapMembers.size() == 0)
	{
		m_byState = DWC_DUNGEON_STATE_END;
		return;
	}

	m_byState = DWC_DUNGEON_STATE_PLAY;

	m_pScript->Start();
}


void CDwcDungeon::Fail()
{
	if (m_byState != DWC_DUNGEON_STATE_PLAY)
		return;

	sSERVERCONFIG_DATA* pConfig = g_pTableContainer->GetServerConfigTable()->GetServerConfigData();

	DWORD dwFailWaitTime = pConfig->sDWCData.dwFailStateWaitTime;
	if (dwFailWaitTime == INVALID_DWORD)
		dwFailWaitTime = 10;

	if (pConfig->sDWCData.failFlashTblidx != INVALID_TBLIDX)
	{
		for (std::map<HOBJECT, sMEMBER>::iterator it = m_mapMembers.begin(); it != m_mapMembers.end(); it++)
		{
			CPlayer* pMember = g_pObjectManager->GetPC(it->first);
			if (pMember && it->second.bInside)
				pMember->SendDirectionFlash(pConfig->sDWCData.failFlashTblidx);
		}
	}

	m_byState = DWC_DUNGEON_STATE_FAIL;
	m_dwWaitTime = dwFailWaitTime * 1000;
}


void CDwcDungeon::Leave()
{
	if (m_bLeaving)
		return;

	m_bLeaving = true;

	if (m_byState == DWC_DUNGEON_STATE_FAIL)
		m_byState = DWC_DUNGEON_STATE_PLAY;

	for (std::map<HOBJECT, sMEMBER>::iterator it = m_mapMembers.begin(); it != m_mapMembers.end(); it++)
	{
		CPlayer* pMember = g_pObjectManager->GetPC(it->first);
		if (pMember && it->second.bInside && pMember->GetWorldID() == m_pWorld->GetID())
			pMember->StartTeleport(it->second.vReturnLoc, it->second.vReturnDir, it->second.returnWorldId, TELEPORT_TYPE_DWC);
	}
}


void CDwcDungeon::LeaveDungeon(CPlayer* pPlayer)
{
	std::map<HOBJECT, sMEMBER>::iterator it = m_mapMembers.find(pPlayer->GetID());
	if (it == m_mapMembers.end())
		return;

	if (m_byState == DWC_DUNGEON_STATE_PROPOSAL) // somebody gave up before everybody accepted
	{
		Cancel(GAME_COMMON_EXIST_USER_WHO_DO_NOT_AGREE);
		return;
	}

	m_mapMembers.erase(it);
	pPlayer->SetDWC(NULL);

	if (m_mapMembers.size() == 0)
		m_byState = DWC_DUNGEON_STATE_END;
}


bool CDwcDungeon::GetReturnLocation(HOBJECT hPlayer, WORLDID& rWorldId, CNtlVector& rLoc, CNtlVector& rDir)
{
	std::map<HOBJECT, sMEMBER>::iterator it = m_mapMembers.find(hPlayer);
	if (it == m_mapMembers.end())
		return false;

	rWorldId = it->second.returnWorldId;
	rLoc = it->second.vReturnLoc;
	rDir = it->second.vReturnDir;

	return true;
}
