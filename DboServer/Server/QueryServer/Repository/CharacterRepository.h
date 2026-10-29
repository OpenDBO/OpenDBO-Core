#pragma once

#include "NtlSingleton.h"
#include "DatabaseEnv.h"
#include "NtlCharacter.h"

class CPlayerCache;
class CGameServerSession;

class CCharacterRepository : public CNtlSingleton<CCharacterRepository>
{

public:

	CCharacterRepository() {}
	virtual ~CCharacterRepository() {}

public:

	smart_ptr<QueryResult>		GetMaxCharId();
	smart_ptr<QueryResult>		GetByName(WCHAR* wszName);
	smart_ptr<QueryResult>		GetCharIdByName(WCHAR* wszName);
	smart_ptr<QueryResult>		GetGuildIdByCharId(CHARACTERID charId);

	void						InsertCharacter(sPC_SUMMARY& sSum, ACCOUNTID accountId);

	void						DeleteCharacter(CHARACTERID charId);

	void						InsertBind(CHARACTERID charId, DWORD worldId, float locX, float locY, float locZ, float dirX, float dirY, float dirZ);
	void						DeleteBind(CHARACTERID charId);

	void						InsertPortal(CHARACTERID charId, DWORD pointId);
	void						DeletePortals(CHARACTERID charId);

	void						DeleteTitles(CHARACTERID charId);
	void						DeleteWarfog(CHARACTERID charId);
	void						DeleteRankBattle(CHARACTERID charId);

	void						UpsertSkyDungeonLimit(CHARACTERID charId, BYTE byEnterCount, DWORD dwEnterDay);
	void						DeleteSkyDungeonLimit(CHARACTERID charId);

	void						UpsertDwcLimit(CHARACTERID charId, BYTE byEnterCount, DWORD dwEnterDay);
	void						UpsertDwcScenario(CHARACTERID charId, TBLIDX dwcTblidx, BYTE byMissionIndex, BYTE byRewardGrade, WORD wClearCount);
	void						DeleteDwcData(CHARACTERID charId);

	void						UpdateMoney(CHARACTERID charId, DWORD money);
	void						UpdateSpPoint(DWORD sp, CHARACTERID charId);
	void						UpdateLevelExpSp(BYTE byLevel, DWORD dwExp, DWORD dwSp, CHARACTERID charId);
	void						SavePcData(DWORD dwExp, float posX, float posY, float posZ, float dirX, float dirZ, WORD worldId, TBLIDX worldTblidx, DWORD mapInfoIndex,
									DWORD tutorialHint, DWORD charLp, WORD wEP, WORD wRP, DWORD charAP, TBLIDX charTitle, TBLIDX mascotTblidx, BYTE byCurRPBall, char* ip, DWORD eAirState, DWORD addPlayTime, CHARACTERID charId);
	void						UpdateGuildNameForMembers(GUILDID guildId, WCHAR* wszGuildName);
	void						UpdateWaguPoint(CHARACTERID charId, DWORD waguPoint);
	void						UpdateBind(DWORD worldId, TBLIDX bindObjectTblidx, BYTE byBindType, float locX, float locY, float locZ, float dirX, float dirY, float dirZ, CHARACTERID charId);
	void						UpdateClass(BYTE byClass, CHARACTERID charId);
	void						UpdateGender(BYTE byGender, CHARACTERID charId);
	void						UpdateNetpy(DWORD points, CHARACTERID charId);
	void						UpdateTutorialFlag(int bTutorialFlag, CHARACTERID charId);
	void						UpdateMudosaPoint(DWORD mudosaPoints, CHARACTERID charId);
	void						UpsertRankBattle(CHARACTERID charId, DWORD win, DWORD draw, DWORD lose, WORD straightKOWin, WORD maxStraightKOWin, WORD maxStraightWin, WORD straightWin, float points);
	void						UpdateMoneyAndMudosa(DWORD money, DWORD mudosaPoint, CHARACTERID charId);
	void						UpdateSpPointAdd(DWORD addSP, CHARACTERID charId);
	void						UpdateMoneyAndHoipoi(DWORD money, BYTE byMixLevel, DWORD mixExp, CHARACTERID charId);
	void						SaveRunTimeData(DWORD dwExp, float locX, float locY, float locZ, float dirX, float dirY, float dirZ, WORD worldId, TBLIDX worldTblidx, DWORD addPlayTime, CHARACTERID charId);
	void						InsertTitle(CHARACTERID charId, TBLIDX titleTblidx);
	void						DeleteTitle(CHARACTERID charId, TBLIDX titleTblidx);
	void						UpdateAdult(BOOL bIsAdult, CHARACTERID charId);
	void						UpdateMoneyCharId64(DWORD money, CHARACTERID charId);
	smart_ptr<QueryResult>		GetAccountIdByCharName(WCHAR* wszName);
	void						UpdateInvisibleCostume(BOOL bInvisibleCostume, CHARACTERID charId);
	smart_ptr<QueryResult>		GetByNameTrailingSpace(WCHAR* wszName);
	bool						UpdateCharNameWait(WCHAR* wszName, CHARACTERID charId);

	smart_ptr<QueryResult>		GetCharIdAndAwayByName(char* nameUtf8);
	smart_ptr<QueryResult>		GetCharIdByNameUtf8(const char* nameUtf8);
	smart_ptr<QueryResult>		GetAccountIdByCharNameUtf8(const std::string& strNameUtf8);
	void						UpdateIsMailAway(BOOL bIsAway, CHARACTERID charId);
	void						InsertWarFog(CHARACTERID charId, DWORD contentsTblidx);

	void						CheckAccountForCharacterAsync(CGameServerSession* pSession, CHARACTERID charId, ACCOUNTID accountId);
	void						LoadCharacterCoreDataAsync(CPlayerCache* pPlayerCache, CHARACTERID charId);
	void						LoadCharacterExtendedDataAsync(CPlayerCache* pPlayerCache, CHARACTERID charId);

};

#define GetCharacterRepository()		CCharacterRepository::GetInstance()
#define g_pCharacterRepository			GetCharacterRepository()
