#ifndef _DBO_TSMAIN_H_
#define _DBO_TSMAIN_H_


#include "DboTSCoreDefine.h"


class CDboTSEntityFactory;
class CDboTSCtrlFactory;
class CDboTSUIFactory;
class CDboTSQAgency;
class CDboTSQRecv;
class CNtlUnzip;


/** 
	Trigger system main
	설명 : 트리거 시스템에서 전반적으로 사용되는 리소스를 관리하며
		   Trigger agency 에 대한 생성 및 소멸을 담당한다
*/


class CDboTSMain : public CNtlTSMain
{
	NTL_TS_DECLARE_RTTI

// Declarations
public:
	typedef stdext::hash_map<std::string, CNtlTSEvtMapper*> hashdef_EVT_MAPPER_LIST;

// Member variables
protected:
	// Log
	CNtlTSLog*							m_pLog;

	// Factories
	CDboTSEntityFactory*				m_pEntityFactory;
	CDboTSCtrlFactory*					m_pCtrlFactory;
	CDboTSUIFactory*					m_pUIFactory;

	// Event mapper
	hashdef_EVT_MAPPER_LIST				m_defEvtMapper;

// Constructions and Destructions
public:
	CDboTSMain( void );
	virtual ~CDboTSMain( void );

// Methods
public:
	virtual bool						Create( void );
	virtual void						Delete( void );

	CDboTSEntityFactory*				GetEntityFactory( void );
	CDboTSCtrlFactory*					GetControlFactory( void );
	CDboTSUIFactory*					GetUIFactory( void );

	hashdef_EVT_MAPPER_LIST&			GetEventMapper( void );
	CNtlTSEvtMapper*					FindEventMapper( const std::string& strMapper );

	void								DeleteAgency( CNtlTSAgency*& pTSAgency );

// Implementations
protected:
	virtual bool						LoadLog( void );
	virtual void						UnloadLog( void );

	virtual bool						LoadFactories( void );
	virtual void						UnloadFactories( void );

	virtual bool						LoadScripts( void );
	virtual void						UnloadAllScripts( void );

	virtual bool						LoadEventMappers( void );
	virtual void						UnloadEventMappers( void );

	virtual bool						LoadRecv( void );
	virtual void						UnloadRecv( void );

	// 지정된 폴더를 포함한 하위 폴더들에 대한 TS 파일( .t )들을 로딩한다
	bool								LoadTSPath( std::string strPath, mapdef_TLIST& defTList );
	// Zip 파일안의 TS 파일( .t )들을 로딩한다
	bool								LoadTSZip( std::string strFile, mapdef_TLIST& defTList );
	// 암호화된 Zip 파일안의 TS 파일( .t )들을 로딩한다
	bool								LoadTSCryptoData( std::string strFile, mapdef_TLIST& defTList );
	// 암호화된 파일안의 Zip 파일을 로딩한다
	bool								LoadTSCryptoData_UnZip( std::string strFile, CNtlUnzip* pclUnzip );

	bool								LoadTriggerObject( const std::string& strPath, const char* pFileName, mapdef_TLIST& defTList );
	bool								LoadTriggerObject( const std::string& strPath, const char* pFileName, char* pBuff, int nSize, mapdef_TLIST& defTList );
	bool								LoadTriggerObjectFromUnZip( NTL_TS_T_ID tID, CNtlUnzip* pclUnzip, mapdef_TLIST& defTList );
};


inline CDboTSEntityFactory* CDboTSMain::GetEntityFactory( void )
{
	return m_pEntityFactory;
}

inline CDboTSCtrlFactory* CDboTSMain::GetControlFactory( void )
{
	return m_pCtrlFactory;
}

inline CDboTSUIFactory* CDboTSMain::GetUIFactory( void )
{
	return m_pUIFactory;
}


#endif