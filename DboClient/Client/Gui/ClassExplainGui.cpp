#include "precomp_dboclient.h"
#include "ClassExplainGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"

CClassExplainGui::CClassExplainGui(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CClassExplainGui::~CClassExplainGui()
{

}

RwBool CClassExplainGui::Create()
{
	NTL_FUNCTION( "CClassExplainGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\CharCreate_Explain.srf", "gui\\CharCreate_Class.frm"))
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

	// Class text surface
	m_srfClassText.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfClassText" ));

	// 클래스 이름 배경
	m_srtClassNameBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfNameBar" ));

	// 종족 이름
	rect.SetRectWH(28, 36, 169, 25);
	m_pClassName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pClassName->CreateFontStd( DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pClassName->SetTextColor( RGB(255, 192, 0) );
	m_pClassName->Enable(false);

	// 클래스 설명
	m_pExplainHtml = (gui::CHtmlBox*)GetComponent("hbxConv");
	m_pExplainHtml->SetLineSpace(8);

	Show(false);

	// sig
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CClassExplainGui::OnPaint );

	LinkMsg(g_EventCharMake);

	NTL_RETURN(TRUE);
}

VOID CClassExplainGui::Destroy()
{
	NTL_FUNCTION("CClassExplainGui::Destroy");

	UnLinkMsg(g_EventCharMake);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CClassExplainGui::SetClass(RwUInt8 byRace)
{
}

VOID CClassExplainGui::SetPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	m_pThis->SetPosition(iXPos, iYPos);

	m_Background.SetPosition(iXPos, iYPos);
	m_srfClassText.SetPosition(iXPos + 49, iYPos + 9);
	m_srtClassNameBack.SetPosition(iXPos + 11, iYPos + 60);
}

VOID CClassExplainGui::OnPaint()
{
	m_Background.Render();
	m_srfClassText.Render();
	m_srtClassNameBack.Render();
}

VOID CClassExplainGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CClassExplainGui::HandleEvents");

	if( msg.Id == g_EventCharMake )
	{
		SNtlEventCharMake* pCharMake = reinterpret_cast<SNtlEventCharMake*>(msg.pData);
		
		m_pClassName->SetText(Logic_GetClassName(pCharMake->byClass));

		const WCHAR* pwcText = GetDisplayStringManager()->GetString(DST_LOBBY_HUMAN_FIGHTER +  pCharMake->byClass);
		
		m_pExplainHtml->Clear();
		m_pExplainHtml->SetHtmlFromMemory( pwcText , wcslen(pwcText) );
	}

	NTL_RETURNVOID();
}