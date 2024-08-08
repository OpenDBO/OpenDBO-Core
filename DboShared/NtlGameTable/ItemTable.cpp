//***********************************************************************************
//
//	File		:	ItemTable.cpp
//
//	Begin		:	2006-03-09
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Doo Sup, Chung ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "ItemTable.h"
#include "NtlDebug.h"
#include "NtlBattle.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CItemTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CItemTable::CItemTable(void)
{
	Init();
}

CItemTable::~CItemTable(void)
{
	Destroy();
}

bool CItemTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CItemTable::Destroy()
{
	CTable::Destroy();
}

void CItemTable::Init()
{
}

void* CItemTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_TBLDAT* pNewItem = new sITEM_TBLDAT;
		if (NULL == pNewItem)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewItem;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewItem;
	}

	return NULL;
}

bool CItemTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_TBLDAT* pItem = (sITEM_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pItem, sizeof(*pItem)))
			return false;

		delete pItem;

		return true;
	}

	return false;
}

bool CItemTable::AddTable(void * pvTable, bool bReload)
{
	sITEM_TBLDAT * pTbldat = (sITEM_TBLDAT*) pvTable;
	sITEM_TBLDAT * pExistTbldat = NULL;
		
	if ( false == pTbldat->bValidity_Able )
	{
		return false;
	}

	if( bReload )
	{
		pExistTbldat = (sITEM_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// 데이타의 리로드 성공을 위한 true 반환
			return true;  
		}
	}

	if ( false == m_mapTableList.insert( std::map<TBLIDX, sTBLDAT*>::value_type(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CItemTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_TBLDAT* pItem = (sITEM_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			READ_STRINGW(bstrData, pItem->wszNameText, _countof(pItem->wszNameText));
		}
		else if ( 0 == wcscmp(pstrDataName->c_str(), L"Validity_Able") )
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bValidity_Able = READ_BOOL( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name"))
		{
			pItem->Name = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Icon_Name"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );

			READ_STRING(bstrData, pItem->szIcon_Name, _countof(pItem->szIcon_Name));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Model_Type"))
		{
			pItem->byModel_Type = READ_BYTE(bstrData, pstrDataName->c_str());

			//  [12/1/2006 zeroera] : 설명 : 테이블 체크로 옮길 것
			//if (ITEM_MODEL_TYPE_FIRST > pItem->byModel_Type || ITEM_MODEL_TYPE_LAST < pItem->byModel_Type)
			//{
			//	_ASSERT(0);
			//}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Model"))
		{
			READ_STRING(bstrData, pItem->szModel, _countof(pItem->szModel));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sub_Weapon_Act_Model"))
		{
			READ_STRING(bstrData, pItem->szSub_Weapon_Act_Model, _countof(pItem->szSub_Weapon_Act_Model));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Item_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byItem_Type = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Equip_Type"))
		{
			pItem->byEquip_Type = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Equip_Slot_Type_Bit_Flag"))
		{
			pItem->wEquip_Slot_Type_Bit_Flag = (WORD)READ_BITFLAG( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Function_Bit_Flag"))
		{
			pItem->wFunction_Bit_Flag = (WORD)READ_BITFLAG( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Max_Stack"))
		{
			pItem->byMax_Stack = READ_BYTE( bstrData, pstrDataName->c_str(), NTL_UNSTACKABLE_ITEM_COUNT );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Rank"))
		{
			pItem->byRank = READ_BYTE( bstrData, pstrDataName->c_str(), ITEM_RANK_NOTHING );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Cost"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->dwCost = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sell_Price"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bySell_Price = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Durability"))
		{
			pItem->byDurability = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Durability_Count"))
		{
			pItem->byDurability_Count = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Battle_Attribute"))
		{
			pItem->byBattle_Attribute = READ_BYTE(bstrData, pstrDataName->c_str(), BATTLE_ATTRIBUTE_NONE);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Physical_Offence"))
		{
			pItem->wPhysical_Offence = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Energy_Offence"))
		{
			pItem->wEnergy_Offence = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Physical_Defence"))
		{
			pItem->wPhysical_Defence = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Energy_Defence"))
		{
			pItem->wEnergy_Defence = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Physical_OffenceUpgrade"))
		{
			pItem->dwPhysical_OffenceUpgrade = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Energy_OffenceUpgrade"))
		{
			pItem->dwEnergy_OffenceUpgrade = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Physical_DefenceUpgrade"))
		{
			pItem->dwPhysical_DefenceUpgrade = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Energy_DefenceUpgrade"))
		{
			pItem->dwEnergy_DefenceUpgrade = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Range_Bonus"))
		{
			pItem->fAttack_Range_Bonus = READ_FLOAT(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Attack_Speed_Rate"))
		{
			pItem->wAttack_Speed_Rate = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Level"))
		{
			pItem->byNeed_Level = READ_BYTE(bstrData, pstrDataName->c_str(), 0);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Class_Bit_Flag"))
		{			
			pItem->dwNeed_Class_Bit_Flag = READ_BITFLAG( bstrData, NTL_ITEM_ALL_USE_FLAG);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Class_Special"))
		{
			pItem->byClass_Special = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Race_Special"))
		{
			pItem->byRace_Special = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Str"))
		{
			pItem->byNeed_Str = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Con"))
		{
			pItem->byNeed_Con = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Foc"))
		{
			pItem->byNeed_Foc = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Dex"))
		{
			pItem->byNeed_Dex = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Sol"))
		{
			pItem->byNeed_Sol = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Eng"))
		{
			pItem->byNeed_Eng = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Set_Item_Tblidx"))
		{
			pItem->set_Item_Tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Note"))
		{
			pItem->Note = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Bag_Size"))
		{
			pItem->byBag_Size = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Scouter_Watt"))
		{
			pItem->wScouter_Watt = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Scouter_MaxPower"))
		{
			pItem->dwScouter_MaxPower = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Scouter_Parts_Type"))
		{
			pItem->byScouter_Parts_Type = READ_BYTE(bstrData, pstrDataName->c_str(), SCOUTER_PARTS_NONE);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Scouter_Parts_Value"))
		{
			pItem->byScouter_Parts_Value = READ_BYTE(bstrData, pstrDataName->c_str(), SCOUTER_PARTS_NONE);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Use_Item_Tblidx"))
		{
			pItem->Use_Item_Tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Item_Option_Tblidx"))
		{
			pItem->Item_Option_Tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Item_Group"))
		{
			pItem->byItemGroup = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Charm_Tblidx"))
		{
			pItem->Charm_Tblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Costume_Hide_Bit_Flag"))
		{
			pItem->wCostumeHideBitFlag = (WORD)READ_BITFLAG(bstrData);
		}	
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Item_Tblidx"))
		{
			pItem->NeedItemTblidx = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Common_Point"))
		{
			pItem->CommonPoint = READ_DWORD(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Common_Point_Type"))
		{
			pItem->byCommonPointType = READ_BYTE(bstrData, pstrDataName->c_str());
		}		
		else if (0 == wcscmp(pstrDataName->c_str(), L"Need_Function"))
		{
			pItem->byNeedFunction = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"Use_Duration_Max" ))
		{
			pItem->dwUseDurationMax = READ_DWORD( bstrData );
		}
		else if(0 == wcscmp(pstrDataName->c_str(), L"Duration_Type" ))
		{
			pItem->byDurationType = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Contents_Tblidx"))
		{
			pItem->contentsTblidx = READ_TBLIDX(bstrData);
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Duration_Group"))
		{
			pItem->dwDurationGroup = READ_DWORD(bstrData);
		}
		else
		{
			CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


sTBLDAT* CItemTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

bool CItemTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload)
{
	if( false == bReload )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	bool bLoop = true;
	do
	{
		sITEM_TBLDAT* pTableData = new sITEM_TBLDAT;
		if (NULL == pTableData)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		if (false == pTableData->LoadFromBinary(serializer))
		{
			delete pTableData;
			bLoop = false;
			break;
		}

		//  [4/26/2008 zeroera] : 설명 : 실패하더라도 Load의 종료여부는 File Loading에서 결정한다
		if( false == AddTable(pTableData, bReload) )
		{
			delete pTableData;
		}

	} while (false != bLoop);

	return true;
}

bool CItemTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sITEM_TBLDAT* pTableData = (sITEM_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}
