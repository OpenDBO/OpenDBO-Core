#ifndef __INC_DBOG_CHARATTRIBUTE_BASE_H__
#define __INC_DBOG_CHARATTRIBUTE_BASE_H__


class CCharacter;
#include "NtlAvatar.h"

class CCharacterAtt
{

public:
	CCharacterAtt();
	virtual	~CCharacterAtt();

public:

	sAVATAR_ATTRIBUTE* GetAvatarAttributePointer() { return &m_pAttribute; }
	sAVATAR_ATTRIBUTE& GetAvatarAttribute() { return m_pAttribute; }

public:

	bool					Create(CCharacter* pChar);

	void					Init();

	//CALCULATION

	virtual void	CalculateAll();
	virtual void	CalculateAtt();

	void			CalculatePercentValues();

public:


	virtual void	CalculateStr(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCon(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFoc(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDex(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateSol(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEng(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateMaxLP(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateMaxEP(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateMaxRP(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateMaxAP(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateLpRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateLpSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateLpBattleRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEpRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEpSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEpBattleRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateApRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateApSitdownRegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateApDegen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateRpRegen(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculatePhysicalOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyOffence(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculatePhysicalDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyDefence(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateAttackRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDodgeRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBlockRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBlockDamageRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCurseSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCurseToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePhysicalCriticalRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyCriticalRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePhysicalCriticalDamageRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyCriticalDamageRate(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateRunSpeed(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFlySpeed(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFlyDashSpeed(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFlyAccelSpeed(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateAttackSpeedRate(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateAttackRange(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateCastingTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCoolTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateKeepTimeChangePercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDotTimeChangeAbsolute(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateRequiredEpChangePercent(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateBattleAttribute(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateHonestOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateHonestDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateStrangeOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateStrangeDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateWildOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateWildDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEleganceOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEleganceDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFunnyOffence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateFunnyDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePhysicalReflection(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyReflection(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateParalyzeToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateTerrorToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateConfuseToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateStoneToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCandyToleranceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateParalyzeKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateTerrorKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateConfuseKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateStoneKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCandyKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBleedingKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePoisonKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateStomachacheKeepTimeDown(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCriticalBlockSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateLpRecoveryWhenHit(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateLpRecoveryWhenHitInPercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEpRecoveryWhenHit(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEpRecoveryWhenHitInPercent(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateStomachacheDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePoisonDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBleedDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBurnDefence(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateMindCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateBodyCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateChangeCurseImmunity(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateSkillAnimationSpeedModifier(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateWeightLimit(float fValue, BYTE byApplyType, bool bIsPlus) {}
	virtual void	CalculateSkillAggroBonus(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateSkillAggroBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDirectHealPowerBonus(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDirectHealPowerBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateHotPowerBonus(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateHotPowerBonusInPercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateDotValueChangePercent(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculatePhysicalCriticalDefenceRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyCriticalDefenceRate(float fValue, BYTE byApplyType, bool bIsPlus);

	virtual void	CalculateGuardRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateSkillDamageBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateCurseBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateKnockdownBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateHtbBlockModeSuccessRate(float fValue, BYTE byApplyType, bool bIsPlus);
	//virtual void	CalculateItemUpgradeBonusRate() {}
	//virtual void	CalculateItemUpgradeBreakBonusRate() {}
	virtual void	CalculateExpBooster(float fValue, BYTE byApplyType, bool bIsPlus);
	//virtual void	CalculateQuestDropRate() {}

	virtual void	CalculatePhysicalArmorPen(float fValue, BYTE byApplyType, bool bIsPlus);
	virtual void	CalculateEnergyArmorPen(float fValue, BYTE byApplyType, bool bIsPlus);

	//GETTER

	WORD	GetStr() { return m_pAttribute.wLastStr; }
	WORD	GetCon() { return m_pAttribute.wLastCon; }
	WORD	GetFoc() { return m_pAttribute.wLastFoc; }
	WORD	GetDex() { return m_pAttribute.wLastDex; }
	WORD	GetSol() { return m_pAttribute.wLastSol; }
	WORD	GetEng() { return m_pAttribute.wLastEng; }

	DWORD	GetMaxLP() { return m_pAttribute.dwLastMaxLP; }
	DWORD	GetMaxEP() { return m_pAttribute.wLastMaxEP; }
	DWORD	GetMaxAP() { return m_pAttribute.dwLastMaxAp; }
	WORD	GetMaxRP() { return m_pAttribute.wLastMaxRP; }

	WORD	GetLPRegen() { return m_pAttribute.wLastLpRegen; }
	WORD	GetLPSitdownRegen() { return m_pAttribute.wLastLpSitdownRegen; }
	WORD	GetLPBattleRegen() { return m_pAttribute.wLastLpBattleRegen; }
	WORD	GetEPRegen() { return m_pAttribute.wLastEpRegen; }
	WORD	GetEPSitdownRegen() { return m_pAttribute.wLastEpSitdownRegen; }
	WORD	GetEPBattleRegen() { return m_pAttribute.wLastEpBattleRegen; }
	WORD	GetAPRegen() { return m_pAttribute.wLastApRegen; }
	WORD	GetAPSitdownRegen() { return m_pAttribute.wLastApSitdownRegen; }
	WORD	GetAPDegen() { return m_pAttribute.wAirAP_Degen; }
	WORD	GetRPRegen() { return m_pAttribute.wLastRpRegen; }
	WORD	GetRPDimimutionRate() { return m_pAttribute.wLastRpDiminutionRate; }
	WORD	GetPhysicalOffence() { return m_pAttribute.wLastPhysicalOffense; }
	WORD	GetPhysicalDefence() { return m_pAttribute.wLastPhysicalDefense; }
	void	SetPhysicalDefence(WORD wDef) { m_pAttribute.wLastPhysicalDefense = wDef; }
	WORD	GetEnergyOffence() { return m_pAttribute.wLastEnergyOffense; }
	WORD	GetEnergyDefence() { return m_pAttribute.wLastEnergyDefense; }
	WORD	GetAttackRate() { return m_pAttribute.wLastHitRate; }
	WORD	GetDodgeRate() { return m_pAttribute.wLastDodgeRate; }
	WORD	GetBlockRate() { return m_pAttribute.wLastBlockRate; }
	WORD	GetBlockDamageRate() { return m_pAttribute.wLastBlockDMG; }
	WORD	GetCurseSuccessRate() { return m_pAttribute.wLastCurseSuccessRate; }
	WORD	GetCurseToleranceRate() { return m_pAttribute.wLastCurseToleranceRate; }
	WORD	GetPhysicalCriticalRate() { return m_pAttribute.wLastPhysicalCriticalRate; }
	WORD	GetEnergyCriticalRate() { return m_pAttribute.wLastEnergyCriticalRate; }
	float	GetPhysicalCriticalDamageRate() { return m_pAttribute.fLastPhysicalCritDmgRate; }
	float	GetEnergyCriticalDamageRate() { return m_pAttribute.fLastEnergyCritDmgRate; }

	virtual float	GetWalkSpeed() = 0;
	float	GetRunSpeed() { return m_pAttribute.fLastRunSpeed; }
	float	GetFlySpeed() { return m_pAttribute.fLastAirSpeed; }
	float	GetFlyDashSpeed() { return m_pAttribute.fLastAirDashSpeed; }
	float	GetFlyAccelSpeed() { return m_pAttribute.fLastAirDashAccelSpeed; }

	WORD	GetAttackSpeedRate() { return m_pAttribute.wLastAttackSpeedRate; }

	float	GetAttackRange() { return m_pAttribute.fLastAttackRange; }

	float	GetCastingTimeChangePercent() { return m_pAttribute.fCastingTimeChangePercent; }
	float	GetCoolTimeChangePercent() { return m_pAttribute.fCoolTimeChangePercent; }
	float	GetKeepTimeChangePercent() { return m_pAttribute.fKeepTimeChangePercent; }
	float	GetDotTimeChangeAbsolute() { return m_pAttribute.fDotKeepTimeChangeInSeconds; } //xanu ??
	float	GetRequiredEpChangePercent() { return m_pAttribute.fRequiredEpChangePercent; }

	float	GetPhysicalReflection() { return m_pAttribute.fPhysicalReflect; }
	float	GetEnergyReflection() { return m_pAttribute.fEnergyReflect; }

	WORD	GetParalyzeToleranceRate() { return m_pAttribute.wParalyzeToleranceRate; }
	WORD	GetTerrorToleranceRate() { return m_pAttribute.wTerrorToleranceRate; }
	WORD	GetConfuseToleranceRate() { return m_pAttribute.wConfuseToleranceRate; }
	WORD	GetStoneToleranceRate() { return m_pAttribute.wStoneToleranceRate; }
	WORD	GetCandyToleranceRate() { return m_pAttribute.wCandyToleranceRate; }

	float	GetParalyzeKeepTimeDown() { return m_pAttribute.fParalyzeKeepTimeDown; }
	float	GetTerrorKeepTimeDown() { return m_pAttribute.fTerrorKeepTimeDown; }
	float	GetConfuseKeepTimeDown() { return m_pAttribute.fConfuseKeepTimeDown; }
	float	GetStoneKeepTimeDown() { return m_pAttribute.fStoneKeepTimeDown; }
	float	GetCandyKeepTimeDown() { return m_pAttribute.fCandyKeepTimeDown; }
	float	GetBleedingKeepTimeDown() { return m_pAttribute.fBleedingKeepTimeDown; }
	float	GetPoisonKeepTimeDown() { return m_pAttribute.fPoisonKeepTimeDown; }
	float	GetStomachacheKeepTimeDown() { return m_pAttribute.fStomachacheKeepTimeDown; }

	float	GetCriticalBlockSuccessRate() { return m_pAttribute.fCriticalBlockSuccessRate; }
	DWORD	GetLpRecoveryWhenHit() { return m_pAttribute.dwLpRecoveryWhenHit; }
	float	GetLpRecoveryWhenHitInPercent() { return m_pAttribute.fLpRecoveryWhenHitInPercent; }
	DWORD	GetEpRecoveryWhenHit() { return m_pAttribute.dwEpRecoveryWhenHit; }
	float	GetEpRecoveryWhenHitInPercent() { return m_pAttribute.fEpRecoveryWhenHitInPercent; }

	WORD	GetStomachacheDefence() { return m_pAttribute.wLastAbdominalPainDefense; }
	WORD	GetPoisonDefence() { return m_pAttribute.wLastPoisonDefense; }
	WORD	GetBleedDefence() { return m_pAttribute.wLastBleedingDefense; }
	WORD	GetBurnDefence() { return m_pAttribute.wLastBurnDefense; }
	float	GetMindCurseImmunity() { return m_pAttribute.fLastMindCurseImmunity; }
	float	GetBodyCurseImmunity() { return m_pAttribute.fLastBodyCurseImmunity; }
	float	GetChangeCurseImmunity() { return m_pAttribute.fLastChangeCurseImmunity; }
	float	GetSkillAnimationSpeedModifier() { return m_pAttribute.fLastSkillAnimationSpeed; }
	DWORD	GetWeightLimit() { return m_pAttribute.dwLastWeightMax; }
	float	GetSkillAggroBonus() { return m_pAttribute.fSkillAggroBonus; }
	float	GetSkillAggroBonusInPercent() { return m_pAttribute.fSkillAggroBonusInPercent; }
	float	GetDirectHealPowerBonus() { return m_pAttribute.fDirectHealPowerBonus; }
	float	GetDirectHealPowerBonusInPercent() { return m_pAttribute.fDirectHealPowerBonusInPercent; }
	float	GetHotPowerBonus() { return m_pAttribute.fHotPowerBonus; }
	float	GetHotPowerBonusInPercent() { return m_pAttribute.fHotPowerBonusInPercent; }
	float	GetDotValueChangePercent() { return m_pAttribute.fDotValueChangePercent; }
	BYTE	GetBattleAttributeOffence() { return m_byBattle_Attribute_Ofence; }
	BYTE	GetBattleAttributeDefence() { return m_byBattle_Attribute_Defence; }
	float	GetPhysicalCriticalDefenceRate() { return m_pAttribute.fPhysicalCriticalDefenseRate; }
	float	GetEnergyCriticalDefenceRate() { return m_pAttribute.fEnergyCriticalDefenseRate; }

	float	GetSkillDamageBlockModeSuccessRate() { return m_pAttribute.fSkillDamageBlockModeSuccessRate; }
	float	GetCurseBlockModeSuccessRate() { return m_pAttribute.fCurseBlockModeSuccessRate; }
	float	GetKnockDownBlockModeSuccessRate() { return m_pAttribute.fKnockdownBlockModeSuccessRate; }
	float	GetHtbKnockDownBlockModeSuccessRate() { return m_pAttribute.fHtbBlockModeSuccessRate; }

	WORD	GetGuardRate() { return m_pAttribute.wGuardRateLast; }

	BYTE	GetExpBoost() { return m_pAttribute.byExpBooster; }

	//float	GetPhysicalArmorPenRate() { return m_pAttribute.fLastPhysicalCritDmgRate; }//Xanu
	//float	GetEnergyArmorPenRate() { return m_pAttribute.fLastEnergyCritDmgRate; }//Xanu

//SETTER
	void	SetMaxLP(int nMaxLP) { m_pAttribute.dwLastMaxLP = nMaxLP; }
	void	SetMaxEP(WORD wMaxEP) { m_pAttribute.wLastMaxEP = wMaxEP; }

	void	SetRunSpeed(float fSpeed) { m_pAttribute.fLastRunSpeed = fSpeed; }
	void	SetFlySpeed(float fSpeed) { m_pAttribute.fLastAirSpeed = fSpeed; }
	void	SetFlyDashSpeed(float fSpeed) { m_pAttribute.fLastAirDashSpeed = fSpeed; }
	void	SetFlyAccelSpeed(float fSpeed) { m_pAttribute.fLastAirDashAccelSpeed = fSpeed; }

	void	SetAttackSpeedRate(WORD wSpeed) { m_pAttribute.wLastAttackSpeedRate = wSpeed; }

	void	SetAttackRange(float fRange) { m_pAttribute.fLastAttackRange = fRange; }

	void	SetSkillAnimationSpeedModifier(float fSpeed) { m_pAttribute.fLastSkillAnimationSpeed = fSpeed; }

	void	SetBattleAttributeOffence(BYTE byAttribute) { m_byBattle_Attribute_Ofence = byAttribute; }
	void	SetBattleAttributeDefence(BYTE byAttribute) { m_byBattle_Attribute_Defence = byAttribute; }
	void	SetPhysicalOffence(WORD wAttack) { m_pAttribute.wLastPhysicalOffense = wAttack; }
	void	SetEnergyOffence(WORD wAttack) { m_pAttribute.wLastEnergyOffense = wAttack; }

	void	SetSubOffence(WORD wOffence) { m_wSubWeaponPhysicalOffence = wOffence; m_wSubWeaponEnergyOffence = wOffence; }
	WORD	GetSubWeaponPhysicalOffence() { return m_wSubWeaponPhysicalOffence; }
	WORD	GetSubWeaponEnergyOffence() { return m_wSubWeaponEnergyOffence; }

protected:

	CCharacter* m_pOwnerRef;
	sAVATAR_ATTRIBUTE		m_pAttribute;
	BYTE					m_byBattle_Attribute_Ofence;		//eBATTLE_ATTRIBUTE //[0] Offence (main)
	BYTE					m_byBattle_Attribute_Defence;		//eBATTLE_ATTRIBUTE //[0] Offence (main)
	float					m_fRunSpeedBackup;
	WORD					m_wAttackSpeedBackup;

	WORD					m_wSubWeaponPhysicalOffence;
	WORD					m_wSubWeaponEnergyOffence;

	float					m_fCurLpPercent;
	float					m_fCurEpPercent;
	float					m_fCurApPercent;

	// percent values
	struct sPERCENT_VALUES
	{
		float					m_fSTR;
		float					m_fCON;
		float					m_fFOC;
		float					m_fDEX;
		float					m_fSOL;
		float					m_fENG;
		float					m_fLP;
		float					m_fEP;
		float					m_fAP;
		float					m_fRP;
		float					m_fLpRegen;
		float					m_fLpSitdownRegen;
		float					m_fLpBattleRegen;
		float					m_fEpRegen;
		float					m_fEpSitdownRegen;
		float					m_fEpBattleRegen;
		float					m_fApRegen;
		float					m_fApSitdownRegen;
		float					m_fApDegen;
		float					m_fRpRegen;
		float					m_fRpDimimutionRate;
		float					m_fPhysicalOffence;
		float					m_fPhysicalDefence;
		float					m_fEnergyOffence;
		float					m_fEnergyDefence;
		float					m_fAttackRate;
		float					m_fDodgeRate;
		float					m_fBlockRate;
		float					m_fBlockDamageRate;
		float					m_fCurseSuccessRate;
		float					m_fCurseToleranceRate;
		float					m_fPhysicalCriticalRate;
		float					m_fEnergyCriticalRate;
		float					m_fPhysicalCriticalDamageRate;
		float					m_fEnergyCriticalDamageRate;
		float					m_fRunSpeed;
		float					m_fFlySpeed;
		float					m_fFlyDashSpeed;
		float					m_fFlyAccelSpeed;
		float					m_fAttackSpeedRate;
		float					m_fAttackRange;
		float					m_fStomachacheDef;
		float					m_fPoisonDef;
		float					m_fBleedDef;
		float					m_fBurnDef;
		float					m_fMindCurseImmunity;
		float					m_fBodyCurseImmunity;
		float					m_fChangeCurseImmunity;
		float					m_fSkillAnimationSpeedModifier;
		float					m_fGuardRate;

		float					m_fSTRNegative;
		float					m_fCONNegative;
		float					m_fFOCNegative;
		float					m_fDEXNegative;
		float					m_fSOLNegative;
		float					m_fENGNegative;
		float					m_fLPNegative;
		float					m_fEPNegative;
		float					m_fAPNegative;
		float					m_fRPNegative;
		float					m_fLpRegenNegative;
		float					m_fLpSitdownRegenNegative;
		float					m_fLpBattleRegenNegative;
		float					m_fEpRegenNegative;
		float					m_fEpSitdownRegenNegative;
		float					m_fEpBattleRegenNegative;
		float					m_fApRegenNegative;
		float					m_fApSitdownRegenNegative;
		float					m_fApBattleRegenNegative;
		float					m_fApDegenNegative;
		float					m_fApBattleDegenNegative;
		float					m_fRpRegenNegative;
		float					m_fRpDimimutionRateNegative;
		float					m_fPhysicalOffenceNegative;
		float					m_fPhysicalDefenceNegative;
		float					m_fEnergyOffenceNegative;
		float					m_fEnergyDefenceNegative;
		float					m_fAttackRateNegative;
		float					m_fDodgeRateNegative;
		float					m_fBlockRateNegative;
		float					m_fBlockDamageRateNegative;
		float					m_fCurseSuccessRateNegative;
		float					m_fCurseToleranceRateNegative;
		float					m_fPhysicalCriticalRateNegative;
		float					m_fEnergyCriticalRateNegative;
		float					m_fPhysicalCriticalDamageRateNegative;
		float					m_fEnergyCriticalDamageRateNegative;
		float					m_fRunSpeedNegative;
		float					m_fFlySpeedNegative;
		float					m_fFlyDashSpeedNegative;
		float					m_fFlyAccelSpeedNegative;
		float					m_fAttackSpeedRateNegative;
		float					m_fAttackRangeNegative;
		float					m_fStomachacheDefNegative;
		float					m_fPoisonDefNegative;
		float					m_fBleedDefNegative;
		float					m_fBurnDefNegative;
		float					m_fMindCurseImmunityNegative;
		float					m_fBodyCurseImmunityNegative;
		float					m_fChangeCurseImmunityNegative;
		float					m_fSkillAnimationSpeedModifierNegative;
		float					m_fGuardRateNegative;
	}
	m_percentValue;
};

#endif