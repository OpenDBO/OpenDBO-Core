/*****************************************************************************
* File			: DboPacketHandler_GUDBScramble.cpp
* Abstract		: Dragonball scramble packet handlers
*****************************************************************************
* Desc			: The scramble runs on one channel. Participants can fight each other
*				  outside of towns (party members excepted) and hunt the 7 battle
*				  dragonballs, which are dropped when the carrier faints, enters a town
*				  or throws them away.
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// core
#include "NtlMovement.h"

// sound
#include "NtlSoundManager.h"
#include "GUISoundDefine.h"

// simulation
#include "NtlSobGroup.h"
#include "NtlSobPlayerAttr.h"
#include "NtlFSMDef.h"

// client
#include "DboEvent.h"


namespace
{
	CNtlSobPlayerAttr* GetDBSPlayerAttr(SERIAL_HANDLE hSerial)
	{
		CNtlSob* pSob = GetNtlSobManager()->GetSobObject(hSerial);
		if (!pSob)
			return NULL;

		if (pSob->GetClassID() != SLCLASS_AVATAR && pSob->GetClassID() != SLCLASS_PLAYER)
			return NULL;

		return reinterpret_cast<CNtlSobPlayerAttr*>(pSob->GetSobAttr());
	}

	// rivals are shown with the pvp name color
	void UpdatePlayerNameColors()
	{
		CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_PLAYER);
		if (!pSobGroup)
			return;

		for (CNtlSobGroup::MapObject::iterator it = pSobGroup->GetObjects().begin(); it != pSobGroup->GetObjects().end(); ++it)
			Logic_SetHeadNameColor(it->second);
	}

	void SetAvatarDBSJoined(bool bJoined)
	{
		SAvatarInfo* pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
		pAvatarInfo->sCharPf.bIsScrambleJoinFlag = bJoined;
		if (!bJoined)
			pAvatarInfo->sCharPf.byHasBattleDragonBallBitFlag = 0;

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if (pAvatar)
		{
			CNtlSobPlayerAttr* pAvatarAttr = reinterpret_cast<CNtlSobPlayerAttr*>(pAvatar->GetSobAttr());
			pAvatarAttr->SetIsInDBSBattle(bJoined);
			if (!bJoined)
				pAvatarAttr->SetDBSBallFlag(0);
		}

		if (bJoined)
		{
			if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DRAGONBALL_SCRAMBLE) == FALSE)
				GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_DRAGONBALL_SCRAMBLE);
		}
		else
		{
			if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DRAGONBALL_SCRAMBLE))
				GetNtlWorldConcept()->RemoveWorldPlayConcept(WORLD_PLAY_DRAGONBALL_SCRAMBLE);
		}

		UpdatePlayerNameColors();

		CDboEventGenerator::DBScramble(DBSCRAMBLE_EVENT_JOIN);
	}

	void SetDBSSeason(bool bSeason)
	{
		sDBSCRAMBLE_INFO* pInfo = GetDboGlobal()->GetDBScrambleInfo();

		if (bSeason)
		{
			pInfo->bSeason = TRUE;
			CDboEventGenerator::DBScramble(DBSCRAMBLE_EVENT_SEASON_START);
			return;
		}

		// everybody leaves the scramble
		CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_PLAYER);
		if (pSobGroup)
		{
			for (CNtlSobGroup::MapObject::iterator it = pSobGroup->GetObjects().begin(); it != pSobGroup->GetObjects().end(); ++it)
			{
				CNtlSobPlayerAttr* pPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>(it->second->GetSobAttr());
				pPlayerAttr->SetIsInDBSBattle(false);
				pPlayerAttr->SetDBSBallFlag(0);
			}
		}

		SetAvatarDBSJoined(false);

		pInfo->Reset();
		CDboEventGenerator::DBScramble(DBSCRAMBLE_EVENT_SEASON_END);
	}
}

void DBScramble_ResetState()
{
	// a new game server sends its own scramble state after the avatar entered the game
	if (GetDboGlobal()->GetDBScrambleInfo()->bSeason || GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DRAGONBALL_SCRAMBLE))
		SetDBSSeason(false);
}

void PacketHandler_GUDragonballScrambleJoinRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_DRAGONBALL_SCRAMBLE_JOIN_RES);

	sGU_DRAGONBALL_SCRAMBLE_JOIN_RES* pResult = (sGU_DRAGONBALL_SCRAMBLE_JOIN_RES*)pPacket;

	if (pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, "GU_DRAGONBALL_SCRAMBLE_JOIN_RES"), TRUE);
		return;
	}

	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup = CHANNEL_GROUP_JINGLE_MUSIC;
	tSoundParam.pcFileName = GSD_DB_SCRAMBLE_START;
	GetSoundManager()->Play(&tSoundParam);

	CDboEventGenerator::FlashNotifyString(dINVALID_FLASH_NOTIFY_FILE_DBS_JOIN);

	SetAvatarDBSJoined(true);
}

void PacketHandler_GUDragonballScrambleJoinNfy(void *pPacket)
{
	sGU_DRAGONBALL_SCRAMBLE_JOIN_NFY* pNfy = (sGU_DRAGONBALL_SCRAMBLE_JOIN_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (pAvatar && pAvatar->GetSerialID() == pNfy->handle)
	{
		// already handled by GU_DRAGONBALL_SCRAMBLE_JOIN_RES
		return;
	}

	CNtlSobPlayerAttr* pPlayerAttr = GetDBSPlayerAttr(pNfy->handle);
	if (!pPlayerAttr)
		return;

	pPlayerAttr->SetIsInDBSBattle(true);

	Logic_SetHeadNameColor(GetNtlSobManager()->GetSobObject(pNfy->handle));
}

void PacketHandler_GUDragonballScrambleBallLocUpdateNfy(void *pPacket)
{
	sGU_DRAGONBALL_SCRAMBLE_BALL_LOC_UPDATE_NFY* pNfy = (sGU_DRAGONBALL_SCRAMBLE_BALL_LOC_UPDATE_NFY*)pPacket;

	sDBSCRAMBLE_INFO* pInfo = GetDboGlobal()->GetDBScrambleInfo();

	for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; ++i)
	{
		RwV3d& vLoc = pInfo->avBallLoc[i];
		NtlLocationDecompress(&pNfy->vBallLoc[i], &vLoc.x, &vLoc.y, &vLoc.z);
	}

	pInfo->bBallLocation = TRUE;

	CDboEventGenerator::DBScramble(DBSCRAMBLE_EVENT_BALL_LOCATION);
}

void PacketHandler_GUDragonballScrambleSeasonStateNfy(void *pPacket)
{
	sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY* pNfy = (sGU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY*)pPacket;

	sDBSCRAMBLE_INFO* pInfo = GetDboGlobal()->GetDBScrambleInfo();

	switch (pNfy->wSeasonState)
	{
		case DRAGONBALLSCRAMBLE_SEASON_STATE_REFRESH:	// only sent to participants
		{
			if (!pInfo->bSeason)
				SetDBSSeason(true);

			SetAvatarDBSJoined(true);
		}
		break;

		case DRAGONBALLSCRAMBLE_SEASON_STATE_START:
		{
			if (pNfy->bIsTypeChange)
				GetAlarmManager()->AlarmMessage("DST_DRAGONBALL_SCRAMBLE_SEASON_BEGIN_NFY");

			SetDBSSeason(true);
		}
		break;

		case DRAGONBALLSCRAMBLE_SEASON_STATE_COOLTIME:
		{
			if (pNfy->bIsTypeChange)
				GetAlarmManager()->AlarmMessage("DST_DRAGONBALL_SCRAMBLE_SEASON_REWARD_COOLTIME_NFY");
		}
		break;

		case DRAGONBALLSCRAMBLE_SEASON_STATE_END:
		{
			if (pNfy->bIsTypeChange && pInfo->bSeason)
				GetAlarmManager()->AlarmMessage("DST_DRAGONBALL_SCRAMBLE_SEASON_END_NFY");

			SetDBSSeason(false);
		}
		break;

		default:
			DBO_WARNING_MESSAGE("GU_DRAGONBALL_SCRAMBLE_SEASON_STATE_NFY unknown state: " << pNfy->wSeasonState);
		break;
	}
}

void PacketHandler_GUDragonballScrambleHasBallNfy(void *pPacket)
{
	sGU_DRAGONBALL_SCRAMBLE_HAS_BALL_NFY* pNfy = (sGU_DRAGONBALL_SCRAMBLE_HAS_BALL_NFY*)pPacket;

	CNtlSobPlayerAttr* pPlayerAttr = GetDBSPlayerAttr(pNfy->handle);
	if (!pPlayerAttr)
		return;

	RwUInt8 byOldFlag = pPlayerAttr->GetDBSBallFlag();
	pPlayerAttr->SetDBSBallFlag(pNfy->byHasDragonBallFlag);

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar || pAvatar->GetSerialID() != pNfy->handle)
		return;

	GetNtlSLGlobal()->GetAvatarInfo()->sCharPf.byHasBattleDragonBallBitFlag = pNfy->byHasDragonBallFlag;

	// picked up
	RwUInt8 byGained = pNfy->byHasDragonBallFlag & ~byOldFlag;
	for (int i = 0; i < NTL_ITEM_MAX_DRAGONBALL; ++i)
	{
		if (byGained & (1 << i))
			GetAlarmManager()->FormattedAlarmMessage("DST_DRAGONBALL_SCRAMBLE_LOOT_BATTLE_DRAGONBALL", FALSE, NULL, i + 1);
	}

	// lost
	if (byOldFlag != 0 && pNfy->byHasDragonBallFlag == 0)
	{
		RwUInt32 uiStateId = Logic_GetActorStateId(pAvatar);
		if (uiStateId == NTL_FSMSID_FAINTING || uiStateId == NTL_FSMSID_DIE)
			GetAlarmManager()->AlarmMessage("DST_DRAGONBALL_SCRAMBLE_FAINT_DROP_MSG");
		else
			GetAlarmManager()->AlarmMessage("DST_DRAGONBALL_SCRAMBLE_DROP_MSG");
	}
}

void PacketHandler_GUDragonballScrambleMessageNfy(void *pPacket)
{
	sGU_DRAGONBALL_SCRAMBLE_MESSAGE_NFY* pNfy = (sGU_DRAGONBALL_SCRAMBLE_MESSAGE_NFY*)pPacket;

	switch (pNfy->eMSG)
	{
		case DRAGONBALLSCRAMBLE_MESSAGE_ENTER_PK_FORBIDDEN_AREA:
			GetAlarmManager()->AlarmMessage("DST_DRAGONBALL_SCRAMBLE_DROP_MSG");
		break;
		case DRAGONBALLSCRAMBLE_MESSAGE_ADD_MUDOSA_POINT:
			GetAlarmManager()->FormattedAlarmMessage("DST_DRAGONBALL_SCRAMBLE_GAIN_BUDOJI_MILEAGE", FALSE, NULL, pNfy->dwVal);
		break;
		case DRAGONBALLSCRAMBLE_MESSAGE_PARTY_LEAVE:
			GetAlarmManager()->AlarmMessage("DST_DRAGONBALL_SCRAMBLE_PARTY_LEAVE");
		break;

		default: break;
	}
}
