/*****************************************************************************
*
* File			: DLLauncherDevMaster.h
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#pragma once


//////////////////////////////////////////////////////////////////////////
//
// CDLLauncherDevMaster
//
//////////////////////////////////////////////////////////////////////////


class CDLLauncherDevMaster
{
protected:
	bool					m_bNoCheckPatchServerEnable;
	bool					m_bNoCheckIntegrityServerEnable;
	bool					m_bOfflineModeEnable;

public:
	CDLLauncherDevMaster( void ) { m_bNoCheckPatchServerEnable = false; m_bNoCheckIntegrityServerEnable = false; m_bOfflineModeEnable = false; }
	~CDLLauncherDevMaster( void ) { return; }

public:
	bool					Load( CString strPath );

	bool					IsNoCheckPatchServerEnable( void ) const;
	bool					IsNoCheckIntegrityServerEnable( void ) const;

	// When enabled, the launcher skips the online server-config/version-list/patch
	// steps entirely and goes straight to the idle screen so the client can be
	// started locally without a reachable patch server.
	bool					IsOfflineModeEnable( void ) const;
};


extern CDLLauncherDevMaster g_clDLLaucherDevMaster;
