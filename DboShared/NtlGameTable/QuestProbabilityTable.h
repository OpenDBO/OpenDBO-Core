#pragma once

#include "Table.h"


const int NTL_QUEST_PROBABILITY_MAX_NAME_LENGTH = 64;
const int NTL_QUEST_PROBABILITY_MAX_NOTE_LENGTH = 255;
const int NTL_QUEST_PROBABILITY_MAX_COUNT = 20;

enum eTEMP_REWARD_TYPE
{
	eTEMP_REWARD_TYPE_NORMAL_ITEM,
	eTEMP_REWARD_TYPE_QUEST_ITEM,
	eTEMP_REWARD_TYPE_EXP,
	eTEMP_REWARD_TYPE_SKILL,
	eTEMP_REWARD_TYPE_ZENY,
	eTEMP_REWARD_TYPE_CHANGE_CLASS,
	eTEMP_REWARD_TYPE_BUFF,
	eTEMP_REWARD_TYPE_PROBABILITY, //안쓴다, 원래 있었는데 안쪽에 계산이 되어서 다른걸로 바뀌기 때문에 없앴다.

	INVALID_TEMP_REWARD_TYPE = 0xff,
};

#pragma pack(push, 4)
struct sQUEST_PROBABILITY_DATA
{
	BYTE	byType;			// eREWARD_TYPE
	TBLIDX	tblidx;			// 테이블 인덱스 INVALID_
	DWORD	dwMinValue;		// 최소 값
	DWORD	dwMaxValue;		// 최대 값
	float	fRate;			// 확율
};
#pragma pack(pop)

#pragma pack(push, 4)
struct sQUEST_PROBABILITY_TBLDAT : public sTBLDAT
{
	sQUEST_PROBABILITY_TBLDAT()
	{
		memset( asProbabilityData, 0xFF, sizeof(sQUEST_PROBABILITY_DATA) * NTL_QUEST_PROBABILITY_MAX_COUNT );
	}

public:
	WCHAR					wszName[NTL_QUEST_PROBABILITY_MAX_NAME_LENGTH + 1];
	WCHAR					wszNote[NTL_QUEST_PROBABILITY_MAX_NOTE_LENGTH + 1];
	BYTE					byProbabilityType;	// ePROBABILITY_TYPE
	sQUEST_PROBABILITY_DATA	asProbabilityData[NTL_QUEST_PROBABILITY_MAX_COUNT];

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)

class CQuestProbabilityTable :
	public CTable
{
public:
	CQuestProbabilityTable(void);
	virtual ~CQuestProbabilityTable(void);


	bool Create(DWORD dwCodePage);
	void Destroy();

protected:
	void Init();

public:
	sTBLDAT *			FindData(TBLIDX tblidx); 

protected:
	WCHAR** GetSheetListInWChar() { return &(CQuestProbabilityTable::m_pwszSheetList[0]); }
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
