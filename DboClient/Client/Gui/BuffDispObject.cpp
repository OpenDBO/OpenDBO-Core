#include "precomp_dboclient.h"
#include "BuffDispObject.h"

// core
#include "NtlDebug.h"

// shared
#include "NtlObject.h"
#include "SystemEffectTable.h"
#include "SkillTable.h"
#include "ItemTable.h"
#include "UseItemTable.h"
#include "TableContainer.h"

// gui
#include "gui_renderer.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSobManager.h"
#include "NtlSobBuff.h"
#include "NtlSobBuffAttr.h"
#include "NtlBuffContainer.h"
#include "NtlSobIcon.h"
#include "NtlSobFeelingActor.h"

// dbo
#include "DboGlobal.h"
#include "DboPacketGenerator.h"
#include "InfoWndManager.h"

#define BUFF_ROW_MARGIN			1		// 버프 사이간의 거리
#define BUFF_COL_MARGIN			1	
#define BUFF_MAX_DISP_COUNT		8		// 보여지는 버프 최대 수
#define BUFF_UPDATE_TIME		1.0f

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CBuff
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID CBuff::SetMove( RwInt32 nScreenX, RwInt32 nScreenY )
{
	m_Surface.SetPosition( nScreenX + m_Rect.left, nScreenY + m_Rect.top );
	m_KeepTimeEffect.SetPosition( nScreenX + m_Rect.left, nScreenY + m_Rect.top );
}

VOID CBuff::SetRect( RwInt32 nX, RwInt32 nSize, gui::CSurface& KeepTimeSurface )
{
	m_Rect.left = nX;
	m_Rect.top = 0;
	m_Rect.right = nX + nSize;
	m_Rect.bottom = nSize;

	m_Surface.SetRect( m_Rect );
	
	m_KeepTimeEffect.SetSurface( KeepTimeSurface );	
}

VOID CBuff::Render(VOID)
{
	if( m_eType != TYPE_NONE )
	{
		m_Surface.Render();
		m_KeepTimeEffect.Render();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CSobBuff
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID CSobBuff::SetBuff( void* pBuff, TYPE eType )
{
	CNtlSobBuff* pSobBuff = reinterpret_cast<CNtlSobBuff*>( pBuff );

	if( pSobBuff )
	{
		m_pBuff = pSobBuff;
		m_eType = eType;
		m_Surface.SetTexture( reinterpret_cast<gui::CTexture*>( pSobBuff->GetIcon()->GetImage() ) );
		m_KeepTimeEffect.StartProc( m_pBuff->GetIcon()->GetMaxCoolingTime(), TRUE, TRUE );
	}
	else
	{
		m_pBuff = NULL;
		m_eType = TYPE_NONE;
		m_KeepTimeEffect.EndProc();
	}
}

VOID CSobBuff::DropBuff(VOID)
{
	if( m_eType != TYPE_NONE )
	{
		CNtlSobBuffAttr* pBuffAttr = reinterpret_cast<CNtlSobBuffAttr*>( m_pBuff->GetSobAttr() );
		
		if( pBuffAttr->IsBlessType() )
			GetDboGlobal()->GetGamePacketGenerator()->SendBuffDropReq( pBuffAttr->GetBuffType(), pBuffAttr->GetTblIdx() );
	}
}

VOID CSobBuff::ShowInfoWnd( RwInt32 nParentX, RwInt32 nParentY, RwInt32 nDlgType )
{
	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_BUFF, m_Rect.left + nParentX, m_Rect.top + nParentY, m_pBuff, nDlgType );
}

VOID CSobBuff::Update( RwReal fElapsedTime )
{
	if( m_eType != TYPE_NONE )
	{
		m_KeepTimeEffect.Update( m_pBuff->GetIcon()->GetCoolingTime() );
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CFakeBuff
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID CFakeBuff::SetBuff( void* pBuff, TYPE eType )
{
	sNtlFakeBuff* pFakeBuff = reinterpret_cast<sNtlFakeBuff*>( pBuff );

	if( pFakeBuff )
	{
		m_pBuff = pFakeBuff;
		m_eType = eType;
		m_Surface.SetTexture( reinterpret_cast<gui::CTexture*>( pFakeBuff->pImage ) );
		m_KeepTimeEffect.StartProc( pFakeBuff->fMaxTime, TRUE, TRUE );
	}
	else
	{
		m_pBuff = NULL;
		m_eType = TYPE_NONE;
		m_KeepTimeEffect.EndProc();
	}
}

VOID CFakeBuff::ShowInfoWnd( RwInt32 nParentX, RwInt32 nParentY, RwInt32 nDlgType )
{
	GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_FAKEBUFF, m_Rect.left + nParentX, m_Rect.top + nParentY, m_pBuff, nDlgType );
}

VOID CFakeBuff::Update( RwReal fElapsedTime )
{
	if( m_eType != TYPE_NONE )
	{
		m_KeepTimeEffect.Update( m_pBuff->fTime );
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CBuffDispBar
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CBuffDispBar::CBuffDispBar(VOID)
: m_pDialog( NULL ), m_ppArrBuff( NULL ), m_nBuffCnt( 0 ), m_nMouseOnIndex( -1 )
{

}

CBuffDispBar::~CBuffDispBar(VOID)
{
	Destroy();
}

VOID CBuffDispBar::Create( RwInt32 nBuffCount, eDialogType eDlgType, RwInt32 nIconSize, RwInt32 eBuffOrigin )
{
	NTL_FUNCTION( "CBuffDispBar:Create" );

	m_nBuffCnt = nBuffCount;

	m_ppArrBuff = NTL_NEW CBuff*[nBuffCount];

	if( eBuffOrigin == CBuffDispGui::SOBBUFF )
	{
		for( RwInt32 i = 0 ; i < nBuffCount ; ++i )
			m_ppArrBuff[i] = NTL_NEW CSobBuff;
	}
	else
	{
		for( RwInt32 i = 0 ; i < nBuffCount ; ++i )
			m_ppArrBuff[i] = NTL_NEW CFakeBuff;
	}

	m_eDlgType = eDlgType;

	m_pDialog = NTL_NEW gui::CDialog( CRectangle( 0, 0, 0, 0 ), GetNtlGuiManager()->GetGuiManager(), GetNtlGuiManager()->GetSurfaceManager() );
	NTL_ASSERT( m_pDialog, "CBuffDispBar::Create : m_pDialog is not allocated, Not enough Memory!" );
	
	if( m_eDlgType == DIALOG_HP )
		m_slotMouseDown = m_pDialog->SigMouseDown().Connect( this, &CBuffDispBar::OnMouseDown );
	m_slotMouseMove = m_pDialog->SigMouseMove().Connect( this, &CBuffDispBar::OnMouseMove );
	m_slotMouseLeave = m_pDialog->SigMouseLeave().Connect( this, &CBuffDispBar::OnMouseLeave );
	m_slotMove = m_pDialog->SigMove().Connect( this, &CBuffDispBar::OnMove );
	m_slotPaint = m_pDialog->SigPaint().Connect( this, &CBuffDispBar::OnPaint );

	SetBuffRect( nIconSize );

	NTL_RETURNVOID();
}

VOID CBuffDispBar::Destroy(VOID)
{
	if( m_ppArrBuff )
	{
		for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
		{
			NTL_DELETE( m_ppArrBuff[i] );
		}

		NTL_ARRAY_DELETE( m_ppArrBuff );		
	}
	
	if( m_pDialog )
		NTL_DELETE( m_pDialog );
}

VOID CBuffDispBar::Update( RwReal fElapsedTime )
{
	for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
	{
		m_ppArrBuff[i]->Update( fElapsedTime );
	}
}

VOID CBuffDispBar::SetBuff( VOID* pSobBuff, CBuff::TYPE eType, RwInt32 nSlot )
{
	m_ppArrBuff[nSlot]->SetBuff( pSobBuff, eType );
}

VOID CBuffDispBar::SetMove( RwInt32 nScreenX, RwInt32 nScreenY )
{
	m_pDialog->SetPosition( nScreenX, nScreenY );
}

VOID CBuffDispBar::SetSize( RwInt32 nWidth, RwInt32 nHeight )
{
	m_pDialog->SetSize( nWidth, nHeight );
}
						 
VOID CBuffDispBar::Show( bool bShow )
{
	m_pDialog->Show( bShow );
	
	if( !bShow )
		OnMouseLeave( NULL );
}

VOID CBuffDispBar::RefreshInfoWnd(VOID)
{
	if( m_nMouseOnIndex < 0 )
		return;
	
	if( m_ppArrBuff[m_nMouseOnIndex]->m_eType != CBuff::TYPE_NONE )
	{
		CRectangle rtScreen = m_pDialog->GetScreenRect();
		m_ppArrBuff[m_nMouseOnIndex]->ShowInfoWnd( rtScreen.left, rtScreen.top, m_eDlgType );		
	}		
	else
	{
		if( GetInfoWndManager()->GetRequestGui() == m_eDlgType )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
	}
	
}

VOID CBuffDispBar::SetBuffRect( RwInt32 nIconSize )
{
	gui::CSurface KeepTimeSurface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "GameCommon.srf", "srfSlotGrayedEffect" );
	KeepTimeSurface.m_Original.rtRect.right = KeepTimeSurface.m_Original.rtRect.left + nIconSize;
	KeepTimeSurface.m_Original.rtRect.bottom = KeepTimeSurface.m_Original.rtRect.top + nIconSize;

	for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
	{
		m_ppArrBuff[i]->SetRect( i * ( nIconSize + BUFF_ROW_MARGIN ), nIconSize, KeepTimeSurface );
	}
}

VOID CBuffDispBar::OnMouseDown( const CKey& key )
{
	for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
	{
		if( m_ppArrBuff[i]->m_Rect.PtInRect( (RwInt32)key.m_fX, (RwInt32)key.m_fY ) )
		{
			m_ppArrBuff[i]->DropBuff();
		}
	}
}

VOID CBuffDispBar::OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos )
{
	for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
	{
		if( m_ppArrBuff[i]->m_Rect.PtInRect( nXPos, nYPos ) )
		{
			if( m_nMouseOnIndex == i )
				return;

			if( m_ppArrBuff[i]->m_eType != CBuff::TYPE_NONE )
			{
				CRectangle rtScreen = m_pDialog->GetScreenRect();
				m_nMouseOnIndex = i;
				m_ppArrBuff[m_nMouseOnIndex]->ShowInfoWnd( rtScreen.left, rtScreen.top, m_eDlgType );				
			}			
			return;
		}
	}

	if( m_nMouseOnIndex != -1 )
	{
		if( GetInfoWndManager()->GetRequestGui() == m_eDlgType )
			GetInfoWndManager()->ShowInfoWindow( FALSE );
		
		m_nMouseOnIndex = -1;
	}
}

VOID CBuffDispBar::OnMouseLeave( gui::CComponent* pComponent )
{
	if( m_nMouseOnIndex != -1 )
	{
		if( GetInfoWndManager()->GetRequestGui() == m_eDlgType )
			GetInfoWndManager()->ShowInfoWindow( FALSE );

		m_nMouseOnIndex = -1;
	}
}

VOID CBuffDispBar::OnMove( RwInt32 nX, RwInt32 nY )
{
	CRectangle rtScreen = m_pDialog->GetScreenRect();

	for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
	{
		m_ppArrBuff[i]->SetMove( rtScreen.left, rtScreen.top );		
	}
}

VOID CBuffDispBar::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < m_nBuffCnt ; ++i )
	{
		m_ppArrBuff[i]->Render();
	}
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ! CBuffDispGui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CBuffDispGui::CBuffDispGui(VOID)
: m_eType( TYPE_2X8 ), m_eIconSize( NORMAL_ICONSIZE ), m_pArrBuffBar( NULL ), 
  m_nOffsetX( 0 ), m_nOffsetY( 0 ), m_nRowCnt( 0 ), m_nColCnt( 0 ), m_fUpdateInfoTime( 0.0f ),
  m_bEnable( FALSE )
{
	
}

CBuffDispGui::~CBuffDispGui(VOID)
{
	Destroy();
}

VOID CBuffDispGui::Create( CBuffDispGui::TYPE eType, CBuffDispGui::ICONSIZE eIconSize, RwInt32 nOffsetX, RwInt32 nOffsetY, eDialogType eDlgType, ORIGIN eOrigin )
{
	NTL_FUNCTION( "CBuffDispGui::Create" );

	m_eType = eType;
	m_eIconSize = eIconSize;
	
	m_nOffsetX = nOffsetX;
	m_nOffsetY = nOffsetY;

	switch( eType )
	{
	case TYPE_2X8: m_nRowCnt = 2; m_nColCnt = 8; break;
	case TYPE_4X4: m_nRowCnt = 4; m_nColCnt = 4; break;
	default: NTL_ASSERTFAIL( "CBuffDispGui::Create : Wrong BuffGui Type!" );
	}

	m_pArrBuffBar = NTL_NEW CBuffDispBar[m_nRowCnt];
	NTL_ASSERT( m_pArrBuffBar, "CBuffDispGui::Create : BuffBar Alloc Fail, Not Enougn Memory" );

	for( RwInt32 i = 0 ; i < m_nRowCnt ; ++i )
	{
		m_pArrBuffBar[i].Create( m_nColCnt, eDlgType, eIconSize, eOrigin );
	}

	NTL_RETURNVOID();
}

VOID CBuffDispGui::Destroy(VOID)
{
	if( !m_pArrBuffBar )
		return;

	for( RwInt32 i = 0 ; i < m_nRowCnt ; ++i )
	{
		m_pArrBuffBar[i].Destroy();								
	}

	NTL_ARRAY_DELETE( m_pArrBuffBar );	
}

VOID CBuffDispGui::Update( RwReal fElapsedTime )
{
	if( !m_bEnable || !m_bShow )
		return;

	RwInt32 i;

	for( i = 0 ; i < m_nRowCnt ; ++i )
	{
		m_pArrBuffBar[i].Update( fElapsedTime );
	}

	m_fUpdateInfoTime += fElapsedTime;

	if( m_fUpdateInfoTime > BUFF_UPDATE_TIME )
	{
		for( i = 0 ; i < m_nRowCnt ; ++i )
			m_pArrBuffBar[i].RefreshInfoWnd();
	}
}

VOID CBuffDispGui::SetBuffAll( RwUInt32 hSerial )
{
	CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( hSerial );
	NTL_ASSERT( pSobObj, "CBuffDispBar::SetBuffAll : Invalid Serial ID" );

	RwInt32 eClassID = pSobObj->GetClassID();

	if( eClassID == SLCLASS_AVATAR ||
		eClassID == SLCLASS_PLAYER ||
		eClassID == SLCLASS_MONSTER ||
		eClassID == SLCLASS_PET )		
	{
		BuffRegisterProc( reinterpret_cast<CNtlSobFeelingActor*>( pSobObj )->GetBuffContainer() );		
		m_bEnable = TRUE;
	}
	else
	{
		ClearBuff();
	}
}

VOID CBuffDispGui::SetBuffAll( CNtlFakeBuffContainer* pFakeBuffContainer )
{
	BuffRegisterProc( pFakeBuffContainer );
	m_bEnable = TRUE;
}

VOID CBuffDispGui::ClearBuff(VOID)
{
	BuffUnregisterProc();
	m_bEnable = FALSE;
}

VOID CBuffDispGui::SetMove( CRectangle& rtScreen )
{
	for( RwInt32 i = 0 ; i < m_nRowCnt ; ++i )
	{
		m_pArrBuffBar[i].SetMove( rtScreen.right + m_nOffsetX, rtScreen.top + m_nOffsetY + i * ( m_eIconSize + BUFF_ROW_MARGIN ) );
	}
}

VOID CBuffDispGui::Show( bool bShow )
{
	for( RwInt32 i = 0 ; i < m_nRowCnt ; ++i )
	{
		m_pArrBuffBar[i].Show( bShow );
	}

	m_bShow = bShow;
}

VOID CBuffDispGui::BuffRegisterProc( CNtlBuffContainer* pBuffContainer )
{
	CNtlBuffContainer::ListBuff::iterator it;
	RwInt32 nBuffIdx = 0, nDeBuffIdx = 0, i;

	for( it = pBuffContainer->GetBeginBuff() ; it != pBuffContainer->GetEndBuff() ; ++it )
	{
		CNtlSobBuff* pBuff = (*it);
		CNtlSobBuffAttr* pBuffAttr = reinterpret_cast<CNtlSobBuffAttr*>( pBuff->GetSobAttr() );
		BUFFATTR eBuffAttrType = CURSE_BUFF;
		CBuff::TYPE eBuffType = CBuff::TYPE_NONE;

		if( pBuffAttr->GetBuffType() == DBO_OBJECT_SOURCE_SKILL )
			eBuffType = CBuff::TYPE_SKILL;
		else if( pBuffAttr->GetBuffType() == DBO_OBJECT_SOURCE_ITEM )
			eBuffType = CBuff::TYPE_ITEM;

		if( pBuffAttr->IsBlessType() )
			eBuffAttrType = BLESS_BUFF;
		
		if( eBuffAttrType == BLESS_BUFF )
		{
			RegisterBuffToSlot( pBuff, eBuffAttrType, eBuffType, nBuffIdx );
			++nBuffIdx;
		}
		else if( eBuffAttrType == CURSE_BUFF )
		{
			RegisterBuffToSlot( pBuff, eBuffAttrType, eBuffType, nDeBuffIdx );
			++nDeBuffIdx;
		}
	}

	for( i = nBuffIdx ; i < BUFF_MAX_DISP_COUNT ; ++i )
		RegisterBuffToSlot( NULL, BLESS_BUFF, CBuff::TYPE_NONE, i );

	for( i = nDeBuffIdx; i < BUFF_MAX_DISP_COUNT ; ++i )
		RegisterBuffToSlot( NULL, CURSE_BUFF, CBuff::TYPE_NONE, i );

	FitBuffBarRect( nBuffIdx, nDeBuffIdx );

	for( i = 0 ; i < m_nRowCnt ; ++i )
	{
		m_pArrBuffBar[i].RefreshInfoWnd();
	}
}

VOID CBuffDispGui::BuffRegisterProc( CNtlFakeBuffContainer* pFakeBuffContainer )
{
	LIST_FAKE_BUFF_ITER it;
	RwInt32 nBuffIdx = 0, nDeBuffIdx = 0, i;

	for( it = pFakeBuffContainer->GetBeginBuff() ; it != pFakeBuffContainer->GetEndBuff() ; ++it )
	{
		sNtlFakeBuff* pFakeBuff = (*it);
		BUFFATTR eBuffAttrType = CURSE_BUFF;
		CBuff::TYPE eBuffType = CBuff::TYPE_NONE;
		
		if( pFakeBuff->eType == DBO_OBJECT_SOURCE_SKILL )
			eBuffType = CBuff::TYPE_SKILL;
		else if( pFakeBuff->eType == DBO_OBJECT_SOURCE_ITEM )
			eBuffType = CBuff::TYPE_ITEM;

		if( pFakeBuff->bBlessType )
			eBuffAttrType = BLESS_BUFF;

		if( eBuffAttrType == BLESS_BUFF )
		{
			RegisterBuffToSlot( pFakeBuff, eBuffAttrType, eBuffType, nBuffIdx );
			++nBuffIdx;
		}
		else if( eBuffAttrType == CURSE_BUFF )
		{
			RegisterBuffToSlot( pFakeBuff, eBuffAttrType, eBuffType, nDeBuffIdx );
			++nDeBuffIdx;
		}
	}

	for( i = nBuffIdx ; i < BUFF_MAX_DISP_COUNT ; ++i )
		RegisterBuffToSlot( NULL, BLESS_BUFF, CBuff::TYPE_NONE, i );

	for( i = nDeBuffIdx; i < BUFF_MAX_DISP_COUNT ; ++i )
		RegisterBuffToSlot( NULL, CURSE_BUFF, CBuff::TYPE_NONE, i );

	FitBuffBarRect( nBuffIdx, nDeBuffIdx );

	for( i = 0 ; i < m_nRowCnt ; ++i )
	{
		m_pArrBuffBar[i].RefreshInfoWnd();
	}
}

VOID CBuffDispGui::BuffUnregisterProc(VOID)
{
	RwInt32 i = 0;

	for( i = 0 ; i < BUFF_MAX_DISP_COUNT ; ++i )
		RegisterBuffToSlot( NULL, BLESS_BUFF, CBuff::TYPE_NONE, i );

	for( i = 0 ; i < BUFF_MAX_DISP_COUNT ; ++i )
		RegisterBuffToSlot( NULL, CURSE_BUFF, CBuff::TYPE_NONE, i );

	FitBuffBarRect( 0, 0 );

	for( i = 0 ; i < m_nRowCnt ; ++i )
	{
		m_pArrBuffBar[i].RefreshInfoWnd();
	}	
}

VOID CBuffDispGui::RegisterBuffToSlot( VOID* pBuff, BUFFATTR eBuffAttrType, CBuff::TYPE eBuffOriginType, RwInt32 nIndex )
{
	RwInt32 nRow, nCol;
	
	if( eBuffAttrType == BLESS_BUFF )
		nRow = nIndex / m_nColCnt;
	else 
		nRow = ( nIndex + BUFF_MAX_DISP_COUNT ) / m_nColCnt;
	
	nCol = nIndex % m_nColCnt;
	
	if( nRow >= m_nRowCnt ) // Buff Overflow
		return;	

	m_pArrBuffBar[nRow].SetBuff( pBuff, eBuffOriginType, nIndex );
}

VOID CBuffDispGui::FitBuffBarRect( RwInt32 nBlessBuffCnt, RwInt32 nCurseBuffCnt )
{
	RwInt32 nBlessRow, nBlessCol, nCurseRow, nCurseCol, i; 

	nBlessRow = nBlessBuffCnt / m_nColCnt;
	nBlessCol = nBlessBuffCnt % m_nColCnt;

	nCurseRow = nCurseBuffCnt / m_nColCnt;
	nCurseCol = nCurseBuffCnt % m_nColCnt;

	for( i = 0 ; i < ( m_nRowCnt / 2 ) ; ++i )
	{
		if( nBlessRow > i )
			m_pArrBuffBar[i].SetSize( m_nColCnt * ( m_eIconSize + BUFF_COL_MARGIN ), m_eIconSize );
		else if( nBlessRow == i )
			m_pArrBuffBar[i].SetSize( nBlessCol * ( m_eIconSize + BUFF_COL_MARGIN ), m_eIconSize );
		else
			m_pArrBuffBar[i].SetSize( 0, 0 );
	}

	for( i = 0 ; i < ( m_nRowCnt / 2 ) ; ++i )
	{
		if( nCurseRow > i )
			m_pArrBuffBar[i+(m_nRowCnt/2)].SetSize( m_nColCnt * ( m_eIconSize + BUFF_COL_MARGIN ), m_eIconSize );
		else if( nBlessRow == i )
			m_pArrBuffBar[i+(m_nRowCnt/2)].SetSize( nCurseCol * ( m_eIconSize + BUFF_COL_MARGIN ), m_eIconSize );
		else
			m_pArrBuffBar[i+(m_nRowCnt/2)].SetSize( 0, 0 );
	}
}