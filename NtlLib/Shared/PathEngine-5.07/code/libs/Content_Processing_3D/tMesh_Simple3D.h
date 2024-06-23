//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef T_MESH_SIMPLE3D_INCLUDED
#define T_MESH_SIMPLE3D_INCLUDED

#include "tMesh_Simple3D_Header.h"
#include "libs/Geometry/interface/tPoint.h"
#include "libs/Geometry/interface/tPoint3.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/Vector3F.h"
#include "common/TemplateMesh/VectorMesh.h"

class cESimple3D
{
public:

//    long edgeindex;

    cESimple3D()
    {
//        edgeindex = -1;
    }

    bool operator==(const cESimple3D& rhs) const;
    bool operator!=(const cESimple3D& rhs) const
    {
        return !((*this) == rhs);
    }
};
class cVSimple3D
{
public:

    tPoint3 _p;
//    long vertexindex;
    
    cVSimple3D()
    {
//        vertexindex = -1;
    }
    cVSimple3D(const tPoint3& p) :
        _p(p)
    {
//        vertexindex = -1;
    }

    bool operator==(const cVSimple3D &rhs) const;
    bool operator!=(const cVSimple3D &rhs) const
    {
        return !((*this) == rhs);
    }

    const tPoint3& getPoint() const
    {
        return _p;
    }
    cVector3F getPoint3F() const;
};

class cFSimple3D
{
    bool _external;

public:

//    long index;

    cFSimple3D()
    {
        _external = false;
//        index =-1;
    }

    bool operator==(const cFSimple3D &rhs) const;
    bool operator!=(const cFSimple3D &rhs) const
    {
        return !((*this) == rhs);
    }

    bool isExternal() const
    {
        return _external;
    }

    static cFSimple3D externalFace()
    {
        cFSimple3D result;
        result._external = true;
        return result;
    }
};

inline tPoint GetVertexPoint(const tMesh_Simple3D::edge e)
{
    tPoint3 p = e.vertex()->getPoint();
    return tPoint(p.getX(), p.getY(), p.getRange());
}
inline tPoint3 GetVertexPoint3(const tMesh_Simple3D::edge e)
{
    return e.vertex()->getPoint();
}
inline tPoint3 GetVertexPoint3(const tMesh_Simple3D::vertex v)
{
    return v->getPoint();
}
inline cVector3F GetVertexPoint3F(const tEdge_Simple3D e)
{
    return e.vertex()->getPoint3F();
}

//inline tPoint GetVertexPoint(const tMesh_3D::vertex v)
//{
//    return v->getPoint();
//}
inline tLine GetEdgeLine(tMesh_Simple3D::edge e)
{
    return tLine(GetVertexPoint(e), GetVertexPoint(e.next()));
}
//inline tPoint GetEdgeAxis(tMesh_3D::edge e)
//{
//    return GetVertexPoint(e.next())-GetVertexPoint(e);
//}
inline tPoint3 GetEdgeAxis3(tMesh_Simple3D::edge e)
{
    return GetVertexPoint3(e.next())-GetVertexPoint3(e);
}
//
//inline tPoint GetVertexPoint(const tMesh_3D_FaceTracked::edge e)
//{
//    return e.vertex()->getPoint();
//}
//inline tPoint GetVertexPoint(const tMesh_3D_FaceTracked::vertex v)
//{
//    return v->getPoint();
//}
//inline tLine GetEdgeLine(tMesh_3D_FaceTracked::edge e)
//{
//    return tLine(GetVertexPoint(e),GetVertexPoint(e.next()));
//}
//inline tPoint GetEdgeAxis(tMesh_3D_FaceTracked::edge e)
//{
//    return GetVertexPoint(e.next())-GetVertexPoint(e);
//}

#endif
