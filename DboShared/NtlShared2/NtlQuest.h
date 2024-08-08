//***********************************************************************************
//
//	File		:	NtlQuest.h
//
//	Begin		:	2006-10-31
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlTSCore.h"
#include "NtlSharedType.h"
#include "NtlBitFlag.h"


//===================================================================================
//	퀘스트 아이디
//===================================================================================

// 퀘스트 아이디 사용 용도 정의
//	주의) 아래에 상태 추가 필요성 발생 시 반드시
//		  퀘스트 담당자 ( lleo52@ntl-inc.com )에게 연락 바람
enum eQUEST_ID_RANGE
{
	// General quest는 일반적인 퀘스트로 아래와 같은 아이디 범주내에서 사용되어야 함.
	// 추후 확장 요청시 Reserve quest range를 이용하여 확장하면 됨.
	eQUEST_ID_RANGE_GENERAL_QUEST_MIN		= 0,
	eQUEST_ID_RANGE_GENERAL_QUEST_MAX		= 7000,
	eQUEST_ID_RANGE_GENERAL_QUEST_COUNT		= eQUEST_ID_RANGE_GENERAL_QUEST_MAX - eQUEST_ID_RANGE_GENERAL_QUEST_MIN,

	// Reserve quest는 추후 General quest의 확장을 기획에서 요구할 수 있다는 가정하에
	// 아래와 같이 예약 해 놓음.
	eQUEST_ID_RANGE_RESERVE_QUEST_MIN		= 7000,
	eQUEST_ID_RANGE_RESERVE_QUEST_MAX		= 11000,
	eQUEST_ID_RANGE_RESERVE_QUEST_COUNT		= eQUEST_ID_RANGE_RESERVE_QUEST_MAX - eQUEST_ID_RANGE_RESERVE_QUEST_MIN,

	// Special quest는 Tutorial, TMQ 와 같이 특정 시스템에서 사용하는 퀘스트 아이디로써
	// 특정 시스템이 종료 되는 시점에 자동 삭제되어야 하는 퀘스트에 한해서 사용됨.
	eQUEST_ID_RANGE_SPECIAL_QUEST_MIN		= 11000,
	eQUEST_ID_RANGE_SPECIAL_QUEST_MAX		= 14000,
	eQUEST_ID_RANGE_SPECIAL_QUEST_COUNT		= eQUEST_ID_RANGE_SPECIAL_QUEST_MAX - eQUEST_ID_RANGE_SPECIAL_QUEST_MIN,

	eQUEST_ID_RANGE_QUEST_MAX				= 14000,
};


//===================================================================================
//	완료된 퀘스트 정보
//===================================================================================

//	퀘스트 완료 상태
//	주의) 아래에 상태 추가 필요성 발생 시 반드시
//		  퀘스트 담당자 ( lleo52@ntl-inc.com )에게 연락 바람
enum eCOMPLETE_QUEST_STATE
{
	eCOMPLETE_QUEST_STATE_CLEAR				= 0x1,

	eCOMPLETE_QUEST_STATE_MAX				= 1
};

enum
{
	eCOMPLETE_QUEST_QUEST_PER_BYTE			= 8 / eCOMPLETE_QUEST_STATE_MAX
};

enum
{
	eCOMPLETE_QUEST_QUEST_PER_STRUCT		= 1000
};

enum
{
	eCOMPLETE_QUEST_QUEST_STRUCT_COUNT		= eQUEST_ID_RANGE_QUEST_MAX / eCOMPLETE_QUEST_QUEST_PER_STRUCT
};

#pragma pack( 1 )
struct sCOMPLETE_QUEST_INFO
{
	unsigned char							abyQCInfo[eQUEST_ID_RANGE_QUEST_MAX/eCOMPLETE_QUEST_QUEST_PER_BYTE];
};


struct sCOMPLETE_QUEST_BIT_INFO
{
	unsigned char							abyQCBitInfo[eCOMPLETE_QUEST_QUEST_PER_STRUCT/eCOMPLETE_QUEST_QUEST_PER_BYTE];//125
};

struct sCOMPLETE_QUEST_TOTAL_BIT_INFO
{
	sCOMPLETE_QUEST_BIT_INFO				asQCBitInfo[eCOMPLETE_QUEST_QUEST_STRUCT_COUNT];	//14 
};

#pragma pack()


//===================================================================================
//	진행 중인 퀘스트 정보
//===================================================================================

// 진행 가능한 최대 퀘스트 개수
enum
{
	eMAX_CAN_PROGRESS_QUEST_NUM				= 20,

	eMAX_CAN_PROGRESS_GENERAL_QUEST_NUM		= 10,	// 일반적인 퀘스트
	eMAX_CAN_PROGRESS_SPECIAL_QUEST_NUM		= 10,	// TMQ, Tutorial... 용 퀘스트
};

// 진행 가능한 최대 트리거 개수
enum
{
	eMAX_CAN_PROGRESS_TRIGGER_NUM			= 20
};

// 진행 중인 퀘스트의 상태
// 주) 현재 상태를 저장하기 위한 공간이 2 Byte로 예약 되어 있음.
//	   아래 상태 추가시 2Byte를 넘지 않도록 주의 필요.
enum eTS_SVR_STATE
{
	eTS_SVR_STATE_ERROR						= 0x0001,		// 에러 상태
	eTS_SVR_STATE_FAILED					= 0x0002,		// 퀘스트 진행 실패 상태
	eTS_SVR_STATE_ESCORT					= 0x0004,		// 호위 상태
	eTS_SVR_STATE_SVR_EVT_WORLD_RANGE		= 0x0008,		// 서버이벤트(몹킬...)의 적용 대상이 월드 ( 이 플래그가 켜져 있지 않으면 파티 기준으로 처리됨 )
	eTS_SVR_STATE_WPS_MODE					= 0x0010,		// World play script 액션이 동작중
	eTS_SVR_STATE_USER_OUT_FAILED_WPS		= 0x0020,		// World play script 동작중 유저가 로그 아웃 한 경우 Failed 처리 되어야 하는 경우를 저장하는 플래그
};

#pragma pack( 1 )
struct sMAIN_TSP
{
	NTL_TS_TC_ID							tcCurId;
	NTL_TS_TC_ID							tcPreId;
};
#pragma pack()

#pragma pack( 1 )
struct sEXCEPT_TIMER
{
	NTL_TS_TC_ID							tcId;
	NTL_TS_TA_ID							taId;

	unsigned int							uiRemainTime;
};
#pragma pack()


#pragma pack( 1 )
struct sEXCEPT_TIMER_SLOT
{
	enum { eTIMER_SLOT_MAX = 4 };

	sEXCEPT_TIMER							asExceptTimer[eTIMER_SLOT_MAX];
};
#pragma pack()

#pragma pack( 1 )
struct sSTORAGE_SLOT_MEMORY
{
	enum { eSTORAGE_SLOT_MEMORY_MAX = 8 };

	// 배열의 인덱스가 SSM 의 아이디로 사용됨
	unsigned int							auiSSM[eSTORAGE_SLOT_MEMORY_MAX];
};
#pragma pack()

#pragma pack( 1 )
struct sSTOC_EVT_DB_DATA
{
	enum { eSTOC_EVT_DB_DATA_USER_DATA_MAX = 4 };

	NTL_TS_TC_ID							tcId;
	NTL_TS_TA_ID							taId;
	unsigned int							m_aUserData[eSTOC_EVT_DB_DATA_USER_DATA_MAX];		// tcId, taId에 의해 영향받는 사용자 정의 데이터

	void Init( void )
	{
		tcId = NTL_TS_TC_ID_INVALID;
		taId = NTL_TS_TA_ID_INVALID;
		memset( m_aUserData, 0, sizeof( m_aUserData ) );
	}
};
#pragma pack()

#pragma pack( 1 )
struct sPROGRESS_QUEST_INFO
{
	enum { ePROGRESS_QUEST_INFO_MAX_SIZE = 256 };
	enum { eCUR_VERSION = 0 };

	sPROGRESS_QUEST_INFO( void )
	{
		byVer = eCUR_VERSION;
		tId = NTL_TS_T_ID_INVALID;

		memset( &uData.sQInfoV0, 0xff, sizeof( uData.sQInfoV0 ) );
		uData.sQInfoV0.wQState = 0;
		uData.sQInfoV0.tcQuestInfo = NTL_TS_TC_ID_INVALID;
		uData.sQInfoV0.taQuestInfo = NTL_TS_TA_ID_INVALID;
	}

	unsigned char							byVer;				// 버전
	NTL_TS_T_ID								tId;				// 퀘스트 아이디

	union uDATA
	{
		struct sEXTENDED_POSSIBLE_V0
		{
			unsigned short					wQState;			// 퀘스트 상태
			sMAIN_TSP						sMainTSP;			// 퀘스트 진행 포인터
			sEXCEPT_TIMER_SLOT				sETSlot;			// 예외 타이머의 저장 슬롯
			sSTORAGE_SLOT_MEMORY			sSSM;				// 저장 슬롯 메모리
			sSTOC_EVT_DB_DATA				sSToCEvtData;		// 서버 이벤트 데이터
			NTL_TS_TG_ID					tgExcCGroup;		// 실행할 클라이언트 그룹

			NTL_TS_TC_ID					tcQuestInfo;		// 퀘스트 정보 컨테이너 아이디
			NTL_TS_TA_ID					taQuestInfo;		// 퀘스트 정보 액션 아이디
		};
		sEXTENDED_POSSIBLE_V0				sQInfoV0;
	};
	uDATA									uData;
};
#pragma pack()

//===================================================================================
//
//===================================================================================
typedef NTL_TS_T_ID		QUESTID;
const	QUESTID			INVALID_QUESTID = NTL_TS_T_ID_INVALID;
//===================================================================================


//-----------------------------------------------------------------------------------
// 트리거 오브젝트의 상태
//-----------------------------------------------------------------------------------
typedef BYTE TOBJECT_STATE;

typedef BYTE TOBJECT_SUBSTATE;


enum eTOBJECT_STATE
{
	TOBJECT_STATE_NONE = 0xFF, // 상태 없음

	MAX_TOBJECT_STATE = 10
};


enum eTOBJECT_STATE_TYPE
{
	TOBJECT_STATE_TYPE_A,
	TOBJECT_STATE_TYPE_B,
	TOBJECT_STATE_TYPE_C,

	MAX_TOBJECT_STATE_TYPE,

	INVALID_TOBJECT_STATE_TYPE = 0xFF
};


enum eTOBJECT_STATE_TYPE_A
{
	TOBJECT_STATE_TYPE_A_CLOSED,
	TOBJECT_STATE_TYPE_A_OPEND,
	TOBJECT_STATE_TYPE_A_DESTROYED,

	MAX_TOBJECT_STATETYPE_A
};


enum eTOBJECT_STATE_TYPE_B
{
	TOBJECT_STATE_TYPE_B_CLOSING,
	TOBJECT_STATE_TYPE_B_CLOSED,
	TOBJECT_STATE_TYPE_B_OPENING,
	TOBJECT_STATE_TYPE_B_OPEND,
	TOBJECT_STATE_TYPE_B_DESTROYING,
	TOBJECT_STATE_TYPE_B_DESTROYED,

	MAX_TOBJECT_STATE_TYPE_B
};


enum eTOBJECT_STATE_TYPE_C
{
	TOBJECT_STATE_TYPE_C_NO_TEAM,
	TOBJECT_STATE_TYPE_C_ATTACK_TEAM,
	TOBJECT_STATE_TYPE_C_DEFENCE_TEAM,

	MAX_TOBJECT_STATE_TYPE_C
};


enum eTOBJECT_SUBSTATE
{
	TOBJECT_SUBSTATE_SHOW,
	TOBJECT_SUBSTATE_UNSELECT,

	MAX_TOBJECT_SUBSTATE,
};


enum eTOBJECT_SUBSTATE_FLAG
{
	TOBJECT_SUBSTATE_FLAG_SHOW = MAKE_BIT_FLAG( TOBJECT_SUBSTATE_SHOW ),
	TOBJECT_SUBSTATE_FLAG_UNSELECT = MAKE_BIT_FLAG( TOBJECT_SUBSTATE_UNSELECT ),
};


enum eTRIGGER_SERVER_EVENT_TYPE
{
	TRIGGER_SERVER_EVENT_TYPE_SPS,
	TRIGGER_SERVER_EVENT_TYPE_TQS,

	TRIGGER_SERVER_EVENT_TYPE_TMQ_START,
	TRIGGER_SERVER_EVENT_TYPE_TMQ_ARRIVE,
	TRIGGER_SERVER_EVENT_TYPE_TMQ_LEAVE,

	TRIGGER_SERVER_EVENT_TYPE_TS,

	TRIGGER_SERVER_EVENT_TYPE_WPS,

	TRIGGER_SERVER_EVENT_TYPE_INVALID = 0xffffffff
};


enum eTS_EVENT_SEND_TYPE
{
	TS_EVENT_SEND_TYPE_WORLD,				// 월드에 브로드캐스트
	TS_EVENT_SEND_TYPE_WORLD_AREA,			// 월드의 특정 지점에서부터 일정거리까지 브로드캐스트
	TS_EVENT_SEND_TYPE_PARTY,				// 모든 파티원 : 파티가 없을 경우 개인
	TS_EVENT_SEND_TYPE_PARTY_AREA,			// 일정 거리 안에 있는 파티원 : 파티가 없을 경우 개인.
	TS_EVENT_SEND_TYPE_PC,					// 개인
	TS_EVENT_SEND_TYPE_INVALID = 0xffffffff
};



//-----------------------------------------------------------------------------------
const BYTE		DBO_MAX_OBJECT_STATE = MAX_TOBJECT_STATE;
const BYTE		DBO_MAX_OBJECT_STATE_ANIMATION = 10;
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------


//===================================================================================
//
//===================================================================================
const unsigned int			NTL_QUEST_INVENTORY_WIDTH = 4;
const unsigned int			NTL_QUEST_TIMER_SLOT_MAX = 4;
const unsigned int			NTL_QUEST_INVENTORY_HEIGHT = 8;
const unsigned int			NTL_QUEST_EVENT_PARAM_MAX = 12;
const unsigned int			NTL_QUEST_INVENTORY_SLOT_COUNT = NTL_QUEST_INVENTORY_WIDTH * NTL_QUEST_INVENTORY_HEIGHT;
//===================================================================================



// 한번에 생성할 수 있는 퀘스트 아이템의 최대 종류
const int NTL_QUEST_ITEM_CREATE_MAX_COUNT = 5;

enum eQUEST_ITEM_UPDATE_TYPE
{
	eQUEST_ITEM_UPDATE_TYPE_CREATE = 0,	// 생성
	eQUEST_ITEM_UPDATE_TYPE_UPDATE,		// 업데이트
	eQUEST_ITEM_UPDATE_TYPE_DELETE		// 삭제
};


// Query Server 로 퀘스트 아이템 생성 및 삭제 요청 시 보낸다.
// res를 받았을 때 처리를 위해여 사용
enum eQUEST_ITEM_REQUEST_TYPE
{
	eQUEST_ITEM_REQUEST_TYPE_TS_CREATE,		// TS 에서 생성 요청	-> callback
	eQUEST_ITEM_REQUEST_TYPE_TS_DELETE,		// TS 에서 삭제 요청	-> callback
	eQUEST_ITEM_REQUEST_TYPE_MOB_DROP,		// 몹이 드랍			-> 없음
	eQUEST_ITEM_REQUEST_TYPE_USER_DELETE,	// 사용자 삭제 요청
	eQUEST_ITEM_REQUEST_TYPE_SERVER_EVENT,	// Server Event에서 have quest item 퀘스트 포기시 사용
	eQUEST_ITEM_REQUEST_TYPE_VISIT_EVENT,	// Visit event에서 사용

	eQUEST_ITEM_REQUEST_TYPE_INVALID = 0xFF
};


// 
enum eQUEST_DROP_PERMISSION_TYPE
{
	eQUEST_DROP_PERMISSION_TYPE_PERSONAL,
	eQUEST_DROP_PERMISSION_TYPE_PARTY,

	eQUEST_DROP_PERMISSION_TYPE_INVALID = 0xFF
};


// 에스코트 타입
enum eESCORT_TYPE
{
	ESCORT_TYPE_UNDER_ESCORT, // 호위당하고 있음
	ESCORT_TYPE_TARGET_FOLLOW, // 누군가를 따라가고 있음

	MAX_ESCORT_TYPE,
	INVALID_ESCORT_TYPE,
};


// Select reward type
#define INVALID_SELECT_REWARD_SLOT_INDEX	(0xffffffff)


//===================================================================================
//
//===================================================================================
#pragma pack(1)


struct sQUESTITEM_DATA
{
	TBLIDX			tblidx;
	BYTE			byCurStackCount;
	BYTE			byPos;				// 인벤토리에서의 아이템의 위치 by ju-hyeong
};


// Quesy Server ↔ Game Server 에서 사용
typedef sCOMPLETE_QUEST_INFO		sQUEST_COMPLETE_DATA;		// 퀘스트 완료 정보
typedef sPROGRESS_QUEST_INFO		sQUEST_PROGRESS_DATA;		// 퀘스트 진행 정보
typedef sQUESTITEM_DATA				sQUEST_INVENTORY_DATA;		// 퀘스트 인벤토리 

typedef sCOMPLETE_QUEST_TOTAL_BIT_INFO		sCOMPLETE_QUEST_TOTAL_BIT_DATA;		// 퀘스트 완료 정보

// 퀘스트 인벤토리 - 클라이언트에서 모든 퀘스트 아이템들을 저장하는 데 사용
struct sQUEST_INVENTORY_FULLDATA
{
	BYTE				byQuestItemCount;	// 아이템 개수
	sQUESTITEM_DATA		aQuestItemData[NTL_QUEST_INVENTORY_SLOT_COUNT];	// 아이템
};


// 서버내의 구조체와 서버 ↔ 클라이언트의 구조체를 같이 사용하므로 다음과 같이 typedef
typedef sCOMPLETE_QUEST_INFO	sQUEST_COMPLETE_INFO;
typedef sPROGRESS_QUEST_INFO	sQUEST_PROGRESS_INFO;
typedef sQUESTITEM_DATA			sQUEST_INVENTORY_INFO;



struct sTOBJECT_BRIEF
{
	TBLIDX		objectID; // = trigger object table index
};


struct sTOBJECT_STATE
{
	BYTE		byState; // Main State
	BYTE		bySubStateFlag; // Sub State Flag
	DWORD		dwStateTime; // 해당 State로 들어와서 지나온 시간	
};


#pragma pack()


#pragma pack( 1 )


// 퀘스트 아이템을 업데이트[생성,변경,삭제] 시 퀘스트 아이템 하나의 정보
struct sQUEST_ITEM_UPDATE_DATA
{
	BYTE		byUpdateType;	// eQUEST_ITEM_UPDATE_TYPE
	TBLIDX		itemTblidx;		// 퀘스트 아이템 index
	BYTE		byPos;			// 퀘스트 아이템의 위치
	BYTE		byCurCount;		// 퀘스트 아이템의 개수
};


// Trigger System Middle Serial
struct sTSM_SERIAL
{
	DWORD		dwKey;				// TSMiddle Serial
	DWORD		dwSub;				// TSMiddle SubSerial
};

//---------------------------------------------------------------------------------------
// Sub Data

//
struct sQUEST_ITEM_TS_SUB_DATA
{
	BYTE		byTSType;				// eTSM_MANAGER_TYPE
	HOBJECT		hObject;				// Trigger Object Handle : ObjectTrigger에서만 쓰인다.
	sTSM_SERIAL	sSerial;
};


// 퀘스트 아이템을 업데이트[생성,변경,삭제]시 보내어지는 서브데이터
// enum eQUEST_ITEM_REQUEST_TYPE 과 매치된다.
union uQUEST_ITEM_UPDATE_SUB_DATA
{
	sQUEST_ITEM_TS_SUB_DATA	sTSCreate;	// 퀘스트 아이템 생성
	sQUEST_ITEM_TS_SUB_DATA	sTSDelete;	// 퀘스트 아이템 삭제
	DWORD					dwReserve;	// 사용하지 않음 - 몹 사냥으로 인한 아이템 생성
};


//---------------------------------------------------------------------------------------


// Quest Server Event Update 시 사용하는 구조
union uSTOC_EVT_UPDATE_DATA
{
	// 몹 죽이기
	struct sMOB_KILL_CNT
	{
		int											nCurMobCnt;
	};

	// 몹 아이템 루팅 하기
	struct sMOB_KILL_ITEM_CNT
	{
		int											nCurMobLICnt;
	};

	// 아이템 배달하기
	struct sDELIVERY_ITEM
	{
		int											nCurItemCnt;
	};

	// 오브젝트 아이템
	struct sOBJECT_ITEM
	{
		int											nCurItemCnt;
	};

	// public 몹 아이템 루팅 - 몹 공통 드랍 이벤트
	struct sPUBLIC_MOB_ITEM_CNT
	{
		int											nCurItemCnt;
	};

	// Custom event counter
	struct sCUSTOM_EVT_CNT
	{
		int											nCurCnt;
	};

	struct sVISIT_EVT
	{
		bool										bCompleted;
	};


	sMOB_KILL_CNT				sMobKillCnt;
	sMOB_KILL_ITEM_CNT			sMobKillItemCnt;
	sDELIVERY_ITEM				sDeliveryItemCnt;
	sOBJECT_ITEM				sObjectItemCnt;
	sPUBLIC_MOB_ITEM_CNT		sPublicMobItemCnt;
	sCUSTOM_EVT_CNT				sCustomEvtCnt;
	sVISIT_EVT					sVisitEvt;
};

#pragma pack()
