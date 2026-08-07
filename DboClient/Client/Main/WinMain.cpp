#include "precomp_dboclient.h"
#include "DboApplication.h"
#include "NtlDebug.h"
#include "DboCompileOpt.h"
#include "DboDef.h"

char g_szAutoLoginID[256] = {0};
char g_szAutoLoginPass[256] = {0};

BOOL IsMultiOpen(const char *pExeName)
{
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, pExeName);
	if(NULL == hMutex)
		return FALSE;

	if(GetLastError() == ERROR_ALREADY_EXISTS)
		return FALSE;
	return TRUE;
}

BOOL IsLimitDirectory(void)
{
	RwChar chCurrDir[1024];
	memset(chCurrDir, 0, 1024);

	GetCurrentDirectory(1024, chCurrDir);

	RwInt32 iLen = (RwInt32)strlen(chCurrDir);
	if(iLen >= 200)
		return FALSE;

	return TRUE;
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{

#ifdef MUTEX_CHECK
	if ( NULL == lpCmdLine && strcmp( lpCmdLine, "nomutex" ) > 0 )
	{
		if(!IsMultiOpen("DragonBall"))
		{
			::MessageBox(NULL, "OpenDBO is already opened !!!", "OpenDBO", MB_OK);
			return 1;
		}
	}
#endif

	NtlClientNetSetPrintFlag( 0 );

	{
		char archBuf[128];
		sprintf_s(archBuf, "[OpenDBO] sizeof(void*)=%zu - Build: %s\n", sizeof(void*),
#if defined(_M_X64) || defined(_WIN64)
			"x64"
#else
			"x86"
#endif
		);
		OutputDebugStringA(archBuf);
	}

	if(!IsLimitDirectory())
	{
		::MessageBox(NULL, "A directory name length must not exceed 200 letters", "OpenDBO", MB_OK);
		return 1;
	}

	// Parse command line for auto-login credentials: --id=<user> --pass=<password>
	if (lpCmdLine && lpCmdLine[0])
	{
		char* idPos = strstr(lpCmdLine, "--id=");
		if (idPos)
		{
			idPos += 5;
			char* end = strchr(idPos, ' ');
			if (end) { strncpy_s(g_szAutoLoginID, idPos, end - idPos); }
			else { strcpy_s(g_szAutoLoginID, idPos); }
		}
		char* passPos = strstr(lpCmdLine, "--pass=");
		if (passPos)
		{
			passPos += 7;
			char* end = strchr(passPos, ' ');
			if (end) { strncpy_s(g_szAutoLoginPass, passPos, end - passPos); }
			else { strcpy_s(g_szAutoLoginPass, passPos); }
		}
	}

	CDboApplication app;
	
	app.Init();

#ifdef NDEBUG
	
	if(!app.Create(hInstance, 0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, FALSE))
	{
		NTL_ASSERTE("Application create failed!!!");
		return 1;
	}

#else

	if(!app.Create(hInstance, 0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, NTL_DEBUG_WINDOW_MODE))
	{
		NTL_ASSERTE("Application create failed!!!");
		return 1;
	}

#endif


	app.MessagePump();
	app.Destroy();

	return 1;
}


