/****************************************************************************
 *                                                                          *
 *  Module  :   crt_compat.cpp                                              *
 *                                                                          *
 *  Purpose :   CRT compatibility shim for old pre-VS2015 libraries         *
 *                                                                          *
 ****************************************************************************/

#include "precomp_dboclient.h"

#include <cstdarg>
#include <stddef.h>

#if defined(_MSC_VER) && _MSC_VER >= 1900

extern "C" FILE* __iob_func(unsigned index)
{
    switch (index)
    {
        case 0:  return stdin;
        case 1:  return stdout;
        case 2:  return stderr;
        default: return (FILE*)0;
    }
}

extern "C" int __cdecl _vsnprintf_s(
    char *buffer,
    size_t sizeOfBuffer,
    size_t count,
    const char *format,
    va_list argptr)
{
    int result = __stdio_common_vsnprintf_s(
        0,
        buffer, sizeOfBuffer, count,
        format, NULL, argptr);

    return result < 0 ? -1 : result;
}

extern "C" int __cdecl vsnprintf_s(
    char *buffer,
    size_t sizeOfBuffer,
    size_t count,
    const char *format,
    va_list argptr)
{
    return _vsnprintf_s(buffer, sizeOfBuffer, count, format, argptr);
}

#endif
