#include "precomp_ntlsimulation.h"
#include "NtlSobAttr.h"

//core
#include "NtlDebug.h"

// CNtlSLAttrFactory* CNtlSLAttrFactory::m_pFactory[MAX_SLCLASS] = { NULL, };

CNtlSobLifeAttr::CNtlSobLifeAttr()
{
	m_byLevel	= 1;
	m_iLp		= 0;
	m_iMaxLp	= 1;
	m_iEp		= 0;
	m_iMaxEp	= 1;
	m_iRp		= 0;
	m_iMaxRp	= 1;
    m_iRPStock  = 0;
    m_iMaxRpStock = 1;
	m_fRadius	= 0.0f;

	m_wstrName	= L"NONE";
	m_NameColor	= RGB(255, 255, 255);
	m_NickNameColor = RGB(255, 255, 255);

	m_bNameCreate = TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

CNtlSobMoveableAttr::CNtlSobMoveableAttr()
{
	m_fDefaultWalkSpeed		= 8.0f;
	m_fDefaultRunSpeed		= 8.0f;
	m_fWalkSpeed			= 8.0f;
	m_fRunSpeed				= 8.0f;
	m_fAttackAnimSpeed		= 1.0f;    
    m_fRunAnimSpeed         = 1.0f;
	m_fFlyHeight			= 0.0f;
	m_fScale				= 1.0f;
}


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

CNtlSobBattleAttr::CNtlSobBattleAttr()
{
	m_fBaseAttackRange		= 1.0f;
	m_fAttackRange			= 1.0f;
	m_fCastingTimeModifier	= 0.0f;
	m_fCoolingTimeModifier	= 0.0f;
	m_fKeepingTimeModifier  = 0.0f;
	m_fDOTValueModifier		= 0.0f;
	m_fDOTTimeModifier		= 0.0f;
	m_fRequiredEPModifier	= 0.0f;
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

CNtlSobSocialAttr::CNtlSobSocialAttr()
{
	m_byRace		= 0;
	m_uiRaceFlag	= 0;
	m_byGender		= 0;
	m_byClass		= 0;
	m_byGender		= 0;
    m_bIsAdult      = FALSE;

	m_byFace		= 0;
	m_byHair		= 0;
	m_byHairColor	= 0;
	m_bySkinColor	= 0;

	m_uiReputation	= 0;
}
