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
#include "interface/PlacedConvexShape.h"
#include "libs/Instrumentation/interface/Profiler.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/tIntersection.h"
#include "libs/Geometry/interface/CompareFractions.h"

//static bool
//LineCollidesWithSegment(const tLine& l, const tPoint& axis, const tPoint* segmentPoints)
//{
//    tPoint segmentAxis = segmentPoints[1] - segmentPoints[0];
//    if(segmentAxis.side(axis) != SIDE_RIGHT)
//    {
//        return false;
//    }
//
//    if(l.side(segmentPoints[0]) != SIDE_RIGHT)
//    {
//        return false;
//    }
//    eSide sideForEnd = l.side(segmentPoints[1]);
//    if(sideForEnd == SIDE_RIGHT)
//    {
//        return false;
//    }
//    if(sideForEnd == SIDE_CENTRE)
//    {
//        tPoint nextAxis = segmentPoints[2] - segmentPoints[1];
//        if(nextAxis.side(axis) != SIDE_RIGHT)
//        {
//            return false;
//        }
//    }
//
//    tLine segmentLine(segmentPoints[0], segmentPoints[1]);
//    return segmentLine.side(l.start()) != SIDE_RIGHT && segmentLine.side(l.end()) == SIDE_RIGHT;
//}

static bool
LineCollidesWithSegment(const tLine& l,
        const cAxisFraction& startDistance, const cAxisFraction& endDistance,
                        const tPoint* segmentPoints)
{
    tPoint segmentAxis = segmentPoints[1] - segmentPoints[0];
    if(segmentAxis.side(l.axis()) != SIDE_RIGHT)
    {
        return false;
    }

    if(l.side(segmentPoints[0]) != SIDE_RIGHT)
    {
        return false;
    }
    eSide sideForEnd = l.side(segmentPoints[1]);
    if(sideForEnd == SIDE_RIGHT)
    {
        return false;
    }
    if(sideForEnd == SIDE_CENTRE)
    {
        tPoint nextAxis = segmentPoints[2] - segmentPoints[1];
        if(nextAxis.side(l.axis()) != SIDE_RIGHT)
        {
            return false;
        }
    }

    tLine segmentLine(segmentPoints[0], segmentPoints[1]);
    tIntersection intersection(l, segmentLine);
    cAxisFraction intersectionDistance;
    intersection.getAxisFraction(intersectionDistance.n, intersectionDistance.d);
    return startDistance <= intersectionDistance && endDistance > intersectionDistance;
}

bool
cPlacedConvexShape::edgeSeparates(long edgeIndex, const cPlacedConvexShape& rhs) const
{
    tPoint base = _pointVector[edgeIndex];
    tPoint axis = _pointVector[edgeIndex + 1] - base;
    long i;
    for(i = 0; i < rhs._points; ++i)
    {
        tPoint offset = rhs._pointVector[i] - base;
        if(axis.side(offset) == SIDE_RIGHT)
        {
            return false;
        }
    }
    return true;
}

bool
cPlacedConvexShape::checkStartAndEndForCornerCrossing(
        long cornerIndex,
        const tPoint& lineStart, const tPoint& lineAxis
        ) const
{
    tPoint lineEnd = lineStart + lineAxis;
    tPoint axisAfter = _pointVector[cornerIndex + 1] - _pointVector[cornerIndex];
    if(axisAfter.side(lineEnd - _pointVector[cornerIndex]) != SIDE_RIGHT)
    {
        return false;
    }
    if(axisAfter.side(lineStart - _pointVector[cornerIndex]) == SIDE_RIGHT)
    {
        return false;
    }
    long iPrev = cornerIndex - 1;
    if(iPrev < 0)
    {
        iPrev = _points - 1;
    }
    tPoint axisBefore = _pointVector[cornerIndex] - _pointVector[iPrev];
    if(axisBefore.side(lineEnd - _pointVector[cornerIndex]) != SIDE_RIGHT)
    {
        return false;
    }
    return true;
}

bool
cPlacedConvexShape::pointCollides2D(const tPoint& p) const
{
    long i;
    for(i = 0; i < _points; i++)
    {
        tLine edgeLine(_pointVector[i], _pointVector[i + 1]);
        if(edgeLine.side(p) != SIDE_RIGHT)
        {
            return false;
        }
    }
    return true;
}
bool
cPlacedConvexShape::pointCollides2D(const tIntersection& p) const
{
    long i;
    for(i = 0; i < _points; i++)
    {
        tLine edgeLine(_pointVector[i], _pointVector[i + 1]);
        if(edgeLine.side(p) != SIDE_RIGHT)
        {
            return false;
        }
    }
    return true;
}
bool
cPlacedConvexShape::pointCollides2D_Inverted(const tPoint& p) const
{
    long i;
    for(i = 0; i < _points; i++)
    {
        tLine edgeLine(_pointVector[i], _pointVector[i + 1]);
        if(edgeLine.side(p) == SIDE_LEFT)
        {
            return true;
        }
    }
    return false;
}
bool
cPlacedConvexShape::pointCollides2D_Inverted(const tIntersection& p) const
{
    long i;
    for(i = 0; i < _points; i++)
    {
        tLine edgeLine(_pointVector[i], _pointVector[i + 1]);
        if(edgeLine.side(p) == SIDE_LEFT)
        {
            return true;
        }
    }
    return false;
}
bool
cPlacedConvexShape::pointIsOnBoundary2D(const tIntersection& p) const
{
    long i;
    for(i = 0; i < _points; i++)
    {
        tLine edgeLine(_pointVector[i], _pointVector[i + 1]);
        if(edgeLine.side(p) != SIDE_CENTRE)
        {
            continue;
        }
        long prevI = i - 1;
        if(prevI == -1)
        {
            prevI = _points - 1;
        }
        tLine prevEdgeLine(_pointVector[prevI], _pointVector[i]);
        if(prevEdgeLine.side(p) == SIDE_LEFT)
        {
            return false;
        }
        long nextI = i + 1;
        if(nextI == _points)
        {
            nextI = 0;
        }
        tLine nextEdgeLine(_pointVector[nextI], _pointVector[nextI + 1]);
        if(nextEdgeLine.side(p) == SIDE_LEFT)
        {
            return false;
        }
        return true;
    }
    return false;
}

bool
cPlacedConvexShape::lineCollides2D(const tPoint& lineStart, const tPoint& lineAxis, long& collidingLineIndex) const
{
    long i;
    eSide side = lineAxis.side(_pointVector[0] - lineStart);
    if(side == SIDE_LEFT)
    {
        i = _points - 1;
        do
        {
            side = lineAxis.side(_pointVector[i] - lineStart);
        }
        while(side == SIDE_LEFT && --i);
        if(!i)
        {
            // all points are on left
            return false;
        }
        collidingLineIndex = i;
        if(side == SIDE_CENTRE)
        {
            return checkStartAndEndForCornerCrossing(i, lineStart, lineAxis);
        }
        assertD(side == SIDE_RIGHT);
        tLine segmentLine(_pointVector[i], _pointVector[i + 1]);
        return segmentLine.side(lineStart) != SIDE_RIGHT && segmentLine.side(lineStart + lineAxis) == SIDE_RIGHT;
    }

    if(side == SIDE_CENTRE)
    {
      // first point checked is side centre
        eSide nextSide = lineAxis.side(_pointVector[1] - lineStart);
        if(nextSide != SIDE_RIGHT)
        {
            if(nextSide == SIDE_CENTRE)
            {
                return false;
            }
            collidingLineIndex = 0;
            return checkStartAndEndForCornerCrossing(0, lineStart, lineAxis);
        }
    }

    i = 1;
    do
    {
        side = lineAxis.side(_pointVector[i] - lineStart);
    }
    while(side == SIDE_RIGHT && ++i != _points);
    if(i == _points)
    {
        // all points are on right
        return false;
    }
    if(side == SIDE_CENTRE)
    {
        collidingLineIndex = i;
        return checkStartAndEndForCornerCrossing(i, lineStart, lineAxis);
    }
    assertD(side == SIDE_LEFT);
    tLine segmentLine(_pointVector[i - 1], _pointVector[i]);
    collidingLineIndex = i - 1;
    return segmentLine.side(lineStart) != SIDE_RIGHT && segmentLine.side(lineStart + lineAxis) == SIDE_RIGHT;
}
//bool
//cPlacedConvexShape::lineCollides2D_Old(const tLine& l, long& collidingLineIndex) const
//{
//    PROFILER_SCOPE("cPlacedConvexShape::lineCollides2D");
//    tPoint axis(l.axis());
//    long i;
//    for(i = 0; i < _points; i++)
//    {
//        if(LineCollidesWithSegment(l, axis, &_pointVector[i]))
//        {
//            collidingLineIndex = i;
//            return true;
//        }
//    }
//    return false;
//}
//bool
//cPlacedConvexShape::lineCollides2D(const tPoint& lineStart, const tPoint& lineAxis, long& collidingLineIndex) const
//{
//    long index1, index2;
//    bool result1, result2;
//    result1 = lineCollides2D_New(lineStart, lineAxis, index1);
//    result2 = lineCollides2D_Old(tLine(lineStart, lineStart + lineAxis), index2);
//    assertD(result1 == result2);
//    if(result1)
//    {
//        assertD(index1 == index2);
//    }
//    collidingLineIndex = index1;
//    return result1;
//}

bool
cPlacedConvexShape::lineCollides2D(const tLine& l,
        const cAxisFraction& startDistance, const cAxisFraction& endDistance,
                                  long& collidingLineIndex) const
{
    long i;
    for(i = 0; i < _points; i++)
    {
        if(LineCollidesWithSegment(l, startDistance, endDistance, &_pointVector[i]))
        {
            collidingLineIndex = i;
            return true;
        }
    }
    return false;
}

void
cPlacedConvexShape::postConstructor_Common()
{
    // repeat start points at end to speed up collision checking
    _pointVector[_points] = _pointVector[0];
    _pointVector[_points + 1] = _pointVector[1];
}

cPlacedConvexShape::cPlacedConvexShape(
        const cCollisionShape& baseShape,
        const tPoint& baseShapeCentre,
        const cCollisionShape& expandBy) :
 _centreP(baseShapeCentre)
{
//.. repeats functionality in cCollisionShape (for efficiency)
    size_t maxPoints = baseShape.size() + expandBy.size();
    _pointVector.resize(maxPoints + 2);

    _points = 0;
    long i;
    long prevEndExpand;
    {
        tPoint edgeVector = baseShape.getEdgeAxis(baseShape.size() - 1);
        prevEndExpand = expandBy.endExpand(edgeVector);
    }
    for(i = 0; i < baseShape.size(); i++)
    {
        tPoint edgeVector = baseShape.getEdgeAxis(i);
        long startExpand, endExpand;
        expandBy.getStartEndExpand(edgeVector, startExpand, endExpand);
        do
        {
            _pointVector[_points] = baseShapeCentre + baseShape.getVertex(i) + expandBy.expandOffset(prevEndExpand);
            _points++;
            prevEndExpand = expandBy.nextVertex(prevEndExpand);
        }
        while(prevEndExpand != expandBy.nextVertex(startExpand));
        prevEndExpand = endExpand;
    }
    
    postConstructor_Common();
}

cPlacedConvexShape::cPlacedConvexShape(
        const cCollisionShape& baseShape,
        const tPoint& baseShapeCentre
        ) :
 _centreP(baseShapeCentre)
{
    _points = baseShape.size();
    _pointVector.resize(_points + 2);
    long i;
    for(i = 0; i < _points; i++)
    {
        _pointVector[i] = baseShapeCentre + baseShape.getVertex(i);
    }    
    postConstructor_Common();
}

cPlacedConvexShape::cPlacedConvexShape(const tPoint& centre, long radius) :
 _centreP(centre),
 _pointVector(6)
{
     _points = 4;
    _pointVector[0] = centre + tPoint(-radius, -radius, radius);
    _pointVector[1] = centre + tPoint(-radius, radius, radius);
    _pointVector[2] = centre + tPoint(radius, radius, radius);
    _pointVector[3] = centre + tPoint(radius, -radius, radius);
    postConstructor_Common();
}

//cPlacedConvexShape::cPlacedConvexShape(std::vector<tPoint>& points)
//{
//    assertD(points.size() >= 3);
//    _points = points.size();
//    points.resize(points.size() + 2);
//    _pointVector.swap(points);
//    postConstructor_Common();
//}

tLine
cPlacedConvexShape::getBoundaryLine(long index) const
{
    return tLine(_pointVector[index], _pointVector[index + 1]);
}
tLine
cPlacedConvexShape::getBoundaryLine_Reversed(long index) const
{
    return tLine(_pointVector[index + 1], _pointVector[index ]);
}


//long
//cPlacedConvexShape::getIndexForSilhouettePoint(eSide side, const tPoint& p) const
//{
//    assertD(!pointCollides2D(p));
//    long i;
//    long result = -1;
//    tPoint bestOffset;
//    for(i = 0; i < sides(); ++i)
//    {
//        if(_pointVector[i] == p)
//        {
//            continue;
//        }
//        if(result == -1)
//        {
//            result = i;
//            bestOffset = _pointVector[i] - p;
//            continue;
//        }
//        tPoint offset = _pointVector[i] - p;
//        eSide sideOfBest = bestOffset.side(offset);
//        if(sideOfBest == SIDE_CENTRE)
//        {
//            long nextI = i + 1;
//            if(nextI == sides())
//            {
//                nextI = 0;
//            }
//            long prevI = i;
//            if(prevI == 0)
//            {
//                prevI = sides();
//            }
//            --prevI;
//            if(side == SIDE_RIGHT && result == nextI)
//            {
//                sideOfBest = side;
//            }
//            else
//            if(side == SIDE_LEFT && result == prevI)
//            {
//                sideOfBest = side;
//            }
//        }
//        else if(sideOfBest == side)
//        {
//            result = i;
//            bestOffset = offset;
//        }
//    }
//    return result;
//}

long
cPlacedConvexShape::getIndexForSilhouettePoint(eSide side, const tPoint& p) const
{
    assertD(!pointCollides2D(p));
    if(side == SIDE_LEFT)
    {
    // find the last edge that p is not on the right of
        long i = 0;
        while(tLine(_pointVector[i], _pointVector[i + 1]).side(p) == SIDE_RIGHT)
        {
            ++i;
            assertD(i < sides());
        }
        do
        {
            ++i;
            if(i == sides())
            {
                i = 0;
            }
        }
        while(tLine(_pointVector[i], _pointVector[i + 1]).side(p) != SIDE_RIGHT);
        // return the index of the point after this last edge
        return i;
    }

    assertD(side == SIDE_RIGHT);
// find the last edge that p is on the right of
    long i = 0;
    while(tLine(_pointVector[i], _pointVector[i + 1]).side(p) != SIDE_RIGHT)
    {
        ++i;
        assertD(i < sides());
    }
    do
    {
        ++i;
        if(i == sides())
        {
            i = 0;
        }
    }
    while(tLine(_pointVector[i], _pointVector[i + 1]).side(p) == SIDE_RIGHT);
    // return the index of the point after this last edge
    return i;
}

bool
cPlacedConvexShape::enters_NotAtCorner(const cPlacedConvexShape& rhs, long& lhsSection, long& rhsSection) const
{
    for(lhsSection = 0; lhsSection < _points; lhsSection++)
    {
        tLine lhsSectionLine(_pointVector[lhsSection], _pointVector[lhsSection + 1]);
        for(rhsSection = 0; rhsSection < rhs._points; rhsSection++)
        {
            tLine rhsSectionLine(rhs._pointVector[rhsSection], rhs._pointVector[rhsSection + 1]);
            if(rhsSectionLine.side(lhsSectionLine.start()) == SIDE_LEFT
             && rhsSectionLine.side(lhsSectionLine.end()) == SIDE_RIGHT
             && lhsSectionLine.side(rhsSectionLine.start()) == SIDE_RIGHT
             && lhsSectionLine.side(rhsSectionLine.end()) == SIDE_LEFT)
            {
                return true;
            }
        }
    }
    return false;
}
bool
cPlacedConvexShape::exits_NotAtCorner(const cPlacedConvexShape& rhs, long& lhsSection, long& rhsSection) const
{
    for(lhsSection = 0; lhsSection < _points; lhsSection++)
    {
        tLine lhsSectionLine(_pointVector[lhsSection], _pointVector[lhsSection + 1]);
        for(rhsSection = 0; rhsSection < rhs._points; rhsSection++)
        {
            tLine rhsSectionLine(rhs._pointVector[rhsSection], rhs._pointVector[rhsSection + 1]);
            if(rhsSectionLine.side(lhsSectionLine.start()) == SIDE_RIGHT
             && rhsSectionLine.side(lhsSectionLine.end()) == SIDE_LEFT
             && lhsSectionLine.side(rhsSectionLine.start()) == SIDE_LEFT
             && lhsSectionLine.side(rhsSectionLine.end()) == SIDE_RIGHT)
            {
                return true;
            }
        }
    }
    return false;
}

bool
cPlacedConvexShape::overlaps2D(const cPlacedConvexShape& rhs) const
{
    long i;
    for(i = 0; i < _points; ++i)
    {
        if(edgeSeparates(i, rhs))
        {
            return false;
        }
    }
    for(i = 0; i < rhs._points; ++i)
    {
        if(rhs.edgeSeparates(i, *this))
        {
            return false;
        }
    }
    return true;
}

bool
cPlacedConvexShape::overlaps2D(const cPlacedConvexShape& rhs, tIntersection& intersection) const
{
    if(pointCollides2D(rhs._centreP))
    {
        intersection = rhs._centreP;
        return true;
    }
    const cPlacedConvexShape& lhs = *this;
    if(rhs.pointCollides2D(lhs._centreP))
    {
        intersection = lhs._centreP;
        return true;
    }
    long i;
    for(i = 0; i != sides(); ++i)
    {
        long collidingLineIndex;
        if(!rhs.lineCollides2D(
                _pointVector[i],
                _pointVector[i + 1] - _pointVector[i],
                collidingLineIndex
                )
            )
        {
            continue;
        }
        intersection = tIntersection(getBoundaryLine(i), rhs.getBoundaryLine(collidingLineIndex));
        return true;
    }
    return false;
}

bool
cPlacedConvexShape::satisfiesMaxCircumference(long maxCircumference) const
{
    long circumference = 0;
    long i;
    for(i = 0; i != sides(); ++i)
    {
        tPoint v = _pointVector[i + 1] - _pointVector[i];
        if(v.getRangeUsed() > maxCircumference)
        {
            return false;
        }
        circumference += Length(v);
        if(circumference > maxCircumference)
        {
            return false;
        }
    }
    return true;
}
