#ifndef _DBO_TSCLICKNPC_H_
#define _DBO_TSCLICKNPC_H_


#include "DboTSCoreDefine.h"


/**
	Click NPC event
	용도 : Client quest, client trigger, server quest, server trigger pc
	설명 : 클라이언트에서는 NPC가 선택되었을 때 발생하는 이벤터로써 사용됨
		   서버에서는 해당 NPC가 선택되었는지 검사하는 용도로 사용됨
*/


class CDboTSClickNPC : public CNtlTSEvent
{
	NTL_TS_DECLARE_RTTI

// Member variables
protected:
	unsigned int						m_uiNPCIdx;

// Constructions
public:
	CDboTSClickNPC( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return DBO_EVENT_TYPE_ID_CLICK_NPC; }

	unsigned int						GetNPCIdx( void ) const;
	void								SetNPCIdx( unsigned int uiNPCIdx );

// Implementations
protected:
	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );
};


inline unsigned int CDboTSClickNPC::GetNPCIdx( void ) const
{
	return m_uiNPCIdx;
}


#endif