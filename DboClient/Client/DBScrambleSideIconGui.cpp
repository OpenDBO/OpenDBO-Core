#include "precomp_dboclient.h"
#include "DBScrambleSideIconGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobPlayerAttr.h"

// dbo
#include "DboEvent.h"
#include "DboGlobal.h"
#include "DialogManager.h"
#include "AlarmManager.h"
#include "DisplayStringManager.h"


namespace
{
	bool IsAvatarInDBScramble()
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if (!pAvatar)
			return false;

		return reinterpret_cast<CNtlSobPlayerAttr*>(pAvatar->GetSobAttr())->GetIsInDBSBattle();
	}
}


CDBScrambleSideIconGui::CDBScrambleSideIconGui(const RwChar* pName)
	: CSideIconBase(pName)
	, m_pBtnIcon(NULL)
{
}

CDBScrambleSideIconGui::~CDBScrambleSideIconGui(void)
{
}

RwBool CDBScrambleSideIconGui::Create()
{
	NTL_FUNCTION(__FUNCTION__);

	if (!CNtlPLGui::Create("gui\\Game.rsr", "gui\\DBSSideIcon.srf", "gui\\DBSSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pBtnIcon = (gui::CButton*)GetComponent("btnDBSSideIcon");
	if (m_pBtnIcon == NULL)
		NTL_RETURN(FALSE);

	m_slotClicked = m_pBtnIcon->SigClicked().Connect(this, &CDBScrambleSideIconGui::OnIconButtonClicked);

	UpdateToolTip();

	LinkMsg(g_EventDBScramble);

	// the scramble may have started before the gui was created
	Show(true);

	NTL_RETURN(TRUE);
}

VOID CDBScrambleSideIconGui::Destroy()
{
	UnLinkMsg(g_EventDBScramble);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CDBScrambleSideIconGui::HandleEvents(RWS::CMsg& msg)
{
	if (msg.Id != g_EventDBScramble)
		return;

	const SDboEventDBScramble* pEvent = reinterpret_cast<const SDboEventDBScramble*>(msg.pData);

	switch (pEvent->byEventType)
	{
		case DBSCRAMBLE_EVENT_SEASON_START:
		{
			UpdateToolTip();
			Show(true);
		}
		break;
		case DBSCRAMBLE_EVENT_SEASON_END:
		{
			GetDialogManager()->CloseDialog(DIALOG_DBSCRAMBLE_CONFIRM);
			Show(false);
		}
		break;
		case DBSCRAMBLE_EVENT_JOIN:
		{
			if (IsAvatarInDBScramble())
				GetDialogManager()->CloseDialog(DIALOG_DBSCRAMBLE_CONFIRM);

			UpdateToolTip();
		}
		break;

		default: break;
	}
}

void CDBScrambleSideIconGui::UpdateToolTip()
{
	m_pBtnIcon->SetToolTip(GetDisplayStringManager()->GetString(IsAvatarInDBScramble() ? "DST_DRAGONBALL_SCRAMBLE_I_AM_PLAYER" : "DST_DRAGONBALL_SCRAMBLE_I_AM_NOT_PLAYER"));
}

VOID CDBScrambleSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	if (IsAvatarInDBScramble())
	{
		GetAlarmManager()->AlarmMessage("DST_DRAGONBALL_SCRAMBLE_ALREADY_JOIN");
		return;
	}

	GetDialogManager()->SwitchDialog(DIALOG_DBSCRAMBLE_CONFIRM);
}

VOID CDBScrambleSideIconGui::OnSideViewClosed()
{
}

void CDBScrambleSideIconGui::Show(bool bShow)
{
	__super::Show(bShow && GetDboGlobal()->GetDBScrambleInfo()->bSeason);
}
