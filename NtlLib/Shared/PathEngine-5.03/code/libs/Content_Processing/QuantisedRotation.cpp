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
#include "QuantisedRotation.h"
#include "common/interface/Assert.h"

inline long
CosOf_90Degrees(long rotation)
{
    assertD(rotation >= 0 && rotation <= 16);
    static const long table[] = {4096, 4076, 4017, 3920, 3784, 3612, 3406, 3166, 2896, 2598, 2276, 1931, 1567, 1189, 799, 401, 0};
    return table[rotation];
}

inline long
CosOf_360Degrees(long rotation)
{
    switch(rotation >> 4)
    {
    default:
        invalid();
    case 0:
        return CosOf_90Degrees(rotation);
    case 1:
        return -CosOf_90Degrees(32 - rotation);
    case 2:
        return -CosOf_90Degrees(rotation - 32);
    case 3:
        return CosOf_90Degrees(64 - rotation);
    }
}

inline long
SinOf_360Degrees(long rotation)
{
    return CosOf_360Degrees((rotation + 48) & 63);
}

void
QuantisedRotationCoefficients(long rotation, long& sinOf, long& cosOf)
{
    assertD(rotation >= 0 && rotation < 64);
    sinOf = SinOf_360Degrees(rotation);
    cosOf = CosOf_360Degrees(rotation);
}

