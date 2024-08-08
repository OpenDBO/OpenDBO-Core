/**********************************************************************

Filename    :   GMemory.h
Content     :   Fast general purpose memory allocation functions
Created     :   December 28, 1998
Authors     :   Michael Antonov

Notes       :   Since static variables are used will not work
                with multithreading
                First allocation might take extended time
                because of preallocation ahead                  

History     :   1/14/1999 MA    Revised file according to new format
                2/12/2002 MA    Major revision to utilize a buddy system

Copyright   :   (c) 1998-2003 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GMEMORY_H
#define INC_GMEMORY_H

#include "GTypes.h"
#include "GAllocator.h"

// (HVS) Needed for placement new - NSA 8/11/2006
#ifdef GFC_OS_PSP
#include <new>
#endif

// **** Declared Classes
class GMemory;


// Define this macro to enable memory tracking; since tracking is not free
// this variable probably should not be defined for a release build.
//#define   GFC_MEMORY_TRACKSIZES

#ifdef GFC_BUILD_DEFINE_NEW
// Leave 'new' definition the way it was by the user, unless undefined.
#ifndef GFC_DEFINE_NEW
#define GFC_DEFINE_NEW new(__FILE__,__LINE__)
#endif
#endif

// Undefine new if necessary
#if defined(GFC_BUILD_DEFINE_NEW) && defined(GFC_DEFINE_NEW)
#undef new
#endif

// *******  Allocation Functions
/*
#if defined(BUILD_GKERNEL) || defined(GFC_BUILD_INCLUDE_GKERNEL)
#define GKERNEL_EXPORT GEXPORT
#else
#define GKERNEL_EXPORT GIMPORT
#endif
*/
#define GKERNEL_EXPORT

// *** Memory macros


#ifndef GFC_BUILD_DEBUG

#define GALLOC(s)                   GMemory::Alloc((s))
#define GALLOC_CLASS(s,c)           GMemory::Alloc((s))
#define GREALLOC(p,s)               GMemory::Realloc((p),(s))
#define GREALLOC_CLASS(p,s)         GMemory::Realloc((p),(s))
#define GFREE(p)                    GMemory::Free((p))
#define GFREE_CLASS(p,c)            GMemory::Free((p))
// Memory allocated with GALLOC_ALIGN *must* be freed with GFREE_ALIGN
#define GALLOC_ALIGN(s,a)           GMemory::AllocAligned((s),(a))
#define GFREE_ALIGN(s)              GMemory::FreeAligned((s))

#else                           
                                
#define GALLOC(s)                   GMemory::DebugAlloc((s),1,__FILE__,__LINE__)
#define GALLOC_CLASS(s,c)           GMemory::DebugAlloc((s),1,__FILE__,__LINE__,(#c))
#define GREALLOC(p,s)               GMemory::DebugRealloc((p),(s),1,__FILE__,__LINE__)
#define GREALLOC_CLASS(p,s,c)       GMemory::DebugRealloc((p),(s),1,__FILE__,__LINE__,(#c))
#define GFREE(p)                    GMemory::DebugFree((p),1,__FILE__,__LINE__)
#define GFREE_CLASS(p,c)            GMemory::DebugFree((p),1,__FILE__,__LINE__,(#c))
// Memory allocated with GALLOC_ALIGN *must* be freed with GFREE_ALIGN
#define GALLOC_ALIGN(s,a)           GMemory::DebugAllocAligned((s),(a),0,1,__FILE__,__LINE__)
#define GFREE_ALIGN(p)              GMemory::DebugFreeAligned((p),1,__FILE__,__LINE__)

#endif //!defined(GFC_BUILD_DEBUG)


class GMemory
{
public:

    // Overrides every individual allocation
    // - avoids using the default memory system
    GKERNEL_EXPORT  static void             GSTDCALL  SetAllocator(GAllocator *pallocator);
    GKERNEL_EXPORT  static GAllocator*      GSTDCALL  GetAllocator();

    GINLINE         static const GAllocatorStats*     GetAllocatorStats()
    {
        return GetAllocator()->GetStats();
    }
    
    // Optional: overrides every individual allocation in Debug mode
    // - by default, the debug calls fallback on the normal allocator
    // - avoids using the default memory system
    GKERNEL_EXPORT  static void             GSTDCALL  SetDebugAllocator(GDebugAllocator *pallocator);
    GKERNEL_EXPORT  static GDebugAllocator* GSTDCALL  GetDebugAllocator();
    

    // *** Standard Allocator

    // Overrides only the block allocation made by the default memory system
    // - will not be used if the 
    GKERNEL_EXPORT  static void             GSTDCALL  SetBlockAllocator(GAllocator *pallocator);
    GKERNEL_EXPORT  static GAllocator*      GSTDCALL  GetBlockAllocator();



    // *** Memory allocation

    // Allocate memory
    // GMemory::Alloc of size==0 will allocate a tiny block & return a valid pointer
    // This makes it suitable for new operator
    GKERNEL_EXPORT  static void* GSTDCALL   Alloc(UPInt size);
    // Reallocate to a new size, 0 return == can't reallocate
    //                           on fail, previous memory is still valid
    // Realloc to decrease size will never fail
    // Realloc of pointer == 0 is equivalent to GMemory::Alloc
    // Realloc of size == 0, is equivalent to GMemory::Free & will return 0
    GKERNEL_EXPORT  static void* GSTDCALL   Realloc(void *pmemBlock, UPInt newSize);
    // Free allocated/reallocated block
    GKERNEL_EXPORT  static void  GSTDCALL   Free(void *pmemBlock);
   
    // Aligned versions.
    // FreeAligned must be called for any memory allocated with AllocAligned.    
    GKERNEL_EXPORT  static void* GSTDCALL   AllocAligned(UPInt size, UPInt align = 0, UPInt extra = 0);    
    GKERNEL_EXPORT  static void  GSTDCALL   FreeAligned(void *pmemBlock);


    // *** Debug Memory allocation

#if defined(GFC_BUILD_DEBUG)
    // Debug Memory tracking allocation
    GKERNEL_EXPORT  static void*    GSTDCALL DebugAlloc(UPInt size, int blocktype, const char* pfilename, int line, const char *pclassname=0);
    GKERNEL_EXPORT  static void*    GSTDCALL DebugRealloc(void *pmemBlock, UPInt newSize, int blocktype, const char* pfilename, int line, const char *pclassname=0);
    GKERNEL_EXPORT  static void     GSTDCALL DebugFree(void *pmemBlock, int blocktype, const char* pfilename, int line, const char *pclassname=0);  
    // Aligned versions.
    GKERNEL_EXPORT  static void*    GSTDCALL DebugAllocAligned(UPInt size, UPInt align, UPInt extra, int blocktype, const char* pfilename, int line, const char *pclassname=0);    
    GKERNEL_EXPORT  static void     GSTDCALL DebugFreeAligned(void *pmemBlock, int blocktype, const char* pfilename, int line, const char *pclassname=0);  

    GKERNEL_EXPORT  static void     GSTDCALL DetectMemoryLeaks();
#else

    GINLINE         static void*    DebugAlloc(UPInt size, int blocktype, const char* pfilename, int line, const char *pclassname=0)
        { return GMemory::Alloc(size); GUNUSED4(blocktype, pfilename, line, pclassname); }
    GINLINE         static void*    DebugRealloc(void *pmemBlock, UPInt newSize, int blocktype, const char* pfilename, int line, const char *pclassname=0)
        { return GMemory::Realloc(pmemBlock, newSize); GUNUSED4(blocktype, pfilename, line, pclassname); }
    GINLINE         static void     DebugFree(void *pmemBlock, int blocktype, const char* pfilename, int line, const char *pclassname=0)
        { GMemory::Free(pmemBlock); return; GUNUSED4(blocktype, pfilename, line, pclassname); }
    // Aligned versions.
    GINLINE         static void*    DebugAllocAligned(UPInt size, UPInt align, UPInt extra, int blocktype, const char* pfilename, int line, const char *pclassname=0)
        { return GMemory::AllocAligned(size, align, extra); GUNUSED4(blocktype, pfilename, line, pclassname); }
    GINLINE         static void     DebugFreeAligned(void *pmemBlock, int blocktype, const char* pfilename, int line, const char *pclassname=0)
        { GMemory::FreeAligned(pmemBlock); return; GUNUSED4(blocktype, pfilename, line, pclassname); }

    GINLINE         static void     DetectMemoryLeaks()
        { }
#endif

};



// ******* Global allocation overrides

#ifdef GFC_BUILD_NEW_OVERRIDE

// Override the new operator if it is desired.
// Global operator new override may be enabled with the GFC_BUILD_NEW_OVERRIDE option,
// however, this can cause problems when linking with other 3rd party C++ libraries.

GINLINE void*   operator new        (size_t n)  
    { return GMemory::Alloc(n); }
GINLINE void*   operator new[]      (size_t n)  
    { return GMemory::Alloc(n); }
GINLINE void    operator delete     (void *ptr) 
    { GMemory::Free(ptr); }
GINLINE void    operator delete[]   (void *ptr) 
    { GMemory::Free(ptr); }

GINLINE void*   operator new        (size_t n, const char* pfilename, int line) 
    { return GMemory::DebugAlloc(n, 1, pfilename, line); }
GINLINE void*   operator new[]      (size_t n, const char* pfilename, int line) 
    { return GMemory::DebugAlloc(n, 1, pfilename, line); }
GINLINE void    operator delete     (void *ptr, const char* pfilename, int line)    
    { GMemory::DebugFree(ptr, 1, pfilename, line); }
GINLINE void    operator delete[]   (void *ptr, const char* pfilename, int line)    
    { GMemory::DebugFree(ptr, 1, pfilename, line); }

GINLINE void*   operator new        (size_t n, int blocktype, const char* pfilename, int line)  
    { return GMemory::DebugAlloc(n, blocktype, pfilename, line); }
GINLINE void*   operator new[]      (size_t n, int blocktype, const char* pfilename, int line)  
    { return GMemory::DebugAlloc(n, blocktype, pfilename, line); }
GINLINE void    operator delete     (void *ptr, int blocktype, const char* pfilename, int line) 
    { GMemory::DebugFree(ptr, blocktype, pfilename, line); }
GINLINE void    operator delete[]   (void *ptr, int blocktype, const char* pfilename, int line) 
    { GMemory::DebugFree(ptr, blocktype, pfilename, line); }

#endif

// Calls constructor on own memory created with "new(ptr) type"
#ifndef __PLACEMENT_NEW_INLINE
#define __PLACEMENT_NEW_INLINE

#if defined(GFC_CC_MSVC) || defined(GFC_OS_SYMBIAN)
GINLINE void*   operator new        (size_t n, void *ptr)   { return ptr; GUNUSED(n); }
GINLINE void    operator delete     (void *ptr, void *ptr2) { return; GUNUSED2(ptr,ptr2); }
#else
#include <new>
#endif

#endif // __PLACEMENT_NEW_INLINE


// ***** Macros to redefine class new/delete operators

#define GFC_MEMORY_REDEFINE_NEW_IMPL(class_name, init_mem)                                          \
    void*   operator new(size_t sz)                                                                 \
    { void *p = GALLOC(sz); init_mem(class_name, p); return p; }                                    \
    void    operator delete(void *p)                                                                \
    { GFREE(p); }                                                                                   \
    void*   operator new(size_t sz, const char* pfilename, int line)                                \
    { void* p = GMemory::DebugAlloc(sz, 1, pfilename, line); init_mem(class_name, p); return p; }   \
    void    operator delete(void *p, const char* pfilename, int line)                               \
    { GMemory::DebugFree(p, 1, pfilename, line); }                                                  \
    void*   operator new(size_t sz, int blocktype, const char* pfilename, int line)                 \
    { void* p = GMemory::DebugAlloc(sz, blocktype, pfilename, line); init_mem(class_name, p); return p; }   \
    void    operator delete(void *p, int blocktype, const char* pfilename, int line)                \
    { GMemory::DebugFree(p, blocktype, pfilename, line); }

#define GFC_MEMORY_INIT_MEM_NONE(class_name, p)

// Redefined all delete/new operators in a class without custom memory initialization
#define GFC_MEMORY_REDEFINE_NEW(class_name) GFC_MEMORY_REDEFINE_NEW_IMPL(class_name, GFC_MEMORY_INIT_MEM_NONE)


// Base class that overrides the new and delete operators.
// Deriving from this class, even as a multiple base, incurs no space overhead.
class GNewOverrideBase
{
public:
    // Redefine all new & delete operators.
    GFC_MEMORY_REDEFINE_NEW(GNewOverrideBase)
};

#if defined(GFC_BUILD_DEFINE_NEW) && defined(GFC_DEFINE_NEW)
#define new GFC_DEFINE_NEW
#endif

#endif // INC_GMEMORY_H
