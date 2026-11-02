#include "precomp_dboclient.h"
#include "MrPoPoHandler.h"
#include "DboGlobal.h"
#include "TableContainer.h"
#include "NtlSLTBCrowdManager.h"
#include "DumpCommand.h"
#include "NtlPLCharacter.h"
#include "NtlCameraController.h"
#include "NtlInstanceEffect.h"
#include "SkillTable.h"
#include "DragonBallTable.h"
#include "NtlSkillContainer.h"

// Presentation
#include "NtlPLGlobal.h"
#include "NtlPLCharacter.h"
#include "NtlPLOptionManager.h"


#include "NtlSobProxy.h"
#include "NtlSLEventFunc.h"
#include "NtlPLEventGenerator.h"
#include "NtlFSMDef.h"

CMrPoPoHandler::CMrPoPoHandler(void)
{
}

CMrPoPoHandler::~CMrPoPoHandler(void)
{

}

void CMrPoPoHandler::HandleEvent( RwInt32 nMsg, DWORD param )
{
    // every event acts on the avatar or the game world
    if(GetNtlSLGlobal()->GetSobAvatar() == NULL)
        return;

    switch(nMsg)
    {
    case MSG_MRPOPO_COLLECT_DRAGONBALL:
        OnCollectDragonBall((RwUInt8)param);
        break;
    case MSG_MRPOPO_LEARN_ALL_SKILL:
        OnLearnAllSkill();
        break;
    case MSG_MRPOPO_TEST_CROWD:
        OnTestCrowd((RwInt32)param);
        break;
    case MSG_MRPOPO_OBSERVER_STATIC:
        CNtlSLEventGenerator::CameraObserver(E_OBSERVER_TYPE_STATIC, (RwInt32)param);
        break;
    case MSG_MRPOPO_OBSERVER_LUA:
        CNtlSLEventGenerator::CameraObserver(E_OBSERVER_TYPE_NODE, (RwInt32)param);
        break;
    case MSG_MRPOPO_TEST1:
        g_fTestVal = (RwReal)param;
        break;
    case MSG_MRPOPO_TEST2:
        g_fTestVal2 = (RwReal)param;
        break;
    case MSG_MRPOPO_TEST3:
        g_fTestVal3 = (RwReal)param;
        break;
    case MSG_MRPOPO_ELAPSEDTIME_WEIGHT:
        g_fElapsedWeightValue = (RwReal)param;
        break;
    case MSG_MRPOPO_DUMP_TO_CONSOLE:
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_CONSOLE, param);
        break;
    case MSG_MRPOPO_DUMP_TO_GUI:
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_GUI, param);
        break;
    case MSG_MRPOPO_DUMP_TO_FILE:
        GetDumpCmdManager()->ActiveDumpTarget(CDumpCmdManager::DUMP_TARGET_FILE, param);
        break;
    case MSG_MRPOPO_DUMP:
        if(param == MPP_DUMP_SELF)
        {
            GetDumpCmdManager()->OutputSobObj(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID());
        }
        else if(param == MPP_DUMP_TARGET)
        {
            GetDumpCmdManager()->OutputSobObj(GetNtlSLGlobal()->GetSobAvatar()->GetTargetID());
        }
        break;
    case MSG_MRPOPO_DUMP_REG:
        GetDumpCmdManager()->SetRegisterSerial(GetNtlSLGlobal()->GetSobAvatar()->GetTargetID());
        break;
    case MSG_MRPOPO_DUMP_UNREG:
        GetDumpCmdManager()->SetRegisterSerial(INVALID_SERIAL_ID);
        break;
    case MSG_MRPOPO_LOWSPEC_CHAR:
        CNtlPLCharacter::SetSkipSimpleMaterial(param);
        break;
    case MSG_MRPOPO_LOWSPEC_EFFECT:
        CNtlInstanceEffect::SetLowSpecEnable(param);
        break;
    case MSG_MRPOPO_FREECAMERA:
        if(param)
        {
            CNtlSLEventGenerator::CameraFree();
        }
        else
        {
            CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_FREE);
        }
        break;
    case MSG_MRPOPO_SCOUTER_RENDER:
        CNtlPLGlobal::m_bRenderScouter = (RwBool)param;
        break;
    case MSG_MRPOPO_RENDER_MESHSYSTEM:
        CNtlInstanceEffect::SetRenderMeshSystem(param);
        break;
    case MSG_MRPOPO_RENDER_DECALSYSTEM:
        CNtlInstanceEffect::SetRenderDecalSystem(param);
        break;
    case MSG_MRPOPO_TENKAICHI_MARK:				OnTenkaichiMark((RwInt32)param);  break;
    case MSG_MRPOPO_TRANSLATE_STATE:			OnChangeState((RwUInt32)param);   break;
    case MSG_MRPOPO_TRANSFORM:					OnTransform((RwUInt32)param);     break;
    case MSG_MRPOPO_STUN:						OnStun((RwUInt32)param);          break;
    case MSG_MRPOPO_TARGET_MARKING:				OnTargetMarking((RwBool)param);   break;
    case MSG_MRPOPO_SPEC_TERRAIN_DIST:			OnTerrainDist((RwUInt32)param);   break;
    case MSG_MRPOPO_SPEC_TERRAIN_SHADOW:		OnTerrainShadow((RwBool)param);   break;
    case MSG_MRPOPO_SPEC_WATER_SPECULAR:		OnWaterSpecular((RwBool)param);   break;
	case MSG_MRPOPO_SPEC_CHAR_DIST:				OnCharDist((RwUInt32)param);	  break;
    case MSG_MRPOPO_SPEC_CHAR_EDGE:				OnCharEdge((RwBool)param);        break;
	case MSG_MRPOPO_SPEC_CHAR_GRAYCOLOR:		OnCharGrayColor((RwBool)param);   break;
    case MSG_MRPOPO_SPEC_EFFECT:				OnLowEffect((RwBool)param);       break;
	case MSG_MRPOPO_SPEC_EFFECT_PARTICLE_RATIO: OnParticleRatio((RwUInt32)param); break;
	case MSG_MRPOPO_SPEC_EFFECT_MESH:			OnMeshEffect((RwBool)param);      break;
	case MSG_MRPOPO_SPEC_EFFECT_DECAL:			OnDecalEffect((RwBool)param);     break;
    case MSG_MRPOPO_PUSHING:					OnPushTest();                     break;
    }
}

RwBool CMrPoPoHandler::SendServerCommand( const WCHAR* pwszCommand )
{
    // the game server takes "@" commands, and only in the game
    if(pwszCommand == NULL || pwszCommand[0] != L'@' || GetNtlSLGlobal()->GetSobAvatar() == NULL)
        return FALSE;

    // the packet copies it with wcscpy_s, a longer one would hit the invalid parameter handler
    if(wcslen(pwszCommand) >= NTL_MAX_LENGTH_OF_CHAT_MESSAGE)
        return FALSE;

    return GetDboGlobal()->GetGamePacketGenerator()->SendServerCommand(pwszCommand);
}

void CMrPoPoHandler::OnCollectDragonBall( RwUInt8 byBallType )
{
    // the seven balls of the first dragon ball table row of that type (eDRAGON_BALL_TYPE)
    CDragonBallTable* pTable = API_GetTableContainer()->GetDragonBallTable();
    for(CTable::TABLEIT it = pTable->Begin(); it != pTable->End(); ++it)
    {
        sDRAGONBALL_TBLDAT* pData = (sDRAGONBALL_TBLDAT*)it->second;
        if(pData->byBallType != byBallType)
            continue;

        WCHAR wBuf[64];
        for(RwInt32 i = 0; i < NTL_ITEM_MAX_DRAGONBALL; ++i)
        {
            swprintf_s(wBuf, L"@additem %u", pData->aBallTblidx[i]);
            SendServerCommand(wBuf);
        }
        return;
    }
}

void CMrPoPoHandler::OnLearnAllSkill()
{
    // first grade skills the class can train at this level, the server checks them again
    CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
    RwUInt8 byLevel = Logic_GetLevel(pAvatar);
    RwUInt8 byClass = Logic_GetPlayerClass(pAvatar);
    CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();
    CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();

    WCHAR wBuf[64];
    for(CTable::TABLEIT it = pSkillTable->Begin(); it != pSkillTable->End(); ++it)
    {
        sSKILL_TBLDAT* pData = (sSKILL_TBLDAT*)it->second;
        if(!pData->bValidity_Able || !pData->bSelfTrain || pData->bDefaultDisplayOff || pData->bySkill_Grade != 1)
            continue;

        if(pData->byRequire_Train_Level > byLevel || !Dbo_CheckClass(byClass, pData->dwPC_Class_Bit_Flag))
            continue;

        if(pSkillContainer->GetSkillFromTableId(pData->tblidx))
            continue;

        swprintf_s(wBuf, L"@addskill %u", pData->tblidx);
        SendServerCommand(wBuf);
    }
}

void CMrPoPoHandler::OnTestCrowd( RwInt32 nCrowdType )
{
    GetNtlSLCrowdManager()->ChangeStep((ETBCrowdStepType)nCrowdType);
}

void CMrPoPoHandler::OnNamekTest() 
{
    CNtlSobActor* pActor = (CNtlSobActor*)GetNtlSLGlobal()->GetSobAvatar();
    if(pActor->GetTargetID() != INVALID_SERIAL_ID)
    {
        CNtlSobActor* pTarget = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(pActor->GetTargetID());
        RwV3d vDir = pTarget->GetPosition() - pActor->GetPosition();
        RwV3dNormalize(&vDir, &vDir);
        pActor->SetDirection(&vDir);
    }

    GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy()->SetBaseAnimation(2001);
}

void CMrPoPoHandler::OnTenkaichiMark(RwInt32 nType)
{
    CNtlSLEventGenerator::TenkaichiMarking(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), (BYTE)nType);
}

void CMrPoPoHandler::OnChangeColor( RwInt32 nType ) 
{
    RwUInt32 hSerialID = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
    SEventColorChange event;
    event.eType = (EColorChangeType)nType;
    event.colorEdge.red = event.colorEdge.green = event.colorEdge.blue = 255;
    event.colorBody.red = 208; event.colorBody.green = 99; event.colorBody.blue = 208;
    event.colorAdd.red = 30; event.colorAdd.green = 30; event.colorAdd.blue = 30;
    CNtlPLEventGenerator::AnimEventColorChange(hSerialID, &event);    
}

void CMrPoPoHandler::OnChangeState( RwUInt32 uiStateID ) 
{
    RwUInt32 hSerialID = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
    CNtlSLEventGenerator::SobStateTransition(hSerialID, uiStateID);
}

void CMrPoPoHandler::OnTransform( RwUInt32 uiTransformID ) 
{
    RwUInt32 hSerialID = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();
    sASPECTSTATE state;
    state.sAspectStateBase.byAspectStateId = (BYTE)uiTransformID;    
    CNtlSLEventGenerator::SobTransform(hSerialID, state);
}

void CMrPoPoHandler::OnStun( RwUInt32 uiStunType )
{
    CNtlSobAvatar* pActor = GetNtlSLGlobal()->GetSobAvatar();

    // none (DBO_STUN_TYPE_INVALID) ends the stun, leaving the state undoes its look
    if(uiStunType >= DBO_STUN_TYPE_COUNT)
    {
        CNtlSLEventGenerator::SobStateTransition(pActor->GetSerialID(), NTL_FSMSID_IDLE);
        return;
    }

    CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(pActor->GetBehaviorData()); 
    pBeData->GetCtrlStuff()->uExtra.sStun.byStunType = (BYTE)uiStunType;

    CNtlSLEventGenerator::SobStateTransition(pActor->GetSerialID(), NTL_FSMSID_STUN);
}

void CMrPoPoHandler::OnTargetMarking( RwBool bSelect ) 
{
    RwUInt32 hSerialID = GetNtlSLGlobal()->GetSobAvatar()->GetSerialID();

    if(bSelect)
    {
        CNtlSLEventGenerator::SobTargetMarkSelect(hSerialID);
    }
    else
    {
        CNtlSLEventGenerator::SobTargetMarkRelease(hSerialID);
    }
}

void CMrPoPoHandler::OnPushTest() 
{
    CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();    
    RwV3d vDestPos = pSobAvatar->GetPosition() - pSobAvatar->GetDirection() * 0.3f;

    CNtlSLEventGenerator::SobPushingNfy(pSobAvatar->GetSerialID(), BATTLE_ATTACK_RESULT_HIT, 99, vDestPos);
}

void CMrPoPoHandler::OnTerrainDist( RwUInt32 uiDist ) 
{
	GetNtlPLOptionManager()->SetTerrainFar((RwReal)uiDist);
}


void CMrPoPoHandler::OnTerrainShadow( RwBool bEnable ) 
{
	GetNtlPLOptionManager()->SetTerrainShadow(bEnable);
}

void CMrPoPoHandler::OnWaterSpecular( RwBool bEnable ) 
{
	GetNtlPLOptionManager()->SetWaterSpecular(bEnable);
}

void CMrPoPoHandler::OnCharDist( RwUInt32 uiDist ) 
{
	CNtlSobManager::SetOptionOutRange((RwReal)uiDist);
}


void CMrPoPoHandler::OnCharEdge( RwBool bEnable ) 
{
	if(bEnable)
		CNtlPLCharacter::SetSkipEdge(FALSE);
	else
		CNtlPLCharacter::SetSkipEdge(TRUE);
}

void CMrPoPoHandler::OnCharGrayColor(RwBool bEnable)
{
	/*
	if(bEnable)
		CNtlPLCharacter::SetSkipSimpleMaterial(TRUE);
	else
		CNtlPLCharacter::SetSkipSimpleMaterial(FALSE);
		*/
}


void CMrPoPoHandler::OnLowEffect( RwBool bEnable ) 
{
    CNtlInstanceEffect::SetLowSpecEnable(bEnable);    
}

void CMrPoPoHandler::OnParticleRatio(RwUInt32 uiRatio)
{
	RwReal fRatio = (RwReal)uiRatio/10.0f;

    CNtlInstanceEffect::SetLowSpecRatio(fRatio);    
}

void CMrPoPoHandler::OnMeshEffect(RwBool bEnable)
{
	CNtlInstanceEffect::SetRenderMeshSystem(bEnable);
}

void CMrPoPoHandler::OnDecalEffect(RwBool bEnable)
{
	CNtlInstanceEffect::SetRenderDecalSystem(bEnable);
}

