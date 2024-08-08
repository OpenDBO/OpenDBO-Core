/**********************************************************************

Filename    :   GTypes.h
Content     :   Standard library defines and simple types
Created     :   June 28, 1998
Authors     :   Michael Antonov, Brendan Iribe

Notes       :   This file represents all the standard types
                to be used. These types are defined for compiler
                independence from size of types.

Copyright   :   (c) 1998-2007 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GTYPES_H
#define INC_GTYPES_H

// GFC Fx Version(s)
#define GFC_FX_MAJOR_VERSION        2
#define GFC_FX_MINOR_VERSION        0
#define GFC_FX_BUILD_VERSION        48

#define GFC_FX_VERSION_STRING       "2.0.48"


// **** Operating System

/* GFC works on the following operating systems: (GFC_OS_x)

     WIN32    - Win32 (Windows 95/98/ME and Windows NT/2000/XP)
     WINCE    - WinCE (Windows CE)
     XBOX     - Xbox console
     XBOX360  - Xbox 360 console
     PSP      - Playstation Portable
     PS2      - Playstation 2 console
     PS3      - Playstation 3 console
     GAMECUBE - GameCube console
     WII      - Wii console
     QNX      - QNX OS
     DARWIN   - Darwin OS (Mac OS X)
     LINUX    - Linux
     SYMBIAN  - Symbian 9.1
*/

#if (defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))) || defined(__MACOS__)
# define GFC_OS_DARWIN
# define GFC_OS_MAC
#elif defined(_XBOX)
// Xbox360 and XBox both share _XBOX definition
#if (_XBOX_VER >= 200)
# define GFC_OS_XBOX360
#else
# define GFC_OS_XBOX
#endif
#elif defined(_WIN32_WCE)
# define GFC_OS_WINCE
#elif (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
# define GFC_OS_WIN32
#elif (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
# define GFC_OS_WIN32
#elif defined(__MWERKS__) && defined(__INTEL__)
# define GFC_OS_WIN32
#elif defined(__linux__) || defined(__linux)
# define GFC_OS_LINUX
#elif (defined(_PSP) || defined(__psp__))
# define GFC_OS_PSP
#elif (defined(_EE) && defined(_MIPSEL)) || (defined(__mips__) && defined(__R5900__)) || defined(__MIPS_PSX2__)
# define GFC_OS_PS2
#elif defined(__PPU__)
# define GFC_OS_PS3
#elif defined(RVL)
# define GFC_OS_WII
#elif defined(GEKKO)
# define GFC_OS_GAMECUBE
#elif defined (__QNX__)
# define GFC_OS_QNX
#elif defined (__symbian__)
# define GFC_OS_SYMBIAN
#else
# define GFC_OS_OTHER
#endif

// **** CPU Architecture

/* GFC supports the following CPUs: (GFC_CPU_x)

     X86        - x86 (IA-32)
     X86_64     - x86_64 (amd64)
     PPC64      - PowerPC64
     OTHER      - CPU for which no special support is present or needed
*/

#if defined(__x86_64__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#  define GFC_CPU_X86_64
#  define GFC_64BIT_POINTERS
#elif defined(__i386__) || defined(GFC_OS_WIN32) || defined(GFC_OS_XBOX)
#  define GFC_CPU_X86
#elif defined(__powerpc64__) || defined(GFC_OS_PS3) || defined(GFC_OS_XBOX360) || defined(GFC_OS_WII)
#  define GFC_CPU_PPC64
// Note: PS3, x360 and WII don't use 64-bit pointers.
#elif defined(__ppc__)
#  define GFC_CPU_PPC
#else
#  define GFC_CPU_OTHER
#endif


// Include conditional compilation options file
// Needs to be after OS detection code
#include "GConfig.h"

// **** Windowing System

/* GFC supports the following windowing systems: (GFC_WS_x)

     WIN32      - Windows
     WINDOTNET  - Windows.NET
     MACX       - Mac OS X
     MAC9       - Mac OS 9
     X11        - X Window System
*/

#if defined(_WIN32_X11_)
# define GFC_WS_X11

#elif defined(GFC_OS_WIN32)
# define GFC_WS_WIN32

//#define WIN32_LEAN_AND_MEAN
// Windows Standard Includes must come before STL definitions
# if defined(GFC_BUILD_MFC)
#  include <afx.h>
# elif !defined(_WINDOWS_)
// GFx does not include windows.h for all files
//#  include <windows.h>
#endif

#elif defined(GFC_OS_XBOX) || defined(GFC_OS_XBOX360)
# include <xtl.h>

#elif defined(GFC_OS_UNIX) || defined(GFC_OS_LINUX)
# if defined(GFC_OS_DARWIN) && !defined(__USE_WS_X11__)
#  define GFC_WS_MACX
# else
#  define GFC_WS_X11
# endif
#else
//# error "GFx does not support this Windowing System - contact support@scaleform.com"
#endif



// **** Compiler

/* GFC is compatible with the following compilers: (GFC_CC_x)

     MSVC     - Microsoft Visual C/C++
     INTEL    - Intel C++ for Linux / Windows
     GNU      - GNU C++
     SNC      - SN Systems ProDG C/C++ (EDG)
     MWERKS   - Metrowerks CodeWarrior
     BORLAND  - Borland C++ / C++ Builder
     RENESAS  - Renesas SH C/C++
*/

#if defined(__INTEL_COMPILER)
// Intel 4.0                    = 400
// Intel 5.0                    = 500
// Intel 6.0                    = 600
// Intel 8.0                    = 800
// Intel 9.0                    = 900
# define GFC_CC_INTEL       __INTEL_COMPILER

#elif defined(_MSC_VER)
// MSVC 5.0                     = 1100
// MSVC 6.0                     = 1200
// MSVC 7.0 (VC2002)            = 1300
// MSVC 7.1 (VC2003)            = 1310
// MSVC 8.0 (VC2005)            = 1400
# define GFC_CC_MSVC        _MSC_VER

#elif defined(__MWERKS__)
// Metrowerks C/C++ 2.0         = 0x2000
// Metrowerks C/C++ 2.2         = 0x2200
# define GFC_CC_MWERKS      __MWERKS__

#elif defined(__BORLANDC__) || defined(__TURBOC__)
// Borland C++ 5.0              = 0x500
// Borland C++ 5.02             = 0x520
// C++ Builder 1.0              = 0x520
// C++ Builder 3.0              = 0x530
// C++ Builder 4.0              = 0x540
// Borland C++ 5.5              = 0x550
// C++ Builder 5.0              = 0x550
// Borland C++ 5.51             = 0x551
# define GFC_CC_BORLAND     __BORLANDC__

// SNC must come before GNUC because
// the SNC compiler defines GNUC as well
#elif defined(__SNC__)
# define GFC_CC_SNC

#elif defined(__GNUC__)
# define GFC_CC_GNU

#elif defined(__RENESAS__) && defined(_SH)
# define GFC_CC_RENESAS

#else
# error "GFC does not support this Compiler - contact support@scaleform.com"
#endif



// **** STL

/* GFC is compatible with the following STLs: (GFC_GTL_x)

     STLSTD   - Standard "system" STL
     STLPORT  - STLPort
     STLSGI   - Standard SGI STL

NOTE: Defined using the GFC_BUILD_x preprocessor
      macros, either - GFC_BUILD_STLPORT, GFC_BUILD_STLSGI, GFC_BUILD_STLSTD
      and the optional GFC_BUILD_STLSHARED
*/

// ** STLport
#if defined(GFC_BUILD_STLPORT) || defined(_STLPORT)
# define GFC_GTL_STLPORT

# ifdef GFC_BUILD_STLSHARED
// Must add "gtl/stlport" to the compilers include directory list
#  define GTL                   _STL
#  define GTLX                  _STLEXT
#  define GFC_GTL_HEADER(file)  <gtl/stlport/file>
# else // GFC_BUILD_STLSHARED
#  define GTL                   _STL
#  define GTLX                  GTL
#  define GFC_GTL_HEADER(file)  <gtl/stlport_local/gtl_##file>
# endif // GFC_BUILD_STLSHARED

# ifdef BUILD_GKERNEL
#  define _STLP_DESIGNATED_DLL  1
# endif
# define _STLP_USE_DECLSPEC     1

// ** STLsgi
#elif defined(GFC_BUILD_STLSGI)
# define GFC_GTL_STLSGI

# ifdef GFC_BUILD_STLSHARED
#  define GTL                   std
#  define GTLX                  stdext
#  define GFC_GTL_HEADER(file)  <gtl/stlsgi/file>
#  define __STL_OWN_NAMESPACE
# else // GFC_BUILD_STLSHARED
#  define GTL                   stlsgi
#  define GTLX                  GTL
#  define GFC_GTL_HEADER(file)  <gtl/stlsgi_local/gtl_##file>
# endif // GFC_BUILD_STLSHARED

// ** STL standard
#elif defined(GFC_CC_SNC)
# define GFC_GTL_STLSTD
# define GTL                    _STL
# define GTLX                   GTL
# define GFC_GTL_HEADER(file)   <file>
#else // defined(GFC_BUILD_STLSTD)
# define GFC_GTL_STLSTD
# define GTL                    std
# define GTLX                   stdext
# define GFC_GTL_HEADER(file)   <file>
#endif


// Ignore compiler warnings specific to SN compiler
#if defined(GFC_CC_SNC)
# pragma diag_suppress=68   // integer conversion resulted in a change of sign
# pragma diag_suppress=112  // statement is unreachable
// (possibly due to SN disliking modifying a mutable object in a const function)
# pragma diag_suppress=175  // expression has no effect
// (deliberately placed so it would NOT produce a warning...)
# pragma diag_suppress=178  // variable was declared but never referenced
# pragma diag_suppress=382  // extra ";" ignored
# pragma diag_suppress=552  // variable was set but never used
# pragma diag_suppress=613  // overloaded virtual function is only partially overridden in class
# pragma diag_suppress=999  // function funcA is hidden by funcB -- virtual function override intended?
#endif


// *** Linux Unicode - must come before Standard Includes

#ifdef GFC_OS_LINUX
/* Use glibc unicode functions on linux. */
#ifndef  _GNU_SOURCE
# define _GNU_SOURCE
#endif
#endif

// *** Symbian - fix defective header files

#ifdef GFC_OS_SYMBIAN
#include <_ansi.h>
#undef _STRICT_ANSI
#include <stdlib.h>
#endif


// ******* Standard Includes

#include    <stddef.h>
#include    <limits.h>
#include    <float.h>

// This macro needs to be defined if it is necessary to avoid the use of Double.
// In that case Double in defined as Float and thus extra #ifdef checks on
// overloads need to be done. Useful for platforms with poor/unavailable
// Double support, such as PS2 and PSP.
#if (defined(GFC_OS_PSP) || defined(GFC_OS_PS2)) && !defined(GFC_NO_DOUBLE)
# define GFC_NO_DOUBLE
#endif

// **** Automatic GFC_BUILD options

// MS Visual Studio defines
#ifdef  GFC_CC_MSVC
# ifndef NDEBUG
#  ifndef GFC_BUILD_DEBUG
#   define GFC_BUILD_DEBUG
#  endif
# endif
#endif // GFC_CC_MSVC

// XBox forces static linking
#ifdef  GFC_OS_XBOX
# ifndef GFC_BUILD_STATICLIB
#  define GFC_BUILD_STATICLIB
# endif
#endif


// ***** Static Lib Linking

#if defined(GFC_BUILD_STATICLIB)
// Force static library linking in BUILD_STATICLIB mode
# define GFC_STATICLIB_LINK(libname)                \
  extern "C" void GFC_ForceLink_##libname();        \
  class GForceLink_##libname {                      \
  public: GForceLink_##libname()    { GFC_ForceLink_##libname(); } };   \
  static GForceLink_##libname GFC_ForceLinkLib_##libname;
#else
# define GFC_STATICLIB_LINK(libname)
#endif



// *******  Definitions

// Byte order
#define GFC_LITTLE_ENDIAN       1
#define GFC_BIG_ENDIAN          2

typedef bool                Bool;
typedef void*               Handle;

// Compiler specific integer
typedef int                 SInt;
typedef unsigned int        UInt;

// Pointer-size integer
typedef size_t              UPInt;
typedef ptrdiff_t           SPInt;



// **** Win32 or XBox

#if defined(GFC_OS_WIN32) || defined(GFC_OS_XBOX) || defined(GFC_OS_XBOX360) || defined(GFC_OS_WINCE)

// ** Miscellaneous compiler definitions

// DLL linking options

#ifdef GFC_BUILD_STATICLIB
// BUILD_STATICLIB means no DLL support, so disable symbol exports
# define    GIMPORT
# define    GEXPORT
#else
# define    GIMPORT         __declspec(dllimport)
# define    GEXPORT         __declspec(dllexport)
#endif // GFC_BUILD_STATICLIB

#define GEXPORTC            extern "C" GEXPORT

// Build is used for importing (must be defined to nothing)
#define GFC_BUILD

// Calling convention - goes after function return type but before function name
#define GFASTCALL           __fastcall
#define GSTDCALL            __stdcall
#define GCDECL              __cdecl

// Byte order
#if defined(GFC_OS_XBOX360)
#define GFC_BYTE_ORDER          GFC_BIG_ENDIAN
#else
#define GFC_BYTE_ORDER          GFC_LITTLE_ENDIAN
#endif

#if defined(GFC_CC_MSVC)
// Disable "inconsistent dll linkage" warning
# pragma warning(disable : 4127)
// Disable "exception handling" warning
# pragma warning(disable : 4530)
# if (GFC_CC_MSVC<1300)
// Disable "unreferenced inline function has been removed" warning
# pragma warning(disable : 4514)
// Disable "function not inlined" warning
# pragma warning(disable : 4710)
// Disable "_force_inline not inlined" warning
# pragma warning(disable : 4714)
// Disable "debug variable name longer than 255 chars" warning
# pragma warning(disable : 4786)
# endif // (GFC_CC_MSVC<1300)
#endif

// Assembly macros
#if defined(GFC_CC_MSVC)
#define GASM                _asm
#else
#define GASM                asm
#endif

// Inline substitute - goes before function declaration
#if defined(GFC_CC_MSVC)
# define GINLINE            __forceinline
#else
# define GINLINE            inline
#endif  // GFC_CC_MSVC


// *** Type definitions for Win32

typedef char                Char;

// 8 bit Integer (Byte)
typedef char                SInt8;
typedef unsigned char       UInt8;
typedef SInt8               SByte;
typedef UInt8               UByte;

// 16 bit Integer (Word)
typedef short               SInt16;
typedef unsigned short      UInt16;

// 32 bit Integer (DWord)
typedef long                SInt32;
typedef unsigned long       UInt32;

// 64 bit Integer (QWord)
typedef __int64             SInt64;
typedef unsigned __int64    UInt64;

// Floating point
typedef float               Float;

#ifdef GFC_NO_DOUBLE
typedef float               Double;
#else
typedef double              Double;
#endif

#ifdef UNICODE
#define GSTR(str) L##str
#else
#define GSTR(str) str
#endif

// **** Standard systems

#else

// ** Miscellaneous compiler definitions

// DLL linking options
#define GIMPORT
#define GEXPORT
#define GEXPORTC                extern "C" GEXPORT

// Build is used for importing, defined to nothing
#define GFC_BUILD

// Assembly macros
#define GASM                    __asm__
#define GASM_PROC(procname)     GASM
#define GASM_END                GASM

// Inline substitute - goes before function declaration
#define GINLINE                 inline

// Calling convention - goes after function return type but before function name
#define GFASTCALL
#define GSTDCALL
#define GCDECL

#if (defined(GFC_OS_PSP) || defined(GFC_OS_PS2))
#define GFC_BYTE_ORDER          GFC_LITTLE_ENDIAN
#elif (defined(BYTE_ORDER) && (BYTE_ORDER == BIG_ENDIAN))|| \
      (defined(_BYTE_ORDER) && (_BYTE_ORDER == _BIG_ENDIAN))
#define GFC_BYTE_ORDER          GFC_BIG_ENDIAN
#elif (defined(GFC_OS_PS3) || defined(__ARMEB__) || defined(GFC_CPU_PPC) || defined(GFC_CPU_PPC64))
#define GFC_BYTE_ORDER          GFC_BIG_ENDIAN
#elif defined (GFC_OS_WII)
#define GFC_BYTE_ORDER          GFC_BIG_ENDIAN
#else
#define GFC_BYTE_ORDER          GFC_LITTLE_ENDIAN
#endif

// *** Type definitions for common systems

typedef char                Char;

#if defined(GFC_CC_GNU) || defined(GFC_CC_SNC)

typedef int          SInt8  __attribute__((__mode__ (__QI__)));
typedef unsigned int UInt8  __attribute__((__mode__ (__QI__)));
typedef int          SByte  __attribute__((__mode__ (__QI__)));
typedef unsigned int UByte  __attribute__((__mode__ (__QI__)));

typedef int          SInt16 __attribute__((__mode__ (__HI__)));
typedef unsigned int UInt16 __attribute__((__mode__ (__HI__)));

typedef int          SInt32 __attribute__((__mode__ (__SI__)));
typedef unsigned int UInt32 __attribute__((__mode__ (__SI__)));

typedef int          SInt64 __attribute__((__mode__ (__DI__)));
typedef unsigned int UInt64 __attribute__((__mode__ (__DI__)));


#elif defined(GFC_OS_WII)
#include <types.h>

// 8 bit Integer (Byte)
typedef s8              SInt8;
typedef u8              UInt8;
typedef s8              SByte;
typedef u8              UByte;

// 16 bit Integer
typedef s16             SInt16;
typedef u16             UInt16;

// 32 bit Integer
typedef s32             SInt32;
typedef u32             UInt32;

// 64 bit Integer
typedef s64             SInt64;
typedef u64             UInt64;

#elif (defined(GFC_OS_PS2) || defined(GFC_OS_PSP)) && defined(GFC_CC_MWERKS)

// 8 bit Integer (Byte)
typedef char            SInt8;
typedef unsigned char   UInt8;
typedef char            SByte;
typedef unsigned char   UByte;

// 16 bit Integer
typedef short           SInt16;
typedef unsigned short  UInt16;

// 32 bit Integer
typedef int             SInt32;
typedef unsigned int    UInt32;

// 64 bit Integer
typedef signed long long    SInt64;
typedef unsigned long long  UInt64;

#elif defined(GFC_CC_RENESAS)

typedef unsigned short      wchar_t;

typedef char                Char;

// 8 bit Integer (Byte)
typedef char                SInt8;
typedef unsigned char       UInt8;
typedef SInt8               SByte;
typedef UInt8               UByte;

// 16 bit Integer (Word)
typedef short               SInt16;
typedef unsigned short      UInt16;

// 32 bit Integer (DWord)
typedef long                SInt32;
typedef unsigned long       UInt32;

// 64 bit Integer (QWord)
typedef long long           SInt64;
typedef unsigned long long  UInt64;

#else
#include <sys/types.h>

// 8 bit Integer (Byte)
typedef int8_t              SInt8;
typedef uint8_t             UInt8;
typedef int8_t              SByte;
typedef uint8_t             UByte;

// 16 bit Integer
typedef int16_t             SInt16;
typedef uint16_t            UInt16;

// 32 bit Integer
typedef int32_t             SInt32;
typedef uint32_t            UInt32;

// 64 bit Integer
typedef int64_t             SInt64;
typedef uint64_t            UInt64;

#endif

// Floating point
typedef float               Float;

#ifdef GFC_NO_DOUBLE
typedef float               Double;
#else
typedef double              Double;
#endif

#endif

// Fixed point data type definitions
// Number after Fix stands for amount of fractional bits
typedef SInt                SFix16;
typedef UInt                UFix16;
typedef SInt                SFix8;
typedef UInt                UFix8;



// ******** Debug stuff

#include    "GDebug.h"

// ***** Compiler Assert

// Compile-time assert.  Thanks to Jon Jagger (http://www.jaggersoft.com) for this trick.
#define GCOMPILER_ASSERT(x)         switch(0){case 0: case x:;}

// Handy macro to quiet compiler warnings about unused parameters/variables.
#if defined(GFC_CC_GNU)
#define     GUNUSED(a)          ({__typeof__ (&a) __attribute__ ((unused)) __tmp = &a; })
#define     GUNUSED2(a,b)       GUNUSED(a); GUNUSED(b)
#define     GUNUSED3(a,b,c)     GUNUSED2(a,c); GUNUSED(b)
#define     GUNUSED4(a,b,c,d)   GUNUSED3(a,c,d); GUNUSED(b)
#else
#define     GUNUSED(a)          (a)
#define     GUNUSED2(a,b)       (a);(b)
#define     GUNUSED3(a,b,c)     (a);(b);(c)
#define     GUNUSED4(a,b,c,d)   (a);(b);(c);(d)
#endif

// ******** Variable range definitions

#if defined(GFC_CC_MSVC)
#define GUINT64(x) x
#else
#define GUINT64(x) x##LL
#endif

// 8 bit Integer ranges (Byte)
#define GFC_MAX_SINT8           SInt8(0x7F)                 //  127
#define GFC_MIN_SINT8           SInt8(0x80)                 // -128
#define GFC_MAX_UINT8           UInt8(0xFF)                 //  255
#define GFC_MIN_UINT8           UInt8(0x00)                 //  0

#define GFC_MAX_SBYTE           GFC_MAX_SINT8               //  127
#define GFC_MIN_SBYTE           GFC_MIN_SINT8               // -128
#define GFC_MAX_UBYTE           GFC_MAX_UINT8               //  255
#define GFC_MIN_UBYTE           GFC_MIN_UINT8               //  0

// 16 bit Integer ranges (Word)
#define GFC_MAX_SINT16          SInt16(SHRT_MAX)            //  32767
#define GFC_MIN_SINT16          SInt16(SHRT_MIN)            // -32768
#define GFC_MAX_UINT16          UInt16(USHRT_MAX)           //  65535
#define GFC_MIN_UINT16          UInt16(0)                   //  0

// 32 bit Integer ranges (Int)
#define GFC_MAX_SINT32          SInt32(INT_MAX)             //  2147483647
#define GFC_MIN_SINT32          SInt32(INT_MIN)             // -2147483648
#define GFC_MAX_UINT32          UInt32(UINT_MAX)            //  4294967295
#define GFC_MIN_UINT32          UInt32(0)                   //  0

// 64 bit Integer ranges (Long)
#define GFC_MAX_SINT64          SInt64(0x7FFFFFFFFFFFFFFF)  // -9223372036854775808
#define GFC_MIN_SINT64          SInt64(0x8000000000000000)  //  9223372036854775807
#define GFC_MAX_UINT64          UInt64(0xFFFFFFFFFFFFFFFF)  //  18446744073709551615
#define GFC_MIN_UINT64          UInt64(0)                   //  0

// Compiler specific
#define GFC_MAX_SINT            SInt(GFC_MAX_SINT32)        //  2147483647
#define GFC_MIN_SINT            SInt(GFC_MIN_SINT32)        // -2147483648
#define GFC_MAX_UINT            UInt(GFC_MAX_UINT32)        //  4294967295
#define GFC_MIN_UINT            UInt(GFC_MIN_UINT32)        //  0


#if defined(GFC_64BIT_POINTERS)
#define GFC_MAX_SPINT           GFC_MAX_SINT64
#define GFC_MIN_SPINT           GFC_MIN_SINT64
#define GFC_MAX_UPINT           GFC_MAX_UINT64
#define GFC_MIN_UPINT           GFC_MIN_UINT64
#else
#define GFC_MAX_SPINT           GFC_MAX_SINT
#define GFC_MIN_SPINT           GFC_MIN_SINT
#define GFC_MAX_UPINT           GFC_MAX_UINT
#define GFC_MIN_UPINT           GFC_MIN_UINT
#endif

// ** Floating point ranges

// Minimum and maximum (positive) Float values
#define GFC_MIN_FLOAT           Float(FLT_MIN)              // 1.175494351e-38F
#define GFC_MAX_FLOAT           Float(FLT_MAX)              // 3.402823466e+38F

#ifdef GFC_NO_DOUBLE
// Minimum and maximum (positive) Double values
#define GFC_MIN_DOUBLE          Double(FLT_MIN)             // 2.2250738585072014e-308
#define GFC_MAX_DOUBLE          Double(FLT_MAX)             // 1.7976931348623158e+308
#else
// Minimum and maximum (positive) Double values
#define GFC_MIN_DOUBLE          Double(DBL_MIN)             // 2.2250738585072014e-308
#define GFC_MAX_DOUBLE          Double(DBL_MAX)             // 1.7976931348623158e+308
#endif



// *** Flags
#define GFC_FLAG32(value)       (UInt32(value))
#define GFC_FLAG64(value)       (UInt64(value))


// ***** Operator extensions

namespace GTL {

// *** min/max are not implemented in Visual Studio 6 standard STL

template <typename T>   GINLINE const T gmin(const T a, const T b)
    { return (a < b) ? a : b; }
template <typename T>   GINLINE const T gmax(const T a, const T b)
    { return (b < a) ? a : b; }
template <typename T>   GINLINE const T gclamp(const T v, const T minVal, const T maxVal)
    { return gmax<T>(minVal, gmin<T>(v, maxVal)); }

template <typename T>   GINLINE void gswap(T &a, T &b)
    {  T temp(a); a = b; b = temp; }

// These functions stand to fix a stupid VC++ warning (with /Wp64 on):
// "warning C4267: 'argument' : conversion from 'size_t' to 'const UInt', possible loss of data"
// Use these functions instead of gmin/gmax if the argument has size
// of the pointer to avoid the warning. Though, functionally they are
// absolutelly the same as regular gmin/gmax.
template <typename T>   GINLINE const T gpmin(const T a, const T b)
{
    GCOMPILER_ASSERT(sizeof(T) == sizeof(UPInt));
    return (a < b) ? a : b;
}
template <typename T>   GINLINE const T gpmax(const T a, const T b)
{
    GCOMPILER_ASSERT(sizeof(T) == sizeof(UPInt));
    return (b < a) ? a : b;
}

// *** abs

template <typename T>   GINLINE const T gabs(const T v)
    { return (v>=0) ? v : -v; }

}; // GTL


// *** Special Documentation Set


// GFC Build type
#ifdef GFC_BUILD_DEBUG
#define GFC_BUILD_STRING        "Debug"
#else
#define GFC_BUILD_STRING        "Release"
#endif


#ifdef GFC_DOM_INCLUDE

// Enables Windows MFC Compatibility
# define GFC_BUILD_MFC

// STL configuration
// - default is STLSTD if not set
# define GFC_BUILD_STLPORT
# define GFC_BUILD_STLSGI
# define GFC_BUILD_STLSTD

// Enables GFC Debugging information
# define GFC_BUILD_DEBUG

// Enables GFC Static library build
# define GFC_BUILD_STATICLIB

// Causes the GFC GMemory allocator
// to be used for all new and delete's
# define GFC_BUILD_NEW_OVERRIDE

// Prevents GFC from defining new within
// type macros, so developers can override
// new using the #define new new(...) trick
// - used with GFC_DEFINE_NEW macro
# define GFC_BUILD_DEFINE_NEW

// Enables GFC Purify customized build
# define GFC_BUILD_PURIFY
// Enables GFC Evaluation build with time locking
# define GFC_BUILD_EVAL

#endif

#endif  // INC_GTYPES_H
