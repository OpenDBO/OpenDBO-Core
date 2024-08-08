//**********************************************************************
//
// Copyright (c) 2005
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_RANDOM_SEED_INCLUDED
#define PLATFORM_COMMON_RANDOM_SEED_INCLUDED

//// headered here to avoid inclusion of windows.h
//__declspec(dllimport) unsigned long __stdcall timeGetTime(void);
//
//namespace nPlatformCommon
//{
//
//unsigned long
//generateRandomSeed()
//{
//    unsigned long result = timeGetTime();
//    return result;
//}
//
//}

#include "platform_common/Timer.h"

namespace nPlatformCommon
{

unsigned long
generateRandomSeed()
{
    unsigned long high, low;
    DoRDTSC(high, low);
    return low;
}

}

#endif
