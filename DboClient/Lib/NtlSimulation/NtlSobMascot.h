/*****************************************************************************
 *
 * File			: NtlSobMascot.h
 * Author		: Alejandro-HUB
 * Copyright	: (C)NTL
 * Date			: 2024. 11. 10
 * Abstract		: Simulation object mascot base class
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/

#ifndef __NTL_SOBMASCOT_H__
#define __NTL_SOBMASCOT_H__

// Core
#include "NtlMath.h"

// shared
#include "NtlMascot.h"

// SL
#include "NtlSobActor.h"
#include "NtlSLDef.h"

class CNtlSobMascotAttr;

class CNtlSobMascot : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL(CNtlSobMascot, NTL_DEFAULT_MEMORY_POOL)

private:
	CNtlSobMascotAttr*		m_pMascotAttr;			///< Mascot attribute
	RwBool					m_bSummoned;			///< Is mascot summoned
	RwUInt8					m_byMascotIndex;		///< Mascot index
	TBLIDX					m_tblidx;				///< Mascot table index
	RwUInt32				m_dwCurrentVP;			///< Current VP
	RwUInt32				m_dwMaxVP;				///< Maximum VP
	RwUInt32				m_dwCurrentExp;			///< Current experience
	RwUInt8					m_byItemRank;			///< Item rank
	TBLIDX					m_skillTblidx[DBO_MASCOT_MAX_SKILL_COUNT];	///< Skill table indices

public:
	CNtlSobMascot();
	virtual ~CNtlSobMascot();

	virtual RwBool Create(void);
	virtual void Destroy(void);
	virtual void Update(RwReal fElapsed);
	virtual void HandleEvents(RWS::CMsg &pMsg);

	// World mascot creation (called only when summoned)
	void					CreateWorldMascot(void);

	// Attribute related functions
	CNtlSobMascotAttr*		GetMascotAttr(void) const;
	void					SetMascotAttr(CNtlSobMascotAttr *pMascotAttr);

	// Mascot specific functions
	RwBool					IsSummoned(void) const;
	void					SetSummoned(RwBool bSummoned);

	RwUInt8					GetMascotIndex(void) const;
	void					SetMascotIndex(RwUInt8 byIndex);

	TBLIDX					GetMascotTblidx(void) const;
	void					SetMascotTblidx(TBLIDX tblidx);
	
	void					SetItemTblidx(TBLIDX itemTblidx);

	RwUInt32				GetCurrentVP(void) const;
	void					SetCurrentVP(RwUInt32 dwVP);

	RwUInt32				GetMaxVP(void) const;
	void					SetMaxVP(RwUInt32 dwMaxVP);

	RwUInt32				GetCurrentExp(void) const;
	void					SetCurrentExp(RwUInt32 dwExp);

	RwUInt8					GetItemRank(void) const;
	void					SetItemRank(RwUInt8 byRank);

	TBLIDX					GetSkillTblidx(RwUInt8 bySkillIndex) const;
	void					SetSkillTblidx(RwUInt8 bySkillIndex, TBLIDX skillTblidx);

	// Override to return mascot attribute as the main attribute
	virtual CNtlSobAttr*	GetSobAttr(void) const;
};

#endif 