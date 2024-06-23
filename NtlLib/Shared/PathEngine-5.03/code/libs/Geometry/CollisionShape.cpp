//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/CollisionShape.h"
#include "Polygon.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "libs/Geometry/interface/tLine.h"
#include "platform_common/CoordinateRanges.h"
#include "common/interface/Assert.h"
#include "common/interface/iOutputStream_Operators.h"
#include "common/STL/vector.h"

cCollisionShape::cCollisionShape()
{
    _vertexArray = 0;
}

cCollisionShape::cCollisionShape(
        long numberOfPoints,
        const long* coordinateData
        )
{
    _vertices = numberOfPoints;
    _vertexArray = new tPoint[numberOfPoints];
    long i;
    for(i = 0; i != numberOfPoints; ++i)
    {
        _vertexArray[i] = tPoint(coordinateData[i * 2], coordinateData[i * 2 + 1], SHAPE_RANGE);
    }
#ifdef ASSERTIONS_ON
    validate();
#endif
}

cCollisionShape::cCollisionShape(
        long numberOfPoints,
        const tPoint* pointArray
        )
{
    _vertices = numberOfPoints;
    _vertexArray = new tPoint[numberOfPoints];
    long i;
    for(i = 0; i != numberOfPoints; ++i)
    {
        _vertexArray[i] = tPoint(pointArray[i].getX(), pointArray[i].getY(), SHAPE_RANGE);
    }
#ifdef ASSERTIONS_ON
    validate();
#endif
}

cCollisionShape::cCollisionShape(
        const cCollisionShape& base,
        const cCollisionShape& expandBy
        )
{
    long maxPoints = base.size() + expandBy.size();
    std::vector<tPoint> vertexVector(maxPoints);
    long points = 0;
    long i;
    long prevEndExpand;
    {
        tPoint edgeVector = base.getEdgeAxis(base.size() - 1);
        prevEndExpand = expandBy.endExpand(edgeVector);
    }
    for(i = 0; i < base.size(); ++i)
    {
        tPoint edgeVector = base.getEdgeAxis(i);
        long startexpand, endExpand;
        expandBy.getStartEndExpand(edgeVector, startexpand, endExpand);
        do
        {
            tPoint p = base.getVertex(i) + expandBy.expandOffset(prevEndExpand);
            vertexVector[points++] = tPoint(p.getX(), p.getY(), SHAPE_RANGE);
            prevEndExpand = expandBy.nextVertex(prevEndExpand);
        } 
        while(prevEndExpand != expandBy.nextVertex(startexpand));
        prevEndExpand = endExpand;
    }
    _vertices = points;
    _vertexArray = new tPoint[points];
    for(i = 0; i != points; ++i)
    {
        _vertexArray[i] = vertexVector[i];
    }
#ifdef ASSERTIONS_ON
    validate();
#endif
}

cCollisionShape::cCollisionShape(const cCollisionShape& rhs)
{
    if(rhs._vertexArray == 0)
    {
        _vertexArray = 0;
        return;
    }
    _vertices = rhs.size();
    _vertexArray = new tPoint[_vertices];
    long i;
    for(i = 0; i != _vertices; ++i)
    {
        _vertexArray[i] = rhs.getVertex(i);
    }
}

cCollisionShape&
cCollisionShape::operator=(const cCollisionShape& rhs)
{
    if(this == &rhs)
    {
        return *this;
    }
    delete [] _vertexArray;
    if(rhs._vertexArray == 0)
    {
        _vertexArray = 0;
        return *this;
    }
    _vertices = rhs.size();
    _vertexArray = new tPoint[_vertices];
    long i;
    for(i = 0; i != _vertices; ++i)
    {
        _vertexArray[i] = rhs.getVertex(i);
    }
    return *this;
}

cCollisionShape::~cCollisionShape()
{
    delete [] _vertexArray;
}

void
cCollisionShape::swap(cCollisionShape& rhs)
{
    std::swap(_vertices, rhs._vertices);
    std::swap(_vertexArray, rhs._vertexArray);
}

void
cCollisionShape::validate()
{
    assertD(PolygonIsConvex(_vertices, _vertexArray));
    assertD(PointInPolygon(_vertices, _vertexArray, tPoint(0,0,0)));
}

void
cCollisionShape::extrude(const tPoint& extrudeVector, cCollisionShape& result) const
{
    std::vector<tPoint> vertexVector(size() + 2);
    vertexVector.resize(0);
    long i;
    for(i = 0; i != size(); ++i)
    {
        if(getEdgeAxis(i).side(extrudeVector) == SIDE_RIGHT
         && getEdgeAxis(nextVertex(i)).side(extrudeVector) != SIDE_RIGHT
            )
        {
            break;
        }
    }
    assertD(i != size());
    long endI = i;
    i = nextVertex(i);
    vertexVector.push_back(_vertexArray[i]);
    if(getEdgeAxis(i).side(extrudeVector) != SIDE_CENTRE)
    {
        vertexVector.push_back(_vertexArray[i] + extrudeVector);
    }
    do
    {
        i = nextVertex(i);
        vertexVector.push_back(_vertexArray[i] + extrudeVector);
    }
    while(getEdgeAxis(i).side(extrudeVector) == SIDE_LEFT);
    if(getEdgeAxis(i).side(extrudeVector) != SIDE_CENTRE)
    {
        vertexVector.push_back(_vertexArray[i]);
    }
    while(i != endI)
    {
        i = nextVertex(i);
        vertexVector.push_back(_vertexArray[i]);
    }

    delete [] result._vertexArray;
    result._vertices = SizeL(vertexVector);
    result._vertexArray = new tPoint[result._vertices];
    for(i = 0; i != result._vertices; ++i)
    {
        result._vertexArray[i] = tPoint(vertexVector[i].getX(), vertexVector[i].getY(), SHAPE_RANGE);
    }

#ifdef ASSERTIONS_ON
    result.validate();
#endif
}

void
cCollisionShape::checksum(iOutputStream& os) const
{
    os << size();
    long i;
    for(i = 0; i < size(); ++i)
    {
        os << _vertexArray[i].getX();
        os << _vertexArray[i].getY();
    }
}

long
cCollisionShape::size() const
{
    if(_vertexArray == 0)
    {
        return 0;
    }
    return _vertices;
}

bool
cCollisionShape::operator==(const cCollisionShape& rhs) const
{
    if(size() != rhs.size())
    {
        return false;
    }
    long i;
    for(i = 0; i != size(); ++i)
    {
        if(getVertex(i) != rhs.getVertex(i))
        {
            return false;
        }
    }
    return true;
}

void cCollisionShape::getStartEndExpand(const tPoint &edgeVector, long &start, long &end) const
{
    long i, nextI;
    for(i = 0; i < size(); i++)
    {
        nextI = nextVertex(i);
        if(expandEdgeVector(i).direction(edgeVector) == DIRECTION_FORWARD)
        {
            start = i;
            end = nextI;
            return;
        }
        if(expandEdgeVector(i).side(edgeVector) == SIDE_RIGHT && 
            expandEdgeVector(nextI).side(edgeVector) == SIDE_LEFT)
        {
            start = nextI;
            end = nextI;
            return;
        }
    }
    invalid();
}
long cCollisionShape::getEdgesForCorner(const tPoint& previousEdge, const tPoint& nextEdge) const
{
    eDirection dir;
    long previousStart;
    long previousEnd;
    long nextStart;
    long nextEnd;
    long edges;

    dir = previousEdge.direction(nextEdge);
    assertD(dir != DIRECTION_BACK);

    getStartEndExpand(previousEdge, previousStart, previousEnd);
    getStartEndExpand(nextEdge, nextStart, nextEnd);

    edges = 0;

    if(dir == DIRECTION_LEFT || dir == DIRECTION_FORWARD)
    {
    // concave corner
    // (or edges are in line)
    // number of interpolated edges will be 0 or negative
        while(nextStart != previousEnd)
        {
            nextStart = nextVertex(nextStart);
            edges--;
        }
    }
    else
    {
    // convex corner
        while(nextStart != previousEnd)
        {
            previousEnd = nextVertex(previousEnd);
            edges++;
        }
    }
    return edges;
}

void cCollisionShape::fillCoordinateArray(long* coords) const
{
    long i;
    for(i = 0; i < size(); i++)
    {
        tPoint p = getVertex(i);
        *coords++ = p.getX();
        *coords++ = p.getY();
    }    
}

bool cCollisionShape::pointIsInside(const tPoint& p) const
{
    long i;
    for(i = 0; i < size(); i++)
    {
        tLine l;
        getEdgeLine(i, l);
        if(l.side(p) != SIDE_RIGHT)
        {
            return false;
        }
    }
    return true;
}


long
cCollisionShape::getEdgesForCorner(
        const tPoint& previousAxis, const tPoint& nextAxis,
        long spinCount
        ) const
{
    assertD(spinCount >= 0);

    long previousEnd;
    long nextStart;
    {
        long previousStart;
        long nextEnd;
        getStartEndExpand(previousAxis, previousStart, previousEnd);
        getStartEndExpand(nextAxis, nextStart, nextEnd);
    }

    long edges;
    edges = 0;

    if(previousAxis.side(nextAxis) != SIDE_RIGHT)
    {
    // concave corner
    // (or in line)
    // number of interpolated edges will be 0 or negative
        while(nextStart != previousEnd)
        {
            nextStart = nextVertex(nextStart);
            edges--;
        }
    }
    else
    {
    // convex corner
    // number of interpolated edges will be 0 or positive
        while(nextStart != previousEnd)
        {
            previousEnd = nextVertex(previousEnd);
            edges++;
        }
    }

    edges += size() * (spinCount / 2);

    return edges;
}

long
cCollisionShape::getEdgesForCorner_DontConnectInline(
        const tPoint& previousAxis, const tPoint& nextAxis,
        long spinCount
        ) const
{
    assertD(spinCount >= 0);

    long previousEnd = startExpand(previousAxis);
    long nextStart = startExpand(nextAxis);

    long edges;
    edges = 0;

    if(previousAxis.side(nextAxis) != SIDE_RIGHT)
    {
    // concave corner
    // (or in line)
    // number of interpolated edges will be 0 or negative
        while(nextStart != previousEnd)
        {
            nextStart = nextVertex(nextStart);
            edges--;
        }
    }
    else
    {
    // convex corner
    // number of interpolated edges will be 0 or positive
        while(nextStart != previousEnd)
        {
            previousEnd = nextVertex(previousEnd);
            edges++;
        }
    }

    edges += size() * (spinCount / 2);

    return edges;
}

bool
cCollisionShape::polygonIsConvex(long numberOfPoints, const tPoint* coords)
{
    return PolygonIsConvex(numberOfPoints, coords);
}

bool
cCollisionShape::pointInPolygon(long numberOfPoints, const tPoint* coords, const tPoint& testPoint)
{
    return PointInPolygon(numberOfPoints, coords, testPoint);
}

void
cCollisionShape::getEdgeLine(long index, tLine& result) const
{
    result = tLine(getVertex(index), getVertex(nextVertex(index)));
}

void
cCollisionShape::normalise(cCollisionShape& result) const
{
    long i, leftMost;
    leftMost = 0;
    tPoint leftMostV = getVertex(0);
    for(i = 1; i < size(); ++i)
    {
        tPoint v = getVertex(i);
        if(v.getX() < leftMostV.getX()
         || (v.getX() == leftMostV.getX() && v.getY() < leftMostV.getY()))
        {
            leftMost = i;
            leftMostV = v;
        }
    }
    std::vector<tPoint> points(size());
    long j = leftMost;
    for(i = 0; i < size(); ++i)
    {
        points[i] = getVertex(j);
        j = nextVertex(j);
    }
    result = cCollisionShape(size(), &points.front());
}

void
cCollisionShape::getBounds(tPoint& minimum, tPoint& maximum) const
{
    long i;
    minimum = getVertex(0);
    maximum = minimum;
    for(i = 1; i != size(); ++i)
    {
        minimum.updateAsMinimumBound(getVertex(i));
        maximum.updateAsMaximumBound(getVertex(i));
    }
}

