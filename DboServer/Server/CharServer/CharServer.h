#pragma once

#include "NtlSfx.h"
#include "NtlServer.h"
#include "TableContainer.h"
#include "GameSessionFactory.h"
#include "NtlSharedType.h"
#include "PlayerManager.h"
#include "QueueManager.h"
#include "SubNeighborServerInfoManager.h"
#include "DatabaseEnv.h"


//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//

class CPacketEvent;
class CPacketEventObj;

class CCharServer : public CNtlServerApp
{
public:

	CCharServer();
	virtual ~CCharServer();


	CNtlString		GetCharDatabaseHost() { return m_config.CharDatabaseHost; }
	UINT			GetCharDatabasePort() { return m_config.CharDatabasePort; }
	CNtlString		GetCharDatabaseUser() { return m_config.CharDatabaseUser; }
	CNtlString		GetCharDatabasePassword() { return m_config.CharDatabasePassword; }
	CNtlString		GetCharDatabaseName() { return m_config.CharDatabaseName; }

	CNtlString		GetAccDatabaseHost() { return m_config.AccDatabaseHost; }
	UINT			GetAccDatabasePort() { return m_config.AccDatabasePort; }
	CNtlString		GetAccDatabaseUser() { return m_config.AccDatabaseUser; }
	CNtlString		GetAccDatabasePassword() { return m_config.AccDatabasePassword; }
	CNtlString		GetAccDatabaseName() { return m_config.AccDatabaseName; }

	SERVERINDEX		GetServerIndex() { return m_config.byServerID; }

	virtual int				OnInitApp();
	virtual int				OnAppStart();
	virtual int				OnCreate();
	virtual void			OnDestroy();

	bool			CreateTableContainer(int LoadTableFormat);
	bool			LoadLocalizedTable();

	int	OnConfiguration(const char * lpszConfigFile)
	{
		CNtlIniFile file;
		int rc = file.Create( lpszConfigFile );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}
		if( !file.Read("Char Server", "Address", m_config.strClientAcceptAddr) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Char Server", "PublicAddress", m_config.strPublicClientAcceptAddr))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Char Server", "Port",  m_config.wClientAcceptPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Char Server", "ServerID",  m_config.byServerID) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		//CONNECT QUERY SERVER
		if (!file.Read("Query Connect", "Address", m_config.strQueryServerIP))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Query Connect", "Port", m_config.wQueryServerPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		//CONNECT MASTER SERVER
		if( !file.Read("Master Connect", "Address", m_config.strMasterServerIP) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Master Connect", "Port",  m_config.wMasterServerPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		// DATABASE_CHARACTER
		if (!file.Read("DATABASE_CHARACTER", "Host", m_config.CharDatabaseHost))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_CHARACTER", "Port", m_config.CharDatabasePort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_CHARACTER", "User", m_config.CharDatabaseUser))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_CHARACTER", "Password", m_config.CharDatabasePassword))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("DATABASE_CHARACTER", "DbName", m_config.CharDatabaseName))
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

		// TABLES
		if( !file.Read("TABLE", "LoadTableFormat",  m_config.LoadTableFormat) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("TABLE", "Path",  m_config.TablePath) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		// CONFIG
		if (!file.Read("CONFIG", "MaxConnection", m_config.nMaxConnection))
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;


		return NTL_SUCCESS;
	}

	virtual BOOL			OnCommandInput(std::string& sCmd);

	virtual void	Run();

private:

	CPacketEvent*				m_pPacketEvent;

public:

	void						PostClientPacketEvent(CPacketEventObj* pEvent);

private:

	CNtlAcceptor				m_clientAcceptor;
	CNtlConnector				m_MasterServerConnector;
	CNtlConnector				m_QueryServerConnector;

public:
	sCHAR_SERVERCONFIG			m_config;
	CNtlLog  					m_log;
	CSubNeighborServerInfoManager*	m_pNeighborServerInfoManager;
	CTableContainer*			g_pTableContainer;
	CMasterServerSession*		m_pMasterServerSession;

private:

	CQueryServerSession*		m_pQueryServerSession;

public:

	void	SetMasterServerSession(CMasterServerSession * pServerSession)
	{
		m_pMasterServerSession = pServerSession;
	}

	void	SetQueryServerSession(CQueryServerSession * pServerSession)
	{
		m_pQueryServerSession = pServerSession;
	}

	CQueryServerSession*	GetQueryServerSession() { return m_pQueryServerSession; }

};