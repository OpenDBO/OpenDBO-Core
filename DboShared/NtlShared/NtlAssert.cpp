#include "stdafx.h"
#include "NtlAssert.h"

#if defined(WIN64)

#include <stdio.h>

bool NtlAssertHelper(bool bCondition, Char* pchMessage, Char* pchFileName, Int32 iLineNumber)
{
	if (NULL == pchMessage || NULL == pchFileName)
		return false;

	if (false == bCondition)
	{
		printf("%s\n", pchMessage);
		printf("File Name : %s\n", pchFileName);
		printf("Line Number : %d\n", iLineNumber);
	}

	return true;
}

bool NtlAssertHelper(bool bCondition, std::wstring* pwstrMessage, Char* pchFileName, Int32 iLineNumber)
{
	if (NULL == pchFileName)
		return false;

	if (false == bCondition)
	{
		wprintf(L"%s\n", pwstrMessage->data());
		printf("File Name : %s\n", pchFileName);
		printf("Line Number : %d\n", iLineNumber);
	}

	return true;
}

bool NtlAssertFailHelper(Char* pchMessage, Char* pchFileName, Int32 iLineNumber)
{
	if (NULL == pchMessage || NULL == pchFileName)
		return false;

	printf("%s\n", pchMessage);
	printf("File Name : %s\n", pchFileName);
	printf("Line Number : %d\n", iLineNumber);

	return true;
}

bool NtlAssertFailHelper(std::wstring* pwstrMessage, Char* pchFileName, Int32 iLineNumber)
{
	if (NULL == pchFileName)
		return false;

	wprintf(L"%s\n", pwstrMessage->data());
	printf("File Name : %s\n", pchFileName);
	printf("Line Number : %d\n", iLineNumber);

	return true;
}

#elif defined(WIN32)

void (*CNtlAssert::m_pvNtlAssertFunction)(Char* pszMessage);

bool CNtlAssert::Init(void (*pvFunction)(Char* pszMessage))
{
	if (FALSE != IsBadCodePtr((FARPROC)pvFunction))
		return false;

	CNtlAssert::m_pvNtlAssertFunction = pvFunction;

	return true;
}

bool CNtlAssert::Destroy()
{
	m_pvNtlAssertFunction = NULL;

	return true;
}

bool CNtlAssert::ExecuteHandler(Char* pszMessage)
{
	(*m_pvNtlAssertFunction)(pszMessage);

	return true;
}

bool CNtlAssert::ExecuteHandler(std::wstring* pwszMessage)
{
	static Char szResultString[1024 + 1];
	int iWrittenChars = ::WideCharToMultiByte(GetACP(), 0, pwszMessage->data(), -1, szResultString, (1024 + 1), NULL, NULL);
	if (0 == iWrittenChars)
		return false;

	(*m_pvNtlAssertFunction)(szResultString);

	return true;
}

#endif