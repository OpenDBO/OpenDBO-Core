#include "Stdafx.h"
#include "DungeonManager.h"
#include "TimeQuest.h"
#include "GameServer.h"
#include "TimeLeapDungeon.h"
#include "CPlayer.h"
#include "TableContainerManager.h"
#include "NtlResultCode.h"
#include "WorldPlayScriptManager.h"
#include "WorldPlayTable.h"


CDungeonManager::CDungeonManager()
{
	m_dwLastDwcDungeonID = 0;
}

CDungeonManager::~CDungeonManager()
{

}


void CDungeonManager::TickProcess(DWORD dwTickDiff)
{
	for (std::map<PARTYID, CTimeQuest*>::iterator it = m_mapTimeQuest.begin(); it != m_mapTimeQuest.end(); )
	{
		CTimeQuest* pDungeon = it->second;

		if (pDungeon->TickProcess(dwTickDiff))
		{
			it = m_mapTimeQuest.erase(it);
			SAFE_DELETE(pDungeon);
		}
		else
		{
			++it;
		}
	}

	for (std::map<DWORD, CDwcDungeon*>::iterator it = m_mapDwcDungeon.begin(); it != m_mapDwcDungeon.end(); )
	{
		CDwcDungeon* pDungeon = it->second;

		if (pDungeon->TickProcess(dwTickDiff))
		{
			it = m_mapDwcDungeon.erase(it);
			SAFE_DELETE(pDungeon);
		}
		else
		{
			++it;
		}
	}
}




WORD CDungeonManager::CreateTimeQuest(CPlayer* player, TBLIDX id, BYTE byDifficulty, BYTE byMode)
{
	sTIMEQUEST_TBLDAT* pTimeQuestTbldat = (sTIMEQUEST_TBLDAT*)g_pTableContainer->GetTimeQuestTable()->FindData(id);
	if (pTimeQuestTbldat == NULL)
	{
		printf("Could not find pTimeQuestTbldat. Tblidx %u\n", id);
		return GAME_TIMEQUEST_ERROR;
	}

	PARTYID partyID = (byMode == TIMEQUEST_MODE_PARTY) ? player->GetPartyID() : 50000 + player->GetCharID(); //party id can never be 12000 or higher

	if (FindTimeQuest(partyID)) //check if party already exist
	{
		return GAME_TIMEQUEST_ALREADY_JOINED;
	}

	CItem* pItem = NULL;

	if (byMode == TIMEQUEST_MODE_PARTY)
	{
		CParty* pParty = player->GetParty();

		pItem = player->GetPlayerItemContainer()->GetItemByIdx(pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].needItemTblidx);

		if (pItem == NULL)
			return GAME_TIMEQUEST_MUST_HAVE_NEED_ITEM;
		else if (pParty == NULL)
			return GAME_PARTY_YOU_ARE_NOT_IN_PARTY;
		else if (player->GetZeni() < pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].dwNeedZenny)
			return GAME_TIMEQUEST_MUST_HAVE_NEED_ZENNY;
		else if (pParty->GetPartyMemberCount() > pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].byMaxMemberCount) //check max member count
			return GAME_TIMEQUEST_EXCEED_MAX_MEMBER_COUNT;
		else if (pParty->GetPartyMemberCount() < pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].byMinMemberCount) //check min member count
			return GAME_TIMEQUEST_NEED_MORE_MEMBER;
		else
		{
			for (int i = 0; i < pParty->GetPartyMemberCount(); i++)
			{
				CPlayer* pMember = g_pObjectManager->GetPC(pParty->GetMemberInfo(i).hHandle);
				if (pMember && pMember->IsInitialized())
				{
					if (pMember->GetLevel() < pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].byMinMemberLevel) //check min level
						return GAME_TOO_LOW_MEMBER_LEVEL;
					else if (pMember->GetLevel() > pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].byMaxMemberLevel) //check max level
						return GAME_TOO_HIGHT_MEMBER_LEVEL;
				}
			}
		}
	}

	CTimeQuest* pTimeQuest = new CTimeQuest(partyID, pTimeQuestTbldat, byDifficulty, byMode);

	if (pTimeQuest->Create(player))
	{
		m_mapTimeQuest.insert({ partyID, pTimeQuest });

		//remove required zeni
		if (pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].dwNeedZenny > 0)
			player->UpdateZeni(ZENNY_CHANGE_TYPE_ITEM_BUY, pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].dwNeedZenny, false, true);

		//remove required item
		if(pItem && pTimeQuestTbldat->sTimeQuestDataset[byDifficulty].needItemTblidx != INVALID_TBLIDX)
			pItem->SetCount(pItem->GetCount() - 1, false, true);
	}
	else
	{
		SAFE_DELETE(pTimeQuest);
		return GAME_TIMEQUEST_ERROR;
	}

	return GAME_SUCCESS;
}


CTimeQuest* CDungeonManager::FindTimeQuest(PARTYID partyid)
{
	std::map<ROOMID, CTimeQuest*>::iterator it = m_mapTimeQuest.find(partyid);

	if (m_mapTimeQuest.end() == it){
		return NULL;
	}

	return it->second;
}



CTimeLeapDungeon* CDungeonManager::CreateTimeLeapDungeon(CPlayer* pOwner, TBLIDX dungeonTblidx)
{
	sDUNGEON_TBLDAT* pDungeon = (sDUNGEON_TBLDAT*)g_pTableContainer->GetDungeonTable()->FindData(dungeonTblidx);
	if (pDungeon == NULL)
	{
		printf("Could not find dungeon tblidx %u \n", dungeonTblidx);
		return NULL;
	}

	//TODO: Checks if player has valid loc and can enter dungeon... etc..

	// a leftover instance would stay alive forever once the map entry is overwritten
	if (CTimeLeapDungeon* pOld = FindTimeLeapDungeon(pOwner->GetCharID()))
	{
		if (pOwner->GetTLQ() == pOld)
			pOwner->SetTLQ(NULL);

		DestroyTimeLeapDungeon(pOwner->GetCharID(), pOld);
	}

	CTimeLeapDungeon* pTLD = new CTimeLeapDungeon(pOwner, pDungeon);

	if (pTLD->Create())
	{
		m_mapTimeLeapDungeon.insert({ pOwner->GetCharID(), pTLD });

		return pTLD;
	}

	SAFE_DELETE(pTLD);

	return NULL;
}


CTimeLeapDungeon* CDungeonManager::FindTimeLeapDungeon(CHARACTERID charID)
{
	std::map<CHARACTERID, CTimeLeapDungeon*>::iterator it = m_mapTimeLeapDungeon.find(charID);

	if (m_mapTimeLeapDungeon.end() == it){
		return NULL;
	}

	return it->second;
}


void CDungeonManager::DestroyTimeLeapDungeon(CHARACTERID charID, CTimeLeapDungeon* pDungeon)
{
	m_mapTimeLeapDungeon.erase(charID);
	SAFE_DELETE(pDungeon);
}




CUltimateDungeon* CDungeonManager::CreateUltimateDungeon(CPlayer* pOwner, GROUPID groupid, BYTE byDifficulty)
{
	sDUNGEON_TBLDAT* pDungeon = (sDUNGEON_TBLDAT*)g_pTableContainer->GetDungeonTable()->FindDataByGroupIdx(groupid, byDifficulty);
	if (pDungeon == NULL)
	{
		printf("Could not find dungeon groupid %u byDifficulty %u\n", groupid, byDifficulty);
		return NULL;
	}

	if (pOwner->GetLevel() < pDungeon->byMinLevel || pOwner->GetLevel() > pDungeon->byMaxLevel)
		return NULL;

	if (FindUltimateDungeon(pOwner->GetPartyID()) != NULL)
		return NULL;

	CUltimateDungeon* pUD = new CUltimateDungeon(pOwner->GetPartyID(), pDungeon);

	if (pUD->Create(pOwner))
	{
		m_mapUltimateDungeon.insert({ pOwner->GetPartyID(), pUD });

		return pUD;
	}

	SAFE_DELETE(pUD);

	return NULL;
}


CUltimateDungeon* CDungeonManager::FindUltimateDungeon(PARTYID partyID)
{
	std::map<PARTYID, CUltimateDungeon*>::iterator it = m_mapUltimateDungeon.find(partyID);

	if (m_mapUltimateDungeon.end() == it){
		return NULL;
	}

	return it->second;
}


void CDungeonManager::DestroyUltimateDungeon(CUltimateDungeon* pDungeon)
{
	m_mapUltimateDungeon.erase(pDungeon->GetPartyID());
	SAFE_DELETE(pDungeon);
}

CBattleDungeon * CDungeonManager::CreateBattleDungeon(CPlayer * pOwner, WORD & rwResultcode, BYTE byBeginStage)
{
	if (FindBattleDungeon(pOwner->GetPartyID()) != NULL)
	{
		rwResultcode = GAME_BATTLE_DUNGEON_FAIL;
		return NULL;
	}

	CBattleDungeon* pCCBD = new CBattleDungeon(pOwner->GetPartyID());

	if (pCCBD->Create(pOwner, byBeginStage))
	{
		m_mapBattleDungeon.insert({ pOwner->GetPartyID(), pCCBD });

		return pCCBD;
	}

	SAFE_DELETE(pCCBD);

	return NULL;
}

CBattleDungeon * CDungeonManager::FindBattleDungeon(PARTYID partyID)
{
	std::map<PARTYID, CBattleDungeon*>::iterator it = m_mapBattleDungeon.find(partyID);

	if (m_mapBattleDungeon.end() == it) {
		return NULL;
	}

	return it->second;
}

void CDungeonManager::DestroyBattleDungeon(CBattleDungeon * pDungeon)
{
	m_mapBattleDungeon.erase(pDungeon->GetPartyID());
	SAFE_DELETE(pDungeon);
}

CDwcDungeon * CDungeonManager::CreateDwcDungeon(const std::vector<CPlayer*>& rMembers, sDWC_TBLDAT * pDwcTbldat, HOBJECT hNpc, bool bProposal/* = true*/)
{
	CDwcDungeon* pDwc = new CDwcDungeon(++m_dwLastDwcDungeonID, pDwcTbldat, hNpc);

	if (pDwc->Create(rMembers, bProposal))
	{
		m_mapDwcDungeon.insert({ pDwc->GetID(), pDwc });

		return pDwc;
	}

	SAFE_DELETE(pDwc);

	return NULL;
}

WORD CDungeonManager::StartQuestDwc(CPlayer * pPlayer, TBLIDX dwcTblidx)
{
	sDWC_TBLDAT* pDwcTbldat = (sDWC_TBLDAT*)g_pTableContainer->GetDwcTable()->FindData(dwcTblidx);
	if (pDwcTbldat == NULL || IsDwcPlayable(pDwcTbldat) == false)
		return DWC_FAIL;

	// the npc invites the avatar, only the admission conditions are left out
	if (pPlayer->GetDWC() || pPlayer->GetTMQ() || pPlayer->GetTLQ() || pPlayer->GetUD() || pPlayer->GetCCBD())
		return GAME_CHAR_IS_WRONG_STATE;
	if (pPlayer->GetCurWorld() == NULL || pPlayer->GetCurWorld()->GetRuleType() != GAMERULE_NORMAL)
		return GAME_COMMON_CAN_NOT_DO_HERE;
	if (pPlayer->GetLevel() < pDwcTbldat->byLevel_Min)
		return GAME_TOO_LOW_MEMBER_LEVEL;
	if (pPlayer->GetLevel() > pDwcTbldat->byLevel_Max)
		return GAME_TOO_HIGHT_MEMBER_LEVEL;
	if (pPlayer->GetDwcRemainCount() == 0)
		return GAME_COMMON_EXIST_USER_WHO_OVER_LIMIT_COUNT;

	std::vector<CPlayer*> vecMembers;
	vecMembers.push_back(pPlayer);

	// the quest sends the avatar in straight away, without the entry question and the enter direction
	if (CreateDwcDungeon(vecMembers, pDwcTbldat, INVALID_HOBJECT, false) == NULL)
		return DWC_FAIL;

	return GAME_SUCCESS;
}

WORD CDungeonManager::GetDwcUseCount(TBLIDX dwcTblidx)
{
	sDWC_TBLDAT* pDwcTbldat = (sDWC_TBLDAT*)g_pTableContainer->GetDwcTable()->FindData(dwcTblidx);
	if (pDwcTbldat == NULL)
		return 0;

	sWORLD_TBLDAT* pWorldTbldat = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(pDwcTbldat->worldTblidx);
	if (pWorldTbldat == NULL || pWorldTbldat->nCreateCount <= 0)
		return 0;

	int nRunning = 0;
	for (std::map<DWORD, CDwcDungeon*>::iterator it = m_mapDwcDungeon.begin(); it != m_mapDwcDungeon.end(); it++)
	{
		if (it->second->GetTbldat()->tblidx == dwcTblidx && it->second->GetWorld())
			nRunning++;
	}

	int nPercent = nRunning * 100 / pWorldTbldat->nCreateCount;

	return (WORD)(nPercent > 100 ? 100 : nPercent);
}

bool CDungeonManager::IsDwcPlayable(sDWC_TBLDAT * pDwcTbldat)
{
	sWORLD_TBLDAT* pWorldTbldat = (sWORLD_TBLDAT*)g_pTableContainer->GetWorldTable()->FindData(pDwcTbldat->worldTblidx);
	if (pWorldTbldat == NULL || pWorldTbldat->wpsLinkIndex == INVALID_TBLIDX)
		return false;

	if (g_pTableContainer->GetWorldPlayTable()->FindData(pWorldTbldat->wpsLinkIndex) == NULL)
		return false;

	// dwc 3 (70200) and dwc 4 (70300) are empty placeholders in the tw data, the retail scripts never surfaced:
	// a script with nothing but its stage node is not offered
	CControlScript* pScript = g_pWorldPlayScriptManager->FindScript(pWorldTbldat->wpsLinkIndex);
	return pScript != NULL && pScript->GetScriptNodeCount() > 1;
}



