////////////////////////////////////////////////////////////////////////////////
// File: QuickSlotGui.h
// Desc: Skill�� Item�� ��ũ�� �����ϴ� GUI
//
// 2006.06.12 Peessi@hitel.net
// 2007.11.30 Kell - RpBonus Skill �߰�
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __QUICK_SLOT_GUI_H__
#define __QUICK_SLOT_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "SurfaceGui.h"
#include "NtlSobSkill.h"
#include "NtlSobIcon.h"

#define QUICKSLOT_MAX_VISIBLE_ROW	3
#define QUICKSLOT_MAXROW	4
#define QUICKSLOT_MAXCOL	12

class CNtlSob;
class CNtlSobAttr;
struct sSKILL_TBLDAT;
struct sACTION_TBLDAT;

class CQuickSlotItem
{
public:
	enum EnableState { ENABLE, DISABLE, ICONPICK_DISABLE, ICONPICK_ENABLE, CAPSULE_LOCK };
	
	typedef struct _stQUICKSLOTOBJ
	{
		RwUInt32		hSerial;
		CNtlSobIcon*	pIcon;

	}stQUICKSLOTOBJ;

	typedef std::list<stQUICKSLOTOBJ>				LIST_QUICKSLOTOBJ;
	typedef std::list<stQUICKSLOTOBJ>::iterator		LIST_QUICKSLOTOBJ_ITER;

	CQuickSlotItem(VOID);
	~CQuickSlotItem(VOID);

	CNtlSobIcon* GetFirstIcon(VOID);
	RwUInt32	 GetFirstSerialID(VOID);
	RwInt32		 GetEnableState(VOID) { return m_eEnableState; }
	RwUInt32	 GetUseableCheckFlag(VOID) { return m_flagUseableCheck; }
	RwUInt32	 GetSLClassID(VOID) { return m_uiSLClassID; }
	
	RwInt32		 GetStackCount(VOID);
	RwBool		 IsValid(VOID);
	RwBool		 IsItem(VOID);
	stQUICKSLOTOBJ* HasObj( RwUInt32 hSerialID );

	VOID		 SetItem( RwUInt32 hSerialID, CNtlSobIcon* pSetIcon, RwUInt8 byQuickSlotID );
	RwBool		 UnSetItem( RwUInt32 hSerialID );						// return FALSE : ���� ��������. TRUE : ������ ������
	VOID		 UnSetItem(VOID);
	VOID		 SetEnableState( RwInt32 eEnableState ) { m_eEnableState = eEnableState; }
	VOID		 SetUseableCheckFlag( RwUInt32 flagUseableCheck ) { m_flagUseableCheck = flagUseableCheck; }

	VOID		 operator = ( CQuickSlotItem& QuickSlotItem );

private:
	LIST_QUICKSLOTOBJ			m_listObj;
	RwInt32						m_eEnableState;
	RwUInt32					m_flagUseableCheck;
	RwUInt32					m_uiSLClassID;
	RwUInt8						m_byQuickSlotID;
};

// �ʱ� ���۽�, ������ ��Ͻ� ���� �������� ã�� ���� �ʿ�. 

class CQuickSlotGui : public CNtlPLGui, RWS::CEventHandler
{
public:
//! Enumuration
	//enum eICONTYPE { TYPE_NONE = 0, TYPE_SKILL, TYPE_ITEM };
	enum eFOCUSEFFECT { SLOT_FOCUS_NONE = 0x00, SLOT_FOCUS_CAN_MOVE = 0x01, SLOT_FOCUS = 0x02 };
	enum eUSEABLECHECKFLAG { CHECK_FLAG_LPEPRP = 0x01, CHECK_FLAG_TARGETSTATE = 0x02, CHECK_FLAG_ITEMNEED = 0x04, CHECK_FLAG_TARGETAPPLY = 0x08, CHECK_FLAG_AVATARSTATE = 0x10, CHECK_FLAG_ALL = 0xFFFFFFFF };
	enum eQUICKSLOTROW { ROW_DEFAULT = 0, ROW_EX, ROW_EX2, ROW_NUMS };
	
//! Constructor & Destrucor
	CQuickSlotGui(VOID);
	CQuickSlotGui( const RwChar *pName );
	~CQuickSlotGui(VOID);

//! Operator
	VOID	Init(VOID);
	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsed );

	RwInt32 SwitchDialog(bool bOpen);
	VOID	LinkActionMap();
	VOID	UnLinkActionMap();

	CPos	GetInitialRowPosOffset(VOID);	// Ʃ�丮�󿡼� ù��° ������ �������� ���ҋ�.

//! Event
	VOID	HandleEvents( RWS::CMsg& msg );
	RwInt32 ActionMapQuickSlotChange( uintptr_t uiParam );
	RwInt32 ActionMapQuickSlotUp( uintptr_t uiKey );
	RwInt32 ActionMapQuickSlotDown( uintptr_t uiKey );
	RwInt32 ActionMapQuickSlotExUp( uintptr_t uiKey );
	RwInt32 ActionMapQuickSlotExDown( uintptr_t uiKey );
	RwInt32 ActionMapQuickSlotEx2Up( uintptr_t uiKey );
	RwInt32 ActionMapQuickSlotEx2Down( uintptr_t uiKey );

private:
//! Implementation
	VOID	LoadQuickSlot(VOID);
	VOID	ClearQuickSlot(VOID);
	VOID	SetIconToQuickSlot( RwInt32 nRow, RwInt32 nCol, RwUInt32 hSerialID, CNtlSobIcon* pIcon );
	VOID	UnsetIconFromQuickSlot( RwInt32 nRow, RwInt32 nCol, RwUInt32 hSerialID = INVALID_SERIAL_ID, RwBool bSendPacket = TRUE );

	VOID	SendPacketToSetIcon( RwInt32 nRow, RwInt32 nCol, CNtlSobIcon* pIcon );

	RwInt32	GetQuickSlotIdx( RwInt32 nX, RwInt32 nY );
	RwBool	IsEnableClick( RwInt32 nSlotIdx );
	RwBool	IsEnablePutDown( RwInt32 nSlotIdx );
	
	VOID	ShowIconDestination(VOID);
	VOID	SetSlotRectHardCode(VOID);

	VOID	CreateShortCutKeyName();
	VOID	DestroyShortCutKeyName();

	VOID	RegisterShortCutkeyName();
	VOID	UpdateShortCutKeyName();

	VOID	IconPutDownProc(RwUInt32 hSerial, RwInt32 ePlace, RwInt32 nSlotIdx);
	VOID	IconUseProc( RwInt32 nSlotIdx );
	VOID	AddItemIcon( RwUInt32 hSerial );			
	VOID	DeleteItemIcon( RwUInt32 hSerial );
	VOID	EnableStateIcon( RwUInt32 hSerial, RwInt32 nEnableState );

	bool	CanPlaceItem(CNtlSobIcon* pIcon); //check if we can put the item into quick slot

	VOID	UpdateAllSlot( RwUInt32 flagUseableCheck = 0 );
	
	RwBool	CreateStackNumber( RwInt32 nSlotIdx, RwInt32 nValue );
	VOID	DestroyStackNumber( RwInt32 nSlotIdx );

	RwBool	CreateFlashEffect( RwInt32 nSlotIdx );
	VOID	DestroyFlashEffect( RwInt32 nSlotIdx );

	VOID	CoolingEffectProc( RwUInt32 hSerial, BOOL bStart );

	RwBool	ClickEffect( RwBool bPush, RwInt32 nSlotIdx = -1 );
	RwBool	KeyClickEffect( RwBool bPush, RwInt32 nSlotIdx );

	RwBool	UseableCheck_LPEPRP( CNtlSobAttr* pAttr );
	RwBool	UseableCheck_TargetState( sSKILL_TBLDAT* pData );
	RwBool	UseableCheck_ItemNeed( sSKILL_TBLDAT* pData );
	RwBool	UseableCheck_TargetApply( sSKILL_TBLDAT* pData );
	RwBool	UseableCheck_TargetApply( sACTION_TBLDAT* pData );
	RwBool  UseableCheck_AvatarState( sSKILL_TBLDAT* pData );
	RwBool	UseableCheck_AvatarState(VOID);
	
	RwBool	Check_EquippedSlot( RwInt32 nEquipSlot, RwUInt8 byRequireItemType );
	RwInt32 GetInfoWndType( RwUInt8 uiSLClassID );

	RwInt32	GetDataRowIndex( RwInt32 nVisibleRowIndex );
	RwInt32 GetVisibleRowIndex( RwInt32 nDataRowIndex );
	RwBool	IsValidRowIndex( RwInt32 nRowIndex );
	RwBool	IsValidColIndex( RwInt32 nColIndex );
	RwBool	IsValidVisibleRowIndex( RwInt32 nVisibleRowIndex );

	RwBool	IsValidSlotIdx( RwInt32 nSlotIdx );
	RwBool	IsValidVisibleSlotIdx( RwInt32 nVisibleSlotIdx );

	RwBool	IsVisibleRow( RwInt32 nDataRow );

	VOID	SetVisibleRowCount( RwInt32 nVisibleRowCount );

	RwBool	CheckIconMoveLock(VOID);
	VOID	SwapKeyInputData( RwInt32 nCurrentVisibleCount, RwInt32 nChangeVisibleCount );
			
//! CallBack
	VOID	OnClickedUpRow( gui::CComponent* pComponent );
	VOID	OnClickedDownRow( gui::CComponent* pComponent );
	VOID	OnMouseDown( const CKey& key );
	VOID	OnMouseUp( const CKey& key );
	VOID	OnMouseMove( RwInt32 nFlags, RwInt32 nX, RwInt32 nY );
	VOID	OnPaint(VOID);
	VOID	OnMouseLeave( gui::CComponent* pComponent );
	VOID	OnMove( RwInt32 nX, RwInt32 nY );

	gui::CSlot	m_slotUpRowClick;
	gui::CSlot	m_slotDownRowClick;
	gui::CSlot	m_slotMouseUp;
	gui::CSlot  m_slotMouseDown;
	gui::CSlot	m_slotMouseMove;
	gui::CSlot	m_slotPaint;
	gui::CSlot  m_slotMouseLeave;
	gui::CSlot	m_slotMove;

	CNtlPLEntity* pEffectRange;

//! Variables
	CQuickSlotItem m_aQuickSlotItem[QUICKSLOT_MAXROW][QUICKSLOT_MAXCOL];
	
	RwInt32		m_nCurrentRow;
	RwInt32		m_nVisibleRowCount;

	CRectangle	m_artQuickSlot[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];
	CSurfaceGui m_asurSlot[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];
	CSurfaceGui	m_asurIcon[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];	
	CSurfaceGui m_asurFocusSlot[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];
	CSurfaceGui	m_asurDisableSlot[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];
	CSurfaceGui	m_asurDisableSkill[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];
	CSurfaceGui m_asurRPType[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];
	RwInt32		m_anFocusEffect[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];

	/*CSurfaceGui m_asurKeyImage[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];*/
		
	RwInt32		m_nLSelectedSlotIdx;
	RwInt32		m_nRSelectedSlotIdx;
	RwInt32		m_nMouseOnIndex;
	
	RwInt32		m_nPushDownIndex;							// ���콺 �ٿ�� Index
	
	// RpBonus
	RwInt32		m_abPushDownKey[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];			// Ű ���� ������ ����
	RwReal		m_afPushDownKeyElapsed[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];	// Ű ���� �������� TRUE���� ����ð�
	RwBool		m_abIsProcDownKey[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];		// ���� �˻����� �������� ó���� �Ǿ��� ���ΰ�?
	RwInt32		m_abEnableIgnoreUp[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];		// TRUE�� Key Up�� IconProc�� ó�� ����

	RwBool		m_bIsProcMouseSelected;
	RwInt32		m_nEnableIgnoreIndex;						// ���콺 Up�� IconProc�� ó�� ���� Index
	RwReal		m_fPushDownMouseElapsed;					// ���콺 Elapsed
		
	/*gui::CPanel*	m_ppnlAlt;
	gui::CPanel*	m_ppnlCtrl;*/
	gui::CButton*	m_pUpRowBtn;
	gui::CButton*	m_pDownRowBtn;
	gui::CStaticBox* m_pRowNum;
	gui::CStaticBox* m_apstbStackNum[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];

	// AttckMode���� �÷��� ����Ʈ
	RwBool			 m_bAttackMode;
	RwUInt32		 m_uiEffectFrame;	// �����ӵ��⸦ ���� ���.
	gui::CFlash*	 m_apflaEffect[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];
	
	// ����Ű �̸��� ������ִ� Comopnent
	gui::CStaticBox* m_apStbShortCutName[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];
	RwInt32			 m_anQuickSlotRow[ROW_NUMS];
	
	gui::CRadarEffect m_CoolTimeEffect[QUICKSLOT_MAX_VISIBLE_ROW][QUICKSLOT_MAXCOL];

	RwBool		m_bLock;
	RwBool		m_bIconMoveLock;							// �������� �������� �����. 
};

#endif