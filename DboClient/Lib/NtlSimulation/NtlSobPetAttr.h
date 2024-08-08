/*****************************************************************************
 *
 * File			: NtlSobPetAtt.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object player attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBPET_ATTR_H__
#define __NTL_SOBPET_ATTR_H__

#include "NtlSharedDef.h"
#include "NtlSobAttr.h"


class CNtlSobPetAttr : public CNtlSobBattleAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobPetAttr, NTL_DEFAULT_MEMORY_POOL)
	
public:

	RwUInt8		m_bySummonSourceType;	/** skill or item */	
	RwUInt32	m_uiSourceTblId;		/** skill or item table id */
	sNPC_TBLDAT *m_pNpcTbl;				/** mob table data pointer */

	RwUInt8 m_byBaseStr;		// 근력
	RwUInt8 m_byLastStr;		// 근력
	RwUInt8 m_byBaseCon;		// 건강
	RwUInt8 m_byLastCon;		// 건강
	RwUInt8 m_byBaseFoc;		// 종족과 클래스에 따른 생성시의 집중을 기입
	RwUInt8 m_byLastFoc;		// 종족과 클래스에 따른 생성시의 집중을 기입
	RwUInt8 m_byBaseDex;		// 민첩
	RwUInt8 m_byLastDex;		// 민첩
	RwUInt8 m_byBaseSol;		// 종족과 클래스에 따른 생성시의 정신을 기입
	RwUInt8 m_byLastSol;		// 종족과 클래스에 따른 생성시의 정신을 기입
	RwUInt8 m_byBaseEng;		// 종족과 클래스에 따른 생성시의 원기를 기입
	RwUInt8 m_byLastEng;		// 종족과 클래스에 따른 생성시의 원기를 기입

	RwUInt16 m_wBasePhyshicalOffence;
	RwUInt16 m_wLastPhyshicalOffence;
	RwUInt16 m_wBasePhyshicalDefence;
	RwUInt16 m_wLastPhyshicalDefence;
	
	RwUInt16 m_wBaseEnergyOffence;
	RwUInt16 m_wLastEnergyOffence;
	RwUInt16 m_wBaseEnergyDefence;
	RwUInt16 m_wLastEnergyDefence;
	
	RwUInt16 m_wBaseAttackRate;
	RwUInt16 m_wLastAttackRate;
	RwUInt16 m_wBaseDodgeRate;
	RwUInt16 m_wLastDodgeRate;
	RwUInt16 m_wBaseBlockRate;
	RwUInt16 m_wLastBlockRate;
	RwUInt16 m_wBaseAttackSpeedRate;
	RwUInt16 m_wLastAttackSpeedRate;

	RwUInt16 m_wBaseCurseSuccessRate;
	RwUInt16 m_wLastCurseSuccessRate;
	RwUInt16 m_wBaseCurseToleranceRate;
	RwUInt16 m_wLastCurseToleranceRate;

	RwUInt16 m_wBasePhysicalCriticalRate;
	RwUInt16 m_wLastPhysicalCriticalRate;
	RwUInt16 m_wBaseEnergyCriticalRate;
	RwUInt16 m_wLastEnergyCriticalRate;
	
public:

	CNtlSobPetAttr(); 

	virtual ~CNtlSobPetAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_PET); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	sNPC_TBLDAT* GetNpcTbl(void) const;
};

#endif
