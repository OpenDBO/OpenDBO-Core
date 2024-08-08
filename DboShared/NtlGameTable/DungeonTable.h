//***********************************************************************************
//
//	File		:	DungeonTable.h
//
//	Begin		:	2008-07-30
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		: Chung, DooSup ( john@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


#include "table.h"

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack(push, 4)
struct sDUNGEON_TBLDAT : public sTBLDAT
{
public:

	sDUNGEON_TBLDAT( void ) { }

public:

	BYTE			byDungeonType;			// 1:normal 2:hard
	BYTE			byMaxMember;
	TBLIDX			linkWorld;
	BYTE			byMinLevel;
	BYTE			byMaxLevel;
	TBLIDX			needItemTblidx;
	DWORD			dwHonorPoint;
	TBLIDX			wpsTblidx;
	TBLIDX			openCine;

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class CDungeonTable : public CTable
{
public:

	CDungeonTable( void );

	virtual ~CDungeonTable( void );

public:

	virtual bool			LoadFromBinary(CNtlSerializer& serializer, bool bReload);

	virtual bool			SaveToBinary(CNtlSerializer& serializer);

public:

	bool					Create( DWORD dwCodePage );

	void					Destroy( void );

public:

	sTBLDAT*				FindData(TBLIDX tblidx); 

protected:

	void					Init( void );

	WCHAR**					GetSheetListInWChar( void ) { return &m_pwszSheetList[0]; }

	void*					AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage );

	bool					DeallocNewTable( void* pvTable, WCHAR* pwszSheetName );

	bool					AddTable( void* pvTable, bool bReload );

	bool					SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData );


protected:

	static WCHAR*			m_pwszSheetList[];

};
