#include "precomp_dboclient.h"
#include "ItemUpgradeCardGui.h"

// core
#include "NtlDebug.h"

// shared
#include "NtlItem.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// Dbo
#include "DboGlobal.h"
#include "AlarmManager.h"
#include "DialogPriority.h"
#include "BackboardGui.h"
#include "ItemUpgradeGui.h"

#define ITEMUPGRADECARD_DROP_FLASHFILE		"upgrade_drop.swf"
#define ITEMUPGRADECARD_SUCCESS_FLASHFILE	"upgrade_Success.swf"
#define ITEMUPGRADECARD_FAIL_FLASHFILE		"upgrade_failed.swf"
#define ITEMUPGRADECARD_BROKEN_FLASHFILE	"upgrade_broken.swf"

static const RwChar* s_aszResultFlash[] = { ITEMUPGRADECARD_SUCCESS_FLASHFILE, ITEMUPGRADECARD_FAIL_FLASHFILE, ITEMUPGRADECARD_BROKEN_FLASHFILE };

#define ITEMUPGRADECARD_CARD_Y				83
#define ITEMUPGRADECARD_DROP_OFFSET_X		130
#define ITEMUPGRADECARD_DROP_Y				-25
#define ITEMUPGRADECARD_RESULT_Y			45
#define ITEMUPGRADECARD_EFFECT_Y			75
#define ITEMUPGRADECARD_SINGLE_SLOT			1
#define ITEMUPGRADECARD_FINISH_TIME			3.0f

// positions used by the retail client, slot 0..2 from left to right
static const RwInt32 s_anCardSlotX[]	= { 267, 444, 619 };
static const RwInt32 s_anResultSlotX[]	= { 207, 383, 559 };
static const RwInt32 s_anEffectSlotX[]	= { 260, 437, 612 };

CItemUpgradeCardGui::CItemUpgradeCardGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	m_pOwner = NULL;
	m_pBackboard = NULL;
	m_pbtnCancel = NULL;
	m_pbtnReSelect = NULL;
	m_ppnlSelectEffect = NULL;

	for( RwInt32 i = 0 ; i < NUM_CARD ; ++i )
	{
		m_pbtnCard[i] = NULL;
		m_ppnlResult[i] = NULL;
		m_pflsDrop[i] = NULL;
		m_pflsResult[i] = NULL;
		m_anCardSlot[i] = i;
	}

	m_eState = STATE_NONE;
	m_bThreeCards = TRUE;
	m_nSelectedSlot = 0;
	m_nResultCard = CARD_SUCCESS;
	m_wResultMessageCode = ITEM_UPGRADE_RESULT_SUCCESS;
	m_byPrevGrade = 0;
	m_byGrade = 0;
	m_bGuardStoneUsed = FALSE;
	m_fFinishTime = 0.0f;
	m_pflsSkipped = NULL;
	m_nSkippedIdx = 0;
}

CItemUpgradeCardGui::~CItemUpgradeCardGui(VOID)
{
}

RwBool CItemUpgradeCardGui::Create( CItemUpgradeGui* pOwner )
{
	NTL_FUNCTION( "CItemUpgradeCardGui::Create" );

	if( !CNtlPLGui::Create( "gui\\ItemUpgrade.rsr", "gui\\ItemUpgradeEx.srf", "gui\\ItemUpgradeSubEx.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_pOwner = pOwner;

	m_ppnlResult[CARD_SUCCESS] = (gui::CPanel*)GetComponent( "pnlResultSuc" );
	m_ppnlResult[CARD_FAIL] = (gui::CPanel*)GetComponent( "pnlResultFail" );
	m_ppnlResult[CARD_BROKEN] = (gui::CPanel*)GetComponent( "pnlResultBro" );

	m_pbtnCard[CARD_SUCCESS] = (gui::CButton*)GetComponent( "btnUpgradeCardSelect1" );
	m_pbtnCard[CARD_FAIL] = (gui::CButton*)GetComponent( "btnUpgradeCardSelect2" );
	m_pbtnCard[CARD_BROKEN] = (gui::CButton*)GetComponent( "btnUpgradeCardSelect3" );

	m_ppnlSelectEffect = (gui::CPanel*)GetComponent( "pnlSelectCardEffect" );
	m_pbtnCancel = (gui::CButton*)GetComponent( "btnUpgradeCancel" );
	m_pbtnReSelect = (gui::CButton*)GetComponent( "btnUpgradeReSelect" );

	m_pflsDrop[0] = (gui::CFlash*)GetComponent( "flCard1" );
	m_pflsDrop[1] = (gui::CFlash*)GetComponent( "flCard2" );
	m_pflsDrop[2] = (gui::CFlash*)GetComponent( "flCard3" );

	m_pflsResult[CARD_SUCCESS] = (gui::CFlash*)GetComponent( "flCardSuccess" );
	m_pflsResult[CARD_FAIL] = (gui::CFlash*)GetComponent( "flCardFail" );
	m_pflsResult[CARD_BROKEN] = (gui::CFlash*)GetComponent( "flCardBroken" );

	if( !m_pThis || !m_ppnlSelectEffect || !m_pbtnCancel || !m_pbtnReSelect )
		NTL_RETURN( FALSE );

	for( RwInt32 i = 0 ; i < NUM_CARD ; ++i )
	{
		if( !m_pbtnCard[i] || !m_ppnlResult[i] || !m_pflsDrop[i] || !m_pflsResult[i] )
			NTL_RETURN( FALSE );
	}

	m_slotClickCard[CARD_SUCCESS] = m_pbtnCard[CARD_SUCCESS]->SigClicked().Connect( this, &CItemUpgradeCardGui::OnClickCard1 );
	m_slotClickCard[CARD_FAIL] = m_pbtnCard[CARD_FAIL]->SigClicked().Connect( this, &CItemUpgradeCardGui::OnClickCard2 );
	m_slotClickCard[CARD_BROKEN] = m_pbtnCard[CARD_BROKEN]->SigClicked().Connect( this, &CItemUpgradeCardGui::OnClickCard3 );
	m_slotClickCancel = m_pbtnCancel->SigClicked().Connect( this, &CItemUpgradeCardGui::OnClickCancel );
	m_slotClickReSelect = m_pbtnReSelect->SigClicked().Connect( this, &CItemUpgradeCardGui::OnClickReSelect );

	m_slotDropEnd[0] = m_pflsDrop[0]->SigMovieEnd().Connect( this, &CItemUpgradeCardGui::OnDropEnd1 );
	m_slotDropEnd[1] = m_pflsDrop[1]->SigMovieEnd().Connect( this, &CItemUpgradeCardGui::OnDropEnd2 );
	m_slotDropEnd[2] = m_pflsDrop[2]->SigMovieEnd().Connect( this, &CItemUpgradeCardGui::OnDropEnd3 );
	m_slotResultEnd[CARD_SUCCESS] = m_pflsResult[CARD_SUCCESS]->SigMovieEnd().Connect( this, &CItemUpgradeCardGui::OnSuccessEnd );
	m_slotResultEnd[CARD_FAIL] = m_pflsResult[CARD_FAIL]->SigMovieEnd().Connect( this, &CItemUpgradeCardGui::OnFailEnd );
	m_slotResultEnd[CARD_BROKEN] = m_pflsResult[CARD_BROKEN]->SigMovieEnd().Connect( this, &CItemUpgradeCardGui::OnBrokenEnd );

	// the cards stay above the regular dialogs, the backboard sits right below them
	m_pThis->SetPriority( dDIALOGPRIORITY_EFDIALOG );

	m_pBackboard = NTL_NEW CBackboardGui( "ItemUpgradeCardBackboard" );
	if( !m_pBackboard->Create() )
	{
		m_pBackboard->Destroy();
		NTL_DELETE( m_pBackboard );
	}

	Show( false );

	LinkMsg( g_EventResize );

	NTL_RETURN( TRUE );
}

VOID CItemUpgradeCardGui::Destroy(VOID)
{
	UnLinkMsg( g_EventResize );

	if( m_pBackboard )
	{
		m_pBackboard->Destroy();
		NTL_DELETE( m_pBackboard );
	}

	for( RwInt32 i = 0 ; i < NUM_CARD ; ++i )
	{
		if( m_pflsDrop[i] )
			m_pflsDrop[i]->Unload();

		if( m_pflsResult[i] )
			m_pflsResult[i]->Unload();
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CItemUpgradeCardGui::Update( RwReal fElapsed )
{
	if( m_eState == STATE_NONE )
		return;

	if( m_pflsSkipped )
	{
		gui::CFlash* pSkipped = m_pflsSkipped;
		m_pflsSkipped = NULL;

		if( pSkipped == m_pflsDrop[m_nSkippedIdx] )
			OnDropEnd( m_nSkippedIdx );
		else
			OnResultEnd( m_nSkippedIdx );

		return;
	}

	for( RwInt32 i = 0 ; i < NUM_CARD ; ++i )
	{
		if( m_pflsDrop[i]->IsPlayMovie() )
			m_pflsDrop[i]->Update( fElapsed );

		if( m_pflsResult[i]->IsPlayMovie() )
			m_pflsResult[i]->Update( fElapsed );
	}

	if( m_eState == STATE_FINISH )
	{
		m_fFinishTime += fElapsed;

		if( m_fFinishTime > ITEMUPGRADECARD_FINISH_TIME )
			m_pOwner->OnCardGuiClose();
	}
}

VOID CItemUpgradeCardGui::Open( RwBool bThreeCards )
{
	m_bThreeCards = bThreeCards;

	ResetCards();

	PositionAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );

	if( m_pBackboard )
		m_pBackboard->SwitchDialog( true );

	Show( true );
	RaiseTop();

	if( m_bThreeCards )
		GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_SELECTCARD" );

	// cards are dealt one after another, the next one starts when the previous lands
	RwInt32 nFirstSlot = m_bThreeCards ? 0 : ITEMUPGRADECARD_SINGLE_SLOT;
	PlayFlash( m_pflsDrop[0], ITEMUPGRADECARD_DROP_FLASHFILE, 0, s_anCardSlotX[nFirstSlot] - ITEMUPGRADECARD_DROP_OFFSET_X, ITEMUPGRADECARD_DROP_Y );
}

VOID CItemUpgradeCardGui::Close(VOID)
{
	for( RwInt32 i = 0 ; i < NUM_CARD ; ++i )
	{
		StopFlash( m_pflsDrop[i] );
		StopFlash( m_pflsResult[i] );
	}

	m_eState = STATE_NONE;
	m_pflsSkipped = NULL;

	if( m_pBackboard )
		m_pBackboard->SwitchDialog( false );

	Show( false );
}

VOID CItemUpgradeCardGui::SetResult( WORD wResultMessageCode, BYTE byPrevGrade, BYTE byGrade, const WCHAR* pwszItemName, RwBool bGuardStoneUsed )
{
	if( m_eState != STATE_PACKETWAIT )
		return;

	m_wResultMessageCode = wResultMessageCode;
	m_byPrevGrade = byPrevGrade;
	m_byGrade = byGrade;
	m_wstrItemName = pwszItemName ? pwszItemName : L"";
	m_bGuardStoneUsed = bGuardStoneUsed;

	switch( wResultMessageCode )
	{
		case ITEM_UPGRADE_RESULT_SUCCESS:	m_nResultCard = CARD_SUCCESS;	break;
		case ITEM_UPGRADE_RESULT_FAIL:		m_nResultCard = CARD_FAIL;		break;
		default:							m_nResultCard = CARD_BROKEN;	break;
	}

	// the drawn card takes the picked slot, the card that was there moves to its old slot
	RwInt32 nPrevCard = GetCardOnSlot( m_nSelectedSlot );
	if( nPrevCard != m_nResultCard )
	{
		m_anCardSlot[nPrevCard] = m_anCardSlot[m_nResultCard];
		m_anCardSlot[m_nResultCard] = m_nSelectedSlot;
	}

	for( RwInt32 i = 0 ; i < NUM_CARD ; ++i )
	{
		StopFlash( m_pflsDrop[i] );

		m_pbtnCard[i]->SetPosition( s_anCardSlotX[m_anCardSlot[i]], ITEMUPGRADECARD_CARD_Y );

		// the cards moved under the cursor, the hover look comes back on the next mouse move
		m_pbtnCard[i]->ClearMouseFocus();

		// the drawn card is replaced by its flip animation, a single card has no others to show
		m_pbtnCard[i]->Show( i != m_nResultCard && m_bThreeCards );
	}

	m_eState = STATE_RESULT;

	PlayFlash( m_pflsResult[m_nResultCard], s_aszResultFlash[m_nResultCard], m_nResultCard, s_anResultSlotX[m_nSelectedSlot], ITEMUPGRADECARD_RESULT_Y );
}

VOID CItemUpgradeCardGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		PositionAlign( pPacket->iWidht, pPacket->iHeight );
	}
}

VOID CItemUpgradeCardGui::PositionAlign( RwInt32 nScreenWidth, RwInt32 nScreenHeight )
{
	m_pThis->SetPosition( ( nScreenWidth - m_pThis->GetWidth() ) / 2, ( nScreenHeight - m_pThis->GetHeight() ) / 2 );
}

VOID CItemUpgradeCardGui::ResetCards(VOID)
{
	m_eState = STATE_DROP;
	m_nSelectedSlot = m_bThreeCards ? 0 : ITEMUPGRADECARD_SINGLE_SLOT;
	m_fFinishTime = 0.0f;
	m_pflsSkipped = NULL;

	m_pbtnCancel->Enable( false );
	m_pbtnReSelect->Enable( false );
	m_ppnlSelectEffect->Show( false );

	for( RwInt32 i = 0 ; i < NUM_CARD ; ++i )
	{
		m_anCardSlot[i] = m_bThreeCards ? i : ITEMUPGRADECARD_SINGLE_SLOT;

		m_pbtnCard[i]->Enable( true );
		m_pbtnCard[i]->ClickEnable( TRUE );
		m_pbtnCard[i]->ClearMouseFocus();
		m_pbtnCard[i]->Show( false );
		m_pbtnCard[i]->SetPosition( s_anCardSlotX[m_anCardSlot[i]], ITEMUPGRADECARD_CARD_Y );

		m_ppnlResult[i]->Show( false );

		StopFlash( m_pflsDrop[i] );
		StopFlash( m_pflsResult[i] );
	}
}

VOID CItemUpgradeCardGui::PlayFlash( gui::CFlash* pFlash, const RwChar* pFileName, RwInt32 nIndex, RwInt32 nX, RwInt32 nY )
{
	// loaded right before playing, like the other flash dialogs do
	pFlash->SetPosition( nX, nY );

	if( !pFlash->Load( pFileName ) )
	{
		DBO_WARNING_MESSAGE( "CItemUpgradeCardGui: failed to load " << pFileName << ", its animation is skipped" );

		// a flash that is not loaded never sends its movie end, finish it on the next update
		m_pflsSkipped = pFlash;
		m_nSkippedIdx = nIndex;
		return;
	}

	pFlash->SetFrameCallback( TRUE );
	pFlash->Show( true );
	pFlash->RestartMovie();
}

VOID CItemUpgradeCardGui::StopFlash( gui::CFlash* pFlash )
{
	pFlash->PlayMovie( FALSE );
	pFlash->Show( false );
}

VOID CItemUpgradeCardGui::OnDropEnd( RwInt32 nCardIdx )
{
	if( m_eState != STATE_DROP )
		return;

	StopFlash( m_pflsDrop[nCardIdx] );
	m_pbtnCard[nCardIdx]->Show( true );

	if( !m_bThreeCards )
	{
		// nothing to pick, the only card is drawn right away
		SelectCard( ITEMUPGRADECARD_SINGLE_SLOT );
		return;
	}

	if( nCardIdx + 1 < NUM_CARD )
	{
		PlayFlash( m_pflsDrop[nCardIdx + 1], ITEMUPGRADECARD_DROP_FLASHFILE, nCardIdx + 1, s_anCardSlotX[nCardIdx + 1] - ITEMUPGRADECARD_DROP_OFFSET_X, ITEMUPGRADECARD_DROP_Y );
		return;
	}

	m_eState = STATE_SELECT;
	m_pbtnCancel->Enable( true );
}

VOID CItemUpgradeCardGui::OnResultEnd( RwInt32 nResultCard )
{
	if( m_eState != STATE_RESULT )
		return;

	StopFlash( m_pflsResult[nResultCard] );

	// flip the cards that were not drawn, their disabled surface is their face
	if( m_bThreeCards )
	{
		for( RwInt32 i = 0 ; i < NUM_CARD ; ++i )
		{
			if( i != nResultCard )
				m_pbtnCard[i]->Enable( false );
		}
	}

	m_ppnlResult[nResultCard]->SetPosition( s_anCardSlotX[m_nSelectedSlot], ITEMUPGRADECARD_CARD_Y );
	m_ppnlResult[nResultCard]->Show( true );

	m_ppnlSelectEffect->SetPosition( s_anEffectSlotX[m_nSelectedSlot], ITEMUPGRADECARD_EFFECT_Y );
	m_ppnlSelectEffect->Show( true );

	ShowResultMessage();

	if( m_bGuardStoneUsed && !m_pOwner->HasGuardStone() )
	{
		GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_EX_GUARD_STONE_ALL_USE" );
		m_pOwner->OnCardGuiClose();
		return;
	}

	m_pbtnCancel->Enable( true );

	if( m_nResultCard != CARD_BROKEN && m_pOwner->CanRetryUpgrade() )
	{
		m_pbtnReSelect->Enable( true );
		m_eState = STATE_REVEALED;
	}
	else
	{
		m_eState = STATE_FINISH;
		m_fFinishTime = 0.0f;
	}
}

VOID CItemUpgradeCardGui::SelectCard( RwInt32 nSlot )
{
	m_pbtnCancel->Enable( false );
	m_pbtnReSelect->Enable( false );

	m_nSelectedSlot = nSlot;
	m_eState = STATE_PACKETWAIT;

	m_pOwner->OnCardSelected();
}

VOID CItemUpgradeCardGui::ShowResultMessage(VOID)
{
	RwInt32 nDownValue = (RwInt32)m_byPrevGrade - (RwInt32)m_byGrade;

	switch( m_wResultMessageCode )
	{
		case ITEM_UPGRADE_RESULT_SUCCESS:
		{
			GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_SUCCESS" );
			GetAlarmManager()->FormattedAlarmMessage( "DST_ITEMUPGRADE_TEXTDATA", FALSE, NULL, m_wstrItemName.c_str(), m_byGrade );
		}
		break;
		case ITEM_UPGRADE_RESULT_FAIL:
		case ITEM_UPGRADE_RESULT_RESET:
		{
			GetAlarmManager()->AlarmMessage( "DST_ITEMUPGRADE_FAIL" );

			if( nDownValue > 0 )
				GetAlarmManager()->FormattedAlarmMessage( "DST_ITEMUPGRADE_DOWN_VALUE_MSG", FALSE, NULL, nDownValue );

			if( m_wResultMessageCode == ITEM_UPGRADE_RESULT_RESET && m_bGuardStoneUsed )
				GetAlarmManager()->AlarmMessage( "GAME_ITEM_UPGRADE_FAIL_AND_NODEL_BY_CORE" );
			else
				GetAlarmManager()->FormattedAlarmMessage( "DST_ITEMUPGRADE_TEXTDATA", FALSE, NULL, m_wstrItemName.c_str(), m_byGrade );
		}
		break;
		case ITEM_UPGRADE_RESULT_DESTROY:
		{
			if( m_bGuardStoneUsed )
				GetAlarmManager()->AlarmMessage( "GAME_ITEM_UPGRADE_FAIL_AND_NODEL_BY_CORE" );
			else
				GetAlarmManager()->FormattedAlarmMessage( "DST_ITEMUPGRADE_BROKEN", FALSE, NULL, m_wstrItemName.c_str(), m_byPrevGrade );
		}
		break;

		default: break;
	}
}

RwInt32 CItemUpgradeCardGui::GetCardOnSlot( RwInt32 nSlot )
{
	for( RwInt32 i = 0 ; i < NUM_CARD ; ++i )
	{
		if( m_anCardSlot[i] == nSlot )
			return i;
	}

	return CARD_SUCCESS;
}

VOID CItemUpgradeCardGui::OnClickCard1( gui::CComponent* pComponent )
{
	if( m_eState == STATE_SELECT )
		SelectCard( m_anCardSlot[0] );
}

VOID CItemUpgradeCardGui::OnClickCard2( gui::CComponent* pComponent )
{
	if( m_eState == STATE_SELECT )
		SelectCard( m_anCardSlot[1] );
}

VOID CItemUpgradeCardGui::OnClickCard3( gui::CComponent* pComponent )
{
	if( m_eState == STATE_SELECT )
		SelectCard( m_anCardSlot[2] );
}

VOID CItemUpgradeCardGui::OnClickCancel( gui::CComponent* pComponent )
{
	if( m_eState == STATE_DROP || m_eState == STATE_PACKETWAIT || m_eState == STATE_RESULT )
		return;

	m_pOwner->OnCardGuiClose();
}

VOID CItemUpgradeCardGui::OnClickReSelect( gui::CComponent* pComponent )
{
	if( m_eState != STATE_REVEALED )
		return;

	m_pOwner->OnCardGuiReSelect();
}

VOID CItemUpgradeCardGui::OnDropEnd1( gui::CComponent* pComponent )
{
	OnDropEnd( 0 );
}

VOID CItemUpgradeCardGui::OnDropEnd2( gui::CComponent* pComponent )
{
	OnDropEnd( 1 );
}

VOID CItemUpgradeCardGui::OnDropEnd3( gui::CComponent* pComponent )
{
	OnDropEnd( 2 );
}

VOID CItemUpgradeCardGui::OnSuccessEnd( gui::CComponent* pComponent )
{
	OnResultEnd( CARD_SUCCESS );
}

VOID CItemUpgradeCardGui::OnFailEnd( gui::CComponent* pComponent )
{
	OnResultEnd( CARD_FAIL );
}

VOID CItemUpgradeCardGui::OnBrokenEnd( gui::CComponent* pComponent )
{
	OnResultEnd( CARD_BROKEN );
}
