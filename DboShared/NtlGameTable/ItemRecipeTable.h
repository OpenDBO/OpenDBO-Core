//***********************************************************************************
//
//	File		:	ItemRecipeTable.h
//
//	Begin		:	2006-03-09
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Chung Doo sup   ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "Table.h"
#include "NtlItem.h"

const DWORD		DBO_MAX_COUNT_RECIPE_CREATE_ITEM = 3;


#pragma pack(push, 4)

struct sRECIPE_CREATE_DATA
{
	TBLIDX	itemTblidx;			// 재료테이블 인덱스 INVALID_
	BYTE	itemRate;
	TBLIDX	itemTblidxGreate;
};


struct sRECIPE_MATERIAL_DATA
{
	TBLIDX	materialTblidx;			// 재료테이블 인덱스 INVALID_
	BYTE	byMaterialCount;
};

struct sITEM_RECIPE_TBLDAT : public sTBLDAT
{	
	sITEM_RECIPE_TBLDAT()
	{
		bValidityAble = true;
	};
public:
	bool						bValidityAble;
	DWORD						dwName;
	BYTE						byRecipeType;
	BYTE						byNeedMixLevel;
	DWORD						dwNeedMixZenny;
	sRECIPE_CREATE_DATA 		asCreateItemTblidx[DBO_MAX_COUNT_RECIPE_CREATE_ITEM];
	TBLIDX						createJunkTblidx;
	sRECIPE_MATERIAL_DATA		asMaterial[DBO_MAX_COUNT_RECIPE_MATERIAL_ITEM];

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CItemRecipeTable : public CTable
{
public:

	CItemRecipeTable(void);
	virtual ~CItemRecipeTable(void);
	
	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 
	
protected:
	WCHAR** GetSheetListInWChar() { return &(CItemRecipeTable::m_pwszSheetList[0]); }
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