#include "precomp_dboclient.h"
#include "PartyBuffGui.h"

// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"
#include "CharmTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSobBuff.h"

// dbo
#include "DboDef.h"
#include "InfoWndManager.h"
#include "DialogPriority.h"
#include "InfoWndManager.h"
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"
#include "AlarmManager.h"


CPartyBuffGui::CPartyBuffGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_fElapsed(0.f)
,m_fRemainTime(0.f)
,m_byBuffIndex(0)
,m_bRightMouseDown(FALSE)
,m_bMouseEnter(FALSE)
{

}

CPartyBuffGui::~CPartyBuffGui()
{

}

RwBool CPartyBuffGui::Create(RwUInt8 byBuffIndex, RwUInt32 uiItemIndex)
{
	NTL_FUNCTION( "CPartyBuffGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\PartyCharmBuff.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_PARTY_MENU);

	m_CoolTimeEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" ) );

	m_slot.Create(m_pThis, DIALOG_PARTYMENU, REGULAR_SLOT_ITEM_TABLE);

	if( !SetBuffItemIndex(byBuffIndex, uiItemIndex) )
		NTL_RETURN(FALSE);

	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CPartyBuffGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CPartyBuffGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CPartyBuffGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseEnter().Connect( this, &CPartyBuffGui::OnMouseEnter );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CPartyBuffGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CPartyBuffGui::OnPaint );

	NTL_RETURN(TRUE);
}

VOID CPartyBuffGui::Destroy()
{
	NTL_FUNCTION("CPartyBuffGui::Destroy");

	CheckInfoWindow();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CPartyBuffGui::Update( RwReal fElapsed )
{
	m_fElapsed += fElapsed;

	if( m_fElapsed > m_fRemainTime)
		m_fElapsed = m_fRemainTime;

	m_CoolTimeEffect.Update(m_fElapsed);

	// 인포인도우를 보고 있다면 쿨타임을 갱신하여 준다
	if( m_bMouseEnter )
		OnMouseEnter(NULL);
}

VOID CPartyBuffGui::Switch(bool bOpen)
{
	Show(bOpen);
}

RwBool CPartyBuffGui::SetBuffItemIndex(RwUInt8 byBuffIndex, RwUInt32 uiItemIndex)
{
	if( m_slot.SetIcon(uiItemIndex) == FALSE )
		return FALSE;

	sITEM_TBLDAT* pITEM_TBLDAT = m_slot.GetItemTable();
	NTL_ASSERT(pITEM_TBLDAT, "CPartyBuffGui::SetBuffItemIndex, Not exist item table of index : " << uiItemIndex);
	sCHARM_TBLDAT* pCHARM_TBLDAT = reinterpret_cast<sCHARM_TBLDAT*>( API_GetTableContainer()->GetCharmTable()->FindData( pITEM_TBLDAT->Charm_Tblidx ) );
	NTL_ASSERT(pCHARM_TBLDAT, "CPartyBuffGui::SetBuffItemIndex, Not exist charm table of index : " << pITEM_TBLDAT->Charm_Tblidx);

	m_byBuffIndex = byBuffIndex;
	m_fRemainTime = pCHARM_TBLDAT->wKeep_Time;

	m_CoolTimeEffect.StartProc(pCHARM_TBLDAT->wKeep_Time, TRUE, TRUE);
	m_CoolTimeEffect.Update(0.0f);

	return TRUE;
}

RwUInt8 CPartyBuffGui::GetBuffIndex()
{
	return m_byBuffIndex;
}

VOID CPartyBuffGui::OnMouseDown(const CKey& key)
{
	if(key.m_nID != UD_RIGHT_BUTTON)
		return;

	m_bRightMouseDown = TRUE;
}

VOID CPartyBuffGui::OnMouseUp(const CKey& key)
{
	if(key.m_nID != UD_RIGHT_BUTTON)
	{
		m_bRightMouseDown = FALSE;
		return;
	}

	sMsgBoxData msgData;
	//const WCHAR* pwcText = GetDisplayStringManager()->GetString(DST_PARTY_RELEASE_CHARM);

	msgData.uiParam = m_byBuffIndex;
	//CDboEventGenerator::MsgBoxShow(pwcText, MBW_PARTY_RELEASE_CHARM, MBTF_OK | MBTF_CANCEL, FALSE, 0.f, &msgData);
	GetAlarmManager()->AlarmMessage( DST_PARTY_RELEASE_CHARM, FALSE, &msgData );

}

VOID CPartyBuffGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_slot.SetPosition_fromParent(rtScreen.left, rtScreen.top);
	m_CoolTimeEffect.SetPosition(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

VOID CPartyBuffGui::OnMouseEnter(gui::CComponent* pComponent)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_CHARM_BUFF,
										rtScreen.left, rtScreen.top,
										m_slot.GetItemTable(), DIALOG_PARTYMENU );
	m_bMouseEnter = TRUE;
}

VOID CPartyBuffGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
	m_bMouseEnter = FALSE;
}

VOID CPartyBuffGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetInfoWndState() == CInfoWndManager::INFOWND_CHARM_BUFF &&
		GetInfoWndManager()->GetRequestGui() == DIALOG_PARTYMENU )
	{
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CPartyBuffGui::OnPaint()
{
	m_slot.Paint();
	m_CoolTimeEffect.Render();
}