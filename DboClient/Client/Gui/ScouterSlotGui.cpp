#include "precomp_dboclient.h"
#include "ScouterSlotGui.h"

// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"
#include "NtlResultCode.h"
#include "TableContainer.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlInventory.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSobItemAttr.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"

// dbo
#include "DboEvent.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "NtlSobManager.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"
#include "ChatGui.h"
#include "AlarmManager.h"
#include "DisplayStringManager.h"



CScouterSlotGui::CScouterSlotGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_iMouseDownSlot(INVALID_INDEX)
,m_bFocus(FALSE)
,m_bIconMoveClickEvent(FALSE)
,m_bSelected(FALSE)
,m_bClickEffect(FALSE)
{

}

CScouterSlotGui::~CScouterSlotGui()
{

}

RwBool CScouterSlotGui::Create()
{
	NTL_FUNCTION( "CScouterSlotGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\ScouterSlot.srf", "gui\\ScouterSlot.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_QUICKSLOT );


	// 스카우터 슬롯
	m_slot.Create(m_pThis, DIALOG_SCOUTER_SLOT, REGULAR_SLOT_ITEM_SOB, SDS_LOCK);
	m_slot.SetPosition_fromParent(0, 0);	

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );
	m_FocusEffect.SetRectWH(935, 715, 32, 32);

	// 슬롯 셀렉트 이펙트
	m_SelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );
	m_SelectEffect.SetRectWH(935, 715, 32, 32);

	// sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CScouterSlotGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CScouterSlotGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CScouterSlotGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CScouterSlotGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CScouterSlotGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CScouterSlotGui::OnPaint );

	LinkMsg(g_EventScouter);
	LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventIconMoveClick);
	LinkMsg(g_EventIconPopupResult);
	LinkMsg(g_EventEnableItemIcon);
	LinkMsg(g_EventGameServerChangeOut);

	NTL_RETURN(TRUE);
}

VOID CScouterSlotGui::Destroy()
{
	NTL_FUNCTION("CScouterSlotGui::Destroy");

	CheckInfoWindow();

	UnLinkMsg(g_EventScouter);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventIconMoveClick);
	UnLinkMsg(g_EventIconPopupResult);
	UnLinkMsg(g_EventEnableItemIcon);
	UnLinkMsg(g_EventGameServerChangeOut);

	m_slot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CScouterSlotGui::OnMouseDown(const CKey& key)
{
	if( m_slot.PtInRect((int)key.m_fX, (int)key.m_fY) )
	{
		m_iMouseDownSlot = 1;
		m_bClickEffect = TRUE;
		m_slot.ClickEffect(m_bClickEffect);
		m_pThis->CaptureMouse();
	}	
}

VOID CScouterSlotGui::OnMouseUp(const CKey& key)
{	
	m_bClickEffect = FALSE;
	m_slot.ClickEffect(m_bClickEffect);
	m_pThis->ReleaseMouse();

	if( !IsShow() )
	{
		m_iMouseDownSlot = INVALID_INDEX;
		return;
	}

	if( !Logic_CanMouseInput_in_Tutorial( ETL_MOUSE_INPUT_TYPE_SCOUTER_SLOT ) )
	{
		m_iMouseDownSlot = INVALID_INDEX;
		return;
	}

	if( m_iMouseDownSlot == 1 )
	{
		if( m_slot.PtInRect((int)key.m_fX, (int)key.m_fY) )
		{
			if( m_slot.IsShowLock() == FALSE )
			{
				if( key.m_nID == UD_LEFT_BUTTON )
				{
					if( GetIconMoveManager()->IsActive() )
					{						
						RwUInt32 uiSobSerial = GetIconMoveManager()->GetSrcSerial();
						sITEM_TBLDAT* pITEM_TBLDAT = NULL;

						if( GetIconMoveManager()->GetSrcPlace() == PLACE_GUILD_WAREHOUSE )
						{
							CNtlGuildWarehouse* pGuildWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse();
							sGuildWarehouseSlot* pSlotData = pGuildWarehouse->GetItem( GetIconMoveManager()->GetSrcSerial() );
							NTL_ASSERT(pSlotData, "CScouterSlotGui::OnMouseUp, Not exist guild warehouse item of serial : " << GetIconMoveManager()->GetSrcSerial());
							pITEM_TBLDAT = pSlotData->pITEM_TBLDAT;
						}
						else
							pITEM_TBLDAT = Logic_GetItemDataFromSob(uiSobSerial);

						if(pITEM_TBLDAT)
						{
							if( pITEM_TBLDAT->byItem_Type == ITEM_TYPE_SCOUTER )
							{
								// 가방에서 스카우터 바디를 등록시키는 경우
								GetIconMoveManager()->IconMovePutDown(PLACE_SCOUTER_SLOT, INVALID_SERIAL_ID, EQUIP_SLOT_TYPE_SCOUTER);
							}
							else if( pITEM_TBLDAT->byItem_Type == ITEM_TYPE_SCOUTER_PART )
							{
								// 가방에서 스카우터 파츠를 등록시키는 경우
								RwUInt8 byIndex = Logic_GetFirstEmptyScouterSlot();

								if( byIndex != INVALID_BYTE )
								{
									GetIconMoveManager()->IconMovePutDown(PLACE_SCOUTER, m_slot.GetSerial(), (RwUInt32)byIndex);

									GetDialogManager()->OpenDialog(DIALOG_SCOUTER_BODY);
								}
								else
								{
									// 스카우터에 파츠가 가득 찼습니다
									GetAlarmManager()->AlarmMessage(DST_SCOUTER_FULL);
								}
							}
							else
							{
								// 그곳에 옮길 수 없습니다
								GetAlarmManager()->AlarmMessage(DST_ITEM_CAN_NOT_MOVE_THERE);
							}
						}				
					}
					else if( GetDialogManager()->IsMode( DIALOGMODE_ITEM_REPAIR ) )
					{
						Logic_ItemRepairProc(CONTAINER_TYPE_EQUIP, EQUIP_SLOT_TYPE_SCOUTER);					
					}
					else if( m_slot.GetSerial() != INVALID_SERIAL_ID )
					{				
						CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(m_slot.GetSerial()) );

						if( pItem->ExistChild() )
						{
							// 스카우터를 먼저 비워야합니다
							GetAlarmManager()->AlarmMessage(DST_SCOUTER_MUST_CLEAR_SCOUTER);
						}
						else
						{
							GetIconMoveManager()->IconMovePickUp(m_slot.GetSerial(), PLACE_SCOUTER_SLOT, 0, 1, m_slot.GetTexture() );
						}					
					}	
				}
				else if( key.m_nID == UD_RIGHT_BUTTON )
				{
					if( GetIconMoveManager()->IsActive() == FALSE && m_slot.GetSerial() != INVALID_SERIAL_ID )
					{
						if( !GetIconMoveManager()->IsActive() )
						{
							// 스카우터 바디가 장착되어 있다면 스카우터 정보창, 스카우터 가방을 열고 닫는다
							GetDialogManager()->SwitchDialog(DIALOG_SCOUTER_BODY);
						}
					}
				}
			}
		}
	}

	m_iMouseDownSlot = INVALID_INDEX;
}

VOID CScouterSlotGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pThis->GetScreenRect();

	m_slot.SetParentPosition(rect.left, rect.top);

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetRectWH(rect.left, rect.top, 32, 32);

	// 슬롯 셀렉트 이펙트
	m_SelectEffect.SetRectWH(rect.left, rect.top, 32, 32);

	CheckInfoWindow();
}

VOID CScouterSlotGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_slot.PtInRect(nX, nY) )
	{
		// 슬롯 이벤트
		m_slot.ClickEffect(m_bClickEffect);

		// 슬롯 포커스 이벤트
		m_bFocus = TRUE;

		// 스카우터 인포창 처리
		SERIAL_HANDLE hScouter = m_slot.GetSerial();

		if( hScouter != INVALID_SERIAL_ID )
		{
			if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_SCOUTER_SLOT )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_SCOUTER_SLOT,
													rtScreen.left + m_slot.GetX_fromParent(),
													rtScreen.top + m_slot.GetY_fromParent(),
													(VOID*)&hScouter, DIALOG_SCOUTER_SLOT );
			}
		}
		else
		{
			if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_JUST_WTEXT )
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
													rtScreen.left + m_slot.GetX_fromParent(),
													rtScreen.top + m_slot.GetY_fromParent(),
													(VOID*)GetDisplayStringManager()->GetString(DST_SCOUTER_SCOUTER_NAME), DIALOG_SCOUTER_SLOT );
			}
		}

		return;
	}
	else
		m_bFocus = FALSE;

	m_slot.ClickEffect(FALSE);
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScouterSlotGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_slot.ClickEffect(FALSE);
	m_bFocus = FALSE;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScouterSlotGui::ShowIconDestination()
{
	if( GetIconMoveManager()->IsActive() )
	{
		if( GetIconMoveManager()->GetSrcPlace() == PLACE_WAREHOUSE ||
			GetIconMoveManager()->GetSrcPlace() == PLACE_BAG )
		{
			SERIAL_HANDLE hHandle = GetIconMoveManager()->GetSrcSerial();
			CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hHandle ) );

			if( !pSobItem )
			{
				DBO_FAIL("Not exist sob item of handle : " << hHandle);
				m_bIconMoveClickEvent	= FALSE;
				m_bSelected				= FALSE;
				return;
			}

			if( m_slot.GetSerial() == pSobItem->GetSerialID() )
			{
				m_bIconMoveClickEvent		= FALSE;
				m_bSelected					= TRUE;
			}
			else if( pSobItem->IsScouterItem() )
			{
				m_bIconMoveClickEvent	= TRUE;
				m_bSelected				= FALSE;
			}
			else if( m_slot.GetSerialType() != INVALID_SERIAL_ID )
			{
				if( pSobItem->IsScouterPartItem() )
				{
					m_bIconMoveClickEvent	= TRUE;
					m_bSelected				= FALSE;
				}
			}
			else
			{
				m_bIconMoveClickEvent	= FALSE;
				m_bSelected				= FALSE;
			}
		}
		else if( GetIconMoveManager()->GetSrcPlace() == PLACE_GUILD_WAREHOUSE )
		{
			CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
			if( !pAvatar )
				return;

			CNtlGuildWarehouse* pGuildWarehouse = pAvatar->GetGuildWarehouse();
			if( !pGuildWarehouse )
				return;

			sGuildWarehouseSlot* pSlotData = pGuildWarehouse->GetItem( GetIconMoveManager()->GetSrcSerial() );
			if( !pSlotData )
			{
				DBO_FAIL("Not exist guild warehouse item of serial : " << GetIconMoveManager()->GetSrcSerial());
			}
			
			sITEM_TBLDAT* pITEM_TBLDAT = pSlotData->pITEM_TBLDAT;
			if( !pITEM_TBLDAT )
			{
				DBO_FAIL("Not exist item table of of handle : " << GetIconMoveManager()->GetSrcSerial() << " in guild warehouse");
				m_bIconMoveClickEvent	= FALSE;
				m_bSelected				= FALSE;
				return;
			}

			if( ITEM_TYPE_SCOUTER == pITEM_TBLDAT->byItem_Type )
			{
				m_bIconMoveClickEvent	= TRUE;
				m_bSelected				= FALSE;
			}
			else
			{
				m_bIconMoveClickEvent	= FALSE;
				m_bSelected				= FALSE;
			}
		}
	}
	else
	{
		m_bIconMoveClickEvent	= FALSE;
		m_bSelected				= FALSE;
	}
}

VOID CScouterSlotGui::ShowDisableSlot(RwBool bShow)
{
	m_slot.ShowLock(bShow);
}

VOID CScouterSlotGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_SCOUTER_SLOT )
		GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CScouterSlotGui::OnPaint()
{
	m_slot.Paint();

	if( m_bFocus || m_bIconMoveClickEvent )
		m_FocusEffect.Render();

	if( m_bSelected )
		m_SelectEffect.Render();
}

RwInt32 CScouterSlotGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		Show(false);
		CheckInfoWindow();		
	}

	return 1;
}

VOID CScouterSlotGui::SetScouterBody(SERIAL_HANDLE hHandle)
{		
	if( hHandle == INVALID_SERIAL_ID )
		return;

	// 스카우터 바디가 바뀌었다면
	if( m_slot.GetSerial() != INVALID_SERIAL_ID )
	{
		UnSetScouterBody();
	}	

	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_slot.SetIcon(hHandle);
	m_slot.SetParentPosition(rtScreen.left, rtScreen.top);
}

VOID CScouterSlotGui::UnSetScouterBody()
{
	if( m_slot.GetSerial() != INVALID_SERIAL_ID )
		m_slot.Clear();
}

VOID CScouterSlotGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CScouterSlotGui::HandleEvents");

	if( msg.Id == g_EventScouter )
	{		
		SDboEventScouter* pPacket = (SDboEventScouter*)msg.pData;
		switch(pPacket->iType)
		{
		case SCOUTER_EVENT_SET_SCOUTER:
			{				
				SetScouterBody(pPacket->uiUserData);
				break;
			}
		case SCOUTER_EVENT_UNSET_SCOUTER:
			{
				UnSetScouterBody();
				break;
			}
		}
	}
	else if( msg.Id == g_EventSobInfoUpdate )
	{
		// 스카우터 바디의 업데이트
		if( Logic_IsUpdateType(EVENT_AIUT_ITEM, msg.pData) == FALSE )
			NTL_RETURNVOID();

		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );
		if( Logic_GetAvatarHandle() != pEvent->hSerialId )
			NTL_RETURNVOID();

		// 각각의 ScouterBodyGui, ScouterGui 클래스에서 검사를 해도 되지만
		// 불필요한 중복 검사를 막고 스카우터 바디 관련 이벤트 발생 시기를
		// 한 곳에서 관리하기 위하여 여기서 검사한다.
		CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
		SERIAL_HANDLE hHandle = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);

		if( hHandle != INVALID_SERIAL_ID )
		{
			if( m_slot.GetSerial() != hHandle )
			{
				CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_SET_SCOUTER, hHandle );
			}
			else
			{
				CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hHandle));
				if(!pSobItem)
					return;

				CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());

				if( pSobItemAttr->GetDur() <= 1 )
				{
					// 스카우터 내구력이 1이하면 고장 이펙트를 보여준다.
					CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_OUT_OF_ORDER);
				}
			}
		}
		else
			CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_UNSET_SCOUTER);
	}
	else if( msg.Id == g_EventIconMoveClick )
	{
		if( GetIconMoveManager()->GetSrcPlace() == PLACE_WAREHOUSE			||
			GetIconMoveManager()->GetSrcPlace() == PLACE_GUILD_WAREHOUSE	||			
			GetIconMoveManager()->GetSrcPlace() == PLACE_BAG )
				ShowIconDestination();
	}
	else if( msg.Id == g_EventIconPopupResult )
	{
		SDboEventIconPopupResult* pPacket = reinterpret_cast<SDboEventIconPopupResult*>( msg.pData );

		if( pPacket->nSrcPlace != PLACE_SCOUTER_SLOT )
			NTL_RETURNVOID();

		if( pPacket->nWorkId == PMW_VIEW )
		{
			GetDialogManager()->OpenDialog(DIALOG_SCOUTER_INFO);
			GetDialogManager()->OpenDialog(DIALOG_SCOUTER_BODY);
		}
		else if( pPacket->nWorkId == PMW_PICKUP )
		{
			// 자식 아이템이 있으면 옮길 수 없다
			CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(pPacket->uiSerial));
			if( pSobItem->ExistChild() )
			{
				GetAlarmManager()->AlarmMessage( GAME_SCOUTER_BODY_IS_NOT_EMPTY );
				NTL_RETURNVOID();
			}

			GetIconMoveManager()->IconMovePickUp( pPacket->uiSerial, PLACE_SCOUTER_SLOT, 0,	1, m_slot.GetTexture() );
		}		
	}
	else if( msg.Id == g_EventEnableItemIcon )
	{
		SDboEventEnableItemIcon* pData = reinterpret_cast<SDboEventEnableItemIcon*>( msg.pData );

		if( pData->ePlace == PLACE_SCOUTER_SLOT ||
			( pData->ePlace == PLACE_EQUIP && pData->uiSlotIdx == EQUIP_SLOT_TYPE_SCOUTER ) )
			ShowDisableSlot(!pData->bEnable);
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_UNSET_SCOUTER);
	}

	NTL_RETURNVOID();
}