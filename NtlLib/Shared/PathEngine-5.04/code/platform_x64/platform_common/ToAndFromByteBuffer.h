//**********************************************************************
//
// Copyright (c) 2005
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_TOANDFROMBYTEBUFFER_INCLUDED
#define PLATFORM_COMMON_TOANDFROMBYTEBUFFER_INCLUDED

#include "common/interface/Assert.h"

inline void
SignedToByteBuffer(long value, long tokenSize, char* bufferPtr)
{
    switch(tokenSize)
    {
    default:
        invalid();
    case 1:
        {
            signed char asChar = static_cast<signed char>(value);
            assertD(value == asChar);
            *(signed char*)bufferPtr = asChar;
            break;
        }
    case 2:
        {
            short asShort = static_cast<short>(value);
            assertD(value == asShort);
            *(short*)bufferPtr = asShort;
            break;
        }
    case 4:
        {
            *(long*)bufferPtr = value;
            break;
        }
    }
}
inline void
UnsignedToByteBuffer(unsigned long value, long tokenSize, char* bufferPtr)
{
    switch(tokenSize)
    {
    default:
        invalid();
    case 1:
        {
            unsigned char asChar = static_cast<unsigned char>(value);
            assertD(value == asChar);
            *(signed char*)bufferPtr = asChar;
            break;
        }
    case 2:
        {
            unsigned short asShort = static_cast<unsigned short>(value);
            assertD(value == asShort);
            *(unsigned short*)bufferPtr = asShort;
            break;
        }
    case 4:
        {
            *(unsigned long*)bufferPtr = value;
            break;
        }
    }
}

inline long
SignedFromByteBuffer(const char* bufferPtr, long tokenSize)
{
    switch(tokenSize)
    {
    default:
        invalid();
    case 1:
        return *(const signed char*)bufferPtr;
    case 2:
        return *(const short*)bufferPtr;
    case 4:
        return *(const long*)bufferPtr;
    }
}
inline unsigned long
UnsignedFromByteBuffer(const char* bufferPtr, long tokenSize)
{
    switch(tokenSize)
    {
    default:
        invalid();
    case 1:
        return *(const unsigned char*)bufferPtr;
    case 2:
        return *(const unsigned short*)bufferPtr;
    case 4:
        return *(const unsigned long*)bufferPtr;
    }
}

inline void
FloatToByteBuffer(float value, char* bufferPtr)
{
    UnsignedToByteBuffer(*(reinterpret_cast<unsigned long*>(&value)), 4, bufferPtr);
}
inline float
FloatFromByteBuffer(const char* bufferPtr)
{
    unsigned long ul = UnsignedFromByteBuffer(bufferPtr, 4);
    return *(reinterpret_cast<float*>(&ul));
}
static const long FLOAT_BYTE_SIZE = 4;

#endif
