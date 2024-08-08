#include "precomp_dboclient.h"
#include "ScouterBodyGui.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// share
#include "ItemTable.h"
#include "TextAllTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlInventory.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "TableContainer.h"

// dbo
#include "DboEvent.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "DboLogic.h"
#include "DboGlobal.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"

namespace
{
	#define dDISPLAYSLOT_X			34
	#define dDISPLAYSLOT_Y			43
	#define dSLOT_STARTX			38
	#define dSLOT_STARTY			92
	#define dSLOT_GAP_VER			38
	#define dSLOT_GAP_HORI			38
	#define dPOWERGAUGE_X			52
	#define dPOWERGAUGE_START_Y		154
	#define dPOWERGAUGE_GAP			38
	#define dPOWERSTATIC_X			93
	#define dPOWERSTATIC_START_Y	136

	#define dSCOUTERSLOT			200
};

CScouterBodyGui::CScouterBodyGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_bySelectedSlot(INVALID_BYTE)
,m_byMouseDownSlot(INVALID_BYTE)
,m_byClickEffectedSlot(INVALID_BYTE)
,m_iScouterSlotCount(0)
,m_bFocus(FALSE)
,m_byInfoWindowIndex(INVALID_BYTE)
{
}

CScouterBodyGui::~CScouterBodyGui()
{

}

RwBool CScouterBodyGui::Create()
{
	NTL_FUNCTION( "CScouterBodyGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\ScouterBody.srf", "gui\\ScouterBody.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	rect = GetPosition();

	// 스카우터 메뉴 키기
	m_pOnMenuButton = (gui::CButton*)GetComponent("btnOnMenu");
	m_slotOnMenuButton = m_pOnMenuButton->SigClicked().Connect(this, &CScouterBodyGui::OnClicked_OnMenuButton);

	// 스카우터 메뉴 끄기
	m_pOffMenuButton = (gui::CButton*)GetComponent("btnOffMenu");
	m_slotOffMenuButton = m_pOffMenuButton->SigClicked().Connect(this, &CScouterBodyGui::OnClicked_OffMenuButton);

	// 모든 파츠 제거 버튼
	m_pRemoveAllParts = (gui::CButton*)GetComponent("btnPartsRemoveAll");
	m_pRemoveAllParts->SetToolTip( GetDisplayStringManager()->GetString(DST_SCOUTER_REMOVER_ALL_PARTS) );
	m_slotRemoveAllPartsButton = m_pRemoveAllParts->SigClicked().Connect(this, &CScouterBodyGui::OnClicked_RemoveAllPartsButton);

	// 스카우터 이름
	rect.SetRectWH(70, 61, 125, 15);
	m_pScouterName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pScouterName->CreateFontStd( "detail", DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pScouterName->SetTextColor( RGB(255, 219, 71) );
	m_pScouterName->Clear();
	m_pScouterName->Enable(false);

	// 스카우터 전략량
	rect.SetRectWH(dPOWERSTATIC_X, dPOWERSTATIC_START_Y, 40, 15);
	m_pPowerStatic = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pPowerStatic->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pPowerStatic->SetTextColor( RGB(131, 222, 197) );
	m_pPowerStatic->SetText(L"0W");
	m_pPowerStatic->Enable(false);

	// 전력 게이지
	m_pPowerGauge = (gui::CProgressBar*)GetComponent( "PowerGauge" );
	m_pPowerGauge->Enable(false);

	// 스카우터 모양
	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ScouterBody.srf", "srfScouterUp") );
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ScouterBody.srf", "srfScouterCenter") );
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ScouterBody.srf", "srfScouterDown") );

	// 정보를 알아볼 스카우터의 아이콘 슬롯
	m_ScouterSlot.Create(m_pThis, DIALOG_SCOUTER_INFO, REGULAR_SLOT_ITEM_SOB);
	m_ScouterSlot.SetPosition_fromParent(dDISPLAYSLOT_X, dDISPLAYSLOT_Y);

	// 스카우터 슬롯
	for( RwInt32 i = 0 ; i < NTL_MAX_SCOUTER_ITEM_SLOT ; ++i )
	{
		m_Slot[i].Create(m_pThis, DIALOG_SCOUTER_BODY, REGULAR_SLOT_ITEM_SOB, SDS_LOCK);
		m_Slot[i].SetPosition_fromParent(dSLOT_STARTX + dSLOT_GAP_VER*( i % 4 ), dSLOT_STARTY + dSLOT_GAP_HORI*( i / 4) );
	}

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );

	// 슬롯 셀렉트 이펙트
	m_SelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );

	// 슬롯 서페이스
	m_srfSlot[0].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srf4Slots" ) );	
	m_srfSlot[1].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srf4Slots" ) );	
	m_srfSlot[2].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srf4Slots" ) );	
	m_srfSlot[3].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srf4Slots" ) );

	m_srfSlot[0].SetPositionfromParent(36, 90);
	m_srfSlot[1].SetPositionfromParent(36, 90 + dSLOT_GAP_HORI);
	m_srfSlot[2].SetPositionfromParent(36, 90 + 2*dSLOT_GAP_HORI);
	m_srfSlot[3].SetPositionfromParent(36, 90 + 3*dSLOT_GAP_HORI);

	// sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CScouterBodyGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CScouterBodyGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CScouterBodyGui::OnMove );	
	m_slotResize		= m_pThis->SigResize().Connect( this, &CScouterBodyGui::OnResize );	
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CScouterBodyGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CScouterBodyGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CScouterBodyGui::OnPaint );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CScouterBodyGui::OnCaptureMouseDown );

	Show(false);

	LinkMsg(g_EventScouter);
	LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventIconPopupResult);
	LinkMsg(g_EventPickedUpHide);
	LinkMsg(g_EventEnableItemIcon);

	NTL_RETURN(TRUE);
}

VOID CScouterBodyGui::Destroy()
{
	NTL_FUNCTION("CScouterBodyGui::Destroy");

	CheckInfoWindow();

	m_ScouterSlot.Destroy();

	for( RwInt32 i = 0 ; i < NTL_MAX_SCOUTER_ITEM_SLOT ; ++i )
		m_Slot[i].Destroy();

	UnLinkMsg(g_EventScouter);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventIconPopupResult);
	UnLinkMsg(g_EventPickedUpHide);
	UnLinkMsg(g_EventEnableItemIcon);	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

RwBool CScouterBodyGui::SetScouterBody(SERIAL_HANDLE hSerial)
{
	if( m_ScouterSlot.GetSerial() == hSerial )
		return FALSE;

	// 스카우터 바디가 바뀌었다면
	RwBool bChangedBody  = FALSE;
	RwBool bOpenedDilaog = GetDialogManager()->IsOpenDialog(DIALOG_SCOUTER_BODY);
	if( m_ScouterSlot.GetSerial() != INVALID_SERIAL_ID )
	{
		bChangedBody = TRUE;
		UnSetScouterBody();
	}

	std::wstring wstr;
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	m_ScouterSlot.SetIcon(hSerial);

	sITEM_TBLDAT* pITEM_TBLDAT = m_ScouterSlot.GetItemTable();
	if( !pITEM_TBLDAT )
	{
		DBO_FAIL("Not exist scouter item table of handle : " << hSerial);
		UnSetScouterBody();
		return FALSE;
	}

	pItemTextTable->GetText(pITEM_TBLDAT->Name, &wstr);

	m_pScouterName->SetText(wstr.c_str());	
	SetPower(m_iCurPower, pITEM_TBLDAT->wScouter_Watt);

	// 스카우터 슬롯 갯수 저장
	m_iScouterSlotCount = pITEM_TBLDAT->byBag_Size;

	CRectangle rect = m_pThis->GetPosition();

	switch( m_iScouterSlotCount )
	{
	case ITEM_BAG_SIZE_4_BY_1:
		{
			rect.SetRectWH(rect.left, rect.top, 224, 184);
			m_pRemoveAllParts->SetPosition(180, 122);
			break;
		}
	case ITEM_BAG_SIZE_4_BY_2:
		{
			rect.SetRectWH(rect.left, rect.top, 224, 222);
			m_pRemoveAllParts->SetPosition(180, 160);
			break;
		}
	case ITEM_BAG_SIZE_4_BY_3:
		{
			rect.SetRectWH(rect.left, rect.top, 224, 260);
			m_pRemoveAllParts->SetPosition(180, 198);
			break;
		}
	case ITEM_BAG_SIZE_4_BY_4:
		{
			rect.SetRectWH(rect.left, rect.top, 224, 298);
			m_pRemoveAllParts->SetPosition(180, 236);
			break;
		}
	default:
		{
			DBO_FAIL("Invalid scouter slot size : " << m_iScouterSlotCount);
			break;
		}
	}

	m_pThis->SetPosition(rect);

	if( bChangedBody )
	{
		// 스카우터를 다른 것으로 교환했을 때를 위하여 현재 다이얼로그가 보여지고 있으면
		// 스카우터 바디 창도 보여준다.
		if( bOpenedDilaog )
		{				
			GetDialogManager()->OpenDialog(DIALOG_SCOUTER_BODY, INVALID_SERIAL_ID, FALSE);
		}
		else
		{			
			GetDialogManager()->CloseDialog(DIALOG_SCOUTER_BODY, FALSE);
		}
	}

	return TRUE;
}

VOID CScouterBodyGui::UnSetScouterBody()
{
	if( m_ScouterSlot.GetSerial() != INVALID_SERIAL_ID )
	{
		m_ScouterSlot.Clear();

		// 슬롯 초기화
		if( NTL_MAX_SCOUTER_ITEM_SLOT >= m_iScouterSlotCount )
		{
			for( RwInt32 i = 0 ; i < m_iScouterSlotCount ; ++i )
			{
				m_Slot[i].Clear();
			}
		}		

		m_iScouterSlotCount = 0;
		m_pScouterName->Clear();
		m_pPowerStatic->Clear();
		m_iCurPower = 0;

		GetDialogManager()->CloseDialog(DIALOG_SCOUTER_BODY, FALSE);
	}	
}

VOID CScouterBodyGui::UpdateParts()
{	
	if( m_ScouterSlot.GetSerial() == INVALID_SERIAL_ID )
		return;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlInventory* pInventory = pAvatar->GetInventory();
	if(!pInventory)
		return;

	SERIAL_HANDLE hSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);
	CNtlSobItem* pScouterBody = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial));
	if(!pScouterBody)
		return;

	if( NTL_MAX_SCOUTER_ITEM_SLOT >= m_iScouterSlotCount )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		for( RwInt32 i = 0 ; i < m_iScouterSlotCount ; ++i )
		{
			CNtlSobItem* pScouterParts = pScouterBody->GetChildItem( i );
			if(pScouterParts)
			{
				// 새로 설정
				m_Slot[i].SetParentPosition(rtScreen.left, rtScreen.top);
				m_Slot[i].SetIcon(pScouterParts->GetSerialID());
			}
			else
				m_Slot[i].Clear();
		}
	}	
}

RwInt32 CScouterBodyGui::GetPartsCount()
{
	RwInt32 iPartsCount = 0;

	if( NTL_MAX_SCOUTER_ITEM_SLOT >= m_iScouterSlotCount )
	{
		for( RwInt32 i = 0 ; i < m_iScouterSlotCount ; ++i )
		{
			if( m_Slot[i].GetSerial() != INVALID_SERIAL_ID )
				++iPartsCount;
		}
	}

	return iPartsCount;
}

RwInt32 CScouterBodyGui::GetCurPower()
{
	return m_iCurPower;
}

VOID CScouterBodyGui::SetPower(RwInt32 iPower, RwInt32 iMaxPower)
{
	m_pPowerStatic->Format("%dW", iPower);

	m_pPowerGauge->SetRange(0, iMaxPower);
	m_pPowerGauge->SetPos(iPower);
}

RwUInt8 CScouterBodyGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	if( m_ScouterSlot.PtInRect(iX, iY) )
		return dSCOUTERSLOT;

	if( NTL_MAX_SCOUTER_ITEM_SLOT >= m_iScouterSlotCount )
	{
		for( RwUInt8 i = 0 ; i < m_iScouterSlotCount ; ++i )
		{
			if( m_Slot[i].PtInRect(iX, iY) )
			{
				return i;
			}		
		}
	}	

	return INVALID_BYTE;
}

VOID CScouterBodyGui::FocusEffect( RwBool bPush, RwUInt8 bySlot /* = INVALID_BYTE */)
{
	if(bPush)
	{
		RwInt32 iX, iY;
		CRectangle rect = m_pThis->GetScreenRect();

		if( bySlot == dSCOUTERSLOT )
		{
			iX = dDISPLAYSLOT_X;
			iY = dDISPLAYSLOT_Y;
		}
		else
		{
			iX = dSLOT_STARTX + dSLOT_GAP_VER*( bySlot % 4 );
			iY = dSLOT_STARTY + dSLOT_GAP_HORI*( bySlot / 4);			
		}


		m_FocusEffect.SetRectWH(rect.left + iX, rect.top + iY, 32, 32);
		m_bFocus = TRUE;
	}
	else
		m_bFocus = FALSE;
}

VOID CScouterBodyGui::SelectEffect( RwBool bPush, RwUInt8 bySlot /* = INVALID_BYTE */)
{
	if( bPush)
	{
		RwInt32 iX = dSLOT_STARTX + dSLOT_GAP_VER*( bySlot % 4 );
		RwInt32 iY = dSLOT_STARTY + dSLOT_GAP_HORI*( bySlot / 4);
		CRectangle rect = m_pThis->GetScreenRect();

		m_SelectEffect.SetRectWH(rect.left + iX, rect.top + iY, 32, 32);
		m_bySelectedSlot = bySlot;
	}
	else
		m_bySelectedSlot = INVALID_BYTE;
}

VOID CScouterBodyGui::OnClicked_OnMenuButton(gui::CComponent* pComponent)
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_SCOUTER_ON_MODE ) )
		return;

	CNtlSLEventGenerator::ActionMapScouterUse();
	GetDialogManager()->CloseDialog(DIALOG_SCOUTER_BODY);
}

VOID CScouterBodyGui::OnClicked_OffMenuButton(gui::CComponent* pComponent)
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_SCOUTER_OFF_MODE ) )
		return;

	CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_EXIT, Logic_GetAvatarHandle());
	GetDialogManager()->CloseDialog(DIALOG_SCOUTER_BODY);
}

VOID CScouterBodyGui::OnClicked_RemoveAllPartsButton(gui::CComponent* pComponent)
{
	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_SCOUTER_ON_MODE ) )
		return;

	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_SCOUTER_OFF_MODE ) )
		return;

	GetDboGlobal()->GetGamePacketGenerator()->SendScouterChipRemoveAllReq();
}

VOID CScouterBodyGui::OnMouseDown(const CKey& key)
{
	if( key.m_nID == UD_RIGHT_BUTTON && GetIconMoveManager()->IsActive() )
		return;

	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	RwUInt8 bySlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( Logic_IsSavableIndex(bySlot, NTL_MAX_SCOUTER_ITEM_SLOT, INVALID_BYTE) ||
		dSCOUTERSLOT == bySlot )
	{
		m_byMouseDownSlot = bySlot;				
		m_pThis->CaptureMouse();	

		// 클릭 이벤트 시작
		if( Logic_IsUsableIndex(bySlot, NTL_MAX_SCOUTER_ITEM_SLOT, INVALID_BYTE) )
		{
			m_byClickEffectedSlot = bySlot;
			m_Slot[m_byClickEffectedSlot].ClickEffect(TRUE);
		}
	}	
}

VOID CScouterBodyGui::OnMouseUp(const CKey& key)
{	
	m_pThis->ReleaseMouse();	

	// 클릭 이벤트 종료	
	if( m_byClickEffectedSlot != INVALID_BYTE )
	{
		if( FALSE == Logic_IsUsableIndex(m_byClickEffectedSlot, NTL_MAX_SCOUTER_ITEM_SLOT, INVALID_BYTE) )
		{
			m_Slot[m_byClickEffectedSlot].ClickEffect(FALSE);
			m_byClickEffectedSlot = INVALID_BYTE;
		}
	}

	if( !IsShow() )
	{
		m_byMouseDownSlot = INVALID_BYTE;
		return;
	}	

	if( m_byMouseDownSlot < 0 || m_byMouseDownSlot >= m_iScouterSlotCount )
	{
		m_byMouseDownSlot = INVALID_BYTE;
		return;
	}

	RwUInt8 bySlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( m_byMouseDownSlot != bySlot )
	{
		m_byMouseDownSlot = INVALID_BYTE;
		return;
	}


	if( Logic_IsUsableIndex(bySlot, NTL_MAX_SCOUTER_ITEM_SLOT, INVALID_BYTE) )
	{
		if( m_Slot[bySlot].IsShowLock() == FALSE )
		{
			if( key.m_nID == UD_LEFT_BUTTON )
			{
				if( GetIconMoveManager()->IsActive() )
				{	
					GetIconMoveManager()->IconMovePutDown(PLACE_SCOUTER, m_ScouterSlot.GetSerial(), bySlot);
				}
				else
				{
					// 스카우터 안에서 파츠를 픽업했을 경우
					if( m_Slot[bySlot].GetSerial() != INVALID_SERIAL_ID )
					{
						// PLACE_BAG : 스카우터도 가방으로 취급한다
						GetIconMoveManager()->IconMovePickUp(m_Slot[bySlot].GetSerial(), PLACE_SCOUTER, bySlot, 1, 
							m_Slot[bySlot].GetTexture() ); // 1: 스카우터 파츠에서 가지고 온 것을 알려준다

						SelectEffect(TRUE, bySlot);
					}
				}
			}
			else if( key.m_nID == UD_RIGHT_BUTTON )
			{
				if( GetIconMoveManager()->IsActive() == FALSE && m_Slot[bySlot].GetSerial() != INVALID_SERIAL_ID )
				{
					RwUInt32 uiBagIndex;
					SERIAL_HANDLE hBagHandle;
					RwUInt32 uiSlot_of_Bag;

					Logic_GetFirstEmptySlot_of_Bag(uiBagIndex, hBagHandle, uiSlot_of_Bag);

					if( uiBagIndex != INVALID_BYTE)
					{
						// 바로 가방으로 옮긴다
						Logic_ItemMoveProc(m_Slot[bySlot].GetSerial(), PLACE_SCOUTER, bySlot, PLACE_BAG, hBagHandle, (RwUInt8)uiSlot_of_Bag);
					}
				}
			}
		}
	}
	
	m_byMouseDownSlot = INVALID_BYTE;
}

VOID CScouterBodyGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	LocateComponent();
	m_bFocus = FALSE;
	MoveLinkedPLGui(rtScreen.left - iOldX, rtScreen.top - iOldY);
	CheckInfoWindow();
}

VOID CScouterBodyGui::OnResize(RwInt32 iOldW, RwInt32 iOldH)
{
	LocateComponent();
}

VOID CScouterBodyGui::LocateComponent()
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_BackPanel.SetRectWH(rtScreen.left, rtScreen.top, rtScreen.GetWidth(), rtScreen.GetHeight());

	m_ScouterSlot.SetParentPosition(rtScreen.left, rtScreen.top);

	// CScouterBodyGui 다이얼로그가 움직일 때	
	for( RwInt32 i = 0 ; i < NTL_MAX_SCOUTER_ITEM_SLOT ; ++i )
	{
		m_Slot[i].SetParentPosition(rtScreen.left, rtScreen.top);
	}

	m_srfSlot[0].SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfSlot[1].SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfSlot[2].SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfSlot[3].SetPositionbyParent(rtScreen.left, rtScreen.top);

	switch( m_iScouterSlotCount )
	{
	case ITEM_BAG_SIZE_4_BY_1:
		{
			m_pPowerGauge->SetPosition(dPOWERGAUGE_X, dPOWERGAUGE_START_Y);
			m_pPowerStatic->SetPosition(dPOWERSTATIC_X, dPOWERSTATIC_START_Y);
			break;
		}
	case ITEM_BAG_SIZE_4_BY_2:
		{
			m_pPowerGauge->SetPosition(dPOWERGAUGE_X, dPOWERGAUGE_START_Y + dPOWERGAUGE_GAP);
			m_pPowerStatic->SetPosition(dPOWERSTATIC_X, dPOWERSTATIC_START_Y + dPOWERGAUGE_GAP);
			break;
		}
	case ITEM_BAG_SIZE_4_BY_3:
		{
			m_pPowerGauge->SetPosition(dPOWERGAUGE_X, dPOWERGAUGE_START_Y + 2*dPOWERGAUGE_GAP);
			m_pPowerStatic->SetPosition(dPOWERSTATIC_X, dPOWERSTATIC_START_Y + 2*dPOWERGAUGE_GAP);
			break;
		}
	case ITEM_BAG_SIZE_4_BY_4:
		{
			m_pPowerGauge->SetPosition(dPOWERGAUGE_X, dPOWERGAUGE_START_Y + 3*dPOWERGAUGE_GAP);
			m_pPowerStatic->SetPosition(dPOWERSTATIC_X, dPOWERSTATIC_START_Y + 3*dPOWERGAUGE_GAP);
			break;
		}
	}
}

VOID CScouterBodyGui::ShowDisableSlot(RwBool bShow, RwUInt8 bySlot)
{
	if( Logic_IsUsableIndex(bySlot, NTL_MAX_SCOUTER_ITEM_SLOT, INVALID_BYTE) )
		m_Slot[bySlot].ShowLock(bShow);
}

VOID CScouterBodyGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_SCOUTER_BODY )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CScouterBodyGui::OnMouseMove(RwInt32 nFlags, RwInt32 iX, RwInt32 iY)
{	
	RwUInt8 bySlot = PtinSlot(iX, iY);

	if( bySlot == dSCOUTERSLOT )
	{
		// 슬롯 활성화 이벤트 처리
		FocusEffect(TRUE, dSCOUTERSLOT);

		if( m_ScouterSlot.GetSerial() != INVALID_SERIAL_ID )
		{
			if( m_byInfoWindowIndex != bySlot )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM,
													rtScreen.left + m_ScouterSlot.GetX_fromParent(),
													rtScreen.top + m_ScouterSlot.GetY_fromParent(),
													m_ScouterSlot.GetSobItem(), DIALOG_SCOUTER_BODY );
				m_byInfoWindowIndex = bySlot;
			}			
		}

		return;
	}
	else if( Logic_IsUsableIndex(bySlot, NTL_MAX_SCOUTER_ITEM_SLOT, INVALID_BYTE) )
	{
		// 슬롯 활성화 이벤트 처리
		FocusEffect(TRUE, bySlot);

		// 슬롯 클릭 이펙트
		if( Logic_IsSavableIndex(m_byClickEffectedSlot, NTL_MAX_SCOUTER_ITEM_SLOT, INVALID_BYTE) )
		{
			if( m_byClickEffectedSlot == bySlot )
				m_Slot[m_byClickEffectedSlot].ClickEffect(TRUE);
			else
				m_Slot[m_byClickEffectedSlot].ClickEffect(FALSE);
		}
		
		// 아이템 정보를 보일 필요가 있는지 검사
		if( m_Slot[bySlot].GetSerial() != INVALID_SERIAL_ID )
		{
			if( m_byInfoWindowIndex != bySlot )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_ITEM,
													rtScreen.left + m_Slot[bySlot].GetX_fromParent(),
													rtScreen.top + m_Slot[bySlot].GetY_fromParent(),
													m_Slot[bySlot].GetSobItem(), DIALOG_SCOUTER_BODY );
				m_byInfoWindowIndex = bySlot;
			}

			return;
		}
	}
		
	FocusEffect(FALSE);

	if( Logic_IsUsableIndex(m_byClickEffectedSlot, NTL_MAX_SCOUTER_ITEM_SLOT, INVALID_BYTE) )
	{
		m_Slot[m_byClickEffectedSlot].ClickEffect(FALSE);
	}

	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScouterBodyGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_BYTE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScouterBodyGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_SCOUTER_BODY, key.m_fX, key.m_fY);
}

VOID CScouterBodyGui::OnPaint()
{
	m_BackPanel.Render();

	switch( m_iScouterSlotCount )
	{
	case ITEM_BAG_SIZE_4_BY_1:
		{
			m_srfSlot[0].Render();
			break;
		}
	case ITEM_BAG_SIZE_4_BY_2:
		{
			m_srfSlot[0].Render();
			m_srfSlot[1].Render();
			break;
		}
	case ITEM_BAG_SIZE_4_BY_3:
		{
			m_srfSlot[0].Render();
			m_srfSlot[1].Render();
			m_srfSlot[2].Render();
			break;
		}
	case ITEM_BAG_SIZE_4_BY_4:
		{
			m_srfSlot[0].Render();
			m_srfSlot[1].Render();
			m_srfSlot[2].Render();
			m_srfSlot[3].Render();
			break;
		}
	default:
		{
			DBO_FAIL("Invalid scouter slot size : " << m_iScouterSlotCount);
			break;
		}
	}

	m_ScouterSlot.Paint();

	if( NTL_MAX_SCOUTER_ITEM_SLOT >= m_iScouterSlotCount )
	{
		for( RwInt32 i = 0 ; i < m_iScouterSlotCount ; ++i )
		{
			m_Slot[i].Paint();
		}
	}	

	if( m_bySelectedSlot != INVALID_BYTE )
		m_SelectEffect.Render();

	if( m_bFocus )
		m_FocusEffect.Render();
}

RwInt32 CScouterBodyGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if( !pAvatar )
		{
			DBO_FAIL("Not exist avatar instance");
			return -1;
		}

		CNtlInventory*	pInventory	= pAvatar->GetInventory();
		SERIAL_HANDLE	hHandle		= pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);

		if( hHandle == INVALID_SERIAL_ID )
			return INVALID_BYTE;

		GetDialogManager()->OpenDialog(DIALOG_SCOUTER_INFO);
	}
	else
	{
		GetDialogManager()->CloseDialog(DIALOG_SCOUTER_INFO);

		if( GetIconMoveManager()->GetSrcPlace() == PLACE_SCOUTER )
			GetIconMoveManager()->IconMoveEnd();

		CheckInfoWindow();

		m_byInfoWindowIndex = INVALID_BYTE;
		FocusEffect(FALSE);
		SelectEffect(FALSE);
	}

	Show(bOpen);
	return 1;
}

VOID CScouterBodyGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION( "CScouterBodyGui::HandleEvents" );

	if( msg.Id == g_EventScouter )
	{
		SDboEventScouter* pPacket = (SDboEventScouter*)msg.pData;
		switch(pPacket->iType)
		{
		case SCOUTER_EVENT_ENTER:
			{
				m_pOnMenuButton->Show(false);
				m_pOffMenuButton->Show(true);
				break;
			}
		case SCOUTER_EVENT_EXIT:
			{
				m_pOnMenuButton->Show(true);
				m_pOffMenuButton->Show(false);
				break;
			}
		case SCOUTER_EVENT_SET_SCOUTER:
			{
				// 새로운 스카우터를 등록한다
				SetScouterBody(pPacket->uiUserData);
				break;
			}
		case SCOUTER_EVENT_UNSET_SCOUTER:
			{
				UnSetScouterBody();
				break;
			}
		case SCOUTER_EVENT_CURRENT_POWER:
			{
				m_iCurPower = (RwInt32)pPacket->uiUserData;

				if(m_ScouterSlot.GetSerial() != INVALID_SERIAL_ID)
					SetPower(m_iCurPower, m_ScouterSlot.GetItemTable()->wScouter_Watt);
				break;
			}
		}
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		if( Logic_IsUpdateType(EVENT_AIUT_ITEM, msg.pData) == FALSE )
			NTL_RETURNVOID();

		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		if( Logic_GetAvatarHandle() != pEvent->hSerialId )
			NTL_RETURNVOID();


		UpdateParts();
	}
	else if( msg.Id == g_EventIconPopupResult )
	{
		SDboEventIconPopupResult* pPacket = reinterpret_cast<SDboEventIconPopupResult*>( msg.pData );

		if( pPacket->nSrcPlace != PLACE_SCOUTER )
			return;

		if( pPacket->nWorkId == PMW_PICKUP )
		{
			if( Logic_IsUsableIndex(pPacket->nSrcSlotIdx, NTL_MAX_SCOUTER_ITEM_SLOT, INVALID_BYTE) )
			{
				GetIconMoveManager()->IconMovePickUp(pPacket->uiSerial, PLACE_SCOUTER, pPacket->nSrcSlotIdx,
													 1, m_Slot[pPacket->nSrcSlotIdx].GetTexture() );
			}			
		}		
	}
	else if( msg.Id == g_EventPickedUpHide )
	{
		RwInt32 nSlotIdx = (RwInt32)msg.pData;
		if( nSlotIdx != PLACE_SCOUTER )
			return;

		SelectEffect(FALSE);
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		SDboEventEnableItemIcon* pData = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );

		if( pData->ePlace != PLACE_SCOUTER )
			return;

		ShowDisableSlot(!pData->bEnable, (RwUInt8)pData->uiSlotIdx);
	}

	NTL_RETURNVOID();
}