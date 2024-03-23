#include "stdafx.h"
#include "NtlBitFlagManager.h"
#include "NtlPacketGU.h"
#include "GameServer.h"
#include "char.h"
#include "SystemEffectTable.h"

#include "calcs.h"



CCharacterAtt::CCharacterAtt()
{
	Init();
}

CCharacterAtt::~CCharacterAtt()
{
}


bool CCharacterAtt::Create(CCharacter* pChar)
{
	m_pOwnerRef = pChar;
	return true;
}


void CCharacterAtt::Init()
{
	m_pOwnerRef = NULL;
	memset(&m_pAttribute, 0, sizeof(sAVATAR_ATTRIBUTE));
	m_byBattle_Attribute_Ofence = BATTLE_ATTRIBUTE_NONE;
	m_byBattle_Attribute_Defence = BATTLE_ATTRIBUTE_NONE;
	m_wSubWeaponPhysicalOffence = 0;
	m_wSubWeaponEnergyOffence = 0;
}

void CCharacterAtt::CalculateAll()
{
	//set backup
	m_fRunSpeedBackup = m_pAttribute.fLastRunSpeed;
	m_wAttackSpeedBackup = m_pAttribute.wLastAttackSpeedRate;
	m_fCurLpPercent = m_pOwnerRef->GetCurLpInPercent();
	m_fCurEpPercent = m_pOwnerRef->GetCurEpInPercent();
	m_fCurApPercent = m_pOwnerRef->GetCurApInPercent();

	//set to zero to avoid bug that stats keep increasing instead
	memset(&m_percentValue, 0, sizeof(m_percentValue));

	//set to zero to avoid bug that stats keep increasing instead
	memset(&m_pAttribute, 0, sizeof(sAVATAR_ATTRIBUTE));

	//calculate the stats
	CalculateAtt();


	//check if our LP changed
	if (m_fCurLpPercent > 0.0f && m_fCurLpPercent < 101.0f && m_fCurLpPercent != m_pOwnerRef->GetCurLpInPercent())
	{
		m_pOwnerRef->SetCurLP(int((float)m_pAttribute.dwLastMaxLP * m_fCurLpPercent / 100.f));

		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_LP));
		sGU_UPDATE_CHAR_LP* res = (sGU_UPDATE_CHAR_LP*)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_LP;
		res->handle = m_pOwnerRef->GetID();
		res->dwLpEpEventId = INVALID_DWORD;
		res->curLp = m_pOwnerRef->GetCurLP();
		res->maxLp = m_pAttribute.dwLastMaxLP;
		m_pOwnerRef->Broadcast(&packet);
	}

	//check if our EP changed
	if (m_fCurEpPercent > 0.0f && m_fCurEpPercent < 101.0f && m_fCurEpPercent != m_pOwnerRef->GetCurEpInPercent())
	{
		m_pOwnerRef->SetCurEP(WORD((float)m_pAttribute.wLastMaxEP * m_fCurEpPercent / 100.f));

		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_EP));
		sGU_UPDATE_CHAR_EP* res = (sGU_UPDATE_CHAR_EP*)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_EP;
		res->handle = m_pOwnerRef->GetID();
		res->dwLpEpEventId = INVALID_DWORD;
		res->wCurEP = m_pOwnerRef->GetCurEP();
		res->wMaxEP = m_pAttribute.wLastMaxEP;
		m_pOwnerRef->Broadcast(&packet);
	}

	//check if our AP changed
	if (m_fCurApPercent > 0.0f && m_fCurApPercent < 101.0f && m_fCurApPercent != m_pOwnerRef->GetCurApInPercent())
	{
		m_pOwnerRef->SetCurAP(int((float)m_pAttribute.dwLastMaxAp * m_fCurApPercent / 100.f));

		CNtlPacket packet(sizeof(sGU_UPDATE_CHAR_AP));
		sGU_UPDATE_CHAR_AP* res = (sGU_UPDATE_CHAR_AP*)packet.GetPacketData();
		res->wOpCode = GU_UPDATE_CHAR_AP;
		res->handle = m_pOwnerRef->GetID();
		res->curAP = m_pOwnerRef->GetCurAP();
		res->BasemaxAP = m_pAttribute.dwBaseMaxAp;
		res->LastmaxAP = m_pAttribute.dwLastMaxAp;
		m_pOwnerRef->SendPacket(&packet);
	}
}

void CCharacterAtt::CalculateAtt()
{
	if (m_pOwnerRef->IsPC())
	{
		sAVATAR_ATTRIBUTE_LINK& newavt = CNtlAvatar::GetInstance()->ConvertAVATAR_ATTRIBUTE(&m_pAttribute);
		CNtlBitFlagManager flagMgr;

		if (false == flagMgr.Create(&m_pAttribute, AVATAR_ATTRIBUTE_COUNT))
		{
			ERR_LOG(LOG_USER, "CalculateAtt: flagMgr.Create == FALSE.");
			return;
		}

		for (DWORD dwIndex = ATTRIBUTE_TO_UPDATE_FIRST; dwIndex < AVATAR_ATTRIBUTE_COUNT; dwIndex++)
			flagMgr.Set(dwIndex);

		DWORD datasize = 0;

		CNtlPacket packet(sizeof(sGU_AVATAR_ATTRIBUTE_UPDATE));
		sGU_AVATAR_ATTRIBUTE_UPDATE* res = (sGU_AVATAR_ATTRIBUTE_UPDATE*)packet.GetPacketData();
		res->wOpCode = GU_AVATAR_ATTRIBUTE_UPDATE;
		res->hHandle = m_pOwnerRef->GetID();
		res->byAttributeTotalCount = AVATAR_ATTRIBUTE_COUNT;

		if (CNtlAvatar::GetInstance()->SaveAvatarAttribute(&flagMgr, &newavt, res->abyFlexibleField, &datasize) == false)
		{
			ERR_LOG(LOG_USER, "CalculateAtt: save avatar attribute failed ");
			return;
		}

		packet.SetPacketLen(sizeof(sGU_AVATAR_ATTRIBUTE_UPDATE));
		m_pOwnerRef->Broadcast(&packet);
	}

	if (m_fRunSpeedBackup != m_pAttribute.fLastRunSpeed)
	{
		m_pOwnerRef->UpdateMoveSpeed(GetWalkSpeed(), m_pAttribute.fLastRunSpeed);

		if (m_pAttribute.fLastRunSpeed <= 0.0f)
		{
			if (m_pOwnerRef->GetCharStateID() == CHARSTATE_DASH_PASSIVE)
			{
				m_pOwnerRef->SendCharStateStanding();
			}
			else if (m_pOwnerRef->GetCharStateID() == CHARSTATE_SKILL_AFFECTING && m_pOwnerRef->GetMoveFlag() == NTL_MOVE_FLAG_DASH)
			{
				m_pOwnerRef->SendCharStateStanding();
			}
		}
	}

	if (m_wAttackSpeedBackup != m_pAttribute.wLastAttackSpeedRate)
	{
		m_pOwnerRef->UpdateAttackSpeed(m_pAttribute.wLastAttackSpeedRate);
	}
}

void CCharacterAtt::CalculatePercentValues()
{
	if (m_percentValue.m_fSTR != 0.f)
		CalculateStr(NtlRound((float)m_pAttribute.wLastStr * m_percentValue.m_fSTR / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fSTRNegative != 0.f)
		CalculateStr(NtlRound(fabs((float)m_pAttribute.wLastStr * m_percentValue.m_fSTRNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fCON != 0.f)
		CalculateCon(NtlRound((float)m_pAttribute.wLastCon * m_percentValue.m_fCON / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fCONNegative != 0.f)
		CalculateCon(NtlRound(fabs((float)m_pAttribute.wLastCon * m_percentValue.m_fCONNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fFOC != 0.f)
		CalculateFoc(NtlRound((float)m_pAttribute.wLastFoc * m_percentValue.m_fFOC / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fFOCNegative != 0.f)
		CalculateFoc(NtlRound(fabs((float)m_pAttribute.wLastFoc * m_percentValue.m_fFOCNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fDEX != 0.f)
		CalculateDex(NtlRound((float)m_pAttribute.wLastDex * m_percentValue.m_fDEX / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fDEXNegative != 0.f)
		CalculateDex(NtlRound(fabs((float)m_pAttribute.wLastDex * m_percentValue.m_fDEXNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fSOL != 0.f)
		CalculateSol(NtlRound((float)m_pAttribute.wLastSol * m_percentValue.m_fSOL / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fSOLNegative != 0.f)
		CalculateSol(NtlRound(fabs((float)m_pAttribute.wLastSol * m_percentValue.m_fSOLNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fENG != 0.f)
		CalculateEng(NtlRound((float)m_pAttribute.wLastEng * m_percentValue.m_fENG / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fENGNegative != 0.f)
		CalculateEng(NtlRound(fabs((float)m_pAttribute.wLastEng * m_percentValue.m_fENGNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fLP != 0.f)
		CalculateMaxLP(NtlRound((float)m_pAttribute.dwLastMaxLP * m_percentValue.m_fLP / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fLPNegative != 0.f)
		CalculateMaxLP(NtlRound(fabs((float)m_pAttribute.dwLastMaxLP * m_percentValue.m_fLPNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEP != 0.f)
		CalculateMaxEP(NtlRound((float)m_pAttribute.wLastMaxEP * m_percentValue.m_fEP / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEPNegative != 0.f)
		CalculateMaxEP(NtlRound(fabs((float)m_pAttribute.wLastMaxEP * m_percentValue.m_fEPNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fAP != 0.f)
		CalculateMaxAP(NtlRound((float)m_pAttribute.dwLastMaxAp * m_percentValue.m_fAP / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fAPNegative != 0.f)
		CalculateMaxAP(NtlRound(fabs((float)m_pAttribute.dwLastMaxAp * m_percentValue.m_fAPNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fRP != 0.f)
		CalculateMaxRP(NtlRound((float)m_pAttribute.wLastMaxRP * m_percentValue.m_fRP / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fRPNegative != 0.f)
		CalculateMaxRP(NtlRound(fabs((float)m_pAttribute.wLastMaxRP * m_percentValue.m_fRPNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fLpRegen != 0.f)
		CalculateLpRegen(NtlRound((float)m_pAttribute.wLastLpRegen * m_percentValue.m_fLpRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fLpRegenNegative != 0.f)
		CalculateLpRegen(NtlRound(fabs((float)m_pAttribute.wLastLpRegen * m_percentValue.m_fLpRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fLpSitdownRegen != 0.f)
		CalculateLpSitdownRegen(NtlRound((float)m_pAttribute.wLastLpSitdownRegen * m_percentValue.m_fLpSitdownRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fLpSitdownRegenNegative != 0.f)
		CalculateLpSitdownRegen(NtlRound(fabs((float)m_pAttribute.wLastLpSitdownRegen * m_percentValue.m_fLpSitdownRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fLpBattleRegen != 0.f)
		CalculateLpBattleRegen(NtlRound((float)m_pAttribute.wLastLpBattleRegen * m_percentValue.m_fLpBattleRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fLpBattleRegenNegative != 0.f)
		CalculateLpBattleRegen(NtlRound(fabs((float)m_pAttribute.wLastLpBattleRegen * m_percentValue.m_fLpBattleRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEpRegen != 0.f)
		CalculateEpRegen(NtlRound((float)m_pAttribute.wLastEpRegen * m_percentValue.m_fEpRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEpRegenNegative != 0.f)
		CalculateEpRegen(NtlRound(fabs((float)m_pAttribute.wLastEpRegen * m_percentValue.m_fEpRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEpSitdownRegen != 0.f)
		CalculateEpSitdownRegen(NtlRound((float)m_pAttribute.wLastEpSitdownRegen * m_percentValue.m_fEpSitdownRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEpSitdownRegenNegative != 0.f)
		CalculateEpSitdownRegen(NtlRound(fabs((float)m_pAttribute.wLastEpSitdownRegen * m_percentValue.m_fEpSitdownRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEpBattleRegen != 0.f)
		CalculateEpBattleRegen(NtlRound((float)m_pAttribute.wLastEpBattleRegen * m_percentValue.m_fEpBattleRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEpBattleRegenNegative != 0.f)
		CalculateEpBattleRegen(NtlRound(fabs((float)m_pAttribute.wLastEpBattleRegen * m_percentValue.m_fEpBattleRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fApRegen != 0.f)
		CalculateApRegen(NtlRound((float)m_pAttribute.wLastApRegen * m_percentValue.m_fApRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fApRegenNegative != 0.f)
		CalculateApRegen(NtlRound(fabs((float)m_pAttribute.wLastApRegen * m_percentValue.m_fApRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fApSitdownRegen != 0.f)
		CalculateApSitdownRegen(NtlRound((float)m_pAttribute.wLastApSitdownRegen * m_percentValue.m_fApSitdownRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fApSitdownRegenNegative != 0.f)
		CalculateApSitdownRegen(NtlRound(fabs((float)m_pAttribute.wLastApSitdownRegen * m_percentValue.m_fApSitdownRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fApDegen != 0.f)
		CalculateApDegen(NtlRound((float)m_pAttribute.wAirAP_Degen * m_percentValue.m_fApDegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fApDegenNegative != 0.f)
		CalculateApDegen(NtlRound(fabs((float)m_pAttribute.wAirAP_Degen * m_percentValue.m_fApDegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fRpRegen != 0.f)
		CalculateRpRegen(NtlRound((float)m_pAttribute.wLastRpRegen * m_percentValue.m_fRpRegen / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fRpRegenNegative != 0.f)
		CalculateRpRegen(NtlRound(fabs((float)m_pAttribute.wLastRpRegen * m_percentValue.m_fRpRegenNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	//if (m_percentValue.m_fRpDimimutionRate != 0.f)
	//	Calculaterpdim((float)m_pAttribute.wRpDimimutionRate * m_percentValue.m_fRpDimimutionRate / 100.f, SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);

	if (m_percentValue.m_fPhysicalOffence != 0.f)
		CalculatePhysicalOffence(NtlRound((float)m_pAttribute.wLastPhysicalOffense * m_percentValue.m_fPhysicalOffence / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fPhysicalOffenceNegative != 0.f)
		CalculatePhysicalOffence(NtlRound(fabs((float)m_pAttribute.wLastPhysicalOffense * m_percentValue.m_fPhysicalOffenceNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fPhysicalDefence != 0.f)
		CalculatePhysicalDefence(NtlRound((float)m_pAttribute.wLastPhysicalDefense * m_percentValue.m_fPhysicalDefence / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fPhysicalDefenceNegative != 0.f)
		CalculatePhysicalDefence(NtlRound(fabs((float)m_pAttribute.wLastPhysicalDefense * m_percentValue.m_fPhysicalDefenceNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEnergyOffence != 0.f)
		CalculateEnergyOffence(NtlRound((float)m_pAttribute.wLastEnergyOffense * m_percentValue.m_fEnergyOffence / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEnergyOffenceNegative != 0.f)
		CalculateEnergyOffence(NtlRound(fabs((float)m_pAttribute.wLastEnergyOffense * m_percentValue.m_fEnergyOffenceNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEnergyDefence != 0.f)
		CalculateEnergyDefence(NtlRound((float)m_pAttribute.wLastEnergyDefense * m_percentValue.m_fEnergyDefence / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEnergyDefenceNegative != 0.f)
		CalculateEnergyDefence(NtlRound(fabs((float)m_pAttribute.wLastEnergyDefense * m_percentValue.m_fEnergyDefenceNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fAttackRate != 0.f)
		CalculateAttackRate(NtlRound((float)m_pAttribute.wLastHitRate * m_percentValue.m_fAttackRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fAttackRateNegative != 0.f)
		CalculateAttackRate(NtlRound(fabs((float)m_pAttribute.wLastHitRate * m_percentValue.m_fAttackRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fDodgeRate != 0.f)
		CalculateDodgeRate(NtlRound((float)m_pAttribute.wLastDodgeRate * m_percentValue.m_fDodgeRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fDodgeRateNegative != 0.f)
		CalculateDodgeRate(NtlRound(fabs((float)m_pAttribute.wLastDodgeRate * m_percentValue.m_fDodgeRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fBlockRate != 0.f)
		CalculateBlockRate(NtlRound((float)m_pAttribute.wLastBlockRate * m_percentValue.m_fBlockRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fBlockRateNegative != 0.f)
		CalculateBlockRate(NtlRound(fabs((float)m_pAttribute.wLastBlockRate * m_percentValue.m_fBlockRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fBlockDamageRate != 0.f)
		CalculateBlockDamageRate(NtlRound((float)m_pAttribute.wLastBlockDMG * m_percentValue.m_fBlockDamageRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fBlockDamageRateNegative != 0.f)
		CalculateBlockDamageRate(NtlRound(fabs((float)m_pAttribute.wLastBlockDMG * m_percentValue.m_fBlockDamageRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fCurseSuccessRate != 0.f)
		CalculateCurseSuccessRate(NtlRound((float)m_pAttribute.wLastCurseSuccessRate * m_percentValue.m_fCurseSuccessRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fCurseSuccessRateNegative != 0.f)
		CalculateCurseSuccessRate(NtlRound(fabs((float)m_pAttribute.wLastCurseSuccessRate * m_percentValue.m_fCurseSuccessRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fCurseToleranceRate != 0.f)
		CalculateCurseToleranceRate(NtlRound((float)m_pAttribute.wLastCurseToleranceRate * m_percentValue.m_fCurseToleranceRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fCurseToleranceRateNegative != 0.f)
		CalculateCurseToleranceRate(NtlRound(fabs((float)m_pAttribute.wLastCurseToleranceRate * m_percentValue.m_fCurseToleranceRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fPhysicalCriticalRate != 0.f)
		CalculatePhysicalCriticalRate(NtlRound((float)m_pAttribute.wLastPhysicalCriticalRate * m_percentValue.m_fPhysicalCriticalRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fPhysicalCriticalRateNegative != 0.f)
		CalculatePhysicalCriticalRate(NtlRound(fabs((float)m_pAttribute.wLastPhysicalCriticalRate * m_percentValue.m_fPhysicalCriticalRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEnergyCriticalRate != 0.f)
		CalculateEnergyCriticalRate(NtlRound((float)m_pAttribute.wLastEnergyCriticalRate * m_percentValue.m_fEnergyCriticalRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEnergyCriticalRateNegative != 0.f)
		CalculateEnergyCriticalRate(NtlRound(fabs((float)m_pAttribute.wLastEnergyCriticalRate * m_percentValue.m_fEnergyCriticalRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fPhysicalCriticalDamageRate != 0.f)
		CalculatePhysicalCriticalDamageRate(NtlRound((float)m_pAttribute.fLastPhysicalCritDmgRate * m_percentValue.m_fPhysicalCriticalDamageRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fPhysicalCriticalDamageRateNegative != 0.f)
		CalculatePhysicalCriticalDamageRate(NtlRound(fabs((float)m_pAttribute.fLastPhysicalCritDmgRate * m_percentValue.m_fPhysicalCriticalDamageRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fEnergyCriticalDamageRate != 0.f)
		CalculateEnergyCriticalDamageRate(NtlRound((float)m_pAttribute.fLastEnergyCritDmgRate * m_percentValue.m_fEnergyCriticalDamageRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fEnergyCriticalDamageRateNegative != 0.f)
		CalculateEnergyCriticalDamageRate(NtlRound(fabs((float)m_pAttribute.fLastEnergyCritDmgRate * m_percentValue.m_fEnergyCriticalDamageRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);


	m_pAttribute.fLastAirSpeed += m_pAttribute.fLastAirSpeed * m_percentValue.m_fFlySpeed / 100.f;
	m_pAttribute.fLastAirDashSpeed += m_pAttribute.fLastAirDashSpeed * m_percentValue.m_fFlyDashSpeed / 100.f;
	m_pAttribute.fLastAirDashAccelSpeed += m_pAttribute.fLastAirDashAccelSpeed * m_percentValue.m_fFlyAccelSpeed / 100.f;

	if (m_percentValue.m_fAttackSpeedRate != 0.f)
		CalculateAttackSpeedRate(NtlRound((float)m_pAttribute.wLastAttackSpeedRate * m_percentValue.m_fAttackSpeedRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fAttackSpeedRateNegative != 0.f)
		CalculateAttackSpeedRate(NtlRound(fabs((float)m_pAttribute.wLastAttackSpeedRate * m_percentValue.m_fAttackSpeedRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fAttackRange != 0.f)
		CalculateAttackRange(NtlRound((float)m_pAttribute.fLastAttackRange * m_percentValue.m_fAttackRange / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fAttackRangeNegative != 0.f)
		CalculateAttackRange(NtlRound(fabs((float)m_pAttribute.fLastAttackRange * m_percentValue.m_fAttackRangeNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fStomachacheDef != 0.f)
		CalculateStomachacheDefence(NtlRound((float)m_pAttribute.wLastAbdominalPainDefense * m_percentValue.m_fStomachacheDef / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fStomachacheDefNegative != 0.f)
		CalculateStomachacheDefence(NtlRound(fabs((float)m_pAttribute.wLastAbdominalPainDefense * m_percentValue.m_fStomachacheDefNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fPoisonDef != 0.f)
		CalculatePoisonDefence(NtlRound((float)m_pAttribute.wLastPoisonDefense * m_percentValue.m_fPoisonDef / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fPoisonDefNegative != 0.f)
		CalculatePoisonDefence(NtlRound(fabs((float)m_pAttribute.wLastPoisonDefense * m_percentValue.m_fPoisonDefNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fBleedDef != 0.f)
		CalculateBleedDefence(NtlRound((float)m_pAttribute.wLastBleedingDefense * m_percentValue.m_fBleedDef / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fBleedDefNegative != 0.f)
		CalculateBleedDefence(NtlRound(fabs((float)m_pAttribute.wLastBleedingDefense * m_percentValue.m_fBleedDefNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	if (m_percentValue.m_fBurnDef != 0.f)
		CalculateBurnDefence(NtlRound((float)m_pAttribute.wLastBurnDefense * m_percentValue.m_fBurnDef / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fBurnDefNegative != 0.f)
		CalculateBurnDefence(NtlRound(fabs((float)m_pAttribute.wLastBurnDefense * m_percentValue.m_fBurnDefNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

	m_pAttribute.fLastMindCurseImmunity += m_pAttribute.fLastMindCurseImmunity * m_percentValue.m_fMindCurseImmunity / 100.f;
	m_pAttribute.fLastBodyCurseImmunity += m_pAttribute.fLastBodyCurseImmunity * m_percentValue.m_fBodyCurseImmunity / 100.f;
	m_pAttribute.fLastChangeCurseImmunity += m_pAttribute.fLastChangeCurseImmunity * m_percentValue.m_fChangeCurseImmunity / 100.f;
	m_pAttribute.fLastSkillAnimationSpeed += m_pAttribute.fLastSkillAnimationSpeed * m_percentValue.m_fSkillAnimationSpeedModifier / 100.f;

	if (m_percentValue.m_fGuardRate != 0.f)
		CalcAttributeW(m_pAttribute.wGuardRateLast, NtlRound((float)m_pAttribute.wGuardRateLast * m_percentValue.m_fGuardRate / 100.f), SYSTEM_EFFECT_APPLY_TYPE_VALUE, true);
	if (m_percentValue.m_fGuardRateNegative != 0.f)
		CalcAttributeW(m_pAttribute.wGuardRateLast, NtlRound(fabs((float)m_pAttribute.wGuardRateLast * m_percentValue.m_fGuardRateNegative / 100.f)), SYSTEM_EFFECT_APPLY_TYPE_VALUE, false);

}


void CCharacterAtt::CalculateStr(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wLastStr, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateCon(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wLastCon, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateFoc(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wLastFoc, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateDex(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wLastDex, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateSol(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wLastSol, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEng(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wLastEng, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateMaxLP(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.dwLastMaxLP += (int)fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLP += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.dwLastMaxLP -= (int)fValue;

			if (m_pAttribute.dwLastMaxLP < 0)
				m_pAttribute.dwLastMaxLP = 0;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLPNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateMaxEP(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastMaxEP = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastMaxEP, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEP += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastMaxEP = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastMaxEP, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEPNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateMaxRP(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastMaxRP = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastMaxRP, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fRP += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastMaxRP = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastMaxRP, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fRPNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateMaxAP(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.dwBaseMaxAp = m_pAttribute.dwLastMaxAp += (int)fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAP += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.dwBaseMaxAp = m_pAttribute.dwLastMaxAp -= (int)fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAPNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateLpRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastLpRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastLpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastLpRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastLpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateLpSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastLpSitdownRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastLpSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpSitdownRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastLpSitdownRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastLpSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpSitdownRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateLpBattleRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastLpBattleRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastLpBattleRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpBattleRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastLpBattleRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastLpBattleRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fLpBattleRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateEpRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEpRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastEpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEpRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateEpSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEpSitdownRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastEpSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpSitdownRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEpSitdownRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEpSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpSitdownRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateEpBattleRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEpBattleRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastEpBattleRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpBattleRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEpBattleRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEpBattleRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEpBattleRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateApRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastApRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastApRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastApRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastApRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApRegenNegative -= fValue;
		}
	}
}


void CCharacterAtt::CalculateApSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastApSitdownRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastApSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApSitdownRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastApSitdownRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastApSitdownRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApSitdownRegenNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateApDegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wAirAP_Degen = UnsignedSafeIncrease<WORD>(m_pAttribute.wAirAP_Degen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApDegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wAirAP_Degen = UnsignedSafeDecrease<WORD>(m_pAttribute.wAirAP_Degen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fApDegenNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateRpRegen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastRpRegen = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastRpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fRpRegen += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastRpRegen = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastRpRegen, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fRpRegenNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculatePhysicalOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastPhysicalOffense = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastPhysicalOffense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalOffence += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastPhysicalOffense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastPhysicalOffense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalOffenceNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateEnergyOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEnergyOffense = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastEnergyOffense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyOffence += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEnergyOffense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEnergyOffense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyOffenceNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculatePhysicalDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastPhysicalDefense = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastPhysicalDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalDefence += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastPhysicalDefense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastPhysicalDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalDefenceNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateEnergyDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEnergyDefense = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastEnergyDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyDefence += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEnergyDefense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEnergyDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyDefenceNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateAttackRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastHitRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastHitRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastHitRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastHitRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateDodgeRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastDodgeRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastDodgeRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fDodgeRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastDodgeRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastDodgeRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fDodgeRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateBlockRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastBlockRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastBlockRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBlockRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastBlockRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastBlockRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBlockRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateBlockDamageRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastBlockDMG = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastBlockDMG, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBlockDamageRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastBlockDMG = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastBlockDMG, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBlockDamageRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateCurseSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastCurseSuccessRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastCurseSuccessRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCurseSuccessRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastCurseSuccessRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastCurseSuccessRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCurseSuccessRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateCurseToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastCurseToleranceRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastCurseToleranceRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCurseToleranceRate += fValue;

			if (m_percentValue.m_fCurseToleranceRate > 60.f)
				m_percentValue.m_fCurseToleranceRate = 60.f;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastCurseToleranceRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastCurseToleranceRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fCurseToleranceRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculatePhysicalCriticalRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastPhysicalCriticalRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastPhysicalCriticalRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalCriticalRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastPhysicalCriticalRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastPhysicalCriticalRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalCriticalRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateEnergyCriticalRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEnergyCriticalRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastEnergyCriticalRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyCriticalRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastEnergyCriticalRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastEnergyCriticalRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyCriticalRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculatePhysicalCriticalDamageRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastPhysicalCritDmgRate += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalCriticalDamageRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastPhysicalCritDmgRate -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPhysicalCriticalDamageRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateEnergyCriticalDamageRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastEnergyCritDmgRate += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyCriticalDamageRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastEnergyCritDmgRate -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fEnergyCriticalDamageRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateRunSpeed(float fValue, BYTE byApplyType, bool bIsPlus)
{	
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastRunSpeed += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_pAttribute.fLastRunSpeed += m_pAttribute.fBaseRunSpeed * fValue / 100.f;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastRunSpeed -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_pAttribute.fLastRunSpeed -= m_pAttribute.fBaseRunSpeed * fValue / 100.f;
		}

		if (m_pAttribute.fLastRunSpeed < 0.f)
			m_pAttribute.fLastRunSpeed = 0.f;
	}	
}

void CCharacterAtt::CalculateFlySpeed(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastAirSpeed += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlySpeed += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastAirSpeed -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlySpeedNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateFlyDashSpeed(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastAirDashSpeed += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlyDashSpeed += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastAirDashSpeed -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlyDashSpeedNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateFlyAccelSpeed(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastAirDashAccelSpeed += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlyAccelSpeed += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastAirDashAccelSpeed -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fFlyAccelSpeedNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateAttackSpeedRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	/*
		Increase = slower
		Decrease = faster
	*/	
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastAttackSpeedRate = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastAttackSpeedRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackSpeedRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastAttackSpeedRate = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastAttackSpeedRate, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackSpeedRateNegative -= fValue;
		}

		if (m_pAttribute.wLastAttackSpeedRate < 100)
			m_pAttribute.wLastAttackSpeedRate = 100;
	}
}

void CCharacterAtt::CalculateAttackRange(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastAttackRange += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackRange += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastAttackRange -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fAttackRangeNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateCastingTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fCastingTimeChangePercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateCoolTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fCoolTimeChangePercent, fValue, byApplyType, bIsPlus, -50.f);
}

void CCharacterAtt::CalculateKeepTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fKeepTimeChangePercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateDotTimeChangeAbsolute(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fDotKeepTimeChangeInSeconds, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateRequiredEpChangePercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fRequiredEpChangePercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateBattleAttribute(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalculateHonestOffence(fValue, byApplyType, bIsPlus);
	CalculateStrangeOffence(fValue, byApplyType, bIsPlus);
	CalculateWildOffence(fValue, byApplyType, bIsPlus);
	CalculateEleganceOffence(fValue, byApplyType, bIsPlus);
	CalculateFunnyOffence(fValue, byApplyType, bIsPlus);
	CalculateHonestDefence(fValue, byApplyType, bIsPlus);
	CalculateStrangeDefence(fValue, byApplyType, bIsPlus);
	CalculateWildDefence(fValue, byApplyType, bIsPlus);
	CalculateEleganceDefence(fValue, byApplyType, bIsPlus);
	CalculateFunnyDefence(fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateHonestOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fHonestOffense, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateHonestDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fHonestDefense, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateStrangeOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fStrangeOffense, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateStrangeDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fStrangeDefense, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateWildOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fWildOffense, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateWildDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fWildDefense, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEleganceOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fEleganceOffense, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEleganceDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fEleganceDefense, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateFunnyOffence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fFunnyOffense, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateFunnyDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fFunnyDefense, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculatePhysicalReflection(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fPhysicalReflect, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEnergyReflection(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fEnergyReflect, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateParalyzeToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wParalyzeToleranceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateTerrorToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wTerrorToleranceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateConfuseToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wConfuseToleranceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateStoneToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wStoneToleranceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateCandyToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeW(m_pAttribute.wCandyToleranceRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateParalyzeKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fParalyzeKeepTimeDown, fValue, byApplyType, bIsPlus, 99.9f);
}

void CCharacterAtt::CalculateTerrorKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fTerrorKeepTimeDown, fValue, byApplyType, bIsPlus, 99.9f);
}

void CCharacterAtt::CalculateConfuseKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fConfuseKeepTimeDown, fValue, byApplyType, bIsPlus, 99.9f);
}

void CCharacterAtt::CalculateStoneKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fStoneKeepTimeDown, fValue, byApplyType, bIsPlus, 99.9f);
}

void CCharacterAtt::CalculateCandyKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fCandyKeepTimeDown, fValue, byApplyType, bIsPlus, 99.9f);
}

void CCharacterAtt::CalculateBleedingKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fBleedingKeepTimeDown, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculatePoisonKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fPoisonKeepTimeDown, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateStomachacheKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fStomachacheKeepTimeDown, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateCriticalBlockSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fCriticalBlockSuccessRate, fValue, byApplyType, bIsPlus, DBO_MAX_CRITICAL_BLOCK_SUCCESS_RATE);
}

void CCharacterAtt::CalculateLpRecoveryWhenHit(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeDW(m_pAttribute.dwLpRecoveryWhenHit, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateLpRecoveryWhenHitInPercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fLpRecoveryWhenHitInPercent, fValue, byApplyType, bIsPlus, m_pOwnerRef->IsPC() == true ? 60.f : INVALID_FLOAT);
}

void CCharacterAtt::CalculateEpRecoveryWhenHit(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeDW(m_pAttribute.dwEpRecoveryWhenHit, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEpRecoveryWhenHitInPercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fEpRecoveryWhenHitInPercent, fValue, byApplyType, bIsPlus, m_pOwnerRef->IsPC() == true ? 60.f : INVALID_FLOAT);
}

void CCharacterAtt::CalculateStomachacheDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastAbdominalPainDefense = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastAbdominalPainDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fStomachacheDef += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastAbdominalPainDefense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastAbdominalPainDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fStomachacheDefNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculatePoisonDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastPoisonDefense = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastPoisonDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPoisonDef += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastPoisonDefense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastPoisonDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fPoisonDefNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateBleedDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastBleedingDefense = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastBleedingDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBleedDef += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastBleedingDefense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastBleedingDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBleedDefNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateBurnDefence(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastBurnDefense = UnsignedSafeIncrease<WORD>(m_pAttribute.wLastBurnDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBurnDef += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wLastBurnDefense = UnsignedSafeDecrease<WORD>(m_pAttribute.wLastBurnDefense, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBurnDefNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateMindCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastMindCurseImmunity += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fMindCurseImmunity += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastMindCurseImmunity -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fMindCurseImmunityNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateBodyCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastBodyCurseImmunity += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBodyCurseImmunity += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastBodyCurseImmunity -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fBodyCurseImmunityNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateChangeCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastChangeCurseImmunity += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fChangeCurseImmunity += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastChangeCurseImmunity -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fChangeCurseImmunityNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateSkillAnimationSpeedModifier(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastSkillAnimationSpeed += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSkillAnimationSpeedModifier += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.fLastSkillAnimationSpeed -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fSkillAnimationSpeedModifierNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateSkillAggroBonus(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fSkillAggroBonus, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateSkillAggroBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fSkillAggroBonusInPercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateDirectHealPowerBonus(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fDirectHealPowerBonus, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateDirectHealPowerBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fDirectHealPowerBonusInPercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateHotPowerBonus(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fHotPowerBonus, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateHotPowerBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fHotPowerBonusInPercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateDotValueChangePercent(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fDotValueChangePercent, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculatePhysicalCriticalDefenceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fPhysicalCriticalDefenseRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateEnergyCriticalDefenceRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fEnergyCriticalDefenseRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateGuardRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wGuardRateLast += (WORD)fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fGuardRate += fValue;
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			m_pAttribute.wGuardRateLast = UnsignedSafeDecrease<WORD>(m_pAttribute.wGuardRateLast, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			m_percentValue.m_fGuardRateNegative -= fValue;
		}
	}
}

void CCharacterAtt::CalculateSkillDamageBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fSkillDamageBlockModeSuccessRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateCurseBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fCurseBlockModeSuccessRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateKnockdownBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fKnockdownBlockModeSuccessRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateHtbBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fHtbBlockModeSuccessRate, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculateExpBooster(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeBYTE(m_pAttribute.byExpBooster, fValue, byApplyType, bIsPlus);
}

void CCharacterAtt::CalculatePhysicalArmorPen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fPhysicalCriticalDefenseRate, fValue, byApplyType, bIsPlus);//Xanu
}

void CCharacterAtt::CalculateEnergyArmorPen(float fValue, BYTE byApplyType, bool bIsPlus)
{
	CalcAttributeF(m_pAttribute.fEnergyCriticalDefenseRate, fValue, byApplyType, bIsPlus);//Xanu
}
