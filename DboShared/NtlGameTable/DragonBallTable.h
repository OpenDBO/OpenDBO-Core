#pragma once

#include "Table.h"
#include "NtlItem.h"
#include "NtlDragonBall.h"

#pragma pack(push, 4)
struct sDRAGONBALL_TBLDAT : public sTBLDAT
{
public:
	DWORD		dwAltarGroup;
	BYTE		byBallType;		
	TBLIDX		aBallTblidx[NTL_ITEM_MAX_DRAGONBALL];
	TBLIDX		ballDropTblidx;
	TBLIDX		ballJunkTblidx;
	TBLIDX		startDialog;
	TBLIDX      endDialog;
	TBLIDX      hurryDialog;
	TBLIDX		timeoverDialog;
	TBLIDX		noRepeatDialog;
	TBLIDX		dragonNPCTblidx;
	TBLIDX		defaultSummonChat;
	TBLIDX      inventoryFullDialog;
	sVECTOR2	fDir;


protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CDragonBallTable : public CTable
{
public:
	CDragonBallTable(void);
	virtual ~CDragonBallTable(void);

	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT*					FindData(TBLIDX tblidx);

	sTBLDAT *					GetDBTbldatByGroup(DWORD dwGroupID);
	bool						SetDBTbldatByGroup(DWORD dwGroupID, TBLIDX tblidx);

	sTBLDAT*					GetDBTbldat(BYTE byType);
	bool						SetDBTbldat(BYTE byType, sTBLDAT * pTbldat);
	bool						GetType(UINT uiSerial, OUT BYTE& byType, OUT BYTE& byNumber);						///< 드래곤볼이 어느 타입인지를 반환한다.
    eDRAGON_BALL_TYPE           GetDropItemType(UINT dropTblidx);                                                   ///< 드롭 아이템이 어떤 타입인지를 반환한다.


protected:
	WCHAR** GetSheetListInWChar() { return &(CDragonBallTable::m_pwszSheetList[0]); }
	void* AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool AddTable(void * pvTable, bool bReload);
	bool SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);

public:

	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload);

	virtual bool				SaveToBinary(CNtlSerializer& serializer);

protected:

	sTBLDAT *					m_aDBTbldat[SCHEDULE_EVENT_TYPE_COUNT];

private:

	typedef std::multimap<DWORD, TBLIDX> DRAGONBALLGROUP;
	typedef DRAGONBALLGROUP::iterator DRAGONBALLGROUP_IT;
	typedef DRAGONBALLGROUP::value_type DRAGONBALLGROUP_VAL;
	DRAGONBALLGROUP				m_dragonGroup;
	static						WCHAR* m_pwszSheetList[];
};