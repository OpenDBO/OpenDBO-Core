//**********************************************************************
//
// Copyright (c) 2002-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef COLLISIONSHAPE_INCLUDED
#define COLLISIONSHAPE_INCLUDED

#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine_Header.h"
//#include "common/STL/vector.h"

class iOutputStream;

class cCollisionShape
{
    long _vertices;
    tPoint* _vertexArray;

public:

    cCollisionShape();

    cCollisionShape(
            long numberOfPoints,
            const long* coordinateArray
            );

    cCollisionShape(
            long points,
            const tPoint* pointArray
            );

    // construct as sum of two other shapes
    cCollisionShape(
            const cCollisionShape& base,
            const cCollisionShape& expandby
            );

    cCollisionShape(const cCollisionShape& rhs);

    cCollisionShape& operator=(const cCollisionShape& rhs);

    ~cCollisionShape();

    void swap(cCollisionShape& rhs);

    void validate();

    void extrude(const tPoint& extrudeVector, cCollisionShape& result) const;

    void checksum(iOutputStream& os) const;

    long size() const;

    bool operator==(const cCollisionShape& rhs) const;

    tPoint expandOffset(long index) const
    {
        return -_vertexArray[index];
    }
    tPoint getVertex(long index) const
    {
        return _vertexArray[index];
    }
    tPoint expandEdgeVector(long index) const
    {
        return -(_vertexArray[nextVertex(index)] - _vertexArray[index]);
    }

    tPoint getEdgeAxis(long index) const
    {
        return getVertex(nextVertex(index))-getVertex(index);
    }

    long nextVertex(const long vertex) const
    {
        if(vertex == size() - 1)
        {
            return 0;
        }
        return vertex + 1;
    }
    long prevVertex(const long vertex) const
    {
        if(vertex == 0)
        {
            return size() - 1;
        }
        return vertex - 1;
    }

    void getEdgeLine(long index, tLine& result) const;

    void getStartEndExpand(const tPoint &edgeVector, long &start, long &end) const;

    long endExpand(const tPoint& edgeVector) const
    {
        long start, end;
        getStartEndExpand(edgeVector, start, end);
        return end;
    }
    //.... optimise
    long startExpand(const tPoint& edgeVector) const
    {
        long start, end;
        getStartEndExpand(edgeVector, start, end);
        return start;
    }

    long getEdgesForCorner(const tPoint& previousEdge, const tPoint& nextEdge) const;
    long getEdgesForCorner(
            const tPoint& previousAxis, const tPoint& nextAxis,
            long spinCount
            ) const;
    long getEdgesForCorner_DontConnectInline(
            const tPoint& previousAxis, const tPoint& nextAxis,
            long spinCount
            ) const;

    void fillCoordinateArray(long* coords) const;

    bool pointIsInside(const tPoint& p) const;

    static bool polygonIsConvex(long numberOfPoints, const tPoint* coords);
    static bool pointInPolygon(long numberOfPoints, const tPoint* coords, const tPoint& testPoint);

    void normalise(cCollisionShape& result) const;

    void getBounds(tPoint& minimum, tPoint& maximum) const;
};

#endif

