/*****************************************************************************
*
* File			: NtlGameRuleConcept.h
* Author		: JeongHo, Rho
* Copyright	: (주)NTL
* Date			: 2008. 1. 8	
* Abstract		: Game rule concept
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_GAME_RULE_CONCEPT_H__
#define __NTL_GAME_RULE_CONCEPT_H__


enum eGAME_RULE_TYPE
{
	// Main Layer
	// ( 한순간에 Main layer에 해당하는 상태끼리는 중복될 수 없음 )
	GAME_RULE_TYPE_MAIN_NORMAL,						// 일반
	GAME_RULE_TYPE_MAIN_PRIVATESHOP_READY,			// 개인 상점 개설
	GAME_RULE_TYPE_MAIN_PRIVATESHOP_RUN,			// 개인 상점 판매
	GAME_RULE_TYPE_MAIN_TMQ_WAIT_LOTTERY,			// 타이머신 신청
	GAME_RULE_TYPE_MAIN_TMQ_READY,					// 타이머신 진입 대기
	GAME_RULE_TYPE_MAIN_TMQ_RUN,					// 타이머신 퀘스트 진행
	GAME_RULE_TYPE_MAIN_RANKBATTLE_READY,			// 랭크배틀 진입 신청
	GAME_RULE_TYPE_MAIN_RANKBATTLE_RUN,				// 랭크배틀 진행
	GAME_RULE_TYPE_MAIN_TENKAICHIBUDOKAI_READY,		// 천하제일 무도회 진입 신청
	GAME_RULE_TYPE_MAIN_TENKAICHIBUDOKAI_RUN,		// 천하제일 무도회 진행
	GAME_RULE_TYPE_MAIN_DRAGONBALL,					// 드래곤볼 진행

	// Sub Layer
	GAME_RULE_TYPE_SUB_FREEPVP,						// 자유 PVP 진행
	GAME_RULE_TYPE_SUB_PARTY_INVITE,				// 파티 초대
};


class CNtlGameRuleConcept
{
public:
	typedef std::vector< eGAME_RULE_TYPE > vecdef_RuleList;
	typedef std::map< eGAME_RULE_TYPE, vecdef_RuleList > mapdef_RuleTransTable;

	typedef RwBool (CNtlGameRuleConcept::*QUERY_RULE_TYPE) ( void );
	typedef std::map< eGAME_RULE_TYPE, QUERY_RULE_TYPE > mapdef_RuleQueryTable;


protected:
	mapdef_RuleTransTable m_defRuleTransTable;
	mapdef_RuleQueryTable m_defMainRuleQueryTable;


public:
	static CNtlGameRuleConcept* m_pInstance;
	static CNtlGameRuleConcept* GetInstance(void);


public:
	CNtlGameRuleConcept( void );
	~CNtlGameRuleConcept( void );

	RwBool Create( void );
	void Destroy( void );

	RwBool CanTransition( eGAME_RULE_TYPE eNextRuleType );


protected:
	void MakeRuleTransTable( void );
	void MakeMainRuleQueryTable( void );

	eGAME_RULE_TYPE GetCurRuleType( void );

	RwBool Is_Main_PrivateShopReady( void );
	RwBool Is_Main_PrivateShopRun( void );
	RwBool Is_Main_TMQ_Wait_Lottery( void );
	RwBool Is_Main_TMQReady( void );
	RwBool Is_Main_TMQRun( void );
	RwBool Is_Main_RankBattleReady( void );
	RwBool Is_Main_RankBattleRun( void );
	RwBool Is_Main_TenkaichibudokaiReady( void );
	RwBool Is_Main_TenkaichibudokaiRun( void );
	RwBool Is_Main_DragonBall( void );
	RwBool Is_Sub_FreePvP( void );
	RwBool Is_Sub_PartyInvite( void );

	void OutputMessage( eGAME_RULE_TYPE eCurRuleType, eGAME_RULE_TYPE eNextRuleType );
};


#endif
