//***********************************************************************************
//
//	File		:	ItemUpgradeTable.cpp
//
//	Begin		:	2008-04-10
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//***********************************************************************************

#include "stdafx.h"
#include "ItemUpgradeTable.h"
#include "NtlDebug.h"
#include "NtlBattle.h"
#include "NtlSerializer.h"

WCHAR* CItemUpgradeTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CItemUpgradeTable::CItemUpgradeTable(void)
{
	Init();
}

CItemUpgradeTable::~CItemUpgradeTable(void)
{
	Destroy();
}

bool CItemUpgradeTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CItemUpgradeTable::Destroy()
{
	CTable::Destroy();
}

void CItemUpgradeTable::Init()
{
}

void* CItemUpgradeTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_UPGRADE_TBLDAT* pNewItem = new sITEM_UPGRADE_TBLDAT;
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

bool CItemUpgradeTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_UPGRADE_TBLDAT* pItem = (sITEM_UPGRADE_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pItem, sizeof(*pItem)))
			return false;

		delete pItem;
		return true;
	}

	return false;
}

bool CItemUpgradeTable::AddTable(void * pvTable, bool bReload)
{
	sITEM_UPGRADE_TBLDAT * pTbldat = (sITEM_UPGRADE_TBLDAT*) pvTable;
	sITEM_UPGRADE_TBLDAT * pExistTbldat = NULL;

	if( true == bReload )
	{
		pExistTbldat = (sITEM_UPGRADE_TBLDAT*)FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
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

bool CItemUpgradeTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{

	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sITEM_UPGRADE_TBLDAT* pItem = (sITEM_UPGRADE_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			READ_STRINGW(bstrData, pItem->wszNameText, _countof(pItem->wszNameText));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up1"))
		{
			pItem->wUp1 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up2"))
		{
			pItem->wUp2 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up3"))
		{
			pItem->wUp3 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up4"))
		{
			pItem->wUp4 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up5"))
		{
			pItem->wUp5 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up6"))
		{
			pItem->wUp6 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up7"))
		{
			pItem->wUp7 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up8"))
		{
			pItem->wUp8 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up9"))
		{
			pItem->wUp9 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up10"))
		{
			pItem->wUp10 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up11"))
		{
			pItem->wUp11 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up12"))
		{
			pItem->wUp12 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up13"))
		{
			pItem->wUp13 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up14"))
		{
			pItem->wUp14 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Up15"))
		{
			pItem->wUp15 = READ_WORD( bstrData, pstrDataName->c_str(), 0 );
		}
	}
	else
	{
		return false;
	}

	return true;
}


sTBLDAT* CItemUpgradeTable::FindData(TBLIDX tblidx)
{
	if ( 0 == tblidx )
	{
		return NULL;
	}

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if ( End() == iter )
	{
		return NULL;
	}

	return (sTBLDAT*)(iter->second); 
}

bool CItemUpgradeTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload)
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
		sITEM_UPGRADE_TBLDAT* pTableData = new sITEM_UPGRADE_TBLDAT;
		if ( NULL == pTableData )
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

bool CItemUpgradeTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sITEM_UPGRADE_TBLDAT* pTableData = (sITEM_UPGRADE_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}