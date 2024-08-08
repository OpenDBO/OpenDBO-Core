/*****************************************************************************
*
* File			: CNtlSobWorldItem.h
* Author		: Peessi
* Copyright		: (аж)NTL
* Date			: 2006. 7. 12	
* Abstract		: Simulation object World item attribute base class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SOB_WORLDITEM_ATTR_H__
#define __NTL_SOB_WORLDITEM_ATTR_H__

#include "NtlSobAttr.h"

class CNtlSobWorldItemAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL( CNtlSobWorldItemAttr, NTL_DEFAULT_MEMORY_POOL )

public:
	enum eDROPTYPE { DROPTYPE_NONE = 0, DROPTYPE_ITEM, DROPTYPE_MONEY };

private:

	eDROPTYPE	  m_eType;

	sITEM_TBLDAT* m_pItemTbl;
	RwUInt32	  m_uiRank;
	RwUInt32	  m_uiGrade;
	RwUInt32	  m_uiOptionTblIdx1;
	RwUInt32	  m_uiOptionTblIdx2;
	RwBool		  m_bUnidentified;
	RwUInt8		  m_ucBattleAttribute;

	RwUInt32	  m_uiMoneyAmount;	
	
public:

	CNtlSobWorldItemAttr(VOID) 
	{ 
		m_eType = DROPTYPE_NONE;
		m_pItemTbl = NULL;
		m_uiGrade = 0xFFFFFFFF;
		m_uiRank = 0xFFFFFFFF;
		m_uiOptionTblIdx1 = INVALID_TBLIDX;
		m_uiOptionTblIdx2 = INVALID_TBLIDX;
		m_uiMoneyAmount = 0;
		m_bUnidentified = FALSE;
	}

	virtual ~CNtlSobWorldItemAttr(VOID) {}
	virtual RwBool Create(VOID) { SetClassID(SLCLASS_WORLD_ITEM); return TRUE; }
	virtual VOID Destroy(VOID) {}
	virtual VOID HandleEvents( RWS::CMsg &msg );

public:

	VOID SetItemTbl(const sITEM_TBLDAT* pItemTbl);
	sITEM_TBLDAT* GetItemTbl(VOID) const;
	RwUInt32 GetGrade(VOID) const;
	RwUInt32 GetRank(VOID) const;
	RwUInt32 GetOptionTblIdx1(VOID) const;
	RwUInt32 GetOptionTblIdx2(VOID) const;
	RwUInt8	 GetBattleAttribute(VOID) const;

	VOID SetMoney(RwUInt32 uiMoney);
	RwUInt32 GetMoney(VOID) const;

	RwBool IsItem(VOID);
	RwBool IsMoney(VOID);
	RwBool IsIdentified(VOID);
	RwBool IsDragonBall(VOID); 
};


inline VOID CNtlSobWorldItemAttr::SetItemTbl(const sITEM_TBLDAT* pItemTbl)
{
	m_pItemTbl = const_cast<sITEM_TBLDAT*>(pItemTbl);
}

inline sITEM_TBLDAT* CNtlSobWorldItemAttr::GetItemTbl(VOID) const
{
	return m_pItemTbl;
}

inline RwUInt32 CNtlSobWorldItemAttr::GetGrade(VOID) const
{
	return m_uiGrade;
}

inline RwUInt32 CNtlSobWorldItemAttr::GetRank(VOID) const
{
	return m_uiRank;
}

inline RwUInt32 CNtlSobWorldItemAttr::GetOptionTblIdx1(VOID) const
{
	return m_uiOptionTblIdx1;
}

inline RwUInt32 CNtlSobWorldItemAttr::GetOptionTblIdx2(VOID) const
{
	return m_uiOptionTblIdx2;
}

inline RwUInt8 CNtlSobWorldItemAttr::GetBattleAttribute(VOID) const
{
	return m_ucBattleAttribute;
}

inline VOID CNtlSobWorldItemAttr::SetMoney(RwUInt32 uiMoney)
{
	m_uiMoneyAmount = uiMoney;
}

inline RwUInt32 CNtlSobWorldItemAttr::GetMoney(VOID) const
{
	return m_uiMoneyAmount;
}

inline RwBool CNtlSobWorldItemAttr::IsItem(VOID) 
{
	return ( m_eType == DROPTYPE_ITEM ) ? TRUE : FALSE;
}

inline RwBool CNtlSobWorldItemAttr::IsMoney(VOID) 
{
	return ( m_eType == DROPTYPE_MONEY ) ? TRUE : FALSE;
}

inline RwBool CNtlSobWorldItemAttr::IsIdentified(VOID)
{
	return !m_bUnidentified;
}

#endif
