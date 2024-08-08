//***********************************************************************************
//
//	File		:	BudokaiTable.h
//
//	Begin		:	2008-04-20
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Lee Ju-hyung
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "table.h"

#include "NtlVector.h"
#include "NtlSharedDef.h"
#include "NtlBudokai.h"
#include "NtlMatch.h"


#define BUDOKAI_MAX_TBLDAT_VALUE_COUNT	10

#define BUDOKAI_MAX_TBLDAT_FILE_LENGTH	64

#define BUDOKAI_MAX_AWARDING_LOCATION_COUNT 20

#define BUDOKAI_MAX_MAJOR_LOCATION_COUNT (MAX_MATCH_TEAM_TYPE_COUNT * NTL_MAX_MEMBER_IN_PARTY)

#define BUDOKAI_MAX_FINAL_LOCATION_COUNT (MAX_MATCH_TEAM_TYPE_COUNT * NTL_MAX_MEMBER_IN_PARTY)

const BYTE BUDOKAI_NEW_FILE_COUNT = 5;

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
#pragma pack(push, 4)


struct sBUDOKAI_WORLDTBLIDX
{
	TBLIDX	minorMatch;
	TBLIDX	majorMatch;
	TBLIDX	finalMatch;
};



// 소식지 파일(client용)
struct sBUDOKAI_NOTICE_FILE
{
	WCHAR	wszOpenNotice[BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
	WCHAR	wszDojoRecommend[BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
	WCHAR	wszRequest[BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
	WCHAR	awszNews[BUDOKAI_NEW_FILE_COUNT][BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
	WCHAR	wszAward[BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
};



struct sBUDOKAI_LOC
{
	CNtlVector	vLoc;
	CNtlVector	vDir;
};


// Minor Match
struct sMINORMATCH_REWARD
{
	DWORD	dwBaseMudosaPoint;			// 기본 보상
	DWORD	dwWinnerMudosaPoint;		// 승자 추가 보상
	DWORD	dwKillCountPoint;			// 킬 카운트 추가 보상
	TBLIDX	winnerItem;					// 승자 아이템
	BYTE	byWinerItemStackCount;		// 승자 아이템 스택카운트
	TBLIDX	loserItem;					// 패자 아이템
	BYTE	byLoserItemStackCount;		// 패자 아이엩 스택카운트
};


// Major Match, Final Match
struct sMATCH_REWARD
{
	DWORD	dwBaseMudosaPoint;		// 기본 보상
	DWORD	dwWinnerMudosaPoint;	// 승자 추가 보상
	TBLIDX	winnerItem;				// 승자 아이템
	BYTE	byWinerItemStackCount;	// 승자 아이템 스택카운트
	TBLIDX	loserItem;				// 패자 아이템
	BYTE	byLoserItemStackCount;	// 패자 아이엩 스택카운트
};


// Awarding
struct sAWARDING_REWARD
{
	struct sREWARD
	{
		DWORD	dwMudosaPoint;
		TBLIDX	itemTblidx;
		BYTE	byStackCount;
	};

	sREWARD	sWinner;			// 우승자
	sREWARD	sSemiWinner;		// 준우승자
	sREWARD	sOther;				// 3,4위
};


//
struct sMATCH_INDIVIDUAL_REWARD
{
	sMINORMATCH_REWARD	sMinorMatch;
	sMATCH_REWARD		aMajorMatch[MAX_BUDOKAI_MATCH_DEPTH_INDIVIDUAL + 1];
	sAWARDING_REWARD	sAwarding;
};


//
struct sMATCH_TEAM_REWARD
{
	sMINORMATCH_REWARD	sMinorMatch;
	sMATCH_REWARD		aMajorMatch[MAX_BUDOKAI_MATCH_DEPTH_TEAM + 1];
	sAWARDING_REWARD	sAwarding;
};


struct sBUDOKAI_TBLINFO
{
	BYTE	byJiniorOpenTerm;			// 주기(week 단위)
	BYTE	byJiniorOpenDayOfWeek;		// 요일
	BYTE	byJiniorOpenHour;			// 시간
	BYTE	byJiniorOpenMinute;			// 분

	BYTE	byOpenTerm;				// 주기(week 단위)
	BYTE	byOpenDayOfWeek;		// 요일
	BYTE	byOpenHour;				// 시간
	BYTE	byOpenMinute;			// 분

	BYTE	byJuniorLevelMin;
	BYTE	byJuniorLevelMax;
	BYTE	byAdultLevelMin;
	BYTE	byAdultLevelMax;

	DWORD	dwMatchIntervalTime;	// 각 경기 사이의 대기 시간
	DWORD	dwBudokaiRestartDelayTime;	// 천하제일무도회 재시작(서버시작으로 인한)시 기본 대기 시간

	// 진행 관련 대기 시간들(초단위)
	DWORD	dwOpenNoticeTime;		// 시작 예고
	DWORD	dwDojoRecommendTime;	// 도장 추천
	DWORD	dwRegisterTime;
	DWORD	dwMinorMatch_WaitTime;
	DWORD	dwMajorMatch_WaitTime;
	DWORD	dwFinalMatch_WaitTime;
	DWORD	dwBudokaiEndTime;

	// 알람시간
	DWORD	dwMinorMatch_AlramTime;
	DWORD	dwMajorMatch_AlramTime;
	DWORD	dwFinalMatch_AlramTime;

	// WorldMap Index
	sBUDOKAI_WORLDTBLIDX	sIndividualWorldTblidx;
	sBUDOKAI_WORLDTBLIDX	sTeamWorldTblidx;

	// 결승전 관련 시간
	DWORD					dwFinalMatchDirectionTime;
	DWORD					dwAwardingTime;
	DWORD					dwEndingWaitTime;

	// 소식지 파일 이름
	WCHAR					wszRegisterFile[BUDOKAI_MAX_TBLDAT_FILE_LENGTH+1];
	sBUDOKAI_NOTICE_FILE	sNoticeFile;

	// location
	sBUDOKAI_LOC			aAwardingLoc[BUDOKAI_MAX_AWARDING_LOCATION_COUNT];

	// 보상
	sMATCH_INDIVIDUAL_REWARD	sJuniorIndividualReward;
	sMATCH_TEAM_REWARD			sJuniorTeamReward;
	sMATCH_INDIVIDUAL_REWARD	sIndividualReward;
	sMATCH_TEAM_REWARD			sTeamReward;

	// MajorMatch Location
	sBUDOKAI_LOC			aMajorLoc[BUDOKAI_MAX_MAJOR_LOCATION_COUNT];

	// MajorMatch Location
	sBUDOKAI_LOC			aFinalLoc[BUDOKAI_MAX_FINAL_LOCATION_COUNT];
};


struct sBUDOKAI_TBLDAT : public sTBLDAT
{
public:

	sBUDOKAI_TBLDAT( void ) { }

public:
	std::wstring		wstrName;
	std::wstring		wstrValue[BUDOKAI_MAX_TBLDAT_VALUE_COUNT];
//	std::wstring		wstrNote;	// 메모리에 로드하지 않음

protected:

	virtual int GetDataSize()
	{
		return sizeof(*this) - sizeof(void*);
	}
};
#pragma pack(pop)


class CBudokaiTable : public CTable
{
public:
	CBudokaiTable(void);
	virtual ~CBudokaiTable(void);

public:

	virtual bool			LoadFromBinary(CNtlSerializer& serializer, bool bReload);

	virtual bool			SaveToBinary(CNtlSerializer& serializer);

public:

	bool					Create( DWORD dwCodePage );

	void					Destroy( void );

public:

	sTBLDAT*				FindData(TBLIDX tblidx);

	sBUDOKAI_TBLINFO *		GetBudokaiTblInfo() { return &m_sBudokaiTblInfo;}

protected:

	void					Init( void );

	WCHAR**					GetSheetListInWChar( void ) { return &m_pwszSheetList[0]; }

	void*					AllocNewTable( WCHAR* pwszSheetName, DWORD dwCodePage );

	bool					DeallocNewTable( void* pvTable, WCHAR* pwszSheetName );

	bool					AddTable( void* pvTable, bool bReload );

	bool					SetTableData( void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData );

	bool					ReadByte( BYTE & rDest, std::wstring & wstrValue, BYTE byMaxValue);

	bool					ReadDWORD( DWORD & rDest, std::wstring & wstrValue, DWORD dwMaxValue);

	bool					ReadTBLIDX( TBLIDX & rDest, std::wstring & wstrValue, TBLIDX dwMaxTblidx);

	bool					ReadFLOAT(float & rDest, std::wstring & wstrValue, float fMaxValue);

	bool					ReadSTR( WCHAR * pDest, DWORD dwDestLength, std::wstring & wstrSrc);

	bool					GetBinaryText(std::wstring & wstrValue, CNtlSerializer& serializer);

protected:

	static WCHAR*			m_pwszSheetList[];

	sBUDOKAI_TBLINFO		m_sBudokaiTblInfo;
};
