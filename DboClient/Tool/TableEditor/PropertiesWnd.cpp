#include "pch.h"
#include "framework.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "TableEditor.h"

#include "Util.h"
#include "ItemTable.h"
#include "NewbieTable.h"
#include "NpcSpeechTable.h"
#include "TextAllTable.h"
#include "MascotTable.h"
#include "MascotGradeTable.h"
#include "MascotStatusTable.h"
#include "CharTitleTable.h"
#include "ChattingFilterTable.h"
#include "ItemEnchantTable.h"
#include "MerchantTable.h"
#include "FormulaTable.h"
#include "ItemMixExpTable.h"
#include "ChatCommandTable.h"
#include "HTBSetTable.h"
#include "DirectionLinkTable.h"
#include "DojoTable.h"
#include "VehicleTable.h"
#include "DungeonTable.h"
#include "MobMovePatternTable.h"
#include "DragonBallRewardTable.h"
#include "ExpTable.h"
#include "LandMarkTable.h"
#include "CharmTable.h"
#include "ActionTable.h"
#include "AirCostumeTable.h"
#include "QuestTextDataTable.h"
#include "HelpTable.h"
#include "GuideHintTable.h"
#include "TimeQuestTable.h"
#include "BudokaiTable.h"
#include "RankBattleTable.h"
#include "ScriptLinkTable.h"
#include "QuestNarrationTable.h"
#include "DynamicObjectTable.h"
#include "UseItemTable.h"
#include "SetItemTable.h"
#include "QuestItemTable.h"
#include "ItemDisassembleTable.h"
#include "PortalTable.h"
#include "ItemRecipeTable.h"
#include "ItemMixMachineTable.h"
#include "DragonBallTable.h"
#include "QuestRewardTable.h"
#include "QuestRewardSelectTable.h"
#include "WorldZoneTable.h"
#include "CommonConfigTable.h"
#include "DwcTable.h"
#include "NpcServerTable.h"
#include "WorldTable.h"
#include "WorldMapTable.h"
#include "SystemEffectTable.h"
#include "ItemOptionTable.h"
#include "SkillTable.h"
#include "PCTable.h"
#include "MobTable.h"
#include "NPCTable.h"
#include "HLSItemTable.h"
#include "StatusTransformTable.h"
#include "DwcMissionTable.h"
#include "QuestDropTable.h"
#include "QuestProbabilityTable.h"
#include "WorldPlayTable.h"
#include "SlotMachineTable.h"
#include "HlsSlotMachineItemTable.h"
#include "ItemUpgradeRateNewTable.h"
#include "ItemBagListTable.h"
#include "ItemGroupListTable.h"
#include "MobServerTable.h"
#include "DragonBallReturnPointTable.h"
#include "EventSystemTable.h"
#include "DynamicFieldSystemTable.h"
#include "SpawnTable.h"
#include "ObjectTable.h"
#include "Theme.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// z is a string literal at most call sites but a CString/(LPCTSTR)
// expression at others (grouped array fields use a shared "strGroup"
// CString) -- _T() only does the right thing on a literal token, so both
// x and z go through CString's own converting constructor instead, which
// handles a literal, a CString, or an LPCTSTR uniformly and correctly
// regardless of the project's character set.
#define ADD_SUB_ITEM(x,y,z) pGroup1->AddSubItem(new CMFCPropertyGridProperty(CString(x), (_variant_t)y, CString(z)))

// sOBJECT_TBLDAT's abyState is a 10x10 array (10 states x 10 animation
// slots) -- one ADD_SUB_ITEM per slot would be 100 rows for this one field
// alone, so each state's 10 values are joined into a single semicolon-
// separated row instead (matches how per-row-array data reads elsewhere in
// this tool, e.g. TextAll's combined XML export).
static CString JoinBytes(const BYTE* pValues, int nCount)
{
	CString strResult;
	for (int i = 0; i < nCount; ++i)
	{
		if (i > 0)
		{
			strResult += _T(";");
		}
		CString strPart;
		strPart.Format(_T("%u"), (unsigned)pValues[i]);
		strResult += strPart;
	}
	return strResult;
}

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	m_nTableType = -1;
	m_pTbldat = nullptr;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BOOL CPropertiesWnd::Create(CWnd* pParentWnd, UINT nID)
{
	static CString strClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
		::LoadCursor(nullptr, IDC_ARROW), (HBRUSH)nullptr, nullptr);

	return CWnd::Create(strClass, _T(""), WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), pParentWnd, nID);
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_BN_CLICKED(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_BN_CLICKED(ID_SORTPROPERTIES, OnSortProperties)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers

BOOL CPropertiesWnd::OnEraseBkgnd(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(rectClient);
	pDC->FillSolidRect(rectClient, Theme::Bg2);
	return TRUE;
}

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd () == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	const int cyTlb = 30;
	const int cyMargin = 4;
	const int cxBtn = 90;
	const int cxGap = 4;

	int x = rectClient.left + cyMargin;
	m_btnExpandAll.SetWindowPos(nullptr, x, cyMargin, cxBtn, cyTlb - cyMargin * 2, SWP_NOACTIVATE | SWP_NOZORDER);
	x += cxBtn + cxGap;
	m_btnSortAZ.SetWindowPos(nullptr, x, cyMargin, cxBtn, cyTlb - cyMargin * 2, SWP_NOACTIVATE | SWP_NOZORDER);

	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + cyTlb, rectClient.Width(), rectClient.Height() - cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CPropertiesWnd::LoadTableData(int nTableType, sTBLDAT* pTbldat)
{
	m_wndPropList.RemoveAll();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("TBLDAT"));

	// All 28 CTextAllTable categories share one row shape (sTEXT_TBLDAT:
	// tblidx + wstrText) -- one generic case for all of them, same as any
	// other single-shape table, rather than 28 near-identical switch cases.
	// ClassView's tree passes the real TABLE_TEXT_ALL for every row under
	// that tab -- it doesn't track which of the 28 categories a row came
	// from, since they're all this same shape and don't need to be told apart
	// here.
	if (nTableType == CTableContainer::TABLE_TEXT_ALL)
	{
		sTEXT_TBLDAT* pTableData = (sTEXT_TBLDAT*)pTbldat;

		ADD_SUB_ITEM("tblidx",		pTableData->tblidx,			"Text Table");
		ADD_SUB_ITEM("wstrText",	pTableData->wstrText.c_str(),	"Text Table");
	}
	else
	switch (nTableType)
	{
		case CTableContainer::TABLE_ITEM:
		{
			sITEM_TBLDAT* pTableData = (sITEM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("TBLIDX",						pTableData->tblidx,						 "Item Table Index");
			ADD_SUB_ITEM("bValidity_Able",				pTableData->bValidity_Able,				 "Set item active/inactive");
			CString wszNameDesc = _T("Name Table Index");
			CTextAllTable* pTextAllTable = GetTableContainer()->GetTextAllTable();
			if (pTextAllTable)
			{
				CTextTable* pItemTextTable = pTextAllTable->GetItemTbl(); // item name translations
				if (pItemTextTable)
				{
					std::wstring wstrTranslated;
					if (pItemTextTable->GetText(pTableData->Name, &wstrTranslated))
					{
						wszNameDesc += _T("\nTranslated: ");
						wszNameDesc += wstrTranslated.c_str();
					}
					else
					{
						wszNameDesc += _T("\nTranslation not found.");
					}
				}
			}
			ADD_SUB_ITEM("Name", pTableData->Name, wszNameDesc);
			ADD_SUB_ITEM("wszNameText", pTableData->wszNameText, "Internal Item Name (Max 32 chars)");
            ADD_SUB_ITEM("szIcon_Name",					pTableData->szIcon_Name,				 "Icon Name");
			ADD_SUB_ITEM("byModel_Type",				(int)pTableData->byModel_Type,			 "Model Type");
			ADD_SUB_ITEM("szModel",						pTableData->szModel,					 "Model Name");
			ADD_SUB_ITEM("szSub_Weapon_Act_Model",		pTableData->szSub_Weapon_Act_Model,		 "Item Table Index");
			ADD_SUB_ITEM("byItem_Type",					(int)pTableData->byItem_Type,			 "Item Type");
			ADD_SUB_ITEM("byEquip_Type",				(int)pTableData->byEquip_Type,			 "Equipment Type");
			ADD_SUB_ITEM("dwEquip_Slot_Type_Bit_Flag",	pTableData->dwEquip_Slot_Type_Bit_Flag,  "Item Table Index");
			ADD_SUB_ITEM("wFunction_Bit_Flag",			pTableData->wFunction_Bit_Flag,			 "Item Table Index");
			ADD_SUB_ITEM("byMax_Stack",					(int)pTableData->byMax_Stack,			 "Item Table Index");
			ADD_SUB_ITEM("byRank",						(int)pTableData->byRank,				 "Item Table Index");
			ADD_SUB_ITEM("dwWeight",					pTableData->dwWeight,					 "Item Table Index");
			ADD_SUB_ITEM("dwCost",						pTableData->dwCost,						 "Item Table Index");
			ADD_SUB_ITEM("dwSell_Price",				pTableData->dwSell_Price,				 "Item Table Index");
			ADD_SUB_ITEM("byDurability",				(int)pTableData->byDurability,			 "Item Table Index");
			ADD_SUB_ITEM("byDurability_Count",			(int)pTableData->byDurability_Count,	 "Item Table Index");
			ADD_SUB_ITEM("byBattle_Attribute",			(int)pTableData->byBattle_Attribute,	 "Item Table Index");
			ADD_SUB_ITEM("wPhysical_Offence",			pTableData->wPhysical_Offence,			 "Item Table Index");
			ADD_SUB_ITEM("wEnergy_Offence",				pTableData->wEnergy_Offence,			 "Item Table Index");
			ADD_SUB_ITEM("wPhysical_Defence",			pTableData->wPhysical_Defence,			 "Item Table Index");
			ADD_SUB_ITEM("wEnergy_Defence",				pTableData->wEnergy_Defence,			 "Item Table Index");
			ADD_SUB_ITEM("fAttack_Range_Bonus",			pTableData->fAttack_Range_Bonus,		 "Item Table Index");
			ADD_SUB_ITEM("wAttack_Speed_Rate",			pTableData->wAttack_Speed_Rate,			 "Item Table Index");
			ADD_SUB_ITEM("byNeed_Min_Level",			(int)pTableData->byNeed_Min_Level,		 "Item Table Index");
			ADD_SUB_ITEM("byNeed_Max_Level",			(int)pTableData->byNeed_Max_Level,		 "Item Table Index");
			ADD_SUB_ITEM("dwNeed_Class_Bit_Flag",		pTableData->dwNeed_Class_Bit_Flag,		 "Item Table Index");
			ADD_SUB_ITEM("dwNeed_Gender_Bit_Flag",		pTableData->dwNeed_Gender_Bit_Flag,		 "Item Table Index");
			ADD_SUB_ITEM("byClass_Special",				(int)pTableData->byClass_Special,		 "Item Table Index");
			ADD_SUB_ITEM("byRace_Special",				(int)pTableData->byRace_Special,		 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Str",					pTableData->wNeed_Str,					 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Con",					pTableData->wNeed_Con,					 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Foc",					pTableData->wNeed_Foc,					 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Dex",					pTableData->wNeed_Dex,					 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Sol",					pTableData->wNeed_Sol,					 "Item Table Index");
			ADD_SUB_ITEM("wNeed_Eng",					pTableData->wNeed_Eng,					 "Item Table Index");
			ADD_SUB_ITEM("set_Item_Tblidx",				pTableData->set_Item_Tblidx,			 "Item Table Index");
			ADD_SUB_ITEM("Note",						pTableData->Note,						 "Item Table Index");
			ADD_SUB_ITEM("byBag_Size",					(int)pTableData->byBag_Size,			 "Item Table Index");
			ADD_SUB_ITEM("wScouter_Watt",				pTableData->wScouter_Watt,				 "Item Table Index");
			ADD_SUB_ITEM("dwScouter_MaxPower",			pTableData->dwScouter_MaxPower,			 "Item Table Index");
			ADD_SUB_ITEM("byScouter_Parts_Type1",		(int)pTableData->byScouter_Parts_Type1,  "Item Table Index");
			ADD_SUB_ITEM("byScouter_Parts_Type2",		(int)pTableData->byScouter_Parts_Type2,  "Item Table Index");
			ADD_SUB_ITEM("byScouter_Parts_Type3",		(int)pTableData->byScouter_Parts_Type3,  "Item Table Index");
			ADD_SUB_ITEM("byScouter_Parts_Type4",		(int)pTableData->byScouter_Parts_Type4,  "Item Table Index");
			ADD_SUB_ITEM("Use_Item_Tblidx",				pTableData->Use_Item_Tblidx,			 "Item Table Index");
			ADD_SUB_ITEM("bIsCanHaveOption",			pTableData->bIsCanHaveOption,			 "Item Table Index");
			ADD_SUB_ITEM("Item_Option_Tblidx",			pTableData->Item_Option_Tblidx,			 "Item Table Index");
			ADD_SUB_ITEM("byItemGroup",					(int)pTableData->byItemGroup,			 "Item Table Index");
			ADD_SUB_ITEM("Charm_Tblidx",				pTableData->Charm_Tblidx,				 "Item Table Index");
			ADD_SUB_ITEM("wCostumeHideBitFlag",			pTableData->wCostumeHideBitFlag,		 "Item Table Index");
			ADD_SUB_ITEM("NeedItemTblidx",				pTableData->NeedItemTblidx,				 "Item Table Index");
			ADD_SUB_ITEM("CommonPoint",					pTableData->CommonPoint,				 "Item Table Index");
			ADD_SUB_ITEM("byCommonPointType",			(int)pTableData->byCommonPointType,		 "Item Table Index");
			ADD_SUB_ITEM("byNeedFunction",				(int)pTableData->byNeedFunction,		 "Item Table Index");
			ADD_SUB_ITEM("dwUseDurationMax",			pTableData->dwUseDurationMax,			 "Item Table Index");
			ADD_SUB_ITEM("byDurationType",				(int)pTableData->byDurationType,		 "Item Table Index");
			ADD_SUB_ITEM("contentsTblidx",				pTableData->contentsTblidx,				 "Item Table Index");
			ADD_SUB_ITEM("dwDurationGroup",				pTableData->dwDurationGroup,			 "Item Table Index");
			ADD_SUB_ITEM("byDropLevel",					(int)pTableData->byDropLevel,			 "Item Table Index");
			ADD_SUB_ITEM("enchantRateTblidx",			pTableData->enchantRateTblidx,			 "Item Table Index");
			ADD_SUB_ITEM("excellentTblidx",				pTableData->excellentTblidx,			 "Item Table Index");
			ADD_SUB_ITEM("rareTblidx",					pTableData->rareTblidx,					 "Item Table Index");
			ADD_SUB_ITEM("legendaryTblidx",				pTableData->legendaryTblidx,			 "Item Table Index");
			ADD_SUB_ITEM("bCreateSuperiorAble",			pTableData->bCreateSuperiorAble,		 "Item Table Index");
			ADD_SUB_ITEM("bCreateExcellentAble",		pTableData->bCreateExcellentAble,		 "Item Table Index");
			ADD_SUB_ITEM("bCreateRareAble",				pTableData->bCreateRareAble,			 "Item Table Index");
			ADD_SUB_ITEM("bCreateLegendaryAble",		pTableData->bCreateLegendaryAble,		 "Item Table Index");
			ADD_SUB_ITEM("byRestrictType",				(int)pTableData->byRestrictType,		 "Item Table Index");
			ADD_SUB_ITEM("fAttack_Physical_Revision",	pTableData->fAttack_Physical_Revision,	 "Item Table Index");
			ADD_SUB_ITEM("fAttack_Energy_Revision",		pTableData->fAttack_Energy_Revision,	 "Item Table Index");
			ADD_SUB_ITEM("fDefence_Physical_Revision",	pTableData->fDefence_Physical_Revision,  "Item Table Index");
			ADD_SUB_ITEM("fDefence_Energy_Revision",	pTableData->fDefence_Energy_Revision,	 "Item Table Index");
			ADD_SUB_ITEM("byTmpTabType",				(int)pTableData->byTmpTabType,			 "Item Table Index");
			ADD_SUB_ITEM("bIsCanRenewal",				pTableData->bIsCanRenewal,				 "Item Table Index");
			ADD_SUB_ITEM("wDisassemble_Bit_Flag",		pTableData->wDisassemble_Bit_Flag,		 "Item Table Index");
			ADD_SUB_ITEM("byDisassembleNormalMin",		(int)pTableData->byDisassembleNormalMin, "Item Table Index");
			ADD_SUB_ITEM("byDisassembleNormalMax",		(int)pTableData->byDisassembleNormalMax, "Item Table Index");
			ADD_SUB_ITEM("byDisassembleUpperMin",		(int)pTableData->byDisassembleUpperMin,	 "Item Table Index");
			ADD_SUB_ITEM("byDisassembleUpperMax",		(int)pTableData->byDisassembleUpperMax,  "Item Table Index");
			ADD_SUB_ITEM("byDropVisual",				(int)pTableData->byDropVisual,			 "Item Table Index");
			ADD_SUB_ITEM("byUseDisassemble",			(int)pTableData->byUseDisassemble,		 "Item Table Index");
		}
		break;

		case CTableContainer::TABLE_NEWBIE:
		{
			sNEWBIE_TBLDAT*	pTableData = (sNEWBIE_TBLDAT*)pTbldat;
			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,		   "Item Table Index");
			ADD_SUB_ITEM("byRace",			(int)pTableData->byRace,   "Item Table Index");
			ADD_SUB_ITEM("byClass",			(int)pTableData->byClass,  "Item Table Index");
			ADD_SUB_ITEM("world_Id",		pTableData->world_Id,	   "Item Table Index");
			ADD_SUB_ITEM("tutorialWorld",	pTableData->tutorialWorld, "Item Table Index");

			ADD_SUB_ITEM("vSpawn_Loc.x",	pTableData->vSpawn_Loc.x,  "Item Table Index");
			ADD_SUB_ITEM("vSpawn_Loc.y",	pTableData->vSpawn_Loc.y,  "Item Table Index");
			ADD_SUB_ITEM("vSpawn_Loc.z",	pTableData->vSpawn_Loc.z,  "Item Table Index");

			ADD_SUB_ITEM("vSpawn_Dir.x",	pTableData->vSpawn_Dir.x,  "Item Table Index");
			ADD_SUB_ITEM("vSpawn_Dir.y",	pTableData->vSpawn_Dir.y,  "Item Table Index");
			ADD_SUB_ITEM("vSpawn_Dir.z",	pTableData->vSpawn_Dir.z,  "Item Table Index");

			ADD_SUB_ITEM("vBind_Loc.x",		pTableData->vBind_Loc.x,   "Item Table Index");
			ADD_SUB_ITEM("vBind_Loc.y",		pTableData->vBind_Loc.y,   "Item Table Index");
			ADD_SUB_ITEM("vBind_Loc.z",		pTableData->vBind_Loc.z,   "Item Table Index");

			ADD_SUB_ITEM("vBind_Dir.x",		pTableData->vBind_Dir.x,   "Item Table Index");
			ADD_SUB_ITEM("vBind_Dir.y",		pTableData->vBind_Dir.y,   "Item Table Index");
			ADD_SUB_ITEM("vBind_Dir.z",		pTableData->vBind_Dir.z,   "Item Table Index");

			for(int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
				ADD_SUB_ITEM("aitem_Tblidx", pTableData->aitem_Tblidx[i], "Item Table Index");

			for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
				ADD_SUB_ITEM("abyPos", (int)pTableData->abyPos[i], "Item Table Index");

			for (int i = 0; i < NTL_MAX_NEWBIE_ITEM; i++)
				ADD_SUB_ITEM("abyStack_Quantity", (int)pTableData->abyStack_Quantity[i], "Item Table Index");

			ADD_SUB_ITEM("mapNameTblidx",	pTableData->mapNameTblidx, "Item Table Index");

			for (int i = 0; i < NTL_MAX_NEWBIE_SKILL; i++)
				ADD_SUB_ITEM("aSkillTblidx", pTableData->aSkillTblidx[i], "Item Table Index");

			for (int i = 0; i < NTL_MAX_NEWBIE_QUICKSLOT_COUNT; i++)
			{
				ADD_SUB_ITEM("QuickSlot Tblidx", pTableData->asQuickData[i].tbilidx,		  "Item Table Index");
				ADD_SUB_ITEM("QuickSlot Type",	 (int)pTableData->asQuickData[i].byType,	  "Item Table Index");
				ADD_SUB_ITEM("QuickSlot Slot",   (int)pTableData->asQuickData[i].byQuickSlot, "Item Table Index");
			}
			
			for (int i = 0; i < 3; i++)
				ADD_SUB_ITEM("defaultPortalId", (int)pTableData->defaultPortalId[i], "Item Table Index");

			ADD_SUB_ITEM("qItemTblidx1",	  pTableData->qItemTblidx1,			  "Quest Item Table Index");
			ADD_SUB_ITEM("byQPosition1",	  (int)pTableData->byQPosition1,	  "Quest item position");
			ADD_SUB_ITEM("byQStackQuantity1", (int)pTableData->byQStackQuantity1, "Quest item quantity");
			ADD_SUB_ITEM("wMixLevelData",	  pTableData->wMixLevelData,		  "Mix level");
		}
		break;
		

		case CTableContainer::TABLE_MASCOT:
		{
			sMASCOT_TBLDAT* pTableData = (sMASCOT_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Mascot Table");
			ADD_SUB_ITEM("Name",				pTableData->Name,					"Mascot Table");
			ADD_SUB_ITEM("wszNameText",			pTableData->wszNameText,			"Mascot Table");
			ADD_SUB_ITEM("bValidity_Able",		pTableData->bValidity_Able,			"Mascot Table");
			ADD_SUB_ITEM("byModel_Type",		(int)pTableData->byModel_Type,		"Mascot Table");
			ADD_SUB_ITEM("szModel",				pTableData->szModel,				"Mascot Table");
			ADD_SUB_ITEM("byRank",				(int)pTableData->byRank,			"Mascot Table");
			ADD_SUB_ITEM("bySlot_Num",			(int)pTableData->bySlot_Num,		"Mascot Table");
			ADD_SUB_ITEM("wSP_Decrease_Rate",	pTableData->wSP_Decrease_Rate,		"Mascot Table");
			ADD_SUB_ITEM("wMax_SP",				pTableData->wMax_SP,				"Mascot Table");
		}
		break;

		case CTableContainer::TABLE_MASCOT_GRADE:
		{
			sMASCOT_GRADE_TBLDAT* pTableData = (sMASCOT_GRADE_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Mascot Grade Table");
			ADD_SUB_ITEM("dwNeedExp",		pTableData->dwNeedExp,		"Mascot Grade Table");
			ADD_SUB_ITEM("wBabyFusion",		pTableData->wBabyFusion,	"Mascot Grade Table");
			ADD_SUB_ITEM("wAdultFusion",	pTableData->wAdultFusion,	"Mascot Grade Table");
			ADD_SUB_ITEM("wLightFusion",	pTableData->wLightFusion,	"Mascot Grade Table");
		}
		break;

		case CTableContainer::TABLE_MASCOT_STATUS:
		{
			sMASCOT_STATUS_TBLDAT* pTableData = (sMASCOT_STATUS_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Mascot Status Table");
			ADD_SUB_ITEM("Name",				pTableData->Name,					"Mascot Status Table");
			ADD_SUB_ITEM("wszNameText",			pTableData->wszNameText,			"Mascot Status Table");
			ADD_SUB_ITEM("bValidity_Able",		pTableData->bValidity_Able,			"Mascot Status Table");
			ADD_SUB_ITEM("szModel",				pTableData->szModel,				"Mascot Status Table");
			ADD_SUB_ITEM("byRank",				(int)pTableData->byRank,			"Mascot Status Table");
			ADD_SUB_ITEM("bySlot_Num",			(int)pTableData->bySlot_Num,		"Mascot Status Table");
			ADD_SUB_ITEM("wVpUpMin",			pTableData->wVpUpMin,				"Mascot Status Table");
			ADD_SUB_ITEM("wVpUpMax",			pTableData->wVpUpMax,				"Mascot Status Table");
			ADD_SUB_ITEM("wSkillGrade1",		pTableData->wSkillGrade1,			"Mascot Status Table");
			ADD_SUB_ITEM("wSkillGrade2",		pTableData->wSkillGrade2,			"Mascot Status Table");
			ADD_SUB_ITEM("wSkillGrade3",		pTableData->wSkillGrade3,			"Mascot Status Table");
			ADD_SUB_ITEM("wVpRegen",			pTableData->wVpRegen,				"Mascot Status Table");
			ADD_SUB_ITEM("wSkillGradeMax",		pTableData->wSkillGradeMax,			"Mascot Status Table");
			ADD_SUB_ITEM("nextMascotTblidx",	pTableData->nextMascotTblidx,		"Mascot Status Table");
			ADD_SUB_ITEM("sealItemIndex",		pTableData->sealItemIndex,			"Mascot Status Table");
		}
		break;

		case CTableContainer::TABLE_CHARTITLE:
		{
			sCHARTITLE_TBLDAT* pTableData = (sCHARTITLE_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",					pTableData->tblidx,					"Char Title Table");
			ADD_SUB_ITEM("tblNameIndex",			pTableData->tblNameIndex,			"Char Title Table");
			ADD_SUB_ITEM("byContentsType",			(int)pTableData->byContentsType,		"Char Title Table");
			ADD_SUB_ITEM("byRepresentationType",	(int)pTableData->byRepresentationType,"Char Title Table");
			ADD_SUB_ITEM("wszBoneName",				pTableData->wszBoneName,			"Char Title Table");
			ADD_SUB_ITEM("wszEffectName",			pTableData->wszEffectName,			"Char Title Table");
			ADD_SUB_ITEM("wszEffectSound",			pTableData->wszEffectSound,			"Char Title Table");

			for (int i = 0; i < NTL_MAX_CHAR_TITLE_EFFECT; i++)
				ADD_SUB_ITEM("atblSystem_Effect_Index", pTableData->atblSystem_Effect_Index[i], "Char Title Table");

			for (int i = 0; i < NTL_MAX_CHAR_TITLE_EFFECT; i++)
				ADD_SUB_ITEM("abySystem_Effect_Type", (int)pTableData->abySystem_Effect_Type[i], "Char Title Table");

			for (int i = 0; i < NTL_MAX_CHAR_TITLE_EFFECT; i++)
				ADD_SUB_ITEM("abySystem_Effect_Value", pTableData->abySystem_Effect_Value[i], "Char Title Table");
		}
		break;

		case CTableContainer::TABLE_CHATTING_FILTER:
		{
			sCHAT_FILTER_TBLDAT* pTableData = (sCHAT_FILTER_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Chat Filter Table");
			ADD_SUB_ITEM("wszSlangText",		pTableData->wszSlangText,			"Chat Filter Table");
			ADD_SUB_ITEM("filteringTextIndex",	pTableData->filteringTextIndex,	"Chat Filter Table");
		}
		break;

		case CTableContainer::TABLE_ITEM_ENCHANT:
		{
			sITEM_ENCHANT_TBLDAT* pTableData = (sITEM_ENCHANT_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Item Enchant Table");
			ADD_SUB_ITEM("wszName",			pTableData->wszName,			"Item Enchant Table");
			ADD_SUB_ITEM("seTblidx",		pTableData->seTblidx,			"Item Enchant Table");
			ADD_SUB_ITEM("bSeType",			pTableData->bSeType,			"Item Enchant Table");
			ADD_SUB_ITEM("byRvType",		(int)pTableData->byRvType,		"Item Enchant Table");
			ADD_SUB_ITEM("byExclIdx",		(int)pTableData->byExclIdx,		"Item Enchant Table");
			ADD_SUB_ITEM("byMinLevel",		(int)pTableData->byMinLevel,	"Item Enchant Table");
			ADD_SUB_ITEM("byMaxLevel",		(int)pTableData->byMaxLevel,	"Item Enchant Table");
			ADD_SUB_ITEM("byFrequency",		(int)pTableData->byFrequency,	"Item Enchant Table");
			ADD_SUB_ITEM("wEnchant_Value",	pTableData->wEnchant_Value,		"Item Enchant Table");
			ADD_SUB_ITEM("byKind",			(int)pTableData->byKind,		"Item Enchant Table");
			ADD_SUB_ITEM("dwEquip",			pTableData->dwEquip,			"Item Enchant Table");
			ADD_SUB_ITEM("byGroupNo",		(int)pTableData->byGroupNo,		"Item Enchant Table");
			ADD_SUB_ITEM("wMaxValue",		pTableData->wMaxValue,			"Item Enchant Table");
			ADD_SUB_ITEM("bIsSuperior",		pTableData->bIsSuperior,		"Item Enchant Table");
			ADD_SUB_ITEM("bIsExcellent",	pTableData->bIsExcellent,		"Item Enchant Table");
			ADD_SUB_ITEM("bIsRare",			pTableData->bIsRare,			"Item Enchant Table");
			ADD_SUB_ITEM("bIsLegendary",	pTableData->bIsLegendary,		"Item Enchant Table");
		}
		break;

		case CTableContainer::TABLE_MERCHANT:
		{
			sMERCHANT_TBLDAT* pTableData = (sMERCHANT_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Merchant Table");
			ADD_SUB_ITEM("wszNameText",		pTableData->wszNameText,		"Merchant Table");
			ADD_SUB_ITEM("bySell_Type",		(int)pTableData->bySell_Type,	"Merchant Table");
			ADD_SUB_ITEM("Tab_Name",		pTableData->Tab_Name,			"Merchant Table");
			ADD_SUB_ITEM("dwNeedMileage",	pTableData->dwNeedMileage,		"Merchant Table");

			for (int i = 0; i < NTL_MAX_MERCHANT_COUNT; i++)
				ADD_SUB_ITEM("aitem_Tblidx", pTableData->aitem_Tblidx[i], "Merchant Table");

			for (int i = 0; i < NTL_MAX_MERCHANT_COUNT; i++)
				ADD_SUB_ITEM("aNeedItemTblidx", pTableData->aNeedItemTblidx[i], "Merchant Table");

			for (int i = 0; i < NTL_MAX_MERCHANT_COUNT; i++)
				ADD_SUB_ITEM("abyNeedItemStack", (int)pTableData->abyNeedItemStack[i], "Merchant Table");

			for (int i = 0; i < NTL_MAX_MERCHANT_COUNT; i++)
				ADD_SUB_ITEM("adwNeedZenny", pTableData->adwNeedZenny[i], "Merchant Table");
		}
		break;

		case CTableContainer::TABLE_FORMULA:
		{
			sFORMULA_TBLDAT* pTableData = (sFORMULA_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Formula Table");
			ADD_SUB_ITEM("bValidity_Able",	pTableData->bValidity_Able,	"Formula Table");

			for (int i = 0; i < DBO_MAX_FORMULA_RATE_COUNT; i++)
				ADD_SUB_ITEM("afRate", pTableData->afRate[i], "Formula Table");
		}
		break;

		case CTableContainer::TABLE_ITEM_MIX_EXP:
		{
			sITEM_MIX_EXP_TBLDAT* pTableData = (sITEM_MIX_EXP_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",		pTableData->tblidx,			"Item Mix Exp Table");
			ADD_SUB_ITEM("dwNeedEXP",	pTableData->dwNeedEXP,		"Item Mix Exp Table");
			ADD_SUB_ITEM("byUnknown",	(int)pTableData->byUnknown,	"Item Mix Exp Table");
		}
		break;

		case CTableContainer::TABLE_CHAT_COMMAND:
		{
			sCHAT_COMMAND_TBLDAT* pTableData = (sCHAT_COMMAND_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",					pTableData->tblidx,					"Chat Command Table");
			ADD_SUB_ITEM("bValidity_Able",			pTableData->bValidity_Able,			"Chat Command Table");
			ADD_SUB_ITEM("wAction_Animation_Index",	pTableData->wAction_Animation_Index,	"Chat Command Table");

			for (int i = 0; i < NTL_MAX_CHAT_COMMAND; i++)
				ADD_SUB_ITEM("aChat_Command", pTableData->aChat_Command[i], "Chat Command Table");
		}
		break;




		case CTableContainer::TABLE_VEHICLE:
		{
			sVEHICLE_TBLDAT* pTableData = (sVEHICLE_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Vehicle Table");
			ADD_SUB_ITEM("szModelName",		pTableData->szModelName,		"Vehicle Table");
			ADD_SUB_ITEM("bySRPType",		(int)pTableData->bySRPType,	"Vehicle Table");
			ADD_SUB_ITEM("bySpeed",			(int)pTableData->bySpeed,	"Vehicle Table");
			ADD_SUB_ITEM("byVehicleType",	(int)pTableData->byVehicleType,"Vehicle Table");
			ADD_SUB_ITEM("wRunHeight",		pTableData->wRunHeight,		"Vehicle Table");
			ADD_SUB_ITEM("byPersonnel",		(int)pTableData->byPersonnel,"Vehicle Table");
		}
		break;

		case CTableContainer::TABLE_DUNGEON:
		{
			sDUNGEON_TBLDAT* pTableData = (sDUNGEON_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Dungeon Table");
			ADD_SUB_ITEM("byDungeonType",	(int)pTableData->byDungeonType,"Dungeon Table");
			ADD_SUB_ITEM("byMaxMember",		(int)pTableData->byMaxMember,"Dungeon Table");
			ADD_SUB_ITEM("linkWorld",		pTableData->linkWorld,		"Dungeon Table");
			ADD_SUB_ITEM("byMinLevel",		(int)pTableData->byMinLevel,	"Dungeon Table");
			ADD_SUB_ITEM("byMaxLevel",		(int)pTableData->byMaxLevel,	"Dungeon Table");
			ADD_SUB_ITEM("needItemTblidx",	pTableData->needItemTblidx,	"Dungeon Table");
			ADD_SUB_ITEM("dwHonorPoint",	pTableData->dwHonorPoint,		"Dungeon Table");
			ADD_SUB_ITEM("wpsTblidx",		pTableData->wpsTblidx,		"Dungeon Table");
			ADD_SUB_ITEM("openCine",		pTableData->openCine,			"Dungeon Table");
			ADD_SUB_ITEM("groupIdx",		pTableData->groupIdx,			"Dungeon Table");
		}
		break;


		case CTableContainer::TABLE_DRAGONBALL_REWARD:
		{
			sDRAGONBALL_REWARD_TBLDAT* pTableData = (sDRAGONBALL_REWARD_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",					pTableData->tblidx,					"Dragon Ball Reward Table");
			ADD_SUB_ITEM("byBallType",				(int)pTableData->byBallType,			"Dragon Ball Reward Table");
			ADD_SUB_ITEM("byRewardCategoryDepth",	(int)pTableData->byRewardCategoryDepth,"Dragon Ball Reward Table");
			ADD_SUB_ITEM("rewardCategoryName",		pTableData->rewardCategoryName,		"Dragon Ball Reward Table");
			ADD_SUB_ITEM("rewardCategoryDialog",	pTableData->rewardCategoryDialog,	"Dragon Ball Reward Table");
			ADD_SUB_ITEM("byRewardType",			(int)pTableData->byRewardType,		"Dragon Ball Reward Table");
			ADD_SUB_ITEM("rewardName",				pTableData->rewardName,				"Dragon Ball Reward Table");
			ADD_SUB_ITEM("rewardLinkTblidx",		pTableData->rewardLinkTblidx,		"Dragon Ball Reward Table");
			ADD_SUB_ITEM("dwRewardZenny",			pTableData->dwRewardZenny,			"Dragon Ball Reward Table");
			ADD_SUB_ITEM("rewardDialog1",			pTableData->rewardDialog1,			"Dragon Ball Reward Table");
			ADD_SUB_ITEM("rewardDialog2",			pTableData->rewardDialog2,			"Dragon Ball Reward Table");
			ADD_SUB_ITEM("dwClassBit",				pTableData->dwClassBit,				"Dragon Ball Reward Table");
		}
		break;

		case CTableContainer::TABLE_EXP:
		{
			sEXP_TBLDAT* pTableData = (sEXP_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",					pTableData->tblidx,					"Exp Table");
			ADD_SUB_ITEM("dwExp",					pTableData->dwExp,					"Exp Table");
			ADD_SUB_ITEM("dwNeed_Exp",				pTableData->dwNeed_Exp,				"Exp Table");
			ADD_SUB_ITEM("wNormal_Race",			pTableData->wNormal_Race,			"Exp Table");
			ADD_SUB_ITEM("wSuperRace",				pTableData->wSuperRace,				"Exp Table");
			ADD_SUB_ITEM("dwMobExp",				pTableData->dwMobExp,				"Exp Table");
			ADD_SUB_ITEM("dwPhyDefenceRef",			pTableData->dwPhyDefenceRef,			"Exp Table");
			ADD_SUB_ITEM("dwEngDefenceRef",			pTableData->dwEngDefenceRef,			"Exp Table");
			ADD_SUB_ITEM("dwMobZenny",				pTableData->dwMobZenny,				"Exp Table");
			ADD_SUB_ITEM("sIndividualRankPoint.wStageWin",		pTableData->sIndividualRankPoint.wStageWin,	"Exp Table (no XML column)");
			ADD_SUB_ITEM("sIndividualRankPoint.wStageDraw",	pTableData->sIndividualRankPoint.wStageDraw,	"Exp Table (no XML column)");
			ADD_SUB_ITEM("sIndividualRankPoint.wStageLose",	pTableData->sIndividualRankPoint.wStageLose,	"Exp Table (no XML column)");
			ADD_SUB_ITEM("sIndividualRankPoint.wWin",			pTableData->sIndividualRankPoint.wWin,			"Exp Table (no XML column)");
			ADD_SUB_ITEM("sIndividualRankPoint.wPerfectWin",	pTableData->sIndividualRankPoint.wPerfectWin,	"Exp Table (no XML column)");
			ADD_SUB_ITEM("sTeamRankPoint.wStageWin",			pTableData->sTeamRankPoint.wStageWin,			"Exp Table (no XML column)");
			ADD_SUB_ITEM("sTeamRankPoint.wStageDraw",			pTableData->sTeamRankPoint.wStageDraw,			"Exp Table (no XML column)");
			ADD_SUB_ITEM("sTeamRankPoint.wStageLose",			pTableData->sTeamRankPoint.wStageLose,			"Exp Table (no XML column)");
			ADD_SUB_ITEM("sTeamRankPoint.wWin",				pTableData->sTeamRankPoint.wWin,				"Exp Table (no XML column)");
			ADD_SUB_ITEM("sTeamRankPoint.wPerfectWin",			pTableData->sTeamRankPoint.wPerfectWin,		"Exp Table (no XML column)");
		}
		break;


		case CTableContainer::TABLE_CHARM:
		{
			sCHARM_TBLDAT* pTableData = (sCHARM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",					pTableData->tblidx,					"Charm Table");
			ADD_SUB_ITEM("wDrop_Rate",				pTableData->wDrop_Rate,				"Charm Table");
			ADD_SUB_ITEM("wEXP",					pTableData->wEXP,					"Charm Table");
			ADD_SUB_ITEM("wRP_Sharing",				pTableData->wRP_Sharing,				"Charm Table");
			ADD_SUB_ITEM("wCool_Time",				pTableData->wCool_Time,				"Charm Table");
			ADD_SUB_ITEM("wKeep_Time",				pTableData->wKeep_Time,				"Charm Table");
			ADD_SUB_ITEM("dwKeep_Time_In_Millisecs",pTableData->dwKeep_Time_In_Millisecs,"Charm Table (derived, no XML column)");
			ADD_SUB_ITEM("dwNeed_Zenny",			pTableData->dwNeed_Zenny,			"Charm Table");
			ADD_SUB_ITEM("byDice_Min",				(int)pTableData->byDice_Min,		"Charm Table");
			ADD_SUB_ITEM("byDice_Max",				(int)pTableData->byDice_Max,		"Charm Table");
			ADD_SUB_ITEM("byCharm_Type_Bit_Flag",	(int)pTableData->byCharm_Type_Bit_Flag,"Charm Table");
		}
		break;

		case CTableContainer::TABLE_ACTION:
		{
			sACTION_TBLDAT* pTableData = (sACTION_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",					pTableData->tblidx,					"Action Table");
			ADD_SUB_ITEM("bValidity_Able",			pTableData->bValidity_Able,			"Action Table");
			ADD_SUB_ITEM("byAction_Type",			(int)pTableData->byAction_Type,		"Action Table");
			ADD_SUB_ITEM("Action_Name",				pTableData->Action_Name,			"Action Table");
			ADD_SUB_ITEM("szIcon_Name",				pTableData->szIcon_Name,			"Action Table");
			ADD_SUB_ITEM("Note",					pTableData->Note,					"Action Table");
			ADD_SUB_ITEM("chat_Command_Index",		pTableData->chat_Command_Index,		"Action Table");
			ADD_SUB_ITEM("byETC_Action_Type",		(int)pTableData->byETC_Action_Type,	"Action Table");
		}
		break;



		case CTableContainer::TABLE_HELP:
		{
			sHELP_TBLDAT* pTableData = (sHELP_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Help Table");
			ADD_SUB_ITEM("byCategory",			(int)pTableData->byCategory,		"Help Table");
			ADD_SUB_ITEM("dwHelpTitle",			pTableData->dwHelpTitle,			"Help Table");
			ADD_SUB_ITEM("dwPopoHint",			pTableData->dwPopoHint,			"Help Table");
			ADD_SUB_ITEM("wszHelpHTMLName",		pTableData->wszHelpHTMLName,		"Help Table");
			ADD_SUB_ITEM("byConditionCheck",	(int)pTableData->byConditionCheck,	"Help Table");
		}
		break;

		case CTableContainer::TABLE_GUIDE_HINT:
		{
			sGUIDE_HINT_TBLDAT* pTableData = (sGUIDE_HINT_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,					"Guide Hint Table");
			ADD_SUB_ITEM("byType",				(int)pTableData->byType,			"Guide Hint Table");
			ADD_SUB_ITEM("byWidthPosition",		(int)pTableData->byWidthPosition,	"Guide Hint Table");
			ADD_SUB_ITEM("byHeightPosition",	(int)pTableData->byHeightPosition,	"Guide Hint Table");
			ADD_SUB_ITEM("wWidthSize",			pTableData->wWidthSize,				"Guide Hint Table");
			ADD_SUB_ITEM("wHeightSize",			pTableData->wHeightSize,			"Guide Hint Table");
			ADD_SUB_ITEM("szResource",			pTableData->szResource,				"Guide Hint Table");
			ADD_SUB_ITEM("szNote",				pTableData->szNote,					"Guide Hint Table");
			ADD_SUB_ITEM("bAutoShow",			pTableData->bAutoShow,				"Guide Hint Table");
		}
		break;






		case CTableContainer::TABLE_DYNAMIC_OBJECT:
		{
			sDYNAMIC_OBJECT_TBLDAT* pTableData = (sDYNAMIC_OBJECT_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Dynamic Object Table");
			ADD_SUB_ITEM("bValidityAble",		pTableData->bValidityAble,			"Dynamic Object Table");
			ADD_SUB_ITEM("byType",				(int)pTableData->byType,			"Dynamic Object Table");
			ADD_SUB_ITEM("szModelName",			pTableData->szModelName,			"Dynamic Object Table");
			ADD_SUB_ITEM("byStateType",			(int)pTableData->byStateType,		"Dynamic Object Table");
			ADD_SUB_ITEM("spawnAnimation",		pTableData->spawnAnimation,		"Dynamic Object Table");
			ADD_SUB_ITEM("idleAnimation",		pTableData->idleAnimation,			"Dynamic Object Table");
			ADD_SUB_ITEM("despawnAnimation",	pTableData->despawnAnimation,		"Dynamic Object Table");
			ADD_SUB_ITEM("state1Animation",		pTableData->state1Animation,		"Dynamic Object Table");
			ADD_SUB_ITEM("state2Animation",		pTableData->state2Animation,		"Dynamic Object Table");
			ADD_SUB_ITEM("byBoundaryDistance",	(int)pTableData->byBoundaryDistance,"Dynamic Object Table");
			ADD_SUB_ITEM("byDespawnDistance",	(int)pTableData->byDespawnDistance,	"Dynamic Object Table");
		}
		break;

		case CTableContainer::TABLE_USE_ITEM:
		{
			sUSE_ITEM_TBLDAT* pTableData = (sUSE_ITEM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",							pTableData->tblidx,							"Use Item Table");
			ADD_SUB_ITEM("byUse_Item_Active_Type",			(int)pTableData->byUse_Item_Active_Type,	"Use Item Table");
			ADD_SUB_ITEM("byBuff_Group",					(int)pTableData->byBuff_Group,				"Use Item Table");
			ADD_SUB_ITEM("byBuffKeepType",					(int)pTableData->byBuffKeepType,			"Use Item Table");
			ADD_SUB_ITEM("dwCool_Time_Bit_Flag",			pTableData->dwCool_Time_Bit_Flag,			"Use Item Table");
			ADD_SUB_ITEM("wFunction_Bit_Flag",				pTableData->wFunction_Bit_Flag,				"Use Item Table");
			ADD_SUB_ITEM("dwUse_Restriction_Rule_Bit_Flag",	pTableData->dwUse_Restriction_Rule_Bit_Flag,"Use Item Table");
			ADD_SUB_ITEM("dwUse_Allow_Rule_Bit_Flag",		pTableData->dwUse_Allow_Rule_Bit_Flag,		"Use Item Table");
			ADD_SUB_ITEM("byAppoint_Target",				(int)pTableData->byAppoint_Target,			"Use Item Table");
			ADD_SUB_ITEM("byApply_Target",					(int)pTableData->byApply_Target,			"Use Item Table");
			ADD_SUB_ITEM("dwApply_Target_Index",			pTableData->dwApply_Target_Index,			"Use Item Table");
			ADD_SUB_ITEM("byApply_Target_Max",				(int)pTableData->byApply_Target_Max,		"Use Item Table");
			ADD_SUB_ITEM("byApply_Range",					(int)pTableData->byApply_Range,				"Use Item Table");
			ADD_SUB_ITEM("byApply_Area_Size_1",				(int)pTableData->byApply_Area_Size_1,		"Use Item Table");
			ADD_SUB_ITEM("byApply_Area_Size_2",				(int)pTableData->byApply_Area_Size_2,		"Use Item Table");
			ADD_SUB_ITEM("wNeed_State_Bit_Flag",			pTableData->wNeed_State_Bit_Flag,			"Use Item Table");

			for (int i = 0; i < NTL_MAX_EFFECT_IN_ITEM; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Use Item Table [System Effect %d]"), i + 1);

				ADD_SUB_ITEM("aSystem_Effect", pTableData->aSystem_Effect[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("abySystem_Effect_Type", (int)pTableData->abySystem_Effect_Type[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("aSystem_Effect_Value", pTableData->aSystem_Effect_Value[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("dwRequire_LP",					pTableData->dwRequire_LP,					"Use Item Table");
			ADD_SUB_ITEM("wRequire_EP",						pTableData->wRequire_EP,						"Use Item Table");
			ADD_SUB_ITEM("byRequire_RP_Ball",				(int)pTableData->byRequire_RP_Ball,			"Use Item Table");
			ADD_SUB_ITEM("fCasting_Time",					pTableData->fCasting_Time,					"Use Item Table");
			ADD_SUB_ITEM("dwCastingTimeInMilliSecs",		pTableData->dwCastingTimeInMilliSecs,		"Use Item Table (derived from Casting_Time)");
			ADD_SUB_ITEM("dwCool_Time",						pTableData->dwCool_Time,						"Use Item Table");
			ADD_SUB_ITEM("dwCoolTimeInMilliSecs",			pTableData->dwCoolTimeInMilliSecs,			"Use Item Table (derived from Cool_Time)");
			ADD_SUB_ITEM("dwKeep_Time",						pTableData->dwKeep_Time,						"Use Item Table");
			ADD_SUB_ITEM("dwKeepTimeInMilliSecs",			pTableData->dwKeepTimeInMilliSecs,			"Use Item Table (derived from Keep_Time)");
			ADD_SUB_ITEM("bKeep_Effect",					pTableData->bKeep_Effect,					"Use Item Table");
			ADD_SUB_ITEM("byUse_Range_Min",					(int)pTableData->byUse_Range_Min,			"Use Item Table");
			ADD_SUB_ITEM("fUse_Range_Min",					pTableData->fUse_Range_Min,					"Use Item Table (derived from Use_Range_Min)");
			ADD_SUB_ITEM("byUse_Range_Max",					(int)pTableData->byUse_Range_Max,			"Use Item Table");
			ADD_SUB_ITEM("fUse_Range_Max",					pTableData->fUse_Range_Max,					"Use Item Table (derived from Use_Range_Max)");
			ADD_SUB_ITEM("Use_Info_Text",					pTableData->Use_Info_Text,					"Use Item Table");
			ADD_SUB_ITEM("szCasting_Effect",				pTableData->szCasting_Effect,				"Use Item Table");
			ADD_SUB_ITEM("szAction_Effect",					pTableData->szAction_Effect,					"Use Item Table");
			ADD_SUB_ITEM("wCasting_Animation_Start",		pTableData->wCasting_Animation_Start,		"Use Item Table");
			ADD_SUB_ITEM("wCasting_Animation_Loop",			pTableData->wCasting_Animation_Loop,		"Use Item Table");
			ADD_SUB_ITEM("wAction_Animation_Index",			pTableData->wAction_Animation_Index,		"Use Item Table");
			ADD_SUB_ITEM("wAction_Loop_Animation_Index",	pTableData->wAction_Loop_Animation_Index,	"Use Item Table");
			ADD_SUB_ITEM("wAction_End_Animation_Index",		pTableData->wAction_End_Animation_Index,	"Use Item Table");
			ADD_SUB_ITEM("byCastingEffectPosition",			(int)pTableData->byCastingEffectPosition,	"Use Item Table");
			ADD_SUB_ITEM("byActionEffectPosition",			(int)pTableData->byActionEffectPosition,	"Use Item Table");
			ADD_SUB_ITEM("useWorldTblidx",					pTableData->useWorldTblidx,					"Use Item Table");
			ADD_SUB_ITEM("fUseLoc_X",						pTableData->fUseLoc_X,						"Use Item Table");
			ADD_SUB_ITEM("fUseLoc_Z",						pTableData->fUseLoc_Z,						"Use Item Table");
			ADD_SUB_ITEM("fUseLoc_Radius",					pTableData->fUseLoc_Radius,					"Use Item Table");
			ADD_SUB_ITEM("RequiredQuestID",					pTableData->RequiredQuestID,					"Use Item Table");
		}
		break;

		case CTableContainer::TABLE_SET_ITEM:
		{
			sSET_ITEM_TBLDAT* pTableData = (sSET_ITEM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Set Item Table");
			ADD_SUB_ITEM("bValidity_Able",	pTableData->bValidity_Able,	"Set Item Table");
			ADD_SUB_ITEM("semiSetOption",	pTableData->semiSetOption,	"Set Item Table");
			ADD_SUB_ITEM("fullSetOption",	pTableData->fullSetOption,	"Set Item Table");

			for (int i = 0; i < NTL_MAX_SET_ITEM_COUNT; i++)
				ADD_SUB_ITEM("aItemTblidx", pTableData->aItemTblidx[i], "Set Item Table");
		}
		break;







		case CTableContainer::TABLE_QUEST_REWARD:
		{
			sQUEST_REWARD_TBLDAT* pTableData = (sQUEST_REWARD_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Quest Reward Table");
			ADD_SUB_ITEM("dwDef_Reward_EXP",	pTableData->dwDef_Reward_EXP,	"Quest Reward Table");
			ADD_SUB_ITEM("dwDef_Reward_Zeny",	pTableData->dwDef_Reward_Zeny,	"Quest Reward Table");

			for (int i = 0; i < QUEST_REWARD_DEF_MAX_CNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Quest Reward Table [Default Reward %d]"), i);

				ADD_SUB_ITEM("byRewardType", (int)pTableData->arsDefRwd[i].byRewardType, (LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwRewardIdx", pTableData->arsDefRwd[i].dwRewardIdx, (LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwRewardVal", pTableData->arsDefRwd[i].dwRewardVal, (LPCTSTR)strGroup);
			}

			for (int i = 0; i < QUEST_REWARD_SEL_MAX_CNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Quest Reward Table [Selectable Reward %d]"), i);

				ADD_SUB_ITEM("byRewardType", (int)pTableData->arsSelRwd[i].byRewardType, (LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwRewardIdx", pTableData->arsSelRwd[i].dwRewardIdx, (LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwRewardVal", pTableData->arsSelRwd[i].dwRewardVal, (LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_QUEST_REWARD_SELECT:
		{
			sQUEST_REWARD_SELECT_TBLDAT* pTableData = (sQUEST_REWARD_SELECT_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Quest Reward Select Table");
			ADD_SUB_ITEM("bySelect_Type",	(int)pTableData->bySelect_Type,"Quest Reward Select Table (no XML column)");

			for (int i = 0; i < DBO_MAX_COUNT_OF_QUEST_REWARD_SELECT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Quest Reward Select Table [Reward %d] (no XML column)"), i + 1);

				ADD_SUB_ITEM("byRewardType", (int)pTableData->aRewardSet[i].byRewardType, (LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwRewardIdx", pTableData->aRewardSet[i].dwRewardIdx, (LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwRewardVal", pTableData->aRewardSet[i].dwRewardVal, (LPCTSTR)strGroup);
			}
		}
		break;




		case CTableContainer::TABLE_NPC_SERVER:
		{
			sNPC_SERVER_TBLDAT* pTableData = (sNPC_SERVER_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Npc Server Table");
			ADD_SUB_ITEM("bValidity_Able",		pTableData->bValidity_Able,		"Npc Server Table (no XML column)");
			ADD_SUB_ITEM("dwServerBitFlag",		pTableData->dwServerBitFlag,		"Npc Server Table (no XML column)");
		}
		break;

		case CTableContainer::TABLE_WORLD:
		{
			sWORLD_TBLDAT* pTableData = (sWORLD_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,					"World Table");
			ADD_SUB_ITEM("szName",						pTableData->szName,					"World Table");
			ADD_SUB_ITEM("wszName",						pTableData->wszName,					"World Table");
			ADD_SUB_ITEM("bDynamic",					pTableData->bDynamic,				"World Table");
			ADD_SUB_ITEM("nCreateCount",					pTableData->nCreateCount,			"World Table");
			ADD_SUB_ITEM("dwDynamicCreateCountShareGroup",pTableData->dwDynamicCreateCountShareGroup,"World Table (no XML column)");
			ADD_SUB_ITEM("byDoorType",					(int)pTableData->byDoorType,		"World Table");
			ADD_SUB_ITEM("dwDestroyTimeInMilliSec",		pTableData->dwDestroyTimeInMilliSec,"World Table (XML column: Field_Destory_Time, seconds*1000)");
			ADD_SUB_ITEM("wszMobSpawn_Table_Name",		pTableData->wszMobSpawn_Table_Name,	"World Table");
			ADD_SUB_ITEM("wszNpcSpawn_Table_Name",		pTableData->wszNpcSpawn_Table_Name,	"World Table");
			ADD_SUB_ITEM("wszObjSpawn_Table_Name",		pTableData->wszObjSpawn_Table_Name,	"World Table");
			ADD_SUB_ITEM("vStart.x",					pTableData->vStart.x,				"World Table (XML: Field_Start_Loc_X)");
			ADD_SUB_ITEM("vStart.z",					pTableData->vStart.z,				"World Table (XML: Field_Start_Loc_Z)");
			ADD_SUB_ITEM("vEnd.x",						pTableData->vEnd.x,					"World Table (XML: Field_End_Loc_X)");
			ADD_SUB_ITEM("vEnd.z",						pTableData->vEnd.z,					"World Table (XML: Field_End_Loc_Z)");
			ADD_SUB_ITEM("vStandardLoc.x",				pTableData->vStandardLoc.x,			"World Table");
			ADD_SUB_ITEM("vStandardLoc.z",				pTableData->vStandardLoc.z,			"World Table");
			ADD_SUB_ITEM("vBattleStartLoc.x",			pTableData->vBattleStartLoc.x,		"World Table (XML: Battle_Start_X)");
			ADD_SUB_ITEM("vBattleStartLoc.z",			pTableData->vBattleStartLoc.z,		"World Table (XML: Battle_Start_Z)");
			ADD_SUB_ITEM("vBattleEndLoc.x",				pTableData->vBattleEndLoc.x,			"World Table (XML: Battle_End_X)");
			ADD_SUB_ITEM("vBattleEndLoc.z",				pTableData->vBattleEndLoc.z,			"World Table (XML: Battle_End_Z)");
			ADD_SUB_ITEM("vBattleStart2Loc.x",			pTableData->vBattleStart2Loc.x,		"World Table (no XML column)");
			ADD_SUB_ITEM("vBattleStart2Loc.z",			pTableData->vBattleStart2Loc.z,		"World Table (no XML column)");
			ADD_SUB_ITEM("vBattleEnd2Loc.x",			pTableData->vBattleEnd2Loc.x,		"World Table (no XML column)");
			ADD_SUB_ITEM("vBattleEnd2Loc.z",			pTableData->vBattleEnd2Loc.z,		"World Table (no XML column)");
			ADD_SUB_ITEM("vOutSideBattleStartLoc.x",	pTableData->vOutSideBattleStartLoc.x,"World Table");
			ADD_SUB_ITEM("vOutSideBattleStartLoc.z",	pTableData->vOutSideBattleStartLoc.z,"World Table");
			ADD_SUB_ITEM("vOutSideBattleEndLoc.x",		pTableData->vOutSideBattleEndLoc.x,	"World Table");
			ADD_SUB_ITEM("vOutSideBattleEndLoc.z",		pTableData->vOutSideBattleEndLoc.z,	"World Table");
			ADD_SUB_ITEM("vSpectatorStartLoc.x",		pTableData->vSpectatorStartLoc.x,	"World Table");
			ADD_SUB_ITEM("vSpectatorStartLoc.z",		pTableData->vSpectatorStartLoc.z,	"World Table");
			ADD_SUB_ITEM("vSpectatorEndLoc.x",			pTableData->vSpectatorEndLoc.x,		"World Table");
			ADD_SUB_ITEM("vSpectatorEndLoc.z",			pTableData->vSpectatorEndLoc.z,		"World Table");
			ADD_SUB_ITEM("vDefaultLoc.x",				pTableData->vDefaultLoc.x,			"World Table");
			ADD_SUB_ITEM("vDefaultLoc.y",				pTableData->vDefaultLoc.y,			"World Table");
			ADD_SUB_ITEM("vDefaultLoc.z",				pTableData->vDefaultLoc.z,			"World Table");
			ADD_SUB_ITEM("vDefaultDir.x",				pTableData->vDefaultDir.x,			"World Table");
			ADD_SUB_ITEM("vDefaultDir.y",				pTableData->vDefaultDir.y,			"World Table (XML column: Default_Dri_Y, typo preserved)");
			ADD_SUB_ITEM("vDefaultDir.z",				pTableData->vDefaultDir.z,			"World Table");
			ADD_SUB_ITEM("vStart1Loc.x",				pTableData->vStart1Loc.x,			"World Table");
			ADD_SUB_ITEM("vStart1Loc.y",				pTableData->vStart1Loc.y,			"World Table");
			ADD_SUB_ITEM("vStart1Loc.z",				pTableData->vStart1Loc.z,			"World Table");
			ADD_SUB_ITEM("vStart1Dir.x",				pTableData->vStart1Dir.x,			"World Table");
			ADD_SUB_ITEM("vStart1Dir.z",				pTableData->vStart1Dir.z,			"World Table");
			ADD_SUB_ITEM("vStart2Loc.x",				pTableData->vStart2Loc.x,			"World Table");
			ADD_SUB_ITEM("vStart2Loc.y",				pTableData->vStart2Loc.y,			"World Table");
			ADD_SUB_ITEM("vStart2Loc.z",				pTableData->vStart2Loc.z,			"World Table");
			ADD_SUB_ITEM("vStart2Dir.x",				pTableData->vStart2Dir.x,			"World Table");
			ADD_SUB_ITEM("vStart2Dir.z",				pTableData->vStart2Dir.z,			"World Table");
			ADD_SUB_ITEM("vWaitingPoint1Loc.x",			pTableData->vWaitingPoint1Loc.x,	"World Table (no XML column)");
			ADD_SUB_ITEM("vWaitingPoint1Loc.z",			pTableData->vWaitingPoint1Loc.z,	"World Table (no XML column)");
			ADD_SUB_ITEM("vWaitingPoint1Dir.x",			pTableData->vWaitingPoint1Dir.x,	"World Table (no XML column)");
			ADD_SUB_ITEM("vWaitingPoint1Dir.z",			pTableData->vWaitingPoint1Dir.z,	"World Table (no XML column)");
			ADD_SUB_ITEM("vWaitingPoint2Loc.x",			pTableData->vWaitingPoint2Loc.x,	"World Table (no XML column)");
			ADD_SUB_ITEM("vWaitingPoint2Loc.z",			pTableData->vWaitingPoint2Loc.z,	"World Table (no XML column)");
			ADD_SUB_ITEM("vWaitingPoint2Dir.x",			pTableData->vWaitingPoint2Dir.x,	"World Table (no XML column)");
			ADD_SUB_ITEM("vWaitingPoint2Dir.z",			pTableData->vWaitingPoint2Dir.z,	"World Table (no XML column)");
			ADD_SUB_ITEM("fSplitSize",					pTableData->fSplitSize,				"World Table");
			ADD_SUB_ITEM("bNight_Able",					pTableData->bNight_Able,				"World Table");
			ADD_SUB_ITEM("byStatic_Time",				(int)pTableData->byStatic_Time,		"World Table");
			ADD_SUB_ITEM("wFuncFlag",					pTableData->wFuncFlag,				"World Table (XML column: funcflag, hex)");
			ADD_SUB_ITEM("byWorldRuleType",				(int)pTableData->byWorldRuleType,	"World Table");
			ADD_SUB_ITEM("worldRuleTbldx",				pTableData->worldRuleTbldx,			"World Table");
			ADD_SUB_ITEM("outWorldTblidx",				pTableData->outWorldTblidx,			"World Table");
			ADD_SUB_ITEM("outWorldLoc.x",				pTableData->outWorldLoc.x,			"World Table");
			ADD_SUB_ITEM("outWorldLoc.z",				pTableData->outWorldLoc.z,			"World Table");
			ADD_SUB_ITEM("outWorldDir.x",				pTableData->outWorldDir.x,			"World Table (XML: Out_Field_Dir_X AND buggy Out_Field_Dir_Z both write here)");
			ADD_SUB_ITEM("outWorldDir.z",				pTableData->outWorldDir.z,			"World Table (dead: Out_Field_Dir_Z bug writes to .x instead)");
			ADD_SUB_ITEM("wszResourceFolder",			pTableData->wszResourceFolder,		"World Table");
			ADD_SUB_ITEM("fBGMRestTime",					pTableData->fBGMRestTime,			"World Table");
			ADD_SUB_ITEM("dwWorldResourceID",			pTableData->dwWorldResourceID,		"World Table");
			ADD_SUB_ITEM("fFreeCamera_Height",			pTableData->fFreeCamera_Height,		"World Table");
			ADD_SUB_ITEM("wszEnterResourceFlash",		pTableData->wszEnterResourceFlash,	"World Table");
			ADD_SUB_ITEM("wszLeaveResourceFlash",		pTableData->wszLeaveResourceFlash,	"World Table");
			ADD_SUB_ITEM("wpsLinkIndex",					pTableData->wpsLinkIndex,			"World Table");
			ADD_SUB_ITEM("byStartPointRange",			(int)pTableData->byStartPointRange,	"World Table");

			for (int i = 0; i < (int)DBO_MAX_WORLD_DRAGONBALLDROP; i++)
			{
				CString strGroup;
				strGroup.Format(_T("World Table [Dragonball %d]"), i + 1);

				ADD_SUB_ITEM("abyDragonBallHaveRate", (int)pTableData->abyDragonBallHaveRate[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("abyDragonBallDropRate", (int)pTableData->abyDragonBallDropRate[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("dwProhibition_Bit_Flag",		pTableData->dwProhibition_Bit_Flag,	"World Table (no XML column)");
		}
		break;


		case CTableContainer::TABLE_SYSTEM_EFFECT:
		{
			sSYSTEM_EFFECT_TBLDAT* pTableData = (sSYSTEM_EFFECT_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,					"System Effect Table");
			ADD_SUB_ITEM("wszName",						pTableData->wszName,					"System Effect Table");
			ADD_SUB_ITEM("byEffect_Type",				(int)pTableData->byEffect_Type,		"System Effect Table");
			ADD_SUB_ITEM("byActive_Effect_Type",		(int)pTableData->byActive_Effect_Type,"System Effect Table");
			ADD_SUB_ITEM("Effect_Info_Text",			pTableData->Effect_Info_Text,		"System Effect Table");
			ADD_SUB_ITEM("Keep_Effect_Name",			pTableData->Keep_Effect_Name,		"System Effect Table");
			ADD_SUB_ITEM("byTarget_Effect_Position",	(int)pTableData->byTarget_Effect_Position,"System Effect Table");
			ADD_SUB_ITEM("szSuccess_Effect_Name",		pTableData->szSuccess_Effect_Name,	"System Effect Table");
			ADD_SUB_ITEM("bySuccess_Projectile_Type",	(int)pTableData->bySuccess_Projectile_Type,"System Effect Table");
			ADD_SUB_ITEM("bySuccess_Effect_Position",	(int)pTableData->bySuccess_Effect_Position,"System Effect Table");
			ADD_SUB_ITEM("szSuccess_End_Effect_Name",	pTableData->szSuccess_End_Effect_Name,"System Effect Table");
			ADD_SUB_ITEM("byEnd_Effect_Position",		(int)pTableData->byEnd_Effect_Position,"System Effect Table");
			ADD_SUB_ITEM("wKeep_Animation_Index",		pTableData->wKeep_Animation_Index,	"System Effect Table");
			ADD_SUB_ITEM("effectCode",					(int)pTableData->effectCode,		"System Effect Table (derived from Name in AddTable, not XML)");
		}
		break;

		case CTableContainer::TABLE_ITEM_OPTION:
		{
			sITEM_OPTION_TBLDAT* pTableData = (sITEM_OPTION_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Item Option Table");
			ADD_SUB_ITEM("wszOption_Name",	pTableData->wszOption_Name,	"Item Option Table");
			ADD_SUB_ITEM("bValidity_Able",	pTableData->bValidity_Able,	"Item Option Table");
			ADD_SUB_ITEM("byOption_Rank",	(int)pTableData->byOption_Rank,"Item Option Table");
			ADD_SUB_ITEM("byItem_Group",	(int)pTableData->byItem_Group,"Item Option Table");
			ADD_SUB_ITEM("byMaxQuality",	(int)pTableData->byMaxQuality,"Item Option Table");
			ADD_SUB_ITEM("byQuality",		(int)pTableData->byQuality,	"Item Option Table");
			ADD_SUB_ITEM("byQualityIndex",	(int)pTableData->byQualityIndex,"Item Option Table");
			ADD_SUB_ITEM("dwCost",			pTableData->dwCost,			"Item Option Table");
			ADD_SUB_ITEM("byLevel",			(int)pTableData->byLevel,	"Item Option Table");

			for (int i = 0; i < NTL_MAX_SYSTEM_EFFECT_COUNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Item Option Table [Effect %d]"), i + 1);

				ADD_SUB_ITEM("system_Effect", pTableData->system_Effect[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("bAppliedInPercent", pTableData->bAppliedInPercent[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("nValue", pTableData->nValue[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("byScouterInfo", (int)pTableData->byScouterInfo[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("activeEffect",	pTableData->activeEffect,	"Item Option Table");
			ADD_SUB_ITEM("fActiveRate",		pTableData->fActiveRate,		"Item Option Table");
			ADD_SUB_ITEM("szNote",			pTableData->szNote,			"Item Option Table");
		}
		break;

		case CTableContainer::TABLE_SKILL:
		{
			sSKILL_TBLDAT* pTableData = (sSKILL_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",							pTableData->tblidx,						"Skill Table");
			ADD_SUB_ITEM("Skill_Name",						pTableData->Skill_Name,					"Skill Table");
			ADD_SUB_ITEM("wszNameText",						pTableData->wszNameText,					"Skill Table");
			ADD_SUB_ITEM("bValidity_Able",					pTableData->bValidity_Able,				"Skill Table");
			ADD_SUB_ITEM("dwPC_Class_Bit_Flag",				pTableData->dwPC_Class_Bit_Flag,			"Skill Table (hex)");
			ADD_SUB_ITEM("byClass_Type",					(int)pTableData->byClass_Type,			"Skill Table (no XML column)");
			ADD_SUB_ITEM("bySkill_Class",					(int)pTableData->bySkill_Class,			"Skill Table");
			ADD_SUB_ITEM("bySkill_Type",					(int)pTableData->bySkill_Type,			"Skill Table");
			ADD_SUB_ITEM("bySkill_Active_Type",				(int)pTableData->bySkill_Active_Type,	"Skill Table");
			ADD_SUB_ITEM("byBuff_Group",					(int)pTableData->byBuff_Group,			"Skill Table");
			ADD_SUB_ITEM("bySlot_Index",						(int)pTableData->bySlot_Index,			"Skill Table");
			ADD_SUB_ITEM("bySkill_Grade",					(int)pTableData->bySkill_Grade,			"Skill Table");
			ADD_SUB_ITEM("dwFunction_Bit_Flag",				pTableData->dwFunction_Bit_Flag,			"Skill Table (hex)");
			ADD_SUB_ITEM("byAppoint_Target",				(int)pTableData->byAppoint_Target,		"Skill Table");
			ADD_SUB_ITEM("byApply_Target",					(int)pTableData->byApply_Target,			"Skill Table");
			ADD_SUB_ITEM("byApply_Target_Max",				(int)pTableData->byApply_Target_Max,		"Skill Table");
			ADD_SUB_ITEM("byApply_Range",					(int)pTableData->byApply_Range,			"Skill Table");
			ADD_SUB_ITEM("byApply_Area_Size_1",				(int)pTableData->byApply_Area_Size_1,	"Skill Table");
			ADD_SUB_ITEM("byApply_Area_Size_2",				(int)pTableData->byApply_Area_Size_2,	"Skill Table");

			for (int i = 0; i < (int)NTL_MAX_EFFECT_IN_SKILL; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Skill Table [Skill Effect %d]"), i + 1);

				ADD_SUB_ITEM("skill_Effect", pTableData->skill_Effect[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("bySkill_Effect_Type", (int)pTableData->bySkill_Effect_Type[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("aSkill_Effect_Value", pTableData->aSkill_Effect_Value[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("dwAdditional_Aggro_Point",		pTableData->dwAdditional_Aggro_Point,	"Skill Table (no XML column)");

			for (int i = 0; i < (int)DBO_MAX_RP_BONUS_COUNT_PER_SKILL; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Skill Table [RP Effect %d]"), i + 1);

				ADD_SUB_ITEM("abyRpEffect", (int)pTableData->abyRpEffect[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("afRpEffectValue", pTableData->afRpEffectValue[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("byRequire_Train_Level",			(int)pTableData->byRequire_Train_Level,	"Skill Table");
			ADD_SUB_ITEM("dwRequire_Zenny",					pTableData->dwRequire_Zenny,				"Skill Table");
			ADD_SUB_ITEM("wNext_Skill_Train_Exp",			pTableData->wNext_Skill_Train_Exp,		"Skill Table");
			ADD_SUB_ITEM("wRequireSP",						pTableData->wRequireSP,					"Skill Table");
			ADD_SUB_ITEM("bSelfTrain",						pTableData->bSelfTrain,					"Skill Table");
			ADD_SUB_ITEM("uiRequire_Skill_Tblidx_Min_1",	pTableData->uiRequire_Skill_Tblidx_Min_1,"Skill Table");
			ADD_SUB_ITEM("uiRequire_Skill_Tblidx_Max_1",	pTableData->uiRequire_Skill_Tblidx_Max_1,"Skill Table");
			ADD_SUB_ITEM("uiRequire_Skill_Tblidx_Min_2",	pTableData->uiRequire_Skill_Tblidx_Min_2,"Skill Table");
			ADD_SUB_ITEM("uiRequire_Skill_Tblidx_Max_2",	pTableData->uiRequire_Skill_Tblidx_Max_2,"Skill Table");
			ADD_SUB_ITEM("Root_Skill",						pTableData->Root_Skill,					"Skill Table");
			ADD_SUB_ITEM("byRequire_Epuip_Slot_Type",		(int)pTableData->byRequire_Epuip_Slot_Type,"Skill Table");
			ADD_SUB_ITEM("byRequire_Item_Type",				(int)pTableData->byRequire_Item_Type,	"Skill Table");
			ADD_SUB_ITEM("szIcon_Name",						pTableData->szIcon_Name,					"Skill Table");
			ADD_SUB_ITEM("dwRequire_LP",						pTableData->dwRequire_LP,					"Skill Table");
			ADD_SUB_ITEM("wRequire_EP",						pTableData->wRequire_EP,					"Skill Table");
			ADD_SUB_ITEM("byRequire_RP_Ball",				(int)pTableData->byRequire_RP_Ball,		"Skill Table");
			ADD_SUB_ITEM("fCasting_Time",					pTableData->fCasting_Time,				"Skill Table");
			ADD_SUB_ITEM("dwCastingTimeInMilliSecs",		pTableData->dwCastingTimeInMilliSecs,	"Skill Table (derived from Casting_Time)");
			ADD_SUB_ITEM("wCool_Time",						pTableData->wCool_Time,					"Skill Table");
			ADD_SUB_ITEM("dwCoolTimeInMilliSecs",			pTableData->dwCoolTimeInMilliSecs,		"Skill Table (derived from Cool_Time)");
			ADD_SUB_ITEM("wKeep_Time",						pTableData->wKeep_Time,					"Skill Table");
			ADD_SUB_ITEM("dwKeepTimeInMilliSecs",			pTableData->dwKeepTimeInMilliSecs,		"Skill Table (derived from Keep_Time)");
			ADD_SUB_ITEM("bKeep_Effect",						pTableData->bKeep_Effect,				"Skill Table");
			ADD_SUB_ITEM("byUse_Range_Min",					(int)pTableData->byUse_Range_Min,		"Skill Table");
			ADD_SUB_ITEM("fUse_Range_Min",					pTableData->fUse_Range_Min,				"Skill Table (derived from Use_Range_Min)");
			ADD_SUB_ITEM("byUse_Range_Max",					(int)pTableData->byUse_Range_Max,		"Skill Table");
			ADD_SUB_ITEM("fUse_Range_Max",					pTableData->fUse_Range_Max,				"Skill Table (derived from Use_Range_Max)");
			ADD_SUB_ITEM("Note",								pTableData->Note,							"Skill Table");
			ADD_SUB_ITEM("dwNextSkillTblidx",				pTableData->dwNextSkillTblidx,			"Skill Table");
			ADD_SUB_ITEM("bDefaultDisplayOff",				pTableData->bDefaultDisplayOff,			"Skill Table");
			ADD_SUB_ITEM("dwAnimation_Time",				pTableData->dwAnimation_Time,			"Skill Table");
			ADD_SUB_ITEM("wCasting_Animation_Start",		pTableData->wCasting_Animation_Start,	"Skill Table");
			ADD_SUB_ITEM("wCasting_Animation_Loop",			pTableData->wCasting_Animation_Loop,	"Skill Table");
			ADD_SUB_ITEM("wAction_Animation_Index",			pTableData->wAction_Animation_Index,	"Skill Table");
			ADD_SUB_ITEM("wAction_Loop_Animation_Index",	pTableData->wAction_Loop_Animation_Index,"Skill Table");
			ADD_SUB_ITEM("wAction_End_Animation_Index",		pTableData->wAction_End_Animation_Index,"Skill Table");
			ADD_SUB_ITEM("bDash_Able",						pTableData->bDash_Able,					"Skill Table");
			ADD_SUB_ITEM("dwTransform_Use_Info_Bit_Flag",	pTableData->dwTransform_Use_Info_Bit_Flag,"Skill Table (hex)");
			ADD_SUB_ITEM("fSuccess_Rate",					pTableData->fSuccess_Rate,				"Skill Table");
			ADD_SUB_ITEM("byPC_Class_Change",				(int)pTableData->byPC_Class_Change,		"Skill Table");
			ADD_SUB_ITEM("byUse_Type",						(int)pTableData->byUse_Type,				"Skill Table");
			ADD_SUB_ITEM("bySkill_Group",					(int)pTableData->bySkill_Group,			"Skill Table (no XML column)");
			ADD_SUB_ITEM("dwRequire_VP",						pTableData->dwRequire_VP,				"Skill Table (no XML column)");
			ADD_SUB_ITEM("dwUse_Restriction_Rule_Bit_Flag",	pTableData->dwUse_Restriction_Rule_Bit_Flag,"Skill Table (no XML column)");
		}
		break;




		case CTableContainer::TABLE_HLS_ITEM:
		{
			sHLS_ITEM_TBLDAT* pTableData = (sHLS_ITEM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"HLS Item Table");
			ADD_SUB_ITEM("itemTblidx",		pTableData->itemTblidx,		"HLS Item Table");
			ADD_SUB_ITEM("byCategory",		(int)pTableData->byCategory,	"HLS Item Table (no XML column)");
			ADD_SUB_ITEM("bOnSale",			pTableData->bOnSale,			"HLS Item Table");
			ADD_SUB_ITEM("dwCash",			pTableData->dwCash,			"HLS Item Table");
			ADD_SUB_ITEM("byStackCount",	(int)pTableData->byStackCount,"HLS Item Table");
			ADD_SUB_ITEM("wDisplayBitFlag",	pTableData->wDisplayBitFlag,	"HLS Item Table (hex)");
		}
		break;

		case CTableContainer::TABLE_STATUS_TRANSFORM:
		{
			sSTATUS_TRANSFORM_TBLDAT* pTableData = (sSTATUS_TRANSFORM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,						"Status Transform Table");
			ADD_SUB_ITEM("fLP_Transform",				pTableData->fLP_Transform,					"Status Transform Table");
			ADD_SUB_ITEM("fEP_Transform",				pTableData->fEP_Transform,					"Status Transform Table");
			ADD_SUB_ITEM("fPhysical_Offence_Transform",	pTableData->fPhysical_Offence_Transform,	"Status Transform Table");
			ADD_SUB_ITEM("fEnergy_Offence_Transform",	pTableData->fEnergy_Offence_Transform,		"Status Transform Table");
			ADD_SUB_ITEM("fPhysical_Defence_Transform",	pTableData->fPhysical_Defence_Transform,	"Status Transform Table");
			ADD_SUB_ITEM("fEnergy_Defence_Transform",	pTableData->fEnergy_Defence_Transform,		"Status Transform Table");
			ADD_SUB_ITEM("fRun_Speed_Transform",		pTableData->fRun_Speed_Transform,			"Status Transform Table");
			ADD_SUB_ITEM("fAttack_Speed_Transform",		pTableData->fAttack_Speed_Transform,		"Status Transform Table");
			ADD_SUB_ITEM("fAttack_Rate_Transform",		pTableData->fAttack_Rate_Transform,			"Status Transform Table");
			ADD_SUB_ITEM("fDodge_Rate_Transform",		pTableData->fDodge_Rate_Transform,			"Status Transform Table");
			ADD_SUB_ITEM("fBlock_Rate_Transform",		pTableData->fBlock_Rate_Transform,			"Status Transform Table");
			ADD_SUB_ITEM("fCurse_Success_Transform",	pTableData->fCurse_Success_Transform,		"Status Transform Table");
			ADD_SUB_ITEM("fCurse_Tolerance_Transform",	pTableData->fCurse_Tolerance_Transform,		"Status Transform Table");
			ADD_SUB_ITEM("fAttack_Range_Change",		pTableData->fAttack_Range_Change,			"Status Transform Table");
			ADD_SUB_ITEM("fLP_Consume_Rate",			pTableData->fLP_Consume_Rate,				"Status Transform Table");
			ADD_SUB_ITEM("fEP_Consume_Rate",			pTableData->fEP_Consume_Rate,				"Status Transform Table");
			// dwDurationInMilliSecs is derived from dwDuration (*1000) by the
			// reader, not its own XML column -- see GetXmlExportCaveat.
			ADD_SUB_ITEM("dwDuration",					pTableData->dwDuration,						"Status Transform Table (XML column: Duration)");
			ADD_SUB_ITEM("dwDurationInMilliSecs",		pTableData->dwDurationInMilliSecs,			"Status Transform Table (no XML column, derived)");
		}
		break;



		case CTableContainer::TABLE_QUEST_PROBABILITY:
		{
			sQUEST_PROBABILITY_TBLDAT* pTableData = (sQUEST_PROBABILITY_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Quest Probability Table");
			ADD_SUB_ITEM("wszName",			pTableData->wszName,			"Quest Probability Table (XML column: Name)");
			ADD_SUB_ITEM("wszNote",			pTableData->wszNote,			"Quest Probability Table (XML column: Note)");
			// eUseType and byCount are recognized by nothing in the XML
			// reader (verified against SetTableData) -- no XML column at all.
			ADD_SUB_ITEM("eUseType",		(int)pTableData->eUseType,		"Quest Probability Table (no XML column)");
			ADD_SUB_ITEM("byProbabilityType",(int)pTableData->byProbabilityType,"Quest Probability Table (XML column: Probability_Type)");
			ADD_SUB_ITEM("bAllowBlank",		pTableData->bAllowBlank,		"Quest Probability Table (XML column: Allow_Blank)");
			ADD_SUB_ITEM("byCount",			(int)pTableData->byCount,		"Quest Probability Table (no XML column)");

			for (int i = 0; i < NTL_QUEST_PROBABILITY_MAX_COUNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Quest Probability Table [Slot %d]"), i + 1);

				ADD_SUB_ITEM("byType",		(int)pTableData->asProbabilityData[i].byType,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("tblidx",		pTableData->asProbabilityData[i].tblidx,		(LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwMinValue",	pTableData->asProbabilityData[i].dwMinValue,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwMaxValue",	pTableData->asProbabilityData[i].dwMaxValue,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwRate",		pTableData->asProbabilityData[i].dwRate,		(LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_WORLD_PLAY:
		{
			sWORLDPLAY_TBLDAT* pTableData = (sWORLDPLAY_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"World Play Table");
			ADD_SUB_ITEM("dwGroup",				pTableData->dwGroup,				"World Play Table (XML column: Group)");
			ADD_SUB_ITEM("byExecuterType",		(int)pTableData->byExecuterType,	"World Play Table (XML column: ExecuterType)");
			ADD_SUB_ITEM("byShareType",			(int)pTableData->byShareType,	"World Play Table (XML column: ShareType)");
			ADD_SUB_ITEM("dwShareLimitTime",	pTableData->dwShareLimitTime,	"World Play Table (XML column: ShareLimitTime)");
		}
		break;

		case CTableContainer::TABLE_HLS_SLOT_MACHINE:
		{
			sHLS_SLOT_MACHINE_TBLDAT* pTableData = (sHLS_SLOT_MACHINE_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Slot Machine Table");
			ADD_SUB_ITEM("dwName",			pTableData->dwName,			"Slot Machine Table (no XML column)");
			ADD_SUB_ITEM("wszNameText",		pTableData->wszNameText,		"Slot Machine Table (no XML column)");
			ADD_SUB_ITEM("szFile_Name",		pTableData->szFile_Name,		"Slot Machine Table (no XML column)");
			ADD_SUB_ITEM("byCoin",			pTableData->byCoin,			"Slot Machine Table (no XML column)");
			ADD_SUB_ITEM("bOnOff",			pTableData->bOnOff,			"Slot Machine Table (no XML column)");
			ADD_SUB_ITEM("byType",			(int)pTableData->byType,		"Slot Machine Table (no XML column)");
			ADD_SUB_ITEM("wfirstWinCoin",	pTableData->wfirstWinCoin,		"Slot Machine Table (no XML column)");

			for (int i = 0; i < (int)DBO_MAX_HLS_SLOT_MACHINES_MAX_ITEMS; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Slot Machine Table [Item %d] (no XML column)"), i + 1);

				ADD_SUB_ITEM("aItemTblidx",	pTableData->aItemTblidx[i],		(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byStack",		(int)pTableData->byStack[i],	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("wQuantity",	pTableData->wQuantity[i],		(LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_HLS_SLOT_MACHINE_ITEM:
		{
			sHLS_SLOT_MACHINE_ITEM_TBLDAT* pTableData = (sHLS_SLOT_MACHINE_ITEM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Slot Machine Item Table");
			ADD_SUB_ITEM("wszNameText",			pTableData->wszNameText,			"Slot Machine Item Table (no XML column)");
			ADD_SUB_ITEM("bActive",				pTableData->bActive,				"Slot Machine Item Table (no XML column)");
			ADD_SUB_ITEM("slotMachineTblidx",	pTableData->slotMachineTblidx,	"Slot Machine Item Table (no XML column)");
			ADD_SUB_ITEM("cashItemTblidx",		pTableData->cashItemTblidx,		"Slot Machine Item Table (no XML column)");
			ADD_SUB_ITEM("byStackCount",		(int)pTableData->byStackCount,	"Slot Machine Item Table (no XML column)");
			ADD_SUB_ITEM("byPercent",			(int)pTableData->byPercent,		"Slot Machine Item Table (no XML column)");
		}
		break;

		case CTableContainer::TABLE_ITEM_UPGRADE_RATE_NEW:
		{
			sITEM_UPGRADE_RATE_NEW_TBLDAT* pTableData = (sITEM_UPGRADE_RATE_NEW_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,						"Item Upgrade Rate Table");
			ADD_SUB_ITEM("byItem_Type",					(int)pTableData->byItem_Type,				"Item Upgrade Rate Table (no XML column)");
			ADD_SUB_ITEM("byGrade",						(int)pTableData->byGrade,					"Item Upgrade Rate Table (no XML column)");
			ADD_SUB_ITEM("fAdditional_Ability",			pTableData->fAdditional_Ability,			"Item Upgrade Rate Table (no XML column)");
			ADD_SUB_ITEM("fUpgrade_Destroy_Rate",		pTableData->fUpgrade_Destroy_Rate,			"Item Upgrade Rate Table (no XML column)");
			ADD_SUB_ITEM("fUpgrade_Success_Basic_Value",pTableData->fUpgrade_Success_Basic_Value,	"Item Upgrade Rate Table (no XML column)");
			ADD_SUB_ITEM("fUpgrade_Success_Stone_Value",pTableData->fUpgrade_Success_Stone_Value,	"Item Upgrade Rate Table (no XML column)");
			ADD_SUB_ITEM("fUpgrade_RateStone_Value1",	pTableData->fUpgrade_RateStone_Value1,		"Item Upgrade Rate Table (no XML column)");
			ADD_SUB_ITEM("fUpgrade_RateStone_Value2",	pTableData->fUpgrade_RateStone_Value2,		"Item Upgrade Rate Table (no XML column)");
		}
		break;

		case CTableContainer::TABLE_ITEM_BAG_LIST:
		{
			sITEM_BAG_LIST_TBLDAT* pTableData = (sITEM_BAG_LIST_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"Item Bag List Table");
			ADD_SUB_ITEM("wszName",			pTableData->wszName,			"Item Bag List Table (no XML column)");
			ADD_SUB_ITEM("byLevel",			(int)pTableData->byLevel,		"Item Bag List Table (no XML column)");
			ADD_SUB_ITEM("bEnchant_Able",	pTableData->bEnchant_Able,		"Item Bag List Table (no XML column)");
			ADD_SUB_ITEM("dwItemCount",		pTableData->dwItemCount,		"Item Bag List Table (no XML column)");
			ADD_SUB_ITEM("dwTotalProb",		pTableData->dwTotalProb,		"Item Bag List Table (no XML column)");

			for (int i = 0; i < (int)DBO_MAX_ITEM_BAG_LIST; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Item Bag List Table [Slot %d] (no XML column)"), i + 1);

				ADD_SUB_ITEM("aItem",	pTableData->aItem[i],	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("adwProb",	pTableData->adwProb[i],	(LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_ITEM_GROUP_LIST:
		{
			sITEM_GROUP_LIST_TBLDAT* pTableData = (sITEM_GROUP_LIST_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Item Group List Table");
			ADD_SUB_ITEM("wszName",				pTableData->wszName,				"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("byLevel",				(int)pTableData->byLevel,			"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("byTry_Count",			(int)pTableData->byTry_Count,		"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("mob_Index",			pTableData->mob_Index,				"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwMob_Type",			pTableData->dwMob_Type,				"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwWorld_Rule_Type",	pTableData->dwWorld_Rule_Type,		"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwInterval",			pTableData->dwInterval,				"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwSuperior",			pTableData->dwSuperior,				"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwExcellent",			pTableData->dwExcellent,			"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwRare",				pTableData->dwRare,					"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwLegendary",			pTableData->dwLegendary,			"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwNo_Drop",			pTableData->dwNo_Drop,				"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwZenny",				pTableData->dwZenny,				"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwItemBagCount",		pTableData->dwItemBagCount,			"Item Group List Table (no XML column)");
			ADD_SUB_ITEM("dwTotalProb",			pTableData->dwTotalProb,			"Item Group List Table (no XML column)");

			for (int i = 0; i < (int)DBO_MAX_ITEM_GROUP_LIST; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Item Group List Table [Slot %d] (no XML column)"), i + 1);

				ADD_SUB_ITEM("aItemBag",	pTableData->aItemBag[i],	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("adwProb",		pTableData->adwProb[i],		(LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_MOB_SERVER:
		{
			sMOB_SERVER_TBLDAT* pTableData = (sMOB_SERVER_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,			"Mob Server Table");
			ADD_SUB_ITEM("bValidity_Able",		pTableData->bValidity_Able,		"Mob Server Table (no XML column)");
			ADD_SUB_ITEM("dwServerBitFlag",		pTableData->dwServerBitFlag,	"Mob Server Table (hex, no XML column)");
		}
		break;

		case CTableContainer::TABLE_DRAGONBALL_RETURN_POINT:
		{
			sDRAGONBALL_RETURN_POINT_TBLDAT* pTableData = (sDRAGONBALL_RETURN_POINT_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"DragonBall Return Point Table");
			ADD_SUB_ITEM("byScatterPoint",	(int)pTableData->byScatterPoint,"DragonBall Return Point Table (no XML column)");
			ADD_SUB_ITEM("fField_X",		pTableData->fField_X,			"DragonBall Return Point Table (no XML column)");
			ADD_SUB_ITEM("fField_Y",		pTableData->fField_Y,			"DragonBall Return Point Table (no XML column)");
			ADD_SUB_ITEM("fField_Z",		pTableData->fField_Z,			"DragonBall Return Point Table (no XML column)");
		}
		break;

		case CTableContainer::TABLE_EVENT_SYSTEM:
		{
			sEVENT_SYSTEM_TBLDAT* pTableData = (sEVENT_SYSTEM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,						"Event System Table");
			ADD_SUB_ITEM("wszName",						pTableData->wszName,						"Event System Table (no XML column)");
			ADD_SUB_ITEM("bOnOff",						pTableData->bOnOff,							"Event System Table (no XML column)");
			ADD_SUB_ITEM("byServerFarm",				(int)pTableData->byServerFarm,				"Event System Table (no XML column)");
			ADD_SUB_ITEM("dwConnectionTime",			pTableData->dwConnectionTime,				"Event System Table (no XML column)");
			ADD_SUB_ITEM("byType",						(int)pTableData->byType,					"Event System Table (no XML column)");
			ADD_SUB_ITEM("tIndex",						pTableData->tIndex,							"Event System Table (no XML column)");
			ADD_SUB_ITEM("dwContentRestrictionBitFlag",	pTableData->dwContentRestrictionBitFlag,	"Event System Table (hex, no XML column)");

			for (int i = 0; i < 3; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Event System Table [Setting %d] (no XML column)"), i + 1);
				ADD_SUB_ITEM("adwSetting", pTableData->adwSetting[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("fRate",		pTableData->fRate,		"Event System Table (no XML column)");
			ADD_SUB_ITEM("byAction",	(int)pTableData->byAction,"Event System Table (no XML column)");
			ADD_SUB_ITEM("aIndex",		pTableData->aIndex,		"Event System Table (no XML column)");
			ADD_SUB_ITEM("byGroup",		(int)pTableData->byGroup,	"Event System Table (no XML column)");
			ADD_SUB_ITEM("dwVolume",	pTableData->dwVolume,		"Event System Table (no XML column)");
			ADD_SUB_ITEM("dwUnknown",	pTableData->dwUnknown,		"Event System Table (no XML column)");
		}
		break;

		case CTableContainer::TABLE_DYNAMIC_FIELD_SYSTEM:
		{
			sDYNAMIC_FIELD_SYSTEM_TBLDAT* pTableData = (sDYNAMIC_FIELD_SYSTEM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,						"Dynamic Field System Table");
			ADD_SUB_ITEM("wszName",						pTableData->wszName,						"Dynamic Field System Table (no XML column)");
			ADD_SUB_ITEM("bOnOff",						pTableData->bOnOff,							"Dynamic Field System Table (no XML column)");
			ADD_SUB_ITEM("byServerFarm",				(int)pTableData->byServerFarm,				"Dynamic Field System Table (no XML column)");
			ADD_SUB_ITEM("dwConnectionTime",			pTableData->dwConnectionTime,				"Dynamic Field System Table (no XML column)");
			ADD_SUB_ITEM("byType",						(int)pTableData->byType,					"Dynamic Field System Table (no XML column)");
			ADD_SUB_ITEM("tIndex",						pTableData->tIndex,							"Dynamic Field System Table (no XML column)");
			ADD_SUB_ITEM("dwContentRestrictionBitFlag",	pTableData->dwContentRestrictionBitFlag,	"Dynamic Field System Table (hex, no XML column)");

			for (int i = 0; i < 3; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Dynamic Field System Table [Setting %d] (no XML column)"), i + 1);
				ADD_SUB_ITEM("adwSetting", pTableData->adwSetting[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("fRate",		pTableData->fRate,		"Dynamic Field System Table (no XML column)");
			ADD_SUB_ITEM("byAction",	(int)pTableData->byAction,"Dynamic Field System Table (no XML column)");
			ADD_SUB_ITEM("aIndex",		pTableData->aIndex,		"Dynamic Field System Table (no XML column)");
			ADD_SUB_ITEM("byGroup",		(int)pTableData->byGroup,	"Dynamic Field System Table (no XML column)");
			ADD_SUB_ITEM("dwVolume",	pTableData->dwVolume,		"Dynamic Field System Table (no XML column)");
		}
		break;

		// Spawn tab rows (see CClassView::LoadSpawnTable/Util.h's
		// SPAWN_ROW_NPC_OR_MOB/SPAWN_ROW_OBJECT) -- neither has a
		// CTableContainer::eTABLE of its own, so these sentinel nTableType
		// values route here the same way a real eTABLE would.
		case SPAWN_ROW_NPC_OR_MOB:
		{
			sSPAWN_TBLDAT* pTableData = (sSPAWN_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Spawn Table");
			ADD_SUB_ITEM("mob_Tblidx",			pTableData->mob_Tblidx,				"Spawn Table");
			ADD_SUB_ITEM("vSpawn_Loc.x",		pTableData->vSpawn_Loc.x,			"Spawn Table");
			ADD_SUB_ITEM("vSpawn_Loc.y",		pTableData->vSpawn_Loc.y,			"Spawn Table");
			ADD_SUB_ITEM("vSpawn_Loc.z",		pTableData->vSpawn_Loc.z,			"Spawn Table");
			ADD_SUB_ITEM("vSpawn_Dir.x",		pTableData->vSpawn_Dir.x,			"Spawn Table");
			ADD_SUB_ITEM("vSpawn_Dir.y",		pTableData->vSpawn_Dir.y,			"Spawn Table");
			ADD_SUB_ITEM("vSpawn_Dir.z",		pTableData->vSpawn_Dir.z,			"Spawn Table");
			ADD_SUB_ITEM("bySpawn_Loc_Range",	(int)pTableData->bySpawn_Loc_Range,	"Spawn Table");
			ADD_SUB_ITEM("bySpawn_Quantity",	(int)pTableData->bySpawn_Quantity,	"Spawn Table");
			ADD_SUB_ITEM("wSpawn_Cool_Time",	pTableData->wSpawn_Cool_Time,		"Spawn Table");
			ADD_SUB_ITEM("bySpawn_Move_Type",	(int)pTableData->bySpawn_Move_Type,	"Spawn Table");
			ADD_SUB_ITEM("byWander_Range",		(int)pTableData->byWander_Range,	"Spawn Table");
			ADD_SUB_ITEM("byMove_Range",		(int)pTableData->byMove_Range,		"Spawn Table");
			ADD_SUB_ITEM("actionPatternTblidx",	(int)pTableData->actionPatternTblidx,"Spawn Table");
			ADD_SUB_ITEM("path_Table_Index",	pTableData->path_Table_Index,		"Spawn Table");
			ADD_SUB_ITEM("vFollowDistance.x",	pTableData->vFollowDistance.x,		"Spawn Table");
			ADD_SUB_ITEM("vFollowDistance.y",	pTableData->vFollowDistance.y,		"Spawn Table");
			ADD_SUB_ITEM("vFollowDistance.z",	pTableData->vFollowDistance.z,		"Spawn Table");
			ADD_SUB_ITEM("playScript",			pTableData->playScript,				"Spawn Table");
			ADD_SUB_ITEM("playScriptScene",		pTableData->playScriptScene,		"Spawn Table");
			ADD_SUB_ITEM("aiScript",			pTableData->aiScript,				"Spawn Table");
			ADD_SUB_ITEM("aiScriptScene",		pTableData->aiScriptScene,			"Spawn Table");
			ADD_SUB_ITEM("dwParty_Index",		pTableData->dwParty_Index,			"Spawn Table");
			ADD_SUB_ITEM("bParty_Leader",		pTableData->bParty_Leader,			"Spawn Table");
			ADD_SUB_ITEM("spawnGroupId",		pTableData->spawnGroupId,			"Spawn Table");
		}
		break;

		case SPAWN_ROW_OBJECT:
		{
			sOBJECT_TBLDAT* pTableData = (sOBJECT_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Object Spawn Table");
			ADD_SUB_ITEM("dwName",				pTableData->dwName,					"Object Spawn Table");
			ADD_SUB_ITEM("dwFunction",			pTableData->dwFunction,				"Object Spawn Table");
			ADD_SUB_ITEM("vMin.x",				pTableData->vMin.x,					"Object Spawn Table");
			ADD_SUB_ITEM("vMin.y",				pTableData->vMin.y,					"Object Spawn Table");
			ADD_SUB_ITEM("vMin.z",				pTableData->vMin.z,					"Object Spawn Table");
			ADD_SUB_ITEM("vMax.x",				pTableData->vMax.x,					"Object Spawn Table");
			ADD_SUB_ITEM("vMax.y",				pTableData->vMax.y,					"Object Spawn Table");
			ADD_SUB_ITEM("vMax.z",				pTableData->vMax.z,					"Object Spawn Table");
			ADD_SUB_ITEM("vLoc.x",				pTableData->vLoc.x,					"Object Spawn Table");
			ADD_SUB_ITEM("vLoc.y",				pTableData->vLoc.y,					"Object Spawn Table");
			ADD_SUB_ITEM("vLoc.z",				pTableData->vLoc.z,					"Object Spawn Table");
			ADD_SUB_ITEM("vDir.x",				pTableData->vDir.x,					"Object Spawn Table");
			ADD_SUB_ITEM("vDir.y",				pTableData->vDir.y,					"Object Spawn Table");
			ADD_SUB_ITEM("vDir.z",				pTableData->vDir.z,					"Object Spawn Table");
			ADD_SUB_ITEM("byStateType",			(int)pTableData->byStateType,		"Object Spawn Table");
			ADD_SUB_ITEM("byDefMainState",		(int)pTableData->byDefMainState,	"Object Spawn Table");
			ADD_SUB_ITEM("byDefSubState",		(int)pTableData->byDefSubState,		"Object Spawn Table");

			for (int i = 0; i < (int)DBO_MAX_OBJECT_STATE; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Object Spawn Table [State %d]"), i + 1);

				ADD_SUB_ITEM("abyState", JoinBytes(pTableData->abyState[i], DBO_MAX_OBJECT_STATE_ANIMATION), (LPCTSTR)strGroup);
				ADD_SUB_ITEM("achClickSound", CString(pTableData->achClickSound[i]), (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("szModelName",			pTableData->szModelName,			"Object Spawn Table");
			ADD_SUB_ITEM("fRadius",				pTableData->fRadius,				"Object Spawn Table");
			ADD_SUB_ITEM("dwSequence",			pTableData->dwSequence,				"Object Spawn Table");
			ADD_SUB_ITEM("contentsTblidx",		pTableData->contentsTblidx,			"Object Spawn Table");
			ADD_SUB_ITEM("objectDirectionIndex",pTableData->objectDirectionIndex,	"Object Spawn Table");
			ADD_SUB_ITEM("byBoundaryDistance",	(int)pTableData->byBoundaryDistance,"Object Spawn Table");
			ADD_SUB_ITEM("minQuestId",			pTableData->minQuestId,				"Object Spawn Table");
			ADD_SUB_ITEM("maxQuestId",			pTableData->maxQuestId,				"Object Spawn Table");
			ADD_SUB_ITEM("byWarfog",			(int)pTableData->byWarfog,			"Object Spawn Table");
			ADD_SUB_ITEM("ZoneId",				pTableData->ZoneId,					"Object Spawn Table");
			ADD_SUB_ITEM("WorldId",				pTableData->WorldId,				"Object Spawn Table");
		}
		break;

		case CTableContainer::TABLE_SPEECH:
		{
			sNPC_SPEECH_TBLDAT* pTableData = (sNPC_SPEECH_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,				"Speech Table");
			ADD_SUB_ITEM("dwDialogGroup",	pTableData->dwDialogGroup,			"Speech Table");
			ADD_SUB_ITEM("szDialogType",	pTableData->szDialogType,			"Speech Table");
			ADD_SUB_ITEM("byRate",			(int)pTableData->byRate,			"Speech Table");
			ADD_SUB_ITEM("textIndex",		pTableData->textIndex,				"Speech Table (XML column: Text_Index)");
			ADD_SUB_ITEM("byBallonType",	(int)pTableData->byBallonType,		"Speech Table");
			ADD_SUB_ITEM("dwDisplayTime",	pTableData->dwDisplayTime,			"Speech Table");
			ADD_SUB_ITEM("szNote",			pTableData->szNote,					"Speech Table");
			ADD_SUB_ITEM("bySpeechType",	(int)pTableData->bySpeechType,		"Speech Table (no XML column: set during load, not from XML)");
		}
		break;

		case CTableContainer::TABLE_HTB_SET:
		{
			sHTB_SET_TBLDAT* pTableData = (sHTB_SET_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",					pTableData->tblidx,					"HtbSet Table");
			ADD_SUB_ITEM("wszNameText",				pTableData->wszNameText,				"HtbSet Table (XML column: Name_Text)");
			ADD_SUB_ITEM("HTB_Skill_Name",			pTableData->HTB_Skill_Name,			"HtbSet Table");
			ADD_SUB_ITEM("bValidity_Able",			pTableData->bValidity_Able,			"HtbSet Table (XML column: Validity_Able)");
			ADD_SUB_ITEM("dwPC_Class_Bit_Flag",		pTableData->dwPC_Class_Bit_Flag,		"HtbSet Table (hex bitflag)");
			ADD_SUB_ITEM("bySlot_Index",			(int)pTableData->bySlot_Index,			"HtbSet Table");
			ADD_SUB_ITEM("bySkill_Grade",			(int)pTableData->bySkill_Grade,		"HtbSet Table");
			ADD_SUB_ITEM("szIcon_Name",				pTableData->szIcon_Name,				"HtbSet Table (XML column: Icon_Name)");
			ADD_SUB_ITEM("wNeed_EP",				pTableData->wNeed_EP,					"HtbSet Table");
			ADD_SUB_ITEM("byRequire_Train_Level",	(int)pTableData->byRequire_Train_Level,"HtbSet Table");
			ADD_SUB_ITEM("dwRequire_Zenny",			pTableData->dwRequire_Zenny,			"HtbSet Table");
			ADD_SUB_ITEM("wNext_Skill_Train_Exp",	pTableData->wNext_Skill_Train_Exp,		"HtbSet Table");
			ADD_SUB_ITEM("wCool_Time",				pTableData->wCool_Time,					"HtbSet Table");
			ADD_SUB_ITEM("dwCoolTimeInMilliSecs",	pTableData->dwCoolTimeInMilliSecs,		"HtbSet Table (no XML column)");
			ADD_SUB_ITEM("Note",					pTableData->Note,						"HtbSet Table");
			ADD_SUB_ITEM("bySetCount",				(int)pTableData->bySetCount,			"HtbSet Table (XML column: Set_Count)");
			ADD_SUB_ITEM("byStop_Point",			(int)pTableData->byStop_Point,			"HtbSet Table");

			for (int i = 0; i < (int)NTL_HTB_MAX_SKILL_COUNT_IN_SET; i++)
			{
				CString strGroup;
				strGroup.Format(_T("HtbSet Table [Action Slot %d]"), i + 1);

				ADD_SUB_ITEM("bySkillType",	(int)pTableData->aHTBAction[i].bySkillType,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("skillTblidx",	pTableData->aHTBAction[i].skillTblidx,			(LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("wRequireSP",				pTableData->wRequireSP,				"HtbSet Table (XML column: Require_SP)");
		}
		break;

		case CTableContainer::TABLE_DIRECTION_LINK:
		{
			sDIRECTION_LINK_TBLDAT* pTableData = (sDIRECTION_LINK_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,			"DirectionLink Table");
			ADD_SUB_ITEM("szFunctionName",	pTableData->szFunctionName,	"DirectionLink Table (XML column: Function_Name)");
			ADD_SUB_ITEM("szNote",			pTableData->szNote,			"DirectionLink Table");
			ADD_SUB_ITEM("byType",			(int)pTableData->byType,		"DirectionLink Table");
			ADD_SUB_ITEM("dwAnimationID",	pTableData->dwAnimationID,		"DirectionLink Table (XML column: Animation_ID)");
			ADD_SUB_ITEM("byFuncFlag",		(int)pTableData->byFuncFlag,	"DirectionLink Table (XML column: Direction_Func_Flag, hex bitflag)");
		}
		break;

		case CTableContainer::TABLE_DOJO:
		{
			sDOJO_TBLDAT* pTableData = (sDOJO_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,					"Dojo Table");
			ADD_SUB_ITEM("zoneTblidx",			pTableData->zoneTblidx,				"Dojo Table");

			for (int i = 0; i < (int)DOJO_MAX_UPGRADE_OBJECT_COUNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Dojo Table [Object Slot %d]"), i + 1);

				ADD_SUB_ITEM("objectTblidx", pTableData->objectTblidx[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("mapName",				pTableData->mapName,				"Dojo Table (XML column: Map_Name)");
			ADD_SUB_ITEM("byReceiveHour",		(int)pTableData->byReceiveHour,		"Dojo Table");
			ADD_SUB_ITEM("byReceiveMinute",		(int)pTableData->byReceiveMinute,	"Dojo Table");
			ADD_SUB_ITEM("byRepeatType",			(int)pTableData->byRepeatType,		"Dojo Table");
			ADD_SUB_ITEM("byRepeatTime",			(int)pTableData->byRepeatTime,		"Dojo Table");
			ADD_SUB_ITEM("wWeekBitFlag",			pTableData->wWeekBitFlag,			"Dojo Table (hex bitflag)");
			ADD_SUB_ITEM("byReceiveDuration",	(int)pTableData->byReceiveDuration,	"Dojo Table");
			ADD_SUB_ITEM("byRejectDuration",		(int)pTableData->byRejectDuration,	"Dojo Table");
			ADD_SUB_ITEM("byStandbyDuration",	(int)pTableData->byStandbyDuration,	"Dojo Table");
			ADD_SUB_ITEM("byInitialDuration",	(int)pTableData->byInitialDuration,	"Dojo Table");
			ADD_SUB_ITEM("byReadyDuration",		(int)pTableData->byReadyDuration,	"Dojo Table");
			ADD_SUB_ITEM("byBattleDuration",		(int)pTableData->byBattleDuration,	"Dojo Table");
			ADD_SUB_ITEM("dwReceivePoint",		pTableData->dwReceivePoint,			"Dojo Table");
			ADD_SUB_ITEM("dwReceiveZenny",		pTableData->dwReceiveZenny,			"Dojo Table (XML column: Receive_Zeny)");
			ADD_SUB_ITEM("controllerTblidx",		pTableData->controllerTblidx,		"Dojo Table");
			ADD_SUB_ITEM("dwBattlePointGoal",	pTableData->dwBattlePointGoal,		"Dojo Table");
			ADD_SUB_ITEM("dwBattlePointGet",		pTableData->dwBattlePointGet,		"Dojo Table");
			ADD_SUB_ITEM("dwBattlePointCharge",	pTableData->dwBattlePointCharge,	"Dojo Table");
			ADD_SUB_ITEM("dwChargePointGoal",	pTableData->dwChargePointGoal,		"Dojo Table");
			ADD_SUB_ITEM("dwChargeTime",			pTableData->dwChargeTime,			"Dojo Table");
			ADD_SUB_ITEM("dwChageTimePoint",		pTableData->dwChageTimePoint,		"Dojo Table (XML column: Charge_Time_Point)");
			ADD_SUB_ITEM("rockTblidx",			pTableData->rockTblidx,				"Dojo Table");

			for (int i = 0; i < (int)DOJO_MAX_REWARD_TYPE_COUNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Dojo Table [Reward Slot %d]"), i + 1);

				ADD_SUB_ITEM("dwGetPoint",	pTableData->asRawrd[i].dwGetPoint,		(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byGetRock",	(int)pTableData->asRawrd[i].byGetRock,	(LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_MOB_MOVE_PATTERN:
		{
			sMOVE_PATTERN_TBLDAT* pTableData = (sMOVE_PATTERN_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx", pTableData->tblidx, "MobMovePattern Table");

			for (int i = 0; i < (int)DBO_MAX_COUNT_MOVE_PATTERN; i++)
			{
				CString strGroup;
				strGroup.Format(_T("MobMovePattern Table [Slot %d]"), i + 1);

				ADD_SUB_ITEM("abyPattern", (int)pTableData->abyPattern[i], (LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_LAND_MARK:
		{
			sLAND_MARK_TBLDAT* pTableData = (sLAND_MARK_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,							"LandMark Table");
			ADD_SUB_ITEM("wszNameText",					pTableData->wszNameText,					"LandMark Table (XML column: Name_Text)");
			ADD_SUB_ITEM("LandmarkName",				pTableData->LandmarkName,					"LandMark Table (XML column: Landmark_Name)");
			ADD_SUB_ITEM("byLandmarkType",				(int)pTableData->byLandmarkType,			"LandMark Table (XML column: Landmark_Type)");
			ADD_SUB_ITEM("bValidityAble",				pTableData->bValidityAble,					"LandMark Table (XML column: Validity_Able)");
			ADD_SUB_ITEM("byLandmarkBitflag",			(int)pTableData->byLandmarkBitflag,			"LandMark Table (XML column: Landmark_BitFlag)");
			ADD_SUB_ITEM("byLandmarkDisplayBitFlag",	(int)pTableData->byLandmarkDisplayBitFlag,	"LandMark Table (XML column: Landmark_Display_BitFlag)");
			ADD_SUB_ITEM("LandmarkLoc.x",				pTableData->LandmarkLoc.x,					"LandMark Table (XML column: Landmark_Loc_X)");
			ADD_SUB_ITEM("LandmarkLoc.y",				pTableData->LandmarkLoc.y,					"LandMark Table (no XML column)");
			ADD_SUB_ITEM("LandmarkLoc.z",				pTableData->LandmarkLoc.z,					"LandMark Table (XML column: Landmark_Loc_Z)");
			ADD_SUB_ITEM("LinkMapIdx",					pTableData->LinkMapIdx,						"LandMark Table (XML column: Link_Map_Idx)");
			ADD_SUB_ITEM("ZoneMapIdx",					pTableData->ZoneMapIdx,						"LandMark Table (XML column: Zone_Map_Idx)");
			ADD_SUB_ITEM("wLinkWarfogIdx",				pTableData->wLinkWarfogIdx,					"LandMark Table (XML column: Link_Warfog_Idx)");
			ADD_SUB_ITEM("wszIconName",					pTableData->wszIconName,					"LandMark Table (XML column: Icon_Name)");
			ADD_SUB_ITEM("byIconSize",					(int)pTableData->byIconSize,				"LandMark Table (XML column: Icon_Size)");
			ADD_SUB_ITEM("Note",						pTableData->Note,							"LandMark Table");
		}
		break;

		case CTableContainer::TABLE_AIR_COSTUME:
		{
			sAIR_COSTUME_TBLDAT* pTableData = (sAIR_COSTUME_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",			pTableData->tblidx,				"AirCostume Table");
			ADD_SUB_ITEM("wUnknown",		pTableData->wUnknown,				"AirCostume Table (no XML column, binary-only; Tblidx-only reader)");
			ADD_SUB_ITEM("byUnknown2",		(int)pTableData->byUnknown2,		"AirCostume Table (no XML column, binary-only)");
			ADD_SUB_ITEM("byUnknown3",		(int)pTableData->byUnknown3,		"AirCostume Table (no XML column, binary-only)");
			ADD_SUB_ITEM("byUnknown4",		(int)pTableData->byUnknown4,		"AirCostume Table (no XML column, binary-only)");
			ADD_SUB_ITEM("wUnknown5",		pTableData->wUnknown5,				"AirCostume Table (no XML column, binary-only)");
			ADD_SUB_ITEM("wUnknown6",		pTableData->wUnknown6,				"AirCostume Table (no XML column, binary-only)");
			ADD_SUB_ITEM("wUnknown7",		pTableData->wUnknown7,				"AirCostume Table (no XML column, binary-only)");
			ADD_SUB_ITEM("wszUnknown8",		pTableData->wszUnknown8,			"AirCostume Table (no XML column, binary-only)");
			ADD_SUB_ITEM("byUnknown9",		(int)pTableData->byUnknown9,		"AirCostume Table (no XML column, binary-only)");
			ADD_SUB_ITEM("wszUnknown10",	pTableData->wszUnknown10,			"AirCostume Table (no XML column, binary-only)");
			ADD_SUB_ITEM("wszUnknown11",	pTableData->wszUnknown11,			"AirCostume Table (no XML column, binary-only)");
		}
		break;

		case CTableContainer::TABLE_QUEST_TEXT_DATA:
		{
			sQUEST_TEXT_DATA_TBLDAT* pTableData = (sQUEST_TEXT_DATA_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",		pTableData->tblidx,				"QuestText Table (XML column: Quest_Text_Index)");
			ADD_SUB_ITEM("wstrText",	pTableData->wstrText.c_str(),		"QuestText Table (XML column: Quest_Text)");
		}
		break;

		case CTableContainer::TABLE_TIMEQUEST:
		{
			sTIMEQUEST_TBLDAT* pTableData = (sTIMEQUEST_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,							"TimeQuest Table");
			ADD_SUB_ITEM("byTimeQuestType",				(int)pTableData->byTimeQuestType,			"TimeQuest Table (XML column: Type)");
			ADD_SUB_ITEM("byDifficultyFlag",			pTableData->byDifficultyFlag,				"TimeQuest Table (XML column: Difficultyflag, hex bitflag)");
			ADD_SUB_ITEM("dwStartTime",					pTableData->dwStartTime,						"TimeQuest Table (XML column: StartTime)");
			ADD_SUB_ITEM("startCharacterDirection",		pTableData->startCharacterDirection,		"TimeQuest Table");
			ADD_SUB_ITEM("startObjectIndex",			pTableData->startObjectIndex,				"TimeQuest Table");
			ADD_SUB_ITEM("startTriggerId",				pTableData->startTriggerId,					"TimeQuest Table");

			for (int i = 0; i < 10; i++)
			{
				CString strGroup;
				strGroup.Format(_T("TimeQuest Table [Start Trigger Direction State %d]"), i + 1);

				ADD_SUB_ITEM("abyStartTriggerDirectionState", (int)pTableData->abyStartTriggerDirectionState[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("arriveCharacterDirection",	pTableData->arriveCharacterDirection,		"TimeQuest Table");
			ADD_SUB_ITEM("arriveObjectIndex",			pTableData->arriveObjectIndex,				"TimeQuest Table");
			ADD_SUB_ITEM("arriveTriggerId",				pTableData->arriveTriggerId,					"TimeQuest Table");
			ADD_SUB_ITEM("leaveCharacterDirection",		pTableData->leaveCharacterDirection,		"TimeQuest Table");
			ADD_SUB_ITEM("leaveObjectIndex",			pTableData->leaveObjectIndex,				"TimeQuest Table");
			ADD_SUB_ITEM("leaveTriggerId",				pTableData->leaveTriggerId,					"TimeQuest Table");
			ADD_SUB_ITEM("dayRecordMailTblidx",			pTableData->dayRecordMailTblidx,			"TimeQuest Table (XML column: DayRecord_MailIndex)");
			ADD_SUB_ITEM("bestRecordMailTblidx",		pTableData->bestRecordMailTblidx,			"TimeQuest Table (XML column: BestRecord_MailIndex)");
			ADD_SUB_ITEM("byResetTime",					(int)pTableData->byResetTime,				"TimeQuest Table (XML column: Reset_Time)");
			ADD_SUB_ITEM("wszPrologueDirection",		pTableData->wszPrologueDirection,			"TimeQuest Table (XML column: Prologue_Direction)");
			ADD_SUB_ITEM("openCine",					pTableData->openCine,						"TimeQuest Table (XML column: Open_Cine)");
			ADD_SUB_ITEM("Note",						pTableData->Note,							"TimeQuest Table");
			ADD_SUB_ITEM("wszStageBgm1",				pTableData->wszStageBgm1,					"TimeQuest Table (XML column: Stage_BGM1)");
			ADD_SUB_ITEM("wszStageBgm2",				pTableData->wszStageBgm2,					"TimeQuest Table (XML column: Stage_BGM2)");
			ADD_SUB_ITEM("wszLastBgm",					pTableData->wszLastBgm,						"TimeQuest Table (XML column: Last_BGM)");

			for (int i = 0; i < (int)MAX_TIMEQUEST_DIFFICULTY; i++)
			{
				CString strGroup;
				strGroup.Format(_T("TimeQuest Table [Difficulty Slot %d] (XML columns end in _Easy/_Normal/_Hard)"), i + 1);

				ADD_SUB_ITEM("nameTblidx",				pTableData->sTimeQuestDataset[i].nameTblidx,				(LPCTSTR)strGroup);
				ADD_SUB_ITEM("questStringTblidx",		pTableData->sTimeQuestDataset[i].questStringTblidx,		(LPCTSTR)strGroup);
				ADD_SUB_ITEM("worldTblidx",				pTableData->sTimeQuestDataset[i].worldTblidx,				(LPCTSTR)strGroup);
				ADD_SUB_ITEM("scriptTblidx",			pTableData->sTimeQuestDataset[i].scriptTblidx,				(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byMinMemberCount",		(int)pTableData->sTimeQuestDataset[i].byMinMemberCount,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byMaxMemberCount",		(int)pTableData->sTimeQuestDataset[i].byMaxMemberCount,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byMinMemberLevel",		(int)pTableData->sTimeQuestDataset[i].byMinMemberLevel,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byMaxMemberLevel",		(int)pTableData->sTimeQuestDataset[i].byMaxMemberLevel,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwLimitTime",				pTableData->sTimeQuestDataset[i].dwLimitTime,				(LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwNeedZenny",				pTableData->sTimeQuestDataset[i].dwNeedZenny,				(LPCTSTR)strGroup);
				ADD_SUB_ITEM("needItemTblidx",			pTableData->sTimeQuestDataset[i].needItemTblidx,			(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byNeedLimitCount",		(int)pTableData->sTimeQuestDataset[i].byNeedLimitCount,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byWorldCount",			(int)pTableData->sTimeQuestDataset[i].byWorldCount,		(LPCTSTR)strGroup);
				ADD_SUB_ITEM("dayRecordRewardTblidx",	pTableData->sTimeQuestDataset[i].dayRecordRewardTblidx,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("bestRecordRewardTblidx",	pTableData->sTimeQuestDataset[i].bestRecordRewardTblidx,	(LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_BUDOKAI:
		{
			sBUDOKAI_TBLDAT* pTableData = (sBUDOKAI_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",		pTableData->tblidx,				"Budokai Table");
			ADD_SUB_ITEM("wstrName",	pTableData->wstrName.c_str(),		"Budokai Table (XML column: Name)");

			for (int i = 0; i < (int)BUDOKAI_MAX_TBLDAT_VALUE_COUNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Budokai Table [Value Slot %d]"), i + 1);

				ADD_SUB_ITEM("wstrValue", pTableData->wstrValue[i].c_str(), (LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_RANKBATTLE:
		{
			sRANKBATTLE_TBLDAT* pTableData = (sRANKBATTLE_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",					pTableData->tblidx,						"RankBattle Table");
			ADD_SUB_ITEM("byRuleType",				(int)pTableData->byRuleType,				"RankBattle Table (XML column: Rule_Type, mapped via special enum values 1/100/101/102)");
			ADD_SUB_ITEM("byBattleMode",				(int)pTableData->byBattleMode,				"RankBattle Table");
			ADD_SUB_ITEM("byMatchType",				(int)pTableData->byMatchType,				"RankBattle Table (no XML column)");
			ADD_SUB_ITEM("wszName",					pTableData->wszName,						"RankBattle Table (XML column: Name)");
			ADD_SUB_ITEM("worldTblidx",				pTableData->worldTblidx,					"RankBattle Table (XML column: Map_Index)");
			ADD_SUB_ITEM("needItemTblidx",			pTableData->needItemTblidx,				"RankBattle Table (XML column: Need_Item)");
			ADD_SUB_ITEM("dwZenny",					pTableData->dwZenny,						"RankBattle Table (XML column: Need_Zenny)");
			ADD_SUB_ITEM("byMinLevel",				(int)pTableData->byMinLevel,				"RankBattle Table");
			ADD_SUB_ITEM("byMaxLevel",				(int)pTableData->byMaxLevel,				"RankBattle Table");
			ADD_SUB_ITEM("byBattleCount",			(int)pTableData->byBattleCount,			"RankBattle Table");
			ADD_SUB_ITEM("dwWaitTime",				pTableData->dwWaitTime,						"RankBattle Table");
			ADD_SUB_ITEM("dwDirectionTime",			pTableData->dwDirectionTime,				"RankBattle Table");
			ADD_SUB_ITEM("dwMatchReadyTime",		pTableData->dwMatchReadyTime,				"RankBattle Table");
			ADD_SUB_ITEM("dwStageReadyTime",		pTableData->dwStageReadyTime,				"RankBattle Table");
			ADD_SUB_ITEM("dwStageRunTime",			pTableData->dwStageRunTime,					"RankBattle Table");
			ADD_SUB_ITEM("dwStageFinishTime",		pTableData->dwStageFinishTime,				"RankBattle Table");
			ADD_SUB_ITEM("dwMatchFinishTime",		pTableData->dwMatchFinishTime,				"RankBattle Table");
			ADD_SUB_ITEM("dwBossDirectionTime",		pTableData->dwBossDirectionTime,			"RankBattle Table (XML column: BossDirection_Time)");
			ADD_SUB_ITEM("dwBossKillTime",			pTableData->dwBossKillTime,					"RankBattle Table (XML column: BossKill_Time)");
			ADD_SUB_ITEM("dwBossEndingTime",		pTableData->dwBossEndingTime,				"RankBattle Table");
			ADD_SUB_ITEM("dwEndTime",				pTableData->dwEndTime,						"RankBattle Table");
			ADD_SUB_ITEM("chScoreKO",				(int)pTableData->chScoreKO,					"RankBattle Table (XML column: KO_Score)");
			ADD_SUB_ITEM("chScoreOutOfArea",		(int)pTableData->chScoreOutOfArea,			"RankBattle Table (XML column: OutofArea_Score)");
			ADD_SUB_ITEM("chScorePointWin",			(int)pTableData->chScorePointWin,			"RankBattle Table (XML column: Pointwin_Score)");
			ADD_SUB_ITEM("chScoreDraw",				(int)pTableData->chScoreDraw,				"RankBattle Table (XML column: Draw_Score)");
			ADD_SUB_ITEM("chScoreLose",				(int)pTableData->chScoreLose,				"RankBattle Table (XML column: Lost_Score)");
			ADD_SUB_ITEM("chResultExcellent",		(int)pTableData->chResultExcellent,			"RankBattle Table (XML column: Excellent_Result)");
			ADD_SUB_ITEM("chResultGreate",			(int)pTableData->chResultGreate,			"RankBattle Table (XML column: Greate_Result)");
			ADD_SUB_ITEM("chResultGood",				(int)pTableData->chResultGood,				"RankBattle Table (XML column: Good_Result)");
			ADD_SUB_ITEM("chResultDraw",				(int)pTableData->chResultDraw,				"RankBattle Table (XML column: Draw_Result)");
			ADD_SUB_ITEM("chResultLose",				(int)pTableData->chResultLose,				"RankBattle Table (XML column: Lost_Result)");
			ADD_SUB_ITEM("chBonusPerfectWinner",	(int)pTableData->chBonusPerfectWinner,		"RankBattle Table (XML column: PerfectWinner_Score)");
			ADD_SUB_ITEM("chBonusNormalWinner",		(int)pTableData->chBonusNormalWinner,		"RankBattle Table (XML column: NormalWinner_Score)");
			ADD_SUB_ITEM("wszBGMName",				pTableData->wszBGMName,						"RankBattle Table (no XML column)");
			ADD_SUB_ITEM("byDayEntryNum",			(int)pTableData->byDayEntryNum,			"RankBattle Table (XML column: Day_Entry_Num)");
			ADD_SUB_ITEM("bOutSizeAble",				pTableData->bOutSizeAble,					"RankBattle Table (XML column: OutSide_Able)");
			ADD_SUB_ITEM("szCameraName",				pTableData->szCameraName,					"RankBattle Table (no XML column)");
			ADD_SUB_ITEM("dwInfoIndex",				pTableData->dwInfoIndex,					"RankBattle Table (XML column: Info_Index)");
			ADD_SUB_ITEM("dwStateMinClearTime",		pTableData->dwStateMinClearTime,			"RankBattle Table (XML column: StageMinClearTime)");
		}
		break;

		case CTableContainer::TABLE_SCRIPT_LINK:
		{
			sSCRIPT_LINK_TBLDAT* pTableData = (sSCRIPT_LINK_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",		pTableData->tblidx,			"ScriptLink Table");
			ADD_SUB_ITEM("wszValue",	pTableData->wszValue,			"ScriptLink Table (XML column: Value)");
			ADD_SUB_ITEM("byType",		(int)pTableData->byType,		"ScriptLink Table");
			ADD_SUB_ITEM("byAction",	(int)pTableData->byAction,		"ScriptLink Table");
		}
		break;

		case CTableContainer::TABLE_QUEST_NARRATION:
		{
			sQUEST_NARRATION_TBLDAT* pTableData = (sQUEST_NARRATION_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",		pTableData->tblidx,			"QuestNarration Table");
			ADD_SUB_ITEM("bType",		pTableData->bType,				"QuestNarration Table (XML column: Type)");
			ADD_SUB_ITEM("byNumber",	(int)pTableData->byNumber,		"QuestNarration Table (XML column: Number)");
			ADD_SUB_ITEM("byTime",		(int)pTableData->byTime,		"QuestNarration Table (XML column: Time)");

			for (int i = 0; i < (int)DBO_MAX_COUNT_OF_NARRATION; i++)
			{
				CString strGroup;
				strGroup.Format(_T("QuestNarration Table [Slot %d]"), i + 1);

				ADD_SUB_ITEM("byUIShowHideDirection",	(int)pTableData->asData[i].byUIShowHideDirection,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byOwnerType",				(int)pTableData->asData[i].byOwnerType,			(LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwOwner",					pTableData->asData[i].dwOwner,						(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byCondition",				(int)pTableData->asData[i].byCondition,			(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byDirection",				(int)pTableData->asData[i].byDirection,			(LPCTSTR)strGroup);
				ADD_SUB_ITEM("dwDialog",				pTableData->asData[i].dwDialog,					(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byUiType",				(int)pTableData->asData[i].byUiType,				(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byUIDirection",			(int)pTableData->asData[i].byUIDirection,			(LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_QUEST_ITEM:
		{
			sQUESTITEM_TBLDAT* pTableData = (sQUESTITEM_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"QuestItem Table (XML column: Item_Tblidx)");
			ADD_SUB_ITEM("ItemName",			pTableData->ItemName,				"QuestItem Table (XML column: Item_Name)");
			ADD_SUB_ITEM("szIconName",			pTableData->szIconName,				"QuestItem Table (XML column: Icon_Name)");
			ADD_SUB_ITEM("Note",				pTableData->Note,					"QuestItem Table");
			ADD_SUB_ITEM("byFunctionBitFlag",	(int)pTableData->byFunctionBitFlag,"QuestItem Table (XML column: Function_Bit_Flag, hex bitflag)");
		}
		break;

		case CTableContainer::TABLE_ITEM_DISASSEMBLE:
		{
			sITEM_DISASSEMBLE_TBLDAT* pTableData = (sITEM_DISASSEMBLE_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx", pTableData->tblidx, "ItemDisassemble Table (Tblidx-only XML reader)");

			for (int i = 0; i < (int)ITEM_DISASSEMBLE_MAX_RESULT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("ItemDisassemble Table [Result Slot %d] (no XML column, binary-only)"), i + 1);

				ADD_SUB_ITEM("ItemTblidxResult", pTableData->ItemTblidxResult[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("ByMat2Rate", (int)pTableData->ByMat2Rate, "ItemDisassemble Table (no XML column, binary-only)");
			ADD_SUB_ITEM("ByMat3Rate", (int)pTableData->ByMat3Rate, "ItemDisassemble Table (no XML column, binary-only)");
		}
		break;

		case CTableContainer::TABLE_PORTAL:
		{
			sPORTAL_TBLDAT* pTableData = (sPORTAL_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"Portal Table");
			ADD_SUB_ITEM("dwPointName",			pTableData->dwPointName,			"Portal Table (XML column: Point_Name)");
			ADD_SUB_ITEM("szPointNameText",		pTableData->szPointNameText,		"Portal Table (XML column: Point_Name_Text)");
			ADD_SUB_ITEM("dwUnknown",			pTableData->dwUnknown,				"Portal Table (no XML column)");
			ADD_SUB_ITEM("worldId",				pTableData->worldId,				"Portal Table (XML column: World)");
			ADD_SUB_ITEM("byGrade",				(int)pTableData->byGrade,			"Portal Table");
			ADD_SUB_ITEM("vLoc.x",				pTableData->vLoc.x,					"Portal Table (XML column: Loc_X)");
			ADD_SUB_ITEM("vLoc.y",				pTableData->vLoc.y,					"Portal Table (XML column: Loc_Y)");
			ADD_SUB_ITEM("vLoc.z",				pTableData->vLoc.z,					"Portal Table (XML column: Loc_Z)");
			ADD_SUB_ITEM("vDir.x",				pTableData->vDir.x,					"Portal Table (XML column: Dir_X)");
			ADD_SUB_ITEM("vDir.z",				pTableData->vDir.z,					"Portal Table (XML column: Dir_Z)");
			ADD_SUB_ITEM("vMap.x",				pTableData->vMap.x,					"Portal Table (XML column: Map_X)");
			ADD_SUB_ITEM("vMap.z",				pTableData->vMap.z,					"Portal Table (XML column: Map_Y -- source writes Map_Y into vMap.z)");

			for (int i = 0; i < (int)DBO_MAX_POINT_PORTAL; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Portal Table [Point Slot %d]"), i + 1);

				ADD_SUB_ITEM("aPoint",			pTableData->aPoint[i],			(LPCTSTR)strGroup);
				ADD_SUB_ITEM("adwPointZenny",	pTableData->adwPointZenny[i],	(LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_ITEM_RECIPE:
		{
			sITEM_RECIPE_TBLDAT* pTableData = (sITEM_RECIPE_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"ItemRecipe Table");
			ADD_SUB_ITEM("bValidityAble",		pTableData->bValidityAble,			"ItemRecipe Table (XML column: Validity_Able)");
			ADD_SUB_ITEM("dwName",				pTableData->dwName,					"ItemRecipe Table (XML column: Name)");
			ADD_SUB_ITEM("byRecipeType",		(int)pTableData->byRecipeType,		"ItemRecipe Table");
			ADD_SUB_ITEM("byNeedMixLevel",		(int)pTableData->byNeedMixLevel,	"ItemRecipe Table");
			ADD_SUB_ITEM("dwNeedMixZenny",		pTableData->dwNeedMixZenny,			"ItemRecipe Table");

			for (int i = 0; i < (int)DBO_MAX_COUNT_RECIPE_CREATE_ITEM; i++)
			{
				CString strGroup;
				strGroup.Format(_T("ItemRecipe Table [Create Item Slot %d]"), i + 1);

				ADD_SUB_ITEM("itemTblidx",	pTableData->asCreateItemTblidx[i].itemTblidx,			(LPCTSTR)strGroup);
				ADD_SUB_ITEM("itemRate",	(int)pTableData->asCreateItemTblidx[i].itemRate,		(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byUnknown",	(int)pTableData->asCreateItemTblidx[i].byUnknown,		(LPCTSTR)(strGroup + _T(" (no XML column)")));
			}

			for (int i = 0; i < (int)DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM; i++)
			{
				CString strGroup;
				strGroup.Format(_T("ItemRecipe Table [Material Slot %d]"), i + 1);

				ADD_SUB_ITEM("materialTblidx",		pTableData->asMaterial[i].materialTblidx,		(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byMaterialCount",		(int)pTableData->asMaterial[i].byMaterialCount,(LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_MIX_MACHINE:
		{
			sITEM_MIX_MACHINE_TBLDAT* pTableData = (sITEM_MIX_MACHINE_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,					"MixMachine Table");
			ADD_SUB_ITEM("bValidityAble",				pTableData->bValidityAble,				"MixMachine Table (XML column: Validity_Able)");
			ADD_SUB_ITEM("name",						pTableData->name,						"MixMachine Table (XML column: Name)");
			ADD_SUB_ITEM("byMachineType",				(int)pTableData->byMachineType,		"MixMachine Table");
			ADD_SUB_ITEM("wFunctionBitFlag",			pTableData->wFunctionBitFlag,			"MixMachine Table (hex bitflag)");
			ADD_SUB_ITEM("byMixZennyDiscountRate",		(int)pTableData->byMixZennyDiscountRate,"MixMachine Table");
			ADD_SUB_ITEM("dynamicObjectTblidx",			pTableData->dynamicObjectTblidx,		"MixMachine Table");

			for (int i = 0; i < (int)DBO_MAX_COUNT_BUILT_IN_RECIPE; i++)
			{
				CString strGroup;
				strGroup.Format(_T("MixMachine Table [Built-In Recipe Slot %d]"), i + 1);

				ADD_SUB_ITEM("aBuiltInRecipeTblidx", pTableData->aBuiltInRecipeTblidx[i], (LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_DRAGONBALL:
		{
			sDRAGONBALL_TBLDAT* pTableData = (sDRAGONBALL_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,					"DragonBall Table");
			ADD_SUB_ITEM("dwAltarGroup",				pTableData->dwAltarGroup,				"DragonBall Table (XML column: Altar_Group)");
			ADD_SUB_ITEM("byBallType",					(int)pTableData->byBallType,			"DragonBall Table (XML column: Ball_Type)");

			for (int i = 0; i < (int)NTL_ITEM_MAX_DRAGONBALL; i++)
			{
				CString strGroup;
				strGroup.Format(_T("DragonBall Table [Ball Slot %d]"), i + 1);

				ADD_SUB_ITEM("aBallTblidx", pTableData->aBallTblidx[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("ballDropTblidx",				pTableData->ballDropTblidx,				"DragonBall Table (XML column: Ball_Drop_Tblidx)");
			ADD_SUB_ITEM("ballJunkTblidx",				pTableData->ballJunkTblidx,				"DragonBall Table (XML column: Ball_Junk_Tblidx)");
			ADD_SUB_ITEM("startDialog",					pTableData->startDialog,					"DragonBall Table (XML column: Start_Dialog)");
			ADD_SUB_ITEM("endDialog",					pTableData->endDialog,					"DragonBall Table (XML column: End_Dialog)");
			ADD_SUB_ITEM("timeoverEndDialog",			pTableData->timeoverEndDialog,			"DragonBall Table (XML column: Timeover_End_Dialog)");
			ADD_SUB_ITEM("hurryDialog",					pTableData->hurryDialog,					"DragonBall Table (XML column: Hurry_Dialog)");
			ADD_SUB_ITEM("timeoverDialog",				pTableData->timeoverDialog,				"DragonBall Table (XML column: Timeover_Dialog)");
			ADD_SUB_ITEM("noRepeatDialog",				pTableData->noRepeatDialog,				"DragonBall Table (XML column: No_Repeat_Dialog)");
			ADD_SUB_ITEM("inventoryFullDialog",			pTableData->inventoryFullDialog,			"DragonBall Table (XML column: Inventory_Full_Dialog)");
			ADD_SUB_ITEM("skillOverlapDialog",			pTableData->skillOverlapDialog,			"DragonBall Table (XML column: Skill_Overlap_Dialog)");
			ADD_SUB_ITEM("skillShortageOfLVDialog",		pTableData->skillShortageOfLVDialog,	"DragonBall Table (XML column: Skill_Shortage_Of_LV_Dialog)");
			ADD_SUB_ITEM("dragonNPCTblidx",				pTableData->dragonNPCTblidx,			"DragonBall Table (XML column: Dragon_NPC_Tblidx)");
			ADD_SUB_ITEM("defaultSummonChat",			pTableData->defaultSummonChat,			"DragonBall Table (XML column: Default_Summon_Chat)");
			ADD_SUB_ITEM("fDir.x",						pTableData->fDir.x,						"DragonBall Table (XML column: Appear_Dir_X)");
			ADD_SUB_ITEM("fDir.z",						pTableData->fDir.z,						"DragonBall Table (XML column: Appear_Dir_Z, sVECTOR2 member named .z)");
		}
		break;

		case CTableContainer::TABLE_WORLD_ZONE:
		{
			sWORLD_ZONE_TBLDAT* pTableData = (sWORLD_ZONE_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",					pTableData->tblidx,					"WorldZone Table");
			ADD_SUB_ITEM("wFunctionBitFlag",		pTableData->wFunctionBitFlag,			"WorldZone Table (XML column: Function_Bit_Flag, hex bitflag)");
			ADD_SUB_ITEM("worldTblidx",				pTableData->worldTblidx,				"WorldZone Table (XML column: World)");
			ADD_SUB_ITEM("nameTblidx",				pTableData->nameTblidx,					"WorldZone Table (XML column: Name)");
			ADD_SUB_ITEM("wszName_Text",			pTableData->wszName_Text,				"WorldZone Table");
			ADD_SUB_ITEM("bForbidden_Vehicle",		pTableData->bForbidden_Vehicle,			"WorldZone Table");
		}
		break;

		case CTableContainer::TABLE_COMMON_CONFIG:
		{
			sCOMMONCONFIG_TBLDAT* pTableData = (sCOMMONCONFIG_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",		pTableData->tblidx,				"CommonConfig Table (Tblidx-only XML reader)");
			ADD_SUB_ITEM("wstrName",	pTableData->wstrName.c_str(),		"CommonConfig Table (no XML column, binary-only)");

			for (int i = 0; i < (int)COMMONCONFIG_MAX_TBLDAT_VALUE_COUNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("CommonConfig Table [Value Slot %d] (no XML column, binary-only)"), i + 1);

				ADD_SUB_ITEM("wstrValue", pTableData->wstrValue[i].c_str(), (LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_DWC:
		{
			sDWC_TBLDAT* pTableData = (sDWC_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,					"Dwc Table");
			ADD_SUB_ITEM("tblNameIndex",				pTableData->tblNameIndex,				"Dwc Table (XML column: Name)");
			ADD_SUB_ITEM("byLevel_Min",					(int)pTableData->byLevel_Min,			"Dwc Table");
			ADD_SUB_ITEM("byLevel_Max",					(int)pTableData->byLevel_Max,			"Dwc Table");
			ADD_SUB_ITEM("wAdmission_Bit_Flag",			pTableData->wAdmission_Bit_Flag,		"Dwc Table (hex bitflag)");
			ADD_SUB_ITEM("byAdmission_Num_Min",			(int)pTableData->byAdmission_Num_Min,	"Dwc Table");
			ADD_SUB_ITEM("byAdmission_Num_Max",			(int)pTableData->byAdmission_Num_Max,	"Dwc Table");

			for (int i = 0; i < (int)MAX_DWC_ADMISSION_CONDITION_COUNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Dwc Table [Condition Slot %d]"), i + 1);

				ADD_SUB_ITEM("aConditionTblidx", pTableData->aConditionTblidx[i], (LPCTSTR)strGroup);
			}

			for (int i = 0; i < (int)MAX_DWC_MISSION_COUNT_PER_SCENARIO; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Dwc Table [Mission Slot %d]"), i + 1);

				ADD_SUB_ITEM("aMissionTblidx", pTableData->aMissionTblidx[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("prologueCinematicTblidx",		pTableData->prologueCinematicTblidx,	"Dwc Table (XML column: Prologue_Cinematic_Tblidx)");
			ADD_SUB_ITEM("prologueTblidx",				pTableData->prologueTblidx,			"Dwc Table (XML column: Prologue_Text)");
			ADD_SUB_ITEM("worldTblidx",					pTableData->worldTblidx,				"Dwc Table (XML column: World_Tblidx)");
		}
		break;

		case CTableContainer::TABLE_WORLD_MAP:
		{
			sWORLD_MAP_TBLDAT* pTableData = (sWORLD_MAP_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",				pTableData->tblidx,				"WorldMap Table");
			ADD_SUB_ITEM("World_Tblidx",		pTableData->World_Tblidx,			"WorldMap Table");
			ADD_SUB_ITEM("Zone_Tblidx",			pTableData->Zone_Tblidx,			"WorldMap Table");
			ADD_SUB_ITEM("Worldmap_Name",		pTableData->Worldmap_Name,			"WorldMap Table");
			ADD_SUB_ITEM("wszNameText",			pTableData->wszNameText,			"WorldMap Table (XML column: Name_Text)");
			ADD_SUB_ITEM("bValidityAble",		pTableData->bValidityAble,			"WorldMap Table (XML column: Validity_Able)");
			ADD_SUB_ITEM("byMapType",			(int)pTableData->byMapType,		"WorldMap Table (XML column: Map_Type)");
			ADD_SUB_ITEM("vStandardLoc.x",		pTableData->vStandardLoc.x,			"WorldMap Table (XML column: Standard_Loc_X)");
			ADD_SUB_ITEM("vStandardLoc.z",		pTableData->vStandardLoc.z,			"WorldMap Table (XML column: Standard_Loc_Z)");
			ADD_SUB_ITEM("fWorldmapScale",		pTableData->fWorldmapScale,			"WorldMap Table (XML column: Worldmap_Scale)");
			ADD_SUB_ITEM("dwLinkMapIdx",			pTableData->dwLinkMapIdx,			"WorldMap Table (XML column: Link_Map_Idx)");
			ADD_SUB_ITEM("dwComboBoxType",		pTableData->dwComboBoxType,			"WorldMap Table (XML column: Combobox_Type)");

			for (int i = 0; i < (int)DBO_WORLD_MAP_TABLE_COUNT_WORLD_WARFOG; i++)
			{
				CString strGroup;
				strGroup.Format(_T("WorldMap Table [Warfog Slot %d]"), i + 1);

				ADD_SUB_ITEM("wWarfog", pTableData->wWarfog[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("byRecomm_Min_Level",	(int)pTableData->byRecomm_Min_Level,"WorldMap Table");
			ADD_SUB_ITEM("byRecomm_Max_Level",	(int)pTableData->byRecomm_Max_Level,"WorldMap Table");
			ADD_SUB_ITEM("vUiModify.x",			pTableData->vUiModify.x,			"WorldMap Table (no XML column)");
			ADD_SUB_ITEM("vUiModify.y",			pTableData->vUiModify.y,			"WorldMap Table (no XML column)");
			ADD_SUB_ITEM("vUiModify.z",			pTableData->vUiModify.z,			"WorldMap Table (no XML column)");
		}
		break;

		case CTableContainer::TABLE_PC:
		{
			sPC_TBLDAT* pTableData = (sPC_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,						"PC Table");
			ADD_SUB_ITEM("byRace",						(int)pTableData->byRace,					"PC Table (XML column: Race)");
			ADD_SUB_ITEM("byGender",					(int)pTableData->byGender,					"PC Table (XML column: Gender)");
			ADD_SUB_ITEM("byClass",						(int)pTableData->byClass,					"PC Table (XML column: Class)");
			ADD_SUB_ITEM("prior_Class_Tblidx",			pTableData->prior_Class_Tblidx,			"PC Table");
			ADD_SUB_ITEM("dwClass_Bit_Flag",			pTableData->dwClass_Bit_Flag,				"PC Table (hex bitflag)");
			ADD_SUB_ITEM("szModel_Child",				pTableData->szModel_Child,					"PC Table");
			ADD_SUB_ITEM("szModel_Adult",				pTableData->szModel_Adult,					"PC Table");
			ADD_SUB_ITEM("fChild_Run_Speed_Origin",		pTableData->fChild_Run_Speed_Origin,		"PC Table");
			ADD_SUB_ITEM("fChild_Run_Speed",			pTableData->fChild_Run_Speed,				"PC Table");
			ADD_SUB_ITEM("fAdult_Run_Speed_Origin",		pTableData->fAdult_Run_Speed_Origin,		"PC Table");
			ADD_SUB_ITEM("fAdult_Run_Speed",			pTableData->fAdult_Run_Speed,				"PC Table");
			ADD_SUB_ITEM("fChild_Fly_Speed_Origin",		pTableData->fChild_Fly_Speed_Origin,		"PC Table (no XML column)");
			ADD_SUB_ITEM("fChild_Fly_Speed",			pTableData->fChild_Fly_Speed,				"PC Table (no XML column)");
			ADD_SUB_ITEM("fAdult_Fly_Speed_Origin",		pTableData->fAdult_Fly_Speed_Origin,		"PC Table (no XML column)");
			ADD_SUB_ITEM("fAdult_Fly_Speed",			pTableData->fAdult_Fly_Speed,				"PC Table (no XML column)");
			ADD_SUB_ITEM("fChild_Dash_Speed_Origin",	pTableData->fChild_Dash_Speed_Origin,		"PC Table (no XML column)");
			ADD_SUB_ITEM("fChild_Dash_Speed",			pTableData->fChild_Dash_Speed,				"PC Table (no XML column)");
			ADD_SUB_ITEM("fAdult_Dash_Speed_Origin",	pTableData->fAdult_Dash_Speed_Origin,		"PC Table (no XML column)");
			ADD_SUB_ITEM("fAdult_Dash_Speed",			pTableData->fAdult_Dash_Speed,				"PC Table (no XML column)");
			ADD_SUB_ITEM("fChild_Accel_Speed_Origin",	pTableData->fChild_Accel_Speed_Origin,		"PC Table (no XML column)");
			ADD_SUB_ITEM("fChild_Accel_Speed",			pTableData->fChild_Accel_Speed,			"PC Table (no XML column)");
			ADD_SUB_ITEM("fAdult_Accel_Speed_Origin",	pTableData->fAdult_Accel_Speed_Origin,		"PC Table (no XML column)");
			ADD_SUB_ITEM("fAdult_Accel_Speed",			pTableData->fAdult_Accel_Speed,			"PC Table (no XML column)");
			ADD_SUB_ITEM("wBasic_RP",					pTableData->wBasic_RP,						"PC Table (XML column: Basic_RP)");
			ADD_SUB_ITEM("wBasic_Physical_Offence",		pTableData->wBasic_Physical_Offence,		"PC Table (XML column: Basic_Physical_Offence)");
			ADD_SUB_ITEM("wBasic_Energy_Offence",		pTableData->wBasic_Energy_Offence,			"PC Table (XML column: Basic_Energy_Offence)");
			ADD_SUB_ITEM("dwWeightLimit",				pTableData->dwWeightLimit,					"PC Table (no XML column)");
			ADD_SUB_ITEM("byLevel_Up_LP",				(int)pTableData->byLevel_Up_LP,			"PC Table");
			ADD_SUB_ITEM("byLevel_Up_EP",				(int)pTableData->byLevel_Up_EP,			"PC Table");
			ADD_SUB_ITEM("byLevel_Up_RP",				(int)pTableData->byLevel_Up_RP,			"PC Table");
			ADD_SUB_ITEM("byLevel_Up_Physical_Offence",	(int)pTableData->byLevel_Up_Physical_Offence,"PC Table");
			ADD_SUB_ITEM("byLevel_Up_Physical_Defence",	(int)pTableData->byLevel_Up_Physical_Defence,"PC Table");
			ADD_SUB_ITEM("byLevel_Up_Energy_Offence",	(int)pTableData->byLevel_Up_Energy_Offence,"PC Table");
			ADD_SUB_ITEM("byLevel_Up_Energy_Defence",	(int)pTableData->byLevel_Up_Energy_Defence,"PC Table");
			ADD_SUB_ITEM("fLevel_Up_Str",				pTableData->fLevel_Up_Str,					"PC Table");
			ADD_SUB_ITEM("fLevel_Up_Con",				pTableData->fLevel_Up_Con,					"PC Table");
			ADD_SUB_ITEM("fLevel_Up_Foc",				pTableData->fLevel_Up_Foc,					"PC Table");
			ADD_SUB_ITEM("fLevel_Up_Dex",				pTableData->fLevel_Up_Dex,					"PC Table");
			ADD_SUB_ITEM("fLevel_Up_Sol",				pTableData->fLevel_Up_Sol,					"PC Table");
			ADD_SUB_ITEM("fLevel_Up_Eng",				pTableData->fLevel_Up_Eng,					"PC Table");
			ADD_SUB_ITEM("dwBasic_LP",					pTableData->dwBasic_LP,						"PC Table (XML column: Basic_LP)");
			ADD_SUB_ITEM("wBasic_EP",					pTableData->wBasic_EP,						"PC Table (XML column: Basic_EP)");
			ADD_SUB_ITEM("wBasic_Physical_Defence",		pTableData->wBasic_Physical_Defence,		"PC Table (XML column: Basic_Physical_Defence)");
			ADD_SUB_ITEM("wBasic_Energy_Defence",		pTableData->wBasic_Energy_Defence,			"PC Table (XML column: Basic_Energy_Defence)");
			ADD_SUB_ITEM("wBasicStr",					pTableData->wBasicStr,						"PC Table (XML column: Basic_Str)");
			ADD_SUB_ITEM("wBasicCon",					pTableData->wBasicCon,						"PC Table (XML column: Basic_Con)");
			ADD_SUB_ITEM("wBasicFoc",					pTableData->wBasicFoc,						"PC Table (XML column: Basic_Foc)");
			ADD_SUB_ITEM("wBasicDex",					pTableData->wBasicDex,						"PC Table (XML column: Basic_Dex)");
			ADD_SUB_ITEM("wBasicSol",					pTableData->wBasicSol,						"PC Table (XML column: Basic_Sol)");
			ADD_SUB_ITEM("wBasicEng",					pTableData->wBasicEng,						"PC Table (XML column: Basic_Eng)");
			ADD_SUB_ITEM("fScale",						pTableData->fScale,							"PC Table (XML column: Scale)");
			ADD_SUB_ITEM("wAttack_Speed_Rate",			pTableData->wAttack_Speed_Rate,			"PC Table (XML column: Attack_Speed_Rate)");
			ADD_SUB_ITEM("byAttack_Type",				(int)pTableData->byAttack_Type,			"PC Table (XML column: Attack_Type)");
			ADD_SUB_ITEM("fAttack_Range",				pTableData->fAttack_Range,					"PC Table (XML column: Attack_Range)");
			ADD_SUB_ITEM("wAttack_Rate",				pTableData->wAttack_Rate,					"PC Table (XML column: Basic_Attack_Rate)");
			ADD_SUB_ITEM("wDodge_Rate",					pTableData->wDodge_Rate,					"PC Table (XML column: Basic_Dodge_Rate)");
			ADD_SUB_ITEM("wBlock_Rate",					pTableData->wBlock_Rate,					"PC Table (XML column: Basic_Block_Rate)");
			ADD_SUB_ITEM("wCurse_Success_Rate",			pTableData->wCurse_Success_Rate,			"PC Table (XML column: Basic_Curse_Success_Rate)");
			ADD_SUB_ITEM("wCurse_Tolerance_Rate",		pTableData->wCurse_Tolerance_Rate,			"PC Table (XML column: Basic_Curse_Tolerance_Rate)");
			ADD_SUB_ITEM("fRadius",						pTableData->fRadius,						"PC Table (no XML column: reader branch is commented out)");
			ADD_SUB_ITEM("wBasic_Aggro_Point",			pTableData->wBasic_Aggro_Point,			"PC Table (XML column: Basic_Aggro_Point)");
		}
		break;

		case CTableContainer::TABLE_NPC:
		{
			sNPC_TBLDAT* pTableData = (sNPC_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,					"Npc Table");
			ADD_SUB_ITEM("bValidity_Able",				pTableData->bValidity_Able,			"Npc Table (XML column: Validity_Able)");
			ADD_SUB_ITEM("Name",							pTableData->Name,						"Npc Table");
			ADD_SUB_ITEM("wszNameText",					pTableData->wszNameText,				"Npc Table (XML column: Name_Text)");
			ADD_SUB_ITEM("szModel",						pTableData->szModel,					"Npc Table (XML column: Model)");
			ADD_SUB_ITEM("byLevel",						(int)pTableData->byLevel,			"Npc Table");
			ADD_SUB_ITEM("byGrade",						(int)pTableData->byGrade,			"Npc Table (no XML column)");
			ADD_SUB_ITEM("dwAi_Bit_Flag",				pTableData->dwAi_Bit_Flag,			"Npc Table (XML column: Ai_Bit_Flag, hex)");
			ADD_SUB_ITEM("wLP_Regeneration",			pTableData->wLP_Regeneration,		"Npc Table");
			ADD_SUB_ITEM("wEP_Regeneration",			pTableData->wEP_Regeneration,		"Npc Table");
			ADD_SUB_ITEM("byAttack_Animation_Quantity",	(int)pTableData->byAttack_Animation_Quantity,"Npc Table (no XML column)");
			ADD_SUB_ITEM("byBattle_Attribute",			(int)pTableData->byBattle_Attribute,	"Npc Table");
			ADD_SUB_ITEM("wBasic_Physical_Offence",		pTableData->wBasic_Physical_Offence,	"Npc Table");
			ADD_SUB_ITEM("wBasic_Energy_Offence",		pTableData->wBasic_Energy_Offence,	"Npc Table");
			ADD_SUB_ITEM("fWalk_Speed_Origin",			pTableData->fWalk_Speed_Origin,		"Npc Table");
			ADD_SUB_ITEM("fWalk_Speed",					pTableData->fWalk_Speed,				"Npc Table");
			ADD_SUB_ITEM("fRun_Speed_Origin",			pTableData->fRun_Speed_Origin,		"Npc Table");
			ADD_SUB_ITEM("fRun_Speed",					pTableData->fRun_Speed,				"Npc Table");
			ADD_SUB_ITEM("fRadius_X",					pTableData->fRadius_X,				"Npc Table");
			ADD_SUB_ITEM("fRadius_Z",					pTableData->fRadius_Z,				"Npc Table");
			ADD_SUB_ITEM("wSight_Range",					pTableData->wSight_Range,			"Npc Table");
			ADD_SUB_ITEM("wScan_Range",					pTableData->wScan_Range,				"Npc Table");

			for (int i = 0; i < (int)NTL_MAX_NPC_HAVE_SKILL; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Npc Table [Skill Slot %d]"), i + 1);

				ADD_SUB_ITEM("wUse_Skill_Time", pTableData->wUse_Skill_Time[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("use_Skill_Tblidx", pTableData->use_Skill_Tblidx[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("byUse_Skill_Basis", (int)pTableData->byUse_Skill_Basis[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("wUse_Skill_LP", pTableData->wUse_Skill_LP[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("byVisible_Sight_Range",		(int)pTableData->byVisible_Sight_Range,"Npc Table");
			ADD_SUB_ITEM("szCamera_Bone_Name",			pTableData->szCamera_Bone_Name,		"Npc Table");
			ADD_SUB_ITEM("wAttackCoolTime",				pTableData->wAttackCoolTime,			"Npc Table (XML column: Attack_Cool_Time)");
			ADD_SUB_ITEM("fFly_Height",					pTableData->fFly_Height,				"Npc Table");
			ADD_SUB_ITEM("bSpawn_Animation",			pTableData->bSpawn_Animation,		"Npc Table");
			ADD_SUB_ITEM("dwDialogGroup",				pTableData->dwDialogGroup,			"Npc Table (XML column: Dialog_Group)");
			ADD_SUB_ITEM("szILLust",					pTableData->szILLust,				"Npc Table");
			ADD_SUB_ITEM("dwAllianceIdx",				pTableData->dwAllianceIdx,			"Npc Table (XML column: Alliance_Idx)");
			ADD_SUB_ITEM("wAggroMaxCount",				pTableData->wAggroMaxCount,			"Npc Table");
			ADD_SUB_ITEM("dwNpcAttributeFlag",			pTableData->dwNpcAttributeFlag,		"Npc Table (no XML column)");
			ADD_SUB_ITEM("wStomachacheDefence",			pTableData->wStomachacheDefence,	"Npc Table (no XML column)");
			ADD_SUB_ITEM("wPoisonDefence",				pTableData->wPoisonDefence,			"Npc Table (no XML column)");
			ADD_SUB_ITEM("wBleedDefence",				pTableData->wBleedDefence,			"Npc Table (no XML column)");
			ADD_SUB_ITEM("wBurnDefence",					pTableData->wBurnDefence,			"Npc Table (no XML column)");
			ADD_SUB_ITEM("szNameText",					CString(pTableData->szNameText),	"Npc Table (no XML column: duplicate legacy char buffer)");
			ADD_SUB_ITEM("wBasic_Aggro_Point",			pTableData->wBasic_Aggro_Point,		"Npc Table");
			ADD_SUB_ITEM("byNpcType",					(int)pTableData->byNpcType,			"Npc Table (XML column: NPC_type)");
			ADD_SUB_ITEM("byJob",						(int)pTableData->byJob,				"Npc Table");
			ADD_SUB_ITEM("dwFunc_Bit_Flag",				pTableData->dwFunc_Bit_Flag,			"Npc Table (XML column: Function_Bit_Flag, hex)");
			ADD_SUB_ITEM("Dialog_Script_Index",			pTableData->Dialog_Script_Index,		"Npc Table");

			for (int i = 0; i < (int)NTL_MAX_MERCHANT_TAB_COUNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Npc Table [Merchant Tab Slot %d]"), i + 1);

				ADD_SUB_ITEM("amerchant_Tblidx", pTableData->amerchant_Tblidx[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("statusTransformTblidx",		pTableData->statusTransformTblidx,	"Npc Table (XML column: Status_Transform_Tblidx)");
			ADD_SUB_ITEM("contentsTblidx",				pTableData->contentsTblidx,			"Npc Table (XML column: Contents_Tblidx)");
			ADD_SUB_ITEM("wUnknown",					pTableData->wUnknown,				"Npc Table (no XML column)");
			ADD_SUB_ITEM("dwUnknown2",					pTableData->dwUnknown2,				"Npc Table (no XML column)");
			ADD_SUB_ITEM("dwUnknown3",					pTableData->dwUnknown3,				"Npc Table (no XML column)");
		}
		break;

		case CTableContainer::TABLE_MOB:
		{
			sMOB_TBLDAT* pTableData = (sMOB_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,					"Mob Table");
			ADD_SUB_ITEM("bValidity_Able",				pTableData->bValidity_Able,			"Mob Table");
			ADD_SUB_ITEM("Name",							pTableData->Name,						"Mob Table");
			ADD_SUB_ITEM("wszNameText",					pTableData->wszNameText,				"Mob Table");
			ADD_SUB_ITEM("szModel",						pTableData->szModel,					"Mob Table");
			ADD_SUB_ITEM("byLevel",						(int)pTableData->byLevel,			"Mob Table");
			ADD_SUB_ITEM("byGrade",						(int)pTableData->byGrade,			"Mob Table");
			ADD_SUB_ITEM("dwAi_Bit_Flag",				pTableData->dwAi_Bit_Flag,			"Mob Table (hex)");
			ADD_SUB_ITEM("wLP_Regeneration",			pTableData->wLP_Regeneration,		"Mob Table");
			ADD_SUB_ITEM("wEP_Regeneration",			pTableData->wEP_Regeneration,		"Mob Table");
			ADD_SUB_ITEM("byAttack_Animation_Quantity",	(int)pTableData->byAttack_Animation_Quantity,"Mob Table");
			ADD_SUB_ITEM("byBattle_Attribute",			(int)pTableData->byBattle_Attribute,	"Mob Table");
			ADD_SUB_ITEM("wBasic_Physical_Offence",		pTableData->wBasic_Physical_Offence,	"Mob Table");
			ADD_SUB_ITEM("wBasic_Energy_Offence",		pTableData->wBasic_Energy_Offence,	"Mob Table");
			ADD_SUB_ITEM("fWalk_Speed_Origin",			pTableData->fWalk_Speed_Origin,		"Mob Table");
			ADD_SUB_ITEM("fWalk_Speed",					pTableData->fWalk_Speed,				"Mob Table");
			ADD_SUB_ITEM("fRun_Speed_Origin",			pTableData->fRun_Speed_Origin,		"Mob Table");
			ADD_SUB_ITEM("fRun_Speed",					pTableData->fRun_Speed,				"Mob Table");
			ADD_SUB_ITEM("fRadius_X",					pTableData->fRadius_X,				"Mob Table");
			ADD_SUB_ITEM("fRadius_Z",					pTableData->fRadius_Z,				"Mob Table");
			ADD_SUB_ITEM("fRadius",						pTableData->fRadius,					"Mob Table (derived: (Radius_X+Radius_Z)*0.5 in AddTable, no XML column)");
			ADD_SUB_ITEM("wSight_Range",					pTableData->wSight_Range,			"Mob Table (parsed via READ_BYTE despite WORD type)");
			ADD_SUB_ITEM("wScan_Range",					pTableData->wScan_Range,				"Mob Table (parsed via READ_BYTE despite WORD type)");

			for (int i = 0; i < (int)NTL_MAX_NPC_HAVE_SKILL; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Mob Table [Skill Slot %d]"), i + 1);

				ADD_SUB_ITEM("wUse_Skill_Time", pTableData->wUse_Skill_Time[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("use_Skill_Tblidx", pTableData->use_Skill_Tblidx[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("byUse_Skill_Basis", (int)pTableData->byUse_Skill_Basis[i], (LPCTSTR)strGroup);
				ADD_SUB_ITEM("wUse_Skill_LP", pTableData->wUse_Skill_LP[i], (LPCTSTR)strGroup);
			}

			ADD_SUB_ITEM("byVisible_Sight_Range",		(int)pTableData->byVisible_Sight_Range,"Mob Table");
			ADD_SUB_ITEM("szCamera_Bone_Name",			pTableData->szCamera_Bone_Name,		"Mob Table");
			ADD_SUB_ITEM("wAttackCoolTime",				pTableData->wAttackCoolTime,			"Mob Table");
			ADD_SUB_ITEM("fFly_Height",					pTableData->fFly_Height,				"Mob Table");
			ADD_SUB_ITEM("bSpawn_Animation",			pTableData->bSpawn_Animation,		"Mob Table");
			ADD_SUB_ITEM("dwDialogGroup",				pTableData->dwDialogGroup,			"Mob Table");
			ADD_SUB_ITEM("szILLust",					pTableData->szILLust,				"Mob Table");
			ADD_SUB_ITEM("dwAllianceIdx",				pTableData->dwAllianceIdx,			"Mob Table");
			ADD_SUB_ITEM("wAggroMaxCount",				pTableData->wAggroMaxCount,			"Mob Table");
			ADD_SUB_ITEM("dwNpcAttributeFlag",			pTableData->dwNpcAttributeFlag,		"Mob Table (XML: Attribute_Bit_Flag)");
			ADD_SUB_ITEM("wStomachacheDefence",			pTableData->wStomachacheDefence,	"Mob Table");
			ADD_SUB_ITEM("wPoisonDefence",				pTableData->wPoisonDefence,			"Mob Table");
			ADD_SUB_ITEM("wBleedDefence",				pTableData->wBleedDefence,			"Mob Table");
			ADD_SUB_ITEM("wBurnDefence",					pTableData->wBurnDefence,			"Mob Table");
			ADD_SUB_ITEM("dwMobGroup",					pTableData->dwMobGroup,				"Mob Table");
			ADD_SUB_ITEM("wMob_Kind",					pTableData->wMob_Kind,				"Mob Table");
			ADD_SUB_ITEM("dwDrop_Zenny",					pTableData->dwDrop_Zenny,			"Mob Table");
			ADD_SUB_ITEM("fDrop_Zenny_Rate",			pTableData->fDrop_Zenny_Rate,		"Mob Table");
			ADD_SUB_ITEM("dwExp",						pTableData->dwExp,					"Mob Table");
			ADD_SUB_ITEM("byMob_Type",					(int)pTableData->byMob_Type,			"Mob Table (XML column: Mob_type)");
			ADD_SUB_ITEM("drop_Item_Tblidx",			pTableData->drop_Item_Tblidx,		"Mob Table (no XML column: reader branch is commented out)");
			ADD_SUB_ITEM("bSize",						pTableData->bSize,					"Mob Table");
			ADD_SUB_ITEM("wTMQPoint",					pTableData->wTMQPoint,				"Mob Table");
			ADD_SUB_ITEM("dropQuestTblidx",				pTableData->dropQuestTblidx,			"Mob Table");
			ADD_SUB_ITEM("idxBigBag1",					pTableData->idxBigBag1,				"Mob Table");
			ADD_SUB_ITEM("byDropRate1",					(int)pTableData->byDropRate1,		"Mob Table");
			ADD_SUB_ITEM("byTryCount1",					(int)pTableData->byTryCount1,		"Mob Table");
			ADD_SUB_ITEM("idxBigBag2",					pTableData->idxBigBag2,				"Mob Table (reader bug: idxBigBag2/3 XML columns overwrite idxBigBag1 instead)");
			ADD_SUB_ITEM("byDropRate2",					(int)pTableData->byDropRate2,		"Mob Table (reader bug: overwrites byDropRate1 instead)");
			ADD_SUB_ITEM("byTryCount2",					(int)pTableData->byTryCount2,		"Mob Table (reader bug: overwrites byTryCount1 instead)");
			ADD_SUB_ITEM("idxBigBag3",					pTableData->idxBigBag3,				"Mob Table (reader bug: overwrites idxBigBag1 instead)");
			ADD_SUB_ITEM("byDropRate3",					(int)pTableData->byDropRate3,		"Mob Table (reader bug: overwrites byDropRate1 instead)");
			ADD_SUB_ITEM("byTryCount3",					(int)pTableData->byTryCount3,		"Mob Table (reader bug: overwrites byTryCount1 instead)");
			ADD_SUB_ITEM("scanRangeConfigTblidx",		pTableData->scanRangeConfigTblidx,	"Mob Table (no XML column)");
			ADD_SUB_ITEM("byUnknown",					(int)pTableData->byUnknown,			"Mob Table (no XML column)");
			ADD_SUB_ITEM("byUnknown2",					(int)pTableData->byUnknown2,			"Mob Table (no XML column)");
			ADD_SUB_ITEM("bShow_Name",					pTableData->bShow_Name,				"Mob Table (XML column recognized but assignment is commented out -- dead)");
			ADD_SUB_ITEM("wSightAngle",					pTableData->wSightAngle,				"Mob Table");
			ADD_SUB_ITEM("dwImmunity_Bit_Flag",			pTableData->dwImmunity_Bit_Flag,		"Mob Table");
			ADD_SUB_ITEM("bIsDragonBallDrop",			pTableData->bIsDragonBallDrop,		"Mob Table (XML column: IsDragonballDrop)");
			ADD_SUB_ITEM("wMonsterClass",				pTableData->wMonsterClass,			"Mob Table (XML column: Class)");
			ADD_SUB_ITEM("wUseRace",					pTableData->wUseRace,				"Mob Table");
			ADD_SUB_ITEM("fRewardExpRate",				pTableData->fRewardExpRate,			"Mob Table");
			ADD_SUB_ITEM("fRewardZennyRate",			pTableData->fRewardZennyRate,		"Mob Table");
			ADD_SUB_ITEM("dwFormulaOffset",				pTableData->dwFormulaOffset,			"Mob Table");
			ADD_SUB_ITEM("fSettingRate_LP",				pTableData->fSettingRate_LP,			"Mob Table");
			ADD_SUB_ITEM("fSettingRate_LPRegen",		pTableData->fSettingRate_LPRegen,	"Mob Table");
			ADD_SUB_ITEM("fSettingRate_PhyOffence",		pTableData->fSettingRate_PhyOffence,	"Mob Table");
			ADD_SUB_ITEM("fSettingRate_EngOffence",		pTableData->fSettingRate_EngOffence,	"Mob Table");
			ADD_SUB_ITEM("fSettingRate_PhyDefence",		pTableData->fSettingRate_PhyDefence,	"Mob Table");
			ADD_SUB_ITEM("fSettingRate_EngDefence",		pTableData->fSettingRate_EngDefence,	"Mob Table");
			ADD_SUB_ITEM("fSettingRate_AttackRate",		pTableData->fSettingRate_AttackRate,	"Mob Table");
			ADD_SUB_ITEM("fSettingRate_DodgeRate",		pTableData->fSettingRate_DodgeRate,	"Mob Table");
			ADD_SUB_ITEM("fSettingPhyOffenceRate",		pTableData->fSettingPhyOffenceRate,	"Mob Table");
			ADD_SUB_ITEM("fSettingEngOffenceRate",		pTableData->fSettingEngOffenceRate,	"Mob Table");
			ADD_SUB_ITEM("fSettingRate_Defence_Role",	pTableData->fSettingRate_Defence_Role,"Mob Table");
		}
		break;

		case CTableContainer::TABLE_DWCMISSION:
		{
			sDWCMISSION_TBLDAT* pTableData = (sDWCMISSION_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx",						pTableData->tblidx,					"Dwc Mission Table");
			ADD_SUB_ITEM("tblNameIndex",				pTableData->tblNameIndex,				"Dwc Mission Table (no XML column)");
			ADD_SUB_ITEM("szImageName",					pTableData->szImageName,				"Dwc Mission Table (no XML column)");
			ADD_SUB_ITEM("tblContainScenarioIndex",		pTableData->tblContainScenarioIndex,	"Dwc Mission Table (no XML column)");
			ADD_SUB_ITEM("byCompleteMinNum",			(int)pTableData->byCompleteMinNum,		"Dwc Mission Table (no XML column)");
			ADD_SUB_ITEM("byCompleteMaxNum",			(int)pTableData->byCompleteMaxNum,		"Dwc Mission Table (no XML column)");
			ADD_SUB_ITEM("byDifficulty",				(int)pTableData->byDifficulty,			"Dwc Mission Table (no XML column)");
			ADD_SUB_ITEM("clearObjectTextTblidx",		pTableData->clearObjectTextTblidx,		"Dwc Mission Table (no XML column)");
			ADD_SUB_ITEM("clearConditionTextTblidx",	pTableData->clearConditionTextTblidx,	"Dwc Mission Table (no XML column)");

			for (int i = 0; i < MAX_DWC_MISSION_COUNT_PER_SCENARIO; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Dwc Mission Table [Reward %d] (no XML column)"), i + 1);

				ADD_SUB_ITEM("cardNameTblidx",		pTableData->asReward[i].cardNameTblidx,	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("byRequireCount",		(int)pTableData->asReward[i].byRequireCount,(LPCTSTR)strGroup);
				ADD_SUB_ITEM("fAcquireRate",			pTableData->asReward[i].fAcquireRate,		(LPCTSTR)strGroup);
			}
		}
		break;

		case CTableContainer::TABLE_QUEST_DROP:
		{
			sQUEST_DROP_TBLDAT* pTableData = (sQUEST_DROP_TBLDAT*)pTbldat;

			ADD_SUB_ITEM("tblidx", pTableData->tblidx, "Quest Drop Table (XML column: Quest_Drop_Tblidx)");

			for (int i = 0; i < QUEST_ITEM_DROP_MAX_COUNT; i++)
			{
				CString strGroup;
				strGroup.Format(_T("Quest Drop Table [Slot %d]"), i + 1);

				ADD_SUB_ITEM("aQuestItemTblidx",	pTableData->aQuestItemTblidx[i],	(LPCTSTR)strGroup);
				ADD_SUB_ITEM("aDropRate",			pTableData->aDropRate[i],			(LPCTSTR)strGroup);
			}
		}
		break;

		default: break;
	}

	m_wndPropList.AddProperty(pGroup1);

	AdjustLayout();
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	m_btnExpandAll.Create(_T("Expand All"), WS_CHILD | WS_VISIBLE, rectDummy, this, ID_EXPAND_ALL);
	m_btnSortAZ.Create(_T("Sort A-Z"), WS_CHILD | WS_VISIBLE, rectDummy, this, ID_SORTPROPERTIES);
	m_btnExpandAll.SetFont(&afxGlobalData.fontRegular);
	m_btnSortAZ.SetFont(&afxGlobalData.fontRegular);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnSortProperties()
{
	bool bNowAlphabetic = !m_wndPropList.IsAlphabeticMode();
	m_wndPropList.SetAlphabeticMode(bNowAlphabetic);
	m_btnSortAZ.SetAccented(bNowAlphabetic);
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	// CMFCPropertyGridCtrl's own dark-theming hook -- background, group
	// background/text, description area, and grid line colors all in one
	// call (see afxpropertygridctrl.h's SetCustomColors).
	m_wndPropList.SetCustomColors(Theme::Bg1, Theme::Text, Theme::Bg2, Theme::Text,
		Theme::Bg2, Theme::TextMuted, Theme::Border);

	/*CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Appearance"));

	ADD_SUB_ITEM("3D Look", false, "Specifies the window's font will be non-bold and controls will have a 3D border");

	CMFCPropertyGridProperty* pProp = new CMFCPropertyGridProperty(_T("Border"), _T("Dialog Frame"), _T("One of: None, Thin, Resizable, or Dialog Frame"));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	ADD_SUB_ITEM("Caption",  _T("About", "Specifies the text that will be displayed in the window's title bar"));

	m_wndPropList.AddProperty(pGroup1);

	CMFCPropertyGridProperty* pSize = new CMFCPropertyGridProperty(_T("Window Size"), 0, TRUE);

	pProp = new CMFCPropertyGridProperty(_T("Height"),  250l, _T("Specifies the window's height"));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridProperty( _T("Width"),  150l, _T("Specifies the window's width"));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropList.AddProperty(pSize);

	CMFCPropertyGridProperty* pGroup2 = new CMFCPropertyGridProperty(_T("Font"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	_tcscpy_s(lf.lfFaceName, _T("Arial"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("Font"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("Specifies the default font for the window")));
	pGroup2->AddSubItem(new CMFCPropertyGridProperty(_T("Use System Font"),  true, _T("Specifies that the window uses MS Shell Dlg font")));

	m_wndPropList.AddProperty(pGroup2);

	CMFCPropertyGridProperty* pGroup3 = new CMFCPropertyGridProperty(_T("Misc"));
	pProp = new CMFCPropertyGridProperty(_T("(Name)"), _T("Application"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Window Color"), RGB(210, 192, 254), nullptr, _T("Specifies the default window color"));
	pColorProp->EnableOtherButton(_T("Other..."));
	pColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("Icon Files(*.ico)|*.ico|All Files(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Icon"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("Specifies the window icon")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("Folder"), _T("c:\\")));

	m_wndPropList.AddProperty(pGroup3);*/
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CWnd::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}
