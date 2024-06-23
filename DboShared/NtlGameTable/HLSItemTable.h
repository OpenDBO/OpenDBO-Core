#pragma once
//last update 20.08.2015
#include "Table.h"
#include "NtlItem.h"

#include "NtlHlsItem.h"

struct sSUB_ITEM
{
	TBLIDX	itemTblidx;
	BYTE	byStackCount;
	
};
#pragma pack(push, 4)
struct sHLS_ITEM_TBLDAT : public sTBLDAT
{

public:

	WCHAR           wszName[32 + 1];
	WCHAR           wszCJIProductID[128 + 1];
	WCHAR           szIcon_Name[32 + 1];

	WORD            wHLSItemType;
	BYTE            byHLSDurationType;
	
	DWORD           dwHLSDurationTime;

	TBLIDX          idxNameText;
	TBLIDX          idxNoteText;
	TBLIDX          itemTblidx;

	bool            bOnSale;
	BYTE            bySellType;
	
	DWORD           dwCash;
	BYTE            byDiscount;
	BYTE            byStackCount;
	WORD            wDisplayBitFlag;        //eHLS_DISPLAY_ITEM_FUNC_BIT_FLAG
	sSUB_ITEM       asSubItem[DBO_MAX_COUNT_HLSITEM_SUB_ITEM];
	BYTE            byQuickLink;
	
	DWORD           dwPriority;
	BYTE            byDisplayConsumeType;
	BYTE            byYadratType;
	

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CHLSItemTable : public CTable
{
public:

	CHLSItemTable(void);
	virtual ~CHLSItemTable(void);
	
	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CHLSItemTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload, bool bUpdate);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

public:
	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate);
	virtual bool				SaveToBinary(CNtlSerializer& serializer);

private:
	static WCHAR* m_pwszSheetList[];
};