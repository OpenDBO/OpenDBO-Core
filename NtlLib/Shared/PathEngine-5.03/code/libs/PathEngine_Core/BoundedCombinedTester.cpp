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
#include "BoundedCombinedTester.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Geometry/interface/tLine.h"

bool
cBoundedCombinedTester::testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
{
    if(_boundsValid && !to.p.insideOrOnBounds(_boundsMin, _boundsMax))
    {
        return true;
    }
    if(_target1->testCollision(qc, from, to))
    {
        return true;
    }
    if(_target2->testCollision(qc, from, to))
    {
        return true;
    }
    return false;
}
bool
cBoundedCombinedTester::testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const
{
    if(_boundsValid && !line.end().insideOrOnBounds(_boundsMin, _boundsMax))
    {
        return true;
    }
    if(_target1->testCollisionToXY(qc, startF, line))
    {
        return true;
    }
    if(_target2->testCollisionToXY(qc, startF, line))
    {
        return true;
    }
    return false;
}
bool
cBoundedCombinedTester::testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const
{
    if(_boundsValid && !line.end().insideOrOnBounds(_boundsMin, _boundsMax))
    {
        return true;
    }
    if(_target1->testCollisionToXY(qc, startF, line, endF))
    {
        return true;
    }
    if(_target2->testCollisionToXY(qc, startF, line))
    {
        return true;
    }
    return false;
}

bool
cBoundedCombinedTester::testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const
{
    if(_target1->testForSnagsToSegmentEnd(qc, segmentLine, approximation))
    {
        return true;
    }
    if(_target2->testForSnagsToSegmentEnd(qc, segmentLine, approximation))
    {
        return true;
    }
    return false;
}
