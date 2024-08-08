/*****************************************************************************
* File			: DboPackethandler_GUTMQ.cpp
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 16
* Abstract		: Time Machine Quest 관련 패킷 핸들
*****************************************************************************
* Desc         : 
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboPacketHandler.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSLPacketGenerator.h"
#include "NtlWorldConceptTutorial.h"
#include "NtlWorldConceptTMQ.h"

// dbo
#include "DboTSCTAgency.h"
#include "DboTSCQAgency.h"


void PacketHandler_GUTimeQuest_Room_List_Res(void *pPacket)
{
	// TMQ 게시판의 정보를 알려준다
	API_GetSLPacketLockManager()->Unlock(GU_TIMEQUEST_ROOM_LIST_RES);

	sGU_TIMEQUEST_ROOM_LIST_RES* pResult = (sGU_TIMEQUEST_ROOM_LIST_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CDboEventGenerator::DialogEvent(DIALOGEVENT_OPEN_FAIL_NPC_DIALOG, PLACE_NONE, DIALOG_TMQBOARD);
		GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);
		return;
	}

	CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_LIST, NULL, NULL, (VOID*)&pResult->sTMQInfo);
}

void PacketHandler_GUTimeQuest_Room_Info_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_TIMEQUEST_ROOM_INFO_RES);

	// 해당 TMQ 방 정보를 알려준다
	sGU_TIMEQUEST_ROOM_INFO_RES* pResult = (sGU_TIMEQUEST_ROOM_INFO_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);
		return;
	}

	CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_INFO, NULL, pResult->byRoomState, (VOID*)&pResult->sIconInfo);
}

void PacketHandler_GUTimeQuest_Room_Join_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_TIMEQUEST_ROOM_JOIN_RES);

	// 예약 신청 결과를 알려준다
	sGU_TIMEQUEST_ROOM_JOIN_RES* pResult = (sGU_TIMEQUEST_ROOM_JOIN_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);
		return;
	}

	CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_RESERVATION, NULL, NULL, (VOID*)&pResult->sJoinInfo);
}

void PacketHandler_GUTimeQuest_Room_Join_Nfy(void *pPacket)
{
	// 1. 예약 신청 결과를 일행에게 알려준다
	// 2. 새로 들어온 그룹 맴버에게 TMQ 정보를 알려준다
	sGU_TIMEQUEST_ROOM_JOIN_NFY* pResult = (sGU_TIMEQUEST_ROOM_JOIN_NFY*)pPacket;

	// 파티 등록인 경우 파티원에게 알려준다
	CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_RESERVATION_NFY, NULL, NULL, (VOID*)&pResult->sJoinInfo);
}

void PacketHandler_GUTimeQuest_Room_Leave_Res(void *pPacket)
{
	API_GetSLPacketLockManager()->Unlock(GU_TIMEQUEST_ROOM_LEAVE_RES);

	// TMQ 예약을 해제한 결과를 알려준다
	sGU_TIMEQUEST_ROOM_LEAVE_RES* pResult = (sGU_TIMEQUEST_ROOM_LEAVE_RES*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);
		return;
	}

	CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_LEAVE);
}

void PacketHandler_GUTimeQuest_Room_Leave_Nfy(void *pPacket)
{
	// TMQ 예약을 해제한 결과를 일행에게 알려준다
	//sGU_TIMEQUEST_ROOM_LEAVE_NFY* pResult = (sGU_TIMEQUEST_ROOM_LEAVE_NFY*)pPacket;

	CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_LEAVE);
}

void PacketHandler_GUTimeQuest_Room_Selection_Nfy(void *pPacket)
{
	// TMQ 추첨 여부
	sGU_TIMEQUEST_ROOM_SELECTION_NFY* pResult = (sGU_TIMEQUEST_ROOM_SELECTION_NFY*)pPacket;

	if( pResult->bIsSecondWinner )
	{
		// TMQ 추첨이 성공하였다
		CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_SUCCESS_SELECTION, NULL, NULL, (VOID*)&pResult->uSelectionInfo);
	}
	else
	{
		// 다음 TMQ를 재신청할지를 물어본다
		CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_ASK_RETRY, NULL, NULL, (VOID*)&pResult->uSelectionInfo);
	}	
}

void PacketHandler_GUTimeQuest_Room_Teleport_Nfy(void *pPacket)
{
	// TMQ 텔레포트
	API_GetSLPacketLockManager()->Unlock(GU_TIMEQUEST_ROOM_TELEPORT_RES);

	sGU_TIMEQUEST_ROOM_TELEPORT_RES* pResult = (sGU_TIMEQUEST_ROOM_TELEPORT_RES*)pPacket;

	if(pResult->wResultCode != GAME_SUCCESS)
	{
		GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);
		return;
	}

	CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_TELEPORT);
}

void PacketHandler_GUTimeQuest_Room_Start_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_START_NFY* pResult = (sGU_TIMEQUEST_START_NFY*)pPacket;

	if( pResult->wResultCode != GAME_SUCCESS )
	{
		CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_LEAVE);
		GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);
		return;
	}

	CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_LAUNCHED_TM);
}

void PacketHandler_GUTimeQuest_Room_Start_Exception_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_START_EXCEPTION_NFY* pResult = (sGU_TIMEQUEST_START_EXCEPTION_NFY*)pPacket;

	GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);

	CNtlSLEventGenerator::SLTMQBoard(TBT_ROOM_SERVER_EXCEPTION);
}

void PacketHandler_GUTimeQuest_Leave_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_LEAVE_NFY* pResult = (sGU_TIMEQUEST_LEAVE_NFY*)pPacket;

	if( pResult->wResultCode == GAME_SUCCESS )
		return;

	if( pResult->handle == INVALID_SERIAL_ID )
	{
		GetAlarmManager()->AlarmMessage(pResult->wResultCode, TRUE);
	}
	else
	{
		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
		NTL_ASSERT(pTMQ->GetTMQMode() == GAMERULE_TIMEQUEST, "PacketHandler_GUTimeQuest_Leave_Nfy, wrong world rule type : " << pTMQ->GetTMQMode());
		sPartyMember* pSM_Member = reinterpret_cast<sPartyMember*>( pTMQ->GetMemberbyKey(pResult->handle) );
		NTL_ASSERT(pSM_Member, "PacketHandler_GUTimeQuest_Leave_Nfy, not exist member of handle : " << pResult->handle);

		if( pSM_Member )
			GetAlarmManager()->FormattedAlarmMessage(DST_TMQ_BALLOON_LEAVE_MEMVER, FALSE, NULL, pSM_Member->wszMemberName);
	}
}

void PacketHandler_GUTimeQuest_Reward_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_REWARD_NFY* pResult = (sGU_TIMEQUEST_REWARD_NFY*)pPacket;

	// TMQ 정보 갱신
	CNtlSLEventGenerator::SLTMQEvent(TET_CLEAR_TIME, INVALID_SERIAL_ID, pResult->dwClearTime);
	CNtlSLEventGenerator::SLTMQEvent(TET_BONUS_TIME, INVALID_SERIAL_ID, pResult->dwBonusTime);
	CNtlSLEventGenerator::SLTMQEvent(TET_DAY_RECORD, INVALID_SERIAL_ID, pResult->bIsDayRecord);
	CNtlSLEventGenerator::SLTMQEvent(TET_IS_BEST_RECORD, INVALID_SERIAL_ID, pResult->bIsBestRecord);

	CNtlSLEventGenerator::SLTMQNotify(TET_TIME_UNTIL_TELEPORT_TO_MAINWORLD, pResult->dwWaitTime);

	// 트리거 실행
	switch ( pResult->byTriggerType )
	{
	case TS_TYPE_QUEST_CS:
		{
			CDboTSCQAgency* pQuestAgency = GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency();
			if ( pQuestAgency )
				pQuestAgency->GU_TS_Update_Event_Nfy( pResult->teid );
		}
		break;

	case TS_TYPE_PC_TRIGGER_CS:
		{
			CDboTSCTAgency* pTriggerAgency = GetNtlSLGlobal()->GetSobAvatar()->GetPCTriggerAgency();
			if ( pTriggerAgency )
				pTriggerAgency->GU_TS_Update_Event_Nfy( pResult->teid );
		}
		break;

	default:
		{
			NTL_ASSERTFAIL( "Not supported trigger type" );
		}
	}
}

void PacketHandler_GUTimeQuest_Infomation_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_INFORMATION_NFY* pResult = (sGU_TIMEQUEST_INFORMATION_NFY*)pPacket;

	CNtlSLEventGenerator::SLTMQNotify(TET_INFOMATION_NFY, pResult->byInfoType);
}

void PacketHandler_GUTimeQuest_Countdown_Nfy(void *pPacket)
{
	sGU_TIMEQUEST_COUNTDOWN_NFY* pResult = (sGU_TIMEQUEST_COUNTDOWN_NFY*)pPacket;

	CNtlSLEventGenerator::SLTMQEvent(TBT_COUNTDOWN, INVALID_SERIAL_ID, pResult->bCountDown);
}

void PacketHandler_GUTimeQuest_GameState(void *pPacket)
{
	sGU_TIMEQUEST_UPDATE_GAME_STATE* pResult = (sGU_TIMEQUEST_UPDATE_GAME_STATE*)pPacket;

	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
	NTL_ASSERT(pWorldConcept, "PacketHandler_GUTimeQuest_GameState, Wrong first grade world concept");

	if( pWorldConcept->GetConceptType() == WORLD_PLAY_TIME_MACHINE )
	{
		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
		CNtlWorldConceptTMQ* pTMQConcept = reinterpret_cast<CNtlWorldConceptTMQ*>(pWorldConcept);

		pTMQ->SetStage(pResult->sTimeQuestState.sTimeQuestStage.byStageNumber);
		pTMQConcept->SetTMQState((eTIMEQUEST_GAME_STATE)pResult->sTimeQuestState.byGameState);
	}
	else if( pWorldConcept->GetConceptType() == WORLD_PLAY_TUTORIAL )
	{
		CNtlWorldConceptTutorial* pTutorialConcept = reinterpret_cast<CNtlWorldConceptTutorial*>(pWorldConcept);

		pTutorialConcept->SetTMQState((eTIMEQUEST_GAME_STATE)pResult->sTimeQuestState.byGameState);
	}
	else
	{
		NTL_ASSERT(false, "PacketHandler_GUTimeQuest_GameState, wrong world concept : " << pWorldConcept->GetConceptType());
	}
}

void PacketHandler_GUTimeQuest_CouponCount(void *pPacket)
{
	// TMQ 쿠폰 정보
	sGU_TIMEQUEST_UPDATE_COUPON_COUNT* pResult = (sGU_TIMEQUEST_UPDATE_COUPON_COUNT*)pPacket;

	CNtlSLEventGenerator::SLTMQEvent(TET_COUPON_COUNT, INVALID_SERIAL_ID, pResult->byCouponCount);
}

void PacketHandler_GUTimeQuest_TMQPoint(void *pPacket)
{
	// TMQ 포인트
	sGU_TIMEQUEST_UPDATE_TMQ_POINT* pResult = (sGU_TIMEQUEST_UPDATE_TMQ_POINT*)pPacket;

	CNtlSLEventGenerator::SLTMQEvent(TET_TMQ_POINT, INVALID_SERIAL_ID, pResult->wTmqPoint);
}

void PacketHandler_GUTimeQuest_GameTime(void *pPacket)
{
	sGU_TIMEQUEST_UPDATE_GAME_TIME* pResult = (sGU_TIMEQUEST_UPDATE_GAME_TIME*)pPacket;

	CNtlSLEventGenerator::SLTMQEvent(TET_GAME_TIME, INVALID_SERIAL_ID, pResult->dwBonusTime);
}