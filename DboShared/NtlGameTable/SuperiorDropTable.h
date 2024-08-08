#pragma once
//***********************************************************************************
//
//	File		:	SuperiorDropTable.h
//
//	Begin		:	2006-06-2
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
struct sSUPERIOR_DROP_TBLDAT : public sTBLDAT
{
public:
	
		BYTE		byMax_Quality;
		float		fOption_Rate;
		TBLIDX		aItem_Tblidx[NTL_MAX_SUPERIOR_DROP];
		float		afDrop_Rate[NTL_MAX_SUPERIOR_DROP];

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CSuperiorDropTable :	public CTable
{
public:
	CSuperiorDropTable(void);

	virtual ~CSuperiorDropTable(void);

public:

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	static TBLIDX		FindDropIndex( sSUPERIOR_DROP_TBLDAT* psTblData, BYTE byIndex);
	static float		FindDropRate( sSUPERIOR_DROP_TBLDAT* psTblData, BYTE byIndex);

protected:
	WCHAR** GetSheetListInWChar() { return &(CSuperiorDropTable::m_pwszSheetList[0]); }
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