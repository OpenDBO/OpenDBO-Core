/*****************************************************************************
*
* File			: NtlSobSkillAttr.h
* Author		: HyungSuk, Jang
* Copyright	: (аж)NTL
* Date			: 2006. 4. 24	
* Abstract		: Simulation object skill attribute base class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SOBBUFF_ATTR_H__
#define __NTL_SOBBUFF_ATTR_H__

#include "NtlSobAttr.h"
#include "SkillTable.h"

class CNtlSobBuffAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobBuffAttr, NTL_DEFAULT_MEMORY_POOL)

public:

	RwUInt8 m_byType;			/** item or skill */
	sSKILL_TBLDAT *m_pBaseSkillTbl;	/** skill table data pointer */
	sSKILL_TBLDAT m_SkillTbl;	/** skill table data effected by passive skill*/
	sITEM_TBLDAT *m_pItemTbl;	/** skill table data pointer */
	RwBool	m_bBlessType;		/** bless or curse */

public:

	CNtlSobBuffAttr() 
	{ 
		m_pBaseSkillTbl = NULL;
		m_pItemTbl = NULL;
		m_bBlessType = FALSE;
	}

	virtual ~CNtlSobBuffAttr() {}

	virtual RwBool Create(void) { SetClassID(SLCLASS_BUFF); return TRUE; }

	virtual void Destroy(void) {}

	virtual void HandleEvents(RWS::CMsg &pMsg);

public:

	void SetButtType(RwUInt8 byType);
	RwUInt8 GetBuffType(void) const;

	TBLIDX GetTblIdx(void);

	void SetSkillTbl(const sSKILL_TBLDAT *pSkillTbl);
	sSKILL_TBLDAT* GetSkillTbl(void);
	sSKILL_TBLDAT* GetBaseSkillTbl(void) const;

	void SetItemTbl(const sITEM_TBLDAT *pItemTbl);
	sITEM_TBLDAT* GetItemTbl(void) const;

	RwBool IsBlessType(void) const;
};

inline void CNtlSobBuffAttr::SetButtType(RwUInt8 byType)
{
	m_byType = byType;
}

inline RwUInt8 CNtlSobBuffAttr::GetBuffType(void) const
{
	return m_byType;
}

inline void CNtlSobBuffAttr::SetSkillTbl(const sSKILL_TBLDAT *pSkillTbl)
{
	m_pBaseSkillTbl = const_cast<sSKILL_TBLDAT*>(pSkillTbl);
	m_SkillTbl = *pSkillTbl;
}

inline sSKILL_TBLDAT* CNtlSobBuffAttr::GetSkillTbl(void)
{
	return &m_SkillTbl;
}

inline sSKILL_TBLDAT* CNtlSobBuffAttr::GetBaseSkillTbl(void) const
{
	return m_pBaseSkillTbl;
}

inline void CNtlSobBuffAttr::SetItemTbl(const sITEM_TBLDAT *pItemTbl)
{
	m_pItemTbl = const_cast<sITEM_TBLDAT*>(pItemTbl);
}

inline sITEM_TBLDAT* CNtlSobBuffAttr::GetItemTbl(void) const
{
	return m_pItemTbl;
}

inline RwBool CNtlSobBuffAttr::IsBlessType(void) const
{
	return m_bBlessType;
}

#endif