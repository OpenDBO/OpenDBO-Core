#include "StdAfx.h"
#include "GameManiaTimeTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CGameManiaTimeTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CGameManiaTimeTable::CGameManiaTimeTable(void)
{
	Init();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CGameManiaTimeTable::~CGameManiaTimeTable(void)
{
	Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CGameManiaTimeTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CGameManiaTimeTable::Destroy()
{
	CTable::Destroy();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CGameManiaTimeTable::Init()
{
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void* CGameManiaTimeTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sGMT_TBLDAT* pGMT = new sGMT_TBLDAT;
		if (NULL == pGMT)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pGMT;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pGMT;
	}

	return NULL;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CGameManiaTimeTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sGMT_TBLDAT* pGMT = (sGMT_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pGMT, sizeof(*pGMT)))
			return false;

		delete pGMT;

		return true;
	}

	return false;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CGameManiaTimeTable::AddTable(void * pvTable, bool bReload)
{
	UNREFERENCED_PARAMETER( bReload );

	sGMT_TBLDAT* pTbldat = (sGMT_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CGameManiaTimeTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sGMT_TBLDAT* pGMT = (sGMT_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pGMT->tblidx = READ_TBLIDX( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Effect_Duration_Time"))
		{
			pGMT->byEffectTerm = READ_BYTE( bstrData, L"Effect_Duration_Time" );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Effect_Type"))
		{
			pGMT->byEffectType = READ_BYTE( bstrData, L"Effect_Type" );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Effect_Value"))
		{
			pGMT->byEffectValue = READ_BYTE( bstrData, L"Effect_Value" );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Effect_Time_Num"))
		{
			pGMT->byMaxTimeNumber = READ_BYTE( bstrData, L"Effect_Time_Num" );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Effect_Time_Set1"))
		{
			pGMT->abyTimeSet[0] = READ_BYTE( bstrData, L"Effect_Time_Set1" );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Effect_Time_Set2"))
		{
			pGMT->abyTimeSet[1] = READ_BYTE( bstrData, L"Effect_Time_Set2" );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Effect_Time_Set3"))
		{
			pGMT->abyTimeSet[2] = READ_BYTE( bstrData, L"Effect_Time_Set3" );
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


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
sTBLDAT* CGameManiaTimeTable::FindData(TBLIDX tblidx)
{
	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CGameManiaTimeTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload)
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
		sGMT_TBLDAT* pTableData = new sGMT_TBLDAT;
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


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CGameManiaTimeTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sGMT_TBLDAT* pTableData = (sGMT_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}
