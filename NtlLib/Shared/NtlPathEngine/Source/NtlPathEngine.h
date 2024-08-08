/*****************************************************************************
* File			: NtlPathEngine.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2009. 03. 10
* Abstract		: Pathengine wrapping project ( for client )
*****************************************************************************
* Desc:
* 
* 이 프로젝트는 현재 사용하고 있는 PathEngine의 Client 확장을 위해서 생성된
* 정적 라이브러리 프로젝트이다. Tool 쪽의 TestPathEngine 프로젝트와 연동되어서 사용된다.
* 
* Note1 : 모든 작업이 안정화가 되면 이 프로젝트는 NtlNavi 프로젝트와 통합할 것
* Note2 : NtlNavi 와 통합 작업이 끝나면 Tool 쪽의 TestPathEngine 프로젝트는 삭제할 것
* Note3 : 클라이언트의 Pathengine R&D 결과로 SKIP ( 2009. 04 )
* 
* Worklist
* (↓) 1차 : Library interface 생성
* (↓) 2차 : PathEngine data create module 생성  ( Federation tile 생성 )
* (↓) 3차 : PathEngine data seamless loading module 생성
* (★) 4차 : Client 와 Library 와의 연동
* (  ) 5차 : 안정화 작업
*****************************************************************************/

#ifndef _NTL_PATHENGINE_H_
#define _NTL_PATHENGINE_H_

#pragma once

#include <Windows.h>

class iPathEngine;
class iErrorHandler;
class CNtlPathEngineLog;

#include "NtlPathSeamlessManager.h"

class CNtlPathEngine
{
public:
	CNtlPathEngine(void);
	~CNtlPathEngine(void);

	typedef iPathEngine* (__stdcall* tGetInterfaceFunction)( iErrorHandler* );

	// SingleTon
	static CNtlPathEngine*	GetInstance();

	virtual bool	Create( const char* pPathEngineDllName, BOOL bSeamless = TRUE );
	virtual void	Update( float fElapsed );
	virtual void	Destroy( void );

	iPathEngine*	GetPathEngine();

	// Seamless
	void	SeamlessAddAgentSize( int nSize );

	void	SeamlessChangeWorld( int nWorldIndex );
	void	SeamlessUpdatePosition( int nPositionX, int nPositionY, int nPositionZ );

	void	SeamlessPathFinding( 
		int nAgentSize, int nSrcPositionX, int nSrcPositionY, int nSrcPositionZ,
		int nDestPositionX, int nDestPositionY, int nDestPositionZ, std::list< cPosition >& vec );


protected:
	HINSTANCE		m_hInstance;
	iPathEngine*	m_pPathEngine;

	CNtlPathSeamlessManager			m_seamManager;
	BOOL							m_bSeamless;

	// Currently Position
	int								m_nPositionX;
	int								m_nPositionY;
	int								m_nPositionZ;
};

static CNtlPathEngine* GetNtlPathEngine()
{
	return CNtlPathEngine::GetInstance();
}

#endif