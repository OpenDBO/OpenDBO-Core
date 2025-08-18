#include "stdafx.h"
#include "StoneDropEvent.h"
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


CStoneDropEvent::CStoneDropEvent()
{
	Init();
}

CStoneDropEvent::~CStoneDropEvent()
{
}

void CStoneDropEvent::Init()
{
	m_bOn = false;
	m_timeStart = 0;
	m_timeEnd = 0;
	m_dwNextUpdateTick = 0;
	m_nMonsterSummoned = 0;
}


void CStoneDropEvent::StartEvent(BYTE byHours/* = 3*/)
{
	if (m_bOn)
		return;

	CGameServer* app = (CGameServer*)g_pApp;

	m_bOn = true;
	m_timeStart = app->GetTime();
	m_timeEnd = m_timeStart + (byHours * 3600);

	CNtlStringW msg;

	CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT* res = (sGU_SYSTEM_DISPLAY_TEXT*)packet.GetPacketData();
	res->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	res->wMessageLengthInUnicode = (WORD)msg.Format(L"Double Stone Drop Event Started! Duration: %u Hours.", byHours);
	res->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(res->awchMessage, msg.c_str());
	packet.SetPacketLen(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	g_pObjectManager->SendPacketToAll(&packet);
}


void CStoneDropEvent::TickProcess(DWORD dwTick)
{
	//CGameServer* app = (CGameServer*)g_pApp;
	//if (dwTick < m_dwNextUpdateTick)
	//	return;

	//DBOTIME curTime = app->GetTime();
	//tm timeStruct = {};
	//localtime_s(&timeStruct, &curTime);

	//if (!m_bOn)
	//{
	//	for (int i = 0; i < 7; i++)
	//	{
	//		if (m_aEventStartInfo.bIsOpen[i])
	//		{
	//			if (timeStruct.tm_wday == i)
	//			{
	//				if (timeStruct.tm_hour >= m_aEventStartInfo.byStartTime && timeStruct.tm_hour < m_aEventStartInfo.byEndTime)
	//				{
	//					StartEvent(abs(m_aEventStartInfo.byStartTime - m_aEventStartInfo.byEndTime));
	//				}
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	if (app->GetTime() >= m_timeEnd)
	//	{
	//		EndEvent();
	//	}
	//}

	//m_dwNextUpdateTick = dwTick + 300000; //update again in 5 seconds

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

void CStoneDropEvent::Update(CMonster* pMob, CCharacter* pPlayer)
{
	if (!m_bOn)
		return;

	if (pPlayer->GetCurWorld() == NULL)
		return;
	if (pPlayer->GetCurWorld()->GetRuleType() != GAMERULE_NORMAL)
		return;

	int stoneLevels[] = { 6, 11, 16, 21, 26, 30, 34, 38, 42, 46, 50, 53, 55, 58, 60, 65, 70 };
	const int stoneLevelsSize = sizeof(stoneLevels) / sizeof(stoneLevels[0]);

	BYTE byMobLevel = pMob->GetLevel();

	for (int i = 0; i < stoneLevelsSize; ++i)
	{
		const int stoneLevel = stoneLevels[i];

		if (stoneLevel < byMobLevel)
			continue;

		if (stoneLevel > byMobLevel + 10)
		    break;

		if (Dbo_CheckProbabilityF(CalculateBaseDropChance()))
		{
			if (Dbo_CheckProbability(RED_DROP_RATE))
				CreateSingleDrop(pMob, pPlayer, GetRedStoneByLevel(stoneLevel));

			if (Dbo_CheckProbability(BLUE_DROP_RATE))
				CreateSingleDrop(pMob, pPlayer, GetBlueStoneByLevel(stoneLevel));

			if (Dbo_CheckProbability(PURPLE_DROP_RATE))
				CreateSingleDrop(pMob, pPlayer, GetPurpleStoneByLevel(stoneLevel));

			if (Dbo_CheckProbability(GREEN_DROP_RATE))
				CreateSingleDrop(pMob, pPlayer, GetGreenStoneByLevel(stoneLevel));

			if (Dbo_CheckProbability(BLACK_BLUE_DROP_RATE))
				CreateSingleDrop(pMob, pPlayer, GetBlackBlueStoneByLevel(stoneLevel));

			if (Dbo_CheckProbability(BLACK_RED_DROP_RATE))
				CreateSingleDrop(pMob, pPlayer, GetBlackRedStoneByLevel(stoneLevel));

			if (Dbo_CheckProbability(WHITE_DROP_RATE))
				CreateSingleDrop(pMob, pPlayer, GetWhiteStoneByLevel(stoneLevel));
		}
	}
}

void CStoneDropEvent::EndEvent()
{
	if (!m_bOn)
		return;

	Init();
	CNtlStringW msg;
	CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT* res = (sGU_SYSTEM_DISPLAY_TEXT*)packet.GetPacketData();
	res->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	res->wMessageLengthInUnicode = (WORD)msg.Format(L"Double Stone Drop Event ended!");
	res->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(res->awchMessage, msg.c_str());
	packet.SetPacketLen(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	g_pObjectManager->SendPacketToAll(&packet);
}

void CStoneDropEvent::LoadEvent(HSESSION hSession)
{
	if (!m_bOn)
		return;

	CNtlStringW msg;
	CNtlPacket packetMsg(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT* resMsg = (sGU_SYSTEM_DISPLAY_TEXT*)packetMsg.GetPacketData();

	resMsg->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	resMsg->byDisplayType = SERVER_TEXT_EMERGENCY;
	resMsg->wMessageLengthInUnicode = (WORD)msg.Format(L"Double Stone Drop Event is currently running!");
	NTL_SAFE_WCSCPY(resMsg->awchMessage, msg.c_str());
	g_pApp->Send(hSession, &packetMsg);
}

float CStoneDropEvent::CalculateBaseDropChance()
{
	CGameServer* app = (CGameServer*)g_pApp;
	int nBaseMultiplier = m_bOn ? 2.f : 1.f;

	return 10.f * nBaseMultiplier * ((float)app->GetStoneDropRate() / 100.f);
}

int CStoneDropEvent::GetRedStoneByLevel(int stoneLevel)
{
	auto it = redStonesConstantMap.find(stoneLevel);
	if (it != redStonesConstantMap.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}

int CStoneDropEvent::GetBlueStoneByLevel(int stoneLevel)
{
	auto it = blueStonesConstantMap.find(stoneLevel);
	if (it != blueStonesConstantMap.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}

int CStoneDropEvent::GetPurpleStoneByLevel(int stoneLevel)
{
	auto it = purpleStonesConstantMap.find(stoneLevel);
	if (it != purpleStonesConstantMap.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}

int CStoneDropEvent::GetGreenStoneByLevel(int stoneLevel)
{
	auto it = greenStonesConstantMap.find(stoneLevel);
	if (it != greenStonesConstantMap.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}

int CStoneDropEvent::GetBlackBlueStoneByLevel(int stoneLevel)
{
	auto it = blackBlueStonesConstantMap.find(stoneLevel);
	if (it != blackBlueStonesConstantMap.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}

int CStoneDropEvent::GetBlackRedStoneByLevel(int stoneLevel)
{
	auto it = blackRedStonesConstantMap.find(stoneLevel);
	if (it != blackRedStonesConstantMap.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}

int CStoneDropEvent::GetWhiteStoneByLevel(int stoneLevel)
{
	auto it = whiteStonesConstantMap.find(stoneLevel);
	if (it != whiteStonesConstantMap.end()) {
		return it->second;
	}
	else {
		return NULL;
	}
}

void CStoneDropEvent::CreateSingleDrop(CMonster* pMob, CCharacter* pPlayer, int dropId)
{
	if (dropId != NULL)
	{
		HOBJECT pPlayerId = pPlayer->GetID();
		HOBJECT pPlayerPartyId = pPlayer->GetPartyID();
		WORLDID nWorldId = pMob->GetWorldID();
		CItemDrop* pDrop = g_pItemManager->CreateSingleDrop(100.f, dropId);

		if (pDrop)
		{
			sVECTOR3 pos;
			pos.x = pMob->GetCurLoc().x + RandomRangeF(-2.0f, 2.0f);
			pos.y = pMob->GetCurLoc().y;
			pos.z = pMob->GetCurLoc().z + RandomRangeF(-2.0f, 2.0f);

			pDrop->SetNeedToIdentify(false);
			pDrop->SetOwnership(pPlayerId, pPlayerPartyId);
			pDrop->StartDestroyEvent();
			pDrop->AddToGround(nWorldId, pos);
		}
	}
}
