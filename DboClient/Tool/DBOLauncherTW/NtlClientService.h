/*****************************************************************************
*
* File			: NtlClientService.h
* Abstract		: Publisher/service abstraction of the CT_TW launcher build.
*                 The retail build links CNtlService_CTTW (publisher id 3),
*                 whose security solution is nProtect GameGuard
*                 (CNtlSecSol_NProtect, game code "DragonBallKR").
*
*                 The launcher itself only uses Begin/End/GetPublisherID; the
*                 GameGuard part is shared with the client and never created
*                 here. The nProtect SDK is not available, so CNtlSecSol is
*                 provided as the retail no-op base class only.
*
*****************************************************************************/


#pragma once


enum eDBO_SERVICE_PUBLISHER
{
	eDBO_SERVICE_PUBLISHER_NTL = 0,
	eDBO_SERVICE_PUBLISHER_CJI_KOR,
	eDBO_SERVICE_PUBLISHER_JPN,
	eDBO_SERVICE_PUBLISHER_CT_TW,
	eDBO_SERVICE_PUBLISHER_CT_HK,
};


//////////////////////////////////////////////////////////////////////////
//
// CNtlSecSol
//
// Security solution base. Keeps the GameGuard callback events in a queue.
//
//////////////////////////////////////////////////////////////////////////


class CNtlSecSol
{
public:
	struct sSEC_EVENT
	{
		unsigned int		uiEvent;
		unsigned int		uiData;
	};

protected:
	std::list< sSEC_EVENT >	m_listEvent;
	unsigned int			m_uiSeq;

public:
	CNtlSecSol( void ) : m_uiSeq( 0 )	{ return; }
	virtual ~CNtlSecSol( void )			{ return; }

public:
	virtual bool			Create( void )								{ return true; }
	virtual void			Delete( void )								{ return; }
	virtual void			SetHWnd( HWND hWnd )						{ return; }
	virtual void			SendUserID( const char* pszUserID )			{ return; }
	virtual void			Check( void )								{ return; }
	virtual unsigned int	GetSeq( void )								{ return m_uiSeq; }
	virtual void			IncSeq( void )								{ ++m_uiSeq; }
	virtual void			Auth( const void* pData, unsigned int uiSize )	{ return; }

	virtual bool			IsEventEmpty( void )						{ return m_listEvent.empty(); }
	virtual void			PushEvent( unsigned int uiEvent, unsigned int uiData );
	virtual bool			PopEvent( unsigned int& uiEvent, unsigned int& uiData );
};


//////////////////////////////////////////////////////////////////////////
//
// INtlClientService
//
// Virtual layout follows the retail vtable of CNtlService_CTTW.
//
//////////////////////////////////////////////////////////////////////////


class INtlClientService
{
public:
	virtual ~INtlClientService( void ) {}

	virtual bool			Begin( const char* szServiceCmd ) = 0;
	virtual void			End( void ) = 0;
	virtual void			SetMainWnd( HWND hWnd ) = 0;
	virtual unsigned int	GetPublisherID( void ) = 0;
	virtual bool			IsServiceOption( unsigned int uiOption ) = 0;

	virtual CNtlSecSol*		GetSecSol( void ) = 0;
	virtual bool			CreateSecSol( void ) = 0;
	virtual bool			CreateSecSolEx( void ) = 0;
	virtual void			DeleteSecSol( void ) = 0;
	virtual bool			IsSecSolCreated( void ) = 0;

	virtual void			SecSol_SetHWnd( HWND hWnd ) = 0;
	virtual void			SecSol_SendUserID( const char* pszUserID ) = 0;
	virtual void			SecSol_Check( void ) = 0;
	virtual unsigned int	SecSol_GetSeq( void ) = 0;
	virtual void			SecSol_IncSeq( void ) = 0;
	virtual void			SecSol_Auth( const void* pData, unsigned int uiSize ) = 0;
	virtual bool			SecSol_IsEventEmpty( void ) = 0;
	virtual void			SecSol_PushEvent( unsigned int uiEvent, unsigned int uiData ) = 0;
	virtual bool			SecSol_PopEvent( unsigned int& uiEvent, unsigned int& uiData ) = 0;

	virtual bool			OnClientCmd( const char* pszCmd, unsigned int uiCmdSize, void* pResult, unsigned int uiResultSize ) = 0;
	virtual void			OnClientTick( void ) = 0;
	virtual void			OnClientEvent( unsigned int uiEvent, unsigned int uiData ) = 0;
	virtual void			OnClientState( unsigned int uiState ) = 0;
	virtual void			OnClientNotify( unsigned int uiNotify, unsigned int uiData1, unsigned int uiData2 ) = 0;
	virtual bool			IsUsableFunction( unsigned int uiFunc, unsigned int uiData ) = 0;
	virtual bool			IsUsableFunctionEx( unsigned int uiFunc, unsigned int uiData ) = 0;
};


//////////////////////////////////////////////////////////////////////////
//
// CNtlService_CTTW
//
//////////////////////////////////////////////////////////////////////////


class CNtlService_CTTW : public INtlClientService
{
protected:
	CNtlSecSol*				m_pSecSol;

public:
	CNtlService_CTTW( void ) : m_pSecSol( NULL ) { return; }
	virtual ~CNtlService_CTTW( void ) { DeleteSecSol(); }

public:
	virtual bool			Begin( const char* szServiceCmd )	{ return true; }
	virtual void			End( void )							{ return; }
	virtual void			SetMainWnd( HWND hWnd )				{ return; }
	virtual unsigned int	GetPublisherID( void )				{ return eDBO_SERVICE_PUBLISHER_CT_TW; }
	virtual bool			IsServiceOption( unsigned int )		{ return false; }

	virtual CNtlSecSol*		GetSecSol( void )					{ return m_pSecSol; }
	virtual bool			CreateSecSol( void );
	virtual bool			CreateSecSolEx( void )				{ return CreateSecSol(); }
	virtual void			DeleteSecSol( void );
	virtual bool			IsSecSolCreated( void )				{ return NULL != m_pSecSol; }

	virtual void			SecSol_SetHWnd( HWND hWnd )										{ m_pSecSol->SetHWnd( hWnd ); }
	virtual void			SecSol_SendUserID( const char* pszUserID )						{ m_pSecSol->SendUserID( pszUserID ); }
	virtual void			SecSol_Check( void )											{ m_pSecSol->Check(); }
	virtual unsigned int	SecSol_GetSeq( void )											{ return m_pSecSol->GetSeq(); }
	virtual void			SecSol_IncSeq( void )											{ m_pSecSol->IncSeq(); }
	virtual void			SecSol_Auth( const void* pData, unsigned int uiSize )			{ m_pSecSol->Auth( pData, uiSize ); }
	virtual bool			SecSol_IsEventEmpty( void )										{ return m_pSecSol->IsEventEmpty(); }
	virtual void			SecSol_PushEvent( unsigned int uiEvent, unsigned int uiData )	{ m_pSecSol->PushEvent( uiEvent, uiData ); }
	virtual bool			SecSol_PopEvent( unsigned int& uiEvent, unsigned int& uiData )	{ return m_pSecSol->PopEvent( uiEvent, uiData ); }

	virtual bool			OnClientCmd( const char*, unsigned int, void*, unsigned int )	{ return true; }
	virtual void			OnClientTick( void )											{ return; }
	virtual void			OnClientEvent( unsigned int, unsigned int )						{ return; }
	virtual void			OnClientState( unsigned int )									{ return; }
	virtual void			OnClientNotify( unsigned int, unsigned int, unsigned int )		{ return; }
	virtual bool			IsUsableFunction( unsigned int, unsigned int )					{ return true; }
	virtual bool			IsUsableFunctionEx( unsigned int, unsigned int )				{ return true; }
};


INtlClientService* GetServiceInterface( void );
