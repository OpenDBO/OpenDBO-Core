#include "stdafx.h"
#include "CharacterAttBot.h"
#include "Monster.h"
#include "FormulaTable.h"
#include "calcs.h"


CCharacterAttBot::CCharacterAttBot()
{
	m_pBotRef = NULL;
}

CCharacterAttBot::~CCharacterAttBot()
{
}


bool CCharacterAttBot::Create(CNpc* pBot)
{
	if (CCharacterAtt::Create(pBot))
	{
		m_pBotRef = pBot;
		return true;
	}

	return false;
}


void CCharacterAttBot::CalculateAtt()
{
	sBOT_TBLDAT* pBotTbldat = reinterpret_cast<sBOT_TBLDAT*>(m_pBotRef->GetTbldat());
	sCHAR_TBLDAT* charTbldat = reinterpret_cast<sCHAR_TBLDAT*>(m_pBotRef->GetTbldat());

	BYTE byGrade = pBotTbldat->byGrade;
	BYTE byLv = pBotTbldat->byLevel;
	BYTE byEfLv = m_pBotRef->GetEffectiveLevel();

	m_pAttribute.dwBaseMaxLP = m_pAttribute.dwLastMaxLP = (int)charTbldat->dwBasic_LP;
	m_pAttribute.dwBaseMaxLP = m_pAttribute.dwLastMaxLP += (byEfLv - byLv) * ((m_pAttribute.dwBaseMaxLP / byLv) * (byGrade + 1));

	m_pAttribute.wBaseMaxEP = m_pAttribute.wLastMaxEP = charTbldat->wBasic_EP;
	m_pAttribute.wBaseMaxEP = m_pAttribute.wLastMaxEP += (byEfLv - byLv) * ((m_pAttribute.wBaseMaxEP / byLv) * (byGrade + 1));

	m_pAttribute.wBasePhysicalDefense = m_pAttribute.wLastPhysicalDefense = charTbldat->wBasic_Physical_Defence;
	m_pAttribute.wBasePhysicalDefense = m_pAttribute.wLastPhysicalDefense += (byEfLv - byLv) * ((m_pAttribute.wBasePhysicalDefense / byLv) * (byGrade + 1));

	m_pAttribute.wBaseEnergyDefense = m_pAttribute.wLastEnergyDefense = charTbldat->wBasic_Energy_Defence;
	m_pAttribute.wBaseEnergyDefense = m_pAttribute.wLastEnergyDefense += (byEfLv - byLv) * ((m_pAttribute.wBaseEnergyDefense / byLv) * (byGrade + 1));

	m_pAttribute.wBasePhysicalOffense = m_pAttribute.wLastPhysicalOffense = pBotTbldat->wBasic_Physical_Offence;
	m_pAttribute.wBasePhysicalOffense = m_pAttribute.wLastPhysicalOffense += (byEfLv - byLv) * ((m_pAttribute.wBasePhysicalOffense / byLv) * (byGrade + 1));

	m_pAttribute.wBaseEnergyOffense = m_pAttribute.wLastEnergyOffense = pBotTbldat->wBasic_Energy_Offence;
	m_pAttribute.wBaseEnergyOffense = m_pAttribute.wLastEnergyOffense += (byEfLv - byLv) * ((m_pAttribute.wBaseEnergyOffense / byLv) * (byGrade + 1));

	m_pAttribute.wBaseStr = m_pAttribute.wLastStr = charTbldat->wBasicStr;
	m_pAttribute.wBaseCon = m_pAttribute.wLastCon = charTbldat->wBasicCon;
	m_pAttribute.wBaseFoc = m_pAttribute.wLastFoc = charTbldat->wBasicFoc;
	m_pAttribute.wBaseDex = m_pAttribute.wLastDex = charTbldat->wBasicDex;
	m_pAttribute.wBaseSol = m_pAttribute.wLastSol = charTbldat->wBasicSol;
	m_pAttribute.wBaseEng = m_pAttribute.wLastEng = charTbldat->wBasicEng;
	m_pAttribute.wBaseAttackSpeedRate = m_pAttribute.wLastAttackSpeedRate = charTbldat->wAttack_Speed_Rate;
	m_pAttribute.fBaseAttackRange = m_pAttribute.fLastAttackRange = charTbldat->fAttack_Range;

	m_pAttribute.wBaseBlockRate = m_pAttribute.wLastBlockRate = charTbldat->wBlock_Rate;
	m_pAttribute.wBaseCurseSuccessRate = m_pAttribute.wLastCurseSuccessRate = charTbldat->wCurse_Success_Rate;
	m_pAttribute.wBaseCurseToleranceRate = m_pAttribute.wLastCurseToleranceRate = charTbldat->wCurse_Tolerance_Rate;

	m_pAttribute.wBaseEpRegen = m_pAttribute.wLastEpRegen = pBotTbldat->wEP_Regeneration;
	m_pAttribute.wBaseEpSitdownRegen = m_pAttribute.wLastEpSitdownRegen = m_pAttribute.wBaseMaxEP * 20 / 100;
	m_pAttribute.wBaseEpBattleRegen = m_pAttribute.wLastEpBattleRegen = pBotTbldat->wEP_Regeneration;
	m_byBattle_Attribute_Ofence = pBotTbldat->byBattle_Attribute;
	m_byBattle_Attribute_Defence = pBotTbldat->byBattle_Attribute;
	m_pAttribute.fBaseRunSpeed = m_pAttribute.fLastRunSpeed = pBotTbldat->fRun_Speed;
	m_pAttribute.fBaseAirSpeed = m_pAttribute.fLastAirSpeed = pBotTbldat->fRun_Speed;
	m_pAttribute.fBaseAirDashSpeed = m_pAttribute.fLastAirDashSpeed = pBotTbldat->fRun_Speed;
	m_pAttribute.fBaseAirDashAccelSpeed = m_pAttribute.fLastAirDashAccelSpeed = pBotTbldat->fRun_Speed;

	m_pAttribute.wBaseAbdominalPainDefense = m_pAttribute.wLastAbdominalPainDefense = pBotTbldat->wStomachacheDefence;
	m_pAttribute.wBasePoisonDefense = m_pAttribute.wLastPoisonDefense = pBotTbldat->wPoisonDefence;
	m_pAttribute.wBaseBleedingDefense = m_pAttribute.wLastBleedingDefense = pBotTbldat->wBleedDefence;
	m_pAttribute.wBaseBurnDefense = m_pAttribute.wLastBurnDefense = pBotTbldat->wBurnDefence;

	m_pAttribute.fBaseSkillAnimationSpeed = m_pAttribute.fLastSkillAnimationSpeed = 100.0f;

	m_pAttribute.wBaseHitRate = m_pAttribute.wLastHitRate = WORD(m_pAttribute.wBaseFoc / CFormulaTable::m_afRate[600][1] * CFormulaTable::m_afRate[600][2]);
	m_pAttribute.wBaseDodgeRate = m_pAttribute.wLastDodgeRate = WORD(m_pAttribute.wBaseDex / CFormulaTable::m_afRate[800][1] * CFormulaTable::m_afRate[800][2]);

	if (m_pAttribute.wBaseAttackSpeedRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "0Attack Rate is invalid. Foc: %u, formula 1: %f, formula 2: %f", m_pAttribute.wBaseFoc, CFormulaTable::m_afRate[600][1], CFormulaTable::m_afRate[600][2]);
	}
	if (m_pAttribute.wBaseDodgeRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "0Dodge Rate is invalid. Dex: %u, formula 1: %f, formula 2: %f", m_pAttribute.wBaseDex, CFormulaTable::m_afRate[800][1], CFormulaTable::m_afRate[800][2]);
	}

	m_pAttribute.wBaseBlockRate = m_pAttribute.wLastBlockRate = WORD(CFormulaTable::m_afRate[919][1] + m_pAttribute.wBaseDex * CFormulaTable::m_afRate[919][2] + m_pAttribute.wBaseCon * CFormulaTable::m_afRate[919][2]);
	m_pAttribute.wGuardRateBase = m_pAttribute.wGuardRateLast = WORD(CFormulaTable::m_afRate[1000][1] + m_pAttribute.wBaseDex * CFormulaTable::m_afRate[1000][2]);

	m_pAttribute.wBaseCurseSuccessRate = m_pAttribute.wLastCurseSuccessRate = WORD(m_pAttribute.wBaseFoc / CFormulaTable::m_afRate[700][1] * CFormulaTable::m_afRate[700][2]);
	m_pAttribute.wBaseCurseToleranceRate = m_pAttribute.wLastCurseToleranceRate = WORD(m_pAttribute.wBaseDex / CFormulaTable::m_afRate[1200][1] * CFormulaTable::m_afRate[1200][2]);

	m_pAttribute.wBaseLpRegen = m_pAttribute.wLastLpRegen = pBotTbldat->wLP_Regeneration;
	m_pAttribute.wBaseLpSitdownRegen = m_pAttribute.wLastLpSitdownRegen = WORD(m_pAttribute.dwBaseMaxLP) * 20 / 100;
	m_pAttribute.wBaseLpBattleRegen = m_pAttribute.wLastLpBattleRegen = m_pAttribute.wBaseLpRegen;

	m_pOwnerRef->GetBuffManager()->CopyBuffAttributesTo(this);

	if (m_pAttribute.wBaseHitRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "1Attack Rate is invalid. Foc: %u, formula 1: %f, formula 2: %f", m_pAttribute.wBaseFoc, CFormulaTable::m_afRate[600][1], CFormulaTable::m_afRate[600][2]);
	}
	if (m_pAttribute.wBaseDodgeRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "1Dodge Rate is invalid. Dex: %u, formula 1: %f, formula 2: %f", m_pAttribute.wBaseDex, CFormulaTable::m_afRate[800][1], CFormulaTable::m_afRate[800][2]);
	}

	if (m_pAttribute.dwBaseMaxLP == 0)
	{
		ERR_LOG(LOG_BOTAI, "1 MAXLP is 0. dwBasic_LP: %u, byEfLv: %u, byLv: %u", charTbldat->dwBasic_LP, byEfLv, byLv);
	}

	CalculatePercentValues();

	if (m_pAttribute.dwBaseMaxLP == 0)
	{
		ERR_LOG(LOG_BOTAI, "2 MAXLP is 0. dwBasic_LP: %u, byEfLv: %u, byLv: %u", charTbldat->dwBasic_LP, byEfLv, byLv);
	}

	if (m_pAttribute.wBaseHitRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "2Attack Rate is invalid. Foc: %u, formula 1: %f, formula 2: %f, Percentfoc: %f, PercentFoc Negative: %f", m_pAttribute.wBaseFoc, CFormulaTable::m_afRate[600][1], CFormulaTable::m_afRate[600][2], m_percentValue.m_fAttackRate, m_percentValue.m_fAttackRateNegative);
	}
	if (m_pAttribute.wBaseDodgeRate == INVALID_WORD)
	{
		ERR_LOG(LOG_BOTAI, "2Dodge Rate is invalid. Dex: %u, formula 1: %f, formula 2: %f, Percentfoc: %f, PercentFoc Negative: %f", m_pAttribute.wBaseDex, CFormulaTable::m_afRate[800][1], CFormulaTable::m_afRate[800][2], m_percentValue.m_fDodgeRate, m_percentValue.m_fDodgeRateNegative);
	}

	CCharacterAtt::CalculateAtt();
}

float CCharacterAttBot::GetWalkSpeed()
{
	return m_pBotRef->GetTbldat()->fWalk_Speed;
}
