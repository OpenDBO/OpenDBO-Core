#pragma once

#include "NtlItem.h"
#include "NtlSharedDef.h"
#include "NtlCharacterState.h"
#include "NtlAvatar.h"
#include "NtlBitFlag.h"
#include "NtlGuild.h"
#include "NtlActionPattern.h"

#define DBO_RP_CHARGE_INTERVAL_IN_MILLISECS			(Dbo_GetRpChargeIntervalInMilliSecs(CFormulaTable::m_afRate[71][4]))
#define DBO_RP_DIMIMUTION_INTERVAL_IN_MILLISECS		(Dbo_GetRpDiminutionIntervalInMilliSecs(CFormulaTable::m_afRate[72][4]))
#define DBO_BATTLE_REGEN_DURATION_IN_MILLISECS		(Dbo_GetRpDiminutionIntervalInMilliSecs(CFormulaTable::m_afRate[603][1]))

enum eRETURNMODE
{
	 eNORMAL_WALK
	,eNORMAL_RUN
	,eCOMEBACK_RUN
	,eINVALID_RETURNMODE
};

enum eBOTROLETYPE // [11/10/2008]
{
	 BOTROLE_NPC = 0
	,BOTROLE_MOB

	,INVALID_BOTROLE_TYPE = -1
};

enum eBOTPARAMETERSCHANGE_TYPE // [11/6/2008]
{
	BOTPARAMTYPE_NICNAME,
	BOTPARAMTYPE_ROLECHANGE,

	INVALID_BOTPARAMETERSCHANGE_TYPE = -1,
};

enum eDEL_TYPE
{
	DEL_TYPE_NONE,
	DEL_TYPE_DONE,
	DEL_TYPE_WAIT,
	
};
// 종족 정의
enum eRACE
{
	RACE_HUMAN,
	RACE_NAMEK,
	RACE_MAJIN,

	RACE_COUNT,
	RACE_UNKNOWN	= 0xFF,

	RACE_FIRST		= RACE_HUMAN,
	RACE_LAST		= RACE_MAJIN
};

// 종족 정의 플래그
enum eRACE_FLAG
{
	RACE_HUMAN_FLAG = MAKE_BIT_FLAG( RACE_HUMAN ),
	RACE_NAMEK_FLAG	= MAKE_BIT_FLAG( RACE_NAMEK ),
	RACE_MAJIN_FLAG	= MAKE_BIT_FLAG( RACE_MAJIN ),
};

enum eGENDER
{
	GENDER_MALE,
	GENDER_FEMALE,
	GENDER_ONE_SEX,

	GENDER_COUNT,
	GENDER_UNKNOWN	= 0xFF,

	GENDER_FIRST	= GENDER_MALE,
	GENDER_LAST		= GENDER_ONE_SEX
};

enum eZENNY_CHANGE_TYPE
{
	ZENNY_CHANGE_TYPE_ITEM_BUY	=	0,
	ZENNY_CHANGE_TYPE_ITEM_SELL,
	ZENNY_CHANGE_TYPE_PICK,
	ZENNY_CHANGE_TYPE_TRADE,
	ZENNY_CHANGE_TYPE_REPAIR,
	ZENNY_CHANGE_TYPE_REWARD,
	ZENNY_CHANGE_TYPE_SKILL,
	ZENNY_CHANGE_TYPE_PARTY_VICTIM_ADD,
	ZENNY_CHANGE_TYPE_PARTY_VICTIM_DEL,
	ZENNY_CHANGE_TYPE_PARTY_VICTIM_RETURN,
	ZENNY_CHANGE_TYPE_CHEAT,
	ZENNY_CHANGE_TYPE_BANK_BUY,
	ZENNY_CHANGE_TYPE_PARTY_PICK,
	ZENNY_CHANGE_TYPE_GUILD_CREATE,
	ZENNY_CHANGE_TYPE_DB_REWARD,
	ZENNY_CHANGE_TYPE_PRIVATESHOP_ITEM_BUY,// [7/20/2007]
	ZENNY_CHANGE_TYPE_MAIL_SEND,
	ZENNY_CHANGE_TYPE_MAIL_RECV,
	ZENNY_CHANGE_TYPE_RANKBATTLE,			// RankBattle 대전료
	ZENNY_CHANGE_TYPE_PORTAL_ADD,

	ZENNY_CHANGE_TYPE_GUILD_FUNCTION_ADD,
	ZENNY_CHANGE_TYPE_GUILD_GIVE_ZENNY,		// 길드 포인트를 올리기위한 제니
	ZENNY_CHANGE_TYPE_GUILD_MARK_CHANGE,
	ZENNY_CHANGE_TYPE_GUILD_NAME_CHANGE,
	ZENNY_CHANGE_TYPE_RIDE_ON_BUS,
	ZENNY_CHANGE_TYPE_ITEM_IDENTIFY,
	ZENNY_CHANGE_TYPE_SCOUTER_ITEM_SELL,
	ZENNY_CHANGE_TYPE_PARTY_ITEM_INVEST,	// 파티인벤 아이템 경매
	ZENNY_CHANGE_TYPE_ITEM_MIX_MAKE,		// 아이템 만드는데 소모
	ZENNY_CHANGE_TYPE_ITEM_MIX_FARE,		// 아이템 만들기로 획득
	ZENNY_CHANGE_TYPE_DOJO_CHANGE,
	ZENNY_CHANGE_TYPE_BANK,					// 창고 입출금
	ZENNY_CHANGE_TYPE_GUILD_BANK,			// 길드 창고 입출금
	ZENNY_CHANGE_TYPE_DOJO_SCRAMBLE_REQ,	// 도장쟁탈전신청금
	ZENNY_CHANGE_TYPE_DOJO_ANTI_SCRAMBLE_ADD,	// 도장쟁탈전거부금 받기
	ZENNY_CHANGE_TYPE_DOJO_ANTI_SCRAMBLE_DEL,	// 도장쟁탈전거부금 보내기
	ZENNY_CHANGE_TYPE_ITEM_RECIPE_JOB_SET,  // 제조기술을 배우기 위해 소모됨

	ZENNY_CHANGE_TYPE_COUNT,
	ZENNY_CHANGE_TYPE_INVALID = 0xFF
};

enum ePC_CLASS
{
	PC_CLASS_HUMAN_FIGHTER,  //(무도가)->(격투가,검술가)
	PC_CLASS_HUMAN_MYSTIC,   //(기공사)->(학선사,귀선사)
	PC_CLASS_HUMAN_ENGINEER, //(엔지니어)->(건매니아,메카메니아)
	PC_CLASS_NAMEK_FIGHTER,  //(전사)->(마계전사,마도전사)
	PC_CLASS_NAMEK_MYSTIC,   //(용족)->(덴덴도사,포코도사)
	PC_CLASS_MIGHTY_MAJIN,   //(대마인)->(그랜마,얼티마)
	PC_CLASS_WONDER_MAJIN,   //(의마인)->(플라즈마,카르마)
	PC_CLASS_STREET_FIGHTER, //(격투가)
	PC_CLASS_SWORD_MASTER,   //(검술가)
	PC_CLASS_CRANE_ROSHI,    //(학선사)
	PC_CLASS_TURTLE_ROSHI,   //(귀선사)
	PC_CLASS_GUN_MANIA,      //(건매니아)
	PC_CLASS_MECH_MANIA,     //(메카매니아)
	PC_CLASS_DARK_WARRIOR,   //(마계전사)
	PC_CLASS_SHADOW_KNIGHT,  //(마도전사)
	PC_CLASS_DENDEN_HEALER,  //(덴덴도사)
	PC_CLASS_POCO_SUMMONER,  //(포코도사)
	PC_CLASS_ULTI_MA,        //(얼티마)
	PC_CLASS_GRAND_MA,       //(그랜마)
	PC_CLASS_PLAS_MA,        //(플라즈마)
	PC_CLASS_KAR_MA,         //(카르마)

	PC_CLASS_COUNT,
	PC_CLASS_UNKNOWN				= 0xFF,
	
	PC_CLASS_1_FIRST = PC_CLASS_HUMAN_FIGHTER,
	PC_CLASS_1_LAST  = PC_CLASS_WONDER_MAJIN,
	PC_CLASS_2_FIRST = PC_CLASS_STREET_FIGHTER,
	PC_CLASS_2_LAST  = PC_CLASS_KAR_MA,

	PC_CLASS_FIRST = PC_CLASS_HUMAN_FIGHTER,
	PC_CLASS_LAST  = PC_CLASS_COUNT - 1
};

// Class 검사용 비트 플래그
enum ePC_CLASS_FLAG
{
	PC_CLASS_FLAG_HUMAN_FIGHTER  = (0x01 << PC_CLASS_HUMAN_FIGHTER),								//(무도가)->(격투가,검술가)
	PC_CLASS_FLAG_HUMAN_MYSTIC   = (0x01 << PC_CLASS_HUMAN_MYSTIC),									//(기공사)->(학선사,귀선사)
	PC_CLASS_FLAG_HUMAN_ENGINEER = (0x01 << PC_CLASS_HUMAN_ENGINEER),								//(엔지니어)->(건매니아,메카메니아)
	PC_CLASS_FLAG_NAMEK_FIGHTER  = (0x01 << PC_CLASS_NAMEK_FIGHTER),								//(전사)->(마계전사,마도전사)
	PC_CLASS_FLAG_NAMEK_MYSTIC   = (0x01 << PC_CLASS_NAMEK_MYSTIC),									//(용족)->(덴덴도사,포코도사)
	PC_CLASS_FLAG_MIGHTY_MAJIN   = (0x01 << PC_CLASS_MIGHTY_MAJIN),									//(대마인)->(그랜마,얼티마)
	PC_CLASS_FLAG_WONDER_MAJIN   = (0x01 << PC_CLASS_WONDER_MAJIN),									//(의마인)->(플라즈마,카르마)
	PC_CLASS_FLAG_STREET_FIGHTER = (0x01 << PC_CLASS_STREET_FIGHTER) | PC_CLASS_FLAG_HUMAN_FIGHTER,	//(격투가)
	PC_CLASS_FLAG_SWORD_MASTER   = (0x01 << PC_CLASS_SWORD_MASTER)   | PC_CLASS_FLAG_HUMAN_FIGHTER,	//(검술가)
	PC_CLASS_FLAG_CRANE_ROSHI    = (0x01 << PC_CLASS_CRANE_ROSHI)    | PC_CLASS_FLAG_HUMAN_MYSTIC,	//(학선사)
	PC_CLASS_FLAG_TURTLE_ROSHI   = (0x01 << PC_CLASS_TURTLE_ROSHI)   | PC_CLASS_FLAG_HUMAN_MYSTIC,	//(거북선사)
	PC_CLASS_FLAG_GUN_MANIA      = (0x01 << PC_CLASS_GUN_MANIA)      | PC_CLASS_FLAG_HUMAN_ENGINEER,//(건매니아)
	PC_CLASS_FLAG_MECH_MANIA     = (0x01 << PC_CLASS_MECH_MANIA)     | PC_CLASS_FLAG_HUMAN_ENGINEER,//(메카매니아)
	PC_CLASS_FLAG_DARK_WARRIOR   = (0x01 << PC_CLASS_DARK_WARRIOR)   | PC_CLASS_FLAG_NAMEK_FIGHTER,	//(마계전사)
	PC_CLASS_FLAG_SHADOW_KNIGHT  = (0x01 << PC_CLASS_SHADOW_KNIGHT)  | PC_CLASS_FLAG_NAMEK_FIGHTER,	//(마도전사)
	PC_CLASS_FLAG_DENDEN_HEALER  = (0x01 << PC_CLASS_DENDEN_HEALER)  | PC_CLASS_FLAG_NAMEK_MYSTIC,	//(덴덴도사)
	PC_CLASS_FLAG_POCO_SUMMONER  = (0x01 << PC_CLASS_POCO_SUMMONER)  | PC_CLASS_FLAG_NAMEK_MYSTIC,	//(포코도사)
	PC_CLASS_FLAG_GRAND_MA       = (0x01 << PC_CLASS_GRAND_MA)       | PC_CLASS_FLAG_MIGHTY_MAJIN,	//(그랜마)
	PC_CLASS_FLAG_ULTI_MA        = (0x01 << PC_CLASS_ULTI_MA)        | PC_CLASS_FLAG_MIGHTY_MAJIN,	//(얼티마)
	PC_CLASS_FLAG_PLAS_MA        = (0x01 << PC_CLASS_PLAS_MA)        | PC_CLASS_FLAG_WONDER_MAJIN,	//(플라즈마)
	PC_CLASS_FLAG_KAR_MA         = (0x01 << PC_CLASS_KAR_MA)         | PC_CLASS_FLAG_WONDER_MAJIN,	//(카르마)
};


// 스폰 캐릭터 움직임 타입
enum eSPAWN_MOVE_TYPE
{
	SPAWN_MOVE_FIX, // 고정
	SPAWN_MOVE_WANDER, // 단순 배회
	SPAWN_MOVE_PATROL, // 경로 따라가기 반복
	SPAWN_MOVE_FOLLOW, // 타겟 따라가기
	SPAWN_MOVE_PATH_ONCE, // 경로 따라가기 1회

	SPAWN_MOVE_COUNT,

	SPAWN_MOVE_UNKNOWN = 0xFF,
	SPAWN_MOVE_FIRST = SPAWN_MOVE_FIX,
	SPAWN_MOVE_LAST = SPAWN_MOVE_PATH_ONCE
};


// 스폰 기능 플래그
enum eSPAWN_FUNC_FLAG
{
	SPAWN_FUNC_FLAG_RESPAWN = 0x01, // 리스폰
	SPAWN_FUNC_FLAG_NO_SPAWN_WAIT = 0x02, // 스폰 대기 시간 없음
	SPAWN_FUNC_FLAG_INCORRECT_HEIGHT = 0x04, // 정확한 높이값 보정 필요
};


// 스폰 제거 타입
enum eSPAWN_REMOVE_TYPE
{
	SPAWN_REMOVE_TYPE_CLEAR, // 그냥 삭제
	SPAWN_REMOVE_TYPE_DESPAWN, // 디스폰 연출 후 삭제
	SPAWN_REMOVE_TYPE_FAINT, // 사망 후 삭제

	INVALID_SPAWN_REMOVE_TYPE = 0xFF
};


// 몹 등급
enum eMOB_GRADE
{
	MOB_GRADE_NORMAL,// (일반)
	MOB_GRADE_SUPER, // (슈퍼)
	MOB_GRADE_ULTRA, // (울트라)
	MOB_GRADE_BOSS,  // (보쓰)
	MOB_GRADE_HERO,  // (히어로)

	MOB_GRADE_UNKNOWN	= 0xFF,

	MOB_GRADE_FIRST		= MOB_GRADE_NORMAL,
	MOB_GRADE_LAST		= MOB_GRADE_HERO
};


// 몹 타입
enum eMOB_TYPE
{
	MOB_TYPE_ANIMAL,
	MOB_TYPE_HUMAN_ANIMAL,
	MOB_TYPE_DINOSAUR,
	MOB_TYPE_ALIEN,
	MOB_TYPE_ANDROID,
	MOB_TYPE_ROBOT,
	MOB_TYPE_DRAGON,
	MOB_TYPE_DEVIL,
	MOB_TYPE_UNDEAD,
	MOB_TYPE_PLANT,
	MOB_TYPE_INSECT,
	MOB_TYPE_HUMAN,
	MOB_TYPE_NAMEC,
	MOB_TYPE_MAJIN,
	MOB_TYPE_BUILDING,

	MOB_TYPE_UNKNOWN	= 0xFF,

	MOB_TYPE_FIRST		= MOB_TYPE_ANIMAL,
	MOB_TYPE_LAST		= MOB_TYPE_INSECT
};

// Avatar Type
//
// Summon Pet과 Item Pet은 클라이언트에서 제어하기 때문에 같은 avatar로 간주한다.
// Summon Pet and Item Pet are regarded as 'avatar' because they are controlled by Client.
// by YOSHIKI(2006-12-26)
enum eDBO_AVATAR_TYPE
{
	DBO_AVATAR_TYPE_AVATAR = 0,
	DBO_AVATAR_TYPE_SUMMON_PET_1,
//	DBO_AVATAR_TYPE_SUMMON_PET_2,
	DBO_AVATAR_TYPE_ITEM_PET_1,
//	DBO_AVATAR_TYPE_ITEM_PET_2,

	DBO_AVATAR_TYPE_COUNT,

	DBO_AVATAR_TYPE_INVALID = 0xFF,

	DBO_AVATAR_TYPE_PET_FIRST = DBO_AVATAR_TYPE_SUMMON_PET_1,
//	DBO_AVATAR_TYPE_PET_LAST = DBO_AVATAR_TYPE_ITEM_PET_2,
	DBO_AVATAR_TYPE_PET_LAST = DBO_AVATAR_TYPE_ITEM_PET_1,

	DBO_AVATAR_TYPE_SUMMON_PET_FIRST = DBO_AVATAR_TYPE_SUMMON_PET_1,
//	DBO_AVATAR_TYPE_SUMMON_PET_LAST = DBO_AVATAR_TYPE_SUMMON_PET_2,
	DBO_AVATAR_TYPE_SUMMON_PET_LAST = DBO_AVATAR_TYPE_SUMMON_PET_1,

	DBO_AVATAR_TYPE_ITEM_PET_FIRST = DBO_AVATAR_TYPE_ITEM_PET_1,
//	DBO_AVATAR_TYPE_ITEM_PET_LAST = DBO_AVATAR_TYPE_ITEM_PET_2,
	DBO_AVATAR_TYPE_ITEM_PET_LAST = DBO_AVATAR_TYPE_ITEM_PET_1,

	DBO_AVATAR_TYPE_COUNT_SUMMON_PET = DBO_AVATAR_TYPE_SUMMON_PET_LAST - DBO_AVATAR_TYPE_SUMMON_PET_FIRST + 1,
	DBO_AVATAR_TYPE_COUNT_ITEM_PET = DBO_AVATAR_TYPE_ITEM_PET_LAST - DBO_AVATAR_TYPE_ITEM_PET_FIRST + 1,
};


// AI Function
// 아래 비트플래그를 수정함 [3/31/2008]
//enum eAI_FUNCTION
//{
//	AI_FUNC_NORMAL = 0,
//	AI_FUNC_SECRECY,
//	AI_FUNC_STAGE,
//	AI_FUNC_STARRING,
//
//	AI_FUNC_LEAVING = 5,
//	AI_FUNC_END,
//	AI_FUNC_COMPLETION,
//
//	AI_FUNC_DEFENSIVE = 9,
//	AI_FUNC_OFFENSIVE,	
//
//	AI_FUNC_DESPERATION = 12,
//	AI_FUNC_FLEE,
//
//	AI_FUNC_FOCUS = 17,
//	AI_FUNC_REVENGE,
//	AI_FUNC_MEAN,
//	AI_FUNC_BRAVE,
//
//	AI_FUNC_APATHY = 24,
//	AI_FUNC_HELP,
//	AI_FUNC_CARE,
//	AI_FUNC_HEALING,
//
//	AI_FUNC_SMART_OFFENSIVE,// 똑똑한 선공몹 [3/10/2008]
//	AI_FUNC_ALLIANCE_HELP, // Alliance Help  [3/11/2008]
//	AI_FUNC_RETREAT, // 퇴각 [3/12/2008]
//	AI_FUNC_TIMID, //겁쟁이 [3/12/2008]
//
//	AI_FUNC_COUNT,
//
//	AI_FUNC_FIRST = AI_FUNC_NORMAL,
//
//	AI_FUNC_LAST = AI_FUNC_HEALING,
//};

enum eAI_FUNCTION
{
	//삭제 리스트
	//AI_FUNC_NORMAL = 0,
	//AI_FUNC_SECRECY,
	//AI_FUNC_STAGE,
	//AI_FUNC_STARRING,

	//AI_FUNC_LEAVING = 5,
	//AI_FUNC_END,
	//AI_FUNC_COMPLETION,

	//AI_FUNC_APATHY = 24,
	//AI_FUNC_HELP,
	//AI_FUNC_CARE,
	//AI_FUNC_HEALING,

	AI_FUNC_DEFENSIVE = 0,			//후공
	AI_FUNC_TIMID,					//겁쟁이
	AI_FUNC_OFFENSIVE,				//선공
	AI_FUNC_SMART_OFFENSIVE,		//똑똑한 선공몹
	AI_FUNC_ONLYSKILL,				//스킬만 사용

	AI_FUNC_DESPERATION = 5,		//필사
	AI_FUNC_FLEE,					//도망
	AI_FUNC_RETREAT,				//퇴각

	AI_FUNC_REVENGE = 9,			//복수
	AI_FUNC_FOCUS,					//집중
	AI_FUNC_MEAN,					//비열
	AI_FUNC_BRAVE,					//용감

	AI_FUNC_ALLIANCE_HELP = 15, // Alliance Help  [3/11/2008]
	AI_FUNC_DEFEND,// 경비 [11/10/2008]

	AI_FUNC_NOT_CHASE = 20, // 추적안해 [11/10/2008]
	AI_FUNC_NOT_MOVE, // 이동안해 [11/10/2008]

	AI_FUNC_COUNT,

	AI_FUNC_FIRST = AI_FUNC_DEFENSIVE,
	AI_FUNC_LAST = AI_FUNC_ALLIANCE_HELP,
};


// Ai Function Flag
// 아래 코드를 수정 [3/31/2008]
//enum eAI_FUNCTION_FLAG
//{
//	AI_FUNC_FLAG_NORMAL				= 0x01 << AI_FUNC_NORMAL,
//	AI_FUNC_FLAG_SECRECY			= 0x01 << AI_FUNC_SECRECY,
//	AI_FUNC_FLAG_STAGE				= 0x01 << AI_FUNC_STAGE,
//	AI_FUNC_FLAG_STARRING			= 0x01 << AI_FUNC_STARRING,
//
//	AI_FUNC_FLAG_LEAVING				= 0x01 << AI_FUNC_LEAVING,
//	AI_FUNC_FLAG_END					= 0x01 << AI_FUNC_END,
//	AI_FUNC_FLAG_COMPLETION			= 0x01 << AI_FUNC_COMPLETION,
//
//	AI_FUNC_FLAG_DEFENSIVE			= 0x01 << AI_FUNC_DEFENSIVE,
//	AI_FUNC_FLAG_OFFENSIVE			= 0x01 << AI_FUNC_OFFENSIVE,
//	
//	AI_FUNC_FLAG_DESPERATION		= 0x01 << AI_FUNC_DESPERATION,
//	AI_FUNC_FLAG_FLEE				= 0x01 << AI_FUNC_FLEE,
//
//	AI_FUNC_FLAG_FOCUS				= 0x01 << AI_FUNC_FOCUS,
//	AI_FUNC_FLAG_REVENGE			= 0x01 << AI_FUNC_REVENGE,
//	AI_FUNC_FLAG_MEAN				= 0x01 << AI_FUNC_MEAN,
//	AI_FUNC_FLAG_BRAVE				= 0x01 << AI_FUNC_BRAVE,
//
//	AI_FUNC_FLAG_APATHY				= 0x01 << AI_FUNC_APATHY,
//	AI_FUNC_FLAG_HELP				= 0x01 << AI_FUNC_HELP,
//	AI_FUNC_FLAG_CARE				= 0x01 << AI_FUNC_CARE,
//	AI_FUNC_FLAG_HEALING			= 0x01 << AI_FUNC_HEALING,
//
//	AI_FUNC_FLAG_SMART_OFFENSIVE	= 0x01 << AI_FUNC_SMART_OFFENSIVE, // [3/11/2008]
//	AI_FUNC_FLAG_ALLIANCE_HELP		= 0x01 << AI_FUNC_ALLIANCE_HELP,// [3/11/2008]
//	AI_FUNC_FLAG_RETREAT			= 0x01 << AI_FUNC_RETREAT,// [3/12/2008]
//	AI_FUNC_FLAG_TIMID				= 0x01 << AI_FUNC_TIMID,// [3/12/2008]
//
//};

enum eAI_FUNCTION_FLAG
{
	AI_FUNC_FLAG_DEFENSIVE			= 0x01 << AI_FUNC_DEFENSIVE,
	AI_FUNC_FLAG_TIMID				= 0x01 << AI_FUNC_TIMID,// [3/12/2008]
	AI_FUNC_FLAG_OFFENSIVE			= 0x01 << AI_FUNC_OFFENSIVE,
	AI_FUNC_FLAG_SMART_OFFENSIVE	= 0x01 << AI_FUNC_SMART_OFFENSIVE, // [3/11/2008]
	AI_FUNC_FLAG_ONLYSKILL			= 0x01 << AI_FUNC_ONLYSKILL, // [11/10/2008]

	AI_FUNC_FLAG_DESPERATION		= 0x01 << AI_FUNC_DESPERATION,
	AI_FUNC_FLAG_FLEE				= 0x01 << AI_FUNC_FLEE,
	AI_FUNC_FLAG_RETREAT			= 0x01 << AI_FUNC_RETREAT,// [3/12/2008]

	AI_FUNC_FLAG_REVENGE			= 0x01 << AI_FUNC_REVENGE,
	AI_FUNC_FLAG_FOCUS				= 0x01 << AI_FUNC_FOCUS,
	AI_FUNC_FLAG_MEAN				= 0x01 << AI_FUNC_MEAN,
	AI_FUNC_FLAG_BRAVE				= 0x01 << AI_FUNC_BRAVE,

	AI_FUNC_FLAG_ALLIANCE_HELP		= 0x01 << AI_FUNC_ALLIANCE_HELP,// [3/11/2008]
	AI_FUNC_FLAG_DEFEND				= 0x01 << AI_FUNC_DEFEND,// 경비 [3/12/2008]

	AI_FUNC_FLAG_NOT_CHASE			= 0x01 << AI_FUNC_NOT_CHASE, // [11/10/2008]
	AI_FUNC_FLAG_NOT_MOVE			= 0x01 << AI_FUNC_NOT_MOVE, // [11/10/2008]
};


// NPC JOB
enum eNPC_JOB
{
	NPC_JOB_WEAPON_MERCHANT,				// 무기상인
	NPC_JOB_ARMOR_MERCHANT,					// 의복상인
	NPC_JOB_GOODS_MERCHANT,					// 잡화상인
	NPC_JOB_SCOUTER_MERCHANT,				// 스카우터상인
	NPC_JOB_GUARD,							// 경비
	NPC_JOB_SKILL_TRAINER_HFI,				// 무도가 교관
	NPC_JOB_SKILL_TRAINER_HMY,				// 기공사 교관
	NPC_JOB_SKILL_TRAINER_HEN,				// 엔지니어 교관
	NPC_JOB_SKILL_TRAINER_NFI,				// 전사 교관
	NPC_JOB_SKILL_TRAINER_NMY,				// 용족 교관
	NPC_JOB_SKILL_TRAINER_MMI,				// 대마인 교관
	NPC_JOB_SKILL_TRAINER_MWO,				// 의마인 교관
	NPC_JOB_BANKER,							// 창고지기
	NPC_JOB_TALKER,							// 이야기꾼
	NPC_JOB_GUILD_MANAGER,					// 길드매니저
	NPC_JOB_SUMMON_PET,						// 소환수
	NPC_JOB_DOGI_MERCHANT,					// 도복 상인
	NPC_JOB_SPECIAL_WEAPON_MERCHANT,		// 고급 무기 상인
	NPC_JOB_SPECIAL_ARMOR_MERCHANT,			// 고급 의복 상인
	NPC_JOB_SPECIAL_GOODS_MERCHANT,			// 고급 잡화 상인
	NPC_JOB_SPECIAL_FOODS_MERCHANT,			// 고급 식료품 상인
	NPC_JOB_SPECIAL_SCOUTER_MERCHANT,		// 고급 스카우터 상인
	NPC_JOB_GRAND_SKILL_TRAINER_HFI,		// 무도가 그랜드 스킬마스터
	NPC_JOB_GRAND_SKILL_TRAINER_HMY,		// 기공사 그랜드 스킬마스터
	NPC_JOB_GRAND_SKILL_TRAINER_HEN,		// 엔지니어 그랜드 스킬마스터
	NPC_JOB_GRAND_SKILL_TRAINER_NFI,		// 전사 그랜드 스킬마스터
	NPC_JOB_GRAND_SKILL_TRAINER_NMY,		// 용족 그랜드 스킬마스터
	NPC_JOB_GRAND_SKILL_TRAINER_MMI,		// 대마인 그랜드 스킬마스터
	NPC_JOB_GRAND_SKILL_TRAINER_MWO,		// 의마인 그랜드 스킬마스터
	NPC_JOB_SUB_WEAPON_MERCHANT,			// 보조 무기 상인
	NPC_JOB_GATE_KEEPER,					// 문지기
	NPC_JOB_VENDING_MACHINE,				// 자판기
	NPC_JOB_TIMEMACHINE_MERCHANT,			// 타임머신진입 NPC
	NPC_JOB_PORTAL_MAN,						// 순간 이동 서비스맨			
	NPC_JOB_BUS,							// 버스
	NPC_JOB_RECEPTION,						// 천하제일 무도회 접수인
	NPC_JOB_BUDOHSI_MERCHANT,				// 무도사 상인
	NPC_JOB_REFEREE,						// 심판
	NPC_JOB_GAMBLE_MERCHANT,				// 뽑기 상인
	NPC_JOB_CHAMPION_MERCHANT,				// 챔피언 상인
	NPC_JOB_DOJO_MANAGER,					// 도장 관리인
	NPC_JOB_DOJO_MERCHANT,					// 도장 상인
	NPC_JOB_DOJO_SEAL,						// 도장 인장
	NPC_JOB_DOJO_BANKER,					// 도장 창고
	NPC_JOB_MIX_MASTER,						// 믹스 마스터

	NPC_JOB_COUNT,
	NPC_JOB_UNKNOWN	= 0xFF,

	NPC_JOB_FIRST = NPC_JOB_WEAPON_MERCHANT,
	NPC_JOB_LAST = NPC_JOB_COUNT - 1
};


// NPC Function Flag
enum eNPC_FUNCTION
{
	NPC_FUNC_MERCHANT,			// 상인기능
	NPC_FUNC_GUARD,				// 경비기능
	NPC_FUNC_SKILL_TRAINER,		// 교관기능
	NPC_FUNC_BANKER,			// 은행원기능
	NPC_FUNC_TALKER,			// 이야기기능
	NPC_FUNC_QUEST_GRANTER,		// 퀘스트 부여
	NPC_FUNC_GUILD_MANAGER,		// 길드매니저
	NPC_FUNC_SUMMON_PET,		// 소환수
	NPC_FUNC_GATE_KEEPER,		// 문지기
	NPC_FUNC_TIME_QUEST,		// 타임머신 퀘스트 기능
	NPC_FUNC_PORTAL,			// Teleport Portal
	NPC_FUNC_SCAN_BY_MOB,		// 몹에게 선공을 당할수 있음
	NPC_FUNC_BUS,				// 버스
	NPC_FUNC_RECEPTION,			// 천하제일 무도회 접수인
	NPC_FUNC_BUDOHSI_MERCHANT,	// 무도사 상인
	NPC_FUNC_REFEREE,			// 심판
	NPC_FUNC_BUILDING,			// 빌딩형
	NPC_FUNC_FACING,			// 대면 : 패트롤 BOT을 클릭하면 멈추게하는 기능
	NPC_FUNC_TURN_OFF,			// 공격을 당해도 그 방향을 안본다
	NPC_FUNC_DISCLOSE_LP,		// LP표시여부
	NPC_FUNC_GAMBLE_MERCHANT,	// 뽑기 상인
	NPC_FUNC_MOVING_NPC,		// 움직이는 NPC (패쓰 데이터)
	NPC_FUNC_SPAWN_NPC,			//중간 스폰 NPC
	NPC_FUNC_DOJO_MANAGER,		// 도장 관리인
	NPC_FUNC_DOJO_MERCHANT,		// 도장 상인
	NPC_FUNC_DOJO_SEAL,			// 도장 인장
	NPC_FUNC_DOJO_BANKER,		// 도장 창고

	NPC_FUNC_COUNT,
	NPC_FUNC_FIRST = NPC_FUNC_MERCHANT,
	NPC_FUNC_LAST = NPC_FUNC_COUNT - 1,
};


// NPC Function Flag
enum eNPC_FUNCTION_FLAG
{
	NPC_FUNC_FLAG_MERCHANT			= MAKE_BIT_FLAG( NPC_FUNC_MERCHANT ),
	NPC_FUNC_FLAG_GUARD				= MAKE_BIT_FLAG( NPC_FUNC_GUARD ),			
	NPC_FUNC_FLAG_SKILL_TRAINER		= MAKE_BIT_FLAG( NPC_FUNC_SKILL_TRAINER ),	
	NPC_FUNC_FLAG_BANKER			= MAKE_BIT_FLAG( NPC_FUNC_BANKER ),			
	NPC_FUNC_FLAG_TALKER			= MAKE_BIT_FLAG( NPC_FUNC_TALKER ),			
	NPC_FUNC_FLAG_QUEST_GRANTER		= MAKE_BIT_FLAG( NPC_FUNC_QUEST_GRANTER ),	
	NPC_FUNC_FLAG_GUILD_MANAGER		= MAKE_BIT_FLAG( NPC_FUNC_GUILD_MANAGER ),
	NPC_FUNC_FLAG_SUMMON_PET		= MAKE_BIT_FLAG( NPC_FUNC_SUMMON_PET ),
	NPC_FUNC_FLAG_GATE_KEEPER		= MAKE_BIT_FLAG( NPC_FUNC_GATE_KEEPER ),
	NPC_FUNC_FLAG_TIME_QUEST		= MAKE_BIT_FLAG( NPC_FUNC_TIME_QUEST ),
	NPC_FUNC_FLAG_PORTAL			= MAKE_BIT_FLAG( NPC_FUNC_PORTAL ),
	NPC_FUNC_FLAG_SCAN_BY_MOB		= MAKE_BIT_FLAG( NPC_FUNC_SCAN_BY_MOB ),
	NPC_FUNC_FLAG_BUS				= MAKE_BIT_FLAG( NPC_FUNC_BUS ),
	NPC_FUNC_FLAG_RECEPTION			= MAKE_BIT_FLAG( NPC_FUNC_RECEPTION ),
	NPC_FUNC_FLAG_BUDOHSI_MERCHANT	= MAKE_BIT_FLAG( NPC_FUNC_BUDOHSI_MERCHANT ),
	NPC_FUNC_FLAG_REFEREE			= MAKE_BIT_FLAG( NPC_FUNC_REFEREE ),
	NPC_FUNC_FLAG_BUILDING			= MAKE_BIT_FLAG( NPC_FUNC_BUILDING ),
	NPC_FUNC_FLAG_FACING			= MAKE_BIT_FLAG( NPC_FUNC_FACING ),	// 수정 ( NPC_FUNC_FLAG_FACING = NPC_FUNC_FLAG_MERCHANT | NPC_FUNC_FLAG_QUEST_GRANTER, // 이동중에 메뉴를 띄우게 되는 NPC 모음 ) [5/19/2008]	
																		// 이렇게하면 문제되는 것이 하나 있는데, 같은 NPC를 다른 곳에서 썼을 때 그곳에서는 말을 걸면 멈추지 말아야 될때에는 NPC를 새로 만들어야 한다
	NPC_FUNC_FLAG_TURN_OFF			= MAKE_BIT_FLAG( NPC_FUNC_TURN_OFF ), // [6/2/2008]
	NPC_FUNC_FLAG_DISCLOSE_LP		= MAKE_BIT_FLAG( NPC_FUNC_DISCLOSE_LP ), // [8/21/2008 Peessi]
	NPC_FUNC_FLAG_GAMBLE_MERCHANT	= MAKE_BIT_FLAG( NPC_FUNC_GAMBLE_MERCHANT ),// [7/21/2008]
	NPC_FUNC_FLAG_MOVING_NPC		= MAKE_BIT_FLAG( NPC_FUNC_MOVING_NPC ), // [8/27/2008]
	NPC_FUNC_FLAG_SPAWN_NPC			= MAKE_BIT_FLAG( NPC_FUNC_SPAWN_NPC ),
	NPC_FUNC_FLAG_DOJO_MANAGER		= MAKE_BIT_FLAG( NPC_FUNC_DOJO_MANAGER ),
	NPC_FUNC_FLAG_DOJO_MERCHANT		= MAKE_BIT_FLAG( NPC_FUNC_DOJO_MERCHANT ),
	NPC_FUNC_FLAG_DOJO_SEAL		    = MAKE_BIT_FLAG( NPC_FUNC_DOJO_SEAL ),
	NPC_FUNC_FLAG_DOJO_BANKER		= MAKE_BIT_FLAG( NPC_FUNC_DOJO_BANKER ),


};

// MERCHANT SELL TYPE
enum eMERCHANT_SELL_TYPE
{
	MERCHANT_SELL_TYPE_ITEM,
	MERCHANT_SELL_TYPE_SKILL,
	MERCHANT_SELL_TYPE_HTB_SKILL,
	MERCHANT_SELL_TYPE_BANK,
	MERCHANT_SELL_TYPE_TIMEMACHINE,
	MERCHANT_SELL_TYPE_BUDOKAI,
	MERCHANT_SELL_TYPE_GAMBLE,
	MERCHANT_SELL_TYPE_NETPY,

	MERCHANT_SELL_TYPE_COUNT,
	MERCHANT_SELL_TYPE_FIRST = MERCHANT_SELL_TYPE_ITEM,
	MERCHANT_SELL_TYPE_LAST  = MERCHANT_SELL_TYPE_NETPY,
};

// QuickSlotType Define
enum eQUICK_SLOT_TYPE
{
	QUICK_SLOT_TYPE_ITEM,
	QUICK_SLOT_TYPE_SKILL,
	QUICK_SLOT_TYPE_HTB_SKILL,
	QUICK_SLOT_TYPE_SOCIALACTION,

	QUICK_SLOT_TYPE_COUNT,
	QUICK_SLOT_TYPE_FIRST = QUICK_SLOT_TYPE_ITEM,
	QUICK_SLOT_TYPE_LAST  = QUICK_SLOT_TYPE_SOCIALACTION,
};


enum eMARKING_TYPE
{
	MARKING_TYPE_BUDOKAI_WINNER01 = 0,
	MARKING_TYPE_BUDOKAI_WINNER02,
	MARKING_TYPE_BUDOKAI_WINNER03,

	MARKING_TYPE_BUDOKAI_JUNIOR_WINNER01 = MARKING_TYPE_BUDOKAI_WINNER01,
	MARKING_TYPE_BUDOKAI_JUNIOR_WINNER02 = MARKING_TYPE_BUDOKAI_WINNER02,
	MARKING_TYPE_BUDOKAI_JUNIOR_WINNER03 = MARKING_TYPE_BUDOKAI_WINNER03,

	INVALID_MARKING_TYPE = INVALID_BYTE,
};

enum eMARKING_CONTENTS_TYPE
{
	MARKING_CONTENTS_TYPE_BUDOKAI,		// 천하제일무도회에서 호출함
	MARKING_CONTENTS_TYPE_COMMUNITY,	// 커뮤니티 서버로 부터 broadcast 요청이 옴

	INVALID_MARKING_CONTENTS_TYPE = 0xff
};



//-----------------------------------------------------------------------------------
// 캐릭터 관련 상수 정의 : [4/25/2006 zeroera] : 수정필요 : lua로 옮길 것
//-----------------------------------------------------------------------------------
const DWORD			NTL_CHAR_RP_REGEN_WAIT_TIME					= 15 * 1000; // 경과 후 RP 감소 시작 (15)

const DWORD			NTL_CHAR_RP_BALL_UPDATE_INTERVAL			= 30 * 1000; // 경과 후 RP 구슬 감소 시작 (30)

const BYTE			NTL_CHAR_RP_BALL_MAX						= 7; // RP 구슬의 최대치.

const BYTE			NTL_CHAR_RP_BALL_CHANGE_VALUE				= 1;

const int			NTL_CHAR_RP_BALL_INCREASE_LEVEL				= 10; // RP ball increases every THIS level value

const int			DBO_CHAR_FACE_SHAPE_COUNT			= 10;

const int			DBO_CHAR_HAIR_SHAPE_COUNT			= 10;

const int			DBO_CHAR_HAIR_COLOR_COUNT			= 10;

const int			DBO_CHAR_SKIN_COLOR_COUNT			= 5;

//const int			NTL_CHAR_MAX_LEVEL					= 100;	// 캐릭터 최대 레벨

const int			NTL_CHAR_CONVERT1_NEED_LEVEL        = 30;	// 1차전직에 필요 레벨

const int           NTL_CHAR_QUICK_SLOT_MAX_COUNT	    = 48;   // 12 * 4 칸

const int           NTL_GM_USE_LEVEL_NONE				= 0; 

const int           NTL_GM_USE_LEVEL1					= 1;   

const int           NTL_GM_USE_LEVEL2					= 2;   

const DWORD			DBO_CHAR_REPUTATION_DRAGONBALL = 100; // 개인 드래곤볼 수집 명성치

extern const char*	DBO_PURE_MAJIN_MODEL_RESOURCE_NAME;

const float			DBO_GREAT_NAMEK_ATTACK_RANGE			= 4.0f;

const DWORD			DBO_NPC_MAX_PARTY_NUMBER			= 100000;

const DWORD			DBO_MAX_PLAY_TIME_TERM				= 300000;	// 실시간 위치 저장 주기 및 한번에 저장하는 play time 최대 시간


const DWORD			NTL_CHAR_MAX_SAVE_ZENNY				= 1000000000;

const DWORD			NTL_CHAR_MAX_BANK_SAVE_ZENNY		= 2000000000;

const int			NTL_MAX_NEWBIE_QUICKSLOT_COUNT		= 5;  // 생성시 부여가능 퀵슬롯

const DWORD			NTL_DELETE_CHAR_MAX_WAIT_TICK		= 5 * 60 * 1000; //삭제 대기 시간

const DWORD			NTL_DELETE_CHAR_CHECK_TICK			= 1 * 1000; //삭제 대기 체크 시간

//-----------------------------------------------------------------------------------
const BYTE			GMT_MAX_EFFECT_LIST_SIZE			= 5;		// GMT 적용 효과의 최대 갯수

const BYTE			GMT_MAX_TIME_SLOT_SIZE				= 3;		// GMT 효과의 시간 설정 갯수


#pragma pack(1)


// NewbieQuickData
struct sNEWBIE_QUICKSLOT_DATA
{
	TBLIDX			tbilidx; 
	BYTE			byType;	
	BYTE			byQuickSlot; 
};

struct sDELETE_WAIT_CHARACTER
{
	ACCOUNTID		accountId;
	CHARACTERID		charId;
	DWORD			dwPastTick;
};

struct sDELETE_WAIT_CHARACTER_INFO
{
	CHARACTERID		charId;
	DWORD			dwPastTick;// 경과 틱
};

struct sMARKING
{
	BYTE byCode;			// eMARKING_TYPE

public:
	sMARKING()
		: byCode( INVALID_MARKING_TYPE )
	{
	}
};//end of sMARKING
	
// 캐릭터 생김새
struct sPC_SHAPE
{
	BYTE			byFace;
	BYTE			byHair;
	BYTE			byHairColor;
	BYTE			bySkinColor;
};

struct sASPECTSTATE_BASE
{
	BYTE							byAspectStateId;
};

union sASPECTSTATE_DETAIL
{
	sASPECTSTATE_SUPER_SAIYAN		sSuperSaiyan;
	sASPECTSTATE_PURE_MAJIN			sPureMajin;
	sASPECTSTATE_GREAT_NAMEK		sGreatNamek;
	sASPECTSTATE_KAIOKEN			sKaioken;
	sASPECTSTATE_SPINNING_ATTACK	sSpinningAttack;
	sASPECTSTATE_VEHICLE			sVehicle;
};

struct sASPECTSTATE
{
	sASPECTSTATE_BASE				sAspectStateBase;
	sASPECTSTATE_DETAIL				sAspectStateDetail;
};

// 캐릭터 상태
struct sCHARSTATE_BASE
{
	BYTE			byStateID;
	DWORD			dwStateTime;
	DWORD			dwConditionFlag;
	sASPECTSTATE	aspectState;
	bool			bFightMode:1;

	sVECTOR3		vCurLoc;
	sVECTOR3		vCurDir;
};


// 상태에 따른 구조체
union sCHARSTATE_DETAIL
{ 
	sCHARSTATE_SPAWNING				sCharStateSpawning;
	sCHARSTATE_DESPAWNING			sCharStateDespawning;
	sCHARSTATE_STANDING				sCharStateStanding;
	sCHARSTATE_SITTING				sCharStateSitting;
	sCHARSTATE_FAINTING				sCharStateFainting;
	sCHARSTATE_CAMPING				sCharStateCamping;
	sCHARSTATE_LEAVING				sCharStateLeaving;

	sCHARSTATE_MOVING				sCharStateMoving;
	sCHARSTATE_DESTMOVE				sCharStateDestMove;
	sCHARSTATE_FOLLOWING			sCharStateFollwing;
	sCHARSTATE_FALLING				sCharStateFalling;
	sCHARSTATE_DASH_PASSIVE			sCharStateDashPassive;
	sCHARSTATE_TELEPORTING			sCharStateTeleporting;
	sCHARSTATE_SLIDING				sCharStateSliding;
	sCHARSTATE_KNOCKDOWN			sCharStateKnockdown;

	sCHARSTATE_FOCUSING				sCharStateFocusing;
	sCHARSTATE_CASTING				sCharStateCasting;
	sCHARSTATE_SKILL_AFFECTING		sCharStateSkillAffecting;
	sCHARSTATE_KEEPING_EFFECT		sCharStateKeepingEffect;
	sCHARSTATE_CASTING_ITEM			sCharStateCastingItem;

	sCHARSTATE_STUNNED				sCharStateStunned;
	sCHARSTATE_SLEEPING				sCharStateSleeping;
	sCHARSTATE_PARALYZED			sCharStateParalyzed;

	sCHARSTATE_HTB					sCharStateHTB;
	sCHARSTATE_SANDBAG				sCharStateSandBag;
	sCHARSTATE_CHARGING				sCharStateCharging;

	sCHARSTATE_PRIVATESHOP			sCharStatePrivateShop;//  [7/16/2007]
	sCHARSTATE_DIRECT_PLAY			sCharStateDirectPlay;
	sCHARSTATE_OPERATING			sCharStateOperating;
	sCHARSTATE_RIDEON				sCharStateRideOn;
	sCHARSTATE_TURNING				sCharStateTurning;
};


// 캐릭터 상태 구조체
struct sCHARSTATE
{
	sCHARSTATE_BASE				sCharStateBase;
	sCHARSTATE_DETAIL			sCharStateDetail;
};

//-----------------------------------------------------------------------------------
// PC ( Player Character = Avatar )
//-----------------------------------------------------------------------------------
//Localize CJ Kor
struct sPC_DATA_LOCALIZE_DEV
{
};

struct sPC_DATA_LOCALIZE_CJIKOR
{
	char   	userCN[NTL_MAX_SIZE_USERCN];
	NETP	netP;
	BYTE	byAge;
	BYTE	bySex;
};

//Localize Data
struct sPC_DATA_LOCALIZE
{
	LOCALIZETYPE type;

	union
	{
		sPC_DATA_LOCALIZE_DEV	pcDataDev;

		sPC_DATA_LOCALIZE_CJIKOR pcDataCJKor;
	};
};

// 캐릭터 DB 정보
struct sPC_DATA
{
	CHARACTERID		charId;
	WCHAR			awchName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];

	BYTE			byRace;
	BYTE			byClass;
	bool			bIsAdult;
	bool			bChangeClass;
	BYTE			byGender;

	float			fPositionX;
	float			fPositionY;
	float			fPositionZ;

	float			fDirX;
	float			fDirY;
	float			fDirZ;

	BYTE			byBindType;
	WORLDID			bindWorldId;
	TBLIDX			bindObjectTblidx;
	sVECTOR3		vBindLoc;
	sVECTOR3		vBindDir;

	TBLIDX			worldTblidx;
	WORLDID			worldId;
	DWORD			dwEXP;
	BYTE			byLevel;
	DWORD			dwLP;
	DWORD			dwEP;
	DWORD			dwRP;
	BYTE			byFace;
	BYTE			byHair;
	BYTE			byHairColor;
	BYTE			bySkinColor;

	DWORD			dwMoney;
	DWORD			dwMoneyBank;
	BYTE			byBlood;

	DWORD			dwGMGroupSerial;
	ACCOUNTID		GMAccountID;

	GUILDID			guildId;
	DWORD			dwTutorialHint;
	DWORD			dwMapInfoIndex;		// 캐릭터 선택시 출력해 줄 현재 장소의 Text Tblidx

	DWORD			dwReputation;
	DWORD			dwMudosaPoint;
	DWORD			dwSpPoint;

	sMARKING		sMarking;
	sHOIPOIMIX_DATA sMixData;

	sPC_DATA_LOCALIZE sLocalize;
};

// Character 서버창 정보 표시 
struct sPC_SUMMARY
{
	CHARACTERID		charId;
	WCHAR			awchName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE			byRace;
	BYTE			byClass;
	bool			bIsAdult;
	BYTE			byGender;
	BYTE			byFace;
	BYTE			byHair;
	BYTE			byHairColor;
	BYTE			bySkinColor;
	BYTE			byLevel;
	TBLIDX			worldTblidx;
	WORLDID			worldId;
	float			fPositionX;
	float			fPositionY;
	float			fPositionZ;
	DWORD			dwMoney;
	DWORD			dwMoneyBank;
	sITEM_SUMMARY	sItem[EQUIP_SLOT_TYPE_COUNT]; // 장착 아이템 정보
	DWORD			dwMapInfoIndex;
	bool			bTutorialFlag;

	sMARKING		sMarking;
	bool			bNeedNameChange;
	sDBO_DOGI_DATA	sDogi;
};

struct sPC_BRIEF
{
	CHARACTERID		charId;
	TBLIDX			tblidx; // pc 테이블 인덱스
	bool			bIsAdult;
	WCHAR			awchName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WCHAR			wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];

	sPC_SHAPE		sPcShape; // pc 외양 ( 얼굴/머리/머리색 )

	WORD			wCurLP;
	WORD			wMaxLP;
	WORD			wCurEP;
	WORD			wMaxEP;
	BYTE			byLevel;
	float			fSpeed;

	sITEM_BRIEF		sItemBrief[EQUIP_SLOT_TYPE_COUNT]; // 장착 아이템 정보

	WORD			wAttackSpeedRate;

	sDBO_GUILD_MARK	sMark;

	sMARKING		sMarking;

	sDBO_DOGI_DATA	sDogi;
};

//Localize CJ Kor
struct sPC_PROFILE_LOCALIZE_DEV
{
};

struct sPC_PROFILE_LOCALIZE_CJIKOR
{
	NETP	netP;
};

//Localize Data
struct sPC_PROFILE_LOCALIZE
{
	LOCALIZETYPE type;

	union
	{
		sPC_PROFILE_LOCALIZE_DEV pcProfileDev;

		sPC_PROFILE_LOCALIZE_CJIKOR pcProfileCJKor;
	};
};

// PC 캐릭터 전체 정보 ( 아바타 로딩이나 캐릭터 조회등에 사용 )
struct sPC_PROFILE
{
	TBLIDX			tblidx; // pc 테이블 인덱스
	bool			bIsAdult;
	bool			bChangeClass;		// 전직 권한을 가지고 있는지의 여부
	CHARACTERID		charId;		// PC 캐릭터의 고유 ID(DB index)
	WCHAR			awchName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];

	sPC_SHAPE		sPcShape; // pc 외양 ( 얼굴/머리/머리색 )

	sAVATAR_ATTRIBUTE		avatarAttribute;

	WORD			wCurLP;
	WORD			wCurEP;
	WORD			wCurRP;

//	float			fSpeed;

	BYTE			byLevel;
	DWORD			dwCurExp;
	DWORD			dwMaxExpInThisLevel;

	DWORD			dwZenny;
	DWORD			dwTutorialHint;

	BYTE			byBindType;
	WORLDID			bindWorldId;
	TBLIDX			bindObjectTblidx;

	DWORD			dwReputation;
	DWORD			dwMudosaPoint;
	DWORD			dwSpPoint;

	sMARKING		sMarking;
	sHOIPOIMIX_DATA sMixData;

	bool			bIsGameMaster;		// true : 운영자 character

	GUILDID			guildId;

	sPC_PROFILE_LOCALIZE sLocalize;
};

//-----------------------------------------------------------------------------------
// BOT ( NPC/MOB/PET 등의 서버가 컨트롤 하는 캐릭터들의 총칭 )
//-----------------------------------------------------------------------------------

// bot data
struct sBOT_DATA
{
	TBLIDX			tblidx;

	WORLDID			worldID; // world id
	sVECTOR3		vCurLoc; // 현재위치
	sVECTOR3		vCurDir; // 현재방향

	sVECTOR3		vSpawnLoc; // 스폰위치
	sVECTOR3		vSpawnDir; // 스폰 방향
	BYTE			bySpawnRange; // 랜덤 스폰 거리
	WORD			wSpawnTime; // 스폰 시간 (초)
	BYTE			bySpawnFuncFlag; // 스폰 기능 플래그

	BYTE			byMoveType; // eSPAWN_MOVE_TYPE
	BYTE			byWanderRange; // 배회 거리 (m)
	BYTE			byMoveRange; // 한번에 움직이는 거리 (m)

	BYTE			byMoveDelayTime; // 움직이는 딜레이 타임
	ACTIONPATTERNTBLIDX actionpatternTblIdx; //비전투 행동 패턴 셋에 테이블 ID

	TBLIDX			pathTblidx; // 경로 테이블 번호

	TBLIDX			playScript; // 연출 스크립트 번호
	TBLIDX			playScriptScene; // 연출 스크립트 신 번호

	TBLIDX			tblidxAiScript; // AI 스크립트 번호
	TBLIDX			tblidxAiScriptScene; // AI 스크립트 신 번호

	PARTYID			partyID; // 파티 아이디
	bool			bPartyLeader; // 파티 리더
	sVECTOR3		vPartyLeaderDistance; // 파티 리더와의 공간 간격

	DWORD			dwCharConditionFlag; // 생성될때 적용되야 하는 캐릭터 컨디션
};


// bot brief
struct sBOT_BRIEF
{
	TBLIDX			tblidx;

	WORD			wCurLP;
	WORD			wCurEP;
	WORD			wMaxLP;
	WORD			wMaxEP;

	float			fLastWalkingSpeed;
	float			fLastRunningSpeed;
	WORD			wAttackSpeedRate;

	ACTIONPATTERNTBLIDX actionpatternTblIdx; //비전투 행동 패턴 셋에 테이블 ID
};


// bot profile
struct sBOT_PROFILE
{
	TBLIDX			tblidx;
	WORLDID			worldID; // 월드

	sVECTOR3		vSpawnLoc; // 스폰위치
	sVECTOR3		vSpawnDir; // 스폰 방향
	BYTE			bySpawnRange; // 랜덤 스폰 거리
	WORD			wSpawnTime; // 스폰 시간 (초)
	BYTE			bySpawnFuncFlag; // 리스폰 여부

	BYTE			byMoveType; // eSPAWN_MOVE_TYPE
	BYTE			byWanderRange; // 배회 거리
	BYTE			byMoveRange; // 한번에 움직이는 거리

	BYTE			byMoveDelayTime; // 움직이는 시간 간격
	ACTIONPATTERNTBLIDX actionpatternTblIdx; //비전투 행동 패턴 셋에 테이블 ID

	TBLIDX			pathTblidx; // 경로 테이블 번호

	TBLIDX			playScript; // 연출 스크립트 번호
	TBLIDX			playScriptScene; // 연출 스크립트 신 번호

	TBLIDX			tblidxAiScript; // AI 스크립트 번호
	TBLIDX			tblidxAiScriptScene; // AI 스크립트 신 번호

	PARTYID			partyID; // 파티 아이디
	bool			bPartyLeader; // 파티 리더
	sVECTOR3		vPartyLeaderDistance; // 리더와의 공간 간격

	WORD			wCurLP; // LP
	WORD			wMaxLP; // Max LP

	float			fLastWalkingSpeed; // 걷는 속도
	float			fLastRunningSpeed; // 뛰는 속도

};



//-----------------------------------------------------------------------------------
// Non Player Character ( NPC )
//-----------------------------------------------------------------------------------

// npc data
struct sNPC_DATA : public sBOT_DATA
{
};


// npc brief
struct sNPC_BRIEF : public sBOT_BRIEF
{
	TBLIDX			nicknameTblidx;
};


// npc profile
struct sNPC_PROFILE : public sBOT_PROFILE
{
};


//-----------------------------------------------------------------------------------
// Monster Character ( MOB )
//-----------------------------------------------------------------------------------

// mob data
struct sMOB_DATA : public sBOT_DATA
{
	SPAWNGROUPID	spawnGroupId;
};


// mob brief
struct sMOB_BRIEF : public sBOT_BRIEF
{
	BYTE		byBallType; // 몹 생성시 드래곤볼을 떨굴수 있는 경우 인지 아닌지 아닌경우 NtlItem.h cf) eDRAGON_BALL_TYPE
};

// mob profile
struct sMOB_PROFILE : public sBOT_PROFILE
{
	SPAWNGROUPID	spawnGroupId;
};



//-----------------------------------------------------------------------------------
// Pet Character ( PET )
//-----------------------------------------------------------------------------------

// pet data
struct sPET_DATA
{
	BYTE			byPetIndex;

	BYTE			bySourceType;		// eDBO_OBJECT_SOURCE
	TBLIDX			sourceTblidx;
};


// pet brief
struct sPET_BRIEF
{
	BYTE			bySourceType;		// eDBO_OBJECT_SOURCE
	TBLIDX			sourceTblidx;
	TBLIDX			npcTblidx;

	HOBJECT			hOwner;
	float			fLastRunningSpeed;
};

// pet profile
struct sPET_PROFILE
{
	BYTE					bySourceType;		// eDBO_OBJECT_SOURCE
	TBLIDX					sourceTblidx;
	TBLIDX					npcTblidx;

	HOBJECT					hOwner;
	BYTE					byAvatarType;		// eDBO_AVATAR_TYPE

	sAVATAR_ATTRIBUTE		attribute;
};


//-----------------------------------------------------------------------------------
// Summon Pet Character ( SUMMON PET )
//-----------------------------------------------------------------------------------

// summon pet data
struct sSUMMON_PET_DATA : public sPET_DATA
{
	WORD			wCurLP;
	WORD			wCurEP;
};

// summon pet brief
struct sSUMMON_PET_BRIEF : public sPET_BRIEF
{
	WORD			wCurLP;
	WORD			wCurEP;
	WORD			wMaxLP;
	WORD			wMaxEP;

	WORD			wAttackSpeedRate;
};

// summon pet profile
struct sSUMMON_PET_PROFILE : public sPET_PROFILE
{
	WORD			wCurLP;
	WORD			wCurEP;
};


//-----------------------------------------------------------------------------------
// Summon Pet Character ( ITEM PET )
//-----------------------------------------------------------------------------------

// item pet data
struct sITEM_PET_DATA : public sPET_DATA
{
	ITEMID		itemId;
};

// item pet brief
struct sITEM_PET_BRIEF : public sPET_BRIEF
{
};

// item pet profile
struct sITEM_PET_PROFILE : public sPET_PROFILE
{
};


//-----------------------------------------------------------------------------------
// QUICKSLOT
//-----------------------------------------------------------------------------------
struct sQUICK_SLOT_DATA
{
	TBLIDX	tblidx;
	BYTE	bySlot;
	BYTE	byType;
	ITEMID  itemID;
	HOBJECT	hItem;
};

struct sQUICK_SLOT_PROFILE
{
	TBLIDX	tblidx;
	BYTE	bySlot;
	BYTE	byType;
	HOBJECT	hItem;
};

enum eACCEPT_RES_TYPE
{
	ACCEPT_RES_TYPE_DENY = 0,
	ACCEPT_RES_TYPE_OK,
	ACCEPT_RES_TYPE_CANCEL, 
};	


//-----------------------------------------------------------------------------------
// 단축키
//-----------------------------------------------------------------------------------
struct sSHORTCUT_UPDATE_DATA
{
	BYTE    byType;		// eSHORTCUT_CHANGE_TYPE 
	WORD	wActionID;	
	WORD	wKey;
};

struct sSHORTCUT_DATA
{
	WORD	wActionID;	
	WORD	wKey;
};

enum eSHORTCUT_CHANGE_TYPE
{
	eSHORTCUT_CHANGE_TYPE_ADD = 0,
	eSHORTCUT_CHANGE_TYPE_DEL,
	eSHORTCUT_CHANGE_TYPE_UPDATE, 
};	

const int		 NTL_SHORTCUT_MAX_COUNT	= 100;   
//-----------------------------------------------------------------------------------
// WARFOG
//-----------------------------------------------------------------------------------
// WARFOG FLAG
struct sCHAR_WAR_FOG_FLAG
{
	char achWarFogFlag[NTL_MAX_SIZE_WAR_FOG];
};


//-----------------------------------------------------------------------------------
// SERVER CHANGE
//-----------------------------------------------------------------------------------
struct sDBO_SERVER_CHANGE_INFO
{
	SERVERCHANNELID		prevServerChannelId;
	WORLDID				destWorldId;
	TBLIDX				destWorldTblidx;
};


//-----------------------------------------------------------------------------------
// Game Mania Time .by Ju-hyeung Lee
//-----------------------------------------------------------------------------------

// from QueryServer
struct sGAME_MANIA_TIME_DATA
{
	DWORD dwSetTime;
	BYTE abyTimeSlot[GMT_MAX_EFFECT_LIST_SIZE][GMT_MAX_TIME_SLOT_SIZE];
};

// to client
struct sGAME_MANIA_TIME
{
	BYTE abyTimeSlot[GMT_MAX_EFFECT_LIST_SIZE][GMT_MAX_TIME_SLOT_SIZE];
};

#pragma pack()


//-----------------------------------------------------------------------------------
// Initialize function for union data
//-----------------------------------------------------------------------------------
void InitBotData(sBOT_DATA & rsBotData);

void InitNpcData(sNPC_DATA & rsNpcData);

void InitMobData(sMOB_DATA & rsMobData);

//-----------------------------------------------------------------------------------
// Copy character state data
//-----------------------------------------------------------------------------------

void CopyCharState( sCHARSTATE* pDest, const sCHARSTATE* pSrc );

//-----------------------------------------------------------------------------------
// Scouter
//-----------------------------------------------------------------------------------
DWORD Dbo_CalculatePowerLevel(
		BYTE byLastStr, BYTE byLastCon, BYTE byLastFoc, BYTE byLastDex, BYTE byLastSol, BYTE byLastEng,
		WORD wLastPhysicalOffence, WORD wLastPhysicalDefence, WORD wLastEnergyOffence, WORD wLastEnergyDefence,
		WORD wLastAttackRate, WORD wLastDodgeRate, WORD wLastCurseSuccessRate, WORD wLastCurseTolerance,
		WORD wLastPhysicalCriticalRate, WORD wLastEnergyCriticalRate,
		WORD wAttackSpeedRate,
		WORD wLastMaxLp, WORD wLastMaxEp, WORD wCurLp, WORD wCurEp, BYTE byCurRpBall,
		BYTE byLevel, BYTE byMobGrade);

float Dbo_ConvertToAgentRadius(float fObjectRadius);

float Dbo_GetTransformScale(eASPECTSTATE eAspect, BYTE byGrade);            ///< 변신시에 변경 비율을 반환한다.

const BYTE CalculateRPBallMaxByLevel( const BYTE byLevel );

DWORD GetSafeRetAdd( DWORD dwMax, DWORD dwCur, DWORD dwAdd );

// Use DBO_RP_CHARGE_INTERVAL_IN_MILLISECS instead of the following function.
// 아래의 함수 대신에 DBO_RP_CHARGE_INTERVAL_IN_MILLISECS를 써야 한다.
// by YOSHIKI(2009-07-28)
inline DWORD Dbo_GetRpChargeIntervalInMilliSecs(float fFormulaTableValue)
{
	float fInterval = fFormulaTableValue * 1000.f;

	if (1.0f > fInterval)
	{
		return 1;
	}
	else
	{
		return (DWORD)fInterval;
	}
}

// Use DBO_RP_DIMIMUTION_INTERVAL_IN_MILLISECS instead of the following function.
// 아래의 함수 대신에 DBO_RP_DIMIMUTION_INTERVAL_IN_MILLISECS를 써야 한다.
inline DWORD Dbo_GetRpDiminutionIntervalInMilliSecs(float fFormulaTableValue)
{
	float fInterval = fFormulaTableValue * 1000.f;

	if (1.0f > fInterval)
	{
		return 1;
	}
	else
	{
		return (DWORD)fInterval;
	}
}
