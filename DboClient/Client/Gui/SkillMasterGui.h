////////////////////////////////////////////////////////////////////////////////
// File: SkillMasterGui.h
// Desc: SkillMaster Gui. 기존 스킬윈도우에서 분리.
//
// 2007.9.3 Peessi@hitel.net   
// To Be Desired:
////////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_MASTER_GUI_H__
#define __SKILL_MASTER_GUI_H__

#define MAX_VIEWABLE_ITEM	5

#include "SurfaceGui.h"
#include "SkillCommon.h"
#include "ceventhandler.h"

class  CSkillMasterGui;

class CSkillMasterItemDlg
{
public:
	//! Enum
	enum eMOUSEFOCUS { MOUSE_NONE, MOUSE_ONICON, MOUSE_ONRQ };
	
	//! Constructor & Destuctor
	CSkillMasterItemDlg( CSkillMasterGui* pSkillWnd );
	~CSkillMasterItemDlg(VOID);

	//! Attributes
	CSkillGuiItem* GetSkillItem(VOID) { return m_pItem; }
	bool	IsShow(VOID) { return m_pThis->IsVisible(); }
	bool	IsEnable(VOID) { return m_pThis->IsEnabled(); }
	CRectangle GetPosition(VOID) { return m_pThis->GetPosition(); }

	//! Operation
	VOID	UpdateData( CSkillGuiItem* pItem );
	VOID	SetSkillItem( CSkillGuiItem* pItem ) { m_pItem = pItem; }

	VOID	Show( bool bShow ) { m_pThis->Show( bShow ); }
	VOID	Enable( bool bEnable ) { m_pThis->Enable( bEnable ); }
	VOID	SetPosition( RwInt32 nX, RwInt32 nY ) { m_pThis->SetPosition( nX, nY ); }
	VOID	SetPosition( const CRectangle& rect ) { m_pThis->SetPosition( rect ); }

	VOID	SetColor( RwUInt8 ucRed, RwUInt8 ucGreen, RwUInt8 ucBlue );
	VOID	SetAlpha( RwUInt8 ucAlpha );

private:
	//! Implementation
	VOID	UpdateSkillData( CSkillGuiItem* pItem );
	VOID	UpdateHTBData( CSkillGuiItem* pItem );
	/// woosungs_test 20090730	del
	//RwUInt32 ExmineRQBySkill(VOID);
	//RwUInt32 ExmineRQByHTB(VOID);

	//! CallBack
	VOID	OnMouseLeave( gui::CComponent* pComponent );
	VOID	OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );	
	
	VOID	OnLearnButtonEnter( gui::CComponent* pComponent );
	VOID	OnLearnButtonLeave( gui::CComponent* pComponent );
	VOID	OnLearnButtonClick( gui::CComponent* pComponent );
	
	gui::CSlot  m_slotMouseLeave;
	gui::CSlot	m_slotMouseMove;
	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotMove;
	
	gui::CSlot	m_slotLearnButtonEnter;
	gui::CSlot	m_slotLearnButtonLeave;
	gui::CSlot	m_slotLearnButtonClick;
	gui::CSlot	m_slotRPModeClick;
	
	//! Variables
	CRectangle			m_rtIcon;		// 아이콘 영역
	CSurfaceGui			m_surIcon;		// 아이콘 그림
	
	gui::CStaticBox*	m_pTitle;
	gui::CMDStaticBox*	m_pZennySpText;
	gui::CStaticBox*	m_pEffectText;
	//gui::CStaticBox*	m_pConsumeTitle;
	gui::CStaticBox*	m_pHelpText;
	gui::CDialog*		m_pThis;

	gui::CButton*		m_pbtnLearn;
	gui::CPanel*		m_ppnlZennyMark;

	eMOUSEFOCUS			m_eMouseOn;
	RwUInt32			m_flagRQState;

	CSkillGuiItem*		m_pItem;

	RwUInt32			m_ucColorRed;	// 다이얼로그 전체 색. 
	RwUInt32			m_ucColorGreen;
	RwUInt32			m_ucColorBlue;
	RwUInt32			m_ucAlpha;

	CSurfaceGui			m_surFocusEffect;		
	
	CSkillMasterGui*	m_pSkillMasterWnd;
};

class CSkillMasterGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	//! Enum
	enum eTAB				{ NOT_TAB = -1, BATTLE_TAB, ASSIST_TAB, NUM_TAB };
	enum eBATTLESKILLFILTER	{ NO_BATTLE_FILTER = -1, BATTLE_FILTER = 0, HTB_FILTER, NUM_BATTLE_FILTER };
	enum eASSISTSKILLFILTER	{ NO_ASSIST_FILTER = -1, ASSIST_FILTER = 0, PASSIVE_FILTER, NUM_ASSIST_FILTER };
	
	typedef std::map<RwUInt32,CSkillGuiItem*>	SKILLMAP;
	typedef SKILLMAP::iterator					SKILLMAP_IT;

	typedef std::vector<CSkillGuiItem*>			FILTERVEC;
	typedef FILTERVEC::iterator					FILTERVEC_IT;	

	//! Constructor & Destructor
	CSkillMasterGui(VOID);
	CSkillMasterGui( const RwChar* pName );
	~CSkillMasterGui(VOID);

	//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	
	RwInt32		GetCurrentTab(VOID) { return (RwInt32)m_eCurrentTab; }
	CNtlSobNpc* GetSkillMaster(VOID) { return m_pSkillMaster; }
	RwInt32		SwitchDialog( bool bOpen);

private:
	//! Implementation
	VOID	Init(VOID);
	VOID	ShowItemDetect(VOID);
	VOID	SetItems( RwUInt32 hNPCSerial );// Skill Master 클릭시
	VOID	ClearItems(VOID);
	VOID	ClearItemDialogs(VOID);	

	VOID	AddSkillMapData( SKILLMAP* pMap, CSkillGuiItem* pItem );
	CSkillGuiItem* GetSkillMapData( SKILLMAP* pMap, RwUInt32 idx );

	VOID	SetFilterItems(VOID);			// Filter 세팅.
	VOID	FilterButtonSetting(VOID);		// 필터 버튼 세팅.
	VOID	SetFilterMode( RwInt32 nIndex );// 필터 버튼 클릭처리.

	VOID	ItemPositionSet( RwInt32 nIndex, CSkillMasterItemDlg* pItemDlg );
	VOID	SetScrollValue(VOID);

	//VOID	UpdateActiveSkillItem( RwInt32 nSlot );
	//VOID	UpdateActiveHTBItem( RwInt32 nSlot );	
	VOID	UpdateAllViewItems(VOID);

	VOID	ShowSkillMaster(VOID);
	VOID	HideSkillMaster(VOID);

	VOID	SetSP(VOID);

	RwBool	IsEnableShowSkillMasterMaster( CNtlSobNpc* pNPC );

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
	
	RwInt32				m_nTabScrollOffset[NUM_TAB];

	FILTERVEC			m_vecBattleFilter[NUM_BATTLE_FILTER];
	RwInt32				m_nBattleFilterScrollOffset[NUM_BATTLE_FILTER];

	FILTERVEC			m_vecAssistFilter[NUM_ASSIST_FILTER];
	RwInt32				m_nAssistFilterScrollOffset[NUM_ASSIST_FILTER];

	CSkillMasterItemDlg* m_arrViewSet[MAX_VIEWABLE_ITEM];

	eTAB				m_eCurrentTab;

	eBATTLESKILLFILTER	m_eCurrentBattleFilter;
	eASSISTSKILLFILTER	m_eCurrentAssistFilter;	

	gui::CScrollBar*	m_pScrollBar;
	gui::CTabButton*	m_pTabButton;
	gui::CButton*		m_pCloseButton;
	gui::CStaticBox*	m_pTitle;
	gui::CStaticBox*	m_pSP;

	gui::CButton*		m_pFilter[4];	

	CNtlSobNpc*			m_pSkillMaster;
};

#endif//__SKILL_MASTER_GUI_H__