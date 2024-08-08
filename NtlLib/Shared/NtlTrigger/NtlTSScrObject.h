#ifndef _NTL_TSSCROBJECT_H_
#define _NTL_TSSCROBJECT_H_


#include "NtlTSRTTI.h"
#include "NtlTSScrProperty.h"


/** 
	Script object
	Script 를 통해서 객체를 자동생성하기 위해서는 실제 생성하고자 하는 클래스는 아래 클래스를
	상속받아 구현해야 함

	( 주의 )
	스크립트에서 런타임 시 객체 생성을 지원하기 위해서는 상속 받는 클래스에서 TSRTTI 를 사용해야
	런타임 시 객체를 동적 생성할 수 있음
*/


class CNtlTSScrObject
{
	NTL_TS_DECLARE_RTTI

// Constructions and Destructions
public:
	virtual ~CNtlTSScrObject( void ) { return; }

// Implementations
protected:
	virtual	CNtlTSScrObject*			GetParentForScript( void ) = 0;
	virtual	void						SetParentForScript( CNtlTSScrObject* pParent ) = 0;

	virtual	CNtlTSScrObject*			BeginChildForScript( void ) = 0;
	virtual	CNtlTSScrObject*			NextChildForScript( void ) = 0;

	virtual	void						AttachChildForScript( CNtlTSScrObject* pChild ) = 0;

public:
	virtual	void						ApplyScriptDataForScript( const CNtlTSScrProperty& clProperty ) = 0;
	virtual	void						TakeScriptDataForScript( CNtlTSScrProperty& clProperty ) = 0;

public:
	virtual void						MakeAllObjectList( std::vector<CNtlTSScrObject*>& defObjList ) = 0;

	friend class CNtlTSScrParser;
	friend class CNtlTSScrSaver;
};


#endif