//***********************************************************************************
//
//	File		:	TimeQuestTable.h
//
//	Begin		:	2007-06-01
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "table.h"

#include "NtlTimeQuest.h"

#include "NtlVector.h"
#include "NtlSharedDef.h"

const DWORD		DBO_MAX_LENGTH_PROLOGUE_DIRECTION = 32;
const DWORD		DBO_MAX_LENGTH_BGM_STRING = 32;
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack(push, 4)
struct sTIMEQUEST_DATASET
{
	TBLIDX			nameTblidx; // 퀘스트 제목 텍스트 인덱스
	TBLIDX			questStringTblidx; // 퀘스트 설명 텍스트 인덱스
	TBLIDX			worldTblidx; // 월드 인덱스
	TBLIDX			scriptTblidx; // 스크립트 인덱스

	BYTE			byMinMemberCount; // 최소 인원
	BYTE			byMaxMemberCount; // 최대 인원
	BYTE			byMinMemberLevel; // 최소 레벨
	BYTE			byMaxMemberLevel; // 최대 레벨

	DWORD			dwLimitTime; // 제한시간
	DWORD			dwNeedZenny; // 입장료
	TBLIDX			needItemTblidx; // 필요 아이템 인덱스
	BYTE			byMaxKillCount; // 최대 킬 카운트

	BYTE			byWorldCount;		// world count

	TBLIDX			dayRecordRewardTblidx;
	TBLIDX			bestRecordRewardTblidx;
	
	
};
#pragma pack(pop)

#pragma pack(push, 4)
struct sTIMEQUEST_TBLDAT : public sTBLDAT
{
public:

	sTIMEQUEST_TBLDAT( void ) { }

public:

	BYTE			byTimeQuestType; // 타임퀘스트 타입 ( eTIMEQUEST_TYPE )
	BYTE			byDifficultyFlag; // 난이도 플래그( eTIMEQUEST_DIFFICULTY_FLAG )
	DWORD			dwStartTime; // 시작시간 ( 하루 00:00 을 기준으로 시작되는 분단위를 넣는다 )
	TBLIDX			startCharacterDirection;
	TBLIDX			startObjectIndex;
	TBLIDX			startTriggerId;
	TBLIDX			arriveCharacterDirection;
	TBLIDX			arriveObjectIndex;
	TBLIDX			arriveTriggerId;
	TBLIDX			leaveCharacterDirection;	
	TBLIDX			leaveObjectIndex;	
	TBLIDX			leaveTriggerId;
	TBLIDX			dayRecordMailTblidx;
	TBLIDX			bestRecordMailTblidx;
	BYTE			byResetTime;				// 리셋 시키는 시간 0~23 사용
	WCHAR			wszPrologueDirection[ DBO_MAX_LENGTH_PROLOGUE_DIRECTION + 1 ];
	sTIMEQUEST_DATASET sTimeQuestDataset[ MAX_TIMEQUEST_DIFFICULTY ];
	TBLIDX			openCine;
	TBLIDX			Note;
	WCHAR			wszStageBgm1[ DBO_MAX_LENGTH_BGM_STRING + 1 ];
	WCHAR			wszStageBgm2[ DBO_MAX_LENGTH_BGM_STRING + 1 ];
	WCHAR			wszLastBgm[ DBO_MAX_LENGTH_BGM_STRING + 1 ];
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
class CTimeQuestTable : public CTable
{
public:

	CTimeQuestTable( void );

	virtual ~CTimeQuestTable( void );

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