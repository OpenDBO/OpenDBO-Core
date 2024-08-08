#include "precomp_dboclient.h"
#include "TMQStatusGui.h"

// core
#include "NtlDebug.h"

// share
#include "TimeQuestTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEventfunc.h"
#include "NtlSLTMQ.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlWorldconceptTMQ.h"

// dbo
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "DboEventGenerator.h"
#include "SideDialogManager.h"
#include "LittleQuestContainer.h"

// test
#include "GUISoundDefine.h"

namespace
{
	// Time bonus
	#define dTIME_BONUS_DISTANCE		(80.f)
	#define dTIME_BONUS_SHOWTIME		(3.f)
	#define dTIME_BONUS_DEFAULT_Y		(50)

	#define dCOUPON_ICON_NAME			"TMQ_Coupon.png"

	// TMQ 초반에 개별 인터페이스 설명을 하는 시간
	#define dUPDATE_TIME				(5.f)

	#define dLEFT_TIME_NUMBER_GAP		(-5)
	#define dRECORD_TIME_NUMBER_GAP		(0)
};


CTMQStatusGui* CTMQStatusGui::m_pInstance = NULL;

CTMQStatusGui::CTMQStatusGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pTMQQuestContainer(NULL)
{
}

CTMQStatusGui::~CTMQStatusGui()
{
	Destroy();
}

RwBool CTMQStatusGui::Create()
{
	NTL_FUNCTION( "CTMQStatusGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\TMQStatus.srf", "gui\\TMQStatus.frm"))
		NTL_RETURN( FALSE  );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 남은시간 분 표시
	m_LeftTimeMinuteNumber.Create(ND_RIGHT, NVD_TOP, dLEFT_TIME_NUMBER_GAP, dNCFLAG_NONE, 2);
	m_LeftTimeMinuteNumber.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_0" ));
	m_LeftTimeMinuteNumber.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_1" ));
	m_LeftTimeMinuteNumber.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_2" ));
	m_LeftTimeMinuteNumber.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_3" ));
	m_LeftTimeMinuteNumber.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_4" ));
	m_LeftTimeMinuteNumber.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_5" ));
	m_LeftTimeMinuteNumber.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_6" ));
	m_LeftTimeMinuteNumber.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_7" ));
	m_LeftTimeMinuteNumber.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_8" ));
	m_LeftTimeMinuteNumber.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_9" ));	
	m_LeftTimeMinuteNumber.SetNumber(0);

	m_srfRemainSemiColon.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSemiColon" ));

	// 남은시간 초 표시
	m_LeftTimeSecondNumber.Create(ND_RIGHT, NVD_TOP, dLEFT_TIME_NUMBER_GAP, dNCFLAG_NONE, 2);
	m_LeftTimeSecondNumber.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_0" ));
	m_LeftTimeSecondNumber.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_1" ));
	m_LeftTimeSecondNumber.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_2" ));
	m_LeftTimeSecondNumber.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_3" ));
	m_LeftTimeSecondNumber.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_4" ));
	m_LeftTimeSecondNumber.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_5" ));
	m_LeftTimeSecondNumber.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_6" ));
	m_LeftTimeSecondNumber.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_7" ));
	m_LeftTimeSecondNumber.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_8" ));
	m_LeftTimeSecondNumber.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_9" ));	
	m_LeftTimeSecondNumber.SetNumber(0);

	RwInt32 iXMinutePos = 35;
	m_LeftTimeMinuteNumber.SetPosition(iXMinutePos, 56);
	m_srfRemainSemiColon.SetPositionfromParent(iXMinutePos + m_LeftTimeMinuteNumber.GetWidth(), 56);
	m_LeftTimeSecondNumber.SetPosition(iXMinutePos + m_LeftTimeMinuteNumber.GetWidth() + m_srfRemainSemiColon.GetWidth(), 56);
	
	
	// TimeBonus surface
	m_TimeBonus.srfTimeBonus.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfBonusTime" ));

	// 최단시간 분 표시
	RwInt32 iRecordX = 103;
	m_RecordTimeMinuteNumber.Create(ND_RIGHT, NVD_TOP, dRECORD_TIME_NUMBER_GAP, dNCFLAG_NONE, 2);
	m_RecordTimeMinuteNumber.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_0" ));
	m_RecordTimeMinuteNumber.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_1" ));
	m_RecordTimeMinuteNumber.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_2" ));
	m_RecordTimeMinuteNumber.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_3" ));
	m_RecordTimeMinuteNumber.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_4" ));
	m_RecordTimeMinuteNumber.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_5" ));
	m_RecordTimeMinuteNumber.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_6" ));
	m_RecordTimeMinuteNumber.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_7" ));
	m_RecordTimeMinuteNumber.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_8" ));
	m_RecordTimeMinuteNumber.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_9" ));	
	m_RecordTimeMinuteNumber.SetRate(0.5);
	m_RecordTimeMinuteNumber.SetNumber(0);
	m_RecordTimeMinuteNumber.SetPosition(iRecordX, 92);
	iRecordX += m_RecordTimeMinuteNumber.GetWidth();

	m_srfRecordSemiColon.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSemiColon" ));
	m_srfRecordSemiColon.SetSize(m_srfRecordSemiColon.GetWidth()/2, m_srfRecordSemiColon.GetHeight()/2);
	m_srfRecordSemiColon.SetPositionfromParent(iRecordX, 92);
	iRecordX += m_srfRecordSemiColon.GetWidth();

	// 최단시간 초 표시	
	m_RecordTimeSecondNumber.Create(ND_RIGHT, NVD_TOP, dRECORD_TIME_NUMBER_GAP, dNCFLAG_NONE, 2);
	m_RecordTimeSecondNumber.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_0" ));
	m_RecordTimeSecondNumber.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_1" ));
	m_RecordTimeSecondNumber.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_2" ));
	m_RecordTimeSecondNumber.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_3" ));
	m_RecordTimeSecondNumber.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_4" ));
	m_RecordTimeSecondNumber.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_5" ));
	m_RecordTimeSecondNumber.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_6" ));
	m_RecordTimeSecondNumber.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_7" ));
	m_RecordTimeSecondNumber.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_8" ));
	m_RecordTimeSecondNumber.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfSideNum_9" ));	
	m_RecordTimeSecondNumber.SetRate(0.5);
	m_RecordTimeSecondNumber.SetNumber(0);
	m_RecordTimeSecondNumber.SetPosition(iRecordX , 92);	


	// 타임 보너스
	m_TimeBonus.TimeBonusNum.Create(ND_LEFT, NVD_TOP, 0, NC_PLUS);
	m_TimeBonus.TimeBonusNum.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_0" ));
	m_TimeBonus.TimeBonusNum.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_1" ));
	m_TimeBonus.TimeBonusNum.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_2" ));
	m_TimeBonus.TimeBonusNum.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_3" ));
	m_TimeBonus.TimeBonusNum.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_4" ));
	m_TimeBonus.TimeBonusNum.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_5" ));
	m_TimeBonus.TimeBonusNum.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_6" ));
	m_TimeBonus.TimeBonusNum.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_7" ));
	m_TimeBonus.TimeBonusNum.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_8" ));
	m_TimeBonus.TimeBonusNum.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_9" ));
	m_TimeBonus.TimeBonusNum.SetSurface(NC_PLUS, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfMinuteNumber_Plus" ));
	m_TimeBonus.TimeBonusNum.SetRate(0.7f);
	m_TimeBonus.TimeBonusNum.SetNumGap(-5);

	m_pDialogName			= (gui::CStaticBox*)GetComponent( "stbDialogName" );
	m_pLeftTime				= (gui::CStaticBox*)GetComponent( "stbLeftTime" );
	m_pRecordTime			= (gui::CStaticBox*)GetComponent( "stbRecordTime" );
	m_pInfo					= (gui::CStaticBox*)GetComponent( "stbInfo" );
	m_pPoint				= (gui::CStaticBox*)GetComponent( "stbPoint" );
	m_pCurPoint				= (gui::CStaticBox*)GetComponent( "stbCurPoint" );
	m_pQuest				= (gui::CStaticBox*)GetComponent( "stbQuest" );
	m_pQuestTitle			= (gui::CStaticBox*)GetComponent( "stbQuestTitle" );
	m_pQuestAim_1			= (gui::CStaticBox*)GetComponent( "stbQuestAim_1" );
	m_pQuestAim_2			= (gui::CStaticBox*)GetComponent( "stbQuestAim_2" );
	m_pQuestAim_3			= (gui::CStaticBox*)GetComponent( "stbQuestAim_3" );
	m_pQuestAim_4			= (gui::CStaticBox*)GetComponent( "stbQuestAim_4" );

	m_pDialogName->CreateFontStd(DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pDialogName->SetText(GetDisplayStringManager()->GetString(DST_TMQ_STATUS_TITLE));
	m_pLeftTime->SetText(GetDisplayStringManager()->GetString(DST_TMQ_STATUS_REMAIN_TIME));
	m_pRecordTime->SetText(GetDisplayStringManager()->GetString(DST_TMQ_STATUS_LASTEST_RECORD));
	m_pInfo->SetText(GetDisplayStringManager()->GetString(DST_TMQ_STATUS_INFO));
	m_pPoint->SetText(GetDisplayStringManager()->GetString(DST_TMQ_STATUS_POINT));
	m_pCurPoint->SetText(L"0%");
	m_pQuest->SetText(GetDisplayStringManager()->GetString(DST_TMQ_STATUS_QUEST));

	// 스테이지 정보
	m_pStageButton = (gui::CButton*)GetComponent( "btnStageInfo" );
	m_pStageButton->SetToolTip(GetDisplayStringManager()->GetString(DST_TMQ_STATUS_STATUS_INFO));
	m_slotStageButton = m_pStageButton->SigClicked().Connect(this, &CTMQStatusGui::OnClicked_InfoButton);

	// 최소화 버튼
	m_pMinimizeButton = (gui::CButton*)GetComponent( "btnMinimize" );
	m_slotMinimizeButton = m_pMinimizeButton->SigClicked().Connect(this, &CTMQStatusGui::OnClicked_MinimizeButton);

	// 시나리오 버튼
	m_pSenarioButton = (gui::CButton*)GetComponent( "btnSenario" );
	m_pSenarioButton->SetToolTip(GetDisplayStringManager()->GetString(DST_TMQ_STATUS_SENARIO));
	m_slotSenarioButton = m_pSenarioButton->SigClicked().Connect(this, &CTMQStatusGui::OnClicked_SenarioButton);	

	// 다음 퀘스트 버튼
	m_pNextButton = (gui::CButton*)GetComponent( "btnNextQuest" );
	m_slotNextButton = m_pNextButton->SigClicked().Connect(this, &CTMQStatusGui::OnClicked_NextButton);

	// 시나리오 버튼 업데이트 알림
	gui::CSurface surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TMQStatus.srf", "srfButtonEffect" );
	m_SenarioButtonFlicker.SetSurface( surface );
	m_SenarioButtonFlicker.SetTime( 1000.0f, 0.5f );
	m_SenarioButtonFlicker.SetAlpha( 0, 255 );

	// 스테이지 버튼 업데이트 알림
	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TMQStatus.srf", "srfButtonEffect" );
	m_StageButtonFlicker.SetSurface( surface );
	m_StageButtonFlicker.SetTime( 1000.0f, 0.5f );
	m_StageButtonFlicker.SetAlpha( 0, 255 );

	// TMQ 포인트 게이지
	m_pTMQGauge = (gui::CProgressBar*)GetComponent( "TMQGauge" );
	m_pTMQGauge->SetToolTip(GetDisplayStringManager()->GetString(DST_TMQ_STATUS_EXPLAIN_POINT));

	// TMQ 포인트 배경
	m_srfGaugeBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TMQStatus.srf", "srfGaugeBack" ));
	m_srfGaugeBack.SetPositionfromParent(8, 198);

	// 쿠폰 슬롯
	m_CouponSlot.Create(m_pThis, DIALOG_TMQBOARD, REGULAR_SLOT_SIMPLE_ICON, SDS_COUNT);
	m_CouponSlot.SetIcon(dCOUPON_ICON_NAME, 0);
	m_CouponSlot.SetPosition_fromParent(122, 175);

	// 구분선
	m_srfTMQContourLine_1.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSideContourLine" ));
	m_srfTMQContourLine_2.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSideContourLine" ));
	m_srfTMQContourLine_3.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSideContourLine" ));

	m_srfTMQContourLine_1.SetPositionfromParent(0, 112);
	m_srfTMQContourLine_2.SetPositionfromParent(0, 154);
	m_srfTMQContourLine_3.SetPositionfromParent(0, 217);

	// 쿠폰 슬롯 배경
	m_srfCouponSlot.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "TMQStatus.srf", "srfCouponSlot" ));
	m_srfCouponSlot.SetPositionfromParent(120, 173);

	m_pTMQQuestContainer = NTL_NEW CLittleQuestContainer;
	if( !m_pTMQQuestContainer->Create(eQUEST_SORT_TYPE_TIMEMACHINE, m_pQuestTitle, m_pQuestAim_1, m_pQuestAim_2,
									  m_pQuestAim_3, m_pQuestAim_4) )
		NTL_RETURN(FALSE);

	m_TimeBonus.bShow		= FALSE;

	GetNtlGuiManager()->AddUpdateFunc( this );

	// sig	
	m_slotMove			= m_pThis->SigMove().Connect( this, &CTMQStatusGui::OnMove );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CTMQStatusGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CTMQStatusGui::OnMouseLeave );	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CTMQStatusGui::OnPaint );
	m_slotPostPaint		= m_pStageButton->SigPaint().Connect( this, &CTMQStatusGui::OnPostPaint );

	Show(false);	

	LinkMsg(g_EventCharReady);
	LinkMsg(g_EventTMQNotify);

	NTL_RETURN(TRUE);
}

VOID CTMQStatusGui::Destroy()
{
	NTL_FUNCTION("CTMQStatusGui::Destroy");

	CheckInfoWindow();
	GetNtlGuiManager()->RemoveUpdateFunc( this );

	if( m_pTMQQuestContainer )
	{
		m_pTMQQuestContainer->Destroy();
		NTL_DELETE(m_pTMQQuestContainer);
	}

	m_CouponSlot.Destroy();

	UnLinkMsg(g_EventCharReady);
	UnLinkMsg(g_EventTMQNotify);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTMQStatusGui::CreateInstance()
{
	if( m_pInstance )
		return;

	m_pInstance = NTL_NEW CTMQStatusGui("TMQRewardGui");
	if( !m_pInstance->Create() )
		NTL_DELETE(m_pInstance);

	CNtlPLGuiManager::GetInstance()->AddGui(m_pInstance);
	GetSideDialogManager()->RegistDialog( SDIALOG_TMQ_STATUS, m_pInstance, &CTMQStatusGui::SwitchDialog );
}

VOID CTMQStatusGui::DestroyInstance()
{
	if( !m_pInstance )
		return;

	GetSideDialogManager()->UnRegistDialog(SDIALOG_TMQ_STATUS);
	GetNtlGuiManager()->RemoveGui(m_pInstance);
	m_pInstance->Destroy();
	NTL_DELETE(m_pInstance);
}

VOID CTMQStatusGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	// 남은 시간
	CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
	//sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = pTMQ->GetTMQTable();
	//sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &pTIMEQUEST_TBLDAT->sTimeQuestDataset[pTMQ->GetDifficult_for_RegRoom()];
	RwReal fRemainTime = pTMQ->GetRemainTime_in_TMQ();
	RwInt32 iMinute = (RwInt32)fRemainTime/60;
	RwInt32 iSecond = (RwInt32)fRemainTime%60;

	m_LeftTimeMinuteNumber.SetNumber(iMinute);
	m_LeftTimeSecondNumber.SetNumber(iSecond);

	m_SenarioButtonFlicker.Update(fElapsed);
	m_StageButtonFlicker.Update(fElapsed);

	// time bonus
	if( m_TimeBonus.bShow )
	{
		RwInt32 iYPos;
		CRectangle rtScreen = m_pThis->GetScreenRect();

		m_TimeBonus.fElapsed += fElapsed;

		if( m_TimeBonus.fElapsed > dTIME_BONUS_SHOWTIME )
			m_TimeBonus.bShow = FALSE;

		// 숫자를 보여주는 동안 부모 다이얼로그가 계속 움직일 수가 있어서 매번 위치를 정해준다
		iYPos = dTIME_BONUS_DEFAULT_Y - (RwInt32)(dTIME_BONUS_DISTANCE * m_TimeBonus.fElapsed / dTIME_BONUS_SHOWTIME);
		m_TimeBonus.TimeBonusNum.SetPosition(rtScreen.left - 10, rtScreen.top + iYPos);
		m_TimeBonus.srfTimeBonus.SetPosition(rtScreen.left - 10 - m_TimeBonus.srfTimeBonus.GetWidth(),
											 rtScreen.top + iYPos - m_TimeBonus.srfTimeBonus.GetHeight() - 3);
		// 3 : 숫자와의 간격조절 조절		
	}
}

VOID CTMQStatusGui::SetTMQPoint(RwInt32 iPoint)
{
	RwInt32 iCurPoint = iPoint % DBO_TIMEQUEST_COUNPON_CHANGE_POINT;

	// m_pCurPoint(최대값이 100 기준), m_pTMQGauge(최대값이 1000 기준)을 백분율로 만든다
	m_pCurPoint->Format(L"%i%%", iCurPoint/10);
	m_pTMQGauge->SetPos(iCurPoint);
}

VOID CTMQStatusGui::ShowBonusTime(RwReal fBonusTime)
{
	m_TimeBonus.fElapsed		= 0.f;
	m_TimeBonus.bShow			= TRUE;
	m_TimeBonus.TimeBonusNum.SetNumber((RwInt32)fBonusTime);
}

VOID CTMQStatusGui::CheckInfoWindow()
{
	if( GetInfoWndManager()->GetRequestGui() == DIALOG_TMQBOARD )
		GetInfoWndManager()->ShowInfoWindow(FALSE);
}

VOID CTMQStatusGui::OnClicked_MinimizeButton(gui::CComponent* pComponent)
{
	CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_MINIMIZE, SDIALOG_TMQ_STATUS, DIALOG_SIDEDIALOG_CONTROLLER);
}

VOID CTMQStatusGui::OnClicked_SenarioButton(gui::CComponent* pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlTMQ* pTMQ = pAvatar->GetTMQ();
	sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = pTMQ->GetTMQTable();
	if( !pTIMEQUEST_TBLDAT )
	{
		DBO_FAIL("Not exist TMQ table");
		return;
	}

	RwUInt32 uiFirstNum = pTIMEQUEST_TBLDAT->tblidx;

	m_SenarioButtonFlicker.EndProc();

	// 시나리오는 2번째 value값이 0이다
	CDboEventGenerator::QuestMessage(uiFirstNum, 0);
}

VOID CTMQStatusGui::OnClicked_InfoButton(gui::CComponent* pComponent)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlTMQ* pTMQ = pAvatar->GetTMQ();
	sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = pTMQ->GetTMQTable();
	if( !pTIMEQUEST_TBLDAT )
	{
		DBO_FAIL("Not exist TMQ table");
		return;
	}

	RwUInt32 uiFirstNum = pTIMEQUEST_TBLDAT->tblidx;

	m_StageButtonFlicker.EndProc();

	CDboEventGenerator::QuestMessage(uiFirstNum, pTMQ->GetStageNum());
}

VOID CTMQStatusGui::OnClicked_NextButton(gui::CComponent* pComponent)
{
	m_pTMQQuestContainer->NextQuest();
}

VOID CTMQStatusGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtRect;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_LeftTimeMinuteNumber.SetParentPos(rtScreen.left, rtScreen.top);
	m_LeftTimeSecondNumber.SetParentPos(rtScreen.left, rtScreen.top);
	m_srfRemainSemiColon.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_RecordTimeMinuteNumber.SetParentPos(rtScreen.left, rtScreen.top);
	m_RecordTimeSecondNumber.SetParentPos(rtScreen.left, rtScreen.top);
	m_srfRecordSemiColon.SetPositionbyParent(rtScreen.left, rtScreen.top);

	rtRect = m_pSenarioButton->GetPosition();
	m_SenarioButtonFlicker.SetCenterPos(rtScreen.left + rtRect.left + rtRect.GetWidth()/2,
										rtScreen.top + rtRect.top + rtRect.GetHeight()/2);

	rtRect = m_pStageButton->GetPosition();
	m_StageButtonFlicker.SetCenterPos(rtScreen.left + rtRect.left + rtRect.GetWidth()/2,
									  rtScreen.top + rtRect.top + rtRect.GetHeight()/2);

	m_CouponSlot.SetParentPosition(rtScreen.left, rtScreen.top);
	m_srfGaugeBack.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_srfTMQContourLine_1.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfTMQContourLine_2.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfTMQContourLine_3.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_srfCouponSlot.SetPositionbyParent(rtScreen.left, rtScreen.top);

	CheckInfoWindow();
}

VOID CTMQStatusGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_CouponSlot.PtInRect(nX, nY) && m_CouponSlot.IsOnlyUsableIcon() )
	{
		if( GetInfoWndManager()->GetInfoWndState() != CInfoWndManager::INFOWND_JUST_WTEXT )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_JUST_WTEXT,
												rtScreen.left + m_CouponSlot.GetX_fromParent(),
												rtScreen.top + m_CouponSlot.GetY_fromParent(),
												(VOID*)GetDisplayStringManager()->GetString(DST_TMQ_STATUS_EXPLAIN_COUPON), DIALOG_TMQBOARD );
		}
		
		return;
	}

	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTMQStatusGui::OnMouseLeave(gui::CComponent* pComponent)
{
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CTMQStatusGui::OnPaint()
{
	m_srfCouponSlot.Render();

	m_LeftTimeMinuteNumber.Render();
	m_LeftTimeSecondNumber.Render();
	m_srfRemainSemiColon.Render();

	m_RecordTimeMinuteNumber.Render();
	m_RecordTimeSecondNumber.Render();
	m_srfRecordSemiColon.Render();

	if( m_CouponSlot.GetCount() > 0 )
		m_CouponSlot.Paint();

	if( m_TimeBonus.bShow )
	{
		m_TimeBonus.TimeBonusNum.Render();
		m_TimeBonus.srfTimeBonus.Render();
	}

	m_srfGaugeBack.Render();

	m_srfTMQContourLine_1.Render();
	m_srfTMQContourLine_2.Render();
	m_srfTMQContourLine_3.Render();	
}

VOID CTMQStatusGui::OnPostPaint()
{
	m_SenarioButtonFlicker.Render();
	m_StageButtonFlicker.Render();
}

RwInt32 CTMQStatusGui::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_TIME_MACHINE) == FALSE )
			return -1;

		CNtlWorldConceptTMQ* pTMQWorldConcept = reinterpret_cast<CNtlWorldConceptTMQ*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TIME_MACHINE) );
		if( !pTMQWorldConcept )
		{
			DBO_FAIL("Not exist TMQ world concept");
			return -1;
		}

		if( pTMQWorldConcept->GetTMQState() < WORLD_STATE_TMQ_WAIT ||
			pTMQWorldConcept->GetTMQState() >= WORLD_STATE_TMQ_END )
			return -1;

		Show(true);
	}
	else
	{
		Show(false);
		CheckInfoWindow();
	}

	return 1;
}

VOID CTMQStatusGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTMQStatusGui::HandleEvents");

	if( msg.Id == g_EventCharReady )
	{
		if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_TIME_MACHINE) == FALSE )
			NTL_RETURNVOID();


		if( GetSideDialogManager()->IsOpenDialog(SDIALOG_TMQ_STATUS) == FALSE )
		{
			CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
			if( !pTMQ )
			{
				DBO_FAIL("Not exist TMQ table");
				NTL_RETURNVOID();
			}

			RwUInt32 uiMinute = (RwUInt32)pTMQ->GetLastDayRecord()/60;
			RwUInt32 uiSecond = (RwUInt32)pTMQ->GetLastDayRecord()%60;
			
			m_RecordTimeMinuteNumber.SetNumber(uiMinute);
			m_RecordTimeSecondNumber.SetNumber(uiSecond);

			// TMQ 스테이터스에서 TMQ 관련창을 연다
			GetSideDialogManager()->OpenDialog(SDIALOG_TMQ_STATUS);
			GetDialogManager()->OpenDialog(DIALOG_TELECAST);
		}
	}
	else if( msg.Id == g_EventTMQNotify )
	{
		SNtlEventTMQNotify* pEvent = reinterpret_cast<SNtlEventTMQNotify*>( msg.pData );

		switch(pEvent->uiEventType)
		{
		case TET_INFOMATION_NFY:
			{
				if( pEvent->uiValue == TIMEQUEST_INFORMATION_TYPE_SCENARIO )
				{
					OnClicked_SenarioButton(NULL);
					m_SenarioButtonFlicker.StartProc(TRUE);
				}
				else
				{
					OnClicked_InfoButton(NULL);
					m_StageButtonFlicker.StartProc(TRUE);
				}

				break;
			}
		case TET_COUPON_COUNT:
			{
				m_CouponSlot.SetCount(pEvent->uiValue);
				break;
			}
		case TET_TMQ_POINT:
			{
				SetTMQPoint(pEvent->uiValue);
				break;
			}
		case TET_GAME_TIME:
			{
				ShowBonusTime((RwReal)pEvent->uiValue);
				break;
			}
		}
	}

	NTL_RETURNVOID();
}