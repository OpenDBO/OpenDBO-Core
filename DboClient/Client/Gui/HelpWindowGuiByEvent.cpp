#include "precomp_dboclient.h"
#include "HelpWindowGuiByEvent.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"

// dbo
#include "DboGlobal.h"

namespace
{
	// tw: the flight practice scrolls and their tutorial flash
	struct sFLIGHT_SCROLL_FLASH
	{
		TBLIDX			itemTblidx;
		const char*		pcFlashFile;
	};

	const sFLIGHT_SCROLL_FLASH s_aFlightScrollFlash[] =
	{
		{ 430001, "Tu_Bukujutsu_move.swf" },
		{ 430002, "Tu_Bukujutsu_dash.swf" },
		{ 430003, "Tu_Bukujutsu_speedup.swf" },
	};

	const char* FindFlightScrollFlash(TBLIDX itemTblidx)
	{
		for( RwUInt32 i = 0 ; i < sizeof(s_aFlightScrollFlash) / sizeof(s_aFlightScrollFlash[0]) ; ++i )
		{
			if( s_aFlightScrollFlash[i].itemTblidx == itemTblidx )
				return s_aFlightScrollFlash[i].pcFlashFile;
		}

		return NULL;
	}
}


CHelpWindowGuiByEvent::CHelpWindowGuiByEvent(const RwChar* pName)
:CNtlPLGui(pName)
,m_pFlash(NULL)
,m_byBuffIndex(0xff)
{
}

CHelpWindowGuiByEvent::~CHelpWindowGuiByEvent()
{
}

RwBool CHelpWindowGuiByEvent::Create()
{
	NTL_FUNCTION("CHelpWindowGuiByEvent::Create");

	if( !CNtlPLGui::Create("", "", "gui\\HelpWindowByEvent.frm") )
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pFlash = (gui::CFlash*)GetComponent("flashHelpWindow");
	if( m_pFlash )
	{
		m_slotMovieEnd = m_pFlash->SigMovieEnd().Connect(this, &CHelpWindowGuiByEvent::OnMovieEnd);

		m_pFlash->Show(false);
		m_pFlash->SetResizeType(gui::CFlash::EXACTFIT);
		m_pFlash->AutoRewind(FALSE);
		m_pFlash->PlayMovie(FALSE);
	}

	Show(false);

	LinkMsg(g_EventSobBuffAdd);
	LinkMsg(g_EventSobBuffDrop);

	GetNtlGuiManager()->AddUpdateFunc(this);

	NTL_RETURN(TRUE);
}

VOID CHelpWindowGuiByEvent::Destroy()
{
	NTL_FUNCTION("CHelpWindowGuiByEvent::Destroy");

	CloseFlash();

	UnLinkMsg(g_EventSobBuffAdd);
	UnLinkMsg(g_EventSobBuffDrop);

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CHelpWindowGuiByEvent::Update(RwReal fElapsed)
{
	if( !m_pFlash || !m_pFlash->IsPlayMovie() )
		return;

	LocateComponent();
	m_pFlash->Update(fElapsed);
}

RwBool CHelpWindowGuiByEvent::LoadFlash(const char* pcFileName)
{
	if( !m_pFlash || !pcFileName )
		return FALSE;

	// tw: a flash already playing is not restarted
	if( m_pFlash->IsPlayMovie() )
		return TRUE;

	if( !m_pFlash->Load(pcFileName) )
	{
		DBO_FAIL("CHelpWindowGuiByEvent::LoadFlash, not exist file : " << pcFileName);
		return FALSE;
	}

	LocateComponent();

	m_pFlash->SetFrameCallback(TRUE);
	m_pFlash->Show(true);
	m_pFlash->PlayMovie(TRUE);

	Show(true);

	return TRUE;
}

VOID CHelpWindowGuiByEvent::LocateComponent()
{
	// tw: the dialog is centred on the screen with the size of the flash
	CRectangle rtFlash = m_pFlash->GetFrameResolution();
	RwInt32 iWidth = rtFlash.GetWidth();
	RwInt32 iHeight = rtFlash.GetHeight();

	m_pThis->SetPosition(((RwInt32)GetDboGlobal()->GetScreenWidth() - iWidth) / 2,
						 ((RwInt32)GetDboGlobal()->GetScreenHeight() - iHeight) / 2);
	m_pFlash->SetSize(iWidth, iHeight);
}

VOID CHelpWindowGuiByEvent::CloseFlash()
{
	if( m_pFlash )
	{
		m_pFlash->Show(false);
		m_pFlash->Unload();
	}

	m_byBuffIndex = 0xff;

	Show(false);
}

VOID CHelpWindowGuiByEvent::OnMovieEnd(gui::CComponent* pComponent)
{
	CloseFlash();
}

VOID CHelpWindowGuiByEvent::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CHelpWindowGuiByEvent::HandleEvents");

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		NTL_RETURNVOID();

	if( msg.Id == g_EventSobBuffAdd )
	{
		SNtlEventSobBuffAdd* pBuffAdd = reinterpret_cast<SNtlEventSobBuffAdd*>(msg.pData);

		if( pBuffAdd->hSerialId != pAvatar->GetSerialID() || pBuffAdd->byBuffType != DBO_OBJECT_SOURCE_ITEM )
			NTL_RETURNVOID();

		const char* pcFlashFile = FindFlightScrollFlash(pBuffAdd->uiTblId);
		if( pcFlashFile && LoadFlash(pcFlashFile) )
		{
			m_byBuffIndex = pBuffAdd->byBuffIndex;
		}
	}
	else if( msg.Id == g_EventSobBuffDrop )
	{
		SNtlEventSobBuffDrop* pBuffDrop = reinterpret_cast<SNtlEventSobBuffDrop*>(msg.pData);

		if( pBuffDrop->hSerialId != pAvatar->GetSerialID() || pBuffDrop->byBuffType != DBO_OBJECT_SOURCE_ITEM )
			NTL_RETURNVOID();

		if( m_byBuffIndex != 0xff && pBuffDrop->byBuffIndex == m_byBuffIndex )
		{
			CloseFlash();
		}
	}

	NTL_RETURNVOID();
}
