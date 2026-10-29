#include "precomp_dboclient.h"
#include "DwcBackGroundGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlWorldConcept.h"

// dbo
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DialogManager.h"

#define dDWC_CURTAIN_FLASH		"DWC_curtain.swf"


CDwcBackGroundGui::CDwcBackGroundGui(const RwChar* pName)
: CNtlPLGui(pName)
{
	m_pflaCurtain = NULL;
}

CDwcBackGroundGui::~CDwcBackGroundGui(VOID)
{
}

RwBool CDwcBackGroundGui::Create(VOID)
{
	NTL_FUNCTION("CDwcBackGroundGui::Create");

	if (!CNtlPLGui::Create("gui\\DWC.rsr", "gui\\DWC.srf", "gui\\Backboard.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_RESULT);

	// tw: the curtain covers the screen and does not take the mouse
	m_pflaCurtain = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());
	m_pflaCurtain->SetResizeType(gui::CFlash::EXACTFIT);
	m_pflaCurtain->Enable(false);
	m_slotMovieEnd = m_pflaCurtain->SigMovieEnd().Connect(this, &CDwcBackGroundGui::OnMovieEnd);

	m_pThis->Enable(false);

	LinkMsg(g_EventResize);
	LinkMsg(g_EventCharReady);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDwcBackGroundGui::Destroy(VOID)
{
	NTL_FUNCTION("CDwcBackGroundGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck(this);

	UnLinkMsg(g_EventResize);
	UnLinkMsg(g_EventCharReady);

	if (m_pflaCurtain)
	{
		m_pflaCurtain->Unload();
		NTL_DELETE(m_pflaCurtain);
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CDwcBackGroundGui::Update(RwReal fElapsed)
{
	if (IsShow())
		m_pflaCurtain->Update(fElapsed);
}

RwInt32 CDwcBackGroundGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		if (!m_pflaCurtain->Load(dDWC_CURTAIN_FLASH))
		{
			DBO_WARNING_MESSAGE("CDwcBackGroundGui::SwitchDialog, can not load " << dDWC_CURTAIN_FLASH);
			return -1;
		}

		m_pflaCurtain->AutoRewind(FALSE);
		m_pflaCurtain->SetFrameCallback(TRUE);	// SigMovieEnd only fires with it

		LocateComponent();

		m_pflaCurtain->PlayMovie(TRUE);
		GetNtlGuiManager()->AddUpdateFunc(this);
	}
	else
	{
		GetNtlGuiManager()->RemoveUpdateFunc(this);
		m_pflaCurtain->Unload();
	}

	Show(bOpen);
	return TRUE;
}

VOID CDwcBackGroundGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id == g_EventResize)
	{
		if (IsShow())
			LocateComponent();
	}
	else if (msg.Id == g_EventCharReady)
	{
		// tw: the curtain opens once the avatar stands in the dwc world
		if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DWC) && !IsShow())
			GetDialogManager()->OpenDialog(DIALOG_DWC_BACKGROUND);
	}
}

VOID CDwcBackGroundGui::LocateComponent(VOID)
{
	CRectangle rtScreen(0, 0, GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	m_pThis->SetPosition(rtScreen);
	m_pflaCurtain->SetPosition(rtScreen);
}

VOID CDwcBackGroundGui::OnMovieEnd(gui::CComponent* pComponent)
{
	// the curtain opened, the world is visible
	GetDialogManager()->CloseDialog(DIALOG_DWC_BACKGROUND);
}
