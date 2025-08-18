#include "StdAfx.h"
#include "CPlayer.h"
#include "RejoinSystem.h"
#include "DungeonManager.h"
#include "ObjectManager.h"
#include "Party.h"
#include "NtlPacketTQ.h"
#include "NtlPacketTU.h"
#include "DojoManager.h"
#include "NtlPacketGT.h"
#include "NtlPacketTG.h"
#include "NtlResultCode.h"
#include "GameServer.h"
#include "Monster.h"
#include "ItemManager.h"
#include "NtlPacketUG.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "NtlPacketGM.h"
#include "SubNeighborServerInfoManager.h"
#include "GameMain.h"
#include "BudokaiManager.h"

CRejoinManager g_Rejoin;

void CRejoinManager::Put(const sREJOIN_TICKET& t)
{
	sREJOIN_RECORD rec;
	rec.ticket = t;
	rec.hasSnapshot = false;
	m_records[t.charId] = rec;
}

void CRejoinManager::PutWithTarget(const sREJOIN_TICKET& t, const sREJOIN_TARGET& target)
{
	sREJOIN_RECORD rec;
	rec.ticket = t;
	rec.hasSnapshot = true;
	rec.snapshot = target;
	m_records[t.charId] = rec;
}

sREJOIN_TICKET* CRejoinManager::Find(CHARACTERID cid)
{
	auto it = m_records.find(cid);
	return (it == m_records.end()) ? nullptr : &it->second.ticket;
}

sREJOIN_RECORD* CRejoinManager::FindRecord(CHARACTERID cid)
{
	auto it = m_records.find(cid);
	return (it == m_records.end()) ? nullptr : &it->second;
}

void CRejoinManager::Erase(CHARACTERID cid)
{
	m_records.erase(cid);
}

bool CRejoinManager::ResolveRejoinTarget(const sREJOIN_TICKET& t, sREJOIN_TARGET& outTarget, CPlayer* cPlayer)
{
	//auto it = m_records.find(t.charId);
	//if (it != m_records.end() && it->second.hasSnapshot)
	//{
	//	outTarget = it->second.snapshot;
	//	return true;
	//}

	CPlayer* pPlayerTarget = nullptr;
	CGameServer* app = (CGameServer*)g_pApp;

	if (cPlayer->GetParty() == NULL) {
		auto pParty = g_pPartyManager->GetParty(t.partyId);
		if (pParty)
		{
			auto memberName = pParty->GetMemberInfo(0).awchMemberName;
			pPlayerTarget = g_pObjectManager->FindByName(memberName);

			if (!pPlayerTarget || !pPlayerTarget->IsInitialized())
			{
				ERR_LOG(LOG_GENERAL, "Party member %ls not found for rejoin", memberName);
				return false; // no target found, cannot rejoin
			}

			CWorld* pWorld = app->GetGameMain()->GetWorldManager()->FindWorld(pPlayerTarget->GetWorldID());
			if (pWorld)
			{
				switch (pWorld->GetRuleType())
				{
				case GAMERULE_NORMAL:
				case GAMERULE_RANKBATTLE:
				case GAMERULE_MUDOSA:
				case GAMERULE_DOJO:
				case GAMERULE_HUNT:
				case GAMERULE_TUTORIAL:
				case GAMERULE_MINORMATCH:
				case GAMERULE_MAJORMATCH:
				case GAMERULE_FINALMATCH:
				case GAMERULE_TEINKAICHIBUDOKAI:
				case INVALID_GAMERULE:
					return false; // cannot rejoin in normal world or other game rules
					break;
				}
			}

			// if we have a party, we can rejoin dungeon
			cPlayer->SetParty(pParty);
			cPlayer->SetPartyID(t.partyId);
			pParty->AddPartyMember(cPlayer);
		}
	} else {
		// if we already have a party, we can rejoin dungeon with the party leader
		return false;
	}

	// if we don't have a party, we cannot rejoin any dungeon
	switch (t.dungeonType)
	{
	case REJOIN_TMQ:
		if (auto* pTQ = g_pDungeonManager->FindTimeQuest(t.partyId))
		{
			cPlayer->StartTeleport(pPlayerTarget->GetCurLoc(), pPlayerTarget->GetCurDir(), pPlayerTarget->GetWorldID(), TELEPORT_TYPE_COMMAND);
			pTQ->AddMember(cPlayer);
			cPlayer->SetTMQ(pTQ);
			return true;
		}
		break;
	case REJOIN_TLQ:
		if (auto* pTL = g_pDungeonManager->FindTimeLeapDungeon(t.charId))
		{
			cPlayer->StartTeleport(pPlayerTarget->GetCurLoc(), pPlayerTarget->GetCurDir(), pPlayerTarget->GetWorldID(), TELEPORT_TYPE_COMMAND);
			cPlayer->SetTLQ(pTL);

			return true;
		}
		break;
	case REJOIN_UD:
		if (auto* pUD = g_pDungeonManager->FindUltimateDungeon(t.partyId))
		{
			cPlayer->StartTeleport(pPlayerTarget->GetCurLoc(), pPlayerTarget->GetCurDir(), pPlayerTarget->GetWorldID(), TELEPORT_TYPE_COMMAND);
			pUD->JoinDungeon(cPlayer);
			cPlayer->SetUD(pUD);
			return true;
		}
		break;
	case REJOIN_CC:
		if (auto* pBD = g_pDungeonManager->FindBattleDungeon(t.partyId))
		{
			cPlayer->StartTeleport(pPlayerTarget->GetCurLoc(), pPlayerTarget->GetCurDir(), pPlayerTarget->GetWorldID(), TELEPORT_TYPE_COMMAND);
			pBD->JoinDungeon(cPlayer);
			cPlayer->SetCCBD(pBD);
			return true;
		}
		break;
	case REJOIN_BUDOKAI:
		g_pBudokaiManager->CanTeleportMajorMatch(cPlayer);
		break; // Budokai rejoin is handled separately
	default: break;
	}
	return false;
}
