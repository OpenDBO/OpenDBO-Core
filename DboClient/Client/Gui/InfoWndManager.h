////////////////////////////////////////////////////////////////////////////////
// File: InfoWndManager.h
// Desc: InfoWnd
//
// 2006.06.17 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __INFOWND_MANAGER_H__
#define __INFOWND_MANAGER_H__

#include "ceventhandler.h"
#include "DialogDefine.h"
#include "NtlBattle.h"
#include "NtlSharedType.h"

class CInfoWindowGui;
struct sITEM_TBLDAT;

struct stINFOWND_REWARD
{
	RwInt32		eType;
	RwInt32		nValue;
};

struct stMACOT_INFO
{
	TBLIDX			tblidx;
	BYTE			byItemRank;
	DWORD			dwCurExp;
	DWORD			dwCurVP;
	DWORD			dwMaxVP;
	DWORD			dwMaxExp;

};

struct stINFOWND_UPGRADE
{
	enum MAXLINE { MAXLINECOUNT = 4 };

	stINFOWND_UPGRADE::stINFOWND_UPGRADE(VOID)
	{
		for (RwInt32 i = 0; i < MAXLINECOUNT; ++i)
		{
			wszText[i] = NULL;
			uiColor[i] = RGB(255, 255, 255);
		}
	}

	WCHAR* wszText[MAXLINECOUNT];
	RwUInt32	uiColor[MAXLINECOUNT];
};

/**
* \brief Battle Attribute�� ���� �Ӽ��� ����ϱ� ���Ͽ�
*/
struct stINFOWND_BATTLEATTR
{
	enum eTYPE
	{
		TYPE_ATTR_WEAPON_INFO = 0,			///< ���� �Ӽ��� ������ �� ����
		TYPE_ATTR_ARMOR_INFO,				///< �� �Ӽ��� ������ �� ����
		TYPE_ATTR_TARGET_ATTACK_RATE,		///< ���� ��󿡰� ����ȿ��
		TYPE_ATTR_TARGET_DEFENCE_RATE,		///< ���� ��󿡰� ���ȿ��
		TYPE_ATTR_TARGET_ALL_RATE,			///< ���� ��󿡰� ����, ��� ȿ��
		TYPE_ATTR_UPGRADE,					///< ���׷��̵�� �� ����
		TYPE_ATTR_NUM,						///< �����Ӽ� ������ Ÿ�� ����
		TYPE_ATTR_INVALID = TYPE_ATTR_NUM
	};

	eTYPE	eBattleAttrInfoType;
	RwUInt8 bySourceWeaponAttr;
	RwUInt8 bySourceArmorAttr;
	RwUInt8 byTargetWeaponAttr;
	RwUInt8 byTargetArmorAttr;
	RwReal	afSourceOffenceBonus[BATTLE_ATTRIBUTE_COUNT];
	RwReal	afSourceDefenceBonus[BATTLE_ATTRIBUTE_COUNT];
	RwReal	afTargetOffenceBonus[BATTLE_ATTRIBUTE_COUNT];
	RwReal	afTargetDefenceBonus[BATTLE_ATTRIBUTE_COUNT];

	stINFOWND_BATTLEATTR::stINFOWND_BATTLEATTR(VOID)
	{
		eBattleAttrInfoType = TYPE_ATTR_INVALID;
		bySourceWeaponAttr = INVALID_BYTE;
		bySourceArmorAttr = INVALID_BYTE;
		byTargetWeaponAttr = INVALID_BYTE;
		byTargetArmorAttr = INVALID_BYTE;

		for (int i = 0; i < BATTLE_ATTRIBUTE_COUNT; ++i)
		{
			afSourceOffenceBonus[i] = 0.0f;
			afSourceDefenceBonus[i] = 0.0f;
			afTargetOffenceBonus[i] = 0.0f;
			afTargetDefenceBonus[i] = 0.0f;
		}
	}
};

struct stINFOWND_QUESTINDICATOR
{
	RwUInt32	uiQuestTitle;
	RwUInt32	uiQuestGoal;
};

class CInfoWndManager : public RWS::CEventHandler
{
private:
	//! Constructor & Destructor
	CInfoWndManager(VOID);
public:
	//! Enum
	enum eINFOWNDSTATE
	{
		INFOWND_NOTSHOW = 0,
		INFOWND_ITEM,							// General items.	
		INFOWND_ITEM_DATA,						// An item of type sITEM_DATA.
		INFOWND_PROFILE_ITEM,					// An item in profile form.	
		INFOWND_TABLE_ITEM,						// Item on the table
		INFOWND_JUST_ITEM_NAME,					// Output only item names
		INFOWND_UNIDENTIFIED_ITEM,
		INFOWND_SKILL,
		INFOWND_NOTLEARN_SKILL,
		INFOWND_SKILLRQ,
		INFOWND_BUFF,
		INFOWND_HTBSKILL,
		INFOWND_NOTLEARN_HTBSKILL,
		INFOWND_JUST_WTEXT,
		INFOWND_ACTIONSKILL,
		INFOWND_QUESTITEM,
		INFOWND_NOTSOBBUFF,
		INFOWND_REWARD,
		INFOWND_FAKEBUFF,						//! ��Ƽ ����
		INFOWND_MINIMAP,
		INFOWND_PORTAL,                         ///< NPC�� �̿��� ��Ż�� ���Ǵ� ���� ����
		INFOWND_FRIEND_LIST,                    ///< ģ������Ʈ���� ���� ������ ǥ���ϴ� ����
		INFOWND_UPGRADE,						// ���׷��̵� ������ ������ ȣ�����̽���.
		INFOWND_BATTLEATTRIBUTE,				///< Battle Attribute�� ���� �Ӽ��� ǥ���ϴ� ����
		INFOWND_QUEST_SEARCH,
		INFOWND_QUEST_INDICATOR,				/// ����Ʈ�˸��� ���콺���� ����.
		INFOWND_SKILL_SPUPGRADE,				/// SP ���׷��̵� ��ư.
		INFOWND_RECIPE,
		INFOWND_HLS_ITEM,

		// Mascot
		INFOWND_MASCOT_INFO,
		INFOWND_MASCOT_SKILL_INFO
	};

	~CInfoWndManager(VOID);
	//! Access
	static RwBool CreateInstance(VOID);
	static VOID   DeleteInstance(VOID);

	static CInfoWndManager* GetInstance(VOID);

	VOID ShowInfoWindow(RwBool bShow, RwInt32 eState = INFOWND_NOTSHOW, RwInt32 nXPos = 0, RwInt32 nYPos = 0, VOID* pInstance = NULL, RwInt32 eRequestGui = DIALOG_UNKNOWN);
	eINFOWNDSTATE GetInfoWndState(VOID) { return m_eWndState; }
	RwInt32 GetRequestGui(VOID) { return m_eRequestGui; }

private:
	//! Implementation
	VOID LinkEvent(VOID);
	VOID UnLinkEvent(VOID);

	//! Event
	VOID HandleEvents(RWS::CMsg& msg);

	//! CallBack

	//! Variable
	CInfoWindowGui* m_pInfoWnd;

	eINFOWNDSTATE	m_eWndState;
	RwInt32			m_eRequestGui;;

	static CInfoWndManager* m_pInstance;
};

static CInfoWndManager* GetInfoWndManager(VOID)
{
	return CInfoWndManager::GetInstance();
}

#endif//__INFOWND_MANAGER_H__