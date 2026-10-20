/*****************************************************************************
*
* File			: DLSkinDlg_CT.h
* Author		:
* Copyright		: (주) NTL
* Date			:
* Abstract		:
*****************************************************************************
* Desc          : Skin of the Cayenne (CT_TW / CT_HK) publisher.
*
*****************************************************************************/


#pragma once


#include "resource.h"
#include "DLSkinBaseDlg.h"
#include "Explorer_information.h"
#include "OutputBox.h"
#include "ImageTextButton.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLSkinDlg_CT
//
//////////////////////////////////////////////////////////////////////////


class CDLSkinDlg_CT : public CDLSkinBaseDlg
{
public:
	#define CT_MAX_PROGRESS_RANGE (1000)

protected:
	CExplorer_information		m_ctrExplorerInfo;

	COutputBox					m_ctrDownload;
	COutputBox					m_ctrVersionInfo;
	COutputBox					m_ctrPatch;

	CImageTextButton			m_ctrCancel;
	CImageTextButton			m_ctrGameStart;
	CImageTextButton			m_ctrGameIntegrity;
	CImageTextButton			m_ctrOption;

	int							m_nIDDownloadProgress;
	int							m_nIDPatchProgress;

	HICON						m_hIcon;

	unsigned int				m_uiTotalFilesInUpdateFile;
	unsigned int				m_uiCurFilesInUpdateFile;

public:
	CDLSkinDlg_CT( void );
	virtual ~CDLSkinDlg_CT( void );

public:
	virtual bool				CreateDlg( void );

	virtual void				DLMessageProc( eDL_EVENT eMsg, sDL_EVENT_DATA_BASE* pData );

protected:
	virtual void				DoDataExchange( CDataExchange* pDX );
	virtual BOOL				OnInitDialog( void );
	virtual BOOL				DestroyWindow( void );

	void						ShowVersion( const CString& strClientVer, const CString& strDate, const CString& strLauncherVer );

	afx_msg void				OnPaint( void );
	afx_msg HCURSOR				OnQueryDragIcon( void );
	afx_msg void				OnClose( void );
	afx_msg LRESULT				OnNcHitTest( CPoint point );
	afx_msg void				OnBnClickedCTGameStart( void );
	afx_msg void				OnBnClickedCTCancel( void );
	afx_msg void				OnBnClickedCTIntegrityCheck( void );
	afx_msg void				OnBnClickedCTOption( void );

	DECLARE_MESSAGE_MAP()
};
