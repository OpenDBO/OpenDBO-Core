#include "StdAfx.h"
#include "NtlClientService.h"


//////////////////////////////////////////////////////////////////////////
//
// CNtlSecSol
//
//////////////////////////////////////////////////////////////////////////


void CNtlSecSol::PushEvent( unsigned int uiEvent, unsigned int uiData )
{
	sSEC_EVENT sEvent;
	sEvent.uiEvent = uiEvent;
	sEvent.uiData = uiData;

	m_listEvent.push_back( sEvent );
}

bool CNtlSecSol::PopEvent( unsigned int& uiEvent, unsigned int& uiData )
{
	if ( m_listEvent.empty() )
	{
		return false;
	}

	uiEvent = m_listEvent.front().uiEvent;
	uiData = m_listEvent.front().uiData;

	m_listEvent.pop_front();

	return true;
}


//////////////////////////////////////////////////////////////////////////
//
// CNtlService_CTTW
//
//////////////////////////////////////////////////////////////////////////


bool CNtlService_CTTW::CreateSecSol( void )
{
	if ( m_pSecSol )
	{
		return true;
	}

	// Retail: new CNtlSecSol_NProtect (nProtect GameGuard, "DragonBallKR").
	m_pSecSol = new CNtlSecSol;

	return m_pSecSol->Create();
}

void CNtlService_CTTW::DeleteSecSol( void )
{
	if ( m_pSecSol )
	{
		m_pSecSol->Delete();

		delete m_pSecSol;
		m_pSecSol = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
//
// GetServiceInterface
//
//////////////////////////////////////////////////////////////////////////


INtlClientService* GetServiceInterface( void )
{
	static CNtlService_CTTW s_clService;

	return &s_clService;
}
