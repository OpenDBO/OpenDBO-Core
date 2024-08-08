#include "precomp_dboclient.h"
#include "YardratStoreGui.h"

#include "NtlDebug.h"
#include "DboDef.h"
#include "NtlPLGuiManager.h"
#include "StageManager.h"
#include "NtlStage.h"
#include "DboGlobal.h"

// dbo
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboPacketGenerator.h"
#include "DboEventGenerator.h"

#include "NtlSobItemAttr.h"

///////////////////////////////////////// CYardratStoreGui !!!!!!
CYardratStoreGui::CYardratStoreGui( const RwChar* pName )
:CNtlPLGui(pName)
{
	Init();
}

CYardratStoreGui::~CYardratStoreGui()
{

}

VOID CYardratStoreGui::Init(VOID)
{

}

RwBool CYardratStoreGui::Create(VOID)
{
	NTL_FUNCTION( "CYardratStoreGui::Create" );

	// Load window
	if (!CNtlPLGui::Create("", "gui\\Yardrat.srf", "gui\\Yardrat.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis						= (gui::CDialog*)GetComponent("dlgMain");

	m_pStatic_stbTitle			= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pButt_btnClose			= (gui::CButton*)GetComponent("btnClose");

	m_pDlg_dlgContentsScrolled	= (gui::CDialog*)GetComponent("dlgContentsScrolled");


	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	// Signal
	m_slotPaint_dlgMain			= m_pThis->SigPaint().Connect( this, &CYardratStoreGui::OnPaint );
	m_slotMove_dlgMain			= m_pThis->SigMove().Connect( this, &CYardratStoreGui::OnMove );

	// init
	m_DialogSurface.SetType(CWindowby3::WT_HORIZONTAL);
	m_DialogSurface.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Yardrat.srf", "srfDialogTop" ) );
	m_DialogSurface.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Yardrat.srf", "srfDialogBoddy" ) );
	m_DialogSurface.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Yardrat.srf", "srfDialogBottom" ) );

	m_pDlg_dlgContentsScrolled->SetParent( m_pThis );
	m_pDlg_dlgContentsScrolled->SetClippingMode( TRUE );

	UpdateSurfacePos();
	//SwitchDialog( false );

	//LinkMsg( g_EventCommercialExtendCommand, 0 );
	//LinkMsg(g_EventMsgBoxResult, 0);

	//// Update 연결
	//GetNtlGuiManager()->AddUpdateFunc( this );	/// 기간 update

	NTL_RETURN( TRUE );
}

VOID CYardratStoreGui::Destroy(VOID)
{
	NTL_FUNCTION( "CYardratStoreGui::Destroy" );

	//GetNtlGuiManager()->RemoveUpdateFunc( this );

	//UnLinkMsg( g_EventCommercialExtendCommand );
	//UnLinkMsg( g_EventMsgBoxResult );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CYardratStoreGui::UpdateSurfacePos()
{
	CRectangle rtScreen;



	rtScreen = m_pThis->GetScreenRect();
	m_DialogSurface.SetRect( rtScreen );
}

RwInt32 CYardratStoreGui::SwitchDialog(bool bOpen)
{
	if(bOpen)
		UpdateSurfacePos();

	Show(bOpen);

	return 1;
}

VOID CYardratStoreGui::OnPaint(VOID)
{
	m_DialogSurface.Render();


}

VOID CYardratStoreGui::OnMove(int nX,int nY)
{
	UpdateSurfacePos();
}
///////////////////////////////////////// HandleEvents  !!!!!!
VOID CYardratStoreGui::HandleEvents( RWS::CMsg& msg )
{

}


///////////////////////////////////////// Test Code