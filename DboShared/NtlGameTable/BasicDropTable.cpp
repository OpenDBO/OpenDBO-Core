//***********************************************************************************
//
//	File		:	BasicDropTable.cpp
//
//	Begin		:	2006-03-27
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Doo  Sup, Chung   ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "BasicDropTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

WCHAR* CBasicDropTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CBasicDropTable::CBasicDropTable(void)
{
	Init();
}

CBasicDropTable::~CBasicDropTable(void)
{
	Destroy();
}

bool CBasicDropTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CBasicDropTable::Destroy()
{
	CTable::Destroy();
}

void CBasicDropTable::Init()
{
}

void* CBasicDropTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sBASIC_DROP_TBLDAT* pDrop = new sBASIC_DROP_TBLDAT;
		if (NULL == pDrop)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pDrop;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pDrop;
	}

	return NULL;
}

bool CBasicDropTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sBASIC_DROP_TBLDAT* pDrop = (sBASIC_DROP_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pDrop, sizeof(*pDrop)))
			return false;

		delete pDrop;

		return true;
	}

	return false;
}

bool CBasicDropTable::AddTable(void * pvTable, bool bReload)
{
	sBASIC_DROP_TBLDAT * pTbldat = (sBASIC_DROP_TBLDAT*)pvTable;
	sBASIC_DROP_TBLDAT * pExistTbldat = NULL;

	if( bReload )
	{
		pExistTbldat = (sBASIC_DROP_TBLDAT*) FindData( pTbldat->tblidx );
		if( pExistTbldat )
		{
			CopyMemory( pTbldat, pExistTbldat, pTbldat->GetDataSize() );
			// 데이타의 해제를 위한 false 반환
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

bool CBasicDropTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sBASIC_DROP_TBLDAT* pDrop = (sBASIC_DROP_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDrop->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Max"))
		{
			CheckNegativeInvalid( pstrDataName->c_str(), bstrData );
			pDrop->byMax = READ_BYTE( bstrData, pstrDataName->c_str() );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Normal_Tblidx_Rate_", wcslen(L"Normal_Tblidx_Rate_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_DROP_TABLE_SELECT; i++ )
			{
				swprintf( szBuffer, 1024, L"Normal_Tblidx_Rate_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pDrop->afNoramalTblidxRate[ i ] = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Normal_Drop_Tblidx_", wcslen(L"Normal_Drop_Tblidx_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_DROP_TABLE_SELECT; i++ )
			{
				swprintf( szBuffer, 1024, L"Normal_Drop_Tblidx_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pDrop->aNoramalDropTblidx[ i ] = READ_DWORD( bstrData );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Superior_Tblidx_Rate_", wcslen(L"Superior_Tblidx_Rate_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_DROP_TABLE_SELECT; i++ )
			{
				swprintf( szBuffer, 1024, L"Superior_Tblidx_Rate_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pDrop->afSuperiorTblidxRate[ i ] = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Superior_Drop_Tblidx_", wcslen(L"Superior_Drop_Tblidx_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_DROP_TABLE_SELECT; i++ )
			{
				swprintf( szBuffer, 1024, L"Superior_Drop_Tblidx_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pDrop->aSuperiorDropTblidx[ i ] = READ_DWORD( bstrData );

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
		else if (0 == wcscmp(pstrDataName->c_str(), L"Superior_Drop_Rate_Control"))
		{
			pDrop->fSuperior_Drop_Rate_Control = READ_FLOAT( bstrData, pstrDataName->c_str(), 1.0f );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Superior_Option_Rate_Control"))
		{
			pDrop->fSuperior_Option_Rate_Control = READ_FLOAT( bstrData, pstrDataName->c_str(), 1.0f );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Excellent_Tblidx_Rate_", wcslen(L"Excellent_Tblidx_Rate_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_DROP_TABLE_SELECT; i++ )
			{
				swprintf( szBuffer, 1024, L"Excellent_Tblidx_Rate_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pDrop->afExcellentTblidxRate[ i ] = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Excellent_Drop_Tblidx_", wcslen(L"Excellent_Drop_Tblidx_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_DROP_TABLE_SELECT; i++ )
			{
				swprintf( szBuffer, 1024, L"Excellent_Drop_Tblidx_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pDrop->aExcellentDropTblidx[ i ] = READ_DWORD( bstrData );

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
		else if (0 == wcscmp(pstrDataName->c_str(), L"Excellent_Drop_Rate_Control"))
		{
			pDrop->fExcellent_Drop_Rate_Control = READ_FLOAT( bstrData, pstrDataName->c_str(), 1.0f );
		}
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Legendary_Tblidx_Rate_", wcslen(L"Legendary_Tblidx_Rate_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_DROP_TABLE_SELECT; i++ )
			{
				swprintf( szBuffer, 1024, L"Legendary_Tblidx_Rate_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pDrop->afLegendaryTblidxRate[ i ] = READ_FLOAT( bstrData, pstrDataName->c_str(), 0.0f );

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
		else if ( 0 == wcsncmp(pstrDataName->c_str(), L"Legendary_Drop_Tblidx_", wcslen(L"Legendary_Drop_Tblidx_") ) )
		{
			bool bFound = false;

			WCHAR szBuffer[1024] = { 0x00, };
			for( int i = 0; i < NTL_MAX_DROP_TABLE_SELECT; i++ )
			{
				swprintf( szBuffer, 1024, L"Legendary_Drop_Tblidx_%d", i + 1 );

				if( 0 == wcscmp(pstrDataName->c_str(), szBuffer) )
				{
					pDrop->aLegendaryDropTblidx[ i ] = READ_DWORD( bstrData );

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
		else if (0 == wcscmp(pstrDataName->c_str(), L"Legendary_Drop_Rate_Control"))
		{
			pDrop->fLegendary_Drop_Rate_Control = READ_FLOAT( bstrData, pstrDataName->c_str(), 1.0f );
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


sTBLDAT* CBasicDropTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

TBLIDX CBasicDropTable::FindNormalDropIndex( sBASIC_DROP_TBLDAT* psTblData, BYTE byIndex)
{
	if ( NTL_MAX_DROP_TABLE_SELECT <= byIndex || 0 > byIndex )
		return INVALID_TBLIDX;

	return psTblData->aNoramalDropTblidx[byIndex];
}

float CBasicDropTable::FindNormalDropRate( sBASIC_DROP_TBLDAT* psTblData, BYTE byIndex)
{
	if ( NTL_MAX_DROP_TABLE_SELECT <= byIndex || 0 > byIndex )
		return 0.0f;

	return psTblData->afNoramalTblidxRate[byIndex];
}

TBLIDX CBasicDropTable::FindSuperiorDropIndex( sBASIC_DROP_TBLDAT* psTblData, BYTE byIndex)
{
	if ( NTL_MAX_DROP_TABLE_SELECT <= byIndex || 0 > byIndex )
		return INVALID_TBLIDX;

	return psTblData->aSuperiorDropTblidx[byIndex];
}

float CBasicDropTable::FindSuperiorDropRate( sBASIC_DROP_TBLDAT* psTblData, BYTE byIndex)
{
	if ( NTL_MAX_DROP_TABLE_SELECT <= byIndex || 0 > byIndex )
		return 0.0f;

	return psTblData->afSuperiorTblidxRate[byIndex];
}

TBLIDX CBasicDropTable::FindLegendaryDropIndex( sBASIC_DROP_TBLDAT* psTblData, BYTE byIndex)
{
	if ( NTL_MAX_DROP_TABLE_SELECT <= byIndex || 0 > byIndex )
		return INVALID_TBLIDX;

	return psTblData->aLegendaryDropTblidx[byIndex];
}

float CBasicDropTable::FindLegendaryDropRate( sBASIC_DROP_TBLDAT* psTblData, BYTE byIndex)
{
	if ( NTL_MAX_DROP_TABLE_SELECT <= byIndex || 0 > byIndex )
		return 0.0f;

	return psTblData->afLegendaryTblidxRate[byIndex];
}

TBLIDX CBasicDropTable::FindExcellentDropIndex( sBASIC_DROP_TBLDAT* psTblData, BYTE byIndex)
{
	if ( NTL_MAX_DROP_TABLE_SELECT <= byIndex || 0 > byIndex )
		return INVALID_TBLIDX;

	return psTblData->aExcellentDropTblidx[byIndex];
}

float CBasicDropTable::FindExcellentDropRate( sBASIC_DROP_TBLDAT* psTblData, BYTE byIndex)
{
	if ( NTL_MAX_DROP_TABLE_SELECT <= byIndex || 0 > byIndex )
		return 0.0f;

	return psTblData->afExcellentTblidxRate[byIndex];
}
bool CBasicDropTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload)
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
		sBASIC_DROP_TBLDAT* pTableData = new sBASIC_DROP_TBLDAT;
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

bool CBasicDropTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sBASIC_DROP_TBLDAT* pTableData = (sBASIC_DROP_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}