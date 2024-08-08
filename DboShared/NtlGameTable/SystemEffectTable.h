//***********************************************************************************
//
//	File		:	SystemEffectTable.h
//
//	Begin		:	2006-08-31
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "Table.h"
#include "NtlSkill.h"

const DWORD		DBO_MAX_LENGTH_SYSTEM_EFFECT_NAME = 40;
const DWORD		DBO_MAX_LENGTH_SYSTEM_EFFECT_SUCCESS_EFFECT_NAME = 32;
const DWORD		DBO_MAX_LENGTH_SYSTEM_EFFECT_SUCCESS_END_EFFECT_NAME = 32;

enum eSYSTEM_EFFECT_APPLY_TYPE
{
	SYSTEM_EFFECT_APPLY_TYPE_VALUE = 0,
	SYSTEM_EFFECT_APPLY_TYPE_PERCENT,
	SYSTEM_EFFECT_APPLY_TYPE_MAX_LP,
	SYSTEM_EFFECT_APPLY_TYPE_MAX_EP,
	SYSTEM_EFFECT_APPLY_TYPE_MAX_RP,
	SYSTEM_EFFECT_APPLY_TYPE_CURRENT_LP,
	SYSTEM_EFFECT_APPLY_TYPE_CURRENT_EP,
	SYSTEM_EFFECT_APPLY_TYPE_CURRENT_RP,
	SYSTEM_EFFECT_APPLY_TYPE_CURRENT_PHYSICAL_OFFENCE,
	SYSTEM_EFFECT_APPLY_TYPE_CURRENT_ENERGY_OFFENCE,
	SYSTEM_EFFECT_APPLY_TYPE_CURRENT_PHYSICAL_DEFENCE,
	SYSTEM_EFFECT_APPLY_TYPE_CURRENT_ENERGY_DEFENCE,

	SYSTEM_EFFECT_APPLY_TYPE_COUNT,

	SYSTEM_EFFECT_APPLY_TYPE_UNKNOWN = 0xFF,

	SYSTEM_EFFECT_APPLY_TYPE_FIRST = SYSTEM_EFFECT_APPLY_TYPE_VALUE,
	SYSTEM_EFFECT_APPLY_TYPE_LAST = SYSTEM_EFFECT_APPLY_TYPE_CURRENT_ENERGY_DEFENCE
};

#pragma pack(push, 4)
struct sSYSTEM_EFFECT_TBLDAT : public sTBLDAT
{
public:
//	std::wstring			wstrName;
	WCHAR					wszName[ DBO_MAX_LENGTH_SYSTEM_EFFECT_NAME + 1 ];
	BYTE					byEffect_Type;
	BYTE					byActive_Effect_Type;
	TBLIDX					Effect_Info_Text;
	TBLIDX					Keep_Effect_Name;
	BYTE					byTarget_Effect_Position;
//	std::string				strSuccess_Effect_Name;
	char					szSuccess_Effect_Name[ DBO_MAX_LENGTH_SYSTEM_EFFECT_SUCCESS_EFFECT_NAME + 1 ];
	BYTE					bySuccess_Projectile_Type;
	BYTE					bySuccess_Effect_Position;
//	std::string				strSuccess_End_Effect_Name;
	char					szSuccess_End_Effect_Name[ DBO_MAX_LENGTH_SYSTEM_EFFECT_SUCCESS_END_EFFECT_NAME + 1 ];
	BYTE					byEnd_Effect_Position;
	WORD					wKeep_Animation_Index;

	eSYSTEM_EFFECT_CODE		effectCode; // Effect Code Match ¿ë

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CSystemEffectTable : public CTable
{
public:

	CSystemEffectTable(void);
	virtual ~CSystemEffectTable(void);

	bool					Create(DWORD dwCodePage);

	void					Destroy();


protected:

	void					Init();

public:

	sTBLDAT *				FindData(TBLIDX tblidx);

	eSYSTEM_EFFECT_CODE		GetEffectCodeWithTblidx(TBLIDX tblidx);

	sSYSTEM_EFFECT_TBLDAT *	FindDataWithEffectCode(DWORD dwEffectCode);

	TBLIDX					GetEffectTblidx(DWORD dwEffectCode);


protected:

	WCHAR **				GetSheetListInWChar() { return &(CSystemEffectTable::m_pwszSheetList[0]); }

	void *					AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);

	bool					DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);

	bool					AddTable(void * pvTable, bool bReload);

	bool					SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:

	static WCHAR *			m_pwszSheetList[];

	TBLIDX					m_aEffectCodeMatchTable[ MAX_SYSTEM_EFFECT_CODE ];
};