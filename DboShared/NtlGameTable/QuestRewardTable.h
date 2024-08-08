#pragma once

#include "Table.h"


const int QUEST_REWARD_DEF_MAX_CNT = 4;			/* DboTSCoreDefine.h => MAX_DEFAULT_REWARD */
const int QUEST_REWARD_SEL_MAX_CNT = 4;			/* DboTSCoreDefine.h => MAX_SELECTION_REWARD */


#pragma pack(push, 4)
struct sQUEST_REWARD_DATASET
{
	BYTE						byRewardType;
	DWORD						dwRewardIdx;
	DWORD						dwRewardVal;
};

struct sQUEST_REWARD_TBLDAT : public sTBLDAT
{
	sQUEST_REWARD_TBLDAT()
	{
		memset( arsDefRwd, 0xff, sizeof( arsDefRwd ) );
		memset( arsSelRwd, 0xff, sizeof( arsSelRwd ) );
	}

public:
	sQUEST_REWARD_DATASET		arsDefRwd[QUEST_REWARD_DEF_MAX_CNT];
	sQUEST_REWARD_DATASET		arsSelRwd[QUEST_REWARD_SEL_MAX_CNT];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)



class CQuestRewardTable : public CTable
{
public:
	CQuestRewardTable(void);
	virtual ~CQuestRewardTable(void);

public:
	bool 						Create(DWORD dwCodePage);
	void 						Destroy();

protected:
	void 						Init();

public:
	sTBLDAT *					FindData(TBLIDX tblidx); 
	

protected:
	WCHAR**						GetSheetListInWChar() { return &(CQuestRewardTable::m_pwszSheetList[0]); }
	void*						AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage);
	bool 						DeallocNewTable(void* pvTable, WCHAR* pwszSheetName);
	bool 						AddTable(void * pvTable, bool bReload);
	bool 						SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData);


public:
	virtual bool				LoadFromBinary(CNtlSerializer& serializer, bool bReload);
	virtual bool				SaveToBinary(CNtlSerializer& serializer);


private:
	static WCHAR* m_pwszSheetList[];
};

