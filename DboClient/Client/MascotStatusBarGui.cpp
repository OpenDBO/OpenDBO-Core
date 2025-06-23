#include "precomp_dboclient.h"
#include "MascotStatusBarGui.h"

// Core
#include "NtlDebug.h"


// Presetation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"

// Dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DboLogic.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "DialogManager.h"

#include "ItemTable.h"

#define SUR_CHAR_LEFT_OFFSET 13
#define SUR_CHAR_TOP_OFFSET  17

CMascotStatusBarGui* CMascotStatusBarGui::m_pInstance = NULL;

CMascotStatusBarGui::CMascotStatusBarGui(const RwChar* pName)
	:CNtlPLGui(pName)
{
	Init();
}

CMascotStatusBarGui::~CMascotStatusBarGui()
{
	Destroy();
}

VOID CMascotStatusBarGui::Init()
{
}

RwBool CMascotStatusBarGui::Create()
{
	NTL_FUNCTION("CMascotStatusBarGui::Create");

	if (!CNtlPLGui::Create("gui\\MascotEx.rsr", "gui\\MascotStatusBar.srf", "gui\\MascotStatusBarEx.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgPetStatus");

	m_sttNickName = (gui::CStaticBox*)GetComponent("Stb_MascotName");
	m_sttSP = (gui::CStaticBox*)GetComponent("Stb_Vp");
	m_pgbSP = (gui::CProgressBar*)GetComponent("Pgb_Vp");

	m_slotMove = m_pThis->SigMove().Connect(this, &CMascotStatusBarGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CMascotStatusBarGui::OnPaint);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CMascotStatusBarGui::Destroy()
{

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CMascotStatusBarGui::HandleEvents(RWS::CMsg& msg)
{

}

RwInt32 CMascotStatusBarGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

void CMascotStatusBarGui::DeleteInstance()
{
	NTL_DELETE(m_pInstance);
}

void CMascotStatusBarGui::SetMascotID(BYTE index)
{
	SMascotInfo* MascotList = GetNtlSLGlobal()->GetAvatarMascotInfo();

	for (int i = 0; i < MascotList->wCount; ++i)
	{
		if (MascotList->asMascotData[i].byIndex == index)
		{
			sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(MascotList->asMascotData[i].itemTblidx);
			std::wstring MascotName = API_GetTableContainer()->GetTextAllTable()->GetItemTbl()->GetText(pItemTbldat->Name);

			m_pgbSP->SetRange(0, MascotList->asMascotData[i].dwMaxVP);
			m_pgbSP->SetPos(MascotList->asMascotData[i].dwCurVP);
			m_pgbSP->SetToolTip(GetDisplayStringManager()->GetString("DST_MASCOTEX_VP_REGEN")); // VP regenerates 1 point every 10 seconds
			m_sttNickName->SetText(MascotName.c_str());
			m_sttSP->Format(L"%u / %u", MascotList->asMascotData[i].dwCurVP, MascotList->asMascotData[i].dwMaxVP);

			if (m_surMascot.GetTexture())
			{
				Logic_DeleteTexture(m_surMascot.GetTexture());
				m_surMascot.UnsetTexture();
			}

			// Create Textrue
			m_surMascot.SetTexture(Logic_CreateTexture(pItemTbldat->szIcon_Name, TEXTYPE_ICON));

			break;
		}
	}
}

CMascotStatusBarGui* CMascotStatusBarGui::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = NTL_NEW CMascotStatusBarGui("Mascot Status Bar");
		if (!m_pInstance->Create())
		{
			NTL_DELETE(m_pInstance);
			return NULL;
		}
	}

	return m_pInstance;
}

VOID CMascotStatusBarGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pThis->GetScreenRect();

	// Portrait
	m_surMascot.SetRectWH(rect.left + SUR_CHAR_LEFT_OFFSET, rect.top + SUR_CHAR_TOP_OFFSET, 38, 38);

}

VOID CMascotStatusBarGui::OnPaint(VOID)
{
	// Pet Illust
	m_surMascot.Render();
}