/******************************************************************************
* File			: PartyCharmGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Abstract		: 2006. 10. 23
*****************************************************************************
* Desc			: 파티 부적을 등록하여 발동시키기 위한 GUI
*****************************************************************************/

// core
#include "ceventhandler.h"

// shared
#include "NtlParty.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLParty.h"
#include "NtlSLDef.h"

// dbo
#include "SlotGui.h"

struct sPartyMember;

class CPartyCharm : public CNtlPLGui, public RWS::CEventHandler
{
public:
	struct sMemberVictim
	{
		gui::CStaticBox*	pName;				///< 이름
		gui::CProgressBar*	pZennyGauge;		///< Zenny 퍼센트
		CSurfaceGui			srfGaugeBack;

		SERIAL_HANDLE		hMember;
		RwUInt32			uiZenny;

		sMemberVictim()
			:hMember(INVALID_SERIAL_ID), uiZenny(0)
		{}
	};

	struct sBagLockInfo
	{
		SERIAL_HANDLE		hHandle;
		RwUInt8				byPlace;
		RwUInt8				byPos;
	};

	CPartyCharm(const RwChar* pName);
	virtual ~CPartyCharm();

	static VOID		CreateInstance();
	static VOID		DestroyInstance();

	// GageStage 종료가 아닌 게임중 클래스가 사라질 때만 호출, 그렇지 않으면 사라진 Sob객체에
	// 접근하여 종료될 위험이 있다
	static VOID		ClearBagLock();

	RwBool		Create();
	VOID		Destroy();	

	RwInt32		SwitchDialog(bool bOpen);

protected:
	CPartyCharm() {}
	virtual VOID HandleEvents( RWS::CMsg &msg );

	VOID		Clear();
	VOID		CheckInfoWindow();
	VOID		CheckIconMove();

	VOID		AddMember(sPartyMember* pMember);
	VOID		DelMember(SERIAL_HANDLE hHandle);

	sMemberVictim* GetMemberVictim(SERIAL_HANDLE hHandle);

	RwUInt32	GetTotalCharmZenny();

	VOID		RefreshTotalZenny();

	RwBool		PtinSlot(RwInt32 iX, RwInt32 iY);
	VOID		FocusEffect(RwBool bPush);
	VOID		SelectEffect(RwBool bPush);

	VOID		OnClicked_CloseButton(gui::CComponent* pComponent);
	VOID		OnClicked_ZennyButton(gui::CComponent* pComponent);
	VOID		OnClicked_ActivateButton(gui::CComponent* pComponent);

	VOID		OnMouseDown(const CKey& key);
	VOID		OnMouseUp(const CKey& key);
	VOID		OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID		OnMouseLeave(gui::CComponent* pComponent);
	VOID		OnPaint();
	VOID		OnPostPaint();

protected:
	static CPartyCharm*	m_pInstance;

	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;	
	gui::CSlot			m_slotCloseButton;
	gui::CSlot			m_slotBtnZenny;
	gui::CSlot			m_slotActivateButton;
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotPostPaint;

	gui::CTexture*		m_pMoneyIconTexture;

	gui::CStaticBox*	m_pDialogName;				///< 다이얼로그의 이름 스태틱
	gui::CStaticBox*	m_pDestZenny;				///< "목표금액"
	gui::CStaticBox*	m_pDestZennyPoint;			///< 목표 Zenny
	gui::CStaticBox*	m_pAvatarName;				///< 아바타 이름
	gui::CStaticBox*	m_pCurZenny;				///< 현재 자신이 투자한 Zenny

	gui::CProgressBar*	m_pZennyGauge;				///< 전체 Zenny 게이지

	gui::CButton*		m_pExitButton;
	gui::CButton*		m_pZennyButton;				///< Zenny 투자 버튼
	gui::CButton*		m_pActivateButton;			///< 부적 발동 버튼

	CRegularSlotGui			m_CharmSlot;

	CSurfaceGui			m_FocusEffect;				///< 슬롯 포커스 이펙트
	CSurfaceGui			m_SelectEffect;				///< 슬롯 셀렉트 이펙트
	CSurfaceGui			m_srfCharmPanel;			///< 부적슬롯이 있는 공간의 배경
	CSurfaceGui			m_srfZennySlotDestination;	///< 제니 슬롯 Destination 이미지
	CSurfaceGui			m_srfZennyIcon;				///< 제니 아이콘
	CSurfaceGui			m_srfCharmSlot;				///< 부적 슬롯 배경
	CSurfaceGui			m_srfZennyGaugeBack;		///< 전체 제니 게이지 배경

	sMemberVictim		m_MemberVictim[NTL_MAX_MEMBER_IN_PARTY];

	RwBool				m_bSelect;
	RwBool				m_bFocus;
	RwBool				m_bZennySlotDestination;
	RwBool				m_bLeftMouseDown;
	RwBool				m_bRightMouseDown;

	sBagLockInfo		m_BagLockInfo;
	sCHARM_TBLDAT*		m_pCHARM_TBLDAT;			///< 등록된 부적의 참 테이블
};