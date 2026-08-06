#ifndef __NTL_STORAGE_DEFINE_H__
#define __NTL_STORAGE_DEFINE_H__

// 占쏙옙占쏙옙 占싹댐옙 Flags
enum eNTL_STORAGE_APPLY
{
	eNTL_STORAGE_APPLY_PRESENTATION				= 0x00000001,
	eNTL_STORAGE_APPLY_SIMULATION				= 0x00000010,
	eNTL_STORAGE_APPLY_CLIENT					= 0x00000100,
	eNTL_STORAGE_APPLY_PRESENTATION_INITONCE	= 0x00001000,
	eNTL_STORAGE_APPLY_ALL						= 0x11110111
};

/**
* \brief NTL_STORAGE UNIT占쏙옙占쏙옙 GROUP TYPE( 占싹놂옙占쏙옙 File 占쏙옙 1:1 占쏙옙칭占싫댐옙. )
*/
enum eNTL_STORAGE_GROUP_TYPE
{
	eNTL_STORAGE_GROUP_ACCOUNT,		///< 占쏙옙占쏙옙 占쏙옙占쏙옙
	eNTL_STORAGE_GROUP_SYSTEM,		///< 占시쏙옙占쏙옙
	eNTL_STORAGE_GROUP_GAMEINFO,	///< 占쏙옙占쏙옙 占쏙옙占쏙옙
	eNTL_STORAGE_GROUP_SCOUTER,		///< 캐占쏙옙占쏙옙 占쏙옙카占쏙옙占쏙옙 占쏙옙占쏙옙
	eNTL_STORAGE_GROUP_QUEST,		///< 占쏙옙占쏙옙트 占싸듸옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
	eNTL_STORAGE_GROUP_CHARACTER,	///< 캐占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
	eNTL_STORAGE_GROUP_EXTRA,		///< New Extra group

	eNTL_STORAGE_GROUP_NUMS,
	eNTL_STORAGE_GROUP_INVALID = 0xFF
};

/**
* \brief NtlStorageUnit 占쏙옙占쏙옙 Type
*/
enum eNTL_STORAGE_TYPE
{
	eNTL_STORAGE_ACCOUNT,			///< 占쏙옙占쏙옙
	eNTL_STORAGE_GRAPHIC,			///< 占쌓뤄옙占쏙옙
	eNTL_STORAGE_SOUND,				///< 占쌀몌옙
	eNTL_STORAGE_GAMEINFO,			///< 占쏙옙占쏙옙 占쏙옙占쏙옙
	eNTL_STORAGE_ETC,				///< 占쏙옙타
	eNTL_STORAGE_CHAT,				///< 채占쏙옙
	eNTL_STORAGE_SCOUTER,			///< 占쏙옙占쏙옙 占쏙옙카占쏙옙占쏙옙 占쏙옙 캐占쏙옙占쏙옙 占쏙옙카占쏙옙占쏙옙 占쏙옙占쏙옙(占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙)
	eNTL_STORAGE_QUEST,				///< 占쏙옙占쏙옙트
	eNTL_STORAGE_CHARACTER,			///< 캐占쏙옙占싶븝옙 占쏙옙占쏙옙
	eNTL_STORAGE_CHARACTER_ETC,
	eNTL_STORAGE_CHARACTER_SCOUTER,
	eNTL_STORAGE_SYSTEM_ETC,
	eNTL_STORAGE_EXTRAFILTER,		///< Added Filter for the new Extra tab

	eNTL_STORAGE_NUMS,
	eNTL_STORAGE_INVALID = 0xFF
};

// eNTL_STORAGE_TYPE占쏙옙 占쏙옙치占싹울옙占쏙옙 占쏙옙
extern const char* g_acNtlStorageTypeString[eNTL_STORAGE_NUMS];

/**
* \brief NtlStorageValue占쏙옙 占쏙옙占쏙옙占쏙옙 Data Type占쏙옙
*/
enum eNTL_STORAGE_VARIABLE_TYPE
{
	eINT,			// INT 占쏙옙
	eFLOAT,			// FLOAT 占쏙옙
	eBOOL,			// bool 占쏙옙
	eSTRING,		// 占쏙옙占쌘울옙 占쏙옙
	eUSER,			// 사용자 정의 형

	eNTL_STORAGE_VARIABLE_NUMS,
	eNTL_STORAGE_VARIABLE_INVALID = 0xFF
};

// Check 함수 포인터 ( 키와 연결된 값의 범위체크에 사용된다. )
typedef bool (*NtlStorageUnitCheckFunc)(unsigned int uiKey);

// Apply 함수 포인터 ( 유닛과 연결되어 유닛 단위로 Apply 한다. )
class CNtlStorageUnit;
typedef bool (*NtlStorageUnitApplyFunc)(CNtlStorageUnit* pUnit, unsigned int uiFlags);

// NTL_STORAGE 키 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙
#define dNTL_STORAGE_MAX_KEY_NAME_LENGTH	(32+1)

// NTL_STORAGE VALUE 占쏙옙 占쏙옙占쏙옙
#define dNTL_STORAGE_MAX_VALUE_LENGTH		(128+1)

// 占쏙옙占쏙옙 占쏙옙占싱븝옙
struct SNtlStorageMappingTableData
{
	unsigned int		uiKey;
	unsigned char		byVarType;
	unsigned char		byCategoryType;
	char				acKeyName[dNTL_STORAGE_MAX_KEY_NAME_LENGTH];
	
	// 占쏙옙占싹븝옙占싶몌옙 占쏙옙占쏙옙 占쏙옙占쏙옙
	NtlStorageUnitCheckFunc		ntlStorageFunc;		///< Check 占쌉쇽옙 占쏙옙占쏙옙占쏙옙
	char						acDefaultValue[dNTL_STORAGE_MAX_VALUE_LENGTH];
};

// NtlStorage Bool Define
#define dSTORAGE_TRUE		"true"
#define dSTORAGE_FALSE		"false"

// 占쏙옙카占쏙옙占쏙옙 占쏙옙占쏙옙 占쏙옙占쏙옙
typedef struct _SMemoryScouterData
{
	_SMemoryScouterData::_SMemoryScouterData(VOID)
	{
		uiPower = 0;
	}

	unsigned int uiPower;				// 占쏙옙占쏙옙占쏙옙

}SMemoryScouterData;

// 占쏙옙占쏙옙트 占쏙옙占쏙옙
typedef struct _SMemoryQuestData
{
	_SMemoryQuestData::_SMemoryQuestData(VOID)
	{
		bIsNew = FALSE;
		bIsIndicated = FALSE;
	}

	RwBool	bIsNew;
	RwBool	bIsIndicated;
}SMemoryQuestData;

// Key ID Defines
// Rule: The KEY ID is incremented in sequence or greater than the last Defined ID.
// Do not add inserts in the middle of an absolute deletion. DEFINE, which is not used, is just a comment.
//----------------------------------------------------------------------------------------

#define dSTORAGE_CATEGORY						0

// Account
#define dSTORAGE_ACCOUNT_LASTCHAR				1000		// 占쏙옙占쏙옙占쏙옙占쏙옙占쏙옙 占시뤄옙占쏙옙 占쏙옙 캐占쏙옙占쏙옙 占싱몌옙

// Graphic
#define dSTORAGE_GRAPHIC_SELECT_VIDEOWIDTH		2000		// 占쌔삼옙 WIDTH
#define dSTORAGE_GRAPHIC_SELECT_VIDEOHEIGHT		2001
#define dSTORAGE_GRAPHIC_SELECT_VIDEODEPTH		2002		// 占쏙옙占쏙옙
#define dSTORAGE_GRAPHIC_GAMMA					2003		// 밝기
#define dSTORAGE_GRAPHIC_WINDOW_MODE			2004		// 창모드
#define dSTORAGE_GRAPHIC_PRESET					2005		// 프리셋 레벨 0~3, 4 = 사용자 설정
#define dSTORAGE_GRAPHIC_TERRAIN_RANGE			2006		// 占쏙옙占쏙옙 占시야거몌옙
#define dSTORAGE_GRAPHIC_OBJECT_RANGE			2007		// �繰 �þ߰Ÿ�
#define dSTORAGE_GRAPHIC_TERRAIN_SHADOW			2010		// 占쏙옙占쏙옙 占쌓몌옙占쏙옙
#define dSTORAGE_GRAPHIC_WATER_EFFECT			2011		// 占쏙옙효占쏙옙
#define dSTORAGE_GRAPHIC_CHARACTER_RANGE		2012		// 캐占쏙옙占쏙옙 占시야거몌옙
#define dSTORAGE_GRAPHIC_CHARACTER_EFFECT		2013		// 캐占쏙옙占쏙옙 占쏙옙占쌓뤄옙占싱듸옙 효占쏙옙 
#define dSTORAGE_GRAPHIC_CHARACTER_SHADOW		2014		// 캐占쏙옙占쏙옙 占쌓몌옙占쏙옙
#define dSTORAGE_GRAPHIC_MAGIC_EFFECT			2015		// 占쏙옙占쏙옙 占쏙옙占쏙옙효占쏙옙
#define dSTORAGE_GRAPHIC_WEATHER_EFFECT			2016		// 占쏙옙占쏙옙 효占쏙옙
#define dSTORAGE_GRAPHIC_TEXTURE_LEVEL			2017		// 占쌔쏙옙占쏙옙 占쌔삼옙
#define dSTORAGE_GRAPHIC_SHADER_HDR				2018		// HDR 효占쏙옙
#define dSTORAGE_GRAPHIC_CHARACTER_EDGE			2019		// 카占쏙옙 占쌤곤옙占쏙옙
#define dSTORAGE_GRAPHIC_SHOW_FPS				2020
#define dSTORAGE_GRAPHIC_FPS					2021
#define dSTORAGE_GRAPHIC_SSAA					2022

// Sound
#define dSTORAGE_SOUND_MAIN_VOLUME				4000	// 占쏙옙占쏙옙 占쏙옙占쏙옙
#define dSTORAGE_SOUND_BACK_VOLUME				4001	// 배경음 볼륨
#define dSTORAGE_SOUND_EFFECT_VOLUME			4002	// 효占쏙옙占쏙옙 占쏙옙占쏙옙
#define dSTORAGE_SOUND_ENV_VOLUME				4003	// 환占쏙옙占쏙옙 占쏙옙占쏙옙

// GameInfo
#define dSTORAGE_GAMEINFO_GUILD_EMBLEM			5000	// 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙
#define dSTORAGE_GAMEINFO_DOJO_MARK				5001	// 占쏙옙占쏙옙 占쏙옙크
#define dSTORAGE_GAMEINFO_SCOUTER_VISIBLE		5002	// 占쏙옙카占쏙옙占쏙옙 표占쏙옙
#define dSTORAGE_GAMEINFO_ITEM_DROP				5003	// show item drop
// Etc
#define dSTORAGE_ETC_FRIEND_NOTIFY				6000	// 친占쏙옙 占쏙옙占쏙옙 占싯몌옙
#define dSTORAGE_ETC_GUILD_MEMBER_NOTIFY		6001	// 占쏙옙占싹삼옙 占쏙옙占쏙옙 占싯몌옙
#define dSTORAGE_ETC_QUESTTEXT_EFFECT			6002	// 占쏙옙占쏙옙트 占쌔쏙옙트 효占쏙옙
#define dSTORAGE_ETC_USER_TRADE					6003	// 占신뤄옙
#define dSTORAGE_ETC_MOVEABLEWINDOW				6004	// 占쏙옙占쏙옙占쏙옙 占싱듸옙 占쏙옙占쏙옙 占쏙옙占쏙옙
#define dSTORAGE_ETC_MOUSE_DASH					6005	// ���콺 ��� ����
#define dSTORAGE_ETC_MOUSE_FOV					6006
#define dSTORAGE_ETC_MOUSE_VIEW_POINT			6007

// Chatting
#define dSTORAGE_CHAT_INPUTMODE					7000	// 채팅창 입력모드
#define dSTORAGE_CHAT_WINDOW_LOCK				7001	// 채팅창 잠금
#define dSTORAGE_CHAT_MOUSE_SCROLL				7002	// ���콺 ��ũ��
#define dSTORAGE_CHAT_EXTEND_WINDOW				7003	// 확占쏙옙 채占쏙옙창
#define dSTORAGE_CHAT_WINDOW_OPACITY			7004	// 占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙
#define dSTORAGE_CHAT_BASIC_NORMAL				7005	// 占썩본 占쏙옙占쏙옙占쏙옙 : 占싹반댐옙화
#define dSTORAGE_CHAT_BASIC_WISPHER				7006	// 占썩본 占쏙옙占쏙옙占쏙옙 : 占쌈속몌옙
#define dSTORAGE_CHAT_BASIC_PARTY				7007	// 占썩본 占쏙옙占쏙옙占쏙옙 : 占쏙옙티
#define dSTORAGE_CHAT_BASIC_GUILD				7008	// 占썩본 占쏙옙占쏙옙占쏙옙 : 占쏙옙占쏙옙
#define dSTORAGE_CHAT_BASIC_TRADE				7009	// 占썩본 占쏙옙占쏙옙占쏙옙 : 占신뤄옙
#define dSTORAGE_CHAT_BASIC_SHOUT				7010	// 占썩본 占쏙옙占쏙옙占쏙옙 : 占쏙옙치占쏙옙
#define dSTORAGE_CHAT_BASIC_SYSTEM				7011	// 占썩본 占쏙옙占쏙옙占쏙옙 : 占시쏙옙占쏙옙
#define dSTORAGE_CHAT_EXTEND_NORMAL				7012	// 확占쏙옙 占쏙옙占쏙옙占쏙옙 : 占싹반댐옙화
#define dSTORAGE_CHAT_EXTEND_WISPHER			7013	// 확占쏙옙 占쏙옙占쏙옙占쏙옙 : 占쌈속몌옙
#define dSTORAGE_CHAT_EXTEND_PARTY				7014	// 확占쏙옙 占쏙옙占쏙옙占쏙옙 : 占쏙옙티
#define dSTORAGE_CHAT_EXTEND_GUILD				7015	// 확占쏙옙 占쏙옙占쏙옙占쏙옙 : 占쏙옙占쏙옙
#define dSTORAGE_CHAT_EXTEND_TRADE				7016	// 확占쏙옙 占쏙옙占쏙옙占쏙옙 : 占신뤄옙
#define dSTORAGE_CHAT_EXTEND_SHOUT				7017	// 확占쏙옙 占쏙옙占쏙옙占쏙옙 : 占쏙옙치占쏙옙
#define dSTORAGE_CHAT_EXTEND_SYSTEM				7018	// 확占쏙옙 占쏙옙占쏙옙占쏙옙 : 占시쏙옙占쏙옙
#define dSTORAGE_CHAT_BASIC_FIND_PARTY			7019
#define dSTORAGE_CHAT_EXTEND_FIND_PARTY			7020
#define dSTORAGE_CHAT_EXTEND2_NORMAL			7021
#define dSTORAGE_CHAT_EXTEND2_WISPHER			7022
#define dSTORAGE_CHAT_EXTEND2_PARTY				7023
#define dSTORAGE_CHAT_EXTEND2_GUILD				7024
#define dSTORAGE_CHAT_EXTEND2_TRADE				7025
#define dSTORAGE_CHAT_EXTEND2_SHOUT				7026
#define dSTORAGE_CHAT_EXTEND2_SYSTEM			7027
#define dSTORAGE_CHAT_EXTEND2_FIND_PARTY		7028

#define dSTORAGE_CHAT_EXTEND2_WINDOW			7029

// Filter (Extra Tab)
#define dSTORAGE_FILTER_RARITY_NORMAL			7100
#define dSTORAGE_FILTER_RARITY_SUPERIOR			7101
#define dSTORAGE_FILTER_RARITY_EXCELLENT		7102
#define dSTORAGE_FILTER_RARITY_RARE				7103
#define dSTORAGE_FILTER_RARITY_LEGENDARY		7104
#define dSTORAGE_FILTER_STONE_BLUE				7105
#define dSTORAGE_FILTER_STONE_RED				7106
#define dSTORAGE_FILTER_STONE_GREEN				7107
#define dSTORAGE_FILTER_STONE_PURPLE			7108
#define dSTORAGE_FILTER_STONE_WHITE				7109
#define dSTORAGE_FILTER_STONE_ADOWN				7110
#define dSTORAGE_FILTER_STONE_WDOWN				7111
#define dSTORAGE_FILTER_MYSTERYITEM				7112
#define dSTORAGE_FILTER_CONSUMABLE				7113

// Scouter
#define dSTORAGE_SCOUTER_MONSTER_SCOUTER_MEMORY_INFO	6000
#define dSTORAGE_SCOUTER_PLAYER_SCOUTER_MEMORY_INFO		6001
#define dSTORAGE_SCOUTER_INFO_TBLID						6002
#define dSTORAGE_SCOUTER_INFO_POWER						6003

// Quest Indicator
#define	dSTORAGE_QUEST_MEMORY_INFO						7000
#define dSTORAGE_QUEST_INFO_ID							7001
#define dSTORAGE_QUEST_INFO_NEW							7002
#define dSTORAGE_QUEST_INFO_INDICATE					7003

// Character Info
#define dSTORAGE_ETC_EX_QUICKSLOT1						8000	// 占쌩곤옙 占쏙옙占쏙옙占쏙옙 1
#define dSTORAGE_ETC_EX_QUICKSLOT2						8001	// 占쌩곤옙 占쏙옙占쏙옙占쏙옙 2
#define dSTORAGE_ETC_QUICKSLOT_LOCK						8002	// 퀵슬롯 잠그기

#define dSTORAGE_CHARACTER_ETC_DISCORD_HIDE_NAME		9000	// show character name in discord 

#define dSTORAGE_CHARACTER_SCOUTER_ON					9010	// scouter status

#define dSTORAGE_SYSTEM_ETC_DISCORD						10000	// Enable/disable discorrd
#define dSTORAGE_SYSTEM_ETC_CONTRACT					10001	// contract/game policy

#define dSTORAGE_EOF									0xFFFFFFFF

#endif//__NTL_STORAGE_DEFINE_H__
