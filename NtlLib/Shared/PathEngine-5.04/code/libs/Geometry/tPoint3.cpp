//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint3.h"
#include "platform_common/SquareRoot.h"
#include "platform_common/PointMultiplication.h"

long Length(const tPoint3& vector)
{
    return static_cast<long>(SquareRoot(tPoint3::tMultiplier::asFloat(vector.lengthSquared())));
}
