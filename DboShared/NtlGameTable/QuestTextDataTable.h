#pragma once

#include <vector>
#include "Table.h"

// Quest Text 모음
#pragma pack(push, 4)
struct sQUEST_TEXT_DATA_TBLDAT : public sTBLDAT
{
public:
	std::wstring wstrText;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CQuestTextDataTable : public CTable
{
public:

	CQuestTextDataTable(void);
	virtual ~CQuestTextDataTable(void);

/*
public:

	BOOL				Check() { return TRUE; }*/

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

	// Tool 에서 사용하기 위해 만든 함수
	// 속도가 느림 !
	void				FindDataRange( TBLIDX tblBegin, TBLIDX tblEnd, std::vector<sQUEST_TEXT_DATA_TBLDAT*>& vecRangeList );
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CQuestTextDataTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:
	static WCHAR* m_pwszSheetList[];
};
