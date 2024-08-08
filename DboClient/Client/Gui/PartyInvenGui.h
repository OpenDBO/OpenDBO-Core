/******************************************************************************
* File			: PartyInvenGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Abstract		: 2006. 10. 30
*****************************************************************************
* Desc			: 파티 플레이시 분배 방식에 따라 일정 등급 이상의 아이템은 
*				  파티 인벤에 보관된다
*
*				  2009. 4. 28 : 기획쪽(근홍씨) 요청으로 파티인벤 기능을 쓰지
*				  않기로 했다. CGameGuiGroup에서 CPartyInvenGui객체를
*				  생성 자체를 하지 않는다
*****************************************************************************/

// core
#include "ceventhandler.h"

// share
#include "NtlParty.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// cleint
#include "SlotGui.h"

class CPartyInvenGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
#define dITEM_GRADE_IN_PARTY_INVEN_USED		ITEM_RANK_COUNT - 1

	enum eInvenDivideType
	{
		INVEN_DIVIDE_TYPE_BY_LEADER,
		INVEN_DIVIDE_TYPE_BY_OCTION,
		INVEN_DIVIDE_TYPE_BY_DICE,

		NUM_INVEN_DIVIDE_TYPE,

		INVALID_DIVIDE_TYPE
	};

	struct sPartyInvenSlot
	{
		CRegularSlotGui			slot;			///< 아이템 슬롯
		sITEM_PROFILE		ItemProfile;	///< InfoWindow 에서 보여주기 위한 정보
	};

	struct sDivide_by_leader
	{
		gui::CStaticBox*	pMemberName[NTL_MAX_MEMBER_IN_PARTY];		///< 파티원 이름
		gui::CButton*		pSendItemButton[NTL_MAX_MEMBER_IN_PARTY];	///< 파티원에게 아이템 주기 버튼
		gui::CSlot			slotButton[NTL_MAX_MEMBER_IN_PARTY];

		SERIAL_HANDLE		hSerial[NTL_MAX_MEMBER_IN_PARTY];			///< 파티원 시리얼 핸들
	};	

	struct sDivide_by_Oction
	{
		struct sSlotInvest
		{
			SERIAL_HANDLE		hSerial;			///< 파티원 시리얼 핸들
			RwUInt32			uiZenny;			///< 투자된 제니
			RwBool				bBestisAvatar;		///< 자신이 가장 많이 투자했는지 여부
			CSurfaceGui			srfOthersInvestedIcon;	///< 남이 가장 많이 투자했을시의 아이콘
			CSurfaceGui			srfMyInvestedIcon;		///< 자신이 가장 많이 투자했을시의 아이콘
		};

		gui::CStaticBox*	pBidedMember;				///< 현재 가장 많은 돈을 투자한 파티원
		gui::CStaticBox*	pBidedZenny;				///< 현재 가장 많이 투자된 돈

		gui::CButton*		pBidedZennyButton;			///< 현재 투자된 가장 많은 돈 및 투자 버튼
		gui::CButton*		pOctionUnLockButton;		///< 경매 언락 버튼
		gui::CButton*		pOctionLockButton;			///< 경매 락 버튼
		gui::CButton*		pBidButton;					///< 낙찰 버튼
		gui::CButton*		pResetButton;				///< 초기화 버튼

		CSurfaceGui			srfZennySlotDestination;	///< 제니 슬롯 Destination 이미지

		gui::CSlot			slotBidedZennyButton;
		gui::CSlot			slotOctionUnLockButton;
		gui::CSlot			slotOctionLockButton;
		gui::CSlot			slotBidButton;
		gui::CSlot			slotResetButton;

		CSurfaceGui			srfOctionBack;				///< 경매창 배경

		RwBool				bOctionLock;				///< 경매중인가 여부
		RwBool				bZennySlotDestination;
		sSlotInvest			slotInvest[NTL_PARTY_INVENTORY_SLOT_COUNT];	///< 슬롯 경매 투자 정보
	};

	struct sDivide_by_Dice
	{
		gui::CButton*		pDiceButton;	///< 주사위 굴리기 버튼
		gui::CSlot			slotButton;

		RwBool				bRollingDice;	///< 주사위 굴리기를 시작했는지 여부
	};
	
	CPartyInvenGui(const RwChar* pName);
	virtual ~CPartyInvenGui();

	static VOID		CreateInstance();
	static VOID		DestroyInstance();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CPartyInvenGui() {}
	virtual VOID HandleEvents( RWS::CMsg &msg );	

	VOID		Clear();
	VOID		CheckInfoWindow();

	VOID		ActivateDivideMethod(eInvenDivideType eType);
	VOID		ActivateDivideMethod_by_leader(bool bActive);
	VOID		ActivateDivideMethod_by_Oction(bool bActive);
	VOID		ActivateDivideMethod_by_Dice(bool bActive);

	VOID		EnableConrollers(bool bEnable);
	VOID		CalcInvestedAvatarZenny();

	VOID		OnClicked_MinimizeButton(gui::CComponent* pComponent);
	VOID		OnClicked_ItemGradeToggleButton(gui::CComponent* pComponent, bool bToggled);
	VOID		OnClicked_ChangeMethodDivideButton(gui::CComponent* pComponent);
	VOID		OnClicked_SendItemButton(gui::CComponent* pComponent);
	VOID		OnClicked_DiceButton(gui::CComponent* pComponent);
	VOID		OnClicked_Oction_ZennyButton(gui::CComponent* pComponent);
	VOID		OnClicked_Oction_LockUnLockButton(gui::CComponent* pComponent);
	VOID		OnClicked_Oction_BidButton(gui::CComponent* pComponent);
	VOID		OnClicked_Oction_ResetButton(gui::CComponent* pComponent);

	RwInt8		PtinSlot(RwInt32 iX, RwInt32 iY);
	VOID		FocusEffect(RwBool bPush, RwInt32 iSlotIdx = -1);
	VOID		SelectEffect(RwInt32 iSlot);

	VOID		OnMouseDown(const CKey& key);
	VOID		OnMouseUp(const CKey& key);
	VOID		OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID		OnMouseLeave(gui::CComponent* pComponent);
	VOID		OnCaptureMouseDown(const CKey& key);
	VOID		OnPaint();
	VOID		OnPostPaint();

protected:
	static CPartyInvenGui*	m_pInstance;

	gui::CSlot			m_slotMinimizeButton;
	gui::CSlot			m_slotItemGradeButton[dITEM_GRADE_IN_PARTY_INVEN_USED];
	gui::CSlot			m_slotLeftButton;
	gui::CSlot			m_slotRightButton;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;
	
	RwInt8				m_byInfoWindowIndex;///< Info Window를 보여주고 있는 슬롯의 인덱스
	RwInt32				m_iMouseDownSlot;	///< 마우스로 눌린 슬롯의 인덱스
	RwInt32				m_iSelectedSlot;	///< 선택된 슬롯의 인덱스
	RwBool				m_bFocus;			///< 포커스를 얻었다.	

	gui::CStaticBox*	m_pDialogName;		///< 다이얼로그의 이름 스태틱
	gui::CStaticBox*	m_pDivideMethod;	///< 현재 분배 방식 설명

	gui::CButton*		m_pMinimizeButton;	///< 최소화 버튼
	gui::CButton*		m_pItemGradeButton[dITEM_GRADE_IN_PARTY_INVEN_USED];	///< 아이템 등급 버튼
	gui::CButton*		m_pLeftButton;		///< 파티 인벤 분배 방식 변경(이전)
	gui::CButton*		m_pRightButton;		///< 파티 인벤 분배 방식 변경(이후)

	CSurfaceGui			m_srfSlots;			///< 파티 인벤토리에 담긴 아이템들의 배경 슬롯
	CSurfaceGui			m_srfDivideMethodBack;	///< 분배 방식 설명의 배경
	CSurfaceGui			m_FocusEffect;		///< 슬롯 포커스 이펙트
	CSurfaceGui			m_SelectEffect;		///< 슬롯 셀렉트 이펙트	

	sPartyInvenSlot		m_SlotInfo[NTL_PARTY_INVENTORY_SLOT_COUNT];	///< 슬롯 정보

	eInvenDivideType	m_eInvenType;

	sDivide_by_leader	m_Divide_by_Leader;	///< 파티장에 의한 분배일 때의 정보
	sDivide_by_Oction	m_Divide_by_Oction;	///< 경매에 의한 분배일 때의 정보
	sDivide_by_Dice		m_Divide_by_Dice;	///< 주사위 굴리기에 의한 분배일 때의 정보
};