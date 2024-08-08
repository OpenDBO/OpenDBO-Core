#pragma once
//***********************************************************************************
//
//	File		:	LegendaryDropTable.h
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
struct sLEGENDARY_DROP_TBLDAT : public sTBLDAT
{
public:

	TBLIDX		aItem_Tblidx[NTL_MAX_LEGENDARY_DROP];
	float		afDrop_Rate[NTL_MAX_LEGENDARY_DROP];

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CLegendaryDropTable :	public CTable
{
public:
	CLegendaryDropTable(void);

	virtual ~CLegendaryDropTable(void);

public:

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	static TBLIDX		FindDropIndex( sLEGENDARY_DROP_TBLDAT* psTblData, BYTE byIndex);
	static float		FindDropRate( sLEGENDARY_DROP_TBLDAT* psTblData, BYTE byIndex);

protected:
	WCHAR** GetSheetListInWChar() { return &(CLegendaryDropTable::m_pwszSheetList[0]); }
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