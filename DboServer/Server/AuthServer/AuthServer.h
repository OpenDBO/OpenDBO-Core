#pragma once

#include "NtlSfx.h"

#include "NtlSharedType.h"

#include "SessionFactory.h"
#include "SubNeighborServerInfoManager.h"

#include "DatabaseEnv.h"

#include <unordered_map>


struct sSERVERCONFIG
{
	CNtlString		strClientAcceptAddr;
	WORD			wClientAcceptPort;
	SERVERINDEX		byServerId;

	CNtlString		AccDatabaseHost;
	UINT			AccDatabasePort;
	CNtlString		AccDatabaseUser;
	CNtlString		AccDatabasePassword;
	CNtlString		AccDatabaseName;

	CNtlString		LogDatabaseHost;
	UINT			LogDatabasePort;
	CNtlString		LogDatabaseUser;
	CNtlString		LogDatabasePassword;
	CNtlString		LogDatabaseName;

	CNtlString		strMasterServerIP;
	WORD			wMasterServerPort;

	BOOL			bDisableConnection;
	int				nMaxConnection;
	BOOL			bAllowFounderConnection;
};



class CAuthServer : public CNtlServerApp
{

public:

	CAuthServer();
	virtual ~CAuthServer();

public:

	CNtlString		GetAccDatabaseHost()		{ return m_config.AccDatabaseHost; }
	UINT			GetAccDatabasePort()		{ return m_config.AccDatabasePort; }
	CNtlString		GetAccDatabaseUser()		{ return m_config.AccDatabaseUser; }
	CNtlString		GetAccDatabasePassword()	{ return m_config.AccDatabasePassword; }
	CNtlString		GetAccDatabaseName()		{ return m_config.AccDatabaseName; }

	CNtlString		GetLogDatabaseHost()		{ return m_config.LogDatabaseHost; }
	UINT			GetLogDatabasePort()		{ return m_config.LogDatabasePort; }
	CNtlString		GetLogDatabaseUser()		{ return m_config.LogDatabaseUser; }
	CNtlString		GetLogDatabasePassword()	{ return m_config.LogDatabasePassword; }
	CNtlString		GetLogDatabaseName()		{ return m_config.LogDatabaseName; }

	BOOL			GetDisableConnection()	{ return m_config.bDisableConnection; }
	BOOL			GetFounderConnection()	{ return m_config.bAllowFounderConnection; }

	virtual int		OnInitApp();
	virtual int		OnCreate();
	virtual void	OnDestroy();
	virtual int		OnAppStart();

	virtual int		OnConfiguration(const char * lpszConfigFile)
	{
		CNtlIniFile file;

		int rc = file.Create( lpszConfigFile );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}
		if( !file.Read("Auth Server", "Address", m_config.strClientAcceptAddr) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Auth Server", "Port",  m_config.wClientAcceptPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Auth Server", "ServerID",  m_config.byServerId) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		// DATABASE_ACCOUNT
		if (!file.Read("DATABASE_ACCOUNT", "Host", m_config.AccDatabaseHost))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_ACCOUNT", "Port", m_config.AccDatabasePort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_ACCOUNT", "User", m_config.AccDatabaseUser))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_ACCOUNT", "Password", m_config.AccDatabasePassword))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_ACCOUNT", "DbName", m_config.AccDatabaseName))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		// DATABASE_LOG
		if (!file.Read("DATABASE_LOG", "Host", m_config.LogDatabaseHost))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_LOG", "Port", m_config.LogDatabasePort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_LOG", "User", m_config.LogDatabaseUser))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_LOG", "Password", m_config.LogDatabasePassword))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_LOG", "DbName", m_config.LogDatabaseName))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		//master server
		if( !file.Read("Master Connect", "Address", m_config.strMasterServerIP) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Master Connect", "Port",  m_config.wMasterServerPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		// CONFIG
		if( !file.Read("CONFIG", "DisableConnection",  m_config.bDisableConnection) )
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		if (!file.Read("CONFIG", "MaxConnection", m_config.nMaxConnection))
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		if (!file.Read("CONFIG", "FounderConnection", m_config.bAllowFounderConnection))
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;


		return NTL_SUCCESS;
	}


	virtual void	Run();

	virtual BOOL	OnCommandInput(std::string& sCmd);

private:

	CNtlAcceptor*				m_pClientAcceptor;
	CNtlConnector*				m_pServerConnector;

public:

	sSERVERCONFIG				m_config;
	CNtlLog  					m_log;
	CSubNeighborServerInfoManager	*m_pNeighborServerInfoManager;

public:

	CMasterServerSession*		m_pMasterServerSession;

	void	SetMasterServerSession(CMasterServerSession * pServerSession)
	{
		m_pMasterServerSession = pServerSession;
	}


	HOBJECT						GetMasterServerHandle() { return m_pMasterServerSession->GetHandle(); }


private:

	std::unordered_map<ACCOUNTID, CClientSession*> m_map_Players;

public:

	bool			AddPlayer(ACCOUNTID AccID, CClientSession* session);
	void			DelPlayer(ACCOUNTID AccID);
	CClientSession*	FindPlayer(ACCOUNTID AccID);


public:

	bool			IsAccountTempBlocked(const char* strUsername);
	void			RegisterAccountTempBann(const char* strUsername);

private:

	std::map<std::string, QWORD>	m_mapBlockedAccounts;

};