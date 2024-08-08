#include "precomp_dboclient.h"
#include "DboApplication.h"
#include "NtlDebug.h"
#include "DboCompileOpt.h"
#include "DboDef.h"
#include "NtlClientService.h"

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
			::MessageBox(NULL, "DragonBall is already opened !!!", "DragonBall", MB_OK);
			return 1;
		}
	}
#endif

	NtlClientNetSetPrintFlag( 0 );

	// 서비스 시작
	if ( !GetServiceInterface()->Begin( lpCmdLine ) )
	{
		::MessageBox(NULL, "Wrong service information.", "DragonBall", MB_OK);
		return 1;
	}

	eDBO_SERVICE_PUBLISHER eService = GetServiceInterface()->GetPublisherID();
	if( eDBO_SERVICE_PUBLISHER_CJI_KOR == eService )
	{
		for( RwUInt8 i = 0 ; i < 4 ; ++i )
		{
			RwChar* pcData = (RwChar*)GetServiceInterface()->GetServiceData(i);

			if( !pcData || strlen(pcData) == 0 )
			{
				::MessageBox(NULL, "Wrong service data", "DragonBall", MB_OK);
				return 1;
			}
		}
	}

	if(!IsLimitDirectory())
	{
		::MessageBox(NULL, "A directory name length must not exceed 200 letters", "DragonBall", MB_OK);
		return 1;
	}

	// 보안 솔루션 시작
	// - 보안 솔루션이 실패 했어도 게임이 실행될때까지 종료하지 않고 대기하고 있다가 종료시킨다
	GetServiceInterface()->CreateSecuritySolution();

	CDboApplication app;
	
	app.Init();

#ifdef NDEBUG
	
	if(!app.Create(hInstance, 0, 0, EXTRA_SCREEN_WIDTH, EXTRA_SCREEN_HEIGHT, TRUE))
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

	// 보안 솔루션에 윈도우를 연결한다
	GetServiceInterface()->AttachSecuritySolutionWindow( app.GetHWnd() );

	app.MessagePump();
	app.Destroy();

	// 보안 솔루션 종료
	GetServiceInterface()->DeleteSecuritySolution();

	// 서비스 종료
	GetServiceInterface()->End();

	return 1;
}


