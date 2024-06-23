#pragma once

enum eATTRIBUTE_TO_UPDATE
{
	AVATAR_ATTRIBUTE_wBaseStr,
	AVATAR_ATTRIBUTE_wLastStr,
	AVATAR_ATTRIBUTE_wBaseCon,
	AVATAR_ATTRIBUTE_wLastCon,
	AVATAR_ATTRIBUTE_wBaseFoc,
	AVATAR_ATTRIBUTE_wLastFoc,
	AVATAR_ATTRIBUTE_wBaseDex,
	AVATAR_ATTRIBUTE_wLastDex,
	AVATAR_ATTRIBUTE_wBaseSol,
	AVATAR_ATTRIBUTE_wLastSol,
	AVATAR_ATTRIBUTE_wBaseEng,
	AVATAR_ATTRIBUTE_wLastEng,

	AVATAR_ATTRIBUTE_dwBaseMaxLP,
	AVATAR_ATTRIBUTE_dwLastMaxLP,
	AVATAR_ATTRIBUTE_wBaseMaxEP,
	AVATAR_ATTRIBUTE_wLastMaxEP,
	AVATAR_ATTRIBUTE_dwBaseMaxAp,
	AVATAR_ATTRIBUTE_dwLastMaxAp,
	AVATAR_ATTRIBUTE_wBaseMaxRP,
	AVATAR_ATTRIBUTE_wLastMaxRP,

	AVATAR_ATTRIBUTE_wBaseLpRegen,
	AVATAR_ATTRIBUTE_wLastLpRegen,
	AVATAR_ATTRIBUTE_wBaseLpSitdownRegen,
	AVATAR_ATTRIBUTE_wLastLpSitdownRegen,
	AVATAR_ATTRIBUTE_wBaseLpBattleRegen,
	AVATAR_ATTRIBUTE_wLastLpBattleRegen,

	AVATAR_ATTRIBUTE_wBaseEpRegen,
	AVATAR_ATTRIBUTE_wLastEpRegen,
	AVATAR_ATTRIBUTE_wBaseEpSitdownRegen,
	AVATAR_ATTRIBUTE_wLastEpSitdownRegen,
	AVATAR_ATTRIBUTE_wBaseEpBattleRegen,
	AVATAR_ATTRIBUTE_wLastEpBattleRegen,

	AVATAR_ATTRIBUTE_wBaseApRegen,
	AVATAR_ATTRIBUTE_wLastApRegen,
	AVATAR_ATTRIBUTE_wBaseApSitdownRegen,
	AVATAR_ATTRIBUTE_wLastApSitdownRegen,
	AVATAR_ATTRIBUTE_wBaseApBattleRegen,
	AVATAR_ATTRIBUTE_wLastApBattleRegen,

	AVATAR_ATTRIBUTE_wAirAPDegen,
	AVATAR_ATTRIBUTE_wAirAPFlyDegen, // This one shows as AP degen in devmaster
	AVATAR_ATTRIBUTE_wAirAPFLyDashDegen,
	AVATAR_ATTRIBUTE_wAirAPFlyDashBoostDegen,

	AVATAR_ATTRIBUTE_wBaseRpRegen,
	AVATAR_ATTRIBUTE_wLastRpRegen,
	AVATAR_ATTRIBUTE_wLastRpDiminutionRate,

	AVATAR_ATTRIBUTE_wBasePhysicalOffense,
	AVATAR_ATTRIBUTE_wLastPhysicalOffense,
	AVATAR_ATTRIBUTE_wBasePhysicalDefense,
	AVATAR_ATTRIBUTE_wLastPhysicalDefense,

	AVATAR_ATTRIBUTE_wBaseEnergyOffense,
	AVATAR_ATTRIBUTE_wLastEnergyOffense,
	AVATAR_ATTRIBUTE_wBaseEnergyDefense,
	AVATAR_ATTRIBUTE_wLastEnergyDefense,

	AVATAR_ATTRIBUTE_wBaseHitRate,
	AVATAR_ATTRIBUTE_wLastHitRate,
	AVATAR_ATTRIBUTE_wBaseDodgeRate,
	AVATAR_ATTRIBUTE_wLastDodgeRate,
	AVATAR_ATTRIBUTE_wBaseBlockRate,
	AVATAR_ATTRIBUTE_wLastBlockRate,

	AVATAR_ATTRIBUTE_wBaseBlockDMG,
	AVATAR_ATTRIBUTE_wLastBlockDMG,

	AVATAR_ATTRIBUTE_wBaseCurseSuccessRate,
	AVATAR_ATTRIBUTE_wLastCurseSuccessRate,
	AVATAR_ATTRIBUTE_wBaseCurseToleranceRate,
	AVATAR_ATTRIBUTE_wLastCurseToleranceRate,

	AVATAR_ATTRIBUTE_wBasePhysicalCriticalRate,
	AVATAR_ATTRIBUTE_wLastPhysicalCriticalRate,
	AVATAR_ATTRIBUTE_wBaseEnergyCriticalRate,
	AVATAR_ATTRIBUTE_wLastEnergyCriticalRate,

	AVATAR_ATTRIBUTE_fBasePhysicalCritDmgRate,
	AVATAR_ATTRIBUTE_fLastPhysicalCritDmgRate,
	AVATAR_ATTRIBUTE_fBaseEnergyCritDmgRate,
	AVATAR_ATTRIBUTE_fLastEnergyCritDmgRate,

	AVATAR_ATTRIBUTE_fBaseRunSpeed,
	AVATAR_ATTRIBUTE_fLastRunSpeed,
	AVATAR_ATTRIBUTE_fBaseAirSpeed,
	AVATAR_ATTRIBUTE_fLastAirSpeed,
	AVATAR_ATTRIBUTE_fBaseAirDashSpeed,
	AVATAR_ATTRIBUTE_fLastAirDashSpeed,
	AVATAR_ATTRIBUTE_fBaseAirDashAccelSpeed,
	AVATAR_ATTRIBUTE_fLastAirDashAccelSpeed,

	AVATAR_ATTRIBUTE_wBaseAttackSpeedRate,
	AVATAR_ATTRIBUTE_wLastAttackSpeedRate,
	AVATAR_ATTRIBUTE_fBaseAttackRange,
	AVATAR_ATTRIBUTE_fLastAttackRange,

	AVATAR_ATTRIBUTE_fCastingTimeChangePercent,
	AVATAR_ATTRIBUTE_fCoolTimeChangePercent,
	AVATAR_ATTRIBUTE_fKeepTimeChangePercent,
	AVATAR_ATTRIBUTE_fDotKeepTimeChangeInSeconds,
	AVATAR_ATTRIBUTE_fRequiredEpChangePercent,

	AVATAR_ATTRIBUTE_fHonestOffense,
	AVATAR_ATTRIBUTE_fHonestDefense,
	AVATAR_ATTRIBUTE_fStrangeOffense,
	AVATAR_ATTRIBUTE_fStrangeDefense,
	AVATAR_ATTRIBUTE_fWildOffense,
	AVATAR_ATTRIBUTE_fWildDefense,
	AVATAR_ATTRIBUTE_fEleganceOffense,
	AVATAR_ATTRIBUTE_fEleganceDefense,
	AVATAR_ATTRIBUTE_fFunnyOffense,
	AVATAR_ATTRIBUTE_fFunnyDefense,

	AVATAR_ATTRIBUTE_fPhysicalReflect,
	AVATAR_ATTRIBUTE_fEnergyReflect,

	AVATAR_ATTRIBUTE_wParalyzeToleranceRate,
	AVATAR_ATTRIBUTE_wTerrorToleranceRate,
	AVATAR_ATTRIBUTE_wConfuseToleranceRate,
	AVATAR_ATTRIBUTE_wStoneToleranceRate,
	AVATAR_ATTRIBUTE_wCandyToleranceRate,

	AVATAR_ATTRIBUTE_fParalyzeKeepTimeDown,
	AVATAR_ATTRIBUTE_fTerrorKeepTimeDown,
	AVATAR_ATTRIBUTE_fConfuseKeepTimeDown,
	AVATAR_ATTRIBUTE_fStoneKeepTimeDown,
	AVATAR_ATTRIBUTE_fCandyKeepTimeDown,
	AVATAR_ATTRIBUTE_fBleedingKeepTimeDown,
	AVATAR_ATTRIBUTE_fPoisonKeepTimeDown,
	AVATAR_ATTRIBUTE_fStomachacheKeepTimeDown,

	AVATAR_ATTRIBUTE_fCriticalBlockSuccessRate,

	AVATAR_ATTRIBUTE_dwLpRecoveryWhenHit,
	AVATAR_ATTRIBUTE_fLpRecoveryWhenHitInPercent,
	AVATAR_ATTRIBUTE_dwEpRecoveryWhenHit,
	AVATAR_ATTRIBUTE_fEpRecoveryWhenHitInPercent,

	AVATAR_ATTRIBUTE_wBaseAbdominalPainDefense,
	AVATAR_ATTRIBUTE_wLastAbdominalPainDefense,
	AVATAR_ATTRIBUTE_wBasePoisonDefense,
	AVATAR_ATTRIBUTE_wLastPoisonDefense,
	AVATAR_ATTRIBUTE_wBaseBleedingDefense,
	AVATAR_ATTRIBUTE_wLastBleedingDefense,
	AVATAR_ATTRIBUTE_wBaseBurnDefense,
	AVATAR_ATTRIBUTE_wLastBurnDefense,

	AVATAR_ATTRIBUTE_fBaseMindCurseImmunity,
	AVATAR_ATTRIBUTE_fLastMindCurseImmunity,
	AVATAR_ATTRIBUTE_fBaseBodyCurseImmunity,
	AVATAR_ATTRIBUTE_fLastBodyCurseImmunity,
	AVATAR_ATTRIBUTE_fBaseChangeCurseImmunity,
	AVATAR_ATTRIBUTE_fLastChangeCurseImmunity,
	AVATAR_ATTRIBUTE_fBaseSkillAnimationSpeed,
	AVATAR_ATTRIBUTE_fLastSkillAnimationSpeed,
	AVATAR_ATTRIBUTE_dwBaseWeightMax,
	AVATAR_ATTRIBUTE_dwLastWeightMax,

	AVATAR_ATTRIBUTE_fSkillAggroBonus,
	AVATAR_ATTRIBUTE_fSkillAggroBonusInPercent,
	AVATAR_ATTRIBUTE_fDirectHealPowerBonus,
	AVATAR_ATTRIBUTE_fDirectHealPowerBonusInPercent,
	AVATAR_ATTRIBUTE_fHotPowerBonus,
	AVATAR_ATTRIBUTE_fHotPowerBonusInPercent,
	AVATAR_ATTRIBUTE_fDotValueChangePercent,

	AVATAR_ATTRIBUTE_fPhysicalCriticalDefenseRate,
	AVATAR_ATTRIBUTE_fEnergyCriticalDefenseRate,

	AVATAR_ATTRIBUTE_wGuardRateBase,
	AVATAR_ATTRIBUTE_wGuardRateLast,

	AVATAR_ATTRIBUTE_fSkillDamageBlockModeSuccessRate,
	AVATAR_ATTRIBUTE_fCurseBlockModeSuccessRate,
	AVATAR_ATTRIBUTE_fKnockdownBlockModeSuccessRate,
	AVATAR_ATTRIBUTE_fHtbBlockModeSuccessRate,
	AVATAR_ATTRIBUTE_fItemUpgradeBonusRate,
	AVATAR_ATTRIBUTE_fItemUpgradeBreakBonusRate,


	AVATAR_ATTRIBUTE_byExpBooster,
	AVATAR_ATTRIBUTE_byQuestDropRate,

	//AVATAR_ATTRIBUTE_byunkTest,

	AVATAR_ATTRIBUTE_COUNT,
	ATTRIBUTE_TO_UPDATE_UNKNOWN = 0xFF,

	ATTRIBUTE_TO_UPDATE_FIRST = AVATAR_ATTRIBUTE_wBaseStr,
	ATTRIBUTE_TO_UPDATE_LAST = AVATAR_ATTRIBUTE_COUNT - 1,
};

#pragma pack(push, 1)

struct sAVATAR_ATTRIBUTE
{
	WORD wBaseStr;
	WORD wLastStr;
	WORD wBaseCon;
	WORD wLastCon;
	WORD wBaseFoc;
	WORD wLastFoc;
	WORD wBaseDex;
	WORD wLastDex;
	WORD wBaseSol;
	WORD wLastSol;
	WORD wBaseEng;
	WORD wLastEng;

	DWORD dwBaseMaxLP;
	DWORD dwLastMaxLP;
	WORD wBaseMaxEP;
	WORD wLastMaxEP;
	DWORD dwBaseMaxAp;
	DWORD dwLastMaxAp;
	WORD wBaseMaxRP;
	WORD wLastMaxRP;

	WORD wBaseLpRegen;
	WORD wLastLpRegen;
	WORD wBaseLpSitdownRegen;
	WORD wLastLpSitdownRegen;
	WORD wBaseLpBattleRegen;
	WORD wLastLpBattleRegen;

	WORD wBaseEpRegen;
	WORD wLastEpRegen;
	WORD wBaseEpSitdownRegen;
	WORD wLastEpSitdownRegen;
	WORD wBaseEpBattleRegen;
	WORD wLastEpBattleRegen;

	WORD wBaseApRegen;
	WORD wLastApRegen;
	WORD wBaseApSitdownRegen;
	WORD wLastApSitdownRegen;
	WORD wBaseApBattleRegen;
	WORD wLastApBattleRegen;

	WORD wAirAP_Degen;
	WORD wAirAP_DegenFly; // This one shows as AP degen in devmaster
	WORD wAirAP_DegenDash;
	WORD wAirAP_DegenAccel;

	WORD wBaseRpRegen;
	WORD wLastRpRegen; // "/second"
	WORD wLastRpDiminutionRate; // "/second"

	WORD wBasePhysicalOffense;
	WORD wLastPhysicalOffense;
	WORD wBasePhysicalDefense;
	WORD wLastPhysicalDefense;

	WORD wBaseEnergyOffense;
	WORD wLastEnergyOffense;
	WORD wBaseEnergyDefense;
	WORD wLastEnergyDefense;

	WORD wBaseHitRate;
	WORD wLastHitRate;
	WORD wBaseDodgeRate;
	WORD wLastDodgeRate;
	WORD wBaseBlockRate;
	WORD wLastBlockRate;
	WORD wBaseBlockDMG;
	WORD wLastBlockDMG;

	WORD wBaseCurseSuccessRate;
	WORD wLastCurseSuccessRate;
	WORD wBaseCurseToleranceRate;
	WORD wLastCurseToleranceRate;

	WORD wBasePhysicalCriticalRate;
	WORD wLastPhysicalCriticalRate;
	WORD wBaseEnergyCriticalRate;
	WORD wLastEnergyCriticalRate;

	FLOAT fBasePhysicalCritDmgRate;
	FLOAT fLastPhysicalCritDmgRate;
	FLOAT fBaseEnergyCritDmgRate;
	FLOAT fLastEnergyCritDmgRate;

	FLOAT fBaseRunSpeed;
	FLOAT fLastRunSpeed;
	FLOAT fBaseAirSpeed;
	FLOAT fLastAirSpeed;
	FLOAT fBaseAirDashSpeed;
	FLOAT fLastAirDashSpeed;
	FLOAT fBaseAirDashAccelSpeed;
	FLOAT fLastAirDashAccelSpeed;

	WORD wBaseAttackSpeedRate;
	WORD wLastAttackSpeedRate;
	FLOAT fBaseAttackRange;
	FLOAT fLastAttackRange;

	FLOAT fCastingTimeChangePercent; // 0 = no change, 10 = cast time longer by 10% (110% total)
	FLOAT fCoolTimeChangePercent; // 0 = no change, 10 = cooldown longer by 10% (110% total), to make cooldow shorter need negative
	FLOAT fKeepTimeChangePercent; // 0 = no change, 10 = duration longer by 10% (110% total)
	FLOAT fDotKeepTimeChangeInSeconds;
	FLOAT fRequiredEpChangePercent; // 0 = No change, 10 = cost EP +10% (110% total)

	FLOAT fHonestOffense;
	FLOAT fHonestDefense;
	FLOAT fStrangeOffense;
	FLOAT fStrangeDefense;
	FLOAT fWildOffense;
	FLOAT fWildDefense;
	FLOAT fEleganceOffense;
	FLOAT fEleganceDefense;
	FLOAT fFunnyOffense;
	FLOAT fFunnyDefense;

	FLOAT fPhysicalReflect;
	FLOAT fEnergyReflect;

	WORD wParalyzeToleranceRate;
	WORD wTerrorToleranceRate;
	WORD wConfuseToleranceRate;
	WORD wStoneToleranceRate;
	WORD wCandyToleranceRate;

	FLOAT fParalyzeKeepTimeDown;
	FLOAT fTerrorKeepTimeDown;
	FLOAT fConfuseKeepTimeDown;
	FLOAT fStoneKeepTimeDown;
	FLOAT fCandyKeepTimeDown;
	FLOAT fBleedingKeepTimeDown;
	FLOAT fPoisonKeepTimeDown;
	FLOAT fStomachacheKeepTimeDown;

	FLOAT fCriticalBlockSuccessRate;

	DWORD dwLpRecoveryWhenHit;
	FLOAT fLpRecoveryWhenHitInPercent;
	DWORD dwEpRecoveryWhenHit;
	FLOAT fEpRecoveryWhenHitInPercent;

	WORD wBaseAbdominalPainDefense;
	WORD wLastAbdominalPainDefense;
	WORD wBasePoisonDefense;
	WORD wLastPoisonDefense;
	WORD wBaseBleedingDefense;
	WORD wLastBleedingDefense;
	WORD wBaseBurnDefense;
	WORD wLastBurnDefense;

	FLOAT fBaseMindCurseImmunity;
	FLOAT fLastMindCurseImmunity;
	FLOAT fBaseBodyCurseImmunity;
	FLOAT fLastBodyCurseImmunity;
	FLOAT fBaseChangeCurseImmunity;
	FLOAT fLastChangeCurseImmunity;

	FLOAT fBaseSkillAnimationSpeed;
	FLOAT fLastSkillAnimationSpeed;
	DWORD dwBaseWeightMax;
	DWORD dwLastWeightMax;

	FLOAT fSkillAggroBonus;
	FLOAT fSkillAggroBonusInPercent;
	FLOAT fDirectHealPowerBonus;
	FLOAT fDirectHealPowerBonusInPercent;
	FLOAT fHotPowerBonus;
	FLOAT fHotPowerBonusInPercent;
	FLOAT fDotValueChangePercent;

	FLOAT fPhysicalCriticalDefenseRate;
	FLOAT fEnergyCriticalDefenseRate;

	WORD wGuardRateBase;
	WORD wGuardRateLast;

	FLOAT fSkillDamageBlockModeSuccessRate;
	FLOAT fCurseBlockModeSuccessRate;
	FLOAT fKnockdownBlockModeSuccessRate;
	FLOAT fHtbBlockModeSuccessRate;

	FLOAT fItemUpgradeBonusRate;
	FLOAT fItemUpgradeBreakBonusRate;

	BYTE byExpBooster;
	BYTE byQuestDropRate;

	//BYTE unkTest; // In latest KR, need remove it. TODO:
};

struct sAVATAR_ATTRIBUTE_LINK
{
	WORD* pwBaseStr;
	WORD* pwLastStr;
	WORD* pwBaseCon;
	WORD* pwLastCon;
	WORD* pwBaseFoc;
	WORD* pwLastFoc;
	WORD* pwBaseDex;
	WORD* pwLastDex;
	WORD* pwBaseSol;
	WORD* pwLastSol;
	WORD* pwBaseEng;
	WORD* pwLastEng;

	DWORD* pdwBaseMaxLP;
	DWORD* pdwLastMaxLP;
	WORD* pwBaseMaxEP;
	WORD* pwLastMaxEP;
	DWORD* pdwBaseMaxAp;
	DWORD* pdwLastMaxAp;
	WORD* pwBaseMaxRP;
	WORD* pwLastMaxRP;

	WORD* pwBaseLpRegen;
	WORD* pwLastLpRegen;
	WORD* pwBaseLpSitdownRegen;
	WORD* pwLastLpSitdownRegen;
	WORD* pwBaseLpBattleRegen;
	WORD* pwLastLpBattleRegen;

	WORD* pwBaseEpRegen;
	WORD* pwLastEpRegen;
	WORD* pwBaseEpSitdownRegen;
	WORD* pwLastEpSitdownRegen;
	WORD* pwBaseEpBattleRegen;
	WORD* pwLastEpBattleRegen;

	WORD* pwBaseApRegen;
	WORD* pwLastApRegen;
	WORD* pwBaseApSitdownRegen;
	WORD* pwLastApSitdownRegen;
	WORD* pwBaseApBattleRegen;
	WORD* pwLastApBattleRegen;

	WORD* pwAirAPDegen;
	WORD* pwAirAPFlyDegen; // This one shows as AP degen in devmaster
	WORD* pwAirAPFLyDashDegen;
	WORD* pwAirAPFlyDashBoostDegen;

	WORD* pwBaseRpRegen;
	WORD* pwLastRpRegen; // "/second"
	WORD* pwLastRpDiminutionRate; // "/second"

	WORD* pwBasePhysicalOffense;
	WORD* pwLastPhysicalOffense;
	WORD* pwBasePhysicalDefense;
	WORD* pwLastPhysicalDefense;

	WORD* pwBaseEnergyOffense;
	WORD* pwLastEnergyOffense;
	WORD* pwBaseEnergyDefense;
	WORD* pwLastEnergyDefense;

	WORD* pwBaseHitRate;
	WORD* pwLastHitRate;
	WORD* pwBaseDodgeRate;
	WORD* pwLastDodgeRate;
	WORD* pwBaseBlockRate;
	WORD* pwLastBlockRate;
	WORD* pwBaseBlockDMG;
	WORD* pwLastBlockDMG;

	WORD* pwBaseCurseSuccessRate;
	WORD* pwLastCurseSuccessRate;
	WORD* pwBaseCurseToleranceRate;
	WORD* pwLastCurseToleranceRate;

	WORD* pwBasePhysicalCriticalRate;
	WORD* pwLastPhysicalCriticalRate;
	WORD* pwBaseEnergyCriticalRate;
	WORD* pwLastEnergyCriticalRate;

	FLOAT* pfBasePhysicalCritDmgRate;
	FLOAT* pfLastPhysicalCritDmgRate;
	FLOAT* pfBaseEnergyCritDmgRate;
	FLOAT* pfLastEnergyCritDmgRate;

	FLOAT* pfBaseRunSpeed;
	FLOAT* pfLastRunSpeed;
	FLOAT* pfBaseAirSpeed;
	FLOAT* pfLastAirSpeed;
	FLOAT* pfBaseAirDashSpeed;
	FLOAT* pfLastAirDashSpeed;
	FLOAT* pfBaseAirDashAccelSpeed;
	FLOAT* pfLastAirDashAccelSpeed;

	WORD* pwBaseAttackSpeedRate;
	WORD* pwLastAttackSpeedRate;
	FLOAT* pfBaseAttackRange;
	FLOAT* pfLastAttackRange;

	FLOAT* pfCastingTimeChangePercent; // 0 = no change, 10 = cast time longer by 10% (110% total)
	FLOAT* pfCoolTimeChangePercent; // 0 = no change, 10 = cooldown longer by 10% (110% total), to make cooldow shorter need negative
	FLOAT* pfKeepTimeChangePercent; // 0 = no change, 10 = duration longer by 10% (110% total)
	FLOAT* pfDotKeepTimeChangeInSeconds;
	FLOAT* pfRequiredEpChangePercent; // 0 = No change, 10 = cost EP +10% (110% total)

	FLOAT* pfHonestOffense;
	FLOAT* pfHonestDefense;
	FLOAT* pfStrangeOffense;
	FLOAT* pfStrangeDefense;
	FLOAT* pfWildOffense;
	FLOAT* pfWildDefense;
	FLOAT* pfEleganceOffense;
	FLOAT* pfEleganceDefense;
	FLOAT* pfFunnyOffense;
	FLOAT* pfFunnyDefense;

	FLOAT* pfPhysicalReflect;
	FLOAT* pfEnergyReflect;

	WORD* pwParalyzeToleranceRate;
	WORD* pwTerrorToleranceRate;
	WORD* pwConfuseToleranceRate;
	WORD* pwStoneToleranceRate;
	WORD* pwCandyToleranceRate;

	FLOAT* pfParalyzeKeepTimeDown;
	FLOAT* pfTerrorKeepTimeDown;
	FLOAT* pfConfuseKeepTimeDown;
	FLOAT* pfStoneKeepTimeDown;
	FLOAT* pfCandyKeepTimeDown;
	FLOAT* pfBleedingKeepTimeDown;
	FLOAT* pfPoisonKeepTimeDown;
	FLOAT* pfStomachacheKeepTimeDown;

	FLOAT* pfCriticalBlockSuccessRate;

	DWORD* pdwLpRecoveryWhenHit;
	FLOAT* pfLpRecoveryWhenHitInPercent;
	DWORD* pdwEpRecoveryWhenHit;
	FLOAT* pfEpRecoveryWhenHitInPercent;

	WORD* pwBaseAbdominalPainDefense;
	WORD* pwLastAbdominalPainDefense;
	WORD* pwBasePoisonDefense;
	WORD* pwLastPoisonDefense;
	WORD* pwBaseBleedingDefense;
	WORD* pwLastBleedingDefense;
	WORD* pwBaseBurnDefense;
	WORD* pwLastBurnDefense;

	FLOAT* pfBaseMindCurseImmunity;
	FLOAT* pfLastMindCurseImmunity;
	FLOAT* pfBaseBodyCurseImmunity;
	FLOAT* pfLastBodyCurseImmunity;
	FLOAT* pfBaseChangeCurseImmunity;
	FLOAT* pfLastChangeCurseImmunity;

	FLOAT* pfBaseSkillAnimationSpeed;
	FLOAT* pfLastSkillAnimationSpeed;
	DWORD* pdwBaseWeightMax;
	DWORD* pdwLastWeightMax;

	FLOAT* pfSkillAggroBonus;
	FLOAT* pfSkillAggroBonusInPercent;
	FLOAT* pfDirectHealPowerBonus;
	FLOAT* pfDirectHealPowerBonusInPercent;
	FLOAT* pfHotPowerBonus;
	FLOAT* pfHotPowerBonusInPercent;
	FLOAT* pfDotValueChangePercent;

	FLOAT* pfPhysicalCriticalDefenseRate;
	FLOAT* pfEnergyCriticalDefenseRate;

	WORD* pwGuardRateBase;
	WORD* pwGuardRateLast;

	FLOAT* pfSkillDamageBlockModeSuccessRate;
	FLOAT* pfCurseBlockModeSuccessRate;
	FLOAT* pfKnockdownBlockModeSuccessRate;
	FLOAT* pfHtbBlockModeSuccessRate;

	FLOAT* pfItemUpgradeBonusRate;
	FLOAT* pfItemUpgradeBreakBonusRate;

	BYTE* pbyExpBooster;
	BYTE* pbyQuestDropRate;

	//BYTE *punkTest; // In latest KR, need remove it. TODO:
};

#pragma pack(pop)

class CNtlBitFlagManager;

class CNtlAvatar
{
public:
	struct sATTRIBUTE_LOGIC
	{
		DWORD dwFieldOffset;
		DWORD(*pCopyAttributeFunction)(void* pvBuffer, void* pvValue);
	};

protected:
	CNtlAvatar(void);
public:
	virtual ~CNtlAvatar(void);

protected:
	void Init();

	virtual void InitializeAttributeLink();

public:
	static CNtlAvatar* GetInstance();

public:

	// Updates sAVATAR_ATTRIBUTE structure with using raw data in a packet.(This function will be used mainly on client-side.)
	// (Packet -> Meta data + sAVATAR_ATTRIBUTE)
	static bool UpdateAvatarAttribute(BYTE byAttributeTotalCount, void* pvRawData, sAVATAR_ATTRIBUTE* pAttributeData);

	// Generates raw data in a packet with using attribute data.(This function will be used mainly on server-side.)
	// (Meta data + Attribute data -> Packet)
	static bool SaveAvatarAttribute(CNtlBitFlagManager* pChangedFlag, sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink, void* pvBuffer, DWORD* pwdDataSize);
	static bool SaveAvatarAttribute(CNtlBitFlagManager* pChangedFlag, sAVATAR_ATTRIBUTE* pAttributeData, void* pvBuffer, DWORD* pwdDataSize);

	// Copies attribute data into sAVATAR_ATTRIBUTE structure.(This function will be used mainly on server-side.)
	// (Attribute data -> sAVATAR_ATTRIBUTE)
	static bool FillAvatarAttribute(sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink, sAVATAR_ATTRIBUTE* pAttributeData);


	//NEW SELFMADE! ABOVE NOT USED
	// convert from sAVATAR_ATTRIBUTE to sAVATAR_ATTRIBUTE_LINK
	static bool FillAvatarAttributeNew(sAVATAR_ATTRIBUTE* pAttributeData, sAVATAR_ATTRIBUTE_LINK* pAttributeDataLink);
	// Very bad way to convert sAVATAR_ATTRIBUTE to sAVATAR_ATTRIBUTE_LINK
	static sAVATAR_ATTRIBUTE_LINK ConvertAVATAR_ATTRIBUTE(sAVATAR_ATTRIBUTE* avt);

public:
	static sATTRIBUTE_LOGIC m_attributeLogic[AVATAR_ATTRIBUTE_COUNT];

	// needed to convert from sAVATAR_ATTRIBUTE to sAVATAR_ATTRIBUTE_LINK
	static sATTRIBUTE_LOGIC m_attributeLogicNew[AVATAR_ATTRIBUTE_COUNT];

	static DWORD CopyValueByType_BYTE(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByType_WORD(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByType_float(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByType_FLOAT(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByType_DWORD(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByType_int(void* pvValue, void* pvBuffer);

	static DWORD CopyValueByTypeNew_BYTE(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByTypeNew_WORD(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByTypeNew_float(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByTypeNew_FLOAT(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByTypeNew_DWORD(void* pvValue, void* pvBuffer);
	static DWORD CopyValueByTypeNew_int(void* pvValue, void* pvBuffer);
};