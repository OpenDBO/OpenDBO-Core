// SampleServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "main.h"

#define SAMPLESERVER
//#define DBSAMPLESERVER


//-----------------------------------------------------------------------------------
//		샘플 서버 메인
//-----------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
#if defined( SAMPLESERVER )
	SampleServerMain(argc, argv);
#elif defined( DBSAMPLESERVER )
	DBSampleServerMain(argc, argv);
#endif

	return 0;
}
