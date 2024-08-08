#ifndef _NTL_TSTRIGGER_H_
#define _NTL_TSTRIGGER_H_


#include "NtlTSEntity.h"


class CNtlTSGroup;


/**
	Trigger entity
*/


class CNtlTSTrigger : public CNtlTSEntity
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef stdext::hash_map<NTL_TS_TG_ID, CNtlTSGroup*> hashdef_GroupList;

// Member variables
protected:
	NTL_TS_T_ID							m_tID;

	bool								m_bCanRepeat;
	bool								m_bCanShare;
	bool								m_bOutStateMsg;

	hashdef_GroupList					m_GroupList;
	hashdef_GroupList::iterator			m_itGroupList;

	unsigned int						m_uiTitle;		// 트리거의 제목

// Constructions and Destructions
public:
	CNtlTSTrigger( void );
	virtual ~CNtlTSTrigger( void );

// Methods
public:
	virtual unsigned int				GetEntityType( void ) const { return NTL_TSENTITY_TYPE_TRIGGER_BEGIN; }

	// Trigger id
	NTL_TS_T_ID							GetID( void ) const;
	NTL_TS_T_ID							SetID( NTL_TS_T_ID tID );

	// 반복 퀘스트
	bool								IsRepeatQuest( void ) const;
	void								SetRepeatQuest( bool bCanRepeat );

	// 공유 가능 퀘스트
	bool								IsShareQuest( void ) const;
	void								SetShareQuest( bool bCanShare );

	// 상태 메시지 출력
	bool								IsOutStateMsg( void ) const;
	void								SetOutStateMsg( bool bOutStateMsg );

	// Group
	CNtlTSGroup*						GetGroup( NTL_TS_TG_ID tgId );
	void								AttachGroup( CNtlTSGroup* pGroup );

	// Title
	unsigned int						GetTitle( void ) const;
	void								SetTitle( unsigned int uiTitle );

	// 자신 및 자식 Entity들에 대한 탐색 및 실행
	virtual	NTL_TSRESULT				SearchAll( CNtlTSRecv* pTSRecv, void* pParam );
	virtual	NTL_TSRESULT				SearchTarget( NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam );

	NTL_TSRESULT						RunTarget( NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CNtlTSRecv* pTSRecv, void* pParam );

// Implementations
protected:
	// 스크립트에서 사용되는 자식 관련 함수들
	virtual	CNtlTSScrObject*			BeginChildForScript( void );
	virtual	CNtlTSScrObject*			NextChildForScript( void );
	virtual	void						AttachChildForScript( CNtlTSScrObject* pChild );

	// 스크립트로 부터 컴포넌트 객체의 데이터를 로딩 및 입력 시키기 위한 함수
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty );
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty );

public:
	virtual void						MakeAllObjectList( std::vector<CNtlTSScrObject*>& defObjList );
};


inline NTL_TS_T_ID CNtlTSTrigger::GetID( void ) const
{
	return m_tID;
}

inline bool CNtlTSTrigger::IsShareQuest( void ) const
{
	return m_bCanShare;
}

inline bool CNtlTSTrigger::IsRepeatQuest( void ) const
{
	return m_bCanRepeat;
}

inline bool CNtlTSTrigger::IsOutStateMsg( void ) const
{
	return m_bOutStateMsg;
}

inline unsigned int CNtlTSTrigger::GetTitle( void ) const
{
	return m_uiTitle;
}


#endif