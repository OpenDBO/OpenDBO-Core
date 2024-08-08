#include "precomp_dboclient.h"
#include "TMQRewardGui.h"

// core
#include "NtlDebug.h"

// share
#include "TimeQuestTable.h"
#include "TableContainer.h"
#include "SkillTable.h"
#include "HTBSetTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEventfunc.h"
#include "NtlSLTMQ.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"

// dbo
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "QuestGuiDef.h"
#include "ResultNarrationGui.h"
#include "CounterGui.h"
#include "DboEventGenerator.h"
#include "AlarmManager.h"
#include "DboGlobal.h"

namespace
{
	// 화면 중앙을 기준점으로 한 좌표
	#define dREWARD_ICON_X		40
	#define dREWARD_ICON_X_GAP	44

	#define dREWARD_ICON_Y		19
	#define dADDREWARD_ICON_Y	66

	#define dSELECTION_REWARD_START_INDEX	4

	// Number Effect
	#define dNUMEFFECT_START_RATE			30.f
	#define dNUMEFFECT_END_RATE				1.f
	#define dNUMEFFECT_TIME					0.1f

	// 각 CNumberGui가 한자리 숫자만을 표시하기에 부득이 여기서 하드코딩으로 숫자간 간격을 조절한다
	#define dNUM_GAP						-2

	// Resule grade : 클리어 시간이 제한 시간에 비해 몇 %단축했는 가에 따라 평가된다
	#define dRESULT_EXCELLENT				.3f
	#define dRESULT_GREATE					.4f
	#define dRESULT_GOOD					.5f
};

CTMQRewardGui* CTMQRewardGui::m_pInstance = NULL;


CTMQRewardGui::CTMQRewardGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pNarrationGui(NULL)
,m_pCounterGui(NULL)
{
}

CTMQRewardGui::~CTMQRewardGui()
{
}

RwBool CTMQRewardGui::Create()
{
	NTL_FUNCTION( "CTMQRewardGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\TMQReward.frm"))
		NTL_RETURN( FALSE  );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	// 제한시간
	rect.SetRectWH(0, 0, 293, 13);
	m_pLimitedTime = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pLimitedTime->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pLimitedTime->SetTextColor(RGB(255, 0, 0));
	m_pLimitedTime->Show(false);
	m_pLimitedTime->Enable(false);

	// 추가 보상 확인 버튼
	m_pConfirmButton = (gui::CButton*)GetComponent( "btnConfirm" );
	m_pConfirmButton->SetText( GetDisplayStringManager()->GetString(DST_LOBBY_OK) );
	m_pConfirmButton->Show(false);
	m_slotConfirmButton = m_pConfirmButton->SigClicked().Connect(this, &CTMQRewardGui::OnConfirmButton);

	// 슬롯 포커스 이펙트
	m_FocusEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );

	// 슬롯 셀렉트 이펙트
	m_SelectEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotFocusEffect" ) );

	// 배경 플래쉬
	m_pFlashBackground		= (gui::CFlash*)GetComponent("flaBackground");
	m_pFlashBackground->SetFrameCallback(TRUE);
	m_pFlashBackground->SetResizeType(gui::CFlash::EXACTFIT);

	// TMQ 결과 플래쉬
	m_pFlashResult			= (gui::CFlash*)GetComponent("flaResult");
	m_pFlashResult->SetFrameCallback(TRUE);	
	m_slotFrameCallbackResult	= m_pFlashResult->SigFSCallBack().Connect( this, &CTMQRewardGui::OnFrameCallback_Result );

	// 캡슐
	m_pFlashCapsule			= (gui::CFlash*)GetComponent("flaCapsule");
	m_slotMovieEndCapsule	= m_pFlashCapsule->SigMovieEnd().Connect( this, &CTMQRewardGui::OnMovieEnd_Capsule );	


	// 보너스 숫자
	for(RwInt8 i = 0 ; i < dTIMER_NUMBER ; ++i)
	{
		m_BonusTimeGui[i].Create(ND_LEFT);
		m_BonusTimeGui[i].SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_0" ));
		m_BonusTimeGui[i].SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_1" ));
		m_BonusTimeGui[i].SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_2" ));
		m_BonusTimeGui[i].SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_3" ));
		m_BonusTimeGui[i].SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_4" ));
		m_BonusTimeGui[i].SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_5" ));
		m_BonusTimeGui[i].SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_6" ));
		m_BonusTimeGui[i].SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_7" ));
		m_BonusTimeGui[i].SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_8" ));
		m_BonusTimeGui[i].SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_9" ));	
		m_BonusTimeGui[i].SetNumGap(-5);
	}

	m_srfBonusNumColon.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_Colon" ) );


	// 최단기록 숫자
	for(RwInt8 i = 0 ; i < dTIMER_NUMBER ; ++i)
	{
		m_BestLapTimeGui[i].Create(ND_LEFT);
		m_BestLapTimeGui[i].SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_0" ));
		m_BestLapTimeGui[i].SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_1" ));
		m_BestLapTimeGui[i].SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_2" ));
		m_BestLapTimeGui[i].SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_3" ));
		m_BestLapTimeGui[i].SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_4" ));
		m_BestLapTimeGui[i].SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_5" ));
		m_BestLapTimeGui[i].SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_6" ));
		m_BestLapTimeGui[i].SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_7" ));
		m_BestLapTimeGui[i].SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_8" ));
		m_BestLapTimeGui[i].SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_9" ));	
		m_BestLapTimeGui[i].SetNumGap(-5);
	}
	
	m_srfBestLapNumColon.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusNum_Colon" ) );


	// 진행타임 숫자
	for(RwInt8 i = 0 ; i < dTIMER_NUMBER ; ++i)
	{
		m_ClearTimeGui[i].Create(ND_LEFT);
		m_ClearTimeGui[i].SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_0" ));
		m_ClearTimeGui[i].SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_1" ));
		m_ClearTimeGui[i].SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_2" ));
		m_ClearTimeGui[i].SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_3" ));
		m_ClearTimeGui[i].SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_4" ));
		m_ClearTimeGui[i].SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_5" ));
		m_ClearTimeGui[i].SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_6" ));
		m_ClearTimeGui[i].SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_7" ));
		m_ClearTimeGui[i].SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_8" ));
		m_ClearTimeGui[i].SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNum_9" ));	
		m_ClearTimeGui[i].SetNumGap(-5);
	}

	m_srfTimeNumColon.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeNumColon" ) );

	// 슬롯
	for( RwInt8 i = 0 ; i < TMQ_REWARD ; ++i )
	{
		// srf Slot
		m_Reward[i].srfSlot.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlot" ));
	}	

	// 메인월드로의 귀환시간이 먼저오고 보상 내용이 이후에 온다
	m_fLimitedTime = 0.f;

	Init();

	// sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CTMQRewardGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CTMQRewardGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CTMQRewardGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CTMQRewardGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CTMQRewardGui::OnMouseLeave );	
	m_slotPaint			= m_pFlashBackground->SigPaint().Connect( this, &CTMQRewardGui::OnPaint );

	LinkMsg(g_EventQuestDirectForward);
	LinkMsg(g_EventTMQCleintState);	
	LinkMsg(g_EventTMQNotify);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CTMQRewardGui::Destroy()
{
	NTL_FUNCTION("CTMQRewardGui::Destroy");

	CheckInfoWindow();
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	for( RwInt8 i = 0 ; i < TMQ_REWARD ; ++i )
		m_Reward[i].SlotGui.Destroy();

	if( m_pNarrationGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pNarrationGui );
		m_pNarrationGui->Destroy();
		NTL_DELETE( m_pNarrationGui );
	}

	if( m_pCounterGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pCounterGui );
		m_pCounterGui->Destroy();
		NTL_DELETE( m_pCounterGui );
	}

	UnLinkMsg(g_EventQuestDirectForward);
	UnLinkMsg(g_EventTMQCleintState);
	UnLinkMsg(g_EventTMQNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTMQRewardGui::CreateInstance()
{
	if( m_pInstance )
		return;

	m_pInstance = NTL_NEW CTMQRewardGui("TMQRewardGui");
	if( !m_pInstance->Create() )
		NTL_DELETE(m_pInstance);

	CNtlPLGuiManager::GetInstance()->AddGui(m_pInstance);	
}

VOID CTMQRewardGui::DestroyInstance()
{
	if( !m_pInstance )
		return;

	GetNtlGuiManager()->RemoveGui(m_pInstance);
	m_pInstance->Destroy();
	NTL_DELETE(m_pInstance);
}

VOID CTMQRewardGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_UNKNOWN )
	{
		m_byInfoWindowIndex = INVALID_BYTE;
		GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
}

VOID CTMQRewardGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_pFlashBackground->Update(fElapsed);
	m_pFlashResult->Update(fElapsed);
	m_pFlashCapsule->Update(fElapsed);

	m_NumberZoomHelper.Update(fElapsed);

	if( m_fLimitedTime == 0.f )
		return;

	m_fLimitedTime -= fElapsed;

	if( m_fLimitedTime < 0.f )
	{
		m_fLimitedTime = 0.f;
	}

	m_pLimitedTime->Format(L"(%s : %d%s)", GetDisplayStringManager()->GetString(DST_TMQ_BOARD_LIMITE_TIME),
											(RwInt32)m_fLimitedTime,
											GetDisplayStringManager()->GetString(DST_TIME_SECOND));
}

VOID CTMQRewardGui::Init()
{
	m_iMouseDownSlot		= INVALID_INDEX;
	m_bFocus				= FALSE;
	m_uiBonusTime			= 0;
	m_bDayRecord			= FALSE;
	m_bBestRecord			= FALSE;
	m_byTurnOn				= TURNON_BEGIN;
    m_byInfoWindowIndex		= INVALID_BYTE;

	m_pConfirmButton->Show(false);
	m_pLimitedTime->Show(false);
}

VOID CTMQRewardGui::RegContents(const SNtlEventQuestDirect_Forward* pData)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	const SNtlEventQuestRewardDialog_Req* pReward = &pData->sReward;

	// 선택보상 사항 저장
	m_stEcho.sReward.bResult		= true;
	m_stEcho.sReward.sTSKey			= pReward->sTSKey;
	//m_stEcho.sReward.nSelRwdIdx		= INVALID_INDEX;
	m_stEcho.sReward.nSelRwdIdx		= INVALID_SELECT_REWARD_SLOT_INDEX;
	m_stEcho.eTDType				= ETD_QuestReward;
	m_stEcho.pTCUnit				= pData->pTCUnit;
	

	// 보상 슬롯
	for( RwInt8 i = 0 ; i < TMQ_REWARD ; ++i )
	{
		if( i < dSELECTION_REWARD_START_INDEX )
		{
			RegReward(m_Reward[i], &pReward->sDefaultReward[i], dREWARD_ICON_X + dREWARD_ICON_X_GAP*i, dREWARD_ICON_Y);
		}
		else
		{
			RegReward(m_Reward[i], &pReward->sSelectReward[i-dSELECTION_REWARD_START_INDEX], dREWARD_ICON_X  + dREWARD_ICON_X_GAP*(i-dSELECTION_REWARD_START_INDEX), dADDREWARD_ICON_Y);
		}
	}
}

VOID CTMQRewardGui::RegReward(sReward& reward, const sREWARD_INFO* pReward, RwInt32 iPosX, RwInt32 iPosY)
{
	reward.RewardInfo = *pReward;

	switch( pReward->m_eType )
	{
	case eREWARD_TYPE_NORMAL_ITEM:
		{
			reward.SlotGui.Create(m_pThis, DIALOG_UNKNOWN, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
			reward.SlotGui.SetIcon(pReward->m_uiIdx, pReward->m_nValue);
			break;
		}
	case eREWARD_TYPE_QUEST_ITEM:
		{
			reward.SlotGui.Create(m_pThis, DIALOG_UNKNOWN, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
			reward.SlotGui.SetIcon(pReward->m_uiIdx, pReward->m_nValue);
			break;
		}
	case eREWARD_TYPE_EXP:
		{
			reward.SlotGui.Create(m_pThis, DIALOG_UNKNOWN, REGULAR_SLOT_SIMPLE_ICON);
			reward.SlotGui.SetIcon(QUEST_REWARD_ICONNAME_EXP);
			break;
		}
	case eREWARD_TYPE_SKILL:
		{
			// peessi : REWARD의 경우, 스킬이 HTB인지 아닌지의 구분이 없이 모두 스킬이므로 예외처리 해주어야 한다.
			CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
			sSKILL_TBLDAT* pTableData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( pReward->m_uiIdx ) );
			if( pTableData )
			{
				if( pTableData->bySkill_Class == NTL_SKILL_CLASS_HTB )
				{
					CHTBSetTable* pHTBTable = API_GetTableContainer()->GetHTBSetTable();
					sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBTable->FindData( pReward->m_uiIdx ) );

					reward.SlotGui.Create(m_pThis, DIALOG_UNKNOWN, REGULAR_SLOT_HTBSKILL_TABLE);
					reward.SlotGui.SetIcon(pReward->m_uiIdx);
				}
				else
				{
					reward.SlotGui.Create(m_pThis, DIALOG_UNKNOWN, REGULAR_SLOT_SKILL_TABLE);
					reward.SlotGui.SetIcon(pReward->m_uiIdx);
				}				
			}			
			break;
		}
	case eREWARD_TYPE_ZENY:
		{
			reward.SlotGui.Create(m_pThis, DIALOG_UNKNOWN, REGULAR_SLOT_SIMPLE_ICON);
			reward.SlotGui.SetIcon(QUEST_REWARD_ICONNAME_ZENY);
			break;
		}
	case eREWARD_TYPE_CHANGE_CLASS:
		{
			reward.SlotGui.Create(m_pThis, DIALOG_UNKNOWN, REGULAR_SLOT_SIMPLE_ICON);
			reward.SlotGui.SetIcon(QUEST_REWARD_ICONNAME_CLASS);
			break;
		}
	case eREWARD_TYPE_BUFF:
		{
			reward.SlotGui.Create(m_pThis, DIALOG_UNKNOWN, REGULAR_SLOT_SKILL_TABLE);
			reward.SlotGui.SetIcon(pReward->m_uiIdx);
			break;
		}
	case eREWARD_TYPE_PROBABILITY:
		{
			reward.SlotGui.Create(m_pThis, DIALOG_UNKNOWN, REGULAR_SLOT_SIMPLE_ICON);
			reward.SlotGui.SetIcon(QUEST_REWARD_ICONNAME_PROB);
			break;
		}
	case eREWARD_TYPE_INVALID:
		break;
	default:
		NTL_ASSERT(false, "CTMQRewardGui::RegReward, Wrong reward type : " << pReward->m_eType);
		break;
	}
}

VOID CTMQRewardGui::NextSchedule()
{
	if( m_byTurnOn == INVALID_TURNON )
		return;

	++m_byTurnOn;

	switch(m_byTurnOn)
	{
	case TURNON_TEXT:
		{
			break;
		}
	case TURNON_CLEAR_TIME:
		{
			NextSchedule();
			break;
		}
	case TURNON_CAPSULE_1:
	case TURNON_CAPSULE_2:
	case TURNON_CAPSULE_3:
	case TURNON_CAPSULE_4:
	case TURNON_CAPSULE_5:
	case TURNON_CAPSULE_6:
	case TURNON_CAPSULE_7:
	case TURNON_CAPSULE_8:
		{
			RwUInt8 byFlashCapsulePlayIndex = (RwUInt8)(m_byTurnOn - TURNON_CAPSULE_1);

			if( byFlashCapsulePlayIndex < TMQ_REWARD )
			{
				if( m_Reward[byFlashCapsulePlayIndex].SlotGui.IsOnlyUsableIcon() ||
					m_Reward[byFlashCapsulePlayIndex].SlotGui.GetSerial() != INVALID_SERIAL_ID )
				{
					GetReadyCapsule(byFlashCapsulePlayIndex);
				}
				else
					NextSchedule();
			}
			else
				NextSchedule();
			
			break;
		}
	}
}

VOID CTMQRewardGui::LoadFlash(const char* pcFileName, gui::CFlash* pFlash)
{
	if( pFlash->Load(pcFileName) == false )
	{
		if( pcFileName )
		{
			char acBuffer[256] = "";
			sprintf_s(acBuffer, 256, "CTMQRewardGui::LoadFlash, Not exist %s flash file", pcFileName);
			NtlLogFilePrint(acBuffer);
		}		
	}

	pFlash->PlayMovie(FALSE);

	CRectangle rtRect = pFlash->GetFrameResolution();
	pFlash->SetSize(rtRect.GetWidth(), rtRect.GetHeight());
}

VOID CTMQRewardGui::OnConfirmButton(gui::CComponent* pComponent)
{	
	if( m_byTurnOn != INVALID_TURNON )
		return;

	if( m_Reward[m_stEcho.sReward.nSelRwdIdx].RewardInfo.m_eType != eREWARD_TYPE_INVALID )
	{
		if( m_stEcho.sReward.nSelRwdIdx == INVALID_INDEX )
		{
			// 선택보상을 고르지 않았습니다
			GetAlarmManager()->AlarmMessage(DST_TMQ_STATUS_MUST_SELECT_ADDITION_REWARD);
			return;
		}
	}

	// TS로 보상내용 전달
	if( m_stEcho.sReward.nSelRwdIdx != INVALID_INDEX )
		CNtlSLEventGenerator::QuestTriggerDirectEcho( &m_stEcho );


	// 메인월드로 돌아가기까지 남은시간
	m_pCounterGui = NTL_NEW CCounterGui("CounterGui");
	if( !m_pCounterGui->Create(COUNTER_DISPLAY_2, m_fLimitedTime, COUNTER_QUADRANT_4) )
	{
		m_pCounterGui->Destroy();
		NTL_DELETE(m_pCounterGui);
	}
	CNtlPLGuiManager::GetInstance()->AddGui( m_pCounterGui );
	m_pCounterGui->AlarmSound(TRUE, 10.f);
	OnMove(0, 0);


	if( m_pNarrationGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pNarrationGui );
		m_pNarrationGui->Destroy();
		NTL_DELETE( m_pNarrationGui );
	}


	GetDialogManager()->OpenGMDialog();
	GetDialogManager()->OpenDefaultDialog();
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	SwitchDialog(false);
}

VOID CTMQRewardGui::OnFrameCallback_Result(const char* pcParam, const char* pcParam2)
{
	if( strcmp(pcParam, "TextEnd1") == 0 )
	{
		m_pFlashResult->Invoke("Mission_Result", "%d", 1);

		m_pFlashResult->Invoke("Clear_Grade", "%d", m_byClearGrade);


		if( m_bBestRecord )
		{
			m_pFlashResult->Invoke("Draw_Best_Lap", "");			
		}
		else if( m_bDayRecord )
		{
			m_pFlashResult->Invoke("Draw_Daily_Lap", "");
		}


		if( m_uiBonusTime > 0 )
			m_pFlashResult->Invoke("Draw_Bonus", "");

		NextSchedule();
	}
	else if( strcmp(pcParam, "TextEnd_cleartime") == 0 )
	{
		m_NumberZoomHelper.RemoveAll();

		for( RwUInt8 i = 0 ; i < dTIMER_NUMBER ; ++i )
			m_NumberZoomHelper.AddNumberGui(&m_ClearTimeGui[i], dNUMEFFECT_START_RATE, dNUMEFFECT_END_RATE, dNUMEFFECT_TIME);

		m_NumberZoomHelper.LinkFinishCallback(this, &CTMQRewardGui::OnCallback_NumberScaleEffect);
		m_NumberZoomHelper.Play(TRUE);
	}	
}

RwInt32 CTMQRewardGui::OnCallback_NumberScaleEffect()
{
	NextSchedule();
	return 1;
}

VOID CTMQRewardGui::OnMovieEnd_Capsule(gui::CComponent* pComponent)
{
	NextSchedule();	
}

VOID CTMQRewardGui::GetReadyCapsule(RwUInt8 byFlashCapsulePlayIndex)
{
	CRectangle rtRect;	
	RwInt32 iX = m_Reward[byFlashCapsulePlayIndex].SlotGui.GetX_fromParent();
	RwInt32 iY = m_Reward[byFlashCapsulePlayIndex].SlotGui.GetY_fromParent();

	rtRect.left		= iX - (m_pFlashCapsule->GetWidth() - m_Reward[byFlashCapsulePlayIndex].SlotGui.GetWidth())/2;
	rtRect.top		= iY - (m_pFlashCapsule->GetHeight() - m_Reward[byFlashCapsulePlayIndex].SlotGui.GetHeight())/2;
	rtRect.right	= rtRect.left + m_pFlashCapsule->GetWidth();
	rtRect.bottom	= rtRect.top  + m_pFlashCapsule->GetHeight();	

	m_pFlashCapsule->Show(true);
	m_pFlashCapsule->SetPosition(rtRect);
	m_pFlashCapsule->RestartMovie();	
}

VOID CTMQRewardGui::OnMouseDown(const CKey& key)
{
	if( key.m_nID != UD_LEFT_BUTTON )
		return;

	if( GetIconMoveManager()->IsActive() )
		return;

	if( GetDialogManager()->GetMode() != DIALOGMODE_UNKNOWN )
		return;

	if( m_pConfirmButton->IsClickEnable() == false )
		return;

	if( m_byTurnOn != INVALID_TURNON )
		return;

	RwInt8 iPtinSlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if(iPtinSlot != INVALID_INDEX)
	{
		m_iMouseDownSlot = iPtinSlot;
	}	
}

VOID CTMQRewardGui::OnMouseUp(const CKey& key)
{
	if( m_iMouseDownSlot ==INVALID_INDEX )
		return;

	if( m_byTurnOn != INVALID_TURNON )
		return;

	if( key.m_nID == UD_LEFT_BUTTON && m_iMouseDownSlot != INVALID_SERIAL_ID )
	{
		RwInt8 iPtinSlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);

		if(iPtinSlot != INVALID_INDEX)
		{
			if( m_Reward[iPtinSlot].SlotGui.IsOnlyUsableIcon() ||
				m_Reward[iPtinSlot].SlotGui.GetSerial() != INVALID_SERIAL_ID )
				SelectEffect(TRUE, m_iMouseDownSlot);
		}
	}

	m_iMouseDownSlot = INVALID_INDEX;
}

VOID CTMQRewardGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen(0, 0, GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
	CRectangle rtRect;
	RwInt32 iPibotX = rtScreen.GetWidth()/2;
	RwInt32 iPibotY = rtScreen.GetHeight()/2;
	RwInt32 iX, iY;

	m_pThis->SetPosition(rtScreen);

	m_pConfirmButton->SetPosition(iPibotX + 300, iPibotY + 60);
	m_pLimitedTime->SetPosition(iPibotX + 300, iPibotY + 30);

	if( m_pCounterGui )
		m_pCounterGui->SetPosition(5, rtScreen.bottom - 125);

	// 배경 플래쉬
	rtRect = rtScreen;
	rtRect.top		= (rtScreen.GetHeight() - m_pFlashResult->GetHeight())/2;
	rtRect.bottom	= rtRect.top + m_pFlashBackground->GetHeight();
	m_pFlashBackground->SetPosition(rtRect);

	// 결과 플래쉬
	m_pFlashResult->SetPosition((rtScreen.GetWidth() - m_pFlashResult->GetWidth())/2,
								(rtScreen.GetHeight() - m_pFlashResult->GetHeight())/2);

	// 슬롯
	iX = dREWARD_ICON_X;
	iY = dREWARD_ICON_Y;
	for( RwInt8 i = 0 ; i < TMQ_REWARD ; ++i )
	{
		m_Reward[i].SlotGui.SetPosition_fromParent(iPibotX + iX + 2, iPibotY + iY + 2);
		m_Reward[i].srfSlot.SetPosition(iX + iPibotX, iY + iPibotY);

		iX += dREWARD_ICON_X_GAP;

		if( i == 3 )
		{
			iX = dREWARD_ICON_X;
			iY = dADDREWARD_ICON_Y;
		}
	}	

	// 셀렉트 이펙트
	if( dSELECTION_REWARD_START_INDEX <= m_stEcho.sReward.nSelRwdIdx + dSELECTION_REWARD_START_INDEX &&
		m_stEcho.sReward.nSelRwdIdx + dSELECTION_REWARD_START_INDEX < 8 )
		SelectEffect(TRUE, m_stEcho.sReward.nSelRwdIdx + dSELECTION_REWARD_START_INDEX);


	// 시간 숫자
	RwInt32 iClearTimeX = iPibotX + 217;
	RwInt32 iBestTimeTimeX = iPibotX + 217;
	RwInt32 iBonusTimeTimeX = iPibotX + 217;

	for(RwInt8 i = dTIMER_NUMBER - 1 ; i >= 0 ; --i)
	{
		m_ClearTimeGui[i].SetPosition(iClearTimeX, iPibotY - 100);
		m_BestLapTimeGui[i].SetPosition(iBestTimeTimeX, iPibotY - 60);
		m_BonusTimeGui[i].SetPosition(iBonusTimeTimeX, iPibotY - 32);

		iClearTimeX		-= m_ClearTimeGui[i].GetContentWidth(i) + dNUM_GAP;
		iBestTimeTimeX	-= m_BestLapTimeGui[i].GetContentWidth(i) + dNUM_GAP;
		iBonusTimeTimeX	-= m_BonusTimeGui[i].GetContentWidth(i) + dNUM_GAP;

		if( i == 2 )
		{			
			iClearTimeX -= m_srfTimeNumColon.GetWidth();
			m_srfTimeNumColon.SetPosition(iClearTimeX, iPibotY - 100);
			iClearTimeX -= dNUM_GAP;

			iBestTimeTimeX -= m_srfBestLapNumColon.GetWidth();
			m_srfBestLapNumColon.SetPosition(iBestTimeTimeX, iPibotY - 60);
			iBestTimeTimeX -= dNUM_GAP;

			iBonusTimeTimeX -= m_srfBonusNumColon.GetWidth();
			m_srfBonusNumColon.SetPosition(iBonusTimeTimeX, iPibotY - 32);
			iBonusTimeTimeX -= dNUM_GAP;
		}
	}


	// 캡슐 플래쉬
	if( TURNON_CAPSULE_1 <= m_byTurnOn && m_byTurnOn <= TURNON_CAPSULE_8 )
	{
		RwUInt8 byFlashCapsulePlayIndex = (RwInt8)(m_byTurnOn - TURNON_CAPSULE_1);

		iX = m_Reward[byFlashCapsulePlayIndex].SlotGui.GetX_fromParent();
		iY = m_Reward[byFlashCapsulePlayIndex].SlotGui.GetY_fromParent();

		rtRect.left		= iX - (m_pFlashCapsule->GetWidth() - m_Reward[byFlashCapsulePlayIndex].SlotGui.GetWidth())/2;
		rtRect.top		= iY - (m_pFlashCapsule->GetHeight() - m_Reward[byFlashCapsulePlayIndex].SlotGui.GetHeight())/2;
		rtRect.right	= rtRect.left + m_pFlashCapsule->GetWidth();
		rtRect.bottom	= rtRect.top  + m_pFlashCapsule->GetHeight();

		m_pFlashCapsule->SetPosition(rtRect);
	}
}

VOID CTMQRewardGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_byTurnOn != INVALID_TURNON )
		return;

	RwInt8 iPtinSlot = PtinSlot(nX, nY);
	if( iPtinSlot != INVALID_INDEX )
	{
		if( m_byTurnOn == INVALID_TURNON )
		{
			// 아이콘 영역으로 마우스를 이동시켜서 아이템 정보를 보일 필요가 있는지 검사
			if( m_Reward[iPtinSlot].SlotGui.IsOnlyUsableIcon() ||
				m_Reward[iPtinSlot].SlotGui.GetSerial() != INVALID_SERIAL_ID &&
				m_byInfoWindowIndex != iPtinSlot )
			{
				CInfoWndManager::eINFOWNDSTATE eInfoState;
				stINFOWND_REWARD RewardInfo;
				VOID* pData = (VOID*)&RewardInfo;
				RwBool bShowInfo = FALSE;

				switch( m_Reward[iPtinSlot].RewardInfo.m_eType )
				{
				case eREWARD_TYPE_NORMAL_ITEM:
					{
						eInfoState = CInfoWndManager::INFOWND_TABLE_ITEM;
						pData = m_Reward[iPtinSlot].SlotGui.GetItemTable();
						bShowInfo = TRUE;
						break;
					}
				case eREWARD_TYPE_QUEST_ITEM:
					{
						eInfoState = CInfoWndManager::INFOWND_QUESTITEM;
						pData = m_Reward[iPtinSlot].SlotGui.GetItemTable();
						bShowInfo = TRUE;
						break;
					}			
				case eREWARD_TYPE_SKILL:
					{
						// peessi : REWARD의 경우, 스킬이 HTB인지 아닌지의 구분이 없이 모두 스킬이므로 예외처리 해주어야 한다.
						if( m_Reward[iPtinSlot].SlotGui.GetSerialType() == REGULAR_SLOT_HTBSKILL_TABLE )
						{
							eInfoState = CInfoWndManager::INFOWND_NOTLEARN_HTBSKILL;
							pData = m_Reward[iPtinSlot].SlotGui.GetHTBSkillTable();
							bShowInfo = TRUE;
						}
						else
						{
							eInfoState = CInfoWndManager::INFOWND_NOTLEARN_SKILL;
							pData = m_Reward[iPtinSlot].SlotGui.GetSkillTable();
							bShowInfo = TRUE;
						}						
						break;
					}			
				case eREWARD_TYPE_CHANGE_CLASS:
					{	
						eInfoState			= (CInfoWndManager::eINFOWNDSTATE)m_Reward[iPtinSlot].RewardInfo.m_eType;
						RewardInfo.eType	= m_Reward[iPtinSlot].RewardInfo.m_eType;
						RewardInfo.nValue	= m_Reward[iPtinSlot].RewardInfo.m_nValue;
						bShowInfo = TRUE;
						break;
					}
				case eREWARD_TYPE_BUFF:
					{
						eInfoState = CInfoWndManager::INFOWND_NOTSOBBUFF;
						pData = m_Reward[iPtinSlot].SlotGui.GetSkillTable();
						bShowInfo = TRUE;
						break;
					}
				case eREWARD_TYPE_EXP:
				case eREWARD_TYPE_ZENY:
				case eREWARD_TYPE_PROBABILITY:
				case eREWARD_TYPE_INVALID:
					{
						eInfoState			= CInfoWndManager::INFOWND_REWARD;
						RewardInfo.eType	= m_Reward[iPtinSlot].RewardInfo.m_eType;
						RewardInfo.nValue	= m_Reward[iPtinSlot].RewardInfo.m_nValue;
						bShowInfo = TRUE;
						break;	
					}
				default:
					NTL_ASSERT(false, "CTMQRewardGui::OnMouseMove, Wrong reward type : " << m_Reward[iPtinSlot].RewardInfo.m_eType);
					break;
				}

				if( bShowInfo )
				{
					CRectangle rtScreen = m_pThis->GetScreenRect();
					GetInfoWndManager()->ShowInfoWindow(TRUE, eInfoState,
						rtScreen.left + m_Reward[iPtinSlot].SlotGui.GetX_fromParent(),
						rtScreen.top + m_Reward[iPtinSlot].SlotGui.GetY_fromParent(),
						pData, DIALOG_UNKNOWN );
					m_byInfoWindowIndex = iPtinSlot;
				}

				FocusEffect(TRUE, iPtinSlot);
				return;
			}		
		}
	}

	m_byInfoWindowIndex = INVALID_INDEX;
	FocusEffect(FALSE);
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTMQRewardGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(FALSE);
	m_byInfoWindowIndex = INVALID_INDEX;
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

RwInt8 CTMQRewardGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	for( RwInt32 i = 0 ; i < TMQ_REWARD ; ++i )
	{
		if( m_Reward[i].SlotGui.PtInRect(iX, iY) )
			return i;
	}

	return INVALID_INDEX;
}

VOID CTMQRewardGui::FocusEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if(bPush)
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		RwInt32 iX = rtScreen.left + m_Reward[iSlotIdx].SlotGui.GetX_fromParent();
		RwInt32 iY = rtScreen.top + m_Reward[iSlotIdx].SlotGui.GetY_fromParent();

		m_FocusEffect.SetRectWH(iX, iY, 32, 32);
		m_bFocus = TRUE;
	}
	else
		m_bFocus = FALSE;
}

VOID CTMQRewardGui::SelectEffect( RwBool bPush, RwInt32 iSlotIdx /* = -1 */)
{
	if(bPush)
	{
		if( iSlotIdx < dSELECTION_REWARD_START_INDEX )
			return;

		m_stEcho.sReward.nSelRwdIdx = iSlotIdx - dSELECTION_REWARD_START_INDEX;

		CRectangle rtScreen = m_pThis->GetScreenRect();
		RwInt32 iX = m_Reward[iSlotIdx].SlotGui.GetX_fromParent();
		RwInt32 iY = m_Reward[iSlotIdx].SlotGui.GetY_fromParent();

		m_SelectEffect.SetSize(32, 32);
		m_SelectEffect.SetPositionfromParent(iX, iY);
		m_SelectEffect.SetPositionbyParent(rtScreen.left, rtScreen.top);
	}
	else
		m_stEcho.sReward.nSelRwdIdx = INVALID_INDEX;
}

VOID CTMQRewardGui::OnPaint()
{
	for(RwInt8 i = 0 ; i < TMQ_REWARD ; ++i)
	{		
		if( m_Reward[i].SlotGui.IsOnlyUsableIcon() ||
			m_Reward[i].SlotGui.GetSerial() != INVALID_SERIAL_ID )
		{
			if( m_byTurnOn > TURNON_CAPSULE_1 + i )
			{
				m_Reward[i].srfSlot.Render();
				m_Reward[i].SlotGui.Paint();
			}
		}
	}

	if( m_bFocus )
		m_FocusEffect.Render();

	if( m_stEcho.sReward.nSelRwdIdx != INVALID_INDEX )
		m_SelectEffect.Render();	


	if( m_byTurnOn >= TURNON_CLEAR_TIME )
	{
		for(RwInt8 i = 0 ; i < dTIMER_NUMBER ; ++i)
		{
			if( m_uiBonusTime > 0 )
				m_BonusTimeGui[i].Render();

			if( m_bBestRecord || m_bDayRecord )
				m_BestLapTimeGui[i].Render();
		}

		if( m_uiBonusTime > 0 )
			m_srfBonusNumColon.Render();

		if( m_bBestRecord || m_bDayRecord )
			m_srfBestLapNumColon.Render();
	}


	if( m_byTurnOn >= TURNON_TEXT )
	{
		for(RwInt8 i = 0 ; i < dTIMER_NUMBER ; ++i)
			m_ClearTimeGui[i].Render();

		m_srfTimeNumColon.Render();
	}
}

RwInt32 CTMQRewardGui::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		CheckInfoWindow();
		Show(false);
	}

	return 1;
}

VOID CTMQRewardGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTMQRewardGui::HandleEvents");

	if( msg.Id == g_EventQuestDirectForward )
	{
		const SNtlEventQuestDirect_Forward* pEvent = reinterpret_cast<const SNtlEventQuestDirect_Forward*>( msg.pData );

		if( pEvent->eTDType != ETD_QuestReward )
			return;

		if( pEvent->sReward.eRewardType != eREWARD_CONTAINER_TYPE_TMQ )
			return;

		Init();

		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
		sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = pTMQ->GetTMQTable();
		sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &pTIMEQUEST_TBLDAT->sTimeQuestDataset[pTMQ->GetDifficult()];
		RwReal fPercent = pTMQ->GetClearTime() / (RwReal)pTIMEQUEST_DATASET->dwLimitTime;
		RwUInt32 uiClearTime = pTMQ->GetClearTime();

		// TMQ 성적
		if( fPercent <= dRESULT_EXCELLENT )
		{
			m_byClearGrade = 3;
		}
		else if( fPercent <= dRESULT_GREATE )
		{
			m_byClearGrade = 2;
		}
		else if( fPercent <= dRESULT_GOOD )
		{
			m_byClearGrade = 1;
		}
		else
		{
			m_byClearGrade = 0;
		}

		// 최단 기록 여부
		if( pTMQ->IsBestRecord() )
		{
			m_bBestRecord = TRUE;
		}
		else if( pTMQ->IsDayRecord() )
		{
			m_bDayRecord = TRUE;
		}

		// 보상 내용을 창에 등록한다
		RegContents(pEvent);


		// 클리어 타임
		RwUInt8 byMinute = (RwUInt8)(uiClearTime/60);
		RwUInt8 bySecond = (RwUInt8)(uiClearTime%60);

		m_ClearTimeGui[0].SetNumber(byMinute/10);
		m_ClearTimeGui[1].SetNumber(byMinute%10);
		m_ClearTimeGui[2].SetNumber(bySecond/10);
		m_ClearTimeGui[3].SetNumber(bySecond%10);


		// 최단 시간
		if( m_bBestRecord || m_bDayRecord )
		{
			m_BestLapTimeGui[0].SetNumber(byMinute/10);
			m_BestLapTimeGui[1].SetNumber(byMinute%10);
			m_BestLapTimeGui[2].SetNumber(bySecond/10);
			m_BestLapTimeGui[3].SetNumber(bySecond%10);
		}


		// 보너스 타임
		m_uiBonusTime = pTMQ->GetBonusTime();

		if( m_uiBonusTime > 0 )
		{
			RwUInt8 byMinute = (RwUInt8)(m_uiBonusTime/60);
			RwUInt8 bySecond = (RwUInt8)(m_uiBonusTime%60);

			m_BonusTimeGui[0].SetNumber(byMinute/10);
			m_BonusTimeGui[1].SetNumber(byMinute%10);
			m_BonusTimeGui[2].SetNumber(bySecond/10);
			m_BonusTimeGui[3].SetNumber(bySecond%10);
		}

		LoadFlash("TMQ_result_bg.swf", m_pFlashBackground);
		LoadFlash("TMQ_result.swf", m_pFlashResult);
		LoadFlash("TMQ_result_Capsule.swf", m_pFlashCapsule);

		m_pFlashBackground->PlayMovie(true);
		m_pFlashResult->PlayMovie(true);

		m_pFlashCapsule->Show(false);

		m_pConfirmButton->Show(true);
		m_pLimitedTime->Show(true);		

		// 보상창이 열림과 동시에 다른 모든 다이얼로그는 닫는다
		GetDialogManager()->CloseAll();
		SwitchDialog(true);

		// NPC 나레이션
		if( !m_pNarrationGui )
		{
			m_pNarrationGui = NTL_NEW CResultNarrationGui("TMQNarrationGui");
			if( !m_pNarrationGui->Create() )
			{
				m_pNarrationGui->Destroy();
				NTL_DELETE(m_pNarrationGui);
			}
			CNtlPLGuiManager::GetInstance()->AddGui( m_pNarrationGui );

			const WCHAR* pwcText = GetDisplayStringManager()->GetString(DST_TMQ_RESULT_NARRATION);
			if( pwcText )
				m_pNarrationGui->SetData("TMQ_result_npc.swf", pwcText);
		}

		OnMove(0, 0);
	}
	else if( msg.Id == g_EventTMQNotify )
	{
		SNtlEventTMQNotify* pEvent = reinterpret_cast<SNtlEventTMQNotify*>( msg.pData );

		// 제한시간
		if( pEvent->uiEventType == TET_TIME_UNTIL_TELEPORT_TO_MAINWORLD )
		{
			m_fLimitedTime = (RwReal)pEvent->uiValue/1000.f;
			GetNtlGuiManager()->AddUpdateFunc( this );
		}
	}
	else if( msg.Id == g_EventTMQCleintState )
	{
		SNtlEventTMQServerState* pEvent = reinterpret_cast<SNtlEventTMQServerState*>( msg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_TIME_MACHINE )
			NTL_RETURNVOID();

		if( pEvent->uiState == WORLD_STATE_TMQ_FAIL)
			SwitchDialog(false);
	}

	NTL_RETURNVOID();
}