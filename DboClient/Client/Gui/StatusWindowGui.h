////////////////////////////////////////////////////////////////////////////////
// File: StatusWindowGui.h
// Desc: StatusWindow 
//
// 2006.04.24 Peessi@hitel.net   
//
// To Be Desired:
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __STATUS_WINDOW_GUI_H__
#define __STATUS_WINDOW_GUI_H__

#include "InfoWndManager.h"
#include "GuiLineTree.h"

#define dSTATUSWINDOW_LIST_SCROLL_WIDTH		12
#define dSTATUSWINDOW_LIST_SCROLL_HEIGHT	19
#define dSTATUSWINDOW_LIST_SLIDER_WIDTH		12
#define dSTATUSWINDOW_LIST_SLIDER_HEIGHT	39

#define dSTATUSWINDOW_CATEGORY_BUTTON_X			0
#define dSTATUSWINDOW_CATEGORY_BUTTON_Y			0
#define dSTATUSWINDOW_CATEGORY_BUTTON_WIDTH		140
#define dSTATUSWINDOW_CATEGORY_BUTTON_HEIGHT	20

#define dSTATUSWINDOW_CONTENTS_X			0
#define dSTATUSWINDOW_CONTENTS_Y			0
#define dSTATUSWINDOW_CONTENTS_WIDTH		300
#define dSTATUSWINDOW_CONTENTS_HEIGHT		200

//class CGuiLineTree;
//class CGuiLineTreeNode;

class CStatusAvatarTab
{
public:
	//! Enumeration
	enum	eFOCUSEFFECT { SLOT_FOCUS_NONE = 0x00, SLOT_FOCUS_CAN_MOVE = 0x01, SLOT_FOCUS = 0x02 };
	enum	eFRITZEFFECT { FRITZ_NONE, FRITZ_HALF, FRITZ_ABSOLUTE };
	enum	eTYPE_BATTLEATTR { TYPE_WEAPON, TYPE_ARMOR, TYPECOUNT };

	enum	ePCINFO { NAME, CLASS, LEVEL, AP, NUM_PCINFO };
	enum	eSTAT { LP, EP, EXP_CUR, EXP_MAX, NUM_STAT };
	enum	eBASICSTAT { STR, DEX, CON, ENG, SOL, FOC, NUM_BASIC_STAT };
	enum	eCOMBATSTAT { OFFENCE, DEFENCE, CRITICAL, NUM_COMBAT_STAT };
	enum	eETCSTAT { ATTACKRATE, DODGE, NUM_ETC_STAT };
	
	//! Contructor & Destructor
	CStatusAvatarTab(VOID);
	~CStatusAvatarTab(VOID);

	//! Attribute
	RwBool	IsShow(VOID);
	
	//! Operation
	RwBool	Create( CNtlPLGui* pParent );
	VOID	Destroy(VOID);

	VOID	UpdateBeforeCamera( RwReal fElapsed );
	
	VOID	Refresh(VOID) { UpdateEquipData(); UpdateStatData(); CalcBattleAttribute(); }
	VOID	Show( bool bShow ) { m_pSelf->Show( bShow ); }

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	//! Implementation
	RwInt32		GetEquipSlotIdx( RwInt32 nX, RwInt32 nY );
	CRectangle  GetEquipSlotRect( RwInt32 nSlotIdx );	
	const WCHAR*	GetEquipSlotText( RwInt32 nSlotIdx );

	RwBool	IsEnableEquipPickUp( RwInt32 nSlotIdx );
	RwBool	IsEnableEquipPutDown( RwInt32 nSlotIdx );
	RwBool	IsEnableUnequipItem( RwInt32 nSlotIdx );
	//RwBool	IsEnableEquipPopup( RwInt32 nSlotIdx );

	VOID	UpdateEquipData(VOID);
	VOID	UpdateStatData(VOID);

	VOID	ShowIconDestination(VOID);
	VOID	SetSlotRectHardCode(VOID);
	VOID	SetTextHardCode(VOID);
	VOID	CalcBattleAttribute(VOID);
	VOID	SetBattleAttribute( eTYPE_BATTLEATTR eType, RwUInt8 byBattleAttribute );

	VOID	ClickEffect( RwBool bPush, RwInt32 nSlotIdx = -1 );
	VOID	FritzEffect( eFRITZEFFECT eFritzState, RwInt32 nSlotIdx );
	VOID	ShowPickedUp( RwBool bShow, RwInt32 nSlotIdx = 0 );
	VOID	ShowDisableSlot( RwBool bShow, RwInt32 nSlotIdx );

	RwBool	IsDisableSlot( RwInt32 nSlotIdx );

	VOID	SetLPInfoWnd(VOID);
	VOID	SetEPInfoWnd(VOID);
	VOID	SetEXPInfoWnd(VOID);

	VOID	SetAttributeToolTip(VOID);

private:
	VOID	OnMouseDown( const CKey& key );
	VOID	OnMouseUp( const CKey& key );
	VOID	OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY );
	VOID	OnPaint(VOID);
	VOID	OnSlotPaint(VOID);
	VOID	OnMove( RwInt32 nX, RwInt32 nY );
	VOID	OnMouseLeave( gui::CComponent* pComponent );

	VOID	OnLeftRotBtnPress( gui::CComponent* pComponent );
	VOID	OnLeftRotBtnRelease( gui::CComponent* pComponent );
	VOID	OnRightRotBtnPress( gui::CComponent* pComponent );
	VOID	OnRightRotBtnRelease( gui::CComponent* pComponent );
	VOID	OnDragRotBtnPress( gui::CComponent* pComponent );
	VOID	OnDragRotBtnRelease( gui::CComponent* pComponent );
	VOID	OnDragRotMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY );
	
	gui::CSlot  m_slotMouseDown;
	gui::CSlot	m_slotMouseUp;
	gui::CSlot	m_slotMouseMove;
	gui::CSlot	m_slotPaint;
	gui::CSlot	m_slotSlotPaint;
	gui::CSlot	m_slotMove;
	gui::CSlot  m_slotMouseLeave;

	gui::CSlot	m_slotLeftRotBtnPress;
	gui::CSlot	m_slotLeftRotBtnRelease;
	gui::CSlot	m_slotRightRotBtnPress;
	gui::CSlot	m_slotRightRotBtnRelease;
	gui::CSlot	m_slotDragRotBtnPress;
	gui::CSlot	m_slotDragRotBtnRelease;
	gui::CSlot	m_slotDragRotMouseMove;
	
	//! Variables
	CRectangle	m_rtAvatar;
	CRectangle	m_rtEquipSlot[NTL_MAX_EQUIP_ITEM_SLOT];

	CSurfaceGui	m_surIcon[NTL_MAX_EQUIP_ITEM_SLOT];	
	CSurfaceGui m_surFocusSlot[NTL_MAX_EQUIP_ITEM_SLOT];
	RwInt32		m_arrFocusEffect[NTL_MAX_EQUIP_ITEM_SLOT];
	CSurfaceGui m_surDisableSlot[NTL_MAX_EQUIP_ITEM_SLOT];
	CSurfaceGui m_surFritzSlot[NTL_MAX_EQUIP_ITEM_SLOT];	// 내구도가 다한녀석들 표시.

	CSurfaceGui m_surPickedUp;
	RwInt32	    m_nShowPickedUp;

	RwInt32		m_nLSelectedSlotIdx;
	RwInt32		m_nRSelectedSlotIdx;
	RwInt32		m_nMouseOnIndex;
	RwInt32		m_nPushDownIndex;

	RwInt32		m_nDragRotX;

	CSurfaceGui		m_surCharacter;
	gui::CTexture	m_texCharacter;

	gui::CStaticBox*	m_pstbPCInfo[NUM_PCINFO];
	gui::CStaticBox*	m_pstbPCInfoTitle[NUM_PCINFO];
	
	gui::CStaticBox*	m_pstbStat[NUM_STAT];

	gui::CStaticBox*	m_pstbBasicStat[NUM_BASIC_STAT];
	gui::CStaticBox*	m_pstbBasicStatTitle[NUM_BASIC_STAT];

	gui::CStaticBox*	m_pstbPhysicalCombatStat[NUM_COMBAT_STAT];
	gui::CStaticBox*	m_pstbPhysicalCombatStatTitle[NUM_COMBAT_STAT];

	gui::CStaticBox*	m_pstbEnergyCombatStat[NUM_COMBAT_STAT];
	gui::CStaticBox*	m_pstbEnergyCombatStatTitle[NUM_COMBAT_STAT];

	gui::CStaticBox*	m_pstbETCStat[NUM_ETC_STAT];
	gui::CStaticBox*	m_pstbETCStatTitle[NUM_ETC_STAT];

	gui::CButton*		m_pbtnLeftRot;
	gui::CButton*		m_pbtnRightRot;
	gui::CButton*		m_pbtnDragRot;
	
	gui::CStaticBox*	m_pstbBattleAttrTitle;
	gui::CPanel*		m_ppnlBattleAttribute[TYPECOUNT];	

	gui::CDialog*		m_pSelf;

protected:
	stINFOWND_BATTLEATTR	m_sBattleAttr;

	gui::CSlot			m_slotWeaponMouseEnter;
	gui::CSlot			m_slotWeaponMouseLeave;
	gui::CSlot			m_slotArmorMouseEnter;
	gui::CSlot			m_slotArmorMouseLeave;

	VOID	OnMouseWeaponAttrEnter(gui::CComponent* pComponent);
	VOID	OnMouseWeaponAttrLeave(gui::CComponent* pComponent);
	VOID	OnMouseArmorAttrEnter(gui::CComponent* pComponent);
	VOID	OnMouseArmorAttrLeave(gui::CComponent* pComponent);

	VOID	SetSourceAttr( RwUInt8 bySourceWeaponAttr, RwUInt8 bySourceArmorAttr );
	VOID	SetTargetAttr( RwUInt8 byTargetWeaponAttr, RwUInt8 byTargetArmorAttr );

	VOID	OnBattleAttributeRefresh();
};

/**
* \ingroup client
* \brief 스테이터스창의 명예 탭
*/
class CStatusHonorTab
{
public:
	CStatusHonorTab();
	~CStatusHonorTab();

	RwBool IsShow(VOID);
	
	//! Operation
	VOID	Init();
	RwBool	Create( CNtlPLGui* pParent );
	VOID	Destroy(VOID);

	VOID	UpdateUI();
	VOID	Refresh(VOID);
	VOID	Show( bool bShow ) 
	{ 
		m_pSelf->Show( bShow ); 
		m_pBtnExpandHonor->Show( false );
		m_pBtnReduceHonor->Show( false );
		m_pHtmlHonor->Show( false ); 
	}

	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	VOID	OnClickedBtnExpandHonor(gui::CComponent* pComponent);
	VOID	OnClickedBtnReduceHonor(gui::CComponent* pComponent);
	VOID	OnClickedBtnExpandRankBattle(gui::CComponent* pComponent);
	VOID	OnClickedBtnReduceRankBattle(gui::CComponent* pComponent);

private:
	gui::CDialog*		m_pSelf;
	/*CStatusHonorList*	m_pListHonor;*/

	gui::CButton*		m_pBtnExpandHonor;
	gui::CButton*		m_pBtnReduceHonor;
	gui::CButton*		m_pBtnExpandRankBattle;
	gui::CButton*		m_pBtnReduceRankBattle;

	RwBool				m_bExpandHonor;
	RwBool				m_bExpandRankBattle;

	gui::CSlot			m_slotClickedBtnExpandHonor;
	gui::CSlot			m_slotClickedBtnReduceHonor;
	gui::CSlot			m_slotClickedBtnExpandRankBattle;
	gui::CSlot			m_slotClickedBtnReduceRankBattle;

	gui::CHtmlBox*		m_pHtmlHonor;
	gui::CHtmlBox*		m_pHtmlRankBattle;
};

/**
* \ingroup client
* \brief 스테이터스창의 Technic 탭
*/
class CStatusTechnicTab
{
public:
	CStatusTechnicTab();
	~CStatusTechnicTab();

	RwBool IsShow(VOID);

	//! Operation
	VOID	Init();
	RwBool	Create( CNtlPLGui* pParent );
	VOID	Destroy(VOID);

	VOID	Show( bool bShow )	{ m_pSelf->Show( bShow ); }
	
	//! Event
	VOID	HandleEvents( RWS::CMsg& msg );

	VOID	UpdateUI(VOID);
	VOID	Refresh(VOID);

	VOID	OnClickedBtnExpandHoipoiMix(gui::CComponent* pComponent);
	VOID	OnClickedBtnReduceHoipoiMix(gui::CComponent* pComponent);
	
private:
	gui::CDialog*		m_pSelf;
	
	gui::CButton*		m_pBtnExpandHoipoiMix;
	gui::CButton*		m_pBtnReduceHoipoiMix;

	bool				m_bHoipoiMixExpand;

	gui::CDialog*		m_pHoipoiMixDlg;
	
	gui::CSlot			m_slotClickedBtnExpandHoipoiMix;
	gui::CSlot			m_slotClickedBtnReduceHoipoiMix;

	gui::CStaticBox*	m_pStbMixLevel;
	gui::CStaticBox*	m_pStbMixLevelData;
	gui::CStaticBox*	m_pStbMixExp;
	gui::CStaticBox*	m_pStbMixExpData;
	gui::CProgressBar*	m_pPgbExpData;

	gui::CStaticBox*	m_pStbNormalMix;
	gui::CStaticBox*	m_pStbNormalRequireGuide;
	gui::CHtmlBox*		m_pHtmlNormalGuide;

	gui::CStaticBox*	m_pStbSpecialMix;
	gui::CStaticBox*	m_pStbSpecialRequireGuide;
	gui::CHtmlBox*		m_pHtmlSpecialGuide;
};

class CStatusWindowGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
//! Enum
	enum TAB { TAB_AVATAR, TAB_HONOR, TAB_TECHNIC };

//! Contructor & Destructor
	CStatusWindowGui(VOID);
	CStatusWindowGui( const RwChar* pName );
	~CStatusWindowGui(VOID);

//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	UpdateBeforeCamera(RwReal fElapsed);

//! Event
	VOID	HandleEvents( RWS::CMsg& msg );
	RwInt32	SwitchDialog( bool bOpen );

private:
//! Implements
	VOID	SetBasicUISetting(VOID);

	VOID	SelectTab( RwInt32 nIndex );

//! Callbacks
	VOID	OnClickedExit( gui::CComponent* pComponent );
	VOID	OnSelectedTab( RwInt32 nSelectIndex, RwInt32 nPreviousIndex );
	VOID	OnAbsolulteClick( const CKey& key );
	VOID	OnClickedHelp( gui::CComponent* pComponent );

	gui::CSlot			m_slotSelectTab;
	gui::CSlot			m_slotExit;
	gui::CSlot			m_slotCaptureMouseDown;

	gui::CStaticBox*	m_pstbTitle;
	gui::CTabButton*	m_ptabStatus;
	gui::CButton*		m_pbtnExit;

	gui::CButton*		m_pBtnHelp;
	gui::CSlot			m_slotHelp;

	CStatusAvatarTab*	m_pAvatarTab;
	CStatusHonorTab*	m_pHonorTab;
	CStatusTechnicTab*	m_pTechnicTab;

	RwInt32				m_nCurrentTab;
};

#endif//__STATUS_WINDOW_GUI_H__
