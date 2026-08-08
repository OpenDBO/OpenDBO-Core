/******************************************************************************
* File			: OptionGraphicTab.h
* Author		: Hae sung, Cho
* Copyright		: (占쏙옙)NTL
* Date			: 2009. 3. 24
* Abstract		: 
*****************************************************************************
* Desc			: Tab of graphics
*****************************************************************************/

#ifndef _OPTION_GRAPHIC_TAB_H_
#define _OPTION_GRAPHIC_TAB_H_

#pragma once

//// Video Mode 占쏙옙占쏙옙트
typedef struct _SVideoModeInfo
{
	RwInt32 iWidth;
	RwInt32 iHeight;
	RwInt32 iColorDepth;
}SVideoModeInfo;

typedef std::list<SVideoModeInfo> ListVideoMode;

/**
* \ingroup client
*/
class COptionGraphic : public COptionBase
{
public:
	enum eQuality
	{
		eQUALITY_LOW = 0,
		eQUALITY_MIDDLE,
		eQUALITY_HIGH,
		eQUALITY_USER,

		eQUALITY_NUMS,
		eQUALITY_INVALID = 0XFF
	};

public:
	COptionGraphic();
	virtual ~COptionGraphic();

	virtual RwBool		Create(COptionWindowGui* pOptionWindow);
	virtual void		Destroy();

	virtual void		Show();
	virtual void		Hide();

	virtual void		OnInit();
	virtual void		OnReset();
	virtual void		OnOk();
	virtual void		OnCancel();
	virtual void		OnHandleEvents(RWS::CMsg &pMsg);

	void				OnScrollBarChanged(RwInt32 nParam);
	void				OnAbsoluteMouseWheel( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	void				OnListToggled(BOOL bToggled, gui::CComponent* pComponent);

	// Resolution & Gamma
public:
	void				SetComponentResolution(RwBool bDefault = FALSE);

	RwInt32				GetVideoMode(RwBool bDefault = FALSE);
	void				SetVideoMode(RwInt32 iIndex);
	std::wstring		GetVideoModeText();
	void				SetVideoModeText(const std::wstring& strText);

	RwInt32				GetGammaValue();
	void				SetGammaValue(RwInt32 iValue);
	void				OnGammaChanged(RwInt32 iParam);

	void				SetFpsValue(RwInt32 iValue);
	void				OnFpsSliderMoved(RwInt32 iValue);
	void				SetFpsToolTip(RwInt32 iValue);
	void				SetFpsSliderEnabled(RwBool bEnable);
	void				OnFpsLimitToggled(gui::CComponent* pComponent, bool bDown);

	void				SetSSAAValue(RwInt32 iValue);
	void				OnSSAAChanged(RwInt32 iValue);
	void				SetSSAAToolTip(RwInt32 iValue);

	void				SelectVideoMode(RwInt32 iWidth, RwInt32 iHeight, RwInt32 iColorDepth); 
	void				SelectVideoModeWin(RwInt32 iWidth, RwInt32 iHeight);
	void				GetSelectVideoMode(SVideoModeInfo& sVideoModeInfo);
	ListVideoMode&		GetVideoModeList(RwBool bFullMode);

	// Preset
public:
	void				SetPreset( RwInt32 nLevel );
	
	void				OnOtherDataChanged( RwInt32 nParam );
	void				OnOtherDataToggled( gui::CComponent* pComponent, bool bToggle );

	// Handler
public:
	int					RestoreHandler();

protected:
	int					m_nCurrentQuality;	
	DEVICE_RESTORE_HANDLE m_hRestore;

protected:
	gui::CDialog*		m_pThis;			///< 占쏙옙占쏙옙 Dialog
	gui::CDialog*		m_pScrollDialog;	///< 占쏙옙크占쏙옙 占실댐옙 Dialog

	gui::CSlot			m_slotAbsoluteMouseWheel;

	gui::CStaticBox*	m_pStbResolutionTitle;	///< 占쌔삼옙 占쏙옙占쏙옙
	gui::CStaticBox*	m_pStbResolution;		///< 占쌔삼옙
	gui::CComboBox*		m_pCbbResolution;		///< 占쌔삼옙 占쏙옙占쏙옙 占쌨븝옙占쌘쏙옙
	gui::CSlot			m_slotListToggled;	

	gui::CStaticBox*	m_pStbGamma;			///< 밝기
	gui::CScrollBar*	m_pScbGamma;			///< 밝기 설정 스크롤바
	gui::CSlot			m_slotGammaSliderMoved;
	gui::CSlot			m_slotGammaValueChanged;

	gui::CStaticBox*	m_pStbFps;
	gui::CScrollBar*	m_pScbFps;			///< FPS 占쏙옙占쏙옙 占쏙옙크占싼뱄옙
	gui::CSlot			m_slotFpsSliderMoved;
	gui::CSlot			m_slotFpsValueChanged;

	gui::CButton*		m_pChkFpsLimit;		///< FPS limiter on/off checkbox
	gui::CSlot			m_slotFpsLimitToggled;

	gui::CStaticBox*	m_pStbSSAATitle;
	gui::CComboBox*		m_pCbbSSAA;
	gui::CSlot			m_slotSSAAChanged;
	gui::CSlot			m_slotSSAASelected;

	gui::CButton*		m_pBtnWindowMode;		///< 윈도우 모드 설정 토글바

	gui::CStaticBox*	m_pStbEffectTitle;		///< 효占쏙옙 타占쏙옙틀
	gui::CStaticBox*	m_pStbGraphicQuality;	///< 占쏙옙체 占쌓뤄옙占쏙옙 품占쏙옙
	gui::CStaticBox*	m_pStbGraphicQualityLow;	///< 占쏙옙占쏙옙
	gui::CStaticBox*	m_pStbGraphicQualityMiddle;	///< 占쏙옙占쏙옙
	gui::CStaticBox*	m_pStbGraphicQualityHigh;	///< 占쏙옙占쏙옙
	gui::CStaticBox*	m_pStbGraphicQualityUser;	///< 사용자 설정
	gui::CScrollBar*	m_pScbGraphicQuality;		///< 占쌓뤄옙占쏙옙 품占쏙옙 占쏙옙占쏙옙 占쏙옙크占싼뱄옙
	gui::CSlot			m_slotQualitySliderMoved;
	gui::CSlot			m_slotQualityValueChanged;

	gui::CStaticBox*	m_pStbTerrainDist;		///< 占쏙옙占쏙옙 占시야거몌옙
	gui::CScrollBar*	m_pScbTerrainDist;		///< 占쏙옙占쏙옙 占시야거몌옙 占쏙옙占쏙옙 占쏙옙크占쏙옙 占쏙옙
	gui::CSlot			m_slotTerrainSliderMoved;
	gui::CSlot			m_slotTerrainValueChanged;
	
	gui::CStaticBox*	m_pStbObjectDist;		///< �繰 �þ߰Ÿ�
	gui::CScrollBar*	m_pScbObjectDist;		///< �繰 �þ߰Ÿ� ���� ��ũ�� ��
	gui::CSlot			m_slotObjectSliderMoved;
	gui::CSlot			m_slotObjectValueChanged;

	gui::CButton*		m_pBtnTerrainShadow;	///< 占쏙옙占쏙옙 占쌓몌옙占쏙옙
	gui::CSlot			m_slotTerrainShadowToggled;

	gui::CButton*		m_pBtnWaterEffect;		///< 占쏙옙 효占쏙옙
	gui::CSlot			m_slotWaterEffectToggled;

	/*gui::CStaticBox*	m_pStbCharacterDist;	///< 캐占쏙옙占쏙옙 占시야거몌옙
	gui::CScrollBar*	m_pScbCharacterDist;	///< 캐占쏙옙占쏙옙 占시야거몌옙 占쏙옙占쏙옙 占쏙옙크占쏙옙 占쏙옙
	gui::CSlot			m_slotCharacterDistSliderMoved;
	gui::CSlot			m_slotCharacterDistValueChanged;*/

	gui::CButton*		m_pBtnCharacterShadow;	///< 캐占쏙옙占쏙옙 占쌓몌옙占쏙옙
	gui::CSlot			m_slotCharacterShadowToggled;

	gui::CStaticBox*	m_pStbMagicEffect;		///< 占쏙옙占쏙옙 占쏙옙占쏙옙효占쏙옙
	gui::CScrollBar*	m_pScbMagicEffect;		///< 占쏙옙占쏙옙 占쏙옙占쏙옙효占쏙옙 占쏙옙占쏙옙 占쏙옙크占쏙옙 占쏙옙
	gui::CSlot			m_slotMagicEffectSliderMoved;
	gui::CSlot			m_slotMagicEffectValueChanged;

	gui::CStaticBox*	m_pStbWeatherEffect;	///< 占쏙옙占쏙옙 효占쏙옙
	gui::CScrollBar*	m_pScbWeatherEffect;	///< 占쏙옙占쏙옙 효占쏙옙 占쏙옙占쏙옙 占쏙옙크占쏙옙 占쏙옙
	gui::CSlot			m_slotWeatherEffectSliderMoved;
	gui::CSlot			m_slotWeatherEffectValueChanged;

	gui::CStaticBox*	m_pStbTextureQuality;	///< 占쌔쏙옙占쏙옙 占쌔삼옙
	gui::CScrollBar*	m_pScbTextureQuality;	///< 占쌔쏙옙占쏙옙 占쌔삼옙 占쏙옙占쏙옙 占쏙옙크占쏙옙 占쏙옙
	gui::CSlot			m_slotTextureQualitySliderMoved;
	gui::CSlot			m_slotTextureQualityValueChanged;

	gui::CStaticBox*	m_pStbCharacterEffectTitle;	///< 캐占쏙옙占쏙옙 효占쏙옙 타占쏙옙틀
	gui::CButton*		m_pBtnCartoonEdge;			///< 카占쏙옙占쏙옙 占쌤곤옙占쏙옙
	/*gui::CButton*		m_pBtnUpgradeEffect;		///< 占쏙옙占쌓뤄옙占싱듸옙 효占쏙옙*/
	gui::CSlot			m_slotCartoonEdgeToggled;
	/*gui::CSlot			m_slotUpgradeEffectToggled;*/

	gui::CStaticBox*	m_pStbEtcTitle;				
	gui::CButton*		m_pBtnHdrEffect;			///< HDR 효占쏙옙
	gui::CSlot			m_slotHdrEffectToggled;
	
	gui::CScrollBar*	m_pThisScrollBar;			///< 占쏙옙크占쏙옙 占쏙옙
	gui::CSlot			m_slotScrollBarChanged;
	gui::CSlot			m_slotScrollBarSliderMoved;

	RwInt32				m_iGamma;
	ListVideoMode		m_listFullVideo;
	ListVideoMode		m_listWinVideo;
	SVideoModeInfo		m_sSelectMode;
};


#endif
