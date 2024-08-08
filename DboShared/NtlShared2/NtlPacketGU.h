#pragma once

#include "NtlPacketCommon.h"

#include "NtlCharacter.h"
#include "NtlWorld.h"
#include "NtlSkill.h"
#include "NtlObject.h"
#include "NtlAvatar.h"
#include "NtlParty.h"
#include "NtlQuest.h"
#include "NtlGuild.h"
#include "NtlChat.h"
#include "NtlTimeQuest.h"
#include "NtlRankBattle.h"
#include "NtlMail.h"
#include "NtlCSArchitecture.h"
#include "NtlTeleport.h"
#include "NtlFriend.h"
#include "NtlBudokai.h"
#include "NtlMatch.h"
#include "NtlPartyDungeon.h"
#include "NtlPetition.h"
#include "NtlDojo.h"
#include "NtlHlsItem.h"
#include "NtlMovement.h"

enum eOPCODE_GU
{
	GU_OPCODE_BEGIN = 5000,

	GU_PONG = GU_OPCODE_BEGIN,
	GU_SESSION_STATE_WRONG, // 현재 세션 상태와 클라이언트의 요청이 맞지 않다.
	GU_CHAR_STATE_WRONG, // 현재 캐릭터 상태와 클라이언트의 요청이 맞지 않다.

	GU_GAME_ENTER_RES,
	GU_GAME_LEAVE_RES,
	GU_ENTER_WORLD_RES,
	GU_ENTER_WORLD_COMPLETE,
	GU_AUTH_KEY_FOR_COMMUNITY_SERVER_RES,

	GU_AVATAR_CHAR_INFO,
	GU_AVATAR_ITEM_INFO,
	GU_AVATAR_SKILL_INFO,
	GU_AVATAR_HTB_INFO,
	GU_AVATAR_BUFF_INFO,
	GU_AVATAR_WORLD_INFO,
	GU_AVATAR_ZONE_INFO,
	GU_AVATAR_QUEST_COMPLETE_INFO,	// 퀘스트 완료 정보
	GU_AVATAR_QUEST_PROGRESS_INFO,	// 퀘스트 진행 정보
	GU_AVATAR_QUEST_INVENTORY_INFO,	// 퀘스트 인벤 정보
	GU_AVATAR_INFO_END,
	GU_AVATAR_ATTRIBUTE_UPDATE,

	GU_OBJECT_CREATE,
	GU_OBJECT_DESTROY,

	GU_CHAR_MOVE, // 이동
	GU_CHAR_DEST_MOVE, // 목적지이동
	GU_CHAR_CHANGE_HEADING, // 방향 회전
	GU_CHAR_MOVE_SYNC, // 위치 보정
	GU_CHAR_CHANGE_JUMP_DIRECTION, // 캐릭터 점프 방향
	GU_CHAR_CHANGE_DIRECTION_ON_FLOATING,		// 캐릭터 점프/falling 때의 바라보는 방향
	GU_CHAR_FOLLOW_MOVE, // 따라가기
	GU_CHAR_JUMP, // 점프
	GU_CHAR_JUMP_END, // 점프 끝
	GU_CHAR_PUSHED_NFY,							// Push 발생
	GU_CHAR_FIGHTMODE, // 공격모드
	GU_CHAR_BLOCK_MODE_COOL_TIME_NFY, // Block 모드 쿨타임 count의 시작 알림
	//------------------------------------------------------------------
	//  [5/24/2006 zeroera] : 보완필요 : 좀더 효율적인 전송을 위해 상태 대신 다음의 메시지들로도 전송 가능하다
	// 현재는 상태 업데이트 메시지로 처리하므로 사용 하지 않는다.
	//------------------------------------------------------------------
	GU_CHAR_SITDOWN,
	GU_CHAR_STANDUP,
	GU_CHAR_CAMPING,
	GU_CHAR_FAINT,
	GU_CHAR_KNOCKDOWN,
	GU_CHAR_SLIDING,
	//------------------------------------------------------------------
	GU_CHAR_CONVERT_CLASS,
	GU_CHAR_DIALOG,									// 캐릭터 대화 출력
	GU_CHAR_DIRECT_PLAY,							// 캐릭터 연출
	GU_CHAR_DIRECT_PLAY_END,					// 캐릭터 연출 끝 ( sync가 설정되어 있는 경우에만 전송 )
	GU_CHAR_AWAY_RES,
	GU_CHAR_MAIL_INFO,	
	GU_CHAR_KEY_INFO,
	GU_CHAR_RANKBATTLE_SCORE,						// RankBattle Score
	GU_CHAR_KEY_UPDATE_RES,
	GU_CHAR_ACTION_ATTACK,
	GU_CHAR_ACTION_SKILL,
	GU_CHAR_ACTION_ITEM,
	GU_CHAR_REVIVAL_RES,
	GU_CHAR_TELEPORT_RES,
	GU_CHAR_BIND_RES,
	GU_CHAR_SERVER_CHANGE_RES,
	GU_CHAR_CHANNEL_CHANGE_RES,
	GU_CHAR_EXIT_RES,
	GU_CHAR_SKILL_RES,
	GU_GAME_EXIT_RES,
	GU_GAME_KICK_NFY,
	GU_EFFECT_AFFECTED,
	
	GU_SKILL_CASTING_CANCELED_NFY,
	GU_SKILL_CASTING_DELAYED_NFY,					// 스킬 캐스팅이 지연되었음을 알림
	GU_SKILL_AFFECTING_CANCELED_NFY,
	GU_SKILL_TARGET_LIST_REQ,
	GU_SKILL_LEARN_RES,
	GU_SKILL_LEARNED_NFY,
	GU_SKILL_UPGRADE_RES,
	GU_SKILL_RP_BONUS_SETTING_RES,
	GU_SKILL_NOTIFY_EXP,
	GU_SKILL_AFTEREFFECT_REMAINING_TIME_NFY,		// 변신 후유증 쿨타임 count의 시작 알림
	GU_SKILL_COOL_TIME_STARTED_NFY,					// 스킬의 cool time이 시작되었음을 알림

	GU_HTB_START_RES, // HTB 시작 응답
	GU_HTB_LEARN_RES, // HTB 습득 응답
	GU_HTB_FORWARD_RES, // HTB 진행 응답
	GU_HTB_RP_BALL_USE_RES,
	GU_HTB_RP_BALL_USED_NFY,
	GU_HTB_RP_BALL_RESULT_DECIDED_NFY,

	GU_BUFF_REGISTERED,
	GU_BUFF_DROPPED,
	GU_BUFF_REFRESH_ALL,
	GU_BUFF_DROP_RES,				// Buff 해제 요청에 대한 결과

	GU_ITEM_CREATE,
	GU_ITEM_UPDATE,
	GU_ITEM_MOVE_RES,
	GU_ITEM_MOVE_STACK_RES,
	GU_ITEM_DELETE,
	GU_ITEM_DELETE_RES,
	GU_ITEM_EQUIP_REPAIR_RES,			// 수리 완료 응답
	GU_ITEM_REPAIR_RES,
	GU_ITEM_USE_RES, // 아이템 사용 요청에 대한 결과
	GU_ITEM_CASTING_CANCELED,		// 아이템 캐스팅이 취소되었음을 알림
	GU_ITEM_CASTING_DELAYED_NFY,					// 아이템 캐스팅이 지연되었음을 알림
	GU_ITEM_DUR_DOWN,
	GU_ITEM_DUR_UPDATE,
	GU_ITEM_STACK_UPDATE,
	GU_ITEM_UPGRADE_RES,
	GU_ITEM_IDENTIFY_RES,

	GU_ITEM_PICK_RES,
	GU_ZENNY_PICK_RES,

	GU_UPDATE_CHAR_STATE,
	GU_UPDATE_CHAR_ASPECT_STATE,
	GU_UPDATE_CHAR_CONDITION,
	GU_UPDATE_CHAR_EXP,
	GU_UPDATE_CHAR_LEVEL,
	GU_UPDATE_CHAR_SPEED,
	GU_UPDATE_CHAR_ATTACK_SPEEDRATE,
	GU_UPDATE_ITEM_EQUIP,	// neighbor pc에만 전달한다.
	GU_UPDATE_ITEM_DELETED,	// 
	GU_UPDATE_TARGET_INFO,
	GU_UPDATE_CHAR_LP,
	GU_UPDATE_CHAR_EP,
	GU_UPDATE_CHAR_RP, 
	GU_UPDATE_CHAR_LP_EP,
	GU_UPDATE_CHAR_ZENNY,

	GU_UPDATE_CHAR_MUDOSA_POINT,

	GU_SYSTEM_DISPLAY_TEXT, // 텍스트 디스플레이
	GU_SYSTEM_DISPLAY_RESULTCODE,	// ResultCode 텍스트를 클라이언트에 출력한다.

	GU_SHOP_START_RES,
	GU_SHOP_BUY_RES,
	GU_SHOP_SELL_RES,
	GU_SHOP_END_RES,
	GU_SHOP_SKILL_BUY_RES,		// 스킬 구매 응답

	GU_PARTY_CREATE_RES,
	GU_PARTY_DISBAND_RES,
	GU_PARTY_DISBANDED_NFY,
	GU_PARTY_INVITE_RES,
	GU_PARTY_INVITE_NFY,
	GU_PARTY_RESPONSE_INVITATION_RES,
	GU_PARTY_MEMBER_JOINED_NFY,
	GU_PARTY_INFO,
	GU_PARTY_INVENTORY_INFO,
	GU_PARTY_OPENED_CHARM_INVENTORY_INFO,
	GU_PARTY_INVITATION_DECLINED_NFY,
	GU_PARTY_INVITATION_EXPIRED_NFY,
	GU_PARTY_LEAVE_RES,
	GU_PARTY_MEMBER_LEFT_NFY,
	GU_PARTY_KICK_OUT_RES,
	GU_PARTY_MEMBER_KICKED_OUT_NFY,
	GU_PARTY_CHANGE_LEADER_RES,
	GU_PARTY_LEADER_CHANGED_NFY,

	GU_PARTY_MEMBER_LEVELED_UP_NFY,
	GU_PARTY_MEMBER_CLASS_CHANGED_NFY,
	GU_PARTY_OPEN_CHARM_SLOT_RES,
	GU_PARTY_CHARM_SLOT_OPENED_NFY,
	GU_PARTY_CLOSE_CHARM_SLOT_RES,
	GU_PARTY_CHARM_SLOT_CLOSED_NFY,
	GU_PARTY_REGISTER_VICTIM_ITEM_RES,
	GU_PARTY_VICTIM_ITEM_REGISTERED_NFY,
	
	GU_PARTY_UNREGISTER_VICTIM_ITEM_RES,
	GU_PARTY_VICTIM_ITEM_UNREGISTERED_NFY,
	GU_PARTY_REGISTER_VICTIM_ZENNY_RES,
	GU_PARTY_VICTIM_ZENNY_REGISTERED_NFY,
	GU_PARTY_UNREGISTER_VICTIM_ZENNY_RES,
	GU_PARTY_VICTIM_ZENNY_UNREGISTERED_NFY,
	GU_PARTY_DECLARE_ZENNY_RES,
	GU_PARTY_DECLARED_ZENNY_CHANGED_NFY,
	GU_PARTY_CHARM_ACTIVATION_FAILED_NFY,
	GU_PARTY_ACTIVATE_CHARM_RES,
	GU_PARTY_CHARM_ACTIVATED_NFY,
	GU_PARTY_DEACTIVATE_CHARM_RES,
	GU_PARTY_CHARM_DEACTIVATED_NFY,
	GU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES,
	GU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY,
	GU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES,
	GU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY,
	GU_PARTY_ZENNY_LOOTING_METHOD_UNLOCKED_NFY,
	GU_PARTY_ITEM_LOOTING_METHOD_UNLOCKED_NFY,
	GU_PARTY_ZENNY_UPDATED_NFY,
	GU_PARTY_INVENTORY_ITEM_ADDED_NFY,
	GU_PARTY_INVENTORY_ITEM_REMOVED_NFY,
	GU_PARTY_MEMBER_GAINED_ITEM_NFY,
	GU_PARTY_MEMBER_GAINED_ZENNY_NFY,
	GU_PARTY_INVEST_ZENNY_RES,
	GU_PARTY_INVEST_ZENNY_NFY,
	GU_PARTY_INVENTORY_ITEM_WINNER_CHANGED_NFY,
	GU_PARTY_MEMBER_LOCATION_NFY,
	GU_PARTY_INVEN_ITEM_RANK_SET_RES,
	GU_PARTY_INVEN_ITEM_RANK_SET_NFY,

	GU_BANK_START_RES,
	GU_BANK_MOVE_RES,
	GU_BANK_MOVE_STACK_RES,
	GU_BANK_END_RES,
	GU_BANK_ZENNY_INFO,
	GU_BANK_ITEM_INFO,
	GU_BANK_ZENNY_RES,
	GU_BANK_BUY_RES,
	GU_BANK_ITEM_DELETE_RES,

	GU_SOCIAL_ACTION, // Social Action 사용

	GU_SCOUTER_INDICATOR_RES, // 스카우터 전투력측정 결과
	GU_SCOUTER_PREDICT_RES,
	GU_SCOUTER_EQUIP_CHECK_RES,
	GU_SCOUTER_CHIP_REMOVE_ALL_RES,

	GU_TS_CONFIRM_STEP_RES,							// 트리거 단계 진행 응답
	GU_TS_UPDATE_SYNC_NFY,							// 트리거 동기 진행 알림
	GU_TS_UPDATE_STATE,								// 트리거 실패 상태 업데이트
	GU_TS_UPDATE_EVENT_NFY,							// 트리거 이벤트 업데이트
	GU_TS_EXCUTE_TRIGGER,							// 클라이언트 트리거 강제 실행
	GU_TS_EXCUTE_TRIGGER_OBJECT_RES,				//
	GU_TS_TRIGGER_OBJECT_DIALOG_NFY,				// TriggerObject Conversation
	GU_TS_PC_DIALOG_NFY,							// PC Conversation
	GU_TS_SKIP_CONTAINER,

	GU_QUEST_ITEM_CREATE_NFY,						// 퀘스트 아이템 생성 알림
	GU_QUEST_ITEM_DELETE_NFY,						// 퀘스트 아이템 삭제 알림
	GU_QUEST_ITEM_UPDATE_NFY,						// 퀘스트 아이템 변경 알림
	GU_QUEST_ITEM_MOVE_RES,							// 퀘스트 아이템 이동 응답
	GU_QUEST_ITEM_DELETE_RES,						// 퀘스트 아이템 삭제 응답
	GU_QUEST_SVREVT_START_NFY,						// 퀘스트 이벤트 시작
	GU_QUEST_SVREVT_END_NFY,						// 퀘스트 이벤트 종료
	GU_QUEST_SVREVT_UPDATE_NFY,						// 퀘스트 이벤트 업데이트
	GU_QUEST_GIVEUP_RES,							// 퀘스트 포기 응답
	GU_QUEST_SHARE_NFY,								// 퀘스트 공유
	GU_QUEST_RESET_NFY,								// 퀘스트의 지정된 범위만큼 진행 정보와 완료 정보를 삭제한다.
	GU_QUEST_OBJECT_VISIT_RES,						// npc, tobject 방문 이벤트
	GU_QUEST_FORCED_COMPLETION_NFY,					// 퀘스트 강제 완료 설정(테스트용)

	GU_FREEBATTLE_CHALLENGE_RES,					// 프리배틀 대전 신청 응답
	GU_FREEBATTLE_ACCEPT_REQ,						// 프리배틀 대전수락 요청
	GU_FREEBATTLE_CANCEL_NFY,						// 프리배틀 대전 취소
	GU_FREEBATTLE_START_NFY,						// 프리배틀 대전 시작
	GU_FREEBATTLE_OUTSIDE_NFY,						// 프리배틀 대전범위 벗어남
	GU_FREEBATTLE_INSIDE_NFY,						// 프리배틀 대전범위 들어옴
	GU_FREEBATTLE_END_NFY,							// 프리배틀 대전 종료

	GU_QUICK_SLOT_UPDATE_RES,						// 퀵슬롯업데이트
	GU_QUICK_SLOT_DEL_NFY,							// 퀵슬롯 삭제	
	GU_QUICK_SLOT_INFO,								// QuickSlot Load

	GU_PET_CHAR_INFO,
	GU_PET_SKILL_INFO,
	GU_PET_INFO_END,
	GU_PET_DESPAWNED_NFY,							// 펫의 사라짐
	GU_PET_DISMISS_PET_RES,							// 펫의 소환 취소 요청 결과

	GU_ACTION_REPORT_MESSAGE,						// 표시하고 싶은 메세지를 리져트 코드 인덱스를 첨부하여 보내면 클라이언트는 표시한다.

	GU_TRADE_START_NFY,
	GU_TRADE_START_RES,
	GU_TRADE_OK_REQ,
	GU_TRADE_ADD_NFY,
	GU_TRADE_ADD_RES,
	GU_TRADE_DEL_NFY,
	GU_TRADE_DEL_RES,
	GU_TRADE_MODIFY_NFY,
	GU_TRADE_MODIFY_RES,
	GU_TRADE_ZENNY_UPDATE_NFY,
	GU_TRADE_ZENNY_UPDATE_RES,
	GU_TRADE_END_NFY,
	GU_TRADE_END_RES,
	GU_TRADE_CANCEL_NFY,
	GU_TRADE_CANCEL_RES,
	GU_TRADE_DENY_RES,

	GU_GUILD_NAME_CHANGED_NFY,					// 길드 이름 변경 알림
	GU_GUILD_CREATE_RES,						// 길드 생성 요청 결과
	GU_GUILD_INVITE_RES,						// 길드 초대 결과

	GU_TOBJECT_UPDATE_STATE,						// 트리거 오브젝트 상태 업데이트
	GU_TOBJECT_ACTION_ATTACK,						// 트리거 오브젝트 데미지
	
	GU_TUTORIAL_HINT_UPDATE_RES,					// 힌트 플래그 업데이트

	GU_TELECAST_MESSAGE_BEG_NFY,					// 방송 메시지 통지 시작
	GU_TELECAST_MESSAGE_END_NFY,					// 방송 메시지 통지 종료
	GU_BROAD_MESSAGE_BEG_NFY,						// 방송 메시지 시작
	GU_BROAD_MESSAGE_END_NFY,						// 방송 메시지 끝
	GU_MINI_NARRATION_NFY,							// 미니나레이션
	GU_PROGRESS_MESSAGE_NFY,						// 타임머신 진행 메시지 통지
	GU_DIRECTION_INDICATE_NFY,						// 방향지시 통지

	GU_TIMEQUEST_ROOM_LIST_RES,						// 타임머신 퀘스트 방 목록 요청 응답
	GU_TIMEQUEST_ROOM_INFO_RES,						// 타임머신 퀘스트 방 정보 요청 응답
	GU_TIMEQUEST_ROOM_JOIN_RES,						// 타임머신 퀘스트 방 진입 요청 응답
	GU_TIMEQUEST_ROOM_JOIN_NFY,						// 타임머신 퀘스트 방 진입을 파티원에게 알림(파티장 제외)
	GU_TIMEQUEST_ROOM_LEAVE_RES,					// 타임머신 퀘스트 방 퇴장 요청 응답
	GU_TIMEQUEST_ROOM_LEAVE_NFY,					// 타임머신 퀘스트 방 퇴장을 파티원에게 알림(파티장 제외)
	GU_TIMEQUEST_ROOM_SELECTION_NFY,				// 타임머신 퀘스트 추첨
	GU_TIMEQUEST_ROOM_TELEPORT_RES,					// 타임머신 퀘스트 시작 위치로 이동 응답
	GU_TIMEQUEST_START_NFY,							// 타임머신 퀘스트 시작 통지
	GU_TIMEQUEST_START_EXCEPTION_NFY,				// 타임머신 퀘스트 월드에 진입 실패함(성공시에는 전송되지 않는다.)
	GU_TIMEQUEST_LEAVE_NFY,							// 타임머신 퀘스트 퇴장 통지
	GU_TIMEQUEST_COUNTDOWN_NFY,						// 타임머신 퀘스트 카운트다운 통지
	GU_TIMEQUEST_REWARD_NFY,						// 타임머신 퀘스트 보상 통지
	GU_TIMEQUEST_INFORMATION_NFY,				// 타임머신 퀘스트 스테이지 정보 통지
	GU_TIMEQUEST_UPDATE_GAME_STATE,					// 타임머신 퀘스트 게임 상태 업데이트
	GU_TIMEQUEST_UPDATE_KILL_COUNT,					// 타임머신 퀘스트 플레이어 킬 카운트 업데이트
	GU_TIMEQUEST_UPDATE_COUPON_COUNT,				// 타임머신 퀘스트 쿠폰정보 업데이트
	GU_TIMEQUEST_UPDATE_TMQ_POINT,					// 타임머신 퀘스트 TMQ 포인트 업데이트
	GU_TIMEQUEST_UPDATE_GAME_TIME,					// 타임머신 퀘스트 게임 시간 업데이트

	GU_RANKBATTLE_INFO_RES,							// 기본 정보
	GU_RANKBATTLE_JOIN_RES,							// 신청
	GU_RANKBATTLE_JOIN_NFY,							// member 에게 신청 알림
	GU_RANKBATTLE_LEAVE_RES,						// 취소
	GU_RANKBATTLE_LEAVE_NFY,						// 취소 파티원 알림
	GU_RANKBATTLE_MATCH_START_NFY,					// 경기 시작
	GU_RANKBATTLE_MATCH_CANCELED_NFY,				// 경기가 취소됨

	GU_RANKBATTLE_ROOM_LIST_RES,					// RankBattle 방 목록 요청
	GU_RANKBATTLE_ROOM_INFO_RES,					// RankBattle 방 정보 요청
	GU_RANKBATTLE_ROOM_CREATE_RES,					// RankBattle 방 생성 요청
	GU_RANKBATTLE_ROOM_CREATE_NFY,					// RankBattle 방이 생성되었음을 파티원에게 알림
	GU_RANKBATTLE_ROOM_LEAVE_RES,					// RankBattle 방 삭제 요청
	GU_RANKBATTLE_ROOM_LEAVE_NFY,					// RankBattle 방이 삭제되었음을 파티원에게 알림
	GU_RANKBATTLE_ROOM_JOIN_INFO_NFY,				// Party에 들어갔을 경우 RankBattle에 대한 참여 정보를 전송
	GU_RANKBATTLE_ROOM_CHALLENGE_RES,				// RankBattle 대련 요청
	GU_RANKBATTLE_ROOM_CHALLENGE_NFY,				// RankBattle 대련 요청 결과를 파티원에게 알림
	GU_RANKBATTLE_ROOM_MATCH_READY_NFY,				// RankBattle이 곧 시작됨을 알림
	GU_RANKBATTLE_ROOM_MATCH_START_NFY,				// RankBattle이 시작됨 - 경기장으로 이동
	GU_RANKBATTLE_ROOM_MATCH_CANCEL_RES,			// RankBattle 대전을 취소함(개인)
	GU_RANKBATTLE_ROOM_MATCH_CANCELED_NFY,			// RankBattle 대전이 취소됨(owner 에게만 전송, 다시 대기상태로 변경)
	GU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY,			// RankBattle Battle State를 업데이트(경기장의 상태)
	GU_RANKBATTLE_BATTLE_TEAM_INFO_NFY,				// RankBattle Team 정보를 알림
	GU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY,			// RankBattle Player의 상태 알림(장외, 죽음등)
	GU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY,		// RankBattle KillScore 업데이트
	GU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY,			// RankBattle Stage Finish - Stage 마침
	GU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY,			// RankBattle Battle Finish - 경기 마침
	GU_RANKBATTLE_BATTLE_END_NFY,					// RankBattle이 끝날때 pc에게 알림(PC 마다 Battle이 끝나는 시점이 다르므로)
	GU_RANKBATTLE_BATTLE_FORCED_LEAVE_NFY,			// RankBattle 에서 강제적으로 WorldLeave 됨(너무 오랜 로딩 상태 등)
	GU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY,			// 개인 전적 업데이트

	GU_DRAGONBALL_START_RES,						// 제단클릭
	GU_DRAGONBALL_CHECK_RES,						// 드래곤볼 UI 7개 배치OK
	GU_DRAGONBALL_REWARD_RES,						// 드래곤볼 다 모음에 대한 보상 선택 요청
	GU_DRAGONBALL_COLLECT_NFY,						// 드래곤볼 스케쥴 타임 오버통보
	GU_DRAGONBALL_SCHEDULE_INFO,						// 드래곤볼 수집기간 정보

	GU_PRIVATESHOP_CREATE_RES,							// 개인상점을 생성 [6/28/2007 SGpro]
	GU_PRIVATESHOP_CREATE_NFY,							// 개인상점 예외 [1/31/2008 SGpro]
	GU_PRIVATESHOP_EXIT_RES,							// 개인상점을 닫는다[6/28/2007 SGpro]
	GU_PRIVATESHOP_OPEN_RES,							// 개인상점에 상태를 Open으로 해주세요[6/28/2007 SGpro]
	GU_PRIVATESHOP_OPEN_NFY,							// [7/31/2007 SGpro]
	GU_PRIVATESHOP_CLOSE_RES,							// 개인상점에 상태를 Close으로 해주세요[6/28/2007 SGpro]
	GU_PRIVATESHOP_CLOSE_NFY,							// [7/31/2007 SGpro]
	GU_PRIVATESHOP_ITEM_INSERT_RES,						// 개인상점에 아이템 추가 응답[6/29/2007 SGpro]
	GU_PRIVATESHOP_ITEM_UPDATE_RES,						// 개인상점에 아이템 수정 응답[6/29/2007 SGpro]
	GU_PRIVATESHOP_ITEM_DELETE_RES,						// 개인상점에 아이템 삭제 응답[6/29/2007 SGpro]
	GU_PRIVATESHOP_ITEM_BUYING_RES,						// 아이템 구매 응답[6/29/2007 SGpro]
	GU_PRIVATESHOP_ITEM_BUYING_NFY,						// [7/31/2007 SGpro]
	GU_PRIVATESHOP_ENTER_RES,							// 손님 입장 응답[6/29/2007 SGpro]
	GU_PRIVATESHOP_LEAVE_RES,							// 손님 퇴장 응답[6/29/2007 SGpro]
	GU_PRIVATESHOP_ITEM_DATA_INFO,						// 아이템 데이타 [7/2/2007 SGpro]
	GU_PRIVATESHOP_ITEM_SELECT_RES,						// [7/19/2007 SGpro]
	GU_PRIVATESHOP_ITEM_SELECT_NFY,						// [7/19/2007 SGpro]
	GU_PRIVATESHOP_ITEM_STATE_NFY,						// [7/31/2007 SGpro]
	GU_PRIVATESHOP_LEAVE_NFY,							// [8/3/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_REQUEST_RES,						// [10/17/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_REQUEST_NFY,						// [10/17/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_CONSENT_REQ,						// [10/17/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_CONSENT_NFY,						// [10/17/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_WAITTIMEOUT_NFY,					// [10/17/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_PRICEFLUCTUATIONS_RES,				// [10/17/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_PRICEFLUCTUATIONS_NFY,				// [10/17/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_IMMEDIATE_ITEM_BUYING_RES,			// [10/17/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_IMMEDIATE_ITEM_BUYING_NFY,			// [10/17/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_WINDOWCLOSE_NFY,					// [2/4/2008 SGpro]
	GU_PRIVATESHOP_BUSINESS_CANCEL_RES,							// [10/17/2007 SGpro]
	GU_PRIVATESHOP_BUSINESS_CANCEL_NFY,							// [10/17/2007 SGpro]

	GU_TUTORIAL_WAIT_NFY,								// 튜토리얼 대기 알림 
	GU_TUTORIAL_WAIT_CANCEL_RES,						// 튜토리얼 대기 취소 요청 응답
	GU_TUTORIAL_PLAY_QUIT_RES,							// 튜토리얼 플레이 취소 요청 응답
	
	GU_MAIL_START_RES,
	GU_MAIL_SEND_RES,
	GU_MAIL_READ_RES,
	GU_MAIL_RELOAD_NFY,
	GU_MAIL_DEL_RES,
	GU_MAIL_RETURN_RES,
	GU_MAIL_RELOAD_RES,
	GU_MAIL_LOAD_RES,
	GU_MAIL_LOAD_DATA,
	GU_MAIL_LOAD_INFO,
	GU_MAIL_ITEM_RECEIVE_RES,
	GU_MAIL_LOCK_RES,
	GU_MAIL_CLOSE_NFY,
	GU_MAIL_MULTI_DEL_RES,

	GU_PORTAL_START_RES,
	GU_PORTAL_INFO,
	GU_PORTAL_ADD_RES,
	GU_PORTAL_RES,

	GU_WAR_FOG_INFO,
	GU_WAR_FOG_UPDATE_RES,

	GU_MOB_LUCKY_DROP_NFY,

	GU_ITEM_UPGRADE_ALL_NFY,
	GU_BOT_BOTCAUTION_NFY,// Bot 경고 [2/20/2008 SGpro]
	GU_UPDATE_CHAR_REPUTATION,

	GU_GUILD_FUNCTION_ADD_RES,
	GU_GUILD_GIVE_ZENNY_RES,

	GU_GUILD_BANK_START_RES,
	GU_GUILD_BANK_MOVE_RES,
	GU_GUILD_BANK_MOVE_STACK_RES,
	GU_GUILD_BANK_END_RES,
	GU_GUILD_BANK_ZENNY_INFO,
	GU_GUILD_BANK_ITEM_INFO,
	GU_GUILD_BANK_ZENNY_RES,
	GU_GUILD_CREATE_MARK_RES,
	GU_GUILD_CHANGE_MARK_RES,
	GU_GUILD_MARK_CHANGED_NFY,

	GU_CROSSFIRE_RES,
	GU_BANK_LOAD_RES,
	
	GU_GUILD_CHANGE_NAME_RES,

	GU_PARTY_SHARETARGET_RES,						// [3/28/2008 SGpro]
	GU_PARTY_SHARETARGET_NFY,						// [3/28/2008 SGpro]

	GU_RIDE_ON_BUS_RES,
	GU_RIDE_OFF_BUS_RES,

	GU_TRANSFORM_CANCEL_RES,
	GU_SHOP_ITEM_IDENTIFY_RES,

	GU_MATCH_MINORMATCH_STATE_UPDATE_NFY,						// Match
	GU_MATCH_MINORMATCH_TEAM_INFO_NFY,				// 예선 팀 정보
	GU_MATCH_MINORMATCH_PLAYER_STATE_NFY,			//
	GU_MATCH_MINORMATCH_UPDATE_SCORE_NFY,			// 예선 Score Update
	GU_MATCH_MINORMATCH_TEAM_SCORE_NFY,				// 예선 팀 스코어 정보, 난입자를 위한 정보
	GU_MATCH_MINORMATCH_SELECTION_NFY,				// 예선 동점 발생시 주사위 연출
	GU_MATCH_MINORMATCH_STAGE_FINISH_NFY,			// 예선 stage finish 알림
	GU_MATCH_MINORMATCH_MATCH_FINISH_NFY,

	GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY,			//
	GU_MATCH_MAJORMATCH_TEAM_INFO_NFY,				// 본선 팀 정보
	GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY,			//
	GU_MATCH_MAJORMATCH_UPDATE_SCORE_NFY,			// 본선 Score Update
	GU_MATCH_MAJORMATCH_STAGE_FINISH_NFY,			// 본선 stage finish 알림
	GU_MATCH_MAJORMATCH_MATCH_FINISH_NFY,			// 본선 match finish 알림

	GU_MATCH_FINALMATCH_STATE_UPDATE_NFY,			//
	GU_MATCH_FINALMATCH_TEAM_INFO_NFY,				// 결선 팀 정보
	GU_MATCH_FINALMATCH_PLAYER_STATE_NFY,			//
	GU_MATCH_FINALMATCH_UPDATE_SCORE_NFY,			// 결선 Score Update
	GU_MATCH_FINALMATCH_STAGE_FINISH_NFY,			// 결선 stage finish 알림
	GU_MATCH_FINALMATCH_MATCH_FINISH_NFY,			// 결선 match finish 알림

	GU_MATCH_AWARDING_NFY,							// 시상식 결과

	GU_SKILL_CONFUSE_TARGET_NFY,					// Confuse 상태일 때의 공격 대상 알림
	GU_SKILL_TERROR_COURSE_CHANGE_NFY,				// Terror 상태일 때 방향 전환 알림

	GU_DICE_ROLL_RES,								// Action Skill 주사위 굴리기
	GU_DICE_ROLLED_NFY,								// Action Skill 주사위 굴리기

	GU_BUDOKAI_STATE_INFO_NFY,						// 천하제일무도회의 현재 상태를 전송
	GU_BUDOKAI_UPDATE_STATE_NFY,					// 천하제일무도회 : main state update
	GU_BUDOKAI_UPDATE_MATCH_STATE_NFY,				// 천하제일무도회 : match state update

	GU_BUDOKAI_JOIN_INDIVIDUAL_RES,					// 개인 등록
	GU_BUDOKAI_LEAVE_INDIVIDUAL_RES,				// 개인 취소

	GU_BUDOKAI_JOIN_TEAM_INFO_RES,					// 팀 포인트
	GU_BUDOKAI_JOIN_TEAM_RES,						// 팀 등록
	GU_BUDOKAI_JOIN_TEAM_NFY,						// 팀 등록 알림
	GU_BUDOKAI_LEAVE_TEAM_RES,						// 팀 등록 취소
	GU_BUDOKAI_LEAVE_TEAM_NFY,						// 팀 등록 취소 알림
	GU_BUDOKAI_LEAVE_TEAM_MEMBER_RES,				// 팀 멤버 취소
	GU_BUDOKAI_LEAVE_TEAM_MEMBER_NFY,				// 팀 멤버 취소 알림

	GU_BUDOKAI_JOIN_INFO_RES,						// 참가 정보
	GU_BUDOKAI_JOIN_STATE_RES,						// 참가 상태
	GU_BUDOKAI_JOIN_STATE_NFY,						// 참가 상태 알림
	GU_BUDOKAI_MUDOSA_INFO_RES,						// 무도사 정보
	GU_BUDOKAI_MUDOSA_TELEPORT_RES,					// 무도사 텔레포트
	GU_BUDOKAI_PROGRESS_MESSAGE_NFY,				// 경기 진행 정보 알림(공지)
	GU_BUDOKAI_SOCIAL_ACTION_NFY,
	GU_BUDOKAI_PRIZEWINNER_NAME_RES,				// 우승자 / 준우승자 : 개인전
//	GU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES,			// 우승자 / 준우승자 : 팀전

	GU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES,			// 천하제일무도회 서버로 텔레포트 요청
	GU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES,			// 천하제일무도회 서버에서 원래 서버로 이동
	GU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES,			// 현재 경기 진행 상황 리스트
	GU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES,			// 경기장 들어가기
	GU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES,			// 경기장 나가기

	GU_DISCONNECTED_NFY,

	GU_BOT_BOTCAUTION_HELPME_NFY,					// Help Me 경고 [5/27/2008 SGpro]

	GU_SCOUTER_TURN_ON_NFY,	
	GU_SCOUTER_TURN_OFF_NFY,
	GU_SCOUTER_BROKEN_NFY,
	GU_SCOUTER_ITEM_SELL_RES,

	GU_SHOP_EVENTITEM_START_RES,					// [7/15/2008 SGpro]
	GU_SHOP_EVENTITEM_BUY_RES,						// EVENT ITEM 구매 [7/11/2008 SGpro]
	GU_SHOP_EVENTITEM_END_RES,						// [7/15/2008 SGpro]

	GU_ITEM_REPLACE,								// ITEM 교체 [7/11/2008 SGpro]

	GU_SHOP_GAMBLE_BUY_RES,						// EVENT ITEM 구매 [7/11/2008 SGpro]

	GU_PARTY_DUNGEON_DIFF_NFY,
	GU_PARTY_DUNGEON_DIFF_RES,
	GU_PARTY_DUNGEON_INIT_NFY,
	GU_PARTY_DUNGEON_INIT_RES,

	GU_UPDATE_CHAR_SP,
	GU_SKILL_INIT_RES,
	GU_SKILL_REMOVE_NFY,
	GU_HTB_REMOVE_NFY,

	GU_CHAR_MARKING_NFY, // 칭호 [9/10/2008 SGpro]

	GU_PARTY_CHARM_DICE_START_NFY,					// 파티 다이스 UI 띄우고 시작하라
	GU_PARTY_CHARM_DICE_ROLLED_NFY,					// 선택된 다이스 결과 방송
	GU_PARTY_CHARM_DICE_ROLLED_RES,					// 선택된 다이스 결과
	GU_PARTY_CHARM_DICE_END_NFY,					// 파티 다이스 선택시간 완료 및 총결과

	GU_PARTY_INVEN_LOCK_NFY,
	GU_PARTY_INVEN_LOCK_RES,
	GU_PARTY_INVEN_RESET_RES,
	GU_PARTY_INVEN_RESET_NFY,
	GU_PARTY_INVEN_ACCEPTED_RES,
	GU_PARTY_INVEN_ACCEPTED_NFY,
	GU_PARTY_INVEN_GIVE_RES,
	GU_PARTY_INVEN_GIVE_NFY,
	GU_PARTY_INVEN_DICE_START_RES,
	GU_PARTY_INVEN_DICE_START_NFY,
	GU_PARTY_INVEN_DICE_ROLL_RES,
	GU_PARTY_INVEN_DICE_ROLL_NFY,
    GU_PARTY_INVEN_DICE_END_NFY,

	GU_TELEPORT_PROPOSAL_NFY,
	GU_TELEPORT_CONFIRM_RES,

	GU_AVATAR_PETITION_INFO,

	GU_GAME_COMMON_MSG_NFY,

	GU_CHAR_NICKNAME_CHANGED_NFY,

	GU_HOIPOIMIX_RECIPE_REG_NFY,					// 레시피등록
	GU_HOIPOIMIX_ITEM_CHECK_RES,					// 만들아이템 선택 (필요한 아이템 수량 체크)
	GU_HOIPOIMIX_ITEM_MAKE_RES,						// 아이템 만들기
	GU_HOIPOIMIX_ITEM_MAKE_EP_RES,					// 아이템 만들기 중 EP 넣기
	GU_HOIPOIMIX_ITEM_RECIPE_INFO,
	GU_DYNAMIC_OBJECT_UPDATE_STATE,
	GU_HOIPOIMIX_ITEM_MACHINE_MAKE_RES,
	GU_HOIPOIMIX_ITEM_MACHINE_DEL_RES,
	GU_HOIPOIMIX_ITEM_MAKE_EXP_NFY,					// 호이포이 믹스 레벨업이나 경험치 업데이트

	GU_CHAR_TARGET_LOST_NFY,
	GU_CHAR_GUARD_CRASHED_NFY,		// Guard가 Skill ability에 의해 강제로 해제됨

	GU_UPDATE_CHAR_RP_BALL,
	GU_UPDATE_CHAR_RP_BALL_MAX,

	GU_HOIPOIMIX_JOB_SET_RES,
	GU_HOIPOIMIX_JOB_RESET_RES,

	GU_AVATAR_RP_INCREASE_START_NFY,
	GU_AVATAR_RP_DECREASE_START_NFY,
	GU_AVATAR_RP_INCREASE_STOP_NFY,

	GU_HTB_LAST_STEP_CHANGED_NFY,

	GU_CHAR_CHARGE_CANCELED_NFY,
	GU_CHAR_REFRESH_LP_EP,

	GU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY,

	GU_VEHICLE_START_NFY,			// 탈 것 시작 위치 알림
	GU_VEHICLE_ENGINE_START_RES,			// 탈 것 시동 켬 RES
	GU_VEHICLE_ENGINE_START_NFY,			// 탈 것 시동 켬 브로드 캐스트
	GU_VEHICLE_ENGINE_STOP_RES,
	GU_VEHICLE_ENGINE_STOP_NFY,
	GU_VEHICLE_FUEL_UPDATE_NFY,				// 탈 것 연료 사용 업데이트
	GU_VEHICLE_END_RES,						// 탈 것 사용 종료 알림
	GU_VEHICLE_END_NFY,
	GU_VEHICLE_STUNT_NFY,					// 스페이스바 누름(탈것)

	/* 이번 마일스톤에는 1인승 작업만 하기 때문에 일단 주석처리 2009.1.5 승원.
	GU_VEHICLE_PASSENGER_GETON_RES,			// 탈 것 승객 탑승 RES
	GU_VEHICLE_PASSENGER_GETON_NFY,			// 탈 것 승객 탑승 브로드 캐스트
	GU_VEHICLE_ASK_REQ,						// 탈 것 운전자에게 타도 되는지 묻는다
	GU_VEHICLE_PASSENGER_GETOFF_RES,		// 탈 것 승객 내림 RES
	GU_VEHICLE_PASSENGER_GETOFF_NFY			// 탈 것 승객 내림 브로드 캐스트
	*/

	GU_SKILL_PASSIVE_EFFECT_APPLIED_NFY,
	GU_AVATAR_UPDATED_SKILL_DATA_INFO,

	GU_MONSTER_TRANSFORMED_NFY,

	GU_DOJO_CREATE_RES,
	GU_DOJO_DELETE_RES,
	GU_DOJO_FUNCTION_ADD_RES,
	GU_DOJO_BANK_HISTORY_RES,
	GU_DOGI_CREATE_RES,								// 도복생성
	GU_DOGI_CHANGE_RES,								// 도복변경
	GU_DOGI_UPDATE_NFY,								// 도복변경알림
	GU_GUILD_DOGI_CREATE_RES,
	GU_GUILD_DOGI_CHANGE_RES,

	GU_CHILD_ADULT_SWITCHED_NFY,
	GU_CHANGE_CLASS_AUTHORITY_CHANGED_NFY,

	GU_PLAY_BGM,

	GU_DOJO_MARK_CHANGED_NFY,
	GU_DOJO_LEVEL_CHANGED_NFY,

	GU_UPDATE_CHAR_NETP,

	GU_DOJO_SCRAMBLE_RES,							// 도장쟁탈전 신청에 대한 응답
	GU_DOJO_SCRAMBLE_RESPONSE_RES,					// 도장쟁탈전 신청응답 처리에 대한 응답

	GU_NETMARBLEMEMBERIP_NFY,

	GU_BUS_LOCATION_NFY,
	GU_BUS_LOCATION_ERASED_NFY,
	GU_BUS_LOCATION_RESET_ALL_NFY,

	GU_ITEM_EXPIRED_NFY,

	GU_SHOP_NETPYITEM_START_RES,
	GU_SHOP_NETPYITEM_BUY_RES,
	GU_SHOP_NETPYITEM_END_RES,

	GU_CHAR_REFRESH_BATTLE_ATTRIBUTE_MODIFIER,

	GU_CHAR_RANKBATTLE_ALLOW_NFY,
	GU_CHAR_RANKPOINT_RESET_NFY,

	GU_DOJO_NPC_INFO_RES,

	GU_GAMEGUARD_AUTH_ANSWER_REQ,

	GU_DOJO_SEAL_ATTACK_BEGIN_NFY,
	GU_DOJO_SEAL_ATTACK_END_NFY,
	GU_DOJO_SEAL_CURRENT_STATE_NFY,
	GU_INVENTORY_CAPSULE_UNLOCK_NFY,

	GU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY,
	
	GU_UPDATE_CHAR_LP_STATUS_NFY,

	GU_DURATION_ITEM_BUY_RES,
	GU_DURATION_RENEW_RES,

	GU_CASHITEM_START_RES,
	GU_CASHITEM_INFO_NFY,
	GU_CASHITEM_END_RES,
	GU_CASHITEM_MOVE_RES,
	GU_CASHITEM_USE_RES,
	GU_CASHITEM_ADD_NFY,
	GU_CASHITEM_DEL_NFY,
	GU_CASHITEM_HLSHOP_START_RES,
	GU_CASHITEM_HLSHOP_END_RES,
	GU_CASHITEM_BUY_RES,

	GU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES,			// 우승자 / 준우승자 : 팀전
	GU_GMT_INFO_NFY,								// GMT 시간을 알림
	GU_GMT_UPDATE_RES,							// GMT 재설정 응답
	GU_GMT_CHANGED_NFY,								// GMT 재설정 값을 적용 알림

	GU_UPDATE_CHAR_MAX_LP,
	GU_UPDATE_CHAR_MAX_EP,
	GU_UPDATE_CHAR_MAX_RP,

	GU_OPCODE_END_DUMMY,
	GU_OPCODE_END = GU_OPCODE_END_DUMMY - 1
};


//------------------------------------------------------------------
//
//------------------------------------------------------------------
const char * NtlGetPacketName_GU(WORD wOpCode);
//------------------------------------------------------------------


#pragma pack(push, 1)
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PONG)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SESSION_STATE_WRONG)
	WORD			wPrevOPCode;	// 이전에 보냈던 코드번호
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_STATE_WRONG)
	WORD			wPrevOPCode; // 이전에 보냈던 코드번호
	HOBJECT			handle; // 캐릭터 핸들
	BYTE			byStateID; // 현재 캐릭터 상태
	//sCHARSTATE		sCharState; // 현재 캐릭터 상태
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GAME_ENTER_RES)
	WORD			wResultCode;
	char			achCommunityServerIP[NTL_MAX_LENGTH_OF_IP + 1];
	WORD			wCommunityServerPort;
	DBOTIME			timeDBOEnter;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GAME_LEAVE_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ENTER_WORLD_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ENTER_WORLD_COMPLETE)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AUTH_KEY_FOR_COMMUNITY_SERVER_RES)
	WORD			wResultCode;
	BYTE			abyAuthKey[NTL_MAX_SIZE_AUTH_KEY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_CHAR_INFO)
	HOBJECT			handle;
	sPC_PROFILE		sPcProfile;
	WORD			wCharStateSize; //-[9/7/2006 zeroera] : 설명 : 클라이언트를 위한 캐릭터상태 사이즈 정보 
	sCHARSTATE		sCharState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_ITEM_INFO)
	BYTE			byBeginCount;
	BYTE			byItemCount;
	sITEM_PROFILE	aItemProfile[NTL_MAX_COUNT_USER_HAVE_INVEN_ITEM];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_SKILL_INFO)
	BYTE			bySkillCount;
	sSKILL_INFO		aSkillInfo[NTL_MAX_PC_HAVE_SKILL];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_HTB_INFO)
	BYTE			byHTBSkillCount; // HTB 스킬 개수
	sHTB_SKILL_INFO	aHTBSkillnfo[NTL_HTB_MAX_PC_HAVE_HTB_SKILL]; // HTB 스킬 정보
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_BUFF_INFO)
	BYTE			byBuffCount;
	sBUFF_INFO		aBuffInfo[NTL_MAX_BLESS_BUFF_CHARACTER_HAS + NTL_MAX_CURSE_BUFF_CHARACTER_HAS];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_WORLD_INFO)
	sVECTOR3		vCurLoc;
	sVECTOR3		vCurDir;
	sWORLD_INFO		worldInfo;
	BYTE			byDojoCount;
	sDBO_DOJO_BRIEF sDojoData[DBO_MAX_COUNT_DOJO_IN_WORLD];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_ZONE_INFO)
	sWORLD_ZONE_INFO		zoneInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_QUEST_COMPLETE_INFO)
	sQUEST_COMPLETE_INFO	completeInfo; // 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_QUEST_PROGRESS_INFO)
	BYTE					byProgressCount; // 현재 진행하고 있는 퀘스트 개수
	sQUEST_PROGRESS_INFO	progressInfo[1]; // 진행 퀘스트 데이타
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_QUEST_INVENTORY_INFO)
	BYTE					byItemCount;
	sQUEST_INVENTORY_INFO	aInventoryInfo[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_ATTRIBUTE_UPDATE)
	HOBJECT			hHandle;
	BYTE			byAttributeTotalCount;
	BYTE			abyFlexibleField[((UCHAR_MAX - 1) / 8 + 1) + sizeof(sAVATAR_ATTRIBUTE)];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_OBJECT_CREATE)
	HOBJECT			handle;
	sOBJECT_INFO	sObjectInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_OBJECT_DESTROY)
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_MOVE)
	HOBJECT			handle;
	//- yoshiki : Do we use dwTimeStamp or not?
	DWORD			dwTimeStamp;
	sVECTOR3		vCurLoc;
	sVECTOR3		vCurDir;
	BYTE			byMoveDirection;
	BYTE			byMoveFlag;		// 뛰기 혹은 걷기(ENtlMovementFlag 참조)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_DEST_MOVE)
	HOBJECT			handle;
	//- yoshiki : Do we use dwTimeStamp or not?
	DWORD			dwTimeStamp;
	sVECTOR3		vCurLoc;
	BYTE			byMoveFlag;		// 뛰기 혹은 걷기(ENtlMovementFlag 참조)
	bool			bHaveSecondDestLoc;
	sVECTOR3		vSecondDestLoc;
	BYTE			byDestLocCount;
	sVECTOR3		avDestLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_CHANGE_HEADING)
	HOBJECT			handle;
	//- yoshiki : Do we use dwTimeStamp or not?
	DWORD			dwTimeStamp;
	sVECTOR3		vNewHeading;
	sVECTOR3		vNewLoc;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_MOVE_SYNC)
	HOBJECT			handle;
	DWORD			dwTimeStamp;
	sVECTOR3		vCurLoc;
	sVECTOR3		vCurDir;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_CHANGE_JUMP_DIRECTION)
	HOBJECT			handle;
	//- yoshiki : Do we use dwTimeStamp or not?
	DWORD			dwTimeStamp;
	sVECTOR3		vJumpDir;
	//	BYTE			byMoveFlag;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_CHANGE_DIRECTION_ON_FLOATING)
	HOBJECT				hSubject;
	//- yoshiki : Do we use dwTimeStamp or not?
	DWORD				dwTimeStamp;
	sVECTOR3			vCurDir;
	BYTE				byMoveDirection;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_FOLLOW_MOVE)
	HOBJECT			handle;
	//- yoshiki : Do we use dwTimeStamp or not?
	DWORD			dwTimeStamp;
	HOBJECT			hTarget;
	float			fDistance;
	BYTE			byMovementReason;
	BYTE			byMoveFlag;		// 뛰기 혹은 걷기(ENtlMovementFlag 참조)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_JUMP)
	HOBJECT			handle;
	sVECTOR3		vCurrentHeading;
	sVECTOR3		vJumpDir;
	BYTE			byMoveDirection;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_JUMP_END)
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_PUSHED_NFY)
	HOBJECT			hSubject;
	BYTE			byAttackResult;		// eBATTLE_ATTACK_RESULT
	WORD			wAttackResultValue;
	sVECTOR3		vDestLoc;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_SITDOWN)
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_STANDUP)
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_FIGHTMODE)
	HOBJECT			handle;
	bool			bFightMode; // 전투 자세 On/Off
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_BLOCK_MODE_COOL_TIME_NFY)
	DWORD			dwCoolTimeRemaining;		// in Millisecs.
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_FAINT)
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_KNOCKDOWN)
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_SLIDING)
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_CAMPING)
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_CHAR_CONVERT_CLASS )
	HOBJECT			hTarget;
	BYTE			byClass;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_DIALOG)
	HOBJECT				hSubject;				// 대화 하는 캐릭터 핸들
	BYTE				byDialogType;			// 대화 종류 ( eCHAR_DIALOG_TYPE )
	TBLIDX				textTblidx;				// 대화 내용 테이블 번호 ( INVALID 가 아니면 해당 텍스트를 찾아 출력하고, INVALID면 아래의 텍스트를 출력해 줄 것)
	bool				bIsRefSpeechTable;		// speech table( NPCSpeechTable 을 참조하는가? )
	WORD				wTextLen;				// 대화내용의 길이
	WCHAR				awchText[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1]; // 대화내용
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_CHAR_DIRECT_PLAY )
	HOBJECT				hSubject;				// 연출 캐릭터 핸들
	BYTE				byPlayMode;				// 연출 모드
	TBLIDX				directTblidx;			// 연출 번호
	bool				bSynchronize;			// 서버 클라이언트 동기화
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_CHAR_DIRECT_PLAY_END )

END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_CHAR_AWAY_RES )
	bool	bIsAway;		// AwayOn 1: Awayoff: 0
	WORD 	wResultCode;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_CHAR_MAIL_INFO )
	BYTE	byMailCount;		// 현재 전체메일 카운트 
	BYTE	byManagerCount;		// 읽지 않은 매니져 메일 카운트
	BYTE	byNormalCount;		// 읽지 않은 노멀 메일 카운트
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_KEY_INFO)
	BYTE			byCount;
	sSHORTCUT_DATA  asData[NTL_SHORTCUT_MAX_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_RANKBATTLE_SCORE)
	sRANKBATTLE_SCORE_INFO	sScoreInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_KEY_UPDATE_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_ACTION_ATTACK)
	HOBJECT			hSubject; // 액션 주체 캐릭터 핸들
	HOBJECT			hTarget; // 액션 타겟 캐릭터 핸들
	DWORD			dwLpEpEventId;
	BYTE			bChainAttack:1; // 체인어택여부
	BYTE			byAttackResult:7; // 액션 결과 ( eBATTLE_ATTACK_RESULT )
	WORD			wAttackResultValue; // 액션의 결과 값 ( 데미지 )
	BYTE			byAttackSequence; // 어택 시퀀스 번호 (체인어택)
	float			fReflectedDamage; // 타겟으로부터 반사되어 입은 피해
	BYTE			byBlockedAction;		// eDBO_GUARD_TYPE
	sVECTOR3		vShift; // 어택으로 인한 이동 벡터
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_ACTION_SKILL)
	HOBJECT				handle;
	WORD				wResultCode;
	DWORD				dwLpEpEventId;
	TBLIDX				skillId;
	BYTE				byRpBonusType;		// eDBO_RP_BONUS_TYPE
	HOBJECT				hAppointedTarget;
	bool				bIsSkillHarmful;
	BYTE				bySkillResultCount;
	sSKILL_RESULT		aSkillResult[NTL_MAX_NUMBER_OF_SKILL_TARGET];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_ACTION_ITEM)
	HOBJECT				handle;
	WORD				wResultCode;
	DWORD				dwLpEpEventId;
	TBLIDX				itemTblidx;
	BYTE				bySkillResultCount;
	sSKILL_RESULT		aSkillResult[NTL_MAX_NUMBER_OF_SKILL_TARGET];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_REVIVAL_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_TELEPORT_RES)
	WORD			wResultCode;
	sVECTOR3		vNewLoc;
	sVECTOR3		vNewDir;
	bool			bIsToMoveAnotherServer;
	// Data in 'sWorldInfo' will not be sent if the character moves within the current world.
	// 현재 월드 안에서의 이동일 경우에는 sWorldInfo 데이타를 보내지 않는다.
	// by YOSHIKI(2007-10-25)
	sWORLD_INFO				sWorldInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_BIND_RES)
	WORD			wResultCode;
	BYTE			byBindType;		// eDBO_BIND_TYPE
	WORLDID			bindWorldId;
	TBLIDX			bindObjectTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_SERVER_CHANGE_RES)
	WORD					wResultCode;
	char					achAuthKey[NTL_MAX_SIZE_AUTH_KEY];
	sSERVER_INFO			serverInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_CHANNEL_CHANGE_RES)
	WORD					wResultCode;
	char					achAuthKey[NTL_MAX_SIZE_AUTH_KEY];
	sSERVER_INFO			serverInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_EXIT_RES)
	WORD					wResultCode;
	char					achAuthKey[NTL_MAX_SIZE_AUTH_KEY];
	BYTE					byServerInfoCount;
	sSERVER_INFO			aServerInfo[DBO_MAX_CHARACTER_SERVER_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_SKILL_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GAME_EXIT_RES)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GAME_KICK_NFY)
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_EFFECT_AFFECTED)
	HOBJECT			handle;
	HOBJECT			hCaster;
	TBLIDX			effectTblidx;
	WORD			wResultCode;
	float			fActualArgument1;
	float			fActualArgument2;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_CASTING_CANCELED_NFY)
	HOBJECT			handle;
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_CASTING_DELAYED_NFY)
	HOBJECT			handle;
	WORD			wResultCode;
	DWORD			dwCastingTime;
	DWORD			dwCastingTimeRemaining;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_AFFECTING_CANCELED_NFY)
	HOBJECT			hSubject;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_TARGET_LIST_REQ)
	BYTE			byAvatarType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_LEARN_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_LEARNED_NFY)
	TBLIDX			skillId;
	BYTE			bySlot;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_UPGRADE_RES)
	WORD			wResultCode;
	TBLIDX			skillId;
	BYTE			bySlot;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_RP_BONUS_SETTING_RES)
	WORD			wResultCode;
	TBLIDX			skillId;
	BYTE			bySlotIndex;
	BYTE			byRpBonusType;		// eDBO_RP_BONUS_TYPE
	bool			bIsRpBonusAuto;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_NOTIFY_EXP)
	BYTE			bySlotIndex;
	WORD			wSkillExp;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_AFTEREFFECT_REMAINING_TIME_NFY)
	DWORD			dwTimeRemaining;		// in Millisecs.
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_COOL_TIME_STARTED_NFY)
	HOBJECT			hSubject;
	TBLIDX			skillTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUFF_REGISTERED)
	HOBJECT			hHandle;
	BYTE			bySourceType;		// eDBO_OBJECT_SOURCE
	TBLIDX			tblidx;
	float			afEffectValue[NTL_MAX_EFFECT_IN_SKILL];
	DWORD			dwInitialDuration;		// In Millisecs.
	DWORD			dwTimeRemaining;		// in millisecs.
	bool			bNeedToDisplayMessage;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUFF_DROPPED)
	HOBJECT			hHandle;
	BYTE			bySourceType;		// eDBO_OBJECT_SOURCE
	TBLIDX			tblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUFF_REFRESH_ALL)
	HOBJECT			hHandle;
	BYTE			byCount;
	sBUFF_INFO		aBuffInfo[DBO_MAX_BUFF_CHARACTER_HAS];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUFF_DROP_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HTB_START_RES)
	WORD			wResultCode;
	BYTE			bySkillSlot;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HTB_LEARN_RES)
	WORD			wResultCode;
	TBLIDX			skillId;
	BYTE			bySkillSlot;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HTB_FORWARD_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HTB_RP_BALL_USE_RES)
	WORD			wResultCode;
	BYTE			byRpBallCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HTB_RP_BALL_USED_NFY)
	HOBJECT			hSubject;
	BYTE			byRpBallCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HTB_RP_BALL_RESULT_DECIDED_NFY)
	HOBJECT			hAttacker;
	HOBJECT			hWinner;
	bool			bHasSubjectRpBallUsed;
	bool			bHasTargetRpBallUsed;
	BYTE			bySubjectRpBallUsed;
	BYTE			byTargetRpBallUsed;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_CREATE)
	HOBJECT			handle;
	sITEM_DATA      sItemData;
	bool			bIsNew;		// Effect 추가 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_UPDATE)
	HOBJECT			handle;
	sITEM_DATA      sItemData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_MOVE_RES)
	WORD			wResultCode;
	HOBJECT			hSrcItem;
	HOBJECT			hDestItem;
	BYTE			bySrcPlace;
	BYTE			bySrcPos;
	BYTE			byDestPlace;
	BYTE			byDestPos;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_MOVE_STACK_RES)
	WORD			wResultCode;
	HOBJECT			hSrcItem;
	HOBJECT			hDestItem;
	BYTE			bySrcPlace;
	BYTE			bySrcPos;
	BYTE			byDestPlace;
	BYTE			byDestPos;	
	BYTE			byStackCount1;//결과적으로 놓여질값 SOURCE
	BYTE			byStackCount2;//결과적으로 놓여질값 DESTINATION
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_DELETE)
	HOBJECT			hSrcItem;
	BYTE			bySrcPlace;
	BYTE			bySrcPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_DELETE_RES)
	WORD			wResultCode;
	BYTE			byPlace;
	BYTE			byPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_EQUIP_REPAIR_RES)
	HOBJECT			handle;			// hNpchandle
	WORD			wResultCode;	
	DWORD			dwSpendedZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_REPAIR_RES)
	HOBJECT				hNpchandle;
	WORD				wResultCode;
	HOBJECT				hItem;
	DWORD				dwSpendedZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_USE_RES)
	WORD			wResultCode;
	TBLIDX			tblidxItem;
	BYTE			byPlace;
	BYTE			byPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_CASTING_CANCELED)
	HOBJECT			handle;
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_CASTING_DELAYED_NFY)
	HOBJECT			handle;
	WORD			wResultCode;
	DWORD			dwCastingTime;
	DWORD			dwCastingTimeRemaining;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_DUR_DOWN)
	BYTE			byDur[NTL_MAX_EQUIP_ITEM_SLOT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_DUR_UPDATE)
	HOBJECT			hItemHandle;
	BYTE			byDur;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_STACK_UPDATE)
	HOBJECT			hItemHandle;
	BYTE			byStack;
	bool			bIsNew;		// Effect 추가 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_UPGRADE_RES)
	WORD			wResultCode;
	BYTE			byStack;			// 스톤 사용후 스택갯수					-> 변경될 스택
	BYTE			byGrade;			// 변경될 그레이드						-> 변경될 그레이드
	BYTE			byItemPlace;		// 업그레이드 할 아이템의 컨테이너 위치
	BYTE			byItemPos;			// 업그레이드 할 아이템의 위치
	BYTE			byStonPlace;		// 호이포이스톤 컨테이너 위치
	BYTE			byStonPos;			// 호이포이스톤 위치
	BYTE			byBattleAttribute;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_IDENTIFY_RES)
	HOBJECT			hItemHandle;
	WORD			wResultCode;
	sITEM_DATA      sItemData;			// 성공시에만 세팅
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_PICK_RES)
	TBLIDX			itemTblidx;
	WORD			wResultCode;
	// Any message doesn't need to be displayed if 'bByPartyHunting' is true.
	// (If a party member picks up an item, the message is displayed by another type of packet.)
	// bByPartyHunting이 true이면 클라이언트에서 메세지를 출력할 필요가 없다.
	// (파티 상태에서 drop된 아이템을 얻는 경우에는 다른 packet에 의해 메세지가 출력된다.)
	bool			bByPartyHunting;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ZENNY_PICK_RES)
	WORD			wResultCode;
	bool			bSharedInParty;
	DWORD			dwZenny;
	DWORD			dwAcquisitionZenny;
	DWORD			dwBonusZenny;
	// 'dwOriginalZenny' MUST NOT be referred if 'bSharedInParty' is false.
	// bSharedInParty가 false이면 dwOriginalZenny값은 참조해서는 안 된다.
	// by YOSHIKI(2007-10-02)
	DWORD			dwOriginalZenny; //Drop Zenny이다.
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_STATE)
	HOBJECT			handle;
	sCHARSTATE		sCharState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_ASPECT_STATE)
	HOBJECT			handle;
	sASPECTSTATE	aspectState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_CONDITION)
	HOBJECT			handle;
	DWORD			dwConditionFlag;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_EXP)
	HOBJECT			handle;
	DWORD			dwCurExp;
	DWORD			dwIncreasedExp;
	DWORD			dwAcquisitionExp;
	DWORD			dwBonusExp;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_LEVEL)
	HOBJECT			handle;
	BYTE			byPrevLevel;
	BYTE			byCurLevel;
	DWORD			dwMaxExpInThisLevel;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_SPEED)
	HOBJECT			handle;
	float			fLastWalkingSpeed;
	float			fLastRunningSpeed;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_ATTACK_SPEEDRATE)
	HOBJECT			handle;
	WORD			wAttackSpeedRate;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_ITEM_EQUIP)
	HOBJECT			handle;
	BYTE			byPos;
	sITEM_BRIEF		sItemBrief;		// 장착 해제 되었을 경우 INVALID VALUE가 들어 온다.
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_ITEM_DELETED)
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_TARGET_INFO)
	HOBJECT			handle;
	WORD			wCurLP;
	WORD			wMaxLP;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_LP)
	HOBJECT			handle;
	DWORD			dwLpEpEventId;
	WORD			wCurLP;
	WORD			wMaxLP;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_EP)
	HOBJECT			handle;
	DWORD			dwLpEpEventId;
	WORD			wCurEP;
	WORD			wMaxEP;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_RP)
	HOBJECT			handle;
	WORD			wCurRP;
	WORD			wMaxRP;
	bool			bHitDelay;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_LP_EP)
	HOBJECT			handle;
	DWORD			dwLpEpEventId;
	WORD			wCurLP;
	WORD			wMaxLP;
	WORD			wCurEP;
	WORD			wMaxEP;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_ZENNY)
	HOBJECT			handle;
	DWORD			dwZenny;
	bool			bIsNew;		// Effect 추가
	BYTE			byChangeType; // eZENNY_CHANGE_TYPE - NtlCharacter.h
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_MUDOSA_POINT)
	DWORD			dwMudosaPoint;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SYSTEM_DISPLAY_TEXT)
	WCHAR			awGMChar[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	BYTE			byDisplayType;				// eSERVER_TEXT_TYPE
	WORD			wMessageLengthInUnicode;
	WCHAR			awchMessage[NTL_MAX_LENGTH_OF_CHAT_MESSAGE_UNICODE + 1]; // 내용
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SYSTEM_DISPLAY_RESULTCODE)
	BYTE			byDisplayType;				// eSERVER_TEXT_TYPE
	WORD			wResultCode;				// 출력할 ResultCode
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_START_RES)
	HOBJECT			handle;
	BYTE			byType; //	0: 기본  1:한정수량 판매 이벤트 NPC 
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_BUY_RES)
	HOBJECT			handle;
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_SELL_RES)
	HOBJECT			handle;
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_END_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_SKILL_BUY_RES)
	HOBJECT				hNpchandle;
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CREATE_RES)
	WORD			wResultCode;
	WCHAR			wszPartyName[NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_DISBAND_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_DISBANDED_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVITE_RES)
	WORD			wResultCode;
	WCHAR			wszTargetName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVITE_NFY)
	bool			bFromPc;
	union
	{
		WCHAR				wszInvitorPcName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
		WCHAR				wszInvitorPartyName[NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE + 1];
	};
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_RESPONSE_INVITATION_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_MEMBER_JOINED_NFY)
	sPARTY_MEMBER_INFO		memberInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INFO)
	WCHAR					awchPartyName[NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE + 1];
	HOBJECT					hLeader;
	BYTE					byItemLootingMethod;
	BYTE					byZennyLootingMethod;
	TBLIDX					registeredCharmTblidx[NTL_PARTY_CHARM_INVENTORY_COUNT];
	DWORD					adwRemainingTime[NTL_PARTY_CHARM_INVENTORY_COUNT];
	sSHARETARGET_INFO		sharetargetInfo[NTL_MAX_SHARETARGET_COUNT];// 공유타겟데이타 [3/28/2008 SGpro]
	ePARTY_DUNGEON_STATE	eDiff;
	BYTE					byItemRank;			// 파티인벤에 넣을 아이템의 eITEM_RANK 추가
	BYTE					byMemberInfoCount;
	sPARTY_MEMBER_INFO		memberInfo[NTL_MAX_MEMBER_IN_PARTY - 1];	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVENTORY_INFO)
	BYTE								bySlotInfoCount;
	sDBO_PARTY_INVENTORY_SLOT_INFO		inventorySlotInfo[NTL_PARTY_INVENTORY_SLOT_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_OPENED_CHARM_INVENTORY_INFO)
	BYTE							byCharmInventoryIndex;
	sDBO_PARTY_VICTIM_ITEM_INFO		victimItemInfo;
	DWORD							dwTotalVictimZenny;
	BYTE							byVictimInfoCount;
	sDBO_VICTIM_INFO				aVictimInfo[NTL_MAX_MEMBER_IN_PARTY - 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVITATION_DECLINED_NFY)
	WCHAR			wszPlayerName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVITATION_EXPIRED_NFY)
	WCHAR			wszPlayerName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_LEAVE_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_MEMBER_LEFT_NFY)
	HOBJECT			hMember;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_KICK_OUT_RES)
	WORD			wResultCode;
	HOBJECT			hTargetMember;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_MEMBER_KICKED_OUT_NFY)
	HOBJECT			hMember;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHANGE_LEADER_RES)
	WORD			wResultCode;
	HOBJECT			hNewLeader;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_LEADER_CHANGED_NFY)
	HOBJECT					hNewLeader;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_MEMBER_LEVELED_UP_NFY)
	HOBJECT			hMember;
	BYTE			byLevel;
	WORD			wMaxLP;
	WORD			wMaxEP;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_MEMBER_CLASS_CHANGED_NFY)
	HOBJECT			hMember;
	BYTE			byNewClass;		// ePC_CLASS
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_OPEN_CHARM_SLOT_RES)
	WORD			wResultCode;
	BYTE			byCharmInventoryIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHARM_SLOT_OPENED_NFY)
	BYTE			byCharmInventoryIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CLOSE_CHARM_SLOT_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHARM_SLOT_CLOSED_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_REGISTER_VICTIM_ITEM_RES)
	WORD			wResultCode;
	BYTE			bySrcPlace;
	BYTE			bySrcPos;
	BYTE			byDestSlotIndex;
	DWORD			dwTotalVictimItemScore;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_VICTIM_ITEM_REGISTERED_NFY)
	HOBJECT			hMember;
	sDBO_PARTY_VICTIM_ITEM_INFO		itemInfo;
	DWORD			dwTotalVictimItemScore;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_UNREGISTER_VICTIM_ITEM_RES)
	WORD			wResultCode;
	BYTE			byDestSlotIndex;
	DWORD			dwTotalVictimItemScore;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_VICTIM_ITEM_UNREGISTERED_NFY)
	HOBJECT			hMember;
	TBLIDX			itemTblidx;
	BYTE			byDestSlotIndex;
	DWORD			dwTotalVictimItemScore;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_REGISTER_VICTIM_ZENNY_RES)
	WORD			wResultCode;
	DWORD			dwRegisteredZenny;
	DWORD			dwReturningZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_VICTIM_ZENNY_REGISTERED_NFY)
	HOBJECT			hMember;
	DWORD			dwIncreasedZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_UNREGISTER_VICTIM_ZENNY_RES)
	WORD			wResultCode;
	DWORD			dwUnregisteredZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_VICTIM_ZENNY_UNREGISTERED_NFY)
	HOBJECT			hMember;
	DWORD			dwDecreasedZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_DECLARE_ZENNY_RES)
	WORD							wResultCode;

	DWORD							dwInitialZenny;
	DWORD							dwFinalDeclaredZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_DECLARED_ZENNY_CHANGED_NFY)
	HOBJECT							hMember;
	DWORD							dwDeclaredZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHARM_ACTIVATION_FAILED_NFY)
	WORD							wResultCode;
	HOBJECT							hMember;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_ACTIVATE_CHARM_RES)
	WORD			wResultCode;
	BYTE							byCharmInventoryIndex;
	TBLIDX							registeredCharmTblidx;
	DWORD							dwRemainingTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHARM_ACTIVATED_NFY)
	BYTE							byCharmInventoryIndex;
	TBLIDX							registeredCharmTblidx;
	DWORD							dwRemainingTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_DEACTIVATE_CHARM_RES)
	WORD							wResultCode;
	BYTE							byCharmInventoryIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHARM_DEACTIVATED_NFY)
	BYTE							byCharmInventoryIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHANGE_ZENNY_LOOTING_METHOD_RES)
	WORD							wResultCode;
	BYTE							byNewLootingMethod;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_ZENNY_LOOTING_METHOD_CHANGED_NFY)
	BYTE							byNewLootingMethod;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHANGE_ITEM_LOOTING_METHOD_RES)
	WORD							wResultCode;
	BYTE							byNewLootingMethod;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_ITEM_LOOTING_METHOD_CHANGED_NFY)
	BYTE							byNewLootingMethod;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_ZENNY_LOOTING_METHOD_UNLOCKED_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_ITEM_LOOTING_METHOD_UNLOCKED_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_ZENNY_UPDATED_NFY)
	BYTE byMemberCount;
	sINVEST_ZENNY_INFO aZennyInfo[NTL_MAX_MEMBER_IN_PARTY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVENTORY_ITEM_ADDED_NFY)
	BYTE							bySlotIndex;
	sPARTY_INVENTORY_ITEM_INFO		newLoot;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVENTORY_ITEM_REMOVED_NFY)
	BYTE					byPos;
	bool					bIsYourItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_MEMBER_GAINED_ITEM_NFY)
	HOBJECT					hMember;
	// If itemTblidx is INVALID_TBLIDX, this means an unidentified item.
	// itemTblidx가 INVALID_TBLIDX일 경우, 감정이 필요한 아이템임을 뜻한다.
	// by YOSHIKI(2006-11-02)
	TBLIDX					itemTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_MEMBER_GAINED_ZENNY_NFY)
	HOBJECT					hMember;
	bool					bIsShared;
	DWORD					dwZenny; //dwAcquisitionZenny + dwBonusZenny
	DWORD					dwAcquisitionZenny;
	DWORD					dwBonusZenny;

	// 'dwOriginalZenny' MUST NOT be referred if 'bSharedInParty' is false.
	// bSharedInParty가 false이면 dwOriginalZenny값은 참조해서는 안 된다.
	// by YOSHIKI(2007-10-02)
	DWORD					dwOriginalZenny; //Mob이 Drop한 Zenny
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEST_ZENNY_RES)
	WORD					wResultCode;
	BYTE					byPos;
	DWORD					dwInvestedZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEST_ZENNY_NFY)
	BYTE					byPos;
	DWORD					dwInvestedZenny;
	HOBJECT					hMember;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVENTORY_ITEM_WINNER_CHANGED_NFY)
	BYTE					byPos;
	HOBJECT					hWinner;
	DWORD					dwInvestedZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_MEMBER_LOCATION_NFY)
	HOBJECT					hMember;
	TBLIDX					worldTblidx;
	WORLDID					worldId;
	sVECTOR3				vCurLoc;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BANK_START_RES)
	HOBJECT			handle;			// Npc handle
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BANK_MOVE_RES)
	HOBJECT			handle;
	HOBJECT			hSrcItem;
	HOBJECT			hDstItem;
	BYTE			bySrcPlace;
	BYTE			bySrcPos;
	BYTE			byDestPlace;
	BYTE			byDestPos;
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BANK_MOVE_STACK_RES)
	HOBJECT			handle;
	HOBJECT			hSrcItem;
	HOBJECT			hDestItem;
	BYTE			bySrcPlace;
	BYTE			bySrcPos;
	BYTE			byDestPlace;
	BYTE			byDestPos;
	BYTE			byStackCount1;
	BYTE			byStackCount2;
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BANK_END_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BANK_ZENNY_INFO)
	DWORD			dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BANK_ITEM_INFO)
	BYTE			byBeginCount;
	BYTE			byItemCount;
	sITEM_PROFILE	aBankProfile[NTL_MAX_COUNT_USER_HAVE_BANK_ITEM];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BANK_ZENNY_RES)
	HOBJECT				handle;
	WORD				wResultCode;
	DWORD				dwZenny;
	bool				bIsSave;		// 1: 저금 0: 인출
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BANK_BUY_RES)
	HOBJECT				hNpchandle;
	WORD				wResultCode;
	HOBJECT				hItemhandle;
	sITEM_DATA			sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BANK_ITEM_DELETE_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SOCIAL_ACTION)
	HOBJECT			hSubject;
	TBLIDX			socialActionId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SCOUTER_INDICATOR_RES)
	HOBJECT			hTarget;
	WORD			wResultCode;
	DWORD			dwRetValue;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SCOUTER_PREDICT_RES)
	BYTE			byPusNum;
	WORD			wResultCode;
	BYTE			byRetRate;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SCOUTER_EQUIP_CHECK_RES)
	HOBJECT			hTarget;
	WORD			wResultCode;
	BYTE			byItemCount;
	sITEM_PROFILE	aItemProfile[NTL_MAX_EQUIP_ITEM_SLOT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SCOUTER_CHIP_REMOVE_ALL_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TS_CONFIRM_STEP_RES)
	BYTE				byTsType; // 트리거 타입
	WORD				wResultCode;
	NTL_TS_T_ID			tId;
	NTL_TS_TC_ID		tcCurId;
	NTL_TS_TC_ID		tcNextId;
	DWORD				dwParam;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TS_UPDATE_SYNC_NFY)
	BYTE				byTsType; // 트리거 타입
	NTL_TS_T_ID			tId;
	NTL_TS_TC_ID		tcId;
	NTL_TS_TA_ID		taId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TS_UPDATE_STATE)
	BYTE				byTsType; // 트리거 타입
	NTL_TS_T_ID			tId;
	BYTE				byType;
	WORD				wTSState;
	DWORD				dwParam;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TS_UPDATE_EVENT_NFY)
	BYTE				byTsType; // 트리거 타입
	NTL_TS_EVENT_ID		teid;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TS_EXCUTE_TRIGGER)
	BYTE				byTsType; // 트리거 타입
	NTL_TS_T_ID			tId; // 트리거 아이디
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TS_EXCUTE_TRIGGER_OBJECT_RES)
	WORD				wResultCode;
	HOBJECT				hTriggerObject;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TS_TRIGGER_OBJECT_DIALOG_NFY)
	HOBJECT				hTriggerObject;
	BYTE				byDialogType;		// eNPCCONV_TYPE
	TBLIDX				textTblidx;			// text
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TS_PC_DIALOG_NFY)
	TBLIDX				textTblidx;			// text
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TS_SKIP_CONTAINER)
	NTL_TS_T_ID			tId;
	NTL_TS_TC_ID		tcId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_ITEM_CREATE_NFY)
	BYTE				byPos;
	TBLIDX				qItemTblidx;
	BYTE				byCurCount;			// 현재 카운트
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_ITEM_DELETE_NFY)
	BYTE				byDeletePos;		// 삭제한 아이템의 위치
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_ITEM_UPDATE_NFY)
	BYTE				byPos;
	BYTE				byCurCount;			// 현재 카운트
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_ITEM_MOVE_RES)
	WORD				wResultCode;
	BYTE				bySrcPos;
	TBLIDX				dwSrcTblidx;
	BYTE				byDestPos;
	TBLIDX				dwDestTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_ITEM_DELETE_RES)
	WORD				wResultCode;
	BYTE				byDeletePos;		// 삭제한 아이템의 위치
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_SVREVT_START_NFY)
	NTL_TS_T_ID				tId;
	NTL_TS_TC_ID			tcId;
	NTL_TS_TA_ID			taId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_SVREVT_END_NFY)
	NTL_TS_T_ID				tId;
	NTL_TS_TC_ID			tcId;
	NTL_TS_TA_ID			taId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_SVREVT_UPDATE_NFY)
	NTL_TS_T_ID				tId;
	NTL_TS_TC_ID			tcId;
	NTL_TS_TA_ID			taId;
	BYTE					bySvrEvtType;		// eSTOC_EVT_DATA_TYPE
	BYTE					bySlot;
	uSTOC_EVT_UPDATE_DATA	uEvtData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_GIVEUP_RES)
	NTL_TS_T_ID			tId;
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_SHARE_NFY)				// 퀘스트 공유
	WORD				wResultCode;
	NTL_TS_T_ID			tId;
	HOBJECT				hActor;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_RESET_NFY)			// 퀘스트의 지정된 범위만큼 진행 정보와 완료 정보를 삭제한다.
	QUESTID				startResetQID;		// 범위 지정
	QUESTID				endResetQID;		// 범위 지정
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_OBJECT_VISIT_RES)
	WORD				wResultCode;

	QUESTID				qId;
	WORLDID				worldId;
	BYTE				byObjType;
	TBLIDX				objectTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUEST_FORCED_COMPLETION_NFY)
	QUESTID				questId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_FREEBATTLE_CHALLENGE_RES)
	HOBJECT				hTarget; // 대전을 요청했던 타겟의 핸들
	WORD				wResultCode; // 응답 코드
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_FREEBATTLE_ACCEPT_REQ)
	HOBJECT				hChallenger; // 대전을 요청한 캐릭터의 핸들
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_FREEBATTLE_CANCEL_NFY)
	WORD				wResultCode; // 대전취소에 대한 이유
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_FREEBATTLE_START_NFY)
	HOBJECT				hTarget; // 대전상대 핸들
	sVECTOR3			vRefreeLoc;	// 심판 위치
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_FREEBATTLE_OUTSIDE_NFY)

END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_FREEBATTLE_INSIDE_NFY)

END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_FREEBATTLE_END_NFY)
	BYTE				byFreeBattleResult; // eFREEBATTLE_RESULT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUICK_SLOT_UPDATE_RES)  
	WORD				wResultCode;
	TBLIDX				tblidx;
	BYTE				bySlotID;
	BYTE				byType;
	BYTE				byPlace;
	BYTE				byPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUICK_SLOT_DEL_NFY)  
	BYTE				bySlotID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_QUICK_SLOT_INFO)  
	BYTE				byQuickSlotCount;
	sQUICK_SLOT_PROFILE	asQuickSlotData[NTL_CHAR_QUICK_SLOT_MAX_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PET_CHAR_INFO)
	HOBJECT				handle;
	BYTE				byObjectType;		// eOBJTYPE
	union
	{
		sSUMMON_PET_PROFILE		sSummonPetProfile;
		sITEM_PET_PROFILE		sItemPetProfile;
	};
	WORD				wCharStateSize; //-[9/7/2006 zeroera] : 설명 : 클라이언트를 위한 캐릭터상태 사이즈 정보
	sCHARSTATE			sCharState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PET_SKILL_INFO)
	BYTE				bySkillCount;
	sSKILL_INFO_PET		aSkillInfo[NTL_MAX_NPC_HAVE_SKILL];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PET_INFO_END)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PET_DESPAWNED_NFY)
	BYTE				byAvatarType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PET_DISMISS_PET_RES)
	WORD				wResultCode;
	BYTE				byAvatarType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ACTION_REPORT_MESSAGE)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_START_NFY)
	HOBJECT				handle;			// 요청자 핸들
	HOBJECT				hTarget;		// 타겟 핸들  (자기 자신)
	WORD				wResultCode;	// 타겟의 응답및 가능 유무 에 따른 여러가지 결과값
	BYTE				byEmptyInven;	// 상대의 인벤의 빈공간	(만약 최대칸보다 작으면 비활성칸수 표시)	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_START_RES)
	HOBJECT				handle;			// 요청자 핸들 (자기 자신)
	HOBJECT				hTarget;		// 타겟 핸들
	WORD				wResultCode;	// 타겟의 응답및 가능 유무 에 따른 여러가지 결과값
	BYTE				byEmptyInven;	// 상대의 인벤의 빈공간	(만약 최대칸보다 작으면 비활성칸수 표시)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_OK_REQ)
	HOBJECT				handle;			// 요청자의 핸들
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_ADD_NFY)			// 성공일 때만
	//HOBJECT				hTarget;			// 타겟 핸들
	HOBJECT				hItem;				// 상대 아이템 핸들
	sITEM_DATA			sItem;				// 상대가 올려 놓을 아이템의 정보
	BYTE				byCount;			// 겹쳐있는 아이템중 몇개인가? Default 1개
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_ADD_RES)
	//HOBJECT				hTarget;			// 타겟 핸들
	HOBJECT				hItem;				// 내 아이템 핸들
	BYTE				byCount;			// 겹쳐있는 아이템중 몇개인가? Default 1개
	WORD				wResultCode;		// 타겟의 응답및 가능 유무 에 따른 여러가지 결과값
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_DEL_NFY)			// 성공일 때만
	//HOBJECT				hTarget;			// 타겟 핸들
	HOBJECT				hItem;				// 상대 아이템 핸들
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_DEL_RES)
	//HOBJECT				hTarget;			// 타겟 핸들
	HOBJECT				hItem;				// 내 아이템 핸들
	WORD				wResultCode;		// 타겟의 응답및 가능 유무 에 따른 여러가지 결과값
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_MODIFY_NFY)
	HOBJECT				hTarget;			// 타겟 핸들
	HOBJECT				hItem;				// 내 아이템 핸들
	BYTE				byCount;			// 겹쳐있는 아이템중 몇개인가? Default 1개
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_MODIFY_RES)
	HOBJECT				hTarget;			// 타겟 핸들
	HOBJECT				hItem;				// 내 아이템 핸들
	BYTE				byCount;			// 겹쳐있는 아이템중 몇개인가? Default 1개
	WORD				wResultCode;		// 타겟의 응답및 가능 유무 에 따른 여러가지 결과값
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_ZENNY_UPDATE_NFY)	// 성공일 때만
	HOBJECT				hTarget;			// 타겟 핸들
	DWORD				dwZenny;			// 옮길 만큼의 제니 (자기가 갖고 있는 제니 한도 내)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_ZENNY_UPDATE_RES)
	HOBJECT				hTarget;			// 타겟 핸들
	DWORD				dwZenny;			// 옮길 만큼의 제니 (자기가 갖고 있는 제니 한도 내 상대의 제니가 오버 되면 자동 실패)
	WORD				wResultCode;		// 타겟의 응답및 가능 유무 에 따른 여러가지 결과값
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_END_NFY)
	HOBJECT				hTarget;			// 타겟 핸들
	WORD				wResultCode;		// 타겟의 응답및 가능 유무 에 따른 여러가지 결과값
	bool				bIsSet;				// false: 풀림 true: 잠김
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_END_RES)
	HOBJECT				hTarget;			// 타겟 핸들
	DWORD				dwPacketCount;		// 상대방의 변경 카운트 ( 상대방이 물건을 올려놓거나 삭제 혹은 변경 등 패킷받은 개수 )
	WORD				wResultCode;		// 타겟의 응답및 가능 유무 에 따른 여러가지 결과값
	bool				bIsSet;				// false: 풀림 true: 잠김
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_CANCEL_NFY)
	HOBJECT				hTarget;			// 타겟 핸들
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_CANCEL_RES)
	HOBJECT				hTarget;			// 타겟 핸들
	WORD				wResultCode;		// 타겟의 응답및 가능 유무 에 따른 여러가지 결과값
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRADE_DENY_RES)
	bool				bIsDeny;			// 거절 프레그 
	WORD				wResultCode;		// 만약 교환중이라면 실패한다. 그외 성공 및 결과
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_NAME_CHANGED_NFY)
	HOBJECT				hSubject;
	WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_CREATE_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_INVITE_RES)
	WORD				wResultCode;
	WCHAR				wszTargetName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TOBJECT_UPDATE_STATE)
	HOBJECT				handle;
	sTOBJECT_BRIEF		tobjectBrief;
	sTOBJECT_STATE		tobjectState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TOBJECT_ACTION_ATTACK)
	HOBJECT				hSubject;				// 액션 주체 캐릭터 핸들
	HOBJECT				hTarget;				// 액션 타겟 캐릭터 핸들
	BYTE				byAttackType;			// 액션 결과 ( eBATTLE_ATTACK_RESULT )
	BYTE				byAttackResult;			// 액션 결과 ( eBATTLE_ATTACK_RESULT )
	WORD				wAttackResultValue;		// 액션의 결과 값 ( 데미지 )
	sVECTOR3			vShift;					// 어택으로 인한 이동 벡터
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TUTORIAL_HINT_UPDATE_RES)
	WORD				wResultCode;
	DWORD				dwTutorialHint;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_ROOM_LIST_RES )
	WORD					wResultCode;			// 응답코드
	sTIMEQUEST_ROOM_TMQ_INFO	sTMQInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_ROOM_INFO_RES )
	WORD					wResultCode;			// 응답코드
	BYTE						byRoomState;			// eTIMEQUEST_ROOM_STATE
	uTIMEQUEST_ROOM_ICON_INFO	sIconInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_ROOM_JOIN_RES )
	WORD					wResultCode;			// 응답코드
	sTIMEQUEST_ROOM_JOIN_INFO	sJoinInfo;
	HOBJECT						hTroubleMember;			// join이 실패한 pc의 핸들
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_ROOM_JOIN_NFY )
	sTIMEQUEST_ROOM_JOIN_INFO	sJoinInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_ROOM_LEAVE_RES )
	WORD					wResultCode;			// 응답코드
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_ROOM_LEAVE_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_ROOM_SELECTION_NFY )
	bool							bIsSecondWinner;
	uTIMEQUEST_ROOM_SELECTION_INFO	uSelectionInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_ROOM_TELEPORT_RES )
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_START_NFY )			
	WORD					wResultCode;			// 결과 코드 ( 실패 할 수도 있음, 반드시 확인할 것)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_START_EXCEPTION_NFY )			
	WORD					wResultCode;			// 결과 코드
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_LEAVE_NFY )
	HOBJECT					handle;					// 퇴장한 멤버의 핸들
	WORD					wResultCode;			// 이유를 나타내는 응답 코드
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_COUNTDOWN_NFY )
	bool					bCountDown;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_REWARD_NFY )
	BYTE					byTriggerType;			// 보상 관련 ts타입 정보
	NTL_TS_EVENT_ID			teid;					// 보상 관련 ts이벤트 정보
	DWORD					dwWaitTime;				// Reward Wait 시간
	DWORD					dwClearTime;			// 클리어 시간
	DWORD					dwBonusTime;			// 보너스 시간
	bool					bIsDayRecord : 1;			// DayRecord를 세웠을 경우 true
	bool					bIsBestRecord : 1;			// 베스트 레코드를 세웠을 경우에는 true
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_INFORMATION_NFY )
	BYTE					byInfoType;				// 정보 타입 eTIMEQUEST_INFORMATION_TYPE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TELECAST_MESSAGE_BEG_NFY )
	TBLIDX					npcTblidx;				// 방송을 하는 NPC 테이블 인덱스
	BYTE					byTelecastType;			// 방송 타입 eTELECAST_MESSAGE_TYPE
	TBLIDX					speechTblidx;			// 방송내용을 담은 Speech 테이블 인덱스
	DWORD					dwDisplayTime;			// 방송시간을 나타내는 millisec ( INVALID_DWORD 이면 telecast를 계속 띄우고, 아니면 이 시간이 지나면 없앨 것 )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TELECAST_MESSAGE_END_NFY )

END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_BROAD_MESSAGE_BEG_NFY )
	BYTE byUIDirType;			// eBROAD_MSG_UI_DIR_TYPE
	BYTE byUIShowHideTypel;		// eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE
	BYTE byUIBalloonShapeType;	// eBROAD_MSG_UI_BALLOON_SHAPE_TYPE
	BYTE byUISpeechDirType;		// eBROAD_MSG_UI_SPEECH_DIR_TYPE
	BYTE byOwnerCondition;		// eBROAD_MSG_OWNER_CONDITION
	BYTE byOwnerType;			// eBROAD_MSG_OWNER_TYPE
	unsigned int uiOwnerTblIdx;
	unsigned int uiQuestTextTblidx;
	float fDisplayTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_BROAD_MESSAGE_END_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MINI_NARRATION_NFY )
	unsigned int uiNarrtionTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PROGRESS_MESSAGE_NFY )
	BYTE					byProgressType;			// 진행 타입 ePROGRESS_MESSAGE_TYPE
	BYTE					byMessageType;			// 메시지 타입 eTIMEQUEST_MESSAGE_TYPE
	BYTE					byMessageValue;			// 메시지의 값 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_DIRECTION_INDICATE_NFY )
	bool					bIndicate;				// true : Arrow On, false : Arrow Off
	sVECTOR3				vLoc;					// 지정위치 ( Off 일경우 Access하지 말것 )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_UPDATE_GAME_STATE )
	sTIMEQUEST_STATE		sTimeQuestState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_UPDATE_KILL_COUNT )
	HOBJECT					handle;					// 타임 퀘스트 중에 기절한 
	BYTE					byKillCount;			// 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_UPDATE_COUPON_COUNT )
	BYTE					byCouponCount;			// 타임 퀘스트 쿠폰 개수
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_UPDATE_TMQ_POINT )
	WORD					wTmqPoint;				// 타임 퀘스트 TMQ 포인트
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TIMEQUEST_UPDATE_GAME_TIME )
	DWORD					dwClearTime;			// 타임 퀘스트 클리어 시간 ( 증가되는 시간 )
	DWORD					dwBonusTime;			// 타임 퀘스트 보너스 시간
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_INFO_RES )
	WORD					wResultCode;

	BYTE					byBattleMode;		// eRANKBATTLE_MODE
	DWORD					dwRemainTime;		// 남은 시간
	BYTE					byArenaCount;
	sRANKBATTLE_ARENA_INFO	asArenaInfo[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_JOIN_RES )
	WORD					wResultCode;

	HOBJECT					hBoardObject;
	TBLIDX					rankBattleTblidx;
	DWORD					dwRemainTime;		// 남은 시간
	BYTE					byArenaCount;
	sRANKBATTLE_ARENA_INFO	asArenaInfo[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_JOIN_NFY )
	TBLIDX					rankBattleTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_LEAVE_RES )
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_LEAVE_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_MATCH_START_NFY )
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_MATCH_CANCELED_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_LIST_RES )
	WORD					wResultCode;

	TBLIDX					rankBattleTblidx;	// RankBattle Tblidx

	WORD					wTotalPage;			// 게시판 총 페이지
	WORD					wCurPage;			// 게시판 출력 Page

	BYTE					byRoomMode;			// RankBattle 모드 : eRANKBATTLE_MODE
	BYTE					byRoomCount;		// 전송하는 Room의 갯수

	uRANKBATTLE_ROOMINFO_LIST	uRoomList;		// Room List
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_INFO_RES )
	WORD						wResultCode;

	// 패킷 사이즈 계산에 주의한다.
	// if( RANKBATTLE_MODE_INDIVIDUAL == byBattleMode )
	// {
	//		wMsgSize = sizeof(sGU_RANKBATTLE_ROOM_INFO_RES) - sizeof(uRANKBATTLE_ROOMINFO) + sizeof(sRANKBATTLE_ROOMINFO_INDIVIDUAL);
	// }
	// else if (RANKBATTLE_MODE_PARTY == byBattleMode )
	// {
	//		wMsgSize = sizeof(sGU_RANKBATTLE_ROOM_INFO_RES) - sizeof(uRANKBATTLE_ROOMINFO) + sizeof(sRANKBATTLE_ROOMINFO_PARTY) + sizeof(sRANKBATTLE_ROOMINFO_MEMBER) * (msg->uRoom.sParty.byMemberCount - 1);
	// }

	BYTE						byRoomMode;		// eRANKBATTLE_MODE
	uRANKBATTLE_ROOMINFO		uRoom;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_CREATE_RES )
	WORD						wResultCode;
	TBLIDX						rankBattleTblidx;	// RankBattle Tblidx
	ROOMID						roomId;
	BYTE						byTroubleCount;		// 문제를 가지는 멤버의 수
	sRANKBATTLE_MEMBER_RESULT	asTroublePc[1];		// 문제를 가지는 멤버 정보
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_CREATE_NFY )
	WORD				wResultCode;
	TBLIDX				rankBattleTblidx;	// RankBattle Tblidx
	ROOMID				roomId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_LEAVE_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_LEAVE_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_JOIN_INFO_NFY )
	TBLIDX				rankBattleTblidx;	// RankBattle Tblidx
	ROOMID				roomId;
	BYTE				byTeamType;			// eRANKBATTLE_TEAM_TYPE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_CHALLENGE_RES )
	WORD						wResultCode;
	TBLIDX						rankBattleTblidx;	// RankBattle Tblidx
	ROOMID						roomId;				// 방 번호
	BYTE						byTroubleCount;		// 문제를 가지는 멤버의 수
	sRANKBATTLE_MEMBER_RESULT	asTroublePc[1];		// 문제를 가지는 멤버 정보
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_CHALLENGE_NFY )
	WORD				wResultCode;
	TBLIDX				rankBattleTblidx;	// RankBattle Tblidx
	ROOMID				roomId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_MATCH_READY_NFY )
	WORD						wResultCode;
	BYTE						byMemberCount;		// 멤버 수
	sRANKBATTLE_MEMBER_RESULT	asMemberResult[1];	// 멤버 정보
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_MATCH_START_NFY )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_MATCH_CANCEL_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_ROOM_MATCH_CANCELED_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_BATTLE_STATE_UPDATE_NFY )
	BYTE				byBattleState;
	BYTE				byStage;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_BATTLE_TEAM_INFO_NFY )
	WORD				wStraightWinCount;		// 연승 카운트
	WORD				wStraightKOWinCount;	// KO 연승 카운트
	BYTE				byCount;				// 인원
	WORD				wMemberInfo;			// sRANKBATTLE_MATCH_MEMBER_INFO [byCount]
	WORD				wOwnerPartyName;		// WCHAR [NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE]
	WORD				wChallengerPartyName;	// WCHAR [NTL_MAX_SIZE_PARTY_NAME_IN_UNICODE]
	sVARIABLE_DATA		sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_BATTLE_PLAYER_STATE_NFY )
	HOBJECT				hPc;
	BYTE				byPCState;	// eRANKBATTLE_MEMBER_STATE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_BATTLE_KILLSCORE_UPDATE_NFY )
	BYTE				byOwnerKillScore;
	BYTE				byChallengerKillScore;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_BATTLE_STAGE_FINISH_NFY )
	BYTE				byBattleResult;			// eRANKBATTLE_MATCH_RESULT
	sRANKBATTLE_SCORE	sStageScore;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_BATTLE_MATCH_FINISH_NFY )
	sRANKBATTLE_MATCH_FINISH_INFO	sMatchResult;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_BATTLE_END_NFY )
	BYTE					byMatchWinner;		// eRANKBATTLE_MATCH_RESULT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_BATTLE_FORCED_LEAVE_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_RANKBATTLE_TOTAL_SCORE_UPDATE_NFY )
	BYTE					byBattlemode;
	sRANKBATTLE_SCORE_INFO	sTotalScore;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_DRAGONBALL_CHECK_RES )// 드래곤볼 UI 7개 배치OK
	HOBJECT				hObject;			// TS Object 제단	
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_DRAGONBALL_REWARD_RES )// 드래곤볼 다 모음에 대한 보상 선택 요청
	HOBJECT				hObject;			// TS Object 제단
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_DRAGONBALL_COLLECT_NFY ) // 서버의 일방적인 통보
	HOBJECT				hObject;			// TS Object 제단
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_DRAGONBALL_SCHEDULE_INFO )
	bool						bIsAlive;			// 기간 유무 false: 기간이 아니다
	BYTE						byEventType;		// eSCHEDULE_EVENT_TYPE
	__int64						nStartTime;			// 최초기간 시작
	__int64						nEndTime;           // 마지막 기간 끝나는 시간
	DWORD						dwMainTerm;			// 기간 단위 (달은 시작일 나머지는 초) 
	DWORD						dwSubTerm;			// 기간 단위 (달은 만료일 나머지는 초) 
	BYTE						byTermType;			// 0: 달 1: 일 2: 시
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_CREATE_RES )
	WORD		 				wResultCode;
	sPACKET_PRIVATESHOP_SHOP_DATA		sPrivateShopData;
	//Private Shop Item들은 GU_PRIVATESHOP_ITEM_DATA_INFO로 보낸다
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_CREATE_NFY )
	HOBJECT							hOwner;
	sSUMMARY_PRIVATESHOP_SHOP_DATA	sSummaryPrivateShopData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_EXIT_RES )
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_OPEN_RES )
	WORD						wResultCode;
	sSUMMARY_PRIVATESHOP_SHOP_DATA	sSummaryPrivateShopData;
	BYTE							byNoticeSize;
	WCHAR							wcNotice[NTL_MAX_PRIVATESHOP_NOTICE_IN_UNICODE + 1];
	//wcNotice가 가변이기 때문에 wcNotice 밑에다 새로운 변수를 설정하면 절대로 안된다.
	//반드시 byNoticeSize위에 변수 선언을 할것.[10/2/2007 SGpro]
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_OPEN_NFY )
	HOBJECT							hOwner;//상점 주인 CharacterID
	sSUMMARY_PRIVATESHOP_SHOP_DATA	sSummaryPrivateShopData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_CLOSE_RES )
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_CLOSE_NFY )
	HOBJECT						hOwner;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_ITEM_INSERT_RES )
	WORD						wResultCode;
	sPRIVATESHOP_ITEM_DATA		sPrivateShopItemData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_ITEM_UPDATE_RES )
	WORD						wResultCode;
	DWORD						dwZenny;
	BYTE						byPrivateShopInventorySlotPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_ITEM_DELETE_RES )
	WORD						wResultCode;
	sPRIVATESHOP_ITEM_DATA		sPrivateShopItemData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_ITEM_BUYING_RES )
	WORD						wResultCode;
	sPRIVATESHOP_ITEM_DATA		sPrivateShopItemData[NTL_MAX_BUY_SHOPPING_CART];
	BYTE						byBuyItemCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_ITEM_BUYING_NFY )
	HOBJECT						hPrivateShopOwner;
	sPRIVATESHOP_ITEM_DATA		sPrivateShopItemData[NTL_MAX_BUY_SHOPPING_CART];
	BYTE						byBuyItemCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_ENTER_RES )
	WORD						wResultCode;
	sPACKET_PRIVATESHOP_SHOP_DATA		sPrivateShopData;
//Private Shop Item들은 GU_PRIVATESHOP_ITEM_DATA_INFO로 보낸다
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_LEAVE_RES )
	WORD						wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PRIVATESHOP_ITEM_DATA_INFO)
	WORD						wResultCode;
	HOBJECT						hOwnerID;
	sPRIVATESHOP_ITEM_DATA		sPrivateShopItemData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_ITEM_SELECT_RES )
	WORD				wResultCode;
	//HOBJECT				hOwner;
	BYTE				byPrivateShopInventorySlotPos;
	BYTE				byItemState;//eITEMSTATE 값을 갖는다
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_ITEM_SELECT_NFY )
	//HOBJECT				hOwner;
	BYTE				byPrivateShopInventorySlotPos;
	BYTE				byItemState;//eITEMSTATE 값을 갖는다
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_ITEM_STATE_NFY )
	//HOBJECT				hOwner;
	BYTE				abyPrivateShopItemState[NTL_MAX_PRIVATESHOP_INVENTORY];
	int					nPrivateShopItemStateCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_LEAVE_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_REQUEST_RES )
	WORD						wResultCode;
	BYTE						byPrivateShopInventorySlotPos;
	BYTE						byItemState;//eITEMSTATE 값을 갖는다
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_REQUEST_NFY )
	HOBJECT						hOwner;
	BYTE						byPrivateShopInventorySlotPos;
	BYTE						byItemState;//eITEMSTATE 값을 갖는다
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_CONSENT_REQ )
	HOBJECT						hVisitorID;
	BYTE						byPrivateShopInventorySlotPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_CONSENT_NFY )
	HOBJECT						hOwner;
	BYTE						byPrivateShopInventorySlotPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_WAITTIMEOUT_NFY )
	HOBJECT						hOwner;
	BYTE						byPrivateShopInventorySlotPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_PRICEFLUCTUATIONS_RES )
	WORD						wResultCode;
	BYTE						byPrivateShopInventorySlotPos;
	DWORD						dwBeforeZenny;
	DWORD						dwAfterZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_PRICEFLUCTUATIONS_NFY )
	HOBJECT						hOwner;
	BYTE						byPrivateShopInventorySlotPos;
	DWORD						dwBeforeZenny;
	DWORD						dwAfterZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_IMMEDIATE_ITEM_BUYING_RES )
	WORD						wResultCode;
	sPRIVATESHOP_ITEM_DATA		sPrivateShopItemData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_IMMEDIATE_ITEM_BUYING_NFY )
	HOBJECT						hPrivateShopOwner;
	sPRIVATESHOP_ITEM_DATA		sPrivateShopItemData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_WINDOWCLOSE_NFY )
	HOBJECT						hPrivateShopOwner;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_CANCEL_RES )
	WORD				wResultCode;
	BYTE				byCancelType;
	BYTE				byPrivateShopInventorySlotPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PRIVATESHOP_BUSINESS_CANCEL_NFY )
	HOBJECT						hOwner;
	BYTE						byCancelType;//1: 흥정대기때 취소, 2: 흥정때 취소
	BYTE						byPrivateShopInventorySlotPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TUTORIAL_WAIT_NFY )
	WORD						wWaitingCount; // 현재 자신앞의 대기수
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TUTORIAL_WAIT_CANCEL_RES )
	WORD						wResultCode; 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_TUTORIAL_PLAY_QUIT_RES )
	WORD						wResultCode; 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_START_RES )
	HOBJECT	hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	WORD 	wResultCode;	// 결과
	bool	bIsAway;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_SEND_RES )
	HOBJECT	hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	WCHAR wszTargetName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];	// 받을 캐릭 이름
	WORD wResultCode;	// 성공유무
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_READ_RES )
	HOBJECT	hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	MAILID	mailID;	// 메일 아이디
	WORD	wResultCode;	// 메일이 이미 삭제 되었을 때는 실패 
	BYTE	byRemainDay;
	DBOTIME endTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_DEL_RES )
	HOBJECT	hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	MAILID	mailID;	// 메일 아이디
	WORD	wResultCode;	// 메일이 이미 삭제 되었을 때는 실패 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_RETURN_RES )
	HOBJECT	hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	MAILID	mailID;	// 메일 아이디
	WORD	wResultCode;	// 메일이 이미 삭제 되었을 때는 실패 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_RELOAD_RES )
	HOBJECT	hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	WORD wResultCode;	// 성공유무
	BYTE	byMailCount;		// 현재 전체메일 카운트 
	BYTE	byManagerCount;		// 읽지 않은 매니져 메일 카운트
	BYTE	byNormalCount;		// 읽지 않은 노멀 메일 카운트
	MAILID   aMailID[NTL_MAX_MAIL_SLOT_COUNT];	// 카운트에 해당하는 저장된 메일 아이디 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_RELOAD_NFY )		// 스케줄에 의해 일정시간에 따라 자동 송신
	BYTE	byMailCount;		// 현재 전체메일 카운트 
	BYTE	byManagerCount;		// 읽지 않은 매니져 메일 카운트
	BYTE	byNormalCount;		// 읽지 않은 노멀 메일 카운트
	MAILID  aMailID[NTL_MAX_MAIL_SLOT_COUNT];	// 카운트에 해당하는 저장된 메일 아이디 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_LOAD_RES )
	HOBJECT	hObject;		// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	WORD	wResultCode;	// 성공유무
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_LOAD_DATA )
	sMAIL_NEW_PROFILE sData;		
	BYTE	byTextSize;
	WCHAR	wszText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE_IN_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_LOAD_INFO )
	sMAIL_NEW_PROFILE sData;	
	BYTE	byTextSize;
	WCHAR	wszText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE_IN_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_ITEM_RECEIVE_RES )
	HOBJECT	hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	WORD wResultCode;	// 성공유무
	MAILID	mailID;	// 메일 아이디
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_LOCK_RES )
	HOBJECT	hObject;	// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	MAILID	mailID;	// 메일 아이디
	bool	bIsLock;		// Lock 1: Unlock: 0
	WORD 	wResultCode;	// 메일이 이미 삭제 되었을 때는 실패
	BYTE	byExpiredDay;	// 남은날짜
	DBOTIME	endTime;		
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_CLOSE_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MAIL_MULTI_DEL_RES )
	HOBJECT	hObject;		// 메일송수신탑 오브젝트 (스카우트파츠 착용시 INVALID_OBJECT )
	WORD	wResultCode;	// 메일이 이미 삭제 되었을 때는 실패
	BYTE	byCount;
	MAILID	aMailID[NTL_MAX_COUNT_OF_MULTI_DEL];	// 메일 아이디
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PORTAL_START_RES)
	WORD						wResultCode;
	HOBJECT			hNpcHandle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PORTAL_INFO)
	BYTE			byCount;
	PORTALID		aPortalID[NTL_PORTAL_MAX_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PORTAL_ADD_RES)
	WORD						wResultCode;
	HOBJECT			hNpcHandle;
	PORTALID		PortalID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PORTAL_RES)
	WORD				wResultCode;
	HOBJECT			hNpcHandle;
	BYTE			byPoint;		// 0~7까지 선택된 인덱스
	CNtlVector		vLoc;
	CNtlVector		vDir;
	WORLDID			worldID;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_WAR_FOG_INFO)
	BYTE			abyWarFogInfo[NTL_MAX_SIZE_WAR_FOG];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_WAR_FOG_UPDATE_RES)
	WORD			wResultCode;
	HOBJECT			handle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MOB_LUCKY_DROP_NFY)
	HOBJECT			hMobhandle;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_UPGRADE_ALL_NFY)
	BYTE			abyGrade[NTL_ITEM_UPGRADE_EQUIP_COUNT];  // 배열 인덱스는 해당 Pos, Value는 해당 결과 그레이드( 255 INVALID는 아이템이 없는 경우)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_BOT_BOTCAUTION_NFY )		//  [2/20/2008 SGpro]
	HOBJECT			hBot;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_REPUTATION)
	HOBJECT			handle;
	DWORD			dwReputation;
END_PROTOCOL()
//-----------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_FUNCTION_ADD_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_GIVE_ZENNY_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_BANK_START_RES)
	HOBJECT			handle;			// Npc handle
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_BANK_MOVE_RES)
	HOBJECT			handle;
	HOBJECT			hSrcItem;
	HOBJECT			hDstItem;
	BYTE			bySrcPlace;
	BYTE			bySrcPos;
	BYTE			byDestPlace;
	BYTE			byDestPos;
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_BANK_MOVE_STACK_RES)
	HOBJECT			handle;
	HOBJECT			hSrcItem;
	HOBJECT			hDestItem;
	BYTE			bySrcPlace;
	BYTE			bySrcPos;
	BYTE			byDestPlace;
	BYTE			byDestPos;
	BYTE			byStackCount1;
	BYTE			byStackCount2;
	ITEMID			splitItemId;
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_BANK_END_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_BANK_ZENNY_INFO)
	DWORD			dwZenny;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_BANK_ITEM_INFO)
	BYTE            byPlace;
	BYTE			byItemCount;
	sITEM_PROFILE	aItemProfile[NTL_MAX_BANK_ITEM_SLOT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_BANK_ZENNY_RES)
	HOBJECT			handle;
	WORD			wResultCode;
	DWORD			dwZenny;
	bool			bIsSave;		// 1: 저금 0: 인출
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_CROSSFIRE_RES )
	HOBJECT			hTarget;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_GUILD_CREATE_MARK_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_GUILD_CHANGE_MARK_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_MARK_CHANGED_NFY)
	HOBJECT				hSubject;
	sDBO_GUILD_MARK		sMark;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BANK_LOAD_RES)
	HOBJECT				handle;			// Npc handle
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_CHANGE_NAME_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PARTY_SHARETARGET_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_PARTY_SHARETARGET_NFY )
	sSHARETARGET_INFO		sharetargetInfo[NTL_MAX_SHARETARGET_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_RIDE_ON_BUS_RES)
	HOBJECT				hTarget;			// Npc handle
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_RIDE_OFF_BUS_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TRANSFORM_CANCEL_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_ITEM_IDENTIFY_RES)
	WORD			wResultCode;
	HOBJECT			hNpchandle;		// NpcHandle
	BYTE			byPlace;
	BYTE			byPos;
	sITEM_DATA      sItemData;			// 성공시에만 세팅
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MINORMATCH_STATE_UPDATE_NFY)
	BYTE			byMatchState;		// eBUDOKAI_MAJORMATCH_STATE
	bool			bIsEnter;			// true : Enter, false : exit
	BYTE			byStage;
	DWORD			dwRemainTime;		// 밀리세컨드 단위
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MINORMATCH_TEAM_INFO_NFY)
	BYTE			byMatchIndex;		// 몇번째 경기인지?[개인:0~31, 팀:0~15]
	BYTE			byTeamCount;
	WORD			wTeamInfo_var;		// sMATCH_MINORMATCH_TEAM_INFO_VAR [NTL_MAX_MINORMATCH_TEAM_COUNT];
	sVARIABLE_DATA	sData;			
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MINORMATCH_PLAYER_STATE_NFY)
	HOBJECT			hPc;
	BYTE			byPcState;			// eMATCH_MEMBER_STATE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MINORMATCH_UPDATE_SCORE_NFY)
	TEAMTYPE		wTeamType;
	BYTE			byScore;
	HOBJECT			hSlayer;			// 기절시킨 character
	HOBJECT			hFainter;			// 기절한 character
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MINORMATCH_TEAM_SCORE_NFY)
	BYTE							byCount;
	sMATCH_MINORMATCH_TEAM_SCORE	aTeamScore[BUDOKAI_MAX_MINORMATCH_TEAM_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MINORMATCH_SELECTION_NFY)
	TEAMTYPE		teamWinner;
	BYTE			byTeamCount;
	sMINORMATCH_SELECTION_DATA	aSelection[BUDOKAI_MAX_MINORMATCH_TEAM_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MINORMATCH_STAGE_FINISH_NFY)
	BYTE			byStageResult;		// eMATCH_RESULT
	WORD			wStageWinner;		// Stage Winner TeamType
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MINORMATCH_MATCH_FINISH_NFY)
	BYTE			byMatchResult;
	WORD			wMatchWinner;
	BYTE			byKillCount;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MAJORMATCH_STATE_UPDATE_NFY)
	BYTE			byMatchState;		// eBUDOKAI_MAJORMATCH_STATE
	bool			bIsEnter;			// true : Enter, false : exit
	BYTE			byStage;
	DWORD			dwRemainTime;		// 밀리세컨드 단위
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MAJORMATCH_TEAM_INFO_NFY)
	BYTE			byMatchDepth;	// 개인[8강~32강] 팀[8강~16강]
	BYTE			byMatchIndex;	// depth에 따른 경기 위치

	BYTE			byTeamCount;
	WORD			wTeamInfo_var;		// sMATCH_MINORMATCH_TEAM_INFO_VAR [2];
	sVARIABLE_DATA	sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MAJORMATCH_PLAYER_STATE_NFY)
	HOBJECT			hPc;
	BYTE			byPcState;			// eMATCH_MEMBER_STATE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MAJORMATCH_UPDATE_SCORE_NFY)
	sMATCH_SCORE	sStageScore;		// 스테이지 스코어
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MAJORMATCH_STAGE_FINISH_NFY)
	BYTE			byStageResult;		// eMATCH_RESULT
	WORD			wStageWinner;		// Stage Winner TeamType
	sMATCH_SCORE	sMatchScore;		// 경기 스코어
	bool			bIsObserver;		// 옵져버 모드에서 사용
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_MAJORMATCH_MATCH_FINISH_NFY)
	BYTE			byMatchResult;		// eMATCH_RESULT
	WORD			wMatchWinner;		// Match Winner TeamType
	sMATCH_SCORE	sMatchScore;		// 경기 스코어
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_FINALMATCH_STATE_UPDATE_NFY)
	BYTE			byMatchState;		// eBUDOKAI_MAJORMATCH_STATE
	bool			bIsEnter;			// true : Enter, false : exit
	BYTE			byStage;
	DWORD			dwRemainTime;		// 밀리세컨드 단위
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_FINALMATCH_TEAM_INFO_NFY)
	BYTE			byMatchDepth;
	BYTE			byMatchIndex;

	BYTE			byTeamCount;
	WORD			wTeamInfo_var;		// sMATCH_FINALMATCH_TEAM_INFO_VAR [4];
	sVARIABLE_DATA	sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_FINALMATCH_PLAYER_STATE_NFY)
	HOBJECT			hPc;
	BYTE			byPcState;			// eMATCH_MEMBER_STATE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_FINALMATCH_UPDATE_SCORE_NFY)
	sMATCH_SCORE	sStageScore;		// 스테이지 스코어
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_FINALMATCH_STAGE_FINISH_NFY)
	BYTE			byStageResult;		// eMATCH_RESULT
	WORD			wStageWinner;		// Stage Winner TeamType
	sMATCH_SCORE	sMatchScore;		// 경기 스코어
	bool			bIsObserver;		// 옵져버 모드에서 사용
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_FINALMATCH_MATCH_FINISH_NFY)
	BYTE			byMatchResult;		// eMATCH_RESULT
	WORD			wMatchWinner;		// Match Winner TeamType
	sMATCH_SCORE	sMatchScore;		// 경기 스코어
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_MATCH_AWARDING_NFY)
	WORD			awTeamName[MAX_FINALMATCH_RESULT];
	sVARIABLE_DATA	sData;	// WCHAR wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1]; : 저장된 값은 NULL을 포함하지 않는다.
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_CONFUSE_TARGET_NFY)
	HOBJECT			hSubject;
	HOBJECT			hTarget;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_TERROR_COURSE_CHANGE_NFY)
	HOBJECT			hSubject;
	HOBJECT			hAttacker;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DICE_ROLL_RES)
	WORD			wResultCode;
	WORD			wDiceResult;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DICE_ROLLED_NFY)
	HOBJECT			hSubject;
	WORD			wDiceResult;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_STATE_INFO_NFY)
	BUDOKAITIME							tmDefaultOpenTime;
	bool								bRankbattleAllow;

	sBUDOKAI_UPDATE_STATE_INFO			sStateInfo;
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	aMatchStateInfo[MAX_BUDOKAI_MATCH_TYPE];

	sBUDOKAI_JOIN_INFO					sJoinInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_UPDATE_STATE_NFY)
	sBUDOKAI_UPDATE_STATE_INFO			sStateInfo;
	bool								bRankbattleAllow;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_UPDATE_MATCH_STATE_NFY)
	BYTE								byMatchType;	// eBUDOKAI_MATCH_TYPE
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	sStateInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_JOIN_INDIVIDUAL_RES)
	WORD			wResultCode;
	WORD			wJoinId;			// 참가 번호
	bool			bDojoRecommender;	// 도장추천자인지?
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_LEAVE_INDIVIDUAL_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_JOIN_TEAM_INFO_RES)
	WORD						wResultCode;

	BYTE						byMemberCount;
	sBUDOKAI_TEAM_POINT_INFO	asPointInfo[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_JOIN_TEAM_RES)
	WORD			wResultCode;
	HOBJECT			hTroublePc;
	float			fTotalRankPoint;
	WORD			wJoinId;
	WCHAR			wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];

	// 
	BYTE						byMemberCount;
	sBUDOKAI_TEAM_POINT_INFO	aTeamInfo[NTL_MAX_MEMBER_IN_PARTY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_JOIN_TEAM_NFY)
	float			fTotalRankPoint;
	WORD			wJoinId;
	WCHAR			wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];

	// 
	BYTE						byMemberCount;
	sBUDOKAI_TEAM_POINT_INFO	aTeamInfo[NTL_MAX_MEMBER_IN_PARTY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_LEAVE_TEAM_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_LEAVE_TEAM_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_LEAVE_TEAM_MEMBER_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_LEAVE_TEAM_MEMBER_NFY)
	WCHAR			wszMemberName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_JOIN_INFO_RES)
	WORD					wResultCode;

	BYTE					byMatchType;		// eBUDOKAI_MATCH_TYPE

	// 추후 다음 데이터 압축 필요
	union
	{
		sBUDOKAI_REGISTER_INDIVIDUAL_INFO	sIndividualInfo;
		sBUDOKAI_REGISTER_TEAM_INFO			sTeamInfo;
	};
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_JOIN_STATE_RES)
	WORD			wResultCode;
	BYTE			byMatchType;		// eBUDOKAI_MATCH_TYPE
	BYTE			byJoinState;		// eBUDOKAI_JOIN_STATE
	BYTE			byJoinResult;		// 참가 결과

	// 정보가 없을 경우
	// byMatchType = INVALID_BUDOKAI_MATCH_TYPE
	// byJoinState = INVALID_BUDOKAI_JOIN_STATE
	// byJoinResult = INVALID_BUDOKAI_JOIN_RESULT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_JOIN_STATE_NFY)
	BYTE			byMatchType;		// eBUDOKAI_MATCH_TYPE
	BYTE			byJoinState;		// eBUDOKAI_JOIN_STATE
	BYTE			byJoinResult;		// eBUDOKAI_JOIN_RESULT
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DISCONNECTED_NFY)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BOT_BOTCAUTION_HELPME_NFY)
	HOBJECT			hBot;
	bool			bIsRequester; // true : SOS를 요청한 자
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_MUDOSA_INFO_RES)
	WORD					wResultCode;
	BYTE					byMudosaCount;
	sBUDOKAI_MUDOSA_INFO	aMudosaInfo[BUDOKAI_MAX_MUDOSA_INFO_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_MUDOSA_TELEPORT_RES)
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_PROGRESS_MESSAGE_NFY)
	BYTE					byMsgId;			// eBUDOKAI_PROGRESS_MESSAGE
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_SOCIAL_ACTION_NFY)
	HOBJECT				hSubject;
	ANIMATIONID			socialAction;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_PRIZEWINNER_NAME_RES)
	WORD				wResultCode;

	// BUDOKAI_MATCH_TYPE_INDIVIDUAL
	//- WCHAR		wszName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
	WORD				wWinner;			
	WORD				wSecondWinner;

	sVARIABLE_DATA		sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_PRIZEWINNER_TEAM_NAME_RES)
	WORD				wResultCode;

	// BUDOKAI_MATCH_TYPE_TEAM
	//- WCHAR		wszTeamName[NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE + 1];
	WORD				wWinner;			
	WORD				wSecondWinner;

	sVARIABLE_DATA		sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_GM_BUDOKAI_SERVER_ENTER_RES)
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_GM_BUDOKAI_SERVER_LEAVE_RES)
	WORD					wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_GM_MATCH_PROGRESS_INFO_RES)
	WORD								wResultCode;

	BYTE								byMatchType;		// eBUDOKAI_MATCH_TYPE
	BYTE								byMatchWorldType;	// eMATCHWORLD_TYPE;
	BYTE								byMatchDepth;		// eBUDOKAI_MATCH_DEPTH

	BYTE								byStateCount;
	sBUDOKAI_GM_MATCH_PROGRESS_STATE	aStateList[BUDOKAI_MAX_MATCH_COUNT];	// 가변
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_GM_MATCH_ARENA_ENTER_RES)
	WORD								wResultCode;

	BYTE								byMatchType;		// eBUDOKAI_MATCH_TYPE
	BYTE								byMatchWorldType;	// eMATCHWORLD_TYPE
	BYTE								byMatchDepth;		// eBUDOKAI_MATCH_DEPTH
	BYTE								byMatchIndex;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUDOKAI_GM_MATCH_ARENA_LEAVE_RES)
	WORD								wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SCOUTER_TURN_ON_NFY)
	HOBJECT			hSubject;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SCOUTER_TURN_OFF_NFY)
	HOBJECT			hSubject;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SCOUTER_BROKEN_NFY)
	HOBJECT			hSubject;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SCOUTER_ITEM_SELL_RES)
	TBLIDX				itemTblidx;
	BYTE				byPlace;
	BYTE				byPos;
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_EVENTITEM_START_RES)
	HOBJECT			handle;
	BYTE			byType; //	0: 기본  1:한정수량 판매 이벤트 NPC 
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_EVENTITEM_BUY_RES)
	HOBJECT			handle;
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_EVENTITEM_END_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_ITEM_REPLACE)
	HOBJECT			hDeleteItem;
	HOBJECT			hCreateItem;
	BYTE			byDeleteItemPlace;
	BYTE			byDeleteItemPos;
	sITEM_DATA      sCreateItemData;
	bool			bIsNew;		// Effect 추가 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_GAMBLE_BUY_RES)
	WORD			wResultCode;
	HOBJECT			handle;
	HOBJECT			hItem;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_DUNGEON_DIFF_NFY)
	ePARTY_DUNGEON_STATE eDiff;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_DUNGEON_DIFF_RES)
	WORD			wResultCode;
	ePARTY_DUNGEON_STATE		eDiff;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_DUNGEON_INIT_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_DUNGEON_INIT_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_SP)
	DWORD			dwSpPoint;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_INIT_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SKILL_REMOVE_NFY)
	TBLIDX			skillId;
	BYTE			bySlot;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HTB_REMOVE_NFY)
	TBLIDX			skillId;
	BYTE			bySlot;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_MARKING_NFY)
	HOBJECT			handle;
	sMARKING		sMarking;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHARM_DICE_START_NFY)
	DWORD			dwWaitTick;		//ms
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHARM_DICE_ROLLED_NFY)
	HOBJECT			handle;	// 해당 캐릭터 핸들
	DWORD			dwRet;	// 결과값
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHARM_DICE_ROLLED_RES)
	DWORD			dwRet;	// 결과값
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_CHARM_DICE_END_NFY)
	bool			bIsSuccess;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_ITEM_RANK_SET_RES)
	WORD			wResultCode;
	BYTE			byItemRank; // eITEM_RANK 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_ITEM_RANK_SET_NFY)
	BYTE			byItemRank; // eITEM_RANK 
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_LOCK_NFY)
	bool				bIsOpen;	// 1: Open 0:Close
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_LOCK_RES)
	WORD				wResultCode;    
	bool				bIsOpen;	// 1: Open 0:Close
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_RESET_RES)
	WORD				wResultCode;      
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_RESET_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_ACCEPTED_RES)
	BYTE				byItemSlot;	// 파티인벤 해당 슬롯 아이템 낙찰
	WORD				wResultCode;          
	HOBJECT				hTarget;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_ACCEPTED_NFY)
	BYTE				byItemSlot;	// 파티인벤 해당 슬롯 아이템 낙찰
	HOBJECT				hTarget;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_GIVE_RES)
	WORD				wResultCode;         
	BYTE				byItemSlot;		// 파티인벤 해당 슬롯 아이템 
	HOBJECT				hTarget;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_GIVE_NFY)
	BYTE				byItemSlot;		// 파티인벤 해당 슬롯 아이템 
	HOBJECT				hTarget;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_DICE_START_RES)
	WORD				wResultCode;         
	BYTE				byItemSlot;		// 파티인벤 해당 슬롯 아이템	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_DICE_START_NFY)
   	BYTE				byItemSlot;		// 파티인벤 해당 슬롯 아이템	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_DICE_ROLL_RES)
	WORD				wResultCode;         
	BYTE				byRet;		// 주사위결과전송	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_DICE_ROLL_NFY)
	HOBJECT				handle;			// 주사위주체
   	BYTE				byRet;			// 주사위결과전송	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PARTY_INVEN_DICE_END_NFY)
   	HOBJECT				hTarget;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TELEPORT_PROPOSAL_NFY)
	BYTE				byTeleportType;
	BYTE				byInfoIndex;		// 서버에서 텔레포트 정보를 참조하기위한 인덱스
	WORD				wWaitTime;
	TBLIDX				worldTblidx;		// 월드 테이블 인덱스 : 정보를 좀더 상세하게 출력하기 위한 용도.

	union
	{
		DWORD			dwReserve;
		BYTE			byBudokaiMatchDepth;	// 천하제일무도회 XX강
	};

	BYTE				bySummonerNameLength;
	WCHAR				wszSummonnerName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];	// 가변
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_TELEPORT_CONFIRM_RES)
	WORD				wResultCode;

	BYTE				byTeleportIndex;
	bool				bTeleport;			// 클라이언트에서 받은 값
	bool				bClearInterface;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_PETITION_INFO)
	PETITIONID			petitionID;
	WCHAR				awchQuestionContent[NTL_MAX_SIZE_QUESTION_CONTENT_UNICODE + 1];
	PETITIONCATEGORY	nCategory1;//ePETITIONSYSTEM_CATEGORY_1
	PETITIONCATEGORY	nCategory2;//ePETITIONSYSTEM_CATEGORY_2
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GAME_COMMON_MSG_NFY)
	WORD				wResultCode;         
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_NICKNAME_CHANGED_NFY)
	HOBJECT				hSubject;
	TBLIDX				nicknameTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HOIPOIMIX_RECIPE_REG_NFY)			// 레시피등록
 	TBLIDX				recipeTblidx;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HOIPOIMIX_ITEM_CHECK_RES)			// 만들아이템 선택 (필요한 아이템 수량 체크)
	WORD				wResultCode;    
	HOBJECT				objHandle;			
	TBLIDX				recipeTblidx;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HOIPOIMIX_ITEM_MAKE_RES)			// 아이템 만들기
	WORD				wResultCode;    
	BYTE				bySuccessResult;			// eITEM_MIX_RESULT_TYPE
	HOBJECT				objHandle;			
	TBLIDX				recipeTblidx;	
	HOBJECT				itemHandle;
END_PROTOCOL()
//------------------------------------------------------------------			
BEGIN_PROTOCOL(GU_HOIPOIMIX_ITEM_MAKE_EP_RES)		// 아이템 만들기 중 EP 넣기
	WORD				wResultCode;    
END_PROTOCOL()
//------------------------------------------------------------------	
BEGIN_PROTOCOL(GU_HOIPOIMIX_ITEM_RECIPE_INFO)  
	BYTE				byCount;
	sRECIPE_DATA		asRecipeData[NTL_ITEM_RECIPE_MAX_COUNT];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DYNAMIC_OBJECT_UPDATE_STATE)
	HOBJECT				handle;
	sDYNAMIC_OBJECT_BRIEF		dynamicObjBrief;
	sDYNAMIC_OBJECT_STATE		dynamicObjState;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HOIPOIMIX_ITEM_MACHINE_MAKE_RES)			// 머신 필드 등록
	WORD				wResultCode;    
	BYTE				byPlace;
	BYTE				byPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HOIPOIMIX_ITEM_MACHINE_DEL_RES)			// 머신 필드 삭제
	WORD				wResultCode;    
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HOIPOIMIX_ITEM_MAKE_EXP_NFY)			
	DWORD				dwCurExp;
	BYTE				byCurLevel;
	bool				bIsNormal; // recipe type이 eRECIPE_NORMAL 인가 아닌가.
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_TARGET_LOST_NFY)
	HOBJECT						hSubject;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_GUARD_CRASHED_NFY)
	HOBJECT						hSubject;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_RP_BALL)
	HOBJECT			handle;
	BYTE			byCurRPBall;
	bool			bDropByTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_RP_BALL_MAX)
	HOBJECT			handle;
	BYTE			byMaxRPBall;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HOIPOIMIX_JOB_SET_RES)
	WORD				wResultCode;  
	HOBJECT				hNpchandle;
	BYTE				byRecipeType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HOIPOIMIX_JOB_RESET_RES)
	WORD				wResultCode;  
	HOBJECT				hNpchandle;
	BYTE				byRecipeType;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_RP_INCREASE_START_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_RP_DECREASE_START_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_AVATAR_RP_INCREASE_STOP_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_HTB_LAST_STEP_CHANGED_NFY)
	HOBJECT			hSubject;
	BYTE			byLastStep;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_CHARGE_CANCELED_NFY)
	HOBJECT			hSubject;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_REFRESH_LP_EP)
	HOBJECT			hSubject;
	WORD			wCurLp;
	WORD			wMaxLp;
	WORD			wCurEp;
	WORD			wMaxEp;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_CHAR_NONBATTLEACTIONPATTERNSET_UNITIDX_NFY )
	HOBJECT				hBotHandle;
	ACTIONPATTERN_FIELD unitIdx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_VEHICLE_START_NFY )
	HOBJECT				hDriverHandle;	
	sVECTOR3			vStartPosition;
	TBLIDX				idFuelItemTblidx;
	HOBJECT				hVehicleItem; 
	TBLIDX				idVehicleItemTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_VEHICLE_ENGINE_START_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_VEHICLE_ENGINE_START_NFY )
	HOBJECT				hDriverHandle;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_VEHICLE_ENGINE_STOP_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_VEHICLE_ENGINE_STOP_NFY )
	HOBJECT				hDriverHandle;	
	DWORD				dwFuelRemain;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_VEHICLE_FUEL_UPDATE_NFY )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_VEHICLE_END_NFY )
	HOBJECT				hDriverHandle;
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_VEHICLE_END_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_VEHICLE_STUNT_NFY )
	HOBJECT			hDriverHandle;	
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_SKILL_PASSIVE_EFFECT_APPLIED_NFY )
	TBLIDX			skillTblidx;
	BYTE			bySlotIndex;
	sDBO_UPDATED_SKILL_DATA		sUpdatedSkillData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_AVATAR_UPDATED_SKILL_DATA_INFO )
	TBLIDX			skillTblidx;
	BYTE			bySlotIndex;
	sDBO_UPDATED_SKILL_DATA		sUpdatedSkillData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_MONSTER_TRANSFORMED_NFY )
	HOBJECT			hSubject;
	TBLIDX			newTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_DOJO_CREATE_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_DOJO_DELETE_RES )
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOJO_FUNCTION_ADD_RES)
	WORD				wResultCode;
	BYTE				byFunction;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOJO_BANK_HISTORY_RES)
	WORD				wResultCode;
	DWORD				dwBalance;
	BYTE				byCount;
    sDBO_DOJO_BANK_HISTORY_INFO asData[DBO_MAX_COUNT_DOJO_BANK_HISTORY];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOGI_CREATE_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOGI_CHANGE_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOGI_UPDATE_NFY)
	HOBJECT				handle;
	sDBO_DOGI_DATA		sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_DOGI_CREATE_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GUILD_DOGI_CHANGE_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHILD_ADULT_SWITCHED_NFY)
	HOBJECT				hSubject;
	bool				bIsAdult;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHANGE_CLASS_AUTHORITY_CHANGED_NFY)
	bool				bCanChangeClass;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_PLAY_BGM)
	bool				bPlay;
	bool				bLoop;
	DWORD				dwDelay;
	BYTE				byLength;
	char				szName[1];		// null 을 포함해서 보낸다.
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOJO_MARK_CHANGED_NFY)
	TBLIDX				dojoTblidx;
	GUILDID				guildId;
	sDBO_GUILD_MARK		sMark;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOJO_LEVEL_CHANGED_NFY)
	TBLIDX				dojoTblidx;
	BYTE				byLevel;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_NETP)
	WORD				wResultCode;
	NETP				netP;
	NETP				dwBonusNetP;
	NETP				dwAccumulationNetP;
	DWORD				timeNextGainTime;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_DOJO_SCRAMBLE_RES )			// 도장쟁탈전 신청에 대한 응답 (도전자)
	WORD				wResultCode;			
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOJO_SCRAMBLE_RESPONSE_RES )
	WORD				wResultCode;
	WCHAR				wszGuildName[NTL_MAX_SIZE_GUILD_NAME_IN_UNICODE + 1];
	bool				bIsRetry; // true면 다시 확인해야한다
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_NETMARBLEMEMBERIP_NFY )
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUS_LOCATION_NFY)
	HOBJECT				hSubject;
	TBLIDX				busTblidx;
	sVECTOR2			vCurLoc;
	sVECTOR2			vCurDir;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUS_LOCATION_ERASED_NFY)
	HOBJECT				hSubject;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_BUS_LOCATION_RESET_ALL_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL( GU_ITEM_EXPIRED_NFY )
	HOBJECT				hItem;
	BYTE				byPlace;
	BYTE				byPos;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_NETPYITEM_START_RES)
	BYTE			byType; //	0: 기본  1:한정수량 판매 이벤트 NPC 
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_NETPYITEM_BUY_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_SHOP_NETPYITEM_END_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_REFRESH_BATTLE_ATTRIBUTE_MODIFIER)
	HOBJECT				hSubject;
	float				fHonestOffenceModifier;
	float				fHonestDefenceModifier;
	float				fStrangeOffenceModifier;
	float				fStrangeDefenceModifier;
	float				fWildOffenceModifier;
	float				fWildDefenceModifier;
	float				fEleganceOffenceModifier;
	float				fEleganceDefenceModifier;
	float				fFunnyOffenceModifier;
	float				fFunnyDefenceModifier;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CHAR_RANKBATTLE_ALLOW_NFY)
	bool				bAllow;
	WORD				wReasonCode;
END_PROTOCOL()
//------------------------------------------------------------------

BEGIN_PROTOCOL(GU_CHAR_RANKPOINT_RESET_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOJO_NPC_INFO_RES)
	WORD					wResultCode;
	sDBO_DOJO_NPC_INFO		sData;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GAMEGUARD_AUTH_ANSWER_REQ)
	BYTE				byDataSize;
	BYTE				abyData[UCHAR_MAX];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOJO_SEAL_ATTACK_BEGIN_NFY)
	TBLIDX				sealObjectTblidx;
	bool				bIsBeingStolen;
	sVECTOR2			vLoc;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOJO_SEAL_ATTACK_END_NFY)
	TBLIDX				sealObjectTblidx;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOJO_SEAL_CURRENT_STATE_NFY)
	TBLIDX				sealObjectTblidx;
	BYTE				byState;		// eTOBJECT_STATE_TYPE_C
	sVECTOR2			vLoc;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_INVENTORY_CAPSULE_UNLOCK_NFY)
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DOJO_SCRAMBLE_SHIFT_SEAL_STATE_NFY)
	TBLIDX						dojoTblidx;
	TBLIDX						objectTblidx;
	GUILDID						guildId;
	WCHAR						wszName[NTL_MAX_SIZE_CHAR_NAME_UNICODE + 1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_LP_STATUS_NFY)
	HOBJECT			handle;
	bool			bEmergency;	// true: Emergency false: General
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DURATION_ITEM_BUY_RES)
	TBLIDX				merchantTblidx;
	BYTE				byPos;
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_DURATION_RENEW_RES)
	HOBJECT				hItemHandle;
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CASHITEM_START_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CASHITEM_INFO_NFY)
	BYTE				byCount;
	sCASHITEM_BRIEF		aInfo[1];
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CASHITEM_END_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CASHITEM_MOVE_RES)
	WORD				wResultCode;
	DWORD				dwProductId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CASHITEM_USE_RES)
	WORD				wResultCode;
	DWORD				dwProductId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CASHITEM_HLSHOP_START_RES)
	WORD				wResultCode;
	DWORD				dwRemainAmount;							// 현재잔액
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CASHITEM_HLSHOP_END_RES)
	WORD				wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CASHITEM_BUY_RES)
	WORD			wResultCode;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GMT_INFO_NFY)
	BYTE					byGMTResetCount;
	sGAME_MANIA_TIME_DATA	sCurrent;
	sGAME_MANIA_TIME_DATA	sNext;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GMT_UPDATE_RES)
	WORD				wResultCode;
	sGAME_MANIA_TIME	sNext;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_GMT_CHANGED_NFY)
	sGAME_MANIA_TIME	sCurrent;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CASHITEM_ADD_NFY)
	sCASHITEM_BRIEF		sInfo;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_CASHITEM_DEL_NFY)
	DWORD				dwProductId;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_MAX_LP)
	HOBJECT			hSubject;
	WORD			wMaxLp;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_MAX_EP)
	HOBJECT			hSubject;
	WORD			wMaxEp;
END_PROTOCOL()
//------------------------------------------------------------------
BEGIN_PROTOCOL(GU_UPDATE_CHAR_MAX_RP)
	HOBJECT			hSubject;
	WORD			wMaxRp;
END_PROTOCOL()
//------------------------------------------------------------------

#pragma pack(pop)
