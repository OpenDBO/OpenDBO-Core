#pragma once

#include "NtlSharedCommon.h"

#if defined(WIN64)

#include <string>

#define NtlAssert(condition, message)		NtlAssertHelper(condition, message, __FILE__, __LINE__)
#define NtlAssertFail(message)		NtlAssertFailHelper(message, __FILE__, __LINE__)

bool NtlAssertHelper(bool bCondition, Char* pchMessage, Char* pchFileName, Int32 iLineNumber);
bool NtlAssertHelper(bool bCondition, std::wstring* pwstrMessage, Char* pchFileName, Int32 iLineNumber);

bool NtlAssertFailHelper(Char* pchMessage, Char* pchFileName, Int32 iLineNumber);
bool NtlAssertFailHelper(std::wstring* pwstrMessage, Char* pchFileName, Int32 iLineNumber);

#elif defined(WIN32)

#include <string>

class CNtlAssert
{
public:
	static bool Init(void (*pvFunction)(Char* pszMessage));
	static bool Destroy();

	static bool ExecuteHandler(Char* pszMessage);
	static bool ExecuteHandler(std::wstring* pwszMessage);

private:
	static void (*m_pvNtlAssertFunction)(Char* pszMessage);
};

#define NtlAssertInit(function)		CNtlAssert::Init(function)

//#define NtlAssert(bCondition, message)
//	do \
//	{ \
//		if (FALSE == bCondition) \
//		{ \
//			CNtlAssert::ExecuteHandler(message); \
//		} \
//	} while (0);

//#define NtlAssertFail(message)
//	do \
//	{ \
//		CNtlAssert::ExecuteHandler(message); \
//	} while (0);

#define NtlAssert(bCondition, message)		void(message);
#define NtlAssertFail(message)		void(message);

#endif