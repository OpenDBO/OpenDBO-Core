#include "precomp_dboclient.h"
#include "DojoSideIconGui.h"

// shared
#include "NtlDojo.h"
#include "DojoTable.h"
#include "TableContainer.h"
#include "TextAllTable.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"

// client
#include "AlarmManager.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"


#define dSTRING_HEIGHT_GAP			(10)
#define dVIEWBOTTOM_GAP				(20)



//////////////////////////////////////////////////////////////////////////
// CDojoSideIconGui
//////////////////////////////////////////////////////////////////////////

CDojoSideIconGui::CDojoSideIconGui(const RwChar* pName)
:CSideIconBase(pName)
{
}

CDojoSideIconGui::~CDojoSideIconGui()
{

}

RwBool CDojoSideIconGui::Create()
{
	NTL_FUNCTION( "CDojoSideIconGui::Create" );

	if(!CNtlPLGui::Create("gui\\Dogi.rsr", "gui\\DojoSideIcon.srf", "gui\\DojoSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pIconButton = (gui::CButton*)GetComponent("btnIcon");
	m_slotIconButton = m_pIconButton->SigClicked().Connect(this, &CDojoSideIconGui::OnIconButtonClicked);

	Show(false);	

	NTL_RETURN(TRUE);
}

VOID CDojoSideIconGui::Destroy()
{
	NTL_FUNCTION("CDojoSideIconGui::Destroy");	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

void CDojoSideIconGui::OnSideViewClosed()
{
}

void CDojoSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_DOJO, NULL);
}

VOID CDojoSideIconGui::SetToolTip(const WCHAR* pwcText)
{
	m_pIconButton->SetToolTip(pwcText ? pwcText : L"");
}

//////////////////////////////////////////////////////////////////////////
// CDojoSideViewGui
//////////////////////////////////////////////////////////////////////////

CDojoSideViewGui::CDojoSideViewGui(const RwChar* pName)
:CSideViewBase(pName)
,m_iViewHeight(0)
,m_fElapsed(0)
,m_uiShownDojo(INVALID_TBLIDX)
,m_byShownState(eDBO_DOJO_STATUS_NORMAL)
{
}

CDojoSideViewGui::~CDojoSideViewGui()
{

}

RwBool CDojoSideViewGui::Create()
{
	NTL_FUNCTION( "CDojoSideViewGui::Create" );

	if(!CNtlPLGui::Create("gui\\Dogi.rsr", "gui\\DojoSideIcon.srf", "gui\\DojoSideView.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	CRectangle rect;

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pExitButton = (gui::CButton*)GetComponent("btnExit");
	m_slotExitButton = m_pExitButton->SigClicked().Connect(this, &CDojoSideViewGui::OnClicked_ExitButton);

	m_pMessage = (gui::CStaticBox*)GetComponent("stbMessage");

	// 배경
	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DojoSideIcon.srf", "srfBackgroundTop"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DojoSideIcon.srf", "srfBackgroundCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("DojoSideIcon.srf", "srfBackgroundBottom"));

	GetNtlGuiManager()->AddUpdateFunc( this );

	// sig
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CDojoSideViewGui::OnPaint );

	LinkMsg(g_EventDojoStateNotify);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CDojoSideViewGui::Destroy()
{
	NTL_FUNCTION("CDojoSideViewGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	UnLinkMsg(g_EventDojoStateNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CDojoSideViewGui::Update(RwReal fElapsed)
{
	if( INVALID_TBLIDX == m_uiShownDojo )
		return;

	m_fElapsed += fElapsed;
	if( m_fElapsed < 1.f )
		return;

	m_fElapsed = 0.f;

	// keep the countdown running and drop challenge periods that ran out
	Refresh(FALSE);
}

// shows our guild war first, otherwise the dojos that are open for challenges
VOID CDojoSideViewGui::Refresh(RwBool bNotify)
{
	CSideIconBase* pSideIcon = CSideIconGui::GetInstance()->GetSideIcon(SIDEICON_DOJO);
	if( !pSideIcon )
		return;

	RwUInt64 tmNow = (RwUInt64)GetNtlSLGlobal()->GetServerTime();

	RwUInt32		uiShowDojo	= INVALID_TBLIDX;
	sDOJO_STATE*	pShowState	= NULL;

	MAP_DOJO_STATE::iterator it = m_mapDojoState.begin();
	while( it != m_mapDojoState.end() )
	{
		sDOJO_STATE& rState = it->second;

		if( eDBO_DOJO_STATUS_RECEIVE == rState.byState )
		{
			// nobody tells us when a challenge period of another guild runs out
			if( rState.tmNextStepTime <= tmNow )
			{
				it = m_mapDojoState.erase(it);
				continue;
			}

			if( !pShowState )
			{
				uiShowDojo = it->first;
				pShowState = &rState;
			}
		}
		else
		{
			// only the two guilds at war receive the other states
			if( !pShowState || eDBO_DOJO_STATUS_RECEIVE == pShowState->byState )
			{
				uiShowDojo = it->first;
				pShowState = &rState;
			}
		}

		++it;
	}

	if( !pShowState )
	{
		m_uiShownDojo	= INVALID_TBLIDX;
		m_byShownState	= eDBO_DOJO_STATUS_NORMAL;

		CSideIconGui::GetInstance()->CloseSideView(SIDEVIEW_DOJO);
		pSideIcon->Show(false);
		return;
	}

	const WCHAR* pwcState = NULL;
	switch( pShowState->byState )
	{
		case eDBO_DOJO_STATUS_RECEIVE:		pwcState = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_RECEIVE"); break;
		case eDBO_DOJO_STATUS_REJECT_END:	pwcState = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_REJECT_END"); break;
		case eDBO_DOJO_STATUS_STANDBY:		pwcState = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_STANDBY"); break;
		case eDBO_DOJO_STATUS_INITIAL:		pwcState = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_INITIAL"); break;
		case eDBO_DOJO_STATUS_READY:		pwcState = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_READY"); break;
		case eDBO_DOJO_STATUS_START:		pwcState = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_START"); break;
		case eDBO_DOJO_STATUS_END:			pwcState = GetDisplayStringManager()->GetString("DST_DOJO_SCRAMBLE_STATE_END"); break;
		default:							pwcState = L""; break;
	}

	// dojo name, state and the time left of the current step
	std::wstring wstrMessage;

	sDOJO_TBLDAT* pDOJO_TBLDAT = reinterpret_cast<sDOJO_TBLDAT*>( API_GetTableContainer()->GetDojoTable()->FindData(uiShowDojo) );
	if( pDOJO_TBLDAT && eDBO_DOJO_STATUS_RECEIVE != pShowState->byState )
	{
		CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetTextTbl(CTextAllTable::MAP_NAME);
		std::wstring wstrRegionName = pTextTable->GetText(pDOJO_TBLDAT->mapName);
		if( !wstrRegionName.empty() )
		{
			wstrMessage += wstrRegionName;
			wstrMessage += L" ";

			const WCHAR* pwcDojo = GetDisplayStringManager()->GetString("DST_DOJO");
			if( pwcDojo )
				wstrMessage += pwcDojo;

			wstrMessage += L"\n";
		}
	}

	wstrMessage += pwcState ? pwcState : L"";

	if( pShowState->tmNextStepTime > tmNow )
	{
		wstrMessage += L"\n";
		wstrMessage += Logic_GetTimeString( (RwReal)(pShowState->tmNextStepTime - tmNow) );
	}

	m_pMessage->SetText(wstrMessage.c_str());

	CRectangle rect = m_pMessage->GetPosition();
	m_iViewHeight = rect.bottom + dSTRING_HEIGHT_GAP + dVIEWBOTTOM_GAP;

	CDojoSideIconGui* pDojoSideIcon = static_cast<CDojoSideIconGui*>( pSideIcon );
	pDojoSideIcon->SetToolTip(pwcState);
	pSideIcon->Show(true);

	// tell the player once per step, not for every dojo opening at the same time
	RwBool bChanged = ( m_uiShownDojo != uiShowDojo || m_byShownState != pShowState->byState );

	m_uiShownDojo	= uiShowDojo;
	m_byShownState	= pShowState->byState;

	if( bNotify && bChanged )
	{
		if( pwcState && pwcState[0] )
			GetAlarmManager()->AlarmMessage(const_cast<WCHAR*>(pwcState));

		CSideIconGui::GetInstance()->OpenSideView(pSideIcon, SIDEVIEW_DOJO, NULL);
	}
}

VOID CDojoSideViewGui::OnPressESC()
{

}

void CDojoSideViewGui::OnSideViewOpen(const void* pData)
{
	Show(true);	
}

VOID CDojoSideViewGui::OnSideViewClose()
{
	Show(false);
}

VOID CDojoSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{
	CRectangle rtScreen;

	rtScreen.SetRectWH(rectSideIcon.left - m_BackPanel.GetWidth() + rectSideIcon.GetWidth(),
					   rectSideIcon.top - m_iViewHeight,
					   m_BackPanel.GetWidth(),
					   m_iViewHeight);	

	m_pThis->SetPosition(rtScreen);
	m_BackPanel.SetRect(rtScreen);
}

VOID CDojoSideViewGui::OnClicked_ExitButton(gui::CComponent* pComponent)
{
	Show(false);
}

VOID CDojoSideViewGui::OnPaint()
{
	m_BackPanel.Render();
}

VOID CDojoSideViewGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDojoSideViewGui::HandleEvents");

	if( msg.Id == g_EventDojoStateNotify )
	{
		SNtlEventDojoMessage* pEvent = reinterpret_cast<SNtlEventDojoMessage*>( msg.pData );

		if( eDBO_DOJO_STATUS_NORMAL == pEvent->byDojoState )
		{
			m_mapDojoState.erase(pEvent->uiDojoTableIndex);
		}
		else
		{
			sDOJO_STATE& rState	= m_mapDojoState[pEvent->uiDojoTableIndex];
			rState.byState			= pEvent->byDojoState;
			rState.tmNextStepTime	= pEvent->ui64time;
		}

		Refresh(TRUE);
	}

	NTL_RETURNVOID();
}
