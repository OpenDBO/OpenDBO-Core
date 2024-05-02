#pragma once

#include "NtlSfx.h"
#include "NtlServer.h"

#include "PlayerManager.h"

#include "GameSessionFactory.h"
#include "SubNeighborServerInfoManager.h"

#include "DatabaseEnv.h"

//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//

class CPacketEvent;
class CPacketEventObj;

class CChatServer : public CNtlServerApp
{
public:

	CChatServer();
	virtual ~CChatServer();

	CNtlString		GetCharDatabaseHost()		{ return m_config.CharDatabaseHost; }
	UINT			GetCharDatabasePort()		{ return m_config.CharDatabasePort; }
	CNtlString		GetCharDatabaseUser()		{ return m_config.CharDatabaseUser; }
	CNtlString		GetCharDatabasePassword()	{ return m_config.CharDatabasePassword; }
	CNtlString		GetCharDatabaseName()		{ return m_config.CharDatabaseName; }

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

	virtual int				OnInitApp();
	virtual int				OnAppStart();
	virtual int				OnCreate();
	virtual int				OnCommandArgument(int argc, _TCHAR* argv[])	{	return NTL_SUCCESS;	}
	virtual BOOL			OnCommandInput(std::string& sCmd);


	int	OnConfiguration(const char * lpszConfigFile)
	{
		CNtlIniFile file;
		int rc = file.Create( lpszConfigFile );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}
		if (!file.Read("Chat Server", "Address", m_config.strClientAcceptAddr))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Chat Server", "PublicAddress", m_config.strPublicClientAcceptAddr))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Chat Server", "Port", m_config.wClientAcceptPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Chat Server", "server_accept_limit",  m_config.ServerAcceptLimit) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Chat Server", "Server_Accept_Address", m_config.strServerAcceptAddr) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Chat Server", "Server_Accept_Port",  m_config.wServerAcceptPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		//MASTER SERVER
		if (!file.Read("Master Connect", "Address", m_config.strMasterServerIP))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Master Connect", "Port", m_config.wMasterServerPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		//QUERY SERVER
		if (!file.Read("Query Connect", "Address", m_config.strQueryServerIP))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Query Connect", "Port", m_config.wQueryServerPort))
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

		// PERFORMANCE
		if (!file.Read("CONFIG", "MaxConnection", m_config.nMaxConnection))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		//TABLE
		if (!file.Read("TABLE", "LoadTableFormat", m_config.LoadTableFormat))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("TABLE", "Path", m_config.TablePath))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		return NTL_SUCCESS;
	}

	virtual void	Run();

private:

	CNtlAcceptor				m_clientAcceptor;
	CNtlAcceptor				m_serverAcceptor;
	CNtlConnector				m_serverConnector;
	CNtlConnector				m_serverQueryConnector;
	CNtlConnector				m_MasterServerConnector;

public:

	sCHAT_SERVERCONFIG			m_config;
	CNtlLog  					m_log;

private:

	CPacketEvent*				m_pPacketEvent;

public:

	void						PostClientPacketEvent(CPacketEventObj* pEvent);

private:

	CServerPassiveSession *		m_pGameServerSession;
	CMasterServerSession *		m_pMasterServerSession;
	CQueryServerSession *		m_pQueryServerSession;

public:



	CQueryServerSession* GetQueryServerSession() { return m_pQueryServerSession; }
	void SetQueryServerSession(CQueryServerSession * pServerSession)
	{
		m_pQueryServerSession = pServerSession;
	}

	CMasterServerSession* GetMasterServerSession() { return m_pMasterServerSession; }
	void SetMasterServerSession(CMasterServerSession * pServerSession)
	{
		m_pMasterServerSession = pServerSession;
	}

};