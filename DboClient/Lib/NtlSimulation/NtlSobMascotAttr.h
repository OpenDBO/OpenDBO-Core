/*****************************************************************************
 *
 * File			: NtlSobMascotAttr.h
 * Author		: Alejandro-HUB
 * Copyright	: (C)NTL
 * Date			: 2024. 11. 10
 * Abstract		: Simulation object player attribute base class
 *****************************************************************************
 * Desc         :
 *
 *****************************************************************************/

#ifndef _NTL_SOB_MASCOT_ATTR_H_
#define _NTL_SOB_MASCOT_ATTR_H_

#include "NtlSharedDef.h"
#include "NtlSobAttr.h"
#include "NtlMascot.h"

// Forward declarations
struct sMASCOT_TBLDAT;
struct sMASCOT_GRADE_TBLDAT;

class CNtlSobMascotAttr : public CNtlSobLifeAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobMascotAttr, NTL_DEFAULT_MEMORY_POOL)

public:
	RwUInt8		m_bySummonSourceType;	/** skill or item */
	TBLIDX	m_uiSourceTblId;		/** skill or item table id */
	sNPC_TBLDAT* m_pNpcTbl;				/** mob table data pointer */
	sMASCOT_TBLDAT* m_pMascotTbl;		/** mascot table data pointer */
	sMASCOT_GRADE_TBLDAT* m_pMascotGradeTbl; /** mascot grade table data pointer */
	
	// Mascot-specific data
	BYTE m_byMascotIndex;				/** mascot index */
	TBLIDX m_uiMascotTblidx;			/** mascot table index */
	TBLIDX m_uiItemTblidx;				/** item table index */
	DWORD m_dwCurVP;					/** current VP */
	DWORD m_dwMaxVP;					/** max VP */
	DWORD m_dwCurExp;					/** current experience */
	BYTE m_byItemRank;					/** item rank */
	TBLIDX m_skillTblidx[DBO_MASCOT_MAX_SKILL_COUNT]; /** skill table indices */
	
	// Additional mascot properties from table
	bool m_bValidityAble;				/** validity flag from table */
	BYTE m_byModelType;					/** model type from table */
	BYTE m_bySlotNum;					/** slot number from table */
	WORD m_wSPDecreaseRate;				/** SP decrease rate from table */

public:

	CNtlSobMascotAttr();
	virtual ~CNtlSobMascotAttr() {};

	virtual RwBool Create(void) { SetClassID(SLCLASS_MASCOT); return TRUE; }
	virtual void Destroy(void) {}
	virtual void HandleEvents(RWS::CMsg& pMsg);

public:

	sNPC_TBLDAT* GetNpcTbl(void) const;
	sMASCOT_TBLDAT* GetMascotTbl(void) const;
	sMASCOT_GRADE_TBLDAT* GetMascotGradeTbl(void) const;
	
	void SetMascotTbl(sMASCOT_TBLDAT* pMascotTbl);
	void SetMascotGradeTbl(sMASCOT_GRADE_TBLDAT* pMascotGradeTbl);
	
	// Setters for mascot data from MascotGui.cpp
	void SetMascotIndex(BYTE byIndex);
	void SetMascotTblidx(TBLIDX uiTblidx);
	void SetItemTblidx(TBLIDX uiItemTblidx);
	void SetCurrentVP(DWORD dwCurVP);
	void SetMaxVP(DWORD dwMaxVP);
	void SetCurrentExp(DWORD dwCurExp);
	void SetItemRank(BYTE byItemRank);
	void SetSkillTblidx(int iIndex, TBLIDX uiSkillTblidx);
	
	// Getters for mascot data
	BYTE GetMascotIndex(void) const;
	TBLIDX GetMascotTblidx(void) const;
	TBLIDX GetItemTblidx(void) const;
	DWORD GetCurrentVP(void) const;
	DWORD GetMaxVP(void) const;
	DWORD GetCurrentExp(void) const;
	BYTE GetItemRank(void) const;
	TBLIDX GetSkillTblidx(int iIndex) const;
	
	// Getters for additional mascot properties
	bool GetValidityAble(void) const;
	BYTE GetModelType(void) const;
	BYTE GetSlotNum(void) const;
	WORD GetSPDecreaseRate(void) const;
};

#endif