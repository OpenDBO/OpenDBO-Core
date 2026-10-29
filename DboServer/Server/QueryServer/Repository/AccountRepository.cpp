#include "stdafx.h"
#include "AccountRepository.h"
#include "../QueryServer.h"
#include "../PlayerCache.h"
#include "NtlPacketQG.h"
#include "../GameServerSession.h"


void CAccountRepository::LoadAccountDataAsync(CAccountCache* pAccount, ACCOUNTID accountId)
{
	SQLCallbackBase* pCallBack = new SQLClassCallbackP0<CAccountCache>(pAccount, &CAccountCache::OnLoadAccountInfo);
	AsyncQuery * q = new AsyncQuery(pCallBack);
	q->AddQuery("SELECT mallpoints, is_gm, premium_slots, event_coins, wagu_coins FROM accounts WHERE id=%u", accountId);
	q->AddQuery("SELECT * FROM cashshop_storage WHERE account_id=%u AND is_moved=0", accountId);
	q->AddQuery("SELECT action_id, key_code FROM shortcuts WHERE account_id=%u", accountId);
	GetAccDB.QueueAsyncQuery(q);
}

bool CAccountRepository::UpdateEventCoinsWait(ACCOUNTID accountId, DWORD eventCoins)
{
	return GetAccDB.WaitExecute("UPDATE accounts SET event_coins=%u WHERE id=%u", eventCoins, accountId); //if player bought cash and did not update his cash in game.. This is why we do like this
}

bool CAccountRepository::UpdateWaguCoinsWait(ACCOUNTID accountId, DWORD waguCoins)
{
	return GetAccDB.WaitExecute("UPDATE accounts SET wagu_coins=%u WHERE id=%u", waguCoins, accountId); //if player bought cash and did not update his cash in game.. This is why we do like this
}

void CAccountRepository::InsertShortcut(ACCOUNTID accountId, WORD wActionID, WORD wKey)
{
	GetAccDB.Execute("INSERT INTO shortcuts (account_id,action_id,key_code) VALUES (%u,%u,%u)", accountId, wActionID, wKey);
}

void CAccountRepository::DeleteShortcut(ACCOUNTID accountId, WORD wActionID)
{
	GetAccDB.Execute("DELETE FROM shortcuts WHERE account_id=%u AND action_id=%u", accountId, wActionID);
}

void CAccountRepository::UpdateShortcutKey(WORD wKey, ACCOUNTID accountId, WORD wActionID)
{
	GetAccDB.Execute("UPDATE shortcuts SET key_code=%u WHERE account_id=%u AND action_id=%u", wKey, accountId, wActionID);
}

smart_ptr<QueryResult> CAccountRepository::GetMallpoints(ACCOUNTID accountId)
{
	return GetAccDB.Query("SELECT mallpoints FROM accounts WHERE id=%u", accountId);
}

bool CAccountRepository::UpdateMallpointsDeductWait(DWORD price, ACCOUNTID accountId)
{
	return GetAccDB.WaitExecute("UPDATE accounts SET mallpoints=mallpoints-%u WHERE id=%u", price, accountId); //if player bought cash and did not update his cash in game.. This is why we do like this
}

void CAccountRepository::BanAccount(ACCOUNTID targetAccountId)
{
	GetAccDB.Execute("UPDATE accounts SET acc_status='block' WHERE id=%u", targetAccountId);
}

void CAccountRepository::UpdateMallpointsWait(DWORD mallpoints, ACCOUNTID accountId)
{
	GetAccDB.WaitExecute("UPDATE accounts SET mallpoints=%u WHERE id=%u", mallpoints, accountId);
}

void CAccountRepository::UpdateEventCoins(ACCOUNTID accountId, DWORD eventCoins)
{
	GetAccDB.Execute("UPDATE accounts SET event_coins=%u WHERE id=%u", eventCoins, accountId);
}

void CAccountRepository::UpdateWaguCoins(ACCOUNTID accountId, DWORD waguCoins)
{
	GetAccDB.Execute("UPDATE accounts SET wagu_coins=%u WHERE id=%u", waguCoins, accountId);
}

void CAccountRepository::LoadEventRewardAsync(CAccountCache* pAccount, ACCOUNTID accountId, HOBJECT handle, CHARACTERID charId)
{
	SQLCallbackBase* pCallBack = new SQLClassCallbackP2<CAccountCache, HOBJECT, CHARACTERID>(pAccount, &CAccountCache::OnLoadEventReward, handle, charId);
	AsyncQuery * q = new AsyncQuery(pCallBack);
	q->AddQuery("SELECT reward_tblidx, char_id, char_name FROM event_reward WHERE account_id=%u LIMIT %u", accountId, NTL_MAX_EVENT_REWARD_COUNT_IN_PACKET);
	GetAccDB.QueueAsyncQuery(q);
}

void CAccountRepository::DeleteEventReward(ACCOUNTID accountId, TBLIDX eventTblidx)
{
	GetAccDB.Execute("DELETE FROM event_reward WHERE account_id=%u AND reward_tblidx=%u", accountId, eventTblidx);
}

// a reward any character of the account takes once (char_id 0), an account that already has it keeps its row
void CAccountRepository::InsertAccountEventReward(ACCOUNTID accountId, TBLIDX eventTblidx)
{
	GetAccDB.Execute("INSERT IGNORE INTO event_reward (account_id,reward_tblidx,char_id,char_name) VALUES (%u,%u,0,\"\")", accountId, eventTblidx);
}

DWORD CAccountRepository::InsertAccountEventRewardAll(TBLIDX eventTblidx)
{
	DWORD dwCount = 0;

	smart_ptr<QueryResult> result = GetAccDB.Query("SELECT COUNT(*) FROM accounts");
	if (result)
		dwCount = result->Fetch()[0].GetUInt32();

	GetAccDB.Execute("INSERT IGNORE INTO event_reward (account_id,reward_tblidx,char_id,char_name) SELECT id,%u,0,\"\" FROM accounts", eventTblidx);

	return dwCount;
}

void CAccountRepository::CheckAccountStatusAsync(CGameServerSession* pSession, CHARACTERID charId, ACCOUNTID accountId)
{
	SQLCallbackBase* pCallBack3 = new SQLClassCallbackP2<CGameServerSession, CHARACTERID, ACCOUNTID>(pSession, &CGameServerSession::OnAccountCheck, charId, accountId);
	AsyncQuery * q3 = new AsyncQuery(pCallBack3);
	q3->AddQuery("SELECT acc_status FROM accounts WHERE id=%u", accountId);
	GetAccDB.QueueAsyncQuery(q3);
}
