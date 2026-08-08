/****************************************************************************
 *
 * File			: DboDef.h
 * Author		: HyungSuk, Jang
 * Copyright	: (占쏙옙)NTL
 * Date			: 2005. 9. 28	
 * Abstract		: DBO standard definition.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DBO_DEF_H___
#define __DBO_DEF_H___

#include "DboCompileOpt.h"
#include "NtlSLDef.h"
#include "DboTSCoreDefine.h"

#include "NtlSob.h"

//----------------------------------------------------------------------------
#define DEFAULT_SCREEN_WIDTH	1024	
#define DEFAULT_SCREEN_HEIGHT	768

#define EXTRA_SCREEN_WIDTH		1280		// cj占쏙옙 占쏙옙청占쏙옙 占쏙옙占쌔쇽옙.
#define EXTRA_SCREEN_HEIGHT		1024		// cj占쏙옙 占쏙옙청占쏙옙 占쏙옙占쌔쇽옙.

#define DEFAULT_CAMERA_NEAR		0.5f
#define DEFAULT_CAMERA_FAR		768.0f

//----------------------------------------------------------------------------
// serialize file

#define USEROPT_SERIALIZE_FOLDERNAME		".\\user"	

#define ACCOUNTOPT_SERIALIZE_FILENAME		"Account.opt"
#define SYSTEMENV_SERIALIZE_FILENAME		"SystemEnv.txt"
#define GAMEENV_SERIALIZE_FILENAME			"GameEnv.txt"
#define EXTRAENV_SERIALIZE_FILENAME			"ExtraEnv.txt"

#define CHARACTERENV_SERIALIZE_FILENAME		"CharacterEnv.txt";
#define SCOUTEROPT_SERILIZE_FILENAME		"Scouter.opt";
#define QUESTOPT_SERIALIZE_FILENAME			"QuestIndicator.opt";


//----------------------------------------------------------------------------
// client control data

#define PICK_TERRAIN_LIMIT		300.0f
#define PICK_OBJECT_LIMIT		100.0f

#define MOUSE_DASH_LIMIT		0.5f

#define	PLAYER_MAX_LEVEL		70

//-----------------------------------------------------------------------------
// stage name
#define LOGIN_STAGE_NAME			"LogIn"
#define CHARACTER_STAGE_NAME		"Character"
#define GAME_STAGE_NAME				"Game"
#define GAME_LOADING_STAGE_NAME		"GameLoading"

//-----------------------------------------------------------------------------
// client 占쏙옙체 buffer length

#define MAX_VIRTUAL_SERVER_PACKET_SIZE		2048
#define MAX_MSG_BOX_STRING_LEN				512
#define CHAT_MSG_BUFFER_LEN					512		/** Ŭ���̾�Ʈ ��ü ��� */

//-----------------------------------------------------------------------------
// Message sender name
#define SENDERNAME_SYSTEM	L"System"
#define SENDERNAME_NOTICE	L"Notice"


//-----------------------------------------------------------------------------
// Error Report
#define ERROR_REPORT_FILE_NAME      "DboErrorInfo.xml"

//-----------------------------------------------------------------------------
// Flash Notify Invalid index && file name
#define dINVALID_FLASH_NOTIFY_FILE_INDEX		0xff
#define dINVALID_FLASH_NOTIFY_FILE_TMQ			"fn000000.swf"
#define dINVALID_FLASH_NOTIFY_FILE_PLEASE_WAIT	"TMQ_PleaseWait.swf"
#define dINVALID_FLASH_NOTIFY_FILE_CLASS_CHANGE "ClassChange_Success.swf"
#define dINVALID_FLASH_NOTIFY_FILE_LEVEL_UP     "Level_Up.swf"
#define dINVALID_FLASH_NOTIFY_FILE_HOIPOIMIX_LEVEL_UP	"Mix_Level_Up.swf"

//-----------------------------------------------------------------------------
// Flash Notify Invalid index && file name
#define dDELAY_BEFORE_VISIBLE_MESSAGEBOX		7.f		// 占쌨쇽옙占쏙옙 占쌘쏙옙占쏙옙 占쌩깍옙占쏙옙占쏙옙占쏙옙 占쌍댐옙 占쏙옙占쏙옙占시곤옙
#define dREMAIN_FOR_SERVER_REPLY				60.f	// ���� ������ ��ٸ��� �ִ� �ð�

//-----------------------------------------------------------------------------
// guild contribution zenny
#define dGUILD_CONTRIBUTION_ZENNY_50000			50000
#define dGUILD_CONTRIBUTION_ZENNY_100000		100000
#define dGUILD_CONTRIBUTION_ZENNY_500000		500000
#define dGUILD_CONTRIBUTION_ZENNY_1000000		1000000
#define dGUILD_CONTRIBUTION_ZENNY_10000000		10000000
#define dGUILD_CONTRIBUTION_ZENNY_100000000		100000000

//-----------------------------------------------------------------------------
// Moive
#define dLOGO_MOVIE								"movie/logo.avi"
#define dOPENING_MOVIE							"movie/opening.avi"

//-----------------------------------------------------------------------------
// gui Message box

#define MAX_GUI_MESSAGE_BOX_BUTTONS			4


enum EMsgBoxResult
{
	MBR_OPENCANCELED	= -1,				/// �޽����ڽ� ������ü�� ���. �޽��� �ڽ��� �ߺ��� ���.
	MBR_CANCEL			= 0,				/// ���.
	MBR_OK				= 1,				/// 확占쏙옙, 占쏙옙占쏙옙,	
};

struct sMsgBoxCustomBtn
{
	sMsgBoxCustomBtn(VOID)
	{
		Init();
	}

	VOID sMsgBoxCustomBtn::Init(VOID)
	{
		bHasButton = FALSE;
		strMessage = "";
		strToolTip = "";
	}

	RwReal bHasButton;
	std::string strMessage;
	std::string strToolTip;
};

//-----------------------------------------------------------------------------

///< avooo : ǥ�õǴ� �켱������ ���� �����Ͽ���.
///< peessi : Display String Define占쏙옙 占쏙옙占실듸옙 占쏙옙占쌘울옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙치占쌔억옙占쏙옙.
enum EIconPopupWork
{
	PMW_USE = 0,						    ///< ����ϱ�
	PMW_OPEN,								///< (占쏙옙占쏙옙)占쏙옙占쏙옙
	PMW_EQUIP,								///< 占쏙옙占쏙옙占싹깍옙
	PMW_CANCEL_EQUIP,						///< 占쏙옙占쏙옙占쏙옙占쏙옙
	PMW_VIEW,								///< 占쏙옙占쏙옙
	PMW_PICKUP,								///< 占쏙옙占쏙옙
	PMW_STOP_USE,							///< �������
	PMW_PULLOUT,							///< 占쏙옙占쏙옙占쏙옙
	PMW_SELL,								///< 占싫깍옙
	PMW_DIVIDE,								///< 占쏙옙占쏙옙占쏙옙
	PMW_CLEAR,								///< 占쏙옙占쏙옙
	PMW_DELETE,								///< 占쏙옙占쏙옙占쏙옙
	PMW_PARTY_CREATE,						///< ��Ƽ �����
	PMW_PARTY_INVITE,						///< 占쏙옙티 占십댐옙
	PMW_PARTY_LEADER_CHANGE,				///< 占쏙옙티占쏙옙 占쏙옙占쏙옙
	PMW_PARTY_KICK_OUT,						///< 占쏙옙티 占쏙옙占쏙옙
	PMW_PARTY_LEAVE,						///< 占쏙옙티 탈占쏙옙
	PMW_PVP_REQUEST_FIGHT,					///< ��� ��û
	PMW_USER_TRADE,							///< 占쏙옙占쏙옙 트占쏙옙占싱듸옙
	PMW_GUILD_INVITE,						///< ��� �ʴ�
	PMW_GUILD_CHANGE_MASTER,				///< ����� �̾�
	PMW_GUILD_APPOINT_SECOND_MASTER,		///< �α���� �Ӹ�
	PMW_GUILD_DISAPPOINT_SECOND_MASTER,		///< �α���� �Ӹ� ����
	PMW_GUILD_KICK_OUT,						///< ��� �߹�
	PMW_GUILD_POST,							///< 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙
	PMW_FRIEND_ADD,							///< 친占쏙옙 占쌩곤옙
	PMW_FOLLOW,								///< 占쏙옙占쏙옙 占쏙옙占쏙옙
	PMW_SEND_WHISPER,
	PMW_CANCEL,								///< ���
	PMW_SET_SHARETARGET_1,					// 1占쏙옙 占쏙옙占쏙옙타占쏙옙 占쏙옙占쏙옙
	PMW_SET_SHARETARGET_2,					// 2占쏙옙 占쏙옙占쏙옙타占쏙옙 占쏙옙占쏙옙
	PMW_SET_SHARETARGET_3,					// 3占쏙옙 占쏙옙占쏙옙타占쏙옙 占쏙옙占쏙옙
	PMW_SET_SHARETARGET_4,					// 4占쏙옙 占쏙옙占쏙옙타占쏙옙 占쏙옙占쏙옙
	PMW_SET_SHARETARGET_5,					// 5占쏙옙 占쏙옙占쏙옙타占쏙옙 占쏙옙占쏙옙
	PMW_UNSET_SHARETARGET_1,				// 1占쏙옙 占쏙옙占쏙옙타占쏙옙 占쏙옙占쏙옙
	PMW_UNSET_SHARETARGET_2,				// 2占쏙옙 占쏙옙占쏙옙타占쏙옙 占쏙옙占쏙옙
	PMW_UNSET_SHARETARGET_3,				// 3占쏙옙 占쏙옙占쏙옙타占쏙옙 占쏙옙占쏙옙
	PMW_UNSET_SHARETARGET_4,				// 4占쏙옙 占쏙옙占쏙옙타占쏙옙 占쏙옙占쏙옙
	PMW_UNSET_SHARETARGET_5,				// 5占쏙옙 占쏙옙占쏙옙타占쏙옙 占쏙옙占쏙옙
	PMW_PARTY_MENU_SWITCH_MEMBER,			///< 占쏙옙티 占심뱄옙창 占쏙옙占쏙옙치
	PMW_PARTY_MENU_SWITCH_MEMBER_BUFF,		///< 占쏙옙티 占심뱄옙창 占쏙옙占쏙옙 占쏙옙占쏙옙치
	PMW_PARTY_DUNGEON_INIT,					///< 占쏙옙占쏙옙 占십깍옙화
	PMW_PARTY_DUNGEON_NORMAL,				///< 占쏙옙占쏙옙 占쏙옙占싱듸옙 占쏙옙占쏙옙
	PMW_PARTY_DUNGEON_HARD,					///< ���� ���̵� �����
	PMW_PARTY_DUNGEON_LEGENDARY,
	PMW_PARTY_DIVIDE_PICKUP_PERSON,			///< �ݴ� ��� ȹ��
	PMW_PARTY_DIVIDE_EQUAL,					///< 占싫곤옙占쏙옙 占쏙옙占쏙옙占쏙옙
	PMW_PARTY_DIVIDE_ORDER,					///< 占쏙옙占쏙옙占실뤄옙 占싻뱄옙
	PMW_PARTY_GRADE_ITEM,					///< ��޺� �����Ƿ� �й�
	PMW_PARTY_LOOTING_BY_DICE,
	PMW_PARTY_LOOTING_BY_DICE_BY_EQUIPPED,
	PMW_PARTY_GRADE_NORMAL,
	PMW_PARTY_GRADE_SUPERIOR,
	PMW_PARTY_GRADE_EXCELLENT,
	PMW_PARTY_GRADE_RARE,
	PMW_PARTY_GRADE_LEGENDARY,
	PMW_PARTY_USE_INVEN,
	PMW_USER_BLOCK,
	PMW_ITEM_SORT,
	PMW_HELP,
	PMW_SCOUTER_CHECK_MENU_EQUIPMENT,
	PMW_WAGU_ITEM_BUNCH,
	PMW_WAGU_ITEM_BUNCH2,
	PMW_ITEM_DISASSEMBLE,

	NUM_PMW
};

enum EPlace
{
	PLACE_NONE = 0,
	PLACE_EQUIP,
	PLACE_BAG,
	PLACE_SCOUTER_SLOT,
	PLACE_BAGSLOT,
	PLACE_POPUPMENU,
	PLACE_QUICKSLOT,
	PLACE_SKILL,
	PLACE_BUFF,
	PLACE_NPCSHOP,
	PLACE_TRADECART,	
	PLACE_QUESTBAG,
	PLACE_ITEMUPGRADE,
	PLACE_WAREHOUSE,
	PLACE_PRIVATESHOP,				///< 占쏙옙占싸삼옙占쏙옙
	PLACE_PRIVATESHOP_TRADEBOX,		///< 占쏙옙占싸삼옙占쏙옙 占쏙옙占쏙옙 (占쏙옙占쏙옙 占쏙옙占쏙옙)
	PLACE_GUILD_WAREHOUSE,
	PLACE_DOJO_UPRAGE,
	PLACE_DOGI,
	PLACE_ITEM_OPTIONRECONSTRUCTION,

	PLACE_SUB_SKILLRQ,
	PLACE_SUB_SKILLNOTLEARN,
	PLACE_SUB_HTB,
	PLACE_SUB_QUICKSLOTITEM,
	PLACE_SUB_QUICKSLOTSKILL,
	PLACE_SUB_TARGETUI,		// 36 in tw
	PLACE_SUB_PARTY_MENU_MEMBER,
	PLACE_SUB_PARTY_MENU_DIVIDE_ZENNY,
	PLACE_SUB_PARTY_MENU_DIVIDE_ITEM,
	PLACE_SUB_PARTY_MENU_SWITCH,
	PLACE_SUB_PARTY_MENU_DUNGEON,
	PLACE_SUB_PARTYMEMBERUI,
	PLACE_SUB_GUILD_DIALOG,
	PLACE_SUB_MAILSYSTEM_ITEM,
	PLACE_SUB_HOIPOIMIXCREATE,

	PLACE_SUB_BIND_SEALING_ITEM,
		
	// Zenny
	PLACE_ZENNY_FIRST,

	PLACE_SUB_BAG_ZENNY = PLACE_ZENNY_FIRST,
	PLACE_SUB_BAG,
	PLACE_SUB_WAREHOUSE_ZENNY,
	PLACE_SUB_GUILD_WAREHOUSE_ZENNY,
	PLACE_SUB_TRADECART_ZENNY,
	PLACE_SUB_MAILSYSTEM_ZENNY,

	PLACE_SUB_AUCTIONHOUSE_MIN_LEVEL,
	PLACE_SUB_AUCTIONHOUSE_MAX_LEVEL,
	PLACE_SUB_AUCTIONHOUSE_PRICE,
	PLACE_SUB_AUCTIONHOUSE_ITEM,

	PLACE_CCBD_DIALOG,

	PLACE_ZENNY_LAST = PLACE_CCBD_DIALOG,
};


enum ELogInStageState
{
	LOGIN_STATE_NONE,
	LOGIN_STATE_LOGO,
	LOGIN_STATE_SERVER_CONNECT,	
	LOGIN_STATE_SERVER_CONNECT_FAIL,	
	LOGIN_STATE_IDLE,
	LOGIN_STATE_CONTRACT,
	LOGIN_STATE_LOGINREQ,
	LOGIN_STATE_CHAR_SERVER_CONNECT,		// character server connecting
	LOGIN_STATE_CHAR_SERVER_LOGIN,			// character server login	
	LOGIN_STATE_EXIT,						// character server login	
	LOGIN_STATE_END
};

enum ECharacterStageState
{
	CHAR_STATE_NONE,

	CHAR_STATE_BEGIN,
	
	CHAR_STATE_SERVER_INFORMAION,		// server information 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙.
	CHAR_STATE_SERVER_ENTER,
	CHAR_STATE_SERVER_IDLE,
	CHAR_STATE_SERVER_CHANGE,
	CHAR_STATE_SERVER_EXIT,		// 14 in tw

	CHAR_STATE_MAKE_ENTER,
	CHAR_STATE_MAKE_IDLE,		// on tw its 15
	CHAR_STATE_MAKE_EXIT,
	CHAR_STATE_MAKE_REQUEST,

	CHAR_STATE_SELECT_INFOMATION,		// Channel Infomation 占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙 占쏙옙
	CHAR_STATE_SELECT_INFOMATION_FROM_SERVER_LIST,		// ���� ����Ʈ ȭ�鿡�� ä�� ���� ��û��	// 13 on tw
	CHAR_STATE_SELECT_ENTER,	// 21 in tw
	CHAR_STATE_SELECT_IDLE,		// on tw its 19
	CHAR_STATE_SELECT_DEL_CHAR,
	CHAR_STATE_SELECT_RECOVERY_CHAR,
	CHAR_STATE_SELECT_RENAME_REQ,
	CHAR_STATE_SELECT_EXIT,			// on tw its 22

	CHAR_STATE_RETURN_LOGIN,

	CHAR_STATE_GAME_SERVER_WAIT_CHECK_REQ,
	CHAR_STATE_CHARACTER_SELECT_REQ,
	CHAR_STATE_SUCCESS_ENTER_GAME,
	CHAR_STATE_WAITING_GAME_SERVER_RESPONSE,
	CHAR_STATE_FAIL_ENTER,

	CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_ENTER,
	CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_IDLE,
	CHAR_STATE_IMMEDIATLY_RETURN_LOGIN_EXIT,

	CHAR_STATE_READY_ENTER_TUTORIAL,
	CHAR_STATE_WAIT_FOR_TUTORIAL,
	CHAR_STATE_WAIT_CANCEL,

	CHAR_STATE_WAIT_GAME_SERVER_CONNECT,			//in tw its 37
	CHAR_STATE_CANCEL_WAIT_GAME_SERVER_CONNECT,

	CHAR_STATE_END,
	CHAR_MAX_STATE
};

enum EGameStageState
{
	GAME_STATE_NONE,
	GAME_STATE_IDLE,
	GAME_STATE_CHAR_SERVER_CONNECT,
	GAME_STATE_CHAR_SERVER_LOGIN,			// character server login	
    GAME_STATE_CHAR_EXIT,                   // Clicking terminate connection in the game to go to the character selection screen
	GAME_STATE_END
};

struct sMsgDboItemInfo
{
	RwUInt32		hSerial;
	EPlace			ePlace;
	RwInt32			nPlaceIdx;
	RwInt32			nSlotIdx;	
};

struct sMsgDboBindEquipToEquipSlotMove
{
	RwUInt32		hSerial;
	RwInt8			bySrcPlace;
	RwInt8			bySrcPos;
	RwInt8			byDestPos;
	bool			bBank;
	bool			bGuildBank;
	RwUInt32		hNPCHandle;
};

struct sMsgShopBuyCart
{
	BYTE byMerchantTab;
	BYTE byItemPos;
	BYTE byStack;
};

struct sMsgChangeItemBattleAttribute
{
	HOBJECT m_hNPCSerial;
	BYTE	byItemPlace;
	BYTE	byItemPos;
	BYTE	byAdditialItemPlace;
	BYTE	byAdditialItemPos;
	BYTE	byAdditionalAttribute;
};

/**
* \brief UNION - 占쏙옙占쏙옙 占쏙옙占쏙옙
*/
struct sMsgBoxData
{
	union
	{
		sMsgDboItemInfo			sItemInfo;

		sMsgShopBuyCart			sShopBuy;

		sMsgDboBindEquipToEquipSlotMove	sMoveEquipInfo;
		
		RwUInt32				uiTblIdx;

		RwUInt64				uiParam;

		struct  
		{
			SERIAL_HANDLE		hHandle;
			RwUInt8				byIndex;
		};

		struct
		{
			sTS_KEY				sTSKey;	
			RwUInt32			uiQuestTitle;
		};

		VOID*					pData;

		RwUInt8					ucData[256];			// Reserved;
	};

	const WCHAR*				pwcText;
	SERIAL_HANDLE				hExtendHandle;				///< The user sets / uses TEXT and SerialHandle simultaneously in Input Box.
};


struct sMINIMAPINFO
{
	RwInt32			iType;
	std::wstring	wcsString;
};

typedef std::list<sMINIMAPINFO>					MINIMAPINFO_LIST;
typedef std::list<sMINIMAPINFO>::iterator		MINIMAPINFO_ITER;


struct sQUEST_SEARCH_INFO
{
	std::wstring*	pwstrNPCName;
	std::list<std::wstring>* pListQuestTitle;
};

typedef std::list<sQUEST_SEARCH_INFO>				QUEST_SEARCH_LIST;
typedef std::list<sQUEST_SEARCH_INFO>::iterator		QUEST_SEARCH_ITER;


enum eMIniMapInfoType
{
	MMIT_NPC,
	MMIT_MOB,
	MMIT_QUEST,
	MMIT_PARTY,
	MMIT_LANDMARK,
	MMIT_BIND_POS,
	MMIT_NEXTQUEST_POSITION,

	NUM_MMIT
};

struct sDetailTime
{
	RwUInt32			uiYear;
	RwUInt8				byMonth;
	RwUInt8				byDay;
	RwUInt8				byHour;
	RwUInt8				byMinute;
	RwUInt8				bySecond;

	std::wstring		strDay;			///< 占쏙옙占쏙옙

	RwUInt8				byDay_of_Year;	///< 占쏙옙 占쏙옙占쏙옙 占쏙옙 占쏙옙째 占쏙옙占싸곤옙
};

struct sWorldPickInfo_for_Cursor
{
	RwBool				bWorldPick;
	CNtlSob*			pSobObj;
};

#endif
