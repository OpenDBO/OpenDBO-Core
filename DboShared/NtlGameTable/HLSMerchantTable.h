#pragma once

#include "Table.h"
#include "NtlItem.h"

const DWORD		DBO_MAX_LENGTH_HLSMERCHANT_NAME_TEXT = 10;

#pragma pack(push, 4)
struct sHLS_MERCHANT_TBLDAT : public sTBLDAT
{
public:
	TBLIDX			Tab_Name;
	WCHAR			wszNameText[DBO_MAX_LENGTH_HLSMERCHANT_NAME_TEXT + 1];
	BYTE			byTabType;//
	TBLIDX			startItemTblidx;
	TBLIDX			endItemTblidx;

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CHLSMerchantTable : public CTable
{
public:
	CHLSMerchantTable(void);
	virtual ~CHLSMerchantTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT* FindData(TBLIDX tblidx);
	static TBLIDX FindMerchantItem(sHLS_MERCHANT_TBLDAT* psTbldat, BYTE byIndex);

protected:
	WCHAR** GetSheetListInWChar() { return &(CHLSMerchantTable::m_pwszSheetList[0]); }
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