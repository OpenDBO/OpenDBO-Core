#pragma once

#include "NtlSfx.h"

#include "Servermanager.h"
#include "SessionFactory.h"
#include "NtlSharedType.h"

#include "DatabaseEnv.h"


struct sSERVERCONFIG
{
	CNtlString		strCharServerAcceptAddr;
	WORD			wCharServerAcceptPort;

	CNtlString		strChatServerAcceptAddr;
	WORD			wChatServerAcceptPort;

	CNtlString		strGameServerAcceptAddr;
	WORD			wGameServerAcceptPort;

	CNtlString		CharDatabaseHost;
	UINT			CharDatabasePort;
	CNtlString		CharDatabaseUser;
	CNtlString		CharDatabasePassword;
	CNtlString		CharDatabaseName;

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
};




//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//

class CQueryServer : public CNtlServerApp
{

public:

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

	CNtlString		GetLogDatabaseHost() { return m_config.LogDatabaseHost; }
	UINT			GetLogDatabasePort() { return m_config.LogDatabasePort; }
	CNtlString		GetLogDatabaseUser() { return m_config.LogDatabaseUser; }
	CNtlString		GetLogDatabasePassword() { return m_config.LogDatabasePassword; }
	CNtlString		GetLogDatabaseName() { return m_config.LogDatabaseName; }

	int				OnInitApp();
	int				OnAppStart();
	int				OnCreate();
	void			OnDestroy();


	int	OnConfiguration(const char * lpszConfigFile)
	{
		CNtlIniFile file;
		int rc = file.Create( lpszConfigFile );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}

		if (!file.Read("Chat Accept", "Address", m_config.strChatServerAcceptAddr))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Chat Accept", "Port", m_config.wChatServerAcceptPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Game Accept", "Address", m_config.strGameServerAcceptAddr))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Game Accept", "Port", m_config.wGameServerAcceptPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Char Accept", "Address", m_config.strCharServerAcceptAddr))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Char Accept", "Port", m_config.wCharServerAcceptPort))
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

		//MASTER SERVER
		if (!file.Read("Master Connect", "Address", m_config.strMasterServerIP))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Master Connect", "Port", m_config.wMasterServerPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		return NTL_SUCCESS;
	}

	virtual void	Run();

	virtual BOOL	OnCommandInput(std::string& sCmd);

	void			CreateQueryTaskRunThread(int nAmount = 1);


private:
	sSERVERCONFIG				m_config;
	CNtlConnector				m_serverConnector; //connect to master server

	CNtlAcceptor				m_ChatServerAcceptor; //accept chat server connection
	CNtlAcceptor				m_GameServerAcceptor; //accept game server connection
	CNtlAcceptor				m_CharServerAcceptor; //accept chat server connection

public:
	CNtlLog  					m_log;


private:

	bool						m_bDragonballScrambleBegin;


public:

	inline void					SetDragonballScrambleBegin(bool bFlag) { m_bDragonballScrambleBegin = bFlag; }
	inline bool					GetDragonballScrambleBegin() { return m_bDragonballScrambleBegin; }

};