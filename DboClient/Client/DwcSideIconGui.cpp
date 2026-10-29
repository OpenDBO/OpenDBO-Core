#include "precomp_dboclient.h"
#include "DwcSideIconGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"

#define dDWC_SIDEVIEW_HEIGHT		100


////////////////////////////////////////////////////////////////////////////////
// CDwcSideIconGui

CDwcSideIconGui::CDwcSideIconGui(const RwChar* pName)
: CSideIconBase(pName)
, m_pbtnIcon(NULL)
{
}

CDwcSideIconGui::~CDwcSideIconGui(void)
{
}

RwBool CDwcSideIconGui::Create()
{
	NTL_FUNCTION(__FUNCTION__);

	if (!CNtlPLGui::Create("gui\\DWC.rsr", "gui\\DWC.srf", "gui\\DWC_SideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pbtnIcon = (gui::CButton*)GetComponent("btnIcon");
	m_slotIconButton = m_pbtnIcon->SigClicked().Connect(this, &CDwcSideIconGui::OnIconButtonClicked);
	m_slotMouseEnter = m_pbtnIcon->SigMouseEnter().Connect(this, &CDwcSideIconGui::OnMouseEnter);
	m_slotMouseLeave = m_pbtnIcon->SigMouseLeave().Connect(this, &CDwcSideIconGui::OnMouseLeave);

	LinkMsg(g_EventDwcNotify);

	// tw: the icon waits for news of the dwc
	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDwcSideIconGui::Destroy()
{
	UnLinkMsg(g_EventDwcNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CDwcSideIconGui::HandleEvents(RWS::CMsg &msg)
{
	if (msg.Id == g_EventDwcNotify)
	{
		SDboEventDwcNotify* pData = reinterpret_cast<SDboEventDwcNotify*>(msg.pData);

		// tw: the challenge hides the icon when it opens
		if (pData->byType == DWC_EVENT_CHALLENGE_OPEN)
		{
			CSideIconGui::GetInstance()->CloseSideView(SIDEVIEW_DWC);
			Show(false);
			return;
		}

		// a new card, or a new admission condition met (the login info has no achievement index)
		RwBool bNews = pData->byType == DWC_EVENT_MISSION_REWARD || (pData->byType == DWC_EVENT_ACHIEVEMENT && pData->uiValue != 0);
		if (!bNews)
			return;

		// the challenge open shows it already
		if (GetDialogManager()->IsOpenDialog(DIALOG_DWC_CHALLENGE))
			return;

		Show(true);
	}
}

VOID CDwcSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->CloseSideView(SIDEVIEW_DWC);

	// tw: the icon opens the challenge, the challenge hides the icon
	GetDialogManager()->OpenDialog(DIALOG_DWC_CHALLENGE);
	Show(false);
}

VOID CDwcSideIconGui::OnSideViewClosed()
{
}

void CDwcSideIconGui::OnMouseEnter(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_DWC, NULL);
}

void CDwcSideIconGui::OnMouseLeave(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->CloseSideView(SIDEVIEW_DWC);
}


////////////////////////////////////////////////////////////////////////////////
// CDwcSideViewGui

CDwcSideViewGui::CDwcSideViewGui(const RwChar* pName)
: CSideViewBase(pName)
, m_pstbViewName(NULL)
{
}

CDwcSideViewGui::~CDwcSideViewGui(void)
{
}

RwBool CDwcSideViewGui::Create()
{
	// tw: the view uses the background of the wagu machine side view
	if (!CNtlPLGui::Create("gui\\WaguMachineSideIcon.rsr", "gui\\WaguMachineSideIcon.srf", "gui\\DWCsideView.frm"))
		return FALSE;

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pstbViewName = (gui::CStaticBox*)GetComponent("stbViewName");
	m_pstbViewName->SetText(GetDisplayStringManager()->GetString("DST_DWC_SIDE_VIEW"));

	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WaguMachineSideIcon.srf", "srfDialogBackUp"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WaguMachineSideIcon.srf", "srfDialogBackCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("WaguMachineSideIcon.srf", "srfDialogBackDown"));

	m_slotPaint = m_pThis->SigPaint().Connect(this, &CDwcSideViewGui::OnPaint);
	m_slotMove = m_pThis->SigMove().Connect(this, &CDwcSideViewGui::OnMove);

	Show(false);

	return TRUE;
}

VOID CDwcSideViewGui::Destroy()
{
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CDwcSideViewGui::OnPressESC()
{
}

VOID CDwcSideViewGui::OnSideViewOpen(const void* pData)
{
	Show(true);
}

VOID CDwcSideViewGui::OnSideViewClose()
{
	Show(false);
}

VOID CDwcSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{
	m_pThis->SetHeight(dDWC_SIDEVIEW_HEIGHT);
	m_pThis->SetPosition(rectSideIcon.left - m_pThis->GetWidth() + rectSideIcon.GetWidth(), rectSideIcon.top - dDWC_SIDEVIEW_HEIGHT);
	m_BackPanel.SetRect(m_pThis->GetScreenRect());
}

VOID CDwcSideViewGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	m_BackPanel.SetRect(m_pThis->GetScreenRect());
}

VOID CDwcSideViewGui::OnPaint()
{
	m_BackPanel.Render();
}
