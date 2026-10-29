#include "stdafx.h"
#include "ScriptAlgoBase.h"
#include "ScriptAlgoObjectManager.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"


CWpsAlgoObject::CWpsAlgoObject(TBLIDX tblidx, sWORLDPLAY_TBLDAT* pWorldPlayTbldat, DWORD dwHandle) :
	CScriptAlgoObject(tblidx, dwHandle, SSD_SCRIPT_TYPE_WPS)
{
	m_pWorldPlayTbldat = pWorldPlayTbldat;

	Init();
}

CWpsAlgoObject::~CWpsAlgoObject()
{
}


void CWpsAlgoObject::Init()
{
	m_byCCBDStage = 1;
	m_bCCBDTimeLimit = false;
	m_bCCBDFail = false;
	m_byCCBDStartStage = m_byCCBDStage;
}


void CWpsAlgoObject::Start()
{
	m_scriptAlgoController.ChangeControlStage_Stage(0, SSD_SCRIPT_TYPE_WPS);
	
	CScriptAlgoObject::Start();
}

void CWpsAlgoObject::Stop(bool bEnterFailStage/* = false*/, bool bRemoveFromWorld/* = true*/)
{
	if (bEnterFailStage)
	{
		GetController()->ChangeControlStage_Failed();
		return;
	}

	CScriptAlgoObject::Stop(bEnterFailStage, bRemoveFromWorld);
}


int CWpsAlgoObject::OnUpdate(DWORD dwTickDiff)
{
	int nStatus = CScriptAlgoObject::OnUpdate(dwTickDiff);

	UpdateEventStatusMobCount();

	if (nStatus == CComplexState::FAILED || nStatus == CComplexState::SYSTEMERROR)
	{
	//	printf("CWpsAlgoObject::OnUpdate STOP! reason SYSTEMERROR\n");
		GetController()->ChangeControlStage_Failed();
	}

	return nStatus;
}


bool CWpsAlgoObject::NotifyPlayerLeave(HOBJECT hPlayer, bool bRemoveScriptFromPlayer/* = true*/)
{
	if (CScriptAlgoObject::NotifyPlayerLeave(hPlayer, bRemoveScriptFromPlayer) == false)
		return false;

	if (m_setPlayers.size() == 0 
		&& (GetExecuterType() == WORLDPLAY_EXECUTER_TS
		|| GetExecuterType() == WORLDPLAY_EXECUTER_ITEM_USE_BY_LOCATION)) //if no players anymore connected then stop the script. Does not USE for scripts hosted on world.
	{
	//	printf("stop script \n");
		Stop(true);
	}

	return true;
}

void CWpsAlgoObject::SendToPlayers(CNtlPacket * pPacket)
{
	CPlayer* pPlayer = GetPlayersFirst();
	while (pPlayer)
	{
		if (GetWorld() && pPlayer->GetWorldID() == GetWorld()->GetID())
			pPlayer->SendPacket(pPacket);

		pPlayer = GetPlayersNext();
	}
}


CDwcDungeon * CWpsAlgoObject::GetDwcDungeon()
{
	CPlayer* pPlayer = GetPlayersFirst();
	while (pPlayer)
	{
		if (pPlayer->GetDWC() && GetWorld() && pPlayer->GetWorldID() == GetWorld()->GetID())
			return pPlayer->GetDWC();

		pPlayer = GetPlayersNext();
	}

	return NULL;
}


void CWpsAlgoObject::RegisterEventStatusMobCount(BYTE byGroup, BYTE byIndex, SPAWNGROUPID spawnGroupId, WORD wMaxCount)
{
	UnregisterEventStatus(byGroup, byIndex);

	sEVENTSTATUS_MOB_COUNT sMobCount;
	sMobCount.byGroup = byGroup;
	sMobCount.byIndex = byIndex;
	sMobCount.spawnGroupId = spawnGroupId;
	sMobCount.wMaxCount = wMaxCount;
	sMobCount.wCount = 0;

	m_vecEventStatusMobCount.push_back(sMobCount);
}


void CWpsAlgoObject::UnregisterEventStatus(BYTE byGroup, BYTE byIndex)
{
	for (std::vector<sEVENTSTATUS_MOB_COUNT>::iterator it = m_vecEventStatusMobCount.begin(); it != m_vecEventStatusMobCount.end(); )
	{
		if (it->byGroup == byGroup && it->byIndex == byIndex)
			it = m_vecEventStatusMobCount.erase(it);
		else
			++it;
	}
}


void CWpsAlgoObject::UnregisterEventStatusGroup(BYTE byGroup)
{
	for (std::vector<sEVENTSTATUS_MOB_COUNT>::iterator it = m_vecEventStatusMobCount.begin(); it != m_vecEventStatusMobCount.end(); )
	{
		if (it->byGroup == byGroup)
			it = m_vecEventStatusMobCount.erase(it);
		else
			++it;
	}
}


// kill counts of the mob count event status shown in the client
void CWpsAlgoObject::UpdateEventStatusMobCount()
{
	for (std::vector<sEVENTSTATUS_MOB_COUNT>::iterator it = m_vecEventStatusMobCount.begin(); it != m_vecEventStatusMobCount.end(); it++)
	{
		DWORD dwKilled = GetSharedRemovedSpawnGroupCount(it->spawnGroupId);
		WORD wCount = (WORD)(dwKilled > it->wMaxCount ? it->wMaxCount : dwKilled);

		if (wCount == it->wCount)
			continue;

		it->wCount = wCount;

		CNtlPacket packet(sizeof(sGU_SERVERSCRIPT_EVENTSTATUS_UPDATE_NFY));
		sGU_SERVERSCRIPT_EVENTSTATUS_UPDATE_NFY* res = (sGU_SERVERSCRIPT_EVENTSTATUS_UPDATE_NFY*)packet.GetPacketData();
		res->wOpCode = GU_SERVERSCRIPT_EVENTSTATUS_UPDATE_NFY;
		res->byGroup = it->byGroup;
		res->byIndex = it->byIndex;
		res->wCount = wCount;
		packet.SetPacketLen(sizeof(sGU_SERVERSCRIPT_EVENTSTATUS_UPDATE_NFY));
		SendToPlayers(&packet);
	}
}
