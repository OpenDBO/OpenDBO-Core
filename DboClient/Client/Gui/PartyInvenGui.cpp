#include "precomp_dboclient.h"
#include "PartyInvenGui.h"
// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlPLGuiManager.h"
#include "NtlSobAvatar.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "SideDialogManager.h"
#include "DboDef.h"
#include "DiceManager.h"


#define dPARTYINVEN_SLOT_ROW			4
#define dPARTYINVEN_SLOT_COLUMN			4

#define dPARTYINVEN_SLOT_ROW_GAP		39
#define dPARTYINVEN_SLOT_COLUMN_GAP		38

CPartyInvenGui* CPartyInvenGui::m_pInstance = NULL;

CPartyInvenGui::CPartyInvenGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_byInfoWindowIndex(INVALID_INDEX)
,m_bFocus(FALSE)
,m_eInvenType(INVALID_DIVIDE_TYPE)
,m_iMouseDownSlot(INVALID_INDEX)
,m_iSelectedSlot(INVALID_INDEX)
{
}

CPartyInvenGui::~CPartyInvenGui()
{
}

RwBool CPartyInvenGui::Create()
{
	NTL_FUNCTION( "CPartyInvenGui::Create" );

	if(!CNtlPLGui::Create("gui\\PartyInven.rsr", "gui\\PartyInven.srf", "gui\\PartyInven.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	char acBuffer[128] = "";
	WCHAR awcBuffer[128] = L"";
	RwInt32 iColumn, iRow;

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// 다이얼로그의 이름 스태틱
	m_pDialogName			= (gui::CStaticBox*)GetComponent( "stbDialogName" );
	m_pDialogName->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pDialogName->SetText(GetDisplayStringManager()->GetString(DST_PARTYGUI_INVEN));

	// 최소화 버튼
	m_pMinimizeButton		= (gui::CButton*)GetComponent( "btnMinimize" );
	m_slotMinimizeButton	= m_pMinimizeButton->SigClicked().Connect(this, &CPartyInvenGui::OnClicked_MinimizeButton);

	// 아이템 등급 버튼	
	for( RwUInt8 i = 0 ; i < dITEM_GRADE_IN_PARTY_INVEN_USED ; ++i )
	{
		sprintf_s(acBuffer, 128, "btnItemGrade%d", i);
		m_pItemGradeButton[i] = (gui::CButton*)GetComponent( acBuffer );
		m_pItemGradeButton[i]->SetToggleMode(true);

		const WCHAR* pwcItemGrade = GetDisplayStringManager()->GetString(DST_ITEM_GRADE_NORMAL + i);
		const WCHAR* pwcText = GetDisplayStringManager()->GetString(DST_PARTYGUI_INVEN_ITEM_GRADE);

		swprintf_s(awcBuffer, 128, L"%s %s", pwcItemGrade, pwcText);
		m_pItemGradeButton[i]->SetToolTip( awcBuffer );
		m_slotItemGradeButton[i] = m_pItemGradeButton[i]->SigToggled().Connect(this, &CPartyInvenGui::OnClicked_ItemGradeToggleButton);
	}

	// 아이템 슬롯	
	for(RwInt32 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i)
	{
		iRow = i % dPARTYINVEN_SLOT_ROW;
		iColumn = i / dPARTYINVEN_SLOT_ROW;

		m_SlotInfo[i].slot.Create(m_pThis, SDIALOG_PARTY_INVEN, REGULAR_SLOT_ITEM_TABLE);
		m_SlotInfo[i].slot.SetPosition_fromParent(7 + dPARTYINVEN_SLOT_ROW_GAP*iRow,
												  50 + dPARTYINVEN_SLOT_COLUMN_GAP*iColumn);
	}	

	// 파티 인벤토리에 담긴 아이템들의 배경 슬롯
	m_srfSlots.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyInven.srf", "srfSlots" ) );
	m_srfSlots.SetPositionfromParent(5, 48);

	// 파티 인벤 분배 방식 변경(이전)
	m_pLeftButton		= (gui::CButton*)GetComponent( "btnLeft" );
	m_slotLeftButton	= m_pLeftButton->SigClicked().Connect(this, &CPartyInvenGui::OnClicked_ChangeMethodDivideButton);

	// 파티 인벤 분배 방식 변경(이전)
	m_pRightButton		= (gui::CButton*)GetComponent( "btnRight" );
	m_slotRightButton	= m_pRightButton->SigClicked().Connect(this, &CPartyInvenGui::OnClicked_ChangeMethodDivideButton);

	// 현재 분배 방식 설명
	m_pDivideMethod		= (gui::CStaticBox*)GetComponent( "stbDivideMethod" );

	// 분배 방식 설명의 배경
	m_srfDivideMethodBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyInven.srf", "srfDivideMethodBack" ) );
	m_srfDivideMethodBack.SetPositionfromParent(23, 204);

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );

	// 슬롯 셀렉트 이펙트
	m_SelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );	


	////////////// 파티장에 의한 분배일 때의 정보 ////////////////////////////////////////////////

	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		// 파티원 이름
		sprintf_s(acBuffer, 128, "stbMemberName%d", i);
		m_Divide_by_Leader.pMemberName[i] = (gui::CStaticBox*)GetComponent(acBuffer);

		// 파티원에게 아이템 주기 버튼
		sprintf_s(acBuffer, 128, "btnSendItem%d", i);
		m_Divide_by_Leader.pSendItemButton[i] = (gui::CButton*)GetComponent(acBuffer);
		m_Divide_by_Leader.pSendItemButton[i]->SetToolTip( GetDisplayStringManager()->GetString(DST_PARTYGUI_INVEN_EXCUTE_DIVIDE) );
		m_Divide_by_Leader.slotButton[i] = m_Divide_by_Leader.pSendItemButton[i]->SigClicked().Connect(this, &CPartyInvenGui::OnClicked_SendItemButton);
	}

	////////////// 경매에 의한 분배일 때의 정보 ////////////////////////////////////////////////

	// 현재 가장 많은 돈을 투자한 파티원
	m_Divide_by_Oction.pBidedMember = (gui::CStaticBox*)GetComponent( "stbBidedMember" );

	// 현재 가장 많이 투자된 돈
	m_Divide_by_Oction.pBidedZenny = (gui::CStaticBox*)GetComponent( "stbBidedZenny" );
	m_Divide_by_Oction.pBidedZenny->SetText(L"0");

	for( RwUInt8 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i )
	{
		iRow = i % dPARTYINVEN_SLOT_ROW;
		iColumn = i / dPARTYINVEN_SLOT_ROW;

		m_Divide_by_Oction.slotInvest[i].srfOthersInvestedIcon.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyInven.srf", "srfOthersInvestedIcon" ) );
		m_Divide_by_Oction.slotInvest[i].srfOthersInvestedIcon.SetPositionfromParent(27 + dPARTYINVEN_SLOT_ROW_GAP*iRow,
																					 69 + dPARTYINVEN_SLOT_COLUMN_GAP*iColumn);

		m_Divide_by_Oction.slotInvest[i].srfMyInvestedIcon.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyInven.srf", "srfMyInvestedIcon" ) );
		m_Divide_by_Oction.slotInvest[i].srfMyInvestedIcon.SetPositionfromParent(27 + dPARTYINVEN_SLOT_ROW_GAP*iRow,
																				69 + dPARTYINVEN_SLOT_COLUMN_GAP*iColumn);
	}

	// 현재 투자된 가장 많은 돈 및 투자 버튼
	m_Divide_by_Oction.pBidedZennyButton = (gui::CButton*)GetComponent( "btnBidedZenny" );
	m_Divide_by_Oction.slotBidedZennyButton = m_Divide_by_Oction.pBidedZennyButton->SigClicked().Connect(this, &CPartyInvenGui::OnClicked_Oction_ZennyButton);

	// 경매 언락 버튼
	m_Divide_by_Oction.pOctionUnLockButton = (gui::CButton*)GetComponent( "btnOctionStart" );
	m_Divide_by_Oction.pOctionUnLockButton->SetToolTip( GetDisplayStringManager()->GetString(DST_PARTYGUI_INVEN_UNLOCK) );
	m_Divide_by_Oction.slotOctionUnLockButton = m_Divide_by_Oction.pOctionUnLockButton->SigClicked().Connect(this, &CPartyInvenGui::OnClicked_Oction_LockUnLockButton);

	// 경매 락 버튼
	m_Divide_by_Oction.pOctionLockButton = (gui::CButton*)GetComponent( "btnOctionStop" );
	m_Divide_by_Oction.pOctionLockButton->SetToolTip( GetDisplayStringManager()->GetString(DST_PARTYGUI_INVEN_LOCK) );
	m_Divide_by_Oction.slotOctionLockButton = m_Divide_by_Oction.pOctionLockButton->SigClicked().Connect(this, &CPartyInvenGui::OnClicked_Oction_LockUnLockButton);

	// 낙찰 버튼
	m_Divide_by_Oction.pBidButton = (gui::CButton*)GetComponent( "btnBid" );
	m_Divide_by_Oction.pBidButton->SetToolTip( GetDisplayStringManager()->GetString(DST_PARTYGUI_INVEN_SUCCESSFUL_BID) );
	m_Divide_by_Oction.slotBidButton = m_Divide_by_Oction.pBidButton->SigClicked().Connect(this, &CPartyInvenGui::OnClicked_Oction_BidButton);

	// 초기화 버튼
	m_Divide_by_Oction.pResetButton = (gui::CButton*)GetComponent( "btnReset" );
	m_Divide_by_Oction.pResetButton->SetToolTip( GetDisplayStringManager()->GetString(DST_PARTYGUI_INVEN_RESET_OCTION) );
	m_Divide_by_Oction.slotResetButton = m_Divide_by_Oction.pResetButton->SigClicked().Connect(this, &CPartyInvenGui::OnClicked_Oction_ResetButton);

	// 경매창 배경
	m_Divide_by_Oction.srfOctionBack.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyInven.srf", "srfOctionBack" ) );
	m_Divide_by_Oction.srfOctionBack.SetPositionfromParent(7, 232);

	// 제니 슬롯 Destination 이미지
	m_Divide_by_Oction.srfZennySlotDestination.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "PartyInven.srf", "srfBtnOctionZennyDestination" ) );
	m_Divide_by_Oction.srfZennySlotDestination.SetPositionfromParent(11, 260);

	////////////// 주사위 굴리기에 의한 분배일 때의 정보 ////////////////////////////////////////////////

	// 주사위 굴리기 버튼
	m_Divide_by_Dice.pDiceButton = (gui::CButton*)GetComponent( "btnDice" );
	m_Divide_by_Dice.pDiceButton->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_ITEM_LOOTING_BY_DICE) );
	m_Divide_by_Dice.slotButton = m_Divide_by_Dice.pDiceButton->SigClicked().Connect(this, &CPartyInvenGui::OnClicked_DiceButton);


	// default value
	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
		m_Divide_by_Leader.hSerial[i] = INVALID_SERIAL_ID;


	m_Divide_by_Oction.bZennySlotDestination = FALSE;
	ActivateDivideMethod(INVALID_DIVIDE_TYPE);


	// sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CPartyInvenGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CPartyInvenGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CPartyInvenGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CPartyInvenGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CPartyInvenGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CPartyInvenGui::OnPaint );
	m_slotPostPaint		= m_Divide_by_Oction.pBidedZennyButton->SigPaint().Connect( this, &CPartyInvenGui::OnPostPaint );

	LinkMsg(g_EventPartyUpdateValue);
	LinkMsg(g_EventPartyInventory);
	LinkMsg(g_EventIconMoveClick);	

	OnMove(100, 100);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CPartyInvenGui::Destroy()
{
	NTL_FUNCTION("CPartyInvenGui::Destroy");

	GetDiceManager()->Cancel(DICE_WORKD_ID_PARTY_INVEN_DICE);
	CalcInvestedAvatarZenny();

	Clear();

	for(RwInt8 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i)
	{
		m_SlotInfo[i].slot.Destroy();
	}

	CheckInfoWindow();

	UnLinkMsg(g_EventPartyUpdateValue);
	UnLinkMsg(g_EventPartyInventory);
	UnLinkMsg(g_EventIconMoveClick);	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CPartyInvenGui::CreateInstance()
{
	if( m_pInstance )
		return;

	m_pInstance = NTL_NEW CPartyInvenGui("PartyInvenGui");
	if( !m_pInstance->Create() )
		NTL_DELETE(m_pInstance);

	CNtlPLGuiManager::GetInstance()->AddGui(m_pInstance);	
	GetSideDialogManager()->RegistDialog(SDIALOG_PARTY_INVEN, m_pInstance, &CPartyInvenGui::SwitchDialog);
}

VOID CPartyInvenGui::DestroyInstance()
{
	if( !m_pInstance )
		return;

	GetSideDialogManager()->UnRegistDialog(SDIALOG_PARTY_INVEN);
	GetNtlGuiManager()->RemoveGui(m_pInstance);
	m_pInstance->Destroy();
	NTL_DELETE(m_pInstance);
}

VOID CPartyInvenGui::Clear()
{
	for( RwUInt8 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i )
	{
		m_SlotInfo[i].slot.Clear();
	}

	m_byInfoWindowIndex		= INVALID_BYTE;
	m_iMouseDownSlot		= INVALID_INDEX;
	m_iSelectedSlot			= INVALID_INDEX;
	m_bFocus				= FALSE;

	CheckInfoWindow();
}

VOID CPartyInvenGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_PROFILE_ITEM ||
		GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM )
	{
		if( GetInfoWndManager()->GetRequestGui() == SDIALOG_PARTY_INVEN )
		{
			FocusEffect(FALSE);
			m_byInfoWindowIndex = INVALID_INDEX;
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		}			
	}
}

VOID CPartyInvenGui::ActivateDivideMethod(eInvenDivideType eType)
{	
	m_eInvenType = eType;

	switch( m_eInvenType )
	{
	case INVEN_DIVIDE_TYPE_BY_LEADER:
		{
			ActivateDivideMethod_by_leader(true);
			ActivateDivideMethod_by_Oction(false);
			ActivateDivideMethod_by_Dice(false);
			EnableConrollers(true);
			SelectEffect(INVALID_INDEX);
			m_pDivideMethod->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_ITEM_LOOTING_BY_LEADER) );
			break;
		}
	case INVEN_DIVIDE_TYPE_BY_OCTION:
		{
			ActivateDivideMethod_by_leader(false);
			ActivateDivideMethod_by_Oction(true);
			ActivateDivideMethod_by_Dice(false);
			EnableConrollers(true);
			SelectEffect(INVALID_INDEX);
			m_pDivideMethod->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_ITEM_LOOTING_BY_ZENNY) );
			break;
		}
	case INVEN_DIVIDE_TYPE_BY_DICE:
		{
			ActivateDivideMethod_by_leader(false);
			ActivateDivideMethod_by_Oction(false);
			ActivateDivideMethod_by_Dice(true);
			EnableConrollers(true);
			SelectEffect(INVALID_INDEX);
			m_pDivideMethod->SetText( GetDisplayStringManager()->GetString(DST_PARTYGUI_ITEM_LOOTING_BY_DICE) );
			break;
		}
	case INVALID_DIVIDE_TYPE:
		{
			m_pDivideMethod->Clear();
			ActivateDivideMethod_by_leader(false);
			ActivateDivideMethod_by_Oction(false);
			ActivateDivideMethod_by_Dice(false);
			EnableConrollers(false);
			SelectEffect(INVALID_INDEX);
			break;
		}
	default:
		{
			DBO_FAIL("Wrong divide type");
			break;
		}
	}
}

VOID CPartyInvenGui::ActivateDivideMethod_by_leader(bool bActive)
{
	CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();

	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		m_Divide_by_Leader.pMemberName[i]->Show(bActive);

		if( bActive && pParty->GetMemberbyIndex(i) )
			m_Divide_by_Leader.pSendItemButton[i]->Show(true);
		else
			m_Divide_by_Leader.pSendItemButton[i]->Show(false);
	}
}

VOID CPartyInvenGui::ActivateDivideMethod_by_Oction(bool bActive)
{
	m_Divide_by_Oction.pBidedMember->Show(bActive);
	m_Divide_by_Oction.pBidedZenny->Show(bActive);
	m_Divide_by_Oction.pBidedZennyButton->Show(bActive);
	m_Divide_by_Oction.pOctionUnLockButton->Show(bActive);
	m_Divide_by_Oction.pOctionLockButton->Show(bActive);
	m_Divide_by_Oction.pBidButton->Show(bActive);
	m_Divide_by_Oction.pResetButton->Show(bActive);
	m_Divide_by_Oction.bOctionLock = bActive;

	if( bActive )
	{
		m_Divide_by_Oction.pBidedZennyButton	->ClickEnable(true);
		m_Divide_by_Oction.pBidButton			->ClickEnable(false);
		m_Divide_by_Oction.pResetButton			->ClickEnable(false);
	}	

	for( RwUInt8 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i )
	{
		m_Divide_by_Oction.slotInvest[i].hSerial		= INVALID_SERIAL_ID;
		m_Divide_by_Oction.slotInvest[i].uiZenny		= 0;
		m_Divide_by_Oction.slotInvest[i].bBestisAvatar	= FALSE;
	}

	CalcInvestedAvatarZenny();
}

VOID CPartyInvenGui::ActivateDivideMethod_by_Dice(bool bActive)
{
	m_Divide_by_Dice.pDiceButton->Show(bActive);
	m_Divide_by_Dice.bRollingDice = FALSE;

	if( !bActive )
		GetDiceManager()->Cancel(DICE_WORKD_ID_PARTY_INVEN_DICE);
}

VOID CPartyInvenGui::EnableConrollers(bool bEnable)
{
	for( RwUInt8 i = 0 ; i < dITEM_GRADE_IN_PARTY_INVEN_USED ; ++i )
		m_pItemGradeButton[i]->ClickEnable(bEnable);

	m_pLeftButton->ClickEnable(bEnable);
	m_pRightButton->ClickEnable(bEnable);

	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		m_Divide_by_Leader.pSendItemButton[i]->ClickEnable(bEnable);
	}

	m_Divide_by_Oction.pBidedZennyButton->ClickEnable(bEnable);
	m_Divide_by_Oction.pOctionUnLockButton->ClickEnable(bEnable);
	m_Divide_by_Oction.pBidButton->ClickEnable(bEnable);
	m_Divide_by_Oction.pResetButton->ClickEnable(bEnable);

	m_Divide_by_Dice.pDiceButton->ClickEnable(bEnable);
}

VOID CPartyInvenGui::CalcInvestedAvatarZenny()
{
	RwInt32 iZenny = 0;

	for( RwUInt8 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i )
	{
		if( m_Divide_by_Oction.slotInvest[i].bBestisAvatar )
			iZenny += m_Divide_by_Oction.slotInvest[i].uiZenny;
	}

	GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::PARTY_INVEN, -1 * iZenny);
}

RwInt8 CPartyInvenGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	for( RwInt8 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i )
	{
		if( m_SlotInfo[i].slot.PtInRect(iX, iY) )
		{
			return i;
		}		
	}

	return INVALID_BYTE;
}

VOID CPartyInvenGui::FocusEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if(bPush)
	{
		CRectangle rectParent = GetPosition();
		RwInt32 iX = rectParent.left + 5 + 39*(iSlotIdx%dPARTYINVEN_SLOT_COLUMN);
		RwInt32 iY = rectParent.top + 48 + 38*(iSlotIdx/dPARTYINVEN_SLOT_COLUMN);

		m_bFocus = TRUE;
		m_FocusEffect.SetRectWH(iX, iY, m_FocusEffect.GetWidth(), m_FocusEffect.GetHeight() );
	}
	else
		m_bFocus = FALSE;
}

VOID CPartyInvenGui::SelectEffect(RwInt32 iSlot)
{
	m_iSelectedSlot = iSlot;

	if( m_eInvenType == INVALID_DIVIDE_TYPE )
		return;

	if( m_iSelectedSlot != INVALID_INDEX )
	{
		CRectangle rectParent = GetPosition();
		RwInt32 iX = rectParent.left + 5 + 39*(iSlot%dPARTYINVEN_SLOT_COLUMN);
		RwInt32 iY = rectParent.top + 48 + 38*(iSlot/dPARTYINVEN_SLOT_COLUMN);

		m_bFocus = TRUE;
		m_SelectEffect.SetRectWH(iX, iY, m_FocusEffect.GetWidth(), m_FocusEffect.GetHeight() );
	}	
}

VOID CPartyInvenGui::OnClicked_MinimizeButton(gui::CComponent* pComponent)
{
	CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_MINIMIZE, SDIALOG_PARTY_INVEN, DIALOG_SIDEDIALOG_CONTROLLER);
}

VOID CPartyInvenGui::OnClicked_ItemGradeToggleButton(gui::CComponent* pComponent, bool bToggled)
{
	for( RwInt8 i = 0 ; i < dITEM_GRADE_IN_PARTY_INVEN_USED ; ++i )
	{
		if( pComponent == m_pItemGradeButton[i] )
		{
			// eITEM_RANK의 첫번째 인자는 ITEM_RANK_NOTHING이다
			GetDboGlobal()->GetGamePacketGenerator()->SendPartyInven_ItemRankSetReq(i + 1);
			break;
		}
	}
}

VOID CPartyInvenGui::OnClicked_ChangeMethodDivideButton(gui::CComponent* pComponent)
{
	RwInt8 byIndex = (RwInt8)m_eInvenType;

	if( pComponent == m_pLeftButton )
	{
		--byIndex;

		if( byIndex < INVEN_DIVIDE_TYPE_BY_LEADER )
			byIndex = NUM_INVEN_DIVIDE_TYPE - 1;
	}
	else if( pComponent == m_pRightButton )
	{
		++byIndex;

		if( byIndex >= NUM_INVEN_DIVIDE_TYPE )
			byIndex = INVEN_DIVIDE_TYPE_BY_LEADER;
	}

	switch( byIndex )
	{
	case INVEN_DIVIDE_TYPE_BY_LEADER:
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPartyItemDivision(NTL_PARTY_ITEM_LOOTING_IN_LEADER);
			break;
		}
	case INVEN_DIVIDE_TYPE_BY_OCTION:
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPartyItemDivision(NTL_PARTY_ITEM_LOOTING_BY_ZENNY);
			break;
		}
	case INVEN_DIVIDE_TYPE_BY_DICE:
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendPartyItemDivision(NTL_PARTY_ITEM_LOOTING_DICE);
			break;
		}
	default:
		{
			DBO_FAIL("Wrong divide method type : " << byIndex);
			return;
		}
	}
}

VOID CPartyInvenGui::OnClicked_SendItemButton(gui::CComponent* pComponent)
{
	if( m_iSelectedSlot == INVALID_INDEX )
	{
		GetAlarmManager()->AlarmMessage(DST_NOT_SELECTED_SLOT);
		return;
	}

	SERIAL_HANDLE hMember = INVALID_SERIAL_ID;

	for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
	{
		if( m_Divide_by_Leader.pSendItemButton[i] == pComponent )
		{
			hMember = m_Divide_by_Leader.hSerial[i];
			break;
		}
	}

	if( hMember == INVALID_SERIAL_ID )
		return;

	GetDboGlobal()->GetGamePacketGenerator()->SendPartyInven_GiveReq((RwUInt8)m_iSelectedSlot, hMember);
}

VOID CPartyInvenGui::OnClicked_DiceButton(gui::CComponent* pComponent)
{
	if( m_iSelectedSlot == INVALID_INDEX )
	{
		GetAlarmManager()->AlarmMessage(DST_NOT_SELECTED_SLOT);
		return;
	}

	GetDboGlobal()->GetGamePacketGenerator()->SendPartyInven_DiceStartReq((RwUInt8)m_iSelectedSlot);	
}

VOID CPartyInvenGui::OnClicked_Oction_ZennyButton(gui::CComponent* pComponent)
{
	if( m_iSelectedSlot == INVALID_INDEX )
	{
		GetAlarmManager()->AlarmMessage(DST_NOT_SELECTED_SLOT);
		return;
	}

	if( GetIconMoveManager()->IsActive() == FALSE )
		return;

	GetIconMoveManager()->IconMovePutDown(PLACE_SUB_PARTYINVEN_ZENNY, INVALID_SERIAL_ID, m_iSelectedSlot);
}

VOID CPartyInvenGui::OnClicked_Oction_LockUnLockButton(gui::CComponent* pComponent)
{
	if( m_Divide_by_Oction.pOctionLockButton == pComponent )
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendPartyInven_LockReq(true);
	}
	else
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendPartyInven_LockReq(false);
	}
}

VOID CPartyInvenGui::OnClicked_Oction_BidButton(gui::CComponent* pComponent)
{
	if( m_iSelectedSlot == INVALID_INDEX )
	{
		GetAlarmManager()->AlarmMessage(DST_NOT_SELECTED_SLOT);
		return;
	}

	if( m_Divide_by_Oction.slotInvest[m_iSelectedSlot].uiZenny <= 0 )
	{
		GetAlarmManager()->AlarmMessage(GAME_PARTY_INVEN_INVEST_ZENNY_NOT_ENOUGH, TRUE);		
		return;
	}

	GetDboGlobal()->GetGamePacketGenerator()->SendPartyInven_AcceptedReq((RwUInt8)m_iSelectedSlot);
}

VOID CPartyInvenGui::OnClicked_Oction_ResetButton(gui::CComponent* pComponent)
{
	GetDboGlobal()->GetGamePacketGenerator()->SendPartyInven_ResetReq();
}

VOID CPartyInvenGui::OnMouseDown(const CKey& key)
{
	if( key.m_nID != UD_LEFT_BUTTON )
		return;

	RwInt8 iPtinSlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if(iPtinSlot != INVALID_INDEX)
		m_iMouseDownSlot = iPtinSlot;
}

VOID CPartyInvenGui::OnMouseUp(const CKey& key)
{	
	if( m_iMouseDownSlot ==INVALID_INDEX )
		return;

	if( key.m_nID == UD_LEFT_BUTTON )
	{
		if( m_SlotInfo[m_iMouseDownSlot].slot.GetSerial() != INVALID_SERIAL_ID )
		{
			if( m_SlotInfo[m_iMouseDownSlot].slot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fY) )
			{
				if( m_eInvenType == INVEN_DIVIDE_TYPE_BY_DICE )
				{
					if( m_Divide_by_Dice.bRollingDice )
					{
						m_iMouseDownSlot = INVALID_INDEX;
						return;
					}
				}

				// 아이템 선택
				SelectEffect(m_iMouseDownSlot);

				if( m_eInvenType == INVEN_DIVIDE_TYPE_BY_OCTION )
				{
					CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
					sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(m_Divide_by_Oction.slotInvest[m_iMouseDownSlot].hSerial) );
					if( pMember )
					{
						m_Divide_by_Oction.pBidedMember->SetText(pMember->wszMemberName);
						m_Divide_by_Oction.pBidedZenny->SetText(m_Divide_by_Oction.slotInvest[m_iMouseDownSlot].uiZenny);
					}
					else
					{
						m_Divide_by_Oction.pBidedMember->Clear();
						m_Divide_by_Oction.pBidedZenny->SetText(L"0");
					}
				}
			}
		}
	}

	m_iMouseDownSlot = INVALID_INDEX;
}

VOID CPartyInvenGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rectParent = GetPosition();
	
	m_srfSlots.SetPositionbyParent(rectParent.left, rectParent.top);
	m_srfDivideMethodBack.SetPositionbyParent(rectParent.left, rectParent.top);
	m_Divide_by_Oction.srfOctionBack.SetPositionbyParent(rectParent.left, rectParent.top);

	SelectEffect(m_iSelectedSlot);

	m_Divide_by_Oction.srfZennySlotDestination.SetPositionbyParent(rectParent.left, rectParent.top);

	for(RwInt32 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i)
	{
		m_SlotInfo[i].slot.SetParentPosition(rectParent.left, rectParent.top);		

		m_Divide_by_Oction.slotInvest[i].srfOthersInvestedIcon.SetPositionbyParent(rectParent.left, rectParent.top);
		m_Divide_by_Oction.slotInvest[i].srfMyInvestedIcon.SetPositionbyParent(rectParent.left, rectParent.top);
	}

	CheckInfoWindow();
}

VOID CPartyInvenGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{	
	RwInt8 iPtinSlot = PtinSlot(nX, nY);
	if( iPtinSlot != INVALID_INDEX )
	{
		FocusEffect(TRUE, iPtinSlot);

		if( m_SlotInfo[iPtinSlot].slot.GetSerial() != INVALID_SERIAL_ID )
		{
			if( m_byInfoWindowIndex != iPtinSlot )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				m_byInfoWindowIndex = iPtinSlot;

				// 아이콘 영역으로 마우스를 이동시켜서 아이템 정보를 보일 필요가 있는지 검사
				if(m_SlotInfo[iPtinSlot].slot.GetSerialType() == REGULAR_SLOT_ITEM_NOT_IDENTIFICATION)
				{
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM,
														rtScreen.left + m_SlotInfo[iPtinSlot].slot.GetX_fromParent(),
														rtScreen.top + m_SlotInfo[iPtinSlot].slot.GetY_fromParent(),
														NULL, SDIALOG_PARTY_INVEN );
				}
				else
				{
					GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_PROFILE_ITEM,
														rtScreen.left + m_SlotInfo[iPtinSlot].slot.GetX_fromParent(),
														rtScreen.top + m_SlotInfo[iPtinSlot].slot.GetY_fromParent(),
														&m_SlotInfo[iPtinSlot].ItemProfile, SDIALOG_PARTY_INVEN );
				}
			}
		}

		return;
	}

	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_INDEX;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CPartyInvenGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_INDEX;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

RwInt32 CPartyInvenGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if( !bOpen )
	{
		GetIconMoveManager()->IconMoveEnd();
		CheckInfoWindow();		
	}

	return 1;
}

VOID CPartyInvenGui::OnPaint()
{
	m_srfSlots.Render();
	m_srfDivideMethodBack.Render();

	for(RwInt32 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i)
	{
		m_SlotInfo[i].slot.Paint();
	}

	if( m_iSelectedSlot != INVALID_INDEX )
		m_SelectEffect.Render();

	if( m_bFocus )
		m_FocusEffect.Render();

	if( m_eInvenType == INVEN_DIVIDE_TYPE_BY_OCTION )
	{
		m_Divide_by_Oction.srfOctionBack.Render();

		for( RwUInt8 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i )
		{
			if( m_Divide_by_Oction.slotInvest[i].uiZenny > 0 )
			{
				if( m_Divide_by_Oction.slotInvest[i].bBestisAvatar )
					m_Divide_by_Oction.slotInvest[i].srfMyInvestedIcon.Render();
				else
					m_Divide_by_Oction.slotInvest[i].srfOthersInvestedIcon.Render();
			}			
		}
	}
}

VOID CPartyInvenGui::OnPostPaint()
{	
	if( FALSE == m_Divide_by_Oction.bZennySlotDestination )
		return;
		
	if( FALSE == m_Divide_by_Oction.pBidedZennyButton->IsClickEnable() )
		return;

	if( m_iSelectedSlot == INVALID_INDEX )
		return;
	
	m_Divide_by_Oction.srfZennySlotDestination.Render();
}

VOID CPartyInvenGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CPartyInvenGui::HandleEvents");

	if(msg.Id == g_EventPartyUpdateValue)
	{
		SNtlEventPartyUpdate* pEvent = reinterpret_cast<SNtlEventPartyUpdate*>(msg.pData);

		if( pEvent->iMessage == PMT_PARTY_LEADER_CHANGE )
		{
			RwBool bAvatarIsPartyLearder = Logic_I_am_PartyLeader();
		
			for( RwUInt8 i = 0 ; i < dITEM_GRADE_IN_PARTY_INVEN_USED ; ++i )
				m_pItemGradeButton[i]->ClickEnable(bAvatarIsPartyLearder);
		}
		else if( pEvent->iMessage == PMT_PARTY_MEMBER_ADD )
		{
			CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
			sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pEvent->hSerialId) );
			if( !pMember )
			{
				NTL_ASSERT(pMember, "CPartyInvenGui::HandleEvents, g_EventPartyUpdateValue, PMT_PARTY_MEMBER_ADD, Not exist party member of handle : " << pEvent->hSerialId);
				NTL_RETURNVOID();
			}
			
			RwInt32 iIndex = pParty->GetMemberCount() - 1;

			m_Divide_by_Leader.hSerial[iIndex] = pMember->hSerial;
			m_Divide_by_Leader.pMemberName[iIndex]->SetText( pMember->wszMemberName );
			m_Divide_by_Leader.pSendItemButton[iIndex]->Show(true);
		}
		else if( pEvent->iMessage == PMT_PARTY_MEMBER_DEL )
		{
			CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();

			for( RwUInt8 i = 0 ; i < NTL_MAX_MEMBER_IN_PARTY ; ++i )
			{
				m_Divide_by_Leader.hSerial[i] = INVALID_SERIAL_ID;
				m_Divide_by_Leader.pMemberName[i]->Clear();
				m_Divide_by_Leader.pSendItemButton[i]->Show(false);
			}

			for( RwUInt8 i = 0 ; i < pParty->GetMemberCount() ; ++i )
			{
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyIndex(i) );
				if( !pMember )
				{
					DBO_FAIL("g_EventPartyUpdateValue, PMT_PARTY_MEMBER_DEL, Not exist party member of index: " << i);
					continue;
				}

				m_Divide_by_Leader.hSerial[i] = pMember->hSerial;
				m_Divide_by_Leader.pMemberName[i]->SetText( pMember->wszMemberName );

				if( INVEN_DIVIDE_TYPE_BY_LEADER == m_eInvenType )
					m_Divide_by_Leader.pSendItemButton[i]->Show(true);
			}
		}
		else if( pEvent->iMessage == PMT_ITEM_DIVISION )
		{
			switch( pEvent->uiValue )
			{
			case NTL_PARTY_ITEM_LOOTING_IN_LEADER:
				{
					ActivateDivideMethod(INVEN_DIVIDE_TYPE_BY_LEADER);
					break;
				}
			case NTL_PARTY_ITEM_LOOTING_BY_ZENNY:
				{
					ActivateDivideMethod(INVEN_DIVIDE_TYPE_BY_OCTION);
					break;
				}
			case NTL_PARTY_ITEM_LOOTING_DICE:
				{
					ActivateDivideMethod(INVEN_DIVIDE_TYPE_BY_DICE);
					break;
				}
			default:
				{
					ActivateDivideMethod(INVALID_DIVIDE_TYPE);
					break;
				}
			}
		}
	}
	else if(msg.Id == g_EventPartyInventory)
	{
		SDboPartyInvetoryItem* pEvent = reinterpret_cast<SDboPartyInvetoryItem*>(msg.pData);

		switch(pEvent->iType)
		{
		case PINVEN_ADD_ITEM:
			{
				NTL_ASSERT(m_SlotInfo[pEvent->byPos].slot.GetSerial() == INVALID_SERIAL_ID, "CPartyInvenGui::HandleEvents, Not Empty Slot : " << pEvent->byPos);

				if( m_SlotInfo[pEvent->byPos].slot.GetSerial() == INVALID_SERIAL_ID )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					m_SlotInfo[pEvent->byPos].slot.SetParentPosition(rtScreen.left, rtScreen.top);
					m_SlotInfo[pEvent->byPos].slot.SetCount(pEvent->uiStackCount);

					if(pEvent->bTrue)
					{
						m_SlotInfo[pEvent->byPos].slot.SetSerialType(REGULAR_SLOT_ITEM_NOT_IDENTIFICATION);
						m_SlotInfo[pEvent->byPos].slot.SetIcon(pEvent->uiSerial);
					}
					else
					{
						m_SlotInfo[pEvent->byPos].slot.SetSerialType(REGULAR_SLOT_ITEM_TABLE);
						m_SlotInfo[pEvent->byPos].slot.SetIcon(pEvent->uiSerial);
					}
				}


				// InfoWindow 를 위한 정보 저장
				m_SlotInfo[pEvent->byPos].ItemProfile.handle			= INVALID_SERIAL_ID;
				m_SlotInfo[pEvent->byPos].ItemProfile.tblidx			= pEvent->uiSerial;
				m_SlotInfo[pEvent->byPos].ItemProfile.byPlace			= 0xff;
				m_SlotInfo[pEvent->byPos].ItemProfile.byPos				= pEvent->byPos;
				m_SlotInfo[pEvent->byPos].ItemProfile.bNeedToIdentify	= B2b(pEvent->bTrue);

				if( pEvent->bTrue == false )
				{					
					m_SlotInfo[pEvent->byPos].ItemProfile.byStackcount		= (BYTE)pEvent->uiStackCount;
					m_SlotInfo[pEvent->byPos].ItemProfile.byRank			= pEvent->byRank;
					m_SlotInfo[pEvent->byPos].ItemProfile.byCurDur			= m_SlotInfo[pEvent->byPos].slot.GetItemTable()->byDurability;
					m_SlotInfo[pEvent->byPos].ItemProfile.aOptionTblidx[0]	= pEvent->uiOptionSerial1;
					m_SlotInfo[pEvent->byPos].ItemProfile.aOptionTblidx[1]	= pEvent->uiOptionSerial2;					
					m_SlotInfo[pEvent->byPos].ItemProfile.byGrade			= pEvent->byGrade;
					m_SlotInfo[pEvent->byPos].ItemProfile.byBattleAttribute	= pEvent->byBattleAttribute;
				}				
				
				break;
			}		
		case PINVEN_DEL_ITEM:
			{
				m_SlotInfo[pEvent->byPos].slot.Clear();
				m_SlotInfo[pEvent->byPos].ItemProfile.handle	= INVALID_SERIAL_ID;
				m_SlotInfo[pEvent->byPos].ItemProfile.tblidx	= INVALID_SERIAL_ID;

				if( m_byInfoWindowIndex != INVALID_INDEX )
					GetInfoWndManager()->ShowInfoWindow( FALSE );

				m_byInfoWindowIndex	= INVALID_BYTE;
				m_iMouseDownSlot	= INVALID_INDEX;
				SelectEffect(INVALID_INDEX);

				break;
			}
		case PINVEN_CHANGE_SAVED_ITEM_GRADE:
			{
				// eITEM_RANK의 첫번째 인자는 ITEM_RANK_NOTHING이다
				RwUInt8 bySelectedIndex = pEvent->byRank - 1;

				for( RwInt8 i = 0 ; i < dITEM_GRADE_IN_PARTY_INVEN_USED ; ++i )
				{
					if( i < bySelectedIndex )
					{
						m_pItemGradeButton[i]->SetDown(false);
					}
					else
					{
						m_pItemGradeButton[i]->SetDown(true);
					}
				}

				break;
			}
		case PINVEN_DIVIDE_BY_LEADER:
			{
				CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pEvent->uiSerial) );
				if( !pMember )
				{
					DBO_FAIL("g_EventPartyInventory, PINVEN_DIVIDE_BY_LEADER, Not exist party member of handle : " << pEvent->uiSerial);
					NTL_RETURNVOID();
				}

				if( m_SlotInfo[m_iSelectedSlot].ItemProfile.bNeedToIdentify )
				{
					GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVEN_GIVE_BY_LEADER_WITH_UNIDENTIFY, FALSE, NULL, pMember->wszMemberName);
				}
				else
				{
					sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(m_SlotInfo[pEvent->byPos].ItemProfile.tblidx);
					if( !pITEM_TBLDAT )
					{
						NTL_ASSERT(pITEM_TBLDAT, "CPartyInvenGui::HandleEvents, g_EventPartyInventory, PINVEN_DIVIDE_BY_LEADER, Not exist item table of index : " << m_SlotInfo[pEvent->byPos].ItemProfile.tblidx);
						NTL_RETURNVOID();
					}

					CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
					std::wstring& strItemName = pItemTextTable->GetText(pITEM_TBLDAT->Name);		

					GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVEN_GIVE_BY_LEADER, FALSE, NULL, strItemName.c_str(), pMember->wszMemberName);
				}

				m_SlotInfo[pEvent->byPos].slot.Clear();
				m_SlotInfo[pEvent->byPos].ItemProfile.handle	= INVALID_SERIAL_ID;
				m_SlotInfo[pEvent->byPos].ItemProfile.tblidx	= INVALID_SERIAL_ID;

				if( m_byInfoWindowIndex != pEvent->byPos )
					GetInfoWndManager()->ShowInfoWindow( FALSE );

				m_byInfoWindowIndex	= INVALID_BYTE;
				m_iMouseDownSlot	= INVALID_INDEX;
				SelectEffect(INVALID_INDEX);

				break;
			}
		case PINVEN_DIVIDE_BY_OCTION_LOCK:
			{
				m_Divide_by_Oction.bOctionLock = pEvent->bTrue;

				if( m_Divide_by_Oction.bOctionLock )
				{
					m_Divide_by_Oction.pOctionLockButton	->Show(false);
					m_Divide_by_Oction.pOctionUnLockButton	->Show(true);

					m_Divide_by_Oction.pBidedZennyButton	->ClickEnable(false);
					m_Divide_by_Oction.pBidButton			->ClickEnable(true);
					m_Divide_by_Oction.pResetButton			->ClickEnable(true);
				}
				else
				{
					m_Divide_by_Oction.pOctionLockButton	->Show(true);
					m_Divide_by_Oction.pOctionUnLockButton	->Show(false);

					m_Divide_by_Oction.pBidedZennyButton	->ClickEnable(true);
					m_Divide_by_Oction.pBidButton			->ClickEnable(false);
					m_Divide_by_Oction.pResetButton			->ClickEnable(false);					
				}

				break;
			}
		case PINVEN_DIVIDE_BY_OCTION_INVEST_NOTIFY:
			{
                if(pEvent->byPos >= NTL_PARTY_INVENTORY_SLOT_COUNT)
                    break;

				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				CNtlParty* pParty = pAvatar->GetParty();
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pEvent->uiSerial) );
				if( !pMember )
				{
					NTL_ASSERT(pMember, "CPartyInvenGui::HandleEvents, g_EventPartyInventory, PINVEN_DIVIDE_BY_OCTION_INVEST_NOTIFY, Not exist party member of handle : " << pEvent->uiSerial);
					NTL_RETURNVOID();
				}				

				if( pMember )
				{
					m_Divide_by_Oction.slotInvest[pEvent->byPos].hSerial = pEvent->uiSerial;
					m_Divide_by_Oction.slotInvest[pEvent->byPos].uiZenny = pEvent->uiStackCount;

					// 현재 해당 슬롯을 보고 있다면 가장 많이 투자한 맴버를 표시한다
					if( m_iSelectedSlot == pEvent->byPos )
					{
						m_Divide_by_Oction.pBidedMember->SetText( pMember->wszMemberName );
						m_Divide_by_Oction.pBidedZenny->SetText( pEvent->uiStackCount );
					}

					if( pMember->hSerial == Logic_GetAvatarHandle() )
					{
						m_Divide_by_Oction.slotInvest[pEvent->byPos].bBestisAvatar = TRUE;
					}
					else
					{						
						m_Divide_by_Oction.slotInvest[pEvent->byPos].bBestisAvatar = FALSE;						
					}

					CalcInvestedAvatarZenny();
				}

				break;
			}
		case PINVEN_DIVIDE_BY_OCTION_RESET:
			{
				for( RwUInt8 i = 0 ; i < NTL_PARTY_INVENTORY_SLOT_COUNT ; ++i )
				{
					m_Divide_by_Oction.slotInvest[i].hSerial		= INVALID_SERIAL_ID;
					m_Divide_by_Oction.slotInvest[i].uiZenny		= 0;
					m_Divide_by_Oction.slotInvest[i].bBestisAvatar	= FALSE;
				}				

				m_Divide_by_Oction.pBidedMember->Clear();
				m_Divide_by_Oction.pBidedZenny->SetText(L"0" );

				CalcInvestedAvatarZenny();

				break;
			}
		case PINVEN_DIVIDE_BY_OCTION_SUCCESSFUL_BID:
			{
				CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pEvent->uiSerial) );
				if( !pMember )
				{					
					DBO_FAIL("g_EventPartyInventory, PINVEN_DIVIDE_BY_OCTION_SUCCESSFUL_BID, Not exist party member of handle : " << pEvent->uiSerial);
					NTL_RETURNVOID();
				}
				
				if( m_SlotInfo[m_iSelectedSlot].ItemProfile.bNeedToIdentify )
				{
					GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVEN_OCTION_SUCCESSFUL_BID_WITH_UNIDENTIFY, FALSE, NULL, pMember->wszMemberName);
				}
				else
				{
					sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(m_SlotInfo[pEvent->byPos].ItemProfile.tblidx);
					NTL_ASSERT(pITEM_TBLDAT, "CPartyInvenGui::HandleEvents, g_EventPartyInventory, PINVEN_DIVIDE_BY_OCTION_SUCCESSFUL_BID, Not exist item table of index : " << m_SlotInfo[pEvent->byPos].ItemProfile.tblidx);
					CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
					std::wstring& strItemName = pItemTextTable->GetText(pITEM_TBLDAT->Name);		

					GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVEN_OCTION_SUCCESSFUL_BID, FALSE, NULL, strItemName.c_str(), pMember->wszMemberName);
				}				

				m_Divide_by_Oction.slotInvest[pEvent->byPos].hSerial		= INVALID_SERIAL_ID;
				m_Divide_by_Oction.slotInvest[pEvent->byPos].uiZenny		= 0;
				m_Divide_by_Oction.slotInvest[pEvent->byPos].bBestisAvatar	= FALSE;

				CalcInvestedAvatarZenny();				

				if( m_iSelectedSlot == pEvent->byPos )
				{
					m_Divide_by_Oction.pBidedMember->Clear();
					m_Divide_by_Oction.pBidedZenny->SetText(L"0");

					SelectEffect(INVALID_INDEX);
				}

				m_byInfoWindowIndex	= INVALID_BYTE;
				m_iMouseDownSlot	= INVALID_INDEX;

				if( m_byInfoWindowIndex != pEvent->byPos )
					GetInfoWndManager()->ShowInfoWindow( FALSE );

				break;
			}
		case PINVEN_DIVIDE_BY_DICE_START:
			{
				CDboEventGenerator::DiceStart(DICE_WORKD_ID_PARTY_INVEN_DICE, (void*)&m_SlotInfo[pEvent->byPos].ItemProfile);
				
				SelectEffect(pEvent->byPos);
				m_Divide_by_Dice.bRollingDice = TRUE;
				break;
			}
		case PINVEN_DIVIDE_BY_DICE_RESULT:
			{
				CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
				sPartyMember* pMember = reinterpret_cast<sPartyMember*>( pParty->GetMemberbyKey(pEvent->uiSerial) );
				if( !pMember )
				{					
					DBO_FAIL("g_EventPartyInventory, PINVEN_DIVIDE_BY_DICE_RESULT, Not exist party member of handle : " << pEvent->uiSerial);
					NTL_RETURNVOID();
				}

				if( m_iSelectedSlot == INVALID_INDEX )
				{
					DBO_FAIL("g_EventPartyInventory, PINVEN_DIVIDE_BY_DICE_RESULT, Invaild selected index : " << m_iSelectedSlot);
					NTL_RETURNVOID();
				}				

				if( m_SlotInfo[m_iSelectedSlot].ItemProfile.bNeedToIdentify )
				{
					GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVEN_DICE_WINNER_WITH_UNIDENTIFY, FALSE, NULL, pMember->wszMemberName);
				}
				else
				{
					sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(m_SlotInfo[m_iSelectedSlot].ItemProfile.tblidx);
					if( !pITEM_TBLDAT )
					{
						NTL_ASSERT(pITEM_TBLDAT, "CPartyInvenGui::HandleEvents, g_EventPartyInventory, PINVEN_DIVIDE_BY_DICE_RESULT, Not exist item table of index : " << m_SlotInfo[m_iSelectedSlot].ItemProfile.tblidx);
						NTL_RETURNVOID();
					}

					CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
					std::wstring& strItemName = pItemTextTable->GetText(pITEM_TBLDAT->Name);

					GetAlarmManager()->FormattedAlarmMessage(DST_PARTY_INVEN_DICE_WINNER, FALSE, NULL, strItemName.c_str(), pMember->wszMemberName);
				}				

				if( m_byInfoWindowIndex != pEvent->byPos )
					GetInfoWndManager()->ShowInfoWindow( FALSE );

				m_byInfoWindowIndex	= INVALID_BYTE;
				m_iMouseDownSlot	= INVALID_INDEX;
				SelectEffect(INVALID_INDEX);

				m_Divide_by_Dice.bRollingDice = FALSE;

				break;
			}
		}
	}
	else if(msg.Id == g_EventIconMoveClick)
	{
		RwBool bPickUp = (RwBool)msg.pData;

		if( bPickUp )
		{
			if( GetIconMoveManager()->GetSrcPlace() == PLACE_SUB_BAG_ZENNY )
			{
				m_Divide_by_Oction.bZennySlotDestination = TRUE;
			}
		}
		else
		{
			m_Divide_by_Oction.bZennySlotDestination = FALSE;
		}
	}

	NTL_RETURNVOID();
}