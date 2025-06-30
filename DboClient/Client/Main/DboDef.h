/****************************************************************************
 *
 * File			: DboDef.h
 * Author		: HyungSuk, Jang
 * Copyright	: (��)NTL
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

#define EXTRA_SCREEN_WIDTH		1280		// cj�� ��û�� ���ؼ�.
#define EXTRA_SCREEN_HEIGHT		1024		// cj�� ��û�� ���ؼ�.

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
// client ��ü buffer length

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
#define dDELAY_BEFORE_VISIBLE_MESSAGEBOX		7.f		// �޼��� �ڽ��� �߱������� �ִ� �����ð�
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
	MBR_OK				= 1,				/// Ȯ��, ����,	
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
///< peessi : Display String Define�� ���ǵ� ���ڿ��� ������ ��ġ�ؾ���.
enum EIconPopupWork
{
	PMW_USE = 0,						    ///< ����ϱ�
	PMW_OPEN,								///< (����)����
	PMW_EQUIP,								///< �����ϱ�
	PMW_CANCEL_EQUIP,						///< ��������
	PMW_VIEW,								///< ����
	PMW_PICKUP,								///< ����
	PMW_STOP_USE,							///< �������
	PMW_PULLOUT,							///< ������
	PMW_SELL,								///< �ȱ�
	PMW_DIVIDE,								///< ������
	PMW_CLEAR,								///< ����
	PMW_DELETE,								///< ������
	PMW_PARTY_CREATE,						///< ��Ƽ �����
	PMW_PARTY_INVITE,						///< ��Ƽ �ʴ�
	PMW_PARTY_LEADER_CHANGE,				///< ��Ƽ�� ����
	PMW_PARTY_KICK_OUT,						///< ��Ƽ ����
	PMW_PARTY_LEAVE,						///< ��Ƽ Ż��
	PMW_PVP_REQUEST_FIGHT,					///< ��� ��û
	PMW_USER_TRADE,							///< ���� Ʈ���̵�
	PMW_GUILD_INVITE,						///< ��� �ʴ�
	PMW_GUILD_CHANGE_MASTER,				///< ����� �̾�
	PMW_GUILD_APPOINT_SECOND_MASTER,		///< �α���� �Ӹ�
	PMW_GUILD_DISAPPOINT_SECOND_MASTER,		///< �α���� �Ӹ� ����
	PMW_GUILD_KICK_OUT,						///< ��� �߹�
	PMW_GUILD_POST,							///< ���� ������
	PMW_FRIEND_ADD,							///< ģ�� �߰�
	PMW_FOLLOW,								///< ���� ����
	PMW_SEND_WHISPER,
	PMW_CANCEL,								///< ���
	PMW_SET_SHARETARGET_1,					// 1�� ����Ÿ�� ����
	PMW_SET_SHARETARGET_2,					// 2�� ����Ÿ�� ����
	PMW_SET_SHARETARGET_3,					// 3�� ����Ÿ�� ����
	PMW_SET_SHARETARGET_4,					// 4�� ����Ÿ�� ����
	PMW_SET_SHARETARGET_5,					// 5�� ����Ÿ�� ����
	PMW_UNSET_SHARETARGET_1,				// 1�� ����Ÿ�� ����
	PMW_UNSET_SHARETARGET_2,				// 2�� ����Ÿ�� ����
	PMW_UNSET_SHARETARGET_3,				// 3�� ����Ÿ�� ����
	PMW_UNSET_SHARETARGET_4,				// 4�� ����Ÿ�� ����
	PMW_UNSET_SHARETARGET_5,				// 5�� ����Ÿ�� ����
	PMW_PARTY_MENU_SWITCH_MEMBER,			///< ��Ƽ �ɹ�â ����ġ
	PMW_PARTY_MENU_SWITCH_MEMBER_BUFF,		///< ��Ƽ �ɹ�â ���� ����ġ
	PMW_PARTY_DUNGEON_INIT,					///< ���� �ʱ�ȭ
	PMW_PARTY_DUNGEON_NORMAL,				///< ���� ���̵� ����
	PMW_PARTY_DUNGEON_HARD,					///< ���� ���̵� �����
	PMW_PARTY_DUNGEON_LEGENDARY,
	PMW_PARTY_DIVIDE_PICKUP_PERSON,			///< �ݴ� ��� ȹ��
	PMW_PARTY_DIVIDE_EQUAL,					///< �Ȱ��� ������
	PMW_PARTY_DIVIDE_ORDER,					///< �����Ƿ� �й�
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
	PLACE_PRIVATESHOP,				///< ���λ���
	PLACE_PRIVATESHOP_TRADEBOX,		///< ���λ��� ���� (���� ����)
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
	
	CHAR_STATE_SERVER_INFORMAION,		// server information ���� ������ ��.
	CHAR_STATE_SERVER_ENTER,
	CHAR_STATE_SERVER_IDLE,
	CHAR_STATE_SERVER_CHANGE,
	CHAR_STATE_SERVER_EXIT,		// 14 in tw

	CHAR_STATE_MAKE_ENTER,
	CHAR_STATE_MAKE_IDLE,		// on tw its 15
	CHAR_STATE_MAKE_EXIT,
	CHAR_STATE_MAKE_REQUEST,

	CHAR_STATE_SELECT_INFOMATION,		// Channel Infomation ���� ������ ��
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
* \brief UNION - ���� ����
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

	std::wstring		strDay;			///< ����

	RwUInt8				byDay_of_Year;	///< �� ���� �� ��° ���ΰ�
};

struct sWorldPickInfo_for_Cursor
{
	RwBool				bWorldPick;
	CNtlSob*			pSobObj;
};

#endif
