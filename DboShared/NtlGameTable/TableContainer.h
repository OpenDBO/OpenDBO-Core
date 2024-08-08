//***********************************************************************************
//
//	File		:	TableContainer.h
//
//	Begin		:	2007-01-31
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "Table.h"

#include "NtlSharedType.h"

#include <map>

class CNtlFileSerializer;
class CNtlBitFlagManager;

class CExpTable;
class CHelpTable;
class CMerchantTable;
class CMobTable;
class CNewbieTable;
class CNPCTable;
class CPCTable;
class CStatusTransformTable;
class CGameManiaTimeTable;

class CBasicDropTable;
class CItemOptionTable;
class CItemTable;
class CNormalDropTable;
class CLegendaryDropTable;
class CExcellentDropTable;
class CSuperiorDropTable;
class CEachDropTable;
class CTypeDropTable;
class CUseItemTable;
class CDragonBallTable;
class CDragonBallRewardTable;

class CActionTable;
class CChatCommandTable;
class CDirectionLinkTable;
class CFormulaTable;

class CCharmTable;

class CQuestDropTable;
class CQuestItemTable;
class CQuestProbabilityTable;
class CQuestTextDataTable;
class CQuestRewardTable;

class CHTBSetTable;
class CSkillTable;
class CSystemEffectTable;

class CTextAllTable;
class CChattingFilterTable;
class CTextServerTable;

class CObjectTable;
class CSpawnTable;
class CWayPointTable;
class CWorldTable;
class CWorldZoneTable;
class CWorldPlayTable;
class CWorldMapTable;
class CLandMarkTable;
class CModelToolCharDataTable;
class CModelToolObjDataTable;
class CWorldPathTable;

class CTimeQuestTable;
class CRankBattleTable;
class CBudokaiTable;
class CDungeonTable;

class CTableFileNameList;

class CGuideHintTable;
class CPortalTable;
class CNpcSpeechTable;
class CSetItemTable;
class CScriptLinkTable;
class CQuestNarrationTable;

class CVehicleTable;
class CItemRecipeTable;

class CDynamicObjectTable;
class CMobMovePatternTable;
class CDojoTable;
class CItemUpgradeTable;
class CItemMixMachineTable;

class CHLSItemTable;
class CHLSMerchantTable;

class CTableContainer
{
public:
	enum eTABLE
	{
		// Character
		TABLE_EXP = 0,
		TABLE_HELP,
		TABLE_MERCHANT,
		TABLE_MOB,
		TABLE_NEWBIE,
		TABLE_NPC,
		TABLE_PC,
		TABLE_STATUS_TRANSFORM,
		TABLE_GAME_MANIA_TIME,
		TABLE_GUIDE_HINT,

		// Item
		TABLE_BASIC_DROP,
		TABLE_ITEM_OPTION,
		TABLE_ITEM,
		TABLE_LEGENDARY_DROP,
		TABLE_NORMAL_DROP,
		TABLE_SUPERIOR_DROP,
		TABLE_USE_ITEM,
		TABLE_SET_ITEM,
		TABLE_DRAGONBALL,
		TABLE_DRAGONBALL_REWARD,

		// Misc
		TABLE_ACTION,
		TABLE_CHAT_COMMAND,
		TABLE_DIRECTION_LINK,
		TABLE_FORMULA,

		// Party
		TABLE_CHARM,

		// Quest
		TABLE_QUEST_DROP,
		TABLE_QUEST_ITEM,
		TABLE_QUEST_PROBABILITY,
		TABLE_QUEST_TEXT_DATA,
		TABLE_QUEST_REWARD,

		// Rule
		TABLE_RANKBATTLE,
		TABLE_TIMEQUEST,
		TABLE_BUDOKAI,

		// Skill
		TABLE_HTB_SET,
		TABLE_SKILL,
		TABLE_SYSTEM_EFFECT,

		// Text
		TABLE_TEXT_ALL,
		TABLE_CHATTING_FILTER,
		TABLE_TEXT_SERVER,

		// World
		TABLE_LAND_MARK,
		TABLE_OBJECT,
		TABLE_NPC_SPAWN,
		TABLE_MOB_SPAWN,
		TABLE_WORLD_MAP,
		TABLE_WORLD,
		TABLE_WORLD_ZONE,
		TABLE_WORLD_PLAY,

		// GraphicData
		TABLE_MODEL_TOOL_CHAR,
		TABLE_MODEL_TOOL_OBJ,

		// World Data
		TABLE_PASS_POINT,

		// Portal Data
		TABLE_PORTAL,

		TABLE_SPEECH,

		TABLE_EACH_DROP,
		TABLE_TYPE_DROP,

		TABLE_SCRIPT_LINK,
		TABLE_EXCELLENT_DROP,
		TABLE_DUNGEON,
		
		TABLE_QUEST_NARRATION,

		TABLE_VEHICLE,
		TABLE_ITEM_RECIPE,
		TABLE_DYNAMIC_OBJECT,
		TABLE_MOB_MOVE_PATTERN,
		TABLE_DOJO,
		TABLE_ITEM_UPGRADE,
		TABLE_MIX_MACHINE,
		
		TABLE_HLS_MERCHANT,
		TABLE_HLS_ITEM,

		TABLE_COUNT
	};

public:
	CTableContainer(void);

	virtual ~CTableContainer(void);

public:

	typedef std::map<TBLIDX, CSpawnTable*> SPAWNTABLEMAP;
	typedef SPAWNTABLEMAP::iterator SPAWNTABLEIT;
	typedef SPAWNTABLEMAP::value_type SPAWNTABLEVAL;

	typedef std::map<TBLIDX, CObjectTable*> OBJTABLEMAP;
	typedef OBJTABLEMAP::iterator OBJTABLEIT;
	typedef OBJTABLEMAP::value_type OBJTABLEVAL;


public:

	class ICallBack
	{
	public:
		virtual bool Call( const char* pfilename, CNtlFileSerializer* pSeralize, const char* pszCryptPassword ) = 0;
	};

	// Create 사용 시 pCall 이 NULL 일 경우 이전의 방식과 동일하게 동작한다. pCall -> Client 사용용 
	bool							Create(CNtlBitFlagManager& rTableFlag, char* pszPath, CTableFileNameList* pFileNameList, CTable::eLOADING_METHOD eLoadingMethod, DWORD dwCodePage, ICallBack* pCall );	

	bool							Create(CNtlBitFlagManager& rTableFlag, WCHAR* pwszPath, CTableFileNameList* pFileNameList, CTable::eLOADING_METHOD eLoadingMethod, DWORD dwCodePage, ICallBack* pCall );

	void							Destroy();


protected:

	void							Init();


public:

	bool							Reload(CNtlBitFlagManager& rTableFlag, CTableFileNameList* pFileNameList);

	bool							SaveToFile(CNtlBitFlagManager& rTableFlag, CTableFileNameList* pFileNameList, bool bNeedToEncrypt);

	void							SetPath(char* pszPath);

	void							SetPath(WCHAR* pwszPath) { m_wstrPath = pwszPath; }

protected:

	bool							InitializeTable(CTable* pTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializeTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializeTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializeTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializePackTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializePackTable(CTable* pTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializePackTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, WCHAR* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							InitializePackTable(CTextAllTable* pTextAllTable, CNtlFileSerializer& serializer, char* pszFileNameWithoutExtension, ICallBack* pCall);

	bool							ReloadTable(CTable* pTable, CNtlFileSerializer& serializer, char* pwszFileNameWithoutExtension);

	bool							ReloadTable(CTable* pTable, CNtlFileSerializer& serializer, WCHAR* pwszFileNameWithoutExtension);


public:

// Character
	CExpTable*						GetExpTable() { return m_pExpTable; }

	CGuideHintTable*				GetGuideHintTable() { return m_pGuideHintTable; }

	CHelpTable*						GetHelpTable() { return m_pHelpTable; }

	CMerchantTable*					GetMerchantTable() { return m_pMerchantTable; }

	CMobTable*						GetMobTable() { return m_pMobTable; }

	CNewbieTable*					GetNewbieTable() { return m_pNewbieTable; }

	CNPCTable*						GetNpcTable() { return m_pNpcTable; }

	CPCTable*						GetPcTable() { return m_pPcTable; }

	CStatusTransformTable*			GetStatusTransformTable() { return m_pStatusTransformTable; }

	CGameManiaTimeTable*			GetGameManiaTimeTable() { return m_pGameManiaTimeTable; }


// Item
	CBasicDropTable*				GetBasicDropTable() { return m_pBasicDropTable; }

	CDragonBallRewardTable*			GetDragonBallRewardTable() { return m_pDragonBallRewardTable; }

	CDragonBallTable*				GetDragonBallTable() { return m_pDragonBallTable; }

	CItemOptionTable*				GetItemOptionTable() { return m_pItemOptionTable; }

	CItemTable*						GetItemTable() { return m_pItemTable; }

	CLegendaryDropTable*			GetLegendaryDropTable() { return m_pLegendaryDropTable; }

	CNormalDropTable*				GetNormalDropTable() { return m_pNormalDropTable; }

	CSuperiorDropTable*				GetSuperiorDropTable() { return m_pSuperiorDropTable; }

	CUseItemTable*					GetUseItemTable() { return m_pUseItemTable; }

	CSetItemTable*					GetSetItemTable() { return m_pSetItemTable; }

	CEachDropTable*					GetEachDropTable() { return m_pEachDropTable; }

	CTypeDropTable*					GetTypeDropTable() { return m_pTypeDropTable; }
	
	CExcellentDropTable*			GetExcellentDropTable() { return m_pExcellentDropTable; }

// Misc
	CActionTable*					GetActionTable() { return m_pActionTable; }

	CChatCommandTable*				GetChatCommandTable() { return m_pChatCommandTable; }

	CDirectionLinkTable *			GetDirectionLinkTable() { return m_pDirectionLinkTable; }

	CFormulaTable*					GetFormulaTable() { return m_pFormulaTable; }

// Party
	CCharmTable*					GetCharmTable() { return m_pCharmTable; }

// Quest
	CQuestDropTable*				GetQuestDropTable() { return m_pQuestDropTable; }

	CQuestItemTable*				GetQuestItemTable() { return m_pQuestItemTable; }

	CQuestProbabilityTable*			GetQuestProbabilityTable() { return m_pQuestProbalityTable; }

	CQuestTextDataTable*			GetQuestTextDataTable() { return m_pQuestTextDataTable; }

	CQuestRewardTable*				GetQuestRewardTable() { return m_pQuestRewardTable; }

// Rule
	CRankBattleTable *				GetRankBattleTable() { return m_pRankBattleTable; }

	CTimeQuestTable *				GetTimeQuestTable() { return m_pTimeQuestTable; }

	CBudokaiTable *					GetBudokaiTable() { return m_pBudokaiTable;}

	CDungeonTable *					GetDungeonTable() { return m_pDungeonTable; }
// Skill
	CHTBSetTable*					GetHTBSetTable() { return m_pHTBSetTable; }

	CSkillTable*					GetSkillTable() { return m_pSkillTable; }

	CSystemEffectTable*				GetSystemEffectTable() { return m_pSystemEffectTable; }

// Text
	CTextAllTable*					GetTextAllTable() { return m_pTextAllTable; }
	CChattingFilterTable*			GetChattingFilterTable() { return m_pChattingFilterTable; }
	CTextServerTable*				GetTextServerTable() { return m_pTextServerTable; }

// World
	CLandMarkTable*					GetLandMarkTable() { return m_pLandMarkTable; } 

	CObjectTable*					GetObjectTable(TBLIDX worldTblidx);

	CSpawnTable*					GetNpcSpawnTable(TBLIDX worldTblidx);

	CSpawnTable*					GetMobSpawnTable(TBLIDX worldTblidx);

	CWorldMapTable*					GetWorldMapTable() { return m_pWorldMapTable; }

	CWorldTable*					GetWorldTable() { return m_pWorldTable; }

	CWorldZoneTable*				GetWorldZoneTable() { return m_pWorldZoneTable; }

// Graphic Data
	CModelToolCharDataTable *		GetModelToolCharDataTable() { return m_pCharDataAnimTable; }

	CModelToolObjDataTable *		GetModelToolObjDataTable() { return m_pObjDataAnimTable; }

// World Data
	CWorldPathTable*				GetWorldPathTable() { return m_pWorldPathTable; }

	CWorldPlayTable*				GetWorldPlayTable() { return m_pWorldPlayTable; }

// Portal Data
	CPortalTable*					GetPortalTable() { return m_pPortalTable; }

	CNpcSpeechTable*				GetNpcSpeechTable() { return m_pNpcSpeechTable; } 
	
	CScriptLinkTable*				GetScriptLinkTable() { return m_pScriptLinkTable; }

	CQuestNarrationTable*			GetQuestNarrationTable() { return m_pQuestNarrationTable; }

	CVehicleTable*					GetVehicleTable() { return m_pVehicleTable; }

	CItemRecipeTable*				GetItemRecipeTable() { return m_pItemRecipeTable; }

	CDynamicObjectTable*			GetDynamicObjectTable() { return m_pDynamicObjectTable; }

	CMobMovePatternTable*			GetMobMovePatternTable() { return m_pMobMovePatternTable; }

	CDojoTable*						GetDojoTable() { return m_pDojoTable; }

	CItemUpgradeTable*				GetItemUpgradeTable() { return m_pItemUpgradeTable; }

	CItemMixMachineTable*			GetItemMixMachineTable() { return m_pItemMixMachineTable; }

	CHLSItemTable*					GetHLSItemTable() { return m_pHLSItemTable; }
	
	CHLSMerchantTable*				GetHLSMerchantTable() { return m_pHLSMerchantTable; }


public:

	OBJTABLEIT						BeginObjectTable();

	OBJTABLEIT						EndObjectTable();

	SPAWNTABLEIT					BeginNpcSpawnTable();

	SPAWNTABLEIT					EndNpcSpawnTable();

	SPAWNTABLEIT					BeginMobSpawnTable();

	SPAWNTABLEIT					EndMobSpawnTable();


protected:

	std::wstring					m_wstrPath;

	CTable::eLOADING_METHOD			m_eLoadingMethod;

	DWORD							m_dwCodePage;

// Character
	CExpTable*						m_pExpTable;

	CGuideHintTable*				m_pGuideHintTable;

	CHelpTable *					m_pHelpTable;

	CMerchantTable*					m_pMerchantTable;

	CMobTable*						m_pMobTable;

	CNewbieTable*					m_pNewbieTable;

	CNPCTable*						m_pNpcTable;

	CPCTable*						m_pPcTable;

	CStatusTransformTable*			m_pStatusTransformTable;

	CGameManiaTimeTable*			m_pGameManiaTimeTable;


// Item
	CBasicDropTable*				m_pBasicDropTable;

	CDragonBallRewardTable*			m_pDragonBallRewardTable;

	CDragonBallTable*				m_pDragonBallTable;

	CItemOptionTable*				m_pItemOptionTable;

	CItemTable*						m_pItemTable;

	CLegendaryDropTable*			m_pLegendaryDropTable;

	CNormalDropTable*				m_pNormalDropTable;

	CSuperiorDropTable*				m_pSuperiorDropTable;

	CUseItemTable*					m_pUseItemTable;

	CSetItemTable*					m_pSetItemTable;

	CEachDropTable*					m_pEachDropTable;

	CTypeDropTable*					m_pTypeDropTable;

	CExcellentDropTable*			m_pExcellentDropTable;
// Misc
	CActionTable*					m_pActionTable;

	CChatCommandTable*				m_pChatCommandTable;

	CDirectionLinkTable *			m_pDirectionLinkTable;

	CFormulaTable*					m_pFormulaTable;

// Party
	CCharmTable*					m_pCharmTable;

// Quest
	CQuestDropTable*				m_pQuestDropTable;		// 퀘스트 드롭 아이템 테이블

	CQuestItemTable*				m_pQuestItemTable;		// 퀘스트 아이템 테이블

	CQuestProbabilityTable*			m_pQuestProbalityTable;

	CQuestTextDataTable*			m_pQuestTextDataTable;

	CQuestRewardTable*				m_pQuestRewardTable;

// Rule
	CRankBattleTable *				m_pRankBattleTable;

	CTimeQuestTable *				m_pTimeQuestTable;

	CBudokaiTable *					m_pBudokaiTable;

	CDungeonTable *					m_pDungeonTable;
// Skill
	CHTBSetTable*					m_pHTBSetTable;

	CSkillTable*					m_pSkillTable;

	CSystemEffectTable*				m_pSystemEffectTable;

// Text
	CTextAllTable*					m_pTextAllTable;
	CChattingFilterTable*			m_pChattingFilterTable;
	CTextServerTable*				m_pTextServerTable;

// World
	CLandMarkTable*					m_pLandMarkTable;

	OBJTABLEMAP						m_mapObjectTable;

	SPAWNTABLEMAP					m_mapNpcSpawnTable;

	SPAWNTABLEMAP					m_mapMobSpawnTable;

	CWorldMapTable*					m_pWorldMapTable;

	CWorldTable*					m_pWorldTable;

	CWorldZoneTable*				m_pWorldZoneTable;

	CWorldPlayTable*				m_pWorldPlayTable;

// Graphic Data
	CModelToolCharDataTable	*		m_pCharDataAnimTable;

	CModelToolObjDataTable *		m_pObjDataAnimTable;

// World Data
	CWorldPathTable*				m_pWorldPathTable;

// Portal Data
	CPortalTable*					m_pPortalTable;

	CNpcSpeechTable*				m_pNpcSpeechTable;	

	CScriptLinkTable*				m_pScriptLinkTable;

	CQuestNarrationTable*			m_pQuestNarrationTable;

	CVehicleTable*					m_pVehicleTable; 

	CItemRecipeTable*				m_pItemRecipeTable;

	CDynamicObjectTable*			m_pDynamicObjectTable;

	CMobMovePatternTable*			m_pMobMovePatternTable;

	CDojoTable*						m_pDojoTable;

	CItemUpgradeTable*				m_pItemUpgradeTable;

	CItemMixMachineTable*			m_pItemMixMachineTable;

	CHLSItemTable*					m_pHLSItemTable; 
	
	CHLSMerchantTable*				m_pHLSMerchantTable; 
};