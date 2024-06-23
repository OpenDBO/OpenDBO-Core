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
#include "interface/ShapeRenderPreprocess.h"
#include "libs/PathEngine_Core/interface/OverlappingPolygon.h"
#include "libs/Geometry/interface/CollisionShape.h"

cShapeRenderPreprocess::cShapeRenderPreprocess(
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& baseShape,
        const cCollisionShape& expandShape
        )
{
    reinit(baseShapePosition, baseShape, expandShape);
}

bool
cShapeRenderPreprocess::isValidFor(const cInternalPosition& baseShapePosition) const
{
    return _preprocessedPosition == baseShapePosition;
}
void
cShapeRenderPreprocess::reinit(
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& baseShape,
        const cCollisionShape& expandShape
        )
{
    _preprocessedPosition = baseShapePosition;
    _circuits.clear();
    std::list<tFace> containedExternalFaces;
    cCollisionShape combinedShape(baseShape, expandShape);
    CircuitsForOverlappingPolygon(_preprocessedPosition, combinedShape, containedExternalFaces, _circuits);
}
