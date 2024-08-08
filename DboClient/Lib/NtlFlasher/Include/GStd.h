/**********************************************************************

Filename    :   GStd.h
Content     :   Standard C function interface
Created     :   2/25/2007
Authors     :   Ankur Mohan, Michael Antonov, Artem Bolgar
Copyright   :   (c) 2007 Scaleform Corp. All Rights Reserved.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GStd_H
#define INC_GStd_H

#include "GTypes.h"
#include <stdarg.h> // for va_list args
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#if defined(GFC_CC_MSVC) && (GFC_CC_MSVC >= 1400)
#define GFC_MSVC_SAFESTRING
#endif


#if defined(GFC_OS_WIN32)
inline char* gfc_itoa(int val, char *dest, size_t destsize, int radix)
{
#if defined(GFC_MSVC_SAFESTRING)
    _itoa_s(val, dest, destsize, radix);
    return dest;
#else
    GUNUSED(destsize);
    return itoa(val, dest, radix);
#endif
}
#endif // GFC_OS_PS3

// String functions

inline size_t gfc_strlen(const char* str)
{
    return strlen(str);
}

inline char* gfc_strcpy(char* dest, size_t destsize, const char* src)
{
#if defined(GFC_MSVC_SAFESTRING)
    strcpy_s(dest, destsize, src);
    return dest;
#else
    GUNUSED(destsize);
    return strcpy(dest, src);
#endif
}

inline char* gfc_strncpy(char* dest, size_t destsize, const char* src, size_t count)
{
#if defined(GFC_MSVC_SAFESTRING)
    strncpy_s(dest, destsize, src, count);
    return dest;
#else
    GUNUSED(destsize);
    return strncpy(dest, src, count);
#endif
}

inline char * gfc_strcat(char* dest, size_t destsize, const char* src)
{
#if defined(GFC_MSVC_SAFESTRING)
    strcat_s(dest, destsize, src);
    return dest;
#else
    GUNUSED(destsize);
    return strcat(dest, src);
#endif
}

inline int gfc_strcmp(const char* dest, const char* src)
{
    return strcmp(dest, src);
}

// Implemented in GStd.cpp in platform-specific manner.
int gfc_stricmp(const char* dest, const char* src);
int gfc_strnicmp(const char* dest, const char* src, size_t count);

inline size_t gfc_sprintf(char *dest, size_t destsize, const char* format, ...)
{
    va_list argList;
    va_start(argList,format);
    size_t ret;
#if defined(GFC_CC_MSVC)
    #if defined(GFC_MSVC_SAFESTRING)
        ret = _vsnprintf_s(dest, destsize, _TRUNCATE, format, argList);
        GASSERT(ret != -1);
    #else
        GUNUSED(destsize);
        ret = _vsnprintf(dest, destsize - 1, format, argList); // -1 for space for the null character
        GASSERT(ret != -1);
        dest[destsize-1] = 0;
    #endif
#else
    GUNUSED(destsize);
    ret = vsprintf(dest, format, argList);
    GASSERT(ret < destsize);
#endif
    va_end(argList);
    return ret;
}

inline size_t gfc_vsprintf(char *dest, size_t destsize, const char * format, va_list argList)
{
    size_t ret;
#if defined(GFC_CC_MSVC)
    #if defined(GFC_MSVC_SAFESTRING)
        ret = vsnprintf_s(dest, destsize, _TRUNCATE, format, argList);
        GASSERT(ret != -1);
    #else
        GUNUSED(destsize);
        ret = _vsnprintf(dest, destsize - 1, format, argList);
        GASSERT(ret != -1);
        dest[destsize-1] = 0;
    #endif
#else
    GUNUSED(destsize);
    ret = vsprintf(dest, format, argList);
    GASSERT(ret < destsize);
#endif
    return ret;
}

// Wide-char funcs
#if !defined(GFC_OS_SYMBIAN) && !defined(GFC_CC_RENESAS) && !defined(GFC_OS_PS2)
# include <wchar.h>
#endif

#if !defined(GFC_OS_SYMBIAN) && !defined(GFC_CC_RENESAS) && !defined(GFC_OS_PS2) && !defined(GFC_CC_SNC)
# include <wctype.h>
#endif

wchar_t*    gfc_wcscpy(wchar_t* dest, size_t destsize, const wchar_t* src);
wchar_t*    gfc_wcscat(wchar_t* dest, size_t destsize, const wchar_t* src);
size_t      gfc_wcslen(const wchar_t* str);

inline int  gfc_iswspace(wchar_t c)
{
#if defined(GFC_OS_SYMBIAN) || defined(GFC_OS_WII) || defined(GFC_CC_RENESAS) || defined(GFC_OS_PS2) || defined(GFC_CC_SNC)
    return ((c) < 128 ? isspace((char)c) : 0);
#else
    return iswspace(c);
#endif
}

inline int  gfc_iswalpha(wchar_t c)
{
#if defined(GFC_OS_SYMBIAN) || defined(GFC_OS_WII) || defined(GFC_CC_RENESAS) || defined(GFC_OS_PS2) || defined(GFC_CC_SNC)
    return ((c) < 128 ? isalpha((char)c) : 0);
#else
    return iswalpha(c);
#endif
}

inline int  gfc_iswalnum(wchar_t c)
{
#if defined(GFC_OS_SYMBIAN) || defined(GFC_OS_WII) || defined(GFC_CC_RENESAS) || defined(GFC_OS_PS2) || defined(GFC_CC_SNC)
    return ((c) < 128 ? isalnum((char)c) : 0);
#else
    return iswalnum(c);
#endif
}

inline wchar_t gfc_towlower(wchar_t c)
{
#if defined(GFC_OS_SYMBIAN) || defined(GFC_CC_RENESAS) || defined(GFC_OS_PS2)
    return (wchar_t)tolower((char)c);
#else
    return (wchar_t)towlower(c);
#endif
}

inline wchar_t gfc_towupper(wchar_t c)
{
#if defined(GFC_OS_SYMBIAN) || defined(GFC_CC_RENESAS) || defined(GFC_OS_PS2)
    return (wchar_t)toupper((char)c);
#else
    return (wchar_t)towupper(c);
#endif
}

#endif // INC_GSTD_H
