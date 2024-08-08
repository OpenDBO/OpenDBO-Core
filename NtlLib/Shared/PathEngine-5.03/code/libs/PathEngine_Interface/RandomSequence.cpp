//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "RandomSequence.h"

cRandomSequence::cRandomSequence()
{
    _seed = 0;
}

long
cRandomSequence::getSeed() const
{
    return _seed;
}
void
cRandomSequence::setSeed(long value)
{
    _seed = value;
}
long
cRandomSequence::random15()
{
    _seed = _seed * 214013L + 2531011L; //..... platform dependant
    return (_seed >> 16) & 0x7fff;
}
long
cRandomSequence::random31()
{
    long result = random15();
    result <<= 15;
    result ^= random15();
    result <<= 1;
    result ^= random15();
    return result;
}
