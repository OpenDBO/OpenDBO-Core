#include "stdafx.h"
#include "DynamicFieldSystemEvent.h"
#include "TableContainerManager.h"
#include "SystemEffectTable.h"
#include "UseItemTable.h"
#include "ItemManager.h"
#include "ItemDrop.h"
#include "Monster.h"
#include "NtlRandom.h"
#include "NtlPacketGU.h"
#include "NtlPacketGQ.h"
#include "NtlAdmin.h"
#include "ScriptAlgoObjectManager.h"
#include "GameServer.h"
#include "GameMain.h"
#include "WpsAlgoObject.h"


namespace
{
	// defaults, overridden by the EVENT_SYSTEM_TYPE_COUNTING / EVENT_SYSTEM_ACTION_WPS row of Table_Event_System_Dynamic_Data
	const TBLIDX	DYNAMIC_FIELD_DEFAULT_WPS_TBLIDX	= 80002;	// WPS script which spawns the field bosses
	const DWORD		DYNAMIC_FIELD_DEFAULT_MAX_COUNT		= 20000;
	const DWORD		DYNAMIC_FIELD_DEFAULT_EVENT_TIME	= DBO_DYNAMIC_FIELD_EVENT_TIME;

	const BYTE		DYNAMIC_FIELD_BOSS_COUNT			= 12;		// amount of bosses spawned by the WPS script
	const BYTE		DYNAMIC_FIELD_MIN_MOB_LEVEL			= 40;		// event table rows only apply to monsters with at least this level
	const int		DYNAMIC_FIELD_COUNTING_FLUSH		= 10;		// send kills to the query server at X pending kills ...
	const DWORD		DYNAMIC_FIELD_COUNTING_INTERVAL		= 10000;	// ... or every X ms, whatever comes first
	const DWORD		DYNAMIC_FIELD_STATUS_INTERVAL		= 60000;	// broadcast event status every X ms while the event runs
}


CDynamicFieldSystemEvent::CDynamicFieldSystemEvent()
{
	Init();
}


CDynamicFieldSystemEvent::~CDynamicFieldSystemEvent()
{
	Destroy();
}


void CDynamicFieldSystemEvent::Init()
{
	m_nAddCount = 0;
	m_dwCurCount = 0;
	m_byBossCount = 0;
	m_dwMaxCount = DYNAMIC_FIELD_DEFAULT_MAX_COUNT;
	m_dwEventTimeSecs = DYNAMIC_FIELD_DEFAULT_EVENT_TIME;
	m_wpsTblidx = DYNAMIC_FIELD_DEFAULT_WPS_TBLIDX;
	m_dwNextCountingFlush = DYNAMIC_FIELD_COUNTING_INTERVAL;
	m_bEventActive = false;
	m_dwEventStartTick = 0;
	m_dwNextStatusUpdate = 0;
}


void CDynamicFieldSystemEvent::Destroy()
{
}


void CDynamicFieldSystemEvent::Create()
{
	LoadSettings();

	// request the current count from the query server
	SendCountingReq(0);
}


void CDynamicFieldSystemEvent::LoadSettings()
{
	for (DYNAMIC_FIELD_SYSTEM_MAP::iterator it = g_pTableContainer->GetDynamicFieldSystemTable()->DynamicFieldBegin(); it != g_pTableContainer->GetDynamicFieldSystemTable()->DynamicFieldEnd(); ++it)
	{
		const sDYNAMIC_FIELD_SYSTEM_TBLDAT* pField = it->second;

		if (pField->byType == EVENT_SYSTEM_TYPE_COUNTING && pField->byAction == EVENT_SYSTEM_ACTION_WPS)
		{
			if (pField->adwSetting[0] > 0 && pField->adwSetting[0] != INVALID_DWORD)
				m_dwMaxCount = pField->adwSetting[0];

			if (pField->adwSetting[1] > 0 && pField->adwSetting[1] != INVALID_DWORD)
				m_dwEventTimeSecs = pField->adwSetting[1];

			if (pField->aIndex != INVALID_TBLIDX)
				m_wpsTblidx = pField->aIndex;

			break;
		}
	}

	ERR_LOG(LOG_GENERAL, "DynamicFieldSystem: required kills %u, event time %u sec, wps %u", m_dwMaxCount, m_dwEventTimeSecs, m_wpsTblidx);
}


bool CDynamicFieldSystemEvent::AddCount(DWORD dwCount)
{
	if (m_bEventActive || dwCount == 0)
		return false;

	// nAddCount is an int in the packet
	if (dwCount > INT_MAX - (DWORD)m_nAddCount)
		dwCount = INT_MAX - (DWORD)m_nAddCount;

	m_nAddCount += (int)dwCount;
	FlushCounting();

	return true;
}


bool CDynamicFieldSystemEvent::ForceStartEvent()
{
	if (m_bEventActive)
		return false;

	StartEvent(GetTickCount());

	return m_bEventActive;
}


bool CDynamicFieldSystemEvent::ForceEndEvent()
{
	if (m_bEventActive == false)
		return false;

	EndEvent();

	return true;
}


void CDynamicFieldSystemEvent::FlushCounting()
{
	if (m_nAddCount <= 0)
		return;

	SendCountingReq(m_nAddCount);
	m_nAddCount = 0;
}


void CDynamicFieldSystemEvent::TickProcess(DWORD dwTickDiff)
{
	DWORD dwCurTick = GetTickCount();

	if (m_bEventActive == false)
	{
		m_dwNextCountingFlush = UnsignedSafeDecrease<DWORD>(m_dwNextCountingFlush, dwTickDiff);
		if (m_dwNextCountingFlush == 0)
		{
			FlushCounting();
			m_dwNextCountingFlush = DYNAMIC_FIELD_COUNTING_INTERVAL;
		}

		// counter reached. Start the event from the main thread (WPS objects must only be created/stopped here)
		if (m_dwCurCount >= m_dwMaxCount)
			StartEvent(dwCurTick);

		return;
	}

	// unsigned subtraction keeps working when GetTickCount wraps around
	if (dwCurTick - m_dwEventStartTick >= m_dwEventTimeSecs * 1000)
	{
		EndEvent();
		return;
	}

	m_dwNextStatusUpdate = UnsignedSafeDecrease<DWORD>(m_dwNextStatusUpdate, dwTickDiff);
	if (m_dwNextStatusUpdate == 0)
	{
		m_byBossCount = CountAliveBosses();
		BroadcastCountNfy(dwCurTick);

		m_dwNextStatusUpdate = DYNAMIC_FIELD_STATUS_INTERVAL;
	}
}


void CDynamicFieldSystemEvent::StartEvent(DWORD dwCurTick)
{
	ERR_LOG(LOG_GENERAL, "Start DynamicFieldSystem Event");

	CGameServer* app = (CGameServer*)g_pApp;
	CWorld* pMainWorld = app->GetGameMain()->GetWorldManager()->GetDefaultWorld();
	if (pMainWorld == NULL)
	{
		ERR_LOG(LOG_GENERAL, "DynamicFieldSystem: default world not found. Event not started");
		return;
	}

	bool bStart = false;
	CWpsAlgoObject* pWps = g_pScriptAlgoManager->CreateWpsObject(m_wpsTblidx, bStart);
	if (pWps == NULL)
	{
		ERR_LOG(LOG_GENERAL, "DynamicFieldSystem: could not create wps %u. Event not started", m_wpsTblidx);
		return;
	}

	pWps->SetWorld(pMainWorld);
	pMainWorld->AddScript(pWps);
	pWps->Start();

	m_bEventActive = true;
	m_dwEventStartTick = dwCurTick;
	m_dwNextStatusUpdate = DYNAMIC_FIELD_STATUS_INTERVAL;
	m_nAddCount = 0;
	m_dwCurCount = m_dwMaxCount;
	m_byBossCount = DYNAMIC_FIELD_BOSS_COUNT; // mobs are spawned by the wps on its next update

	BroadcastCountNfy(dwCurTick);
	BroadcastSystemText(L"The World Bosses have appeared! Open the world map to find their locations.");
}


void CDynamicFieldSystemEvent::EndEvent()
{
	ERR_LOG(LOG_GENERAL, "Reset DynamicFieldSystem Event");

	// remove remaining bosses and reward npcs
	CWpsAlgoObject* pWps = GetEventWps();
	if (pWps && pWps->IsStopped() == false)
		pWps->Stop();

	m_bEventActive = false;
	m_dwEventStartTick = 0;
	m_dwNextStatusUpdate = 0;
	m_nAddCount = 0;
	m_dwCurCount = 0;
	m_byBossCount = 0;

	BroadcastCountNfy(GetTickCount());
	BroadcastSystemText(L"The World Boss event has ended.");

	// re-sync with the stored progress (it may not be 0 if the event was started by a GM)
	SendCountingReq(0);
}


CWpsAlgoObject* CDynamicFieldSystemEvent::GetEventWps() const
{
	return g_pScriptAlgoManager->GetWpsObject(m_wpsTblidx);
}


BYTE CDynamicFieldSystemEvent::CountAliveBosses() const
{
	CWpsAlgoObject* pWps = GetEventWps();
	if (pWps == NULL || pWps->IsStopped())
		return 0;

	BYTE byCount = 0;

	for (CWpsAlgoObject::SPAWNGROUP_IT it = pWps->GetMobSpawnBegin(); it != pWps->GetMobSpawnEnd() && byCount < NTL_MAX_DYNAMIC_BOSS; ++it)
	{
		CMonster* pMob = g_pObjectManager->GetMob(it->second);
		if (pMob && pMob->IsInitialized() && pMob->IsFainting() == false)
			++byCount;
	}

	return byCount;
}


DBOTIME CDynamicFieldSystemEvent::GetRemainingEventTime(DWORD dwCurTick) const
{
	if (m_bEventActive == false)
		return 0;

	DWORD dwElapsedSecs = (dwCurTick - m_dwEventStartTick) / 1000;

	return (DBOTIME)UnsignedSafeDecrease<DWORD>(m_dwEventTimeSecs, dwElapsedSecs);
}


void CDynamicFieldSystemEvent::BuildCountNfy(CNtlPacket& rPacket, DWORD dwCurTick) const
{
	sGU_DYNAMIC_FIELD_COUNT_NFY* res = (sGU_DYNAMIC_FIELD_COUNT_NFY*)rPacket.GetPacketData();
	res->wOpCode = GU_DYNAMIC_FIELD_COUNT_NFY;
	res->byBossCount = m_bEventActive ? m_byBossCount : 0;
	res->dwMaxCount = m_dwMaxCount;
	res->dwCurCount = m_bEventActive ? m_dwMaxCount : m_dwCurCount;
	res->nEventTime = GetRemainingEventTime(dwCurTick); // remaining seconds. 0 = no event in progress
	rPacket.SetPacketLen(sizeof(sGU_DYNAMIC_FIELD_COUNT_NFY));
}


void CDynamicFieldSystemEvent::BroadcastCountNfy(DWORD dwCurTick) const
{
	CNtlPacket packet(sizeof(sGU_DYNAMIC_FIELD_COUNT_NFY));
	BuildCountNfy(packet, dwCurTick);
	g_pObjectManager->SendPacketToAll(&packet);
}


void CDynamicFieldSystemEvent::BroadcastSystemText(const WCHAR* pwszMessage) const
{
	CNtlPacket packet(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	sGU_SYSTEM_DISPLAY_TEXT* res = (sGU_SYSTEM_DISPLAY_TEXT*)packet.GetPacketData();
	res->wOpCode = GU_SYSTEM_DISPLAY_TEXT;
	res->byDisplayType = SERVER_TEXT_SYSNOTICE;
	NTL_SAFE_WCSCPY(res->awchMessage, pwszMessage);
	res->wMessageLengthInUnicode = (WORD)wcslen(res->awchMessage);
	packet.SetPacketLen(sizeof(sGU_SYSTEM_DISPLAY_TEXT));
	g_pObjectManager->SendPacketToAll(&packet);
}


void CDynamicFieldSystemEvent::SendCountingReq(int nAddCount) const
{
	CGameServer* app = (CGameServer*)g_pApp;

	CNtlPacket packet(sizeof(sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ));
	sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ* res = (sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ*)packet.GetPacketData();
	res->wOpCode = GQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ;
	res->byChannelId = app->GetGsChannel();
	res->ServerFarmId = app->GetGsServerId();
	res->nAddCount = nAddCount;
	res->dwSettingCount = m_dwMaxCount;
	res->tblidx = m_wpsTblidx;
	packet.SetPacketLen(sizeof(sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ));
	app->SendTo(app->GetQueryServerSession(), &packet);
}


void CDynamicFieldSystemEvent::LoadDynamicField(CCharacter * pPlayer)
{
	CNtlPacket packet(sizeof(sGU_DYNAMIC_FIELD_COUNT_NFY));
	BuildCountNfy(packet, GetTickCount());
	pPlayer->SendPacket(&packet);
}

void CDynamicFieldSystemEvent::UpdateDynamicFieldCount(DWORD dwCount)
{
	// while the event runs the counter is frozen. Counter from query server is already reset to 0.
	if (m_bEventActive)
		return;

	m_dwCurCount = dwCount;

	if (m_dwCurCount >= m_dwMaxCount)
		m_nAddCount = 0; // event starts on next TickProcess

	BroadcastCountNfy(GetTickCount());
}


void CDynamicFieldSystemEvent::Update(CMonster* pMob, CCharacter* pPlayer)
{
	if (pMob->GetLevel() < DYNAMIC_FIELD_MIN_MOB_LEVEL) //dont do the following if monster below lv 40
		return;

	if (pPlayer->GetCurWorld() == NULL)
		return;
	if (pPlayer->GetCurWorld()->GetRuleType() != GAMERULE_NORMAL)
		return;

	DYNAMIC_FIELD_SYSTEM_MAP::iterator it = g_pTableContainer->GetDynamicFieldSystemTable()->DynamicFieldBegin();

	for (; it != g_pTableContainer->GetDynamicFieldSystemTable()->DynamicFieldEnd(); it++)
	{
		sDYNAMIC_FIELD_SYSTEM_TBLDAT* pField = it->second;

		switch (pField->byAction)
		{
			case EVENT_SYSTEM_ACTION_ITEM: //drop item
			{
				if (pField->tIndex == pMob->GetTblidx())
				{
					CItemDrop* pDrop = g_pItemManager->CreateSingleDrop(pField->fRate, pField->aIndex);
					if (pDrop)
					{
						sVECTOR3 pos;
						pos.x = pMob->GetCurLoc().x + RandomRangeF(-2.0f, 2.0f);
						pos.y = pMob->GetCurLoc().y;
						pos.z = pMob->GetCurLoc().z + RandomRangeF(-2.0f, 2.0f);

						pDrop->SetOwnership(pPlayer->GetID(), pPlayer->GetPartyID());
						pDrop->StartDestroyEvent();
						pDrop->AddToGround(pMob->GetWorldID(), pos);
					}
				}
			}
			break;
			case EVENT_SYSTEM_ACTION_MOB: //summon mob
			{
				if (m_bEventActive) //dont summon sub monsters while the bosses are out
					break;

				if (pMob->GetLevel() >= pField->adwSetting[0] && pMob->GetLevel() <= pField->adwSetting[1])
				{
					if (Dbo_CheckProbabilityF(pField->fRate)) //check percent
					{
						sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(pField->aIndex);
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
						else
						{
							ERR_LOG(LOG_SCRIPT, "Could not find MOB-TBLDAT. Tblidx %u", pField->aIndex);
						}
					}
				}
			}
			break;
			case EVENT_SYSTEM_ACTION_BUFF: //give buff
			{
				if (pField->bOnOff && pField->tIndex == pMob->GetTblidx())
				{
					if (Dbo_CheckProbabilityF(pField->fRate))
					{
						sUSE_ITEM_TBLDAT* pUseItemTbldat = (sUSE_ITEM_TBLDAT*)g_pTableContainer->GetUseItemTable()->FindData(pField->aIndex);
						if (pUseItemTbldat)
						{
							sBUFF_INFO buffInfo;
							buffInfo.buffIndex = INVALID_BYTE;
							buffInfo.sourceTblidx = pField->aIndex;
							buffInfo.dwTimeRemaining = pUseItemTbldat->dwKeepTimeInMilliSecs;
							buffInfo.dwInitialDuration = pUseItemTbldat->dwKeepTimeInMilliSecs;
							buffInfo.bySourceType = DBO_OBJECT_SOURCE_ITEM;

							eSYSTEM_EFFECT_CODE effectCode[NTL_MAX_EFFECT_IN_ITEM];
							effectCode[0] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pUseItemTbldat->aSystem_Effect[0]);
							effectCode[1] = g_pTableContainer->GetSystemEffectTable()->GetEffectCodeWithTblidx(pUseItemTbldat->aSystem_Effect[1]);

							for (int x = 0; x < NTL_MAX_EFFECT_IN_ITEM; x++)
							{
								switch (effectCode[x])
								{
									case ACTIVE_HEAL_OVER_TIME:
									case ACTIVE_EP_OVER_TIME:
									{
										buffInfo.aBuffParameter[x].byBuffParameterType = DBO_BUFF_PARAMETER_TYPE_HOT;
										buffInfo.aBuffParameter[x].buffParameter.dwRemainTime = pUseItemTbldat->dwKeepTimeInMilliSecs;
										buffInfo.aBuffParameter[x].buffParameter.fParameter = (float)pUseItemTbldat->aSystem_Effect_Value[x];
									}
									break;
									default: break;
								}
							}

							WORD wTemp;
							pPlayer->GetBuffManager()->RegisterSubBuff(&buffInfo, effectCode, pPlayer->GetID(), pUseItemTbldat->byBuff_Group, wTemp, pUseItemTbldat->abySystem_Effect_Type);
						}
					}
				}
			}
			break;
			case EVENT_SYSTEM_ACTION_INVEN_INSERT: //add item to inventory
			{
			}
			break;
			case EVENT_SYSTEM_ACTION_COUNTING://mob kill counter. Increase counter at event aIndex
			{
				if (m_bEventActive) //counter is frozen while the bosses are out
					break;

				if (pMob->GetTblidx() == pField->tIndex && m_dwCurCount < m_dwMaxCount)
				{
					++m_nAddCount;

					if (m_nAddCount >= DYNAMIC_FIELD_COUNTING_FLUSH)
						FlushCounting();
				}
			}
			break;
			case EVENT_SYSTEM_ACTION_WPS:
			{
			}
			break;
		}
	}

}

void CDynamicFieldSystemEvent::GetBosses(CCharacter * pPlayer)
{
	if (m_bEventActive == false) //check if event in progress
		return;

	CNtlPacket packet(sizeof(sGU_DYNAMIC_FIELD_SYSTEM_BOSS_POSITION_RES));
	sGU_DYNAMIC_FIELD_SYSTEM_BOSS_POSITION_RES * res = (sGU_DYNAMIC_FIELD_SYSTEM_BOSS_POSITION_RES *)packet.GetPacketData();
	res->wOpCode = GU_DYNAMIC_FIELD_SYSTEM_BOSS_POSITION_RES;
	res->byBossCount = 0;

	CWpsAlgoObject* pWps = GetEventWps();
	if (pWps && !pWps->IsPaused())
	{
		for (CWpsAlgoObject::SPAWNGROUP_IT it = pWps->GetMobSpawnBegin(); it != pWps->GetMobSpawnEnd() && res->byBossCount < NTL_MAX_DYNAMIC_BOSS; ++it)
		{
			CMonster* pMob = g_pObjectManager->GetMob(it->second);
			if (pMob && pMob->IsInitialized() && pMob->IsFainting() == false)
			{
				sDYNAMIC_FIELD_BOSS_INFO& rInfo = res->aDynamicFieldBossInfo[res->byBossCount++];
				rInfo.handle = pMob->GetID();
				pMob->GetCurLoc().CopyTo(rInfo.vBossPos);
			}
		}
	}

	m_byBossCount = res->byBossCount;

	packet.SetPacketLen(sizeof(sGU_DYNAMIC_FIELD_SYSTEM_BOSS_POSITION_RES));
	pPlayer->SendPacket(&packet);
}
