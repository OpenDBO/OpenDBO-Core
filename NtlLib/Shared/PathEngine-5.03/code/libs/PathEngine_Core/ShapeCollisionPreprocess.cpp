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
#include "interface/ShapeCollisionPreprocess.h"
#include "interface/LocalisedConvexShape.h"
#include "libs/Instrumentation/interface/Profiler.h"

cShapeCollisionPreprocess::cShapeCollisionPreprocess(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& expandShape)
{
    _expansion2D = 0;
    reinit(qc, baseShape, baseShapePosition, expandShape);
}
cShapeCollisionPreprocess::cShapeCollisionPreprocess(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition
        )
{
    _expansion2D = 0;
    reinit(qc, baseShape, baseShapePosition);
}
cShapeCollisionPreprocess::~cShapeCollisionPreprocess()
{
    delete _expansion2D;
}

bool
cShapeCollisionPreprocess::isValidFor(const cInternalPosition& baseShapePosition) const
{
    return _preprocessedPosition == baseShapePosition;
}
void
cShapeCollisionPreprocess::reinit(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& expandShape)
{
    PROFILER_SCOPE("cShapeCollisionPreprocess::reinit");
    _preprocessedPosition = baseShapePosition;
    delete _expansion2D;
    _expansion2D = new cLocalisedConvexShape(qc, baseShape, baseShapePosition, expandShape);
}
void
cShapeCollisionPreprocess::reinit(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition
        )
{
    PROFILER_SCOPE("cShapeCollisionPreprocess::reinit");
    _preprocessedPosition = baseShapePosition;
    delete _expansion2D;
    _expansion2D = new cLocalisedConvexShape(qc, baseShape, baseShapePosition);
}
