#include "precomp_ntlsimulation.h"
#include "NtlSLPacketGenerator.h"

// shared
#include "NtlClientNet.h"
#include "NtlPacketUG.h"
#include "NtlPacketGU.h"

// core
#include "NtlDebug.h"

#include "NtlPLApi.h"
// simulation
#include "NtlNetSender.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSLPacketGenerator.h"


CNtlSLPacketGenerator::CNtlSLPacketGenerator(CNtlNetSender *pNetSender)
:CNtlPacketGenerator(pNetSender)
{

}

bool CNtlSLPacketGenerator::SendCharSpawnReady(void)
{
	sUG_CHAR_READY_TO_SPAWN sPacket;

	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CHAR_READY_TO_SPAWN;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CNtlSLPacketGenerator::SendCharReady(RwUInt8 byAvatarType)
{
	sUG_CHAR_READY sPacket;

	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_CHAR_READY;
	sPacket.byAvatarType = byAvatarType;	

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CNtlSLPacketGenerator::SendPetAttackTargetNfy( RwUInt8 byAvatarType )
{
	sUG_PET_ATTACK_TARGET_NFY sPacket;

	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PET_ATTACK_TARGET_NFY;
	sPacket.byAvatarType = byAvatarType;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendPetDismissReq(RwUInt8 byAvatarType)
{
	sUG_PET_DISMISS_PET_REQ sPacket;

	memset(&sPacket, 0, sizeof(sPacket));

	sPacket.wOpCode = UG_PET_DISMISS_PET_REQ;
	sPacket.byAvatarType = byAvatarType;	

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);  
}

bool CNtlSLPacketGenerator::SendJump(RwV3d vDir)
{
	sUG_CHAR_JUMP sPacket;

	memset(&sPacket, 0, sizeof(sUG_CHAR_JUMP));

	sPacket.wOpCode = UG_CHAR_JUMP;

	sPacket.vCurrentHeading.x = vDir.x;
	sPacket.vCurrentHeading.y = vDir.y;
	sPacket.vCurrentHeading.z = vDir.z;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendChangeDirectionFloating(RwUInt8 byMoveDirection, RwV3d vDir)
{
	sUG_CHAR_CHANGE_DIRECTION_ON_FLOATING sPacket;

	memset(&sPacket, 0, sizeof(sUG_CHAR_CHANGE_DIRECTION_ON_FLOATING));

	vDir.y = 0.0f;
	RwV3dNormalize(&vDir, &vDir);

	sPacket.wOpCode			= UG_CHAR_CHANGE_DIRECTION_ON_FLOATING;
	sPacket.dwTimeStamp		= 0;
	sPacket.vCurDir.x		= vDir.x;
	sPacket.vCurDir.y		= vDir.y;
	sPacket.vCurDir.z		= vDir.z;
	sPacket.byMoveDirection	= byMoveDirection;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendJumpEnd(void)
{
	sUG_CHAR_JUMP_END sPacket;

	memset(&sPacket, 0, sizeof(sUG_CHAR_JUMP_END));

	sPacket.wOpCode = UG_CHAR_JUMP_END;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharging(void)
{
	sUG_CHAR_CHARGE sPacket;

	memset(&sPacket, 0, sizeof(sUG_CHAR_CHARGE));

	sPacket.wOpCode = UG_CHAR_CHARGE;
	sPacket.bCharge = true;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendChargingEnd(void)
{
	sUG_CHAR_CHARGE sPacket;

	memset(&sPacket, 0, sizeof(sUG_CHAR_CHARGE));

	sPacket.wOpCode = UG_CHAR_CHARGE;
	sPacket.bCharge = false;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendBlocking(void)
{
	sUG_CHAR_BLOCK_MODE sPacket;

	memset(&sPacket, 0, sizeof(sUG_CHAR_BLOCK_MODE));

	sPacket.wOpCode = UG_CHAR_BLOCK_MODE;
	sPacket.bIsOn = true;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendBlockingEnd(void)
{
	sUG_CHAR_BLOCK_MODE sPacket;

	memset(&sPacket, 0, sizeof(sUG_CHAR_BLOCK_MODE));

	sPacket.wOpCode = UG_CHAR_BLOCK_MODE;
	sPacket.bIsOn = false;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendSitAndStand(RwBool bSitDown)
{
	sUG_CHAR_TOGG_SITDOWN sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_TOGG_SITDOWN));

	sPacket.wOpCode		= UG_CHAR_TOGG_SITDOWN;
	sPacket.bSitDown	= B2b(bSitDown);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendFalling(RwBool bFalling, RwV3d vCurrPos, RwV3d vCurrDir, RwUInt8 byMoveDirection)
{
	sUG_CHAR_FALLING sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_FALLING));

	sPacket.wOpCode			= UG_CHAR_FALLING;
	sPacket.bIsFalling		= B2b(bFalling);

	sPacket.vCurLoc.x		= vCurrPos.x;
	sPacket.vCurLoc.y		= vCurrPos.y;
	sPacket.vCurLoc.z		= vCurrPos.z;

	vCurrDir.y = 0.0f;
	RwV3dNormalize(&vCurrDir, &vCurrDir);

	sPacket.vCurDir.x		= vCurrDir.x;
	sPacket.vCurDir.z		= vCurrDir.z;

	sPacket.byMoveDirection = byMoveDirection;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendMouseMove(RwV3d vCurr, RwV3d vDest, RwUInt8 byAvatarType)
{
	sUG_CHAR_DEST_MOVE sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_DEST_MOVE));

	sPacket.wOpCode = UG_CHAR_DEST_MOVE;
	sPacket.dwTimeStamp = 0;
	sPacket.byAvatarType = byAvatarType;

	sPacket.vCurLoc.x = vCurr.x;
	sPacket.vCurLoc.y = vCurr.y;
	sPacket.vCurLoc.z = vCurr.z;

	sPacket.vDestLoc.x = vDest.x;
	sPacket.vDestLoc.y = vDest.y;
	sPacket.vDestLoc.z = vDest.z;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendKeyboardMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveFlag, RwUInt8 byAvatarType)
{
	sUG_CHAR_MOVE sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_MOVE));

	sPacket.wOpCode = UG_CHAR_MOVE;
	sPacket.dwTimeStamp = 0;
	sPacket.byAvatarType = byAvatarType;
	sPacket.vCurLoc.x = vCurr.x;
	sPacket.vCurLoc.y = vCurr.y;
	sPacket.vCurLoc.z = vCurr.z;
	vDir.y = 0.0f;
	RwV3dNormalize(&vDir, &vDir);
	sPacket.vCurDir.x = vDir.x;
	sPacket.vCurDir.z = vDir.z;
	sPacket.byMoveDirection = byMoveFlag;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}


bool CNtlSLPacketGenerator::SendMouseDashMove(RwV3d vDest)
{
	sUG_CHAR_DASH_MOUSE sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_DASH_MOUSE));

	sPacket.wOpCode = UG_CHAR_DASH_MOUSE;
	sPacket.dwTimeStamp = 0;
	sPacket.vDestLoc.x = vDest.x;
	sPacket.vDestLoc.y = vDest.y;
	sPacket.vDestLoc.z = vDest.z;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}


bool CNtlSLPacketGenerator::SendKeyboardDashMove(RwV3d vCurr, RwV3d vDir, RwUInt8 byMoveFlag)
{
	sUG_CHAR_DASH_KEYBOARD sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_DASH_KEYBOARD));

	sPacket.wOpCode = UG_CHAR_DASH_KEYBOARD;
	sPacket.dwTimeStamp = 0;
	sPacket.vCurLoc.x = vCurr.x;
	sPacket.vCurLoc.y = vCurr.y;
	sPacket.vCurLoc.z = vCurr.z;
	vDir.y = 0.0f;
	RwV3dNormalize(&vDir, &vDir);
	sPacket.vCurDir.x = vDir.x;
	sPacket.vCurDir.z = vDir.z;
	sPacket.byMoveDirection = byMoveFlag;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharToggleFighting(RwBool bFightingMode, RwUInt8 byAvatarType)
{
	sUG_CHAR_TOGG_FIGHTING sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_TOGG_FIGHTING));

	sPacket.wOpCode = UG_CHAR_TOGG_FIGHTING;
	sPacket.byAvatarType = byAvatarType;

	if(bFightingMode)
		sPacket.bFightMode = true;
	else
		sPacket.bFightMode = false;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharAttackBegin(RwUInt8 byType, RwUInt8 byAvatarType)
{
	sUG_CHAR_ATTACK_BEGIN sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_ATTACK_BEGIN));

	sPacket.wOpCode = UG_CHAR_ATTACK_BEGIN;

	sPacket.byAvatarType = byAvatarType;
	sPacket.byType	= byType;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharAttackEnd(RwUInt8 byType, RwUInt8 byAvatarType)
{
	sUG_CHAR_ATTACK_END sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_ATTACK_END));

	sPacket.wOpCode = UG_CHAR_ATTACK_END;

	sPacket.byAvatarType = byAvatarType;
	sPacket.byType	= byType;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendChangePosition(RwV3d vCurr, RwV3d vDir, RwUInt8 byAvatarType)
{
	sUG_CHAR_MOVE_SYNC sPacket;

	memset(&sPacket, 0, sizeof(sUG_CHAR_MOVE_SYNC));

	sPacket.wOpCode = UG_CHAR_MOVE_SYNC;

	sPacket.dwTimeStamp = 0;
	sPacket.byAvatarType = byAvatarType;

	sPacket.vCurLoc.x = vCurr.x;
	sPacket.vCurLoc.y = vCurr.y;
	sPacket.vCurLoc.z = vCurr.z;

	vDir.y = 0.0f;
	RwV3dNormalize(&vDir, &vDir);

	sPacket.vCurDir.x = vDir.x;
	sPacket.vCurDir.y = vDir.y;
	sPacket.vCurDir.z = vDir.z;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendChangeHeading(RwV3d vCurr, RwV3d vDir)
{
	sUG_CHAR_CHANGE_HEADING sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_CHANGE_HEADING));

	sPacket.wOpCode = UG_CHAR_CHANGE_HEADING;

	sPacket.vCurrentPosition.x = vCurr.x;
	sPacket.vCurrentPosition.y = vCurr.y;
	sPacket.vCurrentPosition.z = vCurr.z;

	vDir.y = 0.0f;
	RwV3dNormalize(&vDir, &vDir);

	sPacket.vCurrentHeading.x = vDir.x;
	sPacket.vCurrentHeading.y = vDir.y;
	sPacket.vCurrentHeading.z = vDir.z;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}


bool CNtlSLPacketGenerator::SendCharTargetSelect(SERIAL_HANDLE hSerialId)
{
	sUG_CHAR_TARGET_SELECT sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_TARGET_SELECT));

	sPacket.wOpCode = UG_CHAR_TARGET_SELECT;

	sPacket.byAvatarType = DBO_AVATAR_TYPE_AVATAR;
	sPacket.hTarget = hSerialId;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharTargetFacing(SERIAL_HANDLE hSerialId)
{
	sUG_CHAR_TARGET_FACING sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_TARGET_FACING));

	sPacket.wOpCode = UG_CHAR_TARGET_FACING;
	sPacket.hTarget = hSerialId;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharFollowMove(SERIAL_HANDLE hTargetSerialId, RwReal fFollowRange, RwUInt8 byMovementReason, RwUInt8 byAvatarType)
{
	sUG_CHAR_FOLLOW_MOVE sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_FOLLOW_MOVE));

	sPacket.wOpCode = UG_CHAR_FOLLOW_MOVE;

	sPacket.byAvatarType = byAvatarType;
	sPacket.hTarget = hTargetSerialId;
	sPacket.fDistance = fFollowRange;
	sPacket.byMovementReason = byMovementReason;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharSkillReq(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerialId, RwUInt8 bySlotIdx, RwUInt8 byRpBonusType, RwUInt8 byAvatarType)
{
	sUG_CHAR_SKILL_REQ sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_SKILL_REQ));

	sPacket.wOpCode			= UG_CHAR_SKILL_REQ;

	sPacket.byAvatarType	= byAvatarType;
	sPacket.bySlotIndex		= bySlotIdx;
	sPacket.byRpBonusType	= byRpBonusType;
	sPacket.hTarget			= hTargetSerialId;

	RwInt32 iIdx = 0;
	std::list<SERIAL_HANDLE>::iterator it;
	std::list<SERIAL_HANDLE> listApplyActor;

	Logic_GetAvatarSkillApplyArea(pActor, hTargetSerialId, bySlotIdx, listApplyActor);

	sPacket.byApplyTargetCount = (RwUInt8)listApplyActor.size();

	for(it = listApplyActor.begin(); it != listApplyActor.end(); it++)
	{
		sPacket.ahApplyTarget[iIdx] = (*it);
		iIdx++;
	}

	Logic_SetSkillReqFlag(TRUE);

	RwInt32 iDataLen = sizeof(sPacket) - sizeof(HOBJECT)*(NTL_MAX_NUMBER_OF_SKILL_TARGET - sPacket.byApplyTargetCount); 

	return m_pNetSender->SendPacket(iDataLen, &sPacket);
}

bool CNtlSLPacketGenerator::SendCharSkillTargetList(RwUInt8 byAvatarType)
{
	sUG_SKILL_TARGET_LIST sPacket;
	memset(&sPacket, 0, sizeof(sUG_SKILL_TARGET_LIST));

	sPacket.wOpCode = UG_SKILL_TARGET_LIST;
	sPacket.byAvatarType = byAvatarType;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlBeCharData *pBeCharData = reinterpret_cast<CNtlBeCharData*>( pSobAvatar->GetBehaviorData() );
	SSkillStuff *pSkillStuff = pBeCharData->GetSkillStuff();

	std::list<SERIAL_HANDLE>::iterator it;
	std::list<SERIAL_HANDLE> listApplyActor;
	Logic_GetAvatarSkillApplyArea(pSkillStuff->hTargetSerialId, pSkillStuff->uiSkillTblId, listApplyActor);

	RwInt32 iIdx = 0;
	sPacket.byApplyTargetCount = (RwUInt8)listApplyActor.size();
	for(it = listApplyActor.begin(); it != listApplyActor.end(); it++)
	{
		sPacket.ahApplyTarget[iIdx] = (*it);
		iIdx++;
	}

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharSocialReq(RwUInt32 uiTblId)
{
	sUG_SOCIAL_ACTION sPacket;
	memset(&sPacket, 0, sizeof(sUG_SOCIAL_ACTION));

	sPacket.wOpCode = UG_SOCIAL_ACTION;
	sPacket.socialActionId = uiTblId;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharHTBReq(SERIAL_HANDLE hTargetSerialId, RwUInt8 bySlotIdx)
{
	sUG_HTB_START_REQ sPacket;
	memset(&sPacket, 0, sizeof(sUG_HTB_START_REQ));

	sPacket.wOpCode = UG_HTB_START_REQ;
	sPacket.hTarget = hTargetSerialId;
	sPacket.bySkillSlot = bySlotIdx;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharHTBForward(RwV3d vCurr, RwV3d vDir)
{
	sUG_HTB_FORWARD_REQ sPacket;
	memset(&sPacket, 0, sizeof(sUG_HTB_FORWARD_REQ));

	sPacket.wOpCode = UG_HTB_FORWARD_REQ;

	sPacket.vCurLoc.x = vCurr.x;
	sPacket.vCurLoc.y = vCurr.y;
	sPacket.vCurLoc.z = vCurr.z;

	vDir.y = 0.0f;
	RwV3dNormalize(&vDir, &vDir);

	sPacket.vCurDir.x = vDir.x;
	sPacket.vCurDir.z = vDir.z;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharSocialAction(RwUInt32 uiTblIdx)
{
	sUG_SOCIAL_ACTION sPacket;
	memset(&sPacket, 0, sizeof(sUG_SOCIAL_ACTION));

	sPacket.wOpCode = UG_SOCIAL_ACTION;

	sPacket.socialActionId = uiTblIdx;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharItemUseReq(SERIAL_HANDLE hTargetSerialId, RwUInt8 byPlace, RwUInt8 byPos)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_ITEM_USE_RES ) )
		return false;

	sUG_ITEM_USE_REQ sPacket;

	memset(&sPacket, 0, sizeof(sUG_ITEM_USE_REQ));

	sPacket.wOpCode = UG_ITEM_USE_REQ;

	sPacket.hTarget = hTargetSerialId;
	sPacket.byPlace = byPlace;
	sPacket.byPos = byPos;

	API_GetSLPacketLockManager()->Lock( GU_ITEM_USE_RES );
	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharTeleportReq(void)
{
	sUG_CHAR_TELEPORT_REQ sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_TELEPORT_REQ));

	sPacket.wOpCode			= UG_CHAR_TELEPORT_REQ;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharDirectPlayAck(void)
{
	sUG_CHAR_DIRECT_PLAY_ACK sPacket;
	memset(&sPacket, 0, sizeof(sUG_CHAR_DIRECT_PLAY_ACK));

	sPacket.wOpCode			= UG_CHAR_DIRECT_PLAY_ACK;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharBindReq( TBLIDX bindObjectTblidx )
{
	sUG_CHAR_BIND_REQ sPacket;

	memset(&sPacket, 0, sizeof(sUG_CHAR_BIND_REQ));

	sPacket.wOpCode = UG_CHAR_BIND_REQ;

	sPacket.bindObjectTblidx = bindObjectTblidx;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendTSConfirmStepReq( BYTE byTSType, NTL_TS_T_ID tId, NTL_TS_TC_ID tcCurId, NTL_TS_TC_ID tcNextId, RwUInt32 uiParam, RwUInt8 byEventType, RwUInt32 uiEventData )
{
	sUG_TS_CONFIRM_STEP_REQ sPacket;

	memset(&sPacket, 0, sizeof(sUG_TS_CONFIRM_STEP_REQ));

	sPacket.wOpCode = UG_TS_CONFIRM_STEP_REQ;

	sPacket.byTsType = byTSType;
	sPacket.tId = tId;
	sPacket.tcCurId = tcCurId;
	sPacket.tcNextId = tcNextId;
	sPacket.dwParam = uiParam;
	sPacket.byEventType = byEventType;
	sPacket.dwEventData = uiEventData;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendQuestGiveUpReq( NTL_TS_T_ID tId )
{
	sUG_QUEST_GIVEUP_REQ sPacket;

	memset(&sPacket, 0, sizeof(sUG_QUEST_GIVEUP_REQ));

	sPacket.wOpCode = UG_QUEST_GIVEUP_REQ;

	sPacket.tId = tId;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendTSUpdateState( NTL_TS_T_ID tId, RwUInt8 byTSType, RwUInt8 byType, RwUInt16 wTSState, RwUInt32 uiParam )
{
	sUG_TS_UPDATE_STATE sPacket;

	memset(&sPacket, 0, sizeof(sUG_TS_UPDATE_STATE));

	sPacket.wOpCode = UG_TS_UPDATE_STATE;

	sPacket.tId = tId;
	sPacket.byTsType = byTSType;
	sPacket.byType = byType;
	sPacket.wTSState = wTSState;
	sPacket.dwParam = uiParam;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendTSExcuteTriggerObject( HOBJECT hSource, HOBJECT hTarget, RwUInt8 byEvtGenType, RwUInt32 uiParam )
{
	sUG_TS_EXCUTE_TRIGGER_OBJECT sPacket;

	memset(&sPacket, 0, sizeof(sUG_TS_EXCUTE_TRIGGER_OBJECT));

	sPacket.wOpCode = UG_TS_EXCUTE_TRIGGER_OBJECT;

	sPacket.hSource = hSource;
	sPacket.hTarget = hTarget;
	sPacket.byEvtGenType = byEvtGenType;
	sPacket.uiParam = uiParam;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendQuestShare( NTL_TS_T_ID tId )
{
	sUG_QUEST_SHARE sPacket;

	memset(&sPacket, 0, sizeof(sUG_QUEST_SHARE));

	sPacket.wOpCode = UG_QUEST_SHARE;

	sPacket.tId = tId;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendQuestObjectVisitReq( NTL_TS_T_ID tId, RwUInt32 uiWorldID, RwUInt8 byObjType, RwUInt32 uiObjectTblIdx )
{
	sUG_QUEST_OBJECT_VISIT_REQ sPacket;

	memset(&sPacket, 0, sizeof(sUG_QUEST_OBJECT_VISIT_REQ));

	sPacket.wOpCode = UG_QUEST_OBJECT_VISIT_REQ;

	sPacket.qId = tId;
	sPacket.worldId = uiWorldID;
	sPacket.byObjType = byObjType;
	sPacket.objectTblidx = uiObjectTblIdx;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

// PrivateShop
bool CNtlSLPacketGenerator::SendPrivateShopCreateReq()
{
	if(API_GetSLPacketLockManager()->IsLock(GU_PRIVATESHOP_CREATE_RES))
		return false;

	sUG_PRIVATESHOP_CREATE_REQ sPacket;
	sPacket.wOpCode = UG_PRIVATESHOP_CREATE_REQ;

	API_GetSLPacketLockManager()->Lock(GU_PRIVATESHOP_CREATE_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

// ActionMap
bool CNtlSLPacketGenerator::SendCharKeyUpdateReq( sSHORTCUT_UPDATE_DATA* pUpdateData , BYTE byCount ) 
{
	sUG_CHAR_KEY_UPDATE_REQ sPacket;
	sPacket.byCount = byCount;
	memcpy( sPacket.asData, pUpdateData, sizeof(sSHORTCUT_UPDATE_DATA) * byCount );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendMailLoadReq(RwUInt32 hSerialID, RwUInt8 uiCount, MAILID aMailID[NTL_MAX_MAIL_SLOT_COUNT])
{
	if(API_GetSLPacketLockManager()->IsLock(GU_MAIL_LOAD_RES))
		return false;

	sUG_MAIL_LOAD_REQ sPacket;
	sPacket.wOpCode	= UG_MAIL_LOAD_REQ;
	sPacket.hObject	= hSerialID;
	sPacket.byCount	= uiCount;

	memcpy(sPacket.aMailID, aMailID, sizeof(MAILID) * NTL_MAX_MAIL_SLOT_COUNT);

	API_GetSLPacketLockManager()->Lock(GU_MAIL_LOAD_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCrossFireReq(RwUInt32 hSerialID)
{
	sUG_CROSSFIRE_REQ sPacket;
	sPacket.wOpCode			= UG_CROSSFIRE_REQ;
	sPacket.hAnotherUser	= hSerialID;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendRideOnBus( RwUInt32 hTargetId ) 
{
	if(API_GetSLPacketLockManager()->IsLock(GU_RIDE_ON_BUS_RES))
		return false;

	sUG_RIDE_ON_BUS_REQ sPacket;
	sPacket.wOpCode     = UG_RIDE_ON_BUS_REQ;
	sPacket.hTarget     = hTargetId;

	API_GetSLPacketLockManager()->Lock(GU_RIDE_ON_BUS_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendRideOffBus() 
{
	if(API_GetSLPacketLockManager()->IsLock(GU_RIDE_OFF_BUS_RES))
		return false;

	sUG_RIDE_OFF_BUS_REQ sPacket;
	sPacket.wOpCode     = UG_RIDE_OFF_BUS_REQ;    

	API_GetSLPacketLockManager()->Lock(GU_RIDE_OFF_BUS_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendVehicle_EngineStartReq(TBLIDX idxItem)
{
	if( API_GetSLPacketLockManager()->IsLock( GU_VEHICLE_ENGINE_START_RES ) )
		return false;

	sUG_VEHICLE_ENGINE_START_REQ sPacket;
	sPacket.wOpCode				= UG_VEHICLE_ENGINE_START_REQ;
	sPacket.idFuelItemTblidx	= idxItem;

	API_GetSLPacketLockManager()->Lock( GU_VEHICLE_ENGINE_START_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CNtlSLPacketGenerator::SendVehicle_EngineStopReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_VEHICLE_ENGINE_STOP_RES ) )
		return false;

	sUG_VEHICLE_ENGINE_STOP_REQ sPacket;
	sPacket.wOpCode = UG_VEHICLE_ENGINE_STOP_REQ;

	API_GetSLPacketLockManager()->Lock( GU_VEHICLE_ENGINE_STOP_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CNtlSLPacketGenerator::SendVehicle_EngineFuelRemoveNfy(RwUInt32 uiFuelItemTblidx)
{
	sUG_VEHICLE_FUEL_REMOVE_NFY sPacket;
	sPacket.wOpCode				= UG_VEHICLE_FUEL_REMOVE_NFY;
	sPacket.idFuelItemTblidx	= uiFuelItemTblidx;

	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CNtlSLPacketGenerator::SendVehicle_EngineEndReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_VEHICLE_END_RES ) )
		return false;

	sUG_VEHICLE_END_REQ sPacket;
	sPacket.wOpCode = UG_VEHICLE_END_REQ;

	API_GetSLPacketLockManager()->Lock( GU_VEHICLE_END_RES );
	return m_pNetSender->SendPacket( sizeof( sPacket ), &sPacket );
}

bool CNtlSLPacketGenerator::SendVehicleDirectPlayCancelNfy() 
{
	sUG_VEHICLE_DIRECT_PLAY_CANCEL_NFY sPacket;
	sPacket.wOpCode     = UG_VEHICLE_DIRECT_PLAY_CANCEL_NFY;    

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendVehicleStuntNfy() 
{
	sUG_VEHICLE_STUNT_NFY sPacket;
	sPacket.wOpCode = UG_VEHICLE_STUNT_NFY;    

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendBudokaiMudosaTeleport(RwUInt8 byMudosaIndex)
{
	if(API_GetSLPacketLockManager()->IsLock(GU_BUDOKAI_MUDOSA_TELEPORT_RES))
		return false;

	sUG_BUDOKAI_MUDOSA_TELEPORT_REQ sPacket;
	sPacket.wOpCode     = UG_BUDOKAI_MUDOSA_TELEPORT_REQ;    
	sPacket.byMudosaIndex = byMudosaIndex;

	API_GetSLPacketLockManager()->Lock(GU_BUDOKAI_MUDOSA_TELEPORT_RES);

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendBudokaiJoinStateReq()
{
	if( API_GetSLPacketLockManager()->IsLock( GU_BUDOKAI_JOIN_STATE_RES ) )
		return false;

	sUG_BUDOKAI_JOIN_STATE_REQ sPacket;
	sPacket.wOpCode = UG_BUDOKAI_JOIN_STATE_REQ;

	API_GetSLPacketLockManager()->Lock( GU_BUDOKAI_JOIN_STATE_RES );

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendBudokaiSocialAction(RwUInt32 uiAnimId)
{
	sUG_BUDOKAI_SOCIAL_ACTION sPacket;
	sPacket.wOpCode = UG_BUDOKAI_SOCIAL_ACTION;
	sPacket.socialAction = (ANIMATIONID)uiAnimId;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}


bool CNtlSLPacketGenerator::SendScouterOnOff( RwBool bOn ) 
{
	if(bOn)
	{
		sUG_SCOUTER_TURN_ON sPacket;
		sPacket.wOpCode = UG_SCOUTER_TURN_ON;

		return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
	}
	else
	{
		sUG_SCOUTER_TURN_OFF sPacket;
		sPacket.wOpCode = UG_SCOUTER_TURN_OFF;

		return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
	}
}


bool CNtlSLPacketGenerator::SendHTBRPBallUseReq(RwUInt8 byBallCount)
{
	sUG_HTB_RP_BALL_USE_REQ sPacket;
	sPacket.wOpCode = UG_HTB_RP_BALL_USE_REQ;
	sPacket.byRpBallCount = byBallCount;

	return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharSkillCastingCanceledNfy(WORD uiResultCode)
{
    sUG_SKILL_CASTING_CANCELED_NFY sPacket;
    sPacket.wOpCode = UG_SKILL_CASTING_CANCELED_NFY;
    sPacket.wResultCode = uiResultCode;
    
    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharLocAfterKnockdownNfy( const RwV3d& vLoc, const RwV3d& vDir )
{
    sUG_CHAR_LOC_AFTER_KNOCKDOWN_NFY sPacket;
    sPacket.wOpCode = UG_CHAR_LOC_AFTER_KNOCKDOWN_NFY;
    sPacket.vCurLoc.x = vLoc.x;
    sPacket.vCurLoc.y = vLoc.y;
    sPacket.vCurLoc.z = vLoc.z;
    sPacket.vCurDir.x = vDir.x;
    sPacket.vCurDir.z = vDir.z;

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharLocAfterSlidingNfy( const RwV3d& vLoc, const RwV3d& vDir )
{
    sUG_CHAR_LOC_AFTER_SLIDING_NFY sPacket;
    sPacket.wOpCode = UG_CHAR_LOC_AFTER_SLIDING_NFY;
    sPacket.vCurLoc.x = vLoc.x;
    sPacket.vCurLoc.y = vLoc.y;
    sPacket.vCurLoc.z = vLoc.z;
    sPacket.vCurDir.x = vDir.x;
    sPacket.vCurDir.z = vDir.z;

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}

bool CNtlSLPacketGenerator::SendCharLocAfterPushNfy( const RwV3d& vLoc, const RwV3d& vDir )
{
    sUG_CHAR_LOC_AFTER_PUSH_NFY sPacket;
    sPacket.wOpCode = UG_CHAR_LOC_AFTER_PUSH_NFY;
    sPacket.vCurLoc.x = vLoc.x;
    sPacket.vCurLoc.y = vLoc.y;
    sPacket.vCurLoc.z = vLoc.z;
    sPacket.vCurDir.x = vDir.x;
    sPacket.vCurDir.z = vDir.z;

    return m_pNetSender->SendPacket(sizeof(sPacket), &sPacket);
}