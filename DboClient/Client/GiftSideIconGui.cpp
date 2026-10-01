#include "precomp_dboclient.h"
#include "GiftSideIconGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DialogManager.h"

CGiftSideIconGui::CGiftSideIconGui(const RwChar* pName)
	: CSideIconBase(pName)
	, m_pBtnGift(NULL)
{
}

CGiftSideIconGui::~CGiftSideIconGui(void)
{
}

RwBool CGiftSideIconGui::Create()
{
	NTL_FUNCTION(__FUNCTION__);

	if (!CNtlPLGui::Create("gui\\GiftShopGui.rsr", "gui\\GiftShopGui.srf", "gui\\GiftShop_SideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pBtnGift = (gui::CButton*)GetComponent("btnIcon");
	if (m_pBtnGift)
	{
		m_slotGiftBtn = m_pBtnGift->SigClicked().Connect(this, &CGiftSideIconGui::OnIconButtonClicked);
		m_slotGiftMouseEnter = m_pBtnGift->SigMouseEnter().Connect(this, &CGiftSideIconGui::OnMouseEnter);
		m_slotGiftMouseLeave = m_pBtnGift->SigMouseLeave().Connect(this, &CGiftSideIconGui::OnMouseLeave);
	}

	Show(true);

	NTL_RETURN(TRUE);
}

VOID CGiftSideIconGui::Destroy()
{
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
	Show(false);
}

VOID CGiftSideIconGui::HandleEvents(RWS::CMsg &msg)
{
}

VOID CGiftSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->CloseSideView(SIDEVIEW_GIFT);

	GetDialogManager()->SwitchDialog(DIALOG_GIFTSHOP);
}

VOID CGiftSideIconGui::OnSideViewClosed()
{
}

void CGiftSideIconGui::OnMouseEnter(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_GIFT, NULL);
}

void CGiftSideIconGui::OnMouseLeave(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->CloseSideView(SIDEVIEW_GIFT);
}

void CGiftSideIconGui::Show(bool bShow)
{
	__super::Show(bShow);
}
