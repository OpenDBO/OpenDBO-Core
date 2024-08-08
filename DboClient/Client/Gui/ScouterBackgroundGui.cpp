#include "precomp_dboclient.h"
#include "ScouterBackgroundGui.h"

// core
#include "NtlDebug.h"

// share
#include "itemTable.h"

// presetation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"

// dbo
#include "DboEvent.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DialogPriority.h"
#include "SideDialogManager.h"

namespace
{
	#define dMAX_ALPHA					255.f
	#define dFADE_OUT_PER_SECOND		200.f	///< 전투력 측정 배경화면의 초당 Fade Out 되는 정도

	#define dSTR_SCOUTER_BACKGROUND		"scouter_screen.swf"
	#define dSTR_SCOUTER_BROKEN			"scouter_broken.swf"

	#define dFLAG_USING_BACKGORUND		0x00000001		///< 배경
	#define dFLAG_MEASURE_POWER			0x00000002		///< 전투력 측정
	#define dFLAG_OUT_OF_ORDER			0x00000004		///< 스카우터 고장
	#define dFLAG_FADE_OUT				0x00000010		///< Fade out
};


CScouterBackgroundGui::CScouterBackgroundGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_uiFlag(0)
,m_fAlpha(dMAX_ALPHA)
{

}

CScouterBackgroundGui::~CScouterBackgroundGui()
{

}

RwBool CScouterBackgroundGui::Create()
{
	NTL_FUNCTION( "CScouterBackgroundGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\ScouterBackground.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_SCOUTER_BACKGROUND);

	// 측정시의 배경
	m_pFlashBackground = (gui::CFlash*)GetComponent("flaBackground");
	m_pFlashBackground->SetFrameCallback(TRUE);

	Enable(false);

	// sig	
	m_slotMove			= m_pThis->SigResize().Connect( this, &CScouterBackgroundGui::OnMove );
	m_slotResize		= m_pThis->SigResize().Connect( this, &CScouterBackgroundGui::OnResize );
	m_slotMovieEnd		= m_pFlashBackground->SigMovieEnd().Connect( this, &CScouterBackgroundGui::OnMovieScheduleEnd );

	GetNtlGuiManager()->AddUpdateFunc( this );

	LinkMsg(g_EventScouter);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CScouterBackgroundGui::Destroy()
{
	NTL_FUNCTION("CScouterBackgroundGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	m_pFlashBackground->Unload();

	UnLinkMsg(g_EventScouter);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CScouterBackgroundGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	if( BIT_FLAG_TEST(m_uiFlag, dFLAG_USING_BACKGORUND) == false )
		return;


	m_pFlashBackground->Update(fElapsed);

	if( BIT_FLAG_TEST(m_uiFlag, dFLAG_OUT_OF_ORDER) == false )
	{
		if( BIT_FLAG_TEST(m_uiFlag, dFLAG_FADE_OUT) == false )
			return;

		m_fAlpha -= dFADE_OUT_PER_SECOND * fElapsed;
		m_pFlashBackground->SetAlpha((RwUInt8)m_fAlpha);	

		if( m_fAlpha <= 0 )
			GetDialogManager()->CloseDialog(DIALOG_SCOUTER_BACKGROUND);
	}
}

VOID CScouterBackgroundGui::LoadFlash(const char* pcText)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	
	m_pFlashBackground->Load(pcText);
	m_pFlashBackground->SetPosition(rtScreen);	
	m_pFlashBackground->PlayMovie(TRUE);
}

VOID CScouterBackgroundGui::LocateComponent()
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	m_pFlashBackground->SetPosition(rtScreen);
}

VOID CScouterBackgroundGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	LocateComponent();
}

VOID CScouterBackgroundGui::OnResize( RwInt32 iOldW, RwInt32 iOldH )
{
	LocateComponent();
}

VOID CScouterBackgroundGui::OnMovieScheduleEnd(gui::CComponent* pComponent)
{
	if( BIT_FLAG_TEST(m_uiFlag, dFLAG_OUT_OF_ORDER) )
	{
		CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_EXIT_AFTER_OUT_OF_ORDER);
	}
	else
	{
		GetDialogManager()->CloseDialog(DIALOG_SCOUTER_BACKGROUND);
	}
}

RwInt32 CScouterBackgroundGui::SwitchDialog(bool bOpen)
{
	if(bOpen)
	{
		// 스카우터 정보
		CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
		RwUInt32 uiSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);

		// 스카우터가 없으면 메뉴가 열리지 않는다
		if( uiSerial == INVALID_SERIAL_ID )
			return -1;

		CNtlSobItem* pScouterItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(uiSerial));
		if(!pScouterItem)
			return -1;

		CNtlSobItemAttr* pScouterItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pScouterItem->GetSobAttr());

		if(pScouterItemAttr->GetDur() <= 1)
		{
			LoadFlash(dSTR_SCOUTER_BROKEN);
			BIT_FLAG_SET(m_uiFlag, dFLAG_FADE_OUT | dFLAG_OUT_OF_ORDER);
		}
		else
			LoadFlash(dSTR_SCOUTER_BACKGROUND);


		m_fAlpha = dMAX_ALPHA;		
		m_pFlashBackground->SetAlpha((RwUInt8)m_fAlpha);
		BIT_FLAG_SET(m_uiFlag, dFLAG_USING_BACKGORUND);

        //CNtlSLEventGenerator::CameraFPS(FALSE);
		CDboEventGenerator::OpenScouterBackgroundGui();

		Show(true);
	}
	else
	{
		CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
		RwUInt32 uiSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);

		if( uiSerial == INVALID_SERIAL_ID )
			return -1;

		m_uiFlag = 0;

		m_pFlashBackground->Unload();

		Show(false);		
	}	

	return 1;
}

VOID CScouterBackgroundGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION( "CScouterBackgroundGui::HandleEvents" );

	if(msg.Id == g_EventScouter)
	{
		SDboEventScouter* pEvent = reinterpret_cast<SDboEventScouter*>( msg.pData );

		switch(pEvent->iType)
		{
		case SCOUTER_EVENT_OUT_OF_ORDER:
			{
				// 스카우터 이용중 고장났다
				BIT_FLAG_SET(m_uiFlag, dFLAG_OUT_OF_ORDER | dFLAG_FADE_OUT | dFLAG_USING_BACKGORUND);
				LoadFlash(dSTR_SCOUTER_BROKEN);
			}
		case SCOUTER_EVENT_MEASURE_POWER_START:
			{
				SERIAL_HANDLE hTarget = GetDboGlobal()->GetTargetSerial();
				if( hTarget != INVALID_SERIAL_ID )
					BIT_FLAG_SET(m_uiFlag, dFLAG_MEASURE_POWER);

				break;
			}
        case SCOUTER_EVENT_ENTER:         // FSM으로 부터 날라온 메시지
            {
                GetDialogManager()->OpenDialog(DIALOG_SCOUTER_BACKGROUND);
                break;
            }
        case SCOUTER_EVENT_EXIT:
            {
				if( IsShow() )
					BIT_FLAG_SET(m_uiFlag, dFLAG_FADE_OUT);
                break;
            }
		}
	}

	NTL_RETURNVOID();
}