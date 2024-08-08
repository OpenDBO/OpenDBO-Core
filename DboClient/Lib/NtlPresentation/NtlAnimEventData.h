#ifndef _NTL_ANIM_EVENT_DATA_H_
#define _NTL_ANIM_EVENT_DATA_H_

#include "GraphicDataTable.h"
#include "NtlInstanceEffect.h"
#include "NtlSoundDefines.h"
#include "NtlSerializer.h"

#define MAX_DEFAULT_NAME	    32
#define ANIM_EVENT_NONE_TIME    0.f

// 카메라 셰이크 
#define CAMERA_SHAKE_FACTOR             8.0f
#define CAMERA_SHAKE_MAX_HEIGHT_FACTOR  2.0f

enum EAttackType
{
	ATTACK_TYPE_PHYSICAL,
	ATTACK_TYPE_ENERGY
};

enum EHandType
{
	HAND_TYPE_LEFT,
	HAND_TYPE_RIGHT,
};

// Projectile이 어디서 나갈지의 플래그
enum EBoneType
{
    BONE_CHARACTER,       ///< 캐릭터의 Bone에서 발사된다.
    BONE_WEAPON,          ///< Weapon의 Bone에서 발사된다. (하나의 Bone이름으로 정해져있다.)
    BONE_SUB_WEAPON,      ///< Sub Weapon의 Bone에서 발사된다. (3개이상의 Bone에서 선택적으로 적용된다)
};

// 어느 SubWeapon Bone을 설정할지(중복설정가능)에 사용되는 Flag
enum ESubWeaponBoneFlag
{
    SUB_WEAPON_PROJECTILE_1 = 1,
    SUB_WEAPON_PROJECTILE_2 = 2,
    SUB_WEAPON_PROJECTILE_3 = 4,    
};

// SubWeapon Event에 사용되는 Flag
enum ESubWeaponActiveFlag
{
    SUB_WEAPON_ACTIVE,      ///< SubWeapon 활성화 (SubWeapon으로 무기를 체인지 한다.)
    SUB_WEAPON_DEACTIVE,    ///< SubWeapon 비활성 (일반 Weapon으로 무기를 체인지 한다.)
};

// PostEffect Event에 사용되는 타겟 플래그
enum EPostEffectTypeFlag
{
    POST_EFFECT_TARGET_TYPE_SELF,     ///< PC에 Post Effect 위치 설정
    POST_EFFECT_TARGET_TYPE_TARGET,  ///< 적에게 Post Effect 위치 설정        
};

/// Target에 표시될 Effect의 방향 설정 플래그
enum ETargetEffectType
{
	TARGET_EFFECT_TYPE_NONE,		///< 아무런 설정값이 없는 일반 이펙트
	TARGET_EFFECT_TYPE_SIDE,		///< 타겟의 옆방향으로 진행되는 이펙트
	TARGET_EFFECT_TYPE_FRONT,		///< 타겟의 정면 방향(or 후면 방향)으로 진행되는 이펙트
};

// Projectile Effect Type
enum EProjectileEffectType
{
    BEID_PROJ_BALL      = 2000,
    BEID_PROJ_BEAM      = 2001,
    BEID_PROJ_HISSIDAN  = 2002,
    BEID_PROJ_HELLZONE  = 2003,
    BEID_PROJ_MULTI_HISSIDAN = 2004,
    BEID_PROJ_MAGARE    = 2005,
};

// TMQ Event Type
enum ETMQEventType
{
    E_TMQ_EVENT_IN,
    E_TMQ_EVENT_OUT,
};

// Explosion event type
enum ENtlPLExplosionEventType
{
    EXPLOSION_EVENT_TYPE_SMALL		= 0,	// 작은 폭발
    EXPLOSION_EVENT_TYPE_NORMAL		= 1,	// 중간 폭발
    EXPLOSION_EVENT_TYPE_BIG		= 2,	// 큰 폭발
};

// Direct Event Type
enum ENtlPLDirectEventType
{
    DIRECT_EVENT_TYPE_CAMERA_SHAKE,         
};

// Foot Step Type
enum EFootStepType
{
    FOOT_LEFT = 0,
    FOOT_RIGHT = 1,    
};

// 발자국날때 어떤 소리가 날지 Mob 타입
enum EFootStepMobType
{
    FOOT_TYPE_NORMAL,
    FOOT_TYPE_LARGE,
};

// 비주얼 이펙트 Attach 타입
enum EVEAttachType
{
    VE_ATTACH_POS = 0,              ///< 모델의 위치에 Attach
    VE_ATTACH_BONE,                 ///< 모델의 본에 Attach
};

//////////////////////////////////////////////////////////////////////////
struct SEventAnim
{
	RwReal			fTime;
	EAnimEventType	eEventID;

	SEventAnim() : fTime(ANIM_EVENT_NONE_TIME) {}    
};

// Projectile Type에 따른 추가 파라미터들을 정의하기 위한 Union
struct SHissidanData        /// Hissidan 타입을 위한 추가 데이터
{
    RwBool      bApplyAngle;                              ///< 각도 적용/비적용 유무 (비적용을 하면 Hand Type에 따라서 방향이 휜다)
    RwV2d       v2dAngle;                                   ///< 발사 각도 (X축, Y축)
};

struct SMultiHissidanData   /// Multi Hissidan 타입을 위한 추가 데이터
{
    RwInt32     nCount;                                   ///< Hissidan 개수
    RwV2d*      pArrayAngle;                              ///< 발사 각도들의 배열 (동적 할당한다)
};

struct SHellZoneData        /// 마공 포위탄형 타입을 위한 추가 데이터
{
    RwV3d				vTargetEffectOffset;						///< Target에서의 Offset 거리
    RwReal				fTargetEffectStartWaitTime;					///< Target에 날아가기 전에 기다리는 시간
    RwReal				fTargetEffectSpeed;							///< Target에 날아가는 속도
};

union UEffectTypeExtraData
{
    SHissidanData       hissidanData;
    SMultiHissidanData  multiHissidanData;
    SHellZoneData       hellZoneData;
};

struct SEventAnimHit : public SEventAnim
{
	RwUInt32			uiDamage;
	RwBool				bPowerEffect;
	EAttackType			eAttackType;								///< Attack Type
    ETargetBehavior		eTargetBehavior;
    RwBool              bKB2Push;                                   ///< 넉다운을 하지 않으면 Push로 처리할지 유무 플래그
    EHandType			eHandType;									///< HandType
    EProjectileEffectType uiProjectileEffectType;					///< Ball, Beam, Hissidan, hell zone, Multi Hissidan

    EBoneType			eProjectileShotType;						///< 어느 Bone에서 Projectile이 날라가는지 설정
	RwInt32             nSubWeaponFlag;                             ///< SubWeapon의 어느 Bone에서 나갈지 체크한 플래그
	
	RwChar				chBoneName[MAX_DEFAULT_NAME];				///< Character Bone Name	
	RwChar              chProjectileEffectName[MAX_DEFAULT_NAME];	///< 날라가는 Effect Name
	RwReal				fProjectileSpeed;							///< projectile speed
    RwBool              bTargetAttach;								///< Effect를 Target에 Attach를 할지의 유무
    RwReal              fTargetHeight;								///< Target Effect가 나올 위치 (1이 기본 100%)

	RwChar              chTargetEffectName[MAX_DEFAULT_NAME];		///< Target에 표시될 Effect Name
	ETargetEffectType	eTargetEffectType;							///< Target Effct의 진행 방향 타입
    RwChar              chSubTargetEffect[MAX_DEFAULT_NAME];        ///< 범위 공격시 주위 타겟에게 표시될 이펙트
    RwChar              chTargetSoundName[MAX_DEFAULT_NAME];		///< Target에 맞았을 경우 나오는 Sound File Name
    eChannelGroupType	eSoundType;									///< Sound type(CHANNEL_GROUP_VOICE_SOUND or CHANNEL_GROUP_EFFECT_SOUND)    
    RwBool              bHitSoundEcho;                              ///< Hit시 사운드가 Echo 적용될지 유무
    RwBool              bCameraShake;                               ///< Target Hit시 Camera Shake유무
    RwReal              fCameraShakeFactor;                         ///< 카메라 셰이크 팩터
    RwReal              fCameraShakeMaxHeight;                      ///< 카메라 셰이크 최대값 팩터
	RwChar              chWordEffect[MAX_DEFAULT_NAME];             ///< Target Hit시 나타날 Word Effect (ex. 파팍)
    
    UEffectTypeExtraData uEffectTypeExtraData;                      ///< Effect Type에 따른 추가 데이터 Union
	
	
	SEventAnimHit() : eAttackType(ATTACK_TYPE_PHYSICAL),
					  eTargetBehavior(TARGET_BEHAVIOR_NONE), 
                      bKB2Push(FALSE),
					  uiDamage(0), 
					  bPowerEffect(FALSE), 
					  eHandType(HAND_TYPE_LEFT),
					  uiProjectileEffectType(BEID_PROJ_BALL),
					  fProjectileSpeed(10.0f),					  
                      eProjectileShotType(BONE_CHARACTER),
                      nSubWeaponFlag(0),
                      bTargetAttach(FALSE),
                      fTargetHeight(1.0f),
                      eSoundType(CHANNEL_GROUP_EFFECT_SOUND),
                      bCameraShake(FALSE),					  
					  eTargetEffectType(TARGET_EFFECT_TYPE_NONE),
                      bHitSoundEcho(FALSE),
                      fCameraShakeFactor(CAMERA_SHAKE_FACTOR),
                      fCameraShakeMaxHeight(CAMERA_SHAKE_MAX_HEIGHT_FACTOR)
	{ 
		eEventID = EVENT_ANIM_HIT; 
		memset(chBoneName, 0, sizeof(chBoneName));
        ZeroMemory(chProjectileEffectName, sizeof(chProjectileEffectName));
        ZeroMemory(chTargetEffectName, sizeof(chTargetEffectName));
        ZeroMemory(chTargetSoundName, sizeof(chTargetSoundName));
        ZeroMemory(chWordEffect, sizeof(chWordEffect));
        ZeroMemory(&uEffectTypeExtraData, sizeof(UEffectTypeExtraData));
        ZeroMemory(chSubTargetEffect, sizeof(chSubTargetEffect));
	}

    ~SEventAnimHit()
    {
        if(uiProjectileEffectType == BEID_PROJ_MULTI_HISSIDAN)
        {
            NTL_ARRAY_DELETE(uEffectTypeExtraData.multiHissidanData.pArrayAngle); // 동적으로 할당한것이라 Delete해줘야 한다.
        }
    }

    void SaveSerialize(CNtlSerializer& sOut);
    void LoadSerialize(CNtlSerializer& sIn);

};

struct SEventAnimEnd : public SEventAnim
{
	RwUInt32	uiAnimKey;
	SEventAnimEnd() : uiAnimKey(0) { eEventID = EVENT_ANIM_END;}

};

struct SEventVisualEffect : public SEventAnim
{
    RwChar  chEffectName[MAX_DEFAULT_NAME];     ///< Effect Name;
    EBoneType eBoneType;                        ///< Effect가 붙을 Bone Type
    RwChar  chBoneName[MAX_DEFAULT_NAME];       ///< Attach가 될 Bone Name
    RwV3d   vOffSetPos;                         //<  Offset;    
    RwBool  bAttach;                            ///< 모델에 Attach 할지 유무
    RwBool  bAttachBone;                        ///< Bone에 Attach 할지 유무 (TRUE = Bone Attach, FALSE = Pos Attach)
    RwBool  bApplyScale;                        ///< 부모 Entity의 Scale을 적용할지 유무
    RwBool  bProjectileType;                    ///< 발사체타입인지 유무

    SEventVisualEffect() : eBoneType(BONE_CHARACTER)
    {
        ZeroMemory(chBoneName, sizeof(chBoneName));
        ZeroMemory(chEffectName, sizeof(chEffectName));        
        ZeroMemory(&vOffSetPos, sizeof(vOffSetPos));        
        bAttach = TRUE;
        bAttachBone = FALSE;        
        bApplyScale = TRUE;
        bProjectileType = FALSE;

        eEventID = EVENT_ANIM_VISUAL_EFFECT;
    }

};

struct SEventSound : public SEventAnim
{
	RwChar	chSoundName[MAX_DEFAULT_NAME * 2]; ///< Sound 파일명 (Sound 폴더및 경로명 포함)
    RwChar	chSoundName2[MAX_DEFAULT_NAME * 2]; ///< Sound 파일명 (Sound 폴더및 경로명 포함)
    RwChar	chSoundName3[MAX_DEFAULT_NAME * 2]; ///< Sound 파일명 (Sound 폴더및 경로명 포함)
    RwChar	chSoundName4[MAX_DEFAULT_NAME * 2]; ///< Sound 파일명 (Sound 폴더및 경로명 포함)
    eChannelGroupType eSoundType;				///< Sound Type (CHANNEL_GROUP_VOICE_SOUND or CHANNEL_GROUP_EFFECT_SOUND)    
    RwBool  bLoop;                              ///< Sound Loop 유무
    RwReal  fSoundVolume;                       ///< Sound Volume
    RwReal  fSoundDist;                         ///< 유효 거리
    RwReal  fSoundDecayDist;                    ///< 사운드 감쇠 거리
    RwReal  fSoundPitchMin;                     ///< 사운드 피치 최소값
    RwReal  fSoundPitchMax;                     ///< 사운드 피치 최대값

	SEventSound()
    {
        eEventID = EVENT_ANIM_VISUAL_SOUND;
        ZeroMemory(chSoundName, sizeof(chSoundName));
        ZeroMemory(chSoundName2, sizeof(chSoundName2));
        ZeroMemory(chSoundName3, sizeof(chSoundName3));
        ZeroMemory(chSoundName4, sizeof(chSoundName4));
        eSoundType = CHANNEL_GROUP_EFFECT_SOUND;
        bLoop = FALSE;
        fSoundVolume = 100.0f;
        fSoundDist = 15.0f;
        fSoundDecayDist = 30.0f;
        fSoundPitchMin = fSoundPitchMax =  dNTLSOUND_PITCH_DEFAULT;        
    }
};

struct SEventFootStep : public SEventAnim
{
    EFootStepType eFootStepType;
    EFootStepMobType eFootStepMobType;

    SEventFootStep()
    {
        eEventID = EVENT_ANIM_FOOT_STEP;
        eFootStepType = FOOT_LEFT;
        eFootStepMobType = FOOT_TYPE_NORMAL;
    }
};

struct SEventLinkEffect : public SEventAnim
{
    RwChar  chEffectName[MAX_DEFAULT_NAME];
    RwChar  chBoneName[MAX_DEFAULT_NAME];
    RwV3d   vOffsetPos;    
    RwBool  bAttachBone;

    SEventLinkEffect()
    {
        eEventID = EVENT_ANIM_LINK_EFFECT;
        ZeroMemory(chEffectName, sizeof(chEffectName));
        ZeroMemory(chBoneName, sizeof(chBoneName));
        ZeroMemory(&vOffsetPos, sizeof(vOffsetPos));        
        bAttachBone = FALSE;
    }

};

///////////////////////////////////////////////////////////////////////////
// Slow Time Event
//////////////////////////////////////////////////////////////////////////
struct SEventWeightTime : public SEventAnim
{
    RwReal fLifeTime;       ///< Slow 효과가 발휘될 시간
    RwReal fWeightValue;    ///< 얼마나 느려질지에 대한 Weight값

    SEventWeightTime()
    {
        eEventID = EVENT_ANIM_WEIGHT_TIME;
        fLifeTime = 0.0f;
        fWeightValue = 1.0f;
    }

};

struct SEventTrace : public SEventAnim
{
    /// Trace가 어디에 붙을지의 타입
    enum EAttachType
    {
        CHARACTER_BONE,     ///< 캐릭터 본에 붙는다.
        WEAPONE_BONE,       ///< 무기 본에 붙는다.
        SUB_WEAPON_BONE,    ///< 보조무기 본에 붙는다.
    };

    /// 궤적 설정이 이벤트 설정값을 따를지, Weapon 설정값을 따를지 결겅
    enum ETraceKind
    {
        EVENT_TRACE,        ///< Event의 설정값을 따른다.
        ITEM_TRACE,         ///< Item의 설정값을 따른다.
    };

    RwReal  fLifeTime;           ///< 궤적의 표시 시간
    RwReal  fEdgeLifeTime;       ///< Edge의 Life Time    
    EAttachType eAttachType;     ///< 궤적이 어디에 붙을지 
    ETraceKind  eTraceKind;      ///< 궤적이 Item 설정을 따를지
    RwChar  strStartBoneName[MAX_DEFAULT_NAME];    ///< 첫번째 Bone의 이름
    RwChar  strEndBoneName[MAX_DEFAULT_NAME];      ///< 두번째 Bone의 이름
    RwChar  strTexture[MAX_DEFAULT_NAME];          ///< Texture File Name
    RwV3d   v3dStartBoneOffset;  ///< 첫번째 Bone의 Offset
    RwV3d   v3dEndBoneOffset;    ///< 두번째 Bone의 Offset
    RwReal  fEdgeGap;             ///< 몇초마다 Edge을 생성할지의 값
    RwInt32 nSplinePointCount;   ///< Spline으로 생성하는 버텍스의 갯수 (두개의 Edge사이에 생성되는 버텍스의 개수이다)
    RwInt32 nMaxEdgeCount;       ///< 최대 Edge Count
    RwReal  fMaxLength;          ///< 궤적의 최대 길이
    RwBlendFunction eSrcBlend;  ///< Src Blend State
    RwBlendFunction eDestBlend; ///< Dest Blend State
    RwRGBA  colStartColor;       ///< 시작 Color값
    RwRGBA  colEndColor;         ///< 끝 Color값   

    SEventTrace() : fLifeTime(1.0f),
                    fEdgeLifeTime(0.3f),
                    fEdgeGap(0.03f),
                    nSplinePointCount(10),
                    nMaxEdgeCount(500),
                    fMaxLength(1.0f)
    {
        eEventID = EVENT_ANIM_TRACE_EFFECT;
        eSrcBlend = rwBLENDSRCALPHA;
        eDestBlend = rwBLENDONE;        
        eAttachType = CHARACTER_BONE;
        eTraceKind = EVENT_TRACE;        
        ZeroMemory(strStartBoneName, sizeof(strStartBoneName));
        ZeroMemory(strEndBoneName, sizeof(strEndBoneName));
        ZeroMemory(strTexture, sizeof(strTexture));
        ZeroMemory(&v3dStartBoneOffset, sizeof(RwV3d));
        ZeroMemory(&v3dEndBoneOffset, sizeof(RwV3d));
        colStartColor.red = colStartColor.green = colStartColor.blue = 255;
        colEndColor.red = colEndColor.green = colEndColor.blue = 255;
        colStartColor.alpha = 255;
        colEndColor.alpha = 0;
    }

};

/// Sub Weapon 활성화 이벤트
struct SEventSubWeapon : SEventAnim
{
    ESubWeaponActiveFlag    eSubWeaponActiveFlag;

    SEventSubWeapon() : eSubWeaponActiveFlag(SUB_WEAPON_DEACTIVE)
    {
        eEventID = EVENT_ANIM_SUB_WEAPON;
    }

};

/// Post Effect 이벤트 (화면위에 중심선 표시 이벤트)
struct SEventPostEffect : SEventAnim
{
    RwChar                   szPostEffectName[MAX_DEFAULT_NAME];          ///< 사용될 PostEffect의 이름
    EPostEffectTypeFlag      eTarget;                                     ///< PostEffect의 Target 종류
    RwV3d                    v3dOffset;                                   ///< PostEffect가 붙을 위치의 Offset값
    RwBool                   bCenterFixEnable;                            ///< 중심점 고정 기능
    RwReal                   fTargetHeight;                               ///< 대상을 타겟으로 설정한경우 높이값(바운딩박스 상대값)
    RwChar                   szPCBoneName[MAX_DEFAULT_NAME];              ///< 대상을 PC로 설정한경우 Attach할 Bone의 이름

    SEventPostEffect()
    {
        eEventID = EVENT_ANIM_POST_EFFECT;

        ZeroMemory(szPostEffectName, sizeof(szPostEffectName));
        ZeroMemory(&v3dOffset, sizeof(v3dOffset));
        ZeroMemory(szPCBoneName, sizeof(szPCBoneName));
        eTarget = POST_EFFECT_TARGET_TYPE_SELF;
        bCenterFixEnable = TRUE;
        fTargetHeight = 1.0f;
    }

};

// Summon Pet Event
struct SEventSummonPet : SEventAnim
{
	SEventSummonPet()
	{
		eEventID = EVENT_ANIM_SUMMON_PET;
	}
};

// TMQ  Event
struct SEventTMQ : SEventAnim
{
    ETMQEventType eTMQEventType;            ///< TMQ 이벤트 타입

    SEventTMQ()
    {
        eEventID = EVENT_ANIM_TMQ;
        eTMQEventType = E_TMQ_EVENT_IN;        
    }
};

// Alpha Event 
struct SEventAlpha : SEventAnim
{
    // Alpha Event Apply Type
    enum EAlphaEventType
    {
        E_ALPHA_EVENT_CLUMP,
        E_ALPHA_EVENT_ATOMIC,
    };
    //////////////////////////////////////////////////////////////////////////

    RwInt32       nStartAlpha;              ///< Fade 시작 알파값 (0~255)
    RwInt32       nDestAlpha;               ///< Fade Dest 알파값 (0~255)    
    RwReal        fFadeTime;                ///< Fade 되는 시간
    RwReal        fLifeTime;                ///< 이벤트가 유지되는 Life Time
    EAlphaEventType eAlphaEventType;        ///< 알파 이벤트 적용 타입
    BITFLAG       bfAtomicList;             ///< 알파가 적용될 아토믹들의 Index 플래그
    

    SEventAlpha()
    {
        eEventID = EVENT_ANIM_ALPHA;
        nStartAlpha = 255;
        nDestAlpha = 0;
        fFadeTime = 1.0f;
        fLifeTime = 1.0f;
        eAlphaEventType = E_ALPHA_EVENT_CLUMP;
        bfAtomicList = 0;
    }
};

// 폭발 이벤트
struct SEventExplosion : SEventAnim
{
    ENtlPLExplosionEventType  eType;

    SEventExplosion()
    {
        eEventID = EVENT_ANIM_EXPLOSION;
        eType = EXPLOSION_EVENT_TYPE_NORMAL;
    }
};

// 연출 이벤트
struct SEventDirect : SEventAnim
{
    ENtlPLDirectEventType eType;
    
    SEventDirect()
    {
        eEventID = EVENT_ANIM_DIRECT;
        eType = DIRECT_EVENT_TYPE_CAMERA_SHAKE;
    }
};

enum EColorChangeType
{
    COLOR_CHANGE_TYPE_START,
    COLOR_CHANGE_TYPE_END,
    COLOR_CHANGE_TYPE_ANIM,
};

// 칼라 체인지 이벤트
struct SEventColorChange : SEventAnim
{
    EColorChangeType eType;    
    RwRGBA           colorEdge;
    RwRGBA           colorBody;
    RwRGBA           colorAdd;

    SEventColorChange()
    {
        eEventID = EVENT_ANIM_COLOR_CHANGE;
        eType = COLOR_CHANGE_TYPE_START;
        colorEdge.red = colorEdge.green = colorEdge.blue = 0;
        colorBody.red = colorBody.green = colorBody.blue = 255;
        colorAdd.red = colorAdd.green = colorAdd.blue = 0;
        colorEdge.alpha = colorBody.alpha = colorAdd.alpha = 255;
    }
};

/// 늘어날 BONE에 대한 정보
struct StretchBoneInfo
{
    RwChar szBoneName[MAX_DEFAULT_NAME];
    RwReal fStretchSpeed;
    RwReal fWidth;

    StretchBoneInfo()
    {
        ZeroMemory(szBoneName, sizeof(szBoneName));
        fStretchSpeed = 100.0f;
        fWidth = 1.0f;
    }
};

/// 스트레치 이벤트 타입
enum EStretchEventType
{
    E_STRETCH_PULLING,              ///< 풀링 (마신완)
    E_STRETCH_HIT,                  ///< 히트 (서프라이즈 펀치)
};

/// BONE을 늘리는 이벤트
struct SEventStretch : SEventAnim
{
    EStretchEventType eType;
    RwChar          szBoneName[2][MAX_DEFAULT_NAME];
    RwReal          fStretchSpeed;
    RwReal          fWidth;    
    RwReal          fAccel;
    RwChar          szScaleBone[MAX_DEFAULT_NAME];
    RwReal          fScaleSize;
    RwChar          szAxisBone[MAX_DEFAULT_NAME];
    RwChar          szTargetEffect[MAX_DEFAULT_NAME];    

    SEventStretch()
    {
        eEventID = EVENT_ANIM_STRETCH;                        
        fScaleSize = 1.0f;
        fStretchSpeed = 10.0f;
        fWidth = 1.0f;
        fAccel = 25.0f;
        sprintf_s(szBoneName[0], "Bip01 L UpperArm");
        sprintf_s(szBoneName[1], "Bip01 L Forearm");
        sprintf_s(szScaleBone, "Bip01 L Hand");
        sprintf_s(szAxisBone, "Bip01 L Clavicle");
        ZeroMemory(szTargetEffect, sizeof(szTargetEffect));
        eType = E_STRETCH_PULLING;
    }
};

/// 스킬등에서 사용되는 트리거 이벤트
struct SEventTrigger : SEventAnim
{
    SEventTrigger()
    {
        eEventID = EVENT_ANIM_TRIGGER;
    }
};

/// 스킬 캔슬에 사용되는 이벤트
struct SEventSkillCancel : SEventAnim
{
    SEventSkillCancel()
    {
        eEventID = EVENT_ANIM_SKILL_CANCEL;
    }
};

/// 각 이벤트 구조체의 사이즈를 반환한하는 함수
static RwUInt32 GetAnimEventSize(EAnimEventType eType)
{
    switch(eType)
    {
    case EVENT_ANIM_HIT:
        return sizeof(SEventAnimHit);
    case EVENT_ANIM_VISUAL_EFFECT:
        return sizeof(SEventVisualEffect);
    case EVENT_ANIM_VISUAL_SOUND:
        return sizeof(SEventSound);
    case EVENT_ANIM_FOOT_STEP:
        return sizeof(SEventFootStep);
    case EVENT_ANIM_LINK_EFFECT:
        return sizeof(SEventLinkEffect);
    case EVENT_ANIM_WEIGHT_TIME:
        return sizeof(SEventWeightTime);
    case EVENT_ANIM_TRACE_EFFECT:
        return sizeof(SEventTrace);
    case EVENT_ANIM_SUB_WEAPON:
        return sizeof(SEventSubWeapon);
    case EVENT_ANIM_POST_EFFECT:
        return sizeof(SEventPostEffect);
    case EVENT_ANIM_SUMMON_PET:
        return sizeof(SEventSummonPet);
    case EVENT_ANIM_TMQ:
        return sizeof(SEventTMQ);
    case EVENT_ANIM_ALPHA:
        return sizeof(SEventAlpha);
    case EVENT_ANIM_EXPLOSION:
        return sizeof(SEventExplosion);
    case EVENT_ANIM_DIRECT:
        return sizeof(SEventDirect);
    case EVENT_ANIM_COLOR_CHANGE:
        return sizeof(SEventColorChange);
    case EVENT_ANIM_STRETCH:
        return sizeof(SEventStretch);
    case EVENT_ANIM_TRIGGER:
        return sizeof(SEventTrigger);
    case EVENT_ANIM_SKILL_CANCEL:
        return sizeof(SEventSkillCancel);        
    }

    return 0;
}


#endif
