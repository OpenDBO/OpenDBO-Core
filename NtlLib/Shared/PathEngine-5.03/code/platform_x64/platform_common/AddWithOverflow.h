//**********************************************************************
//
// Copyright (c) 2004
// Thomas Young, PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PLATFORM_COMMON_ADD_WITH_OVERFLOW_INCLUDED
#define PLATFORM_COMMON_ADD_WITH_OVERFLOW_INCLUDED

inline bool AddWithOverflow(unsigned long source1, unsigned long source2, unsigned long &result)
{
    result = source1 + source2;
    return result < source1;
}

#endif
