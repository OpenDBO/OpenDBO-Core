//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COLLISION_AGAINST_SHAPE_SET_INCLUDED
#define COLLISION_AGAINST_SHAPE_SET_INCLUDED

#include "libs/PathEngine_Core/interface/LocalisedConvexShape.h"
#include "libs/Mesh2D/interface/InternalPosition.h"
#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "common/STL/vector.h"

class cQueryContext;
class cLocalisedConvexShapeSet;
class cLocalisedConvexShape;
class cAxisFraction;
class cInternalPosition;
class cAgent;
class cBitVector;

bool
LineCollision_AgainstShapeSet(
        tFace startF, const tLine& testLine,
        const cLocalisedConvexShapeSet& shapeSet,
        bool reportFirst,
        long& collidingShape,
        long& collidingLineIndex,
        cAxisFraction& distanceAlong,
        tFace& endF
        );

bool
TestForSnagsToSegmentEnd_AgainstShapeSet(
        tFace startF, const tLine& testLine,
        const cLocalisedConvexShapeSet& shapeSet,
        const tLine& segmentLine
        );

bool
LineCollision_AgainstShapeSet(
        tFace startF, const tLine& testLine,
        const cAxisFraction& startFraction, const cAxisFraction& endFraction,
        const cLocalisedConvexShapeSet& shapeSet,
        long& collidingShape,
        long& collidingLineIndex,
        cAxisFraction& distanceAlong
        );

bool
CollisionToXY_AgainstShapeSet(
        cQueryContext& qc,
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF, const tLine& line, tFace& endF
        );

bool
PointCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        const cInternalPosition& p,
        long& collidingShape
        );
bool
PointCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        long faceIndex, const tIntersection& p,
        long& collidingShape
        );
//bool
//PointCollision_AgainstShapeSet_IgnoringOneAgent(
//        const cLocalisedConvexShapeSet& shapeSet,
//        const cInternalPosition& p,
//        const cAgent* agentToIgnore
//        );
bool
PointCollision_AgainstShapeSet_IgnoringOneShape(
        const cLocalisedConvexShapeSet& shapeSet,
        const cInternalPosition& p,
        long shapeToIgnore
        );

void
FirstCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF, const tLine& line, bool& collides, cAxisFraction& distanceAlong, tLine& collidingLine,
        long& collidingShape
        );
void
FirstCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF,
        const tLine& line, const cAxisFraction& startFraction, const cAxisFraction& endFraction,
        bool& collides, cAxisFraction& distanceAlong, tLine& collidingLine,
        long& collidingShape
        );

void
LastCollision_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        tFace startF, const tLine& testLine, bool& collides, cAxisFraction& distanceAlong
        );
void
GetAllShapesOverlapped_AgainstShapeSet(
        const cLocalisedConvexShapeSet& shapeSet,
        const cInternalPosition& p, cBitVector& isOverlapped
        );

//long
//GetAllAgentsOverlapped_AgainstShapeSet(
//        const cLocalisedConvexShapeSet& shapeSet,
//        const cInternalPosition& p,
//        cAgent** resultsBuffer
//        );

// only used for small convex shape set, at time of writing
// so doesn't need to support compound shape sets
// or to be fast against compount shape sets
void
GetShapesPotentiallyOverlappedBy(
        const cLocalisedConvexShapeSet& shapeSet,
        const cLocalisedConvexShape& shape, std::vector<long>& appendTo
        );

template <class tShapeSet> bool
PointCollision_AgainstShapeSet(
        const tShapeSet& shapeSet,
        long faceIndex, const tIntersection& p,
        const cLocalisedConvexShape*& collidingShapePtr
        )
{
    typename tShapeSet::tShapesInFaceIterator it;
    shapeSet.initShapesInFaceIterator(faceIndex, it);
    const cLocalisedConvexShape* candidateShapePtr;
    while(shapeSet.nextShapeInFace(it, candidateShapePtr))
    {
        if(candidateShapePtr->pointCollides2D(p))
        {
            collidingShapePtr = candidateShapePtr;
            return true;
        }
    }
    return false;
}

template <class tShapeSet> bool
PointCollision_AgainstShapeSet(
        const tShapeSet& shapeSet,
        const cInternalPosition& p
        )
{
    typename tShapeSet::tShapesInFaceIterator it;
    shapeSet.initShapesInFaceIterator(p.cell(), it);
    const cLocalisedConvexShape* candidateShapePtr;
    while(shapeSet.nextShapeInFace(it, candidateShapePtr))
    {
        if(candidateShapePtr->pointCollides2D(p.p))
        {
            return true;
        }
    }
    return false;
}

#endif
