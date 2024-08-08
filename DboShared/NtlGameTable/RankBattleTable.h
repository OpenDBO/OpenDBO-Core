//***********************************************************************************
//
//	File		:	TimeQuestTable.h
//
//	Begin		:	2007-06-07
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Ju-hyung Lee ( niam@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once


#include "table.h"

#include "NtlRankBattle.h"


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack(push, 4)
struct sRANKBATTLE_TBLDAT : public sTBLDAT
{
public:

	sRANKBATTLE_TBLDAT( void ) { }

public:

	BYTE			byRuleType;				// 경기 룰 eGAMERULE_TYPE
	BYTE			byBattleMode;			// 전투 타입 0 = 개인, 1 = party
	WCHAR			wszName[ DBO_RANKBATTLE_NAME_LENGTH + 1 ];
	TBLIDX			worldTblidx;			// World_Table Index
	TBLIDX			needItemTblidx;			// 진입하기 위해 필요한 아이템
	DWORD			dwZenny;				// 진입하기 위해 필요한 제니
	BYTE			byMinLevel;				// 최소 레벨
	BYTE			byMaxLevel;				// 최대 레벨
	BYTE			byBattleCount;			// 전투 횟수

	DWORD			dwWaitTime;				// 진입 대기 시간
	DWORD			dwDirectionTime;		// 연출 시간
	DWORD			dwMatchReadyTime;		// 경기 준비 시간
	DWORD			dwStageReadyTime;		// 스테이지 준비 시간
	DWORD			dwStageRunTime;			// 스테이지 경기 시간
	DWORD			dwStageFinishTime;		// 스테이지 종료 시간
	DWORD			dwMatchFinishTime;		// 경기 종료 시간
	DWORD			dwBossDirectionTime;	// 난입자 연출
	DWORD			dwBossKillTime;			// 난입자 사냥 시간
	DWORD			dwBossEndingTime;		// 정리 시간
	DWORD			dwEndTime;				// 종료

	char			chScoreKO;				// KO승시 점수
	char			chScoreOutOfArea;		// 장외승시 점수
	char			chScorePointWin;		// 판정승시 점수
	char			chScoreDraw;			// 비겼을시 점수
	char			chScoreLose;			// 패배시 점수

	char			chResultExcellent;		// 경기 결과 출력,  >= chResultExcellent
	char			chResultGreate;			// 경기 결과 출력,  < chResultExcellent, >= chResultGreate
	char			chResultGood;			// 경기 결과 출력,  < chResultGreate, >= chResultDraw
	char			chResultDraw;			// 경기 결과 출력,  == chResultDraw
	char			chResultLose;			// 경기 결과 출력,  == chResultLost

	char			chBonusPerfectWinner;	// 완승으로 연승했을 경우 추가 점수
	char			chBonusNormalWinner;	// 일반적인 연승을 했을때의 추가 점수

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
class CRankBattleTable : public CTable
{
public:

	CRankBattleTable( void );

	virtual ~CRankBattleTable( void );

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
