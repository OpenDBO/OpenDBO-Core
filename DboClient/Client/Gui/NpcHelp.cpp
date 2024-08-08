#include "precomp_dboclient.h"

// core
#include "NpcHelp.h"
#include "NtlDebug.h"

// shared

// presentation
#include "NtlPLGuiManager.h"

// simulation 
#include "NtlSLEvent.h"
#include "NtlSobNpc.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"

// dbo
#include "DboGlobal.h"


CNpcHelpGui::CNpcHelpGui()
{
}

CNpcHelpGui::CNpcHelpGui(const RwChar *pName)
:CNtlPLGui(pName)
{
}

CNpcHelpGui::~CNpcHelpGui()
{
}

RwBool CNpcHelpGui::Create(void)
{
	NTL_FUNCTION("CNpcHelpGui::Create");

	if(!CNtlPLGui::Create("gui\\NpcHelp.rsr", "gui\\NpcHelp.srf", "gui\\NpcHelp.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_slotDialogPaint = m_pThis->SigPaint().Connect(this, &CNpcHelpGui::OnDialogPaint);

	m_psttDisplay = (gui::CStaticBox*)GetComponent("sttDisplay");

	Show(false);

	// event µî·Ï.
	LinkMsg(g_EventSobNpcCommunity, 0);
	
	NTL_RETURN(TRUE);
}

void CNpcHelpGui::Destroy(void)
{
	NTL_FUNCTION("CNpcHelpGui::Destroy");

	UnLinkMsg(g_EventSobNpcCommunity);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

RwInt32 CNpcHelpGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

void CNpcHelpGui::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNpcHelpGui::HandleEvents");

	if(pMsg.Id == g_EventSobNpcCommunity)
	{
		SNtlEventSobNpcCommunity *pNpcCommunity = reinterpret_cast<SNtlEventSobNpcCommunity*>(pMsg.pData); 
		if(pNpcCommunity->bShow)
		{
			m_hTargetSerail = pNpcCommunity->hSerialId;
			SetNpcWords(m_hTargetSerail);
			Show(true);
			OnDialogPaint();
		}
		else
		{
			m_hTargetSerail = INVALID_SERIAL_ID;
			Show(false);
		}
	}

	NTL_RETURNVOID();
}

void CNpcHelpGui::SetNpcWords(RwUInt32 uiTargetSerail)
{
	CNtlSobNpc *pSobNpc = reinterpret_cast<CNtlSobNpc*>(GetNtlSobManager()->GetSobObject(uiTargetSerail));
	if(pSobNpc == NULL)
		return;

//	CNtlChapterNpc *pChapterNpc = pSobNpc->GetChapterNpc();
//	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

//	m_psttDisplay->SetShadowColor(RGB(200, 200, 200));
//	m_psttDisplay->SetText(pChapterNpc->GetDialog());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// signal function


void CNpcHelpGui::OnDialogPaint(void)
{
	/*
	CNtlSobNpc *pSobNpc = reinterpret_cast<CNtlSobNpc*>(GetNtlSobManager()->GetSobObject(m_hTargetSerail, FALSE));
	if(pSobNpc == NULL)
	{
		Show(false);
		return;
	}

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	RwV3d vAvatarPos = pSobAvatar->GetPosition();
	RwV3d vNpcPos = pSobNpc->GetPosition(); 
	RwV3d vDist;
	RwV3dSub(&vDist, &vAvatarPos, &vNpcPos);
	RwReal fLen = RwV3dLength(&vDist);
	if(fLen > 15.0f)
	{
		Show(false);
		return;
	}
	
	CNtlChapterNpc *pChapterNpc = pSobNpc->GetChapterNpc();

	RwV3d vPos = pSobNpc->GetPosition();
	RwReal fHeight = pChapterNpc->GetScaleHeight();
	vPos.y += fHeight + 0.5f;

	RwV3d v2dPoint;
	RwMatrix *pMatView = RwCameraGetViewMatrix(CNtlPLGlobal::m_RwCamera);
	RwV3dTransformPoints(&v2dPoint, &vPos, 1, pMatView);
	v2dPoint.x = (v2dPoint.x / v2dPoint.z) * GetDboGlobal()->GetScreenWidth();
	v2dPoint.y = (v2dPoint.y / v2dPoint.z) * GetDboGlobal()->GetScreenHeight();

	CPos ptNewPos;
	CRectangle rect = GetPosition();
	RwInt32 iNewPosX = (RwInt32)(v2dPoint.x - rect.GetWidth()/2);
	RwInt32 iNewPosY = (RwInt32)(v2dPoint.y - rect.GetHeight() - 10);
	SetPosition(iNewPosX, iNewPosY);
	*/
}
