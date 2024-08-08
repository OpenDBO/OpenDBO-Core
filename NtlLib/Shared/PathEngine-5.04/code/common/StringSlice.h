//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/string.h"

inline std::string
StringSlice(const std::string& s, long i1, long i2)
{
    if(i2 > SizeL(s))
    {
        i2 = SizeL(s);
    }
    if(i1 < 0)
    {
        i1 = SizeL(s) - i1;
    }
    if(i2 < 0)
    {
        i2 = SizeL(s) - i2;
    }
    if(i2 < i1)
    {
        i2 = i1;
    }
    return s.substr(i1, i2 - i1);
}

inline std::string
StringSlice(const std::string& s, long i1)
{
    return StringSlice(s, i1, SizeL(s));
}
