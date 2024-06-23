//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef LOCALISED_CONVEX_SHAPE_INCLUDED
#define LOCALISED_CONVEX_SHAPE_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/PathEngine_Core/interface/PlacedConvexShape.h"
#include "libs/Mesh2D/interface/InternalPosition.h"

class cFlagVector;
class cQueryContext;
template <class T> class cCollapsedVectorVector;
class cAgent;

class cLocalisedConvexShape : public cPlacedConvexShape
{
    std::vector<long> _facesOverlapped;
    tFace _centreF;
    bool _exitsMesh;

public:

    cLocalisedConvexShape(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& expandBy
        );

    cLocalisedConvexShape(
        cQueryContext& qc,
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition
        );

    cLocalisedConvexShape(
        const cCollisionShape& baseShape,
        const cInternalPosition& baseShapePosition,
        const cCollisionShape& expandBy,
        tFace* facesBuffer,
        cFlagVector& faceFlagVector
        );

    cLocalisedConvexShape(
        const cPlacedConvexShape& baseShape,
        const cInternalPosition& baseShapePosition,
        tFace* facesBuffer,
        cFlagVector& faceFlagVector
        );

    cLocalisedConvexShape(
        cQueryContext& qc,
        const cInternalPosition& centre,
        long radius
        );

    bool exitsMesh() const
    {
        return _exitsMesh;
    }
    bool overlapsFace(long f) const;
    cInternalPosition centre() const;

    const tPoint* dataForSideElement(long side) const
    {
        return &_pointVector.front() + side;
    }

    const std::vector<long>& refFacesOverlapped() const
    {
        return _facesOverlapped;
    }

    bool testPointCollision(const cInternalPosition& p) const;
    bool testPointCollision(tFace f, const tPoint& p) const
    {
        return testPointCollision(cInternalPosition(f, p));
    }
    // the intersection must be on its axis
    bool testPointCollision(tFace f, const tIntersection& p) const;

    bool testPointCollision_Inverted(const cInternalPosition& p) const;
    // the intersection must be on its axis
    bool testPointCollision_Inverted(tFace f, const tIntersection& p) const;

    bool testPointIsOnBoundary(tFace f, const tIntersection& p) const;
};

#endif
