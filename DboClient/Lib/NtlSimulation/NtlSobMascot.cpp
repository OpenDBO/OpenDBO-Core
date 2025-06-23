#include "precomp_ntlsimulation.h"
#include "NtlSobMascot.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGlobal.h"
#include "NtlPLSceneManager.h"

// mascot
#include "NtlSobMascotAttr.h"
#include "NtlSobMascotProxy.h"

DEFINITION_MEMORY_POOL(CNtlSobMascot)

CNtlSobMascot::CNtlSobMascot()
{
	m_pMascotAttr = NULL;
	m_bSummoned = FALSE;
	m_byMascotIndex = INVALID_BYTE;
	m_tblidx = INVALID_TBLIDX;
	m_dwCurrentVP = 0;
	m_dwMaxVP = 0;
	m_dwCurrentExp = 0;
	m_byItemRank = 0;
	memset(m_skillTblidx, 0, sizeof(m_skillTblidx));
	
	// Set flags like pets do - this is crucial for character proxy to work!
	SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_LUA_INTERLOCKING | SLFLAG_LUA_TRANSFORM_ENABLE);
	SetActorFlags(SLFLAG_ACTOR_HAVE_LIFE);  // This is the key flag for character proxy!
}

CNtlSobMascot::~CNtlSobMascot()
{
}

RwBool CNtlSobMascot::Create(void)
{
	NTL_FUNCTION("CNtlSobMascot::Create");

	// Create attribute
	m_pMascotAttr = NTL_NEW CNtlSobMascotAttr;
	if (!m_pMascotAttr->Create())
		NTL_RETURN(FALSE);

	// Set the base class attribute pointer so character proxy can access it
	m_pSobAttr = m_pMascotAttr;

	// Use CNtlSobMascotProxy (original system) for both UI and world rendering
	m_pSobProxy = NTL_NEW CNtlSobMascotProxy;
	m_pSobProxy->Create(0);
	m_pSobProxy->SetSobObj(this);

	// Call base class Create - mascots inherit from CNtlSobActor for UI functionality
	if (!CNtlSobActor::Create())
		NTL_RETURN(FALSE);

	// Link only specific events (NOT global creation events)
	LinkMsg(g_EventSobDelete);
	NTL_RETURN(TRUE);
}

void CNtlSobMascot::Destroy(void)
{
	NTL_FUNCTION("CNtlSobMascot::Destroy");

	// Unlink events
	UnLinkMsg(g_EventSobDelete);

	// Destroy attribute
	if (m_pMascotAttr)
	{
		m_pSobAttr = NULL;  // Clear base class pointer to avoid double deletion
		m_pMascotAttr->Destroy();
		NTL_DELETE(m_pMascotAttr);
		m_pMascotAttr = NULL;
	}

	// Call base class destroy (like pets do)
	CNtlSobActor::Destroy();

	NTL_RETURNVOID();
}

void CNtlSobMascot::Update(RwReal fElapsed)
{
	// Call base class update like pets do - this handles proxy and actor updates
	CNtlSobActor::Update(fElapsed);
}

void CNtlSobMascot::HandleEvents(RWS::CMsg &pMsg)
{
	// No longer handle global g_EventSobCreate events - this was causing duplicates
	// Mascots handle their specific creation through direct calls, not global events
	
	// Call base class to handle actor events properly (like pets do)
	CNtlSobActor::HandleEvents(pMsg);

	// Handle mascot-specific events with mascot attribute
	if (m_pMascotAttr)
		m_pMascotAttr->HandleEvents(pMsg);
}

void CNtlSobMascot::CreateWorldMascot(void)
{
	// Create world mascot (moved from HandleEvents to be called only when summoned)
	
	// game attribute data setting (like pets do)
	if (m_pMascotAttr)
		m_pMascotAttr->HandleEvents(RWS::CMsg(g_EventSobCreate, NULL));
	
	// proxy setting (like pets do) - this enables world rendering!
	if (GetSobProxy())
		GetSobProxy()->HandleEvents(RWS::CMsg(g_EventSobCreate, NULL));
}

//---------------------------------------------------
// Attribute related functions
//---------------------------------------------------
CNtlSobMascotAttr* CNtlSobMascot::GetMascotAttr(void) const
{
	return m_pMascotAttr;
}

void CNtlSobMascot::SetMascotAttr(CNtlSobMascotAttr *pMascotAttr)
{
	m_pMascotAttr = pMascotAttr;
}

//---------------------------------------------------
// Mascot specific functions
//---------------------------------------------------
RwBool CNtlSobMascot::IsSummoned(void) const
{
	return m_bSummoned;
}

void CNtlSobMascot::SetSummoned(RwBool bSummoned)
{
	m_bSummoned = bSummoned;
}

RwUInt8 CNtlSobMascot::GetMascotIndex(void) const
{
	return m_byMascotIndex;
}

void CNtlSobMascot::SetMascotIndex(RwUInt8 byIndex)
{
	m_byMascotIndex = byIndex;
	if (m_pMascotAttr)
		m_pMascotAttr->SetMascotIndex(byIndex);
}

TBLIDX CNtlSobMascot::GetMascotTblidx(void) const
{
	return m_tblidx;
}

void CNtlSobMascot::SetMascotTblidx(TBLIDX tblidx)
{
	m_tblidx = tblidx;
	if (m_pMascotAttr)
		m_pMascotAttr->SetMascotTblidx(tblidx);
}

void CNtlSobMascot::SetItemTblidx(TBLIDX itemTblidx)
{
	if (m_pMascotAttr)
		m_pMascotAttr->SetItemTblidx(itemTblidx);
}

RwUInt32 CNtlSobMascot::GetCurrentVP(void) const
{
	return m_dwCurrentVP;
}

void CNtlSobMascot::SetCurrentVP(RwUInt32 dwVP)
{
	m_dwCurrentVP = dwVP;
	if (m_pMascotAttr)
		m_pMascotAttr->SetCurrentVP(dwVP);
}

RwUInt32 CNtlSobMascot::GetMaxVP(void) const
{
	return m_dwMaxVP;
}

void CNtlSobMascot::SetMaxVP(RwUInt32 dwMaxVP)
{
	m_dwMaxVP = dwMaxVP;
	if (m_pMascotAttr)
		m_pMascotAttr->SetMaxVP(dwMaxVP);
}

RwUInt32 CNtlSobMascot::GetCurrentExp(void) const
{
	return m_dwCurrentExp;
}

void CNtlSobMascot::SetCurrentExp(RwUInt32 dwExp)
{
	m_dwCurrentExp = dwExp;
	if (m_pMascotAttr)
		m_pMascotAttr->SetCurrentExp(dwExp);
}

RwUInt8 CNtlSobMascot::GetItemRank(void) const
{
	return m_byItemRank;
}

void CNtlSobMascot::SetItemRank(RwUInt8 byRank)
{
	m_byItemRank = byRank;
	if (m_pMascotAttr)
		m_pMascotAttr->SetItemRank(byRank);
}

TBLIDX CNtlSobMascot::GetSkillTblidx(RwUInt8 bySkillIndex) const
{
	if (bySkillIndex < DBO_MASCOT_MAX_SKILL_COUNT)
		return m_skillTblidx[bySkillIndex];
	return INVALID_TBLIDX;
}

void CNtlSobMascot::SetSkillTblidx(RwUInt8 bySkillIndex, TBLIDX skillTblidx)
{
	if (bySkillIndex < DBO_MASCOT_MAX_SKILL_COUNT)
	{
		m_skillTblidx[bySkillIndex] = skillTblidx;
		if (m_pMascotAttr)
			m_pMascotAttr->SetSkillTblidx(bySkillIndex, skillTblidx);
	}
}

// Override to return mascot attribute as the main attribute
CNtlSobAttr* CNtlSobMascot::GetSobAttr(void) const
{
	return m_pMascotAttr;  // Return mascot attribute so character proxy can access it
} 