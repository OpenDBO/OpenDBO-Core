#ifndef __DISPLAY_STRING_DEF_H__
#define __DISPLAY_STRING_DEF_H__

enum EDispStingType
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Client Result Text _ 동작의 결과를 표기하기 위한 텍스트. 
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Global Message
	DST_INPUT_YOUR_ID,				// 아이디를 입력하십시요
	DST_INPUT_YOUR_PASSWORD,		// 비밀번호를 입력하십시요
	DST_SERVER_CONNECTING,			// 서버 정보를 얻어오는 중입니다.
	DST_ACCOUNT_SERVER_CONNECT_FAIL,// 인증서버연결 실패
	DST_CHAR_SERVER_CONNECT_FAIL,	// 캐릭터서버 연결 실패 
	DST_GAME_SERVER_CONNECT_FAIL,	// 게임서버 연결 실패 
	DST_CHAT_SERVER_CONNECT_FAIL,	// 채팅서버 연결 실패
	DST_CHAT_SERVER_TITLE,          // 채팅 서버 알림 (타이틀)
	DST_CHAT_SERVER_DISCONNECT,     // 채팅서버 연결이 끊겼습니다.
	DST_CHAT_SERVER_RECONNECT,      // 채팅서버에 다시 연결이 됐습니다.
	DST_SERVER_DISCONNECT,			// 서버연결 종료 
	DST_ACCOUNT_CREATE_SUCCESS,		// 계정생성 성공

	DST_CHECKING_GAME_SERVER,		// 게임 서버가 점검중입니다
	DST_CHECKING_COMMUNITY_SERVER,	// 커뮤니티 서버가 점검중입니다

	DST_REGEN_PLAYER_MAINWORLD,		//"캐릭터가 기절 했습니다.[br][br][font size = "9" color = "ffdb47"]이동하기 : 포포의 돌로 순간 이동합니다."
	DST_REGEN_PLAYER_TLQ,			//"캐릭터가 기절 했습니다.[br][br][font size = "9" color = "ffdb47"]돌아가기 : 원래의 세계로 돌아갑니다.[br]재도전 : 현재 위치에서 다시 시작합니다.[/font][br]"
	DST_REGEN_PLAYER_TMQ,			//"캐릭터가 기절 했습니다.[br][br][font size = "9" color = "ffdb47"]돌아가기 : 원래의 세계로 돌아갑니다."
	DST_REGEN_PLAYER_PARTY_DUNGEON,	//"캐릭터가 기절 했습니다.[br][br][font size = "9" color = "ffdb47"]재도전 : 던전의 초기 위치로 이동합니다"
	DST_REGEN_PLAYER_DOJO,			//"캐릭터가 기절 했습니다.[br][br][font size = "9" color = "ffdb47"]재도전 : 도장의 초기 위치로 이동합니다."

	DST_GAME_EXIT,					// 게임을 종료하시겠습니까
	DST_CHAR_SERVER_EXIT,			// 캐릭터 선택화면으로 나가시겠습니까?
	DST_PLEASE_RETRY,				// 데이터를 처리중이니 잠시 뒤에 다시 시도해 주십시요

	// PrivateShop Message
	DST_PRIVATESHOP_MSG_SALE_ITEM,			// 팔려는 물품이 없습니다. 물품을 등록해야 합니다.
	DST_PRIVATESHOP_MSG_SALE_OPEN,			// 개인 상점을 열고 아이템 판매를 시작합니다.
	DST_PRIVATESHOP_MSG_SALE_CLOSE,			// 개인 상점을 닫고 아이템 판매를 중지합니다.
	DST_PRIVATESHOP_MSG_ITEM_NOT_REG,		// 개인 상점에 등록할 수 없는 아이템 입니다.

	DST_PRIVATESHOP_MSG_SHOPNAME_SLANGDER,	// 상점 이름에 욕설이 포함되어 있습니다. 다시 작성하여 주십시오.
	DST_PRIVATESHOP_MSG_NOTICE_SLANGDER,	// 공지 내용에 욕설이 포함되어 있습니다. 다시 작성하여 주십시오.
	DST_PRIVATESHOP_MSG_NOTICE_BUFFEROVER,  // 공지 내용을 초과하였습니다.
	DST_PRIVATESHOP_MSG_BUY_PROCESS,		// 마우스 오른쪽 클릭으로 아이템을 카트에 등록할 수 있습니다.

	// MailSystem Message
	DST_MAILSYSTEM_MSG_SEND_SUCCEED,			// %s님에게 메일을 보냈습니다.
	DST_MAILSYSTEM_MSG_ATTACH_ZENNY,			// 송금 및 청구 가능한 금액은 %u 제니 이하 입니다.
	DST_MAILSYSTEM_MSG_PLEASE_TARGETNAME,		// 받는 사람을 입력해 주세요.
	DST_MAILSYSTEM_MSG_PLEASE_MAILCONTENT,		// 본문을 입력해 주세요.
	DST_MAILSYSTEM_MSG_MAIL_NOT_LOCK,			// 청구된 금액이 있는 메일은 잠금 설정할 수 없습니다.
	DST_MAILSYSTEM_MSG_RECEIVE_ITEM,			// %s 아이템을 수령했습니다.
	DST_MAILSYSTEM_MSG_RECEIVE_ZENNY,			// %u 제니를 수령했습니다.
	DST_MAILSYSTEM_MSG_RECEIVE_ZENNY_REQ,		// %u 제니를 %s님에게 송금했습니다.
	DST_MAILSYSTEM_MSG_RECEIVE_ACCEPT_MAIL,		// %s님에게 수령 확인 답장을 보냈습니다.
	DST_MAILSYSTEM_MSG_SEND_BASIC,				// 우편이용료 %u 제니를 지불하고 메일을 보내시겠습니까
	DST_MAILSYSTEM_MSG_SEND_ITEM,				// 우편이용료 %u 제니를 지불하고 메일을 보내시겠습니까?\n(%s 아이템을 같이 보냅니다)
	DST_MAILSYSTEM_MSG_SEND_ZENNY,				// 우편이용료 %u 제니를 지불하고 메일을 보내시겠습니까?\n(%u 제니를 같이 보냅니다)
	DST_MAILSYSTEM_MSG_SEND_ITEM_ZENNY,			// 우편이용료 %u 제니를 지불하고 메일을 보내시겠습니까?\n(%s 아이템과 %u 제니를 같이 보냅니다)
	DST_MAILSYSTEM_MSG_SEND_ITEM_ZENNY_REQ,		// 우편이용료 %u 제니를 지불하고 메일을 보내시겠습니까?\n(%s 아이템을 보내고 %u 제니를 청구합니다)
	DST_MAILSYSTEM_MSG_SEND_ZENNY_REQ,			// 우편이용료 %u 제니를 지불하고 메일을 보내시겠습니까?\n(%u 제니를 청구합니다)
	DST_MAILSYSTEM_MSG_AWAY_ON,					// 부재중으로 설정하고 앞으로 오는 메일을 수신하지 않겠습니까?
	DST_MAILSYSTEM_MSG_AWAY_OFF,				// 부재중 설정을 해제하고 메일을 수신하겠습니까?
	DST_MAILSYSTEM_MSG_ATTACH_ITEM_NOT,			// 첨부물품으로 등록 할 수 없는 아이템 입니다.

	DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ACCEPT,		// %s 아이템을 받으시겠습니까?
	DST_MAILSYSTEM_MSG_RECEIVE_ZENNY_ACCEPT,	// %u 제니를 받으시겠습니까?
	DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ZENNY_ACCEPT,		// %u제니와 %s 아이템을 받으시겠습니까?
	DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ZENNY_REQ_ACCEPT,	// 메일에 첨부된 %s 아이템을 받기 위해서 상대방이 청구한 %u 제니를 지불하시겠습니까?
	DST_MAILSYSTEM_MSG_RECEIVE_ZENNY_REQ_ACCEPT,		// %u 제니를 지불하시겠습니까?
	DST_MAILSYSTEM_MSG_DELETE,					// 메일을 삭제하시겠습니까?
	DST_MAILSYSTEM_MSG_RETURN,					// 메일을 반송하시겠습니까? 

	DST_MAILSYSTEM_MSG_SELECT_REQ_OR_SEND,		// 먼저 송금 또는 청구를 선택해야 합니다.

	DST_MAILSYSTEM_MSG_RETRUN_MAIL,				// %s님이 수신을 거부하고 반송시킨 메일입니다.
	DST_MAILSYSTEM_MSG_REPLY_MAIL_ITEM,			// %s님이 %s 아이템을 수령했습니다.
	DST_MAILSYSTEM_MSG_REPLY_MAIL_ZENNY,		// %s님이 %u 제니를 수령했습니다.
	DST_MAILSYSTEM_MSG_REPLY_MAIL_ITEM_ZENNY,	// %s님이 %s 아이템과 %u 제니를 수령했습니다.
	DST_MAILSYSTEM_MSG_REPLY_MAIL_ITEM_ZENNY_REQ,// %s님이 %s 아이템을 수령하고 청구액 %u 제니를 송금했습니다.
	DST_MAILSYSTEM_MSG_REPLY_MAIL_ZENNY_REQ,	// %s님이 청구액 %u 제니를 송금했습니다.

	DST_MAILSYSTEM_MSG_MAIL_CONTENT_SLANGDER,	// 메일 본문에 욕설이 포함되어 있습니다. 다시 작성하여 주십시오.
	DST_MAILSYSTEM_MSG_MAIL_CONTENT_BUFFEROVER,	// 메일 내용을 초과하였습니다.

	DST_MAILSYSTEM_MSG_NOTFIND_SELECTED_MAIL,	// 선택한 메일이 없습니다.
	DST_MAILSYSTEM_TOOLTIP_DELETE_BUTTON,		// 선택한 메일들을 삭제합니다.

	// tutorial message
	DST_TUTORIAL_ASK_BEGIN,					// 튜토리얼을 시작하시겠습니까?(MsgBox, 튜토리얼 시작 물어보기)
	DST_TUTORIAL_WAITING_NUMBER,			// 튜토리얼 입장까지 %d명 남았습니다(MsgBox, 튜토리얼 입장 대기중)
	DST_TUTORIAL_ASK_END,					// 튜토리얼을 종료하시겠습니까?(MsbBox, 튜토리얼 진행중 튜토리얼을 중단할지 물어보기)
	DST_TUTORIAL_CAN_NOT_WORK,				// 지금은 할 수 없습니다(튜토리얼 중 유저가 Lock 상태인 동작을 하려고 할 때)

	// TMQ message
	DST_TMQ_BALLOON_LOTTERY_WAITING,		// 타임머신 퀘스트 추첨 대기중(TMQ Side View)
	DST_TMQ_BALLOON_HOLDING_A_LOTTERY,		// 타임머신 퀘스트 추첨중입니다(TMQ Side View)
	DST_TMQ_BALLOON_PLEASE_WAIT,			// 잠시만 기다려 주십시요(TMQ Side View)
	DST_TMQ_BALLOON_ASK_RETRY_LOTTERY,		// 다음 타임머신 퀘스트를 예약하시겠습니까?(TMQ Side View)
	DST_TMQ_BALLOON_RETRY_LOTTERY,			// 타임머신 퀘스트 재신청(TMQ Side View)
	DST_TMQ_BALLOON_READY_FOR_TMQ,			// 타임머신 퀘스트 준비를 해주십시요(TMQ Side View)
	DST_TMQ_BALLOON_CONGRATULATION,			// 축하합니다(TMQ Side View)
	DST_TMQ_BALLOON_SUCCESS_LOTTERY,		// 신청하신 타임머신 퀘스트에 당첨되었습니다(TMQ Side View)
	DST_TMQ_BALLOON_I_AM_SORRY,				// 죄송합니다(TMQ Side View)
	DST_TMQ_BALLOON_FAIL_LOTTERY,			// 신청하신 타임머신 퀘스트에 당첨되지 않았습니다(TMQ Side View)
	DST_TMQ_BALLOON_CAN_TELEPORT,			// 타임머신 퀘스트 진입 %d초 전까지 등록한 로보트 근처로 단 한번 텔레포트 할 수 있습니다(TMQ Side View)

	DST_TMQ_BALLOON_CANNOT_TELEPORT,		// 타임머신 퀘스트 진입 직전입니다. 텔레포트 하실 수 없습니다(TMQ Side View)
	DST_TMQ_BALLOON_LEAVE_MEMVER,			// %s님이 퇴장하였습니다(TMQ 대기중, 진행중 한 놈 나갔다)(TMQ Side View)

	DST_TMQ_STATUS_MUST_SELECT_ADDITION_REWARD, // 선택보상을 고르지 않았습니다(TMQ 보상중 선택보상이 있는데도 고르지 않고 확인버튼을 눌렀을때)
	DST_TMQ_STATUS_CAN_SEE_SENARIO,			 // TMQ 시나리오를 볼 수 있습니다(TMQ 시나리오 버튼 툴팁)
	DST_TMQ_STATUS_CAN_SEE_STAGE_INFO,		 // TMQ 스테이지 정보를 볼 수 있습니다(TMQ 스테이지 버튼 툴팁)
	DST_TMQ_STATUS_EXPLAIN_POINT,			 // Point 게이지가 100%차게 되면 TMQ 쿠폰으로 바뀝니다(TMQ 게이지 툴팁)
	DST_TMQ_STATUS_EXPLAIN_COUPON,			 // TMQ 쿠폰을 점쟁할매 바바에게 가져가면 보상을 받을 수 있습니다(TMQ 쿠폰 투팁)
	DST_TMQ_STATUS_EXPLAIN_INDICATOR,		 // TMQ에서 받은 여러가지 퀘스를 볼 수 있습니다(TMQ 퀘스트 인디케이트 버튼 툴팁)

	DST_TMQ_RESULT_NARRATION,				// TMQ를 클리어했습니다(TMQ 보상창에서 나레이션창의 대사)

	// guild message
	DST_GUILD_ENTRUST_MASTER,				// 최고사범의 모든 권한을 %s님에게 맡기시겠습니까?(MsgBox, 길드장 넘길지 여부)
	DST_GUILD_APPOINT_SECOND_MASTER,		// %s님을 유파의 사범으로 임명하시겠습니까?(MsgBox, 부길드장 임명 여부)
	DST_GUILD_DISMISS_SECOND_MASTER_REQ,	// %s님의 사범 직위를 박탈하시겠습니까?(MsgBox, 부길드장 직위해제 여부)
	DST_GUILD_KICK_OUT,						// %s님을 유파에서 추방하시겠습니까?(MsgBox, 길드원 추방 여부)
	DST_GUILD_LEAVE_ASK,					// 길드를 탈퇴하시겠습니까?(MsgBox, 자신이 길드 탈퇴 여부)
	DST_GUILD_DO_MAKE,						// 길드를 만드시겠습니까?(MsgBox, 길드 생성 여부)
	DST_GUILD_DO_DISSOLVE,					// 길드를 해산하시겠습니까?(MsgBox, 길드 해산 여부)
	DST_GUILD_NOTICE_DISSOLVE,				// %d년 %d월 %d일(%s) %d시 %d분 길드 해산이 예약되었습니다(길드 해산 공지)
	DST_GUILD_CANCEL_DISSOLVE,				// 길드 해산을 취소하시겠습니까?(MsgBox, 길드 해산 취소 여부)
	DST_GUILD_CANCELED_DISSOLVE,			// 길드 해제가 취소되었습니다(길드 해산 취소 공지)

	DST_GUILD_CREATE,						// 길드를 만들었습니다(Notify, 길드장에게)
	DST_GUILD_NAME_RULE,					// 길드 이름은 2자에서 16자까지만 사용할 수 있습니다(길드를 만들거나 이름을 변경할 때)
	DST_GUILD_CREATE_NFY,					// %s 길드가 생성되었습니다(Notify, 길드원에게)
	DST_GUILD_INVITE,						// %s님을 길드에 초대하였습니다(Notify, 길드장에게)
	DST_GUILD_INVITE_RES,					// 길드에 가입되었습니다(Notify, 길드원에게)
	DST_GUILD_INVITE_REQ,					// %s 길드에 가입하시겠습니까?(Notify, 일반 PC에게)
	DST_GUILD_DECLINE,						// %s님이 길드 가입을 거절하였습니다(Notify, 길드장에게)
	DST_GUILD_EXPIRED,						// %s님 길드 가입에 응답하지 않았습니다(Notify, 길드장에게)
	DST_GUILD_NEW_MEMBER,					// %s님이 길드에 가입하였습니다(Notify, 모든 길드원에게)
	DST_GUILD_LEAVE_MEMBER,					// %s님이 길드를 탈퇴하였습니다(Notify, 모든 길드원에게)
	DST_GUILD_KICKOUT_MEMBER,				// %s님이 길드에서 추방되었습니다(Notify, 모든 길드원에게)
	DST_GUILD_SECOND_MASTER_APPOINTED,		// %s님이 부길드장이 되었습니다(Notify, 모든 길드원에게)
	DST_GUILD_CHANGE_MASTER,				// %s님이 길드장이 되었습니다(Notify, 모든 길드원에게)

	DST_GUILD_ONLINE_MEMBER,				// %s님이 접속했습니다(Notify, 모든 길드원에게)
	DST_GUILD_CLASS_CHANGE_MEMBER,			// %s님이 %s 클래스가 되었습니다(Notify, 모든 길드원에게)
	DST_GUILD_LEVEL_CHANGE_MEMBER,			// %s님이 %d 레벨이 되었습니다(Notify, 모든 길드원에게)
	DST_GUILD_DISMISS_SECOND_MASTER,		// %s님이 부길드 마스터에서 일반 길드맴버가 되었습니다(Notify, 모든 길드원에게)
	DST_GUILD_DISBAND,						// 길드가 해산되었습니다(Notify, 모든 길드원에게)
	DST_GUILD_NOT_JOIN,						// 길드에 가입하지 않았습니다(가입한 길드가 없는데 탈퇴 버튼을 눌렀다)
	DST_GUILD_EMPTY,						// 미가입

	DST_GUILD_ADD_FUNCTION,					// '%s' 길드 기능을 추가하시겠습니까?(MsgBox, 길드장에게)
	DST_GUILD_ADD_FUNCTION_NOTIFY,			// 길드에 기능이 변경되었습니다(Notify, 모든 길드원에게)
	DST_GUILD_CHANGE_GUILD_NAME,			// 바꾸실 길드 이름을 입력해주십시요(MsgBox, 길드장에게)
	DST_GUILD_CHANGED_GUILD_NAME,			// 길드 이름이 변경되었습니다(Notify, 모든 길드원에게)
	DST_GUILD_CHANGED_GUILD_NOTICE_SUCCESS,	// 길드 공지를 발송하였습니다(자신에게)
	DST_GUILD_CHANGED_GUILD_NOTICE,			// 새로운 길드 공지가 있습니다(Notify, 모든 길드원에게)
	DST_GUILD_CREATE_GUILD_EMBLEM,			// 길드 문장이 생성되었습니다(Notify, 모든 길드원에게)
	DST_GUILD_CHANGE_GUILD_EMBLEM,			// 길드 문장이 변경되었습니다(Notify, 모든 길드원에게)
	DST_GUILD_CONTRIBUTION_ZENNY,			// 길드를 위해 %d 제니를 기부하시겠습니까?(MsgBox, 제니를 기부할 것인지 물어볼 때)
	DST_GUILD_CONTRIBUTION_ZENNY_NOTIFY,	// 제니를 기부하였습니다(Notify, 자신에게)
	DST_GUILD_CONTRIBUTION_ZENNY_OTHERS,	// %s님이 %d 제니를 기부하였습니다(Notify, 모든 길드원에게)
	DST_GUILD_EMBLEM_INFO,					// 길드 문장이 아직 없습니다(guild 다이얼로그에서 엠블렘 버튼툴팁)
	DST_GUILD_EMBLEM_MODIFY_INFO,			// 길드 문장 수정(guild 다이얼로그에서 엠블렘 수정 버튼툴팁)
	DST_GUILD_EMBLEM_EXPLAIN_1,				// 각 이미지에서 3종류를 조합해주세요(길드 엠블렘 만들기 창 메세지1)
	DST_GUILD_EMBLEM_EXPLAIN_2,				// 각 이미지에서 3종류를 조합해주세요(길드 엠블렘 만들기 창 메세지2)
	DST_GUILD_NOT_SELECT_EMBLEM,			// 문장의 요소를 전부 선택하지 않았습니다(선택한 엠블렘을 서버에 보내서 확인하려고 할때)

	DST_GUILD_WAREHOUSE_CAN_NOT_DISCARD,	// 길드 창고의 아이템은 버릴 수 없습니다

	// guile warehouse
	DST_GUILD_WAREHOUSE_ZENNY_DRAW_OUT,		// %d 제니를 출금하였습니다(길드 창고에 저금하였다)
	DST_GUILD_WAREHOUSE_SAVE_ZENNY,			// %d 제니를 저금하였습니다(길드 창고서 출금하였다)

	// Dialog mode message
	DST_DIALOG_MODE_ITEM_REPAIR,			// 손상된 아이템을 선택하십시요(아이템 수리 모드에서)
	DST_DIALOG_MODE_ITEM_IDENTIFICATION,	// 아이템을 감정중입니다(아이템 감정중 다른 작업을 하려고 할 때)
	DST_DIALOG_MODE_NARRATION,				// 이벤트가 진행중입니다(나레이션 모드중 다른 작업을 하려고 할 때)
	DST_DIALOG_MODE_CHAT_RESIZING,			// 채팅창의 크기를 변경중입니다(채팅 크기 변경중 다른 작업을 하려고 할 때)

	// World concept messsage
	DST_WORLD_CONCEPT_TUTORIAL,				// 지금은 튜토리얼을 진행중입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_TMQ,					// 지금은 타임머신 퀘스트를 진행중입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_RANK_BATTLE,			// 지금은 랭크배틀을 진행중입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_BUDOKAI,				// 지금은 천하제일 무도회를 진행중입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_PARTY_DUNGEON,		// 지금은 파티 던전을 진행중입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_TLQ,					// 지금은 타임립 퀘스트를 진행중입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_DOJO_SCRAMBLE,		// 지금은 도장 쟁탈전을 진행중입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)

	DST_WORLD_CONCEPT_FREEBATTLE,			// 지금은 대결중입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_NPC_COMMUNICATION,	// 지금은 NPC와 대화중입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_TRADE,				// 지금은 거래중입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_PLAY_DRAGONBALL,		// 지금은 용신을 불러내고 있습니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_PLAY_PRIVATESHOP,		// 지금은 개인상점 판매중 입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_PLAY_PRIVATESHOP_VISITOR, // 지금은 개인상점 이용중 입니다(월드컨셉에 의해 다른 작업을 할 수 없을 때)
	DST_WORLD_CONCEPT_PLAY_DIRECT,				// 지금은 연출중입니다.(월드컨셉에 의해 다른 작업을 할 수 없을 때)

	// Notify Get message
	DST_NOTIFY_GET_ITEM,						// %s 아이템을 획득하였습니다(아이템을 얻었다)
	DST_NOTIFY_GET_ZENNY,						// %d 제니를 획득하였습니다(제니를 얻었다
	DST_NOTIFY_GET_ZENNY_AND_BONUS,				// %d(+%d) 제니를 획득하였습니다(제니를 얻었다
	DST_NOTIFY_GET_UNIDENTIFIED_ITEM,			// 미확인 아이템을 획득하였습니다.
	DST_NOTIFY_LEARN_SKILL,						// %s 스킬을 배웠습니다.
	DST_NOTIFY_UPGRADE_SKILL,					// %s 스킬을 업그레이드 했습니다.
	DST_NOTIFY_GAIN_EXP,						// EXP를 %u 획득 하였습니다. 
	DST_NOTIFY_GAIN_EXP_AND_BONUS,				// EXP를 %u(+%u) 획득 하였습니다. 
	DST_NOTIFY_GET_QUEST_ITEM,					// 퀘스트아이템( %s )을 획득하였습니다. 
	DST_NOTIFY_GET_BUFF,						// %s 버프를 얻었습니다.
	DST_NOTIFY_GET_REPUTATION,					// %u 명성을 얻었습니다.
	DST_NOTIFY_CHANGE_CLASS,					// 직업 변경 ( %s -> %s )
	DST_NOTIFY_LEVEL_UP,						// 레벨업( %u -> %u )

	// Quest GiveUp Message
	DST_QUEST_GIVEUP_MESSAGE,				// %s 퀘스트를 포기하시겠습니까? (메세지박스)
	DST_QUEST_GAVEUP_MESSAGE,				// %s 퀘스트를 포기하였습니다. (노티파이)
	DST_QUEST_SUCCESS_DELIVERY,				// %s 퀘스트 아이템 배달 완료.
	DST_QUEST_SUCCESS_MOBHUNT,				// %s 퀘스트 몬스터 사냥 완료.
	DST_QUEST_SUCCESS_COLLECTION,			// %s 퀘스트 아이템 수집 완료.
	DST_QUEST_SUCCESS_CUSTOM,				// %s 퀘스트 임무 완료.
	DST_QUEST_SUCCESS_VISIT,				// %s 퀘스트 방문임무 완료.
	DST_QUEST_FAILED,						// %s 퀘스트를 실패하였습니다.
	DST_QUEST_CLEAR,						// %s 퀘스트 클리어. 보상을 받으세요.
	DST_QUEST_COMPLETE,						// %s 퀘스트 완수.

	// Quest share message
	DST_QUEST_SHARE_TRY,						// %s 퀘스트를 파티원들에게 공유 했습니다.
	DST_QUEST_SHARE_ALREADY_PROGRESS,			// [퀘스트 공유] < %s > 가 현재 진행 중입니다.
	DST_QUEST_SHARE_HAS_BEEN,					// [퀘스트 공유] < %s > 는 이미 진행한 적이 있어 공유 받을 수 없습니다.
	DST_QUEST_SHARE_OTHER_QUEST_PROGRESS,		// [퀘스트 공유] < %s > 가 현재 인터페이스를 사용 중입니다. 인터페이스를 닫은 후 다시 시도해주세요.
	DST_QUEST_SHARE_MAX_OVERFLOW,				// [퀘스트 공유] 진행 가능한 최대 퀘스트 개수를 초과해 공유 받을 수 없습니다.
	DST_QUEST_SHARE_COND_COMPLETE,				// [퀘스트 공유] 연계 퀘스트를 진행하지 않아 < %s > 를 공유 받을 수 없습니다.
	DST_QUEST_SHARE_COND_NPC_REGION,			// [퀘스트 공유] < %s > 를 공유 받기 위해서는 특정 NPC 근처에 있어야 됩니다.
	DST_QUEST_SHARE_COND_ITEM,					// [퀘스트 공유] < %s > 를 공유 받기 위해서는 아이템을 가지고 있어야 합니다.
	DST_QUEST_SHARE_COND_QITEM,					// [퀘스트 공유] < %s > 를 공유 받기 위해서는 퀘스트 아이템을 가지고 있어야 합니다.
	DST_QUEST_SHARE_COND_LEVEL,					// [퀘스트 공유] 레벨이 맞지 않아 < %s > 를 공유 받을 수 없습니다.
	DST_QUEST_SHARE_COND_PC_CLASS,				// [퀘스트 공유] 직업이 맞지 않아 < %s > 를 공유 받을 수 없습니다.
	DST_QUEST_SHARE_COND_PC_RACE,				// [퀘스트 공유] 종족이 맞지 않아 < %s > 를 공유 받을 수 없습니다.
	DST_QUEST_SHARE_COND_EQUIP_ITEM,			// [퀘스트 공유] < %s > 를 공유 받기 위해서는 특정 아이템을 착용하고 있어야 합니다.
	DST_QUEST_SHARE_COND_IN_WORLD,				// [퀘스트 공유] < %s > 를 공유 받기 위해서는 특정 지역에 있어야 합니다.

	// Quest Indicator Message
	DST_QUEST_INDICATOR_CANNOT_ADD_INDICATE_ITEM,	// 더이상 퀘스트 알리미에 등록할 수 없습니다.

	// Quest 
	DST_QUEST_LINK_ACTION_ERROR,			// 잠시후 다시 시도해주세요

	// user trade message
	DST_TRADE_REQUEST_START,				// %s님에게 거래를 요청하였습니다(거래를 요청한 사람에게)
	DST_TRADE_YOUR_BAG_NOT_ENOUGH,			// 당신의 가방에 빈 공간이 모자랍니다(거래중 사용할 수 없는 카트의 슬롯의 인포창 메세지)
	DST_TRADE_THE_OTHERS_BAG_NOT_ENOUGH,	// 상대의 가방에 빈 공간이 모자랍니다(거래중 사용할 수 없는 카트의 슬롯의 인포창 메세지)
	DST_TRADE_ACCEPT_TRADE,					// %s님의 거래 요청에 응하시겠습니까?(MsgBox, 자신이 거래 요청을 받았다)
	DST_TRADE_OTHER_CANCLE_TRADE,			// 상대방이 거래를 종료하였습니다(거래중 자신에게 온 메세지)

	// lobby message
	DST_LOBBY_MUST_CHOICE_CHANNEL,			// 채널을 선택하십시요
	DST_LOBBY_MUST_CHOICE_CHAR,				// 캐릭터를 선택하십시요
	DST_LOBBY_MUST_CHOICE_SERVER,			// 서버를 선택하십시요
	DST_LOBBY_DEL_CHARACTER,				// 캐릭터를 지우시겠습니까
	DST_LOBBY_PLEASE_CHECK_DEL_AND_RECOVERY_DATA,	// 캐릭터 이름을 정확히 입력해 주십시요(캐릭터 삭제, 복구시 잘못 입력했을 때,로비,OK버튼)
	DST_LOBBY_ASK_RENAME,					// 새로운 캐릭터 이름을 입력해 주십시요(캐릭터 이름을 변경할 때,Inputbox,로비,OK버튼,Cancel버튼)
	DST_LOBBY_NO_RELPY_SERVER,				// 서버로부터 응답이 없습니다
	DST_LOBBY_CAN_NOT_USE_THE_WORD,			// 사용할 수 없는 단어가 있습니다	
	DST_LOBBY_EMPTYNAME,					// 이름을 입력하여 주십시요
	DST_LOBBY_RESERVATED_CHAR,				// 삭제 대기중인 캐릭터입니다(삭제 대기중인 캐릭터로 게임을 시작하려고 할 때)
	DST_LOBBY_CAN_NOT_USE_CHAR_UNTIL_RENAME,// 이름을 수정하기 전까지는 사용할 수 없는 캐릭터입니다(서버 정책에 의해 사용할 수 없는 캐릭터 이름으로 변했을 때, 로비 메세지, OK버튼)
	DST_LOBBY_COMPLETE_RENAME,				// 캐릭터의 이름이 %s에서 %s으로 바뀌었습니다(캐릭터 이름을 바꾸기 성공했을 때, 로비 메세지, OK버튼)
	DST_LOBBY_RECOVERY_CHAR,				// 캐릭터를 복구하시겠습니까(삭제 대기중인 캐릭터를 복구하려고 할 때)
	DST_LOBBY_WAIT_GAME_SERVER_CONNECT,		// 당신은 %d번째 대기자입니다(로비에서 게임으로 들어가려 할 때 대기순번이 떳다)

	// warehouse message
	DST_WAREHOUSE_WANT_YOU_BUY,				// 창고(%d 제니)를 구입하시겠습니까?(MsgBox, 창고 구입 여부)
	DST_WAREHOUSE_WANT_YOU_BUY_COMMON,		// 공유 창고(%d 제니)를 구입하시겠습니까?(MsgBox, 공유 창고 구입 여부)
	DST_WAREHOUSE_FULL,						// 창고가 가득 찼습니다(창고로 아이템을 옮기려고 할 때)
	DST_WAREHOUSE_SUCCESS_BUY,				// 창고를 구입하였습니다(창고 구입 성공)

	// scouter message
	DST_SCOUTER_NEED_SCOUTER,					// 스카우터가 필요합니다(스카우터 측정하려 할 때)
	DST_SCOUTER_OUT_OF_ORDER_SCOUTER,			// 스카우터가 고장났습니다(스카우터 측정하려 할 때)
	DST_SCOUTER_MUST_CLEAR_SCOUTER,				// 스카우터를 옮기려면 먼저 비워야합니다(스카우터를 옮기려고 할 때)
	DST_SCOUTER_FULL,							// 스카우터에 파츠가 가득 찼습니다(스카우터에 파츠를 장착하려고 할 때)
	DST_SCOUTER_CAN_NOT_MOVE_PARTS_ON_MENU,		// 스카우터 메뉴가 작동중에는 파츠를 변경할 수 없습니다(스카우터에 장착된 파츠를 옮기려고 할 때)
	DST_SCOUTER_EXPLAIN_SLOT,					// %s키로 스카우터 정보창을 열 수 있습니다(스카우터 슬롯 설명)
	DST_SCOUTER_WORKING,						// 스카우터를 사용중입니다(스카우터 사용중 스카우터를 장착 해제/버리기하려고 할 때)
	DST_SCOUTER_QUEST_SEARCH_NOT_QUEST,			// 스카우터의 검색 범위에 퀘스트를 가진 NPC를 찾을 수 없습니다(퀘스트 서치중)
	DST_SCOUTER_SUCCESS_FIND_HIDDEN_OBJECT,		// 숨겨진 물체를 %d개 찾았습니다(스카우터로 숨겨진 물체 찾기 기능을 썻을 때)
	DST_SCOUTER_NOT_EXIST_HIDDEN_OBJECT,		// 근처에 숨겨진 물체가 없습니다(스카우터로 숨겨진 물체 찾기 기능을 썻을 때)

	// party message
	DST_PARTY_CREATE,							// 파티를 만드시겠습니까?(MsgBox, 파티를 만들 때)
	DST_PARTY_NAME_RULE,						// 파티 이름은 2자에서 16자까지만 사용할 수 있습니다(파티를 만들려고 할 때)
	DST_PARTY_INVITE_SUCCESS,					// s님을 파티에 초대하였습니다(Notify, 파티장에게)
	DST_PARTY_PARTY_INVITE,						// %s 파티에 가입하시겠습니까?(MsgBox, 일반 PC에게)
	DST_PARTY_PARTY_INVITE_WITHOUT_PARTY,		// %s 님의 파티에 가입하시겠습니까?(MsgBox, 파티에 가입하지 않은 PC가 일반 PC에게)
	DST_PARTY_NEW_MEMBER,						// %s님이 파티에 들어오셨습니다(Notify, 모든 파티원에게)
	DST_PARTY_INVITE_DECLINE,					// %s님이 초대를 거절하였습니다(Notify, 파티장에게)
	DST_PARTY_LEAVE_ASK,						// 파티를 탈퇴하시겠습니까?(MsgBox, 자신에게)
	DST_PARTY_LEAVE,							// %s님이 파티에서 탈퇴하였습니다(Notify, 모든 파티원에게)
	DST_PARTY_CHANGE_LEADER,					// %s님이 파티장이 되었습니다(Notify, 모든 파티원에게)
	DST_PARTY_CHANGE_ITEM_DIVISION,				// 아이템 분배방식이 %s로 변경되었습니다(Notify, 모든 파티원에게)
	DST_PARTY_CHANGE_ZENNY_DIVISION,			// 제니 분배방식이 %s로 변경되었습니다(Notify, 모든 파티원에게)
	DST_PARTY_NOTIFY_DIVISION_INFO,				// 분배 방식을 변경한 후 이 버튼을 눌러 적용하십시요(Notify, 버튼의 툴팁)
	DST_PARTY_NOTIFY_WAIT_DIVISION,				// 분배 방식을 변경하려면 잠시 기다리십시요(Notify, 파티장에게)

	DST_PARTY_GET_ITEM,							// %s님이 %s 획득(Notify, 모든 파티원에게)
	DST_PARTY_GET_ZENNY,						// %d중 %d 제니를 분배받았습니다(Notify, 자신에게)
	DST_PARTY_GET_ZENNY_AND_BONUS,				// %d중 %d(+%d) 제니를 분배받았습니다(Notify, 자신에게)
	DST_PARTY_GET_ZENNY_OTHER,                  // %s님이 %d 제니를 획득하였습니다(Notify, 모든 파티원에게)
	DST_PARTY_GET_ZENNY_OTHER_AND_BONUS,		// %s님이 %d(+%d) 제니를 획득하였습니다(Notify, 모든 파티원에게)
	DST_PARTY_INVEN_ADD_ITEM,					// 파티 인벤토리에 %s 획득(Notify, 모든 파티원에게)
	DST_PARTY_INVEN_ADD_UNDEFINED_ITEM,			// 파티 인벤토리에 미확인 아이템을 획득하였습니다(Notify, 모든 파티원에게)
	DST_PARTY_INVEN_ADD_UNDEFINED_ITEM_OTHER,	// %s님이 미확인 아이템을 획득하였습니다(Notify, 모든 파티원에게)
	DST_PARTY_INVEN_CHANGE_ITEM_GRADE,			// 파티 인벤토리에 보관되는 아이템의 등급이 %s이상으로 변경되었습니다(Notify, 모든 파티원에게)
	DST_PARTY_INVEN_OCTION_LOCK,				// 파티 인벤토리의 경매가 중지 되었습니다(Notify, 모든 파티원에게)
	DST_PARTY_INVEN_OCTION_UNLOCK,				// 파티 인벤토리의 경매가 시작 되었습니다(Notify, 모든 파티원에게)
	DST_PARTY_INVEN_OCTION_RESET,				// 파티 인벤토리의 경매 정보가 초기화 되었습니다(Notify, 모든 파티원에게)
	DST_PARTY_INVEN_OCTION_SUCCESSFUL_BID,		// 파티 인벤토리의 %s 아이템의 경매가 %s님에게 낙찰 되었습니다(Notify, 모든 파티원에게 경매 낙찰 시)
	DST_PARTY_INVEN_OCTION_SUCCESSFUL_BID_WITH_UNIDENTIFY,	// 파티 인벤토리의 미확인 아이템의 경매가 %s님에게 낙찰 되었습니다(Notify, 모든 파티원에게 경매 낙찰시)
	DST_PARTY_INVEN_GIVE_BY_LEADER,				// 파티 인벤토리의 %s 아이템을 파티장이 %s님에게 분배 하였습니다(Notify, 모든 파티원에게 파티장이 분배시)
	DST_PARTY_INVEN_GIVE_BY_LEADER_WITH_UNIDENTIFY,	// 파티 인벤토리의 미확인 아이템을 파티장이 %s님에게 분배 하였습니다(Notify, 모든 파티원에게 파티장이 분배시)
	DST_PARTY_INVEN_START_DICE,					// 파티 인벤토리의 아이템 분배를 위한 주사위 굴리기가 시작 되었습니다(Notify, 모든 파티원에게 주사위 분배 시작시)
	DST_PARTY_INVEN_DICE_WINNER,				// 파티 인벤토리의 %s 아이템을 %s님이 차지 하였습니다(Notify, 모든 파티원에게 주사위 분배시)
	DST_PARTY_INVEN_DICE_WINNER_WITH_UNIDENTIFY,// 파티 인벤토리의 미확인 아이템을 %s님이 차지 하였습니다(Notify, 모든 파티원에게 주사위 분배시)

	DST_PARTY_NEED_ZENNY,						// %d 제니가 필요합니다(파티 부적을 발동하려고 할 때)
	DST_PARTY_DECLARE_ZENNY,					// 현재 %d 제니를 등록하였습니다(파티부적창에 제니를 등록했다)
	DST_PARTY_DECLARE_ZENNY_PART,				// %d 제니중 %d 제니를 등록하였습니다(파티부적창에 제니를 등록했다)
	DST_PARTY_TOOLTIP_ZENNY,					// %u 제니를 등록하였습니다(파티부적창 등록한 제니를 툴팁으로 보여준다)
	DST_PARTY_REG_VICTIM_ITEM,					// %s님이 %s 부적을 등록했습니다(Notify, 자신(파티장)에게)
	DST_PARTY_UNREG_VICTIM_ITEM,				// %s 부적을 해제하였습니다(Notify, 자신(파티장)에게)
	DST_PARTY_ACTIVATE_CHARM,					// %s 부적이 발동되었습니다(Notify, 모든 파티원에게)
	DST_PARTY_STOP_CHARM,						// %s 부적이 해제되었습니다(Notify, 모든 파티원에게)
	DST_PARTY_RELEASE_CHARM,					// 부적의 사용을 중지하시겠습니까?(MsgBox, 자신에게)
	DST_PARTY_CHARMSLOT_NOT_EMPTY,				// 부적 슬롯이 비어있지 않습니다(Notify, 자신에게)
	DST_PARTY_CHARM_START_DICE,					// 파티 부적 주사위를 굴립니다(주사위 다이얼로그에, 모든 파티원에게 고급 파티참을 발동시)
	DST_PARTY_CHARM_START_DICE_RESULT_FAIL,		// 파티 부적 굴리기에 실패했습니다(Notify, 모든 파티원에게 고급 파티참을 발동시)

	DST_PARTY_DUNGEON_INITIALIZE,				// 파티 던전이 초기화되었습니다(파티장이 파티 던전을 초기화하였다)
	DST_PARTY_DUNGEON_CHANGE_NORMAL,			// 파티 던전의 난이도가 보통으로 변경되었습니다(파티장이 파티 던전의 상태를 변경하였다)
	DST_PARTY_DUNGEON_CHANGE_HARD,				// 파티 던전의 난이도가 어려움으로 변경되었습니다(파티장이 파티 던전의 상태를 변경하였다)

	// trade cart message
	DST_TRADECART_CANNOT_SELL_ITEM,				// 팔 수 없는 아이템입니다
	DST_TRADECART_NO_MORE_SLOT,					// 카트에 슬롯이 부족합니다

	// Dragonball Collection Message 
	DST_DBC_NOT_ENOUGH,						// 드래곤볼의 개수가 부족합니다.
	DST_DBC_NOT_KEYWORD,					// 주문을 입력하세요
	DST_DBC_NOT_SAME,						// 같은 타입의 드래곤볼이 아닙니다.
	DST_DBC_ALREADY_USE_ALTAR,				// 제단이 이미 사용중입니다.
	DST_DBC_DRAGONBALL_NOT_FOUND,			// 드래곤볼을 가방에서 찾을 수 없습니다.
	DST_DBC_NOT_DRAGONBALL,                 // 드래곤볼외의 아이템은 제단에 넣을수 없습니다
	DST_DBC_NOT_SAME_NUMBER,		        // 제단에 같은 숫자의 드래곤볼을 넣을 수 없습니다.

	// Skill
	DST_SKILL_USING_TRINER_DIALOG,	        // 스킬트레이너 창을 이용시에는 자신의 스킬창을 열 수 없습니다
	DST_SKILL_CANCEL,				        // 스킬 시전이 취소되었습니다.
    DST_SKILL_OCCLUSION_CHECK,              // 대상이 오브젝트에 가려져서 스킬을 사용할 수 없습니다.


	// Skill요구사항 ( 메세지는 인포윈도우에서도 처리함)
	DST_SKILL_RQ_LEARN_LEVEL,		// 스킬을 배우려면 Level을 올려야 한다. 
	DST_SKILL_RQ_LEARN_SKILL,		// 스킬을 배우려면 다른 스킬을 먼저 배워야한다.
	DST_SKILL_RQ_READY_LEARN_BY_SKILLMASTER,// 스킬마스터에게 스킬을 배울 수 있다.
	DST_SKILL_RQ_READY_LEARN_BY_SELF,// 스킬을 스스로 배울 수 있다.
	DST_SKILL_RQ_UPGRADE_LEVEL,		// 스킬을 업그레이드하기 위해 Level이 모지라다
	DST_SKILL_RQ_UPGRADE_SKILLEXP,	// 스킬을 업그레이드하기 위해 스킬EXP가 필요하다
	DST_SKILL_RQ_READY_UPGRADE,		// 스킬을 업그레이드 할 수 있다.
	DST_SKILL_RQ_MASTERED,			// 스킬을 마스터 했다.
	DST_SKILL_RQ_LEARNED,			// 스킬을 이미 배웠다.	
	DST_SKILL_RQ_LEARN_CLASS,		// 클래스가 달라서 배울수 없다.
	DST_SKILL_RQ_NOT_LEARN_CHANGECLASSRIGHT,	// 전직권한을 얻어야 획득 수 있다.

	// Skill 알리미					/// woosungs_test 20090730
	DST_SKILL_NOTYFY_UPGRADE_TITLE,	// 스킬 업그레이드 알리미
	DST_SKILL_NOTYFY_UPGRADE_ALARM,	// 업그레이드 가능한 스킬이 있으니 스킬창을 확인해 주세요
	DST_SKILL_NOTYFY_LEARN_MASTER_SKILL_TITLE,	// 스킬 습득 알리미
	DST_SKILL_NOTYFY_LEARN_MASTER_SKILL_ALARM,	// 새로운 스킬을 배울 수 있습니다 스킬창을 확인해 주세요 
	DST_SKILL_NOTYFY_LEARN_SELF_SKILL_TITLE,	// 스킬 습득 알리미
	DST_SKILL_NOTYFY_LEARN_SELF_SKILL_ALARM,	// 새로운 스킬을 배울 수 있습니다 스킬창을 확인해 주세요

	// 전투
	DST_BATTLE_CAN_NOT_ATTACK_TARGET,	// 공격할 수 없는 대상입니다(공격하려 할 때)

	// Item
	DST_ITEM_CONFIRM_DROP,			// %s 아이템을 버리시겠습니까?(MsgBox, 아이템을 버리려고 할 때)
	DST_STACKITEM_CONFIRM_DROP,		// %s 아이템 %d개를 버리시겠습니까?
	DST_ITEM_REMOTE_SELL,			// %s 아이템을 판매하시겠습니까?(MsgBox, 아이템을 원격판매 할 때)
	DST_STACKITEM_REMOTE_SELL,		// %s 아이템 %d개를 판매하시겠습니까?(MsgBox, 아이템을 원격판매 할 때)
	DST_ITEM_BAG_FULL,				// 가방에 아이템이 가득 찼습니다(아이템을 가방으로 옮기려고 할 때)
	DST_ITEM_CAN_NOT_MOVE_THERE,	// 그곳에 옮길 수 없습니다(옮길 수 없는 곳에 아이템을 옮기려고 했다)
	DST_ITEM_CAN_NOT_MOVE_TO_WAREHOUSE, // 창고로 옮길 수 없는 아이템입니다(창고로 옮길 수 없는 아이템을 옮기려고 했다)
	DST_ITEM_CAN_NOT_TRADE,			// 거래할 수 없는 아이템입니다(거래할 수 없는 아이템을 거래하려고 했다)
	DST_ITEM_CAN_NOT_SELL,			// 팔 수 없는 아이템입니다(팔 수 없는 아이템을 팔려고 했다)
	DST_ITEM_CAN_NOT_SAVE_COMMON_WAREHOUSE, // 공유창고에 저장할 수 없는 아이템입니다(공유창고에 저장할 수 없는 아이템을 저장하려고 했다)
	DST_ITEM_CAN_NOT_EQUIP_ZERO_DURABILITY,	// 내구도가 0인 아이템은 장착할 수 없습니다.
	DST_ITEM_CAN_NOT_EQUIP_EXPIRED_DURATION,// 사용기간이 만료되어 장착할 수 없습니다.
	DST_ITEM_CAN_NOT_MOVE_BAG_EXPIRED_DURATION,	// 가방의 사용기간이 만료되어 옮길 수 없습니다.
	DST_ITEM_CAN_NOT_MOVE_SCOUTER_EXPIRED_DURATION, // 스카우터의 사용기간이 만료되어 파츠를 장착할 수 없습니다. 	
	DST_ITEM_CAN_NOT_MOVE_NETPYSTORE,// 넷창고로 옮길 수 없는 상태입니다. 넷마블 피씨방을 이용해 어쩌고...
	DST_ITEM_CAN_NOT_MOVE_BASICBAG,	 // 기본가방은 옮길 수 없습니다.
	DST_ITEM_CAN_NOT_MOVE_NOT_EMPTY_BAG,	// 비어있지 않은 가방을 옮길 수 없습니다. 
	DST_ITEM_CAN_NOT_MOVE_NOT_EQUIAL_EQUIPTYPE,	// 장비타입이 달라 장착할 수 없습니다.	
	DST_ITEM_CAN_NOT_MOVE_TO_BAGSLOT_NOT_BAG,	// 캡슐키트슬롯에는 캡슐키트만 장착 가능합니다. 
	DST_ITEM_CAN_NOT_MOVE_TO_EQUIP_NOT_EQUIPITEM,	// 장착가능한 아이템만 착용할 수 있습니다.
	DST_ITEM_CAN_NOT_MOVE_TO_SCOUTER_NOT_SCOUTERPARTS,	// 스카우터 파츠 아이템만 장착 가능합니다.
	DST_ITEM_CAN_NOT_MOVE_TO_BAGSLOT_FULLBAGSLOT,	// 가방을 더이상 장착할 수 없습니다.

	DST_ZENNY_CAN_NOT_MOVE_TO_TRADECART_NOT_VALID,	// 거래창이 없습니다. 

	DST_ITEM_CAN_NOT_DISCARD_NOT_PICKUP,	// 아이템을 집은 상태에서 클릭해야 버릴 수 있습니다
	DST_ITEM_CAN_NOT_DISCARD_NOT_STATE,		// 현재상태에서는 아이템을 버릴 수 없습니다.	
	DST_ITEM_CAN_NOT_UNEQUIP_BAG_FULL,		// 가방이 꽉차서 아이템을 해제할 수 없습니다
	DST_ITEM_NO_TARGET_APPOINTED,			// 사용 대상을 선택해야 합니다. (아이템 사용시 적용 타겟이 없을 때)
	DST_ITEM_CAN_NOT_USE_MAX_LP,			// LP가 꽉차서 사용할 수 없습니다. ( LP, EP, RPBALL은 하나의 아이템 사용시 중복 출력될 수 있음 )
	DST_ITEM_CAN_NOT_USE_MAX_EP,			// EP가 꽉차서 사용할 수 없습니다. 
	DST_ITEM_CAN_NOT_USE_MAX_RP_BALL,		// RP BALL이 꽉차서 사용할 수 없습니다. 

	DST_ITEM_CAN_NOT_MOVE_ERROR_SERIAL_ID,	// 시리얼 ID 오류입니다. ( GM에게 문의하세요. )
	DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND,// 네트워크 오류입니다. ( 다시 시도해 주세요. )

	// Upgrade
	DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_HOIPOISTONE,		// 이미 호이포이 스톤이 장착되어 등록할 수 없습니다.
	DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_HOIPOISTONE,		// 호이포이스톤만 장착 가능합니다.
	DST_ITEMUPGRADE_NOTIFY_CANNOT_UPGRADE,					// 더이상 업그레이드 할 수 없습니다.
	DST_ITEMUPGRADE_NOTIFY_CANNOT_DOWNGRADE,				// 더이상 다운그레이드 할 수 없습니다.
	DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEBLE_ITEM,	// 업그레이드 가능한 아이템만 장착 가능합니다.
	DST_ITEMUPGRADE_NOTIFY_CANNOT_INSERT_DUPLEX,			// 슬롯이 비어있지 않습니다. 오른쪽키로 해제가능합니다.
	DST_ITEMUPGRADE_NOTIFY_CANNOT_CONTROL_STATE,			// 업그레이드 슬롯을 조작할 수 없는 상태입니다.	
	DST_ITEMUPGRADE_NOTIFY_CANNOT_PICKUP_UPGRADE_ITEM,		// 업그레이드 아이템은 집을 수 없습니다. 오른쪽 키로 해제 가능합니다.
	DST_ITEMUPGRADE_NOTIFY_CAN_INSERT_ONLY_UPGRADEITEM_OR_HOIPOISTONE,	// 업그레이드 가능한 아이템이나 호이포이스톤만 장착가능 합니다.
	
	// Target
	DST_TARGET_NONE,							// 타켓이 없습니다
	DST_TARGET_CANNOT_TARGETING_FOR_SCOUTER,	// 자기 자신을 측정할 수 없습니다(스카우터 전투력 측정시)

	// free pvp
	DST_FREEPVP_CHALLENGE,						// "%s"님이 도전을 하였습니다.\n전투를 벌이시겠습니까?
	DST_FREEPVP_OUTSIDE_STAGE,					// XX초 후에 장외 패배하게 됩니다. 
	DST_FREEPVP_WIN,							// "%s"님이 패배 하셨습니다.
	DST_FREEPVP_LOSE,							// "%s"님이 승리 하셨습니다.
	DST_FREEPVP_DRAW,							// "%s"님이 승리 하셨습니다.

	// Rank Battle
	DST_RANKBATTLE_CANCELED,					// 랭크배틀이 취소되었습니다.
	DST_RANKBATTLE_MUST_SELECT_ROOMID,			// 도전할 랭크배틀을 선택해야 합니다.
	DST_RANKBATTLE_MEMBER_FAINT,				// %s 님이 기절했습니다.
	DST_RANKBATTLE_MEMBER_OUTOFAREA,			// %s 님이 장외입니다.
	DST_RANKBATTLE_MEMBER_GIVEUP,				// %s 님이 도망갔습니다.
	DST_RANKBATTLE_MATCH_READY,					// 랭크배틀 준비중에는 게시판을 열 수 없습니다.
	
	// 천하제일무도회
	DST_BUDOKAI_MINOR_KILL_ANNOUNCE,		// %s님이 %s님을 기절시켰습니다.
	DST_BUDOKAI_MINOR_DICE_READY_NOTIFY,	// %u초 후 주사위를 굴려 승부를 결정합니다.
	DST_BUDOKAI_MINOR_DICE_WIN_NOTIFY,		// %s팀이 %d로 승리하셨습니다.
	DST_BUDOKAI_MINOR_DICE_SCORE_NOTIFY,	// %s팀 주사위 결과 ( %u )
	DST_BUDOKAI_MINOR_WAIT_FOR_DICE_NOTIFY,	// 추첨이 진행중입니다. 잠시만 기다려주십시요.
	DST_BUDOKAI_MINOR_WAIT_FOR_KO,			// 기절 후유증으로 %d초 동안 일어날 수 없습니다.
	DST_BUDOKAI_MINOR_WAIT_FOR_RINGOUT,		// 장외로 %d초 동안 전투에 참여할 수 없습니다.

	// Bind (바인드시 특별한 값을 필요로함)
	DST_BIND_TEXT,							// 바인드 하시겠습니까? \\n바인드 하면 부활시 현재 위치에서 부활합니다(MsgBox, 부활 위치를 갱신하려고 할 때)
	DST_BIND_SUCCESS,						// 바인드가 성공적으로 이루어졌습니다(부활 위치를 갱신했다)

	// Dialog manager
	DST_CAN_NOT_OPEN_MORE_REGULAR_DIALOG,	// 더 이상 창을 열 수 없습니다(레귤러 다이얼로그를 열려고 할 때 더이상 열 공간이 없다)

	// Item repair
	DST_REPAIR_ITEM,						// %d 제니를 지불하고 %s 아이템을 수리하였습니다(수리 성공시)
	DST_REPAIR_ALL_ITEM,					// %d 제니를 지불하고 모든 아이템을 수리하였습니다(수리 성공시)

	// Item Identify
	DST_ITEM_IS_IDENTIFIED_ALREADY,			// 이미 확인된 아이템입니다. 
	DST_ITEM_IS_UNIDENTIFIED,				// 확인되지 않은 아이템입니다. 

	// Block Mode Cooltime
	DST_BLOCK_MODE_COOLTIME,				// 블록모드를 사용할 수 없습니다(쿨타임때문에)

	// World map
	DST_WORLDMAP_CAN_OPEN_AREA,				// 월드맵을 열 수 없는 지역입니다(월드맵을	 열려고 할 때 열 수 없는 지역이다)
	DST_WORLDMAP_BIND_POPOICON_TOOLTIP,		// 기절한후 포포스톤에서 부활을 선택하면\n이곳에서 부활하게 됩니다(포포스톤 툴팁)

	// Select Reward
	DST_MUST_SELECT_REWARD,					// 선택보상을 선택하지 않았습니다(TMQ 선택보상을 선택하지 않고 확인버튼을 눌렀을 때)

	DST_MSG_MUST_INPUT_MESSAGE,				// 아무것도 입력하지 않았습니다(인풋 메세지 박스에 아무것도 입력하지 않고 확인을 눌렀다)

	// bus
	DST_BUS_ASK_GET_ON,						// 버스에 탑승하시겠습니까?(MsgBox, 버스에 탈 것인지 물어본다)
	DST_BUS_ASK_GET_OFF,					// 버스에서 내리시겠습니까?(MsgBox, 버스에 내릴 것인지 물어본다)
	DST_BUS_SUCCESS_GET_ON,					// 버스에 탑승하였습니다(요금 : %d Zenny)(버스에 탑승 성공시)

	// 탈 것
	DST_VEHICLE_ONLY_REG_FUEL,				// 연료만 넣을 수 있습니다(연료가 아닌 아이템을 탈 것 GUI에 등록하려 할 때)
	DST_VEHICLE_NEED_FUEL,					// 연료가 필요합니다(연료 없이 엔진에 시동을 걸려고 할 때)
	DST_VEHICLE_CAN_NOT_DO_THAT_WORKING_ENGINE,	// 엔진이 시동이 걸려있으면 할 수 없습니다(엔진이 시동이 걸렸을 때 불가한 행동을 하려고 하면 ex : 연료교체)
	DST_VEHICLE_ASK_GET_OFF,				// 내리시겠습니까?(MsgBox, 탈 것에서 내리는지 물어볼 때)

	// npc shop	
	DST_NPCSHOP_SUCCESS_REMOTE_SELL,		// %s 아이템을 원격 판매하였습니다(원격판매에 성공했을 때)

	// namekan sign
	DST_UNCOVER_NAMEKAN_SIGN,				// 나메칸사인의 데이터를 다운 받았습니다(나메칸 사인을 클릭했을 때)

	// 상태
	DST_KNOCKDOWN,							// 실신 상태입니다

	// Petition
	DST_PETITION_CHOICE_GREATE_CATEGORY,	// 대분류를 선택하십시요(GM 채팅 만족도 창에서 만족도를 보내려 할 때)
	DST_PETITION_CHOICE_SMALL_CATEGORY,		// 소분류를 선택하십시요(GM 채팅 만족도 창에서 만족도를 보내려 할 때)
	DST_PETITION_INPUT_CONTENT,				// 내용을 입력하세요(GM 채팅 만족도 창에서 만족도를 보내려 할 때)
	DST_PETITION_TOO_LONG_CONTENT,			// 내용이 너무 깁니다(GM 채팅 만족도 창에서 만족도를 보내려 할 때)

	DST_PETITION_SIDEVIEW_MESSAGE,			// 진정이 신청되었을 때 사이드의 메세지

	// 도장
	DST_DOJO_ASK_ESTABLISHMENT,				// 도장을 설립하시겠습니까? 500,000 제니가 필요합니다(MsgBox : 도장을 설립하려고 할 때)
	DST_DOJO_SUCCESS_ESTABLISHMENT,			// %s의 도장을 설립하였습니다(유파원에게 도장이 설립됬음을 알린다)
	DST_DOJO_ADD_FUNCTION,					// '%s' 도장 기능을 추가하시겠습니까?(MsgBox, 길드장이 도장 기능을 추가할 때)
	DST_DOJO_ADD_FUNCTION_NOTIFY,			// 도장에 기능이 변경되었습니다(Notify, 모든 길드원에게)

	DST_DOJO_ONLY_REGI_BAG_ZENNY,			// 가방의 제니만 등록할 수 있습니다(도장 업그레이드를 위해 제니를 등록하려 할 때)
	DST_DOJO_ASK_UPGRAGE_WITH_ZENNY,		// 도장을 업그레이드 하시겠습니까? 확인을 누리시면 %d 제니가 소모됩니다(MsgBox : 업그레이드 할지를 물어볼 때)
	DST_DOJO_ASK_UPGRAGE_WITH_ZENNY_AND_HOIPOI,	// 도장을 업그레이드 하시겠습니까? 확인을 누리시면 %d 제니와 호이포이락 % 개가 소모됩니다(MsgBox : 업그레이드 할지를 물어볼 때)

	DST_DOJO_ADD_TENKAI_SEED,				// 천하제일 무도회 시드 출전자 이름을 입력해 주십시요(Msgbox : 도장을 가진 유파장이 천하제일 무도회 시드 출전자를 결정할 때)

	DST_DOJO_SCRAMBLE_ASK_CHALLENGE,		// 도장 쟁탈전을 신청하시겠습니까? 신청금 %d 제니가 필요합니다(Msgbox : 도장 쟁탈전 신청)
	DST_DOJO_SCRAMBLE_ACCEPT_CHALLENGE,		// %s 유파의 쟁탈전 신청을 수락하시겠습니까? 거부를 선택하시면 배상금 %d 제니가 소모됩니다(Msgbox : 도장 쟁탈전 수락)
	DST_DOJO_SCRAMBLE_ASK_TELEPORT,			// 쟁탈전이 진행중인 도장안으로 순간이동 하시겠습니까?(Msgbox : 도장 진입 텔레포트)

	DST_DOJO_LOST_DOJO,						// 도장의 소유 길드가 도장의 소유권을 잃어버렸습니다(아바타가 속한 유파가 가지고 있던 도장의 소유권을 잃었다)
	DST_DOJO_ASK_SCRAMBLE,					// 도장 쟁탈전 신청을 했습니다(도장 쟁탈전을 신청한 유파장에게)
	DST_DOJO_ACCEPTED_SCRAMBLE,				// 쟁탈전 신청이 수락되었습니다(도장 쟁탈전을 신청한 유파장에게)
	DST_DOJO_REJECT_SCRAMBLE,				// 쟁탈전 신청이 거부당했습니다(도장 쟁탈전을 신청한 유파장에게)

	DST_DOJO_SCRAMBLE_STATE_NORMAL,			// 오늘 %d시 %d분부터 도장 쟁탈전이 진행될 예정입니다(도장 쟁탈전 진행 메세지)
	DST_DOJO_SCRAMBLE_STATE_NORMAL2,		// 오늘 %d시부터 도장 쟁탈전이 진행될 예정입니다(도장 쟁탈전 진행 메세지)
	DST_DOJO_SCRAMBLE_STATE_RECEIVE,		// 도장 쟁탈전 신청 가능한 시간입니다(도장 쟁탈전 진행 메세지)
	DST_DOJO_SCRAMBLE_STATE_REJECT_END,		// 도장 쟁탈전 거부 시간이 종료되었습니다(도장 쟁탈전 진행 메세지)
	DST_DOJO_SCRAMBLE_STATE_STANDBY,		// 도장 쟁탈전 준비중입니다(도장 쟁탈전 진행 메세지)
	DST_DOJO_SCRAMBLE_STATE_INITIAL,		// 도장 쟁탈전 준비중입니다(도장 쟁탈전 진행 메세지)
	DST_DOJO_SCRAMBLE_STATE_READY,			// 도장 쟁탈전 시작을 위해 도장에 입장하여 주십시요(도장 쟁탈전 진행 메세지)
	DST_DOJO_SCRAMBLE_STATE_START,			// 도장 쟁탈전이 시작되었습니다(도장 쟁탈전 진행 메세지)
	DST_DOJO_SCRAMBLE_STATE_END,			// 도장 쟁탈전이 종료되었습니다(도장 쟁탈전 진행 메세지)

	DST_DOJO_SCRAMBLE_CHANGE_SEAL_OWNER,	// %s님이 %s을 획득(도장 쟁탈전시 인장을 획득한 사람에 대한 정보를 받았을 때)

	DST_DOJO_CAN_NOT_UPGRADE_BECAUSE_MAX_LEVEL, // 현재 도장 레벨이 최고입니다(최고 레벨의 도장을 업그레이드를 하려고 할 때)
	DST_DOJO_CAN_NOT_UPGRADE_BECAUSE_NEED_ITEM, // 도장 업그레이드에 필요한 호이포이락을 등록해 주십시요(도장 업그레이드를 하려고 할 때)
	DST_DOJO_CAN_NOT_UPGRADE_BECAUSE_NEED_MORE_ITEM, // 등록된 호이포이락이 부족합니다(도장 업그레이드를 하려고 할 때)
	DST_DOJO_ONLY_REGISTER_ITEM,			 // 호이포이돌만 등록할 수 있습니다(도장 업그레이드 GUI에 아이템을 등록하려고 할 때)
	DST_DOJO_HAVE_NOT_DOJO,					// 소유한 도장이 없습니다(도장 업그레이드 GUI에 아이템을 등록하려고 할 때)

	DST_DOJO_REWARD_GUILD_REPUTATION,		// 도장 쟁탈전 승리로 %d의 길드 명성치를 획득했습니다(도장 쟁탈전 끝나고 이긴팀에게 보상)
	DST_DOJO_REWARD_ITEM,					// 도장 쟁탈전 승리로 %s 아이템을 %d개 획득했습니다(도장 쟁탈전 끝나고 이긴팀에게 보상)
	DST_DOJO_REWARD_NPC_NARRATION,			// 수고하셨습니다. 당신의 유파가 세계 최강입니다!!(도장 쟁탈전 끝나고 이긴팀에게 보상시 NPC 대사)

	// 도복
	DST_DOGI_MUST_REGI_DOGI_SLOT,			// 먼저 도복 아이템을 슬롯에 등록해 주십시요(도복GUI에서 도복의 색상을 바꾸려고 할 때)
	DST_DOGI_CHANGED,						// 도복이 변경되었습니다(도복이 변경되었음을 알린다)
	DST_DOGI_ASK_APPLY_CHANGE,				// 유파 도복 설정을 완료하시겠습니까?(Msgbox : 유파 도복 설정을 적용할지 물어볼 때)
	DST_DOJO_DOGI_ASK_APPLY_CHANGE,			// 도장 도복 설정을 완료하시겠습니까?(Msgbox : 도장 도복 설정을 적용할지 물어볼 때)
	DST_DOGI_ONLY_REGISTER_DOGI_ITEM,		// 도복만 등록할 수 있습니다(아이템을 도복 설정 GUI에 등록하려 할 때)

	// Etc
	DST_POPOSTONE_RESURRECT_SAY,			// 목숨은 소중한거야. 조심해서 행동해 (포포스톤부활시 말풍선)

	DST_ASK_CHANGE_CHANNEL,					// 채널을 변경하시겠습니까?(MsgBox : 채널 변경할지를 물어본다)
	DST_ALREADY_SAME_CHANNEL,				// 이미 접속한 채널을 선택하셨습니다(게임중 채널을 변경하려고 할 때)

	DST_NOT_SELECTED_SLOT,					// 선택한 슬롯이 없습니다(어떠한 작업을 하려 할 때 선택한 슬롯이 없어서 할 수 없다)

	DST_LOBBY_ENABLE_LOCAL_TEXT,			// 완성형 한글과 숫자, 영어만 사용할 수 있습니다(캐릭터, 파티, 길드 이름을 생성할 때/언어별 다른 설명이 필요함)

	DST_LOBBY_NO_HAVE_ZENNY,				// 소지금 없음

	DST_CHAR_GROWN_UP,						// 캐릭터가 성장할때의 메시지. Notify용
	DST_CHAR_CAN_CHANGE_CLASS,				// 전직할 수 있을 때의 메시지. Notify용

	// 사용기간 제한 MsgBox 관련
	DST_COMMERCIAL_MB_UPDATE_TERM_LIMETED_ITEM_MSG,	// HTML, %s의 사용 기간이 만료되어 사용이 정지되었습니다.
	DST_COMMERCIAL_MB_DELETE_TERM_LIMETED_ITEM_MSG, // HTML, %s은 사용 기간이 만료된 아이템입니다.\n기간연장을 하지 않고 버리시겠습니까?
	DST_COMMERCIAL_MB_USE_TERM_LIMETED_ITEM_MSG,	// HTML, %s은 사용 기간이 만료된 아이템입니다.\n기간연장을 신청하시겠습니까?
	DST_COMMERCIAL_TERM_LIMITED_ITEM_WARNING_MSG,	// %s 사용 기간 곧 만료됩니다.

	// 심의등급 표시
	DST_DELIBERATION_RANK_MESSAGE,			// 국가별 심의등급 표시 문구

	// 퀵슬롯
	DST_QUICKSLOT_IS_LOCK,					// 퀵슬롯이 잠김상태입니다. 

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// UI Display Text _ UI에 표시하기 위한 텍스트
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 공  통
	DST_GUILD,								// 유파
	DST_RACE,								// 종족
	DST_RECORD,								// 전적
	DST_NOT_JOINED_GUILD,					// 유파 없음


	// 드래곤볼 컬렉션 관련
	DST_DBC_ITEM_CONFIRM,					// [UI표시용]보상을 받으시겠습니까
	DST_DBC_ITEM_OK,						// 확인
	DST_DBC_ITEM_BACK,						// 돌아가기
	DST_DBC_TIME_LIMIT,						// 제한시간	
	DST_DBC_SEASON,							// 드래곤볼 컬렉션 이벤트 진행중
	DST_DBC_NO_SEASON,						// 드래곤볼 컬렉션 이벤트 기간이 아닙니다
	DST_DBC_COLLECT_COMPLETE,				// 드래곤볼 보상 받기
	DST_DBC_REWARD_COMMENT,					// 가까운 드래곤볼 신전에서 보상을 받으십시오.
	DST_DBC_KEYWORD,						// 주문	

	// Pet Status
	DST_PET_TOOLTIP_ATTACK,                     // 타겟으로 지정된 적을 공격합니다.

	// Option
	DST_OPTION_TITLE,						// 옵션 설정
	DST_OPTION_RESET,						// 기본값
	DST_OPTION_OK,							// 적용
	DST_OPTION_CANCEL,						// 취소
	DST_OPTION_TAB_SYSTEM,					// 시스템
	DST_OPTION_TAB_GAME,					// 게임
	DST_OPTION_TAB_SYSTEM_GRAPHIC,			// 일반(그래픽)
	DST_OPTION_TAB_SYSTEM_EFFECT,			// 효과
	DST_OPTION_TAB_SYSTEM_SOUND,			// 소리
	DST_OPTION_TAB_GAME_CONTROL,			// 조작키
	DST_OPTION_TAB_GAME_INFO,				// 일반(정보)
	DST_OPTION_TAB_GAME_ETC,				// 기타
	DST_OPTION_TAB_GAME_CHATTING,			// 채팅
	DST_OPTION_GRAPHIC_RESOLUTION_TITLE,	// 해상도 설정
	DST_OPTION_GRAPHIC_RESOLUTION,			// 해상도
	DST_OPTION_GRAPHIC_GAMMA,				// 밝기
	DST_OPTION_GRAPHIC_WINDOWMODE,			// 창 모드
	DST_OPTION_GRAPHIC_TITLE,				// 효과
	DST_OPTION_GRAPHIC_QUALITY_TOTAL,		// 전체 그래픽 품질
	DST_OPTION_GRAPHIC_QUALITY_LOW,			// 낮음
	DST_OPTION_GRAPHIC_QUALITY_MIDDLE,		// 보통
	DST_OPTION_GRAPHIC_QUALITY_HIGH,		// 높음
	DST_OPTION_GRAPHIC_QUALITY_USER,		// 사용자
	DST_OPTION_GRAPHIC_QUALITY_TERRAIN_RANGE,	// 지형 시야거리
	DST_OPTION_GRAPHIC_QUALITY_OBJECT_RANGE,	// 사물 시야거리
	DST_OPTION_GRAPHIC_TERRAIN_SHADOW,			// 지형 그림자
	DST_OPTION_GRAPHIC_WATER_EFFECT,		// 물 효과
	DST_OPTION_GRAPHIC_CHARACTER_RANGE,		// 캐릭터 시야거리
	DST_OPTION_GRAPHIC_CHARACTER_SHADOW,	// 캐릭터 그림자
	DST_OPTION_GRAPHIC_MAGIC_EFFECT,		// 마법 세부효과
	DST_OPTION_GRAPHIC_WEATHER_EFFECT,		// 날씨 효과
	DST_OPTION_GRAPHIC_TEXTURE_QUALITY,		// 텍스쳐 해상도
	DST_OPTION_GRAPHIC_CHARACTER_EFFECT,	// 캐릭터 효과
	DST_OPTION_GRAPHIC_CARTOON_EDGE,		// 카툰식 외곽선
	DST_OPTION_GRAPHIC_UPGRADE_EFFECT,		// 업그레이드 효과
	DST_OPTION_GRAPHIC_ETC_TITLE,			// 기타 효과
	DST_OPTION_GRAPHIC_HDR_EFFECT,			// HDR 효과
	DST_OPTION_SOUND_MAIN,					// 메인 볼륨
	DST_OPTION_SOUND_BGM,					// 배경 볼륨
	DST_OPTION_SOUND_SE,					// 효과 볼륨
	DST_OPTION_SOUND_ENV,					// 환경 볼륨
	DST_OPTION_INFO_GUILDEMBLEM,			// 길드 엠블
	DST_OPTION_INFO_DOJOMARK,				// 도장 마크
	DST_OPTION_INFO_SCOUTERMODEL,			// 스카우트 모델
	DST_OPTION_ETC_FRIENDCONNECT,			// 친구 접속
	DST_OPTION_ETC_GUILDCONNECT,			// 길드 멤버 접속
	DST_OPTION_ETC_QUESTEFFECT,				// 퀘스트 텍스트 효과
	DST_OPTION_ETC_USERTRADE,				// 유저 거래
	DST_OPTION_ETC_WINDOWMOVE,				// 윈도우 고정 옵션
	DST_OPTION_ETC_WINDOWMOVE_DEFAULT,		// 윈도우 위치를 원래되로
	DST_OPTION_ETC_MOUSEDASH,				// 마우스 대시 간격
	DST_OPTION_ETC_EX_QUICKSLOT1,			// 추가 퀵슬롯 1
	DST_OPTION_ETC_EX_QUICKSLOT2,			// 추가 퀵슬롯 2
	DST_OPTION_ETC_EX_QUICKSLOT_LOCK,		// 퀵슬롯 잠그기
	DST_OPTION_RADIO_ON,					// On
	DST_OPTION_RADIO_OFF,					// Off
	DST_OPTION_RESOLUTION_FORMAT,			// %d * %d * %dbit
	DST_OPTION_RESOLUTION_FORMAT_WINMODE,	// %d * %d

	// Control Option Window
	DST_OPTION_CONTROL_TEXT_TITLE,			// 조작키 설정
	DST_OPTION_CONTROL_TEXT_GUIDE,			// 단축키가 중복될 경우 이전 단축키는 [%s] 상태가 됩니다.
	DST_OPTION_CONTROL_TEXT_CHANGEGUIDE,	// [%s]와 중복됩니다. [%s]는 [%s]으로 변경됩니다.
	DST_OPTION_CONTROL_TEXT_READYGUIDE,		// 변경될 키를 입력하세요.
	DST_OPTION_CONTROL_TEXT_WAITGUIDE,		// 변경된 키가 없거나 아직 적용할 수 없습니다. 잠시만 기다려 주십시오.
	DST_OPTION_CONTROL_TEXT_NOTCHANGEGUIDE, // %s 키는 변경이 불가능합니다.
	DST_OPTION_CONTROL_TEXT_FIXEDKEYGUIDE,	// 입력하신 키는 유저가 임의로 사용할 수 없는 [고정된 키] 입니다.
	DST_OPTION_CONTROL_TEXT_NOTCOMBINEGUIDE,// 조합 키로는 사용할 수 없는 액션입니다.
	DST_OPTION_CONTROL_TEXT_SAMEKEYGUIDE,	// 기존에 있던 키와 같은 키입니다.
	DST_OPTION_CONTROL_TEXT_NEGATIVEMODEGUIDE,	// 현재 모드에서는 키 변경이 불가능합니다.
	DST_OPTION_CONTROL_TEXT_EMPTYKEY,		// 지정안됨 


	DST_OPTION_CONTROL_CATEGORY_AVATAR,		// 아바타 행동(카테고리)
	DST_OPTION_CONTROL_CATEGORY_TARGET,		// 타겟 행동
	DST_OPTION_CONTROL_CATEGORY_QUICKSLOT,	// 퀵슬롯 행동
	DST_OPTION_CONTROL_CATEGORY_WINDOW,		// 윈도우 행동
	DST_OPTION_CONTROL_CATEGORY_FIXED,		// 고정(단축키 변경 불가능)

	DST_OPTION_CONTROL_ACTION_AVATAR_FORWARD,	// 전진
	DST_OPTION_CONTROL_ACTION_AVATAR_BACKWARD,	// 후진
	DST_OPTION_CONTROL_ACTION_AVATAR_LEFTTURN,	// 좌회전
	DST_OPTION_CONTROL_ACTION_AVATAR_RIGHTTURN,	// 우회전
	DST_OPTION_CONTROL_ACTION_AVATAR_SITDOWN,	// 안기/일어서기
	DST_OPTION_CONTROL_ACTION_AVATAR_JUMP,		// 점프
	DST_OPTION_CONTROL_ACTION_AVATAR_AUTORUN,	// 자동달리기
	DST_OPTION_CONTROL_ACTION_AVATAR_SCOUTER,	// 스카우터 사용
	DST_OPTION_CONTROL_ACTION_AVATAR_CHARGE,	// 기 모으기
	DST_OPTION_CONTROL_ACTION_AVATAR_LOOTING,	// 아이템 줍기
	DST_OPTION_CONTROL_ACTION_AVATAR_BLOCKING,	// 블록킹

	DST_OPTION_CONTROL_ACTION_TARGET_SELF,		// 자신 선택
	DST_OPTION_CONTROL_ACTION_TARGET_1STPARTY,	// 첫번째 파티원 선택
	DST_OPTION_CONTROL_ACTION_TARGET_2NDPARTY,	// 두번째 파티원 선택
	DST_OPTION_CONTROL_ACTION_TARGET_3RDPARTY,	// 세번째 파티원 선택
	DST_OPTION_CONTROL_ACTION_TARGET_4THPARTY,	// 네번째 파티원 선택
	DST_OPTION_CONTROL_ACTION_TARGET_AUTO,		// 가까운 적 선택
	DST_OPTION_CONTROL_ACTION_TARGET_AUTOATK,	// 자동 공격
	DST_OPTION_CONTROL_ACTION_TARGET_AUTOFOLLOW,// 자동 따라가기
	DST_OPTION_CONTROL_ACTION_TARGET_MARKING1,	// 공유 타겟1 지정
	DST_OPTION_CONTROL_ACTION_TARGET_MARKING2,	// 공유 타겟2 지정
	DST_OPTION_CONTROL_ACTION_TARGET_MARKING3,	// 공유 타겟3 지정
	DST_OPTION_CONTROL_ACTION_TARGET_MARKING4,	// 공유 타겟4 지정
	DST_OPTION_CONTROL_ACTION_TARGET_MARKING5,	// 공유 타겟5 지정
	DST_OPTION_CONTROL_ACTION_TARGET_SELECTING1,// 공유 타겟1 선택
	DST_OPTION_CONTROL_ACTION_TARGET_SELECTING2,// 공유 타겟2 선택
	DST_OPTION_CONTROL_ACTION_TARGET_SELECTING3,// 공유 타겟3 선택
	DST_OPTION_CONTROL_ACTION_TARGET_SELECTING4,// 공유 타겟4 선택
	DST_OPTION_CONTROL_ACTION_TARGET_SELECTING5,// 공유 타겟5 선택
	DST_OPTION_CONTROL_ACTION_TARGET_AUTOPARTY,	// 파티원 자동 선택

	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK1,	// 퀵슬롯 1번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK2,	// 퀵슬롯 2번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK3,	// 퀵슬롯 3번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK4,	// 퀵슬롯 4번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK5,	// 퀵슬롯 5번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK6,	// 퀵슬롯 6번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK7, // 퀵슬롯 7번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK8,	// 퀵슬롯 8번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK9,	// 퀵슬롯 9번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK0,	// 퀵슬롯 0번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKMIN,	// 퀵슬롯 -
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKPLUS,	// 퀵슬롯 +
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK1_EX,	// 퀵슬롯 1번 (확장 1번)
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK2_EX,	// 퀵슬롯 2번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK3_EX,	// 퀵슬롯 3번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK4_EX,	// 퀵슬롯 4번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK5_EX,	// 퀵슬롯 5번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK6_EX,	// 퀵슬롯 6번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK7_EX, // 퀵슬롯 7번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK8_EX,	// 퀵슬롯 8번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK9_EX,	// 퀵슬롯 9번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK0_EX,	// 퀵슬롯 0번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKMIN_EX,	// 퀵슬롯 -
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKPLUS_EX,	// 퀵슬롯 +
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK1_EX2,	// 퀵슬롯 1번 (확장 2번)
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK2_EX2,	// 퀵슬롯 2번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK3_EX2,	// 퀵슬롯 3번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK4_EX2,	// 퀵슬롯 4번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK5_EX2,	// 퀵슬롯 5번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK6_EX2,	// 퀵슬롯 6번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK7_EX2, // 퀵슬롯 7번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK8_EX2,	// 퀵슬롯 8번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK9_EX2,	// 퀵슬롯 9번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK0_EX2,	// 퀵슬롯 0번
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKMIN_EX2,	// 퀵슬롯 -
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKPLUS_EX2,	// 퀵슬롯 +
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_PREV,	// 이전 퀵슬롯
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_NEXT,	// 다음 퀵슬롯
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_2NDCAP,	// 2번 캡슐키트
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_3RDCAP,	// 3번 캡슐키트
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_4THCAP,	// 4번 캡슐키트
	DST_OPTION_CONTROL_ACTION_QUICKSLOT_5THCAP,	// 5번 캡슐키트

	DST_OPTION_CONTROL_ACTION_WINDOW_PROFILE,	// 캐릭터정보
	DST_OPTION_CONTROL_ACTION_WINDOW_SKILL,		// 스킬정보
	DST_OPTION_CONTROL_ACTION_WINDOW_QUEST,		// 퀘스트 정보
	DST_OPTION_CONTROL_ACTION_WINDOW_MINIMAP,	// 미니맵
	DST_OPTION_CONTROL_ACTION_WINDOW_WORLDMAP,	// 월드맵
	DST_OPTION_CONTROL_ACTION_WINDOW_MAINCAP,	// 메인 캡슐키트
	DST_OPTION_CONTROL_ACTION_WINDOW_SCOUTER,	// 스카우터 정보
	DST_OPTION_CONTROL_ACTION_WINDOW_PARTY,		// 파티 정보
	DST_OPTION_CONTROL_ACTION_WINDOW_GUILD,		// 길드 정보
	DST_OPTION_CONTROL_ACTION_WINDOW_FRIEND,	// 친구 정보
	DST_OPTION_CONTROL_ACTION_WINDOW_OPTION,	// 옵션
	DST_OPTION_CONTROL_ACTION_WINDOW_HELP,		// 도움말
	DST_OPTION_CONTROL_ACTION_WINDOW_MAIN,		// 메인메뉴
	DST_OPTION_CONTROL_ACTION_WINDOW_RANKBOARD,	// 랭크보드

	DST_OPTION_CONTROL_ACTION_MINIMAP_ZOOMOUT,	// 미니맵 축소
	DST_OPTION_CONTROL_ACTION_MINIMAP_ZOOMIN,	// 미니맵 확대
	DST_OPTION_CONTROL_ACTION_CHAT_PGUP,		// 채팅창 이전 페이지
	DST_OPTION_CONTROL_ACTION_CHAT_PGDN,		// 채팅창 다음 페이지
	DST_OPTION_CONTROL_ACTION_GLOBAL_CANCLE,	// 취소
	DST_OPTION_CONTROL_ACTION_GLOBAL_SNAPSHOT,	// 스크린샷
	DST_OPTION_CONTROL_ACTION_GLOBAL_CHAT,		// 채팅 입력

	// Chatting Option
	DST_OPTION_CHATTING_TITLE_COMMON,			// 공통 옵션
	DST_OPTION_CHATTING_COMMON_INPUTMODE,		// 채팅창 입력모드
	DST_OPTION_CHATTING_COMMON_LOCK,			// 채팅창 잠금
	DST_OPTION_CHATTING_COMMON_SCROLL,			// 마우스 스크롤
	DST_OPTION_CHATTING_COMMON_ADDEDWINDOW,		// 추가 윈도우
	DST_OPTION_CHATTING_COMMON_OPACITY,			// 윈도우 투명도

	DST_OPTION_CHATTING_TITLE_BASIC,			// 기본 채팅창
	DST_OPTION_CHATTING_BASIC_NORMAL,			// 일반 대화
	DST_OPTION_CHATTING_BASIC_WISPHER,			// 귓속말
	DST_OPTION_CHATTING_BASIC_PARTY,			// 파티
	DST_OPTION_CHATTING_BASIC_GUILD,			// 길드
	DST_OPTION_CHATTING_BASIC_TRADE,			// 거래
	DST_OPTION_CHATTING_BASIC_SHOUT,			// 외치기
	DST_OPTION_CHATTING_BASIC_SYSTEM,			// 시스템

	DST_OPTION_CHATTING_TITLE_ADDED,			// 기본 채팅창
	DST_OPTION_CHATTING_ADDED_NORMAL,			// 일반 대화
	DST_OPTION_CHATTING_ADDED_WISPHER,			// 귓속말
	DST_OPTION_CHATTING_ADDED_PARTY,			// 파티
	DST_OPTION_CHATTING_ADDED_GUILD,			// 길드
	DST_OPTION_CHATTING_ADDED_TRADE,			// 거래
	DST_OPTION_CHATTING_ADDED_SHOUT,			// 외치기
	DST_OPTION_CHATTING_ADDED_SYSTEM,			// 시스템 

	DST_OPTION_TOOLTIP_TAB_GRAPHIC,				// 그래픽 옵션을 설정합니다. ( 시스템-그래픽 카테고리의 툴팁 )
	DST_OPTION_TOOLTIP_RESOLUTION,				// 게임 해상도를 설정합니다.
	DST_OPTION_TOOLTIP_GAMMA,					// 게임의 밝기를 설정합니다. 전체화면 모드에서만 사용 할 수 있습니다.
	DST_OPTION_TOOLTIP_WINDOWMODE,				// 게임의 화면 모드를 결정합니다.
	DST_OPTION_TOOLTIP_GRAPHIC_QUALITY,			// 전체적인 그래픽 품질을 결정합니다.
	DST_OPTION_TOOLTIP_TERRAIN_DISTANCE,		// 지형 시야거리를 설정합니다.
	DST_OPTION_TOOLTIP_OBJECT_DISTANCE,			// 사물 시야거리를 설정합니다.
	DST_OPTION_TOOLTIP_TERRAIN_SHADOW,			// 지형 그림자의 출력 유/무를 결정합니다.
	DST_OPTION_TOOLTIP_WATER_EFFECT,			// 물 효과의 출력 유/무를 결정합니다.

	DST_OPTION_TOOLTIP_CHARACTER_DISTANCE,		// 캐릭터 시야거리를 결정합니다.
	DST_OPTION_TOOLTIP_SHADOW,					// 캐릭터의 그림자 출력 유/무를 결정합니다.
	DST_OPTION_TOOLTIP_MAGIC_DETAIL,			// 마법 세부효과의 품질을 결정합니다.
	DST_OPTION_TOOLTIP_WEATHER_EFFECT,			// 날씨 효과의 품질을 결정합니다.
	DST_OPTION_TOOLTIP_TEXTURE_LEVEL,			// 텍스쳐의 품질을 결정합니다. 시스템을 재시작해야지만 적용됩니다.
	DST_OPTION_TOOLTIP_CARTOON_EDGE,			// 카툰식 외곽선의 출력 유/무를 결정합니다.
	DST_OPTION_TOOLTIP_UPGRADE_EFFECT,			// 캐릭터 업그레이드 효과의 출력 유/무를 결정합니다.
	DST_OPTION_TOOLTIP_HDR,						// HDR 효과의 출력 유/무를 결정합니다.

	DST_OPTION_TOOLTIP_TAB_SOUND,				// 게임 내 사운드 관련 옵션을 설정합니다. ( 시스템-사운드 )
	DST_OPTION_TOOLTIP_MAINVOLUME,				// 전체 볼륨을 조절합니다.
	DST_OPTION_TOOLTIP_BGMVOLUME,				// 배경 음악 볼륨을 조절합니다.
	DST_OPTION_TOOLTIP_EFFECTVOLUME,			// 효과음 볼륨을 조절합니다.
	DST_OPTION_TOOLTIP_ENVVOLUME,				// 환경음 볼륨을 조절합니다.

	DST_OPTION_TOOLTIP_TAB_CONTROL,				// 게임 내 단축키 옵션을 설정합니다. ( 게임-조작 )

	DST_OPTION_TOOLTIP_TAB_INFO,				// 게임 내 정보 표시 옵션들을 설정합니다. ( 게임-정보 )
	DST_OPTION_TOOLTIP_GUILD_EMBLEM,			// 유파 엠블렘 출력 유/무를 결정합니다.
	DST_OPTION_TOOLTIP_DOJO_MARK,				// 도장 마크의 출력 유/무를 결정합니다.
	DST_OPTION_TOOLTIP_SCOUTER,					// 아바타의 스카우터 모델 표시 유/무를 결정합니다.

	DST_OPTION_TOOLTIP_TAB_ETC,					// 게임 내 자잘한 옵션들을 설정합니다. ( 게임-기타 )

	DST_OPTION_TOOLTIP_FRIEND_CONNECT,			// 친구가 접속을 하면 이벤트로 알려줍니다.
	DST_OPTION_TOOLTIP_MEMBER_CONNECT,			// 문하생이 접속을 하면 이벤트로 알려줍니다.
	DST_OPTION_TOOLTIP_QUEST_TEXT,				// 퀘스트 텍스트 효과를 지정합니다.
	DST_OPTION_TOOLTIP_TRADE,					// 거래 가능 여부를 설정합니다.
	DST_OPTION_TOOLTIP_WINDOW_MOVEABLE,			// UI 윈도우 이동 여부를 설정합니다.
	DST_OPTION_TOOLTIP_WINDOW_DEFAULT,			// UI 윈도우를 기본 위치로 이동합니다.
	DST_OPTION_TOOLTIP_MOUSE_DASH,				// 마우스로 대시를 하는 타임을 설정합니다.
	DST_OPTION_TOOLTIP_EX_QUICKSLOT1,			// 추가 퀵슬롯 1에 대한 기능의 사용 여부를 설정합니다.
	DST_OPTION_TOOLTIP_EX_QUICKSLOT2,			// 추가 퀵슬롯 2에 대한 기능의 사용 여부를 설정합니다.
	DST_OPTION_TOOLTIP_QUICKSLOT_LOCK,			// 퀵슬롯을 잠글 것인지 안 잘글 것인지 설정합니다.

	DST_OPTION_TOOLTIP_TAB_CHATTING,			// 게임 내 채팅 옵션을 설정합니다. ( 게임-채팅 )
	DST_OPTION_TOOLTIP_CHAT_INPUTMODE,			// 채팅 입력모드 툴팁
	DST_OPTION_TOOLTIP_CHAT_LOCK,				// 채팅창 잠금 툴팁
	DST_OPTION_TOOLTIP_CHAT_MOUSESCROLL,		// 마우스 스크롤 툴팁
	DST_OPTION_TOOLTIP_CHAT_ADDED_WINDOW,		// 추가 윈도우 툴팁
	DST_OPTION_TOOLTIP_CHAT_WINDOW_OPACITY,		// 윈도우 투명도 툴팁
	DST_OPTION_TOOLTIP_CHAT_NORMAL,				// 일반대화 툴팁
	DST_OPTION_TOOLTIP_CHAT_WHISPER,			// 귓속말 툴팁
	DST_OPTION_TOOLTIP_CHAT_PARTY,				// 파티말 툴팁
	DST_OPTION_TOOLTIP_CHAT_GUILD,				// 길드말 툴팁
	DST_OPTION_TOOLTIP_CHAT_TRADE,				// 거래말 툴팁
	DST_OPTION_TOOLTIP_CHAT_SHOUT,				// 외치기 툴팁
	DST_OPTION_TOOLTIP_CHAT_SYSTEM,				// 시스템말 툴팁

	// Help Window(Guide Book)
	DST_HELPWND_TITLE,						// 드래곤볼 온라인 가이드
	DST_HELPWND_TOP_TITLE,					// 드래곤볼 온라인 도움말
	DST_HELPWND_HELPCONTENTS,				// 도움말 목차
	DST_HELPWND_GOLIST,						// 목차로 이동
	DST_HELPWND_PETITION_TO_GM,				// GM상담

	// SideIcon : HelpHintView
	DST_SIDEICON_HINT_HELP_MORE,			// 바로 가기(헬프 윈도우 바로가기 버튼)

	// SideIcon : PoPo Icon
	DST_SIDEICON_HINT_FRIEND_TITLE,			// 친구 접속(타이틀)
	DST_SIDEICON_HINT_FRIEND_TEXT,			// %s님이 접속하셨습니다.
	DST_SIDEICON_HINT_SKILL_UPGRADE_TITLE,	// 스킬 업그레이드 (타이틀)
	DST_SIDEICON_HINT_SKILL_UPGRADE_TEXT,	// %s을 업그레이드 할 수 있습니다.
	DST_SIDEICON_HINT_NEWMAIL_TITLE,		// 메일 도착(타이틀)
	DST_SIDEICON_HINT_NEWMAIL_TEXT,			// 새로운 메일이 도착하였습니다.
	DST_SIDEICON_HINT_CLASS_CHANGE_TITLE,	// 전직 가능(타이틀)
	DST_SIDEICON_HINT_CLASS_CHANGE_TEXT,	// 상위 클래스로 체인지 할 수 있습니다.

	// PrivateShop Text
	DST_PRIVATESHOP_SHOPNAME,				// 상점이름
	DST_PRIVATESHOP_NOTICE,					// 알리는 글
	DST_PRIVATESHOP_AWAY,					// 자리비움
	DST_PRIVATESHOP_TAB_BUY_1,				// 물품
	DST_PRIVATESHOP_TAB_BUY_2,				// 부가정보
	DST_PRIVATESHOP_TAB_SELL_1,				// 물품
	DST_PRIVATESHOP_TAB_SELL_2,				// 설정
	DST_PRIVATESHOP_SHOPNAME_EMPTY,			// [Charater Name]의 개인상점
	DST_PRIVATESHOP_SALE_START,				// 판매시작
	DST_PRIVATESHOP_SALE_STOP,				// 판매중지
	DST_PRIVATESHOP_ZENNY,					// 소지금

	DST_PRIVATESHOP_NOTICE_FOOT,			// by %s
	DST_PRIVATESHOP_ITEM_NOT_IDENTIFICATION,// 미확인 아이템

	DST_PRIVATESHOP_NAMETAG_READY,			// 개인상점 준비중
	DST_PRIVATESHOP_NAMETAG_AWAY,			// [자리비움]

	DST_PRIVATESHOP_EMPTY_SLOT,				// 모든 품목이 팔렸습니다.

	// PrivateShop Business Text
	DST_PRIVATESHOP_BUSINESS_WAITREQUEST,	// 주인의 응답을 기다리는 중입니다.
	DST_PRIVATESHOP_BUSINESS_CONSENT,		// %s님이 %s의 흥정을 요청해왔습니다. 수락하시겠습니까?

	// PrivateShop Business Trade Gui
	DST_PRIVATESHOP_BUSINESS_TRADE_TITLE,		// 아이템 가격 흥정
	DST_PRIVATESHOP_BUSINESS_TRADE_ITEMTITLE,	// 흥정중인 아이템
	DST_PRIVATESHOP_BUSINESS_TRADE_ITEMPRICE,	// 현재 가격
	DST_PRIVATESHOP_BUSINESS_TRADE_CHAT,		// 채팅
	DST_PRIVATESHOP_BUSINESS_TRADE_APPLYPRICE,	// 변동된 가격이 적용되었습니다.
	DST_PRIVATESHOP_BUSINESS_TRADE_APPLYNOTIFY,	// 가격이 변동되었습니다.
	DST_PRIVATESHOP_BUSINESS_TRADE_REALLYBUY,	// 정말 구입하시겠습니까?

	// PrivateShopBusiness Trade Box
	DST_PRIVATESHOP_BUSINESS_TRADE_IMMEDIATE,	// 즉시 구매
	DST_PRIVATESHOP_BUSINESS_TRADE_FLUCTUATIONS,		// 가격 조정

	// PrivateShopBusiness TradeBox Gui
	DST_PRIVATESHOP_BUSINESS_TRADEBOX_TITLE,				// 가격 조정
	DST_PRIVATESHOP_BUSINESS_TRADEBOX_FLUCTUATIONS,			// 현재 아이템을 이렇게 조정합니다.
	DST_PRIVATESHOP_BUSINESS_TRADEBOX_PREVPRICE,			// 변경 전 가격
	DST_PRIVATESHOP_BUSINESS_TRADEBOX_NEXTPRICE,			// 변경 후 가격
	DST_PRIVATESHOP_BUSINESS_TRADEBOX_ACCEPT,				// 적용
	DST_PRIVATESHOP_BUSINESS_TRADEBOX_CANCEL,				// 취소

	// PrivateShop Cart Text
	DST_PRIVATESHOP_CART_BUY,				// 사기
	DST_PRIVATESHOP_CART_PRICE,				// 총 구입 금액

	// MailSystem Text
	DST_MAILSYSTEM_STATUS,						// 메일박스
	DST_MAILSYSTEM_STATUS_UNREAD,				// 읽지 않은 메일이 %u통 있습니다.
	DST_MAILSYSTEM_STATUS_FULL,					// 메일함이 수신용량을 초과하였습니다.
	DST_MAILSYSTEM_MAILBOX,						// 메일박스

	DST_MAILSYSTEM_TAB_MAILBOX,					// 메일박스
	DST_MAILSYSTEM_TAB_SENDMAIL,				// 메일쓰기
	DST_MAILSYSTEM_TAB_READMAIL,				// 메일읽기
	DST_MAILSYSTEM_MAIL_EMPTYMAIL,				// 메일이 한 통도 없습니다.
	DST_MAILSYSTEM_MAIL_EXPIRED_DAY_TITLE,		// 남은 기간
	DST_MAILSYSTEM_MAIL_EXPIRED_DAY,			// %u 일
	DST_MAILSYSTEM_MAIL_EXPIRED_DAY_LOCK,		// 잠김
	DST_MAILSYSTEM_MAIL_TO,						// 받는사람
	DST_MAILSYSTEM_MAIL_FROM,					// 보낸 사람
	DST_MAILSYSTEM_MAIL_CONTENT,				// 메일 내용
	DST_MAILSYSTEM_MAIL_ATTACH,					// 첨부하기
	DST_MAILSYSTEM_MAIL_ATTACH_2,				// 첨부된 아이템
	DST_MAILSYSTEM_MAIL_ATTACH_SEND,			// 송금하기 
	DST_MAILSYSTEM_MAIL_ATTACH_REQ,				// 청구하기
	DST_MAILSYSTEM_MAIL_SEND,					// 보내기
	DST_MAILSYSTEM_MAIL_RESET,					// 다시쓰기
	DST_MAILSYSTEM_MAIL_AWAY,					// 자리비움
	DST_MAILSYSTEM_MAIL_AWAY_ON,				// 현재 부재중으로 설정되어 있으며, 다른 플레이어로부터 메일을 수신하지 않습니다.
	DST_MAILSYSTEM_MAIL_AWAY_OFF,				// 현재 메일 수신이 가능한 상태입니다.
	DST_MAILSYSTEM_MAIL_REPLY,					// 답장하기
	DST_MAILSYSTEM_MAIL_RETURN,					// 반송하기
	DST_MAILSYSTEM_MAIL_DELETE,					// 지우기

	DST_MAILSYSTEM_MAIL_ZENNY,					// 금액
	DST_MAILSYSTEM_MAIL_ZENNY_SEND,				// 이체금액
	DST_MAILSYSTEM_MAIL_ZENNY_REQ,				// 청구금액

	DST_MAILSYSTEM_MAIL_SENDDATE,				// %u년 %u월 %u일 %s %u시 %u분

	DST_MAILSYSTEM_NOT_IDENTIFICATION,			// 미확인
	DST_MAILSYSTEM_NOT_IDENTIFICATION_ITEM,		// 미확인 아이템

	DST_MAILSYSTEM_TOOLTIP_NOT_ATTACH,			// 첨부할 아이템을 슬롯에 등록하세요
	DST_MAILSYSTEM_TOOLTIP_EXPIREDDAY,			// 해당 기간이 지나면 본 메일은 반송 또는 자동삭제 됩니다.
	DST_MAILSYSTEM_TOOLTIP_ATTACHACTION,		// 첨부된 물품을 받거나 지불 합니다.
	DST_MAILSYSTEM_TOOLTIP_RELOAD,				// 새로고침 : 1분마다 활성화 시킬 수 있습니다.

	DST_MAILSYSTEM_ATTACH_DISABLED_MSG,			// 물질전송칩을 착용하거나 우체통을 이용하여 주십시오.

	DST_MAILSYSTEM_NEGATIVE_CHECK_LOCK,			// 잠겨진 메일은 선택할 수 없습니다.
	DST_MAILSYSTEM_NEGATIVE_CHECK_ATTACH,		// 첨부된 물품이 있는 메일은 선택할 수 없습니다.

	// Skill Gui, Skill Info
	DST_SKILL_HAVE_MORE_EFFECT,		// 외 %d개 효과
	DST_SKILL_CONSUME_TEXT,			// 소모/
	DST_SKILL_LP_CONSUMPTION,		// LP소모
	DST_SKILL_EP_CONSUMPTION,		// EP소모
	DST_SKILL_RP_CONSUMPTION,		// RP소모
	DST_SKILL_GRADE,				// 등급표시
	DST_SKILL_GRADE_MASTERED,		// 마스터등급
	DST_SKILL_NOT_LEARN,			// 스킬을 배우지 않았다.
	DST_SKILL_READY_LEARN,			// 스킬을 배울 수 있다.
	DST_SKILL_READY_UPGRADE,		// 스킬 업그레이드가 가능하다.
	DST_SKILL_CANT_LEARN,			// 스킬을 배울 수 없다.
	DST_SKILL_ALREADY_LEARNED,		// 이미 스킬을 배웠다.
	DST_SKILL_CANT_LEARN_FOREVER,	// 배울 수 없는 스킬입니다.
	DST_SKILL_MASTERED,				// 스킬을 마스터했다.
	DST_SKILL_TYPE_PASSIVE,			// 스킬타입
	DST_SKILL_TYPE_ACTIVE,			
	DST_SKILL_TYPE_HTB,	
	DST_SKILL_CASTTIME,				// 시전시간
	DST_SKILL_DURATIONTIME,			// 지속시간
	DST_SKILL_COOLTIME,				// 쿨타임	
	DST_SKILL_USERANGE,				// 사용거리
	DST_SKILL_EXP,					// 숙련도
	DST_SKILL_SP,					// 스킬포인트 : %u

	DST_SKILL_NEED_TEXT,			// 습득조건
	DST_SKILL_NEED_LEVEL,			// 캐릭터 레벨 %3d 이상
	DST_SKILL_NEED_SKILL,			// %s %d등급 이상 습득
	DST_SKILL_NEED_MASTERSKILL,		// %s 습득
	DST_SKILL_TOTAL_DAMAGE,			// 총 대미지 : %d 
	DST_SKILL_APPLY_INSTANCE_ENEMY,	// 적을 대상으로 즉시 시전

	DST_SKILL_TARGET_AND_RANGE_INFO1,	// 스킬 타겟 설명.
	DST_SKILL_TARGET_AND_RANGE_INFO2,
	DST_SKILL_TARGET_AND_RANGE_INFO3,
	DST_SKILL_TARGET_AND_RANGE_INFO4,
	DST_SKILL_TARGET_AND_RANGE_INFO5,
	DST_SKILL_TARGET_AND_RANGE_INFO6,
	DST_SKILL_TARGET_AND_RANGE_INFO7,
	DST_SKILL_TARGET_AND_RANGE_INFO8,
	DST_SKILL_TARGET_AND_RANGE_INFO9,
	DST_SKILL_TARGET_AND_RANGE_INFO11,
	DST_SKILL_TARGET_AND_RANGE_INFO12,
	DST_SKILL_TARGET_AND_RANGE_INFO13,
	DST_SKILL_TARGET_AND_RANGE_INFO14,
	DST_SKILL_TARGET_AND_RANGE_INFO15,
	DST_SKILL_TARGET_AND_RANGE_INFO16,
	DST_SKILL_TARGET_AND_RANGE_INFO17,
	DST_SKILL_TARGET_AND_RANGE_INFO18,
	DST_SKILL_TARGET_AND_RANGE_INFO19,
	DST_SKILL_TARGET_AND_RANGE_INFO20,
	DST_SKILL_TARGET_AND_RANGE_INFO21,
	DST_SKILL_TARGET_AND_RANGE_INFO22,
	DST_SKILL_TARGET_AND_RANGE_INFO23,
	DST_SKILL_TARGET_AND_RANGE_INFO24,
	DST_SKILL_TARGET_AND_RANGE_INFO25,
	DST_SKILL_TARGET_AND_RANGE_INFO26,
	DST_SKILL_TARGET_AND_RANGE_INFO27,
	DST_SKILL_TARGET_AND_RANGE_INFO28,
	DST_SKILL_TARGET_AND_RANGE_INFO29,
	DST_SKILL_TARGET_AND_RANGE_INFO30,
	DST_SKILL_TARGET_AND_RANGE_INFO31,
	DST_SKILL_TARGET_AND_RANGE_INFO32,
	DST_SKILL_TARGET_AND_RANGE_INFO33,
	DST_SKILL_TARGET_AND_RANGE_INFO34,
	DST_SKILL_TARGET_AND_RANGE_INFO35,
	DST_SKILL_TARGET_AND_RANGE_INFO36,
	DST_SKILL_TARGET_AND_RANGE_INFO37,
	DST_SKILL_TARGET_AND_RANGE_INFO38,
	DST_SKILL_TARGET_AND_RANGE_INFO39,
	DST_SKILL_TARGET_AND_RANGE_INFO40,
	DST_SKILL_TARGET_AND_RANGE_INFO41,

	DST_SKILL_MUST_EQUIP_ITEM,		// 필수장착아이템 : %s

	DST_SKILL_MASTER_LEARN,			// 스킬마스터 배우기버튼 인포텍스트
	DST_SKILL_MASTER_UPGRADE,		// 스킬마스터 업그레이드버튼 인포텍스트
	DST_SKILL_RPMODE,				// 스킬윈도우 RPMODE버튼, 스킬어빌리티 설정 

	DST_SKILL_MASTER_WINDOW_TITLE,	// 스킬 트레이너
	DST_SKILL_WINDOW_TITLE,			// 스킬 윈도우

	DST_SKILL_TAB_BATTLE_SKILL,		// 전투스킬
	DST_SKILL_TAB_SUB_SKILL,		// 보조스킬
	DST_SKILL_TAB_ACTION_SKILL,		// 액션스킬
	DST_SKILL_FILTER_ALL,			// 전체
	DST_SKILL_FILTER_ACTION,		// 필살기
	DST_SKILL_FILTER_HTB,			// HTB
	DST_SKILL_FILTER_SUB,			// 보조기
	DST_SKILL_FILTER_PASSIVE,		// 잠재기
	DST_SKILL_FILTER_SOCIAL,		// 소셜액션
	DST_SKILL_FILTER_ETC,			// 기타액션	

	DST_SKILL_TEXT_EMOTION,			// 감정 
	DST_SKILL_TEXT_ACTION,			// 액션

	DST_SKILL_SP_INFO_CURRENT_GRADE,	// 현재 랭크 : %u
	DST_SKILL_SP_INFO_NEED_SP,			// 랭크업에 필요한 SP : 
	DST_SKILL_SP_INFO_NEED_SP_LEARN,	// 배우는데 필요한 SP : 
	DST_SKILL_SP_INFO_MASTER,			// 현재 랭크 : %u ( 마스터 )
	DST_SKILL_SP_INFO_NEED_SKILL,		// 랭크업에 필요한 스킬 : 
	DST_SKILL_SP_INFO_NEED_SKILLGRADE,  // "%s %d등급"
	DST_SKILL_SP_INFO_NEED_LEARN,		// 스킬을 스킬마스터에게 배워야 합니다.
	DST_SKILL_SP_INFO_CANT_LEARN,		// 아직 스킬을 배울 수 없습니다.
	DST_SKILL_SP_INFO_CANT_SELFTRAIN,	// 아직 스킬을 터득할 수 없습니다.
	DST_SKILL_SP_INFO_UPGRADE,			// 스킬을 업그레이드 할 수 있습니다.	
	DST_SKILL_SP_INFO_SELFTRAIN,		// 스킬을 스스로 배울 수 있습니다.
	DST_SKILL_SP_INFO_AVATAR_LEVEL,		// 랭크업에 필요한 레벨 :
	DST_SKILL_SP_INFO_CANT_LEARN_BY_CLASS,	// 클래스가 달라 배울 수 없습니다. 

	// RP Bonus Tool Tip
	DST_SKILL_RPBONUS_TOOLTIP_MANUAL,				// Manual의 설명
	DST_SKILL_RPBONUS_TOOLTIP_KNOCKDOWN,			// KnockDown
	DST_SKILL_RPBONUS_TOOLTIP_RESULT_ABSOLUTE,		// Result Absolute
	DST_SKILL_RPBONUS_TOOLTIP_RESULT_PERCENT,		// Result Percent
	DST_SKILL_RPBONUS_TOOLTIP_KEEPTIME_PLUS,		// KeepTime Plus
	DST_SKILL_RPBONUS_TOOLTIP_CASTING_TIME_MINUS,	// CastingTime Plus
	DST_SKILL_RPBONUS_TOOLTIP_COOLTIME_MINUS,		// Cooltime Minus
	DST_SKILL_RPBONUS_TOOLTIP_EP_MINUS,				// Ep Minus
	DST_SKILL_RPBONUS_TOOLTIP_GUARD_CRASH,			// Guard Crash의 툴팁
	DST_SKILL_RPBONUS_TOOLTIP_INVALID,				// Invalid == 사용안함

	// Rp Bonus Gui text
	DST_SKILL_RPBONUS_TEXT_TITLE,					// 스킬 모드 설정
	DST_SKILL_RPBONUS_TEXT_MANUAL,					// 수동 모드
	DST_SKILL_RPBONUS_TEXT_DISABLE,					// 사용 안함

	// Rp Bonus Gui InfoWnd Text
	DST_INFOWND_RPBONUS_USE,						// 스킬어빌리티 사용시
	DST_INFOWND_RPBONUS_KNOCKDOWN,					// 넉다운 어빌리티 
	DST_INFOWND_RPBONUS_KEEPTIME,					// 유지시간 증가 어빌리티
	DST_INFOWND_RPBONUS_CASTTIME,					// 발동시간 감소 어빌리티
	DST_INFOWND_RPBONUS_COOLTIME,					// 쿨타임 감소 어빌리티
	DST_INFOWND_RPBONUS_EP,							// EP 감소 어빌리티
	DST_INFOWND_RPBONUS_POWER,						// 파워 증가 어빌리티
	DST_INFOWND_RPBONUS_GUARDCRASH,					// 방어 격파

	// Rp Bonus Gui Guide Text
	DST_SKILL_ABILITY_ACCEPT,						// 스킬 어빌리티 설정이 적용되었습니다.

	// ITEM Gui, Item Info
	DST_ITEM_GRADE_NORMAL,			// 아이템등급
	DST_ITEM_GRADE_SUPERIOR,
	DST_ITEM_GRADE_EXCELLENT,
	DST_ITEM_GRADE_RARE,
	DST_ITEM_GRADE_LEGENDARY,
	DST_ITEM_EQUIP_CLASS_TEXT,		// 착용종족
	DST_ITEM_EQUIP_LEVEL_TEXT,		// 제한레벨
	DST_ITEM_EQUIP_SLOT_TEXT,		// 착용 슬롯 : %s
	DST_ITEM_NEED_STR,				// 필요힘
	DST_ITEM_NEED_CON,				// 필요건강
	DST_ITEM_NEED_FOC,				// 필요집중
	DST_ITEM_NEED_DEX,				// 필요민첩
	DST_ITEM_NEED_SOL,				// 필요정신
	DST_ITEM_NEED_ENG,				// 필요원기
	DST_ITEM_PHYSICAL_OFFENCE,		// 물리공격
	DST_ITEM_ENERGY_OFFENCE,		// 원기공격
	DST_ITEM_PHYSICAL_DEFENCE,		// 물리방어
	DST_ITEM_ENERGY_DEFENCE,		// 원기방어
	DST_ITEM_ATTACK_RANGE,			// 공격거리 %d 미터
	DST_ITEM_CLOSE_ATTACK_RANGE,	// 근접공격
	DST_ITEM_ATTACK_SPEED,			// 공격속도
	DST_ITEM_ATTR_INFO,				// 속성 : %s
	DST_ITEM_ATTR_NONE,				// 무
	DST_ITEM_ATTR_HONEST,			// 순수			// 속성 : 순수 -> 순수 로..
	DST_ITEM_ATTR_STRANGE,			// 기이			// 밑에도 마찬가지.
	DST_ITEM_ATTR_WILD,
	DST_ITEM_ATTR_ELEGANCE,
	DST_ITEM_ATTR_FUNNY,
	DST_ITEM_BUY_COST,				// 구입비 : %%u
	DST_ITEM_SELL_COST,				// 판매비 : %%u
	DST_ITEM_REPAIR_COST,			// 수리비 : %%u
	DST_ITEM_DURABILITY,			// 아이템내구 "내구도:현재/최대"
	DST_ITEM_JUST_DURABILITY,		// 아이템내구 "내구도:최대"
	DST_ITEM_ADDITIONAL_EFFECT,		// 추가효과	
	DST_ITEM_SPECIAL_CLASS,			// 특화클래스
	DST_ITEM_SPECIAL_RACE,			// 특화종족
	DST_ITEM_SPECIAL_EFFECT,		// 특화효과 : +20%
	DST_ITEM_OPTION_EFFECT,			// 옵션효과
	DST_ITEM_UNIDENTIFIED,			// 미확인아이템
	DST_ITEM_CAN_EQUIP,				// %s 착용가능
	DST_ITEM_ALL_RACE,				// 모든 종족
	DST_ITEM_ALL_RACE_ENABLE_EQUIP_BUT_NMY,	// 모든 종족 착용가능\n (나메크 용족 제외)
	DST_ITEM_PHYSICAL_CLASS,		// 타격계열 클래스
	DST_ITEM_ENERGY_CLASS,			// 기공계열 클래스
	DST_ITEM_DIFF_STATUS,			// + %u
	DST_ITEM_LIMITED_SELL,			// 판매 불가능
	DST_ITEM_LIMITED_USER_TRADE,	// 유저거래 불가능
	DST_ITEM_LIMITED_STORE_WAREHOUSE,// 창고저장 불가능
	DST_ITEM_LIMITED_STORE_COMMON_WAREHOUSE,//공유창고저장 불가능
	DST_ITEM_IDENTIFY_MUST_IDENTIFY,		// 스카우터를 이용하거나 상점에서\\n 아이템을 감정해야 합니다(미확인 아이템 인포창)
	DST_ITEM_IDENTIFY_COST,			// 감정비 : %u
	DST_ITEM_FUEL_EFFICIENCY,		// 연비 %d초당 한 개 소비
	DST_ITEM_MOVE_SPEED,			// 이동속도 %d
	DST_ITEM_ALREADY_EQUIPPED,		// 장착중인 아이템

	// NPCShop Gui
	DST_NPCSHOP_MYZENNY,			// 나의 소지금
	DST_NPCSHOP_MUDOA_POINT,		// 나의 무도사 포인트
	DST_NPCSHOP_SHOPINGCART,		// 쇼핑 카트
	DST_NPCSHOP_ITEM_REPAIR,		// 아이템 수리
	DST_NPCSHOP_ITEM_ALL_REPAIR,	// 아이템 전체수리
	DST_NPCSHOP_ITEM_IDENTIFY,		// 아이템 감정

	DST_NPCSHOP_LARGE_BUY_EXPLAIN,			// Ctrl + 마우스 오른쪽 클릭 : 20개 구입
	DST_NPCSHOP_NO_MORE_REPAIR_REG_ITEM,	// 현재 착용한 모든 아이템은 수리할 필요가 없습니다(모든 아이템 수리 버튼 툴팁)

	// Trade Cart Gui
	DST_TRADECART_TOTAL,			// 총 금액
	DST_TRADECART_BUY,				// 사 기
	DST_TRADECART_SELL,				// 팔 기	

	// Status Gui
	DST_STATUS_WINDOW_TITLE,		// 내 정보
	DST_STATUS_TAB_AVATAR,			// 아바타
	DST_STATUS_TAB_SUMMON,			// 소환수
	DST_STATUS_TAB_FAMILY,			// 패밀리 

	DST_STATUS_NAME,				// 이름
	DST_STATUS_JOB,					// 직업
	DST_STATUS_LEVEL,				// 레벨
	DST_STATUS_AP,					// 전투력

	DST_STATUS_PHYSICAL_ATTACK,		// 타격공격력
	DST_STATUS_PHYSICAL_DEFENCE,	// 타격방어력
	DST_STATUS_PHYSICAL_CRITICAL,	// 타격크리티컬
	DST_STATUS_ENERGY_ATTACK,		// 기공공격력
	DST_STATUS_ENERGY_DEFENCE,		// 기공방어력
	DST_STATUS_ENERGY_CRITICAL,		// 기공크리티컬

	DST_STATUS_ATTACK_RATE,			// 명중율
	DST_STATUS_DODGE,				// 회피율

	DST_STATUS_ATTR,				// 속성
	DST_STATUS_STAT_STR,			// STR
	DST_STATUS_STAT_DEX,			// DEX
	DST_STATUS_STAT_CON,			// CON
	DST_STATUS_STAT_ENG,			// ENG
	DST_STATUS_STAT_SOL,			// SOL
	DST_STATUS_STAT_FOC,			// FOC

	DST_STATUS_ATTACK,				// 공격력 (Pet에서 사용)

	DST_STATUS_UNIDENTIFIED_LPEP,	// 측정안했을때 LPEP ?? / ??
	DST_STATUS_UNIDENTIFIED_NAME,	// 스카우터 측정 안했을때 이름

	DST_EQUIP_SLOT_TYPE_HAND,		// 주무기
	DST_EQUIP_SLOT_TYPE_SUB_WEAPON, // 보조무기
	DST_EQUIP_SLOT_TYPE_JACKET,		// 상의
	DST_EQUIP_SLOT_TYPE_PANTS,		// 하의
	DST_EQUIP_SLOT_TYPE_BOOTS,		// 신발
	DST_EQUIP_SLOT_TYPE_SCOUTER,	// 스카우터
	DST_EQUIP_SLOT_TYPE_DOGI,		// 도복
	DST_EQUIP_SLOT_TYPE_NECKLACE,	// 목걸이
	DST_EQUIP_SLOT_TYPE_EARRING,	// 귀걸이
	DST_EQUIP_SLOT_TYPE_RING,		// 반지	

	DST_STATUS_CURRENT_EXP,			// 현재
	DST_STATUS_GOAL_EXP,			// 목표

	// Status Window Tooltip
	DST_STATUS_TOOLTIP_ATTR,		// 속성
	DST_STATUS_TOOLTIP_STR,			// 힘
	DST_STATUS_TOOLTIP_DEX,			// 민첩
	DST_STATUS_TOOLTIP_CON,			// 근성
	DST_STATUS_TOOLTIP_ENG,			// 기운
	DST_STATUS_TOOLTIP_SOL,			// 기합
	DST_STATUS_TOOLTIP_FOC,			// 집중

	DST_STATUS_TOOLTIP_PHYSICAL_ATTACK,		// 타격 공격력
	DST_STATUS_TOOLTIP_PHYSICAL_DEFENCE,	// 타격 방어력
	DST_STATUS_TOOLTIP_PHYSICAL_CRITICAL,	// 타격 크리티컬 확률
	DST_STATUS_TOOLTIP_ENERGY_ATTACK,		// 기공 공격력
	DST_STATUS_TOOLTIP_ENERGY_DEFENCE,		// 기공 방어력
	DST_STATUS_TOOLTIP_ENERGY_CRITICAL,		// 기공 크리티컬 확률
	DST_STATUS_TOOLTIP_ATTACK_RATING,	// 명중률
	DST_STATUS_TOOLTIP_DODGE_RATING,	// 회피율

	// Status Window Honor Tab
	DST_STATUS_HONOR_TITLE_HONOR,		// 명예
	DST_STATUS_HONOR_TITLE_RANKBATTLE,	// 랭크배틀

	DST_STATUS_HONOR_HTMLTAG_HONOR,		// (HTML Tag) %s[br][br]%s
	DST_STATUS_HONOR_HONOR_SCORE,		// 명예점수 : %d
	DST_STATUS_HONOR_GUIDE_HONOR,		// 명예점수는 레이드, 천하제일 무도회 .....

	DST_STATUS_HONOR_HTMLTAG_RANKBATTLE,	// (HTML Tag) %s[br][br]%s[br]%s
	DST_STATUS_HONOR_PRIVATE_SCORE,		// 전적 : %d전 %d승 %d패
	DST_STATUS_HONOR_RANK_SCORE,		// 랭크포인트 : %d 점
	DST_STATUS_HONOR_MUDOSA_SCORE,		// 무도사포인트 : %d 점

	// Status Window Technic Tab
	DST_STATUS_TECHNIC_TITLE,					// 테크닉
	DST_STATUS_TECHNIC_HOIPOIMIX_TITLE,			// 호이포이믹스
	DST_STATUS_TECHNIC_HOIPOIMIX_MIXLEVEL,		// 믹스 레벨
	DST_STATUS_TECHNIC_HOIPOIMIX_MIXEXPDATA,	// 믹스 경험치
	DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL,		// 일반 제조 기술
	DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE,	// 10레벨 이상 습득 가능
	DST_STATUS_TECHNIC_HOIPOIMIX_NORMAL_SKILL_GUIDE_HTML,	// (HTML) 믹스 마스터에게서 배울 수 있으며....
	DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL,		// 특수 제조 기술
	DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL_GUIDE,	// 15레벨 이상 습득 가능
	DST_STATUS_TECHNIC_HOIPOIMIX_SPECIAL_SKILL_GUIDE_HTML,	// (HTML) 믹스 마스터에게서 배울수 있으며...

	DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_FOOD_HTML,		// (HTML) 음식 제조 가능
	DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_MAIN_HTML,		// (HTML) 주무기 제조가능
	DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_ARMOR_HTML,		// (HTML) 방어구 제조 가능
	DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_ACCESSORY_HTML,	// (HTML) 악세서리 제조 가능
	DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_SCOUTER_HTML,	// (HTML) 스카우터 제조 가능
	DST_STATUS_TECHNIC_HOIPOIMIX_SKILL_SUB_WEAPON_HTML,	// (HTML) 부무기 제조 가능


	// Pet Status Gui
	DST_PET_STATUS_PHYSICAL_OFFENSE, // 타격공격력
	DST_PET_STATUS_PHYSICAL_DEFENSE, // 타격방어력
	DST_PET_STATUS_ENERGY_OFFENSE, // 기공공격력
	DST_PET_STATUS_ENERGY_DEFENSE, // 기공방어력

	// PC_Race
	DST_HUMAN,						// 인간
	DST_NAMEK,						// 나메크인
	DST_MAJIN,						// 마인

	// PC_Class
	DST_HUMAN_FIGHTER,
	DST_HUMAN_MYSTIC,
	DST_ENGINEER,
	DST_NAMEK_FIGHTER,
	DST_NAMEK_MYSTIC,
	DST_MIGHTY_MAJIN,
	DST_WONDER_MAJIN,
	DST_STREET_FIGHTER,
	DST_SWORD_MASTER,
	DST_CRANE_ROSHI,
	DST_TURTLE_ROSHI,
	DST_GUN_MANIA,
	DST_MECH_MANIA,
	DST_DARK_WARRIOR,
	DST_SHADOW_KNIGHT,
	DST_DENDEN_HEALER,
	DST_POCO_SUMMONER,
	DST_ULTI_MA,
	DST_GRAND_MA,
	DST_PLAS_MA,
	DST_KAR_MA,

	// Action Type (Action Info Window)
	DST_ACTION_TYPE_NORMAL,			// 노멀액션
	DST_ACTION_TYPE_SOCIAL,			// 소셜액션
	DST_CHAT_COMMAND,				// "채팅명령 : "

	// 
	DST_SHENRON,					// 신룡

	// Scouter Parts Explain for ScouterBody
	DST_SCOUTERPARTS_NORMAL_START_LP,// LP + %d
	DST_SCOUTERPARTS_EP,			// EP + %d
	DST_SCOUTERPARTS_STR,			// 힘 + %d
	DST_SCOUTERPARTS_CON,			// 근성 + %d
	DST_SCOUTERPARTS_FOC,			// 집중 + %d
	DST_SCOUTERPARTS_DEX,			// 민첩 + %d
	DST_SCOUTERPARTS_SOL,			// 기합 + %d
	DST_SCOUTERPARTS_ENG,			// 기력 + %d
	DST_SCOUTERPARTS_ATTACK_SPEED,	// 공격 속도 + %d%%
	DST_SCOUTERPARTS_ATTACK_RATE,	// 명중 + %d%%
	DST_SCOUTERPARTS_DODGE_RATE,	// 회피 + %d%%
	DST_SCOUTERPARTS_BLOCK_RATE,	// 방어 + %d%%
	DST_SCOUTERPARTS_CURSESUCCESS_RATE,	// 상태이상 성공 + %d%%
	DST_SCOUTERPARTS_CURSE_TOLERANCE_RATE,	// 상태이상 저항 + %d%%
	DST_SCOUTERPARTS_UPGRADE_RATE_UP,		// 조합효율 %02.02f배 증가
	DST_SCOUTERPARTS_ITEM_BREAK_RATE_DOWN,	// 조합 실패시 아이템 파괴 확률 %02.02f배 감소

	DST_SCOUTERPARTS_BEST_START,

	DST_SCOUTERPARTS_SPECIAL_START,
	DST_SCOUTERPARTS_PC_INFO_JAMMING,			// 엿보기 방해
	DST_SCOUTERPARTS_FIND_OBJECT,				// 숨겨진 물건 찾기	
	DST_SCOUTERPARTS_MAIL_CONNECTOR,			// 우편함 접속
	DST_SCOUTERPARTS_MAIL_TRANSMITTER,			// 물질 전송
	DST_SCOUTERPARTS_FIND_MOB_TO_GET_DRAGONBALL,// 드래곤볼 몬스터 표시
	DST_SCOUTERPARTS_ITEM_IDENTIFICATION,		// 아이템 감정	
	DST_SCOUTERPARTS_PC_INFO_VIEWER,			// 수행자 엿보기
	DST_SCOUTERPARTS_QUEST_SEARCH,				// 주변 퀘스트 검색
	// End of 'Scouter Parts Explain for ScouterBody

	// Explain effect for charm buff
	DST_CHARM_EXP,								// 경험치 %d%% 추가 획득
	DST_CHARM_COOLTIME,							// 스킬 쿨타임 %d%% 감소
	DST_CHARM_DROPRATE,							// 아이템 드롭 확률 %d%% 증가
	DST_CHARM_RP_SHARING,						// RP 공유 %d 공유
	// End of effect Explain charm buff

	// Community
	DST_COMMUNITY_NAME,							// 커뮤니티
	DST_TAB_PARTY,								// 파티
	DST_TAB_GUILD,								// 길드
	DST_TAB_RAID,								// 레이드

	// Party UI
	DST_PARTYGUI_PARTYLEADER,					// 파티장

	DST_PARTYGUI_ITEM_LOOTING,					// 아이템 분배 방식 변경
	DST_PARTYGUI_ITEM_LOOTING_PICKUP_PERSON,	// 줍는 사람 획득
	DST_PARTYGUI_ITEM_LOOTING_IN_ORDER,			// 순서되로 분배
	DST_PARTYGUI_ITEM_LOOTING_IN_GRADE,			// 등급별 순서되로 분배
	DST_PARTYGUI_ITEM_LOOTING_BY_LEADER,		// 파티장이 분배
	DST_PARTYGUI_ITEM_LOOTING_BY_ZENNY,			// 경매
	DST_PARTYGUI_ITEM_LOOTING_BY_DICE,			// 주사위 굴리기

	DST_PARTYGUI_ZENNY_LOOTING,					// 제니 분배 방식 변경
	DST_PARTYGUI_ZENNY_LOOTING_PICKUP_PERSON,	// 줍는 사람 획득
	DST_PARTYGUI_ZENNY_LOOTING_EQUAL,			// 똑같이 나누기

	DST_PARTYGUI_MEMBER,						// 파티 맴버

	DST_PARTYGUI_CHARM,							// 파티 부적
	DST_PARTYGUI_DEST_ZENNY,					// 목표금액
	DST_PARTYGUI_CHARM_EXPLAIN,					// 파티부적을 사용하여 파티원 전체가 특수한 버프효과를 얻을 수 있습니다
	DST_PARTYGUI_INVEN,							// 파티 인벤토리	
	DST_PARTYGUI_INVEN_COMPETITOR,				// 기여점수를 투자한 유저수
	DST_PARTYGUI_INVEN_MYSCORE,					// 내가 투자한 기여점수
	DST_PARTYGUI_INVEN_EXCUTE_DIVIDE,			// 분배
	DST_PARTYGUI_INVEN_LOCK,					// 경매 중지
	DST_PARTYGUI_INVEN_UNLOCK,					// 경매 시작
	DST_PARTYGUI_INVEN_SUCCESSFUL_BID,			// 낙찰
	DST_PARTYGUI_INVEN_RESET_OCTION,			// 경매 정보 초기화
	DST_PARTYGUI_INVEN_ITEM_GRADE,				// 보관(파티 인벤의 아이템 등급 설정할 때)

	DST_PARTYGUI_LEAVE_BUTTON,					// 파티 탈퇴
	DST_PARTYGUI_INVITE_BUTTON,					// 파티 초대	
	DST_PARTYGUI_SWITCH,						// 보기 설정
	DST_PARTYGUI_DUNGEON,						// 파티 던전
	DST_PARTYGUI_SPREAD,						// 메뉴 펼치기
	DST_PARTYGUI_FOLD,							// 메뉴 접기

	DST_PARTYGUI_CHARM_USE,						// 부적사용

	DST_PARTY_DUNGEON_NORMAL,					// 보통
	DST_PARTY_DUNGEON_HARD,						// 어려움

	// Scouter GUI
	DST_SCOUTER_SCOUTER_NAME,					// 스카우터
	DST_SCOUTER_PARTS_NAME,						// 파츠
	DST_SCOUTER_PARTS_OPTION,					// 파츠 옵션
	DST_SCOUTER_SCOUTER_ABILITY,				// 스카우터 성능
	DST_SCOUTER_POWER,							// 사용량
	DST_SCOUTER_ENDURANCE,						// 내구도	
	DST_SCOUTER_POWER_MAX_INFO,					// 최대 스카우터 용량 %d	( 미장착상태에서의  표시 )
	DST_SCOUTER_POWER_CURRENT_INFO,				// 스카우터 용량 %d / %d ( 장착시 현재용량도 같이 표시 )
	DST_SCOUTER_PARTS_POWER_CUNSUMPTION,		// 전력 소모량 %d
	DST_SCOUTER_REMOVER_ALL_PARTS,				// 모든 파츠 제거


	// ItemUpgrade Gui
	DST_ITEMUPGRADE_DLGTITLE,					// 아이템 업그레이드
	DST_ITEMUPGRADE_INFOTITLE,					// 업그레이드 상황
	DST_ITEMUPGRADE_PREDICT,					// 확률 예측
	DST_ITEMUPGRADE_DESTROY_PREDICT,			// 소멸 예측
	DST_ITEMUPGRADE_BTN_COMPOUND,				// 업그레이드
	DST_ITEMUPGRADE_BTN_COMPOUND_DOWN,			// 다운그레이드
	DST_ITEMUPGRADE_BTN_CANCEL,					// 취소

	DST_ITEMUPGRADE_PREDICT_VERYGOOD,			// 매우 좋음
	DST_ITEMUPGRADE_PREDICT_GOOD,				// 좋음
	DST_ITEMUPGRADE_PREDICT_NORMAL,				// 보통
	DST_ITEMUPGRADE_PREDICT_BAD,				// 별로
	DST_ITEMUPGRADE_PREDICT_POOR,				// 난감
	DST_ITEMUPGRADE_PREDICT_VERYPOOR,			// 최악
	DST_ITEMUPGRADE_PREDICT_HOPELESS,			// 절망

	// Item Upgrade MSG( Html 형식으로 써야 합니다 )
	DST_ITEMUPGRADE_MSG_SLOT_IS_EMPTY,			// 업그레이드 할 아이템과 호이포이스톤을 올려주십시오.	
	DST_ITEMUPGRADE_MSG_ITEM_IN,				// 호이포이스톤을 올려주십시오.
	DST_ITEMUPGRADE_MSG_MINLEVEL_ITEM_IN,		// 0레벨 아이템이 들어왔을때.
	DST_ITEMUPGRADE_MSG_MAXLEVEL_ITEM_IN,		// 맥스레벨 아이템이 들어왔을때.
	DST_ITEMUPGRADE_MSG_HOIPOISTONE_IN,			// 업그레이드 할 아이템을 올려주십시오.
	DST_ITEMUPGRADE_MSG_BLACK_HOIPOISTONE_IN,	// 다운그레이드 할 아이템을 올려주십시오.
	DST_ITEMUPGRADE_MSG_PURE_HOIPOISTONE_IN,	// 업그레이드 할 아이템을 올려주십시오. 퓨어 호이스톤은...
	DST_ITEMUPGRADE_MSG_UPGRADEREADY,			// 업그레이드가 가능합니다. 업그레이드 성공시[br]...
	DST_ITEMUPGRADE_MSG_DOWNGRADEREADY,			// 다운그레이드가 가능합니다. 다운그레이드 성공시[br]...
	DST_ITEMUPGRADE_MSG_USE_HOIPOISTONECOUNT,	// %s %d개 사용
	DST_ITEMUPGRADE_MSG_PHYSICAL_UP,
	DST_ITEMUPGRADE_MSG_ENERGY_UP,
	DST_ITEMUPGRADE_MSG_PURE_PHYSICAL_UP,		//
	DST_ITEMUPGRADE_MSG_PURE_ENERGY_UP,			//
	DST_ITEMUPGRADE_MSG_PHYSICAL_DOWN,
	DST_ITEMUPGRADE_MSG_ENERGY_DOWN,
	DST_ITEMUPGRADE_MSG_NO_CHANGE_PARAMETER,			// 변화없음.
	DST_ITEMUPGRADE_MSG_CHANGE_ATTR,					// 속성변화 ->
	DST_ITEMUPGRADE_MSG_NO_CHANGE_ATTR,					// 속성변화 없음.
	DST_ITEMUPGRADE_MSG_HOIPOISTONE_IN_FAIL_CLASS,		// 업그레이드의 재료인 호이포이 스톤의 레벨이 모자랍니다. 확인해 주십시오.
	DST_ITEMUPGRADE_MSG_BLACKHOIPOISTONE_IN_FAIL_CLASS,	// 다운그레이드의 재료인 블랙호이포이 스톤의 레벨이 모자랍니다. 확인해 주십시오.
	DST_ITEMUPGRADE_MSG_HOIPOISTONE_IN_FAIL_QUANTITY,		// 업그레이드에 필요한 호이포이 스톤이 모자랍니다. 확인해 주십시오.[br] %s : %s ( %d / %d ) 
	DST_ITEMUPGRADE_MSG_BLACKHOIPOISTONE_IN_FAIL_QUANTITY,	// 다운그레이드에 필요한 블랙호이포이 스톤이 모자랍니다. 확인해 주십시오.[br] %s : %s ( %d / %d ) 
	DST_ITEMUPGRADE_MSG_PUREHOIPOISTONE_IN_FAIL_QUANTITY,	// 업그레이드에 필요한 블랙호이포이 스톤이 모자랍니다. 확인해 주십시오.[br] %s : %s ( %d / %d ) 
	DST_ITEMUPGRADE_MSG_UNUPGRADABLE_ITEM,				// 아이템을 더이상 업그레이드 할 수 없습니다.
	DST_ITEMUPGRADE_MSG_UNDOWNGRADABLE_ITEM,			// 아이템을 더이상 다운그레이드 할 수 없습니다.
	// 여기까지 Html 형식으로 씁니다.

	DST_ITEMUPGRADE_RESULT_MSG_SUCCESS,			// 축하합니다./w아이템 업그레이드가 성공했습니다.
	DST_ITEMUPGRADE_RESULT_MSG_DOWNGRADE_SUCCESS,	// 아이템 다운그레이드가 성공했습니다. 
	DST_ITEMUPGRADE_RESULT_MSG_FAIL,			// 아이템 업그레이드가 실패했습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_UPGRADE,		// 아이템 등급( %d -> %d )이 올랐습니다.
	DST_ITEMUPGRADE_RESTLT_REPORT_DOWNGRADE,	// 아이템 등급( %d -> %d )이 내렸습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_STATUS_PHYSICAL_OFFENCE_UP,	// 아이템의 물리공격력( %d -> %d )이 올랐습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_STATUS_PHYSICAL_OFFENCE_DOWN, // 아이템의 물리공격력( %d -> %d )이 내렸습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_STATUS_ENERGY_OFFENCE_UP,		// 아이템의 기공공격력( %d -> %d )이 올랐습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_STATUS_ENERGY_OFFENCE_DOWN,	// 아이템의 기공공격력( %d -> %d )이 내렸습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_STATUS_PHYSICAL_DEFENCE_UP,	// 아이템의 물리방어력( %d -> %d )이 올랐습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_STATUS_PHYSICAL_DEFENCE_DOWN, // 아이템의 물리방어력( %d -> %d )이 내렸습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_STATUS_ENERGY_DEFENCE_UP,		// 아이템의 기공방어력( %d -> %d )이 올랐습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_STATUS_ENERGY_DEFENCE_DOWN,	// 아이템의 기공방어력( %d -> %d )이 내렸습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_ATTRIBUTE,	// 아이템의 속성( %s -> %s )이 변했습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_ITEM_DESTROY,	// 아이템이 파괴되었습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_NO_PHYSICAL_OFFENCE_CHANGE,	// 아이템 물리공격력의 변화가 없습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_NO_PHYSICAL_DEFENCE_CHANGE,	// 아이템 물리방어력의 변화가 없습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_NO_ENERGY_OFFENCE_CHANGE,		// 아이템 기공공격력의 변화가 없습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_NO_ENERGY_DEFENCE_CHANGE,		// 아이템 기공방어력의 변화가 없습니다.
	DST_ITEMUPGRADE_RESULT_REPORT_NO_ATTRIBUTE_CHANGE,	// 아이템 속성의 변화가 없습니다.

	DST_ITEMUPGRADE_INFO_ITEM_SLOT,						// 업그레이드할 장비 아이템을 넣는 곳입니다. 장비아이템의 등급에 따라 재료인...
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY_TITLE,		//  호이포이 스톤 슬롯 ( 첫문자 공백. )
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY_LINE1,		// 업그레이드 성공시
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY_LINE2_1,		// 속성으로 변경되며
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY_LINE2_2,		// 속성은
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_EMPTY_LINE3,		// 업그레이드 실패시 속성에 따라\n파괴 확률이 변하거나 무속성으로 변할 수 있습니다.
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_ATTR,				//  속성에 대해 공/방 ( 첫문자 공백 )
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_ATTR_BETTER,		//  %.0f%% 이득 ( 첫문자 공백. )
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_ATTR_WORSE,		//  %.0f%% 손해 ( 첫문자 공백. )
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_1_CHANGE,		// 업그레이드 성공시 아이템이 %s 속성으로 변합니다.
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_1_BLACKCHANGE,   // 아이템이 다운그레이드 되며 %s 속성으로 변합니다
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_1_NO_CHANGE,	// 업그레이드 성공시 속성은 변하지 않습니다.
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_1_NO_BLACKCHANGE,//아이템이 다운그레이드 되며 속성은 변하지 않습니다
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_COMMON,		// 속성 상성으로 인해 업그레이드 실패 시,\n
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_BEST,			// 아이템이 파괴될 확률이 많이 내려갑니다.
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_GOOD,			// 아이템이 파괴될 확률이 조금 내려갑니다.
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_LITTLE_GOOD,	// 아이템이 파괴될 확률이 아주 조금 내려갑니다.
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_LITTLE_BAD,	// 아이템이 파괴될 확률이 아주 조금 올라갑니다.
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_NORMAL,		// 아이템이 파괴될 확률에 영향을 주지 않습니다.
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_2_BAD,			// 아이템이 파괴될 확률이 조금 올라갑니다.
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_3,				// 업그레이드 실패시 아이템의 속성이 무속성으로 변합니다.
	DST_ITEMUPGRADE_INFO_HOIPOI_SLOT_IN_3_NO_CHANGE,	// 업그레이드 실패시 속성은 변하지 않습니다. 

	DST_ITEMUPGRADE_INFO_OFFENCE,				// 공격력
	DST_ITEMUPGRADE_INFO_DEFENCE,				// 방어력

	// Quest 
	DST_QUESTREWARD_INFO_EXP,					// 경험치 %d 획득
	DST_QUESTREWARD_INFO_ZENNY,					// 제니 %d 획득
	DST_QUESTREWARD_INFO_CLASS,					// %s 클래스로 변경
	DST_QUESTREWARD_INFO_REPUTATION,			// 명성 %d 획득
	DST_QUESTREWARD_PROB_REWARD,				// 확률보상
	DST_QUESTREWARD_TEXT_BASIC,					// 기본보상
	DST_QUESTREWARD_TEXT_SELECT,				// 선택보상
	DST_QUESTREWARD_CHANGE_ADULT,				// 어른으로 성장
	DST_QUESTREWARD_CONVERT_CLASS_RIGHT,		// 2차직으로 전직가능

	DST_QUESTLIST_TITLE,						// 퀘스트 리스트
	DST_QUESTLIST_INVENTITLE,					// 퀘스트 인벤토리
	DST_QUESTLIST_NOQUEST,						// 수행 가능한 퀘스트가 없습니다.

	DST_QUESTLIST_TIP_NEW_QUEST,				// 새 퀘스트
	DST_QUESTLIST_TIP_CLEARD_QUEST,				// 완료된 퀘스트
	DST_QUESTLIST_TIP_PROGRESS_QUEST,			// 진행중인 퀘스트
	DST_QUESTLIST_TIP_FAILD_QUEST,				// 실패한 퀘스트
	DST_QUESTLIST_TIP_ERROR_BLOCK,				// 퀘스트 에러

	DST_QUEST_TEXT_CONVERSATION,				// 대화
	DST_QUEST_TEXT_AIM,							// 목표

	DST_QUEST_GIVEUP_BTN,						// 포기
	DST_QUEST_CLOSE_BTN,						// 닫기
	DST_QUEST_ACCEPT_BTN,						// 수락
	DST_QUEST_REJECT_BTN,						// 거절
	DST_QUEST_REWARD_BTN,						// 보상
	DST_QUEST_CANCEL_BTN,						// 취소

	DST_QUEST_PROGRESS_WINDOW_TITLE,			// 퀘스트 진행
	DST_QUEST_LIMIT_TIME,						// 제한시간
	DST_QUEST_PROGRESS_INFO,					// 진행상황

	DST_QUEST_PROPOSAL_WINDOW_TITLE,			// 퀘스트 제안
	DST_QUEST_REWARD_WINDOW_TITLE,				// 퀘스트 보상

	DST_QUEST_REMAIN_TIME,						// 남은시간 %02d : %02d

	DST_QUEST_SHOW_QUEST_CATALOG,				// 퀘스트 목록 보이기
	DST_QUEST_HIDE_QUEST_CATALOG,				// 퀘스트 목록 감추기
	DST_QUEST_SHARE,							// 퀘스트 공유
	DST_QUEST_NEW,								// <새퀘스트>
	DST_QUEST_INDICATOR,						// 퀘스트 알리미
	DST_QUEST_NEXT_INVENTORY,					// 다음 
	DST_QUEST_PREV_INVENTORY,					// 이전

	DST_QUEST_CAT_TASK,							// 일반 퀘스트
	DST_QUEST_CAT_SCENARIO,						// 시나리오 퀘스트
	DST_QUEST_CAT_TIMEMACHINE,					// 타임머신 퀘스트
	DST_QUEST_CAT_DRAGONBALL,					// 드래곤볼 퀘스트
	DST_QUEST_CAT_JOBCHANGE,					// 전직 퀘스트
	DST_QUEST_CAT_TLQ,							// 타임리프 퀘스트

	DST_QUEST_SENDER,							// 의뢰처
	DST_QUEST_RECIEVER,							// 보고처

	// 캐릭터 메이킹 관련
	DST_AVATAR_SEX_MALE,						// 남자
	DST_AVATAR_SEX_FEMALE,						// 여자
	DST_AVATAR_SEX_NOT_EXIST,					// 성별 없음

	// Warehouse
	DST_WAREHOUSE_NAME_1,					// 창고 1
	DST_WAREHOUSE_NAME_2,					// 창고 2
	DST_WAREHOUSE_NAME_3,					// 창고 3
	DST_WAREHOUSE_NAME_COMMON,				// 공유 창고	
	DST_WAREHOUSE_ALL,						// 모든 창고

	// log in
	DST_LOGIN,								// 들어가기
	DST_LOGIN_ID,							// 아이디
	DST_LOGIN_PASSWARD,						// 비밀번호
	DST_LOGIN_PLAY_MOVIE,					// 동영상 재생
	DST_LOGIN_CREDIT,						// 제작진 보기
	DST_LOGIN_CONTRACT,						// 약 관
	DST_LOGIN_NEW_ACCOUNT,					// 새 계정 만들기

	// Lobby
	DST_LOBBY_CHARACTER_MAKE,				// 캐릭터 만들기
	DST_LOBBY_RANDOM_CHOICE,				// 자 동 선 택
	DST_LOBBY_MAKE,							// 만 들 기
	DST_LOBBY_RETURN,						// 돌아가기
	DST_LOBBY_EXIT,							// 나 가 기
	DST_LOBBY_RACE,							// 종   족
	DST_LOBBY_GENDER,						// 성   별
	DST_LOBBY_CLASS,						// 직   업
	DST_LOBBY_HAIR,							// 머리모양
	DST_LOBBY_HAIRCOLOR,					// 머리색
	DST_LOBBY_FACE,							// 얼   굴
	DST_LOBBY_SKIN,							// 피부색
	DST_LOBBY_HUMAN_EXPLAIN,				// 종족 설명
	DST_LOBBY_NAMEK_EXPLAIN,				// 종족 설명
	DST_LOBBY_MAJIN_EXPLAIN,				// 종족 설명
	DST_LOBBY_HUMAN_FIGHTER,				// 클래스 설명
	DST_LOBBY_HUMAN_MYSTIC,					// 클래스 설명
	DST_LOBBY_HUMAN_ENGINEER,				// 클래스 설명
	DST_LOBBY_NAMEK_FIGHTER,				// 클래스 설명
	DST_LOBBY_NAMEK_MYSTIC,					// 클래스 설명
	DST_LOBBY_MIGHTY_MAJIN,					// 클래스 설명
	DST_LOBBY_WONDER_MAJIN,					// 클래스 설명

	DST_LOBBY_CHANNEL,						// 채널
	DST_LOBBY_SERVER_NAME,					// 서버 이름
	DST_LOBBY_SERVER_STATE,					// 서버 상태
	DST_LOBBY_PERIOD,						// 계정 만료일까지 10일 남았습니다
	DST_LOBBY_UNTIL_DEL_CHAR,				// 캐릭터가 삭제되기까지 %d%s 남았습니다

	DST_LOBBY_SERVER_NOT_BUSY,				// 한가
	DST_LOBBY_SERVER_NORMAL,				// 보통
	DST_LOBBY_SERVER_BUSY,					// 바쁨
	DST_LOBBY_SERVER_EXCESS,				// 초과
	DST_LOBBY_SERVER_LOCK,					// 잠김
	DST_LOBBY_SERVER_CHECK,					// 점검

	DST_LOBBY_CHAR_LEVEL,					// 레벨
	DST_LOBBY_CHAR_CLASS,					// 클래스
	DST_LOBBY_ZENNY,						// 소유제니	
	DST_LOBBY_POSITION,						// 최종위치
	DST_LOBBY_DEL_DAT,						// 삭제 예정일
	DST_LOBBY_CHAR,							// 캐릭터
	DST_LOBBY_CHOICE_CHANNEL,				// 채널 선택
	DST_LOBBY_DEL_CHAR,						// 캐릭터 삭제
	DST_LOBBY_GAME_START,					// 게임시작
	DST_LOBBY_OK,							// 확 인
	DST_LOBBY_CANCLE,						// 취소
	DST_LOBBY_CANCLE2,						// 취 소	
	DST_LOBBY_CHOICE_SERVER,				// 서버 선택
	DST_LOBBY_SERVER,						// 서  버
	DST_LOBBY_STATE,						// 상태	
	DST_LOBBY_RECOVERY_CHAR_INFO,			// 캐릭터 복구
	DST_LOBBY_LEFT_DELETE_TIME,				// 삭제 대기 시간

	DST_LOBBY_DEFAULT_POSITION_HUMAN,		// 새싹 마을 부근
	DST_LOBBY_DEFAULT_POSITION_NAMEK,		// 달팽이 마을 부근
	DST_LOBBY_DEFAULT_POSITION_MAJIN,		// 베에빌리지 마을 부근

	// Guild
	DST_GUILD_LEAVE,						// 길드 탈퇴
	DST_GUILD_MEMBER,						// 길드맴버
	DST_GUILD_LEVEL,						// 레벨
	DST_GUILD_CUR_POS,						// 현재위치
	DST_GUILD_NOTICE,						// 길드 공지
	DST_GUILD_GYM,							// 길드 도장
	DST_GUILD_MASTER,						// 길드장
	DST_GUILD_SECOND_MASTER,				// 부길드장

	DST_GUILD_EMBLEM_TITLE,					// 유파 엠블렘 편집
	DST_GUILD_NOTICE_TITLE,					// 유파 공지 입력

	// Guild warehouse
	DST_GUILDWAREHOUSE_TITLE_1,				// 길드 창고 1
	DST_GUILDWAREHOUSE_TITLE_2,				// 길드 창고 2
	DST_GUILDWAREHOUSE_TITLE_3,				// 길드 창고 3

	// ChatDisplayMode
	DST_CHAT_MODE_GENERAL,					// 일반
	DST_CHAT_MODE_WHISPER,					// 귓속말
	DST_CHAT_MODE_PARTY,					// 파티
	DST_CHAT_MODE_TRADE,					// 거래
	DST_CHAT_MODE_GUILD,					// 길드
	DST_CHAT_MODE_SHOUT,					// 외침
	DST_CHAT_MODE_SYSTEM,					// 시스템	

	DST_CHAT_FILTER,						// 채팅 필터	
	DST_CHAT_DISP_COMMAND_FIRST,			// 맨 위로
	DST_CHAT_DISP_COMMAND_PREV,				// 한 줄 위로
	DST_CHAT_DISP_COMMAND_NEXT,				// 한 줄 아래로
	DST_CHAT_DISP_COMMAND_LAST,				// 맨 밑으로
	DST_CHAT_DISP_COMMAND_LOCK,				// 채팅창 크기 잠금
	DST_CHAT_DISP_COMMAND_UNLOCK,			// 채팅창 크기 잠금 해제

	// 시간
	DST_TERM,								// 기간
	DST_A_DAY,								// 날짜

	DST_MONDAY,								// 월
	DST_TUESDAY,							// 화
	DST_WEDNESDAY,							// 수
	DST_THURSDAY,							// 목
	DST_FRIDAY,								// 금
	DST_SATURDAY,							// 토
	DST_SUNDAY,								// 일

	DST_TIME_MONTH,							// 월
	DST_TIME_DAY,							// 일
	DST_TIME_HOUR,							// 시
	DST_TIME_AN_HOUR,						// 시간
	DST_TIME_MINUTE,						// 분
	DST_TIME_SECOND,						// 초

	// 타임머신 퀘스트	
	DST_TMQ_BALLOON_TITLE,					// 제   목
	DST_TMQ_BALLOON_CURRENT_PEOPLE,			// 현재 신청한 파티/개인 : 
	DST_TMQ_BALLOON_PRIVATE_REG,			// 개인 등록
	DST_TMQ_BALLOON_PARTY_REG,				// 파티 등록
	DST_TMQ_BALLOON_TIME_BY_LOTTERY,		// 추첨까지 남은 시간 : 
	DST_TMQ_BALLOON_CENCLE_RESERVATION,		// 예약취소
	DST_TMQ_BALLOON_START_TIME,				// 출발시간
	DST_TMQ_BALLOON_REMAIN_TIME_BY_RETRY,	// 재신청까지 남은 시간 : 
	DST_TMQ_BALLOON_RETRY,					// 재신청
	DST_TMQ_BALLOON_REG_STATUS,				// 등록상태 : 
	DST_TMQ_BALLOON_REMAIN_TIME,			// 남은시간 : 
	DST_TMQ_BALLOON_NEED_ITEM,				// 필요아이템 : 
	DST_TMQ_BALLOON_NOT_THING,				// 없음
	DST_TMQ_BALLOON_MOVE,					// 이동하기

	DST_TMQ_BOARD_CURRENT_TIME,				// 현재시간
	DST_TMQ_BOARD_LIMITE_TIME,				// 제한시간
	DST_TMQ_BOARD_LIMITE_LEVEL,				// 제한레벨
	DST_TMQ_BOARD_NEXT_TMQ,					// 다음 타임머신 퀘스트
	DST_TMQ_BOARD_RESERVATION_PRIVATE,		// 개인예약
	DST_TMQ_BOARD_RESERVATION_PARTY,		// 파티예약	
	DST_TMQ_BOARD_TEAM,						// 팀
	DST_TMQ_BOARD_PRIVATE,					// 개인
	DST_TMQ_BOARD_RESERVATING,				// 예약중
	DST_TMQ_DEGREE_OF_DIFFICULTY_1,			// 쉬움
	DST_TMQ_DEGREE_OF_DIFFICULTY_2,			// 보통
	DST_TMQ_DEGREE_OF_DIFFICULTY_3,			// 어려움

	DST_TMQ_STATUS_TITLE,					// 타임머신 퀘스트
	DST_TMQ_STATUS_INFO,					// 정보
	DST_TMQ_STATUS_POINT,					// 포인트
	DST_TMQ_STATUS_QUEST,					// 퀘스트
	DST_TMQ_STATUS_LASTEST_RECORD,			// 최단기록
	DST_TMQ_STATUS_REMAIN_TIME,				// 진행시간
	DST_TMQ_STATUS_SENARIO,					// 시나리오
	DST_TMQ_STATUS_STATUS_INFO,				// 진행정보

	DST_TMQ_INDICATE_DROPRATE,				// 드롭확률 : 

	// RankBattle
	DST_RB_TITLE,							// 참가 신청
	DST_RB_FIELD,							// 경기장
	DST_RB_CHALLENGER_COUNT,				// 참가 인원
	DST_RB_BTN_INDIVIDUAL,					// 개인전투
	DST_RB_BTN_PARTY,						// 파티전투
	DST_RB_BTN_APPLY,						// 참가신청
	DST_RB_BTN_CANCEL,						// 참가취소
	DST_RB_INFO_1,							// %s 후에 전투가 시작될 예정입니다. 
	DST_RB_INFO_2,							// 전투 타입 : %s\n경기장 : %s\n전투 예정 시간 : %s 후

	DST_RB_APPLY_CHALLANGE,					// 도전을 신청하시겠습니까?
	DST_RB_CANCEL,							// 취소
	DST_RB_OK,								// 확인

	DST_RB_STRAIGHT_WIN,					// 연승
	DST_RB_PERFECT_WIN,						// 완승
	DST_RB_LEVEL_LIMIT,						// %uLv.~%uLv.
	DST_RB_PAGE,							// %u / %u
	DST_RB_LEVEL,							// %d 레벨

	DST_RB_SIDE_WAIT_FOR_ENTER,				// 랭크배틀 참여대기중 (%u~%uLv)
	DST_RB_SIDE_CHECK_FOR_ENTER,			// 랭크배틀 참여확인 (%u~%uLv)
	DST_RB_SIDE_DO_ENTER_RANKBATTLE,		// 랭크배틀에 참여하시겠습니까?
	DST_RB_SIDE_RANKBATTLE_CANCELED,		// 랭크배틀이 취소되었습니다.
	DST_RB_SIDE_DOING_RANKBATTLE,			// 랭크배틀 중입니다.	
	DST_RB_SIDE_ON_ENTER,					// 입장합니다
	DST_RB_SIDE_ON_ENTER_SECOND,			// %d초 후 입장

	DST_RB_SIDE_ENTER_SUCESS,				// 참여가능
	DST_RB_SIDE_ENTER_FAIL,					// 참여불가
	DST_RB_SIDE_ENTER_TOO_LOW_LEVEL,		// (레벨부족)
	DST_RB_SIDE_ENTER_TOO_HIGHTLEVEL,		// (레벨초과)
	DST_RB_SIDE_ENTER_MUST_HAVE_NEED_ITEM,	// (아이템없음)
	DST_RB_SIDE_ENTER_MUST_HAVE_NEED_ZENNY, // (제니부족)
	DST_RB_SIDE_ENTER_STATE_ERROR,			// (상태이상)		// 괄호 꼭 부탁!!

	DST_RB_VS,								// vs 혹은 對

	DST_RB_RESULT_RECORD_INFO_1,			// 총 %d전 %d승 %d패
	DST_RB_RESULT_RECORD_INFO_2,			// %d 연승 ( 완승 : %d회 )
	DST_RB_RESULT_POINT_TEXT,				// 점수
	DST_RB_RESULT_POINT_RESULT,				// 결과
	DST_RB_RESULT_WAIT_TEXT,				// 잠시만 기다려 주세요.
	DST_RB_RESULT_WAIT_TIME,				// (제한시간:%u초)

	DST_RB_BLUE_TEAM,						// 블루팀
	DST_RB_RED_TEAM,						// 레드팀
	DST_RB_ENEMY_MEMBER,					// 상대팀 멤버

	DST_RB_RESULT_MESSAGE,					// 수고하셨습니다. 월드로 이동합니다. (HTML형식)

	DST_RANKBATTLE_SIDEVIEW_MESSAGE_JOIN,		// %s 에\n참여신청을 하였습니다.
	DST_RANKBATTLE_SIDEVIEW_MESSAGE_MATCHFAIL,	// 전투 상대를 찾을 수 없어\n전투가 연기되었습니다.

	// Buff
	DST_BUFF_REMAIN_TIME,					// 남은시간 %s
	DST_BUFF_DROP_TEXT,						// 클릭하면 버프를 무효화할 수 있습니다.

	// Exp
	DST_EXP_INFO,							// EXP : %d / %d

	// Chat
	DST_CHAT_IME_ENG,						// A
	DST_CHAT_IME_KOR,						// 가

	// MainMenu
	DST_MAINMENU_HELP,						// 도움말
	DST_MAINMENU_STATUS,					// 상태보기
	DST_MAINMENU_INVEN,						// 캡슐키트
	DST_MAINMENU_QUEST,						// 퀘스트
	DST_MAINMENU_OPTION,					// 옵션
	DST_MAINMENU_COMMUNITY,					// 커뮤니티
	DST_MAINMENU_RANKBOARD,					// 랭크보드
	DST_MAINMENU_SKILL,						// 스킬
	DST_MAINMENU_FRIENDLIST,				// 친구리스트
	DST_MAINMENU_END_SESSION,				// 접속종료
	DST_MAINMENU_CHANNEL_CHANGE,			// 채널변경
	DST_MAINMENU_END_GAME,					// 게임종료
	DST_MAINMENU_TOOLTIP,					// 메인메뉴

	// MultiDialog
	DST_MULTIDIALOG_MENU_SHOP,				// 상점열기
	DST_MULTIDIALOG_MENU_SKILL,				// 스킬배우기
	DST_MULTIDIALOG_MENU_BANK,				// 은행 열기
	DST_MULTIDIALOG_MENU_BUY_BANK,			// 창고 구입
	DST_MULTIDIALOG_MENU_OPEN_GUILD_BANK,	// 길드 창고 열기
	DST_MULTIDIALOG_MENU_QUEST,				// 퀘스트 수행하기
	DST_MULTIDIALOG_MENU_UPGRADE,			// 아이템 업그레이드
	DST_MULTIDIALOG_MENU_EXIT,				// 나가기
	DST_MULTIDIALOG_MENU_GUILD,				// 길드 관리
	DST_MULTIDIALOG_MENU_TMQ,				// 타임머신 퀘스트

	DST_MULTIDIALOG_GUILDMENU_MAKE,				// 길드 만들기
	DST_MULTIDIALOG_GUILDMENU_CANCEL_DISSOLVE,	// 길드 해체 취소
	DST_MULTIDIALOG_GUILDMENU_DISSOLVE,			// 길드 해체
	DST_MULTIDIALOG_GUILDMENU_POINT_USE,		// 길드 포인트 사용
	DST_MULTIDIALOG_GUILDMENU_CONTIBUTION,		// 길드 제니 기부
	DST_MULTIDIALOG_GUILDMENU_CONTIBUTION_TITLE,// %d 제니 기부

	DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_20,		// 최대 길드인원 20명
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_SECOND_MASTER_1,	// 부마스터 1명
	DST_MULTIDIALOG_GUILDFUNCTION_NOTICE,				// 길드 공지
	DST_MULTIDIALOG_GUILDFUNCTION_POST,					// 길드 편지
	DST_MULTIDIALOG_GUILDFUNCTION_WAREHOUSE,			// 길드 창고
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_30,		// 최대 길드인원 30명
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_SECOND_MASTER_2,	// 부마스터 2명
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_40,		// 최대 길드인원 40명
	DST_MULTIDIALOG_GUILDFUNCTION_CHANGE_NAME,			// 길드 이름 변경
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_50,		// 최대 길드인원 50명
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_SECOND_MASTER_3,	// 부마스터 3명
	DST_MULTIDIALOG_GUILDFUNCTION_SET_EMBLEM,			// 길드 문장 사용
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_60,		// 최대 길드인원 60명
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_70,		// 최대 길드인원 70명
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_SECOND_MASTER_4,	// 부마스터 4명
	DST_MULTIDIALOG_GUILDFUNCTION_HAVE_DOJO,			// 도장 소유	
	DST_MULTIDIALOG_GUILDFUNCTION_USE_DOJO_TELEPORT,	// 도장 텔레포트
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_80,		// 최대 길드인원 80명
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_1,		// 도장 업그레이드 레벨 1
	DST_MULTIDIALOG_GUILDFUNCTION_MAKE_DOGI,			// 도복 사용
	DST_MULTIDIALOG_GUILDFUNCTION_MAKE_DOJO_DOGI,		// 도장 도복 사용
	DST_MULTIDIALOG_GUILDFUNCTION_CHANGE_EMBLEM,		// 문장 변경
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_2,		// 도장 업그레이드 레벨 2
	DST_MULTIDIALOG_GUILDFUNCTION_TENKAICHI_SEED,		// 천하제일 무도회 시드 지정
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_90,		// 최대 길드인원 90명
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_3,		// 도장 업그레이드 레벨 3
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_QUEST_1,			// 도장 퀘스트 1
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_MIX_RATE_UP_1,	// 도장 믹스 확률 증가 1
	DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_100,		// 최대 길드인원 100명
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_4,		// 도장 업그레이드 레벨 4
	DST_MULTIDIALOG_GUILDFUNCTION_CAN_CHANGE_DOGI,		// 도복 변경
	DST_MULTIDIALOG_GUILDFUNCTION_CAN_CHANGE_DOJO_DOGI,	// 도장 도복 변경
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_5,		// 도장 업그레이드 레벨 5
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_QUEST_2,			// 도장 퀘스트 2
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_VEHICLE,			// 도장 탈 것 사용
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_6,		// 도장 업그레이드 레벨 6
	DST_MULTIDIALOG_GUILDFUNCTION_CAN_HAVE_BOOK,		// 도장 교본
	DST_MULTIDIALOG_GUILDFUNCTION_CAN_HAVE_SKILL,		// 도장 스킬
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_7,		// 도장 업그레이드 레벨 7
	DST_MULTIDIALOG_GUILDFUNCTION_DOJO_MIX_RATE_UP_2,	// 도장 믹스 확률 증가 2

	DST_MULTIDIALOG_DOJO_ESTABLISHMENT,					// 도장 설립	
	DST_MULTIDIALOG_DOJO_INFO,							// 도장 정보
	DST_MULTIDIALOG_DOJO_ACCOUNT,						// 도장 창고 사용 내역

	DST_MULTIDIALOG_BUYBANKMENU_BUYBANK,				// 창고 %d 구입 %dZ
	DST_MULTIDIALOG_BUYBANKMENU_BUYPUBLIC_BANK,			// 공유창고 구입 %dZ

	// Multidialog HoipoiMix Npc
	DST_MULTIDIALOG_HOIPOIMIX_SET_COMMAND,				// 호이포이믹스 스킬 배우기
	DST_MULTIDIALOG_HOIPOIMIX_RESET_COMMAND,			// 호이포이믹스 스킬 초기화하기

	DST_MULTIDIALOG_HOIPOIMIX_SET_FOOD_DRINK,			// 호이포이믹스 기본 스킬 배우기
	DST_MULTIDIALOG_HOIPOIMIX_SET_MAIN_WEAPON,			// 호이포이믹스 메인웨폰 만들기 스킬
	DST_MULTIDIALOG_HOIPOIMIX_SET_ARMOR,				// 호이포이믹스 방어구 만들기 스킬
	DST_MULTIDIALOG_HOIPOIMIX_SET_ACCESSORY,			// 호이포이믹스 악세서리 만들기 스킬
	DST_MULTIDIALOG_HOIPOIMIX_SET_SCOUTER,				// 호이포이믹스 스카우터 만들기 스킬
	DST_MULTIDIALOG_HOIPOIMIX_SET_SUB_WEAPON,			// 호이포이믹스 서브웨폰 만들기 스킬

	DST_MULTIDIALOG_HOIPOIMIX_RESET_NORMAL,				// 호이포이믹스 일반 기술 초기화
	DST_MULTIDIALOG_HOIPOIMIX_RESET_SPECIAL,			// 호이포이믹스 특수 기술 초기화

	// MsgBox
	DST_MSG_BOX_TITLE,						// 메세지
	DST_MSG_BTN_OK,							// 확인
	DST_MSG_BTN_CANCEL,						// 취소
	DST_MSG_REMAIN_TIME,					// [br][br] 남은시간 : %s 

	// Bag
	DST_BAG_TITLE,							// 인벤토리
	DST_ZENNY_BTN,							// 소지금
	DST_DISCARD_BTN,						// 휴지통
	DST_BAG_SLOT,							// 캡슐 슬롯,
	DST_BAG_REMOTE_SELL,					// 원격판매,

	// DropItem
	DST_DROPITEM_ZENNY,						// %d 제니

	// PopupMenu Text -> 필요없는건 체크해서 알려주세요
	DST_POPUPMENU_USE,							    // 사용하기
	DST_POPUPMENU_OPEN,								// 열기
	DST_POPUPMENU_EQUIP,							// 장착하기
	DST_POPUPMENU_CANCEL_EQUIP,						// 장착해제
	DST_POPUPMENU_VIEW,								// 보기
	DST_POPUPMENU_PICKUP,							// 집기
	DST_POPUPMENU_STOP_USE,							// 사용중지
	DST_POPUPMENU_PULLOUT,							// 등록취소
	DST_POPUPMENU_SELL,								// 팔기
	DST_POPUPMENU_DIVIDE,							// 나누기
	DST_POPUPMENU_CLEAR,							// 비우기
	DST_POPUPMENU_DELETE,							// 버리기
	DST_POPUPMENU_PARTY_CREATE,						// 파티 만들기
	DST_POPUPMENU_PARTY_INVITE,						// 파티 초대
	DST_POPUPMENU_PARTY_LEADER_CHANGE,				// 파티장 위임
	DST_POPUPMENU_PARTY_KICK_OUT,					// 파티 강퇴
	DST_POPUPMENU_PARTY_LEAVE,						// 파티 탈퇴
	DST_POPUPMENU_PVP_REQUEST_FIGHT,				// 대련 요청
	DST_POPUPMENU_USER_TRADE,						// 유저 거래
	DST_POPUPMENU_GUILD_INVITE,						// 길드 초대
	DST_POPUPMENU_GUILD_CHANGE_MASTER,				// 길드장 이양
	DST_POPUPMENU_GUILD_APPOINT_SECOND_MASTER,		// 부길드장 임명
	DST_POPUPMENU_GUILD_DISAPPOINT_SECOND_MASTER,	// 부길드장 해제
	DST_POPUPMENU_GUILD_KICK_OUT,					// 길드 추방
	DST_POPUPMENU_GUILD_POST,						// 편지 보내기
	DST_POPUPMENU_FRIEND_ADD,						// 친구 추가
	DST_POPUPMENU_FOLLOW,							// 따라 가기
	DST_POPUPMENU_CANCEL,							// 취소
	DST_POPUPMENU_SET_SHARETARGET_1,				// 1번 공유타겟 설정
	DST_POPUPMENU_SET_SHARETARGET_2,				// 2번 공유타겟 설정
	DST_POPUPMENU_SET_SHARETARGET_3,				// 3번 공유타겟 설정
	DST_POPUPMENU_SET_SHARETARGET_4,				// 4번 공유타겟 설정
	DST_POPUPMENU_SET_SHARETARGET_5,				// 5번 공유타겟 설정
	DST_POPUPMENU_UNSET_SHARETARGET_1,				// 1번 공유타겟 해제
	DST_POPUPMENU_UNSET_SHARETARGET_2,				// 2번 공유타겟 해제
	DST_POPUPMENU_UNSET_SHARETARGET_3,				// 3번 공유타겟 해제
	DST_POPUPMENU_UNSET_SHARETARGET_4,				// 4번 공유타겟 해제
	DST_POPUPMENU_UNSET_SHARETARGET_5,				// 5번 공유타겟 해제
	DST_POPUPMENU_PARTY_MENU_SWITCH_MEMBER,			// 파티 맴버창 스위치
	DST_POPUPMENU_PARTY_MENU_SWITCH_MEMBER_BUFF,	// 파티 맴버창 버프 스위치
	DST_POPUPMENU_PARTY_DUNGEON_INIT,				// 던전 초기화
	DST_POPUPMENU_PARTY_DUNGEON_NORMAL,				// 던전 난이도 보통
	DST_POPUPMENU_PARTY_DUNGEON_HARD,				// 던전 난이도 어려움
	DST_POPUPMENU_PARTY_DIVIDE_PICKUP_PERSON,		///< 줍는 사람 획득
	DST_POPUPMENU_PARTY_DIVIDE_EQUAL,				///< 똑같이 나누기
	DST_POPUPMENU_PARTY_DIVIDE_ORDER,				///< 순서되로 분배
	DST_POPUPMENU_PARTY_GRADE_ITEM,					///< 등급별 순서되로 분배
	DST_POPUPMENU_PARTY_USE_INVEN,					///< 파티 인벤토리 사용

	// NPC job name
	DST_JOB_WEAPON_MERCHANT,						// 무기상인
	DST_JOB_ARMOR_MERCHANT,							// 의복상인
	DST_JOB_GOODS_MERCHANT,							// 잡화상인
	DST_JOB_SCOUTER_MERCHANT,						// 스카우터상인
	DST_JOB_GUARD,									// 경비
	DST_JOB_SKILL_TRAINER_HFI,						// 무도가 교관
	DST_JOB_SKILL_TRAINER_HMY,						// 기공사 교관
	DST_JOB_SKILL_TRAINER_HEN,						// 엔지니어 교관
	DST_JOB_SKILL_TRAINER_NFI,						// 전사 교관
	DST_JOB_SKILL_TRAINER_NMY,						// 용족 교관
	DST_JOB_SKILL_TRAINER_MMI,						// 대마인 교관
	DST_JOB_SKILL_TRAINER_MWO,						// 의마인 교관
	DST_JOB_BANKER,									// 창고지기 
	DST_JOB_TALKER,									// 이야기꾼
	DST_JOB_GUILD_MANAGER,							// 길드매니저
	DST_JOB_SUMMON_PET,								// 소환수
	DST_JOB_DOGI_MERCHANT,							// 도복 상인
	DST_JOB_SPECIAL_WEAPON_MERCHANT,				// 고급 무기 상인
	DST_JOB_SPECIAL_ARMOR_MERCHANT,					// 고급 의복 상인
	DST_JOB_SPECIAL_GOODS_MERCHANT,					// 고급 잡화 상인
	DST_JOB_SPECIAL_FOODS_MERCHANT,					// 고급 식료품 상인
	DST_JOB_SPECIAL_SCOUTER_MERCHANT,				// 고급 스카우터 상인
	DST_JOB_GRAND_SKILL_TRAINER_HFI,				// 무도가 그랜드 스킬마스터
	DST_JOB_GRAND_SKILL_TRAINER_HMY,				// 기공사 그랜드 스킬마스터
	DST_JOB_GRAND_SKILL_TRAINER_HEN,				// 엔지니어 그랜드 스킬마스터
	DST_JOB_GRAND_SKILL_TRAINER_NFI,				// 전사 그랜드 스킬마스터
	DST_JOB_GRAND_SKILL_TRAINER_NMY,				// 용족 그랜드 스킬마스터
	DST_JOB_GRAND_SKILL_TRAINER_MMI,				// 대마인 그랜드 스킬마스터
	DST_JOB_GRAND_SKILL_TRAINER_MWO,				// 의마인 그랜드 스킬마스터
	DST_JOB_SUB_WEAPON_MERCHANT,					// 보조 무기 상인
	DST_JOB_GATE_KEEPER,							// 문지기
	DST_JOB_VENDING_MACHINE,						// 자판기
	DST_JOB_TIMEMACHINE_MERCHANT,					// 타임머신진입 NPC
	DST_JOB_POTALMAN,								// 포탈맨
	DST_JOB_BUS,									// 버스
	DST_JOB_RECEPTION,								// 천하제일 무도회 접수인
	DST_JOB_BUDOHSI_MERCHANT,						// 무도사 상인
	DST_JOB_REFEREE,								// 심판
	DST_JOB_GAMBLE_MERCHANT,						// 뽑기 상인
	DST_JOB_CHAMPION_MERCHANT,						// 참피온 상인
	DST_JOB_DOJO_MANAGER,							// 도장 관리인
	DST_JOB_DOJO_MERCHANT,							// 도장 상인
	DST_JOB_DOJO_SEAL,								// 도장 인장
	DST_JOB_DOJO_BANKER,							// 도장 창고
	DST_JOB_MIX_MASTER,								// 믹스 마스터

	// Mob_Type
	DST_MOB_TYPE_ANIMAL,							// 동물
	DST_MOB_TYPE_HUMAN_ANIMAL,						// ...
	DST_MOB_TYPE_DINOSAUR,
	DST_MOB_TYPE_ALIEN,
	DST_MOB_TYPE_ANDROID,
	DST_MOB_TYPE_ROBOT,
	DST_MOB_TYPE_DRAGON,
	DST_MOB_TYPE_DEVIL,
	DST_MOB_TYPE_UNDEAD,
	DST_MOB_TYPE_PLANT,
	DST_MOB_TYPE_INSECT,
	DST_MOB_TYPE_HUMAN,
	DST_MOB_TYPE_NAMEC,
	DST_MOB_TYPE_MAJIN,
	DST_MOB_TYPE_BUILDING,

	// Portal 
	DST_MULTIDIALOG_MENU_PORTAL,                    // 포탈 타기
	DST_PORTAL_START_POINT,                         // 현재 위치
	DST_PORTAL_ZENNY,                               // 필요 제니
	DST_PORTAL_DISABLE,                             // 현재 위치에서 갈수 없습니다.
	DST_PORTAL_ADD,                                 // 새로운 이동 경로를 등록하였습니다.
	DST_PORTAL_ICON_START,                          // 현재 포인트
	DST_PORTAL_ICON_ENABLE,                         // 연결된 포인트
	DST_PORTAL_ICON_DISABLE,                        // 등록된 포인트
	DST_PORTAL_ICON_NOT_REGIST,                     // 미등록 포인트
	DST_PORTAL_CONFIRM,                             // '%s'으로 이동합니다. \n %d 제니 소비

	// Item Endurance warning
	DST_ITEM_DOFIX_SIDE_TEXT,						// 현재 내구도가 0이되어 사용할 수 없는 아이템이 있습니다. 가까운...
	DST_ITEM_DOEXTEND_SIDE_TEXT,					// 현재 사용기간이 만료된 아이템이 있습니다. 사용기간을 연장하여 주십시오.

	// Rank Board
	DST_RANKBOARD_BOARD_TITLE_PVP,					// PvP
	DST_RANKBOARD_BOARD_TITLE_TMQ,					// TMQ

	DST_RANKBOARD_BOARD_PREV,						// 이전 페이지
	DST_RANKBOARD_BOARD_NEXT,						// 다음 페이지

	DST_RANKBOARD_BOARD_DATALOADING,				// 데이터 처리중입니다.

	DST_RANKBOARD_BOARD_MSGBOX_TOPPAGE,				// 처음 페이지입니다.
	DST_RANKBOARD_BOARD_MSGBOX_NOTPAGE,				// 페이지가 없습니다.
	DST_RANKBOARD_BOARD_MSGBOX_NODATA,				// 월드의 랭크배틀 기록이 없습니다.
	DST_RANKBOARD_BOARD_MSGBOX_INPUTSEARCH,			// 검색할 순위나 이름을 입력해주세요.
	DST_RANKBOARD_BOARD_MSGBOX_NONESCENARIO,		// 없는 시나리오입니다.
	DST_RANKBOARD_BOARD_MSGBOX_NOTMEMBER,			// 파티원 정보가 없습니다.

	DST_RANKBOARD_RANKBATTLE_SUBJECT_RANK,			// 순위
	DST_RANKBOARD_RANKBATTLE_SUBJECT_NAME,			// 이름
	DST_RANKBOARD_RANKBATTLE_SUBJECT_LEVEL,			// 레벨
	DST_RANKBOARD_RANKBATTLE_SUBJECT_CLASS,			// 직업
	DST_RANKBOARD_RANKBATTLE_SUBJECT_GUILD,			// 길드
	DST_RANKBOARD_RANKBATTLE_SUBJECT_POINT,			// 점수
	DST_RANKBOARD_RANKBATTLE_SUBJECT_STRAIGHTWIN,	// 연승

	DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE,		// 변동사항

	DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_1,		// 1일전
	DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_2,		// 2일전
	DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_3,		// 3일전
	DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_4,		// 4일전
	DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_5,		// 5일전
	DST_RANKBOARD_RANKBATTLE_SUBJECT_COMPARE_6,		// 6일전

	DST_RANKBOARD_TMQ_SUBJECT_RANK,					// 순위
	DST_RANKBOARD_TMQ_SUBJECT_PARTYNAME,			// 파티 이름
	DST_RANKBOARD_TMQ_SUBJECT_CLEARTIME,			// 클리어 시간
	DST_RANKBOARD_TMQ_SUBJECT_PARTYMEMBERINFO,		// 파티원 정보
	DST_RANKBOARD_TMQ_SUBJECT_PARTYMEMBER,			// 파티 멤버
	DST_RANKBOARD_TMQ_SUBJECT_LEVEL,				// Lv.
	DST_RANKBOARD_TMQ_SUBJECT_CLASS,				// 클래스
	DST_RANKBOARD_TMQ_SUBJECT_SCENARIO,				// 시나리오 %d

	DST_RANKBOARD_TMQ_OVER_TIME,					// 하루종일

	// Friend List
	DST_FRIEND_TAB_FRIEND,                          // 친구 리스트
	DST_FRIEND_TAB_BLACKLIST,                       // 블랙 리스트
	DST_FRIEND_SUBJECT_NAME,                        // 이름
	DST_FRIEND_SUBJECT_LEVEL,                       // 레벨
	DST_FRIEND_SUBJECT_LOC,                         // 위치
	DST_FRIEND_SUBJECT_CHANNEL,                     // 채널
	DST_FRIEND_OFFLINE,                             // 오프라인
	DST_FRIEND_ADD,                                 // 추가
	DST_FRIEND_INVITE,                              // 파티 초대
	DST_FRIEND_WHISPER,                             // 귓속 말 보내기
	DST_FRIEND_MAIL,                                // 메일 보내기
	DST_FRIEND_BLOCK,                               // 차단
	DST_FRIEND_DELETE,                              // 삭제
	DST_FRIEND_MSG_FRIEND_ADD,                      // 친구로 추가할 대상의 이름을 입력하고 확인을 눌러주세요
	DST_FRIEND_MSG_BLACK_ADD,                       // 블랙 리스트에 추가할 상대방의 이름을 입력하고 확인을 눌러주세요
	DST_FRIEND_MSG_FRIEND_DEL,                      // 님을 정말로 친구 리스트에서 제거하시겠습니까?
	DST_FRIEND_MSG_BLACk_DEL,                       // 님을 정말로 블랙 리스트에서 제거하시겠습니까?
	DST_FRIEND_MSG_LOGIN,                           // 님이 게임에 접속하였습니다
	DST_FRIEND_ERR_EXIST_BLACK,                     // 블랙 리스트에 있는 유저를 친구로 추가할 수는 없습니다
	DST_FRIEND_ERR_EXIST_FRIEND,                    // 친구 리스트의 차단버튼을 사용해 차단해 주세요
	DST_FRIEND_INVALID_USER,                        // 선택된 대상이 없습니다.
	DST_FRIEND_TOO_OVER_NAME,                       // 입력한 캐릭터 이름이 너무 깁니다.

	// Communication Target Window
	DST_COMMU_TARGET_FRIEND,                        // 친구
	DST_COMMU_TARGET_GUILD,                         // 길드
	DST_COMMU_TARGET_PARTY,                         // 파티

	// Minimap
	DST_MINIMAP_CHECK_POINT,                        // 체크 포인트
	DST_MINIMAP_SHOW_MINIMAP,                       // 미니맵 보기
	DST_MINIMAP_MINIMIZE,	                        // 미니맵 최소화
	DST_MINIMAP_SHOW_WORLDMAP,                      // 월드맵
	DST_MINIMAP_ZOOM_IN,	                        // 미니맵 확대
	DST_MINIMAP_ZOOM_OUT,	                        // 미니맵 축소
	DST_MINIMAP_POPOSTONE,	                        // 포포스톤
	DST_MINIMAP_HIDDEN,		                        // 숨겨진 물체
	DST_MINIMAP_UNKNOWN,		                    // 알 수 없는 곳

	// WorldMap
	DST_WORLDMAP_TITLE,		                        // 월드맵
	DST_WORLDMAP_TRANSPARENCY,                      // 투명도
	DST_WORLDMAP_BUS_ROUTE_PAINT,                   // 버스 노선도
	DST_WORLDMAP_SHOW_OUR_GUILD_MEMBER,				// 우리 유파원
	DST_WORLDMAP_SHOW_OUR_GUILD,					// 우리 유파원 표시
	DST_WORLDMAP_SHOW_OUR_GUILD_IN_MINIMAP,			// 우리 유파원 미니맵에 표시
	DST_WORLDMAP_SHOW_OTHER_GUILD_MEMBER,			// 상대 유파원
	DST_WORLDMAP_SHOW_OTHER_GUILD,					// 상대 유파원 표시
	DST_WORLDMAP_SHOW_OTHER_GUILD_IN_MINIMAP,		// 상대 유파원 미니맵에 표시

	// Chat Shortcut
	DST_CHAT_SHORT_CUT_GENERAL,						// '/n '
	DST_CHAT_SHORT_CUT_TRADE,						// '/t '
	DST_CHAT_SHORT_CUT_WHISPER,						// '/w '
	DST_CHAT_SHORT_CUT_PARTY,						// '/p '
	DST_CHAT_SHORT_CUT_PARTY_NOTIFY,				// '/c '
	DST_CHAT_SHORT_CUT_GUILD,						// '/g '
	DST_CHAT_SHORT_CUT_SHOUT,						// '/s '
	DST_CHAT_SHORT_CUT_LAST_WHISPER_USER,			// '/r '
	DST_CHAT_SHORT_CUT_HELP,						// '/? '
	DST_CHAT_SHORT_CUT_GENERAL_LOCAL,				// '/ㅜ '
	DST_CHAT_SHORT_CUT_TRADE_LOCAL,					// '/ㅅ '
	DST_CHAT_SHORT_CUT_WHISPER_LOCAL,				// '/ㅈ '	Local이 붙은 녀석은 국가 버전에 맞게 수정해야 합니다.
	DST_CHAT_SHORT_CUT_PARTY_LOCAL,					// '/ㅔ '
	DST_CHAT_SHORT_CUT_PARTY_LOCAL_NOTIFY,			// '/ㅊ '
	DST_CHAT_SHORT_CUT_GUILD_LOCAL,					// '/ㅎ '
	DST_CHAT_SHORT_CUT_SHOUT_LOCAL,					// '/ㄴ '
	DST_CHAT_SHORT_CUT_LAST_WHISPER_USER_LOCAL,		// '/ㄱ '
	DST_CHAT_SHORT_CUT_INVITE_PARTY,				// '/초대 '
	DST_CHAT_SHORT_CUT_COMMAND,						// 채팅 명령어
	DST_CHAT_HAVE_NO_USER_TO_REPLY,					// 대답할 대상이 없습니다.
	DST_CHAT_FAIL_TO_USE_WHISPER,					// 귓속말을 하기위해서는 /w(/W,/ㅈ) 캐릭터이름 내용 의 형식대로 입력하여야 합니다.
	DST_CHAT_FAIL_TO_WHISPER_NO_TARGET,				// %s님은 로그인상태가 아닙니다.
	DST_CHAT_HELP_TEXT,								// 채팅 단축키 헬프텍스트. 기획서 참조
	DST_CHAT_OPTION_GUI_TITLE,						// 채팅 입력모드

	// Tutorial Guide Window
	DST_TUTORIAL_GUIDE_TITLE,						// 튜토리얼 가이드 윈도우

	// Mob Grade
	DST_MOB_GRADE_SUPER,							// 수퍼
	DST_MOB_GRADE_ULTRA,							// 울트라
	DST_MOB_GRADE_BOSS,								// 보스
	DST_MOB_GRADE_HERO,								// 히어로

	// TargetInfo
	DST_TARGET_MENU,								// 타겟 메뉴	(버튼 툴팁)
	DST_TARGET_SHARED_TARGET,						// 공유 타겟 메뉴 (버튼 툴팁)

	// GuardGauge 
	DST_GUARD_GAUGE_CAN_USE,						// 가드 스킬을 사용합니다.
	DST_GUARD_GAUGE_CANNOT_USE,						// 대기시간이 끝날 때까지\\n가드 스킬을 사용할 수 없습니다

	// Item Type
	DST_GLOVE,
	DST_STAFF,
	DST_GUN,
	DST_DUALDST_GUN,
	DST_CLAW,
	DST_AXE,
	DST_SCROLL,
	DST_GEM,
	DST_STICK,
	DST_SWORD,
	DST_FAN,
	DST_WAND,
	DST_BAZOOKA,
	DST_BACKDST_PACK,
	DST_INSTRUMENT,
	DST_CLUB,
	DST_DRUM,
	DST_MASK,

	// Side dialog title
	DST_SIDEDLG_TITLE_TMQINFO,						// TMQ 정보
	DST_SIDEDLG_TITLE_TARGETITEMINFO,				// 상대 아이템 정보
	DST_SIDEDLG_TITLE_RANKBATTLEINFO,				// 랭크배틀 정보
	DST_SIDEDLG_TITLE_QUESTINDICATOR,				// 퀘스트 알리미

	// Scouter PC VIewer
	DST_SCOUTER_PC_VIEWER_WEAPON,					// 무기
	DST_SCOUTER_PC_VIEWER_DEFENCE,					// 방어구
	DST_SCOUTER_PC_VIEWER_ACCESARY,					// 장신구
	DST_SCOUTER_PC_VIEWER_SPECIAL,					// 특수

	// Battle attribute info window text
	DST_BATTLEATTR_ATTR,							// 속성 :
	DST_BATTLEATTR_WEAPON,							// 무기 속성 : 
	DST_BATTLEATTR_ARMOR,							// 방어구 속성 :
	DST_BATTLEATTR_ON_TARGET,						// 현재 대상에게
	DST_BATTLEATTR_ATTACK_RATE_TARGET,				// 속성에게 공격력 %.0f%%
	DST_BATTLEATTR_DEFENCE_RATE_TARGET,				// 속성에게 방어력 %.0f%%
	DST_BATTLEATTR_ATTACK_RATE,						// 공격력 %.0f%%
	DST_BATTLEATTR_DEFENCE_RATE,					// 방어력 %.0f%%

	// 튜토리얼
	DST_TUTORIAL_MESSAGE_START,				// 드래곤볼 온라인의 세계에 오신 것을 환영합니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_002000,			// 이제부터 튜토리얼을 통해 기본 조작법을 배워 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_002100,			// 마우스 오른쪽 버튼을 누른채 마우스를 움직여 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_002200,			// 이렇게 카메라를 움직이면 주변의 적이나 지형, 지물의 위치를 잘 파악할 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_002300,			// 체크포인트 주변에 커서를 이동시키고 마우스 왼쪽 버튼을 눌러 자신의 캐릭터를 움직여 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_002400,			// 캐릭터는 마우스만이 아니라 키보드를 사용해서 움직일 수도 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_002500,			// W, A, S, D 키를 눌러 체크포인트까지 이동해 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_002600,			// 이 외에도 캐릭터를 움직이는 다양한 방법이 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_002700,			// 천리길도 한 걸음부터라는 말이 있습니다. 차차 배워나가도록 합시다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_002800,			// 캐릭터 정보창에서는 생명력(LP), 기력(EP), Level 등 캐릭터에 관한 여러가지 정보를 알 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_002900,			// 미니맵에서는 NPC나 몬스터, 지형 등 자신의 캐릭터 주변의 정보를 간단히 파악할 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_003000,			// 채팅창에서는 다른 유저들, 친구, 파티원들과 대화를 할 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_003100,			// 단축창에서는 스킬이나 아이템을 등록해 빠르고 간편하게 사용할 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_003200,			// 메인메뉴를 마우스 왼쪽 버튼으로 눌러 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_003300,			// 스킬 버튼을 마우스 왼쪽 버튼으로 선택해 스킬창을 열어 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_003400,			// 스킬아이콘을 마우스 왼쪽 버튼으로 눌러 주십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_003500,			// 단축창의 빈 슬롯을 마우스 왼쪽 버튼으로 눌러서 스킬아이콘을 사용해 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_003600,			// 이렇게 단축창에 스킬을 등록해 놓으면 빠르고 간편하게 사용할 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_003700,			// 스킬창의 닫기 버튼을 마우스 왼쪽 버튼으로 눌러서 스킬창을 닫아 주십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_003800,			// 이제, 메인메뉴의 모든 것을 사용할 수 있게 되었습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_003900,			// 이제 단축키로 창을 열 수 있게 되었습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_004000,			// 몬스터를 마우스 왼쪽 버튼을 눌러서 타겟으로 지정해 보십시요.(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_004100,			// 몬스터를 마우스 오른쪽 버튼을 눌러 공격해 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_004200,			// 몬스터를 공격할 때 단축창을 마우스 오른쪽 버튼을 누르십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_004300,			// 이렇게 스킬을 사용하면 훨씬 강력한 공격을 할 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_004400,			// 스킬은 단축창의 키보드 단축키 '1'부터 '='를 눌러서 사용할 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_004500,			// 주변에 떨어진 캡슐키트를 마우스 왼쪽 버튼을 눌러서 주워 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_004600,			// 기본 캡슐키트를 마우스 오른쪽 버튼으로 눌러 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_004700,			// 캡슐키트의 스카우터를 마우스 오른쪽 버튼을 눌러 장착해 보십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_004800,			// 이제 스카우터를 장착했으니 스카우터를 사용할 수 있게 되었습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_004900,			// 키보드 단축키 'T'를 눌러 스카우터를 켜주세요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_005000,			// 몬스터를 타겟으로 설정해 주십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_005100,			// 몬스터를 타겟으로 설정했으면 스카우터 메뉴의 'PowerLevel'을 선택해 주십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_005200,			// 이름을 알 수 없던 몬스터의 이름이 바로 "콩재배맨'이였군요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_005300,			// 이렇게 스카우터를 적이나 동료를 측정하면 알 수 없던 정보들을 알아낼 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_005400,			// 스카우터의 닫기 버튼을 눌러서 스카우터를 닫아 주십시요(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_005500,			// 스카우터나 다른 창을 닫을 때는 'ESC'키를 눌러 간단히 닫을 수도 있습니다)
	DST_TUTORIAL_MESSAGE_005600,			// 메인메뉴의 도움말 메뉴를 선택하면 도움말창을 볼 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_005700,			// 도움말창에서는 튜토리얼에서 배우지 못했던 많은 것을 배울 수 있습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_005800,			// Dbo의 세계를 타험하면서 모르는 것이 있는 때는 배우지 못했던 도움말을 참고하는 것은 좋은 방법입니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_005900,			// 이제 드래곤볼 온라인 세계로 여행을 떠나봅시다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_006000,			// 이제 휴지통에 아이템을 버릴 수 있게 되었습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_006100,			// 이제 장착한 아이템을 마우스 오른쪽 버튼으로 벗을 수 있게 되었습니다(message, 튜토리얼 진행 메세지)
	DST_TUTORIAL_MESSAGE_006200,		    //	Scouter나  Window을 닫을 때는 'ESC' 키를 눌러 간단히 닫을 수도 있습니다.
	DST_TUTORIAL_MESSAGE_006300,			//	휴지통을 사용할 수 있게 되었습니다.
	DST_TUTORIAL_MESSAGE_006400,			//	장착한 아이템을 Mouse Right Button으로 벗을 수 있게 되었습니다.
	DST_TUTORIAL_MESSAGE_006500,			//	Mainmenu의 도움말 메뉴를 선택하거나 단축키 F1을 눌러 도움말 Window을 열수 있습니다.
	DST_TUTORIAL_MESSAGE_006600,			//	도움말 Window에서는 Tutorial에서 배우지 못했던 많을 것을 배울 수 있습니다
	DST_TUTORIAL_MESSAGE_006700,			//	DBO의 세계를 탐험하면서 모르는 것이 있을 때는 도움말을 참고하는 것이 좋은 방법이죠.
	DST_TUTORIAL_MESSAGE_006800,			//	자, DBO의 세계로 여행을 떠나봅시다. 파이팅!
	DST_TUTORIAL_MESSAGE_006900,			//	Minimap 최소화 하기

	// Caution SideIcon
	DST_CAUTION_SIDEICON_TITLE,				// 경고 메시지

	// 주사위
	DST_DICE_EXPLAIN,						// 주사위(1~99)를 굴립니다
	DST_DICE_EXPLAIN2,						// 주사위를 클릭해 주세요
	DST_DICE_BUDOKAI_MINOR_EXPLAIN,			// 주사위는 자동으로 굴려집니다.

	// 천하제일 무도회
	DST_BUDOKAI_NEWS_TITLE,					// 제 %d 회 천하제일무도회

	DST_BUDOKAI_NEWS_HEADLINE_OPENNOTICE,	// 천하제일 무도회 개막!
	DST_BUDOKAI_NEWS_HEADLINE_DOJORECOMMEND,// 천하제일 무도회 도장 추천자 신청 기간
	DST_BUDOKAI_NEWS_HEADLINE_REGISTER,		// 천하제일 무도회 예선 신청 기간
	DST_BUDOKAI_NEWS_HEADLINE_MAJOR,		// 천하제일 무도회 본선 기간
	DST_BUDOKAI_NEWS_HEADLINE_AWARD,		// 천하제일 무도회 폐막!

	DST_BUDOKAI_NEWS_NOTICE_GUIDE_MINOR_TITLE,	// 예선 신청 기간
	DST_BUDOKAI_NEWS_NOTICE_GUIDE_DOJO_TITLE,	// 도장 추천자 추천 기간
	DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME,		// %d월 %d일 %s %d:%d
	DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_FULL,// %s ~ %s 
	DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_AM,	// 오전
	DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_PM,	// 오후

	DST_BUDOKAI_NEWS_NOTICE_INDI_STATE,		// 개인전 진행 안내
	DST_BUDOKAI_NEWS_NOTICE_TEAM_STATE,		// 팀전 진행 안내

	DST_BUDOKAI_NEWS_NOTICE_INDI_REGISTER,	// 개인전 예선 신청 기간 안내
	DST_BUDOKAI_NEWS_NOTICE_TEAM_REGISTER,	// 팀전 예선 신청 기간 안내

	DST_BUDOKAI_NEWS_NOTICE_WAIT_GUIDE,		// %s 기간 안내 : %s
	
	DST_BUDOKAI_NEWS_NOTICE_INDI_WINNER,	// 개인전 우승/준 우승
	DST_BUDOKAI_NEWS_NOTICE_TEAM_WINNER,	// 팀전 우승/준 우승

	DST_BUDOKAI_NEWS_NOTICE_PRIZE_WINNER,	// 우승자 : %s
	DST_BUDOKAI_NEWS_NOTICE_PRIZE_SECOND_WINNER,	// 준 우승자 : %s

	DST_BUDOKAI_NEWS_NOTICE_PRIZE_TEAM_WINNER,	// 우승팀 : %s
	DST_BUDOKAI_NEWS_NOTICE_PRIZE_TEAM_SECOND_WINNER,	// 준 우승팀 : %s

	DST_BUDOKAI_NEWS_NOTICE_NOT_PRIZE,		// 수상자가 없습니다.
	DST_BUDOKAI_NEWS_NTOCIE_NOT_PRIZE_TEAM,	// 수상팀이 없습니다.
	
	DST_BUDOKAI_REQUEST_MINOR,				// 예선 
	DST_BUDOKAI_REQUEST_MAJOR,				// 본선
	DST_BUDOKAI_REQUEST_FINAL,				// 결선

	DST_BUDOKAI_TYPE_ADULT_TITLE,			// 성인부
	DST_BUDOKAI_TYPE_CHILD_TITLE,			// 소년부

	DST_BUDOKAI_NEWS_BUTTON_INDIREQ,		// 개인 신청
	DST_BUDOKAI_NEWS_BUTTON_TEAMREQ,		// 파티 신청
	DST_BUDOKAI_NEWS_BUTTON_TICKET,			// 접수증
	DST_BUDOKAI_NEWS_BUTTON_TOURNAMENT,		// 대진표

	DST_BUDOKAI_NEWS_CAN_ONLY_LEADER,		// 파티의 리더만이 파티전 신청을 할 수 있습니다.
	DST_BUDOKAI_NEWS_YOU_ARE_NOT_ENTRANT,	// 당신은 천하제일 무도회 참가자가 아닙니다.
	
	DST_BUDOKAI_NEWS_INFO_INDI_STATE,		// 당신은 개인전 %s 입니다.
	DST_BUDOKAI_NEWS_INFO_TEAM_STATE,		// 당신은 파티전 %s 입니다.

	DST_BUDOKAI_NEWS_INFO_STATE_REGISTER,	// %s 예선 접수가 %s 후에 마감됩니다.
	DST_BUDOKAI_NEWS_INFO_STATE_WAIT_MINOR_MATCH,	// %s 예선전이 %s 후에 시작됩니다.
	DST_BUDOKAI_NEWS_INFO_STATE_MINOR_MATCH,		// %s 예선전이 진행 중 입니다.
	DST_BUDOKAI_NEWS_INFO_STATE_WAIT_MAJOR_MATCH,	// %s %s이 %s 후에 시작됩니다.
	DST_BUDOKAI_NEWS_INFO_STATE_MAJOR_MATCH,		// %s %s이 진행 중입니다.
	DST_BUDOKAI_NEWS_INFO_STATE_WAIT_SEMIFINAL_MATCH,	// %s 준결승이 %s 후에 시작됩니다.
	DST_BUDOKAI_NEWS_INFO_STATE_SEMIFINAL_MATCH,	// %s 준결승이 진행 중 입니다.
	DST_BUDOKAI_NEWS_INFO_STATE_WAIT_FINAL_MATCH,	// %s 결승이 %s 후에 시작됩니다.
	DST_BUDOKAI_NEWS_INFO_STATE_FINAL_MATCH,		// %s 결승이 진행 중 입니다.
	DST_BUDOKAI_NEWS_INFO_STATE_MATCH_END,			// %s 은 끝났습니다.

	DST_BUDOKAI_NEWS_NEXT_MUDOSA_OPEN,				// 다음 무도회 개최 일정 : %s
	DST_BUDOKAI_NEWS_MUDOSA_ENTER_LEFT_TIME,		// 무도사 마을 입장 종료 : %s 남았습니다.
	DST_BUDOKAI_NEWS_NOPLAN,						// 미정

	DST_BUDOKAI_REQUEST_TITLE,				// 신청서
	DST_BUDOKAI_TICKET_TITLE,				// 접수증

	DST_BUDOKAI_INDI_REQ_NAME,				// 이름
	DST_BUDOKAI_INDI_REQ_LEVEL,				// 레벨
	DST_BUDOKAI_INDI_REQ_CLASS,				// 클래스
	DST_BUDOKAI_INDI_REQ_RECORD,			// 전적
	DST_BUDOKAI_INDI_REQ_RANKING,			// 랭킹포인트
	DST_BUDOKAI_INDI_REQ_GUILD,				// 소속유파

	DST_BUDOKAI_INDI_REQ_RECORD_DATA,		// %d전 %d승 %d패

	DST_BUDOKAI_TEAM_REQ_TEAMNAME,			// 팀이름:
	DST_BUDOKAI_TEAM_REQ_TEAMLEVEL,			// Lv. %d

	DST_BUDOKAI_SUCCESS_APPLICATION,		// %s 신청이 완료되었습니다.
	DST_BUDOKAI_SUCCESS_GUIDE,				// %s의 접수번호:
	DST_BUDOKAI_SUCCESS_MEMBER,				// Member
	
	DST_BUDOKAI_INDI_DOJO_RECOMMNED,		// 도장 추천 본선 진출자
	DST_BUDOKAI_TEAM_LEAVE_NOTIFY,			// 팀의 리더가 천하제일 무도회 신청을 취소하였습니다.
	DST_BUDOKAI_TEAM_MEMBER_LEAVE_NOTIFY,	// %s가 천하제일 무도회 신청을 취소하였습니다.

	DST_BUDOKAI_REQ_BUTTON_ACCEPT,			// 신청하기
	DST_BUDOKAI_REQ_BUTTON_CANCEL,			// 신청취소

	// 신청 취소
	DST_BUDOKAI_INDI_REQ_CANCEL,			// 개인전 신청을 취소하시겠습니까?
	DST_BUDOKAI_TEAM_REQ_CANCEL,			// 팀전 신청을 취소하시겠습니까?

	DST_BUDOKAI_PC_STATE_NONE,				// 신청안함
	DST_BUDOKAI_PC_STATE_MINOR_APPLICANT,	// 예선 신청자
	DST_BUDOKAI_PC_STATE_MINOR_ENTRY,		// 예선 진출자
	DST_BUDOKAI_PC_STATE_32_ENTRY,			// 32강 진출자
	DST_BUDOKAI_PC_STATE_16_ENTRY,			// 16강 진출자
	DST_BUDOKAI_PC_STATE_8_ENTRY,			// 8강 진출자
	DST_BUDOKAI_PC_STATE_SEMIFINAL_ENTRY,	// 준결승 진출자
	DST_BUDOKAI_PC_STATE_FINAL_ENTRY,		// 결승 진출자

	DST_BUDOKAI_PC_STATE_MINOR_DROPOUT,		// 예선 탈락
	DST_BUDOKAI_PC_STATE_32_DROPOUT,		// 32강 탈락
	DST_BUDOKAI_PC_STATE_16_DROPOUT,		// 16강 탈락
	DST_BUDOKAI_PC_STATE_8_DROPOUT,			// 8강 탈락
	DST_BUDOKAI_PC_STATE_PRIZE_WINNER,		// 입상자
	DST_BUDOKAI_PC_STATE_SEMIFINAL_WINNER,	// 준우승자
	DST_BUDOKAI_PC_STATE_FINAL_WINNER,		// 우승자

	DST_BUDOKAI_MINORMATCH_INFO_TITLE,			// 배틀로얄
	DST_BUDOKAI_MINORMATCH_ENEMY_NAME_TITLE,	// 이름
	DST_BUDOKAI_MINORMATCH_KOCOUNTER_TITLE,		// K.O
	DST_BUDOKAI_MINORMATCH_ALIVE_TITLE,			// 남은 인원
	DST_BUDOKAI_MINORMATCH_TOTAL_TITLE,			// 전체 인원

	DST_BUDOKAI_REGISTER_INFO_TITLE,		// 등록 기간
	DST_BUDOKAI_MINORMATCH_INFO_TITLE_WAIT,	// 예선전 대기
	DST_BUDOKAI_MINORMATCH_INFO_TITLE_BATTLE,	// 예선전
	DST_BUDOKAI_MAJORMATCH_INFO_TITLE_32_WAIT,	// 32강 대기
	DST_BUDOKAI_MAJORMATCH_INFO_TITLE_32,	// 32강
	DST_BUDOKAI_MAJORMATCH_INFO_TITLE_16_WAIT,	// 16강 대기
	DST_BUDOKAI_MAJORMATCH_INFO_TITLE_16,	// 16강
	DST_BUDOKAI_MAJORMATCH_INFO_TITLE_8_WAIT,	// 8강 대기
	DST_BUDOKAI_MAJORMATCH_INFO_TITLE_8,	// 8강 
	DST_BUDOKAI_MAJORMATCH_INFO_TITLE_SEMIFINAL_WAIT,	// 준결승 대기
	DST_BUDOKAI_MAJORMATCH_INFO_TITLE_SEMIFINAL, // 준결승
	DST_BUDOKAI_MAJORMATCH_INFO_TITLE_FINAL, // 결승
	DST_BUDOKAI_MAJORMATCH_INFO_TITLE_END,		// 시상식

	DST_BUDOKAI_TIME_DAY,					// %d 일
	DST_BUDOKAI_TIME_HOUR,					// %d 시간
	DST_BUDOKAI_TIME_MINUTE,				// %d 분
	DST_BUDOKAI_TIME_SECOND,				// %d 초

	DST_BUDOKAI_TOURNAMENT_A_GROUP,			// A조
	DST_BUDOKAI_TOURNAMENT_B_GROUP,			// B조
	DST_BUDOKAI_TOURNAMENT_FINAL,			// 결선
	DST_BUDOKAI_TOURNAMENT_INFO_1,			// 토너먼트 표 팁내용.

	DST_BUDOKAI_TELEPORT_MSG_TBSERVER,		// 무도사 ( 사실은 천하제일 무도회 서버로 이동하는 거에요 )
	DST_BUDOKAI_TELEPORT_MSG_MATCH,			// %s 경기장 ( 본선 32강 경기장, 본선 16강 경기장, 결선 16강 경기장 등등.. )

	// Budokai Side icon
	DST_BUDOKAI_SIDEICON_TITLE,				// 천하제일 무도회

	DST_BUDOKAI_NOTICE_SOLO,				// 개인전
	DST_BUDOKAI_NOTICE_PARTY,				// 파티전

	// 소년부, 성인부 작업 때문에 분리합니다.
	DST_BUDOKAI_NOTICE_OPEN_NOTICE,			// 천하제일 무도회 %s 시작 공지입니다.
	DST_BUDOKAI_NOTICE_DOJO_RECOMMEND,		// %s 도장 추천이 가능합니다.
	DST_BUDOKAI_NOTICE_REGISTER,			// %s %s 예선 등록 신청을 받습니다.
	DST_BUDOKAI_NOTICE_WAIT_MINOR_MATCH,	// %s %s 예선전 대기 기간입니다.
	DST_BUDOKAI_NOTICE_MINOR_MATCH,			// %s %s 예선전이 시작되었습니다.
	DST_BUDOKAI_NOTICE_WAIT_MAJOR_MATCH,	// %s %s %s 대기 기간입니다.
	DST_BUDOKAI_NOTICE_MAJOR_MATCH,			// %s %s %s이 시작되었습니다.
	DST_BUDOKAI_NOTICE_WAIT_SEMIFINAL_MATCH,// %s %s 준결승 준비중입니다.
	DST_BUDOKAI_NOTICE_SEMIFINAL_MATCH,		// %s %s 준결승이 시작되었습니다.
	DST_BUDOKAI_NOTICE_WAIT_FINAL_MATCH,	// %s %s 결승 준비 기간입니다.
	DST_BUDOKAI_NOTICE_FINAL_MATCH,			// %s %s 결승전이 시작되었습니다.
	DST_BUDOKAI_NOTICE_END,					// %s %s 시상식이 진행중입니다.

	DST_BUDOKAI_NOTICE_DATA_LOADING,		// 천하제일 무도회의 정보를 불러오고 있습니다.

	// Budokai Gamble System
	DST_GAMBLE_GUIDE,						// 메뉴를 선택해주세요.\\n(캡슐키트를 뽑으려면 %d pt가 필요합니다. 
	DST_GAMBLE_GUIDE_RESULT,				// 캡슐이 나왔습니다.\\n캡슐키트에서 확인해주세요.
	DST_GAMBLE_COMMAND,						// 뽑기!!
	DST_GAMBLE_NOT_ENOUGH_MUDOSA_POINT,		// 무도사 포인트가 모자랍니다.

	// Budokai Tournament TeamInfo
	DST_BUDOKAI_TEAMINFO_TITLE,				// 경기정보
	DST_BUDOKAI_TEAMINFO_MATCH_NAME_COMMON,	// %d강전 제%u경기
	DST_BUDOKAI_TEAMINFO_MATCH_NAME_SEMIFINAL,// 준결승 제 %u경기
	DST_BUDOKAI_TEAMINFO_MATCH_NAME_FINAL,	// 결승
	DST_BUDOKAI_TEAMINFO_CLASS,				// 클래스
	DST_BUDOKAI_TEAMINFO_LEVEL,				// 레벨
	DST_BUDOKAI_TEAMINFO_RECORD,			// 전적
	DST_BUDOKAI_TEAMINFO_RECORD_DATA,		// %u전 %u승 %u패
	DST_BUDOKAI_TEAMINFO_RANKING_POINT,		// 랭킹포인트(PT)
	DST_BUDOKAI_TEAMINFO_GUILD_NAME,		// 소속유파
	DST_BUDOKAI_TEAMINFO_LEVEL_CLASS,		// Lv.%u %s

	DST_BUDOKAI_TEAMINFO_LEVEL_VALUE,		// Lv.%u
	DST_BUDOKAI_TEAMINFO_PRIVATE_RECORD,	// 개인전적
	DST_BUDOKAI_TEAMINFO_NOT_IN_GUILD,		// 미가입

	DST_BUDOKAI_TEAMINFO_DOJO_RECOMMAND,	// 도장 대표	

	// Budokai Result Narration
	DST_BUDOKAI_RESULT_MESSAGE,				// 수고하셨습니다. 무도사 마을로 이동합니다. (HTML형식)
	DST_BUDOKAI_RESULT_MESSAGE_SEMIFINAL_1,	// HTML : 그야말로 쟁쟁한 실력파들의 대결입니다. 이번 대회는 멋진 대회가 될 것 같습니다. 
	DST_BUDOKAI_RESULT_MESSAGE_SEMIFINAL_2,	// HTML : 곧 이어 다음 경기가 시작됩니다. 이 대회에서 우승하여 세계 완전 제패를 노리시기 바랍니다.
	DST_BUDOKAI_RESULT_MESSAGE_FINAL,		// HTML : 최종 승자가 가려졌습니다. 우승입니다. 우승! 우승자에게는 막대한 부와 명예가 주어집니다. 축하드립니다.

	// 시작
	DST_BUDOKAI_PROGRESS_MESSAGE_START,					// 천하제일무도회가 시작됩니다.
	DST_BUDOKAI_PROGRESS_MESSAGE_RECEIVE_NEWS,			// 소식지가 배달 되었습니다.

	// 예선
	DST_BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ENTERING,	// 예선 진입 시작합니다. 
	DST_BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_ENTERING2,  // 입장을 시작하셔도 좋습니다.
	DST_BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_BEFORE_1MIN,// 곧 예선 전투가 시작됩니다.
	DST_BUDOKAI_PROGRESS_MESSAGE_MINORMATCH_BEFORE_1MIN2,// 입장을 서둘러 주십시오.

	DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_END,				// %s 님의 %s 전투가 종료되었습니다.
	DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_WINNER,			// 축하합니다. %s 진출이 확정 되셨습니다.
	DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_LOSER,			// 안타깝게도 %s에 진출하지 못하셨습니다.
	DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_ALL_FINISHED,	// %s이 모두 종료되었습니다.
	DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_DECISION_NEXT,	// %s 진출자가 모두 결정되었습니다.
	DST_BUDOKAI_PROGRESS_MESSAGE_MATCH_BEFORE_10SEC,	// 10초 후 %s 경기장으로 자동 진입 합니다.

	DST_BUDOKAI_PROGRESS_MESSAGE_MINOR,					// 예선
	DST_BUDOKAI_PROGRESS_MESSAGE_32,					// 32강
	DST_BUDOKAI_PROGRESS_MESSAGE_16,					// 16강
	DST_BUDOKAI_PROGRESS_MESSAGE_8,						// 8강
	DST_BUDOKAI_PROGRESS_MESSAGE_SF,					// 준 결승

	// 준 결승
	DST_BUDOKAI_PROGRESS_MESSAGE_SEMI_FINAL_RUNNING,		// 준 결승이 진행중입니다. 대기 하여 주십시오.
	DST_BUDOKAI_PROGRESS_MESSAGE_FINAL_WAIT_LOSER,			// 결승이 종료 된 후 시상식이 있습니다.
	DST_BUDOKAI_PROGRESS_MESSAGE_FINAL_WAIT_LOSER1,			// 지금 나가시면 보상을 받을 수 없습니다.

	DST_BUDOKAI_PROGRESS_MESSAGE_END,						// 천하제일 무도회가 모두 종료되었습니다

	DST_BUDOKAI_MUDOSA_INFO,								// 무도사 마을 %d번
	DST_BUDOKAI_MUDOSA_TELEPOR_INFO,						// 무도사 마을로 이동합니다.

	// loading
	DST_TODAT_TIP,							// 오늘의 도움말

	// Petition
	DST_PETITION_TITLE,						// GM 상담 요청
	DST_PETITION_GREAT_CATEGORY,			// 대분류
	DST_PETITION_SMALL_CATEGORY,			// 소분류
	DST_PETITION_SATISFACTION_TITLE,		// GM 상담 요청
	DST_PETITION_SATISFACTION_COMMENT,		// 상담 내용에 대한 의견을 알려주세요
	DST_PETITION_SATISFACTION_GREAT,		// 아주 만족
	DST_PETITION_SATISFACTION_GOOD,			// 만족
	DST_PETITION_SATISFACTION_NOT_BAD,		// 보통
	DST_PETITION_SATISFACTION_BAD,			// 불만
	DST_PETITION_SATISFACTION_WORST,		// 매우 불만
	DST_PETITION_SEND,						// 전송
	DST_PETITION_GM_CHAT_TITLE,				// GM 대화창

	DST_PETITION_CATEGORY1_ACCUSE,			// 불량 이용자 신고
	DST_PETITION_CATEGORY1_BUG,				// 버그 관련
	DST_PETITION_CATEGORY1_GAMEINFO,		// 게임 정보
	DST_PETITION_CATEGORY1_STUCK,			// 캐릭터 이동 불가

	DST_PETITION_CATEGORY2_ACCUSE_AUTOPROGRAM,	// 불법 프로그램
	DST_PETITION_CATEGORY2_ACCUSE_FRAUD,		// 사기
	DST_PETITION_CATEGORY2_ACCUSE_PAPERING,		// 도배
	DST_PETITION_CATEGORY2_BUG_SYSTEM,			// 시스템
	DST_PETITION_CATEGORY2_BUG_GRAPHIC,			// 그래픽
	DST_PETITION_CATEGORY2_BUG_ETC,				// 기타
	DST_PETITION_CATEGORY2_GAME_INFO,			// 게임 정보
	DST_PETITION_CATEGORY2_STUCK,				// 캐릭터 이동 불가

	DST_PETITION_CANCEL_PETITION,				// GM 상담 취소

	// Teleport
	DST_TELEPORT_COMMON_CONFIRM_MSG,			// 이 장소로 순간 이동 하시겠습니까? ( 장소와 제한시간 )
	DST_TELEPORT_COMMON_CONFIRM_YES_BTN,		// 이동한다.
	DST_TELEPORT_COMMON_CONFIRM_NO_BTN,			// 가지 않는다.
	DST_TELEPORT_FINAL_CONFIRM_MSG,				// 정말 순간이동을 취소하시겠습니까? ( 취소시 해당장소로 이동할 수 없다는 것을 명시 )
	DST_TELEPORT_FINAL_CONFIRM_YES_BTN,			// 순간이동을 취소
	DST_TELEPORT_FINAL_CONFIRM_NO_BTN,			// 다시 생각하기

	// Observer system
	DST_OBSERVER_RECORD,						// %d전 %d승 %d패(Playerinfo 창에서..)

	DST_OBSERVER_SHOW_AVATAR,					// 아바타 보이기
	DST_OBSERVER_HIDE_AVATAR,					// 아바타 숨기기
	DST_OBSERVER_TENKAICHI,						// 천하제일 무도회 메뉴
	DST_OBSERVER_PORTAL,						// 아바타 이동
	DST_OBSERVER_FREE_CAMERA,					// 프리 카메라
	DST_OBSERVER_NORMAL_CAMERA,					// 3인칭 카메라

	DST_OBSERVER_CAMERA_LOC,					// 카메라 위치 변경
	DST_OBSERVER_CAMERA_MOVE,					// 카메라 이동
	DST_OBSERVER_MENU_ON,						// 메뉴 On
	DST_OBSERVER_MENU_OFF,						// 메뉴 Off
	DST_OBSERVER_PLAYER_INFO,					// 선수 정보
	DST_OBSERVER_TOURNAMENT,					// 대진표 

	// 탈 것
	DST_VEHICLE_GET_OFF,						// 내리기
	DST_VEHICLE_ENGINE_START,					// 엔진 시동걸기
	DST_VEHICLE_ENGINE_STOP,					// 엔진 멈추기

	// HoipoiMix System
	DST_HOIPOIMIX_CREATE,						// 호이포이 캡슐 제조하기
	DST_HOIPOIMIX_FARERATE,						// 호이포이 머신 사용료 설정
	DST_HOIPOIMIX_MACHINE_DEL,					// 호이포이 머신 회수하기	

	DST_HOIPOIMIX_RECIPE_TAB,					// 레시피
	DST_HOIPOIMIX_RECIPE_FILTER_TOTAL,			// 전체	( 레시피 목록의 필터 버튼 )
	DST_HOIPOIMIX_RECIPE_FILTER_BUILT_IN,		// 머신 ( 레시피 목록의 필터 버튼 )
	DST_HOIPOIMIX_RECIPE_FILTER_NORMAL,			// 일반 ( 레시피 목록의 필터 버튼 )
	DST_HOIPOIMIX_RECIPE_FILTER_SPECIAL,		// 특수 ( 레시피 목록의 필터 버튼 )

	DST_HOIPOIMIX_CREATE_TAB,					// 제조

	DST_HOIPOIMIX_REQUIRED_LEVEL,				// 필요 제조 레벨 : %d
	DST_HOIPOIMIX_REQUIRED_ZENNY,				// 제조 비용 : %d
	DST_HOIPOIMIX_UNSET_FARERATE,				// 호이포이 믹스 머신 가격이 설정되어 있지 않습니다.

	DST_HOIPOIMIX_MATERIAL_CATEGORY,			// 재료 및 수량 설정
	DST_HOIPOIMIX_PREVIEW_CATEGORY,				// 미리보기
	DST_HOIPOIMIX_CONCENT_CATEGORY,				// 집중력 컨트롤

	DST_HOIPOIMIX_CONCENT_RATE_PERFECT,			// 완벽함!
	DST_HOIPOIMIX_CONCENT_RATE_GOOD,			// 일치함!
	DST_HOIPOIMIX_CONCENT_RATE_BAD,				// 일치하지 않음!

	DST_HOIPOIMIX_BUTTON_CREATE,				// 만들기
	DST_HOIPOIMIX_BUTTON_STOP,					// 중지하기

	DST_HOIPOIMIX_CREATE_SUCCESS_NORMAL,		// (제조창) %s 제조를 성공했습니다. (일반)
	DST_HOIPOIMIX_CREATE_SUCCESS_GREAT,		// (제조창) %s 제조를 성공했습니다. (대성공)
	DST_HOIPOIMIX_CREATE_FAILED,				// (제조창) 제조가 실패하여 %s 를 만들었습니다.

	DST_HOIPOIMIX_CREATE_SUCCESS_NORMAL_NOTIFY,	// (알림) 호이포이 믹스 성공! %s 를 제조하였습니다. (일반)
	DST_HOIPOIMIX_CREATE_SUCCESS_GREAT_NOTIFY,	// (알림) 호이포이 믹스 대성공! %s 를 제조하였습니다. (대성공)
	DST_HOIPOIMIX_CREATE_FAILED_NOTIFY,			// (알림) 호이포이 믹스 실패! %s 를 제조하였습니다. (실패)

	DST_HOIPOIMIX_CREATE_FIRST_GUIDE,			// (호이포이믹스 제조창 하단에 처음으로 뜨는 가이드 메시지) 스페이스바를 타이밍에 맞춰 눌러 어쩌고 저쩌고..
	DST_HOIPOIMIX_CREATE_NEED_MATERIAL,			// 재료가 모자라서 수량을 설정할 수 없습니다.
	DST_HOIPOIMIX_CREATE_NEED_SET_NUMBER,		// 먼저 수량을 설정해야 제조를 시작 할 수 있습니다.

	DST_HOIPOIMIX_REGISTER_RECIPE,				// %s 레시피를 스카우터에 등록했습니다.
	DST_HOIPOIMIX_REGISTER_SKILL,				// 축하합니다. %s 제조 기술을 배웠습니다.
	DST_HOIPOIMIX_SKILL_SET_MSGBOX,				// [MSGBOX]이 제조 기술을 배우시겠습니가?[br]%s 제조기술[br]레벨제한 : %d[br]소비제니 : %d
	DST_HOIPOIMIX_SKILL_RESET_MSGBOX,			// [MSGBOX]이 제조 기술을 포기하시겠습니까?[br]%s 제조기술[br]제조 경험치와 제조레벨이...
	DST_HOIPOIMIX_SKILL_RESET_RESULT,			// %s 제조기술을 포기했습니다.

	DST_HOIPOIMIX_TOOLTIP_MAX,					// 현재 가진 아이템을 자동으로 검색해서 만들 수 있는 최대 갯수로 설정합니다.
	DST_HOIPOIMIX_TOOLTIP_CALC,					// 호이포이 믹스 캡슐 아이템을 몇개를 만들지 수량을 정합니다.
	DST_HOIPOIMIX_TOOLTIP_CREATE,				// 집중력 컨트롤 위에서 마우스 클릭을 해서 제조할 때 EP를 충전할 수 있습니다. 모두 다 성공하면 대성공 확률이 올라갑니다.
	DST_HOIPOIMIX_TOOLTIP_STOP,					// 제조하는 것을 중단 할 수 있습니다.

	// Item
	DST_ITEM_RECIPE_TYPE_FOOD_DRINK,			// FOOD DRINK
	DST_ITEM_RECIPE_TYPE_MAIN_WEAPON,			// MAIN WEAPON
	DST_ITEM_RECIPE_TYPE_ARMOR,					// ARMOR
	DST_ITEM_RECIPE_TYPE_ACCESSORY,				// ACCESSORY
	DST_ITEM_RECIPE_TYPE_SCOUTER,				// SCOUTER
	DST_ITEM_RECIPE_TYPE_SUB_WEAPON,			// SUB WEAPON

	// ETC
	DST_ETC_PERSONS,							// 명
	DST_FINISHED_MONSTER,						// %s 잡기에 성공하였습니다(던전 등에서 보스를 잡았을 때 화면 중앙에 뿌리는 메세지)
	DST_ZENNY,									// 제니

	// SYSTEM EFFECT 변환용
	DST_SYSTEMEFFECT_PHYSICAL,					// 물리
	DST_SYSTEMEFFECT_ENERGY,					// 기공
	DST_SYSTEMEFFECT_STATE,						// 상태이상

	DST_SYSTEMEFFECT_METER,						// 미터

	// 약관
	DST_CONTRACT_PLEASE_READ,					// 계약 내용을 끝까지 읽으면 ""동의"" 버튼이 활성화 됩니다
	DST_CONTRACT_ACCEPT,						// 동 의
	DST_CONTRACT_NOT_ACCEPT,					// 동의 안함

	// 부활 메시지 박스 커스텀 버튼
	DST_REGEN_MB_MAINWORLD_MOVE_TO_POPOSTONE_BTN,	//"이동하기"
	DST_REGEN_MB_TLQ_MOVE_TO_MAINWORLD_BTN,		//"돌아가기"
	DST_REGEN_MB_TLQ_RETRY_BTN,					//"재도전"
	DST_REGEN_MB_TMQ_MOVE_TO_MAINWORLD_BTN,		//"돌아가기"
	DST_REGEN_MB_PARTYDUNGEON_RETRY_BTN,			//"재도전"
	DST_REGEN_MB_DOJO_RETRY_BTN,					//"재도전"

	// 도장
	DST_DOJO,									// 도장
	DST_DOJO_UPGRADE,							// 도장 보강하기
	DST_DOJO_NAME,								// 도장 이름
	DST_DOJO_NEED_GUILD_POINT,					// 필요한 유파 포인트
	DST_DOJO_NEED_ZENNY,						// 필요한 제니
	DST_DOJO_NEED_ITEM,							// 필요한 아이템
	DST_DOJO_RUN_UPGRADE,						// 보강하기

	DST_DOJO_ACCOUNT_WAREHOUSE,					// 도장 창고 사용내역
	DST_DOJO_ACCOUNT_EXPENDITURE,				// 사용내역
	DST_DOJO_ACCOUNT_ITEM,						// 내용
	DST_DOJO_ACCOUNT_AMOUNT,					// 금액
	DST_DOJO_ACCOUNT_BALANCE,					// 잔액
	DST_DOJO_ACCOUNT_TYPE_LEADER_INCOME,		// 사범 입금
	DST_DOJO_ACCOUNT_TYPE_LEADER_OUTCOME,		// 사범 출금
	DST_DOJO_ACCOUNT_TYPE_VILLAGE_INCOME,		// 기부 출금
	DST_DOJO_ACCOUNT_TYPE_BATTLE_OUTCOME,		// 배상 출금
	DST_DOJO_ACCOUNT_TYPE_BATTLE_INCOME,		// 신청 환급금

	DST_DOGI_SETUP,								// 도복 설정
	DST_DOGI_PREVIEW,							// 도복 미리보기
	DST_DOGI_COLOR,								// 도복 색상

	DST_DOJO_PEACE_STATE_PANIC,					// 공황
	DST_DOJO_PEACE_STATE_FEAR,					// 불안
	DST_DOJO_PEACE_STATE_COMMON,				// 보통
	DST_DOJO_PEACE_STATE_QUIET,					// 평온
	DST_DOJO_PEACE_STATE_FESTIVAL,				// 축제

	DST_DOJO_INFO,								// 도장 정보
	DST_DOJO_INFO_INFO,							// 정보
	DST_DOJO_INFO_BATTLE,						// 전투

	DST_DOJO_INFO_SET_TENKAI_SEED_PLAYER,		// 천하제일 무도회 시드 출전자 지정
	DST_DOJO_INFO_GUILD_NOTICE,					// 길드 공지
	DST_DOJO_INFO_DOJO_INFO,					// 도장 정보
	DST_DOJO_INFO_DOJO_NAME,					// 도장 이름
	DST_DOJO_INFO_DOJO_LEVEL,					// 도장 레벨
	DST_DOJO_INFO_CITY_NAME,					// 마을 이름
	DST_DOJO_INFO_GUILD_NAME,					// 유파 이름
	DST_DOJO_INFO_GUILD_POINT,					// 유파 포인트
	DST_DOJO_INFO_PUBLIC_SECURITY,				// 마을 치안
	DST_DOJO_INFO_TENKAI_SEED_PLAYER,			// 무도회 출전자
	DST_DOJO_INFO_NOTICE_INPUT,					// 도장 공지 입력

	DST_DOJO_BATTLE_APPLICATION_FOR_BATTLE,		// 신청하기
	DST_DOJO_BATTLE_DUEL_DEFFENCE_PLAYER,		// 비무 출전자 - 방어측
	DST_DOJO_BATTLE_DULE_OFFENCE_PLAYER,		// 비무전 도전 유파
	DST_DOJO_BATTLE_SCRAMBLE_OFFENCE_PLAYER,	// 쟁탈전 도전 유파

	DST_DOJO_SCRAMBLE_ACCEPT_CHALLENGE_OK_BTN,		// 수락
	DST_DOJO_SCRAMBLE_ACCEPT_CHALLENGE_CANCEL_BTN,	// 거부
	DST_DOJO_SCRAMBLE_ASK_CHALLENGE_OK_BTN,			// 신청
	DST_DOJO_SCRAMBLE_ASK_TELEPORT_OK_BTN,			// 이동

	DST_DOJO_SCRAMBLE_RULE_TITLE,					// 쟁탈전을 승리하기 위한 두 가지 방법
	DST_DOJO_SCRAMBLE_RULE,							// 1. 사범실, 연공실, 보급실, 접견실, 비무대에 있는 총 5개의 인장을 점유하여 쟁탈점수를 100점 획득하라") 2.두 시간의 쟁탈전 동안 상대유파보다 많은 쟁탈점수를 획득하라
	
	DST_DOJO_SCRAMBLE_REWARD_REPUTATION,			// 획득 유파 포인트
	DST_DOJO_SCRAMBLE_REWARD_ITEM,					// 획득 아이템

	DST_DOJO_SCRAMBLE_TITLE,						// 도장 쟁탈전
	DST_DOJO_SCRAMBLE_GUILD_MEMBER_COUNT,			// 유파 인원
	DST_DOJO_SCRAMBLE_OWNER_SEAL_COUNT,				// 획득 인장 갯수
	DST_DOJO_SCRAMBLE_ENTER,						// 입장하기

	DST_DOJO_SCRAMBLE_OUR_GUILD_SEAL,				// 우리 유파 소유
	DST_DOJO_SCRAMBLE_OTHER_GUILD_SEAL,				// 상대 유파 소유
	DST_DOJO_SCRAMBLE_NO_OWNER_SEAL,				// 소유 유파 없음

	DST_DOJO_UPGRADE_CONSUMPTION_GUILD_POINT,		// 소모 유파 포인트
	DST_DOJO_UPGRADE_CONSUMPTION_ZENNY,				// 소모 제니
	DST_DOJO_UPGRADE_NEED_ITEM,						// 호이포이락

	// 도장 현재 레벨의 특징과 다음 레벨의 특징을 설명
	DST_DOJO_UPGRADE_CURENT_LEVEL_1,				// 현재 레벨 1
	DST_DOJO_UPGRADE_CURENT_LEVEL_2,				// 현재 레벨 2
	DST_DOJO_UPGRADE_CURENT_LEVEL_3,				// 현재 레벨 3
	DST_DOJO_UPGRADE_CURENT_LEVEL_4,				// 현재 레벨 4
	DST_DOJO_UPGRADE_CURENT_LEVEL_5,				// 현재 레벨 5
	DST_DOJO_UPGRADE_CURENT_LEVEL_6,				// 현재 레벨 6
	DST_DOJO_UPGRADE_CURENT_LEVEL_7,				// 현재 레벨 7

	DST_DOJO_UPGRADE_NEXT_LEVEL_2,					// 다음 레벨 2
	DST_DOJO_UPGRADE_NEXT_LEVEL_3,					// 다음 레벨 3
	DST_DOJO_UPGRADE_NEXT_LEVEL_4,					// 다음 레벨 4
	DST_DOJO_UPGRADE_NEXT_LEVEL_5,					// 다음 레벨 5
	DST_DOJO_UPGRADE_NEXT_LEVEL_6,					// 다음 레벨 6
	DST_DOJO_UPGRADE_NEXT_LEVEL_7,					// 다음 레벨 7

	//
	DST_RPBALL_TOOLTIP,							// RPBall 툴팁

	// 사용기간 제한 관련
	DST_COMMERCIAL_MB_UPDATE_TERM_LIMETED_ITEM_BTN,	// 기간 연장
	DST_COMMERCIAL_MB_DELETE_TERM_LIMETED_ITEM_BTN,	// 버리기
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_INFO,		// 남은 사용기간\n%s
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_INFO_BELOW_HOUR,	// 1시간 이하
	DST_COMMERCIAL_USE_TERM_LIMITED_ITEM_EXPIRED_INFO,	// 사용기간 만료
	DST_COMMERCIAL_USE_TERM_LIMITED_ITEM_TITLE,		// 사용기간 연장
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_REMIAN_TIME,	// 남은 사용기간
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_ADD_TIME,		// 추가 사용기간
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CONSUME_JENNY,	// 소모 제니
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_OWNED_JENNY,	// 보유 제니
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CONSUME_NETPHY,	// 소모 넷피
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_OWNED_NETPHY,	// 보유 넷피
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CONSUME_CASH,	// 구매 CC Cash
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_OWNED_CASH,	// 보유 CC Cash
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_EXTEND_Q,		// 아이템의 사용시간을 연장하시겠습니까?
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_EXTEND_Q,	// 아이템의 사용시간을 구매하여 연장하시겠습니까?
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_BUY_WARN_DESC1, // 상품구매시 되팔기, 교환, 환불이 불가능합니다.
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_BUY_WARN_DESC2, // 청약 철회(구매취소, 환불)가 불가능합니다.
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_CASH_BUY_WARN_DESC3, // 구매 아이템 List확인 Page 정보
	DST_COMMERCIAL_USE_TERM_BUY_DURATION,			// 기간 구매
	DST_COMMERCIAL_REFILL_CASH,						// 캐쉬 충전
	DST_COMMERCIAL_USE_TERM_LIMITED_ITEM_BUY_TITLE,		// 구매 확인
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_BUY_Q1,		// 요청하신 구매내역은 위와 같습니다.
	DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_BUY_Q2,		// 확인을 누르시면 구매가 완료됩니다.

	DST_COMMERCIAL_MB_DIRECT_BUY_MSG,					// 안내 메시지, 아이템 이름 %S, 소모제니 : %d
	DST_COMMERCIAL_MB_DIRECT_BUY_NETPY_MSG ,			// 안내 메시지, 아이템 이름 %S, 소모넷피 : %d
	DST_COMMERCIAL_MB_ITEM_BUY_MSG,						// 기간구매
	DST_COMMERCIAL_MB_ITEM_TERM_EXTENTION_BTN,			// 기간연장
	DST_COMMERCIAL_MB_CAPSULEKIT_ITEM_TERM_EXTENTION,	// 캐슐키트기간연장
	DST_COMMERCIAL_MB_CAPSULEKIT_TERM_EXTENTION_BTN,	//기간연장
	DST_COMMERCIAL_MB_CAPSULEKIT_BUY_BTN,				//아이템 구입
	DST_COMMERCIAL_MB_ITEM_BUY_CONFIRM_MSG,				//"안내 메시지"
	DST_COMMERCIAL_MB_ITEM_TAKE_CONFIRM_MSG,			// "안내 메시지"
	DST_COMMERCIAL_MB_TERM_OR_BUY_SELECT_CONFIRM_MSG,	// "안내 메시지"
	DST_COMMERCIAL_MB_TERM_OR_BUY_SELECT_CONFIRM_BTN,	////////// 추가
	DST_COMMERCIAL_MB_CAPSULEKIT_ITEM_SELECT,			// "안내 메시지"
	DST_COMMERCIAL_MB_CAPSULEKIT_ITEM_TERM_EXTENTION_BTN,// 기간연장
	DST_COMMERCIAL_MB_CAPSULEKIT_MOVE_BTN,				// 아이템 구입
	DST_COMMERCIAL_MB_ITEM_MOVE_CONFIRM_MSG,			// "안내 메시지"
	// DST_COMMERCIAL_MB_ITEM_MOVE_CONFIRM_MSG용
	DST_COMMERCIAL_MB_TERM_EXTENTION,					////////// 추가
	DST_COMMERCIAL_MB_TERM_EXTENTION_BTN,				// "기간연장" 

	DST_LACK_OF_ZENNY,									// 보유한 제니가 부족합니다.
	DST_LACK_OF_NETPY,									// 보유한 넷피가 부족합니다.
	DST_LACK_OF_CASH,									// 보유한 캐쉬가 부족합니다.

	DST_CASH,						// 캐쉬
	DST_DAY,						// 일
	DST_TIME,						// 시간
	DST_MINUTE,						// 분

	// N-Protect
	DST_NPGAMEMON_COMM_ERROR,							// 게임가드와의 통신이 끊어졌습니다.
	DST_NPGAMEMON_COMM_CLOSE,							// 게임가드가 종료되었습니다.
	DST_NPGAMEMON_INIT_ERROR,							// 게임가드 실행 에러 : %lu
	DST_NPGAMEMON_SPEEDHACK,							// 스피드핵이 감지되었습니다.
	DST_NPGAMEMON_GAMEHACK_KILLED,						// 게임핵이 발견되었습니다.
	DST_NPGAMEMON_GAMEHACK_DETECT,						// 게임핵이 발견되었습니다.
	DST_NPGAMEMON_GAMEHACK_DOUBT,						// 게임이나 게임가드가 변조되었습니다.
	DST_NPGAMEMON_ERROR_EXIST,							// 게임가드가 실행 중 입니다. 잠시 후나 재부팅 후에 다시 실행해보시기 바랍니다.
	DST_NPGAMEMON_ERROR_GAME_EXIST,						// 게임이 중복 실행되었거나 게임가드가 이미 실행 중 입니다. 게임 종료 후 다시 실행해보시기 바랍니다.
	DST_NPGAMEMON_ERROR_INIT,							// 게임가드 초기화 에러입니다. 재부팅 후 다시 실행해보거나 충돌할 수 있는 다른 프로그램들을 종료한 후 실행해 보시기 바랍니다.
	DST_NPGAMEMON_ERROR_NO_CONFIG_FILE,					// 게임가드 설정 파일이 없거나 변조되었습니다.
	DST_NPGAMEMON_ERROR_CRYPTOAPI,						// 윈도우의 일부 시스템 파일이 손상되었습니다. 인터넷 익스플로러(IE)를 다시 설치해보시기 바랍니다.
	DST_NPGAMEMON_ERROR_EXECUTE,						// 게임가드 실행에 실패했습니다. 게임가드 셋업 파일을 다시 설치해보시기 바랍니다.
	DST_NPGAMEMON_ERROR_ILLEGAL_PRG,					// 불법 프로그램이 발견되었습니다. 불필요한 프로그램을 종료한 후에 게임을 다시 실행해보시기 바랍니다.
	DST_NPGMUP_ERROR_ABORT,								// 게임가드 업데이트를 취소하셨습니다. 접속이 계속 되지 않을 경우 인터넷 및 개인 방화벽 설정을 조정해 보시기 바랍니다.
	DST_NPGMUP_ERROR_CONNECT,							// 게임가드 업데이트 서버 접속에 실패하셨습니다. 잠시 후 다시 접속하거나 네트웍 상태를 점검해 보시기 바랍니다.
	DST_NPGAMEMON_ERROR_GAMEGUARD,						// 게임가드 초기화 에러 또는 구버젼의 게임가드 파일입니다. 게임가드 셋업 파일을 다시 설치하고 게임을 실행해 보시기 바랍니다.
	DST_NPGMUP_ERROR_PARAM,								// ini 파일이 없거나 변조되었습니다. 게임가드 셋업 파일을 설치하시면 해결할 수 있습니다.
	DST_NPGMUP_ERROR_INIT,								// npgmup.des 초기화 에러입니다. 게임가드 폴더를 삭제후 다시 게임실행을 해보시기 바랍니다.
	DST_NPGMUP_ERROR_DOWNCFG,							// 게임가드 업데이트 서버 접속에 실패하였습니다. 잠시 후 다시 시도해 보거나 개인 방화벽이 있다면 설정을 조정해 보시기 바랍니다.
	DST_NPGMUP_ERROR_AUTH,								// 게임가드 업데이트를 완료하지 못 했습니다. 바이러스 백신을 일시 중지시킨 후 재시도 해보시거나, pc 관리 프로그램을 사용하신다면 설정을 조정해 보시기 바랍니다.
	DST_NPGAMEMON_ERROR_NPSCAN,							// 바이러스 및 해킹툴 검사 모듈 로딩에 실패 했습니다. 메모리 부족이거나 바이러스에 의한 감염일 수 있습니다.
	DST_NPGAMEGUARD_ERROR_ETC,							// 게임가드 실행 중 에러가 발생하였습니다. 게임 폴더 안의 GameGuard 폴더에 있는 *.erl 파일들을 Game1@inca.co.kr 로 첨부하여 메일 보내주시기 바랍니다.

	// PC방 관련
	DST_NETPY_REMAIN_TIME,                              // 다음 넷피 획득까지 남은 시간
	DST_NETPY_GET_NETPY,                                // 접속 후 획득한 넷피
	DST_NETPY_CURRENT_NETPY,                            // 현재 보유 넷피
	DST_NETPY_GET_NOTIFY,                               // 넷피 %d 포인트를 획득했습니다.
	DST_NETPY_POINT,                                    // 포인트

	DST_NETPY,											// 넷피
	DST_NETPYSHOP,										// 넷피샾
	DST_NETSTORE,										// 넷스토어

	DST_INFOWINDOW_USE_STATE,							// 사용 조건
	DST_INFOWINDOW_ZENNY,								// %d Zenny 
	DST_INFOWINDOW_RECIPE_MAT,							// 레시피 재료
	DST_INFOWINDOW_CREATE_ITEM,							// 믹스 아이템(만들어질 아이템)
	DST_INFOWINDOW_MAKE_ZENNY,							// 제조 비용
	DST_INFOWINDOW_SELL_PRICE,							// 판매 가격
	DST_INFOWINDOW_MAKE_LEVEL,							// 제조 레벨 : %d 레벨

	// Teleport Type
	DST_TELEPORT_TYPE_GAME_IN,							// 게임에 처음 접속했을 때의 텔레포트	
	DST_TELEPORT_TYPE_GAME_OUT,							// 게임에 나갈 때의 텔레포트
	DST_TELEPORT_TYPE_SKILL,							// 스킬
	DST_TELEPORT_TYPE_NPC_PORTAL,						// NPC Portal
	DST_TELEPORT_TYPE_POPOSTONE,						// 포포스톤
	DST_TELEPORT_TYPE_WORLD_MOVE,						// 월드 이동
	DST_TELEPORT_TYPE_TMQ_WORLD_MOVE,					// TMQ
	DST_TELEPORT_TYPE_TMQ_PORTAL,						// TMQ
	DST_TELEPORT_TYPE_RANKBATTLE,						// 랭크배틀
	DST_TELEPORT_TYPE_TIMEQUEST,						// TIME QUEST
	DST_TELEPORT_TYPE_TUTORIAL,							// 튜토리얼
	DST_TELEPORT_TYPE_BUDOKAI,							// 천하제일무도회 서버
	DST_TELEPORT_TYPE_MATCH,							// 천하제일무도회 경기
	DST_TELEPORT_TYPE_MINORMATCH,						// 천하제일무도회 예선
	DST_TELEPORT_TYPE_MAJORMATCH,						// 천하제일무도회 본선
	DST_TELEPORT_TYPE_FINALMATCH,						// 천하제일무도회 결선
	DST_TELEPORT_TYPE_COMMAND,							// COMMAND
	DST_TELEPORT_TYPE_GM,								// GM
	DST_TELEPORT_TYPE_DUNGEON,							// DUNGEON
	DST_TELEPORT_TYPE_DOJO,								// 도장

	// GMT
	DST_OPTION_GMT_TAB,									// 매니아 타임 ( 탭에 들어가는 텍스트 )
	DST_GMT_TITLE,										// 게임 매니아 타임
	DST_GMT_PROFILE,									// 게임 플레이에 도움이 되는 추가 획득 시간대를말합니다. 재설정 횟수는...
	DST_GMT_EXT_GET,									// 추가 획득
	DST_GMT_RESET,										// 재설정
	DST_GMT_APPLY_TIME,									// 적용 시간
	DST_GMT_CHANGE_TIME,								// 변경될 시간

	DST_GMT_LIST_EXP,									// 경험치 추가 획득 %u%%
	DST_GMT_LIST_ZENNY,									// 제니 추가 획득 %u%%

	DST_GMT_REMAIN_COUNT,								// 남은 재설정 횟수 : %d
	DST_GMT_TIME_INFO,									// %2d:00
	DST_GMT_NO_TIME,									// 없음
	
	DST_GMT_RESET_TITLE,								// 매니아 타임 설정

	DST_GMT_RESET_ALREADY_OPEN,							// GMT 세팅 창이 이미 열려 있습니다. ( 메시지박스 )
	DST_GMT_RESET_COUNT_OVER,							// 정할 수 있는 시간 개수를 초과하였습니다.
	
	DST_GMT_ENABLE_CHECK_COUNT,							// 선택 가능한 시간대 갯수 : %d
	
	DST_GMT_DONOT_CHECK_ALL,							// GMT를 재설정 하기 위해서는 모든 GMT 항목의 시간대를 다 체크하여야 합니다.

};

#endif