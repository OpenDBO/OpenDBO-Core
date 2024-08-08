//***********************************************************************************
//
//	File		:	VehicleTable.h
//
//	Begin		:	2008-11-04
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Jace Jung ( sooshia@ntl-inc.com )
//
//***********************************************************************************

#pragma once

#include "Table.h"

const DWORD		DBO_MAX_LENGTH_VEHICLE_MODEL_NAME = 32;

#pragma pack(push, 4)
struct sVEHICLE_TBLDAT : public sTBLDAT
{
	sVEHICLE_TBLDAT()
	{
	};

public:
	char			szModelName[ DBO_MAX_LENGTH_VEHICLE_MODEL_NAME + 1 ];
	BYTE			bySRPType;
	BYTE			bySpeed;
	BYTE			byVehicleType;
	WORD			wRunHeight;
	BYTE			byPersonnel;
	//BYTE			byFuelEfficiency;
	//DWORD			dwName;

public:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CVehicleTable : public CTable
{
public:
	CVehicleTable(void);
	virtual	~CVehicleTable(void);

	bool			Create(DWORD dwCodePage);
	void			Destroy();

protected:
	void			Init();

public:
	sTBLDAT*		FindData(TBLIDX tblidx); 

protected:
	WCHAR**			GetSheetListInWChar() { return &(CVehicleTable::m_pwszSheetList[0]); }
	void*			AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool			DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool			AddTable(void * pvTable, bool bReload);
	bool			SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

public:
	virtual bool	LoadFromBinary(CNtlSerializer& serializer, bool bReload);
	virtual bool	SaveToBinary(CNtlSerializer& serializer);

private:
	static WCHAR*	m_pwszSheetList[];
};