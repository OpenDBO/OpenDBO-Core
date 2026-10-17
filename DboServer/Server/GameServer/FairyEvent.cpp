#include "stdafx.h"
#include "FairyEvent.h"
#include "GameServer.h"
#include "NtlPacketGU.h"
#include "NtlStringW.h"
#include "ObjectManager.h"
#include "NtlAdmin.h"
#include "Monster.h"
#include "NtlRandom.h"
#include "TableContainerManager.h"
#include "ItemManager.h"
#include "ItemDrop.h"


#define FAIRY_EVENT_DROP_ITEM				11160029
#define FAIRY_EVENT_SPAWN_RATE				5	// % chance per kill
#define FAIRY_EVENT_CARP_EVERY				50	// every Nth summon is a carp instead of a fairy
#define FAIRY_EVENT_MAX_LEVEL_GAP			10

// mobs summoned by level bracket: 1-9, 10-19, 20-29, 30-39, 40-49, 50-59, 60+
static const TBLIDX s_aFairyMob[] = { 16454203, 16454202, 16454201, 16454104, 16454103, 16454102, 16454101 };
static const TBLIDX s_aCarpMob[] = { 13211201, 13211202, 13211203, 13211204, 13211205, 13211206, 13211207 };
static const int FAIRY_EVENT_BRACKET_COUNT = _countof(s_aFairyMob);


static bool IsInList(TBLIDX tblidx, const TBLIDX* aList)
{
	for (int i = 0; i < FAIRY_EVENT_BRACKET_COUNT; i++)
	{
		if (aList[i] == tblidx)
			return true;
	}

	return false;
}


CFairyEvent::CFairyEvent()
{
	Init();
}

CFairyEvent::~CFairyEvent()
{
}

void CFairyEvent::Init()
{
	m_bOn = false;
	m_timeStart = 0;
	m_timeEnd = 0;
	m_dwNextUpdateTick = 0;
	m_nFairySummoned = 0;
}


void CFairyEvent::StartEvent(BYTE byHours/* = 3*/)
{
	if (m_bOn)
		return;

	if (byHours == 0)
		byHours = 3;

	CGameServer* app = (CGameServer*)g_pApp;

	m_bOn = true;
	m_timeStart = app->GetTime();
	m_timeEnd = m_timeStart + (byHours * 3600);

	CNtlStringW msg;

	CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT * res = (sGU_SYSTEM_DISPLAY_TEXT*)packet.GetPacketData();
	res->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	res->wMessageLengthInUnicode = (WORD)msg.Format(L"Fairy Event Started. Kill Monsters to summon Fairies. You have %u hours time to farm", byHours);
	res->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(res->awchMessage, msg.c_str());
	packet.SetPacketLen(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	g_pObjectManager->SendPacketToAll(&packet);
}

DBOTIME CFairyEvent::GetRemainingTime()
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (m_bOn == false || app->GetTime() >= m_timeEnd)
		return 0;

	return m_timeEnd - app->GetTime();
}

int CFairyEvent::GetSummonsUntilCarp()
{
	return FAIRY_EVENT_CARP_EVERY - m_nFairySummoned;
}

void CFairyEvent::TickProcess(DWORD dwTick)
{
	CGameServer* app = (CGameServer*)g_pApp;

	if (dwTick < m_dwNextUpdateTick)
		return;

	if (m_bOn)
	{
		if (app->GetTime() >= m_timeEnd)
		{
			EndEvent();
		}
	}

	m_dwNextUpdateTick = dwTick + 5000; //update again in 5 seconds
}

void CFairyEvent::Update(CMonster * pMob, CCharacter * pPlayer)
{
	if (m_bOn == false)
		return;

	if (pPlayer->GetCurWorld() == NULL)
		return;
	if (pPlayer->GetCurWorld()->GetRuleType() != GAMERULE_NORMAL)
		return;

	// fairy killed: one drop owned by the killer
	if (IsInList(pMob->GetTblidx(), s_aFairyMob))
	{
		DropItem(pMob, pPlayer, 90.f, 2.0f);
		return;
	}

	// carp killed: (level + 1) free-for-all drops scattered around it
	if (IsInList(pMob->GetTblidx(), s_aCarpMob))
	{
		for (int i = 0; i <= pMob->GetLevel(); i++)
			DropItem(pMob, NULL, 80.f, 30.0f);

		return;
	}

	if (abs(pPlayer->GetLevel() - pMob->GetLevel()) > FAIRY_EVENT_MAX_LEVEL_GAP)
		return;

	if (Dbo_CheckProbability(FAIRY_EVENT_SPAWN_RATE) == false)
		return;

	int nBracket = pMob->GetLevel() / 10;
	if (nBracket >= FAIRY_EVENT_BRACKET_COUNT)
		nBracket = FAIRY_EVENT_BRACKET_COUNT - 1;

	if (++m_nFairySummoned >= FAIRY_EVENT_CARP_EVERY)
	{
		m_nFairySummoned = 0;
		SpawnMob(pMob, s_aCarpMob[nBracket]);
	}
	else
	{
		SpawnMob(pMob, s_aFairyMob[nBracket]);
	}
}

void CFairyEvent::SpawnMob(CMonster * pMob, TBLIDX mobTblidx)
{
	sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(mobTblidx);
	if (pTbldat == NULL)
		return;

	sMOB_DATA data;
	InitMobData(data);

	data.worldID = pMob->GetWorldID();
	data.worldtblidx = pMob->GetWorldTblidx();

	data.tblidx = pTbldat->tblidx;

	pMob->GetCurLoc().CopyTo(data.vCurLoc);
	pMob->GetCurLoc().CopyTo(data.vSpawnLoc);
	pMob->GetCurDir().CopyTo(data.vCurDir);
	pMob->GetCurDir().CopyTo(data.vSpawnDir);

	data.actionpatternTblIdx = 1;

	CMonster* pNewMob = (CMonster*)g_pObjectManager->CreateCharacter(OBJTYPE_MOB);
	if (pNewMob)
	{
		if (pNewMob->CreateDataAndSpawn(data, pTbldat))
		{
			pNewMob->SetStandAlone(true);
		}
	}
}

void CFairyEvent::DropItem(CMonster * pMob, CCharacter * pOwner, float fRate, float fRange)
{
	CItemDrop* pDrop = g_pItemManager->CreateSingleDrop(fRate, FAIRY_EVENT_DROP_ITEM);
	if (pDrop == NULL)
		return;

	sVECTOR3 pos;
	pos.x = pMob->GetCurLoc().x + RandomRangeF(-fRange, fRange);
	pos.y = pMob->GetCurLoc().y;
	pos.z = pMob->GetCurLoc().z + RandomRangeF(-fRange, fRange);

	pDrop->SetNeedToIdentify(false);
	if (pOwner)
		pDrop->SetOwnership(pOwner->GetID(), pOwner->GetPartyID());
	pDrop->StartDestroyEvent();
	pDrop->AddToGround(pMob->GetWorldID(), pos);
}

void CFairyEvent::EndEvent()
{
	if (m_bOn == false)
		return;

	Init();

	CNtlStringW msg;

	CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT * res = (sGU_SYSTEM_DISPLAY_TEXT*)packet.GetPacketData();
	res->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	res->wMessageLengthInUnicode = (WORD)msg.Format(L"Fairy Event Ended");
	res->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(res->awchMessage, msg.c_str());
	packet.SetPacketLen(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	g_pObjectManager->SendPacketToAll(&packet);
}

void CFairyEvent::LoadEvent(HSESSION hSession)
{
	if (m_bOn == false)
		return;

	CNtlStringW msg;

	CNtlPacket packetMsg(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT * resMsg = (sGU_SYSTEM_DISPLAY_TEXT *)packetMsg.GetPacketData();
	resMsg->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	resMsg->byDisplayType = SERVER_TEXT_EMERGENCY;
	resMsg->wMessageLengthInUnicode = (WORD)msg.Format(L"Fairy Event is currently running!");
	NTL_SAFE_WCSCPY(resMsg->awchMessage, msg.c_str());
	g_pApp->Send(hSession, &packetMsg);
}
