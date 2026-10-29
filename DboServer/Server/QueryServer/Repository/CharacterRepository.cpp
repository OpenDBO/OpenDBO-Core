#include "stdafx.h"
#include "CharacterRepository.h"
#include "../QueryServer.h"
#include "../PlayerCache.h"
#include "../GameServerSession.h"


smart_ptr<QueryResult> CCharacterRepository::GetMaxCharId()
{
	return GetCharDB.Query("SELECT MAX(id) FROM characters");
}

smart_ptr<QueryResult> CCharacterRepository::GetByName(WCHAR* wszName)
{
	return GetCharDB.Query("SELECT id FROM characters WHERE char_name='%ls'", wszName);
}

smart_ptr<QueryResult> CCharacterRepository::GetCharIdByName(WCHAR* wszName)
{
	return GetCharDB.Query("SELECT id FROM characters WHERE char_name=\"%ls\"", wszName);
}

smart_ptr<QueryResult> CCharacterRepository::GetGuildIdByCharId(CHARACTERID charId)
{
	return GetCharDB.Query("SELECT guild_id FROM characters WHERE id=%u", charId);
}

void CCharacterRepository::DeleteCharacter(CHARACTERID charId)
{
	GetCharDB.Execute("DELETE FROM characters WHERE id=%u", charId);
}

void CCharacterRepository::InsertBind(CHARACTERID charId, DWORD worldId, float locX, float locY, float locZ, float dirX, float dirY, float dirZ)
{
	GetCharDB.Execute("INSERT INTO bind (char_id,world_id,loc_x,loc_y,loc_z,dir_x,dir_y,dir_z) VALUES (%u,%u,%f,%f,%f,%f,%f,%f)",
		charId, worldId, locX, locY, locZ, dirX, dirY, dirZ);
}

void CCharacterRepository::DeleteBind(CHARACTERID charId)
{
	GetCharDB.Execute("DELETE FROM bind WHERE char_id=%u", charId);
}

void CCharacterRepository::InsertPortal(CHARACTERID charId, DWORD pointId)
{
	GetCharDB.Execute("INSERT INTO portals (char_id,point) VALUES (%u,%u)", charId, pointId);
}

void CCharacterRepository::DeletePortals(CHARACTERID charId)
{
	GetCharDB.Execute("DELETE FROM portals WHERE char_id=%u", charId);
}

void CCharacterRepository::DeleteTitles(CHARACTERID charId)
{
	GetCharDB.Execute("DELETE FROM titles WHERE char_id=%u", charId);
}

void CCharacterRepository::DeleteWarfog(CHARACTERID charId)
{
	GetCharDB.Execute("DELETE FROM warfog WHERE char_id=%u", charId);
}

void CCharacterRepository::DeleteRankBattle(CHARACTERID charId)
{
	GetCharDB.Execute("DELETE FROM rank_battle WHERE char_id=%u", charId);
}

void CCharacterRepository::UpsertSkyDungeonLimit(CHARACTERID charId, BYTE byEnterCount, DWORD dwEnterDay)
{
	GetCharDB.Execute("INSERT INTO sky_dungeon_limit (char_id,enter_count,enter_day) VALUES (%u,%u,%u) ON DUPLICATE KEY UPDATE enter_count=VALUES(enter_count),enter_day=VALUES(enter_day)", charId, byEnterCount, dwEnterDay);
}

void CCharacterRepository::DeleteSkyDungeonLimit(CHARACTERID charId)
{
	GetCharDB.Execute("DELETE FROM sky_dungeon_limit WHERE char_id=%u", charId);
}

void CCharacterRepository::UpsertDwcLimit(CHARACTERID charId, BYTE byEnterCount, DWORD dwEnterDay)
{
	GetCharDB.Execute("INSERT INTO dwc_limit (char_id,enter_count,enter_day) VALUES (%u,%u,%u) ON DUPLICATE KEY UPDATE enter_count=VALUES(enter_count),enter_day=VALUES(enter_day)", charId, byEnterCount, dwEnterDay);
}

void CCharacterRepository::UpsertDwcScenario(CHARACTERID charId, TBLIDX dwcTblidx, BYTE byMissionIndex, BYTE byRewardGrade, WORD wClearCount)
{
	GetCharDB.Execute("INSERT INTO dwc_scenario (char_id,dwc_tblidx,mission_index,reward_grade,clear_count) VALUES (%u,%u,%u,%u,%u) ON DUPLICATE KEY UPDATE reward_grade=VALUES(reward_grade),clear_count=VALUES(clear_count)", charId, dwcTblidx, byMissionIndex, byRewardGrade, wClearCount);
}

void CCharacterRepository::DeleteDwcData(CHARACTERID charId)
{
	GetCharDB.Execute("DELETE FROM dwc_limit WHERE char_id=%u", charId);
	GetCharDB.Execute("DELETE FROM dwc_scenario WHERE char_id=%u", charId);
}

void CCharacterRepository::UpdateMoney(CHARACTERID charId, DWORD money)
{
	GetCharDB.Execute("UPDATE characters SET money=%u WHERE id=%u", money, charId);
}

void CCharacterRepository::UpdateGuildNameForMembers(GUILDID guildId, WCHAR* wszGuildName)
{
	GetCharDB.Execute("UPDATE characters SET guild_name=\"%ls\" WHERE guild_id=%u", wszGuildName, guildId);
}

void CCharacterRepository::UpdateWaguPoint(CHARACTERID charId, DWORD waguPoint)
{
	GetCharDB.Execute("UPDATE characters SET wagu_point=%u WHERE id=%u", waguPoint, charId);
}

void CCharacterRepository::UpdateSpPoint(DWORD sp, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET sp_point=%u WHERE id=%u", sp, charId);
}

void CCharacterRepository::UpdateLevelExpSp(BYTE byLevel, DWORD dwExp, DWORD dwSp, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET level=%u,exp=%u,sp_point=%u WHERE id=%u", byLevel, dwExp, dwSp, charId);
}

void CCharacterRepository::SavePcData(DWORD dwExp, float posX, float posY, float posZ, float dirX, float dirZ, WORD worldId, TBLIDX worldTblidx, DWORD mapInfoIndex,
	DWORD tutorialHint, DWORD charLp, WORD wEP, WORD wRP, DWORD charAP, TBLIDX charTitle, TBLIDX mascotTblidx, BYTE byCurRPBall, char* ip, DWORD eAirState, DWORD addPlayTime, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET exp=%u,cur_loc_x=%f,cur_loc_y=%f,cur_loc_z=%f,cur_dir_x=%f,cur_dir_z=%f,world_id=%u,world_table=%u,map_info_index=%u,tutorial_hint=%u,cur_lp=%u,cur_ep=%u,cur_rp=%u,cur_ap=%u,title=%u,mascot=%u,rp_ball=%u,ip=\"%s\",air_state=%u,play_time=play_time+%u WHERE id=%u",
		dwExp, posX, posY, posZ, dirX, dirZ, worldId, worldTblidx, mapInfoIndex,
		tutorialHint, charLp, wEP, wRP, charAP,
		charTitle, mascotTblidx, byCurRPBall, ip, eAirState, addPlayTime,
		charId);
}

void CCharacterRepository::UpdateBind(DWORD worldId, TBLIDX bindObjectTblidx, BYTE byBindType, float locX, float locY, float locZ, float dirX, float dirY, float dirZ, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE bind SET world_id=%u,bind_object_tblidx=%u,loc_x=%f,loc_y=%f,loc_z=%f,dir_x=%f,dir_y=%f,dir_z=%f,type=%u WHERE char_id=%u",
		worldId, bindObjectTblidx, locX, locY, locZ, dirX, dirY, dirZ, byBindType, charId);
}

void CCharacterRepository::UpdateClass(BYTE byClass, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET class=%u WHERE id=%u", byClass, charId);
}

void CCharacterRepository::UpdateGender(BYTE byGender, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET gender=%u WHERE id=%u", byGender, charId);
}

void CCharacterRepository::UpdateNetpy(DWORD points, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET netpy=%u WHERE id=%u", points, charId);
}

void CCharacterRepository::UpdateTutorialFlag(int bTutorialFlag, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET tutorial_flag=%i WHERE id=%u", bTutorialFlag, charId);
}

void CCharacterRepository::UpdateMudosaPoint(DWORD mudosaPoints, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET mudosa_point=%u WHERE id=%u", mudosaPoints, charId);
}

void CCharacterRepository::UpsertRankBattle(CHARACTERID charId, DWORD win, DWORD draw, DWORD lose, WORD straightKOWin, WORD maxStraightKOWin, WORD maxStraightWin, WORD straightWin, float points)
{
	GetCharDB.Execute("INSERT INTO rank_battle (char_id,win,draw,lose,straight_ko_win,max_straight_ko_win,max_straight_win,straight_win,points) VALUES (%u,%u,%u,%u,%u,%u,%u,%u,%f) ON DUPLICATE KEY UPDATE "
		"win=VALUES(win),draw=VALUES(draw),lose=VALUES(lose),straight_ko_win=VALUES(straight_ko_win),max_straight_ko_win=VALUES(max_straight_ko_win),max_straight_win=VALUES(max_straight_win),straight_win=VALUES(straight_win),points=VALUES(points)",
		charId, win, draw, lose, straightKOWin, maxStraightKOWin, maxStraightWin, straightWin, points);
}

smart_ptr<QueryResult> CCharacterRepository::GetCharIdAndAwayByName(char* nameUtf8)
{
	return GetCharDB.Query("SELECT id, mail_is_away FROM characters WHERE char_name=\"%s\"", GetCharDB.EscapeString(nameUtf8).c_str());
}

smart_ptr<QueryResult> CCharacterRepository::GetCharIdByNameUtf8(const char* nameUtf8)
{
	return GetCharDB.Query("SELECT id FROM characters WHERE char_name=\"%s\" LIMIT 1", nameUtf8);
}

smart_ptr<QueryResult> CCharacterRepository::GetAccountIdByCharNameUtf8(const std::string& strNameUtf8)
{
	return GetCharDB.Query("SELECT account_id FROM characters WHERE char_name=\"%s\" LIMIT 1", GetCharDB.EscapeString(strNameUtf8).c_str());
}

void CCharacterRepository::UpdateIsMailAway(BOOL bIsAway, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET mail_is_away=%i WHERE id=%u", bIsAway, charId);
}

void CCharacterRepository::InsertWarFog(CHARACTERID charId, DWORD contentsTblidx)
{
	GetCharDB.Execute("INSERT INTO warfog VALUES (%u,%u)", charId, contentsTblidx);
}

void CCharacterRepository::UpdateMoneyAndMudosa(DWORD money, DWORD mudosaPoint, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET money=%u,mudosa_point=%u WHERE id=%u", money, mudosaPoint, charId);
}

void CCharacterRepository::UpdateSpPointAdd(DWORD addSP, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET sp_point=sp_point+%u WHERE id=%u", addSP, charId);
}

void CCharacterRepository::UpdateMoneyAndHoipoi(DWORD money, BYTE byMixLevel, DWORD mixExp, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET money=%u, hoipoi_mix_level=%u, hoipoi_mix_exp=%u WHERE id=%u", money, byMixLevel, mixExp, charId);
}

void CCharacterRepository::SaveRunTimeData(DWORD dwExp, float locX, float locY, float locZ, float dirX, float dirY, float dirZ, WORD worldId, TBLIDX worldTblidx, DWORD addPlayTime, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET exp=%u, cur_loc_x=%f,cur_loc_y=%f,cur_loc_z=%f, cur_dir_x=%f,cur_dir_y=%f,cur_dir_z=%f, world_id=%u,world_table=%u, play_time=play_time+%u WHERE id=%u",
		dwExp, locX, locY, locZ, dirX, dirY, dirZ, worldId, worldTblidx, addPlayTime, charId);
}

void CCharacterRepository::InsertTitle(CHARACTERID charId, TBLIDX titleTblidx)
{
	GetCharDB.Execute("INSERT INTO titles (char_id,title_tblidx) VALUES (%u,%u)", charId, titleTblidx);
}

void CCharacterRepository::DeleteTitle(CHARACTERID charId, TBLIDX titleTblidx)
{
	GetCharDB.Execute("DELETE FROM titles WHERE char_id=%u AND title_tblidx=%u", charId, titleTblidx);
}

void CCharacterRepository::UpdateAdult(BOOL bIsAdult, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET adult=%i WHERE id=%u", bIsAdult, charId);
}

void CCharacterRepository::UpdateMoneyCharId64(DWORD money, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET money=%u WHERE id=%I64u", money, charId);
}

smart_ptr<QueryResult> CCharacterRepository::GetAccountIdByCharName(WCHAR* wszName)
{
	return GetCharDB.Query("SELECT account_id FROM characters WHERE char_name=\"%ls\"", wszName);
}

void CCharacterRepository::UpdateInvisibleCostume(BOOL bInvisibleCostume, CHARACTERID charId)
{
	GetCharDB.Execute("UPDATE characters SET invisible_costume=%i WHERE id=%u", bInvisibleCostume, charId);
}

smart_ptr<QueryResult> CCharacterRepository::GetByNameTrailingSpace(WCHAR* wszName)
{
	return GetCharDB.Query("SELECT id FROM characters WHERE char_name=\"%ls\" ", wszName);
}

bool CCharacterRepository::UpdateCharNameWait(WCHAR* wszName, CHARACTERID charId)
{
	return GetCharDB.WaitExecute("UPDATE characters SET char_name=\"%ls\" WHERE id=%u", wszName, charId); //required waitexecute so none can get the same name
}

void CCharacterRepository::CheckAccountForCharacterAsync(CGameServerSession* pSession, CHARACTERID charId, ACCOUNTID accountId)
{
	SQLCallbackBase* pCallBack3 = new SQLClassCallbackP2<CGameServerSession, CHARACTERID, ACCOUNTID>(pSession, &CGameServerSession::OnLoadPcCheck, charId, accountId);
	AsyncQuery * q3 = new AsyncQuery(pCallBack3);
	q3->AddQuery("SELECT account_id FROM characters WHERE id=%u", charId);
	GetCharDB.QueueAsyncQuery(q3);
}

void CCharacterRepository::LoadCharacterCoreDataAsync(CPlayerCache* pPlayerCache, CHARACTERID charId)
{
	SQLCallbackBase* pCallBack = new SQLClassCallbackP0<CPlayerCache>(pPlayerCache, &CPlayerCache::OnLoadPcData);
	AsyncQuery * q = new AsyncQuery(pCallBack);
	q->AddQuery("SELECT * FROM characters WHERE id=%u", charId);
	q->AddQuery("SELECT * FROM bind WHERE char_id=%u", charId);
	q->AddQuery("SELECT id, sender_type, is_read FROM mail WHERE char_id=%u", charId); //NO NEED LIMIT HERE. IT WILL RELOAD ALL
	q->AddQuery("SELECT * FROM rank_battle WHERE char_id=%u", charId);
	q->AddQuery("SELECT title_tblidx FROM titles WHERE char_id=%u LIMIT 64", charId); // LIMIT NTL_MAX_CHAR_TITLE_COUNT_IN_FLAG
	q->AddQuery("SELECT war_fog FROM warfog WHERE char_id=%u", charId);
	q->AddQuery("SELECT enter_count, enter_day FROM sky_dungeon_limit WHERE char_id=%u", charId);
	q->AddQuery("SELECT enter_count, enter_day FROM dwc_limit WHERE char_id=%u", charId);
	q->AddQuery("SELECT dwc_tblidx, mission_index, reward_grade, clear_count FROM dwc_scenario WHERE char_id=%u", charId);
	GetCharDB.QueueAsyncQuery(q);
}

void CCharacterRepository::LoadCharacterExtendedDataAsync(CPlayerCache* pPlayerCache, CHARACTERID charId)
{
	SQLCallbackBase* pCallBack2 = new SQLClassCallbackP0<CPlayerCache>(pPlayerCache, &CPlayerCache::OnLoadPcData2);
	AsyncQuery * q2 = new AsyncQuery(pCallBack2);
	q2->AddQuery("SELECT * FROM items WHERE char_id=%u AND place < 7 LIMIT 166", charId); // NTL_MAX_BAGSLOT_COUNT + NTL_MAX_ITEM_SLOT + (NTL_MAX_BAG_ITEM_SLOT * 4) + EQUIP_SLOT_TYPE_COUNT | 7 = CONTAINER_TYPE_BANKSLOT
	q2->AddQuery("SELECT * FROM skills WHERE char_id=%u LIMIT 60", charId);
	q2->AddQuery("SELECT * FROM htb_skills WHERE char_id=%u LIMIT 2", charId); // limit NTL_HTB_MAX_PC_HAVE_HTB_SKILL
	q2->AddQuery("SELECT * FROM buffs WHERE char_id=%u LIMIT 49", charId);
	q2->AddQuery("SELECT * FROM questitems WHERE char_id=%u LIMIT 30", charId);// LIMIT NTL_QUEST_INVENTORY_SLOT_COUNT
	q2->AddQuery("SELECT * FROM quests WHERE char_id=%u", charId);
	q2->AddQuery("SELECT * FROM quickslot WHERE char_id=%u LIMIT 48", charId);
	q2->AddQuery("SELECT * FROM mascots WHERE char_id=%u LIMIT 40", charId);
	q2->AddQuery("SELECT * FROM hoipoi_recipe WHERE char_id=%u LIMIT 200", charId);
	q2->AddQuery("SELECT * FROM items_cd WHERE char_id=%u LIMIT 19", charId);
	q2->AddQuery("SELECT point FROM portals WHERE char_id=%u LIMIT 100", charId);
	GetCharDB.QueueAsyncQuery(q2);
}

void CCharacterRepository::InsertCharacter(sPC_SUMMARY& sSum, ACCOUNTID accountId)
{
	GetCharDB.Execute("INSERT INTO characters (id,char_name,account_id,race,class,gender,face,hair,hair_color,skin_color,cur_loc_x,cur_loc_y,cur_loc_z,world_id,world_table,map_info_index,create_time)"
		"VALUES (%u,'%ls',%u,%u,%u,%u,%u,%u,%u,%u,%f,%f,%f,%u,%u,%u,%I64u)",
		sSum.charId, sSum.awchName, accountId, sSum.byRace, sSum.byClass, sSum.byGender, sSum.byFace, sSum.byHair, sSum.byHairColor, sSum.bySkinColor,
		sSum.fPositionX, sSum.fPositionY, sSum.fPositionZ,
		sSum.worldId, sSum.worldTblidx, sSum.dwMapInfoIndex, time(0));
}
