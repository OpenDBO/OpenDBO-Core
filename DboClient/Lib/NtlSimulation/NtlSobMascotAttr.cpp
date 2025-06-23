#include "precomp_ntlsimulation.h"
#include "NtlSobMascotAttr.h"

// shared
#include "NPCTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"
#include "MascotTable.h"
#include "MascotGradeTable.h"
#include "MascotStatusTable.h"
#include "ItemTable.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"

DEFINITION_MEMORY_POOL(CNtlSobMascotAttr)

CNtlSobMascotAttr::CNtlSobMascotAttr()
	: CNtlSobLifeAttr()
{
	m_bySummonSourceType = 0;
	m_uiSourceTblId = 0;
	m_pNpcTbl = NULL;
	m_pMascotTbl = NULL;
	m_pMascotGradeTbl = NULL;
	
	// Initialize mascot-specific data
	m_byMascotIndex = INVALID_BYTE;
	m_uiMascotTblidx = INVALID_TBLIDX;
	m_uiItemTblidx = INVALID_TBLIDX;
	m_dwCurVP = 0;
	m_dwMaxVP = 0;
	m_dwCurExp = 0;
	m_byItemRank = 0;
	memset(m_skillTblidx, 0, sizeof(m_skillTblidx));
	
	// Initialize additional mascot properties
	m_bValidityAble = false;
	m_byModelType = 0;
	m_bySlotNum = 0;
	m_wSPDecreaseRate = 0;
}

void CNtlSobMascotAttr::HandleEvents(RWS::CMsg& pMsg)
{
	if (pMsg.Id == g_EventSobCreate)
	{
		// Use standard SOB creation event (like other entities)
		SNtlEventSobBaseCreate *pBaseCreate = reinterpret_cast<SNtlEventSobBaseCreate*>(pMsg.pData);
		
		SetClassID(SLCLASS_MASCOT);
		
		// Mascot data will be set through SetItemTblidx() calls from the GUI/packet handlers
		OutputDebugStringA("[MASCOT_ATTR_DEBUG] Handled standard SOB creation event\n");
	}
}

sNPC_TBLDAT* CNtlSobMascotAttr::GetNpcTbl(void) const
{
	return m_pNpcTbl;
}

sMASCOT_TBLDAT* CNtlSobMascotAttr::GetMascotTbl(void) const
{
	return m_pMascotTbl;
}

sMASCOT_GRADE_TBLDAT* CNtlSobMascotAttr::GetMascotGradeTbl(void) const
{
	return m_pMascotGradeTbl;
}

void CNtlSobMascotAttr::SetMascotTbl(sMASCOT_TBLDAT* pMascotTbl)
{
	OutputDebugStringA("[MASCOT_ATTR_DEBUG] SetMascotTbl - Entry\n");
	OutputDebugStringA(("[MASCOT_ATTR_DEBUG] Mascot table pointer: " + std::string(pMascotTbl ? "VALID" : "NULL") + "\n").c_str());
	
	m_pMascotTbl = pMascotTbl;
	
	if (m_pMascotTbl)
	{
		OutputDebugStringA(("[MASCOT_ATTR_DEBUG] Setting model name: " + std::string(m_pMascotTbl->szModel) + "\n").c_str());
		
		// Set all attributes from mascot table data
		SetModelName(m_pMascotTbl->szModel);
		
		// Debug: Check what the model name is after setting
		const RwChar* pSetModelName = GetModelName();
		OutputDebugStringA(("[MASCOT_ATTR_DEBUG] Model name after setting: " + std::string(pSetModelName ? pSetModelName : "NULL") + "\n").c_str());
		
		SetName(m_pMascotTbl->wszNameText);
		SetLevel(1); // Mascots typically start at level 1
		
		// Set basic attributes that are available in CNtlSobLifeAttr
		SetRadius(1.0f);
		
		// Set LP (Life Points) - mascots should have some HP
		SetMaxLp(100); // Default HP for mascots
		SetLp(100);
		
		// Set SP (Spirit Points) from mascot table
		SetMaxEp(m_pMascotTbl->wMax_SP);
		SetEp(m_pMascotTbl->wMax_SP);
		
		// Set RP (Rage Points) - default value
		SetMaxRp(100);
		
		// Set name color for proper display
		SetNameColor(NTL_NPC_NAME_COLOR);
		
		// Set mascot-specific properties
		m_uiMascotTblidx = m_pMascotTbl->tblidx;
		m_byItemRank = m_pMascotTbl->byRank;
		m_bValidityAble = m_pMascotTbl->bValidity_Able;
		m_byModelType = m_pMascotTbl->byModel_Type;
		m_bySlotNum = m_pMascotTbl->bySlot_Num;
		m_wSPDecreaseRate = m_pMascotTbl->wSP_Decrease_Rate;
		
		OutputDebugStringA("[MASCOT_ATTR_DEBUG] All mascot properties set successfully\n");
	}
	else
	{
		OutputDebugStringA("[MASCOT_ATTR_DEBUG] Mascot table is NULL, skipping property setting\n");
	}
	
	OutputDebugStringA("[MASCOT_ATTR_DEBUG] SetMascotTbl - Exit\n");
}

void CNtlSobMascotAttr::SetMascotGradeTbl(sMASCOT_GRADE_TBLDAT* pMascotGradeTbl)
{
	m_pMascotGradeTbl = pMascotGradeTbl;
}

// Setters for mascot data from MascotGui.cpp
void CNtlSobMascotAttr::SetMascotIndex(BYTE byIndex)
{
	m_byMascotIndex = byIndex;
}

void CNtlSobMascotAttr::SetMascotTblidx(TBLIDX uiTblidx)
{
	m_uiMascotTblidx = uiTblidx;
}

void CNtlSobMascotAttr::SetItemTblidx(TBLIDX uiItemTblidx)
{
	OutputDebugStringA(("[MASCOT_ATTR_DEBUG] SetItemTblidx - Entry with tblidx: " + std::to_string(uiItemTblidx) + "\n").c_str());
	
	m_uiItemTblidx = uiItemTblidx;
	
	// Load mascot table data when itemTblidx is set
	if (m_uiItemTblidx != INVALID_TBLIDX)
	{
		OutputDebugStringA("[MASCOT_ATTR_DEBUG] Loading mascot table data\n");
		
		// Try MascotStatusTable first (this is the correct table for mascot data)
		CMascotStatusTable* pMascotStatusTbl = API_GetTableContainer()->GetMascotStatusTable();
		if (pMascotStatusTbl)
		{
			OutputDebugStringA("[MASCOT_ATTR_DEBUG] MascotStatus table found\n");
			
			sMASCOT_STATUS_TBLDAT* pMascotStatusData = (sMASCOT_STATUS_TBLDAT*)pMascotStatusTbl->FindData(m_uiItemTblidx);
			if (pMascotStatusData)
			{
				OutputDebugStringA(("[MASCOT_ATTR_DEBUG] MascotStatus data found, model name: " + std::string(pMascotStatusData->szModel) + "\n").c_str());
				
				// Create mascot table entry from status data
				sMASCOT_TBLDAT mascotData;
				memset(&mascotData, 0, sizeof(mascotData));
				
				// Copy data from status table
				strcpy_s(mascotData.szModel, pMascotStatusData->szModel);
				mascotData.tblidx = pMascotStatusData->tblidx;
				mascotData.byRank = pMascotStatusData->byRank;
				mascotData.bValidity_Able = pMascotStatusData->bValidity_Able;
				mascotData.byModel_Type = MASCOT_MODEL_TYPE_EGG; // Based on the egg icon
				mascotData.bySlot_Num = pMascotStatusData->bySlot_Num;
				mascotData.wSP_Decrease_Rate = 10;
				mascotData.wMax_SP = pMascotStatusData->wVpUpMax;
				
				// Set the name from status data
				wcscpy_s(mascotData.wszNameText, pMascotStatusData->wszNameText);
				
				OutputDebugStringA(("[MASCOT_ATTR_DEBUG] Created mascot data from status table with model: " + std::string(mascotData.szModel) + "\n").c_str());
				SetMascotTbl(&mascotData);
				return;
			}
			else
			{
				OutputDebugStringA(("[MASCOT_ATTR_DEBUG] MascotStatus data not found for tblidx: " + std::to_string(m_uiItemTblidx) + "\n").c_str());
			}
		}
		else
		{
			OutputDebugStringA("[MASCOT_ATTR_DEBUG] MascotStatus table not found\n");
		}
		
		// Try regular MascotTable as second option
		CMascotTable* pMascotTbl = API_GetTableContainer()->GetMascotTable();
		if (pMascotTbl)
		{
			OutputDebugStringA("[MASCOT_ATTR_DEBUG] Mascot table found\n");
			
			sMASCOT_TBLDAT* pMascotData = (sMASCOT_TBLDAT*)pMascotTbl->FindData(m_uiItemTblidx);
			if (pMascotData)
			{
				OutputDebugStringA(("[MASCOT_ATTR_DEBUG] Mascot data found, model name: " + std::string(pMascotData->szModel) + "\n").c_str());
				SetMascotTbl(pMascotData);
				return;
			}
			else
			{
				OutputDebugStringA(("[MASCOT_ATTR_DEBUG] Mascot data not found for tblidx: " + std::to_string(m_uiItemTblidx) + "\n").c_str());
			}
		}
		else
		{
			OutputDebugStringA("[MASCOT_ATTR_DEBUG] Mascot table not found\n");
		}
		
		// Try item table as final fallback
		OutputDebugStringA("[MASCOT_ATTR_DEBUG] Trying item table as fallback\n");
		CItemTable* pItemTbl = API_GetTableContainer()->GetItemTable();
		if (pItemTbl)
		{
			sITEM_TBLDAT* pItemData = (sITEM_TBLDAT*)pItemTbl->FindData(m_uiItemTblidx);
			if (pItemData)
			{
				OutputDebugStringA(("[MASCOT_ATTR_DEBUG] Item data found, icon name: " + std::string(pItemData->szIcon_Name) + "\n").c_str());
				OutputDebugStringA(("[MASCOT_ATTR_DEBUG] Item model name: " + std::string(pItemData->szModel) + "\n").c_str());
				
				// Create a fake mascot table entry using item data
				sMASCOT_TBLDAT fakeMascotData;
				memset(&fakeMascotData, 0, sizeof(fakeMascotData));
				
				// Use the actual model name from the item table
				strcpy_s(fakeMascotData.szModel, pItemData->szModel);
				fakeMascotData.tblidx = m_uiItemTblidx;
				fakeMascotData.byRank = pItemData->byRank;
				fakeMascotData.bValidity_Able = pItemData->bValidity_Able;
				fakeMascotData.byModel_Type = pItemData->byModel_Type;
				fakeMascotData.bySlot_Num = 1;
				fakeMascotData.wSP_Decrease_Rate = 10;
				fakeMascotData.wMax_SP = 100;
				
				// Set the name from item data
				wcscpy_s(fakeMascotData.wszNameText, pItemData->wszNameText);
				
				OutputDebugStringA(("[MASCOT_ATTR_DEBUG] Created mascot data with model: " + std::string(fakeMascotData.szModel) + "\n").c_str());
				SetMascotTbl(&fakeMascotData);
			}
			else
			{
				OutputDebugStringA(("[MASCOT_ATTR_DEBUG] Item data not found for tblidx: " + std::to_string(m_uiItemTblidx) + "\n").c_str());
			}
		}
		else
		{
			OutputDebugStringA("[MASCOT_ATTR_DEBUG] Item table not found\n");
		}
	}
	else
	{
		OutputDebugStringA("[MASCOT_ATTR_DEBUG] Invalid item tblidx\n");
	}
	
	OutputDebugStringA("[MASCOT_ATTR_DEBUG] SetItemTblidx - Exit\n");
}

void CNtlSobMascotAttr::SetCurrentVP(DWORD dwCurVP)
{
	m_dwCurVP = dwCurVP;
	SetEp((RwInt32)dwCurVP);
}

void CNtlSobMascotAttr::SetMaxVP(DWORD dwMaxVP)
{
	m_dwMaxVP = dwMaxVP;
	SetMaxEp((RwInt32)dwMaxVP);
}

void CNtlSobMascotAttr::SetCurrentExp(DWORD dwCurExp)
{
	m_dwCurExp = dwCurExp;
}

void CNtlSobMascotAttr::SetItemRank(BYTE byItemRank)
{
	m_byItemRank = byItemRank;
	
	// Load mascot grade table data when rank is set
	if (m_byItemRank > 0)
	{
		CMascotGradeTable* pMascotGradeTbl = API_GetTableContainer()->GetMascotGradeTable();
		if (pMascotGradeTbl)
		{
			sMASCOT_GRADE_TBLDAT* pGradeData = (sMASCOT_GRADE_TBLDAT*)pMascotGradeTbl->FindData(m_byItemRank);
			if (pGradeData)
			{
				SetMascotGradeTbl(pGradeData);
			}
		}
	}
}

void CNtlSobMascotAttr::SetSkillTblidx(int iIndex, TBLIDX uiSkillTblidx)
{
	if (iIndex >= 0 && iIndex < DBO_MASCOT_MAX_SKILL_COUNT)
	{
		m_skillTblidx[iIndex] = uiSkillTblidx;
	}
}

// Getters for mascot data
BYTE CNtlSobMascotAttr::GetMascotIndex(void) const
{
	return m_byMascotIndex;
}

TBLIDX CNtlSobMascotAttr::GetMascotTblidx(void) const
{
	return m_uiMascotTblidx;
}

TBLIDX CNtlSobMascotAttr::GetItemTblidx(void) const
{
	return m_uiItemTblidx;
}

DWORD CNtlSobMascotAttr::GetCurrentVP(void) const
{
	return m_dwCurVP;
}

DWORD CNtlSobMascotAttr::GetMaxVP(void) const
{
	return m_dwMaxVP;
}

DWORD CNtlSobMascotAttr::GetCurrentExp(void) const
{
	return m_dwCurExp;
}

BYTE CNtlSobMascotAttr::GetItemRank(void) const
{
	return m_byItemRank;
}

TBLIDX CNtlSobMascotAttr::GetSkillTblidx(int iIndex) const
{
	if (iIndex >= 0 && iIndex < DBO_MASCOT_MAX_SKILL_COUNT)
	{
		return m_skillTblidx[iIndex];
	}
	return INVALID_TBLIDX;
}

// Getters for additional mascot properties
bool CNtlSobMascotAttr::GetValidityAble(void) const
{
	return m_bValidityAble;
}

BYTE CNtlSobMascotAttr::GetModelType(void) const
{
	return m_byModelType;
}

BYTE CNtlSobMascotAttr::GetSlotNum(void) const
{
	return m_bySlotNum;
}

WORD CNtlSobMascotAttr::GetSPDecreaseRate(void) const
{
	return m_wSPDecreaseRate;
}