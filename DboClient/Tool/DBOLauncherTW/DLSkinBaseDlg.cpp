/*****************************************************************************
*
* File			: DLSkinBaseDlg.cpp
* Author		: 
* Copyright		: (주) NTL
* Date			:
* Abstract		: 
*****************************************************************************
* Desc          : 
*
*****************************************************************************/


#include "stdafx.h"
#include "DLSkinBaseDlg.h"
#include "DBOLauncherDef.h"
#include "DLUtility.h"
#include "DLCmdLine.h"
#include "DLState.h"
#include "DLDBOSettingDlg.h"
#include "NtlClientService.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLSkinBaseDlg
//
//////////////////////////////////////////////////////////////////////////


CDLSkinBaseDlg::CDLSkinBaseDlg( void )
{
}

CDLSkinBaseDlg::~CDLSkinBaseDlg( void )
{
}

void CDLSkinBaseDlg::DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData )
{
}

void CDLSkinBaseDlg::UpdateDialog( void )
{
	bool bContinue = true;
	MSG msg;

	while ( bContinue )
	{
		if ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			bContinue = false;
		}
	}
}

void CDLSkinBaseDlg::DoGameStart( void )
{
	CString strCmd = g_clDLCmdLine.GetServiceCmd();

	GetServiceInterface()->End();

	ShellExecute( NULL, NULL, DL_CLIENT_NAME, strCmd, NULL, SW_SHOWNORMAL );

	sDL_ED_USER_EXIT sEDUserExit;
	DLSendEvent( eDL_EVENT_USER_EXIT, &sEDUserExit );
}

void CDLSkinBaseDlg::DoUserExit( void )
{
	sDL_ED_USER_EXIT sEDUserExit;
	DLSendEvent( eDL_EVENT_USER_EXIT, &sEDUserExit );
}

void CDLSkinBaseDlg::DoIntegrityCheck( void )
{
	sDL_ED_CHANGE_STATE sEDCState;
	sEDCState.uiState = eDL_STATE_INTEGRITY_CHECK;
	sEDCState.pData = NULL;
	DLSendEvent( eDL_EVENT_CHANGE_STATE, &sEDCState );
}

void CDLSkinBaseDlg::DoOption( void )
{
	CDLDBOSettingDlg clSettingDlg;
	clSettingDlg.DoModal();
}
