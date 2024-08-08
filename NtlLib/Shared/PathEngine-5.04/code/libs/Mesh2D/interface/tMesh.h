//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TMESH_INCLUDED
#define TMESH_INCLUDED

#include "libs/Mesh2D/interface/tMesh_Iterators.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/TemplateMesh/VectorMesh.h"

class cE
{
public:

//    long edgeindex;

//    bool operator==(const cE& rhs) const
//    {
//        return edgeindex == rhs.edgeindex;
//    }
//    bool operator!=(const cE& rhs) const
//    {
//        return !(*this == rhs);
//    }
};
class cV
{
    long _elementOn3D;

public:

//    long vertexindex;
    tPoint _p;

    cV()
    {
    }
    explicit cV(const tPoint& p);

    void
    setElementOn3DIndex(long value)
    {
        _elementOn3D = value;
    }

    bool operator==(const cV& rhs) const
    {
        return _elementOn3D == rhs._elementOn3D
//            && vertexindex == rhs.vertexindex
            && _p == rhs._p;
    }
    bool operator!=(const cV& rhs) const
    {
        return !(*this == rhs);
    }

    long getTargetVertexIndex() const;
    long getElementOn3DIndex() const
    {
        return _elementOn3D;
    }

    tPoint getPoint() const
    {
        return _p;
    }
};
class cF
{
    bool _external;

public:

//    long index;

    cF()
    {
        _external = false;
//        index = -1;
    }

    bool operator==(const cF& rhs) const
    {
//        return _external == rhs._external && index == rhs.index;
        return _external == rhs._external;
    }
    bool operator!=(const cF& rhs) const
    {
        return !(*this == rhs);
    }

    bool isExternal() const
    {
        return _external;
    }

    static cF externalFace()
    {
        cF result;
        result._external = true;
        return result;
    }
};

inline tPoint GetVertexPoint(tMesh::edge e)
{
    return e.vertex()->getPoint();
}
inline tPoint GetVertexPoint(tMesh::vertex v)
{
    return v->getPoint();
}
inline tLine GetEdgeLine(tMesh::edge e)
{
    return tLine(GetVertexPoint(e),GetVertexPoint(e.next()));
}
inline tPoint GetEdgeAxis(tMesh::edge e)
{
    return GetVertexPoint(e.next())-GetVertexPoint(e) ;
}

inline cE MakeEdge(tMesh::edge e, const tLine &l)
{
    return *e;
}

#endif
