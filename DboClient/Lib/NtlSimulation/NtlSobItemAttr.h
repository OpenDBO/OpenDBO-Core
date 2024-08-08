/*****************************************************************************
 *
 * File			: NtlSobItemAttr.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 30	
 * Abstract		: Simulation object item attribute base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SOBITEM_ATTR_H__
#define __NTL_SOBITEM_ATTR_H__

#include "NtlItem.h"
#include "NtlSobAttr.h"

class CNtlSobItemAttr : public CNtlSobAttr
{
	DECLEAR_MEMORY_POOL(CNtlSobItemAttr, NTL_DEFAULT_MEMORY_POOL)
	
private:

	sITEM_TBLDAT *m_pItemTbl;
	sITEM_OPTION_TBLDAT* m_pOptionTbl;
	RwUInt8 m_byStackNum;
	RwUInt8 m_byRank;		/** item 등급 */
	RwUInt8 m_byGrade;		/** item 업그레이드 등급 **/
	RwUInt8 m_byDur;		/** item 내구도 */
	RwUInt8 m_byMaxDur;		/** item max 내구도 */
	RwBool	m_bNeedToIdentify; /** item identify 필요여부 **/
	RwUInt32 m_uiOptionIdx1;	/** 추가 옵션 테이블인덱스 **/
	RwUInt32 m_uiOptionIdx2;	/** 추가 옵션 테이블인덱스 **/
	RwUInt8 m_ucBattleAttribute;

	RwUInt8	m_ucRestrictType;	/** 귀속상태 eITEM_RESTRICT_TYPE **/
	std::wstring m_wstrMaker;	/** 제작자 **/

	RwUInt8		m_ucDurationType;	/** 사용기간 eDURATIONTYPE **/
	DBOTIME		m_StartTime;		/** 인벤토리에 들어온 날짜 **/
	DBOTIME		m_EndTime;			/** 사용 만료 기간 **/
	DBOTIME		m_RemainTime;		/** 남은시간 초 **/
	RwReal		m_fRemainTimeBelowPeriod;		/** 남은시간 소수점 이하 **/
	RwReal		m_fNotifyTime;		/** 남은시간 알림간격 **/
		
public:

	CNtlSobItemAttr() 
	{ 
		m_pItemTbl = NULL;
		m_byStackNum = 1;
		m_byRank = 0;
		m_byGrade = 0;
		m_byDur = 1;
		m_byMaxDur = 1;
		m_bNeedToIdentify = FALSE;
		m_uiOptionIdx1 = INVALID_TBLIDX;
		m_uiOptionIdx2 = INVALID_TBLIDX;
		m_ucBattleAttribute = 0;

		m_ucRestrictType = ITEM_RESTRICT_TYPE_NONE;
		
		m_ucDurationType = eDURATIONTYPE_NORMAL;
		m_StartTime = 0;
		m_EndTime = 0;
		m_RemainTime = 0;
		m_fRemainTimeBelowPeriod = 0.0f;
		m_fNotifyTime = 0;
	}

	virtual ~CNtlSobItemAttr() {}
	virtual RwBool Create(void) { SetClassID(SLCLASS_SLOT_ITEM); return TRUE; }
	virtual void Destroy(void) {}
	virtual void HandleEvents(RWS::CMsg &pMsg);

	void	Update(RwReal fElapsedTime);		

public:

	void SetItemTbl(const sITEM_TBLDAT *pItemTbl);
	sITEM_TBLDAT* GetItemTbl(void) const;

	void SetStackNum(RwUInt8 byStackNum);
	RwUInt8 GetStackNum(void) const;

	void SetRank(RwUInt8 byRank);
	RwUInt8 GetRank(void) const;

	void SetGrade(RwUInt8 byGrade);
	RwUInt8 GetGrade(void) const;

	void SetDur(RwUInt8 byDur);
	RwUInt8 GetDur(void) const;

	void SetMaxDur(RwUInt8 byDur);
	RwUInt8 GetMaxDur(void) const;

	void SetOptionIdx1(RwUInt32 uiOptionTblIdx);
	RwUInt32 GetOptionIdx1(void) const;

	void SetOptionIdx2(RwUInt32 uiOptionTblIdx);
	RwUInt32 GetOptionIdx2(void) const;

	void SetBattleAttribute(RwUInt8 byBattleAttribute);
	RwUInt8 GetBattleAttribute(void) const;

	RwUInt8 GetRestrictType(void) const;
	const WCHAR* GetMaker(void) const;

	RwUInt8 GetDurationType(void) const;
	DBOTIME GetStartTime(void) const;
	DBOTIME GetEndTime(void) const;

	void	SetRemainTime( DBOTIME RemainTime );
	DBOTIME GetRemainTime(void) const;

	RwBool	IsExpired(void) const;
	void	Expired(void);

	RwUInt32 GetTblIdx(void) const;

	RwBool IsNeedToIdentify(void);

	RwBool IsContainerItem(void);
	RwInt32 GetChildSlotSize(void);

	RwBool IsEquipItem(void);
	RwBool IsBagItem(void);
	RwBool IsScouterItem(void);
	RwBool IsScouterPartItem();
};


inline void CNtlSobItemAttr::SetItemTbl(const sITEM_TBLDAT *pItemTbl)
{
	m_pItemTbl = const_cast<sITEM_TBLDAT*>(pItemTbl);
}

inline sITEM_TBLDAT* CNtlSobItemAttr::GetItemTbl(void) const
{
	return m_pItemTbl;
}

inline void CNtlSobItemAttr::SetStackNum(RwUInt8 byStackNum)
{
	m_byStackNum = byStackNum;
}

inline RwUInt8 CNtlSobItemAttr::GetStackNum(void) const
{
	return m_byStackNum;
}

inline void CNtlSobItemAttr::SetRank(RwUInt8 byRank)
{
	m_byRank = byRank;
}

inline RwUInt8 CNtlSobItemAttr::GetRank(void) const
{
	return m_byRank;
}

inline void CNtlSobItemAttr::SetGrade( RwUInt8 byGrade )
{
	m_byGrade = byGrade;
}

inline RwUInt8 CNtlSobItemAttr::GetGrade(void) const
{
	return m_byGrade;
}

inline void CNtlSobItemAttr::SetDur(RwUInt8 byDur)
{
	m_byDur	= byDur;
}

inline RwUInt8 CNtlSobItemAttr::GetDur(void) const
{
	return m_byDur;
}

inline void CNtlSobItemAttr::SetMaxDur(RwUInt8 byDur)
{
	m_byMaxDur = byDur;
}

inline RwUInt8 CNtlSobItemAttr::GetMaxDur(void) const
{
	return m_byMaxDur;
}

inline void CNtlSobItemAttr::SetOptionIdx1(RwUInt32 uiOptionTblIdx)
{
	m_uiOptionIdx1 = uiOptionTblIdx;
}

inline RwUInt32 CNtlSobItemAttr::GetOptionIdx1(void) const
{
	return m_uiOptionIdx1;
}

inline void CNtlSobItemAttr::SetOptionIdx2(RwUInt32 uiOptionTblIdx)
{
	m_uiOptionIdx2 = uiOptionTblIdx;
}

inline RwUInt32 CNtlSobItemAttr::GetOptionIdx2(void) const
{
	return m_uiOptionIdx2;
}

inline void CNtlSobItemAttr::SetBattleAttribute(RwUInt8 byBattleAttribute)
{
	m_ucBattleAttribute = byBattleAttribute;
}

inline RwUInt8 CNtlSobItemAttr::GetBattleAttribute(void) const
{
	return m_ucBattleAttribute;
}

inline RwUInt8 CNtlSobItemAttr::GetRestrictType(void) const
{
	return m_ucRestrictType;
}

inline const WCHAR* CNtlSobItemAttr::GetMaker(void) const
{
	return m_wstrMaker.c_str();
}

inline RwUInt8 CNtlSobItemAttr::GetDurationType(void) const
{
	return m_ucDurationType;
}

inline DBOTIME CNtlSobItemAttr::GetStartTime(void) const
{
	return m_StartTime;
}

inline DBOTIME CNtlSobItemAttr::GetEndTime(void) const
{
	return m_EndTime;
}

inline void CNtlSobItemAttr::SetRemainTime( DBOTIME RemainTime )
{
	m_RemainTime = RemainTime;
	m_fRemainTimeBelowPeriod = 0.0f;
	m_fNotifyTime = 0.0f;
}

inline RwBool CNtlSobItemAttr::IsExpired(void) const
{
	if( m_ucDurationType && m_RemainTime <= 0 )
		return TRUE;

	return FALSE;
}

inline void CNtlSobItemAttr::Expired(void)
{
	SetRemainTime( 0 );
}

inline DBOTIME CNtlSobItemAttr::GetRemainTime(void) const
{
	return m_RemainTime;
}

inline RwBool CNtlSobItemAttr::IsNeedToIdentify(void)
{
	return m_bNeedToIdentify;
}


#endif
