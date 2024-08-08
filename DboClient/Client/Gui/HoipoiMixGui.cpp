#include "precomp_dboclient.h"
#include "HoipoiMixGui.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"
#include "NtlSobDynamicObject.h"
#include "NtlSobManager.h"
#include "ntlSobTriggerObjectAttr.h"
#include "NtlSobDynamicObjectAttr.h"

// Shared
#include "DynamicObjectTable.h"

// Client
#include "DialogDefine.h"
#include "DialogManager.h"
#include "DialogPriority.h"

#include "HoipoiMixRecipe.h"
#include "HoipoiMixCreate.h"
#include "DisplayStringManager.h"

#include "DboEvent.h"

#include "InfoWndManager.h"



CHoipoiMixCapsuleGui::CHoipoiMixCapsuleGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	m_pTabButton = NULL;
	m_eCurTab	= RECIPE_TAB;

	m_pStbTitle = NULL;
	m_pBtnClose = NULL;

	m_pRecipeTab = NULL;
	m_pCreateTab = NULL;

	m_hObject = INVALID_SERIAL_ID;
}

CHoipoiMixCapsuleGui::~CHoipoiMixCapsuleGui()
{
	
}

RwBool CHoipoiMixCapsuleGui::Create()
{
	NTL_FUNCTION( "CHoipoiMixCapsule::Create" );

	if( !CNtlPLGui::Create( "", "gui\\HoipoiMix.srf", "gui\\HoipoiMix.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pThis->SetPriority( dDIALOGPRIORITY_DEFAULT );

	m_pBtnClose = (gui::CButton*)GetComponent( "btnClose" );
	m_slotClickedClose = m_pBtnClose->SigClicked().Connect( this, &CHoipoiMixCapsuleGui::OnClickedClose );

	m_pTabButton = (gui::CTabButton*)GetComponent( "tabHoipoiMix" );
	m_slotSelectTab = m_pTabButton->SigSelectChanged().Connect( this, &CHoipoiMixCapsuleGui::OnSelectTab );

	// 각자의 TAB들을 생성
	m_pRecipeTab = NTL_NEW CHoipoiMixRecipe();
	if( !m_pRecipeTab->Create( this ) )
	{
		DBO_FAIL( "Failed to RecipeTab.");
		NTL_DELETE( m_pRecipeTab );
		NTL_RETURN( FALSE );
	}

	m_pCreateTab = NTL_NEW CHoipoiMixCreate();
	if( !m_pCreateTab->Create( this ) )
	{
		DBO_FAIL( "Failed to CreateTab.");
		NTL_DELETE( m_pCreateTab );
		NTL_RETURN( FALSE );
	}
	
	m_pTabButton->AddTab( std::wstring(GetDisplayStringManager()->GetString( DST_HOIPOIMIX_RECIPE_TAB )) );
	m_pTabButton->AddTab( std::wstring(GetDisplayStringManager()->GetString( DST_HOIPOIMIX_CREATE_TAB )) );
	m_pTabButton->EnableTab( CREATE_TAB, FALSE );
		
	m_pTabButton->SelectTab( RECIPE_TAB );
	SelectTab( RECIPE_TAB );

	Show( false );

	LinkMsg( g_EventNPCDialogOpen );
	LinkMsg( g_EventSobDelete );
	LinkMsg( g_EventHoipoiMixRecipeRegNfy );
	LinkMsg( g_EventHoipoiMixCreateStart );
	LinkMsg( g_EventCalcPopupResult );
	LinkMsg( g_EventHoipoiMixItemMakeRes );
	LinkMsg( g_EventSobInfoUpdate );
	LinkMsg( g_EventHoipoiMixJobResetRes );

	NTL_RETURN( TRUE );
}

VOID CHoipoiMixCapsuleGui::Destroy()
{
	if( m_pRecipeTab )
	{
		m_pRecipeTab->Destroy();
		NTL_DELETE( m_pRecipeTab );
	}

	if( m_pCreateTab )
	{
		m_pCreateTab->Destroy();
		NTL_DELETE( m_pCreateTab );
	}

	UnLinkMsg( g_EventNPCDialogOpen );
	UnLinkMsg( g_EventSobDelete );
	UnLinkMsg( g_EventHoipoiMixRecipeRegNfy );
	UnLinkMsg( g_EventHoipoiMixCreateStart );
	UnLinkMsg( g_EventCalcPopupResult );
	UnLinkMsg( g_EventHoipoiMixItemMakeRes );
	UnLinkMsg( g_EventSobInfoUpdate );
	UnLinkMsg( g_EventHoipoiMixJobResetRes );


	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CHoipoiMixCapsuleGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{
		if( m_eCurTab == CREATE_TAB )
			m_eCurTab = RECIPE_TAB;

		m_pTabButton->SelectTab( m_eCurTab );
		SelectTab(m_eCurTab);

		m_pRecipeTab->SelectFirstItem();
	}
	else
	{
		CNtlSLEventGenerator::CameraControlDelete( CAMERA_CONTROL_NPC );
		GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_NPC_COMMU );
		m_hObject = INVALID_SERIAL_ID;


		if ( GetInfoWndManager()->GetRequestGui() == DIALOG_HOIPOIMIX_CAPSULE )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
	}

	return 1;
}

VOID CHoipoiMixCapsuleGui::SelectTab( eTAB eTabIndex )
{
	m_eCurTab = eTabIndex;

	switch( eTabIndex )
	{
	case RECIPE_TAB:
		m_pRecipeTab->Show( true );
		m_pCreateTab->Show( false );
		break;
	case CREATE_TAB:
		m_pRecipeTab->Show( false );
		m_pCreateTab->InitGui();
		m_pCreateTab->Show( true );
		m_pCreateTab->InitFlash();
		break;
	default:
		break;
	}
}

VOID CHoipoiMixCapsuleGui::SelectRecipe( CRecipeDlgItem* pSelectItem )
{
	if( pSelectItem )
	{
		m_pRecipeTab->SelectRecipeItem( pSelectItem );
		m_pCreateTab->SetRecipe( pSelectItem->GetRecipeTableIndex() );
		m_pTabButton->EnableTab( CREATE_TAB, TRUE );
	}
	else
	{
		m_pRecipeTab->DeSelectRecipeItem();
		m_pTabButton->EnableTab( CREATE_TAB, FALSE );
	}
}

VOID CHoipoiMixCapsuleGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventNPCDialogOpen )
	{
		SDboNpcDialogOpen* pData = reinterpret_cast<SDboNpcDialogOpen*>(msg.pData);

		if(pData->eDialog != DIALOG_HOIPOIMIX_CAPSULE)
			return;

		if( pData->hSerialId == INVALID_SERIAL_ID )
		{
			DBO_FAIL( "invalid serial id" );
			return;
		}

		m_hObject = pData->hSerialId;

		CNtlSob* pSob = GetNtlSobManager()->GetSobObject( pData->hSerialId );
		if( pSob->GetClassID() == SLCLASS_TRIGGER_OBJECT )
		{
			// Func가 HOIPOIMIX 일 경우
			CNtlSobTriggerObjectAttr* pSobAttr = (CNtlSobTriggerObjectAttr*)pSob->GetSobAttr();
			sOBJECT_TBLDAT* pObjTableData = (sOBJECT_TBLDAT*)pSobAttr->GetTriggerObjectTbl();
			if( pObjTableData->wFunction & eDBO_TRIGGER_OBJECT_FUNC_HOIPOIMIX )
			{

				// Built-In을 제거
				
				SelectRecipe( NULL );
				m_pRecipeTab->DestroyBuiltInRecipeList();
				

				GetDialogManager()->OpenDialog( DIALOG_HOIPOIMIX_CAPSULE, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
			}
		}
		else if( pSob->GetClassID() == SLCLASS_DYNAMIC_OBJECT )
		{
			CNtlSobDynamicObject* pSobObj = (CNtlSobDynamicObject*)GetNtlSobManager()->GetSobObject( m_hObject );
			if( !pSobObj )
			{
				NTL_ASSERTFAIL( "CHoipoiMixCapsuleGui::HandleEvents - Invalid object serial id" << m_hObject );
				return;
			}

			// 다이나믹 오브젝트가 HoipoiMix 타입일 경우에만 Open 한다.
			CNtlSobDynamicObjectAttr* pSobObjAttr = (CNtlSobDynamicObjectAttr*)pSobObj->GetSobAttr();
			if( pSobObjAttr->GetDynamicObjectType() != eDBO_DYNAMIC_OBJECT_TYPE_HOIPOI_MIX_MACHINE )
				return;

			// 호이포이 믹스 TblIdx를 세팅

			SelectRecipe( NULL );
			m_pRecipeTab->CreateBuiltInRecipeList( pSobObjAttr->GetTableIndexForType() );

			GetDialogManager()->OpenDialog( DIALOG_HOIPOIMIX_CAPSULE, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );		
		}
	}
	else if( msg.Id == g_EventSobDelete )
	{
		SNtlEventSobDelete* pData = reinterpret_cast<SNtlEventSobDelete*>(msg.pData);

		if( pData->hSerialId == m_hObject )
		{
			GetDialogManager()->CloseDialog( DIALOG_HOIPOIMIX_CAPSULE );
		}
	}
	else if( msg.Id == g_EventHoipoiMixCreateStart )
	{
		SDboEventHoipoiMixCreateStart* pData = reinterpret_cast<SDboEventHoipoiMixCreateStart*>( msg.pData );

		// 만약 이 이벤트가 날라오면 탭을 제조탭으로 바꾸고 레시피로 세팅을 한다.
		m_pTabButton->SelectTab( CREATE_TAB );
		SelectTab( CREATE_TAB );
		m_pCreateTab->SetRecipe( pData->idxRecipeTbl );
	}
	else if( msg.Id == g_EventCalcPopupResult ||
		msg.Id == g_EventHoipoiMixItemMakeRes ||
		msg.Id == g_EventSobInfoUpdate )
	{
		m_pCreateTab->HandleEvents( msg );
	}
	else if( msg.Id == g_EventHoipoiMixRecipeRegNfy ||
		msg.Id == g_EventHoipoiMixJobResetRes)
	{
		m_pRecipeTab->HandleEvents( msg );
	}
}

VOID CHoipoiMixCapsuleGui::OnClickedClose( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_HOIPOIMIX_CAPSULE );
}

VOID CHoipoiMixCapsuleGui::OnSelectTab( RwInt32 nSelectIdx, RwInt32 nPreviousIndex )
{
	SelectTab( (eTAB) nSelectIdx );
}

VOID CHoipoiMixCapsuleGui::Update( RwReal fElapsed )
{
	// 자기 자신 업데이트

	// 탭 업데이트
	m_pCreateTab->Update( fElapsed );
}

SERIAL_HANDLE CHoipoiMixCapsuleGui::GetObject()
{
	return m_hObject;
}

