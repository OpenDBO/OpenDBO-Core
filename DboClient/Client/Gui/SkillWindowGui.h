////////////////////////////////////////////////////////////////////////////////
// File: SkillWindowGui.h
// Desc: SkillWindow 
//
// 2006.5.4 Peessi@hitel.net   
//
// To Be Desired:
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_WINDOW_GUI_H__
#define __SKILL_WINDOW_GUI_H__

#define MAX_VIEWABLE_ITEM	5

#include "SurfaceGui.h"
#include "SkillCommon.h"

class CSkillWindowGui;

class CSkillWindowItemDlg
{
public:
//! Enum
	
	enum eMOUSEFOCUS { MOUSE_NONE, MOUSE_ONICON, MOUSE_ONRQ };

	//! Constructor & Destuctor
	CSkillWindowItemDlg( CSkillWindowGui* pSkillWnd, gui::CTexture* pUnknownTexture );
	~CSkillWindowItemDlg(VOID);

	//! Attributes
	CSkillGuiItem* GetSkillItem(VOID) { return m_pItem; }
	bool	IsShow(VOID) { return m_pThis->IsVisible(); }
	bool	IsEnable(VOID) { return m_pThis->IsEnabled(); }
	CRectangle GetPosition(VOID) { return m_pThis->GetPosition(); }
	gui::CRadarEffect* GetCoolTimeEffect(VOID) { return &m_CoolTimeEffect; }

	//! Operation
	VOID	Update( RwReal fElapsedTime );

	VOID	UpdateData( CSkillGuiItem* pItem );
	VOID	SetSkillItem( CSkillGuiItem* pItem ) { m_pItem = pItem; }

	VOID	Show( bool bShow ) { m_pThis->Show( bShow ); }
	VOID	Enable( bool bEnable ) { m_pThis->Enable( bEnable ); }
	VOID	SetPosition( RwInt32 nX, RwInt32 nY ) { m_pThis->SetPosition( nX, nY ); }
	VOID	SetPosition( const CRectangle& rect ) { m_pThis->SetPosition( rect ); }

	VOID	SetColor( RwUInt8 ucRed, RwUInt8 ucGreen, RwUInt8 ucBlue );
	VOID	SetAlpha( RwUInt8 ucAlpha );

	VOID	StartCoolTime(VOID);
	VOID	EndCoolTime(VOID);
	
	VOID	ClickEffect( RwBool bPush, RwBool bStillPush = TRUE );
	VOID	ShowPickedUp( RwBool bShow );
private:
//! Implementation
	VOID	UpdateSkillData( CSkillGuiItem* pItem );
	VOID	UpdateHTBData( CSkillGuiItem* pItem );
	VOID	UpdateActionData( CSkillGuiItem* pItem );
		
	VOID	SetRQSurface( RwBool bFocus );
	VOID	SetRPTypeImage( RwBool bAutoMode, RwInt32 eRPType = 0xFF );

	RwUInt32 ExmineRQBySkill(VOID);
	RwUInt32 ExmineRQByHTB(VOID);

	RwBool	CanLearnSKill( RwUInt8 ucAvatarLevel );
	RwBool  CanLearnHTBSkill( RwUInt8 ucAvatarLevel );
	RwBool	CanRQDisplay(VOID);	
							
//! CallBack
	VOID	OnMouseDown( const CKey& key );
	VOID	OnMouseUp( const CKey& key );
	VOID	OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
	VOID	OnMouseLeave( gui::CComponent* pComponent );
	VOID	OnLearnButtonEnter( gui::CComponent* pComponent );
	VOID	OnLearnButtonLeave( gui::CComponent* pComponent );
	VOID	OnUpgradeButtonEnter( gui::CComponent* pComponent );
	VOID	OnUpgradeButtonLeave( gui::CComponent* pComponent );
	VOID	OnLearnButtonClick( gui::CComponent* pComponent );
	VOID	OnUpgradeButtonClick( gui::CComponent* pComponent );
	VOID	OnRPModeClick( gui::CComponent* pComponent );

	gui::CSlot	m_slotMouseDown;
	gui::CSlot	m_slotMouseUp;
	gui::CSlot	m_slotMouseMove;
	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotMove;
	gui::CSlot  m_slotMouseLeave;
	gui::CSlot	m_slotLearnButtonEnter;
	gui::CSlot	m_slotLearnButtonLeave;
	gui::CSlot	m_slotUpgradeButtonEnter;
	gui::CSlot	m_slotUpgradeButtonLeave;
	gui::CSlot	m_slotLearnButtonClick;
	gui::CSlot	m_slotUpgradeButtonClick;
	gui::CSlot	m_slotRPModeClick;

//! Variables
	CRectangle			m_rtIcon;		// 아이콘 영역
	CSurfaceGui			m_surIcon;		// 아이콘 그림
	CRectangle			m_rtRQ;			// RQ 영역
	CSurfaceGui			m_surRQ;		// RQ 그림
	CSurfaceGui			m_surRPType;	// RPType

	gui::CStaticBox*	m_pTitle;
	gui::CStaticBox*	m_pGrade;
	gui::CProgressBar*	m_pGauge;
	gui::CPanel*		m_pGaugeBack;
	gui::CStaticBox*	m_pGaugeNumber;
	gui::CStaticBox*	m_pConsumeTitle;
	gui::CMDStaticBox*	m_pText;
	gui::CStaticBox*	m_pHelpText;

	gui::CDialog*		m_pThis;
	gui::CTexture*		m_pUnknownTexture;
	
	gui::CButton*		m_pbtnRPMode;
	gui::CButton*		m_pBtnUpgrade;
	gui::CButton*		m_pBtnLearn;
	
	RwUInt32			m_flagRQState;
	CSkillGuiItem*		m_pItem;
	
	RwBool				m_bLClickIcon;	// MouseDown이 icon영역에서 이루어졌으면 TRUE
	RwBool				m_bRClickIcon;	
	eMOUSEFOCUS			m_eMouseOn;
	RwBool				m_bPushDownIcon;
	
	RwReal				m_fRightClickTime;	// 오른쪽 버튼 눌렀을때의 시간. RPBonus적용

	RwUInt32			m_ucColorRed;	// 다이얼로그 전체 색. 
	RwUInt32			m_ucColorGreen;
	RwUInt32			m_ucColorBlue;
	RwUInt32			m_ucAlpha;
	
	gui::CRadarEffect	m_CoolTimeEffect;
	CSurfaceGui			m_surFocusEffect;

	CSurfaceGui			m_surPickedUp;
	RwBool				m_bShowPickedUp;
	
	RwBool				m_bShowRQ;	
		
	CSkillWindowGui*	m_pSkillWnd;
};

class CSkillWindowGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
//! Enum
	enum eTAB				{ NOT_TAB = -1, BATTLE_TAB, ASSIST_TAB, ACTION_TAB, NUM_TAB };
	enum eBATTLESKILLFILTER	{ NO_BATTLE_FILTER = -1, BATTLE_FILTER = 0, HTB_FILTER, NUM_BATTLE_FILTER };
	enum eASSISTSKILLFILTER	{ NO_ASSIST_FILTER = -1, ASSIST_FILTER = 0, PASSIVE_FILTER, NUM_ASSIST_FILTER };
	enum eACTIONSKILLFILTER { NO_ACTION_FILTER = -1, SOCIAL_FILTER = 0, ETC_FILTER, NUM_ACTION_FILTER };
			
	typedef std::map<RwUInt32,CSkillGuiItem*>		SKILLMAP;
	typedef SKILLMAP::iterator						SKILLMAP_IT;
	
	typedef std::vector<CSkillGuiItem*>				FILTERVEC;
	typedef FILTERVEC::iterator						FILTERVEC_IT;	
	
//! Constructor & Destructor
	CSkillWindowGui(VOID);
	CSkillWindowGui( const RwChar* pName );
	~CSkillWindowGui(VOID);

//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsed );

	RwInt32		GetCurrentTab(VOID) { return (RwInt32)m_eCurrentTab; }	
	RwInt32		SwitchDialog( bool bOpen);
	
private:
//! Implementation
	VOID	Init(VOID);
	VOID	ClearItems(VOID);
	VOID	ClearItemDialogs(VOID);

	VOID	ShowItemDetect(VOID);
	RwBool	CreateItems(VOID);					// UI 생성시 1회. 
	VOID	AddItemsByClass( RwUInt8 byClass );	// 전직시 사용.

	VOID	FindAddedSkill( RwUInt32 uiSlot );	// 새로 추가되거나 나타나야하는 스킬 설정.

	VOID	AddSkillMapData( SKILLMAP* pMap, CSkillGuiItem* pItem );
	CSkillGuiItem* GetSkillMapData( SKILLMAP* pMap, RwUInt32 idx );

	VOID	SetFilterItems(VOID);				// Filter 세팅.
	VOID	FilterButtonSetting(VOID);			// 필터 버튼 세팅.
	VOID	SetFilterMode( RwInt32 nIndex );	// 필터 버튼 클릭처리.

	VOID	ItemPositionSet( RwInt32 nIndex, CSkillWindowItemDlg* pItemDlg );
	VOID	SetScrollValue(VOID);

	VOID	UpdateActiveSkillItem( RwInt32 nSlot );
	VOID	UpdateSkillCanUpgrade( RwInt32 nSlot );
	VOID	UpdateActiveHTBItem( RwInt32 nSlot );
	VOID	UpdateActiveActionSkillItem( RwInt32 nSlot );
	VOID	UpdateAllViewItems(VOID);

	VOID	CoolingEffectProc( RwUInt32 hSerial, BOOL bStart );

	VOID	ShowSkillWindow(VOID);
	VOID	HideSkillWindow(VOID);

//! Event
	VOID	HandleEvents( RWS::CMsg& msg );
		
//! CallBack
	VOID	OnTabChanged( RwInt32 nIndex, RwInt32 nOldIndex );
	VOID	OnCloseButtonClicked( gui::CComponent* pComponent );
	VOID	OnScrollChanged( RwInt32 nNewOffset );
	
	VOID	OnFilterBtn0Clicked( gui::CComponent* pComponent, bool bDown );
	VOID	OnFilterBtn1Clicked( gui::CComponent* pComponent, bool bDown );
	VOID	OnFilterBtn2Clicked( gui::CComponent* pComponent, bool bDown );
	VOID	OnFilterBtn3Clicked( gui::CComponent* pComponent, bool bDown );

	VOID	OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	VOID	OnCaptureMouseDown( const CKey& key );

	VOID	OnClickedBtnHelp( gui::CComponent* pComponent );
	
	gui::CSlot			m_slotTabChanged;
	gui::CSlot			m_slotCloseButtonClicked;
	gui::CSlot			m_slotScrollChanged;
	gui::CSlot			m_slotScrollSliderMoved;
	
	gui::CSlot			m_slotFilterBtnClicked[4];

	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotCaptureMouseDown;
		
//! Variables
	SKILLMAP			m_mapBattle;
	SKILLMAP			m_mapAssist;
	SKILLMAP			m_mapAction;

	RwInt32				m_nTabScrollOffset[NUM_TAB];

	FILTERVEC			m_vecBattleFilter[NUM_BATTLE_FILTER];
	RwInt32				m_nBattleFilterScrollOffset[NUM_BATTLE_FILTER];

	FILTERVEC			m_vecAssistFilter[NUM_ASSIST_FILTER];
	RwInt32				m_nAssistFilterScrollOffset[NUM_ASSIST_FILTER];

	FILTERVEC			m_vecActionFilter[NUM_ACTION_FILTER];
	RwInt32				m_nActionFilterScrollOffset[NUM_ACTION_FILTER];

	CSkillWindowItemDlg* m_arrViewSet[MAX_VIEWABLE_ITEM];

	eTAB				m_eCurrentTab;

	eBATTLESKILLFILTER	m_eCurrentBattleFilter;
	eASSISTSKILLFILTER	m_eCurrentAssistFilter;
	eACTIONSKILLFILTER	m_eCurrentActionFilter;

	gui::CScrollBar*	m_pScrollBar;
	gui::CTabButton*	m_pTabButton;
	gui::CButton*		m_pCloseButton;
	gui::CStaticBox*	m_pTitle;
	
	gui::CButton*		m_pFilter[4];
	
	gui::CTexture*		m_pUnknownTexture;	

	// Help
	gui::CButton*		m_pBtnHelp;
	gui::CSlot			m_slotHelp;
};

#endif//__SKILL_WINDOW_GUI_H__