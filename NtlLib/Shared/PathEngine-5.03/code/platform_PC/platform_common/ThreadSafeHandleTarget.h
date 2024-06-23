//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_THREAD_SAFE_HANDLE_TARGET_INCLUDED
#define PLATFORM_COMMON_THREAD_SAFE_HANDLE_TARGET_INCLUDED

//#include <windows.h>

extern "C"
{
__declspec(dllimport) long __stdcall InterlockedIncrement(long volatile *);
__declspec(dllimport) long __stdcall InterlockedDecrement(long volatile *);
__declspec(dllimport) long __stdcall InterlockedExchangeAdd(long volatile *, long);
}

class bThreadSafeHandleTarget
{
    long _ref;

    // prevent copying and assignment
    bThreadSafeHandleTarget(const bThreadSafeHandleTarget&);
    const bThreadSafeHandleTarget& operator=(const bThreadSafeHandleTarget&);

public:

    typedef bThreadSafeHandleTarget tTargetBase;

    bThreadSafeHandleTarget() :
        _ref(0)
    {
    }
    virtual ~bThreadSafeHandleTarget()
    {
    }

    void incRef()
    {
        InterlockedIncrement(&_ref);
    }
    void decRef()
    {
        if(InterlockedDecrement(&_ref) == 0)
        {
            delete this;
        }
    }
    bool hasMoreThanOneRef()
    {
        return InterlockedExchangeAdd(&_ref, 0) > 1;
    }

};

#endif
