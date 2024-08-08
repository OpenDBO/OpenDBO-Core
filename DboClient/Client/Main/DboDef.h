/****************************************************************************
 *
 * File			: DboDef.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
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
// client version

#define	CLIENT_LVER		0
#define CLIENT_RVER		50

//----------------------------------------------------------------------------
#define DEFAULT_SCREEN_WIDTH	1024	
#define DEFAULT_SCREEN_HEIGHT	768

#define EXTRA_SCREEN_WIDTH		1280		// cj의 요청에 의해서.
#define EXTRA_SCREEN_HEIGHT		1024		// cj의 요청에 의해서.

#define DEFAULT_CAMERA_NEAR		0.5f
#define DEFAULT_CAMERA_FAR		512.0f

//----------------------------------------------------------------------------
// serialize file

#define USEROPT_SERIALIZE_FOLDERNAME		".\\user"	

#define ACCOUNTOPT_SERIALIZE_FILENAME		"Account.opt"
#define SYSTEMENV_SERIALIZE_FILENAME		"SystemEnv.txt"
#define GAMEENV_SERIALIZE_FILENAME			"GameEnv.txt"

#define CHARACTERENV_SERIALIZE_FILENAME		"CharacterEnv.txt";
#define SCOUTEROPT_SERILIZE_FILENAME		"Scouter.opt";
#define QUESTOPT_SERIALIZE_FILENAME			"QuestIndicator.opt";


//----------------------------------------------------------------------------
// client control data

#define PICK_TERRAIN_LIMIT		300.0f
#define PICK_OBJECT_LIMIT		100.0f

#define MOUSE_DASH_LIMIT		0.5f

//-----------------------------------------------------------------------------
// stage name
#define SECURITY_STAGE_NAME			"Security"
#define WEB_LOGIN_STAGE_NAME		"WebLogIn"
#define LOGIN_STAGE_NAME			"LogIn"
#define CHARACTER_STAGE_NAME		"Character"
#define GAME_STAGE_NAME				"Game"
#define GAME_LOADING_STAGE_NAME		"GameLoading"

//-----------------------------------------------------------------------------
// client 자체 buffer length

#define MAX_VIRTUAL_SERVER_PACKET_SIZE		2048
#define MAX_MSG_BOX_STRING_LEN				512
#define CHAT_MSG_BUFFER_LEN					240		/** 클라이언트 자체 사용 */

//-----------------------------------------------------------------------------
// Message sender name
#define SENDERNAME_SYSTEM	L"System"
#define SENDERNAME_NOTICE	L"Notice"


//-----------------------------------------------------------------------------
// Error Report
#define ERROR_REPORT_FILE_NAME      "DboErrorInfo.xml"

//-----------------------------------------------------------------------------
// client data folder
#define HUMAN_GUI_TEXTURE_PATH		"gui\\human\\"
#define NAMEK_GUI_TEXTURE_PATH		"gui\\namek\\"
#define MAJIN_GUI_TEXTURE_PATH		"gui\\majin\\"

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
#define dDELAY_BEFORE_VISIBLE_MESSAGEBOX		7.f		// 메세지 박스가 뜨기전까지 주는 지연시간
#define dREMAIN_FOR_SERVER_REPLY				60.f	// 서버 응답을 기다리는 최대 시간

//-----------------------------------------------------------------------------
// guild contribution zenny
#define dGUILD_CONTRIBUTION_ZENNY_1000			1000
#define dGUILD_CONTRIBUTION_ZENNY_5000			5000
#define dGUILD_CONTRIBUTION_ZENNY_10000			10000
#define dGUILD_CONTRIBUTION_ZENNY_50000			50000
#define dGUILD_CONTRIBUTION_ZENNY_100000		100000

//-----------------------------------------------------------------------------
// Moive
#define dLOGO_MOVIE								"movie/logo.avi"
#define dOPENING_MOVIE							"movie/opening.avi"

//-----------------------------------------------------------------------------
// gui Message box
// msg type begin : 여기서 부터 end 까지 

enum EMsgBoxWork
{	
	MBW_NONE,
	MBW_REGEN_MB_MAINWORLD_MOVE_TO_POPOSTONE,		/// 메인 월드에서 포포의 돌로 이동.
	MBW_REGEN_MB_TLQ_MOVE_TO_MAINWORLD,			/// TLQ에서 메인월드 지정위치로.
	MBW_REGEN_MB_TLQ_RETRY,				/// TLQ에서 현재위치 부활.
	MBW_REGEN_MB_TMQ_MOVE_TO_MAINWORLD,			/// TMQ에서 메인월드 지정위치로.
	MBW_REGEN_MB_PARTYDUNGEON_RETRY,			/// 파티 던전에서 초기위치로 부활.
	MBW_REGEN_MB_DOJO_RETRY,				/// 도장쟁탈전에서 초기위치로 부활.

	MBW_NET_CONNECT_FAIL,
	MBW_NET_DISCONNECT,
	MBW_GAME_EXIT,
	MBW_CAHR_SERVER_EXIT,

	MBW_LOGIN_CONNECT_FAIL,					///< Auth server connect fail...
	MBW_AUTHKEY_FAIL,						///< Auth server key 인증 fail..
	MBW_CHARACTRE_CONNECT_FAIL,				///< Auth server connect fail...

	MBW_DEL_CHARACTER,						///< 캐릭터를 지운다
	MBW_RECOVERY_CHARACTER,					///< 캐릭터를 복구한다

	MBW_POST_QUIT_MESSAGE,					///< 클라이언트를 종료한다

	MBW_ASK_TUTORIAL,						///< 튜토리얼로 진행할지 물어본다
	MBW_WAIT_FOR_TUTORIAL,					///< 튜토리얼 대기중

	MBW_ITEMDROP_CONFIRM,					///< 아이템 드롭 확인

	MBW_PARTY_CREATE,						///< 파티 만들기
	MBW_PARTY_INVITE_REQ,					///< 파티 초대 응답
	MBW_PARTY_LEAVE,						///< 파티 탈퇴
	MBW_PARTY_RELEASE_CHARM,				///< 파티 참 버프 해제

	MBW_BUY_WAREHOUSE,						///< 창고 슬롯을 구입하겠습니까
	MBW_BUY_COMMON_WAREHOUSE,				///< 공유 창고 슬롯을 구입하겠습니까

	MBW_FREEPVP_CHALLENGE,					///< free pvp 도전 신청

	MBW_BIND_POPOSTONE,						///< 포포스톤 바인드.

	MBW_QUEST_GIVEUP,

	MBW_USER_TRADE_REQ,

	MBW_GUILD_CREATE,						///< 길드 생성
	MBW_GUILD_INVITE_REQ,					///< 길드 초대 응답
	MBW_GUILD_DISSOLVE,						///< 길드 해산
	MBW_GUILD_DISSOLVE_CANCLE,				///< 길드 해산 취소
	MBW_GUILD_CHANGE_MASTER,				///< 길드장 이양
	MBW_GUILD_APPOINT_SECOND_MASTER,		///< 부길드장 임명
	MBW_GUILD_DISAPPOINT_SECOND_MASTER,		///< 부길드장 임명 해제
	MBW_GUILD_KICK_OUT,						///< 길드 추방
	MBW_GUILD_LEAVE,						///< 길드 탈퇴
	MBW_GUILD_ADD_FUNCTION,					///< 길드 기능 추가
	MBW_GUILD_CONTRIBUTION,					///< 길드 제니 기부
	MBW_GUILD_CHANGE_GUILD_NAME,			///< 길드 이름 변경

	MBW_TUTORIAL_ASK_END,					///< 튜토리얼 종료

	MBW_PRIVATESHOP_SALE_START,				///< 개인상점 판매를 시작하겠습니까?
	MBW_PRIVATESHOP_SALE_STOP,				///< 개인상점 판매를 중지하겠습니까?

	MBW_PRIVATESHOP_BUSINESS_WAIT,			///< 주인이 흥정 수락을 할 때까지 기다려 주십시오.
	MBW_PRIVATESHOP_BUSINESS_CONSENT,		///< 손님이 흥정을 요청해왔습니다. 수락하시겠습니까?
	MBW_PRIVATESHOP_BUSINESS_IMMEDIATE,		///< 정말 구입하시겠습니까?

	MBW_MAILSYSTEM_SEND_ASK,				///< 메일을 보내시겠습니까?
	MBW_MAILSYSTEM_AWAY_ON,
	MBW_MAILSYSTEM_AWAY_OFF,
	MBW_MAILSYSTEM_RECEIVE_ASK,
	MBW_MAILSYSTEM_DELETE_ASK,
	MBW_MAILSYSTEM_RETURN_ASK,

	MBW_PORTAL_ADD_OK,

	MBW_FRIENDLIST_ADD_ASK,                 ///< 친구 리스트에 대상을 등록하겠습니까?
	MBW_BLACKLIST_ADD_ASK,                  ///< 블랙 리스트에 대상을 등록하겠습니까?
	MBW_FRIENDLIST_DEL_ASK,                 ///< 친구 리스트에서 삭제하시겠습니까?
	MBW_BLACKLIST_DEL_ASK,                  ///< 블랙 리스트에서 삭제하시겠습니까?

	MBW_GET_ON_BUS_ASK,		                ///< 버스에 탑승하시겠습니까?
	MBW_GET_OFF_BUS_ASK,					///< 버스에서 내리시겠습니까?

	MBW_GET_CP_FROM_ITEM,					///< 아이템을 CP로 전환하시겠습니까?
	MBW_ASK_CHANGE_CHANNEL,					///< 채널을 변경하시겠습니까?

	MBW_DO_NOT_ANYTHING,					///< 아무것도 하지 않는다

	MBW_TELEPORT_CONFIRM_TIMEOUT,			///< DST_TELEPORT_COMMON_CONFIRM_MSG
	MBW_TELEPORT_COMMON_CONFIRM_YES,		///< DST_TELEPORT_COMMON_CONFIRM_YES_BTN
	MBW_TELEPORT_COMMON_CONFIRM_NO,			///< DST_TELEPORT_COMMON_CONFIRM_NO_BTN
	MBW_TELEPORT_FINAL_CONFIRM_TIMEOUT,		///< DST_TELEPORT_FINAL_CONFIRM_MSG
	MBW_TELEPORT_FINAL_CONFIRM_YES,		///< DST_TELEPORT_FINAL_CONFIRM_YES_BTN
	MBW_TELEPORT_FINAL_CONFIRM_NO,		///< DST_TELEPORT_FINAL_CONFIRM_NO_BTN


	MBW_PORTAL_CONFIRM,                     ///< 포탈 이동하시겠습니까?

	MBW_TB_REQUEST_INDI_CANCEL,				///< 천하제일 무도회 개인전 신청을 취소하시겠습니까?
	MBW_TB_REQUEST_TEAM_CANCEL,				///< 천하제일 무도회 팀전 신청을 취소하시겠습니까?

	MBW_HOIPOIMIX_SET_FARE,					///< 호이포이 믹스 사용료를 세팅하세요.

	MBW_VEHICLE_ASK_GET_OFF,				///< 내리시겠습니까?

	MBW_LOBBY_WAIT_GAME_SERVER_CONNECT,		///<당신은 %d번째 대기자입니다

	MBW_DOJO_ASK_ESTABLISHMENT, 			//"도장을 설립하시겠습니까? 500,000 제니가 필요합니다(MsgBox : 도장을 설립하려고 할 때"
	MBW_DOJO_ASK_UPGRAGE_WITH_ZENNY,		//"도장을 업그레이드 하시겠습니까? 확인을 누리시면 %d 제니가 소모됩니다"
	MBW_DOJO_ASK_UPGRAGE_WITH_ZENNY_AND_HOIPOI,	//"도장을 업그레이드 하시겠습니까? 확인을 누리시면 %d 제니와 호이포이락 % 개가 소모됩니다"
	MBW_DOJO_ADD_FUNCTION,					///< 도장 기능을 추가하기겠습니까?

	MBW_DOJO_SCRAMBLE_ACCEPT_CHALLENGE_OK,		// %s 유파의 쟁탈전 신청을 수락하시겠습니까[br][br]거부를 선택하시면 배상금으로  %d 제니가 소모됩니다 (수락을 눌렀을 때)
	MBW_DOJO_SCRAMBLE_ACCEPT_CHALLENGE_CANCEL,	// %s 유파의 쟁탈전 신청을 수락하시겠습니까[br][br]거부를 선택하시면 배상금으로  %d 제니가 소모됩니다 (거부를 눌렀을 때) 
	MBW_DOJO_SCRAMBLE_ASK_CHALLENGE_OK,		// 도장 쟁탈전을 신청하시겠습니까[br]도장 쟁탈전 신청금 %d 제니가 필요합니다 (신청을 눌렀을 때)
	MBW_DOJO_SCRAMBLE_ASK_TELEPORT_OK,		// 쟁탈전이 진행중인 도장 안으로 순간이동 하시겠습니까 (이동을 눌렀을 때)

	MBW_COMMERCIAL_MB_UPDATE_TERM_LIMETED_ITEM,	// 사용기간 만료된 아이템의 기간을 연장하기.

	MBW_DOJO_ADD_TENKAI_SEED,			// 천하제일 무도회 시드 출전자 이름을 입력해 주십시요(Msgbox : 도장을 가진 유파장이 천하제일 무도회 시드 출전자를 결정할 때)

	MBW_DOGI_ASK_APPLY_CHANGE,			// 유파의 도복 설정을 완료하시겠습니까?(Msgbox : 도복 설정을 적용할지 물어볼 때)
	MBW_DOJO_DOGI_ASK_APPLY_CHANGE,			// 도장의 도복 설정을 완료하시겠습니까?(Msgbox : 도복 설정을 적용할지 물어볼 때)
	MBW_HOIPOIMIX_SKILL_SET_MSGBOX,			// [MSGBOX]이 제조 기술을 배우시겠습니가?[br]%s 제조기술
	MBW_HOIPOIMIX_SKILL_RESET_MSGBOX,		// [MSGBOX]이 제조 기술을 포기하시겠습니까?[br]%s 제조기술[br]제조 경험치와 제조레벨이..
	MBW_LOBBY_ASK_RENAME,				// 상용화 아이템으로 캐릭터 이름 변경할 때
	MBW_FORCED_LOBBY_ASK_RENAME,			// 강제적으로 이름을 변경되어 복구된 캐릭터의 이름을 변경할 때

	MBW_COMMERCIAL_MB_ITEM_TERM_EXTENTION,		// 기간 구매/아이템 구매 선택 MSG 박스
	MBW_COMMERCIAL_MB_CAPSULEKIT_TERM_EXTENTION,	// 캡슐키트, 기간/아이템 구매 선택 MSG 박스 - 기간 구매 BTN
	MBW_COMMERCIAL_MB_CAPSULEKIT_BUY,		// 캡슐키트, 기간/아이템 구매 선택 MSG 박스 - 아이템 구매 BTN
	MBW_COMMERCIAL_MB_TERM_OR_BUY_SELECT_CONFIRM,	// 기간구매, 아이템 구매 선택 MSG 박스 - 기간 구매 BTN
	MBW_COMMERCIAL_MB_CAPSULEKIT_ITEM_TERM_EXTENTION,	// 캡슐키트 기간 구매/아이템 구매 선택 MSG 박스 - 기간 연장 BTN
	MBW_COMMERCIAL_MB_CAPSULEKIT_MOVE,		// 캡슐키트 기간 구매/아이템 구매 선택 MSG 박스 - 아이템 이동 BTN
	MBW_COMMERCIAL_MB_TERM_EXTENTION,		// 기간만료 확인 MSG 박스 - 기간 구매 BTN

	MBW_COMMERCIAL_MB_DIRECT_BUY,			// 기간제 아이템 바로 구매 확인,   DST_COMMERCIAL_MB_DIRECT_BUY_MSG
	MBW_COMMERCIAL_MB_ITEM_BUY_CONFIRM,		// 기간제 아이템 구매 확인, DST_COMMERCIAL_MB_ITEM_BUY_CONFIRM_MSG, DST_COMMERCIAL_MB_ITEM_MOVE_CONFIRM_MSG

	MBW_COMMERCIAL_MB_DUR_ITEM_GET_FROM_YARDRAT, // 야드래트로부터 기간제 아이템 get start point DST_COMMERCIAL_MB_ITEM_TAKE_CONFIRM_MSG
};

// msgtype end

enum EMsgBoxTypeFlag
{
	MBTF_NONE			= 0x00,
	MBTF_OK				= 0x01,
	MBTF_CANCEL			= 0x02,
	MBTF_INPUT			= 0x04,
	MBTF_CUSTOM			= 0x08,
};

enum EMsgBoxResult
{
	MBR_OPENCANCELED	= -1,				/// 메시지박스 열림자체가 취소. 메시지 박스가 중복인 경우.
	MBR_CANCEL			= 0,				/// 취소.
	MBR_OK				= 1,				/// 확인, 승인,	
};

struct sMsgBoxCustomBtn
{
	sMsgBoxCustomBtn(VOID)
	{
		Init();
	}

	VOID sMsgBoxCustomBtn::Init(VOID)
	{
		uiStringID = INVALID_DWORD;
		uiWorkID = MBW_NONE;
	}

	RwUInt32 uiStringID;
	RwUInt32 uiWorkID;
};

//-----------------------------------------------------------------------------

///< avooo : 표시되는 우선순위에 따라 정리하였다.
///< peessi : Display String Define에 정의된 문자열의 순서도 일치해야함.
enum EIconPopupWork	
{
	PMW_USE = 0,						    ///< 사용하기
	PMW_OPEN,								///< (가방)열기
	PMW_EQUIP,								///< 장착하기
	PMW_CANCEL_EQUIP,						///< 장착해제
	PMW_VIEW,								///< 보기
	PMW_PICKUP,								///< 집기
	PMW_STOP_USE,							///< 사용중지
	PMW_PULLOUT,							///< 등록취소
	PMW_SELL,								///< 팔기
	PMW_DIVIDE,								///< 나누기
	PMW_CLEAR,								///< 비우기
	PMW_DELETE,								///< 버리기
	PMW_PARTY_CREATE,						///< 파티 만들기
	PMW_PARTY_INVITE,						///< 파티 초대
	PMW_PARTY_LEADER_CHANGE,				///< 파티장 위임
	PMW_PARTY_KICK_OUT,						///< 파티 강퇴
	PMW_PARTY_LEAVE,						///< 파티 탈퇴
	PMW_PVP_REQUEST_FIGHT,					///< 대련 요청
	PMW_USER_TRADE,							///< 유저 트레이드
	PMW_GUILD_INVITE,						///< 길드 초대
	PMW_GUILD_CHANGE_MASTER,				///< 길드장 이양
	PMW_GUILD_APPOINT_SECOND_MASTER,		///< 부길드장 임명
	PMW_GUILD_DISAPPOINT_SECOND_MASTER,		///< 부길드장 임명 해제
	PMW_GUILD_KICK_OUT,						///< 길드 추방
	PMW_GUILD_POST,							///< 우편 보내기
	PMW_FRIEND_ADD,							///< 친구 추가
	PMW_FOLLOW,								///< 따라 가기
	PMW_CANCEL,								///< 취소
	PMW_SET_SHARETARGET_1,					// 1번 공유타겟 설정
	PMW_SET_SHARETARGET_2,					// 2번 공유타겟 설정
	PMW_SET_SHARETARGET_3,					// 3번 공유타겟 설정
	PMW_SET_SHARETARGET_4,					// 4번 공유타겟 설정
	PMW_SET_SHARETARGET_5,					// 5번 공유타겟 설정
	PMW_UNSET_SHARETARGET_1,				// 1번 공유타겟 해제
	PMW_UNSET_SHARETARGET_2,				// 2번 공유타겟 해제
	PMW_UNSET_SHARETARGET_3,				// 3번 공유타겟 해제
	PMW_UNSET_SHARETARGET_4,				// 4번 공유타겟 해제
	PMW_UNSET_SHARETARGET_5,				// 5번 공유타겟 해제
	PMW_PARTY_MENU_SWITCH_MEMBER,			///< 파티 맴버창 스위치
	PMW_PARTY_MENU_SWITCH_MEMBER_BUFF,		///< 파티 맴버창 버프 스위치
	PMW_PARTY_DUNGEON_INIT,					///< 던전 초기화
	PMW_PARTY_DUNGEON_NORMAL,				///< 던전 난이도 보통
	PMW_PARTY_DUNGEON_HARD,					///< 던전 난이도 어려움
	PMW_PARTY_DIVIDE_PICKUP_PERSON,			///< 줍는 사람 획득
	PMW_PARTY_DIVIDE_EQUAL,					///< 똑같이 나누기
	PMW_PARTY_DIVIDE_ORDER,					///< 순서되로 분배
	PMW_PARTY_GRADE_ITEM,					///< 등급별 순서되로 분배
	PMW_PARTY_USE_INVEN,					///< 파티 인벤토리 사용

	NUM_PMW
};

enum EPlace
{
	PLACE_NONE = 0,
	PLACE_EQUIP,
	PLACE_BAG,
	PLACE_SCOUTER,
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
	PLACE_PRIVATESHOP,				///< 개인상점
	PLACE_PRIVATESHOP_TRADEBOX,		///< 개인상점 흥정 (가격 조정)
	PLACE_PARTYCHARM,
	PLACE_GUILD_WAREHOUSE,
	PLACE_DOJO_UPRAGE,
	PLACE_DOGI,

	PLACE_SUB_SKILLRQ,
	PLACE_SUB_SKILLNOTLEARN,
	PLACE_SUB_HTB,
	PLACE_SUB_QUICKSLOTITEM,
	PLACE_SUB_QUICKSLOTSKILL,
	PLACE_SUB_TARGETUI,
	PLACE_SUB_PARTY_MENU_MEMBER,
	PLACE_SUB_PARTY_MENU_DIVIDE_ZENNY,
	PLACE_SUB_PARTY_MENU_DIVIDE_ITEM,
	PLACE_SUB_PARTY_MENU_SWITCH,
	PLACE_SUB_PARTY_MENU_DUNGEON,
	PLACE_SUB_PARTYMEMBERUI,
	PLACE_SUB_GUILD_DIALOG,
	PLACE_SUB_MAILSYSTEM_ITEM,
	PLACE_SUB_HOIPOIMIXCREATE,
		
	// Zenny
	PLACE_ZENNY_FIRST,

	PLACE_SUB_BAG_ZENNY = PLACE_ZENNY_FIRST,
	PLACE_SUB_PARTYCHARM_ZENNY,
	PLACE_SUB_PARTYINVEN_ZENNY,
	PLACE_SUB_WAREHOUSE_ZENNY,
	PLACE_SUB_GUILD_WAREHOUSE_ZENNY,
	PLACE_SUB_TRADECART_ZENNY,
	PLACE_SUB_MAILSYSTEM_ZENNY,

	PLACE_ZENNY_LAST = PLACE_SUB_MAILSYSTEM_ZENNY,
};


enum ELogInStageState
{
	LOGIN_STATE_NONE,
	LOGIN_STATE_LOGO,
	LOGIN_STATE_SERVER_CONNECT,	
	LOGIN_STATE_SERVER_CONNECT_FAIL,	
	LOGIN_STATE_IDLE,
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
	
	CHAR_STATE_SERVER_INFORMAION,		// server information 정보 얻어오는 중.
	CHAR_STATE_SERVER_ENTER,
	CHAR_STATE_SERVER_IDLE,
	CHAR_STATE_SERVER_CHANGE,
	CHAR_STATE_SERVER_EXIT,

	CHAR_STATE_MAKE_ENTER,
	CHAR_STATE_MAKE_IDLE,
	CHAR_STATE_MAKE_EXIT,
	CHAR_STATE_MAKE_REQUEST,

	CHAR_STATE_SELECT_INFOMATION,		// Channel Infomation 정보 얻어오는 중
	CHAR_STATE_SELECT_INFOMATION_FROM_SERVER_LIST,		// 서버 리스트 화면에서 채널 정보 요청중
	CHAR_STATE_SELECT_ENTER,
	CHAR_STATE_SELECT_IDLE,
	CHAR_STATE_SELECT_DEL_CHAR,
	CHAR_STATE_SELECT_RECOVERY_CHAR,
	CHAR_STATE_SELECT_RENAME_REQ,
	CHAR_STATE_SELECT_EXIT,

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

	CHAR_STATE_WAIT_GAME_SERVER_CONNECT,
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
    GAME_STATE_CHAR_EXIT,                   // 게임에서 접속 종료를 클릭하여 캐릭터 선택화면으로 나갈때
	GAME_STATE_END
};

struct sDboItemDeleteInfo
{
	RwUInt32		hSerial;
	EPlace			ePlace;
	RwInt32			nPlaceIdx;
	RwInt32			nSlotIdx;	
	RwBool			bRemoteSell;
};

/**
* \brief UNION - 사용시 주의
*/
struct sMsgBoxData
{
	union
	{
		sDboItemDeleteInfo		sItemDeleteInfo;	
		
		RwUInt32				uiTblIdx;		

		const WCHAR*			pwcText;

		RwUInt32				uiParam;

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

	SERIAL_HANDLE				hExtendHandle;				///< Input Box에서 TEXT와 SerialHandle을 동시에 받을 경우 사용자가 세팅/사용 한다.
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

	std::wstring		strDay;			///< 요일

	RwUInt8				byDay_of_Year;	///< 한 해의 몇 번째 날인가
};

struct sITEM_PROFILE;
struct sITEM_TBLDAT;

struct sPartyInvenDisplay
{
	sITEM_TBLDAT*		pITEM_TBLDAT;
	sITEM_PROFILE*		pITEM_PROFILE;
	RwUInt8				byCompetitor;
	RwUInt8				byScore;
};

struct sWorldPickInfo_for_Cursor
{
	RwBool				bWorldPick;
	CNtlSob*			pSobObj;
};

#endif
