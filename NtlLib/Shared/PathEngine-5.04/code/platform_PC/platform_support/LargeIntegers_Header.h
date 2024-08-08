//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LARGE_INTEGERS_HEADER_INCLUDED
#define LARGE_INTEGERS_HEADER_INCLUDED

class cLargeInteger2
{
public:

    unsigned long digits[3];

    cLargeInteger2()
    {
    }
    explicit inline cLargeInteger2(long value);
};

class cLargeInteger4
{
public:

    unsigned long digits[5];

    cLargeInteger4()
    {
    }
    explicit inline cLargeInteger4(const cLargeInteger2& value);
    explicit inline cLargeInteger4(long value);
};

#endif
