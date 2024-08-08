#include "precomp_dboclient.h"
#include "ScouterMeasureGui.h"

// core
#include "math.h"
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"
// table
#include "ItemTable.h"
#include "MobTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSob.h"
#include "NtlSobProxy.h"
#include "NtlSLEventFunc.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobPetAttr.h"
#include "NtlSobGroup.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"

// dbo
#include "DboPacketGenerator.h"
#include "GuiFunctor.h"
#include "DisplayStringManager.h"
#include "ChatGui.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"

// test
#include "NtlCameraManager.h"

#define dXPOS_ADJUST		8

// Edge effect
#define dTARGET_EDGE_THICKNESS_ORIGINAL			1.f
#define dTARGET_EDGE_THICKNESS_EFFECT			4.f
#define dTARGET_EDGE_THICKNESS_ORIGINAL_RED		0
#define dTARGET_EDGE_THICKNESS_ORIGINAL_GREEN	0
#define dTARGET_EDGE_THICKNESS_ORIGINAL_BLUE	0
#define dTARGET_EDGE_THICKNESS_EFFECT_RED		50
#define dTARGET_EDGE_THICKNESS_EFFECT_GREEN		50
#define dTARGET_EDGE_THICKNESS_EFFECT_BLUE		255

// GUI Flag
#define dDISPLAY_POWER			0x00000001	///< 전투력 표시
#define dDISPLAY_ON				0x01000000	///< GUI 표시

// flash resource
#define dSTR_POWER_SCAN			"scouter_scan.swf"
#define dSTR_TARGET_CHECK		"scouter_target_check.swf"
#define dSTR_TARGET_ARROW		"scouter_arrow.swf"
#define dSTR_POWER_DIRECT		"scouter_target_direct.swf"
#define dSTR_AVATAR_POWER_DIRECT	"scouter_pc_direct.swf"

#define dCHECK_WIDTH			800
#define dCHECK_HEIGHT			800

#define dCIRCLE_SIZE			100

// time schudle
#define dTIME_DISPLAY_POWER				4.f	///< 전투력을 보여주는 시간

#define dTIME_UPDATE_POWER				1.f	///< 전투력을 업데이트하고 서버로 다시 요청하는 시간
#define dTIME_UPDATE_DELAY_POWER		0.1f///< 표시되는 전투력이 변하는 시간 간격

#define dTIME_LIMIT						10.f///< 제한된 시간을 넘어서면 전투력 측정을 강제로 종료한다

#define dTIME_REMAIN_VISIBLE_HIDDEN_OBJECT		30.f	///< 스카우터를 끄고 나서도 히든 오브젝트를 보여주는 시간

// etc
#define	dNORMAL_POWER_MEASURE_COUNT		2	///< 포커스를 맞추기까지 갱신되는 전투력
#define dMAX_POWER_MEASURE_COUNT		8	///< 측정되는 대상의 전투력이 계속 올라갈 때 최대 측정횟수


CScouterMeasureGui::CScouterMeasureGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_bOutofOrder(FALSE)
{

}

CScouterMeasureGui::~CScouterMeasureGui()
{

	NTL_FUNCTION("CScouterMeasureGui::~CScouterMeasureGui");

	NTL_RETURNVOID();
}

RwBool CScouterMeasureGui::Create()
{
	NTL_FUNCTION( "CScouterMeasureGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\ScouterMeasure.srf", "gui\\ScouterMeasure.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_SCOUTER_MEASURE);

	Enable(false);

	CRectangle rect;	

	// 전투력을 표시하는 숫자(빨강)
	m_Number[NUMBERCOLOR_RED].Create();
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureRed_0" ));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureRed_1" ));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureRed_2" ));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureRed_3" ));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureRed_4" ));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureRed_5" ));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureRed_6" ));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureRed_7" ));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureRed_8" ));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureRed_9" ));
	m_Number[NUMBERCOLOR_RED].SetNumGap(-5);

	// 전투력을 표시하는 숫자(노랑)
	m_Number[NUMBERCOLOR_YELLOW].Create();
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureYellow_0" ));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureYellow_1" ));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureYellow_2" ));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureYellow_3" ));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureYellow_4" ));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureYellow_5" ));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureYellow_6" ));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureYellow_7" ));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureYellow_8" ));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureYellow_9" ));
	m_Number[NUMBERCOLOR_YELLOW].SetNumGap(-5);

	// 전투력을 표시하는 숫자(흰색)
	m_Number[NUMBERCOLOR_WHITE].Create();
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureWhite_0" ));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureWhite_1" ));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureWhite_2" ));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureWhite_3" ));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureWhite_4" ));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureWhite_5" ));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureWhite_6" ));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureWhite_7" ));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureWhite_8" ));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterMeasureWhite_9" ));
	m_Number[NUMBERCOLOR_WHITE].SetNumGap(-5);

	//HP 다이얼로그를 가리키는 플래시
	m_AvatarPower.pflaAvatarDirect	= (gui::CFlash*)GetComponent("flaAvatarPower");
	m_AvatarPower.slotOnMoiveEnd	= m_AvatarPower.pflaAvatarDirect->SigMovieEnd().Connect( this, &CScouterMeasureGui::OnMovieEnd_AvatarDirect );
	m_AvatarPower.bShow				= FALSE;
	m_AvatarPower.iMeasurePowerCount = 0;

	// 자기 전투력
	m_AvatarPower.NumberAvatarPower.Create();
	m_AvatarPower.NumberAvatarPower.SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_0" ));
	m_AvatarPower.NumberAvatarPower.SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_1" ));
	m_AvatarPower.NumberAvatarPower.SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_2" ));
	m_AvatarPower.NumberAvatarPower.SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_3" ));
	m_AvatarPower.NumberAvatarPower.SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_4" ));
	m_AvatarPower.NumberAvatarPower.SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_5" ));
	m_AvatarPower.NumberAvatarPower.SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_6" ));
	m_AvatarPower.NumberAvatarPower.SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_7" ));
	m_AvatarPower.NumberAvatarPower.SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_8" ));
	m_AvatarPower.NumberAvatarPower.SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_9" ));
	m_AvatarPower.NumberAvatarPower.SetHoriDirection(ND_RIGHT);
	m_AvatarPower.NumberAvatarPower.SetNumGap(-5);
	m_AvatarPower.NumberAvatarPower.SetPosition(200, 150);

	// 거리 표시
	m_NumberDistance.Create();
	m_NumberDistance.SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_0" ));
	m_NumberDistance.SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_1" ));
	m_NumberDistance.SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_2" ));
	m_NumberDistance.SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_3" ));
	m_NumberDistance.SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_4" ));
	m_NumberDistance.SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_5" ));
	m_NumberDistance.SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_6" ));
	m_NumberDistance.SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_7" ));
	m_NumberDistance.SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_8" ));
	m_NumberDistance.SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterAvatarPower_9" ));
	m_NumberDistance.SetNumGap(-5);
	m_NumberDistance.SetRate(.8f);

	m_srfDistanceMark.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterDistanceMark" ));
	m_srfDistanceUnit.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfScouterDistanceUnit" ));

	// default value
	m_HiddenObject.bRemainVisible	= FALSE;
	m_HiddenObject.fElapsed			= 0.f;

	GetNtlGuiManager()->AddUpdateFunc( this );

	// Sig
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CScouterMeasureGui::OnPaint );

	LinkMsg(g_EventScouter);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventTrigObjScouterDir_CreateSTObj);	
	LinkMsg(g_EventTrigObjScouterDir_DeleteSTObj);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CScouterMeasureGui::Destroy()
{
	NTL_FUNCTION("CScouterMeasureGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	m_HiddenObject.mapObjectPos.clear();

	Clear();	

	UnLinkMsg(g_EventScouter);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventTrigObjScouterDir_CreateSTObj);
	UnLinkMsg(g_EventTrigObjScouterDir_DeleteSTObj);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CScouterMeasureGui::Clear()
{
	MEASURE_ITER it = m_mapMeasure.begin();
	for( ; it != m_mapMeasure.end() ; ++it )
	{
		sTarget* pTarget = it->second;
		(this->*(pTarget->pCallback_Destory))(pTarget);
	}
	m_mapMeasure.clear();

	m_listRemove.clear();

	m_AvatarPower.bShow = FALSE;
	m_AvatarPower.pflaAvatarDirect->PlayMovie(false);
	m_AvatarPower.pflaAvatarDirect->Unload();
	m_AvatarPower.iMeasurePowerCount = 0;	
}

VOID CScouterMeasureGui::StartMeasure(eMeasureType eMode)
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	RwUInt32 uiSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);

	if( uiSerial == INVALID_SERIAL_ID )
		return;

	CNtlSobItem* pScouterItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(uiSerial));
	CNtlSobItemAttr* pScouterItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pScouterItem->GetSobAttr());

	if(pScouterItemAttr->GetDur() <= 1)
		return;


	switch(eMode)
	{
	case MEASURE_POWER:
		{
			if( MeasurePower_Init() == FALSE )
				return;

			break;
		}
	case MEASURE_FIND_HIDDEN_OBEJCT:
		{
			if( MeasureFindObject_Init() == FALSE )
				return;

			break;
		}
	default:
		{
			NTL_ASSERT(false, "CScouterMeasureGui::StartMeasure, Invalid measure mode : " << eMode);
			break;
		}
	}

	GetDialogManager()->OpenDialog(DIALOG_SCOUTER_MEASURE);;
}

VOID CScouterMeasureGui::Update(RwReal fElapsed)
{
	if( m_HiddenObject.bRemainVisible )
	{
		// 스카우터가 꺼지고 일정시간 뒤에 오브젝트가 사라진다
		m_HiddenObject.fElapsed -= fElapsed;

		if( m_HiddenObject.fElapsed < 0.f )
		{
			m_HiddenObject.bRemainVisible = FALSE;

			MAP_OBJECTPOS_ITER it = m_HiddenObject.mapObjectPos.begin();
			for( ; it != m_HiddenObject.mapObjectPos.end() ; ++it )
			{
				CDboEventGenerator::MapEvent(MMT_DISAPPEAR_HIDDEN_OBJECT, it->first);

				// 히든 오브젝트의 경우 사라질 때 별도의 이벤트가 없으니 주의!
				CNtlSob* pSob = GetNtlSobManager()->GetSobObject(it->first);
				if( !pSob )
					continue;

				CNtlSobTriggerObject* pSobTriggerObject = reinterpret_cast<CNtlSobTriggerObject*>( pSob );
				pSobTriggerObject->ShowTOForSCT(FALSE);				
			}

			m_HiddenObject.mapObjectPos.clear();
		}
	}


	if( IsShow() == FALSE )
		return;

	// 측정이 끝난 것은 삭제
	// 이벤트로 Update 이전에 측정이 중지될 수 있기에 삭제되는 목록을 먼저 검사한다
	std::list<sTarget*>::iterator it_remove = m_listRemove.begin();
	for( ; it_remove != m_listRemove.end() ; ++it_remove )
	{
		sTarget* pTarget = *it_remove;
		m_mapMeasure.erase(pTarget->hTarget);
		(this->*(pTarget->pCallback_Destory))(pTarget);		
	}
	m_listRemove.clear();


	// 아바타 전투력
	CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>( GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr() );
	m_AvatarPower.uiPower = pAttr->GetAP();

	if( m_AvatarPower.bShow )
	{
		if( m_AvatarPower.uiPower != (RwUInt32)m_AvatarPower.NumberAvatarPower.GetNumber() )
			m_AvatarPower.NumberAvatarPower.SetNumber(m_AvatarPower.uiPower);
	}


	// 모드별 업데이트
	MEASURE_ITER it = m_mapMeasure.begin();
	for( ; it != m_mapMeasure.end() ; ++it )
	{
		sTarget* pTarget = it->second;
		(this->*(pTarget->pCallback_Update))(fElapsed, pTarget);
	}

	m_AvatarPower.pflaAvatarDirect->Update(fElapsed);
}

RwBool CScouterMeasureGui::MeasurePower_Init()
{
	SERIAL_HANDLE hTarget = GetDboGlobal()->GetTargetSerial();
	if( hTarget == INVALID_SERIAL_ID )
	{
		// 타켓이 없습니다
		GetAlarmManager()->AlarmMessage( DST_TARGET_NONE );
		return FALSE;
	}

/* 기획이 변경되었다 2008. 3.
	if( Logic_GetAvatarHandle() == hTarget )
	{
		// 자기 자신을 측정할 수 없습니다
		GetAlarmManager()->AlarmMessage( DST_TARGET_CANNOT_TARGETING_FOR_SCOUTER );
		return FALSE;
	}
*/
	// 이미 측정중인 대상이다
	MEASURE_ITER it = m_mapMeasure.find(hTarget);
	if( it != m_mapMeasure.end() )
		return FALSE;

	CNtlSob* pSob = GetNtlSobManager()->GetSobObject(hTarget);
	NTL_ASSERT(pSob, "CScouterMeasureGui::MeasurePower_Init, Not exist target of handle " << hTarget);
	if( pSob->GetClassID() == SLCLASS_AVATAR ||	pSob->GetClassID() == SLCLASS_PLAYER ||
		pSob->GetClassID() == SLCLASS_NPC	 ||	pSob->GetClassID() == SLCLASS_MONSTER ||
		pSob->GetClassID() == SLCLASS_PET )
	{
		sPowerData* pPowerData = NTL_NEW sPowerData;
        m_mapMeasure[hTarget] = pPowerData;

		pPowerData->eType	= MEASURE_POWER;

		pPowerData->hTarget = hTarget;
		pPowerData->pSobTarget = pSob;

		// 플래쉬 두번째 애니메이션
		pPowerData->pFlashCircle = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		pPowerData->pFlashCircle->SetFrameCallback(FALSE);
		pPowerData->pFlashCircle->Load(dSTR_TARGET_CHECK);
		pPowerData->pFlashCircle->PlayMovie(TRUE);

		// 플래쉬 화살표
		pPowerData->pFlashArrow = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		pPowerData->pFlashArrow->SetFrameCallback(FALSE);
		pPowerData->pFlashArrow->Load(dSTR_TARGET_ARROW);
		pPowerData->pFlashArrow->PlayMovie(TRUE);

		// 플래쉬 화살표
		pPowerData->pFlashDirect = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		pPowerData->pFlashDirect->SetFrameCallback(FALSE);
		pPowerData->pFlashDirect->Load(dSTR_POWER_DIRECT);
		pPowerData->pFlashDirect->PlayMovie(TRUE);

		Logic_PlayGUISound(GSD_SYSTEM_SCOUTER_SCAN);

		pPowerData->pCallback_Update	= &CScouterMeasureGui::MeasurePower_Update;
		pPowerData->pCallback_Destory	= &CScouterMeasureGui::MeasurePower_Destroy;
		pPowerData->pCallback_Paint		= &CScouterMeasureGui::MeasurePower_Paint;

		++m_AvatarPower.iMeasurePowerCount;

		MeasurePower_AskPowertoServer(pPowerData);

		return TRUE;
	}
	
	return FALSE;
}

VOID CScouterMeasureGui::MeasurePower_Destroy(sTarget* pTarget)
{
	sPowerData* pPowerData = reinterpret_cast<sPowerData*>( pTarget );

	--m_AvatarPower.iMeasurePowerCount;

	if( m_AvatarPower.iMeasurePowerCount == 0 )
	{
		m_AvatarPower.bShow = FALSE;
		m_AvatarPower.pflaAvatarDirect->PlayMovie(false);
		m_AvatarPower.pflaAvatarDirect->Unload();
	}

	// 테두리 감추기
	TargetEdgeEffect(FALSE, pTarget);

	pPowerData->pFlashCircle->Unload();
	pPowerData->pFlashArrow->Unload();
	pPowerData->pFlashDirect->Unload();

	NTL_DELETE(pPowerData->pFlashCircle);
	NTL_DELETE(pPowerData->pFlashArrow);
	NTL_DELETE(pPowerData->pFlashDirect);

	NTL_DELETE(pPowerData);
}

VOID CScouterMeasureGui::MeasurePower_Paint(sTarget* pTarget)
{
	sPowerData* pPowerData = reinterpret_cast<sPowerData*>( pTarget );
	CPos focusPos;
	RwV3d v3Pos;
	RwReal fTargetHeight;
	CRectangle rect;
	RwUInt8 byNumberColor;
	RwReal fPowerRate = (RwReal)pPowerData->uiDisplayPower / (RwReal)m_AvatarPower.uiPower;

	v3Pos = pPowerData->pSobTarget->GetPosition();	
	fTargetHeight = pPowerData->pSobTarget->GetSobProxy()->GetPLEntityHeight();		

	if( fPowerRate >= 1.6f )
	{
		byNumberColor = NUMBERCOLOR_RED;
	}
	else if( fPowerRate >= 1.3f )
	{
		byNumberColor = NUMBERCOLOR_YELLOW;
	}
	else
	{
		byNumberColor = NUMBERCOLOR_WHITE;
	}

	if( GetGuiFuntor()->Calc3DPosTo2D(v3Pos, fTargetHeight/2, NULL, &focusPos) )
	{
		pPowerData->pFlashCircle->Show(true);
		pPowerData->pFlashArrow->Show(false);
		pPowerData->pFlashDirect->Show(true);

		rect.left	= focusPos.x - dCHECK_WIDTH/2;
		rect.top	= focusPos.y - dCHECK_HEIGHT/2;
		rect.right	= focusPos.x + dCHECK_WIDTH/2;
		rect.bottom	= focusPos.y + dCHECK_HEIGHT/2;

		pPowerData->pFlashCircle->SetPosition(rect);
		pPowerData->pFlashDirect->SetPosition(rect);

		// 전투력 표시
		if( BIT_FLAG_TEST(pPowerData->dwDisplayFlag, dDISPLAY_POWER) )
		{
			RwInt32 iPosX, iPosY;
			CRectangle rtRect;
			RwV3d v3Distance;

			m_Number[byNumberColor].SetNumber(pPowerData->uiDisplayPower);

			// adjust
			iPosX = focusPos.x - dCIRCLE_SIZE - 20;
			iPosY = focusPos.y - 115;

			// 전투력
			m_Number[byNumberColor].SetPosition(iPosX, iPosY);
			m_Number[byNumberColor].Render();

			// 'M' 표시
			rtRect = m_Number[byNumberColor].GetScreenRect();
			iPosX = rtRect.right;
			iPosY = rtRect.bottom + 5;
			m_srfDistanceUnit.SetPosition(iPosX, iPosY);
			m_srfDistanceUnit.Render();
			
			// 거리
			m_srfDistanceUnit.GetRect(rtRect);			
            v3Distance = pPowerData->pSobTarget->GetPosition() - GetNtlSLGlobal()->GetSobAvatar()->GetPosition();

			m_NumberDistance.SetNumber((RwUInt32)RwV3dLength(&v3Distance));
			m_NumberDistance.SetPosition(rtRect.left - 5, iPosY);
			m_NumberDistance.Render();

			// 거리 마크
			rtRect = m_NumberDistance.GetScreenRect();
			m_srfDistanceMark.SetPosition(rtRect.left - m_srfDistanceMark.GetWidth() - 5, iPosY);
			m_srfDistanceMark.Render();
		}
	}
	else
	{
		pPowerData->pFlashCircle->Show(false);
		pPowerData->pFlashArrow->Show(true);
		pPowerData->pFlashDirect->Show(false);

		return;

		//const RwV3d* pV3CameraDir = GetNtlGameCameraManager()->GetCameraDir();
		//RwV3d v3TargetDir;
		//RwV3dSub(&v3TargetDir, &v3Pos, &GetNtlSLGlobal()->GetSobAvatar()->GetPosition());
		//RwV2d v2TargetDir, v2CameraDir;

		//v2CameraDir.x = pV3CameraDir->x;
		//v2CameraDir.y = pV3CameraDir->z;

		//v2TargetDir.x = v3TargetDir.x;
		//v2TargetDir.y = v3TargetDir.z;

		//RwV2dNormalize(&v2TargetDir, &v2TargetDir);
		//RwReal fResult = RwV2dDotProduct(&v2CameraDir, &v2TargetDir);

		//rect.left	= 200;
		//rect.top	= 200;
		//rect.right	= 400;
		//rect.bottom	= 400;

		//if( pPowerData->pFlashArrow )
		//	pPowerData->pFlashArrow->SetPosition(rect);

		//m_Number[byNumberColor].SetNumber(pPowerData->uiDisplayPower);

		//m_Number[byNumberColor].SetPosition(400, 400);
		//m_Number[byNumberColor].Render();
	}
}

VOID CScouterMeasureGui::MeasurePower_Update(RwReal fElapsed, sTarget* pTarget)
{
	sPowerData* pPowerData = reinterpret_cast<sPowerData*>( pTarget );

	pPowerData->pFlashCircle->Update(fElapsed);
	pPowerData->pFlashArrow->Update(fElapsed);
	pPowerData->pFlashDirect->Update(fElapsed);

	pPowerData->fTimer += fElapsed;

	if( dTIME_LIMIT <= pPowerData->fTimer )
		EndDisplay(pPowerData);

	pPowerData->dwDisplayFlag = dDISPLAY_ON;

	if( m_bOutofOrder )
	{
		pPowerData->dwDisplayFlag |= dDISPLAY_POWER;

		if( pPowerData->fTimer >= pPowerData->fTimeRecievePower )
		{
			// 추가 전투력 측정시 종료
			EndDisplay(pPowerData);
			return;
		}
		else
		{
			return;
		}
	}	


	if( pPowerData->bShowScouterBack == FALSE)
		pPowerData->fTimeDisplayPower += fElapsed;

	if( pPowerData->uiMoreMeasureCount < dMAX_POWER_MEASURE_COUNT && m_bOutofOrder == FALSE )
	{
		if( pPowerData->fTimeAdjustPower <= pPowerData->fTimer )
		{
			// 새로 받은 전투력까지 클라이언트 자체내에서 전투력을 보간한다.
			RwUInt32 uiDisplayPower = pPowerData->uiPostPower + pPowerData->iAdjustCount*pPowerData->uiAdjustPower;

			++pPowerData->iAdjustCount;

			if( uiDisplayPower > pPowerData->uiOffencePower )
				uiDisplayPower = pPowerData->uiOffencePower;

			pPowerData->uiDisplayPower = uiDisplayPower;
		}		

		if( pPowerData->uiMoreMeasureCount >= dNORMAL_POWER_MEASURE_COUNT )
		{
			pPowerData->dwDisplayFlag |= dDISPLAY_POWER;

			// 스카우터 배경 종료
			// 이후에도 전투력은 한동안 표시된다
			if( pPowerData->bShowScouterBack &&
				pPowerData->uiOffencePower == pPowerData->uiPostPower )
			{
				pPowerData->bShowScouterBack = FALSE;
			}
			else if( pPowerData->fTimeDisplayPower >= dTIME_DISPLAY_POWER )
			{
				// 일반 전투력 측정시 종료
				EndDisplay(pPowerData);
				return;
			}
		}	
	}	


	if( pPowerData->bAskPower == FALSE &&
		dTIME_UPDATE_POWER <= pPowerData->fTimer - pPowerData->fTimeRecievePower )
	{		
		if( pPowerData->uiMoreMeasureCount < dNORMAL_POWER_MEASURE_COUNT )
		{
			// 서버로부터 전투력을 받고 일정 시간후 전투력을 서버에 요청한다
			MeasurePower_AskPowertoServer(pPowerData);
		}
		else if( pPowerData->uiOffencePower > pPowerData->uiPostPower )
		{
			// 마지막 전투력이 이전보다 더 높다면 다시 전투력을 요청한다(상대방이 기를 모으거나 할때)
			MeasurePower_AskPowertoServer(pPowerData);
		}
	}
}

VOID CScouterMeasureGui::MeasurePower_Display(sPowerData* pPowerData, RwUInt32 uiPower)
{
	pPowerData->uiMoreMeasureCount += 1;
	pPowerData->fTimeRecievePower = pPowerData->fTimer;	
	pPowerData->uiPostPower = pPowerData->uiOffencePower;
	pPowerData->uiOffencePower = uiPower;
	pPowerData->iAdjustCount = 0;
	pPowerData->bAskPower = FALSE;	

	if( uiPower > pPowerData->uiPostPower )
	{
		// 새로 받은 전투력까지 클라이언트 자체내에서 전투력을 보간한다.
		pPowerData->uiAdjustPower = (RwUInt32)(((RwReal)(pPowerData->uiOffencePower - pPowerData->uiPostPower))*dTIME_UPDATE_DELAY_POWER/dTIME_UPDATE_POWER);
		pPowerData->uiDisplayPower = pPowerData->uiPostPower + pPowerData->uiAdjustPower;
	}
	else 
	{
		if( pPowerData->uiMoreMeasureCount >= dNORMAL_POWER_MEASURE_COUNT &&
			pPowerData->uiOffencePower > pPowerData->uiPostPower )
		{
			// 최종 전투력
			pPowerData->uiDisplayPower = pPowerData->uiOffencePower;
		}	

		// 연출이 끝나면 현재 타겟스테이터스를 리프레쉬, (현재는 몹만..)
		CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( pPowerData->hTarget );

		if( pSobObj->GetClassID() == SLCLASS_MONSTER )
		{			
			CNtlSobMonsterAttr* pMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( pSobObj->GetSobAttr() );
			sMOB_TBLDAT* pMobData = pMonsterAttr->GetMobTbl();
			RwBool bHaveScanData = FALSE;

			CNtlStorageGroupScouter* pGroupScouter = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_SCOUTER );

			if( pGroupScouter->IsUseMonsterScouter(pMobData->dwMobGroup) ||	pMobData->bShow_Name )
				bHaveScanData = TRUE;


			pGroupScouter->UseMonsterScouter( pMobData->dwMobGroup, pPowerData->uiOffencePower );
			CDboEventGenerator::TargetStatusRefresh();


			// 처음으로 측정을 하는 몬스터 그룹이라면 주변의 같은 그룹의 몬스터 이름을 바로 보여준다
			if( bHaveScanData == FALSE )
			{
				CNtlSobGroup::MapObject::iterator it;
				CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup( SLCLASS_MONSTER );

                CNtlSobMonsterAttr* pTargetMonsterAttr = NULL;
				for( it = pSobGroup->GetObjects().begin() ; it != pSobGroup->GetObjects().end() ; ++it )
				{
					CNtlSobMonster* pMonster = reinterpret_cast<CNtlSobMonster*>(it->second);
					pTargetMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( pMonster->GetSobAttr() );
					sMOB_TBLDAT* pMOB_TBLDAT = pTargetMonsterAttr->GetMobTbl();

					if( pMobData->dwMobGroup == pMOB_TBLDAT->dwMobGroup )
						Logic_SetHeadNameColor(it->second);
				}
			}
		}		
		else if( pSobObj->GetClassID() == SLCLASS_PLAYER )
		{
			CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>( pSobObj );

			CNtlStorageGroupScouter* pGroupScouter = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_SCOUTER );
			pGroupScouter->UsePlayerScouter( pPlayer->GetCharID(), pPowerData->uiOffencePower );
			CDboEventGenerator::TargetStatusRefresh();
		}
	}
}

VOID CScouterMeasureGui::MeasurePower_AskPowertoServer(sPowerData* pPowerData)
{
	// 서버에 갱신 요청을 한다.
	GetDboGlobal()->GetGamePacketGenerator()->SendScouterMeasure(pPowerData->hTarget);
	pPowerData->bAskPower = TRUE;
}

RwBool CScouterMeasureGui::MeasureFindObject_Init()
{
	if( IsWorking(MEASURE_FIND_HIDDEN_OBEJCT) )
		return FALSE;

	MAP_OBJECTPOS_ITER it = m_HiddenObject.mapObjectPos.begin();

	for( ; it != m_HiddenObject.mapObjectPos.end() ; ++it )
	{		
		sHiddenObjectData* pHiddenObject = NTL_NEW sHiddenObjectData();
        m_mapMeasure[it->first] = pHiddenObject;

		pHiddenObject->eType	= MEASURE_FIND_HIDDEN_OBEJCT;

		pHiddenObject->hTarget	= it->first;
		pHiddenObject->pSobTarget = GetNtlSobManager()->GetSobObject(pHiddenObject->hTarget);

		CNtlSobTriggerObject* pSobTriggerObject = reinterpret_cast<CNtlSobTriggerObject*>( pHiddenObject->pSobTarget );
		pSobTriggerObject->ShowTOForSCT(true);

		CDboEventGenerator::MapEvent(MMT_APPEAR_HIDDEN_OBJECT, pHiddenObject->hTarget);

		pHiddenObject->dwDisplayFlag = dDISPLAY_ON;

		// 플래쉬
		pHiddenObject->pFlashCircle = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		pHiddenObject->pFlashCircle->SetFrameCallback(FALSE);
		pHiddenObject->pFlashCircle->Load(dSTR_TARGET_CHECK);
		pHiddenObject->pFlashCircle->PlayMovie(TRUE);

		// 플래쉬 화살표
		pHiddenObject->pFlashArrow = NTL_NEW gui::CFlash(m_pThis, GetNtlGuiManager()->GetSurfaceManager());
		pHiddenObject->pFlashArrow->SetFrameCallback(FALSE);
		pHiddenObject->pFlashArrow->Load(dSTR_TARGET_ARROW);
		pHiddenObject->pFlashArrow->PlayMovie(TRUE);

		TargetEdgeEffect(TRUE, pHiddenObject);

		pHiddenObject->pCallback_Update		= &CScouterMeasureGui::MeasureFindObject_Update;
		pHiddenObject->pCallback_Destory	= &CScouterMeasureGui::MeasureFindObject_Destroy;
		pHiddenObject->pCallback_Paint		= &CScouterMeasureGui::MeasureFindObject_Paint;
	}	

	RwInt32 iHiddenObjectCount = m_HiddenObject.mapObjectPos.size();
	if( iHiddenObjectCount > 0 )
	{
		GetAlarmManager()->FormattedAlarmMessage(DST_SCOUTER_SUCCESS_FIND_HIDDEN_OBJECT, FALSE, NULL, iHiddenObjectCount);
	}
	else
	{
		GetAlarmManager()->AlarmMessage(DST_SCOUTER_NOT_EXIST_HIDDEN_OBJECT);
	}

	return TRUE;
}

VOID CScouterMeasureGui::MeasureFindObject_Destroy(sTarget* pTarget)
{
	sHiddenObjectData* pHiddenObject = reinterpret_cast<sHiddenObjectData*>( pTarget );

	// 테두리 감추기
	TargetEdgeEffect(FALSE, pTarget);

	pHiddenObject->pFlashCircle->Unload();
	pHiddenObject->pFlashArrow->Unload();

	NTL_DELETE(pHiddenObject->pFlashCircle);
	NTL_DELETE(pHiddenObject->pFlashArrow);

	NTL_DELETE(pHiddenObject);
}

VOID CScouterMeasureGui::MeasureFindObject_Update(RwReal fElapsed, sTarget* pTarget)
{
	sHiddenObjectData* pHiddenObjectData = reinterpret_cast<sHiddenObjectData*>( pTarget );

	pHiddenObjectData->pFlashCircle->Update(fElapsed);
}

VOID CScouterMeasureGui::MeasureFindObject_Paint(sTarget* pTarget)
{
	sHiddenObjectData* pHiddenObjectData = reinterpret_cast<sHiddenObjectData*>( pTarget );
	CPos focusPos;
	RwV3d v3Pos;
	RwReal fTargetHeight;
	CRectangle rect;

	v3Pos = pHiddenObjectData->pSobTarget->GetPosition();	
	fTargetHeight = pHiddenObjectData->pSobTarget->GetSobProxy()->GetPLEntityHeight();		

	if( GetGuiFuntor()->Calc3DPosTo2D(v3Pos, fTargetHeight/2, NULL, &focusPos) )
	{
		pHiddenObjectData->pFlashCircle->Show(true);
		pHiddenObjectData->pFlashArrow->Show(false);

		rect.left	= focusPos.x - dCHECK_WIDTH/2;
		rect.top	= focusPos.y - dCHECK_HEIGHT/2;
		rect.right	= focusPos.x + dCHECK_WIDTH/2;
		rect.bottom	= focusPos.y + dCHECK_HEIGHT/2;

		pHiddenObjectData->pFlashCircle->SetPosition(rect);
	}
	else
	{
		pHiddenObjectData->pFlashCircle->Show(false);
		pHiddenObjectData->pFlashArrow->Show(true);

		return;

		//const RwV3d* pV3CameraDir = GetNtlGameCameraManager()->GetCameraDir();
		//RwV3d v3TargetDir = v3Pos - GetNtlSLGlobal()->GetSobAvatar()->GetPosition();		
		//RwV2d v2TargetDir, v2CameraDir;

		//v2CameraDir.x = pV3CameraDir->x;
		//v2CameraDir.y = pV3CameraDir->z;

		//v2TargetDir.x = v3TargetDir.x;
		//v2TargetDir.y = v3TargetDir.z;

		//RwV2dNormalize(&v2TargetDir, &v2TargetDir);
		//RwReal fResult = RwV2dDotProduct(&v2CameraDir, &v2TargetDir);

		//rect.left	= 200;
		//rect.top	= 200;
		//rect.right	= 400;
		//rect.bottom	= 400;

		//if( pHiddenObjectData->pFlashArrow )
		//	pHiddenObjectData->pFlashArrow->SetPosition(rect);
	}
}

VOID CScouterMeasureGui::EndDisplayImmediately(SERIAL_HANDLE hHandle)
{
	m_HiddenObject.mapObjectPos.erase(hHandle);	

	MEASURE_ITER it = m_mapMeasure.find(hHandle);
	if( it != m_mapMeasure.end() )
	{
		sTarget* pTarget = it->second;
		(this->*(pTarget->pCallback_Destory))(pTarget);
		m_mapMeasure.erase(it);
	}

	std::list<sTarget*>::iterator it_remove = m_listRemove.begin();
	for( ; it_remove != m_listRemove.end() ; ++it_remove )
	{
		sTarget* pTarget = *it_remove;
		if( pTarget->hTarget == hHandle )
		{
			m_listRemove.erase(it_remove);
			break;
		}
	}

	CDboEventGenerator::MapEvent(MMT_DISAPPEAR_HIDDEN_OBJECT, hHandle);
}

VOID CScouterMeasureGui::EndDisplay(sTarget* pTarget)
{
	std::list<sTarget*>::iterator it_remove = m_listRemove.begin();
	for( ; it_remove != m_listRemove.end() ; ++it_remove )
	{
		sTarget* pTarget_Remove = *it_remove;
		if( pTarget_Remove == pTarget )
			return;
	}

	m_listRemove.push_back(pTarget);	
}

VOID CScouterMeasureGui::OutOfOrder()
{
	m_bOutofOrder = TRUE;

	MEASURE_ITER it = m_mapMeasure.begin();
	for( ; it != m_mapMeasure.end() ; ++it )
	{
		sTarget* pTarget = it->second;
		switch(pTarget->eType)
		{
		case MEASURE_POWER:
			{
				sPowerData* pPowerData = reinterpret_cast<sPowerData*>( pTarget );
				pPowerData->uiDisplayPower = pPowerData->uiOffencePower;
				break;
			}
		case MEASURE_FIND_HIDDEN_OBEJCT:
			{
				break;
			}
		}
	}		
}

VOID CScouterMeasureGui::OnMovieEnd_AvatarDirect(gui::CComponent* pComponent)
{
	m_AvatarPower.bShow = TRUE;
}

VOID CScouterMeasureGui::TargetEdgeEffect(RwBool bActive, sTarget* pTarget)
{
	// 이미 Sob 객체가 지워진 상태에서 들어오는 경우는 리턴한다
	if( GetNtlSobManager()->GetSobObject(pTarget->hTarget) == NULL )
		return;

	CNtlSobProxy* pSobProxy = pTarget->pSobTarget->GetSobProxy();
	NTL_ASSERT(pSobProxy, "CScouterMeasureGui::TargetEdgeEffect, Not exist sob proxy of handle : " << pTarget->pSobTarget->GetSerialID());

	if( bActive )
	{
		pSobProxy->SetInkColor(dTARGET_EDGE_THICKNESS_EFFECT_RED,
							   dTARGET_EDGE_THICKNESS_EFFECT_GREEN,
							   dTARGET_EDGE_THICKNESS_EFFECT_BLUE);
		pSobProxy->SetInkThicknessWeight(dTARGET_EDGE_THICKNESS_EFFECT);
	}
	else
	{
		pSobProxy->SetInkColor(dTARGET_EDGE_THICKNESS_ORIGINAL_RED,
							   dTARGET_EDGE_THICKNESS_ORIGINAL_GREEN,
							   dTARGET_EDGE_THICKNESS_ORIGINAL_BLUE);
		pSobProxy->SetInkThicknessWeight(dTARGET_EDGE_THICKNESS_ORIGINAL);
	}
}

RwBool CScouterMeasureGui::IsWorking(eMeasureType eType)
{
	MEASURE_ITER it = m_mapMeasure.begin();
	for( ; it != m_mapMeasure.end() ; ++it )
	{
		sTarget* pTarget = reinterpret_cast<sTarget*>( it->second );
		if( pTarget->eType == eType )
			return TRUE;
	}

	return FALSE;
}

VOID CScouterMeasureGui::OnPaint()
{	
	MEASURE_ITER it = m_mapMeasure.begin();
	for( ; it != m_mapMeasure.end() ; ++it )
	{
		sTarget* pTarget = it->second;

		if( BIT_FLAG_TEST(pTarget->dwDisplayFlag, dDISPLAY_ON) == false )
			continue;		

		(this->*(pTarget->pCallback_Paint))(pTarget);		
	}

	if( m_AvatarPower.bShow )
		m_AvatarPower.NumberAvatarPower.Render();
}

RwInt32 CScouterMeasureGui::SwitchDialog(bool bOpen)
{
	m_bOutofOrder = FALSE;

	if( bOpen )
	{
		Show(true);

		m_HiddenObject.bRemainVisible	= FALSE;
		m_HiddenObject.fElapsed			= 0.f;
	}
	else
	{
		Clear();
		Show(false);

		m_HiddenObject.bRemainVisible	= TRUE;
		m_HiddenObject.fElapsed			= dTIME_REMAIN_VISIBLE_HIDDEN_OBJECT;
	}

	return 1;
}

VOID CScouterMeasureGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CScouterMeasureGui::HandleEvents");

	if( msg.Id == g_EventScouter )
	{		
		SDboEventScouter* pEvent = (SDboEventScouter*)msg.pData;
		switch(pEvent->iType)
		{
		case SCOUTER_EVENT_MEASURE_POWER_START:
			{			
				StartMeasure(MEASURE_POWER);

				if( m_AvatarPower.pflaAvatarDirect->IsPlayMovie() == false )
				{
					if( m_AvatarPower.pflaAvatarDirect->Load(dSTR_AVATAR_POWER_DIRECT) == false )
					{
						NTL_ASSERT(false, "CScouterMeasureGui::HandleEvents, SCOUTER_EVENT_MEASURE_POWER_START, Not eixst flash file : " << dSTR_AVATAR_POWER_DIRECT );	
						NTL_RETURNVOID();
					}

					CRectangle rtFlash = m_AvatarPower.pflaAvatarDirect->GetFrameResolution();

					m_AvatarPower.pflaAvatarDirect->SetSize(rtFlash.GetWidth(), rtFlash.GetHeight());
					m_AvatarPower.pflaAvatarDirect->PlayMovie(TRUE);
				}

				break;
			}
		case SCOUTER_EVENT_MEASURE_MEASURE_RESULT:
			{
				MEASURE_ITER it = m_mapMeasure.begin();
				for( ; it != m_mapMeasure.end() ; ++it )
				{
					sPowerData* pPowerData = reinterpret_cast<sPowerData*>( it->second );

					if( pPowerData->hTarget != pEvent->uiUserData )
						continue;

					if( pPowerData->uiMoreMeasureCount == 0)
					{	
						// 튜토리얼을 위해 스카우터를 사용하여 전투력을 측정했음을 알린다
						SERIAL_HANDLE hHandle = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);
						sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromSob(hHandle);
						RwUInt32 uiClassID = pPowerData->pSobTarget->GetClassID();
						RwUInt32 uiTargetTableIndex = 0xffffffff;

						switch(uiClassID)
						{
						case SLCLASS_AVATAR:
							{
								NTL_RETURNVOID();
							}							
						case SLCLASS_PLAYER:
							{
								CNtlSobPlayerAttr* pPlayer = reinterpret_cast<CNtlSobPlayerAttr*>( pPowerData->pSobTarget->GetSobAttr() );
								uiTargetTableIndex = pPlayer->GetPcTbl()->tblidx;
								break;
							}
						case SLCLASS_NPC:
							{
								CNtlSobNpcAttr* pNPC = reinterpret_cast<CNtlSobNpcAttr*>( pPowerData->pSobTarget->GetSobAttr() );
								uiTargetTableIndex = pNPC->GetNpcTbl()->tblidx;
								break;
							}
						case SLCLASS_MONSTER:
							{
								CNtlSobMonsterAttr* pMonster = reinterpret_cast<CNtlSobMonsterAttr*>( pPowerData->pSobTarget->GetSobAttr() );
								uiTargetTableIndex = pMonster->GetMobTbl()->dwMobGroup;
								break;
							}
						case SLCLASS_PET:
							{
								CNtlSobPetAttr* pPet = reinterpret_cast<CNtlSobPetAttr*>( pPowerData->pSobTarget->GetSobAttr() );
								uiTargetTableIndex = pPet->GetNpcTbl()->tblidx;
								break;
							}
						default:
							{
								NTL_ASSERT(false, "CScouterMeasureGui::HandleEvents, g_EventScouter, SCOUTER_EVENT_MEASURE_MEASURE_RESULT, Can not measure target of class ID : " << uiClassID);
								break;
							}
						}

						CNtlSLEventGenerator::TSScoutUse(pITEM_TBLDAT->tblidx, uiClassID, uiTargetTableIndex);
						Logic_SendTutorialCondition(ETL_CONDITION_TYPE_SCOUTER_POWERLEVEL);

						MeasurePower_Display(pPowerData, pEvent->iUserData2);

						// 타겟의 테두리 보여주기
						TargetEdgeEffect(TRUE, pPowerData);
					}
					else
					{
						MeasurePower_Display(pPowerData, pEvent->iUserData2);					
					}
				}				
				
				break;
			}
		case SCOUTER_EVENT_FIND_OBJECT:
			{
				StartMeasure(MEASURE_FIND_HIDDEN_OBEJCT);
				break;
			}
		case SCOUTER_EVENT_MEASURE_ERROR:
			{
				MEASURE_ITER it = m_mapMeasure.begin();
				for( ; it != m_mapMeasure.end() ; ++it )
				{
					sPowerData* pPowerData = reinterpret_cast<sPowerData*>( it->second );

					if( pPowerData->hTarget == pEvent->uiUserData )
					{
						EndDisplay(pPowerData);
					}
				}
				
				break;
			}
		case SCOUTER_EVENT_OUT_OF_ORDER:
			{
				OutOfOrder();
				break;
			}
		case SCOUTER_EVENT_EXIT_AFTER_OUT_OF_ORDER:
			{
				GetDialogManager()->CloseDialog(DIALOG_SCOUTER_MEASURE);
				break;
			}		
		case SCOUTER_EVENT_EXIT:
			{
				GetDialogManager()->CloseDialog(DIALOG_SCOUTER_MEASURE);
				break;
			}
		}		
	}
	else if( msg.Id == g_EventCharObjDelete )
	{
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );
		MEASURE_ITER it = m_mapMeasure.begin();

		for( ; it != m_mapMeasure.end() ; ++it )
		{
			if( it->second->hTarget == *pDeleteSerial )
			{
				EndDisplayImmediately(*pDeleteSerial);
				break;
			}
		}		
	}
	else if(msg.Id == g_EventTrigObjScouterDir_CreateSTObj)
	{
		SNtlEventTrigObjScouterDir_CreateSTObj* pPacket = (SNtlEventTrigObjScouterDir_CreateSTObj*)msg.pData;
		CNtlSob* pSob = GetNtlSobManager()->GetSobObject(pPacket->hSerial);

		m_HiddenObject.mapObjectPos[pPacket->hSerial] = pSob->GetPosition();
	}
	else if(msg.Id == g_EventTrigObjScouterDir_DeleteSTObj)
	{
		SNtlEventTrigObjScouterDir_DeleteSTObj* pPacket = (SNtlEventTrigObjScouterDir_DeleteSTObj*)msg.pData;
		m_HiddenObject.mapObjectPos.erase(pPacket->hSerial);
		EndDisplayImmediately(pPacket->hSerial);
	}

	NTL_RETURNVOID();
}