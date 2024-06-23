//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_MUTEX_INCLUDED
#define PLATFORM_COMMON_MUTEX_INCLUDED

//#include <windows.h>

struct sCriticalSectionStandIn
{
    void* a;
    long b;
    long c;
    void* d;
    void* e;
    unsigned long f;
};

struct _RTL_CRITICAL_SECTION;
extern "C"
{
__declspec(dllimport) void __stdcall InitializeCriticalSection(_RTL_CRITICAL_SECTION*);
__declspec(dllimport) void __stdcall DeleteCriticalSection(_RTL_CRITICAL_SECTION*);
__declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION*);
__declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION*);
}

class cMutex
{
    mutable sCriticalSectionStandIn _cs;

    // prevent copying and assignment
    cMutex(const cMutex&);
    const cMutex& operator=(const cMutex&);

public:

    cMutex()
    {
        InitializeCriticalSection((_RTL_CRITICAL_SECTION*)&_cs);
    }
    ~cMutex()
    {
        DeleteCriticalSection((_RTL_CRITICAL_SECTION*)&_cs);
    }

    void lock() const
    {
        EnterCriticalSection((_RTL_CRITICAL_SECTION*)&_cs);
    }
    void unlock() const
    {
        LeaveCriticalSection((_RTL_CRITICAL_SECTION*)&_cs);
    }
};

#endif
