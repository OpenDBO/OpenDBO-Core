/*****************************************************************************
*
* File			: DLDownLoadSys.h
* Author		:
* Copyright		: (주) NTL
* Date			:
* Abstract		:
*****************************************************************************
* Desc          :
*
*****************************************************************************/


#pragma once


#include "DLEventDef.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLDownLoadSys
//
//////////////////////////////////////////////////////////////////////////


class CDLDownLoadSys
{
protected:
	bool					m_bPendingDestroy;

public:
	CDLDownLoadSys( void );
	~CDLDownLoadSys( void );

public:
	bool					Create( void );
	void					Delete( void );

	void					DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );

protected:
	static bool				DoDownload( bool* pbPendingDestroy,
										CString strServerName,
										CString strServerPath,
										CString strServerFileName,
										CString strClientPath,
										CString strClientFileName );
};
