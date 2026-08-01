#ifndef __INC_DBO_GAME_BATTLE_H__
#define __INC_DBO_GAME_BATTLE_H__

#include "NtlRandom.h"
#include "SkillTable.h"

class CPlayer;
class CCharacter;
class CCharacterObject;
struct sAVATAR_ATTRIBUTE;
class CCharacterAtt;

extern bool BattleIsCrit(CCharacterAtt* pAttackerAtt, CCharacterAtt* pTargetAtt, bool bIsPhysical);
extern bool	BattleIsDodge(bool bTargetPC, WORD hitrate, WORD dodge, BYTE byAttackerLv, BYTE byTargetLv);
extern bool	BattleIsResist(WORD wSuccessRate, WORD wResistRate, BYTE byAttackerLv, BYTE byTargetLv);

extern bool	BattleIsBlock(WORD wDefenceRate, BYTE byAttackerLv, BYTE byTargetLv);

extern float CalcMeleeDamage(CCharacter* pAttacker, CCharacter* pVictim);

extern void CalcSkillDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float fBaseSkillDmg, float& resultvalue, BYTE& rAttackResult, int& rfReflectDmg, sDBO_LP_EP_RECOVERED* pLpEpRecover, bool bIncreaseDmg = false, bool bAttackFromBehindBonus = false);
extern void CalcSpecialSkillDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float fBaseSkillDmg, float& resultvalue, BYTE& rAttackResult, int& rfReflectDmg, sDBO_LP_EP_RECOVERED& rLpEpRecover);
extern void CalcSkillDotDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, WORD wDefence, float fBaseSkillDmg, float fBonusDmg, float& resultvalue, BYTE rAttackResult, BYTE byEffectCode);

extern void CalcLifeStealDamage(CCharacterObject* pCaster, CCharacterObject* victim, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float fBaseSkillDmg, float& resultvalue);
///

extern void CalcDirectHeal(CCharacterObject* pCaster, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float& resultvalue);
extern void CalcHealOverTime(CCharacterObject* pCaster, sSKILL_TBLDAT* skilltbl, BYTE byEffectNr, float& resultvalue);

extern void IncreaseTargetEnemyAggro(CCharacter* pCaster, CCharacter* pTarget, DWORD dwDefaultAggro);

extern float GetReflectDamage(float fDmg, BYTE byAttackType, CCharacterAtt* pVictimAtt);
extern float GetSkillReflectDamage(float fDmg, BYTE bySkillType, CCharacterAtt* pVictimAtt);

extern float GetAttributeBonusRate(bool bIsPc, bool bSubWeapon, BYTE byOffence, BYTE byDefence, BYTE bySubOffence, sAVATAR_ATTRIBUTE& sOffenceAttribute, sAVATAR_ATTRIBUTE& sDefenceAttribute);

extern float CalcPropBonus(CCharacterObject* pCaster, CCharacterObject* pVictim, float fAttackerPower);

extern float CalcDefensePower(float fDefence, float fArmorPenRate);

extern float CalcCritBonus(float fResultValue, CCharacterAtt* pCasterAtt, BYTE bySkill_Type, BYTE byAttackResult, float fCritDefRate, bool bIncreaseDmg = false);

extern float CalcMinDamage(float fDamage, float fLevel);

extern float CalcMaxDamage(float fDamage, float fLevel);

extern bool IsValidAttribute(BYTE byAttr);

extern float GetOffenceAttribute(BYTE byAttr, sAVATAR_ATTRIBUTE& sOffenceAttribute);

extern float GetDefenceAttribute(BYTE byAttr, sAVATAR_ATTRIBUTE& sDefenceAttribute);
#endif


