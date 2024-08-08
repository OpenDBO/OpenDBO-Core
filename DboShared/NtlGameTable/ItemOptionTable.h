//***********************************************************************************
//
//	File		:	ItemOptionTable.h
//
//	Begin		:	2006-06-12
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

const DWORD		DBO_MAX_LENGTH_ITEM_OPTION_NAME = 30;
const DWORD		DBO_MAX_LENGTH_ITEM_OPTION_NOTE = 128;

#pragma pack(push, 4)
struct sITEM_OPTION_TBLDAT : public sTBLDAT
{
	sITEM_OPTION_TBLDAT()
	{
		bValidity_Able = true;
	};

public:
//	std::string		strOption_Name;
	WCHAR			wszOption_Name[ DBO_MAX_LENGTH_ITEM_OPTION_NAME + 1 ];
	bool			bValidity_Able;
	BYTE			byOption_Rank;
	BYTE			byItem_Group;
	BYTE			byMaxQuality;		// 해당 그룹의 퀄러티의 총수
	BYTE			byQuality;			// 미니테이블에 있는 퀄러티
	BYTE			byQualityIndex;		// 몇번째 퀄러티
	DWORD			dwCost;
	BYTE			byLevel;
	TBLIDX			system_Effect[NTL_MAX_SYSTEM_EFFECT_COUNT];
	bool			bAppliedInPercent[NTL_MAX_SYSTEM_EFFECT_COUNT];
	int				nValue[NTL_MAX_SYSTEM_EFFECT_COUNT];

	TBLIDX			activeEffect;
	float			fActiveRate;
//	std::string		strNote;
	char			szNote[ DBO_MAX_LENGTH_ITEM_OPTION_NOTE + 1 ];
	BYTE			byScouterInfo;   // cf) Defined by eSCOUTER_INFO of NtlItem.h 

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CItemOptionTable : public CTable
{
public:

	CItemOptionTable(void);
	virtual ~CItemOptionTable(void);
	
	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *					FindData(TBLIDX tblidx); 

	//sTBLDAT *					GetOptionTbldat(BYTE byType, BYTE byDefLevel, BYTE byRank, BYTE byCount);

	//bool						SetOptionTbldat(BYTE byType, BYTE byDefLevel, BYTE byRank, BYTE byCount, sTBLDAT * pTbldat);
protected:
	WCHAR** GetSheetListInWChar() { return &(CItemOptionTable::m_pwszSheetList[0]); }
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