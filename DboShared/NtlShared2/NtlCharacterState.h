//***********************************************************************************
//
//	File		:	NtlCharacterState.h
//
//	Begin		:	2006-03-31
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlBitFlag.h"
#include "NtlSharedType.h"
#include "NtlVector.h"
#include "NtlSkill.h"
#include "NtlItem.h"
#include "NtlMovement.h"

//-----------------------------------------------------------------------------------
// Movement ( 캐릭터 이동 ) : 위치이동
//-----------------------------------------------------------------------------------
enum eMOVESTATE 
{
	MOVE_DISABLE,		// Move Disable
	MOVE_STOP,			// Stop
	MOVE_JUMP,			// Jump
	MOVE_F,				// Forward
	MOVE_B,				// Backward
	MOVE_L,				// Left
	MOVE_R,				// Right
	MOVE_TURN_L,		// Turning Left
	MOVE_TURN_R,		// Turning Right
	MOVE_RUN,			// Walk / Run
	MOVE_DASH,			// Dash
	MOVE_DESTMOVE,		// DestMove ( = Mouse Move )

	MAX_MOVESTATE
};


//-----------------------------------------------------------------------------------
// Movement ( 캐릭터 이동 ) : 위치이동 플래그
//-----------------------------------------------------------------------------------
enum eMOVE_FLAG
{
	MOVE_FLAG_DISABLE	= MAKE_BIT_FLAG( MOVE_DISABLE ),	// Move Disable
	MOVE_FLAG_STOP		= MAKE_BIT_FLAG( MOVE_STOP ),		// Stop
	MOVE_FLAG_JUMP		= MAKE_BIT_FLAG( MOVE_JUMP ),		// Jump
	MOVE_FLAG_F			= MAKE_BIT_FLAG( MOVE_F ),			// Forward
	MOVE_FLAG_B			= MAKE_BIT_FLAG( MOVE_B ),			// Backward
	MOVE_FLAG_L			= MAKE_BIT_FLAG( MOVE_L ),			// Left
	MOVE_FLAG_R			= MAKE_BIT_FLAG( MOVE_R ),			// Right
	MOVE_FLAG_TURN_L	= MAKE_BIT_FLAG( MOVE_TURN_L ),	// Turning Left
	MOVE_FLAG_TURN_R	= MAKE_BIT_FLAG( MOVE_TURN_R ),	// Turning Right
	MOVE_FLAG_RUN		= MAKE_BIT_FLAG( MOVE_RUN ),		// Walk / Run
	MOVE_FLAG_DASH		= MAKE_BIT_FLAG( MOVE_DASH ),		// Dash
	MOVE_FLAG_DESTMOVE	= MAKE_BIT_FLAG( MOVE_DESTMOVE ),	// DestMove ( = Mouse Move )

	MOVE_FLAG_F_L		= MOVE_FLAG_F & MOVE_FLAG_L,		// Forward + Left
	MOVE_FLAG_F_R		= MOVE_FLAG_F & MOVE_FLAG_R,		// Forward + Right
	MOVE_FLAG_B_L		= MOVE_FLAG_B & MOVE_FLAG_L,		// Backward + Left
	MOVE_FLAG_B_R		= MOVE_FLAG_B & MOVE_FLAG_R,		// Backward + Right
	MOVE_FLAG_F_TURN_L	= MOVE_FLAG_F & MOVE_FLAG_TURN_L,	// Forward + Turning Left
	MOVE_FLAG_F_TURN_R	= MOVE_FLAG_F & MOVE_FLAG_TURN_R,	// Forward + Turning Right
	MOVE_FLAG_B_TURN_L	= MOVE_FLAG_B & MOVE_FLAG_TURN_L,	// Backward + Turning Left
	MOVE_FLAG_B_TURN_R	= MOVE_FLAG_B & MOVE_FLAG_TURN_R,	// Backward + Turning Right
};


//-----------------------------------------------------------------------------------
// Character State
//-----------------------------------------------------------------------------------
enum eCHARSTATE
{
	CHARSTATE_SPAWNING,				// 스폰중 ( 캐릭터가 생성 될때 )
	CHARSTATE_DESPAWNING,			// 디스폰중 ( 캐릭터가 삭제 될때 )
	CHARSTATE_STANDING,				// 서있음
	CHARSTATE_SITTING,				// 앉아 있음
	CHARSTATE_FAINTING,				// 기절해 있음
	CHARSTATE_CAMPING,				// 종료 대기중
	CHARSTATE_LEAVING,				// 종료 중

	CHARSTATE_MOVING,				// 일반적인 필드 이동 ( RUNNING, WALKING, JUMPING, DASHING 포함 )
	CHARSTATE_DESTMOVE,				// 목적지 이동
	CHARSTATE_FOLLOWING,			// 따라가기 이동
	CHARSTATE_FALLING,				// 떨어지는 중
	CHARSTATE_DASH_PASSIVE,			// Passive dash 이동
	CHARSTATE_TELEPORTING,			// 텔레포트중 ( 캐릭터가 워프 등으로 이동중 )
	CHARSTATE_SLIDING,				// 슬라이딩 되고 있음
	CHARSTATE_KNOCKDOWN,			// 넉다운 되고 있음

	CHARSTATE_FOCUSING,				// 집중 상태(RP 보너스 사용 시 초기 단계)
	CHARSTATE_CASTING,				// 캐스팅중
	CHARSTATE_SKILL_AFFECTING,		// 스킬 적용중
	CHARSTATE_KEEPING_EFFECT,		// 스킬/아이템 효과 유지 상태
	CHARSTATE_CASTING_ITEM,			// 아이템 캐스팅 중

	CHARSTATE_STUNNED,				// Stun 상태
	CHARSTATE_SLEEPING,				// 자고 있음
	CHARSTATE_PARALYZED,			// 마비 상태

	CHARSTATE_HTB,					// HTB를 실행하고 있음
	CHARSTATE_SANDBAG,				// HTB를 당하고 있음
	CHARSTATE_CHARGING,				// 기모으는중
	CHARSTATE_GUARD,				// 가드 모드 상태

	CHARSTATE_PRIVATESHOP,			// 개인상점 [7/16/2007 SGpro]
	CHARSTATE_DIRECT_PLAY,			// 캐릭터 연출 상태
	CHARSTATE_OPERATING,			// 오브젝트 동작 상태
	CHARSTATE_RIDEON,				// 탈것에 타있는 상태
	CHARSTATE_TURNING,				// 회전중
	//CHARSTATE_CHASING,				// 추적중
	//CHARSTATE_STONING,			// 돌로 되어 있음
	//CHARSTATE_SPRAWLING,			// 누워 있음
	//CHARSTATE_FALLING,			// 미끄러지고있음
	//CHARSTATE_SWIMMING,			// 수영하고 있음
	//CHARSTATE_RUNNING,			// 달리고 있음
	//CHARSTATE_WALKING,			// 걷고 있음
	//CHARSTATE_JUMPING,			// 점프하고 있음
	//CHARSTATE_HOVERING,			// 부양하고 있음
	//CHARSTATE_FLYING,				// 날고 있음
	//CHARSTATE_CANDYING,			// 캔디로 되어 있음
	//CHARSTATE_FIGHTING,				// 전투중

	//CHARSTATE_CRAFTING,			// 생산중
	//CHARSTATE_LOOTING,			// 루팅중
	//CHARSTATE_TRADING,			// 기모으는중
	//CHARSTATE_SHOPPING,			// 쇼핑중

	CHARSTATE_COUNT,
	INVALID_CHARSTATE = 0xFF

};


//-----------------------------------------------------------------------------------
// Character State Flag
//-----------------------------------------------------------------------------------
enum eCHARSTATE_FLAG
{
	CHARSTATE_FLAG_SPAWNING			= MAKE_BIT_FLAG( CHARSTATE_SPAWNING ),				// 스폰중 ( 캐릭터가 월드에 처음 생성 )
	CHARSTATE_FLAG_DESPAWNING		= MAKE_BIT_FLAG( CHARSTATE_DESPAWNING ),			// 디스폰중
	CHARSTATE_FLAG_STANDING			= MAKE_BIT_FLAG( CHARSTATE_STANDING ),				// 서있음
	CHARSTATE_FLAG_SITTING			= MAKE_BIT_FLAG( CHARSTATE_SITTING ),				// 앉아 있음
	CHARSTATE_FLAG_FAINTING			= MAKE_BIT_FLAG( CHARSTATE_FAINTING ),				// 기절해 있음
	CHARSTATE_FLAG_CAMPING			= MAKE_BIT_FLAG( CHARSTATE_CAMPING ),				// 종료 대기중

	CHARSTATE_FLAG_MOVING			= MAKE_BIT_FLAG( CHARSTATE_MOVING ),				// 일반적인 필드 이동 ( RUNNING, WALKING, JUMPING, DASHING 포함 )
	CHARSTATE_FLAG_DESTMOVE			= MAKE_BIT_FLAG( CHARSTATE_DESTMOVE ),				// 목적지 이동
	CHARSTATE_FLAG_FOLLOWING		= MAKE_BIT_FLAG( CHARSTATE_FOLLOWING ),			// 따라가기 이동
	CHARSTATE_FLAG_FALLING			= MAKE_BIT_FLAG( CHARSTATE_FALLING ),				// 떨어지는 중
	CHARSTATE_FLAG_DASH_PASSIVE		= MAKE_BIT_FLAG( CHARSTATE_DASH_PASSIVE ),			// Passive dash 이동
	CHARSTATE_FLAG_TELEPORTING		= MAKE_BIT_FLAG( CHARSTATE_TELEPORTING ),			// 텔레포트중 ( 캐릭터가 워프 등으로 이동하여 생성중 )
	CHARSTATE_FLAG_SLIDING			= MAKE_BIT_FLAG( CHARSTATE_SLIDING ),				// 슬라이딩 되고 있음
	CHARSTATE_FLAG_KNOCKDOWN		= MAKE_BIT_FLAG( CHARSTATE_KNOCKDOWN ),			// 넉다운 되고 있음

	CHARSTATE_FLAG_FOCUSING			= MAKE_BIT_FLAG( CHARSTATE_FOCUSING ),				// 집중 상태(RP 보너스 사용 시 초기 단계)
	CHARSTATE_FLAG_CASTING			= MAKE_BIT_FLAG( CHARSTATE_CASTING ),				// 캐스팅중
	CHARSTATE_FLAG_SKILL_AFFECTING	= MAKE_BIT_FLAG( CHARSTATE_SKILL_AFFECTING ),		// 스킬 적용중
	CHARSTATE_FLAG_KEEPING_EFFECT	= MAKE_BIT_FLAG( CHARSTATE_KEEPING_EFFECT ),		// 스킬/아이템 효과 유지 상태
	CHARSTATE_FLAG_CASTING_ITEM		= MAKE_BIT_FLAG( CHARSTATE_CASTING_ITEM ),			// 아이템 캐스팅 중

	CHARSTATE_FLAG_STUNNED			= MAKE_BIT_FLAG( CHARSTATE_STUNNED ),				// Stun 상태
	CHARSTATE_FLAG_SLEEPING			= MAKE_BIT_FLAG( CHARSTATE_SLEEPING ),				// 자고 있음
	CHARSTATE_FLAG_PARALYZED		= MAKE_BIT_FLAG( CHARSTATE_PARALYZED ),				// 마비 상태

	CHARSTATE_FLAG_HTB				= MAKE_BIT_FLAG( CHARSTATE_HTB ),					// HTB를 실행하고 있음
	CHARSTATE_FLAG_SANDBAG			= MAKE_BIT_FLAG( CHARSTATE_SANDBAG ),				// HTB를 당하고 있음
	CHARSTATE_FLAG_CHARGING			= MAKE_BIT_FLAG( CHARSTATE_CHARGING ),				// 기모으는중
	CHARSTATE_FLAG_GUARD			= MAKE_BIT_FLAG( CHARSTATE_GUARD ),					// 가드모드 상태

	CHARSTATE_FLAG_PRIVATESHOP		= MAKE_BIT_FLAG( CHARSTATE_PRIVATESHOP ),			// 개인상점 [7/16/2007 SGpro]
	CHARSTATE_FLAG_DIRECT_PLAY		= MAKE_BIT_FLAG( CHARSTATE_DIRECT_PLAY ),			// 개인상점 [7/16/2007 SGpro]
	CHARSTATE_FLAG_OPERATING		= MAKE_BIT_FLAG( CHARSTATE_OPERATING ),				// 개인상점 [7/16/2007 SGpro]
	CHARSTATE_FLAG_RIDEON			= MAKE_BIT_FLAG( CHARSTATE_RIDEON ),
	CHARSTATE_FLAG_TURNING			= MAKE_BIT_FLAG( CHARSTATE_TURNING ),

	CHARSTATE_FLAG_ALL				= 0xFFFFFFFF >> ( 32 - CHARSTATE_COUNT )
};


//-----------------------------------------------------------------------------------
// Aspect ( 캐릭터 특이 상태 : 변신등 )중복불가 개념
//-----------------------------------------------------------------------------------
enum eASPECTSTATE
{
	ASPECTSTATE_SUPER_SAIYAN,		// 수퍼 사이어인
	ASPECTSTATE_PURE_MAJIN,			// 순수 마인
	ASPECTSTATE_GREAT_NAMEK,		// 그레이트 나메크인
	ASPECTSTATE_KAIOKEN,				// 계왕권
	ASPECTSTATE_SPINNING_ATTACK,		// 회전 공격
	ASPECTSTATE_VEHICLE,			// 탈 것

	ASPECTSTATE_COUNT,
	ASPECTSTATE_INVALID = 0xFF
};


//-----------------------------------------------------------------------------------
// Condition, 중복가능 개념
//-----------------------------------------------------------------------------------
enum eCHARCONDITION
{
	CHARCOND_INVISIBLE,				// 보이지 않음
	CHARCOND_HIDING_KI,				// 기 숨기기
	CHARCOND_INVINCIBLE,			// 무적
	CHARCOND_TAUNT,					// 타겟고정
	CHARCOND_ATTACK_DISALLOW,		// 공격불가
	CHARCOND_TRANSPARENT,			// 투명 (GM용)
	CHARCOND_CANT_BE_TARGETTED,		// 타겟되지 않음
	CHARCOND_DIRECT_PLAY,			// 연출중 ( PC : 클라이언트 연출일때, NPC or MOB : 서버 연출일 때 필요에 따라 )

	CHARCOND_BLEEDING,				// 출혈
	CHARCOND_POISON,				// 독
	CHARCOND_STOMACHACHE,			// 복통
	CHARCOND_CONFUSED,				// 혼란
	CHARCOND_TERROR,				// 공포
	CHARCOND_BARRIER,				// 데미지 무력화
	CHARCOND_DAMAGE_REFLECTION,		// 데미지 반사
	//CHARCOND_SLOW,				// 느려짐

	CHARCOND_AFTEREFFECT,			// 변신 후유증
	CHARCOND_CHARGING_BLOCKED,		// 기 모으기 불가
	CHARCOND_FAKE_DEATH,			// 죽은 척하기
	CHARCOND_NULLIFIED_DAMAGE,		// 데미지를 무효화시키기

	CHARCOND_MOB_ROLE,				// Mob 역할 중인 NPC

	CHARCOND_CLICK_DISABLE,			// 캐릭터 클릭 금지
	CHARCOND_CLIENT_UI_DISABLE,		// 클라이언트측의 캐릭터 관련 ui 출력 금지 ( 메뉴 등 )

	CHARCONDITION_COUNT,
	INVALID_CHARCONDITION = 0xFF,

	CHARCOND_FIRST = CHARCOND_INVISIBLE,
	CHARCOND_LAST = CHARCONDITION_COUNT - 1,
};


//-----------------------------------------------------------------------------------
// Condition Flag, 중복가능 개념
//-----------------------------------------------------------------------------------
enum eCHARCONDITION_FLAG
{
	CHARCOND_FLAG_INVISIBLE				= MAKE_BIT_FLAG( CHARCOND_INVISIBLE ),				// 보이지 않음
	CHARCOND_FLAG_HIDING_KI				= MAKE_BIT_FLAG( CHARCOND_HIDING_KI ),				// 기 숨기기
	CHARCOND_FLAG_INVINCIBLE			= MAKE_BIT_FLAG( CHARCOND_INVINCIBLE ),				// 무적
	CHARCOND_FLAG_TAUNT					= MAKE_BIT_FLAG( CHARCOND_TAUNT ),					// 타겟고정
	CHARCOND_FLAG_ATTACK_DISALLOW		= MAKE_BIT_FLAG( CHARCOND_ATTACK_DISALLOW ),		// 공격불가
	CHARCOND_FLAG_TRANSPARENT			= MAKE_BIT_FLAG( CHARCOND_TRANSPARENT ),			// 투명 (GM용)
	CHARCOND_FLAG_CANT_BE_TARGETTED		= MAKE_BIT_FLAG( CHARCOND_CANT_BE_TARGETTED ),		// 타겟되지 않음
	CHARCOND_FLAG_DIRECT_PLAY			= MAKE_BIT_FLAG( CHARCOND_DIRECT_PLAY ),			// 연출중

	CHARCOND_FLAG_BLEEDING				= MAKE_BIT_FLAG( CHARCOND_BLEEDING ),				// 출혈
	CHARCOND_FLAG_POISON				= MAKE_BIT_FLAG( CHARCOND_POISON ),					// 독
	CHARCOND_FLAG_STOMACHACHE			= MAKE_BIT_FLAG( CHARCOND_STOMACHACHE ),			// 복통
	CHARCOND_FLAG_CONFUSED				= MAKE_BIT_FLAG( CHARCOND_CONFUSED ),				// 혼란
	CHARCOND_FLAG_TERROR				= MAKE_BIT_FLAG( CHARCOND_TERROR ),					// 공포
	CHARCOND_FLAG_BARRIER				= MAKE_BIT_FLAG( CHARCOND_BARRIER ),				// 데미지 무력화
	CHARCOND_FLAG_DAMAGE_REFLECTION		= MAKE_BIT_FLAG( CHARCOND_DAMAGE_REFLECTION ),		// 데미지 반사
//	CHARCOND_FLAG_SLOW					= MAKE_BIT_FLAG( CHARCOND_SLOW ),					// 느려짐

	CHARCOND_FLAG_AFTEREFFECT			= MAKE_BIT_FLAG( CHARCOND_AFTEREFFECT ),			// 변신 후유증
	CHARCOND_FLAG_CHARGING_BLOCKED		= MAKE_BIT_FLAG( CHARCOND_CHARGING_BLOCKED ),		// 기 모으기 불가
	CHARCOND_FLAG_FAKE_DEATH			= MAKE_BIT_FLAG( CHARCOND_FAKE_DEATH ),				// 죽은 척하기
	CHARCOND_FLAG_NULLIFIED_DAMAGE		= MAKE_BIT_FLAG( CHARCOND_NULLIFIED_DAMAGE ),		// 데미지를 무효화시키기

	CHARCOND_FLAG_MOB_ROLE				= MAKE_BIT_FLAG( CHARCOND_MOB_ROLE ),				// Mob 역할 중인 NPC

	CHARCOND_FLAG_CLICK_DISABLE			= MAKE_BIT_FLAG( CHARCOND_CLICK_DISABLE ),			// 이 플래그가 켜진 객체 선택 금지
	CHARCOND_FLAG_CLIENT_UI_DISABLE		= MAKE_BIT_FLAG( CHARCOND_CLIENT_UI_DISABLE ),		// 이 플래그가 켜진 객체 UI 출력 금지
};


//-----------------------------------------------------------------------------------
// Condition Flag, 중복가능 개념
//-----------------------------------------------------------------------------------
enum eCHARLEAVING_TYPE
{
	CHARLEAVING_DISCONNECT,			// 접속이 끈어짐
	CHARLEAVING_SERVER_CHANGE,		// 다른 게임 서버로의 이동
	CHARLEAVING_CHANNEL_CHANGE,		// 다른 게임 채널로의 이동
	CHARLEAVING_CHARACTER_EXIT,		// 캐릭터 종료 : Lobby로 이동
	CHARLEAVING_GAME_EXIT,			// ACCOUNT 종료 : 게임 종료
	CHARLEAVING_GAME_KICK,			// 시스템에 의한 종료(KICK 등)

	INVALID_CHARLEAVING = INVALID_BYTE,
};


//-----------------------------------------------------------------------------------
// 상태 관련 함수 정의 : 
//-----------------------------------------------------------------------------------
const char *				NtlGetCharStateString(BYTE byStateID);

const char *				NtlGetAspectStateString(BYTE byStateID);

const char *				NtlGetConditionStateString(BYTE byStateID);



//-----------------------------------------------------------------------------------
// 상태 관련 구조체
//-----------------------------------------------------------------------------------
#pragma pack(1)

//-----------------------------------------------------------------------------------
struct sCHARSTATE_SPAWNING
{
	BYTE								byTeleportType;		// eTELEPORT_TYPE
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_DESPAWNING
{
	BYTE								byTeleportType;		// eTELEPORT_TYPE
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_STANDING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_SITTING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_FAINTING
{
	BYTE			byReason; // eFAINT_REASON
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_CAMPING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_LEAVING
{
	BYTE			byLeavingType;	// eCHARLEAVING_TYPE
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_MOVING
{
	DWORD			dwTimeStamp;
	BYTE			byMoveFlag;
	BYTE			byMoveDirection;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_DESTMOVE
{
	DWORD			dwTimeStamp;
	BYTE			byMoveFlag;
	bool			bHaveSecondDestLoc;
	sVECTOR3		vSecondDestLoc;	// 목적지 이동일때 목적지 다음 좌표
	BYTE			byDestLocCount;
	sVECTOR3		avDestLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_FOLLOWING
{
	DWORD			dwTimeStamp;
	BYTE			byMoveFlag;
	HOBJECT			hTarget; // 타겟 따라가기일때 따라갈 타겟 핸들
	float			fDistance; // 타겟 앞에서 멈출 거리
	BYTE			byMovementReason; // 공격 or 스킬사용 등의 FOLLOW의 이유를 설정
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_FALLING
{
	BYTE			byMoveDirection;		// ENtlMovementDirection
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_DASH_PASSIVE
{
	DWORD			dwTimeStamp;
	BYTE			byMoveDirection;		// NTL_MOVE_DASH_F, NTL_MOVE_DASH_B and so on
	sVECTOR3		vDestLoc;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_TELEPORTING
{
	BYTE								byTeleportType;		// eTELEPORT_TYPE
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_SLIDING
{
	sVECTOR3		vShift;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_KNOCKDOWN
{
	sVECTOR3		vShift;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_FOCUSING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_CASTING
{
	TBLIDX			skillId;
	HOBJECT			hTarget;
	DWORD			dwCastingTime;
	DWORD			dwCastingTimeRemaining;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_SKILL_AFFECTING
{
	TBLIDX			skillId;
	HOBJECT			hTarget;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_KEEPING_EFFECT
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_CASTING_ITEM
{
	TBLIDX			itemTblidx;
	HOBJECT			hTarget;
	DWORD			dwCastingTime;
	DWORD			dwCastingTimeRemaining;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_STUNNED
{
	BYTE			byStunType;		// eDBO_STUN_TYPE
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_SLEEPING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_PARALYZED
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_HTB
{
	TBLIDX				HTBId;			// HTB 스킬 ID
	HOBJECT				hTarget;		// HTB 타겟 핸들
	BYTE				byStepCount;	// HTB 스텝 개수
	BYTE				byCurStep;		// 현재 스텝
	BYTE				byResultCount;	// HTB 스킬 결과 개수
	sHTBSKILL_RESULT	aHTBSkillResult[NTL_HTB_MAX_SKILL_COUNT_IN_SET]; // HTB 스킬 결과
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_SANDBAG
{
	
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_CHARGING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_BLOCKING
{
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_PRIVATESHOP
{
	sSUMMARY_PRIVATESHOP_SHOP_DATA		sSummaryPrivateShopData;
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_DIRECT_PLAY
{
	BYTE				byDirectPlayType; // 연출 타입 ( eDIRECT_PLAY_TYPE )
	TBLIDX				directTblidx; // 연출 TBLIDX
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_OPERATING
{
	HOBJECT				hTargetObject; // 타겟 오브젝트 핸들
	DWORD				dwOperateTime; // 동작 시간
	TBLIDX				directTblidx; // 연출 TBLIDX
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_RIDEON
{
	HOBJECT				hTarget; // 타겟 핸들
};
//-----------------------------------------------------------------------------------
struct sCHARSTATE_TURNING
{
	TBLIDX				directTblidx; // 연출 TBLIDX
	sVECTOR3			vDestDir;
};
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
struct sASPECTSTATE_SUPER_SAIYAN
{
	BYTE				bySourceGrade;
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_PURE_MAJIN
{
	BYTE				bySourceGrade;
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_GREAT_NAMEK
{
	BYTE				bySourceGrade;
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_KAIOKEN
{
	BYTE				bySourceGrade;
	BYTE				byRepeatingCount;
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_SPINNING_ATTACK
{
};
//-----------------------------------------------------------------------------------
struct sASPECTSTATE_VEHICLE
{
	//HOBJECT	hVehicleItem;
	TBLIDX		idVehicleTblidx;
	bool		bIsEngineOn;
};
//-----------------------------------------------------------------------------------
#pragma pack()