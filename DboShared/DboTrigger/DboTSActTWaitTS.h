#ifndef _DBO_TSACTTWAITTS_H_
#define _DBO_TSACTTWAITTS_H_


#include "DboTSCoreDefine.h"


/**
	Time wait TS action
	용도 : 모두
	설명 : TSP( Trigger system pointer )의 진행을 주어진 시간만큼 중단한다
*/


class CDboTSActTWaitTS : public CNtlTSAction
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiWTime;	// 초단위 대기 시간

// Constructions
public:
	CDboTSActTWaitTS( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_ACT_TYPE_ID_ACT_TWAITTS; }

	unsigned int						GetWaitTime( void ) const;
	void								SetWaitTime( unsigned int uiWTime );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSActTWaitTS::GetWaitTime( void ) const
{
	return m_uiWTime;
}


#endif