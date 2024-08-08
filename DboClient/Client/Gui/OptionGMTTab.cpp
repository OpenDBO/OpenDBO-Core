#include "precomp_dboclient.h"

// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// Table
#include "TableContainer.h"
#include "GameManiaTimeTable.h"

// Simulation
#include "NtlSLApi.h"
#include "NtlSLGlobal.h"

// dbo
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "DboEvent.h"

// Option
#include "OptionWindowGui.h"
#include "OptionBase.h"
#include "OptionGMTTab.h"
#include "AlarmManager.h"
#include "DboEventGenerator.h"

COptionGMT::COptionGMT()
{
	m_pThis = NULL;
	m_pStbGameManiaTime = NULL;
	m_pStbGameManiaTimeProfile = NULL;
	m_pStbExGet = NULL;
	m_ePresentIndex = GPL_NUMS;
	m_uiResetCount = 0;

	// 초기화 중
	for( RwInt32 i = GPL_1; i < GPL_NUMS; ++i )
	{
		m_apBtnGPL[i] = NULL;
		m_adwGmtPresentList[i] = INVALID_TBLIDX;
	}

	m_pStbRemainCount = NULL;
	m_pBtnSetting = NULL;

	m_pStbManiaCurrentTime = NULL;
	m_pStbManiaChangeTime = NULL;

	for( RwInt32 i= GPT_1; i < GPT_NUMS; ++i )
	{
		m_apStbCurrentTime[i];
		m_apStbTo[i];
		m_apStbChangeTime[i];
	}

	memset( m_asGmtGuiInfo, 0xFF, sizeof( sGameManiaTimeGuiInfo ) * GPL_NUMS );

	m_eUIMode = GMT_UI_NORMAL;
}

COptionGMT::~COptionGMT()
{

}

RwBool COptionGMT::Create( COptionWindowGui* pOptionWindow )
{
	NTL_FUNCTION("COptionGMT::Create");
	COptionBase::Create( pOptionWindow );

	m_pThis = (gui::CDialog*)GetComponent("dlgGMT");

	// 게임 매니아 타임 타이틀
	m_pStbGameManiaTime = (gui::CStaticBox*)GetComponent("stbGameManiaTime");

	// 게임 매니아 타임 프로필
	m_pStbGameManiaTimeProfile	= (gui::CStaticBox*)GetComponent("stbGameManiaTimeProfile");

	// 추가 획득
	m_pStbExGet	= (gui::CStaticBox*)GetComponent("stbExGet" );
	
	m_pStbGameManiaTime->SetText( GetDisplayStringManager()->GetString( DST_GMT_TITLE ) );
	m_pStbGameManiaTimeProfile->SetText( GetDisplayStringManager()->GetString( DST_GMT_PROFILE ) );
	m_pStbExGet->SetText( GetDisplayStringManager()->GetString( DST_GMT_EXT_GET ) );
	
	for( RwInt32 i=0; i < GPL_NUMS; ++i )
	{
		char acBuffer[64];
		sprintf_s(acBuffer, 64, "btnList_%d", i+1 );
		m_apBtnGPL[i] = (gui::CButton*)GetComponent( acBuffer );
		m_slotToggledGPL[i] = m_apBtnGPL[i]->SigToggled().Connect( this, &COptionGMT::OnToggledGMTButton );
	}

	// 남은 재설정 횟수
	m_pStbRemainCount = (gui::CStaticBox*)GetComponent("stbRemainCount");
	
	// 재설정 버튼
	m_pBtnSetting = (gui::CButton*)GetComponent("btnSetting");
	m_pBtnSetting->SetText( GetDisplayStringManager()->GetString( DST_GMT_RESET ) );
	m_slotClickedBtnSetting = m_pBtnSetting->SigClicked().Connect( this, &COptionGMT::OnClickedSetting );
	
	// 적용 시간 & 변경될 시간
	m_pStbManiaCurrentTime = (gui::CStaticBox*)GetComponent("stbManiaCurrentTime");
	m_pStbManiaChangeTime	= (gui::CStaticBox*)GetComponent("stbManiaChangeTime");

	m_pStbManiaCurrentTime->SetText( GetDisplayStringManager()->GetString( DST_GMT_APPLY_TIME ) );
	m_pStbManiaChangeTime->SetText( GetDisplayStringManager()->GetString( DST_GMT_CHANGE_TIME ) );

	for( RwInt32 i=0; i < GPT_NUMS; ++i )
	{
		char acStartTime[32];
		sprintf_s( acStartTime, 32, "stbManiaCurrentTime%d", i+1 );
		m_apStbCurrentTime[i] = (gui::CStaticBox*)GetComponent(acStartTime);

		char acTo[32];
		sprintf_s( acTo, 32, "stbTo%d", i+1 );
		m_apStbTo[i] = (gui::CStaticBox*)GetComponent(acTo);
		m_apStbTo[i]->SetText( L"▶" );

		char acEndTime[32];
		sprintf_s( acEndTime, 32, "stbManiaChangeTime%d", i+1 );
		m_apStbChangeTime[i] = (gui::CStaticBox*)GetComponent(acEndTime);
	}

	InitGMTList();
	UpdateUIMode( m_eUIMode );

	NTL_RETURN(TRUE);
}

void COptionGMT::Destroy()
{
}

void COptionGMT::Show()
{
	m_pThis->Show(true);

	UpdateUIMode( m_eUIMode );
}

void COptionGMT::Hide()
{
	m_pThis->Show(false);
}

void COptionGMT::OnInit()
{

}

void COptionGMT::OnReset()
{

}

void COptionGMT::OnOk()
{

}

void COptionGMT::OnCancel()
{

}

void COptionGMT::OnHandleEvents( RWS::CMsg &pMsg )
{
	if( pMsg.Id == g_EventGmtUpdate )
	{
		SDboEventGmtUpdate* pData = reinterpret_cast<SDboEventGmtUpdate*>(pMsg.pData);

		m_uiResetCount--;
		UpdateResetCount();

		for( RwUInt8 i=GPL_1; i < GPL_NUMS; ++i )
		{
			m_asGmtNextInfo[i].dwTblIdx = i;
	
			// 무도회 테이블
			sGMT_TBLDAT* pTblDat = (sGMT_TBLDAT*)API_GetTableContainer()->GetGameManiaTimeTable()->FindData( i );
			if( !pTblDat )
				continue;

			m_asGmtNextInfo[i].byEffectDurrationTime = pTblDat->byEffectTerm;
			m_asGmtNextInfo[i].byEffectType = pTblDat->byEffectType;
			m_asGmtNextInfo[i].byEffectValue = pTblDat->byEffectValue;
			m_asGmtNextInfo[i].byEffectTimeNum = pTblDat->byMaxTimeNumber;
			m_asGmtNextInfo[i].dwSetTime = 0;
			
			for( RwUInt8 j=0; j < GMT_MAX_TIME_SLOT_SIZE; ++j )
			{
				m_asGmtNextInfo[i].abySetTime[j] = pData->sNext.abyTimeSlot[i][j];
			}
		}

		m_eUIMode = GMT_UI_CHANGE;
		UpdateUIMode( m_eUIMode );

		SetDownGMT( m_ePresentIndex );
	}
	else if ( pMsg.Id == g_EventGmtChanged )
	{
		SDboEventGmtChanged* pData = reinterpret_cast<SDboEventGmtChanged*>(pMsg.pData);

		m_eUIMode = GMT_UI_NORMAL;
		UpdateUIMode( m_eUIMode );

		for( RwUInt8 i=GPL_1; i < GPL_NUMS; ++i )
		{
			m_asGmtNextInfo[i].dwTblIdx = i;

			// 무도회 테이블
			sGMT_TBLDAT* pTblDat = (sGMT_TBLDAT*)API_GetTableContainer()->GetGameManiaTimeTable()->FindData( i );
			if( !pTblDat )
				continue;

			m_asGmtNextInfo[i].byEffectDurrationTime = pTblDat->byEffectTerm;
			m_asGmtNextInfo[i].byEffectType = pTblDat->byEffectType;
			m_asGmtNextInfo[i].byEffectValue = pTblDat->byEffectValue;
			m_asGmtNextInfo[i].byEffectTimeNum = pTblDat->byMaxTimeNumber;
			m_asGmtNextInfo[i].dwSetTime = 0;

			for( RwUInt8 j=0; j < GMT_MAX_TIME_SLOT_SIZE; ++j )
			{
				m_asGmtNextInfo[i].abySetTime[j] = pData->sCurrent.abyTimeSlot[i][j];
			}
		}

		SetDownGMT( m_ePresentIndex );
	}
}

void COptionGMT::InitGMTList()
{
	SGameManiaTimeInfo* pGMTInfo = GetNtlSLGlobal()->GetGameManiaTimeInfo();
	if( !pGMTInfo )
	{
		DBO_FAIL( "COptionGMT::InitGMTList - Failed getting game mania time info" );
		return;
	}

	m_uiResetCount = pGMTInfo->byGMTResetCount;

	UpdateResetCount();

	RwInt32 nListCount = 0;
	for( RwInt32 i = GPL_1; i < GPL_NUMS; ++i )
	{
		// 유효한 Index를 검사하고 
		sGMT_TBLDAT* pTblDat = (sGMT_TBLDAT*)API_GetTableContainer()->GetGameManiaTimeTable()->FindData( i );
		if( pTblDat )
		{
			// 세팅해준다.
			m_adwGmtPresentList[nListCount] = i;
			m_apBtnGPL[nListCount]->ClearText();

			WCHAR awcBuffer[128];
			if( pTblDat->byEffectType == ET_EXP )
			{
				swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( DST_GMT_LIST_EXP ), pTblDat->byEffectValue );
			}
			else if( pTblDat->byEffectType == ET_ZENNY )
			{	
				swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( DST_GMT_LIST_ZENNY ), pTblDat->byEffectValue );
			}

			m_apBtnGPL[nListCount]->SetText( awcBuffer );
			m_apBtnGPL[nListCount]->ClickEnable( TRUE );
			m_apBtnGPL[nListCount]->Show( true );

			// 정보를 구성해준다.
			m_asGmtGuiInfo[nListCount].dwTblIdx = i;

			// NtlSLGLobal에서 정보를 받아온다.
			m_asGmtGuiInfo[nListCount].byEffectDurrationTime = pTblDat->byEffectTerm;
			m_asGmtGuiInfo[nListCount].byEffectType = pTblDat->byEffectType;
			m_asGmtGuiInfo[nListCount].byEffectValue = pTblDat->byEffectValue;
			m_asGmtGuiInfo[nListCount].byEffectTimeNum = pTblDat->byMaxTimeNumber;
			m_asGmtGuiInfo[nListCount].dwSetTime = pGMTInfo->sCurrent.dwSetTime;

			m_asGmtNextInfo[nListCount].dwTblIdx = i;

			m_asGmtNextInfo[nListCount].byEffectDurrationTime = pTblDat->byEffectTerm;
			m_asGmtNextInfo[nListCount].byEffectType = pTblDat->byEffectType;
			m_asGmtNextInfo[nListCount].byEffectValue = pTblDat->byEffectValue;
			m_asGmtNextInfo[nListCount].byEffectTimeNum = pTblDat->byMaxTimeNumber;
			m_asGmtNextInfo[nListCount].dwSetTime = pGMTInfo->sNext.dwSetTime;

			for( RwInt32 j=0; j < GMT_MAX_TIME_SLOT_SIZE; ++j )
			{
				m_asGmtGuiInfo[nListCount].abySetTime[j] = pGMTInfo->sCurrent.abyTimeSlot[i][j];
				m_asGmtNextInfo[nListCount].abySetTime[j] = pGMTInfo->sNext.abyTimeSlot[i][j];
			}
			
			nListCount++;
		}
	}

	for( RwInt32 i = nListCount; i < GPL_NUMS; ++i )
	{
		m_apBtnGPL[i]->Show( false );
	}

	// 초기에는 하나만 세팅한다.
	if( m_apBtnGPL[GPL_1]->IsVisible() )
	{
		SetDownGMT( GPL_1 );
	}
}

void COptionGMT::UpdateUIMode( eGMT_MODE eUIMode )
{
	switch( eUIMode )
	{
	case GMT_UI_NORMAL:
		{
			m_pStbManiaCurrentTime->SetPosition( 110, 279 );
			m_apStbCurrentTime[GPT_1]->SetPosition( 88, 299 );
			m_apStbCurrentTime[GPT_2]->SetPosition( 88, 322 );
			m_apStbCurrentTime[GPT_3]->SetPosition( 88, 345 );

			m_pStbManiaChangeTime->Show( false );

			for( RwInt32 i=GPT_1; i < GPT_NUMS; ++i )
			{
				m_apStbTo[i]->Show(false);
				m_apStbChangeTime[i]->Show(false);
			}
		}
		break;
	case GMT_UI_CHANGE:
		{
			m_pStbManiaCurrentTime->SetPosition( 40, 279 );
			m_pStbManiaChangeTime->SetPosition( 175, 279 );
			m_apStbCurrentTime[GPT_1]->SetPosition( 18, 299 );
			m_apStbCurrentTime[GPT_2]->SetPosition( 18, 322 );
			m_apStbCurrentTime[GPT_3]->SetPosition( 18, 345 );
			m_apStbTo[GPT_1]->SetPosition( 122, 299 );
			m_apStbTo[GPT_2]->SetPosition( 122, 322 );
			m_apStbTo[GPT_3]->SetPosition( 122, 345 );
			m_apStbChangeTime[GPT_1]->SetPosition( 149, 299 );
			m_apStbChangeTime[GPT_2]->SetPosition( 149, 322 );
			m_apStbChangeTime[GPT_3]->SetPosition( 149, 345 );

			m_pStbManiaChangeTime->Show(true);

			for( RwInt32 i=GPT_1; i < GPT_NUMS; ++i )
			{
				m_apStbTo[i]->Show(true);
				m_apStbChangeTime[i]->Show(true);
			}
		}
		break;
	default:
		break;
	}
}

void COptionGMT::UpdateResetCount()
{
	// Reset Count 세팅
	WCHAR awcBuffer[256];
	swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( DST_GMT_REMAIN_COUNT ), m_uiResetCount );
	m_pStbRemainCount->SetText( awcBuffer );

}

void COptionGMT::SetDownGMT( eGMT_PRESENT_LIST eList )
{
	if( GPL_1 <= eList && GPL_5 >= eList )
	{
		m_ePresentIndex = eList;

		if( m_apBtnGPL[eList]->IsVisible() )
		{
			m_apBtnGPL[eList]->SetDown( true );
			m_apBtnGPL[eList]->ClickEnable( FALSE );

			// UI Mode 지정
			if( m_asGmtNextInfo[eList].dwSetTime != INVALID_DWORD )
			{
				m_eUIMode = GMT_UI_CHANGE;
			}

			for( int i=0; i < GMT_MAX_TIME_SLOT_SIZE; ++i )
			{
				if( m_asGmtGuiInfo[eList].abySetTime[i] != INVALID_BYTE )
				{
					WCHAR awcBuffer[32];

					RwUInt8 byStartTime = m_asGmtGuiInfo[eList].abySetTime[i];
					RwUInt8 byEndTime = (m_asGmtGuiInfo[eList].abySetTime[i] + m_asGmtGuiInfo[eList].byEffectDurrationTime) % 24;

					WCHAR awcStartTime[32];
					swprintf_s( awcStartTime, 32, GetDisplayStringManager()->GetString( DST_GMT_TIME_INFO ), byStartTime );

					WCHAR awcEndTime[32];
					swprintf_s( awcEndTime, 32, GetDisplayStringManager()->GetString( DST_GMT_TIME_INFO), byEndTime );

					swprintf_s( awcBuffer, 32, L"%s ~ %s", awcStartTime, awcEndTime );
					m_apStbCurrentTime[i]->SetText( awcBuffer );
				}
				else
				{
					m_apStbCurrentTime[i]->Clear();
					m_apStbCurrentTime[i]->SetText( GetDisplayStringManager()->GetString( DST_GMT_NO_TIME ) );
				}
				
				if( m_asGmtNextInfo[eList].abySetTime[i] != INVALID_BYTE )
				{
					WCHAR awcBuffer[32];

					RwUInt8 byStartTime = m_asGmtNextInfo[eList].abySetTime[i];
					RwUInt8 byEndTime = (m_asGmtNextInfo[eList].abySetTime[i] + m_asGmtNextInfo[eList].byEffectDurrationTime) % 24;

					WCHAR awcStartTime[32];
					swprintf_s( awcStartTime, 32, GetDisplayStringManager()->GetString( DST_GMT_TIME_INFO ), byStartTime );

					WCHAR awcEndTime[32];
					swprintf_s( awcEndTime, 32, GetDisplayStringManager()->GetString( DST_GMT_TIME_INFO), byEndTime );

					swprintf_s( awcBuffer, 32, L"%s ~ %s", awcStartTime, awcEndTime );
					m_apStbChangeTime[i]->SetText( awcBuffer );
				}
				else
				{
					m_apStbChangeTime[i]->Clear();
					m_apStbChangeTime[i]->SetText( GetDisplayStringManager()->GetString( DST_GMT_NO_TIME ) );
				}
			}
		}
	}

	for( RwInt32 i=GPL_1; i < GPL_NUMS; ++i )
	{
		if( i == eList )
			continue;
		
		m_apBtnGPL[i]->ClickEnable( TRUE );
		m_apBtnGPL[i]->SetDown( false );
	}
}

void COptionGMT::OnToggledGMTButton( gui::CComponent* pComponent, bool bToggled )
{
	for( RwUInt8 i=GPL_1; i < GPL_NUMS; ++i )
	{
		if( m_apBtnGPL[i] == (gui::CButton*)pComponent )
		{
			SetDownGMT( (eGMT_PRESENT_LIST)i );
		}
		else
		{
			if( m_apBtnGPL[i]->IsVisible() )
			{
				m_apBtnGPL[i]->SetDown( false );
				m_apBtnGPL[i]->ClickEnable( TRUE );
			}
		}
	}
}

void COptionGMT::OnClickedSetting( gui::CComponent* pComponent )
{
	if( GetDialogManager()->IsOpenDialog( DIALOG_GMT_RESET ) )
	{
		GetAlarmManager()->AlarmMessage( DST_GMT_RESET_ALREADY_OPEN  );
		return;
	}

	if( m_uiResetCount > 0 )
	{
		CDboEventGenerator::GmtResetOpen( TRUE, pComponent->GetScreenRect().left, pComponent->GetScreenRect().top );
	}
}