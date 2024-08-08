//***********************************************************************************
//
//	File		:	HLSItemTable.h
//
//	Begin		:	2009-8-14
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Chung Doo sup   ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "stdafx.h"
#include "HLSItemTable.h"
#include "NtlDebug.h"
#include "NtlBattle.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CHLSItemTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CHLSItemTable::CHLSItemTable(void)
{
	Init();
}

CHLSItemTable::~CHLSItemTable(void)
{
	Destroy();
}

bool CHLSItemTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CHLSItemTable::Destroy()
{
	CTable::Destroy();
}

void CHLSItemTable::Init()
{
}

void* CHLSItemTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sHLS_ITEM_TBLDAT* pNewItem = new sHLS_ITEM_TBLDAT;
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

bool CHLSItemTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sHLS_ITEM_TBLDAT* pItem = (sHLS_ITEM_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pItem, sizeof(*pItem)))
			return false;

		delete pItem;

		return true;
	}

	return false;
}

bool CHLSItemTable::AddTable(void * pvTable, bool bReload)
{
	sHLS_ITEM_TBLDAT * pTbldat = (sHLS_ITEM_TBLDAT*) pvTable;
	sHLS_ITEM_TBLDAT * pExistTbldat = NULL;
		
	if( bReload )
	{
		pExistTbldat = (sHLS_ITEM_TBLDAT*) FindData( pTbldat->tblidx );
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

bool CHLSItemTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{

	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sHLS_ITEM_TBLDAT* pItem = (sHLS_ITEM_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Name_Text"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			READ_STRINGW(bstrData, pItem->wszName, _countof(pItem->wszName));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"CJI_Product_ID"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			READ_STRINGW(bstrData, pItem->wszCJIProductID, _countof(pItem->wszCJIProductID));
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"HLS_Item_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->wHLSItemType = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Item_Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->itemTblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"On_Sale"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bOnSale = READ_BOOL(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Sell_Type"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bySellType = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Cash"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->dwCash = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Discount"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byDiscount = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Item_Stack_Count"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byStackCount = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"CJ_VIP"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->bCjVip = READ_BOOL(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Vip_Discount"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->byVipDiscount = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Display_Bit_Flag"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pItem->wDisplayBitFlag = (WORD)READ_BITFLAG( bstrData );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Sub_Item_Tblidx_", wcslen(L"Sub_Item_Tblidx_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_HLSITEM_SUB_ITEM; i++ )
			{
				swprintf( szBuffer, 1024, L"Sub_Item_Tblidx_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pItem->asSubItem[ i ].itemTblidx = READ_DWORD( bstrData );

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Sub_Item_Count_", wcslen(L"Sub_Item_Count_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < DBO_MAX_COUNT_HLSITEM_SUB_ITEM; i++ )
			{
				swprintf( szBuffer, 1024, L"Sub_Item_Count_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pItem->asSubItem[ i ].byStackCount = READ_BYTE(bstrData, pstrDataName->c_str());

					bFound = true;
					break;
				}
			}

			if( false == bFound )
			{
				CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
				return false;
			}
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Limited_Start_Year"))
		{
			pItem->sLimitedStart.year = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Limited_Start_Month"))
		{
			pItem->sLimitedStart.month = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Limited_Start_Day"))
		{
			pItem->sLimitedStart.day = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Limited_Start_Hour"))
		{
			pItem->sLimitedStart.hour = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Limited_End_Year"))
		{
			pItem->sLimitedEnd.year = READ_WORD(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Limited_End_Month"))
		{
			pItem->sLimitedEnd.month = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Limited_End_Day"))
		{
			pItem->sLimitedEnd.day = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Limited_End_Hour"))
		{
			pItem->sLimitedEnd.hour = READ_BYTE(bstrData, pstrDataName->c_str());
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Limited_Sell_Num"))
		{
			pItem->dwLimitedSellNum = READ_DWORD( bstrData );
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


sTBLDAT* CHLSItemTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

bool CHLSItemTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload)
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
		sHLS_ITEM_TBLDAT* pTableData = new sHLS_ITEM_TBLDAT;
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

bool CHLSItemTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sHLS_ITEM_TBLDAT* pTableData = (sHLS_ITEM_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}
