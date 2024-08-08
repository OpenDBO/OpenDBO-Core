//***********************************************************************************
//
//	File		:	CDboNavi.h
//
//	Begin		:	2007-07-27
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hong Ho Dong   ( battery@ntl-inc.com )
//
//	Desc		:	Path & Field(Zone) & Tile Attribute
//
//***********************************************************************************
#pragma once

#include "WorldTable.h"
#include "DboWeInfoGroup.h"
#include "GraphicDataTable.h"

// 프로그램에서 사용하는 하위 개념의 속성
enum eDBO_WORLD_ATTR_BASIC
{
	DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE = 0,
	DBO_WORLD_ATTR_BASIC_FORBID_VEHICLE,
	DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP,
	DBO_WORLD_ATTR_BASIC_FORBID_PC_MOVE,
	DBO_WORLD_ATTR_BASIC_FORBID_NPC_MOVE,
	DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE,
	DBO_WORLD_ATTR_BASIC_ENABLE_SWIM,

	DBO_WORLD_ATTR_BASIC_COUNT,

	DBO_WORLD_ATTR_BASIC_FIRST = DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE,
	DBO_WORLD_ATTR_BASIC_LAST = DBO_WORLD_ATTR_BASIC_ENABLE_SWIM,
};

// 지형 툴에서 사용하는 상위 개념의 속성(eDBO_WORLD_ATTR_BASIC의 그룹)
enum eDBO_WORLD_ATTR_GROUP
{
	DBO_WORLD_ATTR_GROUP_TOWN = 0,
	DBO_WORLD_ATTR_GROUP_SWIM,
	DBO_WORLD_ATTR_GROUP_FORBID_PC_BATTLE,
	DBO_WORLD_ATTR_GROUP_FORBID_PC_SHOP,
	DBO_WORLD_ATTR_GROUP_FORBID_ALL_MOVE,
	DBO_WORLD_ATTR_GROUP_FORBID_MOB_MOVE,
	DBO_WORLD_ATTR_GROUP_WORLD_SHADOW = 15,
};

#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_PC_BATTLE		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_PC_BATTLE)
#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_VEHICLE		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_VEHICLE)
#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_PC_SHOP		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_PC_SHOP)
#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_PC_MOVE		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_PC_MOVE)
#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_NPC_MOVE		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_NPC_MOVE)
#define DBO_WORLD_ATTR_BASIC_FLAG_FORBID_MOB_MOVE		(0x00000001 << DBO_WORLD_ATTR_BASIC_FORBID_MOB_MOVE)
#define DBO_WORLD_ATTR_BASIC_FLAG_ENABLE_SWIM			(0x00000001 << DBO_WORLD_ATTR_BASIC_ENABLE_SWIM)

#define DBO_WORLD_ATTR_GROUP_FLAG_TOWN					(0x00000001 << DBO_WORLD_ATTR_GROUP_TOWN)
#define DBO_WORLD_ATTR_GROUP_FLAG_SWIM					(0x00000001 << DBO_WORLD_ATTR_GROUP_SWIM)
#define DBO_WORLD_ATTR_GROUP_FLAG_FORBID_PC_BATTLE		(0x00000001 << DBO_WORLD_ATTR_GROUP_FORBID_PC_BATTLE)
#define DBO_WORLD_ATTR_GROUP_FLAG_FORBID_PC_SHOP		(0x00000001 << DBO_WORLD_ATTR_GROUP_FORBID_PC_SHOP)
#define DBO_WORLD_ATTR_GROUP_FLAG_FORBID_ALL_MOVE		(0x00000001 << DBO_WORLD_ATTR_GROUP_FORBID_ALL_MOVE)
#define DBO_WORLD_ATTR_GROUP_FLAG_FORBID_MOB_MOVE		(0x00000001 << DBO_WORLD_ATTR_GROUP_FORBID_MOB_MOVE)
#define DBO_WORLD_ATTR_GROUP_FLAG_WORLD_SHADOW			(0x00000001 << DBO_WORLD_ATTR_GROUP_WORLD_SHADOW)

class CDboNavi
{
public:
	static DWORD CREATE_INVALID;
	static DWORD CREATE_TILE_INFO;				// Tile Info Create
	static DWORD CREATE_FIELD_INFO;				// Field Info Create
	static DWORD CREATE_PATH_ENGINE_INFO;		// Path Engine Info Create

	struct SCreateParam
	{
		const char *pszWeTileInfoPath;			// TileInfo File Path
		const char *pszWeFieldInfoPath;			// FieldInfo File Path		
		const char *pszPathEnginePath;			// PathEngine File Path
		CWorldTable	*pWorldTable;				// Pointer WorldTable

		DWORD dwCreateInfoFlag;					// CreateFlag

		SCreateParam(): pszWeTileInfoPath(NULL), 
					    pszWeFieldInfoPath(NULL),
					    pszPathEnginePath(NULL),
						pWorldTable(NULL) { dwCreateInfoFlag = CREATE_INVALID; }
	};

public:
	CDboNavi();
	~CDboNavi();

protected:
	CDboWeInfoGroup	m_DboWeInfoGroup;

public:
	bool	Create(SCreateParam *pCreateParam);
	void	Destroy();

protected:
	void	Init();

public:
	DWORD	GetAttribute(unsigned int uiWorldIndex, float x, float z);
	bool	IsBasicAttributeSet(eDBO_WORLD_ATTR_BASIC eAttrBasic, unsigned int uiWorldIndex, float x, float z);
	
	TBLIDX	GetTextAllIndex(TBLIDX worldTblidx, float x, float z);
	ZONEID	GetZoneIndex(TBLIDX worldTblidx, float x, float z);

	// 아직 구현 안 되었음
	bool	CanSee(unsigned int uiWorldIndex, float curX, float curY, float curZ, float desX, float desY, float desZ);
	

protected:
	void	RegisterTerrainAttributeGroup(eDBO_WORLD_ATTR_GROUP eAttrGroup, eDBO_WORLD_ATTR_BASIC eAttrBasic);

private:
	DWORD	m_adwBasicAttributeCheckFlag[DBO_WORLD_ATTR_BASIC_COUNT];
};