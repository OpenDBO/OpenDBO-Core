#include "precomp_dboclient.h"
#include "ScouterGui.h"

// core
#include "NtlDebug.h"

// shared
#include "PCTable.h"
#include "ItemOptionTable.h"
#include "ItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// sound
#include "GUISoundDefine.h"

// presetation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLEventfunc.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"
#include "NtlSobItem.h"
#include "NtlInventory.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"

// dbo
#include "DboEvent.h"
#include "IconMoveManager.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "DboTableInfo.h"
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"

CScouterGui::CScouterGui(const RwChar* pName)
:CNtlPLGui( pName )
,m_pPartsListBox(NULL)
,m_hScouterHandle(INVALID_SERIAL_ID)
{
	
}

CScouterGui::~CScouterGui()
{

}

RwBool CScouterGui::Create()
{
	NTL_FUNCTION( "CScouterGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\Scouter.srf", "gui\\Scouter.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	m_pExitButton = (gui::CButton*)GetComponent("btnExit");
	m_slotExitButton = m_pExitButton->SigClicked().Connect(this, &CScouterGui::OnClicked_ExitButton);

	// 파츠 옵션 리스트박스
	m_pPartsListBox = (gui::CListBox*)GetComponent( "listParts" );
	m_pPartsListBox->SetTextColor( RGB(185, 227, 166) );
	m_pPartsListBox->SetSelectedMaskColor(0, 0, 0, 0);
	m_pPartsListBox->SetSelectedTextColor( RGB(185, 227, 166) );

	// sig
	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CScouterGui::OnCaptureWheelMove );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CScouterGui::OnCaptureMouseDown );

	LinkMsg(g_EventScouter);
	LinkMsg(g_EventSobInfoUpdate);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CScouterGui::Destroy()
{
	NTL_FUNCTION("CScouterGui::Destroy");

	UnLinkMsg(g_EventScouter);
	UnLinkMsg(g_EventSobInfoUpdate);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CScouterGui::SetScouterInfo(RwUInt32 uiSerial)
{	
	RwBool bOpened = GetDialogManager()->IsOpenDialog(DIALOG_SCOUTER_INFO);

	// 스카우터 바디가 바뀌었다면
	if( m_hScouterHandle != INVALID_SERIAL_ID )
	{
		UnSetScouterInfo();		
	}

	m_hScouterHandle = uiSerial;

	CRectangle rect = m_pThis->GetScreenRect();

	// 스카우터를 다른 것으로 교환했을 때를 위하여 현재 다이얼로그가 보여지고 있으면
	// 스카우터 바디 창도 보여준다.
	if( bOpened )
	{				
		GetDialogManager()->OpenDialog(DIALOG_SCOUTER_INFO);
	}
	else
	{			
		GetDialogManager()->CloseDialog(DIALOG_SCOUTER_INFO);
	}
}

VOID CScouterGui::UnSetScouterInfo()
{
	if( m_hScouterHandle != INVALID_SERIAL_ID )
	{
		GetDialogManager()->CloseDialog(DIALOG_SCOUTER_INFO);

		m_pPartsListBox->ClearAllItems();		

		m_hScouterHandle = INVALID_SERIAL_ID;
	}
}

VOID CScouterGui::UpdatePartsInfo()
{
	if( m_hScouterHandle == INVALID_SERIAL_ID )
		return;

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	RwUInt32 uiSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);
	CNtlSobItem* pScouterBody = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(uiSerial));	
	
	RwInt32 iCurrentPower = 0;
	CNtlSobItem* pScouterParts;
	sITEM_TBLDAT* pPART_ITEM_TBLDAT;
	sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromSob(m_hScouterHandle);
	if(!pITEM_TBLDAT)
		return;

	// 일단 리스트박스의 내용을 지운다.
	m_pPartsListBox->ClearAllItems();

	m_mapPartsExplain.clear();

	for( RwInt32 i = 0 ; i < pITEM_TBLDAT->byBag_Size ; ++i )
	{
		// 스카우터 바디안의 각 슬롯에 파츠가 있다면 
		pScouterParts = pScouterBody->GetChildItem( i );
		if(pScouterParts)
		{
			// 파츠의 정보를 읽어와서
			pPART_ITEM_TBLDAT = Logic_GetItemDataFromSob(pScouterParts->GetSerialID());
			if(pPART_ITEM_TBLDAT)
			{
				// 파츠 정보를 가공한다
				ReadyPartsExplain(pPART_ITEM_TBLDAT->byScouter_Parts_Value, pPART_ITEM_TBLDAT->Item_Option_Tblidx);				

				// 현재의 전력 수치
				iCurrentPower += pPART_ITEM_TBLDAT->wScouter_Watt;
			}			
		}
	}

	CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_CURRENT_POWER, iCurrentPower);

	// 파츠 정보를 보여준다
	DisplayPartsInfo();
}

VOID CScouterGui::ReadyPartsExplain(RwUInt32 uiPartsValue, RwUInt32 uiItemOptionTableIndex)
{
	sITEM_OPTION_TBLDAT* pItemOption = reinterpret_cast<sITEM_OPTION_TBLDAT*>(API_GetTableContainer()->GetItemOptionTable()->FindData(uiItemOptionTableIndex));

	// 일정 수치를 올려주는 파츠가 아니라면 아이템 옵션 정보가 없을 수도 있다
	if(!pItemOption)
		return;

	MAP_PARTS_EXPLAIN_ITER it = m_mapPartsExplain.find(pItemOption->byScouterInfo);
	if( it == m_mapPartsExplain.end() )
	{
		// 새로운 파츠 정보

		if( pItemOption->byScouterInfo >= SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION )
		{
			// 스카우터 파츠의 설명을 테이블 그대로 보여질 것은 값을 0으로 일관적으로 통일한다
			// 어차피 보여질 단계에서 first로 들어간 타입에 따라 해당 글귀를 읽어온다			
			m_mapPartsExplain[pItemOption->byScouterInfo] = 0;

			// 예외적으로 SCOUTER_INFO_TRANSMITTER 기능이 있다면
			// 하위 기능인 SCOUTER_INFO_CONNECTOR 정보는 보여주지 않는다
			if( pItemOption->byScouterInfo == SCOUTER_INFO_TRANSMITTER)
			{
				m_mapPartsExplain.erase(SCOUTER_INFO_CONNECTOR);
			}
			else if( pItemOption->byScouterInfo == SCOUTER_INFO_CONNECTOR &&
					 m_mapPartsExplain.find(SCOUTER_INFO_TRANSMITTER) != m_mapPartsExplain.end() )
			{
				m_mapPartsExplain.erase(SCOUTER_INFO_CONNECTOR);
			}
		}
		else
		{			
			if( pItemOption->byScouterInfo < SCOUTER_INFO_BEGIN_ONLY_ALLPY_MAX )
			{
				// sITEM_OPTION_TBLDAT의 nValue[0]				
				if(pItemOption->bAppliedInPercent[0] == 0)
				{
					m_mapPartsExplain[pItemOption->byScouterInfo] = pItemOption->nValue[0];
				}
				else if(pItemOption->bAppliedInPercent[0] == 1)
				{	
					// 비율로 적혀있는 경우 기본 수치에 적용된 비율을 구한다			
					//RwUInt32 iMaxValue = GetDefaultValue(pItemOption->byScouterInfo);

					m_mapPartsExplain[pItemOption->byScouterInfo] = pItemOption->nValue[0];
				}	
			}
			else if( pItemOption->byScouterInfo < SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION )
			{
				// 가장 좋은 수치 하나만 적용하는 파츠
				// sITEM_TBLDAT의 byScouter_Parts_Value
				m_mapPartsExplain[pItemOption->byScouterInfo] = uiPartsValue;
			}			
		}				
	}
	else
	{
		// 같은 종류의 파츠가 이미 있을 경우

		if( pItemOption->byScouterInfo < SCOUTER_INFO_BEGIN_ONLY_ALLPY_MAX )
		{
			// 기존의 파츠 정보에 수치를 더한다
			if(pItemOption->bAppliedInPercent[0] == 0)
			{
				it->second += pItemOption->nValue[0];
			}
			else if(pItemOption->bAppliedInPercent[0] == 1)
			{	
				// 비율로 적혀있는 경우 기본 수치에 적용된 비율을 구한다			
				//RwUInt32 iMaxValue = GetDefaultValue(pItemOption->byScouterInfo);

				it->second += pItemOption->nValue[0];
			}	
		}			
		else if( pItemOption->byScouterInfo < SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION )
		{
			// 가장 좋은 아이템 하나만을 적용한다
			if( it->second < uiPartsValue )
				m_mapPartsExplain[pItemOption->byScouterInfo] = uiPartsValue;
		}

		// SPI_BEGIN_SPECIAL_FUNCTION보다 클 경우는 단순히 설명만을 보여주기에
		// 몇 개의 파츠를 등록하여도 내용은 변하지 않는다.
		// 단, 예외적으로 SCOUTER_INFO_TRANSMITTER 기능이 있다면
		// 하위 기능인 SCOUTER_INFO_CONNECTOR 정보는 보여주지 않는다		
	}
}

RwInt32 CScouterGui::GetDefaultValue(RwUInt32 uiType)
{
	// 해당 타입별 아바타의 기본 수치를 얻어온다	
	sPC_TBLDAT* pPC_TBLDAT = reinterpret_cast<sPC_TBLDAT*>(API_GetTableContainer()->GetPcTable()->FindData(uiType));

	switch(uiType)
	{
	case SCOUTER_INFO_LP:						return pPC_TBLDAT->wBasic_LP;
	case SCOUTER_INFO_EP:						return pPC_TBLDAT->wBasic_EP;
	case SCOUTER_INFO_STR:						return pPC_TBLDAT->byStr;
	case SCOUTER_INFO_CON:						return pPC_TBLDAT->byCon;
	case SCOUTER_INFO_FOC:						return pPC_TBLDAT->byFoc;
	case SCOUTER_INFO_DEX:						return pPC_TBLDAT->byDex;
	case SCOUTER_INFO_SOL:						return pPC_TBLDAT->bySol;
	case SCOUTER_INFO_ENG:						return pPC_TBLDAT->byEng;
	case SCOUTER_INFO_ATTACK_SPEED:				return pPC_TBLDAT->wAttack_Speed_Rate;
	case SCOUTER_INFO_ATTACK_RATE:				return pPC_TBLDAT->wAttack_Rate;
	case SCOUTER_INFO_DODGE_RATE:				return pPC_TBLDAT->wDodge_Rate;
	case SCOUTER_INFO_BLOCK_RATE:				return pPC_TBLDAT->wBlock_Rate;
	case SCOUTER_INFO_CURSE_SUCCESS_RATE:		return pPC_TBLDAT->wCurse_Success_Rate;
	case SCOUTER_INFO_CURSE_TOLERANCE_RATE:		return pPC_TBLDAT->wCurse_Tolerance_Rate;
	default:
		{
			DBO_FAIL("Invalid scouter parts type" << uiType);
			break;
		}
	}

	return 0;
}

VOID CScouterGui::DisplayPartsInfo()
{	
	MAP_PARTS_EXPLAIN_ITER it = m_mapPartsExplain.begin();

	for( ; it != m_mapPartsExplain.end() ; ++it )
	{
		if( it->first < SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION )
		{
			// 스카우터 파츠의 수치
            WCHAR awcBuffer[50] = {0,};
			RwUInt8 byPartsInfo = (RwUInt8)it->first;
			RwUInt32 iType = ConvertPartsInfotoStringTableIndex(byPartsInfo);

			if( SCOUTER_INFO_UPGRADE_RATE_UP		== byPartsInfo ||
				SCOUTER_INFO_ITEM_BREAK_RATE_DOWN	== byPartsInfo )
			{
				// avooo's commnet : 기획쪽 추가에 따라.. 테이블에 있는 수치는 백분율단위이다
				// 예 ) 23 -> 1.23배 로 표시하여야 한다
				swprintf_s(awcBuffer, 50, GetDisplayStringManager()->GetString(iType), (RwReal)it->second/100.f + 1.f);
			}
			else
				swprintf_s(awcBuffer, 50, GetDisplayStringManager()->GetString(iType), it->second);

			// 설명을 입력한다.
			m_pPartsListBox->AddItem(awcBuffer);
		}
		else
		{
			// 스카우터 파츠의 설명을 입력한다.
			RwUInt32 iType = ConvertPartsInfotoStringTableIndex(it->first);
			m_pPartsListBox->AddItem(GetDisplayStringManager()->GetString(iType));
		}		
	}	
}

RwUInt32 CScouterGui::ConvertPartsInfotoStringTableIndex(RwUInt32 iType)
{
	if( iType < SCOUTER_INFO_BEGIN_ONLY_ALLPY_MAX )
	{
		return DST_SCOUTERPARTS_NORMAL_START_LP + (iType - SCOUTER_INFO_BEGIN_DEFAULT_VALUE);
	}
	else if( iType < SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION )
	{
		return DST_SCOUTERPARTS_BEST_START + (iType - SCOUTER_INFO_BEGIN_ONLY_ALLPY_MAX);
	}
	
	return DST_SCOUTERPARTS_SPECIAL_START + (iType - SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION);
}

VOID CScouterGui::OnCaptureWheelMove(RwInt32 nFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if( m_pThis->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != m_pThis )
		return;

	if( !m_pPartsListBox->HasScrollBar() )
		return;

	RwInt32 iCurIndex = m_pPartsListBox->GetTopVisibleItem();
	RwInt32 iMaxIndex = m_pPartsListBox->GetMaxVisibleItems();
	RwInt32 iDelta = iCurIndex - sDelta / GUI_MOUSE_WHEEL_DELTA;

	if( iDelta < 0 )
		iDelta = 0;
	else if( iDelta > iMaxIndex )
		iDelta = iMaxIndex;

	m_pPartsListBox->SetTopItem(iDelta);	
}

VOID CScouterGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE_TOP_LINKED(DIALOG_SCOUTER_BODY, key.m_fX, key.m_fY);
}

VOID CScouterGui::OnClicked_ExitButton(gui::CComponent* pComponent)
{	
	GetDialogManager()->CloseDialog(DIALOG_SCOUTER_BODY);
}

RwInt32 CScouterGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CScouterGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION( "CScouterGui::HandleEvents" );

	if( msg.Id == g_EventScouter )
	{
		SDboEventScouter* pPacket = (SDboEventScouter*)msg.pData;
		switch(pPacket->iType)
		{
		case SCOUTER_EVENT_SET_SCOUTER:
			{					
				SetScouterInfo(pPacket->uiUserData);				
				break;
			}
		case SCOUTER_EVENT_UNSET_SCOUTER:
			{
				UnSetScouterInfo();
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


		UpdatePartsInfo();
	}

	NTL_RETURNVOID();
}