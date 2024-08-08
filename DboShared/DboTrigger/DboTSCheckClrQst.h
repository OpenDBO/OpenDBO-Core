#ifndef _DBO_TSCHECKCLRQST_H_
#define _DBO_TSCHECKCLRQST_H_


#include "DboTSCoreDefine.h"


/**
	Check cleared quest condition
	용도 : Client quest, client trigger, server quest, server trigger pc
	설명 : 이전에 진행한 퀘스트가 있는지 검사한다
*/


class CDboTSCheckClrQst : public CNtlTSCond
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	std::vector<NTL_TS_T_ID>			m_vecAndIdList;
	std::vector<NTL_TS_T_ID>			m_vecOrIdList;

// Constructions and Destructions
public:
	CDboTSCheckClrQst( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_COND_TYPE_ID_CHECK_CLRQST; }

	// And 및 Or 아이디 입력 시 "숫자;숫자;" 형태로 입력되어야 한다
	const std::vector<NTL_TS_T_ID>&		GetAndIdList( void ) const;
	bool								SetAndIdList( const std::string& strIDList );
	void								ClearAndIdList( void );

	const std::vector<NTL_TS_T_ID>&		GetOrIdList( void ) const;
	bool								SetOrIdList( const std::string& strIDList );
	void								ClearOrIdList( void );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline const std::vector<NTL_TS_T_ID>& CDboTSCheckClrQst::GetAndIdList( void ) const
{
	return m_vecAndIdList;
}

inline const std::vector<NTL_TS_T_ID>& CDboTSCheckClrQst::GetOrIdList( void ) const
{
	return m_vecOrIdList;
}


#endif