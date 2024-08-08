#ifndef _NTL_NAVI_H_
#define _NTL_NAVI_H_


#include "NtlNaviDefine.h"

//////////////////////////////////////////////////////////////////////////
//
// 길찾기 로고 시스템
//
//////////////////////////////////////////////////////////////////////////


class INtlNaviLog
{
public:
	virtual ~INtlNaviLog( void ) { return; }


public:
	virtual void Log( const char* pLog, ... )														= 0;
};


//////////////////////////////////////////////////////////////////////////
//
// 길찾기 인터페이스
//
//////////////////////////////////////////////////////////////////////////

class CNtlNaviPEWorld;
class iMesh;

class INtlNavi
{
public:
	virtual ~INtlNavi( void ) { return; }


public:
	//////////////////////////////////////////////////////////////////////////
	//
	// Navigation library 구동 관련 인터페이스
	//
	//////////////////////////////////////////////////////////////////////////

	virtual bool					Create( INtlNaviLog* pLog,
											const char* pPathDllName,
											bool bThreadLoading )									= 0;

	virtual void					Delete( void )													= 0;


	//////////////////////////////////////////////////////////////////////////
	//
	// Tool 전용 인터페이스
	//
	//////////////////////////////////////////////////////////////////////////

	virtual bool					ImportWorldToolData( const char* pRootFolder )					= 0;

	virtual void					GetListImportedWorldIDList( vecdef_WorldIDList& vecOut )		= 0;

	virtual bool					ExportPathEngineData( const char* pRootFolder )					= 0;

	virtual bool					ExportPathEngineDataSelect( const char* pRootFolder, mapdef_ExportList& list ) = 0;

	virtual bool					UpdateToolData( void )											= 0;

	virtual int						GetWorldVertexCount( NAVI_INST_HANDLE hHandle )					= 0;

	virtual int						GetWorldVertexCount( unsigned int uiWorldID )					= 0;

	virtual bool					GetWorldVertexToBuffer( NAVI_INST_HANDLE hHandle, sNAVI_VERTEX* pBuffer )		= 0;

	virtual bool					GetWorldVertexToBuffer( unsigned int uiWorldID, sNAVI_VERTEX* pBuffer )			= 0;

	virtual iMesh*					GetNearestMesh( NAVI_INST_HANDLE hHandle, sNAVI_POS& vPos )						= 0;


	//////////////////////////////////////////////////////////////////////////
	//
	// Game 전용 인터페이스
	//
	//////////////////////////////////////////////////////////////////////////

	// 패스 엔진 데이터를 로딩한다
	virtual bool					LoadPathEngineData( const char* pRootFolder,
														vecdef_WorldIDList& defWorldIDList,
														unsigned char byLoadFlags )					= 0;

	// 로딩이 완료되는 시점까지 Main-Thread 를 Block 한다
	// 로딩 완료 전에 아래 존재하는 함수들을 사용하는 것에 대한 결과는 책임 지지 않는다
	virtual bool					WaitUntilLoadingFinish( unsigned int uiWaitTime,
															unsigned int& uiResult )				= 0;

	// 로딩 시간 반환
	// 반환값 -> 0xffffffff				: 로딩 실패
	//			 0xffffffff 이외의 값	: 로딩 시간
	virtual unsigned int			GetLoadingTime( void )											= 0;

	// 현재 패스엔진이 사용하고 있는 메모리 양
	virtual unsigned long			GetTotalMemory( void )											= 0;

	// Instance handler 생성
	virtual NAVI_INST_HANDLE		CreateInstanceHandler( unsigned int uiWorldID )					= 0;

	// Instance handler 제거
	virtual void					DeleteInstanceHandler( NAVI_INST_HANDLE hHandle )				= 0;

	// 속성 데이터가 존재하는지를 검사한다
	virtual bool					IsPropDataLoaded( unsigned int uiWorldID )						= 0;

	// 패스 데이터가 존재하는지를 검사한다
	virtual bool					IsPathDataLoaded( unsigned int uiWorldID )						= 0;

	// NPC, Mob의 크기 ( 반경 ) 지원 여부
	virtual bool					CanSupportAgent( NAVI_INST_HANDLE hHandle, float fAgentRadius )	= 0;

	// 맵
	virtual unsigned int			GetTextAllIndex( NAVI_INST_HANDLE hHandle,
													 float x,
													 float z )										= 0;

	// 존
	virtual unsigned int			GetZoneIndex( NAVI_INST_HANDLE hHandle,
												  float x,
												  float z )											= 0;

	// 해당 위치의 모든 속성
	virtual unsigned int			GetAttribute( NAVI_INST_HANDLE hHandle,
												  float x,
												  float z )											= 0;

	// 기본 속성의 존재 유무
	virtual bool					IsBasicAttributeSet( NAVI_INST_HANDLE hHandle,
														 float x,
														 float z,
														 eDBO_WORLD_ATTR_BASIC eAttrBasic )			= 0;

	// 반환 값이 NAVI_FLT_MAX 이면 Invalid
	virtual float					GetHeight( NAVI_INST_HANDLE hHandle,
											   float x,
											   float y,
											   float z )											= 0;

	// 반환 값이 NAVI_FLT_MAX 이면 Invalid
	virtual float					GetGuaranteedHeight( NAVI_INST_HANDLE hHandle,
														 float x,
														 float y,
														 float z )									= 0;


	// 가장 근접한 곳의 위치 반환
	virtual bool					FindNearestPos( NAVI_INST_HANDLE hHandle,
													float fAgentRadius,
													sNAVI_POS& sSrcPos )							= 0;

	// 충돌 테스트
	virtual eCOL_TEST_RESULT		CollisionTest( NAVI_INST_HANDLE hHandle,
												   float fAgentRadius,
												   sNAVI_POS& sSrcPos,
												   sNAVI_POS& sDestPos )							= 0;

	// 충돌 테스트 : 첫번째 충돌 위치를 출력
	virtual eCOL_TEST_RESULT		FirstCollisionTest( NAVI_INST_HANDLE hHandle,
													float fAgentRadius, 
													sNAVI_POS& sSrcPos, 
													sNAVI_POS& sDestPos, 
													sNAVI_POS& sFirstCollisionPos )					= 0;

	// 길 찾기
	virtual bool					FindPath( NAVI_INST_HANDLE hHandle,
											  float fAgentRadius,
											  sNAVI_POS& sSrcPos,
											  sNAVI_POS& sDestPos,
											  vecdef_NaviPosList& defNaviPosList )					= 0;
};


// 현재 생성된 길찾기 인터페이스 얻기
INtlNavi* GetNaviEngine( void );


#endif