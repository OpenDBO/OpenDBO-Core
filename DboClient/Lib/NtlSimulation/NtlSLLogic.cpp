#include "precomp_ntlsimulation.h"
#include "NtlSLLogic.h"

#include "psapi.h"

// shared
#include "NtlResultCode.h"
#include "MobTable.h"
#include "NpcTable.h"
#include "PcTable.h"
#include "ItemTable.h"
#include "CharmTable.h"
#include "NtlMovement.h"
#include "SkillTable.h"
#include "ObjectTable.h"
#include "GraphicDataTable.h"
#include "WorldTable.h"
#include "MerchantTable.h"
#include "DragonBallTable.h"
#include "SystemEffectTable.h"
#include "DboNavi.h"
#include "NtlCharacter.h"
#include "DynamicObjectTable.h"
#include "UseItemTable.h"
#include "ItemUpgradeTable.h"
#include "WorldZoneTable.h"

// core
#include "NtlDebug.h"

// framework
#include "NtlTimer.h"

// sound
#include "NtlSoundDefines.h"
#include "NtlSoundManager.h"
#include "GUISoundDefine.h"

// presentation
#include "NtlMath.h"
#include "NtlPLCharacter.h"
#include "NtlPLSceneManager.h"
#include "NtlPLDamageBox.h"
#include "NtlPLObject.h"
#include "NtlInstanceEffect.h"
#include "NtlAtomic.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLOptionManager.h"
#include "NtlPLPalette.h"
#include "NtlPLWorldEntity.h"

// simulation
#include "NtlSobGroup.h"
#include "TableContainer.h"
#include "NtlSobPlayer.h"
#include "NtlSobAvatar.h"
#include "NtlSobItem.h"
#include "NtlSobSkill.h"
#include "NtlSobMonster.h"
#include "NtlSobNpc.h"
#include "NtlSobPet.h"
#include "NtlSobManager.h"
#include "NtlSobSkillIcon.h"
#include "NtlSobTriggerObject.h"
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSobPlayerAttr.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobPetAttr.h"
#include "NtlSobItemAttr.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSobDynamicObjectAttr.h"
#include "NtlSobCharProxy.h"
#include "NtlSobTriggerObjectProxy.h"
#include "NtlSobDynamicObject.h"
#include "NtlSobDynamicObjectAttr.h"
#include "NtlSobDynamicObjectProxy.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLEventFunc.h"
#include "NtlSobProxy.h"
#include "NtlSobElapsedController.h"
#include "NtlFSMDef.h"
#include "NtlFSMLayer.h"
#include "NtlFSMStateBase.h"
#include "NtlFSMUtil.h"
#include "NtlWorldConcept.h"
#include "NtlSLParty.h"
#include "NtlSLApi.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSLLogic.h"
#include "NtlCameraController.h"
#include "NtlCameraManager.h"
#include "NtlGameRuleConcept.h"
#include "NtlFSMCharActAgent.h"
#include "NtlSobNpc.h"
#include "NtlSobCharProxyDecoration.h"
#include "NtlSobCharProxyTransform.h"
#include "NtlWorldConceptTutorial.h"
#include "DisplayStringDef.h"
#include "NtlSobCharProxyEquip.h"
#include "NtlSobVehicle.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"


RwReal		g_fCharColliAngle		= 0.75f;
RwReal		g_fCharSlippingAngle	= 0.5f;
RwUInt32	g_uiFlashBalloonWidth	= 300;
RwUInt32	g_uiFlashBalloonHeight	= 150;
RwReal		g_fMatrixTime			= 0.15f;
RwReal		g_fMatrixValue			= 0.1f;
RwReal		g_fMatrixDelayTime		= 0.15f;
RwReal		g_fMatrixCamera			= 2.0f;
RwBool		g_bDispPacketLockMsg	= FALSE;
RwBool		g_bDispServerMsg		= FALSE;

RwUInt8		g_byShadowRed			= 166;
RwUInt8		g_byShadowGreen			= 166;
RwUInt8		g_byShadowBlue			= 166;

RwReal      g_fTestVal              = 1.0f;
RwReal      g_fTestVal2             = 1.0f;
RwReal      g_fTestVal3             = 1.0f;

RwInt32		g_nTestVal				= 75;

RwInt32		g_nBroadCastGuiPosX		= 600;
RwUInt32    g_uiTargetMarkingId     = INVALID_SERIAL_ID;

RwBool		g_bActiveAnimSync		= TRUE;

RwUInt32	g_uiLimitMemory			= 950;
RwUInt32	g_uiCreateHitEffectRate = 6;

RwV3d		g_v3Default = {1.f, 1.f, 1.f};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 함수 포인터 등록
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Callback_GetNPCJobName g_fnGetNPCJobName = NULL;

void LinkCallback_GetNPCJobName(Callback_GetNPCJobName fn)
{
	g_fnGetNPCJobName = fn;
}

const WCHAR* Logic_GetSLNPCJobName(RwUInt8 byJob)
{
	if( !g_fnGetNPCJobName )
		return NULL;

	return g_fnGetNPCJobName(byJob);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// memory
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Logic_GetCurMemory( DWORD& PageFaultCount,
						 SIZE_T& PeakWorkingSetSize,
						 SIZE_T& WorkingSetSize,
						 SIZE_T& QuotaPeakPagedPoolUsage,
						 SIZE_T& QuotaPagedPoolUsage,
						 SIZE_T& QuotaPeakNonPagedPoolUsage,
						 SIZE_T& QuotaNonPagedPoolUsage,
						 SIZE_T& PagefileUsage,
						 SIZE_T& PeakPagefileUsage )
{
	PROCESS_MEMORY_COUNTERS pmc;

	if ( 0 == GetProcessMemoryInfo( GetCurrentProcess(), &pmc, sizeof( pmc ) ) )
	{
		return false;
	}

	PageFaultCount = pmc.PageFaultCount;
	PeakWorkingSetSize = pmc.PeakWorkingSetSize;
	WorkingSetSize = pmc.WorkingSetSize;
	QuotaPeakPagedPoolUsage = pmc.QuotaPeakPagedPoolUsage;
	QuotaPagedPoolUsage = pmc.QuotaPagedPoolUsage;
	QuotaPeakNonPagedPoolUsage = pmc.QuotaPeakNonPagedPoolUsage;
	QuotaNonPagedPoolUsage = pmc.QuotaNonPagedPoolUsage;
	PagefileUsage = pmc.PagefileUsage;
	PeakPagefileUsage = pmc.PeakPagefileUsage;

	return true;
}

void   Logic_CalcLimitMemory(SIZE_T TotalPhysMemory)
{
	if(TotalPhysMemory < 1024*1024*600)
	{
		g_uiLimitMemory = 600;
	}
	else if(TotalPhysMemory < 1024*1024*1100)
	{
		g_uiLimitMemory = 800;
	}
	else
	{
		g_uiLimitMemory = 950;
	}
}

RwBool Logic_IsCreateHitEffect(void)
{
	DWORD PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;


	bool bRes = Logic_GetCurMemory(PageFaultCount, PeakWorkingSetSize, WorkingSetSize, QuotaPeakPagedPoolUsage, QuotaPagedPoolUsage,
									QuotaPeakNonPagedPoolUsage, QuotaNonPagedPoolUsage, PagefileUsage, PeakPagefileUsage);

	if(!bRes)
		return TRUE;

	SIZE_T LimitMemory = 1024*1024*g_uiLimitMemory;

	if(PagefileUsage > LimitMemory || CNtlTimer::GetFps() < 1.0f)
	{
		if(rand() % g_uiCreateHitEffectRate == 0)
			return TRUE;
		else 
			return FALSE;
	}

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// game master key definition
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool Logic_IsDevUser(void)
{
	SDevMasterInfo *pMasterInfo = API_GetSLDevMasterInformation();
	if(pMasterInfo == NULL || !pMasterInfo->bDevUser)
		return FALSE;

	return TRUE;
}


RwBool Logic_IsUIDevInfoVisible(void)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// monster/player logic
// 주의 : 속도는 table data와 상태 및 버프가 고려된 속도가 될것이다.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor 속성 logic

const WCHAR* Logic_GetName(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return NULL;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		return pSobLifeAttr->GetName();
	}

	return NULL;
}

void Logic_SetLp(CNtlSob *pSobObj, RwUInt32 uiLp)
{
	if( !pSobObj )
		return;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 

		RwInt32 iMaxLp = pSobLifeAttr->GetMaxLp();
		if( (RwInt32)uiLp > iMaxLp )
			uiLp = (RwUInt32)iMaxLp;

		pSobLifeAttr->SetLp((RwInt32)uiLp);

		CNtlSLEventGenerator::SobUpdate(pSobActor->GetSerialID(), EVENT_AIUT_ATTR, 0);
	}
}

void Logic_SetLp(CNtlSob *pSobObj, RwUInt32 uiLp, RwUInt32 uiMaxLp)
{
	if( !pSobObj )
		return;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 

		if(uiLp > uiMaxLp)
			uiLp = uiMaxLp;

		pSobLifeAttr->SetLp((RwInt32)uiLp);
		pSobLifeAttr->SetMaxLp((RwInt32)uiMaxLp);

		CNtlSLEventGenerator::SobUpdate(pSobActor->GetSerialID(), EVENT_AIUT_ATTR, 0);
	}
}


void Logic_SetLpPlusMinus(CNtlSob *pSobObj, RwInt32 iDeltaLp)
{
	if( !pSobObj )
		return;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		RwInt32 iCurrLp = (RwInt32)Logic_GetLp (pSobObj);

		iCurrLp = iCurrLp - iDeltaLp;
		if(iCurrLp < 0)
			iCurrLp = 0;

		Logic_SetLp(pSobObj, (RwUInt32)iCurrLp);
	}
}


RwUInt32 Logic_GetLp(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return NTL_INVALID_LP;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		return pSobLifeAttr->GetLp();
	}

	return NTL_INVALID_LP;
}



RwUInt32 Logic_GetMaxLp(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return NTL_INVALID_LP;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		return pSobLifeAttr->GetMaxLp();
	}

	return NTL_INVALID_LP;
}

void Logic_SetEp(CNtlSob *pSobObj, RwUInt32 uiEp)
{
	if( !pSobObj )
		return;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		pSobLifeAttr->SetEp((RwInt32)uiEp);

		CNtlSLEventGenerator::SobUpdate(pSobObj->GetSerialID(), EVENT_AIUT_ATTR, 0);
	}
}

void Logic_SetEp(CNtlSob *pSobObj, RwUInt32 uiEp, RwUInt32 uiMaxEp)
{
	if( !pSobObj )
		return;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		pSobLifeAttr->SetEp((RwInt32)uiEp);
		pSobLifeAttr->SetMaxEp((RwInt32)uiMaxEp);

		CNtlSLEventGenerator::SobUpdate(pSobObj->GetSerialID(), EVENT_AIUT_ATTR, 0);
	}
}

RwUInt32 Logic_GetEp(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return NTL_INVALID_EP;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		return pSobLifeAttr->GetEp();
	}

	return NTL_INVALID_EP;
}

RwUInt32 Logic_GetMaxEp( CNtlSob *pSobObj )
{
	if( !pSobObj )
		return NTL_INVALID_EP;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		return pSobLifeAttr->GetMaxEp();
	}

	return NTL_INVALID_EP;
}

void Logic_SetRp(CNtlSob *pSobObj, RwUInt32 uiRp)
{
	if( !pSobObj )
		return;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		pSobLifeAttr->SetRp((RwInt32)uiRp);
		//CNtlSLEventGenerator::SobUpdate(pSobObj->GetSerialID(), EVENT_AIUT_ATTR, 0);
	}
}

void Logic_SetRp(CNtlSob *pSobObj, RwUInt32 uiRp, RwUInt32 uiMaxRp)
{
	if( !pSobObj )
		return;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		pSobLifeAttr->SetRp((RwInt32)uiRp);
		pSobLifeAttr->SetMaxRp((RwInt32)uiMaxRp);
		//CNtlSLEventGenerator::SobUpdate(pSobObj->GetSerialID(), EVENT_AIUT_ATTR, 0);
	}
}

RwUInt32 Logic_GetRp(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return NTL_INVALID_RP;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		return pSobLifeAttr->GetRp();
	}

	return NTL_INVALID_RP;
}

RwUInt32 Logic_GetMaxRp(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return NTL_INVALID_RP;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		return pSobLifeAttr->GetMaxRp();
	}

	return NTL_INVALID_RP;
}


void Logic_SetRpStock( CNtlSob* pSobObj, RwUInt32 uiRpStock ) 
{
	if( !pSobObj )
		return;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
    {
        
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 

		if( uiRpStock > (RwUInt32)pSobLifeAttr->GetRp() )
        {
            // RP Stock이 올랐을때 이벤트 
            if(Logic_GetActorStateId(pSobActor) == NTL_FSMSID_CHARGING)
            {
                pSobActor->GetSobProxy()->CreatePLEffect(NTL_VID_RPBALL_UP, &pSobActor->GetPosition());
            }
			Logic_SetRp( pSobObj, 0 );
        }

		pSobLifeAttr->SetRpStock(uiRpStock);
		CNtlSLEventGenerator::SobUpdate(pSobObj->GetSerialID(), EVENT_AIUT_ATTR, 0);
	}
}

void Logic_SetRpStockMax(CNtlSob* pSobObj, RwUInt32 uiRpStockMax)
{
	if( !pSobObj )
		return;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		pSobLifeAttr->SetMaxRpStock(uiRpStockMax);
		CNtlSLEventGenerator::SobUpdate(pSobObj->GetSerialID(), EVENT_AIUT_ATTR, 0);
	}
}

RwUInt32 Logic_GetRpStock( CNtlSob* pSobObj ) 
{
	if( !pSobObj )
		return NTL_INVALID_RP;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		return pSobLifeAttr->GetRpStock();
	}

	return NTL_INVALID_RP;
}

RwUInt32 Logic_GetMaxRpStock( CNtlSob* pSobObj ) 
{
	if( !pSobObj )
		return NTL_INVALID_RP;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		return pSobLifeAttr->GetMaxRpStock();
	}

	return NTL_INVALID_RP;    
}

void Logic_SetLevel(CNtlSob *pSobObj, RwUInt8 byLevel)
{
	if( !pSobObj )
		return;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		pSobLifeAttr->SetLevel(byLevel);
		CNtlSLEventGenerator::SobUpdate(pSobObj->GetSerialID(), EVENT_AIUT_ATTR | EVENT_AIUT_ATTR_LEVELUP, 0);
	}
}

RwUInt8 Logic_GetLevel(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return NTL_INVALID_LEVEL;

	CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( pSobObj );
	if(pSobActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_LIFE)
	{
		CNtlSobLifeAttr *pSobLifeAttr = reinterpret_cast<CNtlSobLifeAttr*>(pSobActor->GetSobAttr()); 
		return pSobLifeAttr->GetLevel();
	}

	return NTL_INVALID_LEVEL;
}

// exp
void Logic_SetExp(CNtlSob *pSobObj, RwUInt32 uiExp)
{
	if( !pSobObj )
		return;

	RwUInt32 uiClassId = pSobObj->GetClassID();

	if(uiClassId == SLCLASS_AVATAR)
	{
		CNtlSobAvatarAttr *pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pSobObj->GetSobAttr());
		pAvatarAttr->m_uiExp = uiExp;

		CNtlSLEventGenerator::SobUpdate(pSobObj->GetSerialID(), EVENT_AIUT_ATTR, 0);
	}
}

RwUInt32 Logic_GetExp(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return NTL_INVALID_EXP;

	RwUInt32 uiClassId = pSobObj->GetClassID();

	if(uiClassId == SLCLASS_AVATAR)
	{
		CNtlSobAvatarAttr *pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pSobObj->GetSobAttr());
		return pAvatarAttr->m_uiExp; 
	}

	return NTL_INVALID_EXP;
}

void Logic_SetMaxExp(CNtlSob *pSobObj, RwUInt32 uiMaxExp)
{
	if( !pSobObj )
		return;

	RwUInt32 uiClassId = pSobObj->GetClassID();

	if(uiClassId == SLCLASS_AVATAR)
	{
		CNtlSobAvatarAttr *pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pSobObj->GetSobAttr());
		pAvatarAttr->m_uiMaxExp = uiMaxExp;

		CNtlSLEventGenerator::SobUpdate(pSobObj->GetSerialID(), EVENT_AIUT_ATTR, 0 );
	}
}

RwUInt32 Logic_GetMaxExp(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return NTL_INVALID_EXP;

	RwUInt32 uiClassId = pSobObj->GetClassID();

	if( uiClassId == SLCLASS_AVATAR )
	{
		CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pSobObj->GetSobAttr());
		return pAvatarAttr->m_uiMaxExp;	
	}

	return NTL_INVALID_EXP;
}

void Logic_SetZenny(RwUInt32 uiZenny)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );

	Logic_PlayGUISound(GSD_SYSTEM_USE_MONEY);

	pAvatarAttr->m_uiZenny = uiZenny;
	CNtlSLEventGenerator::SobUpdate( pAvatar->GetSerialID(), EVENT_AIUT_ZENNY, 0 );
}

RwUInt32 Logic_GetZenny(void)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return 0;

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );

	RwUInt32 uiAdjustedZenny = pAvatarAttr->m_uiZenny;

	for( RwInt32 i = 0 ; i < SAdjustZennyInfo::NUM_TYPE ; ++i )
	{
		uiAdjustedZenny += GetNtlSLGlobal()->GetAdjustZennyInfo()->GetAdjustZenny( i );
	}

	return uiAdjustedZenny;
}

void Logic_SetSp( RwUInt32 uiSp )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );	

	pAvatarAttr->m_uiSp = uiSp;
	CNtlSLEventGenerator::SobUpdate( pAvatar->GetSerialID(), EVENT_AIUT_SKILL_SP, 0 );
}

RwUInt32 Logic_GetSp(void)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return 0;

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );

	return pAvatarAttr->m_uiSp;
}

void Logic_SetHonorPoint( RwUInt32 uiHonorPoint )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pAvatar == NULL )
		return;

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );	

	pAvatarAttr->m_uiHonorPoint = uiHonorPoint;
	CNtlSLEventGenerator::SobUpdate( pAvatar->GetSerialID(), EVENT_AIUT_POINT_HONOR, 0 );
}

RwUInt32 Logic_GetHonorPoint( void )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pAvatar == NULL )
		return 0;

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );

	return pAvatarAttr->m_uiHonorPoint;
}

void Logic_SetMudosaPoint( RwUInt32 uiMudosaPoint )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pAvatar == NULL )
		return;

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );	

	pAvatarAttr->m_uiMudosaPoint = uiMudosaPoint;
	CNtlSLEventGenerator::SobUpdate( pAvatar->GetSerialID(), EVENT_AIUT_POINT_MUDOSA, 0 );
}

RwUInt32 Logic_GetMudosaPoint( void )
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( pAvatar == NULL )
		return 0;

	CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );

	return pAvatarAttr->m_uiMudosaPoint;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// avatar status value
void Logic_SetStatus(CNtlSob *pSobObj, RwUInt8 byAttributeTotalCount, RwUInt8 *pData)
{
	if(pSobObj == NULL)
		return;

	if(pSobObj->GetClassID() == SLCLASS_AVATAR)
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );

		sAVATAR_ATTRIBUTE sAttrData;

		sAttrData.wLastMaxLP = (RwUInt16)pAvatarAttr->GetMaxLp();
		sAttrData.wLastMaxEP = (RwUInt16)pAvatarAttr->GetMaxEp();
		sAttrData.wLastMaxRP = (RwUInt16)pAvatarAttr->GetMaxRp();

		sAttrData.byBaseStr = pAvatarAttr->m_byBaseStr;
		sAttrData.byLastStr = pAvatarAttr->m_byLastStr;
		sAttrData.byBaseCon = pAvatarAttr->m_byBaseCon;
		sAttrData.byLastCon = pAvatarAttr->m_byLastCon;
		sAttrData.byBaseFoc = pAvatarAttr->m_byBaseFoc;
		sAttrData.byLastFoc = pAvatarAttr->m_byLastFoc;
		sAttrData.byBaseDex = pAvatarAttr->m_byBaseDex;
		sAttrData.byLastDex = pAvatarAttr->m_byLastDex;
		sAttrData.byBaseSol = pAvatarAttr->m_byBaseSol;
		sAttrData.byLastSol = pAvatarAttr->m_byLastSol;
		sAttrData.byBaseEng = pAvatarAttr->m_byBaseEng;
		sAttrData.byLastEng = pAvatarAttr->m_byLastEng;

		sAttrData.wLastRpRegen = pAvatarAttr->m_wLastRpRegen;
		sAttrData.wLastRpDimimutionRate = pAvatarAttr->m_wLastRpDimimutionRate;

		sAttrData.wBasePhysicalOffence	= pAvatarAttr->m_wBasePhyshicalOffence;
		sAttrData.wLastPhysicalOffence	= pAvatarAttr->m_wLastPhyshicalOffence;
		sAttrData.wBasePhysicalDefence	= pAvatarAttr->m_wBasePhyshicalDefence;
		sAttrData.wLastPhysicalDefence	= pAvatarAttr->m_wLastPhyshicalDefence;
		sAttrData.wBaseEnergyOffence	= pAvatarAttr->m_wBaseEnergyOffence;
		sAttrData.wLastEnergyOffence	= pAvatarAttr->m_wLastEnergyOffence;
		sAttrData.wBaseEnergyDefence	= pAvatarAttr->m_wBaseEnergyDefence;
		sAttrData.wLastEnergyDefence	= pAvatarAttr->m_wLastEnergyDefence;

		sAttrData.wBaseAttackRate		= pAvatarAttr->m_wBaseAttackRate;
		sAttrData.wLastAttackRate		= pAvatarAttr->m_wLastAttackRate;
		sAttrData.wBaseDodgeRate		= pAvatarAttr->m_wBaseDodgeRate;
		sAttrData.wLastDodgeRate		= pAvatarAttr->m_wLastDodgeRate;
		sAttrData.wBaseBlockRate		= pAvatarAttr->m_wBaseBlockRate;
		sAttrData.wLastBlockRate		= pAvatarAttr->m_wLastBlockRate;

		sAttrData.wBaseCurseSuccessRate = pAvatarAttr->m_wBaseCurseSuccessRate;
		sAttrData.wLastCurseSuccessRate = pAvatarAttr->m_wLastCurseSuccessRate;
		sAttrData.wBaseCurseToleranceRate = pAvatarAttr->m_wBaseCurseToleranceRate;
		sAttrData.wLastCurseToleranceRate = pAvatarAttr->m_wLastCurseToleranceRate;

		sAttrData.wBasePhysicalCriticalRate = pAvatarAttr->m_wBasePhysicalCriticalRate;
		sAttrData.wLastPhysicalCriticalRate = pAvatarAttr->m_wLastPhysicalCriticalRate;
		sAttrData.wBaseEnergyCriticalRate	= pAvatarAttr->m_wBaseEnergyCriticalRate;
		sAttrData.wLastEnergyCriticalRate	= pAvatarAttr->m_wLastEnergyCriticalRate;
		
		sAttrData.wBaseAttackSpeedRate	= pAvatarAttr->m_wBaseAttackSpeedRate;
		sAttrData.wLastAttackSpeedRate	= pAvatarAttr->m_wLastAttackSpeedRate;
		sAttrData.fBaseAttackRange		= pAvatarAttr->GetBaseAttackRange();
		sAttrData.fLastAttackRange		= pAvatarAttr->GetAttackRange();
		
		sAttrData.fLastRunSpeed	= pAvatarAttr->GetRunSpeed();

		sAttrData.fCastingTimeChangePercent	= pAvatarAttr->GetCastingTimeModifier();
		sAttrData.fCoolTimeChangePercent = pAvatarAttr->GetCoolingTimeModifier();
		sAttrData.fKeepTimeChangePercent = pAvatarAttr->GetKeepingTimeModifier();
		sAttrData.fDotValueChangePercent = pAvatarAttr->GetDOTValueModifier();
		sAttrData.fDotTimeChangeAbsolute = pAvatarAttr->GetDOTTimeModifier();
		sAttrData.fRequiredEpChangePercent = pAvatarAttr->GetRequiredEPModifier();

		sAttrData.fHonestOffence = pAvatarAttr->m_fHonestOffence;		
		sAttrData.fHonestDefence = pAvatarAttr->m_fHonestDefence;		
		sAttrData.fStrangeOffence = pAvatarAttr->m_fStrangeOffence;		
		sAttrData.fStrangeDefence = pAvatarAttr->m_fStrangeDefence;		
		sAttrData.fWildOffence = pAvatarAttr->m_fWildOffence;		
		sAttrData.fWildDefence = pAvatarAttr->m_fWildDefence;		
		sAttrData.fEleganceOffence = pAvatarAttr->m_fEleganceOffence;	
		sAttrData.fEleganceDefence = pAvatarAttr->m_fEleganceDefence;	
		sAttrData.fFunnyOffence = pAvatarAttr->m_fFunnyOffence;		
		sAttrData.fFunnyDefence = pAvatarAttr->m_fFunnyDefence;

		//----------------------------------------------------------------------------
		// 데이터 적용.
		CNtlAvatar::UpdateAvatarAttribute(byAttributeTotalCount, pData, &sAttrData);

		pAvatarAttr->SetMaxLp((RwInt32)sAttrData.wLastMaxLP);
		pAvatarAttr->SetMaxEp((RwInt32)sAttrData.wLastMaxEP);
		pAvatarAttr->SetMaxRp((RwInt32)sAttrData.wLastMaxRP);

		pAvatarAttr->m_byBaseStr = sAttrData.byBaseStr;
		pAvatarAttr->m_byLastStr = sAttrData.byLastStr;
		pAvatarAttr->m_byBaseCon = sAttrData.byBaseCon;
		pAvatarAttr->m_byLastCon = sAttrData.byLastCon;
		pAvatarAttr->m_byBaseFoc = sAttrData.byBaseFoc;
		pAvatarAttr->m_byLastFoc = sAttrData.byLastFoc;
		pAvatarAttr->m_byBaseDex = sAttrData.byBaseDex;
		pAvatarAttr->m_byLastDex = sAttrData.byLastDex;
		pAvatarAttr->m_byBaseSol = sAttrData.byBaseSol;
		pAvatarAttr->m_byLastSol = sAttrData.byLastSol;
		pAvatarAttr->m_byBaseEng = sAttrData.byBaseEng;
		pAvatarAttr->m_byLastEng = sAttrData.byLastEng;

		pAvatarAttr->m_wLastRpRegen = sAttrData.wLastRpRegen;
		pAvatarAttr->m_wLastRpDimimutionRate = sAttrData.wLastRpDimimutionRate;

		pAvatarAttr->m_wBasePhyshicalOffence = sAttrData.wBasePhysicalOffence;
		pAvatarAttr->m_wLastPhyshicalOffence = sAttrData.wLastPhysicalOffence;
		pAvatarAttr->m_wBasePhyshicalDefence = sAttrData.wBasePhysicalDefence;
		pAvatarAttr->m_wLastPhyshicalDefence = sAttrData.wLastPhysicalDefence;

		pAvatarAttr->m_wBaseEnergyOffence = sAttrData.wBaseEnergyOffence;
		pAvatarAttr->m_wLastEnergyOffence = sAttrData.wLastEnergyOffence;
		pAvatarAttr->m_wBaseEnergyDefence = sAttrData.wBaseEnergyDefence;
		pAvatarAttr->m_wLastEnergyDefence = sAttrData.wLastEnergyDefence;

		pAvatarAttr->m_wBaseAttackRate = sAttrData.wBaseAttackRate;
		pAvatarAttr->m_wLastAttackRate = sAttrData.wLastAttackRate;
		pAvatarAttr->m_wBaseDodgeRate = sAttrData.wBaseDodgeRate;
		pAvatarAttr->m_wLastDodgeRate = sAttrData.wLastDodgeRate;
		pAvatarAttr->m_wBaseBlockRate = sAttrData.wBaseBlockRate;
		pAvatarAttr->m_wLastBlockRate = sAttrData.wLastBlockRate;

		pAvatarAttr->m_wBaseCurseSuccessRate = sAttrData.wBaseCurseSuccessRate;
		pAvatarAttr->m_wLastCurseSuccessRate = sAttrData.wLastCurseSuccessRate;
		pAvatarAttr->m_wBaseCurseToleranceRate = sAttrData.wBaseCurseToleranceRate;
		pAvatarAttr->m_wLastCurseToleranceRate = sAttrData.wLastCurseToleranceRate;

		pAvatarAttr->m_wBasePhysicalCriticalRate = sAttrData.wBasePhysicalCriticalRate;
		pAvatarAttr->m_wLastPhysicalCriticalRate = sAttrData.wLastPhysicalCriticalRate;
		pAvatarAttr->m_wBaseEnergyCriticalRate = sAttrData.wBaseEnergyCriticalRate;
		pAvatarAttr->m_wLastEnergyCriticalRate = sAttrData.wLastEnergyCriticalRate;
		
		pAvatarAttr->m_wBaseAttackSpeedRate = sAttrData.wBaseAttackSpeedRate;
		pAvatarAttr->m_wLastAttackSpeedRate = sAttrData.wLastAttackSpeedRate;
		pAvatarAttr->SetBaseAttackRange(sAttrData.fBaseAttackRange);
		pAvatarAttr->SetAttackRange(sAttrData.fLastAttackRange);

		pAvatarAttr->SetRunSpeed(sAttrData.fLastRunSpeed);
		if(sAttrData.wLastAttackSpeedRate > 0)
			pAvatarAttr->SetAttackAnimSpeed((RwReal)1000/(RwReal)sAttrData.wLastAttackSpeedRate);

		pAvatarAttr->SetCastingTimeModifier( sAttrData.fCastingTimeChangePercent );
		pAvatarAttr->SetCoolingTimeModifier( sAttrData.fCoolTimeChangePercent );
		pAvatarAttr->SetKeepingTimeModifier( sAttrData.fKeepTimeChangePercent );
		pAvatarAttr->SetDOTValueModifier( sAttrData.fDotValueChangePercent );
		pAvatarAttr->SetDOTTimeModifier( sAttrData.fDotTimeChangeAbsolute );
		pAvatarAttr->SetRequiredEPModifier( sAttrData.fRequiredEpChangePercent );

		pAvatarAttr->m_fHonestOffence =	sAttrData.fHonestOffence;
		pAvatarAttr->m_fHonestDefence =	sAttrData.fHonestDefence;
		pAvatarAttr->m_fStrangeOffence = sAttrData.fStrangeOffence;
		pAvatarAttr->m_fStrangeDefence = sAttrData.fStrangeDefence;
		pAvatarAttr->m_fWildOffence = sAttrData.fWildOffence;
		pAvatarAttr->m_fWildDefence = sAttrData.fWildDefence;
		pAvatarAttr->m_fEleganceOffence	= sAttrData.fEleganceOffence;
		pAvatarAttr->m_fEleganceDefence	= sAttrData.fEleganceDefence;
		pAvatarAttr->m_fFunnyOffence = sAttrData.fFunnyOffence;
		pAvatarAttr->m_fFunnyDefence = sAttrData.fFunnyDefence;

		CNtlSLEventGenerator::SobUpdate( pAvatar->GetSerialID(), EVENT_AIUT_ATTR, 0);
	}

	else if(pSobObj->GetClassID() == SLCLASS_PET)
	{
		CNtlSobPetAttr* pPetAttr = reinterpret_cast<CNtlSobPetAttr*>( pSobObj->GetSobAttr() );

		sAVATAR_ATTRIBUTE sAttrData;

		sAttrData.wLastMaxLP = (RwUInt16)pPetAttr->GetMaxLp();
		sAttrData.wLastMaxEP = (RwUInt16)pPetAttr->GetMaxEp();
		sAttrData.wLastMaxRP = (RwUInt16)pPetAttr->GetMaxRp();

		sAttrData.byBaseStr = pPetAttr->m_byBaseStr;
		sAttrData.byLastStr = pPetAttr->m_byLastStr;
		sAttrData.byBaseCon = pPetAttr->m_byBaseCon;
		sAttrData.byLastCon = pPetAttr->m_byLastCon;
		sAttrData.byBaseFoc = pPetAttr->m_byBaseFoc;
		sAttrData.byLastFoc = pPetAttr->m_byLastFoc;
		sAttrData.byBaseDex = pPetAttr->m_byBaseDex;
		sAttrData.byLastDex = pPetAttr->m_byLastDex;
		sAttrData.byBaseSol = pPetAttr->m_byBaseSol;
		sAttrData.byLastSol = pPetAttr->m_byLastSol;
		sAttrData.byBaseEng = pPetAttr->m_byBaseEng;
		sAttrData.byLastEng = pPetAttr->m_byLastEng;

		sAttrData.wBasePhysicalOffence	= pPetAttr->m_wBasePhyshicalOffence;
		sAttrData.wLastPhysicalOffence	= pPetAttr->m_wLastPhyshicalOffence;
		sAttrData.wBasePhysicalDefence	= pPetAttr->m_wBasePhyshicalDefence;
		sAttrData.wLastPhysicalDefence	= pPetAttr->m_wLastPhyshicalDefence;
		sAttrData.wBaseEnergyOffence	= pPetAttr->m_wBaseEnergyOffence;
		sAttrData.wLastEnergyOffence	= pPetAttr->m_wLastEnergyOffence;
		sAttrData.wBaseEnergyDefence	= pPetAttr->m_wBaseEnergyDefence;
		sAttrData.wLastEnergyDefence	= pPetAttr->m_wLastEnergyDefence;

		sAttrData.wBaseAttackRate		= pPetAttr->m_wBaseAttackRate;
		sAttrData.wLastAttackRate		= pPetAttr->m_wLastAttackRate;
		sAttrData.wBaseDodgeRate		= pPetAttr->m_wBaseDodgeRate;
		sAttrData.wLastDodgeRate		= pPetAttr->m_wLastDodgeRate;
		sAttrData.wBaseBlockRate		= pPetAttr->m_wBaseBlockRate;
		sAttrData.wLastBlockRate		= pPetAttr->m_wLastBlockRate;
		sAttrData.wBaseAttackSpeedRate	= pPetAttr->m_wBaseAttackSpeedRate;
		sAttrData.wLastAttackSpeedRate	= pPetAttr->m_wLastAttackSpeedRate;

		sAttrData.wBaseCurseSuccessRate = pPetAttr->m_wBaseCurseSuccessRate;
		sAttrData.wLastCurseSuccessRate = pPetAttr->m_wLastCurseSuccessRate;
		sAttrData.wBaseCurseToleranceRate = pPetAttr->m_wBaseCurseToleranceRate;
		sAttrData.wLastCurseToleranceRate = pPetAttr->m_wLastCurseToleranceRate;

		sAttrData.wBasePhysicalCriticalRate = pPetAttr->m_wBasePhysicalCriticalRate;
		sAttrData.wLastPhysicalCriticalRate = pPetAttr->m_wLastPhysicalCriticalRate;
		sAttrData.wBaseEnergyCriticalRate	= pPetAttr->m_wBaseEnergyCriticalRate;
		sAttrData.wLastEnergyCriticalRate	= pPetAttr->m_wLastEnergyCriticalRate;

		sAttrData.fLastRunSpeed	= pPetAttr->GetRunSpeed();

		sAttrData.fCastingTimeChangePercent = pPetAttr->GetCastingTimeModifier();
		sAttrData.fCoolTimeChangePercent = pPetAttr->GetCoolingTimeModifier();
		sAttrData.fKeepTimeChangePercent = pPetAttr->GetKeepingTimeModifier();
		sAttrData.fDotValueChangePercent = pPetAttr->GetDOTValueModifier();
		sAttrData.fDotTimeChangeAbsolute = pPetAttr->GetDOTTimeModifier();
		sAttrData.fRequiredEpChangePercent = pPetAttr->GetRequiredEPModifier();

		// 데이터 적용.
		CNtlAvatar::UpdateAvatarAttribute(byAttributeTotalCount, pData, &sAttrData);

		pPetAttr->SetMaxLp((RwInt32)sAttrData.wLastMaxLP);
		pPetAttr->SetMaxEp((RwInt32)sAttrData.wLastMaxEP);
		pPetAttr->SetMaxRp((RwInt32)sAttrData.wLastMaxRP);

		pPetAttr->m_byBaseStr = sAttrData.byBaseStr;
		pPetAttr->m_byLastStr = sAttrData.byLastStr;
		pPetAttr->m_byBaseCon = sAttrData.byBaseCon;
		pPetAttr->m_byLastCon = sAttrData.byLastCon;
		pPetAttr->m_byBaseFoc = sAttrData.byBaseFoc;
		pPetAttr->m_byLastFoc = sAttrData.byLastFoc;
		pPetAttr->m_byBaseDex = sAttrData.byBaseDex;
		pPetAttr->m_byLastDex = sAttrData.byLastDex;
		pPetAttr->m_byBaseSol = sAttrData.byBaseSol;
		pPetAttr->m_byLastSol = sAttrData.byLastSol;
		pPetAttr->m_byBaseEng = sAttrData.byBaseEng;
		pPetAttr->m_byLastEng = sAttrData.byLastEng;

		pPetAttr->m_wBasePhyshicalOffence = sAttrData.wBasePhysicalOffence;
		pPetAttr->m_wLastPhyshicalOffence = sAttrData.wLastPhysicalOffence;
		pPetAttr->m_wBasePhyshicalDefence = sAttrData.wBasePhysicalDefence;
		pPetAttr->m_wLastPhyshicalDefence = sAttrData.wLastPhysicalDefence;

		pPetAttr->m_wBaseEnergyOffence = sAttrData.wBaseEnergyOffence;
		pPetAttr->m_wLastEnergyOffence = sAttrData.wLastEnergyOffence;
		pPetAttr->m_wBaseEnergyDefence = sAttrData.wBaseEnergyDefence;
		pPetAttr->m_wLastEnergyDefence = sAttrData.wLastEnergyDefence;

		pPetAttr->m_wBaseAttackRate = sAttrData.wBaseAttackRate;
		pPetAttr->m_wLastAttackRate = sAttrData.wLastAttackRate;
		pPetAttr->m_wBaseDodgeRate = sAttrData.wBaseDodgeRate;
		pPetAttr->m_wLastDodgeRate = sAttrData.wLastDodgeRate;
		pPetAttr->m_wBaseBlockRate = sAttrData.wBaseBlockRate;
		pPetAttr->m_wLastBlockRate = sAttrData.wLastBlockRate;
		pPetAttr->m_wBaseAttackSpeedRate	= sAttrData.wBaseAttackSpeedRate;
		pPetAttr->m_wLastAttackSpeedRate	= sAttrData.wLastAttackSpeedRate;

		pPetAttr->m_wBaseCurseSuccessRate = sAttrData.wBaseCurseSuccessRate;
		pPetAttr->m_wLastCurseSuccessRate = sAttrData.wLastCurseSuccessRate;
		pPetAttr->m_wBaseCurseToleranceRate = sAttrData.wBaseCurseToleranceRate;
		pPetAttr->m_wLastCurseToleranceRate = sAttrData.wLastCurseToleranceRate;

		pPetAttr->m_wBasePhysicalCriticalRate = sAttrData.wBasePhysicalCriticalRate;
		pPetAttr->m_wLastPhysicalCriticalRate = sAttrData.wLastPhysicalCriticalRate;
		pPetAttr->m_wBaseEnergyCriticalRate = sAttrData.wBaseEnergyCriticalRate;
		pPetAttr->m_wLastEnergyCriticalRate = sAttrData.wLastEnergyCriticalRate;


		pPetAttr->SetRunSpeed(sAttrData.fLastRunSpeed);
		if(sAttrData.wLastAttackSpeedRate > 0)
			pPetAttr->SetAttackAnimSpeed((RwReal)1000/(RwReal)sAttrData.wLastAttackSpeedRate);

		pPetAttr->SetCastingTimeModifier( sAttrData.fCastingTimeChangePercent );
		pPetAttr->SetCoolingTimeModifier( sAttrData.fCoolTimeChangePercent );
		pPetAttr->SetKeepingTimeModifier( sAttrData.fKeepTimeChangePercent );
		pPetAttr->SetDOTValueModifier( sAttrData.fDotValueChangePercent );
		pPetAttr->SetDOTTimeModifier( sAttrData.fDotTimeChangeAbsolute );
		pPetAttr->SetRequiredEPModifier( sAttrData.fRequiredEpChangePercent );
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// heading

void Logic_CharacterHeading(CNtlSobActor *pActor, RwUInt32 uiTargetSeriaIId)
{
	if( !pActor )
		return;

	CNtlSobActor *pTargetActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(uiTargetSeriaIId)); 
	if(pTargetActor)
	{
		RwV3d vPos = pActor->GetPosition(); 
		RwV3d vTargetPos = pTargetActor->GetPosition(); 

		RwV3d vDir;
		RwV3dSub(&vDir, &vTargetPos, &vPos); 
		pActor->SetDirection(&vDir); 
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// speed

void Logic_SetSpeed(CNtlSobActor *pActor, RwReal fWalkSpeed, RwReal fRunSpeed)
{
	if( !pActor )
		return;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>(pActor->GetSobAttr());
		pMoveableAttr->SetWalkSpeed(fWalkSpeed);
		pMoveableAttr->SetRunSpeed(fRunSpeed);
	}
}


RwReal Logic_GetDefaultFrontWalkSpeed(CNtlSobActor *pActor)
{
	if( !pActor )
		return 8.0f;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>(pActor->GetSobAttr());
		return pMoveableAttr->GetDefaultWalkSpeed();
	}

	return 8.0f;
}

RwReal Logic_GetFrontWalkSpeed(CNtlSobActor *pActor)
{
	if( !pActor )
		return 8.0f;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>(pActor->GetSobAttr());
		return pMoveableAttr->GetWalkSpeed();
	}

	return 8.0f;
}

RwReal Logic_GetDefaultFrontRunSpeed(CNtlSobActor *pActor)
{
	if( !pActor )
		return 8.0f;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>(pActor->GetSobAttr());
		return pMoveableAttr->GetDefaultRunSpeed();
	}

	return 8.0f;
}

RwReal Logic_GetFrontRunSpeed(CNtlSobActor *pActor)
{
	if( !pActor )
		return 8.0f;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>(pActor->GetSobAttr());
		return pMoveableAttr->GetRunSpeed();
	}

	return 8.0f;
}

RwReal Logic_GetDefaultBackSpeed(CNtlSobActor *pActor)
{
	if( !pActor )
		return 8.0f;

	return Logic_GetDefaultFrontRunSpeed(pActor)*NTL_BACKWARD_MOVEMENT_SPEED_RATE;
}

RwReal Logic_GetBackSpeed(CNtlSobActor *pActor)
{
	if( !pActor )
		return 8.0f;

	return Logic_GetFrontRunSpeed(pActor)*NTL_BACKWARD_MOVEMENT_SPEED_RATE;
}

RwReal Logic_GetTurningSpeed(CNtlSobActor *pActor)
{
	return 60.f;
}


RwBool Logic_IsActorWalkMove(CNtlSobActor *pActor, RwUInt8 byMoveForm)
{
	if( !pActor )
		return FALSE;

	RwUInt32 uiClassId = pActor->GetClassID();
	if(uiClassId == SLCLASS_MONSTER)
	{
		if(byMoveForm != NTL_MOVE_FLAG_RUN)
			return TRUE;
	}
	else if(uiClassId == SLCLASS_NPC)
	{
		if(byMoveForm != NTL_MOVE_FLAG_RUN)
			return TRUE;
	}

	return FALSE;
}

void Logic_SetAttackAnimSpeed(CNtlSobActor *pActor, RwReal fAnimSpeed)
{
	if( !pActor )
		return;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>(pActor->GetSobAttr());
		pMoveableAttr->SetAttackAnimSpeed(fAnimSpeed);
	}
}

RwReal Logic_GetAttackAnimSpeed(CNtlSobActor *pActor)
{
	if( !pActor )
		return 1.0f;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>(pActor->GetSobAttr());
		return pMoveableAttr->GetAttackAnimSpeed();
	}

	return 1.0f;
}

RwReal Logic_GetSkillAnimSpeed(CNtlSobActor *pActor, sSKILL_TBLDAT *pSkillTblData)
{
	if( !pActor )
		return 1.0f;

	CSystemEffectTable *pSystemEffTbl = API_GetTableContainer()->GetSystemEffectTable();

	for(RwInt32 i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
	{
		if(pSkillTblData->skill_Effect[i] == INVALID_TBLIDX)
			continue;

		sSYSTEM_EFFECT_TBLDAT *pSystemEffTblData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSystemEffTbl->FindData(pSkillTblData->skill_Effect[i]) );
		if(pSystemEffTblData)
		{
			RwUInt8 byEffectType = pSystemEffTblData->byActive_Effect_Type;
			if(byEffectType == SKILL_ACTIVE_TYPE_DD)
			{
				if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
				{
					CNtlSobMoveableAttr *pMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>(pActor->GetSobAttr());
					return pMoveableAttr->GetAttackAnimSpeed();
				}

				return 1.0f;
			}
		}
	}

	return 1.0f;
}

RwReal Logic_GetRunAnimSpeed( CNtlSobActor* pActor ) 
{
	if( !pActor )
		return 1.0f;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr* pMoveableAttr = (CNtlSobMoveableAttr*)pActor->GetSobAttr();
		return pMoveableAttr->GetRunAnimSpeed();
	}

	return 1.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// player 전직 

RwUInt8	Logic_GetPlayerRace(CNtlSobActor *pActor)
{
	if( !pActor )
		return RACE_UNKNOWN;

	RwUInt8 byRace = RACE_UNKNOWN;
	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_SOCIAL)
	{
		CNtlSobSocialAttr *pSocialAttr = reinterpret_cast<CNtlSobSocialAttr*>(pActor->GetSobAttr());
		if(pSocialAttr)
		{
			byRace = pSocialAttr->GetRace();
		}
	}

	return byRace;
}

RwUInt8 Logic_GetPlayerClass(CNtlSobActor *pActor)
{
	if( !pActor )
		return PC_CLASS_UNKNOWN;

	RwUInt8 byClass = PC_CLASS_UNKNOWN;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_SOCIAL)
	{
		CNtlSobSocialAttr *pSocialAttr = reinterpret_cast<CNtlSobSocialAttr*>(pActor->GetSobAttr());
		byClass = pSocialAttr->GetClass();
	}

	return byClass;
}

RwUInt8 Logic_GetPlayerGender( CNtlSobActor *pActor ) 
{
	if( !pActor )
		return GENDER_UNKNOWN;

	RwUInt8 byGender = GENDER_UNKNOWN;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_SOCIAL)
	{
		CNtlSobSocialAttr *pSocialAttr = reinterpret_cast<CNtlSobSocialAttr*>(pActor->GetSobAttr());
		byGender = pSocialAttr->GetGender();
	}    

	return byGender;
}

void Logic_SetReputation(CNtlSobActor *pActor, RwUInt32 uiReputation)
{
	if( !pActor )
		return;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_SOCIAL)
	{
		CNtlSobSocialAttr *pSocialAttr = reinterpret_cast<CNtlSobSocialAttr*>(pActor->GetSobAttr());
		pSocialAttr->SetReputation(uiReputation);
		CNtlSLEventGenerator::SobUpdate(pActor->GetSerialID(), EVENT_AIUT_ATTR, 0 );
	}    
}

RwUInt32 Logic_GetReputation(CNtlSobActor *pActor)
{
	if( !pActor )
		return 0;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_SOCIAL)
	{
		CNtlSobSocialAttr *pSocialAttr = reinterpret_cast<CNtlSobSocialAttr*>(pActor->GetSobAttr());
		return pSocialAttr->GetReputation();
	}

	return 0;
}

RwUInt8	Logic_ConverClasstoRace(RwUInt8 byClass)
{
	// 클래스가 어떤 종족에 속하는지 알아본다
	switch(byClass)
	{
	case PC_CLASS_HUMAN_FIGHTER:
	case PC_CLASS_HUMAN_MYSTIC:
	case PC_CLASS_HUMAN_ENGINEER:
	case PC_CLASS_STREET_FIGHTER:
	case PC_CLASS_SWORD_MASTER:
	case PC_CLASS_CRANE_ROSHI:
	case PC_CLASS_TURTLE_ROSHI:
	case PC_CLASS_GUN_MANIA:
	case PC_CLASS_MECH_MANIA:
		{
			return RACE_HUMAN;
		}
	case PC_CLASS_NAMEK_FIGHTER:
	case PC_CLASS_NAMEK_MYSTIC:
	case PC_CLASS_DARK_WARRIOR:
	case PC_CLASS_SHADOW_KNIGHT:
	case PC_CLASS_DENDEN_HEALER:
	case PC_CLASS_POCO_SUMMONER:
		{
			return RACE_NAMEK;
		}
	case PC_CLASS_MIGHTY_MAJIN:
	case PC_CLASS_WONDER_MAJIN:
	case PC_CLASS_GRAND_MA:
	case PC_CLASS_ULTI_MA:
	case PC_CLASS_PLAS_MA:
	case PC_CLASS_KAR_MA:
		{
			return RACE_MAJIN;
		}
	}

	NTL_ASSERTFAIL("Logic_ConverClasstoRace, Invalid class : " << byClass);    
	return RACE_UNKNOWN;
}

RwBool Logic_IsChild(CNtlSobActor *pActor)
{
	if( !pActor )
		return FALSE;

	CNtlSobSocialAttr* pSocialAttr = (CNtlSobSocialAttr*)pActor->GetSobAttr();
	return !pSocialAttr->IsAdult();
}

RwBool Logic_IsFirstClass(CNtlSobPlayer* pPlayer)
{
	if( !pPlayer )
		return FALSE;

	CNtlSobPlayerAttr* pPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pPlayer->GetSobAttr() );
	sPC_TBLDAT* pPCData = pPlayerAttr->GetPcTbl();

	switch( pPCData->byClass )
	{
	case PC_CLASS_HUMAN_FIGHTER:
	case PC_CLASS_HUMAN_MYSTIC:
	case PC_CLASS_HUMAN_ENGINEER:
	case PC_CLASS_NAMEK_FIGHTER:
	case PC_CLASS_NAMEK_MYSTIC:
	case PC_CLASS_MIGHTY_MAJIN:
	case PC_CLASS_WONDER_MAJIN:
		return TRUE;
	case PC_CLASS_STREET_FIGHTER:
	case PC_CLASS_SWORD_MASTER:
	case PC_CLASS_CRANE_ROSHI:
	case PC_CLASS_TURTLE_ROSHI:
	case PC_CLASS_GUN_MANIA:
	case PC_CLASS_MECH_MANIA:		
	case PC_CLASS_DARK_WARRIOR:
	case PC_CLASS_SHADOW_KNIGHT:
	case PC_CLASS_DENDEN_HEALER:
	case PC_CLASS_POCO_SUMMONER:
	case PC_CLASS_GRAND_MA:
	case PC_CLASS_ULTI_MA:
	case PC_CLASS_PLAS_MA:
	case PC_CLASS_KAR_MA:		
		return FALSE;
	}

	NTL_ASSERTFAIL( "Logic_IsFirstClass : PC Class is wrong " << pPCData->byClass );
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// attack range

RwReal Logic_GetRadius(CNtlSobActor *pActor)
{
	if( !pActor )
		return 0.f;

	RwReal fWidth = pActor->GetSobProxy()->GetPLEntityWidth();
	RwReal fDepth = pActor->GetSobProxy()->GetPLEntityDepth();

	return (fWidth + fDepth) / 4.0f;
}

RwReal Logic_GetAttackRange(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial)
{
	RwReal fAttackRange = 3.0f;

	if( !pActor )
		return fAttackRange;

	RwBool bNamekTransform = Logic_IsTransformGreatNamek(pActor);
	if(bNamekTransform)
	{
		fAttackRange = DBO_GREAT_NAMEK_ATTACK_RANGE;
	}
	else
	{
		if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_BATTLE)
		{
			CNtlSobBattleAttr *pSobBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>(pActor->GetSobAttr());
			fAttackRange =  pSobBattleAttr->GetAttackRange();
		}
	}

	RwUInt32 uiClassId = pActor->GetClassID();
	if(uiClassId == SLCLASS_PLAYER)
	{
		CNtlSobPlayer *pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( pActor );
		CNtlInventoryBase *pInventory = pSobPlayer->GetEquipInventory();
		RwUInt32 uiTblId = pInventory->GetEquipTableId(EQUIP_SLOT_TYPE_HAND);
		if(uiTblId != INVALID_TBLIDX)
		{
			CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
			sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(uiTblId) );

			if( !pItemTblData )
			{
				DBO_FAIL( "ItemTbl Must be Present!" );
				return fAttackRange;
			}
			
			fAttackRange += pItemTblData->fAttack_Range_Bonus;
		}
	}
	else if(uiClassId == SLCLASS_AVATAR)
	{
		CNtlSobAvatar *pSobAvatar = reinterpret_cast<CNtlSobAvatar*>( pActor );
		CNtlInventory *pInventory = pSobAvatar->GetInventory();
		SERIAL_HANDLE hItem = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_HAND);
		if(hItem != INVALID_SERIAL_ID)
		{
			CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hItem) );
			CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
			sITEM_TBLDAT *pItemTblData = pSobItemAttr->GetItemTbl();

			if( !pItemTblData )
			{
				DBO_FAIL( "ItemTbl Must be Present!" );
				return fAttackRange;
			}

			fAttackRange += pItemTblData->fAttack_Range_Bonus;
		}
	}

	CNtlSobActor *pSobTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(hTargetSerial) );
	if(pSobTarActor)
		fAttackRange += Logic_GetRadius(pSobTarActor);

	return fAttackRange;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sob community range

RwReal Logic_GetCommunityRange(CNtlSobActor *pActor)
{
	if( !pActor )
		return 2.0f;

	if(pActor->GetClassID() == SLCLASS_NPC)
	{
		if(Logic_IsBus(pActor))
		{
			return NTL_MAX_BUS_DISTANCE;
		}
		else
		{
			return NTL_MAX_MERCHANT_DISTANCE;
		}
	}
	else if(pActor->GetClassID() == SLCLASS_WORLD_ITEM)
	{
		return NTL_WORLD_ITEM_PICK_RANGE;
	}
	else if(pActor->GetClassID() == SLCLASS_TRIGGER_OBJECT)
	{
		CNtlSobTriggerObjectAttr *pTriggerAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>( pActor->GetSobAttr() );
		RwReal fWidth = pActor->GetSobProxy()->GetPLEntityWidth();
		RwReal fDepth = pActor->GetSobProxy()->GetPLEntityDepth();

		RwReal fDistance = (fWidth + fDepth) / 4.0f;

		sOBJECT_TBLDAT *pObjTblData = pTriggerAttr->GetTriggerObjectTbl();
		if(pObjTblData)
			return fDistance + (INVALID_BYTE == pObjTblData->byBoundaryDistance ? 0 : pObjTblData->byBoundaryDistance);
		else
			return fDistance + NTL_TRIGGER_COMMUNITY_IN_RANGE;
	}
	else if(pActor->GetClassID() == SLCLASS_DYNAMIC_OBJECT)
	{
		CNtlSobDynamicObjectAttr* pDynamicAttr = reinterpret_cast<CNtlSobDynamicObjectAttr*>( pActor->GetSobAttr() );
		RwReal fWidth = pActor->GetSobProxy()->GetPLEntityWidth();
		RwReal fDepth = pActor->GetSobProxy()->GetPLEntityDepth();

		RwReal fDistance = (fWidth + fDepth) / 4.0f;

		sDYNAMIC_OBJECT_TBLDAT* pDynamicOTbldata = pDynamicAttr->GetDynamicObjectTbl();
		if( pDynamicAttr )
			return fDistance + (INVALID_BYTE == pDynamicOTbldata->byBoundaryDistance ? 0 : pDynamicOTbldata->byBoundaryDistance);
		else
			return fDistance + NTL_DYNAMIC_COMMUNITY_IN_RANGE;
	}

	return 2.0f;
}

RwReal Logic_GetCommunityOutRange(CNtlSobActor *pActor)
{
	if( !pActor )
		return 2.0f;

	if( pActor->GetClassID() == SLCLASS_NPC )
	{
		return NTL_NPC_COMMUNITY_OUT_RANGE;
	}
	else if( pActor->GetClassID() == SLCLASS_WORLD_ITEM )
	{
		return NTL_WORLD_ITEM_PICK_RANGE;
	}
	else if( pActor->GetClassID() == SLCLASS_TRIGGER_OBJECT )
	{
		RwReal fWidth = pActor->GetSobProxy()->GetPLEntityWidth();
		RwReal fDepth = pActor->GetSobProxy()->GetPLEntityDepth();

		return NTL_TRIGGER_COMMUNITY_OUT_RANGE + (fWidth + fDepth) * 0.5f;
	}

	return 2.0f;
}

void Logic_BeginActorCommunity(CNtlSobActor *pActor, CNtlSobActor *pTarActor)
{
	if( !pActor )
		return;

	// 현재 스카우터 사용 상태면 아무것도 하지 않는다.
	if(FSMUtil_IsScouterUse(pActor->GetCurrentState()))
	{
		return;
	}

	RwUInt32 uiTarClassId = pTarActor->GetClassID();
	if(uiTarClassId == SLCLASS_NPC)  
	{
		CNtlSLEventGenerator::SobNpcCommunity(pTarActor->GetSerialID(), TRUE, Logic_GetNPCTriggerId(pTarActor), eEVENT_GEN_TYPE_CLICK_NPC);         
	}
	else if(uiTarClassId == SLCLASS_WORLD_ITEM)
		CNtlSLEventGenerator::SobPickWorldItem(pTarActor->GetSerialID());
	else if(uiTarClassId == SLCLASS_TRIGGER_OBJECT)
		CNtlSLEventGenerator::SobTriggerObjCommunity(pTarActor->GetSerialID(), TRUE, Logic_GetTriggerObjectId(pTarActor), eEVENT_GEN_TYPE_CLICK_OBJECT);
	else if(uiTarClassId == SLCLASS_DYNAMIC_OBJECT)
		CNtlSLEventGenerator::SobDynamicObjCommunity(pTarActor->GetSerialID(), TRUE );
	else if(uiTarClassId == SLCLASS_PLAYER)
	{
		if(FSMUtil_IsPrivateShopping(pTarActor->GetCurrentState()))
		{
			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( pTarActor->GetBehaviorData() );
			SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
			CNtlSLEventGenerator::SobPrivateShopCommunity(pTarActor->GetSerialID(), 
				pCtrlStuff->uExtra.sPrivateShop.bShopOwnerEmpty, 
				pCtrlStuff->uExtra.sPrivateShop.byShopState);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// skill use range

RwReal Logic_GetSkillUseRange(sSKILL_TBLDAT *pSkillTblData)
{
	if( !pSkillTblData )
		return 0.f;

	RwReal fSkillUseRange = pSkillTblData->fUse_Range_Max;
	if(pSkillTblData->bDash_Able)
		fSkillUseRange += (RwUInt8)NTL_MAX_DASH_DISTANCE_FOR_SKILL;

	return fSkillUseRange;
}

RwBool Logic_IsMasterSkill(SERIAL_HANDLE hSkillSerial)
{
	CNtlSobSkill *pSobSkill = reinterpret_cast<CNtlSobSkill*>( GetNtlSobManager()->GetSobObject(hSkillSerial) );
	if(pSobSkill == NULL)
		return FALSE;

	CNtlSobSkillAttr *pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobSkill->GetSobAttr() );
	if(pSobSkillAttr == NULL)
		return FALSE;

	return pSobSkillAttr->IsMasterSkill();
}


RwReal Logic_GetSkillApplyRange(CNtlSobActor* pActor, RwUInt32 uiSkillTblidx)
{
	// 아바타만 사용 가능
	if(!pActor || pActor->GetClassID() != SLCLASS_AVATAR)
		return 0.0f;

	CNtlSobAvatar* pAvatar = (CNtlSobAvatar*)pActor;
	CNtlSobSkill* pSobSkill = (CNtlSobSkill*)pAvatar->GetSkillContainer()->GetSkillFromTableId(uiSkillTblidx);
	if(!pSobSkill)
		return 0.0f;

	CNtlSobSkillAttr *pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobSkill->GetSobAttr() );
	if(pSobSkillAttr == NULL)
		return 0.0f;

	return pSobSkillAttr->GetSkillTbl()->byApply_Area_Size_1;
}

RwReal Logic_GetCurrentCastingTime(CNtlSobActor *pActor)
{
	if( !pActor )
		return 0.f;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
	return pSkillStuff->fCurrCastingTime;
}

RwReal Logic_GetMaxCastingTime(CNtlSobActor *pActor)
{
	if( !pActor )
		return 0.f;

	CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
	SSkillStuff *pSkillStuff = pBeData->GetSkillStuff();
	return pSkillStuff->fMaxCastingTime;
}

void Logic_SetSkillReqFlag(RwBool bFlag)
{
	CNtlSobSkillIcon::m_bUseReq = bFlag;
	if(!bFlag)
		CNtlSobSkillIcon::m_fUseResTimeOut = 0.0f;
}

void Logic_UpdateSkillResTimeOut(RwReal fElapsed)
{
	if(CNtlSobSkillIcon::m_bUseReq)
	{
		CNtlSobSkillIcon::m_fUseResTimeOut += fElapsed;
		if(CNtlSobSkillIcon::m_fUseResTimeOut >= 5.0f)
			Logic_SetSkillReqFlag(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Active, HTB Skill Time Notifier

RwReal Logic_GetCastingTimeFactor( CNtlSobActor* pActor )
{
	if( !pActor )
		return 1.f;

	if( pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_BATTLE )
	{
		CNtlSobBattleAttr* pBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>( pActor->GetSobAttr() );
		return 1.0f + pBattleAttr->GetCastingTimeModifier() * 0.01f;
	}

	return 1.0f;
}

RwReal Logic_GetCoolingTimeFactor( CNtlSobActor* pActor )
{
	if( !pActor )
		return 1.f;

	if( pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_BATTLE )
	{
		CNtlSobBattleAttr* pBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>( pActor->GetSobAttr() );
		return 1.0f + pBattleAttr->GetCoolingTimeModifier() * 0.01f;
	}

	return 1.0f;
}

RwReal Logic_GetKeepingTimeFactor( CNtlSobActor* pActor )
{
	if( !pActor )
		return 1.f;

	if( pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_BATTLE )
	{
		CNtlSobBattleAttr* pBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>( pActor->GetSobAttr() );
		return 1.0f + pBattleAttr->GetKeepingTimeModifier() * 0.01f;
	}

	return 1.0f;
}

RwReal Logic_GetDOTValueFactor( CNtlSobActor* pActor )
{
	if( !pActor )
		return 1.f;

	if( pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_BATTLE )
	{
		CNtlSobBattleAttr* pBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>( pActor->GetSobAttr() );
		return 1.0f + pBattleAttr->GetDOTValueModifier() * 0.01f;
	}

	return 1.0f;
}

RwReal Logic_GetDOTTimeFactor( CNtlSobActor* pActor )
{
	if( !pActor )
		return 0.f;

	if( pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_BATTLE )
	{
		CNtlSobBattleAttr* pBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>( pActor->GetSobAttr() );
		return pBattleAttr->GetDOTTimeModifier();
	}

	return 0.0f;
}

RwReal Logic_GetRequiredEPFactor( CNtlSobActor* pActor )
{
	if( !pActor )
		return 1.f;

	if( pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_BATTLE )
	{
		CNtlSobBattleAttr* pBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>( pActor->GetSobAttr() );
		return 1.0f + pBattleAttr->GetRequiredEPModifier() * 0.01f;
	}

	return 1.0f;
}



//RwReal Logic_GetCastingTimeModifier( CNtlSobActor* pActor )
//{
//	if( !pActor )
//		return 0.f;
//
//	if( pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_BATTLE )
//	{
//		CNtlSobBattleAttr* pBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>( pActor->GetSobAttr() );
//		return pBattleAttr->GetCastingTimeModifier();
//	}
//
//	return 0.0f;
//}
//
//RwReal Logic_GetCoolingTimeModifier( CNtlSobActor* pActor )
//{
//	if( !pActor )
//		return 0.f;
//
//	if( pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_BATTLE )
//	{
//		CNtlSobBattleAttr* pBattleAttr = reinterpret_cast<CNtlSobBattleAttr*>( pActor->GetSobAttr() );
//		return pBattleAttr->GetCoolingTimeModifier();
//	}
//
//	return 0.0f;
//}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor의 상태 및 mode

// actor가 community actor인가?(npc, trigger object, world item)
RwBool Logic_IsCommunityActor(CNtlSobActor *pActor)
{
	if( !pActor )
		return FALSE;

	if(pActor->GetFlags() & SLFLAG_TARGET_NEAR_APPROACH)
		return TRUE;

	return FALSE;
}

RwBool Logic_IsGambleActor( CNtlSobActor *pActor )
{
	if( !pActor )
		return FALSE;

	if(pActor->GetClassID() != SLCLASS_NPC)
		return FALSE;

	CNtlSobNpcAttr* pNpcAttr = reinterpret_cast<CNtlSobNpcAttr*>(pActor->GetSobAttr());

	if( pNpcAttr == NULL )
		return FALSE;

	sNPC_TBLDAT* pTableData = pNpcAttr->GetNpcTbl();

	if( pTableData == NULL )
		return FALSE;

	if( pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_GAMBLE_MERCHANT )
		return TRUE;

	return FALSE;
}

// operation trigger actor 인가?
RwBool Logic_IsOperationTriggerActor(CNtlSobActor *pActor)
{
	if( !pActor )
		return FALSE;

	if(pActor->GetFlags() & SLFLAG_TARGET_TRIGGER_OPERATION)
		return TRUE;

	return FALSE;
}

RwUInt32 Logic_GetDirectIdTriggerActor(CNtlSobActor *pActor)
{
	if( !pActor )
		return INVALID_TBLIDX;

	CNtlSobTriggerObject *pTriggerObject = reinterpret_cast<CNtlSobTriggerObject*>(pActor);
	CNtlSobTriggerObjectAttr *pTriggerObjectAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>(pTriggerObject->GetSobAttr());
	sOBJECT_TBLDAT *pObjTblData = pTriggerObjectAttr->GetTriggerObjectTbl();

	return pObjTblData->objectDirectionIndex;
}

// actor의 state id를 넘겨준다.
RwUInt32 Logic_GetActorStateId(CNtlSobActor *pActor)
{
	if( !pActor )
		return NTL_FSMSID_INVALID;

	CNtlFSMLayer *pFSMLayer = pActor->GetFSMLayer();
	if(pFSMLayer == NULL)
		return NTL_FSMSID_INVALID;

	CNtlFSMStateBase *pStateBase = pFSMLayer->GetCurrentState();
	if(!pStateBase)
		return NTL_FSMSID_INVALID;


	return pStateBase->GetStateId();    
}


RwUInt32 Logic_GetActorStateFlags( CNtlSobActor* pActor ) 
{
	if( !pActor )
		return NTL_FSMSID_INVALID;

	CNtlFSMLayer *pFSMLayer = pActor->GetFSMLayer();
	if(pFSMLayer == NULL)
		return NTL_FSMSID_INVALID;

	CNtlFSMStateBase *pStateBase = pFSMLayer->GetCurrentState();
	if(!pStateBase)
		return NTL_FSMSID_INVALID;


	return pStateBase->GetFlags();
}

// actor가 target 하고 있는 serial id
SERIAL_HANDLE Logic_GetActorTargetSerialId(CNtlSobActor *pActor)
{
	if( !pActor )
		return INVALID_SERIAL_ID;

	RwUInt32 uiClassId = pActor->GetClassID();
	if(uiClassId == SLCLASS_AVATAR || uiClassId == SLCLASS_PLAYER)
	{
		CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>( pActor->GetBehaviorData() );
		SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
		return pCtrlStuff->hTargetSerialId;
	}
	else 
	{
		return INVALID_SERIAL_ID;
	}
}


// actor 현재 전투중인가?
RwBool Logic_IsActorBattleMode(CNtlSobActor *pActor)
{
	if( !pActor )
		return FALSE;

	RwUInt32 uiActorState = Logic_GetActorStateId(pActor);

	if( uiActorState == NTL_FSMSID_FIGHTING || uiActorState == NTL_FSMSID_FIGHTING_SHRINK ||
		uiActorState == NTL_FSMSID_FIGHTING_POSE || uiActorState == NTL_FSMSID_HURT || 
		uiActorState == NTL_FSMSID_KNOCKDOWN || uiActorState == NTL_FSMSID_SLIDING ||
		uiActorState == NTL_FSMSID_STUN || uiActorState == NTL_FSMSID_SLEEP ||
		uiActorState == NTL_FSMSID_PARALYZE || uiActorState == NTL_FSMSID_SKILL_FOLLOW || 
		uiActorState == NTL_FSMSID_SKILL_READY || uiActorState == NTL_FSMSID_SKILL_CASTING ||
		uiActorState == NTL_FSMSID_SKILL_ACTION)
	{
		return TRUE;
	}

	return FALSE;
}

RwBool Logic_CanSkillUse(CNtlSobActor *pActor)
{
	if( !pActor )
		return FALSE;

	if( !(pActor->GetFlags() & SLFLAG_CAN_SKILLUSE) )
		return FALSE;

	CNtlFSMLayer *pFSMLayer = pActor->GetFSMLayer();
	if(pFSMLayer == NULL)
		return FALSE;

	CNtlFSMStateBase *pStateBase = pFSMLayer->GetCurrentState();
	return FSMUtil_CanSkillUse(pStateBase);
}


int DepthSortSkillAreaFunc(const void *a, const void *b)
{
	if( !a )
		return 1;

	const SSkillAreaSortStuff *ptrA = (const SSkillAreaSortStuff*)(a);
	//const SSkillAreaSortStuff *ptrB = (const SSkillAreaSortStuff*)(b);

	RwReal fDepthA = ptrA->fDist;
	RwReal fDepthB = ptrA->fDist;

	if (fDepthA > fDepthB) return -1;
	else if (fDepthA == fDepthB) return 0;
	else return 1;
}


RwLine Logic_GetSkillAreaFrontLine(CNtlSob *pSobObj, CNtlSob *pSobTarObj, RwReal fAreaHalfWidth, RwReal fAreaDepth)
{
	RwLine Line;

	Line.start.x	= 0.f;
	Line.start.y	= 0.f;
	Line.start.z	= 0.f;

	Line.end = Line.start;

	if( !pSobObj || !pSobTarObj )
		return Line;

	RwV3d vAvatarPos = pSobObj->GetPosition();
	RwV3d vTarPos = pSobTarObj->GetPosition();

	RwReal fAvatarModelHalfHeight = pSobObj->GetSobProxy()->GetPLEntityHeight()/2.0f;
	RwReal fTargetModelHalfHeight = pSobTarObj->GetSobProxy()->GetPLEntityHeight()/2.0f;

	CNtlMath::MathRwV3dAssign(&Line.start, vAvatarPos.x, vAvatarPos.y + fAvatarModelHalfHeight, vAvatarPos.z);
	CNtlMath::MathRwV3dAssign(&Line.end, vTarPos.x, vTarPos.y + fTargetModelHalfHeight, vTarPos.z);

	return Line;
}

RwSphere Logic_GetSkillAreaTargetSphere(CNtlSob *pSobTarObj, RwReal fRadius)
{
	RwSphere Sphere;

	Sphere.center.x	= 0.f;
	Sphere.center.y	= 0.f;
	Sphere.center.z	= 0.f;

	Sphere.radius	= 0.f;

	if( !pSobTarObj )
		return Sphere;

	RwV3d vTarPos = pSobTarObj->GetPosition();
	RwReal fTargetModelHalfHeight = pSobTarObj->GetSobProxy()->GetPLEntityHeight()/2.0f;

	CNtlMath::MathRwV3dAssign(&Sphere.center, vTarPos.x, vTarPos.y + fTargetModelHalfHeight, vTarPos.z);

	Sphere.radius = fRadius;

	return Sphere;
}

void Logic_FindSkillAreaEnemy(std::list<CNtlSob*>& listEntity)
{
	CNtlSobGroup *pSobGroup;
	CNtlSobGroup::MapObject::iterator it;

	pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_MONSTER);
	CNtlSobGroup::MapObject& mapMonster = pSobGroup->GetObjects();
	if(pSobGroup)
	{
		for(it = mapMonster.begin(); it != mapMonster.end(); it++)
			listEntity.push_back((*it).second);
	}

	pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_PLAYER);
	if(pSobGroup)
	{
		CNtlSobGroup::MapObject& mapPlayer = pSobGroup->GetObjects();
		for(it = mapPlayer.begin(); it != mapPlayer.end(); it++)
			listEntity.push_back((*it).second);
	}
}

void Logic_FindSkillAreaAlliance(std::list<CNtlSob*>& listEntity)
{
	CNtlSobGroup *pSobGroup;
	CNtlSobGroup::MapObject::iterator it;

	pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_PLAYER);
	if(pSobGroup)
	{
		CNtlSobGroup::MapObject& mapPlayer = pSobGroup->GetObjects();
		for(it = mapPlayer.begin(); it != mapPlayer.end(); it++)
			listEntity.push_back((*it).second);
	}

	pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_NPC);
	if(pSobGroup)
	{
		CNtlSobGroup::MapObject& mapPlayer = pSobGroup->GetObjects();
		for(it = mapPlayer.begin(); it != mapPlayer.end(); it++)
			listEntity.push_back((*it).second);
	}
}

void Logic_FindSkillAreaParty(std::list<CNtlSob*>& listEntity)
{
	CNtlSobAvatar *pSobAvatar	= GetNtlSLGlobal()->GetSobAvatar();
	CNtlParty *pParty	= pSobAvatar->GetParty();
	COMMUNITY_ITER it_Begin = pParty->Begin();
	COMMUNITY_ITER it_End = pParty->End();

	CNtlSob *pSobObj;

	for( ; it_Begin != it_End ; ++it_Begin )
	{
		sPartyMember* pMember = reinterpret_cast<sPartyMember*>( *it_Begin );
		pSobObj = GetNtlSobManager()->GetSobObject(pMember->hSerial);
		if(pSobObj)
			listEntity.push_back(pSobObj);
	}
}

void Logic_FindSkillAreaAny(std::list<CNtlSob*>& listEntity)
{
	CNtlSobGroup *pSobGroup;
	CNtlSobGroup::MapObject::iterator it;

	pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_MONSTER);
	CNtlSobGroup::MapObject& mapMonster = pSobGroup->GetObjects();
	if(pSobGroup)
	{
		for(it = mapMonster.begin(); it != mapMonster.end(); it++)
			listEntity.push_back((*it).second);
	}

	pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_PLAYER);
	if(pSobGroup)
	{
		CNtlSobGroup::MapObject& mapPlayer = pSobGroup->GetObjects();
		for(it = mapPlayer.begin(); it != mapPlayer.end(); it++)
			listEntity.push_back((*it).second);
	}

	pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_NPC);
	if(pSobGroup)
	{
		CNtlSobGroup::MapObject& mapNpc = pSobGroup->GetObjects();
		for(it = mapNpc.begin(); it != mapNpc.end(); it++)
			listEntity.push_back((*it).second);
	}

	pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_PET);
	if(pSobGroup)
	{
		CNtlSobGroup::MapObject& mapPet = pSobGroup->GetObjects();
		for(it = mapPet.begin(); it != mapPet.end(); it++)
			listEntity.push_back((*it).second);
	}
}

RwBool Logic_FindSkillArea(RwUInt8 byApplyTarget, std::list<CNtlSob*>& listEntity)
{
	RwBool bSuccess = TRUE;

	if(byApplyTarget == DBO_SKILL_APPLY_TARGET_ENEMY)
		Logic_FindSkillAreaEnemy(listEntity);
	else if(byApplyTarget == DBO_SKILL_APPLY_TARGET_ALLIANCE)
		Logic_FindSkillAreaAlliance(listEntity);
	else if(byApplyTarget == DBO_SKILL_APPLY_TARGET_PARTY)
		Logic_FindSkillAreaParty(listEntity);
	else if(byApplyTarget == DBO_SKILL_APPLY_TARGET_ANY)
		Logic_FindSkillAreaAny(listEntity);
	else
		bSuccess = FALSE;

	return bSuccess;
}


RwBool Logic_FindSkillApplyValidTarget(RwUInt8 byApplyTarget, SERIAL_HANDLE hSerialId)
{
	if(byApplyTarget == DBO_SKILL_APPLY_TARGET_ENEMY)
	{
		if(!Logic_IsEnemyTargetFromAvatarActor(hSerialId))
			return FALSE;
	}
	else if(byApplyTarget == DBO_SKILL_APPLY_TARGET_ALLIANCE)
	{
		if(Logic_IsEnemyTargetFromAvatarActor(hSerialId))
			return FALSE;
	}
	else if(byApplyTarget == DBO_SKILL_APPLY_TARGET_PARTY)
	{
		if(Logic_IsEnemyTargetFromAvatarActor(hSerialId))
			return FALSE;
	}
	else if(byApplyTarget == DBO_SKILL_APPLY_TARGET_ANY)
	{
		return TRUE;
	}
	else 
	{
		return FALSE;
	}

	return TRUE;
}

void Logic_GetSkillAreaCircle(SERIAL_HANDLE hTargetSerial, SSkillAreaSortStuff *pSortStuff, RwInt32& iValidNum, sSKILL_TBLDAT *pSkillTblData)
{
	if( !pSkillTblData )
		return;

	std::list<CNtlSob*>::iterator it;
	std::list<CNtlSob*> listSobObj;

	RwUInt8 byApplyTarget = pSkillTblData->byApply_Target;

	if(!Logic_FindSkillArea(byApplyTarget, listSobObj))
		return;

	RwV3d vPos, vSub;

	CNtlSob *pSobObj;
	CNtlSob *pSobTarObj			= GetNtlSobManager()->GetSobObject(hTargetSerial);
	NTL_ASSERT(pSobTarObj, "Not Target Object");
	if(!pSobTarObj)
		return;

	RwV3d vTarPos				= pSobTarObj->GetPosition();

	for(it = listSobObj.begin(); it != listSobObj.end(); it++)
	{
		if(iValidNum >= 100)
			return;

		pSobObj = (*it);
		if(pSobObj->GetSerialID() == hTargetSerial)
			continue;

		if(!Logic_FindSkillApplyValidTarget(byApplyTarget, pSobObj->GetSerialID()))
			continue;

		vPos = pSobObj->GetPosition();
		RwReal fRange = (RwReal)pSkillTblData->byApply_Area_Size_1;
		RwV3dSub(&vSub, &vPos, &vTarPos);
		RwReal fDist = RwV3dLength(&vSub);

		if(fDist < fRange)
		{
			pSortStuff[iValidNum].fDist = fDist;
			pSortStuff[iValidNum].hSerialId = pSobObj->GetSerialID();
			iValidNum++;
		}
	}
}


void Logic_GetSkillAreaRectangle(SERIAL_HANDLE hTargetSerial, SSkillAreaSortStuff *pSortStuff, RwInt32& iValidNum, sSKILL_TBLDAT *pSkillTblData)
{
	if( !pSkillTblData )
		return;

	std::list<CNtlSob*>::iterator it;
	std::list<CNtlSob*> listSobObj;

	RwUInt8 byApplyTarget = pSkillTblData->byApply_Target;

	if(!Logic_FindSkillArea(byApplyTarget, listSobObj))
		return;

	RwV3d vPos, vSub;

	CNtlSob *pSobObj;
	CNtlSob *pSobTarObj			= GetNtlSobManager()->GetSobObject(hTargetSerial);
	NTL_ASSERT(pSobTarObj, "Not Target Object");
	if(!pSobTarObj)
		return;
	RwV3d vTarPos				= pSobTarObj->GetPosition();

	for(it = listSobObj.begin(); it != listSobObj.end(); it++)
	{
		if(iValidNum >= 100)
			return;

		pSobObj = (*it);
		if(pSobObj->GetSerialID() == hTargetSerial)
			continue;

		if(!Logic_FindSkillApplyValidTarget(byApplyTarget, pSobObj->GetSerialID()))
			continue;

		vPos = pSobObj->GetPosition();
		RwReal fRange = (RwReal)pSkillTblData->byApply_Area_Size_1;
		RwV3dSub(&vSub, &vPos, &vTarPos);
		RwReal fDist = RwV3dLength(&vSub);

		if(fDist < fRange)
		{
			pSortStuff[iValidNum].fDist = fDist;
			pSortStuff[iValidNum].hSerialId = pSobObj->GetSerialID();
			iValidNum++;
		}
	}
}

void Logic_GetSkillAreaForward(SERIAL_HANDLE hTargetSerial, SSkillAreaSortStuff *pSortStuff, RwInt32& iValidNum, sSKILL_TBLDAT *pSkillTblData)
{
	if( !pSkillTblData )
		return;

	std::list<CNtlSob*>::iterator it;
	std::list<CNtlSob*> listSobObj;

	RwUInt8 byApplyTarget = pSkillTblData->byApply_Target;

	if(!Logic_FindSkillArea(byApplyTarget, listSobObj))
		return;

	RwLine LineFront;
	RwSphere Sphere;
	RwReal fDistance;
	RwReal fAreaHalfWidth, fAreaDepth;
	RwV3d vPos, vSub;

	CNtlSob *pSobObj;
	CNtlSobAvatar *pSobAvatar	= GetNtlSLGlobal()->GetSobAvatar();
	CNtlSob *pSobTarObj			= GetNtlSobManager()->GetSobObject(hTargetSerial);
	NTL_ASSERT(pSobTarObj, "Not Target Object");
	if(!pSobTarObj)
		return;
	RwV3d vTarPos				= pSobTarObj->GetPosition();

	fAreaHalfWidth = (RwReal)pSkillTblData->byApply_Area_Size_1/2.0f;
	fAreaDepth = (RwReal)pSkillTblData->byApply_Area_Size_2;

	LineFront = Logic_GetSkillAreaFrontLine(pSobAvatar, pSobTarObj, fAreaHalfWidth, fAreaDepth);

	for(it = listSobObj.begin(); it != listSobObj.end(); it++)
	{
		if(iValidNum >= 100)
			return;

		pSobObj = (*it);
		if(pSobObj->GetSerialID() == hTargetSerial)
			continue;

		if(!Logic_FindSkillApplyValidTarget(byApplyTarget, pSobObj->GetSerialID()))
			continue;

		vPos = pSobObj->GetPosition();
		RwV3dSub(&vSub, &vPos, &vTarPos);
		RwReal fDist = RwV3dLength(&vSub);

		Sphere = Logic_GetSkillAreaTargetSphere(pSobObj, fAreaHalfWidth);

		if(RtLineSphereIntersectionTest(&LineFront, &Sphere, &fDistance))
		{
			pSortStuff[iValidNum].fDist = fDist;
			pSortStuff[iValidNum].hSerialId = pSobObj->GetSerialID();
			iValidNum++;
		}
	}
}

void Logic_GetSkillAreaTargetParty(SERIAL_HANDLE hTargetSerial, SSkillAreaSortStuff *pSortStuff, RwInt32& iValidNum, sSKILL_TBLDAT *pSkillTblData)
{
	if( !pSkillTblData )
		return;

	RwV3d vPos, vSub;
	CNtlSob *pSobObj;

	CNtlSob *pSobTarObj			= GetNtlSobManager()->GetSobObject(hTargetSerial);
	RwV3d vTarPos				= pSobTarObj->GetPosition();

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlParty *pParty = pSobAvatar->GetParty();
	COMMUNITY_ITER it_Begin = pParty->Begin();
	COMMUNITY_ITER it_End = pParty->End();

	for( ; it_Begin != it_End ; ++it_Begin )
	{
		sPartyMember *pMember = reinterpret_cast<sPartyMember*>( *it_Begin );
		if(pMember->hSerial == hTargetSerial)
			continue;

		pSobObj = GetNtlSobManager()->GetSobObject(pMember->hSerial);
		if(pSobObj == NULL)
			continue;


		vPos = pSobObj->GetPosition();
		RwReal fRange = (RwReal)pSkillTblData->byApply_Area_Size_1;
		RwV3dSub(&vSub, &vPos, &vTarPos);
		RwReal fDist = RwV3dLength(&vSub);

		if(fDist < fRange)
		{
			pSortStuff[iValidNum].fDist = fDist;
			pSortStuff[iValidNum].hSerialId = pSobObj->GetSerialID();
			iValidNum++;
		}
	}
}

void Logic_GetAvatarSkillAcpplyAreaActor(SERIAL_HANDLE hTargetSerial, sSKILL_TBLDAT *pSkillTblData, std::list<SERIAL_HANDLE>& listApplyActor)
{
	if( !pSkillTblData )
		return;

	RwUInt8 byApplyRange	= pSkillTblData->byApply_Range;

	RwInt32 iValidNum = 0;
	static SSkillAreaSortStuff sAreaSortStuff[100];

	switch(byApplyRange)
	{
	case DBO_SKILL_APPLY_RANGE_SINGLE:
		break;
	case DBO_SKILL_APPLY_RANGE_CIRCLE:
		Logic_GetSkillAreaCircle(hTargetSerial, sAreaSortStuff, iValidNum, pSkillTblData);
		break;
	case DBO_SKILL_APPLY_RANGE_RECTANGLE:
		Logic_GetSkillAreaRectangle(hTargetSerial, sAreaSortStuff, iValidNum, pSkillTblData);
		break;
	case DBO_SKILL_APPLY_RANGE_FORWARD:
		Logic_GetSkillAreaForward(hTargetSerial, sAreaSortStuff, iValidNum, pSkillTblData);
		break;
	}

	listApplyActor.push_back(hTargetSerial);

	if(iValidNum > 0)
	{
		qsort((void*)sAreaSortStuff, iValidNum, sizeof(SSkillAreaSortStuff), DepthSortSkillAreaFunc);

		if(iValidNum > pSkillTblData->byApply_Target_Max - 1)
			iValidNum = pSkillTblData->byApply_Target_Max - 1;

		for(RwInt32 i = 0; i < iValidNum; i++)
			listApplyActor.push_back(sAreaSortStuff[i].hSerialId);
	}
}

RwBool Logic_IsNotFollowMoveSkill(CNtlSobActor *pActor, RwUInt8 bySlotIdx)
{
	if( !pActor )
		return FALSE;

	sSKILL_TBLDAT *pSkillTblData = NULL;

	if(pActor->GetClassID() == SLCLASS_AVATAR)
	{
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlSkillContainer *pSkillContainer = pSobAvatar->GetSkillContainer();
		pSkillTblData = pSkillContainer->GetSkillFromTable(bySlotIdx);

		CSystemEffectTable *pSysEffTable = API_GetTableContainer()->GetSystemEffectTable();

		for(RwInt32 i = 0;i < NTL_MAX_EFFECT_IN_SKILL; i++)
		{
			if(pSkillTblData->skill_Effect[i] == INVALID_TBLIDX)
				continue;

			sSYSTEM_EFFECT_TBLDAT *pSysEffTblData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSysEffTable->FindData(pSkillTblData->skill_Effect[i]) );

			if(!pSysEffTblData && pSysEffTblData->effectCode == ACTIVE_VISIT)
				return TRUE;
		}
	}

	return FALSE;
}

void Logic_GetAvatarSkillApplyArea(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial, RwUInt8 bySlotIdx, std::list<SERIAL_HANDLE>& listApplyActor)
{
	if( !pActor )
		return;

	sSKILL_TBLDAT *pSkillTblData = NULL;

	if(pActor->GetClassID() == SLCLASS_PET)
	{
		CNtlSobPet *pSobPet = reinterpret_cast<CNtlSobPet*>( pActor );
		CNtlPetSkillContainer *pSkillContainer = pSobPet->GetSkillContainer();
		pSkillTblData = pSkillContainer->GetSkillFromTable(bySlotIdx);
	}
	else
	{
		CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlSkillContainer *pSkillContainer = pSobAvatar->GetSkillContainer();
		pSkillTblData = pSkillContainer->GetSkillFromTable(bySlotIdx);
	}

	if( !pSkillTblData )
	{
		DBO_FAIL("skill table pointer is null !!!");
		return;
	}

	Logic_GetAvatarSkillAcpplyAreaActor(hTargetSerial, pSkillTblData, listApplyActor);
}

void Logic_GetAvatarSkillApplyArea(SERIAL_HANDLE hTargetSerial, RwUInt32 uiSkillId, std::list<SERIAL_HANDLE>& listApplyActor)
{
	CNtlSobSkill* pSkill = (CNtlSobSkill*)GetNtlSLGlobal()->GetSobAvatar()->GetSkillContainer()->GetSkillFromTableId(uiSkillId);
	NTL_ASSERT(pSkill, __FUNCTION__ << "Skill is Not Skill Container");
    if(!pSkill)
        return;

	CNtlSobSkillAttr* pSobSkillAttr = (CNtlSobSkillAttr*)pSkill->GetSobAttr();

	Logic_GetAvatarSkillAcpplyAreaActor(hTargetSerial, pSobSkillAttr->GetSkillTbl(), listApplyActor);
}

RwBool Logic_IsEnemyTargetFromActor(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial)
{
	if( !pActor )
		return FALSE;

	RwUInt32 uiClassId = pActor->GetClassID();
	if(uiClassId == SLCLASS_PET)
		return Logic_IsEnemyTagetFromPetActor(pActor, hTargetSerial);
	else if(uiClassId == SLCLASS_AVATAR)
		return Logic_IsEnemyTargetFromAvatarActor(hTargetSerial);
	else 
		return TRUE;
}

RwBool Logic_IsEnemyTargetFromAvatarActor(SERIAL_HANDLE hTargetSerial)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar == NULL)
		return FALSE;

	return GetNtlWorldConcept()->IsEnemyTargetFromAvatarActor(pSobAvatar, hTargetSerial);
}

RwBool Logic_IsEnemyTagetFromPetActor(CNtlSobActor *pActor, SERIAL_HANDLE hTargetSerial)
{
	if(pActor->GetClassID() != SLCLASS_PET)
		return FALSE;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar == NULL)
		return FALSE;

	if(pSobAvatar->GetSerialID() != pActor->GetOwnerID())
		return FALSE;

	return GetNtlWorldConcept()->IsEnemyTargetFromAvatarActor(pSobAvatar, hTargetSerial);
}

CNtlSobActor* Logic_FindNearEnemyFromAvatarActor(void)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar == NULL)
		return NULL;

    // PVP, 랭크 배틀, 천하제일무도회등의 상황이 있기 때문에 WorldConcept에서 타겟을 검색해야 한다.
    return GetNtlWorldConcept()->FindNearEnemyFromAvatarActor(pSobAvatar);
}

// 자동 타겟팅의 타겟 정보 리스
struct STargetInfo
{
    CNtlSobActor*   pActor;
    RwReal          fDistance;
};

bool TargetInfoSorter(STargetInfo* elem1, STargetInfo* elem2)
{
    return elem1->fDistance < elem2->fDistance;
}

/// 일반적인 경우에서 찾는 자동 타겟
CNtlSobActor* Logic_GetAutoTarget(CNtlSobActor* pActor)
{
    NTL_ASSERT(pActor, "pActor is Null!");
    if(!pActor)
        return NULL;

    const RwReal  MAX_TARGET_LENGTH = 35.0f;
    static RwUInt32 nTargetIndex = 0;                // 순차적으로 타겟을 선택하기 위한 Static 변수
    static CNtlSobActor* pPrevTargetActor = NULL;    // 기존 타겟을 저장해둔다.
    static CNtlSobActor* pNearTargetActor = NULL;    // 가장 가까운 타겟을 저장해둔다.
    RwReal fLen = 0.0f;

    // 아바타의 눈위치에서 상대의 중간 위치까지의 라인으로 판단한다.
    RwV3d vStartPos = pActor->GetPosition();
    vStartPos.y += pActor->GetSobProxy()->GetPLEntityHeight() * 0.9f;
    
    std::vector<STargetInfo*> vecTargetList;
    vecTargetList.reserve(30);

    CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_MONSTER);
    if ( pSobGroup )
    {
        CNtlSobActor* pTarActor = NULL;
        RwSphere sSphere;
        RwV3d vTargetPos = ZeroAxis;

        CNtlSobGroup::MapObject& mapObject = pSobGroup->GetObjects();
        CNtlSobGroup::MapObject::iterator it = mapObject.begin();
        for ( ; it != mapObject.end(); ++it )
        {
            pTarActor = (CNtlSobActor*) (*it).second;            
            if (!pTarActor || pTarActor->IsDie() || !Logic_CanTargetted( pTarActor ) || Logic_IsCondition( pTarActor, CHARCOND_FLAG_HIDING_KI ) )
            {
                continue;
            }
            
            fLen = CNtlMath::GetLength(pActor->GetPosition(), pTarActor->GetPosition());            

            // 일정 거리 이상이면 타겟팅 하지 않는다
            if ( fLen > MAX_TARGET_LENGTH)
            {
                continue;
            }

            // 시야에 들어오지 않으면 타겟팅 하지 않는다.
            sSphere.center = pTarActor->GetPosition();
            sSphere.radius = pTarActor->GetSobProxy()->GetPLEntityHeight();
            if ( !RwCameraFrustumTestSphere( GetNtlGameCameraManager()->GetCamera()->GetCamera(), &sSphere ) )
            {
                continue;
            }

            // 오브젝트에 차폐되어 있으면 타겟팅 하지 않는다.
            vTargetPos = pTarActor->GetPosition();
            vTargetPos.y += pTarActor->GetSobProxy()->GetPLEntityHeight() * 0.5f;            
            if(Logic_IsCollisionLine(vStartPos, vTargetPos))
            {
                continue;                                        
            }

            // 리스트에 담는다.
            STargetInfo* pTargetInfo = NTL_NEW STargetInfo();
            pTargetInfo->fDistance = fLen;
            pTargetInfo->pActor = pTarActor;
            vecTargetList.push_back(pTargetInfo);
       }        
        
        if(!vecTargetList.empty())
        {
            // 리스트를 소팅한다.
            std::sort(vecTargetList.begin(), vecTargetList.end(), TargetInfoSorter);
            nTargetIndex = nTargetIndex >= vecTargetList.size() ? 0 : nTargetIndex;            

            if(pNearTargetActor != vecTargetList[0]->pActor)        // 가장 가까운 타겟이 변경되면 새로 타겟팅한다. (캐릭터 이동, 몹 배치 변경)
            {
                nTargetIndex = 0;                                
                pNearTargetActor = vecTargetList[0]->pActor;
            }

            CNtlSobActor* pTargetActor = vecTargetList[nTargetIndex++]->pActor;
            
            if(pTargetActor == pPrevTargetActor)    // 이전 타겟과 새로운 타겟이 같으면 다음 타겟을 선택한다.
            {
                nTargetIndex = nTargetIndex >= vecTargetList.size() ? 0 : nTargetIndex;
                pTarActor = vecTargetList[nTargetIndex++]->pActor;
            }

            pPrevTargetActor = pTargetActor;
            
            for each(STargetInfo* pTargetInfo in vecTargetList)
            {
                NTL_DELETE(pTargetInfo);
            }
            vecTargetList.clear();

            return pTargetActor;
        }        
    }

    nTargetIndex = 0;
    pPrevTargetActor = NULL;
    return NULL;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ground fly height

RwBool Logic_IsGroundFlyActor(CNtlSobActor *pActor)
{
	if( !pActor )
		return FALSE;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr *pSobMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>( pActor->GetSobAttr() );
		if(pSobMoveableAttr->GetFlyHeight() > 0.0f)
			return TRUE;
	}

	return FALSE;
}

RwReal Logic_GetGroundFlyHeight(CNtlSobActor *pActor)
{
	if( !pActor )
		return 0.0f;

	if(pActor->GetActorFlags() & SLFLAG_ACTOR_HAVE_MOVEABLE)
	{
		CNtlSobMoveableAttr *pSobMoveableAttr = reinterpret_cast<CNtlSobMoveableAttr*>( pActor->GetSobAttr() );
		return pSobMoveableAttr->GetFlyHeight();
	}

	return 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hit Mark

const RwChar* Logic_GetVisualHitMark(CNtlSobActor *pActor, EAttackType eAttackType, RwBool bCritical, RwBool bPowerEffect)
{
	if( !pActor )
		return NTL_VID_HIT_PLAYER_PHYSIC;

	if(pActor->GetClassID() == SLCLASS_MONSTER)
	{
		switch(eAttackType)
		{
		case ATTACK_TYPE_PHYSICAL:
			if(bCritical)           return NTL_VID_HIT_MOB_PHYSIC_CRITICAL;
			else if(bPowerEffect)   return NTL_VID_HIT_MOB_PHYSIC_POWER;                
			else                    return rand() % 2 ? NTL_VID_HIT_MOB_PHYSIC : NTL_VID_HIT_MOB_PHYSIC_02;            
			break;
		case ATTACK_TYPE_ENERGY:
			if(bCritical)           return NTL_VID_HIT_MOB_ENERGY_CRITICAL;
			else if(bPowerEffect)   return NTL_VID_HIT_MOB_ENERGY_POWER;                
			else                    return rand() % 2 ? NTL_VID_HIT_MOB_ENERGY : NTL_VID_HIT_MOB_ENERGY_02;            
			break;
		}
	}
	else 
	{
		switch(eAttackType)
		{
		case ATTACK_TYPE_PHYSICAL:
			if(bCritical)           return NTL_VID_HIT_PLAYER_PHYSIC_CRITICAL;
			else if(bPowerEffect)   return NTL_VID_HIT_PLAYER_PHYSIC_POWER;                
			else                    return rand() % 2 ? NTL_VID_HIT_PLAYER_PHYSIC : NTL_VID_HIT_PLAYER_PHYSIC_02;            
			break;
		case ATTACK_TYPE_ENERGY:
			if(bCritical)           return NTL_VID_HIT_PLAYER_ENERGY_CRITICAL;
			else if(bPowerEffect)   return NTL_VID_HIT_PLAYER_ENERGY_POWER;                
			else                    return rand() % 2 ? NTL_VID_HIT_PLAYER_ENERGY : NTL_VID_HIT_PLAYER_ENERGY_02;                        
			break;
		}
	}

	return NTL_VID_HIT_PLAYER_PHYSIC;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// item

void Logic_SetItemStack(CNtlSobItem* pSobItem, RwUInt8 ucStackNum)
{
	if( !pSobItem )
		return;

	if( ucStackNum == 0 )
	{
		NTL_ASSERT( pSobItem->GetParentItem(), "SLLogic_SetItemStack : StackItem must be present in Bag" );
		CNtlSLEventGenerator::SobItemDelete( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), pSobItem->GetSerialID(),
			(RwUInt8)(pSobItem->GetParentItemSlotIdx() + CONTAINER_TYPE_BAG1), (RwUInt8)pSobItem->GetItemSlotIdx() );
	}
	else
	{
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
		pItemAttr->SetStackNum( ucStackNum );
		CNtlSLEventGenerator::SobUpdate(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), EVENT_AIUT_ITEM, 0);
	}
}

sITEM_TBLDAT* Logic_GetEquipedWeaponItemTableData(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return NULL;

	if(Logic_IsTransformGreatNamek((CNtlSobActor*)pSobObj))
		return NULL;

	RwUInt32 uiClassId = pSobObj->GetClassID();

	if(uiClassId == SLCLASS_AVATAR)
	{
		CNtlInventory *pInventory = reinterpret_cast<CNtlSobAvatar*>( pSobObj )->GetInventory();
		SERIAL_HANDLE hItem = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_HAND);
		if(hItem == INVALID_SERIAL_ID)
			return NULL;

		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hItem) );
		if(!pSobItem)
			return NULL;

		CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
		return pSobItemAttr->GetItemTbl();
	}
	else if(uiClassId == SLCLASS_PLAYER)
	{
		CNtlInventoryBase *pInventory = reinterpret_cast<CNtlSobPlayer*>( pSobObj )->GetEquipInventory();
		RwUInt32 uiItemTblId = pInventory->GetEquipTableId(EQUIP_SLOT_TYPE_HAND);
		if(uiItemTblId == INVALID_TBLIDX)
			return NULL;

		CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
		return reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(uiItemTblId) );
	}

	return NULL;
}

SERIAL_HANDLE Logic_GetEquipedSubWeaponItemSerial(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return INVALID_SERIAL_ID;

	RwUInt32 uiClassId = pSobObj->GetClassID();

	if(uiClassId == SLCLASS_AVATAR)
	{
		CNtlInventory *pInventory = reinterpret_cast<CNtlSobAvatar*>( pSobObj )->GetInventory();
		SERIAL_HANDLE hItem = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SUB_WEAPON);
		return hItem;
	}

	return INVALID_SERIAL_ID;
}

SERIAL_HANDLE Logic_GetEquipedItemSerial(CNtlSob *pSobObj, RwUInt8 bySlotIdx)
{
	if( !pSobObj )
		return INVALID_SERIAL_ID;

	RwUInt32 uiClassId = pSobObj->GetClassID();

	if(uiClassId == SLCLASS_AVATAR)
	{
		CNtlInventory *pInventory = reinterpret_cast<CNtlSobAvatar*>( pSobObj )->GetInventory();
		SERIAL_HANDLE hItem = pInventory->GetEquipItem(bySlotIdx);
		return hItem;
	}

	return INVALID_SERIAL_ID;
}

RwBool Logic_IsEquipedStaffWeapon(sITEM_TBLDAT *pItemTblData)
{
	if( !pItemTblData )
		return FALSE;

	if(pItemTblData->byItem_Type == ITEM_TYPE_STAFF)
		return TRUE;

	return FALSE;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// skill
sSKILL_TBLDAT* Logic_GetSkillDataFromTable(TBLIDX index)
{
	sSKILL_TBLDAT* pSKILL_TBLDAT = (sSKILL_TBLDAT*)API_GetTableContainer()->GetSkillTable()->FindData(index);
	if(pSKILL_TBLDAT)
	{
		return pSKILL_TBLDAT;
	}

	return NULL;
}

void Logic_SetSkillExp(RwUInt32 uiSkillSlotIdx, RwUInt16 wExp)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pSobAvatar )
		return;

	CNtlSkillContainer *pSkillContainer = pSobAvatar->GetSkillContainer();
	CNtlSobSkill *pSobSkill = pSkillContainer->GetSkill(uiSkillSlotIdx);
	NTL_ASSERT(pSobSkill, "Logic_SetSkillExp => slot id (" << uiSkillSlotIdx << ") is invalid table vale ");
	CNtlSobSkillAttr *pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobSkill->GetSobAttr() );
	pSobSkillAttr->m_uiExp = wExp;

	//CNtlSLEventGenerator::SobUpdate(pSobAvatar->GetSerialID(), EVENT_AIUT_SKILL_EXP, uiSkillSlotIdx);
}

RwUInt8 g_byActiveRpSkillBonus = DBO_RP_BONUS_TYPE_INVALID;

void Logic_SetActiveRpSkillBonus(RwUInt8 byRpBonusType)
{
	g_byActiveRpSkillBonus = byRpBonusType;
}

RwUInt8 Logic_GetActiveRpSkillBonus(void)
{
	return g_byActiveRpSkillBonus;
}


RwBool Logic_IsSkillSystemEffectCode(TBLIDX index, RwUInt32 uiSystemEffectCode)
{
	sSKILL_TBLDAT* pSKILL_TBLDAT = (sSKILL_TBLDAT*)API_GetTableContainer()->GetSkillTable()->FindData(index);
	if(!pSKILL_TBLDAT)
		return FALSE;

	for(RwInt32 i = 0; i < NTL_MAX_EFFECT_IN_SKILL; ++i)
	{
		sSYSTEM_EFFECT_TBLDAT* pEffectData = (sSYSTEM_EFFECT_TBLDAT*)API_GetTableContainer()->GetSystemEffectTable()->FindData(pSKILL_TBLDAT->skill_Effect[i]);
		if(!pEffectData)
			continue;

		if(pEffectData->effectCode == (RwInt32)uiSystemEffectCode)
		{
			return TRUE;
		}
	}

	return FALSE;
}


RwReal Logic_GetSkillSystemEffectValue( TBLIDX index, RwUInt32 uiSystemEffect ) 
{
	sSKILL_TBLDAT* pSKILL_TBLDAT = (sSKILL_TBLDAT*)API_GetTableContainer()->GetSkillTable()->FindData(index);
	if(!pSKILL_TBLDAT)
		return 0.0f;

	for(RwUInt32 i = 0; i < NTL_MAX_EFFECT_IN_SKILL; ++i)
	{
		sSYSTEM_EFFECT_TBLDAT* pEffectData = (sSYSTEM_EFFECT_TBLDAT*)API_GetTableContainer()->GetSystemEffectTable()->FindData(pSKILL_TBLDAT->skill_Effect[i]);
		if(!pEffectData)
			continue;

		if(pEffectData->effectCode == (RwInt32)uiSystemEffect)
		{
			return pSKILL_TBLDAT->fSkill_Effect_Value[i];
		}        
	}

	return 0.0f;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// target

CNtlSobActor* Logic_GetAvatarActor(void)
{
	return GetNtlSLGlobal()->GetSobAvatar();
}

SERIAL_HANDLE Logic_GetAvatarHandle(void)
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pSobAvatar )
		return INVALID_SERIAL_ID;

	return pSobAvatar->GetSerialID();
}

const WCHAR* Logic_GetAvatarName(void)
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pSobAvatar )
		return NULL;

	CNtlSobAvatarAttr* pSobAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pSobAvatar->GetSobAttr());
	return pSobAvatarAttr->GetName();
}

void Logic_AvatarTargetStateChange(SERIAL_HANDLE uiTargetSeriaIId)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pSobAvatar )
		return;

	SERIAL_HANDLE hSerialId = pSobAvatar->GetTargetID();
	if(hSerialId == INVALID_SERIAL_ID)
		return;

	if(hSerialId != uiTargetSeriaIId)
		return;

	CNtlSLEventGenerator::AvatarTargetStateChange();
}

void Logic_AvatarStateChange(void)
{
	CNtlSLEventGenerator::AvatarTargetStateChange();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// table data 얻어오기.

void Logic_MakeMobBrief(sMOB_BRIEF *pBrief, TBLIDX tblidx)
{
	if( !pBrief )
		return;

	CMobTable *pMobTbl = API_GetTableContainer()->GetMobTable();
	sMOB_TBLDAT *pMobTblData = reinterpret_cast<sMOB_TBLDAT*>( pMobTbl->FindData(tblidx) );

	if( !pMobTblData )
	{
		DBO_FAIL("monster table error( " << tblidx << " ) ");
		return;
	}

	pBrief->tblidx				= tblidx;
	pBrief->fLastWalkingSpeed	= pMobTblData->fWalk_Speed;
	pBrief->fLastRunningSpeed	= pMobTblData->fRun_Speed;
	pBrief->wCurLP				= pMobTblData->wBasic_LP;
	pBrief->wMaxLP				= pMobTblData->wBasic_LP;
	pBrief->wAttackSpeedRate	= pMobTblData->wAttack_Speed_Rate;
}

void Logic_MakeNPCBrief(sNPC_BRIEF *pBrief, TBLIDX tblidx)
{
	if( !pBrief )
		return;

	CNPCTable *pNPCTbl = API_GetTableContainer()->GetNpcTable();
	sNPC_TBLDAT *pNPCTblData = reinterpret_cast<sNPC_TBLDAT*>( pNPCTbl->FindData(tblidx) );

	if( !pNPCTblData )
	{
		DBO_FAIL("npc table error( " << tblidx << " ) ");
		return;
	}

	pBrief->tblidx				= tblidx;
	pBrief->fLastWalkingSpeed	= pNPCTblData->fWalk_Speed;
	pBrief->fLastRunningSpeed	= pNPCTblData->fRun_Speed;
	pBrief->wCurLP				= pNPCTblData->wBasic_LP;
	pBrief->wMaxLP				= pNPCTblData->wBasic_LP;
	pBrief->wAttackSpeedRate	= pNPCTblData->wAttack_Speed_Rate;
	pBrief->nicknameTblidx		= 0xffffffff;
}

void Logic_MakeMobSpawnState(sCHARSTATE *pState, RwV3d vLoc, RwV3d vDir)
{
	if( !pState )
		return;

	pState->sCharStateBase.bFightMode		= FALSE;
	pState->sCharStateBase.byStateID		= CHARSTATE_SPAWNING;
	pState->sCharStateBase.dwConditionFlag	= 0;
	pState->sCharStateBase.aspectState.sAspectStateBase.byAspectStateId			= 0;
	pState->sCharStateBase.dwStateTime		= 0;
	pState->sCharStateBase.vCurLoc.x		= vLoc.x;
	pState->sCharStateBase.vCurLoc.y		= vLoc.y;
	pState->sCharStateBase.vCurLoc.z		= vLoc.z;
	pState->sCharStateBase.vCurDir.x		= vDir.x;
	pState->sCharStateBase.vCurDir.y		= vDir.y;
	pState->sCharStateBase.vCurDir.z		= vDir.z;
}

void Logic_MakeNPCSpawnState(sCHARSTATE *pState, RwV3d vLoc, RwV3d vDir)
{
	if( !pState )
		return;

	pState->sCharStateBase.bFightMode		= FALSE;
	pState->sCharStateBase.byStateID		= CHARSTATE_SPAWNING;
	pState->sCharStateBase.dwConditionFlag	= 0;
	pState->sCharStateBase.aspectState.sAspectStateBase.byAspectStateId			= 0;
	pState->sCharStateBase.dwStateTime		= 0;
	pState->sCharStateBase.vCurLoc.x		= vLoc.x;
	pState->sCharStateBase.vCurLoc.y		= vLoc.y;
	pState->sCharStateBase.vCurLoc.z		= vLoc.z;
	pState->sCharStateBase.vCurDir.x		= vDir.x;
	pState->sCharStateBase.vCurDir.y		= vDir.y;
	pState->sCharStateBase.vCurDir.z		= vDir.z;
}


sNPC_TBLDAT* Logic_GetNPCDataFromSob(SERIAL_HANDLE hSerial)
{

	CNtlSobNpc* pSobNPC =  reinterpret_cast<CNtlSobNpc*>(GetNtlSobManager()->GetSobObject(hSerial));
	if(pSobNPC)
	{
		CNtlSobNpcAttr* pSobNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>(pSobNPC->GetSobAttr());
		if(pSobNPCAttr)
		{
			sNPC_TBLDAT* pNPC_TBLDAT = pSobNPCAttr->GetNpcTbl();			
			return pNPC_TBLDAT;
		}
	}

	return NULL;
}

TBLIDX Logic_GetNPCTriggerId(CNtlSob *pSobObj)
{
	if(pSobObj)
	{
		CNtlSobNpcAttr* pSobNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>(pSobObj->GetSobAttr());
		if(pSobNPCAttr)
		{
			sNPC_TBLDAT* pNpcTblData = pSobNPCAttr->GetNpcTbl();			
			return pNpcTblData->tblidx;
		}
	}

	return INVALID_TBLIDX;
}

RwReal Logic_GetQuestNpcDistanceFromAvatar(CNtlSob *pSobNpc)
{
	if( !pSobNpc )
		return 0.f;

	RwV3d vAvatarPos	= GetNtlSLGlobal()->GetSobAvatar()->GetPosition();
	RwV3d vNpcPos		= pSobNpc->GetPosition();
	RwV3d vDist;
	RwV3dSub(&vDist, &vNpcPos, &vAvatarPos);
	return RwV3dLength(&vDist);
}

RwBool Logic_IsDragonBallNPC(TBLIDX tblidx)
{
	CDragonBallTable *pDBTable = API_GetTableContainer()->GetDragonBallTable();

	CTable::TABLEIT it;

	sDRAGONBALL_TBLDAT *pDBTblData;

	for(it = pDBTable->Begin(); it != pDBTable->End(); it++)
	{
		pDBTblData = reinterpret_cast<sDRAGONBALL_TBLDAT*>( (*it).second );
		if(pDBTblData->dragonNPCTblidx == tblidx)
			return TRUE;
	}

	return FALSE;
}

RwBool Logic_IsDragonBallNPCActor(SERIAL_HANDLE hSerialId)
{
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(hSerialId) );
	if(pActor == NULL)
		return FALSE;

	if(pActor->GetClassID() != SLCLASS_NPC)
		return FALSE;

	CNtlSobNpcAttr *pNpcAttr = reinterpret_cast<CNtlSobNpcAttr*>( pActor->GetSobAttr() );
	sNPC_TBLDAT *pNpcTblData = pNpcAttr->GetNpcTbl();

	CDragonBallTable *pDBTable = API_GetTableContainer()->GetDragonBallTable();

	CTable::TABLEIT it;

	sDRAGONBALL_TBLDAT *pDBTblData;

	for(it = pDBTable->Begin(); it != pDBTable->End(); it++)
	{
		pDBTblData = reinterpret_cast<sDRAGONBALL_TBLDAT*>( (*it).second );
		if(pDBTblData->dragonNPCTblidx == pNpcTblData->tblidx)
			return TRUE;
	}

	return FALSE;
}

sCHARM_TBLDAT* Logic_GetCharmDataFromTable(TBLIDX index)
{
	sCHARM_TBLDAT* pCHARM_TBLDAT = (sCHARM_TBLDAT*)API_GetTableContainer()->GetCharmTable()->FindData(index);
	if(pCHARM_TBLDAT)
	{
		return pCHARM_TBLDAT;
	}

	return NULL;
}

sMERCHANT_TBLDAT* Logic_GetMerchantDataFromTable(TBLIDX index)
{	
	sMERCHANT_TBLDAT* pMERCHANT_TBLDAT = (sMERCHANT_TBLDAT*)API_GetTableContainer()->GetMerchantTable()->FindData(index);
	if(pMERCHANT_TBLDAT)
	{
		return pMERCHANT_TBLDAT;
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Trigger object
RwBool Logic_IsTriggerActive(CNtlSob *pSobObj)
{
	if( !pSobObj )
		return FALSE;

	if(pSobObj->GetClassID() == SLCLASS_NPC || pSobObj->GetClassID() == SLCLASS_TRIGGER_OBJECT)
		return TRUE;

	return FALSE;
}

TBLIDX Logic_GetTriggerObjectId(CNtlSob *pSobObj)
{
	if(pSobObj)
	{
		CNtlSobTriggerObjectAttr* pSobTriggerObjectAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>(pSobObj->GetSobAttr());
		if(pSobTriggerObjectAttr)
		{
			sOBJECT_TBLDAT* pTriggerObjectTblData = pSobTriggerObjectAttr->GetTriggerObjectTbl();			
			return pTriggerObjectTblData->tblidx;
		}
	}

	return INVALID_TBLIDX;
}

RwBool Logic_IsNormalQuest( RwUInt16 tID )
{
	if( tID >= eQUEST_ID_RANGE_GENERAL_QUEST_MIN && tID < eQUEST_ID_RANGE_GENERAL_QUEST_MAX )
		return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic Object

TBLIDX Logic_GetDynamicObjTriggerId( CNtlSob* pSobObj )
{
	/*if(pSobObj)
	{
	CNtlSobDynamicObjectAttr* pSobDynamicObjectAttr = reinterpret_cast<CNtlSobDynamicObjectAttr*>(pSobObj->GetSobAttr());
	if(pSobDynamicObjectAttr)
	{
	sDYNAMIC_OBJECT_TBLDAT* pDynamicObjectTblData = pSobDynamicObjectAttr->GetDynamicObjectTbl();			
	return pDynamicObjectTblData->tblidx;
	}
	}*/

	return INVALID_TBLIDX;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Item

sITEM_TBLDAT* Logic_GetItemDataFromTable(TBLIDX index)
{		
	sITEM_TBLDAT* pITEM_TBLDAT = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(index);
	if(pITEM_TBLDAT)
	{
		return pITEM_TBLDAT;
	}	

	return NULL;
}

sITEM_TBLDAT* Logic_GetItemDataFromSob(SERIAL_HANDLE hSerial)
{
	CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hSerial));
	if(pSobItem)
	{
		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
		NTL_ASSERT(pSobItemAttr, "Logic_GetItemDataFromSob, Not exist sob item attiribure of handle : " << hSerial);

		// 미확인 아이템
		if( pSobItemAttr->IsNeedToIdentify() )
			return NULL;

		sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();	
		NTL_ASSERT(pITEM_TBLDAT, "Logic_GetItemDataFromSob, Not exist item table of sob item handle  : " << hSerial);

		return pITEM_TBLDAT;
	}

	return NULL;	
}

CNtlSobItemAttr* Logic_GetItemSobAttrFromSobSerial( SERIAL_HANDLE hSerial )
{
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerial ) );
	if( pSobItem )
	{
		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
		return pSobItemAttr;
	}
	
	return NULL;
}

RwBool Logic_IsMoveableSubWeapon(sITEM_TBLDAT *pItemTblData)
{
	if( !pItemTblData )
		return FALSE;

	BYTE byItemType = pItemTblData->byItem_Type;
	if(byItemType == ITEM_TYPE_GEM || byItemType == ITEM_TYPE_DRUM)
		return TRUE;

	return FALSE;
}

RwBool Logic_IsActiveNotAttachSubWeapon(TBLIDX tblidx)
{
	CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(tblidx) );
	if(pItemTblData == NULL)
		return FALSE;

	//	if(pItemTblData->byItem_Type == ITEM_TYPE_SCROLL)
	//		return TRUE;

	return FALSE;
}


RwBool Logic_IsMoveableSubWeapon(TBLIDX tblidx)
{
	CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(tblidx) );
	if(pItemTblData == NULL)
		return FALSE;

	BYTE byItemType = pItemTblData->byItem_Type;

	if(byItemType == ITEM_TYPE_GEM || byItemType == ITEM_TYPE_DRUM)
		return TRUE;

	return FALSE;
}

RwBool Logic_IsWeaponChangeSubWeapon(TBLIDX tblidx)
{
	CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
	sITEM_TBLDAT *pItemTblData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(tblidx) );
	if(pItemTblData == NULL)
		return FALSE;

	if(pItemTblData->byItem_Type == ITEM_TYPE_CLAW)
		return TRUE;

	if(pItemTblData->wFunction_Bit_Flag & ITEM_FUNC_BIT_FLAG_MAIN_WEAPON_OFF)
		return TRUE;

	return FALSE;
}

RwBool Logic_IsCostumeItem(sITEM_TBLDAT *pItemTblData)
{
	if( !pItemTblData )
		return FALSE;

	if(pItemTblData->byItem_Type == ITEM_TYPE_COSTUME ||
       pItemTblData->byItem_Type == ITEM_TYPE_DOGI)
		return TRUE;

	return FALSE;
}

RwBool Logic_IsHideCostumeItem(RwUInt16 wHideBigFlags, void *pData)
{
	if( !pData )
		return FALSE;

	CNtlPLItem *pPLItem = (CNtlPLItem*)pData;

	if(pPLItem == NULL)
		return FALSE;

	EItemEquipSlotType eSlotType = pPLItem->GetEquipSlotType();

	if(eSlotType == ITEM_EQUIP_HEAD)
	{
		if(wHideBigFlags & eCBSTS_HEAD)
			return TRUE;

		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_FACE)
	{
		if(wHideBigFlags & eCBSTS_FACE)
			return TRUE;

		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_JACKET)
	{
		if(wHideBigFlags & eCBSTS_JACKET)
			return TRUE;

		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_PANTS)
	{
		if(wHideBigFlags & eCBSTS_PANTS)
			return TRUE;

		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_BOOTS)
	{
		if(wHideBigFlags & eCBSTS_BOOTS)
			return TRUE;

		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_TWO_HAND)
	{
		if(wHideBigFlags & eCBSTS_TWO_HAND)
			return TRUE;

		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_R_EYE)
	{
		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_R_HAND)
	{
		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_L_HAND)
	{
		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_BACK)
	{
		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_USER_MOVE1)
	{
		return FALSE;
	}
	else if(eSlotType == ITEM_EQUIP_SCOUTER)
	{
		if(wHideBigFlags & eCBSTS_SCOUTER_AT_EYE)
			return TRUE;

		return FALSE;
	}

	return FALSE;
}

RwBool Logic_IsCanSellItem(sITEM_TBLDAT* pITEM_TBLDAT)
{
	NTL_ASSERT( pITEM_TBLDAT, "Logic_IsCanSellItem, Invalid pointer sITEM_TBLDAT" );

	if( pITEM_TBLDAT )
	{
	if( pITEM_TBLDAT->wFunction_Bit_Flag & ITEM_FUNC_BIT_FLAG_SELL_FORBIDDEN )
			return FALSE;

	}

	return TRUE;
}

RwBool Logic_IsCanSellItem( CNtlSobItemAttr* pSobItemAttr )
{
	NTL_ASSERT( pSobItemAttr, "Logic_IsCanSellItem, Invalid pointer CNtlSobItemAttr" );

	if( pSobItemAttr )
	{
		if( !pSobItemAttr->IsNeedToIdentify() )
		{
			if( !Logic_IsCanSellItem( pSobItemAttr->GetItemTbl() ) )
				return FALSE;

			if( pSobItemAttr->GetDurationType() != eDURATIONTYPE_NORMAL )
				return FALSE;
		}
	}

	return TRUE;
}

RwBool Logic_IsCanSellItem( sITEM_PROFILE* pItemProfile )
{
	NTL_ASSERT( pItemProfile, "Logic_IsCanSellItem, Invalid pointer sITEM_PROFILE" );

	if( pItemProfile )
	{
		if( !pItemProfile->bNeedToIdentify )
		{
			CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
			sITEM_TBLDAT *pItemData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData( pItemProfile->tblidx ) );

			if( !Logic_IsCanSellItem( pItemData ) )
				return FALSE;

			if( pItemProfile->byDurationType != eDURATIONTYPE_NORMAL )
				return FALSE;
		}
	}	

	return TRUE;
}

RwBool Logic_IsCanUserTradeItem(sITEM_TBLDAT* pITEM_TBLDAT)
{
	NTL_ASSERT(pITEM_TBLDAT, "Logic_IsCanUserTradeItem, Invalid pointer");

	if( pITEM_TBLDAT )
	{
	if( pITEM_TBLDAT->wFunction_Bit_Flag & ITEM_FUNC_BIT_FLAG_TRADE_FORBIDDEN )
			return FALSE;
	}	

	return TRUE;
}

RwBool Logic_IsCanUserTradeItem( CNtlSobItemAttr* pSobItemAttr )
{
	NTL_ASSERT( pSobItemAttr, "Logic_IsCanUserTradeItem, Invalid pointer CNtlSobItemAttr" );

	if( pSobItemAttr )
	{
		if( !pSobItemAttr->IsNeedToIdentify() )
		{
			if( !Logic_IsCanUserTradeItem( pSobItemAttr->GetItemTbl() ) )
				return FALSE;

			if( pSobItemAttr->GetDurationType() != eDURATIONTYPE_NORMAL )
				return FALSE;
		}

	}
	
	return TRUE;
}

RwBool Logic_IsCanUserTradeItem( sITEM_PROFILE* pItemProfile )
{
	NTL_ASSERT( pItemProfile, "Logic_IsCanUserTradeItem, Invalid pointer sITEM_PROFILE" );

	if( pItemProfile )
	{
		if( !pItemProfile->bNeedToIdentify )
		{
			CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
			sITEM_TBLDAT *pItemData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData( pItemProfile->tblidx) );

			if( !Logic_IsCanUserTradeItem( pItemData ) )
				return FALSE;

			if( pItemProfile->byDurationType != eDURATIONTYPE_NORMAL )
				return FALSE;
		}
	}	

	return TRUE;
}

RwBool Logic_IsCanSaveWarehouse(sITEM_TBLDAT* pITEM_TBLDAT)
{
	NTL_ASSERT( pITEM_TBLDAT, "Logic_IsCanSaveWarehouse, Invalid pointer sITEM_TBLDAT" );

	if( pITEM_TBLDAT )
	{
	if( pITEM_TBLDAT->wFunction_Bit_Flag & ITEM_FUNC_BIT_FLAG_STORE_FORBIDDEN )
			return FALSE;
	}	

	return TRUE;
}

RwBool Logic_IsCanSaveWarehouse( CNtlSobItemAttr* pSobItemAttr )
{
	NTL_ASSERT( pSobItemAttr, "Logic_IsCanSaveWarehouse, Invalid pointer CNtlSobItemAttr" );

	if( pSobItemAttr )
	{
		if( !pSobItemAttr->IsNeedToIdentify() )
		{
			if( !Logic_IsCanSaveWarehouse( pSobItemAttr->GetItemTbl() ) )
				return FALSE;

			if( pSobItemAttr->GetDurationType() != eDURATIONTYPE_NORMAL )
				return FALSE;
		}
	}	

	return TRUE;
}

RwBool Logic_IsCanSaveWarehouse( sITEM_PROFILE* pItemProfile )
{
	NTL_ASSERT( pItemProfile, "Logic_IsCanSaveWarehouse, Invalid pointer sITEM_PROFILE" );

	if( pItemProfile )
	{
		if( !pItemProfile->bNeedToIdentify )
		{
			CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
			sITEM_TBLDAT *pItemData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData( pItemProfile->tblidx ) );

			if( !Logic_IsCanSaveWarehouse( pItemData ) )
				return FALSE;

			if( pItemProfile->byDurationType != eDURATIONTYPE_NORMAL )
				return FALSE;
		}
	}	

	return TRUE;
}

RwBool Logic_IsCanSaveCommonWarehouse(sITEM_TBLDAT* pITEM_TBLDAT)
{
	NTL_ASSERT( pITEM_TBLDAT, "Logic_IsCanSaveCommonWarehouse, Invalid pointer sITEM_TBLDAT" );

	if( pITEM_TBLDAT )
	{
		if( !Logic_IsCanSaveWarehouse( pITEM_TBLDAT ) )
			return FALSE;

	if( pITEM_TBLDAT->byItem_Type == ITEM_TYPE_DRAGONBALL )
			return FALSE;

	}
	
	return TRUE;
}

RwBool Logic_IsCanSaveCommonWarehouse( CNtlSobItemAttr* pSobItemAttr )
{
	NTL_ASSERT( pSobItemAttr, "Logic_IsCanSaveCommonWarehouse, Invalid pointer CNtlSobItemAttr" );

	if( pSobItemAttr )
	{
		if( !pSobItemAttr->IsNeedToIdentify() )
		{
			if( !Logic_IsCanSaveCommonWarehouse( pSobItemAttr->GetItemTbl() ) )
				return FALSE;

			if( pSobItemAttr->GetDurationType() != eDURATIONTYPE_NORMAL )
				return FALSE;
		}
	}	

	return TRUE;
}

RwBool Logic_IsCanSaveCommonWarehouse( sITEM_PROFILE* pItemProfile )
{
	NTL_ASSERT( pItemProfile, "Logic_IsCanSaveCommonWarehouse, Invalid pointer sITEM_PROFILE" );

	if( pItemProfile )
	{
		if( !pItemProfile->bNeedToIdentify )
		{
			CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();
			sITEM_TBLDAT *pItemData = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData( pItemProfile->tblidx ) );

			if( !Logic_IsCanSaveCommonWarehouse( pItemData ) )
				return FALSE;

			if( pItemProfile->byDurationType != eDURATIONTYPE_NORMAL )
				return FALSE;
		}
	}	

	return TRUE;
}

RwBool Logic_IsTimeLimitItem( CNtlSobItemAttr* pItemAttr )
{
	if( pItemAttr )
	{
		if( pItemAttr->GetDurationType() )
			return TRUE;
	}

	return FALSE;
}

RwBool Logic_IsTimeLimitItem( sITEM_PROFILE* pItemProfile )
{
	if( pItemProfile )
	{
		if( pItemProfile->byDurationType )
			return TRUE;
	}

	return FALSE;
}

RwBool Logic_IsTimeLimitItem( sITEM_TBLDAT* pITEM_TBLDAT )
{
	if( pITEM_TBLDAT )
	{
		if( pITEM_TBLDAT->byDurationType )
			return TRUE;
	}

	return FALSE;
}

RwInt32 Logic_GetCountEmptySlotofBag()
{
	RwInt32 iEmptySlot = 0;
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	// peessitodo : 6번째 NETSTORE인 경우 NetMarbleMember인지 체크할 것.
	for( RwUInt8 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		SERIAL_HANDLE hHandle = pInventory->GetBagItem(i);

		if( hHandle == INVALID_SERIAL_ID )
			continue;		

		if( i == BAGSLOT_POSITION_BAGSLOT_POSITION_NETPYSTORE &&
			!GetNtlSLGlobal()->IsEnableUseNetpyStore() )
			continue;

		CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hHandle));
		if( !pSobItem )
		{
			DBO_FAIL("Not exist sob item of handle : " << hHandle);
			continue;
		}

		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
		sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();

		for( RwUInt32 k = 0 ; k < pITEM_TBLDAT->byBag_Size ; ++k )
		{
			CNtlSobItem* pChildItem = pSobItem->GetChildItem(k);

			if(!pChildItem)
				++iEmptySlot;
		}
	}

	return iEmptySlot;
}

void Logic_GetFirstEmptySlot_of_Bag(RwUInt32& uiBagIndex, SERIAL_HANDLE& hBagHandle, RwUInt32& uiSlot_of_bag)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlInventory* pInventory = pAvatar->GetInventory();

	for( RwUInt8 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		SERIAL_HANDLE hHandle = pInventory->GetBagItem(i);

		if( hHandle == INVALID_SERIAL_ID )
			continue;

		if( i == BAGSLOT_POSITION_BAGSLOT_POSITION_NETPYSTORE &&
			!GetNtlSLGlobal()->IsEnableUseNetpyStore() )
			continue;

		CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hHandle));
		if( !pSobItem )
		{
			DBO_FAIL("Not exist sob item of handle : " << hHandle);
			continue;
		}

		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
		sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();

		for( RwUInt32 k = 0 ; k < pITEM_TBLDAT->byBag_Size ; ++k )
		{
			CNtlSobItem* pChildItem = pSobItem->GetChildItem(k);

			if(!pChildItem)
			{
				uiBagIndex = i;
				hBagHandle = hHandle;
				uiSlot_of_bag = k;

				return;
			}
		}		
	}

	uiBagIndex = INVALID_INDEX;
	hBagHandle = INVALID_SERIAL_ID;
	uiSlot_of_bag = INVALID_INDEX;
}

void Logic_GetFirstEmptySlot_of_Warehouse(RwUInt8& byWarehouseIndex, SERIAL_HANDLE& hWarehouseHandle, RwUInt8& bySlot_of_warehouse)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlWarehouse* pWarehouse = pAvatar->GetWarehouse();

	for( RwUInt8 i = 0 ; i < NTL_MAX_BANKSLOT_COUNT ; ++i )
	{
		SERIAL_HANDLE hHandle = pWarehouse->GetSlotSerailID(i);

		if( hHandle == INVALID_SERIAL_ID )
			continue;


		CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hHandle));
		if( !pSobItem )
		{
			DBO_FAIL("Not exist sob item of handle : " << hHandle);
			continue;
		}

		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
		sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();

		for( RwUInt8 k = 0 ; k < pITEM_TBLDAT->byBag_Size ; ++k )
		{
			CNtlSobItem* pChildItem = pSobItem->GetChildItem(k);

			if(!pChildItem)
			{
				byWarehouseIndex	= i;
				hWarehouseHandle	= hHandle;
				bySlot_of_warehouse	= k;

				return;
			}
		}		
	}

	byWarehouseIndex	= INVALID_BYTE;
	hWarehouseHandle	= INVALID_SERIAL_ID;
	bySlot_of_warehouse = INVALID_BYTE;
}

RwBool Logic_FindWarehouseItem(SERIAL_HANDLE hItem, RwUInt8& byWarehouseIndex, RwUInt8& bySlot_of_warehouse)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return FALSE;

	CNtlWarehouse* pWarehouse = pAvatar->GetWarehouse();

	for( RwUInt8 i = 0 ; i < NTL_MAX_BANKSLOT_COUNT ; ++i )
	{
		SERIAL_HANDLE hHandle = pWarehouse->GetSlotSerailID(i);

		if( hHandle == INVALID_SERIAL_ID )
			continue;


		CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hHandle));
		if( !pSobItem )
		{
			DBO_FAIL("Not exist sob item of handle : " << hHandle);
			return FALSE;
		}

		CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
		sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();

		for( RwUInt8 k = 0 ; k < pITEM_TBLDAT->byBag_Size ; ++k )
		{
			CNtlSobItem* pChildItem = pSobItem->GetChildItem(k);

			if( !pChildItem )
				continue;

			if( pChildItem->GetSerialID() == hItem )
			{
				byWarehouseIndex		= i;
				bySlot_of_warehouse		= k;
				return TRUE;
			}
		}		
	}

	return FALSE;
}

void Logic_GetFirstEmptySlot_of_GuildWarehouse(RwUInt8& byGuildWarehouseIndex, RwUInt8& bySlot_of_GuildWarehouse)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return;

	CNtlGuildWarehouse* pGuildWarehouse = pAvatar->GetGuildWarehouse();

	for( RwUInt8 i = 0 ; i < NTL_MAX_GUILD_BANK_COUNT ; ++i )
	{
		for( RwUInt8 k = 0 ; k < NTL_MAX_GUILD_ITEM_SLOT ; ++k )
		{
			sGuildWarehouseSlot* pGuildWarehouseSlot = pGuildWarehouse->GetItem(i, k);

			if( !pGuildWarehouseSlot )
				continue;

			if( pGuildWarehouseSlot->hHandle != INVALID_SERIAL_ID )
				continue;

			byGuildWarehouseIndex		= i;
			bySlot_of_GuildWarehouse	= k;
			return;
		}
	}

	byGuildWarehouseIndex		= INVALID_BYTE;
	bySlot_of_GuildWarehouse	= INVALID_BYTE;
}

RwUInt8 Logic_GetFirstEmptyScouterSlot()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return INVALID_BYTE;

	CNtlInventory* pInventory = pAvatar->GetInventory();
	SERIAL_HANDLE hHandle = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);

	if( hHandle == INVALID_SERIAL_ID )
		return INVALID_BYTE;

	CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hHandle));
	if( !pSobItem )
	{
		DBO_FAIL("Not exist sob item of handle : " << hHandle);
		return INVALID_BYTE;
	}

	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
	sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();

	CNtlSobItem* pChildItem;
	for( RwUInt8 i = 0 ; i < pITEM_TBLDAT->byBag_Size ; ++i )
	{
		pChildItem = pSobItem->GetChildItem(i);

		if(!pChildItem)
			return i;
	}

	return INVALID_BYTE;
}

SERIAL_HANDLE Logic_FirstItem_in_Bag(TBLIDX index)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return INVALID_SERIAL_ID;

	CNtlInventory* pInventory = pAvatar->GetInventory();

	for( RwUInt8 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		SERIAL_HANDLE hBagHandle = pInventory->GetBagItem(i);
		if( hBagHandle == INVALID_SERIAL_ID )
			continue;

		CNtlSobItem* pSobBagItem			= reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hBagHandle ) );
		if( !pSobBagItem )
		{
			DBO_FAIL("Not exist sob item of handle : " << hBagHandle);
			return INVALID_SERIAL_ID;
		}

		CNtlSobItemAttr* pSobBagItemAttr	= reinterpret_cast<CNtlSobItemAttr*>( pSobBagItem->GetSobAttr() );
		sITEM_TBLDAT* pBagITEM_TBLDAT		= pSobBagItemAttr->GetItemTbl();


		// 가방 안에 현재 등록된 연료와 같은 연료의 갯수를 세어본다.
		for( RwUInt8 k = 0 ; k < pBagITEM_TBLDAT->byBag_Size ; ++k )
		{
			CNtlSobItem* pSobItem			= pSobBagItem->GetChildItem(k);
			if( !pSobItem )
				continue;

			CNtlSobItemAttr* pSobItemAttr	= reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );

			if( pSobItemAttr->IsNeedToIdentify() )
				continue;

			sITEM_TBLDAT* pITEM_TBLDAT		= pSobItemAttr->GetItemTbl();

			if( pITEM_TBLDAT->tblidx == index )
				return pSobItem->GetSerialID();
		}
	}

	return INVALID_SERIAL_ID;
}

RwBool Logic_ExistScouterPartsType(RwUInt32 iType)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(!pAvatar)
		return FALSE;

	CNtlInventory* pInventory = pAvatar->GetInventory();
	SERIAL_HANDLE hScouterSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);
	if( hScouterSerial == INVALID_SERIAL_ID )
		return FALSE;

	CNtlSobItem* pScouterItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hScouterSerial));
	if(!pScouterItem)
		return FALSE;

	CNtlSobItemAttr* pScouterItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pScouterItem->GetSobAttr());
	sITEM_TBLDAT* pITEM_TBLDAT = pScouterItemAttr->GetItemTbl();			
	if( !pITEM_TBLDAT )
	{
		DBO_FAIL("pITEM_TBLDAT is Must be Present" );
		return FALSE;
	}

	CNtlSobItem* pPartsItem;
	CNtlSobItemAttr* pPartsItemAttr;
	sITEM_TBLDAT* pPartsTBLDAT;

	for(RwInt32 i = 0 ; i < pITEM_TBLDAT->byBag_Size ; ++i )
	{
		pPartsItem = pScouterItem->GetChildItem(i);
		if(!pPartsItem)
			continue;

		pPartsItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pPartsItem->GetSobAttr());
		pPartsTBLDAT = pPartsItemAttr->GetItemTbl();
		if( !pPartsTBLDAT )
		{
			DBO_FAIL("pPartsTBLDAT is Must be Present" );
			continue;
		}

		if(pPartsTBLDAT->byScouter_Parts_Type == iType)
			return TRUE;
	}

	return FALSE;
}

RwReal Logic_GetItemSystemEffectValue(TBLIDX idxItem, RwUInt32 uiSystemEffect)
{
	sITEM_TBLDAT* pITEM_TBLDAT = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(idxItem);
	if( !pITEM_TBLDAT )
		return 0.f;

	return Logic_GetItemSystemEffectValue(pITEM_TBLDAT, uiSystemEffect);
}

RwReal Logic_GetItemSystemEffectValue(sITEM_TBLDAT* pITEM_TBLDAT, RwUInt32 uiSystemEffect)
{
	if( !pITEM_TBLDAT )
	{
		NTL_ASSERTFAIL("Logic_GetItemSystemEffectValue, Invalid item table");
		return 0.f;
	}

	sUSE_ITEM_TBLDAT* pUSE_ITEM_TBLDAT = (sUSE_ITEM_TBLDAT*)API_GetTableContainer()->GetUseItemTable()->FindData(pITEM_TBLDAT->Use_Item_Tblidx);
	if( !pUSE_ITEM_TBLDAT )
	{
		NTL_ASSERTFAIL("Logic_GetItemSystemEffectValue, Not exist USE_ITEM_TABLE of index : " << pITEM_TBLDAT->Use_Item_Tblidx);
		return 0.f;
	}

	for( RwUInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_ITEM ; ++i )
	{
		sSYSTEM_EFFECT_TBLDAT* pSYSTEM_EFFECT_TBLDAT = (sSYSTEM_EFFECT_TBLDAT*)API_GetTableContainer()->GetSystemEffectTable()->FindData(pUSE_ITEM_TBLDAT->aSystem_Effect[i]);
		if( !pSYSTEM_EFFECT_TBLDAT )
			continue;

		// pSYSTEM_EFFECT_TBLDAT->effectCode의 값은 NtlSystemEffect.h에 정의되어 있다
		if( (RwUInt32)pSYSTEM_EFFECT_TBLDAT->effectCode == uiSystemEffect )
		{
			return pUSE_ITEM_TBLDAT->afSystem_Effect_Value[0];
		}        
	}

	return 0.f;
}

RwBool Logic_IsNetStore( RwUInt32 uiBagIndex )
{
	if( uiBagIndex == BAGSLOT_POSITION_BAGSLOT_POSITION_NETPYSTORE )
		return TRUE;

	return FALSE;
}

RwBool Logic_IsEnableItemMoveToNetStore(VOID)
{
	if( GetNtlSLGlobal()->IsEnableUseNetpyStore() )
		return TRUE;

	return FALSE;
}

RwBool Logic_ItemMoveToNetStoreCheck( RwUInt32 uiBagIndex )
{
	if( Logic_IsNetStore( uiBagIndex ) )
		return Logic_IsEnableItemMoveToNetStore();				

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor math logic
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool Logic_InFollowRange(CNtlSobActor *pActor, CNtlSobActor *pTarActor, RwReal fFollowRange)
{
	if(pActor == NULL)
		return FALSE;

	if(pTarActor == NULL)
		return FALSE;

	RwV3d vPos = pActor->GetPosition();
	RwV3d vDestPos = pTarActor->GetPosition(); 
	
		RwV3d vDir;
	RwV3dSub(&vDir, &vDestPos, &vPos); 
	RwReal fCurrLen = RwV3dLength(&vDir);

	if(fCurrLen <= fFollowRange)
		return TRUE;

	return FALSE;
}

RwBool Logic_InFollowRange(CNtlSobActor *pActor, const RwV3d& v3TargetPos, RwReal fFollowRange)
{
	if(pActor == NULL)
		return FALSE;

	RwV3d vPos = pActor->GetPosition();

	RwV3d vDir;
	RwV3dSub(&vDir, &v3TargetPos, &vPos);
	RwReal fCurrLen = RwV3dLength(&vDir);

	if(fCurrLen <= fFollowRange)
		return TRUE;

	return FALSE;
}

RwBool Logic_CanActorCommunity( CNtlSobActor* pActor, CNtlSobActor* pTarActor, RwReal fFollowRange )
{
	if(pActor == NULL)
		return FALSE;

	if(pTarActor == NULL)
		return FALSE;

	if(Logic_IsVehicleDriver(pActor))
		return FALSE;

	if( SLCLASS_VEHICLE == pActor->GetClassID() )
		return FALSE;

	RwV3d vPos = pActor->GetPosition();
	RwV3d vDestPos = pTarActor->GetPosition();

	RwV3d vDir;
	RwV3dSub(&vDir, &vDestPos, &vPos); 
	RwReal fCurrLen = RwV3dLength(&vDir);

	if( fCurrLen <= fFollowRange )
		return TRUE;

	if( fabs( fCurrLen - fFollowRange) < NTL_MAX_MERCHANT_DISTANCE_EXTRA )
		return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor world presenation coordinate
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CHAR_COLLISION_LINE_FRONT_OFFSET		0.5f
#define CHAR_COLLISION_LINE_HEIGHT_OFFSET		0.3f
#define CHAR_COLLISION_LINE_LIMIT_HEIGHT		0.9f
#define CHAR_COLLISION_LINE_COUNT               4
#define CHAR_COLLISION_LINE_JUMP_INDEX          3

#define VEHICLE_COLLISION_LINE_FRONT_OFFSET		0.5f
#define VEHICLE_COLLISION_LINE_HEIGHT_OFFSET	0.3f
#define VEHICLE_COLLISION_LINE_LIMIT_HEIGHT		0.9f
#define VEHICLE_COLLISION_LINE_COUNT			4
#define VEHICLE_COLLISION_LINE_JUMP_INDEX		3

typedef struct _SCharCollisionBrief
{
	RwReal fCharWidth;
	RwReal fCharHeight;
	RwReal fCharDepth;
	RwReal fCharHalfWidth;
	RwReal fCharHalfHeight;
	RwReal fCharHalfDepth;
	RwReal fInterval;
	RwV3d vCharCurrPos;
	RwV3d vCharDestPos;
	RwV3d vCharFront;
	RwV3d vCharRight;
	RwReal fCollDist;
	RwV3d vCollPos;
	RwV3d vCollNormal;
	RwV3d vAdjustCollPos;	// char 이동 방향에 따른 collisin position을 보정한다.
	RwBool bCollRecursive;
	RwV3d vNewPos;
	RwBool bCollMoveImPossible;	// 충돌은 했는데, 어떤쪽으로 이동할 수 없는 경우.
	RwBool bCollJump;           // 점프를 해서 위로 충돌했을 경우
	CNtlPLEntity *pCollPLEntity;
}SCharCollisionBrief;

RwBool				g_bColliSlantCheck  = TRUE;
RwBool				g_SlippingCheck		= TRUE;

SCharCollisionBrief g_sColliBrief;

RwBool Logic_GetWorldHeight(const RwV3d *pPos, SWorldHeightStuff& sHStuff, RwV3d *pNormal /*= NULL*/)
{
	sHStuff.bWaterAttr = FALSE;

	RwBool bReady;
	bReady = GetSceneManager()->GetWorldHeight(pPos, sHStuff.fWorldHeight, pNormal);
	sHStuff.fFinialHeight = sHStuff.fWorldHeight;

	if(!bReady)
		return FALSE;

	sHStuff.fWaterHeight = GetSceneManager()->GetWorldWaterHeight(*pPos);
	if(sHStuff.fWaterHeight > sHStuff.fWorldHeight)
	{
		sHStuff.bWaterAttr = TRUE;
		sHStuff.fFinialHeight = sHStuff.fWaterHeight;
	}

	/*
	DWORD dwAttr = GetSceneManager()->GetWorldNormalAttribute(*pPos);
	if(dwAttr & DBO_WORLD_ATTR_GROUP_FLAG_SWIM)
	{
	sHStuff.bWaterAttr = TRUE;
	sHStuff.fWaterHeight = GetSceneManager()->GetWorldWaterHeight(*pPos);
	if(sHStuff.fWaterHeight > sHStuff.fWorldHeight)
	sHStuff.fFinialHeight = sHStuff.fWaterHeight;
	}
	*/

	return TRUE;
}

void Logic_ProcWorldLight(CNtlSobActor *pActor, RwV3d vPos)
{
	if( !pActor )
		return;

	CNtlSobProxy*	pSobProxy	= pActor->GetSobProxy();

	// vPos의 ShadowProperty정보를 가져온다.
	RwBool			bIsInShadow	= GetNtlPLOptionManager()->GetTerrainShadow() && (GetSceneManager()->GetWorldNormalAttribute(vPos) & DBO_WORLD_ATTR_GROUP_FLAG_WORLD_SHADOW);

	if (bIsInShadow && CNtlPLGlobal::m_pWHEntity && CNtlPLGlobal::m_pWHEntity->GetClassType() == PLENTITY_OBJECT)
	{
		// pActor가 Object 위에 있고 SelfShadow(FALSE)라면 Shadow를 적용하지 않는다.
		CNtlPLObject* pPLObject = reinterpret_cast<CNtlPLObject*>(CNtlPLGlobal::m_pWHEntity);
		if (!(pPLObject->GetFlags() & NTL_PLEFLAG_SHADOW_PROP))
		{
			if (!pPLObject->GetEnableGenShadowMap()) 
			{
				bIsInShadow = FALSE;
			}
		}
	}

	RwRGBA rgba;
	GetSceneManager()->GetWorldLightColor(vPos, &rgba, bIsInShadow);
	pSobProxy->EnableWorldLight(TRUE, rgba.red, rgba.green, rgba.blue);
}

RwBool Logic_GetWorldHeight(CNtlSobActor *pActor, const RwV3d *pPos, SWorldHeightStuff& sHStuff, RwV3d *pNormal /*= NULL*/)
{
	if( !pActor )
		return FALSE;

	sHStuff.bWaterAttr = FALSE;

	RwBool bReady = GetSceneManager()->GetWorldHeight(pPos, sHStuff.fWorldHeight, pNormal);
	sHStuff.fFinialHeight = sHStuff.fWorldHeight;

	if(!bReady)
	{
		sHStuff.fFinialHeight = pPos->y;
		return FALSE;
	}

	// 지형 굴곡을 따라서 날아다니는 객체.
	if(pActor->GetFlags() & SLFLAG_NOT_WORLD_HEIGHT_CHECK)
	{
		sHStuff.fFinialHeight = pPos->y;

		Logic_ProcWorldLight(pActor, *pPos);

		return TRUE;
	}

	if( pActor->GetFlags() & SLFLAG_CAN_SWIMMING )
	{
		sHStuff.fWaterHeight	= GetSceneManager()->GetWorldWaterHeight(*pPos);

		if(sHStuff.fWaterHeight > sHStuff.fWorldHeight)
		{
			sHStuff.bWaterAttr		= TRUE;
			RwReal fOffset			= Logic_GetSwimmingOffset(pActor);

			if(sHStuff.fWaterHeight > sHStuff.fWorldHeight + fOffset) 
			{
				sHStuff.fFinialHeight = sHStuff.fWaterHeight - fOffset;
			}
		}
	}

	Logic_ProcWorldLight(pActor, *pPos);

	/*
	DWORD dwAttr = GetSceneManager()->GetWorldNormalAttribute(*pPos);

	if(dwAttr & DBO_WORLD_ATTR_GROUP_FLAG_SWIM)
	{
	sHStuff.bWaterAttr		= TRUE;
	RwReal fOffset			= Logic_GetSwimmingOffset(pActor);
	sHStuff.fWaterHeight	= GetSceneManager()->GetWorldWaterHeight(*pPos);

	if(sHStuff.fWaterHeight > sHStuff.fWorldHeight + fOffset) 
	{
	sHStuff.fFinialHeight = sHStuff.fWaterHeight - fOffset;
	}
	}
	*/

	return TRUE;
}


void Logic_GetWorldSlop(CNtlSobActor *pActor, const RwV3d& vPos, const RwV3d& vDir, RwReal fDepth, RwV3d& vCurPos, RwV3d& vCurSlop, RwReal fUpThreshold /*= 30.f*/, RwReal fDownThreshold /*= 30.f*/)
{
	if( !pActor )
		return;

	RwReal fWorldHeight;

	float fPLHeight = pActor->GetSobProxy()->GetPLEntityHeight();

	RwV3d vBeginPos = vPos; vBeginPos.y = pActor->GetPosition().y + fPLHeight;
	GetSceneManager()->GetWorldHeight( &vBeginPos, fWorldHeight, NULL);
	vBeginPos.y = fWorldHeight;

	RwV3d vEndPos = vPos + vDir * fDepth;  vEndPos.y = pActor->GetPosition().y + fPLHeight;
	GetSceneManager()->GetWorldHeight( &vEndPos, fWorldHeight, NULL);
	vEndPos.y = fWorldHeight;

	vCurSlop = vEndPos - vBeginPos;
	if ( RwV3dNormalize( &vCurSlop, &vCurSlop ) < 0.00001f )
	{
		vCurPos = vBeginPos;
		vCurSlop = vDir;

		Logic_ProcWorldLight( pActor, vCurPos );

		return;
	}

	const RwReal fUpThreshodTheta = fUpThreshold * rwPI / 180.f;
	const RwReal fDownThreshodTheta = fDownThreshold * rwPI / 180.f;

	if ( vCurSlop.y < -0.9999f )	vCurSlop.y = -1.f;
	if ( vCurSlop.y > 0.9999f )		vCurSlop.y = 1.f;

	RwReal fCurTheta = RwASin( vCurSlop.y );

	if ( fCurTheta >= 0.f )
	{
		if ( fCurTheta > fUpThreshodTheta )
		{
			vCurSlop.y = RwSin( fUpThreshodTheta );

			if ( RwV3dNormalize( &vCurSlop, &vCurSlop ) < 0.00001f )
			{
				vCurPos = vBeginPos;
				vCurSlop = vDir;

				Logic_ProcWorldLight( pActor, vCurPos );

				return;
			}
		}
	}
	else
	{
		fCurTheta *= -1.f;

		if ( fCurTheta > fDownThreshodTheta )
		{
			vCurSlop.y = -RwSin( fDownThreshodTheta );

			if ( RwV3dNormalize( &vCurSlop, &vCurSlop ) < 0.00001f )
			{
				vCurPos = vBeginPos;
				vCurSlop = vDir;

				Logic_ProcWorldLight( pActor, vCurPos );

				return;
			}
		}
	}

	vCurPos = vBeginPos;

	RwReal fWeightValue = fabs( vBeginPos.y - vEndPos.y ) / 2.f;
	RwReal fSlopHeight = (vBeginPos.y + vEndPos.y) / 2.f + fWeightValue;
	RwReal fWaterHeight = GetSceneManager()->GetWorldWaterHeight( vCurPos );
	if ( fWaterHeight < fSlopHeight )	vCurPos.y = fSlopHeight;
	else								vCurPos.y = fWaterHeight;

	Logic_ProcWorldLight( pActor, vCurPos );
}

void Logic_GetWorldSlopForVehicle(CNtlSobVehicle* pActor, const RwV3d& vPos, const RwV3d& vDir, RwReal fDepth, RwV3d& vCurPos, RwV3d& vCurSlop, RwReal fUpThreshold /*= 30.f*/, RwReal fDownThreshold /*= 30.f*/)
{
	if( !pActor )
		return;

	RwV3d vBeginPos = vPos;
	SWorldHeightStuff m_sSrcHStuff;
	Logic_GetWorldHeight( &vBeginPos, m_sSrcHStuff );
	vBeginPos.y = m_sSrcHStuff.fFinialHeight;

	RwV3d vEndPos = vPos + vDir * fDepth;
	SWorldHeightStuff m_sDestHStuff;
	Logic_GetWorldHeight( &vEndPos, m_sDestHStuff );
	vEndPos.y = m_sDestHStuff.fFinialHeight;

	vCurSlop = vEndPos - vBeginPos;
	if ( RwV3dNormalize( &vCurSlop, &vCurSlop ) < 0.00001f )
	{
		vCurPos = vPos;
		vCurSlop = vDir;

		Logic_ProcWorldLight( pActor, vCurPos );

		return;
	}

	const RwReal fUpThreshodTheta = fUpThreshold * rwPI / 180.f;
	const RwReal fDownThreshodTheta = fDownThreshold * rwPI / 180.f;

	if ( vCurSlop.y < -0.9999f )	vCurSlop.y = -1.f;
	if ( vCurSlop.y > 0.9999f )		vCurSlop.y = 1.f;

	RwReal fCurTheta = RwASin( vCurSlop.y );

	if ( fCurTheta >= 0.f )
	{
		if ( fCurTheta > fUpThreshodTheta )
		{
			RwV3d vRotSrcDir = vEndPos - vBeginPos; vRotSrcDir.y = 0.f;
			if ( RwV3dNormalize( &vRotSrcDir, &vRotSrcDir ) < 0.0001f )
			{
				vCurPos = vPos;
				vCurSlop = vDir;

				Logic_ProcWorldLight( pActor, vCurPos );

				return;
			}

			RwV3d vCross;
			RwV3dCrossProduct( &vCross, &vRotSrcDir, &YAxis );
			RwV3dNormalize( &vCross, &vCross );

			RtQuat quat;
			RtQuatRotate( &quat, &vCross, fUpThreshodTheta, rwCOMBINEREPLACE );

			RwMatrix matRot;
			RtQuatConvertToMatrix( &quat, &matRot );

			RwV3dTransformPoint( &vCurSlop, &vRotSrcDir, &matRot );
		}
	}
	else
	{
		fCurTheta *= -1.f;

		if ( fCurTheta > fDownThreshodTheta )
		{
			RwV3d vRotSrcDir = vEndPos - vBeginPos; vRotSrcDir.y = 0.f;
			if ( RwV3dNormalize( &vRotSrcDir, &vRotSrcDir ) < 0.0001f )
			{
				vCurPos = vPos;
				vCurSlop = vDir;

				Logic_ProcWorldLight( pActor, vCurPos );

				return;
			}

			RwV3d vCross;
			RwV3dCrossProduct( &vCross, &vRotSrcDir, &-YAxis );
			RwV3dNormalize( &vCross, &vCross );

			RtQuat quat;
			RtQuatRotate( &quat, &vCross, fUpThreshodTheta, rwCOMBINEREPLACE );

			RwMatrix matRot;
			RtQuatConvertToMatrix( &quat, &matRot );

			RwV3dTransformPoint( &vCurSlop, &vRotSrcDir, &matRot );
		}
	}

	vCurPos = vBeginPos;

	Logic_ProcWorldLight( pActor, vCurPos );
}

RwReal Logic_GetSwimmingOffset(CNtlSobActor *pActor)
{
#define SWIM_OFFSET_ADULT           1.48f
#define SWIM_OFFSET_CHILD           0.69f
#define SWIM_OFFSET_CHILD_HUMAN_MALE      0.77f
#define SWIM_OFFSET_CHILD_HUMAN_FEMALE    0.82f


	if( !pActor )
		return 0.f;

	RwUInt32 uiClassID = pActor->GetClassID();
	if( !(uiClassID == SLCLASS_AVATAR || uiClassID == SLCLASS_PLAYER) )
		return 0.0f;

	RwReal fOffset = 0.0f;
	CNtlSobPlayerAttr *pPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pActor->GetSobAttr() );
	if(pPlayerAttr->IsAdult())
	{
		fOffset = SWIM_OFFSET_ADULT;
	}
	else
	{
		if(pPlayerAttr->GetRace() == RACE_HUMAN)
		{
			fOffset = pPlayerAttr->GetGender() == GENDER_MALE ? SWIM_OFFSET_CHILD_HUMAN_MALE : SWIM_OFFSET_CHILD_HUMAN_FEMALE;
		}
        else if(pPlayerAttr->GetRace() == RACE_MAJIN && Logic_IsTransform(pActor))
        {
            // 변신한 순수마인은 어른의 Offset을 적용해야한다.
            fOffset = SWIM_OFFSET_ADULT;
        }
		else
		{
			fOffset = SWIM_OFFSET_CHILD;           
		}
	}

	return fOffset * pActor->GetSobProxy()->GetScale();
}

RwReal Logic_GetSwimmingHeight(CNtlSobActor *pActor, const RwV3d *pPos)
{
	if( !pActor )
		return 0.f;

	SWorldHeightStuff sHStuff;

	RwBool bReady = GetSceneManager()->GetWorldHeight(pPos, sHStuff.fWorldHeight, NULL);
	if(!bReady)
		return pPos->y;

	//	DWORD dwAttr = GetSceneManager()->GetWorldNormalAttribute(*pPos);
	//	if( !(dwAttr & DBO_WORLD_ATTR_GROUP_FLAG_SWIM) )
	//		return sHStuff.fWorldHeight;

	RwReal fOffset			= Logic_GetSwimmingOffset(pActor);
	sHStuff.fWaterHeight	= GetSceneManager()->GetWorldWaterHeight(*pPos);

	if(sHStuff.fWaterHeight < sHStuff.fWorldHeight)
		return sHStuff.fWorldHeight;

	return sHStuff.fWaterHeight - fOffset;
}

RwBool Logic_IsSwimmingActor(CNtlSobActor *pActor, const RwV3d *pPos, SWorldHeightStuff& sHStuff)
{
	if( !pActor )
		return FALSE;

	sHStuff.bWaterAttr = FALSE;

	if(!(pActor->GetFlags() & SLFLAG_CAN_SWIMMING) )
		return FALSE;

	if(pActor->GetFlags() & SLFLAG_NOT_WORLD_HEIGHT_CHECK)
		return FALSE;

	//	DWORD dwAttr = GetSceneManager()->GetWorldNormalAttribute(*pPos);
	//	if( !(dwAttr & DBO_WORLD_ATTR_GROUP_FLAG_SWIM) )
	//		return FALSE;

	if(!GetSceneManager()->GetWorld()->GetWorldReady())
		return FALSE;

	sHStuff.bWaterAttr = TRUE;	
	GetSceneManager()->GetWorldHeight(pPos, sHStuff.fWorldHeight, NULL);
	sHStuff.fFinialHeight = sHStuff.fWorldHeight;

	RwReal fOffset			= Logic_GetSwimmingOffset(pActor);
	sHStuff.fWaterHeight	= GetSceneManager()->GetWorldWaterHeight(*pPos);

	if(sHStuff.fWaterHeight > sHStuff.fWorldHeight + fOffset) 
	{
		sHStuff.fFinialHeight = sHStuff.fWaterHeight - fOffset;
		return TRUE;
	}

	return FALSE;
}

RwBool Logic_IsSwimming(CNtlSobActor *pActor, const RwV3d *pPos, RwReal fWorldHeight, RwReal fWaterHeight)
{
	if( !pActor )
		return FALSE;

	if(!(pActor->GetFlags() & SLFLAG_CAN_SWIMMING) )
		return FALSE;

	if(pActor->GetFlags() & SLFLAG_NOT_WORLD_HEIGHT_CHECK)
		return FALSE;

	//	DWORD dwAttr = GetSceneManager()->GetWorldNormalAttribute(*pPos);
	//	if( !(dwAttr & DBO_WORLD_ATTR_GROUP_FLAG_SWIM) )
	//		return FALSE;

	RwReal fOffset	= Logic_GetSwimmingOffset(pActor);

	if(fWaterHeight > fWorldHeight + fOffset) 
		return TRUE;

	return FALSE;
}

const RwV3d* Logic_GetCameraPos(const RwCamera *pCamera)
{
	if( !pCamera )
		return &g_v3Default;

	return &RwCameraGetFrame(pCamera)->modelling.pos;
}

const RwV3d* Logic_GetCameraAt(const RwCamera *pCamera)
{
	if( !pCamera )
		return &g_v3Default;

	return &RwCameraGetFrame(pCamera)->modelling.at;
}

const RwV3d* Logic_GetCameraUp(const RwCamera *pCamera)
{
	if( !pCamera )
		return &g_v3Default;

	return &RwCameraGetFrame(pCamera)->modelling.up;
}

void Logic_CharacterCollisionBriefAssign(CNtlSobActor *pActor, const RwV3d *pDestPos, SCharCollisionBrief *pCollBrief)
{
	if( !pActor || !pDestPos || !pCollBrief )
		return;

	CNtlSobProxy *pSobProxy = pActor->GetSobProxy();

	pCollBrief->fCharWidth			= pSobProxy->GetPLEntityWidth() + 0.3f;
	pCollBrief->fCharHeight			= pSobProxy->GetPLEntityHeight();
	pCollBrief->fCharDepth			= pSobProxy->GetPLEntityDepth() + 0.3f;

	pCollBrief->fCharHalfWidth		= pCollBrief->fCharWidth/2.0f;
	pCollBrief->fCharHalfHeight		= pCollBrief->fCharHeight/2.0f;
	pCollBrief->fCharHalfDepth		= pCollBrief->fCharDepth/2.0f;

	pCollBrief->vCharCurrPos = pActor->GetPosition();
	RwV3dAssign(&pCollBrief->vCharDestPos, pDestPos);

	pCollBrief->bCollRecursive		= FALSE;
	pCollBrief->bCollMoveImPossible = FALSE;

	pCollBrief->pCollPLEntity		= NULL;
}

void Logic_GetCharacterCollisionAxis(RwReal fInterval, SCharCollisionBrief *pCollBrief)
{
	if( !pCollBrief )
		return;

	RwV3dSub(&pCollBrief->vCharFront,&pCollBrief->vCharDestPos, &pCollBrief->vCharCurrPos);
	pCollBrief->vCharFront.y = 0.0f;
	pCollBrief->fInterval = pCollBrief->fCharHalfDepth + fInterval;
	//pCollBrief->fInterval = RwV3dLength(&pCollBrief->vCharFront) + pCollBrief->fCharHalfDepth + CHAR_COLLISION_LINE_FRONT_OFFSET; 

	RwV3dNormalize(&pCollBrief->vCharFront, &pCollBrief->vCharFront);
	RwV3dCrossProduct(&pCollBrief->vCharRight, &pCollBrief->vCharFront, &CNtlPLGlobal::m_vYAxisV3);
	RwV3dNormalize(&pCollBrief->vCharRight, &pCollBrief->vCharRight);
}

void Logic_GetCharacterCollisionBBox(RwBBox *pBox, SCharCollisionBrief *pCollBrief)
{
	if( !pBox || !pCollBrief )
		return;

	RwV3d vBoxVertex[2];

	RwV3d *pPos = &pCollBrief->vCharCurrPos;
	RwV3d *pFront = &pCollBrief->vCharFront;
	RwV3d *pRight = &pCollBrief->vCharRight;

	// 충돌용 바운딩 박스는 원래 박스 높이의 0.3~0.8까지만 체크한다 (거대 나멕때문)
	// by agebreak 
	vBoxVertex[0].x = pPos->x + (pRight->x * pCollBrief->fCharHalfWidth);
	vBoxVertex[0].y = pPos->y;// + pCollBrief->fCharHeight * 0.3f;
	vBoxVertex[0].z = pPos->z + (pFront->z * pCollBrief->fCharHalfDepth);
	vBoxVertex[1].x = pPos->x - (pRight->x * pCollBrief->fCharHalfWidth);
	vBoxVertex[1].y = pPos->y + pCollBrief->fCharHeight;// * 0.8f;
	vBoxVertex[1].z = pPos->z - (pFront->z * pCollBrief->fCharHalfDepth);

	RwBBoxCalculate(pBox, vBoxVertex, 2);
}

void Logic_GetCharacterCollisionLine(CNtlSobActor *pActor, RwLine *pLine, SCharCollisionBrief *pCollBrief )
{
	// 높이 : character 위치에 character 높이의 30% 지점으로 한다.

	if( !pActor || !pLine || !pCollBrief )
		return;

	RwV3d *pPos = &pCollBrief->vCharCurrPos;
	RwV3d *pFront = &pCollBrief->vCharFront;
	RwV3d *pRight = &pCollBrief->vCharRight;

	RwReal fLineY = pCollBrief->fCharHeight * CHAR_COLLISION_LINE_HEIGHT_OFFSET;

	if ( fLineY < CHAR_COLLISION_LINE_LIMIT_HEIGHT )
	{
		// GreatNamek가 아닌 경우는 Collision height 를 Clipping 한다
		if ( NULL == pActor || !Logic_IsTransformGreatNamek( pActor ) )
		{
			fLineY = CHAR_COLLISION_LINE_LIMIT_HEIGHT;
		}
	}

	fLineY += pPos->y;
	RwReal fRightX = pRight->x * pCollBrief->fCharHalfWidth;
	RwReal fRightZ = pRight->z * pCollBrief->fCharHalfWidth;

	// 중앙
	pLine[0].start.x = pPos->x;
	pLine[0].start.y = fLineY;
	pLine[0].start.z = pPos->z;

	//// 왼쪽
	pLine[1].start.x = pPos->x - fRightX;
	pLine[1].start.y = fLineY;
	pLine[1].start.z = pPos->z - fRightZ;

	// 오른쪽
	pLine[2].start.x = pPos->x + fRightX;
	pLine[2].start.y = fLineY;
	pLine[2].start.z = pPos->z + fRightZ;

	// Y축 방향 체크 라인
	pLine[3].start.x = pPos->x;
	pLine[3].start.y = fLineY;
	pLine[3].start.z = pPos->z;
	pLine[3].end.x = pLine[3].start.x;
	pLine[3].end.y = pLine[3].start.y + (pCollBrief->fCharHeight * (1 - CHAR_COLLISION_LINE_HEIGHT_OFFSET));
	pLine[3].end.z = pLine[3].start.z;

	for(RwInt32 i = 0; i < 3; ++i)
	{
		pLine[i].end.x = pLine[i].start.x + pFront->x*pCollBrief->fInterval;
		pLine[i].end.y = pLine[i].start.y;
		pLine[i].end.z = pLine[i].start.z + pFront->z*pCollBrief->fInterval;
	}    
}


RwBool Logic_GetCharacterCollisionAtomicNormal(RwLine *pLine, SWorldCharIntersectionInfo& sCharInfo, SCharCollisionBrief *pCollBrief, RwBool bJump)
{
	if( !pLine || !pCollBrief )
		return FALSE;

	RwBool bCollision = FALSE;
	RwReal fCollDist = RwRealMAXVAL;
	SWorldCharIntersectionData sCharData;
	pCollBrief->bCollJump = FALSE;

	RwBool bIndoorWorld = GetSceneManager()->GetActiveWorldType() == AW_RWWORLD ? TRUE : FALSE;

	// 점프가 아니면 마지막 라인(y축체크)은 체크하지 않는다.
	RwInt32 nLineCnt = bJump ? CHAR_COLLISION_LINE_COUNT : CHAR_COLLISION_LINE_COUNT - 1;

	for(RwInt32 iLineIdx = 0; iLineIdx < nLineCnt; ++iLineIdx)
	{
		// Indoor일 경우.
		if(bIndoorWorld)
		{
			if(Collision_WorldIntersectionWorldSector(pLine[iLineIdx], sCharData))
			{
				bCollision = TRUE;

				if(sCharData.fMinDist < fCollDist)
				{
					RwV3dAssign(&pCollBrief->vCollPos, &sCharData.vCollPos);
					RwV3dAssign(&pCollBrief->vCollNormal, &sCharData.vNormal);
					pCollBrief->fCollDist = sCharData.fMinDist;
					fCollDist = sCharData.fMinDist;

					pCollBrief->pCollPLEntity = NULL;

					// 점프로 충돌한건지 체크한다.
					if(iLineIdx == CHAR_COLLISION_LINE_JUMP_INDEX)
					{
						pCollBrief->bCollJump = TRUE;
					}
				}
			}
		}

		for(RwInt32 i = 0; i < sCharInfo.byAtomicCount; ++i)
		{
			if(Collision_WorldIntersectionCharacterData(pLine[iLineIdx], sCharInfo.pAtomic[i], sCharData))
			{
				bCollision = TRUE;

				if(sCharData.fMinDist < fCollDist)
				{
					RwV3dAssign(&pCollBrief->vCollPos, &sCharData.vCollPos);
					RwV3dAssign(&pCollBrief->vCollNormal, &sCharData.vNormal);
					pCollBrief->fCollDist = sCharData.fMinDist;
					fCollDist = sCharData.fMinDist;

					pCollBrief->pCollPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData(sCharInfo.pAtomic[i]) );

					// 점프로 충돌한건지 체크한다.
					if(iLineIdx == CHAR_COLLISION_LINE_JUMP_INDEX)
					{
						pCollBrief->bCollJump = TRUE;
					}
				}
			}
		}
	}

	return bCollision;
}

RwBool Logic_IsCharacterCollisionMove(CNtlSobActor *pActor, RwBool bCollision, SCharCollisionBrief *pCollBrief, RwReal fSpeed, RwBool bJump)
{
	if( !pActor || !pCollBrief )
		return FALSE;

	if(!GetSceneManager()->GetAvailablePos(pCollBrief->vCharDestPos))
		return FALSE;

	if(!g_bColliSlantCheck)
		return TRUE;

	RwV3d vCurrPos = pCollBrief->vCharCurrPos;
	RwV3d vDestPos = pCollBrief->vCharDestPos;

	if(!GetNtlWorldConcept()->IsMoveableArea(pActor, &vCurrPos, &vDestPos))
		return FALSE;

	DWORD dwAttr = GetSceneManager()->GetWorldNormalAttribute(vDestPos);
	if( (dwAttr & DBO_WORLD_ATTR_GROUP_FLAG_FORBID_ALL_MOVE) )
		return FALSE;

	if(bCollision)
	{
		if(pCollBrief->pCollPLEntity == NULL)
			return TRUE;
		if(pCollBrief->pCollPLEntity->GetClassType() != PLENTITY_WORLD)
			return TRUE;
	}

	SWorldHeightStuff sHStuff;

	if(bJump)
	{
		Logic_GetWorldHeight(&vCurrPos, sHStuff);
		vCurrPos.y = sHStuff.fFinialHeight;
	}

	RwV3d vNormal;
	bCollision = Logic_GetWorldHeight(&vDestPos, sHStuff, &vNormal);
	if(!bCollision)
		return TRUE;

	if(CNtlPLGlobal::m_pWHEntity)
		return TRUE;

	if(sHStuff.bWaterAttr)
		return TRUE;

	vDestPos.y = sHStuff.fWorldHeight;
	if(vDestPos.y < vCurrPos.y)
		return TRUE;
    
    RwReal fLen = CNtlMath::GetLength(vDestPos, vCurrPos);
	if(fLen >= fSpeed / 1.5f)
		return FALSE;

	RwV3d vRight, vUp;
	RwV3dCrossProduct(&vRight, &vNormal, &CNtlPLGlobal::m_vYAxisV3);
	RwV3dNormalize(&vRight, &vRight);
	RwV3dCrossProduct(&vUp, &vNormal, &vRight);
	RwV3dNormalize(&vUp, &vUp);
	RwV3dScale(&vUp, &vUp, -1.0f);

	if(vUp.y >= g_fCharColliAngle )
	{
		RwV3d vStartPos = pCollBrief->vCharCurrPos;

		RwV3d vEndPos = pCollBrief->vCharDestPos;
		vEndPos.y = vStartPos.y;

		RwV3d vDist = vEndPos - vStartPos;

		fLen = RwV3dLength( &vDist );
		if ( fLen < 0.01f )
		{
			return FALSE;
		}

		RwV3dNormalize( &vDist, &vDist );

		// Ray의 길이를 좀더 길게 잡아준다
		vDist *= fLen + 1.5f;

		vEndPos = vStartPos + vDist;

		SWorldPickInfo sPickingInfo;
        return !GetSceneManager()->PickTerrian(vStartPos, vEndPos, sPickingInfo);		
	}

	return TRUE;
}

void Logic_CharacterCollisionPositinAdjust(SCharCollisionBrief *pCollBrief)
{
	if( !pCollBrief )
		return;

	RwV3d vFrom, vAt, vBack;

	vFrom.x = pCollBrief->vCharCurrPos.x;
	vFrom.y = pCollBrief->vCharCurrPos.y + pCollBrief->fCharHeight*CHAR_COLLISION_LINE_HEIGHT_OFFSET;
	vFrom.z = pCollBrief->vCharCurrPos.z;

	vAt.x = vFrom.x + pCollBrief->vCharFront.x*100.0f;
	vAt.y = vFrom.y;
	vAt.z = vFrom.z + pCollBrief->vCharFront.z*100.0f;

	RwV3dScale(&vBack, &pCollBrief->vCharFront, -1.0f);

	CNtlPlane plane;
	plane.PlaneFromPointNormal(&pCollBrief->vCollPos, &vBack);

	pCollBrief->vAdjustCollPos = plane.SplitPoint(&vFrom, &vAt);

	RwV3d vDelta;
	RwV3dSub(&vDelta, &pCollBrief->vAdjustCollPos, &pCollBrief->vCharCurrPos);
	RwReal fLen = RwV3dLength(&vDelta);
	RwReal fDeltaDist = fLen - pCollBrief->fCharHalfDepth - CHAR_COLLISION_LINE_FRONT_OFFSET;
	if(fDeltaDist > 0)
	{
		pCollBrief->vAdjustCollPos.x = pCollBrief->vCharCurrPos.x + pCollBrief->vCharFront.x*fDeltaDist;
		pCollBrief->vAdjustCollPos.z = pCollBrief->vCharCurrPos.z + pCollBrief->vCharFront.z*fDeltaDist;
	}
	else
	{
		pCollBrief->vAdjustCollPos.x = pCollBrief->vCharCurrPos.x;
		pCollBrief->vAdjustCollPos.z = pCollBrief->vCharCurrPos.z;
	}
};


RwBool Logic_IsCharacterCollisionNewPosition(SCharCollisionBrief *pCollBrief)
{
	if( !pCollBrief )
		return FALSE;

	RwV3d vDelta;
	RwV3dSub(&vDelta, &pCollBrief->vAdjustCollPos, &pCollBrief->vCharCurrPos);
	vDelta.y = 0.0f;

	RwReal fLen = RwV3dLength(&vDelta);
	if(fLen <= pCollBrief->fCharHalfDepth + CHAR_COLLISION_LINE_FRONT_OFFSET)
		return TRUE;

	return FALSE;
}

void Logic_GetCharacterCollisionNewPosition(SCharCollisionBrief *pCollBrief, RwReal fSpeed, RwReal fElapsed)
{
	RwV3d vCollNormal = pCollBrief->vCollNormal;

	vCollNormal.y = 0.0f;
	RwV3dNormalize(&vCollNormal, &vCollNormal);

	RwV3d vCollLeft, vCollRight;
	RwV3dCrossProduct(&vCollRight, &vCollNormal, &CNtlPLGlobal::m_vYAxisV3);
	RwV3dNormalize(&vCollRight, &vCollRight);
	RwV3dScale(&vCollLeft, &vCollRight, -1.0f);

	RwV3d v0, v1;
	RwV3dAdd(&v0, &vCollRight, &vCollNormal);
	RwV3dAdd(&v1, &vCollLeft, &vCollNormal);

	RwV3dNormalize(&v0, &v0);
	RwV3dNormalize(&v1, &v1);

	RwV3d vBack;
	RwV3dScale(&vBack, &pCollBrief->vCharFront, -1.0f);

	RwReal fValue0 = fabs(RwV3dDotProduct(&v0, &vBack));
	RwReal fValue1 = fabs(RwV3dDotProduct(&v1, &vBack));
	RwReal fIncDecSpeed = 1.0f - fabs(RwV3dDotProduct(&vCollNormal, &vBack)) + 0.2f;
	if(fIncDecSpeed > 1.0f)
		fIncDecSpeed = 1.0f;

	if(fElapsed > 1.0f)
		fElapsed = 1.0f;

	fIncDecSpeed = fSpeed*fIncDecSpeed*fElapsed;

	if(fValue0 < fValue1)
	{
		pCollBrief->vCharDestPos.x = pCollBrief->vCharCurrPos.x + vCollRight.x * fIncDecSpeed;
		pCollBrief->vCharDestPos.z = pCollBrief->vCharCurrPos.z + vCollRight.z * fIncDecSpeed;
	}
	else
	{
		pCollBrief->vCharDestPos.x = pCollBrief->vCharCurrPos.x + vCollLeft.x * fIncDecSpeed;
		pCollBrief->vCharDestPos.z = pCollBrief->vCharCurrPos.z + vCollLeft.z * fIncDecSpeed;
	}
}


RwUInt8 Logic_CharacterCollisionBegin(CNtlSobActor *pActor, RwReal fSpeed, RwReal fElapsed, RwBool bJump)
{
	RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;

	if( !pActor )
		return byColliResult;

	Logic_GetCharacterCollisionAxis(fSpeed * fElapsed, &g_sColliBrief);

	// 충돌할 bounding box 영역을 구한다.
	RwBBox box;
	Logic_GetCharacterCollisionBBox(&box, &g_sColliBrief);

	// bounding box 충돌을 한다.	
	RwBool bBoxCollision = FALSE, bCollision = FALSE;
	SWorldCharIntersectionInfo sCharInfo;
	bBoxCollision = Collision_WorldIntersectionCharacter(box, sCharInfo);

	// 충돌한 ray를 구한다.
	RwLine Line[CHAR_COLLISION_LINE_COUNT];
	Logic_GetCharacterCollisionLine(pActor, Line, &g_sColliBrief);

	// 충돌한 atomic의 가까운 polygon을 찾아 normal을 구한다.
	RwBool bIndoorWorld = GetSceneManager()->GetActiveWorldType() == AW_RWWORLD ? TRUE : FALSE;
	if(bBoxCollision || bIndoorWorld)
	{
		bCollision = Logic_GetCharacterCollisionAtomicNormal(Line, sCharInfo, &g_sColliBrief, bJump);    
	}

	if(bCollision)
	{
		byColliResult = g_sColliBrief.bCollJump ? NTL_CHARACTER_COLLI_OBJECT_JUMP : NTL_CHARACTER_COLLI_OBJECT;        

		if(pActor->GetClassID() == SLCLASS_AVATAR && g_sColliBrief.pCollPLEntity)
		{
			RwUInt32 uiSerialId = g_sColliBrief.pCollPLEntity->GetSerialID();
			CNtlSob *pColliSobObj = GetNtlSobManager()->GetSobObject(uiSerialId);
			if(pColliSobObj && pColliSobObj->GetClassID() == SLCLASS_TRIGGER_OBJECT)
			{
				CNtlSLEventGenerator::TSColTriggerObject( uiSerialId );
			}
		}

		if(Logic_IsCharacterCollisionMove(pActor, TRUE, &g_sColliBrief, fSpeed, bJump))
		{
			Logic_CharacterCollisionPositinAdjust(&g_sColliBrief);

			if(Logic_IsCharacterCollisionNewPosition(&g_sColliBrief))
			{
				// 충돌을 했을 경우.. 충돌 기울기를 구한다.
				// 새로운 위치를 setting 한다.
				if(Logic_IsTransformGreatNamek(pActor))
				{
					g_sColliBrief.vNewPos = g_sColliBrief.vCharCurrPos;
					g_sColliBrief.bCollRecursive = FALSE;
				}
				else
				{
					Logic_GetCharacterCollisionNewPosition(&g_sColliBrief, fSpeed, fElapsed);
					g_sColliBrief.vNewPos = g_sColliBrief.vCharDestPos;				    
					g_sColliBrief.bCollRecursive = TRUE;
				}
			}
			else
			{
				RwV3dAssign(&g_sColliBrief.vNewPos, &g_sColliBrief.vAdjustCollPos);
			}

			g_sColliBrief.vNewPos.y = g_sColliBrief.vCharDestPos.y;
		}
		else
		{
			RwV3dAssign(&g_sColliBrief.vNewPos, &g_sColliBrief.vCharCurrPos);
			g_sColliBrief.vNewPos.y = g_sColliBrief.vCharDestPos.y;

			g_sColliBrief.bCollMoveImPossible = TRUE;
		}
	}
	else
	{
		if(!Logic_IsCharacterCollisionMove(pActor, FALSE, &g_sColliBrief, fSpeed, bJump))
		{
			RwV3dAssign(&g_sColliBrief.vNewPos, &g_sColliBrief.vCharCurrPos);			

			g_sColliBrief.bCollMoveImPossible = TRUE;
			bCollision = TRUE;

			byColliResult = NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT;
		}
	}

	return byColliResult;
}


RwBool Logic_CharacterCollisionRecusive(CNtlSobActor* pActor, RwReal fSpeed, RwReal fElapsed, RwBool bJump)
{
	if( !pActor )
		return FALSE;

	Logic_GetCharacterCollisionAxis(fSpeed * fElapsed, &g_sColliBrief);

	// 충돌할 bounding box 영역을 구한다.
	RwBBox box;
	Logic_GetCharacterCollisionBBox(&box, &g_sColliBrief);

	// bounding box 충돌을 한다.
	RwBool bCollision;
	SWorldCharIntersectionInfo sCharInfo;
	bCollision = Collision_WorldIntersectionCharacter(box, sCharInfo);

	// 충돌한 ray를 구한다.
	RwLine Line[CHAR_COLLISION_LINE_COUNT];
	Logic_GetCharacterCollisionLine(pActor, Line, &g_sColliBrief);

	// 충돌한 atomic의 가까운 polygon을 찾아 normal을 구한다.
	RwBool bIndoorWorld = GetSceneManager()->GetActiveWorldType() == AW_RWWORLD ? TRUE : FALSE;
	if(bCollision || bIndoorWorld)
		bCollision = Logic_GetCharacterCollisionAtomicNormal(Line, sCharInfo, &g_sColliBrief, bJump);

	if(bCollision)
	{
		Logic_CharacterCollisionPositinAdjust(&g_sColliBrief);
		if(Logic_IsCharacterCollisionNewPosition(&g_sColliBrief))
			g_sColliBrief.bCollMoveImPossible = TRUE;

		RwV3dAssign(&g_sColliBrief.vNewPos, &g_sColliBrief.vAdjustCollPos);
		g_sColliBrief.vNewPos.y = g_sColliBrief.vCharDestPos.y;
	}

	return bCollision;
}

void Logic_EnableCharacterCollisionSlant(RwBool bSlantCheck)
{
	g_bColliSlantCheck = bSlantCheck;
}

void Logic_EnableCharacterSlipping(RwBool bSlippingCheck)
{
	g_SlippingCheck = bSlippingCheck;
}

RwUInt8 Logic_CharacterCollisionEx(CNtlSobActor *pActor, const RwV3d *pDestPos, RwReal fSpeed, RwV3d& vNewPos, RwBool& bCollMoveImPossible, RwReal fElapsed, RwBool bJump )
{
	if( !pActor )
		return NTL_CHARACTER_COLLI_NONE;

	Logic_CharacterCollisionBriefAssign(pActor, pDestPos, &g_sColliBrief);
	RwUInt8 byColliResult = Logic_CharacterCollisionBegin(pActor, fSpeed, fElapsed, bJump);
	if(byColliResult == NTL_CHARACTER_COLLI_OBJECT && g_sColliBrief.bCollRecursive)
	{
		Logic_CharacterCollisionRecusive(pActor, fSpeed, fElapsed, bJump);
		byColliResult = g_sColliBrief.bCollJump ? NTL_CHARACTER_COLLI_OBJECT_JUMP : NTL_CHARACTER_COLLI_OBJECT;
	}

	if(byColliResult != NTL_CHARACTER_COLLI_NONE)
	{
		bCollMoveImPossible = g_sColliBrief.bCollMoveImPossible;
		RwV3dAssign(&vNewPos, &g_sColliBrief.vNewPos);
	}

	return byColliResult;
}


RwBool Logic_CharacterSlipping(CNtlSobActor *pActor, RwV3d& vNewPos, RwReal fElapsed)
{
	if( !pActor )
		return FALSE;

	if(!g_SlippingCheck)
		return FALSE;

	RwV3d vNormal;
	RwV3d vPos = pActor->GetPosition();

	SWorldHeightStuff sHStuff;
	RwBool bCollision = Logic_GetWorldHeight(pActor, &vPos, sHStuff, &vNormal);
	if(!bCollision)
		return FALSE;

	RwV3d vRight, vUp;
	RwV3dCrossProduct(&vRight, &vNormal, &CNtlPLGlobal::m_vYAxisV3);
	RwV3dNormalize(&vRight, &vRight);
	RwV3dCrossProduct(&vUp, &vNormal, &vRight);
	RwV3dNormalize(&vUp, &vUp);	
	vUp *= -1.0f;

	RwReal fUpValue = vUp.y;
	if(fUpValue < g_fCharSlippingAngle)
		return FALSE;

	vNormal.y = 0.0f;
	RwV3dNormalize(&vNormal, &vNormal);

	if(fElapsed >= 0.1f)
		fElapsed = 0.1f;

	RwReal fWeightSpeed = fUpValue + 1.0f;
	fWeightSpeed = fWeightSpeed * fWeightSpeed;
	vNewPos = vPos + fElapsed * fWeightSpeed * vNormal;
	vNewPos.y = vPos.y;

	DWORD dwAttr = GetSceneManager()->GetWorldNormalAttribute(vNewPos);
	if( (dwAttr & DBO_WORLD_ATTR_GROUP_FLAG_FORBID_ALL_MOVE) )
		return FALSE;

	//Logic_GetWorldHeight(pActor, &vNewPos, sHStuff);
	vNewPos.y = sHStuff.fFinialHeight;

	return TRUE;
}

RwBool Logic_IsMoveWorldArea(CNtlSobActor *pActor, RwV3d vCurrPos, RwV3d vDestPos)
{
	if( !pActor )
		return FALSE;

	if(!GetSceneManager()->GetAvailablePos(vDestPos))
		return FALSE;

	if(!GetNtlWorldConcept()->IsMoveableArea(pActor, &vCurrPos, &vDestPos))
		return FALSE;

	DWORD dwAttr = GetSceneManager()->GetWorldNormalAttribute(vDestPos);
	if( (dwAttr & DBO_WORLD_ATTR_GROUP_FLAG_FORBID_ALL_MOVE) )
		return FALSE;

	return TRUE;
}


CNtlSobPlayer* Logic_GetPlayer_from_CharID(CHARACTERID charID)
{
	CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_PLAYER);

	CNtlSobGroup::MapObject& mapPlayer = pSobGroup->GetObjects();
	CNtlSobGroup::MapObject::iterator it;

	for(it = mapPlayer.begin(); it != mapPlayer.end(); it++)
	{
		CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>( it->second );

		if( pPlayer->GetCharID() == charID )
			return pPlayer;
	}

	return NULL;
}

RwBool Logic_I_am_PartyLeader()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return FALSE;

	CNtlParty* pParty = pAvatar->GetParty();

	if( pAvatar->GetSerialID() == pParty->GetLeaderHandle() )
	{
		return TRUE;
	}

	return FALSE;
}

RwBool Logic_IsMyPartyMember(SERIAL_HANDLE hSerial)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return FALSE;

	CNtlParty* pParty = pAvatar->GetParty();

	if( pParty->IsMember(hSerial) )
		return TRUE;

	return FALSE;
}

RwBool Logic_CanPartyLeave()
{
	// 메인월드는 컨셉이 정의되어 있지 않아 NULL이 반환된다
	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
	if( !pWorldConcept )
		return TRUE;

	EWorldPlayConcept eWorldConcept = pWorldConcept->GetConceptType();
	switch(eWorldConcept)
	{
	case WORLD_PLAY_PARTY_DUNGEON:
	case WORLD_PLAY_DOJO_SCRAMBLE:
		return TRUE;
	}

	// State중에서 Exit가 가장 마지막이라는 가정하에 검사한다
	if( pWorldConcept->GetState() == WORLD_STATE_EXIT ||
		pWorldConcept->GetState() == WORLD_STATE_NONE )
		return TRUE;

	return FALSE;
}

RwBool Logic_CanPartyInvite()
{
	// 메인월드는 컨셉이 정의되어 있지 않아 NULL이 반환된다
	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
	if( !pWorldConcept )
		return TRUE;

	EWorldPlayConcept eWorldConcept = pWorldConcept->GetConceptType();
	switch(eWorldConcept)
	{
	case WORLD_PLAY_PARTY_DUNGEON:
	case WORLD_PLAY_DOJO_SCRAMBLE:
		return TRUE;
	}

	// State중에서 Exit가 가장 마지막이라는 가정하에 검사한다
	if( pWorldConcept->GetState() == WORLD_STATE_EXIT ||
		pWorldConcept->GetState() == WORLD_STATE_NONE )
		return TRUE;

	return FALSE;
}

RwBool Logic_I_am_GuildLeader()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return FALSE;

	CNtlGuild* pGuild = pAvatar->GetGuild();

	if( pGuild->IsGuildMaster( pAvatar->GetCharID() ) )
		return TRUE;

	return FALSE;
}

RwBool Logic_I_am_SecondGuildLeader()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return FALSE;

	CNtlGuild* pGuild = pAvatar->GetGuild();

	if( pGuild->IsSecondGuildMaster( pAvatar->GetCharID() ) )
		return TRUE;

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// world information

RwUInt8	Logic_GetActiveWorldRuleType(void)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	return pAvatarInfo->sWorldInfo.sRuleInfo.byRuleType;
}

sWORLD_INFO* Logic_GetActiveWorldInfo(void)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	return &pAvatarInfo->sWorldInfo;
}

HOBJECT Logic_GetCurrentWorldTriggerOffsetHandle(void)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	return pAvatarInfo->sWorldInfo.hTriggerObjectOffset;
}

RwUInt32 Logic_GetActiveWorldId(void)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	return (RwUInt32)pAvatarInfo->sWorldInfo.worldID;
}

RwUInt32 Logic_GetActiveWorldTableId(void)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	return pAvatarInfo->sWorldInfo.tblidx;
}

const RwChar* Logic_GetActiveWorldName(void)
{
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	CWorldTable *pWorldTable = API_GetTableContainer()->GetWorldTable();
	sWORLD_TBLDAT *pWorldTblData = reinterpret_cast<sWORLD_TBLDAT*>( pWorldTable->FindData(pAvatarInfo->sWorldInfo.tblidx) );
	return pWorldTblData->szName;
}

void Logic_SetHeadNameColor(CNtlSob* pSob)
{
	if( !pSob )
		return;

	switch( pSob->GetClassID() )
	{
	case SLCLASS_AVATAR:
		{
			CNtlSobAvatar* pAvatar = reinterpret_cast<CNtlSobAvatar*>(pSob);
			CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );			
			CNtlSobCharProxy* pPlayerProxy = reinterpret_cast<CNtlSobCharProxy*>( pAvatar->GetSobProxy() );
			CNtlParty* pParty = pAvatar->GetParty();
			CNtlGuild* pGuild = pAvatar->GetGuild();
			COLORREF avatarNameColor = NTL_PLAYER_NAME_COLOR;

			if( pParty->IsHaveGroup() )
				avatarNameColor = NTL_PARTY_NAME_COLOR;

			pPlayerProxy->SetNameColor(pAvatarAttr->GetName(), avatarNameColor,
				pGuild->GetGuildName(), NTL_GUILD_NAME_COLOR);

			break;
		}		
	case SLCLASS_PLAYER:	
		{
			CNtlSobPlayer* pPlayer = reinterpret_cast<CNtlSobPlayer*>(pSob);
			CNtlSobPlayerAttr* pPlayerAttr = reinterpret_cast<CNtlSobPlayerAttr*>( pPlayer->GetSobAttr() );			
			CNtlSobCharProxy* pPlayerProxy = reinterpret_cast<CNtlSobCharProxy*>( pPlayer->GetSobProxy() );
			CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();
			COLORREF playerNameColor = NTL_PLAYER_NAME_COLOR;

			if( pParty->IsMember( pSob->GetSerialID() ) )
				playerNameColor = NTL_PARTY_NAME_COLOR;

			pPlayerProxy->SetNameColor(pPlayerAttr->GetName(), playerNameColor,
				pPlayer->GetGuildName(), NTL_GUILD_NAME_COLOR);

			break;
		}
	case SLCLASS_MONSTER:
		{
			CNtlSobMonster* pMonster = reinterpret_cast<CNtlSobMonster*>(pSob);
			CNtlSobMonsterAttr* pMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>( pMonster->GetSobAttr() );
			sMOB_TBLDAT* pMOB_TBLDAT = pMonsterAttr->GetMobTbl();
			CNtlSobCharProxy* pMonsterProxy = reinterpret_cast<CNtlSobCharProxy*>( pMonster->GetSobProxy() );

			CNtlStorageGroupScouter* pGroup = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup( eNTL_STORAGE_GROUP_SCOUTER );
			if( pGroup->IsUseMonsterScouter(pMOB_TBLDAT->dwMobGroup) ||
				pMOB_TBLDAT->bShow_Name )
			{
				COLORREF monsterNameColor = NTL_MONSTER_NAME_COLOR;

				// 아바타와 몬스터의 레벨을 비교하여 이름을 출력한다.
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( pAvatar )
				{
					CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );			

					if( pAvatarAttr->GetLevel() + 3 < pMonsterAttr->GetLevel() )
						monsterNameColor = NTL_MONSTER_NAME_COLOR_STRONG;
					else if( pAvatarAttr->GetLevel() - 3 > pMonsterAttr->GetLevel() )
						monsterNameColor = NTL_MONSTER_NAME_COLOR_WEAKNESS;
				}

				pMonsterProxy->SetNameColor(pMonsterAttr->GetName(), monsterNameColor,
					NULL, NTL_GUILD_NAME_COLOR);
			}
			else
				pMonsterProxy->SetNameColor(L"???", NTL_MONSTER_NAME_COLOR,
				NULL, NTL_GUILD_NAME_COLOR);

			break;
		}		
	case SLCLASS_NPC:
		{
			CNtlSobNpc* pNPC = reinterpret_cast<CNtlSobNpc*>(pSob);
			CNtlSobNpcAttr* pNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pNPC->GetSobAttr() );
			CNtlSobCharProxy* pNPCProxy = reinterpret_cast<CNtlSobCharProxy*>( pNPC->GetSobProxy() );

			pNPCProxy->SetNameColor(pNPCAttr->GetName(), NTL_NPC_NAME_COLOR,
				NULL, NTL_GUILD_NAME_COLOR);

			break;
		}		
	case SLCLASS_PET:
		break;
	case SLCLASS_SLOT_ITEM:
		break;
	case SLCLASS_SLOT_QUESTITEM:
		break;
	case SLCLASS_PROJECTILE:
		break;
	case SLCLASS_BUFF:
		break;
	case SLCLASS_SKILL:
		break;
	case SLCLASS_WORLD_ITEM:
		break;
	case SLCLASS_HTBSKILL:
		break;
	case SLCLASS_ACTIONSKILL:
		break;
	case SLCLASS_TRIGGER_OBJECT:
		{
			CNtlSobTriggerObject* pTriggerObject = reinterpret_cast<CNtlSobTriggerObject*>(pSob);
			CNtlSobTriggerObjectAttr* pTriggerObjectAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>( pTriggerObject->GetSobAttr() );
			CNtlSobTriggerObjectProxy* pTriggerObjectAttrProxy = reinterpret_cast<CNtlSobTriggerObjectProxy*>( pTriggerObject->GetSobProxy() );

			// 트리거 오브젝트는 이름이 없을 수도 있다
			if( pTriggerObjectAttr->GetName() )
				pTriggerObjectAttrProxy->SetNameColor(pTriggerObjectAttr->GetName(), NTL_OBJ_NAME_COLOR);
			else
				pTriggerObjectAttrProxy->SetNameColor(L"", NTL_OBJ_NAME_COLOR);
		}
		break;
	case SLCLASS_DYNAMIC_OBJECT:
		{
			CNtlSobDynamicObject* pDynamicObject = reinterpret_cast<CNtlSobDynamicObject*>(pSob);
			CNtlSobDynamicObjectAttr* pDynamicObjectAttr = reinterpret_cast<CNtlSobDynamicObjectAttr*>( pDynamicObject->GetSobAttr() );
			CNtlSobDynamicObjectProxy* pDynamicObjectAttrProxy = reinterpret_cast<CNtlSobDynamicObjectProxy*>( pDynamicObject->GetSobProxy() );

			if( pDynamicObjectAttr->GetName() )
				pDynamicObjectAttrProxy->SetNameColor(pDynamicObjectAttr->GetName(), NTL_OBJ_NAME_COLOR);
			else
				pDynamicObjectAttrProxy->SetNameColor(L"", NTL_OBJ_NAME_COLOR);
		}
		break;
	case SLCLASS_VEHICLE:
		break;
	}
}

void Logic_SetNickNameColor(CNtlSob* pSob)
{
	if( !pSob )
		return;

	switch( pSob->GetClassID() )
	{
	case SLCLASS_AVATAR:
		break;
	case SLCLASS_PLAYER:	
		break;
	case SLCLASS_MONSTER:
		break;
	case SLCLASS_NPC:
		{
			CNtlSobNpc* pNPC = reinterpret_cast<CNtlSobNpc*>(pSob);
			CNtlSobNpcAttr* pNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pNPC->GetSobAttr() );
			CNtlSobCharProxy* pNPCProxy = reinterpret_cast<CNtlSobCharProxy*>( pNPC->GetSobProxy() );

			pNPCProxy->SetNickNameColor( pNPCAttr->GetNickName(), pNPCAttr->GetNickNameColor() );

			break;
		}		
	case SLCLASS_PET:
		break;
	case SLCLASS_SLOT_ITEM:
		break;
	case SLCLASS_SLOT_QUESTITEM:
		break;
	case SLCLASS_PROJECTILE:
		break;
	case SLCLASS_BUFF:
		break;
	case SLCLASS_SKILL:
		break;
	case SLCLASS_WORLD_ITEM:
		break;
	case SLCLASS_HTBSKILL:
		break;
	case SLCLASS_ACTIONSKILL:
		break;
	case SLCLASS_TRIGGER_OBJECT:
		break;
	case SLCLASS_DYNAMIC_OBJECT:
		break;
	case SLCLASS_VEHICLE:
		break;
	}
}

RwBool Logic_IsCanCreatePrivateShop()
{
	// 게임 룰 컨셉에 게임상점 준비 상태가 가능한지 물어보고 불가능하다면 리턴시킨다.
	if( CNtlGameRuleConcept::GetInstance()->CanTransition( GAME_RULE_TYPE_MAIN_PRIVATESHOP_READY ) == FALSE )
		return FALSE;

	CNtlSobAvatar*	pAvater	= GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvater )
		return FALSE;

	RwV2d v2Avater, v2Target, v2Sub;
	v2Avater.x	= pAvater->GetPosition().x;	
	v2Avater.y	= pAvater->GetPosition().z;

	CNtlSob *pSobObj;
	CNtlSobPlayer *pPlayer;

	CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_PLAYER);
	if (pSobGroup)
	{
		RwReal			fLimitedDistance	= 3.0f;

		for (CNtlSobGroup::MapObject::iterator it = pSobGroup->GetObjects().begin();
			it != pSobGroup->GetObjects().end(); ++it)
		{
			pPlayer		= reinterpret_cast<CNtlSobPlayer*>(it->second);

			v2Target.x	= pPlayer->GetPosition().x;
			v2Target.y	= pPlayer->GetPosition().z;

			RwV2dSub(&v2Sub, &v2Target, &v2Avater);

			RwReal fLength = RwV2dLength(&v2Sub);
			if (fLimitedDistance < fLength)
			{
				continue;
			}

			if(FSMUtil_IsPrivateShopping(pPlayer->GetCurrentState()))
			{
				// 반경 3M 이내에 개인 상점이 하나 이상 있습니다
				CNtlSLEventGenerator::SysMsg(pAvater->GetSerialID(), GAME_PRIVATESHOP_PRIVATESHOP_ANOTHER_PRIVATESHOP);
				return FALSE;
			}
		}
	}

	RwReal fPLWidth, fPLDepth, fLimitedDistance = 2.0f;
	RwInt32 arrClass[2] = {SLCLASS_NPC, SLCLASS_TRIGGER_OBJECT};
	RwV3d vPos;

	for(int i = 0; i < 2; i++)
	{
		// npc 검사.
		pSobGroup = GetNtlSobManager()->GetSobGroup(arrClass[i]);
		if (pSobGroup)
		{
			for (CNtlSobGroup::MapObject::iterator it = pSobGroup->GetObjects().begin();
				it != pSobGroup->GetObjects().end(); ++it)
			{
				pSobObj	= it->second;

				vPos = pSobObj->GetPosition();

				v2Target.x	= vPos.x;
				v2Target.y	= vPos.z;

				fPLWidth = pSobObj->GetSobProxy()->GetPLEntityWidth();
				fPLDepth = pSobObj->GetSobProxy()->GetPLEntityDepth();

				RwV2dSub(&v2Sub, &v2Target, &v2Avater);

				RwReal fLength = RwV2dLength(&v2Sub);
				if (fLength <= fLimitedDistance + (fPLWidth + fPLDepth)/4.0f )
				{
					// 반경 2M 이내에 NPC가 존재합니다. ( NPC 와 겹치는 문제를 해결 )
					CNtlSLEventGenerator::SysMsg(pAvater->GetSerialID(), GAME_PRIVATESHOP_PRIVATESHOP_ANOTHER_NPC);
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

RwBool Logic_IsTMQBoarding(CNtlSobActor *pActor)
{
	if( !pActor )
		return FALSE;

	RwUInt32 uiClassId = pActor->GetClassID();

	if(uiClassId == SLCLASS_AVATAR || uiClassId == SLCLASS_PLAYER)
	{
		sCHARSTATE* pState = pActor->GetServerFullState();

		if ( pState->sCharStateBase.byStateID == CHARSTATE_DIRECT_PLAY )
		{
			switch ( pState->sCharStateDetail.sCharStateDirectPlay.byDirectPlayType )
			{
			case DIRECT_PLAY_TQS_START:
			case DIRECT_PLAY_TQS_ARRIVE:
			case DIRECT_PLAY_TQS_LEAVE:
				return TRUE;
			case DIRECT_PLAY_NORMAL:
			case DIRECT_PLAY_VEHICLE:
				return FALSE;
			}
		}
	}

	return FALSE;
}

RwBool Logic_IsCoolTimeUpdate(CNtlSobActor *pActor)
{
	if( !pActor )
		return FALSE;

	CNtlFSMStateBase *pStateBase = pActor->GetCurrentState();

	if(pStateBase == NULL)
		return FALSE;

	RwUInt32 uiCurrStateId = pStateBase->GetStateId();

	if(uiCurrStateId == NTL_FSMSID_TELEPORT || uiCurrStateId == NTL_FSMSID_DESPAWN ||
		uiCurrStateId == NTL_FSMSID_SPAWN)
		return FALSE;

	return TRUE;
}

// 블랙리스트에 있는지 확인한다.
RwBool Logic_IsBlackList( RwUInt32 uiSerialID ) 
{
	CNtlSob* pActor = GetNtlSobManager()->GetSobObject(uiSerialID);
	if(!pActor)
		return FALSE;

	if(GetNtlSLGlobal()->GetSobAvatar()->GetBlackList()->GetMemberbyName((WCHAR*)Logic_GetName(pActor)))
		return TRUE;
	else
		return FALSE;
}


RwBool Logic_ProcKnockdownMatrixDirection(CNtlSob *pSobObj, RwUInt32 uiMatrixSerialId, const SHitStuff *pHitStuff)
{
	if( !pSobObj )
		return FALSE;

	if( GetNtlGameCameraManager()->FindController(CAMERA_CONTROL_KNOCKDOWN_MATRIX) )
		return FALSE;

	RwUInt8 byServerState = 255, byClientState = 255;
	CNtlSLEventGenerator::SobGetState(pSobObj->GetSerialID(), byServerState, byClientState);

	if(byServerState == 255)
		return FALSE;

	//if(byServerState == CHARSTATE_HTB)
	//	return FALSE;

	if(pHitStuff->uiAttr.bitLogic == NTL_ATTACK_LOGIC_SKILL)
	{
		CSkillTable *pSkillTable = API_GetTableContainer()->GetSkillTable();
		CSystemEffectTable *pSysEffTable = API_GetTableContainer()->GetSystemEffectTable();

		sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData(pHitStuff->uiAttrLogicTblId) );
		if(pSkillTblData)
		{
			for(RwInt32 i = 0; i < NTL_MAX_EFFECT_IN_SKILL; i++)
			{
				if(pSkillTblData->skill_Effect[i] == INVALID_TBLIDX)
					continue;

				sSYSTEM_EFFECT_TBLDAT *pSysEffTblData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSysEffTable->FindData(pSkillTblData->skill_Effect[i]) );
				if( pSysEffTblData->effectCode == PASSIVE_GUARD_COUNTERATTACK)
					return FALSE;
			}
		}
	}

	GetNtlSobElapsedControlManager()->CreateController(pSobObj, g_fMatrixTime, g_fMatrixValue, g_fMatrixDelayTime);
	CNtlSLEventGenerator::CameraKnockDownMatrix(uiMatrixSerialId, g_fMatrixCamera);

	return TRUE;
}

RwBool Logic_IsCinematic() 
{
	return GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DIRECT);
}

void Logic_SetClickDisable( CNtlSobActor* pActor, RwBool bDisable ) 
{
	CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)pActor->GetFSMLayer()->GetFSMAgent();
	pAgent->SetClickDisable(bDisable);
}

RwBool Logic_IsClickDisable()
{
	CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)GetNtlSLGlobal()->GetSobAvatar()->GetFSMLayer()->GetFSMAgent();
	return pAgent->IsClickDisable();
}

void Logic_SetAutoRun(RwBool bAutoRun)
{
	if ( Logic_IsVehicleDriver( GetNtlSLGlobal()->GetSobAvatar() ) )
	{
		CNtlSobVehicle* pSobVehicle = (CNtlSobVehicle*)GetNtlSobManager()->GetSobObject( GetNtlSLGlobal()->GetSobAvatar()->GetVehicleID() );

		if ( pSobVehicle )
		{
			CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)pSobVehicle->GetFSMLayer()->GetFSMAgent();
			pAgent->SetAutoRun(bAutoRun);
		}
	}
	else
	{
		CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)GetNtlSLGlobal()->GetSobAvatar()->GetFSMLayer()->GetFSMAgent();
		pAgent->SetAutoRun(bAutoRun);
	}
}

RwInt32 Logic_isPartyShareTarget( SERIAL_HANDLE hSerial ) 
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(!pSobAvatar)
		return -1;

	sSHARETARGET_INFO* pShareTargetInfos = pSobAvatar->GetParty()->GetShareTargetInfos();

	for(int i = 0; i < NTL_MAX_SHARETARGET_COUNT; ++i)
	{
		if(pShareTargetInfos[i].hTarget == hSerial)
		{
			return i;
		}
	}

	// 일치하는것이 없으면 -1을 반환한다.
	return -1;
}

RwUInt32 Logic_GetShareTargetSerial( RwInt32 nSlot ) 
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(!pSobAvatar)
		return INVALID_SERIAL_ID;

	sSHARETARGET_INFO* pShareTargetInfos = pSobAvatar->GetParty()->GetShareTargetInfos();

	return pShareTargetInfos[nSlot].hTarget;
}

RwInt32 Logic_GetShareTargetBlankSlot() 
{
	static RwInt32 nBlankSlot = 0;

	RwInt32 nSlot = Logic_isPartyShareTarget(INVALID_SERIAL_ID);
	if(nSlot == -1) // 빈 슬롯이 없을때
	{
		nSlot = nBlankSlot;

		++nBlankSlot;
		if(nBlankSlot >= NTL_MAX_SHARETARGET_COUNT)
		{
			nBlankSlot = 0;
		}
	}

	return nSlot;
}

sDOJO_INFO* Logic_GetDojoInfo(SERIAL_HANDLE hNPC)
{
	sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(hNPC);
	if( !pNPC_TBLDAT )
		return NULL;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return NULL;

	CNtlDojo* pDojo = pAvatar->GetDojo();
	sDOJO_INFO* pDOJO_INFO = pDojo->GetDojoInfo( pNPC_TBLDAT->contentsTblidx );

	return pDOJO_INFO;
}

eDBO_TEAM Logic_GetScrambleTeam(eTOBJECT_STATE_TYPE_C eTObjectState)
{
	if( MAX_TOBJECT_STATE_TYPE_C <= eTObjectState )
		return DBO_TEAM_NEUTRAILITY;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return DBO_TEAM_NEUTRAILITY;

	CNtlDojo* pDojo = pAvatar->GetDojo();
	sSCRAMBLE_INFO* pSCRAMBLE_INFO = pDojo->GetScramble();


	switch(eTObjectState)
	{
	case TOBJECT_STATE_TYPE_C_ATTACK_TEAM:
		{
			if( SCRAMBLE_TEAM_OFFENCE == pSCRAMBLE_INFO->eAvatarTeam )
			{
				return DBO_TEAM_MY_TEAM;
			}
			else
			{
				return DBO_TEAM_ENEMY;
			}
		}
	case TOBJECT_STATE_TYPE_C_DEFENCE_TEAM:
		{
			if( SCRAMBLE_TEAM_DEFENCE == pSCRAMBLE_INFO->eAvatarTeam )
			{
				return DBO_TEAM_MY_TEAM;
			}
			else
			{
				return DBO_TEAM_ENEMY;
			}
		}
	}

	return DBO_TEAM_NEUTRAILITY;
}

RwBool Logic_IsBus( CNtlSobActor* pSobObj ) 
{
	if(!pSobObj || pSobObj->GetClassID() != SLCLASS_NPC)
		return FALSE;

	CNtlSobNpcAttr* pNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pSobObj->GetSobAttr() );
	sNPC_TBLDAT* pTableData = pNPCAttr->GetNpcTbl();
	if(!pTableData)
		return FALSE;

	return pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_BUS;        
}

RwBool Logic_IsBus(SERIAL_HANDLE hHandle)
{
	CNtlSobActor *pActor = reinterpret_cast<CNtlSobActor*>(GetNtlSobManager()->GetSobObject(hHandle)); 

	return Logic_IsBus( pActor );
}

RwBool Logic_IsVehicleDriver( CNtlSobActor* pSobObj ) 
{
	if ( NULL == pSobObj ) return FALSE;

	if ( SLCLASS_PLAYER != pSobObj->GetClassID() &&
		SLCLASS_AVATAR != pSobObj->GetClassID() )
	{
		return FALSE;
	}

	if ( ASPECTSTATE_VEHICLE != ((CNtlSobPlayer*) pSobObj)->GetAspectState()->sAspectStateBase.byAspectStateId )
	{
		return FALSE;
	}

	return TRUE;
}

RwBool Logic_IsVehicleDriver( SERIAL_HANDLE hHandle ) 
{
	CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject( hHandle ) ); 

	return Logic_IsVehicleDriver( pActor );
}

RwBool Logic_CanRideOnVehicle(SERIAL_HANDLE hHandle, CNtlSobItem* pSobItem)
{
	if ( NULL == pSobItem )
	{
		return FALSE;
	}

	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
	if ( NULL == pSobItemAttr )
	{
		return FALSE;
	}

	sITEM_TBLDAT* pItemTbl = pSobItemAttr->GetItemTbl();
	if ( NULL == pItemTbl )
	{
		return FALSE;
	}

	CUseItemTable* pUseItemTbl = API_GetTableContainer()->GetUseItemTable();

	if ( NULL == pUseItemTbl )
	{
		// 해당 아이템이 vehicle 아이템이 아닌 경우는 TRUE
		return TRUE;
	}

	sUSE_ITEM_TBLDAT* pUseItemTblData = reinterpret_cast< sUSE_ITEM_TBLDAT* >( pUseItemTbl->FindData( pItemTbl->Use_Item_Tblidx ) );

	if ( NULL == pUseItemTblData )
	{
		// 해당 아이템이 vehicle 아이템이 아닌 경우는 TRUE
		return TRUE;
	}

	CSystemEffectTable* pSystemEffectTbl = API_GetTableContainer()->GetSystemEffectTable();

	if ( NULL == pSystemEffectTbl )
	{
		// 해당 아이템이 vehicle 아이템이 아닌 경우는 TRUE
		return TRUE;
	}

	RwBool bVehicleItem = FALSE;

	for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_ITEM ; ++i )
	{
		sSYSTEM_EFFECT_TBLDAT* pSystemEffectData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSystemEffectTbl->FindData( pUseItemTblData->aSystem_Effect[i] ) );

		if ( pSystemEffectData && pSystemEffectData->effectCode == ACTIVE_VEHICLE )
		{
			bVehicleItem = TRUE;
			break;
		}
	}

	if ( !bVehicleItem )
	{
		// 해당 아이템이 vehicle 아이템이 아닌 경우는 TRUE
		return TRUE;
	}

	// 지형 검사

	CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject( hHandle );

	if ( NULL == pSobObj )
	{
		return FALSE;
	}

	CNtlPLWorldEntity* pWorldEntity = (CNtlPLWorldEntity*)GetSceneManager()->GetWorld();
	if ( NULL == pWorldEntity )
	{
		return FALSE;
	}

	RwV3d vCurPos = pSobObj->GetPosition();

	TBLIDX tblAreaInfoIndex = pWorldEntity->GetMapNameCode( vCurPos );

	if ( 0xffffffff == tblAreaInfoIndex )
	{
		return FALSE;
	}

	TBLIDX tblZoneTblIdx = tblAreaInfoIndex / 1000;

	sWORLD_ZONE_TBLDAT* pZoneData = (sWORLD_ZONE_TBLDAT*) API_GetTableContainer()->GetWorldZoneTable()->FindData( tblZoneTblIdx );

	if ( NULL == pZoneData )
	{
		return FALSE;
	}

	if ( pZoneData->bForbidden_Vehicle )
	{
		return FALSE;
	}

	return TRUE;
}

TBLIDX Logic_GetRegisterFuelItemTableIndex()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
		return INVALID_SERIAL_ID;

	CNtlOtherParam*	pOtherParam	= pAvatar->GetOtherParam();

	return pOtherParam->GetRegisterFuelItemTableIndex();
}

void Logic_SetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex)
{
	if( uiIndex > uiMaxIndex )
		return;

	if( !pcFlags )
		return;

	RwUInt32 uiArrayPos = uiIndex / 8;
	RwUInt8 byCurBit	= (RwUInt8)(uiIndex % 8);

	pcFlags[uiArrayPos] |= 0x01ui8 << byCurBit;
}

void Logic_UnsetFlags(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex)
{
	if( uiIndex > uiMaxIndex )
		return;

	if( !pcFlags )
		return;

	RwUInt32 uiArrayPos = uiIndex / 8;
	RwUInt8 byCurBit	= (RwUInt8)(uiIndex % 8);

	pcFlags[uiArrayPos] &= ~(0x01ui8 << byCurBit);
}

RwBool Logic_IsExistFlag(RwChar* pcFlags, RwUInt32 uiIndex, RwUInt32 uiMaxIndex)
{
	if( uiIndex > uiMaxIndex )
		return FALSE;

	if( !pcFlags )
		return FALSE;

	RwUInt32 uiArrayPos = uiIndex / 8;
	RwUInt8 byCurBit	= (RwUInt8)(uiIndex % 8);

	if( BIT_FLAG_TEST(pcFlags[uiArrayPos], 0x01ui8 << byCurBit) )
		return TRUE;

	return FALSE;
}

RwBool Logic_IsTransform( CNtlSobActor* pSobActor ) 
{
	if(!pSobActor)
		return FALSE;

	if(pSobActor->GetClassID() != SLCLASS_AVATAR && pSobActor->GetClassID() != SLCLASS_PLAYER)
		return FALSE;

	CNtlSobCharProxy* pProxy = (CNtlSobCharProxy*)pSobActor->GetSobProxy();
	if(!pProxy->GetDecorationProxy())
		return FALSE;

	return pProxy->GetDecorationProxy()->GetProxyTransform()->IsTransform();
}

RwBool Logic_IsTransformGreatNamek( CNtlSobActor* pSobActor ) 
{
	if( !pSobActor )
		return FALSE;

	if(Logic_GetPlayerRace(pSobActor) == RACE_NAMEK)
	{
		CNtlSobPlayer* pPlayer = (CNtlSobPlayer*)pSobActor;

		return pPlayer->GetAspectState()->sAspectStateBase.byAspectStateId == ASPECTSTATE_GREAT_NAMEK;
	}

	return FALSE;
}

RwBool Logic_IsTransformSpinAttack(CNtlSobActor* pSobActor)
{
	if( !pSobActor )
		return FALSE;

	if(Logic_GetPlayerRace(pSobActor) == RACE_MAJIN)
	{
		CNtlSobPlayer* pPlayer = (CNtlSobPlayer*)pSobActor;

		return (pPlayer->GetAspectState()->sAspectStateBase.byAspectStateId == ASPECTSTATE_SPINNING_ATTACK);
	}

	return FALSE;
}

// 변신스킬이 아니면, 0을 반환
// 변신 스킬이면 몇레벨 변신 스킬인지 반환
RwUInt32 Logic_IsTransformSkill( RwUInt32 uiSkillTblId ) 
{
	sSKILL_TBLDAT* pData = (sSKILL_TBLDAT*)API_GetTableContainer()->GetSkillTable()->FindData(uiSkillTblId);
	if(!pData)
		return 0;

	for(RwInt32 i = 0; i < NTL_MAX_EFFECT_IN_SKILL; ++i)
	{
		sSYSTEM_EFFECT_TBLDAT* pEffectData = (sSYSTEM_EFFECT_TBLDAT*)API_GetTableContainer()->GetSystemEffectTable()->FindData(pData->skill_Effect[i]);
		if(!pEffectData)
			continue;

		if(pEffectData->effectCode == ACTIVE_TRANSFORM      ||
			pEffectData->effectCode == ACTIVE_SUPER_SAIYAN   ||
			pEffectData->effectCode == ACTIVE_GREAT_NAMEK    ||
			pEffectData->effectCode == ACTIVE_PURE_MAJIN     ||
			pEffectData->effectCode == ACTIVE_KAIOKEN)
		{
			return pEffectData->effectCode;
		}
	}

	return NULL;
}

RwBool Logic_CanMouseInput_in_Tutorial(ETL_MOUSE_INPUT_TYPE byType)
{
	if ( !CNtlWorldConcept::GetInstance()->HasTutorialWorldConcept() )
	{
		return TRUE;
	}

	CNtlWorldConceptTutorial* pTutorialController = reinterpret_cast<CNtlWorldConceptTutorial*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TUTORIAL) );
	if( pTutorialController )
	{
		if( pTutorialController->CanMouseInput(byType) )
			return TRUE;
		else
		{
			CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, DST_TUTORIAL_CAN_NOT_WORK, FALSE );
			return FALSE;
		}
	}

	return TRUE;
}

RwBool Logic_CanKeybaordInput_in_Tutorial(ETL_KEYBOARD_INPUT_TYPE byType)
{
	if ( !CNtlWorldConcept::GetInstance()->HasTutorialWorldConcept() )
	{
		return TRUE;
	}

	CNtlWorldConceptTutorial* pTutorialController = reinterpret_cast<CNtlWorldConceptTutorial*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TUTORIAL) );
	if( pTutorialController )
	{
		if( pTutorialController->CanKeybaordInput(byType) )
			return TRUE;
		else
		{
			CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, DST_TUTORIAL_CAN_NOT_WORK, FALSE );
			return FALSE;
		}
	}

	return TRUE;
}

RwBool Logic_CanAttack_in_Tutorial(ETL_ATTACK_TYPE byType)
{
	if ( !CNtlWorldConcept::GetInstance()->HasTutorialWorldConcept() )
	{
		return TRUE;
	}

	CNtlWorldConceptTutorial* pTutorialController = reinterpret_cast<CNtlWorldConceptTutorial*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TUTORIAL) );
	if( pTutorialController )
	{
		if( pTutorialController->CanAttack(byType) )
			return TRUE;
		else
		{
			CNtlSLEventGenerator::SysMsg( INVALID_SERIAL_ID, DST_TUTORIAL_CAN_NOT_WORK, FALSE );
			return FALSE;
		}
	}

	return TRUE;
}

VOID Logic_SendTutorialCondition(ETL_CONDITION_TYPE byType, RwUInt32 uiParam1 /*= 0xffffffff*/ )
{
	if ( !CNtlWorldConcept::GetInstance()->HasTutorialWorldConcept() )
	{
		return;
	}

	CNtlWorldConceptTutorial* pTutorialController = reinterpret_cast<CNtlWorldConceptTutorial*>( GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_TUTORIAL) );
	if( pTutorialController )
	{
		CNtlSLEventGenerator::TLCondition(byType, uiParam1);
	}
}

RwBool Logic_IsCondition( CNtlSobActor* pActor, RwUInt32 uiConditon ) 
{
	if(!pActor)
		return FALSE;

	CNtlFSMLayer *pFSMLayer = pActor->GetFSMLayer();
	if(pFSMLayer == NULL)
		return FALSE;

	CNtlFSMCharActAgent* pAgent = (CNtlFSMCharActAgent*)pFSMLayer->GetFSMAgent();

	return pAgent->GetServerCondition() & uiConditon;
}

RwBool Logic_IsDirectMode( CNtlSobActor* pActor ) 
{
	if ( !pActor )
	{
		return FALSE;
	}

	CNtlSobActor* pVehicle = (CNtlSobActor*)GetNtlSobManager()->GetSobObject( pActor->GetVehicleID() );

	CNtlFSMCharActAgent* pAgent;

	if ( pVehicle )
	{
		CNtlFSMLayer* pFSMLayer = pVehicle->GetFSMLayer();
		if ( NULL == pFSMLayer )
		{
			return FALSE;
		}

		pAgent = (CNtlFSMCharActAgent*)pFSMLayer->GetFSMAgent();
	}
	else
	{
		CNtlFSMLayer* pFSMLayer = pActor->GetFSMLayer();
		if ( NULL == pFSMLayer )
		{
			return FALSE;
		}

		pAgent = (CNtlFSMCharActAgent*)pFSMLayer->GetFSMAgent();
	}

	return pAgent->IsDirectMode();
}

RwBool Logic_RandomMove( CNtlSobActor* pActor, RwReal fDistance ) 
{
	if(!pActor || fDistance == 0.0f)
		return FALSE;

	RwV3d vDest = ZeroAxis;
	vDest.x = NtlRandomNumber(-1.0f, 1.0f);
	vDest.z = NtlRandomNumber(-1.0f, 1.0f);
	RwV3dNormalize(&vDest, &vDest);
	vDest = pActor->GetPosition() + vDest * fDistance;

	API_GetSLPacketGenerator()->SendMouseMove(pActor->GetPosition(), vDest, pActor->GetServerSyncAvatarType());  
	CNtlSLEventGenerator::SobDestMove(pActor->GetSerialID(), NTL_MOVE_FLAG_RUN, pActor->GetPosition(), vDest, 0);

	return TRUE;
}

RwBool Logic_CanTranslateIdleState( CNtlSobActor* pActor ) 
{
	if(!pActor)
		return FALSE;

	RwUInt32 nStateId = Logic_GetActorStateId(pActor);

	if(nStateId == NTL_FSMSID_ONBUS)            // 버스인 경우에는 안됨
		return FALSE;

	if(nStateId == NTL_FSMSID_ONVEHICLE)        // 탈것인 경우에는 안됨
		return FALSE;

    if(nStateId == NTL_FSMSID_KNOCKDOWN)        // 넉다운일 경우에는 안됨
        return FALSE;

	return TRUE;
}

RwBool Logic_CanCharging( CNtlSobActor* pActor ) 
{
	if( !pActor )
		return FALSE;

	if(pActor->GetClassID() != SLCLASS_AVATAR &&
		pActor->GetClassID() != SLCLASS_PLAYER)
		return FALSE;

	RwUInt32 nStateId = Logic_GetActorStateId(pActor);

	if(Logic_IsCondition(pActor,CHARCOND_FLAG_CHARGING_BLOCKED))        ///< 기모으기 불가 컨디션
		return FALSE;

	if(nStateId == NTL_FSMSID_ONBUS)            // 버스인 경우에는 안됨
		return FALSE;

	if(nStateId == NTL_FSMSID_ONVEHICLE)        // 탈것인 경우에는 안됨
		return FALSE;

	return TRUE;
}

RwBool Logic_IsBehind( CNtlSob* pSob, RwV3d& vTargetPos ) 
{
	if(!pSob)
		return FALSE;

	RwV3d vTargetDir = vTargetPos - pSob->GetPosition();
	RwV3dNormalize(&vTargetDir, &vTargetDir);

	if(RwV3dDotProduct(&pSob->GetDirection(), &vTargetDir) <= 0.0f)
		return TRUE;

	return FALSE;
}

RwUInt32 Logic_GetIdleAnimationID( CNtlSob* pSobObj ) 
{
	if( !pSobObj )
		return NML_IDLE_LOOP;

	sITEM_TBLDAT *pItemTblData = Logic_GetEquipedWeaponItemTableData(pSobObj);
	if(pItemTblData && Logic_IsEquipedStaffWeapon(pItemTblData))
		return NML_STAFF_IDLE_LOOP;
	else
		return NML_IDLE_LOOP;
}

RwBool Logic_IsCastingTargetSkill( TBLIDX tblidx ) 
{
	// TODO: 테이블의 Func를 확인해서 스킬을 체크한다.
	return Logic_GetSkillDataFromTable(tblidx)->wFunction_Bit_Flag & SKILL_FUNCTION_FLAG_SPECIAL_TARGET;
}

RwBool Logic_IsStrongCastSkill( TBLIDX tblidx )
{
	sSKILL_TBLDAT* pSkillData = Logic_GetSkillDataFromTable( tblidx );
	if( pSkillData )
		return pSkillData->wFunction_Bit_Flag & SKILL_FUNCTION_FLAG_STRONG_CASTING;

	return FALSE;
}

RwBool Logic_IsSkillUseAfterAttack(SERIAL_HANDLE hSkillSerial)
{
	CNtlSobSkill *pSobSkill = reinterpret_cast<CNtlSobSkill*>( GetNtlSobManager()->GetSobObject(hSkillSerial) );
	if(pSobSkill == NULL)
		return FALSE;

	CNtlSobSkillAttr *pSobSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSobSkill->GetSobAttr() );
	if(pSobSkillAttr == NULL)
		return FALSE;

	sSKILL_TBLDAT *pSkillTblData = pSobSkillAttr->GetSkillTbl();
	if(pSkillTblData == NULL)
		return FALSE;

	BYTE byActiveEffectType = pSkillTblData->bySkill_Active_Type;

	if(byActiveEffectType == SKILL_ACTIVE_TYPE_DD || byActiveEffectType == SKILL_ACTIVE_TYPE_DOT ||
		byActiveEffectType == SKILL_ACTIVE_TYPE_DC || byActiveEffectType == SKILL_ACTIVE_TYPE_CB)
		return TRUE;

	return FALSE;
}

RwBool Logic_CanTargetted( CNtlSobActor* pActor ) 
{
	if( !pActor )
		return FALSE;

	if(Logic_IsCondition(pActor, CHARCOND_FLAG_CANT_BE_TARGETTED))
		return FALSE;

	if(pActor->GetFlags() & SLFLAG_NOT_TARGETED)
		return FALSE;

	return TRUE;
}


void Logic_ShowHTBRPSelectGui(CNtlSobActor* pActor, SERIAL_HANDLE hTargetSerial)
{
	if( !pActor )
		return;

	if(pActor->GetClassID() != SLCLASS_AVATAR)
		return;

	CNtlSobActor* pTarActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(hTargetSerial) );
	if(pTarActor == NULL)
		return;

	if(pTarActor->GetClassID() != SLCLASS_PLAYER)
		return;

	CNtlSLEventGenerator::HTBRPStock();
}

void Logic_ShowSandBagRPSelectGUI(CNtlSobActor* pActor)
{
	if( !pActor )
		return;

	if(pActor->GetClassID() != SLCLASS_AVATAR)
		return;

	CNtlSLEventGenerator::HTBRPStock();
}

void Logic_SetActorAnimation( RwUInt32 uiSerialID, RwUInt32 uiAnimID, RwBool bLoop) 
{
	CNtlSobActor* pActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(uiSerialID);
	if(pActor)
	{
		pActor->GetSobProxy()->SetBaseAnimation(uiAnimID, bLoop);
	}
}

RwBool Logic_IsMovePatternApply(CNtlSobActor* pActor)
{
	if( !pActor )
		return FALSE;

	if(pActor->GetClassID() == SLCLASS_MONSTER)
		return TRUE;

	return FALSE;
}

void Logic_GetMovePatternSync(RwUInt32 uiSerialID, RwUInt8 byPatternId)
{
	CNtlSobActor* pActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(uiSerialID);
	if(pActor == NULL)
		return;

	if(pActor->GetClassID() == SLCLASS_MONSTER)
	{
		CNtlSobMonsterAttr *pSobMobAttr = reinterpret_cast<CNtlSobMonsterAttr*>( pActor->GetSobAttr() );
		pSobMobAttr->SetMovePatternId(byPatternId);
	}
}

//////////////////////////////////////////////////////////////////////////
// Upgrade Item 수치
RwUInt16	Logic_GetFinalItemValue( RwUInt16 wBaseValue, RwUInt8 byGrade, TBLIDX upgradetblidx )
{
	sITEM_UPGRADE_TBLDAT* pUpgradeData = reinterpret_cast<sITEM_UPGRADE_TBLDAT*>( API_GetTableContainer()->GetItemUpgradeTable()->FindData( upgradetblidx ) );
	RwUInt32 wFinalValue = wBaseValue;

	if( !pUpgradeData )
		return wBaseValue;

	switch( byGrade )
	{
	case 1: wFinalValue += pUpgradeData->wUp1; break;
	case 2: wFinalValue += pUpgradeData->wUp2; break;
	case 3: wFinalValue += pUpgradeData->wUp3; break;
	case 4: wFinalValue += pUpgradeData->wUp4; break;
	case 5: wFinalValue += pUpgradeData->wUp5; break;
	case 6: wFinalValue += pUpgradeData->wUp6; break;
	case 7: wFinalValue += pUpgradeData->wUp7; break;
	case 8: wFinalValue += pUpgradeData->wUp8; break;
	case 9: wFinalValue += pUpgradeData->wUp9; break;
	case 10: wFinalValue += pUpgradeData->wUp10; break;
	case 11: wFinalValue += pUpgradeData->wUp11; break;
	case 12: wFinalValue += pUpgradeData->wUp12; break;
	case 13: wFinalValue += pUpgradeData->wUp13; break;
	case 14: wFinalValue += pUpgradeData->wUp14; break;
	case 15: wFinalValue += pUpgradeData->wUp15; break;
	}

	return (RwUInt16)wFinalValue;
}

void Logic_SetNetPy(DWORD dwNetPy )
{
    CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();	
    if(!pSobAvatar)
        return;

	CNtlSobAvatarAttr* pAvatarAttr = (CNtlSobAvatarAttr*)pSobAvatar->GetSobAttr();
	if(!pAvatarAttr)
		return;

	pAvatarAttr->m_dwNetPy = dwNetPy;

	CNtlSLEventGenerator::SobUpdate(pSobAvatar->GetSerialID(), EVENT_AIUT_NETPY, 0);	
}

DWORD Logic_GetNetPy() 
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();	
    if(!pSobAvatar)
        return NTL_INVALID_VALUE;

	CNtlSobAvatarAttr* pAvatarAttr = (CNtlSobAvatarAttr*)pSobAvatar->GetSobAttr();
	if(pAvatarAttr)
		return pAvatarAttr->m_dwNetPy;	

	return NTL_INVALID_VALUE;
}

void Logic_SetTargetMarkingID( RwUInt32 uiSerialId ) 
{
	g_uiTargetMarkingId = uiSerialId;
}

RwUInt32 Logic_GetTargetMarkingID() 
{
	return g_uiTargetMarkingId;
}

RwUInt8 Logic_GetSobPlayerDogiColor(RwUInt32 hSobPlayerHandle)
{
	CNtlSob* pSob = GetNtlSobManager()->GetSobObject(hSobPlayerHandle);
	if( !pSob )
		return INVALID_BYTE;

	if( pSob->GetClassID() != SLCLASS_AVATAR &&
		pSob->GetClassID() != SLCLASS_PLAYER )
		return INVALID_BYTE;

	CNtlSobPlayer* pSobPlayer = reinterpret_cast<CNtlSobPlayer*>( pSob );

	return pSobPlayer->GetRealDogiColorIndex();
}

RwUInt8 Logic_GetDogiItemColor(CNtlSobPlayer* pSobPlayer, RwBool bUICharacter /* = FALSE */)
{
	if ( NULL == pSobPlayer )
	{
		DBO_FAIL( "Not exist sob player" );
		return INVALID_BYTE;
	}

	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( pSobPlayer->GetSobProxy() );

	if ( NULL == pCharProxy )
	{
		DBO_FAIL( "Not exist player proxy of handle : " << pSobPlayer->GetSerialID() );
		return INVALID_BYTE;
	}

	SEquipItem* pEquipItem = NULL;

	if ( bUICharacter )
	{
		CNtlSobCharProxy::SUICharacter* pUICharacter = pCharProxy->GetUIDogiCharacter();

		if ( NULL == pUICharacter || NULL == pUICharacter->pUIEquipProxy )
		{
			DBO_FAIL( "Not exist UI Character" );
			return INVALID_BYTE;
		}

		pEquipItem = pUICharacter->pUIEquipProxy->GetEquipItem( EQUIP_SLOT_TYPE_COSTUME );
	}
	else
	{
		pEquipItem = pCharProxy->GetEquipItem( EQUIP_SLOT_TYPE_COSTUME );
	}

	if ( NULL == pEquipItem || NULL == pEquipItem->pPLItem )
	{
		return INVALID_BYTE;
	}

	RwRGBA* pRGBA = pEquipItem->pPLItem->GetDogiEditedColor();

	const COLORREF colorREF = RGB( pRGBA->red, pRGBA->green, pRGBA->blue );

	return GetNtlPLPalette()->GetIndex( PALETTETYPE_DOGI, &colorREF );
}

RwBool Logic_SetDogiItemColor(CNtlSobPlayer* pSobPlayer, RwUInt8 byColorIndex,
							  RwBool bUICharacter /* = FALSE */, RwBool bForce /*= FALSE */)
{
	if ( NULL == pSobPlayer )
	{
		DBO_FAIL( "Not exist sob player" );
		return FALSE;
	}

	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( pSobPlayer->GetSobProxy() );

	if ( NULL == pCharProxy )
	{
		DBO_FAIL( "Not exist player proxy of handle : " << pSobPlayer->GetSerialID() );
		return FALSE;
	}

	SEquipItem* pEquipItem = NULL;

	if ( bUICharacter )
	{
		CNtlSobCharProxy::SUICharacter* pUICharacter = pCharProxy->GetUIDogiCharacter();

		if ( NULL == pUICharacter || NULL == pUICharacter->pUIEquipProxy )
		{
			DBO_FAIL( "Not exist UI Character" );
			return FALSE;
		}

		if( !bForce && FALSE == pUICharacter->pUIEquipProxy->IsResourceLoadComplete() )
			return FALSE;


		pEquipItem = pUICharacter->pUIEquipProxy->GetEquipItem( EQUIP_SLOT_TYPE_COSTUME );
	}
	else
	{
		CNtlSobCharEquipProxy* pCharEquipProxy = pCharProxy->GetEuqipProxy();

		if( !bForce && FALSE == pCharEquipProxy->IsResourceLoadComplete() )
			return FALSE;


		pEquipItem = pCharEquipProxy->GetEquipItem( EQUIP_SLOT_TYPE_COSTUME );
	}


	if ( NULL == pEquipItem || NULL == pEquipItem->pPLItem )
		return FALSE;


	COLORREF colorREF = GetNtlPLPalette()->GetColor( PALETTETYPE_DOGI, byColorIndex );
	RwRGBA color;

	color.red	= GetRValue(colorREF);
	color.green	= GetGValue(colorREF);
	color.blue	= GetBValue(colorREF);
	color.alpha	= 255;

	pEquipItem->pPLItem->SetDogiEditedColor(color);

	return TRUE;
}

RwBool Logic_GetEmblemFactor(sEmblemFactor* pOutEmblemFactor, CNtlSobActor* pSobActor)
{
	if( !pOutEmblemFactor || !pSobActor )
		return FALSE;

	CNtlSobCharProxy* pCharProxy = reinterpret_cast<CNtlSobCharProxy*>( pSobActor->GetSobProxy() );
	if( !pCharProxy )
		return FALSE;

	CNtlSobCharDecorationProxy* pCharDecorationProxy = pCharProxy->GetDecorationProxy();
	if( !pCharDecorationProxy )
		return FALSE;

	const sEmblemFactor* pEmblemFactor	= pCharDecorationProxy->GetEmblemFactor();
	if( !pEmblemFactor )
		return FALSE;

	pOutEmblemFactor->byTypeA			= pEmblemFactor->byTypeA;
	pOutEmblemFactor->byTypeB			= pEmblemFactor->byTypeB;
	pOutEmblemFactor->byTypeC			= pEmblemFactor->byTypeC;
	pOutEmblemFactor->byTypeAColor		= pEmblemFactor->byTypeAColor;
	pOutEmblemFactor->byTypeBColor		= pEmblemFactor->byTypeBColor;
	pOutEmblemFactor->byTypeCColor		= pEmblemFactor->byTypeCColor;

	return TRUE;
}

RwBool Logic_IsSavableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex)
{
	if( uiIndex >= uiMaxIndex )
		return FALSE;

	return TRUE;
}

RwBool Logic_IsSavableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex, RwUInt32 uiInvalidIndex)
{
	if( uiIndex >= uiMaxIndex )
	{
		if( uiIndex != uiInvalidIndex )
			return FALSE;
	}	

	return TRUE;
}

RwBool Logic_IsUsableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex)
{
	if( uiIndex >= uiMaxIndex )
		return FALSE;

	return TRUE;
}

RwBool Logic_IsUsableIndex(RwUInt32 uiIndex, RwUInt32 uiMaxIndex, RwUInt32 uiInvalidIndex)
{
	if( uiIndex >= uiMaxIndex )
		return FALSE;

	if( uiIndex == uiInvalidIndex )
		return FALSE;

	return TRUE;
}

RwBool Logic_IsCollisionLine( RwV3d& vStartPos, RwV3d& vEndPos, OUT CNtlPLEntity* pColEntity /*= NULL*/ ) 
{
    // 오브젝트에 대해서만 충돌을 검사한다.
    RwLine colRay;
    colRay.start = vStartPos;
    colRay.end = vEndPos;    

    SWorldPickInfo pickInfo;    
    pickInfo.fMinDist = 25.0f;

    if(!pColEntity)
    {
        return Collision_WorldIntersectionObjectForTarget(colRay, &pickInfo);
    }
    else
    {
        RwBool bReturn = Collision_WorldIntersectionObjectForTarget(colRay, &pickInfo);
        pColEntity = pickInfo.pPLEntity;
        return bReturn;
    }
}

void Logic_VehicleCollisionBriefAssign( CNtlSobVehicle* pActor, const RwV3d* pDestPos, SCharCollisionBrief* pCollBrief )
{
	if( !pActor || !pDestPos || !pCollBrief )
		return;

	CNtlSobProxy* pSobProxy = pActor->GetSobProxy();

	pCollBrief->fCharWidth			= pSobProxy->GetPLEntityWidth();
	pCollBrief->fCharHeight			= pSobProxy->GetPLEntityHeight();
	pCollBrief->fCharDepth			= pSobProxy->GetPLEntityDepth();

	pCollBrief->fCharHalfWidth		= pCollBrief->fCharWidth * 0.5f;
	pCollBrief->fCharHalfHeight		= pCollBrief->fCharHeight * 0.5f;
	pCollBrief->fCharHalfDepth		= pCollBrief->fCharDepth * 0.5f;

	pCollBrief->vCharCurrPos = pActor->GetPosition();
	RwV3dAssign( &pCollBrief->vCharDestPos, pDestPos );

	pCollBrief->bCollRecursive		= FALSE;
	pCollBrief->bCollMoveImPossible = FALSE;

	pCollBrief->pCollPLEntity		= NULL;
}

void Logic_GetVehicleCollisionAxis( SCharCollisionBrief* pCollBrief )
{
	if( !pCollBrief )
		return;

	RwV3dSub( &pCollBrief->vCharFront, &pCollBrief->vCharDestPos, &pCollBrief->vCharCurrPos );
	pCollBrief->vCharFront.y = 0.0f;
	pCollBrief->fInterval = RwV3dLength( &pCollBrief->vCharFront ) + pCollBrief->fCharHalfDepth + VEHICLE_COLLISION_LINE_FRONT_OFFSET; 

	RwV3dNormalize( &pCollBrief->vCharFront, &pCollBrief->vCharFront );
	RwV3dCrossProduct( &pCollBrief->vCharRight, &pCollBrief->vCharFront, &CNtlPLGlobal::m_vYAxisV3 );
	RwV3dNormalize( &pCollBrief->vCharRight, &pCollBrief->vCharRight );
}

void Logic_GetVehicleCollisionBBox( RwBBox* pBox, SCharCollisionBrief* pCollBrief )
{
	if( !pBox || !pCollBrief )
		return;

	RwV3d vBoxVertex[2];

	RwV3d* pPos = &pCollBrief->vCharCurrPos;
	RwV3d* pFront = &pCollBrief->vCharFront;
	RwV3d* pRight = &pCollBrief->vCharRight;

	vBoxVertex[0].x = pPos->x + (pRight->x * pCollBrief->fCharHalfWidth);
	vBoxVertex[0].y = pPos->y;
	vBoxVertex[0].z = pPos->z + (pFront->z * pCollBrief->fCharHalfDepth);
	vBoxVertex[1].x = pPos->x - (pRight->x * pCollBrief->fCharHalfWidth);
	vBoxVertex[1].y = pPos->y + pCollBrief->fCharHeight;
	vBoxVertex[1].z = pPos->z - (pFront->z * pCollBrief->fCharHalfDepth);

	RwBBoxCalculate( pBox, vBoxVertex, 2 );
}

void Logic_GetVehicleCollisionLine( RwLine* pLine, SCharCollisionBrief* pCollBrief )
{
	if( !pLine || !pCollBrief )
		return;

	RwV3d* pPos = &pCollBrief->vCharCurrPos;
	RwV3d* pFront = &pCollBrief->vCharFront;
	RwV3d* pRight = &pCollBrief->vCharRight;

	RwReal fLineY = pCollBrief->fCharHeight * VEHICLE_COLLISION_LINE_HEIGHT_OFFSET;
	if ( fLineY < VEHICLE_COLLISION_LINE_LIMIT_HEIGHT )
	{
		fLineY = VEHICLE_COLLISION_LINE_LIMIT_HEIGHT;
	}

	fLineY += pPos->y;
	RwReal fRightX = pRight->x * pCollBrief->fCharHalfWidth;
	RwReal fRightZ = pRight->z * pCollBrief->fCharHalfWidth;

	// 중앙
	pLine[0].start.x = pPos->x;
	pLine[0].start.y = fLineY;
	pLine[0].start.z = pPos->z;

	// 왼쪽
	pLine[1].start.x = pPos->x - fRightX;
	pLine[1].start.y = fLineY;
	pLine[1].start.z = pPos->z - fRightZ;

	// 오른쪽
	pLine[2].start.x = pPos->x + fRightX;
	pLine[2].start.y = fLineY;
	pLine[2].start.z = pPos->z + fRightZ;

	// Y축 방향 체크 라인
	pLine[3].start.x = pPos->x;
	pLine[3].start.y = fLineY;
	pLine[3].start.z = pPos->z;
	pLine[3].end.x = pLine[3].start.x;
	pLine[3].end.y = pLine[3].start.y + (pCollBrief->fCharHeight * (1 - VEHICLE_COLLISION_LINE_HEIGHT_OFFSET));
	pLine[3].end.z = pLine[3].start.z;

	for ( RwInt32 i = 0; i < 3; ++i )
	{
		pLine[i].end.x = pLine[i].start.x + pFront->x*pCollBrief->fInterval;
		pLine[i].end.y = pLine[i].start.y;
		pLine[i].end.z = pLine[i].start.z + pFront->z*pCollBrief->fInterval;
	}    
}

RwBool Logic_IsVehicleCollisionMove( CNtlSobVehicle* pActor, RwBool bCollision, SCharCollisionBrief* pCollBrief, RwReal fSpeed, RwBool bJump )
{
	if ( !pActor || !pCollBrief )
		return FALSE;

	if ( !GetSceneManager()->GetAvailablePos( pCollBrief->vCharDestPos ) )
	{
		return FALSE;
	}

	if ( !g_bColliSlantCheck )
	{
		return TRUE;
	}

	RwV3d vCurrPos = pCollBrief->vCharCurrPos;
	RwV3d vDestPos = pCollBrief->vCharDestPos;

	if ( !GetNtlWorldConcept()->IsMoveableArea( pActor, &vCurrPos, &vDestPos ) )
	{
		return FALSE;
	}

	DWORD dwAttr = GetSceneManager()->GetWorldNormalAttribute( vDestPos );
	if ( (dwAttr & DBO_WORLD_ATTR_GROUP_FLAG_FORBID_ALL_MOVE) )
	{
		return FALSE;
	}

	if ( bCollision )
	{
		if ( pCollBrief->pCollPLEntity == NULL )
		{
			return TRUE;
		}

		if ( pCollBrief->pCollPLEntity->GetClassType() != PLENTITY_WORLD )
		{
			return TRUE;
		}
	}

	SWorldHeightStuff sHStuff;

	if ( bJump )
	{
		Logic_GetWorldHeight( &vCurrPos, sHStuff );
		vCurrPos.y = sHStuff.fFinialHeight + pActor->GetRideOnHeight() / 100.f;
	}

	RwV3d vNormal;
	bCollision = Logic_GetWorldHeight( &vDestPos, sHStuff, &vNormal );

	if ( !bCollision )
	{
		return TRUE;
	}

	if ( CNtlPLGlobal::m_pWHEntity )
	{
		return TRUE;
	}

	if ( sHStuff.bWaterAttr )
	{
		return TRUE;
	}

	vDestPos.y = sHStuff.fWorldHeight + pActor->GetRideOnHeight() / 100.f;

	RwV3d vSub;
	RwV3dSub( &vSub, &vDestPos, &vCurrPos );

	if ( vDestPos.y < vCurrPos.y )
	{
		return TRUE;
	}

	RwReal fLen = RwV3dLength( &vSub );

	if ( fLen >= fSpeed / 1.5f )
	{
		return FALSE;
	}

	RwV3d vRight, vUp;
	RwV3dCrossProduct( &vRight, &vNormal, &CNtlPLGlobal::m_vYAxisV3 );
	RwV3dNormalize( &vRight, &vRight );
	RwV3dCrossProduct( &vUp, &vNormal, &vRight );
	RwV3dNormalize( &vUp, &vUp );
	RwV3dScale( &vUp, &vUp, -1.0f );

	if ( vUp.y >= g_fCharColliAngle )
	{
		RwV3d vStartPos = pCollBrief->vCharCurrPos;

		RwV3d vEndPos = pCollBrief->vCharDestPos;
		vEndPos.y = vStartPos.y;

		RwV3d vDist = vEndPos - vStartPos;

		fLen = RwV3dLength( &vDist );
		if ( fLen < 0.01f )
		{
			return FALSE;
		}

		RwV3dNormalize( &vDist, &vDist );

		// Ray의 길이를 좀더 길게 잡아준다
		vDist *= fLen + 1.5f;

		vEndPos = vStartPos + vDist;

		SWorldPickInfo sPickingInfo;

		if ( !GetSceneManager()->PickTerrian( vStartPos, vEndPos, sPickingInfo ) )
		{
			return TRUE;
		}

		return FALSE;
	}

	return TRUE;
}

void Logic_VehicleCollisionPositinAdjust( SCharCollisionBrief* pCollBrief )
{
	if( !pCollBrief )
		return;

	RwV3d vFrom, vAt, vBack;

	vFrom.x = pCollBrief->vCharCurrPos.x;
	vFrom.y = pCollBrief->vCharCurrPos.y + pCollBrief->fCharHeight * VEHICLE_COLLISION_LINE_HEIGHT_OFFSET;
	vFrom.z = pCollBrief->vCharCurrPos.z;

	vAt.x = vFrom.x + pCollBrief->vCharFront.x * 100.0f;
	vAt.y = vFrom.y;
	vAt.z = vFrom.z + pCollBrief->vCharFront.z * 100.0f;

	RwV3dScale( &vBack, &pCollBrief->vCharFront, -1.0f );

	CNtlPlane plane;
	plane.PlaneFromPointNormal( &pCollBrief->vCollPos, &vBack );

	pCollBrief->vAdjustCollPos = plane.SplitPoint( &vFrom, &vAt );

	RwV3d vDelta;
	RwV3dSub( &vDelta, &pCollBrief->vAdjustCollPos, &pCollBrief->vCharCurrPos );
	RwReal fLen = RwV3dLength( &vDelta );
	RwReal fDeltaDist = fLen - pCollBrief->fCharHalfDepth - VEHICLE_COLLISION_LINE_FRONT_OFFSET;

	if ( fDeltaDist > 0 )
	{
		pCollBrief->vAdjustCollPos.x = pCollBrief->vCharCurrPos.x + pCollBrief->vCharFront.x * fDeltaDist;
		pCollBrief->vAdjustCollPos.z = pCollBrief->vCharCurrPos.z + pCollBrief->vCharFront.z * fDeltaDist;
	}
	else
	{
		pCollBrief->vAdjustCollPos.x = pCollBrief->vCharCurrPos.x;
		pCollBrief->vAdjustCollPos.z = pCollBrief->vCharCurrPos.z;
	}
};

RwBool Logic_IsVehicleCollisionNewPosition( SCharCollisionBrief* pCollBrief )
{
	if( !pCollBrief )
		return FALSE;

	RwV3d vDelta;
	RwV3dSub( &vDelta, &pCollBrief->vAdjustCollPos, &pCollBrief->vCharCurrPos );
	vDelta.y = 0.0f;

	RwReal fLen = RwV3dLength( &vDelta );

	if ( fLen <= pCollBrief->fCharHalfDepth + VEHICLE_COLLISION_LINE_FRONT_OFFSET )
	{
		return TRUE;
	}

	return FALSE;
}

void Logic_GetVehicleCollisionNewPosition( SCharCollisionBrief *pCollBrief, RwReal fSpeed, RwReal fElapsed )
{
	if( !pCollBrief )
		return;

	RwV3d vCollNormal = pCollBrief->vCollNormal;

	vCollNormal.y = 0.0f;
	RwV3dNormalize( &vCollNormal, &vCollNormal );

	RwV3d vCollLeft, vCollRight;
	RwV3dCrossProduct( &vCollRight, &vCollNormal, &CNtlPLGlobal::m_vYAxisV3 );
	RwV3dNormalize( &vCollRight, &vCollRight );
	RwV3dScale( &vCollLeft, &vCollRight, -1.0f );

	RwV3d v0, v1;
	RwV3dAdd( &v0, &vCollRight, &vCollNormal );
	RwV3dAdd( &v1, &vCollLeft, &vCollNormal );

	RwV3dNormalize( &v0, &v0 );
	RwV3dNormalize( &v1, &v1 );

	RwV3d vBack;
	RwV3dScale( &vBack, &pCollBrief->vCharFront, -1.0f );

	RwReal fValue0 = fabs( RwV3dDotProduct( &v0, &vBack ) );
	RwReal fValue1 = fabs( RwV3dDotProduct( &v1, &vBack ) );
	RwReal fIncDecSpeed = 1.0f - fabs( RwV3dDotProduct( &vCollNormal, &vBack ) ) + 0.2f;
	if ( fIncDecSpeed > 1.0f )
		fIncDecSpeed = 1.0f;

	if ( fElapsed > 1.0f )
		fElapsed = 1.0f;

	fIncDecSpeed = fSpeed * fIncDecSpeed * fElapsed;

	if ( fValue0 < fValue1 )
	{
		pCollBrief->vCharDestPos.x = pCollBrief->vCharCurrPos.x + vCollRight.x * fIncDecSpeed;
		pCollBrief->vCharDestPos.z = pCollBrief->vCharCurrPos.z + vCollRight.z * fIncDecSpeed;
	}
	else
	{
		pCollBrief->vCharDestPos.x = pCollBrief->vCharCurrPos.x + vCollLeft.x * fIncDecSpeed;
		pCollBrief->vCharDestPos.z = pCollBrief->vCharCurrPos.z + vCollLeft.z * fIncDecSpeed;
	}
}

RwBool Logic_GetVehicleCollisionAtomicNormal( RwLine* pLine, SWorldCharIntersectionInfo& sCharInfo, SCharCollisionBrief* pCollBrief, RwBool bJump )
{
	if( !pLine || !pCollBrief )
		return FALSE;

	RwBool bCollision = FALSE;
	RwReal fCollDist = RwRealMAXVAL;
	SWorldCharIntersectionData sCharData;
	pCollBrief->bCollJump = FALSE;

	RwBool bIndoorWorld = GetSceneManager()->GetActiveWorldType() == AW_RWWORLD ? TRUE : FALSE;

	// 점프가 아니면 마지막 라인(y축 체크)은 체크하지 않는다.
	RwInt32 nLineCnt = bJump ? VEHICLE_COLLISION_LINE_COUNT : VEHICLE_COLLISION_LINE_COUNT - 1;

	for ( RwInt32 iLineIdx = 0; iLineIdx < nLineCnt; ++iLineIdx )
	{
		// Indoor일 경우.
		if ( bIndoorWorld )
		{
			if ( Collision_WorldIntersectionWorldSector( pLine[iLineIdx], sCharData ) )
			{
				bCollision = TRUE;

				if ( sCharData.fMinDist < fCollDist )
				{
					RwV3dAssign( &pCollBrief->vCollPos, &sCharData.vCollPos );
					RwV3dAssign( &pCollBrief->vCollNormal, &sCharData.vNormal );
					pCollBrief->fCollDist = sCharData.fMinDist;
					fCollDist = sCharData.fMinDist;

					pCollBrief->pCollPLEntity = NULL;

					// 점프로 충돌한건지 체크한다.
					if ( iLineIdx == VEHICLE_COLLISION_LINE_JUMP_INDEX )
					{
						pCollBrief->bCollJump = TRUE;
					}
				}
			}
		}

		for ( RwInt32 i = 0; i < sCharInfo.byAtomicCount; ++i )
		{
			if ( Collision_WorldIntersectionCharacterData( pLine[iLineIdx], sCharInfo.pAtomic[i], sCharData ) )
			{
				bCollision = TRUE;

				if ( sCharData.fMinDist < fCollDist )
				{
					RwV3dAssign( &pCollBrief->vCollPos, &sCharData.vCollPos );
					RwV3dAssign( &pCollBrief->vCollNormal, &sCharData.vNormal );
					pCollBrief->fCollDist = sCharData.fMinDist;
					fCollDist = sCharData.fMinDist;

					pCollBrief->pCollPLEntity = reinterpret_cast<CNtlPLEntity*>( RpNtlAtomicGetData( sCharInfo.pAtomic[i] ) );

					// 점프로 충돌한건지 체크한다.
					if ( iLineIdx == CHAR_COLLISION_LINE_JUMP_INDEX )
					{
						pCollBrief->bCollJump = TRUE;
					}
				}
			}
		}
	}

	return bCollision;
}

RwUInt8 Logic_VehicleCollisionBegin( CNtlSobVehicle* pActor, RwReal fSpeed, RwReal fElapsed, RwBool bJump )
{
	if( !pActor )
		return FALSE;

	Logic_GetVehicleCollisionAxis( &g_sColliBrief );

	// 충돌할 bounding box 영역을 구한다.
	RwBBox box;
	Logic_GetVehicleCollisionBBox( &box, &g_sColliBrief );

	// bounding box 충돌을 한다.
	RwUInt8 byColliResult = NTL_CHARACTER_COLLI_NONE;
	RwBool bBoxCollision = FALSE, bCollision = FALSE;
	SWorldCharIntersectionInfo sCharInfo;
	bBoxCollision = Collision_WorldIntersectionCharacter( box, sCharInfo );

	// 충돌한 ray를 구한다.
	RwLine Line[CHAR_COLLISION_LINE_COUNT];
	Logic_GetVehicleCollisionLine( Line, &g_sColliBrief );

	// 충돌한 atomic의 가까운 polygon을 찾아 normal을 구한다.
	RwBool bIndoorWorld = GetSceneManager()->GetActiveWorldType() == AW_RWWORLD ? TRUE : FALSE;

	if ( bBoxCollision || bIndoorWorld )
	{
		bCollision = Logic_GetVehicleCollisionAtomicNormal( Line, sCharInfo, &g_sColliBrief, bJump );
	}

	if ( bCollision )
	{
		byColliResult = g_sColliBrief.bCollJump ? NTL_CHARACTER_COLLI_OBJECT_JUMP : NTL_CHARACTER_COLLI_OBJECT;

		if ( pActor->GetClassID() == SLCLASS_AVATAR && g_sColliBrief.pCollPLEntity )
		{
			RwUInt32 uiSerialId = g_sColliBrief.pCollPLEntity->GetSerialID();
			CNtlSob *pColliSobObj = GetNtlSobManager()->GetSobObject( uiSerialId );
			if ( pColliSobObj && pColliSobObj->GetClassID() == SLCLASS_TRIGGER_OBJECT )
			{
				CNtlSLEventGenerator::TSColTriggerObject( uiSerialId );
			}
		}

		if ( Logic_IsVehicleCollisionMove( pActor, TRUE, &g_sColliBrief, fSpeed, bJump ) )
		{
			Logic_VehicleCollisionPositinAdjust( &g_sColliBrief );

			if ( Logic_IsVehicleCollisionNewPosition( &g_sColliBrief ) )
			{
				Logic_GetVehicleCollisionNewPosition( &g_sColliBrief, fSpeed, fElapsed );
				g_sColliBrief.vNewPos = g_sColliBrief.vCharDestPos;
				g_sColliBrief.bCollRecursive = TRUE;
			}
			else
			{
				RwV3dAssign( &g_sColliBrief.vNewPos, &g_sColliBrief.vAdjustCollPos );
			}

			g_sColliBrief.vNewPos.y = g_sColliBrief.vCharDestPos.y;
		}
		else
		{
			RwV3dAssign( &g_sColliBrief.vNewPos, &g_sColliBrief.vCharCurrPos );

			g_sColliBrief.vNewPos.y = g_sColliBrief.vCharDestPos.y;

			g_sColliBrief.bCollMoveImPossible = TRUE;
		}
	}
	else
	{
		if ( !Logic_IsVehicleCollisionMove( pActor, FALSE, &g_sColliBrief, fSpeed, bJump ) )
		{
			RwV3dAssign( &g_sColliBrief.vNewPos, &g_sColliBrief.vCharCurrPos );

			g_sColliBrief.bCollMoveImPossible = TRUE;
			bCollision = TRUE;

			byColliResult = NTL_CHARACTER_COLLI_WORLD_ATTR_TERRAIN_SLANT;
		}
	}

	return byColliResult;
}

RwBool Logic_VehicleCollisionRecusive( RwReal fSpeed, RwReal fElapsed, RwBool bJump )
{
	Logic_GetVehicleCollisionAxis( &g_sColliBrief );

	// 충돌할 bounding box 영역을 구한다.
	RwBBox box;
	Logic_GetVehicleCollisionBBox( &box, &g_sColliBrief );

	// bounding box 충돌을 한다.
	RwBool bCollision;
	SWorldCharIntersectionInfo sCharInfo;
	bCollision = Collision_WorldIntersectionCharacter( box, sCharInfo );

	// 충돌한 ray를 구한다.
	RwLine Line[CHAR_COLLISION_LINE_COUNT];
	Logic_GetVehicleCollisionLine( Line, &g_sColliBrief );

	// 충돌한 atomic의 가까운 polygon을 찾아 normal을 구한다.
	RwBool bIndoorWorld = GetSceneManager()->GetActiveWorldType() == AW_RWWORLD ? TRUE : FALSE;
	if ( bCollision || bIndoorWorld )
	{
		bCollision = Logic_GetVehicleCollisionAtomicNormal( Line, sCharInfo, &g_sColliBrief, bJump );
	}

	if ( bCollision )
	{
		Logic_VehicleCollisionPositinAdjust( &g_sColliBrief );

		if ( Logic_IsVehicleCollisionNewPosition( &g_sColliBrief ) )
		{
			g_sColliBrief.bCollMoveImPossible = TRUE;
		}

		RwV3dAssign( &g_sColliBrief.vNewPos, &g_sColliBrief.vAdjustCollPos );

		g_sColliBrief.vNewPos.y = g_sColliBrief.vCharDestPos.y;
	}

	return bCollision;
}

RwUInt8 Logic_VehicleCollisionEx( CNtlSobVehicle* pActor, const RwV3d* pDestPos, RwReal fSpeed, RwV3d& vNewPos, RwBool& bCollMoveImPossible, RwReal fElapsed, RwBool bJump /*= FALSE*/ )
{
	if ( !pActor || !pDestPos )
		return 0;

	Logic_VehicleCollisionBriefAssign( pActor, pDestPos, &g_sColliBrief );

	RwUInt8 byColliResult = Logic_VehicleCollisionBegin( pActor, fSpeed, fElapsed, bJump );

	if ( byColliResult == NTL_CHARACTER_COLLI_OBJECT && g_sColliBrief.bCollRecursive )
	{
		Logic_VehicleCollisionRecusive( fSpeed, fElapsed, bJump );

		byColliResult = g_sColliBrief.bCollJump ? NTL_CHARACTER_COLLI_OBJECT_JUMP : NTL_CHARACTER_COLLI_OBJECT;
	}

	if ( byColliResult != NTL_CHARACTER_COLLI_NONE )
	{
		bCollMoveImPossible = g_sColliBrief.bCollMoveImPossible;
		RwV3dAssign( &vNewPos, &g_sColliBrief.vNewPos );
	}

	return byColliResult;
}

RwBool Logic_isEnemyTargetOcclusion( SERIAL_HANDLE hTargetSerial ) 
{
    CNtlSobActor* pActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(hTargetSerial);
    if(!pActor)
        return FALSE;

    RwV3d vStartPos = GetNtlSLGlobal()->GetSobAvatar()->GetPosition();
    vStartPos.y += GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy()->GetPLEntityHeight() * 0.9f;

    RwV3d vEndPos = pActor->GetPosition();
    vEndPos.y += pActor->GetSobProxy()->GetPLEntityHeight() * 0.5f;

     if(Logic_IsCollisionLine(vStartPos, vEndPos))
     {
         CNtlSLEventGenerator::SysMsg(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), DST_SKILL_OCCLUSION_CHECK, FALSE);

         return TRUE;
     }

     return FALSE;
}

RwBool Logic_IsNPCCameraMode(void)
{
	if ( CNtlGameCameraManager::GetInstance()->FindController( CAMERA_CONTROL_NPC ) )
	{
		return TRUE;
	}

	return FALSE;
}

SOUND_HANDLE Logic_PlayGUISound(char* pcFileName)
{
	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
	tSoundParam.pcFileName		= pcFileName;

	GetSoundManager()->Play(&tSoundParam);

	return tSoundParam.hHandle;
}

void Logic_PlayItemPickUpSound(sITEM_TBLDAT* pITEM_TBLDAT)
{
	// 일단 소리가 하나이므로 null 검사하지 않고 바로 디폴트 사운드 출력
	//switch( pITEM_TBLDAT->byItem_Type )
	//{
	//default: break;
	//}

	Logic_PlayGUISound(GSD_SYSTEM_ICON_SELECT);
}

void Logic_PlayItemPutDownSound(sITEM_TBLDAT* pITEM_TBLDAT, RwUInt8 byDestPlace)
{
	if( !pITEM_TBLDAT )
	{
		Logic_PlayGUISound(GSD_SYSTEM_ICON_SELECT);
		return;
	}

	switch( byDestPlace )
	{
	case CONTAINER_TYPE_SCOUT:
		{
			// 스카우트에
			Logic_PlayGUISound(GSD_SYSTEM_PARTS_EQUIP);
			return;
		}
	case CONTAINER_TYPE_EQUIP:
		{
			// 캐릭터 스텟창
			Logic_PlayGUISound(GSD_SYSTEM_ITEM_EQUIP);
			return;
		}
	}
	/*
	// 아마도 소리의 종류가 하나 더 늘어놔야 될듯 함. 일반 가방에 등록되는 소리로...
	// 각종 가방
	CONTAINER_TYPE_BAG1
	CONTAINER_TYPE_BAG2
	CONTAINER_TYPE_BAG3
	CONTAINER_TYPE_BAG4
	CONTAINER_TYPE_BAG5
	// 오른쪽 하단 가방 등록하는 곳
	CONTAINER_TYPE_BAGSLOT
	*/
	// Defualt
	Logic_PlayGUISound(GSD_SYSTEM_ICON_SELECT);
}