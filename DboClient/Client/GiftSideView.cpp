#include "precomp_dboclient.h"
#include "GiftSideView.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// sl
#include "NtlSLLogic.h"

// dbo
#include "DisplayStringManager.h"
#include "DialogDefine.h"
#include "DboEvent.h"
#include "DboGlobal.h"

CGiftSideViewGui::CGiftSideViewGui(const RwChar* pName)
: CSideViewBase(pName)
, m_pstbViewName(NULL)
{
}

CGiftSideViewGui::~CGiftSideViewGui(void)
{
}

RwBool CGiftSideViewGui::Create()
{
	if(!CNtlPLGui::Create("", "gui\\GiftShopGui.srf", "gui\\GiftShop_SideView.frm"))
		return FALSE;

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pstbViewName = (gui::CStaticBox*)GetComponent("stbViewName");

	// 배경 (NetPy Side View의 공용 배경 재사용)
	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("NetPySideView.srf", "srfDialogBackUp"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("NetPySideView.srf", "srfDialogBackCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("NetPySideView.srf", "srfDialogBackDown"));

	m_slotPaint	= m_pThis->SigPaint().Connect( this, &CGiftSideViewGui::OnPaint );
	m_slotMove	= m_pThis->SigMove().Connect( this, &CGiftSideViewGui::OnMove );

	LinkMsg(g_EventGiftShopEvent);

	Show(false);

	return TRUE;
}

VOID CGiftSideViewGui::Destroy()
{
	UnLinkMsg(g_EventGiftShopEvent);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CGiftSideViewGui::OnPressESC()
{
}

VOID CGiftSideViewGui::OnSideViewOpen( const void* pData )
{
	RefreshText();

	Show(true);
}

VOID CGiftSideViewGui::OnSideViewClose()
{
	Show(false);
}

VOID CGiftSideViewGui::OnSideViewLocate( const CRectangle& rectSideIcon )
{
	RwInt32 iHeight = 90;
	m_pThis->SetHeight(iHeight);
	LocateComponent();
	m_pThis->SetPosition(rectSideIcon.left - m_pThis->GetWidth() + rectSideIcon.GetWidth(), rectSideIcon.top - iHeight);
}

VOID CGiftSideViewGui::LocateComponent()
{
	m_BackPanel.SetRect( m_pThis->GetScreenRect() );
}

VOID CGiftSideViewGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	LocateComponent();
}

VOID CGiftSideViewGui::OnPaint()
{
	m_BackPanel.Render();
}

VOID CGiftSideViewGui::HandleEvents( RWS::CMsg &msg )
{
	if (msg.Id == g_EventGiftShopEvent)
	{
		SDboEventGiftShopEvent* pData = (SDboEventGiftShopEvent*)msg.pData;

		if (pData->byEventType == eGIFTSHOP_EVENT_BUY_SUCCESS || pData->byEventType == eGIFTSHOP_EVENT_WP_UPDATED)
		{
			if (IsShow())
				RefreshText();
		}
	}
}

VOID CGiftSideViewGui::RefreshText()
{
	if (!m_pstbViewName)
		return;

	WCHAR wcText[128] = {0,};
	swprintf_s(wcText, GetDisplayStringManager()->GetString("DST_GIFT_SHOP_SIDE_ICON_TOOLTIP"), Logic_GetWaguPoint());
	m_pstbViewName->SetText(wcText);
}
