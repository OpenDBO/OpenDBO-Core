/**********************************************************************

Filename    :   GAtomic.h
Content     :   Contains atomic operations and inline fastest locking
                functionality. Will contain #ifdefs for OS efficiency.
                Have non-thread-safe implementaion if not available.
Created     :   May 5, 2003
Authors     :   Michael Antonov, Andrew Reisse

Copyright   :   (c) 2003-2007 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GATOMIC_H
#define INC_GATOMIC_H

#include "GTypes.h"

// Include System thread functionality.
#if defined(GFC_OS_WIN32) || defined(GFC_OS_WINCE)
#include <windows.h>
#elif defined(GFC_OS_XBOX) || defined(GFC_OS_XBOX360)
#include <xtl.h>

#elif defined(GFC_OS_PS3)
#include <sys/synchronization.h>
#include <pthread.h>

#elif defined(GFC_OS_WII)
#include <revolution/os.h>

#elif defined(GFC_OS_SYMBIAN)

#elif defined(GFC_CC_RENESAS)

#elif defined(GFC_OS_PSP)

#else
#include <pthread.h>
#endif


// ****** Declared classes

// If there is NO thread support we implement GAtomicOps and
// GLock objects as no-ops. The other classes are not defined.
template<class C> class GAtomicOps;
template<class T> class GAtomicInt;
template<class T> class GAtomicPtr;

class   GLock;


// ***** AtomicOps

// Atomic operations are provided by the GAtomicOps templates class,
// implemented through system-specific GAtomicOpsRaw specializations.
// It provides several fundamental operations such as Exchange, ExchangeAdd
// CompareAndSet, and Store_Release. Each function includes several memory
// synchronization versions, important for multiprocessing CPUs with weak
// memory consistency. The following memory fencing strategies are supported:
//
//  - NoSync.  No memory synchronization is done for atomic op.
//  - Release. All other memory writes are completed before atomic op
//             writes its results.
//  - Acquire. Further memory reads are forced to wait until atomic op
//             executes, guaranteeing that the right values will be seen.
//  - Sync.    A combination of Release and Acquire.


// *** GAtomicOpsRaw

// GAtomicOpsRaw is a specialized template that provides atomic operations 
// used by GAtomicOps. This class has two fundamental qualities: (1) it
// defines a type T of correct size, and (2) provides operations that work
// atomically, such as Exchange_Sync and CompareAndSet_Release.

// GAtomicOpsRawBase class contains shared constants/classes for GAtomicOpsRaw.
// The primary thing is does is define sync class objects, whose destructor and
// constructor provide places to insert appropriate synchronization calls, on 
// systems where such calls are necessary. So far, the breakdown is as follows:
// 
//  - X86 systems don't need custom syncs, since their exchange/atomic
//    instructions are implicitly synchronized.
//  - PowerPC requires lwsync/isync instructions that can use this mechanism.
//  - If some other systems require a mechanism where syncing type is associated
//    with a particular instruction, the default implementation (which implements
//    all Sync, Acquire, and Release modes in terms of NoSync and fence) may not
//    work. Ii that case it will need to be #ifdef-ed conditionally.

struct GAtomicOpsRawBase
{
#if defined(GFC_NO_THREADSUPPORT) || defined(GFC_OS_WIN32) || defined(GFC_OS_XBOX)
    // Need to have empty constructor to avoid class 'unused' variable warning.
    struct FullSync { inline FullSync() { } };
    struct AcquireSync { inline AcquireSync() { } };
    struct ReleaseSync { inline ReleaseSync() { } };

#elif defined(GFC_OS_XBOX360) 
    // XBox360 CPU implements weak memory ordering and it thus needs to be synced. Unlike Win32
    // where synchronizing is implied with interlocked operations, on 360 it needs to be done explicitly.
    // For some reason 360 documentation recommends the use of '__lwsync()' for both acquire and
    // release semantics, although technically PowerPC should use 'isync' for acquire. XBox360
    // docs never mentions 'isync'. This should be investigated in more detail.
    struct AcquireSync { inline AcquireSync() { }  ~AcquireSync() { __lwsync(); } };
    struct ReleaseSync { inline ReleaseSync() { __lwsync(); }  ~ReleaseSync() { } };
    // TBD: Can we not have two sides?
    struct FullSync    { inline FullSync() { __lwsync(); }  ~FullSync() { __lwsync(); } };

#elif defined(GFC_CPU_PPC64) || defined(GFC_CPU_PPC)
    struct FullSync { inline FullSync() { asm volatile("sync\n"); } ~FullSync() { asm volatile("isync\n"); } };
    struct AcquireSync { inline AcquireSync() { } ~AcquireSync() { asm volatile("isync\n"); } };
    struct ReleaseSync { inline ReleaseSync() { asm volatile("sync\n"); } };

#elif defined(GFC_CC_GNU) && (__GNUC__ >= 4)
    // __sync functions are already full sync
    struct FullSync { inline FullSync() { } };
    struct AcquireSync { inline AcquireSync() { } };
    struct ReleaseSync { inline ReleaseSync() { } };
#endif
};


// 4-Byte raw data atomic op implementation class.
struct GAtomicOpsRaw_4ByteImpl : public GAtomicOpsRawBase
{
#if defined(GFC_NO_THREADSUPPORT)

    // Provide a type for no-thread-support cases. Used by GAtomicOpsRaw_DefImpl.
    typedef UInt32 T;   

    // *** Thread - Safe Atomic Versions.

#elif defined(GFC_OS_WIN32) || defined(GFC_OS_WINCE) || defined(GFC_OS_XBOX) || defined(GFC_OS_XBOX360)

    // Use special defined for VC6, where volatile is not used and
    // InterlockedCompareExchange is declared incorrectly.
    typedef LONG T;      
#if defined(GFC_CC_MSVC) && (GFC_CC_MSVC < 1300)
    typedef T* InterlockTPtr;
    typedef LPVOID ET;
    typedef ET* InterlockETPtr;
#else
    typedef volatile T* InterlockTPtr;
    typedef T ET;
    typedef InterlockTPtr InterlockETPtr;
#endif
    inline static T     Exchange_NoSync(volatile T* p, T val)           { return InterlockedExchange((InterlockTPtr)p, val); }
    inline static T     ExchangeAdd_NoSync(volatile T* p, T val)        { return InterlockedExchangeAdd((InterlockTPtr)p, val); }
    inline static bool  CompareAndSet_NoSync(volatile T* p, T c, T val)  { return InterlockedCompareExchange((InterlockETPtr)p, (ET)val, (ET)c) == (ET)c; }

#elif defined(GFC_CPU_PPC64) || defined(GFC_CPU_PPC)
    typedef UInt32 T;
#ifdef GFC_CC_MWERKS
    static inline UInt32   Exchange_NoSync(volatile register UInt32 *i, register UInt32 j)
    {
        register UInt32 ret;
        asm {
            @L1:    lwarx   ret,0,i
                    stwcx.  j,0,i
                    bne-    @L1
        };

        return ret;
    }

    static inline UInt32   ExchangeAdd_NoSync(volatile register UInt32 *i, register UInt32 j)
    {
        register UInt32 tmp, ret;

        asm {
            @L1:    lwarx   ret,0,i
                    add     tmp,ret,j
                    stwcx.  tmp,0,i
                    bne-    @L1
        };

        return ret;
    }

    static inline bool     CompareAndSet_NoSync(volatile register UInt32 *i, register UInt32 c, register UInt32 value)
    {
        register UInt32 ret;

        asm {
            @L1:    lwarx   ret,0,i
                    cmpw    0,ret,c
                    mfcr    ret
                    bne-    @L2
                    stwcx.  value,0,i
                    bne-    @L1
            @L2:
        };

        return (ret & 0x20000000) ? 1 : 0;
    }

#else
    static inline UInt32   Exchange_NoSync(volatile UInt32 *i, UInt32 j)
    {
        UInt32 ret;

        asm volatile("1:\n\t"
                     "lwarx  %[r],0,%[i]\n\t"
                     "stwcx. %[j],0,%[i]\n\t"
                     "bne-   1b\n"
                     : "+m" (*i), [r] "=&b" (ret) : [i] "b" (i), [j] "b" (j) : "cc");

        return ret;
    }

    static inline UInt32   ExchangeAdd_NoSync(volatile UInt32 *i, UInt32 j)
    {
        UInt32 dummy, ret;

        asm volatile("1:\n\t"
                     "lwarx  %[r],0,%[i]\n\t"
                     "add    %[o],%[r],%[j]\n\t"
                     "stwcx. %[o],0,%[i]\n\t"
                     "bne-   1b\n"
                     : "+m" (*i), [r] "=&b" (ret), [o] "=&r" (dummy) : [i] "b" (i), [j] "b" (j) : "cc");

        return ret;
    }

    static inline bool     CompareAndSet_NoSync(volatile UInt32 *i, UInt32 c, UInt32 value)
    {
        UInt32 ret;

        asm volatile("1:\n\t"
                     "lwarx  %[r],0,%[i]\n\t"
                     "cmpw   0,%[r],%[cmp]\n\t"
                     "mfcr   %[r]\n\t"
                     "bne-   2f\n\t"
                     "stwcx. %[val],0,%[i]\n\t"
                     "bne-   1b\n\t"
                     "2:\n"
                     : "+m" (*i), [r] "=&b" (ret) : [i] "b" (i), [cmp] "b" (c), [val] "b" (value) : "cc");

        return (ret & 0x20000000) ? 1 : 0;
    }

#endif
#elif defined(GFC_CC_GNU) && (__GNUC__ >= 4 && __GNUC_MINOR__ >= 1)

    typedef UInt32 T;

    static inline T   Exchange_NoSync(volatile T *i, T j)
    {
        T v;
        do {
            v = *i;
        } while (!__sync_bool_compare_and_swap(i, v, j));
        return v;
    }

    static inline T   ExchangeAdd_NoSync(volatile T *i, T j)
    {
        return __sync_fetch_and_add(i, j);
    }

    static inline bool     CompareAndSet_NoSync(volatile T *i, T c, T value)
    {
        return __sync_bool_compare_and_swap(i, c, value);
    }

#endif // OS
};


// 8-Byte raw data data atomic op implementation class.
// Currently implementation is provided only on systems with 64-bit pointers.
struct GAtomicOpsRaw_8ByteImpl : public GAtomicOpsRawBase
{    
#if !defined(GFC_64BIT_POINTERS) || defined(GFC_NO_THREADSUPPORT)

    // Provide a type for no-thread-support cases. Used by GAtomicOpsRaw_DefImpl.
    typedef UInt64 T;

    // *** Thread - Safe OS specific versions.
#elif defined(GFC_OS_WIN32)

    // This is only for 64-bit systems.
    typedef LONG64      T;
    typedef volatile T* InterlockTPtr;    
    inline static T     Exchange_NoSync(volatile T* p, T val)           { return InterlockedExchange64((InterlockTPtr)p, val); }
    inline static T     ExchangeAdd_NoSync(volatile T* p, T val)        { return InterlockedExchangeAdd64((InterlockTPtr)p, val); }
    inline static bool  CompareAndSet_NoSync(volatile T* p, T c, T val)  { return InterlockedCompareExchange64((InterlockTPtr)p, val, c) == c; }

#elif defined(GFC_CPU_PPC64)
 
    typedef UInt64 T;

    static inline UInt64   Exchange_NoSync(volatile UInt64 *i, UInt64 j)
    {
        UInt64 dummy, ret;

        asm volatile("1:\n\t"
                     "ldarx  %[r],0,%[i]\n\t"
                     "mr     %[o],%[j]\n\t"
                     "stdcx. %[o],0,%[i]\n\t"
                     "bne-   1b\n"
                     : "+m" (*i), [r] "=&b" (ret), [o] "=&r" (dummy) : [i] "b" (i), [j] "b" (j) : "cc");

        return ret;
    }

    static inline UInt64   ExchangeAdd_NoSync(volatile UInt64 *i, UInt64 j)
    {
        UInt64 dummy, ret;

        asm volatile("1:\n\t"
                     "ldarx  %[r],0,%[i]\n\t"
                     "add    %[o],%[r],%[j]\n\t"
                     "stdcx. %[o],0,%[i]\n\t"
                     "bne-   1b\n"
                     : "+m" (*i), [r] "=&b" (ret), [o] "=&r" (dummy) : [i] "b" (i), [j] "b" (j) : "cc");

        return ret;
    }

    static inline bool     CompareAndSet_NoSync(volatile UInt64 *i, UInt64 c, UInt64 value)
    {
        UInt64 ret, dummy;

        asm volatile("1:\n\t"
                     "ldarx  %[r],0,%[i]\n\t"
                     "cmpw   0,%[r],%[cmp]\n\t"
                     "mfcr   %[r]\n\t"
                     "bne-   2f\n\t"
                     "stdcx. %[val],0,%[i]\n\t"
                     "bne-   1b\n\t"
                     "2:\n"
                     : "+m" (*i), [r] "=&b" (ret), [o] "=&r" (dummy) : [i] "b" (i), [cmp] "b" (c), [val] "b" (value) : "cc");

        return (ret & 0x20000000) ? 1 : 0;
    }

#elif defined(GFC_CC_GNU) && (__GNUC__ >= 4 && __GNUC_MINOR__ >= 1)

    typedef UInt64 T;

    static inline T   Exchange_NoSync(volatile T *i, T j)
    {
        T v;
        do {
            v = *i;
        } while (!__sync_bool_compare_and_swap(i, v, j));
        return v;
    }

    static inline T   ExchangeAdd_NoSync(volatile T *i, T j)
    {
        return __sync_fetch_and_add(i, j);
    }

    static inline bool     CompareAndSet_NoSync(volatile T *i, T c, T value)
    {
        return __sync_bool_compare_and_swap(i, c, value);
    }

#endif // OS
};


// Default implementation for GAtomicOpsRaw; provides implementation of mem-fenced
// atomic operations where fencing is done with a sync object wrapped around a NoSync
// operation implemented in the base class. If such implementation is not possible
// on a given platform, #ifdefs can be used to disable it and then op functions can be
// implemented individually in the appropriate GAtomicOpsRaw<size> class.

template<class O>
struct GAtomicOpsRaw_DefImpl : public O
{
    typedef typename O::T O_T;
    typedef typename O::FullSync    O_FullSync;
    typedef typename O::AcquireSync O_AcquireSync;
    typedef typename O::ReleaseSync O_ReleaseSync;

    // If there is no thread support, provide the default implementation. In this case,
    // the base class (0) must still provide the T declaration.
#ifdef GFC_NO_THREADSUPPORT   

    // Atomic exchange of val with argument. Returns old val.
    inline static O_T   Exchange_NoSync(volatile O_T* p, O_T val)           { O_T old = *p; *p = val; return old; }
    // Adds a new val to argument; returns its old val.
    inline static O_T   ExchangeAdd_NoSync(volatile O_T* p, O_T val)        { O_T old = *p; *p += val; return old; }
    // Compares the argument data with 'c' val.
    // If succeeded, stores val int '*p' and returns true; otherwise returns false.
    inline static bool  CompareAndSet_NoSync(volatile O_T* p, O_T c, O_T val) { if (*p==c) { *p = val; return 1; } return 0; }

#endif

    // If NoSync wrapped implementation may not be possible, it this block should be
    //  replaced with per-function implementation in O.
    inline static O_T   Exchange_Sync(volatile O_T* p, O_T val)             { O_FullSync sync;    return Exchange_NoSync(p, val); }
    inline static O_T   Exchange_Release(volatile O_T* p, O_T val)          { O_ReleaseSync sync; return Exchange_NoSync(p, val); }
    inline static O_T   Exchange_Acquire(volatile O_T* p, O_T val)          { O_AcquireSync sync; return Exchange_NoSync(p, val); }  
    inline static O_T   ExchangeAdd_Sync(volatile O_T* p, O_T val)          { O_FullSync sync;    return ExchangeAdd_NoSync(p, val); }
    inline static O_T   ExchangeAdd_Release(volatile O_T* p, O_T val)       { O_ReleaseSync sync; return ExchangeAdd_NoSync(p, val); }
    inline static O_T   ExchangeAdd_Acquire(volatile O_T* p, O_T val)       { O_AcquireSync sync; return ExchangeAdd_NoSync(p, val); }
    inline static bool  CompareAndSet_Sync(volatile O_T* p, O_T c, O_T val)    { O_FullSync sync;    return CompareAndSet_NoSync(p,c,val); }
    inline static bool  CompareAndSet_Release(volatile O_T* p, O_T c, O_T val) { O_ReleaseSync sync; return CompareAndSet_NoSync(p,c,val); }
    inline static bool  CompareAndSet_Acquire(volatile O_T* p, O_T c, O_T val) { O_AcquireSync sync; return CompareAndSet_NoSync(p,c,val); }

    // Loads and stores with memory fence. These have only the relevant versions.
#ifdef GFC_CPU_X86
    // On X86, Store_Release is implemented as exchange. Note that we can also
    // consider 'sfence' in the future, although it is not as compatible with older CPUs.
    inline static void  Store_Release(volatile O_T* p, O_T val)             { Exchange_Release(p, val); }
#else
    inline static void  Store_Release(volatile O_T* p, O_T val)             { O_ReleaseSync sync; *p = val; }       
#endif
    inline static O_T     Load_Acquire(volatile O_T* p)                     { O_AcquireSync sync; return *p; }
};


template<int size>
struct GAtomicOpsRaw : public GAtomicOpsRawBase { };

template<>
struct GAtomicOpsRaw<4> : public GAtomicOpsRaw_DefImpl<GAtomicOpsRaw_4ByteImpl>
{   
    // Ensure that assigned type size is correct.
    GAtomicOpsRaw()
    { GCOMPILER_ASSERT(sizeof(GAtomicOpsRaw_DefImpl<GAtomicOpsRaw_4ByteImpl>::T) == 4); }
};
template<>
struct GAtomicOpsRaw<8> : public GAtomicOpsRaw_DefImpl<GAtomicOpsRaw_8ByteImpl>
{
    GAtomicOpsRaw()
    { GCOMPILER_ASSERT(sizeof(GAtomicOpsRaw_DefImpl<GAtomicOpsRaw_8ByteImpl>::T) == 8); }
};


// *** GAtomicOps - implementation of atomic Ops for specified class

// Implements atomic ops on a class, provided that the object is either
// 4 or 8 bytes in size (depending on the GAtomicOpsRaw specializations
// available). Relies on GAtomicOpsRaw for much of implementation.

template<class C>
class GAtomicOps
{
    typedef GAtomicOpsRaw<sizeof(C)>      Ops;
    typedef typename Ops::T               T;
    typedef volatile typename Ops::T*     PT;
    // We cast through unions to (1) avoid pointer size compiler warnings
    // and (2) ensure that there are no problems with strict pointer aliasing.
    union C2T_union { C c; T t; };

public:
    // General purpose implementation for standard syncs.    
    inline static C     Exchange_Sync(volatile C* p, C val)             { C2T_union u; u.c = val; u.t = Ops::Exchange_Sync((PT)p, u.t); return u.c; }
    inline static C     Exchange_Release(volatile C* p, C val)          { C2T_union u; u.c = val; u.t = Ops::Exchange_Release((PT)p, u.t); return u.c; }
    inline static C     Exchange_Acquire(volatile C* p, C val)          { C2T_union u; u.c = val; u.t = Ops::Exchange_Acquire((PT)p, u.t); return u.c; }
    inline static C     Exchange_NoSync(volatile C* p, C val)           { C2T_union u; u.c = val; u.t = Ops::Exchange_NoSync((PT)p, u.t); return u.c; }
    inline static C     ExchangeAdd_Sync(volatile C* p, C val)          { C2T_union u; u.c = val; u.t = Ops::ExchangeAdd_Sync((PT)p, u.t); return u.c; }
    inline static C     ExchangeAdd_Release(volatile C* p, C val)       { C2T_union u; u.c = val; u.t = Ops::ExchangeAdd_Release((PT)p, u.t); return u.c; }
    inline static C     ExchangeAdd_Acquire(volatile C* p, C val)       { C2T_union u; u.c = val; u.t = Ops::ExchangeAdd_Acquire((PT)p, u.t); return u.c; }
    inline static C     ExchangeAdd_NoSync(volatile C* p, C val)        { C2T_union u; u.c = val; u.t = Ops::ExchangeAdd_NoSync((PT)p, u.t); return u.c; }
    inline static bool  CompareAndSet_Sync(volatile C* p, C c, C val)   { C2T_union u,cu; u.c = val; cu.c = c; return Ops::CompareAndSet_Sync((PT)p, cu.t, u.t); }
    inline static bool  CompareAndSet_Release(volatile C* p, C c, C val){ C2T_union u,cu; u.c = val; cu.c = c; return Ops::CompareAndSet_Release((PT)p, cu.t, u.t); }
    inline static bool  CompareAndSet_Acquire(volatile C* p, C c, C val){ C2T_union u,cu; u.c = val; cu.c = c; return Ops::CompareAndSet_Acquire((PT)p, cu.t, u.t); }
    inline static bool  CompareAndSet_NoSync(volatile C* p, C c, C val) { C2T_union u,cu; u.c = val; cu.c = c; return Ops::CompareAndSet_NoSync((PT)p, cu.t, u.t); }
    // Loads and stores with memory fence. These have only the relevant versions.    
    inline static void  Store_Release(volatile C* p, C val)             { C2T_union u; u.c = val; Ops::Store_Release((PT)p, u.t); }    
    inline static C     Load_Acquire(volatile C* p)                     { C2T_union u; u.t = Ops::Load_Acquire((PT)p); return u.c; }
};



// Atomic value base class - implements operations shared for integers and pointers.
template<class T>
class GAtomicValueBase
{
protected:
    typedef GAtomicOps<T> Ops;
public:

    volatile T  Value;

    inline GAtomicValueBase()                  { }
    explicit inline GAtomicValueBase(T val)    { Ops::Store_Release(&Value, val); }

    // Most libraries (TBB and Joshua Scholar's) library do not do Load_Acquire
    // here, since most algorithms do not require atomic loads. Needs some research.    
    inline operator T() const { return Value; }

    // *** Standard Atomic inlines
    inline T     Exchange_Sync(T val)               { return Ops::Exchange_Sync(&Value,  val); }
    inline T     Exchange_Release(T val)            { return Ops::Exchange_Release(&Value, val); }
    inline T     Exchange_Acquire(T val)            { return Ops::Exchange_Acquire(&Value, val); }
    inline T     Exchange_NoSync(T val)             { return Ops::Exchange_NoSync(&Value, val); }
    inline bool  CompareAndSet_Sync(T c, T val)     { return Ops::CompareAndSet_Sync(&Value, c, val); }
    inline bool  CompareAndSet_Release(T c, T val)  { return Ops::CompareAndSet_Release(&Value, c, val); }
    inline bool  CompareAndSet_Acquire(T c, T val)  { return Ops::CompareAndSet_Acquire(&Value, c, val); }
    inline bool  CompareAndSet_NoSync(T c, T val)   { return Ops::CompareAndSet_NoSync(&Value, c, val); }
    // Load & Store.
    inline void  Store_Release(T val)               { Ops::Store_Release(&Value, val); }
    inline T     Load_Acquire()                     { return Ops::Load_Acquire(&Value);  }
};


// ***** GAtomicPtr - Atomic pointer template

// This pointer class supports atomic assignments with release,
// increment / decrement operations, and conditional compare + set.

template<class T>
class GAtomicPtr : public GAtomicValueBase<T*>
{
    typedef typename GAtomicValueBase<T*>::Ops Ops;

public:
    // Initialize pointer value to 0 by default; use Store_Release only with explicit constructor.
    inline GAtomicPtr() : GAtomicValueBase<T*>()                     { this->Value = 0; }
    explicit inline GAtomicPtr(T* val) : GAtomicValueBase<T*>(val)   { }
        
    // Pointer access.
    inline T* operator -> () const     { return this->Value; }

    // *** Standard Atomic inlines (applicable to pointers)

    // ExhangeAdd considers pointer size for pointers.
    template<class I>
    inline T*     ExchangeAdd_Sync(I incr)      { return Ops::ExchangeAdd_Sync(&this->Value, ((T*)0) + incr); }
    template<class I>
    inline T*     ExchangeAdd_Release(I incr)   { return Ops::ExchangeAdd_Release(&this->Value, ((T*)0) + incr); }
    template<class I>
    inline T*     ExchangeAdd_Acquire(I incr)   { return Ops::ExchangeAdd_Acquire(&this->Value, ((T*)0) + incr); }
    template<class I>
    inline T*     ExchangeAdd_NoSync(I incr)    { return Ops::ExchangeAdd_NoSync(&this->Value, ((T*)0) + incr); }

    // *** Atomic Operators

    inline T* operator = (T* val)  { Store_Release(val); return val; }

    template<class I>
    inline T* operator += (I val) { return ExchangeAdd_Sync(val) + val; }
    template<class I>
    inline T* operator -= (I val) { return operator += (-val); }

    inline T* operator ++ ()      { return ExchangeAdd_Sync(1) + 1; }
    inline T* operator -- ()      { return ExchangeAdd_Sync(-1) - 1; }
    inline T* operator ++ (int)   { return ExchangeAdd_Sync(1); }
    inline T* operator -- (int)   { return ExchangeAdd_Sync(-1); }
};


// ***** GAtomicInt - Atomic integer template

// Implements an atomic integer type; the exact type to use is provided 
// as an argument. Supports atomic Acquire / Release semantics, atomic
// arithmetic operations, and atomic conditional compare + set.

template<class T>
class GAtomicInt : public GAtomicValueBase<T>
{
    typedef typename GAtomicValueBase<T>::Ops Ops;

public:
    inline GAtomicInt() : GAtomicValueBase<T>()                     { }
    explicit inline GAtomicInt(T val) : GAtomicValueBase<T>(val)    { }


    // *** Standard Atomic inlines (applicable to int)   
    inline T     ExchangeAdd_Sync(T val)            { return Ops::ExchangeAdd_Sync(&this->Value, val); }
    inline T     ExchangeAdd_Release(T val)         { return Ops::ExchangeAdd_Release(&this->Value, val); }
    inline T     ExchangeAdd_Acquire(T val)         { return Ops::ExchangeAdd_Acquire(&this->Value, val); }
    inline T     ExchangeAdd_NoSync(T val)          { return Ops::ExchangeAdd_NoSync(&this->Value, val); }
    // These increments could be more efficient because they don't return a value.
    inline void  Increment_Sync()                   { ExchangeAdd_Sync((T)1); }
    inline void  Increment_Release()                { ExchangeAdd_Release((T)1); }
    inline void  Increment_Acquire()                { ExchangeAdd_Acquire((T)1); }    
    inline void  Increment_NoSync()                 { ExchangeAdd_NoSync((T)1); }

    // *** Atomic Operators

    inline T operator = (T val)  { Store_Release(val); return val; }
    inline T operator += (T val) { return ExchangeAdd_Sync(val) + val; }
    inline T operator -= (T val) { return ExchangeAdd_Sync(0 - val) - val; }

    inline T operator ++ ()      { return ExchangeAdd_Sync((T)1) + 1; }
    inline T operator -- ()      { return ExchangeAdd_Sync(((T)0)-1) - 1; }
    inline T operator ++ (int)   { return ExchangeAdd_Sync((T)1); }
    inline T operator -- (int)   { return ExchangeAdd_Sync(((T)0)-1); }

    // More complex atomic operations. Leave it to compiler whether to optimize them or not.
    T operator &= (T arg)
    {
        T comp, newVal;
        do {
            comp   = this->Value;
            newVal = comp & arg;
        } while(!CompareAndSet_Sync(comp, newVal));
        return newVal;
    }

    T operator |= (T arg)
    {
        T comp, newVal;
        do {
            comp   = this->Value;
            newVal = comp | arg;
        } while(!CompareAndSet_Sync(comp, newVal));
        return newVal;
    }

    T operator ^= (T arg)
    {
        T comp, newVal;
        do {
            comp   = this->Value;
            newVal = comp ^ arg;
        } while(!CompareAndSet_Sync(comp, newVal));
        return newVal;
    }

    T operator *= (T arg)
    {
        T comp, newVal;
        do {
            comp   = this->Value;
            newVal = comp * arg;
        } while(!CompareAndSet_Sync(comp, newVal));
        return newVal;
    }

    T operator /= (T arg)
    {
        T comp, newVal;
        do {
            comp   = this->Value;
            newVal = comp / arg;
        } while(!CompareAndSet_Sync(comp, newVal));
        return newVal;
    }

    T operator >>= (unsigned bits)
    {
        T comp, newVal;
        do {
            comp   = this->Value;
            newVal = comp >> bits;
        } while(!CompareAndSet_Sync(comp, newVal));
        return newVal;
    }

    T operator <<= (unsigned bits)
    {
        T comp, newVal;
        do {
            comp   = this->Value;
            newVal = comp << bits;
        } while(!CompareAndSet_Sync(comp, newVal));
        return newVal;
    }
};




// ***** GLock

// GLock is a simplest and most efficient mutial-exclusion lock class.
// Unlike GMutex, it cannot be waited on.

class GLock
{
    // NOTE: Locks are not allocatable and they themselves should not allocate 
    // memory by standard means. This is the case because GStandardAllocator
    // relies on this class.
    // Make 'delete' private. Don't do this for 'new' since it can be redefined.  
    void    operator delete(void*) {}


    // *** Lock implementation for various platforms.
    
#if defined(GFC_NO_THREADSUPPORT)

public:
    // With no thread support, lock does nothing.
    inline GLock() { }
    inline GLock(UInt) { }
    inline ~GLock() { }    
    inline void Lock() { }
    inline void Unlock() { }

   // Windows.
#elif defined(GFC_OS_WIN32) && (defined(GFC_CPU_X86) || defined(GFC_CPU_X86_64))

    // Optimized Win32 CriticalSection similar to code provided by
    // "Fast critical sections with timeout" by Vladislav Gelfer on code project.
    // Promises 2x lock/unlock performance improvement.
    #define GFC_FAST_GLOCK
    
    GAtomicInt<DWORD>   LockedThreadId;
    GAtomicInt<int>     WaiterCount;
    volatile HANDLE     hSemaphore;

    UInt                SpinMax;
    UInt                RecursiveLockCount;

    inline bool PerfLockImmediate(DWORD threadId)  { return LockedThreadId.CompareAndSet_Acquire(0, threadId); }
    inline void WaiterPlus()                       { WaiterCount.ExchangeAdd_NoSync(1); }
    inline void WaiterMinus()                      { WaiterCount.ExchangeAdd_NoSync(-1); }

    void        PerfLock(DWORD threadId);
    void        PerfUnlock();
    void        AllocateKernelSemaphore();
    void        SetSpinMax(UInt maxCount);

public:
    GLock(UInt spinCount = 0);
    ~GLock();
    // Locking functions.
    void Lock();
    void Unlock();

    
#elif defined(GFC_OS_WINCE) || defined(GFC_OS_XBOX) || defined(GFC_OS_XBOX360)

    CRITICAL_SECTION cs;
public:   
    GEXPORT GLock(UInt spinCount = 0);      
    GEXPORT ~GLock();
    // Locking functions.
    inline void Lock()      { ::EnterCriticalSection(&cs); }
    inline void Unlock()    { ::LeaveCriticalSection(&cs); }

#elif defined(GFC_OS_PS3)

    UByte          mutex[sizeof(sys_lwmutex_t) + 4] __attribute__((aligned(4)));
    sys_lwmutex_t* pmutex;
    static sys_lwmutex_attribute_t LockAttr;

public:
    GLock (UInt dummy = 0)
    {
        pmutex =  (sys_lwmutex_t *) (UPInt(&mutex) & 4 ? mutex+4 : mutex);
        sys_lwmutex_create(pmutex,&LockAttr);
    }
    ~GLock ()               { sys_lwmutex_destroy(pmutex); }
    inline void Lock()      { sys_lwmutex_lock(pmutex,SYS_NO_TIMEOUT); }
    inline void Unlock()    { sys_lwmutex_unlock(pmutex); }

#elif defined(GFC_OS_WII)
    OSMutex     mutex;
public:
    GLock (UInt dummy = 0)  { OSInitMutex(&mutex); }
    inline void Lock()      { OSLockMutex(&mutex); }
    inline void Unlock()    { OSUnlockMutex(&mutex); }

#else

    pthread_mutex_t mutex;
public:
    GLock (UInt dummy = 0)  { pthread_mutex_init(&mutex,0); }
    ~GLock ()               { pthread_mutex_destroy(&mutex); }
    inline void Lock()      { pthread_mutex_lock(&mutex); }
    inline void Unlock()    { pthread_mutex_unlock(&mutex); }

#endif // GFC_NO_THREADSUPPORT


public:
    // Locker class, used for automatic locking
    class Locker
    {
    public:     
        GLock *pLock;
        inline Locker(GLock *plock)
        { pLock = plock; pLock->Lock(); }
        inline ~Locker()
        { pLock->Unlock();  }
    };
};


#endif // INC_GATOMIC_H
