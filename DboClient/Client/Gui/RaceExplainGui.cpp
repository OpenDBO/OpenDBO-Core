#include "precomp_dboclient.h"
#include "RaceExplainGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"


CRaceExplainGui::CRaceExplainGui(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CRaceExplainGui::~CRaceExplainGui()
{

}

RwBool CRaceExplainGui::Create()
{
	NTL_FUNCTION( "CRaceExplainGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\CharCreate_Explain.srf", "gui\\CharCreate_Race.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 배경
	m_Background.SetType(CWindowby3::WT_VERTICAL);
	m_Background.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundLeft" ));
	m_Background.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundCenter" ));
	m_Background.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundRight" ));	
	m_Background.SetSize(225, 298);

	// Race text surface
	m_srfRaceText.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfRaceText" ));

	// 종족 이름 배경
	m_srtRaceNameBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfNameBar" ));

	// 종족 이름
	rect.SetRectWH(28, 36, 169, 25);
	m_pRaceName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pRaceName->CreateFontStd( DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pRaceName->SetTextColor( RGB(255, 192, 0) );
	m_pRaceName->Enable(false);

	// 종족 설명
	m_pExplainHtml = (gui::CHtmlBox*)GetComponent("hbxConv");
	m_pExplainHtml->SetLineSpace(8);

	Show(false);

	// sig
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CRaceExplainGui::OnPaint );

	LinkMsg(g_EventCharMake);

	NTL_RETURN(TRUE);
}

VOID CRaceExplainGui::Destroy()
{
	NTL_FUNCTION("CRaceExplainGui::Destroy");

	UnLinkMsg(g_EventCharMake);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CRaceExplainGui::SetRace(RwUInt8 byRace)
{
}

VOID CRaceExplainGui::SetPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	m_pThis->SetPosition(iXPos, iYPos);

	m_Background.SetPosition(iXPos, iYPos);
	m_srfRaceText.SetPosition(iXPos + 49, iYPos + 9);
	m_srtRaceNameBack.SetPosition(iXPos + 11, iYPos + 60);
}

VOID CRaceExplainGui::OnPaint()
{
	m_Background.Render();
	m_srfRaceText.Render();
	m_srtRaceNameBack.Render();
}

VOID CRaceExplainGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CRaceExplainGui::HandleEvents");

	if( msg.Id == g_EventCharMake )
	{
		SNtlEventCharMake* pCharMake = reinterpret_cast<SNtlEventCharMake*>(msg.pData);

		const WCHAR* pwcText = GetDisplayStringManager()->GetString(DST_LOBBY_HUMAN_EXPLAIN +  pCharMake->byRace);

		m_pRaceName->SetText(GetDisplayStringManager()->GetString(DST_HUMAN +  pCharMake->byRace));

		m_pExplainHtml->Clear();
		m_pExplainHtml->SetHtmlFromMemory(pwcText, wcslen(pwcText));
	}

	NTL_RETURNVOID();
}