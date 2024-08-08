#ifndef _DBO_TSACTSWPROBSF_H_
#define _DBO_TSACTSWPROBSF_H_


#include "DboTSCoreDefine.h"


/**
	Switch probability success fail
*/


class CDboTSActSWProbSF : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	float								m_fProbility;	// 확률 ( 0 ~ 1 ) 사이
	unsigned int						m_uiSuccessBID;	// 성공시 분기 ID
	unsigned int						m_uiFailBID;	// 실패시 분기 ID

// Constructions
public:
	CDboTSActSWProbSF( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_SWPROBSF; }

	float								GetProbility( void ) const;
	void								SetProbility( float fProbility );

	unsigned int						GetSuccessBID( void ) const;
	void								SetSuccessBID( unsigned int uiBID );

	unsigned int						GetFailBID( void ) const;
	void								SetFailBID( unsigned int uiBID );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline float CDboTSActSWProbSF::GetProbility( void ) const
{
	return m_fProbility;
}


inline unsigned int CDboTSActSWProbSF::GetSuccessBID( void ) const
{
	return m_uiSuccessBID;
}


inline unsigned int CDboTSActSWProbSF::GetFailBID( void ) const
{
	return m_uiFailBID;
}


#endif