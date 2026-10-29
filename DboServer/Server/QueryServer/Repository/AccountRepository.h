#pragma once

#include "NtlSingleton.h"
#include "DatabaseEnv.h"
#include "NtlSharedType.h"

class CAccountCache;
class CGameServerSession;

class CAccountRepository : public CNtlSingleton<CAccountRepository>
{

public:

	CAccountRepository() {}
	virtual ~CAccountRepository() {}

public:

	void						LoadAccountDataAsync(CAccountCache* pAccount, ACCOUNTID accountId);

	bool						UpdateEventCoinsWait(ACCOUNTID accountId, DWORD eventCoins);
	bool						UpdateWaguCoinsWait(ACCOUNTID accountId, DWORD waguCoins);

	void						InsertShortcut(ACCOUNTID accountId, WORD wActionID, WORD wKey);
	void						DeleteShortcut(ACCOUNTID accountId, WORD wActionID);
	void						UpdateShortcutKey(WORD wKey, ACCOUNTID accountId, WORD wActionID);

	smart_ptr<QueryResult>		GetMallpoints(ACCOUNTID accountId);
	bool						UpdateMallpointsDeductWait(DWORD price, ACCOUNTID accountId);
	void						BanAccount(ACCOUNTID targetAccountId);
	void						UpdateMallpointsWait(DWORD mallpoints, ACCOUNTID accountId);
	void						UpdateEventCoins(ACCOUNTID accountId, DWORD eventCoins);
	void						UpdateWaguCoins(ACCOUNTID accountId, DWORD waguCoins);
	void						LoadEventRewardAsync(CAccountCache* pAccount, ACCOUNTID accountId, HOBJECT handle, CHARACTERID charId);
	void						DeleteEventReward(ACCOUNTID accountId, TBLIDX eventTblidx);
	void						InsertAccountEventReward(ACCOUNTID accountId, TBLIDX eventTblidx);
	DWORD						InsertAccountEventRewardAll(TBLIDX eventTblidx);
	void						CheckAccountStatusAsync(CGameServerSession* pSession, CHARACTERID charId, ACCOUNTID accountId);

};

#define GetAccountRepository()		CAccountRepository::GetInstance()
#define g_pAccountRepository		GetAccountRepository()
