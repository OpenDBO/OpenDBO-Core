#ifndef _DBO_TSACTCONTREWARD_H_
#define _DBO_TSACTCONTREWARD_H_


#include "DboTSContGAct.h"


class CQuestRewardTable;


/**
	Reward container
*/


struct sREWARD_INFO
{
	eREWARD_TYPE						m_eType;
	unsigned int						m_uiIdx;
	int									m_nValue;
};


class CDboTSContReward : public CNtlTSCont
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	NTL_TS_TC_ID						m_tcNextLinkId;
	NTL_TS_TC_ID						m_tcCancelLinkId;

	eREWARD_CONTAINER_TYPE				m_eRwdContType;
	unsigned int						m_uiLimitTime;

	unsigned int						m_uiDesc;

	bool								m_bUseTable;
	bool								m_bLoadTable;

	unsigned int						m_uiRewardTblIdx;

	sREWARD_INFO						m_asDefReward[MAX_DEFAULT_REWARD];		// 기본 보상
	sREWARD_INFO						m_asSelReward[MAX_SELECTION_REWARD];	// 선택 보상

// Constructions and Destructions
public:
	CDboTSContReward( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_CONT_TYPE_ID_CONT_REWARD; }

	// Next link container
	NTL_TS_TC_ID						GetNextLinkID( void ) const;
	bool								AttachNextLink( NTL_TS_TC_ID tcId );
	void								DetachNextLink( void );

	// Cancel link container
	NTL_TS_TC_ID						GetCancelLinkID( void ) const;
	bool								AttachCancelLink( NTL_TS_TC_ID tcId );
	void								DetachCancelLink( void );

	eREWARD_CONTAINER_TYPE				GetRewardContType( void ) const;
	void								SetRewardContType( eREWARD_CONTAINER_TYPE eRwdContType );

	unsigned int						GetLimitTime( void ) const;
	void								SetLimitTime( unsigned int uiLimitTime );

	unsigned int						GetDesc( void ) const;
	void								SetDesc( unsigned int uiDesc );

	bool								IsUseTable( void ) const;

	unsigned int						GetRewardTableIndex( void ) const;

	const sREWARD_INFO&					GetDefRewardInfo( int nIdx, CQuestRewardTable* pRewardTbl );
	const sREWARD_INFO&					GetSelRewardInfo( int nIdx, CQuestRewardTable* pRewardTbl );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );

	void								LoadTable( CQuestRewardTable* pRewardTbl );
};

inline NTL_TS_TC_ID CDboTSContReward::GetNextLinkID( void ) const
{
	return m_tcNextLinkId;
}

inline NTL_TS_TC_ID CDboTSContReward::GetCancelLinkID( void ) const
{
	return m_tcCancelLinkId;
}

inline eREWARD_CONTAINER_TYPE CDboTSContReward::GetRewardContType( void ) const
{
	return m_eRwdContType;
}

inline unsigned int CDboTSContReward::GetLimitTime( void ) const
{
	return m_uiLimitTime;
}

inline unsigned int CDboTSContReward::GetDesc( void ) const
{
	return m_uiDesc;
}

inline bool CDboTSContReward::IsUseTable( void ) const
{
	return m_bUseTable;
}

inline unsigned int CDboTSContReward::GetRewardTableIndex( void ) const
{
	return m_uiRewardTblIdx;
}


#endif