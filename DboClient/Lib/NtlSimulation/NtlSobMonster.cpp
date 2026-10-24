#include "precomp_ntlsimulation.h"
#include "NtlSobMonster.h"

//shared
#include "MobTable.h"
#include "CommonConfigTable.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"


// presentation
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlPLPlayerName.h"
#include "NtlPLHelpers.h"

// simulation
#include "TableContainer.h"
#include "NtlSobManager.h"
#include "NtlSobAttrFactory.h"
#include "NtlFSMDef.h"
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSLEventFunc.h"
#include "NtlFSMCharActLayer.h"
#include "NtlBehaviorData.h"
#include "NtlSobCharProxy.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "NtlSLVisualDeclear.h"
#include "NtlSoundManager.h"


DEFINITION_MEMORY_POOL(CNtlSobMonster)

CNtlSobMonster::CNtlSobMonster()
{
	SetFlags(SLFLAG_ADD_UPDATE | SLFLAG_CAN_SKILLUSE | SLFLAG_CAN_GROUND_FLY | SLFLAG_LUA_INTERLOCKING | 
			SLFLAG_LUA_TRANSFORM_ENABLE | SLFLAG_TARGET_SELECT_GUI_UPDATE);

	SetActorFlags(SLFLAG_ACTOR_HAVE_LIFE | SLFLAG_ACTOR_HAVE_MOVEABLE | SLFLAG_ACTOR_HAVE_BATTLE);

	m_pScanRangeEffect = NULL;
}

CNtlSobMonster::~CNtlSobMonster()
{
}

RwBool CNtlSobMonster::Create(void)
{
	NTL_FUNCTION("CNtlSobMonster::Create");

	// proxy 설정
	m_pSobProxy = NTL_NEW CNtlSobCharProxy;
	m_pSobProxy->Create(0);
	m_pSobProxy->SetSobObj(this);

	// control data create
	m_pBeData = NTL_NEW CNtlBeCharData; 

	if(!CNtlSobFeelingActor::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name 설정.
	SetClassName(SLCLASS_NAME_MONSTER);
			
	NTL_RETURN(TRUE);
}

void CNtlSobMonster::Destroy(void)
{
	NTL_FUNCTION("CNtlSobMonster::Destroy");

	// behavior data
	NTL_DELETE(m_pBeData);

	CNtlSobFeelingActor::Destroy();

	NTL_RETURNVOID();
}


void CNtlSobMonster::Update(RwReal fElapsed)
{
	CNtlSobFeelingActor::Update(fElapsed);
}

CNtlFSMLayer* CNtlSobMonster::CreateFSM(void)
{
	CNtlFSMLayer *pFSMLayer = NTL_NEW CNtlFSMCharActLayer;
	pFSMLayer->SetActor(this);
	pFSMLayer->Create(SLCONTROLLER_SERVER); 
	
	return pFSMLayer;
}

void CNtlSobMonster::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobMonster::HandleEvents");

	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobMonsterCreate *pSobCreate = reinterpret_cast<SNtlEventSobMonsterCreate*>(pMsg.pData);

		// game attribute data setting.
		GetSobAttr()->HandleEvents(pMsg);

		// proxy setting
		GetSobProxy()->HandleEvents(pMsg);

		// 좌표와 방향 세팅.
		RwV3d vLoc, vDir;
		RwV3dAssignMacro(&vLoc, &pSobCreate->vLoc); 
		RwV3dAssignMacro(&vDir, &pSobCreate->vDir); 
		
		SWorldHeightStuff sHStuff;
		Logic_GetWorldHeight(this, &vLoc, sHStuff);
		vLoc.y = sHStuff.fFinialHeight;
		
		SetPosition(&vLoc);
		SetDirection(&vDir);

		CNtlSobMonsterAttr *pSobMobAttr = reinterpret_cast<CNtlSobMonsterAttr*>( GetSobAttr() );
		if(pSobMobAttr->GetFlyHeight() > 0.0f)
		{
			RwUInt32 uiFlags = GetFlags();
			SetFlags(uiFlags | SLFLAG_CAN_GROUND_FLY);
		}

		RwReal fScale = pSobMobAttr->GetScale();
		GetSobProxy()->SetScale(fScale);

		CreateScanRangeEffect(FALSE);
	}
	else if(pMsg.Id == g_EventSobFainting)
	{
		DeleteScanRangeEffect();
	}
    else if(pMsg.Id == g_EventBotCaution_Nfy)   // 플레이어를 발각했을때
    {
        OnEventBotCautionNfy_Handler(pMsg);
    }
    else if(pMsg.Id == g_EventBotHelpMe_Nfy)
    {
        OnEventBotHelpMeNfy_Handler(pMsg);
    }

	CNtlSobFeelingActor::HandleEvents(pMsg); 

	NTL_RETURNVOID();
}

void CNtlSobMonster::OnEventBotCautionNfy_Handler( RWS::CMsg& pMsg ) 
{
    // 이펙트 생성
    RwV3d vOffset;
    vOffset.x = vOffset.z = 0.0f;
    vOffset.y = GetSobProxy()->GetPLEntityHeight() + 1.0f;
    GetSobProxy()->CreatePLChildEffect(NTL_VID_MARK_CAUTION, vOffset);

    // 사운드 플레이
    RwV3d vPos = GetPosition();        
	sNtlSoundPlayParameta tSoundParam;
	tSoundParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
	tSoundParam.pcFileName		= SOUND_CAUTION;
	tSoundParam.fXPos			= vPos.x;
	tSoundParam.fYPos			= vPos.y;
	tSoundParam.fZPos			= vPos.z;

	GetSoundManager()->Play(&tSoundParam);

	// the scan range turns into its alert look
	if (m_pScanRangeEffect)
		CreateScanRangeEffect(TRUE);
}

// tw 0x525B40: a mob whose table row names a MOB_SCAN_RANGE_SHOW_EFFECT common config row
// shows value 0 as a ring around it (value 8 once it has spotted someone). The ring covers
// value 9 meters, or the mob scan range when empty, scaled as 2.2 * range + 2.
void CNtlSobMonster::CreateScanRangeEffect(RwBool bAlert)
{
	DeleteScanRangeEffect();

	CNtlSobMonsterAttr* pSobMobAttr = reinterpret_cast<CNtlSobMonsterAttr*>(GetSobAttr());
	sMOB_TBLDAT* pMobTbl = pSobMobAttr->GetMobTbl();
	if (pMobTbl == NULL || pMobTbl->scanRangeConfigTblidx == INVALID_TBLIDX)
		return;

	sCOMMONCONFIG_TBLDAT* pConfig = reinterpret_cast<sCOMMONCONFIG_TBLDAT*>(API_GetTableContainer()->GetCommonConfigTable()->FindData(pMobTbl->scanRangeConfigTblidx));
	if (pConfig == NULL || pConfig->wstrName != L"MOB_SCAN_RANGE_SHOW_EFFECT")
		return;

	const std::wstring& wstrEffect = (bAlert && !pConfig->wstrValue[8].empty()) ? pConfig->wstrValue[8] : pConfig->wstrValue[0];
	if (wstrEffect.empty())
		return;

	RwReal fRange = pConfig->wstrValue[9].empty() ? (RwReal)pMobTbl->wScan_Range : (RwReal)_wtoi(pConfig->wstrValue[9].c_str());

	std::string strEffect = ws2s(wstrEffect);

	RwV3d vOffset = { 0.0f, 0.0f, 0.0f };
	m_pScanRangeEffect = GetSobProxy()->CreatePLChildEffect(strEffect.c_str(), vOffset, TRUE);
	if (m_pScanRangeEffect)
		m_pScanRangeEffect->SetScale(fRange * 2.2f + 2.0f);
}

void CNtlSobMonster::DeleteScanRangeEffect(void)
{
	if (m_pScanRangeEffect)
	{
		GetSobProxy()->DeletePLChildEffect(m_pScanRangeEffect);
		m_pScanRangeEffect = NULL;
	}
}

void CNtlSobMonster::OnEventBotHelpMeNfy_Handler( RWS::CMsg& pMsg ) 
{
    SNtlEventBotHelpMe_Nfy* pData = (SNtlEventBotHelpMe_Nfy*)pMsg.pData;

    // 이펙트 생성
    RwV3d vOffset;
    vOffset.x = vOffset.z = 0.0f;
    vOffset.y = GetSobProxy()->GetPLEntityHeight() + 1.0f;    
    if(pData->bIsRequester)
    {
        GetSobProxy()->CreatePLChildEffect(NTL_VID_MARK_HELPME_REQUESTER, vOffset);
    }    
    else
    {
        GetSobProxy()->CreatePLChildEffect(NTL_VID_MARK_HELPME_REQUESTEE, vOffset);
    }
}
