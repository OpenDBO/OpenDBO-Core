#include "precomp_dboclient.h"
#include "ContractGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlStorageManager.h"

// dbo
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "LoginStageState.h"
#include "DialogPriority.h"


#define dPOSITION_Y_ADJUST		(56)


CContractGui::CContractGui(const RwChar *pName)
:CNtlPLGui(pName)
{
}

CContractGui::~CContractGui()
{
}

RwBool CContractGui::Create()
{
	NTL_FUNCTION("CContractGui::Create");

	if(!CNtlPLGui::Create("gui\\Contract.rsr", "gui\\Contract.srf", "gui\\Contract.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_QUICKSLOT );

	CRectangle rect;

	// 약관 설명
	m_pContractHtml = (gui::CHtmlBox*)GetComponent("hbxContract");
	m_pContractHtml->SetLineSpace(8);

	// 계약 내용을 읽어 달라는 내용
	m_pPleaseRead = (gui::CStaticBox*)GetComponent("stbPleaseRead");
	m_pPleaseRead->SetText( GetDisplayStringManager()->GetString(DST_CONTRACT_PLEASE_READ) );

	// 동의 버튼
	m_pAgreementButton = (gui::CButton*)GetComponent("btnAgreement");
	m_pAgreementButton->SetText( GetDisplayStringManager()->GetString(DST_CONTRACT_ACCEPT) );
	m_slotOKButton = m_pAgreementButton->SigClicked().Connect( this, &CContractGui::OnClicked_AgreementButton);

	// OK 버튼
	m_pOKButton = (gui::CButton*)GetComponent("btnOK");
	m_pOKButton->SetText( GetDisplayStringManager()->GetString(DST_LOBBY_OK) );
	m_slotAgreementButton = m_pOKButton->SigClicked().Connect( this, &CContractGui::OnClicked_OKButton);

	// Cancel 버튼
	m_pCancelButton = (gui::CButton*)GetComponent("btnCancel");
	m_pCancelButton->SetText( GetDisplayStringManager()->GetString(DST_CONTRACT_NOT_ACCEPT) );
	m_slotCancelButton = m_pCancelButton->SigClicked().Connect( this, &CContractGui::OnClicked_CancelButton);

	// 검은 배경
	m_srfBackground.SetColor(0, 0, 0, 100);

	// default value
	m_pAgreementButton	->Show(false);
	m_pOKButton			->Show(false);
	m_pCancelButton		->Show(false);

	LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	// sig	
	m_slotServerScrollChanged		= m_pContractHtml->GetScrollBar()->SigValueChanged().Connect( this, &CContractGui::OnScrollChanged );
	m_slotServerScrollSliderMoved	= m_pContractHtml->GetScrollBar()->SigSliderMoved().Connect( this, &CContractGui::OnScrollChanged );
	m_slotCaptureWheelMove			= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CContractGui::OnCaptureWheelMove );
	m_slotPaint						= m_pThis->SigPaint().Connect( this, &CContractGui::OnPaint );

	LinkMsg(g_EventLoginMessage);
	LinkMsg(g_EventLogInStageStateExit);
	LinkMsg(g_EventResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CContractGui::Destroy()
{
	NTL_FUNCTION("CContractGui::Destroy");

	Popup(false);

	UnLinkMsg(g_EventLoginMessage);
	UnLinkMsg(g_EventLogInStageStateExit);
	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CContractGui::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
	CRectangle rtRect;

	rtRect.left		= 0;
	rtRect.top		= 0;
	rtRect.right	= GetDboGlobal()->GetScreenWidth();
	rtRect.bottom	= GetDboGlobal()->GetScreenHeight();

	m_srfBackground.SetRect(rtRect);


	SetPosition((rtRect.GetWidth()  - m_pThis->GetWidth())/2,
		(rtRect.GetHeight() - m_pThis->GetHeight())/2 + dPOSITION_Y_ADJUST);
}

VOID CContractGui::OnClicked_AgreementButton(gui::CComponent* pComponent)
{
	GetNtlStorageManager()->SetData( dSTORAGE_ACCOUNT_CONTRACT, (RwInt32)GetDboGlobal()->GetContractVerstion() );
	GetDboGlobal()->SetGameContract(true);
	Logic_SaveAccountOption();

	Show(false);
	Popup(false);
}

VOID CContractGui::OnClicked_OKButton(gui::CComponent* pComponent)
{
	Show(false);
	Popup(false);
}

VOID CContractGui::OnClicked_CancelButton(gui::CComponent* pComponent)
{
	SUserData* pUserData = GetDboGlobal()->GetUserData();
	CLoginPacketGenerator* pLoginPacketGenerator = GetDboGlobal()->GetLoginPacketGenerator(); 

	pLoginPacketGenerator->SendLogOutReq(pUserData->wchUserID, false);

	PostQuitMessage(0);
}

VOID CContractGui::OnScrollChanged(RwInt32 iOffset)
{	
	RwInt32 iValue		= m_pContractHtml->GetScrollBar()->GetValue();
	RwInt32 iMaxValue	= m_pContractHtml->GetScrollBar()->GetMaxValue();

	if( iValue == iMaxValue )
		m_pAgreementButton->ClickEnable(true);
}

VOID CContractGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if( m_pThis->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != m_pThis )
		return;

	if( m_pThis->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;


	RwInt32 iValue		= m_pContractHtml->GetScrollBar()->GetValue();
	RwInt32 iMaxValue	= m_pContractHtml->GetScrollBar()->GetMaxValue();
	RwInt32 iDelta		= iValue - (sDelta*18) / GUI_MOUSE_WHEEL_DELTA;

	if( iDelta < 0 )
		iDelta = 0;
	else if( iDelta > iMaxValue )
		iDelta = iMaxValue;

	m_pContractHtml->GetScrollBar()->SetValue(iDelta);
}

VOID CContractGui::OnPaint()
{
	m_srfBackground.Render();
}

VOID CContractGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CContractGui::HandleEvents");

	if(msg.Id == g_EventLoginMessage)
	{
		SDboEventLoginMessage* pEvent = reinterpret_cast<SDboEventLoginMessage*>( msg.pData );

		if( LOGIN_EVENT_FINISH_LOADING_AND_MOVIE == pEvent->byMessage )
		{
			WCHAR* pwcContract = GetDboGlobal()->GetContract();
			m_pContractHtml->SetHtmlFromMemory(pwcContract, wcslen(pwcContract));
		}
		else if( LOGIN_EVENT_SHOW_CONTRACT_PAGE == pEvent->byMessage )
		{
			if( GetDboGlobal()->IsAcceptGameContract() )
			{
				m_pOKButton->Show(true);

				m_pAgreementButton	->Show(false);
				m_pCancelButton		->Show(false);
				m_pPleaseRead		->Show(false);
			}
			else
			{
				m_pOKButton->Show(false);

				// 약관을 다 읽어야 m_pAgreementButton 버튼이 활성화 된다
				m_pAgreementButton	->ClickEnable(false);
				m_pAgreementButton	->Show(true);
				m_pCancelButton		->Show(true);
				m_pPleaseRead		->Show(true);
			}

			Show(true);
			Popup(true);
		}
	}
	else if(msg.Id == g_EventLogInStageStateExit)
	{
		SDboEventLogInStageStateExit* pStageStateExit = reinterpret_cast<SDboEventLogInStageStateExit*>(msg.pData);

		if( LOGIN_STATE_SERVER_CONNECT == (ELogInStageState)pStageStateExit->byState )
		{
			// 약관을 처음 동의하는 경우이다
			if( GetDboGlobal()->GetContractVerstion() != (RwUInt32)GetNtlStorageManager()->GetIntData( dSTORAGE_ACCOUNT_CONTRACT ) )
			{
				Show(false);
				Popup(false);
			}
		}
	}	
	else if(msg.Id == g_EventResize)
	{
		SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );		
		LocateComponent(pPacket->iWidht, pPacket->iHeight);	
	}

	NTL_RETURNVOID();
}