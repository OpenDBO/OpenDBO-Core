#include "stdafx.h"
#include "QueryServer.h"
#include "PlayerCache.h"
#include "NtlPacketQG.h"
#include "NtlPacketGQ.h"
#include "NtlResultCode.h"
#include "ItemManager.h"
#include "CashshopManager.h"
#include "Utils.h"
#include "Guild.h"
#include "DynamicFieldSystem.h"
#include "BudokaiManager.h"
#include "Repository/CharacterRepository.h"
#include "Repository/ItemRepository.h"
#include "Repository/SkillRepository.h"
#include "Repository/BuffRepository.h"
#include "Repository/MailRepository.h"
#include "Repository/QuestRepository.h"
#include "Repository/QuickSlotRepository.h"
#include "Repository/MascotRepository.h"
#include "Repository/RecipeRepository.h"
#include "Repository/CashShopRepository.h"
#include "Repository/AccountRepository.h"
#include "Repository/GuildRepository.h"
#include "Repository/DynamicFieldRepository.h"
#include "Repository/BudokaiRepository.h"
#include "Repository/AuditLogRepository.h"
#include "Repository/FriendRepository.h"
#include "Repository/AuctionHouseRepository.h"


void CGameServerSession::OnAccountCheck(QueryResultVector & results, CHARACTERID charId, ACCOUNTID accountId)
{
	CAccountCache* pAccountCache = g_pPlayerCache->GetAccount(accountId);

	QueryResult* result = results[0].result;
	if (result)
	{
		Field* f = result->Fetch();

		std::string accstatus = f[0].GetString();

		if (accstatus != "block")
		{
			g_pCharacterRepository->CheckAccountForCharacterAsync(this, charId, accountId);

			return;
		}
	}

	CNtlPacket packet(sizeof(sQG_PC_DATA_LOAD_RES));
	sQG_PC_DATA_LOAD_RES * res = (sQG_PC_DATA_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_DATA_LOAD_RES;
	res->wResultCode = GAME_FAIL;
	res->accountId = accountId;
	res->sPcData.charId = charId;
	packet.SetPacketLen(sizeof(sQG_PC_DATA_LOAD_RES));
	g_pApp->Send(pAccountCache->GetSession(), &packet);
}

void CGameServerSession::OnLoadPcCheck(QueryResultVector & results, CHARACTERID charId, ACCOUNTID accountId)
{
	CAccountCache* pAccountCache = g_pPlayerCache->GetAccount(accountId);

	QueryResult* result = results[0].result;
	if (result)
	{
		Field* f = result->Fetch();
		if (f[0].GetUInt32() == accountId)
		{
			CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(charId);
			if (pPlayerCache)
			{
				//	NTL_PRINT(PRINT_APP, "RecvPcDataLoadReq: load %u from cache..", req->charId);
				pPlayerCache->SetSession(GetHandle());

				pPlayerCache->SendPcDataLoadRes();
				pPlayerCache->SendMascotLoadRes();
				pPlayerCache->SendSkillLoadRes();
				pPlayerCache->SendBuffLoadRes();
				pPlayerCache->SendHtbSkillLoadRes();
				pPlayerCache->SendItemLoadRes();
				pPlayerCache->SendQuestItemLoadRes();
				pPlayerCache->SendQuestCompleteLoadRes();
				pPlayerCache->SendQuestProgressLoadRes();
				pPlayerCache->SendQuickSlotLoadRes();
				pPlayerCache->SendItemRecipeLoadRes();
				pPlayerCache->SendPortalLoadRes();
				pPlayerCache->SendItemCoolTimeLoadRes(); //must be last

				if (pPlayerCache->IsChannelChanged() == false)
					pAccountCache->SendShortcutLoadRes(charId);
			}
			else
			{
				//NTL_PRINT(PRINT_APP, "RecvPcDataLoadReq: load %u from db..", charId);
				CPlayerCache* pPlayerCache = new CPlayerCache(accountId, pAccountCache);
				pPlayerCache->SetSession(GetHandle());

				g_pPlayerCache->InsertCharacter(charId, pPlayerCache);

				g_pCharacterRepository->LoadCharacterCoreDataAsync(pPlayerCache, charId);
				g_pCharacterRepository->LoadCharacterExtendedDataAsync(pPlayerCache, charId);
			}

			return;
		}
	}

	CNtlPacket packet(sizeof(sQG_PC_DATA_LOAD_RES));
	sQG_PC_DATA_LOAD_RES * res = (sQG_PC_DATA_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_PC_DATA_LOAD_RES;
	res->wResultCode = GAME_FAIL;
	res->accountId = accountId;
	res->sPcData.charId = charId;
	packet.SetPacketLen(sizeof(sQG_PC_DATA_LOAD_RES));
	g_pApp->Send(pAccountCache->GetSession(), &packet);
}



void CGameServerSession::RecvCreateItemReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_ITEM_CREATE_REQ * req = (sGQ_ITEM_CREATE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_CREATE_RES));
	sQG_ITEM_CREATE_RES * res = (sQG_ITEM_CREATE_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_CREATE_RES;
	res->wResultCode = GAME_SUCCESS;
	res->charID = req->charId;
	res->handle = req->handle;
	memcpy(&res->sItem, &req->sItem, sizeof(sITEM_DATA));

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		res->sItem.itemId = g_pItemManager->CreateItem(req->sItem);

		pCache->AddItem(res->sItem);
	}
	else res->wResultCode = QUERY_FAIL;

	
	packet.SetPacketLen(sizeof(sQG_ITEM_CREATE_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvItemMoveReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_ITEM_MOVE_REQ * req = (sGQ_ITEM_MOVE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_MOVE_RES));
	sQG_ITEM_MOVE_RES * res = (sQG_ITEM_MOVE_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_MOVE_RES;
	res->handle = req->handle;
	res->wResultCode = GAME_SUCCESS;
	res->charID = req->charId;
	res->hSrcItem = req->hSrcItem;
	res->hDstItem = req->hDstItem;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->byDstPlace = req->byDstPlace;
	res->byDstPos = req->byDstPos;
	res->bRestrictUpdate = req->bRestrictUpdate;
	res->bySrcRestrictState = req->bySrcRestrictState;
	res->byDestRestrictState = req->byDestRestrictState;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		sITEM_DATA* pSrcItem = pCache->GetItemData(req->srcItemId);
		if (pSrcItem)
		{
			if (req->dstItemId != INVALID_ITEMID) //if dest item exist then move dest item to source item place
			{
				sITEM_DATA* pDstItem = pCache->GetItemData(req->dstItemId);
				if (pDstItem)
				{
					pDstItem->byPlace = req->bySrcPlace;
					pDstItem->byPosition = req->bySrcPos;

					g_pItemRepository->UpdatePlace(req->dstItemId, req->bySrcPlace, req->bySrcPos);
				}
				else
				{
					res->wResultCode = QUERY_FAIL;
					goto END;
				}
			}

			if (req->bRestrictUpdate)
			{
				g_pItemRepository->UpdatePlaceWithRestrict(req->srcItemId, req->byDstPlace, req->byDstPos, req->bySrcRestrictState);

				pSrcItem->byRestrictState = req->bySrcRestrictState;
			}
			else
			{
				g_pItemRepository->UpdatePlace(req->srcItemId, req->byDstPlace, req->byDstPos);
			}

			pSrcItem->byPlace = req->byDstPlace;
			pSrcItem->byPosition = req->byDstPos;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	
END:

	packet.SetPacketLen(sizeof(sQG_ITEM_MOVE_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvItemMoveStackReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_ITEM_MOVE_STACK_REQ * req = (sGQ_ITEM_MOVE_STACK_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_MOVE_STACK_RES));
	sQG_ITEM_MOVE_STACK_RES * res = (sQG_ITEM_MOVE_STACK_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_MOVE_STACK_RES;
	res->handle = req->handle;
	res->wResultCode = GAME_SUCCESS;
	res->byDstPlace = req->byDstPlace;
	res->byDstPos = req->byDstPos;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->byStackCount1 = req->byStackCount1;
	res->byStackCount2 = req->byStackCount2;
	res->charID = req->charId;
	res->hDstItem = req->hDstItem;
	res->hSrcItem = req->hSrcItem;
	res->splitItemId = INVALID_ITEMID;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		sITEM_DATA* pSrcItem = pCache->GetItemData(req->srcItemId);
		if (pSrcItem)
		{
			if (req->hDstItem == INVALID_HOBJECT)	//unstack item -> create new item, update stackcount from src item
			{
				g_pItemRepository->UpdateCount(req->srcItemId, req->byStackCount1);//update stack count from source item
				pSrcItem->byStackcount = req->byStackCount1;

				res->splitItemId = g_pItemManager->IncLastItemID();

				sITEM_DATA* pDstItem = new sITEM_DATA;
				memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
				pDstItem->itemId = res->splitItemId;
				pDstItem->byPlace = req->byDstPlace;
				pDstItem->byPosition = req->byDstPos;
				pDstItem->byStackcount = req->byStackCount2;

				pCache->InsertItem(pDstItem);

				g_pItemRepository->InsertSplitFromSource(res->splitItemId, req->byDstPlace, req->byDstPos, req->byStackCount2, req->srcItemId);//update stack count from source item
			}
			else //if source stack count is zero then delete else update the count. Updte count of dest item
			{
				sITEM_DATA* pDstItem = pCache->GetItemData(req->dstItemId);
				if (pDstItem)
				{
					if (req->byStackCount1 == 0)
					{
						g_pItemRepository->DeleteById(req->srcItemId); //delete source item
						pCache->RemoveItem(req->srcItemId);
					}
					else
					{
						g_pItemRepository->UpdateCount(req->srcItemId, req->byStackCount1);//update stack count from source item
						pSrcItem->byStackcount = req->byStackCount1;
					}

					g_pItemRepository->UpdateCount(req->dstItemId, req->byStackCount2);	//update stack count from dest item
					pDstItem->byStackcount = req->byStackCount2;
				}
				else res->wResultCode = QUERY_FAIL;
			}
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_ITEM_MOVE_STACK_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvItemUpdateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_ITEM_UPDATE_REQ * req = (sGQ_ITEM_UPDATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pItem = pCache->GetItemData(req->sItem.itemId))
		{
			pItem->byGrade = req->sItem.byGrade;
			pItem->byRank = req->sItem.byRank;

			g_pItemRepository->UpdateRankGrade(req->sItem.itemId, req->sItem.byRank, req->sItem.byGrade);
		}
	}
}


void CGameServerSession::RecvDeleteItemReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);
	sGQ_ITEM_DELETE_REQ * req = (sGQ_ITEM_DELETE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		pCache->RemoveItem(req->itemId);

		g_pItemRepository->DeleteById(req->itemId);
	}
}


void CGameServerSession::RecvItemEquipRepairReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_ITEM_EQUIP_REPAIR_REQ * req = (sGQ_ITEM_EQUIP_REPAIR_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->GetZeni() >= req->dwZenny)
		{
			pCache->SetZeni(pCache->GetZeni() - req->dwZenny);
			g_pCharacterRepository->UpdateMoney(req->charId, pCache->GetZeni());

			for (BYTE byPos = 0; byPos < EQUIP_SLOT_TYPE_COUNT; byPos++)
			{
				if (req->asItemData[byPos].itemID != 0)
				{
					pCache->UpdateItemDur(req->asItemData[byPos].itemID, req->asItemData[byPos].byDur);
					g_pItemRepository->UpdateDurability(req->asItemData[byPos].itemID, req->asItemData[byPos].byDur);
				}
			}
		}
	}
}


void CGameServerSession::RecvItemRepairReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);
	sGQ_ITEM_REPAIR_REQ * req = (sGQ_ITEM_REPAIR_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->GetZeni() >= req->dwZenny)
		{
			pCache->SetZeni(pCache->GetZeni() - req->dwZenny);
			g_pCharacterRepository->UpdateMoney(req->charId, pCache->GetZeni());

			pCache->UpdateItemDur(req->sItemData.itemID, req->sItemData.byDur);
			g_pItemRepository->UpdateDurability(req->sItemData.itemID, req->sItemData.byDur);
		}
	}
}


void CGameServerSession::RecvItemDurDown(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_ITEM_DUR_DOWN * req = (sGQ_ITEM_DUR_DOWN*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		for (BYTE byPos = 0; byPos <= EQUIP_SLOT_TYPE_SCOUTER; byPos++)
		{
			if (req->byDur[byPos] != INVALID_BYTE)
			{
				if(pCache->UpdateEquipItemDur(byPos, req->byDur[byPos]))
					g_pItemRepository->UpdateDurabilityByOwnerPlacePos(req->byDur[byPos], req->charId, CONTAINER_TYPE_EQUIP, byPos);
			}
		}
	}
}


void CGameServerSession::RecvItemIdentifyReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_ITEM_IDENTIFY_REQ * req = (sGQ_ITEM_IDENTIFY_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->GetZeni() >= req->dwZeni)
		{
			pCache->SetZeni(pCache->GetZeni() - req->dwZeni);
			pCache->UpdateIdentifyItem(req->itemId);

			g_pCharacterRepository->UpdateMoney(req->charId, pCache->GetZeni());
			g_pItemRepository->ClearNeedToIdentify(req->itemId);
		}
	}
}

void CGameServerSession::RecvItemAutoEquipReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_ITEM_AUTO_EQUIP_REQ * req = (sGQ_ITEM_AUTO_EQUIP_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_AUTO_EQUIP_RES));
	sQG_ITEM_AUTO_EQUIP_RES * res = (sQG_ITEM_AUTO_EQUIP_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_AUTO_EQUIP_RES;
	res->wResultCode = GAME_SUCCESS;
	res->charId = req->charId;
	res->handle = req->handle;
	memcpy(&res->sItem, &req->sItem, sizeof(sITEM_AUTO_EQUIP_DATA));

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		res->sItem.sEquipItem.itemId = g_pItemManager->CreateItem(req->sItem.sEquipItem);

		pCache->AddItem(res->sItem.sEquipItem);
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_ITEM_AUTO_EQUIP_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvSkillAddReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SKILL_ADD_REQ * req = (sGQ_SKILL_ADD_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		pCache->AddSkillData(req->bySlot, req->skillId, DBO_RP_BONUS_TYPE_INVALID, false);

		if(pCache->GetSkillPoints() != req->dwSP)
			g_pCharacterRepository->UpdateSpPoint(req->dwSP, req->charId);

		pCache->SetSkillPoints(req->dwSP);

		g_pSkillRepository->InsertSkill(req->skillId, req->charId, req->bySlot);
	}
}


void CGameServerSession::RecvSkillUpdateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SKILL_UPDATE_REQ * req = (sGQ_SKILL_UPDATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		pCache->UpdateSkillData(req->sSkill);
		g_pSkillRepository->UpdateSkill(req->sSkill.skillId, req->sSkill.bIsRpBonusAuto, req->sSkill.byRpBonusType, req->sSkill.nRemainSec, req->charId, req->sSkill.skillIndex);
	}
}


void CGameServerSession::RecvSkillUpgradeReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SKILL_UPGRADE_REQ * req = (sGQ_SKILL_UPGRADE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		pCache->UpdateSkillData(req->bySlot, req->newSkillId);
		pCache->SetSkillPoints(req->dwSP);

		g_pSkillRepository->UpdateSkillId(req->newSkillId, req->charId, req->bySlot);
		g_pCharacterRepository->UpdateSpPoint(req->dwSP, req->charId);
	}
}

void CGameServerSession::RecvBuffAddReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUFF_ADD_REQ * req = (sGQ_BUFF_ADD_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->AddBuffData(req->sBuff))
		{
			g_pBuffRepository->InsertBuff(req->charId, req->sBuff.sourceTblidx, req->sBuff.bySourceType, req->sBuff.buffIndex, req->sBuff.byBuffGroup, req->sBuff.dwInitialDuration, req->sBuff.dwTimeRemaining,
				req->sBuff.effectValue1, req->sBuff.effectValue2,
				req->sBuff.anArgument1[0], req->sBuff.anArgument1[1], req->sBuff.anArgument1[2],
				req->sBuff.anArgument2[0], req->sBuff.anArgument2[1], req->sBuff.anArgument2[2]);
		}
	}
}

void CGameServerSession::RecvBuffDelReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUFF_DEL_REQ * req = (sGQ_BUFF_DEL_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->DelBuffData(req->buffIndex))
		{
			g_pBuffRepository->DeleteBuff(req->charId, req->buffIndex);
		}
	}
}


void CGameServerSession::RecvHtbSkillAddReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_HTB_SKILL_ADD_REQ * req = (sGQ_HTB_SKILL_ADD_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		pCache->AddHtbSkillData(req->skillId);
		g_pSkillRepository->InsertHtbSkill(req->skillId, req->charId, req->skillIndex);
	}
}


void CGameServerSession::RecvPcUpdateLevelReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_PC_UPDATE_LEVEL_REQ * req = (sGQ_PC_UPDATE_LEVEL_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		pCache->SetLevel(req->byLevel);
		pCache->SetExp(req->dwEXP);
		pCache->SetSkillPoints(req->dwSP);

		if (pCache->GetGuildID() > 0)
		{
			if (sDBO_GUILD_MEMBER_DATA* pMember = g_pGuild->GetGuildMemberData(pCache->GetGuildID(), req->charId))
				pMember->byLevel = req->byLevel;
		}

		g_pCharacterRepository->UpdateLevelExpSp(req->byLevel, req->dwEXP, req->dwSP, req->charId);
	}
}

void CGameServerSession::RecvSavePcDataReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SAVE_PC_DATA_REQ * req = (sGQ_SAVE_PC_DATA_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->sPcData.charId);
	if (pCache)
	{
		pCache->SavePcData(req->sPcData);
		pCache->SaveServerChangeInfo(req->serverChangeInfo);

		g_pCharacterRepository->SavePcData(req->sPcData.dwEXP, req->sPcData.fPositionX, req->sPcData.fPositionY, req->sPcData.fPositionZ, req->sPcData.fDirX, req->sPcData.fDirZ, req->sPcData.worldId, req->sPcData.worldTblidx, req->sPcData.dwMapInfoIndex,
			req->sPcData.dwTutorialHint, req->sPcData.charLp, req->sPcData.wEP, req->sPcData.wRP, req->sPcData.charAP,
			req->sPcData.charTitle, req->sPcData.mascotTblidx, req->sPcData.byCurRPBall, req->IP, req->sPcData.eAirState, req->dwAddPlayTime,
			req->sPcData.charId);
	}
}


void CGameServerSession::RecvSaveSkillDataReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SAVE_SKILL_DATA_REQ * req = (sGQ_SAVE_SKILL_DATA_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		for (int i = 0; i < req->bySkillCount; i++)
		{
			if (pCache->UpdateSkillTime(req->asSkill[i]))
			{
				g_pSkillRepository->UpdateSkillTimeRemaining(req->asSkill[i].nRemainSec, req->charId, req->asSkill[i].skillIndex);
			}
		}
	}
}

void CGameServerSession::RecvSaveHtbDataReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SAVE_HTB_DATA_REQ * req = (sGQ_SAVE_HTB_DATA_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		for (int i = 0; i < req->bySkillCount; i++)
		{
			if (pCache->UpdateHtbSkillTime(req->asHTBSkill[i]))
			{
				g_pSkillRepository->UpdateHtbSkillTimeRemaining(req->asHTBSkill[i].dwSkillTime, req->asHTBSkill[i].skillId, req->charId);
			}
		}
	}
}

void CGameServerSession::RecvSaveBuffDataReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SAVE_BUFF_DATA_REQ * req = (sGQ_SAVE_BUFF_DATA_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		for (int i = 0; i < req->byBuffCount; i++)
		{
			if (pCache->UpdateBuffTime(req->asBuff[i]))
			{
				g_pBuffRepository->UpdateBuffTime(req->asBuff[i].dwTimeRemaining,
					req->asBuff[i].anArgument1[1], req->asBuff[i].anArgument1[2],
					req->charId, req->asBuff[i].buffIndex);
			}
			else
			{
				g_pBuffRepository->DeleteBuff(req->charId, req->asBuff[i].buffIndex);
			}
		}
	}
}

void CGameServerSession::RecvShopBuyReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_SHOP_BUY_REQ * req = (sGQ_SHOP_BUY_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_SHOP_BUY_RES));
	sQG_SHOP_BUY_RES * res = (sQG_SHOP_BUY_RES *)packet.GetPacketData();
	res->wOpCode = QG_SHOP_BUY_RES;
	res->byBuyCount = req->byBuyCount;
	res->charId = req->charId;
	res->handle = req->handle;
	res->hNpchandle = req->hNpchandle;
	res->wResultCode = GAME_SUCCESS;
	memcpy(res->sInven, req->sInven, sizeof(req->sInven));

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->GetZeni() >= req->dwMoney)
		{
			for (BYTE i = 0; i < req->byBuyCount; i++)
			{
				res->itemID[i] = g_pItemManager->CreateItem(req->sInven[i], req->charId);

				pCache->AddItem(req->sInven[i], res->itemID[i]);
			}

			pCache->SetZeni(pCache->GetZeni() - req->dwMoney);
			g_pCharacterRepository->UpdateMoney(req->charId, pCache->GetZeni());
		}
		else res->wResultCode = GAME_ZENNY_NOT_ENOUGH;
	}
	else res->wResultCode = QUERY_FAIL;
	
	packet.SetPacketLen(sizeof(sQG_SHOP_BUY_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvShopSellReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SHOP_SELL_REQ * req = (sGQ_SHOP_SELL_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		bool bError = false;
		for (BYTE i = 0; i < req->bySellCount; i++)
		{
			bool bDelete = false;
			if (pCache->DecreaseItemCount(req->sInven[i].itemID, req->sInven[i].byStack, bDelete))
			{
				if(bDelete)
					g_pItemRepository->DeleteById(req->sInven[i].itemID);
				else
					g_pItemRepository->UpdateCount(req->sInven[i].itemID, req->sInven[i].byStack);
			}
			else
			{
				ERR_LOG(LOG_USER, "ERROR: User %u could not find item %I64u to sell.", req->charId, req->sInven[i].itemID);
				bError = true;
				break;
			}
		}

		if (bError == false)
		{
			pCache->SetZeni(UnsignedSafeIncrease<DWORD>(pCache->GetZeni(), req->dwMoney));
			g_pCharacterRepository->UpdateMoney(req->charId, pCache->GetZeni());
		}
	}
}


void CGameServerSession::RecvLoadBankDataReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_LOAD_PC_BANK_DATA_REQ * req = (sGQ_LOAD_PC_BANK_DATA_REQ*)pPacket->GetPacketData();

	CNtlPacket packet2(sizeof(sQG_LOAD_PC_BANK_DATA_RES));
	sQG_LOAD_PC_BANK_DATA_RES * res2 = (sQG_LOAD_PC_BANK_DATA_RES *)packet2.GetPacketData();
	res2->wOpCode = QG_LOAD_PC_BANK_DATA_RES;
	res2->charID = req->charId;
	res2->dwZenny = 0;
	res2->npchandle = req->npchandle;
	res2->wResultCode = GAME_SUCCESS;
	res2->handle = req->handle;


	if (CAccountCache* pAccount = g_pPlayerCache->GetAccount(req->accountId))
	{
		if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId))
		{
			pAccount->SetSession(GetHandle());
			pCache->SetSession(GetHandle());

			if (pCache->IsBankLoaded() == false && pAccount->IsBankLoaded() == false) //load normal & bank from query
			{
				g_pItemRepository->LoadBankDataAsync(pCache, pAccount, false, req->handle, req->npchandle, req->charId, req->accountId);

				return;
			}
			else if (pCache->IsBankLoaded() == false && pAccount->IsBankLoaded() == true) //load normal bank from db & acc from cache
			{
				g_pItemRepository->LoadBankDataAsync(pCache, pAccount, true, req->handle, req->npchandle, req->charId, req->accountId);

				return;
			}
			else
			{
				pCache->SendBankItemLoad();
				pAccount->SendBankItemLoad(req->charId);
			}
		}
		else res2->wResultCode = QUERY_FAIL;
	}
	else res2->wResultCode = QUERY_FAIL;
	
	packet2.SetPacketLen(sizeof(sQG_LOAD_PC_BANK_DATA_RES));
	app->Send(GetHandle(), &packet2);
}


void CGameServerSession::RecvBankMoveReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_BANK_MOVE_REQ * req = (sGQ_BANK_MOVE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_BANK_MOVE_RES));
	sQG_BANK_MOVE_RES * res = (sQG_BANK_MOVE_RES *)packet.GetPacketData();
	res->wOpCode = QG_BANK_MOVE_RES;
	res->handle = req->handle;
	res->hNpcHandle = req->hNpcHandle;
	res->wResultCode = GAME_SUCCESS;

	if (CAccountCache* pAccount = g_pPlayerCache->GetAccount(req->accountId))
	{
		if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId))
		{
			switch (req->bySrcPlace)
			{
				//move from bank to ???
				case CONTAINER_TYPE_BANK1:
				case CONTAINER_TYPE_BANK2:
				case CONTAINER_TYPE_BANK3:
				{
					if (sITEM_DATA* pSrcItem = pCache->GetBankItem(req->srcItemId, req->bySrcPlace, req->bySrcPos))
					{
						switch (req->byDstPlace)
						{
							//move from bank to bank
							case CONTAINER_TYPE_BANK1:
							case CONTAINER_TYPE_BANK2:
							case CONTAINER_TYPE_BANK3:
							{
								if (req->dstItemId != INVALID_ITEMID) //do we have a dest item which we do switch place/pos with src item?
								{
									if (sITEM_DATA* pDstItem = pCache->GetBankItem(req->dstItemId, req->byDstPlace, req->byDstPos))
									{
										//put dest item on source place/pos
										pDstItem->byPlace = req->bySrcPlace;
										pDstItem->byPosition = req->bySrcPos;
										g_pItemRepository->UpdatePlace(req->dstItemId, req->bySrcPlace, req->bySrcPos);
									}
									else res->wResultCode = QUERY_FAIL;
								}

								if (res->wResultCode == GAME_SUCCESS)
								{
									pSrcItem->byPlace = req->byDstPlace;
									pSrcItem->byPosition = req->byDstPos;
									g_pItemRepository->UpdatePlace(req->srcItemId, req->byDstPlace, req->byDstPos);
								}
							}
							break;

							//move from bank to shared bank
							case CONTAINER_TYPE_BANK4:
							{
								if (req->dstItemId != INVALID_ITEMID) //do we have a dest item which we do switch place/pos with src item?
								{
									if (sITEM_DATA* pDstItem = pAccount->GetBankItem(req->dstItemId, req->byDstPlace, req->byDstPos))
									{
										//put dest item on source place/pos
										pDstItem->byPlace = req->bySrcPlace;
										pDstItem->byPosition = req->bySrcPos;

										pAccount->EraseBankItem(req->dstItemId); //erase from shared bank
										pCache->InsertBankItem(pDstItem); //insert into bank

										g_pItemRepository->MoveToPersonalBank(req->dstItemId, req->charId, req->bySrcPlace, req->bySrcPos);
									}
									else res->wResultCode = QUERY_FAIL;
								}
								
								if (res->wResultCode == GAME_SUCCESS)
								{
									pSrcItem->byPlace = req->byDstPlace;
									pSrcItem->byPosition = req->byDstPos;

									pCache->EraseBankItem(req->srcItemId); //erase item from bank
									pAccount->InsertBankItem(pSrcItem); //insert item into shared bank

									g_pItemRepository->MoveToSharedBank(req->srcItemId, req->byDstPlace, req->byDstPos, req->accountId);
								}
							}
							break;

							//move from bank to inventory
							default:
							{
								if (req->dstItemId != INVALID_ITEMID) //do we have a dest item which we do switch place/pos with src item?
								{
									if (sITEM_DATA* pDstItem = pCache->GetItemData(req->dstItemId))
									{
										//put dest item on source place/pos
										pDstItem->byPlace = req->bySrcPlace;
										pDstItem->byPosition = req->bySrcPos;

										pCache->EraseItem(req->dstItemId); //erase from inventory
										pCache->InsertBankItem(pDstItem); //insert into bank

										g_pItemRepository->UpdatePlace(req->dstItemId, req->bySrcPlace, req->bySrcPos);
									}
									else res->wResultCode = QUERY_FAIL;
								}
								
								if (res->wResultCode == GAME_SUCCESS)
								{
									pSrcItem->byPlace = req->byDstPlace;
									pSrcItem->byPosition = req->byDstPos;

									pCache->EraseBankItem(req->srcItemId); //erase from bank
									pCache->InsertItem(pSrcItem); //insert into inventory
									
									g_pItemRepository->UpdatePlace(req->srcItemId, req->byDstPlace, req->byDstPos);
								}
							}
							break;
						}
					}
					else res->wResultCode = QUERY_FAIL;
				}
				break;

				//move from shared account bank to ???
				case CONTAINER_TYPE_BANK4:
				{
					if (sITEM_DATA* pSrcItem = pAccount->GetBankItem(req->srcItemId, req->bySrcPlace, req->bySrcPos))
					{
						switch (req->byDstPlace)
						{
							//move from shared account bank to bank
							case CONTAINER_TYPE_BANK1:
							case CONTAINER_TYPE_BANK2:
							case CONTAINER_TYPE_BANK3:
							{
								if (req->dstItemId != INVALID_ITEMID) //do we have a dest item which we do switch place/pos with src item?
								{
									if (sITEM_DATA* pDstItem = pCache->GetBankItem(req->dstItemId, req->byDstPlace, req->byDstPos))
									{
										//put dest item on source place/pos
										pDstItem->byPlace = req->bySrcPlace;
										pDstItem->byPosition = req->bySrcPos;

										pCache->EraseBankItem(req->dstItemId); //erase from bank
										pAccount->InsertBankItem(pDstItem); //insert into shared bank

										g_pItemRepository->MoveToSharedBank(req->dstItemId, req->bySrcPlace, req->bySrcPos, req->accountId);
									}
									else res->wResultCode = QUERY_FAIL;
								}

								if (res->wResultCode == GAME_SUCCESS)
								{
									pSrcItem->byPlace = req->byDstPlace;
									pSrcItem->byPosition = req->byDstPos;

									pAccount->EraseBankItem(req->srcItemId); //erase item from shared bank
									pCache->InsertBankItem(pSrcItem); //insert item into bank

									g_pItemRepository->MoveToPersonalBank(req->srcItemId, req->charId, req->byDstPlace, req->byDstPos);
								}
							}
							break;

							//move from shared account bank to shared bank
							case CONTAINER_TYPE_BANK4:
							{
								if (req->dstItemId != INVALID_ITEMID) //do we have a dest item which we do switch place/pos with src item?
								{
									if (sITEM_DATA* pDstItem = pAccount->GetBankItem(req->dstItemId, req->byDstPlace, req->byDstPos))
									{
										//put dest item on source place/pos
										pDstItem->byPlace = req->bySrcPlace;
										pDstItem->byPosition = req->bySrcPos;
										g_pItemRepository->UpdatePlace(req->dstItemId, req->bySrcPlace, req->bySrcPos);
									}
									else res->wResultCode = QUERY_FAIL;
								}

								if (res->wResultCode == GAME_SUCCESS)
								{
									pSrcItem->byPlace = req->byDstPlace;
									pSrcItem->byPosition = req->byDstPos;
									g_pItemRepository->UpdatePlace(req->srcItemId, req->byDstPlace, req->byDstPos);
								}
							}
							break;

							//move from shared account bank to inventory
							default:
							{
								if (req->dstItemId != INVALID_ITEMID) //do we have a dest item which we do switch place/pos with src item?
								{
									if (sITEM_DATA* pDstItem = pCache->GetItemData(req->dstItemId, req->byDstPlace, req->byDstPos))
									{
										//put dest item on source place/pos
										pDstItem->byPlace = req->bySrcPlace;
										pDstItem->byPosition = req->bySrcPos;

										pCache->EraseItem(req->dstItemId); //erase from inventory
										pAccount->InsertBankItem(pDstItem); //insert into shared bank

										g_pItemRepository->MoveToSharedBank(req->dstItemId, req->bySrcPlace, req->bySrcPos, req->accountId);
									}
									else res->wResultCode = QUERY_FAIL;
								}

								if (res->wResultCode == GAME_SUCCESS)
								{
									pSrcItem->byPlace = req->byDstPlace;
									pSrcItem->byPosition = req->byDstPos;

									pAccount->EraseBankItem(req->srcItemId); //erase item from shared bank
									pCache->InsertItem(pSrcItem); //insert item into inventory

									g_pItemRepository->MoveToPersonalBank(req->srcItemId, req->charId, req->byDstPlace, req->byDstPos);
								}
							}
							break;
						}
					}
					else res->wResultCode = QUERY_FAIL;
				}
				break;

				//move from inventory to ???
				default:
				{
					if (sITEM_DATA* pSrcItem = pCache->GetItemData(req->srcItemId, req->bySrcPlace, req->bySrcPos))
					{
						switch (req->byDstPlace)
						{
							//move from inventory to bank
							case CONTAINER_TYPE_BANK1:
							case CONTAINER_TYPE_BANK2:
							case CONTAINER_TYPE_BANK3:
							{
								if (req->dstItemId != INVALID_ITEMID) //do we have a dest item which we do switch place/pos with src item?
								{
									if (sITEM_DATA* pDstItem = pCache->GetBankItem(req->dstItemId, req->byDstPlace, req->byDstPos))
									{
										//put dest item on source place/pos
										pDstItem->byPlace = req->bySrcPlace;
										pDstItem->byPosition = req->bySrcPos;

										pCache->EraseBankItem(req->dstItemId); //erase from bank
										pCache->InsertItem(pDstItem); //insert into inventory

										g_pItemRepository->UpdatePlace(req->dstItemId, req->bySrcPlace, req->bySrcPos);
									}
									else res->wResultCode = QUERY_FAIL;
								}

								if (res->wResultCode == GAME_SUCCESS)
								{
									pSrcItem->byPlace = req->byDstPlace;
									pSrcItem->byPosition = req->byDstPos;

									pCache->EraseItem(req->srcItemId); //erase item from inventory
									pCache->InsertBankItem(pSrcItem); //insert item into bank

									g_pItemRepository->UpdatePlace(req->srcItemId, req->byDstPlace, req->byDstPos);
								}
							}
							break;

							//move from inventory to shared bank
							case CONTAINER_TYPE_BANK4:
							{
								if (req->dstItemId != INVALID_ITEMID) //do we have a dest item which we do switch place/pos with src item?
								{
									if (sITEM_DATA* pDstItem = pAccount->GetBankItem(req->dstItemId, req->byDstPlace, req->byDstPos))
									{
										//put dest item on source place/pos
										pDstItem->byPlace = req->bySrcPlace;
										pDstItem->byPosition = req->bySrcPos;

										pAccount->EraseBankItem(req->dstItemId); //erase from shared bank
										pCache->InsertItem(pDstItem); //insert into inventory

										g_pItemRepository->MoveToPersonalBank(req->dstItemId, req->charId, req->bySrcPlace, req->bySrcPos);
									}
									else res->wResultCode = QUERY_FAIL;
								}

								if (res->wResultCode == GAME_SUCCESS)
								{
									pSrcItem->byPlace = req->byDstPlace;
									pSrcItem->byPosition = req->byDstPos;

									pCache->EraseItem(req->srcItemId); //erase from inventory
									pAccount->InsertBankItem(pSrcItem); //insert into shared bank

									g_pItemRepository->MoveToSharedBank(req->srcItemId, req->byDstPlace, req->byDstPos, req->accountId);
								}
							}
							break;

							default:
							{
								res->wResultCode = QUERY_FAIL;	
							}
							break;
						}
					}
					else res->wResultCode = QUERY_FAIL;
				}
				break;
			}


		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	res->charID = req->charId;
	res->hSrcItem = req->hSrcItem;
	res->hDstItem = req->hDstItem;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->byDstPlace = req->byDstPlace;
	res->byDstPos = req->byDstPos;
	res->bRestrictUpdate = req->bRestrictUpdate;
	res->bySrcRestrictState = req->bySrcRestrictState;
	res->byDestRestrictState = req->byDestRestrictState;
	packet.SetPacketLen(sizeof(sQG_BANK_MOVE_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvBankMoveStackReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_BANK_MOVE_STACK_REQ * req = (sGQ_BANK_MOVE_STACK_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_BANK_MOVE_STACK_RES));
	sQG_BANK_MOVE_STACK_RES * res = (sQG_BANK_MOVE_STACK_RES *)packet.GetPacketData();
	res->wOpCode = QG_BANK_MOVE_STACK_RES;
	res->handle = req->handle;
	res->hNpcHandle = req->hNpcHandle;
	res->wResultCode = GAME_SUCCESS;
	res->byDstPlace = req->byDstPlace;
	res->byDstPos = req->byDstPos;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->byStackCount1 = req->byStackCount1;
	res->byStackCount2 = req->byStackCount2;
	res->charID = req->charId;
	res->hDstItem = req->hDstItem;
	res->hSrcItem = req->hSrcItem;
	res->splitItemId = INVALID_ITEMID;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (CAccountCache* pAccount = g_pPlayerCache->GetAccount(req->accountID))
		{
				switch (req->bySrcPlace)
				{
					//stack from bank to ???
					case CONTAINER_TYPE_BANK1:
					case CONTAINER_TYPE_BANK2:
					case CONTAINER_TYPE_BANK3:
					{
						if (sITEM_DATA* pSrcItem = pCache->GetBankItem(req->srcItemID, req->bySrcPlace, req->bySrcPos))
						{
							switch (req->byDstPlace)
							{
								//from bank to bank
								case CONTAINER_TYPE_BANK1:
								case CONTAINER_TYPE_BANK2:
								case CONTAINER_TYPE_BANK3:
								{
									if (req->dstItemID != INVALID_ITEMID) //if dest item exist, then update stack from dest item
									{
										if (sITEM_DATA* pDstItem = pCache->GetBankItem(req->dstItemID, req->byDstPlace, req->byDstPos))
										{
											//update dest stack count
											pDstItem->byStackcount = req->byStackCount2;
											g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);
										}
										else res->wResultCode = QUERY_FAIL;
									}
									else //unstack item -> create new item inside bank
									{
										sITEM_DATA* pDstItem = new sITEM_DATA;
										memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
										pDstItem->byStackcount = req->byStackCount2;
										pDstItem->byPlace = req->byDstPlace;
										pDstItem->byPosition = req->byDstPos;

										res->splitItemId = g_pItemManager->CreateItem(*pDstItem);
										pDstItem->itemId = res->splitItemId;
										pCache->InsertBankItem(pDstItem);
									}
								}
								break;

								//stack from bank to shared bank
								case CONTAINER_TYPE_BANK4:
								{
									if (req->dstItemID != INVALID_ITEMID) //if dest item exist, then update stack from dest item
									{
										if (sITEM_DATA* pDstItem = pAccount->GetBankItem(req->dstItemID, req->byDstPlace, req->byDstPos))
										{
											//update dest stack count
											pDstItem->byStackcount = req->byStackCount2;
											g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);
										}
										else res->wResultCode = QUERY_FAIL;
									}
									else //unstack item -> create new item inside shared bank
									{
										sITEM_DATA* pDstItem = new sITEM_DATA;
										memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
										pDstItem->byStackcount = req->byStackCount2;
										pDstItem->byPlace = req->byDstPlace;
										pDstItem->byPosition = req->byDstPos;

										res->splitItemId = g_pItemManager->CreateSharedBankItem(*pDstItem, req->accountID);
										pDstItem->itemId = res->splitItemId;
										pAccount->InsertBankItem(pDstItem);
									}
								}
								break;

								//stack from bank to inventory
								default:
								{
									if (req->dstItemID != INVALID_ITEMID) 
									{
										if (sITEM_DATA* pDstItem = pCache->GetItemData(req->dstItemID, req->byDstPlace, req->byDstPos))
										{
											//update dest stack count
											pDstItem->byStackcount = req->byStackCount2;
											g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);
										}
										else res->wResultCode = QUERY_FAIL;
									}
									else //unstack item -> create new item inside inventory
									{
										sITEM_DATA* pDstItem = new sITEM_DATA;
										memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
										pDstItem->byStackcount = req->byStackCount2;
										pDstItem->byPlace = req->byDstPlace;
										pDstItem->byPosition = req->byDstPos;
										pDstItem->charId = req->charId;

										res->splitItemId = g_pItemManager->CreateItem(*pDstItem);
										pDstItem->itemId = res->splitItemId;
										pCache->InsertItem(pDstItem);
									}
								}
								break;
							}


							//if create/update dest item success, then update source item
							if (res->wResultCode == GAME_SUCCESS)
							{
								if (req->byStackCount1 == 0)
								{
									g_pItemRepository->DeleteById(req->srcItemID); //delete source item
									delete pSrcItem;
									pCache->EraseBankItem(req->srcItemID);
								}
								else
								{
									pSrcItem->byStackcount = req->byStackCount1;
									g_pItemRepository->UpdateCount(req->srcItemID, req->byStackCount1);
								}
							}

						}
						else res->wResultCode = QUERY_FAIL;
					}
					break;

					//stack from shared account bank to ???
					case CONTAINER_TYPE_BANK4:
					{
						if (sITEM_DATA* pSrcItem = pAccount->GetBankItem(req->srcItemID, req->bySrcPlace, req->bySrcPos))
						{
							switch (req->byDstPlace)
							{
								//stack from shared account bank to bank
								case CONTAINER_TYPE_BANK1:
								case CONTAINER_TYPE_BANK2:
								case CONTAINER_TYPE_BANK3:
								{
									if (req->dstItemID != INVALID_ITEMID) //if dest item exist, then update stack from dest item
									{
										if (sITEM_DATA* pDstItem = pCache->GetBankItem(req->dstItemID, req->byDstPlace, req->byDstPos))
										{
											//update dest stack count
											pDstItem->byStackcount = req->byStackCount2;
											g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);
										}
										else res->wResultCode = QUERY_FAIL;
									}
									else //unstack item -> create new item inside bank
									{
										sITEM_DATA* pDstItem = new sITEM_DATA;
										memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
										pDstItem->byStackcount = req->byStackCount2;
										pDstItem->byPlace = req->byDstPlace;
										pDstItem->byPosition = req->byDstPos;

										res->splitItemId = g_pItemManager->CreateItem(*pDstItem);
										pDstItem->itemId = res->splitItemId;
										pCache->InsertBankItem(pDstItem);
									}
								}
								break;

								//stack from shared bank to shared bank
								case CONTAINER_TYPE_BANK4:
								{
									if (req->dstItemID != INVALID_ITEMID) //if dest item exist, then update stack from dest item
									{
										if (sITEM_DATA* pDstItem = pAccount->GetBankItem(req->dstItemID, req->byDstPlace, req->byDstPos))
										{
											//update dest stack count
											pDstItem->byStackcount = req->byStackCount2;
											g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);
										}
										else res->wResultCode = QUERY_FAIL;
									}
									else //unstack item -> create new item inside shared bank
									{
										sITEM_DATA* pDstItem = new sITEM_DATA;
										memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
										pDstItem->byStackcount = req->byStackCount2;
										pDstItem->byPlace = req->byDstPlace;
										pDstItem->byPosition = req->byDstPos;

										res->splitItemId = g_pItemManager->CreateSharedBankItem(*pDstItem, req->accountID);
										pDstItem->itemId = res->splitItemId;
										pAccount->InsertBankItem(pDstItem);
									}
								}
								break;

								//move from shared account bank to inventory
								default:
								{
									if (req->dstItemID != INVALID_ITEMID)
									{
										if (sITEM_DATA* pDstItem = pCache->GetItemData(req->dstItemID, req->byDstPlace, req->byDstPos))
										{
											//update dest stack count
											pDstItem->byStackcount = req->byStackCount2;
											g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);
										}
										else res->wResultCode = QUERY_FAIL;
									}
									else //unstack item -> create new item inside inventory
									{
										sITEM_DATA* pDstItem = new sITEM_DATA;
										memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
										pDstItem->byStackcount = req->byStackCount2;
										pDstItem->byPlace = req->byDstPlace;
										pDstItem->byPosition = req->byDstPos;
										pDstItem->charId = req->charId;

										res->splitItemId = g_pItemManager->CreateItem(*pDstItem);
										pDstItem->itemId = res->splitItemId;
										pCache->InsertItem(pDstItem);
									}
								}
								break;
							}

							//if create/update dest item success, then update source item
							if (res->wResultCode == GAME_SUCCESS)
							{
								if (req->byStackCount1 == 0)
								{
									g_pItemRepository->DeleteById(req->srcItemID); //delete source item
									delete pSrcItem;
									pAccount->EraseBankItem(req->srcItemID);
								}
								else
								{
									pSrcItem->byStackcount = req->byStackCount1;
									g_pItemRepository->UpdateCount(req->srcItemID, req->byStackCount1);
								}
							}
						}
						else res->wResultCode = QUERY_FAIL;
					}
					break;

					//move from inventory to ???
					default:
					{
						if (sITEM_DATA* pSrcItem = pCache->GetItemData(req->srcItemID, req->bySrcPlace, req->bySrcPos))
						{
							switch (req->byDstPlace)
							{
								//stack from inventory to bank
								case CONTAINER_TYPE_BANK1:
								case CONTAINER_TYPE_BANK2:
								case CONTAINER_TYPE_BANK3:
								{
									if (req->dstItemID != INVALID_ITEMID) //if dest item exist, then update stack from dest item
									{
										if (sITEM_DATA* pDstItem = pCache->GetBankItem(req->dstItemID, req->byDstPlace, req->byDstPos))
										{
											//update dest stack count
											pDstItem->byStackcount = req->byStackCount2;
											g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);
										}
										else res->wResultCode = QUERY_FAIL;
									}
									else //unstack item -> create new item inside bank
									{
										sITEM_DATA* pDstItem = new sITEM_DATA;
										memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
										pDstItem->byStackcount = req->byStackCount2;
										pDstItem->byPlace = req->byDstPlace;
										pDstItem->byPosition = req->byDstPos;

										res->splitItemId = g_pItemManager->CreateItem(*pDstItem);
										pDstItem->itemId = res->splitItemId;
										pCache->InsertBankItem(pDstItem);
									}
								}
								break;

								//stack from inventory to shared bank
								case CONTAINER_TYPE_BANK4:
								{
									if (req->dstItemID != INVALID_ITEMID) //if dest item exist, then update stack from dest item
									{
										if (sITEM_DATA* pDstItem = pAccount->GetBankItem(req->dstItemID, req->byDstPlace, req->byDstPos))
										{
											//update dest stack count
											pDstItem->byStackcount = req->byStackCount2;
											g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);
										}
										else res->wResultCode = QUERY_FAIL;
									}
									else //unstack item -> create new item inside shared bank
									{
										sITEM_DATA* pDstItem = new sITEM_DATA;
										memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
										pDstItem->byStackcount = req->byStackCount2;
										pDstItem->byPlace = req->byDstPlace;
										pDstItem->byPosition = req->byDstPos;

										res->splitItemId = g_pItemManager->CreateSharedBankItem(*pDstItem, req->accountID);
										pDstItem->itemId = res->splitItemId;
										pAccount->InsertBankItem(pDstItem);
									}
								}
								break;

								default:
								{
									res->wResultCode = QUERY_FAIL;
								}
								break;
							}

							//if create/update dest item success, then update source item
							if (res->wResultCode == GAME_SUCCESS)
							{
								if (req->byStackCount1 == 0)
								{
									g_pItemRepository->DeleteById(req->srcItemID); //delete source item
									delete pSrcItem;
									pCache->EraseItem(req->srcItemID);
								}
								else
								{
									pSrcItem->byStackcount = req->byStackCount1;
									g_pItemRepository->UpdateCount(req->srcItemID, req->byStackCount1);
								}
							}
						}
						else res->wResultCode = QUERY_FAIL;
					}
					break;
				}
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_BANK_MOVE_STACK_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvBankBuyReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_BANK_BUY_REQ * req = (sGQ_BANK_BUY_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_BANK_BUY_RES));
	sQG_BANK_BUY_RES * res = (sQG_BANK_BUY_RES *)packet.GetPacketData();
	res->wOpCode = QG_BANK_BUY_RES;
	res->handle = req->handle;
	res->npchandle = req->npchandle;
	res->wResultCode = GAME_SUCCESS;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->IsBankLoaded() == true) //check if bank loaded
		{
			if (pCache->GetZeni() >= req->dwZenny)
			{
				pCache->SetZeni(pCache->GetZeni() - req->dwZenny);
				g_pCharacterRepository->UpdateMoney(req->charId, pCache->GetZeni());

				res->itemID = g_pItemManager->CreateBank(req->charId, req->itemNo, req->byPlace, req->byPosition, req->byRank, req->byDurationType, req->nUseStartTime, req->nUseStartTime);

				sITEM_DATA* pData = new sITEM_DATA;
				pData->Init();
				pData->sOptionSet.Init();
				pData->byDurationType = req->byDurationType;
				pData->byPlace = req->byPlace;
				pData->byPosition = req->byPosition;
				pData->byStackcount = 1;
				pData->charId = req->charId;
				pData->itemId = res->itemID;
				pData->byRank = req->byRank;
				pData->itemNo = req->itemNo;
				pData->nUseEndTime = req->nUseEndTime;
				pData->nUseStartTime = req->nUseStartTime;

				pCache->InsertBankItem(pData);
			}
			else
			{
				res->wResultCode = QUERY_FAIL;
				ERR_LOG(LOG_GENERAL, "ERROR: Char %u does not have enough zeni ( %u < %u )", req->charId, req->dwZenny, pCache->GetZeni());
			}
		}
		else 
		{ 
			res->wResultCode = QUERY_FAIL; 
			ERR_LOG(LOG_GENERAL, "ERROR: Char %u cant buy bank because bank has not been loaded", req->charId);
		}
	}
	else
	{
		res->wResultCode = QUERY_FAIL;
		ERR_LOG(LOG_GENERAL, "ERROR: Char %u cant buy bank because the character does not exist in cache", req->charId);
	}

	res->byMerchantTab = req->byMerchantTab;
	res->byMerchantPos = req->byMerchantPos;
	res->dwZenny = req->dwZenny;
	res->itemNo = req->itemNo;
	res->charId = req->charId;
	res->byPlace = req->byPlace;
	res->byPosition = req->byPosition;
	res->byRank = req->byRank;
	res->byDurationType = req->byDurationType;
	res->nUseStartTime = req->nUseStartTime;
	res->nUseEndTime = req->nUseEndTime;
	packet.SetPacketLen(sizeof(sQG_BANK_BUY_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvBankAddWithCommandReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_BANK_ADD_WITH_COMMAND_REQ * req = (sGQ_BANK_ADD_WITH_COMMAND_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_BANK_ADD_WITH_COMMAND_RES));
	sQG_BANK_ADD_WITH_COMMAND_RES * res = (sQG_BANK_ADD_WITH_COMMAND_RES *)packet.GetPacketData();
	res->wOpCode = QG_BANK_ADD_WITH_COMMAND_RES;
	res->handle = req->handle;
	res->wResultCode = GAME_SUCCESS;
	res->itemNo = req->itemNo;
	res->charId = req->charId;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->IsBankLoaded() == true) //bank should already be loaded otherwise the bank slot 0 sITEM_DATA will be created 2x and memory leak
		{
			if (pCache->GetBankItem(req->itemNo) == NULL)
			{
				res->itemID = g_pItemManager->CreateBank(req->charId, req->itemNo, CONTAINER_TYPE_BANKSLOT, 0, 0, 0, 0, 0);

				sITEM_DATA* pData = new sITEM_DATA;
				pData->Init();
				pData->sOptionSet.Init();
				pData->byPlace = CONTAINER_TYPE_BANKSLOT;
				pData->byPosition = 0;
				pData->byStackcount = 1;
				pData->charId = req->charId;
				pData->itemId = res->itemID;
				pData->itemNo = req->itemNo;

				pCache->InsertBankItem(pData);
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQG_BANK_ADD_WITH_COMMAND_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvPcUpdateBindReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_PC_UPDATE_BIND_REQ * req = (sGQ_PC_UPDATE_BIND_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->UpdateBind(req->bindWorldId, req->bindObjectTblidx, req->byBindType, req->vBindLoc, req->vBindDir);

		g_pCharacterRepository->UpdateBind(req->bindWorldId, req->bindObjectTblidx, req->byBindType, req->vBindLoc.x, req->vBindLoc.y, req->vBindLoc.z, req->vBindDir.x, req->vBindDir.y, req->vBindDir.z, req->charId);
	}
}

void CGameServerSession::RecvCharConvertClassReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_CHAR_CONVERT_CLASS_REQ * req = (sGQ_CHAR_CONVERT_CLASS_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->SetClass(req->byClass);

		if (pPlayerCache->GetGuildID() > 0)
		{
			if (sDBO_GUILD_MEMBER_DATA* pMember = g_pGuild->GetGuildMemberData(pPlayerCache->GetGuildID(), req->charId))
				pMember->byClass = req->byClass;
		}

		g_pCharacterRepository->UpdateClass(req->byClass, req->charId);
	}
}

void CGameServerSession::RecvCharConvertGenderNfy(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_CHAR_CONVERT_GENDER_NFY * req = (sGQ_CHAR_CONVERT_GENDER_NFY*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->SetGender(req->byGender);

		g_pCharacterRepository->UpdateGender(req->byGender, req->charId);
	}
}

void CGameServerSession::RecvUpdateCharZennyReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_UPDATE_CHAR_ZENNY_REQ * req = (sGQ_UPDATE_CHAR_ZENNY_REQ*)pPacket->GetPacketData();

	CPlayerCache* pBuyer = g_pPlayerCache->GetCharacter(req->charId);
	if (pBuyer)
	{
		if (req->bIsNew)
			pBuyer->SetZeni(UnsignedSafeIncrease<DWORD>(pBuyer->GetZeni(), req->dwZenny));
		else
			pBuyer->SetZeni(UnsignedSafeDecrease<DWORD>(pBuyer->GetZeni(), req->dwZenny));

		g_pCharacterRepository->UpdateMoney(req->charId, pBuyer->GetZeni());
	}
}

void CGameServerSession::RecvUpdateCharNetpyReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_UPDATE_CHAR_NETPY_REQ* req = (sGQ_UPDATE_CHAR_NETPY_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->SetNetPy(req->dwPoints);
		g_pCharacterRepository->UpdateNetpy(req->dwPoints, req->charId);
	}
}

void CGameServerSession::RecvUpdateCharWaguPointReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_UPDATE_CHAR_WAGUPOINT_REQ* req = (sGQ_UPDATE_CHAR_WAGUPOINT_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->SetWaguPoints(req->dwPoints);
		g_pCharacterRepository->UpdateWaguPoint(req->charId, req->dwPoints);
	}
}

void CGameServerSession::RecvQuestItemCreateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_QUEST_ITEM_CREATE_REQ * req = (sGQ_QUEST_ITEM_CREATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		if (req->aItems.byUpdateType == eQUEST_ITEM_UPDATE_TYPE_UPDATE)
		{
			if(pPlayerCache->UpdateQuestItem(req->aItems.byPos, req->aItems.byCurCount))
				g_pQuestRepository->UpdateQuestItemAmount(req->aItems.byCurCount, req->charId, req->aItems.byPos);
		}
		else
		{
			if(pPlayerCache->CreateQuestItem(req->aItems.itemTblidx, req->aItems.byPos, req->aItems.byCurCount))
				g_pQuestRepository->InsertQuestItem(req->charId, req->aItems.itemTblidx, req->aItems.byCurCount, req->aItems.byPos);
		}
	}
}

void CGameServerSession::RecvQuestItemDeleteReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_QUEST_ITEM_DELETE_REQ * req = (sGQ_QUEST_ITEM_DELETE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		if (pPlayerCache->DeleteQuestItem(req->aItems.byPos))
			g_pQuestRepository->DeleteQuestItem(req->charId, req->aItems.byPos);
	}
}

void CGameServerSession::RecvQuestItemMoveReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_QUEST_ITEM_MOVE_REQ * req = (sGQ_QUEST_ITEM_MOVE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		if (req->dwDestTblidx == INVALID_TBLIDX) //move item
		{
			if(pPlayerCache->MoveQuestItem(req->bySrcPos, req->byDestPos))
				g_pQuestRepository->UpdateQuestItemPos(req->byDestPos, req->charId, req->bySrcPos);
		}
		else //switch item
		{
			if(pPlayerCache->SwitchQuestItem(req->bySrcPos, req->byDestPos))
			{
				g_pQuestRepository->UpdateQuestItemPosByTblidx(req->byDestPos, req->charId, req->dwSrcTblidx, req->bySrcPos);
				g_pQuestRepository->UpdateQuestItemPosByTblidx(req->bySrcPos, req->charId, req->dwDestTblidx, req->byDestPos);
			}
		}
	}
}

void CGameServerSession::RecvQuestProgressCreateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_QUEST_PROGRESS_DATA_CREATE_REQ * req = (sGQ_QUEST_PROGRESS_DATA_CREATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		if (req->bIsComplete)
		{
			if (pPlayerCache->CompleteQuest(req->progressInfo.tId))
			{
				g_pQuestRepository->UpsertQuestProgress(req->charId, req->progressInfo.tId,
					254, 255, req->progressInfo.uData.sQInfoV0.tgExcCGroup,
					254, 255, 255, 255,
					req->progressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[0], req->progressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[1], req->progressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[2], req->progressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[3],
					req->progressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[0].tcId, req->progressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[0].taId, req->progressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[0].uiRemainTime, INVALID_WORD);
			}
		}
		else
		{
			pPlayerCache->StoreQuestProgress(req->progressInfo);

			g_pQuestRepository->UpsertQuestProgress(req->charId, req->progressInfo.tId,
				req->progressInfo.uData.sQInfoV0.tcQuestInfo, req->progressInfo.uData.sQInfoV0.taQuestInfo, req->progressInfo.uData.sQInfoV0.tgExcCGroup,
				req->progressInfo.uData.sQInfoV0.sMainTSP.tcPreId, req->progressInfo.uData.sQInfoV0.sMainTSP.tcCurId, req->progressInfo.uData.sQInfoV0.sSToCEvtData.tcId, req->progressInfo.uData.sQInfoV0.sSToCEvtData.taId,
				req->progressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[0], req->progressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[1], req->progressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[2], req->progressInfo.uData.sQInfoV0.sSToCEvtData.m_aUserData[3],
				req->progressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[0].tcId, req->progressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[0].taId, req->progressInfo.uData.sQInfoV0.sETSlot.asExceptTimer[0].uiRemainTime, req->progressInfo.uData.sQInfoV0.wQState);
		}
	}
}

void CGameServerSession::RecvQuestProgressDataDeleteReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_QUEST_PROGRESS_DATA_DELETE_REQ * req = (sGQ_QUEST_PROGRESS_DATA_DELETE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		if (pPlayerCache->DeleteQuestProgressData(req->questID))
			g_pQuestRepository->DeleteQuestProgress(req->charId, req->questID);
	}
}

void CGameServerSession::RecvQuestServerEventUpdateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_QUEST_SERVER_EVENT_UPDATE_REQ * req = (sGQ_QUEST_SERVER_EVENT_UPDATE_REQ*)pPacket->GetPacketData();

	UNREFERENCED_PARAMETER(app);
	UNREFERENCED_PARAMETER(req);
}


void CGameServerSession::RecvQuickSlotUpdateReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_QUICK_SLOT_UPDATE_REQ * req = (sGQ_QUICK_SLOT_UPDATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->UpdateQuickSlot(req->bySlotID, req->byType, req->tblidx, req->itemID);

		if (req->byType == QUICK_SLOT_TYPE_ITEM)
		{
			g_pQuickSlotRepository->UpsertQuickSlot(req->charId, req->tblidx, req->bySlotID, req->byType, req->itemID);
		}
		else
		{
			g_pQuickSlotRepository->UpsertQuickSlot(req->charId, req->tblidx, req->bySlotID, req->byType, 0);
		}
	}
}


void CGameServerSession::RecvQuickSlotDelReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_QUICK_SLOT_DEL_REQ * req = (sGQ_QUICK_SLOT_DEL_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->DeleteQuickSlot(req->bySlotID);

		g_pQuickSlotRepository->DeleteQuickSlot(req->charId, req->bySlotID);
	}
}

void CGameServerSession::RecvTradeReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_TRADE_REQ * req = (sGQ_TRADE_REQ*)pPacket->GetPacketData();

	//owner = delete item, take zeni
	//target = give item, give zeni

	CNtlPacket packet(sizeof(sQG_TRADE_RES));
	sQG_TRADE_RES * res = (sQG_TRADE_RES *)packet.GetPacketData();
	res->wOpCode = QG_TRADE_RES;
	memcpy(res->asRecvData, req->asGiveData, sizeof(req->asGiveData));
	memcpy(res->asSendData, req->asTakeData, sizeof(req->asTakeData));
	res->byRecvCount = req->byTakeCount;
	res->bySendCount = req->byGiveCount;
	res->charID = req->charID;
	res->dstcharID = req->dstcharID;
	res->dwGiveZenny = req->dwGiveZenny;
	res->dwTakeZenny = req->dwTakeZenny;
	res->handle = req->handle;
	res->hTarget = req->hTarget;
	res->wResultCode = GAME_SUCCESS;

	if (req->byGiveCount <= TRADE_INVEN_MAX_COUNT && req->byTakeCount <= TRADE_INVEN_MAX_COUNT)
	{
		CPlayerCache* pOwner = g_pPlayerCache->GetCharacter(req->charID);
		CPlayerCache* pTarget = g_pPlayerCache->GetCharacter(req->dstcharID);
		if (pOwner && pTarget)
		{
			if (pOwner->GetZeni() >= req->dwGiveZenny)
			{
				if (req->dwGiveZenny > 0)
				{
					pOwner->SetZeni(UnsignedSafeDecrease<DWORD>(pOwner->GetZeni(), req->dwGiveZenny));
					g_pCharacterRepository->UpdateMoney(req->charID, pOwner->GetZeni());

					pTarget->SetZeni(UnsignedSafeIncrease<DWORD>(pTarget->GetZeni(), req->dwTakeZenny));
					g_pCharacterRepository->UpdateMoney(req->dstcharID, pTarget->GetZeni());
				}

				//add items to target
				for (BYTE i = 0; i < req->byGiveCount; i++)
				{
					if (sITEM_DATA* pItem = pOwner->GetItemData(req->asGiveData[i].itemSerial))
					{
						sITEM_DATA* pNewItem = new sITEM_DATA;
						memcpy(pNewItem, pItem, sizeof(sITEM_DATA));
						pNewItem->charId = req->dstcharID;
						pNewItem->byPlace = req->asGiveData[i].byDstPlace;
						pNewItem->byPosition = req->asGiveData[i].byDstPos;
						pNewItem->byStackcount = req->asGiveData[i].byDstCount;

						res->asRecvData[i].itemNewSerial = pNewItem->itemId = g_pItemManager->CreateItem(*pNewItem);

						pTarget->InsertItem(pNewItem);
					}
				}

				//remove items from owner
				for (BYTE i = 0; i < req->byTakeCount; i++)
				{
					if (req->asTakeData[i].byDstCount == 0)
					{
						if (pOwner->RemoveItem(req->asTakeData[i].itemSerial))
							g_pItemRepository->DeleteById(req->asTakeData[i].itemSerial);
					}
					else
					{
						if (sITEM_DATA* pItem = pOwner->GetItemData(req->asTakeData[i].itemSerial))
						{
							pItem->byStackcount = req->asTakeData[i].byDstCount;
							g_pItemRepository->UpdateCount(req->asTakeData[i].itemSerial, pItem->byStackcount);
						}
					}
				}
			}
			else res->wResultCode = GAME_ZENNY_NOT_ENOUGH;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	g_pAuditLogRepository->InsertTradeLog(req->charID, req->dstcharID, req->dwGiveZenny, req->byGiveCount,
		res->asRecvData[0].itemNewSerial, res->asRecvData[0].itemTblidx, res->asRecvData[1].itemNewSerial, res->asRecvData[1].itemTblidx, res->asRecvData[2].itemNewSerial, res->asRecvData[2].itemTblidx,
		res->asRecvData[3].itemNewSerial, res->asRecvData[3].itemTblidx, res->asRecvData[4].itemNewSerial, res->asRecvData[4].itemTblidx, res->asRecvData[5].itemNewSerial, res->asRecvData[5].itemTblidx,
		res->asRecvData[6].itemNewSerial, res->asRecvData[6].itemTblidx, res->asRecvData[7].itemNewSerial, res->asRecvData[7].itemTblidx, res->asRecvData[8].itemNewSerial, res->asRecvData[8].itemTblidx,
		res->asRecvData[9].itemNewSerial, res->asRecvData[9].itemTblidx, res->asRecvData[10].itemNewSerial, res->asRecvData[10].itemTblidx, res->asRecvData[11].itemNewSerial, res->asRecvData[11].itemTblidx
	);

	packet.SetPacketLen(sizeof(sQG_TRADE_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvPrivateShopItemBuyReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_PRIVATESHOP_ITEM_BUYING_REQ * req = (sGQ_PRIVATESHOP_ITEM_BUYING_REQ*)pPacket->GetPacketData();

	CPlayerCache* pBuyer = g_pPlayerCache->GetCharacter(req->charID_To);
	CPlayerCache* pSeller = g_pPlayerCache->GetCharacter(req->charID_From);

	BOOL bIssues = FALSE;
	std::string strIssue = "None";
	TBLIDX itemIdx[NTL_MAX_BUY_SHOPPING_CART] = { 0 };

	if (pBuyer && pSeller)
	{
		if (pBuyer->GetZeni() >= req->dwAllZenny)
		{
			for (BYTE i = 0; i < req->byCount; i++)
			{
				itemIdx[i] = pSeller->SwitchItem(req->asEmpty[i].itemID, pBuyer, req->asEmpty[i].byPlace, req->asEmpty[i].byPos);

				if (itemIdx[i] != INVALID_TBLIDX)
				{
					g_pItemRepository->UpdateOwnerPlace(req->asEmpty[i].itemID, req->charID_To, req->asEmpty[i].byPlace, req->asEmpty[i].byPos);
				}
				else
				{
					bIssues = TRUE;
					strIssue = "Could not switch item ownership.. For sure abusing exploit";
					ERR_LOG(LOG_GENERAL, "Could not switch item ownership.. Seller: %u, Buyer: %u", req->charID_From, req->charID_To);
					break;
				}
			}

			if (bIssues == FALSE)
			{
				pBuyer->SetZeni(UnsignedSafeDecrease<DWORD>(pBuyer->GetZeni(), req->dwAllZenny));
				pSeller->SetZeni(UnsignedSafeIncrease<DWORD>(pSeller->GetZeni(), req->dwAllZenny));

				g_pCharacterRepository->UpdateMoney(req->charID_To, pBuyer->GetZeni());
				g_pCharacterRepository->UpdateMoney(req->charID_From, pSeller->GetZeni());
			}
		}
		else
		{
			bIssues = TRUE;
			strIssue = "Buyer does not have enough zeni";
		}
	}
	else
	{
		bIssues = TRUE;
		strIssue = "Buyer or Seller not found in database";
	}

	g_pAuditLogRepository->InsertPrivateShopLog(req->charID_From, req->charID_To, req->dwAllZenny, req->byCount,
		req->asEmpty[0].itemID, itemIdx[0], req->asEmpty[1].itemID, itemIdx[1], req->asEmpty[2].itemID, itemIdx[2],
		req->asEmpty[3].itemID, itemIdx[3], req->asEmpty[4].itemID, itemIdx[4], req->asEmpty[5].itemID, itemIdx[5],
		req->asEmpty[6].itemID, itemIdx[6], req->asEmpty[7].itemID, itemIdx[7], req->asEmpty[8].itemID, itemIdx[8],
		req->asEmpty[9].itemID, itemIdx[9], req->asEmpty[10].itemID, itemIdx[10], req->asEmpty[11].itemID, itemIdx[11],
		bIssues, strIssue.c_str());
}

void CGameServerSession::RecvRankBattleScoreUpdateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_RANKBATTLE_SCORE_UPDATE_REQ * req = (sGQ_RANKBATTLE_SCORE_UPDATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		pCache->SetMudusaPoints(pCache->GetMudusaPoints() + req->dwMudosaPoint);
		pCache->SetRankBattleScoreInfo(req->sScoreInfo);

		if (req->dwMudosaPoint > 0)
			g_pCharacterRepository->UpdateMudosaPoint(pCache->GetMudusaPoints(), req->charID);

		g_pCharacterRepository->UpsertRankBattle(req->charID, req->sScoreInfo.dwWin, req->sScoreInfo.dwDraw, req->sScoreInfo.dwLose, req->sScoreInfo.wStraightKOWin, req->sScoreInfo.wMaxStraightKOWin, req->sScoreInfo.wMaxStraightWin, req->sScoreInfo.wStraightWin, req->sScoreInfo.fPoint);
	}
}


void CGameServerSession::RecvTutorialDataUpdateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_TUTORIAL_DATA_UPDATE_REQ * req = (sGQ_TUTORIAL_DATA_UPDATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		g_pCharacterRepository->UpdateTutorialFlag(req->bTutorialFlag, req->charId);

		if (req->bTutorialFlag == false)
			pCache->SetTutorialFlag(true);
		else
			pCache->SetTutorialFlag(false);
	}
}



void CGameServerSession::RecvMailStartReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MAIL_START_REQ * req = (sGQ_MAIL_START_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		pCache->SetSession(GetHandle());

		g_pMailRepository->LoadMailboxAsync(pCache, req->charID, req->handle, req->hObject);
	}
}

void CGameServerSession::RecvMailSendReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_MAIL_SEND_REQ * req = (sGQ_MAIL_SEND_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_MAIL_SEND_RES));
	sQG_MAIL_SEND_RES * res = (sQG_MAIL_SEND_RES *)packet.GetPacketData();
	res->wOpCode = QG_MAIL_SEND_RES;
	res->handle = req->handle;
	res->hObject = req->hObject;
	res->charID = req->charID;
	wcscpy_s(res->wszTargetName, NTL_MAX_SIZE_CHAR_NAME + 1, req->wszTargetName);
	res->byMailType = req->byMailType;
	res->wResultCode = GAME_SUCCESS;
	res->sItemData = req->sItemData;
	res->dwZenny = req->dwZenny;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		DWORD dwFee = NTL_MAX_BASIC_MAIL_SEND_ZENNY;
		if (req->byMailType != eMAIL_TYPE_BASIC)
			dwFee = NTL_MAX_ATTACH_MAIL_SEND_ZENNY;

		if (req->byMailType != eMAIL_TYPE_ITEM_ZENNY_REQ && req->byMailType != eMAIL_TYPE_ZENNY_REQ)
			dwFee += req->dwZenny;

		res->dwZenny = dwFee;

		if (pCache->GetZeni() >= dwFee)
		{
			char* chTargetName = Ntl_WC2MB(req->wszTargetName);

			smart_ptr<QueryResult> result = g_pCharacterRepository->GetCharIdAndAwayByName(chTargetName);
			if (result)
			{
				Field* f = result->Fetch();

				if (f[1].GetBool())
				{
					res->wResultCode = GAME_MAIL_TARGET_AWAY_STATE; //cant accept mails when away
					goto END;
				}

				if (req->sItemData.itemID > 0) //check if send item
				{
					if (pCache->RemoveItem(req->sItemData.itemID) == false)
					{
						res->wResultCode = QUERY_FAIL;
						goto END;
					}

					g_pItemRepository->ClearOwner(req->sItemData.itemID);
				}

				res->targetCharID = f[0].GetUInt32();

				SYSTEMTIME ti;
				GetLocalTime(&ti);
				char* message = Ntl_WC2MB(req->wszText);

				g_pMailRepository->InsertTargetedMail(res->targetCharID, eMAIL_SENDER_TYPE_BASIC, req->byMailType, req->byTextSize, GetCharDB.EscapeString(message).c_str(), req->dwZenny, req->sItemData.itemID, chTargetName, req->wszName, req->byDay, ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond);

				Ntl_CleanUpHeapString(message);

				pCache->SetZeni(UnsignedSafeDecrease<DWORD>(pCache->GetZeni(), dwFee));
				g_pCharacterRepository->UpdateMoney(req->charID, pCache->GetZeni());

			}
			else res->wResultCode = GAME_MAIL_NOT_EXISTING_PLAYER;

			Ntl_CleanUpHeapString(chTargetName);
		}
		else res->wResultCode = GAME_ZENNY_NOT_ENOUGH;
	}
	else res->wResultCode = QUERY_FAIL;

	END:

	res->bIsTemp = false;
	packet.SetPacketLen(sizeof(sQG_MAIL_SEND_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvMailReadReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MAIL_READ_REQ * req = (sGQ_MAIL_READ_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		sMAIL_DATA* pMail = pCache->GetMailData(req->mailID);
		if (pMail)
		{
			pMail->bIsRead = true;
			g_pMailRepository->UpdateReadFlag(req->mailID);

			if (pMail->bySenderType == eMAIL_SENDER_TYPE_BASIC || pMail->bySenderType == eMAIL_SENDER_TYPE_REPLY || pMail->bySenderType == eMAIL_SENDER_TYPE_RETURN)
				pCache->DecreaseMailUnreadNormal();

			else if (pMail->bySenderType == eMAIL_SENDER_TYPE_SYSTEM || pMail->bySenderType == eMAIL_SENDER_TYPE_GM || pMail->bySenderType == eMAIL_SENDER_TYPE_QUEST)
				pCache->DecreaseMailUnreadManager();
		}
	}
}


void CGameServerSession::RecvMailDelReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MAIL_DEL_REQ * req = (sGQ_MAIL_DEL_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		if (sMAIL_DATA* pMail = pCache->GetMailData(req->mailID))
		{
			char* message = Ntl_WC2MB(pMail->wszText);

			g_pAuditLogRepository->InsertMailDeletedLog(pMail->mailID, req->charID, pMail->bySenderType, pMail->byMailType, pMail->byTextSize, GetCharDB.EscapeString(message).c_str(), pMail->dwZenny, pMail->sItemData.itemId, pMail->wszFromName, pMail->bIsAccept, pMail->bIsLock, pMail->bIsRead, pMail->endTime
			, pMail->tCreateTime.year, pMail->tCreateTime.month, pMail->tCreateTime.day, pMail->tCreateTime.hour, pMail->tCreateTime.minute, pMail->tCreateTime.second);

			pCache->DeleteMail(req->mailID);

			g_pMailRepository->DeleteMailById(req->mailID);

			Ntl_CleanUpHeapString(message);
		}
	}
}


void CGameServerSession::RecvMailReturnReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_MAIL_RETURN_REQ * req = (sGQ_MAIL_RETURN_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_MAIL_RETURN_RES));
	sQG_MAIL_RETURN_RES * res = (sQG_MAIL_RETURN_RES *)packet.GetPacketData();
	res->wOpCode = QG_MAIL_RETURN_RES;
	res->wResultCode = GAME_SUCCESS;
	res->charID = req->charID;
	res->handle = req->handle;
	res->hObject = req->hObject;
	res->mailID = req->mailID;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		sMAIL_DATA* pMail = pCache->GetMailData(req->mailID);
		if (pMail)
		{
			if (pMail->bIsAccept == false)
			{
				if (pMail->bIsLock == false)
				{
					smart_ptr<QueryResult> qrGetFromName = g_pMailRepository->GetFromNameById(pMail->mailID); //we have to do this, in case the sender changed his char name
					if (qrGetFromName)
					{
						Field* fGetFromName = qrGetFromName->Fetch();

						const char* chFromName = fGetFromName->GetString();

						smart_ptr<QueryResult> result = g_pCharacterRepository->GetCharIdByNameUtf8(chFromName);
						if (result)
						{
							Field* f = result->Fetch();
							g_pMailRepository->UpdateReturnToSender(f[0].GetUInt32(), chFromName, pCache->GetCharName(), req->mailID);

							//delete mail from owner
							pCache->DeleteMail(req->mailID);
						}
						else res->wResultCode = GAME_MAIL_NOT_EXISTING_PLAYER;
					}
					else res->wResultCode = GAME_MAIL_NOT_EXISTING_PLAYER;
				}
				else res->wResultCode = GAME_MAIL_INVALID_LOCK;
			}
			else res->wResultCode = GAME_MAIL_INVALID_ACCEPT;
		}
		else res->wResultCode = GAME_MAIL_NOT_FOUND;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_MAIL_RETURN_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvMailReloadReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MAIL_RELOAD_REQ * req = (sGQ_MAIL_RELOAD_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		pCache->SetSession(GetHandle());

		if (req->bIsSchedule)
		{
			g_pMailRepository->ReloadMailboxAsync(pCache, req->charID, req->handle, true);
		}
		else
		{
			g_pMailRepository->ReloadMailboxAsync(pCache, req->charID, req->handle, false);
		}
	}
}

void CGameServerSession::RecvMailLoadReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_MAIL_LOAD_REQ * req = (sGQ_MAIL_LOAD_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		pCache->SetSession(GetHandle());

		g_pMailRepository->LoadMailDetailAsync(pCache, req->charID, req->handle, req->hObject);
	}
	else
	{
		CNtlPacket packet2(sizeof(sQG_MAIL_LOAD_RES));
		sQG_MAIL_LOAD_RES * res2 = (sQG_MAIL_LOAD_RES *)packet2.GetPacketData();
		res2->wOpCode = QG_MAIL_START_RES;
		res2->handle = req->handle;
		res2->hObject = req->hObject;
		res2->charID = req->charID;
		res2->wResultCode = QUERY_FAIL;
		packet2.SetPacketLen(sizeof(sQG_MAIL_LOAD_RES));
		app->Send(GetHandle(), &packet2);
	}
}


void CGameServerSession::RecvMailItemReceiveReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_MAIL_ITEM_RECEIVE_REQ * req = (sGQ_MAIL_ITEM_RECEIVE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_MAIL_ITEM_RECEIVE_RES));
	sQG_MAIL_ITEM_RECEIVE_RES * res = (sQG_MAIL_ITEM_RECEIVE_RES *)packet.GetPacketData();
	res->wOpCode = QG_MAIL_ITEM_RECEIVE_RES;
	res->byMailType = req->byMailType;
	res->charID = req->charID;
	res->handle = req->handle;
	res->hObject = req->hObject;
	res->mailID = req->mailID;
	res->wResultCode = GAME_SUCCESS;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		sMAIL_DATA* pMail = pCache->GetMailData(req->mailID);
		if (pMail)
		{
			if (pMail->bIsAccept == false) //already accepted?!?!!!!! Hacker!
			{
				if (pMail->dwZenny > 0) //if mail has zeni attached
				{
					if (req->byMailType == eMAIL_TYPE_ITEM_ZENNY_REQ || req->byMailType == eMAIL_TYPE_ZENNY_REQ) //add mail with zeni to receiver 
					{
						if (pMail->bySenderType != eMAIL_SENDER_TYPE_RETURN) //only send zeni if not returned
						{
							if (pCache->GetZeni() >= pMail->dwZenny)
							{
								smart_ptr<QueryResult> qrGetFromName = g_pMailRepository->GetFromNameById(pMail->mailID); //we have to do this, in case the sender changed his char name
								if (qrGetFromName)
								{
									Field* fGetFromName = qrGetFromName->Fetch();

									const char* chFromName = fGetFromName->GetString();

									smart_ptr<QueryResult> receiver = g_pCharacterRepository->GetCharIdByNameUtf8(chFromName);
									if (receiver)
									{
										Field* r = receiver->Fetch();
										res->fromCharId = r[0].GetUInt32();

										SYSTEMTIME ti;
										GetLocalTime(&ti);

										g_pMailRepository->InsertTargetedMail(res->fromCharId, eMAIL_SENDER_TYPE_SYSTEM, eMAIL_TYPE_ZENNY, 17, "You received Zeni", pMail->dwZenny, 0, chFromName, pCache->GetCharName(), 10, ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond);

										res->dwZenny = pMail->dwZenny;

										pCache->SetZeni(pCache->GetZeni() - pMail->dwZenny);
										g_pCharacterRepository->UpdateMoney(req->charID, pCache->GetZeni());
									}
									else res->wResultCode = GAME_MAIL_NOT_EXISTING_PLAYER;
								}
								else res->wResultCode = GAME_MAIL_NOT_EXISTING_PLAYER;
							}
							else res->wResultCode = GAME_MAIL_INVALID_ZENNY;
						}
					}
					else if (req->byMailType == eMAIL_TYPE_ZENNY || req->byMailType == eMAIL_TYPE_ITEM_ZENNY) //add zeni to the receiver
					{
						pCache->SetZeni(UnsignedSafeIncrease<DWORD>(pCache->GetZeni(), pMail->dwZenny));
						g_pCharacterRepository->UpdateMoney(req->charID, pCache->GetZeni());
						res->dwZenny = pMail->dwZenny;
					}
				}

				if (res->wResultCode == GAME_SUCCESS)
				{
					pMail->bIsAccept = true;

					if (pMail->sItemData.itemId > 0 && pMail->sItemData.itemId != INVALID_ITEMID) //if has item attached
					{
						g_pItemRepository->UpdateOwnerPlacePos(pMail->sItemData.itemId, req->charID, req->sInven.byPlace, req->sInven.byPos);

						memcpy(&res->sItemData, &pMail->sItemData, sizeof(sITEM_DATA));
						res->sItemData.charId = req->charID;
						res->sItemData.byPlace = req->sInven.byPlace;
						res->sItemData.byPosition = req->sInven.byPos;

						//add item to player cache
						pCache->AddItem(res->sItemData);
					}

					g_pMailRepository->UpdateAcceptFlag(req->mailID);
				}
			}
			else res->wResultCode = GAME_MAIL_INVALID_ACCEPT;
		}
		else res->wResultCode = GAME_MAIL_NOT_FOUND;
	}

	packet.SetPacketLen(sizeof(sQG_MAIL_ITEM_RECEIVE_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvMailLockReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MAIL_LOCK_REQ * req = (sGQ_MAIL_LOCK_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		sMAIL_DATA* pMail = pCache->GetMailData(req->mailID);
		if (pMail)
		{
			pMail->bIsLock = req->bIsLock;
			g_pMailRepository->UpdateLockFlag(req->bIsLock, req->mailID);
		}
	}
}


void CGameServerSession::RecvCharAwayReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_CHAR_AWAY_REQ * req = (sGQ_CHAR_AWAY_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		pCache->SetIsMailAway(req->bIsAway);
		g_pCharacterRepository->UpdateIsMailAway(req->bIsAway, req->charID);
	}
}


void CGameServerSession::RecvCharKeyUpdateReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_CHAR_KEY_UPDATE_REQ * req = (sGQ_CHAR_KEY_UPDATE_REQ*)pPacket->GetPacketData();

	CAccountCache* pCache = g_pPlayerCache->GetAccount(req->accountID);
	if (pCache)
	{
		for (BYTE i = 0; i < req->byCount; i++)
		{
			if (req->asData[i].byType == eSHORTCUT_CHANGE_TYPE_ADD)
			{
				pCache->AddShortcut(req->asData[i].wActionID, req->asData[i].wKey);
				g_pAccountRepository->InsertShortcut(req->accountID, req->asData[i].wActionID, req->asData[i].wKey);
			}
			else if (req->asData[i].byType == eSHORTCUT_CHANGE_TYPE_DEL)
			{
				pCache->DeleteShortcut(req->asData[i].wActionID);
				g_pAccountRepository->DeleteShortcut(req->accountID, req->asData[i].wActionID);
			}
			else if (req->asData[i].byType == eSHORTCUT_CHANGE_TYPE_UPDATE)
			{
				if(pCache->UpdateShortcut(req->asData[i].wActionID, req->asData[i].wKey))
					g_pAccountRepository->UpdateShortcutKey(req->asData[i].wKey, req->accountID, req->asData[i].wActionID);
			}
		}
	}
}


void CGameServerSession::RecvPortalAddReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_PORTAL_ADD_REQ * req = (sGQ_PORTAL_ADD_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pPlayerCache)
	{
		pPlayerCache->AddPortal(req->PortalID);
	}

	g_pCharacterRepository->InsertPortal(req->charID, req->PortalID);
}

void CGameServerSession::RecvWarFogUpdateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_WAR_FOG_UPDATE_REQ * req = (sGQ_WAR_FOG_UPDATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pPlayerCache)
	{
		pPlayerCache->UpdateWarFog(req->sInfo.achWarFogFlag);
	}

	g_pCharacterRepository->InsertWarFog(req->charID, req->contentsTblidx);
}


void CGameServerSession::RecvGuildBankLoadReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_GUILD_BANK_LOAD_REQ * req = (sGQ_GUILD_BANK_LOAD_REQ*)pPacket->GetPacketData();

	bool bGuildBankCreated = false;

	CNtlPacket packet2(sizeof(sQG_GUILD_BANK_LOAD_RES));
	sQG_GUILD_BANK_LOAD_RES * res2 = (sQG_GUILD_BANK_LOAD_RES *)packet2.GetPacketData();
	res2->wOpCode = QG_GUILD_BANK_LOAD_RES;
	res2->wResultCode = GAME_SUCCESS;
	res2->dwZenny = 0;
	res2->charID = req->charID;
	res2->guildID = req->guildID;
	res2->handle = req->handle;
	res2->hNpcHandle = req->hNpcHandle;


	if (CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID))
	{
		sGUILD_BANK* pBank = g_pGuild->GetGuildBank(req->guildID);
		if (pBank == NULL)
		{
			pBank = new sGUILD_BANK;			
			g_pGuild->InsertGuildBank(req->guildID, pBank);

			bGuildBankCreated = true;
		}

		if (bGuildBankCreated) //if new bank created, then we do query.
		{
			if (smart_ptr<QueryResult> result = g_pItemRepository->GetItemsByGuildId(req->guildID))
			{
				do
				{
					Field* f = result->Fetch();

					sITEM_DATA* pItemData = new sITEM_DATA;

					pItemData->itemId = f[0].GetUInt64();
					pItemData->itemNo = f[1].GetUInt32();
					pItemData->charId = req->charID;
					pItemData->byPlace = f[3].GetBYTE();
					pItemData->byPosition = f[4].GetBYTE();
					pItemData->byStackcount = f[5].GetBYTE();
					pItemData->byRank = f[6].GetBYTE();
					pItemData->byCurrentDurability = f[7].GetBYTE();
					pItemData->byGrade = f[8].GetBYTE();
					pItemData->bNeedToIdentify = f[9].GetBool();
					pItemData->byBattleAttribute = f[10].GetBYTE();
					NTL_SAFE_WCSCPY(pItemData->awchMaker, s2ws(f[11].GetString()).c_str());
					pItemData->sOptionSet.aOptionTblidx[0] = f[12].GetUInt32();
					pItemData->sOptionSet.aOptionTblidx[1] = f[13].GetUInt32();
					pItemData->sOptionSet.aRandomOption[0].wOptionIndex = f[14].GetWORD();
					pItemData->sOptionSet.aRandomOption[0].optionValue = f[15].GetINT();
					pItemData->sOptionSet.aRandomOption[1].wOptionIndex = f[16].GetWORD();
					pItemData->sOptionSet.aRandomOption[1].optionValue = f[17].GetINT();
					pItemData->sOptionSet.aRandomOption[2].wOptionIndex = f[18].GetWORD();
					pItemData->sOptionSet.aRandomOption[2].optionValue = f[19].GetINT();
					pItemData->sOptionSet.aRandomOption[3].wOptionIndex = f[20].GetWORD();
					pItemData->sOptionSet.aRandomOption[3].optionValue = f[21].GetINT();
					pItemData->sOptionSet.aRandomOption[4].wOptionIndex = f[22].GetWORD();
					pItemData->sOptionSet.aRandomOption[4].optionValue = f[23].GetINT();
					pItemData->sOptionSet.aRandomOption[5].wOptionIndex = f[24].GetWORD();
					pItemData->sOptionSet.aRandomOption[5].optionValue = f[25].GetINT();
					pItemData->sOptionSet.aRandomOption[6].wOptionIndex = f[26].GetWORD();
					pItemData->sOptionSet.aRandomOption[6].optionValue = f[27].GetINT();
					pItemData->sOptionSet.aRandomOption[7].wOptionIndex = f[28].GetWORD();
					pItemData->sOptionSet.aRandomOption[7].optionValue = f[29].GetINT();
					pItemData->nUseStartTime = (DBOTIME)f[30].GetDWORD();
					pItemData->nUseEndTime = (DBOTIME)f[31].GetDWORD();
					pItemData->byRestrictState = f[32].GetBYTE();
					pItemData->byDurationType = f[33].GetBYTE();

					pBank->m_mapItems.insert(std::make_pair(pItemData->itemId, pItemData));

				} while (result->NextRow());
			}

			if (smart_ptr<QueryResult> result2 = g_pGuildRepository->GetZeniByGuildId(req->guildID))
			{
				Field* f = result2->Fetch();

				pBank->dwZeni = f[0].GetUInt32();
			}
		}

		//send item info
		for (BYTE i = CONTAINER_TYPE_GUILD1; i <= CONTAINER_TYPE_GUILD3; i++)
		{
			int nItemCount = 0;
			
			CNtlPacket packet(sizeof(sQG_GUILD_BANK_LOAD_DATA));
			sQG_GUILD_BANK_LOAD_DATA * res = (sQG_GUILD_BANK_LOAD_DATA *)packet.GetPacketData();
			res->wOpCode = QG_GUILD_BANK_LOAD_DATA;
			res->charID = req->charID;
			res->byTotalCount = (BYTE)pBank->m_mapItems.size();
			res->byCurPacketCount = i;
			res->byItemCount = 0;

			for (std::map<ITEMID, sITEM_DATA*>::iterator it = pBank->m_mapItems.begin(); it != pBank->m_mapItems.end(); it++)
			{
				if (it->second->byPlace != i)
					continue;

				++nItemCount;

				memcpy(&res->asItemData[res->byItemCount++], it->second, sizeof(sITEM_DATA));

				if (res->byItemCount == res->byTotalCount || res->byItemCount == NTL_MAX_GUILD_ITEM_SLOT / 2)
				{
					packet.SetPacketLen(sizeof(sQG_GUILD_BANK_LOAD_DATA));
					app->Send(GetHandle(), &packet);

					res->byItemCount = 0;
					::ZeroMemory(res->asItemData, sizeof(sITEM_DATA) * (NTL_MAX_GUILD_ITEM_SLOT / 2));
				}
			}

			if (nItemCount == 0 || res->byItemCount > 0) //if no item found or packet has not been sent
			{
				packet.SetPacketLen(sizeof(sQG_GUILD_BANK_LOAD_DATA));
				app->Send(GetHandle(), &packet);
			}
		}


		//zeni info
		res2->dwZenny = pBank->dwZeni;
	}
	else res2->wResultCode = QUERY_FAIL;

	
	packet2.SetPacketLen(sizeof(sQG_GUILD_BANK_LOAD_RES));
	app->Send(GetHandle(), &packet2);
}


void CGameServerSession::RecvGuildBankMoveReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_GUILD_BANK_MOVE_REQ * req = (sGQ_GUILD_BANK_MOVE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_GUILD_BANK_MOVE_RES));
	sQG_GUILD_BANK_MOVE_RES * res = (sQG_GUILD_BANK_MOVE_RES *)packet.GetPacketData();
	res->wOpCode = QG_GUILD_BANK_MOVE_RES;
	res->handle = req->handle;
	res->wResultCode = GAME_SUCCESS;
	res->hNpcHandle = req->hNpcHandle;

	if (CPlayerCache* pPlayer = g_pPlayerCache->GetCharacter(req->charId))
	{
		if (sGUILD_BANK* pBank = g_pGuild->GetGuildBank(req->guildID))
		{
			//get pSrcItem.. Note pSrcItem might be from inventory or from guild bank
			sITEM_DATA* pSrcItem = NULL;
			if (IsGuildContainer(req->bySrcPlace))
				pSrcItem = pBank->GetItemData(req->srcItemID);
			else
				pSrcItem = pPlayer->GetItemData(req->srcItemID);

			//get pDstItem.. Note dest item might be from inventory or from guild bank
			sITEM_DATA* pDstItem = NULL;
			if (req->dstItemId != INVALID_ITEMID)
			{
				if (IsGuildContainer(req->byDstPlace))
					pDstItem = pBank->GetItemData(req->dstItemId);
				else
					pDstItem = pPlayer->GetItemData(req->dstItemId);
			}

			if (pSrcItem && ((req->dstItemId != INVALID_ITEMID && pDstItem) || (req->dstItemId == INVALID_ITEMID && pDstItem == NULL)))
			{
				if (IsGuildContainer(req->byDstPlace)) //check if moving item into/within guild bank
				{
					if (req->bySrcPlace >= CONTAINER_TYPE_INVEN_FIRST && req->bySrcPlace <= CONTAINER_TYPE_INVEN_LAST) //if move from inventory into guild bank
					{
						//src item to guild container
						g_pItemRepository->MoveToGuildBank(req->srcItemID, req->byDstPlace, req->byDstPos, req->guildID);
						pPlayer->EraseItem(req->srcItemID); //remove from inventory
						pBank->m_mapItems.insert(std::make_pair(req->srcItemID, pSrcItem)); //insert to guild bank
						pSrcItem->byPlace = req->byDstPlace; //set pSrcItem place to dest place
						pSrcItem->byPosition = req->byDstPos; //set pSrcItem pos to dest pos

						//if dest item exist, then move dest item from guild bank to inventory
						if (pDstItem)
						{
							g_pItemRepository->MoveFromGuildBankToOwner(req->dstItemId, req->charId, req->bySrcPlace, req->bySrcPos);
							pBank->m_mapItems.erase(req->dstItemId); //remove from guild bank
							pPlayer->InsertItem(pDstItem); //insert from inventory
							pDstItem->byPlace = req->bySrcPlace;//set pDstItem place to src place
							pDstItem->byPosition = req->bySrcPos;//set pDstItem pos to src pos
							pDstItem->charId = req->charId;
						}
					}
					else //else move inside guild bank <-> guild bank
					{
						g_pItemRepository->UpdatePlace(req->srcItemID, req->byDstPlace, req->byDstPos);
						pSrcItem->byPlace = req->byDstPlace; //set pSrcItem place to dest place
						pSrcItem->byPosition = req->byDstPos; //set pSrcItem pos to dest pos

						if (pDstItem) //if dest item exist then move dest item to source item place
						{
							g_pItemRepository->UpdatePlace(req->dstItemId, req->bySrcPlace, req->bySrcPos);
							pDstItem->byPlace = req->bySrcPlace;//set pDstItem place to src place
							pDstItem->byPosition = req->bySrcPos;//set pDstItem pos to src pos
						}
					}
				}
				else if (req->byDstPlace >= CONTAINER_TYPE_INVEN_FIRST && req->byDstPlace <= CONTAINER_TYPE_INVEN_LAST) //check if moving item into inventory
				{
					if (IsGuildContainer(req->bySrcPlace)) //if move from guild bank into inventory
					{
						g_pItemRepository->MoveFromGuildBankToOwner(req->srcItemID, req->charId, req->byDstPlace, req->byDstPos);
						pPlayer->InsertItem(pSrcItem); //insert to inventory
						pBank->m_mapItems.erase(req->srcItemID); //remove from guild bank
						pSrcItem->byPlace = req->byDstPlace; //set srcitem place to dest place
						pSrcItem->byPosition = req->byDstPos; //set srcitem pos to dest pos
						pSrcItem->charId = req->charId;

						//if dest item exist then move destitem from inventory to guild
						if (pDstItem)
						{
							//src item to guild container
							g_pItemRepository->MoveToGuildBank(req->srcItemID, req->bySrcPlace, req->bySrcPos, req->guildID);
							pPlayer->EraseItem(req->dstItemId); //remove from inventory
							pBank->m_mapItems.insert(std::make_pair(req->dstItemId, pDstItem)); //insert to guild bank
							pDstItem->byPlace = req->bySrcPlace; //set pDstItem place to src place
							pDstItem->byPosition = req->bySrcPos; //set pDstItem pos to src pos
						}
					}
					else res->wResultCode = GAME_FAIL;
				}
				else res->wResultCode = GAME_FAIL;

			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	res->charID = req->charId;
	res->hSrcItem = req->hSrcItem;
	res->hDstItem = req->hDstItem;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->byDstPlace = req->byDstPlace;
	res->byDstPos = req->byDstPos;
	res->bRestrictUpdate = req->bRestrictUpdate;
	res->bySrcRestrictState = req->bySrcRestrictState;
	res->byDestRestrictState = req->byDestRestrictState;
	packet.SetPacketLen(sizeof(sQG_GUILD_BANK_MOVE_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvGuildBankMoveStackReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_GUILD_BANK_MOVE_STACK_REQ * req = (sGQ_GUILD_BANK_MOVE_STACK_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_GUILD_BANK_MOVE_STACK_RES));
	sQG_GUILD_BANK_MOVE_STACK_RES * res = (sQG_GUILD_BANK_MOVE_STACK_RES *)packet.GetPacketData();
	res->wOpCode = QG_GUILD_BANK_MOVE_STACK_RES;
	res->handle = req->handle;
	res->wResultCode = GAME_SUCCESS;
	res->byDstPlace = req->byDstPlace;
	res->byDstPos = req->byDstPos;
	res->bySrcPlace = req->bySrcPlace;
	res->bySrcPos = req->bySrcPos;
	res->byStackCount1 = req->byStackCount1;
	res->byStackCount2 = req->byStackCount2;
	res->charID = req->charId;
	res->hDstItem = req->hDstItem;
	res->hSrcItem = req->hSrcItem;
	res->splitItemId = INVALID_ITEMID;
	res->hNpcHandle = req->hNpcHandle;

	if (CPlayerCache* pPlayer = g_pPlayerCache->GetCharacter(req->charId))
	{
		if (sGUILD_BANK* pBank = g_pGuild->GetGuildBank(req->guildID))
		{
			if (IsGuildContainer(req->byDstPlace)) //check if stacking into guild bank
			{
				if (IsInvenContainer(req->bySrcPlace)) //check if stack from inventory into guild bank
				{
					if (sITEM_DATA* pSrcItem = pPlayer->GetItemData(req->srcItemID))
					{
						if (req->hDstItem == INVALID_HOBJECT)	//unstack item -> create new item, update stackcount from src item
						{
							//update pSrcItem stack count from
							g_pItemRepository->UpdateCount(req->srcItemID, req->byStackCount1);
							pSrcItem->byStackcount = req->byStackCount1;

							//create pDstItem and insert into guild
							sITEM_DATA* pDstItem = new sITEM_DATA;
							memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
							pDstItem->charId = req->charId;
							pDstItem->byStackcount = req->byStackCount2;
							pDstItem->byPlace = req->byDstPlace;
							pDstItem->byPosition = req->byDstPos;

							res->splitItemId = g_pItemManager->CreateGuildItem(*pDstItem, req->guildID);

							pDstItem->itemId = res->splitItemId;
							pBank->m_mapItems.insert(std::make_pair(pDstItem->itemId, pDstItem));
						}
						else //update stack count
						{
							if (sITEM_DATA* pDstItem = pDstItem = pBank->GetItemData(req->dstItemID))
							{
								//delete/update src item..
								if (req->byStackCount1 == 0)
								{
									pPlayer->EraseItem(req->srcItemID);
									delete pSrcItem;
									g_pItemRepository->DeleteById(req->srcItemID); //delete source item
								}
								else
								{
									pSrcItem->byStackcount = req->byStackCount1;
									g_pItemRepository->UpdateCount(req->srcItemID, req->byStackCount1);//update stack count from source item
								}

								pDstItem->byStackcount = req->byStackCount2;
								g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);	//update stack count from dest item
							}
							else res->wResultCode = QUERY_FAIL;
						}
					}
					else res->wResultCode = QUERY_FAIL;
				}
				else //else stack from guild bank to guild bank
				{
					if (sITEM_DATA* pSrcItem = pBank->GetItemData(req->srcItemID))
					{
						if (req->hDstItem == INVALID_HOBJECT)	//unstack item -> create new item, update stackcount from src item
						{
							//update pSrcItem stack count from
							g_pItemRepository->UpdateCount(req->srcItemID, req->byStackCount1);
							pSrcItem->byStackcount = req->byStackCount1;

							//create pDstItem and insert into guild
							sITEM_DATA* pDstItem = new sITEM_DATA;
							memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
							pDstItem->charId = req->charId;
							pDstItem->byStackcount = req->byStackCount2;
							pDstItem->byPlace = req->byDstPlace;
							pDstItem->byPosition = req->byDstPos;

							res->splitItemId = g_pItemManager->CreateGuildItem(*pDstItem, req->guildID);

							pDstItem->itemId = res->splitItemId;
							pBank->m_mapItems.insert(std::make_pair(pDstItem->itemId, pDstItem));
						}
						else //update stack count
						{
							if (sITEM_DATA* pDstItem = pDstItem = pBank->GetItemData(req->dstItemID))
							{
								//delete/update src item..
								if (req->byStackCount1 == 0)
								{
									pBank->m_mapItems.erase(req->srcItemID);
									delete pSrcItem;
									g_pItemRepository->DeleteById(req->srcItemID); //delete source item
								}
								else
								{
									pSrcItem->byStackcount = req->byStackCount1;
									g_pItemRepository->UpdateCount(req->srcItemID, req->byStackCount1);//update stack count from source item
								}

								pDstItem->byStackcount = req->byStackCount2;
								g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);	//update stack count from dest item
							}
							else res->wResultCode = QUERY_FAIL;
						}
					}
					else res->wResultCode = QUERY_FAIL;
				}
			}
			else if (IsInvenContainer(req->byDstPlace)) //check if stacking from guild into inventory
			{
				if (IsGuildContainer(req->bySrcPlace)) //check if stacking from guild bank to inventory
				{
					if (sITEM_DATA* pSrcItem = pBank->GetItemData(req->srcItemID))
					{
						if (req->hDstItem == INVALID_HOBJECT)	//unstack item -> create new item, update stackcount from src item
						{
							//update pSrcItem stack count from
							g_pItemRepository->UpdateCount(req->srcItemID, req->byStackCount1);
							pSrcItem->byStackcount = req->byStackCount1;

							//create pDstItem and insert into inventory
							sITEM_DATA* pDstItem = new sITEM_DATA;
							memcpy(pDstItem, pSrcItem, sizeof(sITEM_DATA));
							pDstItem->charId = req->charId;
							pDstItem->byStackcount = req->byStackCount2;
							pDstItem->byPlace = req->byDstPlace;
							pDstItem->byPosition = req->byDstPos;

							res->splitItemId = g_pItemManager->CreateItem(*pDstItem);

							pDstItem->itemId = res->splitItemId;
							pPlayer->InsertItem(pDstItem);
						}
						else //update stack count
						{
							if (sITEM_DATA* pDstItem = pDstItem = pPlayer->GetItemData(req->dstItemID))
							{
								//delete/update src item..
								if (req->byStackCount1 == 0)
								{
									pBank->m_mapItems.erase(req->srcItemID);
									delete pSrcItem;
									g_pItemRepository->DeleteById(req->srcItemID); //delete source item
								}
								else
								{
									pSrcItem->byStackcount = req->byStackCount1;
									g_pItemRepository->UpdateCount(req->srcItemID, req->byStackCount1);//update stack count from source item
								}

								pDstItem->byStackcount = req->byStackCount2;
								g_pItemRepository->UpdateCount(req->dstItemID, req->byStackCount2);	//update stack count from dest item
							}
							else res->wResultCode = QUERY_FAIL;
						}
					}
					else res->wResultCode = QUERY_FAIL;
				}
				else res->wResultCode = GAME_FAIL;
			}
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_GUILD_BANK_MOVE_STACK_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvGuildBankZeniReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_GUILD_BANK_ZENNY_REQ * req = (sGQ_GUILD_BANK_ZENNY_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_GUILD_BANK_ZENNY_RES));
	sQG_GUILD_BANK_ZENNY_RES * res = (sQG_GUILD_BANK_ZENNY_RES *)packet.GetPacketData();
	res->wOpCode = QG_GUILD_BANK_ZENNY_RES;
	res->handle = req->handle;
	res->charID = req->charId;
	res->npchandle = req->npchandle;
	res->wResultCode = GAME_SUCCESS;
	res->dwZenny = req->dwZenny;
	res->bIsSave = req->bIsSave;

	if (CPlayerCache* pPlayer = g_pPlayerCache->GetCharacter(req->charId))
	{
		if (sGUILD_BANK* pBank = g_pGuild->GetGuildBank(req->guildID))
		{
			if (req->bIsSave) //check if move item to guild bank
			{
				if (pPlayer->GetZeni() >= req->dwZenny) //check if player has enough zeni
				{
					pPlayer->SetZeni(pPlayer->GetZeni() - req->dwZenny);
					pBank->dwZeni = UnsignedSafeIncrease<DWORD>(pBank->dwZeni, req->dwZenny);

					g_pGuildRepository->UpdateZeni(pBank->dwZeni, req->guildID);
					g_pCharacterRepository->UpdateMoney(req->charId, pPlayer->GetZeni());
				}
				else res->wResultCode = QUERY_FAIL;
			}
			else
			{
				if (pBank->dwZeni >= req->dwZenny) //check if enough zeni in bank
				{
					pBank->dwZeni -= req->dwZenny;
					pPlayer->SetZeni(UnsignedSafeIncrease<DWORD>(pPlayer->GetZeni(), req->dwZenny));

					g_pGuildRepository->UpdateZeni(pBank->dwZeni, req->guildID);
					g_pCharacterRepository->UpdateMoney(req->charId, pPlayer->GetZeni());
				}
				else res->wResultCode = QUERY_FAIL;
			}
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_GUILD_BANK_ZENNY_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvBudokaiDataReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_DATA_REQ * req = (sGQ_BUDOKAI_DATA_REQ*)pPacket->GetPacketData();

	UNREFERENCED_PARAMETER(req);
}

void CGameServerSession::RecvUpdateStateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_UPDATE_STATE_REQ * req = (sGQ_BUDOKAI_UPDATE_STATE_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->UpdateState(req->sStateData);
}

void CGameServerSession::RecvUpdateMatchStateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_UPDATE_MATCH_STATE_REQ * req = (sGQ_BUDOKAI_UPDATE_MATCH_STATE_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->UpdateMatchState(req->byMatchType, req->sStateData);
}

void CGameServerSession::RecvBudokaiJoinIndividualReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_JOIN_INDIVIDUAL_REQ * req = (sGQ_BUDOKAI_JOIN_INDIVIDUAL_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->JoinIndividual(req->handle, req->charId, req->fPoint, GetHandle());
}

void CGameServerSession::RecvBudokaiLeaveIndividualReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_LEAVE_INDIVIDUAL_REQ * req = (sGQ_BUDOKAI_LEAVE_INDIVIDUAL_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->LeaveIndividual(req->handle, req->charId, GetHandle());
}

void CGameServerSession::RecvBudokaiTournamentIndividualAddEntryListReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ * req = (sGQ_BUDOKAI_TOURNAMENT_INDIVIDUAL_ADD_ENTRY_LIST_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->TournamentIndividualAddEntry(req->wJoinId, req->byMatchIndex, req->byWinnerJoinResult, req->byLoserResultCondition, req->byLoserJoinState);
}

void CGameServerSession::RecvBudokaiJoinTeamReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_JOIN_TEAM_REQ * req = (sGQ_BUDOKAI_JOIN_TEAM_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->JoinTeam(req->handle, req->charId, req->wszTeamName, req->byMemberCount, req->aMembers, req->fPoint, req->aTeamInfo, GetHandle());
}

void CGameServerSession::RecvBudokaiLeaveTeamReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_LEAVE_TEAM_REQ * req = (sGQ_BUDOKAI_LEAVE_TEAM_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->LeaveLeam(req->handle, req->charId, GetHandle());
}

void CGameServerSession::RecvBudokaiTournamentTeamAddEntryListReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ * req = (sGQ_BUDOKAI_TOURNAMENT_TEAM_ADD_ENTRY_LIST_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->TournamentTeamAddEntry(req->wJoinId, req->byMatchIndex, req->byWinnerJoinResult, req->byLoserResultCondition, req->byLoserJoinState);
}

void CGameServerSession::RecvBudokaiHistoryWriteReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_HISTORY_WRITE_REQ * req = (sGQ_BUDOKAI_HISTORY_WRITE_REQ*)pPacket->GetPacketData();

	g_pBudokaiManager->HistoryWrite(req->byBudokaiType, req->byMatchType);
}


void CGameServerSession::RecvShopEventItemBuyReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_SHOP_EVENTITEM_BUY_REQ * req = (sGQ_SHOP_EVENTITEM_BUY_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_SHOP_EVENTITEM_BUY_RES));
	sQG_SHOP_EVENTITEM_BUY_RES * res = (sQG_SHOP_EVENTITEM_BUY_RES *)packet.GetPacketData();
	res->wOpCode = QG_SHOP_EVENTITEM_BUY_RES;
	res->handle = req->handle;
	res->wResultCode = GAME_SUCCESS;
	res->byBuyCount = req->byBuyCount;
	res->byDeleteItemCount = req->byDeleteItemCount;
	res->charId = req->charId;
	res->dwMudosaPoint = req->dwMudosaPoint;
	res->hNpchandle = req->hNpchandle;
	memcpy(res->sInven, req->sInven, sizeof(req->sInven));
	memcpy(res->asDeleteItem, req->asDeleteItem, sizeof(req->asDeleteItem));

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->GetMudusaPoints() >= req->dwMudosaPoint)
		{
			for (BYTE i = 0; i < req->byDeleteItemCount; i++)
			{
				if (pCache->RemoveItem(req->asDeleteItem[i].itemId))
					g_pItemRepository->DeleteById(req->asDeleteItem[i].itemId);
				else
				{
					res->wResultCode = GAME_FAIL;
					ERR_LOG(LOG_SYSTEM, "ERROR: Player %u could not find required itemID %I64u to delete..", req->charId, req->asDeleteItem[i].itemId);
					break;
				}
			}

			if (res->wResultCode == GAME_SUCCESS)
			{
				if (req->byUpdateCount > 0)
				{
					for (int i = 0; i < req->byUpdateCount; i++)
					{
						if (sITEM_DATA* pData = pCache->GetItemData(req->aUpdateItem[i].nItemID))
						{
							pData->byStackcount = req->aUpdateItem[i].byStack;
							g_pItemRepository->UpdateCount(req->aUpdateItem[i].nItemID, req->aUpdateItem[i].byStack);
						}
						else
						{
							res->wResultCode = GAME_FAIL;
							ERR_LOG(LOG_SYSTEM, "ERROR: Player %u could not find required itemID %I64u to delete..", req->charId, req->asDeleteItem[i].itemId);
							break;
						}
					}
				}

				if (res->wResultCode == GAME_SUCCESS)
				{
					for (BYTE i = 0; i < req->byBuyCount; i++)
					{
						res->itemID[i] = g_pItemManager->CreateItem(req->sInven[i], req->charId);

						pCache->AddItem(req->sInven[i], res->itemID[i]);
					}

					pCache->SetMudusaPoints(UnsignedSafeDecrease<DWORD>(pCache->GetMudusaPoints(), req->dwMudosaPoint));
					g_pCharacterRepository->UpdateMudosaPoint(pCache->GetMudusaPoints(), req->charId);
				}
			}
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_SHOP_EVENTITEM_BUY_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvShopGambleBuyReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_SHOP_GAMBLE_BUY_REQ * req = (sGQ_SHOP_GAMBLE_BUY_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_SHOP_GAMBLE_BUY_RES));
	sQG_SHOP_GAMBLE_BUY_RES * res = (sQG_SHOP_GAMBLE_BUY_RES *)packet.GetPacketData();
	res->wOpCode = QG_SHOP_GAMBLE_BUY_RES;
	res->handle = req->handle;
	res->wResultCode = GAME_SUCCESS;
	memcpy(&res->sInven, &req->sInven, sizeof(sITEM_DATA));

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		if (req->bIsZeni)
		{
			if (pPlayerCache->GetZeni() >= req->dwPoint)
			{
				res->sInven.itemId = g_pItemManager->CreateItem(req->sInven);

				pPlayerCache->SetZeni(pPlayerCache->GetZeni() - req->dwPoint);
				g_pCharacterRepository->UpdateMoney(req->charId, pPlayerCache->GetZeni());

				pPlayerCache->AddItem(res->sInven);
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else
		{
			if (pPlayerCache->GetZeni() >= ZENI_GAMBLE_FEE)
			{
				if (pPlayerCache->GetMudusaPoints() >= req->dwPoint)
				{
					res->sInven.itemId = g_pItemManager->CreateItem(req->sInven);

					pPlayerCache->SetMudusaPoints(pPlayerCache->GetMudusaPoints() - req->dwPoint);
					pPlayerCache->SetZeni(pPlayerCache->GetZeni() - ZENI_GAMBLE_FEE);
					g_pCharacterRepository->UpdateMoneyAndMudosa(pPlayerCache->GetZeni(), pPlayerCache->GetMudusaPoints(), req->charId);

					pPlayerCache->AddItem(res->sInven);
				}
				else res->wResultCode = QUERY_FAIL;
			}
			else res->wResultCode = QUERY_FAIL;
		}
	}
	else res->wResultCode = QUERY_FAIL;

	
	res->hNpchandle = req->hNpchandle;
	res->charId = req->charId;
	res->dwMudosaPoint = req->dwPoint;
	packet.SetPacketLen(sizeof(sQG_SHOP_GAMBLE_BUY_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvUpdateMudosaPointReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_BUDOKAI_UPDATE_MUDOSA_POINT_REQ * req = (sGQ_BUDOKAI_UPDATE_MUDOSA_POINT_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->SetMudusaPoints(req->dwMudosaPoint);
		g_pCharacterRepository->UpdateMudosaPoint(req->dwMudosaPoint, req->charId);
	}
}


void CGameServerSession::RecvSkillInitReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_SKILL_INIT_REQ * req = (sGQ_SKILL_INIT_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_SKILL_INIT_RES));
	sQG_SKILL_INIT_RES * res = (sQG_SKILL_INIT_RES *)packet.GetPacketData();
	res->wOpCode = QG_SKILL_INIT_RES;
	res->handle = req->handle;
	res->charId = req->charId;
	res->bySkillResetMethod = req->bySkillResetMethod;
	res->dwSP = req->dwSP;
	res->wResultCode = GAME_SUCCESS;
	res->itemId = req->itemId;
	res->dwZeni = req->dwZenny;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (req->bySkillResetMethod == 0) // only use zeni
		{
			if (pCache->GetZeni() >= req->dwZenny)
			{
				if (pCache->CanInitSkills())
				{
					pCache->SetZeni(pCache->GetZeni() - req->dwZenny);
				}
				else res->wResultCode = GAME_FAIL;
			}
			else res->wResultCode = GAME_ZENNY_NOT_ENOUGH;
		}
		else if (req->bySkillResetMethod == 1)
		{
			if (sITEM_DATA* pItem = pCache->GetItemData(req->itemId))
			{
				pItem->byStackcount = UnsignedSafeDecrease<BYTE>(pItem->byStackcount, 1);

				if (pItem->byStackcount == 0)
				{
					pCache->RemoveItem(req->itemId);
					g_pItemRepository->DeleteById(req->itemId);
				}
				else
				{
					g_pItemRepository->UpdateCount(req->itemId, pItem->byStackcount);
				}
			}
			else res->wResultCode = GAME_NEEDITEM_NOT_FOUND;
		}
		else if (req->bySkillResetMethod == 2)
		{

		}
		else
		{
			pCache->InitSkills();
			pCache->SetSkillPoints(req->dwSP);

			g_pSkillRepository->DeleteSkills(req->charId);
			g_pCharacterRepository->UpdateSpPoint(req->dwSP, req->charId);

			return;
		}
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQG_SKILL_INIT_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvSkillOneResetReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SKILL_ONE_RESET_REQ * req = (sGQ_SKILL_ONE_RESET_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pItem = pCache->GetItemData(req->itemId))
		{
			pItem->byStackcount -= 1;

			if (pItem->byStackcount == 0)
			{
				pCache->RemoveItem(req->itemId);
				g_pItemRepository->DeleteById(req->itemId);
			}
			else
			{
				g_pItemRepository->UpdateCount(req->itemId, pItem->byStackcount);
			}

			pCache->SetSkillPoints(req->dwAddSP + pCache->GetSkillPoints());
			g_pCharacterRepository->UpdateSpPointAdd(req->dwAddSP, req->charId);

			if (req->newSkillTblidx == 0)
			{
				if (pCache->DeleteSkill(req->skillIndex))
					g_pSkillRepository->DeleteSkillBySlot(req->skillIndex, req->charId);
			}
			else
			{
				if (sSKILL_DATA* pData = pCache->GetSkillData(req->skillIndex))
				{
					pData->skillId = req->newSkillTblidx;
					pData->bIsRpBonusAuto = req->bRpBonusAuto;
					pData->byRpBonusType = req->byRpBonusType;

					g_pSkillRepository->UpdateSkillIdAndBonus(req->newSkillTblidx, req->bRpBonusAuto, req->byRpBonusType, req->skillIndex, req->charId);
				}
			}
		}
	}
}


void CGameServerSession::RecvRecipeRegisterReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_RECIPE_REG_REQ * req = (sGQ_RECIPE_REG_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->RegisterRecipe(req->recipeTblidx, req->byRecipeType);
	}

	g_pRecipeRepository->InsertRecipe(req->charId, req->recipeTblidx, req->byRecipeType);
}


void CGameServerSession::RecvHoipoiItemMakeReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_HOIPOIMIX_ITEM_MAKE_REQ * req = (sGQ_HOIPOIMIX_ITEM_MAKE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_HOIPOIMIX_ITEM_MAKE_RES));
	sQG_HOIPOIMIX_ITEM_MAKE_RES * res = (sQG_HOIPOIMIX_ITEM_MAKE_RES *)packet.GetPacketData();
	res->wOpCode = QG_HOIPOIMIX_ITEM_MAKE_RES;
	res->wResultCode = GAME_SUCCESS;
	res->bIsNew = req->bIsNew;
	res->byMixLevel = req->byMixLevel;
	res->charId = req->charId;
	res->dwMixExp = req->dwMixExp;
	res->dwSpendZenny = req->dwSpendZenny;
	res->handle = req->handle;
	res->objHandle = req->objHandle;
	res->recipeTblidx = req->recipeTblidx;
	memcpy(&res->sCreateData, &req->sCreateData, sizeof(sITEM_DATA));
	res->dwExpGained = req->dwExpGained;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->GetZeni() >= req->dwSpendZenny)
		{
			//delete / update required items
			for (int i = 0; i < req->byCount; i++)
			{
				sITEM_DATA* pItem = pCache->GetItemData(req->asData[i].nItemID);
				if (pItem)
				{
					if (req->asData[i].byStack == 0)
					{
						pCache->RemoveItem(req->asData[i].nItemID);
						g_pItemRepository->DeleteById(pItem->itemId);
					}
					else
					{
						pItem->byStackcount = req->asData[i].byStack;
						g_pItemRepository->UpdateCount(pItem->itemId, pItem->byStackcount);
					}
				}
			}

			pCache->SetHoipoiMix(req->byMixLevel, req->dwMixExp);
			pCache->SetZeni(UnsignedSafeDecrease<DWORD>(pCache->GetZeni(), req->dwSpendZenny));
			g_pCharacterRepository->UpdateMoneyAndHoipoi(pCache->GetZeni(), req->byMixLevel, req->dwMixExp, req->charId);



			//create new / update current item
			if (req->bIsNew)
			{
				res->sCreateData.itemId = g_pItemManager->CreateItem(req->sCreateData);

				pCache->AddItem(res->sCreateData);
			}
			else
			{
				sITEM_DATA* pItem = pCache->GetItemData(req->sCreateData.itemId);
				if (pItem)
				{
					pItem->byStackcount = req->sCreateData.byStackcount;
					g_pItemRepository->UpdateCount(pItem->itemId, pItem->byStackcount);
				}
			}
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	
	packet.SetPacketLen(sizeof(sQG_HOIPOIMIX_ITEM_MAKE_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvRunTimePcDataSaveNfy(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_RUNTIME_PCDATA_SAVE_NFY * req = (sGQ_RUNTIME_PCDATA_SAVE_NFY*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		pCache->StoreRunTimeData(req->dwExp, req->worldTblidx, req->worldId, req->vLoc, req->vDir);

		g_pCharacterRepository->SaveRunTimeData(req->dwExp, req->vLoc.x, req->vLoc.y, req->vLoc.z, req->vDir.x, req->vDir.y, req->vDir.z, req->worldId, req->worldTblidx, req->dwAddPlayTime, req->charID);
	}
}


void CGameServerSession::RecvCashItemRefreshReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_CASHITEM_HLSHOP_REFRESH_REQ * req = (sGQ_CASHITEM_HLSHOP_REFRESH_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sGQ_CASHITEM_HLSHOP_REFRESH_RES));
	sGQ_CASHITEM_HLSHOP_REFRESH_RES * res = (sGQ_CASHITEM_HLSHOP_REFRESH_RES *)packet.GetPacketData();
	res->wOpCode = GQ_CASHITEM_HLSHOP_REFRESH_RES;
	res->wResultCode = GAME_SUCCESS;
	res->dwRemainAmount = 0;
	res->accountId = req->accountId;
	res->charId = req->charId;
	res->handle = req->handle;

	CAccountCache* pCache = g_pPlayerCache->GetAccount(req->accountId);
	if (pCache)
	{
		smart_ptr<QueryResult> spQuery = g_pAccountRepository->GetMallpoints(req->accountId);
		if (spQuery)
		{
			Field* f = spQuery->Fetch();

			pCache->SetCash(f[0].GetDWORD());

			res->dwRemainAmount = pCache->GetCash();
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sGQ_CASHITEM_HLSHOP_REFRESH_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvCashItemInfoReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_CASHITEM_INFO_REQ * req = (sGQ_CASHITEM_INFO_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_CASHITEM_INFO_RES));
	sQG_CASHITEM_INFO_RES * res = (sQG_CASHITEM_INFO_RES *)packet.GetPacketData();
	res->wOpCode = QG_CASHITEM_INFO_RES;
	res->wResultCode = GAME_SUCCESS;
	res->charId = req->charId;
	res->handle = req->handle;

	CAccountCache* pCache = g_pPlayerCache->GetAccount(req->accountId);
	if (pCache)
	{
		pCache->SetSession(GetHandle());
		pCache->SendCashshopLoadRes(req->charId, req->handle);
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_CASHITEM_INFO_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvCashitemMoveReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_CASHITEM_MOVE_REQ * req = (sGQ_CASHITEM_MOVE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_CASHITEM_MOVE_RES));
	sQG_CASHITEM_MOVE_RES * res = (sQG_CASHITEM_MOVE_RES *)packet.GetPacketData();
	res->wOpCode = QG_CASHITEM_MOVE_RES;
	res->wResultCode = GAME_SUCCESS;
	res->charId = req->charId;
	res->handle = req->handle;
	res->qwProductId = req->qwProductId;

	CAccountCache* pCache = g_pPlayerCache->GetAccount(req->accountId);
	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache && pPlayerCache)
	{
		if (pCache->RemoveCashItem(req->qwProductId))
		{
			memcpy(&res->sData, &req->sData, sizeof(req->sData));

			res->itemId = g_pItemManager->CreateItem(req->sData, req->charId);

			pPlayerCache->AddItem(req->sData, res->itemId);

			g_pCashShopRepository->UpdateStorageItemMoved(res->itemId, req->qwProductId);
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_CASHITEM_MOVE_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvCashitemUnpackReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_CASHITEM_UNPACK_REQ * req = (sGQ_CASHITEM_UNPACK_REQ*)pPacket->GetPacketData();

	UNREFERENCED_PARAMETER(app);
	UNREFERENCED_PARAMETER(req);
}


void CGameServerSession::RecvCashitemBuyReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_CASHITEM_BUY_REQ * req = (sGQ_CASHITEM_BUY_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_CASHITEM_BUY_RES));
	sQG_CASHITEM_BUY_RES * res = (sQG_CASHITEM_BUY_RES *)packet.GetPacketData();
	res->wOpCode = QG_CASHITEM_BUY_RES;
	res->wResultCode = GAME_SUCCESS;
	res->handle = req->handle;
	res->byCount = req->byCount;
	res->characterId = req->characterId;
	res->dwPrice = req->dwPrice;
	res->HLSitemTblidx = req->HLSitemTblidx;
	res->accountId = req->accountId;

	CAccountCache* pCache = g_pPlayerCache->GetAccount(req->accountId);
	if (pCache)
	{
		if (pCache->GetCash() >= req->dwPrice)
		{
			SYSTEMTIME ti;
			GetLocalTime(&ti);

			QWORD productId = g_pCashshopManager->AcquireProductId();

			g_pCashShopRepository->InsertStorageItem(productId, req->accountId, req->HLSitemTblidx, req->byCount, ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond, ti.wMilliseconds, req->accountId, req->dwPrice);

			pCache->SetCash(pCache->GetCash() - req->dwPrice);
			g_pAccountRepository->UpdateMallpointsDeductWait(req->dwPrice, req->accountId);

			res->qwProductId = productId;
			res->dwRemainAmount = pCache->GetCash();

			//create & add item to cache
			sCASHITEM_BRIEF* pBrief = new sCASHITEM_BRIEF;
			pBrief->byStackCount = req->byCount;
			pBrief->HLSitemTblidx = req->HLSitemTblidx;
			pBrief->qwProductId = productId;
			pBrief->tRegTime.day = (BYTE)ti.wDay;
			pBrief->tRegTime.hour = (BYTE)ti.wHour;
			pBrief->tRegTime.minute = (BYTE)ti.wMinute;
			pBrief->tRegTime.month = (BYTE)ti.wMonth;
			pBrief->tRegTime.second = (BYTE)ti.wSecond;
			pBrief->tRegTime.year = ti.wYear;
			pCache->InsertCashItem(pBrief);

			memcpy(&res->tRegTime, &pBrief->tRegTime, sizeof(sDBO_TIME));
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	
	packet.SetPacketLen(sizeof(sQG_CASHITEM_BUY_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvPcDataLoadReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_PC_DATA_LOAD_REQ * req = (sGQ_PC_DATA_LOAD_REQ*)pPacket->GetPacketData();

	CAccountCache* pAccountCache = g_pPlayerCache->GetAccount(req->accountId);
	if(pAccountCache)
		pAccountCache->SetSession(GetHandle());

	g_pAccountRepository->CheckAccountStatusAsync(this, req->charId, req->accountId);
}


void CGameServerSession::RecvQuickTeleportInfoLoadReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_QUICK_TELEPORT_INFO_LOAD_REQ * req = (sGQ_QUICK_TELEPORT_INFO_LOAD_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_QUICK_TELEPORT_INFO_LOAD_RES));
	sQG_QUICK_TELEPORT_INFO_LOAD_RES * res = (sQG_QUICK_TELEPORT_INFO_LOAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_QUICK_TELEPORT_INFO_LOAD_RES;
	res->charId = req->charId;
	res->wResultCode = GAME_SUCCESS;
	res->byCount = 0;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->IsQuickTeleportLoaded() == false)
		{
			smart_ptr<QueryResult> result = g_pQuickSlotRepository->LoadQuickTeleports(req->charId);

			if (result)
			{
				do
				{
					Field* f = result->Fetch();

					res->asData[res->byCount].bySlotNum = f[1].GetBYTE();
					res->asData[res->byCount].worldTblidx = f[2].GetUInt32();
					res->asData[res->byCount].vLoc.x = f[3].GetFloat();
					res->asData[res->byCount].vLoc.y = f[4].GetFloat();
					res->asData[res->byCount].vLoc.z = f[5].GetFloat();
					res->asData[res->byCount].mapNameTblidx = f[6].GetUInt32();
					res->asData[res->byCount].tSaveTime.day = f[7].GetBYTE();
					res->asData[res->byCount].tSaveTime.hour = f[8].GetBYTE();
					res->asData[res->byCount].tSaveTime.minute = f[9].GetBYTE();
					res->asData[res->byCount].tSaveTime.month = f[10].GetBYTE();
					res->asData[res->byCount].tSaveTime.second = f[11].GetBYTE();
					res->asData[res->byCount].tSaveTime.year = f[12].GetWORD();

					pCache->AddQuickTeleport(res->asData[res->byCount].bySlotNum, res->asData[res->byCount]);

					res->byCount += 1;
				} while (result->NextRow());

				pCache->SetQuickTeleportLoaded(true);
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else
		{
			res->byCount = 0;
			pCache->FillQuickTeleportData(res->asData, res->byCount);
		}
	}
	else
	{
		res->wResultCode = QUERY_FAIL;
	}

	
	packet.SetPacketLen(sizeof(sQG_QUICK_TELEPORT_INFO_LOAD_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvQuickTeleportUpdateReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_QUICK_TELEPORT_UPDATE_REQ * req = (sGQ_QUICK_TELEPORT_UPDATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		pCache->UpdateQuickTeleport(req->asData.bySlotNum, req->asData);
	}

	g_pQuickSlotRepository->UpsertQuickTeleport(req->charId, req->asData.bySlotNum, req->asData.worldTblidx, req->asData.vLoc.x, req->asData.vLoc.y, req->asData.vLoc.z, req->asData.mapNameTblidx,
		req->asData.tSaveTime.day, req->asData.tSaveTime.hour, req->asData.tSaveTime.minute, req->asData.tSaveTime.month, req->asData.tSaveTime.second, req->asData.tSaveTime.year);
}


void CGameServerSession::RecvQuickTeleportDelReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_QUICK_TELEPORT_DEL_REQ * req = (sGQ_QUICK_TELEPORT_DEL_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if(pCache->DeleteQuickTeleport(req->bySlot))
			g_pQuickSlotRepository->DeleteQuickTeleportBySlot(req->charId, req->bySlot);
	}
}


void CGameServerSession::RecvSaveItemCoolTimeDataReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SAVE_ITEM_COOL_TIME_DATA_REQ * req = (sGQ_SAVE_ITEM_COOL_TIME_DATA_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		// clear them
		g_pItemRepository->ClearItemCoolTimes(req->charId);

		pCache->ClearItemCoolTime();

		//insert
		for (int i = 0; i < ITEM_COOL_TIME_GROUP_COUNT; i++)
		{
			if (req->aItemCoolTimeData[i].dwItemCoolTimeRemaining > 3000)
			{
				pCache->UpdateItemCoolTime(req->aItemCoolTimeData[i]);

				g_pItemRepository->InsertItemCd(req->charId, req->aItemCoolTimeData[i].byItemCoolTimeGroupIndex, req->aItemCoolTimeData[i].dwInitialItemCoolTime, req->aItemCoolTimeData[i].dwItemCoolTimeRemaining);
			}
		}
	}
}


void CGameServerSession::RecvAccountBann(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_ACCOUNT_BANN * req = (sGQ_ACCOUNT_BANN*)pPacket->GetPacketData();

	g_pAccountRepository->BanAccount(req->targetAccountID);

	g_pAuditLogRepository->InsertAccountBanLog(req->gmAccountID, req->targetAccountID, req->szReason, req->byDuration);
}

void CGameServerSession::RecvCharTitleSelectReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_CHARTITLE_SELECT_REQ * req = (sGQ_CHARTITLE_SELECT_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->UpdateSelectedTitle(req->charTitle);
	}

	//dont do query here. We do query when player logout
}

void CGameServerSession::RecvCharTitleAddReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_CHARTITLE_ADD_REQ * req = (sGQ_CHARTITLE_ADD_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->UpdateTitle(req->TitleIndexFlag);
	}

	g_pCharacterRepository->InsertTitle(req->charId, req->charTitle);
}

void CGameServerSession::RecvCharTitleDelReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_CHARTITLE_DELETE_REQ* req = (sGQ_CHARTITLE_DELETE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->UpdateTitle(req->TitleIndexFlag);
	}

	g_pCharacterRepository->DeleteTitle(req->charId, req->charTitle);
}


void CGameServerSession::RecvItemStackUpdateReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_ITEM_STACK_UPDATE_REQ * req = (sGQ_ITEM_STACK_UPDATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		if (sITEM_DATA* pItem = pPlayerCache->GetItemData(req->itemId))
		{
			pItem->byStackcount = req->byStackCount;
			g_pItemRepository->UpdateCount(req->itemId, req->byStackCount);
		}
	}
}

void CGameServerSession::RecvSwitchChildAdult(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_SWITCH_CHILD_ADULT * req = (sGQ_SWITCH_CHILD_ADULT*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->SetChildAdult(req->bIsAdult);
	}

	g_pCharacterRepository->UpdateAdult(req->bIsAdult, req->charId);
}


void CGameServerSession::RecvItemChangeAttributeReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_ITEM_CHANGE_ATTRIBUTE_REQ * req = (sGQ_ITEM_CHANGE_ATTRIBUTE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		if (pPlayerCache->GetZeni() >= req->dwZeni)
		{
			pPlayerCache->SetItemBattleAttribute(req->byBattleAttribute, req->itemId);
			pPlayerCache->SetZeni(pPlayerCache->GetZeni() - req->dwZeni);

			g_pItemRepository->UpdateBattleAttribute(req->itemId, req->byBattleAttribute);
			g_pCharacterRepository->UpdateMoneyCharId64(pPlayerCache->GetZeni(), req->charId);
		}
	}
}

void CGameServerSession::RecvItemChangeDurationTimeReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_ITEM_CHANGE_DURATIONTIME_REQ * req = (sGQ_ITEM_CHANGE_DURATIONTIME_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_CHANGE_DURATIONTIME_RES));
	sQG_ITEM_CHANGE_DURATIONTIME_RES * res = (sQG_ITEM_CHANGE_DURATIONTIME_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_CHANGE_DURATIONTIME_RES;
	res->charId = req->charId;
	res->handle = req->handle;
	res->wResultCode = QUERY_FAIL;
	memcpy(&res->sItem, &req->sItem, sizeof(sITEM_DATA));

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pItem = pCache->GetItemData(req->sItem.itemId))
		{
			res->wResultCode = GAME_SUCCESS;

			pItem->nUseEndTime = req->sItem.nUseEndTime;
			g_pItemRepository->UpdateUseEndTime32(req->sItem.itemId, pItem->nUseEndTime);
		}
	}

	packet.SetPacketLen(sizeof(sQG_ITEM_CHANGE_DURATIONTIME_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvShopNetpyItemBuyReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_SHOP_NETPYITEM_BUY_REQ* req = (sGQ_SHOP_NETPYITEM_BUY_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_SHOP_NETPYITEM_BUY_RES));
	sQG_SHOP_NETPYITEM_BUY_RES* res = (sQG_SHOP_NETPYITEM_BUY_RES*)packet.GetPacketData();
	res->wOpCode = QG_SHOP_NETPYITEM_BUY_RES;
	res->charId = req->charId;
	res->handle = req->handle;
	res->wResultCode = GAME_SUCCESS;
	res->byBuyCount = req->byBuyCount;
	res->netpyPoint = req->netpyPoint;
	memcpy(res->sInven, req->sInven, sizeof(req->sInven));

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		for (BYTE i = 0; i < req->byBuyCount; i++)
		{
			res->itemID[i] = g_pItemManager->CreateItem(req->sInven[i], req->charId);

			pCache->AddItem(req->sInven[i], res->itemID[i]);
		}

		pCache->SetNetPy(UnsignedSafeDecrease<DWORD>(pCache->GetNetPy(), req->netpyPoint));
		g_pCharacterRepository->UpdateNetpy(pCache->GetNetPy(), req->charId);
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_SHOP_NETPYITEM_BUY_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvDurationItemBuyReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_DURATION_ITEM_BUY_REQ* req = (sGQ_DURATION_ITEM_BUY_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_DURATION_ITEM_BUY_RES));
	sQG_DURATION_ITEM_BUY_RES* res = (sQG_DURATION_ITEM_BUY_RES*)packet.GetPacketData();
	res->wOpCode = QG_DURATION_ITEM_BUY_RES;
	res->byPayType = req->byPayType;
	res->byPos = req->byPos;
	res->charId = req->charId;
	res->dwPayAmount = req->dwPayAmount;
	res->handle = req->handle;
	res->merchantTblidx = req->merchantTblidx;
	memcpy(&res->sItem, &req->sItem, sizeof(sITEM_DATA));
	res->wResultCode = GAME_SUCCESS;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->GetNetPy() >= req->dwPayAmount)
		{
			if (req->sItem.itemId == INVALID_ITEMID)
			{
				res->sItem.itemId = g_pItemManager->CreateItem(req->sItem);

				pCache->AddItem(res->sItem);
			}
			else
			{
				if (sITEM_DATA* pItem = pCache->GetItemData(req->sItem.itemId))
				{
					pItem->byStackcount = req->sItem.byStackcount;
					g_pItemRepository->UpdateCount(req->sItem.itemId, req->sItem.byStackcount);
				}
				else res->wResultCode = QUERY_FAIL;
			}

			if (res->wResultCode == GAME_SUCCESS)
			{
				pCache->SetNetPy(UnsignedSafeDecrease<NETP>(pCache->GetNetPy(), req->dwPayAmount));
				g_pCharacterRepository->UpdateNetpy(pCache->GetNetPy(), req->charId);
			}
		}
		else res->wResultCode = GAME_NETP_POINT_NOT_ENOUGH;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_DURATION_ITEM_BUY_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvDurationRenewReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_DURATION_RENEW_REQ* req = (sGQ_DURATION_RENEW_REQ*)pPacket->GetPacketData();

	CAccountCache* pAccount = g_pPlayerCache->GetAccount(req->accountId);
	if (pAccount)
	{
		CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
		if (pCache)
		{
			bool bSuccess = true;

			if (req->byPayType == 1) // NetPy Token
			{
				if (pCache->GetNetPy() >= req->dwPayAmount)
				{
					pCache->SetNetPy(pCache->GetNetPy() - req->dwPayAmount);
				}
				else bSuccess = false;
			}
			else if (req->byPayType == 2) // Cash Token
			{
				if (pAccount->GetCash() >= req->dwPayAmount)
				{
					pAccount->SetCash(pAccount->GetCash() - req->dwPayAmount);
				}
				else bSuccess = false;
			}
			else // Zeni
			{
				if (pCache->GetZeni() >= req->dwPayAmount)
				{
					pCache->SetZeni(pCache->GetZeni() - req->dwPayAmount);
				}
				else bSuccess = false;
			}

			if (bSuccess)
			{
				pCache->RenewItemDuration(req->sData.nUseEndTime, req->sData.itemID);

				g_pItemRepository->UpdateUseEndTime64(req->sData.itemID, req->sData.nUseEndTime);

				if (req->byPayType == 1) // NetPy Token
				{
					g_pCharacterRepository->UpdateNetpy(pCache->GetNetPy(), req->charId);
				}
				else if (req->byPayType == 2) // Cash Token
				{
					g_pAccountRepository->UpdateMallpointsWait(pAccount->GetCash(), req->accountId);
				}
				else // Zeni
				{
					g_pCharacterRepository->UpdateMoney(req->charId, pCache->GetZeni());
				}
			}
		}
	}
}


void CGameServerSession::RecvCashitemSendGiftReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_CASHITEM_SEND_GIFT_REQ * req = (sGQ_CASHITEM_SEND_GIFT_REQ*)pPacket->GetPacketData();

	WORD wResultcode = GAME_SUCCESS;
	QWORD productId = INVALID_QWORD;

	CNtlPacket packet2(sizeof(sQG_CASHITEM_SEND_GIFT_RES));
	sQG_CASHITEM_SEND_GIFT_RES * res2 = (sQG_CASHITEM_SEND_GIFT_RES *)packet2.GetPacketData();
	res2->wOpCode = QG_CASHITEM_SEND_GIFT_RES;
	res2->SenderCharId = req->SenderCharId;
	wcscpy_s(res2->wchSenderName, NTL_MAX_SIZE_CHAR_NAME + 1, req->wchSenderName);
	res2->DestAccountId = INVALID_ACCOUNTID;
	wcscpy_s(res2->wchDestName, NTL_MAX_SIZE_CHAR_NAME + 1, req->wchDestName);
	res2->DestServerFarmId = req->DestServerFarmId;
	res2->dwIdxHlsTable = req->dwIdxHlsTable;
	
	CAccountCache* pSenderCache = g_pPlayerCache->GetAccount(req->SenderAccountId);
	if (pSenderCache)
	{
		res2->dwRemainAmount = pSenderCache->GetCash();

		if (pSenderCache->GetCash() >= req->dwPrice)
		{
			smart_ptr<QueryResult> acc_check = g_pCharacterRepository->GetAccountIdByCharName(req->wchDestName); //Chck if account exist
			if (acc_check)
			{
				Field* f_acc_check = acc_check->Fetch();
				if (f_acc_check[0].GetUInt32() != req->SenderAccountId)
				{
					productId = g_pCashshopManager->AcquireProductId();

					if (g_pAccountRepository->UpdateMallpointsDeductWait(req->dwPrice, req->SenderAccountId))
					{
						SYSTEMTIME ti;
						GetLocalTime(&ti);

						g_pCashShopRepository->InsertGiftStorageItem(productId, f_acc_check[0].GetUInt32(), req->dwIdxHlsTable, req->byCount, req->SenderCharId, req->wchSenderName, ti.wYear, ti.wMonth, ti.wDay, ti.wHour, ti.wMinute, ti.wSecond, ti.wMilliseconds, req->SenderAccountId, req->dwPrice);

						pSenderCache->SetCash(pSenderCache->GetCash() - req->dwPrice);
						res2->dwRemainAmount = pSenderCache->GetCash();

						res2->DestAccountId = f_acc_check[0].GetUInt32();

						CAccountCache* pReceiverAccountCache = g_pPlayerCache->GetAccount(res2->DestAccountId);
						if (pReceiverAccountCache)
						{
							//create & add item to cache
							sCASHITEM_BRIEF* pBrief = new sCASHITEM_BRIEF;
							pBrief->byStackCount = req->byCount;
							pBrief->HLSitemTblidx = req->dwIdxHlsTable;
							pBrief->qwProductId = productId;
							pBrief->tRegTime.day = (BYTE)ti.wDay;
							pBrief->tRegTime.hour = (BYTE)ti.wHour;
							pBrief->tRegTime.minute = (BYTE)ti.wMinute;
							pBrief->tRegTime.month = (BYTE)ti.wMonth;
							pBrief->tRegTime.second = (BYTE)ti.wSecond;
							pBrief->tRegTime.year = ti.wYear;
							wcscpy_s(pBrief->wchSenderName, NTL_MAX_SIZE_CHAR_NAME + 1, req->wchSenderName);
							pReceiverAccountCache->InsertCashItem(pBrief);

							memcpy(&res2->tRegTime, &pBrief->tRegTime, sizeof(sDBO_TIME));
						}
					}
					else wResultcode = QUERY_FAIL;
				}
				else wResultcode = CASHITEM_FAIL_CANT_GIFT_MYSELF;
			}
			else wResultcode = CASHITEM_FAIL_CANT_FIND_CHARNAME;
		}
		else wResultcode = QUERY_FAIL;
	}
	else wResultcode = QUERY_FAIL;


	res2->wResultCode = wResultcode;
	res2->qwProductId = productId;
	res2->byCount = req->byCount;
	res2->dwPrice = req->dwPrice;
	packet2.SetPacketLen(sizeof(sQG_CASHITEM_SEND_GIFT_RES));
	app->Send(GetHandle(), &packet2);
}


void CGameServerSession::RecvMascotRegisterReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MASCOT_REGISTER_REQ * req = (sGQ_MASCOT_REGISTER_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->RegisterMascot(req->slotId, req->tblidx, req->byRank, req->dwVP);
		g_pMascotRepository->InsertMascot(req->charId, req->slotId, req->tblidx, req->dwVP, req->dwVP);
	}
}

void CGameServerSession::RecvMascotDeleteReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MASCOT_DELETE_REQ * req = (sGQ_MASCOT_DELETE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->DeleteMascot(req->slotId);
	}

	g_pMascotRepository->DeleteMascotBySlot(req->charId, req->slotId);
}

void CGameServerSession::RecvMascotSkillAddReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MASCOT_SKILL_ADD_REQ * req = (sGQ_MASCOT_SKILL_ADD_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		pPlayerCache->MascotSkillAdd(req->byMascotIndex, req->slotId, req->skillTblidx);
	}

	g_pMascotRepository->UpdateMascotSkill(req->slotId, req->skillTblidx, req->charId, req->byMascotIndex);
}

void CGameServerSession::RecvMascotSkillUpdateReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MASCOT_SKILL_UPDATE_REQ * req = (sGQ_MASCOT_SKILL_UPDATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		if (sITEM_DATA* pItem = pPlayerCache->GetItemData(req->itemId))
		{
			if (req->byItemCount == 0)
			{
				pPlayerCache->RemoveItem(req->itemId);
				g_pItemRepository->DeleteById(req->itemId);
			}
			else
			{
				pItem->byStackcount = req->byItemCount;
				g_pItemRepository->UpdateCount(req->itemId, req->byItemCount);
			}

			pPlayerCache->MascotSkillAdd(req->byMascotIndex, req->slotId, req->skillTblidx);
			g_pMascotRepository->UpdateMascotSkill(req->slotId, req->skillTblidx, req->charId, req->byMascotIndex);
		}
	}
}

void CGameServerSession::RecvMascotSkillUpgradeReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MASCOT_SKILL_UPGRADE_REQ * req = (sGQ_MASCOT_SKILL_UPGRADE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		if (sITEM_DATA* pItem = pPlayerCache->GetItemData(req->itemID))
		{
			if (pItem->byStackcount == 1)
			{
				pPlayerCache->RemoveItem(req->itemID);
				g_pItemRepository->DeleteById(req->itemID);
			}
			else
			{
				pItem->byStackcount -= 1;
				g_pItemRepository->UpdateCount(req->itemID, pItem->byStackcount);
			}

			pPlayerCache->MascotSkillAdd(req->byMascotIndex, req->slotId, req->skillTblidx);
			g_pMascotRepository->UpdateMascotSkill(req->slotId, req->skillTblidx, req->charId, req->byMascotIndex);
		}
	}
}

void CGameServerSession::RecvMascotFusionReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MASCOT_FUSION_REQ * req = (sGQ_MASCOT_FUSION_REQ*)pPacket->GetPacketData();

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pPlayerCache)
	{
		sMASCOT_DATA_EX* pMainMascot = pPlayerCache->GetMascotData(req->byMascotIndex);
		if (pMainMascot)
		{
			sMASCOT_DATA_EX* pOfferMascot = pPlayerCache->GetMascotData(req->byOfferingMascotIndex);
			if (pOfferMascot)
			{
				if (req->byItemPlace != INVALID_BYTE)
				{
					if (sITEM_DATA* pItem = pPlayerCache->GetItemData(req->itemId))
					{
						if (pItem->byStackcount == 1)
						{
							pPlayerCache->RemoveItem(req->itemId);
							g_pItemRepository->DeleteById(req->itemId);
						}
						else
						{
							pItem->byStackcount -= 1;
							g_pItemRepository->UpdateCount(req->itemId, pItem->byStackcount);
						}
					}
					else
					{
						//error.
						ERR_LOG(LOG_USER, "ERROR: Player %u does not have the item %I64u", req->charId, req->itemId);
					}
				}

				//delete offering mascot
				pPlayerCache->EraseMascot(req->byOfferingMascotIndex);
				SAFE_DELETE(pOfferMascot);
				g_pMascotRepository->DeleteMascotBySlot(req->charId, req->byOfferingMascotIndex);

				//update new mascot (if success)
				if (req->bIsSuccess)
				{
					pMainMascot->tblidx = req->nextMascotTblidx;
					pMainMascot->byItemRank = req->byNewRank;
					pMainMascot->dwCurExp = 0;
					pMainMascot->dwMaxVP = req->dwMaxVP;
					pMainMascot->itemTblidx = req->nextMascotTblidx;
					g_pMascotRepository->UpdateMascotFusion(req->nextMascotTblidx, req->dwMaxVP, req->charId, req->byMascotIndex);
				}
			}
		}
	}
}

void CGameServerSession::RecvMascotSaveDataReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_MASCOT_SAVE_DATA_REQ * req = (sGQ_MASCOT_SAVE_DATA_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		for (BYTE i = 0; i < req->byCount; i++)
		{
			if (sMASCOT_DATA_EX* pData = pCache->GetMascotData(req->slotId[i]))
			{
				pData->dwCurExp = req->dwExp[i];
				pData->dwCurVP = req->dwVp[i];
				g_pMascotRepository->UpdateMascotVpExp(pData->dwCurVP, pData->dwCurExp, req->charId, req->slotId[i]);
			}
		}
	}
}

void CGameServerSession::RecvMaterialDisassembleReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_MATERIAL_DISASSEMBLE_REQ * req = (sGQ_MATERIAL_DISASSEMBLE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_MATERIAL_DISASSEMBLE_RES));
	sQG_MATERIAL_DISASSEMBLE_RES * res = (sQG_MATERIAL_DISASSEMBLE_RES *)packet.GetPacketData();
	res->wOpCode = QG_MATERIAL_DISASSEMBLE_RES;
	res->charId = req->charId;
	res->handle = req->handle;
	res->byCount = req->byCount;
	res->wResultCode = GAME_SUCCESS;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		memcpy(&res->sDeleteItem, &req->sDeleteItem, sizeof(sITEM_DELETE_DATA));
		memcpy(res->asCreateItem, req->asCreateItem, sizeof(req->asCreateItem));

		if (pCache->RemoveItem(req->sDeleteItem.itemId)) //remove item and check if success
		{
			g_pItemRepository->DeleteById(req->sDeleteItem.itemId); //query to remove item

			for (int i = 0; i < req->byCount; i++)
			{
				if (req->asCreateItem[i].itemId == INVALID_ITEMID) //check if we have to create a new item
				{
					res->asCreateItem[i].itemId = g_pItemManager->CreateItem(req->asCreateItem[i]);

					pCache->AddItem(res->asCreateItem[i]);
				}
				else
				{
					if (sITEM_DATA* pItem = pCache->GetItemData(req->asCreateItem[i].itemId)) //get item
					{
						pItem->byStackcount = req->asCreateItem[i].byStackcount;
						g_pItemRepository->UpdateCount(req->asCreateItem[i].itemId, pItem->byStackcount); //update db stack count
					}
				}
			}
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	
	packet.SetPacketLen(sizeof(sQG_MATERIAL_DISASSEMBLE_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvInvisibleCostumeUpdateReq(CNtlPacket* pPacket, CQueryServer* app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_INVISIBLE_COSTUME_UPDATE_REQ * req = (sGQ_INVISIBLE_COSTUME_UPDATE_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		pCache->SetInvisibleCostume(req->bInvisibleCostume);
		g_pCharacterRepository->UpdateInvisibleCostume(req->bInvisibleCostume, req->charId);
	}
}


void CGameServerSession::RecvItemSocketInsertBeadReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_ITEM_SOCKET_INSERT_BEAD_REQ * req = (sGQ_ITEM_SOCKET_INSERT_BEAD_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_SOCKET_INSERT_BEAD_RES));
	sQG_ITEM_SOCKET_INSERT_BEAD_RES * res = (sQG_ITEM_SOCKET_INSERT_BEAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_SOCKET_INSERT_BEAD_RES;
	res->wResultCode = GAME_SUCCESS;
	res->handle = req->handle;
	res->charId = req->charId;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->ItemId = req->ItemId;
	res->byRestrictState = req->byRestrictState;
	res->byDurationType = res->byDurationType;
	res->byBeadPlace = req->byBeadPlace;
	res->byBeadPos = req->byBeadPos;
	res->BeadItemId = req->BeadItemId;
	res->byBeadRemainStack = req->byBeadRemainStack;
	res->sItemRandomOption[0].optionValue = req->sItemRandomOption[0].optionValue;
	res->sItemRandomOption[0].wOptionIndex = req->sItemRandomOption[0].wOptionIndex;
	res->sItemRandomOption[1].optionValue = req->sItemRandomOption[1].optionValue;
	res->sItemRandomOption[1].wOptionIndex = req->sItemRandomOption[1].wOptionIndex;
	res->nUseStartTime = req->nUseStartTime;
	res->nUseEndTime = req->nUseEndTime;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pItem = pCache->GetItemData(req->ItemId))
		{
			//update bead
			if (sITEM_DATA* pBeadItem = pCache->GetItemData(req->BeadItemId))
			{
				if (pBeadItem->byStackcount == req->byBeadRemainStack + 1)
				{
					if (req->byBeadRemainStack == 0)
					{
						if (pCache->RemoveItem(req->BeadItemId))
							g_pItemRepository->DeleteById(req->BeadItemId);
						else res->wResultCode = QUERY_FAIL;
					}
					else
					{
						g_pItemRepository->UpdateCount(req->BeadItemId, req->byBeadRemainStack);
						pBeadItem->byStackcount = req->byBeadRemainStack;
					}
				}
				else res->wResultCode = QUERY_FAIL;
			}

			if (res->wResultCode == GAME_SUCCESS)
			{
				//update item
				pItem->byRestrictState = req->byRestrictState;
				pItem->byDurationType = req->byDurationType;
				pItem->sOptionSet.aRandomOption[6].optionValue = req->sItemRandomOption[0].optionValue;
				pItem->sOptionSet.aRandomOption[6].wOptionIndex = req->sItemRandomOption[0].wOptionIndex;
				pItem->sOptionSet.aRandomOption[7].optionValue = req->sItemRandomOption[1].optionValue;
				pItem->sOptionSet.aRandomOption[7].wOptionIndex = req->sItemRandomOption[1].wOptionIndex;
				pItem->nUseStartTime = req->nUseStartTime;
				pItem->nUseEndTime = req->nUseEndTime;

				g_pItemRepository->UpdateBeadOptions(req->sItemRandomOption[0].wOptionIndex, req->sItemRandomOption[0].optionValue, req->sItemRandomOption[1].wOptionIndex, req->sItemRandomOption[1].optionValue,
					req->nUseStartTime, req->nUseEndTime, req->byRestrictState, req->byDurationType, req->ItemId);
			}
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	
	packet.SetPacketLen(sizeof(sQG_ITEM_SOCKET_INSERT_BEAD_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvItemSocketDestroyBeadReq(CNtlPacket* pPacket, CQueryServer* app)
{
	sGQ_ITEM_SOCKET_DESTROY_BEAD_REQ * req = (sGQ_ITEM_SOCKET_DESTROY_BEAD_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_SOCKET_DESTROY_BEAD_RES));
	sQG_ITEM_SOCKET_DESTROY_BEAD_RES * res = (sQG_ITEM_SOCKET_DESTROY_BEAD_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_SOCKET_DESTROY_BEAD_RES;
	res->wResultCode = GAME_SUCCESS;
	res->handle = req->handle;
	res->charId = req->charId;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->ItemId = req->ItemId;
	res->byRestrictState = req->byRestrictState;
	res->byDurationType = req->byDurationType;
	res->bTimeOut = req->bTimeOut;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pItem = pCache->GetItemData(req->ItemId))
		{
			//update item
			pItem->byRestrictState = req->byRestrictState;
			pItem->byDurationType = req->byDurationType;
			pItem->sOptionSet.aRandomOption[6].optionValue = 0;
			pItem->sOptionSet.aRandomOption[6].wOptionIndex = INVALID_WORD;
			pItem->sOptionSet.aRandomOption[7].optionValue = 0;
			pItem->sOptionSet.aRandomOption[7].wOptionIndex = INVALID_WORD;
			pItem->nUseStartTime = 0;
			pItem->nUseEndTime = 0;

			g_pItemRepository->ClearBeadOptions(INVALID_WORD, INVALID_WORD, req->byRestrictState, req->byDurationType, req->ItemId);
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQG_ITEM_SOCKET_DESTROY_BEAD_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvCashItemPublicBankAddReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_CASHITEM_PUBLIC_BANK_ADD_REQ * req = (sGQ_CASHITEM_PUBLIC_BANK_ADD_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_CASHITEM_PUBLIC_BANK_ADD_RES));
	sQG_CASHITEM_PUBLIC_BANK_ADD_RES * res = (sQG_CASHITEM_PUBLIC_BANK_ADD_RES *)packet.GetPacketData();
	res->wOpCode = QG_CASHITEM_PUBLIC_BANK_ADD_RES;
	res->handle = req->handle;
	res->wResultCode = GAME_SUCCESS;
	res->hlsItemNo = req->hlsItemNo;
	res->accountId = req->accountID;
	res->charId = req->charId;
	res->dwProductID = req->dwProductId;
	res->byPlace = req->byPlace;
	res->byPosition = req->byPosition;
	res->byRank = req->byRank;
	res->byDurationType = req->byDurationType;
	res->nUseStartTime = req->nUseStartTime;
	res->nUseEndTime = req->nUseEndTime;
	res->itemNo = req->itemNo;
	res->itemID = INVALID_ITEMID;

	CPlayerCache* pPlayerCache = g_pPlayerCache->GetCharacter(req->charId);
	CAccountCache* pAccountCache = g_pPlayerCache->GetAccount(req->accountID);
	if (pPlayerCache && pAccountCache)
	{
		if (pAccountCache->RemoveCashItem(req->dwProductId)) //remove cash item
		{
			//if already have shared bank then extend the time. Otherwise add new
			sITEM_DATA* pItem = pAccountCache->GetBankItem(req->itemNo);
			if (pItem)
			{
					pItem->nUseStartTime = req->nUseStartTime;
					pItem->nUseEndTime = req->nUseEndTime;

					res->itemID = pItem->itemId;

					g_pItemRepository->UpdateUseTimeRange(pItem->itemId, req->nUseStartTime, req->nUseEndTime);
			}
			else
			{
				pItem = new sITEM_DATA;
				pItem->Init();

				res->itemID = g_pItemManager->IncLastItemID();

				g_pItemRepository->InsertSharedBankItemMinimal(res->itemID, req->itemNo, req->byPlace, req->byPosition, req->byRank, req->nUseStartTime, req->nUseEndTime, req->byDurationType, req->accountID);

				pItem->charId = req->charId;
				pItem->byStackcount = 1;
				pItem->itemId = res->itemID;
				pItem->itemNo = req->itemNo;
				pItem->byPlace = req->byPlace;
				pItem->byPosition = req->byPosition;
				pItem->byRank = req->byRank;
				pItem->nUseStartTime = req->nUseStartTime;
				pItem->nUseEndTime = req->nUseEndTime;
				pItem->byDurationType = req->byDurationType;

				pAccountCache->InsertBankItem(pItem);
			}

			g_pCashShopRepository->UpdateStorageItemMoved(res->itemID, req->dwProductId);
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQG_CASHITEM_PUBLIC_BANK_ADD_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvItemExchangeReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_ITEM_EXCHANGE_REQ * req = (sGQ_ITEM_EXCHANGE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_EXCHANGE_RES));
	sQG_ITEM_EXCHANGE_RES * res = (sQG_ITEM_EXCHANGE_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_EXCHANGE_RES;
	memcpy(res->aCreateItem, req->aCreateItem, sizeof(req->aCreateItem));
	memcpy(res->aDeleteItem, req->aDeleteItem, sizeof(req->aDeleteItem));
	memcpy(res->aUpdateItem, req->aUpdateItem, sizeof(req->aUpdateItem));

	res->byCreateCount = req->byCreateCount;
	res->byDeleteCount = req->byDeleteCount;
	res->byReplaceCount = req->byReplaceCount;
	res->byUpdateCount = req->byUpdateCount;
	res->charId = req->charId;
	res->dwMudosapoint = req->dwMudosapoint;
	res->dwZenny = req->dwZenny;
	res->handle = req->handle;
	res->wResultCode = GAME_SUCCESS;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (pCache->GetZeni() >= req->dwZenny)
		{
			if (req->byDeleteCount > 0)
			{
				for (int i = 0; i < req->byDeleteCount; i++)
				{
					if (pCache->RemoveItem(req->aDeleteItem[i].itemId))
						g_pItemRepository->DeleteById(req->aDeleteItem[i].itemId);
					else
					{
						res->wResultCode = GAME_FAIL;
						ERR_LOG(LOG_SYSTEM, "ERROR: Player %u could not find required itemID %I64u to delete..", req->charId, req->aDeleteItem[i].itemId);
						break;
					}
				}
			}

			if (res->wResultCode == GAME_SUCCESS)
			{
				if (req->byCreateCount > 0)
				{
					for (int i = 0; i < req->byCreateCount; i++)
					{
						ITEMID nItemID = g_pItemManager->CreateItem(req->aCreateItem[i]);
						res->aCreateItem[i].itemId = nItemID; //update the req

						pCache->AddItem(res->aCreateItem[i]);
					}
				}

				if (req->byUpdateCount > 0)
				{
					for (int i = 0; i < req->byUpdateCount; i++)
					{
						if (sITEM_DATA* pData = pCache->GetItemData(req->aUpdateItem[i].nItemID))
						{
							pData->byStackcount = req->aUpdateItem[i].byStack;
							g_pItemRepository->UpdateCount(req->aUpdateItem[i].nItemID, req->aUpdateItem[i].byStack);
						}
					}
				}

				if (req->dwMudosapoint > 0 && req->dwMudosapoint != INVALID_DWORD)
				{

				}

				if (req->dwZenny > 0 && req->dwZenny != INVALID_DWORD)
				{
					pCache->SetZeni(UnsignedSafeDecrease<DWORD>(pCache->GetZeni(), req->dwZenny));
					g_pCharacterRepository->UpdateMoney(req->charId, pCache->GetZeni());
				}
			}
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQG_ITEM_EXCHANGE_RES));
	app->Send(GetHandle(), &packet);
}


void CGameServerSession::RecvEventCoinAddReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_EVENT_COIN_ADD_REQ * req = (sGQ_EVENT_COIN_ADD_REQ*)pPacket->GetPacketData();

	CAccountCache* pCache = g_pPlayerCache->GetAccount(req->accountId);
	if (pCache)
	{
		if (req->byCoinType == 0)
		{
			pCache->SetEventCoin(UnsignedSafeIncrease<DWORD>(pCache->GetEventCoin(), req->byIncreaseCoin));
			g_pAccountRepository->UpdateEventCoins(req->accountId, pCache->GetEventCoin());
		}
	}
}

void CGameServerSession::RecvWaguWaguMachineCoinIncreaseReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_WAGUWAGUMACHINE_COIN_INCREASE_REQ * req = (sGQ_WAGUWAGUMACHINE_COIN_INCREASE_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_WAGUWAGUMACHINE_COIN_INCREASE_RES));
	sQG_WAGUWAGUMACHINE_COIN_INCREASE_RES * res = (sQG_WAGUWAGUMACHINE_COIN_INCREASE_RES *)packet.GetPacketData();
	res->wOpCode = QG_WAGUWAGUMACHINE_COIN_INCREASE_RES;
	res->wResultCode = GAME_SUCCESS;
	res->accountId = req->accountId;
	res->characterId = req->characterId;
	res->handle = req->handle;
	res->qwProductId = req->qwProductId;
	res->wWaguCoin = req->wWaguCoin;


	CAccountCache* pCache = g_pPlayerCache->GetAccount(req->accountId);
	if (pCache)
	{
		if (req->qwProductId != INVALID_ITEMID)
		{
			if (pCache->RemoveCashItem(req->qwProductId))
			{
				g_pCashShopRepository->UpdateStorageItemMovedOnly(req->qwProductId);
			}
			else res->wResultCode = QUERY_FAIL;
		}

		if (res->wResultCode == GAME_SUCCESS)
		{
			pCache->SetWaguCoin(UnsignedSafeIncrease<DWORD>(pCache->GetWaguCoin(), req->wWaguCoin));
			g_pAccountRepository->UpdateWaguCoins(req->accountId, pCache->GetWaguCoin());

			res->wTotalWaguCoin = (WORD)pCache->GetWaguCoin();
		}
	}
	else res->wResultCode = QUERY_FAIL;


	packet.SetPacketLen(sizeof(sQG_WAGUWAGUMACHINE_COIN_INCREASE_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvCharacterRenameReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_CHARACTER_RENAME_REQ * req = (sGQ_CHARACTER_RENAME_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_CHARACTER_RENAME_RES));
	sQG_CHARACTER_RENAME_RES * res = (sQG_CHARACTER_RENAME_RES *)packet.GetPacketData();
	res->wOpCode = QG_CHARACTER_RENAME_RES;
	res->wResultCode = GAME_SUCCESS;
	res->byPlace = req->byPlace;
	res->byPos = req->byPos;
	res->charId = req->charId;
	res->dwRemainMin = 0;
	res->handle = req->handle;
	res->itemId = req->itemId;
	NTL_SAFE_WCSCPY(res->wszCharName, req->wszCharName);

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (smart_ptr<QueryResult> namecheck = g_pCharacterRepository->GetByNameTrailingSpace(req->wszCharName))
			res->wResultCode = GAME_SAMENAME_EXIST;
		else
		{
			if (pCache->RemoveItem(req->itemId))
			{
				g_pItemRepository->DeleteById(req->itemId);
				g_pCharacterRepository->UpdateCharNameWait(req->wszCharName, req->charId); //required waitexecute so none can get the same name
				g_pFriendRepository->UpdateFriendName(req->wszCharName, req->charId);
				g_pAuctionHouseRepository->UpdateSellerName(req->wszCharName, req->charId);
				g_pMailRepository->UpdateFromNameByOldName(req->wszCharName, pCache->GetCharName());
				g_pMailRepository->UpdateTargetNameByOldName(req->wszCharName, pCache->GetCharName());

				g_pAuditLogRepository->InsertCharNameChangeLog(req->charId, pCache->GetCharName(), req->wszCharName);

				//if inside guild then update guild member data
				if (pCache->GetGuildID() > 0)
				{
					if (sDBO_GUILD_MEMBER_DATA* pGuildMember = g_pGuild->GetGuildMemberData(pCache->GetGuildID(), req->charId))
					{
						NTL_SAFE_WCSCPY(pGuildMember->wszMemberName, req->wszCharName);
					}
				}

				//set name
				pCache->SetCharName(req->wszCharName);
			}
			else res->wResultCode = QUERY_FAIL;
		}
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_CHARACTER_RENAME_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvItemChangeOptionReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_ITEM_CHANGE_OPTION_REQ * req = (sGQ_ITEM_CHANGE_OPTION_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_CHANGE_OPTION_RES));
	sQG_ITEM_CHANGE_OPTION_RES * res = (sQG_ITEM_CHANGE_OPTION_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_CHANGE_OPTION_RES;
	res->wResultCode = GAME_SUCCESS;
	res->handle = req->handle;
	res->charId = req->charId;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->ItemId = req->ItemId;
	res->byKitPlace = req->byKitPlace;
	res->byKitPos = req->byKitPos;
	res->KitItemId = req->KitItemId;
	res->byKitStack = req->byKitStack;
	memcpy(&res->sItemOptionSet, &req->sItemOptionSet, sizeof(sITEM_OPTION_SET));

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pItem = pCache->GetItemData(req->ItemId))
		{
			if (sITEM_DATA* pKitItem = pCache->GetItemData(req->KitItemId))
			{
				pKitItem->byStackcount = UnsignedSafeDecrease<BYTE>(pKitItem->byStackcount, 1);

				res->byKitStack = pKitItem->byStackcount;

				if (pKitItem->byStackcount == 0)
				{
					pCache->RemoveItem(req->KitItemId);
					g_pItemRepository->DeleteById(req->KitItemId);
				}
				else
				{
					g_pItemRepository->UpdateCount(req->KitItemId, pKitItem->byStackcount);
				}

				memcpy(&pItem->sOptionSet, &req->sItemOptionSet, sizeof(sITEM_OPTION_SET));

				g_pItemRepository->UpdateOptionSet(req->sItemOptionSet.aOptionTblidx[0], req->sItemOptionSet.aOptionTblidx[1],
					req->sItemOptionSet.aRandomOption[0].wOptionIndex, req->sItemOptionSet.aRandomOption[0].optionValue,
					req->sItemOptionSet.aRandomOption[1].wOptionIndex, req->sItemOptionSet.aRandomOption[1].optionValue,
					req->sItemOptionSet.aRandomOption[2].wOptionIndex, req->sItemOptionSet.aRandomOption[2].optionValue,
					req->sItemOptionSet.aRandomOption[3].wOptionIndex, req->sItemOptionSet.aRandomOption[3].optionValue,
					req->ItemId);
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	packet.SetPacketLen(sizeof(sQG_ITEM_CHANGE_OPTION_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvItemUpgradeWorkReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_ITEM_UPGRADE_WORK_REQ * req = (sGQ_ITEM_UPGRADE_WORK_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pItem = pCache->GetItemData(req->itemId))
		{
			if (sITEM_DATA* pStoneItem = pCache->GetItemData(req->stoneId))
			{
				pStoneItem->byStackcount = UnsignedSafeDecrease<BYTE>(pStoneItem->byStackcount, 1);

				if (pStoneItem->byStackcount == 0)
				{
					pCache->RemoveItem(req->stoneId);
					g_pItemRepository->DeleteById(req->stoneId);
				}
				else
				{
					g_pItemRepository->UpdateCount(req->stoneId, pStoneItem->byStackcount);
				}

				TBLIDX coreItemIdx = INVALID_TBLIDX;
				BYTE byCurGrade = pItem->byGrade;

				if (req->bCoreItemUse)
				{
					if (sITEM_DATA* pCoreItem = pCache->GetItemData(req->coreId))
					{
						coreItemIdx = pCoreItem->itemNo;

						pCoreItem->byStackcount = UnsignedSafeDecrease<BYTE>(pCoreItem->byStackcount, 1);

						if (pCoreItem->byStackcount == 0)
						{
							pCache->RemoveItem(req->coreId);
							g_pItemRepository->DeleteById(req->coreId);
						}
						else
						{
							g_pItemRepository->UpdateCount(req->coreId, pCoreItem->byStackcount);
						}

						pItem->byGrade = req->byItemGrade;
						g_pItemRepository->UpdateGrade(req->itemId, req->byItemGrade);
					}
					else
					{
						ERR_LOG(LOG_USER, "ERROR: Could not find CoreItem to upgrade. CharID %u, coreId %I64u, itemId %I64u", req->charId, req->coreId, req->itemId);
					}
				}
				else
				{
					pItem->byGrade = req->byItemGrade;
					g_pItemRepository->UpdateGrade(req->itemId, req->byItemGrade);
				}

				if (byCurGrade >= 12 || req->byItemGrade >= 12)
				{
					g_pAuditLogRepository->InsertItemUpgradeLog(req->charId, req->bIsSuccessful, req->itemId, pItem->itemNo, byCurGrade, req->byItemGrade, req->stoneId, pStoneItem->itemNo, req->bCoreItemUse, req->coreId, coreItemIdx);
				}
			}
		}
	}
}

void CGameServerSession::RecvDynamicFieldSystemCountingReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ * req = (sGQ_DYNAMIC_FIELD_SYSTEM_COUNTING_REQ*)pPacket->GetPacketData();

	/*If nAddCount = 0 then we only send count information to game server (used when gameserver start). When we add to total count and total count exceeds settingcount, then we broadcast to all channel*/

	CNtlPacket packet(sizeof(sQG_DYNAMIC_FIELD_SYSTEM_COUNTING_RES));
	sQG_DYNAMIC_FIELD_SYSTEM_COUNTING_RES * res = (sQG_DYNAMIC_FIELD_SYSTEM_COUNTING_RES *)packet.GetPacketData();
	res->wOpCode = QG_DYNAMIC_FIELD_SYSTEM_COUNTING_RES;
	res->tblidx = req->tblidx;
	res->tWpsEndTime = 0;
	res->dwSettingCount = req->dwSettingCount;
	res->dwTotalCount = g_pDynamicFieldSystem->GetCurCount();

	if (req->nAddCount == 0) //if 0 then we only send count to client
	{
		g_pDynamicFieldSystem->SetMaxCount(req->dwSettingCount);
	}
	else
	{
		g_pDynamicFieldSystem->SetCurCount(g_pDynamicFieldSystem->GetCurCount() + req->nAddCount);
		res->dwTotalCount = g_pDynamicFieldSystem->GetCurCount();

		if (res->dwTotalCount >= req->dwSettingCount) //send to all channels to start dynamic event
		{
			//set counter to 0 because event going to start now
			g_pDynamicFieldRepository->ResetCount(req->ServerFarmId);
			g_pDynamicFieldSystem->SetCurCount(0);

			packet.SetPacketLen(sizeof(sQG_DYNAMIC_FIELD_SYSTEM_COUNTING_RES));
			g_SrvMgr->Broadcast(&packet);
		}
		else
		{
			g_pDynamicFieldRepository->UpdateCount(res->dwTotalCount, req->ServerFarmId);
		}
	}

	if (req->nAddCount == 0 || res->dwTotalCount < req->dwSettingCount)
	{
		packet.SetPacketLen(sizeof(sQG_DYNAMIC_FIELD_SYSTEM_COUNTING_RES));
		app->Send(GetHandle(), &packet);
	}
}

void CGameServerSession::RecvItemSealReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_ITEM_SEAL_REQ * req = (sGQ_ITEM_SEAL_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_SEAL_RES));
	sQG_ITEM_SEAL_RES * res = (sQG_ITEM_SEAL_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_SEAL_RES;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->byRestrictState = req->byRestrictState;
	res->bySealPlace = req->bySealPlace;
	res->bySealPos = req->bySealPos;
	res->bySealStack = req->bySealStack;
	res->charId = req->charId;
	res->handle = req->handle;
	res->itemId = req->itemId;
	res->SealitemId = req->SealitemId;
	res->wResultCode = QUERY_FAIL;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pItem = pCache->GetItemData(req->itemId))
		{
			if (sITEM_DATA* pSealItem = pCache->GetItemData(req->SealitemId))
			{
				if (pSealItem->byStackcount >= req->bySealStack)
				{
					pSealItem->byStackcount -= req->bySealStack;

					res->bySealRemainStack = pSealItem->byStackcount;
					res->wResultCode = GAME_SUCCESS;

					if (pSealItem->byStackcount == 0)
					{
						pCache->RemoveItem(req->SealitemId);
						g_pItemRepository->DeleteById(req->SealitemId);
					}
					else
					{
						g_pItemRepository->UpdateCount(req->SealitemId, pSealItem->byStackcount);
					}

					pItem->byRestrictState = req->byRestrictState;
					g_pItemRepository->UpdateRestrictState(req->itemId, req->byRestrictState);
				}
			}
		}
	}

	packet.SetPacketLen(sizeof(sQG_ITEM_SEAL_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvItemSealExtractReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_ITEM_SEAL_EXTRACT_REQ * req = (sGQ_ITEM_SEAL_EXTRACT_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_SEAL_EXTRACT_RES));
	sQG_ITEM_SEAL_EXTRACT_RES * res = (sQG_ITEM_SEAL_EXTRACT_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_SEAL_EXTRACT_RES;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->byRestrictState = req->byRestrictState;
	res->charId = req->charId;
	res->handle = req->handle;
	res->itemId = req->itemId;
	res->wResultCode = QUERY_FAIL;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pItem = pCache->GetItemData(req->itemId))
		{
			res->wResultCode = GAME_SUCCESS;

			pItem->byRestrictState = req->byRestrictState;
			g_pItemRepository->UpdateRestrictState(req->itemId, req->byRestrictState);
		}
	}

	packet.SetPacketLen(sizeof(sQG_ITEM_SEAL_EXTRACT_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvDragonballScrambleNfy(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_DRAGONBALL_SCRAMBLE_NFY * req = (sGQ_DRAGONBALL_SCRAMBLE_NFY*)pPacket->GetPacketData();

	app->SetDragonballScrambleBegin(req->bStart);

	if (req->bStart == false)
	{
		CNtlPacket packet(sizeof(sQG_DRAGONBALL_SCRAMBLE_END_NFY));
		sQG_DRAGONBALL_SCRAMBLE_END_NFY * res = (sQG_DRAGONBALL_SCRAMBLE_END_NFY*)packet.GetPacketData();
		res->wOpCode = QG_DRAGONBALL_SCRAMBLE_END_NFY;
		g_SrvMgr->Broadcast(&packet, 0);
	}
}

void CGameServerSession::RecvItemUpgradeByCouponReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_ITEM_UPGRADE_BY_COUPON_REQ * req = (sGQ_ITEM_UPGRADE_BY_COUPON_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_UPGRADE_BY_COUPON_RES));
	sQG_ITEM_UPGRADE_BY_COUPON_RES * res = (sQG_ITEM_UPGRADE_BY_COUPON_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_UPGRADE_BY_COUPON_RES;
	res->wResultCode = QUERY_FAIL;
	res->byCouponPlace = req->byCouponPlace;
	res->byCouponPos = req->byCouponPos;
	res->byDurationType = req->byDurationType;
	res->byGrade = req->byGrade;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->byRestrictState = req->byRestrictState;
	res->charId = req->charId;
	res->couponId = req->couponId;
	res->handle = req->handle;
	res->nUseEndTime = req->nUseEndTime;
	res->nUseStartTime = req->nUseStartTime;


	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pEquipmentItem = pCache->GetItemData(req->itemId))
		{
			if (sITEM_DATA* pCouponItem = pCache->GetItemData(req->couponId))
			{
				res->wResultCode = GAME_SUCCESS;

				pEquipmentItem->byRestrictState = req->byRestrictState;
				pEquipmentItem->byGrade = req->byGrade;
				pEquipmentItem->nUseEndTime = req->nUseEndTime;
				pEquipmentItem->nUseStartTime = req->nUseStartTime;
				pEquipmentItem->byDurationType = req->byDurationType;

				g_pItemRepository->UpdateGradeAndDuration(req->byGrade, req->nUseStartTime, req->nUseEndTime, req->byRestrictState, req->byDurationType, req->itemId);

				if (pCouponItem->byStackcount == 1)
				{
					pCache->RemoveItem(req->couponId);
					res->byStack = 0;
					g_pItemRepository->DeleteById(req->couponId);
				}
				else
				{
					pCouponItem->byStackcount -= 1;
					res->byStack = pCouponItem->byStackcount;
					g_pItemRepository->UpdateCount(req->couponId, res->byStack);
				}
			}
		}
	}


	packet.SetPacketLen(sizeof(sQG_ITEM_UPGRADE_BY_COUPON_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvItemGradeInitByCouponReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_ITEM_GRADEINIT_BY_COUPON_REQ * req = (sGQ_ITEM_GRADEINIT_BY_COUPON_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_ITEM_GRADEINIT_BY_COUPON_RES));
	sQG_ITEM_GRADEINIT_BY_COUPON_RES * res = (sQG_ITEM_GRADEINIT_BY_COUPON_RES *)packet.GetPacketData();
	res->wOpCode = QG_ITEM_GRADEINIT_BY_COUPON_RES;
	res->wResultCode = QUERY_FAIL;
	res->byItemPlace = req->byItemPlace;
	res->byItemPos = req->byItemPos;
	res->byRestrictState = req->byRestrictState;
	res->charId = req->charId;
	res->handle = req->handle;

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pEquipmentItem = pCache->GetItemData(req->itemId))
		{
			res->wResultCode = GAME_SUCCESS;

			pEquipmentItem->byRestrictState = req->byRestrictState;
			pEquipmentItem->byGrade = 0;
			pEquipmentItem->nUseEndTime = 0;
			pEquipmentItem->nUseStartTime = 0;
			pEquipmentItem->byDurationType = eDURATIONTYPE_NORMAL;

			g_pItemRepository->ResetGradeAndDuration(req->byRestrictState, req->itemId);
		}
	}

	packet.SetPacketLen(sizeof(sQG_ITEM_GRADEINIT_BY_COUPON_RES));
	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvEventRewardLoadReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_EVENT_REWARD_LOAD_REQ * req = (sGQ_EVENT_REWARD_LOAD_REQ*)pPacket->GetPacketData();

	CAccountCache* pAccount = g_pPlayerCache->GetAccount(req->accountId);
	if (pAccount)
	{
		pAccount->SetSession(GetHandle()); //set session
		pAccount->ClearEventReward(); //clear all reward

		g_pAccountRepository->LoadEventRewardAsync(pAccount, req->accountId, req->handle, req->charId);
	}
}

void CGameServerSession::RecvEventRewardSelectReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sGQ_EVENT_REWARD_SELECT_REQ * req = (sGQ_EVENT_REWARD_SELECT_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_EVENT_REWARD_SELECT_RES));
	sQG_EVENT_REWARD_SELECT_RES * res = (sQG_EVENT_REWARD_SELECT_RES *)packet.GetPacketData();
	res->wOpCode = QG_EVENT_REWARD_SELECT_RES;
	res->wResultCode = GAME_SUCCESS;
	res->charId = req->charId;
	res->handle = req->handle;
	res->eventTblidx = req->eventTblidx;
	memcpy(&res->sItem, &req->sItem, sizeof(sITEM_DATA));

	CAccountCache* pAccount = g_pPlayerCache->GetAccount(req->accountId);
	if (pAccount)
	{
		CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
		if (pCache)
		{
			if (pAccount->HasEventReward(req->eventTblidx, req->charId))
			{
				pAccount->EraseEventReward(req->eventTblidx); // erase reward

				g_pAccountRepository->DeleteEventReward(req->accountId, req->eventTblidx);

				res->sItem.itemId = g_pItemManager->CreateItem(req->sItem);

				pCache->AddItem(res->sItem);
			}
			else res->wResultCode = QUERY_FAIL;
		}
		else res->wResultCode = QUERY_FAIL;
	}
	else res->wResultCode = QUERY_FAIL;

	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvMascotSealSetReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_MASCOT_SEAL_SET_REQ * req = (sGQ_MASCOT_SEAL_SET_REQ*)pPacket->GetPacketData();

	CNtlPacket packet(sizeof(sQG_MASCOT_SEAL_SET_RES));
	sQG_MASCOT_SEAL_SET_RES * res = (sQG_MASCOT_SEAL_SET_RES *)packet.GetPacketData();
	res->wOpCode = QG_MASCOT_SEAL_SET_RES;
	res->wResultCode = GAME_SUCCESS;
	res->charId = req->charId;
	res->handle = req->handle;
	res->byMascotPos = req->byMascotPos;
	res->bySealPlace = req->bySealPlace;
	res->bySealPos = req->bySealPos;
	res->SealitemId = req->SealitemId;
	memcpy(&res->sNewItemData, &req->sNewItemData, sizeof(sITEM_DATA));

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charId);
	if (pCache)
	{
		if (sITEM_DATA* pItem = pCache->GetItemData(req->SealitemId))
		{
			sMASCOT_DATA_EX* pMainMascot = pCache->GetMascotData(req->byMascotPos);
			if (pMainMascot)
			{
				pItem->byStackcount = UnsignedSafeDecrease<BYTE>(pItem->byStackcount, req->bySealStack);

				res->bySealStack = pItem->byStackcount;

				if (pItem->byStackcount == 0)
				{
					pCache->RemoveItem(req->SealitemId);
					g_pItemRepository->DeleteById(req->SealitemId);
				}
				else
				{
					g_pItemRepository->UpdateCount(req->SealitemId, pItem->byStackcount);
				}

				pCache->DeleteMascot(req->byMascotPos);
				g_pMascotRepository->DeleteMascotBySlot(req->charId, req->byMascotPos);

				res->sNewItemData.itemId = g_pItemManager->CreateItem(req->sNewItemData);

				pCache->AddItem(res->sNewItemData);
			}
			else res->wResultCode = MASCOT_NOT_EXIST;
		}
		else res->wResultCode = GAME_NEEDITEM_NOT_FOUND;
	}
	else res->wResultCode = QUERY_FAIL;

	app->Send(GetHandle(), &packet);
}

void CGameServerSession::RecvMascotSealClearReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_MASCOT_SEAL_CLEAR_REQ * req = (sGQ_MASCOT_SEAL_CLEAR_REQ*)pPacket->GetPacketData();


}

void CGameServerSession::RecvGmLog(CNtlPacket * pPacket, CQueryServer * app)
{
	sGQ_GM_LOG * req = (sGQ_GM_LOG*)pPacket->GetPacketData();

	g_pAuditLogRepository->InsertGmLog(req->charId, req->byLogType, ws2s(req->wchBuffer).c_str());
}