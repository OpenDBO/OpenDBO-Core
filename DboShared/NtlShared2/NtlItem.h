//***********************************************************************************
//
//	File		:	NtlItem.h
//
//	Begin		:	2006-04-19
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once
#include "NtlSharedType.h"
#include "NtlSharedDef.h"
#include "NtlVector.h"
#include "NtlQuest.h"
#include "NtlStringHandler.h"


//-----------------------------------------------------------------------------------
// 아이템 타입
//-----------------------------------------------------------------------------------
enum eITEM_TYPE
{
	ITEM_TYPE_GLOVE,
	ITEM_TYPE_STAFF,
	ITEM_TYPE_GUN,
	ITEM_TYPE_DUAL_GUN,
	ITEM_TYPE_CLAW,
	ITEM_TYPE_AXE,
	ITEM_TYPE_SCROLL,
	ITEM_TYPE_GEM,
	ITEM_TYPE_STICK,
	ITEM_TYPE_SWORD,
	ITEM_TYPE_FAN,
	ITEM_TYPE_WAND,
	ITEM_TYPE_BAZOOKA,
	ITEM_TYPE_BACK_PACK,
	ITEM_TYPE_INSTRUMENT,
	ITEM_TYPE_CLUB,
	ITEM_TYPE_DRUM,
	ITEM_TYPE_MASK,

	ITEM_TYPE_JACKET,
	ITEM_TYPE_PANTS,
	ITEM_TYPE_BOOTS,

	ITEM_TYPE_NECKLACE,
	ITEM_TYPE_EARRING,
	ITEM_TYPE_RING,

	ITEM_TYPE_SCOUTER,
	ITEM_TYPE_SCOUTER_PART,
	
	ITEM_TYPE_COSTUME,
	ITEM_TYPE_BAG,
	ITEM_TYPE_QUEST,
	ITEM_TYPE_STONE,

	ITEM_TYPE_RECOVER,
	ITEM_TYPE_FOOD,
	ITEM_TYPE_UTILITY,
	ITEM_TYPE_CHARM,
	ITEM_TYPE_CAPSULE,			// VEHICLE 탈것이나 호이포이머신 캡슐하우스등 던저서 생성되는 캡슐형태로된 아이템
	ITEM_TYPE_FUEL,
	ITEM_TYPE_JUNK,
	ITEM_TYPE_COLLECTION,
	ITEM_TYPE_WAREHOUSE,			// 창고 추가용 가방아이템에 사용할것임
	ITEM_TYPE_DRAGONBALL,
	ITEM_TYPE_GAMBLE,
	ITEM_TYPE_MATERIAL,			// 조합재료
	ITEM_TYPE_RECIPE,			
	ITEM_TYPE_HOIPOIROCK,
	ITEM_TYPE_DOGI,
	ITEM_TYPE_PURE_STONE,
	ITEM_TYPE_BLACK_STONE,
	ITEM_TYPE_NETPYSTORE,

	ITEM_TYPE_COUNT,
	ITEM_TYPE_UNKNOWN    = 0xFF,

	ITEM_TYPE_WEAPON_FIRST = ITEM_TYPE_GLOVE,
	ITEM_TYPE_WEAPON_LAST  = ITEM_TYPE_MASK,

	ITEM_TYPE_ARMOR_FIRST = ITEM_TYPE_JACKET,
	ITEM_TYPE_ARMOR_LAST  = ITEM_TYPE_BOOTS,

	ITEM_TYPE_GROUP_WEAPON = 0,
	ITEM_TYPE_GROUP_ARMOR  = 1,
	ITEM_TYPE_GROUP_ETC	   = 2,

	ITEM_TYPE_FIRST = ITEM_TYPE_GLOVE,
	ITEM_TYPE_LAST = ITEM_TYPE_COUNT - 1
};

//-----------------------------------------------------------------------------------
// 기간 아이템 지불 방식
//-----------------------------------------------------------------------------------
enum eDURATION_ITEM_PAY_TYPE
{
	DURATION_ITEM_PAY_TYPE_ZENNY,	// 제니
	DURATION_ITEM_PAY_TYPE_NETPY,	// 넷피
	DURATION_ITEM_PAY_TYPE_CASH,	// 캐쉬
};
//-----------------------------------------------------------------------------------
// 장착 아이템 타입
//-----------------------------------------------------------------------------------
enum eEQUIP_TYPE
{
	EQUIP_TYPE_MAIN_WEAPON,
	EQUIP_TYPE_SUB_WEAPON,
	EQUIP_TYPE_ARMOR,
	EQUIP_TYPE_SCOUTER,					// Scoute Body ( 스카우터 가방 들어가는곳 )
	EQUIP_TYPE_COSTUME,
	EQUIP_TYPE_ACCESSORY,

	EQUIP_TYPE_COUNT,
	EQUIP_TYPE_UNKNOWN				= 0xFF,

	EQUIP_TYPE_FIRST = EQUIP_TYPE_MAIN_WEAPON,
	EQUIP_TYPE_LAST = EQUIP_TYPE_COUNT - 1
};


//-----------------------------------------------------------------------------------
// 아이템 등급
//-----------------------------------------------------------------------------------
enum eITEM_RANK
{
	ITEM_RANK_NOTHING,
	ITEM_RANK_NORMAL,
	ITEM_RANK_SUPERIOR,
	ITEM_RANK_EXCELLENT,
	ITEM_RANK_RARE,
	ITEM_RANK_LEGENDARY,

	ITEM_RANK_COUNT,
	ITEM_RANK_UNKNOWN				= 0xFF,

	ITEM_RANK_FIRST = ITEM_RANK_NOTHING,
	ITEM_RANK_LAST = ITEM_RANK_LEGENDARY
};

//-----------------------------------------------------------------------------------
// 아이템 모델 타입
//-----------------------------------------------------------------------------------
enum eITEM_MODEL_TYPE
{
	ITEM_MODEL_TYPE_NONE,
	ITEM_MODEL_TYPE_RACE_GENDER,
	ITEM_MODEL_TYPE_RACE_GENDER_SKIN,

	ITEM_MODEL_TYPE_COUNT,
	ITEM_MODEL_TYPE_UNKNOWN				= 0xFF,

	ITEM_MODEL_TYPE_FIRST = ITEM_MODEL_TYPE_NONE,
	ITEM_MODEL_TYPE_LAST  = ITEM_MODEL_TYPE_RACE_GENDER_SKIN
};

//-----------------------------------------------------------------------------------
// 아이템 백 사이즈 정의
//-----------------------------------------------------------------------------------
enum eITEM_BAG_SIZE
{
	ITEM_BAG_SIZE_4_BY_1			= 4,
	ITEM_BAG_SIZE_4_BY_2			= 8,
	ITEM_BAG_SIZE_4_BY_3			= 12,
	ITEM_BAG_SIZE_4_BY_4			= 16,
};

//-----------------------------------------------------------------------------------
// 아이템 UPGRADE GRADE정의
//-----------------------------------------------------------------------------------
enum eITEM_GRADE_LEVEL
{
	ITEM_GRADE_LEVEL_0 = 0,		
	ITEM_GRADE_LEVEL_1,			
	ITEM_GRADE_LEVEL_2,			
	ITEM_GRADE_LEVEL_3,			
	ITEM_GRADE_LEVEL_4,			

	ITEM_GRADE_LEVEL_INVALID	= 0xFF
};
//-----------------------------------------------------------------------------------
// 컨테이너 타입
//-----------------------------------------------------------------------------------
enum eCONTAINER_TYPE
{
	//DB Table( Item )에 저장
	CONTAINER_TYPE_BAGSLOT,				// 가방이 들어갈곳 (0~5) 각각의 가방이 활성화
	CONTAINER_TYPE_BAG1,				// 위의 Bag이 활성화 된경우 각 가방에 들어가는 인벤들...	
	CONTAINER_TYPE_BAG2,
	CONTAINER_TYPE_BAG3,
	CONTAINER_TYPE_BAG4,
	CONTAINER_TYPE_BAG5,

	CONTAINER_TYPE_NETPYSTORE,

	CONTAINER_TYPE_EQUIP,
	CONTAINER_TYPE_SCOUT,				// Place 가 7인 가방과 같은 역할 Scouter Parts 가 들어 갈곳

	//DB Table(Bank)에 저장
	CONTAINER_TYPE_BANKSLOT,
	CONTAINER_TYPE_BANK1,
	CONTAINER_TYPE_BANK2,
	CONTAINER_TYPE_BANK3,
	CONTAINER_TYPE_BANK4,

	//DB Table(Guild Bank)에 저장
	CONTAINER_TYPE_GUILD1,
	CONTAINER_TYPE_GUILD2,
	CONTAINER_TYPE_GUILD3,

	CONTAINER_TYPE_COUNT,
	CONTAINER_TYPE_NONE = 0xFF,

	CONTAINER_TYPE_BAG_FIRST = CONTAINER_TYPE_BAG1,
	CONTAINER_TYPE_BAG_LAST = CONTAINER_TYPE_NETPYSTORE,

	CONTAINER_TYPE_INVEN_FIRST = CONTAINER_TYPE_BAGSLOT,
	CONTAINER_TYPE_INVEN_LAST = CONTAINER_TYPE_SCOUT,
	
	CONTAINER_TYPE_BANK_FIRST = CONTAINER_TYPE_BANKSLOT,
	CONTAINER_TYPE_BANK_LAST  = CONTAINER_TYPE_BANK4,

	CONTAINER_TYPE_GUILD_FIRST = CONTAINER_TYPE_GUILD1,
	CONTAINER_TYPE_GUILD_LAST  = CONTAINER_TYPE_GUILD3,

	CONTAINER_TYPE_FIRST = CONTAINER_TYPE_BAGSLOT,
	CONTAINER_TYPE_LAST = CONTAINER_TYPE_BANK4,

};

//-----------------------------------------------------------------------------------
// Bag Slot Position
//-----------------------------------------------------------------------------------
enum eBAGSLOT_POSITION
{
	BAGSLOT_POSITION_BAGSLOT_POSITION_0 = 0,
	BAGSLOT_POSITION_BAGSLOT_POSITION_1,
	BAGSLOT_POSITION_BAGSLOT_POSITION_2,
	BAGSLOT_POSITION_BAGSLOT_POSITION_3,
	BAGSLOT_POSITION_BAGSLOT_POSITION_4,

	BAGSLOT_POSITION_BAGSLOT_POSITION_NETPYSTORE = 5,

	BAGSLOT_POSITION_NONE = 0xFF,
};


//-----------------------------------------------------------------------------------
// 장착 아이템 슬롯
//-----------------------------------------------------------------------------------
enum eEQUIP_SLOT_TYPE
{
	EQUIP_SLOT_TYPE_HAND, 
	EQUIP_SLOT_TYPE_SUB_WEAPON,
	EQUIP_SLOT_TYPE_JACKET,
	EQUIP_SLOT_TYPE_PANTS,
	EQUIP_SLOT_TYPE_BOOTS,
	EQUIP_SLOT_TYPE_SCOUTER,
	EQUIP_SLOT_TYPE_COSTUME,
	EQUIP_SLOT_TYPE_NECKLACE,
	EQUIP_SLOT_TYPE_EARRING_1,
	EQUIP_SLOT_TYPE_EARRING_2,
	EQUIP_SLOT_TYPE_RING_1,
	EQUIP_SLOT_TYPE_RING_2,

	EQUIP_SLOT_TYPE_COUNT,
	EQUIP_SLOT_TYPE_UNKNOWN	= 0xFF,

	EQUIP_SLOT_TYPE_FIRST	= EQUIP_SLOT_TYPE_HAND,
	EQUIP_SLOT_TYPE_LAST	= EQUIP_SLOT_TYPE_RING_2,
};

//-----------------------------------------------------------------------------------
// Costume을 착용하거나 벗을때 캐릭터에서 각각 detach, attach이 되어야 할 Item들에
// 대해 BitFlagSet을 활용하도록 하며 다음은 기획파트에 BitFlagTool을 이용하여 Set을
// 만들때 필요한 상수값을 정의하며 프로그램에서 확인할수 있는 실 FLAG에 대해서는 
// 0x01을 SHIFT하여 사용한다. 고로 32개까지만 추가수정될수 있으며 추가수정사항에
// 대해서는 매우 주의를 요한다.
//
// 2008.03.24 우디
//-----------------------------------------------------------------------------------
enum eCOSTUME_BITFLAG_SLOT_TYPE
{
	eCBST_JACKET,
	eCBST_PANTS,
	eCBST_BOOTS,
	eCBST_TWO_HAND,
	eCBST_SUB_WEAPON_BACK,
	eCBST_HEAD,
	eCBST_FACE,
	eCBST_SCOUTER_AT_EYE,
};

enum eCOSTUME_BITFLAG_SLOT_TYPE_SHIFT
{
	eCBSTS_JACKET			= 0x01 << eCBST_JACKET,
	eCBSTS_PANTS			= 0x01 << eCBST_PANTS,
	eCBSTS_BOOTS			= 0x01 << eCBST_BOOTS,
	eCBSTS_TWO_HAND			= 0x01 << eCBST_TWO_HAND,
	eCBSTS_SUB_WEAPON_BACK	= 0x01 << eCBST_SUB_WEAPON_BACK,
	eCBSTS_HEAD				= 0x01 << eCBST_HEAD,
	eCBSTS_FACE				= 0x01 << eCBST_FACE,
	eCBSTS_SCOUTER_AT_EYE	= 0x01 << eCBST_SCOUTER_AT_EYE,
};

//-----------------------------------------------------------------------------------
// 아이템 테이블 타입
//-----------------------------------------------------------------------------------
enum eITEM_TABLE_TYPE
{
	ITEM_TABLE_TYPE_ITEM,
	ITEM_TABLE_TYPE_BANK,
	ITEM_TABLE_TYPE_BANK_PUBLIC,
	ITEM_TABLE_TYPE_GUILD_BANK,

	ITEM_TABLE_TYPE_NONE = 0xFF,

	ITEM_TABLE_TYPE_FIRST = ITEM_TABLE_TYPE_ITEM,
	ITEM_TABLE_TYPE_LAST = ITEM_TABLE_TYPE_GUILD_BANK,
};

//-----------------------------------------------------------------------------------
// Bank 아이템 테이블 타입
//-----------------------------------------------------------------------------------
enum eBANK_TABLE_TYPE
{
	BANK_TABLE_TYPE_BASIC,
	BANK_TABLE_TYPE_ADD1,
	BANK_TABLE_TYPE_ADD2,
	BANK_TABLE_TYPE_PUBLIC,

	BANK_TABLE_TYPE_NONE = 0xFF,

	BANK_TABLE_TYPE_FIRST = BANK_TABLE_TYPE_BASIC,
	BANK_TABLE_TYPE_LAST = BANK_TABLE_TYPE_PUBLIC,
};

//-----------------------------------------------------------------------------------
// 장착 아이템 슬롯 검사용 비트 플래그
//-----------------------------------------------------------------------------------
enum eEQUIP_SLOT_FLAG
{
	EQUIP_SLOT_FLAG_HAND			= 0x01 << EQUIP_SLOT_TYPE_HAND,
	EQUIP_SLOT_FLAG_SUB_WEAPON		= 0x01 << EQUIP_SLOT_TYPE_SUB_WEAPON,
	EQUIP_SLOT_FLAG_JACKET			= 0x01 << EQUIP_SLOT_TYPE_JACKET,
	EQUIP_SLOT_FLAG_PANTS			= 0x01 << EQUIP_SLOT_TYPE_PANTS,
	EQUIP_SLOT_FLAG_BOOTS			= 0x01 << EQUIP_SLOT_TYPE_BOOTS,
	EQUIP_SLOT_FLAG_SCOUTER			= 0x01 << EQUIP_SLOT_TYPE_SCOUTER,
	EQUIP_SLOT_FLAG_COSTUME			= 0x01 << EQUIP_SLOT_TYPE_COSTUME,
	EQUIP_SLOT_FLAG_NECKLACE		= 0x01 << EQUIP_SLOT_TYPE_NECKLACE,
	EQUIP_SLOT_FLAG_EARRING_1		= 0x01 << EQUIP_SLOT_TYPE_EARRING_1,
	EQUIP_SLOT_FLAG_EARRING_2		= 0x01 << EQUIP_SLOT_TYPE_EARRING_2,
	EQUIP_SLOT_FLAG_RING_1			= 0x01 << EQUIP_SLOT_TYPE_RING_1,
	EQUIP_SLOT_FLAG_RING_2			= 0x01 << EQUIP_SLOT_TYPE_RING_2,
};

//-----------------------------------------------------------------------------------
// 아이템 옵션 품질
//-----------------------------------------------------------------------------------
enum eOPTION_QUALITY
{
	OPTION_QUALITY_SUPERIOR,
	OPTION_QUALITY_EXCELLENT,
	OPTION_QUALITY_RARE,
	OPTION_QUALITY_LEGENDARY,
	OPTION_QUALITY_SET,

	OPTION_QUALITY_COUNT,
	OPTION_QUALITY_UNKNOWN	= 0xFF,

	OPTION_QUALITY_FIRST	= OPTION_QUALITY_SUPERIOR,
	OPTION_QUALITY_LAST  	= OPTION_QUALITY_SET,
};
//-----------------------------------------------------------------------------------
// 아이템 펑션 타입
//-----------------------------------------------------------------------------------
enum eITEM_FUNC_TYPE
{
	ITEM_FUNC_TYPE_MAIN_WEAPON_OFF,
	ITEM_FUNC_TYPE_TRADE_FORBIDDEN,
	ITEM_FUNC_TYPE_STORE_FORBIDDEN,
	ITEM_FUNC_TYPE_SELL_FORBIDDEN,
};

//-----------------------------------------------------------------------------------
// 아이템 펑션 타입 플래그
//-----------------------------------------------------------------------------------
enum eITEM_FUNC_BIT_FLAG
{
	ITEM_FUNC_BIT_FLAG_MAIN_WEAPON_OFF  = 0x01 << ITEM_FUNC_TYPE_MAIN_WEAPON_OFF,
	ITEM_FUNC_BIT_FLAG_TRADE_FORBIDDEN  = 0x01 << ITEM_FUNC_TYPE_TRADE_FORBIDDEN,
	ITEM_FUNC_BIT_FLAG_STORE_FORBIDDEN  = 0x01 << ITEM_FUNC_TYPE_STORE_FORBIDDEN,
	ITEM_FUNC_BIT_FLAG_SELL_FORBIDDEN   = 0x01 << ITEM_FUNC_TYPE_SELL_FORBIDDEN,
};

//-----------------------------------------------------------------------------------
// 아이템 옵션 등급
//-----------------------------------------------------------------------------------
enum eOPTION_RANK
{
	OPTION_RANK_1 =	1,
	OPTION_RANK_2 =	3,
	OPTION_RANK_3 =	5,
	OPTION_RANK_4 =	10,
	OPTION_RANK_5 =	20,
	OPTION_RANK_6 =	25,
	OPTION_RANK_7 =	36,   // 옵션 미니테이블 참조 % 희소성 모두 합치면 100%

	OPTION_RANK_COUNT = 7,
};
////-----------------------------------------------------------------------------------
//// 호이포이 아이템 등급
////-----------------------------------------------------------------------------------
//enum eHOIPOI_RANK		//업그레이드에 필요한 호이포이스톤의 등급은 레벨에 따라 등급을 달리한다.
//{
//	HOIPOI_RANK_1 = 15, 
//	HOIPOI_RANK_2 =	100,
//};
//-----------------------------------------------------------------------------------
// 아이템 옵션 Quality등급
//-----------------------------------------------------------------------------------
enum eOPTION_QUALITY_RANK		//업그레이드에 필요한 호이포이스톤의 등급은 레벨에 따라 등급을 달리한다.
{
	OPTION_QUALITY_RANK_0 = 100,
	OPTION_QUALITY_RANK_1 = 64, 
	OPTION_QUALITY_RANK_2 =	39,
	OPTION_QUALITY_RANK_3 =	19,
	OPTION_QUALITY_RANK_4 =	9,
	OPTION_QUALITY_RANK_5 = 4,
	OPTION_QUALITY_RANK_6 = 1,

	OPTION_QUALITY_RANK_COUNT = 7, 
};
//-----------------------------------------------------------------------------------
// 객체 업데이트 타입 (현재 서버 내부용)
//-----------------------------------------------------------------------------------
enum eSTUFF_UPDATE_TYPE
{
	STUFF_UPDATE_INCREASE,
	STUFF_UPDATE_DECREASE,
	STUFF_UPDATE_REPLACE,

};

//-----------------------------------------------------------------------------------
// 아이템 사용 시의 CoolTime Group
//-----------------------------------------------------------------------------------
enum eITEM_COOL_TIME_GROUP
{
	ITEM_COOL_TIME_GROUP_DH_LP = 0,
	ITEM_COOL_TIME_GROUP_DH_EP,
	ITEM_COOL_TIME_GROUP_HOT_LP,
	ITEM_COOL_TIME_GROUP_HOT_EP,
	ITEM_COOL_TIME_GROUP_DH_RESCUE,
	ITEM_COOL_TIME_GROUP_DH_POISON,
	ITEM_COOL_TIME_GROUP_DH_STOMACHACHE,
	ITEM_COOL_TIME_GROUP_TELEPORT,
	ITEM_COOL_TIME_GROUP_CB_OFFENCE,
	ITEM_COOL_TIME_GROUP_CB_DEFENCE,
	ITEM_COOL_TIME_GROUP_HOT_FOOD,
	ITEM_COOL_TIME_GROUP_HOT_DRINK,

	ITEM_COOL_TIME_GROUP_COUNT,

	ITEM_COOL_TIME_GROUP_FIRST = ITEM_COOL_TIME_GROUP_DH_LP,
	ITEM_COOL_TIME_GROUP_LAST = ITEM_COOL_TIME_GROUP_COUNT - 1
};

//-----------------------------------------------------------------------------------
// 아이템 스카우터 파츠 기능 타입정의
//-----------------------------------------------------------------------------------
enum eSCOUTER_PARTS_TYPE
{
	SCOUTER_PARTS_NONE,
	SCOUTER_PARTS_UPGRADE_PREDICTION,
	SCOUTER_PARTS_ITEM_IDENTIFICATION,
	SCOUTER_PARTS_PC_INFO_VIEWER,
	SCOUTER_PARTS_PC_INFO_JAMMING,
	SCOUTER_PARTS_OBJECT_FINDER,
	SCOUTER_PARTS_MAILER,
	SCOUTER_PARTS_TRANSMITTER,
	SCOUTER_PARTS_DRAGON_BALL_SEARCH,
	SCOUTER_PARTS_QUEST_SEARCH,
	
	SCOUTER_PARTS_COUNT,

	SCOUTER_PARTS_FIRST = SCOUTER_PARTS_NONE ,
	SCOUTER_PARTS_LAST = SCOUTER_PARTS_DRAGON_BALL_SEARCH
};
//-----------------------------------------------------------------------------------
// 옵션 테이블의 스카우터 인포의 미니테이블
//-----------------------------------------------------------------------------------
enum eSCOUTER_INFO
{
	SCOUTER_INFO_BEGIN_DEFAULT_VALUE = 0,
	SCOUTER_INFO_LP = SCOUTER_INFO_BEGIN_DEFAULT_VALUE,
	SCOUTER_INFO_EP,
	SCOUTER_INFO_STR,
	SCOUTER_INFO_CON,
	SCOUTER_INFO_FOC,
	SCOUTER_INFO_DEX,
	SCOUTER_INFO_SOL,
	SCOUTER_INFO_ENG,
	SCOUTER_INFO_ATTACK_SPEED,
	SCOUTER_INFO_ATTACK_RATE,
	SCOUTER_INFO_DODGE_RATE,
	SCOUTER_INFO_BLOCK_RATE,
	SCOUTER_INFO_CURSE_SUCCESS_RATE,
	SCOUTER_INFO_CURSE_TOLERANCE_RATE,
	SCOUTER_INFO_UPGRADE_RATE_UP,			///< 아이템 확률 %d 배
	SCOUTER_INFO_ITEM_BREAK_RATE_DOWN,		///< 아이템 파괴 확률 -%d

	SCOUTER_INFO_COUNT,
	
	SCOUTER_INFO_FIRST = SCOUTER_INFO_LP,
	SCOUTER_INFO_LAST = SCOUTER_INFO_ITEM_BREAK_RATE_DOWN,
	
	SCOUTER_INFO_BEGIN_ONLY_ALLPY_MAX   = 30,	///< 이후는 가장 좋은 기능의 파츠 하나만 표시된다

	SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION = 100,	///< 이후는 테이블에 설명된 그대로 보여준다
	SCOUTER_INFO_RESERVATED = SCOUTER_INFO_BEGIN_SPECIAL_FUNCTION,
	SCOUTER_INFO_PC_INFO_JAMMING,									///< 엿보기 방해
	SCOUTER_INFO_OBJECT_FINDER,										///< 숨은 물건 찾기
	SCOUTER_INFO_CONNECTOR,											///< 우편함 접속
	SCOUTER_INFO_TRANSMITTER,										///< 물질 전송
	SCOUTER_INFO_DRAGON_BALL_SEARCH,								///< 드래곤볼 소지 몬스터 표시
	SCOUTER_INFO_ITEM_IDENTIFICATION,								///< 아이템 감정
	SCOUTER_INFO_PC_INFO_VIEWER,									///< 수행자 엿보기
	SCOUTER_INFO_QUEST_SEARCH,										///< 주변퀘스트 검색
	
};

// 아이템 처리 방법 [7/23/2008 SGpro]
enum eITEM_PROCESS_TYPE
{
	 ITEM_PROCESS_USEITEM = 0 // Item Use에 의해 사용 Replace됨 [7/23/2008 SGpro]
};

//-----------------------------------------------------------------------------------
// 아이템 레시피 타입
//-----------------------------------------------------------------------------------
enum eITEM_RECIPE_TYPE
{
	ITEM_RECIPE_TYPE_FOOD_DRINK,
	ITEM_RECIPE_TYPE_MAIN_WEAPON,
	ITEM_RECIPE_TYPE_ARMOR,
	ITEM_RECIPE_TYPE_ACCESSORY,
	ITEM_RECIPE_TYPE_SCOUTER,
	ITEM_RECIPE_TYPE_SUB_WEAPON,
};

//-----------------------------------------------------------------------------------
// 아이템 구매시 필요한 권한
//-----------------------------------------------------------------------------------
enum eITEM_NEED_FUNCTION
{
	ITEM_NEED_FUNCTION_FOUNDATION,				// 도장설립
	ITEM_NEED_FUNCTION_MAKING_DOGI,				// 도복제작
	ITEM_NEED_FUNCTION_BUYING_VEHICLE,			// 탈것구매	
	ITEM_NEED_FUNCTION_BUYING_MANUAL,			// 교본구매

	ITEM_NEED_FUNCTION_FIRST = ITEM_NEED_FUNCTION_FOUNDATION ,
	ITEM_NEED_FUNCTION_LAST  = ITEM_NEED_FUNCTION_BUYING_MANUAL
};

//-----------------------------------------------------------------------------------
// 아이템 관련 정의
//-----------------------------------------------------------------------------------
const int			NTL_MAX_ITEM_STACK_COUNT	= 20; // 스택 아이템의 최대 스택 개수

const DWORD			NTL_DEF_ITEM_ATTACK_SPEEED = 1000; // 아이템의 기준 공격 스피드

const int			NTL_MAX_SYSTEM_EFFECT_COUNT	= 4;

const int			NTL_MAX_OPTION_COUNT		= 21; // 옵션아이템의 선별 옵션 최대 갯수 

const int			NTL_DEF_LUCKY_CHANCE_RATE   = 10;  // 럭키 드랍시에 올라가는 드랍 배율

const int			NTL_DEF_MULTI_OPTION_RATE   = 30;  // superial item 의 multi option 배율

const int			NTL_MAX_MERCHANT_COUNT		= 24;  // 상점탭에 전시하는 최대갯수

//const int			NTL_MAX_BASIC_DROP			= 12;  // 기본 아이템 드랍 최대수

const int			NTL_MAX_SUPERIOR_DROP		= 18;  // Superior 아이템 드랍 최대수

const int			NTL_MAX_LEGENDARY_DROP		= 18;  // 유니크 아이템 드랍 최대수 

const int			NTL_MAX_NORMAL_DROP			= 18;  // 노말 아이템 드랍 최대수 

const int			NTL_MAX_EXCELLENT_DROP		= 18;  // 엑설런트 아이템 드랍 최대수 

const int			NTL_MAX_EACH_DROP			= 12;  // 이치 아이템 드랍 최대수 

const int			NTL_MAX_TYPE_DROP			= 12;  // 타입 아이템 드랍 최대수 

const int			NTL_MAX_DROP_TABLE_SELECT	= 8;   // 아이템 드랍 택일 최대수

const int			NTL_MAX_IDENTIFY_DROP_RATE	= 10;  // Unidentifyed 아이템 드랍 확률% 

const int			NTL_TICK_OF_DROP_OWNNER		= 30000; // 소유권유지시간 30000미리세크 즉 30초

const int			NTL_TICK_OF_DROP_DISAPPEAR	= 40000; // 아이템 사라지는시간 40초	

const int			NTL_MAX_MERCHANT_TAB_COUNT  = 6; 

const int			NTL_DEF_OPTION_LEVEL_COUNT  = 20;   // 100Level 일 경우 5렙으로 20구간 

const DWORD			NTL_MAX_EFFECT_IN_ITEM = 2;		// 아이템이 가질 수 있는 system effect의 최대 갯수

const DWORD			NTL_MAX_OPTION_IN_ITEM = 2;		// 아이템이 가질 수 있는 Option의 최대 갯수

const float			NTL_MAX_MERCHANT_DISTANCE = 3.0f;	// 아이템 상인간의 허용 최대 거리

const float			NTL_MAX_MERCHANT_DISTANCE_EXTRA = 1.0f;	// 아이템 상인간의 허용 최대 거리의 오차 범위(for sync.)

const float			NTL_MAX_NAMEKAN_DISTANCE = 15.0f;	// 아이템 나메칸사인의 허용 최대 거리

const float			NTL_MAX_BUS_DISTANCE = 20.0f;		// PC BUS의 허용 최대 거리

const float			NTL_MAX_LOOTING_DISTANCE = 5.0f;	// 아이템이나 제니를 주울 때의 허용 최대 거리

const float			NTL_MAX_LOOTING_DISTANCE_EXTRA = 1.0f;	// 아이템이나 제니를 주울 때의 허용 최대 거리의 오차 범위(for sync.)

const float			NTL_MAX_OBJECT_DISTANCE = 5.0f;	// 아이템이나 제니를 주울 때의 허용 최대 거리

const float			NTL_MAX_OBJECT_DISTANCE_EXTRA = 1.0f;	// 아이템이나 제니를 주울 때의 허용 최대 거리의 오차 범위(for sync.)

const BYTE			NTL_DUR_UNKNOWN				= 0xFF;

const int			NTL_UNIDENTIFY_LEGNEDARY_RATE = 50;

const int			NTL_UNIDENTIFY_RARE_RATE      = 30;

const int			NTL_UNIDENTIFY_EXCELLENT_RATE = 20;

const int			NTL_UNIDENTIFY_SUPERIOR_RATE  = 10;

const int			NTL_UNIDENTIFY_NORMAL_RATE  = 5;

const int			NTL_ITEM_MAX_GRADE			= 15;

const int			NTL_ITEM_MAX_RANK			= ITEM_RANK_LAST;	// NONE 아이템 을 제외한 갯수

const int			NTL_ITEM_COUNT_GRADE		= 5;

const int			NTL_ITEM_UPGRADE_EQUIP_COUNT = 5;	// Cheat 로 올릴 장비의 총 갯수

const int			NTL_ITEM_OPTION_BIND_LEVEL	= 5;	//  옵션을 붙일때 쓰일 나눌 구간 

const DWORD			NTL_ITEM_ALL_USE_FLAG       = 0xFFFFFFFF;

const BYTE			NTL_UNSTACKABLE_ITEM_COUNT	= 1;

const int			NTL_ITEM_MAX_DRAGONBALL		= 7;

const int			NTL_ITEM_MAX_BALL			= 9;	// 드롭,정크 볼 포함

const float			DBO_ITEM_CLASS_DEFENCE_BONUS_IN_PERCENT = 20.0f;		// 직업별 방어구의 특화 보너스

const int			NTL_ITEM_MAX_UPGRADE_POSITION = 14;

const int			NTL_SHOP_ITEM_IDENTIFY_ZENNY = 100;

const int			NTL_ITEM_RECIPE_MAX_COUNT = 100;

const int			NTL_ITEM_RECIPE_NEED_ITEM_COUNT = 20;

const DWORD			DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM = 5;

const DWORD			DBO_HOIPOIMIX_BASIC_EXP = 100;

const BYTE			DBO_HOIPOIMIX_CHANGE_EP_RATE = 10;			// 아이템 믹스시 사용 EP의 양은 레시피레벨에 DBO_HOIPOIMIX_CHANGE_EP_RATE 을 곱한값

const BYTE			DBO_HOIPOIMIX_CASH_RATE_MIN = 50;

const BYTE			DBO_HOIPOIMIX_CASH_RATE_MAX = 200;

const BYTE			DBO_HOIPOIMIX_MIX_LEVEL_MAX = 100;
//-----------------------------------------------------------------------------------
// 쇼핑카트 구조체
//-----------------------------------------------------------------------------------
struct sSHOP_BUY_CART
{
	BYTE byMerchantTab;
	BYTE byItemPos;
	BYTE byStack;
};
struct sSHOP_SELL_CART
{
	BYTE byPlace;
	BYTE byPos;
	BYTE byStack;
};

struct sEMPTY_INVEN
{
	BYTE byPlace;
	BYTE byPos;
};
#pragma pack(1)

struct sSHOP_BUY_INVEN
{
	BYTE byPlace;
	BYTE byPos;
	BYTE byStack;
	TBLIDX tblItem;
	BYTE byRank;
	BYTE byCurrentDurability;
	BYTE byGrade;
	BYTE byRestrictType;			// 귀속상태 eITEM_RESTRICT_TYPE
	WCHAR awchMaker[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1]; // 제작자

	TBLIDX aOptionTblidx[NTL_MAX_OPTION_IN_ITEM];

	BYTE byDurationType; //eDURATIONTYPE
	DBOTIME nUseStartTime;// 인벤토리에 들어온 날짜
	DBOTIME nUseEndTime;// 사용만료 기간
};
#pragma pack()
struct sSHOP_SELL_INVEN
{
	ITEMID itemID;
	BYTE   byStack;
	BYTE   byPlace;
	BYTE   byPos;
};


//-----------------------------------

struct sINVEN_SLOT
{
	BYTE	byPlace;
	BYTE	byPos;
};

struct sITEMID_SLOT : public sINVEN_SLOT
{
	HOBJECT	hItem;
	ITEMID	itemId;
};

struct sHITEM_SLOT : public sINVEN_SLOT
{
	HOBJECT	hItem;
};

// query와의 통신에서 사용
struct sITEM_MOVE_DATA
{
	sITEMID_SLOT	sSrc;
	sITEMID_SLOT	sDest;
};

// client와의 통신에서 사용
struct sITEM_MOVE_INFO
{
	sHITEM_SLOT		sSrc;	// 이동할 아이템과 이동할 아이템이 있는 위치.
	sHITEM_SLOT		sDest;	// 목적지에 이미 있는 아이템과 목적지의 위치
};


//-----------------------------------------------------------------------------------
// 인벤토리 검사 타입 (서버 내부용)
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// 인벤토리 관련 정의
//-----------------------------------------------------------------------------------
const int			NTL_MAX_BAGSLOT_COUNT		=  6; // 가방 슬롯의 개수

const int			NTL_MAX_BANKSLOT_COUNT		=  4; // 창고 슬롯의 개수

const int			NTL_MAX_GUILD_BANK_COUNT	=  3; // 길드 창고 개수

const int			NTL_MAX_EQUIP_ITEM_SLOT		= EQUIP_SLOT_TYPE_COUNT;

const int			NTL_MAX_ITEM_SLOT			= 16; // 슬롯의 최대 크기

const int			NTL_MAX_BAG_ITEM_SLOT		= 16; // 가방의 최대 크기

const int			NTL_MAX_SCOUTER_ITEM_SLOT	= 16; // 스카우터 가방의 최대 크기

const int			NTL_MAX_BANK_ITEM_SLOT		= 16; // 은행의 최대 크기

const int			NTL_MAX_GUILD_ITEM_SLOT		= 16; // 길드 창고의 인벤 최대 크기

const int			NTL_MAX_NETPYSTORE_ITEM_SLOT = 16; //Netpy Store의 인벤 최대 크기

const int			NTL_MAX_COUNT_GUILD_HAVE_TOTAL_ITEM	= NTL_MAX_GUILD_BANK_COUNT * NTL_MAX_GUILD_ITEM_SLOT; // 길드 창고의 최대 크기


const int			NTL_MAX_BUY_SHOPPING_CART	= 12;  // 쇼핑카트의 살수있는 최대 크기

const int			NTL_MAX_SELL_SHOPPING_CART	= 12;  // 쇼핑카트의 팔수있는 최대 크기

const int			NTL_MAX_SHOPPING_CART		= ( (NTL_MAX_BUY_SHOPPING_CART * sizeof(sSHOP_BUY_CART) ) + (NTL_MAX_SELL_SHOPPING_CART * sizeof(sSHOP_SELL_CART) ) );  // 쇼핑카트의 최대 크기

const int			NTL_MAX_NEWBIE_ITEM			= 8;  // 생성시 부여가능 최대 아이템

const int			NTL_MAX_NEWBIE_SKILL		= 3;  // 생성시 부여가능 최대 스킬

const int			NTL_MAX_SCOUTER_WATT		= 1000;

const int			NTL_MAX_SET_ITEM_COUNT		= 3;  // jacket pants boots

const int			NTL_SET_ITEM_SOLO_COUNT		= 1;  // solo count

const int			NTL_SET_ITEM_SEMI_COUNT		= 2;  // semi count

const int			NTL_SET_ITEM_FULL_COUNT		= 3;  // full count

const int			NTL_MAX_BUY_HLSSHOP_CART	= 10;  // full count
//===================================================================================
// 호이포이 아이템 믹스
//===================================================================================

enum eITEM_MIX_RESULT_TYPE
{
	ITEM_MIX_RESULT_TYPE_FAIL,				// 실패
	ITEM_MIX_RESULT_TYPE_SUCCESS,			// 성공
	ITEM_MIX_RESULT_TYPE_GREAT_SUCCESS,		// 대성공

	ITEM_MIX_RESULT_TYPE_NUMS				// 열거형의 갯수
};

enum eRECIPE_TYPE
{
	eRECIPE_NORMAL,
	eRECIPE_MAINWEAPON,		
	eRECIPE_ARMOR,			
	eRECIPE_ACCESSORY,
	eRECIPE_SCOUTER,
	eRECIPE_SUBWEAPON,	

	eRECIPE_INVALID = 0xFF,
};

// 호이포이 믹스 레벨 데이터
struct sHOIPOIMIX_DATA
{
	bool			bNormalStart;	// 노멀등록여부 // 퀘스트 보상 혹은 구매
	
	bool			bSpecialStart;	// 스페샬 믹스 등록여부
	BYTE			bySpecialType;	// eRECIPE_TYPE

	BYTE			byMixLevel;
	DWORD			dwMixExp;
	
	//BYTE			byNormalLevel;	 
	//DWORD			dwNormalExp;
	//BYTE			bySpecialLevel; 	
	//DWORD			dwSpecialExp;
};

// 호이포이 레시피 데이터
struct sRECIPE_DATA
{
	TBLIDX		recipeTblidx;
	BYTE		byRecipeType;		//eRECIPE_TYPE
};

struct sITEM_BASIC_DATA
{
	int	   nItemID;		// DB에서도 쓰기에 int형으로
	BYTE   byPlace;
	BYTE   byPos;
	BYTE   byStack;		
};

struct sITEM_MIX_MAIN_DATA
{
	sITEM_BASIC_DATA		asBasicData[NTL_MAX_BAGSLOT_COUNT * NTL_MAX_BAG_ITEM_SLOT];
};

//-----------------------------------------------------------------------------------
// 아이템 수리 관련 정의
//-----------------------------------------------------------------------------------
struct sITEM_REPAIR
{
	ITEMID itemID;
	BYTE byPlace;
	BYTE byPosition;
	BYTE byDur;			// 올릴 맥스 내구력
};

//-----------------------------------------------------------------------------------
// 인벤토리 검사 타입 (서버 내부용)
//-----------------------------------------------------------------------------------
enum eSTUFF_ENABLE_TYPE
{
	eSTUFF_ENABLE_NONE,			// 검사 없이 통과
	eSTUFF_ENABLE_BASIC,		// 기본 검사만	
	eSTUFF_ENABLE_ALL,			// 모든 검사

};

//-----------------------------------------------------------------------------------
// 아이템 사용 기간
//-----------------------------------------------------------------------------------
enum eDURATIONTYPE
{
	 eDURATIONTYPE_NORMAL = 0
	,eDURATIONTYPE_FLATSUM			//정액제
	,eDURATIONTYPE_METERRATE		//종량제

	,eDURATIONTYPE_COUNT
};

//-----------------------------------------------------------------------------------
// 귀속 아이템 상태
//-----------------------------------------------------------------------------------
enum eITEM_RESTRICT_TYPE
{
	ITEM_RESTRICT_TYPE_NONE,			// 귀속 아이템 아님 검사없음
	ITEM_RESTRICT_TYPE_READY,			// 귀속 아이템 (아직 착용하지 않음)
	ITEM_RESTRICT_TYPE_LIMIT,			// 귀속됨 ( 착용후 귀속됨 )
};


#pragma pack(1)

// 아이템 DB 정보
struct sITEM_DATA
{
	ITEMID			itemId;					// Serial Number
	TBLIDX			itemNo;					// Template Number
	CHARACTERID		charId;					// Owner Serial
	BYTE			byPlace;					// eCONTAINER_TYPE
	BYTE			byPosition;
	BYTE			byStackcount;
	BYTE			byRank;
	BYTE			byCurrentDurability;		// 현재 내구도 
	bool			bNeedToIdentify;
	BYTE			byGrade;				// 아이템 업그레이드 등급
	BYTE			byBattleAttribute;		// 진기맹여락 cf) NtlBattle.h eBATTLE_ATTRIBUTE 

	BYTE			byRestrictType;			// 귀속상태 eITEM_RESTRICT_TYPE
	WCHAR			awchMaker[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1]; // 제작자

	TBLIDX			aOptionTblidx[NTL_MAX_OPTION_IN_ITEM];

	BYTE			byDurationType; //eDURATIONTYPE
	DBOTIME			nUseStartTime;// 인벤토리에 들어온 날짜
	DBOTIME			nUseEndTime;// 사용만료 기간

};

//-----------------------------------------------------------------------------------
// 아이템 기간 관련 정의
//-----------------------------------------------------------------------------------
struct sITEM_DURATION
{
	ITEMID			itemID;
	BYTE			byPlace;
	BYTE			byPosition;
	DBOTIME			nUseEndTime;// 사용만료 기간
};

// 아이템 간략 정보
struct sITEM_BRIEF
{
	TBLIDX			tblidx; // item table index
	BYTE			byRank;
	BYTE            byGrade; // item effect
	BYTE			byBattleAttribute;
	TBLIDX			aOptionTblidx[NTL_MAX_OPTION_IN_ITEM];
};

struct sITEM_SUMMARY
{
	TBLIDX			tblidx; // item table index
	BYTE			byRank;
	BYTE			byGrade;
	BYTE			byBattleAttribute;
	TBLIDX			aOptionTblidx[NTL_MAX_OPTION_IN_ITEM];
};

// 아이템 상태
struct sITEM_STATE
{
	sVECTOR3		vCurLoc; // 현재 위치
	bool			bNeedToIdentify;
	bool			bIsNew;
};


// 아이템 전체 정보
struct sITEM_PROFILE
{
	HOBJECT			handle;			// 아이템 handle
	TBLIDX			tblidx;			// item table index
	BYTE			byPlace;		// eCONTAINER_TYPE
	BYTE			byPos;
	BYTE			byStackcount;
	BYTE			byRank;
	BYTE			byCurDur;		// 내구도
	bool			bNeedToIdentify;
	BYTE			byGrade;		// 아이템 업그레이드 등급
	BYTE			byBattleAttribute;		// 진기맹여락 cf) NtlBattle.h eBATTLE_ATTRIBUTE

	BYTE			byRestrictType;			// 귀속상태 eITEM_RESTRICT_TYPE
	WCHAR			awchMaker[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1]; // 제작자

	TBLIDX			aOptionTblidx[NTL_MAX_OPTION_IN_ITEM];

	BYTE			byDurationType; //eDURATIONTYPE
	DBOTIME			nUseStartTime;// 인벤토리에 들어온 날짜
	DBOTIME			nUseEndTime;// 사용만료 기간
};
// 뱅크 아이템 전체 정보
/*struct sBANK_PROFILE
{
	HOBJECT			handle;			// 아이템 handle
	TBLIDX			tblidx;			// item table index
	BYTE			byPos;
	BYTE			byStackcount;
	BYTE			byRank;
	BYTE			byCurDur;		// 내구도 맥스
	bool			bNeedToIdentify;
	BYTE			byGrade;		// 아이템 업그레이드 등급
	BYTE			byBattleAttribute;		// 진기맹여락 cf) NtlBattle.h eBATTLE_ATTRIBUTE 

	BYTE			byRestrictType;			// 귀속상태 eITEM_RESTRICT_TYPE
	WCHAR			awchMaker[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1]; // 제작자

	TBLIDX			aOptionTblidx[NTL_MAX_OPTION_IN_ITEM];

	BYTE			byDurationType; //eDURATIONTYPE
	DBOTIME			nUseStartTime;// 인벤토리에 들어온 날짜
	DBOTIME			nUseEndTime;// 사용만료 기간
};*/


// Zenny 간략 정보
struct sMONEY_BRIEF
{
	DWORD			dwZenny;
};


// Zenny 상태
struct sMONEY_STATE
{
	sVECTOR3		vCurLoc; // 현재 위치
	bool			bIsNew;
};


// GameServer에서 Query Server로 아이템 삭제를 요청할때 사용
struct sITEM_DELETE_DATA
{
	BYTE			byPlace;	// 장소
	BYTE			byPos;		// 위치
	HOBJECT			hItem;		// 아이템 핸들
	ITEMID			itemId;		// 아이템 Serial
	TBLIDX			tblidx;     // 아이템 tblidx		

public:
	sITEM_DELETE_DATA( void )
		: byPlace( INVALID_BYTE )
		, byPos( INVALID_BYTE )
		, hItem( INVALID_HOBJECT )
		, itemId( INVALID_ITEMID )
		, tblidx( INVALID_TBLIDX )
	{
	}

};

//-----------------------------------------------------------------------
// TRADE
//-----------------------------------------------------------------------
struct sTRADE_INVEN
{
	HOBJECT	hItem;			// 아이템 핸들
	ITEMID itemSerial;		// 옮길 아이템의 DB 시리얼
	BYTE byDstPlace;		// 옮겨질 곳
	BYTE byDstPos;			// 옮겨질 곳
	BYTE byDstCount;		// 옮길 갯수	
	ITEMID	itemNewSerial;	// 다중스택의 일부 아이템을 옮겼을때 새로 생겨난 시리얼
};
const int	TRADE_INVEN_MAX_COUNT = 12;
const DWORD NTL_TRADE_UPDATE_INTERVAL = 5 * 1000;		//5 초
const DWORD NTL_TRADE_REPLY_WAIT_TIME = 30 * 1000;      // 거래요청시 대기하는 시간 30초뒤에도 응답없으면 자동캔슬

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//  [11/20/2006 niam]
// 아이템을 여러개 생성 혹은 삭제 할 때 사용
//-----------------------------------------------------------------------


// 서브 데이터
//	- 아이템 생성 정보 이외에 게임을 진행하는데에 필요한 추가적인 정보들을 저장
//	- 일반적으로 쿼리서버에서 데이터 조작등은 하지않고 받은 그대로 다시 게임 서버로 되돌려 준다.



// ITEM_CREATE_EX 메시지가 한번에 생성할 수 있는 최대 아이템의 갯수
const int	ITEM_CREATE_EX_MAX_COUNT = 10;

// ITEM_CREATE_EX 메시지가 한번에 삭제할 수 있는 최대 아이템의 갯수
const int	ITEM_DELETE_EX_MAX_COUNT = 10;



//-----------------------------------------------------------------------
//	Desc : GQ_CREATE_ITEM_EX_REQ 에서 사용하는 공용체 정의
//

struct sITEM_TS_SUB_DATA
{
	BYTE		byTSType;				// eTSM_MANAGER_TYPE      수정해야 함->   TS_TYPE_QUEST_CS, TS_TYPE_PC_TRIGGER_CS, TS_TYPE_OBJECT_TRIGGER_S
	HOBJECT		hObject;				// Trigger Object 의 핸들 ObjectTrigger에서 사용한다
	sTSM_SERIAL	sSerial;
};


// 아이템 생성 타입
enum eITEM_CREATE_TYPE
{
	eITEM_CREATE_TYPE_TS_MIDDLE,				// TS Middle 에서 요청
};


// 아이템 생성시 프로토콜에 들어가는 서브 데이터를 가지는 공용체
union uITEM_CREATE_SUB_DATA
{
										// 값			   - 설명
	sITEM_TS_SUB_DATA	sTSSubData;		// Trigger System 에서 사용
};


//-----------------------------------------------------------------------
//	Desc : GQ_DELETE_ITEM_EX_REQ 에서 사용하는 공용체 정의
//

// 아이템 삭제 타입
enum eITEM_DELETE_TYPE
{
	eITEM_DELETE_TYPE_TS_MIDDLE,				// TS Middle 에서 요청
	eITEM_DELETE_TYPE_TMQ,						// TMQ에서 요청
};


// 아이템 삭제시 프로토콜에 들어가는 서브 데이터를 가지는 공용체
union uITEM_DELETE_SUB_DATA
{
											// 값				- 설명
	sITEM_TS_SUB_DATA	sTSSubData;		// Trigger System 에서 사용
};

struct sITEM_POSITION_DATA
{
	HOBJECT			hItem;		// 아이템 핸들
	BYTE			byPlace;	// 장소
	BYTE			byPos;		// 위치
};



//-----------------------------------------------------------------------------
// 아이템 자동 장착

struct sITEM_PLACE
{
	BYTE byPlace;
	BYTE byPos;
};

struct sITEM_AUTO_EQUIP_DATA
{
	// 이동 아이템
	ITEMID					srcItemId;
	HOBJECT					hSrcItem;
	BYTE					bySrcEquipPlace;
	BYTE					bySrcEquipPos;

	BYTE					byDestPlace;	// 이동해 갈 곳(비어있는 위치이어야 한다.)
	BYTE					byDestPos;

	// 생성 & 장착 하는 아이템
	sITEM_DATA				sEquipItem;
};


struct sITEM_AUTO_EQUIP_ROLLBACK_DATA
{
	// 삭제 아이템
	ITEMID					delItemId;
	HOBJECT					hDelEquipItem;
};


//-----------------------------------------------------------------------------


WORD Dbo_GetFinalOffence(WORD wBaseOffence, BYTE byGrade);
WORD Dbo_GetFinalDefence(WORD wBaseDefence, BYTE byGrade);

const BYTE			NTL_MAX_PRIVATESHOP_INVENTORY			= 36;	// 개인 상점에 최대 크기 (주의 : sizeof(BYTE)보다 크면 안된다) (개인상점 GUI의 슬롯 개수의 배수로 올라가던지 내려가야 한다)
const int			NTL_MAX_PRIVATESHOP_BUY_SHOPPING_CART	= 12;	// 쇼핑카트의 살수있는 최대 크기
const BYTE			PRIVATESHOP_ITEMPOS_INVALID				= 0xFF;
const DWORD			NTL_PRIVATESHOP_ITEM_BUY_ZENNY_DEFAULT	= 1;	//개인상점 팔때 기본 제니값

//-- SGpro --------------------------------------------------------------------------
//  Purpose : Game Srv와 Query Srv 사이에서 사용[7/2/2007 SGpro]
//  Return  : 
//-----------------------------------------------------------------------------------
struct sPRIVATESHOP_ITEM_POS_DATA
{
	bool	bIsSaveItem; //true이면 Shop에 아이템이 저장된 것 (Buy 때 Query Srv가 필요함)
	BYTE	byPlace;
	BYTE	byPos; //개인상점 인벤토리 Slot Pos
	BYTE	byInventoryPlace; //인벤토리의 Place (Buy 때 Query Srv가 필요함)
	BYTE	byInventoryPos;//인벤토리의 Pos (Buy 때 Query Srv가 필요함)
	ITEMID	itemID;
};

struct sINVEN_ITEM_POS_DATA
{
	BYTE byPlace;
	BYTE byPos;
	ITEMID	itemID;
};


//-- SGpro --------------------------------------------------------------------------
//  Purpose : Game Srv와 User PC 사이에서 사용[7/3/2007 SGpro]
//  Return  : 
//-----------------------------------------------------------------------------------
struct sPRIVATESHOP_ITEM_POS_DATA_UG
{
	BYTE	byPlace;
	BYTE	byPos;
};

//-- SGpro --------------------------------------------------------------------------
//  Purpose : [7/2/2007 SGpro]
//  Return  :
//-----------------------------------------------------------------------------------
enum ePRIVATESHOP_STATE
{
	 PRIVATESHOP_STATE_CLOSE			= 0 //오픈전
	,PRIVATESHOP_STATE_OPEN					//오픈후
	,PRIVATESHOP_STATE_BUSINESS_WAIT		//흥정 대기
	,PRIVATESHOP_STATE_BUSINESS				//흥정중
	,PRIVATESHOP_STATE_NONE					//개인상점 상태가 아니다
	,PRIVATESHOP_STATE_NULL					//개인상점이 한번도 생성되지 않은 상태
	,PRIVATESHOP_STATE_DATALOADING			//Query Srv에서 데이타가 오기를 기다리는 상태
};

// 생성자를 만들지 마세요. 
// 내부에서 사용됩니다. [10/2/2007 SGpro]
struct sSUMMARY_PRIVATESHOP_SHOP_DATA
{
	bool bIsOwnerEmpty; //true이면 주인이 자리를 비운것
	BYTE byShopState; //Shop State 저장(ePRIVATESHOP_STATE)
	WCHAR wcPrivateShopName[NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1];
};// end of struct sSUMMARY_PRIVATESHOP_SHOP_DATA


//-- server & client 내부 PrivateShop Data --------------------------------------
struct sPRIVATESHOP_SHOP_DATA
{
	HOBJECT hOwner;								// 상점 주인 CharacterID	
	PSHOP_WAITTIME waittimeCashShopStart;		// 유료 샵이 된 날짜
	PSHOP_WAITTIME waittimeCashShopEnd;			// 유료 샵 만료일
	sSUMMARY_PRIVATESHOP_SHOP_DATA sSummaryPrivateShopData;	
	WCHAR wcNotice[NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1];

public: 
	sPRIVATESHOP_SHOP_DATA()
		: hOwner ( INVALID_HOBJECT )
		, waittimeCashShopStart ( 0 )
		, waittimeCashShopEnd ( 0 )
	{
		sSummaryPrivateShopData.bIsOwnerEmpty = false;
		sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_NULL;
		ZeroMemory( sSummaryPrivateShopData.wcPrivateShopName,	sizeof(WCHAR) * ( NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1 ) );
		ZeroMemory( wcNotice,			sizeof(WCHAR) * ( NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1 ) );
	}
};//end of struct sPRIVATESHOP_SHOP_DATA


// -- 패킷용 PrivateShop Data임 ------------------------------------------------------
// wcNotice가 가변으로 변해야 하기 때문에 struct sPRIVATESHOP_SHOP_DATA와 분리를 했다
// Notice는 가변 Size이기 때문에 반드시 
// struct sPACKET_PRIVATESHOP_SHOP_DATA에 가장 밑에 있어야 된다 [10/2/2007 SGpro]
struct sPACKET_PRIVATESHOP_SHOP_DATA
{
	HOBJECT hOwner;// 상점 주인 CharacterID	
	PSHOP_WAITTIME waittimeCashShopStart; // 유료 샵이 된 날짜
	PSHOP_WAITTIME waittimeCashShopEnd; // 유료 샵 만료일
	sSUMMARY_PRIVATESHOP_SHOP_DATA sSummaryPrivateShopData;	
	BYTE byNoticeSize;
	WCHAR wcNotice[NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1];

public: 
	sPACKET_PRIVATESHOP_SHOP_DATA()
		: hOwner ( INVALID_HOBJECT )
		, waittimeCashShopStart ( 0 )
		, waittimeCashShopEnd ( 0 )
		, byNoticeSize ( 0 )
	{
		sSummaryPrivateShopData.bIsOwnerEmpty = false;
		sSummaryPrivateShopData.byShopState = PRIVATESHOP_STATE_NONE;
		ZeroMemory( sSummaryPrivateShopData.wcPrivateShopName,	sizeof(WCHAR) * ( NTL_MAX_PRIVATESHOP_NAME_IN_UNICODE + 1 ) );
		ZeroMemory( wcNotice,			sizeof(WCHAR) * ( NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1 ) );
	}
};// end of struct sPRIVATESHOP_SHOP_DATA

// -- SGpro --------------------------------------------------------------------------
//  Purpose : [7/2/2007 SGpro]
//  Return  :
// -----------------------------------------------------------------------------------
struct sPRIVATESHOP_ITEM_DATA
{
	enum eITEMSTATE
	{
		PRIVATESHOP_GENERAL					= 0, // 일반
		PRIVATESHOP_BUSINESS_ITEM			= 1, // 흥정중인 아이템(찜한 아이템은 흥정할 수 없다)
		PRIVATESHOP_VISITOR_SELECT_ITEM		= 2, // 아이템 찜
		PRIVATESHOP_VISITOR_UNSELECT_ITEM	= 3, // 아이템 찜 풀기
		PRIVATESHOP_INVALID					= 0xFF,
	};

	HOBJECT							hItem;
	BYTE							byPrivateShopInventorySlotPos;
	bool							bIsSaveItem; // true이면 Shop에 아이템이 저장된 것
	sITEM_DATA						sItem;// bIsSaveItem의 값이 true일 경우 채워서 UserPC에 보낸다
	DWORD							dwZenny;
	BYTE							byItemState; // eITEMSTATE 값을 갖는다

	sPRIVATESHOP_ITEM_DATA()
		: hItem ( INVALID_HOBJECT )
		, byPrivateShopInventorySlotPos ( PRIVATESHOP_ITEMPOS_INVALID )
		, dwZenny ( NTL_PRIVATESHOP_ITEM_BUY_ZENNY_DEFAULT )
		, byItemState ( PRIVATESHOP_INVALID )
		, bIsSaveItem ( false )
	{
		ZeroMemory( &sItem, sizeof( sItem ));
	}
};

// ShopSystem 사용 하는 아이템 팔기 Sell_Price, dwCost, dwOptionCost -> S C O

#define NTL_SELL_PRICE(S, C, O)	  ( ( ( ( (float)(S) / 100.0 ) * (float)C) + O) )

// ItemUpgrade
//-----------------------------------------------------------------------------------
// 아이템 UPGRADE Scouter Add Rate 정의
//-----------------------------------------------------------------------------------
enum eITEM_UPGRADE_RATE
{
	ITEM_UPGRADE_RATE_0 = 0,//-10.0f,	
	ITEM_UPGRADE_RATE_1 ,//-5.0f,			
	ITEM_UPGRADE_RATE_2 ,// 0.0f,			
	ITEM_UPGRADE_RATE_3 ,// 5.0f,			
	ITEM_UPGRADE_RATE_4 ,// 10.0f,			
	ITEM_UPGRADE_RATE_5 ,// 20.0f,	

};

float Dbo_UpgradeSuccessRate( BYTE byGrade, BYTE byStoneType );
float Dbo_ItemDestroyRateByUpgradeFail( const BYTE byRank, const BYTE byItemBattleAttribute, const BYTE byPosNum, const float fAddRateByScouter );// 업그레이드 실패시 아이템이 파괴될 확률
// 중앙 아이템의 Rank 속성 등급과 바깥의 테두리에 선택한속성1~5와 스카우터 착용에 따른 추가 확률을 넣어 최종확률을 얻어낸다
BYTE Dbo_GetUpgradeRate( BYTE byItemBattleAttribute, BYTE byPosNum );
//	각 파츠의 높아지고 낮아지는 확률
float Dbo_GetAttributeRate( BYTE byItemBattleAttribute, BYTE byPosNum );
// 속성에 따른 확률을 구하기.
BYTE Dbo_GetHoipoiStoneCount( const BYTE byStoneType, const BYTE byItemType, BYTE byGrade );
// 아이템 타입과 등급에 따른 소요 호이포이 갯수 가져오기
DWORD Dbo_GetRepairPay( DWORD dwCost, BYTE byDur, BYTE byCurDur);
// 아이템 믹스 EXP
DWORD Dbo_GetHoipoiMixEXP( bool bIsNormalSuccess, bool bIsGreatSuccess, BYTE byMadeLevel, BYTE byNeedMixLevel );
// 아이템 믹스 다음 레벨 필요 경험치
DWORD Dbo_GetHoipoiMixNeedLevelExp( BYTE byCurLevel );
// 아이템 믹스 사용료
DWORD Dbo_GetHoipoiMixFare( DWORD dwCost, BYTE byDiscountRate = 0);
// 제조 기술을 배우기 위한 습득 레벨
BYTE Dbo_GetHoipoiMixNeedLevelForJobSet( const BYTE byRecipeType );
// 제조 기술을 배우기 위해 필요한 제니
DWORD Dbo_GetHoipoiMixNeedZennyForJobSet( const BYTE byRecipeType );

// Item Copy
bool Dbo_SetItemData( sITEM_DATA* const pItemData, ITEMID itemId, TBLIDX itemNo, CHARACTERID charId, BYTE byPlace, BYTE byPosition, BYTE byStackcount, BYTE byRank, BYTE byCurrentDurability, bool bNeedToIdentify, BYTE byGrade, BYTE byBattleAttribute, BYTE byRestrictType, const WCHAR* const awchMaker, TBLIDX* const aOptionTblidx, BYTE byDurationType, DBOTIME nUseStartTime, DBOTIME nUseEndTime );
bool Dbo_SetItemData_CheckNeedToIdentify( sITEM_DATA* const pItemData, ITEMID itemId, TBLIDX itemNo, CHARACTERID charId, BYTE byPlace, BYTE byPosition, BYTE byStackcount, BYTE byRank, BYTE byCurrentDurability, bool bNeedToIdentify, BYTE byGrade, BYTE byBattleAttribute, BYTE byRestrictType, WCHAR* const awchMaker, TBLIDX* const aOptionTblidx, BYTE byDurationType, DBOTIME nUseStartTime, DBOTIME nUseEndTime );
// Item Profile
bool Dbo_SetItemProfile( sITEM_PROFILE* const pItemProfile, HOBJECT handle, TBLIDX tblidx, BYTE byPlace, BYTE byPos, BYTE byStackcount, BYTE byRank, BYTE byCurDur, bool bNeedToIdentify, BYTE byGrade, BYTE byBattleAttribute, BYTE byRestrictType, WCHAR* const awchMaker, TBLIDX* const aOptionTblidx, BYTE byDurationType, DBOTIME nUseStartTime, DBOTIME nUseEndTime );
// Bank Item Profile
//bool Dbo_SetBankItemProfile( sBANK_PROFILE* const pItemProfile, HOBJECT handle, TBLIDX tblidx, BYTE byPos, BYTE byStackcount, BYTE byRank, BYTE byCurDur, bool bNeedToIdentify, BYTE byGrade, BYTE byBattleAttribute, BYTE byRestrictType, WCHAR* const awchMaker, TBLIDX* const aOptionTblidx, BYTE byDurationType, DBOTIME nUseStartTime, DBOTIME nUseEndTime );
// Item Brief
bool Dbo_SetItemBrief( sITEM_BRIEF* const pItemBrief, TBLIDX tblidx, BYTE byRank, BYTE byGrade, BYTE byBattleAttribute, TBLIDX* const aOptionTblidx );
// Item Shop
bool Dbo_SetShopBuyInven( sSHOP_BUY_INVEN* const pShopBuyInven, BYTE byPlace, BYTE byPos, BYTE byStack, TBLIDX tblItem, BYTE byRank, BYTE byCurrentDurability, BYTE byGrade, BYTE byRestrictType, WCHAR* const awchMaker, TBLIDX* const aOptionTblidx, BYTE byDurationType, DBOTIME nUseStartTime, DBOTIME nUseEndTime );
// Item -> Data
bool Dbo_SetItemDataToQryData( const sITEM_DATA& rItemDataOrg, int& itemNo, int& charId, BYTE& byPlace, BYTE& byPosition, BYTE& byStackcount, BYTE& byRank, BYTE& byCurrentDurability, BYTE& byNeedToIdentify, BYTE& byGrade, BYTE& byBattleAttribute, BYTE& byRestrictType, char* pMakerInHex, size_t nMakerInHex_Size, int* aOptionTblidx, size_t nOptionTblidx_Size, BYTE& byDurationType, __int64& nUseStartTime, __int64& nUseEndTime );
bool Dbo_SetItemDataToData( const sITEM_DATA& rItemDataOrg, ITEMID& itemId, TBLIDX& itemNo, CHARACTERID& charId, BYTE& byPlace, BYTE& byPosition, BYTE& byStackcount, BYTE& byRank, BYTE& byCurrentDurability, bool& bNeedToIdentify, BYTE& byGrade, BYTE& byBattleAttribute, BYTE& byRestrictType, WCHAR* pMakerInHex, size_t nMakerInHex_Size, TBLIDX* pOptionTblidx, size_t nOptionTblidx_Size, eDURATIONTYPE& eDurationType, DBOTIME& nUseStartTime, DBOTIME& nUseEndTime );
bool Dbo_SetAutoEquipItemDataToQryData( const sITEM_AUTO_EQUIP_DATA& rItemAutoEquipData, int& srcItemId, HOBJECT& hSrcItem, BYTE& bySrcEquipPlace, BYTE& bySrcEquipPos, BYTE& byDestPlace, BYTE& byDestPos, int& itemNo, int& charId, BYTE& byPlace, BYTE& byPosition, BYTE& byStackcount, BYTE& byRank, BYTE& byCurrentDurability, BYTE& byNeedToIdentify, BYTE& byGrade, BYTE& byBattleAttribute, BYTE& byRestrictType, char* pMakerInHex, size_t nMakerInHex_Size, int* aOptionTblidx, size_t nOptionTblidx_Size, BYTE& byDurationType, __int64& nUseStartTime, __int64& nUseEndTime );
bool Dbo_SetShopBuyInvenToQryData( const sSHOP_BUY_INVEN& rShopBuyInven, BYTE& byPlace, BYTE& byPos, BYTE& byStack, int& tblItem, BYTE& byRank, BYTE& byCurrentDurability, BYTE& byGrade, BYTE& byRestrictType, char* pMakerInHex, size_t nMakerInHex_Size, int* aOptionTblidx, size_t nOptionTblidx_Size, BYTE& byDurationType, __int64& nUseStartTime, __int64& nUseEndTime );

#pragma pack()
