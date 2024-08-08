#include "precomp_ntlsimulation.h"
#include "NtlFSMCharActEventProc.h"

// shared
#include "NtlBattle.h"
#include "NtlPacketGU.h"
#include "NtlMovement.h"
#include "SkillTable.h"
#include "SystemEffectTable.h"
#include "TableContainer.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlSoundManager.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlAnimEventData.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSobActor.h"
#include "NtlBehaviorDef.h"
#include "NtlBehaviorData.h"
#include "NtlSobProxy.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLEventFunc.h"
#include "NtlFSMStateBase.h"
#include "NtlSobProjectile.h"
#include "NtlSLApi.h"



void FillAttackStuff(SAttackStuff *pAttStuff, SERIAL_HANDLE hAttackerSerialId, SERIAL_HANDLE hDefenderSerialId, RwUInt8 byAttResult,
					 RwBool bChainAtt, RwUInt8 byAttSeq, RwUInt8 byBlocking, SAttackDamageStuff sDamage, RwV3d vPushDir, RwUInt8 byLogicType, 
					 RwBool bAnimSpeedApply, RwUInt32 uiLogicTblId, RwUInt32 uiSyncEventId)
{

	pAttStuff->Reset();

	RwBool bAnimSync = TRUE;
	RwBool bSkillConsider = TRUE;

	if(byAttResult == BATTLE_ATTACK_RESULT_HIT)
	{
		pAttStuff->uiType.bitResult = NTL_ATTACK_RESULT_SUCCESS;
	}
	else if(byAttResult == BATTLE_ATTACK_RESULT_CRITICAL_HIT)
	{
		pAttStuff->uiType.bitResult = NTL_ATTACK_RESULT_SUCCESS;
		pAttStuff->uiType.bitCritical = NTL_ATTACK_ATTR_CRITICAL;
	}
	else if(byAttResult == BATTLE_ATTACK_RESULT_DODGE)
	{
		pAttStuff->uiType.bitResult = NTL_ATTACK_RESULT_DODGE;
		bAnimSync = FALSE;
	}
	else if(byAttResult == BATTLE_ATTACK_RESULT_BLOCK)
	{
		pAttStuff->uiType.bitResult = NTL_ATTACK_RESULT_BLOCK;
	}
	else if(byAttResult == BATTLE_ATTACK_RESULT_KNOCKDOWN)
	{
		pAttStuff->uiType.bitResult = NTL_ATTACK_RESULT_KNOCKDOWN;
		bSkillConsider = FALSE;
	}
	else if(byAttResult == BATTLE_ATTACK_RESULT_SLIDING)
	{
		pAttStuff->uiType.bitResult = NTL_ATTACK_RESULT_SLIDING;
		bSkillConsider = FALSE;
	}
	else if(byAttResult == BATTLE_ATTACK_RESULT_RESISTED )
	{
		pAttStuff->uiType.bitResult = NTL_ATTACK_RESULT_BLOCK;
	}
	
	// skill 사용시 DH or HOT or DB or BB or DC or CB
	if(byLogicType == NTL_ATTACK_LOGIC_SKILL)
	{
		CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
		CSystemEffectTable *pSystemEffTbl = API_GetTableContainer()->GetSystemEffectTable();
		sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(uiLogicTblId) );
		sSYSTEM_EFFECT_TBLDAT *pSystemEffTblData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSystemEffTbl->FindData(pSkillTblData->skill_Effect[0]) );
		if(pSystemEffTblData)
		{
			RwUInt8 byEffectType = pSystemEffTblData->byActive_Effect_Type;
			if(byEffectType == SKILL_ACTIVE_TYPE_DH || byEffectType == SKILL_ACTIVE_TYPE_HOT)
			{
				pAttStuff->uiType.bitResult = NTL_ATTACK_RESULT_HEALING;
			}
			else if(byEffectType == SKILL_ACTIVE_TYPE_DB || byEffectType == SKILL_ACTIVE_TYPE_BB ||
                    byEffectType == SKILL_ACTIVE_TYPE_DC ||	byEffectType == SKILL_ACTIVE_TYPE_CB)
			{
				pAttStuff->uiType.bitResult = NTL_ATTACK_RESULT_BUFF;
				bAnimSync = FALSE;
			}
		}
	}

	pAttStuff->uiType.bitChainAttack	= bChainAtt;
	pAttStuff->uiType.bitLogic			= byLogicType;
	pAttStuff->uiType.bitBlocking		= byBlocking;
	pAttStuff->uiAttrLogicTblId			= uiLogicTblId;
	
	pAttStuff->hAttackerSerialId		= hAttackerSerialId; 
	pAttStuff->hDefenderSerialId		= hDefenderSerialId;
	pAttStuff->sDamage					= sDamage;
	pAttStuff->byAttSeq					= byAttSeq;
	RwV3dAssign(&pAttStuff->vPushDir, &vPushDir);
	pAttStuff->bAnimSpeedApply			= bAnimSpeedApply;

	if(bAnimSync)
	{
		pAttStuff->uiSyncEventId = uiSyncEventId;
	}
}

RwUInt8 JumpKeyMapping(RwUInt8 byCurrKey, RwUInt8 byNextKey)
{
	RwUInt8 byRetKey = byNextKey;

	if(byCurrKey == NTL_MOVE_NONE || byCurrKey == NTL_MOVE_TURN_L ||
		byCurrKey == NTL_MOVE_TURN_R)
	{
		if(byNextKey == NTL_MOVE_F_TURN_L)
			byRetKey = NTL_MOVE_F_TURN_L_JUMP;
		else if(byNextKey == NTL_MOVE_F_TURN_R)
			byRetKey = NTL_MOVE_F_TURN_R_JUMP;
		else if(byNextKey == NTL_MOVE_B_TURN_L)
			byRetKey = NTL_MOVE_B_TURN_L_JUMP;
		else if(byNextKey == NTL_MOVE_B_TURN_R)
			byRetKey = NTL_MOVE_B_TURN_R_JUMP;
		else
			byRetKey = byNextKey;
	}
	else
	{
		if(byCurrKey == NTL_MOVE_F || byCurrKey == NTL_MOVE_F_TURN_L_JUMP || byCurrKey == NTL_MOVE_F_TURN_R_JUMP)
		{
			if(byNextKey == NTL_MOVE_F_TURN_L || byNextKey == NTL_MOVE_B_TURN_L)
				byRetKey = NTL_MOVE_F_TURN_L_JUMP;
			else if(byNextKey == NTL_MOVE_F_TURN_R || byNextKey == NTL_MOVE_B_TURN_R)
				byRetKey = NTL_MOVE_F_TURN_R_JUMP;
			else 
				byRetKey = NTL_MOVE_F;
		}
		else
		{
			if(byNextKey == NTL_MOVE_F_TURN_L || byNextKey == NTL_MOVE_B_TURN_L)
				byRetKey = NTL_MOVE_B_TURN_L_JUMP;
			else if(byNextKey == NTL_MOVE_F_TURN_R || byNextKey == NTL_MOVE_B_TURN_R)
				byRetKey = NTL_MOVE_B_TURN_R_JUMP;
			else 
				byRetKey = NTL_MOVE_B;
		}
	}

	return byRetKey;
}

RwUInt8 JumpKeyStopMapping(RwUInt8 byCurrKey)
{
	RwUInt8 byRetKey;

	if(byCurrKey == NTL_MOVE_F || byCurrKey == NTL_MOVE_F_TURN_L_JUMP || byCurrKey == NTL_MOVE_F_TURN_R_JUMP)
		byRetKey = NTL_MOVE_F;
	else if(byCurrKey == NTL_MOVE_B || byCurrKey == NTL_MOVE_B_TURN_L_JUMP || byCurrKey == NTL_MOVE_B_TURN_R_JUMP)
		byRetKey = NTL_MOVE_B;
	else
		byRetKey = NTL_MOVE_NONE;

	return byRetKey;
}


void HitTargetEffectDirect(CNtlSobActor *pActor, const SHitStuff *pHitStuff)
{
	// Guard Block을 성공했으면...
	RwUInt8 byBlocking			= pHitStuff->uiAttr.bitBlocking;
	RwUInt8 byTargetEffectDir	= pHitStuff->byTargetEffectDir;
	if(byBlocking != DBO_GUARD_TYPE_INVALID && byTargetEffectDir == TARGET_EFFECT_TYPE_NONE)
	{
		return;
	}

	//-------------------------------------------------
	// effect를 출력한다.
	CNtlSobProxy *pSobProxy = pActor->GetSobProxy();
	RwV3d vPos = pActor->GetPosition();

	// 기본 attack effect
	RwV3d vTargetEffPos;
	CNtlPLEntity *pPLTarEntity = NULL;
	if(pHitStuff->bCreateHitStuffEffect && strlen(pHitStuff->chTargetEffName) > 0)
	{
        vTargetEffPos = vPos;
		// physic 공격(miss 일때는 타격 이펙트를 뺀다.)
		if(pHitStuff->uiAttr.bitPhysic == NTL_ATTACK_PHYSIC_MELEE)
		{
			if(pHitStuff->uiAttr.bitResult != NTL_ATTACK_RESULT_DODGE)
			{
				RwMatrix *pMatrix = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);

				vTargetEffPos.x +=  (-pMatrix->at.x)*pSobProxy->GetPLEntityWidth()/2.0f;
				vTargetEffPos.z +=  (-pMatrix->at.z)*pSobProxy->GetPLEntityWidth()/2.0f;
				vTargetEffPos.y += pSobProxy->GetPLEntityHeight() * 0.7f;

				if(pHitStuff->uiAttr.bitResult == NTL_ATTACK_RESULT_BLOCK)
					pPLTarEntity = pSobProxy->CreatePLEffect(NTL_VID_TARGET_BLOCK, &vTargetEffPos);
				else
                {
                    if(pHitStuff->byTargetAttach)
                    {
                        RwV3d vOffset;
                        CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, pSobProxy->GetPLEntityHeight() * pHitStuff->fTargetLocHeight, 0.0f);
                        pPLTarEntity = pSobProxy->CreatePLChildEffect(pHitStuff->chTargetEffName, vOffset);
                    }
                    else
                    {	
                        pPLTarEntity = pSobProxy->CreatePLEffect(pHitStuff->chTargetEffName, &vTargetEffPos);
                    }
                }
					
			}
		}
		// energy attack.(healing, buff 전부 포함)
		else
		{
			
			if(pHitStuff->byTargetAttach)
			{
				RwV3d vOffset;
				CNtlMath::MathRwV3dAssign(&vOffset, 0.0f, pSobProxy->GetPLEntityHeight() * pHitStuff->fTargetLocHeight, 0.0f);
				pPLTarEntity = pSobProxy->CreatePLChildEffect(pHitStuff->chTargetEffName, vOffset);
			}
			else
			{	
				vTargetEffPos.y += pSobProxy->GetPLEntityHeight() * pHitStuff->fTargetLocHeight;
				pPLTarEntity = pSobProxy->CreatePLEffect(pHitStuff->chTargetEffName, &vTargetEffPos);
			}
		}
	}

	// target effect 방향을 설정한다.)
	if(pPLTarEntity)
	{
		if(byTargetEffectDir != TARGET_EFFECT_TYPE_NONE)
		{
			CNtlSob *pAttSobObj = GetNtlSobManager()->GetSobObject(pHitStuff->hAttackerSerialId);
			if(pAttSobObj)
			{
				RwV3d vAttPos = pAttSobObj->GetPosition();
				RwV3d vTarPos = pActor->GetPosition();
				RwV3d vEffPos = pPLTarEntity->GetPosition();

				RwV3d vDir;
				RwV3dSub(&vDir, &vTarPos, &vAttPos);
				vDir.y = 0.0f;
				RwV3dNormalize(&vDir, &vDir);				

				if(byTargetEffectDir == TARGET_EFFECT_TYPE_SIDE)
				{
					RwMatrix mat;
					CNtlMath::MathRwMatrixAssign(&mat, &vEffPos, &vDir);
					pPLTarEntity->SetMatrix(mat);
				}
				else if(byTargetEffectDir == TARGET_EFFECT_TYPE_FRONT)
				{
					RwMatrix mat;
					CNtlMath::MathRwMatrixAssign(&mat, &vEffPos, &vDir);
					pPLTarEntity->SetMatrix(mat);
				}
			}
		}
	}

}

void HitResultDirect(CNtlSobActor *pActor, const SHitStuff *pHitStuff)
{
	CNtlSobProxy *pSobProxy = pActor->GetSobProxy();
	RwV3d vPos = pActor->GetPosition();
	RwV3d vTargetEffPos;

	// heal or buff skill이 아니면?
	RwUInt8 byResult	= pHitStuff->uiAttr.bitResult;
	RwUInt8 byBlocking	= pHitStuff->uiAttr.bitBlocking;

	if( !(byResult == NTL_ATTACK_RESULT_HEALING || byResult == NTL_ATTACK_RESULT_BUFF || 
			byResult == NTL_ATTACK_RESULT_STEAL) )
	{
		if(byBlocking == DBO_GUARD_TYPE_INVALID)
		{
			// critical effect
			if(pHitStuff->uiAttr.bitCritical == NTL_ATTACK_ATTR_CRITICAL && pHitStuff->uiAttr.bitHitSeq == 0)
			{
				RwV3dAssign(&vTargetEffPos, &vPos);
				vTargetEffPos.y += pSobProxy->GetPLEntityHeight();
				pSobProxy->CreatePLEffect(NTL_VID_TEXT_CRITICAL, &vTargetEffPos);
			}

			// block effect
			else if(byResult == NTL_ATTACK_RESULT_BLOCK && pHitStuff->uiAttr.bitHitSeq == 0)
			{
				RwV3dAssign(&vTargetEffPos, &vPos);
				vTargetEffPos.y += pSobProxy->GetPLEntityHeight();
				pSobProxy->CreatePLEffect(NTL_VID_TEXT_BLOCK, &vTargetEffPos);
			}

			// dodge effect
			else if(byResult == NTL_ATTACK_RESULT_DODGE && pHitStuff->uiAttr.bitHitSeq == 0)
			{
				RwV3dAssign(&vTargetEffPos, &vPos);
				vTargetEffPos.y += pSobProxy->GetPLEntityHeight();
				pSobProxy->CreatePLEffect(NTL_VID_TEXT_DODGE, &vTargetEffPos);
			}

			// dodge effect
			if(pHitStuff->uiAttr.bitBehavior == NTL_ATTACK_KNOCKDOWN)
			{
				RwV3dAssign(&vTargetEffPos, &vPos);
				vTargetEffPos.y += pSobProxy->GetPLEntityHeight();
				pSobProxy->CreatePLEffect(NTL_VID_TEXT_KNOCKDOWN, &vTargetEffPos);
			}

			// 기본 attack word effect(miss는 띄우지 않는다.)
			if(byResult != NTL_ATTACK_RESULT_DODGE)
			{
				RwV3dAssign(&vTargetEffPos, &vPos);
				vTargetEffPos.y += pSobProxy->GetPLEntityHeight();
				if(pHitStuff->bCreateHitStuffEffect && pHitStuff->chWordEffName[0])
					pSobProxy->CreatePLEffect(pHitStuff->chWordEffName, &vTargetEffPos);
				
				// damage를 출력한다.
				pSobProxy->CreatePLDamageBox(vPos, pHitStuff->sDamageStuff.iDamage);
			}

			//-------------------------------------------------
			// hit sound 출력.(dodge일 경우에는 play 안한다)
			if(pActor->IsVisible() && byResult != NTL_ATTACK_RESULT_DODGE)
			{
                SOUND_HANDLE hSound = NULL;

				if(pHitStuff->uiAttr.bitPowerEffect)
				{
					sNtlSoundPlayParameta tSoundParam;
					tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
					tSoundParam.pcFileName		= SOUND_HIT_POWER;
					tSoundParam.fXPos			= vPos.x;
					tSoundParam.fYPos			= vPos.y;
					tSoundParam.fZPos			= vPos.z;

					GetSoundManager()->Play(&tSoundParam);

					hSound = tSoundParam.hHandle;
				}
				else
				{
					if(rand()%2)
					{
						sNtlSoundPlayParameta tSoundParam;
						tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
						tSoundParam.pcFileName		= SOUND_HIT1;
						tSoundParam.fXPos			= vPos.x;
						tSoundParam.fYPos			= vPos.y;
						tSoundParam.fZPos			= vPos.z;

						GetSoundManager()->Play(&tSoundParam);

						hSound = tSoundParam.hHandle;
					}
					else
					{
						sNtlSoundPlayParameta tSoundParam;
						tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
						tSoundParam.pcFileName		= SOUND_HIT2;
						tSoundParam.fXPos			= vPos.x;
						tSoundParam.fYPos			= vPos.y;
						tSoundParam.fZPos			= vPos.z;

						GetSoundManager()->Play(&tSoundParam);

						hSound = tSoundParam.hHandle;
					}
				}

                // Hit Sound Echo
                if(pHitStuff->bHitSoundEcho)
                {
                    GetSoundManager()->SetChannelEffect(hSound, FMOD_DSP_TYPE_ECHO);
                }
			}
		}

		//---------------------------------------------------
		// 

		pActor->SetFinialHurtSerialId(pHitStuff->hAttackerSerialId);
	}
	else
	{	
		if(byResult == NTL_ATTACK_RESULT_STEAL)
		{
			if(pHitStuff->uiAttr.bitLogic == NTL_ATTACK_LOGIC_SKILL)
			{
                if(pHitStuff->sDamageStuff.LP_EP_STEAL_fLpGained > 0)
                {
                    pSobProxy->CreatePLDamageBox(vPos, (RwUInt32)pHitStuff->sDamageStuff.LP_EP_STEAL_fLpGained, TRUE);
                }
                
                if(pHitStuff->sDamageStuff.LP_EP_STEAL_fEpGained > 0)
                {
                    pSobProxy->CreatePLDamageBox(vPos, (RwUInt32)pHitStuff->sDamageStuff.LP_EP_STEAL_fEpGained, TRUE);
                } 
			}
		}
		else if(byResult == NTL_ATTACK_RESULT_HEALING)
		{
			// damage를 출력한다.
			pSobProxy->CreatePLDamageBox(vPos, pHitStuff->sDamageStuff.iDamage, TRUE);
		}
	}

	//---------------------------------------------------
	// blocking effect 출력.
		
	if(byBlocking != DBO_GUARD_TYPE_INVALID)
	{
		// effect 연출.

		RwReal fPLEntityHalfWidth = pSobProxy->GetPLEntityWidth()/2.0f;
		RwReal fPLEntityHeight = pSobProxy->GetPLEntityHeight();

		RwV3dAssign(&vTargetEffPos, &vPos);
		RwMatrix *pMatrix = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
		vTargetEffPos.x +=  (-pMatrix->at.x)*fPLEntityHalfWidth;
		vTargetEffPos.z +=  (-pMatrix->at.z)*fPLEntityHalfWidth;
		vTargetEffPos.y += fPLEntityHeight * 0.7f;
		pSobProxy->CreatePLEffect(NTL_VID_TARGET_BLOCK, &vTargetEffPos);

        if(pHitStuff->uiAttr.bitHitSeq == 0)
		{
			RwV3dAssign(&vTargetEffPos, &vPos);
			vTargetEffPos.y += fPLEntityHeight;

			RwBool bBlockingSuccess = FALSE;

			if(byBlocking == DBO_GUARD_TYPE_DEFENCE)
			{
				pSobProxy->CreatePLEffect(NTL_VID_BLOCKING_DEFENCE, &vTargetEffPos);
				bBlockingSuccess = TRUE;
			}
			else if(byBlocking == DBO_GUARD_TYPE_DAMAGE_SKILL)
			{
				pSobProxy->CreatePLEffect(NTL_VID_BLOCKING_DAMAGE, &vTargetEffPos);
				bBlockingSuccess = TRUE;
			}
			else if(byBlocking == DBO_GUARD_TYPE_CURSE_SKILL)
			{
				pSobProxy->CreatePLEffect(NTL_VID_BLOCKING_CURSE, &vTargetEffPos);
				bBlockingSuccess = TRUE;
			}
			else if(byBlocking == DBO_GUARD_TYPE_KNOCKDOWN)
			{
				pSobProxy->CreatePLEffect(NTL_VID_BLOCKING_KNOCKDOWN, &vTargetEffPos);
				bBlockingSuccess = TRUE;
			}
			else if(byBlocking == DBO_GUARD_TYPE_HTB)
			{
				pSobProxy->CreatePLEffect(NTL_VID_BLOCKING_HTB, &vTargetEffPos);
				bBlockingSuccess = TRUE;
			}

			if(bBlockingSuccess)
			{
				//pSobProxy->CreatePLEffect(NTL_VID_GUARD_SUCCESS, &vPos);
                pSobProxy->CreatePLChildEffect(NTL_VID_GUARD_SUCCESS, ZeroAxis, FALSE, TRUE);                
			}
		}
	}
}

void HitStatusStealSkillDirect(CNtlSobActor *pActor, const SHitStuff *pHitStuff)
{
	if(pHitStuff->uiAttr.bitResult == NTL_ATTACK_RESULT_STEAL ||            // 이 플래그는 Projectile 전용이다. (Client 처리용) 아래 코드 참조
       pHitStuff->uiAttr.bitResult == NTL_ATTACK_RESULT_BLOCK ||
       pHitStuff->uiAttr.bitReserved == NTL_ATTACK_RESULT_DODGE)
		return;

	if(pHitStuff->uiAttr.bitLogic == NTL_ATTACK_LOGIC_SKILL)
	{
		CSkillTable *pSkillTable = API_GetTableContainer()->GetSkillTable();
		CSystemEffectTable *pSysEffTable = API_GetTableContainer()->GetSystemEffectTable();

		sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData(pHitStuff->uiAttrLogicTblId) );

		if(pSkillTblData == NULL)
			return;

		SHitStuff sHit;

		for(RwInt32 i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
		{
			if(pSkillTblData->skill_Effect[i] == INVALID_TBLIDX)
				continue;
			sSYSTEM_EFFECT_TBLDAT *pSysEffTblData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSysEffTable->FindData(pSkillTblData->skill_Effect[i]) );

			if( !(pSysEffTblData->effectCode == ACTIVE_LP_STEAL || pSysEffTblData->effectCode == ACTIVE_EP_STEAL) )
				continue;

			void *pSobObj = CNtlSLEventGenerator::SobProjectileCreate(SLCLASS_PROJECTILE, pActor->GetSerialID(), pHitStuff->hAttackerSerialId, 
																		NTL_BEID_PROJ_STEAL, BONE_CHARACTER, 20.0f, TRUE, pHitStuff->bCreateHitStuffEffect, NULL, 
																	  pSysEffTblData->szSuccess_Effect_Name, 0.5f, NULL);
			if(pSobObj)
			{
				CNtlSobProjectile *pSobProj = reinterpret_cast<CNtlSobProjectile*>(pSobObj);
				pSobProj->SetTargetID(pHitStuff->hAttackerSerialId);
				
				sHit.Reset();

				sHit.uiAttr						= pHitStuff->uiAttr;
				sHit.uiAttr.bitResult			= NTL_ATTACK_RESULT_STEAL;
				sHit.hAttackerSerialId			= pHitStuff->hDefenderSerialId;
				sHit.hDefenderSerialId			= pHitStuff->hAttackerSerialId;
                sHit.sDamageStuff               = pHitStuff->sDamageStuff;
				sHit.sDamageStuff.iDamage		= pHitStuff->sDamageStuff.iDamage * (RwUInt32)pSkillTblData->fSkill_Effect_Value[i] / 100;
				sHit.uiAttrLogicTblId			= pHitStuff->uiAttrLogicTblId;
				sHit.uiProjectileBehaviorId		= NTL_BEID_PROJ_STEAL;
				sHit.byShotType					= BONE_CHARACTER;
				sHit.bySubWeaponShotFlag		= BONE_CHARACTER;
				sHit.fSpeed						= 20.0f;
				sHit.bLeftHand					= FALSE;

				if( strlen(pSysEffTblData->szSuccess_End_Effect_Name) > 0 )
					strcpy_s(sHit.chTargetEffName, pSysEffTblData->szSuccess_End_Effect_Name);

				sHit.byTargetAttach				= FALSE;	
				sHit.fTargetLocHeight			= (RwReal)((RwReal)pSysEffTblData->byEnd_Effect_Position / (RwReal)100);
				sHit.bCameraShake				= FALSE;

				pSobProj->AddHit(sHit);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// event 처리 함수.

void FSMEvent_CharActActionMapJump(CNtlSobActor *pActor, SNtlEventActionMapJump *pJump, CNtlFSMStateBase *pStateBase)
{
	NTL_FUNCTION("FSMEvent_CharActActionMapJump");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	SJumpStuff *pJumpStuff = pBeData->GetJumpStuff();

	pMoveStuff->Reset();
	pJumpStuff->Reset();

	pMoveStuff->byType = NTL_MOVETARGET_DIR;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;

	if(pJump->uiMoveFlags == NTL_MOVE_NONE)
	{
		if( !(pStateBase->GetFlags() & NTL_FSMSF_NOT_RUNJUMP) )
		{
			pMoveStuff->byMoveFlags = NTL_MOVE_F;
		}
	}
	else
	{
		if(pJump->uiMoveFlags == NTL_MOVE_F_TURN_L)
			pMoveStuff->byMoveFlags = NTL_MOVE_F_TURN_L_JUMP;
		else if(pJump->uiMoveFlags == NTL_MOVE_F_TURN_R)
			pMoveStuff->byMoveFlags = NTL_MOVE_F_TURN_R_JUMP;
		else if(pJump->uiMoveFlags == NTL_MOVE_B_TURN_L)
			pMoveStuff->byMoveFlags = NTL_MOVE_B_TURN_L_JUMP;
		else if(pJump->uiMoveFlags == NTL_MOVE_B_TURN_R)
			pMoveStuff->byMoveFlags = NTL_MOVE_B_TURN_R_JUMP;
		else 
			pMoveStuff->byMoveFlags = (RwUInt8)pJump->uiMoveFlags;
	}
	
	pJumpStuff->vJumpDir = pActor->GetDirection();

	NTL_RETURNVOID();
}

void FSMEvent_CharActActionMapSkillUse(CNtlSobActor *pActor, SNtlEventActionMapSkillUse *pSkillUse)
{
	NTL_FUNCTION("FSMEvent_CharActActionMapSkillUse");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 

	if(pSkillUse->hSerialId != pSkillUse->hTargetSerialId)
	{
		SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
		pMoveStuff->Reset();
		pMoveStuff->byType = NTL_MOVETARGET_ACTOR;
		pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
		pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
		pMoveStuff->hFollowSerialId = pSkillUse->hTargetSerialId;
		pMoveStuff->fFollowRange = pSkillUse->fFollowRange;
	}

	SSkillStuff *pSkillStuff		= pBeData->GetSkillStuff();
	pSkillStuff->Reset();
	pSkillStuff->hTargetSerialId	= pSkillUse->hTargetSerialId;
	pSkillStuff->hSkillSerialId		= pSkillUse->hSkillSerialId;
	pSkillStuff->bySlotIdx			= pSkillUse->bySkillSlotIdx;
	pSkillStuff->byRpBonusType		= pSkillUse->byRpBonusType;

	if(pSkillUse->hSerialId != pSkillUse->hTargetSerialId)
	{
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		pCtrlStuff->fFollowRange = pSkillUse->fFollowRange;
		pCtrlStuff->hTargetSerialId = pSkillUse->hTargetSerialId;
	}
		
	NTL_RETURNVOID();
}

void FSMEvent_CharActActionMapSocialUse(CNtlSobActor *pActor, SNtlEventActionMapSocialUse *pSocialUse)
{
	NTL_FUNCTION("FSMEvent_CharActActionMapSocialUse");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 

	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
	pSkillStuff->Reset();
	pSkillStuff->uiSkillTblId = pSocialUse->uiTblId;

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->Reset();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
		
	NTL_RETURNVOID();
}

void FSMEvent_CharActActionMapHTBUse(CNtlSobActor *pActor, SNtlEventActionMapHTBUse *pHTBUse)
{
	NTL_FUNCTION("FSMEvent_CharActActionMapHTBUse");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_ACTOR;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->hFollowSerialId = pHTBUse->hTargetSerialId;
	pMoveStuff->fFollowRange = pHTBUse->fFollowRange;

	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();
	pHTBStuff->Reset();
	pHTBStuff->hAttackSerialId = pActor->GetSerialID();
	pHTBStuff->hTargetSerialId = pHTBUse->hTargetSerialId;
	pHTBStuff->bySlotIdx = pHTBUse->bySkillSlotIdx;

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->fFollowRange = pHTBUse->fFollowRange;
	pCtrlStuff->hTargetSerialId = pHTBUse->hTargetSerialId;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActActionMapAutoFollow(CNtlSobActor *pActor, SNtlEventActionMapAutoFollow *pAutoFollow)
{
	NTL_FUNCTION("FSMEvent_CharActActionMapAutoFollow");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_ACTOR;	
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->hFollowSerialId = pAutoFollow->hTargetSerialId;

    // 현재 위치가 육지인지 물속인지 파악한다.    
    SWorldHeightStuff hStuff;
    if(Logic_IsSwimmingActor(pActor, &pActor->GetPosition(), hStuff))
        pMoveStuff->byForm = NTL_MOVEFORM_SWIMMING;
    else
        pMoveStuff->byForm = NTL_MOVEFORM_GROUND;

	CNtlSobActor *pSobTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pAutoFollow->hTargetSerialId) );
	pMoveStuff->fFollowRange = Logic_GetCommunityRange(pSobTarActor);
	pMoveStuff->vCurrLoc = pActor->GetDirection();

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->fFollowRange = pMoveStuff->fFollowRange;
	pCtrlStuff->hTargetSerialId = pAutoFollow->hTargetSerialId;
	
	NTL_RETURNVOID();
}


void FSMEvent_CharActTerrainClick(CNtlSobActor *pActor, SNtlEventTerrainClick *pTerrainClick)
{
	NTL_FUNCTION("FSMEvent_CharActTerrainClick");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_LOC;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->hFollowSerialId = INVALID_SERIAL_ID;
	RwV3dAssign(&pMoveStuff->vDest, &pTerrainClick->vPickPos);  

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
	
	NTL_RETURNVOID();
}

// 클릭한 위치가 아닌 그 위치쪽 방향으로 이동한다.
void FSMEvent_CharActTerrainClickDir( CNtlSobActor* pActor, SNtlEventTerrainClick* pTerrainClick ) 
{
    NTL_FUNCTION("FSMEvent_CharKeyboardMove");

    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
    RwV3d vDir = pTerrainClick->vPickPos - pActor->GetPosition();
    RwV3dNormalize(&vDir, &vDir);
    pActor->SetDirection(&vDir);

    SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
    pMoveStuff->Reset();
    pMoveStuff->byType = NTL_MOVETARGET_DIR;
    pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
    pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
    pMoveStuff->byMoveFlags = NTL_MOVE_F;
    pMoveStuff->vDir = pActor->GetDirection();	

    SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
    pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;

    NTL_RETURNVOID();
}

void FSMEvent_CharActKeyboardMove(CNtlSobActor *pActor, SNtlEventKeyboardMove *pKeyboardMove)
{
	NTL_FUNCTION("FSMEvent_CharKeyboardMove");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();

	pMoveStuff->byType = NTL_MOVETARGET_DIR;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->byMoveFlags = pKeyboardMove->byMoveFlags;
    pMoveStuff->vDir = pActor->GetDirection();	

	if(pKeyboardMove->bAvatarAngleChange)
		pActor->SetAngleY(pKeyboardMove->fAvatarAngleY);

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;

	NTL_RETURNVOID();
}

void FSMEvent_CharActKeyboardMoveStop(CNtlSobActor *pActor)
{
	NTL_FUNCTION("FSMEvent_CharKeyboardMoveStop");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
		
	NTL_RETURNVOID();
}

void FSMEvent_CharActMouseDashMove(CNtlSobActor *pActor, SNtlEventMouseDashMove *pMouseDashMove)
{
	NTL_FUNCTION("FSMEvent_CharActMouseDashMove");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();

	pMoveStuff->byType = NTL_MOVETARGET_DASH;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->byMoveFlags = NTL_MOVE_DASH_F;

	RwV3d vDir, vDest;
	RwV3dAssign(&vDest, &pMouseDashMove->vPickPos);
	RwV3d vPos = pActor->GetPosition();
	RwV3dSub(&vDir, &vDest, &vPos);

	RwReal fLen = RwV3dLength(&vDir);
	RwV3dNormalize(&vDir, &vDir);

	if(fLen > NTL_DASH_DISTANCE_F)
	{
		vDest.x = vPos.x + vDir.x * NTL_DASH_DISTANCE_F;
		vDest.y = vPos.y + vDir.y * NTL_DASH_DISTANCE_F;
		vDest.z = vPos.z + vDir.z * NTL_DASH_DISTANCE_F;
	}

	RwV3dAssign(&pMoveStuff->vDir, &vDir);  
	RwV3dAssign(&pMoveStuff->vDest, &vDest);

	pActor->SetDirection(&vDir);

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;

	NTL_RETURNVOID();
}


void FSMEvent_CharActKeyboardDashMove(CNtlSobActor *pActor, SNtlEventKeyboardDashMove *pKeyboardDashMove)
{
	NTL_FUNCTION("FSMEvent_CharActKeyboardDashMove");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();

	pMoveStuff->byType = NTL_MOVETARGET_DASH;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->byMoveFlags = (RwUInt8)pKeyboardDashMove->uiMoveFlags;

	RwV3d vDest;
	RwV3d vPos = pActor->GetPosition();
	RwV3d vDir = pActor->GetDirection();
	RwUInt32 uiMoveFlags = pKeyboardDashMove->uiMoveFlags;

	RwReal fDashDist, fDashHeightOffset, fDahsHeightWeight = 0.5f;

	if(uiMoveFlags == NTL_MOVE_DASH_F)
	{
		fDashDist = NTL_DASH_DISTANCE_F;
		fDashHeightOffset = NTL_DASH_DISTANCE_F*fDahsHeightWeight;
	}
	else if(uiMoveFlags == NTL_MOVE_DASH_B)
	{
		RwV3dScale(&vDir, &vDir, -1.0f);
		fDashDist = NTL_DASH_DISTANCE_B;
		fDashHeightOffset = NTL_DASH_DISTANCE_B*fDahsHeightWeight;
	}
	else if(uiMoveFlags == NTL_MOVE_DASH_L)
	{
		RwV3d vMoveDir;
		RwV3dCrossProduct(&vMoveDir, &vDir, &CNtlPLGlobal::m_vYAxisV3);
		RwV3dAssign(&vDir, &vMoveDir);
		vDir.x = -vDir.x;
		vDir.z = -vDir.z;
		fDashDist = NTL_DASH_DISTANCE_L;
		fDashHeightOffset = NTL_DASH_DISTANCE_L*fDahsHeightWeight;
	}
	else
	{
		RwV3d vMoveDir;
		RwV3dCrossProduct(&vMoveDir, &vDir, &CNtlPLGlobal::m_vYAxisV3);
		RwV3dAssign(&vDir, &vMoveDir);
		fDashDist = NTL_DASH_DISTANCE_R;
		fDashHeightOffset = NTL_DASH_DISTANCE_R*fDahsHeightWeight;
	}

	vDest.x = vPos.x + vDir.x*fDashDist;
	vDest.y = vPos.y + vDir.y*fDashDist + fDashHeightOffset;
	vDest.z = vPos.z + vDir.z*fDashDist;

	RwV3dAssign(&pMoveStuff->vDir, &vDir);  
	RwV3dAssign(&pMoveStuff->vDest, &vDest);

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;

	NTL_RETURNVOID();
}

void FSMEvent_CharActInputChangeHeading(CNtlSobActor *pActor, SNtlEventInputChangeHading *pChangeHeading)
{	
	NTL_FUNCTION("FSMEvent_CharActInputChangeHeading");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->uExtra.sRotate.fDeltaAngle = pChangeHeading->fDeltaAngle;
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActSobMove(CNtlSobActor *pActor, SNtlEventSobMove *pMove)
{
	NTL_FUNCTION("FSMEvent_CharActMouseMove");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
    pMoveStuff->vCurrLoc = pMove->vCurrLoc;

	if(pMove->bMouseMove)
	{
		pMoveStuff->byType = NTL_MOVETARGET_LOC;
        pMoveStuff->vDest = pMove->vLoc;		
	}
	else
	{
		pMoveStuff->byType = NTL_MOVETARGET_DIR;
		pMoveStuff->byMoveFlags = pMove->byMoveFlags;
        pMoveStuff->vDir = pMove->vDir;		
		pActor->SetDirection(&pMove->vDir);
	}

	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;

	pMoveStuff->byFormFlag = pMove->byFormFlag;
	if(pMove->byPathNodeNum)
	{
		pMoveStuff->byPathNodeNum = pMove->byPathNodeNum;
		if(pMoveStuff->byPathNodeNum > DBO_MAX_NEXT_DEST_LOC_COUNT)
			pMoveStuff->byPathNodeNum = DBO_MAX_NEXT_DEST_LOC_COUNT;
		
		memcpy(pMoveStuff->arrPathNode, pMove->pPathNode, sizeof(RwV3d)*pMoveStuff->byPathNodeNum);
	}

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActSobSecondDestMove(CNtlSobActor *pActor, SNtlEventSobSecondDestMove *pMove)
{
	NTL_FUNCTION("FSMEvent_CharActSobSecondDestMove");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();

	RwV3dAssign(&pMoveStuff->vCurrLoc, &pMove->vCurrLoc);  
	RwV3dAssign(&pMoveStuff->vDest, &pMove->vLoc);  
	RwV3dAssign(&pMoveStuff->vSendDest, &pMove->vSecondDest);  

	if(Logic_IsBus(pActor))
	{
		pMoveStuff->byForm = NTL_MOVEFORM_BUS;
	}
	else
	{
		pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	}

	pMoveStuff->byFormFlag = pMove->byFormFlag;

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;

	NTL_RETURNVOID();
}

void FSMEvent_CharActSobSplineMove(CNtlSobActor *pActor, SNtlEventSobSplineMove *pSplineMove)
{
	NTL_FUNCTION("FSMEvent_CharActSobSplineMove");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();

	pMoveStuff->byForm = NTL_MOVEFORM_SPLINE;
	pMoveStuff->bSplineGroundCheck = pSplineMove->bGroundCheck;
	pMoveStuff->bSplineRunMove = pSplineMove->bRunMove;
	strcpy_s(pMoveStuff->chSpline, NTL_MAX_SPLINE_NAME, pSplineMove->pSplineName);
	RwV3dAssign(&pMoveStuff->vCurrLoc, &pSplineMove->vStartPos);

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActSobDash(CNtlSobActor *pActor, SNtlEventSobDash *pDash)
{
	NTL_FUNCTION("FSMEvent_CharActSobDash");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();

	pMoveStuff->byType = NTL_MOVETARGET_DASH;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->byMoveFlags = pDash->byMoveFlags;

	RwV3d vPos = pActor->GetPosition();

	RwV3d vDir;
	RwV3dSub(&vDir, &pDash->vDest, &vPos);
	vDir.y = 0.0f;
	RwV3dNormalize(&vDir, &vDir);

	RwV3dAssign(&pMoveStuff->vDir, &vDir);  
	RwV3dAssign(&pMoveStuff->vDest, &pDash->vDest);

	RwUInt32 uiMoveFlags = pDash->byMoveFlags;
	
	if(uiMoveFlags == NTL_MOVE_DASH_F)
	{
	}
	else if(uiMoveFlags == NTL_MOVE_DASH_B)
	{
		RwV3dScale(&vDir, &vDir, -1.0f);
	}
	else if(uiMoveFlags == NTL_MOVE_DASH_L)
	{
		RwV3d vHeading;
		RwV3dCrossProduct(&vHeading, &vDir, &CNtlPLGlobal::m_vYAxisV3);
		RwV3dAssign(&vDir, &vHeading);
	}
	else
	{
		RwV3d vHeading;
		RwV3dCrossProduct(&vHeading, &vDir, &CNtlPLGlobal::m_vYAxisV3);
		RwV3dAssign(&vDir, &vHeading);
		RwV3dScale(&vDir, &vDir, -1.0f);
	}

	pActor->SetDirection(&vDir);

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActSobSkillFollowMove(CNtlSobActor *pActor, SNtlEventSobFollowMove *pFollowMove)
{
	NTL_FUNCTION("FSMEvent_CharActActionMapSkillUse");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_ACTOR;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->hFollowSerialId = pFollowMove->hTargetSerialId;
	pMoveStuff->fFollowRange = pFollowMove->fFollowRange;
	pMoveStuff->vCurrLoc = pFollowMove->vCurrLoc;

	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
	pSkillStuff->Reset();
	pSkillStuff->hTargetSerialId = pFollowMove->hTargetSerialId;
	pSkillStuff->bySlotIdx = 0;

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->fFollowRange = pFollowMove->fFollowRange;
	pCtrlStuff->hTargetSerialId = pFollowMove->hTargetSerialId;
		
	NTL_RETURNVOID();
}


void FSMEvent_CharActSobFollowMove(CNtlSobActor *pActor, SNtlEventSobFollowMove *pFollowMove)
{
	NTL_FUNCTION("FSMEvent_CharActSobFollowMove");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_ACTOR;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = pFollowMove->byFormFlag;
	pMoveStuff->hFollowSerialId = pFollowMove->hTargetSerialId;
	pMoveStuff->fFollowRange = pFollowMove->fFollowRange;
	pMoveStuff->vCurrLoc = pFollowMove->vCurrLoc;

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->fFollowRange = pFollowMove->fFollowRange;
	pCtrlStuff->hTargetSerialId = pFollowMove->hTargetSerialId;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActSobFightingFollowMove(CNtlSobActor *pActor, SNtlEventSobFollowMove *pFollowMove)
{
	NTL_FUNCTION("FSMEvent_CharActSobFightingFollowMove");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_ACTOR;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->hFollowSerialId = pFollowMove->hTargetSerialId;
	pMoveStuff->fFollowRange = pFollowMove->fFollowRange;
	pMoveStuff->vCurrLoc = pFollowMove->vCurrLoc;

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->fFollowRange = pFollowMove->fFollowRange;
	pCtrlStuff->hTargetSerialId = pFollowMove->hTargetSerialId;
		
	NTL_RETURNVOID();
}

void FSMEvent_CharActSobMoveStop(CNtlSobActor *pActor, SNtlEventSobMoveStop *pMoveStop)
{
	NTL_FUNCTION("FSMEvent_CharActMouseMoveStop");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());

	SMoveStuff *pMoveStuff      = pBeData->GetMoveStuff(); 
	pMoveStuff->byType          = NTL_MOVETARGET_NONE;
    pMoveStuff->vDest           = pMoveStop->vLoc;
    pMoveStuff->vDir            = pMoveStop->vDir;	
	pMoveStuff->byPathNodeNum   = 0;

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;

    pBeData->GetMoveSyncStuff()->Clear();

	NTL_RETURNVOID();
}

void FSMEvent_CharActSobDirectionFloat(CNtlSobActor *pActor, SNtlEventSobDirectionFloat *pFloat)
{
	NTL_FUNCTION("FSMEvent_CharActSobDirectionFloat");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->byMoveFlags = pFloat->byMoveFlags;

	NTL_RETURNVOID();
}

void FSMEvent_CharActSobTargetSelect(CNtlSobActor *pActor, SNtlEventSobTargetSelect *pSobTargetSelect)
{
	NTL_FUNCTION("FSMEvent_CharActSobTargetSelect");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
    CNtlSobActor *pSobTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pSobTargetSelect->hSerialId) );

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_ACTOR;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->hFollowSerialId = pSobTargetSelect->hSerialId; 	

	if( Logic_IsCommunityActor(pSobTarActor) )
	{
		pMoveStuff->fFollowRange = Logic_GetCommunityRange(pSobTarActor);
	}
    else
    {
        pMoveStuff->fFollowRange = Logic_GetAttackRange(pActor, pSobTargetSelect->hSerialId);
    }

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 
	pCtrlStuff->fFollowRange = pMoveStuff->fFollowRange;
	pCtrlStuff->hTargetSerialId = pSobTargetSelect->hSerialId;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActSobAttackSelect(CNtlSobActor *pActor, SNtlEventSobAttackSelect *pSobAttackSelect)
{
	NTL_FUNCTION("FSMEvent_CharActSobAttackSelect");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());

	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff(); 
	pMoveStuff->Reset();
	pMoveStuff->byType = NTL_MOVETARGET_ACTOR;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->hFollowSerialId = pSobAttackSelect->hSerialId; 
	pMoveStuff->fFollowRange = Logic_GetAttackRange(pActor, pSobAttackSelect->hSerialId);

	CNtlSobActor *pSobTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pSobAttackSelect->hSerialId) );
	if( Logic_IsCommunityActor(pSobTarActor) )
	{
		pMoveStuff->fFollowRange = Logic_GetCommunityRange(pSobTarActor);
	}

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff(); 
	pCtrlStuff->fFollowRange = pMoveStuff->fFollowRange;
	pCtrlStuff->hTargetSerialId = pSobAttackSelect->hSerialId;
		
	NTL_RETURNVOID();
}

void FSMEvent_CharActAttack(CNtlSobActor *pActor, SNtlEventSobAttack *pAttack)
{
	NTL_FUNCTION("FSMEvent_CharActAttack");

	CNtlBeCharData *pBeData			= reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	CNtlAttackData *pAttData		= pBeData->GetAttackData();
	SCtrlStuff *pCtrlStuff			= pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId		= pAttack->hDefenderSerialId;

	pAttData->Reset();
	pAttData->SetAttackStuffCount(1);
	SAttackStuff *pAttStuff = pAttData->GetAttackStuff(0);

	SAttackDamageStuff sDamageStuff;
	sDamageStuff.iDamage			= pAttack->uiDamage;
	sDamageStuff.iAbsorbedDamage	= pAttack->uiAbsorbedDamage;
	sDamageStuff.iReflectedDamage	= pAttack->uiReflectedDamage;

	FillAttackStuff(pAttStuff, pAttack->hAttackerSerialId, pAttack->hDefenderSerialId, pAttack->byAttRes, pAttack->bChainAtt,
		pAttack->byAttSeq, pAttack->byBlocking, sDamageStuff, pAttack->vPushDir, NTL_ATTACK_LOGIC_NORMAL, TRUE, 0, pAttack->uiSyncEventId);

	NTL_RETURNVOID();
}

void FSMEvent_CharActHit(CNtlSobActor *pActor, SNtlEventSobHit *pHit)
{
	NTL_FUNCTION("FSMEvent_CharActHit");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SHitStuff *pHitStuff = pBeData->GetHitStuff(); 
	pHitStuff->Reset();

	memcpy(pHitStuff, &pHit->sHitStuff, sizeof(SHitStuff));

	HitTargetEffectDirect(pActor, pHitStuff);
	
	HitResultDirect(pActor, pHitStuff);
	
	HitStatusStealSkillDirect(pActor, pHitStuff);

	NTL_RETURNVOID();
}

void FSMEvent_CharActHit( CNtlSobActor* pActor, BYTE byAttackResult, WORD wAttackResultValue ) 
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
    SHitStuff *pHitStuff = pBeData->GetHitStuff(); 
    pHitStuff->Reset();

    pHitStuff->uiAttr.bitResult = byAttackResult;
    pHitStuff->uiAttr.bitBlocking = DBO_GUARD_TYPE_INVALID;
    pHitStuff->sDamageStuff.iDamage = wAttackResultValue;

    HitResultDirect(pActor, pHitStuff);
}

void FSMEvent_CharActStanding(CNtlSobActor *pActor, SNtlEventSobStanding *pStanding)
{
	NTL_FUNCTION("FSMEvent_CharActStanding");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	pMoveStuff->Reset();

	pMoveStuff->byType = NTL_MOVETARGET_LOC;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	RwV3dAssign(&pMoveStuff->vDest, &pStanding->vLoc);
	RwV3dAssign(&pMoveStuff->vDir, &pStanding->vDir);

	NTL_RETURNVOID();
}

void FSMEvent_CharActJump(CNtlSobActor *pActor, SNtlEventSobJump *pJump)
{
	NTL_FUNCTION("FSMEvent_CharActJump");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	SJumpStuff *pJumpStuff = pBeData->GetJumpStuff();

	pMoveStuff->Reset();
	pJumpStuff->Reset();

	pMoveStuff->byType = NTL_MOVETARGET_DIR;
	pMoveStuff->byForm = NTL_MOVEFORM_GROUND;
	pMoveStuff->byFormFlag = NTL_MOVE_FLAG_RUN;
	pMoveStuff->byMoveFlags = pJump->byMoveFlags;
	RwV3dAssign(&pJumpStuff->vJumpDir, &pJump->vJumpDir);

	pActor->SetDirection(&pJump->vDir);
	
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActSit(CNtlSobActor *pActor, SNtlEventSobSit *pSit)
{
	NTL_FUNCTION("FSMEvent_CharActSit");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SMoveStuff *pMoveStuff = pBeData->GetMoveStuff();
	pMoveStuff->Reset();

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActDirectPlay(CNtlSobActor *pActor, SNtlEventSobDirectPlay *pDirectPlay)
{
	NTL_FUNCTION("FSMEvent_CharActDirectPlay");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sDirection.bAnimUse		= FALSE;
	pCtrlStuff->uExtra.sDirection.byDirectType	= pDirectPlay->byDirectType;
	pCtrlStuff->uExtra.sDirection.uiTblId		= pDirectPlay->uiTblId;
	pCtrlStuff->uExtra.sDirection.bTimeLoop		= pDirectPlay->bTimeLoop;
	pCtrlStuff->uExtra.sDirection.fLoopTime		= pDirectPlay->fLoopTime;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActAnimPlay(CNtlSobActor *pActor, SNtlEventSobAnimPlay *pAnimPlay)
{
	NTL_FUNCTION("FSMEvent_CharActDirectPlay");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();

	pCtrlStuff->uExtra.sDirection.bAnimUse		= TRUE;
	pCtrlStuff->uExtra.sDirection.uiAnimId		= pAnimPlay->uiAnimId;
	pCtrlStuff->uExtra.sDirection.bTimeLoop		= pAnimPlay->bTimeLoop;
	pCtrlStuff->uExtra.sDirection.fLoopTime		= pAnimPlay->fLoopTime;
	
	NTL_RETURNVOID();
}


void FSMEvent_CharActPrivateShop(CNtlSobActor *pActor, SNtlEventSobPrivateShop *pPrivateShop)
{
	NTL_FUNCTION("FSMEvent_CharActPrivateShop");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	SCtrlStuffPrivateShop *pPrivateShopStuff = &pCtrlStuff->uExtra.sPrivateShop;

	wcscpy_s(pPrivateShopStuff->wchShopName, NTL_MAX_PRIVATESHOP_NAME, pPrivateShop->wcShopName);
	pPrivateShopStuff->bShopOwnerEmpty	= pPrivateShop->bIsOwnerEmpty;
	pPrivateShopStuff->byShopState		= pPrivateShop->byShopState;

	NTL_RETURNVOID();
}

void FSMEvent_CharActPrivateShopState(CNtlSobActor *pActor, SNtlEventPrivateShopState *pPrivateShopState)
{
	NTL_FUNCTION("FSMEvent_CharActPrivateShopState");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	SCtrlStuffPrivateShop *pPrivateShopStuff = &pCtrlStuff->uExtra.sPrivateShop;

	pPrivateShopStuff->byShopState = (RwUInt8)pPrivateShopState->uiPrivateShopState;

	if (pPrivateShopState->pPrivateShopData)
	{
		wcscpy_s(pPrivateShopStuff->wchShopName, NTL_MAX_PRIVATESHOP_NAME, pPrivateShopState->pPrivateShopData->sSummaryPrivateShopData.wcPrivateShopName);
		pPrivateShopStuff->bShopOwnerEmpty = pPrivateShopState->pPrivateShopData->sSummaryPrivateShopData.bIsOwnerEmpty;
	}

	NTL_RETURNVOID();
}


void FSMEvent_CharActSkillCasting(CNtlSobActor *pActor, SNtlEventSobSkillCasting *pSkillCasting)
{
	NTL_FUNCTION("FSMEvent_CharActSkillCasting");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
	pSkillStuff->Reset();
	pSkillStuff->hTargetSerialId	= pSkillCasting->hTargetSerialId;
    pActor->SetTargetID(pSkillCasting->hTargetSerialId);
	pSkillStuff->uiSkillTblId		= pSkillCasting->uiSkillTblId;
	pSkillStuff->fCurrCastingTime	= pSkillCasting->fCurrCastingTime;
	pSkillStuff->fMaxCastingTime	= pSkillCasting->fMaxCastingTime;

	if(pSkillCasting->hSerialId != pSkillCasting->hTargetSerialId)
	{
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		pCtrlStuff->hTargetSerialId = pSkillCasting->hTargetSerialId;
	}

	NTL_RETURNVOID();
}

void FSMEvent_CharActSkillAction(CNtlSobActor *pActor, SNtlEventSobSkillAction *pSkillAction)
{
	NTL_FUNCTION("FSMEvent_CharActSkillAction");

	// skill data setting
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
	pSkillStuff->Reset();
	pSkillStuff->hTargetSerialId = pSkillAction->hTargetSerialId;
	pSkillStuff->uiSkillTblId = pSkillAction->uiSkillTblId;
	pSkillStuff->byRpBonusType = pSkillAction->byRpBonusType;

	// attack data setting
	CNtlAttackData *pAttData = pBeData->GetAttackData();
	pAttData->Reset();
	pAttData->SetAttackStuffCount(pSkillAction->byResultCount);
	pAttData->SetAttackStuffLogic(NTL_ATTACK_LOGIC_SKILL);

	for(RwInt32 i = 0; i < pSkillAction->byResultCount; ++i)
	{
		SAttackStuff *pAttStuff = pAttData->GetAttackStuff(i);
		sSKILL_RESULT *pActRes = &pSkillAction->pSkillAct[i];
		sVECTOR3 *pActPush = &pActRes->vShift;
		
		RwV3d vPushDir;
		CNtlMath::MathRwV3dAssign(&vPushDir, pActPush->x, pActPush->y, pActPush->z);               
		RwV3dNormalize(&vPushDir, &vPushDir);

		SAttackDamageStuff sDamageStuff;
        ZeroMemory(&sDamageStuff, sizeof(SAttackDamageStuff));

        for(int i = 0; i < 2; ++i)
        {
            sDBO_SYSTEM_EFFECT_RESULT* pResult = (i == 0) ? &pActRes->effectResult1 : &pActRes->effectResult2;

            switch(pResult->eResultType)
            {
            case DBO_SYSTEM_EFFECT_RESULT_TYPE_GENERAL:                
                sDamageStuff.iDamage			+= (DWORD)pResult->fResultValue;
                sDamageStuff.iAbsorbedDamage	= 0;
                sDamageStuff.iReflectedDamage	= 0;
                break;
            case DBO_SYSTEM_EFFECT_RESULT_TYPE_DH_HOT:
            case DBO_SYSTEM_EFFECT_RESULT_TYPE_DD_DOT:                
                sDamageStuff.iDamage			+= (DWORD)pResult->DD_DOT_fDamage;
                sDamageStuff.iAbsorbedDamage	= (DWORD)((pActRes->effectResult1).DD_DOT_fAbsorbedDamage) + (DWORD)((pActRes->effectResult2).DD_DOT_fAbsorbedDamage);
                sDamageStuff.iReflectedDamage	= (DWORD)((pActRes->effectResult1).DD_DOT_fReflectedDamage) + (DWORD)((pActRes->effectResult2).DD_DOT_fReflectedDamage);
                break;
            case DBO_SYSTEM_EFFECT_RESULT_TYPE_LP_EP_STEAL:
                sDamageStuff.iDamage                = (RwInt32)pResult->fResultValue;
                sDamageStuff.LP_EP_STEAL_fLpDamage  = pResult->LP_EP_STEAL_fLpDamage;
                sDamageStuff.LP_EP_STEAL_fEpDamage  = pResult->LP_EP_STEAL_fEpDamage;
                sDamageStuff.LP_EP_STEAL_fLpGained  = pResult->LP_EP_STEAL_fLpGained;
                sDamageStuff.LP_EP_STEAL_fEpGained  = pResult->LP_EP_STEAL_fEpGained;
                sDamageStuff.LP_EP_STEAL_fAbsorbedDamage = pResult->DD_DOT_fAbsorbedDamage;
                sDamageStuff.LP_EP_STEAL_fReflectedDamage = pResult->DD_DOT_fReflectedDamage;            
                break;
            }
        }
		
		FillAttackStuff(pAttStuff, pActor->GetSerialID(), pActRes->hTarget, pActRes->byAttackResult, FALSE, 0, pActRes->byBlockedAction,
						sDamageStuff, vPushDir, NTL_ATTACK_LOGIC_SKILL,	TRUE, pSkillAction->uiSkillTblId, pSkillAction->uiSyncEventId);
	}

	if(pSkillAction->hSerialId != pSkillAction->hTargetSerialId)
	{
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		pCtrlStuff->hTargetSerialId = pSkillAction->hTargetSerialId;
	}

	NTL_RETURNVOID();
}

void FSMEvent_CharActHTBSkillAction(CNtlSobActor *pActor, SNtlEventSobHTBSkillAction *pHTBSkillAction)
{
	NTL_FUNCTION("FSMEvent_CharActHTBSkillAction");

	sCHARSTATE_HTB *pHTB = &pHTBSkillAction->pCharState->sCharStateDetail.sCharStateHTB;

	// skill data setting
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SHTBStuff *pHTBStuff = pBeData->GetHTBStuff();
	pHTBStuff->Reset();

	pHTBStuff->hAttackSerialId = pActor->GetSerialID();
	pHTBStuff->hTargetSerialId = pHTB->hTarget;
	pHTBStuff->uiSkillTblId = pHTB->HTBId;
	pHTBStuff->byCurrStep = pHTB->byCurStep;
	pHTBStuff->byStepCount = pHTB->byStepCount;
	pHTBStuff->byResultCount = pHTB->byResultCount;
	memcpy(pHTBStuff->sResult, pHTB->aHTBSkillResult, sizeof(sHTBSKILL_RESULT)*pHTB->byResultCount);

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = pHTB->hTarget;

	NTL_RETURNVOID();
}

void FSMEvent_CharActSocialAction(CNtlSobActor *pActor, SNtlEventSobSocialAction *pSocialAction)
{
	NTL_FUNCTION("FSMEvent_CharActSocialAction");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 

	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
	pSkillStuff->Reset();
	pSkillStuff->uiSkillTblId = pSocialAction->uiTblIdx;

	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->Reset();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActItemCasting(CNtlSobActor *pActor, SNtlEventSobItemCasting *pItemCasting)
{
	NTL_FUNCTION("FSMEvent_CharActItemCasting");

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
	
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
	pSkillStuff->Reset();
	pSkillStuff->hTargetSerialId	= pItemCasting->hTargetSerialId;
	pSkillStuff->uiSkillTblId		= pItemCasting->uiItemTblId;
	pSkillStuff->fCurrCastingTime	= pItemCasting->fCurrCastingTime;
	pSkillStuff->fMaxCastingTime	= pItemCasting->fMaxCastingTime;
	
	NTL_RETURNVOID();
}

void FSMEvent_CharActFainting(CNtlSobActor *pActor, SNtlEventSobFainting *pFainting)
{
	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
	SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
	pCtrlStuff->hTargetSerialId = INVALID_SERIAL_ID;
	pCtrlStuff->fTime = 0.0f;
}

void FSMEvent_CharActSobPushing( CNtlSobActor* pActor, SNtlEventSobPushingNfy* pPushingNfy ) 
{
    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData());
    SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
    pCtrlStuff->uExtra.sPushing.byAttackResult = pPushingNfy->byAttackResult;
    pCtrlStuff->uExtra.sPushing.wAttackResultValue = pPushingNfy->wAttackResultValue;
    pCtrlStuff->uExtra.sPushing.vDestPos = pPushingNfy->vDestPos;
}
