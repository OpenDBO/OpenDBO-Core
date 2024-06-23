//**********************************************************************
//
// Copyright (c) 2002-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

// This file to be included in just one .obj
// Defines global new and delete operators if enabled by the relevant define
// Only one such set of global operators should be enabled for any given build

#include "common/MemoryTracking_Interface.h"

#ifdef MEMORY_TRACKING

#include "common/MutexLock.h"
#include "platform_common/WorkingDirectory.h"

#include <stdlib.h>
#include <stdio.h>

static unsigned long totalAllocated = 0;
static unsigned long maximumAllocated = 0;

#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
static cMutex mutex;
#endif

#ifdef MEMORY_TRACKING_LOG_RETURN_TO_ZERO
static void
SetReturnToZeroFlag(char value)
{
    FILE* fp;
    fp = fopen(STATIC_STRING_FOR_FILE_IN_WORKING_DIRECTORY("mem_tracking_zero_flag.bin"), "wb");
    fwrite(&value, 1, 1, fp);
    fclose(fp);
}
#endif

void PlatformDebugBreak();

void*
operator new(size_t size)
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    size_t sizeToMalloc = size + sizeof(size_t);
    size_t* result = reinterpret_cast<size_t*>(malloc(sizeToMalloc));
    if(sizeToMalloc && !result)
    {
        PlatformDebugBreak();
    }
#ifdef MEMORY_TRACKING_LOG_RETURN_TO_ZERO
    if(totalAllocated == 0 && size)
    {
        SetReturnToZeroFlag(1);
    }
#endif
    totalAllocated += static_cast<unsigned long>(size);
    if(totalAllocated > maximumAllocated)
    {
        maximumAllocated = totalAllocated;
//        if(maximumAllocated > 400000000)
//        {
//            PlatformDebugBreak();
//        }
    }
    *result = size;
    return result + 1;
}
void
operator delete(void* ptr)
{
    if(!ptr)
    {
        return;
    }
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    size_t* ptrToSizeT = reinterpret_cast<size_t*>(ptr);
    --ptrToSizeT;
    totalAllocated -= static_cast<unsigned long>(*ptrToSizeT);
    free(ptrToSizeT);
#ifdef MEMORY_TRACKING_LOG_RETURN_TO_ZERO
    if(totalAllocated == 0)
    {
        SetReturnToZeroFlag(0);
    }
#endif
}
void*
operator new[](size_t size)
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    size_t sizeToMalloc = size + sizeof(size_t);
    size_t* result = reinterpret_cast<size_t*>(malloc(sizeToMalloc));
    if(sizeToMalloc && !result)
    {
        PlatformDebugBreak();
    }
#ifdef MEMORY_TRACKING_LOG_RETURN_TO_ZERO
    if(totalAllocated == 0 && size)
    {
        SetReturnToZeroFlag(1);
    }
#endif
    totalAllocated += static_cast<unsigned long>(size);
    if(totalAllocated > maximumAllocated)
    {
        maximumAllocated = totalAllocated;
//        if(maximumAllocated > 400000000)
//        {
//            PlatformDebugBreak();
//        }
    }
    *result = size;
    return result + 1;
}
void
operator delete[](void* ptr)
{
    if(!ptr)
    {
        return;
    }
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    size_t* ptrToSizeT = reinterpret_cast<size_t*>(ptr);
    --ptrToSizeT;
    totalAllocated -= static_cast<unsigned long>(*ptrToSizeT);
    free(ptrToSizeT);
#ifdef MEMORY_TRACKING_LOG_RETURN_TO_ZERO
    if(totalAllocated == 0)
    {
        SetReturnToZeroFlag(0);
    }
#endif
}

bool MemoryTrackingIsEnabled()
{
    return true;
}
unsigned long GetTotalMemoryAllocated()
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    return totalAllocated;
}
unsigned long GetMaximumMemoryAllocated()
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    return maximumAllocated;
}
void ResetMaximumMemoryAllocated()
{
#ifdef MEMORY_TRACKING_SUPPORT_MULTITHREADING
    cMutexLock lock(mutex);
#endif
    maximumAllocated = totalAllocated;
}

#else // memory tracking is not enabled

bool MemoryTrackingIsEnabled() {return false;}
unsigned long GetTotalMemoryAllocated() {return 0;}
unsigned long GetMaximumMemoryAllocated() {return 0;}
void ResetMaximumMemoryAllocated() {}

#endif
