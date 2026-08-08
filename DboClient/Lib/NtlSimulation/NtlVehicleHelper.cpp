#include "precomp_ntlsimulation.h"
#include "NtlVehicleHelper.h"

// shared
#include "GraphicDataTable.h"
#include "VehicleTable.h"

// Presentation
#include "NtlPLEvent.h"

// sound
#include "NtlSoundManager.h"
#include "NtlSound.h"

// simmulation
#include "NtlSLLogic.h"
#include "NtlSob.h"
#include "NtlSobManager.h"
#include "NtlSobPlayer.h"
#include "NtlSobVehicle.h"
#include "NtlSobProxy.h"
#include "NtlSobVehicleAttr.h"
#include "NtlBehaviorData.h"


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


#define dVEHICLE_SOUND_PATH_IDLE			"vehicle\\v_bik_idle"			// v_bik_idle[1-3].wav
#define dVEHICLE_SOUND_PATH_RUN			"vehicle\\v_bik_run"			// v_bik_run[1-2].wav
#define dVEHICLE_SOUND_PATH_START			"vehicle\\v_bik_start"			// v_bik_start[1-2].wav
#define dVEHICLE_SOUND_PATH_STOP			"vehicle\\v_bik_stop"			// v_bik_stop[1-2].wav
#define dVEHICLE_SOUND_PATH_JUMP			"vehicle\\v_bik_jump"			// v_bik_jump[1-2].wav
#define dVEHICLE_SOUND_PATH_LANDING		"vehicle\\v_bik_landing"		// v_bik_landing[1-2].wav

// irregular naming, can't be built from a common prefix + index
static const char* s_apcVehicleSoundTurn[] =
{
	"vehicle\\v_bik_left_right_1.wav",
	"vehicle\\v_bik_left_rihgt_2.wav",
	"vehicle\\v_bik_left_right_3.wav",
	"vehicle\\v_bik_left_right_4.wav",
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


#if ((VEHICLE_SRP1_ANIMATION_END - VEHICLE_SRP1_ANIMATION_START) != (VEHICLE_SRP_X_ANIMATION_END - VEHICLE_SRP_X_ANIMATION_START))
	#error Please checking the vehicle animation list
#endif


static void BuildVehicleSoundFileName( char* pcBuffer, size_t uiBufferSize, const char* pcPrefix, int iMinIndex, int iMaxIndex )
{
	sprintf_s( pcBuffer, uiBufferSize, "%s%u.wav", pcPrefix, NtlRandomNumber( iMinIndex, iMaxIndex ) );
}


CNtlVehicleHelper::CNtlVehicleHelper( void )
{
	// Vehicle animation mapper
	m_defAminMapper[1] = VEHICLE_SRP1_ANIMATION_START;
	m_defAminMapper[2] = VEHICLE_SRP2_ANIMATION_START;

	m_pPlayer = NULL;
	m_pVehicle = NULL;

	m_pVehicleTblData = NULL;

	m_bStrunAniPlay = FALSE;

	m_hVehicleLoopSoundHandle = INVALID_SOUND_HANDLE;
	m_eVehicleSoundLoop = VEHICLE_SOUND_LOOP_NONE;
	m_acLastLoopFileName[0] = '\0';
}

CNtlVehicleHelper::~CNtlVehicleHelper( void )
{
	DetachPlayer();
}

void CNtlVehicleHelper::AttachPlayer( SERIAL_HANDLE hDriver, SERIAL_HANDLE hVehicle )
{
	CNtlSobPlayer* pPlayer = (CNtlSobPlayer*) GetNtlSobManager()->GetSobObject( hDriver );
	CNtlSobVehicle* pVehicle = (CNtlSobVehicle*) GetNtlSobManager()->GetSobObject( hVehicle );

	if ( pPlayer )
	{
		RwUInt32 uiClassID = pPlayer->GetClassID();

		if ( uiClassID == SLCLASS_PLAYER ||
			 uiClassID == SLCLASS_AVATAR )
		{
			m_pPlayer = (CNtlSobPlayer*) pPlayer;
			m_pVehicle = (CNtlSobVehicle*) pVehicle;
			m_pVehicleTblData = ((CNtlSobVehicleAttr*)m_pVehicle->GetSobAttr())->GetVehicleTable();
		}
		else
		{
			m_pPlayer = NULL;
			m_pVehicle = NULL;
			m_pVehicleTblData = NULL;
		}
	}
	else
	{
		m_pPlayer = NULL;
		m_pVehicle = NULL;
		m_pVehicleTblData = NULL;
	}
}

void CNtlVehicleHelper::DetachPlayer( void )
{
	Vehicle_StopLoopSound();

	m_pPlayer = NULL;
	m_pVehicle = NULL;
	m_pVehicleTblData = NULL;
}

RwBool CNtlVehicleHelper::IsDriver( void )
{
	return Logic_IsVehicleDriver( m_pPlayer );
}

void CNtlVehicleHelper::Vehicle_EnableVisible( RwBool bVisible )
{
	if ( m_pPlayer )
	{
		m_pPlayer->EnableVisible( bVisible );
	}

	if ( m_pVehicle )
	{
		m_pVehicle->EnableVisible( bVisible );
	}
}

void CNtlVehicleHelper::Vehicle_Anim_Idle( RwBool bBlend /*= TRUE*/ )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		if ( !bBlend )
		{
			m_pPlayer->GetSobProxy()->SetAnimBlendNoneOneTime();
		}

		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_IDLE );

		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		if ( !bBlend )
		{
			m_pVehicle->GetSobProxy()->SetAnimBlendNoneOneTime();
		}

		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_IDLE );
	}
}

void CNtlVehicleHelper::Vehicle_Anim_Run( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_RUN );

		if(uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation( uiVehicleAnimIdx );
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_RUN );
	}

	char acSoundFile[64];
	BuildVehicleSoundFileName( acSoundFile, sizeof(acSoundFile), dVEHICLE_SOUND_PATH_RUN, 1, 2 );
	Vehicle_SetLoopSound( VEHICLE_SOUND_LOOP_RUN, acSoundFile );
}

void CNtlVehicleHelper::Vehicle_Anim_Start( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_START );
		
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_START, FALSE );
	}

	Vehicle_SetLoopSound( VEHICLE_SOUND_LOOP_NONE, NULL );

	char acSoundFile[64];
	BuildVehicleSoundFileName( acSoundFile, sizeof(acSoundFile), dVEHICLE_SOUND_PATH_START, 1, 2 );
	Vehicle_PlayOneShotSound( acSoundFile );
}

void CNtlVehicleHelper::Vehicle_Anim_Stop( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_STOP );
		
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_STOP, FALSE );
	}

	Vehicle_SetLoopSound( VEHICLE_SOUND_LOOP_NONE, NULL );

	char acSoundFile[64];
	BuildVehicleSoundFileName( acSoundFile, sizeof(acSoundFile), dVEHICLE_SOUND_PATH_STOP, 1, 2 );
	Vehicle_PlayOneShotSound( acSoundFile );
}

void CNtlVehicleHelper::Vehicle_Anim_Jump( void )
{
	if(!IsDriver())
		return;

	if (m_bStrunAniPlay)
		return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_JUMP );
		
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_JUMP, FALSE );
	}

	char acSoundFile[64];
	BuildVehicleSoundFileName( acSoundFile, sizeof(acSoundFile), dVEHICLE_SOUND_PATH_JUMP, 1, 2 );
	Vehicle_PlayOneShotSound( acSoundFile );
}

void CNtlVehicleHelper::Vehicle_Anim_TurnLeft( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_TURN_LEFT );
		
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_TURN_LEFT, FALSE );
	}

	Vehicle_PlayOneShotSound( s_apcVehicleSoundTurn[ NtlRandomNumber( 0, 3 ) ] );
}

void CNtlVehicleHelper::Vehicle_Anim_TurnRight( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_TURN_RIGHT );
		
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_TURN_RIGHT, FALSE );
	}

	Vehicle_PlayOneShotSound( s_apcVehicleSoundTurn[ NtlRandomNumber( 0, 3 ) ] );
}

void CNtlVehicleHelper::Vehicle_Anim_Landing( void )
{
	if ( !IsDriver() ) return;

	if ( m_bStrunAniPlay ) return;

	if ( m_pPlayer && m_pVehicleTblData )
	{
		RwUInt32 uiVehicleAnimIdx = GetVehicleAnimIdxFromSRPType( m_pVehicleTblData->bySRPType, VEHICLE_SRP_X_LANDING );
	
		if (uiVehicleAnimIdx != 0xffffffff)
			m_pPlayer->GetSobProxy()->SetBaseAnimation(uiVehicleAnimIdx);
	}

	if ( m_pVehicle )
	{
		m_pVehicle->GetSobProxy()->SetBaseAnimation( VEHICLE_LANDING, FALSE );
	}

	char acSoundFile[64];
	BuildVehicleSoundFileName( acSoundFile, sizeof(acSoundFile), dVEHICLE_SOUND_PATH_LANDING, 1, 2 );
	Vehicle_PlayOneShotSound( acSoundFile );
}

void CNtlVehicleHelper::UpdateEvent( RWS::CMsg& pMsg )
{
	if ( pMsg.Id == g_EventSobVehicleStunt )
	{
		Vehicle_Anim_Jump();

		m_bStrunAniPlay = TRUE;
	}
	else if ( pMsg.Id == g_EventAnimEnd )
	{
		SNtlEventAnimEnd* pData = (SNtlEventAnimEnd*) pMsg.pData;

		if ( pData->uiBaseAnimKey == VEHICLE_JUMP )
		{
			m_bStrunAniPlay = FALSE;
		}
	}
}

RwUInt32 CNtlVehicleHelper::GetVehicleAnimIdxFromSRPType( RwUInt8 bySRPType, RwUInt8 byRVAnimIdx )
{
	mapdef_VEHICLE_ANIM_MAPPER::iterator it = m_defAminMapper.find( bySRPType );
	if ( it == m_defAminMapper.end() ) return 0xffffffff;

	return (RwUInt32)it->second + (RwUInt32)byRVAnimIdx;
}

// Called every frame while idle (see CNtlFSMVehicleActIdleState::Update), since the engine can be
// turned on/off while already parked, unlike the other Vehicle_Anim_* triggers which are edge-fired
// from a move-state transition and can just rely on that transition happening at the right time.
void CNtlVehicleHelper::UpdateVehicleIdleSound( void )
{
	if ( !IsDriver() || m_bStrunAniPlay || !m_pVehicle )
	{
		Vehicle_StopLoopSound();
		return;
	}

	CNtlBeCharData* pBeData = reinterpret_cast<CNtlBeCharData*>( m_pVehicle->GetBehaviorData() );
	SCtrlStuff* pCtrlStuff = pBeData->GetCtrlStuff();

	if ( !pCtrlStuff->sVehicle.bEngineOn )
	{
		Vehicle_StopLoopSound();
		return;
	}

	if ( m_eVehicleSoundLoop != VEHICLE_SOUND_LOOP_IDLE || !Vehicle_IsLoopSoundAlive() )
	{
		char acSoundFile[64];
		BuildVehicleSoundFileName( acSoundFile, sizeof(acSoundFile), dVEHICLE_SOUND_PATH_IDLE, 1, 3 );
		Vehicle_SetLoopSound( VEHICLE_SOUND_LOOP_IDLE, acSoundFile );
	}

	UpdateVehicleLoopSoundPosition();
}

// The loop is only positioned once, at the moment it starts - a 3D sound's volume is computed from
// the (fixed) distance between that position and the listener, so left alone it fades out as the
// vehicle drives away from wherever it happened to be when the sound started, then fades back in if
// it happens to circle back, which reads as random volume drift rather than a clean loop. Called every
// frame from both the idle state and the move behaviors to keep it pinned to the vehicle's live position.
void CNtlVehicleHelper::UpdateVehicleLoopSoundPosition( void )
{
	if ( m_hVehicleLoopSoundHandle == INVALID_SOUND_HANDLE || !m_pVehicle )
		return;

	RwV3d vPos = m_pVehicle->GetPosition();
	GetSoundManager()->SetSoundPosition( m_hVehicleLoopSoundHandle, vPos.x, vPos.y, vPos.z );
}

void CNtlVehicleHelper::Vehicle_PlayOneShotSound( const char* pcFileName )
{
	if ( !m_pVehicle || !pcFileName )
		return;

	RwV3d vPos = m_pVehicle->GetPosition();

	// CHANNEL_GROUP_OBJECT_MUSIC only ever keeps one sound active at a time (it queues the rest via
	// its own released/sleeping-sound priority replay), which stepped on the vehicle's own looping
	// idle/run sound. CHANNEL_GROUP_EFFECT_SOUND is the same group footsteps already use for several
	// overlapping positional one-shots, so it plays fine alongside the loop.
	sNtlSoundPlayParameta tParam;
	tParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
	tParam.pcFileName		= (char*)pcFileName;
	tParam.bLoop			= false;
	tParam.fVolume			= NtlRandomNumber( 0.85f, 1.f );
	tParam.fXPos			= vPos.x;
	tParam.fYPos			= vPos.y;
	tParam.fZPos			= vPos.z;
	tParam.fMinDistance		= 7.f;
	tParam.fMaxDistance		= 30.f;

	GetSoundManager()->Play( &tParam );
}

void CNtlVehicleHelper::Vehicle_SetLoopSound( eVehicleSoundLoop eLoop, const char* pcFileName )
{
	if ( m_eVehicleSoundLoop == eLoop && Vehicle_IsLoopSoundAlive() )
		return;

	// Stopping and immediately requesting a new sound with the same file name can hit FMOD before it
	// has actually freed the previous channel (the stop is only fully processed on the sound system's
	// own next update tick), so a fast stop/restart with the same random pick can get silently
	// rejected as a duplicate. Remember what was playing and avoid re-picking it right away.
	char acPrevFileName[64];
	strncpy_s( acPrevFileName, sizeof(acPrevFileName), m_acLastLoopFileName, _TRUNCATE );

	Vehicle_StopLoopSound();

	if ( eLoop != VEHICLE_SOUND_LOOP_NONE && m_pVehicle && pcFileName && strcmp( pcFileName, acPrevFileName ) == 0 )
	{
		// caller handed us the file it just tried to stop - nudge it back to NONE so the next
		// self-heal pass on the following frame retries with a fresh random pick instead.
		m_eVehicleSoundLoop = VEHICLE_SOUND_LOOP_NONE;
		return;
	}

	if ( eLoop != VEHICLE_SOUND_LOOP_NONE && m_pVehicle && pcFileName )
	{
		RwV3d vPos = m_pVehicle->GetPosition();

		sNtlSoundPlayParameta tParam;
		tParam.iChannelGroup	= CHANNEL_GROUP_EFFECT_SOUND;
		tParam.pcFileName		= (char*)pcFileName;
		tParam.bLoop			= true;
		tParam.fVolume			= NtlRandomNumber( 0.85f, 1.f );
		tParam.fXPos			= vPos.x;
		tParam.fYPos			= vPos.y;
		tParam.fZPos			= vPos.z;
		tParam.fMinDistance		= 7.f;
		tParam.fMaxDistance		= 30.f;

		int iResult = GetSoundManager()->Play( &tParam );

		if ( iResult == SOUNDRESULT_OK && tParam.hHandle != INVALID_SOUND_HANDLE )
		{
			m_hVehicleLoopSoundHandle = tParam.hHandle;
			strncpy_s( m_acLastLoopFileName, sizeof(m_acLastLoopFileName), pcFileName, _TRUNCATE );
			m_eVehicleSoundLoop = eLoop;
		}
		else
		{
			// Play() was rejected (duplicate name still winding down, channel cap, etc.) - leave
			// the loop state at NONE so the per-frame self-heal check retries next frame instead
			// of believing a sound is playing when nothing actually started.
			m_eVehicleSoundLoop = VEHICLE_SOUND_LOOP_NONE;
		}

		return;
	}

	m_eVehicleSoundLoop = eLoop;
}

void CNtlVehicleHelper::Vehicle_StopLoopSound( void )
{
	if ( m_hVehicleLoopSoundHandle != INVALID_SOUND_HANDLE )
	{
		GetSoundManager()->Stop( m_hVehicleLoopSoundHandle );
		m_hVehicleLoopSoundHandle = INVALID_SOUND_HANDLE;
	}

	m_eVehicleSoundLoop = VEHICLE_SOUND_LOOP_NONE;
}

// Self-heals against the loop sound having been stopped/evicted by something other than us
// (channel exhaustion, engine hiccups, etc.) instead of trusting m_eVehicleSoundLoop blindly.
RwBool CNtlVehicleHelper::Vehicle_IsLoopSoundAlive( void )
{
	if ( m_hVehicleLoopSoundHandle == INVALID_SOUND_HANDLE )
		return FALSE;

	CNtlSound* pSound = GetSoundManager()->GetSound( m_hVehicleLoopSoundHandle );

	return ( pSound && pSound->m_eState != SPS_NONE ) ? TRUE : FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
