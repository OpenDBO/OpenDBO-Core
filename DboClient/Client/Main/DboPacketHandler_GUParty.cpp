/*****************************************************************************
* File			: DboPackethandler_GUParty.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: 파티 관련 패킷 핸들
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// shared
#include "ItemTable.h"

// sound
#include "GUISoundDefine.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLEventFunc.h"
#include "NtlSLPacketGenerator.h"

// dbo
#include "IconMoveManager.h"
#include "DiceManager.h"

void PacketHandler_GSPartyCreateRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_CREATE_RES );

	// 자신이 요청한 파티 생성 결과를 알려준다.
	sGU_PARTY_CREATE_RES* pResult = (sGU_PARTY_CREATE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pAvatar->GetSobAttr());

	// 파티 생성
	CNtlSLEventGenerator::PartyCreate(pResult->wszPartyName);

	// 파티 리스트에 자신을 추가시킨다
	CNtlSLEventGenerator::PartyMemberAdd(pAvatar->GetSerialID(), (WCHAR*)pAvatarAttr->GetName());

	// 자신이 파티장이다
	CNtlSLEventGenerator::PartyLeaderChange(pAvatar->GetSerialID());

	// 제니 분배 방식
	CNtlSLEventGenerator::PartyUpdate(PMT_ZENNY_DIVISION, INVALID_SERIAL_ID, NTL_PARTY_ZENNY_LOOTING_GREEDILY);

	// 아이템 분배 방식
	CNtlSLEventGenerator::PartyUpdate(PMT_ITEM_DIVISION, INVALID_SERIAL_ID, NTL_PARTY_ITEM_LOOTING_GREEDILY);

	// 파티 인벤에 저장되는 아이템의 등급
	CDboEventGenerator::PartyInventory(PINVEN_CHANGE_SAVED_ITEM_GRADE, 0, INVALID_SERIAL_ID, 0, DBO_PARTY_INVEN_DEFAULT_ITEM_RANK);

	// 파티 퀘스트
	CNtlSLEventGenerator::TSParty();
}

void PacketHandler_GSPartyDisbandRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_DISBAND_RES );

	// 자신이 파티를 해산한 결과를 알려준다.
	sGU_PARTY_DISBAND_RES* pResult = (sGU_PARTY_DISBAND_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyDisband();
}

void PacketHandler_GSPartyDisbandNfy(void *pPacket)
{
	// 파티가 해산되었음을 파티장이 아닌 맴버들에게 알려준다.
	//sGU_PARTY_DISBANDED_NFY* pResult = (sGU_PARTY_DISBANDED_NFY*)pPacket;

	CNtlSLEventGenerator::PartyDisband();
}

void PacketHandler_GSPartyInviteRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_INVITE_RES );

	// 파티 리더에게 파티 초대 결과를 알려준다.	
	sGU_PARTY_INVITE_RES* pResult = (sGU_PARTY_INVITE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}	

	// %s님을 파티에 초대하였습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVITE_SUCCESS, FALSE, NULL, pResult->wszTargetName);
}

void PacketHandler_GSPartyInviteNfy(void *pPacket)
{
	// 초대 대상에게 메세지를 보낸다.
	sGU_PARTY_INVITE_NFY* pResult = (sGU_PARTY_INVITE_NFY*)pPacket;

	if( pResult->bFromPc )
	{
		// %s 님의 파티에 가입하시겠습니까?
		GetAlarmManager()->FormattedAlarmMessage( DST_PARTY_PARTY_INVITE_WITHOUT_PARTY, FALSE, NULL, pResult->wszInvitorPcName );
	}
	else
	{
		// %s 파티에 가입하시겠습니까?
		GetAlarmManager()->FormattedAlarmMessage( DST_PARTY_PARTY_INVITE, FALSE, NULL, pResult->wszInvitorPartyName );
	}
}

void PacketHandler_GSPartyResponseInvitationRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_RESPONSE_INVITATION_RES );

	// 파티 초대의 응답에 대한 결과를 리더에게 보낸다.
	sGU_PARTY_RESPONSE_INVITATION_RES* pResult = (sGU_PARTY_RESPONSE_INVITATION_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}	
}

void PacketHandler_GSPartyMemberJoinedNfy(void *pPacket)
{
	// 새로운 파티원이 가입했음을 알려준다.
	sGU_PARTY_MEMBER_JOINED_NFY* pResult = (sGU_PARTY_MEMBER_JOINED_NFY*)pPacket;

	CNtlSLEventGenerator::PartyMemberAdd(pResult->memberInfo.hHandle, pResult->memberInfo.awchMemberName,
		pResult->memberInfo.byRace, pResult->memberInfo.byClass, pResult->memberInfo.byLevel,
		pResult->memberInfo.wCurLP, pResult->memberInfo.wMaxLP, pResult->memberInfo.wCurEP,
		pResult->memberInfo.wMaxEP, pResult->memberInfo.worldTblidx, pResult->memberInfo.worldId,
		pResult->memberInfo.vCurLoc.x, pResult->memberInfo.vCurLoc.y, pResult->memberInfo.vCurLoc.z);

	// %s님이 파티에 들어오셨습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_NEW_MEMBER, FALSE, NULL, pResult->memberInfo.awchMemberName);
}

void PacketHandler_GSPartyInfo(void *pPacket)
{
	// 새로 가입한 파티원이 파티정보를 받는다.
	sGU_PARTY_INFO* pResult = (sGU_PARTY_INFO*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	// 파티를 생성한다
	CNtlSLEventGenerator::PartyCreate(pResult->awchPartyName);

	for(RwInt32 i = 0 ; i < pResult->byMemberInfoCount ; ++i )
	{		
		CNtlSLEventGenerator::PartyMemberAdd(pResult->memberInfo[i].hHandle, pResult->memberInfo[i].awchMemberName,
			pResult->memberInfo[i].byRace, pResult->memberInfo[i].byClass,
			pResult->memberInfo[i].byLevel, pResult->memberInfo[i].wCurLP,
			pResult->memberInfo[i].wMaxLP, pResult->memberInfo[i].wCurEP,
			pResult->memberInfo[i].wMaxEP,
			pResult->memberInfo[i].worldTblidx, pResult->memberInfo[i].worldId,
			pResult->memberInfo[i].vCurLoc.x, pResult->memberInfo[i].vCurLoc.y, pResult->memberInfo[i].vCurLoc.z);
	}	

	// 파티 리스트에 자신을 추가시킨다	
	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pAvatar->GetSobAttr());
	CNtlSLEventGenerator::PartyMemberAdd(pAvatar->GetSerialID(), (WCHAR*)pAvatarAttr->GetName());

	// 파티장을 설정한다
	CNtlSLEventGenerator::PartyLeaderChange(pResult->hLeader);

	// 아이템 분배 방식
	CNtlSLEventGenerator::PartyUpdate(PMT_ITEM_DIVISION, INVALID_SERIAL_ID, pResult->byItemLootingMethod);

	// 제니 분배 방식
	CNtlSLEventGenerator::PartyUpdate(PMT_ZENNY_DIVISION, INVALID_SERIAL_ID, pResult->byZennyLootingMethod);

	// 파티 버프 정보
	CDboEventGenerator::PartyCharm(PCET_ACTIVATE, 0, pResult->registeredCharmTblidx[0]);
	CDboEventGenerator::PartyCharm(PCET_ACTIVATE, 1, pResult->registeredCharmTblidx[1]);

	for( RwUInt8 i = 0 ; i < NTL_PARTY_CHARM_INVENTORY_COUNT ; ++i )
	{
		CNtlSLEventGenerator::PartyCharmActivity(PCAT_ACTIVE, i, pResult->registeredCharmTblidx[i], (RwReal)pResult->adwRemainingTime[i]);
	}

    // 공유 타겟 데이터    
    CNtlSLEventGenerator::PartyShareTargetNfy(pResult->sharetargetInfo);

	// 파티 던전 정보
	CNtlSLEventGenerator::PartyUpdate(PMT_PARTY_DUNGEON_STATE, INVALID_SERIAL_ID, pResult->eDiff);	

	// 파티 인벤에 들어오는 아이템 등급
	CDboEventGenerator::PartyInventory(PINVEN_CHANGE_SAVED_ITEM_GRADE, 0, INVALID_SERIAL_ID, 0, pResult->byItemRank);
}

void PacketHandler_GSPartyInventoryInfo(void *pPacket)
{
	// 새로 들어온 파티 맴버에게 파티 인벤 정보를 알려준다
	sGU_PARTY_INVENTORY_INFO* pResult = (sGU_PARTY_INVENTORY_INFO*)pPacket;

	for(RwInt32 i = 0 ; i < pResult->bySlotInfoCount ; ++i )
	{
		CDboEventGenerator::PartyInventory(PINVEN_ADD_ITEM, pResult->inventorySlotInfo[i].bySlotIndex,
			pResult->inventorySlotInfo[i].itemInfo.tblidx,
			pResult->inventorySlotInfo[i].itemInfo.byStackCount,
			pResult->inventorySlotInfo[i].itemInfo.byRank,
			pResult->inventorySlotInfo[i].itemInfo.aOptionTblidx[0],
			pResult->inventorySlotInfo[i].itemInfo.aOptionTblidx[1],

			pResult->inventorySlotInfo[i].itemInfo.bNeedToIdentify,
			pResult->inventorySlotInfo[i].itemInfo.byGrade);
	}	
}

void PacketHandler_GSPartyOpenedCharmInventoryInfo(void *pPacket)
{
	// 새로 들어온 파티 맴버에게 현재의 파티 부적창 정보를 알려준다
	sGU_PARTY_OPENED_CHARM_INVENTORY_INFO* pResult = (sGU_PARTY_OPENED_CHARM_INVENTORY_INFO*)pPacket;

	// 부적 등록
	if( pResult->victimItemInfo.itemTblidx != INVALID_SERIAL_ID )
	{
		CDboEventGenerator::PartyVictim(PVI_REG_CHARM, pResult->victimItemInfo.itemTblidx, pResult->victimItemInfo.bySlotIndex);

		for(RwInt32 i = 0 ; i < pResult->byVictimInfoCount ; ++i )
		{
			// 제니
			if( pResult->aVictimInfo[i].dwZenny > 0 )
				CDboEventGenerator::PartyVictim(PVI_ZENNY, pResult->aVictimInfo[i].hMember, pResult->aVictimInfo[i].dwZenny);
		}
	}

	// 현재 열려 있는 부적창
	if( 0 <= pResult->byCharmInventoryIndex && pResult->byCharmInventoryIndex < NTL_PARTY_CHARM_INVENTORY_COUNT )
	{
		CNtlSLEventGenerator::PartyUpdate(PMT_LEADER_CHARM_OPEN);
		CDboEventGenerator::PartyCharm(PCET_OPEN, pResult->byCharmInventoryIndex);
	}	
}

void PacketHandler_GSPartyInviteDeclinedNfy(void *pPacket)
{
	// 초대한 대상이 거절했다.
	sGU_PARTY_INVITATION_DECLINED_NFY* pResult = (sGU_PARTY_INVITATION_DECLINED_NFY*)pPacket;	

	// %s님이 초대를 거절하였습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVITE_DECLINE, FALSE, NULL, pResult->wszPlayerName);
}

void PacketHandler_GSPartyInviteExpiredNfy(void *pPacket)
{
	// 파티 초대를 했지만 일정 시간동안 응답이 없었다.
	sGU_PARTY_INVITATION_EXPIRED_NFY* pResult = (sGU_PARTY_INVITATION_EXPIRED_NFY*)pPacket;

	// %s님이 초대를 거절하였습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVITE_DECLINE, FALSE, NULL, pResult->wszPlayerName);
}

void PacketHandler_GSPartyLeaveRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_LEAVE_RES );

	// 자신이 요청한 파티 탈퇴의 결과를 알려준다.
	sGU_PARTY_LEAVE_RES* pResult = (sGU_PARTY_LEAVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyDisband();
}

void PacketHandler_GSPartyMemberLeftNfy(void *pPacket)
{
	// 자신의 파티원이 떠났다.
	sGU_PARTY_MEMBER_LEFT_NFY* pResult = (sGU_PARTY_MEMBER_LEFT_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
		return;
	}

	WCHAR* pwcText	= pMember->wszMemberName;

	// %s님이 파티에서 탈퇴하였습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_LEAVE, FALSE, NULL, pwcText);
	CNtlSLEventGenerator::PartyMemberDel(pResult->hMember);
}

void PacketHandler_GSPartyKickOutRes(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_KICK_OUT_RES );

	// 파티 맴버 강퇴 결과
	sGU_PARTY_KICK_OUT_RES* pResult = (sGU_PARTY_KICK_OUT_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hTargetMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hTargetMember);
		return;
	}

	WCHAR* pwcText = pMember->wszMemberName;

	// %s님이 파티에서 탈퇴하였습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_LEAVE, FALSE, NULL, pwcText);
	CNtlSLEventGenerator::PartyMemberDel(pResult->hTargetMember);
}

void PacketHandler_GSPartyMemberKickedOutNfy(void *pPacket)
{
	// 파티 맴버가 강퇴된 것을 남아있는 파티원에게 알려준다.
	sGU_PARTY_MEMBER_KICKED_OUT_NFY* pResult = (sGU_PARTY_MEMBER_KICKED_OUT_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
		return;
	}

	WCHAR* pwcText = pMember->wszMemberName;

	// %s님이 파티에서 탈퇴하였습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_LEAVE, FALSE, NULL, pwcText);
	CNtlSLEventGenerator::PartyMemberDel(pResult->hMember);
}

void PacketHandler_GSPartyChangeLeaderRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_CHANGE_LEADER_RES );

	// 파티 리더에게 리더 변경 요청을 결과를 알려준다.
	sGU_PARTY_CHANGE_LEADER_RES* pResult = (sGU_PARTY_CHANGE_LEADER_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hNewLeader) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hNewLeader);
		return;
	}

	WCHAR* pwcText = pMember->wszMemberName;

	// %s님이 파티장이 되었습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_CHANGE_LEADER, FALSE, NULL, pwcText);
	CNtlSLEventGenerator::PartyLeaderChange(pResult->hNewLeader);
}

void PacketHandler_GSPartyChangedLeaderNfy(void *pPacket)
{
	// 파티 리더가 변경되었음을 파티 맴버에게 알려준다
	sGU_PARTY_LEADER_CHANGED_NFY* pResult = (sGU_PARTY_LEADER_CHANGED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hNewLeader) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hNewLeader);
		return;
	}	

	WCHAR* pwcText = pMember->wszMemberName;

	// %s님이 파티장이 되었습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_CHANGE_LEADER, FALSE, NULL, pwcText);
	CNtlSLEventGenerator::PartyLeaderChange(pResult->hNewLeader);
}

void PacketHandler_GSPartyLevelUp(void* pPacket)
{
	// 파티원이 레벨업을 하였다
	sGU_PARTY_MEMBER_LEVELED_UP_NFY* pResult = (sGU_PARTY_MEMBER_LEVELED_UP_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_LEVEL, pResult->hMember, pResult->byLevel);
	CNtlSLEventGenerator::PartyUpdate(PMT_LP, pResult->hMember, pMember->wCurLP, pResult->wMaxLP);
	CNtlSLEventGenerator::PartyUpdate(PMT_EP, pResult->hMember, pMember->wCurEP, pResult->wMaxEP);	
}

void PacketHandler_GSPartyChangeClass(void* pPacket)
{
	// 파티원의 클래스가 바뀌었다
	sGU_PARTY_MEMBER_CLASS_CHANGED_NFY* pResult = (sGU_PARTY_MEMBER_CLASS_CHANGED_NFY*)pPacket;
	CNtlSLEventGenerator::PartyUpdate(PMT_CLASS, pResult->hMember, pResult->byNewClass);
}

void PacketHandler_GSPartyMember_LocationNfy(void* pPacket)
{
	// 파티 맴버의 위치를 업데이트 한다
	sGU_PARTY_MEMBER_LOCATION_NFY* pResult = (sGU_PARTY_MEMBER_LOCATION_NFY*)pPacket;

	RwV3d vPos;
	vPos.x = pResult->vCurLoc.x;
	vPos.y = pResult->vCurLoc.y;
	vPos.z = pResult->vCurLoc.z;

	CNtlSLEventGenerator::PartyMemberLocation(pResult->hMember, pResult->worldId, pResult->worldTblidx, vPos);
}

void PacketHandler_GSPartyMemberGainedItemNfy(void *pPacket)
{
	// 파티 맴버가 아이템을 주은 것을 알려준다
	sGU_PARTY_MEMBER_GAINED_ITEM_NFY* pResult = (sGU_PARTY_MEMBER_GAINED_ITEM_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return;
	}

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
		return;
	}

	WCHAR* pwcText = pMember->wszMemberName;

	if(pResult->itemTblidx == INVALID_SERIAL_ID)
	{
		// %s님이 미확인 아이템을 획득하였습니다
		GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVEN_ADD_UNDEFINED_ITEM_OTHER, FALSE, NULL, pwcText);
	}
	else
	{
		// %s님의 획득 아이템 %s
		sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(pResult->itemTblidx);
		if( !pITEM_TBLDAT )
		{
			DBO_FAIL("Not exist item table of index : " << pResult->itemTblidx);
			return;
		}

		memset((char*)awcPacketMessageBuffer, 0, sizeof(WCHAR) * dPACKET_MEESAGE_LENGTH);

		CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
		std::wstring& strItemName = pItemTextTable->GetText(pITEM_TBLDAT->Name);		

		GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_GET_ITEM, FALSE, NULL, pwcText, strItemName.c_str());
	}
}

void PacketHandler_GSPartyMemberGainedZennyNfy(void *pPacket)
{
	// 제니를 분배받았다는 메세지
	sGU_PARTY_MEMBER_GAINED_ZENNY_NFY* pResult = (sGU_PARTY_MEMBER_GAINED_ZENNY_NFY*)pPacket;

	if( pResult->bIsShared )
	{
		// 다른 파티원이 제니를 주어서 분배받았다
		// %d중 %d 제니를 분배받았습니다
        if(pResult->dwBonusZenny == 0)
        {
		    GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_GET_ZENNY, FALSE, NULL, pResult->dwOriginalZenny, pResult->dwZenny);
        }
        else    // PC방 추가 획득
        {
            GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_GET_ZENNY_AND_BONUS, FALSE, NULL, pResult->dwOriginalZenny, pResult->dwAcquisitionZenny, pResult->dwBonusZenny);
        }
	}
	else
	{
		// 다른 파티원이 제니를 주었다
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_FAIL("Not exist avatar instance");
			return;
		}

		CNtlParty*		pParty	= pAvatar->GetParty();
		sPartyMember*	pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
		if( !pMember )
		{
			DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
			return;
		}

		// %s님이 %d 제니를 획득하였습니다
        if(pResult->dwBonusZenny == 0)
        {
		    GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_GET_ZENNY_OTHER, FALSE, NULL, pMember->wszMemberName, pResult->dwZenny);
        }
        else    // PC방 추가 획득
        {
            GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_GET_ZENNY_OTHER_AND_BONUS, FALSE, NULL, pMember->wszMemberName, pResult->dwAcquisitionZenny, pResult->dwBonusZenny);
        }
	}
}

void PacketHandler_GSPartyChangeZennyLootinMethodRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES );

	// 제니 분배 방식 변경 결과를 파티장에게 알려준다
	sGU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES* pResult = (sGU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )	
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_ZENNY_DIVISION, INVALID_SERIAL_ID, pResult->byNewLootingMethod);	

	// 제니 분배방식이 %s로 변경되었습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_CHANGE_ZENNY_DIVISION, FALSE, NULL, GetDisplayStringManager()->GetString(DST_PARTYGUI_ZENNY_LOOTING_PICKUP_PERSON + pResult->byNewLootingMethod));
}

void PacketHandler_GSPartyZennyLootingMethodChangedNfy(void *pPacket)
{
	// 파티장을 제외한 맴버들에게 제니 분배 방식 변경 알리기
	sGU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY* pResult = (sGU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::PartyUpdate(PMT_ZENNY_DIVISION, INVALID_SERIAL_ID, pResult->byNewLootingMethod);

	// 제니 분배방식이 %s로 변경되었습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_CHANGE_ZENNY_DIVISION, FALSE, NULL, GetDisplayStringManager()->GetString(DST_PARTYGUI_ZENNY_LOOTING_PICKUP_PERSON + pResult->byNewLootingMethod));
}

void PacketHandler_GSPartyChangeItemLootinMethodRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES );

	// 아이템 분배 방식 변경 결과를 파티장에게 알려준다
	sGU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES* pResult = (sGU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_ITEM_DIVISION, INVALID_SERIAL_ID, pResult->byNewLootingMethod);

	// 아이템 분배방식이 %s로 변경되었습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_CHANGE_ITEM_DIVISION, FALSE, NULL, GetDisplayStringManager()->GetString(DST_PARTYGUI_ITEM_LOOTING_PICKUP_PERSON + pResult->byNewLootingMethod));
}

void PacketHandler_GSPartyItemLootingMethodChangedNfy(void *pPacket)
{
	// 파티장을 제외한 맴버들에게 제니 아이템 분배 방식 변경 알리기
	sGU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY* pResult = (sGU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY*)pPacket;

	CNtlSLEventGenerator::PartyUpdate(PMT_ITEM_DIVISION, INVALID_SERIAL_ID, pResult->byNewLootingMethod);

	// 아이템 분배방식이 %s로 변경되었습니다
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_CHANGE_ITEM_DIVISION, FALSE, NULL, GetDisplayStringManager()->GetString(DST_PARTYGUI_ITEM_LOOTING_PICKUP_PERSON + pResult->byNewLootingMethod));
}

void PacketHandler_GSPartyInven_ItemAddedNfy(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_ITEM_PICK_RES );

	// 파티 인벤토리에 아이템이 들어왔다
	sGU_PARTY_INVENTORY_ITEM_ADDED_NFY* pResult = (sGU_PARTY_INVENTORY_ITEM_ADDED_NFY*)pPacket;

	CDboEventGenerator::PartyInventory(PINVEN_ADD_ITEM, pResult->bySlotIndex, pResult->newLoot.tblidx, pResult->newLoot.byStackCount,
		pResult->newLoot.byRank, pResult->newLoot.aOptionTblidx[0], pResult->newLoot.aOptionTblidx[1], pResult->newLoot.bNeedToIdentify,
		pResult->newLoot.byGrade, pResult->newLoot.byBattleAttribute);

	if(pResult->newLoot.bNeedToIdentify)
	{
		GetAlarmManager()->AlarmMessage(DST_PARTY_INVEN_ADD_UNDEFINED_ITEM);
	}
	else
	{
		// 파티 인벤토리에 %s 획득
		sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(pResult->newLoot.tblidx);		
		if( !pITEM_TBLDAT )
		{
			DBO_FAIL("Not exist item table of index : " << pResult->newLoot.tblidx);
			return;
		}

		memset((char*)awcPacketMessageBuffer, 0, sizeof(WCHAR) * dPACKET_MEESAGE_LENGTH);
		
		CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
		std::wstring& strItemName = pItemTextTable->GetText(pITEM_TBLDAT->Name);

		GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVEN_ADD_ITEM, FALSE, NULL, strItemName.c_str());
	}	
}

void PacketHandler_GSPartyInven_ItemRemovedNfy(void *pPacket)
{
	// 파티 인벤토리에 아이템이 삭제되었다
	sGU_PARTY_INVENTORY_ITEM_REMOVED_NFY* pResult = (sGU_PARTY_INVENTORY_ITEM_REMOVED_NFY*)pPacket;

	CDboEventGenerator::PartyInventory(PINVEN_DEL_ITEM, pResult->byPos, 0, 0, 0, 0, pResult->bIsYourItem);
}

void PacketHandler_GSPartyInven_RankSetRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_INVEN_ITEM_RANK_SET_RES );

	// 파티장에게 파티 인벤에 보관되는 아이템의 등급이 바뀌었음을 알린다
	sGU_PARTY_INVEN_ITEM_RANK_SET_RES* pResult = (sGU_PARTY_INVEN_ITEM_RANK_SET_RES*)pPacket;	

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::PartyInventory(PINVEN_CHANGE_SAVED_ITEM_GRADE, 0, INVALID_SERIAL_ID, 0, pResult->byItemRank);

	const WCHAR* pwcItemRankNane = GetDisplayStringManager()->GetString(DST_ITEM_GRADE_NORMAL + pResult->byItemRank - 1);
	if( pwcItemRankNane )
		GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVEN_CHANGE_ITEM_GRADE, FALSE, NULL, pwcItemRankNane);
}

void PacketHandler_GSPartyInven_RankSetNfy(void *pPacket)
{
	// 파티원에게 파티 인벤에 보관되는 아이템의 등급이 바뀌었음을 알린다
	sGU_PARTY_INVEN_ITEM_RANK_SET_NFY* pResult = (sGU_PARTY_INVEN_ITEM_RANK_SET_NFY*)pPacket;

	CDboEventGenerator::PartyInventory(PINVEN_CHANGE_SAVED_ITEM_GRADE, 0, INVALID_SERIAL_ID, 0, pResult->byItemRank);

	const WCHAR* pwcItemRankNane = GetDisplayStringManager()->GetString(DST_ITEM_GRADE_NORMAL + pResult->byItemRank - 1);
	if( pwcItemRankNane )
		GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVEN_CHANGE_ITEM_GRADE, FALSE, NULL, pwcItemRankNane);
}

void PacketHandler_GSPartyInven_InvestZennyRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_INVEST_ZENNY_RES );	
	GetIconMoveManager()->IconMoveEnd();

	// 자신이 파티 인벤의 경매에 투자한 결과를 알려준다
	sGU_PARTY_INVEST_ZENNY_RES* pResult = (sGU_PARTY_INVEST_ZENNY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_OCTION_INVEST_NOTIFY, pResult->byPos, Logic_GetAvatarHandle(), pResult->dwInvestedZenny);
}

void PacketHandler_GSPartyInven_InvestZennyNfy(void *pPacket)
{
	// 파티 인벤의 경매에 가장 많은 돈을 투자한 정보를 받는다
	sGU_PARTY_INVEST_ZENNY_NFY* pResult = (sGU_PARTY_INVEST_ZENNY_NFY*)pPacket;

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_OCTION_INVEST_NOTIFY, pResult->byPos, pResult->hMember, pResult->dwInvestedZenny);
}

void PacketHandler_GSPartyInven_LockRes(void *pPacket)
{
	// 파티 인벤의 아이템의 경매 시작 중지 여부를 파티장에게 알려준다
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_INVEN_LOCK_RES );

	sGU_PARTY_INVEN_LOCK_RES* pResult = (sGU_PARTY_INVEN_LOCK_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_OCTION_LOCK, 0, INVALID_SERIAL_ID, 0, 0,
									   INVALID_TBLIDX, INVALID_TBLIDX, pResult->bIsOpen);

	if( pResult->bIsOpen )
		GetAlarmManager()->AlarmMessage(DST_PARTY_INVEN_OCTION_LOCK);
	else
		GetAlarmManager()->AlarmMessage(DST_PARTY_INVEN_OCTION_UNLOCK);
}

void PacketHandler_GSPartyInven_LockNfy(void *pPacket)
{
	// 파티 인벤의 아이템의 경매 시작 중지 여부를 파티원에게 알려준다
	sGU_PARTY_INVEN_LOCK_NFY* pResult = (sGU_PARTY_INVEN_LOCK_NFY*)pPacket;

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_OCTION_LOCK, 0, INVALID_SERIAL_ID, 0, 0,
									   INVALID_TBLIDX, INVALID_TBLIDX, pResult->bIsOpen);

	if( pResult->bIsOpen )
		GetAlarmManager()->AlarmMessage(DST_PARTY_INVEN_OCTION_LOCK);
	else
		GetAlarmManager()->AlarmMessage(DST_PARTY_INVEN_OCTION_UNLOCK);
}

void PacketHandler_GSPartyInven_ResetRes(void *pPacket)
{
	// 파티장에게 파티 인벤의 경매 정보의 초기화에 대한 결과를 알려준다
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_INVEN_RESET_RES );

	sGU_PARTY_INVEN_RESET_RES* pResult = (sGU_PARTY_INVEN_RESET_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_OCTION_RESET, 0);
	GetAlarmManager()->AlarmMessage(DST_PARTY_INVEN_OCTION_RESET);
}

void PacketHandler_GSPartyInven_ResetNfy(void *pPacket)
{
	// 파티원에게 파티 인벤의 경매 정보가 초기화 되었음을 알린다
	//sGU_PARTY_INVEN_RESET_NFY* pResult = (sGU_PARTY_INVEN_RESET_NFY*)pPacket;

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_OCTION_RESET, 0);
	GetAlarmManager()->AlarmMessage(DST_PARTY_INVEN_OCTION_RESET);
}

void PacketHandler_GSPartyInven_AcceptedRes(void *pPacket)
{
	// 파티장에게 경매 낙찰을 한 결과를 알려준다
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_INVEN_ACCEPTED_RES );

	sGU_PARTY_INVEN_ACCEPTED_RES* pResult = (sGU_PARTY_INVEN_ACCEPTED_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_OCTION_SUCCESSFUL_BID, pResult->byItemSlot, pResult->hTarget);
}

void PacketHandler_GSPartyInven_AcceptedNfy(void *pPacket)
{
	// 파티원에게 경매가 낙찰되었음을 알린다
	sGU_PARTY_INVEN_ACCEPTED_NFY* pResult = (sGU_PARTY_INVEN_ACCEPTED_NFY*)pPacket;

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_OCTION_SUCCESSFUL_BID, pResult->byItemSlot, pResult->hTarget);
}

void PacketHandler_GSPartyInven_GiveRes(void *pPacket)
{
	// 파티 인벤의 아이템을 파티장이 직접 분배한 결과를 파티장에게 알려준다
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_INVEN_GIVE_RES );

	sGU_PARTY_INVEN_GIVE_RES* pResult = (sGU_PARTY_INVEN_GIVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_LEADER, pResult->byItemSlot, pResult->hTarget);
}

void PacketHandler_GSPartyInven_GiveNfy(void *pPacket)
{
	// 파티 인벤의 아이템을 파티장이 직접 분배한 결과 파티원에게 알려준다
	sGU_PARTY_INVEN_GIVE_NFY* pResult = (sGU_PARTY_INVEN_GIVE_NFY*)pPacket;

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_LEADER, pResult->byItemSlot, pResult->hTarget);
}

void PacketHandler_GSPartyInven_DiceStartRes(void *pPacket)
{
	// 파티장이 파티 인벤의 아이템을 주사위 굴리기를 시작한 결과를 알려준다
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_INVEN_DICE_START_RES );

	sGU_PARTY_INVEN_DICE_START_RES* pResult = (sGU_PARTY_INVEN_DICE_START_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_DICE_START, pResult->byItemSlot);
	GetAlarmManager()->AlarmMessage(DST_PARTY_INVEN_START_DICE);
}

void PacketHandler_GSPartyInven_DiceStartNfy(void *pPacket)
{
	// 파티원에게 파티 인벤의 아이템을 주사위 굴리기를 시작했음을 알린다
	sGU_PARTY_INVEN_DICE_START_NFY* pResult = (sGU_PARTY_INVEN_DICE_START_NFY*)pPacket;

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_DICE_START, pResult->byItemSlot);
	GetAlarmManager()->AlarmMessage(DST_PARTY_INVEN_START_DICE);
}

void PacketHandler_GSPartyInven_DiceRollRes(void *pPacket)
{
	// 파티 인벤의 아이템의 분배를 위해 자신이 굴린 주사위의 결과를 알려준다
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_INVEN_DICE_ROLL_RES );

	sGU_PARTY_INVEN_DICE_ROLL_RES* pResult = (sGU_PARTY_INVEN_DICE_ROLL_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::DiceResult(Logic_GetAvatarHandle(), DICE_WORKD_ID_PARTY_INVEN_DICE, pResult->byRet);
}

void PacketHandler_GSPartyInven_DiceRollNfy(void *pPacket)
{
	// 파티 인벤의 아이템의 분배를 위해 파티워이 굴린 주사위의 결과를 알려준다
	sGU_PARTY_INVEN_DICE_ROLL_NFY* pResult = (sGU_PARTY_INVEN_DICE_ROLL_NFY*)pPacket;

	CDboEventGenerator::DiceResult(pResult->handle, DICE_WORKD_ID_PARTY_INVEN_DICE, pResult->byRet);
}

void PacketHandler_GSPartyInven_DiceEndNfy(void *pPacket)
{
	// 파티 인벤의 아이템 분배를 위해 파티 주사위 굴리기를 한 결과를 알려준다
	sGU_PARTY_INVEN_DICE_END_NFY* pResult = (sGU_PARTY_INVEN_DICE_END_NFY*)pPacket;

	CDboEventGenerator::PartyInventory(PINVEN_DIVIDE_BY_DICE_RESULT, 0, pResult->hTarget);
}

void PacketHandler_GSPartyCharm_OpenRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_OPEN_CHARM_SLOT_RES );

	// 파티 리더인 자신이 파티 부적창을 열었다
	sGU_PARTY_OPEN_CHARM_SLOT_RES* pResult = (sGU_PARTY_OPEN_CHARM_SLOT_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_LEADER_CHARM_OPEN);
	CDboEventGenerator::PartyCharm(PCET_OPEN, pResult->byCharmInventoryIndex);
}

void PacketHandler_GSPartyCharm_OpenedNfy(void *pPacket)
{
	// 파티 리더가 파티 부적창을 열어서 맴버들도 같이 열린다
	sGU_PARTY_CHARM_SLOT_OPENED_NFY* pResult = (sGU_PARTY_CHARM_SLOT_OPENED_NFY*)pPacket;

	CNtlSLEventGenerator::PartyUpdate(PMT_LEADER_CHARM_OPEN);
	CDboEventGenerator::PartyCharm(PCET_OPEN, pResult->byCharmInventoryIndex);
}

void PacketHandler_GSPartyCharm_CloseRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_CLOSE_CHARM_SLOT_RES );

	// 파티 리더인 자신이 파티 부적창을 닫았다
	sGU_PARTY_CLOSE_CHARM_SLOT_RES* pResult = (sGU_PARTY_CLOSE_CHARM_SLOT_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_LEADER_CHARM_CLOSE);
	CDboEventGenerator::PartyCharm(PCET_CLOSE);
}

void PacketHandler_GSPartyCharm_CloseNfy(void *pPacket)
{
	// 파티 리더가 파티 부적창을 닫아서 맴버들도 같이 닫힌다
	//sGU_PARTY_CHARM_SLOT_CLOSED_NFY* pResult = (sGU_PARTY_CHARM_SLOT_CLOSED_NFY*)pPacket;

	CNtlSLEventGenerator::PartyUpdate(PMT_LEADER_CHARM_CLOSE);
	CDboEventGenerator::PartyCharm(PCET_CLOSE);
}

void PacketHandler_GSPartyCharm_RegVictimItemRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_REGISTER_VICTIM_ITEM_RES );

	// 내가 등록한 파티 부적 아이템의 등록 처리 결과에 따라 아이템 락을 건다.
	sGU_PARTY_REGISTER_VICTIM_ITEM_RES* pResult = (sGU_PARTY_REGISTER_VICTIM_ITEM_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}		

	GetIconMoveManager()->IconMoveEnd();

	// UI에 등록
	RwInt8 iBagIndex = Logic_ConvertContainerTypeToBagIdx(pResult->bySrcPlace);

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlInventory* pInventory = pAvatar->GetInventory();
	SERIAL_HANDLE hBagHandle = pInventory->GetBagItem(iBagIndex);
	CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hBagHandle));

	if( !pBag )
	{
		DBO_FAIL("Not exist sob item of handle : " << hBagHandle);
		return;
	}
	
	SERIAL_HANDLE hItemHandle = pBag->GetChildSerial(pResult->bySrcPos);
	sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromSob(hItemHandle);
	if( !pITEM_TBLDAT )
	{
		DBO_FAIL("Not exist item table of index : " << hItemHandle);
		return;
	}

	CDboEventGenerator::PartyVictim(PVI_REG_CHARM, pITEM_TBLDAT->tblidx,
		pResult->byDestSlotIndex, iBagIndex, pResult->bySrcPos, TRUE);

	// %s님이 %s 등록
	const WCHAR* pwcText = Logic_GetAvatarName();	
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();	
	std::wstring& strItemName = pItemTextTable->GetText(pITEM_TBLDAT->Name);

	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_REG_VICTIM_ITEM, FALSE, NULL, pwcText, strItemName.c_str());
}

void PacketHandler_GSPartyCharm_RegVictimItemNfy(void *pPacket)
{
	// 파티 부적 아이템이 등록이 되었다.
	sGU_PARTY_VICTIM_ITEM_REGISTERED_NFY* pResult = (sGU_PARTY_VICTIM_ITEM_REGISTERED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;	

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember	= reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
		return;
	}

	WCHAR* pwcText = pMember->wszMemberName;

	sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(pResult->itemInfo.itemTblidx);	
	if( !pITEM_TBLDAT )
	{
		DBO_FAIL("Not exist item table of index : " << pResult->itemInfo.itemTblidx);
		return;
	}

	// 부적 등록
	CDboEventGenerator::PartyVictim(PVI_REG_CHARM, pResult->itemInfo.itemTblidx, pResult->itemInfo.bySlotIndex);

	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
	std::wstring& strItemName = pItemTextTable->GetText(pITEM_TBLDAT->Name);
	
	// %s님이 %s 등록
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_REG_VICTIM_ITEM, FALSE, NULL, pwcText, strItemName.c_str());
}

void PacketHandler_GSPartyCharm_UnRegVictimItemRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_UNREGISTER_VICTIM_ITEM_RES );

	// 내가 등록했던 파티 부적 또는 제물 아이템의 등록 해제 결과에 따라 아이템 락을 해제한다.
	sGU_PARTY_UNREGISTER_VICTIM_ITEM_RES* pResult = (sGU_PARTY_UNREGISTER_VICTIM_ITEM_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	// 파티 부적 제물용 아이템 해제
	CDboEventGenerator::PartyVictim(PVI_UNREG_CHARM, pResult->byDestSlotIndex);	
}

void PacketHandler_GSPartyCharm_UnRegVictimItemNfy(void *pPacket)
{
	// 파티 부적 또는 제물 아이템이 해제되었다.
	sGU_PARTY_VICTIM_ITEM_UNREGISTERED_NFY* pResult = (sGU_PARTY_VICTIM_ITEM_UNREGISTERED_NFY*)pPacket;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;	

	CNtlParty*		pParty	= pAvatar->GetParty();
	sPartyMember*	pMember	= reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
	if( !pMember )
	{
		DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
		return;
	}	

	// %s님이 %s 등록 취소(부적 동일)
	sITEM_TBLDAT* pItem_TBLDAT = Logic_GetItemDataFromTable(pResult->itemTblidx);
	if( !pItem_TBLDAT )
	{
		DBO_FAIL("Not exist item table of handle : " << pResult->itemTblidx);
		return;
	}

	WCHAR* pwcText = pMember->wszMemberName;

	// 부적 아이템 해제
	CDboEventGenerator::PartyVictim(PVI_UNREG_CHARM, pResult->byDestSlotIndex);	

	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
	std::wstring& strItemName = pItemTextTable->GetText(pItem_TBLDAT->Name);
	GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_UNREG_VICTIM_ITEM, FALSE, NULL, pwcText, strItemName.c_str());
}

void PacketHandler_GSPartyCharm_DeclareZennyRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_DECLARE_ZENNY_RES );

	// 자신이 부적 사용을 위해 제니를 등록했다
	sGU_PARTY_DECLARE_ZENNY_RES* pResult = (sGU_PARTY_DECLARE_ZENNY_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	GetIconMoveManager()->IconMoveEnd();

	if( pResult->dwInitialZenny == pResult->dwFinalDeclaredZenny )
	{
		GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_DECLARE_ZENNY, FALSE, NULL, pResult->dwFinalDeclaredZenny);
	}
	else
	{
		GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_DECLARE_ZENNY_PART, FALSE, NULL, pResult->dwInitialZenny, pResult->dwFinalDeclaredZenny);
	}

	CDboEventGenerator::PartyVictim(PVI_ZENNY, Logic_GetAvatarHandle(), pResult->dwFinalDeclaredZenny);
}

void PacketHandler_GSPartyCharm_DeclareZennyNfy(void *pPacket)
{
	// 파티원이 부적 사용을 위해 제니를 등록했다
	sGU_PARTY_DECLARED_ZENNY_CHANGED_NFY* pResult = (sGU_PARTY_DECLARED_ZENNY_CHANGED_NFY*)pPacket;
	CDboEventGenerator::PartyVictim(PVI_ZENNY, pResult->hMember, pResult->dwDeclaredZenny);
}

void PacketHandler_GSPartyCharm_Activation_Failed_Nfy(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_ACTIVATE_CHARM_RES );

	// 파티 리더에게 파티참 발동 실패를 알린다
	sGU_PARTY_CHARM_ACTIVATION_FAILED_NFY* pResult = (sGU_PARTY_CHARM_ACTIVATION_FAILED_NFY*)pPacket;
	
	if( pResult->hMember == INVALID_SERIAL_ID )
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
	else
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
			return;	

		CNtlParty*		pParty	= pAvatar->GetParty();
		sPartyMember*	pMember	= reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pResult->hMember) );
		if( !pMember )
		{
			DBO_FAIL("Not exist party member of handle : " << pResult->hMember);
			return;
		}

		GetAlarmManager()->FormattedAlarmMessage(pResult->wResultCode, TRUE, NULL, pMember->wszMemberName);
	}
}

void PacketHandler_GSPartyCharm_ActivateRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_ACTIVATE_CHARM_RES );

	// 파티 리더에게 파티 부적 발동 결과를 알려준다
	sGU_PARTY_ACTIVATE_CHARM_RES* pResult = (sGU_PARTY_ACTIVATE_CHARM_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::PartyCharm(PCET_ACTIVATE, pResult->byCharmInventoryIndex, pResult->registeredCharmTblidx);

	CNtlSLEventGenerator::PartyCharmActivity(PCAT_ACTIVE, pResult->byCharmInventoryIndex, pResult->registeredCharmTblidx, (RwReal)pResult->dwRemainingTime);
}

void PacketHandler_GSPartyCharm_ActivateNfy(void *pPacket)
{
	// 파티 부적이 발동되었음을 알려준다
	sGU_PARTY_CHARM_ACTIVATED_NFY* pResult = (sGU_PARTY_CHARM_ACTIVATED_NFY*)pPacket;

	CDboEventGenerator::PartyCharm(PCET_ACTIVATE, pResult->byCharmInventoryIndex, pResult->registeredCharmTblidx);

	CNtlSLEventGenerator::PartyCharmActivity(PCAT_ACTIVE, pResult->byCharmInventoryIndex, pResult->registeredCharmTblidx, (RwReal)pResult->dwRemainingTime);
}

void PacketHandler_GSPartyCharm_DeActivateCharmRes(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_DEACTIVATE_CHARM_RES );

	// 파티 리더에게 파티 버프가 취소되었음을 알린다
	sGU_PARTY_DEACTIVATE_CHARM_RES* pResult = (sGU_PARTY_DEACTIVATE_CHARM_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CDboEventGenerator::PartyCharm(PCET_UNDISPLAY_CHARM_BUFF, pResult->byCharmInventoryIndex);

	CNtlSLEventGenerator::PartyCharmActivity(PCAT_RELEASE, pResult->byCharmInventoryIndex);
}

void PacketHandler_GSPartyCharm_DeActivateCharmNfy(void *pPacket)
{
	// 파티 맴버들에게 파티 버프가 취소되었음을 알린다
	sGU_PARTY_CHARM_DEACTIVATED_NFY* pResult = (sGU_PARTY_CHARM_DEACTIVATED_NFY*)pPacket;

	CDboEventGenerator::PartyCharm(PCET_UNDISPLAY_CHARM_BUFF, pResult->byCharmInventoryIndex);

	CNtlSLEventGenerator::PartyCharmActivity(PCAT_RELEASE, pResult->byCharmInventoryIndex);
}

void PacketHandler_GSPartyCharm_DiceStart_Nfy(void* pPacket)
{
	//sGU_PARTY_CHARM_DEACTIVATED_NFY* pResult = (sGU_PARTY_CHARM_DEACTIVATED_NFY*)pPacket;

	CDboEventGenerator::PartyVictim(PVI_START_CHARM_DICE);	
}

void PacketHandler_GSPartyCharm_DiceRolled_Res(void* pPacket)
{
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_CHARM_DICE_ROLLED_RES );

	sGU_PARTY_CHARM_DICE_ROLLED_RES* pResult = (sGU_PARTY_CHARM_DICE_ROLLED_RES*)pPacket;

	CDboEventGenerator::DiceResult(Logic_GetAvatarHandle(), DICE_WORKD_ID_PARTY_CHARM_DICE, (RwUInt8)pResult->dwRet);
}

void PacketHandler_GSPartyCharm_DiceRolled_Nfy(void* pPacket)
{
	sGU_PARTY_CHARM_DICE_ROLLED_NFY* pResult = (sGU_PARTY_CHARM_DICE_ROLLED_NFY*)pPacket;

	CDboEventGenerator::DiceResult(pResult->handle, DICE_WORKD_ID_PARTY_CHARM_DICE, (RwUInt8)pResult->dwRet);
}

void PacketHandler_GSPartyCharm_DiceEnd_Nfy(void* pPacket)
{
	sGU_PARTY_CHARM_DICE_END_NFY* pResult = (sGU_PARTY_CHARM_DICE_END_NFY*)pPacket;

	if( pResult->bIsSuccess )
		CDboEventGenerator::FlashNotifyString("Lucky.swf");
	else
		GetAlarmManager()->AlarmMessage(DST_PARTY_CHARM_START_DICE_RESULT_FAIL);
}

// 파티 공유 타겟 요청에 대한 결과 패킷
void PacketHandler_GSPartyShareTargetRes( void* pPacket ) 
{
    sGU_PARTY_SHARETARGET_RES* pData = (sGU_PARTY_SHARETARGET_RES*)pPacket;
    if( pData->wResultCode != GAME_SUCCESS )
    {
        GetAlarmManager()->AlarmMessage( pData->wResultCode, TRUE );
        return;
    }

    CNtlSLEventGenerator::PartyShareTargetRes(pData->wResultCode);
}

// 파티 공유 타겟 선택에 대한 알림 패킷
void PacketHandler_GSPartyShareTargetNfy( void* pPacket ) 
{
    sGU_PARTY_SHARETARGET_NFY* pData = (sGU_PARTY_SHARETARGET_NFY*)pPacket;

    CNtlSLEventGenerator::PartyShareTargetNfy(pData->sharetargetInfo);
}

void PacketHandler_GSPartyDungeonDiffRes(void *pPacket)
{
	// 파티장에게 파티 던전의 난이도 변경 결과를 알린다
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_DUNGEON_DIFF_RES );

	sGU_PARTY_DUNGEON_DIFF_RES* pResult = (sGU_PARTY_DUNGEON_DIFF_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_PARTY_DUNGEON_STATE, INVALID_SERIAL_ID, pResult->eDiff);	

	GetAlarmManager()->AlarmMessage(DST_PARTY_DUNGEON_CHANGE_NORMAL + pResult->eDiff);
}

void PacketHandler_GSPartyDungeonDiffNfy(void *pPacket)
{
	// 파티원들에게 파티 던전 난이도 변경을 알린다
	sGU_PARTY_DUNGEON_DIFF_NFY* pResult = (sGU_PARTY_DUNGEON_DIFF_NFY*)pPacket;

	CNtlSLEventGenerator::PartyUpdate(PMT_PARTY_DUNGEON_STATE, INVALID_SERIAL_ID, pResult->eDiff);	
	
	GetAlarmManager()->AlarmMessage(DST_PARTY_DUNGEON_CHANGE_NORMAL + pResult->eDiff);
}

void PacketHandler_GSPartyDungeonInitRes(void *pPacket)
{
	// 파티장에게 파티 던전 초기화 결과를 알린다
	API_GetSLPacketLockManager()->Unlock( GU_PARTY_DUNGEON_INIT_RES );

	sGU_PARTY_DUNGEON_INIT_RES* pResult = (sGU_PARTY_DUNGEON_INIT_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage( pResult->wResultCode, TRUE );
		return;
	}

	CNtlSLEventGenerator::PartyUpdate(PMT_PARTY_DUNGEON_INITIALIZE);

	GetAlarmManager()->AlarmMessage(DST_PARTY_DUNGEON_INITIALIZE);
}

void PacketHandler_GSPartyDungeonInitNfy(void *pPacket)
{
	// 파티원에게 파티 던전 초기화를 알린다
	//sGU_PARTY_DUNGEON_INIT_NFY* pResult = (sGU_PARTY_DUNGEON_INIT_NFY*)pPacket;

	GetAlarmManager()->AlarmMessage(DST_PARTY_DUNGEON_INITIALIZE);
}