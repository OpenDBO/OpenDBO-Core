/**********************************************************************

Filename    :   GAllocator.h
Content     :   Fast general purpose memory allocation functions
Created     :   December 28, 1998
Authors     :   Michael Antonov

Copyright   :   (c) 2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GALLOCATOR_H
#define INC_GALLOCATOR_H

#include "GTypes.h"


// *** Declared Classes
class   GAllocator;
class   GDebugAllocator;
struct  GAllocatorStats;


class GAllocator 
{
public:
    virtual ~GAllocator() { }


    // *** Core allocation APIs
   
    // Allocates a memory block of specified size. Memory must be aligned to
    // sizeof(void*) unless the allocation is smaller, in the later case
    // block must be aligned to the power of two of greater or equal size.
    virtual void*   Alloc(UPInt size)                       = 0;

    // Frees a memory block allocated with Alloc or Realloc.
    virtual void    Free(void *pmemBlock)                   = 0;

    // Reallocates a block allocated with Alloc, same alignment
    // requirements apply.
    virtual void*   Realloc(void *pmemBlock, UPInt newSize) = 0;    

    virtual const GAllocatorStats*   GetStats() const
    { GASSERT(0); return 0; }


    // *** Aligned Allocation APIs

    // Aligned allocation versions of allocator. Blocks allocated with
    // AllocAlligned can ONLY be freed with FreeAligned. Default implementation
    // is provided that relies on Alloc and Free by intruding internal
    // headers and padding; however, users are encouraged to provide their own
    // more efficient version.    

    // Allocate aligned memory, with optional extra header.  Total block size
    // allocated is (size + extra). Align must be zero or a power of two.
    // The address of (returned value + extra) is aligned as specified, while
    // the returned pointer may not be aligned if extra is not zero.
    // Not that if extra is specified, it must be a multiple of pointer size.
    virtual void*   AllocAligned(UPInt size, UPInt align, UPInt extra = 0);

    // Frees memory allocated by AllocAligned; the original returned value
    // must be specified.
    virtual void    FreeAligned(void *pmemBlock);
};



class GDebugAllocator
{
public:
    virtual ~GDebugAllocator() { }

    virtual void    Free(void *pmemBlock, 
                         int blocktype, const char* pfilename, 
                         int line, const char *pclassname=0);

    virtual void*   Alloc(UPInt size,
                          int blocktype, const char* pfilename, 
                          int line, const char *pclassname=0);

    virtual void*   Realloc(void *pmemBlock, UPInt newSize,
                            int blocktype, const char* pfilename, 
                            int line, const char *pclassname=0);

    // *** Aligned Allocation APIs

    // Aligned allocation - same requirements apply as outlined in GAllocator.
    
    virtual void    FreeAligned(void *pmemBlock, 
                                int blocktype, const char* pfilename, 
                                int line, const char *pclassname=0);

    virtual void*   AllocAligned(UPInt size, UPInt align, UPInt extra,
                                 int blocktype, const char* pfilename, 
                                 int line, const char *pclassname=0);
    
    virtual const GAllocatorStats*   GetStats() const;

    virtual void    Dump();

};


// Allocation statistics, tracked only if GFC_MEMORY_TRACKSIZES is enabled
struct GAllocatorStats
{
    // Allocation statistics
    UInt        AllocCount;
    UInt        FreeCount;
    UInt        ReallocCount;
    // Allocated/Released sizes
    UInt64      AllocSize;      
    UInt64      FreeSize;       
    UInt64      AllocSizeUsed;
    UInt64      FreeSizeUsed;
    // This variable is manually adjusted to take into account kernel's static allocations
    UInt64      StaticAllocSize;
    UInt64      StaticAllocSizeUsed;

    UPInt       GetSizeAllocated() const
    { return (UPInt) (AllocSize - FreeSize); }
    UPInt       GetSizeUsed() const
    { return (UPInt) (AllocSizeUsed - FreeSizeUsed); }
};

#endif // INC_GALLOCATOR_H
