/*****************************************************************************
 *
 * File			: NtlSobGlobal.h
 * Author		: HyungSuk, Jang
 * Copyright	: (��)NTL
 * Date			: 2005. 8. 23	
 * Abstract		: Simulation global type
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SLGLOBAL_H__
#define __NTL_SLGLOBAL_H__


class CNtlSobAvatar;
class CNtlSLPacketGenerator;
class CNtlNetSender;
class CNtlTriggerSyncManager;

#include "NtlCharacter.h"
#include "NtlItem.h"
#include "NtlMail.h"
#include "NtlSkill.h"
#include "NtlSobSkill.h"
#include "NtlWorld.h"
#include "NtlPacketLockManager.h"
#include "NtlQuest.h"
#include "NtlRankBattle.h"
#include "NtlSLTBudokai.h"
#include "NtlTeleport.h"
#include "NtlDojo.h"
#include "NtlMascot.h"

/**
* \brief Avatar Info
*/
typedef struct _SAvatarInfo
{
	RwUInt32			uiSerialId;										// avatar serial id
	sPC_PROFILE			sCharPf;										// avatar profile
	sCHARSTATE			sCharState;										// avatar state
	RwUInt16			wItemCount;									// ��ȿ�� item count	
	sITEM_PROFILE		sItemPf[NTL_MAX_COUNT_USER_HAVE_INVEN_ITEM];	// item profile
	RwUInt8				bySkillCount;									// ��ȿ�� skill count
	sSKILL_INFO			sSkillPf[NTL_MAX_PC_HAVE_SKILL];				// skill profile
	RwUInt8				byHTBCount;										// ��ȿ�� HTB count
	sHTB_SKILL_INFO		sHTBPf[NTL_HTB_MAX_PC_HAVE_HTB_SKILL];			// HTB profile
	RwUInt8				byBuffCount;
	sBUFF_INFO			sBuffPf[DBO_MAX_BUFF_CHARACTER_HAS];
	sWORLD_INFO			sOldWorldInfo;
	sWORLD_INFO			sWorldInfo;
	sQUEST_COMPLETE_INFO sQuestCompleteInfo;								// ����Ʈ �Ϸ� ����
	RwUInt8				byQuestProgressInfoCount;							// ����Ʈ ���� ���� ����
	sQUEST_PROGRESS_INFO sQuestProgressInfo[eMAX_CAN_PROGRESS_QUEST_NUM];	// ����Ʈ ���� ����
	sQUEST_INVENTORY_FULLDATA sQuestInventoryInfo;							// ����Ʈ �κ��丮 ����
	RwUInt8				bySetQuickSlotInfo;									// ������ ������ ������ TRUE;
	RwUInt8				byQuickSlotInfoCount;								// ������ ���� ����
	sQUICK_SLOT_PROFILE	sQuickSlotInfo[NTL_CHAR_QUICK_SLOT_MAX_COUNT];		// ������ ����
    RwUInt8             byMaxRpStock;                                       ///< �ִ� RP Stock ����
	
	RwUInt8				byMailCount;										// ���� ����
	RwUInt8				byUnreadMailCountNormal;							// �� ���� ���� : �Ϲ� ����
	RwUInt8				byUnreadMailCountManager;							// �� ���� ���� : �Ŵ��� ����

	char				acWarFogFlag[NTL_MAX_SIZE_WAR_FOG];					// ������ ����

	char				abyTitleIndexFlag[NTL_MAX_CHAR_TITLE_COUNT_IN_FLAG];

	sRANKBATTLE_SCORE_INFO sRankBattleScoreInfo;							// ��ũ��Ʋ ���� ����

	RwUInt8				byDojoBriefCount;
	sDBO_DOJO_BRIEF		sDojoBrief[DBO_MAX_COUNT_DOJO_IN_WORLD];

	PORTALID			aPortalID[NTL_PORTAL_MAX_COUNT];	// portals we have registered

	bool				bCombatMode;

}SAvatarInfo;

/**
* \brief ��ȯ���� ����
*/
typedef struct _SSummonPetInfo
{
	RwUInt32				uiSerialId;			// avatar summon pet serial id
	sSUMMON_PET_PROFILE		sCharPf;			// avatar summon pet profile
	sCHARSTATE				sCharState;			// avatar summon pet character state
}SSummonPetInfo;

/**
* \brief ��ȯ�� ��ų�� ����
*/
typedef struct _SSummonPetSkillInfo
{
	RwUInt8				bySkillCount;						// avatar summon pet skill count
	sSKILL_INFO_PET		aSkillInfo[NTL_MAX_NPC_HAVE_SKILL];	// avatar summon pet skill info
}SSummonPetSkillInfo;

/**
* \brief õ������ ����ȸ ���¿� ���� ����
*/
typedef struct _STenkaichiBudokaiInfo
{
	sBUDOKAI_UPDATE_STATE_INFO			sStateInfo;			///< õ������ ����ȸ ���� ������Ʈ
	sBUDOKAI_UPDATE_MATCH_STATE_INFO	sMatchStateInfo[MAX_BUDOKAI_STATE];	///< ��ġ ����
	sBUDOKAI_JOIN_INFO					sJoinInfo;							///< �÷��̾��� ��û ����
} STenkaichiBudokaiInfo;

// Mascot
typedef struct _SMascotInfo
{
	RwUInt16			wCount;
	sMASCOT_DATA_EX		asMascotData[GMT_MAX_PC_HAVE_MASCOT];
}SMascotInfo;

/**
* \brief	!! AdjustMoney !!
*			������ Ŭ���̾�Ʈ ���� ���� ��� Ÿ�̹��� �����ֱ� ���Ͽ� ����. (��Ƽ��,���� ��..)
*/
struct SAdjustZennyInfo
{
	enum ZENNYADJUSTTYPE { USER_TRADE, PARTY_CHARM, MAIL_BOX, PARTY_INVEN, NUM_TYPE };
	
	RwInt32	m_aiAdjustZenny[NUM_TYPE];	

	void	Init(void);
	void	SetAdjustZenny(RwInt32 eType, RwInt32 nValue);	
	RwInt32	GetAdjustZenny(RwInt32 eType);
};

struct SAvatarRecipeInfo
{
	RwUInt16			wCount;
	sRECIPE_DATA		asRecipeData[NTL_ITEM_RECIPE_MAX_COUNT];
};

/**
* \brief	!! PassiveSkillEffect !!
*			�нú� ��ų�� ���� ��ų ��ȭ�� ����. 
*/
struct SPassiveSkillEffect
{
	RwUInt32				SkillTblidx;
	RwUInt8					bySlotidx;
	sDBO_UPDATED_SKILL_DATA	sUpdatedSkillData;
};

typedef std::list<SPassiveSkillEffect>				LIST_PASSIVE_SKILL_EFFECT;
typedef std::list<SPassiveSkillEffect>::iterator	LIST_PASSIVE_SKILL_EFFECT_ITER;	

/**
* \brief	GameManiaTime
* GMT ������ ����
*/
struct SGameManiaTimeInfo
{
	RwUInt8 byGMTResetCount;
	sGAME_MANIA_TIME_DATA sCurrent;
	sGAME_MANIA_TIME_DATA sNext;
};

/**
* \ingroup Simulation
* \brief Simulation layer���� �����Ӱ� ������ �� �ִ� ���� �ڷᱸ��
*/
class CNtlSLGlobal
{
private:

	static CNtlSLGlobal		*m_pInstance;
	
	SAvatarInfo				m_sAvatarInfo;						///< ���� ���Խ� avatar ������ ��� �ڷᱸ��
	STenkaichiBudokaiInfo	m_sTenkaichiBudokaiInfo;			///< ���� ���Խ� õ������ ����ȸ ������ ��� �ڷᱸ��						
	SSummonPetInfo			m_sSummonPetInfo;					///< avatar�� summon pet ��ȯ�� �����ִ� ����
	SSummonPetSkillInfo		m_sSummonPetSkillInfo;				///< avatar�� summon pet ��ȯ�� �����ִ� ���� (skill)
	SAdjustZennyInfo		m_sAdjustZennyInfo;					///< ����,Ŭ���̾�Ʈ�� ���ϻ��Ÿ�̹��� �ٸ����� ���� ����� ���� ����.
	SAvatarRecipeInfo		m_sAvatarRecipeInfo;				///< ���� ���Խ� �ƹ�Ÿ�� ������ ��ƼŸ�� ��� �ڷᱸ��
	SGameManiaTimeInfo		m_sGameManiaTimeInfo;
	LIST_PASSIVE_SKILL_EFFECT m_listPassveSkillEffectInfo;		///< �ƹ�Ÿ ���� ������ ������� ��ų�� �нú�����Ʈ ���� ����.

	CNtlSobAvatar			*m_pSobAvatar;						///< avatar ��ü instance pointer
	RwBool					m_bWorldUpdateAvaterPos;
	DBOTIME					m_ServerTimeOffset;					///< ( ����Ÿ�� - Ŭ���̾�Ʈ ), Ŭ���̾�Ʈ Ÿ�ӿ� ���ؼ� ���.

	HOBJECT					m_hNpcFacingHandle;

	// Mascot
	SMascotInfo				m_sMascotInfo;

public:

	static bool				m_bSelectSkillMode;
	static SERIAL_HANDLE	m_hSelectSkillModeHandle;
	
public:

	CNtlSLGlobal();
	~CNtlSLGlobal();

	static CNtlSLGlobal* GetInstance(void);

	RwBool Create(RwBool bVirtualNet);
	void Destroy(void);

	void			Reset(void);
	void			ResetTenkaichiBudokaiInfo(void);
	
	SAvatarInfo*	GetAvatarInfo(void);

	SSummonPetInfo*			GetAvatarSummonPetInfo(void);
	SSummonPetSkillInfo*	GetAvatarSummonPetSkillInfo(void);
	STenkaichiBudokaiInfo*	GetTBudokaiStateInfo(void);
	SAdjustZennyInfo*		GetAdjustZennyInfo(void);
	SAvatarRecipeInfo*		GetAvatarRecipeInfo(void);
	SGameManiaTimeInfo*		GetGameManiaTimeInfo(void);
	LIST_PASSIVE_SKILL_EFFECT* GetPassiveSkillEffectInfo(void);
	
	void			SetSobAvatar(CNtlSobAvatar *pSobAvatar);
	CNtlSobAvatar*	GetSobAvatar(void);

	bool			IsSkillSelectMode();
	SERIAL_HANDLE	GetSkillSelectModeHandle();
	
	void			SetWorldUpdateAvatarPosition(RwBool bAvatarPos);
	RwBool			IsWorldUpdateAvaterPosition(void);

	void			SetServerTime( DBOTIME serverTime );
	DBOTIME			GetServerTime(void);

	void			SetNpcFacingHandle(HOBJECT hHandle) { m_hNpcFacingHandle = hHandle; }
	HOBJECT			GetNpcFacingHandle() { return m_hNpcFacingHandle; }

	// Mascot
	SMascotInfo* GetAvatarMascotInfo(void);
};

static CNtlSLGlobal* GetNtlSLGlobal(void)
{
	return CNtlSLGlobal::GetInstance(); 
}

// Inline ----------
inline SAvatarInfo* CNtlSLGlobal::GetAvatarInfo(void)
{
	return &m_sAvatarInfo;
}

inline SSummonPetInfo* CNtlSLGlobal::GetAvatarSummonPetInfo(void)
{
	return &m_sSummonPetInfo;
}

inline SSummonPetSkillInfo* CNtlSLGlobal::GetAvatarSummonPetSkillInfo(void)
{
	return &m_sSummonPetSkillInfo;
}

inline SAvatarRecipeInfo* CNtlSLGlobal::GetAvatarRecipeInfo(void)
{
	return &m_sAvatarRecipeInfo;
}

inline void CNtlSLGlobal::SetSobAvatar(CNtlSobAvatar *pSobAvatar)
{
	m_pSobAvatar = pSobAvatar;
}

inline void	CNtlSLGlobal::SetWorldUpdateAvatarPosition(RwBool bAvatarPos)
{
	m_bWorldUpdateAvaterPos = bAvatarPos;
}

inline RwBool CNtlSLGlobal::IsWorldUpdateAvaterPosition(void)
{
	return m_bWorldUpdateAvaterPos;
}

inline STenkaichiBudokaiInfo* CNtlSLGlobal::GetTBudokaiStateInfo( void )
{
	return &m_sTenkaichiBudokaiInfo;
}

inline SAdjustZennyInfo* CNtlSLGlobal::GetAdjustZennyInfo( void )
{
	return &m_sAdjustZennyInfo;
}

inline LIST_PASSIVE_SKILL_EFFECT* CNtlSLGlobal::GetPassiveSkillEffectInfo( void )
{
	return &m_listPassveSkillEffectInfo;
}

inline SGameManiaTimeInfo* CNtlSLGlobal::GetGameManiaTimeInfo( void )
{
	return &m_sGameManiaTimeInfo;
}

inline void CNtlSLGlobal::SetServerTime( DBOTIME serverTime )
{
	m_ServerTimeOffset = serverTime - time(NULL);
}

inline DBOTIME CNtlSLGlobal::GetServerTime(void)
{
	return time(NULL) + m_ServerTimeOffset;
}

// Mascot
inline SMascotInfo* CNtlSLGlobal::GetAvatarMascotInfo(void)
{
	return &m_sMascotInfo;
}

#endif