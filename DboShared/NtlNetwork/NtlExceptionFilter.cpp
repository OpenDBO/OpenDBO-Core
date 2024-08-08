#include "NtlExceptionFilter.h"
#include <time.h>

CRITICAL_SECTION CNtlExceptionFilter::m_exceptionCriticalSection;
bool CNtlExceptionFilter::m_bPrepared = false;
DWORD CNtlExceptionFilter::m_dwOldOptions = 0x00000000;
DWORD CNtlExceptionFilter::m_dwNewOptions = 0x00000000;
SYSTEMTIME CNtlExceptionFilter::m_localTime;
char CNtlExceptionFilter::m_szTempBuffer[TEMP_BUFFER_LENGTH];
STACKFRAME CNtlExceptionFilter::m_stackFrame;
IMAGEHLP_SYMBOL* CNtlExceptionFilter::m_pSymbol = (IMAGEHLP_SYMBOL*)CNtlExceptionFilter::m_symbolAlternative;
char CNtlExceptionFilter::m_symbolAlternative[SYMBOL_INFO_CONTAINER_LENGTH];
IMAGEHLP_MODULE CNtlExceptionFilter::m_module;
IMAGEHLP_LINE CNtlExceptionFilter::m_line;

CNtlExceptionFilter::CNtlExceptionFilter(void)
{
	CNtlExceptionFilter::Init();
}

CNtlExceptionFilter::~CNtlExceptionFilter(void)
{
	CNtlExceptionFilter::Destroy();
}

bool
CNtlExceptionFilter::Create()
{
	::InitializeCriticalSection(&m_exceptionCriticalSection);

	CNtlExceptionFilter::Init();

	return true;
}

bool
CNtlExceptionFilter::Destroy()
{
	::DeleteCriticalSection(&m_exceptionCriticalSection);

	return true;
}

bool
CNtlExceptionFilter::Init()
{
	return true;
}

DWORD
CNtlExceptionFilter::ExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo)
{
	// Prevents other threads from handling exceptions at the same time.
	::EnterCriticalSection(&m_exceptionCriticalSection);

	GetLocalTime(&m_localTime);
	sprintf_s<_countof(m_szTempBuffer)>(m_szTempBuffer,
			"MemoryDump_%04d%02d%02d_%02d%02d%02d.txt",
			m_localTime.wYear,
			m_localTime.wMonth,
			m_localTime.wDay,
			m_localTime.wHour,
			m_localTime.wMinute,
			m_localTime.wSecond);

	FILE* pFile = NULL;
	if (0 != fopen_s(&pFile, m_szTempBuffer, "at"))
	{
		::LeaveCriticalSection(&m_exceptionCriticalSection);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	if (false == IsPrepared())
	{
		if (false == Prepare(pFile))
		{
			fclose(pFile);
			::LeaveCriticalSection(&m_exceptionCriticalSection);
			return EXCEPTION_EXECUTE_HANDLER;
		}
	}

	DWORD dwProcessID = GetCurrentProcessId();
	DWORD dwThreadID = GetCurrentThreadId();
	CONTEXT* pContext = pExceptionInfo->ContextRecord;

	fprintf(pFile, "==================================================\n");
	_strdate_s<_countof(m_szTempBuffer)>(m_szTempBuffer);
	fprintf(pFile, "Date : %s\n", m_szTempBuffer);
	_strtime_s<_countof(m_szTempBuffer)>(m_szTempBuffer);
	fprintf(pFile, "Time : %s\n", m_szTempBuffer);
	fprintf(pFile, "Process : 0x%x(%d)\n", dwProcessID, dwProcessID);
	fprintf(pFile, "Thread : 0x%x(%d)\n", dwThreadID, dwThreadID);
	fprintf(pFile, "\n");
	fprintf(pFile, "Exception Type : 0x%08x(%s)\n",
			pExceptionInfo->ExceptionRecord->ExceptionCode,
			GetExceptionDescription(pExceptionInfo->ExceptionRecord->ExceptionCode));

	ZeroMemory(&m_stackFrame, sizeof(m_stackFrame));
//- yoshiki : Modify this macro!
#if 0
//	m_stackFrame.AddrPC.Offset = pContext->Rip;
//	m_stackFrame.AddrPC.Mode = AddrModeFlat;
//	m_stackFrame.AddrStack.Offset = pContext->Rsp;
//	m_stackFrame.AddrStack.Mode = AddrModeFlat;
//	m_stackFrame.AddrFrame.Offset = pContext->Rbp;
//	m_stackFrame.AddrFrame.Mode = AddrModeFlat;
#else
	m_stackFrame.AddrPC.Offset = pContext->Eip;
	m_stackFrame.AddrPC.Mode = AddrModeFlat;
	m_stackFrame.AddrStack.Offset = pContext->Esp;
	m_stackFrame.AddrStack.Mode = AddrModeFlat;
	m_stackFrame.AddrFrame.Offset = pContext->Ebp;
	m_stackFrame.AddrFrame.Mode = AddrModeFlat;
#endif

	// Traces up to 512 stack frames.
	for (int i=0 ; i<512 ; i++)
	{
//- yoshiki : Modify this macro!
#if 0
		if (FALSE == StackWalk(IMAGE_FILE_MACHINE_AMD64, GetCurrentProcess(), GetCurrentThread(), &m_stackFrame, pContext, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
			break;
#else
		if (FALSE == StackWalk(IMAGE_FILE_MACHINE_I386, GetCurrentProcess(), GetCurrentThread(), &m_stackFrame, pContext, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL))
			break;
#endif

		if (0 == m_stackFrame.AddrFrame.Offset)
			break;

		ZeroMemory(m_pSymbol, SYMBOL_INFO_CONTAINER_LENGTH);
		m_pSymbol->SizeOfStruct = sizeof(*m_pSymbol);
		// MaxNameLength could have bigger value by 1, but I'm not sure if SymGetSymFromAddr() guarantees a null-terminated symbol name.
		m_pSymbol->MaxNameLength = SYMBOL_INFO_CONTAINER_LENGTH - sizeof(*m_pSymbol);

		fprintf(pFile, "--------------------------------------------------\n");
//- yoshiki : Modify this macro!
#if 0
		DWORD64 qwDisplacement = 0;
		if (FALSE != SymGetSymFromAddr(GetCurrentProcess(), m_stackFrame.AddrPC.Offset, &qwDisplacement, m_pSymbol))
#else
		DWORD dwDisplacement = 0;
		if (FALSE != SymGetSymFromAddr(GetCurrentProcess(), m_stackFrame.AddrPC.Offset, &dwDisplacement, m_pSymbol))
#endif
		{
			fprintf(pFile, "0x%08x - %s() + %xh\n", m_stackFrame.AddrPC.Offset, m_pSymbol->Name, (m_stackFrame.AddrPC.Offset - m_pSymbol->Address));
		}
		else
		{
			int iErrorCode = GetLastError();
			fprintf(pFile, "0x%08x - [Unknown Symbol : %s(%d)]\n", m_stackFrame.AddrPC.Offset, GetErrorMessage(iErrorCode), iErrorCode);
		}

		ZeroMemory(&m_module, sizeof(m_module));
		m_module.SizeOfStruct = sizeof(m_module);
		if (FALSE != SymGetModuleInfo(GetCurrentProcess(), m_stackFrame.AddrPC.Offset, &m_module))
		{
			fprintf(pFile, "\tImageName : %s\n", m_module.ImageName);
			fprintf(pFile, "\tLoadedImageName : %s\n", m_module.LoadedImageName);
		}
		else
		{
			int iErrorCode = GetLastError();
			fprintf(pFile, "SymGetModuleInfo failed.(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
		}

		ZeroMemory(&m_line, sizeof(m_line));
		m_line.SizeOfStruct = sizeof(m_line);

		// There was a bug called 'source line bug', I heard, which doesn't need to be considered now.
		// But just in case.
		for (DWORD i=0 ; i <= 512 ; i++)
		{
//- yoshiki : Modify this macro!
#if 0
			DWORD64 qwDisplacement = 0;
			if (FALSE != SymGetLineFromAddr(GetCurrentProcess(), (m_stackFrame.AddrPC.Offset - i), &qwDisplacement, &m_line))
#else
			DWORD dwDisplacement = 0;
			if (FALSE != SymGetLineFromAddr(GetCurrentProcess(), (m_stackFrame.AddrPC.Offset - i), &dwDisplacement, &m_line))
#endif
			{
				fprintf(pFile, "\tFileName : %s\n", m_line.FileName);
				fprintf(pFile, "\tLineNumber : %d\n", m_line.LineNumber);
				break;
			}
//			else
//			{
//				int iErrorCode = GetLastError();
//				fprintf(pFile, "SymGetLineFromAddr failed.(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
//			}
		}

		fprintf(pFile, "\n");
		fprintf(pFile, "\tstackFrame.AddrPC.Offset = 0x%08x(%d)\n", m_stackFrame.AddrPC.Offset, m_stackFrame.AddrPC.Offset);
		fprintf(pFile, "\tstackFrame.AddrReturn.Offset = 0x%08x(%d)\n", m_stackFrame.AddrReturn.Offset, m_stackFrame.AddrReturn.Offset);
		fprintf(pFile, "\tstackFrame.AddrFrame.Offset = 0x%08x(%d)\n", m_stackFrame.AddrFrame.Offset, m_stackFrame.AddrFrame.Offset);
		fprintf(pFile, "\tstackFrame.AddrStack.Offset = 0x%08x(%d)\n", m_stackFrame.AddrStack.Offset, m_stackFrame.AddrStack.Offset);
		fprintf(pFile, "\tstackFrame.Params[0] = 0x%08x(%d)\n", m_stackFrame.Params[0], m_stackFrame.Params[0]);
		fprintf(pFile, "\tstackFrame.Params[1] = 0x%08x(%d)\n", m_stackFrame.Params[1], m_stackFrame.Params[1]);
		fprintf(pFile, "\tstackFrame.Params[2] = 0x%08x(%d)\n", m_stackFrame.Params[2], m_stackFrame.Params[2]);
		fprintf(pFile, "\tstackFrame.Params[3] = 0x%08x(%d)\n", m_stackFrame.Params[3], m_stackFrame.Params[3]);
	} 

	if (false == CleanUp(pFile))
	{
		fclose(pFile);
		::LeaveCriticalSection(&m_exceptionCriticalSection);
		return EXCEPTION_EXECUTE_HANDLER;
	}

	fclose(pFile);
	::LeaveCriticalSection(&m_exceptionCriticalSection);

	Destroy();
	return EXCEPTION_EXECUTE_HANDLER;
}

bool
CNtlExceptionFilter::Prepare(FILE* pDumpFile)
{
	if (false != m_bPrepared)
		return true;
	if (NULL == pDumpFile)
		return false;

	// Guarantees the following options.
	// SYMOPT_DEFERRED_LOADS
	// SYMOPT_UNDNAME
	// SYMOPT_LOAD_LINES
	m_dwOldOptions = SymGetOptions();
	m_dwNewOptions = SymSetOptions(m_dwOldOptions | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES);

	if (FALSE == SymInitialize(GetCurrentProcess(), NULL, TRUE))
	{
//		int iErrorCode = GetLastError();
//		fprintf(pDumpFile, "ERROR : SymInitialize failed.(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
		return false;
	}

	m_bPrepared = true;

	return true;
}

bool
CNtlExceptionFilter::CleanUp(FILE* pDumpFile)
{
	if (false == m_bPrepared)
		return true;
	if (NULL == pDumpFile)
		return false;

	// Restores options with the previous one.
	m_dwNewOptions = SymSetOptions(m_dwOldOptions);

	if (FALSE == SymCleanup(GetCurrentProcess()))
	{
//		int iErrorCode = GetLastError();
//		fprintf(pDumpFile, "ERROR : SymCleanup failed.(%s : %d)\n", GetErrorMessage(iErrorCode), iErrorCode);
		return false;
	}

	m_bPrepared = false;

	return true;
}

char *
CNtlExceptionFilter::GetExceptionDescription(DWORD dwExceptionCode)
{
	switch (dwExceptionCode)
	{
	case EXCEPTION_ACCESS_VIOLATION :
		return "EXCEPTION_ACCESS_VIOLATION";
		break;

	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED :
		return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
		break;

	case EXCEPTION_BREAKPOINT :
		return "EXCEPTION_BREAKPOINT";
		break;

	case EXCEPTION_DATATYPE_MISALIGNMENT :
		return "EXCEPTION_DATATYPE_MISALIGNMENT";
		break;

	case EXCEPTION_FLT_DENORMAL_OPERAND :
		return "EXCEPTION_FLT_DENORMAL_OPERAND";
		break;

	case EXCEPTION_FLT_DIVIDE_BY_ZERO :
		return "EXCEPTION_FLT_DIVIDE_BY_ZERO";
		break;

	case EXCEPTION_FLT_INEXACT_RESULT :
		return "EXCEPTION_FLT_INEXACT_RESULT";
		break;

	case EXCEPTION_FLT_INVALID_OPERATION :
		return "EXCEPTION_FLT_INVALID_OPERATION";
		break;

	case EXCEPTION_FLT_OVERFLOW :
		return "EXCEPTION_FLT_OVERFLOW";
		break;

	case EXCEPTION_FLT_STACK_CHECK :
		return "EXCEPTION_FLT_STACK_CHECK";
		break;

	case EXCEPTION_FLT_UNDERFLOW :
		return "EXCEPTION_FLT_UNDERFLOW";
		break;

	case EXCEPTION_ILLEGAL_INSTRUCTION :
		return "EXCEPTION_ILLEGAL_INSTRUCTION";
		break;

	case EXCEPTION_IN_PAGE_ERROR :
		return "EXCEPTION_IN_PAGE_ERROR";
		break;

	case EXCEPTION_INT_DIVIDE_BY_ZERO :
		return "EXCEPTION_INT_DIVIDE_BY_ZERO";
		break;

	case EXCEPTION_INT_OVERFLOW :
		return "EXCEPTION_INT_OVERFLOW";
		break;

	case EXCEPTION_INVALID_DISPOSITION :
		return "EXCEPTION_INVALID_DISPOSITION";
		break;

	case EXCEPTION_NONCONTINUABLE_EXCEPTION :
		return "EXCEPTION_NONCONTINUABLE_EXCEPTION";
		break;

	case EXCEPTION_PRIV_INSTRUCTION :
		return "EXCEPTION_PRIV_INSTRUCTION";
		break;

	case EXCEPTION_SINGLE_STEP :
		return "EXCEPTION_SINGLE_STEP";
		break;

	case EXCEPTION_STACK_OVERFLOW :
		return "EXCEPTION_STACK_OVERFLOW";
		break;

	default :
		return "Unknown exception";
		break;
	}
}

char *
CNtlExceptionFilter::GetErrorMessage(int iErrorCode)
{
	static char szErrorMessage[256 + 1];
	szErrorMessage[0] = '\0';
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, iErrorCode, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), szErrorMessage, 256, NULL);
	szErrorMessage[strlen(szErrorMessage) - 1] = '\0';
	szErrorMessage[strlen(szErrorMessage) - 1] = '\0';

	return szErrorMessage;
}