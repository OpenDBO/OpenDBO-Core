#pragma once
//***********************************************************************************
//
//	File		:	TypeDropTable.h
//
//	Begin		:	2008-01-21
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Doo Sup, Chung   ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#include "Table.h"
#include "NtlVector.h"
#include "NtlItem.h"

#pragma pack(push, 4)
struct sTYPE_DROP_TBLDAT : public sTBLDAT
{
public:

	TBLIDX		aItem_Tblidx[NTL_MAX_TYPE_DROP];
	float		afDrop_Rate[NTL_MAX_TYPE_DROP];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CTypeDropTable :	public CTable
{
public:
	CTypeDropTable(void);

	virtual ~CTypeDropTable(void);

public:

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	static TBLIDX		FindDropIndex( sTYPE_DROP_TBLDAT* psTblData, BYTE byIndex);
	static float		FindDropRate( sTYPE_DROP_TBLDAT* psTblData, BYTE byIndex);

protected:
	WCHAR** GetSheetListInWChar() { return &(CTypeDropTable::m_pwszSheetList[0]); }
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