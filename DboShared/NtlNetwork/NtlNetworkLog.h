#pragma once
#include "NtlSharedCommon.h"

class CNtlNetworkLog
{
public:
	enum EMaxFileNameLength
	{
		MAX_FILE_NAME_LENGTH = 32
	};

public:
	CNtlNetworkLog(void);
	virtual ~CNtlNetworkLog(void);

	bool Create(char* szLogFileNamePrefix);
	bool Destroy();

protected:
	bool Init();

public:
	bool IsEnabled() { return m_bIsEnabled; }
	void SetEnabled() { m_bIsEnabled = true; }
	void SetDisabled() { m_bIsEnabled = false; }

	bool AddLog(char* szFormatString, ...);
	bool AddLogAlternative(char* szFormatString, va_list args);

private:
	static char* m_szShortMonthName[];

private:
	char m_szLogFileNamePrefix[(MAX_FILE_NAME_LENGTH - 13) + 1];
	char m_szLogFileName[MAX_FILE_NAME_LENGTH + 1];
	WORD m_wLastLogYear;
	WORD m_wLastLogMonth;
	WORD m_wLastLogDay;

	CRITICAL_SECTION m_logLock;

	bool m_bIsEnabled;
};
