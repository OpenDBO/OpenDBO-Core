//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

void
QuantisedRotationCoefficients(long rotation, long& sinOf, long& cosOf);

inline long
QuantisedRotationDivisor()
{
    return 4096; // 2 ^ 12
}
inline long
QuantisedRotationAsPathEngineAngle(long rotation)
{
    return rotation * (1048576 / 4096);
}
inline long
MaximumValueSupportedByQuantisedRotation()
{
    return 524287; // 2^19 - 1
}
inline long
QuantisedRotationNumberOfRotations()
{
    return 64;
}
