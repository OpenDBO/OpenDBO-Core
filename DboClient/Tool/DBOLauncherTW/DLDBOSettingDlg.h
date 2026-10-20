/*****************************************************************************
*
* File			: DLDBOSettingDlg.h
* Author		:
* Copyright		: (주) NTL
* Date			:
* Abstract		:
*****************************************************************************
* Desc          : Game option dialog opened by the skin's option button.
*                 Edits resolution, brightness, graphic preset, window mode
*                 and the four volumes in ".\user\SystemEnv.txt".
*
*****************************************************************************/


#pragma once


#include "resource.h"
#include "BitmapDlg.h"
#include "ImageTextButton.h"
#include "BitmapSlider.h"


//////////////////////////////////////////////////////////////////////////
//
// CDLDBOSettingDlg
//
//////////////////////////////////////////////////////////////////////////


class CDLDBOSettingDlg : public CBitmapDlg
{
public:
	typedef std::map< CString, DEVMODE > mapdef_ResolutionList;

	// Slider with its own decrease / increase buttons
	struct sSLIDER_GROUP
	{
		CImageTextButton		ctrLeft;
		CBitmapSlider			ctrSlider;
		CImageTextButton		ctrRight;
	};

	enum eSLIDER
	{
		eSLIDER_GAMMA,
		eSLIDER_QUALITY,
		eSLIDER_MAIN_VOLUME,
		eSLIDER_BGM_VOLUME,
		eSLIDER_EFFECT_VOLUME,
		eSLIDER_ENV_VOLUME,

		eSLIDER_COUNT
	};

protected:
	mapdef_ResolutionList		m_defResolutionList;

	CFont*						m_pFontBold;
	CFont*						m_pFont;

	CImageTextButton			m_ctrCloseX;
	CComboBox					m_ctrResolution;
	CButton						m_ctrWindowMode;

	sSLIDER_GROUP				m_asSlider[eSLIDER_COUNT];

	CImageTextButton			m_ctrApply;
	CImageTextButton			m_ctrDefault;
	CImageTextButton			m_ctrClose;

public:
	CDLDBOSettingDlg( CWnd* pParent = NULL );
	virtual ~CDLDBOSettingDlg( void );

protected:
	virtual void				DoDataExchange( CDataExchange* pDX );
	virtual BOOL				OnInitDialog( void );

	static CFont*				CreateDlgFont( bool bBold, const CString& strFaceName, int nHeight );

	void						RefreshResolutionList( void );
	static void					EnumResolution( mapdef_ResolutionList& defList, bool bWindowMode );
	DEVMODE*					FindResolution( int nWidth, int nHeight, int nDepth );

	void						LoadOption( void );
	bool						SaveOption( void );
	void						SaveOptionFile( void );

	void						StepSlider( int nSlider, int nStep );

	afx_msg void				OnGammaLeft( void )			{ StepSlider( eSLIDER_GAMMA, -1 ); }
	afx_msg void				OnGammaRight( void )		{ StepSlider( eSLIDER_GAMMA, 1 ); }
	afx_msg void				OnQualityLeft( void )		{ StepSlider( eSLIDER_QUALITY, -1 ); }
	afx_msg void				OnQualityRight( void )		{ StepSlider( eSLIDER_QUALITY, 1 ); }
	afx_msg void				OnMainVolumeLeft( void )	{ StepSlider( eSLIDER_MAIN_VOLUME, -1 ); }
	afx_msg void				OnMainVolumeRight( void )	{ StepSlider( eSLIDER_MAIN_VOLUME, 1 ); }
	afx_msg void				OnBgmVolumeLeft( void )		{ StepSlider( eSLIDER_BGM_VOLUME, -1 ); }
	afx_msg void				OnBgmVolumeRight( void )	{ StepSlider( eSLIDER_BGM_VOLUME, 1 ); }
	afx_msg void				OnEffectVolumeLeft( void )	{ StepSlider( eSLIDER_EFFECT_VOLUME, -1 ); }
	afx_msg void				OnEffectVolumeRight( void )	{ StepSlider( eSLIDER_EFFECT_VOLUME, 1 ); }
	afx_msg void				OnEnvVolumeLeft( void )		{ StepSlider( eSLIDER_ENV_VOLUME, -1 ); }
	afx_msg void				OnEnvVolumeRight( void )	{ StepSlider( eSLIDER_ENV_VOLUME, 1 ); }

	afx_msg void				OnBnClickedClose( void );
	afx_msg void				OnBnClickedApply( void );
	afx_msg void				OnBnClickedDefault( void );
	afx_msg void				OnBnClickedWindowMode( void );
	afx_msg void				OnPaint( void );

	DECLARE_MESSAGE_MAP()
};
