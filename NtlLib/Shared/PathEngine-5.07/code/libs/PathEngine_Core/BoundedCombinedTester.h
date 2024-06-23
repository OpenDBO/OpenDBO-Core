//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef BOUNDED_COLLISION_TESTER_INCLUDED
#define BOUNDED_COLLISION_TESTER_INCLUDED

#include "libs/PathEngine_Core/interface/iCollisionTester.h"
#include "libs/Geometry/interface/tPoint.h"

class cBoundedCombinedTester : public iLineCollisionTester
{
    const iLineCollisionTester* _target1;
    const iLineCollisionTester* _target2;
    tPoint _boundsMin, _boundsMax;
    bool _boundsValid;

public:

    cBoundedCombinedTester(
        const iLineCollisionTester* target1,
        const iLineCollisionTester* target2,
        const tPoint& boundsMin,
        const tPoint& boundsMax,
        bool boundsValid
        )
    {
        _target1 = target1;
        _target2 = target2;
        _boundsMin = boundsMin;
        _boundsMax = boundsMax;
        _boundsValid = boundsValid;
    }

    bool testCollision(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const;
    bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line) const;
    bool testCollisionToXY(cQueryContext& qc, tFace startF, const tLine& line, tFace& endF) const;
    bool testForSnagsToSegmentEnd(cQueryContext& qc, const tLine& segmentLine, const cInternalPosition& approximation) const;
};

#endif
