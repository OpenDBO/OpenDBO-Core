//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PATHENGINE_CORE__PLACED_CONVEX_SHAPE_INCLUDED
#define PATHENGINE_CORE__PLACED_CONVEX_SHAPE_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine_Header.h"
#include "libs/Geometry/interface/tIntersection_Header.h"
#include "common/STL/vector.h"

class cCollisionShape;
class cAxisFraction;

class cPlacedConvexShape
{
    bool edgeSeparates(long edgeIndex, const cPlacedConvexShape& rhs) const;
    bool checkStartAndEndForCornerCrossing(long cornerIndex, const tPoint& lineStart, const tPoint& lineAxis) const;
    void postConstructor_Common();

protected:

    tPoint _centreP;
    std::vector<tPoint> _pointVector;
    long _points;

public:

    bool pointCollides2D(const tPoint& p) const;
    bool pointCollides2D(const tIntersection& p) const;
    bool pointCollides2D_Inverted(const tPoint& p) const;
    bool pointCollides2D_Inverted(const tIntersection& p) const;
    bool pointIsOnBoundary2D(const tIntersection& p) const;

    bool lineCollides2D(const tPoint& lineStart, const tPoint& lineAxis, long& collidingLineIndex) const;

    bool
    lineCollides2D(
            const tLine& l,
            const cAxisFraction& startDistance, const cAxisFraction& endDistance,
            long& collidingLineIndex
            ) const;

    cPlacedConvexShape(const cCollisionShape& baseShape,
                        const tPoint& baseShapeCentre,
                        const cCollisionShape& expandBy);
    cPlacedConvexShape(const cCollisionShape& baseShape,
                        const tPoint& baseShapeCentre
                        );
    cPlacedConvexShape(const tPoint& centre, long radius);

    tLine getBoundaryLine(long index) const;
    tLine getBoundaryLine_Reversed(long index) const;
    long sides() const
    {
        return _points;
    }
    const tPoint& operator[](long index) const
    {
        return _pointVector[index];
    }

    const std::vector<tPoint>& refPointVector() const
    {
        return _pointVector;
    }

    long getIndexForSilhouettePoint(eSide side, const tPoint& p) const;

    bool enters_NotAtCorner(const cPlacedConvexShape& rhs, long& lhsSection, long& rhsSection) const;
    bool exits_NotAtCorner(const cPlacedConvexShape& rhs, long& lhsSection, long& rhsSection) const;

    bool overlaps2D(const cPlacedConvexShape& rhs) const;
    bool overlaps2D(const cPlacedConvexShape& rhs, tIntersection& intersection) const;

    bool satisfiesMaxCircumference(long maxCircumference) const;
};

#endif
