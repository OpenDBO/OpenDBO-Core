//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "libs/Geometry/interface/tLine.h"
#include "platform_common/SquareRoot.h"

long Length(const tPoint &vector)
{
    return static_cast<long>(SquareRoot(tPoint::tMultiplier::asFloat(vector.lengthSquared())));
}
double Length_AsDouble(const tPoint &vector)
{
    return SquareRoot(tPoint::tMultiplier::asFloat(vector.lengthSquared()));
}
void SetLength(tPoint& vector, long new_length)
{
    double x = vector.getX();
    double y = vector.getY();
    double ratio = new_length;
    ratio /= SquareRoot(tPoint::tMultiplier::asFloat(vector.lengthSquared()));
    x *= ratio;
    y *= ratio;
    vector = tPoint(static_cast<long>(x), static_cast<long>(y), new_length + 1);
}

void
LengthSquared(const tPoint& vector, tPoint::tDistance& result)
{
    result = vector.lengthSquared();
}

int
CompareDistances(const tPoint::tDistance& lhs, const tPoint::tDistance& rhs)
{
    return tMultiplier::compare(lhs, rhs);
}

eSide Side_Vectors(const tPoint& base, const tPoint& test)
{
    return base.side(test);
}
eDirection Direction_Vectors(const tPoint& base, const tPoint& test)
{
    return base.direction(test);
}

//eSide SideOfLine(const tLine& line, const tPoint& testPoint)
//{
//    return line.side(testPoint);
//}

