/*****************************************************************************
 *
 * File			: NtlSobAvatarAtt.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object avatar attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBAVATAR_ATTR_H__
#define __NTL_SOBAVATAR_ATTR_H__

#include "NtlSobPlayerAttr.h"

class CNtlSobAvatarAttr : public CNtlSobPlayerAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobAvatarAttr, NTL_DEFAULT_MEMORY_POOL)
	
public:

	RwUInt32 m_uiExp;		// 현재 level에서의 current exp	
	RwUInt32 m_uiMaxExp;	// 현재 level에서의 max exp

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

	RwUInt16 m_wLastRpRegen;			// RP 증가 속도.(/second)
	RwUInt16 m_wLastRpDimimutionRate;	// RP 감소 속도.(/second)

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
	
	RwUInt16 m_wBaseCurseSuccessRate;
	RwUInt16 m_wLastCurseSuccessRate;
	RwUInt16 m_wBaseCurseToleranceRate;
	RwUInt16 m_wLastCurseToleranceRate;

	RwUInt16 m_wBasePhysicalCriticalRate;
	RwUInt16 m_wLastPhysicalCriticalRate;
	RwUInt16 m_wBaseEnergyCriticalRate;
	RwUInt16 m_wLastEnergyCriticalRate;

	RwUInt16 m_wBaseAttackSpeedRate;
	RwUInt16 m_wLastAttackSpeedRate;

	RwUInt32 m_uiZenny;	

	RwUInt32 m_uiSp;

	RwUInt32 m_uiHonorPoint;			///< 명예 포인트
	RwUInt32 m_uiMudosaPoint;			///< 무도사 포인트

	RwBool	 m_bCanChangeClass;			///< 전직 가능 여부

    //////////////////////////////////////////////////////////////////////////
    // PC방 관련
    DWORD    m_dwNetPy;                 ///< 현재 NetPy

	// Battle Attributes bonus rate
	RwReal m_fHonestOffence;
	RwReal m_fHonestDefence;
	RwReal m_fStrangeOffence;
	RwReal m_fStrangeDefence;
	RwReal m_fWildOffence;
	RwReal m_fWildDefence;
	RwReal m_fEleganceOffence;
	RwReal m_fEleganceDefence;
	RwReal m_fFunnyOffence;
	RwReal m_fFunnyDefence;

public:

	CNtlSobAvatarAttr(); 

	virtual ~CNtlSobAvatarAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_AVATAR); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

	RwUInt32 GetAP(void);	
};

#endif
