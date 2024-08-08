//***********************************************************************************
//
//	File		:	NtlBase.h
//
//	Begin		:	2005-12-05
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	NTL Library Base header file
//
//***********************************************************************************

#ifndef __NTLSYSTEM_H__
#define __NTLSYSTEM_H__

// for QueueUserWorkItem API & InitializeCriticalSectionAndSpinCount
#if !defined(_WIN32_WINNT)
	#define _WIN32_WINNT 0x0500 	
#elif (_WIN32_WINNT < 0x0500)
	#undef _WIN32_WINNT
	#define _WIN32_WINNT 0x0500
#endif


#include "..\..\Shared\NtlSharedCommon.h"
#include <stdlib.h>


// NULL define
#ifndef NULL
	#ifdef __cplusplus
		#define NULL    0
	#else
		#define NULL    ((void *)0)
	#endif
#endif

// memory safe delete
#if !defined(SAFE_DELETE)
	#define SAFE_DELETE(p)			if(p) { delete p; p = 0; }
#endif
#define SAFE_DELETE_ARRAY(p)	if(p) { delete[] p; p = 0; }
#define SAFE_FREE(p)			if(p) { free(p); p = 0; }

// macro sleep
#define SleepSec(s) Sleep( (s) * 1000 )

#define NTL_SUCCESS				0
#define INVALID_HSESSION		0xFFFFFFFF
#define INVALID_HDATABASE		0xFFFFFFFF

typedef unsigned int			HSESSION;
typedef unsigned short			SESSIONTYPE;
typedef unsigned int			HDATABASE;







#endif // __NTLSYSTEM_H__
