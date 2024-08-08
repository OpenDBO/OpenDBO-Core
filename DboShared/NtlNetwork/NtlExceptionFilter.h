#pragma once
#include "NtlSharedCommon.h"
#include <Dbghelp.h>
#include <stdio.h>

class CNtlExceptionFilter
{
	enum EMaxNumber
	{
		TEMP_BUFFER_LENGTH = 128,
		SYMBOL_INFO_CONTAINER_LENGTH = 256
	};

public:
	virtual ~CNtlExceptionFilter(void);

	static bool Create();
	static bool Destroy();
protected:
	static bool Init();

public:
	static DWORD ExceptionFilter(EXCEPTION_POINTERS* pExceptionInfo);

protected:
	CNtlExceptionFilter(void);

	static bool IsPrepared() { return m_bPrepared; }
	static bool Prepare(FILE* pDumpFile);
	static bool CleanUp(FILE* pDumpFile);

	static char *GetExceptionDescription(DWORD dwExceptionCode);
	static char *GetErrorMessage(int iErrorCode);

private:
	static CRITICAL_SECTION m_exceptionCriticalSection;

	static bool m_bPrepared;
	static DWORD m_dwOldOptions;
	static DWORD m_dwNewOptions;

	static SYSTEMTIME m_localTime;
	static char m_szTempBuffer[TEMP_BUFFER_LENGTH];
	static STACKFRAME m_stackFrame;
	static IMAGEHLP_SYMBOL* m_pSymbol;
	static char m_symbolAlternative[SYMBOL_INFO_CONTAINER_LENGTH];
	static IMAGEHLP_MODULE m_module;
	static IMAGEHLP_LINE m_line;
};
