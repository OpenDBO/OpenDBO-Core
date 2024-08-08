#include "precomp_dboclient.h"

// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"
#include "InputActionMap.h"
#include "NtlSobAvatar.h"
#include "NtlSobPlayer.h"
#include "NtlSobPlayerAttr.h"

// table
#include "TableContainer.h"
#include "TextAllTable.h"
#include "ItemTable.h"

// dbo
#include "DboGlobal.h"
#include "DboDef.h"
#include "DboEvent.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"
#include "DboPacketGenerator.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "SayFilter.h"

// privateShop Trade Gui
#include "PrivateShopGui.h"
#include "PrivateShopTradeGui.h"

#define dPRIVATESHOP_COLOR_TALK_I		RGB(255, 255, 255)
#define dPRIVATESHOP_COLOR_TALK_YOU		RGB(255, 255, 128)

//////////////////////////////////////////////////////////////////////////
// CPrivateShopTradeGui
//////////////////////////////////////////////////////////////////////////

/**
* \brief 생성자
*/
CPrivateShopTradeGui::CPrivateShopTradeGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pStbTitle(NULL)
,m_pBtnClose(NULL)
,m_pStbTradeItemTitle(NULL)
,m_pDlgTradeItem(NULL)
,m_pStbTradeItemName(NULL)
,m_pStbTradeItemPriceTitle(NULL)
,m_pStbTradeItemPrice(NULL)
,m_pStbChattingTitle(NULL)
,m_pOpbChatting(NULL)
,m_pIpbChatting(NULL)
,m_pStbIME(NULL)
,m_pBtnAction(NULL)
,m_uiShopOwner(0)
,m_uiTradeType(0)
,m_uiBusinessPrice(0)
,m_pShopGui(NULL)
,m_uiFluctuationCount(0)
{
	memset( m_awcTargetName, 0, sizeof(WCHAR) * NTL_MAX_SIZE_CHAR_NAME_UNICODE );
}

/**
* \brief 소멸자
*/
CPrivateShopTradeGui::~CPrivateShopTradeGui()
{
}

/**
* \brief Create
*/
RwBool CPrivateShopTradeGui::Create()
{
	NTL_FUNCTION("CPrivateShopTradeGui::Create");

	if (!CNtlPLGui::Create("", "gui\\PrivateShopTrade.srf", "gui\\PrivateShopTrade.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Components
	m_pThis						= (gui::CDialog*)GetComponent("dlgMain");
	m_pStbTitle					= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pBtnClose					= (gui::CButton*)GetComponent("btnClose");
	m_pStbTradeItemTitle		= (gui::CStaticBox*)GetComponent("stbTradeItemTitle");
	m_pDlgTradeItem				= (gui::CDialog*)GetComponent("dlgTradeItem");
	m_pStbTradeItemName			= (gui::CStaticBox*)GetComponent("stbTradeItemName");
	m_pStbTradeItemPriceTitle	= (gui::CStaticBox*)GetComponent("stbTradeItemPriceTitle");
	m_pStbTradeItemPrice		= (gui::CStaticBox*)GetComponent("stbTradeItemPrice");
	m_pStbChattingTitle			= (gui::CStaticBox*)GetComponent("stbChattingTitle");
	m_pOpbChatting				= (gui::COutputBox*)GetComponent("opbChatting");
	m_pIpbChatting				= (gui::CInputBox*)GetComponent("ipbChatting");
	m_pStbIME					= (gui::CStaticBox*)GetComponent("stbIME");
	m_pBtnAction				= (gui::CButton*)GetComponent("btnAction");

	// Priority
	m_pThis->SetPriority( dDIALOGPRIORITY_PRIVATE_BUSINESS );

	// 흥정 중의 아이템 생성
	m_sgTradeItem.Create(m_pDlgTradeItem, DIALOG_PRIVATESHOP_TRADE, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);

	// Signals
	m_slotIMEChanged		= m_pIpbChatting->SigIMEChanged().Connect(this, &CPrivateShopTradeGui::OnIMEChanged);
	m_slotMoveDlgTradeItem	= m_pDlgTradeItem->SigMove().Connect(this, &CPrivateShopTradeGui::OnMoveDlgTradeItem);
	m_slotPaintDlgTradeItem	= m_pDlgTradeItem->SigPaint().Connect(this, &CPrivateShopTradeGui::OnPaintDlgTradeItem);
	m_slotGotFocus			= m_pIpbChatting->SigGotFocus().Connect(this, &CPrivateShopTradeGui::OnGotFocus);
	m_slotLostFocus			= m_pIpbChatting->SigLostFocus().Connect(this, &CPrivateShopTradeGui::OnLostFocus);
	m_slotClickedBtnAction	= m_pBtnAction->SigClicked().Connect(this, &CPrivateShopTradeGui::OnClickedBtnAction);
	m_slotClickedBtnClose	= m_pBtnClose->SigClicked().Connect(this, &CPrivateShopTradeGui::OnClickedBtnClose );
	m_slotChatReturn		= m_pIpbChatting->SigReturnPressed().Connect(this, &CPrivateShopTradeGui::OnChatReturn);

	m_pStbTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	m_pStbTitle->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADE_TITLE ) );
	m_pStbTradeItemTitle->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADE_ITEMTITLE ) );
	m_pStbTradeItemPriceTitle->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADE_ITEMPRICE ) );
	m_pStbChattingTitle->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADE_CHAT ) );

	// 인풋 박스를 NativeMode로 초기화해주고,
	m_pIpbChatting->SetNativeMode();
	OnIMEChanged(m_pIpbChatting->GetCurrentIMEState());

	// 유저 인터페이스를 초기화한다.
	ClearUserInterface();
	
	Show(false);

	// Link
	LinkMsg( g_EventChatMessagePrivateShopBusiness, 0 );
	LinkMsg( g_EventPrivateShopBusinessPricefluctuationsRes, 0 );
	LinkMsg( g_EventPrivateShopBusinessPricefluctuationsNfy, 0 );
	LinkMsg( g_EventPrivateShopBusinessStart, 0 );
	LinkMsg( g_EventPrivateShopBusinessCloseWindow, 0 );
	LinkMsg( g_EventMsgBoxResult, 0 );
		
	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
void CPrivateShopTradeGui::Destroy()
{
	NTL_FUNCTION("CPrivateShopTradeGui::Destroy");

	// UnLink
	UnLinkMsg( g_EventChatMessagePrivateShopBusiness );
	UnLinkMsg( g_EventPrivateShopBusinessPricefluctuationsRes );
	UnLinkMsg( g_EventPrivateShopBusinessPricefluctuationsNfy );
	UnLinkMsg( g_EventPrivateShopBusinessStart );
	UnLinkMsg( g_EventPrivateShopBusinessCloseWindow );
	UnLinkMsg( g_EventMsgBoxResult );

	m_sgTradeItem.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
* \brief HandleEvents
*/
void CPrivateShopTradeGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CPrivateShopTradeGui::HandleEvents");

	// 개인상점 흥정 시작의 이벤트	
	if( msg.Id == g_EventPrivateShopBusinessStart )
	{
		SDboEventPrivateShopBusinessStart* pData =
			reinterpret_cast<SDboEventPrivateShopBusinessStart*>(msg.pData);

		// Interface Setting 
		SetInterface( pData->uiType, pData->uiShopOwner, pData->awcTargetName, pData->pPrivateShopItemData );
		m_uiBusinessPrice = pData->pPrivateShopItemData->dwZenny;	// 가격
		m_pShopGui = reinterpret_cast<CPrivateShopGui*>(pData->pGui);

		// Open Dialog
		if( FALSE == GetDialogManager()->IsOpenDialog( DIALOG_PRIVATESHOP_TRADE ) )
		{
			GetDialogManager()->OpenDialog( DIALOG_PRIVATESHOP_TRADE );

			// Chatting 창의 Focus
			m_pIpbChatting->SetFocus();
		}
	}
	// Todo( 커뮤니티 서버에서 TU_CHAT_MESSAGE_PRIVATESHOP_BUSINESS )
	// 1. 그사람의 이름과 : 메시지를 뿌려줘야 한다.
	else if( msg.Id == g_EventChatMessagePrivateShopBusiness )
	{
		SNtlEventChatMessagePrivateShopBusiness* pChat =
			reinterpret_cast<SNtlEventChatMessagePrivateShopBusiness*>(msg.pData);

		// Out창에 채팅 출력
		WCHAR awcBuffer[512];
		swprintf_s( awcBuffer, 512, L"[%s] : %s", pChat->pwchSenderCharName, pChat->awcMessage );

		// 이름이 같을 경우
		if( !wcscmp( Logic_GetAvatarName(), pChat->pwchSenderCharName ) )
		{
			AddChattingString( ePRIVATESHOP_TRADE_TALK_TYPE_I, awcBuffer );
		}
		// 다를 경우
		else
		{
			AddChattingString( ePRIVATESHOP_TRADE_TALK_TYPE_YOU, awcBuffer );
		}
	}
	// Todo(주인이 UG_PRIVATESHOP_BUSINESS_PRICEFLUCTUATIONS_RES )
	// 1. 가격이 적용된다.
	// 2. 가격이 적용되었다는 대화상자를 띄운다.
	// 3. 주인이 OK 를 하면 대화상자는 off되고 흥정 창에 가격이 적용된다.
	else if( msg.Id == g_EventPrivateShopBusinessPricefluctuationsRes )
	{
		SNtlEventPrivateShopBusinessPricefluctuationsRes* pResult =
			reinterpret_cast<SNtlEventPrivateShopBusinessPricefluctuationsRes*>(msg.pData);

		m_pStbTradeItemPrice->SetText( pResult->dwAfterZenny );
		m_uiBusinessPrice = pResult->dwAfterZenny;	// 가격

		//CDboEventGenerator::MsgBoxShow( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADE_APPLYPRICE ), 
		//	MBW_NONE, MBTF_OK, TRUE );
		GetAlarmManager()->AlarmMessage( DST_PRIVATESHOP_BUSINESS_TRADE_APPLYPRICE );
	}
	// Todo(흥정 중인 손님이 UG_PRIVATESHOP_BUSINESS_PRICEFLUCTUATIONS_NFY)
	// 1. 가격이 적용된다.
	// 2. 가격이 적용되었다는 대화상자를 띄운다.
	// 3. 손님이 OK 를 하면 대화상자는 off되고 흥정 창에 가격이 적용된다.
	else if( msg.Id == g_EventPrivateShopBusinessPricefluctuationsNfy )
	{
		SNtlEventPrivateShopBusinessPricefluctuationsNfy* pResult =
			reinterpret_cast<SNtlEventPrivateShopBusinessPricefluctuationsNfy*>(msg.pData);

		m_pStbTradeItemPrice->SetText( pResult->dwAfterZenny );
		m_uiBusinessPrice = pResult->dwAfterZenny;

		// 손님이라면 카운트를 증가시켜준다.
		m_uiFluctuationCount++;

		//CDboEventGenerator::MsgBoxShow( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADE_APPLYNOTIFY ),
		//	MBW_NONE, MBTF_OK, TRUE );
		GetAlarmManager()->AlarmMessage( DST_PRIVATESHOP_BUSINESS_TRADE_APPLYNOTIFY );
	}
	else if( msg.Id == g_EventMsgBoxResult )
	{
		SDboEventMsgBoxResult* pMsgBoxResult = reinterpret_cast<SDboEventMsgBoxResult*>(msg.pData);

		// 메시지 박스의 결과
		switch (pMsgBoxResult->uiWorkId)
		{
		case MBW_PRIVATESHOP_BUSINESS_IMMEDIATE:	// 즉시 구매를 수락한 경우
			{
				// 정말 구입하시겠습니까? -> OK
				if( pMsgBoxResult->eResult == MBR_OK )
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopBusinessImmediateItemBuyingReq( m_uiShopOwner, m_uiFluctuationCount );
				}
			}
			break;
		}
	}
	else if( msg.Id == g_EventPrivateShopBusinessCloseWindow )
	{
		//SDboEventPrivateShopBusinessCloseWindowNfy* pNotify = reinterpret_cast<SDboEventPrivateShopBusinessCloseWindowNfy*>( msg.pData );

		m_bCloseNotify = TRUE;

		if( GetDialogManager()->IsOpenDialog( DIALOG_PRIVATESHOP_TRADE ) )
			GetDialogManager()->CloseDialog( DIALOG_PRIVATESHOP_TRADE );
	}

	NTL_RETURNVOID();
}

RwInt32 CPrivateShopTradeGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if (!bOpen)
	{
		// 만약 흥정 중인데 ESC나 기타 다른 여부에 의하여 흥정 창이 닫긴다면,
		if( m_pShopGui )
		{
			if( m_pShopGui->GetShopBusiness() )
			{
				RwUInt32 uiAvatarSerial = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();

				if( m_bCloseNotify )
				{
					m_pShopGui = NULL;
					return TRUE;
				}
				
				// 자신이 손님이라거나 상점 주인일 경우
				if( m_pShopGui->GetShopBusinessTarget() == uiAvatarSerial
					|| m_uiShopOwner == uiAvatarSerial )
				{
					// 취소 요청 패킷을 날린다.
					GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopBusinessCancelReq( m_uiShopOwner );
					m_pShopGui = NULL;

					//GetDialogManager()->CloseDialog( DIALOG_PRIVATESHOP_TRADE );
					return TRUE;
				}
			}
		}

		ClearUserInterface();
	}
	else
	{
		m_bCloseNotify = FALSE;
		m_pOpbChatting->Clear();
		m_pIpbChatting->Clear();
	}

	return TRUE;
}

/**
* \brief m_pIpbChatting의 IME 모드가 변경되었을 때의 Callback
* \param eIMEState		IME의 상태
*/
void CPrivateShopTradeGui::OnIMEChanged(RwInt32 eIMEState)
{
	if (eIMEState == 0)
	{
		m_pStbIME->SetText( GetDisplayStringManager()->GetString( DST_CHAT_IME_ENG ) );
	}
	else
	{
		m_pStbIME->SetText( GetDisplayStringManager()->GetString( DST_CHAT_IME_KOR ) );
	}
}

void CPrivateShopTradeGui::OnMoveDlgTradeItem(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pDlgTradeItem->GetScreenRect();
	m_sgTradeItem.SetParentPosition(rect.left, rect.top);
}

void CPrivateShopTradeGui::OnPaintDlgTradeItem()
{
	m_sgTradeItem.Paint();
}

/**
* \brief Ime창이 Focus를 얻었을 경우
*/
VOID CPrivateShopTradeGui::OnGotFocus() 
{
	GetDboGlobal()->GetGameData()->bChatMode = TRUE;
}

/**
* \brief Ime창이 Focus를 잃었을 경우
*/
VOID CPrivateShopTradeGui::OnLostFocus() 
{
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;
}

/**
* \brief Action 버튼을 클릭하였을 경우 ( 주인 : 가격조정, 손님 : 즉시구매 )
*/
void CPrivateShopTradeGui::OnClickedBtnAction( gui::CComponent* pComponent ) 
{
	// 주인이 가격 조정을 클릭하였을 경우
	if( m_uiTradeType == CPrivateShopGui::PRIVATESHOP_SELL )
	{
		CDboEventGenerator::PrivateShopBusinessFluctuations( CPrivateShopGui::PRIVATESHOP_SELL,
			const_cast<sPRIVATESHOP_ITEM_DATA*>(m_pPrivateShopItemData) , m_uiBusinessPrice);
	}
	// 손님이 즉시 구매를 클릭하였을 경우
	else if( m_uiTradeType == CPrivateShopGui::PRIVATESHOP_BUY )
	{
		if( GetDialogManager()->IsOpenDialog( DIALOG_PRIVATESHOP_TRADE ) )
		{
			//CDboEventGenerator::MsgBoxShow( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADE_REALLYBUY ),
			//	MBW_PRIVATESHOP_BUSINESS_IMMEDIATE,
			//	MBTF_OK | MBTF_CANCEL, TRUE );
			GetAlarmManager()->AlarmMessage( DST_PRIVATESHOP_BUSINESS_TRADE_REALLYBUY );
		}
	}
}

void CPrivateShopTradeGui::OnClickedBtnClose( gui::CComponent* pComponent ) 
{
	// 취소 요청 패킷
	GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopBusinessCancelReq( m_uiShopOwner );
}

/**
* \brief 채팅 입력
// Todo( 흥정 창에서의 채팅을 입력했을 경우 )
// 1. 대화요청자가 대화상대자의 이름으로 메시지를 보내준다.
//  -1. 대화요청자의 GUI에 자신의 채팅을 표시한다.
//  -2. UT_CHAT_MESSAGE_PRIVATESHOP_BUSINESS(메시지를 보낸다.)
*/
void CPrivateShopTradeGui::OnChatReturn() 
{
	if( m_pIpbChatting->GetLength() )
	{
		// 채팅 내용
		const WCHAR *pText = m_pIpbChatting->GetText();
		CChatPacketGenerator *pChatPacketGenerator = CDboGlobal::GetInstance()->GetChatPacketGenerator();

		// 욕설 필터링
		std::wstring strFilterText = GetChattingFilter()->Filtering( pText );


		// 자신의 채팅 또한 패킷을 받아서 처리하는 것으로 한다.
		
		//// OutPut창에 채팅 출력
		//WCHAR awcBuffer[512];
		//swprintf_s( awcBuffer, 512, L"[%s] : %s", Logic_GetName( pSobAvatar ), strFilterText.c_str() );
		//AddChattingString( ePRIVATESHOP_TRADE_TALK_TYPE_I, awcBuffer );

		// 커뮤니티 서버에 채팅 내역을 보내줌
		pChatPacketGenerator->SendChatMessagePrivateShopBusiness( m_awcTargetName, (RwUInt16)strFilterText.size(), strFilterText.c_str() );
		
		// Input Box의 내용을 지워준다.
		m_pIpbChatting->Clear();
	}
}

/**
* \brief 손님이나 주인에 알맞은 인터페이스 형성
* 손님이라면 즉시 구매 버튼을 세팅
* 주인이라면 가격 조정 버튼을 세팅
*/
void CPrivateShopTradeGui::SetInterface( RwUInt8 uiType, RwUInt32 uiShopOwner, WCHAR* pwcTargetName, const sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData ) 
{
	// 손님
	if( uiType == CPrivateShopGui::PRIVATESHOP_BUY )
	{
		m_pBtnAction->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADE_IMMEDIATE ) );
	}
	// 주인
	else if( uiType == CPrivateShopGui::PRIVATESHOP_SELL )
	{
		m_pBtnAction->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADE_FLUCTUATIONS ) );
	}
	else
		return;

	m_uiFluctuationCount = 0;
	m_uiShopOwner = uiShopOwner;
	m_uiTradeType = uiType;
	swprintf_s( m_awcTargetName, NTL_MAX_SIZE_CHAR_NAME_UNICODE+1, L"%s", pwcTargetName );
	

	// 인터페이스에 들어갈 아이템을 셋팅
	SetTradeItem( pPrivateShopItemData );
}

/**
* \brief 인터페이스를 초기화
* 아이템 가격이나 이름, Output창을 클리어한다.
*/
void CPrivateShopTradeGui::ClearUserInterface()
{
	m_sgTradeItem.SetIcon(INVALID_TBLIDX);
	m_pStbTradeItemPrice->Clear();
	m_pStbTradeItemName->Clear();
	m_pOpbChatting->Clear();
	m_pIpbChatting->Clear();
	m_pShopGui = NULL;
	m_uiFluctuationCount = 0;

	// 인터페이스 및 상태 회복
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;
}

/**
* \brief 흥정 중인 아이템을 세팅
* \param pPrivateShopItemData		개인상점 아이템 데이타
*/
void CPrivateShopTradeGui::SetTradeItem(const sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData)
{
	DBO_ASSERT(pPrivateShopItemData != NULL && pPrivateShopItemData->byPrivateShopInventorySlotPos != PRIVATESHOP_ITEMPOS_INVALID,"CPrivateShopTradeBoxGui::SetTradeItem Invalid Param");
	m_pPrivateShopItemData = pPrivateShopItemData;

	sITEM_TBLDAT* pItemTable = Logic_GetItemDataFromTable( m_pPrivateShopItemData->sItem.itemNo );
	if( pItemTable == NULL )
	{
		DBO_FAIL( "pItemTable is NULL" );
		return;
	}

	if ( m_pPrivateShopItemData->sItem.bNeedToIdentify )
	{
		m_sgTradeItem.Clear();
		m_sgTradeItem.SetSerialType( REGULAR_SLOT_ITEM_NOT_IDENTIFICATION );
		m_sgTradeItem.SetIcon( pPrivateShopItemData->sItem.itemNo, pPrivateShopItemData->sItem.byStackcount);
		m_pStbTradeItemName->SetText(GetDisplayStringManager()->GetString(DST_PRIVATESHOP_ITEM_NOT_IDENTIFICATION));
	}
	else
	{
		m_sgTradeItem.Clear();
		m_sgTradeItem.SetSerialType( REGULAR_SLOT_ITEM_TABLE );
		m_sgTradeItem.SetIcon( pPrivateShopItemData->sItem.itemNo, pPrivateShopItemData->sItem.byStackcount);
		m_pStbTradeItemName->SetText(m_sgTradeItem.GetItemTable()->wszNameText);
	}

	m_pStbTradeItemPrice->SetText((RwUInt32)pPrivateShopItemData->dwZenny); 
	memcpy(&m_sTradeItemData, &pPrivateShopItemData->sItem, sizeof(sITEM_DATA));
}

/**
* \brief 채팅 Output창의 글씨 출력
* \param pwcString		문자열
* \param uiColor		글씨 색깔
*/
void CPrivateShopTradeGui::AddChattingString(const WCHAR* pwcString, RwUInt32 uiColor)
{
	m_pOpbChatting->AddText(pwcString, uiColor);
}

/**
* \brief 채팅 Output창의 글씨 출력
* \param eTalkType		자신, 상대
* \param pwcString		문자열
*/
void CPrivateShopTradeGui::AddChattingString(ePRIVATESHOP_TRADE_TALK_TYPE eTalkType, const WCHAR* pwcString)
{
	// eTalkType에 따라서 알맞은 (define 된) 색깔을 지정해줘서 출력한다.
	switch (eTalkType)
	{
	case ePRIVATESHOP_TRADE_TALK_TYPE_I:
		AddChattingString(pwcString, dPRIVATESHOP_COLOR_TALK_I);
		break;
	case ePRIVATESHOP_TRADE_TALK_TYPE_YOU:
		AddChattingString(pwcString, dPRIVATESHOP_COLOR_TALK_YOU);
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// CPrivateShopTradeBoxGui
//////////////////////////////////////////////////////////////////////////

/**
* \brief 생성자
*/
CPrivateShopTradeBoxGui::CPrivateShopTradeBoxGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pStbTitle(NULL)
,m_pStbSubTitle(NULL)
,m_pBtnClose(NULL)
,m_pDlgTradeItem(NULL)
,m_pStbTradeItemPricePrevTitle(NULL)
,m_pStbTradeItemPricePrev(NULL)
,m_pStbTradeItemPriceNextTitle(NULL)
,m_pStbTradeItemPriceNext(NULL)
,m_pBtnSubmit(NULL)
,m_uiPrice(0)
{
} 

/**
* \brief 소멸자
*/
CPrivateShopTradeBoxGui::~CPrivateShopTradeBoxGui()
{
}

/**
* \brief Create
*/
RwBool CPrivateShopTradeBoxGui::Create()
{
	NTL_FUNCTION("CPrivateShopTradeBoxGui::Create");

	if (!CNtlPLGui::Create("", "gui\\PrivateShopTradeBox.srf", "gui\\PrivateShopTradeBox.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Components
	m_pThis							= (gui::CDialog*)GetComponent("dlgMain");
	m_pStbTitle						= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pStbSubTitle					= (gui::CStaticBox*)GetComponent("stbSubTitle");
	m_pBtnClose						= (gui::CButton*)GetComponent("btnClose");
	m_pDlgTradeItem					= (gui::CDialog*)GetComponent("dlgTradeItem");
	m_pStbTradeItemPricePrevTitle	= (gui::CStaticBox*)GetComponent("stbTradeItemPricePrevTitle");
	m_pStbTradeItemPricePrev		= (gui::CStaticBox*)GetComponent("stbTradeItemPricePrev");
	m_pStbTradeItemPriceNextTitle	= (gui::CStaticBox*)GetComponent("stbTradeItemPriceNextTitle");
	m_pStbTradeItemPriceNext		= (gui::CStaticBox*)GetComponent("stbTradeItemPriceNext");
	m_pBtnSubmit					= (gui::CButton*)GetComponent("btnSubmit");
	m_pBtnCancel					= (gui::CButton*)GetComponent("btnCancel");
	m_pBtnPrice						= (gui::CButton*)GetComponent("btnTradeItemPriceNext");

	// Priority
	m_pThis->SetPriority( dDIALOGPRIORITY_PRIVATE_BUSINESS_PRICE );

	m_sgTradeItem.Create(m_pDlgTradeItem, DIALOG_PRIVATESHOP_TRADEBOX, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);

	// Signals
	m_slotMoveDlgTradeItem	= m_pDlgTradeItem->SigMove().Connect(this, &CPrivateShopTradeBoxGui::OnMoveDlgTradeItem);
	m_slotPaintDlgTradeItem	= m_pDlgTradeItem->SigPaint().Connect(this, &CPrivateShopTradeBoxGui::OnPaintDlgTradeItem);
	m_slotClickedBtnSubmit = m_pBtnSubmit->SigClicked().Connect(this, &CPrivateShopTradeBoxGui::OnClickedBtnSubmit);
	m_slotClickedBtnCancel = m_pBtnCancel->SigClicked().Connect(this, &CPrivateShopTradeBoxGui::OnClickedBtnCancel);
	m_slotClickedBtnClose = m_pBtnCancel->SigClicked().Connect(this, &CPrivateShopTradeBoxGui::OnClickedBtnCancel);
	m_slotClickedBtnPrice = m_pBtnPrice->SigClicked().Connect(this, &CPrivateShopTradeBoxGui::OnClickedBtnPrice );

	m_pStbTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	m_pStbTitle->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADEBOX_TITLE ) );
	m_pStbSubTitle->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADEBOX_FLUCTUATIONS ) );
	m_pStbTradeItemPricePrevTitle->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADEBOX_PREVPRICE ) );
	m_pStbTradeItemPricePrev->SetText(L"0");
	m_pStbTradeItemPriceNextTitle->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADEBOX_NEXTPRICE ) );
	m_pStbTradeItemPriceNext->SetText(L"0");
	m_pBtnSubmit->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADEBOX_ACCEPT ) );
	m_pBtnCancel->SetText( GetDisplayStringManager()->GetString( DST_PRIVATESHOP_BUSINESS_TRADEBOX_CANCEL ) );

	LinkMsg( g_EventPrivateShopBusinessFluctuations, 0 );
	LinkMsg( g_EventCalcPopupResult, 0 );

	ClearUserInterface();
	Show(false);
	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
void CPrivateShopTradeBoxGui::Destroy()
{
	NTL_FUNCTION("CPrivateShopTradeBoxGui::Destroy");

	UnLinkMsg( g_EventCalcPopupResult );
	UnLinkMsg( g_EventPrivateShopBusinessFluctuations );

	m_sgTradeItem.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}


/**
* \brief HandleEvents
*/
void CPrivateShopTradeBoxGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CPrivateShopTradeBoxGui::HandleEvents");

	// 가격 조정 창을 띄운다.
	if( msg.Id == g_EventPrivateShopBusinessFluctuations )
	{
		SDboEventPrivateShopBusinessFluctuations* pFluctuations
			= reinterpret_cast<SDboEventPrivateShopBusinessFluctuations*>(msg.pData);

		SetTradeItem( pFluctuations->pPrivateShopItemData, pFluctuations->uiStartPrice );
		
		if( FALSE == GetDialogManager()->IsOpenDialog( DIALOG_PRIVATESHOP_TRADEBOX ) )
		{
			GetDialogManager()->OpenDialog( DIALOG_PRIVATESHOP_TRADEBOX );
			RaiseTop();
		}
	}
	// 계산기로 가격을 입력했을 경우
	else if (msg.Id == g_EventCalcPopupResult)
	{
		SDboEventCalcPopupResult* pPopupResult = reinterpret_cast<SDboEventCalcPopupResult*>(msg.pData);
		if (pPopupResult->nSrcPlace == PLACE_PRIVATESHOP_TRADEBOX)
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, L"%u", pPopupResult->uiValue );
			m_pStbTradeItemPriceNext->SetText( awcBuffer );
			m_uiPrice = pPopupResult->uiValue;
		}
	}


	NTL_RETURNVOID();
}

RwInt32 CPrivateShopTradeBoxGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if (!bOpen)
	{
		ClearUserInterface();
	}

	return TRUE;
}

void CPrivateShopTradeBoxGui::OnMoveDlgTradeItem(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pDlgTradeItem->GetScreenRect();
	m_sgTradeItem.SetParentPosition(rect.left, rect.top);
}

void CPrivateShopTradeBoxGui::OnPaintDlgTradeItem()
{
	m_sgTradeItem.Paint();
}

/**
* \brief 적용 버튼을 클릭
* 변동된 가격을 적용한다.
*/
void CPrivateShopTradeBoxGui::OnClickedBtnSubmit( gui::CComponent* pComponent ) 
{
	// 조정된 가격을 서버에 보낸다.
	GetDboGlobal()->GetGamePacketGenerator()->SendPrivateShopBusinessPricefluctuationsReq( m_uiPrice );
	GetDialogManager()->CloseDialog( DIALOG_PRIVATESHOP_TRADEBOX );
}

/**
* \brief 취소 버튼을 클릭
* 가격 조정 창을 닫는다.
*/
void CPrivateShopTradeBoxGui::OnClickedBtnCancel( gui::CComponent* pComponent ) 
{
	GetDialogManager()->CloseDialog( DIALOG_PRIVATESHOP_TRADEBOX );
}

/**
* \brief 주인이 가격을 변동하기 위해 가격 버튼을 클릭했을 때
*/
void CPrivateShopTradeBoxGui::OnClickedBtnPrice( gui::CComponent* pComponent ) 
{
	CRectangle rect = pComponent->GetScreenRect();
	CDboEventGenerator::CalcPopupShow(TRUE, m_pPrivateShopItemData->byPrivateShopInventorySlotPos, PLACE_PRIVATESHOP_TRADEBOX,
		rect.left - dPRIVATESHOP_UI_CALC_OFFSET, rect.top, dPRIVATESHOP_ITEM_PRICE_MAX);
}

/**
* \brief 가격 조정 중인 아이템을 셋팅
*/
void CPrivateShopTradeBoxGui::SetTradeItem(const sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData, RwUInt32 uiStartPrice)
{
	DBO_ASSERT(pPrivateShopItemData != NULL && pPrivateShopItemData->byPrivateShopInventorySlotPos != PRIVATESHOP_ITEMPOS_INVALID,"CPrivateShopTradeBoxGui::SetTradeItem Invalid Param");
	m_pPrivateShopItemData = pPrivateShopItemData;
	
	sITEM_TBLDAT* pItemTable = Logic_GetItemDataFromTable( m_pPrivateShopItemData->sItem.itemNo );
	if( pItemTable == NULL )
	{
		DBO_FAIL( " pItemTable is NULL " );
		return;
	}
		
	if ( m_pPrivateShopItemData->sItem.bNeedToIdentify )
	{
		m_sgTradeItem.Clear();
		m_sgTradeItem.SetSerialType( REGULAR_SLOT_ITEM_NOT_IDENTIFICATION );
		m_sgTradeItem.SetIcon( pPrivateShopItemData->sItem.itemNo, pPrivateShopItemData->sItem.byStackcount);
	}
	else
	{
		m_sgTradeItem.Clear();
		m_sgTradeItem.SetSerialType( REGULAR_SLOT_ITEM_TABLE );
		m_sgTradeItem.SetIcon( pPrivateShopItemData->sItem.itemNo, pPrivateShopItemData->sItem.byStackcount);
	}
	
	m_pStbTradeItemPricePrev->SetText((RwUInt32)uiStartPrice);
	m_pStbTradeItemPriceNext->SetText( uiStartPrice );
	m_uiPrice = uiStartPrice;

	memcpy(&m_sTradeItemData, &pPrivateShopItemData->sItem, sizeof(sITEM_DATA));
}

void CPrivateShopTradeBoxGui::ClearUserInterface()
{
	m_sgTradeItem.Clear();
	m_pStbTradeItemPricePrev->Clear();
	m_pStbTradeItemPriceNext->Clear();
	m_uiPrice = 0;
}