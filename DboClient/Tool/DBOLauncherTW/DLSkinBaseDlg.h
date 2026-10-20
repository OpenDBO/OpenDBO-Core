/*****************************************************************************
*
* File			: DLSkinBaseDlg.h
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
#include "BitmapDlg.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLSkinBaseDlg
//
//////////////////////////////////////////////////////////////////////////


class CDLSkinBaseDlg : public CBitmapDlg
{
public:
	CDLSkinBaseDlg( void );
	virtual ~CDLSkinBaseDlg( void );

public:
	virtual bool				CreateDlg( void ) { return false; }

	virtual void				DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );

protected:
	void						UpdateDialog( void );

	// Button actions shared by every skin (one function each in the retail
	// binary, used by all skin message maps)
	void						DoGameStart( void );
	void						DoUserExit( void );
	void						DoIntegrityCheck( void );
	void						DoOption( void );
};
