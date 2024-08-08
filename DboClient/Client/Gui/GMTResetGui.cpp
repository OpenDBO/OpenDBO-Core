#include "precomp_dboclient.h"
#include "GMTResetGui.h"

// core
#include "NtlDebug.h"

// Simulation
#include "NtlSLApi.h"

// Presentation
#include "NtlPLGuiManager.h"

// Table
#include "TableContainer.h"
#include "GameManiaTimeTable.h"

// Client
#include "OptionGMTTab.h"
#include "DboGlobal.h"
#include "DialogManager.h"

//////////////////////////////////////////////////////////////////////////
// GMT SetTime

CGMTSetTime::CGMTSetTime()
{
	m_byTimeNum = INVALID_BYTE;
	m_byEnableCheckNum = INVALID_BYTE;
	m_tblidx = INVALID_TBLIDX;
	m_pParentGui = NULL;

	m_ppStbTime = NULL;
	m_ppBtnCheck = NULL;
}

CGMTSetTime::~CGMTSetTime()
{

}

RwBool CGMTSetTime::Create( gui::CDialog* pParentGui, RwUInt32 idxGmt )
{
	m_pParentGui = pParentGui;

	sGMT_TBLDAT* pTblData = (sGMT_TBLDAT*)API_GetTableContainer()->GetGameManiaTimeTable()->FindData( idxGmt );
	if( !pTblData )
		return FALSE;
	
	if( !CreateHardCode( pTblData->byEffectTerm ) )
		return FALSE;
	
	// Text Setting
	// 유효한 Index를 검사하고 
	sGMT_TBLDAT* pTblDat = (sGMT_TBLDAT*)API_GetTableContainer()->GetGameManiaTimeTable()->FindData( idxGmt );
	if( !pTblDat )
		return FALSE;

	m_tblidx = idxGmt;

	RwInt32 nStartTime = 0;
	for( RwInt8 i=0; i < m_byTimeNum;++i )
	{
		WCHAR awcStartTime[32];
		swprintf_s( awcStartTime, 32, GetDisplayStringManager()->GetString( DST_GMT_TIME_INFO ), nStartTime );

		WCHAR awcEndTime[32];
		swprintf_s( awcEndTime, 32, GetDisplayStringManager()->GetString( DST_GMT_TIME_INFO), nStartTime + pTblDat->byEffectTerm );

		WCHAR awcBuffer[64];
		swprintf_s( awcBuffer, 64, L"%s ~ %s", awcStartTime, awcEndTime );

	
		m_ppStbTime[i]->SetText(awcBuffer);

		nStartTime += pTblDat->byEffectTerm;
	}

	// 체크 할 수 있는 갯수
	m_byEnableCheckNum = pTblData->byMaxTimeNumber;

	return TRUE;
}

RwBool CGMTSetTime::CreateHardCode(RwUInt8 byDurrationTime)
{
	RwInt32 nStbFirstX = 0;
	RwInt32 nStbSecondX = 135;
	RwInt32 nStbY = 0;
	RwInt32 nStbWidth = 97;
	RwInt32 nStbHeight = 19;

	RwInt32 nBtnFirstX = 98;
	RwInt32 nBtnSecondX = 230;
	RwInt32 nBtnWidth = 14;
	RwInt32 nBtnHeight = 14;

	switch( byDurrationTime )
	{
	case 2:
		m_ppStbTime = m_apStbTime2;
		m_ppBtnCheck = m_apBtnCheck2;
		m_byTimeNum = 12;
		break;
	case 3:
		m_ppStbTime = m_apStbTime3;
		m_ppBtnCheck = m_apBtnCheck3;
		m_byTimeNum = 8;
		break;
	case 4:
		m_ppStbTime = m_apStbTime4;
		m_ppBtnCheck = m_apBtnCheck4;
		m_byTimeNum = 6;
		break;
	case 6:
		m_ppStbTime = m_apStbTime6;
		m_ppBtnCheck = m_apBtnCheck6;
		m_byTimeNum = 4;
		break;
	default:
		return FALSE;
	}

	CRectangle rectThis;
	rectThis.SetRectWH( 0, 0, 246, 117 );
	m_pThis = NTL_NEW gui::CDialog( &rectThis, m_pParentGui, GetNtlGuiManager()->GetSurfaceManager() ); 

	CRectangle rectStb;
	CRectangle rectBtn;
	for( RwUInt8 i=0 ; i < m_byTimeNum; ++i )
	{
		if( ( i % 2) == 0 )
		{
			rectStb.SetRectWH( nStbFirstX, nStbY, nStbWidth, nStbHeight );
			rectBtn.SetRectWH( nBtnFirstX, nStbY + 4, nBtnWidth, nBtnHeight );
		}
		else
		{
			// Y 좌표값을 증가시켜준다.
			rectStb.SetRectWH( nStbSecondX, nStbY, nStbWidth, nStbHeight );
			rectBtn.SetRectWH( nBtnSecondX, nStbY + 4, nBtnWidth, nBtnHeight );

			nStbY = nStbY + nStbHeight;
		}

		m_ppStbTime[i] = NTL_NEW gui::CStaticBox( &rectStb, m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
		m_ppStbTime[i]->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
		/*m_ppStbTime[i]->SetText( m_byTimeNum );*/
		
		m_ppBtnCheck[i] = NTL_NEW gui::CButton( rectBtn, m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
		m_ppBtnCheck[i]->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GMTResetGui.srf", "srfCheckBtnUp" ) );
		m_ppBtnCheck[i]->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GMTResetGui.srf", "srfCheckBtnDown" ) );
		m_ppBtnCheck[i]->AddSurfaceDisabled( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GMTResetGui.srf", "srfCheckBtnUp" ) );
		m_ppBtnCheck[i]->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GMTResetGui.srf", "srfCheckBtnUp" ) );
		m_ppBtnCheck[i]->SetToggleMode( true );

		m_slotToggledBtnCheck[i] = m_ppBtnCheck[i]->SigToggled().Connect( this, &CGMTSetTime::OnToggledBtnCheck );
	}

	return TRUE;
}

void CGMTSetTime::Destroy()
{
	if( m_byTimeNum != INVALID_BYTE )
	{
		for( RwUInt32 i=0; i < m_byTimeNum; ++i )
		{
			NTL_DELETE( m_ppStbTime[i] );
			NTL_DELETE( m_ppBtnCheck[i] );
		}
	}
}

void CGMTSetTime::Show( RwBool bShow )
{
	m_pThis->Show( B2b(bShow) );
}

void CGMTSetTime::OnToggledBtnCheck( gui::CComponent* pComponent, bool bToggled )
{
	if( bToggled == true )
	{
		if( GetCountCheckNum() > m_byEnableCheckNum )
		{
			GetAlarmManager()->AlarmMessage( DST_GMT_RESET_COUNT_OVER );
			gui::CButton* pButton = (gui::CButton*)pComponent;
			pButton->SetDown( false );
		}
	}
}

void CGMTSetTime::Init()
{
	for( RwUInt8 i=0; i < m_byTimeNum; ++i )
	{
		m_ppBtnCheck[i]->SetDown( false );
	}
}

RwBool CGMTSetTime::IsDone()
{
	int nCount = 0;
	for( RwUInt8 i = 0; i < m_byTimeNum; ++i )
	{
		if( m_ppBtnCheck[i]->IsDown() )
		{
			nCount++;
		}
	}

	// 눌려진 버튼의 갯수와 선택해야 하는 갯수가 맞아야지 완료된 것이다.
	return (nCount == m_byEnableCheckNum);
}

RwUInt8 CGMTSetTime::GetCountCheckNum()
{
	RwUInt8 nCount = 0;
	for( RwUInt8 i = 0; i < m_byTimeNum; ++i )
	{
		if( m_ppBtnCheck[i]->IsDown() )
		{
			nCount++;
		}
	}

	// 눌려진 버튼의 갯수와 선택해야 하는 갯수가 맞아야지 완료된 것이다.
	return nCount;
}

RwUInt8 CGMTSetTime::GetEnableCheckNum()
{
	return m_byEnableCheckNum;
}

void CGMTSetTime::GetCheckTimeIndex( std::vector<RwUInt8>& vecTimeIdx )
{
	for( RwUInt8 i=0; i < m_byTimeNum; ++i )
	{
		if( m_ppBtnCheck[i]->IsDown() )
		{
			vecTimeIdx.push_back( ((24 / m_byTimeNum) * i ) );			
		}
	}
}

RwUInt32 CGMTSetTime::GetTblIdx()
{
	return m_tblidx;
}

///////////////////////////////////////////////////////////////////////////
// GMT Reset Gui

CGMTResetGui::CGMTResetGui( const RwChar* pName )
: CNtlPLGui(pName)
{
	m_pStbGMTResetTitle = NULL;
	m_pStbExtGet = NULL;
	m_pCbbGMTList = NULL;
	m_pBtnClose = NULL;
	m_pStbRemainCount = NULL;
	m_pStbSetTime = NULL;

	m_pDlgSetTime = NULL;

	for( RwUInt8 i = 0; i < GMT_MAX_EFFECT_LIST_SIZE; ++i )
	{
		m_apGMTSetTime[i] = NULL;
	}
}

CGMTResetGui::~CGMTResetGui()
{

}

RwBool CGMTResetGui::Create()
{
	NTL_FUNCTION("CGMTResetGui::Create");

	if (!CNtlPLGui::Create("", "gui\\GMTResetGui.srf", "gui\\GMTResetGui.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pThis->SetPriority( dDIALOGPRIORITY_MANIA_TIME_SET );

	m_pStbGMTResetTitle = (gui::CStaticBox*)GetComponent("stbManiaTimeResetTitle");
	m_pStbGMTResetTitle->SetText( GetDisplayStringManager()->GetString( DST_GMT_RESET_TITLE ) );
	m_pStbExtGet = (gui::CStaticBox*)GetComponent("stbExtGet");
	m_pStbExtGet->SetText( GetDisplayStringManager()->GetString( DST_GMT_EXT_GET ) );
	m_pCbbGMTList = (gui::CComboBox*)GetComponent("cbbGMTList" );
	m_slotListToggled = m_pCbbGMTList->SigListToggled().Connect( this, &CGMTResetGui::OnListToggledGMT );
	m_slotListSelected = m_pCbbGMTList->SigSelected().Connect( this, &CGMTResetGui::OnSelectedGMT );
	


	m_pBtnClose = (gui::CButton*)GetComponent("btnClose");
	m_slotClickedBtnClose = m_pBtnClose->SigClicked().Connect( this, &CGMTResetGui::OnClickedBtnCancel );
	m_pStbRemainCount = (gui::CStaticBox*)GetComponent("stbRemainCount");
	
	// 남은 재설정 횟수 넣어야함

	m_pStbSetTime = (gui::CStaticBox*)GetComponent("stbSetTime");
	m_pStbSetTime->SetText( GetDisplayStringManager()->GetString( DST_GMT_APPLY_TIME ) );

	m_pDlgSetTime = (gui::CDialog*)GetComponent("dlgSetTime" );

	m_pStbTimeNum = (gui::CStaticBox*)GetComponent("stbTimeNum");

	m_pBtnOK = (gui::CButton*)GetComponent("btnOk");
	m_slotClickedBtnOK = m_pBtnOK->SigClicked().Connect( this, &CGMTResetGui::OnClickedBtnOk );
	m_pBtnOK->SetText( GetDisplayStringManager()->GetString( DST_MSG_BTN_OK ) );
	m_pBtnCancel = (gui::CButton*)GetComponent("btnCancel");
	m_slotClickedBtnCancel = m_pBtnCancel->SigClicked().Connect( this, &CGMTResetGui::OnClickedBtnCancel );
	m_pBtnCancel->SetText( GetDisplayStringManager()->GetString( DST_MSG_BTN_CANCEL ) );

	// GMT의 SetTime을 구성
	// 테이블이 몇개나 유효한지 확인한다.

	int nCount = 0;
	for( RwUInt8 i = 0; i < GMT_MAX_EFFECT_LIST_SIZE; ++i )
	{
		sGMT_TBLDAT* pTblData = (sGMT_TBLDAT*)API_GetTableContainer()->GetGameManiaTimeTable()->FindData( i );
		if( pTblData )
		{
			m_apGMTSetTime[nCount] = NTL_NEW CGMTSetTime();
			if( !m_apGMTSetTime[nCount]->Create( m_pDlgSetTime, i ) )
			{
				m_apGMTSetTime[nCount]->Destroy();
				NTL_RETURN(FALSE);
			}

			WCHAR awcBuffer[128];
			if( pTblData->byEffectType == COptionGMT::ET_EXP )
			{
				swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( DST_GMT_LIST_EXP ), pTblData->byEffectValue );
			}
			else if( pTblData->byEffectType == COptionGMT::ET_ZENNY )
			{	
				swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( DST_GMT_LIST_ZENNY ), pTblData->byEffectValue );
			}

			m_pCbbGMTList->AddItem( awcBuffer );

			nCount++;
		}
	}

	Show( false );

	if( nCount > 0 )
	{
		SelectGMT( 0 );
	}

	LinkMsg( g_EventGmtResetOpen, 0 );

	NTL_RETURN(TRUE);
}

void CGMTResetGui::Destroy()
{
	NTL_FUNCTION("CGMTResetGui::Destroy");

	UnLinkMsg( g_EventGmtResetOpen );

	for( RwUInt8 i = 0; i < GMT_MAX_EFFECT_LIST_SIZE; ++i )
	{
		if( m_apGMTSetTime[i] )
		{
			m_apGMTSetTime[i]->Destroy();
			NTL_DELETE( m_apGMTSetTime[i] );
		}
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CGMTResetGui::Update( RwReal fElapsed )
{
	
}

RwInt32 CGMTResetGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{
		for( RwUInt8 i=0; i < GMT_MAX_EFFECT_LIST_SIZE; ++i )
		{
			if( m_apGMTSetTime[i] != NULL )
			{
				m_apGMTSetTime[i]->Init();
			}
		}
	}

	return 1;
}

void CGMTResetGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventGmtResetOpen )
	{
		SDboEventGmtResetOpen* pData = reinterpret_cast<SDboEventGmtResetOpen*>(msg.pData);

		if( pData->bOpen )
		{
			SetPosPopup( pData->nLeft, pData->nTop );
			GetDialogManager()->OpenDialog( DIALOG_GMT_RESET );
		}
	}
}

void CGMTResetGui::SelectGMT( RwUInt8 byIndex )
{
	if( m_apGMTSetTime[byIndex] == NULL )
	{
		DBO_FAIL( "CGMTResetGui::SelectGMT - Failed to select gmt index" << byIndex );
		return;
	}

	m_pCbbGMTList->SelectItem( byIndex, true );

	for( RwUInt8 i = 0; i < GMT_MAX_EFFECT_LIST_SIZE; ++i )
	{
		if( m_apGMTSetTime[i] != NULL )
		{
			if( byIndex == i )
			{
				m_apGMTSetTime[i]->Show( true );
				WCHAR awcBuffer[64];
				swprintf_s( awcBuffer, 64, GetDisplayStringManager()->GetString( DST_GMT_ENABLE_CHECK_COUNT ), m_apGMTSetTime[i]->GetEnableCheckNum() );
				m_pStbTimeNum->SetText( awcBuffer );
			}
			else
				m_apGMTSetTime[i]->Show( false );
		}
	}
}

void CGMTResetGui::SetPosPopup( RwInt32 nX, RwInt32 nY )
{
	int nNewX, nNewY;
	if( nX > (RwInt32)(GetDboGlobal()->GetScreenWidth()/2) )
	{
		nNewX = nX - m_pThis->GetWidth() - 40;
	}
	else
	{
		nNewX = nX + 40;
	}

	if( nY > (RwInt32)(GetDboGlobal()->GetScreenHeight()/2) )
	{
		nNewY = nY - m_pThis->GetHeight() - 20;
	}
	else
	{
		nNewY = nY + 20;
	}

	m_pThis->SetPosition( nNewX , nNewY );
}

void CGMTResetGui::OnSelectedGMT( RwInt32 nSelectIndex )
{
	SelectGMT( (RwUInt8)nSelectIndex );
}

void CGMTResetGui::OnListToggledGMT( BOOL bToggled, gui::CComponent* pComponent )
{
	if (bToggled)
	{
		pComponent->Raise();
	}
	else
	{
		pComponent->Lower();
	}
}

void CGMTResetGui::OnClickedBtnOk( gui::CComponent* pComponent )
{
	// 갯수를 체크하여 모두 완료가 안되었으면 리턴한다.
	RwBool bDone = TRUE;
	for( RwUInt8 i = 0; i < GMT_MAX_EFFECT_LIST_SIZE; ++i )
	{
		if( m_apGMTSetTime[i] != NULL )
		{
			if( !m_apGMTSetTime[i]->IsDone() )
				bDone = FALSE;
		}
	}

	if( bDone )
	{
		// 완료 되었습니다.
		// 데이타 긁어오기
		sGAME_MANIA_TIME sData;
		memset( &sData, 0xFF, sizeof( sGAME_MANIA_TIME ) );

		for( RwUInt8 i=0; i < GMT_MAX_EFFECT_LIST_SIZE; ++i )
		{
			if( m_apGMTSetTime[i] != NULL )
			{
				std::vector<RwUInt8> vecIdx;

				m_apGMTSetTime[i]->GetCheckTimeIndex( vecIdx );

				RwUInt8 byCount = 0;
				for each( RwUInt8 byIdx in vecIdx )
				{
					sData.abyTimeSlot[m_apGMTSetTime[i]->GetTblIdx()][byCount++] = byIdx;
					if( byCount > GMT_MAX_TIME_SLOT_SIZE )
						break;
				}
			}
		}

		GetDboGlobal()->GetGamePacketGenerator()->SendGmtUpdateReq( sData );

		GetDialogManager()->CloseDialog( DIALOG_GMT_RESET );

		// 패킷 보내기
	}
	else
	{
		GetAlarmManager()->AlarmMessage( DST_GMT_DONOT_CHECK_ALL );
	}
}

void CGMTResetGui::OnClickedBtnCancel( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_GMT_RESET );
}