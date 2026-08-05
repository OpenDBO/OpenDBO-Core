/******************************************************************************
* File			: OptionGraphicTab.h
* Author		: Hae sung, Cho
* Copyright		: (��)NTL
* Date			: 2009. 3. 24
* Abstract		: 
*****************************************************************************
* Desc			: Tab of graphics
*****************************************************************************/

#ifndef _OPTION_GRAPHIC_TAB_H_
#define _OPTION_GRAPHIC_TAB_H_

#pragma once

//// Video Mode ����Ʈ
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
	gui::CDialog*		m_pThis;			///< ���� Dialog
	gui::CDialog*		m_pScrollDialog;	///< ��ũ�� �Ǵ� Dialog

	gui::CSlot			m_slotAbsoluteMouseWheel;

	gui::CStaticBox*	m_pStbResolutionTitle;	///< �ػ� ����
	gui::CStaticBox*	m_pStbResolution;		///< �ػ�
	gui::CComboBox*		m_pCbbResolution;		///< �ػ� ���� �޺��ڽ�
	gui::CSlot			m_slotListToggled;	

	gui::CStaticBox*	m_pStbGamma;			///< ���
	gui::CScrollBar*	m_pScbGamma;			///< ��� ���� ��ũ�ѹ�
	gui::CSlot			m_slotGammaSliderMoved;
	gui::CSlot			m_slotGammaValueChanged;

	gui::CStaticBox*	m_pStbFps;
	gui::CScrollBar*	m_pScbFps;			///< FPS ���� ��ũ�ѹ�
	gui::CSlot			m_slotFpsSliderMoved;
	gui::CSlot			m_slotFpsValueChanged;

	gui::CButton*		m_pChkFpsLimit;		///< FPS limiter on/off checkbox
	gui::CSlot			m_slotFpsLimitToggled;

	gui::CStaticBox*	m_pStbSSAATitle;
	gui::CComboBox*		m_pCbbSSAA;
	gui::CSlot			m_slotSSAAChanged;
	gui::CSlot			m_slotSSAASelected;

	gui::CButton*		m_pBtnWindowMode;		///< ������ ��� ���� ��۹�

	gui::CStaticBox*	m_pStbEffectTitle;		///< ȿ�� Ÿ��Ʋ
	gui::CStaticBox*	m_pStbGraphicQuality;	///< ��ü �׷��� ǰ��
	gui::CStaticBox*	m_pStbGraphicQualityLow;	///< ����
	gui::CStaticBox*	m_pStbGraphicQualityMiddle;	///< ����
	gui::CStaticBox*	m_pStbGraphicQualityHigh;	///< ����
	gui::CStaticBox*	m_pStbGraphicQualityUser;	///< ����� ����
	gui::CScrollBar*	m_pScbGraphicQuality;		///< �׷��� ǰ�� ���� ��ũ�ѹ�
	gui::CSlot			m_slotQualitySliderMoved;
	gui::CSlot			m_slotQualityValueChanged;

	gui::CStaticBox*	m_pStbTerrainDist;		///< ���� �þ߰Ÿ�
	gui::CScrollBar*	m_pScbTerrainDist;		///< ���� �þ߰Ÿ� ���� ��ũ�� ��
	gui::CSlot			m_slotTerrainSliderMoved;
	gui::CSlot			m_slotTerrainValueChanged;
	
	gui::CStaticBox*	m_pStbObjectDist;		///< �繰 �þ߰Ÿ�
	gui::CScrollBar*	m_pScbObjectDist;		///< �繰 �þ߰Ÿ� ���� ��ũ�� ��
	gui::CSlot			m_slotObjectSliderMoved;
	gui::CSlot			m_slotObjectValueChanged;

	gui::CButton*		m_pBtnTerrainShadow;	///< ���� �׸���
	gui::CSlot			m_slotTerrainShadowToggled;

	gui::CButton*		m_pBtnWaterEffect;		///< �� ȿ��
	gui::CSlot			m_slotWaterEffectToggled;

	/*gui::CStaticBox*	m_pStbCharacterDist;	///< ĳ���� �þ߰Ÿ�
	gui::CScrollBar*	m_pScbCharacterDist;	///< ĳ���� �þ߰Ÿ� ���� ��ũ�� ��
	gui::CSlot			m_slotCharacterDistSliderMoved;
	gui::CSlot			m_slotCharacterDistValueChanged;*/

	gui::CButton*		m_pBtnCharacterShadow;	///< ĳ���� �׸���
	gui::CSlot			m_slotCharacterShadowToggled;

	gui::CStaticBox*	m_pStbMagicEffect;		///< ���� ����ȿ��
	gui::CScrollBar*	m_pScbMagicEffect;		///< ���� ����ȿ�� ���� ��ũ�� ��
	gui::CSlot			m_slotMagicEffectSliderMoved;
	gui::CSlot			m_slotMagicEffectValueChanged;

	gui::CStaticBox*	m_pStbWeatherEffect;	///< ���� ȿ��
	gui::CScrollBar*	m_pScbWeatherEffect;	///< ���� ȿ�� ���� ��ũ�� ��
	gui::CSlot			m_slotWeatherEffectSliderMoved;
	gui::CSlot			m_slotWeatherEffectValueChanged;

	gui::CStaticBox*	m_pStbTextureQuality;	///< �ؽ��� �ػ�
	gui::CScrollBar*	m_pScbTextureQuality;	///< �ؽ��� �ػ� ���� ��ũ�� ��
	gui::CSlot			m_slotTextureQualitySliderMoved;
	gui::CSlot			m_slotTextureQualityValueChanged;

	gui::CStaticBox*	m_pStbCharacterEffectTitle;	///< ĳ���� ȿ�� Ÿ��Ʋ
	gui::CButton*		m_pBtnCartoonEdge;			///< ī���� �ܰ���
	/*gui::CButton*		m_pBtnUpgradeEffect;		///< ���׷��̵� ȿ��*/
	gui::CSlot			m_slotCartoonEdgeToggled;
	/*gui::CSlot			m_slotUpgradeEffectToggled;*/

	gui::CStaticBox*	m_pStbEtcTitle;				
	gui::CButton*		m_pBtnHdrEffect;			///< HDR ȿ��
	gui::CSlot			m_slotHdrEffectToggled;
	
	gui::CScrollBar*	m_pThisScrollBar;			///< ��ũ�� ��
	gui::CSlot			m_slotScrollBarChanged;
	gui::CSlot			m_slotScrollBarSliderMoved;

	RwInt32				m_iGamma;
	ListVideoMode		m_listFullVideo;
	ListVideoMode		m_listWinVideo;
	SVideoModeInfo		m_sSelectMode;
};


#endif