#include "stdafx.h"
#include "QueryServer.h"
#include "NtlPacketCQ.h"
#include "NtlPacketQC.h"
#include "NtlResultCode.h"
#include "ItemManager.h"
#include "CharacterManager.h"
#include "PlayerCache.h"
#include "Repository/CharacterRepository.h"
#include "Repository/SkillRepository.h"
#include "Repository/BuffRepository.h"
#include "Repository/FriendRepository.h"
#include "Repository/RecipeRepository.h"
#include "Repository/MailRepository.h"
#include "Repository/MascotRepository.h"
#include "Repository/QuestRepository.h"
#include "Repository/QuickSlotRepository.h"
#include "Repository/ItemRepository.h"
#include "Repository/AuctionHouseRepository.h"
#include "Repository/AuditLogRepository.h"
#include "Repository/AccountRepository.h"



void CCharServerSession::RecvCharacterInfoReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sCQ_CHARACTER_INFO_REQ * req = (sCQ_CHARACTER_INFO_REQ*)pPacket->GetPacketData();

	UNREFERENCED_PARAMETER(app);
	UNREFERENCED_PARAMETER(req);
}


////--------------------------------------------------------------------------------------//
////		CREATE CHARACTER
////--------------------------------------------------------------------------------------//
void CCharServerSession::RecvCreateCharReq(CNtlPacket * pPacket, CQueryServer* app)
{
	sCQ_CHARACTER_ADD_REQ * req = (sCQ_CHARACTER_ADD_REQ*)pPacket->GetPacketData();

	//check if char name already exist
	smart_ptr<QueryResult> namecheck = g_pCharacterRepository->GetByName(req->awchCharName);
	if (namecheck)
	{
		CNtlPacket packet(sizeof(sQC_CHARACTER_ADD_RES));
		sQC_CHARACTER_ADD_RES * res = (sQC_CHARACTER_ADD_RES *)packet.GetPacketData();
		res->wOpCode = QC_CHARACTER_ADD_RES;
		res->accountId = req->accountId;
		res->wResultCode = CHARACTER_SAMENAME_EXIST;
		packet.SetPacketLen(sizeof(sQC_CHARACTER_ADD_RES));
		app->Send(GetHandle(), &packet);
	}
	else
	{
		CHARACTERID newCharId = g_pCharacterManager->AcquireCharID(); //TO GENERATE CHAR ID

		CNtlPacket packet(sizeof(sQC_CHARACTER_ADD_RES));
		sQC_CHARACTER_ADD_RES * res = (sQC_CHARACTER_ADD_RES *)packet.GetPacketData();
		res->wOpCode = QC_CHARACTER_ADD_RES;
		res->wResultCode = CHARACTER_SUCCESS;
		res->accountId = req->accountId;
		res->sPcDataSummary.charId = newCharId;
		wcscpy_s(res->sPcDataSummary.awchName, NTL_MAX_SIZE_CHAR_NAME + 1, req->awchCharName);
		res->sPcDataSummary.byRace = req->byRace;
		res->sPcDataSummary.byClass = req->byClass;
		res->sPcDataSummary.bIsAdult = false;
		res->sPcDataSummary.byGender = req->byGender;
		res->sPcDataSummary.byFace = req->byFace;
		res->sPcDataSummary.byHair = req->byHair;
		res->sPcDataSummary.byHairColor = req->byHairColor;
		res->sPcDataSummary.bySkinColor = req->bySkinColor;
		res->sPcDataSummary.byLevel = 1;
		res->sPcDataSummary.worldTblidx = req->bindWorldId;
		res->sPcDataSummary.worldId = req->bindWorldId;
		res->sPcDataSummary.fPositionX = req->vSpawn_Loc.x;
		res->sPcDataSummary.fPositionY = req->vSpawn_Loc.y;
		res->sPcDataSummary.fPositionZ = req->vSpawn_Loc.z;
		res->sPcDataSummary.dwMoney = 0;
		res->sPcDataSummary.dwMoneyBank = 0;

		memset(&res->sPcDataSummary.sItem, -1, sizeof(res->sPcDataSummary.sItem));

		for (int i = 0; i < req->byItemCount; i++)
		{
			if ((req->sItem[i].itemNo != 0 || req->sItem[i].itemNo != INVALID_TBLIDX) && req->sItem[i].byPlace == CONTAINER_TYPE_EQUIP)
			{
				res->sPcDataSummary.sItem[req->sItem[i].byPosition].byBattleAttribute = req->sItem[i].byBattleAttribute;
				res->sPcDataSummary.sItem[req->sItem[i].byPosition].byGrade = req->sItem[i].byGrade;
				res->sPcDataSummary.sItem[req->sItem[i].byPosition].byRank = req->sItem[i].byRank;
				res->sPcDataSummary.sItem[req->sItem[i].byPosition].tblidx = req->sItem[i].itemNo;
			}

			req->sItem[i].charId = newCharId;
			g_pItemManager->CreateItem(req->sItem[i]);
		}

		for (int i = 0; i < req->bySkillCount; i++)
		{
			g_pSkillRepository->InsertSkill(req->aSkill[i], newCharId, i);
		}

		for (int i = 0; i < 3; i++)
		{
			g_pCharacterRepository->InsertPortal(newCharId, req->defaultPortalId[i]);
		}

		g_pCharacterRepository->InsertBind(newCharId, req->bindWorldId, req->vBind_Loc.x, req->vBind_Loc.y, req->vBind_Loc.z, req->vBind_Dir.x, req->vBind_Dir.y, req->vBind_Dir.z);

		res->sPcDataSummary.dwMapInfoIndex = (DWORD)req->mapNameTblidx;
		res->sPcDataSummary.bTutorialFlag = false; //must change to false when tutorial works
		res->sPcDataSummary.bNeedNameChange = false;
		res->sPcDataSummary.bInvisibleCostume = false;
		res->sPcDataSummary.bySuperiorEffectType = req->bySuperiorType;
		res->sPcDataSummary.charTitle = INVALID_TBLIDX;
		res->sPcDataSummary.sDogi.byDojoColor = INVALID_BYTE;
		res->sPcDataSummary.sDogi.byGuildColor = INVALID_BYTE;
		res->sPcDataSummary.sDogi.byType = INVALID_BYTE;
		res->sPcDataSummary.sDogi.guildId = INVALID_GUILDID;
		res->sPcDataSummary.sMark.byMarkInColor = INVALID_BYTE;
		res->sPcDataSummary.sMark.byMarkInLine = INVALID_BYTE;
		res->sPcDataSummary.sMark.byMarkMain = INVALID_BYTE;
		res->sPcDataSummary.sMark.byMarkMainColor = INVALID_BYTE;
		res->sPcDataSummary.sMark.byMarkOutColor = INVALID_BYTE;
		res->sPcDataSummary.sMark.byMarkOutLine = INVALID_BYTE;

		g_pCharacterManager->CreateCharacter(req->accountId, res->sPcDataSummary, req->serverId); //create the char in database

		packet.SetPacketLen(sizeof(sQC_CHARACTER_ADD_RES));
		app->Send(GetHandle(), &packet);
	}
}

void CCharServerSession::RecvCharacterDelReq(CNtlPacket * pPacket, CQueryServer * app)
{
	UNREFERENCED_PARAMETER(app);

	sCQ_CHARACTER_DEL_REQ * req = (sCQ_CHARACTER_DEL_REQ*)pPacket->GetPacketData();

	CPlayerCache* pCache = g_pPlayerCache->GetCharacter(req->charID);
	if (pCache)
	{
		g_pPlayerCache->EraseCharacter(req->charID);
		delete pCache;
	}

	g_pAuditLogRepository->InsertCharacterDeleteLog(req->accountID, req->charID);

	g_pCharacterRepository->DeleteCharacter(req->charID);
	g_pAuctionHouseRepository->DeleteListingsByChar(req->charID);
	g_pCharacterRepository->DeleteBind(req->charID);
	g_pBuffRepository->DeleteBuffs(req->charID);
	g_pFriendRepository->DeleteFriendList(req->charID);
	g_pRecipeRepository->DeleteRecipes(req->charID);
	g_pSkillRepository->DeleteHtbSkills(req->charID);
	g_pItemRepository->DeleteItemsByOwner(req->charID);
	g_pMailRepository->DeleteMailByChar(req->charID);
	g_pMascotRepository->DeleteMascots(req->charID);
	g_pCharacterRepository->DeletePortals(req->charID);
	g_pQuestRepository->DeleteQuestItems(req->charID);
	g_pQuestRepository->DeleteQuests(req->charID);
	g_pQuickSlotRepository->DeleteQuickSlots(req->charID);
	g_pSkillRepository->DeleteSkills(req->charID);
	g_pCharacterRepository->DeleteTitles(req->charID);
	g_pCharacterRepository->DeleteWarfog(req->charID);
	g_pItemRepository->DeleteItemsCdByChar(req->charID);
	g_pQuickSlotRepository->DeleteQuickTeleports(req->charID);
	g_pCharacterRepository->DeleteRankBattle(req->charID);
	g_pCharacterRepository->DeleteSkyDungeonLimit(req->charID);
	g_pCharacterRepository->DeleteDwcData(req->charID);
}

void CCharServerSession::RecvCharacterLoadReq(CNtlPacket * pPacket, CQueryServer * app)
{
	sCQ_CHARACTER_LOAD_REQ * req = (sCQ_CHARACTER_LOAD_REQ*)pPacket->GetPacketData();

	CAccountCache* pAccount = g_pPlayerCache->GetAccount(req->accountId);
	if (pAccount)
	{
		pAccount->SetSession(GetHandle());

		CNtlPacket pQry(sizeof(sQC_CHARACTER_LOAD_RES));
		sQC_CHARACTER_LOAD_RES * qRes = (sQC_CHARACTER_LOAD_RES *)pQry.GetPacketData();
		qRes->wOpCode = QC_CHARACTER_LOAD_RES;
		qRes->accountId = req->accountId;
		qRes->serverFarmId = req->serverFarmId;
		pQry.SetPacketLen(sizeof(sQC_CHARACTER_LOAD_RES));
		app->Send(GetHandle(), &pQry);
	}
	else
	{
		pAccount = new CAccountCache(req->accountId);

		pAccount->SetSession(GetHandle());

		g_pAccountRepository->LoadAccountDataAsync(pAccount, req->accountId);

		g_pPlayerCache->InsertAccount(req->accountId, pAccount);
	}
}

