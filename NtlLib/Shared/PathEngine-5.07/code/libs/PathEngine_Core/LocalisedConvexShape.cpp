//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/LocalisedConvexShape.h"
#include "ResolveFaceForIntersection.h"
#include "interface/OverlappingPolygon.h"
#include "interface/QueryContext.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Geometry/interface/CompareFractions.h"
#include "common/Containers/FlagVector.h"
#include "common/Containers/CollapsedVectorVector.h"

cLocalisedConvexShape::cLocalisedConvexShape(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& expandBy
        ) :
 cPlacedConvexShape(baseShape, baseShapePosition.p, expandBy),
 _centreF(baseShapePosition.f)
{
    tFace* openQueueBuffer = qc.lockFaceBuffer();
    cFlagVector& resultBuffer = qc.lockFaceFlagVector();

    FacesOverlappedByOverlappingPolygon_SI(baseShapePosition.f, &_pointVector.front(), _points, openQueueBuffer, resultBuffer, _exitsMesh);
    _facesOverlapped.resize(resultBuffer.numberSet());
    {
        long i;
        for(i = 0; i < resultBuffer.numberSet(); i++)
        {
            _facesOverlapped[i] = resultBuffer.indexSet(i);
        }
    }

    qc.releaseFlagVector();
    qc.releaseGeneralPurposeBuffer();
}

cLocalisedConvexShape::cLocalisedConvexShape(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition
        ) :
 cPlacedConvexShape(baseShape, baseShapePosition.p),
 _centreF(baseShapePosition.f)
{
    tFace* openQueueBuffer = qc.lockFaceBuffer();
    cFlagVector& resultBuffer = qc.lockFaceFlagVector();
    FacesOverlappedByOverlappingPolygon_SI(baseShapePosition.f, &_pointVector.front(), _points, openQueueBuffer, resultBuffer, _exitsMesh);
    _facesOverlapped.resize(resultBuffer.numberSet());
    {
        long i;
        for(i = 0; i < resultBuffer.numberSet(); i++)
        {
            _facesOverlapped[i] = resultBuffer.indexSet(i);
        }
    }
    qc.releaseFlagVector();
    qc.releaseGeneralPurposeBuffer();
}

cLocalisedConvexShape::cLocalisedConvexShape(
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& expandBy,
        tFace* facesBuffer,
        cFlagVector& faceFlagVector
        ) :
 cPlacedConvexShape(baseShape, baseShapePosition.p, expandBy),
 _centreF(baseShapePosition.f)
{
    FacesOverlappedByOverlappingPolygon_SI(baseShapePosition.f, &_pointVector.front(), _points, facesBuffer, faceFlagVector, _exitsMesh);
    _facesOverlapped.resize(faceFlagVector.numberSet());
    {
        long i;
        for(i = 0; i < faceFlagVector.numberSet(); i++)
        {
            _facesOverlapped[i] = faceFlagVector.indexSet(i);
        }
    }
}

cLocalisedConvexShape::cLocalisedConvexShape(
        const cPlacedConvexShape& baseShape,
        const cInternalPosition& baseShapePosition,
        tFace* facesBuffer,
        cFlagVector& faceFlagVector
        ) :
 cPlacedConvexShape(baseShape),
 _centreF(baseShapePosition.f)
{
    FacesOverlappedByOverlappingPolygon_SI(baseShapePosition.f, &_pointVector.front(), _points, facesBuffer, faceFlagVector, _exitsMesh);
    _facesOverlapped.resize(faceFlagVector.numberSet());
    {
        long i;
        for(i = 0; i < faceFlagVector.numberSet(); i++)
        {
            _facesOverlapped[i] = faceFlagVector.indexSet(i);
        }
    }
}

cLocalisedConvexShape::cLocalisedConvexShape(
        cQueryContext& qc,
        const cInternalPosition& centre,
        long radius
        ) :
 cPlacedConvexShape(centre.p, radius),
 _centreF(centre.f)
{
    tFace* openQueueBuffer = qc.lockFaceBuffer();
    cFlagVector& resultBuffer = qc.lockFaceFlagVector();

    FacesOverlappedByOverlappingPolygon_SI(centre.f, &_pointVector.front(), _points, openQueueBuffer, resultBuffer, _exitsMesh);

    _facesOverlapped.resize(resultBuffer.numberSet());
    {
        long i;
        for(i = 0; i < resultBuffer.numberSet(); i++)
        {
            _facesOverlapped[i] = resultBuffer.indexSet(i);
        }
    }

    qc.releaseFlagVector();
    qc.releaseGeneralPurposeBuffer();
}

bool
cLocalisedConvexShape::overlapsFace(long f) const
{
    long i;
    for(i = 0; i < SizeL(_facesOverlapped); i++)
    {
        if(_facesOverlapped[i] == f)
        {
            return true;
        }
    }
    return false;
}

cInternalPosition
cLocalisedConvexShape::centre() const
{
    return cInternalPosition(_centreF, _centreP);
}

bool
cLocalisedConvexShape::testPointCollision(const cInternalPosition& p) const
{
    if(!pointCollides2D(p.p))
    {
        return false;
    }
    return overlapsFace(p.f.index());
}
bool
cLocalisedConvexShape::testPointCollision(tFace f, const tIntersection& p) const
{
    if(!pointCollides2D(p))
    {
        return false;
    }
    return overlapsFace(f.index());
}
bool
cLocalisedConvexShape::testPointCollision_Inverted(const cInternalPosition& p) const
{
    if(pointCollides2D_Inverted(p.p))
    {
        return true;
    }
    return !overlapsFace(p.f.index());
}
bool
cLocalisedConvexShape::testPointCollision_Inverted(tFace f, const tIntersection& p) const
{
    if(pointCollides2D_Inverted(p))
    {
        return true;
    }
    return !overlapsFace(f.index());
}

bool
cLocalisedConvexShape::testPointIsOnBoundary(tFace f, const tIntersection& p) const
{
    if(!pointIsOnBoundary2D(p))
    {
        return false;
    }
    return overlapsFace(f.index());
}
