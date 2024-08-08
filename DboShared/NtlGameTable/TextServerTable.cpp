#include "StdAfx.h"
#include "TextServerTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"

//- yoshiki : Let's consider of implementing NtlAssert series.
//#include "NtlAssert.h"

WCHAR* CTextServerTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CTextServerTable::CTextServerTable(void)
{
	Init();
}

CTextServerTable::~CTextServerTable(void)
{
	Destroy();
}

bool CTextServerTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CTextServerTable::Destroy()
{
	CTable::Destroy();
}

void CTextServerTable::Init()
{
}

void* CTextServerTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sTEXT_SERVER_TBLDAT* pText = new sTEXT_SERVER_TBLDAT;
		if (NULL == pText)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pText;
			return NULL;
		}

		m_dwCodePage = dwCodePage;
		return pText;
	}

	return NULL;
}

bool CTextServerTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sTEXT_SERVER_TBLDAT* pText = (sTEXT_SERVER_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pText, sizeof(*pText)))
			return false;

		delete pText;

		return true;
	}

	return false;
}

bool CTextServerTable::AddTable(void * pvTable, bool bReload)
{
	UNREFERENCED_PARAMETER( bReload );

	sTEXT_SERVER_TBLDAT* pTbldat = (sTEXT_SERVER_TBLDAT*) pvTable;

	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}

	return true;
}

bool CTextServerTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sTEXT_SERVER_TBLDAT* pText = (sTEXT_SERVER_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Server_Text_Index"))
		{
			pText->tblidx = READ_DWORD( bstrData );
		}
		else if (0 == wcscmp(pstrDataName->c_str(), L"Server_Text"))
		{
			READ_STR( pText->wstrText, bstrData);
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


sTBLDAT* CTextServerTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;

	return (sTBLDAT*)(iter->second); 
}

bool CTextServerTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload)
{
	if( false == bReload )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	while (0 < serializer.GetDataSize())
	{
		sTEXT_SERVER_TBLDAT* pTableData = new sTEXT_SERVER_TBLDAT;
		if (NULL == pTableData)
		{
			//- yoshiki : To log system!
			Destroy();
			return false;
		}

		WORD wTextLength = 0;
		if (serializer.GetDataSize() < sizeof(pTableData->tblidx) + sizeof(wTextLength))
		{
			//- yoshiki : To log system!

			delete pTableData;

			Destroy();
			return false;
		}
		serializer >> pTableData->tblidx;
		serializer >> wTextLength;

		if (serializer.GetDataSize() < (int)(wTextLength * sizeof(WCHAR)))
		{
			//- yoshiki : To log system!

			delete pTableData;

			Destroy();
			return false;
		}

		WCHAR* pwszText = new WCHAR[wTextLength + 1];
		if (NULL == pwszText)
		{
			//- yoshiki : To log system!

			delete pTableData;

			Destroy();
			return false;
		}
		serializer.Out(pwszText, wTextLength * sizeof(WCHAR));
		pwszText[wTextLength] = L'\0';

		pTableData->wstrText = pwszText;

		delete [] pwszText;

		//  [4/26/2008 zeroera] : 설명 : 실패하더라도 Load의 종료여부는 File Loading에서 결정한다
		if( false == AddTable(pTableData, bReload) )
		{
			delete pTableData;
		}
	};

	return true;
}

bool CTextServerTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sTEXT_SERVER_TBLDAT* pTableData = (sTEXT_SERVER_TBLDAT*)(iter->second);

		serializer << pTableData->tblidx;
		serializer << (WORD)((pTableData->wstrText).size());
		serializer.In((pTableData->wstrText).c_str(), (int)((pTableData->wstrText).size() * sizeof(WCHAR)));
	}

	return true;
}
