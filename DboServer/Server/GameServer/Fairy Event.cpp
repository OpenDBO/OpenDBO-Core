#include "stdafx.h"
#include "Fairy Event.h"
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
	m_nMonsterSummoned = 0;
}


void CFairyEvent::StartEvent(BYTE byHours/* = 3*/)
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
	res->wMessageLengthInUnicode = (WORD)msg.Format(L"Fairy Event Started. Kill Monsters to summon Fairy. You have %u hours to farm", byHours);
	res->byDisplayType = SERVER_TEXT_EMERGENCY;
	NTL_SAFE_WCSCPY(res->awchMessage, msg.c_str());
	packet.SetPacketLen(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	g_pObjectManager->SendPacketToAll(&packet);
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

void CFairyEvent::Update(CMonster* pMob, CCharacter* pPlayer)
{
	if (m_bOn == false)
		return;

	if (pPlayer->GetCurWorld() == NULL)
		return;
	if (pPlayer->GetCurWorld()->GetRuleType() != GAMERULE_NORMAL)
		return;	
	

	//Fairy Drop´s
	if (pMob->GetTblidx() == 16454101 || pMob->GetTblidx() == 16454102 || pMob->GetTblidx() == 16454103 ||
		pMob->GetTblidx() == 16454104 || pMob->GetTblidx() == 16454201 || pMob->GetTblidx() == 16454202 ||
		pMob->GetTblidx() == 16454203)
	{
		CItemDrop* pDrop = g_pItemManager->CreateSingleDrop(90.f, 11160029);
		if (pDrop)
		{
			sVECTOR3 pos;
			pos.x = pMob->GetCurLoc().x + RandomRangeF(-2.0f, 2.0f);
			pos.y = pMob->GetCurLoc().y;
			pos.z = pMob->GetCurLoc().z + RandomRangeF(-2.0f, 2.0f);

			pDrop->SetNeedToIdentify(false);
			pDrop->SetOwnership(pPlayer->GetID(), pPlayer->GetPartyID());
			pDrop->StartDestroyEvent();
			pDrop->AddToGround(pMob->GetWorldID(), pos);
		}

		return;
	}	
	//Fairy Drop´s
	if (pMob->GetTblidx() == 13211201 || pMob->GetTblidx() == 13211202 || pMob->GetTblidx() == 13211203 ||
		pMob->GetTblidx() == 13211204 || pMob->GetTblidx() == 13211205 || pMob->GetTblidx() == 13211206 ||
		pMob->GetTblidx() == 13211207)
	{
		for (int i = 0; i <= pMob->GetLevel(); i++)
		{
			CItemDrop* pDrop = g_pItemManager->CreateSingleDrop(80.f, 11160029);
			if (pDrop)
			{
				sVECTOR3 pos;
				pos.x = pMob->GetCurLoc().x + RandomRangeF(-30.0f, 30.0f);
				pos.y = pMob->GetCurLoc().y;
				pos.z = pMob->GetCurLoc().z + RandomRangeF(-30.0f, 30.0f);

				pDrop->SetNeedToIdentify(false);
				//pDrop->SetOwnership(pPlayer->GetID(), pPlayer->GetPartyID());
				pDrop->StartDestroyEvent();
				pDrop->AddToGround(pMob->GetWorldID(), pos);
			}			
		}	
		return;
	}
	int l_LevelGap = abs(pPlayer->GetLevel() - pMob->GetLevel());
	if (l_LevelGap > 10)
		return;
	BYTE SpawnType; //Fairy 1 Magicarp 2	
	if (Dbo_CheckProbability(5) == false)
		return;
	else
	{
		SpawnType = 1;
		FairyKilled++;
		if (FairyKilled >= 50)
		{
			SpawnType = 2;
			FairyKilled = 0;
		}
	}
	
	if (SpawnType == 1)
	{
		if (pMob->GetLevel() >= 1 && pMob->GetLevel() < 10)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(16454203);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 10 && pMob->GetLevel() < 20)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(16454202);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 20 && pMob->GetLevel() < 30)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(16454201);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 30 && pMob->GetLevel() < 40)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(16454104);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 40 && pMob->GetLevel() < 50)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(16454103);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 50 && pMob->GetLevel() < 60)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(16454102);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 60 && pMob->GetLevel() < 101)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(16454101);
			if (pTbldat)
			{
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
		}
	}
	if (SpawnType == 2)
	{
		if (pMob->GetLevel() >= 1 && pMob->GetLevel() < 10)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(13211201);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 10 && pMob->GetLevel() < 20)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(13211202);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 20 && pMob->GetLevel() < 30)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(13211203);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 30 && pMob->GetLevel() < 40)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(13211204);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 40 && pMob->GetLevel() < 50)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(13211205);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 50 && pMob->GetLevel() < 60)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(13211206);
			if (pTbldat)
			{
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
		}
		if (pMob->GetLevel() >= 60 && pMob->GetLevel() < 101)
		{
			sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(13211207);
			if (pTbldat)
			{
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
		}
	}
}

void CFairyEvent::EndEvent()
{
	if (m_bOn == false)
		return;

	Init();

	CNtlStringW msg;

	CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT* res = (sGU_SYSTEM_DISPLAY_TEXT*)packet.GetPacketData();
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
	sGU_SYSTEM_DISPLAY_TEXT* resMsg = (sGU_SYSTEM_DISPLAY_TEXT*)packetMsg.GetPacketData();
	resMsg->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	resMsg->byDisplayType = SERVER_TEXT_EMERGENCY;
	resMsg->wMessageLengthInUnicode = (WORD)msg.Format(L"Fairy Event is currently running!");
	NTL_SAFE_WCSCPY(resMsg->awchMessage, msg.c_str());
	g_pApp->Send(hSession, &packetMsg);
}
