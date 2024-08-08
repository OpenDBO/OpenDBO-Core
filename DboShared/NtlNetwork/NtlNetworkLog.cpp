#include "NtlNetworkLog.h"
#include <stdio.h>

char* CNtlNetworkLog::m_szShortMonthName[] =
{
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec",
	NULL
};

CNtlNetworkLog::CNtlNetworkLog(void)
{
	::InitializeCriticalSection(&m_logLock);

	Init();
}

CNtlNetworkLog::~CNtlNetworkLog(void)
{
	Destroy();

	::DeleteCriticalSection(&m_logLock);
}

bool
CNtlNetworkLog::Create(char* szLogFileNamePrefix)
{
	if (false == m_bIsEnabled)
		return true;

	ZeroMemory(m_szLogFileNamePrefix, (MAX_FILE_NAME_LENGTH - 13) + 1);
	ZeroMemory(m_szLogFileName, (MAX_FILE_NAME_LENGTH + 1));

	::EnterCriticalSection(&m_logLock);

	if (NULL != szLogFileNamePrefix)
	{
		// The final log file name will be like "LogFile_20050218.txt" where 20050218 means the date when the file created.
		// 13 means the length of the string "_20050218.txt".
		strncpy_s<_countof(m_szLogFileNamePrefix)>(m_szLogFileNamePrefix, szLogFileNamePrefix, MAX_FILE_NAME_LENGTH);
		m_szLogFileNamePrefix[MAX_FILE_NAME_LENGTH] = '\0';

		SYSTEMTIME localTime;
		GetLocalTime(&localTime);
		sprintf_s<_countof(m_szLogFileName)>(m_szLogFileName, "%s_%04d%02d%02d.txt", m_szLogFileNamePrefix, localTime.wYear, localTime.wMonth, localTime.wDay);

		FILE* pLogFile = NULL;
		if (0 != fopen_s(&pLogFile, m_szLogFileName, "a"))
		{
			ZeroMemory(m_szLogFileNamePrefix, (MAX_FILE_NAME_LENGTH - 13) + 1);
			ZeroMemory(m_szLogFileName, (MAX_FILE_NAME_LENGTH + 1));
			::LeaveCriticalSection(&m_logLock);

			return false;
		}
		fclose(pLogFile);

		m_wLastLogYear = localTime.wYear;
		m_wLastLogMonth = localTime.wMonth;
		m_wLastLogDay = localTime.wDay;
	}

	::LeaveCriticalSection(&m_logLock);

	return true;
}

bool
CNtlNetworkLog::Destroy()
{
	m_wLastLogYear = 0;
	m_wLastLogMonth = 0;
	m_wLastLogDay = 0;

	m_bIsEnabled = false;

	return true;
}

bool
CNtlNetworkLog::Init()
{
	m_wLastLogYear = 0;
	m_wLastLogMonth = 0;
	m_wLastLogDay = 0;

	m_bIsEnabled = true;

	return true;
}

bool
CNtlNetworkLog::AddLog(char* szFormatString, ...)
{
	bool bResult = false;
	va_list args;

	va_start(args, szFormatString);
	bResult = AddLogAlternative(szFormatString, args);
	va_end(args);

	return bResult;
}

bool
CNtlNetworkLog::AddLogAlternative(char* szFormatString, va_list args)
{
	if (false == m_bIsEnabled)
		return true;

	FILE* pLogFile = NULL;

	SYSTEMTIME localTime;
	char szTimeString[64 + 1];

	GetLocalTime(&localTime);
	sprintf_s<_countof(szTimeString)>(szTimeString,
			"%04d %03s %02d, %02d:%02d:%02d : ",
			localTime.wYear,
			CNtlNetworkLog::m_szShortMonthName[localTime.wMonth - 1],
			localTime.wDay,
			localTime.wHour,
			localTime.wMinute,
			localTime.wSecond);

	::EnterCriticalSection(&m_logLock);

	if ('\0' != m_szLogFileName[0])
	{
		if (m_wLastLogYear != localTime.wYear ||
			m_wLastLogMonth != localTime.wMonth ||
			m_wLastLogDay != localTime.wDay)
		{
			ZeroMemory(m_szLogFileName, (MAX_FILE_NAME_LENGTH + 1));
			sprintf_s<_countof(m_szLogFileName)>(m_szLogFileName, "%s_%04d%02d%02d.txt", m_szLogFileNamePrefix, localTime.wYear, localTime.wMonth, localTime.wDay);
		}

		pLogFile = NULL;
		if (0 != fopen_s(&pLogFile, m_szLogFileName, "a"))
		{
			m_wLastLogYear = localTime.wYear;
			m_wLastLogMonth = localTime.wMonth;
			m_wLastLogDay = localTime.wDay;
		}
	}

	if (NULL != pLogFile)
	{
		fprintf(pLogFile, szTimeString);
		vfprintf(pLogFile, szFormatString, args);

		fprintf(pLogFile, "\n");

		fclose(pLogFile);
	}
	else
	{
		printf(szTimeString);
		vprintf(szFormatString, args);

		printf("\n");
	}

	::LeaveCriticalSection(&m_logLock);

	return true;
}
